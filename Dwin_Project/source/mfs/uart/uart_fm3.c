/*!
 ******************************************************************************
 **
 ** \file uart_fm3.c
 **
 ** \brief UART drivers
 **
 ** \author FSLA AE Team
 **
 ** \version V0.10
 **
 ** \date 2011-12-09
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
#include "uart_fm3.h"
/*---------------------------------------------------------------------------*/
/* Bit definition                                                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* local datatypes                                                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* local data                                                                */
/*---------------------------------------------------------------------------*/
static FM3_MFS03_UART_TypeDef* pMFS_UART_REG_07[8] = 
{
    (FM3_MFS03_UART_TypeDef*)(FM3_MFS0_UART),
    (FM3_MFS03_UART_TypeDef*)(FM3_MFS1_UART),
    (FM3_MFS03_UART_TypeDef*)(FM3_MFS2_UART),
    (FM3_MFS03_UART_TypeDef*)(FM3_MFS3_UART),
    (FM3_MFS03_UART_TypeDef*)(FM3_MFS4_UART),
    (FM3_MFS03_UART_TypeDef*)(FM3_MFS5_UART),
    (FM3_MFS03_UART_TypeDef*)(FM3_MFS6_UART),
    (FM3_MFS03_UART_TypeDef*)(FM3_MFS7_UART),
};
static FM3_MFS47_UART_TypeDef* pMFS_UART_REG_47[8] = 
{
    (FM3_MFS47_UART_TypeDef*)(0),
    (FM3_MFS47_UART_TypeDef*)(0),
    (FM3_MFS47_UART_TypeDef*)(0),
    (FM3_MFS47_UART_TypeDef*)(0),
    (FM3_MFS47_UART_TypeDef*)(FM3_MFS4_UART),
    (FM3_MFS47_UART_TypeDef*)(FM3_MFS5_UART),
    (FM3_MFS47_UART_TypeDef*)(FM3_MFS6_UART),
    (FM3_MFS47_UART_TypeDef*)(FM3_MFS7_UART),
};
/*---------------------------------------------------------------------------*/
/* local functions prototypes                                                */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* global data                                                               */
/*---------------------------------------------------------------------------*/
extern MFS_CallbackT g_pMFSIRQCallback[16];
/*---------------------------------------------------------------------------*/
/* global functions                                                          */
/*---------------------------------------------------------------------------*/

/*!
 ******************************************************************************
 ** \brief Config the UART baud rate
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **         
 ** \param baudrate
 ** \param unsigned int
 ** \arg   Baud rate value
 **         
 ** \retval None
 **
 ******************************************************************************
 */
uint16_t MFS_UART0ConfigBaudrate(uint8_t Ch, uint32_t baudrate)
{
    uint16_t tBGR;
    uint32_t tSysFrePCLK2;
    /* Check Cfg parameter */
    ASSERT(Ch <= UART0_Ch_MAX);
    tSysFrePCLK2 = SystemCoreClock/(1<<((FM3_CRG->APBC2_PSR&0x03)));
    tBGR = (uint16_t)(((tSysFrePCLK2 + (baudrate / 2)) / (uint32_t)baudrate) - 1);
    return tBGR;
}

