/*!
 ******************************************************************************
 **
 ** \file usb_dma_fm3.c
 **
 ** \brief This file provides DMA operation functions when it is enabled 
 **        in the USB transfer
 **
 ** \author FSLA AE Team
 **
 ** \version V0.10
 **
 ** \date 2011-11-24
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
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "usb_dma_fm3.h"

/*---------------------------------------------------------------------------*/
/* local defines                                                             */
/*---------------------------------------------------------------------------*/
typedef struct
{
  uint32_t        DMACA;
  uint32_t        DMACB;
  uint16_t *      DMACSA;
  uint16_t *      DMACDA;
}DMA_RegInfoT ;

/*---------------------------------------------------------------------------*/
/* local data                                                                */
/*---------------------------------------------------------------------------*/
#if USB_DMA
const static uint8_t g_USBDmaCh[6]=
{
  0,            /* EP 0 does not have DMA channel*/
  EP1_DMA,
  EP2_DMA,
  EP3_DMA,
  EP4_DMA,
  EP5_DMA,
};

/*---------------------------------------------------------------------------*/
/* local functions prototypes                                                */
/*---------------------------------------------------------------------------*/
static void DMA_INT(USB_EpIndexT Ep);


/*!
 ******************************************************************************
 ** \brief USB DMA interrupt initialization
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */ 
void USB_DMAIntInit(void)
{
    /*DMA enable*/
    NVIC_EnableIRQ(DMAC2_IRQn);
    NVIC_SetPriority(DMAC2_IRQn, USB_INTR_PRIORITY);
    NVIC_EnableIRQ(DMAC3_IRQn);
    NVIC_SetPriority(DMAC3_IRQn, USB_INTR_PRIORITY);
}

/*!
 ******************************************************************************
 ** \brief Disable DMA Transfers for EP1~EP5
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
void USB_DMAReset(void)
{
    DMA_RegInfoT * DMA_CH = (DMA_RegInfoT *)&(FM3_DMAC->DMACA0); 

    for(volatile uint32_t k = 1; k < 6 ; k++)
    {
        DMA_CH[g_USBDmaCh[k]].DMACA &= ~(1<<31);
    }
}

/*!
 ******************************************************************************
 ** \brief USB DMA interrup routine
 **
 ** \param Ep Endpoint index
 **
 ** \return None
 **
 ******************************************************************************
 */
static void DMA_INT(USB_EpIndexT Ep)
{
    volatile uint16_t * pEPStatus = USB_GET_EPxS_ADDR(Ep);
    volatile uint16_t * pEPControl = USB_GET_EPxC_ADDR(Ep);

    EntrCritSection();

    DMA_RegInfoT * DMA_CH = (DMA_RegInfoT *)&FM3_DMAC->DMACA0 + g_EpCnfg[Ep].DmaCh; 

    if(g_EpCnfg[Ep].Size & 0x01)
    {   /**/
        *((uint8_t *)DMA_CH->DMACDA) = *((uint8_t *)DMA_CH->DMACSA);
        * pEPStatus &= ~(1u<<10);
    }

    /*Clear DMAE*/
    * pEPControl &= ~(1u<<11);
    * pEPStatus &= ~(1u<<14);

    DMA_CH->DMACB = 0; /*clear stats, disable interrupts*/

    g_EpCnfg[Ep].Status = COMPLETE;
    //call callback function
    if(g_EpCnfg[Ep].pFn)
    {
        ((void(*)(USB_EpIndexT))g_EpCnfg[Ep].pFn)(Ep);
    }
    ExtCritSection();

}

/*!
 ******************************************************************************
 ** \brief DMA Channel 2 interrupt Handler
 **
 ** \param Ep Endpoint index
 **
 ** \return None
 **
 ******************************************************************************
 */
void DMAC2_Handler( void )
{
    for(USB_EpIndexT k = ENP1_OUT; ENP_MAX_NUMB > k; k++)
    { 
        /*Find which EP uses DMA channel 2*/
        if(2 == g_EpCnfg[k].DmaCh)
        {
            DMA_INT(k);
            break;
        }
    }
}

