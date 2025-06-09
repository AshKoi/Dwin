/*!
 ******************************************************************************
 **
 ** \file usb_hw_fm3.c
 **
 ** \brief USB function low level driver head file
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

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "includes.h"

#ifndef _USB_HW_H
#define _USB_HW_H

#ifdef USB_HW_GLOBAL
#define USB_HW_EXTERN
#else
#define USB_HW_EXTERN  extern
#endif

/*! \brief Get EPxC register address */
#define  USB_GET_EPxC_ADDR(Ep)   ((&FM3_USB0->EP0C) + ((Ep) & ~(1u<<0)))
/*! \brief Get EPxS register address */
#define  USB_GET_EPxS_ADDR(Ep)   ((Ep) == 1)? (&FM3_USB0->EP0IS): \
                                 ((&FM3_USB0->EP0OS) + ((Ep) & ~(1u<<0)))
/*! \brief Get EPxDT register address */
#define  USB_GET_EPxDT_ADDR(Ep)  (&FM3_USB0->EP0DTL + (((Ep) & ~(1<<0))<<1))

/*!
 ******************************************************************************
 ** \brief USB error code data enumeration
 ******************************************************************************
 */
typedef enum _USB_ErrorCodesT
{
  USB_OK = 0,USB_PLL_ERROR, USB_INTR_ERROR,
  USB_EP_OCCUPIER, USB_MEMORY_FULL, USB_BUF_OVERFLOW,
  USB_EP_NOT_VALID, UB_EP_SETUP_UNDERRUN, USB_EP_STALLED,
  UB_EP_SETUP_OVERWRITE, USB_EP_FATAL_ERROR, USB_DMA_DESC_OVERFLOW
} USB_ErrorCodesT;

/*!
 ******************************************************************************
 ** \brief USB Endpoint index data enumeration
 ******************************************************************************
 */
typedef enum _USB_EpIndexT
{
    CTRL_ENP_OUT=0, CTRL_ENP_IN,
    ENP1_OUT      , ENP1_IN    ,
    ENP2_OUT      , ENP2_IN    ,
    ENP3_OUT      , ENP3_IN    ,
    ENP4_OUT      , ENP4_IN    ,
    ENP5_OUT      , ENP5_IN    ,
    ENP_MAX_NUMB
} USB_EpIndexT;

/*!
 ******************************************************************************
 ** \brief USB transfer status data enumeration
 ******************************************************************************
 */
typedef enum _USB_TrsfStatusT
{
    NOT_READY = 0, NO_SERVICED, BEGIN_SERVICED, COMPLETE, BUFFER_UNDERRUN, BUFFER_OVERRUN,
    SETUP_OVERWRITE, STALLED, NOT_VALID
} USB_TrsfStatT;

/*!
 ******************************************************************************
 ** \brief USB Endpoint configuration data structure
 ******************************************************************************
 */
typedef struct _EpCnfgT
{
    uint32_t              MaxSize;       //!< Endpoint maximum size      
    USB_EpTransferTypeT   EpType;        //!< Endpoint transfer type  
    void *                pFn;           //!< Point to callback function 
    uint32_t              Offset;        //!< Transfer data offset 
    uint32_t              Size;          //!< Transfer data size 
    USB_TrsfStatT         Status;        //!< Transfer status 
    uint8_t*              pBuffer;       //!< Point to data buffer
    uint8_t               AvbBuff;       //!< Available buffer
#if USB_DMA
    uint8_t               DmaCh;         //!< DMA Channel index
#endif /* USB_DMA*/
    union
    {
        uint8_t Flags;
        struct
        {
          uint8_t bDoubleBuffered     : 1;  //!< Double buffer flag
          uint8_t bDMA_Transfer       : 1;  //!< DMA transfer flag
          uint8_t bZeroPacket         : 1;  //!< Zero packet flag
          uint8_t bZeroPacketPossible : 1;  //!< Zero packet possible flag
        };
    };
} EpCnfgT, *pEpCnfgT;

/*! \brief Check if Endpoint index is valid */
#define IS_USB_EP_VALID(pEP) (0 != pEP->MaxSize)

/*!
 ******************************************************************************
 ** \brief USB Endpoint extent data structure
 ******************************************************************************
 */
typedef struct _UsbEP_ExtDataT
{
    uint8_t DMA_Transfer;
} UsbEP_ExtDataT, *pUsbEP_ExtDataT;

/*! \brief Endpoint configuration array declare */
USB_HW_EXTERN EpCnfgT g_EpCnfg[ENP_MAX_NUMB];


/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
void USB_HwReset (void);
void USB_HwInit(void);
USB_ErrorCodesT USB_RealizeEp(const USB_StandardEpDescriptorT * pEPDesc,
                                   const UsbEP_ExtDataT * pUsbEPExtData,
                                   uint8_t Enable);
#if USB_REMOTE_WAKEUP != 0
void USB_HwWakeUp (void);
#endif // USB_REMOTE_WAKEUP != 0
void USB_SetStallEP (USB_EpIndexT Ep, uint8_t Stall);
void USB_StallCtrlEP (void);
void USB_GetStallEP (USB_EpIndexT Ep, uint8_t* pStall);
USB_EpIndexT USB_EpLogToPhysAdd (uint8_t EpLogAdd);
#if USB_SOF_EVENT > 0
uint32_t USB_GetFrameNumb (void);
#endif
void USB_StatusPhase (uint8_t In);
USB_TrsfStatT USB_DataTransfer (USB_EpIndexT EP, uint8_t* pBuffer, uint32_t Size, void * pFn);
#ifdef __cplusplus
}
#endif

#endif /*_USB_HW_H */
/*****************************************************************************/
/* END OF FILE */
