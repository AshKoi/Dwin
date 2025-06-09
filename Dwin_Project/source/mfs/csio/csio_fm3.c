/*!
 ******************************************************************************
 **
 ** \file csio_fm3.c	
 **
 ** \brief CSIO drivers
 **
 ** \author FSLA AE Team
 **
 ** \version V0.20
 **
 ** \date 2012-06-07
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
#include "csio_fm3.h"
#include "global.h"
/*---------------------------------------------------------------------------*/
/* Bit definition                                                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* local datatypes                                                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* local data                                                                */
/*---------------------------------------------------------------------------*/
static FM3_MFS03_CSIO_TypeDef* pMFS_CSIO_REG_07[8] = 
{
    (FM3_MFS03_CSIO_TypeDef*)(FM3_MFS0_CSIO),
    (FM3_MFS03_CSIO_TypeDef*)(FM3_MFS1_CSIO),
    (FM3_MFS03_CSIO_TypeDef*)(FM3_MFS2_CSIO),
    (FM3_MFS03_CSIO_TypeDef*)(FM3_MFS3_CSIO),
    (FM3_MFS03_CSIO_TypeDef*)(FM3_MFS4_CSIO),
    (FM3_MFS03_CSIO_TypeDef*)(FM3_MFS5_CSIO),
    (FM3_MFS03_CSIO_TypeDef*)(FM3_MFS6_CSIO),
    (FM3_MFS03_CSIO_TypeDef*)(FM3_MFS7_CSIO),
};
static FM3_MFS47_CSIO_TypeDef* pMFS_CSIO_REG_47[8] = 
{
    (FM3_MFS47_CSIO_TypeDef*)(0),
    (FM3_MFS47_CSIO_TypeDef*)(0),
    (FM3_MFS47_CSIO_TypeDef*)(0),
    (FM3_MFS47_CSIO_TypeDef*)(0),
    (FM3_MFS47_CSIO_TypeDef*)(FM3_MFS4_CSIO),
    (FM3_MFS47_CSIO_TypeDef*)(FM3_MFS5_CSIO),
    (FM3_MFS47_CSIO_TypeDef*)(FM3_MFS6_CSIO),
    (FM3_MFS47_CSIO_TypeDef*)(FM3_MFS7_CSIO),
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
 ** \brief Config the CSIO baud rate
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **     
 ** \param baudrate
 ** \arg   Baud rate value
 **         
 ** \return None
 **
 ******************************************************************************
 */
uint16_t MFS_CSIOConfigBaudrate(uint8_t Ch, uint32_t baudrate)
{
    uint16_t tBGR;
    uint32_t tSysFrePCLK2;
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    tSysFrePCLK2 = SystemCoreClock/(1<<((FM3_CRG->APBC2_PSR&0x03)));
    tBGR = (uint16_t)(((tSysFrePCLK2 + (baudrate / 2)) / (uint32_t)baudrate) - 1);
    return tBGR;
}

/*!
 ******************************************************************************
 ** \brief Config the CSIO mode
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **         
 ** \param Mode initialization data
 ** \arg   Mode info
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOConfigMode(uint8_t Ch, MFS_CSIOModeConfigT *pModeConfig)
{
    uint8_t tSMR = 0;
    uint8_t tESCR = 0;
    uint16_t tBGR = 0;
    uint8_t tSCR = 0;
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    ASSERT((pModeConfig->Mode == MFS_CSIO_SIO) || (pModeConfig->Mode == MFS_CSIO_SPI));
    ASSERT((pModeConfig->Databits >= CSIO_DATABITS_5) && (pModeConfig->Databits <= CSIO_DATABITS_9));
    ASSERT(pModeConfig->Waitbits <= CSIO_WAITBITS_3);
    ASSERT((pModeConfig->Bitorder == CSIO_BITORDER_MSB) || (pModeConfig->Bitorder == CSIO_BITORDER_LSB));
    ASSERT((pModeConfig->Sckinv == CSIO_CLK) || (pModeConfig->Sckinv == CSIO_CLK_INV));
    /* Config the MOD as CSIO */
    tSMR = tSMR | MFS_MODE_CSIO;
    /* Config as normal or SPI mode */
    if(pModeConfig->Mode == MFS_CSIO_SPI)
    {
        tSCR = tSCR | CSIO_SCR_SPI;
    }
    tSCR = tSCR | CSIO_SCR_TXE;
    /* config the Master/Slave mode */
    if(pModeConfig->MasterSalve == CSIO_SLAVE)
    {
        tSCR = tSCR | CSIO_SCR_MS;
    }
    else
    {
        /* Enable the clock */
        tSMR = tSMR | CSIO_SMR_SCKE;
    }
    /* Set Baudrate */
    tBGR = MFS_CSIOConfigBaudrate(Ch,pModeConfig->Baudrate);
    /* LSB or MSB */
    if(pModeConfig->Bitorder == CSIO_BITORDER_MSB)
    {
        tSMR = tSMR | CSIO_SMR_BDS;
    }
    /* Wait bit */
    switch(pModeConfig->Waitbits)
    {
        case CSIO_WAITBITS_0:
            break;
        case CSIO_WAITBITS_1:
            tESCR = tESCR | CSIO_ESCR_WT0;
            break;
        case CSIO_WAITBITS_2:
            tESCR = tESCR | CSIO_ESCR_WT1;
            break;
        case CSIO_WAITBITS_3:
            tESCR = tESCR | CSIO_ESCR_WT0;
            tESCR = tESCR | CSIO_ESCR_WT1;
            break;
        default:
            break;
    }
    /* Data bits */
    switch(pModeConfig->Databits)
    {
        case CSIO_DATABITS_8:
          tESCR = tESCR | CSIO_ESCR_BITLEN_8;
          break;
        case CSIO_DATABITS_5:
          tESCR = tESCR | CSIO_ESCR_BITLEN_5;
          break;
        case CSIO_DATABITS_6:
          tESCR = tESCR | CSIO_ESCR_BITLEN_6;
          break;
        case CSIO_DATABITS_7:
          tESCR = tESCR | CSIO_ESCR_BITLEN_7;
          break;
        case CSIO_DATABITS_9:
          tESCR = tESCR | CSIO_ESCR_BITLEN_9;
          break;
        default:
          break;
    }
    /* INV */
    if(pModeConfig->Sckinv == CSIO_CLK)
    {
    }
    else
    {
        tSMR = tSMR | CSIO_SMR_SCINV;
//        if(pModeConfig->MasterSalve == CSIO_SLAVE)
        {          
//            tSCR = tSCR | CSIO_SCR_RXE;
        }
    }
    /* Config OK */
    pMFS_CSIO_REG_07[Ch]->SCR = pMFS_CSIO_REG_07[Ch]->SCR | CSIO_SCR_UPCL;
    pMFS_CSIO_REG_07[Ch]->SMR = tSMR;
    pMFS_CSIO_REG_07[Ch]->SCR = tSCR;
    pMFS_CSIO_REG_07[Ch]->ESCR = tESCR;
    pMFS_CSIO_REG_07[Ch]->BGR = tBGR;
    pMFS_CSIO_REG_07[Ch]->SSR = pMFS_CSIO_REG_07[Ch]->SSR | 0x80;
    
    if((pModeConfig->MasterSalve == CSIO_SLAVE) && (pModeConfig->Sckinv == CSIO_CLK_INV))
    {
//        tSCR = pMFS_CSIO_REG_07[Ch]->SCR | (CSIO_SCR_RIE | CSIO_SCR_RXE);
    }

    return;
}