/*!
 ******************************************************************************
 ** \brief DMA Channel 3 interrupt Handler
 **
 ** \param Ep Endpoint index
 **
 ** \return None
 **
 ******************************************************************************
 */
void DMAC3_Handler( void )
{
    for(USB_EpIndexT k = ENP1_OUT; ENP_MAX_NUMB > k; k++)
    { 
        /*Find which EP uses DMA channel 3*/
        if(3 == g_EpCnfg[k].DmaCh)
        {
            DMA_INT(k);
            break;
        }
    }
}

/*!
 ******************************************************************************
 ** \brief Initialize and start DMA transfer
 **
 ** \param Ep Endpoint index
 **
 ** \return None
 **
 ******************************************************************************
 */
void DMA_TrsfStart(USB_EpIndexT Ep)
{
    volatile uint16_t * pEPControl = USB_GET_EPxC_ADDR(Ep);
    DMA_RegInfoT * pDmaCh = (DMA_RegInfoT *)&FM3_DMAC->DMACA0 + g_EpCnfg[Ep].DmaCh; 
    if(Ep > CTRL_ENP_IN)
    {
        if((Ep&0x1) &&  (g_EpCnfg[Ep].Size & 0x1))
        {
            FM3_INTREQ->DRQSEL &= ~(1u<<((Ep>>1)-1));
        }
        else
        {
            pDmaCh->DMACA = (0u<<31) |   /*Channel 0 disable*/
                            (0u<<30) |  /*Clear Pause bit*/
                            (0u<<29) |  /*Clear Software trigger*/
                            ((0x20  | ((Ep>>1)-1))<<23)|  /*EP interrupt signals*/
                            (0u<<16) |                     /*BC 1*/
                            ((g_EpCnfg[Ep].Size>>1)-1) ;    /*TC Size */

            pDmaCh->DMACB = (2u<<28) |  /*Demand transfer mode*/
                            (1u<<26) |  /*16-Bit transfer*/
                            (0u<<25) |  /*Increments the transfer source address*/
                            (0u<<24) |  /*Increments the transfer destination address.*/
                            (0u<<23) |  /*Disables the reload function.*/
                            (0u<<22) |  /*Disables the reload function of the transfer source address.*/
                            (0u<<21) |  /*Disables the reload function of the transfer destination address.*/
                            (0u<<20) |  /*Disables error interrup.*/
                            (0u<<19) |  /*Disables interrupt.*/
                            (0u<<16) |  /*status clear*/
                            (0u<<0) ;   /*Clears EB upon completion of the transfer.*/

            FM3_INTREQ->DRQSEL |= (1u<<((Ep>>1)-1));

            if(Ep & 1)
            {   /*IN EP*/
                pDmaCh->DMACB |= (1u<<24) ;  /*Fix the transfer destination address*/
                pDmaCh->DMACSA = (uint16_t *)g_EpCnfg[Ep].pBuffer;
                pDmaCh->DMACDA = (uint16_t *)&FM3_USB0->EP0DTL + (Ep & ~(0x1));
            }
            else
            {   /*Out EP*/
                pDmaCh->DMACB |= (1u<<25) ;  /*Fix the transfer source address*/
                pDmaCh->DMACDA = (uint16_t *)g_EpCnfg[Ep].pBuffer;
                pDmaCh->DMACSA = (uint16_t *)&FM3_USB0->EP0DTL + (Ep & ~(0x1));
            }

            pDmaCh->DMACB |=  (1u<<19); //Interrupts
            pDmaCh->DMACA |=  (1u<<31); //Enable end point;
            /*Set DMAE*/
            * pEPControl |= (1u<<11);
        }
    }
}

/*!
 ******************************************************************************
 ** \brief Get DMA channel index
 **
 ** \param Ep Endpoint index
 **
 ** \return DMA channel index
 **
 ******************************************************************************
 */
uint8_t DMA_GetChIndex(USB_EpIndexT Ep)
{
    return g_USBDmaCh[(Ep>>1)];
}


#endif /* USB_DMA*/

/*****************************************************************************/
/* END OF FILE */


