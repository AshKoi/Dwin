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
//const unsigned char LED_Code[] = { 0x09,0x5F,0x31,0x13,0x47,0x83,0x81,0x1F,0x01,0x03,0x05,0xC1,0xA9,0x51,0xA1,0xA5,0xF7 };// 数码管显示码0123456789abcdef,0.1.2.3.4.5.6.7.8.9.a.b.c.d.e.f.
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
 //     GpioClearOutputEX(K_OE);   // 开显示
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
  
  if(Dot==1)     // 取代小数点的段码，控制秒闪点.,..............
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
  
  for(i=0;i<4;i++)                      // 四位数码管
  {
      SCode = Show_Code[i];             // 取显示段码      
      for(j=0;j<8;j++)//送数据          // 每位数码管8段,送8位数据
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
          GpioSetOutputEX(K_CLK);        //时钟信号          
          Delayus( 20 );          
         
      }      
  }
  
    
    
    GpioClearOutputEX(K_ST);//锁存信号    
    Delayus( 10 );
    GpioSetOutputEX(K_ST); //锁存信号
    Delayus( 10 );
    GpioClearOutputEX(K_ST);//锁存信号   
    Delayus( 10 );
    GpioClearOutputEX(K_OE);   // 开显示
}

void timer_Ctrl(INT8U function)
{
  if(function==1)     // 设置到计时时间
  {
      
  }
  if(function==2)     // 启动倒计时
  {
  
  }
  if(function==3)     // 停止倒计时
  {
  
  }
  if(function==4)     // 查询计时器时钟
  {
  
  }    
  xianshi(1);          // 刷新计时器
}






























