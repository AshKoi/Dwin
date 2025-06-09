#ifndef _SOCKETAPI_HEADER_
#define _SOCKETAPI_HEADER_

#include "include/ipv4/lwip/icmp.h"
#include "include/ipv4/lwip/ip.h"
#include "include/ipv4/lwip/inet_chksum.h"
#include "include/lwip/raw.h"
#include "lwip/emac_api.h"

#include "emac_fm3.h"
extern BOOLEAN Init(ENET_PARAM* pEnetParam);

#endif