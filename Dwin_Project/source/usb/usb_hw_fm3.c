/*!
 ******************************************************************************
 **
 ** \file usb_hw_fm3.c
 **
 ** \brief USB function hardware driver
 **
 ** \author FSLA AE Team
 **
 ** \version V0.10
 **
 ** \date 2011-12-18
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

#define USB_HW_GLOBAL

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "usb_hw_fm3.h"

/*---------------------------------------------------------------------------*/
/* local defines                                                             */
/*---------------------------------------------------------------------------*/
/*! \brief Disable USB operation */
#define  USB_DISABLE_USB_OPERATION()     (bFM3_USBCLK_USBEN_USBEN = 0)
/*! \brief Enable USB operation */
#define  USB_ENABLE_USB_OPERATION()      (bFM3_USBCLK_USBEN_USBEN = 1)
/*! \brief Connect USB device */
#define  USB_DEV_SET_CONNECT()           (bFM3_USB0_UDCC_HCONX = 0)
/*! \brief Disconnect USB device */
#define  USB_DEV_SET_DISCONNECT()           (bFM3_USB0_UDCC_HCONX = 1)
/*! \brief Set USB Function mode */
#define  USB_SET_USB_FUNCTION_MODE()     (bFM3_USB0_HCNT0_HOST = 0)
/*! \brief Reset USB engine */
#define  USB_SET_RESET()            (bFM3_USB0_UDCC_RST = 1)
/*! \brief Release USB Reset */
#define  USB_RELEASE_RESET()        { bFM3_USB0_UDCC_RST = 0;  \
                                       while(bFM3_USB0_UDCC_RST); }
/*! \brief Enable USB stall auto clear */
#define  USB_STALL_AUTO_CLR_ENABLE()  (bFM3_USB0_UDCC_STALCLREN = 1)
/*! \brief Enable USB status register */
#define  USB_CLR_USB_STATUS()       (FM3_USB0->UDCS = 0x00)
/*! \brief Read USB status register */
#define  USB_GET_USB_STATUS()       (FM3_USB0->UDCS)

/*---------------------------------------------------------------------------*/
/* local data                                                                */
/*---------------------------------------------------------------------------*/

/*! \brief USB Standard Endpoint 0(OUT) Descriptor */
static const USB_StandardEpDescriptorT g_usbCtrlEpDescr0 =
{
    sizeof(USB_StandardEpDescriptorT),
    UsbDescriptorEp,
    USB_EpOut(CTRL_ENP_OUT>>1),
    {(uint8_t)UsbEpTransferControl | (uint8_t)UsbEpSynchNoSynchronization | (uint8_t)UsbEpUsageData},
    Ep0MaxSize,
    0
};

/*! \brief USB Endpoint External Data */
static const UsbEP_ExtDataT g_usbCtrlEpExtDescr0 =
{
    0
};

/*! \brief USB Standard Endpoint 0(IN) Descriptor */
static const USB_StandardEpDescriptorT g_usbCtrlEpDescr1 =
{
    sizeof(USB_StandardEpDescriptorT),
    UsbDescriptorEp,
    USB_EpIn(CTRL_ENP_IN>>1),
    {(uint8_t)UsbEpTransferControl | (uint8_t)UsbEpSynchNoSynchronization | (uint8_t)UsbEpUsageData},
    Ep0MaxSize,
    0
};

/*! \brief USB Endpoint External Descriptor */
static const UsbEP_ExtDataT g_usbCtrlEpExtDescr1 =
{
    0
};


#if  USB_SOF_EVENT > 0
static uint32_t  USB_SofNumbHold;
#endif

/*---------------------------------------------------------------------------*/
/* local functions prototypes                                                */
/*---------------------------------------------------------------------------*/
static void USB_SuspendClr(void);
static void USB_EpTransfer(USB_EpIndexT Ep);
static void USB_IntInit(void);
static void USB_ClkInit(void);
static void USB_IOInit(void);
static void USB_SelfPowerCnfg(void);
static void USB_InitTrsfBuf(void);
static void USB_ReleaseTrsfBuf(void);
static void USB_ClrTrsfIntFlag(void);
static void USB_InitEp(void);
static void USB_SuspendClr(void);
static void USB_ConnectEvent(void);
static void USB_DisConnectEvent(void);
static void USB_BusResetEvent(void);
static void USB_SetConfigureEvent(void);
static void USB_SuspendEvent(void);
static void USB_WakeupEvent(void);
static void USB_Ep0SetupEvent(void);
static void USB_Ep0OutEvent(void);
static void USB_Ep0InEvent(void);

