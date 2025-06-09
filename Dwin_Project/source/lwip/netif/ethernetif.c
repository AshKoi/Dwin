/**
 * @file
 * Ethernet Interface Skeleton
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

/*
 * This file is a skeleton for developing Ethernet network interface
 * drivers for lwIP. Add code to the low_level functions and do a
 * search-and-replace for the word "ethernetif" to replace it with
 * something that better describes your network interface.
 */

#include "lwip/include/lwip/opt.h"

#if 1/* don't build, this is only a skeleton, see previous comment */

#include "lwip/include/lwip/def.h"
#include "lwip/include/lwip/mem.h"
#include "lwip/include/lwip/pbuf.h"
#include "lwip/include/lwip/sys.h"
#include "lwip/include/lwip/stats.h"
#include "lwip/include/lwip/snmp.h"
#include "lwip/include/netif/etharp.h"
#include "lwip/include/netif/ppp_oe.h"
#include <fm3/periph/emac/dm9161.h>
#include "user/sys/sys_general.h"
#include <fm3/periph/emac/dm9161_define.h>
#include <string.h>
/* Define those to better describe your network interface. */
static  INT8U ethernet_continu;
RxBuffer  Emac_RxBuffer	;
/* 接收任务信号量*/
sys_sem_t Lwip_EthInputSem;

uint8_t MACaddr0[6]={0x00,0x00,0x0E,0x21,0x27,0x58};
static OS_EVENT* Lwip_BlockOutput = NULL;
INT8U    Emac_NoPbuf_GetCnt;
//static const struct eth_addr ethbroadcast = {{0xff,0xff,0xff,0xff,0xff,0xff}};

//#define LWIP_ETH_INPUT_PRIO			55
#define	LWIP_ETH_INPUT_STK_SIZE		128
#define IFNAME0 'e'
#define IFNAME1 'n'
int Emac_Entry( unsigned char* phwaddr );
OS_STK Lwip_EthInputStk[LWIP_ETH_INPUT_STK_SIZE];

void  arp_timer(void *arg);
u16_t sys_ms_to_ticks(u32_t time_ms);
int Emac_TransmitPacket(unsigned char *pData, unsigned int length, unsigned char end_flag);

/**
 * Helper struct to hold private data used to operate your ethernet interface.
 * Keeping the ethernet address of the MAC in this struct is not necessary
 * as it is already kept in the struct netif.
 * But this is only an example, anyway...
 */
struct ethernetif {
  struct eth_addr *ethaddr;
  /* Add whatever per-interface state that is needed here. */
};


/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
static void
low_level_init(struct netif *netif)
{
  //struct ethernetif *ethernetif = netif->state;
 //  at91_debug_printk("low_level_init\n");
  /* set MAC hardware address length */
    netif->hwaddr_len = ETHARP_HWADDR_LEN;
	#if  LWIP_TRACE_ERROR
	TRACE_ERROR("low_level_init\n");
	#endif
	
/*  会导致传入Mac地址被修改同一
    netif->hwaddr[0] =  MACaddr0[0];
    netif->hwaddr[1] =  MACaddr0[1];
    netif->hwaddr[2] =  MACaddr0[2];
    netif->hwaddr[3] =  MACaddr0[3];
    netif->hwaddr[4] =  MACaddr0[4];
    netif->hwaddr[5] =  MACaddr0[5];
*/
	
  /* maximum transfer unit */
    netif->mtu = 1500;

  /* device capabilities */
  /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
     netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;

  /* Do whatever else is needed to initialize interface. */
    if( Emac_Entry( netif->hwaddr ) > 0 )
	    OSTaskCreate((void (*)(void *))(ethernetif_input), netif, &Lwip_EthInputStk[LWIP_ETH_INPUT_STK_SIZE-1], LWIP_ETH_INPUT_PRIO);

}

