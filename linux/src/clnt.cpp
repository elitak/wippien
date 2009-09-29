#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <netdb.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <pthread.h>
#include <signal.h>
#include <openssl/bio.h>
#include <openssl/rsa.h>
#include <openssl/buffer.h>
#include <openssl/evp.h>

#include "strophe.h"
#include "buffer.h"
#include "proto.h"

#include "/home/kreso/weonlydo/wodVPN/code/unixVPN/unixVPN.h"
#include "/home/kreso/weonlydo/wodVPN/code/Win32LIB/Win32LIB.h"

const char * WIPPIENIM="WippienIM3";
const char *WIPPIENINITREQUEST = "WippienInitRequest";
const char *WIPPIENINITRESPONSE	= "WippienInitResponse";
const char *WIPPIENCONNECT = "WippienConnect";
const char *WIPPIENDISCONNECT = "WippienDisconnect";
const char *WIPPIENDETAILSTHREAD = "ExchangeDetailsThread";
const char *_hex="0123456789abcdef";

const char * DOMAINSOCKET_NAME="/var/run/wippien.sock";
const char * IPProviderHost = "www.wippien.com";
const char * IPProviderURI = "/ip/?jid=";
const char * MediatorHost = "mediator.wippien.com";
const int MediatorPort = 8000;

char *JID = (char *)"";
char *Password = (char *)"";

struct in_addr IP = {0}, Netmask = {0};
RSA *rsa = NULL;
unsigned char MAC[6] = {0};
VPNEventsStruct m_Events = {0};
int m_MsgAlive = 1;
BOOL m_Debug = 0;

int domsocket = -1;
xmpp_ctx_t *ctx = NULL;
xmpp_conn_t *conn = NULL;

typedef enum WippienState
{
	WipWaitingInitRequest,
		WipWaitingInitResponse,
		WipDisconnected,
		WipConnecting,
		WipNegotiating,
		WipConnected,
		WipUndefined
} WippienState;

typedef struct user
{
	unsigned char MAC[6], SharedKey[16], MyKey[16];
	struct in_addr IP;
	char JID[1024], MediatorHost[1024];
	int MediatorPort;
	RSA *rsa;
	WippienState LocalWippienState, RemoteWippienState;
	void *VPNHandle;
	int Connected;
} user;

#define MAXUSERS	100
user *USERLIST[MAXUSERS];
int TOTALUSERS = 0;

char *strlwr(char *string)
{
	char *s;
	
	if (string)
	{
		for (s = string; *s; ++s)
			*s = tolower(*s);
	}
	return string;
}

void ToBase64(const unsigned char *input, int length, unsigned char *output)
{
	BIO *bmem, *b64;
	BUF_MEM *bptr;
	
	b64 = BIO_new(BIO_f_base64());
	bmem = BIO_new(BIO_s_mem());
	b64 = BIO_push(b64, bmem);
	BIO_write(b64, input, length);
	BIO_flush(b64);
	BIO_get_mem_ptr(b64, &bptr);
	
	memcpy(output, bptr->data, bptr->length-1);
	output[bptr->length-1] = 0;
	
	BIO_free_all(b64);
}

int prepare_domainsocket(char *name)
{
	
	int fd, result;
	struct sockaddr_un domaddr;
	socklen_t domaddr_len = sizeof(domaddr);
	
	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd < 0)
	{
		perror("Unable to create UNIX socket");
		return -1;
	}
	
	
	// remove previous, if exists
	unlink(name);
	
	domaddr.sun_family = AF_UNIX;
	strcpy(domaddr.sun_path, name);
	result = bind(fd, (struct sockaddr *)&domaddr, domaddr_len);
	if (result < 0)
	{
		perror("Unable to bind UNIX socket");
		return -1;
	}
	
	result = listen(fd, 5);
	if (result < 0)
	{
		perror("Unable to listen UNIX socket");
		return -1;
	}
	
	return fd;
}