/*!
 ******************************************************************************
 ** \brief USB Interrupt initialization
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */ 
static void USB_IntInit(void)
{
    /*Configure VBUS_INT*/
    FM3_GPIO->EPFR06 |= (uint32_t)(2ul<<30);
    /*Disable interrupt*/
    FM3_EXTI->ENIR &= ~(1ul << 15);
    /*High level detection EINT15*/
    FM3_EXTI->ELVR &= ~(3ul << 30);
    FM3_EXTI->ELVR |= (1ul<<30);
    /*Clear pending interrupt*/
    FM3_EXTI->EICL &= ~(1ul << 15);
    /*Enable EINT15 interrupt*/
    FM3_EXTI->ENIR |= (1ul << 15);

    /*USB Interrupts*/
    NVIC_EnableIRQ(USBF_IRQn);
    NVIC_SetPriority(USBF_IRQn, USB_INTR_PRIORITY);
    NVIC_EnableIRQ(USBF_USBH_IRQn);
    NVIC_SetPriority(USBF_USBH_IRQn, USB_INTR_PRIORITY);
    NVIC_EnableIRQ(EXINT8_15_IRQn);
}

/*!
 ******************************************************************************
 ** \brief USB Clock initialization
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */ 
static void USB_ClkInit(void)
{
    /*Disable USB*/
    FM3_USBCLK->UCCR = 0x00;
    /*USB PLL disable*/
    bFM3_USBCLK_UPCR1_UPLLEN = 0;
    /*Set USB PLL stabilization 
    wait time to 512uS*/
    FM3_USBCLK->UPCR2 &= ~7u;
    FM3_USBCLK->UPCR2 |= 2u;
    /*Set USB PLL to 96MHz*/
    FM3_USBCLK->UPCR3 = 0;    /*K = 1*/
    FM3_USBCLK->UPCR4 = 23;   /*N = 24*/
    /*Enable PLL*/
    bFM3_USBCLK_UPCR1_UPLLEN = 1;
    
    /*Wait PLL lock*/
    while(!bFM3_USBCLK_UP_STR_UPRDY);
    
    /*Selsect PLL*/
    bFM3_USBCLK_UCCR_UCSEL = 1;
    /*Enable Clock*/
    bFM3_USBCLK_UCCR_UCEN = 1;
}

/*!
 ******************************************************************************
 ** \brief USB IO port initialization
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
static void USB_IOInit(void)
{
    /*Configure UDM0 and UDP0*/
    FM3_GPIO->SPSR |= 0x10; 
    FM3_GPIO->PFR8 |= 0x00000003; /*Peripheral functions select*/
    /*Configure UHCONX*/
    FM3_GPIO->EPFR00 |= 0x00000200;
    FM3_GPIO->PFR6 |= 0x00000002; /*Peripheral functions select*/
}

/*!
 ******************************************************************************
 ** \brief USB power supply configuration
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
static void USB_SelfPowerCnfg(void)
{
#if (USB_SELF_POWERED)
    bFM3_USB0_UDCC_PWC = 1;
#else
    bFM3_USB0_UDCC_PWC = 0;
#endif
}


/*!
 ******************************************************************************
 ** \brief Initialize the USB clock, Endpoint, Interrupt
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
void USB_HwInit(void)
{
    volatile unsigned long dummy;
    // Init SOF number hold
#if  USB_SOF_EVENT > 0
    USB_SofNumbHold = 0;
#endif
    
    USB_DISABLE_USB_OPERATION();
    USB_ClkInit();
    /* wait over 5clock */
    dummy = FM3_USBCLK->UCCR;
    dummy = FM3_USBCLK->UCCR;
    dummy = FM3_USBCLK->UCCR;
    dummy = FM3_USBCLK->UCCR;
    dummy = FM3_USBCLK->UCCR;
    
    /*Enable USB*/
    USB_ENABLE_USB_OPERATION();
    /*USB Pins configure*/
    USB_IOInit();
    /*USB*/
    /*Select funtion mode*/ 
    USB_SET_USB_FUNCTION_MODE();
    /*Disconect*/
    USB_DEV_SET_DISCONNECT();
    /* Configure self power  */
    USB_SelfPowerCnfg();
    /**/
    USB_HwReset();
    /*NVIC intitialization*/
#if USB_DMA
    USB_DMAIntInit();
#endif  /*USB_DMA*/
    USB_IntInit();
    // Init Device status
    USB_SetDevState(UsbDevStatusUnknow);    
}