void low_level_deinit( void )
{
	INT8U err;
	if( Lwip_BlockOutput != NULL )
	{
		OSMutexDel( Lwip_BlockOutput, OS_DEL_ALWAYS, &err );
		Lwip_BlockOutput = NULL;
	}
	
	if( Lwip_EthInputSem != NULL )
	{
		OSSemDel( Lwip_EthInputSem, OS_DEL_ALWAYS, &err );
		Lwip_EthInputSem = NULL;
	}
	OSTaskDel( LWIP_ETH_INPUT_PRIO );
}

 /*
**********************************************************************
*	函数名称:	low_level_output():
*	函数功能:	得到一整帧数据
*	入口参数:	
*	出口参数:	
**********************************************************************
*/
static err_t
low_level_output(struct netif *pstNetif, struct pbuf *pstPbuf)
{
	struct pbuf *lwip_send_pbuf = pstPbuf;
	err_t err_return = ERR_OK;
	INT8U err_temp=0;
     #if  LWIP_TRACE_ERROR
	//TRACE_ERROR("low_level_output\n");
	#endif
	if(Lwip_BlockOutput == NULL)
	{
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// 2007-08-19 HCF 将互斥型信号量Lwip_BlockOutput的优先级5改成0
		// 因为优先级为5的任务已经被建立，而0优先级是空闲的。
	
		Lwip_BlockOutput =OSMutexCreate(0, &err_temp);
	}

	// 阻塞对EMAC的访问，以避免不同的任务同时访问EMAC造成访问冲突的问题，最长等待时间是2秒
	OSMutexPend(Lwip_BlockOutput, sys_ms_to_ticks(2000), &err_temp);

	if(OS_NO_ERR == err_temp)
	{
		for(; lwip_send_pbuf!=NULL; lwip_send_pbuf=lwip_send_pbuf->next)
		{
			// 发送pbuf中的数据，每次一个pbuf，如果__pstSendPbuf->next指针为空则表明已经到达pbuf链表的末尾
		    if(!Emac_TransmitPacket(lwip_send_pbuf->payload, lwip_send_pbuf->len, (lwip_send_pbuf->next == NULL)))
			{
				err_return = ~ERR_OK;
				break;
			}
		}

        OSMutexPost(Lwip_BlockOutput);
	}

    return err_return;
}

/**********************************************************************
*	函数名称:	 low_level_input():
*	函数功能:	 链路层发送函数，实现err_t (* linkoutput)接口。
*	入口参数:		    sizeof(Emac_RxBuffer.Buffer)
*	出口参数:	
**********************************************************************/
static struct pbuf *low_level_input(struct netif *netif)
{
    struct pbuf* lwip_pst_pbuf = NULL, *lwip_pst_cur_pbuf;
	unsigned int frmSize;
	unsigned int  count,i=0;
	ethernet_continu = 0;
	//需要初始化
	if(Emac_RxBuffer.status== 0)
	{
		if(EMAC_Poll(Emac_RxBuffer.Buffer,EMAC_FRAME_LENTGH_MAX, &frmSize)==EMAC_RX_OK )
		{
			Emac_RxBuffer.status = 1;
			ethernet_continu = 1;

			if(frmSize == 351) {
				    frmSize = frmSize;
				}
		}

	}
	if(Emac_RxBuffer.status == 1)
	{
		/*if (strstr(Emac_RxBuffer.Buffer,"zzwhappy"))
		{
				bFM3_GPIO_PDOR4_P2=0;
		}*/
	     	if(frmSize == 351) {
				    frmSize = 351;
				}
			lwip_pst_pbuf = pbuf_alloc(PBUF_RAW,frmSize,PBUF_POOL);
			//如果申请到内存
			if(lwip_pst_pbuf!=NULL)
			{
			    if(frmSize >= 351) {
				    frmSize = frmSize;
				}
			   //将数据放到pbuf内存中
				 for(lwip_pst_cur_pbuf=lwip_pst_pbuf; lwip_pst_cur_pbuf!=NULL; lwip_pst_cur_pbuf=lwip_pst_cur_pbuf->next)
			    {		
					
			        for(count = 0; count < lwip_pst_cur_pbuf->len;count++)
			        {
						*((unsigned char *)(lwip_pst_cur_pbuf->payload)+count) = *(Emac_RxBuffer.Buffer+i);
						i++;

			        }
			    } 	
				 ethernet_continu = 0;
				 Emac_RxBuffer.status = 0;	//将缓存区的读取状态位置一
				 Emac_NoPbuf_GetCnt=0;	  	//缓存区申请不到进行计数 清零
			}
			else
			{
				  Emac_NoPbuf_GetCnt++;	  //如如果申请不到内存计数
			}
   }
   