int connect_domainsocket(char *name)
{
	int fd, result;
	struct sockaddr_un domaddr;
	socklen_t domaddr_len = sizeof(domaddr);
	
	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd < 0)
	{
		perror("Unable to create UNIX socket");
		return -1;
	}
	
	domaddr.sun_family = AF_UNIX;
	strcpy(domaddr.sun_path, name);
	result = connect(fd, (struct sockaddr *)&domaddr, domaddr_len);
	if (result < 0)
	{
		perror("Unable to connect to UNIX socket");
		return -1;
	}
	
	return fd;
}

char *parse_provider_response(char *buff, char *text)
{
	char b[1024];
	
	sprintf(b, "<%s>", text);
	char *a = strstr(buff, b);
	if (!a)
		return NULL;
	a += strlen(text)+2;
	sprintf(b, "</%s>", text);
	char *c = strstr(a, b);
	if (!c)
		return NULL;
	*c = 0;
	return a;
}

int connect_getip(void)
{
	int fd;
	struct hostent *record;
	struct in_addr *addr;
	struct sockaddr_in saddr;
	int result;
	char buff[32768], buffcopy[32768];
	
	record = gethostbyname(IPProviderHost);
	if (!record)
	{
		perror("Unable to resolve IPProviderHost");
		return 0;
	}
	
	addr = (struct in_addr *) record->h_addr;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
	{
		perror("Unable to create outgoing socket");
		return 0;
	}
	
	saddr.sin_family = AF_INET;
	saddr.sin_addr = *addr;
	saddr.sin_port = htons(80);
	result = connect(fd, (struct sockaddr *) &saddr, sizeof(saddr));
	if (result < 0)
	{
		perror("Unable to connect to IP Provider host");
		return 0;
	}
    
	sprintf(buff, "GET %s%s HTTP/1.0\r\nHost: %s\r\nConnection: close\r\n\r\n", IPProviderURI, JID, IPProviderHost);
	result = send(fd, buff, strlen(buff), 0);
	if (result < 0)
	{
		perror("Unable to send request to IP provider host");
		return 0;
	}
    
	do
	{
		result = recv(fd, buff, sizeof(buff), 0);
		if (result > 0)
		{
			char *res;
			if (!IP.s_addr)
			{
				memcpy(buffcopy, buff, result);
				res = parse_provider_response(buffcopy, (char *)"VirtualIP");
				if (res)
					IP.s_addr = inet_addr(res);
			}
			if (!Netmask.s_addr)
			{
				memcpy(buffcopy, buff, result);
				res = parse_provider_response(buffcopy, (char *)"VirtualMask");
				if (res)
					Netmask.s_addr = inet_addr(res);
			}
		}
	} while (result>0);
	close(fd);
	
	return 1;    
}

void ToHex(WIPNUX::Buffer *in, WIPNUX::Buffer *out)
{
	while (buffer_len(in))
	{
		char *a = (char *)buffer_ptr(in);
		unsigned char b = (unsigned char)*a;
		buffer_append(out, &_hex[b/16],1);
		buffer_append(out, &_hex[b%16],1);
		buffer_consume(in, 1);
	}
	buffer_clear(in);
}

void FromHex(WIPNUX::Buffer *in, WIPNUX::Buffer *out)
{
	char buff[3];
	buff[2] = 0;
	
	while (buffer_len(in))
	{
		if (buffer_len(in)<2)
			return;
		
		memcpy(buff, buffer_ptr(in), 2);
		strlwr(buff);
		
		char *s1, *s2;
		s1 = strchr(_hex, buff[0]);
		s2 = strchr(_hex, buff[1]);
		
		if (!s1 || !s2)
			return;
		
		buff[0] = (s1 - _hex)*16 + (s2-_hex);
		buffer_append(out,buff,1);
		buffer_consume(in,2);
	}
}