/*!
 ******************************************************************************
 ** \brief Initialize USB transfer buffer
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
static void USB_InitTrsfBuf(void)
{
    /*Init EP0 Buffers*/
    bFM3_USB0_EP0OS_BFINI = 1;
    bFM3_USB0_EP0IS_BFINI = 1;
    /*Init EP1 Buffers*/
    bFM3_USB0_EP1S_BFINI = 1;
    /*Init EP2 Buffers*/
    bFM3_USB0_EP2S_BFINI = 1;
    /*Init EP3 Buffers*/
    bFM3_USB0_EP3S_BFINI = 1;
    /*Init EP4 Buffers*/
    bFM3_USB0_EP4S_BFINI = 1;
    /*Init EP5 Buffers*/
    bFM3_USB0_EP5S_BFINI = 1;
}

/*!
 ******************************************************************************
 ** \brief Release USB transfer buffer
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
static void USB_ReleaseTrsfBuf(void)
{
    /*Init EP0 Buffers*/
    bFM3_USB0_EP0OS_BFINI = 0;
    bFM3_USB0_EP0IS_BFINI = 0;
    /*Init EP1 Buffers*/
    bFM3_USB0_EP1S_BFINI = 0;
    /*Init EP2 Buffers*/
    bFM3_USB0_EP2S_BFINI = 0;
    /*Init EP3 Buffers*/
    bFM3_USB0_EP3S_BFINI = 0;
    /*Init EP4 Buffers*/
    bFM3_USB0_EP4S_BFINI = 0;
    /*Init EP5 Buffers*/
    bFM3_USB0_EP5S_BFINI = 0;
}

/*!
 ******************************************************************************
 ** \brief Clear USB interrupt flag
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
static void USB_ClrTrsfIntFlag(void)
{
    bFM3_USB0_EP0IS_DRQIIE = 0;
    bFM3_USB0_EP0OS_DRQOIE = 0;
    bFM3_USB0_EP1S_DRQIE = 0;
    bFM3_USB0_EP2S_DRQIE = 0;
    bFM3_USB0_EP3S_DRQIE = 0;
    bFM3_USB0_EP4S_DRQIE = 0;
    bFM3_USB0_EP5S_DRQIE = 0;
}

/*!
 ******************************************************************************
 ** \brief Initial USB Endpoint
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
static void USB_InitEp(void)
{   
    /*Init EP0 max data size*/
    FM3_USB0->EP0C |= Ep0MaxSize;

    /*Init EP1 Type, Size, Direction, Endpoint enable*/
    FM3_USB0->EP1C = (1u<<15)                |    /*Enable*/
                     (ENP1_MAX_SIZE)         |    /*Size*/
                     ((ENP1_TYPE & 0x3)<<13) |    /*Type*/
                     (ENP1_DIR<<12) ;             /*Dir*/

    /*Init EP2 Type, Size, Direction*/
    FM3_USB0->EP2C = (1u<<15)                |    /*Enable*/
                     (ENP2_MAX_SIZE)         |    /*Size*/
                     ((ENP2_TYPE & 0x3)<<13) |    /*Type*/
                     (ENP2_DIR<<12) ;             /*Dir*/

    /*Init EP3 Type, Size, Direction*/
    FM3_USB0->EP3C = (1u<<15)                |    /*Enable*/
                     (ENP3_MAX_SIZE)         |    /*Size*/
                     ((ENP3_TYPE & 0x3)<<13) |    /*Type*/
                     (ENP3_DIR<<12) ;             /*Dir*/

    /*Init EP4 Type, Size, Direction*/
    FM3_USB0->EP4C = (1u<<15) |                   /*Enable*/
                     (ENP4_MAX_SIZE) |            /*Size*/
                     ((ENP4_TYPE & 0x3)<<13) |    /*Type*/
                     (ENP4_DIR<<12) ;             /*Dir*/

    /*Init EP5 Type, Size, Direction*/
    FM3_USB0->EP5C = (1u<<15) |                   /*Enable*/
                     (ENP5_MAX_SIZE) |            /*Size*/
                     ((ENP5_TYPE & 0x3)<<13) |    /*Type*/
                     (ENP5_DIR<<12) ;             /*Dir*/
}

