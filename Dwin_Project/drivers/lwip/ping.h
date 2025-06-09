#ifndef _PING_HEADER_
#define _PING_HEADER_

#include "include/ipv4/lwip/icmp.h"
#include "include/ipv4/lwip/ip.h"
#include "include/ipv4/lwip/inet_chksum.h"
#include "include/lwip/raw.h"
#include "emac_api.h"

static err_t ping_send( int s, struct ip_addr *addr );
void ping_recv( int s, struct ip_addr *addr );
void ping_thread(void *arg);

#endif