void exchangeWippienDetails(user *u, char *subj, char *msgtext, int msglen)
{
    xmpp_stanza_t *msg = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(msg, "message");
    xmpp_stanza_set_type(msg, "headline");
    xmpp_stanza_set_attribute(msg, "to", u->JID);
    
    xmpp_stanza_t *body = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(body, "body");
    xmpp_stanza_t *t1 = xmpp_stanza_new(ctx);
	
    WIPNUX::Buffer bin, bout;
    buffer_init(&bin);
    buffer_init(&bout);
    buffer_append(&bin, msgtext, msglen);
    ToHex(&bin, &bout);
    buffer_free(&bin);
    buffer_append(&bout, "\0",1);    
    xmpp_stanza_set_text(t1, (char *)buffer_ptr(&bout));
    buffer_free(&bout);
    xmpp_stanza_add_child(body, t1);
    
    xmpp_stanza_t *subject = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(subject, "subject");
    xmpp_stanza_t *t2 = xmpp_stanza_new(ctx);
    xmpp_stanza_set_text(t2, subj);
    xmpp_stanza_add_child(subject, t2);
	
    xmpp_stanza_t *thread = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(thread, "thread");
    xmpp_stanza_t *t3 = xmpp_stanza_new(ctx);
    xmpp_stanza_set_text(t3, WIPPIENDETAILSTHREAD);
    xmpp_stanza_add_child(thread, t3);
	
    xmpp_stanza_add_child(msg, thread);
    xmpp_stanza_add_child(msg, subject);
    xmpp_stanza_add_child(msg, body);
    
    xmpp_send(conn, msg);
    xmpp_stanza_release(msg);
}

long EventStateChange(void *wodVPN, StatesEnum OldState)
{
	StatesEnum st;
	VPN_GetState(wodVPN, &st);
	char buff[1024];
	int len = sizeof(buff);
	VPN_GetStateText(wodVPN, st, buff, &len);
	return 1;
}

long EventConnected(void *wodVPN, char * PeerID, char * IP, long Port)
{
	printf("VPN Connected\n");
	int i;
	
	for (i=0;i<TOTALUSERS;i++)
	{
		if (wodVPN == USERLIST[i]->VPNHandle)
		{
			USERLIST[i]->Connected = 1;
			break;
		}
	}
	return 1;
}

long EventDisconnected(void *wodVPN, long ErrorCode, char * ErrorText)
{
	printf("VPN Disconnected\n");
	int i;
	
	for (i=0;i<TOTALUSERS;i++)
	{
		if (wodVPN == USERLIST[i]->VPNHandle)
		{
			USERLIST[i]->Connected = 0;
			break;
		}
	}
	return 1;
}

long EventSearchDone(void *wodVPN, char * IP, long Port, long ErrorCode, char * ErrorText)
{
	printf("VPN SearchDone\n");
	
	if (!ErrorCode)
	{
		VPN_Connect(wodVPN, IP, Port);
	}
	else
	{   
		printf("Search error: %s\n", ErrorText);
	}
	return 1;
}


long EventIncomingData(void *wodVPN, void *Data, int Len)
{
	// send to socket
//	ETH_HEADER *ethr = (ETH_HEADER *) Data;
//	printf("INCOMING %x packet from %x:%x:%x:%x:%x:%x to %x:%x:%x:%x:%x:%x\n", ethr->proto, ethr->src[0], ethr->src[1], ethr->src[2], ethr->src[3], ethr->src[4], ethr->src[5], ethr->dest[0], ethr->dest[1], ethr->dest[2], ethr->dest[3], ethr->dest[4], ethr->dest[5]); 
	/*int result = */send(domsocket, Data, Len, 0);
	return 1;
}

