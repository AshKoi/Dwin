/*!
 ******************************************************************************
 **
 ** \file emac_fm3.c
 **
 ** \brief MAC driver .
 **
 ** \author FSLA AE Team
 **
 ** \version V1.0
 **
 ** \date 2012-5-28
 **
 ** \attention THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU SEMICONDUCTOR
 **            ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
 **            OMMISSIONS.
 **
 ** (C) Copyright 200x-201x by Fujitsu Semiconductor(Shanghai) Co.,Ltd.
 **
 ******************************************************************************
 **
 ** \note Other information.
 **
 ******************************************************************************
 */

/*---------------------------------------------------------------------------*/
/* include files                                                                                                                 */
/*---------------------------------------------------------------------------*/
#include "emac_fm3.h"
#include "emac_api.h"
#include "others/fm3/periph/emac/emac.h"
#include "debug.h" //enable debug message output to the Terminal I/O of IAR
#define ENABLE_AUTO_DETCT //enable speed and duplex auto negotiation
/*---------------------------------------------------------------------------*/
/* local functions prototypes                                                                                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* global data                                                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* global functions                                                                                                            */
/*---------------------------------------------------------------------------*/
extern void DelayResolution100us(unsigned long Dly);


/*!
 ******************************************************************************
 ** \brief Initializes the ETHERNET peripheral according to the specified
 **
 **
 ** \param FM3_ETHERNET_MAC point to ETHERNET-MAC registers
 **
 ** \return EMAC_SUCCESS Ethernet configuration success
 ******************************************************************************
 */
