/******************************************************************************************************
* Copyright (c) 2013
* 文件名:
* 创建人: 王一凡
* 日　期: 2013/09/12
* 修改人:
* 日　期:
* 描　述:
* 版　本: Version 1.0
*****************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include "Version.h"
#include "Base.h"
#include "ChkCrc.h"
#include "Table.h"
#include "SysRes.h"
#include "GuiwinCfg.h"
#include "Guiwin.h"
#include "Guimenu.h"
#include "Fun.h"
#include "linkcomExt.h"
#include "MemDrv.h"
#include "MemTbl.h"
#include "GB3761SetPrmExt.h"
#include "MemDrvExt.h"
#include "ChkCrc.h"
#include "MemData.h"
#include "DataItem.h"
#include "Gw3761.h"
#include "Lcd.h"
#include "Ac7022.h"
#include "SoftTimer.h"
#include "AppCfg.h"
#include "Key.h"
#include "Macro.h"
#include "rmotlink.h"
#include "CarrCol.h"
#include "Telit.h"
#include "Reset.h"
#include "Telit.h"
#include "meter645.h"
#include "SecTask.h"
#include "PowerOn.h"
#include "WiringProc.h"
#include "Dwin.h"
/**************************************************************************************************
                                           宏定义
***************************************************************************************************/
#define GET_KEY_VAL(nKey, nType)	nKey = GetKeyVal(nType)

#define A_MENU_LEN  30

#define DEC2BCD(x)	              ((((x)/10)<<4)+(x)%10)
#define BCD2DEC(x)	              (((x)&0x0F)+((x)>>4)*10)
//#if  TRIPHASE_METER
#define  LcdDisplayPage_NUM           82                  // 三相表的数据屏显数 
//#endif
//#if  SINGLEPHASE_METER
#define  LcdDisplayPage_NUM           22                // 单相表的数据屏显数
//#endif

/**************************************************************************************************
                                           变量定义
***************************************************************************************************/
//logo
const INT8U g_nLogo[] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xFF,0xFF,0xFF,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x0F,0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xFF,0xFF,0xFF,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xE0,0x00,0x00,0x00,
	0x00,0x00,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF8,0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0x00,0x00,0x00,0x00,0x3F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC0,0x00,0x00,0x0F,0xFE,0x3F,0xFF,0xFF,0xFF,0xF1,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x03,0xF8,0x0F,0xFF,0xFF,0xFF,0xC0,0x7F,0xFF,0xFF,0xFF,0xFF,
	0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xE0,0x03,0xFF,0xFF,0xFF,0x00,0x1F,0xFF,0xFF,0xFF,0xFF,0xFE,0x00,0x00,0x7F,
	0xE0,0x03,0xFF,0xFF,0xFF,0x00,0x1F,0xFF,0xFF,0xFF,0xFF,0xFF,0x80,0x01,0xFF,0xE0,0x03,0xFF,0xFF,0xFF,0x00,0x1F,0xFF,0xFF,0xFF,0xFF,0xFE,0x00,0x00,0x7F,0xE0,0x03,0xFF,0xFF,0xFF,0x00,0x1F,0xFF,0xFF,0xFF,0xFF,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xE0,0x03,0xFF,0xFF,0xFF,0x00,0x18,0x00,0x00,0x0F,0xFF,0xFC,0x00,0x0F,0xFF,0xE0,0x03,0xFF,0xFF,0xFF,0x00,0x1C,0x00,0x00,0x01,0xFF,0xFF,0xC0,0x03,0xFF,0xE0,0x03,0xFF,0xFF,0xFF,0x00,0x1C,0x00,
	0x00,0x00,0x7F,0xFF,0xE0,0x00,0xFF,0xE0,0x03,0xFF,0xFF,0xFF,0x00,0x1F,0x00,0x00,0x00,0x1F,0xFF,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x07,0xFF,0xE0,0x03,0xFF,0xFF,0xFF,0x00,0x1F,0xFF,0xFF,0x80,0x02,0xFF,0xE0,0x0F,0xFF,0xE0,0x03,0xFF,0xFF,0xFF,0x00,0x1F,0xFF,0xFF,0xE0,0x01,0xFF,0xF0,0x03,0xFF,0xE0,0x03,0xFF,0xFF,0xFF,0x00,0x1F,0xFF,0xFF,0xF0,0x00,0xFF,0xC0,0x01,0xFF,0xE0,0x01,0xFF,0xFF,0xFF,0x00,0x1F,0xFF,0xFF,0xF0,0x00,0xFF,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xF0,0x00,0x3F,0xFF,0xFF,0x00,0x1F,0xFF,0xFF,0xE0,0x00,0xFE,0x00,0x00,0xFF,0xFC,0x00,0x0F,0xFF,0xFF,
	0x00,0x1F,0xFF,0xFF,0xF0,0x01,0xFF,0x80,0x03,0xFF,0xFE,0x00,0x01,0xFF,0xFF,0x00,0x1F,0xFF,0xFF,0xE0,0x01,0xFF,0xE0,0x00,0x7F,0xFF,0x00,0x00,0x00,0x07,0x00,0x1F,0xFF,0xFF,0xC0,0x03,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x1F,0xFF,0xFC,0x00,0x0F,0x80,0x00,0x00,0x7F,0xFF,0xFE,0x00,0x00,0x0F,0x00,0x1F,0xFF,0xF8,0x00,0x1F,0xFF,0x00,0x00,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x1F,0xFF,0xE0,0x00,0x3F,0xFF,0x00,
	0x00,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x1F,0xFF,0xC0,0x00,0x7F,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
	0xFF,0xFF,0xFF,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xC0,0x00,0x00,0x00,0x3F,0xFF,0xC0,0x00,0x00,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xE0,0x00,0x00,0x07,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC0,0x00,0x00,0x00,0x00,0x3F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFC,0x00,0x00,0x00,0x00,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x3F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0x00,0x00,0x00,0x00,0x00,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x80,0x00,0x00,0x00,0x00,0x00,0x3F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xFF,0xFF,0xFF,0xE0,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xFF,0xFF,0xFF,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

// YIBANG POWER
const INT8U g_nAscii16[12][16] = {
	{0x00,0x00,0x66,0x66,0x66,0x66,0x3C,0x18,0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x3C,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00},
	{0x00,0x00,0xFC,0x66,0x66,0x66,0x7C,0x66,0x66,0x66,0x66,0xFC,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x10,0x38,0x6C,0xC6,0xC6,0xFE,0xC6,0xC6,0xC6,0xC6,0x00,0x00,0x00,0x00},
	{0x00,0x00,0xC6,0xE6,0xF6,0xFE,0xDE,0xCE,0xC6,0xC6,0xC6,0xC6,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x3C,0x66,0xC2,0xC0,0xC0,0xDE,0xC6,0xC6,0x66,0x3A,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0xFC,0x66,0x66,0x66,0x7C,0x60,0x60,0x60,0x60,0xF0,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x7C,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00},
	{0x00,0x00,0xC6,0xC6,0xC6,0xC6,0xD6,0xD6,0xD6,0xFE,0xEE,0x6C,0x00,0x00,0x00,0x00},
	{0x00,0x00,0xFE,0x66,0x62,0x68,0x78,0x68,0x60,0x62,0x66,0xFE,0x00,0x00,0x00,0x00},
	{0x00,0x00,0xFC,0x66,0x66,0x66,0x7C,0x6C,0x66,0x66,0x66,0xE6,0x00,0x00,0x00,0x00}
};	


const INT8S  	g_nUartArray[5]		= {COMM_IR, COMM_WH485, COMM_WH232, COMM_WH4852, COMM_ZB};
INT8S			g_nSynBaseUartMode 	= 0;         			// 通讯口,红外
/*
const INT32S 	g_nUartBpsArray[13] 	= {
									UART_BPS_300, 	UART_BPS_600, 	UART_BPS_900, 	UART_BPS_1200,
									UART_BPS_2400, 	UART_BPS_4800, 	UART_BPS_9600, 	UART_BPS_14400, 
									UART_BPS_19200, UART_BPS_28800, UART_BPS_38400, UART_BPS_57600,
									UART_BPS_115200 
								  };*/
const INT32S 	g_nUartBpsArray[13] 	= {
									UART_BPS_300, 	UART_BPS_600, 	UART_BPS_1200,UART_BPS_2400, 	
                                    UART_BPS_4800, UART_BPS_7200,	UART_BPS_9600,  UART_BPS_19200, 
                                    UART_BPS_38400, UART_BPS_57600,UART_BPS_115200 
								  };
INT8S			g_nSynBaseUartBPS 	= 3;          			// 波特率,1200

const INT8S 	g_nParityArray[3]		= {NO_PARITY, ODD_PARITY, EVEN_PARITY};
INT8S			g_nSynBaseParity 		= 2;           			// 奇偶校验,E

const INT8S 	g_nStopBitArray[] 		= {ONE_STOP_BIT, TWO_STOP_BIT};
INT8S			g_nSynBaseStopBit 	= 0;          			// 停止位,1

const INT8S 	g_nCharBitArray[] 		= {SEVEN_CHAR_BIT, EIGHT_CHAR_BIT};
INT8S 			g_nSynBaseCharBit 	= 1;          			// 数据位,8

const INT8S 	g_nCharDispFrm[]  		= {0, 1};
INT8S  			g_nShowHex				= 0;		        	// 报文显示

const INT8S   	g_nCtIRSelct[]      	= {1, 0};               //穿透时选择红外
INT8S   		g_nCtIr              	= 0;


INT8S   		g_nBeepFlg         	= 0;					//蜂鸣器
INT8S   		g_nContinueKeyFlg     = 0;					//连续按键开关关闭

INT8S   		g_nChargeKeyFlg = 0;					
				


INT8S 			g_nTimeOut[10] 		= {"3000"};				//超时时间


INT8S 			g_nUpGradeTimeOut[3] 	= {"10"};				//超时时间

const INT32S   	g_nLedBackLight[]   	= {600, 1200, 1800, 0};
INT8S 			g_LedBackLtCloseTime  	= 1;


const INT8S   	g_nLedBackArray[]   	= {3, 5, 10, 0};
INT8S 			g_LedBackCloseTime  	= 1; 					//背光自动关闭

const INT8S   	g_nTermCloseArray[] 	= {5, 15, 30,60, 0};
INT8S  			g_TermCloseTime     	= 4; 					//终端自动关机


// 
static INT8U	g_nLedBackCnt			= 0;					//背光计数
static INT8U	g_nTermDispCnt			= 0;					//自动关机计数

static INT8U 	g_nBgFlg 				= 0;					//背光关标志

#define LEN_REAL_SCREEN_NUM           14                      //实时数据的屏显数

#define LEN_DAY_SCREEN_NUM            3                       //日(月)数据的屏显数

INT8U   nSwitchFlg                    = 1;                      //电表参数读写和设置转换

extern  const MENU_ITEM     g_sZbMenu[];

METER_INFO           sMeterInfo;
pMETER_INFO	         pMeterInfo	                 = &sMeterInfo;

INT8U      GprsStep            = 0;           //GPRS执行步骤
static    INT8U      nCounter   = 0;
INT8U	    nStepInfo[7] 	     = {0};

extern    pKEY_SAMP	      g_pKeySamp;

INT8U                nSwMpFlg                     = 1;  //测量点查看菜单

OPERTOR_PRM			sOpertor;
pOPERTOR_PRM		pOpertor	= &sOpertor;

  
extern INT8U   g_MeterEventWrong[4];   
extern INT8U   g_MeterInsideWrong[2]    ;   
extern INT8U   g_MeterHardwareWrong[2]  ;
const  INT8S*	c_PartTypeC[]		={"[0]100A本地费控表",
                                    "[1]100A远程费控表",
                                    "[2]60A本地费控表",
                                    "[3]60A远程费控表",
                                    "[4]100A三相分时表",
                                    "[5]60A三相分时表",
                                    "[6]100A三相卡表",
                                    "[7]100A三相有功表",
                                    "[8]60A三相卡表",
                                    "[9]60A三相有功表",
                                    "[10]简易多功能",
                                    "[11]多功能电表",
                                    "[12]智能电能表",
										"未知电表",0};
const  INT8S*	c_PartTypeD[]		={"[0]80A本地费控表",
                                    "[1]80A远程费控表",
                                    "[2]60A本地费控表",
                                    "[3]60A远程费控表",
                                    "[4]80A单相分时表",
                                    "[5]60A单相分时表",
										"未知电表",0};
const  INT8S*	c_PartTypeE[]		={"[0]100A三相本地费控",
                                    "[1]100A三相远程费控",
                                    "[2]60A三相本地费控",
                                    "[3]60A三相远程费控",
                                    "[4]100A三相分时",
                                    "[5]60A三相分时表",
                                    "[6]100A三相卡表",
                                    "[7]100A三相有功表",
                                    "[8]60A三相卡表",
                                    "[9]60A三相有功表",
                                    "[10]80A单相本地费控",
                                    "[11]80A单相远程费控",
                                    "[12]60A单相本地费控",
                                    "[13]60A单相远程费控",
                                    "[14]80A单相分时表",
                                    "[15]60A单相分时表",0};
const  INT8S*	c_PartTypeF[]		={"[0]未知电表",
                                    "[1]Ⅰ类计量装置",
                                    "[2]Ⅱ类计量装置",
                                    "[3]Ⅰ/Ⅱ类计量装置",
                                    "[4]Ⅲ类计量装置",
                                    "[5]Ⅲ/Ⅳ类计量装置",0};


const  INT8S*	c_PartTypeAB[]		={"默认",0};

const  INT8S**   ArryCollect[] = {c_PartTypeC , c_PartTypeD ,c_PartTypeE ,c_PartTypeF ,c_PartTypeAB,0};
const  INT8S*    ArryName[]    = {"用户小类(C)","用户小类(D)","用户小类(E)","用户小类(F)","用户小类(AB)",0};


INT8U  Data0[35] = {   0xff,0xff,  0xff,0xff,  0xff,0xff,  0xff,0xff,  0xff,0xff,  0xff,0xff,  0xff,0xff,  0xff,0xff,  
                              0xff,0xff,0xff,0xff,   0xff,0xff,  0xff,0xff,0xff,0xff,  0xff,0xff,  0xff,0xff,0xff,0xff,  0xff,0xff,  0xff
                 };   // 初始化数组 

/**************************************************************************************************
                                        参数存储偏移信息
***************************************************************************************************/
#if 0
//6.3 系统时钟
const MENU_ITEM  g_sSysTimeMenu[] =
{
    {MENU_ITEM_DISCRIB, "系统时钟", NULL}, 
    
    {0}
};


//6.1 通讯参数
const MENU_ITEM  g_sCommInfoCfg[] =
{
    {MENU_ITEM_DISCRIB, "通讯参数", NULL}, 
  	{MENU_ITEM_EXE,"1.通信基本配置", (void*)BaseCommInfoCfg},
    {0}
};


//6.本机配置
const MENU_ITEM  g_sSysParmMenu[] =
{
    {MENU_ITEM_DISCRIB, "本机配置", NULL}, 
    //{MENU_ITEM_SHOW,"1.通讯参数配置", (void*)g_sCommInfoCfg},
    {MENU_ITEM_EXE, "2.系统参数配置", (void*)OtherParConfig},
   // {MENU_ITEM_SHOW,"3.系统时钟", (void*)g_sSysTimeMenu},
    {MENU_ITEM_EXE, "4.版本信息", (void*)VersionInfo},

	//{MENU_ITEM_EXE, "6.配置选择", (void*)CfgSelectProc},

    {0}
};
#endif

const MENU_ITEM  FacUnitTestMenu[] =
{
    {MENU_ITEM_DISCRIB, "生产模块检测", NULL},
	
    {MENU_ITEM_EXE, "1.故障板测试", (void*)StratCheck},
    {MENU_ITEM_EXE, "2.GPRS测试", (void*)GprsTest},
    {MENU_ITEM_EXE, "3.控制板测试", (void*)CtrlProcTest},

    {0}
};


const MENU_ITEM  g_sInTestMenu[] =
{
    {MENU_ITEM_DISCRIB, "内部调试", NULL},
	
    {MENU_ITEM_EXE, "1.交采信息", (void*)AcInfo},
    //{MENU_ITEM_EXE, "2.GPRS监测开关", (void*)GprsDataWatch},
    //{MENU_ITEM_SHOW, "3.生产模块检测", (void*)FacUnitTestMenu},

    {0}
};


//1.3终端管理--集中器I 型
const MENU_ITEM  g_sConcerAdmin[] =
{
    {MENU_ITEM_DISCRIB, "终端管理与维护", NULL},
	{MENU_ITEM_EXE, "1.重启抄表", (void*)RestartRdMeter},
	{MENU_ITEM_EXE, "2.停止抄表", (void*)StopRdMeter},
	{MENU_ITEM_EXE, "3.恢复抄表", (void*)ReturnRdMeter},
	{MENU_ITEM_EXE, "4.激活连接", (void*)ActivateConnect},
	{MENU_ITEM_EXE, "5.断开连接", (void*)Disconnect},
	{MENU_ITEM_EXE, "6.节点维护", (void*)NodeMaintain},
	{MENU_ITEM_EXE, "7.USB 功能", (void*)USBFunction},
	{MENU_ITEM_EXE, "8.硬件复位", (void*)HardWareReset},
    {MENU_ITEM_EXE, "9.数据清零", (void*)DataReset},
    {MENU_ITEM_EXE, "10.参数清零", (void*)ParaReset},
    {MENU_ITEM_SHOW, "11.内部调试", (void*)g_sInTestMenu},

    {0}
};


//1.3终端数据--集中器I 型
const MENU_ITEM  g_sConcerData[] =
{
    {MENU_ITEM_DISCRIB, "终端数据", NULL},
	{MENU_ITEM_EXE,  "1.遥信状态", (void*)RemoteCommState},
	{MENU_ITEM_EXE,  "2.事件记录", (void*)EventLog},

    {0}
};


//1.3终端管理与维护--集中器I 型
const MENU_ITEM  g_sConcerAdminMain[] =
{
    {MENU_ITEM_DISCRIB, "终端管理与维护", NULL},
	{MENU_ITEM_SHOW, "1.终端数据", (void*)g_sConcerData},
	{MENU_ITEM_EXE,  "2.终端信息", (void*)ConcerInfo},
	{MENU_ITEM_SHOW, "3.终端管理", (void*)g_sConcerAdmin},

    {0}
};

//1.2通信通道设置
const MENU_ITEM  g_sCommparaSet[] =
{
    {MENU_ITEM_DISCRIB, "通信通道设置", NULL},
	{MENU_ITEM_EXE, "1.通讯方式",   (void*)CommunicationModeSet},
	{MENU_ITEM_EXE, "2.短信中心",   (void*)MessageCenterSet},
	{MENU_ITEM_EXE,  "3.主站IP地址", (void*)MainIpAddrSet},
	{MENU_ITEM_EXE,  "4.以太网参数", (void*)EthernetParaSet},

    {0}
};

//1.2通信通道查询
const MENU_ITEM  g_sCommparaView[] =
{
    {MENU_ITEM_DISCRIB, "通信通道查询", NULL},
    {MENU_ITEM_EXE,  "1.通讯方式",   (void*)CommunicationMode},
	{MENU_ITEM_EXE,  "2.短信中心",   (void*)MessageCenter},
	{MENU_ITEM_EXE,  "3.主站IP地址", (void*)MainIpAddr},
	{MENU_ITEM_EXE,  "4.以太网参数", (void*)EthernetPara},

    {0}
};

//通信通道设置（选择模式）
const MENU_ITEM  g_sCommpara[] =
{
    {MENU_ITEM_DISCRIB, "通信通道", NULL},
	{MENU_ITEM_SHOW, "1.查询模式", (void*)g_sCommparaView},
	{MENU_ITEM_SHOW, "2.设置模式", (void*)g_sCommparaSet},

    {0}
};

//电表参数设置（选择模式）
const MENU_ITEM  g_sMeterPara[] =
{
    {MENU_ITEM_DISCRIB, "电表参数", NULL},
	{MENU_ITEM_EXE, "1.查询模式", (void*)MeterParamQuery},
	{MENU_ITEM_EXE, "2.设置模式", (void*)MeterParaSet},

    {0}
};

//终端时钟的设置（选择模式）
const MENU_ITEM  g_sConcerTime[] =
{
    {MENU_ITEM_DISCRIB, "终端时钟", NULL},
	{MENU_ITEM_EXE, "1.查询模式", (void*)ConCerTimequery},
	{MENU_ITEM_EXE, "2.设置模式", (void*)ConCerTimeSet},

    {0}
};

//终端编号设置（选择模式）
const MENU_ITEM  g_sConcerAddr[] =
{
    {MENU_ITEM_DISCRIB, "终端编号", NULL},
	{MENU_ITEM_EXE, "1.查询模式", (void*)ConcerNumQuery},
	{MENU_ITEM_EXE, "2.设置模式", (void*)ConcerNumSet},

    {0}
};


//1.2参数设置与查看--集中器I 型（查询模式）
const MENU_ITEM  g_sParaSetView[] =
{
    {MENU_ITEM_DISCRIB, "参数设置与查看", NULL},
	{MENU_ITEM_SHOW,  "1.通信通道设置", (void*)g_sCommpara},
	{MENU_ITEM_SHOW,  "2.电表参数设置", (void*)g_sMeterPara},
	{MENU_ITEM_SHOW,  "3.终端时间设置", (void*)g_sConcerTime},
	{MENU_ITEM_EXE,  "4.界面密码设置", (void*)PswCheck},
	{MENU_ITEM_SHOW,  "5.终端编号设置", (void*)g_sConcerAddr},

    {0}
};

//1.1测量点数据查询--集中器I 型
const MENU_ITEM  g_sMeasurePoint[] =
{
    {MENU_ITEM_DISCRIB, "测量点数据", NULL},
	{MENU_ITEM_EXE, "1.实时数据", (void*)RealTimeData},
    {MENU_ITEM_EXE, "2.日数据", (void*)DayData},
    {MENU_ITEM_EXE, "3.月数据", (void*)MonData},

    {0}
};


//1主菜单--集中器I 型
const MENU_ITEM   g_sMainMenu[] =
{
    {MENU_ITEM_DISCRIB, "主菜单", NULL},
	{MENU_ITEM_SHOW, "1.测量点数据查询", (void*)g_sMeasurePoint},
    {MENU_ITEM_SHOW, "2.参数设置与查看", (void*)g_sParaSetView},
    {MENU_ITEM_SHOW, "3.终端管理与维护", (void*)g_sConcerAdminMain},
    //{MENU_ITEM_EXE, "4.交采信息", (void*)AcInfo},
    //{MENU_ITEM_EXE, "5.资产编号配置", (void*)TermSelfAddrConfig},

    {0}
};

/**************************************************************************************************
                                           函数定义
***************************************************************************************************/
extern void LCD_ShowCharString(INT16U x,INT16U y, INT8U nRows, const INT8U *p, INT8U nMode);

/* 箭头图标 */
void  Arrow(INT8U nFlg)
{
    INT8U bmpUp[]= {0x10,0x00,0x38,0x00,0x54,0x00,0x10,0x00,0x10};
    INT8U bmpDown[]= {0x08,0x00,0x08,0x00,0x2A,0x00,0x1C,0x00,0x08,0x00};

    switch(nFlg)
    {
         case 0:
  {
    LcdRecClear(35, 0, 43, 14, g_nBackColour);
    LCD_ShowLogo(35, 0, 8, sizeof(bmpUp) * 8, bmpUp, g_nBackColour, g_nDrawColour, LCD_DISP_NORMAL);
             break;
  }
         case 1:
  {
    LcdRecClear(35, 0, 43, 14, g_nBackColour);
    LCD_ShowLogo(35, 0, 8, sizeof(bmpDown) * 8, bmpDown, g_nBackColour, g_nDrawColour, LCD_DISP_NORMAL);
             break;
         }
         case 3:
         {
             LcdRecClear(35, 0, 43, 14, g_nBackColour);
             break;
         }
    }
  }

//信号强度的指示T接口
void LcdMakeTFrame(INT8U nFlg)
{
	INT8U nBuf[12] = {0xFF,0x81,0xC3,0x66,0x3C,0x18,0x18,0x18,0x18,0x18,0x18,0x18};

	if (nFlg)
	{
		LcdRecClear(0, 0, 7, 14, g_nBackColour);
		LCD_ShowLogo(0, 2, 8, sizeof(nBuf) * 8, nBuf, g_nBackColour, g_nDrawColour, LCD_DISP_NORMAL);
	}
	else
	{
		LcdRecClear(0, 0, 7, 14, g_nBackColour);
	}
}

//液晶底层内容显示
void  BOttleDisplay(void)
{
  INT8U	    nInfo[8] 	       = {0};
  INT8S*    c_BottleShow[] = {"正在获取信号强度       ", "正在进行网络注册","正在设置APN",
                             "正在设置用户名和密码","正在获取IP地址","正在连接主站        ","正在发送登陆报文","GPRS在线","离线状态",0};
  INT8S*    c_Result[] ={"获取成功","注册成功","设置APN成功","设置用户名和密码成功",
                                  "获取IP地址成功","链接主站成功","收到主站确认",0};

    do
    {
        nInfo[0] = nCSQFlg;
        nInfo[1] = nSIMReg;
        nInfo[2] = nAPNConfig;
        nInfo[3] = nPWFlg;
        nInfo[4] = nIpPort;
        nInfo[5] = nDialFlg;
        nInfo[6] = nMessageFlg;

        if(nCounter > 3)
        {
          GuiwinTextOut(0, 144, LCD_DISP_NORMAL, "                                              ");
          GuiwinTextOut(0, 144, LCD_DISP_NORMAL, "%s", c_BottleShow[8]);
          break;
        }
        //1.上电启动
        if(g_pRmotValClr->nNdStart)
        {
             nCSQFlg = 0;
             nSIMReg = 0;
             nAPNConfig = 0;
             nPWFlg = 0;
             nIpPort = 0;
             nDialFlg = 0;
             nMessageFlg = 0;
             GprsStep = 0;
        }
        GuiwinTextOut(0, 144, LCD_DISP_NORMAL, "%s", c_BottleShow[GprsStep]);
        if((nInfo[GprsStep] == 0)&&(GprsStep != 7))
        {
           nCounter++;
           break;
        }
        if(nInfo[GprsStep])
        {
          SetBufBit(&nStepInfo[0], sizeof(nStepInfo), GprsStep);
          GuiwinTextOut(0, 144, LCD_DISP_NORMAL, "                                              ");
          GuiwinTextOut(0, 144, LCD_DISP_NORMAL, "%s", c_Result[GprsStep]);
          GprsStep++;
          if(GprsStep == 3)
          {
            LcdMakeTFrame(1);
          }
          break;
        }
    }while(0);

    if (TestBufBit(&nStepInfo[0], sizeof(nStepInfo), 6))
    {
      GuiwinTextOut(0, 144, LCD_DISP_NORMAL, "                                              ");
      GuiwinTextOut(0, 144, LCD_DISP_NORMAL, "%s", c_BottleShow[7]);
      if(g_pRmotValClr->nLoginSucc == 0)
      {
        GuiwinTextOut(0, 144, LCD_DISP_NORMAL, "%s", c_BottleShow[8]);
        if(nCSQFlg)
        {
          GprsStep = 0;
        }
      }
    }
    if(nInfo[GprsStep])
    {
      nCounter = 0;
    }
}

//信号强度无信号
static void SignalNull(void)
{
	LcdRecClear(8, 0, 23, 14, g_nBackColour);
}

//清G
static void GNull(void)
{
    LcdRecClear(25, 0, 34, 14, g_nBackColour);
}

//信号强度满格信号
static void SignalFour(void)
{
	INT8U nBuf[24] = {0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x66,0x00,0x66,0x00,0x66, 0x06,0x66,0x06,0x66,0x06,0x66,0x66,0x66,0x66,0x66,0x66,0x66};

	SignalNull();
	LCD_ShowLogo(8, 2, 16, sizeof(nBuf) * 8, nBuf, g_nBackColour, g_nDrawColour, LCD_DISP_NORMAL);
}

//信号强度三格信号
static void SignalThree(void)
{
	INT8U nBuf[24] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x00,0x60,0x00,0x60, 0x06,0x60,0x06,0x60,0x06,0x60,0x66,0x60,0x66,0x60,0x66,0x60};

	SignalNull();
	LCD_ShowLogo(8, 2, 16, sizeof(nBuf) * 8, nBuf, g_nBackColour, g_nDrawColour, LCD_DISP_NORMAL);
}
//信号强度二格信号
static void SignalTwo(void)
{
	INT8U nBuf[24] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 0x06,0x00,0x06,0x00,0x06,0x00,0x66,0x00,0x66,0x00,0x66,0x00};

	SignalNull();
	LCD_ShowLogo(8, 2, 16, sizeof(nBuf) * 8, nBuf, g_nBackColour, g_nDrawColour, LCD_DISP_NORMAL);
}

//信号强度一格信号
static void SignalOne(void)
{
	INT8U nBuf[24] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x00,0x60,0x00,0x60,0x00};

	SignalNull();
	LCD_ShowLogo(8, 2, 16, sizeof(nBuf) * 8, nBuf, g_nBackColour, g_nDrawColour, LCD_DISP_NORMAL);
}

//信号强度指示标志接口
void SignalDisp(INT8U nNum)
{
    switch(nNum)
    {
        case 4:
            SignalFour();
            break;
        case 3:
            SignalThree();
            break;
        case 2:
            SignalTwo();
            break;
        case 1:
            SignalOne();
            break;
        default:
            SignalNull();
            break;
    }
}


//秒显示内容处理，时钟显示，状态符显示
void DisSec(void)
{
    STD_TIME sTmpTime;
    static BOOLEAN  bFlg = FALSE;
    BOOLEAN bOnFlg    = FALSE;
    INT8U   nRmotSigl = 0;
    INT8U   nRmotCli  = 0;
    INT8U   nRmotSer  = 0;
    INT8U   nRomtModl = 0;

    LcdDisplayTime();
    
    //1、信号强度
    if (g_nSignalErrFlg)
    {
        SignalOne();
    }
    else
    {
        if (RET_ERR_OK == MemReadRamXmpData(F8, 0, &nRmotSigl, sizeof(nRmotSigl)))
        {
            SignalDisp(nRmotSigl);
        }
        else
        {
            SignalOne();
        }
    }
    
    BOttleDisplay();
    
    //2、上线标志
    if (g_pRmotValClr->nLoginSucc > 0)
    {
        CommSign();
    }
    else
    {
        GNull();
    }

    //3.GPRS通讯箭头标志
    if (g_pRmotValClr->nLoginSucc > 0)
    {
      if(Ascend)
      {
        Arrow(1);
        Ascend = 0;
      }
      else if(Descend)
        {
        Arrow(0);
        Descend = 0;
        }
      else
      {
        Arrow(3);
      }
    }
}

//液晶屏顶栏时间的显示
void  LcdDisplayTime(void)                  //每秒刷新一次
{
    STD_TIME    sTime;
    pSTD_TIME   pTime = &sTime;
    INT8U       nBuf[10] = {0};

    memset(pTime, 0, sizeof(STD_TIME));

    if (RET_ERR_OK != GetSysClk(pTime))
    {
        return;
    }
    sprintf((char*)&nBuf[0], "%02x:", pTime->nHour);
    LCD_ShowCharString(96, 0,  15, nBuf, LCD_DISP_NORMAL);
    sprintf((char*)&nBuf[0], "%02x:", pTime->nMin);
    LCD_ShowCharString(96 + 24, 0, 15, nBuf, LCD_DISP_NORMAL);
    sprintf((char*)&nBuf[0], "%02x:", pTime->nSec);
    LCD_ShowCharString(96 + 24 * 2, 0, 15, nBuf, LCD_DISP_NORMAL);

    OSSemPost(g_pSem[SEM_DISP_IDX]);
}


//液晶屏顶栏测量点的显示
void MearPoint(INT16S nMp,  INT8U nFlag)
{
   if(nFlag)
	{
        LCD_DrawRectangle2(70, 0, 95, 14, 1);
       GuiwinTextOut(71, 1, LCD_DISP_NORMAL, "%03d", nMp);
	}
	else
	{
       LcdRecClear(70, 0, 95, 14, g_nBackColour);
   }
}

//通信方式指示
void CommSign(void)
{
   INT8U          nBuf[10]        = "G";

   LCD_DrawXieLine2(25, 0, 34, 14, 1);
   LCD_ShowCharString(26, 0, 15, nBuf, LCD_DISP_NORMAL);
}

//设置密码
void InterfacePasswordSet(void)
{
    PARAMSET		sParamSet[4] 	               = {0};
    pPARAMSET 		pParamSet		               = &sParamSet[0];
    INT32S          nRet                           = 0;
    INT8S           nComSetMenu[1][A_MENU_LEN]     = {"000000"};
    INT8U           nBuf[1]                        = {1};

    GuiwinSetNullCfgParam(&pParamSet, NULL);
    GuiwinSetNullCfgParam(&pParamSet, NULL);
    GuiwinSetEditCfgParam(&pParamSet, "请输入密码", nComSetMenu[0], 6);

    pParamSet = &sParamSet[0];
    while(1)
    {
        if (GuiwinSetParamMenu("密码设置", pParamSet) >= 0)
        {
            memcpy(&nComSetMenu[0], pParamSet[2].nEditBuf, GET_MIN(sizeof(pParamSet[2].nEditBuf), sizeof(nComSetMenu[0]) - 1));
            MemoryWrite(STR_OFF(pMEM_FRAM_STORE, sXFramData.nPsw[0]), (INT8U*)&nComSetMenu[0], 6, MEM_FRAM);

            MemoryWrite(STR_OFF(pMEM_FRAM_STORE, sXFramData.nPswSetFlg), (INT8U*)&nBuf[0], 1, MEM_FRAM);
            nRet = Guiwin1MboxCreate("确认","设置成功");
            if (KEY_ENTER == nRet)
            {
                return;
            }
        }
        else
        {
          return;
        }
    }
}

//密码验证
INT32S InterfacePassword(void)
{
    PARAMSET		sParamSet[3]					= {0};
	pPARAMSET		pParamSet						= &sParamSet[0];
    INT32S          nRet                            = 0;
	INT8S			nPwdMenu[A_MENU_LEN]			= {0};
    INT8U           nBuf[10]                        = {0};

    MemoryRead(STR_OFF(pMEM_FRAM_STORE, sXFramData.nPswSetFlg),&nBuf[0] ,1, MEM_FRAM);
    if(nBuf[0] == 0)   //默认初始密码值"000000"
    {
       memset(&nBuf[0] , 0 ,sizeof(nBuf));
       MemoryWrite(STR_OFF(pMEM_FRAM_STORE, sXFramData.nPsw[0]), &nBuf[0], 6, MEM_FRAM);
    }
	while(1)
	{
		memset(&sParamSet[0], 0, sizeof(sParamSet));
		memset(&nPwdMenu[0], 0, sizeof(nPwdMenu));
		GuiwinSetNullCfgParam(&pParamSet, NULL);
		GuiwinSetEditCfgParam(&pParamSet, "请输入密码:", "******", 100 + STR_SIZE(pOPERTOR_PRM, nOperPsw) * 2);

        pParamSet = &sParamSet[0];
		if (GuiwinSetParamMenu("密码", pParamSet) >= 0)
		{
			memset(&nPwdMenu[0], 0, sizeof(nPwdMenu));
            StringToBuf((INT8U*)pParamSet[1].nEditBuf, (INT8U*)&nPwdMenu[0], GET_MIN(STR_SIZE(pOPERTOR_PRM, nOperPsw) * 2, pParamSet[1].nEditLen));
			if (PasswordChk(NULL, (INT8U*)&nPwdMenu[0], 0) == 1)
			{
               return 1;
			}
			Guiwin1MboxCreate("返回","密码不正确");
               continue;
		}
		else
		{
			return -1;
		}
	}
}

void PswCheck(void)
{
  INT8U   nRet    = 0;

  nRet = InterfacePassword();
  if(1 == nRet)
        {
    InterfacePasswordSet();
    }
}

//有效测量点
INT16U ValidMpNum(void)
{
    INT16U               nSeq                         = 0;
    INT16S               nValidMp                    = 0;
	INT16U				 nMaxMp						= 0;

    for(nSeq = 1; nSeq < MAX_METER_NUM ; nSeq++)
    {
	   //有效测量点
       memset(pMeterInfo, 0, sizeof(METER_INFO));
	   if (GetMeterInfo(nSeq, pMeterInfo) < 0)
       {
            
       }
       else
       {
	   		nMaxMp = nSeq;
       }
    }
	return  nMaxMp;
}

INT16S  MpRightProc(INT16S nMp, INT16S nMaxMp)
{
    INT16U               nSeq                        = 0;
    INT16S               nValidMp                    = 0;
    INT16U               nTmpMp                     = 0;
    INT8U                nFlg                       = 0;

    for(nSeq = nMp + 1; nSeq < MAX_METER_NUM ; nSeq++)
    {
	   //有效测量点
       memset(pMeterInfo, 0, sizeof(METER_INFO));
	   if (GetMeterInfo(nSeq, pMeterInfo) < 0)
       {
            
       }
       else
       {
            nTmpMp = nSeq;
            nFlg = 1;
			break;
       }
    }
    if (nFlg)
    {
        nValidMp =  nTmpMp;
    }
    else
    {
        nValidMp = 1;
    }
    return  nValidMp;
}

INT16S  MpLeftProc(INT16S nMp, INT16S nMaxMp)
{
    INT16U               nSeq                        = 0;
    INT16S               nValidMp                    = 0;
    INT16U               nTmpMp                     = 0;
    INT8U                nFlg                       = 0;

    for(nSeq = nMp - 1; nSeq >= 0 ; nSeq--)
    {
	   //有效测量点
       memset(pMeterInfo, 0, sizeof(METER_INFO));
	   if (GetMeterInfo(nSeq, pMeterInfo) < 0)
       {
            
       }
       else
       {
            nTmpMp = nSeq;
            nFlg = 1;
			break;
       }
       
    }
    if (nFlg)
    {
        nValidMp =  nTmpMp;
    }
    else
    {
        nValidMp = nMaxMp;
    }
    return  nValidMp;
}

//按键处理
INT32S  KeyProc (INT8U* nFlgCnt ,INT16S* nValidNum ,INT16S* nMearpoint ,INT8U nLenFlg)
	{
	INT8U                nKey                        = 0;
	INT16U               nStep                       = 0;
    INT8U                nTmpFlgCnt                  = 0;
    INT16S               nTmpValidNum                = 0;
    INT16S               nTmpMearpoint               = 0;
    INT8U                nFlg                        = 0;

    nTmpFlgCnt = *nFlgCnt;
    nTmpValidNum = *nValidNum;
    nTmpMearpoint = *nMearpoint;

	if(nLenFlg == 0)
	{
		nStep = LEN_REAL_SCREEN_NUM ;
            }
	else
    {
		nStep = LEN_DAY_SCREEN_NUM ;
	}
    while(1)
    {
        GET_KEY_VAL(nKey, 1);
        if (nKey != 0xFF)
        {
                if(KEY_DOWN == nKey)
       {
               nTmpFlgCnt ++ ;
               if(nTmpFlgCnt == (nStep + 1))
	{
                  nTmpFlgCnt = 1;
	   }
	}
	else if(KEY_UP == nKey)
	{
                nTmpFlgCnt -- ;
                if (nTmpFlgCnt == 0)
	   {
                   nTmpFlgCnt = nStep;
            }
	}
	else if (nKey == KEY_LEFT)
    {
		nTmpMearpoint = MpLeftProc(nTmpMearpoint, nTmpValidNum);
     }
	else if (nKey == KEY_RIGHT)
	{
       	nTmpMearpoint = MpRightProc(nTmpMearpoint, nTmpValidNum);
     }
            else if (nKey == KEY_CANCEL)
            {
                nFlg = 1;
            }
            *nFlgCnt = nTmpFlgCnt;
            *nValidNum = nTmpValidNum;
            *nMearpoint = nTmpMearpoint;

     break;
        }
    }
    return nFlg;
}
	
//电表参数查询(设置)按键处理
INT32S  KeyProc1(INT16S* nMp , INT16S* nValidNum)
{
   INT8U                nFlg                        = 0;
   INT8U                nKey                        = 0;
   INT16S               nTmpMp                      = 0;
   INT16S               nTmpValidNum                = 0;

   nTmpMp = *nMp;
   nTmpValidNum = *nValidNum;
	
   while(1)
    {
        GET_KEY_VAL(nKey, 1);
        if(nKey != 0xFF)
        {
            if (nKey == KEY_LEFT)
            {
                nTmpMp = MpLeftProc(nTmpMp, nTmpValidNum);
                nSwMpFlg = 1;
            }
            else if (nKey == KEY_RIGHT)
            {
                nTmpMp = MpRightProc(nTmpMp, nTmpValidNum);
                nSwMpFlg = 1;
            }
            else if (nKey == KEY_CANCEL)
            {
                nFlg = 1;
            }
            else if((nKey == KEY_UP)||(nKey == KEY_DOWN))
            {
              if(nSwMpFlg / 2 == 0)
              {
                nSwMpFlg = 2;
                break;
              }
              nSwMpFlg = 1;
            }
            *nMp = nTmpMp;
            *nValidNum = nTmpValidNum;

        break;
        }
    }
    return nFlg;
}

//测量点数据的抄表时间
static void  MpRdTime(INT8U* pErrFlg ,pSTD_TIME pVldTime)
{
  INT8U  nBuf[30] = {0};
  INT8U* pBuf                   = &nBuf[0];
  INT8U  nErrFlg                 = 0;

  nErrFlg = *pErrFlg;

    if(nErrFlg)
    {
       sprintf((char*)&pBuf[0], "--/--/-- --:--:--");
       GuiwinTextOutAuto("%3s%s","",pBuf);
    }
    else
    {
      GuiwinTextOutAuto("%4s%02x/%02x/%02x  %02x:%02x","",pVldTime->nYear,pVldTime->nMon ,pVldTime->nDay , pVldTime->nHour , pVldTime->nMin);
    }
}


//实时数据
void  RealTimeData(void)
{
    INT8U                nBuf[30]                    = {0};
    INT8U*               pBuf                        = &nBuf[0];
    INT16S               nMp                         = 1;
	INT8U                nStepCnt                    = 1;
	INT16S               nValidMp                   = 0;
    INT32S               nFlg                        = 0;
    INT8U                nIdex                       = 0;
    INT8U                nStr[30]                    ={0};
    INT8U               nDispBuf[10]                = {0};
    INT8U                nErrFlg                     = 0;
    STD_TIME             sVldTime;
    pSTD_TIME            pVldTime                 = &sVldTime;

	//测量点
	while(1)
	{
       nErrFlg = 0;
	   nValidMp =ValidMpNum();                             //有效测量点

       //图形标志
       MearPoint(nMp , 1);                                   //测量点

       //判断时间的有效性
       memset(pBuf, 0, sizeof(nBuf));
       memset(pVldTime , 0 ,sizeof(STD_TIME));
       MemReadRealData(nMp, F2, pBuf, sizeof(nBuf));
       memcpy(pVldTime ,pBuf, 6);
       if(!IsVldStdTime(pVldTime))
       {
          nErrFlg = 1;
       }
       switch (nStepCnt)
       {
            //测量点显示
            case 1: //正向有功总
		   {
            // 第1屏
                nIdex = 0;
                memset(pBuf, 0, sizeof(nBuf));
                //判断时间有效性
                MemReadRealData(nMp, F5, pBuf, sizeof(nBuf)); // 正向有功电能示值
                if(nErrFlg)
                    {
                        memset(pBuf, 0xEE, sizeof(nBuf));
                    }
                GuiwinTextOutAuto("%2s%s","","正向有功电能示值");
                GuiwinTextOutAuto("正向有功总:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("正向有功尖:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("正向有功峰:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("正向有功平:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("正向有功谷:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("抄表时间");

                MpRdTime(&nErrFlg ,pVldTime);
                break;
            }
            case 2: //反向有功总
            {
            // 第2屏
                nIdex = 0;
            memset(pBuf, 0, sizeof(nBuf));
                //判断时间有效性
                MemReadRealData(nMp, F6, pBuf, sizeof(nBuf)); // 反向有功电能示值
                if(nErrFlg)
                {
                    memset(pBuf, 0xEE, sizeof(nBuf));
                }
            GuiwinTextOutAuto("%2s%s","","反向有功电能示值");
                GuiwinTextOutAuto("反向有功总:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("反向有功尖:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("反向有功峰:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("反向有功平:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("反向有功谷:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
            GuiwinTextOutAuto("抄表时间");

                MpRdTime(&nErrFlg ,pVldTime);
                break;
            }
            case 3: //正向无功总
            {
            // 第3屏
                nIdex = 0 ;
            memset(pBuf, 0, sizeof(nBuf));
                //判断时间有效性
                MemReadRealData(nMp, F7, pBuf, sizeof(nBuf)); // 正向无功电能示值
                if(nErrFlg)
            {
                memset(pBuf, 0xEE, sizeof(nBuf));
            }
            GuiwinTextOutAuto("%2s%s","","正向无功电能示值");
                GuiwinTextOutAuto("正向无功总:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("正向无功尖:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("正向无功峰:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("正向无功平:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("正向无功谷:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
            GuiwinTextOutAuto("抄表时间");

                MpRdTime(&nErrFlg ,pVldTime);
                break;
            }
            case 4: //反向无功总
            {
            //第4屏
                nIdex = 0 ;
            memset(pBuf, 0, sizeof(nBuf));
                //判断时间有效性
                MemReadRealData(nMp, F8, pBuf, sizeof(nBuf)); // 反向无功电能示值
                if(nErrFlg)
            {
                memset(pBuf, 0xEE, sizeof(nBuf));
            }
            GuiwinTextOutAuto("%2s%s","","反向无功电能示值");
                GuiwinTextOutAuto("反向无功总:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("反向无功尖:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("反向无功峰:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("反向无功平:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("反向无功谷:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
            GuiwinTextOutAuto("抄表时间");

                MpRdTime(&nErrFlg ,pVldTime);
                break;
            }
            case 5: //一象限无功电能示值
            {
		        //第4屏
                nIdex = 0 ;
            memset(pBuf, 0, sizeof(nBuf));
                //判断时间有效性
                MemReadRealData(nMp, F9, pBuf, sizeof(nBuf)); // 一象限无功电能示值
                if(nErrFlg)
            {
                memset(pBuf, 0xEE, sizeof(nBuf));
            }
            GuiwinTextOutAuto("%1s%s","","一象限无功电能示值");
                GuiwinTextOutAuto("总:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("尖:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("峰:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("平:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("谷:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
            GuiwinTextOutAuto("抄表时间");

                MpRdTime(&nErrFlg ,pVldTime);
                break;
            }
            case 6: //二象限无功电能示值
            {
		    //第6屏
                nIdex = 0 ;
            memset(pBuf, 0, sizeof(nBuf));
                //判断时间有效性
                MemReadRealData(nMp, F10, pBuf, sizeof(nBuf)); // 二象限无功电能示值
                if(nErrFlg)
            {
                memset(pBuf, 0xEE, sizeof(nBuf));
            }
            GuiwinTextOutAuto("%1s%s","","二象限无功电能示值");
                GuiwinTextOutAuto("总:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("尖:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("峰:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("平:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("谷:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
            GuiwinTextOutAuto("抄表时间");

                MpRdTime(&nErrFlg ,pVldTime);
                break;
            }
            case 7: //三象限无功电能示值
            {
		    //第7屏
                nIdex = 0 ;
            memset(pBuf, 0, sizeof(nBuf));
                //判断时间有效性
                MemReadRealData(nMp, F11, pBuf, sizeof(nBuf)); // 三象限无功电能示值
                if(nErrFlg)
            {
                memset(pBuf, 0xEE, sizeof(nBuf));
            }
            GuiwinTextOutAuto("%1s%s","","三象限无功电能示值");
                GuiwinTextOutAuto("总:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("尖:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("峰:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("平:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("谷:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
            GuiwinTextOutAuto("抄表时间");

                MpRdTime(&nErrFlg ,pVldTime);
                break;
            }
            case 8: //四象限无功电能示值
            {
		    //第8屏
                nIdex = 0 ;
            memset(pBuf, 0, sizeof(nBuf));
                //判断时间有效性
                MemReadRealData(nMp, F12, pBuf, sizeof(nBuf)); // 四象限无功电能示值
                if(nErrFlg)
            {
                memset(pBuf, 0xEE, sizeof(nBuf));
            }
            GuiwinTextOutAuto("%1s%s","","四象限无功电能示值");
                GuiwinTextOutAuto("总:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("尖:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("峰:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("平:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("谷:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
            GuiwinTextOutAuto("抄表时间");

                MpRdTime(&nErrFlg ,pVldTime);
                break;
            }
            case 9: //最大需量及发生时刻
            {
		    //第9屏
                nIdex = 0;
            memset(pBuf, 0, sizeof(nBuf));
                //判断时间有效性
                MemReadRealData(nMp, F13, pBuf, sizeof(nBuf)); // 最大需量及发生时刻
                if(nErrFlg)
            {
                memset(pBuf, 0xEE, sizeof(nBuf));
            }
            GuiwinTextOutAuto("%1s%s","","最大需量及发生时刻");
            GuiwinTextOutAuto("正向有功总(KW)");
                GuiwinTextOutAuto("%11s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("%3s%02x月%02x日%02x时%02x分","",pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
            GuiwinTextOutAuto("");

                nIdex = 0;
            memset(pBuf, 0, sizeof(nBuf));
                //判断时间有效性
                MemReadRealData(nMp, F14, pBuf, sizeof(nBuf));
                if(nErrFlg)
            {
                memset(pBuf, 0xEE, sizeof(nBuf));
            }
            GuiwinTextOutAuto("反向有功总(KW)");
                GuiwinTextOutAuto("%11s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("%3s%02x月%02x日%02x时%02x分","",pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                break;
            }
            case 10: //三相电压和三相电流
            {
		    //第10屏
                nIdex = 0;
            memset(pBuf, 0, sizeof(nBuf));
                //判断时间有效性
                MemReadRealData(nMp, F17, pBuf, sizeof(nBuf));
                if(nErrFlg)
            {
                memset(pBuf, 0xEE, sizeof(nBuf));
            }
                BcdToAscii(&pBuf[nIdex], 6, TRUE, &nStr[0]);
                nIdex = 8;
                GuiwinTextOutAuto("三相电压%6s%c%c%c.%c","Ua:",nStr[nIdex],nStr[nIdex + 1],nStr[nIdex + 2],nStr[nIdex + 3]);
                nIdex -= 4;
                GuiwinTextOutAuto("%14s%c%c%c.%c","Ub:",nStr[nIdex],nStr[nIdex + 1],nStr[nIdex + 2],nStr[nIdex + 3]);
                nIdex -= 4;
                GuiwinTextOutAuto("%14s%c%c%c.%c","Uc:",nStr[nIdex],nStr[nIdex + 1],nStr[nIdex + 2],nStr[nIdex + 3]);

                nIdex = 0;
            memset(pBuf, 0, sizeof(nBuf));
                //判断时间有效性
                MemReadRealData(nMp, F18, pBuf, sizeof(nBuf));
                if(nErrFlg)
            {
                memset(pBuf, 0xEE, sizeof(nBuf));
            }
                DrawFormat25((pFORMAT25)&pBuf[0], &nDispBuf[0]);
                GuiwinTextOutAuto("三相电流 Ia:%s",nDispBuf);    

                DrawFormat25((pFORMAT25)&pBuf[3], &nDispBuf[0]);
                GuiwinTextOutAuto("%9sIb:%s","",nDispBuf);
                
                DrawFormat25((pFORMAT25)&pBuf[6], &nDispBuf[0]);
                GuiwinTextOutAuto("%9sIc:%s","",nDispBuf);
                
            GuiwinTextOutAuto("抄表时间");

                MpRdTime(&nErrFlg ,pVldTime);
                break;
            }
            case 11: //有功功率
            {
		    //第11屏
                nIdex = 0;
            memset(pBuf, 0, sizeof(nBuf));
                //判断时间有效性
                MemReadRealData(nMp, F19, pBuf, sizeof(nBuf));
                if(nErrFlg)
            {
                memset(pBuf, 0xEE, sizeof(nBuf));
            }
            GuiwinTextOutAuto("%-s","有功功率");
            GuiwinTextOutAuto("");
            GuiwinTextOutAuto("%4s总:%1s%02x.%02x%02x","","",pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
            nIdex += 3;
            GuiwinTextOutAuto("%4sA:%2s%02x.%02x%02x","","",pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
            nIdex += 3;
            GuiwinTextOutAuto("%4sB:%2s%02x.%02x%02x","","",pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
            nIdex += 3;
            GuiwinTextOutAuto("%4sC:%2s%02x.%02x%02x","","",pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
            nIdex += 3;
            GuiwinTextOutAuto("%6s%-s","","单位(KW)");
            GuiwinTextOutAuto("");
                break;
            }
            case 12: //无功功率
            {
		    //第12屏
                nIdex = 0;
            memset(pBuf, 0, sizeof(nBuf));
                //判断时间有效性
                MemReadRealData(nMp, F20, pBuf, sizeof(nBuf));
                if(nErrFlg)
            {
                memset(pBuf, 0xEE, sizeof(nBuf));
            }
            GuiwinTextOutAuto("%-s","无功功率");
            GuiwinTextOutAuto("");
            GuiwinTextOutAuto("%4s总:%1s%02x.%02x%02x","","",pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
            nIdex += 3;
            GuiwinTextOutAuto("%4sA:%2s%02x.%02x%02x","","",pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
            nIdex += 3;
            GuiwinTextOutAuto("%4sB:%2s%02x.%02x%02x","","",pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
            nIdex += 3;
            GuiwinTextOutAuto("%4sC:%2s%02x.%02x%02x","","",pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
            nIdex += 3;
            GuiwinTextOutAuto("%5s%-s","","单位(Kvar)");
            GuiwinTextOutAuto("");
                break;
            }
            case 13: //视在功率
            {
		    //第13屏
                nIdex = 0;
            memset(pBuf, 0, sizeof(nBuf));
                //判断时间有效性
                MemReadRealData(nMp, F21, pBuf, sizeof(nBuf));
                if(nErrFlg)
            {
                memset(pBuf, 0xEE, sizeof(nBuf));
            }

                GuiwinTextOutAuto("%-s","视在功率");
            GuiwinTextOutAuto("");
            GuiwinTextOutAuto("%4s总:%1s%02x.%02x%02x","","",pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
            nIdex += 3;
            GuiwinTextOutAuto("%4sA:%2s%02x.%02x%02x","","",pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
            nIdex += 3;
            GuiwinTextOutAuto("%4sB:%2s%02x.%02x%02x","","",pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
            nIdex += 3;
            GuiwinTextOutAuto("%4sC:%2s%02x.%02x%02x","","",pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
            nIdex += 3;
            GuiwinTextOutAuto("抄表时间");

                MpRdTime(&nErrFlg ,pVldTime);
                break;
            }
            case 14:
            {
		    //第14屏
                nIdex = 0;
            memset(pBuf, 0, sizeof(nBuf));
                //判断时间有效性
                MemReadRealData(nMp, F22, pBuf, sizeof(nBuf));
                if(nErrFlg)
            {
                memset(pBuf, 0xEE, sizeof(nBuf));
            }
                BcdToAscii(&pBuf[nIdex], 8, TRUE, &nStr[0]);
                nIdex = 12;
            GuiwinTextOutAuto("%-s","功率因数");
            GuiwinTextOutAuto("");
                GuiwinTextOutAuto("%4s总:%6s%c.%c%c%c","","",nStr[nIdex] ,nStr[nIdex + 1],nStr[nIdex + 2],nStr[nIdex + 3]);
                nIdex -= 4;
                GuiwinTextOutAuto("%4sA:%7s%c.%c%c%c","","", nStr[nIdex] ,nStr[nIdex + 1],nStr[nIdex + 2],nStr[nIdex + 3]);
                nIdex -= 4;
                GuiwinTextOutAuto("%4sB:%7s%c.%c%c%c","","", nStr[nIdex] ,nStr[nIdex + 1],nStr[nIdex + 2],nStr[nIdex + 3]);
                nIdex -= 4;
                GuiwinTextOutAuto("%4sC:%7s%c.%c%c%c","","", nStr[nIdex] ,nStr[nIdex + 1],nStr[nIdex + 2],nStr[nIdex + 3]);
            GuiwinTextOutAuto("抄表时间");

                MpRdTime(&nErrFlg ,pVldTime);
                break;
                }
            }
       GuiwinAutoDispProc(NULL , NULL , 0 , (INT8U*)1);                     //屏显结束语
       
       nFlg = KeyProc(&nStepCnt , &nValidMp , &nMp , 0);                    //按键处理
       if (nFlg)
            {
            MearPoint(nValidMp , 0);
            break;
            }
    }
}

//日数据
void DayData(void)
{
    INT8U                nBuf[40]                     = {0};
    INT8U*               pBuf                          = &nBuf[0];
    STD_TIME		     sTime;
	pSTD_TIME		     pTime = &sTime;
	PARAMSET		     sParamSet[3]				   = {0};
	pPARAMSET		     pParamSet					   = &sParamSet[0];
	const  INT8S 	     c_nTimeMenuBak[11]		       = "2015-12-14";
    INT8S			     nTimeMenu[11] 		           = {0};
    INT16S               nMp                           = 1;
    INT16S               nValidMp                          = 0;
	INT8U                nStepCnt                          = 1;
    INT8U                nErrFlg                        = 0;
    INT32S               nFlg                        = 0;
    INT8U                nIdex                       = 0;
    INT16U               nSeq                          = 0;
    STD_TIME             sVldTime;
    pSTD_TIME            pVldTime                 = &sVldTime;


    memset(pTime, 0, sizeof(STD_TIME));
    RTCDateTimeRead(pTime);
    pParamSet = &sParamSet[0];

	memcpy(&nTimeMenu[0], &c_nTimeMenuBak[0], sizeof(nTimeMenu));
	ValToAsciiSub((INT8U*)&nTimeMenu[2], pTime->nYear, 2, 1, 2);
	ValToAsciiSub((INT8U*)&nTimeMenu[5], pTime->nMon, 2, 1, 2);
	ValToAsciiSub((INT8U*)&nTimeMenu[8], pTime->nDay, 2, 1, 2);

	GuiwinSetNullCfgParam(&pParamSet, NULL);
    GuiwinSetEditCfgParam(&pParamSet, "     ", (INT8S*)&nTimeMenu[0], 11);

	pParamSet = &sParamSet[0];
    while (1)
    {
		if (GuiwinSetParamMenu("请选择查询日期", pParamSet) >= 0)
		{
			memcpy(&nTimeMenu[0], pParamSet[1].nEditBuf, GET_MIN(sizeof(pParamSet[1].nEditBuf), sizeof(nTimeMenu) - 1));
			if ((nTimeMenu[4] == '-') && (nTimeMenu[7] == '-'))
			{
				pTime->nYear = INT8U_TO_BCD(atoi((char *)&nTimeMenu[2]));
				pTime->nMon = INT8U_TO_BCD(atoi((char *)&nTimeMenu[5]));
				pTime->nDay = INT8U_TO_BCD(atoi((char *)&nTimeMenu[8]));
			}
           while(1)
			{
                nErrFlg = 0;
			   nValidMp =ValidMpNum();                                  //有效测量点
		       //图形标志
	           MearPoint(nMp , 1);                                            //测量点

                memset(pBuf, 0, sizeof(nBuf));
                nSeq = GetSeqFromMp(nMp);
                if (GetMeterInfo(nSeq, pMeterInfo) < 0)
                {
                    nErrFlg = 1;
                }
                MemReadDayData(nSeq, F1, &pTime->nDay, &pBuf[0], 6);
                //判断是否为有效地址
                if (0 != memcmp(&pBuf[0], &pMeterInfo->nMeterAddr[0],6))
                {
                    nErrFlg = 1;
                }

                //抄表时间
                memset(pBuf, 0, sizeof(nBuf));
                memset(pVldTime , 0 ,sizeof(STD_TIME));
                MemReadDayData(nMp, F3, &pTime->nDay ,pBuf, sizeof(nBuf));
                memcpy(pVldTime ,pBuf, sizeof(STD_TIME));

                memset(pBuf, 0, sizeof(nBuf));
                if(MemReadDayData(nMp, F2, &pTime->nDay ,&pBuf[0], 3) < 0)      //时间(标志)YY-MM-DD
                {
                   nErrFlg = 1;
                }
                //比对时间标志

                if((0 != memcmp(&pBuf[0], &pTime->nDay, 3)) || (!IsVldStdTime(pVldTime)))
                {
                    nErrFlg = 1;
                }
		       switch (nStepCnt)
		       {
		            //测量点显示
		            case 1: //正向有功总
                    {
			// 第1屏
                        nIdex = 0;
            memset(pBuf, 0, sizeof(nBuf));
                        //判断时间有效性
                        MemReadDayData(nMp, F4, &pTime->nDay, pBuf, sizeof(nBuf)); // 正向有功电能示值
                        if(nErrFlg)
                        {
                          memset(pBuf , 0xEE ,sizeof(nBuf));
                        }
            GuiwinTextOutAuto("%2s%s","","正向有功电能示值");
                        GuiwinTextOutAuto("正向有功总:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("正向有功尖:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("正向有功峰:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("正向有功平:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("正向有功谷:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
            GuiwinTextOutAuto("抄表时间");

                        MpRdTime(&nErrFlg ,pVldTime);
		                break;
		            }
		            case 2: //反向有功总
		            {
            // 第2屏
                        nIdex = 0;
            memset(pBuf, 0, sizeof(nBuf));
                        //判断时间有效性
                        MemReadDayData(nMp, F5, &pTime->nDay, pBuf, sizeof(nBuf)); // 正向有功电能示值
                        if(nErrFlg)
                        {
                          memset(pBuf , 0xEE ,sizeof(nBuf));
                        }
            GuiwinTextOutAuto("%2s%s","","反向有功电能示值");
                        GuiwinTextOutAuto("反向有功总:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("反向有功尖:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("反向有功峰:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("反向有功平:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("反向有功谷:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
            GuiwinTextOutAuto("抄表时间");

                        MpRdTime(&nErrFlg ,pVldTime);
		                break;
		            }
		            case 3: //最大需量及发生时刻
		            {
		                //第9屏
                        nIdex = 0;
            memset(pBuf, 0, sizeof(nBuf));
                        //判断时间有效性
                        MemReadDayData(nMp, F12, &pTime->nDay, pBuf, sizeof(nBuf)); // 正向有功电能示值
                        if(nErrFlg)
                        {
                          memset(pBuf , 0xEE ,sizeof(nBuf));
                        }
            GuiwinTextOutAuto("%1s%s","","最大需量及发生时刻");
            GuiwinTextOutAuto("正向有功总(KW)");
                        GuiwinTextOutAuto("%11s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("%3s%02x月%02x日%02x时%02x分","",pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
            GuiwinTextOutAuto("");

                        nIdex = 0;
            memset(pBuf, 0, sizeof(nBuf));
                        //判断时间有效性
                        MemReadDayData(nMp, F13, &pTime->nDay, pBuf, sizeof(nBuf)); // 正向有功电能示值
                        if(nErrFlg)
                        {
                          memset(pBuf , 0xEE ,sizeof(nBuf));
                        }
            GuiwinTextOutAuto("反向有功总(KW)");
                        GuiwinTextOutAuto("%11s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("%3s%02x月%02x日%02x时%02x分","",pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
		                break;
			}
	            }
               GuiwinAutoDispProc(NULL , NULL , 0 , (INT8U*)1);              //屏显结束语

               nFlg = KeyProc(&nStepCnt , &nValidMp , &nMp , 1);                    //按键处理
               if (nFlg)
			{
                    MearPoint(nValidMp , 0);
                    break;
			}
		}
        }
            return;
        }
    }

//月数据
void MonData (void)
{
    INT8U                nBuf[40]                     = {0};
    INT8U*               pBuf                          = &nBuf[0];
    STD_TIME		     sTime;
	pSTD_TIME		     pTime = &sTime;
	PARAMSET		     sParamSet[3]				   = {0};
	pPARAMSET		     pParamSet					   = &sParamSet[0];
	INT8S			     nTimeMenu[8] 		           = {0};
	INT8S 	             c_nTimeMenuBak[8]		       = "2015-12";
    INT8U                nKey                          = 0;
    INT16S               nMp                           = 1;
    INT16S               nValidMp                     = 0;
	INT8U                nStepCnt                     = 1;
    INT8U                nErrFlg                        = 0;
    INT32S               nFlg                        = 0;
    INT8U                nIdex                       = 0;
    INT16U               nSeq                          = 0;
    STD_TIME             sVldTime;
    pSTD_TIME            pVldTime                 = &sVldTime;

    memset(pTime, 0, sizeof(STD_TIME));
    RTCDateTimeRead(pTime);
    pParamSet = &sParamSet[0];

	memcpy(&nTimeMenu[0], &c_nTimeMenuBak[0], sizeof(nTimeMenu));
	ValToAsciiSub((INT8U*)&nTimeMenu[2], pTime->nYear, 2, 1, 2);
	ValToAsciiSub((INT8U*)&nTimeMenu[5], pTime->nMon, 2, 1, 2);

	GuiwinSetNullCfgParam(&pParamSet, NULL);
	GuiwinSetEditCfgParam(&pParamSet, "      ", (INT8S*)&nTimeMenu[0], 8);

	pParamSet = &sParamSet[0];
    while (1)
    {
		if (GuiwinSetParamMenu("请选择查询日期", pParamSet) >= 0)
		{
			memcpy(&nTimeMenu[0], pParamSet[1].nEditBuf, GET_MIN(sizeof(pParamSet[1].nEditBuf), sizeof(nTimeMenu) - 1));
			if (nTimeMenu[4] == '-')
			{
				pTime->nYear = INT8U_TO_BCD(atoi((char *)&nTimeMenu[2]));
				pTime->nMon = INT8U_TO_BCD(atoi((char *)&nTimeMenu[5]));
			}
		   while(1)
           {
               nErrFlg = 0;
			   nValidMp =ValidMpNum();                                  //有效测量点
		       //图形标志
               MearPoint(nMp , 1);                                            //测量点

                nSeq = GetSeqFromMp(nMp);
                if (GetMeterInfo(nSeq, pMeterInfo) < 0)
                {
                    nErrFlg = 1;
                }
                //判断地址
                MemReadMonData(nSeq, F1, &pTime->nMon, &pBuf[0], 6);
                if (0 != memcmp(&pBuf[0], &pMeterInfo->nMeterAddr[0],6))
                {
                    nErrFlg = 1;
                }

                //抄表时间
                memset(pBuf , 0 ,sizeof(nBuf));
                memset(pVldTime , 0 ,sizeof(STD_TIME));
                MemReadMonData(nMp, F3, &pTime->nMon ,pBuf, sizeof(nBuf)); // 读表时间YY-MM-DD-HH-MM-SS
                memcpy(pVldTime ,pBuf, sizeof(STD_TIME));

                //判断时间
                memset(pBuf , 0 ,sizeof(nBuf));
                if (MemReadMonData(nSeq, F2, &pTime->nMon, &pBuf[0], 2) < 0)
                {
                    nErrFlg = 1;
                }
                if((0 != memcmp(&pTime->nMon, &pBuf[0],2)) || (!IsVldStdTime(pVldTime)))
                {
                    nErrFlg = 1;
                }
		       switch (nStepCnt)
		       {
		            //测量点显示
		            case 1: //正向有功总
			{
			// 第1屏
                        nIdex = 0;
            memset(pBuf, 0, sizeof(nBuf));
                        //判断时间有效性
                          MemReadMonData(nMp, F4, &pTime->nMon, pBuf, sizeof(nBuf)); // 正向有功电能示值
                        if(nErrFlg)
                        {
                          memset(pBuf , 0xEE ,sizeof(nBuf));
                        }
            GuiwinTextOutAuto("%2s%s","","正向有功电能示值");
                        GuiwinTextOutAuto("正向有功总:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("正向有功尖:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("正向有功峰:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("正向有功平:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("正向有功谷:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
            GuiwinTextOutAuto("抄表时间");

                        MpRdTime(&nErrFlg ,pVldTime);
		                break;
		            }
		            case 2: //反向有功总
		            {
            // 第2屏
                        nIdex = 0;
            memset(pBuf, 0, sizeof(nBuf));
                        //判断时间有效性
                          MemReadMonData(nMp, F5, &pTime->nMon, pBuf, sizeof(nBuf)); // 反向有功电能示值
                        if(nErrFlg)
                        {
                          memset(pBuf , 0xEE ,sizeof(nBuf));
                        }
            GuiwinTextOutAuto("%2s%s","","反向有功电能示值");
                        GuiwinTextOutAuto("反向有功总:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("反向有功尖:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("反向有功峰:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("反向有功平:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("反向有功谷:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
            GuiwinTextOutAuto("抄表时间");

                        MpRdTime(&nErrFlg ,pVldTime);
		                break;
		            }
		            case 3: //最大需量及发生时刻
		            {
		                //第9屏
                        nIdex = 0;
            memset(pBuf, 0, sizeof(nBuf));
                        //判断时间有效性
                        MemReadMonData(nMp, F12, &pTime->nMon, pBuf, sizeof(nBuf)); // 正向有功电能示值
                        if(nErrFlg)
                        {
                          memset(pBuf , 0xEE ,sizeof(nBuf));
                        }
            GuiwinTextOutAuto("%1s%s","","最大需量及发生时刻");
            GuiwinTextOutAuto("正向有功总(KW)");
                        GuiwinTextOutAuto("%11s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("%3s%02x月%02x日%02x时%02x分","",pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
            GuiwinTextOutAuto("");

                        nIdex = 0;
            memset(pBuf, 0, sizeof(nBuf));
                        //判断时间有效性
                        MemReadMonData(nMp, F13, &pTime->nMon, pBuf, sizeof(nBuf)); // 正向有功电能示值
                        if(nErrFlg)
                        {
                          memset(pBuf , 0xEE ,sizeof(nBuf));
                        }
            GuiwinTextOutAuto("反向有功总(KW)");
                        GuiwinTextOutAuto("%11s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("%3s%02x月%02x日%02x时%02x分","",pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
		                break;
			}
			}
               GuiwinAutoDispProc(NULL , NULL , 0 , (INT8U*)1);              //屏显结束语

               nFlg = KeyProc(&nStepCnt , &nValidMp , &nMp , 1);                    //按键处理
               if (nFlg)
			{
                    MearPoint(nValidMp , 0);
                    break;
			}
		}
		}
            return;
        }
    }

//通信通道查询——通讯方式
void CommunicationMode(void)
{
    INT8U                nBuf[20]                   = {0};
    INT8U*               pBuf                        = &nBuf[0];

    pBuf = &nBuf[0];
    memset(pBuf, 0, sizeof(nBuf));
    MemReadCommPrm(F1, &nBuf[0], sizeof(nBuf));
    GuiwinTextOutAuto("重发次数:%5s%02x", "",((pBuf[3] >> 4) & 0x03));
    GuiwinTextOutAuto("心跳周期:%5s%02d", "",pBuf[5]);

    pBuf = &nBuf[0];
    memset(pBuf, 0, sizeof(nBuf));
    MemReadCommPrm(F8, &nBuf[0], sizeof(nBuf));
    GuiwinTextOutAuto("重拨次数:%5s%03x", "",pBuf[3]);
    GuiwinTextOutAuto("重拨间隔:%5s%05d", "",pBuf[2] * 256 + pBuf[1]);
    GuiwinTextOutAuto("无通讯时间:%3s%03d", "",pBuf[4]);

    switch(nBuf[0]&0x03)
    {
        case 1:
            strcpy((char *)nBuf, "永久在线");
            break;
        case 2:
            strcpy((char *)nBuf, "被动激活");
            break;
        case 3:
            strcpy((char *)nBuf, "时段在线");
            break;
        default:
            strcpy((char *)nBuf, "默认");
            break;
    }
    GuiwinTextOutAuto("在线方式:%3s%s","",nBuf);
    GuiwinAutoDispProc("通讯方式" , NULL , 0 , NULL);
}

//通信通道读取——短信中心
void MessageCenter(void)
{
    INT8U                nBuf[20]                   = {0};
    INT8U*               pBuf                       = &nBuf[0];
    INT8U                nData[40]                  = {0};
    INT8U*               pData                      = &nData[0];
    INT8U                i                          = 0;
    INT8U                nIdx                       = 0;
    INT8U                nStr[20]                   ={0};
    INT8U                nLen                       = 0;

    pBuf = &nBuf[0];
    pData = &nData[0];
    memset(pBuf, 0, sizeof(nBuf));
    memset(pData , 0 ,sizeof(nData));
    MemReadCommPrm(F4, &nBuf[0], sizeof(nBuf));

    //非BCD码不打印-------主站号码
    nIdx = 0;
    for (i = 0; i < 8; i++)
    {
         nLen =ValToAsciiSub((INT8U*)&pData[nIdx], pBuf[i], 2, 1, 2);
         nIdx += nLen;
         }
    for(i = 0 ; i < nIdx ; i++)
         {
      if(pData[i] == 'f')
      {
         pData[i] = '\0';
             break;
         }
    }
    //打印主站号码
    memcpy(nStr ,pData ,sizeof(nStr));
    GuiwinTextOutAuto("主站号码");
    GuiwinTextOutAuto("%3s%s","",nStr);

    //非BCD码不打印-------短信中心
    nIdx = 0;
    memset(pData , 0 ,sizeof(nData));
    memset(&nStr[0], 0 ,sizeof(nStr));
    for (i = 0; i < 8; i++)
    {
         nLen =ValToAsciiSub((INT8U*)&pData[nIdx], pBuf[i + 8], 2, 1, 2);
         nIdx += nLen;
         }
    for(i = 0 ; i < nIdx ; i++)
    {
      if(pData[i] == 'f')
         {
         pData[i] = '\0';
             break;
         }
    }
    //打印短信中心
    memcpy(nStr ,pData ,sizeof(nStr));
    GuiwinTextOutAuto("短信中心");
    GuiwinTextOutAuto("%3s%s","",nStr);

    GuiwinTextOutAuto("用户名:");
    GuiwinTextOutAuto("密码:");

    GuiwinAutoDispProc("短信中心" , NULL , 0 , NULL);

}

//通信通道读取——主站IP地址
void MainIpAddr(void)
{
    INT8U                nBuf[30]                   = {0};
    INT8U*               pBuf                       = &nBuf[0];
    INT8U                nAPN[]                     = {0};

    pBuf = &nBuf[0];
    memset(pBuf, 0, sizeof(nBuf));
    MemReadCommPrm(F3, &nBuf[0], sizeof(nBuf));

    GuiwinTextOutAuto("主用IP");
    GuiwinTextOutAuto("%4s%03d.%03d.%03d.%03d","",pBuf[0],pBuf[1],pBuf[2],pBuf[3]);
    GuiwinTextOutAuto("主用端口: %05d", pBuf[5] * 256 + pBuf[4]);
    GuiwinTextOutAuto("备用IP");
    GuiwinTextOutAuto("%4s%03d.%03d.%03d.%03d","",pBuf[6],pBuf[7],pBuf[8],pBuf[9]);
    GuiwinTextOutAuto("备用端口: %05d", pBuf[11] * 256 + pBuf[10]);
    memcpy(nAPN ,&pBuf[12],30);
    GuiwinTextOutAuto("APN: %s", nAPN);
    GuiwinAutoDispProc(NULL , NULL , 0 , NULL);
}
//通信通道读取——以太网参数
void EthernetPara(void)
{
    INT8U                nBuf[70]                   = {0};
    INT8U*               pBuf                       = &nBuf[0];
    pBuf = &nBuf[0];
    memset(pBuf, 0, sizeof(nBuf));
    MemReadCommPrm(F7, &nBuf[0], sizeof(nBuf));

    GuiwinTextOutAuto("终端IP");
    GuiwinTextOutAuto("%4s%03d.%03d.%03d.%03d","",pBuf[0],pBuf[1],pBuf[2],pBuf[3]);
    GuiwinTextOutAuto("子网掩码");
    GuiwinTextOutAuto("%4s%03d.%03d.%03d.%03d","",pBuf[4],pBuf[5],pBuf[6],pBuf[7]);
    GuiwinTextOutAuto("网关地址");
    GuiwinTextOutAuto("%4s%03d.%03d.%03d.%03d","",pBuf[8],pBuf[9],pBuf[10],pBuf[11]);
    GuiwinTextOutAuto("侦听端口: %05d", pBuf[61] * 256 + pBuf[60]);

    pBuf = &nBuf[0];
    memset(&nBuf[0], 0, sizeof(nBuf));
    MemReadCommPrm(F8, &nBuf[0], sizeof(nBuf));
    //链接模式
    switch(((nBuf[0] >> 7) & 0x01))
    {
        case 0:
            strcpy((char *)nBuf, "TCP");
            break;
        case 1:
            strcpy((char *)nBuf, "UDP");
            break;
    }
    GuiwinTextOutAuto("连接模式:%1s%s","",nBuf);
    //工作模式
    pBuf = &nBuf[0];
    memset(&nBuf[0], 0, sizeof(nBuf));
    MemReadCommPrm(F8, &nBuf[0], sizeof(nBuf));
    switch((pBuf[0] >> 4) & 0x03)
    {
        case 0:
            strcpy((char *)nBuf, "混合模式");
            break;
        case 1:
            strcpy((char *)nBuf, "客户机模式");
            break;
        case 2:
            strcpy((char *)nBuf, "服务器模式");
            break;
        default:
            strcpy((char *)nBuf, "无效");
            break;
    }
    GuiwinTextOutAuto("工作模式:");
    GuiwinTextOutAuto("%10s%s","", nBuf);
    GuiwinAutoDispProc(NULL , NULL , 0 , NULL);
}

//电能表参数
void Read_Meter_Param(INT16U* pDatanum ,INT16U nMearpoint ,const pMEM_METER_PRM pMeterVld)
{
    INT16U 	             Index                      = 0;
    INT8U			     nBuf[50]		            = {0};
	INT8U*			     pData			            = &nBuf[0];
    INT8U                RecvBuf[A_MENU_LEN]        ={0};
    INT32U               nBaud                      = 0;

	Index = 0;
    pData = &nBuf[0];
    memset(pData, 0, sizeof(nBuf));
    memset(&RecvBuf[0], 0, sizeof(RecvBuf));

	MemReadMeterPrm(pDatanum[nMearpoint], F10, &nBuf[0], sizeof(nBuf));

    GuiwinTextOutAuto("测量点号:%3s%04d","", nMearpoint);

    if (nSwMpFlg == 1)
    {
	//测量点端口
    switch(pData[Index + 5]&0x1F)
    {
        case 1:
            strcpy((char *)RecvBuf, "交采口");
            break;
        case 2:
            strcpy((char *)RecvBuf, "RS4851");
            break;
        case 3:
            strcpy((char *)RecvBuf, "RS4852");
            break;
        case 4:
            strcpy((char *)RecvBuf, "RS4853");
            break;
        case 30:
            strcpy((char *)RecvBuf, "级联RS485");
            break;
        case 31:
            strcpy((char *)RecvBuf, "载波口");
            break;
        default:
            strcpy((char *)RecvBuf, "无效");
            break;
    }
    GuiwinTextOutAuto("通信端口:%2s%s","", RecvBuf);

	//波特率
    nBaud = Buf2Long(&pData[Index + 6], 4);
    GuiwinTextOutAuto("通信速率:%3s%d","", nBaud);

    //测量点通信规约
    memset(&RecvBuf[0], 0, sizeof(RecvBuf));
    switch(pData[Index + 10])
    {
        case 0:
            strcpy((char *)RecvBuf, "无效");
            break;
        case 1:
            strcpy((char *)RecvBuf, "DL/T 645-1997");
            break;
        case 2:
            strcpy((char *)RecvBuf, "交流采样协议");
            break;
        case 30:
            strcpy((char *)RecvBuf, "DL/T 645-2007");
            break;
        case 31:
            strcpy((char *)RecvBuf, "串行载波协议");
            break;
        default:
            strcpy((char *)RecvBuf, "未知");
            break;
    }
    GuiwinTextOutAuto("协议:  %s",RecvBuf);

    Index = 0;
        GuiwinTextOutAuto("电表地址:");
        GuiwinTextOutAuto("      %02x%02x%02x%02x%02x%02x",pData[Index+16], pData[Index+15],
                   pData[Index+14], pData[Index+13], pData[Index+12], pData[Index+11]);

    Index = 0;
        GuiwinTextOutAuto("采集器:");
        GuiwinTextOutAuto("      %02x%02x%02x%02x%02x%02x", pData[Index+31], pData[Index+30],
                    pData[Index+29], pData[Index+28], pData[Index+27], pData[Index+26]);
    }
    else
    {
    memset(&RecvBuf[0], 0, sizeof(RecvBuf));
    switch(pData[Index + 32])
    {
        case 1:
                strcpy((char *)RecvBuf, "大型专变用户");
            break;
        case 2:
                strcpy((char *)RecvBuf, "中小型专变用户");
            break;
        case 3:
                strcpy((char *)RecvBuf, "三相工商业用户");
            break;
        case 4:
                strcpy((char *)RecvBuf, "单相工商业用户");
            break;
        case 5:
                strcpy((char *)RecvBuf, "居民用户");
            break;
        case 6:
                strcpy((char *)RecvBuf, "配变计量点");
            break;
        default:
                strcpy((char *)RecvBuf, "默认用户");
            break;
    }
        GuiwinTextOutAuto("用户大类:");
        GuiwinTextOutAuto("%s",RecvBuf);

		if(pData[Index + 32] == 3)
    {
            GuiwinTextOutAuto("用户小类:");
			GuiwinTextOutAuto("%s",c_PartTypeC[pData[Index + 33]]);
    }
		else if(pData[Index + 32] == 4)
		{
			GuiwinTextOutAuto("用户小类:");
			GuiwinTextOutAuto("%s",c_PartTypeD[pData[Index + 33]]);
		}
		else if(pData[Index + 32] == 5)
		{
			GuiwinTextOutAuto("用户小类:");
			GuiwinTextOutAuto("%s",c_PartTypeE[pData[Index + 33]]);
		}
		else if(pData[Index + 32] == 6)
		{
			GuiwinTextOutAuto("用户小类:");
			GuiwinTextOutAuto("%s",c_PartTypeF[pData[Index + 33]]);
    }
    else
    {
            GuiwinTextOutAuto("用户小类:");
			GuiwinTextOutAuto("%s", c_PartTypeAB[0]);
		}

        memset(pData , 0 ,sizeof(nBuf));
        MemReadMeterPrm(pDatanum[nMearpoint],F25 ,pData ,sizeof(nBuf));

        GuiwinTextOutAuto("电压倍率:%3s%d","", Buf2Long(&pData[0] , 2));
        GuiwinTextOutAuto("电流倍率:%3s%d","", Buf2Long(&pData[2] , 2));
    }
    GuiwinAutoDispProc(NULL, NULL, 0, (INT8U*)1);   //不需要等待按键。
}

//终端时钟的查询
void ConCerTimequery(void)
{
    STD_TIME		     sTime;
	pSTD_TIME		     pTime                       = &sTime;

    memset(pTime, 0, sizeof(STD_TIME));
    RTCDateTimeRead(pTime);

    GuiwinTextOutAuto("%6s终端时钟","");
    GuiwinTextOutAuto("");
    GuiwinTextOutAuto("%4s%02x年%02x月%02x日","",pTime->nYear,pTime->nMon,pTime->nDay );
    GuiwinTextOutAuto("");
    GuiwinTextOutAuto("%4s%02x时%02x分%02x秒","",pTime->nHour,pTime->nMin,pTime->nSec);
    GuiwinAutoDispProc(NULL, NULL, 0, NULL);
}

//终端编号查询
void ConcerNumQuery(void)
{
    INT8U			     nBuf[10]		             = {0};
	INT8U*			     pBuf			             = &nBuf[0];

    pBuf = &nBuf[0];
    memset(pBuf, 0, sizeof(nBuf));
    MemReadCommPrm(F94, &nBuf[0], sizeof(nBuf));

    GuiwinTextOutAuto("");
    GuiwinTextOutAuto("%6s终端编码","");
    GuiwinTextOutAuto("");
    GuiwinTextOutAuto("行政区码:%4s%02x%02x","", pBuf[1], pBuf[0]);
    GuiwinTextOutAuto("");
    GuiwinTextOutAuto("终端地址:");
    GuiwinTextOutAuto("%4s%02x%02x (十六进制)","", pBuf[3], pBuf[2]);
    GuiwinTextOutAuto("%4s%05d   (十进制)","", Buf2INT16U(&nBuf[2]));

    GuiwinAutoDispProc(NULL, NULL, 0, NULL);

}

//遥信状态
void RemoteCommState(void)
{


}

//事件记录
void EventLog(void)
{

}

//终端信息
void ConcerInfo(void)
{
    INT8U			     nBuf[100]		             = {0};
	INT8U*			     pBuf			             = &nBuf[0];
    STD_TIME		     sTime;
	pSTD_TIME		     pTime                       = &sTime;

    pBuf = &nBuf[0];
    memset(pBuf, 0, sizeof(nBuf));
    MemReadCommPrm(F94, &nBuf[0], sizeof(nBuf));

    GuiwinTextOutAuto("行政区码:%02x%02x", pBuf[1], pBuf[0]);
    GuiwinTextOutAuto("终端地址:%02x%02x(H)",pBuf[3], pBuf[2]);
    GuiwinTextOutAuto("终端地址:%05d(D)", Buf2INT16U(&nBuf[2]));

	memset(pTime, 0, sizeof(STD_TIME));
	GetSysClk(pTime);
    GuiwinTextOutAuto("终端日期:%02x/%02x/%02x",pTime->nYear,pTime->nMon,pTime->nDay);

    pBuf = &nBuf[0];
    memset(pBuf, 0, sizeof(nBuf));
    MemReadCommPrm(F7, &nBuf[0], sizeof(nBuf));

    GuiwinTextOutAuto("终端IP");
    GuiwinTextOutAuto("%4s%03d.%03d.%03d.%03d","",pBuf[0],pBuf[1],pBuf[2],pBuf[3]);
	GuiwinTextOutAuto("终端端口:%d", Buf2INT16U(&pBuf[62]));

	pBuf = &nBuf[0];
    memset(pBuf, 0, sizeof(nBuf));
	strcat((char*)pBuf, (char*)"软件版本:V");
	strcat((char*)pBuf, (char*)PRODUCT);
#ifdef MIN_PRODUCT
	strcat((char*)pBuf, (char*)MIN_PRODUCT);
#endif
	strcat((char*)pBuf, (char*)"-");
	strcat((char*)pBuf, (char*)MAIN_VERSION_INFO);
	strcat((char*)pBuf, (char*)VERSION_INFO);
#ifdef MIN_VERSION_INFO
	strcat((char*)pBuf, (char*)"-");
	strcat((char*)pBuf, (char*)MIN_VERSION_INFO);
#endif
	GuiwinTextOutAuto("%s", pBuf);

	pBuf = &nBuf[0];
    memset(pBuf, 0, sizeof(nBuf));
	strcat((char*)pBuf, (char*)"软件日期:");
	strcat((char*)pBuf, (char*)DATE_INFO);
	GuiwinTextOutAuto("%s", pBuf);
	
	pBuf = &nBuf[0];
    memset(pBuf, 0, sizeof(nBuf));
    MemReadCommPrm(F95, &nBuf[0], sizeof(nBuf));

    GuiwinTextOutAuto("资产号:%02x%02x%02x%02x", pBuf[1], pBuf[0], pBuf[3], pBuf[2]);

    GuiwinAutoDispProc("终端信息", NULL, 0, NULL);
}

//通信通道设置——通信方式
void CommunicationModeSet(void)
{
    PARAMSET		sParamSet[10] 	            = {0};
	pPARAMSET 		pParamSet		            = &sParamSet[0];
    INT8U           nBuf[15]                      = {0};
    INT8U*          pBuf                        =&nBuf[0];
	INT8U           nBuf1[10]                     = {0};
    INT8U*          pBuf1                         =&nBuf1[0];
    INT8U           nData[6]                      = {0};
    INT8U*          pData                       = &nData[0];
    INT8U           nIndex                        = 0;
    INT8U           nLen                        = 0;
    const INT8S*	nOnlineMode[]			    ={"无效","永久在线", "被动激活","时段在线", 0};
    INT8S           nLineMode                   = 0;
	INT8U           nRet                          = 0;

    InterfacePassword();

    nIndex = 0;
    pParamSet = &sParamSet[0];
    memset(&sParamSet[0], 0, sizeof(sParamSet));

    pBuf = &nBuf[0];
        memset(&nBuf[0], 0, sizeof(nBuf));
    MemReadCommPrm(F1, &nBuf[0], sizeof(nBuf));

    nIndex = 0;                                                     // 转换   nBuf 、 ip  ----> pData  重发次数
    memset(pData, 0, sizeof(nData));
    nLen = ValToAscii(&pData[nIndex], (nBuf[3] >> 4)&0x03, 3);
    nIndex += nLen ;
    GuiwinSetEditCfgParam(&pParamSet, "重发次数:     ",(INT8S*)pData, 3);

    nIndex = 0;                                                     // 转换   nBuf 、 port  ----> pData  心跳周期
    memset(pData, 0, sizeof(nData));
    nLen = ValToAscii(&pData[nIndex], nBuf[5], 5);
    nIndex += nLen ;
    GuiwinSetEditCfgParam(&pParamSet, "心跳周期:     ",(INT8S*)pData, 5);

    pBuf1 = &nBuf1[0];
    memset(&nBuf1[0], 0, sizeof(nBuf1));
    MemReadCommPrm(F8, &nBuf1[0], sizeof(nBuf1));

    nIndex = 0;                                                     // 转换   nBuf 、 port  ----> pData  重拨间隔
    memset(pData, 0, sizeof(nData));
    nLen = ValToAscii(&pData[nIndex], Buf2INT16U(&nBuf1[1]), 5);
    nIndex += nLen ;
    GuiwinSetEditCfgParam(&pParamSet, "重拨间隔:     ",(INT8S*)pData, 5);

    nIndex = 0;
    memset(pData, 0, sizeof(nData));                                // 转换   nBuf 、 port  ----> pData  重拨次数
    nLen = ValToAscii(&pData[nIndex], nBuf1[3], 3);
    GuiwinSetEditCfgParam(&pParamSet, "重拨次数:     ",(INT8S*)pData, 3);

    nIndex = 0;                                                    // 转换   nBuf 、 port  ----> pData  无通讯时间
    memset(pData, 0, sizeof(nData));
    nLen = ValToAscii(&pData[nIndex], nBuf1[4], 3);
    nIndex += nLen ;
    GuiwinSetEditCfgParam(&pParamSet, "无通讯时间:   ",(INT8S*)pData, 3);

    nLineMode = nBuf1[0] &0x03;                                    // 转换   nBuf 、 port  ----> pData  在线模式
    GuiwinSetSelectCfgParam(&pParamSet, "在线方式:", (INT8S*)&nLineMode, nOnlineMode);

    pParamSet = &sParamSet[0];
    while(1)
    {
        if (GuiwinSetParamMenu("", pParamSet) >= 0)
        {
			pBuf = &nBuf[0];
            memset(pBuf, 0, sizeof(nBuf));
            MemReadCommPrm(F1, &nBuf[0], sizeof(nBuf));

            //重发次数
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[0].nEditBuf, GET_MIN(sizeof(pParamSet[0].nEditBuf), sizeof(nData) - 1));
            pBuf[3]&= 0xCF;
            pBuf[3]|= (INT8U)(atoi((char *)pData)<<4);

            //心跳周期
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[1].nEditBuf, GET_MIN(sizeof(pParamSet[1].nEditBuf), sizeof(nData) - 1));
            pBuf[5] = (INT8U)( atoi((char *)pData) & 0x00FF );

            pBuf1 = &nBuf1[0];
            memset(pBuf1, 0, sizeof(nBuf1));
            MemReadCommPrm(F8, &nBuf1[0], sizeof(nBuf));
            //在线方式
            memset(pData, 0, sizeof(nData));
            pBuf1[0] &= 0xFC;
            pBuf1[0] |= nLineMode;

           //重拨间隔
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[2].nEditBuf, GET_MIN(sizeof(pParamSet[2].nEditBuf), sizeof(nData) - 1));
            pBuf1[1] = (INT8U)( atoi((char *)pData) & 0x00FF );
            pBuf1[2] = (INT8U)(( atoi((char *)pData) & 0xFF00 ) >> 8);

            //重拨次数
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[3].nEditBuf, GET_MIN(sizeof(pParamSet[3].nEditBuf), sizeof(nData) - 1));
            pBuf1[3] = (INT8U)(atoi((char *)pData));

            //无通讯时间
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[4].nEditBuf, GET_MIN(sizeof(pParamSet[4].nEditBuf), sizeof(nData) - 1));
            pBuf1[4] = (INT8U)(atoi((char *)pData));

            nRet = Guiwin1MboxCreate("确认","是否确认修改通讯参数?");
            if (KEY_ENTER != nRet)
            {
                 continue;
            }
            MemWriteCommPrm(F1, &nBuf[0], sizeof(nBuf)); //设置通信参数
            MemWriteCommPrm(F8, &nBuf1[0], sizeof(nBuf1)); //设置通信参数
        }
        return ;
    }
}

//通信通道设置——短信中心
void MessageCenterSet(void)
{
    PARAMSET		sParamSet[5] 	            = {0};
	pPARAMSET 		pParamSet		            = &sParamSet[0];
    INT8U           nBuf[50]                    = {0};
    INT8U*          pBuf                        =&nBuf[0];
    INT8U           nData[20]                   = {0};
    INT8U*          pData                       = &nData[0];
    INT8U           nIdx                        = 0;
    INT8U           nLen                        = 0;
    INT32S          nRet                        = 0;
    INT8U           i                           = 0;

    InterfacePassword();

    nIdx = 0;
    pParamSet = &sParamSet[0];
    memset(&sParamSet[0], 0, sizeof(sParamSet));

    memset(&nBuf[0], 0, sizeof(nBuf));
    MemReadCommPrm(F4, &nBuf[0], sizeof(nBuf));

    //判断是否内容全为0x00
    if(IsAllEqualVal(pBuf, sizeof(nBuf), 0))
    {
        // nCommSetMenu   ----> nBuf
        //主站号码
        nBuf[nIdx++] = 0x13;
        nBuf[nIdx++] = 0x80;
        nBuf[nIdx++] = 0x00;
        nBuf[nIdx++] = 0x00;
        nBuf[nIdx++] = 0x00;
        nBuf[nIdx++] = 0x0F;
        nBuf[nIdx++] = 0xFF;
        nBuf[nIdx++] = 0xFF;
        //短信中心
        nBuf[nIdx++] = 0x13;
        nBuf[nIdx++] = 0x80;
        nBuf[nIdx++] = 0x00;
        nBuf[nIdx++] = 0x00;
        nBuf[nIdx++] = 0x00;
        nBuf[nIdx++] = 0x0F;
        nBuf[nIdx++] = 0xFF;
        nBuf[nIdx]   = 0xFF;
    }
    //设置短信中心和主站号码有非BCD码的就转换成0
    nIdx = 0;                                            // 转换   nBuf 、 ip  ----> pData  主站号码
    memset(pData, 0, sizeof(nData));
    for (i = 0; i < 8; i++)
    {
         nLen =ValToAsciiSub((INT8U*)&pData[nIdx], pBuf[i], 2, 1, 2);
         nIdx += nLen;
         }
    for(i = 0 ; i < nIdx ; i++)
    {
      if(pData[i] == 'f')
         {
         pData[i] = '\0';
            break;
         }
    }
    GuiwinSetNullCfgParam(&pParamSet, "主站号码:");
    GuiwinSetEditCfgParam(&pParamSet, "    ", (INT8S*)&pData[0], 16);
    
    nIdx = 0;                                            // 转换   nBuf 、 port  ----> pData  短信中心
    pData = &nData[0];
    memset(pData, 0, sizeof(nData));
    for (i = 0; i < 8; i++)
{
         nLen =ValToAsciiSub((INT8U*)&pData[nIdx], pBuf[i + 8], 2, 1, 2);
         nIdx += nLen;
         }
    for(i = 0 ; i < nIdx ; i++)
    {
      if(pData[i] == 'f')
         {
         pData[i] = '\0';
            break;
         }
    }
    GuiwinSetNullCfgParam(&pParamSet, "短信中心:");
    GuiwinSetEditCfgParam(&pParamSet, "    ", (INT8S*)&pData[0], 16);
    
    pParamSet = &sParamSet[0];
    while(1)
    {
        if (GuiwinSetParamMenu("通信参数设置", pParamSet) >= 0)
        {
            nIdx = 0;
            memset(pBuf, 0, sizeof(nBuf));

            //主站号码
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[1].nEditBuf, GET_MIN(sizeof(pParamSet[1].nEditBuf), sizeof(nData) - 1));
            //判断输入是否为BCD码----非BCD码转换为0
            for(i = 0 ; i < GET_MIN(sizeof(pParamSet[1].nEditBuf), sizeof(nData) - 1); i++)
            {
               if(pData[i] > '9')
               {
                  pData[i] = '0';
               }
                if(pData[i] == '\0')
            {
                   pData[i] = 'f';
                }
            }
            StringToBuf((INT8U*)&pData[0], &nBuf[0], 16);

            //短信中心
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[3].nEditBuf, GET_MIN(sizeof(pParamSet[3].nEditBuf), sizeof(nData) - 1));
            //判断输入是否为BCD码----非BCD码转换为0
            for(i = 0 ; i < GET_MIN(sizeof(pParamSet[3].nEditBuf), sizeof(nData) - 1); i++)
            {
               if(pData[i] > '9')
               {
                  pData[i] = '0';
               }
                if(pData[i] == '\0')
            {
                   pData[i] = 'f';
                }
            }
            StringToBuf((INT8U*)&pData[0], &nBuf[8], 16);

            nRet = Guiwin1MboxCreate("确认","是否确认修改通讯参数?");
            if (KEY_ENTER != nRet)
{
                continue;    
            }
            MemWriteCommPrm(F4, &nBuf[0], sizeof(nBuf)); //设置通信参数
        }
        return ;
            }
        } 
    
//通信通道设置——主站IP设置
void MainIpAddrSet(void)
{
    PARAMSET		sParamSet[10] 	            = {0};
	pPARAMSET 		pParamSet		            = &sParamSet[0];
    INT8U           nBuf[50]                    = {0};
    INT8U*          pBuf                        =&nBuf[0];
    INT8U           nData[40]                   = {0};
    INT8U*          pData                        = &nData[0];
    INT32S          i                           = 0;
    INT8U           nIdx                        = 0;
    INT8U           nLen                        = 0;
    INT32S          nRet                        = 0;
    char*			pString 					= NULL;
    
    InterfacePassword();

    nIdx = 0;
    pParamSet = &sParamSet[0];
    memset(&sParamSet[0], 0, sizeof(sParamSet));
    
    memset(&nBuf[0], 0, sizeof(nBuf));
    MemReadCommPrm(F3, &nBuf[0], sizeof(nBuf));
    //判断是否内容全为0x00
    if(IsAllEqualVal(pBuf, sizeof(nBuf), 0))
    {
        // nCommSetMenu   ----> nBuf
        nBuf[nIdx++] = 10;
        nBuf[nIdx++] = 101;
        nBuf[nIdx++] = 1;
        nBuf[nIdx++] = 190;
        nBuf[nIdx++] = LOBYTE(5200);
        nBuf[nIdx++] = HIBYTE(5200);
        nBuf[nIdx++] = 10;
        nBuf[nIdx++] = 101;
        nBuf[nIdx++] = 1;
        nBuf[nIdx++] = 189;
        nBuf[nIdx++] = LOBYTE(5200);
        nBuf[nIdx++] = HIBYTE(5200);
        strcpy((char*)&nBuf[nIdx], (char*)"jsepc.js");
        }

    nIdx = 0;                                                 // 转换   nBuf 、 ip  ----> pData
    memset(pData, 0, sizeof(nData));
    for (i = 0; i < 4; i++)
    {
         nLen = ValToAscii(&pData[nIdx], nBuf[i], 3);
         nIdx += nLen;
         if (i != 3)
         {
             pData[nIdx++] = '.';
    }
}
    GuiwinSetNullCfgParam(&pParamSet, "主用IP:");
    GuiwinSetEditCfgParam(&pParamSet, "    ", (INT8S*)pData, 15);

    nIdx = 0;                                                 // 转换   nBuf 、 port  ----> pData
    memset(pData, 0, sizeof(nData));
    nLen = ValToAscii(&pData[nIdx], Buf2INT16U(&nBuf[4]), 5);
    nIdx += nLen;
    GuiwinSetEditCfgParam(&pParamSet, "主用端口:",(INT8S*)pData, 5);

    nIdx = 0;
    // 转换   nBuf 、 ip  ----> pData
    memset(pData, 0, sizeof(nData));
    for (i = 0; i < 4; i++)
{
         nLen = ValToAscii(&pData[nIdx], nBuf[6 + i], 3);
         nIdx += nLen;
         if (i != 3)
         {
             pData[nIdx++] = '.';
         }
    }
    GuiwinSetNullCfgParam(&pParamSet, "备用IP:");
    GuiwinSetEditCfgParam(&pParamSet, "    ", (INT8S*)pData, 15);

    nIdx = 0;                                                 // 转换   nBuf 、 port  ----> pData
    memset(pData, 0, sizeof(nData));
    nLen = ValToAscii(&pData[nIdx], Buf2INT16U(&nBuf[10]), 5);
    nIdx += nLen;
    GuiwinSetEditCfgParam(&pParamSet, "备用端口:", (INT8S*)pData, 5);
    
    for (i = 0; i < LEN_APN; i++)                                 // 转换   nBuf 、 apn  ----> pData
    {
        if (nBuf[i + 12] == 0)
        {
            break;
        }
    }
    nLen = i;
    nIdx = 0;
    memset(pData, 0, sizeof(nData));
    memcpy(&pData[nIdx], &nBuf[12], nLen);
    //判断如果pData[0]为'\0' ，给其赋默认值jsepc.js ，否则APN不显示，不可设
    #define    DEFAULT_APN		"jsepc.js"
    pString = (char*)DEFAULT_APN;
    if(0 == pData[0])
    {
      memcpy(&pData[0] ,pString ,strlen(pString));
    }
    nIdx += nLen;
    GuiwinSetNullCfgParam(&pParamSet, "APN:");
    GuiwinSetEditCfgParam(&pParamSet, "  ", (INT8S*)pData, LEN_APN);

    while(1)
    {
    pParamSet = &sParamSet[0];
        if (GuiwinSetParamMenu("通信参数设置", pParamSet) >= 0)
        {
            nIdx = 0;
            memset(pBuf, 0, sizeof(nBuf));

            //主用IP
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[1].nEditBuf, GET_MIN(sizeof(pParamSet[1].nEditBuf), sizeof(nData) - 1));
            pBuf[nIdx++] = atoi(strtok((char *)pData, "."));
            pBuf[nIdx++] = atoi(strtok(0, "."));
            pBuf[nIdx++] = atoi(strtok(0, "."));
            pBuf[nIdx++] = atoi(strtok(0, "."));

            //主用端口
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[2].nEditBuf, GET_MIN(sizeof(pParamSet[2].nEditBuf), sizeof(nData) - 1));
            pBuf[nIdx++] = (INT8U)( atoi((char *)pData) & 0x00FF );
            pBuf[nIdx++] = (INT8U)(( atoi((char *)pData) & 0xFF00 ) >> 8);

            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[4].nEditBuf, GET_MIN(sizeof(pParamSet[4].nEditBuf), sizeof(nData) - 1));
            pBuf[nIdx++] = atoi(strtok((char *)pData, "."));
            pBuf[nIdx++] = atoi(strtok(0, "."));
            pBuf[nIdx++] = atoi(strtok(0, "."));
            pBuf[nIdx++] = atoi(strtok(0, "."));

            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[5].nEditBuf, GET_MIN(sizeof(pParamSet[5].nEditBuf), sizeof(nData) - 1));
            pBuf[nIdx++] = (INT8U)( atoi((char *)pData) & 0x00FF );
            pBuf[nIdx++] = (INT8U)(( atoi((char *)pData) & 0xFF00 ) >> 8);

            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[7].nEditBuf, GET_MIN(sizeof(pParamSet[7].nEditBuf), sizeof(nData) - 1));
            for (i = 0; i < LEN_APN - strlen((char *)pData); i++)                    //后续补00H
            {
                pData[strlen((char *)pData) + i] = 0x00;
            }
            memcpy(&pBuf[nIdx], pData, LEN_APN);

            nRet = Guiwin1MboxCreate("确认","是否确认修改通讯参数?");
            if (KEY_ENTER != nRet)
            {
                continue;    
            }
            MemWriteCommPrm(F3, &nBuf[0], sizeof(nBuf)); //设置通信参数
        } 
					return ;
        }
    }

//通信通道设置——以太网参数设置
void EthernetParaSet(void)
{
    PARAMSET		sParamSet[12] 	            = {0};
	pPARAMSET 		pParamSet		            = &sParamSet[0];
    INT8U           nBuf[70]                   = {0};
    INT8U*          pBuf                        =&nBuf[0];
	INT8U           nBuf1[10]                   = {0};
    INT8U*          pBuf1                        =&nBuf1[0];
    INT8U           nData[25]                   = {0};
    INT8U*          pData                        = &nData[0];
    INT32S          i                           = 0;
    INT8U           nIdx                        = 0;
    INT8U           nLen                        = 0;
    INT32S          nRet                        = 0;
    const INT8S*	nLinkModMenu[]			={"TCP", "UDP", 0};
    const INT8S*	nWorkModMenu[]			={"混合模式", "客户机模式", "服务器模式", 0};
    INT8S           nWorkMod                = 0;
    INT8S           nLinkMod                = 0;

    InterfacePassword();

    nIdx = 0;
    pParamSet = &sParamSet[0];
    memset(&sParamSet[0], 0, sizeof(sParamSet));

    memset(&nBuf[0], 0, sizeof(nBuf));
    MemReadCommPrm(F7, &nBuf[0], sizeof(nBuf));
    if(IsAllEqualVal(pBuf, sizeof(nBuf), 0))
		{
        // nCommSetMenu   ----> nBuf
        nBuf[nIdx++] = 0;
        nBuf[nIdx++] = 0;
        nBuf[nIdx++] = 0;
        nBuf[nIdx++] = 0;                 //终端IP
        nBuf[nIdx++] = 0;
        nBuf[nIdx++] = 0;
        nBuf[nIdx++] = 0;
        nBuf[nIdx++] = 0;               //子网掩码
        nBuf[nIdx++] = 0;
        nBuf[nIdx++] = 0;
        nBuf[nIdx++] = 0;
        nBuf[nIdx++] = 0;               //网关地址
        nBuf[60] = LOBYTE(5100);
        nBuf[61] = HIBYTE(5100);        //侦听端口
        }
    nIdx = 0;                                              // 转换   nBuf 、 ip  ----> pData
    memset(pData, 0, sizeof(nData));
    for (i = 0; i < 4; i++)
    {
         nLen = ValToAscii(&pData[nIdx], nBuf[i + 0], 3);
         nIdx += nLen;
         if (i != 3)
         {
             pData[nIdx++] = '.';
    }
}
    GuiwinSetNullCfgParam(&pParamSet, "终端IP:");
    GuiwinSetEditCfgParam(&pParamSet, "    ", (INT8S*)pData, 15);

    nIdx = 0;                                              // 转换   nBuf 、 ip  ----> pData
    memset(pData, 0, sizeof(nData));
    for (i = 0; i < 4; i++)
    {
         nLen = ValToAscii(&pData[nIdx], nBuf[i + 4], 3);
         nIdx += nLen;
         if (i != 3)
{
             pData[nIdx++] = '.';
	}
}
    GuiwinSetNullCfgParam(&pParamSet, "子网掩码:");
    GuiwinSetEditCfgParam(&pParamSet, "    ", (INT8S*)pData, 15);

    nIdx = 0;                                              // 转换   nBuf 、 ip  ----> pData
    memset(pData, 0, sizeof(nData));
    for (i = 0; i < 4; i++)
	{
         nLen = ValToAscii(&pData[nIdx], nBuf[i + 8], 3);
         nIdx += nLen;
         if (i != 3)
		{
             pData[nIdx++] = '.';
         }
    }
    GuiwinSetNullCfgParam(&pParamSet, "网关地址:");
    GuiwinSetEditCfgParam(&pParamSet, "    ", (INT8S*)pData, 16);

    nIdx = 0;                                              // 转换   nBuf 、 port  ----> pData
    memset(pData, 0, sizeof(nData));
    nLen = ValToAscii(&pData[nIdx], Buf2INT16U(&nBuf[60]), 5);
    nIdx += nLen;
    GuiwinSetEditCfgParam(&pParamSet, "侦听端口:",(INT8S*)pData, 5);

    memset(&nBuf1[0], 0, sizeof(nBuf1));
    nRet = MemReadCommPrm(F8, &nBuf1[0], sizeof(nBuf1));

    nIdx = 0;                                              // 转换   nBuf 、 ip  ----> pData
    memset(pData, 0, sizeof(nData));
    if (TestBufBit(&nBuf1[0], 1, 7))
			{
        nLinkMod = 1;
    }
    else
    {
        nLinkMod = 0;
			}
    nWorkMod = (nBuf1[0] >> 4) & 0x03;
    GuiwinSetSelectCfgParam(&pParamSet, "连接模式:", (INT8S*)&nLinkMod, nLinkModMenu);
    GuiwinSetSelectCfgParam(&pParamSet, "工作模式:", (INT8S*)&nWorkMod, nWorkModMenu);

	pParamSet = &sParamSet[0];
    while (1)
    {
        if (GuiwinSetParamMenu("通信参数设置", pParamSet) >= 0)
			{
            nIdx = 0;
            memset(pBuf, 0, sizeof(nBuf));
            MemReadCommPrm(F7, &nBuf[0], sizeof(nBuf));

            //终端IP
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[1].nEditBuf, GET_MIN(sizeof(pParamSet[1].nEditBuf), sizeof(nData) - 1));
            pBuf[nIdx++] = atoi(strtok((char *)pData, "."));
            pBuf[nIdx++] = atoi(strtok(0, "."));
            pBuf[nIdx++] = atoi(strtok(0, "."));
            pBuf[nIdx++] = atoi(strtok(0, "."));

            //子网掩码
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[3].nEditBuf, GET_MIN(sizeof(pParamSet[3].nEditBuf), sizeof(nData) - 1));
            pBuf[nIdx++] = atoi(strtok((char *)pData, "."));
            pBuf[nIdx++] = atoi(strtok(0, "."));
            pBuf[nIdx++] = atoi(strtok(0, "."));
            pBuf[nIdx++] = atoi(strtok(0, "."));

            //网关
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[5].nEditBuf, GET_MIN(sizeof(pParamSet[5].nEditBuf), sizeof(nData) - 1));
            pBuf[nIdx++] = atoi(strtok((char *)pData, "."));
            pBuf[nIdx++] = atoi(strtok(0, "."));
            pBuf[nIdx++] = atoi(strtok(0, "."));
            pBuf[nIdx++] = atoi(strtok(0, "."));

            //侦听端口
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[6].nEditBuf, GET_MIN(sizeof(pParamSet[6].nEditBuf), sizeof(nData) - 1));
            pBuf[60] = (INT8U)( atoi((char *)pData) & 0x00FF );
            pBuf[61] = (INT8U)(( atoi((char *)pData) & 0xFF00 ) >> 8);

            pBuf1 =&nBuf1[0];
			memset(&pBuf1[0] , 0 ,sizeof(nBuf1));
            MemReadCommPrm(F8, &nBuf1[0], sizeof(nBuf1));
            //模式
            pBuf1[0] &= 0x03;
            if (nLinkMod)
				{
                pBuf1[0] |= 0x80;
				}
            pBuf1[0] |= (nWorkMod << 4);

            nRet = Guiwin1MboxCreate("确认","是否确认修改通讯参数?");
            if (KEY_ENTER != nRet)
			{
                 continue;
			}
            MemWriteCommPrm(F7, &nBuf[0], sizeof(nBuf)); //设置通信参数
            MemWriteCommPrm(F8, &nBuf1[0], sizeof(nBuf1));
            return ;
		}
        else
        {
        return ;
	}
}
}

//电表参数设置
INT32S Meter_Param_Set(INT16U nMearpoint , pMEM_METER_PRM pMeterVld)
{
    PARAMSET		sParamSet[30] 	            = {0};
	pPARAMSET		     pParamSet					   = &sParamSet[0];
    INT8U           nBuf[40]                    = {0};
    INT8U*          pBuf                        =&nBuf[0]; 
    INT8U           nData[15]                   = {0};
    INT8U*          pData                        = &nData[0];
    INT32S          i                           = 0;
    INT8U           nIdx                        = 0;
    INT8U           nLen                        = 0;
    INT32S          nRet                        = 0;
    const INT32U   nBuadRate[8]                  ={0, 600, 1200, 2400, 4800, 7200, 9600, 19200};
    const INT8S*	nBaudCfg[]			        ={"0","600","1200","2400","4800","7200","9600","19200",0};
    const INT8S*	nCommPort[]			        ={"未知","交采","RS4851","RS4852","RS4853","级联RS485","载波口",0};
    const INT8S*	CommProtocol[]              ={"无效","DL/T645-97","交流采样协议","DL/T645-07","窄带低压载波",0};
    const INT8S*	UserTypes[]                 = {"默认用户","大型专变用户","中小型专变用户","三相工商业用户","单相工商业用户","居民用户","配变计量点",0};
    const INT8S*	nReadMeter[]			    ={"否","是",0};
    INT8U           nBaudIdx                    = 0;
    INT8U           nProt                       = 0;
    INT8U           nCommProtol1                = 0;
    INT8U           nUserType                   = 0;
    INT8U           nUserLittle                 = 0;
    INT32U          nBaud                       = 0;
    INT8U           nComm                       = 0;
    INT8U           nAddr[6]                    ={0};
    INT8U           nExBuf[15]                  ={0};
    INT8U*          pExtBuf                     = &nExBuf[0];
    INT8U           nChoice                     = 0;
    INT8U           PartEnum[5]                 ={0};

    memset(&sParamSet[0], 0, sizeof(sParamSet));
    memset(pMeterVld , 0 ,sizeof(MEM_METER_PRM));

    memset(pBuf, 0, sizeof(nBuf));
	MemReadMeterPrm(nMearpoint, F10, &nBuf[0], sizeof(nBuf));

    while (1)
    {
    nIdx = 0;                                      // 转换   nBuf 、 ip  ----> pData （测量点序号）
    memset(pData, 0, sizeof(nData));
    nLen = ValToAscii(&pData[nIdx], Buf2INT16U(&pBuf[1]), 4);
         nIdx += nLen;
    GuiwinSetEditCfgParam(&pParamSet, "电表序号:   ",(INT8S*)pData, 4);

    nIdx = 0;                                      // 转换   nBuf 、 ip  ----> pData  （测量点号）
    memset(pData, 0, sizeof(nData));
    nLen = ValToAscii(&pData[nIdx], Buf2INT16U(&pBuf[3]), 4);
         nIdx += nLen;
    GuiwinSetEditCfgParam(&pParamSet, "测量点号:   ",(INT8S*)pData, 4);

    nProt = (pBuf[5]&0x1F);                                 // 转换   nBuf 、 ip  ----> pData  (端口号)
    if((nProt == 30)||(nProt == 31))
			{
        nProt -=25;
    }
    GuiwinSetSelectCfgParam(&pParamSet,"通讯端口: ",(INT8S*)&nProt, nCommPort);

    nIdx = 0;
    // 转换   nBuf 、 ip  ----> pData  (通讯速率)
    for(i = 0 ; i <= 7; i++)
    {
      if(Buf2Long(&pBuf[6], 4) == nBuadRate[i])
      {
        nBaudIdx = i;
        break;
      }
    }
    GuiwinSetSelectCfgParam(&pParamSet,"通信速率: ",(INT8S*)&nBaudIdx, nBaudCfg);

    nCommProtol1 = pBuf[10];                          // 转换   nBuf 、 ip  ----> pData  (通讯协议)
    if((nCommProtol1 == 30)||(nCommProtol1 == 31))
			{
        nCommProtol1 -=27;
    }
    GuiwinSetNullCfgParam(&pParamSet, "通讯协议:");
    GuiwinSetSelectCfgParam(&pParamSet, "          ", (INT8S*)&nCommProtol1, CommProtocol);

    nIdx = 0;                                   // 转换   nBuf 、 ip  ----> pData  （通讯地址）
    memset(pData, 0, sizeof(nData));
    for (i = 0; i < 6; i++)
    {
         nLen =ValToAsciiSub((INT8U*)&pData[nIdx], pBuf[16 - i], 2, 1, 2);
         nIdx += nLen;
    }
    GuiwinSetEditCfgParam(&pParamSet,"地址:",(INT8S*)pData, 12);

    nIdx = 0;                                  // 转换   nBuf 、 ip  ----> pData （采集器）
    memset(pData, 0, sizeof(nData));
    for (i = 0; i < 6; i++)
    {
         nLen =ValToAsciiSub((INT8U*)&pData[nIdx], pBuf[31 - i], 2, 1, 2);
         nIdx += nLen;
    }
    GuiwinSetEditCfgParam(&pParamSet,"采集器: ",(INT8S*)pData, 12);

    nUserType = pBuf[32];                 // 转换   nBuf 、 ip  ----> pData （用户大类）
        GuiwinSetNullCfgParam(&pParamSet, "用户大类:");
        GuiwinSetSelectCfgParam(&pParamSet,"",(INT8S*)&nUserType, UserTypes);

        // 转换   nBuf 、 ip  ----> pData （用户小类)
        for(i = 0 ; i < ARR_SIZE(ArryName) - 1 ; i++)
        {
           GuiwinSetNullCfgParam(&pParamSet, (char*)ArryName[i]);
           GuiwinSetSelectCfgParam(&pParamSet,"",(INT8S*)&PartEnum[i], ArryCollect[i]);
        }
    nIdx = 0;
    memset(pExtBuf , 0 ,sizeof(nExBuf));
    memset(pData, 0, sizeof(nData));
    MemReadMeterPrm(nMearpoint, F25, &pExtBuf[0], sizeof(nExBuf));

    nLen = ValToAscii(&pData[nIdx], Buf2INT16U(&pExtBuf[0]), 4);
    nIdx += nLen;
    GuiwinSetEditCfgParam(&pParamSet,"电压变比: ",(INT8S*)pData, 4);

    nIdx = 0;
    memset(pData, 0, sizeof(nData));
    nLen = ValToAscii(&pData[nIdx], Buf2INT16U(&pExtBuf[2]), 4);
    nIdx += nLen;
    GuiwinSetEditCfgParam(&pParamSet,"电流变比: ",(INT8S*)pData, 4);

        GuiwinSetSelectCfgParam(&pParamSet,"是否投抄:",(INT8S*)&nChoice, nReadMeter);

	pParamSet = &sParamSet[0];

        if (GuiwinSetParamMenu("电表参数设置", pParamSet) >= 0)
		{
            nIdx = 0;
            pBuf =&nBuf[0];
            memset(pBuf, 0, sizeof(nBuf));
            MemReadMeterPrm(nMearpoint,  F10, &nBuf[0], sizeof(nBuf));

            //电表序号
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[0].nEditBuf, GET_MIN(sizeof(pParamSet[0].nEditBuf), sizeof(nData) - 1));
            //是否投抄
            pBuf[0] = nChoice;

            pBuf[1] = (INT8U)( atoi((char *)pData) & 0x00FF );
            pBuf[2] = (INT8U)(( atoi((char *)pData) & 0xFF00 ) >> 8);

            //测量点号
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[1].nEditBuf, GET_MIN(sizeof(pParamSet[1].nEditBuf), sizeof(nData) - 1));

            pBuf[3] = (INT8U)( atoi((char *)pData) & 0x00FF );
            pBuf[4] = (INT8U)(( atoi((char *)pData) & 0xFF00 ) >> 8);

            //测量点端口号
            pBuf[5] &= 0xE0;
            if((nProt == 5)||(nProt == 6))
            {
               nProt +=25;
            }
            pBuf[5] |= nProt;
            nComm = nProt;

            //通讯速率
            nIdx = 0;
               memset(pData, 0, sizeof(nData));
            nBaud = nBuadRate[nBaudIdx];
            pBuf[5] &= 0x1F;
            //pBuf[5] |= nBaudIdx << 5 ;
            Long2Buf(&pBuf[6], nBaud, 4);

            //通讯协议
            pBuf[10] = 0;
            if((nCommProtol1 == 3)||(nCommProtol1 == 4))
            {
               pBuf[10] |= (nCommProtol1 + 27);
            }
            else
            {
               pBuf[10] |= nCommProtol1;
            }

            //通讯地址
            memset(pData, 0, sizeof(nData));
            memcpy(&pData[0], pParamSet[6].nEditBuf, GET_MIN(sizeof(pParamSet[6].nEditBuf), sizeof(nData) - 1));
            StringToBufSwap((INT8U*)&pData[0], &pBuf[11], 12);
            memcpy(&nAddr[0], &pBuf[11], 6);

            //采集器地址
            memset(pData, 0, sizeof(nData));
            memcpy(&pData[0], pParamSet[7].nEditBuf, GET_MIN(sizeof(pParamSet[7].nEditBuf), sizeof(nData) - 1));
            StringToBufSwap((INT8U*)&pData[0], &pBuf[26], 12);

            //用户大类
            pBuf[32] = nUserType;
            //用户小类
            for(i = 0; i < 5 ; i++)
            {
                if(PartEnum[i] > 0)
                {
                  pBuf[33] = PartEnum[i];
                  break;
                }
            }
            memcpy(pMeterVld ,&nBuf[0], sizeof(MEM_METER_PRM));
            if (!IsVldMeterPrm(pMeterVld))
            {
                Guiwin1MboxCreate("确认","核查参数有效性");
                continue;
            }
            if (IsRedupMtAddr(pMeterVld))
            {
                Guiwin1MboxCreate("确认","表计地址重复");
                continue;
            }

            //电压变比
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[20].nEditBuf, GET_MIN(sizeof(pParamSet[20].nEditBuf), sizeof(nData) - 1));
            pExtBuf[0] = (INT8U)( atoi((char *)pData) & 0x00FF );
            pExtBuf[1] = (INT8U)(( atoi((char *)pData) & 0xFF00 ) >> 8);

            //电流变比
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[21].nEditBuf, GET_MIN(sizeof(pParamSet[21].nEditBuf), sizeof(nData) - 1));
            pExtBuf[2] = (INT8U)( atoi((char *)pData) & 0x00FF );
            pExtBuf[3] = (INT8U)(( atoi((char *)pData) & 0xFF00 ) >> 8);

            nRet = Guiwin1MboxCreate("确认","是否确认修改通讯参数?");
            if (KEY_ENTER != nRet)
			{
                 continue;
            }   
            if (nComm == 0x1F)
            {
                //载波表时需要判断
                if (g_nCarrZbSelFlg == TYPE_CARR)
                {
                    //if (g_nCarrModType != CARR_MOD_DX)
                    {
                        if (g_nCarrInitFlg == 0)
                        {
                            continue;
                        }
                        //if (nComm == 0x1F)
                        {
                            CarrRouteAdd(&nAddr[0], 2);
                        }
                    }
                }
            }
            
            //交采地址
            if (nMearpoint == 1)
            {
                //MemWriteAcPrm(F19, &pMeterVld->nAddr[0], LEN_METER_ADDR);
            }
            MemWriteMeterPrm(nMearpoint, F10, pMeterVld, sizeof(nBuf));//设置表计信息
            MemWriteMeterPrm(nMearpoint, F25, pExtBuf, sizeof(nExBuf));
            MeterMpMap(1);
            if (nComm == 0x1F)
            {
                if (g_nCarrZbSelFlg != TYPE_CARR)   //载波抄表配置后不立即抄表，反正影响载波节点添加
                {
                    RdMeterSemProc();
                }
            }
            else
            {
                RdMeterSemProc();
            }
            
            return 1;
        }
        else
        {
           return -1;
        }
    }
}


//读电表参数
void MeterParamQuery(void)
{
    PARAMSET		     sParamSet[5] 	                 = {0};
	pPARAMSET 		     pParamSet		                 = &sParamSet[0];
    const INT8S         nMeterCfgMenu[5]                = "0001";
    INT8S                nTempMenu[5]                    = {0};
	INT16U               nSeqArry[]                      = {0};
	INT16U               nSeq                            = 1;
    INT16S               nMp                             = 1;
    INT16U               nTmpMp                          = 1;
	INT16S               nValidMp                        = 0;
    INT32S               nFlg                        = 0;
    MEM_METER_PRM	     sMeterVld;
    pMEM_METER_PRM	     pMeterVld	                 = &sMeterVld;

    memcpy(&nTempMenu[0],&nMeterCfgMenu[0] ,sizeof(nMeterCfgMenu));

    GuiwinSetNullCfgParam(&pParamSet, NULL);
    GuiwinSetNullCfgParam(&pParamSet, NULL);
    GuiwinSetEditCfgParam(&pParamSet, "        ",&nTempMenu[0], 5);

    pParamSet = &sParamSet[0];
	while(1)
	{
	    if (GuiwinSetParamMenu("请输入测量点号", pParamSet) >= 0)
		{
	        memcpy(&nTempMenu[0], pParamSet[2].nEditBuf, GET_MIN(sizeof(pParamSet[2].nEditBuf), sizeof(nTempMenu) - 1));

	        if((atoi((char *)nTempMenu)==0 )||(atoi((char *)nTempMenu)>MAX_MP))
			{
	            Guiwin1MboxCreate("确认", "输入无效,范围:(1-999)");
				continue;
        }
            nMp = atoi((char *)nTempMenu);

            while(1)
            {
            //将测量点按顺序排列，对应出相应的表计序号
            for(nSeq = 1; nSeq < MAX_METER_NUM ; nSeq++)
            {
                nValidMp = ValidMpNum();
                memset(pMeterVld , 0 ,sizeof(MEM_METER_PRM));
                MemReadMeterPrm(nSeq , F10 , pMeterVld, sizeof(MEM_METER_PRM));
                nTmpMp = Buf2INT16U(&pMeterVld->nMp[0]);    //读取表计序号匹配的测量点号

                if(nMp == nTmpMp)    //当判断出输入的测量点和求出测量点相同，即得出该表计序列号
                {
                     nSeqArry[nMp] =(nSeq = Buf2INT16U(&pMeterVld->nSeq[0]));
                     Read_Meter_Param(nSeqArry ,nMp , pMeterVld);
                     break;
                }
            }
                //测量点巡检 ，按键处理
                nFlg = KeyProc1(&nMp ,&nValidMp);
            if (nFlg)
            {
                   break;
                }
            }
            if (nFlg)
            {
               return;
            }
	    }
		else
		{
				return ;
			}
	}
			}

//电表参数设置
void MeterParaSet(void)
{
    PARAMSET		sParamSet[5] 	= {0};
	pPARAMSET 		pParamSet		= &sParamSet[0];
    const INT8S    nMeterCfgMenu[5]  = "0001";
    INT8S           nTempMenu[5]      = {0};
    INT16S               nMp                             = 0;
	INT16S               nValidMp                        = 0;
    INT32S               nFlg                            = 0;
    MEM_METER_PRM	     sMeterVld;
    pMEM_METER_PRM	     pMeterVld	                     = &sMeterVld;

    InterfacePassword();

    memcpy(&nTempMenu[0],&nMeterCfgMenu[0] ,sizeof(nMeterCfgMenu));

    GuiwinSetNullCfgParam(&pParamSet, NULL);
    GuiwinSetNullCfgParam(&pParamSet, NULL);
    GuiwinSetEditCfgParam(&pParamSet, "        ",&nTempMenu[0], 4);

    pParamSet = &sParamSet[0];
	while(1)
	{
    if (GuiwinSetParamMenu("请输入测量点号", pParamSet) >= 0)
			{
        memcpy(&nTempMenu[0], pParamSet[2].nEditBuf, GET_MIN(sizeof(pParamSet[2].nEditBuf), sizeof(nTempMenu) - 1));

        if((atoi((char *)nTempMenu)==0 )||(atoi((char *)nTempMenu)>MAX_MP ))
		{
                Guiwin1MboxCreate("确认", "输入无效,范围:(1-999)");
				return ;
			}
            nMp = atoi((char *)nTempMenu);
            nValidMp = MAX_MP ;       //所有测量点均可设置


            //测量点巡检 ，按键处理
            while(1)
                    {
                //测量点的设置
                nFlg = Meter_Param_Set(nMp ,pMeterVld);
                if (nFlg)
                        {
                   break;
                        }
    }
            if(nFlg)
            {
               continue;
            }
        }
        else
        {
            return ;
        }
     }
}



//终端时间设置
void ConCerTimeSet(void)
{
    STD_TIME		        sTime;
	pSTD_TIME		        pTime = &sTime;
    PARAMSET		sParamSet[5] 	            = {0};
	pPARAMSET		        pParamSet				    = &sParamSet[0];
	const INT8S		c_nTimeMenuBak[2][9]		= {{"15-07-08"}, {"16:16:16"}};
	INT8S			nTimeMenu[2][9]			    = {0};
    INT32S                  nRet                        = 0;

    InterfacePassword();

	memset(pTime, 0, sizeof(STD_TIME));
	RTCDateTimeRead(pTime);
	pParamSet = &sParamSet[0];
	memcpy(&nTimeMenu[0][0], &c_nTimeMenuBak[0][0], sizeof(nTimeMenu));
	ValToAsciiSub((INT8U*)&nTimeMenu[0][0], pTime->nYear, 2, 1, 2);
	ValToAsciiSub((INT8U*)&nTimeMenu[0][3], pTime->nMon, 2, 1, 2);
    ValToAsciiSub((INT8U*)&nTimeMenu[0][6], pTime->nDay, 2, 1, 2);
    ValToAsciiSub((INT8U*)&nTimeMenu[1][0], pTime->nHour, 2, 1, 2);
    ValToAsciiSub((INT8U*)&nTimeMenu[1][3], pTime->nMin, 2, 1, 2);
    ValToAsciiSub((INT8U*)&nTimeMenu[1][6], pTime->nSec, 2, 1, 2);

	GuiwinSetNullCfgParam(&pParamSet, NULL);
	GuiwinSetEditCfgParam(&pParamSet, "     ", nTimeMenu[0], 8);
    GuiwinSetNullCfgParam(&pParamSet, NULL);
	GuiwinSetEditCfgParam(&pParamSet, "     ", nTimeMenu[1], 8);

	pParamSet = &sParamSet[0];
    while (1)
    {
        if (GuiwinSetParamMenu("终端时钟", pParamSet) >= 0)
		{
            memcpy(&nTimeMenu[0][0], pParamSet[1].nEditBuf, GET_MIN(sizeof(pParamSet[1].nEditBuf), sizeof(nTimeMenu[0]) - 1));
            memcpy(&nTimeMenu[1][0], pParamSet[3].nEditBuf, GET_MIN(sizeof(pParamSet[3].nEditBuf), sizeof(nTimeMenu[1]) - 1));
            if ((nTimeMenu[0][2] == '-') && (nTimeMenu[0][5] == '-') && (nTimeMenu[1][2] == ':') && (nTimeMenu[1][5] == ':'))
			{
				pTime->nYear = INT8U_TO_BCD(atoi((char *)&nTimeMenu[0][0]));
				pTime->nMon = INT8U_TO_BCD(atoi((char *)&nTimeMenu[0][3]));
                pTime->nDay = INT8U_TO_BCD(atoi((char *)&nTimeMenu[0][6]));
                pTime->nHour = INT8U_TO_BCD(atoi((char *)&nTimeMenu[1][0]));
                pTime->nMin = INT8U_TO_BCD(atoi((char *)&nTimeMenu[1][3]));
                pTime->nSec = INT8U_TO_BCD(atoi((char *)&nTimeMenu[1][6]));
			}
            SyncSysClk(pTime);
            nRet = Guiwin1MboxCreate("确认","设置成功");
            if (KEY_ENTER == nRet)
            {
                return;
            }
		}
        else
        {
		return ;
	}
}
}

//终端编号设置
void ConcerNumSet(void)
{
    PARAMSET		sParamSet[12] 	            = {0};
	pPARAMSET 		pParamSet		            = &sParamSet[0];
    INT8U           nBuf[10]                     = {0};
    INT8U*          pBuf                        =&nBuf[0];
    INT8U           nData[25]                   = {0};
    INT8U*          pData                        = &nData[0];
    INT8U           nTmpBuf[10]                 = {0};
    INT8U           nSetFLg                     = 0;
    INT32S          i                           = 0;
    INT8U           nIdx                        = 0;
    INT8U           nLen                        = 0;
    INT32S          nRet                        = 0;
    INT8S*          pTmpVal                     = NULL;

    InterfacePassword();

    nIdx = 0;
    pParamSet = &sParamSet[0];
    memset(&pBuf[0], 0, sizeof(nBuf));
    nRet = MemReadCommPrm(F94, &nBuf[0], sizeof(nBuf));

    nIdx = 0;                                             // 转换   nBuf 、 port  ----> pData
    memset(pData, 0, sizeof(nData));
    for (i = 0; i < 2; i++)
{
         nLen =ValToAsciiSub((INT8U*)&pData[nIdx], nBuf[1 - i], 2, 1, 2);
         nIdx += nLen;
}
    GuiwinSetNullCfgParam(&pParamSet, NULL);
    GuiwinSetEditCfgParam(&pParamSet, "行政区码:    ",(INT8S*)pData, 4);

    nIdx = 0;                                             // 转换   nBuf 、 port  ----> pData
    memset(pData, 0, sizeof(nData));
    for (i = 0; i < 2; i++)
{
         nLen =ValToAsciiSub((INT8U*)&pData[nIdx], nBuf[3 - i], 2, 1, 2);
         nIdx += nLen;
}
    GuiwinSetNullCfgParam(&pParamSet, NULL);
    GuiwinSetNullCfgParam(&pParamSet, "终端地址:");
    GuiwinSetEditCfgParam(&pParamSet, "    (十六进制)", (INT8S*)pData, 4);

    nIdx = 0;                                             // 转换   nBuf 、 port  ----> pData
    memset(pData, 0, sizeof(nData));
    nLen = ValToAscii(&pData[nIdx],Buf2INT16U(&nBuf[2]) , 5);
    nIdx += nLen;
    GuiwinSetEditCfgParam(&pParamSet, "      (十进制)", (INT8S*)pData, 5);

    pParamSet = &sParamSet[0];
    while(1)
{
        if (GuiwinSetParamMenu("终端编号", pParamSet) >= 0)
{
            nIdx = 0;
            memset(pBuf, 0, sizeof(nBuf));
            MemReadCommPrm(F94, &nBuf[0], sizeof(nBuf));

            //行政区码
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[1].nEditBuf, GET_MIN(sizeof(pParamSet[1].nEditBuf), sizeof(nData) - 1));
            StringToBufSwap((INT8U*)&pData[0], &pBuf[0], 4);

            //终端地址(十六进制)
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[4].nEditBuf, GET_MIN(sizeof(pParamSet[4].nEditBuf), sizeof(nData) - 1));
            StringToBufSwap((INT8U*)&pData[0], &nTmpBuf[0], 4);
            if(memcmp(nTmpBuf , &pBuf[2] ,2) != 0)
            {
            StringToBufSwap((INT8U*)&pData[0], &pBuf[2], 4);
            }
            else
            {
            //终端地址（十进制）
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[5].nEditBuf, GET_MIN(sizeof(pParamSet[5].nEditBuf), sizeof(nData) - 1));
            pBuf[2] = (INT8U)( atoi((char *)pData) & 0x00FF );
            pBuf[3] = (INT8U)(( atoi((char *)pData) & 0xFF00 ) >> 8);
            }

            nRet = Guiwin1MboxCreate("确认","是否确认修改通讯参数?");
            if (KEY_ENTER != nRet)
{
                 continue;
}
            MemWriteCommPrm(F94, &nBuf[0], sizeof(nBuf)); //设置通信参数
}
        return ;
}
}

//重启抄表
void  RestartRdMeter(void)
{
    SetRdMeterFlagsAllMp(0);
    RdMeterSemProc();
    OSTimeDlyHMSM(0, 0, 1, 0);
    GuiwinDynMboxCreate(0, "重启抄表..."); 
    Guiwin1MboxCreate("确认","设置成功");
}
//停止抄表
void StopRdMeter(void)
{
    ClrRdMeterFlagsAllMp(0);
    OSTimeDlyHMSM(0, 0, 1, 0);
    GuiwinDynMboxCreate(0, "停止抄表..."); 
    Guiwin1MboxCreate("确认","设置成功");
}
//恢复抄表
void ReturnRdMeter(void)
{
    SetRdMeterFlagsAllMp(0);
    RdMeterSemProc();
    GuiwinDynMboxCreate(0, "恢复抄表..."); 
    OSTimeDlyHMSM(0, 0, 1, 0);
    Guiwin1MboxCreate("确认","设置成功");
}
//激活连接
void ActivateConnect(void)
{
    INT8U   nLinkReq = 0;
    nLinkReq = 1;
    MemWriteRamXmpData(F9, 0, &nLinkReq, sizeof(nLinkReq));
    OSTimeDlyHMSM(0, 0, 1, 0);
    Guiwin1MboxCreate("确认","设置成功");
}
//断开连接
void Disconnect(void)
{
    INT8U   nLinkReq = 0;
    nLinkReq = 2;
    MemWriteRamXmpData(F9, 0, &nLinkReq, sizeof(nLinkReq));
    OSTimeDlyHMSM(0, 0, 1, 0);
    Guiwin1MboxCreate("确认","设置成功");
}
//节点维护
void NodeMaintain(void)
{

}
//USB 功能
void USBFunction(void)
{

}
//硬件复位
void HardWareReset(void)
{
    GuiwinDynMboxCreate(0, "硬件复位..."); 
    ResetDelayHardInit(1000);
}
//数据清零
void DataReset(void)
{
    GuiwinDynMboxCreate(0, "数据初始化..."); 
    g_nInitType = MEM_INIT_DATA;
    //MemDataInit();
    OSTimeDlyHMSM(0 ,0 ,5, 0);
    
    Guiwin1MboxCreate("确认","设置成功");
}
//参数清零
void ParaReset(void)
{
    GuiwinDynMboxCreate(0, "参数初始化..."); 
    g_nInitType = MEM_INIT_FACTORY;
    //MemFactoryInit();
    OSTimeDlyHMSM(0 ,0 ,5, 0);
    
    Guiwin1MboxCreate("确认","设置成功");
}

#if 0
//进入参数设置与查看输入验证密码
void  ParamInterfaceSet (void)
{
    PARAMSET		sParamSet[1]					= {0};
	pPARAMSET		pParamSet						= &sParamSet[0];
    INT8U           nPassWordBuf[3]                 = {0};
    INT8S           nConCerPWEditBuf[A_MENU_LEN]    = {"000000"};

    ValToAsciiSub((INT8U*)&nConCerPWEditBuf[0], nPassWordBuf[2], 2, 1, 2);
    ValToAsciiSub((INT8U*)&nConCerPWEditBuf[2], nPassWordBuf[1], 2, 1, 2);
    ValToAsciiSub((INT8U*)&nConCerPWEditBuf[4], nPassWordBuf[0], 2, 1, 2);

    GuiwinSetEditCfgParam(&pParamSet, "     ", &nConCerPWEditBuf[0], 6);
    pParamSet = &sParamSet[0];

	while(1)
	{
         if (GuiwinSetParamMenu("请输入密码...", pParamSet) >= 0)
{

            memcpy(&nConCerPWEditBuf[0], pParamSet[0].nEditBuf, GET_MIN(sizeof(pParamSet[0].nEditBuf), sizeof(nConCerPWEditBuf) - 1));
            StringToBufSwap((INT8U*)&nConCerPWEditBuf[0], &nPassWordBuf[0], 6);
        }
        else
        {
            Guiwin1MboxCreate("返回","密码不正确");
            return;
        }
    }
    return;
}

//INT8U    nPassWordBuf[3] = {0};
//界面密码设置
void  InterfacePassword(void)
{
    PARAMSET		sParamSet[1]					= {0};
	pPARAMSET		pParamSet						= &sParamSet[0];
    INT8S           nConCerPWEditBuf[A_MENU_LEN]   = {"000000"};

    //ValToAsciiSub((INT8U*)&nConCerPWEditBuf[0], nPassWordBuf[2], 2, 1, 2);
    //ValToAsciiSub((INT8U*)&nConCerPWEditBuf[2], nPassWordBuf[1], 2, 1, 2);
    //ValToAsciiSub((INT8U*)&nConCerPWEditBuf[4], nPassWordBuf[0], 2, 1, 2);

    GuiwinSetEditCfgParam(&pParamSet, "请输入密码:", &nConCerPWEditBuf[0], 6);
    pParamSet = &sParamSet[0];

	while(1)
	{
         if (GuiwinSetParamMenu("密码", pParamSet) >= 0)
        {

            memcpy(&nConCerPWEditBuf[0], pParamSet[0].nEditBuf, GET_MIN(sizeof(pParamSet[0].nEditBuf), sizeof(nConCerPWEditBuf) - 1));
            //StringToBufSwap((INT8U*)&nConCerPWEditBuf[0], &nPassWordBuf[0], 6);

            ParamInterfaceSet();
        }
        else
        {
            Guiwin1MboxCreate("返回","密码不正确");
            return;
        }
    }
    return;
}

#endif

//设置通讯参数
void BaseCommInfoCfg( void* pData)
{
	PARAMSET		sParamSet[10]			= {0};
	pPARAMSET		pParamSet				= &sParamSet[0];
	const INT8S*	c_pShowHexParam[]		= {"否", "是", 0};
	const INT8S*	c_pCtIrParam[]			= {"是", "否", 0};

	GuiwinSetSelectCfgParam(&pParamSet, "报文显示:", &g_nShowHex, c_pShowHexParam);
	GuiwinSetEditCfgParam(&pParamSet, "超时(ms):", &g_nTimeOut[0], sizeof(g_nTimeOut));
	GuiwinSetEditCfgParam(&pParamSet, "远程升级超时(s):", &g_nUpGradeTimeOut[0], sizeof(g_nUpGradeTimeOut));
	GuiwinSetSelectCfgParam(&pParamSet, "终端通信是否红外:", &g_nCtIr, c_pCtIrParam);

	//调用设置菜单
	pParamSet = &sParamSet[0];
	if (GuiwinSetParamMenu("基本通讯参数", pParamSet) >= 0)
	{
		memset(&g_nTimeOut[0], 0, sizeof(g_nTimeOut));
		memcpy(&g_nTimeOut[0], pParamSet[1].nEditBuf, GET_MIN(sizeof(g_nTimeOut) - 1, pParamSet[1].nEditLen));
		memset(&g_nUpGradeTimeOut[0], 0, sizeof(g_nUpGradeTimeOut));
		memcpy(&g_nUpGradeTimeOut[0], pParamSet[2].nEditBuf, GET_MIN(sizeof(g_nUpGradeTimeOut) - 1, pParamSet[2].nEditLen));
	}	
	else
	{
		//Guiwin1MboxCreate("退出","未配置");	
	}
}

//设置通讯参数
void SetCommInfo( void* pData)
{
	PARAMSET		sParamSet[20]			= {0};
	pPARAMSET		pParamSet				= &sParamSet[0];
	const INT8S*	c_pUartModeParam[]	= {"红外", "RS485", "RS232", 0};
	const INT8S*	c_pUartBPSParam[]		= {
												"300bps",	"600bps",	"900bps",	"1200bps",
												"2400bps",	"4800bps",	"9600bps",	"14400bps",
												"19200bps", "28800bps", "38400bps", "57600bps", 
												"115200bps", 0};
	const INT8S*	c_pParityParam[]		= {"无校验", "奇校验", "偶校验", 0};
	const INT8S*	c_pStopBitParam[]		= {"1位", "2位", 0};
	const INT8S*	c_pCharBitParam[]		= {"7位", "8位", 0};

	GuiwinSetSelectCfgParam(&pParamSet, "通讯口:", &g_nSynBaseUartMode, c_pUartModeParam);
	GuiwinSetSelectCfgParam(&pParamSet, "波特率:", &g_nSynBaseUartBPS, c_pUartBPSParam);
	GuiwinSetSelectCfgParam(&pParamSet, "检验位:", &g_nSynBaseParity, c_pParityParam);
	GuiwinSetSelectCfgParam(&pParamSet, "停止位:", &g_nSynBaseStopBit, c_pStopBitParam);
	GuiwinSetSelectCfgParam(&pParamSet, "数据位:", &g_nSynBaseCharBit, c_pCharBitParam);
	
	//调用设置菜单
	pParamSet = &sParamSet[0];
	if (GuiwinSetParamMenu("通讯口参数配置", pParamSet) >= 0)
	{

	}	
	else
	{
		//Guiwin1MboxCreate("退出","未配置");	
	}
}

//系统参数其他配置
void OtherParConfig(void)
{
	PARAMSET		sParamSet[10]				= {0};
	pPARAMSET		pParamSet					= &sParamSet[0];
	
	const INT8S*	c_pLedBackClose[]		= {"3分钟", "5分钟", "10分钟", "无限", 0};
	const INT8S*	c_pTermClose[]			= {"5分钟","15分钟","30分钟","1小时","无限",0};
	const INT8S*	c_pBeepClose[]			= {"关","开",0};
	const INT8S*	c_pContinueKeyClose[]	= {"关","开",0};

	GuiwinSetSelectCfgParam(&pParamSet, "背光时间:", &g_LedBackCloseTime, c_pLedBackClose);
	GuiwinSetSelectCfgParam(&pParamSet, "关机时间:", &g_TermCloseTime, c_pTermClose);
	GuiwinSetSelectCfgParam(&pParamSet, "按键音开关:", &g_nBeepFlg, c_pBeepClose);
	GuiwinSetSelectCfgParam(&pParamSet, "长按键开关:", &g_nContinueKeyFlg, c_pContinueKeyClose);
	
	pParamSet = &sParamSet[0];
	if (GuiwinSetParamMenu("其他参数", pParamSet) >= 0)
	{
  		BackClsTimeSet(g_LedBackCloseTime);
		TermClsTimeSet(g_TermCloseTime);
		ConKeyFlgSet(g_nContinueKeyFlg);
	}
	else
	{
		//Guiwin1MboxCreate("退出","未配置");
	}
}

//资产编号配置
void TermSelfAddrConfig(void)
{
	PARAMSET		sParamSet[5]					= {0};
	pPARAMSET		pParamSet						= &sParamSet[0];
	INT8U			nKey							= 0;
	INT8S			nTermSelfAddrMenu[A_MENU_LEN]	= {0}; 
	INT8U			nTermAreaAddr[4]				= {0};
    INT8U           nBuf[10]                     = {0};
	
    _API_FLASH_Read(&nTermAreaAddr[0], STR_OFF(pFRAM_STORE, nTermAreaAddr), STR_SIZE(pFRAM_STORE, nTermAreaAddr));
	//_API_FRAM_Read(STR_OFF(pFRAM_STORE, nTermAreaAddr), &nTermAreaAddr[0], STR_SIZE(pFRAM_STORE, nTermAreaAddr));
	memset(&nTermSelfAddrMenu[0], 0, sizeof(nTermSelfAddrMenu));
	ValToAsciiSub((INT8U*)&nTermSelfAddrMenu[0], nTermAreaAddr[0], 2, 1, 2);
	ValToAsciiSub((INT8U*)&nTermSelfAddrMenu[2], nTermAreaAddr[1], 2, 1, 2);
	ValToAsciiSub((INT8U*)&nTermSelfAddrMenu[4], nTermAreaAddr[2], 2, 1, 2);
	ValToAsciiSub((INT8U*)&nTermSelfAddrMenu[6], nTermAreaAddr[3], 2, 1, 2);
	GuiwinTextOutAuto("");
	GuiwinTextOutAuto("");
	GuiwinTextOutAuto("资产编号:%s",&nTermSelfAddrMenu[0]);
	GuiwinTextOutAuto("");
	GuiwinTextOutAuto("");
	GuiwinTextOutAuto("");
	GuiwinTextOutAuto("");
	GuiwinTextOutAuto("");
	GuiwinTextOutAuto("  [按确认键修改]");
	GuiwinAutoDispProc(NULL, NULL, 0, &nKey);
	
	if(KEY_ENTER != nKey)
	{
		return;    
	}
	/*while(1)
	{
		memset(&sParamSet[0], 0, sizeof(sParamSet));
		memset(&nTermSelfAddrMenu[0], 0, sizeof(nTermSelfAddrMenu));
		GuiwinSetNullCfgParam(&pParamSet, NULL);
		GuiwinSetEditCfgParam(&pParamSet, "请输入密码:", "80000000", 100 + STR_SIZE(pOPERTOR_PRM, nOperPsw) * 2);
		pParamSet = &sParamSet[0];
		if (GuiwinSetParamMenu("密码", pParamSet) >= 0)
		{
			memset(&nTermSelfAddrMenu[0], 0, sizeof(nTermSelfAddrMenu));
			StringToBuf((INT8U*)(&(pParamSet[1].nEditBuf[0])), (INT8U*)&nTermSelfAddrMenu[0], GET_MIN(STR_SIZE(pOPERTOR_PRM, nOperPsw) * 2, pParamSet[1].nEditLen));
			if (PasswordChk(NULL, (INT8U*)&nTermSelfAddrMenu[0], 0) >= S_PSW_RET)
			{
				break; 
			}
			else
			{
				Guiwin1MboxCreate("返回","密码不正确");
			}
		}
		else
		{
			return;    
		}
	}*/
	
	memset(&sParamSet[0], 0, sizeof(sParamSet));
	memset(&nTermSelfAddrMenu[0], 0, sizeof(nTermSelfAddrMenu));
	ValToAsciiSub((INT8U*)&nTermSelfAddrMenu[0], nTermAreaAddr[0], 2, 1, 2);
	ValToAsciiSub((INT8U*)&nTermSelfAddrMenu[2], nTermAreaAddr[1], 2, 1, 2);
	ValToAsciiSub((INT8U*)&nTermSelfAddrMenu[4], nTermAreaAddr[2], 2, 1, 2);
	ValToAsciiSub((INT8U*)&nTermSelfAddrMenu[6], nTermAreaAddr[3], 2, 1, 2);
	GuiwinSetNullCfgParam(&pParamSet, NULL);
	GuiwinSetEditCfgParam(&pParamSet, "资产号:", &nTermSelfAddrMenu[0], 8);
	pParamSet = &sParamSet[0];
	if (GuiwinSetParamMenu("资产编号", pParamSet) >= 0)
	{
		memset(&nTermSelfAddrMenu[0], 0, sizeof(nTermSelfAddrMenu));
		memcpy(&nTermSelfAddrMenu[0], pParamSet[1].nEditBuf, GET_MIN(sizeof(pParamSet[1].nEditBuf), 8));
		StringToBuf((INT8U*)&nTermSelfAddrMenu[0], &nTermAreaAddr[0], 8);
		//SwapBufVal(&nTermAreaAddr[0], 2);
		//SwapBufVal(&nTermAreaAddr[2], 2);
		//_API_FRAM_Write(STR_OFF(pFRAM_STORE, nTermAreaAddr), &nTermAreaAddr[0], STR_SIZE(pFRAM_STORE, nTermAreaAddr));
        _API_FLASH_EraseWrite(&nTermAreaAddr[0], STR_OFF(pFRAM_STORE, nTermAreaAddr), STR_SIZE(pFRAM_STORE, nTermAreaAddr));
		memcpy(&nBuf[0], &nTermAreaAddr[0], sizeof(nTermAreaAddr));
        MemWriteCommPrm(F94, &nBuf[0], sizeof(nBuf)); //设置通信参数
        MemReadCommPrm(F94, &nBuf[0], sizeof(nBuf));
	}
	
}






// 0,正常;1,充电;2,充满
static INT32S BatteryState(void)
{
	INT8U nState = 0;

	nState = BAK_stat();
	if (nState == 1)
	{
		return 1;
	}
	else if (nState == 2)
	{
		return 2;
	}
	else
	{
		return 0;
	}
}

#define BAT_BLK_L			9
#define BAT_OFFSET_X 		201
#define BAT_OFFSET_X_L 	(BAT_BLK_L * 3 + 1)

#define BAT_OFFSET_Y 		2
#define BAT_OFFSET_Y_H 	9

//电池
static void BatteryProcFillpower(void)
{
	INT8U 			cy1 			= 5 + BAT_OFFSET_Y;
	INT16U			nColour			= 0;
	INT8U			nBlock			= 0;
    INT32U          nTmp           	= 0;
	INT8U			nState			= 0;
	static INT8U	s_nBatteryCnt	= 0;

	nState = BatteryState();
	if (0 == nState)
	{
		return;
	}
    if (OSSemAccept(g_pSem[SEM_DISP_IDX]) <= 0)
    {
        return;
    }

	nColour = COLOUR_SET(0, 58, 0);
	if (2 == nState)
	{
		nBlock = 3;
	}
	else
	{
		nBlock  = (s_nBatteryCnt % 3) + 1;
		s_nBatteryCnt++;
	}
	
	nTmp = 3;
    LCD_REC_CLEAR(BAT_OFFSET_X + 1, cy1 + nTmp - 1, BAT_OFFSET_X + BAT_OFFSET_X_L - 1, cy1 + BAT_OFFSET_Y_H * 2 - nTmp, g_nBackColour);
    LCD_REC_CLEAR(BAT_OFFSET_X + BAT_OFFSET_X_L - (nBlock * BAT_BLK_L), cy1 + nTmp - 1, BAT_OFFSET_X + BAT_OFFSET_X_L - 1, cy1 + BAT_OFFSET_Y_H * 2 - nTmp, nColour);
    OSSemPost(g_pSem[SEM_DISP_IDX]);
}

//电池
static void BatteryProc(void)
{
	INT32U	        nEqVal 			= 0;
    INT8U           i               = 0;
	INT8U 			cy1 			= 5 + BAT_OFFSET_Y;
	INT16U			nColour			= 0;
	INT8U			nBlock			= 0;
    INT32U          nTmp            = 0;

	if (0 != BatteryState())
	{
		return;
	}
    if (OSSemAccept(g_pSem[SEM_DISP_IDX]) <= 0)
    {
        return;
    }
    for (i = 0; i < 1; i++)
    {
        nTmp = GetAD0Voltage();
	    nEqVal += nTmp;
    }
    if(nEqVal >= 4050)
    {  
        nColour = COLOUR_SET(0, 58, 0);
        nBlock  = 3;
    }
    else if ((nEqVal >= 3600) && (nEqVal < 4050))
    {
        nColour = COLOUR_SET(0, 58, 0);
        nBlock  = 2;
    }
    else if ((nEqVal >= 3200) && (nEqVal < 3600))
    {
        nColour = COLOUR_SET(31, 0, 0);
        nBlock  = 1;
    }
    else if (nEqVal < 3200)
    {
        //GpioClearOutputEX(KEY_CTR);          // 电量不足自动关机
        GpioClearOutput(IO_PORTA, IO_PINx5);
    }
 
	nTmp = 3;
    //LCD_DRAW_REC(BAT_OFFSET_X + 1, cy1 + nTmp - 1, BAT_OFFSET_X + BAT_OFFSET_X_L - 1, cy1 + BAT_OFFSET_Y_H * 2 - nTmp, g_nBackColour);
	LCD_REC_CLEAR(BAT_OFFSET_X + 1, cy1 + nTmp - 1, BAT_OFFSET_X + BAT_OFFSET_X_L - 1, cy1 + BAT_OFFSET_Y_H * 2 - nTmp, g_nBackColour);
    LCD_REC_CLEAR(BAT_OFFSET_X + BAT_OFFSET_X_L - (nBlock * BAT_BLK_L), cy1 + nTmp - 1, BAT_OFFSET_X + BAT_OFFSET_X_L - 1, cy1 + BAT_OFFSET_Y_H * 2 - nTmp, nColour);
    OSSemPost(g_pSem[SEM_DISP_IDX]);
	//LCD_RANG_POST();
}


//电池
static void BatteryArea(void)
{
	INT8U 	cy1 	= 5 + BAT_OFFSET_Y;
	INT8U	nTmp	= 0;

	nTmp = 1;
	LCD_DRAW_REC(BAT_OFFSET_X, cy1 + nTmp - 1, BAT_OFFSET_X + BAT_OFFSET_X_L, cy1 + BAT_OFFSET_Y_H * 2 - nTmp, g_nDrawColour);
	nTmp = 2;
    LCD_DRAW_REC(BAT_OFFSET_X, cy1 + nTmp - 1, BAT_OFFSET_X + BAT_OFFSET_X_L, cy1 + BAT_OFFSET_Y_H * 2 - nTmp, g_nDrawColour);
	nTmp = 6;
    LCD_DRAW_REC(BAT_OFFSET_X - BAT_BLK_L + 3, cy1 + nTmp - 1, BAT_OFFSET_X, cy1 + BAT_OFFSET_Y_H * 2 - nTmp, g_nDrawColour);
    
    BatteryProc();
}


/********************************************************************************
* 函数名:VersionInfo
* 输　入:
* 输　出:
* 返  回:
* 功  能: 版本信息
*********************************************************************************/
void VersionInfo( void )
{
#define DISP_BUFF_POS(nBuf) 						(240 - strlen((char*)nBuf) * 12) / 2
    INT8U       nKey 		= 0;
	INT8U		nBuf[30] 	= {0};
	INT16U		nRed 		= 31;
	INT16U		nGreen		= 63;
	INT16U		nBlue		= 31;
	INT16U		nOffsetY	= 0;

	for(;;)
	{
		//g_nBackColour = COLOUR_SET(nRed, nGreen, nBlue);
		//g_nDrawColour = COLOUR_SET(0, 0, 31);
		LCD_CLEAR(g_nBackColour);   	//清屏cd
		
	 	BatteryArea();
		OSTimeDlyHMSM(0, 0, 0, 50);

		//时间
		TimeDisp();

		//g_nBackColour = COLOUR_SET(nRed, nGreen, nBlue);
		//g_nDrawColour = COLOUR_SET(0, 58, 0);//COLOUR_SET(0, 57, 8);
		LCD_ShowLogo(7, 58, 120, sizeof(g_nLogo) * 8, (INT8U*)g_nLogo, g_nBackColour, g_nDrawColour, 0);
	    
		memset(&nBuf[0], 0, sizeof(nBuf));
		strcat((char*)nBuf, (char*)COMPANY_INFO);
		nBuf[sizeof(nBuf) - 1] = 0;
		GuiwinTextOut(129, 83, 0, (INT8S*)nBuf);
		LCD_ShowAsciiLogo(129, 110, 96, sizeof(g_nAscii16) * 8, (INT8U*)g_nAscii16, g_nBackColour, g_nDrawColour, 0);


		//nOffsetY = 58;

		//g_nBackColour 	= COLOUR_SET(nRed, nGreen, nBlue);
		//g_nDrawColour 	= COLOUR_SET(0, 0, 31);
		memset(&nBuf[0], 0, sizeof(nBuf));
		strcat((char*)nBuf, (char*)TOOL_INFO);
		nBuf[sizeof(nBuf) - 1] = 0;
	    GuiwinTextOut(DISP_BUFF_POS(nBuf), 188 - nOffsetY, 0, (INT8S*)nBuf);

		memset(&nBuf[0], 0, sizeof(nBuf));
		strcat((char*)nBuf, (char*)"版本:V");
		strcat((char*)nBuf, (char*)PRODUCT);
#ifdef MIN_PRODUCT
		strcat((char*)nBuf, (char*)MIN_PRODUCT);
#endif
		strcat((char*)nBuf, (char*)"-");
		strcat((char*)nBuf, (char*)MAIN_VERSION_INFO);
		strcat((char*)nBuf, (char*)VERSION_INFO);
#ifdef MIN_VERSION_INFO
		strcat((char*)nBuf, (char*)"-");
		strcat((char*)nBuf, (char*)MIN_VERSION_INFO);
#endif
		nBuf[sizeof(nBuf) - 1] = 0;
	    GuiwinTextOut(DISP_BUFF_POS(nBuf), 224 - nOffsetY, 0, (INT8S*)nBuf);

		memset(&nBuf[0], 0, sizeof(nBuf));
		if (AREA_SICHUAN == AreaRunGet(0))
		{
			strcat((char*)nBuf, (char*)SCCOMPANY_ALL_INFO);
		}
		else
		{
			strcat((char*)nBuf, (char*)COMPANY_ALL_INFO);
		}
		nBuf[sizeof(nBuf) - 1] = 0;
		GuiwinTextOut(DISP_BUFF_POS(nBuf), 260 - nOffsetY, 0, (INT8S*)nBuf);

		nBuf[0] = 0;
	    while(1)
	    {
	        GET_KEY_VAL(nKey, 1);
	        switch(nKey)
	        {
	            case KEY_UP:
				case KEY_LEFT:
	            case KEY_DOWN:
				case KEY_RIGHT:
	            case KEY_ENTER:
	            case KEY_CANCEL:
					//g_nBackColour		= COLOUR_SET(0, 0, 31);
					//g_nDrawColour 		= COLOUR_SET(31, 63, 31);;
					LCD_CLEAR(g_nBackColour);   	//清屏cd
					//背光置高
					BatteryArea();
					OSTimeDlyHMSM(0, 0, 0, 50);
					TimeDisp();
					nBuf[0] = 1;
	                break;
	            default:
	                break;
	        }
			if (nBuf[0] == 1)
			{
				break;
			}
	    }
		break;
	}	
}

void KeyLedOnProc(void)
{
    g_nLedBackCnt = 0;
    if (g_nBgFlg)
    {
        g_nLedBackCnt = 0;
        LCD_BG_ON();
        KEY_LED_PDOR = 0x01;
        g_nBgFlg = 0;
    }
}

void KeyLedOffProc(void)
{
    if (g_nBgFlg == 0)
    {
        g_nLedBackCnt = 0;
        g_nBgFlg = 1;
        KEY_LED_PDOR = 0x00;
        LCD_BG_OFF(); 
    }
}

//背光2分钟自动关闭
static void LedBackClose(void)
{
	INT8S	nTmp = 0;
    INT8U   nKey = 0;

	nTmp = g_nLedBackArray[g_LedBackCloseTime];
	g_nLedBackCnt++;
    if(0 == nTmp)
    {
        return;
    }
	if (g_nLedBackCnt >= nTmp)
	{
		KeyLedOffProc();  
	}
}

//顶栏时间显示
void TimeDisp(void)
{
    STD_TIME    sTime;
    pSTD_TIME   pTime = &sTime;    

    if (OSSemAccept(g_pSem[SEM_DISP_IDX]) <= 0)
    {
        return;
    }
      
    memset(pTime, 0, sizeof(STD_TIME));
      
    RTCDateTimeRead(pTime);
    
    GuiwinTextOut(7, 4, LCD_DISP_NORMAL, "%02x-", pTime->nYear);
    GuiwinTextOut(7 + 36, 4, LCD_DISP_NORMAL, "%02x-", pTime->nMon);
    GuiwinTextOut(7 + 36 * 2, 4, LCD_DISP_NORMAL, "%02x", pTime->nDay);
    
    GuiwinTextOut(7 + 36 * 3, 4, LCD_DISP_NORMAL, "%02x:", pTime->nHour);
    GuiwinTextOut(7 + 36 * 4, 4, LCD_DISP_NORMAL, "%02x", pTime->nMin);
    //GuiwinTextOut(7 + 36 * 2, 4, LCD_DISP_NORMAL, "%02x", pTime->nSec);
    
    OSSemPost(g_pSem[SEM_DISP_IDX]);
}



/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 100ms进程显示处理
*********************************************************************************/
void DispMSecProc(void)
{
	
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: s进程显示处理
*********************************************************************************/
void DispSecProc(void)
{
	BatteryProcFillpower();
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 分级进程显示处理
*********************************************************************************/
void DispMinProc(void)
{
	// 背光
	//LedBackClose();

	//BatteryProc();
	//BatteryArea();
    
    //时间
    //TimeDisp();
}


/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 显示任务初始化
*********************************************************************************/
static void TaskDisplayInit(void)
{
    OSTimeDlyHMSM(0, 0, 0, 100);
  	//g_nBackColour = COLOUR_SET(0, 0, 31);;
	//g_nDrawColour = COLOUR_SET(31, 63, 31);;
	
	LCD_INIT();		   			//tft初始化
	
	LCD_CLEAR(g_nBackColour);   	//清屏cd
    
    OSSemPost(g_pSem[SEM_DISP_IDX]);
}

#if 0
void DispTask(void *pData)
{
	pGUI_WIN	pCurr 			= NULL;
	pGUI_WIN	pWin 			= NULL;
	INT8U		nKey			= KEY_NULL;
	INT32S		nItemSelect	= 0; 
	INT8U*		pArg			= (INT8U*)pData;
	INT8U		nBuf[6]			= {0};
	INT8U		nFlg			= 1;//0;
    
    GuiwinInit();
		
	if (0 == *pArg)
	{
		TaskDisplayInit();		
	    //VersionInfo();          //版本信息
	    *pArg = 1;
	}
	else
	{
		//g_nBackColour		= COLOUR_SET(0, 0, 31);;
		//g_nDrawColour 		= COLOUR_SET(31, 63, 31);;
		LCD_CLEAR(g_nBackColour);   	//清屏cd
		//背光置高
		//BatteryArea();
		OSTimeDlyHMSM(0, 0, 0, 50);
		//TimeDisp();
	}
    if (g_nZbDispFlg == 0)
   {
     //集中器界面
	pCurr = GuiwinRootCreat((void*)g_sMainMenu, 0); 
   }
   else if(g_nZbDispFlg == 1)
   {
     //专变界面
     pCurr = GuiwinRootCreat((void*)g_sZbMenu, 0);
   }
	while(1)
	{
		nItemSelect = GuiwinSelectMenuList(pCurr, &nKey);
		pCurr->nItemSelect = GET_MAX(nItemSelect, 1);
        switch(nItemSelect)
        {
			case -1:  //返回或退出
            {
                if (pCurr->nWinLevel > 0)
                {
                    //返回上一级菜单
                    pWin = pCurr;
					if (NULL == (pCurr = GuiwinGetPre(pWin)))
					{
						break;
					}
                }
                break;
            }
			default:  //下一层菜单,执行或其他的操作
            {
                if (KEY_ENTER == nKey)
                {
                    //插入新的菜单条目内容
					pWin  = pCurr;
					if (NULL == (pCurr = GuiwinGetNext(pWin)))
					{
						break;	
					}
					
					if (GuiwinInsert(pWin, pCurr) < 0)      //菜单型的空
					{
					    memset(pCurr, 0, sizeof(GUI_WIN));
					    pCurr = pWin;
                        break;	
					}
					if (NULL == pCurr->pMenuItem)               //函数执行型
					{
						memset(pCurr, 0, sizeof(GUI_WIN));
						pCurr = pWin;
                        break;
					}
                } 
                break;
            }
        }
	}
}
#endif





#if 0
#endif
/********************************************************************************
* 函数名:CfgSelectProcSub
* 输　入:
* 输　出:
* 返  回:
* 功  能: 配置选择子函数
*********************************************************************************/
static INT32S CfgSelectProcSub(void)
{
    PARAMSET		sParamSet[10] 				= {0};
	pPARAMSET 		pParamSet					= &sParamSet[0];
	INT8U			nAreaRun					= 0;
	INT8U			nCarrierCom				= 0;
	INT8U			nOrder						= 0;
	INT8U			nJcMenu						= 0;
	INT8U			nDefaultServ				= 0;
	INT8U           nMenuMode       			= 0;
	const INT8S*	c_pUartCarrModeParam[] 	= {"RS485", "RS232", 0};
	const INT8S*	c_pWorkOrderParam[] 		= {"是", "否", 0};
	const INT8S*	c_pJcMenuParam[] 		= {"简易", "非简易", 0};
	const INT8S*	c_pDefaultServParam[] 		= {"否", "是", 0};
	//const INT8S*	c_pSimpleMenu[]				= {"非简易", "简易", 0};

	nAreaRun = AreaRunGet(1);
	GuiwinSetSelectCfgParam(&pParamSet, "地区选择:", (INT8S*)&nAreaRun, g_sAreaString);
	nCarrierCom = CarrierComGet(0);
    GuiwinSetSelectCfgParam(&pParamSet, "载波仪通讯口:", (INT8S*)(&nCarrierCom), c_pUartCarrModeParam);
	nMenuMode = MenuModeGet();
	nOrder = WorkOrderGet(0);
	GuiwinSetSelectCfgParam(&pParamSet, "是否需要工单:", (INT8S*)(&nOrder), c_pWorkOrderParam);
	nJcMenu = JcMenuGet(0);
	GuiwinSetSelectCfgParam(&pParamSet, "集抄菜单选择:", (INT8S*)(&nJcMenu), c_pJcMenuParam);
	nDefaultServ = DefaultServGet(0);
	GuiwinSetSelectCfgParam(&pParamSet, "公司服务器选择:", (INT8S*)(&nDefaultServ), c_pDefaultServParam);

	
	//GuiwinSetSelectCfgParam(&pParamSet, "菜单选择:", (INT8S*)&nMenuMode, c_pSimpleMenu);
	//调用设置菜单
    pParamSet = &sParamSet[0];
	if (GuiwinSetParamMenu("配置选择", pParamSet) >= 0)
    {
        AreaRunSet(nAreaRun);
		CarrierComSet(nCarrierCom);
		WorkOrderSet(nOrder);
		JcMenuSet(nJcMenu);
		MenuModeSet(nMenuMode);
		DefaultServSet(nDefaultServ);
		return 0;    
    }	
    return -1;
}


/********************************************************************************
* 函数名:CfgSelectProc
* 输　入:
* 输　出:
* 返  回:
* 功  能: 配置选择
*********************************************************************************/
INT32S CfgSelectProc(void)
{
    PARAMSET		sParamSet[3]					= {0};
	pPARAMSET		pParamSet						= &sParamSet[0];
	INT8S			nPwdMenu[A_MENU_LEN]			= {0}; 
	
	while(1)
	{
		memset(&sParamSet[0], 0, sizeof(sParamSet));
		memset(&nPwdMenu[0], 0, sizeof(nPwdMenu));
		GuiwinSetNullCfgParam(&pParamSet, NULL);
		GuiwinSetEditCfgParam(&pParamSet, "请输入密码:", "********", 100 + STR_SIZE(pOPERTOR_PRM, nOperPsw) * 2);
		pParamSet = &sParamSet[0];
		if (GuiwinSetParamMenu("密码", pParamSet) >= 0)
		{
			memset(&nPwdMenu[0], 0, sizeof(nPwdMenu));
			StringToBuf((INT8U*)pParamSet[1].nEditBuf, (INT8U*)&nPwdMenu[0], GET_MIN(STR_SIZE(pOPERTOR_PRM, nOperPsw) * 2, pParamSet[1].nEditLen));
			if (PasswordChk(NULL, (INT8U*)&nPwdMenu[0], 0) >= S_PSW_RET)
			{
				break; 
			}
			Guiwin1MboxCreate("返回","密码不正确");
		}
		else
		{
			return -1;    
		}	 
	}
	if (CfgSelectProcSub() >= 0)
	{
		Guiwin1MboxCreate("确认","选择成功");  
	}
    return 0;
}

/********************************************************************************
* 函数名:AcInfo
* 输　入:
* 输　出:
* 返  回:
* 功  能: 交采信息
*********************************************************************************/
void AcInfo(void)
{
    INT8U   nMemAddr[6] = {0};
    INT8U   nBuf[2]     = {0};
    INT8U   nBufTmp[10] = {0};
    INT8U   j = 0;
    INT8U   i   = 0;
    INT8U   nIdex = 0;
    
    MemReadAcPrm(F19, &nMemAddr[0], 6);  //读取交采地址，转成内存
    
    GuiwinTextOutAuto("地址:%02x%02x%02x%02x%02x%02x", nMemAddr[5], nMemAddr[4],
                                        nMemAddr[3],nMemAddr[2],nMemAddr[1],nMemAddr[0]); 
    
    GuiwinTextOutAuto("A相电压:%02x%02x", pDataTable->sRealInstant.sRealVolt[0].nIntg,
                                        pDataTable->sRealInstant.sRealVolt[0].nFrac); 
    GuiwinTextOutAuto("B相电压:%02x%02x", pDataTable->sRealInstant.sRealVolt[1].nIntg,
                                        pDataTable->sRealInstant.sRealVolt[1].nFrac); 
    GuiwinTextOutAuto("C相电压:%02x%02x", pDataTable->sRealInstant.sRealVolt[2].nIntg,
                                        pDataTable->sRealInstant.sRealVolt[2].nFrac); 
    
    GuiwinTextOutAuto("A相电流:%02x%02x%02x", pDataTable->sRealInstant.sRealCurr[0].nD3,
                                        pDataTable->sRealInstant.sRealCurr[0].nD2,
                                        pDataTable->sRealInstant.sRealCurr[0].nD1);
    GuiwinTextOutAuto("B相电流:%02x%02x%02x", pDataTable->sRealInstant.sRealCurr[1].nD3,
                                        pDataTable->sRealInstant.sRealCurr[1].nD2,
                                        pDataTable->sRealInstant.sRealCurr[1].nD1);
    GuiwinTextOutAuto("C相电流:%02x%02x%02x", pDataTable->sRealInstant.sRealCurr[2].nD3,
                                        pDataTable->sRealInstant.sRealCurr[2].nD2,
                                        pDataTable->sRealInstant.sRealCurr[2].nD1);
    
    /*for (i = 0; i < 3; i++)
    {
        memcpy(&nBuf[0], &pDataTable->sRealInstant.sRealVolt[i], 2);
        memset(&nBufTmp[0], 0, sizeof(nBufTmp));
        SwapBufVal(&nBuf[i * 2 + nIdex], 2);
        for (j = 0; j < 2; j++)
        {
            sprintf((char*)(&nBufTmp[j * 2]),"%02x", nBuf[i * 2 + nIdex + j]);
        }
        //SwapBufVal(&nBufTmp[0], 4);
        memmove(&nBufTmp[4], &nBufTmp[3], 1);
        nBufTmp[3] = '.';
        if ((nBuf[i * 2 + nIdex + 1] & 0x80) == 0x80)
        {
            if (i == 0)
            {
                GuiwinTextOutAuto("A相:-%s", &nBufTmp[0]); 
            }
            else if (i == 1)
            {
                GuiwinTextOutAuto("B相:-%s", &nBufTmp[0]); 
            }
            else if (i == 2)
            {
                GuiwinTextOutAuto("C相:-%s", &nBufTmp[0]); 
            }
        }
        else
        {
            if (i == 0)
            {
                GuiwinTextOutAuto("A相:%s", &nBufTmp[0]); 
            }
            else if (i == 1)
            {
                GuiwinTextOutAuto("B相:%s", &nBufTmp[0]); 
            }
            else if (i == 2)
            {
                GuiwinTextOutAuto("C相:%s", &nBufTmp[0]); 
            }
        }
    }*/
    
    GuiwinAutoDispProc("交采信息", NULL , 0 , NULL);
}
/********************************************************************************
* 函数名:GprsDataWatch
* 输　入:
* 输　出:
* 返  回:
* 功  能: GPRS数据监测开关
*********************************************************************************/
void GprsDataWatch(void)
{
    PARAMSET		sParamSet[10] 				= {0};
	pPARAMSET 		pParamSet					= &sParamSet[0];
	INT8U			nOrder						= 0;


	const INT8S*	c_pWorkOrderParam[] 		= {"否","是",  0};

    if (nOrder != 0)
    {
        nOrder = 1;
    }
	GuiwinSetSelectCfgParam(&pParamSet, "是否开启开关:", (INT8S*)(&g_nGprsWatchFlg), c_pWorkOrderParam);;

	
	//GuiwinSetSelectCfgParam(&pParamSet, "菜单选择:", (INT8S*)&nMenuMode, c_pSimpleMenu);
	//调用设置菜单
    pParamSet = &sParamSet[0];
	if (GuiwinSetParamMenu("开关设置", pParamSet) >= 0)
    {
		MemoryWrite(STR_OFF(pMEM_FRAM_STORE, sXFramData.nGprsFlg),&g_nGprsWatchFlg ,1, MEM_FRAM);
		return;    
    }	
    return ;
}

/********************************************************************************
* 函数名:StratCheck
* 输　入:
* 输　出:
* 返  回:
* 功  能: 启动检测
*********************************************************************************/
void StratCheck(void)
{
    INT8U	l_buff[30]   = {0x68 ,0x36,0x00,0x36,0x00,0x68,0x4B,0xFF,0xFF,
                            0xFF,0xFF,0x03,0xDF,0x60,0x00,0x00,0x01,0x00,0x01,0x8B,0x16};
	INT8U   l_Tmpbuff[30] = {0};
    INT8U	l_ret = 0;
    INT8U   i             = 0;
    INT8U   j               = 0;
    INT8U   nBuf          = 0;
    INT8U   nErrFlg         = 0;
    

	V1_IRTXM_ON();
    
    LcdClear(g_nBackColour);
    
    //GuiwinTextOut(8, 16, 0,"故障板测试...");
    
    for (i = 0; i < 8; i++)
    {
        SetBufBit(&nBuf, 1, i);
        GuiwinTextOut(8, 16, 0,"第%d继电器吸合",i + 1);
        if (MainF5Proc(nBuf) < 0)
        {
            Guiwin1MboxCreate("确认", "通信失败"); 
            return;
        }
        OSTimeDlyHMSM( 0, 0, 1, 500);
    }
    
    for (i = 0; i < 8; i++)
    {
        ClrBufBit(&nBuf, 1, i);
        GuiwinTextOut(8, 16, 0,"第%d继电器断开",i + 1);
        if (MainF5Proc(nBuf) < 0)
        {
            Guiwin1MboxCreate("确认", "通信失败"); 
            return;
        }
        OSTimeDlyHMSM( 0, 0, 1, 500);
    }
    beep();
    Guiwin1MboxCreate("确认", "测试成功");
    
    
    
    /*if (MainF5Proc(0x0F) < 0)
    {
        Guiwin1MboxCreate("确认", "通信失败");   
    }
    OSTimeDlyHMSM( 0, 0, 3, 0);
    
    if (MainF5Proc(0xFF) < 0)
    {
        Guiwin1MboxCreate("确认", "通信失败");   
    }
    OSTimeDlyHMSM( 0, 0, 3, 0);
    
    
    if (MainF5Proc(0xF0) < 0)
    {
        Guiwin1MboxCreate("确认", "通信失败");   
    }
    OSTimeDlyHMSM( 0, 0, 3, 0);
    
    if (MainF5Proc(0x00) < 0)
    {
        Guiwin1MboxCreate("确认", "通信失败");   
    }
    beep();
    Guiwin1MboxCreate("确认", "通信成功"); 
    //WriteComm(COMM_ERR, l_buff, 21);
    //l_ret = ReadCommMs(COMM_ERR, l_Tmpbuff, 100, 500, 500);
    /*if (l_ret > 0)
    {
        Guiwin1MboxCreate("确认", "通信成功"); 
    }
    else
    {
        Guiwin1MboxCreate("确认", "通信失败");   
    }*/
    
}

/********************************************************************************
* 函数名:GprsTest
* 输　入:
* 输　出:
* 返  回:
* 功  能: 
*********************************************************************************/
void GprsTest(void)
{
    INT8U	    l_buff[128];
    INT16U		nX		= 8;
	INT16U		nY		= 8;
    INT8U       nFlg    = 0;
	INT8U		j		= 0;
    
    LcdClear(g_nBackColour);
    
    GuiwinTextOut(nX, nY, 0,"GPRS测试...");
	nY += 16;
    
    GuiwinTextOut(nX, nY, 0,"正在进行基本配置...");
    nY += 16;
    
    if (!SendAtCmdAndWaitATResp((char*)"AT\x0d",(char*)g_Modl_OK,3000,3,0,&l_buff[0], sizeof(l_buff)))
	{
        nFlg = 1; 
		//return;	
	}
	if (!SendAtCmdAndWaitATResp((char*)"ATE0\x0d",(char*)g_Modl_OK,3000,3,0,NULL, 0))
	{
		//return;	
	}
	if (!SendAtCmdAndWaitATResp((char*)"AT+CMEE=1\x0d",(char*)g_Modl_OK,3000,3,0,NULL, 0))
	{
		//return;	
	}
	if (!SendAtCmdAndWaitATResp((char*)"AT+IFC=0,0\r\n",(char*)g_Modl_OK,3000,3,0,NULL, 0))
	{
		//return;
	}
	/*if (!SendAtCmdAndWaitATResp((char*)"AT+GMR\x0d",(char*)g_AT_OK,3000,3,0,NULL, 0))//读版本
	{
		//return;	//版本读不通不影响功能，故不退出
	}*/
    
    if (!SendAtCmdAndWaitATResp((char*)"AT#CCID\x0d\x0a",(char*)g_Modl_OK,3000,3,0,NULL, sizeof(l_buff)))
	{
		//return;	
	}
    
    if (nFlg)
    {
        GuiwinTextOut(nX, nY, 0,"AT 通信失败"); 
        GuiwinTextOutAuto("AT 通信失败");
        return;    
    }
    
    LfCr(l_buff);
    GuiwinTextOut(nX, nY, 0,(INT8S*)l_buff);
	nY += 16;
    OSTimeDlyHMSM( 0, 0, 1, 500);
    
    if (!SendAtCmdAndWaitATResp((char*)"AT+GMR\x0d",(char*)"0/-/0-07/",1000,1,0,&l_buff[0], 0))
    {
        nFlg = 1;  
    }
    if (nFlg)
    {
        GuiwinTextOut(nX, nY, 0,"模块版本号低");
        GuiwinTextOutAuto("模块版本号低");
        //g_nErrFlg = 1; 
        //V5_GPRS_OFF();
        return; 
    }
    else
    {
        LfCr(l_buff);
        GuiwinTextOut(nX, nY, 0,(INT8S*)l_buff); 
        nY += 16;    
    }
    OSTimeDlyHMSM( 0, 0, 1, 500);
    
    //SIM卡注册
	for(j=0; j<80; j++)
    {
        if (KEY_CANCEL == GetKeyVal(1))
        {
            j = 80;
            break;
        }
        GuiwinDynMboxCreate(j, "SIM卡正在注册");
		if (TltSimRegExTmp(&l_buff[0], sizeof(l_buff)))
		{
			break;
		}
        OSTimeDlyHMSM(0, 0, 0, 500);
    }
    LcdClear(g_nBackColour);
    if (j < 80)
	{
		LfCr(l_buff);
		GuiwinTextOut(nX, nY, 0,"CREG:");
		GuiwinTextOut(nX + 16, nY, 0,(INT8S*)l_buff);		
	}
	else
	{
		nFlg = 1;
		GuiwinTextOut(nX, nY, 0,"SIM注册失败");	
        GuiwinTextOutAuto("SIM注册失败");
		//g_nErrFlg = 1; 
        //V5_GPRS_OFF();
        return;
	}
    OSTimeDlyHMSM(0, 0, 1, 500);
    
    beep();    
    Guiwin1MboxCreate("确认", "GPRS测试成功");

}

/********************************************************************************
* 函数名:CtrlProc
* 输　入:
* 输　出:
* 返  回:
* 功  能: 控制板测试
*********************************************************************************/
void CtrlProcTest(void)
{
    INT8U	l_buff[30]   = {0x68, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x68, 0x15, 0x02, 0x32, 0x32, 0x47, 0x16};
    INT8U	l_buff2[30]   = {0x68, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x68, 0x15, 0x02, 0x33, 0x33, 0x49, 0x16};
	INT8U   l_Tmpbuff[30] = {0};
    INT8U	l_ret = 0;
    

	V1_IRTXM_ON();
    
    LcdClear(g_nBackColour);
    
    GuiwinTextOut(8, 16, 0,"控制板测试...");
    
    WriteComm(COMM_ZB, l_buff, 14);
    l_ret = ReadCommMs(COMM_ZB, l_Tmpbuff, 100, 500, 500);
    
    if (l_ret == 0)
    {
        Guiwin1MboxCreate("确认", "通信失败");
    }
    OSTimeDlyHMSM(0, 0, 3, 0);
    WriteComm(COMM_ZB, l_buff2, 14);
    l_ret = ReadCommMs(COMM_ZB, l_Tmpbuff, 100, 500, 500);
    
    if (l_ret > 0)
    {
        beep();    
        Guiwin1MboxCreate("确认", "通信成功");
    }
    else
    {
        Guiwin1MboxCreate("确认", "通信失败");
    }
}


/*****************************************************************************
* 函 数 名:BcdToAcii
* 输　  入:pData     待转换的源数据
*          nDataLen  待转换的源数据长度
*          bDescFlg  倒序转换标志 TRUE  20 83 -> 83 20->38 33 32 30
*                                 FALSE 20 83 -> 20 83->32 30 38 33
* 输　  出:pDest     转换后的数据
* 返    回:TRUE      转换成功
*          FALSE     输入参数无效
* 功能描述:将BCD的格式转换为ACAII格式 TRUE    20 83 -> 83 20->38 33 32 30
*                                     FALSE   20 83 -> 20 83->32 30 38 33
******************************************************************************/
BOOLEAN BcdToAscii(INT8U *pData, INT8U nDataLen, BOOLEAN bDescFlg, INT8U *pDest)
{
	INT8U	bb = 0;
	INT8U   i = 0;

	if((pData == NULL) || (pDest == NULL) || (nDataLen == 0))
    {
	    return FALSE;
    }
	if (bDescFlg)
	{
		for(i = nDataLen; i > 0; i--)
		{
			bb = pData[i - 1] >> 4;
            *pDest++ = (bb > 9) ? (bb + 55) : (bb + '0');	//'A'-10==55
			bb = pData[i - 1] & 0x0F;
            *pDest++ = (bb > 9) ? (bb + 55) : (bb + '0');
		}
	}
	else
	{
		for(i = 0; i < nDataLen; i++)
		{
			bb = pData[i] >> 4;
            *pDest++ = (bb > 9) ? (bb + 55) : (bb + '0');	//'A'-10==55
			bb = pData[i] & 0x0F;
            *pDest++ = (bb > 9) ? (bb + 55) : (bb + '0');
		}
	}
	*pDest = 0x00;
	return TRUE;
}

void LfCr(INT8U * ch)
{
	INT8U i,c;

	for (i=0; i< 64;i++)
	{
		c = ch[i];
		if (c==0x00)
		{
			break;
		}
		else if((c == 0x0A) || (c == 0x0D))
		{
			ch[i] = 32;
		}
	}
}

/*
*****************************************************************************
* 函数名: TltSimRegEx
* 输　入: 
* 输  出: 
* 返  回: 
*       
*       
* 功  能: sim卡注册
*****************************************************************************
*/
BOOLEAN TltSimRegExTmp(INT8U* pBuf, INT16U nLen)
{
    INT8U			nAtCmdLen      = strlen((char*)"*BQDF9\x1f");
	INT8U			nTemp		   = 0;
	//INT8U			nBuf[100];
    
    memset(&pBuf[0], 0, nLen);
	if (SendAtCmdAndWaitATResp((char*)"AT+CREG?\x0d",(char*)"*BQDF9\x1f",2000,1,0,&pBuf[0], nLen))//"+CREG: "
	{
		nTemp = pBuf[nAtCmdLen + 2];
		if ((nTemp == 0x31) || (nTemp == 0x35))
		{
			return TRUE;
		}	
	}
	return FALSE;
}

void meterPowerOff(void)
{
    INT8U nTmpBuf[35] = {0};
  
    //背光不亮
    GpioSetOutput(IO_PORT7, IO_PINx5); 
    //不显示字
    memset(nTmpBuf, 0, 35);
    BULCDWrite1(0, nTmpBuf, 35);
    //脉冲灯不亮
    ATT7022Init();
    ATT7022Reset( 200 );
}

void GetLastMonData(pMEM_AC_DJ_DATA  sHistMonData,INT8U LastNum)
{
    INT8U i = 0;
    INT8U j = 0;
    STD_TIME sTime;
    STD_TIME sLastTime;
    MEM_AC_DJ_DATA     sAcHistData;
    
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));
    //获取系统时钟
    memset(&sTime, 0, sizeof(STD_TIME));
    memset(&sLastTime, 0, sizeof(STD_TIME));
    
    if (RET_ERR_OK != GetSysClk(&sTime))
    {
        //return 0;
    }
    
    for(j = 0; j < LastNum; j++)
    {
        GetLastMon(&sLastTime, &sTime);
        memcpy(&sTime,&sLastTime,sizeof(sLastTime));
    }
    
    //GetNextMon(&sLastTime, &sTime); //存储为本月1号时间，应在去上几月在加上一月
    memcpy(&sLastTime,&sTime,sizeof(sTime));
    
    for(int ii = 0; ii<12; ii++)
    {
        memset(&sAcHistData, 0, sizeof(sAcHistData));
        MemReadAcHistDJDataProc(F7, ii, &sAcHistData, sizeof(sAcHistData));
        if( 0 == memcmp(&sLastTime.nMon,&sAcHistData.sTime.nMon,2))
        { 
            i = ii;
            ii = 33;
        }
        else
        {
            i = 0xFF;
        }
    }
    
    if(i == 0xff)
    {
        memset(sHistMonData, 0, sizeof(MEM_AC_DJ_DATA));
    }
    else
    {
        memcpy(sHistMonData,&sAcHistData,sizeof(MEM_AC_DJ_DATA));
    }
    //rerutn sHistMonData;
}

INT8U EveWrongNum=0;
INT8U MeterWrongNum=0;
INT32S  AutoDisPlayProc2(INT8U* nFlgCnt ,INT8U nLenFlg)
{
    INT8U                nStep                       = 0;
    INT8U                nFlg                        = 0;
    INT8U                nTmpFlgCnt                  = 0;
   
    nTmpFlgCnt = *nFlgCnt;
    
    if(nLenFlg == 0)
    { 
        if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_100 == g_nHardType)||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_100U == g_nHardType) ||(TRIPHASE_METER_220ZT == g_nHardType))
      {
        nStep =  MeterWrongNum;
      }
        else if(SINGLEPHASE_METER == g_nHardType)
      { 
        nStep =  MeterWrongNum;       
      }
    }

    while(1)
    {       
         if(0 == nTmpFlgCnt)  
        {
          OSTimeDlyHMSM(0, 0, 3, 0);
          nTmpFlgCnt = 1;
          g_nFlg = 0;
         if(SINGLEPHASE_METER == g_nHardType)
          {
            nTmpFlgCnt = MeterWrongNum; 
          }
        }
          if (g_nFlg)
          {
              g_nFlg = 0;
              nTmpFlgCnt ++ ;
            
            if((nTmpFlgCnt == (nStep + 1)) && ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_100 == g_nHardType)||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_100U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType)))
            {
              nTmpFlgCnt = 1;
            }    
            else if((nTmpFlgCnt == (nStep + 1)) && (SINGLEPHASE_METER == g_nHardType))
            {
              nTmpFlgCnt = MeterWrongNum;
            }
          }
          *nFlgCnt = nTmpFlgCnt;
            
     break;
        }
    
    return nFlg;
}
INT32S  KeyProc02 (INT8U* nFlgCnt ,INT8U nLenFlg)
{
    INT8U                nStep                       = 0;
    INT8U                nTmpFlgCnt                  = 0;
    INT8U                nFlg                        = 0;
    INT8U                nKey                        = 0   ;
     
    nTmpFlgCnt = *nFlgCnt;
    
    if(nLenFlg == 0)
    { if((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_100 == g_nHardType)||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_100U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType))
      {
        nStep =  MeterWrongNum;
      }
       else if(SINGLEPHASE_METER == g_nHardType)
      { 
        nStep =  MeterWrongNum;       
      }
    }

    while(1)
    {     
         GET_KEY_VAL(nKey, 1);
       if(0 == nTmpFlgCnt)  
      {
       if(SINGLEPHASE_METER == g_nHardType)
        {
          nTmpFlgCnt = MeterWrongNum; 
        }
      }
       if(KEY_DOWN == nKey)
       {  
         if(1 == g_nFlg_Disp)
         {  
            if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_100 == g_nHardType)||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_100U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType))
          {
            nTmpFlgCnt = 0;
          }
            else if(SINGLEPHASE_METER == g_nHardType)
            {
              nTmpFlgCnt = 0;
            } 
          }
          nTmpFlgCnt ++ ;
          if((nTmpFlgCnt == (nStep + 1)) &&((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_100 == g_nHardType)||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_100U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType)))
          {
            nTmpFlgCnt = 1;
          }
          else if((nTmpFlgCnt == (nStep + 1)) && (SINGLEPHASE_METER == g_nHardType))
          {
            nTmpFlgCnt = 1;
          }
          g_nFlg_Disp = 0;
          if(0 == g_nFlgBattery)
          {    
          GpioClearOutput(IO_PORT7, IO_PINx5);  //背光点亮 
	}
          else
          {
             GpioSetOutput(IO_PORT2, IO_PINx1);
          }
	}
	else if(KEY_UP == nKey)
	{   
        if((1 == g_nFlg_Disp) && ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_100 == g_nHardType)||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_100U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType)))
        {
          nTmpFlgCnt = 2;
        }
        else if((1 == g_nFlg_Disp) && (SINGLEPHASE_METER == g_nHardType))
        {
          nTmpFlgCnt = 2;
        }
            nTmpFlgCnt -- ;
        if((nTmpFlgCnt == 0) && ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_100 == g_nHardType)||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_100U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType)))
	   {
          nTmpFlgCnt = nStep;
        }
        if((nTmpFlgCnt == 0) && (SINGLEPHASE_METER == g_nHardType))
	   {
              nTmpFlgCnt = nStep;
            }
        g_nFlg_Disp = 0;
        if(0 == g_nFlgBattery)
        {    
         GpioClearOutput(IO_PORT7, IO_PINx5);  //背光点亮 
	}
        else
        {
           GpioSetOutput(IO_PORT2, IO_PINx1);
        }
	}

       *nFlgCnt = nTmpFlgCnt;
     break;
        }

    return nFlg;
}
INT8U   nFlgStep = 0;
//自动显示
INT32S  AutoDisPlayProc(INT8U* nFlgCnt ,INT8U nLenFlg)
{
    INT8U                nStep                       = 0;
    INT8U                nFlg                        = 0;
    INT8U                nTmpFlgCnt                  = 0;
   
    nTmpFlgCnt = *nFlgCnt;
    
    if(nLenFlg == 0)
    { 
        if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_100 == g_nHardType) ||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_100U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType))
        {
            if(DISPLY_SWITCH == 1)
                nStep =  15+EveWrongNum;
            else
                nStep =  82+EveWrongNum;

        }
        else if(SINGLEPHASE_METER == g_nHardType)
        { 
            nStep =  104+19;       
        }
    }

    while(1)
    {       
        if(0 == nTmpFlgCnt)  
        {
            OSTimeDlyHMSM(0, 0, 3, 0);
            nTmpFlgCnt = 1;
            g_nFlg = 0;
            if(SINGLEPHASE_METER == g_nHardType)
            {
                nTmpFlgCnt = 83+17; 
            }
        }
        if (g_nFlg)
        {
            g_nFlg = 0;
            nTmpFlgCnt ++ ;
            nFlgStep ++;
            
            if(DISPLY_SWITCH == 1) 
            {
                if(((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_100 == g_nHardType)||(TRIPHASE_METER_100U == g_nHardType) ||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType)))
                {
                    if(nTmpFlgCnt == 8)
                    {
                        nTmpFlgCnt = 75;
                    }
                    else if(nTmpFlgCnt == 76)
                    {
                        nTmpFlgCnt = 13;
                    }   
                    else if(nTmpFlgCnt == 15)
                    {
                        nTmpFlgCnt = 8;
                    }
                    else if((nTmpFlgCnt == 13)&&(EveWrongNum ==0))
                    {
                        nTmpFlgCnt = 1;
                    }
                    else if((nFlgStep == 16)&&(nTmpFlgCnt == 13)&&(EveWrongNum > 0))
                    {
                        nTmpFlgCnt = 69;
                    }
                }     
            }   
            if((nFlgStep == 1)&&(((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_100 == g_nHardType)||(TRIPHASE_METER_100U == g_nHardType) ||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType))))
            {
                nTmpFlgCnt = 1;
            }
            if((nFlgStep > (nStep+1))&&(((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_100 == g_nHardType)||(TRIPHASE_METER_100U == g_nHardType) ||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType))))
            {
                nFlgStep = 1;
            }
            //在上电时防止三相表出现未完全启动时就进行故障代码错误判断大于100,显示单相表轮显页
            if((nTmpFlgCnt > 0x63) && ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_100 == g_nHardType)||(TRIPHASE_METER_100U == g_nHardType) ||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType)))
            {
                nTmpFlgCnt = 1;
            }
            if((nFlgStep == (nStep + 1)) && ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_100 == g_nHardType)||(TRIPHASE_METER_100U == g_nHardType) ||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType)))
            {
                nTmpFlgCnt = 1; 
                nFlgStep = 1;
            }    
            else if((nTmpFlgCnt == (nStep + 1)) && (SINGLEPHASE_METER == g_nHardType))
            {
                nTmpFlgCnt = 83+17;
            }
        }
        *nFlgCnt = nTmpFlgCnt;

        break;
    }
    
    return nFlg;
}

//按键处理
INT32S  KeyProc0 (INT8U* nFlgCnt ,INT8U nLenFlg)
{
    INT8U                nStep                       = 0;
    INT8U                nTmpFlgCnt                  = 0;
    INT8U                nFlg                        = 0;
    INT8U                nKey                        = 0   ;

    nTmpFlgCnt = *nFlgCnt;

    if(nLenFlg == 0)
    { 
        if((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_100 == g_nHardType)||(TRIPHASE_METER_100U == g_nHardType) ||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType))
        {
            if(DISPLY_SWITCH == 1)
                nStep =  68+EveWrongNum;  
            else
                nStep =  82+EveWrongNum;  
        }
        else if(SINGLEPHASE_METER == g_nHardType)
        { 
            nStep =  104+19;       
        }
    }

    while(1)
    {     
        GET_KEY_VAL(nKey, 1);
        if(0 == nTmpFlgCnt)  
        {
            if(SINGLEPHASE_METER == g_nHardType)
            {
                nTmpFlgCnt = 83+17;   //83
            }
        }
        if(KEY_DOWN == nKey)
        {  
            if(1 == g_nFlg_Disp)
            {  
                if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_100 == g_nHardType)||(TRIPHASE_METER_100U == g_nHardType) ||(TRIPHASE_METER_57U == g_nHardType) ||(TRIPHASE_METER_220ZT == g_nHardType))
                {
                    nTmpFlgCnt = 0;
                    if(EveWrongNum>0)
                    {
                        if(DISPLY_SWITCH == 1)
                            nTmpFlgCnt =  68;  
                        else
                            nTmpFlgCnt =  82;  
                    }

                }
                else if(SINGLEPHASE_METER == g_nHardType)
                {
                    nTmpFlgCnt = 82+17; 
                } 
            }
            nTmpFlgCnt ++ ;
            if((nTmpFlgCnt == (nStep + 1)) &&((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_100 == g_nHardType)||(TRIPHASE_METER_100U == g_nHardType) ||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType)))
            {
                nTmpFlgCnt = 1;
            }
            else if((nTmpFlgCnt == (nStep + 1)) && (SINGLEPHASE_METER == g_nHardType))
            {
                nTmpFlgCnt = 83+17;
            }
            g_nFlg_Disp = 0;
            if(0 == g_nFlgBattery)
            {
                if(((g_MeterHardwareWrong[0]&0x04)==0)&&((g_MeterHardwareWrong[0]&0x01)==0))  //未收到黑屏及不启动命令
                {
                    GpioClearOutput(IO_PORT7, IO_PINx5);  //背光点亮 
                }
            }
            else
            {
                GpioSetOutput(IO_PORT2, IO_PINx1);
            }
        }
        else if(KEY_UP == nKey)
        {   
            if((1 == g_nFlg_Disp) && ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_100 == g_nHardType) ||(TRIPHASE_METER_100U == g_nHardType) ||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType)))
            {
                nTmpFlgCnt = 2;
                if(EveWrongNum>0)
                {
                    if(DISPLY_SWITCH == 1)
                        nTmpFlgCnt =  70;  
                    else
                        nTmpFlgCnt =  84;  
                }
            }
            else if((1 == g_nFlg_Disp) && (SINGLEPHASE_METER == g_nHardType))
            {
                nTmpFlgCnt = 84+17;
            }
            nTmpFlgCnt -- ;
            if((nTmpFlgCnt == 0) && ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_100 == g_nHardType)||(TRIPHASE_METER_100U == g_nHardType) ||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType)))
            {
                nTmpFlgCnt = nStep;
            }
            if((nTmpFlgCnt == 82+17) && (SINGLEPHASE_METER == g_nHardType))
            {
                nTmpFlgCnt = nStep;
            }
            g_nFlg_Disp = 0;
            if(0 == g_nFlgBattery)
            {
                if(((g_MeterHardwareWrong[0]&0x04)==0)&&((g_MeterHardwareWrong[0]&0x01)==0))  //未收到黑屏及不启动命令
                {
                    GpioClearOutput(IO_PORT7, IO_PINx5);  //背光点亮 
                }
            }
            else
            {
                GpioSetOutput(IO_PORT2, IO_PINx1);
            }
        }

    *nFlgCnt = nTmpFlgCnt;
    break;
    }
    
    return nFlg;
}

 INT8U  Data1[35] = {   0x0a,0x0f,  0x0a,0x0f,  0x10,0x00,  0x00,0x00,   0x10,0x00,  0x04,0x00,   0x10,0x00,  0x00,0x00,   0xa0,0xf0,
                       0x00,       0x00,0x00,  0x00,0x00,  0x00,0x00,   0x00,0x00,   0x00,0x00,   0x00,0x00,  0x00,0x00,  0x00,   0x00
                    };   // 初始化数组

INT8U ERR01=0,ERR02=0,ERR03=0,ERR04=0,ERR05=0,ERR06=0,ERR08=0;
INT8U ERR51=0,ERR52=0,ERR53=0,ERR54=0,ERR55=0,ERR56=0;
INT8U       nTempBattery                    = 0;
INT8U       nTempBattery2                    = 0;

Page_Nsp currentPage = PAGE_HOME;
/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 显示任务
*********************************************************************************/

void DispTask(void *pData)
{
    INT8U   nStepCnt    = 0;
    INT8U   M=0,nTmp    = 0;     
    INT8U   data0[2]    = 0;
    INT8U   nCtrlFlg    = 0;
    
	
	// 定义一个变量用于存储下一个要切换到的页面，初始化为首页
    //Page_Nsp nextPage = PAGE_HOME;
	
    while(1)
	{
		updateLCDDisplay();
	}
}

// 根据当前页面更新LCD显示
void updateLCDDisplay() 
{
	currentPage = (Page_Nsp)g_DwinPageID;
    switch (currentPage) {
        case PAGE_HOME:
            updateHomePage();      //主页面
            break;

        case PAGE_SETTINGS:
            updateSettingsPage();  //设置页面
            break;

        case PAGE_STORAGE:
            //updateStoragePage();   //存储页面
            break;

        case PAGE_DATETIME:
            //updateDatetimePage();  //日期时间页面
            break;
        case PAGE_MONITOR:
            updateMonitorPage();  //电参数监视页面
            break;    
        case PAGE_SCANNER:
            //updateScannerPage();  //扫码页面
            break;

        

        default:
            break;
    }
}



 // 向液晶屏发送数据
 INT8U sendDataToLCD(INT8U* pData, INT32S nDatalen) {
    INT8U	l_ret = 0;
    INT8U   nCom = COMM_232;  // 对应串口 COMM7:  P15--UART0_TX, P14--UART0_RX  

    if ((NULL == pData) || (nDatalen < 0) ) {
        return -1;
    }
    //V7_TX_ON();

    l_ret = WriteComm((INT8U)nCom, pData, nDatalen);

    return l_ret;
}


// 十进制整数转IEEE 754单精度float的编码（以uint32_t返回）
INT32U int_to_ieee754(int n) {
    if(n == 0) return 0; // 浮点数0的编码

    // Step 1: 符号位
    INT32U sign = 0;
    if (n < 0) 
    {
        sign = 1;
        n = -n;
    }

    // Step 2: 找到最高位1所在的bit（即指数）
    int exponent = 0;
    int temp = n;
    while (temp >>= 1) exponent++;

    // Step 3: 计算尾数
    // 去掉最高位的那个1，其余位左移到23位
    INT32U mantissa = ((INT32U)n & ((1U << exponent) - 1)) << (23 - exponent);

    // Step 4: 指数编码（加偏移量127）
    INT32U exp_encoded = exponent + 127;

    // Step 5: 组合
    INT32U ieee = (sign << 31) | (exp_encoded << 23) | mantissa;
    return ieee;
}

INT64U int_to_ieee754_double(int n) {
    if (n == 0) return 0; // 0的编码

    // Step 1: 符号位
    INT64U sign = 0;
    if (n < 0) {
        sign = 1;
        n = -n;
    }

    // Step 2: 找到最高位1所在的bit（即指数）
    int exponent = 0;
    int temp = n;
    while (temp >>= 1) exponent++;

    // Step 3: 计算尾数
    // 去掉最高位的那个1，其余位左移到52位
    INT64U mantissa = ((INT64U)n & ((1ULL << exponent) - 1)) << (52 - exponent);

    // Step 4: 指数编码（加偏移量1023）
    INT64U exp_encoded = exponent + 1023;

    // Step 5: 组合
    INT64U ieee = (sign << 63) | (exp_encoded << 52) | mantissa;
    return ieee;
}




/******************************************************************************
*更新首页显示内容 
*迪文屏ram变量地址存储空间使用范围：0x5000~0xffff；每个变量地址对应相应空间的2字节，对于字型（2字节）的指令或数据，总是采用高字节先传送的方式（MSB）；
*日期时间变量地址0x0010，该地址固定，不能自定义
*进线A/B/C状态，出线，相别的变量地址为：0x1000，0x1030, 0x1060, 0x1090, 0x10C0； 数据：0x0001:接入,0X0000:未接入
*电压V，电流A，功率kW，电能kWh，CPU温度的变量地址为：0x1120, 0x1150, 0x1180, 0x11B0, 0x1210； 数据: ASCII码具体长度根据数据位调整
*用户号码，提示信息，“连接/断开”指示灯的变量地址为：0x10F0, 0x11E0, 0x1240；
******************************************************************************/
void updateHomePage() 
{
    INT8U  		i = 0;  
    INT8U       Data[92] 			= {0}; 
    INT8U       nBuf[10] 			= {0}; 
    INT16U 		offset 				= 0;
	INT16U 		nAddr[14] 			= {0x1000,0x1030,0x1060,0x1090,0x10c0,0x10f0,0x1120,0x1150,0x1180,0x11b0,0x11e0,0x1210,0x1240};
    INT8U       dateTimeArray[8] 	= {0};
    STD_TIME        sTime;
	INT8U       displayDateTimeCmd[16];
    INT8U       nTempBuf[50];
    INT8U       nStatus             = 0;
    INT32U      nConvert            = 0;
    INT32U      nNum                = 0;
    INT64U      nValue            = 0;
    
    MT_DGN_C_ENERGY_DATA sAcCombEnergy;
    
    MemReadAcRealData(F5, &sAcCombEnergy.sPPEnergy[0], sizeof(sAcCombEnergy));

	//进线A的状态
    offset = 0;
    Data[offset++]=0x00; 
 
    if (wiringStatusBitmap & 0x01) 
	{
        Data[offset++] = 0x01;        
    }    
    else   
	{
		Data[offset++] = 0x00;   
	}  
	
	offset = DwinMakeFrm(&Data[0],offset,nAddr[0],0x82);
	
    sendDataToLCD(Data, offset); // 发送进线A状态数据

	OSTimeDlyHMSM(0,0,0,5);
	
	//显示进出线状态及相别_B  
	memset(Data,0,sizeof(Data));
    offset = 0; // 重置偏移量
	Data[offset++]=0x00; 
    if (wiringStatusBitmap & 0x02) 
	{
        Data[offset++] = 0x01;    //进线B的状态    
    }    
    else  
	{
		Data[offset++] = 0x00; 
	}

    offset = DwinMakeFrm(&Data[0],offset,nAddr[1],0x82);
	
    sendDataToLCD(Data, offset); // 发送进线B状态数据
	
	OSTimeDlyHMSM(0,0,0,5);
	
	//显示进出线状态及相别_C  
	memset(Data,0,sizeof(Data));
    offset = 0; // 重置偏移量
    Data[offset++]=0x00; 
    if (wiringStatusBitmap & 0x04) 
	{
        Data[offset++] = 0x01;    //进线C的状态    
    }    
    else    
	{
		Data[offset++] = 0x00;  
	}

	offset = DwinMakeFrm(&Data[0],offset,nAddr[2],0x82);
	
    sendDataToLCD(Data, offset); // 发送进线C状态数据
	
	OSTimeDlyHMSM(0,0,0,5);
	
	//显示出线状态_出线 
	memset(Data,0,sizeof(Data));
    offset = 0; // 重置偏移量
    Data[offset++]=0x00; 
    if (wiringStatusBitmap & 0x10) 
	{
        Data[offset++] = 0x01;    //出线的状态    
    }    
    else
	{
		Data[offset++] = 0x00; 
	}
	
    offset = DwinMakeFrm(&Data[0],offset,nAddr[3],0x82);
    sendDataToLCD(Data, offset);
	OSTimeDlyHMSM(0,0,0,5);
	
	 //显示相别_出线  
	memset(Data,0,sizeof(Data));
    offset = 0; // 重置偏移量
    Data[offset++]=0x00; 
    nStatus = wiringStatusBitmap & 0x0f;
    switch (nStatus)
    {
		case 1:
			Data[offset++]=0x01;      //A相
			break;
		case 2:
			Data[offset++]=0x02;      //B相 
			break;    
		case 4:
			Data[offset++]=0x03;      //C相 
			break;            
		default:
            Data[offset++]=0x00;      //？
			break;
    }

    offset = DwinMakeFrm(&Data[0],offset,nAddr[4],0x82);
    sendDataToLCD(Data, offset);
	
	// 显示用户号码
	OSTimeDlyHMSM(0,0,0,5);
	memset(Data,0,sizeof(Data));
    offset = 0; // 重置偏移量

    Data[offset++] = 0x00;
    Data[offset++] = 0x00;  
    Data[offset++] = 0x00;
    Data[offset++] = 0x17; 
    Data[offset++] = 0x48;
    Data[offset++] = 0x76;   
    Data[offset++] = 0xE7;
    Data[offset++] = 0xFF;
  
	offset = DwinMakeFrm(&Data[0],offset,nAddr[5],0x82);
    sendDataToLCD(Data, offset);

	// 显示电压
	OSTimeDlyHMSM(0,0,0,5);
	memset(Data,0,sizeof(Data));
	memset(nBuf,0,sizeof(nBuf));
    offset = 0; // 重置偏移量
	//memcpy(nBuf, &pDataTable->sRealInstant.sRealVolt[i], 2);
    nConvert = Bcdbuf2Long(&pDataTable->sRealInstant.sRealVolt[i].nFrac, 2);
    nConvert = 220;
    nNum = int_to_ieee754(nConvert);
    //test    
    Data[offset++] = (nNum>>24)&0xFF;    
    Data[offset++] = (nNum>>16)&0xFF;    
    Data[offset++] = (nNum>>8)&0xFF;    
    Data[offset++] = nNum&0xFF;    

	offset = DwinMakeFrm(&Data[0],offset,nAddr[6],0x82);
    sendDataToLCD(Data, offset);  
 
	OSTimeDlyHMSM(0,0,0,5);
	memset(Data,0,sizeof(Data));
    offset = 0; // 重置偏移量
    // 显示电流 
	//memcpy(Data, &pDataTable->sRealInstant.sRealCurr[0], 3);
    nConvert = Bcdbuf2Long(&pDataTable->sRealInstant.sRealCurr[0].nD1, 3); 
    nConvert = 220;
    nNum = int_to_ieee754(nConvert);
    memset(nTempBuf, 0, sizeof(nTempBuf));
    Long2Buf(nTempBuf, nNum, 4);
    ReverseOutput(nTempBuf,Data, 4);
    /*//test    
    Data[offset++] = (nNum>>24)&0xFF;    
    Data[offset++] = (nNum>>16)&0xFF;    
    Data[offset++] = (nNum>>8)&0xFF;    
    Data[offset++] = nNum&0xFF;*/
    offset = 4;  
    offset = DwinMakeFrm(&Data[0],offset,nAddr[7],0x82);
    sendDataToLCD(Data, offset);  
	
	OSTimeDlyHMSM(0,0,0,5);
	memset(Data,0,sizeof(Data));
    offset = 0; // 重置偏移量
    //显示功率 
	memcpy(Data, &pDataTable->sRealInstant.sRealP[0], 3);
    nConvert = Bcdbuf2Long(&pDataTable->sRealInstant.sRealP[i].nFrac0, 3);
    memset(nTempBuf, 0, sizeof(nTempBuf));
    nNum = int_to_ieee754(nConvert);
    Long2Buf(nTempBuf, nNum, 4);
    ReverseOutput(nTempBuf, Data, 4);
    offset += 4;  

    offset = DwinMakeFrm(&Data[0],offset,nAddr[8],0x82);
    sendDataToLCD(Data, offset);  
	
	OSTimeDlyHMSM(0,0,0,5);
	memset(Data,0,sizeof(Data));

    offset = 0; // 重置偏移量
    //显示电量  
	//memcpy(&nTempBuf[0], &sAcCombEnergy.sPPEnergy[i], sizeof(FORMAT11));
    nValue = Bcdbuf2Long(&sAcCombEnergy.sPPEnergy[0].nD1, sizeof(FORMAT11));
    nValue  = 2200;
    nValue = int_to_ieee754_double(nValue);
    Long2Buf(nTempBuf, nValue, 8);
    ReverseOutput(nTempBuf,Data, 8);
    offset += 8;

    offset = DwinMakeFrm(&Data[0],offset,nAddr[9],0x82);
    sendDataToLCD(Data, offset);  
	
	OSTimeDlyHMSM(0,0,0,5);
	memset(Data,0,sizeof(Data));
	
    offset = 0; // 重置偏移量
    //显示提示信息
#if 1
    //test 工作
    Data[offset++] = 0xB9;    
    Data[offset++] = 0xA4; 
    Data[offset++] = 0xD7;    
    Data[offset++] = 0xF7;      
#else
    //test 空闲
    Data[offset++] = 0xbf;    
    Data[offset++] = 0xd5; 
    Data[offset++] = 0xcf;    
    Data[offset++] = 0xd0;    
#endif
    // 计算CRC并发送
    offset = DwinMakeFrm(&Data[0],offset,nAddr[10],0x82);
    sendDataToLCD(Data, offset);  
	
	OSTimeDlyHMSM(0,0,0,5);
	memset(Data,0,sizeof(Data));   

    offset = 0; // 重置偏移量
    //显示CPU温度
    Data[offset++] = 0x44;
    Data[offset++] = 0x79;    
    Data[offset++] = 0xF9; 
    Data[offset++] = 0x9A;        
    
    offset = DwinMakeFrm(&Data[0],offset,nAddr[11],0x82);
    sendDataToLCD(Data, offset);  
	
	OSTimeDlyHMSM(0,0,0,5);
	memset(Data,0,sizeof(Data));   

    offset = 0; // 重置偏移量
    //显示连接状态
/*    
    displayLinkstatCmd[offset++] = GlbHmPgInf_Nsp.ConStaus;    //“连接/断开”指示灯   
*/
	Data[offset++] = 0x00; 
    
    if(g_pKeySamp->nKeyValBak == 0x01)
    {
       Data[offset++] = 0x01;    //“连接”指示灯 
    }
    else
    {
        Data[offset++] = 0x00;    //“断开”指示灯
    }

    offset = DwinMakeFrm(&Data[0],offset,nAddr[12],0x82);
    sendDataToLCD(Data, offset);  
	
	//OSTimeDlyHMSM(0,0,0,5);
	//memset(Data,0,sizeof(Data));  



}

void updateSettingsPage()
{
    
}

//电参数界面
void updateMonitorPage()
{
    INT8U  		i = 0;  
    INT8U       Data[92] 			= {0}; 
    INT8U       nBuf[10] 			= {0}; 
    INT16U 		offset 				= 0;
	INT16U 		nAddr[14] 			= {0x1000,0x1030,0x1060,0x1090,0x10c0,0x10f0,0x1120,0x1150,0x1180,0x11b0,0x11e0,0x1210,0x1240};
    INT8U       dateTimeArray[8] 	= {0};
    STD_TIME        sTime;
	INT8U       displayDateTimeCmd[16];
    INT8U       nTempBuf[50];
    INT8U       nStatus             = 0;
    INT32U      nConvert            = 0;
    INT32U      nNum                = 0;
    INT64U      nValue            = 0;
    
    MT_DGN_C_ENERGY_DATA sAcCombEnergy;
    
    MemReadAcRealData(F5, &sAcCombEnergy.sPPEnergy[0], sizeof(sAcCombEnergy));
    
    // 电压
	memset(Data,0,sizeof(Data));
	memset(nBuf,0,sizeof(nBuf));
    
    offset = 0; // 重置偏移量
    nConvert = Bcdbuf2Long(&pDataTable->sRealInstant.sRealVolt[i].nFrac, 2);
    nConvert = 220;
    nNum = int_to_ieee754(nConvert);  
	offset = DwinMakeFrm(&Data[0],offset,nAddr[6],0x82);
    sendDataToLCD(Data, offset);  
 
	OSTimeDlyHMSM(0,0,0,5);
	memset(Data,0,sizeof(Data));
    offset = 0; // 重置偏移量
    // 电流 
	//memcpy(Data, &pDataTable->sRealInstant.sRealCurr[0], 3);
    nConvert = Bcdbuf2Long(&pDataTable->sRealInstant.sRealCurr[0].nD1, 3); 
    nConvert = 220;
    nNum = int_to_ieee754(nConvert);
    memset(nTempBuf, 0, sizeof(nTempBuf));
    Long2Buf(nTempBuf, nNum, 4);
    ReverseOutput(nTempBuf,Data, 4);
    /*//test    
    Data[offset++] = (nNum>>24)&0xFF;    
    Data[offset++] = (nNum>>16)&0xFF;    
    Data[offset++] = (nNum>>8)&0xFF;    
    Data[offset++] = nNum&0xFF;*/
    offset = 4;  
    offset = DwinMakeFrm(&Data[0],offset,nAddr[7],0x82);
    sendDataToLCD(Data, offset);  
	
	OSTimeDlyHMSM(0,0,0,5);
	memset(Data,0,sizeof(Data));
    offset = 0; // 重置偏移量
    //功率 
	memcpy(Data, &pDataTable->sRealInstant.sRealP[0], 3);
    nConvert = Bcdbuf2Long(&pDataTable->sRealInstant.sRealP[i].nFrac0, 3);
    memset(nTempBuf, 0, sizeof(nTempBuf));
    nNum = int_to_ieee754(nConvert);
    Long2Buf(nTempBuf, nNum, 4);
    ReverseOutput(nTempBuf, Data, 4);
    offset += 4;  

    offset = DwinMakeFrm(&Data[0],offset,nAddr[8],0x82);
    sendDataToLCD(Data, offset);  
	
	OSTimeDlyHMSM(0,0,0,5);
	memset(Data,0,sizeof(Data));

    offset = 0; // 重置偏移量
    //显示电量  
	//memcpy(&nTempBuf[0], &sAcCombEnergy.sPPEnergy[i], sizeof(FORMAT11));
    nValue = Bcdbuf2Long(&sAcCombEnergy.sPPEnergy[0].nD1, sizeof(FORMAT11));
    nValue  = 2200;
    nValue = int_to_ieee754_double(nValue);
    Long2Buf(nTempBuf, nValue, 8);
    ReverseOutput(nTempBuf,Data, 8);
    offset += 8;

    offset = DwinMakeFrm(&Data[0],offset,nAddr[9],0x82);
    sendDataToLCD(Data, offset);  
	
	OSTimeDlyHMSM(0,0,0,5);
	memset(Data,0,sizeof(Data));
    
    offset = 0;
    //功率因数
	memcpy(Data, &pDataTable->sRealInstant.sRealPf[0], 2);
    nConvert = Bcdbuf2Long(&pDataTable->sRealInstant.sRealPf[0].nFrac, 2);
    memset(nTempBuf, 0, sizeof(nTempBuf));
    nNum = int_to_ieee754(nConvert);
    Long2Buf(nTempBuf, nNum, 4);
    ReverseOutput(nTempBuf, Data, 4);
    offset += 4;  

    offset = DwinMakeFrm(&Data[0],offset,nAddr[8],0x82);
    sendDataToLCD(Data, offset);  
	
	OSTimeDlyHMSM(0,0,0,5);
	memset(Data,0,sizeof(Data));
    
    //CO2排量
    
    //温度
    offset = 0;
    memcpy(Data, &pDataTable->sTpSlip.nVal, 4);
    memset(nTempBuf, 0, sizeof(nTempBuf));
    nNum = int_to_ieee754(nConvert);
    Long2Buf(nTempBuf, nNum, 4);
    ReverseOutput(nTempBuf, Data, 4);
    offset += 4;  

    offset = DwinMakeFrm(&Data[0],offset,nAddr[8],0x82);
    sendDataToLCD(Data, offset);  
	
	OSTimeDlyHMSM(0,0,0,5);
	memset(Data,0,sizeof(Data));
    //频率
    
}

//时间设置界面
void updateDatetimePage()
{
    
}



























  
//单相表显示  对于Data[16]位的值由0x04改成0x00,目的是去掉显示上的小房子标识

//INT8U  Data2[23] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x04, 0, 0x04, 0, 0, 0, 0,};   // 初始化数组
//INT8U  Data2Copy[23] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x04, 0, 0x04, 0, 0, 0, 0,}; 


INT8U  Data2[23] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0x04, 0, 0, 0, 0,};   // 初始化数组

INT8U  Data2Copy[23] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0x04, 0, 0, 0, 0,}; 


INT8U  DataAll_2[20] = {0xe0,0xb0,  0x60,0x00,  0xc0,0x70,  0xe0,0x50,   0x60,0xc0,  
                            0xa0,0xd0, 0xa0,0xf0,  0xe0,0x00,   0xe0,0xf0,  0xe0,0xd0
                          };   // 初始化数组
INT8U  Data_P_2[20] = {0xf0,0xb0,  0x70,0x00,  0xd0,0x70,  0xf0,0x50,   0x70,0xc0,  
                            0xb0,0xd0, 0xb0,0xf0,  0xf0,0x00,   0xf0,0xf0,  0xf0,0xd0
                          };   // 初始化数组
//第 1 屏  当前组合有功总电量 
void  LcdDisplayPage_1(void)                  //每秒刷新一次
{    
    INT8U       Data[23] ;  
    INT8U       nBuf[4];
    MT_DGN_C_ENERGY_DATA sAcCombEnergy;  

    memcpy(Data,Data2,23); 
    memset(&sAcCombEnergy, 0, sizeof(MT_DGN_C_ENERGY_DATA));
    MemReadAcRealData(F5, &sAcCombEnergy, sizeof(sAcCombEnergy));
    memcpy(nBuf,  &sAcCombEnergy.sPPEnergy[0],  sizeof(FORMAT11));
    if (g_MeterAcErrFlg[0])
    {
        nBuf[0] = 0x66;
        nBuf[3] = 0x78;
    }
    else if (g_MeterAcErrFlg[3])
    {
        nBuf[0] = 0x66;
        nBuf[1] = 0x34;
        nBuf[2] = 0x05;
    }
    sAcCombEnergyData(nBuf, Data);
    Data[14] += 0x04;
    Data[18] += 0x08;                  // 当前

    BULCDWrite1(0, Data, 23);
}
//第 2 屏  当前组合有功尖电量 
void  LcdDisplayPage_2(void)                  //每秒刷新一次
{    
    INT8U       Data[23] ;  
    INT8U       nBuf[4];
    MT_DGN_C_ENERGY_DATA sAcCombEnergy;  

    memcpy(Data,Data2,23); 
    memset(&sAcCombEnergy, 0, sizeof(MT_DGN_C_ENERGY_DATA));
    MemReadAcRealData(F5, &sAcCombEnergy, sizeof(sAcCombEnergy));
    memcpy(nBuf,  &sAcCombEnergy.sPPEnergy[1],  sizeof(FORMAT11));

    if (g_MeterAcErrFlg[0])
    {
        nBuf[0] = 0x66;
        nBuf[3] = 0x78;
    }
    else if (g_MeterAcErrFlg[3])
    {
        nBuf[0] = 0x33;
        nBuf[1] = 0x67;
        nBuf[2] = 0x04;
    }
    sAcCombEnergyData(nBuf, Data);
    Data[13] += 0x04;
    Data[18] += 0x08;                  // 当前

    BULCDWrite1(0, Data, 23);
}
//第 3 屏  当前组合有功峰电量 
void  LcdDisplayPage_3(void)                  //每秒刷新一次
{    
    INT8U       Data[23] ;  
    INT8U       nBuf[4];
    MT_DGN_C_ENERGY_DATA sAcCombEnergy;  

    memcpy(Data,Data2,23); 
    memset(&sAcCombEnergy, 0, sizeof(MT_DGN_C_ENERGY_DATA));
    MemReadAcRealData(F5, &sAcCombEnergy, sizeof(sAcCombEnergy));
    memcpy(nBuf,  &sAcCombEnergy.sPPEnergy[2],  sizeof(FORMAT11));

    sAcCombEnergyData(nBuf, Data);

    Data[12] += 0x04;
    Data[18] += 0x08;                  // 当前

    BULCDWrite1(0, Data, 23);
}

//第 4 屏  当前组合有功平电量 
void  LcdDisplayPage_4(void)                 
{    
    INT8U       Data[23] ;  
    INT8U       nBuf[4];
    MT_DGN_C_ENERGY_DATA sAcCombEnergy;  

    memcpy(Data,Data2,23); 
    memset(&sAcCombEnergy, 0, sizeof(MT_DGN_C_ENERGY_DATA));
    MemReadAcRealData(F5, &sAcCombEnergy, sizeof(sAcCombEnergy));
    memcpy(nBuf,  &sAcCombEnergy.sPPEnergy[3],  sizeof(FORMAT11));

    sAcCombEnergyData(nBuf,  Data);

    Data[11] += 0x04;     
    Data[18] += 0x08;                  // 当前

    BULCDWrite1(0, Data, 23);
}
//第 5屏  当前组合有功谷电量 
void  LcdDisplayPage_5(void)                  //每秒刷新一次
{ 
    INT8U       Data[23] ;  
    INT8U       nBuf[4];
    MT_DGN_C_ENERGY_DATA sAcCombEnergy;  

    memcpy(Data,Data2,23); 
    memset(&sAcCombEnergy, 0, sizeof(MT_DGN_C_ENERGY_DATA));
    MemReadAcRealData(F5, &sAcCombEnergy, sizeof(sAcCombEnergy));
    memcpy(nBuf,  &sAcCombEnergy.sPPEnergy[4],  sizeof(FORMAT11));

    sAcCombEnergyData(nBuf, Data);
    Data[10] += 0x04;
    Data[18] += 0x08;                  // 当前

    BULCDWrite1(0, Data, 23);
}

//第 6 屏  上 1 月组合有功总电量 
void  LcdDisplayPage_6(void)                  //每秒刷新一次
{    
    INT8U       Data[23] ;  
    INT8U       nBuf[4];
    MEM_AC_RDAY_DATA     sAcHistData;
    MT_DGN_C_ENERGY_DATA sAcCombEnergy;

    memcpy(Data,Data2,23); 
    memset(&sAcHistData, 0, sizeof(MEM_AC_RDAY_DATA));
    memset(&sAcCombEnergy, 0, sizeof(MT_DGN_C_ENERGY_DATA));
    MemReadAcHistDataProc(F2, 1, &sAcHistData, sizeof(sAcHistData));
    memcpy(&sAcCombEnergy, &sAcHistData.sAcCombEnergy, sizeof(sAcCombEnergy));
    memcpy(nBuf,  &sAcCombEnergy.sPPEnergy[0],  sizeof(FORMAT11));

    sAcCombEnergyData(nBuf,  Data);
    Data[14] += 0x04;
    Data[16] += 0xc0; 
    Data[20] += 0x04;  // 上1月
    Data[17] += 0x80;

    BULCDWrite1(0, Data, 23);
}
//第 7 屏  上 1 月组合有功尖电量 
void  LcdDisplayPage_7(void)                  //每秒刷新一次
{
    INT8U       Data[23] ;  
    INT8U       nBuf[4];
    MEM_AC_RDAY_DATA     sAcHistData;
    MT_DGN_C_ENERGY_DATA sAcCombEnergy;

    memcpy(Data, Data2, 23); 
    memset(&sAcHistData, 0, sizeof(MEM_AC_RDAY_DATA));
    memset(&sAcCombEnergy, 0, sizeof(MT_DGN_C_ENERGY_DATA));
    MemReadAcHistDataProc(F2, 1, &sAcHistData, sizeof(sAcHistData));
    memcpy(&sAcCombEnergy, &sAcHistData.sAcCombEnergy, sizeof(sAcCombEnergy));
    memcpy(nBuf,  &sAcCombEnergy.sPPEnergy[1],  sizeof(FORMAT11));

    sAcCombEnergyData(nBuf,  Data);
    Data[13] += 0x04;
    Data[16] += 0xc0; 
    Data[20] += 0x04;  // 上1月                  
    Data[17] += 0x80;

    BULCDWrite1(0, Data, 23);
}
//第 8 屏  上 1 月组合有功峰电量 
void  LcdDisplayPage_8(void)                  //每秒刷新一次
{    
    INT8U       Data[23] ;  
    INT8U       nBuf[4];
    MEM_AC_RDAY_DATA     sAcHistData;
   MT_DGN_C_ENERGY_DATA sAcCombEnergy;
      
    memcpy(Data,Data2,23); 
     memset(&sAcHistData, 0, sizeof(MEM_AC_RDAY_DATA));
    memset(&sAcCombEnergy, 0, sizeof(MT_DGN_C_ENERGY_DATA));
    MemReadAcHistDataProc(F2, 1, &sAcHistData, sizeof(sAcHistData));
    memcpy(&sAcCombEnergy, &sAcHistData.sAcCombEnergy, sizeof(sAcCombEnergy));
    memcpy(nBuf,  &sAcCombEnergy.sPPEnergy[2],  sizeof(FORMAT11));
    
    sAcCombEnergyData(nBuf,  Data);

    Data[12] += 0x04;
    Data[16] += 0xc0; 
    Data[20] += 0x04;                  // 上1月
    Data[17] += 0x80;
     
      BULCDWrite1(0, Data, 23);
}
//第 9 屏  上 1 月组合有功平电量 
void  LcdDisplayPage_9(void)                  //每秒刷新一次
{    
    INT8U       Data[23] ;  
    INT8U       nBuf[4];
     MEM_AC_RDAY_DATA     sAcHistData;
   MT_DGN_C_ENERGY_DATA sAcCombEnergy;
      
    memcpy(Data,Data2,23); 
     memset(&sAcHistData, 0, sizeof(MEM_AC_RDAY_DATA));
    memset(&sAcCombEnergy, 0, sizeof(MT_DGN_C_ENERGY_DATA));
    MemReadAcHistDataProc(F2, 1, &sAcHistData, sizeof(sAcHistData));
    memcpy(&sAcCombEnergy, &sAcHistData.sAcCombEnergy, sizeof(sAcCombEnergy));
    memcpy(nBuf,  &sAcCombEnergy.sPPEnergy[3],  sizeof(FORMAT11));
    
    sAcCombEnergyData(nBuf,  Data);

     Data[11] += 0x04;         
          Data[16] += 0xc0; 
          Data[20] += 0x04;                  // 上1月
          Data[17] += 0x80;
     
      BULCDWrite1(0, Data, 23);
}
//第 10 屏  上 1 月组合有功谷电量 
void  LcdDisplayPage_10(void)                  //每秒刷新一次
{    
    INT8U       Data[23] ;  
    INT8U       nBuf[4];
    MEM_AC_RDAY_DATA     sAcHistData;
   MT_DGN_C_ENERGY_DATA sAcCombEnergy;
      
    memcpy(Data,Data2,23); 
     memset(&sAcHistData, 0, sizeof(MEM_AC_RDAY_DATA));
    memset(&sAcCombEnergy, 0, sizeof(MT_DGN_C_ENERGY_DATA));
    MemReadAcHistDataProc(F2, 1, &sAcHistData, sizeof(sAcHistData));
    memcpy(&sAcCombEnergy, &sAcHistData.sAcCombEnergy, sizeof(sAcCombEnergy));
    memcpy(nBuf,  &sAcCombEnergy.sPPEnergy[4],  sizeof(FORMAT11));
    
   sAcCombEnergyData(nBuf,  Data);

     Data[10] += 0x04;
         Data[16] += 0xc0; 
          Data[20] += 0x04;                   // 上1月
          Data[17] += 0x80;
     
      BULCDWrite1(0, Data, 23);
}
//第 11 屏  上 2 月组合有功总电量 
void  LcdDisplayPage_11(void)                  //每秒刷新一次
{    
    INT8U       Data[23] ;  
    INT8U       nBuf[4];
    MEM_AC_RDAY_DATA     sAcHistData;
    MT_DGN_C_ENERGY_DATA sAcCombEnergy;
      
    memcpy(Data,Data2,23); 
    memset(&sAcHistData, 0, sizeof(MEM_AC_RDAY_DATA));
    memset(&sAcCombEnergy, 0, sizeof(MT_DGN_C_ENERGY_DATA));
    MemReadAcHistDataProc(F2, 2, &sAcHistData, sizeof(sAcHistData));
    memcpy(&sAcCombEnergy, &sAcHistData.sAcCombEnergy, sizeof(sAcCombEnergy));
    memcpy(nBuf,  &sAcCombEnergy.sPPEnergy[0],  sizeof(FORMAT11));
    
    sAcCombEnergyData(nBuf,  Data);

    Data[14] += 0x04;    
    Data[16] += 0xc0; 
    Data[20] += 0x78;                   // 上2月
    Data[17] += 0x80;
     
    BULCDWrite1(0, Data, 23);
}
//第 12屏  上 2 月组合有功尖电量 
void  LcdDisplayPage_12(void)                  //每秒刷新一次
{    
    INT8U       Data[23] ;  
    INT8U       nBuf[4];
     MEM_AC_RDAY_DATA     sAcHistData;
   MT_DGN_C_ENERGY_DATA sAcCombEnergy;
      
    memcpy(Data,Data2,23); 
    memset(&sAcHistData, 0, sizeof(MEM_AC_RDAY_DATA));
    memset(&sAcCombEnergy, 0, sizeof(MT_DGN_C_ENERGY_DATA));
    MemReadAcHistDataProc(F2, 2, &sAcHistData, sizeof(sAcHistData));
    memcpy(&sAcCombEnergy, &sAcHistData.sAcCombEnergy, sizeof(sAcCombEnergy));
    memcpy(nBuf,  &sAcCombEnergy.sPPEnergy[1],  sizeof(FORMAT11));
    
    sAcCombEnergyData(nBuf,  Data);

    Data[13] += 0x04;    
    Data[16] += 0xc0; 
    Data[20] += 0x78;                 // 上2月
    Data[17] += 0x80;
     
      BULCDWrite1(0, Data, 23);
}
//第 13 屏  上 2 月组合有功峰电量 
void  LcdDisplayPage_13(void)                  //每秒刷新一次
{    
    INT8U       Data[23] ;  
    INT8U       nBuf[4];
    MEM_AC_RDAY_DATA     sAcHistData;
   MT_DGN_C_ENERGY_DATA sAcCombEnergy;
      
    memcpy(Data, Data2, 23); 
    memset(&sAcHistData, 0, sizeof(MEM_AC_RDAY_DATA));
    memset(&sAcCombEnergy, 0, sizeof(MT_DGN_C_ENERGY_DATA));
    MemReadAcHistDataProc(F2, 2, &sAcHistData, sizeof(sAcHistData));
    memcpy(&sAcCombEnergy, &sAcHistData.sAcCombEnergy, sizeof(sAcCombEnergy));
    memcpy(nBuf,  &sAcCombEnergy.sPPEnergy[2],  sizeof(FORMAT11));

    sAcCombEnergyData(nBuf,  Data);
     Data[12] += 0x04;     
       Data[16] += 0xc0; 
          Data[20] += 0x78;                  // 上2月
          Data[17] += 0x80;
     
      BULCDWrite1(0, Data, 23);
}
//第 14 屏  上 2 月组合有功平电量 
void  LcdDisplayPage_14(void)                  //每秒刷新一次
{    
    INT8U       Data[23] ;  
    INT8U       nBuf[4];
    MEM_AC_RDAY_DATA     sAcHistData;
   MT_DGN_C_ENERGY_DATA sAcCombEnergy;
      
    memcpy(Data,Data2,23); 
    memset(&sAcHistData, 0, sizeof(MEM_AC_RDAY_DATA));
    memset(&sAcCombEnergy, 0, sizeof(MT_DGN_C_ENERGY_DATA));
    MemReadAcHistDataProc(F2, 2, &sAcHistData, sizeof(sAcHistData));
    memcpy(&sAcCombEnergy, &sAcHistData.sAcCombEnergy, sizeof(sAcCombEnergy));
    memcpy(nBuf,  &sAcCombEnergy.sPPEnergy[3],  sizeof(FORMAT11));
    
    sAcCombEnergyData(nBuf,  Data);

     Data[11] += 0x04;         
        Data[16] += 0xc0; 
          Data[20] += 0x78;                  // 上2月
          Data[17] += 0x80;
     
      BULCDWrite1(0, Data, 23);
}
//第 15 屏  上 2 月组合有功谷电量 
void  LcdDisplayPage_15(void)                  //每秒刷新一次
{    
    INT8U       Data[23] ;  
    INT8U       nBuf[4];
    MEM_AC_RDAY_DATA     sAcHistData;
   MT_DGN_C_ENERGY_DATA sAcCombEnergy;
      
    memcpy(Data,Data2,23); 
    memset(&sAcHistData, 0, sizeof(MEM_AC_RDAY_DATA));
    memset(&sAcCombEnergy, 0, sizeof(MT_DGN_C_ENERGY_DATA));
    MemReadAcHistDataProc(F2, 2, &sAcHistData, sizeof(sAcHistData));
    memcpy(&sAcCombEnergy, &sAcHistData.sAcCombEnergy, sizeof(sAcCombEnergy));
    memcpy(nBuf,  &sAcCombEnergy.sPPEnergy[4],  sizeof(FORMAT11));

    sAcCombEnergyData(nBuf,  Data);
     Data[10] += 0x04;    
        Data[16] += 0xc0; 
          Data[20] += 0x78;                  // 上2月
          Data[17] += 0x80;
      BULCDWrite1(0, Data, 23);
}
//第 16屏  通信地址低 8位
void  LcdDisplayPage_16(void)                  //每秒刷新一次
{    
    INT8U       i ;
    INT8U       Data[23];
    INT8U       nBuf1[8];
    INT8U       nBuf[6];
    INT8U        nMemAddr[6];
        
    memcpy(Data,Data2,23); 
     memset(&nBuf, 0, sizeof(nBuf));
    MemReadAcPrm(F19, &nMemAddr[0], 6);
    memcpy(nBuf, &nMemAddr[0], 6);
     if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }
       
     for(i = 0; i < 4; i ++ )
    {
      nBuf1[i*2]  = nBuf[i]&0x0f;
      nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }
                                                    
     for(i = 5; i < 8; i ++)
        {   Data[i*2] += DataAll_2[nBuf1[i- 5] *2];
            Data[i*2 + 1] += DataAll_2[nBuf1[i-5] *2 + 1];
        }
      for(i = 0; i < 4; i ++)
        {   Data[i*2] += DataAll_2[nBuf1[i + 3] *2];
            Data[i*2 + 1] += DataAll_2[nBuf1[i + 3] *2 + 1];
        }
      Data[21] += DataAll_2[nBuf1[7] *2];
      Data[22] += DataAll_2[nBuf1[7] *2 + 1];
      BULCDWrite1(0, Data, 23);     
}
//第 17 屏  通信地址高 4 位
void  LcdDisplayPage_17(void)                  //每秒刷新一次
{   INT8U       i ;
    INT8U       Data[23];
    INT8U       nBuf1[4];
    INT8U       nBuf[6];
    INT8U        nMemAddr[6];

    memcpy(Data,Data2,23); 
    memset(&nBuf, 0, sizeof(nBuf));
    MemReadAcPrm(F19, &nMemAddr[0], 6);
    memcpy(nBuf, &nMemAddr[0], 6);
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }
       
     for(i = 0; i < 2; i ++ )
    {
      nBuf1[i*2]  = nBuf[i +4]&0x0f;
      nBuf1[i*2+1]  = nBuf[i +4]>>0x04;
    }

     for(i = 5; i < 8; i ++)
    {   Data[i*2] += DataAll_2[nBuf1[i- 5] *2];
        Data[i*2 + 1] += DataAll_2[nBuf1[i-5] *2 + 1];
    }
     Data[0] += DataAll_2[nBuf1[3] *2];
     Data[1] += DataAll_2[nBuf1[3] *2 + 1]; 
      BULCDWrite1(0, Data, 23);     
}


//第 18 屏 日期
void LcdDisplayPage_18(void) 
 
 { INT8U       i  = 0;
    STD_TIME    sTime;
    pSTD_TIME   pTime = &sTime;
    INT8U       Data[23];
    INT8U       nBuf1[6];
    INT8U       nBuf[3];
    
      memcpy(Data,Data2,23);    
     memset(pTime, 0, sizeof(STD_TIME));
      GetSysClk(&sTime);
      
      nBuf[i++] = sTime.nDay;
      nBuf[i++] = sTime.nMon;
      nBuf[i++] = sTime.nYear;
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }
     if(0x02==(g_MeterInsideWrong[0]&0x02)) //日期显示故障
     {
        if (nBuf[0]<0x27)
        {
            nBuf[0] +=2;
        }
        else
        {
            nBuf[0] = 3;
        }
    }
     if(0x20==(g_MeterInsideWrong[0]&0x20)) //时钟故障
     {
         for(i=0;i<7;i++)
         {
           Data[i]|=0x80;
         }
         for(i=10;i<15;i++)
         {
           Data[i]|=0x80;
         }
     }
     for(i = 0; i < 3; i ++ )
    {
      nBuf1[i*2]  = nBuf[i]&0x0f;
      nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }
      Data[10] += DataAll_2[nBuf1[0] *2];
      Data[11] += DataAll_2[nBuf1[0] *2 + 1] + 0x08;
       Data[12] += DataAll_2[nBuf1[1] *2] + 0x08;
      Data[13] += DataAll_2[nBuf1[1] *2 +1];
      
      Data[14] = Data_P_2[nBuf1[2] *2];
      Data[15] = Data_P_2[nBuf1[2] *2 +1];
      
      Data[0] = DataAll_2[nBuf1[3] *2];
      Data[1] = DataAll_2[nBuf1[3] *2 +1];
      
      Data[2] = Data_P_2[nBuf1[4] *2];
      Data[3] = Data_P_2[nBuf1[4] *2 +1 ];
      Data[4] += DataAll_2[nBuf1[5] *2 ];
      Data[5] += DataAll_2[nBuf1[5] *2 +1];
      Data[18] += 0x08;// 当前
    BULCDWrite1(0, Data, 23); 
   }
//第 19 屏  时间
void  LcdDisplayPage_19(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;
    STD_TIME    sTime;
    pSTD_TIME   pTime = &sTime;
    INT8U       Data[23];
    INT8U       nBuf1[6];
    INT8U       nBuf[3];

    memcpy(Data,Data2,23); 
     memset(pTime, 0, sizeof(STD_TIME));
      GetSysClk(&sTime);
      
      nBuf[i++] = sTime.nSec;
      nBuf[i++] = sTime.nMin;
      nBuf[i++] = sTime.nHour;    
     if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }
     if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }
     if(0x02==(g_MeterInsideWrong[0]&0x02)) //时间显示故障模拟
     {
        if (sTime.nMin < 0x48)
        {
            nBuf[1] = sTime.nMin+0x10;
        }
        else
        {
            nBuf[1] = 5;
        }
     } 
      if(0x20==(g_MeterInsideWrong[0]&0x20)) //时钟故障
      {
         for(i=0;i<7;i++)
         {
           Data[i]|=0x80;
         }
         for(i=10;i<15;i++)
         {
           Data[i]|=0x80;
         }
      }
    for(i = 0; i < 3; i ++ )
    {
      nBuf1[i*2]  = nBuf[i]&0x0f;
      nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }
      Data[10] += DataAll_2[nBuf1[0] *2];
      Data[11] += DataAll_2[nBuf1[0] *2 + 1] + 0x08;
       Data[12] += DataAll_2[nBuf1[1] *2] + 0x08;
      Data[13] += DataAll_2[nBuf1[1] *2 +1];
      
      Data[14] += Data_P_2[nBuf1[2] *2] + 0x08;
      Data[15] += Data_P_2[nBuf1[2] *2 +1];
      
      Data[0] += DataAll_2[nBuf1[3] *2];
      Data[1] += DataAll_2[nBuf1[3] *2 +1];
      
      Data[2] += Data_P_2[nBuf1[4] *2] + 0x08;
      Data[3] += Data_P_2[nBuf1[4] *2 +1 ];
      Data[4] += DataAll_2[nBuf1[5] *2 ];
      Data[5] += DataAll_2[nBuf1[5] *2 +1];
      Data[18] += 0x08;// 当前
    BULCDWrite1(0, Data, 23); 
}
//第 20 屏   电压
void  LcdDisplayPage_20(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;   
    INT8U       Data[23];
    INT8U       nBuf[2];
    INT8U       nBuf1[4]; 
    MEM_AC_REAL_DATA            sRealData;             //使用全局变量
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    memcpy(Data,Data2,23); 
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    memcpy(nBuf, &pDataTable->sRealInstant.sRealVolt[0].nFrac, 2);
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }
    
    //A相电压为50%
    if (((g_MeterHardwareWrong[1]&0x04)==0x04) || (1 == g_MeterEventWrong[0]&0x01))
    { 
        DivideTwo(nBuf);
    }
    
    if (0x11 == (g_MeterVoltPlusFlg&0x11))//电压越上限
    {
        nBuf[1] += 0x05;
    }
    else if((0x21 == (g_MeterVoltPlusFlg&0x21))||(g_MeterVoltPlusFlg == 0x01))//电压越下限
    {
        DivideTwo2(nBuf); //DivideTwo(nBuf);
    }

    //A相无电压
    if(((g_MeterHardwareWrong[0]&0x10)==0x10) || (0x02==(g_MeterEventWrong[0]&0x02)))
    {
       memset(nBuf, 0, sizeof(nBuf));
    }
    
    for(i = 0; i < 2; i ++ )
    {
      nBuf1[i*2]  = nBuf[i]&0x0f;
      nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }
  
      Data[10] |= DataAll_2[nBuf1[0] *2];
      Data[11] |= DataAll_2[nBuf1[0] *2 + 1];   
     Data[12] |= Data_P_2[nBuf1[1] *2];
     Data[13] |= Data_P_2[nBuf1[1] *2 + 1]; 
      Data[14] |= DataAll_2[nBuf1[2] *2];
      Data[15] |= DataAll_2[nBuf1[2] *2 + 1];   
     Data[0] |= DataAll_2[nBuf1[3] *2];
     Data[1] |= DataAll_2[nBuf1[3] *2 + 1];  
 
      Data[9] |= 0x20;
    BULCDWrite1(0, Data, 23);
}
//第 21屏   电流
void  LcdDisplayPage_21(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;   
    INT8U       Data[23];
    INT8U       nBuf[3];
    INT8U       nBuf1[6]; 
    MEM_AC_REAL_DATA            sRealData;             //使用全局变量
    pMEM_AC_REAL_DATA           pRealData = &sRealData;        
    
    memcpy(Data,Data2,23);  
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    memcpy(nBuf,  &pDataTable->sRealInstant.sRealCurr[0].nD1, 3);
     nBuf[2] &= 0x7f;   //不显方向位
     
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
        memset(nBuf, 0, sizeof(nBuf));
    }
    
    //A相电流为50%
    if((g_MeterHardwareWrong[1]&0x20)==0x20)
    { 
       DivideTwo(nBuf);
    }
    
    //A相无电流
    if (((g_MeterHardwareWrong[0]&0x80)==0x80) || (0x04==(g_MeterEventWrong[0]&0x04)) || (0x02==(g_MeterEventWrong[0]&0x02)))
    {
       memset(nBuf, 0, sizeof(nBuf));
    }
    
    for (i = 0; i < 3; i ++ )
    {
      nBuf1[i*2]  = nBuf[i]&0x0f;
      nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }
     for( i = 0; i < 3; i ++)
    { 
      Data[i*2 +10] |= DataAll_2[nBuf1[i] *2];
      Data[i*2 +11] |= DataAll_2[nBuf1[i] *2 + 1];   
    }
    
      Data[0] |= Data_P_2[nBuf1[3] *2];
      Data[1] |= Data_P_2[nBuf1[3] *2 + 1];    
      if(nBuf1[5]!= 0)       
      { for(i = 1;i < 3;i++)
        { Data[i*2] = DataAll_2[nBuf1[i + 3]* 2 ];
          Data[i*2 +1] = DataAll_2[nBuf1[i + 3]* 2 + 1];
        }
      }
      if(nBuf1[4]!= 0)
      {
        Data[2] = DataAll_2[nBuf1[4]* 2 ];
        Data[3] = DataAll_2[nBuf1[4]* 2 +1 ];
      }
  
      Data[8] |= 0x20;
      Data[19] |= 0x20;
    BULCDWrite1(0, Data, 23);
}
//第 22 屏  有功功率
void  LcdDisplayPage_22(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;   
    INT8U       j  = 0;  
    INT8U       Data[23];
    INT8U       nBuf[3];
    INT8U       nBuf1[8];    
    MEM_AC_REAL_DATA            sRealData;             //使用全局变量
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    memcpy(Data,Data2,23);  
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    memcpy(nBuf, &pDataTable->sRealInstant.sRealP[0].nFrac0, 3 );
    
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
        memset(nBuf, 0, sizeof(nBuf));
    } 
    
    for(i = 0; i < 3; i ++ )
    {
        nBuf1[i*2]  = nBuf[i]&0x0f;
        nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }
 
    Data[0*2 +10] += DataAll_2[nBuf1[0] *2];
    Data[0*2 +11] += DataAll_2[nBuf1[0] *2 + 1]; 
    Data[1*2 +10] += DataAll_2[nBuf1[1] *2];
    Data[1*2 +11] += DataAll_2[nBuf1[1] *2 + 1]; 
    Data[2*2 +10] += DataAll_2[nBuf1[2] *2];
    Data[2*2 +11] += DataAll_2[nBuf1[2] *2 + 1]; 
    Data[0] += DataAll_2[nBuf1[3] *2];
    Data[1] += DataAll_2[nBuf1[3] *2 + 1]; 
    Data[2] += Data_P_2[nBuf1[4] *2];
    Data[3] += Data_P_2[nBuf1[4] *2 + 1];
     
    for(j = 7; j >= 5; j --)
    {
        if(nBuf1[j]!= 0)       
        {    
            if(7 == j)
            {   
                Data[21] += DataAll_2[nBuf1[j] *2 ];
                Data[22] += DataAll_2[nBuf1[j] *2 + 1];
                j = 6;
            }
            for(i = 2; i < j-2; i ++)
            {  
                Data[i*2] += DataAll_2[nBuf1[i + 3] *2];
                Data[i*2 + 1] += DataAll_2[nBuf1[i + 3] *2 + 1];
            } 
            j = 5;
        }       
    } 

    Data[10] += 0x10; 
    
    BULCDWrite1(0, Data, 23); 
}

//第 23 屏  功率因数
void  LcdDisplayPage_23(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;   
    INT8U       Data[23];
    INT8U       nBuf[2];
    INT8U       nBuf1[4];    
    MEM_AC_REAL_DATA            sRealData;             //使用全局变量
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
      memcpy(Data,Data2,23);    
   MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    memcpy(nBuf, &pDataTable->sRealInstant.sRealPf[1].nFrac, 2 );
    
    nBuf[1] = nBuf[1] & 0x7f;
    
     if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    } 
    for(i = 0; i < 2; i ++ )
    {
      nBuf1[i*2]  = nBuf[i]&0x0f;
      nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }
      for(i = 0; i < 3; i ++ )
    {
      Data[i*2 +10] += DataAll_2[nBuf1[i] *2];
      Data[i*2 +11] += DataAll_2[nBuf1[i] *2 + 1];   
    }
      Data[0] += Data_P_2[nBuf1[3] *2];
      Data[1] += Data_P_2[nBuf1[3] *2 + 1];      
      
      Data[9] += 0x40;
    BULCDWrite1(0, Data, 23); 
}

//第 24屏   电流
void  LcdDisplayPage_24(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;   
    INT8U       Data[23];
    INT8U       nBuf[3];
    INT8U       nBuf1[6]; 
    INT8U       nTmp[2];
    INT32U      nNum;
    MEM_AC_REAL_DATA            sRealData;             //使用全局变量
    pMEM_AC_REAL_DATA           pRealData = &sRealData;        
    
    memcpy(Data,Data2,23); 
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    if(g_nSetShortcurFlg == 1)
    {
        nBuf[0] =0x96;
        nBuf[1] =0x29;
        nBuf[2] =0x00;
        
    }
    else
    {
        memcpy(nBuf,  &pDataTable->sRealInstant.sRealCurr[0].nD1, 3);
        nTmp[0]=BCD2Dec(nBuf[0]);
        nTmp[1]=BCD2Dec(nBuf[1]);
        nNum=(nTmp[0]+nTmp[1]*100)*0.995;
        nBuf[1]=Dec2BCD(nNum/100);
        nBuf[0]=Dec2BCD(nNum%100);
    }
     nBuf[2] &= 0x7f;   //不显方向位
     
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
        memset(nBuf, 0, sizeof(nBuf));
    }
    
    //A相电流为50%
    if((g_MeterHardwareWrong[1]&0x20)==0x20)
    { 
       DivideTwo(nBuf);
    }
    
    //A相无电流
    if (((g_MeterHardwareWrong[0]&0x80)==0x80) || (0x04==(g_MeterEventWrong[0]&0x04)) || (0x02==(g_MeterEventWrong[0]&0x02)))
    {
       memset(nBuf, 0, sizeof(nBuf));
    }
    
    for (i = 0; i < 3; i ++ )
    {
      nBuf1[i*2]  = nBuf[i]&0x0f;
      nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }
     for( i = 0; i < 3; i ++)
    { 
      Data[i*2 +10] |= DataAll_2[nBuf1[i] *2];
      Data[i*2 +11] |= DataAll_2[nBuf1[i] *2 + 1];   
    }
    
      Data[0] |= Data_P_2[nBuf1[3] *2];
      Data[1] |= Data_P_2[nBuf1[3] *2 + 1];    
      if(nBuf1[5]!= 0)       
      { for(i = 1;i < 3;i++)
        { Data[i*2] = DataAll_2[nBuf1[i + 3]* 2 ];
          Data[i*2 +1] = DataAll_2[nBuf1[i + 3]* 2 + 1];
        }
      }
      if(nBuf1[4]!= 0)
      {
        Data[2] = DataAll_2[nBuf1[4]* 2 ];
        Data[3] = DataAll_2[nBuf1[4]* 2 +1 ];
      }
  
      Data[8] |= 0x20;
      Data[18] |= 0x20;
    BULCDWrite1(0, Data, 23);
}

/*
/******************************************************************************************************
* 函数名: AcCalcEventState2
* 输　入:
* 输  出:
* 返  回:
* 功能描述:交采事件判断处理
******************************************************************************************************/
void AcCalcEventState2(void)
{
    INT8U       i  = 0; 
    INT8U       j  ;
    INT32U      nTmp = 0;
    INT8U       Data[35] = {0};
    INT8U       nBuf[2] = {0};
    FP32        nTmp1 = 100.0;
   STD_TIME     sTime;
       pSTD_TIME   pTime = &sTime;
    MEM_AC_REAL_DATA            sRealData;             //使用全局变量
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    AC_EVENT_STAT               sEventStat;
    pAC_EVENT_STAT              pEventStat;
    EVT_TRIP_DATA		        tEvtTrip;		//跳闸状态
    EVT_ALARM_DATA              tAlarm; 
    EVT_GUARPOWER_DATA	        tGuarantPower;		//保电状态
    
    //INT8U       nAcEventTrip = 0;
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    
    //MemReadAcXFlhBackData(F85, &nAcEventTrip, sizeof(nAcEventTrip));
    MemReadAcXFlhBackData(F85, &tEvtTrip, sizeof(tEvtTrip));
     GetSysClk(&sTime);
   //  if(( sTime.nHour >= 0x18)&&(sTime.nHour < 0x21 ))        //当前时间尖、峰、平、谷  
      if(1 == pDataTable->nRateNo) 
      { Data2[18]  &= 0xaf;
           Data2[19]  &= 0xef;
        Data2[19]  |= 0x40;
      }
    //else if(( sTime.nHour >= 0x08)&&(sTime.nHour < 0x11 ))            
    else if(2 == pDataTable->nRateNo) 
    {   Data2[18]  &= 0xef;
        Data2[19]  &= 0xaf;
        Data2[18]  |= 0x40;
      }
   // else if((( sTime.nHour >= 0x07)&&(sTime.nHour < 0x08 ))|| (( sTime.nHour >= 0x11)&&(sTime.nHour < 0x18 )))         
     else if(3 == pDataTable->nRateNo) 
      { Data2[18]  &= 0xaf;
           Data2[19]  &= 0xbf;
         
        Data2[19]  |= 0x10;
      }
      //else 
      else if(4 == pDataTable->nRateNo) 
      {   Data2[18]  &= 0xbf;
           Data2[19]  &= 0xaf;
        Data2[18]  |= 0x10;
      }
     
      MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
      memcpy(nBuf, &pDataTable->sRealInstant.sRealVolt[0].nFrac, 2);
      
      if( !IsAllEqualVal(&pDataTable->sRealInstant.sRealVolt[0].nFrac, 2, 0))
      { 
        g_nFlg_Pro = 1;
      }
      
    //if((pDataTable->sRealInstant.sRealP[1].nSint)&0x80)                                         //判断功率方向
 
    //if((pDataTable->nPFlag & (0x01 << 3))&&(!(pDataTable->sRealInstant.bsRunStat[3] & 0x01)))
     if ((pDataTable->nPFlag&0x01)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[1].nFrac0, 3, 0)))&&(pDataTable->fCurr[0]>nTmp1))   //判断方向
    {
       Data2[19]  |= 0x04;
    }
    else
    {
      Data2[19]  &= 0xfb;
    }

    if ((0x40 == (g_MeterInsideWrong[0] & 0x40)) || (1 == tEvtTrip.nEvtTrip)) //拉闸
    {
        Data2[0] |= 0x04; 
        GpioClearOutputEX(0x42); 
    }
    else
    {
        Data2[0] &= 0xfb; 
        GpioSetOutputEX(0x42);
    }
    
    if ((g_MeterInsideWrong[1] & 0x01) == 1)
    {
        Data2[22] |= 0x04;
    }
    else
    {
        Data2[22] &= 0xfb;
    } 
}
  




INT8U  Data1Copy[35]={   0x0a,0x0f,  0x0a,0x0f,  0x10,0x00,  0x00,0x00,   0x10,0x00,  0x04,0x00,   0x10,0x00,  0x00,0x00,   0xa0,0xf0,
                       0x00,       0x00,0x00,  0x00,0x00,  0x00,0x00,   0x00,0x00,   0x00,0x00,   0x00,0x00,  0x00,0x00,  0x00,   0x00
                    };
extern  INT8U  DataAll[32] = { 0xf0,0xa0,  0x60,0x00,   0xd0,0x60,  0xf0,0x40,   0x60,0xc0,  0xb0,0xc0,  0xb0,0xe0,  0xe0,0x00,  0xf0,0xe0,  0xf0,0xc0  ,0xe0,0xe0  ,0x30,0xe0,  0x90,0xa0, 0x70,0x60, 0x90,0xe0,  0x80,0xe0};// 不带点数
extern  INT8U  Data_P[20] = {  0xf0, 0xb0,  0x60,0x10,   0xd0,0x70,  0xf0,0x50,   0x60,0xd0,  0xb0,0xd0,  0xb0,0xf0,  0xe0,0x10,  0xf0,0xf0,  0xf0,0xd0};// 带点数


/*
/******************************************************************************************************
* 函数名: AcCalcEventState
* 输　入:
* 输  出:
* 返  回:
* 功能描述:交采事件判断处理
******************************************************************************************************/
void AcCalcEventState(void)
{
    INT8U       i  = 0; 
    INT8U       j  ;
    INT32U      nTmp = 0;
    INT8U       Data[35] = {0};
    INT8U       nBuf[2] = {0};
   STD_TIME     sTime;
       pSTD_TIME   pTime = &sTime;
    MEM_AC_REAL_DATA            sRealData;             //使用全局变量
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    AC_EVENT_STAT               sEventStat;
    pAC_EVENT_STAT              pEventStat;
    EVT_TRIP_DATA		        tEvtTrip;		//跳闸状态
    EVT_ALARM_DATA              tAlarm; 
    EVT_GUARPOWER_DATA	        tGuarantPower;		//保电状态
    //INT8U       nAcEventTrip = 0;
    
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    MemReadAcXFlhBackData(F85, &tEvtTrip, sizeof(tEvtTrip));
    MemReadAcXFlhBackData(F86, &tAlarm, sizeof(tAlarm));
    MemReadAcXFlhBackData(F87, &tGuarantPower, sizeof(tGuarantPower));
    
     GetSysClk(&sTime);
     Data1[7] |= 0x02;
    //if(( pTime->nHour == 0x19) || (pTime->nHour == 0x20 ) || (pTime->nHour == 0x11 ))       //当前时间尖、峰、平、谷     
     if(1 == pDataTable->nRateNo) 
      { 
        Data1[4]  &= 0xf7;
        Data1[33]  &= 0xf7;
         Data1[34]  &= 0xfd;
        Data1[34]  |= 0x01;
      }
    //else if(( pTime->nHour == 0x09) || (pTime->nHour == 0x10)||( pTime->nHour == 0x21)||(pTime->nHour == 0x22) || (pTime->nHour == 0x18))       
     else if(2 == pDataTable->nRateNo)  
      {
        Data1[4]  &= 0xf7;
        Data1[33]  &= 0xf7;
         Data1[34]  &= 0xfe;
        Data1[34]  |= 0x02;
      }
   // else if((( pTime->nHour == 0x07) || (pTime->nHour == 0x08 ))|| (( pTime->nHour >= 0x12)&&(pTime->nHour <= 0x17 )))           
    else if(3 == pDataTable->nRateNo)  
      { 
        Data1[33]  &= 0xf7;
         Data1[34]  &= 0xfc;
        Data1[4]  |= 0x08;
      }
      else if(4 == pDataTable->nRateNo) 
      { Data1[4]  &= 0xf7;
        Data1[34]  &= 0xfc;
        Data1[33]  |= 0x08;
      }
     i = pDataTable->nPFlag & (0x01 << 3);
     j = pDataTable->nPFlag & (0x01 << 7);
 if (((pDataTable->fVolt[0] > (g_nVolt * LOSS_PH_OC_RATIO)) || (pDataTable->fVolt[1] > (g_nVolt * LOSS_PH_OC_RATIO)) ||
       (pDataTable->fVolt[2] > (g_nVolt * LOSS_PH_OC_RATIO)))&&(pDataTable->fCurr[0]/1000 < (1.5 * 0.005))&&(pDataTable->fCurr[1]/1000 < (1.5 * 0.005))
       &&(pDataTable->fCurr[2]/1000 < (1.5 * 0.005)))
     {
       Data1[32]  &= 0xfc;
       Data1[33]  &= 0xfa; 
       Data1[32]  |= 0x01; 
     
     }
    if(((i == 0x00) &&(j == 0x00))&&((pDataTable->fCurr[0]/1000 > (1.5 * 0.005))||(pDataTable->fCurr[1]/1000 > (1.5 * 0.005))
        ||(pDataTable->fCurr[2]/1000 > (1.5 * 0.005))))
     {
       Data1[32]  &= 0xfc;
       Data1[33]  &= 0xfa; 
       Data1[32]  |= 0x01; 
     }
   else if(((i == 0x08) &&(j == 0x00))&&((pDataTable->fCurr[0]/1000 > (1.5 * 0.005))||(pDataTable->fCurr[1]/1000 > (1.5 * 0.005))
        ||(pDataTable->fCurr[2]/1000 > (1.5 * 0.005))))
     {
       Data1[32]  &= 0xfc;
       Data1[33]  &= 0xfa;
       Data1[33]  |= 0x01;
     }
  else if(((i == 0x08) &&(j== 0x80))&&((pDataTable->fCurr[0]/1000 > (1.5 * 0.005))||(pDataTable->fCurr[1]/1000 > (1.5 * 0.005))
        ||(pDataTable->fCurr[2]/1000 > (1.5 * 0.005))))
     {
       Data1[32]  &= 0xfc;
       Data1[33]  &= 0xfa; 
       Data1[33]  |= 0x04;
     }
    else if(((i == 0x00) &&(j== 0x80))&&((pDataTable->fCurr[0]/1000 > (1.5 * 0.005))||(pDataTable->fCurr[1]/1000 > (1.5 * 0.005))
        ||(pDataTable->fCurr[2]/1000 > (1.5 * 0.005))))
     {
       Data1[32]  &= 0xfc;
       Data1[33]  &= 0xfa;
       Data1[32]  |= 0x02; 
     }
   // if (pDataTable->sRealInstant.bsRunStat[3] & 0x80)         //电压断相
  if ((pDataTable->fVolt[0] < (g_nVolt * LOSS_PH_OC_RATIO)) && (pDataTable->fCurr[0] < (RATED_CURR * LOSS_PH_CURR)))
   { 
     if(1!=(g_MeterEventWrong[0]&0x01)) //A失压模拟
     {
       Data1[15] &= 0xf7; 
     }
      g_nFlg_Alarm = 1;
   }
   else 
   {  
     //if(pDataTable->sRealInstant.bsRunStat[3] & 0x01)            //失压
      if ((pDataTable->fVolt[0] < (g_nVolt * LOSS_VOLT_OC_RATIO)) && (pDataTable->fCurr[0] > (RATED_CURR * OC_CURR)))
       {
          g_nFlgUa = 1;
          g_nFlg_Alarm = 1;
       }
       else
       {  
         if(1!=(g_MeterEventWrong[0]&0x01)) //A失压模拟
         {
            Data1[15] |= 0x08;
         }
       }    
   }
if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType))//三相四线与三相三区分
{
   //if(pDataTable->sRealInstant.bsRunStat[4] & 0x80)                      //电压断相    
   if ((pDataTable->fVolt[1] < (g_nVolt * LOSS_PH_OC_RATIO)) && (pDataTable->fCurr[1] < (RATED_CURR * LOSS_PH_CURR)))
   {
     if(1!=(g_MeterEventWrong[1]&0x01)) //B失压模拟
     {
       Data1[15] &= 0xfb;
     }
    g_nFlg_Alarm = 1;
   }
   else 
   {    
      //if(pDataTable->sRealInstant.bsRunStat[4] & 0x01)                    //失压
      if ((pDataTable->fVolt[1] < (g_nVolt * LOSS_VOLT_OC_RATIO)) && (pDataTable->fCurr[1] > (RATED_CURR * OC_CURR)))
       {
          g_nFlgUb = 1;
          g_nFlg_Alarm = 1;
       }
       else
       {  
          if(1!=(g_MeterEventWrong[1]&0x01)) //B失压模拟
          {
            Data1[15] |= 0x04;
          }
       }    
   }
}
    //if(pDataTable->sRealInstant.bsRunStat[5] & 0x80)                       //电压断相    
  if ((pDataTable->fVolt[2] < (g_nVolt * LOSS_PH_OC_RATIO)) && (pDataTable->fCurr[2] < (RATED_CURR * LOSS_PH_CURR)))
   { 
     if(1!=(g_MeterEventWrong[2]&0x01)) //C失压模拟
     {
       Data1[14] &= 0xfb;
     }
     g_nFlg_Alarm = 1;
   }
   else 
   {
     // if(pDataTable->sRealInstant.bsRunStat[5] & 0x01)                     //失压
     if ((pDataTable->fVolt[2] < (g_nVolt * LOSS_VOLT_OC_RATIO)) && (pDataTable->fCurr[2] > (RATED_CURR * OC_CURR)))
       {
          g_nFlgUc = 1;
          g_nFlg_Alarm = 1;
       }
       else
       {
          if(1!=(g_MeterEventWrong[2]&0x01)) //C失压模拟
          {
           Data1[14] |= 0x04;
          }
       }   
   }
 if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType))//三相四线与三相三区分
   {
 
 if(((pDataTable->sRealInstant.bsRunStat[6]&0x01)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealVolt[0].nFrac, 2, 0)))&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealVolt[1].nFrac, 2, 0)))
            &&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealVolt[2].nFrac, 2, 0))))||((pDataTable->sRealInstant.bsRunStat[6]&0x02)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealCurr[0].nD1, 3, 0)))&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealCurr[1].nD1, 3, 0)))
        &&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealCurr[2].nD1, 3, 0)))))//电压逆相序
    {
      g_nFlgVoltPhase = 1;
      g_nFlg_Alarm = 1;
    }
   else 
   {
      Data1[14] &= 0xf7;
   }
   }
 else
 {
     //if((pDataTable->sRealInstant.bsRunStat[6]&0x01)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealVolt[0].nFrac, 2, 0)))&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealVolt[2].nFrac, 2, 0))))//电压逆相序
        if ((((pDataTable->sRealInstant.bsRunStat[6]&0x01)) && pDataTable->fVolt[0] > (g_nVolt * LOSS_VOLT_OC_RATIO) && pDataTable->fVolt[2] > (g_nVolt * LOSS_VOLT_OC_RATIO))
            ||((pDataTable->sRealInstant.bsRunStat[6]&0x02) && pDataTable->fCurr[0] /1000 >(1.5 * 0.005) && pDataTable->fCurr[2] /1000 >(1.5 * 0.005)))
        {
          g_nFlgVoltPhase = 1;
          g_nFlg_Alarm = 1;
        }
       else 
       {
          Data1[14] &= 0xf7;
       } 
 }

   if((pDataTable->fCurr[0]/1000 < (1.5 * 0.005))&&(pDataTable->fCurr[1]/1000 < (1.5 * 0.005)&&(pDataTable->fCurr[2]/1000 < (1.5 * 0.005))))                                //电流断相
    {
      if(0x04!=(g_MeterEventWrong[2]&0x04)) //C失流模拟
      {
        Data1[11] &= 0xf3;
      }
      if(0x04 !=(g_MeterEventWrong[1]&0x04)) //B失流模拟
      {
         Data1[12] &= 0xf3;
      }
      if(0x04!=(g_MeterEventWrong[0]&0x04)) //A失流模拟
      {
         Data1[13] &= 0xf3;
      }
      g_nFlg_Alarm = 1;
    }
   else
   {  
        //if((pDataTable->fCurr[0] < (1.5 * 0.005))&&(pDataTable->fVolt[0] < (g_nVolt * LOSS_PH_OC_RATIO)))                                     //电流断相
        if(pDataTable->fCurr[0]/1000 < (1.5 * 0.005))
        {   
           //if(0x04!=(g_MeterEventWrong[0]&0x04)) //A失流模拟
           {
            // Data1[13] &= 0xf3;
           }
           if((pDataTable->fVolt[0] < (g_nVolt * LOSS_PH_OC_RATIO)))
           {
              Data1[13] &= 0xf3;
           }
           else
           {
              g_nFlgIa = 1;
              g_nFlg_Alarm = 1;
           }
        }
        else
        {
         // if(pDataTable->sRealInstant.bsRunStat[3] & 0x08)                        //失流     
         if ((((pDataTable->fVolt[0] > (g_nVolt * LOSS_CURR_RATIO)))
                && ((pDataTable->fCurr[0] > (RATED_CURR * RE_CURR)) || (pDataTable->fCurr[1] > (RATED_CURR * RE_CURR)) || (pDataTable->fCurr[2] > (RATED_CURR * RE_CURR))))
                    &&(pDataTable->fCurr[0] < RATED_CURR * OC_CURR))
          {  
              g_nFlgIa = 1;
              g_nFlg_Alarm = 1;
          }
         else if((pDataTable->sRealInstant.bsRunStat[3] != 8)&&(pDataTable->sRealInstant.bsRunStat[3] != 0x80))
          {  
            
             if(0x04!=(g_MeterEventWrong[0]&0x04)) //A失流模拟
             {
               Data1[13] |= 0x08; 
             }
          //  if((pDataTable->nPFlag & (0x01 << 3))&&(!(pDataTable->sRealInstant.bsRunStat[3] & 0x01)))   //电流逆相序
           if((pDataTable->nPFlag&0x01)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[1].nFrac0, 3, 0))))               
           {    
             g_nFlg_Alarm = 1;     
             Data1[13] |= 0x04;
             g_nFlg2 = 1;
           }
            else
            {
              Data1[13] &= 0xfb;
              
              g_nFlg2 = 0;
            }
          }
        }
        if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType))//三相四线与三相三区分
        {
            //if((pDataTable->fCurr[1] < (1.5 * 0.005))&&(pDataTable->fVolt[1] < (g_nVolt * LOSS_PH_OC_RATIO)))                                   //电流断相
            if (pDataTable->fCurr[1]/1000 < (1.5 * 0.005))
            { 
              //if(0x04!=(g_MeterEventWrong[1]&0x04)) //B失流模拟
              {
                //Data1[12] &= 0xf3;
              }
              if((pDataTable->fVolt[1] < (g_nVolt * LOSS_PH_OC_RATIO)))
               {
                  Data1[12] &= 0xf3;
               }
               else
               {
                  g_nFlgIb = 1;
                  g_nFlg_Alarm = 1;
               }
            }
            else
            {
              //if(pDataTable->sRealInstant.bsRunStat[4] & 0x08)          //失流                      
              if ((((pDataTable->fVolt[1] > (g_nVolt * LOSS_CURR_RATIO)))
                    && ((pDataTable->fCurr[0] > (RATED_CURR * RE_CURR)) || (pDataTable->fCurr[1] > (RATED_CURR * RE_CURR)) || (pDataTable->fCurr[2] > (RATED_CURR * RE_CURR))))
                        &&(pDataTable->fCurr[1] < RATED_CURR * OC_CURR))
              {  
                  g_nFlgIb = 1;
                  g_nFlg_Alarm = 1;
              }
             else if((pDataTable->sRealInstant.bsRunStat[4] != 8)&&(pDataTable->sRealInstant.bsRunStat[4] != 0x80))
              { 
                if(0x04!=(g_MeterEventWrong[1]&0x04)) //B失流模拟
                {
                  Data1[12] |= 0x08;
                }
               //if((pDataTable->nPFlag & (0x01 << 3))&&(!(pDataTable->sRealInstant.bsRunStat[4] & 0x01)))                 //电流逆相序 
                if((pDataTable->nPFlag&0x2)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[2].nFrac0, 3, 0))))              
                {
                  g_nFlg_Alarm = 1;
                   Data1[12] |= 0x04;
                   
                   g_nFlg2 = 1;
                }
                else
                {
                  Data1[12] &= 0xfb;
                  
                  g_nFlg2 = 0;
                }
              } 
            }
        }
        //if((pDataTable->fCurr[2] < (1.5 * 0.005))&&(pDataTable->fVolt[2] < (g_nVolt * LOSS_PH_OC_RATIO)))                                   //电流断相
        if (pDataTable->fCurr[2]/1000 < (1.5 * 0.005))
        { 
          //if(0x04!=(g_MeterEventWrong[2]&0x04)) //C失流模拟 
          {
           // Data1[11] &= 0xf3;
          }
          if((pDataTable->fVolt[2] < (g_nVolt * LOSS_PH_OC_RATIO)))
          {
              Data1[11] &= 0xf3;
          }
          else
          {
              g_nFlgIc = 1;
              g_nFlg_Alarm = 1;
          }
        }
       else
       {
         //if(pDataTable->sRealInstant.bsRunStat[5] & 0x08)   //失流                              
          if ((((pDataTable->fVolt[2] > (g_nVolt * LOSS_CURR_RATIO)))
                && ((pDataTable->fCurr[0] > (RATED_CURR * RE_CURR)) || (pDataTable->fCurr[1] > (RATED_CURR * RE_CURR)) || (pDataTable->fCurr[2] > (RATED_CURR * RE_CURR))))
                    &&(pDataTable->fCurr[2] < RATED_CURR * OC_CURR))
          {  
              g_nFlgIc = 1;
              g_nFlg_Alarm = 1;
          }
          else if((pDataTable->sRealInstant.bsRunStat[5] != 8)&&(pDataTable->sRealInstant.bsRunStat[5] != 0x80))
          { 
            if(0x04!=(g_MeterEventWrong[2]&0x04)) //C失流模拟
            {
              Data1[11] |= 0x08;
            }
           // if((pDataTable->nPFlag & (0x01 << 3))&&(!(pDataTable->sRealInstant.bsRunStat[5] & 0x01)))        //电流逆相序 
            if((pDataTable->nPFlag&0x04)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[3].nFrac0, 3, 0))))               //判断方向
            {
               g_nFlg_Alarm = 1;
               Data1[11] |= 0x04;
               g_nFlg2 = 1;
            }
            else
            {
              Data1[11] &= 0xfb;
              g_nFlg2 = 0;
            }
          }
          
        }
      if(ERR51+ERR52+ERR53+ERR54+ERR55+ERR56>=1)
         g_nFlg_Alarm=1;
       }
 
   
   if(1==g_MeterEventWrong[3]) ////逆相序
   {
       Data1[14] |= 0x08; 
   }
   
   if(((g_MeterInsideWrong[0]&0x80)==0x80)||(g_ClockWrong2 ==1)) //主控电池
   {
       Data1[9] |= 0x04;
   }
   else
   { 
      Data1[9] &= 0xFB;
   }
   if(((g_MeterInsideWrong[1]&0x01)==1)||(g_ClockWrong ==1)) //时钟电池
   {
       Data1[9] |= 0x08;
   }
   else
   {
       Data1[9] &= 0xF7;
   }
 
   if(0x02==(g_MeterEventWrong[0]&0x02)) //A断相不显示
   {
      Data1[15] &= 0xf7;
      Data1[13] &= 0xf3;
   }
   if(0x02==(g_MeterEventWrong[1]&0x02)) //B断相不显示
   {
      Data1[15] &= 0xfb;
      Data1[12] &= 0xf3;
   }
   if(0x02==(g_MeterEventWrong[2]&0x02)) //C断相不显示
   {   
      Data1[14] &= 0xfb;
      Data1[11] &= 0xf3;
   }
   GpioConfig(IO_PORT1, IO_PINxE, GPIO_MODE_INPUT);
     nTempBattery2 = GpioGetInput(IO_PORT1, IO_PINxE);
   if ((nTempBattery2)||(pDataTable->fVolt[1] > 0)||(pDataTable->fVolt[2] > 0)||(pDataTable->fVolt[3] > 0))
    {
      g_nFlgBattery = 0;
      GpioClearOutput(IO_PORT2, IO_PINx1);
       Data1[33] |= 0x02;
    } 
    else
    {
       g_nFlgBattery = 1;  
       GpioSetOutput(IO_PORT7, IO_PINx5);
       Data1[32]  &= 0xfc;
       Data1[33]  &= 0xf8; 
    }

   if ((0x40 == (g_MeterInsideWrong[0] & 0x40)) || (1 == tEvtTrip.nEvtTrip)) //拉闸
   {
       Data1[10] |= 0x01;
       GpioClearOutputEX(0x42);
   }
   else
   {
       Data1[10] &= 0xfe;
       GpioSetOutputEX(0x42);
   } 
   /*   模组化电表无此项功能（反馈电表会显示请购电字样）
   if (1 == tAlarm.nAlarm)//购电告警
   {
        Data1[9] |= 0x02;
        Data1[10] |= 0x02;
   }
   else
   {
        Data1[9] &= 0xfd;
        Data1[10] &= 0xfd;
   }*/
}

/*****************************************************************************
* 函 数 名:  LcdDisplayPage1
* 输　  入:
*                                 
* 输　  出:
* 返    回:
*          
* 功能描述:
*                                     
******************************************************************************/
//第 1 屏

void  LcdDisplayPage1(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;
    STD_TIME    sTime;
    pSTD_TIME   pTime = &sTime;
    INT8U       Data[35] ;
    INT8U       nBuf1[6];
    INT8U       nBuf[3];
    //INT32U      nDI         = 0x04000101;
    INT32U      nDI         = 0x40000200;  //698
    memcpy(Data,Data1,35);    
    memset(pTime, 0, sizeof(STD_TIME));
    GetSysClk(&sTime);

    nBuf[i++] = sTime.nDay;
    nBuf[i++] = sTime.nMon;
    nBuf[i++] = sTime.nYear;
    if(0x02==(g_MeterInsideWrong[0]&0x02)) //时间显示故障
    {
        /*nBuf[0]+=1;
        if(nBuf[0]>0x28)
        {
        nBuf[0]=1;
        nBuf[1]+=1;
        if(nBuf[1]>0x12)
        {
        nBuf[1]=1;
        nBuf[2]+=1;
        }
        }*/
        if(nBuf[0]<0x27)
        {
            nBuf[0] +=2;
        }
        else
        {
            nBuf[0] = 3;
        }
    }
    if(0x20==(g_MeterInsideWrong[0]&0x20)) //时钟故障
    {
        for(i=21;i<34;i++)
        {
            Data[i]|=0x80;
        }
    }

    for(i = 0; i < 3; i ++ )
    {
        nBuf1[i*2]  = nBuf[i]&0x0f;
        nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }

    Time1Deal( nBuf1, nDI, Data);
    Data[17] += 0x08;
    Data[16] += 0x08;
    Data[31] += 0x01;// 当前
    
    Data[2] &= 0xF0;
    Data[3] &= 0xF6;


    BULCDWrite1(0, Data, 35);     
}
//第 2 屏
void  LcdDisplayPage2(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;
    STD_TIME    sTime;
    pSTD_TIME   pTime = &sTime;
    INT8U       Data[35];
    INT8U       nBuf1[6];
    INT8U       nBuf[3];
    //INT32U      nDI         = 0x04000102;
    INT32U      nDI         = 0x40000200;  //698 
    memcpy(Data,Data1,35); 
    memset(pTime, 0, sizeof(STD_TIME));
    GetSysClk(&sTime);

    nBuf[i++] = sTime.nSec;
    nBuf[i++] = sTime.nMin;
    nBuf[i++] = sTime.nHour;  

    if(0x02==(g_MeterInsideWrong[0]&0x02)) //时间显示故障模拟
    {
        /*nBuf[1]+=0x10; //加十分钟
        if(nBuf[1]>=0x60)
        {
        nBuf[1]=0;
        nBuf[2]+=1;
        if(nBuf[2]>0x23)
        {
        nBuf[2]=0;
        }
        }*/
        if (sTime.nMin < 0x48)
        {
            nBuf[1] = sTime.nMin+0x10;
        }
        else
        {
            nBuf[1] = 5;
        }
    }
    if(0x20==(g_MeterInsideWrong[0]&0x20)) //时钟故障
    {
        for(i=21;i<34;i++)
        {
            Data[i]|=0x80;
        }
    }
    for(i = 0; i < 3; i ++ )
    {
        nBuf1[i*2]  = nBuf[i]&0x0f;
        nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }

    Time2Deal( nBuf1, nDI, Data);
    Data[17] += 0x08;
    Data[16] += 0x08;  
    Data[31] += 0x01;// 当前
    Data[2] += 0x04;
    Data[2] &= 0xF6;
    Data[3] &= 0xFD;
    
    BULCDWrite1(0, Data, 35);     
}
//第 3 屏  当前组合有功总电量  

void  LcdDisplayPage3(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;    
    INT8U       Data[35];  
    INT8U       nBuf[4] ;
    //INT32U      nDI         = 0x00000000;
    INT32U      nDI         = 0x00000201;
      
    MT_DGN_C_ENERGY_DATA sAcCombEnergy;  
    memcpy(Data,Data1,35); 
    memset(&sAcCombEnergy, 0, sizeof(MT_DGN_C_ENERGY_DATA));
   MemReadAcRealData(F5, &sAcCombEnergy, sizeof(sAcCombEnergy));
    memcpy(nBuf,  &sAcCombEnergy.sPPEnergy[0],  sizeof(FORMAT11));
    if (g_MeterAcErrFlg[0])
    {
        nBuf[0] = 0x66;
        nBuf[3] = 0x78;
    }
    else if (g_MeterAcErrFlg[3])
    {
        nBuf[0] = 0x66;
        nBuf[1] = 0x34;
        nBuf[2] = 0x05;
    }
     DataDeal(nBuf ,nDI, Data);
     Data[21] += 0x01;
     Data[24] += 0x0f;
     Data[12] += 0x02;
     Data[14] += 0x01;
     Data[15] += 0x01;
     Data[27] += 0x01;
     Data[31] += 0x01;                   //当前
     
      BULCDWrite1(0, Data, 35);
}

//第 4 屏  (当前)正向有功总电能   
void  LcdDisplayPage4(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;    
    INT8U       Data[35];  
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x00010000;
    INT32U      nDI         = 0x00100201;
      
    MT_DGN_ENERGY_DATA	sAcEn;
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    MemReadAcRealData(F1, &sAcEn, sizeof(sAcEn));
    memcpy(nBuf,   &sAcEn.sPPEnergy[0], sizeof(FORMAT11));
    if (g_MeterAcErrFlg[0])
    {
        nBuf[0] = 0x66;
        nBuf[3] = 0x78;
    }
    else if (g_MeterAcErrFlg[3])
    {
        nBuf[0] = 0x66;
        nBuf[1] = 0x34;
        nBuf[2] = 0x05;
    }
    DataDeal(nBuf ,nDI, Data);
     
    Data[21] += 0x01;
    Data[24] +=  0x0f;
    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01;
    Data[25] += 0x06;                  //正向
    Data[31] += 0x01;                   //当前
    BULCDWrite1(0, Data, 35);
}
//第 5 屏   (当前)正向有功费率 1 电能 
void  LcdDisplayPage5(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI  = 0x00010100;
    INT32U      nDI         = 0x00100202;
     MT_DGN_ENERGY_DATA	sAcEn;
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    MemReadAcRealData(F1, &sAcEn, sizeof(sAcEn));
    memcpy(nBuf,   &sAcEn.sPPEnergy[1], sizeof(FORMAT11));
    if (g_MeterAcErrFlg[0])
    {
        nBuf[0] = 0x66;
        nBuf[3] = 0x78;
    }
    else if (g_MeterAcErrFlg[3])
    {
        nBuf[0] = 0x33;
        nBuf[1] = 0x67;
        nBuf[2] = 0x04;
    }
    DataDeal(nBuf ,nDI, Data);
     Data[21] += 0x02;
     Data[24] += 0x0f;
     Data[12] += 0x02;
     Data[14] += 0x01;
     Data[15] += 0x01;
     Data[25] += 0x06;                  //正向
     Data[31] += 0x01;                   //当前
     BULCDWrite1(0, Data, 35);
}

//第 6 屏   (当前)正向有功费率 2 电能 
void  LcdDisplayPage6(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI  = 0x00010200;
    INT32U      nDI         = 0x00100203;
     MT_DGN_ENERGY_DATA	sAcEn;
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    MemReadAcRealData(F1, &sAcEn, sizeof(sAcEn));
    memcpy(nBuf,   &sAcEn.sPPEnergy[2], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data);
     
     Data[16] +=  0x01;      
     Data[24] += 0x0f;
     Data[12] += 0x02;
     Data[14] += 0x01;
     Data[15] += 0x01;
      Data[25] += 0x06;                  //正向
     Data[31] += 0x01;                     //当前
    BULCDWrite1(0, Data, 35);
}

//第 7屏   (当前)正向有功费率 3 电能 
void  LcdDisplayPage7(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI  = 0x00010300;
    INT32U      nDI         = 0x00100204;
    MT_DGN_ENERGY_DATA	sAcEn;
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    MemReadAcRealData(F1, &sAcEn, sizeof(sAcEn));
    memcpy(nBuf,   &sAcEn.sPPEnergy[3], sizeof(FORMAT11));

     DataDeal(nBuf ,nDI, Data);
     
     Data[16] += 0x02;     
     Data[24] += 0x0f;
     Data[12] += 0x02;
     Data[14] += 0x01;
     Data[15] += 0x01;
      Data[25] += 0x06;                  //正向
     Data[31] += 0x01;                     //当前
    BULCDWrite1(0, Data, 35);
}
//第 8 屏   (当前)正向有功费率 4 电能 
void  LcdDisplayPage8(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[4] ;
    //INT32U      nDI  = 0x00010400;
    INT32U      nDI         = 0x00100205;
     MT_DGN_ENERGY_DATA	sAcEn;
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    MemReadAcRealData(F1, &sAcEn, sizeof(sAcEn));
    memcpy(nBuf,   &sAcEn.sPPEnergy[4], sizeof(FORMAT11));
    
     DataDeal(nBuf ,nDI, Data);
     
     Data[17] += 0x02;
     Data[24] += 0x0f;
     Data[12] += 0x02;
     Data[14] += 0x01;
     Data[15] += 0x01;
     Data[25] += 0x06;                  //正向
     Data[31] += 0x01;             //当前
    BULCDWrite1(0, Data, 35);
}
//第 9 屏   当前正向有功总最大需量
void  LcdDisplayPage9(void)                  
{    
    INT8U       i  = 0;   
    INT8U       Data[35] ;
    INT8U       nBuf[3] ;
    INT8U       nBuf1[8]; 
    INT8U       nBuf2[16]; 
    INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x01010000;
    INT32U      nDI         = 0x10100201;

    MT_DGN_DEMAND_DATA	sAcDe;
    memcpy(Data,Data1,35); 
    memset(&sAcDe, 0, sizeof(MT_DGN_DEMAND_DATA));
    MemReadAcRealData(F7, &sAcDe, sizeof(sAcDe));
    memcpy(nBuf, &sAcDe.sPPDemand[0], sizeof(FORMAT23));

    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
        memset(nBuf, 0, sizeof(nBuf));
    }
    for(i = 0; i < 3; i ++ )
    {
        nBuf1[i*2]  = nBuf[i]&0x0f;
        nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }   
    Long2Buf((INT8U*)&nTemBuf, nDI, 4);    
    for(i = 0; i < 8; i ++)
    { 
        nBuf2[i*2]  = nTemBuf[i]&0x0f;
        nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
        Data[15-i*2] += DataAll[nBuf2[i] *2];
        Data[14-i*2] += DataAll[nBuf2[i] *2 + 1];
    }

    Data[17] = DataAll[nBuf1[0] *2];
    Data[16] = DataAll[nBuf1[0] *2 + 1] ;     
    Data[21] += DataAll[nBuf1[1] *2 ] + 0x01;
    Data[22] += DataAll[nBuf1[1] *2 + 1];

    Data[23] += DataAll[nBuf1[2]*2 ];
    Data[24] += DataAll[nBuf1[2]*2 + 1] + 0x07;
    Data[25] += Data_P[nBuf1[3] *2] + 0x0e;
    Data[26] += Data_P[nBuf1[3] *2 + 1]; 
    Data[27] += DataAll[nBuf1[4]*2  ];
    Data[28] += DataAll[nBuf1[4]*2 + 1];
    if(nBuf1[5] != 0)
    { 
        Data[29] += DataAll[nBuf1[5]*2];
        Data[30] += DataAll[nBuf1[5]*2 + 1];  
    }
    Data[14] += 0x01;
    Data[15] += 0x01;
    Data[31] += 0x01;                   //当前
    Data[2] &= 0xF0;
    Data[3] &= 0xF6;
    BULCDWrite1(0, Data, 35);
}
//第 10 屏   当前正向有功总最大需量发生日期
void  LcdDisplayPage10(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;
    INT8U       Data[35];
    INT8U       nBuf1[6];
    INT8U       nBuf[5];
    //INT32U      nDI         = 0x01010000;
    INT32U      nDI         = 0x10100201;

    MT_DGN_DEMAND_DATA	sAcDe;
    memcpy(Data,Data1,35); 
    memset(&sAcDe, 0, sizeof(MT_DGN_DEMAND_DATA));
    MemReadAcRealData(F7, &sAcDe, sizeof(sAcDe));
    memcpy(nBuf, &sAcDe.sPPDemandTime[0], sizeof(FORMAT15));

    for(i = 0; i < 3; i ++ )
    {
        nBuf1[i*2]  = nBuf[i +2]&0x0f;
        nBuf1[i*2+1]  = nBuf[i +2]>>0x04;
    }   
             
    Time1Deal( nBuf1, nDI, Data);
    Data[17] += 0x08;
    Data[16] += 0x08;
    Data[21] += 0x01;
    Data[24] += 0x07;
    Data[25] += 0x0e;
    Data[31] += 0x01;                   //当前

    Data[2] += 0x04;
    Data[2] &= 0xF6;
    Data[3] &= 0xFD;

    BULCDWrite1(0, Data, 35);     
}
//第 11 屏   当前正向有功总最大需量发生时间
void  LcdDisplayPage11(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;
    INT8U       Data[35];
    INT8U       nBuf1[4];
    INT8U       nBuf[5];
    //INT32U      nDI         = 0x01010000;
    INT32U      nDI         = 0x10100201;

    MT_DGN_DEMAND_DATA	sAcDe;
    memcpy(Data,Data1,35); 
    memset(&sAcDe, 0, sizeof(MT_DGN_DEMAND_DATA));
    MemReadAcRealData(F7, &sAcDe, sizeof(sAcDe));
    memcpy(nBuf, &sAcDe.sPPDemandTime[0], sizeof(FORMAT15));   
    for(i = 0; i < 2; i ++ )
    {
        nBuf1[i*2]  = nBuf[i]&0x0f;
        nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }    
    DemandTime( nBuf1, nDI, Data);
    Data[17] += 0x08;
    Data[16] += 0x08;  
    Data[21] += 0x01;
    Data[24] += 0x07;
    Data[25] += 0x0e;
    Data[31] += 0x01;// 当前

    Data[2] += 0x04;
    Data[2] &= 0xF4;
    Data[3] &= 0xFF;

    BULCDWrite1(0, Data, 35);     
}
//第 12 屏  (当前)反向有功总电能   
void  LcdDisplayPage12(void)                  //每秒刷新一次
{    
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x00020000;
    INT32U      nDI         = 0x00200201;
    
    MT_DGN_ENERGY_DATA	sAcEn;
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    MemReadAcRealData(FEND, &sAcEn, sizeof(sAcEn));
    memcpy(nBuf,   &sAcEn.sNPEnergy[0], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data);

    Data[21] += 0x01;
    Data[24] += 0x0f;
    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01;
    Data[25] += 0x02;                    //反向      
    Data[26] += 0x01;
    Data[31] += 0x01;                     //当前
       
      BULCDWrite1(0, Data, 35);
}
//第 13 屏   (当前)反向有功费率 1 电能 
void  LcdDisplayPage13(void)                  //每秒刷新一次
{    
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI  = 0x00020100;
    INT32U      nDI         = 0x00200202;
    MT_DGN_ENERGY_DATA	sAcEn;
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    MemReadAcRealData(F1, &sAcEn, sizeof(sAcEn));
    memcpy(nBuf,   &sAcEn.sNPEnergy[1], sizeof(FORMAT11));
    
    DataDeal(nBuf ,nDI, Data);

    Data[21] += 0x02;
    Data[24] += 0x0f;
    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01;
    Data[25] += 0x02;                    //反向      
    Data[26] += 0x01;
    Data[31] += 0x01;                     //当前

    BULCDWrite1(0, Data, 35);
}

//第 14 屏   (当前)反向有功费率 2 电能 
void  LcdDisplayPage14(void)                  //每秒刷新一次
{    
    INT8U       Data[35];
    INT8U       nBuf[4] ;
    //INT32U      nDI  = 0x00020200;
    INT32U      nDI         = 0x00200203;
    
    MT_DGN_ENERGY_DATA	sAcEn;
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    MemReadAcRealData(F1, &sAcEn, sizeof(sAcEn));
    memcpy(nBuf,   &sAcEn.sNPEnergy[2], sizeof(FORMAT11));
     
    DataDeal(nBuf ,nDI, Data);

    Data[16] += 0x01;     
    Data[24] += 0x0f;
    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01;
    Data[25] += 0x02;                    //反向      
    Data[26] += 0x01;
    Data[31] += 0x01;                     //当前

    BULCDWrite1(0, Data, 35);
}

//第 15 屏   (当前)反向有功费率 3 电能 
void  LcdDisplayPage15(void)                  //每秒刷新一次
{    
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI  = 0x00020300;
    INT32U      nDI         = 0x00200204;
   MT_DGN_ENERGY_DATA	sAcEn;
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    MemReadAcRealData(F1, &sAcEn, sizeof(sAcEn));
    memcpy(nBuf,   &sAcEn.sNPEnergy[3], sizeof(FORMAT11));
     
    DataDeal(nBuf ,nDI, Data);
     
     Data[16] += 0x02;     
    Data[24] += 0x0f;
      Data[12] += 0x02;
     Data[14] += 0x01;
     Data[15] += 0x01;
     Data[25] += 0x02;                    //反向      
     Data[26] += 0x01;
     Data[31] += 0x01;                     //当前
    
    BULCDWrite1(0, Data, 35);
}
//第 16 屏   (当前)反向有功费率 4 电能 
void  LcdDisplayPage16(void)                  //每秒刷新一次
{    
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI  = 0x00020400;
    INT32U      nDI         = 0x00200205;
    MT_DGN_ENERGY_DATA	sAcEn;
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    MemReadAcRealData(F1, &sAcEn, sizeof(sAcEn));
    memcpy(nBuf,   &sAcEn.sNPEnergy[4], sizeof(FORMAT11));
     
     DataDeal(nBuf ,nDI, Data);
     Data[17] += 0x02;
     Data[24] += 0x0f;
      Data[12] += 0x02;
     Data[14] += 0x01;
     Data[15] += 0x01;
     Data[25] += 0x02;                    //反向      
     Data[26] += 0x01;
     Data[31] += 0x01;                     //当前
    
    BULCDWrite1(0, Data, 35);
}
//第 17 屏   当前反向有功总最大需量
void  LcdDisplayPage17(void)                  
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[3];
    INT8U       nBuf1[6]; 
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x01020000;
    INT32U      nDI         = 0x10200201;
    MT_DGN_DEMAND_DATA	sAcDe;
    memcpy(Data,Data1,35); 
    memset(&sAcDe, 0, sizeof(MT_DGN_DEMAND_DATA));
    MemReadAcRealData(F7, &sAcDe, sizeof(sAcDe));
    memcpy(nBuf, &sAcDe.sNPDemand[0], sizeof(FORMAT23));

    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
        memset(nBuf, 0, sizeof(nBuf));
    }
    for(i = 0; i < 3; i ++ )
    {
        nBuf1[i*2]  = nBuf[i]&0x0f;
        nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }   
    Long2Buf((INT8U*)&nTemBuf, nDI, 4);    
    for(i = 0; i < 8; i ++)
    { 
        nBuf2[i*2]  = nTemBuf[i]&0x0f;
        nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
        Data[15-i*2] += DataAll[nBuf2[i] *2];
        Data[14-i*2] += DataAll[nBuf2[i] *2 + 1];
    }                                                  
    Data[17] = DataAll[nBuf1[0] *2];
    Data[16] = DataAll[nBuf1[0] *2 + 1] ;     
    Data[21] += DataAll[nBuf1[1] *2 ] + 0x01;
    Data[22] += DataAll[nBuf1[1] *2 + 1];

    Data[23] += DataAll[nBuf1[2]*2 ];
    Data[24] += DataAll[nBuf1[2]*2 + 1] + 0x07;
    Data[25] += Data_P[nBuf1[3] *2] + 0x0a;
    Data[26] += Data_P[nBuf1[3] *2 + 1] +0x01; 
    Data[27] += DataAll[nBuf1[4]*2  ];
    Data[28] += DataAll[nBuf1[4]*2 + 1];
    if(nBuf1[5] != 0)
    { 
        Data[29] += DataAll[nBuf1[5]*2];
        Data[30] += DataAll[nBuf1[5]*2 + 1];  
    }
    Data[14] += 0x01;
    Data[15] += 0x01;
    Data[31] += 0x01;                   //当前
    Data[2] &= 0xF0;
    Data[3] &= 0xF6;
    BULCDWrite1(0, Data, 35);
}

//第 18 屏   当前反向有功总最大需量发生日期
void  LcdDisplayPage18(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;
    INT8U       Data[35];
    INT8U       nBuf1[6];
    INT8U       nBuf[5];
    //INT32U      nDI         = 0x01020000;
    INT32U      nDI         = 0x10200201;
    MT_DGN_DEMAND_DATA	sAcDe;
    memcpy(Data,Data1,35); 
    memset(&sAcDe, 0, sizeof(MT_DGN_DEMAND_DATA));
    MemReadAcRealData(F7, &sAcDe, sizeof(sAcDe));
    memcpy(nBuf, &sAcDe.sNPDemandTime[0], sizeof(FORMAT15));

    for(i = 0; i < 3; i ++ )
    {
        nBuf1[i*2]  = nBuf[i +2]&0x0f;
        nBuf1[i*2+1]  = nBuf[i +2]>>0x04;
    }   
                 
    Time1Deal( nBuf1, nDI, Data);
    Data[17] +=  0x08;
    Data[16] += 0x08;
    Data[21] += 0x01;
    Data[24] +=  0x07;
    Data[25] += 0x0a;
    Data[26] += 0x01;
    Data[31] += 0x01;                   //当前
    Data[2] += 0x04;
    Data[2] &= 0xF6;
    Data[3] &= 0xFD;
    BULCDWrite1(0, Data, 35);     
}
//第 19 屏   当前反向有功总最大需量发生时间
void  LcdDisplayPage19(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;
    INT8U       Data[35];
    INT8U       nBuf1[4];
    INT8U       nBuf[5];
    //INT32U      nDI         = 0x01020000;
    INT32U      nDI         = 0x10200201;

    memcpy(Data,Data1,35); 
    MT_DGN_DEMAND_DATA	sAcDe;
    memset(&sAcDe, 0, sizeof(MT_DGN_DEMAND_DATA));
    MemReadAcRealData(F7, &sAcDe, sizeof(sAcDe));
    memcpy(nBuf, &sAcDe.sNPDemandTime[0], sizeof(FORMAT15));  
    for(i = 0; i < 2; i ++ )
    {
        nBuf1[i*2]  = nBuf[i]&0x0f;
        nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }    
    DemandTime( nBuf1, nDI, Data);
    Data[17] += 0x08;
    Data[16] += 0x08;  
    Data[21] += 0x01;
    Data[24] += 0x07;
    Data[25] += 0x0a;
    Data[26] += 0x01;
    Data[31] += 0x01;// 当前
    Data[2] += 0x04;
    Data[2] &= 0xF4;
    Data[3] &= 0xFF;
    BULCDWrite1(0, Data, 35);     
}
//第 20 屏   当前组合无功 1总 电量 
void  LcdDisplayPage20(void)                  //每秒刷新一次
{    
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI  = 0x00030000;
    INT32U      nDI         = 0x00300201;
    
     MT_DGN_ENERGY_DATA	sAcEn;
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    MemReadAcRealData(F1, &sAcEn, sizeof(sAcEn));
    memcpy(nBuf,   &sAcEn.sPNEnergy[0], sizeof(FORMAT11));
     
    DataDeal(nBuf ,nDI, Data);
     Data[21] += 0x01;
     Data[23] += 0x02;
     Data[24] += 0x0d;
     Data[14] += 0x02;
     Data[15] += 0x02;     
     Data[25] += 0x01;
     Data[27] += 0x01;
     Data[31] += 0x01;// 当前
    
    BULCDWrite1(0, Data, 35);
}

//第 21 屏   当前组合无功 2 总 电量
void  LcdDisplayPage21(void)                  //每秒刷新一次
{    
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI  = 0x00040000;
    INT32U      nDI         = 0x00400201;
    
    MT_DGN_ENERGY_DATA	sAcEn;
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    MemReadAcRealData(F1, &sAcEn, sizeof(sAcEn));
    memcpy(nBuf,   &sAcEn.sNNEnergy[0], sizeof(FORMAT11));
     
     DataDeal(nBuf ,nDI, Data);
      Data[21] += 0x01;
     Data[23] += 0x03;
     Data[24] += 0x0d;
     Data[14] += 0x02;
     Data[15] += 0x02; 
     Data[25] += 0x01;
     Data[27] += 0x01;
     Data[31] += 0x01;// 当前
    
    BULCDWrite1(0, Data, 35);

}
//第 22 屏   当前第 1 象限无功总电量 
void  LcdDisplayPage22(void)                  //每秒刷新一次
{    
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI  = 0x00050000;
    INT32U      nDI         = 0x10500201;
    
    MT_DGN_Q_ENERGY_DATA  sAcQEn;
    memcpy(Data,Data1,35); 
    memset(&sAcQEn, 0, sizeof(MT_DGN_Q_ENERGY_DATA));
    MemReadAcRealData(F2, &sAcQEn, sizeof(sAcQEn));
    memcpy(nBuf,  &sAcQEn.sQ1Energy[0], sizeof(FORMAT11));
     
    DataDeal(nBuf ,nDI, Data);
      Data[21] += 0x01;
     Data[22] += 0x02;
     Data[24] += 0x0d;
     Data[14] += 0x02;
     Data[15] += 0x02; 
     Data[25] += 0x01;                        
     Data[31] += 0x01;                     //当前

    BULCDWrite1(0, Data, 35);
}

//第 23屏   当前第 2 象限无功总电量 
void  LcdDisplayPage23(void)                  //每秒刷新一次
{    
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI  = 0x00060000;
    INT32U      nDI         = 0x10600201;
    MT_DGN_Q_ENERGY_DATA  sAcQEn;
    memcpy(Data,Data1,35); 
    memset(&sAcQEn, 0, sizeof(MT_DGN_Q_ENERGY_DATA));
    MemReadAcRealData(F2, &sAcQEn, sizeof(sAcQEn));
    memcpy(nBuf,  &sAcQEn.sQ2Energy[0], sizeof(FORMAT11));
     
    DataDeal(nBuf ,nDI, Data);
     Data[21] += 0x01;
     Data[22] += 0x02;     
     Data[23] += 0x02;
     Data[24] += 0x0d;    
     Data[14] += 0x02;
     Data[15] += 0x02; 
     Data[25] += 0x01;                        
     Data[31] += 0x01;                     //当前
    
    BULCDWrite1(0, Data, 35);
}

//第 24 屏   当前第 3象 限无功总电量 
void  LcdDisplayPage24(void)                  //每秒刷新一次
{    
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI  = 0x00070000;
    INT32U      nDI         = 0x10700201;
    
     MT_DGN_Q_ENERGY_DATA  sAcQEn;
    memcpy(Data,Data1,35); 
    memset(&sAcQEn, 0, sizeof(MT_DGN_Q_ENERGY_DATA));
    MemReadAcRealData(F2, &sAcQEn, sizeof(sAcQEn));
    memcpy(nBuf,  &sAcQEn.sQ3Energy[0], sizeof(FORMAT11));
     
    DataDeal(nBuf ,nDI, Data);
       Data[21] += 0x01;
     Data[22] += 0x02;     
     Data[23] += 0x03;
     Data[24] += 0x0d; 
     Data[14] += 0x02;
     Data[15] += 0x02; 
     Data[25] += 0x01;                        
     Data[31] += 0x01;                     //当前
    
    BULCDWrite1(0, Data, 35);
}

//第 25 屏   当前第 4 象限无功总电量 
void  LcdDisplayPage25(void)                  //每秒刷新一次
{    
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI  = 0x00080000;
    INT32U      nDI         = 0x10800201;
     MT_DGN_Q_ENERGY_DATA  sAcQEn;
    memcpy(Data,Data1,35); 
    memset(&sAcQEn, 0, sizeof(MT_DGN_Q_ENERGY_DATA));
    MemReadAcRealData(F2, &sAcQEn, sizeof(sAcQEn));
    memcpy(nBuf,  &sAcQEn.sQ4Energy[0], sizeof(FORMAT11));
     
    DataDeal(nBuf ,nDI, Data);
      Data[21] += 0x01;
     Data[22] += 0x03;  
     Data[24] += 0x0d;
     Data[14] += 0x02;
     Data[15] += 0x02; 
     Data[25] += 0x01;                        
     Data[31] += 0x01;                     //当前
    
    BULCDWrite1(0, Data, 35);
}

//第 26 屏  上 1月 正 向有功总 电量
void  LcdDisplayPage26(void)                  //每秒刷新一次
{    
    INT8U       Data[35];  
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x00010001;
    INT32U      nDI         = 0x50050201;
    MT_DGN_ENERGY_DATA	sAcEn;
    MEM_AC_DJ_DATA     sAcHistData;
      
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(sAcHistData));

    GetLastMonData(&sAcHistData,0);
    memcpy(&sAcEn, &sAcHistData.sAcEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcEn.sPPEnergy[0], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data); 

    Data[21] += 0x01;
    Data[24] += 0x0f; 
    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01; 
    Data[25] += 0x06;                  //正向
    Data[28] += 0x01;                  //上 1月
    Data[29] += 0x06;                  
    Data[31] += 0x02; 
     
    BULCDWrite1(0, Data, 35);
}



//第 27 屏   上 1月正向有功费率 1 电能 
void  LcdDisplayPage27(void)                  //每秒刷新一次
{    
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI  = 0x00010101;
    INT32U      nDI         = 0x50050201;
    MEM_AC_DJ_DATA     sAcHistData;
    MT_DGN_ENERGY_DATA	sAcEn;
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));

    GetLastMonData(&sAcHistData,0);
    memcpy(&sAcEn, &sAcHistData.sAcEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcEn.sPPEnergy[1], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data); 
    Data[21] += 0x02;
    Data[24] += 0x0f;
    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01; 
    Data[25] += 0x06;                  //正向
    Data[28] += 0x01;                  //上 1月
    Data[29] += 0x06;                  
    Data[31] += 0x02; 
    BULCDWrite1(0, Data, 35);
}

//第 28 屏   上 1月正向有功费率 2 电能 
void  LcdDisplayPage28(void)                  //每秒刷新一次
{    
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI  = 0x00010201;
    INT32U      nDI         = 0x50050201;
    MEM_AC_DJ_DATA     sAcHistData;
    MT_DGN_ENERGY_DATA	sAcEn;

    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));

    GetLastMonData(&sAcHistData,0);
    memcpy(&sAcEn, &sAcHistData.sAcEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcEn.sPPEnergy[2], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data); 
    Data[16] += 0x01;     
    Data[24] += 0x0f;
    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01; 
    Data[25] += 0x06;                  //正向
    Data[28] += 0x01;                  //上 1月
    Data[29] += 0x06;                  
    Data[31] += 0x02; 
    BULCDWrite1(0, Data, 35);
}

//第 29 屏   上 1月正向有功费率 3 电能 
void  LcdDisplayPage29(void)                  //每秒刷新一次
{    
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI  = 0x00010301;
    INT32U      nDI         = 0x50050201;
    MEM_AC_DJ_DATA     sAcHistData;
    MT_DGN_ENERGY_DATA	sAcEn;
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));

    GetLastMonData(&sAcHistData,0);
    memcpy(&sAcEn, &sAcHistData.sAcEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcEn.sPPEnergy[3], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data); 
    Data[16] += 0x02;     
    Data[24] += 0x0f;
    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01; 
    Data[25] += 0x06;                  //正向
    Data[28] += 0x01;                  //上 1月
    Data[29] += 0x06;                  
    Data[31] += 0x02; 
    BULCDWrite1(0, Data, 35);
}
//第 30 屏   上 1月正向有功费率 4 电能 
void  LcdDisplayPage30(void)                  //每秒刷新一次
{    
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI  = 0x00010401;
    INT32U      nDI         = 0x50050201;

    MEM_AC_DJ_DATA     sAcHistData;
    MT_DGN_ENERGY_DATA	sAcEn;
    
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));

    GetLastMonData(&sAcHistData,0);
    memcpy(&sAcEn, &sAcHistData.sAcEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcEn.sPPEnergy[4], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data);              
    Data[17] += 0x02;
    Data[24] += 0x0f;
    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01; 
    Data[25] += 0x06;                  //正向
    Data[28] += 0x01;                  //上 1月
    Data[29] += 0x06;                  
    Data[31] += 0x02; 
    BULCDWrite1(0, Data, 35);
}
//第 31 屏   上 1月正向有功总最大需量
void  LcdDisplayPage31(void)                  
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[3];
    INT8U       nBuf1[6]; 
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x01010001;
    INT32U      nDI         = 0x50050201;
    MEM_AC_DJ_DATA     sAcHistData;
    MT_DGN_DEMAND_DATA	sAcDe;

    memcpy(Data,Data1,35); 
    memset(&sAcDe, 0, sizeof(MT_DGN_DEMAND_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));

    GetLastMonData(&sAcHistData,0);
    memcpy(&sAcDe, &sAcHistData.sAcDemand, sizeof(sAcDe)); 
    memcpy(nBuf,  &sAcDe.sPPDemand[0], sizeof(FORMAT23));

    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
        memset(nBuf, 0, sizeof(nBuf));
    }
    for(i = 0; i < 3; i ++ )
    {
        nBuf1[i*2]  = nBuf[i]&0x0f;
        nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }       
    Long2Buf((INT8U*)&nTemBuf, nDI, 4);    
    for(i = 0; i < 8; i ++)
    { 
        nBuf2[i*2]  = nTemBuf[i]&0x0f;
        nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
        Data[15-i*2] += DataAll[nBuf2[i] *2];
        Data[14-i*2] += DataAll[nBuf2[i] *2 + 1];
    }             
    Data[17] = DataAll[nBuf1[0] *2];
    Data[16] = DataAll[nBuf1[0] *2 + 1] ;     
    Data[21] += DataAll[nBuf1[1] *2 ] + 0x01;
    Data[22] += DataAll[nBuf1[1] *2 + 1];

    Data[23] += DataAll[nBuf1[2]*2 ];
    Data[24] += DataAll[nBuf1[2]*2 + 1] + 0x07;
    Data[25] += Data_P[nBuf1[3] *2] + 0x0e;
    Data[26] += Data_P[nBuf1[3] *2 + 1]; 
    Data[27] += DataAll[nBuf1[4]*2  ];
    Data[28] += DataAll[nBuf1[4]*2 + 1] + 0x01;
    if(nBuf1[5] != 0)
    { 
        Data[29] += DataAll[nBuf1[5]*2];
        Data[30] += DataAll[nBuf1[5]*2 + 1];  
    }           
    Data[14] += 0x01;
    Data[15] += 0x01; 
    Data[29] += 0x06;                  
    Data[31] += 0x02;           //上 1月
    Data[2] &= 0xF0;
    Data[3] &= 0xF6;
    BULCDWrite1(0, Data, 35);
}
//第 32 屏   上 1月正向有功总最大需量发生日期
void  LcdDisplayPage32(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;
    INT8U       Data[35];
    INT8U       nBuf1[6];
    INT8U       nBuf[5];
    //INT32U      nDI         = 0x01010001;
    INT32U      nDI         = 0x50050201;
    
    MEM_AC_DJ_DATA     sAcHistData;
    MT_DGN_DEMAND_DATA	sAcDe;
    
    memcpy(Data,Data1,35); 
    memset(&sAcDe, 0, sizeof(MT_DGN_DEMAND_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));

    GetLastMonData(&sAcHistData,0);
    memcpy(&sAcDe, &sAcHistData.sAcDemand, sizeof(sAcDe)); 
    memcpy(nBuf,  &sAcDe.sPPDemandTime[0], sizeof(FORMAT15));

    for(i = 0; i < 3; i ++ )
    {
        nBuf1[i*2]  = nBuf[i +2]&0x0f;
        nBuf1[i*2+1]  = nBuf[i +2]>>0x04;
    }   
             
    Time1Deal( nBuf1, nDI, Data);
    Data[17] += 0x08;
    Data[16] += 0x08;
    Data[21] += 0x01;
    Data[24] += 0x07;
    Data[25] += 0x0e;
    Data[28] += 0x01;
    Data[29] += 0x06;
    Data[31] += 0x02; //上 1月

    Data[2] += 0x04;
    Data[2] &= 0xF6;
    Data[3] &= 0xFD;
    BULCDWrite1(0, Data, 35);     
}
//第 33 屏   上 1月正向有功总最大需量发生时间
void  LcdDisplayPage33(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;
    INT8U       Data[35];
    INT8U       nBuf1[4];
    INT8U       nBuf[5];
    //INT32U      nDI         = 0x01010001;
    INT32U      nDI         = 0x50050201;
    MEM_AC_DJ_DATA     sAcHistData;
    MT_DGN_DEMAND_DATA	sAcDe;

    memcpy(Data,Data1,35); 
    memset(&sAcDe, 0, sizeof(MT_DGN_DEMAND_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));

    GetLastMonData(&sAcHistData,0);
    memcpy(&sAcDe, &sAcHistData.sAcDemand, sizeof(sAcDe)); 
    memcpy(nBuf,  &sAcDe.sPPDemandTime[0], sizeof(FORMAT15));    
    for(i = 0; i < 2; i ++ )
    {
        nBuf1[i*2]  = nBuf[i]&0x0f;
        nBuf1[i*2+1] = nBuf[i]>>0x04;
    }    

    DemandTime( nBuf1, nDI, Data);
    Data[17] += 0x08;
    Data[16] += 0x08;  
    Data[21] += 0x01;
    Data[24] += 0x07;
    Data[25] += 0x0e;
    Data[28] += 0x01;                  //上 1月
    Data[29] += 0x06;                  
    Data[31] += 0x02; 

    Data[2] += 0x04;
    Data[2] &= 0xF4;
    Data[3] &= 0xFF;
    BULCDWrite1(0, Data, 35);     
}
//第 34屏  上 1月反向有功总电能   
void  LcdDisplayPage34(void)                  //每秒刷新一次
{    
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x00020001;
    INT32U      nDI         = 0x50050201;
    MEM_AC_DJ_DATA     sAcHistData;
    MT_DGN_ENERGY_DATA	sAcEn;
    
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));

    GetLastMonData(&sAcHistData,0);
    memcpy(&sAcEn, &sAcHistData.sAcEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcEn.sNPEnergy[0], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data);
    Data[21] += 0x01;
    Data[24] += 0x0f;
    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01; 
    Data[25] += 0x02;                    //反向      
    Data[26] += 0x01;
    Data[28] += 0x01;                  //上 1月
    Data[29] += 0x06;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}
//第 35 屏   上 1月反向有功费率 1 电能 
void  LcdDisplayPage35(void)                  //每秒刷新一次
{    
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI  = 0x00020101;
    INT32U      nDI         = 0x50050201;
    MEM_AC_DJ_DATA     sAcHistData;
    MT_DGN_ENERGY_DATA	sAcEn;
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));

    GetLastMonData(&sAcHistData,0);
    memcpy(&sAcEn, &sAcHistData.sAcEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcEn.sNPEnergy[1], sizeof(FORMAT11));
    
    DataDeal(nBuf ,nDI, Data);
    Data[21] += 0x02;
    Data[24] += 0x0f;
    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01; 
    Data[25] += 0x02;                    //反向      
    Data[26] += 0x01;
    Data[28] += 0x01;                  //上 1月
    Data[29] += 0x06;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}

//第 36 屏   上 1月反向有功费率 2 电能 
void  LcdDisplayPage36(void)                  //每秒刷新一次
{    
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI  = 0x00020201;
    INT32U      nDI         = 0x50050201;
    MEM_AC_DJ_DATA     sAcHistData;
    MT_DGN_ENERGY_DATA	sAcEn;
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));

    GetLastMonData(&sAcHistData,0);
    memcpy(&sAcEn, &sAcHistData.sAcEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcEn.sNPEnergy[2], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data);

    Data[16] += 0x01;     
    Data[24] += 0x0f;
    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01; 
    Data[25] += 0x02;                    //反向      
    Data[26] += 0x01;
    Data[28] += 0x01;                  //上 1月
    Data[29] += 0x06;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}

//第 37 屏   上 1月反向有功费率 3 电能 
void  LcdDisplayPage37(void)                  //每秒刷新一次
{    
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI  = 0x00020301;
    INT32U      nDI         = 0x50050201;
    MEM_AC_DJ_DATA     sAcHistData;
    MT_DGN_ENERGY_DATA	sAcEn;
    
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));

    GetLastMonData(&sAcHistData,0);
    memcpy(&sAcEn, &sAcHistData.sAcEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcEn.sNPEnergy[3], sizeof(FORMAT11));
    
     DataDeal(nBuf ,nDI, Data);
     
     Data[16] += 0x02;     
     Data[24] += 0x0f;
     Data[12] += 0x02;
     Data[14] += 0x01;
     Data[15] += 0x01; 
     Data[25] += 0x02;                    //反向      
     Data[26] += 0x01;
     Data[28] += 0x01;                  //上 1月
     Data[29] += 0x06;                  
     Data[31] += 0x02; 
    
    BULCDWrite1(0, Data, 35);
}
//第 38 屏   上 1月反向有功费率 4 电能 
void  LcdDisplayPage38(void)                  //每秒刷新一次
{    
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI  = 0x00020401;
    INT32U      nDI         = 0x50050201;
    MEM_AC_DJ_DATA     sAcHistData;
    MT_DGN_ENERGY_DATA	sAcEn;
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));

    GetLastMonData(&sAcHistData,0);
    memcpy(&sAcEn, &sAcHistData.sAcEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcEn.sNPEnergy[4], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data);

    Data[17] += 0x02;
    Data[24] += 0x0f;    
    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01; 
    Data[25] += 0x02;                    //反向      
    Data[26] += 0x01;
    Data[28] += 0x01;                  //上 1月
    Data[29] += 0x06;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}
//第 39屏   上 1月反向有功总最大需量
void  LcdDisplayPage39(void)                  
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[3];
    INT8U       nBuf1[6]; 
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x01020001;
    INT32U      nDI         = 0x50050201;
    MT_DGN_DEMAND_DATA	sAcDe; 
    MEM_AC_DJ_DATA     sAcHistData;
    memcpy(Data,Data1,35); 
    memset(&sAcDe, 0, sizeof(MT_DGN_DEMAND_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));

    GetLastMonData(&sAcHistData,0);
    memcpy(&sAcDe, &sAcHistData.sAcDemand, sizeof(sAcDe)); 
    memcpy(nBuf,  &sAcDe.sNPDemand[0], sizeof(FORMAT23)); 
   
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
        memset(nBuf, 0, sizeof(nBuf));
    }
    for(i = 0; i < 3; i ++ )
    {
        nBuf1[i*2]  = nBuf[i]&0x0f;
        nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }   
    Long2Buf((INT8U*)&nTemBuf, nDI, 4);    
    for(i = 0; i < 8; i ++)
    { 
        nBuf2[i*2]  = nTemBuf[i]&0x0f;
        nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
        Data[15-i*2] += DataAll[nBuf2[i] *2];
        Data[14-i*2] += DataAll[nBuf2[i] *2 + 1];
    }            
    Data[17] = DataAll[nBuf1[0] *2];
    Data[16] = DataAll[nBuf1[0] *2 + 1] ;     
    Data[21] += DataAll[nBuf1[1] *2 ] + 0x01;
    Data[22] += DataAll[nBuf1[1] *2 + 1];

    Data[23] += DataAll[nBuf1[2]*2 ];
    Data[24] += DataAll[nBuf1[2]*2 + 1] + 0x07;
    Data[25] += Data_P[nBuf1[3] *2] + 0x0a;
    Data[26] += Data_P[nBuf1[3] *2 + 1]+ 0x01; 
    Data[27] += DataAll[nBuf1[4]*2  ];
    Data[28] += DataAll[nBuf1[4]*2 + 1];
    if(nBuf1[5] != 0)
    { 
        Data[29] += DataAll[nBuf1[5]*2];
        Data[30] += DataAll[nBuf1[5]*2 + 1];  
    }
    Data[14] += 0x01;
    Data[15] += 0x01; 
    Data[28] += 0x01;                  //上 1月
    Data[29] += 0x06;                  
    Data[31] += 0x02; 
    
    Data[2] &= 0xF0;
    Data[3] &= 0xF6;
    BULCDWrite1(0, Data, 35);
}
//第 40 屏   上 1月反向有功总最大需量发生日期
void  LcdDisplayPage40(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;
    INT8U       Data[35];
    INT8U       nBuf1[6];
    INT8U       nBuf[5];
    //INT32U      nDI         = 0x01020001;
    INT32U      nDI         = 0x50050201;
    MEM_AC_DJ_DATA     sAcHistData;
    MT_DGN_DEMAND_DATA	sAcDe;
    memcpy(Data,Data1,35); 
    memset(&sAcDe, 0, sizeof(MT_DGN_DEMAND_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));

    GetLastMonData(&sAcHistData,0);
    memcpy(&sAcDe, &sAcHistData.sAcDemand, sizeof(sAcDe)); 
    memcpy(nBuf,  &sAcDe.sNPDemandTime[0], sizeof(FORMAT15)); 

    for(i = 0; i < 3; i ++ )
    {
        nBuf1[i*2]  = nBuf[i +2]&0x0f;
        nBuf1[i*2+1]  = nBuf[i +2]>>0x04;
    }   
             
    Time1Deal( nBuf1, nDI, Data); 
    Data[17] += 0x08;
    Data[16] += 0x08;
    Data[21] += 0x01; 
    Data[24] += 0x07;
    Data[25] += 0x0a;
    Data[26] += 0x01;
    Data[28] += 0x01;
    Data[29] += 0x06;
    Data[31] += 0x02;  //上 1月 

    Data[2] += 0x04;
    Data[2] &= 0xF6;
    Data[3] &= 0xFD;
    BULCDWrite1(0, Data, 35);     
}
//第 41屏   上 1月反向有功总最大需量发生时间
void  LcdDisplayPage41(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;
    INT8U       Data[35] ;
    INT8U       nBuf1[4];
    INT8U       nBuf[5];
    //INT32U      nDI         = 0x01020001;
    INT32U      nDI         = 0x50050201;
    MEM_AC_DJ_DATA     sAcHistData;
    MT_DGN_DEMAND_DATA	sAcDe;
    memcpy(Data,Data1,35); 
    memset(&sAcDe, 0, sizeof(MT_DGN_DEMAND_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));

    GetLastMonData(&sAcHistData,0);
    memcpy(&sAcDe, &sAcHistData.sAcDemand, sizeof(sAcDe)); 
    memcpy(nBuf,  &sAcDe.sNPDemandTime[0], sizeof(FORMAT15));    
    for(i = 0; i < 2; i ++ )
    {
        nBuf1[i*2]  = nBuf[i]&0x0f;
        nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }    

    DemandTime( nBuf1, nDI, Data);
    Data[17] += 0x08;
    Data[16] += 0x08;  
    Data[21] += 0x01;
    Data[24] += 0x07;
    Data[25] += 0x0a;
    Data[26] += 0x01;
    Data[28] += 0x01;                  //上 1月
    Data[29] += 0x06;                  
    Data[31] += 0x02; 

    Data[2] += 0x04;
    Data[2] &= 0xF4;
    Data[3] &= 0xFF;
    BULCDWrite1(0, Data, 35);     
}
//第 42 屏   上 1月 第 1象 限无功总电量 
void  LcdDisplayPage42(void)                  //每秒刷新一次
{    
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI  = 0x00050001;
    INT32U      nDI         = 0x50050201;
    MEM_AC_DJ_DATA     sAcHistData; 
    MT_DGN_Q_ENERGY_DATA  sAcQEn;
    MT_DGN_ENERGY_DATA	sAcEn;
    memcpy(Data,Data1,35); 
    memset(&sAcQEn, 0, sizeof(MT_DGN_Q_ENERGY_DATA));
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));

    GetLastMonData(&sAcHistData,0);
    memcpy(&sAcQEn, &sAcHistData.sAcQEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcQEn.sQ1Energy[0], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data);

    Data[21] += 0x01;
    Data[22] += 0x02;
    Data[24] += 0x0d;
    Data[14] += 0x02;
    Data[15] += 0x02; 
    Data[25] += 0x01;                        
    Data[28] += 0x01;                  //上 1月
    Data[29] += 0x06;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}

//第 43屏   上 1月第 2 象限无功总电量 
void  LcdDisplayPage43(void)                  //每秒刷新一次
{    
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI  = 0x00060001;
    INT32U      nDI         = 0x50050201;
    MEM_AC_DJ_DATA     sAcHistData; 
    MT_DGN_Q_ENERGY_DATA  sAcQEn;
    MT_DGN_ENERGY_DATA	sAcEn;
    memcpy(Data,Data1,35); 

    memset(&sAcQEn, 0, sizeof(MT_DGN_Q_ENERGY_DATA));
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));

    GetLastMonData(&sAcHistData,0);
    memcpy(&sAcQEn, &sAcHistData.sAcQEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcQEn.sQ2Energy[0], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data);

    Data[21] += 0x01;
    Data[22] += 0x02;    
    Data[23] += 0x02;
    Data[24] += 0x0d; 
    Data[14] += 0x02;
    Data[15] += 0x02;
    Data[25] += 0x01;                        
    Data[28] += 0x01;                  //上 1月
    Data[29] += 0x06;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}

//第 44 屏   上 1月第 3象 限无功总电量 
void  LcdDisplayPage44(void)                  //每秒刷新一次
{    
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI  = 0x00070001;
    INT32U      nDI         = 0x50050201;
    MEM_AC_DJ_DATA     sAcHistData;
    MT_DGN_Q_ENERGY_DATA  sAcQEn;
    MT_DGN_ENERGY_DATA	sAcEn;
    memcpy(Data,Data1,35); 

    memset(&sAcQEn, 0, sizeof(MT_DGN_Q_ENERGY_DATA));
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));

    GetLastMonData(&sAcHistData,0);
    memcpy(&sAcQEn, &sAcHistData.sAcQEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcQEn.sQ3Energy[0], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data);
    Data[21] += 0x01;
    Data[22] += 0x02;    
    Data[23] += 0x03;
    Data[24] += 0x0d;
    Data[14] += 0x02;
    Data[15] += 0x02;
    Data[25] += 0x01;                        
    Data[28] += 0x01;                  //上 1月
    Data[29] += 0x06;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}

//第 45 屏   上 1月第 4 象限无功总电量 
void  LcdDisplayPage45(void)                  //每秒刷新一次
{    
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI  = 0x00080001;
    INT32U      nDI         = 0x50050201;
    MEM_AC_DJ_DATA     sAcHistData; 
    MT_DGN_Q_ENERGY_DATA  sAcQEn;
    MT_DGN_ENERGY_DATA	sAcEn;
    memcpy(Data,Data1,35); 

    memset(&sAcQEn, 0, sizeof(MT_DGN_Q_ENERGY_DATA));
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));

    GetLastMonData(&sAcHistData,0);
    memcpy(&sAcQEn, &sAcHistData.sAcQEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcQEn.sQ4Energy[0], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data);

    Data[21] += 0x01;
    Data[22] += 0x03;
    Data[24] += 0x0d;
    Data[14] += 0x02;
    Data[15] += 0x02;
    Data[25] += 0x01;                        
    Data[28] += 0x01;                  //上 1月
    Data[29] += 0x06;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}
//第 46屏  通信地址低 8位
void  LcdDisplayPage46(void)                  //每秒刷新一次
{    
    INT8U       i ;
    INT8U       Data[35];
    INT8U       nBuf1[8];
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    INT8U       nBuf[4];
    INT8U        nMemAddr[6] = {0};
    //INT32U      nDI         = 0x04000401;
    INT32U      nDI         = 0x40010200;

    memcpy(Data,Data1,35); 
    MemReadAcPrm(F19, &nMemAddr[0], 6);
    memcpy(nBuf, &nMemAddr[0], 6);
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
        memset(nBuf, 0, sizeof(nBuf));
    }

    for(i = 0; i < 4; i ++ )
    {
        nBuf1[i*2]  = nBuf[i]&0x0f;
        nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }
    Long2Buf((INT8U*)&nTemBuf, nDI, 4);    
    for(i = 0; i < 8; i ++)
    { 
        nBuf2[i*2]  = nTemBuf[i]&0x0f;
        nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
        Data[15-i*2] += DataAll[nBuf2[i] *2];
        Data[14-i*2] += DataAll[nBuf2[i] *2 + 1];
    }           
    Data[17] = DataAll[nBuf1[0] *2 ];
    Data[16] = DataAll[nBuf1[0] *2 + 1];
    for(i = 1; i < 8; i ++ )
    {
        Data[19 + i*2] += DataAll[nBuf1[i] *2];
        Data[20 + i*2] += DataAll[nBuf1[i] *2 + 1 ];
    }  

    Data[2] += 0x04;
    Data[2] &= 0xF6;
    Data[3] &= 0xFD;

    BULCDWrite1(0, Data, 35);     
}
//第 47 屏  通信地址高 4位
void  LcdDisplayPage47(void)                  //每秒刷新一次
{   
    INT8U       i ;
    INT8U       Data[35] ;
    INT8U       nBuf1[4];
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    INT8U       nBuf[6] ;
    INT8U        nMemAddr[6] = {0};
    //INT32U      nDI         = 0x04000401;
    INT32U      nDI         = 0x40010200;

    memcpy(Data,Data1,35); 
    MemReadAcPrm(F19, &nMemAddr[0], 6);
    memcpy(nBuf, &nMemAddr[0], 6);
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
        memset(nBuf, 0, sizeof(nBuf));
    }

    for(i = 0; i < 2; i ++ )
    {
        nBuf1[i*2]  = nBuf[i +4]&0x0f;
        nBuf1[i*2+1]  = nBuf[i +4]>>0x04;
    }
    Long2Buf((INT8U*)&nTemBuf, nDI, 4);    
    for(i = 0; i < 8; i ++)
    { 
        nBuf2[i*2]  = nTemBuf[i]&0x0f;
        nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
        Data[15-i*2] += DataAll[nBuf2[i] *2];
        Data[14-i*2] += DataAll[nBuf2[i] *2 + 1];
    }
    Data[17] = DataAll[nBuf1[0] *2];
    Data[16] = DataAll[nBuf1[0] *2 + 1 ];
    for(i = 1; i < 4; i ++ )
    {
        Data[19 + i*2] += DataAll[nBuf1[i] *2];
        Data[20 + i*2] += DataAll[nBuf1[i] *2 +1];     
    }  
    Data[2] &= 0xF0;
    Data[3] &= 0xF6;

    BULCDWrite1(0, Data, 35);     
}
//第 48 屏   通信波特率
void  LcdDisplayPage48(void)                  
{   INT8U       i ;
    INT8U       Data[35];
    INT8U       nBuf1[4];
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];;
    INT32U      Baud = 2400;
    //INT32U      nDI         = 0x04000703;
    INT32U      nDI         = 0xf2010201;
    MEM_AC_PRM	sAc_Param;
    
    MemReadAcPrm(F27, &sAc_Param.nCom1Baud, sizeof(sAc_Param.nCom1Baud));
    if(0x20 ==sAc_Param.nCom1Baud)
    {
        Baud = 9600;
    }
    memcpy(Data,Data1,35);
    for(i = 0; i < 4; i ++)
    { 
        nBuf1[i]=Baud%10;
        Baud/= 10;
    }
    Long2Buf((INT8U*)&nTemBuf, nDI, 4);    
    for(i = 0; i < 8; i ++)
    { 
        nBuf2[i*2]  = nTemBuf[i]&0x0f;
        nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
        Data[15-i*2] += DataAll[nBuf2[i] *2];
        Data[14-i*2] += DataAll[nBuf2[i] *2 + 1];
    } 
    Data[17] = DataAll[nBuf1[0] *2 ];
    Data[16] = DataAll[nBuf1[0] *2 +1];
    for(i = 1; i < 4; i ++ )
    {
        Data[19 + i*2] += DataAll[nBuf1[i] *2 ];
        Data[20 + i*2] += DataAll[nBuf1[i] *2 +1];     
    }  

    BULCDWrite1(0, Data, 35);     
}
//第 49 屏   有功脉冲常数   
void  LcdDisplayPage49(void)                  
{   INT8U       i ;
    INT8U       Data[35] ;
    INT8U       nBuf1[5];
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    INT32U       Ac_Pulse_Const = g_nAcPulseConst;   //pDataTable->nPulseConst;
    //INT32U      nDI         = 0x04000409;
    INT32U      nDI         = 0x41090200;
    
    memcpy(Data,Data1,35); 
   for(i = 0; i < 5; i ++)
    {
     nBuf1[i]=Ac_Pulse_Const%10;
    Ac_Pulse_Const/= 10;
        }
   Long2Buf((INT8U*)&nTemBuf, nDI, 4);    
   for(i = 0; i < 8; i ++)
   { 
      nBuf2[i*2]  = nTemBuf[i]&0x0f;
      nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
      Data[15-i*2] += DataAll[nBuf2[i] *2];
      Data[14-i*2] += DataAll[nBuf2[i] *2 + 1];
      }
       Data[17] = DataAll[nBuf1[0] *2 ];
      Data[16] = DataAll[nBuf1[0] *2 +1];
      for(i = 1; i < 5; i ++ )
      { 
        Data[19 + i*2] += DataAll[nBuf1[i] *2 ];
        Data[20 + i*2] += DataAll[nBuf1[i] *2 +1];     
      }
      Data[24] += 0x03;         //有功
    BULCDWrite1(0, Data, 35);
}
//第 50 屏   无功脉冲常数   
void  LcdDisplayPage50(void)                  
{   INT8U       i ;
    INT8U       Data[35];
    INT8U       nBuf1[5];
    INT8U       nBuf2[16];
     INT8U       nTemBuf[4];
    INT32U       Ac_Pulse_Const = g_nAcPulseConst;     //pDataTable->nPulseConst;
    //INT32U      nDI         = 0x0400040A;
    INT32U      nDI         = 0x410A0200;
    
    memcpy(Data,Data1,35); 
   for(i = 0; i < 5; i ++)
    {
        nBuf1[i]=Ac_Pulse_Const%10;
        Ac_Pulse_Const/= 10;
     }
   Long2Buf((INT8U*)&nTemBuf, nDI, 4);    
   for(i = 0; i < 8; i ++)
   { 
      nBuf2[i*2]  = nTemBuf[i]&0x0f;
      nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
      Data[15-i*2] += DataAll[nBuf2[i] *2];
      Data[14-i*2] += DataAll[nBuf2[i] *2 + 1];
    }
       Data[17] = DataAll[nBuf1[0] *2 ];
      Data[16] = DataAll[nBuf1[0] *2 +1];
      for(i = 1; i < 5; i ++ )
      { 
        Data[19 + i*2] += DataAll[nBuf1[i] *2 ];
        Data[20 + i*2] += DataAll[nBuf1[i] *2 +1];     
      }
      Data[24] += 0x01;         //无功
      Data[25] += 0x01;         
    BULCDWrite1(0, Data, 35);
}

//第 51 屏   总失压次数
void  LcdDisplayPage51(void)                  
{   
    INT8U       i ;
    INT8U       j ;
    INT8U       Data[35];
    INT8U       nBuf1[10];
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    INT8U       nBuf[3];
    INT32U      nTmp32U    = 0;
    //INT32U      nDI         = 0x10000001;
    INT32U      nDI         = 0x30000D01;

    memcpy(Data,Data1,35); 
    MemReadAcXFlhBackData(F25, &nTmp32U, sizeof(nTmp32U));
    INT32U2BCD(nTmp32U, 6,&g_nAcEventBuf[0]);
    memcpy(nBuf, &g_nAcEventBuf[0], 3);
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
        memset(nBuf, 0, sizeof(nBuf));
    }

    for(i = 0; i < 3; i ++)
    {
        nBuf1[i*2]  = nBuf[i]&0x0f;
        nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }
    Long2Buf((INT8U*)&nTemBuf, nDI, 4);    
    for(i = 0; i < 8; i ++)
    { 
        nBuf2[i*2]  = nTemBuf[i]&0x0f;
        nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
        Data[15-i*2] += DataAll[nBuf2[i] *2];
        Data[14-i*2] += DataAll[nBuf2[i] *2 + 1];
    }           
    Data[17] = DataAll[nBuf1[0] *2 ];
    Data[16] = DataAll[nBuf1[0] *2 +1];

    for(j = 5;j > 0;j--)
    {
        if(nBuf1[j]!= 0)
        {
            for(i = 0; i < j; i ++)
            {  
                Data[21 + i*2] += DataAll[nBuf1[i + 1] *2];
                Data[22 + i*2] += DataAll[nBuf1[i + 1] *2 + 1];
            }
            j = 1;
        }
    }

    Data[22] += 0x04; 
    Data[21] += 0x09;        
    BULCDWrite1(0, Data, 35);
}
//第 52 屏   总失压累计时间
void  LcdDisplayPage52(void)                  
{    
    INT8U       i ;
    INT8U       j ;
    INT8U       Data[35];
    INT8U       nBuf1[10];
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    INT8U       nBuf[4];
    INT32U      nTmp32U    = 0;
    //INT32U      nDI         = 0x10000002;
    INT32U      nDI         = 0x30000D02;

    memcpy(Data,Data1,35); 
    MemReadAcXBackData(F1, &nTmp32U, sizeof(nTmp32U));
    nTmp32U = (nTmp32U / 60);

    INT32U2BCD(nTmp32U, 6,&g_nAcEventBuf[0]);
    memcpy(nBuf, &g_nAcEventBuf[0], 3);

    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
        memset(nBuf, 0, sizeof(nBuf));
    }
    for(i = 0; i < 3; i ++ )
    {
        nBuf1[i*2]  = nBuf[i]&0x0f;
        nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }
    Long2Buf((INT8U*)&nTemBuf, nDI, 4);    
    for(i = 0; i < 8; i ++)
    { 
        nBuf2[i*2]  = nTemBuf[i]&0x0f;
        nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
        Data[15-i*2] += DataAll[nBuf2[i] *2];
        Data[14-i*2] += DataAll[nBuf2[i] *2 + 1];
    }           
    Data[17] = DataAll[nBuf1[0] *2 ] + 0x08;
    Data[16] = DataAll[nBuf1[0] *2 +1] + 0x08;

    for(j = 5;j > 0;j--)
    {
        if(nBuf1[j]!= 0)
        {
            for(i = 0; i < j; i ++)
            {   
                Data[21 + i*2] += DataAll[nBuf1[i + 1] *2];
                Data[22 + i*2] += DataAll[nBuf1[i + 1] *2 + 1];
            } 
            j = 1;
        }
    }

    Data[21] += 0x09;
    Data[22] += 0x04; 
    BULCDWrite1(0, Data, 35);     
}
//第 53 屏  最近一次失压起始 日期
void  LcdDisplayPage53(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf1[6];
    INT8U   nTmpTime[6];
    //INT32U      nDI         = 0x10000101;
    INT32U      nDI         = 0x30000D03;

    memcpy(Data,Data1,35);    
    MemReadAcXFlhBackData(F26, &nTmpTime[0], sizeof(nTmpTime));

    for(i = 0; i < 3; i ++ )
    {
        nBuf1[i*2]  = nTmpTime[i +3]&0x0f;
        nBuf1[i*2+1]  = nTmpTime[i +3]>>0x04;
    }

    Time1Deal( &nBuf1[0], nDI, Data);
    Data[17] += 0x08;
    Data[16] += 0x08;
    Data[21] += 0x08;
    Data[22] += 0x04;
    Data[29] += 0x06;
    Data[31] += 0x02;
    Data[2] &= 0xF0;
    Data[3] &= 0xF6;
    BULCDWrite1(0, Data, 35);     
}
//第 54 屏 最近一次失压起始时间
void  LcdDisplayPage54(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;
    INT8U       Data[35];
    INT8U       nBuf1[6];
    INT8U       nTmpTime[6];
    //INT32U      nDI         = 0x10000101;
    INT32U      nDI         = 0x30000D03;

    memcpy(Data,Data1,35); 
    MemReadAcXFlhBackData(F26, &nTmpTime[0], sizeof(nTmpTime));

    for(i = 0; i < 3; i ++ )
    {
        nBuf1[i*2]  = nTmpTime[i]&0x0f;
        nBuf1[i*2+1]  = nTmpTime[i]>>0x04;
    }

    Time2Deal( nBuf1, nDI, Data);
    Data[17] += 0x08;
    Data[16] += 0x08;  
    Data[21] += 0x08;
    Data[22] += 0x04;
    Data[29] += 0x06;
    Data[31] += 0x02;
    Data[2] += 0x04;
    Data[2] &= 0xF6;
    Data[3] &= 0xFD;

    BULCDWrite1(0, Data, 35);     
}
//第 55 屏  最近一次失压结束 日期
void  LcdDisplayPage55(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;
    INT8U       Data[35];
    INT8U       nBuf1[6];
    INT8U       nTmpTime[6];
    //INT32U      nDI         = 0x10000201;
    INT32U      nDI         = 0x30000D04;

    memcpy(Data,Data1,35);    
    MemReadAcXFlhBackData(F27, &nTmpTime[0], sizeof(nTmpTime));

    for(i = 0; i < 3; i ++ )
    {
        nBuf1[i*2]  = nTmpTime[i +3]&0x0f;
        nBuf1[i*2+1]  = nTmpTime[i +3]>>0x04;
    }

    Time1Deal( nBuf1, nDI, Data);
    Data[17] += 0x08;
    Data[16] += 0x08;
    Data[21] += 0x08;
    Data[22] += 0x04;
    Data[29] += 0x06;
    Data[31] += 0x02;
    Data[2] &= 0xF0;
    Data[3] &= 0xF6;
    BULCDWrite1(0, Data, 35);     
}
//第 56 屏 最近一次失压结束 时间
void  LcdDisplayPage56(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;
    INT8U       Data[35] ;
    INT8U       nBuf1[6];
    INT8U       nTmpTime[6];
    //INT32U      nDI         = 0x1000201;
    INT32U      nDI         = 0x30000D04;

    memcpy(Data,Data1,35); 
    MemReadAcXFlhBackData(F27, &nTmpTime[0], sizeof(nTmpTime));
    for(i = 0; i < 3; i ++ )
    {
        nBuf1[i*2]  = nTmpTime[i]&0x0f;
        nBuf1[i*2+1]  = nTmpTime[i]>>0x04;
    }         
    Time2Deal( nBuf1, nDI, Data);
    Data[17] += 0x08;
    Data[16] += 0x08;  
    Data[21] += 0x08;
    Data[22] += 0x04;
    Data[29] += 0x06;
    Data[31] += 0x02;
    Data[2] += 0x04;
    Data[2] &= 0xF6;
    Data[3] &= 0xFD;
    BULCDWrite1(0, Data, 35);     
}
// 第 57 屏  最近一次 A相 失压起始时刻正 向有功 电量
void  LcdDisplayPage57(void)                  //每秒刷新一次
{      
    INT8U       Data[35];  
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x10010201;
    INT32U      nDI         = 0x30000701;
    
    memcpy(Data,Data1,35); 
     MemReadAcEvtProc(F1, 1, &g_nAcEventBuf[0], 195);
      memcpy(nBuf, &g_nAcEventBuf[6], 4);

      DataDeal(nBuf ,nDI, Data);
       Data[12] += 0x02;
      Data[14] += 0x01;
     Data[15] += 0x01;
      Data[21] += 0x09;
     Data[22] += 0x04;
     Data[24] += 0x0f;
     Data[25] += 0x06;                  //正向
     Data[28] += 0x02;
     Data[29] += 0x06;
     Data[31] += 0x02;

      BULCDWrite1(0, Data, 35);
}
// 第 58 屏  最近一次 A相 失压结束时刻正 向有功 电量
void  LcdDisplayPage58(void)                  //每秒刷新一次
{    
    INT8U       Data[35];  
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x10012601;
    INT32U      nDI         = 0x30000701;
    
   memcpy(Data,Data1,35); 
   MemReadAcEvtProc(F1, 1, &g_nAcEventBuf[0], 195);
      memcpy(nBuf, &g_nAcEventBuf[131], 4);
     
     DataDeal(nBuf ,nDI, Data);
      Data[12] += 0x02;
     Data[14] += 0x01;
     Data[15] += 0x01;
     Data[21] += 0x09;
     Data[22] += 0x04;
     Data[24] += 0x0f;
     Data[25] += 0x06;                  //正向
     Data[28] += 0x02;
     Data[29] += 0x06;
     Data[31] += 0x02;

      BULCDWrite1(0, Data, 35);
}
// 第 59屏  最近一次 A相 失压起始时刻反 向有功 电量
void  LcdDisplayPage59(void)                  //每秒刷新一次
{      
    INT8U       Data[35];  
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x10010301;
    INT32U      nDI         = 0x30000701;
      
    memcpy(Data,Data1,35); 
    MemReadAcEvtProc(F1, 1, &g_nAcEventBuf[0], 195);
      memcpy(nBuf, &g_nAcEventBuf[10], 4);
     
    DataDeal(nBuf ,nDI, Data);
     Data[12] += 0x02;
    Data[14] += 0x01;
     Data[15] += 0x01;
    Data[21] += 0x09;
     Data[22] += 0x04;
     Data[24] += 0x0f;
    Data[25] += 0x02; 
     Data[26] += 0x01;                  //反向
     Data[28] += 0x02;
     Data[29] += 0x06;
     Data[31] += 0x02;

      BULCDWrite1(0, Data, 35);
}
// 第 60 屏  最近一次 A相 失压结束时刻反 向有功 电量
void  LcdDisplayPage60(void)                  //每秒刷新一次
{       
    INT8U       Data[35];  
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x10012701;
    INT32U      nDI         = 0x30000701;
      
    memcpy(Data,Data1,35); 
    MemReadAcEvtProc(F1, 1, &g_nAcEventBuf[0], 195);
    memcpy(nBuf, &g_nAcEventBuf[135], 4);
    
      DataDeal(nBuf ,nDI, Data);
       Data[12] += 0x02;
      Data[14] += 0x01;
     Data[15] += 0x01;
      Data[21] += 0x09;
     Data[22] += 0x04;
     Data[24] += 0x0f;
     Data[25] += 0x02; 
      Data[26] += 0x01;                  //反向
     Data[28] += 0x02;
     Data[29] += 0x06;
     Data[31] += 0x02;

      BULCDWrite1(0, Data, 35);
}
// 第 61 屏  最近一次 B相 失压起始时刻正 向有功 电量
void  LcdDisplayPage61(void)                  //每秒刷新一次
{     
    INT8U       Data[35];  
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x10020201;
    INT32U      nDI         = 0x30000801;
      
    memcpy(Data,Data1,35); 
     MemReadAcEvtProc(F2, 1, &g_nAcEventBuf[0], 195);
      memcpy(nBuf, &g_nAcEventBuf[6], 4);
         
      DataDeal(nBuf ,nDI, Data); 
       Data[12] += 0x02;
      Data[14] += 0x01;
     Data[15] += 0x01;
      Data[21] += 0x09;
     Data[22] += 0x04;
     Data[24] += 0x0f;
     Data[25] += 0x06;                  //正向
     Data[28] += 0x04;
     Data[29] += 0x06;
     Data[31] += 0x02;

      BULCDWrite1(0, Data, 35);
}
// 第 62 屏  最近一次 B相 失压结束时刻正 向有功 电量
void  LcdDisplayPage62(void)                  //每秒刷新一次
{        
    INT8U       Data[35];  
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x10022601;
    INT32U      nDI         = 0x30000801;
      
    memcpy(Data,Data1,35);  
     MemReadAcEvtProc(F2, 1, &g_nAcEventBuf[0], 195);
      memcpy(nBuf, &g_nAcEventBuf[131], 4);

      DataDeal(nBuf ,nDI, Data);
       Data[12] += 0x02;
      Data[14] += 0x01;
     Data[15] += 0x01;
      Data[21] += 0x09;
     Data[22] += 0x04;
      Data[24] += 0x0f;
     Data[25] += 0x06;                  //正向
     Data[28] += 0x04;
     Data[29] += 0x06;
     Data[31] += 0x02;

      BULCDWrite1(0, Data, 35);
}
// 第 63 屏  最近一次 B相 失压起始时刻反 向有功 电量
void  LcdDisplayPage63(void)                  //每秒刷新一次
{       
    INT8U       Data[35];  
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x10020301;
    INT32U      nDI         = 0x30000801;
      
    memcpy(Data,Data1,35); 
     MemReadAcEvtProc(F2, 1, &g_nAcEventBuf[0], 195);
      memcpy(nBuf, &g_nAcEventBuf[10], 4);
  
      DataDeal(nBuf ,nDI, Data);
       Data[12] += 0x02;
      Data[14] += 0x01;
     Data[15] += 0x01;
      Data[21] += 0x09;
     Data[22] += 0x04;
     Data[24] += 0x0f;
     Data[25] += 0x02; 
     Data[26] += 0x01;                  //反向
     Data[28] += 0x04;
     Data[29] += 0x06;
     Data[31] += 0x02;

      BULCDWrite1(0, Data, 35);
}
// 第 64 屏  最近一次 B相 失压结束时刻反 向有功 电量
void  LcdDisplayPage64(void)                  //每秒刷新一次
{       
    INT8U       Data[35];  
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x10022701;
    INT32U      nDI         = 0x30000801;
      
    memcpy(Data,Data1,35); 
     MemReadAcEvtProc(F2, 1, &g_nAcEventBuf[0], 195);
      memcpy(nBuf, &g_nAcEventBuf[135], 4);
    
      DataDeal(nBuf ,nDI, Data);
       Data[12] += 0x02;
      Data[14] += 0x01;
     Data[15] += 0x01;
       Data[21] += 0x09;
     Data[22] += 0x04;
     Data[24] += 0x0f;
     Data[25] += 0x02; 
      Data[26] += 0x01;                  //反向
     Data[28] += 0x04;
     Data[29] += 0x06;
     Data[31] += 0x02;

      BULCDWrite1(0, Data, 35);
}
// 第 65屏  最近一次 C相 失压起始时刻正 向有功 电量
void  LcdDisplayPage65(void)                  //每秒刷新一次
{        
    INT8U       Data[35];  
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x10030201;
    INT32U      nDI         = 0x30000901;
    
    memcpy(Data,Data1,35); 
    MemReadAcEvtProc(F3, 1, &g_nAcEventBuf[0], 195);
      memcpy(nBuf, &g_nAcEventBuf[6], 4);
       
     DataDeal(nBuf ,nDI, Data);
      Data[12] += 0x02;
     Data[14] += 0x01;
     Data[15] += 0x01;
    Data[21] += 0x09;
     Data[22] += 0x04;
     Data[24] += 0x0f;
     Data[25] += 0x06;                  //正向
     Data[28] += 0x08;
     Data[29] += 0x06;
     Data[31] += 0x02;

      BULCDWrite1(0, Data, 35);
}
// 第 66 屏  最近一次 C相 失压结束时刻正 向有功 电量
void  LcdDisplayPage66(void)                  //每秒刷新一次
{    
    INT8U       Data[35];  
    INT8U       nBuf[4] ;
    //INT32U      nDI         = 0x10032601;
    INT32U      nDI         = 0x30000901;

    memcpy(Data,Data1,35); 
    MemReadAcEvtProc(F3, 1, &g_nAcEventBuf[0], 195);
    memcpy(nBuf, &g_nAcEventBuf[131], 4);

    DataDeal(nBuf ,nDI, Data);

    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01;
    Data[21] += 0x09;
    Data[22] += 0x04;  
    Data[24] += 0x0f;
    Data[25] += 0x06;                  //正向
    Data[28] += 0x08;
    Data[29] += 0x06;
    Data[31] += 0x02;

    BULCDWrite1(0, Data, 35);
}
// 第 67 屏  最近一次 C相 失压起始时刻反 向有功 电量
void  LcdDisplayPage67(void)                  //每秒刷新一次
{      
    INT8U       Data[35] ;  
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x10030301;
    INT32U      nDI         = 0x30000901;
      
    memcpy(Data,Data1,35); 
     MemReadAcEvtProc(F3, 1, &g_nAcEventBuf[0], 195);
      memcpy(nBuf, &g_nAcEventBuf[10], 4);
    
     DataDeal(nBuf ,nDI, Data);
        
      Data[12] += 0x02;
     Data[14] += 0x01;
     Data[15] += 0x01;
     Data[21] += 0x09;
     Data[22] += 0x04;
     Data[24] += 0x0f;
     Data[25] += 0x02; 
     Data[26] += 0x01;                  //反向
     Data[28] += 0x08;
     Data[29] += 0x06;
     Data[31] += 0x02;

      BULCDWrite1(0, Data, 35);
}
// 第 68 屏  最近一次 C相 失压结束时刻反 向有功 电量
void  LcdDisplayPage68(void)                  //每秒刷新一次
{      
    INT8U       Data[35];  
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x10032701;
    INT32U      nDI         = 0x30000901;
      
    memcpy(Data,Data1,35); 
     MemReadAcEvtProc(F3, 1, &g_nAcEventBuf[0], 195);
      memcpy(nBuf, &g_nAcEventBuf[135], 4);
    
     DataDeal(nBuf ,nDI, Data);
      
      Data[12] += 0x02;
     Data[14] += 0x01;
     Data[15] += 0x01;
     Data[21] += 0x09;
     Data[22] += 0x04;
     Data[24] += 0x0f;
     Data[25] += 0x02; 
      Data[26] += 0x01;                  //反向
     Data[28] += 0x08;
     Data[29] += 0x06;
     Data[31] += 0x02;

      BULCDWrite1(0, Data, 35);
}
//第 69 屏   A相电压
void  LcdDisplayPage69(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[2];
    INT8U       nBuf1[10]; 
    INT8U       nBuf2[16];
     INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x02010100;
     INT32U      nDI         = 0x20000201;
    MEM_AC_REAL_DATA            sRealData;             //使用全局变量
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    memcpy(Data,Data1,35); 
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    memcpy(nBuf, &pDataTable->sRealInstant.sRealVolt[0].nFrac, 2);
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }
    
    //A相电压为50%
    if (((g_MeterHardwareWrong[1]&0x04)==0x04) || (1 == g_MeterEventWrong[0]&0x01))
    { 
       DivideTwo(nBuf);
    }
    
    if (0x11 == (g_MeterVoltPlusFlg&0x11))//电压越上限
    {
        nBuf[1] |= 0x05;
    }
    else if((0x21 == (g_MeterVoltPlusFlg&0x21))||(g_MeterVoltPlusFlg == 0x01))//电压越下限
    {
        DivideTwo2(nBuf); //DivideTwo(nBuf);
    }
    
    //A相无电压
    if(((g_MeterHardwareWrong[0]&0x10)==0x10) || (0x02==(g_MeterEventWrong[0]&0x02)))
    {
       memset(nBuf, 0, sizeof(nBuf));
    }
    
    for(i = 0; i < 2; i ++ )
    {
      nBuf1[i*2]  = nBuf[i]&0x0f;
      nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }
    Long2Buf((INT8U*)&nTemBuf, nDI, 4);    
   for(i = 0; i < 8; i ++)
   { 
      nBuf2[i*2]  = nTemBuf[i]&0x0f;
      nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
      Data[15-i*2] |= DataAll[nBuf2[i] *2];
      Data[14-i*2] |= DataAll[nBuf2[i] *2 + 1];
   }           
     Data[17] = Data_P[nBuf1[0] *2];
     Data[16] = Data_P[nBuf1[0] *2 + 1];     
     Data[21] |= DataAll[nBuf1[1] *2] + 0x08;
     Data[22] |= DataAll[nBuf1[1] *2 + 1];
     if(nBuf1[3]!= 0)       
      { for(i = 0;i < 2;i ++)
        { Data[23 + i*2] = DataAll[nBuf1[i + 2]*2];
          Data[24 + i*2] = DataAll[nBuf1[i + 2]*2 + 1];
        }
      }
     else if(nBuf1[2]!= 0)       
      { 
         Data[23] = DataAll[nBuf1[2]*2 ];
         Data[24] = DataAll[nBuf1[2]*2 + 1 ];
      }
      Data[14] |= 0x01;
      Data[24] |= 0x08;
     Data[28] |= 0x02;

    BULCDWrite1(0, Data, 35);
}
//第 70屏   B 相电压
void  LcdDisplayPage70(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;   
    INT8U       Data[35] ;
    INT8U       nBuf[2];
    INT8U       nBuf1[10];
    INT8U       nBuf2[16];
     INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x02010200;
     INT32U      nDI         = 0x20000202;
     
    MEM_AC_REAL_DATA            sRealData;             //使用全局变量
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    memset(nBuf, 0, sizeof(nBuf));
    memcpy(Data,Data1,35); 
if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType))//三相四线与三相三线区分
{
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    memcpy(nBuf, &pDataTable->sRealInstant.sRealVolt[1].nFrac, 2);
}  
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }

    //B相电压为50%
    if(((g_MeterHardwareWrong[1]&0x08)==0x08) || (1 == g_MeterEventWrong[1]&0x01))
    { 
       DivideTwo(nBuf);
    }
    
    if (0x12 == (g_MeterVoltPlusFlg&0x12))//电压越上限
    {
        nBuf[1] |= 0x05;
    }
    else if((0x22 == (g_MeterVoltPlusFlg&0x22))||(g_MeterVoltPlusFlg == 0x01))//电压越下限
    {
        DivideTwo2(nBuf); //DivideTwo(nBuf);
    }

    //B相无电压
    if(((g_MeterHardwareWrong[0]&0x20)==0x20) || (0x02==(g_MeterEventWrong[1]&0x02)))
    {
       memset(nBuf, 0, sizeof(nBuf));
    }
    for(i = 0; i < 2; i ++ )
    {
      nBuf1[i*2]  = nBuf[i]&0x0f;
      nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }
    Long2Buf((INT8U*)&nTemBuf, nDI, 4);    
   for(i = 0; i < 8; i ++)
   { 
      nBuf2[i*2]  = nTemBuf[i]&0x0f;
      nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
      Data[15-i*2] |= DataAll[nBuf2[i] *2];
      Data[14-i*2] |= DataAll[nBuf2[i] *2 + 1];
   }           
     Data[17] = Data_P[nBuf1[0] *2];
     Data[16] = Data_P[nBuf1[0] *2 + 1];     
     Data[21] |= DataAll[nBuf1[1] *2 ] + 0x08;
     Data[22] |= DataAll[nBuf1[1] *2 + 1];
      if(nBuf1[3]!= 0)       
      { 
        for(i = 0;i < 2;i ++)
        { Data[23 + i*2] = DataAll[nBuf1[i + 2]*2 ];
          Data[24 + i*2] = DataAll[nBuf1[i + 2]*2 + 1];
        }
      }
     else if(nBuf1[2]!= 0)       
      { 
         Data[23] = DataAll[nBuf1[2]*2 ];
         Data[24] = DataAll[nBuf1[2]*2 +1 ];
      }
     
     Data[14] |= 0x01;
      Data[24] |= 0x08;
     Data[28] |= 0x04;

    BULCDWrite1(0, Data, 35);
}
//第 71 屏   C 相电压
void  LcdDisplayPage71(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[2];
    INT8U       nBuf1[10];
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x02010300;
    INT32U      nDI         = 0x20000203;
    
    MEM_AC_REAL_DATA            sRealData;             //使用全局变量
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    memcpy(Data,Data1,35); 
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    memcpy(nBuf, &pDataTable->sRealInstant.sRealVolt[2].nFrac, 2);
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }
    
    //C相电压为50%
    if(((g_MeterHardwareWrong[1]&0x10)==0x10) || (1 == g_MeterEventWrong[2]&0x01))
    {
       DivideTwo(nBuf);
    }
    
    if (0x14 == (g_MeterVoltPlusFlg&0x14))//电压越上限
    {
        nBuf[1] |= 0x05;
    }
    else if((0x24 == (g_MeterVoltPlusFlg&0x24))||(g_MeterVoltPlusFlg == 0x01))//电压越下限
    {
       DivideTwo2(nBuf); //DivideTwo(nBuf);
    }
    
    //C相无电压
    if(((g_MeterHardwareWrong[0]&0x40)==0x40) || (0x02==(g_MeterEventWrong[2]&0x02)))
    {
       memset(nBuf, 0, sizeof(nBuf));
    }
    
    for(i = 0; i < 2; i ++ )
    {
      nBuf1[i*2]  = nBuf[i]&0x0f;
      nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }
   Long2Buf((INT8U*)&nTemBuf, nDI, 4);    
   for(i = 0; i < 8; i ++)
   { 
      nBuf2[i*2]  = nTemBuf[i]&0x0f;
      nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
      Data[15-i*2] |= DataAll[nBuf2[i] *2];
      Data[14-i*2] |= DataAll[nBuf2[i] *2 + 1];
   }           
     Data[17] = Data_P[nBuf1[0] *2];
     Data[16] = Data_P[nBuf1[0] *2 + 1];     
     Data[21] |= DataAll[nBuf1[1] *2 ] + 0x08;
     Data[22] |= DataAll[nBuf1[1] *2 + 1];
     if(nBuf1[3]!= 0)       
      { for(i = 0;i < 2;i ++)
        { Data[23 + i*2] = DataAll[nBuf1[i + 2]*2 ];
          Data[24 + i*2] = DataAll[nBuf1[i + 2]*2 + 1];
        }
      }
     else if(nBuf1[2]!= 0)       
      { 
         Data[23] = DataAll[nBuf1[2]*2 ];
         Data[24] = DataAll[nBuf1[2]*2 +1 ];
      }
     
     Data[14] |= 0x01;
     Data[24] |= 0x08;
     Data[28] |= 0x08;

    BULCDWrite1(0, Data, 35);
}
//第 72屏   A 相电流
void  LcdDisplayPage72(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[3];
    INT8U       nBuf1[10];
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x02020100;
    INT32U      nDI         = 0x20010201;
    
    MEM_AC_REAL_DATA            sRealData;             //使用全局变量
    pMEM_AC_REAL_DATA           pRealData = &sRealData;        
     
    memcpy(Data,Data1,35); 
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    memcpy(nBuf,  &pDataTable->sRealInstant.sRealCurr[0].nD1, 3);
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }
    //A相无电流
    if (((g_MeterHardwareWrong[0]&0x80)==0x80) || (0x04==(g_MeterEventWrong[0]&0x04)) || (0x02==(g_MeterEventWrong[0]&0x02)))
    {
       memset(nBuf, 0, sizeof(nBuf));
    }
    //A相电流为50%
    if((g_MeterHardwareWrong[1]&0x20)==0x20)
    { 
       DivideTwo(nBuf);
    }
     if(pDataTable->fCurr[0] < (1.5 * 0.005))
    {
      
    }
    else
    {
        //if((pDataTable->nPFlag & (0x01 << 3))&&(!(pDataTable->sRealInstant.bsRunStat[3] & 0x01)))   //判断方向
        if((pDataTable->nPFlag&0x01)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[1].nFrac0, 3, 0))))               
        {
          Data[25] |= 0x02;
          Data[26] |= 0x01;
        }
    }
     nBuf[2] &= ~0x80;
    for(i = 0; i < 3; i ++ )
    {
      nBuf1[i*2]  = nBuf[i]&0x0f;
      nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }
   Long2Buf((INT8U*)&nTemBuf, nDI, 4);    
   for(i = 0; i < 8; i ++)
   { 
      nBuf2[i*2]  = nTemBuf[i]&0x0f;
      nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
      Data[15-i*2] |= DataAll[nBuf2[i] *2];
      Data[14-i*2] |= DataAll[nBuf2[i] *2 + 1];
   }           
     Data[17] = DataAll[nBuf1[0] *2 ];
     Data[16] = DataAll[nBuf1[0] *2 + 1];     
     Data[21] |= DataAll[nBuf1[1] *2] + 0x04;
     Data[22] |= DataAll[nBuf1[1] *2 + 1];
     
     Data[23] = Data_P[nBuf1[2] *2];
     Data[24] = Data_P[nBuf1[2] *2 + 1] + 0x08;
     Data[25] |= DataAll[nBuf1[3] *2 ];
     Data[26] |= DataAll[nBuf1[3] *2 + 1];
      if(nBuf1[5]!= 0)       
      { for(i = 0;i < 2;i ++)
        { Data[27 + i*2] = DataAll[nBuf1[i + 4]* 2 ];
          Data[28 + i*2] = DataAll[nBuf1[i + 4]* 2 + 1];
        }
      }
     else if(nBuf1[4]!= 0)       
      { 
         Data[27] = DataAll[nBuf1[4]* 2 ];
         Data[28] = DataAll[nBuf1[4]* 2 +1 ];
      }
     
    Data[13] |= 0x02;
    Data[28] |= 0x02 ;
    BULCDWrite1(0, Data, 35);
}

//第 73 屏   B 相电流
void  LcdDisplayPage73(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[3];
    INT8U       nBuf1[10]; 
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x02020200;
    INT32U      nDI         = 0x20010202;
    
    MEM_AC_REAL_DATA            sRealData;             //使用全局变量
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    memset(nBuf, 0, sizeof(nBuf));
    memcpy(Data,Data1,35); 
    if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType))//三相四线与三相三线区分
    {
        MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
        memcpy(nBuf, &pDataTable->sRealInstant.sRealCurr[1].nD1, 3);
    }
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }
    //B相无电流
    if (((g_MeterHardwareWrong[1]&0x01)==0x01) || (0x04==(g_MeterEventWrong[1]&0x04)) || (0x02==(g_MeterEventWrong[1]&0x02)))
    {
       memset(nBuf, 0, sizeof(nBuf));
    }
    //B相电流为50%
    if((g_MeterHardwareWrong[1]&0x40)==0x40)
    { 
       DivideTwo(nBuf);
    }

    if(pDataTable->fCurr[1] < (1.5 * 0.005))
    {
      
    }
    else
    {
        //if((pDataTable->nPFlag & (0x01 << 3))&&(!(pDataTable->sRealInstant.bsRunStat[4] & 0x01)))   //判断方向
        if((pDataTable->nPFlag&(0x01<<1))&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[2].nFrac0, 3, 0))))               
        {
          Data[25] |= 0x02;
         Data[26] |= 0x01;
        }
    }
    nBuf[2] &= ~0x80;
    for(i = 0; i < 3; i ++ )
    {
      nBuf1[i*2]  = nBuf[i]&0x0f;
      nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }
   Long2Buf((INT8U*)&nTemBuf, nDI, 4);    
   for(i = 0; i < 8; i ++)
   { 
      nBuf2[i*2]  = nTemBuf[i]&0x0f;
      nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
      Data[15-i*2] |= DataAll[nBuf2[i] *2];
      Data[14-i*2] |= DataAll[nBuf2[i] *2 + 1];
   }           
     Data[17] = DataAll[nBuf1[0] *2 ];
     Data[16] = DataAll[nBuf1[0] *2 + 1];     
     Data[21] |= DataAll[nBuf1[1] *2 ] + 0x04;
     Data[22] |= DataAll[nBuf1[1] *2 + 1];
     
     Data[23] = Data_P[nBuf1[2] *2];
     Data[24] = Data_P[nBuf1[2] *2 + 1] +0x08;
     Data[25] |= DataAll[nBuf1[3] *2 ];
     Data[26] |= DataAll[nBuf1[3] *2 + 1];
      if(nBuf1[5]!= 0)       
      { for(i = 0;i < 2;i ++)
        { Data[27 + i*2] = DataAll[nBuf1[i + 4]* 2 ];
          Data[28 + i*2] = DataAll[nBuf1[i + 4]* 2 + 1];
        }
      }
     else if(nBuf1[4]!= 0)       
      { 
         Data[27] = DataAll[nBuf1[4] *2];
         Data[28] = DataAll[nBuf1[4] *2 +1 ];
      }
    Data[13] |= 0x02;
    Data[28] |= 0x04;
    BULCDWrite1(0, Data, 35);
}
//第 74 屏   C 相电流
void  LcdDisplayPage74(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[3];
    INT8U       nBuf1[10];
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x02020300;
    INT32U      nDI         = 0x20010203;
    MEM_AC_REAL_DATA            sRealData;             //使用全局变量
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    memcpy(Data,Data1,35); 
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    memcpy(nBuf, &pDataTable->sRealInstant.sRealCurr[2].nD1, 3);
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }
    //C相无电流
    if (((g_MeterHardwareWrong[1]&0x02)==0x02)  || (0x04==(g_MeterEventWrong[2]&0x04)) || (0x02==(g_MeterEventWrong[2]&0x02)))
    {
       memset(nBuf, 0, sizeof(nBuf));
    }
    //C相电流为50%
    if ((g_MeterHardwareWrong[1]&0x80)==0x80)
    { 
       DivideTwo(nBuf);
    }
    
    
    if(pDataTable->fCurr[2] < (1.5 * 0.005))
    {
      
    }
    else
    {
        //if((pDataTable->nPFlag & (0x01 << 3))&&(!(pDataTable->sRealInstant.bsRunStat[5] & 0x01)))   //判断方向
        if((pDataTable->nPFlag&(0x01<<2))&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[3].nFrac0, 3, 0))))               
        {
          Data[25] |= 0x02;
          Data[26] |= 0x01;
        }
    }
     nBuf[2] &= ~0x80;
    
    for(i = 0; i < 3; i ++ )
    {
      nBuf1[i*2]  = nBuf[i]&0x0f;
      nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }
  Long2Buf((INT8U*)&nTemBuf, nDI, 4);    
   for(i = 0; i < 8; i ++)
   { 
      nBuf2[i*2]  = nTemBuf[i]&0x0f;
      nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
      Data[15-i*2] |= DataAll[nBuf2[i] *2];
      Data[14-i*2] |= DataAll[nBuf2[i] *2 + 1];
   }           
     Data[17] = DataAll[nBuf1[0] *2 ];
     Data[16] = DataAll[nBuf1[0] *2 + 1];     
     Data[21] |= DataAll[nBuf1[1] *2 ] + 0x04;
     Data[22] |= DataAll[nBuf1[1] *2 + 1];
     
     Data[23] = Data_P[nBuf1[2] *2];
     Data[24] = Data_P[nBuf1[2] *2 + 1] +0x08;
     Data[25] |= DataAll[nBuf1[3] *2 ];
     Data[26] |= DataAll[nBuf1[3] *2 + 1];
      if(nBuf1[5]!= 0)       
      { for(i = 0;i < 2;i ++)
        { Data[27 + i*2] = DataAll[nBuf1[i + 4]* 2 ];
          Data[28 + i*2] = DataAll[nBuf1[i + 4]* 2 + 1];
        }
      }
     else if(nBuf1[4]!= 0)       
      { 
         Data[27] = DataAll[nBuf1[4] *2];
         Data[28] = DataAll[nBuf1[4] *2 +1 ];
      }
    Data[13] |= 0x02; 
    Data[28] |= 0x08;
    BULCDWrite1(0, Data, 35);
}

//第 75 屏   瞬时总有功功率
void  LcdDisplayPage75(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[3];
    INT8U       nBuf1[10]; 
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x02030000;
    INT32U      nDI         = 0x20040201;
    MEM_AC_REAL_DATA            sRealData;             //使用全局变量
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    memcpy(Data,Data1,35); 
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    memcpy(nBuf, &pDataTable->sRealInstant.sRealP[0].nFrac0, 3 );
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }
    if((pDataTable->nPFlag&0x08)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[0].nFrac0, 3, 0))))               //判断方向
    {
      Data[25] += 0x02;
     Data[26] += 0x01;
    }
    nBuf[2] &= ~0x80;
    for(i = 0; i < 3; i ++ )
    {
      nBuf1[i*2]  = nBuf[i]&0x0f;
      nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }
  Long2Buf((INT8U*)&nTemBuf, nDI, 4);    
   for(i = 0; i < 8; i ++)
   { 
      nBuf2[i*2]  = nTemBuf[i]&0x0f;
      nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
      Data[15-i*2] += DataAll[nBuf2[i] *2];
      Data[14-i*2] += DataAll[nBuf2[i] *2 + 1];
   }           
     Data[17] = DataAll[nBuf1[0] *2 ];
     Data[16] = DataAll[nBuf1[0] *2 + 1] + 0x04;     
     Data[21] += DataAll[nBuf1[1] *2 ] + 0x01;
     Data[22] += DataAll[nBuf1[1] *2 + 1];
     
     Data[23] = DataAll[nBuf1[2]*2 ];
     Data[24] = DataAll[nBuf1[2]*2 + 1] + 0x03;
     Data[25] += Data_P[nBuf1[3] *2];
     Data[26] += Data_P[nBuf1[3] *2 + 1];
     Data[27] = DataAll[nBuf1[4]*2];
    Data[28] = DataAll[nBuf1[4]*2 + 1];
    if((nBuf1[5] != 8)&&(nBuf1[5] != 0))
    { Data[29] = DataAll[nBuf1[5]*2];
      Data[30] = DataAll[nBuf1[5]*2 + 1];  
    }
    
    Data[14] += 0x01;
    Data[15] += 0x01;
    BULCDWrite1(0, Data, 35);
}
//第 76 屏   瞬时A 相有功功率
void  LcdDisplayPage76(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[3];
    INT8U       nBuf1[10];
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x02030100;
    INT32U      nDI         = 0x20040202;
    MEM_AC_REAL_DATA            sRealData;             //使用全局变量
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    memcpy(Data,Data1,35); 
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    memcpy(nBuf, &pDataTable->sRealInstant.sRealP[1].nFrac0, 3 );
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }
     if((pDataTable->nPFlag&0x01)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[1].nFrac0, 3, 0))))               //判断方向
    {
      Data[25] += 0x02;
      Data[26] += 0x01;
    }
    nBuf[2] &= ~0x80;
    for(i = 0; i < 3; i ++ )
    {
      nBuf1[i*2]  = nBuf[i]&0x0f;
      nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }   
  Long2Buf((INT8U*)&nTemBuf, nDI, 4);    
   for(i = 0; i < 8; i ++)
   { 
      nBuf2[i*2]  = nTemBuf[i]&0x0f;
      nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
      Data[15-i*2] += DataAll[nBuf2[i] *2];
      Data[14-i*2] += DataAll[nBuf2[i] *2 + 1];
   }           
     Data[17] = DataAll[nBuf1[0] *2 ];
     Data[16] = DataAll[nBuf1[0] *2 + 1] + 0x04;     
     Data[21] += DataAll[nBuf1[1] *2 ];
     Data[22] += DataAll[nBuf1[1] *2 + 1];
     
     Data[23] = DataAll[nBuf1[2]*2  ];
     Data[24] = DataAll[nBuf1[2]*2 + 1] + 0x03;
     Data[25] += Data_P[nBuf1[3] *2];
     Data[26] += Data_P[nBuf1[3] *2 + 1];
     Data[27] = DataAll[nBuf1[4]*2 ];
     Data[28] = DataAll[nBuf1[4]*2 + 1] + 0x02;
    if((nBuf1[5] != 8)&&(nBuf1[5] != 0))
    { Data[29] = DataAll[nBuf1[5]*2 ];
      Data[30] = DataAll[nBuf1[5]*2 + 1];  
    }
    
    Data[14] += 0x01;
    Data[15] += 0x01;
    BULCDWrite1(0, Data, 35);
}
//第 77 屏   瞬时 B 相有功功率
void  LcdDisplayPage77(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[3];
    INT8U       nBuf1[10];
    INT8U       nBuf2[16];
     INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x02030200;
     INT32U      nDI         = 0x20040203;
    MEM_AC_REAL_DATA            sRealData;             //使用全局变量
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    memset(nBuf, 0, sizeof(nBuf));
    memcpy(Data,Data1,35); 
  if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType))//三相四线与三相三线区分
  {
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    memcpy(nBuf, &pDataTable->sRealInstant.sRealP[2].nFrac0, 3 );
  }
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }
     if((pDataTable->nPFlag&0x02)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[2].nFrac0, 3, 0))))               //判断方向
    {
      Data[25] += 0x02;
     Data[26] += 0x01;
    }
  
    nBuf[2] &= ~0x80;
    for(i = 0; i < 3; i ++ )
    {
      nBuf1[i*2]  = nBuf[i]&0x0f;
      nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }   
  Long2Buf((INT8U*)&nTemBuf, nDI, 4);    
   for(i = 0; i < 8; i ++)
   { 
      nBuf2[i*2]  = nTemBuf[i]&0x0f;
      nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
      Data[15-i*2] += DataAll[nBuf2[i] *2];
      Data[14-i*2] += DataAll[nBuf2[i] *2 + 1];
   }           
    Data[17] = DataAll[nBuf1[0] *2];
     Data[16] = DataAll[nBuf1[0] *2 + 1] + 0x04;     
     Data[21] += DataAll[nBuf1[1] *2];
     Data[22] += DataAll[nBuf1[1] *2 + 1];
     
     Data[23] = DataAll[nBuf1[2]*2 ];
     Data[24] = DataAll[nBuf1[2]*2 + 1] + 0x03;
     Data[25] += Data_P[nBuf1[3] *2];
     Data[26] += Data_P[nBuf1[3] *2 + 1]; 
     Data[27] = DataAll[nBuf1[4]*2 ];
     Data[28] = DataAll[nBuf1[4]*2 + 1] + 0x04;
    if((nBuf1[5] != 8)&&(nBuf1[5] != 0))
    { Data[29] = DataAll[nBuf1[5]*2];
      Data[30] = DataAll[nBuf1[5]*2 + 1];  
    }
    Data[14] += 0x01;
    Data[15] += 0x01;
    BULCDWrite1(0, Data, 35);
}
//第 78 屏   瞬时 C 相有功功率
void  LcdDisplayPage78(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[3];
    INT8U       nBuf1[10];
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x02030300;
    INT32U      nDI         = 0x20040204;
    MEM_AC_REAL_DATA            sRealData;             //使用全局变量
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    memcpy(Data,Data1,35); 
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    memcpy(nBuf, &pDataTable->sRealInstant.sRealP[3].nFrac0, 3 );
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }
    if((pDataTable->nPFlag&0x04)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[3].nFrac0, 3, 0))))               //判断方向
    {
      Data[25] += 0x02;
      Data[26] += 0x01;
    }
    nBuf[2] &= ~0x80;
    for(i = 0; i < 3; i ++ )
    {
      nBuf1[i*2]  = nBuf[i]&0x0f;
      nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }   
    Long2Buf((INT8U*)&nTemBuf, nDI, 4);    
   for(i = 0; i < 8; i ++)
   { 
      nBuf2[i*2]  = nTemBuf[i]&0x0f;
      nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
      Data[15-i*2] += DataAll[nBuf2[i] *2];
      Data[14-i*2] += DataAll[nBuf2[i] *2 + 1];
   }           
     Data[17] = DataAll[nBuf1[0] *2];
     Data[16] = DataAll[nBuf1[0] *2 + 1] + 0x04;     
     Data[21] += DataAll[nBuf1[1] *2 ];
     Data[22] += DataAll[nBuf1[1] *2 + 1];
     
     Data[23] = DataAll[nBuf1[2]*2 ];
     Data[24] = DataAll[nBuf1[2]*2 + 1] + 0x03;
     Data[25] += Data_P[nBuf1[3] *2];
     Data[26] += Data_P[nBuf1[3] *2 + 1]; 
     Data[27] = DataAll[nBuf1[4]*2  ];
     Data[28] = DataAll[nBuf1[4]*2 + 1] + 0x08;
    if((nBuf1[5] != 8)&&(nBuf1[5] != 0))
    { Data[29] = DataAll[nBuf1[5]*2];
      Data[30] = DataAll[nBuf1[5]*2 + 1];  
    }
    Data[14] += 0x01;
    Data[15] += 0x01;
    BULCDWrite1(0, Data, 35);
}
//第 79 屏   瞬时总功率 因数
void  LcdDisplayPage79(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[2];
    INT8U       nBuf1[10];
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x02060000 ;
    INT32U      nDI         = 0x200A0201;
    MEM_AC_REAL_DATA            sRealData;             //使用全局变量
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    memcpy(Data,Data1,35); 
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    memcpy(nBuf, &pDataTable->sRealInstant.sRealPf[0].nFrac, 2 );
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }
    nBuf[1] &= ~0x80;
    for(i = 0; i < 2; i ++ )
    {
      nBuf1[i*2]  = nBuf[i]&0x0f;
      nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }
      if((pDataTable->nPFlag&0x08)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[0].nFrac0, 3, 0))))               //判断方向
    {
      Data[25] += 0x02;
      Data[26] += 0x01;
    }
    
  Long2Buf((INT8U*)&nTemBuf, nDI, 4);    
   for(i = 0; i < 8; i ++)
    {
      nBuf2[i*2]  = nTemBuf[i]&0x0f;
      nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
      Data[15-i*2] += DataAll[nBuf2[i] *2];
      Data[14-i*2] += DataAll[nBuf2[i] *2 + 1];
    }
     Data[17] = DataAll[nBuf1[0] *2 ];
     Data[16] = DataAll[nBuf1[0] *2 + 1];     
     Data[21] += DataAll[nBuf1[1] *2] + 0x01;
     Data[22] += DataAll[nBuf1[1] *2 + 1];
     
     Data[23] = Data_P[nBuf1[2] *2];
     Data[24] = Data_P[nBuf1[2] *2 + 1];
     Data[25] += DataAll[nBuf1[3] *2];
     Data[26] += DataAll[nBuf1[3] *2 + 1];
     Data[27] = 0x06;

     BULCDWrite1(0, Data, 35);
}
//第 80 屏   A 相功率因数 
void  LcdDisplayPage80(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[2];
    INT8U       nBuf1[10]; 
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x02070100 ;
    INT32U      nDI         = 0x20030201;
    MEM_AC_REAL_DATA            sRealData;             //使用全局变量
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    memcpy(Data,Data1,35); 
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    memcpy(nBuf, &pDataTable->sRealInstant.sRealPg[0].nFrac, 2 );
     if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }
    nBuf[1] &= ~0x80;
    for(i = 0; i < 2; i ++ )
    {
      nBuf1[i*2]  = nBuf[i]&0x0f;
      nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }
      if((pDataTable->nPFlag&0x01)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[1].nFrac0, 3, 0))))               //判断方向
    {
      Data[25] += 0x02;
      Data[26] += 0x01;
    }
    
    Long2Buf((INT8U*)&nTemBuf, nDI, 4);    
   for(i = 0; i < 8; i ++)
    {
      nBuf2[i*2]  = nTemBuf[i]&0x0f;
      nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
      Data[15-i*2] += DataAll[nBuf2[i] *2];
      Data[14-i*2] += DataAll[nBuf2[i] *2 + 1];
    }
     Data[17] = Data_P[nBuf1[0] *2];
     Data[16] = Data_P[nBuf1[0] *2 + 1];     
     Data[21] += DataAll[nBuf1[1] *2 ] ;
     Data[22] += DataAll[nBuf1[1] *2 + 1];
     if(nBuf1[3]!= 0)       
      { 
        for(i = 0;i < 2;i ++)
        { Data[23 + i*2] = DataAll[nBuf1[i + 2]*2 ];
          Data[24 + i*2] = DataAll[nBuf1[i + 2]*2 + 1];
        }
      }
     else if(nBuf1[2]!= 0)       
      { 
         Data[23] = DataAll[nBuf1[2]*2 ];
         Data[24] = DataAll[nBuf1[2]*2 +1 ];
      }
     //Data[23] = DataAll[nBuf1[2] *2];
     //Data[24] = DataAll[nBuf1[2] *2 + 1];
     //Data[25] += DataAll[nBuf1[3] *2 ];
     //Data[26] += DataAll[nBuf1[3] *2 + 1];
 
     Data[27] = 0x02;
     Data[28] = 0x02;

     BULCDWrite1(0, Data, 35);
}
//第 81 屏   B 相功率因数 
void  LcdDisplayPage81(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[2];
    INT8U       nBuf1[10] ;
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x02070200 ;
    INT32U      nDI         = 0x20030202;
    MEM_AC_REAL_DATA            sRealData;             //使用全局变量
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    memset(nBuf, 0, sizeof(nBuf));
    memcpy(Data,Data1,35); 
    if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType))//三相四线与三相三线区分
    {
        MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
        memcpy(nBuf, &pDataTable->sRealInstant.sRealPg[1].nFrac, 2 );
    }
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }
    nBuf[1] &= ~0x80;
    for(i = 0; i < 2; i ++ )
    {
      nBuf1[i*2]  = nBuf[i]&0x0f;
      nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }
     if((pDataTable->nPFlag&0x02)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[2].nFrac0, 3, 0))))               //判断方向
    {
     Data[25] += 0x02;
     Data[26] += 0x01;
    }
    
  Long2Buf((INT8U*)&nTemBuf, nDI, 4);    
   for(i = 0; i < 8; i ++)
    {
      nBuf2[i*2]  = nTemBuf[i]&0x0f;
      nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
      Data[15-i*2] += DataAll[nBuf2[i] *2];
      Data[14-i*2] += DataAll[nBuf2[i] *2 + 1];
    }
     Data[17] = Data_P[nBuf1[0] *2 ];
     Data[16] = Data_P[nBuf1[0] *2 + 1];     
     Data[21] += DataAll[nBuf1[1] *2 ];
     Data[22] += DataAll[nBuf1[1] *2 + 1];
     if(nBuf1[3]!= 0)       
      { 
        for(i = 0;i < 2;i ++)
        { Data[23 + i*2] = DataAll[nBuf1[i + 2]*2 ];
          Data[24 + i*2] = DataAll[nBuf1[i + 2]*2 + 1];
        }
      }
     else if(nBuf1[2]!= 0)       
      { 
         Data[23] = DataAll[nBuf1[2]*2 ];
         Data[24] = DataAll[nBuf1[2]*2 +1 ];
      }
     //Data[23] = DataAll[nBuf1[2] *2];
     //Data[24] = DataAll[nBuf1[2] *2 + 1];
     //Data[25] += DataAll[nBuf1[3] *2];
     //Data[26] += DataAll[nBuf1[3] *2 + 1];
   
     Data[27] = 0x02;
     Data[28] = 0x04;

     BULCDWrite1(0, Data, 35);
}
//第 82 屏   C 相功率因数 
void  LcdDisplayPage82(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[2];
    INT8U       nBuf1[10]; 
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x02070300 ;
    INT32U      nDI         = 0x20030203;
    MEM_AC_REAL_DATA            sRealData;             //使用全局变量
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    memcpy(Data,Data1,35); 
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    memcpy(nBuf, &pDataTable->sRealInstant.sRealPg[2].nFrac, 2 );
     if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    } 
    
    nBuf[1] &= ~0x80;
    
    for(i = 0; i < 2; i ++ )
    {
      nBuf1[i*2]  = nBuf[i]&0x0f;
      nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }
    
    if((pDataTable->nPFlag&0x04)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[3].nFrac0, 3, 0))))               //判断方向
    {
      Data[25] += 0x02;
      Data[26] += 0x01;
    }
    
   Long2Buf((INT8U*)&nTemBuf, nDI, 4);    
   for(i = 0; i < 8; i ++)
    {
      nBuf2[i*2]  = nTemBuf[i]&0x0f;
      nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
      Data[15-i*2] += DataAll[nBuf2[i] *2];
      Data[14-i*2] += DataAll[nBuf2[i] *2 + 1];
    }
     Data[17] = Data_P[nBuf1[0] *2];
     Data[16] = Data_P[nBuf1[0] *2 + 1];     
     Data[21] += DataAll[nBuf1[1] *2] ;
     Data[22] += DataAll[nBuf1[1] *2 + 1];
     if(nBuf1[3]!= 0)       
      { 
        for(i = 0;i < 2;i ++)
        { Data[23 + i*2] = DataAll[nBuf1[i + 2]*2 ];
          Data[24 + i*2] = DataAll[nBuf1[i + 2]*2 + 1];
        }
      }
     else if(nBuf1[2]!= 0)       
      { 
         Data[23] = DataAll[nBuf1[2]*2 ];
         Data[24] = DataAll[nBuf1[2]*2 +1 ];
      }
     //Data[23] = DataAll[nBuf1[2] *2];
     //Data[24] = DataAll[nBuf1[2] *2 + 1];

     //Data[25] += DataAll[nBuf1[3] *2];
     //Data[26] += DataAll[nBuf1[3] *2 + 1];
     Data[27] = 0x02;
     Data[28] = 0x08;

     BULCDWrite1(0, Data, 35);
}

//第 83 屏  上 2月 正 向有功总 电量
void  LcdDisplayPage83(void)                  //每秒刷新一次
{    
    INT8U       Data[35];  
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x00010001;
    INT32U      nDI         = 0x50050202;
    MT_DGN_ENERGY_DATA	sAcEn;
    MEM_AC_DJ_DATA     sAcHistData;
      
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));

    GetLastMonData(&sAcHistData,1);
    memcpy(&sAcEn, &sAcHistData.sAcEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcEn.sPPEnergy[0], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data); 

    Data[21] += 0x01;
    Data[24] += 0x0f; 
    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01; 
    Data[25] += 0x06;                  //正向
    Data[28] += 0x01;                  //上 1月
    Data[29] += 0x0D;                  //1 0x06  
    Data[30] += 0x06;
    Data[31] += 0x02; 
     
    BULCDWrite1(0, Data, 35);
}

//第 84屏  上 2月反向有功总电能   
void  LcdDisplayPage84(void)                  //每秒刷新一次
{    
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x00020001;
    INT32U      nDI         = 0x50050202;
    MEM_AC_DJ_DATA     sAcHistData;
    MT_DGN_ENERGY_DATA	sAcEn;
    
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));

    GetLastMonData(&sAcHistData,1);
    memcpy(&sAcEn, &sAcHistData.sAcEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcEn.sNPEnergy[0], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data);
    Data[21] += 0x01;
    Data[24] += 0x0f;
    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01; 
    Data[25] += 0x02;                    //反向      
    Data[26] += 0x01;
    Data[28] += 0x01;                  //上 1月
    Data[29] += 0x0D;                  
    Data[30] += 0x06;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}

//第 85屏  上 3月 正 向有功总 电量
void  LcdDisplayPage85(void)                  //每秒刷新一次
{    
    INT8U       Data[35];  
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x00010001;
    INT32U      nDI         = 0x50050203;
    MT_DGN_ENERGY_DATA	sAcEn;
    MEM_AC_DJ_DATA     sAcHistData;
      
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));

    GetLastMonData(&sAcHistData,2);
    memcpy(&sAcEn, &sAcHistData.sAcEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcEn.sPPEnergy[0], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data); 

    Data[21] += 0x01;
    Data[24] += 0x0f; 
    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01; 
    Data[25] += 0x06;                  //正向
    Data[28] += 0x01;                  //上 2月
    Data[29] += 0x0F;                   
    Data[30] += 0x04;
    Data[31] += 0x02; 
     
    BULCDWrite1(0, Data, 35);
}

//第 86屏  上 3月反向有功总电能   
void  LcdDisplayPage86(void)                  //每秒刷新一次
{    
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x00020001;
    INT32U      nDI         = 0x50050203;
    MEM_AC_DJ_DATA     sAcHistData;
    MT_DGN_ENERGY_DATA	sAcEn;
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));

    GetLastMonData(&sAcHistData,2);
    memcpy(&sAcEn, &sAcHistData.sAcEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcEn.sNPEnergy[0], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data);
    Data[21] += 0x01;
    Data[24] += 0x0f;
    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01; 
    Data[25] += 0x02;                    //反向      
    Data[26] += 0x01;
    Data[28] += 0x01;                  //上 2月
    Data[29] += 0x0F;                  
    Data[30] += 0x04;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}

//第 87屏  上 4月 正 向有功总 电量
void  LcdDisplayPage87(void)                  //每秒刷新一次
{    
    INT8U       Data[35];  
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x00010001;
    INT32U      nDI         = 0x50050204;
    MT_DGN_ENERGY_DATA	sAcEn;
    MEM_AC_DJ_DATA     sAcHistData;
      
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));

    GetLastMonData(&sAcHistData,3);
    memcpy(&sAcEn, &sAcHistData.sAcEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcEn.sPPEnergy[0], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data); 

    Data[21] += 0x01;
    Data[24] += 0x0f; 
    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01; 
    Data[25] += 0x06;                  //正向
    Data[28] += 0x01;                  //上 3月
    Data[29] += 0x06;                  //  
    Data[30] += 0x0C;
    Data[31] += 0x02; 
     
    BULCDWrite1(0, Data, 35);
}

//第 88屏  上 4月反向有功总电能   
void  LcdDisplayPage88(void)                  //每秒刷新一次
{    
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x00020001;
    INT32U      nDI         = 0x50050204;
    MEM_AC_DJ_DATA     sAcHistData;
    MT_DGN_ENERGY_DATA	sAcEn;
    
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));

    GetLastMonData(&sAcHistData,3);
    memcpy(&sAcEn, &sAcHistData.sAcEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcEn.sNPEnergy[0], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data);
    Data[21] += 0x01;
    Data[24] += 0x0f;
    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01; 
    Data[25] += 0x02;                    //反向      
    Data[26] += 0x01;
    Data[28] += 0x01;                  //上 3月
    Data[29] += 0x06;                  //  
    Data[30] += 0x0C;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}

//第 89屏  上 5月 正 向有功总 电量
void  LcdDisplayPage89(void)                  //每秒刷新一次
{    
    INT8U       Data[35];  
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x00010001;
    INT32U      nDI         = 0x50050205;
    MT_DGN_ENERGY_DATA	sAcEn;
    MEM_AC_DJ_DATA     sAcHistData;
      
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));

    GetLastMonData(&sAcHistData,4);
    memcpy(&sAcEn, &sAcHistData.sAcEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcEn.sPPEnergy[0], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data); 

    Data[21] += 0x01;
    Data[24] += 0x0f; 
    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01; 
    Data[25] += 0x06;                  //正向
    Data[28] += 0x01;                  //上 3月
    Data[29] += 0x0B;                  //  
    Data[30] += 0x0C;
    Data[31] += 0x02; 
     
    BULCDWrite1(0, Data, 35);
}

//第 90屏  上 5月反向有功总电能   
void  LcdDisplayPage90(void)                  //每秒刷新一次
{    
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x00020001;
    INT32U      nDI         = 0x50050205;
    MEM_AC_DJ_DATA     sAcHistData;
    MT_DGN_ENERGY_DATA	sAcEn;
    
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));
    GetLastMonData(&sAcHistData,4);

    memcpy(&sAcEn, &sAcHistData.sAcEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcEn.sNPEnergy[0], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data);
    Data[21] += 0x01;
    Data[24] += 0x0f;
    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01; 
    Data[25] += 0x02;                    //反向      
    Data[26] += 0x01;
    Data[28] += 0x01;                  //上 3月
    Data[29] += 0x0B;                  //  
    Data[30] += 0x0C;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}

//第 91屏  上 6月 正 向有功总 电量
void  LcdDisplayPage91(void)                  //每秒刷新一次
{    
    INT8U       Data[35];  
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x00010001;
    INT32U      nDI         = 0x50050206;
    MT_DGN_ENERGY_DATA	sAcEn;
    MEM_AC_DJ_DATA     sAcHistData;
      
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));
    
    GetLastMonData(&sAcHistData,5);
    memcpy(&sAcEn, &sAcHistData.sAcEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcEn.sPPEnergy[0], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data); 

    Data[21] += 0x01;
    Data[24] += 0x0f; 
    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01; 
    Data[25] += 0x06;                  //正向
    Data[28] += 0x01;                  //上 3月
    Data[29] += 0x0B;                  //  
    Data[30] += 0x0E;
    Data[31] += 0x02; 
     
    BULCDWrite1(0, Data, 35);
}

//第 92屏  上 6月反向有功总电能   
void  LcdDisplayPage92(void)                  //每秒刷新一次
{    
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x00020001;
    INT32U      nDI         = 0x50050206;
    MEM_AC_DJ_DATA     sAcHistData;
    MT_DGN_ENERGY_DATA	sAcEn;
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));
    
    GetLastMonData(&sAcHistData,5);
    memcpy(&sAcEn, &sAcHistData.sAcEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcEn.sNPEnergy[0], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data);
    Data[21] += 0x01;
    Data[24] += 0x0f;
    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01; 
    Data[25] += 0x02;                    //反向      
    Data[26] += 0x01;
    Data[28] += 0x01;                  //上 3月
    Data[29] += 0x0B;                  //  
    Data[30] += 0x0E;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}


//第 93屏  上 7月 正 向有功总 电量
void  LcdDisplayPage93(void)                  //每秒刷新一次
{    
    INT8U       Data[35];  
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x00010001;
    INT32U      nDI         = 0x50050207;
    MT_DGN_ENERGY_DATA	sAcEn;
    MEM_AC_DJ_DATA     sAcHistData;
      
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));
    
    GetLastMonData(&sAcHistData,6);
    memcpy(&sAcEn, &sAcHistData.sAcEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcEn.sPPEnergy[0], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data); 

    Data[21] += 0x01;
    Data[24] += 0x0f; 
    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01; 
    Data[25] += 0x06;                  //正向
    Data[28] += 0x01;                  //上 3月
    Data[29] += 0x0E;                  //  
    //Data[30] += 0x0E;
    Data[31] += 0x02; 
     
    BULCDWrite1(0, Data, 35);
}

//第 94屏  上 7月反向有功总电能   
void  LcdDisplayPage94(void)                  //每秒刷新一次
{    
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x00020001;
    INT32U      nDI         = 0x50050207;
    MEM_AC_DJ_DATA     sAcHistData;
    MT_DGN_ENERGY_DATA	sAcEn;
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));
    
    GetLastMonData(&sAcHistData,6);
    memcpy(&sAcEn, &sAcHistData.sAcEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcEn.sNPEnergy[0], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data);
    Data[21] += 0x01;
    Data[24] += 0x0f;
    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01; 
    Data[25] += 0x02;                    //反向      
    Data[26] += 0x01;
    Data[28] += 0x01;                  //上 3月
    Data[29] += 0x0E;                  //  
    //Data[30] += 0x0E;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}

//第 95屏  上 8月 正 向有功总 电量
void  LcdDisplayPage95(void)                  //每秒刷新一次
{    
    INT8U       Data[35];  
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x00010001;
    INT32U      nDI         = 0x50050208;
    MT_DGN_ENERGY_DATA	sAcEn;
    MEM_AC_DJ_DATA     sAcHistData;
      
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));
    
    GetLastMonData(&sAcHistData,7);
    memcpy(&sAcEn, &sAcHistData.sAcEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcEn.sPPEnergy[0], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data); 

    Data[21] += 0x01;
    Data[24] += 0x0f; 
    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01; 
    Data[25] += 0x06;                  //正向
    Data[28] += 0x01;                  //上 3月
    Data[29] += 0x0F;                  //  
    Data[30] += 0x0E;
    Data[31] += 0x02; 
     
    BULCDWrite1(0, Data, 35);
}

//第 96屏  上 8月反向有功总电能   
void  LcdDisplayPage96(void)                  //每秒刷新一次
{    
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x00020001;
    INT32U      nDI         = 0x50050208;
    MEM_AC_DJ_DATA     sAcHistData;
    MT_DGN_ENERGY_DATA	sAcEn;
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));
    
    GetLastMonData(&sAcHistData,7);
    memcpy(&sAcEn, &sAcHistData.sAcEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcEn.sNPEnergy[0], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data);
    Data[21] += 0x01;
    Data[24] += 0x0f;
    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01; 
    Data[25] += 0x02;                    //反向      
    Data[26] += 0x01;
    Data[28] += 0x01;                  //上 3月
    Data[29] += 0x0F;                  //  
    Data[30] += 0x0E;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}

//第 97屏  上 9月 正 向有功总 电量
void  LcdDisplayPage97(void)                  //每秒刷新一次
{    
    INT8U       Data[35];  
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x00010001;
    INT32U      nDI         = 0x50050209;
    MT_DGN_ENERGY_DATA	sAcEn;
    MEM_AC_DJ_DATA     sAcHistData;
      
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));
    
    GetLastMonData(&sAcHistData,8);
    memcpy(&sAcEn, &sAcHistData.sAcEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcEn.sPPEnergy[0], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data); 

    Data[21] += 0x01;
    Data[24] += 0x0f; 
    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01; 
    Data[25] += 0x06;                  //正向
    Data[28] += 0x01;                  //上 3月
    Data[29] += 0x0F;                  //  
    Data[30] += 0x0C;
    Data[31] += 0x02; 
     
    BULCDWrite1(0, Data, 35);
}

//第 98屏  上 9月反向有功总电能   
void  LcdDisplayPage98(void)                  //每秒刷新一次
{    
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x00020001;
    INT32U      nDI         = 0x50050209;
    MEM_AC_DJ_DATA     sAcHistData;
    MT_DGN_ENERGY_DATA	sAcEn;
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));
    
    GetLastMonData(&sAcHistData,8);
    memcpy(&sAcEn, &sAcHistData.sAcEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcEn.sNPEnergy[0], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data);
    Data[21] += 0x01;
    Data[24] += 0x0f;
    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01; 
    Data[25] += 0x02;                    //反向      
    Data[26] += 0x01;
    Data[28] += 0x01;                  //上 3月
    Data[29] += 0x0F;                  //  
    Data[30] += 0x0C;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}

//第 99屏  上 10月 正 向有功总 电量
void  LcdDisplayPage99(void)                  //每秒刷新一次
{    
    INT8U       Data[35];  
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x00010001;
    INT32U      nDI         = 0x5005020A;
    MT_DGN_ENERGY_DATA	sAcEn;
    MEM_AC_DJ_DATA     sAcHistData;
      
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));
    
    GetLastMonData(&sAcHistData,9);
    memcpy(&sAcEn, &sAcHistData.sAcEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcEn.sPPEnergy[0], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data); 

    Data[21] += 0x01;
    Data[24] += 0x0f; 
    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01; 
    Data[25] += 0x06;                  //正向
    Data[28] += 0x01;                  //上 3月
    Data[29] += 0x0F;                  //  
    Data[30] += 0x0B;
    Data[31] += 0x02; 
     
    BULCDWrite1(0, Data, 35);
}

//第100屏  上 10月反向有功总电能   
void  LcdDisplayPage100(void)                  //每秒刷新一次
{    
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x00020001;
    INT32U      nDI         = 0x5005020A;
    MEM_AC_DJ_DATA     sAcHistData;
    MT_DGN_ENERGY_DATA	sAcEn;
    
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));
    
    GetLastMonData(&sAcHistData,9);
    memcpy(&sAcEn, &sAcHistData.sAcEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcEn.sNPEnergy[0], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data);
    Data[21] += 0x01;
    Data[24] += 0x0f;
    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01; 
    Data[25] += 0x02;                    //反向      
    Data[26] += 0x01;
    Data[28] += 0x01;                  //上 3月
    Data[29] += 0x0F;                  //  
    Data[30] += 0x0B;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}

//第101屏  上 11月 正 向有功总 电量
void  LcdDisplayPage101(void)                  //每秒刷新一次
{    
    INT8U       Data[35];  
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x00010001;
    INT32U      nDI         = 0x5005020B;
    MT_DGN_ENERGY_DATA	sAcEn;
    MEM_AC_DJ_DATA     sAcHistData;
      
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));
    
    GetLastMonData(&sAcHistData,10);
    memcpy(&sAcEn, &sAcHistData.sAcEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcEn.sPPEnergy[0], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data); 

    Data[21] += 0x01;
    Data[24] += 0x0f; 
    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01; 
    Data[25] += 0x06;                  //正向
    Data[28] += 0x01;                  //上 3月
    Data[29] += 0x06;                  //  
    Data[30] += 0x01;
    Data[31] += 0x02; 
     
    BULCDWrite1(0, Data, 35);
}

//第102屏  上 11月反向有功总电能   
void  LcdDisplayPage102(void)                  //每秒刷新一次
{    
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x00020001;
    INT32U      nDI         = 0x5005020B;
    MEM_AC_DJ_DATA     sAcHistData;
    MT_DGN_ENERGY_DATA	sAcEn;
    
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));
    
    GetLastMonData(&sAcHistData,10);
    memcpy(&sAcEn, &sAcHistData.sAcEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcEn.sNPEnergy[0], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data);
    Data[21] += 0x01;
    Data[24] += 0x0f;
    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01; 
    Data[25] += 0x02;                    //反向      
    Data[26] += 0x01;
    Data[28] += 0x01;                  //上 3月
    Data[29] += 0x06;                  //  
    Data[30] += 0x01;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}

//第103屏  上 12月 正 向有功总 电量
void  LcdDisplayPage103(void)                  //每秒刷新一次
{    
    INT8U       Data[35];  
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x00010001;
    INT32U      nDI         = 0x5005020C;
    MT_DGN_ENERGY_DATA	sAcEn;
    MEM_AC_DJ_DATA     sAcHistData;
      
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));
    
    GetLastMonData(&sAcHistData,11);
    memcpy(&sAcEn, &sAcHistData.sAcEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcEn.sPPEnergy[0], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data); 

    Data[21] += 0x01;
    Data[24] += 0x0f; 
    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01; 
    Data[25] += 0x06;                  //正向
    Data[28] += 0x01;                  //上 3月
    Data[29] += 0x0D;                  //  
    Data[30] += 0x07;
    Data[31] += 0x02; 
     
    BULCDWrite1(0, Data, 35);
}

//第104屏  上 12月反向有功总电能   
void  LcdDisplayPage104(void)                  //每秒刷新一次
{    
    INT8U       Data[35];
    INT8U       nBuf[4];
    //INT32U      nDI         = 0x00020001;
    INT32U      nDI         = 0x5005020C;
    MEM_AC_DJ_DATA     sAcHistData;
    MT_DGN_ENERGY_DATA	sAcEn;
    
    memcpy(Data,Data1,35); 
    memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcHistData, 0, sizeof(MEM_AC_DJ_DATA));
    
    GetLastMonData(&sAcHistData,11);
    memcpy(&sAcEn, &sAcHistData.sAcEnergy, sizeof(sAcEn));
    memcpy(nBuf, &sAcEn.sNPEnergy[0], sizeof(FORMAT11));

    DataDeal(nBuf ,nDI, Data);
    Data[21] += 0x01;
    Data[24] += 0x0f;
    Data[12] += 0x02;
    Data[14] += 0x01;
    Data[15] += 0x01; 
    Data[25] += 0x02;                    //反向      
    Data[26] += 0x01;
    Data[28] += 0x01;                  //上 12月
    Data[29] += 0x0D;                  //  
    Data[30] += 0x07;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}

//第 105 屏   时钟电池总时间
void  LcdDisplayPage105(void)                  
{    
    INT8U       i ;
    INT8U       j ;
    INT8U       Data[35];
    INT8U       nBuf1[10];
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    INT8U       nBuf[4];
    INT32U      nTmp32U    = 0;
    //INT32U      nDI         = 0x10000002;
    INT32U      nDI         = 0x20130200;

    memcpy(Data,Data1,35); 
    
    MemReadAcXBackData(F39, &nTmp32U, sizeof(nTmp32U));

    INT32U2BCD(nTmp32U, 8,&g_nAcEventBuf[0]);
    memcpy(nBuf, &g_nAcEventBuf[0], 4);

    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
        memset(nBuf, 0, sizeof(nBuf));
    }
    for(i = 0; i < 4; i ++ )
    {
        nBuf1[i*2]  = nBuf[i]&0x0f;
        nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }
    Long2Buf((INT8U*)&nTemBuf, nDI, 4);    
    for(i = 0; i < 8; i ++)
    { 
        nBuf2[i*2]  = nTemBuf[i]&0x0f;
        nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
        Data[15-i*2] += DataAll[nBuf2[i] *2];
        Data[14-i*2] += DataAll[nBuf2[i] *2 + 1];
    }           
    Data[17] = DataAll[nBuf1[0] *2 ] + 0x08;
    Data[16] = DataAll[nBuf1[0] *2 +1] + 0x08;

    for(j = 6;j > 0;j--)
    {
        if(nBuf1[j]!= 0)
        {
            for(i = 0; i < j; i ++)
            {   
                Data[21 + i*2] += DataAll[nBuf1[i + 1] *2];
                Data[22 + i*2] += DataAll[nBuf1[i + 1] *2 + 1];
            } 
            j = 1;
        }
    }

    //Data[21] += 0x01;
    //Data[22] += 0x04; 
    BULCDWrite1(0, Data, 35);     
}

//第 106屏  最近一次编程 日期
void  LcdDisplayPage106(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf1[6];
    INT8U   nTmpTime[6];
    //INT32U      nDI         = 0x10000101;
    INT32U      nDI         = 0x30120201;

    memcpy(Data,Data1,35);    

    MemReadAcXBackData(F40, &nTmpTime[0], sizeof(nTmpTime));
    for(i = 0; i < 3; i ++ )
    {
        nBuf1[i*2]  = nTmpTime[i +3]&0x0f;
        nBuf1[i*2+1]  = nTmpTime[i +3]>>0x04;
    }

    Time1Deal( &nBuf1[0], nDI, Data);
    Data[17] += 0x08;
    Data[16] += 0x08;
    //Data[21] += 0x01;
    //Data[22] += 0x04;
    Data[29] += 0x06;
    Data[31] += 0x02;
    Data[2] &= 0xF0;
    Data[3] &= 0xF6;
    BULCDWrite1(0, Data, 35);     
}
//第 107屏 最近一次编程时间
void  LcdDisplayPage107(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;
    INT8U       Data[35];
    INT8U       nBuf1[6];
    INT8U       nTmpTime[6];
    //INT32U      nDI         = 0x10000101;
    INT32U      nDI         = 0x30120201;

    memcpy(Data,Data1,35); 
    
    MemReadAcXBackData(F40, &nTmpTime[0], sizeof(nTmpTime));

    for(i = 0; i < 3; i ++ )
    {
        nBuf1[i*2]  = nTmpTime[i]&0x0f;
        nBuf1[i*2+1]  = nTmpTime[i]>>0x04;
    }

    Time2Deal( nBuf1, nDI, Data);
    Data[17] += 0x08;
    Data[16] += 0x08;  
    //Data[21] += 0x01;
    //Data[22] += 0x04;
    Data[29] += 0x06;
    Data[31] += 0x02;
    Data[2] += 0x04;
    Data[2] &= 0xF6;
    Data[3] &= 0xFD;

    BULCDWrite1(0, Data, 35);     
}

//第 108屏   A总失压累计时间
void  LcdDisplayPage108(void)                  
{    
    INT8U       i ;
    INT8U       j ;
    INT8U       Data[35];
    INT8U       nBuf1[10];
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    INT8U       nBuf[3];
    INT32U      nTmp32U    = 0;
    //INT32U      nDI         = 0x10000002;
    INT32U      nDI         = 0x30000A02;

    memcpy(Data,Data1,35); 
    MemReadAcXBackData(F2, &nTmp32U, sizeof(nTmp32U));
    nTmp32U = (nTmp32U / 60);
    INT32U2BCD(nTmp32U, 6,&g_nAcEventBuf[0]);
    memcpy(nBuf, &g_nAcEventBuf[0], 3);

    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
        memset(nBuf, 0, sizeof(nBuf));
    }
    for(i = 0; i < 3; i ++ )
    {
        nBuf1[i*2]  = nBuf[i]&0x0f;
        nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }
    Long2Buf((INT8U*)&nTemBuf, nDI, 4);    
    for(i = 0; i < 8; i ++)
    { 
        nBuf2[i*2]  = nTemBuf[i]&0x0f;
        nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
        Data[15-i*2] += DataAll[nBuf2[i] *2];
        Data[14-i*2] += DataAll[nBuf2[i] *2 + 1];
    }           
    Data[17] = DataAll[nBuf1[0] *2 ] + 0x08;
    Data[16] = DataAll[nBuf1[0] *2 +1] + 0x08;

    for(j = 5;j > 0;j--)
    {
        if(nBuf1[j]!= 0)
        {
            for(i = 0; i < j; i ++)
            {   
                Data[21 + i*2] += DataAll[nBuf1[i + 1] *2];
                Data[22 + i*2] += DataAll[nBuf1[i + 1] *2 + 1];
            } 
            j = 1;
        }
    }

    Data[21] += 0x09;
    Data[22] += 0x04; 
    Data[28] |= 0x02;
    BULCDWrite1(0, Data, 35);     
}

//第 108屏   B总失压累计时间
void  LcdDisplayPage109(void)                  
{    
    INT8U       i ;
    INT8U       j ;
    INT8U       Data[35];
    INT8U       nBuf1[10];
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    INT8U       nBuf[3];
    INT32U      nTmp32U    = 0;
    //INT32U      nDI         = 0x10000002;
    INT32U      nDI         = 0x30000A03;

    memcpy(Data,Data1,35); 
    MemReadAcXBackData(F3, &nTmp32U, sizeof(nTmp32U));
    nTmp32U = (nTmp32U / 60);
    INT32U2BCD(nTmp32U, 6,&g_nAcEventBuf[0]);
    memcpy(nBuf, &g_nAcEventBuf[0], 3);

    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
        memset(nBuf, 0, sizeof(nBuf));
    }
    for(i = 0; i < 3; i ++ )
    {
        nBuf1[i*2]  = nBuf[i]&0x0f;
        nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }
    Long2Buf((INT8U*)&nTemBuf, nDI, 4);    
    for(i = 0; i < 8; i ++)
    { 
        nBuf2[i*2]  = nTemBuf[i]&0x0f;
        nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
        Data[15-i*2] += DataAll[nBuf2[i] *2];
        Data[14-i*2] += DataAll[nBuf2[i] *2 + 1];
    }           
    Data[17] = DataAll[nBuf1[0] *2 ] + 0x08;
    Data[16] = DataAll[nBuf1[0] *2 +1] + 0x08;

    for(j = 5;j > 0;j--)
    {
        if(nBuf1[j]!= 0)
        {
            for(i = 0; i < j; i ++)
            {   
                Data[21 + i*2] += DataAll[nBuf1[i + 1] *2];
                Data[22 + i*2] += DataAll[nBuf1[i + 1] *2 + 1];
            } 
            j = 1;
        }
    }

    Data[21] += 0x09;
    Data[22] += 0x04; 
    Data[28] |= 0x04;
    BULCDWrite1(0, Data, 35);     
}

//第 110屏   C总失压累计时间
void  LcdDisplayPage110(void)                  
{    
    INT8U       i ;
    INT8U       j ;
    INT8U       Data[35];
    INT8U       nBuf1[10];
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    INT8U       nBuf[3];
    INT32U      nTmp32U    = 0;
    //INT32U      nDI         = 0x10000002;
    INT32U      nDI         = 0x30000A04;

    memcpy(Data,Data1,35); 
    MemReadAcXBackData(F4, &nTmp32U, sizeof(nTmp32U));
    nTmp32U = (nTmp32U / 60);
    INT32U2BCD(nTmp32U, 6,&g_nAcEventBuf[0]);
    memcpy(nBuf, &g_nAcEventBuf[0], 3);

    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
        memset(nBuf, 0, sizeof(nBuf));
    }
    for(i = 0; i < 3; i ++ )
    {
        nBuf1[i*2]  = nBuf[i]&0x0f;
        nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }
    Long2Buf((INT8U*)&nTemBuf, nDI, 4);    
    for(i = 0; i < 8; i ++)
    { 
        nBuf2[i*2]  = nTemBuf[i]&0x0f;
        nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
        Data[15-i*2] += DataAll[nBuf2[i] *2];
        Data[14-i*2] += DataAll[nBuf2[i] *2 + 1];
    }           
    Data[17] = DataAll[nBuf1[0] *2 ] + 0x08;
    Data[16] = DataAll[nBuf1[0] *2 +1] + 0x08;

    for(j = 5;j > 0;j--)
    {
        if(nBuf1[j]!= 0)
        {
            for(i = 0; i < j; i ++)
            {   
                Data[21 + i*2] += DataAll[nBuf1[i + 1] *2];
                Data[22 + i*2] += DataAll[nBuf1[i + 1] *2 + 1];
            } 
            j = 1;
        }
    }

    Data[21] += 0x09;
    Data[22] += 0x04; 
    Data[28] |= 0x08;
    BULCDWrite1(0, Data, 35);     
}

//第 111屏   零线电流
void  LcdDisplayPage111(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[3];
    INT8U       nBuf1[10];
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x02020300;
    INT32U      nDI         = 0x20010400;
    MEM_AC_REAL_DATA            sRealData;             //使用全局变量
    pMEM_AC_REAL_DATA           pRealData = &sRealData;

    memcpy(Data,Data1,35); 
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    memcpy(nBuf, &pDataTable->sRealInstant.sRealCurr[2].nD1, 3);
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
        memset(nBuf, 0, sizeof(nBuf));
    }
    memset(nBuf, 0, sizeof(nBuf));

    nBuf[2] &= ~0x80;

    for(i = 0; i < 3; i ++ )
    {
        nBuf1[i*2]  = nBuf[i]&0x0f;
        nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }
    Long2Buf((INT8U*)&nTemBuf, nDI, 4);    
    for(i = 0; i < 8; i ++)
    { 
        nBuf2[i*2]  = nTemBuf[i]&0x0f;
        nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
        Data[15-i*2] |= DataAll[nBuf2[i] *2];
        Data[14-i*2] |= DataAll[nBuf2[i] *2 + 1];
    }           
    Data[17] = DataAll[nBuf1[0] *2 ];
    Data[16] = DataAll[nBuf1[0] *2 + 1];     
    Data[21] |= DataAll[nBuf1[1] *2 ] + 0x04;
    Data[22] |= DataAll[nBuf1[1] *2 + 1];

    Data[23] = Data_P[nBuf1[2] *2];
    Data[24] = Data_P[nBuf1[2] *2 + 1] +0x08;
    Data[25] |= DataAll[nBuf1[3] *2 ];
    Data[26] |= DataAll[nBuf1[3] *2 + 1];
    if(nBuf1[5]!= 0)       
    { 
        for(i = 0;i < 2;i ++)
        {   
            Data[27 + i*2] = DataAll[nBuf1[i + 4]* 2 ];
            Data[28 + i*2] = DataAll[nBuf1[i + 4]* 2 + 1];
        }
    }
    else if(nBuf1[4]!= 0)       
    { 
        Data[27] = DataAll[nBuf1[4] *2];
        Data[28] = DataAll[nBuf1[4] *2 +1 ];
    }
    Data[13] |= 0x02; 
    Data[27] |= 0x08;
    BULCDWrite1(0, Data, 35);
}

//第 112屏   零序电流
void  LcdDisplayPage112(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[3];
    INT8U       nBuf1[10];
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x02020300;
    INT32U      nDI         = 0x20010600;
    MEM_AC_REAL_DATA            sRealData;             //使用全局变量
    pMEM_AC_REAL_DATA           pRealData = &sRealData;

    memcpy(Data,Data1,35); 
    if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType))//三相四线与三相三线区分
    {
        MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
        memcpy(nBuf, &pDataTable->sRealInstant.sRealCurr[1].nD1, 3);
    }

    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
        memset(nBuf, 0, sizeof(nBuf));
    }

    nBuf[2] &= ~0x80;

    for(i = 0; i < 3; i ++ )
    {
        nBuf1[i*2]  = nBuf[i]&0x0f;
        nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }
    Long2Buf((INT8U*)&nTemBuf, nDI, 4);    
    for(i = 0; i < 8; i ++)
    { 
        nBuf2[i*2]  = nTemBuf[i]&0x0f;
        nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
        Data[15-i*2] |= DataAll[nBuf2[i] *2];
        Data[14-i*2] |= DataAll[nBuf2[i] *2 + 1];
    }           
    Data[17] = DataAll[nBuf1[0] *2 ];
    Data[16] = DataAll[nBuf1[0] *2 + 1];     
    Data[21] |= DataAll[nBuf1[1] *2 ] + 0x04;
    Data[22] |= DataAll[nBuf1[1] *2 + 1];

    Data[23] = Data_P[nBuf1[2] *2];
    Data[24] = Data_P[nBuf1[2] *2 + 1] +0x08;
    Data[25] |= DataAll[nBuf1[3] *2 ];
    Data[26] |= DataAll[nBuf1[3] *2 + 1];
    if(nBuf1[5]!= 0)       
    { 
        for(i = 0;i < 2;i ++)
        {   
            Data[27 + i*2] = DataAll[nBuf1[i + 4]* 2 ];
            Data[28 + i*2] = DataAll[nBuf1[i + 4]* 2 + 1];
        }
    }
    else if(nBuf1[4]!= 0)       
    { 
        Data[27] = DataAll[nBuf1[4] *2];
        Data[28] = DataAll[nBuf1[4] *2 +1 ];
    }
    Data[13] |= 0x02; 
    Data[27] |= 0x08;
    BULCDWrite1(0, Data, 35);
}

void  LcdDisplayPage113(void)                  //每秒刷新一次
{    
    INT8U       i  = 0;
    INT8U       Data[35] ;
    INT8U       nBuf1[6];
    INT8U       nBuf[3];
    INT8U       nTemBuf[8];
    INT8U       nBuf2[16];
    //INT32U      nDI         = 0x04000101;
    INT32U      nDI         = 0x41160201;  //698
    memcpy(Data,Data1,35);    


    nBuf[i++] = 0;
    nBuf[i++] = 1;

    for(i = 0; i < 2; i ++ )
    {
        nBuf1[i*2]  = nBuf[i]&0x0f;
        nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }
    Long2Buf((INT8U*)&nTemBuf, nDI, 4);
    for(i = 0; i < 8; i ++)
    { 
        nBuf2[i*2]  = nTemBuf[i]&0x0f;
        nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
        Data[15-i*2] += DataAll[nBuf2[i] *2];
        Data[14-i*2] += DataAll[nBuf2[i] *2 + 1];
    }
    //Time1Deal( nBuf1, nDI, Data);
    Data[17] = DataAll[nBuf1[0] *2];
    Data[16] = DataAll[nBuf1[0] *2 + 1];
    Data[21] += Data_P[nBuf1[1] *2];
    Data[22] += Data_P[nBuf1[1] *2 +1];

    Data[23] += DataAll[nBuf1[2] *2];
    Data[24] += DataAll[nBuf1[2] *2 +1];
    Data[25] += DataAll[nBuf1[3] *2];
    Data[26] += DataAll[nBuf1[3] *2 +1];
    
    //Data[17] += 0x08;
    //Data[16] +=  0x08;
    //Data[31] += 0x01;// 当前

    BULCDWrite1(0, Data, 35);  
}







//智能电表错误
//err01
void Err01()
{
  INT8U Data[35];
  INT8U i     =0;
  for(i=0;i<35;i++)
  {
  Data[i]=0;
  }
  
  Data[17]=0x60;
  Data[21]=0xF0;
  Data[22]=0xA0;
  Data[24]=0x40;
  Data[26]=0x60;
  Data[28]=0x60;
  Data[29]=0x90;
  Data[30]=0xE0;
  BULCDWrite1(0, Data, 35);
}
//err02
void Err02()
{
  INT8U Data[35];
  INT8U i     =0;
  for(i=0;i<35;i++)
  {
  Data[i]=0;
  }
  
  Data[16]=0x60;
  Data[17]=0xD0;
  Data[21]=0xF0;
  Data[22]=0xA0;
  Data[24]=0x40;
  Data[26]=0x60;
  Data[28]=0x60;
  Data[29]=0x90;
  Data[30]=0xE0;
  BULCDWrite1(0, Data, 35);
}
//err03
void Err03()
{
  INT8U Data[35];
  INT8U i     =0;
  for(i=0;i<35;i++)
  {
  Data[i]=0;
  }
  
  Data[16]=0x40;
  Data[17]=0xF0;
  Data[21]=0xF0;
  Data[22]=0xA0;
  Data[24]=0x40;
  Data[26]=0x60;
  Data[28]=0x60;
  Data[29]=0x90;
  Data[30]=0xE0;
  BULCDWrite1(0, Data, 35);
}
//err04
void Err04()
{
  INT8U Data[35];
  INT8U i     =0;
  for(i=0;i<35;i++)
  {
  Data[i]=0;
  }
  
  
  Data[9]=0x08;
  
  Data[17]=0x60;
  Data[16]=0xC0;
  Data[21]=0xF0;
  Data[22]=0xA0;
  Data[24]=0x40;
  Data[26]=0x60;
  Data[28]=0x60;
  Data[29]=0x90;
  Data[30]=0xE0;
  BULCDWrite1(0, Data, 35);
}
//err05
void Err05()
{
  INT8U Data[35];
  INT8U i     =0;
  for(i=0;i<35;i++)
  {
  Data[i]=0;
  }
  
  Data[17]=0xB0;
  Data[16]=0xC0;
  Data[21]=0xF0;
  Data[22]=0xA0;
  Data[24]=0x40;
  Data[26]=0x60;
  Data[28]=0x60;
  Data[29]=0x90;
  Data[30]=0xE0;
  BULCDWrite1(0, Data, 35);
}
//err06
void Err06()
{
  INT8U Data[35];
  INT8U i     =0;
  for(i=0;i<35;i++)
  {
  Data[i]=0;
  }
  
  Data[17]=0xB0;
  Data[16]=0xE0;
  Data[21]=0xF0;
  Data[22]=0xA0;
  Data[24]=0x40;
  Data[26]=0x60;
  Data[28]=0x60;
  Data[29]=0x90;
  Data[30]=0xE0;
  BULCDWrite1(0, Data, 35);
}
//err08
void Err08()
{
  INT8U Data[35];
  INT8U i     =0;
  for(i=0;i<35;i++)
  {
  Data[i]=0;
  }
  
  Data[17]=0xF0;
  Data[16]=0xE0;
  Data[21]=0xF0;
  Data[22]=0xA0;
  Data[24]=0x40;
  Data[26]=0x60;
  Data[28]=0x60;
  Data[29]=0x90;
  Data[30]=0xE0;
  BULCDWrite1(0, Data, 35);
}
//err51
void Err51()
{ 
  INT8U Data[35];
  memcpy(Data,Data1,35); 
  INT8U i=0;
 for(i=0;i<4;i++)
 {
   Data[i]&=0x00;
 }
  Data[17]|=0x60;
  Data[17]-=0x90;
  Data[16]-=0xA0;
  Data[21]+=0xB0;
  Data[22]+=0xC0;
  Data[33]+=0x90;
  Data[34]+=0xE0;
  Data[32]+=0x60;
  Data[30]|=0x60;
  Data[28]+=0x40;
  Data[26]+=0x40;
  
  Data[4]-=0x10;
  Data[8]-=0x10;
  Data[12]-=0x10;
  BULCDWrite1(0, Data, 35);
}
//err52
void Err52()
{
  INT8U Data[35];
  memcpy(Data,Data1,35); 
  INT8U i=0;
 for(i=0;i<4;i++)
 {
   Data[i]&=0x00;
 }
  Data[17]|=0xD0;
  Data[17]-=0x20;
  Data[16]|=0x60;
  Data[16]-=0x80;
  Data[21]+=0xB0;
  Data[22]+=0xC0;
  Data[33]+=0x90;
  Data[34]+=0xE0;
  Data[32]+=0x60;
  Data[30]|=0x60;
  Data[28]+=0x40;
  Data[26]+=0x40;
  
  Data[4]-=0x10;
  Data[8]-=0x10;
  Data[12]-=0x10;
  BULCDWrite1(0, Data, 35);
}
//err53
void Err53()
{
  INT8U Data[35];
  memcpy(Data,Data1,35); 
  INT8U i=0;
  for(i=0;i<4;i++)
  {
   Data[i]&=0x00;
  }
  Data[17]|=0xF0;
  Data[16]|=0x40;
  Data[16]-=0xA0;
  Data[21]+=0xB0;
  Data[22]+=0xC0;
  Data[33]+=0x90;
  Data[34]+=0xE0;
  Data[32]+=0x60;
  Data[30]|=0x60;
  Data[28]+=0x40;
  Data[26]+=0x40;
  
  Data[4]-=0x10;
  Data[8]-=0x10;
  Data[12]-=0x10;
  BULCDWrite1(0, Data, 35);
}
//err54
void Err54()
{
  INT8U Data[35];
  memcpy(Data,Data1,35); 
  INT8U i=0;
  for(i=0;i<4;i++)
  {
   Data[i]&=0x00;
  }
  Data[17]|=0x60;
  Data[17]-=0x90;
  Data[16]|=0xC0;
  Data[16]-=0x20;
  Data[21]+=0xB0;
  Data[22]+=0xC0;
  Data[33]+=0x90;
  Data[34]+=0xE0;
  Data[32]+=0x60;
  Data[30]|=0x60;
  Data[28]+=0x40;
  Data[26]+=0x40;
  
  Data[4]-=0x10;
  Data[8]-=0x10;
  Data[12]-=0x10;
  BULCDWrite1(0, Data, 35);
}
//err55
void Err55()
{
  INT8U Data[35];
  memcpy(Data,Data1,35); 
  INT8U i=0;
  for(i=0;i<4;i++)
  {
   Data[i]&=0x00;
  }
  Data[17]|=0xF0;
  Data[17]-=0x40;
  Data[16]|=0xE0;
  Data[16]-=0x20;
  Data[21]+=0xB0;
  Data[22]+=0xC0;
  Data[33]+=0x90;
  Data[34]+=0xE0;
  Data[32]+=0x60;
  Data[30]|=0x60;
  Data[28]+=0x40;
  Data[26]+=0x40;
  
  Data[4]-=0x10;
  Data[8]-=0x10;
  Data[12]-=0x10;
  BULCDWrite1(0, Data, 35);
}
//err56
void Err56()
{
  INT8U Data[35];
  memcpy(Data,Data1,35); 
  INT8U i=0;
 for(i=0;i<4;i++)
 {
   Data[i]&=0x00;
 }
  Data[17]|=0xB0;
  Data[17]-=0x40;
  Data[16]|=0xE0;
  Data[21]+=0xB0;
  Data[22]+=0xC0;
  Data[33]+=0x90;
  Data[34]+=0xE0;
  Data[32]+=0x60;
  Data[30]|=0x60;
  Data[28]+=0x40;
  Data[26]+=0x40;
  
  Data[4]-=0x10;
  Data[8]-=0x10;
  Data[12]-=0x10;
  BULCDWrite1(0, Data, 35);
}
//判断电表故障 得出故障数目******************************************************
extern INT32U GetAD0Voltage();
extern INT8U g_nErrBuf[2];
extern INT8U g_ClockWrong;
void CheckMeterFault()
{  
//时钟电池电压低 电池电压低于3.3v报此错误
  if(g_ClockWrong==1) 
  {
   ERR04=1;
   MeterWrongNum++;
  }
  else
  {
  ERR04=0;
  }
//内部程序错误 电量计算的全局变量校验和不对
//内部erpom读写故障
  if((g_nErrBuf[0]&0x01)==1||((g_nErrBuf[0]&0x02)>>1)==1)
  {
   ERR06=1;
   MeterWrongNum++;
  }
  else
  {
  ERR06=0;
  }
//电表时钟故障，电表硬时钟不能正常读写
  if(((g_nErrBuf[0]&0x04)>>2)==1)
  {
   ERR08=1;
   MeterWrongNum++;
  }
  else
  {
  ERR08=0;
  }
}
//判断事件错误 求出事件错误数目*************************************************
INT8U tmptime[5]={0};//电流不平衡，功率因数超限，超功需量，有功电能方向改变时间存储
INT8U WrongState[5]={0};//电流不平衡，功率因数超限，超功需量，有功电能方向改变事件发生状况存储
INT8U Eventalarm=0;
INT8U Wrong_54flg=0;
void CheckMistake()
{  
   //过载51 用户使用负荷大于的1.2倍的最大电流时，电表轮显“Err－51”******************
   MEM_AC_REAL_DATA            sRealData;             //使用全局变量
   pMEM_AC_REAL_DATA           pRealData = &sRealData;
   MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
  
   if (	(pDataTable->sRealInstant.bsRunStat[3 + 0] & 0x0020)||(pDataTable->sRealInstant.bsRunStat[3 + 1] & 0x0020)
       ||(pDataTable->sRealInstant.bsRunStat[3 + 2] & 0x0020))
    {
       ERR51=1;
       EveWrongNum++;
     }
   else
   {
     ERR51=0;
   }
  //电流严重不平衡52*****************************************************
  FP32 a=(pDataTable->fCurr[0]);
  FP32 b=(pDataTable->fCurr[1]);
  FP32 c=(pDataTable->fCurr[2]);
  FP32 t=0;

  if(a==0&&b==0&&c==0)
  {
     t=0;
  }
  else
   {
        if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType))
        {   
            if(a>=b)
              {
               if(b>=c)
                {
                 t=(a-c)/a;   
                }
               else
                {
                  if(a>=c)
                   {
                     t=(a-b)/a;
                   }
                  else
                   {
                     t=(c-b)/c; 
                   }
                 }
               }  
              else
               {
                 if(a>=c)
                  {
                    t=(b-c)/b;   
                  }
                 else
                  {
                    if(b>=c)
                     {
                       t=(b-a)/b;
                     }
                    else
                     {
                       t=(c-a)/c;  
                     }
                  }
               }
        }
   }
 if(second1==1)
  { 
    second1=0;
    if(t>=0.28)
     {  
       if(++tmptime[0]>=3)
        {
          WrongState[0]=1;
          if(tmptime[0]>3)
             tmptime[0]=3;
        }
     }
    else
    {
     tmptime[0]=0;
     WrongState[0]=0;
    }
  }
  if(WrongState[0]==1)
  {
  ERR52=1;
  EveWrongNum++;
  }
  else
  {
  ERR52=0;
  }
  //过压53 电压大于1.15倍Un****************************************************
  if(second5==1)
  {
    second5=0;
    if((pDataTable->fVolt[0] > (g_nVolt * OVER_VOLT_OC_RATIO))||(pDataTable->fVolt[1] > (g_nVolt * OVER_VOLT_OC_RATIO))
       ||(pDataTable->fVolt[2] > (g_nVolt * OVER_VOLT_OC_RATIO)))
      {
       if(++tmptime[4]>=3)
          {
            WrongState[4]=1;
            if(tmptime[4]>3)
               tmptime[4]=3;
          } 
      }
   
    else
     {
       tmptime[4]=0;
       WrongState[4]=0;
     }
       
  }
 if( WrongState[4]==1)
  {
    ERR53=1;
    EveWrongNum++;
   }
  else
  {
   ERR53=0;
  }
  //功率因数超限54 用户环境功率因数小于0.2，电表轮显“Err－54”*****************
  if(second2==1)
   {  
    second2=0;
    if(Wrong_54flg == 0)  //刚上电时不进如此错误判断
    {
      Wrong_54flg = 1 ;
    }
    else
    {
      if((((pDataTable->sRealInstant.sRealPf[1].nSint)&0x10)==0&&((pDataTable->sRealInstant.sRealPf[1].nSint)&0x0F)<2)||
         (((pDataTable->sRealInstant.sRealPf[2].nSint)&0x10)==0&&((pDataTable->sRealInstant.sRealPf[2].nSint)&0x0F)<2)||
         (((pDataTable->sRealInstant.sRealPf[3].nSint)&0x10)==0&&((pDataTable->sRealInstant.sRealPf[3].nSint)&0x0F)<2))
        {
          if(++tmptime[1]>=3)
            {
              WrongState[1]=1;
              if(tmptime[1]>3)
                 tmptime[1]=3;
            } 
        }
     
      else
       {
         tmptime[1]=0;
         WrongState[1]=0;
       }
    }
   }
  if( WrongState[1]==1)
   {
    ERR54=1;
    EveWrongNum++;
   }
  else
  {
   ERR54=0;
  }
  //超有功需量超限55*******************************************************
   
  MT_DGN_DEMAND_DATA	sAcDe;
  memset(&sAcDe, 0, sizeof(MT_DGN_DEMAND_DATA));
  MemReadAcRealData(F7, &sAcDe, sizeof(sAcDe));
  if(second3==1)
   { 
    second3=0;
    if((( (sAcDe.sPPDemand[0].nFrac0>>4)*0.001+(sAcDe.sPPDemand[0].nFrac0&0x0F)*0.0001+
       (sAcDe.sPPDemand[0].nFrac1>>4)*0.1+(sAcDe.sPPDemand[0].nFrac1&0x0F)*0.01+
       (sAcDe.sPPDemand[0].nIntg>>4)*10+(sAcDe.sPPDemand[0].nIntg&0x0F))>
       (3*g_nVolt*1.2*MAX_CURR/(10000000)))||
       (( (sAcDe.sNPDemand[0].nFrac0>>4)*0.001+(sAcDe.sNPDemand[0].nFrac0&0x0F)*0.0001+
       (sAcDe.sNPDemand[0].nFrac1>>4)*0.1+(sAcDe.sNPDemand[0].nFrac1&0x0F)*0.01+
       (sAcDe.sNPDemand[0].nIntg>>4)*10+(sAcDe.sNPDemand[0].nIntg&0x0F))>
       (3*g_nVolt*1.2*MAX_CURR/(10000000)))
        )
      {
         if(++tmptime[2]>=3)
          {
            WrongState[2]=1;
            if(tmptime[2]>3)
               tmptime[2]=3;
          } 
      }
    else
    {
       tmptime[2]=0;
       WrongState[2]=0;
    }
   }
  if( WrongState[2]==1)
  {
  //ERR55=1;
  //EveWrongNum++;
  }
  else
  {
  ERR55=0;
  }
   //有功电能方向改变56****************************************************** 
  
  if(second4==1)
   { 
     second4=0;
     if(((pDataTable->nPFlag&0x08)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[0].nFrac0, 3, 0))))||
       ((pDataTable->nPFlag&0x01)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[1].nFrac0, 3, 0))))||
       ((pDataTable->nPFlag&0x02)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[2].nFrac0, 3, 0))))||
       ((pDataTable->nPFlag&0x04)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[3].nFrac0, 3, 0))))
        )   
     { 
       if(++tmptime[3]>=3)
        {
         WrongState[3]=1;
         if(tmptime[3]>30)
           tmptime[3]=3;
        }
      }
     else
     {
       tmptime[3]=0;
       WrongState[3]=0;
     }
   }
    
    if(WrongState[3]==1)
    {
    ERR56=1;
    EveWrongNum++;
    } 
    else
    {
     ERR56=0;
    }
  /*
  if(ERR51+ERR52+ERR53+ERR54+ERR55+ERR56>=1)
   {
     if(g_nFlg_Alarm==0)
     {
        g_nFlg_Alarm=1;
        Eventalarm=1;
     }
   }
  else
  {
    if(Eventalarm==1)
    {
      if(g_nFlg_Alarm==1)
      {
       g_nFlg_Alarm=0;
       Eventalarm=0;
      }
     }
  }
  */
}
//依次显示事件错误*************************************************************
void DisplyWrong2(INT8U nStepCnt)
{
if(ERR04==1&&nStepCnt==1)
  {
   Err04();
  }
else if((ERR06==1&&nStepCnt==2&&ERR04==1)||
        (ERR06==1&&nStepCnt==1&&ERR04==0))
  {
   Err06();
   }
else if((ERR08==1&&ERR04+ERR06==0&&nStepCnt==1)||
         (ERR08==1&&ERR04+ERR06==1&&nStepCnt==2)||
         (ERR08==1&&ERR04+ERR06==2&&nStepCnt==3))
  {
   Err08();
  }
}
void DisplyWrong(INT8U nStepCnt)
{
#if DISPLY_SWITCH
    if(ERR51==1&&nStepCnt==69)
    {
        Err51();
    }
    else if((ERR52==1&&nStepCnt==70&&ERR51==1)||
        (ERR52==1&&nStepCnt==69&&ERR51==0))
    {
        Err52();
    }
    else if((ERR53==1&&ERR52+ERR51==0&&nStepCnt==69)||
            (ERR53==1&&ERR52+ERR51==1&&nStepCnt==70)||
            (ERR53==1&&ERR52+ERR51==2&&nStepCnt==71))
    {
        Err53();
    }
    else if((ERR54==1&&ERR53+ERR52+ERR51==0&&nStepCnt==69)||
            (ERR54==1&&ERR53+ERR52+ERR51==1&&nStepCnt==70)||
            (ERR54==1&&ERR53+ERR52+ERR51==2&&nStepCnt==71)||
            (ERR54==1&&ERR53+ERR52+ERR51==3&&nStepCnt==72))
    {
        Err54();
    }
    else if((ERR55==1&&ERR54+ERR53+ERR52+ERR51==0&&nStepCnt==69)||
            (ERR55==1&&ERR54+ERR53+ERR52+ERR51==1&&nStepCnt==70)||
            (ERR55==1&&ERR54+ERR53+ERR52+ERR51==2&&nStepCnt==71)||
            (ERR55==1&&ERR54+ERR53+ERR52+ERR51==3&&nStepCnt==72)||
            (ERR55==1&&ERR54+ERR53+ERR52+ERR51==4&&nStepCnt==73))
    {
        Err55();
    }
    else if((ERR56==1&&ERR55+ERR54+ERR53+ERR52+ERR51==0&&nStepCnt==69)||
            (ERR56==1&&ERR55+ERR54+ERR53+ERR52+ERR51==1&&nStepCnt==70)||
            (ERR56==1&&ERR55+ERR54+ERR53+ERR52+ERR51==2&&nStepCnt==71)||
            (ERR56==1&&ERR55+ERR54+ERR53+ERR52+ERR51==3&&nStepCnt==72)||
            (ERR56==1&&ERR55+ERR54+ERR53+ERR52+ERR51==4&&nStepCnt==73)||
            (ERR56==1&&ERR55+ERR54+ERR53+ERR52+ERR51==5&&nStepCnt==74))
    {
        Err56();
    }
#else
    if(ERR51==1&&nStepCnt==83)
    {
        Err51();
    }
    else if((ERR52==1&&nStepCnt==84&&ERR51==1)||
        (ERR52==1&&nStepCnt==83&&ERR51==0))
    {
        Err52();
    }
    else if((ERR53==1&&ERR52+ERR51==0&&nStepCnt==83)||
            (ERR53==1&&ERR52+ERR51==1&&nStepCnt==84)||
            (ERR53==1&&ERR52+ERR51==2&&nStepCnt==85))
    {
        Err53();
    }
    else if((ERR54==1&&ERR53+ERR52+ERR51==0&&nStepCnt==83)||
            (ERR54==1&&ERR53+ERR52+ERR51==1&&nStepCnt==84)||
            (ERR54==1&&ERR53+ERR52+ERR51==2&&nStepCnt==85)||
            (ERR54==1&&ERR53+ERR52+ERR51==3&&nStepCnt==86))
    {
        Err54();
    }
    else if((ERR55==1&&ERR54+ERR53+ERR52+ERR51==0&&nStepCnt==83)||
            (ERR55==1&&ERR54+ERR53+ERR52+ERR51==1&&nStepCnt==84)||
            (ERR55==1&&ERR54+ERR53+ERR52+ERR51==2&&nStepCnt==85)||
            (ERR55==1&&ERR54+ERR53+ERR52+ERR51==3&&nStepCnt==86)||
            (ERR55==1&&ERR54+ERR53+ERR52+ERR51==4&&nStepCnt==87))
    {
        Err55();
    }
    else if((ERR56==1&&ERR55+ERR54+ERR53+ERR52+ERR51==0&&nStepCnt==83)||
            (ERR56==1&&ERR55+ERR54+ERR53+ERR52+ERR51==1&&nStepCnt==84)||
            (ERR56==1&&ERR55+ERR54+ERR53+ERR52+ERR51==2&&nStepCnt==85)||
            (ERR56==1&&ERR55+ERR54+ERR53+ERR52+ERR51==3&&nStepCnt==86)||
            (ERR56==1&&ERR55+ERR54+ERR53+ERR52+ERR51==4&&nStepCnt==87)||
            (ERR56==1&&ERR55+ERR54+ERR53+ERR52+ERR51==5&&nStepCnt==88))
    {
        Err56();
    }
#endif
}

//电量数据显示
void DataDeal(INT8U*  nBuf,INT32U    nDI,INT8U* Data)
{

    INT8U       i  = 0; 
    INT8U       j = 0 ;
    INT8U       nBuf1[8]; 
    INT8U       nBuf2[8];
    INT8U       nTemBuf[4];
    
    if((g_MeterInsideWrong[0]&0x04)==0x04) //电量数据显示故障模拟
    {
        memset(&nBuf, 0, 4); 
    }
    Long2Buf((INT8U*)&nTemBuf, nDI, 4); 
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
        memset(nBuf, 0, sizeof(nBuf));
    }
    for(i = 0; i < 4; i ++ )
    {
        nBuf1[i*2]  = nBuf[i]&0x0f;
        nBuf1[i*2+1]  = nBuf[i]>>0x04;

        nBuf2[i*2]  = nTemBuf[i]&0x0f;
        nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
    }

    for(i = 0; i < 8; i ++)
    { 
        Data[15-i*2] += DataAll[nBuf2[i] *2];
        Data[14-i*2] += DataAll[nBuf2[i] *2 + 1];
    }
    Data[17] = DataAll[nBuf1[0] *2 ];
    Data[16] = DataAll[nBuf1[0] *2 + 1];     
    Data[21] += Data_P[nBuf1[1] *2];
    Data[22] += Data_P[nBuf1[1] *2 +1 ];

    Data[23] += DataAll[nBuf1[2] *2];
    Data[24] += DataAll[nBuf1[2] *2 + 1];

    for(j = 7; j >= 3; j --)
    {
        if(nBuf1[j]!= 0)       
        { 
            for(i = 0; i < j-2; i ++)
            {  
                Data[25 + i*2] += DataAll[nBuf1[i + 3] *2];
                Data[26 + i*2] += DataAll[nBuf1[i + 3] *2 + 1];
            }
            j = 3;
        }
    }

}
//日期数据显示
void Time1Deal(INT8U*  nBuf1,INT32U    nDI,INT8U* Data)
{
    INT8U       i  = 0; 
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];

    Long2Buf((INT8U*)&nTemBuf, nDI, 4);    
    if(!IsAllBcd(&nBuf1[0], 6))		/*BCD码有效性判断*/
    {
        memset(nBuf1, 0, 6);
    }
    for(i = 0; i < 8; i ++)
    { 
        nBuf2[i*2]  = nTemBuf[i]&0x0f;
        nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
        Data[15-i*2] += DataAll[nBuf2[i] *2];
        Data[14-i*2] += DataAll[nBuf2[i] *2 + 1];
    }

    Data[17] = DataAll[nBuf1[0] *2];
    Data[16] = DataAll[nBuf1[0] *2 + 1];
    Data[21] += Data_P[nBuf1[1] *2];
    Data[22] += Data_P[nBuf1[1] *2 +1];

    Data[23] += DataAll[nBuf1[2] *2];
    Data[24] += DataAll[nBuf1[2] *2 +1];
    Data[25] += Data_P[nBuf1[3] *2];
    Data[26] += Data_P[nBuf1[3] *2 +1];

    Data[27] += DataAll[nBuf1[4] *2];
    Data[28] += DataAll[nBuf1[4] *2 +1 ];
    Data[29] += DataAll[nBuf1[5] *2 ];
    Data[30] += DataAll[nBuf1[5] *2 +1];
}
//时间数据显示
void Time2Deal(INT8U*  nBuf1,INT32U    nDI,INT8U* Data)
{
    INT8U       i  = 0; 
   INT8U       nBuf2[16];
   INT8U       nTemBuf[8];
   
   INT8U  Data_P2[20] = {  0xf0, 0xb8,  0x60,0x18,   0xd0,0x78,  0xf0,0x58,   0x60,0xd8,  0xb0,0xd8,  0xb0,0xf8,  0xe0,0x18,  0xf0,0xf8,  0xf0,0xe8};//带秒、分前两点

    Long2Buf((INT8U*)&nTemBuf, nDI, 4);
    if(!IsAllBcd(&nBuf1[0], 6))	/*BCD码有效性判断*/
    {
      memset(nBuf1, 0, 6);
    }
   for(i = 0; i < 8; i ++)
   { 
      nBuf2[i*2]  = nTemBuf[i]&0x0f;
      nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
      Data[15-i*2] += DataAll[nBuf2[i] *2];
      Data[14-i*2] += DataAll[nBuf2[i] *2 + 1];
   }
  
     Data[17] = DataAll[nBuf1[0] *2];
      Data[16] = DataAll[nBuf1[0] *2 +1 ];  
     Data[21] += Data_P2[nBuf1[1] *2];
      Data[22] += Data_P2[nBuf1[1] *2 +1];
     Data[23] += DataAll[nBuf1[2] *2];
      Data[24] += DataAll[nBuf1[2] *2 +1];
     Data[25] += Data_P2[nBuf1[3] *2];
      Data[26] += Data_P2[nBuf1[3] *2 +1];
      
      Data[27] += DataAll[nBuf1[4] *2];
      Data[28] += DataAll[nBuf1[4] *2 +1];
      Data[29] += DataAll[nBuf1[5] *2];
      Data[30] += DataAll[nBuf1[5] *2 + 1];
    
}
void DemandTime(INT8U*  nBuf1,INT32U    nDI,INT8U* Data)
{
   INT8U       i  = 0; 
   INT8U       nBuf2[16];
   INT8U       nTemBuf[8];
   
   INT8U  Data_P2[20] = {  0xf0, 0xb8,  0x60,0x18,   0xd0,0x78,  0xf0,0x58,   0x60,0xd8,  0xb0,0xd8,  0xb0,0xf8,  0xe0,0x18,  0xf0,0xf8,  0xf0,0xe8};//带秒、分前两点

    Long2Buf((INT8U*)&nTemBuf, nDI, 4);
  if(!IsAllBcd(&nBuf1[0], sizeof(nBuf1)))		/*BCD码有效性判断*/
    {
      memset(nBuf1, 0, sizeof(nBuf1));
    }
   for(i = 0; i < 8; i ++)
   { 
      nBuf2[i*2]  = nTemBuf[i]&0x0f;
      nBuf2[i*2+1]  = nTemBuf[i]>>0x04;
      Data[15-i*2] += DataAll[nBuf2[i] *2];
      Data[14-i*2] += DataAll[nBuf2[i] *2 + 1];
   }
  
    Data[17] = DataAll[nBuf1[0] *2];
    Data[16] = DataAll[nBuf1[0] *2 +1 ];  
    Data[21] += Data_P2[nBuf1[1] *2];
    Data[22] += Data_P2[nBuf1[1] *2 +1];
    Data[23] += DataAll[nBuf1[2] *2];
    Data[24] += DataAll[nBuf1[2] *2 +1];
    Data[25] += DataAll[nBuf1[3] *2];
    Data[26] += DataAll[nBuf1[3] *2 +1];
       
}

//用于将电流电压显示值缩小30%
void DivideTwo2(INT8U* pData)
{  
  INT8U nTmp[2];
  INT32U nNum;
  memcpy(nTmp,pData,2);  
  nTmp[0]=BCD2Dec(nTmp[0]);
  nTmp[1]=BCD2Dec(nTmp[1]);
  nNum=(nTmp[0]+nTmp[1]*100)/1.5;
  nTmp[1]=Dec2BCD(nNum/100);
  nTmp[0]=Dec2BCD(nNum%100);
  
  *pData=nTmp[0];
  *(pData+1)=nTmp[1];
}


//用于将电流电压显示值缩小50%
void DivideTwo(INT8U* pData)
{  
  INT8U nTmp[2];
  INT32U nNum;
  memcpy(nTmp,pData,2);  
  nTmp[0]=BCD2Dec(nTmp[0]);
  nTmp[1]=BCD2Dec(nTmp[1]);
  nNum=(nTmp[0]+nTmp[1]*100)/2;
  nTmp[1]=Dec2BCD(nNum/100);
  nTmp[0]=Dec2BCD(nNum%100);
  
  *pData=nTmp[0];
  *(pData+1)=nTmp[1];
}
// 白屏，黑屏，花屏 死机 模拟故障
void ScreenDisplay(INT8U* nStepCnt)
{    
     INT8U Tmp1[35];
     INT8U Tmp2[23];
     INT8U i=0;
     INT8U Num1=0;
     static INT8U Num2=0;
     Num1  =  *nStepCnt;
     if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_100 == g_nHardType))
     {
         if((g_MeterHardwareWrong[0]&0x02)==0x02)   //白屏
         {   
              GpioClearOutput(IO_PORT7, IO_PINx5);
              memset(Tmp1,0,sizeof(Tmp1));
              BULCDWrite1(0, Tmp1, 35); 
              Num1=200;
          }  
         else if((g_MeterHardwareWrong[0]&0x04)||(g_MeterHardwareWrong[0]&0x01))   //黑屏 不启动 故障
         {    
              GpioSetOutput(IO_PORT7, IO_PINx5);       
              GpioClearOutput(IO_PORT2, IO_PINx1);
              memset(Tmp1,0,sizeof(Tmp1));
              BULCDWrite1(0, Tmp1, 35); 
              Num1=200;
         } 
         else if((g_MeterHardwareWrong[0]&0x08)==0x08)   //花屏
         {  
             if(Num2==0) //对初始化数组进行改变
             {    
                  if(Num1>199)
                  {
                    Num1=1;
                  }
                  Num2=1;  //仅改变一次
                  for(i=0;i<15;i++)
                  {
                     Data1[i]+=0x01; 
                  }
                  for(i=15;i<35;i++)
                  {
                     Data1[i]-=0x02; 
                  }
              }
         }
         else if((g_MeterHardwareWrong[0]&0x0E)==0) 
         {   
              if(Num1>199)
              {
                 Num1=1;
              }
              if(Num2==1)
              {   
                  Num2=0;
                  for(i=0;i<35;i++)
                  {
                     Data1[i]=Data1Copy[i]; 
                  }
              }
         }
     }
     else if(SINGLEPHASE_METER == g_nHardType)
     {
         if((g_MeterHardwareWrong[0]&0x02)==0x02)   //白屏
         {   
              GpioClearOutput(IO_PORT7, IO_PINx5);
              memset(Tmp2,0,sizeof(Tmp2));
              BULCDWrite1(0, Tmp2, 23); 
              Num1=200;
         }  
         else if((g_MeterHardwareWrong[0]&0x04)||(g_MeterHardwareWrong[0]&0x01))   //黑屏
         {    
              GpioSetOutput(IO_PORT7, IO_PINx5);       
              GpioClearOutput(IO_PORT2, IO_PINx1);
              memset(Tmp2,0,sizeof(Tmp2));
              BULCDWrite1(0, Tmp2, 23); 
              Num1=200;
         } 
         else if((g_MeterHardwareWrong[0]&0x08)==0x08)   //花屏
         {  
             if(Num2==0) //对初始化数组进行改变
             {    
                  if(Num1>199)
                  {
                    Num1=100;
                  }
                  Num2=1;  //仅改变一次
                  for(i=0;i<10;i++)
                  {
                     Data2[i]+=0x01; 
                  }
                  for(i=10;i<23;i++)
                  {
                     Data2[i]-=0x02; 
                  }
              }
         }
         else if((g_MeterHardwareWrong[0]&0x0E)==0) 
         {   
              if(Num1>199)
              {
                 Num1=100;
              }
              if(Num2==1)
              {   
                  Num2=0;
                  for(i=0;i<23;i++)
                  {
                     Data2[i]=Data2Copy[i]; 
                  }
              }
         }
     }
     if((g_MeterInsideWrong[0]&0x08))   //死机故障
     {
        while((g_MeterInsideWrong[0]&0x08))
        {
        }
     }
     *nStepCnt    =Num1;
} 
//三相表事件显示
void TriphaseEventDisp(void)
{
   static INT8U  nFlg_Event ;
     static INT8U  nFlg ;
    static  INT8U    Temp = 0;            
     
    Temp = (nFlg_Event++) % 2;
    
    if(1==(g_MeterEventWrong[0]&0x01))   //A失压闪烁  模拟
    {
       g_nFlgUa=1;
    }

    if(1==(g_MeterEventWrong[1]&0x01))  //B失压闪烁
    {
      g_nFlgUb=1; 
    }

    if(1==(g_MeterEventWrong[2]&0x01)) //C失压闪烁
    {
      g_nFlgUc=1;
    }  
    
    if (0x04==(g_MeterEventWrong[0]&0x04)) //A失流闪烁 模拟
    {
      g_nFlgIa=1;
    }  

    if (0x04==(g_MeterEventWrong[1]&0x04))  //B失流闪烁
    {
      g_nFlgIb=1; 
    }

    if (0x04==(g_MeterEventWrong[2]&0x04)) //C失流闪烁
    {
      g_nFlgIc=1; 
    }  
    
    if(1 == g_nFlgUa)
      {
        if (Temp)
        {
         Data1[15] &= 0xf7;
        }
        else
        {
          Data1[15] |= 0x08;
         g_nFlgUa = 0;
        }
      }
 if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType))//三相四线与三相三线区分
 {
    if(1 == g_nFlgUb)
     {
        if (Temp)
        {
         Data1[15] &= 0xfb;
        }
        else
        {
          Data1[15] |= 0x04;
         g_nFlgUb = 0;
        }
      }
 }
    if(1 == g_nFlgUc)
     {
        if(Temp)
        {
         Data1[14] &= 0xfb;
        }
        else
        {
          Data1[14] |= 0x04;
         g_nFlgUc = 0;
        }
     }
       if(1 == g_nFlgVoltPhase)
     {
        if (Temp)
        {
          Data1[14] &= 0xf7;
        }
        else
        {
          Data1[14] |= 0x08;
         g_nFlgVoltPhase = 0;
        } 
        
     }
     if(1 == g_nFlgIa)
     {
        if(Temp)
        {
         Data1[13] &= 0xf3;
        }
        else
        {
         Data1[13] |= 0x08;
         g_nFlgIa = 0;
        }
      }
if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType))//三相四线与三相三线区分
{
     if(1 == g_nFlgIb)
     {
        if(Temp)
        {
         Data1[12] &= 0xf3;
        }
        else
        {
         Data1[12] |= 0x08;
         g_nFlgIb = 0;
        }
      }
}
      if(1 == g_nFlgIc)
     {
        if(Temp)
        {
         Data1[11] &= 0xf3;
        }
        else
        {
         Data1[11] |= 0x08;
         g_nFlgIc = 0;
        }
      }
   if(((1 == g_nFlg_Commu)||(4 == g_nFlg_Commu))&&((g_MeterHardwareWrong[0]&0x04)!=0x04)&&(0x01 != (g_MeterHardwareWrong[0]&0x01)))
     {
         Data1[6] |= 0x02;
         g_nFlg_Commu = 0;
        GpioClearOutput(IO_PORT7, IO_PINx5);
        //g_nCmt = 10;
      }
   if((2 == g_nFlg_Commu)&&((g_MeterHardwareWrong[0]&0x04)!=0x04)&&(0x01 != (g_MeterHardwareWrong[0]&0x01)))
   {
         Data1[6] |= 0x06;
         g_nFlg_Commu = 0;
         GpioClearOutput(IO_PORT7, IO_PINx5);
        // g_nCmt = 10;
    }
   if((3 == g_nFlg_Commu)&&((g_MeterHardwareWrong[0]&0x04)!=0x04)&&(0x01 != (g_MeterHardwareWrong[0]&0x01)))
   {
         Data1[6] |= 0x0a;
         g_nFlg_Commu = 0;
         GpioClearOutput(IO_PORT7, IO_PINx5);
        // g_nCmt = 10;
    }
    
    if((4 == g_nFlg_Commu)&&((g_MeterHardwareWrong[0]&0x04)!=0x04)&&(0x01 != (g_MeterHardwareWrong[0]&0x01)))
   {
         Data1[6] |= 0x01;
         g_nFlg_Commu = 0;
         GpioClearOutput(IO_PORT7, IO_PINx5);
        // g_nCmt = 10;
    }
   
   if(1 == g_nFlg_Commu2)
    {  if((nFlg++) % 2)    
      { 
            Data1[6] &= 0xf0;
         g_nFlg_Commu2 = 0;
      }
    }
      
    if(1 == g_nFlg_Alarm)
    { 
      if(Temp)    
      { 
         Data1[7] &= 0xfe;  //告警标志
      }  
      else
      {
        Data1[7] |= 0x01; 
       g_nFlg_Alarm = 0;
      }
      GpioClearOutput(IO_PORT3, IO_PINxD);
    }
    else
    {
      Data1[7] &= 0xfe;
      GpioSetOutput(IO_PORT3, IO_PINxD);
    }

}


//单相表电量数据显示
void sAcCombEnergyData(INT8U*  nBuf, INT8U* Data)
{
    INT8U       i = 0;  
    INT8U       j = 0;     
   INT8U       nBuf1[8] ; 
   if((g_MeterInsideWrong[0]&0x04)==0x04) //电量显示故障模拟
   {
       memset(&nBuf, 0, 4); 
   }

  if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD码有效性判断*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }
  
    for(i = 0; i < 4; i ++ )
    {
      nBuf1[i*2]  = nBuf[i]&0x0f;
      nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }

     Data[10] += DataAll_2[nBuf1[0] *2 ];
     Data[11] += DataAll_2[nBuf1[0] *2 + 1];     
     Data[12] += DataAll_2[nBuf1[1] *2] ;
     Data[13] += DataAll_2[nBuf1[1] *2 +1 ];
     Data[14] += Data_P_2[nBuf1[2] *2];
     Data[15] += Data_P_2[nBuf1[2] *2 + 1];    
     
     for(j = 7; j >= 3; j --)
      {
        if(nBuf1[j]!= 0)       
        {    
            if(7 == j)
             {   
                Data[21] += DataAll_2[nBuf1[j] *2 ];
                Data[22] += DataAll_2[nBuf1[j] *2 + 1];
                j = 6;
             }
            for(i = 0; i < j-2; i ++)
            {  
              Data[i*2] += DataAll_2[nBuf1[i + 3] *2];
              Data[i*2 + 1] += DataAll_2[nBuf1[i + 3] *2 + 1];
            } 
          j = 3;
         }       
      }  
         
    Data[0] += 0x08;
    Data[1] += 0x08;
    Data[9] += 0x10;
    Data[10] += 0x10; 
}


//单相表事件显示
void SinglephaseEventDisp(void)
{
  static INT8U  nFlg_Event ;
    static  INT8U    Temp = 0;  
    Temp = (nFlg_Event++) % 2;
  
   if(((1 == g_nFlg_Commu)||(2 == g_nFlg_Commu)||(3 == g_nFlg_Commu))&&((g_MeterHardwareWrong[0]&0x04)!=0x04))//&&(0x01 != (g_MeterHardwareWrong[0]&0x01)
   {
         Data2[7] |= 0x08;
         g_nFlg_Commu = 0;
         GpioClearOutput(IO_PORT7, IO_PINx5);
         //g_nCmt = 10;
    }
   
    if(((4 == g_nFlg_Commu))&&((g_MeterHardwareWrong[0]&0x04)!=0x04))
    {
        Data2[19] |= 0x08;
        g_nFlg_Commu = 0;
        GpioClearOutput(IO_PORT7, IO_PINx5);
    }
    if(1 == g_nFlg_Commu2)
    {  
        if (Temp)      
        { 
            Data2[7]  &= 0xf7;
            Data2[19] &= 0xf7;
            g_nFlg_Commu2 = 0;
        }
    }
   if(1 == g_nFlg_Pro)
     {
        if (Temp)
        {
            Data2[21]  |= 0x04; 
        }
        else
        {
            Data2[21]  &= 0xfb;
            g_nFlg_Pro = 0;
        }
      }
   
} 




