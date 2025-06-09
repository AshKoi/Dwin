/*
**********************************************************************
*
*				Fujian Sunnanda Communication Corporation
*					Embedded System Building Blocks
*					uC/OS-II 2.83 for at91sam7x256
*
**********************************************************************

----------------------------------------------------------------------
文 件 名	: Main.c
文件描述	: system main function
创建作者	: LWP
创建日期	: 2007.02.01
======================================================================
版本历史	:
----------------------------------------------------------------------
修改日期	:
修改内容	:
----------------------------------------------------------------------
存在问题	:
======================================================================
*/
#include <string.h>
#include <board.h>
#include "lwip/utility/trace.h"
#include "lwip/utility/assert.h"
#include "lwip/others/fm3/periph/aic/aic.h"
#include "lwip/others/fm3/periph/emac/dm9161.h"
#include "lwip/others/fm3/periph/emac/dm9161_define.h"
#include "lwip/include/arch/sys_arch.h"
#include "mb9bf218s.h"
#include "lwip/others/fm3/periph/emac/emac.h"
#include <stdio.h>
#include "lwipopts.h"
#include "lwip/include/netif/etharp.h"
#include <lwip/icmp.h>
#include "lwipopts.h"

#include "pbuf.h"
#include "ethernetif.h"
#include "emac_fm3.h"
FM3_EAC_TypeDef    fm3_emac_device0;
struct netif netif0;
unsigned int Emac_TxBufIndex;
unsigned int Emac_TxRestIndex;	                        // 是否处于发送状态的标志
														// Statistics
int gDriverEmacInitStatus;
void ETHER_MAC0_IRQHandler(void)
{
    EMAC_IRQHandler(&fm3_emac_device0);
}
/*
**********************************************************************
*	函数名称:	 EMAC_WaitPhy
*	函数功能:	
*	入口参数:    retry   等待时间参数
*	出口参数:	 等待    1正常   0超时
**********************************************************************/
void DelayResolution100us(unsigned long Dly)
{
    volatile unsigned long  loop_100us;
    for(;Dly > 0;Dly--)
    {
        for(loop_100us = 550; loop_100us > 0; loop_100us--)
        {  }
    }
}


/*
**********************************************************************
*	函数名称:	 EMAC_EnableMII
*	函数功能:	
*	入口参数:
*	出口参数:	
**********************************************************************/
void EMAC_EnableMII( void )
{
    uint32_t value = 0;
    value |= (1<<8); /* [bit8]: RST0 */
    /* step 2: Select MII/RMII and issue a hardware reset to Ethernet-MAC */
    FM3_ETHERNET_CONTROL->ETH_MODE = value;
}

/*
**********************************************************************
*	函数名称:	 EMAC_EnableRMII
*	函数功能:	
*	入口参数:
*	出口参数:	
**********************************************************************/
void EMAC_EnableRMII( void )
{
    uint32_t value = 1;
    value |= (1<<8); /* [bit8]: RST0 */
    /* step 2: Select MII/RMII and issue a hardware reset to Ethernet-MAC */
    FM3_ETHERNET_CONTROL->ETH_MODE = value;
}

/*
**********************************************************************
*	函数名称:	 EMAC_Handler
*	函数功能:	
*	入口参数:
*	出口参数:	
**********************************************************************/
extern sys_sem_t Lwip_EthInputSem;

