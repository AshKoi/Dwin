/*!
 ******************************************************************************
 **
 ** \file io_fm3.c
 **
 ** \brief IO driver
 **
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

#include "config.h"
#include "global.h"
#include "JDQCtrl.h"
#include "Sound.h"
const unsigned char c_SoundBuf[] = {"[x0][m55]热烈欢迎。"};		
SOUND_BUF g_SoundBuf[SOUND_FRAME_BUF_SIZE] = {0};
INT8U SoundSendBuf[SOUND_FRAME_BUF_SIZE] = {0};
/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 初始化语音芯片接口
*********************************************************************************/
void SoundInit( INT8U port )
{
      GpioConfigEX( SOUND_RDY, GPIO_MODE_INPUT, IO_PULLUP_ON );   
      GpioConfigEX( SOUND_RST, GPIO_MODE_OUTPUT, IO_PULLUP_ON );
      GpioSetOutputEX( SOUND_RST);
      OSTimeDlyHMSM(0, 0,1, 200);
      GpioClearOutputEX( SOUND_RST);
      OSTimeDlyHMSM(0, 0,1, 200);
      GpioSetOutputEX( SOUND_RST);
      OSTimeDlyHMSM(0, 0,1, 200);
      OpenComm(COMM_S485, UART_BPS_9600, NO_PARITY, ONE_STOP_BIT, EIGHT_CHAR_BIT);       
      CommFlush( COMM_S485 );    
}
/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 初始化语音芯片接口
*********************************************************************************/
void SoundPlay(void)
{
    INT8U   i;
    INT8U   j;
    pSound_BUF pBuf = g_SoundBuf;    
    INT8U* psBuf = pBuf->pBuf;
    pBuf->FRAME_H = 0xfd;
    pBuf->nLen = sizeof(c_SoundBuf)+1 ;
    pBuf->nCtrl = 0x01;
    pBuf->nFormat = 0x00;
    memcpy( pBuf->pBuf, c_SoundBuf, sizeof(c_SoundBuf) );
    i = pBuf->nLen+3;
    i = 0;
    SoundSendBuf[i] = 0xfd;
    i++;
    SoundSendBuf[i] = (pBuf->nLen>>8);
    i++;
    SoundSendBuf[i] = (pBuf->nLen&0xff);
    i++;
    SoundSendBuf[i] = pBuf->nCtrl;
    i++;
    SoundSendBuf[i] = pBuf->nFormat;
    i++;
    j = 0;
    for(;j<(sizeof(c_SoundBuf));j++)
    {
        SoundSendBuf[i] = c_SoundBuf[j];
        i++;
    }
 //   memcpy( pBuf->pBuf, c_SoundBuf, sizeof(c_SoundBuf) );
    WriteComm(COMM_S485, SoundSendBuf, i);    
}
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  