/*!
 ******************************************************************************
 ** \brief Reset USB engine
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
void USB_HwReset(void)
{
    volatile uint32_t dummy;
#if USB_DMA
    USB_DMAReset();
#endif /* USB_DMA*/
    USB_SuspendClr();
    /*Reset USB Core*/
    USB_SET_RESET();
    /* Initialize transfer buffer */
    USB_InitTrsfBuf();
    /* Confiure Endpoint */
    USB_InitEp();
    /* Auto clear stall */ 
    USB_STALL_AUTO_CLR_ENABLE();
    dummy = FM3_USB0->UDCC;
    /*Release USB Reset*/
    USB_RELEASE_RESET();
    /*Release EP buffers*/
    USB_ReleaseTrsfBuf();  
    /*Disable interrupts 
    and clear flags*/
    USB_ClrTrsfIntFlag();  
    USB_CLR_USB_STATUS();
    dummy = USB_GET_USB_STATUS();
}

/*!
 ******************************************************************************
 ** \brief Enable or disable an Endpoint
 **
 ** \param pEPDesc Point to Standard Endpoint Descriptor 
 ** \param pUsbEPExtData Point to USB Endpoint extent data
 ** \param Enable Indicate whether to enable or disable an Endpoint
 **
 ** \return USB Error Codes
 **
 ******************************************************************************
 */
USB_ErrorCodesT USB_RealizeEp(const USB_StandardEpDescriptorT * pEPDesc,
                               const UsbEP_ExtDataT * pUsbEPExtData,
                               uint8_t Enable)
{
    USB_EpIndexT Ep = (USB_EpIndexT)USB_EpLogToPhysAdd(pEPDesc->bEndpointAddress);

    if (Enable)
    {
        // Set EP status
        g_EpCnfg[Ep].Status  = NOT_READY;
        // Init EP flags
        g_EpCnfg[Ep].Flags = 0;
        g_EpCnfg[Ep].bDMA_Transfer  = pUsbEPExtData->DMA_Transfer;
        g_EpCnfg[Ep].bDoubleBuffered = 0;
        // Set endpoint type
        g_EpCnfg[Ep].EpType = (USB_EpTransferTypeT)pEPDesc->bmAttributes.TransferType;
        // Init EP max packet size
        g_EpCnfg[Ep].MaxSize = pEPDesc->wMaxPacketSize;
        if(0 == Ep)
        {
            bFM3_USB0_EP0OS_DRQOIE = 1;
        }
#if USB_DMA    
        g_EpCnfg[Ep].DmaCh = DMA_GetChIndex(Ep);
#endif /* USB_DMA*/
    }
    else
    {
        g_EpCnfg[Ep].MaxSize = 0;
        if(0 == Ep)
        {
            bFM3_USB0_EP0OS_DRQOIE = 0;
        }
    }
    return(USB_OK);
}

/*!
 ******************************************************************************
 ** \brief Clear USB suspend flag
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
static void USB_SuspendClr(void)
{
    volatile int dummy;
    /*Wait PLL lock*/
    while(!bFM3_USBCLK_UP_STR_UPRDY);
    /*Clear Suspend Bit*/
    bFM3_USB0_UDCS_SUSP = 0;
    dummy = FM3_USB0->UDCS;
    bFM3_USB0_UDCC_USTP = 0;
    dummy = FM3_USB0->UDCC;
}
#if USB_REMOTE_WAKEUP != 0
/*!
 ******************************************************************************
 ** \brief Wake up USB
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
void USB_HwWakeUp (void)
{
    USB_SuspendClr();
    bFM3_USB0_UDCC_RESUM = 1;
    bFM3_USB0_UDCC_RESUM = 0;
    bFM3_USB0_UDCIE_SUSPIE = 1;
}
#endif // USB_REMOTE_WAKEUP != 0

 /*!
 ******************************************************************************
 ** \brief Stall/Unstall USB Endpoint
 **
 ** \param Ep Endpoint index
 ** \param Stall Indicate whether to stall or unstall USB Endpoint
 **
 ** \return None
 **
 ******************************************************************************
 */
void USB_SetStallEP (USB_EpIndexT Ep, uint8_t Stall)
{
  
    volatile uint16_t * pEPControl =   USB_GET_EPxC_ADDR(Ep);

    if(Stall)
    {
        *pEPControl |= (1u<<9);
        USB_EpUnstallHook(Ep);
    }
    /*else
    {
        *pEPControl &= ~(1u<<9);
    }*/
}