static void EMAC_IRQHandler(FM3_EAC_TypeDef * fm3_emac_device)
{
    uint32_t status;
    FM3_ETHERNET_MAC_TypeDef * FM3_ETHERNET_MAC;

    ASSERT(fm3_emac_device==&fm3_emac_device0);

    FM3_ETHERNET_MAC = fm3_emac_device->FM3_ETHERNET_MAC;

    /* get DMA IT status */
    status = FM3_ETHERNET_MAC->SR;

    if ( (status & EMAC_DMA_INT_R) != (uint32_t)RESET ) /* packet reception */
    {
        /* 有一帧需要接受 */
       /* struct pbuf * p;
        p=eth_emac_rx(fm3_emac_device);//for  test only
        if(p!=NULL)
            free(p);
        */
        OSSemPost(Lwip_EthInputSem);
        EMAC_clear_pending(FM3_ETHERNET_MAC, EMAC_DMA_INT_R);

    }

    if ( (status & EMAC_DMA_INT_T) != (uint32_t)RESET ) /* packet transmission */
    {

        /*有一帧需要发送*/

        EMAC_clear_pending(FM3_ETHERNET_MAC, EMAC_DMA_INT_T);
    }

    /* Clear received IT */
    if ((status & EMAC_DMA_INT_NIS) != (uint32_t)RESET)
        FM3_ETHERNET_MAC->SR = (uint32_t)EMAC_DMA_INT_NIS;
    if ((status & EMAC_DMA_INT_AIS) != (uint32_t)RESET)
        FM3_ETHERNET_MAC->SR = (uint32_t)EMAC_DMA_INT_AIS;
    if ((status & EMAC_DMA_INT_RO) != (uint32_t)RESET)
        FM3_ETHERNET_MAC->SR = (uint32_t)EMAC_DMA_INT_RO;

    if ((status & EMAC_DMA_INT_RBU) != (uint32_t)RESET)
    {
        EMAC_resume_reception(FM3_ETHERNET_MAC);
        FM3_ETHERNET_MAC->SR = (uint32_t)EMAC_DMA_INT_RBU;
    }

    if ((status & EMAC_DMA_INT_TBU) != (uint32_t)RESET)
    {
        EMAC_resume_transmission(FM3_ETHERNET_MAC);
        FM3_ETHERNET_MAC->SR = (uint32_t)EMAC_DMA_INT_TBU;
    }
}
static void set_usb_EMAC_clock(void)
{

    /* disable USB and Ethernet clock */
    bFM3_USBETHERNETCLK_UCCR_UCEN0 = 0;
    bFM3_USBETHERNETCLK_UCCR_UCEN1 = 0;
    bFM3_USBETHERNETCLK_UCCR_ECEN  = 0;

    /* wait for USB and Ethernet clock stop */
    while ((bFM3_USBETHERNETCLK_UCCR_UCEN0)
            || (bFM3_USBETHERNETCLK_UCCR_UCEN1)
            || (bFM3_USBETHERNETCLK_UCCR_ECEN));

    /* disable USB-PLL clock */
     /* disable USB/Ethernet-PLL clock */
    bFM3_USBETHERNETCLK_UPCR1_UPLLEN = 0;

    /* select PLL macro clock */
    bFM3_USBETHERNETCLK_UCCR_ECSEL0 = 1;
    bFM3_USBETHERNETCLK_UCCR_ECSEL1 = 0;

    /* select main clock as input clock */
    bFM3_USBETHERNETCLK_UPCR1_UPINC = 0;

    /* select clock stabilization time */
    FM3_USBETHERNETCLK->UPCR2 = 0x07;

    /* USB-PLL clock configuration register(K) initialize */
    //REG_W(FM3_USBCLK->UPCR3, UPCR3_UPLLK_INIT_VALUE);
    FM3_USBETHERNETCLK->UPCR3 = 1-1;

    /* USB-PLL clock configuration register(N) initialize */
    //REG_W(FM3_USBCLK->UPCR4, UPCR4_UPLLN_INIT_VALUE);
    FM3_USBETHERNETCLK->UPCR4 = 50-1; /* N */

    FM3_USBETHERNETCLK->UPCR5 = 4-1; /* M */

    bFM3_USBETHERNETCLK_UPCR7_EPLLEN = 1;

    /* USB-PLL clock stabilize interrupt disable  */

    //bFM3_USBETHERNETCLK_UPINT_ENR_UPCSE = 0;

    /* enable USB-PLL clock */
    //REG_W(bFM3_USBCLK_UPCR1_UPLLEN, BIT_SET);
    bFM3_USBETHERNETCLK_UPCR1_UPLLEN = 1;

    /* wait for USB-PLL clock ready */
    while ((bFM3_USBETHERNETCLK_UP_STR_UPRDY==0))
    {
        ;
    }

    /* enable USB clock */
    //REG_W(bFM3_USBCLK_UCCR_UCEN, BIT_SET);
     bFM3_USBETHERNETCLK_UCCR_UCEN1  = 1;//USB1 clock

    bFM3_USBETHERNETCLK_UCCR_UCEN0 = 1;
    bFM3_USBETHERNETCLK_UCCR_ECEN  = 1;

/* enable Ethernet clock output */
    FM3_USBETHERNETCLK->UCCR |= 1<<4; /* [4] : 1-Enable Ethernet clock output */
    /* wait for 5 cycle */
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();

    /* enable USB controller */
    // REG_W(bFM3_USBCLK_USBEN_USBEN, BIT_SET);
    // bFM3_USBETHERNETCLK_USBEN0_USBEN0 = 1;
     bFM3_USBETHERNETCLK_USBEN1_USBEN1 = 1;//USB1
    bFM3_USBETHERNETCLK_UPCR7_EPLLEN = 1;



    FM3_GPIO->EPFR14 |= (1<<26); /* [bit26] E_CKE: E_COUT Output Select Bit */
    FM3_GPIO->PFRC |= (1<<0x0B); /* PCB: E_COUT */

    return;
}
/*
**********************************************************************
*	函数名称:	 EMAC_Init
*	函数功能:	
*	入口参数:
*	出口参数:	
**********************************************************************/
void EMAC_Init( unsigned char id,  unsigned char *pMacAddress,
                unsigned char enableCAF, unsigned char enableNBC )
{
    uint32_t value = 0;

    /**< IO config */

    FM3_GPIO->PFRC |= (1<<3) | (1<<4) | (1<<5) | (1<<6) | (1<<7) | (1 << 8);
    FM3_GPIO->PFRD |= (1<<1) | (1<<2) | (1<<3);
    value |= (1<<24) ; /* [bit24] E_MD0B :E_MDO0 I/O Select Bit */
    value |= (1<<22) ; /* [bit22] E_MC0E :E_MDC0 Output Select Bit */
    value |= (1<<20) ; /* [bit20] E_TE0E: E_TXEN0 Output Select Bit */
    value |= (1<<18) ; /* [bit18] E_TD0E: E_TX00, E_TX01 Output Select Bit */
    FM3_GPIO->EPFR14 = FM3_GPIO->EPFR14 & 0xFFFF0000 | value;

//set_ethernet_e_cout_clock(1);
    set_usb_EMAC_clock();

    /**< config PHY0_RESET */
    PHY0_RESET_LOW();

     /* delay 10ms */
    DelayResolution100us(100);

    PHY0_RESET_HIGH();
    /* delay 10ms */
    DelayResolution100us(100);


    /* setup Ethernet-MAC */
    //为网卡提供电源
    /* step 1: Start clock supply to Ethernet-MAC */
    FM3_ETHERNET_CONTROL->ETH_CLKG = 1;

    //选择模式MIIorRMII
    value = 1;
    value |= (1<<8); /* [bit8]: RST0 */
    /* step 2: Select MII/RMII and issue a hardware reset to Ethernet-MAC */
    FM3_ETHERNET_CONTROL->ETH_MODE = value;

    /* step 3: */
    /* At this point, clock signal (REF_CLK) must be input from external PHY. */
    /* If the clock signal has not been input, wait until it is input. */
    /* set autonegotiation mode */
    fm3_emac_device0.phy_mode = EMAC_PHY_AUTO;
    fm3_emac_device0.FM3_ETHERNET_MAC = FM3_ETHERNET_MAC0;
    fm3_emac_device0.ETHER_MAC_IRQ  = ETHERNET0_IRQn;

    // OUI 00-00-0E FUJITSU LIMITED
    fm3_emac_device0.dev_addr[0] = pMacAddress[0];
    fm3_emac_device0.dev_addr[1] = pMacAddress[1];
    fm3_emac_device0.dev_addr[2] = pMacAddress[2];
    /* set mac address: (only for test) */
    fm3_emac_device0.dev_addr[3] = pMacAddress[3];
    fm3_emac_device0.dev_addr[4] = pMacAddress[4];
    fm3_emac_device0.dev_addr[5] = pMacAddress[5];

    fm3_emac_device0.parent.eth_rx			 = eth_emac_rx;
    fm3_emac_device0.parent.eth_tx			 = eth_emac_tx;
    fm3_emac_device0.parent.netif = &netif0;
    Lwip_EthInputSem = OSSemCreate(0);//创建信号量
    FM3_ETHERNET_CONTROL->ETH_MODE = 1;
    /* init tx buffer free semaphore */
    eth_emac_init(&fm3_emac_device0);

}
static void eth_DMA_rx_desc_init(EMAC_DMADESCTypeDef *DMARxDescTab, uint8_t *RxBuff, uint32_t RxBuffCount)
{
    uint32_t i = 0;
    EMAC_DMADESCTypeDef *DMARxDesc;

    ASSERT(DMARxDescTab!=NULL);
    ASSERT(RxBuff!=NULL);
    ASSERT(RxBuffCount==EMAC_RXBUFNB);

    /* Fill each DMARxDesc descriptor with the right values */
    for(i=0; i < RxBuffCount; i++)
    {
        /* Get the pointer on the ith member of the Rx Desc list */
        DMARxDesc = DMARxDescTab+i;
        /* Set Own bit of the Rx descriptor Status */
        DMARxDesc->Status = EMAC_DMARxDesc_OWN;

        /* Set Buffer1 size and Second Address Chained bit */
        DMARxDesc->ControlBufferSize = EMAC_DMARxDesc_RCH | (uint32_t)EMAC_MAX_PACKET_SIZE;
        /* Set Buffer1 address pointer */
        DMARxDesc->Buffer1Addr = (uint32_t)(&RxBuff[i*EMAC_MAX_PACKET_SIZE]);

        /* Initialize the next descriptor with the Next Descriptor Polling Enable */
        if(i < (RxBuffCount-1))
        {
            /* Set next descriptor address register with next descriptor base address */
            DMARxDesc->Buffer2NextDescAddr = (uint32_t)(DMARxDescTab+i+1);
        }
        else
        {
            /* For last descriptor, set next descriptor address register equal to the first descriptor base address */
            DMARxDesc->Buffer2NextDescAddr = (uint32_t)(DMARxDescTab);
        }
    }
}
static void eth_DMA_tx_desc_init(EMAC_DMADESCTypeDef *DMATxDescTab, uint8_t* TxBuff, uint32_t TxBuffCount)
{
    uint32_t i = 0;
    EMAC_DMADESCTypeDef *DMATxDesc;

    ASSERT(DMATxDescTab!=NULL);
    ASSERT(TxBuff!=NULL);
    ASSERT(TxBuffCount==EMAC_TXBUFNB);

    /* Fill each DMATxDesc descriptor with the right values */
    for(i=0; i < TxBuffCount; i++)
    {
        /* Get the pointer on the ith member of the Tx Desc list */
        DMATxDesc = DMATxDescTab + i;
        /* Set Second Address Chained bit */
        DMATxDesc->Status = EMAC_DMATxDesc_TCH;

        /* Set Buffer1 address pointer */
        DMATxDesc->Buffer1Addr = (uint32_t)(&TxBuff[i*EMAC_MAX_PACKET_SIZE]);

        /* Initialize the next descriptor with the Next Descriptor Polling Enable */
        if(i < (TxBuffCount-1))
        {
            /* Set next descriptor address register with next descriptor base address */
            DMATxDesc->Buffer2NextDescAddr = (uint32_t)(DMATxDescTab+i+1);
        }
        else
        {
            /* For last descriptor, set next descriptor address register equal to the first descriptor base address */
            DMATxDesc->Buffer2NextDescAddr = (uint32_t) DMATxDescTab;
        }
    }
}