uint32_t RegValue = 0;
int i = 0;
uint32_t EMAC_init(FM3_ETHERNET_MAC_TypeDef * FM3_ETHERNET_MAC)
{

    uint32_t  value = 0;
    __IO uint32_t timeout = 0;


    /*-------------------------------- MAC Config ------------------------------*/
    /*---------------------- ETHERNET MACMIIAR Configuration -------------------*/
    /* Get the ETHERNET MACMIIAR value */
    value = FM3_ETHERNET_MAC->GAR;
    /* Clear CSR Clock Range CR[2:0] bits */
    value &= MACMIIAR_CR_MASK;

    /* Get hclk frequency value */
    /* Set CR bits depending on hclk value */
    if((SystemCoreClock >= 20000000)&&(SystemCoreClock < 35000000))
    {
        /* CSR Clock Range between 20-35 MHz */
        value |= (uint32_t)EMAC_MACMIIAR_CR_Div16;
    }
    else if((SystemCoreClock >= 35000000)&&(SystemCoreClock < 60000000))
    {
        /* CSR Clock Range between 35-60 MHz */
        value |= (uint32_t)EMAC_MACMIIAR_CR_Div26;
    }
    else if((SystemCoreClock >= 60000000)&&(SystemCoreClock <= 100000000))
    {
        /* CSR Clock Range between 60-100 MHz */
        value |= (uint32_t)EMAC_MACMIIAR_CR_Div42;
    }
    else if((SystemCoreClock >= 100000000)&&(SystemCoreClock <= 150000000))
    {
        /* CSR Clock Range between 100-150 MHz */
        value |= (uint32_t)EMAC_MACMIIAR_CR_Div62;
    }
    else if((SystemCoreClock >= 150000000)&&(SystemCoreClock <= 250000000))
    {
        /* CSR Clock Range between 150-250 MHz */
        value |= (uint32_t)EMAC_MACMIIAR_CR_Div102;
    }
    else /* if((SystemCoreClock >= 250000000)&&(SystemCoreClock <= 300000000)) */
    {
        /* CSR Clock Range between 250-300 MHz */
        value |= (uint32_t)EMAC_MACMIIAR_CR_Div122;
    }
    /* Write to ETHERNET MAC MIIAR: Configure the ETHERNET CSR Clock Range */
    FM3_ETHERNET_MAC0->GAR = (uint32_t)value;

/*---------------PHY autonegotiation begin------------*/

#ifdef ENABLE_AUTO_DETCT
    uint32_t DM=0;
    uint32_t FES=0;
    int PHY_Add = 0;
    for (i = 1; i < 64;i ++)
    {
         RegValue = 0xffff;
         RegValue = EMAC_PHY_read(FM3_ETHERNET_MAC, i, 2);
         if (RegValue != 0xffff && RegValue != 0)
         {
            PHY_Add = i;
            break;
         }
    }
	
	if (PHY_Add == 0)
	{
		gDriverEmacInitStatus = -1;
        /* Return ERROR in case of write timeout */
        return EMAC_ERROR;
	}
    if(!(EMAC_PHY_write(FM3_ETHERNET_MAC,PHY_Add, PHY_BCR, PHY_Reset)))
    {
		gDriverEmacInitStatus = -1;
        /* Return ERROR in case of write timeout */
        return EMAC_ERROR;
    }
				
	OSTimeDlyHMSM(0, 0, 0, 100);
//    DelayResolution100us(20000);//wait for 2 seconds autonegotiation
	RegValue = 0xffff;
	RegValue = EMAC_PHY_read(FM3_ETHERNET_MAC, 32, 2);
	RegValue++;
      /* We wait for linked status... */

	while (!(EMAC_PHY_read(FM3_ETHERNET_MAC,PHY_Add, PHY_BSR) & PHY_Linked_Status) && (timeout < PHY_READ_TO))
    {
        timeout++;
		OSTimeDlyHMSM(0, 0, 0, 100);	
//		DelayResolution100us(1000);
		if(timeout==50)
		{
			bFM3_GPIO_PDOR0_P2 = 0;			//3秒检测不到连接则复位以太网芯片
			gDriverEmacInitStatus = -1;
			return EMAC_ERROR;

		}
    }

    /* Return ERROR in case of timeout */
    if(timeout == PHY_READ_TO)
    {
		gDriverEmacInitStatus = -1;
        return EMAC_ERROR;
    }


    if(!(EMAC_PHY_write(FM3_ETHERNET_MAC,PHY_Add, PHY_BCR,PHY_AutoNegotiation )))//
    {
		gDriverEmacInitStatus = -1;
        /* Return ERROR in case of write timeout */
        return EMAC_ERROR;
    }
   /* Wait until the auto-negotiation will be completed */
	
    while (!(EMAC_PHY_read(FM3_ETHERNET_MAC,PHY_Add, PHY_BSR) & PHY_AutoNego_Complete) && (timeout < (uint32_t)PHY_READ_TO))
	{
        timeout++;
		OSTimeDlyHMSM(0, 0, 0, 100);	
//		DelayResolution100us(1000);
    } ;

    /* Return ERROR in case of timeout */
    if(timeout == PHY_READ_TO)
    {
		gDriverEmacInitStatus = -1;
        return EMAC_ERROR;
    }

    /* Reset Timeout counter */
    timeout = 0;
    RegValue = 0;
    /* Read the result of the auto-negotiation */
    RegValue = EMAC_PHY_read(FM3_ETHERNET_MAC,PHY_Add, 31);
    RegValue = RegValue & ((uint16_t)0x001c);
    if (RegValue == 0x0004)
    {
        DM = EMAC_Mode_HalfDuplex;
        FES = EMAC_Speed_10M;
    }
    else if (RegValue == 0x0014)
    {
        DM = EMAC_Mode_FullDuplex;
        FES = EMAC_Speed_10M;
    }
    else if (RegValue == 0x0008)
    {
        DM = EMAC_Mode_HalfDuplex;
        FES = EMAC_Speed_100M;
    }
    else
    {
        DM = EMAC_Mode_FullDuplex;
        FES = EMAC_Speed_100M;
    }

#endif
/*---------------PHY autonegotiation end--------------*/


    /*------------------------ ETHERNET MACCR Configuration --------------------*/
    /* Get the ETHERNET MACCR value */
    value = FM3_ETHERNET_MAC->MCR;
    /* Clear WD, PCE, PS, TE and RE bits */
    value &= MACCR_CLEAR_MASK;

    value |= (uint32_t)(EMAC_Watchdog_Enable |
                        EMAC_Jabber_Enable |
                        EMAC_InterFrameGap_96Bit |
                        EMAC_CarrierSense_Enable |
#ifdef ENABLE_AUTO_DETCT
                        FES|
#else
                        EMAC_Speed_100M |
#endif
                        EMAC_ReceiveOwn_Enable |
                        EMAC_LoopbackMode_Disable |
#ifdef ENABLE_AUTO_DETCT
                        DM|
#else
                        EMAC_Mode_FullDuplex|
#endif
                        EMAC_ChecksumOffload_Enable |
                        EMAC_RetryTransmission_Disable |
                        EMAC_AutomaticPadCRCStrip_Disable |
                        EMAC_BackOffLimit_10 |
                        EMAC_DeferralCheck_Disable);

    /* Write to ETHERNET MACCR */
    value |= (1<<15);
    value &= ~(1<<25);
    value &= ~(1<<24);
    FM3_ETHERNET_MAC->MCR = (uint32_t)value;

    /*----------------------- ETHERNET MACFFR Configuration --------------------*/
    /* Write to ETHERNET MACFFR */
    FM3_ETHERNET_MAC->MFFR = (uint32_t)(EMAC_ReceiveAll_Enable |
                                        EMAC_SourceAddrFilter_Disable |
                                        EMAC_PassControlFrames_BlockAll |
                                        EMAC_BroadcastFramesReception_Disable |
                                        EMAC_DestinationAddrFilter_Normal |
                                        EMAC_PromiscuousMode_Disable |
                                        EMAC_MulticastFramesFilter_Perfect |
                                        EMAC_UnicastFramesFilter_Perfect);

    /*--------------- ETHERNET MACHTHR and MACHTLR Configuration ---------------*/
    /* Write to ETHERNET MACHTHR */
    FM3_ETHERNET_MAC->MHTRH = 0;
    /* Write to ETHERNET MACHTLR */
    FM3_ETHERNET_MAC->MHTRL = 0;
    /*----------------------- ETHERNET MACFCR Configuration --------------------*/
    /* Get the ETHERNET MACFCR value */
    value = FM3_ETHERNET_MAC->FCR;
    /* Clear xx bits */
    value &= MACFCR_CLEAR_MASK;

    value |= (uint32_t)((0 << 16) |
                        EMAC_ZeroQuantaPause_Disable |
                        EMAC_PauseLowThreshold_Minus4 |
                        EMAC_UnicastPauseFrameDetect_Disable |
                        EMAC_ReceiveFlowControl_Disable |
                        EMAC_TransmitFlowControl_Disable);

    /* Write to ETHERNET MACFCR */
    FM3_ETHERNET_MAC->FCR = (uint32_t)value;
    /*----------------------- ETHERNET MACVLANTR Configuration -----------------*/
    FM3_ETHERNET_MAC->VTR = (uint32_t)(EMAC_VLANTagComparison_16Bit |
                                       0);

    /*-------------------------------- DMA Config ------------------------------*/
    /*----------------------- ETHERNET DMAOMR Configuration --------------------*/
    /* Get the ETHERNET DMAOMR value */
    value = FM3_ETHERNET_MAC->OMR;
    /* Clear xx bits */
    value &= DMAOMR_CLEAR_MASK;

    value |= (uint32_t)(EMAC_DropTCPIPChecksumErrorFrame_Disable |
                        EMAC_ReceiveStoreForward_Enable |
                        EMAC_FlushReceivedFrame_Enable |
                        EMAC_TransmitStoreForward_Enable |
                        EMAC_TransmitThresholdControl_64Bytes |
                        EMAC_ForwardErrorFrames_Disable |
                        EMAC_ForwardUndersizedGoodFrames_Disable |
                        EMAC_ReceiveThresholdControl_64Bytes |
                        EMAC_SecondFrameOperate_Disable);

    /* Write to ETHERNET DMAOMR */
    FM3_ETHERNET_MAC->OMR = (uint32_t)value;

    /*----------------------- ETHERNET DMABMR Configuration --------------------*/
    FM3_ETHERNET_MAC->BMR = (uint32_t)(EMAC_AddressAlignedBeats_Enable |
                                       EMAC_FixedBurst_Enable |
                                       EMAC_RxDMABurstLength_32Beat | /* !! if 4xPBL is selected for Tx or Rx it is applied for the other */
                                       EMAC_TxDMABurstLength_32Beat |
                                       (0 << 2) |
                                       EMAC_DMAArbitration_RoundRobin_RxTx_2_1 |
                                       EMAC_DMABMR_USP); /* Enable use of separate PBL for Rx and Tx */

    /* Return Ethernet configuration success */
    return EMAC_SUCCESS;
}

 /*!
 ******************************************************************************
 ** \brief Enables or disables the specified ETHERNET DMA interrupts.
 **
 ** \param FM3_ETHERNET_MAC point to ETHERNET-MAC registers
 ** \param EMAC_DMA_IT interrupt vector number
 ** \param NewState function state
 **
 ******************************************************************************
 */