/*!
 ******************************************************************************
 ** \brief Config the UART mode
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **         
 ** \param  Mode initialization data
 ** \param MFS_UART0ModeConfigT
 ** \arg   Mode info
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_UART0ConfigMode(uint8_t Ch, MFS_UART0ModeConfigT *pModeConfig)
{
    uint8_t tSMR = 0;
    uint8_t tESCR = 0;
    uint16_t tBGR = 0;
    /* Check Cfg parameter */
    ASSERT(Ch <= UART0_Ch_MAX);
    ASSERT((pModeConfig->Databits >= Uart0_DATABITS_5) && (pModeConfig->Databits <= Uart0_DATABITS_9));
    ASSERT((pModeConfig->Stopbits >= Uart0_STOPBITS_1) && (pModeConfig->Stopbits <= Uart0_STOPBITS_4));
    ASSERT(pModeConfig->Parity <= Uart0_PARITY_EVEN);
    ASSERT((pModeConfig->Bitorder == Uart0_BITORDER_MSB) || (pModeConfig->Bitorder == Uart0_BITORDER_LSB));
    ASSERT((pModeConfig->NRZ == Uart0_NRZ) || (pModeConfig->NRZ == Uart0_INV_NRZ));
    /* Config the MOD as UART0 */
    tSMR = tSMR | MFS_MODE_UART0;
    /* Enable Output */
    tSMR = tSMR | UART0_SMR_SOE;
    /* LSB or MSB */
    if(pModeConfig->Bitorder == Uart0_BITORDER_MSB)
    {
        tSMR = tSMR | UART0_SMR_BDS;
    }
    /* Stop bit */
    switch(pModeConfig->Stopbits)
    {
        case Uart0_STOPBITS_1:
            break;
        case Uart0_STOPBITS_2:
            tSMR = tSMR | UART0_SMR_SBL;
            break;
        case Uart0_STOPBITS_3:
            tESCR = tESCR | UART0_ESCR_ESBL;
            break;
        case Uart0_STOPBITS_4:
            tSMR = tSMR | UART0_SMR_SBL;
            tESCR = tESCR | UART0_ESCR_ESBL;
            break;
        default:
            break;
    }
    /* Parity */
    if(pModeConfig->Parity == Uart0_PARITY_NONE)
    {
    }
    else 
    {
        tESCR = tESCR | UART0_ESCR_PEN;
        if(pModeConfig->Parity == Uart0_PARITY_ODD)
        {
            tESCR = tESCR | UART0_ESCR_P;
        }
    }
    /* Data bits */
    switch(pModeConfig->Databits)
    {
        case Uart0_DATABITS_5:
            tESCR = tESCR | UART0_ESCR_BITLEN_5;
            break;
        case Uart0_DATABITS_6:
            tESCR = tESCR | UART0_ESCR_BITLEN_6;
            break;
        case Uart0_DATABITS_7:
            tESCR = tESCR | UART0_ESCR_BITLEN_7;
            break;
        case Uart0_DATABITS_8:
            tESCR = tESCR | UART0_ESCR_BITLEN_8;
            break;
        case Uart0_DATABITS_9:
            tESCR = tESCR | UART0_ESCR_BITLEN_9;
            break;
        default:
            break;
    }
    /* INV */
    if(pModeConfig->NRZ == Uart0_NRZ)
    {
    }
    else
    {
        tESCR = tESCR | UART0_ESCR_INV;
    }
    tBGR = MFS_UART0ConfigBaudrate(Ch,pModeConfig->Baudrate);
    /* Config OK */
    pMFS_UART_REG_07[Ch]->SMR = tSMR;
    pMFS_UART_REG_07[Ch]->ESCR = tESCR;
    pMFS_UART_REG_07[Ch]->BGR = tBGR;
    return;
}
/*!
 ******************************************************************************
 ** \brief Config the UART ISR call back
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **         
 ** \param  Mode initialization data
 ** \param MFS_UART0ISRCallbackT
 ** \arg   Mode info
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_UART0ConfigISRCallback(uint8_t Ch, MFS_UART0ISRCallbackT *pCallback)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART0_Ch_MAX);
    ASSERT((pCallback->pISRRXCallback != NULL) &&
        (pCallback->pISRTXCallback != NULL));
    /* Init the callback */
    g_pMFSIRQCallback[Ch*2] = pCallback->pISRRXCallback;
    g_pMFSIRQCallback[Ch*2+1] = pCallback->pISRTXCallback;
    return;
}
/*!
 ******************************************************************************
 ** \brief Config the UART FIFO
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **         
 ** \param FIFO initialization data
 ** \param MFS_UART0FIFOConfigT
 ** \arg   FIFO config info
 **         
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_UART0ConfigFIFO(uint8_t Ch, MFS_UART0FIFOConfigT *pFIFOConfig)
{
    uint8_t tFCR1 = 0;
    uint8_t tFCR0 = 0;
    /* Check Cfg parameter */
    ASSERT((Ch <= UART0_Ch_MAX) && (Ch >= UART0_Ch_FIFOMIN));
    if(pFIFOConfig->FIFOSel == MFS_FIFO1TX_FIFO2RX)
    {
    }
    else
    {
        tFCR1 = tFCR1 | UART0_FCR1_FSEL;
    }
    tFCR0 = tFCR0 | UART0_FCR0_FCL2 | UART0_FCR0_FCL1 ;
    pMFS_UART_REG_47[Ch]->FCR1 = tFCR1;
    pMFS_UART_REG_47[Ch]->FCR1 = tFCR1 | UART0_FCR1_FRIIE;
	pMFS_UART_REG_47[Ch]->FCR0 = 0;
    pMFS_UART_REG_47[Ch]->FCR0 = tFCR0 | UART0_FCR0_FE1 | UART0_FCR0_FE2;
    pMFS_UART_REG_47[Ch]->FBYTE1 = pFIFOConfig->Bytecount1;
    pMFS_UART_REG_47[Ch]->FBYTE2 = pFIFOConfig->Bytecount2;
    
    return;
}