static int32_t eth_emac_init(FM3_EAC_TypeDef *dev)
{
	uint8_t i = 0;
    FM3_EAC_TypeDef * fm3_emac_device;
    FM3_ETHERNET_MAC_TypeDef * FM3_ETHERNET_MAC;

    ASSERT(dev==&fm3_emac_device0);
    ASSERT(EMAC_PHY_AUTO==fm3_emac_device0.phy_mode);
    ASSERT(FM3_ETHERNET_MAC0==fm3_emac_device0.FM3_ETHERNET_MAC);
    ASSERT(ETHER_MAC0_IRQn==fm3_emac_device0.ETHER_MAC_IRQ);

    fm3_emac_device=dev;
    FM3_ETHERNET_MAC = fm3_emac_device->FM3_ETHERNET_MAC;

    /* Software reset */
    FM3_ETHERNET_MAC->BMR |= (1<<0); /* [bit0]SWR (Software Reset) */

    /* Wait for software reset */
    while(FM3_ETHERNET_MAC->BMR & (1<<0))
	{
		i++;
		OSTimeDlyHMSM(0, 0, 0, 10);	
		if (i > 200)
		{
			gDriverEmacInitStatus = -1;
			return RET_ERROR;
		}	
	}

    /* Configure ETHERNET */
    EMAC_init(FM3_ETHERNET_MAC);

    /* Enable DMA Receive interrupt (need to enable in this case Normal interrupt) */
    EMAC_INT_config(FM3_ETHERNET_MAC, EMAC_DMA_INT_NIS | EMAC_DMA_INT_R | EMAC_DMA_INT_T , ENABLE);

    /* Initialize Tx Descriptors list: Chain Mode */
    eth_DMA_tx_desc_init(fm3_emac_device->DMATxDscrTab, &fm3_emac_device->Tx_Buff[0][0], EMAC_TXBUFNB);
    fm3_emac_device->DMATxDescToSet = fm3_emac_device->DMATxDscrTab;
    /* Set Transmit Descriptor List Address Register */
    FM3_ETHERNET_MAC->TDLAR = (uint32_t) fm3_emac_device->DMATxDescToSet;

    /* Initialize Rx Descriptors list: Chain Mode  */
    eth_DMA_rx_desc_init(fm3_emac_device->DMARxDscrTab, &fm3_emac_device->Rx_Buff[0][0], EMAC_RXBUFNB);
    fm3_emac_device->DMARxDescToGet = fm3_emac_device->DMARxDscrTab;
    /* Set Receive Descriptor List Address Register */
    FM3_ETHERNET_MAC->RDLAR = (uint32_t) fm3_emac_device->DMARxDescToGet;

    /* MAC address configuration */
    EMAC_MAC_Addr_config(FM3_ETHERNET_MAC, EMAC_MAC_Address0, (uint8_t*)&fm3_emac_device->dev_addr[0]);

    NVIC_EnableIRQ( fm3_emac_device->ETHER_MAC_IRQ );

    /* Enable MAC and DMA transmission and reception */
    EMAC_start(FM3_ETHERNET_MAC);

    return RET_SUCCESS;

}