void checkWippienState(user *u)
{
	if (!u->rsa || u->RemoteWippienState < WipWaitingInitResponse || u->LocalWippienState < WipWaitingInitResponse)
	{
		WIPNUX::Buffer b;
		buffer_init(&b);
		buffer_put_int(&b, IP.s_addr);
		buffer_append(&b, MAC, sizeof(MAC));
		buffer_put_bignum2(&b, rsa->e);
		buffer_put_bignum2(&b, rsa->n);
		buffer_put_char(&b, (char)u->LocalWippienState);
		
		WIPNUX::Buffer b1;
		buffer_init(&b1);
		buffer_put_cstring(&b1, "mediator.wippien.com");
		buffer_put_int(&b1, 8000);
		buffer_put_string(&b, (char *)buffer_ptr(&b1), buffer_len(&b1));
		buffer_free(&b1);
		
		exchangeWippienDetails(u, (char *)WIPPIENINITREQUEST, (char *)buffer_ptr(&b), buffer_len(&b));    
		buffer_free(&b);
		printf("Sending WippienInitRequest to %s\n", u->JID);
		return;
	}
	else
		if (u->RemoteWippienState < WipDisconnected)
		{
			if (u->rsa)
			{
				WIPNUX::Buffer b;
				char src[128], dst[128];
				memcpy(src+24, u->MyKey, 16);
				RSA_public_encrypt(128 - 11, (unsigned char *)src, (unsigned char *)dst, u->rsa, 1);
				buffer_init(&b);
				buffer_append(&b, dst, 128);
				buffer_put_char(&b, (char)u->LocalWippienState);
				buffer_put_string(&b, MediatorHost, 20);
				buffer_put_int(&b, MediatorPort);
				buffer_put_int(&b, 0); 
				
				exchangeWippienDetails(u, (char *)WIPPIENINITRESPONSE, (char *)buffer_ptr(&b), buffer_len(&b));    
				buffer_free(&b);
				printf("Sending WippienInitResponse to %s\n", u->JID);
				return;
			}
		}
		else
			if (u->RemoteWippienState < WipConnecting)
			{
				WIPNUX::Buffer b, b2;
				buffer_init(&b2);
				buffer_put_char(&b2, (char)u->LocalWippienState);
				buffer_init(&b);
				ToHex(&b2, &b);
				buffer_free(&b2);
				
				exchangeWippienDetails(u, (char *)WIPPIENCONNECT, (char *)buffer_ptr(&b), buffer_len(&b));    
				buffer_free(&b);
				printf("Sending WippienConnect to %s\n", u->JID);
				return;
			}
			
			// let's try to connect
			if (u->LocalWippienState < WipConnecting)
			{
				char out[1024], myid[1024], hisid[1024];
				VPN_Stop(u->VPNHandle);
				strcpy(out, u->JID);
				char *slash = strchr(out, '/');
				if (slash)
					*slash = 0;
				sprintf(myid, "%s_%s", JID, out);
				strlwr(myid);
				VPN_SetMyID(u->VPNHandle, myid);
				
				sprintf(hisid, "%s_%s", out, JID);
				strlwr(hisid);
				
				VPN_SetRetryCount(u->VPNHandle, 10);
				VPN_SetRetryWait(u->VPNHandle, 1000);
				VPN_SetThreads(u->VPNHandle, TRUE);
				ToBase64((unsigned char *)u->SharedKey, 16, (unsigned char *)out);  
//				printf("Using password %s\n", out);
				VPN_SetPassword(u->VPNHandle, out);
				VPN_Start(u->VPNHandle, 0);
				
				
				VARIANT arg1, arg2, arg3;
				arg1.vt = VT_BSTR;
				arg2.vt = VT_I2;
				arg3.vt = VT_ERROR;
				arg1.bstrVal = SysAllocStringByte((char *)u->MediatorHost);
				arg2.iVal = u->MediatorPort;
				
				VPN_Search(u->VPNHandle, (SearchEnum)/*SrchUDPSingle*/0, hisid, arg1, arg2, arg3);
				
			}
}

user *getWippienUser(char *from)
{
//	int iswippien = 0;
	int i;
	
	//printf("checking if %s is wippien\n", from);
	for (i=0;i<TOTALUSERS;i++)
	{
		if (!strcmp(USERLIST[i]->JID, from))
			return USERLIST[i];
		
	}
	
	return NULL;
}


