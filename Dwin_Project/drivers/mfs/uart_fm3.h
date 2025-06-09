/*!
 ******************************************************************************
 **
 ** \file uart_fm3.h
 **
 ** \brief UART drivers head file
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
 
#ifndef _UART_FM3_H_
#define _UART_FM3_H_


/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "fm3_common.h"
#include "mfs_fm3.h"
/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/
/*! \brief MFS: UART0 mode */
#define MFS_MODE_UART0                   0

/*! \brief Databit Length: 5 */
#define Uart0_DATABITS_5                 5
/*! \brief Databit Length: 6 */
#define Uart0_DATABITS_6                 6
/*! \brief Databit Length: 7 */
#define Uart0_DATABITS_7                 7
/*! \brief Databit Length: 8 */
#define Uart0_DATABITS_8                 8
/*! \brief Databit Length: 9 */
#define Uart0_DATABITS_9                 9 

/*! \brief BIT Length switch case: 8 bit */
#define UART0_ESCR_BITLEN_8   0
/*! \brief BIT Length switch case: 5 bit */
#define UART0_ESCR_BITLEN_5   1
/*! \brief BIT Length switch case: 6 bit */
#define UART0_ESCR_BITLEN_6   2
/*! \brief BIT Length switch case: 7 bit */
#define UART0_ESCR_BITLEN_7   3
/*! \brief BIT Length switch case: 9 bit */
#define UART0_ESCR_BITLEN_9   4

/*! \brief Stopbit Length: 1 */
#define Uart0_STOPBITS_1                 1
/*! \brief Stopbit Length: 2 */
#define Uart0_STOPBITS_2                 2
/*! \brief Stopbit Length: 3 */
#define Uart0_STOPBITS_3                 3
/*! \brief Stopbit Length: 4 */
#define Uart0_STOPBITS_4                 4

/*! \brief Parity mode: None */
#define Uart0_PARITY_NONE                0
/*! \brief Parity mode: Odd */
#define Uart0_PARITY_ODD                 1
/*! \brief Parity mode: Even */
#define Uart0_PARITY_EVEN                2

/*! \brief Bit order: MSB */
#define Uart0_BITORDER_MSB               1
/*! \brief Bit order: LSB */
#define Uart0_BITORDER_LSB               0

/*! \brief Inverted serial data format: NRZ */
#define Uart0_NRZ               0
/*! \brief Inverted serial data format: Inverted NRZ */
#define Uart0_INV_NRZ               1

/*! \brief MFS UART Mininum FIFO Channel: 4 */
#define UART0_Ch_FIFOMIN                4
/*! \brief MFS UART0 Max Channel Number */
#define UART0_Ch_MAX                7

/*! \brief SCR: Programmable Clear bit */
#define UART0_SCR_UPCL   0x80
/*! \brief SCR: Receive interrupt enable bit */
#define UART0_SCR_RIE   0x10
/*! \brief SCR: Transmit interrupt enable bit */
#define UART0_SCR_TIE   0x08
/*! \brief SCR: Transmit bus idle interrupt enable bit */
#define UART0_SCR_TBIE   0x04
/*! \brief SCR: Receive operation enable bit */
#define UART0_SCR_RXE   0x02
/*! \brief SCR: Transmit operation enable bit */
#define UART0_SCR_TXE   0x01

/*! \brief SMR: Wake-up control bit */
#define UART0_SMR_WUCR 0x10
/*! \brief SMR: Stop bit length select bit */
#define UART0_SMR_SBL 0x08
/*! \brief SMR: Transfer direction select bit */
#define UART0_SMR_BDS 0x04
/*! \brief SMR: Serial data output enable bit */
#define UART0_SMR_SOE 0x01

/*! \brief SSR: Receive error flag clear bit */
#define UART0_SSR_REC   0x80
/*! \brief SSR: Parity error flag bit */
#define UART0_SSR_PE   0x20
/*! \brief SSR: Framing error flag bit */
#define UART0_SSR_FRE   0x10
/*! \brief SSR: Overrun error flag bit */
#define UART0_SSR_ORE   0x08
/*! \brief SSR: Receive data full flag */
#define UART0_SSR_RDRF   0x04
/*! \brief SSR: Transmit data empty flag bi */
#define UART0_SSR_TDRE   0x02
/*! \brief SSR: Transmit bus idle flag */
#define UART0_SSR_TBI   0x01

/*! \brief ESCR: Flow control enable bit */
#define UART0_ESCR_FLWEN 0x80
/*! \brief ESCR: Extension stop bit length select bit */
#define UART0_ESCR_ESBL 0x40
/*! \brief ESCR: Inverted serial data format bit */
#define UART0_ESCR_INV 0x020
/*! \brief ESCR: Parity enable bit */
#define UART0_ESCR_PEN 0x010
/*! \brief ESCR: Parity select bit */
#define UART0_ESCR_P 0x08

/*! \brief BGR1: External clock select bit */
#define UART0_BGR1_EXT 0x80