/*
**********************************************************************
*	函数名称:	 EMAC_Poll
*	函数功能:	
*	入口参数:
*	出口参数:	
**********************************************************************/
unsigned char EMAC_Poll(unsigned char *pFrame,
						unsigned int frameSize
						,unsigned int *pRcvSize)
{
    unsigned char ret = EMAC_RX_NO_DATA;
    struct pbuf * p;
    p = eth_emac_rx(&fm3_emac_device0);//for  test only
    if(p!=NULL)
    {
        if (frameSize > p->len)
        {
            memcpy(pFrame, p->payload, p->len);
            *pRcvSize = p->len;
            ret = EMAC_RX_OK;
        }
        else
        {
            memcpy(pFrame, p->payload, frameSize);
            *pRcvSize = frameSize;
            ret = EMAC_RX_FRAME_SIZE_TOO_SMALL;
        }

        //EmacStatistics.rx_packets++;
        pbuf_free(p);
    }
    else
    {
        ret = EMAC_RX_NO_DATA;
    }
    return ret;


}


extern unsigned char gDriverEmacDebugLevel;

int Emac_Entry( unsigned char* phwaddr )
{
 	EMAC_Init(0, phwaddr, 0, 0);
    return 1;
}

/*
**********************************************************************
*	函数名称:	 Emac_TransmitPacket
*	函数功能:	
*	入口参数:
*	出口参数:	
**********************************************************************/
int Emac_TransmitPacket(unsigned char *pData, unsigned int length,
						unsigned char end_flag)
{
    struct pbuf buf;
    buf.next=NULL;
    buf.len=length;//74
    buf.tot_len=length;//74
    buf.payload=pData;
    eth_emac_tx(&fm3_emac_device0,&buf);
    return 1;
 }
