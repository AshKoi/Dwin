/******************************************************************************************************
* Copyright (c) 2013
* �ļ���:
* ������: ��һ��
* �ա���: 2013/09/12
* �޸���:
* �ա���:
* �衡��:
* �桡��: Version 1.0
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
                                           �궨��
***************************************************************************************************/
#define GET_KEY_VAL(nKey, nType)	nKey = GetKeyVal(nType)

#define A_MENU_LEN  30

#define DEC2BCD(x)	              ((((x)/10)<<4)+(x)%10)
#define BCD2DEC(x)	              (((x)&0x0F)+((x)>>4)*10)
//#if  TRIPHASE_METER
#define  LcdDisplayPage_NUM           82                  // ���������������� 
//#endif
//#if  SINGLEPHASE_METER
#define  LcdDisplayPage_NUM           22                // ����������������
//#endif

/**************************************************************************************************
                                           ��������
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
INT8S			g_nSynBaseUartMode 	= 0;         			// ͨѶ��,����
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
INT8S			g_nSynBaseUartBPS 	= 3;          			// ������,1200

const INT8S 	g_nParityArray[3]		= {NO_PARITY, ODD_PARITY, EVEN_PARITY};
INT8S			g_nSynBaseParity 		= 2;           			// ��żУ��,E

const INT8S 	g_nStopBitArray[] 		= {ONE_STOP_BIT, TWO_STOP_BIT};
INT8S			g_nSynBaseStopBit 	= 0;          			// ֹͣλ,1

const INT8S 	g_nCharBitArray[] 		= {SEVEN_CHAR_BIT, EIGHT_CHAR_BIT};
INT8S 			g_nSynBaseCharBit 	= 1;          			// ����λ,8

const INT8S 	g_nCharDispFrm[]  		= {0, 1};
INT8S  			g_nShowHex				= 0;		        	// ������ʾ

const INT8S   	g_nCtIRSelct[]      	= {1, 0};               //��͸ʱѡ�����
INT8S   		g_nCtIr              	= 0;


INT8S   		g_nBeepFlg         	= 0;					//������
INT8S   		g_nContinueKeyFlg     = 0;					//�����������عر�

INT8S   		g_nChargeKeyFlg = 0;					
				


INT8S 			g_nTimeOut[10] 		= {"3000"};				//��ʱʱ��


INT8S 			g_nUpGradeTimeOut[3] 	= {"10"};				//��ʱʱ��

const INT32S   	g_nLedBackLight[]   	= {600, 1200, 1800, 0};
INT8S 			g_LedBackLtCloseTime  	= 1;


const INT8S   	g_nLedBackArray[]   	= {3, 5, 10, 0};
INT8S 			g_LedBackCloseTime  	= 1; 					//�����Զ��ر�

const INT8S   	g_nTermCloseArray[] 	= {5, 15, 30,60, 0};
INT8S  			g_TermCloseTime     	= 4; 					//�ն��Զ��ػ�


// 
static INT8U	g_nLedBackCnt			= 0;					//�������
static INT8U	g_nTermDispCnt			= 0;					//�Զ��ػ�����

static INT8U 	g_nBgFlg 				= 0;					//����ر�־

#define LEN_REAL_SCREEN_NUM           14                      //ʵʱ���ݵ�������

#define LEN_DAY_SCREEN_NUM            3                       //��(��)���ݵ�������

INT8U   nSwitchFlg                    = 1;                      //��������д������ת��

extern  const MENU_ITEM     g_sZbMenu[];

METER_INFO           sMeterInfo;
pMETER_INFO	         pMeterInfo	                 = &sMeterInfo;

INT8U      GprsStep            = 0;           //GPRSִ�в���
static    INT8U      nCounter   = 0;
INT8U	    nStepInfo[7] 	     = {0};

extern    pKEY_SAMP	      g_pKeySamp;

INT8U                nSwMpFlg                     = 1;  //������鿴�˵�

OPERTOR_PRM			sOpertor;
pOPERTOR_PRM		pOpertor	= &sOpertor;

  
extern INT8U   g_MeterEventWrong[4];   
extern INT8U   g_MeterInsideWrong[2]    ;   
extern INT8U   g_MeterHardwareWrong[2]  ;
const  INT8S*	c_PartTypeC[]		={"[0]100A���طѿر�",
                                    "[1]100AԶ�̷ѿر�",
                                    "[2]60A���طѿر�",
                                    "[3]60AԶ�̷ѿر�",
                                    "[4]100A�����ʱ��",
                                    "[5]60A�����ʱ��",
                                    "[6]100A���࿨��",
                                    "[7]100A�����й���",
                                    "[8]60A���࿨��",
                                    "[9]60A�����й���",
                                    "[10]���׶๦��",
                                    "[11]�๦�ܵ��",
                                    "[12]���ܵ��ܱ�",
										"δ֪���",0};
const  INT8S*	c_PartTypeD[]		={"[0]80A���طѿر�",
                                    "[1]80AԶ�̷ѿر�",
                                    "[2]60A���طѿر�",
                                    "[3]60AԶ�̷ѿر�",
                                    "[4]80A�����ʱ��",
                                    "[5]60A�����ʱ��",
										"δ֪���",0};
const  INT8S*	c_PartTypeE[]		={"[0]100A���౾�طѿ�",
                                    "[1]100A����Զ�̷ѿ�",
                                    "[2]60A���౾�طѿ�",
                                    "[3]60A����Զ�̷ѿ�",
                                    "[4]100A�����ʱ",
                                    "[5]60A�����ʱ��",
                                    "[6]100A���࿨��",
                                    "[7]100A�����й���",
                                    "[8]60A���࿨��",
                                    "[9]60A�����й���",
                                    "[10]80A���౾�طѿ�",
                                    "[11]80A����Զ�̷ѿ�",
                                    "[12]60A���౾�طѿ�",
                                    "[13]60A����Զ�̷ѿ�",
                                    "[14]80A�����ʱ��",
                                    "[15]60A�����ʱ��",0};
const  INT8S*	c_PartTypeF[]		={"[0]δ֪���",
                                    "[1]�������װ��",
                                    "[2]�������װ��",
                                    "[3]��/�������װ��",
                                    "[4]�������װ��",
                                    "[5]��/�������װ��",0};


const  INT8S*	c_PartTypeAB[]		={"Ĭ��",0};

const  INT8S**   ArryCollect[] = {c_PartTypeC , c_PartTypeD ,c_PartTypeE ,c_PartTypeF ,c_PartTypeAB,0};
const  INT8S*    ArryName[]    = {"�û�С��(C)","�û�С��(D)","�û�С��(E)","�û�С��(F)","�û�С��(AB)",0};


INT8U  Data0[35] = {   0xff,0xff,  0xff,0xff,  0xff,0xff,  0xff,0xff,  0xff,0xff,  0xff,0xff,  0xff,0xff,  0xff,0xff,  
                              0xff,0xff,0xff,0xff,   0xff,0xff,  0xff,0xff,0xff,0xff,  0xff,0xff,  0xff,0xff,0xff,0xff,  0xff,0xff,  0xff
                 };   // ��ʼ������ 

/**************************************************************************************************
                                        �����洢ƫ����Ϣ
***************************************************************************************************/
#if 0
//6.3 ϵͳʱ��
const MENU_ITEM  g_sSysTimeMenu[] =
{
    {MENU_ITEM_DISCRIB, "ϵͳʱ��", NULL}, 
    
    {0}
};


//6.1 ͨѶ����
const MENU_ITEM  g_sCommInfoCfg[] =
{
    {MENU_ITEM_DISCRIB, "ͨѶ����", NULL}, 
  	{MENU_ITEM_EXE,"1.ͨ�Ż�������", (void*)BaseCommInfoCfg},
    {0}
};


//6.��������
const MENU_ITEM  g_sSysParmMenu[] =
{
    {MENU_ITEM_DISCRIB, "��������", NULL}, 
    //{MENU_ITEM_SHOW,"1.ͨѶ��������", (void*)g_sCommInfoCfg},
    {MENU_ITEM_EXE, "2.ϵͳ��������", (void*)OtherParConfig},
   // {MENU_ITEM_SHOW,"3.ϵͳʱ��", (void*)g_sSysTimeMenu},
    {MENU_ITEM_EXE, "4.�汾��Ϣ", (void*)VersionInfo},

	//{MENU_ITEM_EXE, "6.����ѡ��", (void*)CfgSelectProc},

    {0}
};
#endif

const MENU_ITEM  FacUnitTestMenu[] =
{
    {MENU_ITEM_DISCRIB, "����ģ����", NULL},
	
    {MENU_ITEM_EXE, "1.���ϰ����", (void*)StratCheck},
    {MENU_ITEM_EXE, "2.GPRS����", (void*)GprsTest},
    {MENU_ITEM_EXE, "3.���ư����", (void*)CtrlProcTest},

    {0}
};


const MENU_ITEM  g_sInTestMenu[] =
{
    {MENU_ITEM_DISCRIB, "�ڲ�����", NULL},
	
    {MENU_ITEM_EXE, "1.������Ϣ", (void*)AcInfo},
    //{MENU_ITEM_EXE, "2.GPRS��⿪��", (void*)GprsDataWatch},
    //{MENU_ITEM_SHOW, "3.����ģ����", (void*)FacUnitTestMenu},

    {0}
};


//1.3�ն˹���--������I ��
const MENU_ITEM  g_sConcerAdmin[] =
{
    {MENU_ITEM_DISCRIB, "�ն˹�����ά��", NULL},
	{MENU_ITEM_EXE, "1.��������", (void*)RestartRdMeter},
	{MENU_ITEM_EXE, "2.ֹͣ����", (void*)StopRdMeter},
	{MENU_ITEM_EXE, "3.�ָ�����", (void*)ReturnRdMeter},
	{MENU_ITEM_EXE, "4.��������", (void*)ActivateConnect},
	{MENU_ITEM_EXE, "5.�Ͽ�����", (void*)Disconnect},
	{MENU_ITEM_EXE, "6.�ڵ�ά��", (void*)NodeMaintain},
	{MENU_ITEM_EXE, "7.USB ����", (void*)USBFunction},
	{MENU_ITEM_EXE, "8.Ӳ����λ", (void*)HardWareReset},
    {MENU_ITEM_EXE, "9.��������", (void*)DataReset},
    {MENU_ITEM_EXE, "10.��������", (void*)ParaReset},
    {MENU_ITEM_SHOW, "11.�ڲ�����", (void*)g_sInTestMenu},

    {0}
};


//1.3�ն�����--������I ��
const MENU_ITEM  g_sConcerData[] =
{
    {MENU_ITEM_DISCRIB, "�ն�����", NULL},
	{MENU_ITEM_EXE,  "1.ң��״̬", (void*)RemoteCommState},
	{MENU_ITEM_EXE,  "2.�¼���¼", (void*)EventLog},

    {0}
};


//1.3�ն˹�����ά��--������I ��
const MENU_ITEM  g_sConcerAdminMain[] =
{
    {MENU_ITEM_DISCRIB, "�ն˹�����ά��", NULL},
	{MENU_ITEM_SHOW, "1.�ն�����", (void*)g_sConcerData},
	{MENU_ITEM_EXE,  "2.�ն���Ϣ", (void*)ConcerInfo},
	{MENU_ITEM_SHOW, "3.�ն˹���", (void*)g_sConcerAdmin},

    {0}
};

//1.2ͨ��ͨ������
const MENU_ITEM  g_sCommparaSet[] =
{
    {MENU_ITEM_DISCRIB, "ͨ��ͨ������", NULL},
	{MENU_ITEM_EXE, "1.ͨѶ��ʽ",   (void*)CommunicationModeSet},
	{MENU_ITEM_EXE, "2.��������",   (void*)MessageCenterSet},
	{MENU_ITEM_EXE,  "3.��վIP��ַ", (void*)MainIpAddrSet},
	{MENU_ITEM_EXE,  "4.��̫������", (void*)EthernetParaSet},

    {0}
};

//1.2ͨ��ͨ����ѯ
const MENU_ITEM  g_sCommparaView[] =
{
    {MENU_ITEM_DISCRIB, "ͨ��ͨ����ѯ", NULL},
    {MENU_ITEM_EXE,  "1.ͨѶ��ʽ",   (void*)CommunicationMode},
	{MENU_ITEM_EXE,  "2.��������",   (void*)MessageCenter},
	{MENU_ITEM_EXE,  "3.��վIP��ַ", (void*)MainIpAddr},
	{MENU_ITEM_EXE,  "4.��̫������", (void*)EthernetPara},

    {0}
};

//ͨ��ͨ�����ã�ѡ��ģʽ��
const MENU_ITEM  g_sCommpara[] =
{
    {MENU_ITEM_DISCRIB, "ͨ��ͨ��", NULL},
	{MENU_ITEM_SHOW, "1.��ѯģʽ", (void*)g_sCommparaView},
	{MENU_ITEM_SHOW, "2.����ģʽ", (void*)g_sCommparaSet},

    {0}
};

//���������ã�ѡ��ģʽ��
const MENU_ITEM  g_sMeterPara[] =
{
    {MENU_ITEM_DISCRIB, "������", NULL},
	{MENU_ITEM_EXE, "1.��ѯģʽ", (void*)MeterParamQuery},
	{MENU_ITEM_EXE, "2.����ģʽ", (void*)MeterParaSet},

    {0}
};

//�ն�ʱ�ӵ����ã�ѡ��ģʽ��
const MENU_ITEM  g_sConcerTime[] =
{
    {MENU_ITEM_DISCRIB, "�ն�ʱ��", NULL},
	{MENU_ITEM_EXE, "1.��ѯģʽ", (void*)ConCerTimequery},
	{MENU_ITEM_EXE, "2.����ģʽ", (void*)ConCerTimeSet},

    {0}
};

//�ն˱�����ã�ѡ��ģʽ��
const MENU_ITEM  g_sConcerAddr[] =
{
    {MENU_ITEM_DISCRIB, "�ն˱��", NULL},
	{MENU_ITEM_EXE, "1.��ѯģʽ", (void*)ConcerNumQuery},
	{MENU_ITEM_EXE, "2.����ģʽ", (void*)ConcerNumSet},

    {0}
};


//1.2����������鿴--������I �ͣ���ѯģʽ��
const MENU_ITEM  g_sParaSetView[] =
{
    {MENU_ITEM_DISCRIB, "����������鿴", NULL},
	{MENU_ITEM_SHOW,  "1.ͨ��ͨ������", (void*)g_sCommpara},
	{MENU_ITEM_SHOW,  "2.����������", (void*)g_sMeterPara},
	{MENU_ITEM_SHOW,  "3.�ն�ʱ������", (void*)g_sConcerTime},
	{MENU_ITEM_EXE,  "4.������������", (void*)PswCheck},
	{MENU_ITEM_SHOW,  "5.�ն˱������", (void*)g_sConcerAddr},

    {0}
};

//1.1���������ݲ�ѯ--������I ��
const MENU_ITEM  g_sMeasurePoint[] =
{
    {MENU_ITEM_DISCRIB, "����������", NULL},
	{MENU_ITEM_EXE, "1.ʵʱ����", (void*)RealTimeData},
    {MENU_ITEM_EXE, "2.������", (void*)DayData},
    {MENU_ITEM_EXE, "3.������", (void*)MonData},

    {0}
};


//1���˵�--������I ��
const MENU_ITEM   g_sMainMenu[] =
{
    {MENU_ITEM_DISCRIB, "���˵�", NULL},
	{MENU_ITEM_SHOW, "1.���������ݲ�ѯ", (void*)g_sMeasurePoint},
    {MENU_ITEM_SHOW, "2.����������鿴", (void*)g_sParaSetView},
    {MENU_ITEM_SHOW, "3.�ն˹�����ά��", (void*)g_sConcerAdminMain},
    //{MENU_ITEM_EXE, "4.������Ϣ", (void*)AcInfo},
    //{MENU_ITEM_EXE, "5.�ʲ��������", (void*)TermSelfAddrConfig},

    {0}
};

/**************************************************************************************************
                                           ��������
***************************************************************************************************/
extern void LCD_ShowCharString(INT16U x,INT16U y, INT8U nRows, const INT8U *p, INT8U nMode);

/* ��ͷͼ�� */
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

//�ź�ǿ�ȵ�ָʾT�ӿ�
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

//Һ���ײ�������ʾ
void  BOttleDisplay(void)
{
  INT8U	    nInfo[8] 	       = {0};
  INT8S*    c_BottleShow[] = {"���ڻ�ȡ�ź�ǿ��       ", "���ڽ�������ע��","��������APN",
                             "���������û���������","���ڻ�ȡIP��ַ","����������վ        ","���ڷ��͵�½����","GPRS����","����״̬",0};
  INT8S*    c_Result[] ={"��ȡ�ɹ�","ע��ɹ�","����APN�ɹ�","�����û���������ɹ�",
                                  "��ȡIP��ַ�ɹ�","������վ�ɹ�","�յ���վȷ��",0};

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
        //1.�ϵ�����
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

//�ź�ǿ�����ź�
static void SignalNull(void)
{
	LcdRecClear(8, 0, 23, 14, g_nBackColour);
}

//��G
static void GNull(void)
{
    LcdRecClear(25, 0, 34, 14, g_nBackColour);
}

//�ź�ǿ�������ź�
static void SignalFour(void)
{
	INT8U nBuf[24] = {0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x66,0x00,0x66,0x00,0x66, 0x06,0x66,0x06,0x66,0x06,0x66,0x66,0x66,0x66,0x66,0x66,0x66};

	SignalNull();
	LCD_ShowLogo(8, 2, 16, sizeof(nBuf) * 8, nBuf, g_nBackColour, g_nDrawColour, LCD_DISP_NORMAL);
}

//�ź�ǿ�������ź�
static void SignalThree(void)
{
	INT8U nBuf[24] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x00,0x60,0x00,0x60, 0x06,0x60,0x06,0x60,0x06,0x60,0x66,0x60,0x66,0x60,0x66,0x60};

	SignalNull();
	LCD_ShowLogo(8, 2, 16, sizeof(nBuf) * 8, nBuf, g_nBackColour, g_nDrawColour, LCD_DISP_NORMAL);
}
//�ź�ǿ�ȶ����ź�
static void SignalTwo(void)
{
	INT8U nBuf[24] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 0x06,0x00,0x06,0x00,0x06,0x00,0x66,0x00,0x66,0x00,0x66,0x00};

	SignalNull();
	LCD_ShowLogo(8, 2, 16, sizeof(nBuf) * 8, nBuf, g_nBackColour, g_nDrawColour, LCD_DISP_NORMAL);
}

//�ź�ǿ��һ���ź�
static void SignalOne(void)
{
	INT8U nBuf[24] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x00,0x60,0x00,0x60,0x00};

	SignalNull();
	LCD_ShowLogo(8, 2, 16, sizeof(nBuf) * 8, nBuf, g_nBackColour, g_nDrawColour, LCD_DISP_NORMAL);
}

//�ź�ǿ��ָʾ��־�ӿ�
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


//����ʾ���ݴ���ʱ����ʾ��״̬����ʾ
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
    
    //1���ź�ǿ��
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
    
    //2�����߱�־
    if (g_pRmotValClr->nLoginSucc > 0)
    {
        CommSign();
    }
    else
    {
        GNull();
    }

    //3.GPRSͨѶ��ͷ��־
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

//Һ��������ʱ�����ʾ
void  LcdDisplayTime(void)                  //ÿ��ˢ��һ��
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


//Һ�����������������ʾ
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

//ͨ�ŷ�ʽָʾ
void CommSign(void)
{
   INT8U          nBuf[10]        = "G";

   LCD_DrawXieLine2(25, 0, 34, 14, 1);
   LCD_ShowCharString(26, 0, 15, nBuf, LCD_DISP_NORMAL);
}

