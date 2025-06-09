/***************************************************************
****************************************************************/
#ifndef __KEY_H
#define __KEY_H	
#include "Os_cpu.h"
#include <stdio.h>
#include <string.h>
#include "global.h"
#include "config.h"
#include "board.h"
/**************************************************************************************************
                                             相关宏定义及结构枚举定义
***************************************************************************************************/
extern INT8U                g_nCmt;
extern INT32S               g_nKey_Flg;

#define AutoDisplayCmt               40  

#define KEY1_PFR                bFM3_GPIO_PFR1_P0
#define KEY1_DDR                bFM3_GPIO_DDR1_P0
#define KEY1_PDOR               bFM3_GPIO_PDOR1_P0
#define KEY1_PDIR               bFM3_GPIO_PDIR1_P0

#define KEY2_PFR                bFM3_GPIO_PFR1_P1
#define KEY2_DDR                bFM3_GPIO_DDR1_P1
#define KEY2_PDOR               bFM3_GPIO_PDOR1_P1
#define KEY2_PDIR               bFM3_GPIO_PDIR1_P1

#define KEY3_PFR                bFM3_GPIO_PFR1_P2
#define KEY3_DDR                bFM3_GPIO_DDR1_P2
#define KEY3_PDOR               bFM3_GPIO_PDOR1_P2
#define KEY3_PDIR               bFM3_GPIO_PDIR1_P2

#define KEY4_PFR                bFM3_GPIO_PFR1_P3
#define KEY4_DDR                bFM3_GPIO_DDR1_P3
#define KEY4_PDOR               bFM3_GPIO_PDOR1_P3
#define KEY4_PDIR               bFM3_GPIO_PDIR1_P3

#define KEY5_PFR                bFM3_GPIO_PFR1_P4
#define KEY5_DDR                bFM3_GPIO_DDR1_P4
#define KEY5_PDOR               bFM3_GPIO_PDOR1_P4
#define KEY5_PDIR               bFM3_GPIO_PDIR1_P4

#define KEY6_PFR                bFM3_GPIO_PFR1_P5
#define KEY6_DDR                bFM3_GPIO_DDR1_P5
#define KEY6_PDOR               bFM3_GPIO_PDOR1_P5
#define KEY6_PDIR               bFM3_GPIO_PDIR1_P5

#define KEY_INT_PFR                bFM3_GPIO_PFR1_P6
#define KEY_INT_DDR                bFM3_GPIO_DDR1_P6
#define KEY_INT_PDOR               bFM3_GPIO_PDOR1_P6
#define KEY_INT_PDIR               bFM3_GPIO_PDIR1_P6

#define KEY_POW_PFR                bFM3_GPIO_PFR1_P9
#define KEY_POW_DDR                bFM3_GPIO_DDR1_P9
#define KEY_POW_PDOR               bFM3_GPIO_PDOR1_P9
#define KEY_POW_PDIR               bFM3_GPIO_PDIR1_P9

#define FMQ_PFR                bFM3_GPIO_PFR7_P4
#define FMQ_DDR                bFM3_GPIO_DDR7_P4
#define FMQ_PDOR               bFM3_GPIO_PDOR7_P4
#define FMQ_PDIR               bFM3_GPIO_PDIR7_P4

#define KEY_LED_PDOR               bFM3_GPIO_PDOR1_P7


#define  KEY_NULL		0xFF

#define  KEY_UP			0xDF//0x0D
#define  KEY_DOWN		0xEF//0x12
#define  KEY_LEFT		0xF7//0x18
#define  KEY_RIGHT		0xFB//0x17
#define  KEY_ENTER		0xFE//0x02
#define  KEY_CANCEL	    0xFD//0x13


#define  KEY_FUN1		0x08    // 
#define  KEY_FUN2		0x03    //
#define  KEY_FUN3		0x0E    // 
#define  KEY_FUN4		0x04    //
#define  KEY_NUM0		0x16
#define  KEY_NUM1		0x0A
#define  KEY_NUM2		0x01
#define  KEY_NUM3		0x06
#define  KEY_NUM4		0x0F
#define  KEY_NUM5		0x0C
#define  KEY_NUM6		0x0B
#define  KEY_NUM7		0x14
#define  KEY_NUM8		0x11
#define  KEY_NUM9		0x10
#define  KEY_C		    0x07
#define  KEY_TAB		0x05
#define  KEY_FN		    0x1A
#define  KEY_SHIFT		0x1B   
#define  KEY_ALT		0x1C  
#define  KEY_SP		    0x15  
#define  KEY_POINT		0x19       

#define  KEY_BG                 0xBF
#define  KEY_JG                 0xCF

#define  KEY_ESC                 	0x03
#define  KEY_OPEN                 	0x02
#define  KEY_CLOSE                 	0x01


//键值采集
typedef struct _tagKEY_SAMP
{
    INT8U       nPressCnt;  //按键持续次数
    INT8U       nPressCntBak;  //按键持续次数
    INT8U       nKeyVal;    //键值
    INT8U       nKeyValBak;    //键值

	//INT16U       nPressCnt2;  //按键持续次数
    //INT8U		nKeyVal2;		//需要按键弹出才算
    //INT8U       nKeyVal2Bak;
}KEY_SAMP, *pKEY_SAMP;


void 	GetKeyValProc(void);
void 	GetKeyValProc0(void);
INT8U 	GetKeyVal(INT8U nType);
INT8U 	GetKeyVal2(void);

void    beep(void);
#endif
