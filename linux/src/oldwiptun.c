#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <time.h>
#include <stdio.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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


int main(void)
{
  fd_set infds, testfds;
  int nfds = 0, minfds = 0, maxfds = 0, nready;
  struct timeval tv = { 15, 0 };
  int client_sock, cli_len;   
  struct sockaddr_in cli_addr, serv_addr;
  char buff[8192];
  int result;
    
  int domsocket = prepare_domainsocket();
  if (domsocket)
  {
    FD_ZERO(&infds);
    FD_SET(domsocket, &infds);
    maxfds = domsocket+1; 
    while (1)
    {
      testfds = infds;
      printf("1\r\n");
      if ((nready = select(maxfds + 1, &testfds, NULL, NULL, &tv)) == 0)
      {
        printf("2\r\n");
        // select timed out
        for (client_sock = minfds; client_sock && client_sock <= maxfds; client_sock++)
        {
          if (client_sock == domsocket)
            continue;
            
          if (FD_ISSET(client_sock, &infds))
          {
            FD_CLR(client_sock, &infds);
            close(client_sock);
          }
        }
        continue;
      }
      else if (nready > 0)
      {
        printf("3\r\n");
        // any new to accept?
        if (FD_ISSET(domsocket, &testfds))
        {
          printf("4\r\n");
          cli_len = sizeof (cli_addr);
          bzero((char *) &cli_addr, sizeof (cli_addr));
          client_sock = accept(domsocket, (struct sockaddr *) &cli_addr, &cli_len);
          if (client_sock < 0)
          {
            printf("5\r\n");
            // error 
            continue;
          }
          FD_SET(client_sock, &infds);
          maxfds = MAX(maxfds, client_sock);
          minfds = (minfds == 0) ? client_sock : MIN(minfds, client_sock);
          nready--;
        }
        printf("6\r\n");
        
        // anything to process?
        for (client_sock = minfds; nready && client_sock <= maxfds; client_sock++)
        {
          printf("7\r\n");
          if (FD_ISSET(client_sock, &testfds))
          {
            printf("8\r\n");
            result = recv(client_sock, buff, sizeof(buff), 0);
            printf("result=%d\n", result);
            if (result>0)
            {
              printf("9\r\n");
              // process what's been read

            }
          }
        }
      } else
        printf("4\r\n");
    }
  }

  return -1;
}

