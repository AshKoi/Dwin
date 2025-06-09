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
 * $Id: lwipopts.h, v1.2.0 2007/08/02 9527 $
 */
#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

#include "ucos_ii.h"
//#include "ucos/ucos_ii/UCOS_II.H"
//----------------------------------------------------------------------
/* ---------- OS 接口配置 ---------- */
//----------------------------------------------------------------------
//  EMAC驱动接口初始化任务优先级
#define LWIP_EMAC_INIT_PRIO		55
//	EMAC驱动接口任务优先级
#define LWIP_ETH_INPUT_PRIO		56
//	创建LwIP TCPIP事务处理线程的OS起始优先级
#define LWIP_THREAD_START_PRIO  57
// 硬件接口线程优先级，该优先级相对于LWIP_THREAD_START_PRIO
#define TCPIP_THREAD_PRIO       1		//	TCPIP 线程优先级
#define PPP_THREAD_PRIO         2		//	PPP 线程优先级
#define SLIPIF_THREAD_PRIO      3		//	SLIPIP 串口 IO 线程优先级

//	线程堆栈空间
#define LWIP_THREAD_STK_SIZE	128		// 128 -> 256

//	最大创建线程数目(1-3)，TCPIP、PPP、SLIPIF
#define LWIP_THREAD_MAX			2

// 20130411添加
//#define MAXNAMELEN				64
//#define MAXSECRETLEN			64
//------------------------------------------------
/* Critical Region Protection */
//------------------------------------------------
/** SYS_LIGHTWEIGHT_PROT
 * define SYS_LIGHTWEIGHT_PROT in lwipopts.h if you want inter-task protection
 * for certain critical regions during buffer allocation, deallocation and memory
 * allocation and deallocation.
 */
#define SYS_LIGHTWEIGHT_PROT	1	// default is 0 针对Stellaris必须1，主要是因为在分配内存的时候，要确保总中断关闭。防止内存分配失败。

#define NO_SYS					0


//----------------------------------------------------------------------
/* ---------- Memory options ---------- */
//----------------------------------------------------------------------
#define MEM_LIBC_MALLOC         0

/* MEM_ALIGNMENT: should be set to the alignment of the CPU for which
   lwIP is compiled. 4 byte alignment -> define MEM_ALIGNMENT to 4, 2
   byte alignment -> define MEM_ALIGNMENT to 2. */
#define MEM_ALIGNMENT           4

/* MEM_SIZE: the size of the heap memory. If the application will send
a lot of data that needs to be copied, this should be set high. */
#define MEM_SIZE                3072//3072


#define MEMP_SANITY_CHECK       0

/* MEMP_NUM_PBUF: the number of memp struct pbufs. If the application
   sends a lot of data out of ROM (or other static memory), this
   should be set high. */
#define MEMP_NUM_PBUF           4		// 32 -> 4

/* Number of raw connection PCBs */
#define MEMP_NUM_RAW_PCB        2		// 8 -> 0

/* MEMP_NUM_UDP_PCB: the number of UDP protocol control blocks. One
   per active UDP "connection". */
#define MEMP_NUM_UDP_PCB        4		// 4 -> 0

/* MEMP_NUM_TCP_PCB: the number of simulatenously active TCP
   connections. */
#define MEMP_NUM_TCP_PCB        4		// 8 -> 4

/* MEMP_NUM_TCP_PCB_LISTEN: the number of listening TCP
   connections. */
#define MEMP_NUM_TCP_PCB_LISTEN 4		// 4 -> 0

/* MEMP_NUM_TCP_SEG: the number of simultaneously queued TCP
   segments. */
#define MEMP_NUM_TCP_SEG        4		// 8 -> 4

/* MEMP_NUM_SYS_TIMEOUT: the number of simulateously active
   timeouts. */
#define MEMP_NUM_SYS_TIMEOUT    8		// 8 -> 4


//----------------------------------------------------------------------
/* The following four are used only with the sequential API and can be
   set to 0 if the application only will use the raw API. */
//----------------------------------------------------------------------
/* MEMP_NUM_NETBUF: the number of struct netbufs. */
//#define MEMP_NUM_NETBUF         32		// 8 -> 4

/* MEMP_NUM_NETCONN: the number of struct netconns. */
//#define MEMP_NUM_NETCONN        4		// 8 -> 4

/* MEMP_NUM_APIMSG: the number of struct api_msg, used for
   communication between the TCP/IP stack and the sequential
   programs. */
//#define MEMP_NUM_API_MSG        4		// 8 -> 4

/* MEMP_NUM_TCPIPMSG: the number of struct tcpip_msg, which is used
   for sequential API communication and incoming packets. Used in
   src/api/tcpip.c. */
//#define MEMP_NUM_TCPIP_MSG      4		// 8 -> 4

//----------------------------------------------------------------------
/* ---------- Pbuf options ---------- */
//----------------------------------------------------------------------
/* PBUF_POOL_SIZE: the number of buffers in the pbuf pool. */
#define PBUF_POOL_SIZE          20