user *isWippien(char *from, xmpp_stanza_t *stanza)
{
	int iswippien = 0;
	int i;
	
	//printf("checking if %s is wippien\n", from);
	for (i=0;i<TOTALUSERS;i++)
	{
		if (!strcmp(USERLIST[i]->JID, from))
			return USERLIST[i];
		
	}
    
	// is he wippien capable?
	xmpp_stanza_t *c = xmpp_stanza_get_child_by_name(stanza, "c");  
	if (c)
	{
		char *ext = xmpp_stanza_get_attribute(stanza, "ext");
		if (ext && strstr(ext, WIPPIENIM))
			iswippien = 1;
	}
	
	if (!iswippien)
	{
		char *d = strchr(from, '/');
		if (d)
		{
			d++;
			if (!strcmp(d, WIPPIENIM))
				iswippien = 1;
		}
	}
	
	if (iswippien)
	{
		//printf("%s is wippien\n", from);
		if (TOTALUSERS < MAXUSERS)
		{
			user *u = (user *)malloc(sizeof(user));
			memset(u, 0, sizeof(user));
			
			strcpy(u->JID, from);
			u->IP.s_addr = 0;
			u->LocalWippienState = (WippienState)WipWaitingInitRequest;
			u->RemoteWippienState = (WippienState)WipWaitingInitRequest;
			u->rsa = NULL;
			memset(u->SharedKey, 0, 16);
			strcpy(u->MediatorHost, MediatorHost);
			u->MediatorPort = MediatorPort;
			u->VPNHandle = _VPN_Create(&m_Events);
			
			
			USERLIST[TOTALUSERS++] = u;
			//printf("%s added to collection, total %d\n", from, TOTALUSERS);
			return u;
		}
	}
	
	
	return NULL;
}

int message_handler(xmpp_conn_t * const conn, xmpp_stanza_t * const stanza, void * const userdata)
{
	char *from = xmpp_stanza_get_attribute(stanza, "from");
//	printf("Got message from %s\n", from);
	user *u = getWippienUser(from);
	if (u)
	{
		xmpp_stanza_t *thread = xmpp_stanza_get_child_by_name(stanza, "thread");
		if (thread)
		{
			char *a = xmpp_stanza_get_text(thread);    
			if (a && !strcmp(a, WIPPIENDETAILSTHREAD))
			{	
				xmpp_stanza_t *body = xmpp_stanza_get_child_by_name(stanza, "body");
				if (body)
				{
					WIPNUX::Buffer b1, b2;		
					char *t = xmpp_stanza_get_text(body);
					if (t)
					{
						buffer_init(&b1);
						buffer_append(&b1, t, strlen(t));
						buffer_init(&b2);
						FromHex(&b1, &b2);
						buffer_free(&b1);
						
						xmpp_stanza_t *subj = xmpp_stanza_get_child_by_name(stanza, "subject");
						if (subj)
						{
							char *b = xmpp_stanza_get_text(subj);
							if (b)
							{	
								if (!strcmp(b, WIPPIENINITREQUEST))
								{
									printf("Got WippienDisconnect from %s\n", from);
									u->IP.s_addr = buffer_get_int(&b2);
									memcpy(u->MAC, buffer_ptr(&b2), 6);
									buffer_consume(&b2, 6);
									
									if (u->rsa)
										RSA_free(u->rsa);
									u->rsa = RSA_new();
									u->rsa->e = BN_new();
									u->rsa->n = BN_new();
									buffer_get_bignum2(&b2, u->rsa->e);
									buffer_get_bignum2(&b2, u->rsa->n);
									u->RemoteWippienState = (WippienState)buffer_get_char(&b2);
									u->LocalWippienState = (WippienState)WipWaitingInitResponse;
									
									
									checkWippienState(u);
								}
								else
									if (!strcmp(b, WIPPIENINITRESPONSE))
									{
										printf("Got WippienInitResponse from %s\n", from);
										char src[1024], dst[1024];
										memcpy(src, buffer_ptr(&b2), 128);
										if (RSA_private_decrypt(128, (unsigned char *)src, (unsigned char *)dst, rsa, 1) < 0)
										{
											printf("Failed to decrypt data from %s\n", u->JID);
											u->LocalWippienState = (WippienState)WipDisconnected;
											return 1;
										}
										
										buffer_consume(&b2, 128);
										u->RemoteWippienState = (WippienState)buffer_get_char(&b2);
										if (buffer_len(&b2))
										{
											char *med = (char *)buffer_get_string_ret(&b2, NULL);
											if (med)
											{
												int port = buffer_get_int(&b2);
												if (port)
												{
													strcpy(u->MediatorHost, med);
													u->MediatorPort = port;
												}
												free(med);
											}
										}
										
										// now let's XOR data
										int i;
										for (i=0;i<16;i++)
											u->SharedKey[i] = u->MyKey[i] ^ dst[i+24];
										
										checkWippienState(u);
									}
									else
										if (!strcmp(b, WIPPIENCONNECT))
										{
											printf("Got WippienConnect from %s\n", from);
											u->RemoteWippienState = (WippienState)buffer_get_char(&b2);
											checkWippienState(u);
										}
										else
											if (!strcmp(b, WIPPIENDISCONNECT))
											{
												printf("Got WippienDisconnect from %s\n", from);
											}
							}
						}
					}
					buffer_free(&b2);
				}
			}
		} 
  }
  return 1;
  
}