int32_t eth_emac_tx( struct fm3_emac *dev, struct pbuf *p)
{
    struct pbuf* q;
    uint32_t offset;
    FM3_EAC_TypeDef * fm3_emac_device;
    FM3_ETHERNET_MAC_TypeDef * FM3_ETHERNET_MAC;


    ASSERT(dev==&fm3_emac_device0);
    ASSERT(p!=NULL);

    //fm3_emac_device = (struct fm3_emac *)dev;
    fm3_emac_device =dev;
    FM3_ETHERNET_MAC = fm3_emac_device->FM3_ETHERNET_MAC;



    offset = 0;
    for (q = p; q != NULL; q = q->next)
    {
        uint8_t* ptr;
        uint32_t len;

        len = q->len;
        ptr = q->payload;

        /* Copy the frame to be sent into memory pointed by the current ETHERNET DMA Tx descriptor */
        while (len)
        {
            (*(__IO uint8_t *)((fm3_emac_device->DMATxDescToSet->Buffer1Addr) + offset)) = *ptr;

            offset ++;
            ptr ++;
            len --;
        }
    }

    /* Setting the Frame Length: bits[12:0] */
    fm3_emac_device->DMATxDescToSet->ControlBufferSize = (p->tot_len & EMAC_DMATxDesc_TBS1);
    /* Setting the last segment and first segment bits (in this case a frame is transmitted in one descriptor) */
    fm3_emac_device->DMATxDescToSet->Status |= EMAC_DMATxDesc_LS | EMAC_DMATxDesc_FS;
    /* Enable TX Completion Interrupt */
    fm3_emac_device->DMATxDescToSet->Status |= EMAC_DMATxDesc_IC;

#ifdef CHECKSUM_BY_HARDWARE
    fm3_emac_device->DMATxDescToSet->Status |= EMAC_DMATxDesc_ChecksumTCPUDPICMPFull;
    /* clean ICMP checksum */
    {
        struct eth_hdr *ethhdr = (struct eth_hdr *)(fm3_emac_device->DMATxDescToSet->Buffer1Addr);
        /* is IP ? */
        if( ethhdr->type == htons(ETHTYPE_IP) )
        {
            struct ip_hdr *iphdr = (struct ip_hdr *)(fm3_emac_device->DMATxDescToSet->Buffer1Addr + SIZEOF_ETH_HDR);
            /* is ICMP ? */
            if( IPH_PROTO(iphdr) == IP_PROTO_ICMP )
            {
                struct icmp_echo_hdr *iecho = (struct icmp_echo_hdr *)(fm3_emac_device->DMATxDescToSet->Buffer1Addr + SIZEOF_ETH_HDR + sizeof(struct ip_hdr) );
                iecho->chksum = 0;
            }
        }
    }
#endif
    /* Set Own bit of the Tx descriptor Status: gives the buffer back to ETHERNET DMA */
    fm3_emac_device->DMATxDescToSet->Status |= EMAC_DMATxDesc_OWN;
    /* When Tx Buffer unavailable flag is set: clear it and resume transmission */
    if ((FM3_ETHERNET_MAC->SR & EMAC_DMASR_TBUS) != (uint32_t)RESET)
    {
        /* Clear TBUS ETHERNET DMA flag */
        FM3_ETHERNET_MAC->SR = EMAC_DMASR_TBUS;
        /* Transmit Poll Demand to resume DMA transmission*/
        FM3_ETHERNET_MAC->TPDR = 0;
    }

    /* Update the ETHERNET DMA global Tx descriptor with next Tx decriptor */
    /* Chained Mode */
    /* Selects the next DMA Tx descriptor list for next buffer to send */
    fm3_emac_device->DMATxDescToSet = (EMAC_DMADESCTypeDef*) (fm3_emac_device->DMATxDescToSet->Buffer2NextDescAddr);

    /* Return SUCCESS */
    return SUCCESS;
}