/*!
 ******************************************************************************
 ** \brief Config the CSIO FIFO
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **         
 ** \param FIFO initialization data
 ** \arg   FIFO config info
 **         
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOConfigFIFO(uint8_t Ch, MFS_CSIOFIFOConfigT *pFIFOConfig)
{
    uint8_t tFCR1 = 0;
    uint8_t tFCR0 = 0;
    /* Check Cfg parameter */
    ASSERT((Ch <= CSIO_Ch_MAX) && (Ch >= CSIO_Ch_FIFOMIN));
    if(pFIFOConfig->FIFOSel == MFS_FIFO1TX_FIFO2RX)
    {
    }
    else
    {
        tFCR1 = tFCR1 | CSIO_FCR1_FSEL;
    }
    tFCR0 = tFCR0 | CSIO_FCR0_FCL2 | CSIO_FCR0_FCL1 ;

    pMFS_CSIO_REG_47[Ch]->FCR1 = tFCR1;
    pMFS_CSIO_REG_47[Ch]->FCR0 = tFCR0 | CSIO_FCR0_FE1 | CSIO_FCR0_FE2;
    pMFS_CSIO_REG_47[Ch]->FBYTE1 = pFIFOConfig->Bytecount1;
    pMFS_CSIO_REG_47[Ch]->FBYTE2 = pFIFOConfig->Bytecount2;
    
    return;
}
/*!
 ******************************************************************************
 ** \brief Config the CSIO ISR call back
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **         
 ** \param  Mode initialization data
 ** \arg   Mode info
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOConfigISRCallback(uint8_t Ch, MFS_CSIOISRCallbackT *pCallback)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    ASSERT((pCallback->pISRRXCallback != NULL) &&
        (pCallback->pISRTXCallback != NULL));
    /* Init the callback */
    g_pMFSIRQCallback[Ch*2] = pCallback->pISRRXCallback;
    g_pMFSIRQCallback[Ch*2+1] = pCallback->pISRTXCallback;
    return;
}
/*!
 ******************************************************************************
 ** \brief Reset the FDRQ
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOResetFDRQ(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    pMFS_CSIO_REG_47[Ch]->FCR1 = pMFS_CSIO_REG_47[Ch]->FCR1 &
        (~CSIO_FCR1_FDRQ);
    return;
}
/*!
 ******************************************************************************
 ** \brief Set the FDRQ to request for the transmit FIFO data
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOSetFDRQ(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    pMFS_CSIO_REG_47[Ch]->FCR1 = pMFS_CSIO_REG_47[Ch]->FCR1 | CSIO_FCR1_FDRQ;
    return;
}
/*!
 ******************************************************************************
 ** \brief Enable Serial Ouput
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOSOEEnable(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    pMFS_CSIO_REG_07[Ch]->SMR = pMFS_CSIO_REG_07[Ch]->SMR  | CSIO_SMR_SOE;
    return;
}
/*!
 ******************************************************************************
 ** \brief Disable Serial Output
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOSOEDisable(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    pMFS_CSIO_REG_07[Ch]->SMR = pMFS_CSIO_REG_07[Ch]->SMR & (~CSIO_SMR_SOE);
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
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOTXOneData(uint8_t Ch, uint16_t Data)
{   
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    pMFS_CSIO_REG_07[Ch]->TDR = Data;
    return;
}

/*!
 ******************************************************************************
 ** \brief Receive one data
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \return Data
 **
 ******************************************************************************
 */