/*!
 ******************************************************************************
 ** \brief Reset the FDRQ
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_UART0ResetFDRQ(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART0_Ch_MAX);
    pMFS_UART_REG_47[Ch]->FCR1 = pMFS_UART_REG_47[Ch]->FCR1 &
        (~UART0_FCR1_FDRQ);
    return;
}
/*!
 ******************************************************************************
 ** \brief Set the FDRQ to request for the transmit FIFO data
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_UART0SetFDRQ(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART0_Ch_MAX);
    pMFS_UART_REG_47[Ch]->FCR1 = pMFS_UART_REG_47[Ch]->FCR1 | UART0_FCR1_FDRQ;
    return;
}
/*!
 ******************************************************************************
 ** \brief Transmit one data
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **         
 ** \param Data
 ** \arg   5 - 9 bits data
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_UART0TXOneData(uint8_t Ch, uint16_t Data)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART0_Ch_MAX);
    pMFS_UART_REG_07[Ch]->TDR = Data;
    return;
}

/*!
 ******************************************************************************
 ** \brief Receive one data
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \retval Data
 **
 ******************************************************************************
 */
uint16_t MFS_UART0RXOneData(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART0_Ch_MAX);
    return (pMFS_UART_REG_07[Ch]->RDR & 0x01FF);
}

/*!
 ******************************************************************************
 ** \brief Enable tranmit function
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_UART0TXEnable(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART0_Ch_MAX);
    pMFS_UART_REG_07[Ch]->SCR = pMFS_UART_REG_07[Ch]->SCR | UART0_SCR_TXE;
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable receive function
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_UART0RXEnable(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART0_Ch_MAX);
    pMFS_UART_REG_07[Ch]->SCR = pMFS_UART_REG_07[Ch]->SCR | UART0_SCR_RXE;
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable FIFO
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \param nfifo (FE1 or FE2)
 ** \arg   1 - 2
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_UART0FIFOEnable(uint8_t Ch, uint8_t nfifo)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= UART0_Ch_MAX) && (Ch >= UART0_Ch_FIFOMIN));
    ASSERT((nfifo == UART0_FCR0_FE1) || (nfifo == UART0_FCR0_FE2));
    pMFS_UART_REG_47[Ch]->FCR0 = pMFS_UART_REG_47[Ch]->FCR0 | nfifo;
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable transmit
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_UART0TXDisable(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART0_Ch_MAX);
    pMFS_UART_REG_07[Ch]->SCR = pMFS_UART_REG_07[Ch]->SCR & (~UART0_SCR_TXE);
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable receive function
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_UART0RXDisable(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART0_Ch_MAX);
    pMFS_UART_REG_07[Ch]->SCR = pMFS_UART_REG_07[Ch]->SCR & (~UART0_SCR_RXE);
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable FIFO
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \param nfifo (FE1 or FE2)
 ** \arg   1 - 2
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_UART0FIFODisable(uint8_t Ch, uint8_t nfifo)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= UART0_Ch_MAX) && (Ch >= UART0_Ch_FIFOMIN));
    ASSERT((nfifo == UART0_FCR0_FE1) || (nfifo == UART0_FCR0_FE2));
    pMFS_UART_REG_47[Ch]->FCR0 = pMFS_UART_REG_47[Ch]->FCR0 & (~nfifo);
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable TX Bus idle interrupt
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_UART0IntTXBusIdleEnable(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART0_Ch_MAX);
    pMFS_UART_REG_07[Ch]->SCR = pMFS_UART_REG_07[Ch]->SCR | UART0_SCR_TBIE;
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable TX interrupt
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_UART0IntTXEnable(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART0_Ch_MAX);
    pMFS_UART_REG_07[Ch]->SCR = pMFS_UART_REG_07[Ch]->SCR | UART0_SCR_TIE;
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable TX FIFO Empty interrupt
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_UART0IntTXFIFOEmptyEnable(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= UART0_Ch_MAX) && (Ch >= UART0_Ch_FIFOMIN));
    pMFS_UART_REG_47[Ch]->FCR1 = pMFS_UART_REG_47[Ch]->FCR1 | UART0_FCR1_FTIE;
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable RX interrupt
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_UART0IntRXEnable(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART0_Ch_MAX);
    pMFS_UART_REG_07[Ch]->SCR = pMFS_UART_REG_07[Ch]->SCR | UART0_SCR_RIE;
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable TX Bus idle interrupt
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_UART0IntTXBusIdleDisable(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART0_Ch_MAX);
    pMFS_UART_REG_07[Ch]->SCR = pMFS_UART_REG_07[Ch]->SCR & (~UART0_SCR_TBIE);
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable TX interrupt
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_UART0IntTXDisable(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART0_Ch_MAX);
    pMFS_UART_REG_07[Ch]->SCR = pMFS_UART_REG_07[Ch]->SCR & (~UART0_SCR_TIE);
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable TX FIFO Empty interrupt
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_UART0IntTXFIFOEmptyDisable(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= UART0_Ch_MAX) && (Ch >= UART0_Ch_FIFOMIN));
    pMFS_UART_REG_47[Ch]->FCR1 = pMFS_UART_REG_47[Ch]->FCR1 &
        (~UART0_FCR1_FTIE);
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable RX interrupt
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_UART0IntRXDisable(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART0_Ch_MAX);
    pMFS_UART_REG_07[Ch]->SCR = pMFS_UART_REG_07[Ch]->SCR & (~UART0_SCR_RIE);
    return;
}

/*!
 ******************************************************************************
 ** \brief Get Overrun Error flag
 **
 ** \param Ch Requested channel
 ** \param unsigned char
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \return Overrun error flag
 ** \retval SET
 ** \retval RESET
 **
 ******************************************************************************
 */