struct pbuf * eth_emac_rx(struct fm3_emac * dev)
{
    struct pbuf* p;
    uint32_t offset = 0, framelength = 0;
    FM3_EAC_TypeDef * fm3_emac_device;
    FM3_ETHERNET_MAC_TypeDef * FM3_ETHERNET_MAC;

    ASSERT(dev==&fm3_emac_device0);

    fm3_emac_device = dev;
    FM3_ETHERNET_MAC = fm3_emac_device->FM3_ETHERNET_MAC;

    /* init p pointer */
    p = NULL;
    /* Check if the descriptor is owned by the ETHERNET DMA (when set) or CPU (when reset) */
    if(((fm3_emac_device->DMARxDescToGet->Status & EMAC_DMARxDesc_OWN) != (uint32_t)RESET))
    {
        return p;
    }

    if (((fm3_emac_device->DMARxDescToGet->Status & EMAC_DMARxDesc_ES) == (uint32_t)RESET) &&
            ((fm3_emac_device->DMARxDescToGet->Status & EMAC_DMARxDesc_LS) != (uint32_t)RESET) &&
            ((fm3_emac_device->DMARxDescToGet->Status & EMAC_DMARxDesc_FS) != (uint32_t)RESET))
    {
        /* Get the Frame Length of the received packet: substruct 4 bytes of the CRC */
        framelength = ((fm3_emac_device->DMARxDescToGet->Status & EMAC_DMARxDesc_FL) >> EMAC_DMARXDESC_FRAME_LENGTHSHIFT) - 4;

        /* allocate buffer */
        p = pbuf_alloc(PBUF_LINK, framelength, PBUF_RAM);
        if (p != NULL)
        {
            uint8_t* ptr;
            struct pbuf* q;
            int32_t len;

            for (q = p; q != NULL; q= q->next)
            {
                ptr = q->payload;
                len = q->len;

                /* Copy the received frame into buffer from memory pointed by the current ETHERNET DMA Rx descriptor */
                while (len)
                {
                    *ptr = (*(__IO uint8_t *)((fm3_emac_device->DMARxDescToGet->Buffer1Addr) + offset));

                    offset ++;
                    ptr ++;
                    len --;
                }
            }

        }
    }

