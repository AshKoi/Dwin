/*!
 ******************************************************************************
 **
 ** \file dmac_fm3.c
 **
 ** \brief this file provides all DMA drivers
 **
 ** \author FSLA AE Team
 **
 ** \version V0.10
 **
 ** \date 2011-11-18
 **
 ** \attention THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU SEMICONDUCTOR
 **            ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
 **            OMMISSIONS.
 **
 ** (C) Copyright 200x-201x by Fujitsu Semiconductor(Shanghai) Co.,Ltd.
 **
 ******************************************************************************
 */

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/

#include "dmac_fm3.h"

/*---------------------------------------------------------------------------*/
/* local datatypes                                                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* local data                                                                */
/*---------------------------------------------------------------------------*/
/*!
 ******************************************************************************
 ** \brief specify DMA irq 
 ******************************************************************************
 */
IRQn_Type irqn;
/*!
 ******************************************************************************
 ** \brief flags for indicating DMA0 busy on normal data transfer by software trigger mode
 ******************************************************************************
 */ 
uint8_t NormSoftTrigFlag0=0;
/*!
 ******************************************************************************
 ** \brief flags for indicating DMA1 busy on normal data transfer by software trigger mode
 ******************************************************************************
 */
uint8_t NormSoftTrigFlag1=0;
/*!
 ******************************************************************************
 ** \brief flags for indicating DMA2 busy on normal data transfer by software trigger mode
 ******************************************************************************
 */
uint8_t NormSoftTrigFlag2=0;
/*!
 ******************************************************************************
 ** \brief flags for indicating DMA3 busy on normal data transfer by software trigger mode
 ******************************************************************************
 */
uint8_t NormSoftTrigFlag3=0;
/*!
 ******************************************************************************
 ** \brief flags for indicating DMA4 busy on normal data transfer by software trigger mode
 ******************************************************************************
 */
uint8_t NormSoftTrigFlag4=0;
/*!
 ******************************************************************************
 ** \brief flags for indicating DMA5 busy on normal data transfer by software trigger mode
 ******************************************************************************
 */
uint8_t NormSoftTrigFlag5=0;
/*!
 ******************************************************************************
 ** \brief flags for indicating DMA6 busy on normal data transfer by software trigger mode
 ******************************************************************************
 */
uint8_t NormSoftTrigFlag6=0;
/*!
 ******************************************************************************
 ** \brief flags for indicating DMA7 busy on normal data transfer by software trigger mode
 ******************************************************************************
 */
uint8_t NormSoftTrigFlag7=0;

/*---------------------------------------------------------------------------*/
/* global data                                                               */
/*---------------------------------------------------------------------------*/

/*!
 ******************************************************************************
 ** \brief DMA callback function array
 ******************************************************************************
 */ 
DMA_Callback DMA_IrqCallback[] = 
{ 
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};
/*---------------------------------------------------------------------------*/
/* global functions                                                          */
/*---------------------------------------------------------------------------*/

/*!
 ******************************************************************************
 ** \brief normal data transfer by software trigger
 **
 ** \param src source address.
 **        This parameter can be one of the following values:
 ** \arg   physical_adress e.g. 0x20000100

 ** \param des destination address.
 **        This parameter can be one of the following values:
 ** \arg   physical_adress e.g. 0x20000800

 ** \param bytes data counts.
 **        This parameter can be one of the following values:
 ** \arg   byt_counts range:0x0000-0xFFFF

 ** \param ch
 **        This parameter can be one of the following values:
 ** \arg   0x00: DMA channel 0
 ** \arg   0x01: DMA channel 1
 ** \arg   0x02: DMA channel 2
 ** \arg   0x03: DMA channel 3
 ** \arg   0x04: DMA channel 4
 ** \arg   0x05: DMA channel 5
 ** \arg   0x06: DMA channel 6
 ** \arg   0x07: DMA channel 7
 **         
 **
 ** \retval None
 **
 ******************************************************************************
 */
void DMA_NrmlDataTrsfBySftTrig(uint32_t src, uint32_t des, 
                               uint16_t bytes, uint8_t ch)
{
    /* Check the parameter */
    ASSERT(IS_DMA_CH_VALID(ch));
    
    FM3_DMAC->DMACR = 0x80000000;//enable DMAC
    switch (ch)
    {
        case 0:
            NormSoftTrigFlag0=1;
            irqn = DMAC0_IRQn;
            NVIC_EnableIRQ(irqn);
            NVIC_SetPriority(irqn, 0);
            FM3_DMAC->DMACSA0 = src;
            FM3_DMAC->DMACDA0 = des;
            FM3_DMAC->DMACB0 = 0x00180000;
            FM3_DMAC->DMACA0 = (0x0000FFFF & bytes);
            bFM3_DMAC_DMACA0_EB = 1;
            bFM3_DMAC_DMACA0_ST = 1;
        break;
        case 1:
            NormSoftTrigFlag1=1;
            irqn = DMAC1_IRQn;
            NVIC_EnableIRQ(irqn);
            NVIC_SetPriority(irqn, 0);
            FM3_DMAC->DMACSA1 = src;
            FM3_DMAC->DMACDA1 = des;
            FM3_DMAC->DMACB1 = 0x00180000;
            FM3_DMAC->DMACA1 = (0x0000FFFF & bytes);
            bFM3_DMAC_DMACA1_EB = 1;
            bFM3_DMAC_DMACA1_ST = 1;
        break;
        case 2:
            NormSoftTrigFlag2=1;
            irqn = DMAC2_IRQn;
            NVIC_EnableIRQ(irqn);
            NVIC_SetPriority(irqn, 0);
            FM3_DMAC->DMACSA2 = src;
            FM3_DMAC->DMACDA2 = des;
            FM3_DMAC->DMACB2 = 0x00180000;
            FM3_DMAC->DMACA2 = (0x0000FFFF & bytes);
            bFM3_DMAC_DMACA2_EB = 1;
            bFM3_DMAC_DMACA2_ST = 1;
        break;
        case 3:
            NormSoftTrigFlag3=1;
            irqn = DMAC3_IRQn;
            NVIC_EnableIRQ(irqn);
            NVIC_SetPriority(irqn, 0);
            FM3_DMAC->DMACSA3 = src;
            FM3_DMAC->DMACDA3 = des;
            FM3_DMAC->DMACB3 = 0x00180000;
            FM3_DMAC->DMACA3 = (0x0000FFFF & bytes);
            bFM3_DMAC_DMACA3_EB = 1;
            bFM3_DMAC_DMACA3_ST = 1;
        break;
        case 4:
            NormSoftTrigFlag4=1;
            irqn = DMAC4_IRQn;
            NVIC_EnableIRQ(irqn);
            NVIC_SetPriority(irqn, 0);
            FM3_DMAC->DMACSA4 = src;
            FM3_DMAC->DMACDA4 = des;
            FM3_DMAC->DMACB4 = 0x00180000;
            FM3_DMAC->DMACA4 = (0x0000FFFF & bytes);
            bFM3_DMAC_DMACA4_EB = 1;
            bFM3_DMAC_DMACA4_ST = 1;
        break;
        case 5:
            NormSoftTrigFlag5=1;
            irqn = DMAC5_IRQn;
            NVIC_EnableIRQ(irqn);
            NVIC_SetPriority(irqn, 0);
            FM3_DMAC->DMACSA5 = src;
            FM3_DMAC->DMACDA5 = des;
            FM3_DMAC->DMACB5 = 0x00180000;
            FM3_DMAC->DMACA5 = (0x0000FFFF & bytes);
            bFM3_DMAC_DMACA5_EB = 1;
            bFM3_DMAC_DMACA5_ST = 1;
        break;
        case 6:
            NormSoftTrigFlag6=1;
            irqn = DMAC6_IRQn;
            NVIC_EnableIRQ(irqn);
            NVIC_SetPriority(irqn, 0);
            FM3_DMAC->DMACSA6 = src;
            FM3_DMAC->DMACDA6 = des;
            FM3_DMAC->DMACB6 = 0x00180000;
            FM3_DMAC->DMACA6 = (0x0000FFFF & bytes);
            bFM3_DMAC_DMACA6_EB = 1;
            bFM3_DMAC_DMACA6_ST = 1;
        break;
        case 7:
            NormSoftTrigFlag7=1;
            irqn = DMAC7_IRQn;
            NVIC_EnableIRQ(irqn);
            NVIC_SetPriority(irqn, 0);
            FM3_DMAC->DMACSA7 = src;
            FM3_DMAC->DMACDA7 = des;
            FM3_DMAC->DMACB7 = 0x00180000;
            FM3_DMAC->DMACA7 = (0x0000FFFF & bytes);
            bFM3_DMAC_DMACA7_EB = 1;
            bFM3_DMAC_DMACA7_ST = 1;
        break;
        default:
            
        break;
    }
    
}