uint16_t MFS_CSIORXOneData(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    return (pMFS_CSIO_REG_07[Ch]->RDR & 0x01FF);
}

/*!
 ******************************************************************************
 ** \brief Transfer one data
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **         
 ** \param Data
 ** \arg   5 - 9 bits data
 **
 ** \return None
 **
 ******************************************************************************
 */
uint16_t MFS_CSIOTransferOneData(uint8_t Ch, uint16_t Data)
{
    uint16_t read;
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);

    while(!MFS_CSIOStatusGetTXBusIdle(Ch));
    
    MFS_CSIORXEnable(Ch);
    MFS_CSIOTXEnable(Ch);
    
    MFS_CSIOTXOneData(Ch,Data);
    
    while(!MFS_CSIOStatusGetRXRegFull(Ch));
    
    read = MFS_CSIORXOneData(Ch);
    
    MFS_CSIORXDisable(Ch);
    
    return read;
}

/*!
 ******************************************************************************
 ** \brief Enable tranmit function
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOTXEnable(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    pMFS_CSIO_REG_07[Ch]->SCR = pMFS_CSIO_REG_07[Ch]->SCR | CSIO_SCR_TXE;
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable receive function
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIORXEnable(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    pMFS_CSIO_REG_07[Ch]->SCR = pMFS_CSIO_REG_07[Ch]->SCR | CSIO_SCR_RXE;
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
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOFIFOEnable(uint8_t Ch, uint8_t nfifo)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= CSIO_Ch_MAX) && (Ch >= CSIO_Ch_FIFOMIN));
    ASSERT((nfifo == CSIO_FCR0_FE1) || (nfifo == CSIO_FCR0_FE2));
    pMFS_CSIO_REG_47[Ch]->FCR0 = pMFS_CSIO_REG_47[Ch]->FCR0 | nfifo;
    return;
}
/*!
 ******************************************************************************
 ** \brief Disable transmit
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOTXDisable(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    pMFS_CSIO_REG_07[Ch]->SCR = pMFS_CSIO_REG_07[Ch]->SCR & (~CSIO_SCR_TXE);
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable receive function
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIORXDisable(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    pMFS_CSIO_REG_07[Ch]->SCR = pMFS_CSIO_REG_07[Ch]->SCR & (~CSIO_SCR_RXE);
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
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOFIFODisable(uint8_t Ch, uint8_t nfifo)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= CSIO_Ch_MAX) && (Ch >= CSIO_Ch_FIFOMIN));
    ASSERT((nfifo == CSIO_FCR0_FE1) || (nfifo == CSIO_FCR0_FE2));
    pMFS_CSIO_REG_47[Ch]->FCR0 = pMFS_CSIO_REG_47[Ch]->FCR0 & (~nfifo);
    return;
}
/*!
 ******************************************************************************
 ** \brief Enable TX Bus idle interrupt
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOIntTXBusIdleEnable(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    pMFS_CSIO_REG_07[Ch]->SCR = pMFS_CSIO_REG_07[Ch]->SCR | CSIO_SCR_TBIE;
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable transmit interrupt
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOIntTXEnable(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    pMFS_CSIO_REG_07[Ch]->SCR = pMFS_CSIO_REG_07[Ch]->SCR | CSIO_SCR_TIE;
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable TX FIFO Empty interrupt
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOIntTXFIFOEmptyEnable(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= CSIO_Ch_MAX) && (Ch >= CSIO_Ch_FIFOMIN));
    pMFS_CSIO_REG_47[Ch]->FCR1 = pMFS_CSIO_REG_47[Ch]->FCR1 | CSIO_FCR1_FTIE;
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable receive interrupt
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOIntRXEnable(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    pMFS_CSIO_REG_07[Ch]->SCR = pMFS_CSIO_REG_07[Ch]->SCR | CSIO_SCR_RIE;
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable TX Bus idle interrupt
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOIntTXBusIdleDisable(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    pMFS_CSIO_REG_07[Ch]->SCR = pMFS_CSIO_REG_07[Ch]->SCR & (~CSIO_SCR_TBIE);
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable transmit interrupt
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOIntTXDisable(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    pMFS_CSIO_REG_07[Ch]->SCR = pMFS_CSIO_REG_07[Ch]->SCR & (~CSIO_SCR_TIE);
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable TX FIFO Empty interrupt
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOIntTXFIFOEmptyDisable(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= CSIO_Ch_MAX) && (Ch >= CSIO_Ch_FIFOMIN));
    pMFS_CSIO_REG_47[Ch]->FCR1 = pMFS_CSIO_REG_47[Ch]->FCR1 & (~CSIO_FCR1_FTIE);
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable receive interrupt
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOIntRXDisable(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    pMFS_CSIO_REG_07[Ch]->SCR = pMFS_CSIO_REG_07[Ch]->SCR & (~CSIO_SCR_RIE);
    return;
}

/*!
 ******************************************************************************
 ** \brief Get Overrun Error flag
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \return Overrun error flag
 ** \retval SET
 ** \retval RESET
 **
 ******************************************************************************
 */
