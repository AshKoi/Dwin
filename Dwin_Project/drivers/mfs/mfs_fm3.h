/*!
 ******************************************************************************
 **
 ** \file mfs_fm3.h
 **
 ** \brief MFS drivers head file
 **
 ** \author FSLA AE Team
 **
 ** \version V0.10
 **
 ** \date 2011-12-26
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
 
#ifndef _MFS_FM3_H_
#define _MFS_FM3_H_


/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "fm3_common.h"
/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/

/*! \brief Transmit FIFO:FIFO1; Receive FIFO:FIFO2 */
#define MFS_FIFO1TX_FIFO2RX  0x00
/*! \brief Transmit FIFO:FIFO2; Receive FIFO:FIFO1 */
#define MFS_FIFO2TX_FIFO1RX  0x01


/*---------------------------------------------------------------------------*/
/* types, enums and structures                                               */
/*---------------------------------------------------------------------------*/

/*!
******************************************************************************
** \brief MFS callback.
******************************************************************************
*/
typedef void (*MFS_CallbackT) (void);

/*!
******************************************************************************
**
** \brief ISR number.
**
******************************************************************************
*/
typedef enum 
{
    MFS_MFS_IntCh0RX = 0, 
    MFS_MFS_IntCh0TX, 
    MFS_MFS_IntCh1RX, 
    MFS_MFS_IntCh1TX, 
    MFS_MFS_IntCh2RX, 
    MFS_MFS_IntCh2TX, 
    MFS_MFS_IntCh3RX, 
    MFS_MFS_IntCh3TX, 
    MFS_MFS_IntCh4RX, 
    MFS_MFS_IntCh4TX, 
    MFS_MFS_IntCh5RX, 
    MFS_MFS_IntCh5TX, 
    MFS_MFS_IntCh6RX, 
    MFS_MFS_IntCh6TX, 
    MFS_MFS_IntCh7RX, 
    MFS_MFS_IntCh7TX, 
} MFS_IntNumT;
/*!
******************************************************************************
**
** \brief MFS Channel number.
**
******************************************************************************
*/
typedef enum 
{
   MFS_Ch0 = 0, 
    MFS_Ch1,
    MFS_Ch2,
    MFS_Ch3,
    MFS_Ch4,
    MFS_Ch5,
    MFS_Ch6,
    MFS_Ch7,
} MFS_ChannelNumT;
/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* _mfs_FM3_H_ */
/*****************************************************************************/
/* END OF FILE */
