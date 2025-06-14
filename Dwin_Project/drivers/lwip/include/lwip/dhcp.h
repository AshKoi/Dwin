/** @file
 */

#ifndef __LWIP_DHCP_H__
#define __LWIP_DHCP_H__

#include "lwip/include/lwip/opt.h"

#if LWIP_DHCP /* don't build if not configured for use in lwipopts.h */

#include "lwip/include/lwip/netif.h"
#include "lwip/include/lwip/udp.h"

#ifdef __cplusplus
extern "C" {
#endif

/** period (in seconds) of the application calling dhcp_coarse_tmr() */
#define DHCP_COARSE_TIMER_SECS 60 
/** period (in milliseconds) of the application calling dhcp_coarse_tmr() */
#define DHCP_COARSE_TIMER_MSECS (DHCP_COARSE_TIMER_SECS * 1000UL)
/** period (in milliseconds) of the application calling dhcp_fine_tmr() */
#define DHCP_FINE_TIMER_MSECS 500 

struct dhcp
{
  /** transaction identifier of last sent request */ 
  u32_t xid;
  /** our connection to the DHCP server */ 
  struct udp_pcb *pcb;
  /** incoming msg */
  struct dhcp_msg *msg_in;
  /** incoming msg options */
  void *options_in; 
  /** ingoing msg options length */
  u16_t options_in_len;
  /** current DHCP state machine state */
  u8_t state;
  /** retries of current request */
  u8_t tries;

  struct pbuf *p_out; /* pbuf of outcoming msg */
  struct dhcp_msg *msg_out; /* outgoing msg */
  u16_t options_out_len; /* outgoing msg options length */
  u16_t request_timeout; /* #ticks with period DHCP_FINE_TIMER_SECS for request timeout */
  u16_t t1_timeout;  /* #ticks with period DHCP_COARSE_TIMER_SECS for renewal time */
  u16_t t2_timeout;  /* #ticks with period DHCP_COARSE_TIMER_SECS for rebind time */
  struct ip_addr server_ip_addr; /* dhcp server address that offered this lease */
  struct ip_addr offered_ip_addr;
  struct ip_addr offered_sn_mask;
  struct ip_addr offered_gw_addr;
  struct ip_addr offered_bc_addr;
#define DHCP_MAX_DNS 2
  u32_t dns_count; /* actual number of DNS servers obtained */
  struct ip_addr offered_dns_addr[DHCP_MAX_DNS]; /* DNS server addresses */
 
  u32_t offered_t0_lease; /* lease period (in seconds) */
  u32_t offered_t1_renew; /* recommended renew time (usually 50% of lease period) */
  u32_t offered_t2_rebind; /* recommended rebind time (usually 66% of lease period)  */
#if LWIP_DHCP_AUTOIP_COOP
  u8_t autoip_coop_state;
#endif
/** Patch #1308
 *  TODO: See dhcp.c "TODO"s
 */
#if 0
  struct ip_addr offered_si_addr;
  u8_t *boot_file_name;
#endif
};

/* MUST be compiled with "pack structs" or equivalent! */
#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/bpstruct.h"
#endif
PACK_STRUCT_BEGIN
/** minimum set of fields of any DHCP message */
struct dhcp_msg
{
  PACK_STRUCT_FIELD(u8_t op);
  PACK_STRUCT_FIELD(u8_t htype);
  PACK_STRUCT_FIELD(u8_t hlen);
  PACK_STRUCT_FIELD(u8_t hops);
  PACK_STRUCT_FIELD(u32_t xid);
  PACK_STRUCT_FIELD(u16_t secs);
  PACK_STRUCT_FIELD(u16_t flags);
  PACK_STRUCT_FIELD(struct ip_addr ciaddr);
  PACK_STRUCT_FIELD(struct ip_addr yiaddr);
  PACK_STRUCT_FIELD(struct ip_addr siaddr);
  PACK_STRUCT_FIELD(struct ip_addr giaddr);
#define DHCP_CHADDR_LEN 16U
  PACK_STRUCT_FIELD(u8_t chaddr[DHCP_CHADDR_LEN]);
#define DHCP_SNAME_LEN 64U
  PACK_STRUCT_FIELD(u8_t sname[DHCP_SNAME_LEN]);
#define DHCP_FILE_LEN 128U
  PACK_STRUCT_FIELD(u8_t file[DHCP_FILE_LEN]);
  PACK_STRUCT_FIELD(u32_t cookie);
#define DHCP_MIN_OPTIONS_LEN 68U
/** make sure user does not configure this too small */
#if ((defined(DHCP_OPTIONS_LEN)) && (DHCP_OPTIONS_LEN < DHCP_MIN_OPTIONS_LEN))
#  undef DHCP_OPTIONS_LEN
#endif
/** allow this to be configured in lwipopts.h, but not too small */
#if (!defined(DHCP_OPTIONS_LEN))
/** set this to be sufficient for your options in outgoing DHCP msgs */
#  define DHCP_OPTIONS_LEN DHCP_MIN_OPTIONS_LEN
#endif
  PACK_STRUCT_FIELD(u8_t options[DHCP_OPTIONS_LEN]);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END
#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/epstruct.h"
#endif

/** start DHCP configuration */
err_t dhcp_start(struct netif *netif);
/** enforce early lease renewal (not needed normally)*/
err_t dhcp_renew(struct netif *netif);
/** release the DHCP lease, usually called before dhcp_stop()*/
err_t dhcp_release(struct netif *netif);
/** stop DHCP configuration */
void dhcp_stop(struct netif *netif);
/** inform server of our manual IP address */
void dhcp_inform(struct netif *netif);
/** Handle a possible change in the network configuration */
void dhcp_network_changed(struct netif *netif);

/** if enabled, check whether the offered IP address is not in use, using ARP */
#if DHCP_DOES_ARP_CHECK
void dhcp_arp_reply(struct netif *netif, struct ip_addr *addr);
#endif

/** to be called every minute */
void dhcp_coarse_tmr(void);
/** to be called every half second */
void dhcp_fine_tmr(void);
 
/** DHCP message item offsets and length */
#define DHCP_MSG_OFS (UDP_DATA_OFS)  
  #define DHCP_OP_OFS (DHCP_MSG_OFS + 0)
  #define DHCP_HTYPE_OFS (DHCP_MSG_OFS + 1)
  #define DHCP_HLEN_OFS (DHCP_MSG_OFS + 2)
  #define DHCP_HOPS_OFS (DHCP_MSG_OFS + 3)
  #define DHCP_XID_OFS (DHCP_MSG_OFS + 4)
  #define DHCP_SECS_OFS (DHCP_MSG_OFS + 8)
  #define DHCP_FLAGS_OFS (DHCP_MSG_OFS + 10)
  #define DHCP_CIADDR_OFS (DHCP_MSG_OFS + 12)
  #define DHCP_YIADDR_OFS (DHCP_MSG_OFS + 16)
  #define DHCP_SIADDR_OFS (DHCP_MSG_OFS + 20)
  #define DHCP_GIADDR_OFS (DHCP_MSG_OFS + 24)
  #define DHCP_CHADDR_OFS (DHCP_MSG_OFS + 28)
  #define DHCP_SNAME_OFS (DHCP_MSG_OFS + 44)
  #define DHCP_FILE_OFS (DHCP_MSG_OFS + 108)
#define DHCP_MSG_LEN 236

#define DHCP_COOKIE_OFS (DHCP_MSG_OFS + DHCP_MSG_LEN)
#define DHCP_OPTIONS_OFS (DHCP_MSG_OFS + DHCP_MSG_LEN + 4)

#define DHCP_CLIENT_PORT 68  
#define DHCP_SERVER_PORT 67

/** DHCP client states */
#define DHCP_REQUESTING 1
#define DHCP_INIT 2
#define DHCP_REBOOTING 3
#define DHCP_REBINDING 4
#define DHCP_RENEWING 5
#define DHCP_SELECTING 6
#define DHCP_INFORMING 7
#define DHCP_CHECKING 8
#define DHCP_PERMANENT 9
#define DHCP_BOUND 10
/** not yet implemented #define DHCP_RELEASING 11 */
#define DHCP_BACKING_OFF 12
#define DHCP_OFF 13

/** AUTOIP cooperatation flags */
#define DHCP_AUTOIP_COOP_STATE_OFF 0
#define DHCP_AUTOIP_COOP_STATE_ON 1
 
#define DHCP_BOOTREQUEST 1
#define DHCP_BOOTREPLY 2

#define DHCP_DISCOVER 1
#define DHCP_OFFER 2
#define DHCP_REQUEST 3
#define DHCP_DECLINE 4
#define DHCP_ACK 5
#define DHCP_NAK 6
#define DHCP_RELEASE 7
#define DHCP_INFORM 8

#define DHCP_HTYPE_ETH 1

#define DHCP_HLEN_ETH 6

#define DHCP_BROADCAST_FLAG 15
#define DHCP_BROADCAST_MASK (1 << DHCP_FLAG_BROADCAST)

/** BootP options */
#define DHCP_OPTION_PAD 0
#define DHCP_OPTION_SUBNET_MASK 1 /* RFC 2132 3.3 */
#define DHCP_OPTION_ROUTER 3
#define DHCP_OPTION_DNS_SERVER 6 
#define DHCP_OPTION_HOSTNAME 12
#define DHCP_OPTION_IP_TTL 23
#define DHCP_OPTION_MTU 26
#define DHCP_OPTION_BROADCAST 28
#define DHCP_OPTION_TCP_TTL 37
#define DHCP_OPTION_END 255

/** DHCP options */
#define DHCP_OPTION_REQUESTED_IP 50 /* RFC 2132 9.1, requested IP address */
#define DHCP_OPTION_LEASE_TIME 51 /* RFC 2132 9.2, time in seconds, in 4 bytes */
#define DHCP_OPTION_OVERLOAD 52 /* RFC2132 9.3, use file and/or sname field for options */

#define DHCP_OPTION_MESSAGE_TYPE 53 /* RFC 2132 9.6, important for DHCP */
#define DHCP_OPTION_MESSAGE_TYPE_LEN 1


#define DHCP_OPTION_SERVER_ID 54 /* RFC 2132 9.7, server IP address */
#define DHCP_OPTION_PARAMETER_REQUEST_LIST 55 /* RFC 2132 9.8, requested option types */

#define DHCP_OPTION_MAX_MSG_SIZE 57 /* RFC 2132 9.10, message size accepted >= 576 */
#define DHCP_OPTION_MAX_MSG_SIZE_LEN 2

#define DHCP_OPTION_T1 58 /* T1 renewal time */
#define DHCP_OPTION_T2 59 /* T2 rebinding time */
#define DHCP_OPTION_US 60
#define DHCP_OPTION_CLIENT_ID 61
#define DHCP_OPTION_TFTP_SERVERNAME 66
#define DHCP_OPTION_BOOTFILE 67

/** possible combinations of overloading the file and sname fields with options */
#define DHCP_OVERLOAD_NONE 0
#define DHCP_OVERLOAD_FILE 1
#define DHCP_OVERLOAD_SNAME  2
#define DHCP_OVERLOAD_SNAME_FILE 3

#ifdef __cplusplus
}
#endif

#endif /* LWIP_DHCP */

#endif /*__LWIP_DHCP_H__*/