int presence_handler(xmpp_conn_t * const conn, xmpp_stanza_t * const stanza, void * const userdata)
{
	char *from = xmpp_stanza_get_attribute(stanza, "from");
	char *type = xmpp_stanza_get_attribute(stanza, "type");
	int isonline = 1;
	
	if (type && !strcasecmp(type, "unavailable"))
		isonline = 0;
    
	user *u = isWippien(from, stanza);
	if (u)
	{
		printf("Got presence from %s, he is now %s\n", from, isonline?"online":"offline");
		if (isonline)
			checkWippienState(u);
		
	}
    
    
	return 1;  
}


void myxmpp_conn_handler(xmpp_conn_t * const conn, const xmpp_conn_event_t status,const int error, xmpp_stream_error_t * const stream_error, void * const userdata) 
{
	xmpp_ctx_t *ctx = (xmpp_ctx_t *)userdata;
	
	if (status == XMPP_CONN_CONNECT) 
	{
		
		printf("Connected to XMPP server\n");
		xmpp_handler_add(conn,message_handler, NULL, "message", NULL, ctx);
		xmpp_handler_add(conn,presence_handler, NULL, "presence", NULL, ctx);
		
		xmpp_stanza_t* pres;  
		pres = xmpp_stanza_new(ctx); 
		xmpp_stanza_set_name(pres, "presence");
		xmpp_send(conn, pres); 
		xmpp_stanza_release(pres);
	}
	else
	{
		printf("Disconnected from XMPP server\n");
		xmpp_stop(ctx);  
	}
	
}

int connect_xmpp(void)
{
	xmpp_initialize();
	xmpp_log_t *log = 0;
	if (m_Debug)
	    log = xmpp_get_default_logger(XMPP_LEVEL_DEBUG);
	ctx = xmpp_ctx_new(NULL, log);
	conn = xmpp_conn_new(ctx);
	char buff[1024];
	sprintf(buff, JID);
	strcat(buff, "/WIPPIENIM3");
	xmpp_conn_set_jid(conn, buff);
	char *j = strchr(JID, '@');
	if (j)
	{
		xmpp_conn_set_pass(conn, Password);
		xmpp_connect_client(conn, j+1, myxmpp_conn_handler, ctx);
		xmpp_run(ctx);
		
		
		
		xmpp_conn_release(conn);
		xmpp_ctx_free(ctx);
		xmpp_shutdown();
		return 1;
	}
	
	return 0;
}

void *MsgProc(void *lpParam)
{
    while (m_MsgAlive)
    {
        WaitMessage();   
        if (m_MsgAlive)  
        {
            MSG m;
            while (PeekMessage(&m, 0, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&m);
                DispatchMessage(&m);
            }
        }
    }	
    return NULL;
}