    /* Set Own bit of the Rx descriptor Status: gives the buffer back to ETHERNET DMA */
    fm3_emac_device->DMARxDescToGet->Status = EMAC_DMARxDesc_OWN;

    /* When Rx Buffer unavailable flag is set: clear it and resume reception */
    if ((FM3_ETHERNET_MAC->SR & EMAC_DMASR_RBUS) != (uint32_t)RESET)
    {
        /* Clear RBUS ETHERNET DMA flag */
        FM3_ETHERNET_MAC->SR = EMAC_DMASR_RBUS;
        /* Resume DMA reception */
        FM3_ETHERNET_MAC->RPDR = 0;
    }

    /* Update the ETHERNET DMA global Rx descriptor with next Rx decriptor */
    /* Chained Mode */
    if((fm3_emac_device->DMARxDescToGet->ControlBufferSize & EMAC_DMARxDesc_RCH) != (uint32_t)RESET)
    {
        /* Selects the next DMA Rx descriptor list for next buffer to read */
        fm3_emac_device->DMARxDescToGet = (EMAC_DMADESCTypeDef*) (fm3_emac_device->DMARxDescToGet->Buffer2NextDescAddr);
    }
    else /* Ring Mode */
    {
        if((fm3_emac_device->DMARxDescToGet->ControlBufferSize & EMAC_DMARxDesc_RER) != (uint32_t)RESET)
        {
            /* Selects the first DMA Rx descriptor for next buffer to read: last Rx descriptor was used */
            fm3_emac_device->DMARxDescToGet = (EMAC_DMADESCTypeDef*) (FM3_ETHERNET_MAC->RDLAR);
        }
        else
        {
            /* Selects the next DMA Rx descriptor list for next buffer to read */
            fm3_emac_device->DMARxDescToGet = (EMAC_DMADESCTypeDef*) ((uint32_t)fm3_emac_device->DMARxDescToGet + 0x10 + ((FM3_ETHERNET_MAC->BMR & EMAC_DMABMR_DSL) >> 2));
        }
    }