/*!
 ******************************************************************************
 ** \brief USB Endpoint Stall contrl
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
void USB_StallCtrlEP (void)
{
    g_EpCnfg[CTRL_ENP_OUT].AvbBuff = 0;
    g_EpCnfg[CTRL_ENP_IN].AvbBuff  = g_EpCnfg[CTRL_ENP_IN].bDoubleBuffered + 1;
    USB_SetStallEP(CTRL_ENP_OUT,TRUE);
}

 /*!
 ******************************************************************************
 ** \brief Get USB Endpoint Stall status
 **
 ** \param Ep Endpoint Index
 ** \param pStall stall status
 **
 ** \return None
 **
 ******************************************************************************
 */
void USB_GetStallEP (USB_EpIndexT Ep, uint8_t* pStall)
{
    volatile uint16_t * pEPControl =   USB_GET_EPxC_ADDR(Ep);

    *pStall = FALSE;
    if((*pEPControl) & (1u<<9))
    {
        *pStall = TRUE;
    }
}


/*!
 ******************************************************************************
 ** \brief Convert the logical to physical address
 **
 ** \param EpLogAdd Endpoint logical address
 **
 ** \return Endpoint index
 **
 ******************************************************************************
 */
USB_EpIndexT USB_EpLogToPhysAdd (uint8_t EpLogAdd)
{
    return (USB_EpIndexT)(((EpLogAdd & 0xF)<<1) + ((EpLogAdd&0x80)?1:0));
}

#if USB_SOF_EVENT > 0
/*!
 ******************************************************************************
 ** \brief Get current value of SOF number
 **
 ** \param None
 **
 ** \return current value of SOF number
 **
 ******************************************************************************
 */
uint32_t USB_GetFrameNumb (void)
{
    return(USB_SofNumbHold);
}
#endif

/*!
 ******************************************************************************
 ** \brief Prepare status phase
 **
 ** \param In Indicate transfer direction
 **
 ** \return None
 **
 ******************************************************************************
 */
void USB_StatusPhase (uint8_t In)
{
    if(In)
    {
        USB_DataTransfer(CTRL_ENP_IN,NULL,0,NULL);
    }
}

/*!
 ******************************************************************************
 ** \brief Handle the USB connect event
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
static void USB_ConnectEvent(void)
{
    volatile uint32_t dummy;
    USB_DevConnectCallback(TRUE);
    /*Disable EINT15 interrupt*/
    FM3_EXTI->ENIR &= ~0x8000;
    /*Low level detection EINT15*/
    FM3_EXTI->ELVR &= ~0xC0000000;
    /*Enable EINT15 interrupt*/
    FM3_EXTI->ENIR |= 0x8000;
    // Control EP Init
    USB_RealizeEp(&g_usbCtrlEpDescr0,&g_usbCtrlEpExtDescr0,TRUE);
    USB_RealizeEp(&g_usbCtrlEpDescr1,&g_usbCtrlEpExtDescr1,TRUE);

    FM3_USB0->UDCS = 0x00;
    dummy = FM3_USB0->UDCS;
    /*Interrupt enable*/
    /* CONFIE=1,WKUPIE=1,BRSTIE=1,SUSPIE=1 */
    FM3_USB0->UDCIE = 0x2D;
    dummy = FM3_USB0->UDCIE;
    /*Conect*/
    //FM3_USB0->UDCC &= ~UDCC_HCONX;
    bFM3_USB0_UDCC_HCONX = 0;
}

/*!
 ******************************************************************************
 ** \brief Handle the USB disconnect event
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
static void USB_DisConnectEvent(void)
{
    USB_DevConnectCallback(FALSE);
    /*Disconect*/
    //FM3_USB0->UDCC |= UDCC_HCONX;
    bFM3_USB0_UDCC_HCONX = 1;
    /**/
    USB_HwReset();
    /*Disable EINT15 interrupt*/
    FM3_EXTI->ENIR &= ~0x8000;
    /*High level detection EINT15*/
    FM3_EXTI->ELVR &= ~0xC0000000;
    FM3_EXTI->ELVR |= 0x40000000;
    /*Enable EINT15 interrupt*/
    FM3_EXTI->ENIR |= 0x8000;
}