//��������
void InterfacePasswordSet(void)
{
    PARAMSET		sParamSet[4] 	               = {0};
    pPARAMSET 		pParamSet		               = &sParamSet[0];
    INT32S          nRet                           = 0;
    INT8S           nComSetMenu[1][A_MENU_LEN]     = {"000000"};
    INT8U           nBuf[1]                        = {1};

    GuiwinSetNullCfgParam(&pParamSet, NULL);
    GuiwinSetNullCfgParam(&pParamSet, NULL);
    GuiwinSetEditCfgParam(&pParamSet, "����������", nComSetMenu[0], 6);

    pParamSet = &sParamSet[0];
    while(1)
    {
        if (GuiwinSetParamMenu("��������", pParamSet) >= 0)
        {
            memcpy(&nComSetMenu[0], pParamSet[2].nEditBuf, GET_MIN(sizeof(pParamSet[2].nEditBuf), sizeof(nComSetMenu[0]) - 1));
            MemoryWrite(STR_OFF(pMEM_FRAM_STORE, sXFramData.nPsw[0]), (INT8U*)&nComSetMenu[0], 6, MEM_FRAM);

            MemoryWrite(STR_OFF(pMEM_FRAM_STORE, sXFramData.nPswSetFlg), (INT8U*)&nBuf[0], 1, MEM_FRAM);
            nRet = Guiwin1MboxCreate("ȷ��","���óɹ�");
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

//������֤
INT32S InterfacePassword(void)
{
    PARAMSET		sParamSet[3]					= {0};
	pPARAMSET		pParamSet						= &sParamSet[0];
    INT32S          nRet                            = 0;
	INT8S			nPwdMenu[A_MENU_LEN]			= {0};
    INT8U           nBuf[10]                        = {0};

    MemoryRead(STR_OFF(pMEM_FRAM_STORE, sXFramData.nPswSetFlg),&nBuf[0] ,1, MEM_FRAM);
    if(nBuf[0] == 0)   //Ĭ�ϳ�ʼ����ֵ"000000"
    {
       memset(&nBuf[0] , 0 ,sizeof(nBuf));
       MemoryWrite(STR_OFF(pMEM_FRAM_STORE, sXFramData.nPsw[0]), &nBuf[0], 6, MEM_FRAM);
    }
	while(1)
	{
		memset(&sParamSet[0], 0, sizeof(sParamSet));
		memset(&nPwdMenu[0], 0, sizeof(nPwdMenu));
		GuiwinSetNullCfgParam(&pParamSet, NULL);
		GuiwinSetEditCfgParam(&pParamSet, "����������:", "******", 100 + STR_SIZE(pOPERTOR_PRM, nOperPsw) * 2);

        pParamSet = &sParamSet[0];
		if (GuiwinSetParamMenu("����", pParamSet) >= 0)
		{
			memset(&nPwdMenu[0], 0, sizeof(nPwdMenu));
            StringToBuf((INT8U*)pParamSet[1].nEditBuf, (INT8U*)&nPwdMenu[0], GET_MIN(STR_SIZE(pOPERTOR_PRM, nOperPsw) * 2, pParamSet[1].nEditLen));
			if (PasswordChk(NULL, (INT8U*)&nPwdMenu[0], 0) == 1)
			{
               return 1;
			}
			Guiwin1MboxCreate("����","���벻��ȷ");
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

//��Ч������
INT16U ValidMpNum(void)
{
    INT16U               nSeq                         = 0;
    INT16S               nValidMp                    = 0;
	INT16U				 nMaxMp						= 0;

    for(nSeq = 1; nSeq < MAX_METER_NUM ; nSeq++)
    {
	   //��Ч������
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
	   //��Ч������
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
	   //��Ч������
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

//��������
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
	
//��������ѯ(����)��������
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

//���������ݵĳ���ʱ��
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


//ʵʱ����
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

	//������
	while(1)
	{
       nErrFlg = 0;
	   nValidMp =ValidMpNum();                             //��Ч������

       //ͼ�α�־
       MearPoint(nMp , 1);                                   //������

       //�ж�ʱ�����Ч��
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
            //��������ʾ
            case 1: //�����й���
		   {
            // ��1��
                nIdex = 0;
                memset(pBuf, 0, sizeof(nBuf));
                //�ж�ʱ����Ч��
                MemReadRealData(nMp, F5, pBuf, sizeof(nBuf)); // �����й�����ʾֵ
                if(nErrFlg)
                    {
                        memset(pBuf, 0xEE, sizeof(nBuf));
                    }
                GuiwinTextOutAuto("%2s%s","","�����й�����ʾֵ");
                GuiwinTextOutAuto("�����й���:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("�����й���:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("�����й���:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("�����й�ƽ:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("�����й���:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("����ʱ��");

                MpRdTime(&nErrFlg ,pVldTime);
                break;
            }
            case 2: //�����й���
            {
            // ��2��
                nIdex = 0;
            memset(pBuf, 0, sizeof(nBuf));
                //�ж�ʱ����Ч��
                MemReadRealData(nMp, F6, pBuf, sizeof(nBuf)); // �����й�����ʾֵ
                if(nErrFlg)
                {
                    memset(pBuf, 0xEE, sizeof(nBuf));
                }
            GuiwinTextOutAuto("%2s%s","","�����й�����ʾֵ");
                GuiwinTextOutAuto("�����й���:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("�����й���:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("�����й���:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("�����й�ƽ:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("�����й���:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
            GuiwinTextOutAuto("����ʱ��");

                MpRdTime(&nErrFlg ,pVldTime);
                break;
            }
            case 3: //�����޹���
            {
            // ��3��
                nIdex = 0 ;
            memset(pBuf, 0, sizeof(nBuf));
                //�ж�ʱ����Ч��
                MemReadRealData(nMp, F7, pBuf, sizeof(nBuf)); // �����޹�����ʾֵ
                if(nErrFlg)
            {
                memset(pBuf, 0xEE, sizeof(nBuf));
            }
            GuiwinTextOutAuto("%2s%s","","�����޹�����ʾֵ");
                GuiwinTextOutAuto("�����޹���:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("�����޹���:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("�����޹���:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("�����޹�ƽ:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("�����޹���:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
            GuiwinTextOutAuto("����ʱ��");

                MpRdTime(&nErrFlg ,pVldTime);
                break;
            }
            case 4: //�����޹���
            {
            //��4��
                nIdex = 0 ;
            memset(pBuf, 0, sizeof(nBuf));
                //�ж�ʱ����Ч��
                MemReadRealData(nMp, F8, pBuf, sizeof(nBuf)); // �����޹�����ʾֵ
                if(nErrFlg)
            {
                memset(pBuf, 0xEE, sizeof(nBuf));
            }
            GuiwinTextOutAuto("%2s%s","","�����޹�����ʾֵ");
                GuiwinTextOutAuto("�����޹���:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("�����޹���:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("�����޹���:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("�����޹�ƽ:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("�����޹���:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
            GuiwinTextOutAuto("����ʱ��");

                MpRdTime(&nErrFlg ,pVldTime);
                break;
            }
            case 5: //һ�����޹�����ʾֵ
            {
		        //��4��
                nIdex = 0 ;
            memset(pBuf, 0, sizeof(nBuf));
                //�ж�ʱ����Ч��
                MemReadRealData(nMp, F9, pBuf, sizeof(nBuf)); // һ�����޹�����ʾֵ
                if(nErrFlg)
            {
                memset(pBuf, 0xEE, sizeof(nBuf));
            }
            GuiwinTextOutAuto("%1s%s","","һ�����޹�����ʾֵ");
                GuiwinTextOutAuto("��:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("��:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("��:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("ƽ:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("��:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
            GuiwinTextOutAuto("����ʱ��");

                MpRdTime(&nErrFlg ,pVldTime);
                break;
            }
            case 6: //�������޹�����ʾֵ
            {
		    //��6��
                nIdex = 0 ;
            memset(pBuf, 0, sizeof(nBuf));
                //�ж�ʱ����Ч��
                MemReadRealData(nMp, F10, pBuf, sizeof(nBuf)); // �������޹�����ʾֵ
                if(nErrFlg)
            {
                memset(pBuf, 0xEE, sizeof(nBuf));
            }
            GuiwinTextOutAuto("%1s%s","","�������޹�����ʾֵ");
                GuiwinTextOutAuto("��:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("��:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("��:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("ƽ:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("��:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
            GuiwinTextOutAuto("����ʱ��");

                MpRdTime(&nErrFlg ,pVldTime);
                break;
            }
            case 7: //�������޹�����ʾֵ
            {
		    //��7��
                nIdex = 0 ;
            memset(pBuf, 0, sizeof(nBuf));
                //�ж�ʱ����Ч��
                MemReadRealData(nMp, F11, pBuf, sizeof(nBuf)); // �������޹�����ʾֵ
                if(nErrFlg)
            {
                memset(pBuf, 0xEE, sizeof(nBuf));
            }
            GuiwinTextOutAuto("%1s%s","","�������޹�����ʾֵ");
                GuiwinTextOutAuto("��:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("��:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("��:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("ƽ:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("��:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
            GuiwinTextOutAuto("����ʱ��");

                MpRdTime(&nErrFlg ,pVldTime);
                break;
            }
            case 8: //�������޹�����ʾֵ
            {
		    //��8��
                nIdex = 0 ;
            memset(pBuf, 0, sizeof(nBuf));
                //�ж�ʱ����Ч��
                MemReadRealData(nMp, F12, pBuf, sizeof(nBuf)); // �������޹�����ʾֵ
                if(nErrFlg)
            {
                memset(pBuf, 0xEE, sizeof(nBuf));
            }
            GuiwinTextOutAuto("%1s%s","","�������޹�����ʾֵ");
                GuiwinTextOutAuto("��:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("��:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("��:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("ƽ:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("��:%8s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
            GuiwinTextOutAuto("����ʱ��");

                MpRdTime(&nErrFlg ,pVldTime);
                break;
            }
            case 9: //�������������ʱ��
            {
		    //��9��
                nIdex = 0;
            memset(pBuf, 0, sizeof(nBuf));
                //�ж�ʱ����Ч��
                MemReadRealData(nMp, F13, pBuf, sizeof(nBuf)); // �������������ʱ��
                if(nErrFlg)
            {
                memset(pBuf, 0xEE, sizeof(nBuf));
            }
            GuiwinTextOutAuto("%1s%s","","�������������ʱ��");
            GuiwinTextOutAuto("�����й���(KW)");
                GuiwinTextOutAuto("%11s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("%3s%02x��%02x��%02xʱ%02x��","",pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
            GuiwinTextOutAuto("");

                nIdex = 0;
            memset(pBuf, 0, sizeof(nBuf));
                //�ж�ʱ����Ч��
                MemReadRealData(nMp, F14, pBuf, sizeof(nBuf));
                if(nErrFlg)
            {
                memset(pBuf, 0xEE, sizeof(nBuf));
            }
            GuiwinTextOutAuto("�����й���(KW)");
                GuiwinTextOutAuto("%11s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                nIdex += 4;
                GuiwinTextOutAuto("%3s%02x��%02x��%02xʱ%02x��","",pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                break;
            }
            case 10: //�����ѹ���������
            {
		    //��10��
                nIdex = 0;
            memset(pBuf, 0, sizeof(nBuf));
                //�ж�ʱ����Ч��
                MemReadRealData(nMp, F17, pBuf, sizeof(nBuf));
                if(nErrFlg)
            {
                memset(pBuf, 0xEE, sizeof(nBuf));
            }
                BcdToAscii(&pBuf[nIdex], 6, TRUE, &nStr[0]);
                nIdex = 8;
                GuiwinTextOutAuto("�����ѹ%6s%c%c%c.%c","Ua:",nStr[nIdex],nStr[nIdex + 1],nStr[nIdex + 2],nStr[nIdex + 3]);
                nIdex -= 4;
                GuiwinTextOutAuto("%14s%c%c%c.%c","Ub:",nStr[nIdex],nStr[nIdex + 1],nStr[nIdex + 2],nStr[nIdex + 3]);
                nIdex -= 4;
                GuiwinTextOutAuto("%14s%c%c%c.%c","Uc:",nStr[nIdex],nStr[nIdex + 1],nStr[nIdex + 2],nStr[nIdex + 3]);

                nIdex = 0;
            memset(pBuf, 0, sizeof(nBuf));
                //�ж�ʱ����Ч��
                MemReadRealData(nMp, F18, pBuf, sizeof(nBuf));
                if(nErrFlg)
            {
                memset(pBuf, 0xEE, sizeof(nBuf));
            }
                DrawFormat25((pFORMAT25)&pBuf[0], &nDispBuf[0]);
                GuiwinTextOutAuto("������� Ia:%s",nDispBuf);    

                DrawFormat25((pFORMAT25)&pBuf[3], &nDispBuf[0]);
                GuiwinTextOutAuto("%9sIb:%s","",nDispBuf);
                
                DrawFormat25((pFORMAT25)&pBuf[6], &nDispBuf[0]);
                GuiwinTextOutAuto("%9sIc:%s","",nDispBuf);
                
            GuiwinTextOutAuto("����ʱ��");

                MpRdTime(&nErrFlg ,pVldTime);
                break;
            }
            case 11: //�й�����
            {
		    //��11��
                nIdex = 0;
            memset(pBuf, 0, sizeof(nBuf));
                //�ж�ʱ����Ч��
                MemReadRealData(nMp, F19, pBuf, sizeof(nBuf));
                if(nErrFlg)
            {
                memset(pBuf, 0xEE, sizeof(nBuf));
            }
            GuiwinTextOutAuto("%-s","�й�����");
            GuiwinTextOutAuto("");
            GuiwinTextOutAuto("%4s��:%1s%02x.%02x%02x","","",pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
            nIdex += 3;
            GuiwinTextOutAuto("%4sA:%2s%02x.%02x%02x","","",pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
            nIdex += 3;
            GuiwinTextOutAuto("%4sB:%2s%02x.%02x%02x","","",pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
            nIdex += 3;
            GuiwinTextOutAuto("%4sC:%2s%02x.%02x%02x","","",pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
            nIdex += 3;
            GuiwinTextOutAuto("%6s%-s","","��λ(KW)");
            GuiwinTextOutAuto("");
                break;
            }
            case 12: //�޹�����
            {
		    //��12��
                nIdex = 0;
            memset(pBuf, 0, sizeof(nBuf));
                //�ж�ʱ����Ч��
                MemReadRealData(nMp, F20, pBuf, sizeof(nBuf));
                if(nErrFlg)
            {
                memset(pBuf, 0xEE, sizeof(nBuf));
            }
            GuiwinTextOutAuto("%-s","�޹�����");
            GuiwinTextOutAuto("");
            GuiwinTextOutAuto("%4s��:%1s%02x.%02x%02x","","",pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
            nIdex += 3;
            GuiwinTextOutAuto("%4sA:%2s%02x.%02x%02x","","",pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
            nIdex += 3;
            GuiwinTextOutAuto("%4sB:%2s%02x.%02x%02x","","",pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
            nIdex += 3;
            GuiwinTextOutAuto("%4sC:%2s%02x.%02x%02x","","",pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
            nIdex += 3;
            GuiwinTextOutAuto("%5s%-s","","��λ(Kvar)");
            GuiwinTextOutAuto("");
                break;
            }
            case 13: //���ڹ���
            {
		    //��13��
                nIdex = 0;
            memset(pBuf, 0, sizeof(nBuf));
                //�ж�ʱ����Ч��
                MemReadRealData(nMp, F21, pBuf, sizeof(nBuf));
                if(nErrFlg)
            {
                memset(pBuf, 0xEE, sizeof(nBuf));
            }

                GuiwinTextOutAuto("%-s","���ڹ���");
            GuiwinTextOutAuto("");
            GuiwinTextOutAuto("%4s��:%1s%02x.%02x%02x","","",pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
            nIdex += 3;
            GuiwinTextOutAuto("%4sA:%2s%02x.%02x%02x","","",pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
            nIdex += 3;
            GuiwinTextOutAuto("%4sB:%2s%02x.%02x%02x","","",pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
            nIdex += 3;
            GuiwinTextOutAuto("%4sC:%2s%02x.%02x%02x","","",pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
            nIdex += 3;
            GuiwinTextOutAuto("����ʱ��");

                MpRdTime(&nErrFlg ,pVldTime);
                break;
            }
            case 14:
            {
		    //��14��
                nIdex = 0;
            memset(pBuf, 0, sizeof(nBuf));
                //�ж�ʱ����Ч��
                MemReadRealData(nMp, F22, pBuf, sizeof(nBuf));
                if(nErrFlg)
            {
                memset(pBuf, 0xEE, sizeof(nBuf));
            }
                BcdToAscii(&pBuf[nIdex], 8, TRUE, &nStr[0]);
                nIdex = 12;
            GuiwinTextOutAuto("%-s","��������");
            GuiwinTextOutAuto("");
                GuiwinTextOutAuto("%4s��:%6s%c.%c%c%c","","",nStr[nIdex] ,nStr[nIdex + 1],nStr[nIdex + 2],nStr[nIdex + 3]);
                nIdex -= 4;
                GuiwinTextOutAuto("%4sA:%7s%c.%c%c%c","","", nStr[nIdex] ,nStr[nIdex + 1],nStr[nIdex + 2],nStr[nIdex + 3]);
                nIdex -= 4;
                GuiwinTextOutAuto("%4sB:%7s%c.%c%c%c","","", nStr[nIdex] ,nStr[nIdex + 1],nStr[nIdex + 2],nStr[nIdex + 3]);
                nIdex -= 4;
                GuiwinTextOutAuto("%4sC:%7s%c.%c%c%c","","", nStr[nIdex] ,nStr[nIdex + 1],nStr[nIdex + 2],nStr[nIdex + 3]);
            GuiwinTextOutAuto("����ʱ��");

                MpRdTime(&nErrFlg ,pVldTime);
                break;
                }
            }
       GuiwinAutoDispProc(NULL , NULL , 0 , (INT8U*)1);                     //���Խ�����
       
       nFlg = KeyProc(&nStepCnt , &nValidMp , &nMp , 0);                    //��������
       if (nFlg)
            {
            MearPoint(nValidMp , 0);
            break;
            }
    }
}

//������
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
		if (GuiwinSetParamMenu("��ѡ���ѯ����", pParamSet) >= 0)
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
			   nValidMp =ValidMpNum();                                  //��Ч������
		       //ͼ�α�־
	           MearPoint(nMp , 1);                                            //������

                memset(pBuf, 0, sizeof(nBuf));
                nSeq = GetSeqFromMp(nMp);
                if (GetMeterInfo(nSeq, pMeterInfo) < 0)
                {
                    nErrFlg = 1;
                }
                MemReadDayData(nSeq, F1, &pTime->nDay, &pBuf[0], 6);
                //�ж��Ƿ�Ϊ��Ч��ַ
                if (0 != memcmp(&pBuf[0], &pMeterInfo->nMeterAddr[0],6))
                {
                    nErrFlg = 1;
                }

                //����ʱ��
                memset(pBuf, 0, sizeof(nBuf));
                memset(pVldTime , 0 ,sizeof(STD_TIME));
                MemReadDayData(nMp, F3, &pTime->nDay ,pBuf, sizeof(nBuf));
                memcpy(pVldTime ,pBuf, sizeof(STD_TIME));

                memset(pBuf, 0, sizeof(nBuf));
                if(MemReadDayData(nMp, F2, &pTime->nDay ,&pBuf[0], 3) < 0)      //ʱ��(��־)YY-MM-DD
                {
                   nErrFlg = 1;
                }
                //�ȶ�ʱ���־

                if((0 != memcmp(&pBuf[0], &pTime->nDay, 3)) || (!IsVldStdTime(pVldTime)))
                {
                    nErrFlg = 1;
                }
		       switch (nStepCnt)
		       {
		            //��������ʾ
		            case 1: //�����й���
                    {
			// ��1��
                        nIdex = 0;
            memset(pBuf, 0, sizeof(nBuf));
                        //�ж�ʱ����Ч��
                        MemReadDayData(nMp, F4, &pTime->nDay, pBuf, sizeof(nBuf)); // �����й�����ʾֵ
                        if(nErrFlg)
                        {
                          memset(pBuf , 0xEE ,sizeof(nBuf));
                        }
            GuiwinTextOutAuto("%2s%s","","�����й�����ʾֵ");
                        GuiwinTextOutAuto("�����й���:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("�����й���:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("�����й���:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("�����й�ƽ:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("�����й���:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
            GuiwinTextOutAuto("����ʱ��");

                        MpRdTime(&nErrFlg ,pVldTime);
		                break;
		            }
		            case 2: //�����й���
		            {
            // ��2��
                        nIdex = 0;
            memset(pBuf, 0, sizeof(nBuf));
                        //�ж�ʱ����Ч��
                        MemReadDayData(nMp, F5, &pTime->nDay, pBuf, sizeof(nBuf)); // �����й�����ʾֵ
                        if(nErrFlg)
                        {
                          memset(pBuf , 0xEE ,sizeof(nBuf));
                        }
            GuiwinTextOutAuto("%2s%s","","�����й�����ʾֵ");
                        GuiwinTextOutAuto("�����й���:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("�����й���:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("�����й���:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("�����й�ƽ:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("�����й���:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
            GuiwinTextOutAuto("����ʱ��");

                        MpRdTime(&nErrFlg ,pVldTime);
		                break;
		            }
		            case 3: //�������������ʱ��
		            {
		                //��9��
                        nIdex = 0;
            memset(pBuf, 0, sizeof(nBuf));
                        //�ж�ʱ����Ч��
                        MemReadDayData(nMp, F12, &pTime->nDay, pBuf, sizeof(nBuf)); // �����й�����ʾֵ
                        if(nErrFlg)
                        {
                          memset(pBuf , 0xEE ,sizeof(nBuf));
                        }
            GuiwinTextOutAuto("%1s%s","","�������������ʱ��");
            GuiwinTextOutAuto("�����й���(KW)");
                        GuiwinTextOutAuto("%11s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("%3s%02x��%02x��%02xʱ%02x��","",pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
            GuiwinTextOutAuto("");

                        nIdex = 0;
            memset(pBuf, 0, sizeof(nBuf));
                        //�ж�ʱ����Ч��
                        MemReadDayData(nMp, F13, &pTime->nDay, pBuf, sizeof(nBuf)); // �����й�����ʾֵ
                        if(nErrFlg)
                        {
                          memset(pBuf , 0xEE ,sizeof(nBuf));
                        }
            GuiwinTextOutAuto("�����й���(KW)");
                        GuiwinTextOutAuto("%11s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("%3s%02x��%02x��%02xʱ%02x��","",pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
		                break;
			}
	            }
               GuiwinAutoDispProc(NULL , NULL , 0 , (INT8U*)1);              //���Խ�����

               nFlg = KeyProc(&nStepCnt , &nValidMp , &nMp , 1);                    //��������
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

//������
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
		if (GuiwinSetParamMenu("��ѡ���ѯ����", pParamSet) >= 0)
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
			   nValidMp =ValidMpNum();                                  //��Ч������
		       //ͼ�α�־
               MearPoint(nMp , 1);                                            //������

                nSeq = GetSeqFromMp(nMp);
                if (GetMeterInfo(nSeq, pMeterInfo) < 0)
                {
                    nErrFlg = 1;
                }
                //�жϵ�ַ
                MemReadMonData(nSeq, F1, &pTime->nMon, &pBuf[0], 6);
                if (0 != memcmp(&pBuf[0], &pMeterInfo->nMeterAddr[0],6))
                {
                    nErrFlg = 1;
                }

                //����ʱ��
                memset(pBuf , 0 ,sizeof(nBuf));
                memset(pVldTime , 0 ,sizeof(STD_TIME));
                MemReadMonData(nMp, F3, &pTime->nMon ,pBuf, sizeof(nBuf)); // ����ʱ��YY-MM-DD-HH-MM-SS
                memcpy(pVldTime ,pBuf, sizeof(STD_TIME));

                //�ж�ʱ��
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
		            //��������ʾ
		            case 1: //�����й���
			{
			// ��1��
                        nIdex = 0;
            memset(pBuf, 0, sizeof(nBuf));
                        //�ж�ʱ����Ч��
                          MemReadMonData(nMp, F4, &pTime->nMon, pBuf, sizeof(nBuf)); // �����й�����ʾֵ
                        if(nErrFlg)
                        {
                          memset(pBuf , 0xEE ,sizeof(nBuf));
                        }
            GuiwinTextOutAuto("%2s%s","","�����й�����ʾֵ");
                        GuiwinTextOutAuto("�����й���:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("�����й���:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("�����й���:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("�����й�ƽ:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("�����й���:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
            GuiwinTextOutAuto("����ʱ��");

                        MpRdTime(&nErrFlg ,pVldTime);
		                break;
		            }
		            case 2: //�����й���
		            {
            // ��2��
                        nIdex = 0;
            memset(pBuf, 0, sizeof(nBuf));
                        //�ж�ʱ����Ч��
                          MemReadMonData(nMp, F5, &pTime->nMon, pBuf, sizeof(nBuf)); // �����й�����ʾֵ
                        if(nErrFlg)
                        {
                          memset(pBuf , 0xEE ,sizeof(nBuf));
                        }
            GuiwinTextOutAuto("%2s%s","","�����й�����ʾֵ");
                        GuiwinTextOutAuto("�����й���:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("�����й���:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("�����й���:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("�����й�ƽ:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("�����й���:%02x%02x%02x.%02x", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
            GuiwinTextOutAuto("����ʱ��");

                        MpRdTime(&nErrFlg ,pVldTime);
		                break;
		            }
		            case 3: //�������������ʱ��
		            {
		                //��9��
                        nIdex = 0;
            memset(pBuf, 0, sizeof(nBuf));
                        //�ж�ʱ����Ч��
                        MemReadMonData(nMp, F12, &pTime->nMon, pBuf, sizeof(nBuf)); // �����й�����ʾֵ
                        if(nErrFlg)
                        {
                          memset(pBuf , 0xEE ,sizeof(nBuf));
                        }
            GuiwinTextOutAuto("%1s%s","","�������������ʱ��");
            GuiwinTextOutAuto("�����й���(KW)");
                        GuiwinTextOutAuto("%11s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("%3s%02x��%02x��%02xʱ%02x��","",pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
            GuiwinTextOutAuto("");

                        nIdex = 0;
            memset(pBuf, 0, sizeof(nBuf));
                        //�ж�ʱ����Ч��
                        MemReadMonData(nMp, F13, &pTime->nMon, pBuf, sizeof(nBuf)); // �����й�����ʾֵ
                        if(nErrFlg)
                        {
                          memset(pBuf , 0xEE ,sizeof(nBuf));
                        }
            GuiwinTextOutAuto("�����й���(KW)");
                        GuiwinTextOutAuto("%11s%02x%02x%02x.%02x","", pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
                        nIdex += 4;
                        GuiwinTextOutAuto("%3s%02x��%02x��%02xʱ%02x��","",pBuf[nIdex + 3],pBuf[nIdex + 2],pBuf[nIdex + 1],pBuf[nIdex]);
		                break;
			}
			}
               GuiwinAutoDispProc(NULL , NULL , 0 , (INT8U*)1);              //���Խ�����

               nFlg = KeyProc(&nStepCnt , &nValidMp , &nMp , 1);                    //��������
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

//ͨ��ͨ����ѯ����ͨѶ��ʽ
void CommunicationMode(void)
{
    INT8U                nBuf[20]                   = {0};
    INT8U*               pBuf                        = &nBuf[0];

    pBuf = &nBuf[0];
    memset(pBuf, 0, sizeof(nBuf));
    MemReadCommPrm(F1, &nBuf[0], sizeof(nBuf));
    GuiwinTextOutAuto("�ط�����:%5s%02x", "",((pBuf[3] >> 4) & 0x03));
    GuiwinTextOutAuto("��������:%5s%02d", "",pBuf[5]);

    pBuf = &nBuf[0];
    memset(pBuf, 0, sizeof(nBuf));
    MemReadCommPrm(F8, &nBuf[0], sizeof(nBuf));
    GuiwinTextOutAuto("�ز�����:%5s%03x", "",pBuf[3]);
    GuiwinTextOutAuto("�ز����:%5s%05d", "",pBuf[2] * 256 + pBuf[1]);
    GuiwinTextOutAuto("��ͨѶʱ��:%3s%03d", "",pBuf[4]);

    switch(nBuf[0]&0x03)
    {
        case 1:
            strcpy((char *)nBuf, "��������");
            break;
        case 2:
            strcpy((char *)nBuf, "��������");
            break;
        case 3:
            strcpy((char *)nBuf, "ʱ������");
            break;
        default:
            strcpy((char *)nBuf, "Ĭ��");
            break;
    }
    GuiwinTextOutAuto("���߷�ʽ:%3s%s","",nBuf);
    GuiwinAutoDispProc("ͨѶ��ʽ" , NULL , 0 , NULL);
}

//ͨ��ͨ����ȡ������������
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

    //��BCD�벻��ӡ-------��վ����
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
    //��ӡ��վ����
    memcpy(nStr ,pData ,sizeof(nStr));
    GuiwinTextOutAuto("��վ����");
    GuiwinTextOutAuto("%3s%s","",nStr);

    //��BCD�벻��ӡ-------��������
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
    //��ӡ��������
    memcpy(nStr ,pData ,sizeof(nStr));
    GuiwinTextOutAuto("��������");
    GuiwinTextOutAuto("%3s%s","",nStr);

    GuiwinTextOutAuto("�û���:");
    GuiwinTextOutAuto("����:");

    GuiwinAutoDispProc("��������" , NULL , 0 , NULL);

}

//ͨ��ͨ����ȡ������վIP��ַ
void MainIpAddr(void)
{
    INT8U                nBuf[30]                   = {0};
    INT8U*               pBuf                       = &nBuf[0];
    INT8U                nAPN[]                     = {0};

    pBuf = &nBuf[0];
    memset(pBuf, 0, sizeof(nBuf));
    MemReadCommPrm(F3, &nBuf[0], sizeof(nBuf));

    GuiwinTextOutAuto("����IP");
    GuiwinTextOutAuto("%4s%03d.%03d.%03d.%03d","",pBuf[0],pBuf[1],pBuf[2],pBuf[3]);
    GuiwinTextOutAuto("���ö˿�: %05d", pBuf[5] * 256 + pBuf[4]);
    GuiwinTextOutAuto("����IP");
    GuiwinTextOutAuto("%4s%03d.%03d.%03d.%03d","",pBuf[6],pBuf[7],pBuf[8],pBuf[9]);
    GuiwinTextOutAuto("���ö˿�: %05d", pBuf[11] * 256 + pBuf[10]);
    memcpy(nAPN ,&pBuf[12],30);
    GuiwinTextOutAuto("APN: %s", nAPN);
    GuiwinAutoDispProc(NULL , NULL , 0 , NULL);
}
//ͨ��ͨ����ȡ������̫������
void EthernetPara(void)
{
    INT8U                nBuf[70]                   = {0};
    INT8U*               pBuf                       = &nBuf[0];
    pBuf = &nBuf[0];
    memset(pBuf, 0, sizeof(nBuf));
    MemReadCommPrm(F7, &nBuf[0], sizeof(nBuf));

    GuiwinTextOutAuto("�ն�IP");
    GuiwinTextOutAuto("%4s%03d.%03d.%03d.%03d","",pBuf[0],pBuf[1],pBuf[2],pBuf[3]);
    GuiwinTextOutAuto("��������");
    GuiwinTextOutAuto("%4s%03d.%03d.%03d.%03d","",pBuf[4],pBuf[5],pBuf[6],pBuf[7]);
    GuiwinTextOutAuto("���ص�ַ");
    GuiwinTextOutAuto("%4s%03d.%03d.%03d.%03d","",pBuf[8],pBuf[9],pBuf[10],pBuf[11]);
    GuiwinTextOutAuto("�����˿�: %05d", pBuf[61] * 256 + pBuf[60]);

    pBuf = &nBuf[0];
    memset(&nBuf[0], 0, sizeof(nBuf));
    MemReadCommPrm(F8, &nBuf[0], sizeof(nBuf));
    //����ģʽ
    switch(((nBuf[0] >> 7) & 0x01))
    {
        case 0:
            strcpy((char *)nBuf, "TCP");
            break;
        case 1:
            strcpy((char *)nBuf, "UDP");
            break;
    }
    GuiwinTextOutAuto("����ģʽ:%1s%s","",nBuf);
    //����ģʽ
    pBuf = &nBuf[0];
    memset(&nBuf[0], 0, sizeof(nBuf));
    MemReadCommPrm(F8, &nBuf[0], sizeof(nBuf));
    switch((pBuf[0] >> 4) & 0x03)
    {
        case 0:
            strcpy((char *)nBuf, "���ģʽ");
            break;
        case 1:
            strcpy((char *)nBuf, "�ͻ���ģʽ");
            break;
        case 2:
            strcpy((char *)nBuf, "������ģʽ");
            break;
        default:
            strcpy((char *)nBuf, "��Ч");
            break;
    }
    GuiwinTextOutAuto("����ģʽ:");
    GuiwinTextOutAuto("%10s%s","", nBuf);
    GuiwinAutoDispProc(NULL , NULL , 0 , NULL);
}

//���ܱ����
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

    GuiwinTextOutAuto("�������:%3s%04d","", nMearpoint);

    if (nSwMpFlg == 1)
    {
	//������˿�
    switch(pData[Index + 5]&0x1F)
    {
        case 1:
            strcpy((char *)RecvBuf, "���ɿ�");
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
            strcpy((char *)RecvBuf, "����RS485");
            break;
        case 31:
            strcpy((char *)RecvBuf, "�ز���");
            break;
        default:
            strcpy((char *)RecvBuf, "��Ч");
            break;
    }
    GuiwinTextOutAuto("ͨ�Ŷ˿�:%2s%s","", RecvBuf);

	//������
    nBaud = Buf2Long(&pData[Index + 6], 4);
    GuiwinTextOutAuto("ͨ������:%3s%d","", nBaud);

    //������ͨ�Ź�Լ
    memset(&RecvBuf[0], 0, sizeof(RecvBuf));
    switch(pData[Index + 10])
    {
        case 0:
            strcpy((char *)RecvBuf, "��Ч");
            break;
        case 1:
            strcpy((char *)RecvBuf, "DL/T 645-1997");
            break;
        case 2:
            strcpy((char *)RecvBuf, "��������Э��");
            break;
        case 30:
            strcpy((char *)RecvBuf, "DL/T 645-2007");
            break;
        case 31:
            strcpy((char *)RecvBuf, "�����ز�Э��");
            break;
        default:
            strcpy((char *)RecvBuf, "δ֪");
            break;
    }
    GuiwinTextOutAuto("Э��:  %s",RecvBuf);

    Index = 0;
        GuiwinTextOutAuto("����ַ:");
        GuiwinTextOutAuto("      %02x%02x%02x%02x%02x%02x",pData[Index+16], pData[Index+15],
                   pData[Index+14], pData[Index+13], pData[Index+12], pData[Index+11]);

    Index = 0;
        GuiwinTextOutAuto("�ɼ���:");
        GuiwinTextOutAuto("      %02x%02x%02x%02x%02x%02x", pData[Index+31], pData[Index+30],
                    pData[Index+29], pData[Index+28], pData[Index+27], pData[Index+26]);
    }
    else
    {
    memset(&RecvBuf[0], 0, sizeof(RecvBuf));
    switch(pData[Index + 32])
    {
        case 1:
                strcpy((char *)RecvBuf, "����ר���û�");
            break;
        case 2:
                strcpy((char *)RecvBuf, "��С��ר���û�");
            break;
        case 3:
                strcpy((char *)RecvBuf, "���๤��ҵ�û�");
            break;
        case 4:
                strcpy((char *)RecvBuf, "���๤��ҵ�û�");
            break;
        case 5:
                strcpy((char *)RecvBuf, "�����û�");
            break;
        case 6:
                strcpy((char *)RecvBuf, "��������");
            break;
        default:
                strcpy((char *)RecvBuf, "Ĭ���û�");
            break;
    }
        GuiwinTextOutAuto("�û�����:");
        GuiwinTextOutAuto("%s",RecvBuf);

		if(pData[Index + 32] == 3)
    {
            GuiwinTextOutAuto("�û�С��:");
			GuiwinTextOutAuto("%s",c_PartTypeC[pData[Index + 33]]);
    }
		else if(pData[Index + 32] == 4)
		{
			GuiwinTextOutAuto("�û�С��:");
			GuiwinTextOutAuto("%s",c_PartTypeD[pData[Index + 33]]);
		}
		else if(pData[Index + 32] == 5)
		{
			GuiwinTextOutAuto("�û�С��:");
			GuiwinTextOutAuto("%s",c_PartTypeE[pData[Index + 33]]);
		}
		else if(pData[Index + 32] == 6)
		{
			GuiwinTextOutAuto("�û�С��:");
			GuiwinTextOutAuto("%s",c_PartTypeF[pData[Index + 33]]);
    }
    else
    {
            GuiwinTextOutAuto("�û�С��:");
			GuiwinTextOutAuto("%s", c_PartTypeAB[0]);
		}

        memset(pData , 0 ,sizeof(nBuf));
        MemReadMeterPrm(pDatanum[nMearpoint],F25 ,pData ,sizeof(nBuf));

        GuiwinTextOutAuto("��ѹ����:%3s%d","", Buf2Long(&pData[0] , 2));
        GuiwinTextOutAuto("��������:%3s%d","", Buf2Long(&pData[2] , 2));
    }
    GuiwinAutoDispProc(NULL, NULL, 0, (INT8U*)1);   //����Ҫ�ȴ�������
}

//�ն�ʱ�ӵĲ�ѯ
void ConCerTimequery(void)
{
    STD_TIME		     sTime;
	pSTD_TIME		     pTime                       = &sTime;

    memset(pTime, 0, sizeof(STD_TIME));
    RTCDateTimeRead(pTime);

    GuiwinTextOutAuto("%6s�ն�ʱ��","");
    GuiwinTextOutAuto("");
    GuiwinTextOutAuto("%4s%02x��%02x��%02x��","",pTime->nYear,pTime->nMon,pTime->nDay );
    GuiwinTextOutAuto("");
    GuiwinTextOutAuto("%4s%02xʱ%02x��%02x��","",pTime->nHour,pTime->nMin,pTime->nSec);
    GuiwinAutoDispProc(NULL, NULL, 0, NULL);
}

//�ն˱�Ų�ѯ
void ConcerNumQuery(void)
{
    INT8U			     nBuf[10]		             = {0};
	INT8U*			     pBuf			             = &nBuf[0];

    pBuf = &nBuf[0];
    memset(pBuf, 0, sizeof(nBuf));
    MemReadCommPrm(F94, &nBuf[0], sizeof(nBuf));

    GuiwinTextOutAuto("");
    GuiwinTextOutAuto("%6s�ն˱���","");
    GuiwinTextOutAuto("");
    GuiwinTextOutAuto("��������:%4s%02x%02x","", pBuf[1], pBuf[0]);
    GuiwinTextOutAuto("");
    GuiwinTextOutAuto("�ն˵�ַ:");
    GuiwinTextOutAuto("%4s%02x%02x (ʮ������)","", pBuf[3], pBuf[2]);
    GuiwinTextOutAuto("%4s%05d   (ʮ����)","", Buf2INT16U(&nBuf[2]));

    GuiwinAutoDispProc(NULL, NULL, 0, NULL);

}

//ң��״̬
void RemoteCommState(void)
{


}

//�¼���¼
void EventLog(void)
{

}

//�ն���Ϣ
void ConcerInfo(void)
{
    INT8U			     nBuf[100]		             = {0};
	INT8U*			     pBuf			             = &nBuf[0];
    STD_TIME		     sTime;
	pSTD_TIME		     pTime                       = &sTime;

    pBuf = &nBuf[0];
    memset(pBuf, 0, sizeof(nBuf));
    MemReadCommPrm(F94, &nBuf[0], sizeof(nBuf));

    GuiwinTextOutAuto("��������:%02x%02x", pBuf[1], pBuf[0]);
    GuiwinTextOutAuto("�ն˵�ַ:%02x%02x(H)",pBuf[3], pBuf[2]);
    GuiwinTextOutAuto("�ն˵�ַ:%05d(D)", Buf2INT16U(&nBuf[2]));

	memset(pTime, 0, sizeof(STD_TIME));
	GetSysClk(pTime);
    GuiwinTextOutAuto("�ն�����:%02x/%02x/%02x",pTime->nYear,pTime->nMon,pTime->nDay);

    pBuf = &nBuf[0];
    memset(pBuf, 0, sizeof(nBuf));
    MemReadCommPrm(F7, &nBuf[0], sizeof(nBuf));

    GuiwinTextOutAuto("�ն�IP");
    GuiwinTextOutAuto("%4s%03d.%03d.%03d.%03d","",pBuf[0],pBuf[1],pBuf[2],pBuf[3]);
	GuiwinTextOutAuto("�ն˶˿�:%d", Buf2INT16U(&pBuf[62]));

	pBuf = &nBuf[0];
    memset(pBuf, 0, sizeof(nBuf));
	strcat((char*)pBuf, (char*)"����汾:V");
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
	strcat((char*)pBuf, (char*)"�������:");
	strcat((char*)pBuf, (char*)DATE_INFO);
	GuiwinTextOutAuto("%s", pBuf);
	
	pBuf = &nBuf[0];
    memset(pBuf, 0, sizeof(nBuf));
    MemReadCommPrm(F95, &nBuf[0], sizeof(nBuf));

    GuiwinTextOutAuto("�ʲ���:%02x%02x%02x%02x", pBuf[1], pBuf[0], pBuf[3], pBuf[2]);

    GuiwinAutoDispProc("�ն���Ϣ", NULL, 0, NULL);
}

//ͨ��ͨ�����á���ͨ�ŷ�ʽ
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
    const INT8S*	nOnlineMode[]			    ={"��Ч","��������", "��������","ʱ������", 0};
    INT8S           nLineMode                   = 0;
	INT8U           nRet                          = 0;

    InterfacePassword();

    nIndex = 0;
    pParamSet = &sParamSet[0];
    memset(&sParamSet[0], 0, sizeof(sParamSet));

    pBuf = &nBuf[0];
        memset(&nBuf[0], 0, sizeof(nBuf));
    MemReadCommPrm(F1, &nBuf[0], sizeof(nBuf));

    nIndex = 0;                                                     // ת��   nBuf �� ip  ----> pData  �ط�����
    memset(pData, 0, sizeof(nData));
    nLen = ValToAscii(&pData[nIndex], (nBuf[3] >> 4)&0x03, 3);
    nIndex += nLen ;
    GuiwinSetEditCfgParam(&pParamSet, "�ط�����:     ",(INT8S*)pData, 3);

    nIndex = 0;                                                     // ת��   nBuf �� port  ----> pData  ��������
    memset(pData, 0, sizeof(nData));
    nLen = ValToAscii(&pData[nIndex], nBuf[5], 5);
    nIndex += nLen ;
    GuiwinSetEditCfgParam(&pParamSet, "��������:     ",(INT8S*)pData, 5);

    pBuf1 = &nBuf1[0];
    memset(&nBuf1[0], 0, sizeof(nBuf1));
    MemReadCommPrm(F8, &nBuf1[0], sizeof(nBuf1));

    nIndex = 0;                                                     // ת��   nBuf �� port  ----> pData  �ز����
    memset(pData, 0, sizeof(nData));
    nLen = ValToAscii(&pData[nIndex], Buf2INT16U(&nBuf1[1]), 5);
    nIndex += nLen ;
    GuiwinSetEditCfgParam(&pParamSet, "�ز����:     ",(INT8S*)pData, 5);

    nIndex = 0;
    memset(pData, 0, sizeof(nData));                                // ת��   nBuf �� port  ----> pData  �ز�����
    nLen = ValToAscii(&pData[nIndex], nBuf1[3], 3);
    GuiwinSetEditCfgParam(&pParamSet, "�ز�����:     ",(INT8S*)pData, 3);

    nIndex = 0;                                                    // ת��   nBuf �� port  ----> pData  ��ͨѶʱ��
    memset(pData, 0, sizeof(nData));
    nLen = ValToAscii(&pData[nIndex], nBuf1[4], 3);
    nIndex += nLen ;
    GuiwinSetEditCfgParam(&pParamSet, "��ͨѶʱ��:   ",(INT8S*)pData, 3);

    nLineMode = nBuf1[0] &0x03;                                    // ת��   nBuf �� port  ----> pData  ����ģʽ
    GuiwinSetSelectCfgParam(&pParamSet, "���߷�ʽ:", (INT8S*)&nLineMode, nOnlineMode);

    pParamSet = &sParamSet[0];
    while(1)
    {
        if (GuiwinSetParamMenu("", pParamSet) >= 0)
        {
			pBuf = &nBuf[0];
            memset(pBuf, 0, sizeof(nBuf));
            MemReadCommPrm(F1, &nBuf[0], sizeof(nBuf));

            //�ط�����
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[0].nEditBuf, GET_MIN(sizeof(pParamSet[0].nEditBuf), sizeof(nData) - 1));
            pBuf[3]&= 0xCF;
            pBuf[3]|= (INT8U)(atoi((char *)pData)<<4);

            //��������
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[1].nEditBuf, GET_MIN(sizeof(pParamSet[1].nEditBuf), sizeof(nData) - 1));
            pBuf[5] = (INT8U)( atoi((char *)pData) & 0x00FF );

            pBuf1 = &nBuf1[0];
            memset(pBuf1, 0, sizeof(nBuf1));
            MemReadCommPrm(F8, &nBuf1[0], sizeof(nBuf));
            //���߷�ʽ
            memset(pData, 0, sizeof(nData));
            pBuf1[0] &= 0xFC;
            pBuf1[0] |= nLineMode;

           //�ز����
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[2].nEditBuf, GET_MIN(sizeof(pParamSet[2].nEditBuf), sizeof(nData) - 1));
            pBuf1[1] = (INT8U)( atoi((char *)pData) & 0x00FF );
            pBuf1[2] = (INT8U)(( atoi((char *)pData) & 0xFF00 ) >> 8);

            //�ز�����
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[3].nEditBuf, GET_MIN(sizeof(pParamSet[3].nEditBuf), sizeof(nData) - 1));
            pBuf1[3] = (INT8U)(atoi((char *)pData));

            //��ͨѶʱ��
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[4].nEditBuf, GET_MIN(sizeof(pParamSet[4].nEditBuf), sizeof(nData) - 1));
            pBuf1[4] = (INT8U)(atoi((char *)pData));

            nRet = Guiwin1MboxCreate("ȷ��","�Ƿ�ȷ���޸�ͨѶ����?");
            if (KEY_ENTER != nRet)
            {
                 continue;
            }
            MemWriteCommPrm(F1, &nBuf[0], sizeof(nBuf)); //����ͨ�Ų���
            MemWriteCommPrm(F8, &nBuf1[0], sizeof(nBuf1)); //����ͨ�Ų���
        }
        return ;
    }
}

//ͨ��ͨ�����á�����������
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

    //�ж��Ƿ�����ȫΪ0x00
    if(IsAllEqualVal(pBuf, sizeof(nBuf), 0))
    {
        // nCommSetMenu   ----> nBuf
        //��վ����
        nBuf[nIdx++] = 0x13;
        nBuf[nIdx++] = 0x80;
        nBuf[nIdx++] = 0x00;
        nBuf[nIdx++] = 0x00;
        nBuf[nIdx++] = 0x00;
        nBuf[nIdx++] = 0x0F;
        nBuf[nIdx++] = 0xFF;
        nBuf[nIdx++] = 0xFF;
        //��������
        nBuf[nIdx++] = 0x13;
        nBuf[nIdx++] = 0x80;
        nBuf[nIdx++] = 0x00;
        nBuf[nIdx++] = 0x00;
        nBuf[nIdx++] = 0x00;
        nBuf[nIdx++] = 0x0F;
        nBuf[nIdx++] = 0xFF;
        nBuf[nIdx]   = 0xFF;
    }
    //���ö������ĺ���վ�����з�BCD��ľ�ת����0
    nIdx = 0;                                            // ת��   nBuf �� ip  ----> pData  ��վ����
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
    GuiwinSetNullCfgParam(&pParamSet, "��վ����:");
    GuiwinSetEditCfgParam(&pParamSet, "    ", (INT8S*)&pData[0], 16);
    
    nIdx = 0;                                            // ת��   nBuf �� port  ----> pData  ��������
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
    GuiwinSetNullCfgParam(&pParamSet, "��������:");
    GuiwinSetEditCfgParam(&pParamSet, "    ", (INT8S*)&pData[0], 16);
    
    pParamSet = &sParamSet[0];
    while(1)
    {
        if (GuiwinSetParamMenu("ͨ�Ų�������", pParamSet) >= 0)
        {
            nIdx = 0;
            memset(pBuf, 0, sizeof(nBuf));

            //��վ����
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[1].nEditBuf, GET_MIN(sizeof(pParamSet[1].nEditBuf), sizeof(nData) - 1));
            //�ж������Ƿ�ΪBCD��----��BCD��ת��Ϊ0
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

            //��������
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[3].nEditBuf, GET_MIN(sizeof(pParamSet[3].nEditBuf), sizeof(nData) - 1));
            //�ж������Ƿ�ΪBCD��----��BCD��ת��Ϊ0
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

            nRet = Guiwin1MboxCreate("ȷ��","�Ƿ�ȷ���޸�ͨѶ����?");
            if (KEY_ENTER != nRet)
{
                continue;    
            }
            MemWriteCommPrm(F4, &nBuf[0], sizeof(nBuf)); //����ͨ�Ų���
        }
        return ;
            }
        } 
    
//ͨ��ͨ�����á�����վIP����
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
    //�ж��Ƿ�����ȫΪ0x00
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

    nIdx = 0;                                                 // ת��   nBuf �� ip  ----> pData
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
    GuiwinSetNullCfgParam(&pParamSet, "����IP:");
    GuiwinSetEditCfgParam(&pParamSet, "    ", (INT8S*)pData, 15);

    nIdx = 0;                                                 // ת��   nBuf �� port  ----> pData
    memset(pData, 0, sizeof(nData));
    nLen = ValToAscii(&pData[nIdx], Buf2INT16U(&nBuf[4]), 5);
    nIdx += nLen;
    GuiwinSetEditCfgParam(&pParamSet, "���ö˿�:",(INT8S*)pData, 5);

    nIdx = 0;
    // ת��   nBuf �� ip  ----> pData
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
    GuiwinSetNullCfgParam(&pParamSet, "����IP:");
    GuiwinSetEditCfgParam(&pParamSet, "    ", (INT8S*)pData, 15);

    nIdx = 0;                                                 // ת��   nBuf �� port  ----> pData
    memset(pData, 0, sizeof(nData));
    nLen = ValToAscii(&pData[nIdx], Buf2INT16U(&nBuf[10]), 5);
    nIdx += nLen;
    GuiwinSetEditCfgParam(&pParamSet, "���ö˿�:", (INT8S*)pData, 5);
    
    for (i = 0; i < LEN_APN; i++)                                 // ת��   nBuf �� apn  ----> pData
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
    //�ж����pData[0]Ϊ'\0' �����丳Ĭ��ֵjsepc.js ������APN����ʾ��������
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
        if (GuiwinSetParamMenu("ͨ�Ų�������", pParamSet) >= 0)
        {
            nIdx = 0;
            memset(pBuf, 0, sizeof(nBuf));

            //����IP
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[1].nEditBuf, GET_MIN(sizeof(pParamSet[1].nEditBuf), sizeof(nData) - 1));
            pBuf[nIdx++] = atoi(strtok((char *)pData, "."));
            pBuf[nIdx++] = atoi(strtok(0, "."));
            pBuf[nIdx++] = atoi(strtok(0, "."));
            pBuf[nIdx++] = atoi(strtok(0, "."));

            //���ö˿�
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
            for (i = 0; i < LEN_APN - strlen((char *)pData); i++)                    //������00H
            {
                pData[strlen((char *)pData) + i] = 0x00;
            }
            memcpy(&pBuf[nIdx], pData, LEN_APN);

            nRet = Guiwin1MboxCreate("ȷ��","�Ƿ�ȷ���޸�ͨѶ����?");
            if (KEY_ENTER != nRet)
            {
                continue;    
            }
            MemWriteCommPrm(F3, &nBuf[0], sizeof(nBuf)); //����ͨ�Ų���
        } 
					return ;
        }
    }

//ͨ��ͨ�����á�����̫����������
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
    const INT8S*	nWorkModMenu[]			={"���ģʽ", "�ͻ���ģʽ", "������ģʽ", 0};
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
        nBuf[nIdx++] = 0;                 //�ն�IP
        nBuf[nIdx++] = 0;
        nBuf[nIdx++] = 0;
        nBuf[nIdx++] = 0;
        nBuf[nIdx++] = 0;               //��������
        nBuf[nIdx++] = 0;
        nBuf[nIdx++] = 0;
        nBuf[nIdx++] = 0;
        nBuf[nIdx++] = 0;               //���ص�ַ
        nBuf[60] = LOBYTE(5100);
        nBuf[61] = HIBYTE(5100);        //�����˿�
        }
    nIdx = 0;                                              // ת��   nBuf �� ip  ----> pData
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
    GuiwinSetNullCfgParam(&pParamSet, "�ն�IP:");
    GuiwinSetEditCfgParam(&pParamSet, "    ", (INT8S*)pData, 15);

    nIdx = 0;                                              // ת��   nBuf �� ip  ----> pData
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
    GuiwinSetNullCfgParam(&pParamSet, "��������:");
    GuiwinSetEditCfgParam(&pParamSet, "    ", (INT8S*)pData, 15);

    nIdx = 0;                                              // ת��   nBuf �� ip  ----> pData
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
    GuiwinSetNullCfgParam(&pParamSet, "���ص�ַ:");
    GuiwinSetEditCfgParam(&pParamSet, "    ", (INT8S*)pData, 16);

    nIdx = 0;                                              // ת��   nBuf �� port  ----> pData
    memset(pData, 0, sizeof(nData));
    nLen = ValToAscii(&pData[nIdx], Buf2INT16U(&nBuf[60]), 5);
    nIdx += nLen;
    GuiwinSetEditCfgParam(&pParamSet, "�����˿�:",(INT8S*)pData, 5);

    memset(&nBuf1[0], 0, sizeof(nBuf1));
    nRet = MemReadCommPrm(F8, &nBuf1[0], sizeof(nBuf1));

    nIdx = 0;                                              // ת��   nBuf �� ip  ----> pData
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
    GuiwinSetSelectCfgParam(&pParamSet, "����ģʽ:", (INT8S*)&nLinkMod, nLinkModMenu);
    GuiwinSetSelectCfgParam(&pParamSet, "����ģʽ:", (INT8S*)&nWorkMod, nWorkModMenu);

	pParamSet = &sParamSet[0];
    while (1)
    {
        if (GuiwinSetParamMenu("ͨ�Ų�������", pParamSet) >= 0)
			{
            nIdx = 0;
            memset(pBuf, 0, sizeof(nBuf));
            MemReadCommPrm(F7, &nBuf[0], sizeof(nBuf));

            //�ն�IP
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[1].nEditBuf, GET_MIN(sizeof(pParamSet[1].nEditBuf), sizeof(nData) - 1));
            pBuf[nIdx++] = atoi(strtok((char *)pData, "."));
            pBuf[nIdx++] = atoi(strtok(0, "."));
            pBuf[nIdx++] = atoi(strtok(0, "."));
            pBuf[nIdx++] = atoi(strtok(0, "."));

            //��������
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[3].nEditBuf, GET_MIN(sizeof(pParamSet[3].nEditBuf), sizeof(nData) - 1));
            pBuf[nIdx++] = atoi(strtok((char *)pData, "."));
            pBuf[nIdx++] = atoi(strtok(0, "."));
            pBuf[nIdx++] = atoi(strtok(0, "."));
            pBuf[nIdx++] = atoi(strtok(0, "."));

            //����
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[5].nEditBuf, GET_MIN(sizeof(pParamSet[5].nEditBuf), sizeof(nData) - 1));
            pBuf[nIdx++] = atoi(strtok((char *)pData, "."));
            pBuf[nIdx++] = atoi(strtok(0, "."));
            pBuf[nIdx++] = atoi(strtok(0, "."));
            pBuf[nIdx++] = atoi(strtok(0, "."));

            //�����˿�
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[6].nEditBuf, GET_MIN(sizeof(pParamSet[6].nEditBuf), sizeof(nData) - 1));
            pBuf[60] = (INT8U)( atoi((char *)pData) & 0x00FF );
            pBuf[61] = (INT8U)(( atoi((char *)pData) & 0xFF00 ) >> 8);

            pBuf1 =&nBuf1[0];
			memset(&pBuf1[0] , 0 ,sizeof(nBuf1));
            MemReadCommPrm(F8, &nBuf1[0], sizeof(nBuf1));
            //ģʽ
            pBuf1[0] &= 0x03;
            if (nLinkMod)
				{
                pBuf1[0] |= 0x80;
				}
            pBuf1[0] |= (nWorkMod << 4);

            nRet = Guiwin1MboxCreate("ȷ��","�Ƿ�ȷ���޸�ͨѶ����?");
            if (KEY_ENTER != nRet)
			{
                 continue;
			}
            MemWriteCommPrm(F7, &nBuf[0], sizeof(nBuf)); //����ͨ�Ų���
            MemWriteCommPrm(F8, &nBuf1[0], sizeof(nBuf1));
            return ;
		}
        else
        {
        return ;
	}
}
}

//����������
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
    const INT8S*	nCommPort[]			        ={"δ֪","����","RS4851","RS4852","RS4853","����RS485","�ز���",0};
    const INT8S*	CommProtocol[]              ={"��Ч","DL/T645-97","��������Э��","DL/T645-07","խ����ѹ�ز�",0};
    const INT8S*	UserTypes[]                 = {"Ĭ���û�","����ר���û�","��С��ר���û�","���๤��ҵ�û�","���๤��ҵ�û�","�����û�","��������",0};
    const INT8S*	nReadMeter[]			    ={"��","��",0};
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
    nIdx = 0;                                      // ת��   nBuf �� ip  ----> pData ����������ţ�
    memset(pData, 0, sizeof(nData));
    nLen = ValToAscii(&pData[nIdx], Buf2INT16U(&pBuf[1]), 4);
         nIdx += nLen;
    GuiwinSetEditCfgParam(&pParamSet, "������:   ",(INT8S*)pData, 4);

    nIdx = 0;                                      // ת��   nBuf �� ip  ----> pData  ��������ţ�
    memset(pData, 0, sizeof(nData));
    nLen = ValToAscii(&pData[nIdx], Buf2INT16U(&pBuf[3]), 4);
         nIdx += nLen;
    GuiwinSetEditCfgParam(&pParamSet, "�������:   ",(INT8S*)pData, 4);

    nProt = (pBuf[5]&0x1F);                                 // ת��   nBuf �� ip  ----> pData  (�˿ں�)
    if((nProt == 30)||(nProt == 31))
			{
        nProt -=25;
    }
    GuiwinSetSelectCfgParam(&pParamSet,"ͨѶ�˿�: ",(INT8S*)&nProt, nCommPort);

    nIdx = 0;
    // ת��   nBuf �� ip  ----> pData  (ͨѶ����)
    for(i = 0 ; i <= 7; i++)
    {
      if(Buf2Long(&pBuf[6], 4) == nBuadRate[i])
      {
        nBaudIdx = i;
        break;
      }
    }
    GuiwinSetSelectCfgParam(&pParamSet,"ͨ������: ",(INT8S*)&nBaudIdx, nBaudCfg);

    nCommProtol1 = pBuf[10];                          // ת��   nBuf �� ip  ----> pData  (ͨѶЭ��)
    if((nCommProtol1 == 30)||(nCommProtol1 == 31))
			{
        nCommProtol1 -=27;
    }
    GuiwinSetNullCfgParam(&pParamSet, "ͨѶЭ��:");
    GuiwinSetSelectCfgParam(&pParamSet, "          ", (INT8S*)&nCommProtol1, CommProtocol);

    nIdx = 0;                                   // ת��   nBuf �� ip  ----> pData  ��ͨѶ��ַ��
    memset(pData, 0, sizeof(nData));
    for (i = 0; i < 6; i++)
    {
         nLen =ValToAsciiSub((INT8U*)&pData[nIdx], pBuf[16 - i], 2, 1, 2);
         nIdx += nLen;
    }
    GuiwinSetEditCfgParam(&pParamSet,"��ַ:",(INT8S*)pData, 12);

    nIdx = 0;                                  // ת��   nBuf �� ip  ----> pData ���ɼ�����
    memset(pData, 0, sizeof(nData));
    for (i = 0; i < 6; i++)
    {
         nLen =ValToAsciiSub((INT8U*)&pData[nIdx], pBuf[31 - i], 2, 1, 2);
         nIdx += nLen;
    }
    GuiwinSetEditCfgParam(&pParamSet,"�ɼ���: ",(INT8S*)pData, 12);

    nUserType = pBuf[32];                 // ת��   nBuf �� ip  ----> pData ���û����ࣩ
        GuiwinSetNullCfgParam(&pParamSet, "�û�����:");
        GuiwinSetSelectCfgParam(&pParamSet,"",(INT8S*)&nUserType, UserTypes);

        // ת��   nBuf �� ip  ----> pData ���û�С��)
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
    GuiwinSetEditCfgParam(&pParamSet,"��ѹ���: ",(INT8S*)pData, 4);

    nIdx = 0;
    memset(pData, 0, sizeof(nData));
    nLen = ValToAscii(&pData[nIdx], Buf2INT16U(&pExtBuf[2]), 4);
    nIdx += nLen;
    GuiwinSetEditCfgParam(&pParamSet,"�������: ",(INT8S*)pData, 4);

        GuiwinSetSelectCfgParam(&pParamSet,"�Ƿ�Ͷ��:",(INT8S*)&nChoice, nReadMeter);

	pParamSet = &sParamSet[0];

        if (GuiwinSetParamMenu("����������", pParamSet) >= 0)
		{
            nIdx = 0;
            pBuf =&nBuf[0];
            memset(pBuf, 0, sizeof(nBuf));
            MemReadMeterPrm(nMearpoint,  F10, &nBuf[0], sizeof(nBuf));

            //������
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[0].nEditBuf, GET_MIN(sizeof(pParamSet[0].nEditBuf), sizeof(nData) - 1));
            //�Ƿ�Ͷ��
            pBuf[0] = nChoice;

            pBuf[1] = (INT8U)( atoi((char *)pData) & 0x00FF );
            pBuf[2] = (INT8U)(( atoi((char *)pData) & 0xFF00 ) >> 8);

            //�������
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[1].nEditBuf, GET_MIN(sizeof(pParamSet[1].nEditBuf), sizeof(nData) - 1));

            pBuf[3] = (INT8U)( atoi((char *)pData) & 0x00FF );
            pBuf[4] = (INT8U)(( atoi((char *)pData) & 0xFF00 ) >> 8);

            //������˿ں�
            pBuf[5] &= 0xE0;
            if((nProt == 5)||(nProt == 6))
            {
               nProt +=25;
            }
            pBuf[5] |= nProt;
            nComm = nProt;

            //ͨѶ����
            nIdx = 0;
               memset(pData, 0, sizeof(nData));
            nBaud = nBuadRate[nBaudIdx];
            pBuf[5] &= 0x1F;
            //pBuf[5] |= nBaudIdx << 5 ;
            Long2Buf(&pBuf[6], nBaud, 4);

            //ͨѶЭ��
            pBuf[10] = 0;
            if((nCommProtol1 == 3)||(nCommProtol1 == 4))
            {
               pBuf[10] |= (nCommProtol1 + 27);
            }
            else
            {
               pBuf[10] |= nCommProtol1;
            }

            //ͨѶ��ַ
            memset(pData, 0, sizeof(nData));
            memcpy(&pData[0], pParamSet[6].nEditBuf, GET_MIN(sizeof(pParamSet[6].nEditBuf), sizeof(nData) - 1));
            StringToBufSwap((INT8U*)&pData[0], &pBuf[11], 12);
            memcpy(&nAddr[0], &pBuf[11], 6);

            //�ɼ�����ַ
            memset(pData, 0, sizeof(nData));
            memcpy(&pData[0], pParamSet[7].nEditBuf, GET_MIN(sizeof(pParamSet[7].nEditBuf), sizeof(nData) - 1));
            StringToBufSwap((INT8U*)&pData[0], &pBuf[26], 12);

            //�û�����
            pBuf[32] = nUserType;
            //�û�С��
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
                Guiwin1MboxCreate("ȷ��","�˲������Ч��");
                continue;
            }
            if (IsRedupMtAddr(pMeterVld))
            {
                Guiwin1MboxCreate("ȷ��","��Ƶ�ַ�ظ�");
                continue;
            }

            //��ѹ���
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[20].nEditBuf, GET_MIN(sizeof(pParamSet[20].nEditBuf), sizeof(nData) - 1));
            pExtBuf[0] = (INT8U)( atoi((char *)pData) & 0x00FF );
            pExtBuf[1] = (INT8U)(( atoi((char *)pData) & 0xFF00 ) >> 8);

            //�������
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[21].nEditBuf, GET_MIN(sizeof(pParamSet[21].nEditBuf), sizeof(nData) - 1));
            pExtBuf[2] = (INT8U)( atoi((char *)pData) & 0x00FF );
            pExtBuf[3] = (INT8U)(( atoi((char *)pData) & 0xFF00 ) >> 8);

            nRet = Guiwin1MboxCreate("ȷ��","�Ƿ�ȷ���޸�ͨѶ����?");
            if (KEY_ENTER != nRet)
			{
                 continue;
            }   
            if (nComm == 0x1F)
            {
                //�ز���ʱ��Ҫ�ж�
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
            
            //���ɵ�ַ
            if (nMearpoint == 1)
            {
                //MemWriteAcPrm(F19, &pMeterVld->nAddr[0], LEN_METER_ADDR);
            }
            MemWriteMeterPrm(nMearpoint, F10, pMeterVld, sizeof(nBuf));//���ñ����Ϣ
            MemWriteMeterPrm(nMearpoint, F25, pExtBuf, sizeof(nExBuf));
            MeterMpMap(1);
            if (nComm == 0x1F)
            {
                if (g_nCarrZbSelFlg != TYPE_CARR)   //�ز��������ú�������������Ӱ���ز��ڵ����
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


//��������
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
	    if (GuiwinSetParamMenu("������������", pParamSet) >= 0)
		{
	        memcpy(&nTempMenu[0], pParamSet[2].nEditBuf, GET_MIN(sizeof(pParamSet[2].nEditBuf), sizeof(nTempMenu) - 1));

	        if((atoi((char *)nTempMenu)==0 )||(atoi((char *)nTempMenu)>MAX_MP))
			{
	            Guiwin1MboxCreate("ȷ��", "������Ч,��Χ:(1-999)");
				continue;
        }
            nMp = atoi((char *)nTempMenu);

            while(1)
            {
            //�������㰴˳�����У���Ӧ����Ӧ�ı�����
            for(nSeq = 1; nSeq < MAX_METER_NUM ; nSeq++)
            {
                nValidMp = ValidMpNum();
                memset(pMeterVld , 0 ,sizeof(MEM_METER_PRM));
                MemReadMeterPrm(nSeq , F10 , pMeterVld, sizeof(MEM_METER_PRM));
                nTmpMp = Buf2INT16U(&pMeterVld->nMp[0]);    //��ȡ������ƥ��Ĳ������

                if(nMp == nTmpMp)    //���жϳ�����Ĳ�����������������ͬ�����ó��ñ�����к�
                {
                     nSeqArry[nMp] =(nSeq = Buf2INT16U(&pMeterVld->nSeq[0]));
                     Read_Meter_Param(nSeqArry ,nMp , pMeterVld);
                     break;
                }
            }
                //������Ѳ�� ����������
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

//����������
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
    if (GuiwinSetParamMenu("������������", pParamSet) >= 0)
			{
        memcpy(&nTempMenu[0], pParamSet[2].nEditBuf, GET_MIN(sizeof(pParamSet[2].nEditBuf), sizeof(nTempMenu) - 1));

        if((atoi((char *)nTempMenu)==0 )||(atoi((char *)nTempMenu)>MAX_MP ))
		{
                Guiwin1MboxCreate("ȷ��", "������Ч,��Χ:(1-999)");
				return ;
			}
            nMp = atoi((char *)nTempMenu);
            nValidMp = MAX_MP ;       //���в������������


            //������Ѳ�� ����������
            while(1)
                    {
                //�����������
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



//�ն�ʱ������
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
        if (GuiwinSetParamMenu("�ն�ʱ��", pParamSet) >= 0)
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
            nRet = Guiwin1MboxCreate("ȷ��","���óɹ�");
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

//�ն˱������
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

    nIdx = 0;                                             // ת��   nBuf �� port  ----> pData
    memset(pData, 0, sizeof(nData));
    for (i = 0; i < 2; i++)
{
         nLen =ValToAsciiSub((INT8U*)&pData[nIdx], nBuf[1 - i], 2, 1, 2);
         nIdx += nLen;
}
    GuiwinSetNullCfgParam(&pParamSet, NULL);
    GuiwinSetEditCfgParam(&pParamSet, "��������:    ",(INT8S*)pData, 4);

    nIdx = 0;                                             // ת��   nBuf �� port  ----> pData
    memset(pData, 0, sizeof(nData));
    for (i = 0; i < 2; i++)
{
         nLen =ValToAsciiSub((INT8U*)&pData[nIdx], nBuf[3 - i], 2, 1, 2);
         nIdx += nLen;
}
    GuiwinSetNullCfgParam(&pParamSet, NULL);
    GuiwinSetNullCfgParam(&pParamSet, "�ն˵�ַ:");
    GuiwinSetEditCfgParam(&pParamSet, "    (ʮ������)", (INT8S*)pData, 4);

    nIdx = 0;                                             // ת��   nBuf �� port  ----> pData
    memset(pData, 0, sizeof(nData));
    nLen = ValToAscii(&pData[nIdx],Buf2INT16U(&nBuf[2]) , 5);
    nIdx += nLen;
    GuiwinSetEditCfgParam(&pParamSet, "      (ʮ����)", (INT8S*)pData, 5);

    pParamSet = &sParamSet[0];
    while(1)
{
        if (GuiwinSetParamMenu("�ն˱��", pParamSet) >= 0)
{
            nIdx = 0;
            memset(pBuf, 0, sizeof(nBuf));
            MemReadCommPrm(F94, &nBuf[0], sizeof(nBuf));

            //��������
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[1].nEditBuf, GET_MIN(sizeof(pParamSet[1].nEditBuf), sizeof(nData) - 1));
            StringToBufSwap((INT8U*)&pData[0], &pBuf[0], 4);

            //�ն˵�ַ(ʮ������)
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[4].nEditBuf, GET_MIN(sizeof(pParamSet[4].nEditBuf), sizeof(nData) - 1));
            StringToBufSwap((INT8U*)&pData[0], &nTmpBuf[0], 4);
            if(memcmp(nTmpBuf , &pBuf[2] ,2) != 0)
            {
            StringToBufSwap((INT8U*)&pData[0], &pBuf[2], 4);
            }
            else
            {
            //�ն˵�ַ��ʮ���ƣ�
            memset(pData, 0, sizeof(nData));
            memcpy(pData, pParamSet[5].nEditBuf, GET_MIN(sizeof(pParamSet[5].nEditBuf), sizeof(nData) - 1));
            pBuf[2] = (INT8U)( atoi((char *)pData) & 0x00FF );
            pBuf[3] = (INT8U)(( atoi((char *)pData) & 0xFF00 ) >> 8);
            }

            nRet = Guiwin1MboxCreate("ȷ��","�Ƿ�ȷ���޸�ͨѶ����?");
            if (KEY_ENTER != nRet)
{
                 continue;
}
            MemWriteCommPrm(F94, &nBuf[0], sizeof(nBuf)); //����ͨ�Ų���
}
        return ;
}
}

//��������
void  RestartRdMeter(void)
{
    SetRdMeterFlagsAllMp(0);
    RdMeterSemProc();
    OSTimeDlyHMSM(0, 0, 1, 0);
    GuiwinDynMboxCreate(0, "��������..."); 
    Guiwin1MboxCreate("ȷ��","���óɹ�");
}
//ֹͣ����
void StopRdMeter(void)
{
    ClrRdMeterFlagsAllMp(0);
    OSTimeDlyHMSM(0, 0, 1, 0);
    GuiwinDynMboxCreate(0, "ֹͣ����..."); 
    Guiwin1MboxCreate("ȷ��","���óɹ�");
}
//�ָ�����
void ReturnRdMeter(void)
{
    SetRdMeterFlagsAllMp(0);
    RdMeterSemProc();
    GuiwinDynMboxCreate(0, "�ָ�����..."); 
    OSTimeDlyHMSM(0, 0, 1, 0);
    Guiwin1MboxCreate("ȷ��","���óɹ�");
}
//��������
void ActivateConnect(void)
{
    INT8U   nLinkReq = 0;
    nLinkReq = 1;
    MemWriteRamXmpData(F9, 0, &nLinkReq, sizeof(nLinkReq));
    OSTimeDlyHMSM(0, 0, 1, 0);
    Guiwin1MboxCreate("ȷ��","���óɹ�");
}
//�Ͽ�����
void Disconnect(void)
{
    INT8U   nLinkReq = 0;
    nLinkReq = 2;
    MemWriteRamXmpData(F9, 0, &nLinkReq, sizeof(nLinkReq));
    OSTimeDlyHMSM(0, 0, 1, 0);
    Guiwin1MboxCreate("ȷ��","���óɹ�");
}
//�ڵ�ά��
void NodeMaintain(void)
{

}
//USB ����
void USBFunction(void)
{

}
//Ӳ����λ
void HardWareReset(void)
{
    GuiwinDynMboxCreate(0, "Ӳ����λ..."); 
    ResetDelayHardInit(1000);
}
//��������
void DataReset(void)
{
    GuiwinDynMboxCreate(0, "���ݳ�ʼ��..."); 
    g_nInitType = MEM_INIT_DATA;
    //MemDataInit();
    OSTimeDlyHMSM(0 ,0 ,5, 0);
    
    Guiwin1MboxCreate("ȷ��","���óɹ�");
}
//��������
void ParaReset(void)
{
    GuiwinDynMboxCreate(0, "������ʼ��..."); 
    g_nInitType = MEM_INIT_FACTORY;
    //MemFactoryInit();
    OSTimeDlyHMSM(0 ,0 ,5, 0);
    
    Guiwin1MboxCreate("ȷ��","���óɹ�");
}

#if 0
//�������������鿴������֤����
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
         if (GuiwinSetParamMenu("����������...", pParamSet) >= 0)
{

            memcpy(&nConCerPWEditBuf[0], pParamSet[0].nEditBuf, GET_MIN(sizeof(pParamSet[0].nEditBuf), sizeof(nConCerPWEditBuf) - 1));
            StringToBufSwap((INT8U*)&nConCerPWEditBuf[0], &nPassWordBuf[0], 6);
        }
        else
        {
            Guiwin1MboxCreate("����","���벻��ȷ");
            return;
        }
    }
    return;
}

//INT8U    nPassWordBuf[3] = {0};
//������������
void  InterfacePassword(void)
{
    PARAMSET		sParamSet[1]					= {0};
	pPARAMSET		pParamSet						= &sParamSet[0];
    INT8S           nConCerPWEditBuf[A_MENU_LEN]   = {"000000"};

    //ValToAsciiSub((INT8U*)&nConCerPWEditBuf[0], nPassWordBuf[2], 2, 1, 2);
    //ValToAsciiSub((INT8U*)&nConCerPWEditBuf[2], nPassWordBuf[1], 2, 1, 2);
    //ValToAsciiSub((INT8U*)&nConCerPWEditBuf[4], nPassWordBuf[0], 2, 1, 2);

    GuiwinSetEditCfgParam(&pParamSet, "����������:", &nConCerPWEditBuf[0], 6);
    pParamSet = &sParamSet[0];

	while(1)
	{
         if (GuiwinSetParamMenu("����", pParamSet) >= 0)
        {

            memcpy(&nConCerPWEditBuf[0], pParamSet[0].nEditBuf, GET_MIN(sizeof(pParamSet[0].nEditBuf), sizeof(nConCerPWEditBuf) - 1));
            //StringToBufSwap((INT8U*)&nConCerPWEditBuf[0], &nPassWordBuf[0], 6);

            ParamInterfaceSet();
        }
        else
        {
            Guiwin1MboxCreate("����","���벻��ȷ");
            return;
        }
    }
    return;
}

#endif

//����ͨѶ����
void BaseCommInfoCfg( void* pData)
{
	PARAMSET		sParamSet[10]			= {0};
	pPARAMSET		pParamSet				= &sParamSet[0];
	const INT8S*	c_pShowHexParam[]		= {"��", "��", 0};
	const INT8S*	c_pCtIrParam[]			= {"��", "��", 0};

	GuiwinSetSelectCfgParam(&pParamSet, "������ʾ:", &g_nShowHex, c_pShowHexParam);
	GuiwinSetEditCfgParam(&pParamSet, "��ʱ(ms):", &g_nTimeOut[0], sizeof(g_nTimeOut));
	GuiwinSetEditCfgParam(&pParamSet, "Զ��������ʱ(s):", &g_nUpGradeTimeOut[0], sizeof(g_nUpGradeTimeOut));
	GuiwinSetSelectCfgParam(&pParamSet, "�ն�ͨ���Ƿ����:", &g_nCtIr, c_pCtIrParam);

	//�������ò˵�
	pParamSet = &sParamSet[0];
	if (GuiwinSetParamMenu("����ͨѶ����", pParamSet) >= 0)
	{
		memset(&g_nTimeOut[0], 0, sizeof(g_nTimeOut));
		memcpy(&g_nTimeOut[0], pParamSet[1].nEditBuf, GET_MIN(sizeof(g_nTimeOut) - 1, pParamSet[1].nEditLen));
		memset(&g_nUpGradeTimeOut[0], 0, sizeof(g_nUpGradeTimeOut));
		memcpy(&g_nUpGradeTimeOut[0], pParamSet[2].nEditBuf, GET_MIN(sizeof(g_nUpGradeTimeOut) - 1, pParamSet[2].nEditLen));
	}	
	else
	{
		//Guiwin1MboxCreate("�˳�","δ����");	
	}
}

//����ͨѶ����
void SetCommInfo( void* pData)
{
	PARAMSET		sParamSet[20]			= {0};
	pPARAMSET		pParamSet				= &sParamSet[0];
	const INT8S*	c_pUartModeParam[]	= {"����", "RS485", "RS232", 0};
	const INT8S*	c_pUartBPSParam[]		= {
												"300bps",	"600bps",	"900bps",	"1200bps",
												"2400bps",	"4800bps",	"9600bps",	"14400bps",
												"19200bps", "28800bps", "38400bps", "57600bps", 
												"115200bps", 0};
	const INT8S*	c_pParityParam[]		= {"��У��", "��У��", "żУ��", 0};
	const INT8S*	c_pStopBitParam[]		= {"1λ", "2λ", 0};
	const INT8S*	c_pCharBitParam[]		= {"7λ", "8λ", 0};

	GuiwinSetSelectCfgParam(&pParamSet, "ͨѶ��:", &g_nSynBaseUartMode, c_pUartModeParam);
	GuiwinSetSelectCfgParam(&pParamSet, "������:", &g_nSynBaseUartBPS, c_pUartBPSParam);
	GuiwinSetSelectCfgParam(&pParamSet, "����λ:", &g_nSynBaseParity, c_pParityParam);
	GuiwinSetSelectCfgParam(&pParamSet, "ֹͣλ:", &g_nSynBaseStopBit, c_pStopBitParam);
	GuiwinSetSelectCfgParam(&pParamSet, "����λ:", &g_nSynBaseCharBit, c_pCharBitParam);
	
	//�������ò˵�
	pParamSet = &sParamSet[0];
	if (GuiwinSetParamMenu("ͨѶ�ڲ�������", pParamSet) >= 0)
	{

	}	
	else
	{
		//Guiwin1MboxCreate("�˳�","δ����");	
	}
}

//ϵͳ������������
void OtherParConfig(void)
{
	PARAMSET		sParamSet[10]				= {0};
	pPARAMSET		pParamSet					= &sParamSet[0];
	
	const INT8S*	c_pLedBackClose[]		= {"3����", "5����", "10����", "����", 0};
	const INT8S*	c_pTermClose[]			= {"5����","15����","30����","1Сʱ","����",0};
	const INT8S*	c_pBeepClose[]			= {"��","��",0};
	const INT8S*	c_pContinueKeyClose[]	= {"��","��",0};

	GuiwinSetSelectCfgParam(&pParamSet, "����ʱ��:", &g_LedBackCloseTime, c_pLedBackClose);
	GuiwinSetSelectCfgParam(&pParamSet, "�ػ�ʱ��:", &g_TermCloseTime, c_pTermClose);
	GuiwinSetSelectCfgParam(&pParamSet, "����������:", &g_nBeepFlg, c_pBeepClose);
	GuiwinSetSelectCfgParam(&pParamSet, "����������:", &g_nContinueKeyFlg, c_pContinueKeyClose);
	
	pParamSet = &sParamSet[0];
	if (GuiwinSetParamMenu("��������", pParamSet) >= 0)
	{
  		BackClsTimeSet(g_LedBackCloseTime);
		TermClsTimeSet(g_TermCloseTime);
		ConKeyFlgSet(g_nContinueKeyFlg);
	}
	else
	{
		//Guiwin1MboxCreate("�˳�","δ����");
	}
}

//�ʲ��������
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
	GuiwinTextOutAuto("�ʲ����:%s",&nTermSelfAddrMenu[0]);
	GuiwinTextOutAuto("");
	GuiwinTextOutAuto("");
	GuiwinTextOutAuto("");
	GuiwinTextOutAuto("");
	GuiwinTextOutAuto("");
	GuiwinTextOutAuto("  [��ȷ�ϼ��޸�]");
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
		GuiwinSetEditCfgParam(&pParamSet, "����������:", "80000000", 100 + STR_SIZE(pOPERTOR_PRM, nOperPsw) * 2);
		pParamSet = &sParamSet[0];
		if (GuiwinSetParamMenu("����", pParamSet) >= 0)
		{
			memset(&nTermSelfAddrMenu[0], 0, sizeof(nTermSelfAddrMenu));
			StringToBuf((INT8U*)(&(pParamSet[1].nEditBuf[0])), (INT8U*)&nTermSelfAddrMenu[0], GET_MIN(STR_SIZE(pOPERTOR_PRM, nOperPsw) * 2, pParamSet[1].nEditLen));
			if (PasswordChk(NULL, (INT8U*)&nTermSelfAddrMenu[0], 0) >= S_PSW_RET)
			{
				break; 
			}
			else
			{
				Guiwin1MboxCreate("����","���벻��ȷ");
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
	GuiwinSetEditCfgParam(&pParamSet, "�ʲ���:", &nTermSelfAddrMenu[0], 8);
	pParamSet = &sParamSet[0];
	if (GuiwinSetParamMenu("�ʲ����", pParamSet) >= 0)
	{
		memset(&nTermSelfAddrMenu[0], 0, sizeof(nTermSelfAddrMenu));
		memcpy(&nTermSelfAddrMenu[0], pParamSet[1].nEditBuf, GET_MIN(sizeof(pParamSet[1].nEditBuf), 8));
		StringToBuf((INT8U*)&nTermSelfAddrMenu[0], &nTermAreaAddr[0], 8);
		//SwapBufVal(&nTermAreaAddr[0], 2);
		//SwapBufVal(&nTermAreaAddr[2], 2);
		//_API_FRAM_Write(STR_OFF(pFRAM_STORE, nTermAreaAddr), &nTermAreaAddr[0], STR_SIZE(pFRAM_STORE, nTermAreaAddr));
        _API_FLASH_EraseWrite(&nTermAreaAddr[0], STR_OFF(pFRAM_STORE, nTermAreaAddr), STR_SIZE(pFRAM_STORE, nTermAreaAddr));
		memcpy(&nBuf[0], &nTermAreaAddr[0], sizeof(nTermAreaAddr));
        MemWriteCommPrm(F94, &nBuf[0], sizeof(nBuf)); //����ͨ�Ų���
        MemReadCommPrm(F94, &nBuf[0], sizeof(nBuf));
	}
	
}






// 0,����;1,���;2,����
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

//���
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

//���
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
        //GpioClearOutputEX(KEY_CTR);          // ���������Զ��ػ�
        GpioClearOutput(IO_PORTA, IO_PINx5);
    }
 
	nTmp = 3;
    //LCD_DRAW_REC(BAT_OFFSET_X + 1, cy1 + nTmp - 1, BAT_OFFSET_X + BAT_OFFSET_X_L - 1, cy1 + BAT_OFFSET_Y_H * 2 - nTmp, g_nBackColour);
	LCD_REC_CLEAR(BAT_OFFSET_X + 1, cy1 + nTmp - 1, BAT_OFFSET_X + BAT_OFFSET_X_L - 1, cy1 + BAT_OFFSET_Y_H * 2 - nTmp, g_nBackColour);
    LCD_REC_CLEAR(BAT_OFFSET_X + BAT_OFFSET_X_L - (nBlock * BAT_BLK_L), cy1 + nTmp - 1, BAT_OFFSET_X + BAT_OFFSET_X_L - 1, cy1 + BAT_OFFSET_Y_H * 2 - nTmp, nColour);
    OSSemPost(g_pSem[SEM_DISP_IDX]);
	//LCD_RANG_POST();
}


//���
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
* ������:VersionInfo
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: �汾��Ϣ
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
		LCD_CLEAR(g_nBackColour);   	//����cd
		
	 	BatteryArea();
		OSTimeDlyHMSM(0, 0, 0, 50);

		//ʱ��
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
		strcat((char*)nBuf, (char*)"�汾:V");
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
					LCD_CLEAR(g_nBackColour);   	//����cd
					//�����ø�
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

//����2�����Զ��ر�
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

//����ʱ����ʾ
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
* ������:
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: 100ms������ʾ����
*********************************************************************************/
void DispMSecProc(void)
{
	
}

/********************************************************************************
* ������:
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: s������ʾ����
*********************************************************************************/
void DispSecProc(void)
{
	BatteryProcFillpower();
}

/********************************************************************************
* ������:
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: �ּ�������ʾ����
*********************************************************************************/
void DispMinProc(void)
{
	// ����
	//LedBackClose();

	//BatteryProc();
	//BatteryArea();
    
    //ʱ��
    //TimeDisp();
}


/********************************************************************************
* ������:
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ��ʾ�����ʼ��
*********************************************************************************/
static void TaskDisplayInit(void)
{
    OSTimeDlyHMSM(0, 0, 0, 100);
  	//g_nBackColour = COLOUR_SET(0, 0, 31);;
	//g_nDrawColour = COLOUR_SET(31, 63, 31);;
	
	LCD_INIT();		   			//tft��ʼ��
	
	LCD_CLEAR(g_nBackColour);   	//����cd
    
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
	    //VersionInfo();          //�汾��Ϣ
	    *pArg = 1;
	}
	else
	{
		//g_nBackColour		= COLOUR_SET(0, 0, 31);;
		//g_nDrawColour 		= COLOUR_SET(31, 63, 31);;
		LCD_CLEAR(g_nBackColour);   	//����cd
		//�����ø�
		//BatteryArea();
		OSTimeDlyHMSM(0, 0, 0, 50);
		//TimeDisp();
	}
    if (g_nZbDispFlg == 0)
   {
     //����������
	pCurr = GuiwinRootCreat((void*)g_sMainMenu, 0); 
   }
   else if(g_nZbDispFlg == 1)
   {
     //ר�����
     pCurr = GuiwinRootCreat((void*)g_sZbMenu, 0);
   }
	while(1)
	{
		nItemSelect = GuiwinSelectMenuList(pCurr, &nKey);
		pCurr->nItemSelect = GET_MAX(nItemSelect, 1);
        switch(nItemSelect)
        {
			case -1:  //���ػ��˳�
            {
                if (pCurr->nWinLevel > 0)
                {
                    //������һ���˵�
                    pWin = pCurr;
					if (NULL == (pCurr = GuiwinGetPre(pWin)))
					{
						break;
					}
                }
                break;
            }
			default:  //��һ��˵�,ִ�л������Ĳ���
            {
                if (KEY_ENTER == nKey)
                {
                    //�����µĲ˵���Ŀ����
					pWin  = pCurr;
					if (NULL == (pCurr = GuiwinGetNext(pWin)))
					{
						break;	
					}
					
					if (GuiwinInsert(pWin, pCurr) < 0)      //�˵��͵Ŀ�
					{
					    memset(pCurr, 0, sizeof(GUI_WIN));
					    pCurr = pWin;
                        break;	
					}
					if (NULL == pCurr->pMenuItem)               //����ִ����
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
* ������:CfgSelectProcSub
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ����ѡ���Ӻ���
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
	const INT8S*	c_pWorkOrderParam[] 		= {"��", "��", 0};
	const INT8S*	c_pJcMenuParam[] 		= {"����", "�Ǽ���", 0};
	const INT8S*	c_pDefaultServParam[] 		= {"��", "��", 0};
	//const INT8S*	c_pSimpleMenu[]				= {"�Ǽ���", "����", 0};

	nAreaRun = AreaRunGet(1);
	GuiwinSetSelectCfgParam(&pParamSet, "����ѡ��:", (INT8S*)&nAreaRun, g_sAreaString);
	nCarrierCom = CarrierComGet(0);
    GuiwinSetSelectCfgParam(&pParamSet, "�ز���ͨѶ��:", (INT8S*)(&nCarrierCom), c_pUartCarrModeParam);
	nMenuMode = MenuModeGet();
	nOrder = WorkOrderGet(0);
	GuiwinSetSelectCfgParam(&pParamSet, "�Ƿ���Ҫ����:", (INT8S*)(&nOrder), c_pWorkOrderParam);
	nJcMenu = JcMenuGet(0);
	GuiwinSetSelectCfgParam(&pParamSet, "�����˵�ѡ��:", (INT8S*)(&nJcMenu), c_pJcMenuParam);
	nDefaultServ = DefaultServGet(0);
	GuiwinSetSelectCfgParam(&pParamSet, "��˾������ѡ��:", (INT8S*)(&nDefaultServ), c_pDefaultServParam);

	
	//GuiwinSetSelectCfgParam(&pParamSet, "�˵�ѡ��:", (INT8S*)&nMenuMode, c_pSimpleMenu);
	//�������ò˵�
    pParamSet = &sParamSet[0];
	if (GuiwinSetParamMenu("����ѡ��", pParamSet) >= 0)
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
* ������:CfgSelectProc
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ����ѡ��
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
		GuiwinSetEditCfgParam(&pParamSet, "����������:", "********", 100 + STR_SIZE(pOPERTOR_PRM, nOperPsw) * 2);
		pParamSet = &sParamSet[0];
		if (GuiwinSetParamMenu("����", pParamSet) >= 0)
		{
			memset(&nPwdMenu[0], 0, sizeof(nPwdMenu));
			StringToBuf((INT8U*)pParamSet[1].nEditBuf, (INT8U*)&nPwdMenu[0], GET_MIN(STR_SIZE(pOPERTOR_PRM, nOperPsw) * 2, pParamSet[1].nEditLen));
			if (PasswordChk(NULL, (INT8U*)&nPwdMenu[0], 0) >= S_PSW_RET)
			{
				break; 
			}
			Guiwin1MboxCreate("����","���벻��ȷ");
		}
		else
		{
			return -1;    
		}	 
	}
	if (CfgSelectProcSub() >= 0)
	{
		Guiwin1MboxCreate("ȷ��","ѡ��ɹ�");  
	}
    return 0;
}

/********************************************************************************
* ������:AcInfo
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ������Ϣ
*********************************************************************************/
void AcInfo(void)
{
    INT8U   nMemAddr[6] = {0};
    INT8U   nBuf[2]     = {0};
    INT8U   nBufTmp[10] = {0};
    INT8U   j = 0;
    INT8U   i   = 0;
    INT8U   nIdex = 0;
    
    MemReadAcPrm(F19, &nMemAddr[0], 6);  //��ȡ���ɵ�ַ��ת���ڴ�
    
    GuiwinTextOutAuto("��ַ:%02x%02x%02x%02x%02x%02x", nMemAddr[5], nMemAddr[4],
                                        nMemAddr[3],nMemAddr[2],nMemAddr[1],nMemAddr[0]); 
    
    GuiwinTextOutAuto("A���ѹ:%02x%02x", pDataTable->sRealInstant.sRealVolt[0].nIntg,
                                        pDataTable->sRealInstant.sRealVolt[0].nFrac); 
    GuiwinTextOutAuto("B���ѹ:%02x%02x", pDataTable->sRealInstant.sRealVolt[1].nIntg,
                                        pDataTable->sRealInstant.sRealVolt[1].nFrac); 
    GuiwinTextOutAuto("C���ѹ:%02x%02x", pDataTable->sRealInstant.sRealVolt[2].nIntg,
                                        pDataTable->sRealInstant.sRealVolt[2].nFrac); 
    
    GuiwinTextOutAuto("A�����:%02x%02x%02x", pDataTable->sRealInstant.sRealCurr[0].nD3,
                                        pDataTable->sRealInstant.sRealCurr[0].nD2,
                                        pDataTable->sRealInstant.sRealCurr[0].nD1);
    GuiwinTextOutAuto("B�����:%02x%02x%02x", pDataTable->sRealInstant.sRealCurr[1].nD3,
                                        pDataTable->sRealInstant.sRealCurr[1].nD2,
                                        pDataTable->sRealInstant.sRealCurr[1].nD1);
    GuiwinTextOutAuto("C�����:%02x%02x%02x", pDataTable->sRealInstant.sRealCurr[2].nD3,
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
                GuiwinTextOutAuto("A��:-%s", &nBufTmp[0]); 
            }
            else if (i == 1)
            {
                GuiwinTextOutAuto("B��:-%s", &nBufTmp[0]); 
            }
            else if (i == 2)
            {
                GuiwinTextOutAuto("C��:-%s", &nBufTmp[0]); 
            }
        }
        else
        {
            if (i == 0)
            {
                GuiwinTextOutAuto("A��:%s", &nBufTmp[0]); 
            }
            else if (i == 1)
            {
                GuiwinTextOutAuto("B��:%s", &nBufTmp[0]); 
            }
            else if (i == 2)
            {
                GuiwinTextOutAuto("C��:%s", &nBufTmp[0]); 
            }
        }
    }*/
    
    GuiwinAutoDispProc("������Ϣ", NULL , 0 , NULL);
}
/********************************************************************************
* ������:GprsDataWatch
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: GPRS���ݼ�⿪��
*********************************************************************************/
void GprsDataWatch(void)
{
    PARAMSET		sParamSet[10] 				= {0};
	pPARAMSET 		pParamSet					= &sParamSet[0];
	INT8U			nOrder						= 0;


	const INT8S*	c_pWorkOrderParam[] 		= {"��","��",  0};

    if (nOrder != 0)
    {
        nOrder = 1;
    }
	GuiwinSetSelectCfgParam(&pParamSet, "�Ƿ�������:", (INT8S*)(&g_nGprsWatchFlg), c_pWorkOrderParam);;

	
	//GuiwinSetSelectCfgParam(&pParamSet, "�˵�ѡ��:", (INT8S*)&nMenuMode, c_pSimpleMenu);
	//�������ò˵�
    pParamSet = &sParamSet[0];
	if (GuiwinSetParamMenu("��������", pParamSet) >= 0)
    {
		MemoryWrite(STR_OFF(pMEM_FRAM_STORE, sXFramData.nGprsFlg),&g_nGprsWatchFlg ,1, MEM_FRAM);
		return;    
    }	
    return ;
}

/********************************************************************************
* ������:StratCheck
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: �������
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
    
    //GuiwinTextOut(8, 16, 0,"���ϰ����...");
    
    for (i = 0; i < 8; i++)
    {
        SetBufBit(&nBuf, 1, i);
        GuiwinTextOut(8, 16, 0,"��%d�̵�������",i + 1);
        if (MainF5Proc(nBuf) < 0)
        {
            Guiwin1MboxCreate("ȷ��", "ͨ��ʧ��"); 
            return;
        }
        OSTimeDlyHMSM( 0, 0, 1, 500);
    }
    
    for (i = 0; i < 8; i++)
    {
        ClrBufBit(&nBuf, 1, i);
        GuiwinTextOut(8, 16, 0,"��%d�̵����Ͽ�",i + 1);
        if (MainF5Proc(nBuf) < 0)
        {
            Guiwin1MboxCreate("ȷ��", "ͨ��ʧ��"); 
            return;
        }
        OSTimeDlyHMSM( 0, 0, 1, 500);
    }
    beep();
    Guiwin1MboxCreate("ȷ��", "���Գɹ�");
    
    
    
    /*if (MainF5Proc(0x0F) < 0)
    {
        Guiwin1MboxCreate("ȷ��", "ͨ��ʧ��");   
    }
    OSTimeDlyHMSM( 0, 0, 3, 0);
    
    if (MainF5Proc(0xFF) < 0)
    {
        Guiwin1MboxCreate("ȷ��", "ͨ��ʧ��");   
    }
    OSTimeDlyHMSM( 0, 0, 3, 0);
    
    
    if (MainF5Proc(0xF0) < 0)
    {
        Guiwin1MboxCreate("ȷ��", "ͨ��ʧ��");   
    }
    OSTimeDlyHMSM( 0, 0, 3, 0);
    
    if (MainF5Proc(0x00) < 0)
    {
        Guiwin1MboxCreate("ȷ��", "ͨ��ʧ��");   
    }
    beep();
    Guiwin1MboxCreate("ȷ��", "ͨ�ųɹ�"); 
    //WriteComm(COMM_ERR, l_buff, 21);
    //l_ret = ReadCommMs(COMM_ERR, l_Tmpbuff, 100, 500, 500);
    /*if (l_ret > 0)
    {
        Guiwin1MboxCreate("ȷ��", "ͨ�ųɹ�"); 
    }
    else
    {
        Guiwin1MboxCreate("ȷ��", "ͨ��ʧ��");   
    }*/
    
}

/********************************************************************************
* ������:GprsTest
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: 
*********************************************************************************/
void GprsTest(void)
{
    INT8U	    l_buff[128];
    INT16U		nX		= 8;
	INT16U		nY		= 8;
    INT8U       nFlg    = 0;
	INT8U		j		= 0;
    
    LcdClear(g_nBackColour);
    
    GuiwinTextOut(nX, nY, 0,"GPRS����...");
	nY += 16;
    
    GuiwinTextOut(nX, nY, 0,"���ڽ��л�������...");
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
	/*if (!SendAtCmdAndWaitATResp((char*)"AT+GMR\x0d",(char*)g_AT_OK,3000,3,0,NULL, 0))//���汾
	{
		//return;	//�汾����ͨ��Ӱ�칦�ܣ��ʲ��˳�
	}*/
    
    if (!SendAtCmdAndWaitATResp((char*)"AT#CCID\x0d\x0a",(char*)g_Modl_OK,3000,3,0,NULL, sizeof(l_buff)))
	{
		//return;	
	}
    
    if (nFlg)
    {
        GuiwinTextOut(nX, nY, 0,"AT ͨ��ʧ��"); 
        GuiwinTextOutAuto("AT ͨ��ʧ��");
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
        GuiwinTextOut(nX, nY, 0,"ģ��汾�ŵ�");
        GuiwinTextOutAuto("ģ��汾�ŵ�");
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
    
    //SIM��ע��
	for(j=0; j<80; j++)
    {
        if (KEY_CANCEL == GetKeyVal(1))
        {
            j = 80;
            break;
        }
        GuiwinDynMboxCreate(j, "SIM������ע��");
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
		GuiwinTextOut(nX, nY, 0,"SIMע��ʧ��");	
        GuiwinTextOutAuto("SIMע��ʧ��");
		//g_nErrFlg = 1; 
        //V5_GPRS_OFF();
        return;
	}
    OSTimeDlyHMSM(0, 0, 1, 500);
    
    beep();    
    Guiwin1MboxCreate("ȷ��", "GPRS���Գɹ�");

}

/********************************************************************************
* ������:CtrlProc
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ���ư����
*********************************************************************************/
void CtrlProcTest(void)
{
    INT8U	l_buff[30]   = {0x68, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x68, 0x15, 0x02, 0x32, 0x32, 0x47, 0x16};
    INT8U	l_buff2[30]   = {0x68, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x68, 0x15, 0x02, 0x33, 0x33, 0x49, 0x16};
	INT8U   l_Tmpbuff[30] = {0};
    INT8U	l_ret = 0;
    

	V1_IRTXM_ON();
    
    LcdClear(g_nBackColour);
    
    GuiwinTextOut(8, 16, 0,"���ư����...");
    
    WriteComm(COMM_ZB, l_buff, 14);
    l_ret = ReadCommMs(COMM_ZB, l_Tmpbuff, 100, 500, 500);
    
    if (l_ret == 0)
    {
        Guiwin1MboxCreate("ȷ��", "ͨ��ʧ��");
    }
    OSTimeDlyHMSM(0, 0, 3, 0);
    WriteComm(COMM_ZB, l_buff2, 14);
    l_ret = ReadCommMs(COMM_ZB, l_Tmpbuff, 100, 500, 500);
    
    if (l_ret > 0)
    {
        beep();    
        Guiwin1MboxCreate("ȷ��", "ͨ�ųɹ�");
    }
    else
    {
        Guiwin1MboxCreate("ȷ��", "ͨ��ʧ��");
    }
}


/*****************************************************************************
* �� �� ��:BcdToAcii
* �䡡  ��:pData     ��ת����Դ����
*          nDataLen  ��ת����Դ���ݳ���
*          bDescFlg  ����ת����־ TRUE  20 83 -> 83 20->38 33 32 30
*                                 FALSE 20 83 -> 20 83->32 30 38 33
* �䡡  ��:pDest     ת���������
* ��    ��:TRUE      ת���ɹ�
*          FALSE     ���������Ч
* ��������:��BCD�ĸ�ʽת��ΪACAII��ʽ TRUE    20 83 -> 83 20->38 33 32 30
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
* ������: TltSimRegEx
* �䡡��: 
* ��  ��: 
* ��  ��: 
*       
*       
* ��  ��: sim��ע��
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
  
    //���ⲻ��
    GpioSetOutput(IO_PORT7, IO_PINx5); 
    //����ʾ��
    memset(nTmpBuf, 0, 35);
    BULCDWrite1(0, nTmpBuf, 35);
    //����Ʋ���
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
    //��ȡϵͳʱ��
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
    
    //GetNextMon(&sLastTime, &sTime); //�洢Ϊ����1��ʱ�䣬Ӧ��ȥ�ϼ����ڼ���һ��
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
          GpioClearOutput(IO_PORT7, IO_PINx5);  //������� 
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
         GpioClearOutput(IO_PORT7, IO_PINx5);  //������� 
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
//�Զ���ʾ
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
            //���ϵ�ʱ��ֹ��������δ��ȫ����ʱ�ͽ��й��ϴ�������жϴ���100,��ʾ���������ҳ
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

//��������
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
                if(((g_MeterHardwareWrong[0]&0x04)==0)&&((g_MeterHardwareWrong[0]&0x01)==0))  //δ�յ�����������������
                {
                    GpioClearOutput(IO_PORT7, IO_PINx5);  //������� 
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
                if(((g_MeterHardwareWrong[0]&0x04)==0)&&((g_MeterHardwareWrong[0]&0x01)==0))  //δ�յ�����������������
                {
                    GpioClearOutput(IO_PORT7, IO_PINx5);  //������� 
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
                    };   // ��ʼ������

INT8U ERR01=0,ERR02=0,ERR03=0,ERR04=0,ERR05=0,ERR06=0,ERR08=0;
INT8U ERR51=0,ERR52=0,ERR53=0,ERR54=0,ERR55=0,ERR56=0;
INT8U       nTempBattery                    = 0;
INT8U       nTempBattery2                    = 0;

Page_Nsp currentPage = PAGE_HOME;
/********************************************************************************
* ������:
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ��ʾ����
*********************************************************************************/

void DispTask(void *pData)
{
    INT8U   nStepCnt    = 0;
    INT8U   M=0,nTmp    = 0;     
    INT8U   data0[2]    = 0;
    INT8U   nCtrlFlg    = 0;
    
	
	// ����һ���������ڴ洢��һ��Ҫ�л�����ҳ�棬��ʼ��Ϊ��ҳ
    //Page_Nsp nextPage = PAGE_HOME;
	
    while(1)
	{
		updateLCDDisplay();
	}
}

// ���ݵ�ǰҳ�����LCD��ʾ
void updateLCDDisplay() 
{
	currentPage = (Page_Nsp)g_DwinPageID;
    switch (currentPage) {
        case PAGE_HOME:
            updateHomePage();      //��ҳ��
            break;

        case PAGE_SETTINGS:
            updateSettingsPage();  //����ҳ��
            break;

        case PAGE_STORAGE:
            //updateStoragePage();   //�洢ҳ��
            break;

        case PAGE_DATETIME:
            //updateDatetimePage();  //����ʱ��ҳ��
            break;
        case PAGE_MONITOR:
            updateMonitorPage();  //���������ҳ��
            break;    
        case PAGE_SCANNER:
            //updateScannerPage();  //ɨ��ҳ��
            break;

        

        default:
            break;
    }
}



 // ��Һ������������
 INT8U sendDataToLCD(INT8U* pData, INT32S nDatalen) {
    INT8U	l_ret = 0;
    INT8U   nCom = COMM_232;  // ��Ӧ���� COMM7:  P15--UART0_TX, P14--UART0_RX  

    if ((NULL == pData) || (nDatalen < 0) ) {
        return -1;
    }
    //V7_TX_ON();

    l_ret = WriteComm((INT8U)nCom, pData, nDatalen);

    return l_ret;
}


// ʮ��������תIEEE 754������float�ı��루��uint32_t���أ�
INT32U int_to_ieee754(int n) {
    if(n == 0) return 0; // ������0�ı���

    // Step 1: ����λ
    INT32U sign = 0;
    if (n < 0) 
    {
        sign = 1;
        n = -n;
    }

    // Step 2: �ҵ����λ1���ڵ�bit����ָ����
    int exponent = 0;
    int temp = n;
    while (temp >>= 1) exponent++;

    // Step 3: ����β��
    // ȥ�����λ���Ǹ�1������λ���Ƶ�23λ
    INT32U mantissa = ((INT32U)n & ((1U << exponent) - 1)) << (23 - exponent);

    // Step 4: ָ�����루��ƫ����127��
    INT32U exp_encoded = exponent + 127;

    // Step 5: ���
    INT32U ieee = (sign << 31) | (exp_encoded << 23) | mantissa;
    return ieee;
}

INT64U int_to_ieee754_double(int n) {
    if (n == 0) return 0; // 0�ı���

    // Step 1: ����λ
    INT64U sign = 0;
    if (n < 0) {
        sign = 1;
        n = -n;
    }

    // Step 2: �ҵ����λ1���ڵ�bit����ָ����
    int exponent = 0;
    int temp = n;
    while (temp >>= 1) exponent++;

    // Step 3: ����β��
    // ȥ�����λ���Ǹ�1������λ���Ƶ�52λ
    INT64U mantissa = ((INT64U)n & ((1ULL << exponent) - 1)) << (52 - exponent);

    // Step 4: ָ�����루��ƫ����1023��
    INT64U exp_encoded = exponent + 1023;

    // Step 5: ���
    INT64U ieee = (sign << 63) | (exp_encoded << 52) | mantissa;
    return ieee;
}




/******************************************************************************
*������ҳ��ʾ���� 
*������ram������ַ�洢�ռ�ʹ�÷�Χ��0x5000~0xffff��ÿ��������ַ��Ӧ��Ӧ�ռ��2�ֽڣ��������ͣ�2�ֽڣ���ָ������ݣ����ǲ��ø��ֽ��ȴ��͵ķ�ʽ��MSB����
*����ʱ�������ַ0x0010���õ�ַ�̶��������Զ���
*����A/B/C״̬�����ߣ����ı�����ַΪ��0x1000��0x1030, 0x1060, 0x1090, 0x10C0�� ���ݣ�0x0001:����,0X0000:δ����
*��ѹV������A������kW������kWh��CPU�¶ȵı�����ַΪ��0x1120, 0x1150, 0x1180, 0x11B0, 0x1210�� ����: ASCII����峤�ȸ�������λ����
*�û����룬��ʾ��Ϣ��������/�Ͽ���ָʾ�Ƶı�����ַΪ��0x10F0, 0x11E0, 0x1240��
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

	//����A��״̬
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
	
    sendDataToLCD(Data, offset); // ���ͽ���A״̬����

	OSTimeDlyHMSM(0,0,0,5);
	
	//��ʾ������״̬�����_B  
	memset(Data,0,sizeof(Data));
    offset = 0; // ����ƫ����
	Data[offset++]=0x00; 
    if (wiringStatusBitmap & 0x02) 
	{
        Data[offset++] = 0x01;    //����B��״̬    
    }    
    else  
	{
		Data[offset++] = 0x00; 
	}

    offset = DwinMakeFrm(&Data[0],offset,nAddr[1],0x82);
	
    sendDataToLCD(Data, offset); // ���ͽ���B״̬����
	
	OSTimeDlyHMSM(0,0,0,5);
	
	//��ʾ������״̬�����_C  
	memset(Data,0,sizeof(Data));
    offset = 0; // ����ƫ����
    Data[offset++]=0x00; 
    if (wiringStatusBitmap & 0x04) 
	{
        Data[offset++] = 0x01;    //����C��״̬    
    }    
    else    
	{
		Data[offset++] = 0x00;  
	}

	offset = DwinMakeFrm(&Data[0],offset,nAddr[2],0x82);
	
    sendDataToLCD(Data, offset); // ���ͽ���C״̬����
	
	OSTimeDlyHMSM(0,0,0,5);
	
	//��ʾ����״̬_���� 
	memset(Data,0,sizeof(Data));
    offset = 0; // ����ƫ����
    Data[offset++]=0x00; 
    if (wiringStatusBitmap & 0x10) 
	{
        Data[offset++] = 0x01;    //���ߵ�״̬    
    }    
    else
	{
		Data[offset++] = 0x00; 
	}
	
    offset = DwinMakeFrm(&Data[0],offset,nAddr[3],0x82);
    sendDataToLCD(Data, offset);
	OSTimeDlyHMSM(0,0,0,5);
	
	 //��ʾ���_����  
	memset(Data,0,sizeof(Data));
    offset = 0; // ����ƫ����
    Data[offset++]=0x00; 
    nStatus = wiringStatusBitmap & 0x0f;
    switch (nStatus)
    {
		case 1:
			Data[offset++]=0x01;      //A��
			break;
		case 2:
			Data[offset++]=0x02;      //B�� 
			break;    
		case 4:
			Data[offset++]=0x03;      //C�� 
			break;            
		default:
            Data[offset++]=0x00;      //��
			break;
    }

    offset = DwinMakeFrm(&Data[0],offset,nAddr[4],0x82);
    sendDataToLCD(Data, offset);
	
	// ��ʾ�û�����
	OSTimeDlyHMSM(0,0,0,5);
	memset(Data,0,sizeof(Data));
    offset = 0; // ����ƫ����

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

	// ��ʾ��ѹ
	OSTimeDlyHMSM(0,0,0,5);
	memset(Data,0,sizeof(Data));
	memset(nBuf,0,sizeof(nBuf));
    offset = 0; // ����ƫ����
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
    offset = 0; // ����ƫ����
    // ��ʾ���� 
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
    offset = 0; // ����ƫ����
    //��ʾ���� 
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

    offset = 0; // ����ƫ����
    //��ʾ����  
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
	
    offset = 0; // ����ƫ����
    //��ʾ��ʾ��Ϣ
#if 1
    //test ����
    Data[offset++] = 0xB9;    
    Data[offset++] = 0xA4; 
    Data[offset++] = 0xD7;    
    Data[offset++] = 0xF7;      
#else
    //test ����
    Data[offset++] = 0xbf;    
    Data[offset++] = 0xd5; 
    Data[offset++] = 0xcf;    
    Data[offset++] = 0xd0;    
#endif
    // ����CRC������
    offset = DwinMakeFrm(&Data[0],offset,nAddr[10],0x82);
    sendDataToLCD(Data, offset);  
	
	OSTimeDlyHMSM(0,0,0,5);
	memset(Data,0,sizeof(Data));   

    offset = 0; // ����ƫ����
    //��ʾCPU�¶�
    Data[offset++] = 0x44;
    Data[offset++] = 0x79;    
    Data[offset++] = 0xF9; 
    Data[offset++] = 0x9A;        
    
    offset = DwinMakeFrm(&Data[0],offset,nAddr[11],0x82);
    sendDataToLCD(Data, offset);  
	
	OSTimeDlyHMSM(0,0,0,5);
	memset(Data,0,sizeof(Data));   

    offset = 0; // ����ƫ����
    //��ʾ����״̬
/*    
    displayLinkstatCmd[offset++] = GlbHmPgInf_Nsp.ConStaus;    //������/�Ͽ���ָʾ��   
*/
	Data[offset++] = 0x00; 
    
    if(g_pKeySamp->nKeyValBak == 0x01)
    {
       Data[offset++] = 0x01;    //�����ӡ�ָʾ�� 
    }
    else
    {
        Data[offset++] = 0x00;    //���Ͽ���ָʾ��
    }

    offset = DwinMakeFrm(&Data[0],offset,nAddr[12],0x82);
    sendDataToLCD(Data, offset);  
	
	//OSTimeDlyHMSM(0,0,0,5);
	//memset(Data,0,sizeof(Data));  



}

void updateSettingsPage()
{
    
}

//���������
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
    
    // ��ѹ
	memset(Data,0,sizeof(Data));
	memset(nBuf,0,sizeof(nBuf));
    
    offset = 0; // ����ƫ����
    nConvert = Bcdbuf2Long(&pDataTable->sRealInstant.sRealVolt[i].nFrac, 2);
    nConvert = 220;
    nNum = int_to_ieee754(nConvert);  
	offset = DwinMakeFrm(&Data[0],offset,nAddr[6],0x82);
    sendDataToLCD(Data, offset);  
 
	OSTimeDlyHMSM(0,0,0,5);
	memset(Data,0,sizeof(Data));
    offset = 0; // ����ƫ����
    // ���� 
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
    offset = 0; // ����ƫ����
    //���� 
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

    offset = 0; // ����ƫ����
    //��ʾ����  
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
    //��������
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
    
    //CO2����
    
    //�¶�
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
    //Ƶ��
    
}

//ʱ�����ý���
void updateDatetimePage()
{
    
}



























  
//�������ʾ  ����Data[16]λ��ֵ��0x04�ĳ�0x00,Ŀ����ȥ����ʾ�ϵ�С���ӱ�ʶ

//INT8U  Data2[23] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x04, 0, 0x04, 0, 0, 0, 0,};   // ��ʼ������
//INT8U  Data2Copy[23] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x04, 0, 0x04, 0, 0, 0, 0,}; 


INT8U  Data2[23] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0x04, 0, 0, 0, 0,};   // ��ʼ������

INT8U  Data2Copy[23] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0x04, 0, 0, 0, 0,}; 


INT8U  DataAll_2[20] = {0xe0,0xb0,  0x60,0x00,  0xc0,0x70,  0xe0,0x50,   0x60,0xc0,  
                            0xa0,0xd0, 0xa0,0xf0,  0xe0,0x00,   0xe0,0xf0,  0xe0,0xd0
                          };   // ��ʼ������
INT8U  Data_P_2[20] = {0xf0,0xb0,  0x70,0x00,  0xd0,0x70,  0xf0,0x50,   0x70,0xc0,  
                            0xb0,0xd0, 0xb0,0xf0,  0xf0,0x00,   0xf0,0xf0,  0xf0,0xd0
                          };   // ��ʼ������
//�� 1 ��  ��ǰ����й��ܵ��� 
void  LcdDisplayPage_1(void)                  //ÿ��ˢ��һ��
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
    Data[18] += 0x08;                  // ��ǰ

    BULCDWrite1(0, Data, 23);
}
//�� 2 ��  ��ǰ����й������ 
void  LcdDisplayPage_2(void)                  //ÿ��ˢ��һ��
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
    Data[18] += 0x08;                  // ��ǰ

    BULCDWrite1(0, Data, 23);
}
//�� 3 ��  ��ǰ����й������ 
void  LcdDisplayPage_3(void)                  //ÿ��ˢ��һ��
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
    Data[18] += 0x08;                  // ��ǰ

    BULCDWrite1(0, Data, 23);
}

//�� 4 ��  ��ǰ����й�ƽ���� 
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
    Data[18] += 0x08;                  // ��ǰ

    BULCDWrite1(0, Data, 23);
}
//�� 5��  ��ǰ����й��ȵ��� 
void  LcdDisplayPage_5(void)                  //ÿ��ˢ��һ��
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
    Data[18] += 0x08;                  // ��ǰ

    BULCDWrite1(0, Data, 23);
}

//�� 6 ��  �� 1 ������й��ܵ��� 
void  LcdDisplayPage_6(void)                  //ÿ��ˢ��һ��
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
    Data[20] += 0x04;  // ��1��
    Data[17] += 0x80;

    BULCDWrite1(0, Data, 23);
}
//�� 7 ��  �� 1 ������й������ 
void  LcdDisplayPage_7(void)                  //ÿ��ˢ��һ��
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
    Data[20] += 0x04;  // ��1��                  
    Data[17] += 0x80;

    BULCDWrite1(0, Data, 23);
}
//�� 8 ��  �� 1 ������й������ 
void  LcdDisplayPage_8(void)                  //ÿ��ˢ��һ��
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
    Data[20] += 0x04;                  // ��1��
    Data[17] += 0x80;
     
      BULCDWrite1(0, Data, 23);
}
//�� 9 ��  �� 1 ������й�ƽ���� 
void  LcdDisplayPage_9(void)                  //ÿ��ˢ��һ��
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
          Data[20] += 0x04;                  // ��1��
          Data[17] += 0x80;
     
      BULCDWrite1(0, Data, 23);
}
//�� 10 ��  �� 1 ������й��ȵ��� 
void  LcdDisplayPage_10(void)                  //ÿ��ˢ��һ��
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
          Data[20] += 0x04;                   // ��1��
          Data[17] += 0x80;
     
      BULCDWrite1(0, Data, 23);
}
//�� 11 ��  �� 2 ������й��ܵ��� 
void  LcdDisplayPage_11(void)                  //ÿ��ˢ��һ��
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
    Data[20] += 0x78;                   // ��2��
    Data[17] += 0x80;
     
    BULCDWrite1(0, Data, 23);
}
//�� 12��  �� 2 ������й������ 
void  LcdDisplayPage_12(void)                  //ÿ��ˢ��һ��
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
    Data[20] += 0x78;                 // ��2��
    Data[17] += 0x80;
     
      BULCDWrite1(0, Data, 23);
}
//�� 13 ��  �� 2 ������й������ 
void  LcdDisplayPage_13(void)                  //ÿ��ˢ��һ��
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
          Data[20] += 0x78;                  // ��2��
          Data[17] += 0x80;
     
      BULCDWrite1(0, Data, 23);
}
//�� 14 ��  �� 2 ������й�ƽ���� 
void  LcdDisplayPage_14(void)                  //ÿ��ˢ��һ��
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
          Data[20] += 0x78;                  // ��2��
          Data[17] += 0x80;
     
      BULCDWrite1(0, Data, 23);
}
//�� 15 ��  �� 2 ������й��ȵ��� 
void  LcdDisplayPage_15(void)                  //ÿ��ˢ��һ��
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
          Data[20] += 0x78;                  // ��2��
          Data[17] += 0x80;
      BULCDWrite1(0, Data, 23);
}
//�� 16��  ͨ�ŵ�ַ�� 8λ
void  LcdDisplayPage_16(void)                  //ÿ��ˢ��һ��
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
     if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
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
//�� 17 ��  ͨ�ŵ�ַ�� 4 λ
void  LcdDisplayPage_17(void)                  //ÿ��ˢ��һ��
{   INT8U       i ;
    INT8U       Data[23];
    INT8U       nBuf1[4];
    INT8U       nBuf[6];
    INT8U        nMemAddr[6];

    memcpy(Data,Data2,23); 
    memset(&nBuf, 0, sizeof(nBuf));
    MemReadAcPrm(F19, &nMemAddr[0], 6);
    memcpy(nBuf, &nMemAddr[0], 6);
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
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


//�� 18 �� ����
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
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }
     if(0x02==(g_MeterInsideWrong[0]&0x02)) //������ʾ����
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
     if(0x20==(g_MeterInsideWrong[0]&0x20)) //ʱ�ӹ���
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
      Data[18] += 0x08;// ��ǰ
    BULCDWrite1(0, Data, 23); 
   }
//�� 19 ��  ʱ��
void  LcdDisplayPage_19(void)                  //ÿ��ˢ��һ��
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
     if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }
     if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }
     if(0x02==(g_MeterInsideWrong[0]&0x02)) //ʱ����ʾ����ģ��
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
      if(0x20==(g_MeterInsideWrong[0]&0x20)) //ʱ�ӹ���
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
      Data[18] += 0x08;// ��ǰ
    BULCDWrite1(0, Data, 23); 
}
//�� 20 ��   ��ѹ
void  LcdDisplayPage_20(void)                  //ÿ��ˢ��һ��
{    
    INT8U       i  = 0;   
    INT8U       Data[23];
    INT8U       nBuf[2];
    INT8U       nBuf1[4]; 
    MEM_AC_REAL_DATA            sRealData;             //ʹ��ȫ�ֱ���
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    memcpy(Data,Data2,23); 
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    memcpy(nBuf, &pDataTable->sRealInstant.sRealVolt[0].nFrac, 2);
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }
    
    //A���ѹΪ50%
    if (((g_MeterHardwareWrong[1]&0x04)==0x04) || (1 == g_MeterEventWrong[0]&0x01))
    { 
        DivideTwo(nBuf);
    }
    
    if (0x11 == (g_MeterVoltPlusFlg&0x11))//��ѹԽ����
    {
        nBuf[1] += 0x05;
    }
    else if((0x21 == (g_MeterVoltPlusFlg&0x21))||(g_MeterVoltPlusFlg == 0x01))//��ѹԽ����
    {
        DivideTwo2(nBuf); //DivideTwo(nBuf);
    }

    //A���޵�ѹ
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
//�� 21��   ����
void  LcdDisplayPage_21(void)                  //ÿ��ˢ��һ��
{    
    INT8U       i  = 0;   
    INT8U       Data[23];
    INT8U       nBuf[3];
    INT8U       nBuf1[6]; 
    MEM_AC_REAL_DATA            sRealData;             //ʹ��ȫ�ֱ���
    pMEM_AC_REAL_DATA           pRealData = &sRealData;        
    
    memcpy(Data,Data2,23);  
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    memcpy(nBuf,  &pDataTable->sRealInstant.sRealCurr[0].nD1, 3);
     nBuf[2] &= 0x7f;   //���Է���λ
     
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
    {
        memset(nBuf, 0, sizeof(nBuf));
    }
    
    //A�����Ϊ50%
    if((g_MeterHardwareWrong[1]&0x20)==0x20)
    { 
       DivideTwo(nBuf);
    }
    
    //A���޵���
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
//�� 22 ��  �й�����
void  LcdDisplayPage_22(void)                  //ÿ��ˢ��һ��
{    
    INT8U       i  = 0;   
    INT8U       j  = 0;  
    INT8U       Data[23];
    INT8U       nBuf[3];
    INT8U       nBuf1[8];    
    MEM_AC_REAL_DATA            sRealData;             //ʹ��ȫ�ֱ���
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    memcpy(Data,Data2,23);  
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    memcpy(nBuf, &pDataTable->sRealInstant.sRealP[0].nFrac0, 3 );
    
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
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

//�� 23 ��  ��������
void  LcdDisplayPage_23(void)                  //ÿ��ˢ��һ��
{    
    INT8U       i  = 0;   
    INT8U       Data[23];
    INT8U       nBuf[2];
    INT8U       nBuf1[4];    
    MEM_AC_REAL_DATA            sRealData;             //ʹ��ȫ�ֱ���
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
      memcpy(Data,Data2,23);    
   MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    memcpy(nBuf, &pDataTable->sRealInstant.sRealPf[1].nFrac, 2 );
    
    nBuf[1] = nBuf[1] & 0x7f;
    
     if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
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

//�� 24��   ����
void  LcdDisplayPage_24(void)                  //ÿ��ˢ��һ��
{    
    INT8U       i  = 0;   
    INT8U       Data[23];
    INT8U       nBuf[3];
    INT8U       nBuf1[6]; 
    INT8U       nTmp[2];
    INT32U      nNum;
    MEM_AC_REAL_DATA            sRealData;             //ʹ��ȫ�ֱ���
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
     nBuf[2] &= 0x7f;   //���Է���λ
     
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
    {
        memset(nBuf, 0, sizeof(nBuf));
    }
    
    //A�����Ϊ50%
    if((g_MeterHardwareWrong[1]&0x20)==0x20)
    { 
       DivideTwo(nBuf);
    }
    
    //A���޵���
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
* ������: AcCalcEventState2
* �䡡��:
* ��  ��:
* ��  ��:
* ��������:�����¼��жϴ���
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
    MEM_AC_REAL_DATA            sRealData;             //ʹ��ȫ�ֱ���
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    AC_EVENT_STAT               sEventStat;
    pAC_EVENT_STAT              pEventStat;
    EVT_TRIP_DATA		        tEvtTrip;		//��բ״̬
    EVT_ALARM_DATA              tAlarm; 
    EVT_GUARPOWER_DATA	        tGuarantPower;		//����״̬
    
    //INT8U       nAcEventTrip = 0;
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    
    //MemReadAcXFlhBackData(F85, &nAcEventTrip, sizeof(nAcEventTrip));
    MemReadAcXFlhBackData(F85, &tEvtTrip, sizeof(tEvtTrip));
     GetSysClk(&sTime);
   //  if(( sTime.nHour >= 0x18)&&(sTime.nHour < 0x21 ))        //��ǰʱ��⡢�塢ƽ����  
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
      
    //if((pDataTable->sRealInstant.sRealP[1].nSint)&0x80)                                         //�жϹ��ʷ���
 
    //if((pDataTable->nPFlag & (0x01 << 3))&&(!(pDataTable->sRealInstant.bsRunStat[3] & 0x01)))
     if ((pDataTable->nPFlag&0x01)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[1].nFrac0, 3, 0)))&&(pDataTable->fCurr[0]>nTmp1))   //�жϷ���
    {
       Data2[19]  |= 0x04;
    }
    else
    {
      Data2[19]  &= 0xfb;
    }

    if ((0x40 == (g_MeterInsideWrong[0] & 0x40)) || (1 == tEvtTrip.nEvtTrip)) //��բ
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
extern  INT8U  DataAll[32] = { 0xf0,0xa0,  0x60,0x00,   0xd0,0x60,  0xf0,0x40,   0x60,0xc0,  0xb0,0xc0,  0xb0,0xe0,  0xe0,0x00,  0xf0,0xe0,  0xf0,0xc0  ,0xe0,0xe0  ,0x30,0xe0,  0x90,0xa0, 0x70,0x60, 0x90,0xe0,  0x80,0xe0};// ��������
extern  INT8U  Data_P[20] = {  0xf0, 0xb0,  0x60,0x10,   0xd0,0x70,  0xf0,0x50,   0x60,0xd0,  0xb0,0xd0,  0xb0,0xf0,  0xe0,0x10,  0xf0,0xf0,  0xf0,0xd0};// ������


/*
/******************************************************************************************************
* ������: AcCalcEventState
* �䡡��:
* ��  ��:
* ��  ��:
* ��������:�����¼��жϴ���
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
    MEM_AC_REAL_DATA            sRealData;             //ʹ��ȫ�ֱ���
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    AC_EVENT_STAT               sEventStat;
    pAC_EVENT_STAT              pEventStat;
    EVT_TRIP_DATA		        tEvtTrip;		//��բ״̬
    EVT_ALARM_DATA              tAlarm; 
    EVT_GUARPOWER_DATA	        tGuarantPower;		//����״̬
    //INT8U       nAcEventTrip = 0;
    
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    MemReadAcXFlhBackData(F85, &tEvtTrip, sizeof(tEvtTrip));
    MemReadAcXFlhBackData(F86, &tAlarm, sizeof(tAlarm));
    MemReadAcXFlhBackData(F87, &tGuarantPower, sizeof(tGuarantPower));
    
     GetSysClk(&sTime);
     Data1[7] |= 0x02;
    //if(( pTime->nHour == 0x19) || (pTime->nHour == 0x20 ) || (pTime->nHour == 0x11 ))       //��ǰʱ��⡢�塢ƽ����     
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
   // if (pDataTable->sRealInstant.bsRunStat[3] & 0x80)         //��ѹ����
  if ((pDataTable->fVolt[0] < (g_nVolt * LOSS_PH_OC_RATIO)) && (pDataTable->fCurr[0] < (RATED_CURR * LOSS_PH_CURR)))
   { 
     if(1!=(g_MeterEventWrong[0]&0x01)) //Aʧѹģ��
     {
       Data1[15] &= 0xf7; 
     }
      g_nFlg_Alarm = 1;
   }
   else 
   {  
     //if(pDataTable->sRealInstant.bsRunStat[3] & 0x01)            //ʧѹ
      if ((pDataTable->fVolt[0] < (g_nVolt * LOSS_VOLT_OC_RATIO)) && (pDataTable->fCurr[0] > (RATED_CURR * OC_CURR)))
       {
          g_nFlgUa = 1;
          g_nFlg_Alarm = 1;
       }
       else
       {  
         if(1!=(g_MeterEventWrong[0]&0x01)) //Aʧѹģ��
         {
            Data1[15] |= 0x08;
         }
       }    
   }
if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType))//��������������������
{
   //if(pDataTable->sRealInstant.bsRunStat[4] & 0x80)                      //��ѹ����    
   if ((pDataTable->fVolt[1] < (g_nVolt * LOSS_PH_OC_RATIO)) && (pDataTable->fCurr[1] < (RATED_CURR * LOSS_PH_CURR)))
   {
     if(1!=(g_MeterEventWrong[1]&0x01)) //Bʧѹģ��
     {
       Data1[15] &= 0xfb;
     }
    g_nFlg_Alarm = 1;
   }
   else 
   {    
      //if(pDataTable->sRealInstant.bsRunStat[4] & 0x01)                    //ʧѹ
      if ((pDataTable->fVolt[1] < (g_nVolt * LOSS_VOLT_OC_RATIO)) && (pDataTable->fCurr[1] > (RATED_CURR * OC_CURR)))
       {
          g_nFlgUb = 1;
          g_nFlg_Alarm = 1;
       }
       else
       {  
          if(1!=(g_MeterEventWrong[1]&0x01)) //Bʧѹģ��
          {
            Data1[15] |= 0x04;
          }
       }    
   }
}
    //if(pDataTable->sRealInstant.bsRunStat[5] & 0x80)                       //��ѹ����    
  if ((pDataTable->fVolt[2] < (g_nVolt * LOSS_PH_OC_RATIO)) && (pDataTable->fCurr[2] < (RATED_CURR * LOSS_PH_CURR)))
   { 
     if(1!=(g_MeterEventWrong[2]&0x01)) //Cʧѹģ��
     {
       Data1[14] &= 0xfb;
     }
     g_nFlg_Alarm = 1;
   }
   else 
   {
     // if(pDataTable->sRealInstant.bsRunStat[5] & 0x01)                     //ʧѹ
     if ((pDataTable->fVolt[2] < (g_nVolt * LOSS_VOLT_OC_RATIO)) && (pDataTable->fCurr[2] > (RATED_CURR * OC_CURR)))
       {
          g_nFlgUc = 1;
          g_nFlg_Alarm = 1;
       }
       else
       {
          if(1!=(g_MeterEventWrong[2]&0x01)) //Cʧѹģ��
          {
           Data1[14] |= 0x04;
          }
       }   
   }
 if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType))//��������������������
   {
 
 if(((pDataTable->sRealInstant.bsRunStat[6]&0x01)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealVolt[0].nFrac, 2, 0)))&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealVolt[1].nFrac, 2, 0)))
            &&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealVolt[2].nFrac, 2, 0))))||((pDataTable->sRealInstant.bsRunStat[6]&0x02)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealCurr[0].nD1, 3, 0)))&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealCurr[1].nD1, 3, 0)))
        &&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealCurr[2].nD1, 3, 0)))))//��ѹ������
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
     //if((pDataTable->sRealInstant.bsRunStat[6]&0x01)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealVolt[0].nFrac, 2, 0)))&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealVolt[2].nFrac, 2, 0))))//��ѹ������
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

   if((pDataTable->fCurr[0]/1000 < (1.5 * 0.005))&&(pDataTable->fCurr[1]/1000 < (1.5 * 0.005)&&(pDataTable->fCurr[2]/1000 < (1.5 * 0.005))))                                //��������
    {
      if(0x04!=(g_MeterEventWrong[2]&0x04)) //Cʧ��ģ��
      {
        Data1[11] &= 0xf3;
      }
      if(0x04 !=(g_MeterEventWrong[1]&0x04)) //Bʧ��ģ��
      {
         Data1[12] &= 0xf3;
      }
      if(0x04!=(g_MeterEventWrong[0]&0x04)) //Aʧ��ģ��
      {
         Data1[13] &= 0xf3;
      }
      g_nFlg_Alarm = 1;
    }
   else
   {  
        //if((pDataTable->fCurr[0] < (1.5 * 0.005))&&(pDataTable->fVolt[0] < (g_nVolt * LOSS_PH_OC_RATIO)))                                     //��������
        if(pDataTable->fCurr[0]/1000 < (1.5 * 0.005))
        {   
           //if(0x04!=(g_MeterEventWrong[0]&0x04)) //Aʧ��ģ��
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
         // if(pDataTable->sRealInstant.bsRunStat[3] & 0x08)                        //ʧ��     
         if ((((pDataTable->fVolt[0] > (g_nVolt * LOSS_CURR_RATIO)))
                && ((pDataTable->fCurr[0] > (RATED_CURR * RE_CURR)) || (pDataTable->fCurr[1] > (RATED_CURR * RE_CURR)) || (pDataTable->fCurr[2] > (RATED_CURR * RE_CURR))))
                    &&(pDataTable->fCurr[0] < RATED_CURR * OC_CURR))
          {  
              g_nFlgIa = 1;
              g_nFlg_Alarm = 1;
          }
         else if((pDataTable->sRealInstant.bsRunStat[3] != 8)&&(pDataTable->sRealInstant.bsRunStat[3] != 0x80))
          {  
            
             if(0x04!=(g_MeterEventWrong[0]&0x04)) //Aʧ��ģ��
             {
               Data1[13] |= 0x08; 
             }
          //  if((pDataTable->nPFlag & (0x01 << 3))&&(!(pDataTable->sRealInstant.bsRunStat[3] & 0x01)))   //����������
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
        if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType))//��������������������
        {
            //if((pDataTable->fCurr[1] < (1.5 * 0.005))&&(pDataTable->fVolt[1] < (g_nVolt * LOSS_PH_OC_RATIO)))                                   //��������
            if (pDataTable->fCurr[1]/1000 < (1.5 * 0.005))
            { 
              //if(0x04!=(g_MeterEventWrong[1]&0x04)) //Bʧ��ģ��
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
              //if(pDataTable->sRealInstant.bsRunStat[4] & 0x08)          //ʧ��                      
              if ((((pDataTable->fVolt[1] > (g_nVolt * LOSS_CURR_RATIO)))
                    && ((pDataTable->fCurr[0] > (RATED_CURR * RE_CURR)) || (pDataTable->fCurr[1] > (RATED_CURR * RE_CURR)) || (pDataTable->fCurr[2] > (RATED_CURR * RE_CURR))))
                        &&(pDataTable->fCurr[1] < RATED_CURR * OC_CURR))
              {  
                  g_nFlgIb = 1;
                  g_nFlg_Alarm = 1;
              }
             else if((pDataTable->sRealInstant.bsRunStat[4] != 8)&&(pDataTable->sRealInstant.bsRunStat[4] != 0x80))
              { 
                if(0x04!=(g_MeterEventWrong[1]&0x04)) //Bʧ��ģ��
                {
                  Data1[12] |= 0x08;
                }
               //if((pDataTable->nPFlag & (0x01 << 3))&&(!(pDataTable->sRealInstant.bsRunStat[4] & 0x01)))                 //���������� 
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
        //if((pDataTable->fCurr[2] < (1.5 * 0.005))&&(pDataTable->fVolt[2] < (g_nVolt * LOSS_PH_OC_RATIO)))                                   //��������
        if (pDataTable->fCurr[2]/1000 < (1.5 * 0.005))
        { 
          //if(0x04!=(g_MeterEventWrong[2]&0x04)) //Cʧ��ģ�� 
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
         //if(pDataTable->sRealInstant.bsRunStat[5] & 0x08)   //ʧ��                              
          if ((((pDataTable->fVolt[2] > (g_nVolt * LOSS_CURR_RATIO)))
                && ((pDataTable->fCurr[0] > (RATED_CURR * RE_CURR)) || (pDataTable->fCurr[1] > (RATED_CURR * RE_CURR)) || (pDataTable->fCurr[2] > (RATED_CURR * RE_CURR))))
                    &&(pDataTable->fCurr[2] < RATED_CURR * OC_CURR))
          {  
              g_nFlgIc = 1;
              g_nFlg_Alarm = 1;
          }
          else if((pDataTable->sRealInstant.bsRunStat[5] != 8)&&(pDataTable->sRealInstant.bsRunStat[5] != 0x80))
          { 
            if(0x04!=(g_MeterEventWrong[2]&0x04)) //Cʧ��ģ��
            {
              Data1[11] |= 0x08;
            }
           // if((pDataTable->nPFlag & (0x01 << 3))&&(!(pDataTable->sRealInstant.bsRunStat[5] & 0x01)))        //���������� 
            if((pDataTable->nPFlag&0x04)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[3].nFrac0, 3, 0))))               //�жϷ���
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
 
   
   if(1==g_MeterEventWrong[3]) ////������
   {
       Data1[14] |= 0x08; 
   }
   
   if(((g_MeterInsideWrong[0]&0x80)==0x80)||(g_ClockWrong2 ==1)) //���ص��
   {
       Data1[9] |= 0x04;
   }
   else
   { 
      Data1[9] &= 0xFB;
   }
   if(((g_MeterInsideWrong[1]&0x01)==1)||(g_ClockWrong ==1)) //ʱ�ӵ��
   {
       Data1[9] |= 0x08;
   }
   else
   {
       Data1[9] &= 0xF7;
   }
 
   if(0x02==(g_MeterEventWrong[0]&0x02)) //A���಻��ʾ
   {
      Data1[15] &= 0xf7;
      Data1[13] &= 0xf3;
   }
   if(0x02==(g_MeterEventWrong[1]&0x02)) //B���಻��ʾ
   {
      Data1[15] &= 0xfb;
      Data1[12] &= 0xf3;
   }
   if(0x02==(g_MeterEventWrong[2]&0x02)) //C���಻��ʾ
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

   if ((0x40 == (g_MeterInsideWrong[0] & 0x40)) || (1 == tEvtTrip.nEvtTrip)) //��բ
   {
       Data1[10] |= 0x01;
       GpioClearOutputEX(0x42);
   }
   else
   {
       Data1[10] &= 0xfe;
       GpioSetOutputEX(0x42);
   } 
   /*   ģ�黯����޴���ܣ�����������ʾ�빺��������
   if (1 == tAlarm.nAlarm)//����澯
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
* �� �� ��:  LcdDisplayPage1
* �䡡  ��:
*                                 
* �䡡  ��:
* ��    ��:
*          
* ��������:
*                                     
******************************************************************************/
//�� 1 ��

void  LcdDisplayPage1(void)                  //ÿ��ˢ��һ��
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
    if(0x02==(g_MeterInsideWrong[0]&0x02)) //ʱ����ʾ����
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
    if(0x20==(g_MeterInsideWrong[0]&0x20)) //ʱ�ӹ���
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
    Data[31] += 0x01;// ��ǰ
    
    Data[2] &= 0xF0;
    Data[3] &= 0xF6;


    BULCDWrite1(0, Data, 35);     
}
//�� 2 ��
void  LcdDisplayPage2(void)                  //ÿ��ˢ��һ��
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

    if(0x02==(g_MeterInsideWrong[0]&0x02)) //ʱ����ʾ����ģ��
    {
        /*nBuf[1]+=0x10; //��ʮ����
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
    if(0x20==(g_MeterInsideWrong[0]&0x20)) //ʱ�ӹ���
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
    Data[31] += 0x01;// ��ǰ
    Data[2] += 0x04;
    Data[2] &= 0xF6;
    Data[3] &= 0xFD;
    
    BULCDWrite1(0, Data, 35);     
}
//�� 3 ��  ��ǰ����й��ܵ���  

void  LcdDisplayPage3(void)                  //ÿ��ˢ��һ��
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
     Data[31] += 0x01;                   //��ǰ
     
      BULCDWrite1(0, Data, 35);
}

//�� 4 ��  (��ǰ)�����й��ܵ���   
void  LcdDisplayPage4(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x06;                  //����
    Data[31] += 0x01;                   //��ǰ
    BULCDWrite1(0, Data, 35);
}
//�� 5 ��   (��ǰ)�����й����� 1 ���� 
void  LcdDisplayPage5(void)                  //ÿ��ˢ��һ��
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
     Data[25] += 0x06;                  //����
     Data[31] += 0x01;                   //��ǰ
     BULCDWrite1(0, Data, 35);
}

//�� 6 ��   (��ǰ)�����й����� 2 ���� 
void  LcdDisplayPage6(void)                  //ÿ��ˢ��һ��
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
      Data[25] += 0x06;                  //����
     Data[31] += 0x01;                     //��ǰ
    BULCDWrite1(0, Data, 35);
}

//�� 7��   (��ǰ)�����й����� 3 ���� 
void  LcdDisplayPage7(void)                  //ÿ��ˢ��һ��
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
      Data[25] += 0x06;                  //����
     Data[31] += 0x01;                     //��ǰ
    BULCDWrite1(0, Data, 35);
}
//�� 8 ��   (��ǰ)�����й����� 4 ���� 
void  LcdDisplayPage8(void)                  //ÿ��ˢ��һ��
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
     Data[25] += 0x06;                  //����
     Data[31] += 0x01;             //��ǰ
    BULCDWrite1(0, Data, 35);
}
//�� 9 ��   ��ǰ�����й����������
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

    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
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
    Data[31] += 0x01;                   //��ǰ
    Data[2] &= 0xF0;
    Data[3] &= 0xF6;
    BULCDWrite1(0, Data, 35);
}
//�� 10 ��   ��ǰ�����й������������������
void  LcdDisplayPage10(void)                  //ÿ��ˢ��һ��
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
    Data[31] += 0x01;                   //��ǰ

    Data[2] += 0x04;
    Data[2] &= 0xF6;
    Data[3] &= 0xFD;

    BULCDWrite1(0, Data, 35);     
}
//�� 11 ��   ��ǰ�����й��������������ʱ��
void  LcdDisplayPage11(void)                  //ÿ��ˢ��һ��
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
    Data[31] += 0x01;// ��ǰ

    Data[2] += 0x04;
    Data[2] &= 0xF4;
    Data[3] &= 0xFF;

    BULCDWrite1(0, Data, 35);     
}
//�� 12 ��  (��ǰ)�����й��ܵ���   
void  LcdDisplayPage12(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x02;                    //����      
    Data[26] += 0x01;
    Data[31] += 0x01;                     //��ǰ
       
      BULCDWrite1(0, Data, 35);
}
//�� 13 ��   (��ǰ)�����й����� 1 ���� 
void  LcdDisplayPage13(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x02;                    //����      
    Data[26] += 0x01;
    Data[31] += 0x01;                     //��ǰ

    BULCDWrite1(0, Data, 35);
}

//�� 14 ��   (��ǰ)�����й����� 2 ���� 
void  LcdDisplayPage14(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x02;                    //����      
    Data[26] += 0x01;
    Data[31] += 0x01;                     //��ǰ

    BULCDWrite1(0, Data, 35);
}

//�� 15 ��   (��ǰ)�����й����� 3 ���� 
void  LcdDisplayPage15(void)                  //ÿ��ˢ��һ��
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
     Data[25] += 0x02;                    //����      
     Data[26] += 0x01;
     Data[31] += 0x01;                     //��ǰ
    
    BULCDWrite1(0, Data, 35);
}
//�� 16 ��   (��ǰ)�����й����� 4 ���� 
void  LcdDisplayPage16(void)                  //ÿ��ˢ��һ��
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
     Data[25] += 0x02;                    //����      
     Data[26] += 0x01;
     Data[31] += 0x01;                     //��ǰ
    
    BULCDWrite1(0, Data, 35);
}
//�� 17 ��   ��ǰ�����й����������
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

    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
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
    Data[31] += 0x01;                   //��ǰ
    Data[2] &= 0xF0;
    Data[3] &= 0xF6;
    BULCDWrite1(0, Data, 35);
}

//�� 18 ��   ��ǰ�����й������������������
void  LcdDisplayPage18(void)                  //ÿ��ˢ��һ��
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
    Data[31] += 0x01;                   //��ǰ
    Data[2] += 0x04;
    Data[2] &= 0xF6;
    Data[3] &= 0xFD;
    BULCDWrite1(0, Data, 35);     
}
//�� 19 ��   ��ǰ�����й��������������ʱ��
void  LcdDisplayPage19(void)                  //ÿ��ˢ��һ��
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
    Data[31] += 0x01;// ��ǰ
    Data[2] += 0x04;
    Data[2] &= 0xF4;
    Data[3] &= 0xFF;
    BULCDWrite1(0, Data, 35);     
}
//�� 20 ��   ��ǰ����޹� 1�� ���� 
void  LcdDisplayPage20(void)                  //ÿ��ˢ��һ��
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
     Data[31] += 0x01;// ��ǰ
    
    BULCDWrite1(0, Data, 35);
}

//�� 21 ��   ��ǰ����޹� 2 �� ����
void  LcdDisplayPage21(void)                  //ÿ��ˢ��һ��
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
     Data[31] += 0x01;// ��ǰ
    
    BULCDWrite1(0, Data, 35);

}
//�� 22 ��   ��ǰ�� 1 �����޹��ܵ��� 
void  LcdDisplayPage22(void)                  //ÿ��ˢ��һ��
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
     Data[31] += 0x01;                     //��ǰ

    BULCDWrite1(0, Data, 35);
}

//�� 23��   ��ǰ�� 2 �����޹��ܵ��� 
void  LcdDisplayPage23(void)                  //ÿ��ˢ��һ��
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
     Data[31] += 0x01;                     //��ǰ
    
    BULCDWrite1(0, Data, 35);
}

//�� 24 ��   ��ǰ�� 3�� ���޹��ܵ��� 
void  LcdDisplayPage24(void)                  //ÿ��ˢ��һ��
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
     Data[31] += 0x01;                     //��ǰ
    
    BULCDWrite1(0, Data, 35);
}

//�� 25 ��   ��ǰ�� 4 �����޹��ܵ��� 
void  LcdDisplayPage25(void)                  //ÿ��ˢ��һ��
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
     Data[31] += 0x01;                     //��ǰ
    
    BULCDWrite1(0, Data, 35);
}

//�� 26 ��  �� 1�� �� ���й��� ����
void  LcdDisplayPage26(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x06;                  //����
    Data[28] += 0x01;                  //�� 1��
    Data[29] += 0x06;                  
    Data[31] += 0x02; 
     
    BULCDWrite1(0, Data, 35);
}



//�� 27 ��   �� 1�������й����� 1 ���� 
void  LcdDisplayPage27(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x06;                  //����
    Data[28] += 0x01;                  //�� 1��
    Data[29] += 0x06;                  
    Data[31] += 0x02; 
    BULCDWrite1(0, Data, 35);
}

//�� 28 ��   �� 1�������й����� 2 ���� 
void  LcdDisplayPage28(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x06;                  //����
    Data[28] += 0x01;                  //�� 1��
    Data[29] += 0x06;                  
    Data[31] += 0x02; 
    BULCDWrite1(0, Data, 35);
}

//�� 29 ��   �� 1�������й����� 3 ���� 
void  LcdDisplayPage29(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x06;                  //����
    Data[28] += 0x01;                  //�� 1��
    Data[29] += 0x06;                  
    Data[31] += 0x02; 
    BULCDWrite1(0, Data, 35);
}
//�� 30 ��   �� 1�������й����� 4 ���� 
void  LcdDisplayPage30(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x06;                  //����
    Data[28] += 0x01;                  //�� 1��
    Data[29] += 0x06;                  
    Data[31] += 0x02; 
    BULCDWrite1(0, Data, 35);
}
//�� 31 ��   �� 1�������й����������
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

    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
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
    Data[31] += 0x02;           //�� 1��
    Data[2] &= 0xF0;
    Data[3] &= 0xF6;
    BULCDWrite1(0, Data, 35);
}
//�� 32 ��   �� 1�������й������������������
void  LcdDisplayPage32(void)                  //ÿ��ˢ��һ��
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
    Data[31] += 0x02; //�� 1��

    Data[2] += 0x04;
    Data[2] &= 0xF6;
    Data[3] &= 0xFD;
    BULCDWrite1(0, Data, 35);     
}
//�� 33 ��   �� 1�������й��������������ʱ��
void  LcdDisplayPage33(void)                  //ÿ��ˢ��һ��
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
    Data[28] += 0x01;                  //�� 1��
    Data[29] += 0x06;                  
    Data[31] += 0x02; 

    Data[2] += 0x04;
    Data[2] &= 0xF4;
    Data[3] &= 0xFF;
    BULCDWrite1(0, Data, 35);     
}
//�� 34��  �� 1�·����й��ܵ���   
void  LcdDisplayPage34(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x02;                    //����      
    Data[26] += 0x01;
    Data[28] += 0x01;                  //�� 1��
    Data[29] += 0x06;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}
//�� 35 ��   �� 1�·����й����� 1 ���� 
void  LcdDisplayPage35(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x02;                    //����      
    Data[26] += 0x01;
    Data[28] += 0x01;                  //�� 1��
    Data[29] += 0x06;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}

//�� 36 ��   �� 1�·����й����� 2 ���� 
void  LcdDisplayPage36(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x02;                    //����      
    Data[26] += 0x01;
    Data[28] += 0x01;                  //�� 1��
    Data[29] += 0x06;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}

//�� 37 ��   �� 1�·����й����� 3 ���� 
void  LcdDisplayPage37(void)                  //ÿ��ˢ��һ��
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
     Data[25] += 0x02;                    //����      
     Data[26] += 0x01;
     Data[28] += 0x01;                  //�� 1��
     Data[29] += 0x06;                  
     Data[31] += 0x02; 
    
    BULCDWrite1(0, Data, 35);
}
//�� 38 ��   �� 1�·����й����� 4 ���� 
void  LcdDisplayPage38(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x02;                    //����      
    Data[26] += 0x01;
    Data[28] += 0x01;                  //�� 1��
    Data[29] += 0x06;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}
//�� 39��   �� 1�·����й����������
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
   
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
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
    Data[28] += 0x01;                  //�� 1��
    Data[29] += 0x06;                  
    Data[31] += 0x02; 
    
    Data[2] &= 0xF0;
    Data[3] &= 0xF6;
    BULCDWrite1(0, Data, 35);
}
//�� 40 ��   �� 1�·����й������������������
void  LcdDisplayPage40(void)                  //ÿ��ˢ��һ��
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
    Data[31] += 0x02;  //�� 1�� 

    Data[2] += 0x04;
    Data[2] &= 0xF6;
    Data[3] &= 0xFD;
    BULCDWrite1(0, Data, 35);     
}
//�� 41��   �� 1�·����й��������������ʱ��
void  LcdDisplayPage41(void)                  //ÿ��ˢ��һ��
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
    Data[28] += 0x01;                  //�� 1��
    Data[29] += 0x06;                  
    Data[31] += 0x02; 

    Data[2] += 0x04;
    Data[2] &= 0xF4;
    Data[3] &= 0xFF;
    BULCDWrite1(0, Data, 35);     
}
//�� 42 ��   �� 1�� �� 1�� ���޹��ܵ��� 
void  LcdDisplayPage42(void)                  //ÿ��ˢ��һ��
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
    Data[28] += 0x01;                  //�� 1��
    Data[29] += 0x06;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}

//�� 43��   �� 1�µ� 2 �����޹��ܵ��� 
void  LcdDisplayPage43(void)                  //ÿ��ˢ��һ��
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
    Data[28] += 0x01;                  //�� 1��
    Data[29] += 0x06;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}

//�� 44 ��   �� 1�µ� 3�� ���޹��ܵ��� 
void  LcdDisplayPage44(void)                  //ÿ��ˢ��һ��
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
    Data[28] += 0x01;                  //�� 1��
    Data[29] += 0x06;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}

//�� 45 ��   �� 1�µ� 4 �����޹��ܵ��� 
void  LcdDisplayPage45(void)                  //ÿ��ˢ��һ��
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
    Data[28] += 0x01;                  //�� 1��
    Data[29] += 0x06;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}
//�� 46��  ͨ�ŵ�ַ�� 8λ
void  LcdDisplayPage46(void)                  //ÿ��ˢ��һ��
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
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
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
//�� 47 ��  ͨ�ŵ�ַ�� 4λ
void  LcdDisplayPage47(void)                  //ÿ��ˢ��һ��
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
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
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
//�� 48 ��   ͨ�Ų�����
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
//�� 49 ��   �й����峣��   
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
      Data[24] += 0x03;         //�й�
    BULCDWrite1(0, Data, 35);
}
//�� 50 ��   �޹����峣��   
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
      Data[24] += 0x01;         //�޹�
      Data[25] += 0x01;         
    BULCDWrite1(0, Data, 35);
}

//�� 51 ��   ��ʧѹ����
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
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
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
//�� 52 ��   ��ʧѹ�ۼ�ʱ��
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

    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
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
//�� 53 ��  ���һ��ʧѹ��ʼ ����
void  LcdDisplayPage53(void)                  //ÿ��ˢ��һ��
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
//�� 54 �� ���һ��ʧѹ��ʼʱ��
void  LcdDisplayPage54(void)                  //ÿ��ˢ��һ��
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
//�� 55 ��  ���һ��ʧѹ���� ����
void  LcdDisplayPage55(void)                  //ÿ��ˢ��һ��
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
//�� 56 �� ���һ��ʧѹ���� ʱ��
void  LcdDisplayPage56(void)                  //ÿ��ˢ��һ��
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
// �� 57 ��  ���һ�� A�� ʧѹ��ʼʱ���� ���й� ����
void  LcdDisplayPage57(void)                  //ÿ��ˢ��һ��
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
     Data[25] += 0x06;                  //����
     Data[28] += 0x02;
     Data[29] += 0x06;
     Data[31] += 0x02;

      BULCDWrite1(0, Data, 35);
}
// �� 58 ��  ���һ�� A�� ʧѹ����ʱ���� ���й� ����
void  LcdDisplayPage58(void)                  //ÿ��ˢ��һ��
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
     Data[25] += 0x06;                  //����
     Data[28] += 0x02;
     Data[29] += 0x06;
     Data[31] += 0x02;

      BULCDWrite1(0, Data, 35);
}
// �� 59��  ���һ�� A�� ʧѹ��ʼʱ�̷� ���й� ����
void  LcdDisplayPage59(void)                  //ÿ��ˢ��һ��
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
     Data[26] += 0x01;                  //����
     Data[28] += 0x02;
     Data[29] += 0x06;
     Data[31] += 0x02;

      BULCDWrite1(0, Data, 35);
}
// �� 60 ��  ���һ�� A�� ʧѹ����ʱ�̷� ���й� ����
void  LcdDisplayPage60(void)                  //ÿ��ˢ��һ��
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
      Data[26] += 0x01;                  //����
     Data[28] += 0x02;
     Data[29] += 0x06;
     Data[31] += 0x02;

      BULCDWrite1(0, Data, 35);
}
// �� 61 ��  ���һ�� B�� ʧѹ��ʼʱ���� ���й� ����
void  LcdDisplayPage61(void)                  //ÿ��ˢ��һ��
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
     Data[25] += 0x06;                  //����
     Data[28] += 0x04;
     Data[29] += 0x06;
     Data[31] += 0x02;

      BULCDWrite1(0, Data, 35);
}
// �� 62 ��  ���һ�� B�� ʧѹ����ʱ���� ���й� ����
void  LcdDisplayPage62(void)                  //ÿ��ˢ��һ��
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
     Data[25] += 0x06;                  //����
     Data[28] += 0x04;
     Data[29] += 0x06;
     Data[31] += 0x02;

      BULCDWrite1(0, Data, 35);
}
// �� 63 ��  ���һ�� B�� ʧѹ��ʼʱ�̷� ���й� ����
void  LcdDisplayPage63(void)                  //ÿ��ˢ��һ��
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
     Data[26] += 0x01;                  //����
     Data[28] += 0x04;
     Data[29] += 0x06;
     Data[31] += 0x02;

      BULCDWrite1(0, Data, 35);
}
// �� 64 ��  ���һ�� B�� ʧѹ����ʱ�̷� ���й� ����
void  LcdDisplayPage64(void)                  //ÿ��ˢ��һ��
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
      Data[26] += 0x01;                  //����
     Data[28] += 0x04;
     Data[29] += 0x06;
     Data[31] += 0x02;

      BULCDWrite1(0, Data, 35);
}
// �� 65��  ���һ�� C�� ʧѹ��ʼʱ���� ���й� ����
void  LcdDisplayPage65(void)                  //ÿ��ˢ��һ��
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
     Data[25] += 0x06;                  //����
     Data[28] += 0x08;
     Data[29] += 0x06;
     Data[31] += 0x02;

      BULCDWrite1(0, Data, 35);
}
// �� 66 ��  ���һ�� C�� ʧѹ����ʱ���� ���й� ����
void  LcdDisplayPage66(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x06;                  //����
    Data[28] += 0x08;
    Data[29] += 0x06;
    Data[31] += 0x02;

    BULCDWrite1(0, Data, 35);
}
// �� 67 ��  ���һ�� C�� ʧѹ��ʼʱ�̷� ���й� ����
void  LcdDisplayPage67(void)                  //ÿ��ˢ��һ��
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
     Data[26] += 0x01;                  //����
     Data[28] += 0x08;
     Data[29] += 0x06;
     Data[31] += 0x02;

      BULCDWrite1(0, Data, 35);
}
// �� 68 ��  ���һ�� C�� ʧѹ����ʱ�̷� ���й� ����
void  LcdDisplayPage68(void)                  //ÿ��ˢ��һ��
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
      Data[26] += 0x01;                  //����
     Data[28] += 0x08;
     Data[29] += 0x06;
     Data[31] += 0x02;

      BULCDWrite1(0, Data, 35);
}
//�� 69 ��   A���ѹ
void  LcdDisplayPage69(void)                  //ÿ��ˢ��һ��
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[2];
    INT8U       nBuf1[10]; 
    INT8U       nBuf2[16];
     INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x02010100;
     INT32U      nDI         = 0x20000201;
    MEM_AC_REAL_DATA            sRealData;             //ʹ��ȫ�ֱ���
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    memcpy(Data,Data1,35); 
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    memcpy(nBuf, &pDataTable->sRealInstant.sRealVolt[0].nFrac, 2);
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }
    
    //A���ѹΪ50%
    if (((g_MeterHardwareWrong[1]&0x04)==0x04) || (1 == g_MeterEventWrong[0]&0x01))
    { 
       DivideTwo(nBuf);
    }
    
    if (0x11 == (g_MeterVoltPlusFlg&0x11))//��ѹԽ����
    {
        nBuf[1] |= 0x05;
    }
    else if((0x21 == (g_MeterVoltPlusFlg&0x21))||(g_MeterVoltPlusFlg == 0x01))//��ѹԽ����
    {
        DivideTwo2(nBuf); //DivideTwo(nBuf);
    }
    
    //A���޵�ѹ
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
//�� 70��   B ���ѹ
void  LcdDisplayPage70(void)                  //ÿ��ˢ��һ��
{    
    INT8U       i  = 0;   
    INT8U       Data[35] ;
    INT8U       nBuf[2];
    INT8U       nBuf1[10];
    INT8U       nBuf2[16];
     INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x02010200;
     INT32U      nDI         = 0x20000202;
     
    MEM_AC_REAL_DATA            sRealData;             //ʹ��ȫ�ֱ���
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    memset(nBuf, 0, sizeof(nBuf));
    memcpy(Data,Data1,35); 
if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType))//����������������������
{
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    memcpy(nBuf, &pDataTable->sRealInstant.sRealVolt[1].nFrac, 2);
}  
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }

    //B���ѹΪ50%
    if(((g_MeterHardwareWrong[1]&0x08)==0x08) || (1 == g_MeterEventWrong[1]&0x01))
    { 
       DivideTwo(nBuf);
    }
    
    if (0x12 == (g_MeterVoltPlusFlg&0x12))//��ѹԽ����
    {
        nBuf[1] |= 0x05;
    }
    else if((0x22 == (g_MeterVoltPlusFlg&0x22))||(g_MeterVoltPlusFlg == 0x01))//��ѹԽ����
    {
        DivideTwo2(nBuf); //DivideTwo(nBuf);
    }

    //B���޵�ѹ
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
//�� 71 ��   C ���ѹ
void  LcdDisplayPage71(void)                  //ÿ��ˢ��һ��
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[2];
    INT8U       nBuf1[10];
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x02010300;
    INT32U      nDI         = 0x20000203;
    
    MEM_AC_REAL_DATA            sRealData;             //ʹ��ȫ�ֱ���
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    memcpy(Data,Data1,35); 
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    memcpy(nBuf, &pDataTable->sRealInstant.sRealVolt[2].nFrac, 2);
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }
    
    //C���ѹΪ50%
    if(((g_MeterHardwareWrong[1]&0x10)==0x10) || (1 == g_MeterEventWrong[2]&0x01))
    {
       DivideTwo(nBuf);
    }
    
    if (0x14 == (g_MeterVoltPlusFlg&0x14))//��ѹԽ����
    {
        nBuf[1] |= 0x05;
    }
    else if((0x24 == (g_MeterVoltPlusFlg&0x24))||(g_MeterVoltPlusFlg == 0x01))//��ѹԽ����
    {
       DivideTwo2(nBuf); //DivideTwo(nBuf);
    }
    
    //C���޵�ѹ
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
//�� 72��   A �����
void  LcdDisplayPage72(void)                  //ÿ��ˢ��һ��
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[3];
    INT8U       nBuf1[10];
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x02020100;
    INT32U      nDI         = 0x20010201;
    
    MEM_AC_REAL_DATA            sRealData;             //ʹ��ȫ�ֱ���
    pMEM_AC_REAL_DATA           pRealData = &sRealData;        
     
    memcpy(Data,Data1,35); 
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    memcpy(nBuf,  &pDataTable->sRealInstant.sRealCurr[0].nD1, 3);
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }
    //A���޵���
    if (((g_MeterHardwareWrong[0]&0x80)==0x80) || (0x04==(g_MeterEventWrong[0]&0x04)) || (0x02==(g_MeterEventWrong[0]&0x02)))
    {
       memset(nBuf, 0, sizeof(nBuf));
    }
    //A�����Ϊ50%
    if((g_MeterHardwareWrong[1]&0x20)==0x20)
    { 
       DivideTwo(nBuf);
    }
     if(pDataTable->fCurr[0] < (1.5 * 0.005))
    {
      
    }
    else
    {
        //if((pDataTable->nPFlag & (0x01 << 3))&&(!(pDataTable->sRealInstant.bsRunStat[3] & 0x01)))   //�жϷ���
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

//�� 73 ��   B �����
void  LcdDisplayPage73(void)                  //ÿ��ˢ��һ��
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[3];
    INT8U       nBuf1[10]; 
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x02020200;
    INT32U      nDI         = 0x20010202;
    
    MEM_AC_REAL_DATA            sRealData;             //ʹ��ȫ�ֱ���
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    memset(nBuf, 0, sizeof(nBuf));
    memcpy(Data,Data1,35); 
    if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType))//����������������������
    {
        MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
        memcpy(nBuf, &pDataTable->sRealInstant.sRealCurr[1].nD1, 3);
    }
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }
    //B���޵���
    if (((g_MeterHardwareWrong[1]&0x01)==0x01) || (0x04==(g_MeterEventWrong[1]&0x04)) || (0x02==(g_MeterEventWrong[1]&0x02)))
    {
       memset(nBuf, 0, sizeof(nBuf));
    }
    //B�����Ϊ50%
    if((g_MeterHardwareWrong[1]&0x40)==0x40)
    { 
       DivideTwo(nBuf);
    }

    if(pDataTable->fCurr[1] < (1.5 * 0.005))
    {
      
    }
    else
    {
        //if((pDataTable->nPFlag & (0x01 << 3))&&(!(pDataTable->sRealInstant.bsRunStat[4] & 0x01)))   //�жϷ���
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
//�� 74 ��   C �����
void  LcdDisplayPage74(void)                  //ÿ��ˢ��һ��
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[3];
    INT8U       nBuf1[10];
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x02020300;
    INT32U      nDI         = 0x20010203;
    MEM_AC_REAL_DATA            sRealData;             //ʹ��ȫ�ֱ���
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    memcpy(Data,Data1,35); 
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    memcpy(nBuf, &pDataTable->sRealInstant.sRealCurr[2].nD1, 3);
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }
    //C���޵���
    if (((g_MeterHardwareWrong[1]&0x02)==0x02)  || (0x04==(g_MeterEventWrong[2]&0x04)) || (0x02==(g_MeterEventWrong[2]&0x02)))
    {
       memset(nBuf, 0, sizeof(nBuf));
    }
    //C�����Ϊ50%
    if ((g_MeterHardwareWrong[1]&0x80)==0x80)
    { 
       DivideTwo(nBuf);
    }
    
    
    if(pDataTable->fCurr[2] < (1.5 * 0.005))
    {
      
    }
    else
    {
        //if((pDataTable->nPFlag & (0x01 << 3))&&(!(pDataTable->sRealInstant.bsRunStat[5] & 0x01)))   //�жϷ���
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

//�� 75 ��   ˲ʱ���й�����
void  LcdDisplayPage75(void)                  //ÿ��ˢ��һ��
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[3];
    INT8U       nBuf1[10]; 
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x02030000;
    INT32U      nDI         = 0x20040201;
    MEM_AC_REAL_DATA            sRealData;             //ʹ��ȫ�ֱ���
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    memcpy(Data,Data1,35); 
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    memcpy(nBuf, &pDataTable->sRealInstant.sRealP[0].nFrac0, 3 );
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }
    if((pDataTable->nPFlag&0x08)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[0].nFrac0, 3, 0))))               //�жϷ���
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
//�� 76 ��   ˲ʱA ���й�����
void  LcdDisplayPage76(void)                  //ÿ��ˢ��һ��
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[3];
    INT8U       nBuf1[10];
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x02030100;
    INT32U      nDI         = 0x20040202;
    MEM_AC_REAL_DATA            sRealData;             //ʹ��ȫ�ֱ���
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    memcpy(Data,Data1,35); 
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    memcpy(nBuf, &pDataTable->sRealInstant.sRealP[1].nFrac0, 3 );
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }
     if((pDataTable->nPFlag&0x01)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[1].nFrac0, 3, 0))))               //�жϷ���
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
//�� 77 ��   ˲ʱ B ���й�����
void  LcdDisplayPage77(void)                  //ÿ��ˢ��һ��
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[3];
    INT8U       nBuf1[10];
    INT8U       nBuf2[16];
     INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x02030200;
     INT32U      nDI         = 0x20040203;
    MEM_AC_REAL_DATA            sRealData;             //ʹ��ȫ�ֱ���
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    memset(nBuf, 0, sizeof(nBuf));
    memcpy(Data,Data1,35); 
  if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType))//����������������������
  {
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    memcpy(nBuf, &pDataTable->sRealInstant.sRealP[2].nFrac0, 3 );
  }
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }
     if((pDataTable->nPFlag&0x02)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[2].nFrac0, 3, 0))))               //�жϷ���
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
//�� 78 ��   ˲ʱ C ���й�����
void  LcdDisplayPage78(void)                  //ÿ��ˢ��һ��
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[3];
    INT8U       nBuf1[10];
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x02030300;
    INT32U      nDI         = 0x20040204;
    MEM_AC_REAL_DATA            sRealData;             //ʹ��ȫ�ֱ���
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    memcpy(Data,Data1,35); 
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    memcpy(nBuf, &pDataTable->sRealInstant.sRealP[3].nFrac0, 3 );
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }
    if((pDataTable->nPFlag&0x04)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[3].nFrac0, 3, 0))))               //�жϷ���
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
//�� 79 ��   ˲ʱ�ܹ��� ����
void  LcdDisplayPage79(void)                  //ÿ��ˢ��һ��
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[2];
    INT8U       nBuf1[10];
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x02060000 ;
    INT32U      nDI         = 0x200A0201;
    MEM_AC_REAL_DATA            sRealData;             //ʹ��ȫ�ֱ���
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    memcpy(Data,Data1,35); 
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    memcpy(nBuf, &pDataTable->sRealInstant.sRealPf[0].nFrac, 2 );
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }
    nBuf[1] &= ~0x80;
    for(i = 0; i < 2; i ++ )
    {
      nBuf1[i*2]  = nBuf[i]&0x0f;
      nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }
      if((pDataTable->nPFlag&0x08)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[0].nFrac0, 3, 0))))               //�жϷ���
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
//�� 80 ��   A �๦������ 
void  LcdDisplayPage80(void)                  //ÿ��ˢ��һ��
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[2];
    INT8U       nBuf1[10]; 
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x02070100 ;
    INT32U      nDI         = 0x20030201;
    MEM_AC_REAL_DATA            sRealData;             //ʹ��ȫ�ֱ���
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    memcpy(Data,Data1,35); 
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    memcpy(nBuf, &pDataTable->sRealInstant.sRealPg[0].nFrac, 2 );
     if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }
    nBuf[1] &= ~0x80;
    for(i = 0; i < 2; i ++ )
    {
      nBuf1[i*2]  = nBuf[i]&0x0f;
      nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }
      if((pDataTable->nPFlag&0x01)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[1].nFrac0, 3, 0))))               //�жϷ���
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
//�� 81 ��   B �๦������ 
void  LcdDisplayPage81(void)                  //ÿ��ˢ��һ��
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[2];
    INT8U       nBuf1[10] ;
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x02070200 ;
    INT32U      nDI         = 0x20030202;
    MEM_AC_REAL_DATA            sRealData;             //ʹ��ȫ�ֱ���
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    memset(nBuf, 0, sizeof(nBuf));
    memcpy(Data,Data1,35); 
    if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType))//����������������������
    {
        MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
        memcpy(nBuf, &pDataTable->sRealInstant.sRealPg[1].nFrac, 2 );
    }
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    }
    nBuf[1] &= ~0x80;
    for(i = 0; i < 2; i ++ )
    {
      nBuf1[i*2]  = nBuf[i]&0x0f;
      nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }
     if((pDataTable->nPFlag&0x02)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[2].nFrac0, 3, 0))))               //�жϷ���
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
//�� 82 ��   C �๦������ 
void  LcdDisplayPage82(void)                  //ÿ��ˢ��һ��
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[2];
    INT8U       nBuf1[10]; 
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x02070300 ;
    INT32U      nDI         = 0x20030203;
    MEM_AC_REAL_DATA            sRealData;             //ʹ��ȫ�ֱ���
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    memcpy(Data,Data1,35); 
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    memcpy(nBuf, &pDataTable->sRealInstant.sRealPg[2].nFrac, 2 );
     if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
    {
      memset(nBuf, 0, sizeof(nBuf));
    } 
    
    nBuf[1] &= ~0x80;
    
    for(i = 0; i < 2; i ++ )
    {
      nBuf1[i*2]  = nBuf[i]&0x0f;
      nBuf1[i*2+1]  = nBuf[i]>>0x04;
    }
    
    if((pDataTable->nPFlag&0x04)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[3].nFrac0, 3, 0))))               //�жϷ���
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

//�� 83 ��  �� 2�� �� ���й��� ����
void  LcdDisplayPage83(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x06;                  //����
    Data[28] += 0x01;                  //�� 1��
    Data[29] += 0x0D;                  //1 0x06  
    Data[30] += 0x06;
    Data[31] += 0x02; 
     
    BULCDWrite1(0, Data, 35);
}

//�� 84��  �� 2�·����й��ܵ���   
void  LcdDisplayPage84(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x02;                    //����      
    Data[26] += 0x01;
    Data[28] += 0x01;                  //�� 1��
    Data[29] += 0x0D;                  
    Data[30] += 0x06;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}

//�� 85��  �� 3�� �� ���й��� ����
void  LcdDisplayPage85(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x06;                  //����
    Data[28] += 0x01;                  //�� 2��
    Data[29] += 0x0F;                   
    Data[30] += 0x04;
    Data[31] += 0x02; 
     
    BULCDWrite1(0, Data, 35);
}

//�� 86��  �� 3�·����й��ܵ���   
void  LcdDisplayPage86(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x02;                    //����      
    Data[26] += 0x01;
    Data[28] += 0x01;                  //�� 2��
    Data[29] += 0x0F;                  
    Data[30] += 0x04;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}

//�� 87��  �� 4�� �� ���й��� ����
void  LcdDisplayPage87(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x06;                  //����
    Data[28] += 0x01;                  //�� 3��
    Data[29] += 0x06;                  //  
    Data[30] += 0x0C;
    Data[31] += 0x02; 
     
    BULCDWrite1(0, Data, 35);
}

//�� 88��  �� 4�·����й��ܵ���   
void  LcdDisplayPage88(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x02;                    //����      
    Data[26] += 0x01;
    Data[28] += 0x01;                  //�� 3��
    Data[29] += 0x06;                  //  
    Data[30] += 0x0C;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}

//�� 89��  �� 5�� �� ���й��� ����
void  LcdDisplayPage89(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x06;                  //����
    Data[28] += 0x01;                  //�� 3��
    Data[29] += 0x0B;                  //  
    Data[30] += 0x0C;
    Data[31] += 0x02; 
     
    BULCDWrite1(0, Data, 35);
}

//�� 90��  �� 5�·����й��ܵ���   
void  LcdDisplayPage90(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x02;                    //����      
    Data[26] += 0x01;
    Data[28] += 0x01;                  //�� 3��
    Data[29] += 0x0B;                  //  
    Data[30] += 0x0C;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}

//�� 91��  �� 6�� �� ���й��� ����
void  LcdDisplayPage91(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x06;                  //����
    Data[28] += 0x01;                  //�� 3��
    Data[29] += 0x0B;                  //  
    Data[30] += 0x0E;
    Data[31] += 0x02; 
     
    BULCDWrite1(0, Data, 35);
}

//�� 92��  �� 6�·����й��ܵ���   
void  LcdDisplayPage92(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x02;                    //����      
    Data[26] += 0x01;
    Data[28] += 0x01;                  //�� 3��
    Data[29] += 0x0B;                  //  
    Data[30] += 0x0E;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}


//�� 93��  �� 7�� �� ���й��� ����
void  LcdDisplayPage93(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x06;                  //����
    Data[28] += 0x01;                  //�� 3��
    Data[29] += 0x0E;                  //  
    //Data[30] += 0x0E;
    Data[31] += 0x02; 
     
    BULCDWrite1(0, Data, 35);
}

//�� 94��  �� 7�·����й��ܵ���   
void  LcdDisplayPage94(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x02;                    //����      
    Data[26] += 0x01;
    Data[28] += 0x01;                  //�� 3��
    Data[29] += 0x0E;                  //  
    //Data[30] += 0x0E;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}

//�� 95��  �� 8�� �� ���й��� ����
void  LcdDisplayPage95(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x06;                  //����
    Data[28] += 0x01;                  //�� 3��
    Data[29] += 0x0F;                  //  
    Data[30] += 0x0E;
    Data[31] += 0x02; 
     
    BULCDWrite1(0, Data, 35);
}

//�� 96��  �� 8�·����й��ܵ���   
void  LcdDisplayPage96(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x02;                    //����      
    Data[26] += 0x01;
    Data[28] += 0x01;                  //�� 3��
    Data[29] += 0x0F;                  //  
    Data[30] += 0x0E;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}

//�� 97��  �� 9�� �� ���й��� ����
void  LcdDisplayPage97(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x06;                  //����
    Data[28] += 0x01;                  //�� 3��
    Data[29] += 0x0F;                  //  
    Data[30] += 0x0C;
    Data[31] += 0x02; 
     
    BULCDWrite1(0, Data, 35);
}

//�� 98��  �� 9�·����й��ܵ���   
void  LcdDisplayPage98(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x02;                    //����      
    Data[26] += 0x01;
    Data[28] += 0x01;                  //�� 3��
    Data[29] += 0x0F;                  //  
    Data[30] += 0x0C;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}

//�� 99��  �� 10�� �� ���й��� ����
void  LcdDisplayPage99(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x06;                  //����
    Data[28] += 0x01;                  //�� 3��
    Data[29] += 0x0F;                  //  
    Data[30] += 0x0B;
    Data[31] += 0x02; 
     
    BULCDWrite1(0, Data, 35);
}

//��100��  �� 10�·����й��ܵ���   
void  LcdDisplayPage100(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x02;                    //����      
    Data[26] += 0x01;
    Data[28] += 0x01;                  //�� 3��
    Data[29] += 0x0F;                  //  
    Data[30] += 0x0B;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}

//��101��  �� 11�� �� ���й��� ����
void  LcdDisplayPage101(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x06;                  //����
    Data[28] += 0x01;                  //�� 3��
    Data[29] += 0x06;                  //  
    Data[30] += 0x01;
    Data[31] += 0x02; 
     
    BULCDWrite1(0, Data, 35);
}

//��102��  �� 11�·����й��ܵ���   
void  LcdDisplayPage102(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x02;                    //����      
    Data[26] += 0x01;
    Data[28] += 0x01;                  //�� 3��
    Data[29] += 0x06;                  //  
    Data[30] += 0x01;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}

//��103��  �� 12�� �� ���й��� ����
void  LcdDisplayPage103(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x06;                  //����
    Data[28] += 0x01;                  //�� 3��
    Data[29] += 0x0D;                  //  
    Data[30] += 0x07;
    Data[31] += 0x02; 
     
    BULCDWrite1(0, Data, 35);
}

//��104��  �� 12�·����й��ܵ���   
void  LcdDisplayPage104(void)                  //ÿ��ˢ��һ��
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
    Data[25] += 0x02;                    //����      
    Data[26] += 0x01;
    Data[28] += 0x01;                  //�� 12��
    Data[29] += 0x0D;                  //  
    Data[30] += 0x07;                  
    Data[31] += 0x02; 

    BULCDWrite1(0, Data, 35);
}

//�� 105 ��   ʱ�ӵ����ʱ��
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

    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
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

//�� 106��  ���һ�α�� ����
void  LcdDisplayPage106(void)                  //ÿ��ˢ��һ��
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
//�� 107�� ���һ�α��ʱ��
void  LcdDisplayPage107(void)                  //ÿ��ˢ��һ��
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

//�� 108��   A��ʧѹ�ۼ�ʱ��
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

    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
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

//�� 108��   B��ʧѹ�ۼ�ʱ��
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

    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
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

//�� 110��   C��ʧѹ�ۼ�ʱ��
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

    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
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

//�� 111��   ���ߵ���
void  LcdDisplayPage111(void)                  //ÿ��ˢ��һ��
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[3];
    INT8U       nBuf1[10];
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x02020300;
    INT32U      nDI         = 0x20010400;
    MEM_AC_REAL_DATA            sRealData;             //ʹ��ȫ�ֱ���
    pMEM_AC_REAL_DATA           pRealData = &sRealData;

    memcpy(Data,Data1,35); 
    MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
    memcpy(nBuf, &pDataTable->sRealInstant.sRealCurr[2].nD1, 3);
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
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

//�� 112��   �������
void  LcdDisplayPage112(void)                  //ÿ��ˢ��һ��
{    
    INT8U       i  = 0;   
    INT8U       Data[35];
    INT8U       nBuf[3];
    INT8U       nBuf1[10];
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];
    //INT32U      nDI  = 0x02020300;
    INT32U      nDI         = 0x20010600;
    MEM_AC_REAL_DATA            sRealData;             //ʹ��ȫ�ֱ���
    pMEM_AC_REAL_DATA           pRealData = &sRealData;

    memcpy(Data,Data1,35); 
    if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType))//����������������������
    {
        MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
        memcpy(nBuf, &pDataTable->sRealInstant.sRealCurr[1].nD1, 3);
    }

    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
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

void  LcdDisplayPage113(void)                  //ÿ��ˢ��һ��
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
    //Data[31] += 0x01;// ��ǰ

    BULCDWrite1(0, Data, 35);  
}







//���ܵ�����
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
//�жϵ����� �ó�������Ŀ******************************************************
extern INT32U GetAD0Voltage();
extern INT8U g_nErrBuf[2];
extern INT8U g_ClockWrong;
void CheckMeterFault()
{  
//ʱ�ӵ�ص�ѹ�� ��ص�ѹ����3.3v���˴���
  if(g_ClockWrong==1) 
  {
   ERR04=1;
   MeterWrongNum++;
  }
  else
  {
  ERR04=0;
  }
//�ڲ�������� ���������ȫ�ֱ���У��Ͳ���
//�ڲ�erpom��д����
  if((g_nErrBuf[0]&0x01)==1||((g_nErrBuf[0]&0x02)>>1)==1)
  {
   ERR06=1;
   MeterWrongNum++;
  }
  else
  {
  ERR06=0;
  }
//���ʱ�ӹ��ϣ����Ӳʱ�Ӳ���������д
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
//�ж��¼����� ����¼�������Ŀ*************************************************
INT8U tmptime[5]={0};//������ƽ�⣬�����������ޣ������������й����ܷ���ı�ʱ��洢
INT8U WrongState[5]={0};//������ƽ�⣬�����������ޣ������������й����ܷ���ı��¼�����״���洢
INT8U Eventalarm=0;
INT8U Wrong_54flg=0;
void CheckMistake()
{  
   //����51 �û�ʹ�ø��ɴ��ڵ�1.2����������ʱ��������ԡ�Err��51��******************
   MEM_AC_REAL_DATA            sRealData;             //ʹ��ȫ�ֱ���
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
  //�������ز�ƽ��52*****************************************************
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
  //��ѹ53 ��ѹ����1.15��Un****************************************************
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
  //������������54 �û�������������С��0.2��������ԡ�Err��54��*****************
  if(second2==1)
   {  
    second2=0;
    if(Wrong_54flg == 0)  //���ϵ�ʱ������˴����ж�
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
  //���й���������55*******************************************************
   
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
   //�й����ܷ���ı�56****************************************************** 
  
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
//������ʾ�¼�����*************************************************************
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

//����������ʾ
void DataDeal(INT8U*  nBuf,INT32U    nDI,INT8U* Data)
{

    INT8U       i  = 0; 
    INT8U       j = 0 ;
    INT8U       nBuf1[8]; 
    INT8U       nBuf2[8];
    INT8U       nTemBuf[4];
    
    if((g_MeterInsideWrong[0]&0x04)==0x04) //����������ʾ����ģ��
    {
        memset(&nBuf, 0, 4); 
    }
    Long2Buf((INT8U*)&nTemBuf, nDI, 4); 
    if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
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
//����������ʾ
void Time1Deal(INT8U*  nBuf1,INT32U    nDI,INT8U* Data)
{
    INT8U       i  = 0; 
    INT8U       nBuf2[16];
    INT8U       nTemBuf[8];

    Long2Buf((INT8U*)&nTemBuf, nDI, 4);    
    if(!IsAllBcd(&nBuf1[0], 6))		/*BCD����Ч���ж�*/
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
//ʱ��������ʾ
void Time2Deal(INT8U*  nBuf1,INT32U    nDI,INT8U* Data)
{
    INT8U       i  = 0; 
   INT8U       nBuf2[16];
   INT8U       nTemBuf[8];
   
   INT8U  Data_P2[20] = {  0xf0, 0xb8,  0x60,0x18,   0xd0,0x78,  0xf0,0x58,   0x60,0xd8,  0xb0,0xd8,  0xb0,0xf8,  0xe0,0x18,  0xf0,0xf8,  0xf0,0xe8};//���롢��ǰ����

    Long2Buf((INT8U*)&nTemBuf, nDI, 4);
    if(!IsAllBcd(&nBuf1[0], 6))	/*BCD����Ч���ж�*/
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
   
   INT8U  Data_P2[20] = {  0xf0, 0xb8,  0x60,0x18,   0xd0,0x78,  0xf0,0x58,   0x60,0xd8,  0xb0,0xd8,  0xb0,0xf8,  0xe0,0x18,  0xf0,0xf8,  0xf0,0xe8};//���롢��ǰ����

    Long2Buf((INT8U*)&nTemBuf, nDI, 4);
  if(!IsAllBcd(&nBuf1[0], sizeof(nBuf1)))		/*BCD����Ч���ж�*/
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

//���ڽ�������ѹ��ʾֵ��С30%
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


//���ڽ�������ѹ��ʾֵ��С50%
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
// ���������������� ���� ģ�����
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
         if((g_MeterHardwareWrong[0]&0x02)==0x02)   //����
         {   
              GpioClearOutput(IO_PORT7, IO_PINx5);
              memset(Tmp1,0,sizeof(Tmp1));
              BULCDWrite1(0, Tmp1, 35); 
              Num1=200;
          }  
         else if((g_MeterHardwareWrong[0]&0x04)||(g_MeterHardwareWrong[0]&0x01))   //���� ������ ����
         {    
              GpioSetOutput(IO_PORT7, IO_PINx5);       
              GpioClearOutput(IO_PORT2, IO_PINx1);
              memset(Tmp1,0,sizeof(Tmp1));
              BULCDWrite1(0, Tmp1, 35); 
              Num1=200;
         } 
         else if((g_MeterHardwareWrong[0]&0x08)==0x08)   //����
         {  
             if(Num2==0) //�Գ�ʼ��������иı�
             {    
                  if(Num1>199)
                  {
                    Num1=1;
                  }
                  Num2=1;  //���ı�һ��
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
         if((g_MeterHardwareWrong[0]&0x02)==0x02)   //����
         {   
              GpioClearOutput(IO_PORT7, IO_PINx5);
              memset(Tmp2,0,sizeof(Tmp2));
              BULCDWrite1(0, Tmp2, 23); 
              Num1=200;
         }  
         else if((g_MeterHardwareWrong[0]&0x04)||(g_MeterHardwareWrong[0]&0x01))   //����
         {    
              GpioSetOutput(IO_PORT7, IO_PINx5);       
              GpioClearOutput(IO_PORT2, IO_PINx1);
              memset(Tmp2,0,sizeof(Tmp2));
              BULCDWrite1(0, Tmp2, 23); 
              Num1=200;
         } 
         else if((g_MeterHardwareWrong[0]&0x08)==0x08)   //����
         {  
             if(Num2==0) //�Գ�ʼ��������иı�
             {    
                  if(Num1>199)
                  {
                    Num1=100;
                  }
                  Num2=1;  //���ı�һ��
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
     if((g_MeterInsideWrong[0]&0x08))   //��������
     {
        while((g_MeterInsideWrong[0]&0x08))
        {
        }
     }
     *nStepCnt    =Num1;
} 
//������¼���ʾ
void TriphaseEventDisp(void)
{
   static INT8U  nFlg_Event ;
     static INT8U  nFlg ;
    static  INT8U    Temp = 0;            
     
    Temp = (nFlg_Event++) % 2;
    
    if(1==(g_MeterEventWrong[0]&0x01))   //Aʧѹ��˸  ģ��
    {
       g_nFlgUa=1;
    }

    if(1==(g_MeterEventWrong[1]&0x01))  //Bʧѹ��˸
    {
      g_nFlgUb=1; 
    }

    if(1==(g_MeterEventWrong[2]&0x01)) //Cʧѹ��˸
    {
      g_nFlgUc=1;
    }  
    
    if (0x04==(g_MeterEventWrong[0]&0x04)) //Aʧ����˸ ģ��
    {
      g_nFlgIa=1;
    }  

    if (0x04==(g_MeterEventWrong[1]&0x04))  //Bʧ����˸
    {
      g_nFlgIb=1; 
    }

    if (0x04==(g_MeterEventWrong[2]&0x04)) //Cʧ����˸
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
 if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType))//����������������������
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
if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_220ZT == g_nHardType))//����������������������
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
         Data1[7] &= 0xfe;  //�澯��־
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


//��������������ʾ
void sAcCombEnergyData(INT8U*  nBuf, INT8U* Data)
{
    INT8U       i = 0;  
    INT8U       j = 0;     
   INT8U       nBuf1[8] ; 
   if((g_MeterInsideWrong[0]&0x04)==0x04) //������ʾ����ģ��
   {
       memset(&nBuf, 0, 4); 
   }

  if(!IsAllBcd(&nBuf[0], sizeof(nBuf)))		/*BCD����Ч���ж�*/
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


//������¼���ʾ
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




