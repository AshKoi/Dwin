/*!
 ******************************************************************************
 **
 ** \file dmac_fm3.h
 **
 ** \brief DMA head file
 **
 ** \author FSLA AE Team
 **
 ** \version V0.10
 **
 ** \date 2011-11-21
 **
 ** \attention THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU SEMICONDUCTOR
 **            ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
 **            OMMISSIONS.
 **
 ** (C) Copyright 200x-201x by Fujitsu Semiconductor(Shanghai) Co.,Ltd.
 **
 ******************************************************************************
 */ 

/*!
 ******************************************************************************
 ** \brief DMA Interrupt Type structure
 ******************************************************************************
 */
#ifndef _DMAC_FM3_H_
#define _DMAC_FM3_H_

#include "fm3_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* DMA channel judge */                                            
/*!
 ******************************************************************************
 ** \brief define DMA channel 0
 ******************************************************************************
 */  
#define DMA_ch_0             ((uint8_t)0x00)
/*!
 ******************************************************************************
 ** \brief define DMA channel 1
 ******************************************************************************
 */  
#define DMA_ch_1             ((uint8_t)0x01)
/*!
 ******************************************************************************
 ** \brief define DMA channel 2
 ******************************************************************************
 */  
#define DMA_ch_2             ((uint8_t)0x02)
/*!
 ******************************************************************************
 ** \brief define DMA channel 3
 ******************************************************************************
 */    
#define DMA_ch_3             ((uint8_t)0x03)
/*!
 ******************************************************************************
 ** \brief define DMA channel 4
 ******************************************************************************
 */    
#define DMA_ch_4             ((uint8_t)0x04)
/*!
 ******************************************************************************
 ** \brief define DMA channel 5
 ******************************************************************************
 */    
#define DMA_ch_5             ((uint8_t)0x05)
/*!
 ******************************************************************************
 ** \brief define DMA channel 6
 ******************************************************************************
 */   
#define DMA_ch_6             ((uint8_t)0x06)
/*!
 ******************************************************************************
 ** \brief define DMA channel 7
 ******************************************************************************
 */   
#define DMA_ch_7             ((uint8_t)0x07)
/*!
 ******************************************************************************
 ** \brief DMA channel range check
 ******************************************************************************
 */
#define IS_DMA_CH_VALID(ch)       ((ch == DMA_ch_0) || \
                                  (ch == DMA_ch_1)  || \
                                  (ch == DMA_ch_2)  || \
                                  (ch == DMA_ch_3)  || \
                                  (ch == DMA_ch_4)  || \
                                  (ch == DMA_ch_5)  || \
                                  (ch == DMA_ch_6)  || \
                                  (ch == DMA_ch_7) )

/*!
 ******************************************************************************
 ** \brief DMA interrupt type
 ******************************************************************************
 */
typedef enum DMAIntType
{
    ErrAddOverflow=1,    //!< address over error
    StopRqst,            //!< stop request
    ErrSrsAddErr,        //!< source address error
    ErrDesAddErr,        //!< destination address error
    Success,             //!< success
    Pause                //!< pause
} DMAIntType;

/*!
 ******************************************************************************
 ** \brief DMA MFS trigger
 ******************************************************************************
 */
typedef enum DMA_MFSTrig
{
    ch0_Rx=1,      //!< MFS channel0 Rx
    ch0_Tx,        //!< MFS channel0 Tx
    ch1_Rx,        //!< MFS channel1 Rx
    ch1_Tx,        //!< MFS channel1 Tx
    ch2_Rx,        //!< MFS channel2 Rx
    ch2_Tx,        //!< MFS channel2 Tx
    ch3_Rx,        //!< MFS channel3 Rx
    ch3_Tx,        //!< MFS channel3 Tx
    ch4_Rx,        //!< MFS channel4 Rx
    ch4_Tx,        //!< MFS channel4 Tx
    ch5_Rx,        //!< MFS channel5 Rx
    ch5_Tx,        //!< MFS channel5 Tx
    ch6_Rx,        //!< MFS channel6 Rx
    ch6_Tx,        //!< MFS channel6 Tx
    ch7_Rx,        //!< MFS channel7 Rx
    ch7_Tx,        //!< MFS channel7 Tx
} DMA_MFSTrig;

/*
 ******************************************************************************
 ** \brief DMA USB trigger
 ******************************************************************************
 */
/*
typedef enum DMA_USBTrig
{
    USB0_EP1=1,
    USB0_EP2,
    USB0_EP3,
    USB0_EP4,
    USB0_EP5
} DMA_USBTrig;
*/

