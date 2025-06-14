/**
 * @file
 * Sequential API Internal module
 *
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

#include "lwip/include/lwip/opt.h"

#if LWIP_NETCONN /* don't build if not configured for use in lwipopts.h */
#include <lwip/utility/trace.h>
#include <lwip/utility/assert.h>
#include "lwip/include/lwip/api_msg.h"

#include "lwip/include/ipv4/lwip/ip.h"
#include "lwip/include/lwip/udp.h"
#include "lwip/include/lwip/tcp.h"
#include "lwip/include/lwip/raw.h"

#include "lwip/include/lwip/memp.h"
#include "lwip/include/lwip/tcpip.h"
#include "lwip/include/ipv4/lwip/igmp.h"
#include "lwip/include/lwip/dns.h"

#include <string.h>

/* forward declarations */
#if LWIP_TCP
static err_t do_writemore(struct netconn *conn);
static void do_close_internal(struct netconn *conn);
#endif

#if LWIP_RAW
/**
 * Receive callback function for RAW netconns.
 * Doesn't 'eat' the packet, only references it and sends it to
 * conn->recvmbox
 *
 * @see raw.h (struct raw_pcb.recv) for parameters and return value
 */
static u8_t
recv_raw(void *arg, struct raw_pcb *pcb, struct pbuf *p,
    struct ip_addr *addr)
{
  struct pbuf *q;
  struct netbuf *buf;
  struct netconn *conn;
#if LWIP_SO_RCVBUF
  int recv_avail;
#endif /* LWIP_SO_RCVBUF */

  LWIP_UNUSED_ARG(addr);
  conn = arg;

#if LWIP_SO_RCVBUF
  SYS_ARCH_GET(conn->recv_avail, recv_avail);
  if ((conn != NULL) && (conn->recvmbox != SYS_MBOX_NULL) &&
      ((recv_avail + (int)(p->tot_len)) <= conn->recv_bufsize)) {
#else  /* LWIP_SO_RCVBUF */
  if ((conn != NULL) && (conn->recvmbox != SYS_MBOX_NULL)) {
#endif /* LWIP_SO_RCVBUF */
    /* copy the whole packet into new pbufs */
    q = pbuf_alloc(PBUF_RAW, p->tot_len, PBUF_RAM);
    if(q != NULL) {
      if (pbuf_copy(q, p) != ERR_OK) {
        pbuf_free(q);
        q = NULL;
      }
    }

    if(q != NULL) {
      buf = memp_malloc(MEMP_NETBUF);
      if (buf == NULL) {
        pbuf_free(q);
        return 0;
      }

      buf->p = q;
      buf->ptr = q;
      buf->addr = &(((struct ip_hdr*)(q->payload))->src);
      buf->port = pcb->protocol;

      if (sys_mbox_trypost(conn->recvmbox, buf) != ERR_OK) {
        netbuf_delete(buf);
        return 0;
      } else {
        SYS_ARCH_INC(conn->recv_avail, q->tot_len);
        /* Register event with callback */
        API_EVENT(conn, NETCONN_EVT_RCVPLUS, q->tot_len);
      }
    }
  }

  return 0; /* do not eat the packet */
}
#endif /* LWIP_RAW*/

#if LWIP_UDP
/**
 * Receive callback function for UDP netconns.
 * Posts the packet to conn->recvmbox or deletes it on memory error.
 *
 * @see udp.h (struct udp_pcb.recv) for parameters
 */
static void
recv_udp(void *arg, struct udp_pcb *pcb, struct pbuf *p,
   struct ip_addr *addr, u16_t port)
{
  struct netbuf *buf;
  struct netconn *conn;
#if LWIP_SO_RCVBUF
  int recv_avail;
#endif /* LWIP_SO_RCVBUF */