/*!
 ******************************************************************************
 ** \brief VBUS interrupt handler
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
void EXTI8_15_IRQHandler(void)
{
    volatile uint32_t dummy;
    volatile uint32_t k;
    EntrCritSection();
    /*Clear pending interrupt*/
    /*Filter*/
    for(k = 0; 1000 >k ; k++);
    dummy = FM3_EXTI->EIRR;

    if(FM3_EXTI->ELVR & 0x40000000)
    {   /*Interrupt by High Level*/
        if((FM3_GPIO->PDIR6 & 0x01))
        {/**/ 
            USB_ConnectEvent();
            #ifdef DEBUG_MODE    
                printf("USB connected!\n");
            #endif
        }
    }
    else
    {   /*Interrupt by Low Level*/
        if(!(FM3_GPIO->PDIR6 & 0x01))
        {
            USB_DisConnectEvent();
            #ifdef DEBUG_MODE
                printf("USB disconnected!\n");
            #endif 
        }
    }

    FM3_EXTI->EICL &= ~(0x8000);
    dummy = FM3_EXTI->EICL;
    ExtCritSection();
}

/*!
 ******************************************************************************
 ** \brief USB EP1~EP5 interrupt handler
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
void USB_1_IRQHandler( void )
{
    volatile uint16_t * pEPControl = &FM3_USB0->EP1C;
    volatile uint16_t * pEPStatus =  &FM3_USB0->EP1S;
    volatile int k;  
    EntrCritSection();

    for(k = 1; 5 >= k; k++)
    {/*Check end point 1 to 5*/
        if(((*pEPStatus) & (1u<<14)) && ((*pEPStatus) & (1u<<10)))
        {   /*If end point interrupt is enabled
            inerrupt flag is set*/
            uint8_t dir = ((*pEPControl)&(1u<<12))?1:0;
            USB_EpTransfer((USB_EpIndexT)((k<<1)+dir));
            break;
        }
        pEPControl += 2;
        pEPStatus += 2;
    }

    ExtCritSection();
}


/*!
 ******************************************************************************
 ** \brief USB bus reset event
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
static void USB_BusResetEvent(void)
{
    /*Flush EP buffers*/
    USB_InitTrsfBuf();
    /*Release EP buffers*/
    USB_ReleaseTrsfBuf();
    USB_DevResetCallback();
}

/*!
 ******************************************************************************
 ** \brief USB set configuration event
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
static void USB_SetConfigureEvent(void)
{
    /*emulate set config command*/
    USB_SetDevState(UsbDevStatusAddress);
    g_UsbEp0SetupPacket.mRequestType.Dir = UsbHost2Device;
    g_UsbEp0SetupPacket.mRequestType.Type = UsbTypeStandart;
    g_UsbEp0SetupPacket.wIndex.Word = 0;
    g_UsbEp0SetupPacket.wLength.Word = 0;
    g_UsbEp0SetupPacket.wValue.Lo = bFM3_USB0_UDCIE_CONFN;
    g_UsbEp0SetupPacket.bRequest  = SET_CONFIGURATION;
    USB_SetupHandler();
}

/*!
 ******************************************************************************
 ** \brief USB suspend event
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
static void USB_SuspendEvent(void)
{
    bFM3_USB0_UDCIE_SUSPIE = 0;  /*Disable suspend interrupt*/
    USB_DevSuspendCallback(TRUE);
    bFM3_USB0_UDCC_USTP = 1;     /*Suspend device*/
}

/*!
 ******************************************************************************
 ** \brief USB wakeup event
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
static void USB_WakeupEvent(void)
{
    USB_SuspendClr();                     /*Clear suspend status*/
    bFM3_USB0_UDCIE_SUSPIE = 1;  /*Enable suspend interrupt*/
    USB_DevSuspendCallback(FALSE);
}

/*!
 ******************************************************************************
 ** \brief USB Endpoint0 Setup event
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
static void USB_Ep0SetupEvent(void)
{
    bFM3_USB0_EP0IS_BFINI = 1;  /* BFINI=1 */
    bFM3_USB0_EP0IS_BFINI = 0;  /* BFINI=0 */
    g_EpCnfg[CTRL_ENP_IN].AvbBuff  = 1;
    /* init IO to receive Setup packet */
    USB_DataTransfer(CTRL_ENP_IN,NULL,(uint32_t)-1,NULL);
    USB_DataTransfer(CTRL_ENP_OUT,g_UsbEp0SetupPacket.Data,sizeof(USB_SetupPacketT),NULL);
    USB_EpTransfer(CTRL_ENP_OUT);
    /* reset EP IO ctrl */
    if (g_UsbEp0SetupPacket.mRequestType.Dir == UsbDevice2Host)
    {
        USB_StatusHandler(CTRL_ENP_OUT);
    }

    USB_SetupHandler();

    if(g_EpCnfg[CTRL_ENP_OUT].Status == STALLED)
    {
        USB_StallCtrlEP();
    }
}

