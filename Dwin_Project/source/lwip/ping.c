#include "ping.h"

extern void dbgu_printf( const INT8U *fmt, ... );
extern void* malloc( int size );
extern void free( void *p );

//static int ping_seq_num;

#if 1
static err_t ping_send( int s, struct ip_addr *addr ){ return 0;}
void ping_recv( int s, struct ip_addr *addr ){}
void ping_thread(void *arg){}
#else
/* Ping using the socket ip */
static err_t ping_send( int s, struct ip_addr *addr )
{
	int err;
	struct icmp_echo_hdr *iecho;
	struct sockaddr_in to;
	
	iecho = (struct icmp_echo_hdr *)mem_malloc( sizeof(struct icmp_echo_hdr) );
	if ( iecho == NULL )
		return ERR_MEM;
		
	ICMPH_TYPE_SET(iecho,ICMP_ECHO);
	ICMPH_CODE_SET(iecho, 0);
	iecho->chksum = 0;
	iecho->id     = PING_ID;
	iecho->seqno  = htons(++ping_seq_num);
	iecho->chksum = inet_chksum(iecho, len);
	
	/* fill the additional data buffer with some data */
	for(i = 0; i < PING_DATA_SIZE; i++) {
		((char*)iecho)[sizeof(struct icmp_echo_hdr) + i] = i;
	}
  	
	to.sin_len = sizeof(to);
	to.sin_family = AF_INET;
	to.sin_addr.s_addr = addr->addr;
	
	err = lwip_sendto( s, iecho, sizeof(*iecho), 0, (struct sockaddr*)&to, sizeof(to) );
	mem_free( iecho );
	
	return (err ? ERR_OK : ERR_VAL);
}

void ping_recv( int s, struct ip_addr *addr )
{
char buf[64];
  int fromlen, len;
  struct sockaddr_in from;
  struct ip_hdr *iphdr;
  struct icmp_echo_hdr *iecho;

  while((len = lwip_recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr*)&from, (socklen_t*)&fromlen)) > 0) {
    if (len >= (sizeof(struct ip_hdr)+sizeof(struct icmp_echo_hdr))) {
      LWIP_DEBUGF( PING_DEBUG, ("ping: recv "));
      ip_addr_debug_print(PING_DEBUG, (struct ip_addr *)&(from.sin_addr));
      LWIP_DEBUGF( PING_DEBUG, (" %lu ms\n", (sys_now()-ping_time)));

      iphdr = (struct ip_hdr *)buf;
      iecho = (struct icmp_echo_hdr *)(buf+(IPH_HL(iphdr) * 4));
      if ((iecho->id == PING_ID) && (iecho->seqno == htons(ping_seq_num))) {
        /* do some ping result processing */
        PING_RESULT((ICMPH_TYPE(iecho) == ICMP_ER));
        return;
      } else {
        LWIP_DEBUGF( PING_DEBUG, ("ping: drop\n"));
      }
    }
  }

  if (len == 0) {
    LWIP_DEBUGF( PING_DEBUG, ("ping: recv - %lu ms - timeout\n", (sys_now()-ping_time)));
  }

  /* do some ping result processing */
  PING_RESULT(0);
}

void ping_thread(void *arg)
{
	 int s;
  int timeout = PING_RCV_TIMEO;
  struct ip_addr ping_target;

  LWIP_UNUSED_ARG(arg);

  if ((s = lwip_socket(AF_INET, SOCK_RAW, IP_PROTO_ICMP)) < 0) {
    return;
  }

  lwip_setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

  while (1) 
  {
    ping_target = PING_TARGET;

    if (ping_send(s, &ping_target) == ERR_OK) {
      LWIP_DEBUGF( PING_DEBUG, ("ping: send "));
      ip_addr_debug_print(PING_DEBUG, &ping_target);
      LWIP_DEBUGF( PING_DEBUG, ("\n"));

      ping_time = sys_now();
      ping_recv(s);
    } else {
      LWIP_DEBUGF( PING_DEBUG, ("ping: send "));
      ip_addr_debug_print(PING_DEBUG, &ping_target);
      LWIP_DEBUGF( PING_DEBUG, (" - error\n"));
    }
    sys_msleep(PING_DELAY);
  }
}
#endif