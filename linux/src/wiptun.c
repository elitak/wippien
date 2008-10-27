#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


const char * DOMAINSOCKET_NAME="/var/run/wippien.sock";

int prepare_domainsocket(void)
{

  int fd, result;
  struct sockaddr_un domaddr;
  socklen_t domaddr_len = sizeof(domaddr);
  struct stat domstat;

  fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (fd < 0)
  {
    perror("Unable to create UNIX socket");
    return 0;
  }


  // remove previous, if exists
  unlink(DOMAINSOCKET_NAME);

  domaddr.sun_family = AF_UNIX;
  strcpy(domaddr.sun_path, DOMAINSOCKET_NAME);
  result = bind(fd, (struct sockaddr *)&domaddr, domaddr_len);
  if (result < 0)
  {
    perror("Unable to bind UNIX socket");
    return 0;
  }

  result = listen(fd, 5);
  if (result < 0)
  {
    perror("Unable to listen UNIX socket");
    return 0;
  }

  // allow access to anyone  
  result = stat(DOMAINSOCKET_NAME, &domstat);
  if (result < 0)
  {
    perror("Unable to stat UNIX socket");
    return 0;
  }

  result = chmod(DOMAINSOCKET_NAME, domstat.st_mode | 511);
  if (result < 0)
  {
    perror("Unable to set UNIX socket to be writable");
    return 0;
  }

  return fd;
}

int tun_alloc(char *dev)
{
  struct ifreq ifr;
  int fd, err;

  if( (fd = open("/dev/net/tun", O_RDWR)) < 0 )
    return -1;  

  memset(&ifr, 0, sizeof(ifr));
  ifr.ifr_flags = IFF_TAP | IFF_NO_PI; 
  if(*dev)
    strncpy(ifr.ifr_name, dev, IFNAMSIZ);

  err = ioctl(fd, TUNSETIFF, (void *) &ifr);
  if(err < 0)
  {
    close(fd);
    return err;
  }
  
  strcpy(dev, ifr.ifr_name);  

//  unsigned long sock_opts = fcntl( fd, F_GETFL, 0 );
//  fcntl(fd, F_SETFL, sock_opts | O_NONBLOCK );

//    unsigned long sock_opts = fcntl(fd, F_GETFL, 0 );
//    sock_opts |= O_NONBLOCK;
//    sock_opts -= O_NONBLOCK;
//    fcntl(fd, F_SETFL, sock_opts);  
  return fd;
}  


int do_child(int client_sock, struct sockaddr_in *sockaddr)
{
  char buff[8192], buff2[8192];
  int result;
  
  struct in_addr IP, Netmask;
  
  // first we read IP/Netmask
  result = recv(client_sock, buff, sizeof(buff), 0);
//  printf("result = %d\n", result);
  if (result >= 2 + sizeof(unsigned long))
  {
    unsigned long *l = (unsigned long *)buff;
    IP.s_addr = *l;
    l++;
    Netmask.s_addr = *l;
    printf("Got IP %s\n", inet_ntoa(IP));
    printf("Got Netmask %s\n", inet_ntoa(Netmask));
    if (IP.s_addr && Netmask.s_addr)
    {
      memset(buff, 0, sizeof(buff));
      strcpy((unsigned char *)buff, "wip%d");
      int fd = tun_alloc(buff);
      if (fd>=0)
      {
        printf("Interface %s %d\n", buff, strlen(buff));
        result = sprintf(buff2, "ifconfig %s %s", buff, inet_ntoa(IP));
        sprintf(buff2 + result, " netmask %s", inet_ntoa(Netmask));
        system(buff2);
        
        // send out interface name
        send(client_sock, buff, strlen(buff)+1, 0);
        
        // now we loop through read/write and redirect data
        fd_set rset;
        FD_ZERO(&rset);
        int m = MAX(client_sock, fd) + 1;
        
//        unsigned long sock_opts = fcntl( client_sock, F_GETFL, 0 );
//        sock_opts |= O_NONBLOCK;
//        sock_opts -= O_NONBLOCK;
//        fcntl(client_sock, F_SETFL, sock_opts);
        do
        {
          FD_SET(fd, &rset);
          FD_SET(client_sock, &rset);
          
          result = select(m, &rset, NULL, NULL, NULL);
          if (result>0)
          {
//            printf("result = %d, max=%d, client_sock=%d, fd=%d\n", result, m, client_sock, fd);
            if (FD_ISSET(fd, &rset))
            {
//              printf("Got data from adapter\n");
              result = read(fd, buff, sizeof(buff));
              FD_CLR(fd, &rset);              
//              printf("Adapter data = %d\n", result);
              if (result>0)
                send(client_sock, buff, result, 0);
            }
            if (FD_ISSET(client_sock, &rset))
            {
//              printf("Got data from Wippien\n");
              result = recv(client_sock, buff, sizeof(buff), 0);
              FD_CLR(client_sock, &rset);       
//              printf("Wippien data = %d\n", result);
              if (result>0)
              {
                result = write(fd, buff, result);
//                printf("Sending data to adapter %d\n", result);
//                if (result<1)
//                  printf("couldn't send packet %d \n",errno);
              }
            }
          }
        } while (result>0);
      }
    }
  }
  
  printf("Leaving...\n");
  return 0;
}

void defunct_handler (int signum)
{
  int sig_status;
  /*int sig_pid = */waitpid (-1, &sig_status, WNOHANG);
}
  
int main(void)
{
  int client_sock, cli_len;   
  struct sockaddr_in cli_addr;
  int result;
    
  struct sigaction sa, osa;
  sa.sa_handler = defunct_handler;
  sigfillset (&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  sigaction (SIGCHLD, &sa, &osa);
  
  int domsocket = prepare_domainsocket();
  if (domsocket)
  {
    while (1)
    {
      cli_len = sizeof (cli_addr);
      bzero((char *) &cli_addr, sizeof (cli_addr));
      client_sock = accept(domsocket, (struct sockaddr *) &cli_addr, &cli_len);
      if (client_sock >= 0)
      {
        // accept and deal
        result = fork();
        if (result == 0)
        {
          close(domsocket);
          // child process
          do_child(client_sock, &cli_addr);
          close(client_sock);
          exit(0);
        }
        close(client_sock);
      }
    }
  }

  return -1;
}