FlagStatusT MFS_CSIOStatusGetOE(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);

    return ((FlagStatusT)((pMFS_CSIO_REG_07[Ch]->SSR &
        CSIO_SSR_ORE) == CSIO_SSR_ORE));
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
FlagStatusT MFS_CSIOStatusGetRXRegFull(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);

    return ((FlagStatusT)((pMFS_CSIO_REG_07[Ch]->SSR &
        CSIO_SSR_RDRF) == CSIO_SSR_RDRF));
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
FlagStatusT MFS_CSIOStatusGetTXRegEmpty(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);

    return ((FlagStatusT)((pMFS_CSIO_REG_07[Ch]->SSR &
        CSIO_SSR_TDRE) == CSIO_SSR_TDRE));
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
FlagStatusT MFS_CSIOStatusGetTXBusIdle(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);

    return ((FlagStatusT)((pMFS_CSIO_REG_07[Ch]->SSR &
        CSIO_SSR_TBI) == CSIO_SSR_TBI));
}
/*!
 ******************************************************************************
 ** \brief Get FBYTE1
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch4-MFS_Ch7
 **
 ** \return FBYTE1
 ** \retval 0 - 127
 **
 ******************************************************************************
 */
uint8_t MFS_CSIOStatusFBYTE1(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);

    return (pMFS_CSIO_REG_47[Ch]->FBYTE1);
}
             /*!
 ******************************************************************************
 ** \brief Get FBYTE2
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch4-MFS_Ch7
 **
 ** \return FBYTE2
 ** \retval 0 - 127
 **
 ******************************************************************************
 */