/*! \brief FCR1: Re-transmit data lost detect enable bit */
#define UART0_FCR1_FLSTE   0x10
/*! \brief FCR1: Receive FIFO idle detection enable bit */
#define UART0_FCR1_FRIIE   0x08
/*! \brief FCR1: Transmit FIFO data request bit */
#define UART0_FCR1_FDRQ   0x04
/*! \brief FCR1: Transmit FIFO interrupt enable bit */
#define UART0_FCR1_FTIE   0x02
/*! \brief FCR1: FIFO select bit */
#define UART0_FCR1_FSEL   0x01

/*! \brief FCR0: FIFO re-transmit data lost flag bit */
#define UART0_FCR0_FLST   0x40
/*! \brief FCR0: FIFO pointer reload bit */
#define UART0_FCR0_FLD   0x20
/*! \brief FCR0: FIFO pointer save bit */
#define UART0_FCR0_FSET   0x10
/*! \brief FCR0: FIFO2 reset bit */
#define UART0_FCR0_FCL2   0x08
/*! \brief FCR0: FIFO1 reset bit */
#define UART0_FCR0_FCL1   0x04
/*! \brief FCR0: FIFO2 operation enable bit */
#define UART0_FCR0_FE2   0x02
/*! \brief FCR0: FIFO1 operation enable bit */
#define UART0_FCR0_FE1   0x01

/*---------------------------------------------------------------------------*/
/* types, enums and structures                                               */
/*---------------------------------------------------------------------------*/
/*!
******************************************************************************
** \brief UART Mode initialization data structure
******************************************************************************
*/
typedef struct
{
    uint32_t Baudrate;  //!< Baudrate
    uint8_t Databits;   //!< Data bit length
    uint8_t Stopbits;   //!< Stop bit length
    uint8_t Parity;     //!< Parity
    uint8_t Bitorder;   //!< Bit order
    uint8_t NRZ;        //!< Signal NRZ
}MFS_UART0ModeConfigT;
/*!
******************************************************************************
** \brief UART FIFO mode initialization data structure
******************************************************************************
*/
typedef struct
{
    uint8_t FIFOSel;    //!< FIFO Selection
    uint8_t Bytecount1; //!< Byte count of FIFO1
    uint8_t Bytecount2; //!< Byte count of FIFO2
}MFS_UART0FIFOConfigT;
/*!
******************************************************************************
** \brief UART ISR Call back
******************************************************************************
*/
typedef struct
{
    MFS_CallbackT pISRRXCallback;//!< ISR RX call back function
    MFS_CallbackT pISRTXCallback;//!< ISR TX call back function
}MFS_UART0ISRCallbackT;

/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/* Config */
void MFS_UART0ConfigMode(uint8_t Ch, MFS_UART0ModeConfigT *pModeConfig);
void MFS_UART0ConfigISRCallback(uint8_t Ch, MFS_UART0ISRCallbackT *pCallback);
uint16_t MFS_UART0ConfigBaudrate(uint8_t Ch, uint32_t baudrate);
void MFS_UART0ConfigFIFO(uint8_t Ch, MFS_UART0FIFOConfigT *pFIFOConfig);
void MFS_UART0ResetFDRQ(uint8_t Ch);
void MFS_UART0SetFDRQ(uint8_t Ch);
/* RX/TX */
void MFS_UART0TXOneData(uint8_t Ch, uint16_t Data);
uint16_t MFS_UART0RXOneData(uint8_t Ch);
void MFS_UART0TXEnable(uint8_t Ch);
void MFS_UART0RXEnable(uint8_t Ch);
void MFS_UART0FIFOEnable(uint8_t Ch, uint8_t nfifo);
void MFS_UART0TXDisable(uint8_t Ch);
void MFS_UART0RXDisable(uint8_t Ch);
void MFS_UART0FIFODisable(uint8_t Ch, uint8_t nfifo);
/* Int */
void MFS_UART0IntTXBusIdleEnable(uint8_t Ch);
void MFS_UART0IntTXEnable(uint8_t Ch);
void MFS_UART0IntTXFIFOEmptyEnable(uint8_t Ch);
void MFS_UART0IntRXEnable(uint8_t Ch);
void MFS_UART0IntTXBusIdleDisable(uint8_t Ch);
void MFS_UART0IntTXDisable(uint8_t Ch);
void MFS_UART0IntTXFIFOEmptyDisable(uint8_t Ch);
void MFS_UART0IntRXDisable(uint8_t Ch);
/* Status */
FlagStatusT MFS_UART0GeStatusGetOE(uint8_t Ch);
FlagStatusT MFS_UART0StatusGetPE(uint8_t Ch);
FlagStatusT MFS_UART0StatusGetFE(uint8_t Ch);
FlagStatusT MFS_UART0StatusGetRXRegFull(uint8_t Ch);
FlagStatusT MFS_UART0StatusGetTXRegEmpty(uint8_t Ch);
FlagStatusT MFS_UART0StatusGetTXBusIdle(uint8_t Ch);
/* Error clear */
void MFS_UART0ErrorClr(uint8_t Ch);
#ifdef __cplusplus
}
#endif

#endif /* _UART_FM3_H_ */
/*****************************************************************************/
/* END OF FILE */