#define MAC_EQUAL(a,b)      (memcmp ((a), (b), sizeof (MACADDR)) == 0)
#define COPY_MAC(dest, src) (memcpy((dest), (src), sizeof(MACADDR)))
unsigned char MAC_BROADCAST[6] = {255,255,255,255,255,255};
void *PumpProc(void *lpParam)
{
	unsigned char packet[32768];
	fd_set rset;
	int m = domsocket + 1;
	int result, i;
	
	FD_ZERO(&rset);
	do
	{
		FD_SET(domsocket, &rset);
		
		result = select(m, &rset, NULL, NULL, NULL);
		if (result>0)
		{
			if (FD_ISSET(domsocket, &rset))
			{
				result = recv(domsocket, packet, sizeof(packet), 0);
				FD_CLR(domsocket, &rset);
//				printf("got %d bytes\n", result);
				if (result>0)
				{
					ETH_HEADER *ethr = (ETH_HEADER *) packet;
//					printf("raw data (protocol %x):\n", ethr->proto);
					//					for (i=0;i<result;i++)
					//						printf("%x:", packet[i]);
					//					printf("\n");
					
					if (ethr->proto == 0x608/*ETH_P_ARP*/)
					{
//						printf("Got ARP packet from ethernet\n");
						ARP_PACKET *p = (ARP_PACKET *)packet;
						// let's see if this is DHCP request
						if (result == sizeof(ARP_PACKET) && p->m_PROTO_AddressType==8 && p->m_ARP_Operation == 256)
						{
							for (i=0;i<TOTALUSERS;i++)
							{
								user *u = USERLIST[i];
//								if (!u->Connected)
//									continue; // skip this user
								if (p->m_Proto == htons(ETH_P_ARP)
									&& MAC_EQUAL(p->m_MAC_Source, MAC)
									&& MAC_EQUAL(p->m_ARP_MAC_Source, MAC)
									&& MAC_EQUAL(p->m_MAC_Destination, MAC_BROADCAST)
									&& p->m_ARP_Operation == htons(ARP_REQUEST)
									&& p->m_MAC_AddressType == htons(MAC_ADDR_TYPE)
									&& p->m_MAC_AddressSize == sizeof(MACADDR)
									&& p->m_PROTO_AddressType == htons(ETH_P_IP)
									&& p->m_PROTO_AddressSize == sizeof(IPADDR)
									&& p->m_ARP_IP_Source == IP.s_addr //htonl?
									&& p->m_ARP_IP_Destination == u->IP.s_addr) //htonl?
								{
									ARP_PACKET arp;
									//----------------------------------------------
									// Initialize ARP reply fields
									//----------------------------------------------
									arp.m_Proto = htons(ETH_P_ARP);
									arp.m_MAC_AddressType = htons(MAC_ADDR_TYPE);
									arp.m_PROTO_AddressType = htons(ETH_P_IP);
									arp.m_MAC_AddressSize = sizeof(MACADDR);
									arp.m_PROTO_AddressSize = sizeof(IPADDR);
									arp.m_ARP_Operation = htons(ARP_REPLY);
									
									//----------------------------------------------
									// ARP addresses
									//----------------------------------------------      
									COPY_MAC(arp.m_MAC_Source, u->MAC);
									COPY_MAC(arp.m_MAC_Destination, MAC);
									COPY_MAC(arp.m_ARP_MAC_Source, u->MAC);
									COPY_MAC(arp.m_ARP_MAC_Destination, MAC);
									arp.m_ARP_IP_Source = u->IP.s_addr; //htonl?
									arp.m_ARP_IP_Destination = IP.s_addr; //htonl?;
									send(domsocket, (char *)&arp, sizeof(ARP_PACKET), 0);
//									printf("Sending back ASP request %d\n", send(domsocket, (char *)&arp, sizeof(ARP_PACKET), 0)); // send back to ETH
								}
								
							}
						}
					}
					else
					if (ethr->proto == 0x8/*ETH_P_IP*/)
					{
//						printf("Got IP packet from ethernet\n");
//						printf("Got %x packet from %x:%x:%x:%x:%x:%x to %x:%x:%x:%x:%x:%x\n", ethr->proto, ethr->src[0], ethr->src[1], ethr->src[2], ethr->src[3], ethr->src[4], ethr->src[5], ethr->dest[0], ethr->dest[1], ethr->dest[2], ethr->dest[3], ethr->dest[4], ethr->dest[5]); 
						for (i=0;i<TOTALUSERS;i++)
						{
							//user *u = USERLIST[i];
							if (USERLIST[i]->Connected && (MAC_EQUAL(USERLIST[i]->MAC, ethr->dest) || MAC_EQUAL(MAC_BROADCAST, ethr->dest)))
							{
//								printf("Sending data to user\n");
								VPN_SendData(USERLIST[i]->VPNHandle, packet, result, 0);
							}
						}
					}						
				}
//				printf("domsocket set\n");
            }
		}
	} while (result>0);
	
	m_MsgAlive = 0;
	printf("server gone\n");
	return 0;
}