FlagStatusT MFS_UART0StatusGetOE(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART0_Ch_MAX);

    return ((FlagStatusT)((pMFS_UART_REG_07[Ch]->SSR &
        UART0_SSR_ORE) == UART0_SSR_ORE));
}

/*!
 ******************************************************************************
 ** \brief Get Parity Error flag
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \return Parity error flag
 ** \retval SET
 ** \retval RESET
 **
 ******************************************************************************
 */
FlagStatusT MFS_UART0StatusGetPE(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART0_Ch_MAX);

    return ((FlagStatusT)((pMFS_UART_REG_07[Ch]->SSR  &
        UART0_SSR_PE) == UART0_SSR_PE));
}

/*!
 ******************************************************************************
 ** \brief Get Frame Error flag
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \return Frame error flag
 ** \retval SET
 ** \retval RESET
 **
 ******************************************************************************
 */
FlagStatusT MFS_UART0StatusGetFE(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART0_Ch_MAX);

    return ((FlagStatusT)((pMFS_UART_REG_07[Ch]->SSR &
        UART0_SSR_FRE) == UART0_SSR_FRE));
}

/*!
 ******************************************************************************
 ** \brief Get receive register full flag
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \return RDR Full flag
 ** \retval SET
 ** \retval RESET
 **
 ******************************************************************************
 */
FlagStatusT MFS_UART0StatusGetRXRegFull(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART0_Ch_MAX);

    return ((FlagStatusT)((pMFS_UART_REG_07[Ch]->SSR &
        UART0_SSR_RDRF) == UART0_SSR_RDRF));
}

/*!
 ******************************************************************************
 ** \brief Get transmit register empty flag
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \return TDR Empty flag
 ** \retval SET
 ** \retval RESET
 **
 ******************************************************************************
 */
FlagStatusT MFS_UART0StatusGetTXRegEmpty(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART0_Ch_MAX);

    return ((FlagStatusT)((pMFS_UART_REG_07[Ch]->SSR &
        UART0_SSR_TDRE) == UART0_SSR_TDRE));
}

/*!
 ******************************************************************************
 ** \brief Get transmit Bus idle flag
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \return TX Bus idle flag
 ** \retval SET
 ** \retval RESET
 **
 ******************************************************************************
 */
FlagStatusT MFS_UART0StatusGetTXBusIdle(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART0_Ch_MAX);

    return ((FlagStatusT)((pMFS_UART_REG_07[Ch]->SSR &
        UART0_SSR_TBI) == UART0_SSR_TBI));
}

/*!
 ******************************************************************************
 ** \brief Clear the error flag
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \retval none
 **
 ******************************************************************************
 */
void MFS_UART0ErrorClr(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART0_Ch_MAX);
    pMFS_UART_REG_07[Ch]->SSR = pMFS_UART_REG_07[Ch]->SSR | UART0_SSR_REC;
    return;
}

/*****************************************************************************/

/* END OF FILE */