  LWIP_UNUSED_ARG(pcb); /* only used for asserts... */
  LWIP_ASSERT("recv_udp must have a pcb argument", pcb != NULL);
  LWIP_ASSERT("recv_udp must have an argument", arg != NULL);
  conn = arg;
  LWIP_ASSERT("recv_udp: recv for wrong pcb!", conn->pcb.udp == pcb);

#if LWIP_SO_RCVBUF
  SYS_ARCH_GET(conn->recv_avail, recv_avail);
  if ((conn == NULL) || (conn->recvmbox == SYS_MBOX_NULL) ||
      ((recv_avail + (int)(p->tot_len)) > conn->recv_bufsize)) {
#else  /* LWIP_SO_RCVBUF */
  if ((conn == NULL) || (conn->recvmbox == SYS_MBOX_NULL)) {
#endif /* LWIP_SO_RCVBUF */
    pbuf_free(p);
    return;
  }

  buf = memp_malloc(MEMP_NETBUF);
  if (buf == NULL) {
    pbuf_free(p);
    return;
  } else {
    buf->p = p;
    buf->ptr = p;
    buf->addr = addr;
    buf->port = port;
#if LWIP_NETBUF_RECVINFO
    {
      const struct ip_hdr* iphdr = ip_current_header();
      /* get the UDP header - always in the first pbuf, ensured by udp_input */
      const struct udp_hdr* udphdr = (void*)(((char*)iphdr) + IPH_LEN(iphdr));
      buf->toaddr = (struct ip_addr*)&iphdr->dest;
      buf->toport = udphdr->dest;
    }
#endif /* LWIP_NETBUF_RECVINFO */
  }

  if (sys_mbox_trypost(conn->recvmbox, buf) != ERR_OK) {
    netbuf_delete(buf);
    return;
  } else {
    SYS_ARCH_INC(conn->recv_avail, p->tot_len);
    /* Register event with callback */
    API_EVENT(conn, NETCONN_EVT_RCVPLUS, p->tot_len);
  }
}
#endif /* LWIP_UDP */

#if LWIP_TCP
/**
 * Receive callback function for TCP netconns.
 * Posts the packet to conn->recvmbox, but doesn't delete it on errors.
 *
 * @see tcp.h (struct tcp_pcb.recv) for parameters and return value
 */
static err_t
recv_tcp(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
  struct netconn *conn;
  u16_t len;

  LWIP_UNUSED_ARG(pcb);
  LWIP_ASSERT("recv_tcp must have a pcb argument", pcb != NULL);
  LWIP_ASSERT("recv_tcp must have an argument", arg != NULL);
  conn = arg;
  LWIP_ASSERT("recv_tcp: recv for wrong pcb!", conn->pcb.tcp == pcb);

  if ((conn == NULL) || (conn->recvmbox == SYS_MBOX_NULL)) {
    return ERR_VAL;
  }

  conn->err = err;
  if (p != NULL) {
    len = p->tot_len;
    SYS_ARCH_INC(conn->recv_avail, len);
  } else {
    len = 0;
  }

  if (sys_mbox_trypost(conn->recvmbox, p) != ERR_OK) {
    return ERR_MEM;
  } else {
    /* Register event with callback */
    API_EVENT(conn, NETCONN_EVT_RCVPLUS, len);
  }

  return ERR_OK;
}

/**
 * Poll callback function for TCP netconns.
 * Wakes up an application thread that waits for a connection to close
 * or data to be sent. The application thread then takes the
 * appropriate action to go on.
 *
 * Signals the conn->sem.
 * netconn_close waits for conn->sem if closing failed.
 *
 * @see tcp.h (struct tcp_pcb.poll) for parameters and return value
 */
static err_t
poll_tcp(void *arg, struct tcp_pcb *pcb)
{
  struct netconn *conn = arg;

  LWIP_UNUSED_ARG(pcb);
  LWIP_ASSERT("conn != NULL", (conn != NULL));

  if (conn->state == NETCONN_WRITE) {
    do_writemore(conn);
  } else if (conn->state == NETCONN_CLOSE) {
    do_close_internal(conn);
  }

  return ERR_OK;
}

/**
 * Sent callback function for TCP netconns.
 * Signals the conn->sem and calls API_EVENT.
 * netconn_write waits for conn->sem if send buffer is low.
 *
 * @see tcp.h (struct tcp_pcb.sent) for parameters and return value
 */
static err_t
sent_tcp(void *arg, struct tcp_pcb *pcb, u16_t len)
{
  struct netconn *conn = arg;

  LWIP_UNUSED_ARG(pcb);
  LWIP_ASSERT("conn != NULL", (conn != NULL));

  if (conn->state == NETCONN_WRITE) {
    LWIP_ASSERT("conn->pcb.tcp != NULL", conn->pcb.tcp != NULL);
    do_writemore(conn);
  } else if (conn->state == NETCONN_CLOSE) {
    do_close_internal(conn);
  }

  if (conn) {
    if ((conn->pcb.tcp != NULL) && (tcp_sndbuf(conn->pcb.tcp) > TCP_SNDLOWAT)) {
      API_EVENT(conn, NETCONN_EVT_SENDPLUS, len);
    }
  }

  return ERR_OK;
}

/**
 * Error callback function for TCP netconns.
 * Signals conn->sem, posts to all conn mboxes and calls API_EVENT.
 * The application thread has then to decide what to do.
 *
 * @see tcp.h (struct tcp_pcb.err) for parameters
 */
static void
err_tcp(void *arg, err_t err)
{
  struct netconn *conn;

  conn = arg;
  LWIP_ASSERT("conn != NULL", (conn != NULL));

  conn->pcb.tcp = NULL;

  conn->err = err;
  if (conn->recvmbox != SYS_MBOX_NULL) {
    /* Register event with callback */
    API_EVENT(conn, NETCONN_EVT_RCVPLUS, 0);
    sys_mbox_post(conn->recvmbox, NULL);
  }
  if (conn->op_completed != SYS_SEM_NULL && conn->state == NETCONN_CONNECT) {
    conn->state = NETCONN_NONE;
    sys_sem_signal(conn->op_completed);
  }
  if (conn->acceptmbox != SYS_MBOX_NULL) {
    /* Register event with callback */
    API_EVENT(conn, NETCONN_EVT_RCVPLUS, 0);
    sys_mbox_post(conn->acceptmbox, NULL);
  }
  if ((conn->state == NETCONN_WRITE) || (conn->state == NETCONN_CLOSE)) {
    /* calling do_writemore/do_close_internal is not necessary
       since the pcb has already been deleted! */
    conn->state = NETCONN_NONE;
    /* wake up the waiting task */
    sys_sem_signal(conn->op_completed);
  }
}

/**
 * Setup a tcp_pcb with the correct callback function pointers
 * and their arguments.
 *
 * @param conn the TCP netconn to setup
 */
static void
setup_tcp(struct netconn *conn)
{
  struct tcp_pcb *pcb;

  pcb = conn->pcb.tcp;
  tcp_arg(pcb, conn);
  tcp_recv(pcb, recv_tcp);
  tcp_sent(pcb, sent_tcp);
  tcp_poll(pcb, poll_tcp, 4);
  tcp_err(pcb, err_tcp);
}

/**
 * Accept callback function for TCP netconns.
 * Allocates a new netconn and posts that to conn->acceptmbox.
 *
 * @see tcp.h (struct tcp_pcb_listen.accept) for parameters and return value
 */
static err_t
accept_function(void *arg, struct tcp_pcb *newpcb, err_t err)
{
  struct netconn *newconn;
  struct netconn *conn;

#if API_MSG_DEBUG
#if TCP_DEBUG
  tcp_debug_print_state(newpcb->state);
#endif /* TCP_DEBUG */
#endif /* API_MSG_DEBUG */
  conn = (struct netconn *)arg;

  LWIP_ERROR("accept_function: invalid conn->acceptmbox",
             conn->acceptmbox != SYS_MBOX_NULL, return ERR_VAL;);

  /* We have to set the callback here even though
   * the new socket is unknown. conn->socket is marked as -1. */
  newconn = netconn_alloc(conn->type, conn->callback);
  if (newconn == NULL) {
    return ERR_MEM;
  }
  newconn->pcb.tcp = newpcb;
  setup_tcp(newconn);
  newconn->err = err;

  if (sys_mbox_trypost(conn->acceptmbox, newconn) != ERR_OK) {
    /* When returning != ERR_OK, the connection is aborted in tcp_process(),
       so do nothing here! */
    newconn->pcb.tcp = NULL;
    netconn_free(newconn);
    return ERR_MEM;
  } else {
    /* Register event with callback */
    API_EVENT(conn, NETCONN_EVT_RCVPLUS, 0);
  }

  return ERR_OK;
}
#endif /* LWIP_TCP */

/**
 * Create a new pcb of a specific type.
 * Called from do_newconn().
 *
 * @param msg the api_msg_msg describing the connection type
 * @return msg->conn->err, but the return value is currently ignored
 */
static err_t
pcb_new(struct api_msg_msg *msg)
{
   msg->conn->err = ERR_OK;

   LWIP_ASSERT("pcb_new: pcb already allocated", msg->conn->pcb.tcp == NULL);

   /* Allocate a PCB for this connection */
   switch(NETCONNTYPE_GROUP(msg->conn->type)) {
#if LWIP_RAW
   case NETCONN_RAW:
     msg->conn->pcb.raw = raw_new(msg->msg.n.proto);
     if(msg->conn->pcb.raw == NULL) {
       msg->conn->err = ERR_MEM;
       break;
     }
     raw_recv(msg->conn->pcb.raw, recv_raw, msg->conn);
     break;
#endif /* LWIP_RAW */
#if LWIP_UDP
   case NETCONN_UDP:
     msg->conn->pcb.udp = udp_new();
     if(msg->conn->pcb.udp == NULL) {
       msg->conn->err = ERR_MEM;
       break;
     }
#if LWIP_UDPLITE
     if (msg->conn->type==NETCONN_UDPLITE) {
       udp_setflags(msg->conn->pcb.udp, UDP_FLAGS_UDPLITE);
     }
#endif /* LWIP_UDPLITE */
     if (msg->conn->type==NETCONN_UDPNOCHKSUM) {
       udp_setflags(msg->conn->pcb.udp, UDP_FLAGS_NOCHKSUM);
     }
     udp_recv(msg->conn->pcb.udp, recv_udp, msg->conn);
     break;
#endif /* LWIP_UDP */
#if LWIP_TCP
   case NETCONN_TCP:
     msg->conn->pcb.tcp = tcp_new();
     if(msg->conn->pcb.tcp == NULL) {
       msg->conn->err = ERR_MEM;
       break;
     }
     setup_tcp(msg->conn);
     break;
#endif /* LWIP_TCP */
   default:
     /* Unsupported netconn type, e.g. protocol disabled */
     msg->conn->err = ERR_VAL;
     break;
   }

  return msg->conn->err;
}

/**
 * Create a new pcb of a specific type inside a netconn.
 * Called from netconn_new_with_proto_and_callback.
 *
 * @param msg the api_msg_msg describing the connection type
 */
void
do_newconn(struct api_msg_msg *msg)
{
   if(msg->conn->pcb.tcp == NULL) {
     pcb_new(msg);
   }
   /* Else? This "new" connection already has a PCB allocated. */
   /* Is this an error condition? Should it be deleted? */
   /* We currently just are happy and return. */

   TCPIP_APIMSG_ACK(msg);
}

/**
 * Create a new netconn (of a specific type) that has a callback function.
 * The corresponding pcb is NOT created!
 *
 * @param t the type of 'connection' to create (@see enum netconn_type)
 * @param proto the IP protocol for RAW IP pcbs
 * @param callback a function to call on status changes (RX available, TX'ed)
 * @return a newly allocated struct netconn or
 *         NULL on memory error
 */
struct netconn*
netconn_alloc(enum netconn_type t, netconn_callback callback)
{
  struct netconn *conn;
  int size;

  conn = memp_malloc(MEMP_NETCONN);
  if (conn == NULL) {
    return NULL;
  }

  conn->err = ERR_OK;
  conn->type = t;
  conn->pcb.tcp = NULL;

#if (DEFAULT_RAW_RECVMBOX_SIZE == DEFAULT_UDP_RECVMBOX_SIZE) && \
    (DEFAULT_RAW_RECVMBOX_SIZE == DEFAULT_TCP_RECVMBOX_SIZE)
  size = DEFAULT_RAW_RECVMBOX_SIZE;
#else
  switch(NETCONNTYPE_GROUP(t)) {
#if LWIP_RAW
  case NETCONN_RAW:
    size = DEFAULT_RAW_RECVMBOX_SIZE;
    break;
#endif /* LWIP_RAW */
#if LWIP_UDP
  case NETCONN_UDP:
    size = DEFAULT_UDP_RECVMBOX_SIZE;
    break;
#endif /* LWIP_UDP */
#if LWIP_TCP
  case NETCONN_TCP:
    size = DEFAULT_TCP_RECVMBOX_SIZE;
    break;
#endif /* LWIP_TCP */
  default:
    LWIP_ASSERT("netconn_alloc: undefined netconn_type", 0);
    break;
  }
#endif

  if ((conn->op_completed = sys_sem_new(0)) == SYS_SEM_NULL) {
    memp_free(MEMP_NETCONN, conn);
    return NULL;
  }
  if ((conn->recvmbox = sys_mbox_new(size)) == SYS_MBOX_NULL) {
    sys_sem_free(conn->op_completed);
    memp_free(MEMP_NETCONN, conn);
    return NULL;
  }

  conn->acceptmbox   = SYS_MBOX_NULL;
  conn->state        = NETCONN_NONE;
  /* initialize socket to -1 since 0 is a valid socket */
  conn->socket       = -1;
  conn->callback     = callback;
  conn->recv_avail   = 0;
#if LWIP_TCP
  conn->write_msg    = NULL;
  conn->write_offset = 0;
#if LWIP_TCPIP_CORE_LOCKING
  conn->write_delayed = 0;
#endif /* LWIP_TCPIP_CORE_LOCKING */
#endif /* LWIP_TCP */
#if LWIP_SO_RCVTIMEO
  conn->recv_timeout = 0;
#endif /* LWIP_SO_RCVTIMEO */
#if LWIP_SO_RCVBUF
  conn->recv_bufsize = RECV_BUFSIZE_DEFAULT;
#endif /* LWIP_SO_RCVBUF */
  return conn;
}

/**
 * Delete a netconn and all its resources.
 * The pcb is NOT freed (since we might not be in the right thread context do this).
 *
 * @param conn the netconn to free
 */
void
netconn_free(struct netconn *conn)
{
  void *mem;
  LWIP_ASSERT("PCB must be deallocated outside this function", conn->pcb.tcp == NULL);

  /* Drain the recvmbox. */
  if (conn->recvmbox != SYS_MBOX_NULL) {
    while (sys_mbox_tryfetch(conn->recvmbox, &mem) != SYS_MBOX_EMPTY) {
      if (conn->type == NETCONN_TCP) {
        if(mem != NULL) {
          pbuf_free((struct pbuf *)mem);
        }
      } else {
        netbuf_delete((struct netbuf *)mem);
      }
    }
    sys_mbox_free(conn->recvmbox);
    conn->recvmbox = SYS_MBOX_NULL;
  }

  /* Drain the acceptmbox. */
  if (conn->acceptmbox != SYS_MBOX_NULL) {
    while (sys_mbox_tryfetch(conn->acceptmbox, &mem) != SYS_MBOX_EMPTY) {
      netconn_delete((struct netconn *)mem);
    }
    sys_mbox_free(conn->acceptmbox);
    conn->acceptmbox = SYS_MBOX_NULL;
  }

  sys_sem_free(conn->op_completed);
  conn->op_completed = SYS_SEM_NULL;

  memp_free(MEMP_NETCONN, conn);
}

#if LWIP_TCP
/**
 * Internal helper function to close a TCP netconn: since this sometimes
 * doesn't work at the first attempt, this function is called from multiple
 * places.
 *
 * @param conn the TCP netconn to close
 */
static void
do_close_internal(struct netconn *conn)
{
  err_t err;

  LWIP_ASSERT("invalid conn", (conn != NULL));
  LWIP_ASSERT("this is for tcp netconns only", (conn->type == NETCONN_TCP));
  LWIP_ASSERT("conn must be in state NETCONN_CLOSE", (conn->state == NETCONN_CLOSE));
  LWIP_ASSERT("pcb already closed", (conn->pcb.tcp != NULL));

  /* Set back some callback pointers */
  tcp_arg(conn->pcb.tcp, NULL);
  if (conn->pcb.tcp->state == LISTEN) {
    tcp_accept(conn->pcb.tcp, NULL);
  } else {
    tcp_recv(conn->pcb.tcp, NULL);
    tcp_accept(conn->pcb.tcp, NULL);
    /* some callbacks have to be reset if tcp_close is not successful */
    tcp_sent(conn->pcb.tcp, NULL);
    tcp_poll(conn->pcb.tcp, NULL, 4);
    tcp_err(conn->pcb.tcp, NULL);
  }
  /* Try to close the connection */
  err = tcp_close(conn->pcb.tcp);
  if (err == ERR_OK) {
    /* Closing succeeded */
    conn->state = NETCONN_NONE;
    /* Set back some callback pointers as conn is going away */
    conn->pcb.tcp = NULL;
    conn->err = ERR_OK;
    /* Trigger select() in socket layer. This send should something else so the
       errorfd is set, not the read and write fd! */
    API_EVENT(conn, NETCONN_EVT_RCVPLUS, 0);
    API_EVENT(conn, NETCONN_EVT_SENDPLUS, 0);
    /* wake up the application task */
    sys_sem_signal(conn->op_completed);
  } else {
    /* Closing failed, restore some of the callbacks */
    /* Closing of listen pcb will never fail! */
    LWIP_ASSERT("Closing a listen pcb may not fail!", (conn->pcb.tcp->state != LISTEN));
    tcp_sent(conn->pcb.tcp, sent_tcp);
    tcp_poll(conn->pcb.tcp, poll_tcp, 4);
    tcp_err(conn->pcb.tcp, err_tcp);
    tcp_arg(conn->pcb.tcp, conn);
  }
  /* If closing didn't succeed, we get called again either
     from poll_tcp or from sent_tcp */
}
#endif /* LWIP_TCP */

/**
 * Delete the pcb inside a netconn.
 * Called from netconn_delete.
 *
 * @param msg the api_msg_msg pointing to the connection
 */
void
do_delconn(struct api_msg_msg *msg)
{
  if (msg->conn->pcb.tcp != NULL) {
    switch (NETCONNTYPE_GROUP(msg->conn->type)) {
#if LWIP_RAW
    case NETCONN_RAW:
      raw_remove(msg->conn->pcb.raw);
      break;
#endif /* LWIP_RAW */
#if LWIP_UDP
    case NETCONN_UDP:
      msg->conn->pcb.udp->recv_arg = NULL;
      udp_remove(msg->conn->pcb.udp);
      break;
#endif /* LWIP_UDP */
#if LWIP_TCP
    case NETCONN_TCP:
      msg->conn->state = NETCONN_CLOSE;
      do_close_internal(msg->conn);
      /* API_EVENT is called inside do_close_internal, before releasing
         the application thread, so we can return at this point! */
      return;
#endif /* LWIP_TCP */
    default:
      break;
    }
  }
  /* tcp netconns don't come here! */

  /* Trigger select() in socket layer. This send should something else so the
     errorfd is set, not the read and write fd! */
  API_EVENT(msg->conn, NETCONN_EVT_RCVPLUS, 0);
  API_EVENT(msg->conn, NETCONN_EVT_SENDPLUS, 0);

  if (msg->conn->op_completed != SYS_SEM_NULL) {
    sys_sem_signal(msg->conn->op_completed);
  }
}

/**
 * Bind a pcb contained in a netconn
 * Called from netconn_bind.
 *
 * @param msg the api_msg_msg pointing to the connection and containing
 *            the IP address and port to bind to
 */
void
do_bind(struct api_msg_msg *msg)
{

	#if  LWIP_TRACE_ERROR
	TRACE_ERROR("do_bind\n");
	#endif
  if (!ERR_IS_FATAL(msg->conn->err)) {
    if (msg->conn->pcb.tcp != NULL) {
      switch (NETCONNTYPE_GROUP(msg->conn->type)) {
#if LWIP_RAW
      case NETCONN_RAW:
        msg->conn->err = raw_bind(msg->conn->pcb.raw, msg->msg.bc.ipaddr);
        break;
#endif /* LWIP_RAW */
#if LWIP_UDP
      case NETCONN_UDP:
        msg->conn->err = udp_bind(msg->conn->pcb.udp, msg->msg.bc.ipaddr, msg->msg.bc.port);
        break;
#endif /* LWIP_UDP */
#if LWIP_TCP
      case NETCONN_TCP:
        msg->conn->err = tcp_bind(msg->conn->pcb.tcp, msg->msg.bc.ipaddr, msg->msg.bc.port);
        break;
#endif /* LWIP_TCP */
      default:
        break;
      }
    } else {
      /* msg->conn->pcb is NULL */
      msg->conn->err = ERR_VAL;
    }
  }
  TCPIP_APIMSG_ACK(msg);
}

#if LWIP_TCP
/**
 * TCP callback function if a connection (opened by tcp_connect/do_connect) has
 * been established (or reset by the remote host).
 *
 * @see tcp.h (struct tcp_pcb.connected) for parameters and return values
 */
static err_t
do_connected(void *arg, struct tcp_pcb *pcb, err_t err)
{
  struct netconn *conn;

  LWIP_UNUSED_ARG(pcb);

  conn = arg;

  if (conn == NULL) {
    return ERR_VAL;
  }

  conn->err = err;
  if ((conn->type == NETCONN_TCP) && (err == ERR_OK)) {
    setup_tcp(conn);
  }
  conn->state = NETCONN_NONE;
  sys_sem_signal(conn->op_completed);
  return ERR_OK;
}
#endif /* LWIP_TCP */

/**
 * Connect a pcb contained inside a netconn
 * Called from netconn_connect.
 *
 * @param msg the api_msg_msg pointing to the connection and containing
 *            the IP address and port to connect to
 */
void
do_connect(struct api_msg_msg *msg)
{
  if (msg->conn->pcb.tcp == NULL) {
    sys_sem_signal(msg->conn->op_completed);
    return;
  }

  switch (NETCONNTYPE_GROUP(msg->conn->type)) {
#if LWIP_RAW
  case NETCONN_RAW:
    msg->conn->err = raw_connect(msg->conn->pcb.raw, msg->msg.bc.ipaddr);
    sys_sem_signal(msg->conn->op_completed);
    break;
#endif /* LWIP_RAW */
#if LWIP_UDP
  case NETCONN_UDP:
    msg->conn->err = udp_connect(msg->conn->pcb.udp, msg->msg.bc.ipaddr, msg->msg.bc.port);
    sys_sem_signal(msg->conn->op_completed);
    break;
#endif /* LWIP_UDP */
#if LWIP_TCP
  case NETCONN_TCP:
    msg->conn->state = NETCONN_CONNECT;
    setup_tcp(msg->conn);
    msg->conn->err = tcp_connect(msg->conn->pcb.tcp, msg->msg.bc.ipaddr, msg->msg.bc.port,
                                 do_connected);
    /* sys_sem_signal() is called from do_connected (or err_tcp()),
     * when the connection is established! */
    break;
#endif /* LWIP_TCP */
  default:
    LWIP_ERROR("Invalid netconn type", 0, do{ msg->conn->err = ERR_VAL;
      sys_sem_signal(msg->conn->op_completed); }while(0));
    break;
  }
}

/**
 * Connect a pcb contained inside a netconn
 * Only used for UDP netconns.
 * Called from netconn_disconnect.
 *
 * @param msg the api_msg_msg pointing to the connection to disconnect
 */
void
do_disconnect(struct api_msg_msg *msg)
{
#if LWIP_UDP
  if (NETCONNTYPE_GROUP(msg->conn->type) == NETCONN_UDP) {
    udp_disconnect(msg->conn->pcb.udp);
  }
#endif /* LWIP_UDP */
  TCPIP_APIMSG_ACK(msg);
}

/**
 * Set a TCP pcb contained in a netconn into listen mode
 * Called from netconn_listen.
 *
 * @param msg the api_msg_msg pointing to the connection
 */
void
do_listen(struct api_msg_msg *msg)
{
#if LWIP_TCP
  if (!ERR_IS_FATAL(msg->conn->err)) {
    if (msg->conn->pcb.tcp != NULL) {
      if (msg->conn->type == NETCONN_TCP) {
        if (msg->conn->pcb.tcp->state == CLOSED) {
#if TCP_LISTEN_BACKLOG
          struct tcp_pcb* lpcb = tcp_listen_with_backlog(msg->conn->pcb.tcp, msg->msg.lb.backlog);
#else  /* TCP_LISTEN_BACKLOG */
          struct tcp_pcb* lpcb = tcp_listen(msg->conn->pcb.tcp);
#endif /* TCP_LISTEN_BACKLOG */
          if (lpcb == NULL) {
            msg->conn->err = ERR_MEM;
          } else {
            /* delete the recvmbox and allocate the acceptmbox */
            if (msg->conn->recvmbox != SYS_MBOX_NULL) {
              /** @todo: should we drain the recvmbox here? */
              sys_mbox_free(msg->conn->recvmbox);
              msg->conn->recvmbox = SYS_MBOX_NULL;
            }
            if (msg->conn->acceptmbox == SYS_MBOX_NULL) {
              if ((msg->conn->acceptmbox = sys_mbox_new(DEFAULT_ACCEPTMBOX_SIZE)) == SYS_MBOX_NULL) {
                msg->conn->err = ERR_MEM;
              }
            }
            if (msg->conn->err == ERR_OK) {
              msg->conn->state = NETCONN_LISTEN;
              msg->conn->pcb.tcp = lpcb;
              tcp_arg(msg->conn->pcb.tcp, msg->conn);
              tcp_accept(msg->conn->pcb.tcp, accept_function);
            }
          }
        } else {
          msg->conn->err = ERR_CONN;
        }
      }
    }
  }
#endif /* LWIP_TCP */
  TCPIP_APIMSG_ACK(msg);
}

/**
 * Send some data on a RAW or UDP pcb contained in a netconn
 * Called from netconn_send
 *
 * @param msg the api_msg_msg pointing to the connection
 */
void
do_send(struct api_msg_msg *msg)
{
  if (!ERR_IS_FATAL(msg->conn->err)) {
    if (msg->conn->pcb.tcp != NULL) {
      switch (NETCONNTYPE_GROUP(msg->conn->type)) {
#if LWIP_RAW
      case NETCONN_RAW:
        if (msg->msg.b->addr == NULL) {
          msg->conn->err = raw_send(msg->conn->pcb.raw, msg->msg.b->p);
        } else {
          msg->conn->err = raw_sendto(msg->conn->pcb.raw, msg->msg.b->p, msg->msg.b->addr);
        }
        break;
#endif
#if LWIP_UDP
      case NETCONN_UDP:
        if (msg->msg.b->addr == NULL) {
          msg->conn->err = udp_send(msg->conn->pcb.udp, msg->msg.b->p);
        } else {
          msg->conn->err = udp_sendto(msg->conn->pcb.udp, msg->msg.b->p, msg->msg.b->addr, msg->msg.b->port);
        }
        break;
#endif /* LWIP_UDP */
      default:
        break;
      }
    }
  }
  TCPIP_APIMSG_ACK(msg);
}

/**
 * Indicate data has been received from a TCP pcb contained in a netconn
 * Called from netconn_recv
 *
 * @param msg the api_msg_msg pointing to the connection
 */
void
do_recv(struct api_msg_msg *msg)
{
#if LWIP_TCP
  if (!ERR_IS_FATAL(msg->conn->err)) {
    if (msg->conn->pcb.tcp != NULL) {
      if (msg->conn->type == NETCONN_TCP) {
#if TCP_LISTEN_BACKLOG
        if (msg->conn->pcb.tcp->state == LISTEN) {
          tcp_accepted(msg->conn->pcb.tcp);
        } else
#endif /* TCP_LISTEN_BACKLOG */
        {
          tcp_recved(msg->conn->pcb.tcp, msg->msg.r.len);
        }
      }
    }
  }
#endif /* LWIP_TCP */
  TCPIP_APIMSG_ACK(msg);
}

#if LWIP_TCP
/**
 * See if more data needs to be written from a previous call to netconn_write.
 * Called initially from do_write. If the first call can't send all data
 * (because of low memory or empty send-buffer), this function is called again
 * from sent_tcp() or poll_tcp() to send more data. If all data is sent, the
 * blocking application thread (waiting in netconn_write) is released.
 *
 * @param conn netconn (that is currently in state NETCONN_WRITE) to process
 * @return ERR_OK
 *         ERR_MEM if LWIP_TCPIP_CORE_LOCKING=1 and sending hasn't yet finished
 */
static err_t
do_writemore(struct netconn *conn)
{
  err_t err;
  void *dataptr;
  u16_t len, available;
  u8_t write_finished = 0;
  size_t diff;

  LWIP_ASSERT("conn->state == NETCONN_WRITE", (conn->state == NETCONN_WRITE));

  dataptr = (u8_t*)conn->write_msg->msg.w.dataptr + conn->write_offset;
  diff = conn->write_msg->msg.w.len - conn->write_offset;
  if (diff > 0xffffUL) { /* max_u16_t */
    len = 0xffff;
#if LWIP_TCPIP_CORE_LOCKING
    conn->write_delayed = 1;
#endif
  } else {
    len = (u16_t)diff;
  }
  available = tcp_sndbuf(conn->pcb.tcp);
  if (available < len) {
    /* don't try to write more than sendbuf */
    len = available;
#if LWIP_TCPIP_CORE_LOCKING
    conn->write_delayed = 1;
#endif
  }

  err = tcp_write(conn->pcb.tcp, dataptr, len, conn->write_msg->msg.w.apiflags);
  LWIP_ASSERT("do_writemore: invalid length!", ((conn->write_offset + len) <= conn->write_msg->msg.w.len));
  if (err == ERR_OK) {
    conn->write_offset += len;
    if (conn->write_offset == conn->write_msg->msg.w.len) {
      /* everything was written */
      write_finished = 1;
      conn->write_msg = NULL;
      conn->write_offset = 0;
      /* API_EVENT might call tcp_tmr, so reset conn->state now */
      conn->state = NETCONN_NONE;
    }
    err = tcp_output_nagle(conn->pcb.tcp);
    conn->err = err;
    if ((err == ERR_OK) && (tcp_sndbuf(conn->pcb.tcp) <= TCP_SNDLOWAT)) {
      API_EVENT(conn, NETCONN_EVT_SENDMINUS, len);
    }
  } else if (err == ERR_MEM) {
    /* If ERR_MEM, we wait for sent_tcp or poll_tcp to be called
       we do NOT return to the application thread, since ERR_MEM is
       only a temporary error! */

    /* tcp_enqueue returned ERR_MEM, try tcp_output anyway */
    err = tcp_output(conn->pcb.tcp);

#if LWIP_TCPIP_CORE_LOCKING
    conn->write_delayed = 1;
#endif
  } else {
    /* On errors != ERR_MEM, we don't try writing any more but return
       the error to the application thread. */
    conn->err = err;
    write_finished = 1;
  }

  if (write_finished) {
    /* everything was written: set back connection state
       and back to application task */
    conn->state = NETCONN_NONE;
#if LWIP_TCPIP_CORE_LOCKING
    if (conn->write_delayed != 0)
#endif
    {
      sys_sem_signal(conn->op_completed);
    }
  }
#if LWIP_TCPIP_CORE_LOCKING
  else
    return ERR_MEM;
#endif
  return ERR_OK;
}
#endif /* LWIP_TCP */

/**
 * Send some data on a TCP pcb contained in a netconn
 * Called from netconn_write
 *
 * @param msg the api_msg_msg pointing to the connection
 */
void
do_write(struct api_msg_msg *msg)
{
  if (!ERR_IS_FATAL(msg->conn->err)) {
    if ((msg->conn->pcb.tcp != NULL) && (msg->conn->type == NETCONN_TCP)) {
#if LWIP_TCP
      msg->conn->state = NETCONN_WRITE;
      /* set all the variables used by do_writemore */
      LWIP_ASSERT("already writing", msg->conn->write_msg == NULL &&
        msg->conn->write_offset == 0);
      msg->conn->write_msg = msg;
      msg->conn->write_offset = 0;
#if LWIP_TCPIP_CORE_LOCKING
      msg->conn->write_delayed = 0;
      if (do_writemore(msg->conn) != ERR_OK) {
        LWIP_ASSERT("state!", msg->conn->state == NETCONN_WRITE);
        UNLOCK_TCPIP_CORE();
        sys_arch_sem_wait(msg->conn->op_completed, 0);
        LOCK_TCPIP_CORE();
        LWIP_ASSERT("state!", msg->conn->state == NETCONN_NONE);
      }
#else
      do_writemore(msg->conn);
#endif
      /* for both cases: if do_writemore was called, don't ACK the APIMSG! */
      return;
#endif /* LWIP_TCP */
#if (LWIP_UDP || LWIP_RAW)
    } else {
      msg->conn->err = ERR_VAL;
#endif /* (LWIP_UDP || LWIP_RAW) */
    }
  }
  TCPIP_APIMSG_ACK(msg);
}

/**
 * Return a connection's local or remote address
 * Called from netconn_getaddr
 *
 * @param msg the api_msg_msg pointing to the connection
 */
void
do_getaddr(struct api_msg_msg *msg)
{
  if (msg->conn->pcb.ip != NULL) {
    *(msg->msg.ad.ipaddr) = (msg->msg.ad.local?msg->conn->pcb.ip->local_ip:msg->conn->pcb.ip->remote_ip);

    switch (NETCONNTYPE_GROUP(msg->conn->type)) {
#if LWIP_RAW
    case NETCONN_RAW:
      if (msg->msg.ad.local) {
        *(msg->msg.ad.port) = msg->conn->pcb.raw->protocol;
      } else {
        /* return an error as connecting is only a helper for upper layers */
        msg->conn->err = ERR_CONN;
      }
      break;
#endif /* LWIP_RAW */
#if LWIP_UDP
    case NETCONN_UDP:
      if (msg->msg.ad.local) {
        *(msg->msg.ad.port) = msg->conn->pcb.udp->local_port;
      } else {
        if ((msg->conn->pcb.udp->flags & UDP_FLAGS_CONNECTED) == 0) {
          msg->conn->err = ERR_CONN;
        } else {
          *(msg->msg.ad.port) = msg->conn->pcb.udp->remote_port;
        }
      }
      break;
#endif /* LWIP_UDP */
#if LWIP_TCP
    case NETCONN_TCP:
      *(msg->msg.ad.port) = (msg->msg.ad.local?msg->conn->pcb.tcp->local_port:msg->conn->pcb.tcp->remote_port);
      break;
#endif /* LWIP_TCP */
    }
  } else {
    msg->conn->err = ERR_CONN;
  }
  TCPIP_APIMSG_ACK(msg);
}

/**
 * Close a TCP pcb contained in a netconn
 * Called from netconn_close
 *
 * @param msg the api_msg_msg pointing to the connection
 */
void
do_close(struct api_msg_msg *msg)
{
#if LWIP_TCP
  if ((msg->conn->pcb.tcp != NULL) && (msg->conn->type == NETCONN_TCP)) {
      msg->conn->state = NETCONN_CLOSE;
      do_close_internal(msg->conn);
      /* for tcp netconns, do_close_internal ACKs the message */
  } else
#endif /* LWIP_TCP */
  {
    msg->conn->err = ERR_VAL;
    sys_sem_signal(msg->conn->op_completed);
  }
}

#if LWIP_IGMP
/**
 * Join multicast groups for UDP netconns.
 * Called from netconn_join_leave_group
 *
 * @param msg the api_msg_msg pointing to the connection
 */
void
do_join_leave_group(struct api_msg_msg *msg)
{
  if (!ERR_IS_FATAL(msg->conn->err)) {
    if (msg->conn->pcb.tcp != NULL) {
      if (NETCONNTYPE_GROUP(msg->conn->type) == NETCONN_UDP) {
#if LWIP_UDP
        if (msg->msg.jl.join_or_leave == NETCONN_JOIN) {
          msg->conn->err = igmp_joingroup(msg->msg.jl.interface, msg->msg.jl.multiaddr);
        } else {
          msg->conn->err = igmp_leavegroup(msg->msg.jl.interface, msg->msg.jl.multiaddr);
        }
#endif /* LWIP_UDP */
#if (LWIP_TCP || LWIP_RAW)
      } else {
        msg->conn->err = ERR_VAL;
#endif /* (LWIP_TCP || LWIP_RAW) */
      }
    }
  }
  TCPIP_APIMSG_ACK(msg);
}
#endif /* LWIP_IGMP */

#if LWIP_DNS
/**
 * Callback function that is called when DNS name is resolved
 * (or on timeout). A waiting application thread is waked up by
 * signaling the semaphore.
 */
static void
do_dns_found(const char *name, struct ip_addr *ipaddr, void *arg)
{
  struct dns_api_msg *msg = (struct dns_api_msg*)arg;

  LWIP_ASSERT("DNS response for wrong host name", strcmp(msg->name, name) == 0);

  if (ipaddr == NULL) {
    /* timeout or memory error */
    *msg->err = ERR_VAL;
  } else {
    /* address was resolved */
    *msg->err = ERR_OK;
    *msg->addr = *ipaddr;
  }
  /* wake up the application task waiting in netconn_gethostbyname */
  sys_sem_signal(msg->sem);
}

/**
 * Execute a DNS query
 * Called from netconn_gethostbyname
 *
 * @param arg the dns_api_msg pointing to the query
 */
void
do_gethostbyname(void *arg)
{
  struct dns_api_msg *msg = (struct dns_api_msg*)arg;

  *msg->err = dns_gethostbyname(msg->name, msg->addr, do_dns_found, msg);
  if (*msg->err != ERR_INPROGRESS) {
    /* on error or immediate success, wake up the application
     * task waiting in netconn_gethostbyname */
    sys_sem_signal(msg->sem);
  }
}
#endif /* LWIP_DNS */

#endif /* LWIP_NETCONN */