/*!
 ******************************************************************************
 ** \brief USB Endpoint0 data out event
 **        Data: Host->Device
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
static void USB_Ep0OutEvent(void)
{
    USB_EpTransfer(CTRL_ENP_OUT);
    if(g_UsbEp0SetupPacket.mRequestType.Dir == UsbDevice2Host &&
     g_EpCnfg[CTRL_ENP_OUT].pFn)
    {
        ((void(*)(USB_EpIndexT))g_EpCnfg[CTRL_ENP_OUT].pFn)(CTRL_ENP_OUT);
    }
}

/*!
 ******************************************************************************
 ** \brief USB Endpoint0 data in event
 **        Data: Host<-Device
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
static void USB_Ep0InEvent(void)
{
    USB_EpTransfer(CTRL_ENP_IN);
    if(g_UsbEp0SetupPacket.mRequestType.Dir == UsbHost2Device &&
    g_EpCnfg[CTRL_ENP_IN].pFn)
    {
        ((void(*)(USB_EpIndexT))g_EpCnfg[CTRL_ENP_IN].pFn)(CTRL_ENP_IN);
    }
}


/*!
 ******************************************************************************
 ** \brief USB EP0 (IN and OUT ) and Status interrupt handler
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
void USB_2_IRQHandler( void )
{
    EntrCritSection();

    /* Status Interrupts */
    if(bFM3_USB0_UDCS_BRST) {  /* Bus-Reset detect */
        bFM3_USB0_UDCS_BRST = 0;
        USB_BusResetEvent();
    #ifdef DEBUG_MODE
        printf("Bus Reset\n");
    #endif
    }
    else if(bFM3_USB0_UDCS_CONF) { /* SET-CONFIG detect */
        USB_SetConfigureEvent();
        bFM3_USB0_UDCS_CONF = 0;    /* CONF clear */
    }
    else if(bFM3_USB0_UDCS_SUSP) { /*Suspend interrupt*/
        bFM3_USB0_UDCS_SUSP = 0;     /*Clear interrupt flag*/
        USB_SuspendEvent();
    #ifdef DEBUG_MODE
        printf("Suspend\n");
    #endif
    }
    else if(bFM3_USB0_UDCS_WKUP) { /*Wakup interrupt*/
        USB_WakeupEvent();
        bFM3_USB0_UDCS_WKUP = 0;
    #ifdef DEBUG_MODE
        printf("Wakeup\n");
    #endif
    }
    else if(bFM3_USB0_EP0OS_DRQO)  /* check DRQO */
    { 
        if(bFM3_USB0_UDCS_SETP)  /* SETUP detect */
        {  
            bFM3_USB0_UDCS_SETP = 0;
            USB_Ep0SetupEvent();
        }
        else
        {
            USB_Ep0OutEvent();
        }
    }
    else if(bFM3_USB0_EP0IS_DRQI)  /* check DRQI */
    { 
        USB_Ep0InEvent();
    }
    ExtCritSection();
}

/*!
 ******************************************************************************
 ** \brief Prepare and transfer data via Endpoint
 **
 ** \param Ep Endpoint index
 ** \param pBuffer Point to transfer data buffer
 ** \param Size Transfer data size
 ** \param pFn Point to callback function when data is transferred
 **
 ** \return USB Transfer status
 **
 ******************************************************************************
 */
USB_TrsfStatT USB_DataTransfer (USB_EpIndexT Ep, uint8_t* pBuffer, uint32_t Size, void * pFn)
{

    pEpCnfgT pEp = &g_EpCnfg[Ep];

    if (Size == (uint32_t)-1)
    {
        pEp->Status  = NOT_READY;
        pEp->pFn     = NULL;
    }
    else
    {

        ENTR_CRT_SECTION();

        if (!IS_USB_EP_VALID(pEp))
        {
            EXT_CRT_SECTION();

            return(NOT_VALID);
        }
        // lock buffer
        if(pEp->Status == BEGIN_SERVICED)
        {
            EXT_CRT_SECTION();
            return(NOT_READY);
        }
        pEp->Offset  = 0;
        pEp->pBuffer = pBuffer;
        pEp->pFn     = pFn;
		pEp->Size = Size;
        if(!pEp->Size)
        {
            pEp->bZeroPacket = 1;
        }
        pEp->Status  = NO_SERVICED;
        USB_EpTransfer(Ep);
        EXT_CRT_SECTION();
    }
    return(pEp->Status);
}


/*!
 ******************************************************************************
 ** \brief Write/Read Endpoint
 **
 ** \param Ep Endpoint index
 **
 ** \return None
 **
 ******************************************************************************
 */