void ReadMAC(char *dev)
{
	int fd;
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(struct ifreq));
	
	
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, dev, strlen(dev));
	
	ioctl(fd, SIOCGIFHWADDR, &ifr);
	
	close(fd);

	memcpy(MAC, ifr.ifr_hwaddr.sa_data, sizeof(MAC));
	
	/* display result */
	printf("MAC %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",
		(unsigned char)ifr.ifr_hwaddr.sa_data[0],
		(unsigned char)ifr.ifr_hwaddr.sa_data[1],
		(unsigned char)ifr.ifr_hwaddr.sa_data[2],
		(unsigned char)ifr.ifr_hwaddr.sa_data[3],
		(unsigned char)ifr.ifr_hwaddr.sa_data[4],
		(unsigned char)ifr.ifr_hwaddr.sa_data[5]);
	
}

int main(int argc, char **argv)
{
	char *u = NULL;
	char *p = NULL;
	
	int c = 1;
	while (c < argc)
	{
		if (argv[c][0] == '-' || argv[c][0] == '/')
		{
			if (tolower(argv[c][1] == 'd'))
				m_Debug = 1;
		}
		else
		if (!u)
			u = argv[c];
		else
		if (!p)
			p = argv[c];
		
		c++;		
	}
	
	if (!u)
	{
		printf("Please start with wipclnt JID Password\n");
		return -1;
	}
	JID = u;
	if (p)
	{
		Password = (char *)p;
	}
	else
	{
		printf("Please enter password for %s: ", JID);
		Password = (char *)malloc(1024);
		memset(Password, 0, 1024);
		scanf("%s", Password);
	}
	
	_VPN_LibInit(0);
	
	// set up initial wodVPN events
	memset(&m_Events, 0, sizeof(m_Events));
	m_Events.StateChange = EventStateChange;
	m_Events.Connected = EventConnected;
	m_Events.Disconnected = EventDisconnected;
	m_Events.SearchDone = EventSearchDone;
	m_Events.IncomingData = EventIncomingData;
	
//	sigset_t sigset;
//	sigemptyset(&sigset);
//	sigaddset(&sigset, SIGRTMIN);
//	sigprocmask(SIG_BLOCK, &sigset, NULL);
	
	if (!IP.s_addr || !Netmask.s_addr)
		if (!connect_getip())
			return -1;
		
		printf("JID %s\n", JID);
		printf("IP %s\n", inet_ntoa(IP));
		printf("Netmask %s\n", inet_ntoa(Netmask));
		
		domsocket = connect_domainsocket((char *)DOMAINSOCKET_NAME);
		if (domsocket>=0)
		{
			// once connected, send out IP/Mask, wait for BOOL
			char buff[8192];
			memcpy(buff, &IP.s_addr, sizeof(IP.s_addr));
			memcpy(buff + sizeof(IP.s_addr), &Netmask.s_addr, sizeof(Netmask.s_addr));
			send(domsocket, buff, sizeof(IP.s_addr)+sizeof(Netmask.s_addr), 0);
			if (recv(domsocket, buff, sizeof(buff), 0) > 0)
			{
				ReadMAC(buff);
				// fire up thread for the pump
				pthread_attr_t threadAttr;
				// initialize the thread attribute
				pthread_attr_init(&threadAttr);
				// Set the stack size of the thread
				pthread_attr_setstacksize(&threadAttr, 120*1024);
				// Set thread to detached state. No need for pthread_join
				pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);
				// Create the threads
				pthread_t htr1, htr2;
				pthread_create(&htr1, &threadAttr, PumpProc, NULL);
				pthread_create(&htr2, &threadAttr, MsgProc, NULL);
				// Destroy the thread attributes
				pthread_attr_destroy(&threadAttr);
//				printf("Data pump initiated\n");
				
				// create rsa key
				rsa = RSA_generate_key(1024,35,NULL,NULL);
				
				// and connect XMPP
				connect_xmpp();
			}
			
			
		}
		_VPN_LibDeinit();
		
		return -1;
}

