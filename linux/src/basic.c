/* basic.c
** libstrophe XMPP client library -- basic usage example
**
** Copyright (C) 2005 OGG, LCC. All rights reserved.
**
**  This software is provided AS-IS with no warranty, either express
**  or implied.
**
**  This software is distributed under license and may not be copied,
**  modified or distributed except as expressly authorized under the
**  terms of the license contained in the file LICENSE.txt in this
**  distribution.
*/

#include <stdio.h>

#include "strophe.h"


int message_handler(xmpp_conn_t * const conn, xmpp_stanza_t * const stanza, void * const userdata) 
{ 
  xmpp_stanza_t *reply, *body, *text; 
  char *intext, *replytext; 
  xmpp_ctx_t *ctx = (xmpp_ctx_t*)userdata; 

  if(!xmpp_stanza_get_child_by_name(stanza, "body")) return 1; 

  intext = xmpp_stanza_get_text(xmpp_stanza_get_child_by_name(stanza, "body")); 

  printf("Incoming message from %s: %s\n", xmpp_stanza_get_attribute(stanza, "from"), intext); 

  reply = xmpp_stanza_new(ctx); 
  xmpp_stanza_set_name(reply, "message"); 
  xmpp_stanza_set_type(reply, xmpp_stanza_get_type(stanza)?xmpp_stanza_get_type(stanza):"chat"); 
  xmpp_stanza_set_attribute(reply, "to", xmpp_stanza_get_attribute(stanza, "from")); 

  body = xmpp_stanza_new(ctx); 
  xmpp_stanza_set_name(body, "body"); 

  replytext = malloc(strlen(" to you too!") + strlen(intext) + 1); 
  strcpy(replytext, intext); 
  strcat(replytext, " to you too!"); 

  text = xmpp_stanza_new(ctx); 
  xmpp_stanza_set_text(text, replytext); 
  xmpp_stanza_add_child(body, text); 
  xmpp_stanza_add_child(reply, body); 

  xmpp_send(conn, reply); 
  xmpp_stanza_release(reply); 
  free(replytext); 
  return 1; 
} 


/* define a handler for connection events */
void conn_handler(xmpp_conn_t * const conn, const xmpp_conn_event_t status, 
		  const int error, xmpp_stream_error_t * const stream_error,
		  void * const userdata)
{
    xmpp_ctx_t *ctx = (xmpp_ctx_t *)userdata;

    if (status == XMPP_CONN_CONNECT) 
    {
        xmpp_handler_add(conn,message_handler, NULL, "message", NULL, ctx);

//	fprintf(stderr, "DEBUG: connected\n");

	xmpp_stanza_t* pres;
	pres = xmpp_stanza_new(ctx); 
        xmpp_stanza_set_name(pres, "presence"); 
        xmpp_send(conn, pres); 
        xmpp_stanza_release(pres); 

//	xmpp_disconnect(conn);
    }
    else {
	fprintf(stderr, "DEBUG: disconnected\n");
//	xmpp_stop(ctx);
    }
}

int main(int argc, char **argv)
{
    xmpp_ctx_t *ctx;
    xmpp_conn_t *conn;
    xmpp_log_t *log;
    char *jid, *pass;
    char *server;

    /* take a jid and password on the command line,
       with optional server to connect to */
    if ((argc < 3) || (argc > 4)) {
	fprintf(stderr, "Usage: basic <jid> <pass> [<server>]\n\n");
	return 1;
    }
    
    jid = argv[1];
    pass = argv[2];
    server = NULL;
    /* Normally we pass NULL for the connection domain, in which case
       the library derives the target host from the jid, but we can
       override this for testing. */
    if (argc >= 4) server = argv[3];
    
    /* init library */
    xmpp_initialize();

    /* create a context */
//    log = xmpp_get_default_logger(XMPP_LEVEL_DEBUG); /* pass NULL instead to silence output */
//    log = xmpp_get_default_logger(NULL); /* pass NULL instead to silence output */
    ctx = xmpp_ctx_new(NULL, NULL);

    /* create a connection */
    conn = xmpp_conn_new(ctx);

    /* setup authentication information */
    xmpp_conn_set_jid(conn, jid);
    xmpp_conn_set_pass(conn, pass);

    /* initiate connection */
    xmpp_connect_client(conn, server, conn_handler, ctx);

    /* enter the event loop - 
       our connect handler will trigger an exit */
    xmpp_run(ctx);

    /* release our connection and context */
    xmpp_conn_release(conn);
    xmpp_ctx_free(ctx);

    /* final shutdown of the library */
    xmpp_shutdown();

    return 0;
}