static void USB_EpTransfer(USB_EpIndexT Ep)
{
    uint32_t Count,Size;
    volatile uint8_t * pEPBuffer;
    volatile uint16_t * pEPStatus;

    pEPStatus = USB_GET_EPxS_ADDR(Ep);
    pEPBuffer = USB_GET_EPxDT_ADDR(Ep);
    if(g_EpCnfg[Ep].Status == NO_SERVICED)
    { 
        g_EpCnfg[Ep].Status = BEGIN_SERVICED;
    #if USB_DMA
        DMA_TrsfStart(Ep);
    #endif /* USB_DMA*/
        /*Enable interrupt on endpoint*/
        * pEPStatus |= (1u<<14);
        return;
    }
    else
    {
        if(g_EpCnfg[Ep].Status == BEGIN_SERVICED)
        {
            if(Ep & 1)
            {   /*IN*/
                Size = g_EpCnfg[Ep].Size - g_EpCnfg[Ep].Offset;

                if(Size == 0 && !g_EpCnfg[Ep].bZeroPacket)
                {
                    g_EpCnfg[Ep].Status = COMPLETE;

                    * pEPStatus &= ~(1u<<14);
                    //call callback function
                    if(g_EpCnfg[Ep].pFn)
                    {
                    ((void(*)(USB_EpIndexT))g_EpCnfg[Ep].pFn)(Ep);
                    }
                    /*Disable interrup*/

                    return;
                }

                if(g_EpCnfg[Ep].bZeroPacketPossible && Size == g_EpCnfg[Ep].MaxSize)
                {
                    g_EpCnfg[Ep].bZeroPacketPossible = 0;
                    g_EpCnfg[Ep].bZeroPacket = 1;
                }

                Size = MIN(Size,g_EpCnfg[Ep].MaxSize);
                // Write data to SIE buffer
                if(Size)
                {
                    Count = 0;
                    do
                    {           
                        if(Count&(1u<<0))
                        {
                            *(pEPBuffer+1) = *(g_EpCnfg[Ep].pBuffer+g_EpCnfg[Ep].Offset++);
                        }
                        else
                        {
                            *(pEPBuffer) = *(g_EpCnfg[Ep].pBuffer+g_EpCnfg[Ep].Offset++);
                        }           
                        Count++;
                    }          
                    while(Count < Size);
                }
                else
                {
                    g_EpCnfg[Ep].bZeroPacket = 0;
                }

            }
            else
            {   /*OUT*/
                // Get data size
                Size = (*pEPStatus) & 0x1FF;
                if(Size > (g_EpCnfg[Ep].Size - g_EpCnfg[Ep].Offset))
                {
                    g_EpCnfg[Ep].Status = BUFFER_OVERRUN;
                    g_EpCnfg[Ep].Size = g_EpCnfg[Ep].Offset;
                }
                else
                {
                    if (Size < g_EpCnfg[Ep].MaxSize)
                    {
                        g_EpCnfg[Ep].Status = BUFFER_UNDERRUN;
                        g_EpCnfg[Ep].Size = g_EpCnfg[Ep].Offset + Size;
                    }

                    Count = 0;
                    // Read data from SIE buffer
                    while(Count<Size)
                    {
                        // because zero lenght packe is possible
                        if(Count&(1u<<0))
                        {
                            *(g_EpCnfg[Ep].pBuffer+g_EpCnfg[Ep].Offset++) = *(pEPBuffer+1);
                        }
                        else
                        {
                            *(g_EpCnfg[Ep].pBuffer+g_EpCnfg[Ep].Offset++) = *(pEPBuffer);
                        }           
                        Count++;
                    }
					Size = g_EpCnfg[Ep].Size - g_EpCnfg[Ep].Offset;
                    if (!Size)
                    {
                        g_EpCnfg[Ep].Status = COMPLETE;
                    }

                }
                if (g_EpCnfg[Ep].Status != BEGIN_SERVICED && g_EpCnfg[Ep].Status != NO_SERVICED)
                {
                    if(0 != Ep)
                    {
                        * pEPStatus &= ~(1u<<14);
                    }
                    //call callback function
                    if(g_EpCnfg[Ep].pFn)
                    {
                        ((void(*)(USB_EpIndexT))g_EpCnfg[Ep].pFn)(Ep);
                    }
                }

            }
        }
    }
    * pEPStatus &= ~(1u<<10);
}
/*****************************************************************************/
/* END OF FILE */
