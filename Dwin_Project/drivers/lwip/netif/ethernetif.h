#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__

#include "board.h"


#include "lwip/include/lwip/err.h"
#include "lwip/include/lwip/netif.h"


#define NIOCTL_GADDR		0x01
#define ETHERNET_MTU		1500

struct eth_tx_msg
{
	struct netif 	*netif;
	struct pbuf 	*buf;
};
extern uint8_t MACaddr0[6];
struct fm3_emac;

struct eth_device
{

	struct netif *netif;
	/* eth device interface */
	struct pbuf* (*eth_rx)(struct fm3_emac * dev);
	int32_t (*eth_tx)(struct fm3_emac*  dev, struct pbuf* p);
};


void eth_system_device_init(void);

static void ethernetif_input(struct netif *netif);
struct netif *ethernetif_register(void);

#ifdef SERVER

#define MAC_ADDR0 0x00
#define MAC_ADDR1 0x00
#define MAC_ADDR2 0x00
#define MAC_ADDR3 0x00
#define MAC_ADDR4 0x00
#define MAC_ADDR5 0x01

#else

#define MAC_ADDR0 0x00
#define MAC_ADDR1 0x00
#define MAC_ADDR2 0x00
#define MAC_ADDR3 0x00
#define MAC_ADDR4 0x00
//#define MAC_ADDR5 0x02
#define MAC_ADDR5 0x03
//#define MAC_ADDR5 0x04

#endif

#endif