/* PBUF_POOL_BUFSIZE: the size of each pbuf in the pbuf pool. */
#define PBUF_POOL_BUFSIZE       128		// 128 -> 256

/* PBUF_LINK_HLEN: the number of bytes that should be allocated for a
   link level header. Defaults to 14 for Ethernet. */
#define PBUF_LINK_HLEN          14


//----------------------------------------------------------------------
/* ---------- ARP options ---------- */
//----------------------------------------------------------------------
/** Number of active hardware address, IP address pairs cached */
#define ARP_TABLE_SIZE			10

/**
 * If enabled, outgoing packets are queued during hardware address
 * resolution.
 *
 * This feature has not stabilized yet. Single-packet queueing is
 * believed to be stable, multi-packet queueing is believed to
 * clash with the TCP segment queueing.
 * 
 * As multi-packet-queueing is currently disabled, enabling this
 * _should_ work, but we need your testing feedback on lwip-users.
 *
 */
#define ARP_QUEUEING			1	// if TCP was used, must disable this in v1.1.0


//----------------------------------------------------------------------
/* ---------- IP options ---------- */
//----------------------------------------------------------------------
/* Define IP_FORWARD to 1 if you wish to have the ability to forward
   IP packets across network interfaces. If you are going to run lwIP
   on a device with only one network interface, define this to 0. */
#define IP_FORWARD              0

/* If defined to 1, IP options are allowed (but not parsed). If
   defined to 0, all packets with IP options are dropped. */
#define IP_OPTIONS              1

/** IP reassembly and segmentation. Even if they both deal with IP
 *  fragments, note that these are orthogonal, one dealing with incoming
 *  packets, the other with outgoing packets
 */

/** Reassemble incoming fragmented IP packets */
#define IP_REASSEMBLY           1

/** Fragment outgoing IP packets if their size exceeds MTU */
#define IP_FRAG                 1

/* IP reassemly default age in seconds */
#define IP_REASS_MAXAGE 		30

/* IP reassembly buffer size (minus IP header) */
#define IP_REASS_BUFSIZE 		1024

/* Assumed max MTU on any interface for IP frag buffer */
#define IP_FRAG_MAX_MTU 		1500//1500

/** Global default value for Time To Live used by transport layers. */
#define IP_DEFAULT_TTL          255


//----------------------------------------------------------------------
/* ---------- ICMP options ---------- */
//----------------------------------------------------------------------

#define ICMP_TTL                (IP_DEFAULT_TTL)


//----------------------------------------------------------------------
/* ---------- RAW options ---------- */
//----------------------------------------------------------------------

#define LWIP_RAW                1

#define RAW_TTL                 (IP_DEFAULT_TTL)


//----------------------------------------------------------------------
/* ---------- DHCP options ---------- */
//----------------------------------------------------------------------

/* Define LWIP_DHCP to 1 if you want DHCP configuration of
   interfaces. DHCP is not implemented in lwIP 0.5.1, however, so
   turning this on does currently not work. */
#define LWIP_DHCP               1

/* 1 if you want to do an ARP check on the offered address
   (recommended). */
#define DHCP_DOES_ARP_CHECK     0

//----------------------------------------------------------------------
/* ---------- SNMP options ---------- */
//----------------------------------------------------------------------
/** @note UDP must be available for SNMP transport */
#define LWIP_SNMP                       0

/** @note At least one request buffer is required.  */
#define SNMP_CONCURRENT_REQUESTS 		1

/** @note At least one trap destination is required */
#define SNMP_TRAP_DESTINATIONS          1

#define SNMP_PRIVATE_MIB                0


//----------------------------------------------------------------------
/* ---------- UDP options ---------- */
//----------------------------------------------------------------------
#define LWIP_UDP                1

#define UDP_TTL                 (IP_DEFAULT_TTL)


//----------------------------------------------------------------------
/* ---------- TCP options ---------- */
//----------------------------------------------------------------------
#define LWIP_TCP                1

#define TCP_TTL                 (IP_DEFAULT_TTL)

/* TCP receive window. */
#define TCP_WND                 2048

/* Maximum number of retransmissions of data segments. */
#define TCP_MAXRTX              12

/* Maximum number of retransmissions of SYN segments. */
#define TCP_SYNMAXRTX           6

/* Controls if TCP should queue segments that arrive out of
   order. Define to 0 if your device is low on memory. */
#define TCP_QUEUE_OOSEQ         1

/* TCP Maximum segment size. */
#define TCP_MSS                 1024		// 512 -> 1024

/* TCP sender buffer space (bytes). */
#define TCP_SND_BUF             512			// 256 -> 512

/* TCP sender buffer space (pbufs). This must be at least = 2 *
   TCP_SND_BUF/TCP_MSS for things to work. */
#define TCP_SND_QUEUELEN		4 * TCP_SND_BUF/TCP_MSS


//----------------------------------------------------------------------
/* ---------- Other options ---------- */
//----------------------------------------------------------------------