/*!
 ******************************************************************************
 ** \brief check DMA channel0 busy status on normal software trigger mode
 **
 ** \param none
 **
 ** \retval 1 busy
 ** \retval 0 not busy
 **
 ******************************************************************************
 */
uint8_t DMA_IsNormSoftTrigBusy_Ch0(void)
{
    if( NormSoftTrigFlag0 == 0 )
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/*!
 ******************************************************************************
 ** \brief check DMA channel1 busy status on normal software trigger mode
 **
 ** \param none
 **
 ** \retval 1 busy
 ** \retval 0 not busy
 **
 ******************************************************************************
 */
uint8_t DMA_IsNormSoftTrigBusy_Ch1(void)
{
    if( NormSoftTrigFlag1 == 0 )
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/*!
 ******************************************************************************
 ** \brief check DMA channel2 busy status on normal software trigger mode
 **
 ** \param none
 **
 ** \retval 1 busy
 ** \retval 0 not busy
 **
 ******************************************************************************
 */
uint8_t DMA_IsNormSoftTrigBusy_Ch2(void)
{
    if( NormSoftTrigFlag2 == 0 )
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/*!
 ******************************************************************************
 ** \brief check DMA channel3 busy status on normal software trigger mode
 **
 ** \param none
 **
 ** \retval 1 busy
 ** \retval 0 not busy
 **
 ******************************************************************************
 */
uint8_t DMA_IsNormSoftTrigBusy_Ch3(void)
{
    if( NormSoftTrigFlag3 == 0 )
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/*!
 ******************************************************************************
 ** \brief check DMA channel4 busy status on normal software trigger mode
 **
 ** \param none
 **
 ** \retval 1 busy
 ** \retval 0 not busy
 **
 ******************************************************************************
 */
uint8_t DMA_IsNormSoftTrigBusy_Ch4(void)
{
    if( NormSoftTrigFlag4 == 0 )
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/*!
 ******************************************************************************
 ** \brief check DMA channel5 busy status on normal software trigger mode
 **
 ** \param none
 **
 ** \retval 1 busy
 ** \retval 0 not busy
 **
 ******************************************************************************
 */
uint8_t DMA_IsNormSoftTrigBusy_Ch5(void)
{
    if( NormSoftTrigFlag5 == 0 )
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/*!
 ******************************************************************************
 ** \brief check DMA channel6 busy status on normal software trigger mode
 **
 ** \param none
 **
 ** \retval 1 busy
 ** \retval 0 not busy
 **
 ******************************************************************************
 */
uint8_t DMA_IsNormSoftTrigBusy_Ch6(void)
{
    if( NormSoftTrigFlag6 == 0 )
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/*!
 ******************************************************************************
 ** \brief check DMA channel7 busy status on normal software trigger mode
 **
 ** \param none
 **
 ** \retval 1 busy
 ** \retval 0 not busy
 **
 ******************************************************************************
 */
uint8_t DMA_IsNormSoftTrigBusy_Ch7(void)
{
    if( NormSoftTrigFlag7 == 0 )
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/*!
 ******************************************************************************
 ** \brief DMA0 handler function
 **
 ** \param none
 **
 ** \retval none
 **
 ******************************************************************************
 */
void DMAC0_Handler(void)
{
    DMAIntType tpye;
    if( (FM3_DMAC->DMACB0 & 0x00070000) == 0x00010000 )
    {
        tpye=ErrAddOverflow;
    }
    else if( (FM3_DMAC->DMACB0 & 0x00070000) == 0x00020000 )
    {
        tpye=StopRqst;
    }
    else if( (FM3_DMAC->DMACB0 & 0x00070000) == 0x00030000 )
    {
        tpye=ErrSrsAddErr;
    }
    else if( (FM3_DMAC->DMACB0 & 0x00070000) == 0x00040000 )
    {
        tpye=ErrDesAddErr;
    }
    else if( (FM3_DMAC->DMACB0 & 0x00070000) == 0x00050000 )
    {
        tpye=Success;
    }
    else if( (FM3_DMAC->DMACB0 & 0x00070000) == 0x00070000 )
    {
        tpye=Pause;
    }
    
    if( DMA_IrqCallback[0]!=NULL )
    {
        DMA_IrqCallback[0](tpye);
    }
    FM3_DMAC->DMACB0 &= 0xFFF8FFFF;//clear SS bit
    NormSoftTrigFlag0=0;
}

/*!
 ******************************************************************************
 ** \brief DMA1 handler function
 **
 ** \param none
 **
 ** \retval none
 **
 ******************************************************************************
 */
void DMAC1_Handler(void)
{
    DMAIntType tpye;
    if( (FM3_DMAC->DMACB1 & 0x00070000) == 0x00010000 )
    {
        tpye=ErrAddOverflow;
    }
    else if( (FM3_DMAC->DMACB1 & 0x00070000) == 0x00020000 )
    {
        tpye=StopRqst;
    }
    else if( (FM3_DMAC->DMACB1 & 0x00070000) == 0x00030000 )
    {
        tpye=ErrSrsAddErr;
    }
    else if( (FM3_DMAC->DMACB1 & 0x00070000) == 0x00040000 )
    {
        tpye=ErrDesAddErr;
    }
    else if( (FM3_DMAC->DMACB1 & 0x00070000) == 0x00050000 )
    {
        tpye=Success;
    }
    else if( (FM3_DMAC->DMACB1 & 0x00070000) == 0x00070000 )
    {
        tpye=Pause;
    }
    
    if( DMA_IrqCallback[1]!=NULL )
    {
        DMA_IrqCallback[1](tpye);
    }
    
    FM3_DMAC->DMACB1 &= 0xFFF8FFFF;
    NormSoftTrigFlag1=0;
}

/*!
 ******************************************************************************
 ** \brief DMA2 handler function
 **
 ** \param none
 **
 ** \retval none
 **
 ******************************************************************************
 */
void DMAC2_Handler(void)
{
    DMAIntType tpye;
    if( (FM3_DMAC->DMACB2 & 0x00070000) == 0x00010000 )
    {
        tpye=ErrAddOverflow;
    }
    else if( (FM3_DMAC->DMACB2 & 0x00070000) == 0x00020000 )
    {
        tpye=StopRqst;
    }
    else if( (FM3_DMAC->DMACB2 & 0x00070000) == 0x00030000 )
    {
        tpye=ErrSrsAddErr;
    }
    else if( (FM3_DMAC->DMACB2 & 0x00070000) == 0x00040000 )
    {
        tpye=ErrDesAddErr;
    }
    else if( (FM3_DMAC->DMACB2 & 0x00070000) == 0x00050000 )
    {
        tpye=Success;
    }
    else if( (FM3_DMAC->DMACB2 & 0x00070000) == 0x00070000 )
    {
        tpye=Pause;
    }
    
    if( DMA_IrqCallback[2]!=NULL )
    {
        DMA_IrqCallback[2](tpye);
    }
    
    FM3_DMAC->DMACB2 &= 0xFFF8FFFF;
    NormSoftTrigFlag2=0;
}

/*!
 ******************************************************************************
 ** \brief DMA3 handler function
 **
 ** \param none
 **
 ** \retval none
 **
 ******************************************************************************
 */
void DMAC3_Handler(void)
{
    DMAIntType tpye;
    if( (FM3_DMAC->DMACB3 & 0x00070000) == 0x00010000 )
    {
        tpye=ErrAddOverflow;
    }
    else if( (FM3_DMAC->DMACB3 & 0x00070000) == 0x00020000 )
    {
        tpye=StopRqst;
    }
    else if( (FM3_DMAC->DMACB3 & 0x00070000) == 0x00030000 )
    {
        tpye=ErrSrsAddErr;
    }
    else if( (FM3_DMAC->DMACB3 & 0x00070000) == 0x00040000 )
    {
        tpye=ErrDesAddErr;
    }
    else if( (FM3_DMAC->DMACB3 & 0x00070000) == 0x00050000 )
    {
        tpye=Success;
    }
    else if( (FM3_DMAC->DMACB3 & 0x00070000) == 0x00070000 )
    {
        tpye=Pause;
    }
    
    if( DMA_IrqCallback[3]!=NULL )
    {
        DMA_IrqCallback[3](tpye);
    }
    
    FM3_DMAC->DMACB3 &= 0xFFF8FFFF;
   NormSoftTrigFlag3=0;
}

/*!
 ******************************************************************************
 ** \brief DMA4 handler function
 **
 ** \param none
 **
 ** \retval none
 **
 ******************************************************************************
 */
void DMAC4_Handler(void)
{
    DMAIntType tpye;
    if( (FM3_DMAC->DMACB4 & 0x00070000) == 0x00010000 )
    {
        tpye=ErrAddOverflow;
    }
    else if( (FM3_DMAC->DMACB4 & 0x00070000) == 0x00020000 )
    {
        tpye=StopRqst;
    }
    else if( (FM3_DMAC->DMACB4 & 0x00070000) == 0x00030000 )
    {
        tpye=ErrSrsAddErr;
    }
    else if( (FM3_DMAC->DMACB4 & 0x00070000) == 0x00040000 )
    {
        tpye=ErrDesAddErr;
    }
    else if( (FM3_DMAC->DMACB4 & 0x00070000) == 0x00050000 )
    {
        tpye=Success;
    }
    else if( (FM3_DMAC->DMACB4 & 0x00070000) == 0x00070000 )
    {
        tpye=Pause;
    }
    
    if( DMA_IrqCallback[4]!=NULL )
    {
        DMA_IrqCallback[4](tpye);
    }
    
    FM3_DMAC->DMACB4 &= 0xFFF8FFFF;
    NormSoftTrigFlag4=0;
}

/*!
 ******************************************************************************
 ** \brief DMA5 handler function
 **
 ** \param none
 **
 ** \retval none
 **
 ******************************************************************************
 */
void DMAC5_Handler(void)
{
    DMAIntType tpye;
    if( (FM3_DMAC->DMACB5 & 0x00070000) == 0x00010000 )
    {
        tpye=ErrAddOverflow;
    }
    else if( (FM3_DMAC->DMACB5 & 0x00070000) == 0x00020000 )
    {
        tpye=StopRqst;
    }
    else if( (FM3_DMAC->DMACB5 & 0x00070000) == 0x00030000 )
    {
        tpye=ErrSrsAddErr;
    }
    else if( (FM3_DMAC->DMACB5 & 0x00070000) == 0x00040000 )
    {
        tpye=ErrDesAddErr;
    }
    else if( (FM3_DMAC->DMACB5 & 0x00070000) == 0x00050000 )
    {
        tpye=Success;
    }
    else if( (FM3_DMAC->DMACB5 & 0x00070000) == 0x00070000 )
    {
        tpye=Pause;
    }
    
    if( DMA_IrqCallback[5]!=NULL )
    {
        DMA_IrqCallback[5](tpye);
    }
    
    FM3_DMAC->DMACB5 &= 0xFFF8FFFF;
    NormSoftTrigFlag5=0;
}

/*!
 ******************************************************************************
 ** \brief DMA6 handler function
 **
 ** \param none
 **
 ** \retval none
 **
 ******************************************************************************
 */
void DMAC6_Handler(void)
{
    
    DMAIntType tpye;
    if( (FM3_DMAC->DMACB6 & 0x00070000) == 0x00010000 )
    {
        tpye=ErrAddOverflow;
    }
    else if( (FM3_DMAC->DMACB6 & 0x00070000) == 0x00020000 )
    {
        tpye=StopRqst;
    }
    else if( (FM3_DMAC->DMACB6 & 0x00070000) == 0x00030000 )
    {
        tpye=ErrSrsAddErr;
    }
    else if( (FM3_DMAC->DMACB6 & 0x00070000) == 0x00040000 )
    {
        tpye=ErrDesAddErr;
    }
    else if( (FM3_DMAC->DMACB6 & 0x00070000) == 0x00050000 )
    {
        tpye=Success;
    }
    else if( (FM3_DMAC->DMACB6 & 0x00070000) == 0x00070000 )
    {
        tpye=Pause;
    }
    
    if( DMA_IrqCallback[6]!=NULL )
    {
        DMA_IrqCallback[6](tpye);
    }
    
    FM3_DMAC->DMACB6 &= 0xFFF8FFFF;
    NormSoftTrigFlag6=0;
}

/*!
 ******************************************************************************
 ** \brief DMA7 handler function
 **
 ** \param none
 **
 ** \retval none
 **
 ******************************************************************************
 */
//int test=0;

void DMAC7_Handler(void)
{
    DMAIntType tpye;
    if( (FM3_DMAC->DMACB7 & 0x00070000) == 0x00010000 )
    {
        tpye=ErrAddOverflow;
    }
    else if( (FM3_DMAC->DMACB7 & 0x00070000) == 0x00020000 )
    {
        tpye=StopRqst;
    }
    else if( (FM3_DMAC->DMACB7 & 0x00070000) == 0x00030000 )
    {
        tpye=ErrSrsAddErr;
    }
    else if( (FM3_DMAC->DMACB7 & 0x00070000) == 0x00040000 )
    {
        tpye=ErrDesAddErr;
    }
    else if( (FM3_DMAC->DMACB7 & 0x00070000) == 0x00050000 )
    {
        tpye=Success;
    }
    else if( (FM3_DMAC->DMACB7 & 0x00070000) == 0x00070000 )
    {
        tpye=Pause;
    }
    
    if( DMA_IrqCallback[7]!=NULL )
    {
        DMA_IrqCallback[7](tpye);
    }
    
    FM3_DMAC->DMACB7 &= 0xFFF8FFFF;
    NormSoftTrigFlag7=0;
}

/*!
 ******************************************************************************
 ** \brief DMA configuration setting for AD. The source address always points to SCFD register of corresponding ADC macro.
 **
 ** \param AD_unit AD unit.
 **        This parameter can be one of the following values:
 ** \arg   0: AD unit 0
 ** \arg   1: AD unit 1
 ** \arg   2: AD unit 2

 ** \param DMA_ch DMA channel.
 **        This parameter can be one of the following values:
 ** \arg   0: DMA channel 0
 ** \arg   1: DMA channel 1
 ** \arg   2: DMA channel 2
 ** \arg   3: DMA channel 3
 ** \arg   4: DMA channel 4
 ** \arg   5: DMA channel 5
 ** \arg   6: DMA channel 6
 ** \arg   7: DMA channel 7

 ** \param DMA_ReptMode DMA repeat mode.
 **        This parameter can be one of the following values:
 ** \arg   0: Single mode. Only catchs the first interrupt request.
 ** \arg   1: Repeat mode. Catchs every interrupt request. But source address and target address and byte counts will be reloaded

 ** \param des Destination address.
 **        This parameter can be one of the following values:
 ** \arg   Physical adress

 ** \param bytes Byte counts.
 **        This parameter can be one of the following values:
 ** \arg   0-65535: Count in word(32 bit). Transfer counts=bytes+1. How many data need to be transferred. Since source target is a 32 bit register, so 0 is recommended if you only need AD result data.

 ** \retval None
 **
 ******************************************************************************
 */

///////////////////////////////////////////////////////////////////////

void DMA_AD(uint8_t AD_unit, 
            uint8_t DMA_ch, 
            uint8_t DMA_ReptMode, 
            uint32_t des, 
            uint16_t bytes)
{

  
    if(AD_unit == 0)
    {
        FM3_INTREQ->DRQSEL = 0x00000020;
        FM3_DMAC->DMACR = 0x80000000;
        *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) = (0x92800000 | bytes);
        //because address base is 4 (not 1), so the step of address calculation should be 4
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x2A180000;
        *(&(FM3_DMAC->DMACSA0)+DMA_ch*0x04) = (uint32_t) (&(FM3_ADC0->SCFD));
        *(&(FM3_DMAC->DMACDA0)+DMA_ch*0x04) = des;
        
    }
    else if(AD_unit == 1)
    {
        FM3_INTREQ->DRQSEL = 0x00000040;
        FM3_DMAC->DMACR = 0x80000000;
        *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) = (0x93000000 | bytes);
        //because address base is 4 (not 1), so the step of address calculation should be 4
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x2A180000;
        *(&(FM3_DMAC->DMACSA0)+DMA_ch*0x04) = (uint32_t) (&(FM3_ADC1->SCFD));
        *(&(FM3_DMAC->DMACDA0)+DMA_ch*0x04) = des;
    }
    else if(AD_unit == 2)
    {
        FM3_INTREQ->DRQSEL = 0x00000080;
        FM3_DMAC->DMACR = 0x80000000;
        *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) = (0x93800000 | bytes);
        //because address base is 4 (not 1), so the step of address calculation should be 4
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x2A180000;
        *(&(FM3_DMAC->DMACSA0)+DMA_ch*0x04) = (uint32_t) (&(FM3_ADC2->SCFD));
        *(&(FM3_DMAC->DMACDA0)+DMA_ch*0x04) = des;
    }
    
    if(DMA_ReptMode==1)
    {
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) |= 0x00000001;//set EM
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) |= 0x00E00000;//set RC,RD,RS
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) &= 0xFFF7FFFF;//clear CI, repeat mode no need CI
    }
    
    
    if(DMA_ch == 0)
    {
        irqn = DMAC0_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
    
    if(DMA_ch == 1)
    {
        irqn = DMAC1_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
    
    if(DMA_ch == 2)
    {
        irqn = DMAC2_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
    
    if(DMA_ch == 3)
    {
        irqn = DMAC3_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
    
    if(DMA_ch == 4)
    {
        irqn = DMAC4_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
    
    if(DMA_ch == 5)
    {
        irqn = DMAC5_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
    
    if(DMA_ch == 6)
    {
        irqn = DMAC6_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
    
    if(DMA_ch == 7)
    {
        irqn = DMAC7_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
}

/*!
 ******************************************************************************
 ** \brief DMA configuration setting for Base timer
 **
 ** \param BSTM_ch Base timer channel.
 **        This parameter can be one of the following values:
 ** \arg   0: Base timer channel 0
 ** \arg   2: Base timer channel 2
 ** \arg   4: Base timer channel 4
 ** \arg   6: Base timer channel 6

 ** \param DMA_ch DMA channel.
 **        This parameter can be one of the following values:
 ** \arg   0: DMA channel 0
 ** \arg   1: DMA channel 1
 ** \arg   2: DMA channel 2
 ** \arg   3: DMA channel 3
 ** \arg   4: DMA channel 4
 ** \arg   5: DMA channel 5
 ** \arg   6: DMA channel 6
 ** \arg   7: DMA channel 7

 ** \param DMA_ReptMode DMA repeat mode.
 **        This parameter can be one of the following values:
 ** \arg   0: Single mode. Only catchs the first interrupt request.
 ** \arg   1: Repeat mode. Catchs every interrupt request. But source address and target address and byte counts will be reloaded

 ** \param src Source address.
 **        This parameter can be one of the following values:
 ** \arg   Physical adress

 ** \param des Destination address.
 **        This parameter can be one of the following values:
 ** \arg   Physical adress

 ** \param bytes Byte counts.
 **        This parameter can be one of the following values:
 ** \arg   0-65535. Transfer counts=bytes+1. How many data need to be transferred.

 ** \retval None
 **
 ******************************************************************************
 */

void DMA_BSTM(uint8_t BSTM_ch, 
              uint8_t DMA_ch, 
              uint8_t DMA_ReptMode, 
              uint32_t src, 
              uint32_t des, 
              uint16_t bytes)
{  
    if(BSTM_ch == 0)
    {
        FM3_INTREQ->DRQSEL = 0x00000100;
        FM3_DMAC->DMACR = 0x80000000;
        *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) = (0x94000000 | bytes);
        //because address base is 4 (not 1), so the step of address calculation should be 4
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x20180000;
        *(&(FM3_DMAC->DMACSA0)+DMA_ch*0x04) = src;
        *(&(FM3_DMAC->DMACDA0)+DMA_ch*0x04) = des;
    }
    
    if(BSTM_ch == 2)
    {
        FM3_INTREQ->DRQSEL = 0x00000200;
        FM3_DMAC->DMACR = 0x80000000;
        *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) = ((0x94000000+(0x01<<23)) | bytes);
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x20180000;
        *(&(FM3_DMAC->DMACSA0)+DMA_ch*0x04) = src;
        *(&(FM3_DMAC->DMACDA0)+DMA_ch*0x04) = des;
    }
    
    if(BSTM_ch == 4)
    {
        FM3_INTREQ->DRQSEL = 0x00000400;
        FM3_DMAC->DMACR = 0x80000000;
        *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) = ((0x94000000+(0x02<<23)) | bytes);
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x20180000;
        *(&(FM3_DMAC->DMACSA0)+DMA_ch*0x04) = src;
        *(&(FM3_DMAC->DMACDA0)+DMA_ch*0x04) = des;
    }
    
    if(BSTM_ch == 6)
    {
        FM3_INTREQ->DRQSEL = 0x00000800;
        FM3_DMAC->DMACR = 0x80000000;
        *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) = ((0x94000000+(0x03<<23)) | bytes);
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x20180000;
        *(&(FM3_DMAC->DMACSA0)+DMA_ch*0x04) = src;
        *(&(FM3_DMAC->DMACDA0)+DMA_ch*0x04) = des;
    }
    
    if(DMA_ReptMode==1)
    {
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) |= 0x00000001;//set EM
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) |= 0x00E00000;//set RC,RD,RS
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) &= 0xFFF7FFFF;//clear CI
    }
    
    
    if(DMA_ch == 0)
    {
        irqn = DMAC0_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
    
    if(DMA_ch == 1)
    {
        irqn = DMAC1_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
    
    if(DMA_ch == 2)
    {
        irqn = DMAC2_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
    
    if(DMA_ch == 3)
    {
        irqn = DMAC3_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
    
    if(DMA_ch == 4)
    {
        irqn = DMAC4_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
    
    if(DMA_ch == 5)
    {
        irqn = DMAC5_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
    
    if(DMA_ch == 6)
    {
        irqn = DMAC6_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
    
    if(DMA_ch == 7)
    {
        irqn = DMAC7_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
}

/*!
 ******************************************************************************
 ** \brief DMA configuration setting for MFS
 **
 ** \param MFS_trig MFS trigger type.
 **        This parameter can be one of the following values:
 ** \arg   ch0_Rx: MFS ch0 Rx
 ** \arg   ch0_Tx: MFS ch0 Tx
 ** \arg   ch1_Rx: MFS ch0 Rx
 ** \arg   ch1_Tx: MFS ch1 Tx
 ** \arg   ch2_Rx: MFS ch0 Rx
 ** \arg   ch2_Tx: MFS ch2 Tx
 ** \arg   ch3_Rx: MFS ch0 Rx
 ** \arg   ch3_Tx: MFS ch3 Tx
 ** \arg   ch4_Rx: MFS ch0 Rx
 ** \arg   ch4_Tx: MFS ch4 Tx
 ** \arg   ch5_Rx: MFS ch0 Rx
 ** \arg   ch5_Tx: MFS ch5 Tx
 ** \arg   ch6_Rx: MFS ch0 Rx
 ** \arg   ch6_Tx: MFS ch6 Tx
 ** \arg   ch7_Rx: MFS ch0 Rx
 ** \arg   ch7_Tx: MFS ch7 Tx

 ** \param DMA_ch DMA channel.
 **        This parameter can be one of the following values:
 ** \arg   0: DMA channel 0
 ** \arg   1: DMA channel 1
 ** \arg   2: DMA channel 2
 ** \arg   3: DMA channel 3
 ** \arg   4: DMA channel 4
 ** \arg   5: DMA channel 5
 ** \arg   6: DMA channel 6
 ** \arg   7: DMA channel 7

 ** \param DMA_ReptMode DMA repeat mode.
 **        This parameter can be one of the following values:
 ** \arg   0: Single mode. Only catchs the first interrupt request.
 ** \arg   1: Repeat mode. Catchs every interrupt request. But source address and target address and byte counts will be reloaded.

 ** \param src Source address.
 **        This parameter can be one of the following values:
 ** \arg   Physical adress

 ** \param des Destination address.
 **        This parameter can be one of the following values:
 ** \arg   Physical adress

 ** \param bytes Byte counts.
 **        This parameter can be one of the following values:
 ** \arg   0-65535: count in byte. Transfer counts=bytes+1. How many data need to be transferred.

 ** \retval None
 **
 ******************************************************************************
 */
void DMA_MFS(DMA_MFSTrig MFS_trig, 
             uint8_t DMA_ch, 
             uint8_t DMA_ReptMode, 
             uint32_t src, 
             uint32_t des, 
             uint16_t bytes)
{
  
    if(MFS_trig == ch0_Rx)
    {
        FM3_INTREQ->DRQSEL |= 0x00001000;        //MFS ch.0的接收中断信号用作传输请求输出到DMAC
        FM3_DMAC->DMACR = 0x80000000;
        *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) = (0x96000000 | bytes);
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x22180000;
        *(&(FM3_DMAC->DMACSA0)+DMA_ch*0x04) = (uint32_t) (&(FM3_MFS0_UART->RDR));
        *(&(FM3_DMAC->DMACDA0)+DMA_ch*0x04) = des;
    }
    if(MFS_trig == ch0_Tx)
    {
        FM3_INTREQ->DRQSEL |= (0x00001000<<1);
        FM3_DMAC->DMACR = 0x80000000;
        *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) = ((0x96000000+(1<<23)) | bytes);
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x21180000;
        *(&(FM3_DMAC->DMACSA0)+DMA_ch*0x04) = src;
        *(&(FM3_DMAC->DMACDA0)+DMA_ch*0x04) = (uint32_t) (&(FM3_MFS0_UART->TDR));
    }
    if(MFS_trig == ch1_Rx)
    {
        FM3_INTREQ->DRQSEL |= (0x00001000<<2);
        FM3_DMAC->DMACR = 0x80000000;
        *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) = ((0x96000000+(2<<23)) | bytes);
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x22180000;
        *(&(FM3_DMAC->DMACSA0)+DMA_ch*0x04) = (uint32_t) (&(FM3_MFS1_UART->RDR));
        *(&(FM3_DMAC->DMACDA0)+DMA_ch*0x04) = des;
    }
    if(MFS_trig == ch1_Tx)
    {
        FM3_INTREQ->DRQSEL |= (0x00001000<<3);
        FM3_DMAC->DMACR = 0x80000000;
        *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) = ((0x96000000+(3<<23)) | bytes);
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x21180000;
        *(&(FM3_DMAC->DMACSA0)+DMA_ch*0x04) = src;
        *(&(FM3_DMAC->DMACDA0)+DMA_ch*0x04) = (uint32_t) (&(FM3_MFS1_UART->TDR));
    }
    if(MFS_trig == ch2_Rx)
    {
        FM3_INTREQ->DRQSEL |= (0x00001000<<4);
        FM3_DMAC->DMACR = 0x80000000;
        *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) = ((0x96000000+(4<<23)) | bytes);
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x22180000;
        *(&(FM3_DMAC->DMACSA0)+DMA_ch*0x04) = (uint32_t) (&(FM3_MFS2_UART->RDR));
        *(&(FM3_DMAC->DMACDA0)+DMA_ch*0x04) = des;
    }
    if(MFS_trig == ch2_Tx)
    {
        FM3_INTREQ->DRQSEL |= (0x00001000<<5);
        FM3_DMAC->DMACR = 0x80000000;
        *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) = ((0x96000000+(5<<23)) | bytes);
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x21180000;
        *(&(FM3_DMAC->DMACSA0)+DMA_ch*0x04) = src;
        *(&(FM3_DMAC->DMACDA0)+DMA_ch*0x04) = (uint32_t) (&(FM3_MFS2_UART->TDR));
    }
    if(MFS_trig == ch3_Rx)
    {
        FM3_INTREQ->DRQSEL |= (0x00001000<<6);
        FM3_DMAC->DMACR = 0x80000000;
        *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) = ((0x96000000+(6<<23)) | bytes);
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x22180000;
        *(&(FM3_DMAC->DMACSA0)+DMA_ch*0x04) = (uint32_t) (&(FM3_MFS3_UART->RDR));
        *(&(FM3_DMAC->DMACDA0)+DMA_ch*0x04) = des;
    }
    if(MFS_trig == ch3_Tx)
    {
        FM3_INTREQ->DRQSEL |= (0x00001000<<7);
        FM3_DMAC->DMACR = 0x80000000;
        *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) = ((0x96000000+(7<<23)) | bytes);
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x21180000;
        *(&(FM3_DMAC->DMACSA0)+DMA_ch*0x04) = src;
        *(&(FM3_DMAC->DMACDA0)+DMA_ch*0x04) = (uint32_t) (&(FM3_MFS3_UART->TDR));
    }
    if(MFS_trig == ch4_Rx)
    {
        FM3_INTREQ->DRQSEL |= (0x00001000<<8);
        FM3_DMAC->DMACR = 0x80000000;
        *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) = ((0x96000000+(8<<23)) | bytes);
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x22180000;
        *(&(FM3_DMAC->DMACSA0)+DMA_ch*0x04) = (uint32_t) (&(FM3_MFS4_UART->RDR));
        *(&(FM3_DMAC->DMACDA0)+DMA_ch*0x04) = des;
    }
    if(MFS_trig == ch4_Tx)
    {
        FM3_INTREQ->DRQSEL |= (0x00001000<<9);
        FM3_DMAC->DMACR = 0x80000000;
        *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) = ((0x96000000+(9<<23)) | bytes);
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x21180000;
        *(&(FM3_DMAC->DMACSA0)+DMA_ch*0x04) = src;
        *(&(FM3_DMAC->DMACDA0)+DMA_ch*0x04) = (uint32_t) (&(FM3_MFS4_UART->TDR));
    }
    if(MFS_trig == ch5_Rx)
    {
        FM3_INTREQ->DRQSEL |= (0x00001000<<10);
        FM3_DMAC->DMACR = 0x80000000;
        *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) = ((0x96000000+(10<<23)) | bytes);
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x22180000;
        *(&(FM3_DMAC->DMACSA0)+DMA_ch*0x04) = (uint32_t) (&(FM3_MFS5_UART->RDR));
        *(&(FM3_DMAC->DMACDA0)+DMA_ch*0x04) = des;
    }
    if(MFS_trig == ch5_Tx)
    {
        FM3_INTREQ->DRQSEL |= (0x00001000<<11);
        FM3_DMAC->DMACR = 0x80000000;
        *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) = ((0x96000000+(11<<23)) | bytes);
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x21180000;
        *(&(FM3_DMAC->DMACSA0)+DMA_ch*0x04) = src;
        *(&(FM3_DMAC->DMACDA0)+DMA_ch*0x04) = (uint32_t) (&(FM3_MFS5_UART->TDR));
    }
    if(MFS_trig == ch6_Rx)
    {
        FM3_INTREQ->DRQSEL |= (0x00001000<<12);
        FM3_DMAC->DMACR = 0x80000000;
        *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) = ((0x96000000+(12<<23)) | bytes);
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x22180000;
        *(&(FM3_DMAC->DMACSA0)+DMA_ch*0x04) = (uint32_t) (&(FM3_MFS6_UART->RDR));
        *(&(FM3_DMAC->DMACDA0)+DMA_ch*0x04) = des;
    }
    if(MFS_trig == ch6_Tx)
    {
        FM3_INTREQ->DRQSEL |= (0x00001000<<13);
        FM3_DMAC->DMACR = 0x80000000;
        *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) = ((0x96000000+(13<<23)) | bytes);
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x21180000;
        *(&(FM3_DMAC->DMACSA0)+DMA_ch*0x04) = src;
        *(&(FM3_DMAC->DMACDA0)+DMA_ch*0x04) = (uint32_t) (&(FM3_MFS6_UART->TDR));
    }
    if(MFS_trig == ch7_Rx)
    {
        FM3_INTREQ->DRQSEL |= (0x00001000<<14);
        FM3_DMAC->DMACR = 0x80000000;
        *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) = ((0x96000000+(14<<23)) | bytes);
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x22180000;
        *(&(FM3_DMAC->DMACSA0)+DMA_ch*0x04) = (uint32_t) (&(FM3_MFS7_UART->RDR));
        *(&(FM3_DMAC->DMACDA0)+DMA_ch*0x04) = des;
    }
    if(MFS_trig == ch7_Tx)
    {
        FM3_INTREQ->DRQSEL |= (0x00001000<<15);
        FM3_DMAC->DMACR = 0x80000000;
        *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) = ((0x96000000+(15<<23)) | bytes);
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x21180000;
        *(&(FM3_DMAC->DMACSA0)+DMA_ch*0x04) = src;
        *(&(FM3_DMAC->DMACDA0)+DMA_ch*0x04) = (uint32_t) (&(FM3_MFS7_UART->TDR));
    }
    
    if(DMA_ReptMode==1)
    {
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) |= 0x00000001;//set EM
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) |= 0x00E00000;//set RC,RD,RS
//         *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) |= 0x00600000;//set RC
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) &= 0xFFF7FFFF;//clear CI
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) &= 0xFFEFFFFF;//clear EI
    }
    
    
    if(DMA_ch == 0)
    {
        irqn = DMAC0_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
    
    if(DMA_ch == 1)
    {
        irqn = DMAC1_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
    
    if(DMA_ch == 2)
    {
        irqn = DMAC2_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
    
    if(DMA_ch == 3)
    {
        irqn = DMAC3_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
    
    if(DMA_ch == 4)
    {
        irqn = DMAC4_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
    
    if(DMA_ch == 5)
    {
        irqn = DMAC5_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
    
    if(DMA_ch == 6)
    {
        irqn = DMAC6_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
    
    if(DMA_ch == 7)
    {
        irqn = DMAC7_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
}


/*!
 ******************************************************************************
 ** \brief DMA configuration setting for Exertna interrupt
 **
 ** \param ExtInt_ch External interrupt channel.
 **        This parameter can be one of the following values:
 ** \arg   0: External interrupt channel 0
 ** \arg   1: External interrupt channel 1
 ** \arg   2: External interrupt channel 2
 ** \arg   3: External interrupt channel 3

 ** \param DMA_ch DMA channel.
 **        This parameter can be one of the following values:
 ** \arg   0: DMA channel 0
 ** \arg   1: DMA channel 1
 ** \arg   2: DMA channel 2
 ** \arg   3: DMA channel 3
 ** \arg   4: DMA channel 4
 ** \arg   5: DMA channel 5
 ** \arg   6: DMA channel 6
 ** \arg   7: DMA channel 7

 ** \param DMA_ReptMode DMA repeat mode.
 **        This parameter can be one of the following values:
 ** \arg   0: Single mode. Only catchs the first interrupt request.
 ** \arg   1: Repeat mode. Catchs every interrupt request. But source address and target address and byte counts will be reloaded.

 ** \param src Source address.
 **        This parameter can be one of the following values:
 ** \arg   Physical adress

 ** \param des Destination address.
 **        This parameter can be one of the following values:
 ** \arg   Physical adress

 ** \param bytes Byte counts.
 **        This parameter can be one of the following values:
 ** \arg   0-65535: count in byte. Transfer counts=bytes+1. How many data need to be transferred.

 ** \retval None
 **
 ******************************************************************************
 */
void DMA_ExtInt(uint8_t ExtInt_ch, 
                uint8_t DMA_ch, 
                uint8_t DMA_ReptMode, 
                uint32_t src, 
                uint32_t des, 
                uint16_t bytes)
{
    if(ExtInt_ch == 0)
    {
        FM3_INTREQ->DRQSEL = 0x10000000;
        FM3_DMAC->DMACR = 0x80000000;
        *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) = (0x9E000000 | bytes);
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x20180000;
        *(&(FM3_DMAC->DMACSA0)+DMA_ch*0x04) = src;
        *(&(FM3_DMAC->DMACDA0)+DMA_ch*0x04) = des;
    }
    
    if(ExtInt_ch == 1)
    {
        FM3_INTREQ->DRQSEL = (0x10000000<<1);
        FM3_DMAC->DMACR = 0x80000000;
        *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) = ((0x9E000000+(1<<23)) | bytes);
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x20180000;
        *(&(FM3_DMAC->DMACSA0)+DMA_ch*0x04) = src;
        *(&(FM3_DMAC->DMACDA0)+DMA_ch*0x04) = des;
    }
    
    if(ExtInt_ch == 2)
    {
        FM3_INTREQ->DRQSEL = (0x10000000<<2);
        FM3_DMAC->DMACR = 0x80000000;
        *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) = ((0x9E000000+(2<<23)) | bytes);
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x20180000;
        *(&(FM3_DMAC->DMACSA0)+DMA_ch*0x04) = src;
        *(&(FM3_DMAC->DMACDA0)+DMA_ch*0x04) = des;
    }
    
    if(ExtInt_ch == 3)
    {
        FM3_INTREQ->DRQSEL = ((uint32_t) 0x10000000<<3);
        FM3_DMAC->DMACR = 0x80000000;
        *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) = ((0x9E000000+(3<<23)) | bytes);
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x20180000;
        *(&(FM3_DMAC->DMACSA0)+DMA_ch*0x04) = src;
        *(&(FM3_DMAC->DMACDA0)+DMA_ch*0x04) = des;
    }
    
    if(DMA_ReptMode==1)
    {
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) |= 0x00000001;//set EM
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) |= 0x00E00000;//set RC,RD,RS
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) &= 0xFFF7FFFF;//clear CI
    }
    
    
    
    if(DMA_ch == 0)
    {
        irqn = DMAC0_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
    
    if(DMA_ch == 1)
    {
        irqn = DMAC1_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
    
    if(DMA_ch == 2)
    {
        irqn = DMAC2_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
    
    if(DMA_ch == 3)
    {
        irqn = DMAC3_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
    
    if(DMA_ch == 4)
    {
        irqn = DMAC4_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
    
    if(DMA_ch == 5)
    {
        irqn = DMAC5_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
    
    if(DMA_ch == 6)
    {
        irqn = DMAC6_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
    
    if(DMA_ch == 7)
    {
        irqn = DMAC7_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
}


/*
 ******************************************************************************
 ** \brief DMA configuration setting for USB
 **
 ** \param USB_EPnb USB end point number.
 **        This parameter can be one of the following values:
 ** \arg   USB0_EP1: USB ch0 end point 1
 ** \arg   USB0_EP2: USB ch0 end point 2
 ** \arg   USB0_EP3: USB ch0 end point 3
 ** \arg   USB0_EP4: USB ch0 end point 4
 ** \arg   USB0_EP5: USB ch0 end point 5

 ** \param DMA_ch DMA channel.
 **        This parameter can be one of the following values:
 ** \arg   0: DMA channel 0
 ** \arg   1: DMA channel 1
 ** \arg   2: DMA channel 2
 ** \arg   3: DMA channel 3
 ** \arg   4: DMA channel 4
 ** \arg   5: DMA channel 5
 ** \arg   6: DMA channel 6
 ** \arg   7: DMA channel 7

 ** \param DMA_ReptMode DMA repeat mode.
 **        This parameter can be one of the following values:
 ** \arg   0: Single mode. Only catchs the first interrupt request.
 ** \arg   1: Repeat mode. Catchs every interrupt request. But source address and target address and byte counts will be reloaded.

 ** \param des Destination address.
 **        This parameter can be one of the following values:
 ** \arg   Physical adress

 ** \param halfwords Halfword counts.
 **        This parameter can be one of the following values:
 ** \arg   0-65535: count in halfword. Transfer counts=bytes+1. How many data need to be transferred.

 ** \retval None
 **
 ******************************************************************************
 */
/*
void DMA_USB(uint8_t USB_EPnb, 
             uint8_t IN_OUT, 
             uint8_t DMA_ch, 
             uint8_t DMA_ReptMode, 
             uint32_t src, 
             uint32_t des, 
             uint16_t halfwords)
{
  
    if(DMA_ch == 0)
    {
        irqn = DMAC0_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
    
    if(DMA_ch == 1)
    {
        irqn = DMAC1_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
    
    if(DMA_ch == 2)
    {
        irqn = DMAC2_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
    
    if(DMA_ch == 3)
    {
        irqn = DMAC3_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
    
    if(DMA_ch == 4)
    {
        irqn = DMAC4_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
    
    if(DMA_ch == 5)
    {
        irqn = DMAC5_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
    
    if(DMA_ch == 6)
    {
        irqn = DMAC6_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
    
    if(DMA_ch == 7)
    {
        irqn = DMAC7_IRQn;
        NVIC_EnableIRQ(irqn);
        NVIC_SetPriority(irqn, 0);
    }
 
  
    if(USB_EPnb == USB0_EP1)
    {
        FM3_INTREQ->DRQSEL = 0x00000001;
        FM3_DMAC->DMACR = 0x80000000;
        *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) = (0x90000000 | halfwords);
        if(IN_OUT == 0)//IN
        {
            *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x25180000;//fix des
        }
        else//OUT
        {
            *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x26180000;//fix src
        }
        *(&(FM3_DMAC->DMACSA0)+DMA_ch*0x04) = src;
        *(&(FM3_DMAC->DMACDA0)+DMA_ch*0x04) = des;
    }
    
    if(USB_EPnb == USB0_EP2)
    {
        FM3_INTREQ->DRQSEL = (0x00000001<<1);
        FM3_DMAC->DMACR = 0x80000000;
        *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) = ((0x90000000+(1<<23)) | halfwords);
        if(IN_OUT == 0)//IN
        {
            *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x25180000;//fix des
        }
        else//OUT
        {
            *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x26180000;//fix src
        }
        *(&(FM3_DMAC->DMACSA0)+DMA_ch*0x04) = src;
        *(&(FM3_DMAC->DMACDA0)+DMA_ch*0x04) = des;
    }
    
    if(USB_EPnb == USB0_EP3)
    {
        FM3_INTREQ->DRQSEL = (0x00000001<<2);
        FM3_DMAC->DMACR = 0x80000000;
        *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) = ((0x90000000+(2<<23)) | halfwords);
        if(IN_OUT == 0)//IN
        {
            *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x25180000;//fix des
        }
        else//OUT
        {
            *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x26180000;//fix src
        }
        *(&(FM3_DMAC->DMACSA0)+DMA_ch*0x04) = src;
        *(&(FM3_DMAC->DMACDA0)+DMA_ch*0x04) = des;
    }
    
    if(USB_EPnb == USB0_EP4)
    {
        FM3_INTREQ->DRQSEL = (0x00000001<<3);
        FM3_DMAC->DMACR = 0x80000000;
        *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) = ((0x90000000+(3<<23)) | halfwords);
        if(IN_OUT == 0)//IN
        {
            *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x25180000;//fix des
        }
        else//OUT
        {
            *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x26180000;//fix src
        }
        *(&(FM3_DMAC->DMACSA0)+DMA_ch*0x04) = src;
        *(&(FM3_DMAC->DMACDA0)+DMA_ch*0x04) = des;
    }
    
    if(USB_EPnb == USB0_EP5)
    {
        FM3_INTREQ->DRQSEL = (0x00000001<<4);
        FM3_DMAC->DMACR = 0x80000000;
        *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) = ((0x90000000+(4<<23)) | halfwords);
        if(IN_OUT == 0)//IN
        {
            *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x25180000;//fix des
        }
        else//OUT
        {
            *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) = 0x26180000;//fix src
        }
        *(&(FM3_DMAC->DMACSA0)+DMA_ch*0x04) = src;
        *(&(FM3_DMAC->DMACDA0)+DMA_ch*0x04) = des;
    }
    
    if(DMA_ReptMode==1)
    {
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) |= 0x00000001;//set EM
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) |= 0x00E00000;//set RC,RD,RS
        *(&(FM3_DMAC->DMACB0)+DMA_ch*0x04) &= 0xFFF7FFFF;//clear CI
    }
}
*/

/*!
 ******************************************************************************
 ** \brief DMA pause.
 **
 ** \param DMA_ch DMA channel.
 **        This parameter can be one of the following values:
 ** \arg   0: DMA channel 0
 ** \arg   1: DMA channel 1
 ** \arg   2: DMA channel 2
 ** \arg   3: DMA channel 3
 ** \arg   4: DMA channel 4
 ** \arg   5: DMA channel 5
 ** \arg   6: DMA channel 6
 ** \arg   7: DMA channel 7

 ** \retval None
 **
 ******************************************************************************
 */
void DMA_Pause(unsigned char DMA_ch)
{
    *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) |= 0x40000000;
}

/*!
 ******************************************************************************
 ** \brief DMA resume
 **
 ** \param DMA_ch DMA channel.
 **        This parameter can be one of the following values:
 ** \arg   0: DMA channel 0
 ** \arg   1: DMA channel 1
 ** \arg   2: DMA channel 2
 ** \arg   3: DMA channel 3
 ** \arg   4: DMA channel 4
 ** \arg   5: DMA channel 5
 ** \arg   6: DMA channel 6
 ** \arg   7: DMA channel 7

 ** \retval None
 **
 ******************************************************************************
 */
void DMA_Resume(unsigned char DMA_ch)
{
    *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) &= 0xBFFFFFFF;
}

/*!
 ******************************************************************************
 ** \brief DMA disable.
 **
 ** \param DMA_ch DMA channel.
 **        This parameter can be one of the following values:
 ** \arg   0: DMA channel 0
 ** \arg   1: DMA channel 1
 ** \arg   2: DMA channel 2
 ** \arg   3: DMA channel 3
 ** \arg   4: DMA channel 4
 ** \arg   5: DMA channel 5
 ** \arg   6: DMA channel 6
 ** \arg   7: DMA channel 7

 ** \retval None
 **
 ******************************************************************************
 */
void DMA_Disable(unsigned char DMA_ch)
{
    *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) &= 0x7FFFFFFF;
}

/*!
 ******************************************************************************
 ** \brief DMA enable.
 **
 ** \param DMA_ch DMA channel.
 **        This parameter can be one of the following values:
 ** \arg   0: DMA channel 0
 ** \arg   1: DMA channel 1
 ** \arg   2: DMA channel 2
 ** \arg   3: DMA channel 3
 ** \arg   4: DMA channel 4
 ** \arg   5: DMA channel 5
 ** \arg   6: DMA channel 6
 ** \arg   7: DMA channel 7

 ** \retval None
 **
 ******************************************************************************
 */
void DMA_Enable(unsigned char DMA_ch)
{
    *(&(FM3_DMAC->DMACA0)+DMA_ch*0x04) |= 0x80000000;
}