/*!
 ******************************************************************************
 ** \brief DMA callback function type
 ******************************************************************************
 */
typedef void (*DMA_Callback)(uint8_t Inttype);

/*!
 ******************************************************************************
 ** \brief DMA callback function array
 ******************************************************************************
 */
extern DMA_Callback DMA_IrqCallback[];


/*!
 ******************************************************************************
 ** \brief normal data transfer by software trigger
 ******************************************************************************
 */
void DMA_NrmlDataTrsfBySftTrig(uint32_t src, uint32_t des, 
                                      uint16_t bytes, uint8_t ch);
/*!
 ******************************************************************************
 ** \brief check DMA channel 0 busy status
 ******************************************************************************
 */
uint8_t DMA_IsNormSoftTrigBusy_Ch0(void);
/*!
 ******************************************************************************
 ** \brief check DMA channel 1 busy status
 ******************************************************************************
 */
uint8_t DMA_IsNormSoftTrigBusy_Ch1(void);
/*!
 ******************************************************************************
 ** \brief check DMA channel 2 busy status
 ******************************************************************************
 */
uint8_t DMA_IsNormSoftTrigBusy_Ch2(void);
/*!
 ******************************************************************************
 ** \brief check DMA channel 3 busy status
 ******************************************************************************
 */
uint8_t DMA_IsNormSoftTrigBusy_Ch3(void);
/*!
 ******************************************************************************
 ** \brief check DMA channel 4 busy status
 ******************************************************************************
 */
uint8_t DMA_IsNormSoftTrigBusy_Ch4(void);
/*!
 ******************************************************************************
 ** \brief check DMA channel 5 busy status
 ******************************************************************************
 */
uint8_t DMA_IsNormSoftTrigBusy_Ch5(void);
/*!
 ******************************************************************************
 ** \brief check DMA channel 6 busy status
 ******************************************************************************
 */
uint8_t DMA_IsNormSoftTrigBusy_Ch6(void);
/*!
 ******************************************************************************
 ** \brief check DMA channel 7 busy status
 ******************************************************************************
 */
uint8_t DMA_IsNormSoftTrigBusy_Ch7(void);

/*
 ******************************************************************************
 ** \brief DMA config setting for base timer
 ******************************************************************************
 */
void DMA_BSTM(uint8_t BSTM_ch, 
              uint8_t DMA_ch, 
              uint8_t DMA_ReptMode, 
              uint32_t src, 
              uint32_t des, 
              uint16_t bytes);

/*
 ******************************************************************************
 ** \brief DMA config setting for MFS
 ******************************************************************************
 */
void DMA_MFS(DMA_MFSTrig MFS_trig, 
             uint8_t DMA_ch, 
             uint8_t DMA_ReptMode, 
             uint32_t src, 
             uint32_t des, 
             uint16_t bytes);

/*
 ******************************************************************************
 ** \brief DMA config setting for external interrupt
 ******************************************************************************
 */
void DMA_ExtInt(uint8_t ExtInt_ch, 
                uint8_t DMA_ch, 
                uint8_t DMA_ReptMode, 
                uint32_t src, 
                uint32_t des, 
                uint16_t bytes);

/*
 ******************************************************************************
 ** \brief DMA config setting for ADC
 ******************************************************************************
 */
void DMA_AD(uint8_t AD_ch, 
            uint8_t DMA_ch, 
            uint8_t DMA_ReptMode, 
            uint32_t des, 
            uint16_t bytes);

/*
 ******************************************************************************
 ** DMA config setting for USB
 ******************************************************************************
 */
/*
void DMA_USB(uint8_t USB_EPnb, 
             uint8_t IN_OUT, 
             uint8_t DMA_ch, 
             uint8_t DMA_ReptMode, 
             uint32_t src, 
             uint32_t des, 
             uint16_t halfwords);
*/

/*
 ******************************************************************************
 ** \brief DMA resume function
 ******************************************************************************
 */
void DMA_Resume(unsigned char DMA_ch);

/*
 ******************************************************************************
 ** \brief DMA pause function
 ******************************************************************************
 */
void DMA_Pause(unsigned char DMA_ch);

/*
 ******************************************************************************
 ** \brief DMA disable function
 ******************************************************************************
 */
void DMA_Disable(unsigned char DMA_ch);

/*
 ******************************************************************************
 ** \brief DMA enable function
 ******************************************************************************
 */
void DMA_Enable(unsigned char DMA_ch);

#ifdef __cplusplus
}
#endif

#endif