void EMAC_INT_config(FM3_ETHERNET_MAC_TypeDef * FM3_ETHERNET_MAC, uint32_t EMAC_DMA_IT, FuncStateT NewState)
{
    ASSERT((EMAC_DMA_INT_NIS==(EMAC_DMA_IT&EMAC_DMA_INT_NIS) )||
                    (EMAC_DMA_INT_NIS==(EMAC_DMA_IT&EMAC_DMA_INT_R))||
                    (EMAC_DMA_INT_NIS==(EMAC_DMA_IT& EMAC_DMA_INT_T )));
    ASSERT((NewState == DISABLE) || (NewState == ENABLE));

    if (NewState != DISABLE)
    {
        /* Enable the selected ETHERNET DMA interrupts */
        FM3_ETHERNET_MAC->IER |= EMAC_DMA_IT;
    }
    else
    {
        /* Disable the selected ETHERNET DMA interrupts */
        FM3_ETHERNET_MAC->IER &=(~(uint32_t)EMAC_DMA_IT);
    }
}

 /*!
 ******************************************************************************
 ** \brief Configures the selected MAC address.
 **
 ** \param FM3_ETHERNET_MAC point to ETHERNET-MAC registers
 ** \param MacAddr offset of MAC address register
 ** \param Addr MAC address settings ,6 bytes
 **
 ******************************************************************************
 */