uint8_t MFS_CSIOStatusFBYTE2(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);

    return (pMFS_CSIO_REG_47[Ch]->FBYTE2);
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
void MFS_CSIOErrorClr(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    pMFS_CSIO_REG_07[Ch]->SSR = pMFS_CSIO_REG_07[Ch]->SSR | CSIO_SSR_REC;
    return;
}
/*!
 ******************************************************************************
 ** \brief Programmable clear
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch7
 **
 ** \retval none
 **
 ******************************************************************************
 */
void MFS_CSIOProgramClr(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    pMFS_CSIO_REG_07[Ch]->SCR = pMFS_CSIO_REG_07[Ch]->SCR | CSIO_SCR_UPCL;
    return;
}

/*****************************************************************************/

/* END OF FILE */
#if 0
void Audiophile( void )
{
    dbgu_printf((INT8U*)"据说真的音乐发烧友连换个保险丝都能听得出来");
    dbgu_printf((INT8U*)"\n\r1、用火电的力度大点，声音偏暖；用水电的声底偏冷，但解析力很高。水电中，");
    dbgu_printf((INT8U*)"以葛州坝的电音色最好；火电中，以北仑电厂的电音质最好（因为烧的无烟煤的比例最高）");
    dbgu_printf((INT8U*)"\n\r2、同一套器材，晚上8点到10点的时候音色就感觉有点偏冷，晚上11点之后声");
    dbgu_printf((INT8U*)"音明显偏暖。 后来才发现高峰电用的是外省的水电，低谷电以本地火电为主，铁证如山！ ");
    dbgu_printf((INT8U*)"\n\r3、风力发的电层次感很差，听感朦胧，听菜可妇司机的A大调B小调，音场明显收缩，小提琴部都混在一起。");
    dbgu_printf((INT8U*)"\n\r4、风力发电的单机功率在500w以下的音色都偏薄。");
    dbgu_printf((INT8U*)"\n\r5、电是三相和两相和音色关系不大，关键是平衡感。三相电播放大编制的交响");
    dbgu_printf((INT8U*)"乐阵脚明显比两相的要稳！核电适合播放《终结者》之类的大片伴音。 ");
    dbgu_printf((INT8U*)"但遗憾水、火、风、核电都并在了国家电网上了，所以放什么声音都是混论一片。");
    dbgu_printf((INT8U*)"\n\r6、有位烧友为此从上海搬家到广州，声音好很多。 因为南方电网以大亚湾核电和");
    dbgu_printf((INT8U*)"两广水电为主，音响既有力度，又比较清澈。 最近准备投资建设独立不并网的雅鲁");
    dbgu_printf((INT8U*)"藏布江水电站，高山雪原的天水将使音响脱胎换骨，但可研报告遭到印度的反对。");
    dbgu_printf((INT8U*)"\n\r7、太阳能发电有何不同？声音偏向温暖,不冷,但是有点薄。 ");
    dbgu_printf((INT8U*)"\n\r8、不久前，我一位朋友，国内最资深的耳机发烧友，一日试听铁三角AT-HA25D耳放配AD2000耳机，");
    dbgu_printf((INT8U*)"听着听着突然摘下耳机说今天没法听了，一定是水电站的水位又涨了，令在场其他烧友目瞪口呆！");
    dbgu_printf((INT8U*)"第二天果然报道山洪爆发，小丰满水电站水库水位暴涨，达到1953年来的最高水位，要知道辽宁");
    dbgu_printf((INT8U*)"电网只有1％的电力来自小丰满！ 四座皆惊，以为神耳！");
}
#endif