    return  lwip_pst_pbuf;
}
/*
**********************************************************************
*	函数名称:	ethernetif_input(struct netif *netif)
*	函数功能:	 实现接收线程，识别数据包是ARP包还是IP包。
*	入口参数:	
*	出口参数:	
**********************************************************************
*/
static void ethernetif_input(struct netif *netif)
{
    struct eth_hdr *ethhdr;
    struct pbuf *p;
    unsigned char  temp_err;
	
	//TRACE_DEBUG("start thread: ethernetif_input\n\r");
	//OSTimeDlyHMSM( 0,0,1,0 );
	while(1)
	{
		//TRACE_DEBUG("     while\n\r");
		do{
			  /* move received packet into a new pbuf */
			p = low_level_input(netif);
			if((p == NULL) && (ethernet_continu == 0))
			{ 		
	            OSSemPend(Lwip_EthInputSem,sys_ms_to_ticks(50), &temp_err);//	
			}
		}while(p == NULL);
		ethhdr = p->payload;
		//TRACE_DEBUG("     ethhdr type=%d\n\r", htons(ethhdr->type) );
		switch (htons(ethhdr->type))
		{
			  /* IP or ARP packet? */
			  case ETHTYPE_IP:
			  case ETHTYPE_ARP:
			#if PPPOE_SUPPORT
			  /* PPPoE packet? */
			  case ETHTYPE_PPPOEDISC:
			  case ETHTYPE_PPPOE:
			#endif /* PPPOE_SUPPORT */
			    /* full packet send to tcpip_thread to process */
			    if (netif->input(p, netif)!=ERR_OK)
			     {
                     pbuf_free(p);
                     p = NULL;
			     }
			    break;

			  default:
			    pbuf_free(p);
			    p = NULL;
			    break;
			  }
	}
}
/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t
ethernetif_init(struct netif *netif)
{
  struct ethernetif *ethernetif;

  LWIP_ASSERT("netif != NULL", (netif != NULL));

  ethernetif = mem_malloc(sizeof(struct ethernetif));
  if (ethernetif == NULL) {
    LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_init: out of memory\n"));
    return ERR_MEM;
  }

#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

  /*
   * Initialize the snmp variables and counters inside the struct netif.
   * The last argument should be replaced with your link speed, in units
   * of bits per second.
   */

  NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, LINK_SPEED_OF_YOUR_NETIF_IN_BPS);

  netif->state = ethernetif;
  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;
  /* We directly use etharp_output() here to save a function call.
   * You can instead declare your own function an call etharp_output()
   * from it if you have to do some checks before sending (e.g. if link
   * is available...) */

  netif->output = etharp_output;

  netif->linkoutput = low_level_output;

  ethernetif->ethaddr = (struct eth_addr *)&(netif->hwaddr[0]);

  /* initialize the hardware */

  low_level_init(netif);

  etharp_init();

  sys_timeout(ARP_TMR_INTERVAL, arp_timer, NULL);

  return ERR_OK;
}

err_t
ethernetif_deinit( void )
{
	low_level_deinit();
    return 1;
}

	
#endif /* 0 */