void EMAC_MAC_Addr_config(FM3_ETHERNET_MAC_TypeDef * FM3_ETHERNET_MAC, uint32_t MacAddr, uint8_t *Addr)
{
    uint32_t value;

    ASSERT((MacAddr==EMAC_MAC_Address0)||
                   (MacAddr==EMAC_MAC_Address1)||
                   (MacAddr==EMAC_MAC_Address2)||
                   (MacAddr==EMAC_MAC_Address3));


    /* Calculate the selectecd MAC address high register */
    value = ((uint32_t)Addr[5] << 8) | (uint32_t)Addr[4];
    /* Load the selectecd MAC address high register */
    (*(__IO uint32_t *) (EMAC_MAC_ADDR_HBASE + MacAddr)) = value;
    /* Calculate the selectecd MAC address low register */
    value = ((uint32_t)Addr[3] << 24) | ((uint32_t)Addr[2] << 16) | ((uint32_t)Addr[1] << 8) | Addr[0];

    /* Load the selectecd MAC address low register */
    (*(__IO uint32_t *) (EMAC_MAC_ADDR_LBASE + MacAddr)) = value;
}

 /*!
 ******************************************************************************
 ** \brief Enables or disables the MAC transmission.
 **
 ** \param FM3_ETHERNET_MAC point to ETHERNET-MAC registers
 ** \param NewState function state
 **
 ******************************************************************************
 */
void EMAC_MACTransmissionCmd(FM3_ETHERNET_MAC_TypeDef * FM3_ETHERNET_MAC, FuncStateT NewState)
{

    ASSERT((NewState == DISABLE) || (NewState == ENABLE));
    if (NewState != DISABLE)
    {
        /* Enable the MAC transmission */
        FM3_ETHERNET_MAC->MCR |= EMAC_MACCR_TE;
    }
    else
    {
        /* Disable the MAC transmission */
        FM3_ETHERNET_MAC->MCR &= ~EMAC_MACCR_TE;
    }
}

 /*!
 ******************************************************************************
 ** \brief Clears the ETHERNET transmit FIFO..
 **
 ** \param FM3_ETHERNET_MAC point to ETHERNET-MAC registers
 **
 ******************************************************************************
 */
void EMAC_FlushTransmitFIFO(FM3_ETHERNET_MAC_TypeDef * FM3_ETHERNET_MAC)
{
     /* Set the Flush Transmit FIFO bit */
    FM3_ETHERNET_MAC->OMR |= EMAC_DMAOMR_FTF;
}

 /*!
 ******************************************************************************
 ** \brief Enables or disables the MAC reception
 **
 ** \param FM3_ETHERNET_MAC point to ETHERNET-MAC registers
  ** \param NewState function state
 **
 ******************************************************************************
 */
void EMAC_MACReceptionCmd(FM3_ETHERNET_MAC_TypeDef * FM3_ETHERNET_MAC, FuncStateT NewState)
{
    ASSERT((NewState == DISABLE) || (NewState == ENABLE));

    if (NewState != DISABLE)
    {
        /* Enable the MAC reception */
        FM3_ETHERNET_MAC->MCR |= EMAC_MACCR_RE;
    }
    else
    {
        /* Disable the MAC reception */
        FM3_ETHERNET_MAC->MCR &= ~EMAC_MACCR_RE;
    }
}

 /*!
 ******************************************************************************
 ** \brief Enables or disables the DMA transmission.
 **
 ** \param FM3_ETHERNET_MAC point to ETHERNET-MAC registers
 ** \param NewState function state
 **
 ******************************************************************************
 */