    return p;
}
unsigned char EMAC_ReadPhy(unsigned char PhyAddress,
                           unsigned char Address,
                           unsigned int *pValue,
                           unsigned int retry)
{
    uint16_t ret = 0;
    ret = EMAC_PHY_read(FM3_ETHERNET_MAC0,PhyAddress,Address);
    if (ret == EMAC_ERROR)
         return 0;
    return ret;
}
uint16_t EMAC_PHY_read(FM3_ETHERNET_MAC_TypeDef * FM3_ETHERNET_MAC, uint16_t PHYAddress, uint16_t PHYReg)
{
    uint32_t value = 0;
    __IO uint32_t timeout = 0;
    __IO    uint32_t data;

    ASSERT(PHYAddress==1);
    ASSERT((PHYReg==PHY_BCR)||(PHYReg==PHY_BSR)||(PHYReg==PHY_SR));

    /* Get the ETHERNET MACMIIAR value */
    value = FM3_ETHERNET_MAC->GAR;
    /* Keep only the CSR Clock Range CR[2:0] bits value */
    value &= ~MACMIIAR_CR_MASK;
    /* Prepare the MII address register value */
    value |=(((uint32_t)PHYAddress<<11) & EMAC_MACMIIAR_PA); /* Set the PHY device address */
    value |=(((uint32_t)PHYReg<<6) & EMAC_MACMIIAR_MR);      /* Set the PHY register address */

    value &= ~EMAC_MACMIIAR_MW;                              /* Set the read mode */

    //value |=0x00000001;//add for test
    value |= EMAC_MACMIIAR_MB;                               /* Set the MII Busy bit */
    /* Write the result value into the MII Address register */
    FM3_ETHERNET_MAC->GAR = value;
    /* Check for the Busy flag */
    do
    {
        timeout++;
        value = FM3_ETHERNET_MAC->GAR;
    }
    while ((value & EMAC_MACMIIAR_MB) && (timeout < (uint32_t)PHY_READ_TO));
    /* Return ERROR in case of timeout */
    if(timeout == PHY_READ_TO)
    {
        return (uint16_t)EMAC_ERROR;
    }
    data=FM3_ETHERNET_MAC->GDR;//test only
    /* Return data register value */
    //return (uint16_t)(FM3_ETHERNET_MAC->GDR);
    return (uint16_t)data;
}

  /*!
******************************************************************************
** \brief  Write to a PHY register
**
** \param FM3_ETHERNET_MAC point to ETHERNET-MAC registers
** \param PHYAddress: PHY device addres,  is the index of one of supported 32 PHY devices.
**   This parameter can be one of the following values: 0,..,31
** \param PHYReg :PHY register address, is the index of one of the 32 PHY register.
** \param  PHYValue: the value to write
** \return  EMAC_ERROR: in case of timeout
**         EMAC_SUCCESS: for correct write
******************************************************************************
*/
unsigned char EMAC_WritePhy(unsigned char PhyAddress,
                            unsigned char Address,
                            unsigned int  Value,
                            unsigned int  retry)
{
    if (EMAC_PHY_write(FM3_ETHERNET_MAC0,PhyAddress,Address,Value) == EMAC_ERROR)
        return 0;
    else
        return 1;
}
uint32_t EMAC_PHY_write(FM3_ETHERNET_MAC_TypeDef * FM3_ETHERNET_MAC, uint16_t PHYAddress, uint16_t PHYReg, uint16_t PHYValue)
{
    uint32_t value = 0;
    __IO uint32_t timeout = 0;

    ASSERT(PHYAddress==1);
    ASSERT((PHYReg==PHY_BCR)||(PHYReg==PHY_BSR));

    /* Get the ETHERNET MACMIIAR value */
    value = FM3_ETHERNET_MAC->GAR;
    /* Keep only the CSR Clock Range CR[2:0] bits value */
    /*~~~~~~~~~~~~~~~~~*/
    do
    {
        timeout++;
        value = FM3_ETHERNET_MAC->GAR;
    }while((value & EMAC_MACMIIAR_MW) && (timeout < (uint32_t)PHY_WRITE_TO));

    if(timeout == PHY_WRITE_TO)
    {
        return EMAC_ERROR;
    }
    /*~~~~~~~~~~~~~~~~~*/
    value &= ~MACMIIAR_CR_MASK;

    /* Prepare the MII register address value */
    value |=(((uint32_t)PHYAddress<<11) & EMAC_MACMIIAR_PA); /* Set the PHY device address */
    value |=(((uint32_t)PHYReg<<6) & EMAC_MACMIIAR_MR);      /* Set the PHY register address */

    value |= EMAC_MACMIIAR_MW;                               /* Set the write mode */


    value |= EMAC_MACMIIAR_MB;                               /* Set the MII Busy bit */
    /* Give the value to the MII data register */
    FM3_ETHERNET_MAC->GDR = PHYValue;
    /* Write the result value into the MII Address register */
    FM3_ETHERNET_MAC->GAR = value;
    /* Check for the Busy flag */
    do
    {
        timeout++;
        value = FM3_ETHERNET_MAC->GAR;
    }
    while ((value & EMAC_MACMIIAR_MB) && (timeout < (uint32_t)PHY_WRITE_TO));
    /* Return ERROR in case of timeout */
    if(timeout == PHY_WRITE_TO)
    {
        return EMAC_ERROR;
    }

    /* Return SUCCESS */
    return EMAC_SUCCESS;
}

/*
uint8_t icmp_payload[]={
                   0x00,0x1e,0xec,0x67,0x18,0x30,    //dt mac 6B
                   0x00,0x00,0x0E,0x22,0x24,0x56,    //src mac 6B
                   0x08,0x00,                        //type 2B
                   0x45,0x00,0x00,0x3c,              //data 60B
                   0xac,0xa9,0x00,0x00,0xff,0x01,
                   0x00,0x00,0xc0,0xa8,0x01,0x1e,//source IP 192.168.1.30
                   0xc0,0xa8,0x01,0x06,0x00,0x00,//dest IP 192.168.1.6
                   0xf0,0x59,0x02,0x00,0x63,0x02,
                   0x61,0x62,0x63,0x64,0x65,0x66,
                   0x67,0x68,0x69,0x6a,0x6b,0x6c,
                   0x6d,0x6e,0x6f,0x70,0x71,0x72,
                   0x73,0x74,0x75,0x76,0x77,0x61,
                   0x62,0x63,0x64,0x65,0x66,0x67,
                   0x68,0x69
                  };
void test_ethernet_driver(void)
{
    struct pbuf buf;

    buf.next=NULL;
    buf.len=0x4A;//74
    buf.tot_len=0x4A;//74
    buf.payload=&icmp_payload;
    EMAC_Init(0, MACaddr0, 0, 0);
  //eth_system_device_init();
    while(1)
    {
        DelayResolution100us(100);
        eth_emac_tx(&fm3_emac_device0,&buf);
    }

}*/