/* Support loop interface (127.0.0.1) */
/*	是否开启回环，没有网卡驱动，可以设置为 1 添加loop设备进行调试运行。*/
#define LWIP_HAVE_LOOPIF		1

/* */
#define LWIP_COMPAT_SOCKETS     1


//----------------------------------------------------------------------
/* ---------- Socket Options ---------- */
//----------------------------------------------------------------------

/* Enable SO_REUSEADDR and SO_REUSEPORT options */ 
#define SO_REUSE 				0

/* 启用TCP KEEPALIVE机制，可快速检测到网线断开，需配合setsockopt使用 */
#define LWIP_TCP_KEEPALIVE      1


#define LWIP_DEBUG_MSG_SPFING	0
 #define LWIP_DEBUG_MSG_WEB  	1
#define at91_debug_printk    TRACE_ERROR
//----------------------------------------------------------------------
/* ---------- Statistics options ---------- */
//----------------------------------------------------------------------

#define LWIP_STATS				0
#define LWIP_TRACE_ERROR	    	0
#define LWIP_LISTEN_TRACE_ERROR		0
#define LWIP_HTTP_LISTEN_TRACE_ERROR  0
#if LWIP_STATS
	#define LWIP_STATS_DISPLAY	1
	#define LINK_STATS			1
	#define IP_STATS			1
	#define IPFRAG_STATS		1
	#define ICMP_STATS			1
	#define UDP_STATS			1
	#define TCP_STATS			1
	#define MEM_STATS			1
	#define MEMP_STATS			1
	#define PBUF_STATS			1
	#define SYS_STATS			1
	#define RAW_STATS			1
#endif /* LWIP_STATS */


//----------------------------------------------------------------------
/* ---------- PPP options ---------- */
//----------------------------------------------------------------------
#define PPP_SUPPORT             1      /* Set for PPP */

#if PPP_SUPPORT
#define PPPOE_SUPPORT			1

#define PAP_SUPPORT             1
#define CHAP_SUPPORT            1
#define MD5_SUPPORT             1
#endif

//----------------------------------------------------------------------
/* ------------if you need to do debug-------------*/
//----------------------------------------------------------------------
/*
define LWIP_DEBUG in compiler    and following...
*/
/* DBG_LEVEL_WARNING DBG_LEVEL_SERIOUS DBG_LEVEL_SEVERE */
#define DBG_MIN_LEVEL			DBG_LEVEL_SERIOUS

/* 0 DBG_TRACE | DBG_STATE |DBG_FRESH | DBG_HALT */
#define DBG_TYPES_ON			DBG_TRACE | DBG_STATE |DBG_FRESH | DBG_HALT
/*
Then, define debug class in opt.h
* --------------------------------------------------*/

#define ETHARP_DEBUG            LWIP_DBG_OFF
#define NETIF_DEBUG             LWIP_DBG_OFF
#define PBUF_DEBUG              LWIP_DBG_OFF
#define API_LIB_DEBUG           LWIP_DBG_OFF
#define API_MSG_DEBUG           LWIP_DBG_OFF
#define SOCKETS_DEBUG           LWIP_DBG_OFF
#define ICMP_DEBUG              LWIP_DBG_OFF
#define INET_DEBUG              LWIP_DBG_OFF
#define IP_DEBUG                LWIP_DBG_OFF
#define IP_REASS_DEBUG          LWIP_DBG_OFF
#define RAW_DEBUG               LWIP_DBG_OFF
#define MEM_DEBUG               LWIP_DBG_OFF
#define MEMP_DEBUG              LWIP_DBG_OFF
#define SYS_DEBUG               LWIP_DBG_OFF
#define TCP_DEBUG               LWIP_DBG_OFF
#define TCP_INPUT_DEBUG         LWIP_DBG_OFF
#define TCP_FR_DEBUG            LWIP_DBG_OFF
#define TCP_RTO_DEBUG           LWIP_DBG_OFF
//#define TCP_REXMIT_DEBUG        LWIP_DBG_OFF
#define TCP_CWND_DEBUG          LWIP_DBG_OFF
#define TCP_WND_DEBUG           LWIP_DBG_OFF
#define TCP_OUTPUT_DEBUG        LWIP_DBG_OFF
#define TCP_RST_DEBUG           LWIP_DBG_OFF
#define TCP_QLEN_DEBUG          LWIP_DBG_OFF
#define UDP_DEBUG               LWIP_DBG_OFF
#define TCPIP_DEBUG             LWIP_DBG_OFF
#define PPP_DEBUG               LWIP_DBG_OFF
#define SLIP_DEBUG              LWIP_DBG_OFF
#define DHCP_DEBUG              LWIP_DBG_OFF

#define ETHARP_STATS			LWIP_DBG_OFF

//#define RAW_STATS   0
//#define ETHARP_QUEUE_FIRST 0

//#define ETHARP_ALWAYS_INSERT 0

#endif /* __LWIPOPTS_H__ */
