/*!
 ******************************************************************************
 **
 ** \file usb_dma_fm3.h
 **
 ** \brief USB DMA transfer head file
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
#include "includes.h"

#ifndef USB_DMA_H
#define USB_DMA_H

/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/
#if USB_DMA
/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
void USB_DMAIntInit(void);
void USB_DMAReset(void);
void USB_DMA_Start(USB_EpIndexT EP);
void DMA_TrsfStart(USB_EpIndexT Ep);
uint8_t DMA_GetChIndex(USB_EpIndexT Ep);
#ifdef __cplusplus
}
#endif

#endif /* USB_DMA */

#endif /* USB_DMA_H */

/*****************************************************************************/
/* END OF FILE */
