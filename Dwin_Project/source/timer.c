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
#include "JDQProc.h"

/*0,1,2,3,4,5,6,7,8,9,A,b,C,d,E,F,*/
//const unsigned char LED_Code[] = { 0x09,0x5F,0x31,0x13,0x47,0x83,0x81,0x1F,0x01,0x03,0x05,0xC1,0xA9,0x51,0xA1,0xA5,0xF7 };// �������ʾ��0123456789abcdef,0.1.2.3.4.5.6.7.8.9.a.b.c.d.e.f.
const unsigned char LED_Code[] = { 0x90,0xFA,0x8C,0xC8,0xE2,0xC1,0x81,0xF8,0x80,0xC0,0xA0,0x83,0x95,0x8A,0x85,0xA5,0xEF };

static INT8U TimerBuf[4];
static INT8U Show_Code[4];

void show_ini(void)    
{
      GpioConfigEX( K_DAT, GPIO_MODE_OUTPUT, IO_PULLUP_ON );
      GpioSetOutputEX( K_DAT);
      GpioConfigEX( K_CLK, GPIO_MODE_OUTPUT, IO_PULLUP_ON );
      GpioClearOutputEX(K_CLK);
      GpioConfigEX( K_ST, GPIO_MODE_OUTPUT, IO_PULLUP_ON );
      GpioSetOutputEX( K_ST);
      GpioConfigEX( K_OE, GPIO_MODE_OUTPUT, IO_PULLUP_ON );
      GpioSetOutputEX( K_OE);
 //     GpioClearOutputEX(K_OE);   // ����ʾ
}


void xianshi(INT8U Dot)
{
  INT8U i;
  INT8U j;
  INT8U SCode;
  TimerBuf[0] = 0x00;
  TimerBuf[1] = 0x01;
  TimerBuf[2] = 0x02;
  TimerBuf[3] = 0x03;
  
  if(Dot==1)     // ȡ��С����Ķ��룬����������.,..............
  {
      Show_Code[3] = LED_Code[TimerBuf[0]];
      Show_Code[2] = LED_Code[TimerBuf[1]]&0x7f;
      Show_Code[1] = LED_Code[TimerBuf[2]]&0x7f;
      Show_Code[0] = LED_Code[TimerBuf[3]];
  }
  else
  {
      Show_Code[3] = LED_Code[TimerBuf[0]];
      Show_Code[2] = LED_Code[TimerBuf[1]];
      Show_Code[1] = LED_Code[TimerBuf[2]];
      Show_Code[0] = LED_Code[TimerBuf[3]];
  }
  
  for(i=0;i<4;i++)                      // ��λ�����
  {
      SCode = Show_Code[i];             // ȡ��ʾ����      
      for(j=0;j<8;j++)//������          // ÿλ�����8��,��8λ����
      {
          GpioClearOutputEX(K_CLK);
          Delayus( 20 );           
          if(TEST_BIT(SCode, j))    
          {
             GpioSetOutputEX(K_DAT);
          }
          else
          {
             GpioClearOutputEX(K_DAT);
          }
     //     GpioClearOutputEX(K_CLK);
          Delayus( 20 );
          GpioSetOutputEX(K_CLK);        //ʱ���ź�          
          Delayus( 20 );          
         
      }      
  }
  
    
    
    GpioClearOutputEX(K_ST);//�����ź�    
    Delayus( 10 );
    GpioSetOutputEX(K_ST); //�����ź�
    Delayus( 10 );
    GpioClearOutputEX(K_ST);//�����ź�   
    Delayus( 10 );
    GpioClearOutputEX(K_OE);   // ����ʾ
}

void timer_Ctrl(INT8U function)
{
  if(function==1)     // ���õ���ʱʱ��
  {
      
  }
  if(function==2)     // ��������ʱ
  {
  
  }
  if(function==3)     // ֹͣ����ʱ
  {
  
  }
  if(function==4)     // ��ѯ��ʱ��ʱ��
  {
  
  }    
  xianshi(1);          // ˢ�¼�ʱ��
}






