void EMAC_DMATransmissionCmd(FM3_ETHERNET_MAC_TypeDef * FM3_ETHERNET_MAC, FuncStateT NewState)
{
    ASSERT((NewState == DISABLE) || (NewState == ENABLE));

    if (NewState != DISABLE)
    {
        /* Enable the DMA transmission */
        FM3_ETHERNET_MAC->OMR |= EMAC_DMAOMR_ST;
    }
    else
    {
        /* Disable the DMA transmission */
        FM3_ETHERNET_MAC->OMR &= ~EMAC_DMAOMR_ST;
    }
}

 /*!
 ******************************************************************************
 ** \brief Enables or disables the DMA reception.
 **
 ** \param FM3_ETHERNET_MAC point to ETHERNET-MAC registers
 ** \param NewState function state
 **
 ******************************************************************************
 */
void EMAC_DMAReceptionCmd(FM3_ETHERNET_MAC_TypeDef * FM3_ETHERNET_MAC, FuncStateT NewState)
{
    ASSERT((NewState == DISABLE) || (NewState == ENABLE));

    if (NewState != DISABLE)
    {
        /* Enable the DMA reception */
        FM3_ETHERNET_MAC->OMR |= EMAC_DMAOMR_SR;
    }
    else
    {
        /* Disable the DMA reception */
        FM3_ETHERNET_MAC->OMR &= ~EMAC_DMAOMR_SR;
    }
}

 /*!
 ******************************************************************************
 ** \brief  Enables ENET MAC and DMA reception/transmission.
 **
 ** \param FM3_ETHERNET_MAC point to ETHERNET-MAC registers
 **
 ******************************************************************************
 */
void EMAC_start(FM3_ETHERNET_MAC_TypeDef * FM3_ETHERNET_MAC)
{
    /* Enable transmit state machine of the MAC for transmission on the MII */
    EMAC_MACTransmissionCmd(FM3_ETHERNET_MAC, ENABLE);
    /* Flush Transmit FIFO */
    EMAC_FlushTransmitFIFO(FM3_ETHERNET_MAC);
    /* Enable receive state machine of the MAC for reception from the MII */
    EMAC_MACReceptionCmd(FM3_ETHERNET_MAC, ENABLE);

    /* Start DMA transmission */
    EMAC_DMATransmissionCmd(FM3_ETHERNET_MAC, ENABLE);
    /* Start DMA reception */
    EMAC_DMAReceptionCmd(FM3_ETHERNET_MAC, ENABLE);
}

/*!
******************************************************************************
** \brief  Clears the ETHERNET's DMA interrupt pending bit.
**
** \param FM3_ETHERNET_MAC point to ETHERNET-MAC registers
** \param pending interrupt of pending
**
******************************************************************************
*/
void EMAC_clear_pending(FM3_ETHERNET_MAC_TypeDef * FM3_ETHERNET_MAC, uint32_t pending)
{
    ASSERT((pending==EMAC_DMA_INT_T)||(pending==EMAC_DMA_INT_R));

    /* Clear the selected ETHERNET DMA IT */
    FM3_ETHERNET_MAC->SR = (uint32_t) pending;
}

/*!
******************************************************************************
** \brief  Resumes the DMA Transmission by writing to the DmaRxPollDemand register
**   (the data written could be anything). This forces the DMA to resume reception.
**
** \param FM3_ETHERNET_MAC point to ETHERNET-MAC registers
**
******************************************************************************
*/

void EMAC_resume_reception(FM3_ETHERNET_MAC_TypeDef * FM3_ETHERNET_MAC)
{
    FM3_ETHERNET_MAC->RPDR = 0;
}


/*!
******************************************************************************
** \brief  Resumes the DMA Transmission by writing to the DmaTxPollDemand register
  *   (the data written could be anything). This forces  the DMA to resume transmission.
**
** \param FM3_ETHERNET_MAC point to ETHERNET-MAC registers
**
******************************************************************************
*/
void EMAC_resume_transmission(FM3_ETHERNET_MAC_TypeDef * FM3_ETHERNET_MAC)
{
    FM3_ETHERNET_MAC->TPDR = 0;
}


/*****************************************************************************/
/* END OF FILE */
