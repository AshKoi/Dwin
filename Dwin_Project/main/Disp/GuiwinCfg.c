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
#include "Os_cpu.h"
#include "GuiwinCfg.h"
#include "Lcd.h"
#include "SysRes.h"
#include "Base.h"

#define GUICFG_SELECT_SWITCH		1									//仿真终端

//显示基本相关配置
const INT8U		g_nCharWide		= LCD_CHAR_W;						//字符的宽  
const INT8U		g_nCharHeight		= LCD_CHAR_H;						//字符显示需要的高
const INT16U	g_nWide				= LCD_W;
const INT16U	g_nHeight			= LCD_H;
const INT8U		g_nMaxRowSheets 	= LCD_H / LCD_CHAR_H; 			//一屏显示的最多的行数
#if GUICFG_SELECT_SWITCH == 1
INT16U			g_nBackColour		= 0;								// 白
INT16U			g_nDrawColour		= 1;								// 黑
#else
INT16U			g_nBackColour		= BLUE;								// blue
INT16U			g_nDrawColour		= WHITE;							// white
#endif

// 按键值配置
const INT8U 	c_nKeyNull			= KEY_NULL;
const INT8U 	c_nKeyUp			= KEY_UP;
const INT8U 	c_nKeyDown      	= KEY_DOWN;
const INT8U 	c_nKeyLeft      	= KEY_LEFT;
const INT8U 	c_nKeyRight     	= KEY_RIGHT;
const INT8U 	c_nKeyEnter     	= KEY_ENTER;
const INT8U 	c_nKeyCancel    	= KEY_CANCEL;
const INT8U 	c_nKeyFun1      	= KEY_FUN1;
const INT8U 	c_nKeyFun2      	= KEY_FUN2;
const INT8U 	c_nKeyFun3      	= KEY_FUN3;
const INT8U 	c_nKeyFun4      	= KEY_FUN4;
const INT8U 	c_nKeyNum0      	= KEY_NUM0;
const INT8U 	c_nKeyNum1      	= KEY_NUM1;
const INT8U 	c_nKeyNum2      	= KEY_NUM2;
const INT8U 	c_nKeyNum3      	= KEY_NUM3;
const INT8U 	c_nKeyNum4      	= KEY_NUM4;
const INT8U 	c_nKeyNum5      	= KEY_NUM5;
const INT8U 	c_nKeyNum6      	= KEY_NUM6;
const INT8U 	c_nKeyNum7      	= KEY_NUM7;
const INT8U 	c_nKeyNum8      	= KEY_NUM8;
const INT8U 	c_nKeyNum9      	= KEY_NUM9;
const INT8U 	c_nKeyC         	= KEY_C;
const INT8U 	c_nKeyTab       	= KEY_TAB;
const INT8U 	c_nKeyFn        	= KEY_FN;
const INT8U 	c_nKeyShift     	= KEY_SHIFT;
const INT8U 	c_nKeyAlt       	= KEY_ALT;
const INT8U 	c_nKeySp        	= KEY_SP;
const INT8U 	c_nKeyPoint     	= KEY_POINT;

// 数字键值对应关系
const NUMKEY_TAB g_sKeyTbl[] 	= 
{
	{KEY_NUM0,	'0', 0},
	{KEY_NUM1,	'1', 0},
	{KEY_NUM2,	'2', 3, 'a', 'b', 'c'},
	{KEY_NUM3,	'3', 3, 'd', 'e', 'f'},
	{KEY_NUM4,	'4', 3, 'g', 'h', 'i'},
	{KEY_NUM5,	'5', 3, 'j', 'k', 'l'},
	{KEY_NUM6,	'6', 3, 'm', 'n', 'o'},
	{KEY_NUM7,	'7', 4, 'p', 'q', 'r', 's'},
	{KEY_NUM8,	'8', 3, 't', 'u', 'v'},
	{KEY_NUM9,	'9', 4, 'x', 'x', 'y', 'z'},
	{KEY_POINT, '.', 4, '*', '.', '-', '_'},
    {KEY_SP,    ' ', 4, ' ', '#', '@', ':'},

	{0}
};

// 软键盘
const char g_nVirKey[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', '!', '@', '#', '$', '%', 
    '^', '&', '*', '(', ')', '_', '-', '+', '/', '\\','~', ':', 'a', 'b', 'c', 'd', 
    'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 
    'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
};
const INT8U     g_nVirKeyRowLen 	= 16;								//软键盘一行个数
const INT8U     g_nVirKeyNum 		= sizeof(g_nVirKey);				//软键盘个数




// 编译开关
#if GUICFG_SELECT_SWITCH == 1
const INT8U     g_nEditNumKey 		= 0;                            	//编辑时没有数字键
const INT8U		g_nHaveFrame		= 0;								//有边框 ((1,1)、(3,3)坐标画线);为0时，g_nReservHeight也应置0
const INT8U		g_nReservHeight	= 0;								//预留高度;没有边框时，需调整数值
#else
const INT8U     g_nEditNumKey 		= 1;                            	//编辑时有数字键
const INT8U		g_nHaveFrame		= 1;								//有边框 ((1,1)、(3,3)坐标画线);为0时，g_nReservHeight也应置0
const INT8U		g_nReservHeight	= 6;								//预留高度;没有边框时，需调整数值
#endif
const INT8U		g_nHaveHead		= 1;								//第一行是否空出来
const INT8U		g_nHaveBottom		= 1;								//最后一行是否空出来


static INT8U	g_nProtectFlg		= 2;								//1时默认内部保护；2时信号量保护

/*
显示屏备份使用
*/
#if GUICFG_SELECT_SWITCH == 1
static INT8U	g_nDispStoreBuf[160 * 20]       = {0};	
#else
static INT8U	g_nDispStoreBuf[320 * 30]       = {0};	
#endif
static INT32U	g_nDispStoreBufSize				= sizeof(g_nDispStoreBuf);


#if GUICFG_SELECT_SWITCH == 1
/*
*****************************************************************************
* 函数名: GuiwinRecDataRd
* 输　入: 
* 输  出: 
* 返  回: 
*		
*		
* 功  能: 备份
*****************************************************************************
*/
void GuiwinRecDataRd(INT16U nX1, INT16U nY1, INT16U nX2, INT16U nY2)
{
    LCD_RANG_PEND(0);
    memcpy(g_nDispStoreBuf, g_nScreenBuf, GET_MIN(sizeof(g_nScreenBuf), sizeof(g_nDispStoreBuf)));
    LCD_RANG_POST();
}
/*
*****************************************************************************
* 函数名: GuiwinRecDataRestore
* 输　入: 
* 输  出: 
* 返  回: 
*		
*		
* 功  能: 恢复
*****************************************************************************
*/
void GuiwinRecDataRestore(INT16U nX1, INT16U nY1, INT16U nX2, INT16U nY2)
{
	INT16U 	i				= 0;
	INT16U	k				= 0;
	INT16U	xx				= 0;
	INT8U	nBuf[81]		= {0};

	LCD_RANG_PEND(0);
    memcpy(g_nScreenBuf, g_nDispStoreBuf, GET_MIN(sizeof(g_nScreenBuf), sizeof(g_nDispStoreBuf)));
	for (k = nY1; k <= nY2; k++)
	{
		memset(&nBuf[0], 0, sizeof(nBuf));
		for (i = 0; i < 20 * 8; i++)
		{
			if (TestBufBit(&g_nScreenBuf[k][0], 20, i))
			{
				if (i % 2)
				{
					nBuf[i / 2] |= 0x0F;
				}
				else
				{
					nBuf[i / 2] |= 0xF0;
				}
			}
		}
		xx = 0;
		for (i = 0; i < 27; i++) 
		{
			if ((xx + 6) > nX1)
			{
				UC1698SetRamStart(xx,  k); 
				break;
			}
			xx += 6;
		}
		xx = 0;
	    for (i = 0; i < 27; i++)       
	    {
			if ((xx + 6) > nX1)
			{
				LCD_WR_DATA_SUB(nBuf[i * 3 + 0]);
			    LCD_WR_DATA_SUB(nBuf[i * 3 + 1]);
			    LCD_WR_DATA_SUB(nBuf[i * 3 + 2]);
			}
			xx += 6;
			if (xx >= nX2)
			{
				break;
			}
	    }
	}
    LCD_RANG_POST();
}
#else
/*
*****************************************************************************
* 函数名: GuiwinRecDataRd
* 输　入: 
* 输  出: 
* 返  回: 
*		
*		
* 功  能: 备份
*****************************************************************************
*/
void GuiwinRecDataRd(INT16U nX1, INT16U nY1, INT16U nX2, INT16U nY2)
{
	INT16U	i 		= 0;
	INT16U  j 		= 0;
	INT16U	nData 	= 0;
	INT32U	nIdx	= 0;
    INT16U  nRed    = 0;
    INT16U  nGreen  = 0;
    INT16U  nBlue   = 0;
    INT16U  nLcdStoreBufSize = 0;
    
    LCD_RANG_PEND(0);
    nLcdStoreBufSize = g_nDispStoreBufSize;
	LCD_SET_REC(nX1, nY1, nX2, nY2);
	memset(&g_nDispStoreBuf[0], 0, nLcdStoreBufSize);
    LCD_WR_REG(0x2E);
	LCD_RD_DATA(nData);
	for (i = nY1; i <= nY2; i++)
	{
		for (j = nX1; j <= nX2 / 2; j++)
		{
            LCD_RD_DATA(nRed);
			LCD_RD_DATA(nGreen);
			LCD_RD_DATA(nBlue);
            if (nIdx >= (nLcdStoreBufSize * 8))
            {
                break;   
            }
			nData = (nRed & R_COLOR) | ((nRed << 3) & G_COLOR) | (nGreen >> 11);
			if (nData == g_nDrawColour)
			{
				SetBufBit(&g_nDispStoreBuf[0], nLcdStoreBufSize, nIdx);
			}
            nIdx++;
            if (nIdx >= (nLcdStoreBufSize * 8))
            {
                break;   
            }
            nData = ((nGreen << 8) & R_COLOR) | ((nBlue >> 5) & G_COLOR) | ((nBlue >> 3) & B_COLOR);
			if (nData == g_nDrawColour)
			{
				SetBufBit(&g_nDispStoreBuf[0], nLcdStoreBufSize, nIdx);
			}
			nIdx++;
		}
	}
    LCD_RANG_POST();
}
/*
*****************************************************************************
* 函数名: GuiwinRecDataRestore
* 输　入: 
* 输  出: 
* 返  回: 
*		
*		
* 功  能: 恢复
*****************************************************************************
*/
void GuiwinRecDataRestore(INT16U nX1, INT16U nY1, INT16U nX2, INT16U nY2)
{
	INT16U	i 		= 0;
	INT16U  j 		= 0;
	INT16U	nData 	= 0;
	INT32U	nIdx	= 0;
    INT16U  nLcdStoreBufSize = 0;
    
    LCD_RANG_PEND(0);
    nLcdStoreBufSize = g_nDispStoreBufSize;
	LCD_SET_REC(nX1, nY1, nX2, nY2);
	for (i = nY1; i <= nY2; i++)
	{
		for (j = nX1; j <= nX2; j++)
		{
            if (nIdx >= (nLcdStoreBufSize * 8))
            {
                break;   
            }
			if (TestBufBit(&g_nDispStoreBuf[0], nLcdStoreBufSize, nIdx))
			{
				nData = g_nDrawColour;
			}
			else
			{
				nData = g_nBackColour;
			}
			LCD_WR_DATA(nData);
			nIdx++;
		}
	}
    LCD_RANG_POST();
}
#endif

/*
*****************************************************************************
* 函数名: LCD_RANG_PEND
* 输　入: 
*           INT8U  nVal
* 输  出: 
* 返  回: 
*		
*		
* 功  能: 保护
*****************************************************************************
*/
void LCD_RANG_PEND(INT8U nVal)		
{							
	if (2 == g_nProtectFlg)									
    {													
        INT8U	nErr = 0;								
		OSSemPend(g_pSem[SEM_LCDPROTECT_IDX], 0, &nErr);
    }													
    else												
    {													
        while(g_nProtectFlg == 0)				
        {	
			if (0 != nVal)
			{
				OSTimeDlyHMSM(0, 0, 0, 10);	
			}		
        }												
        g_nProtectFlg = 0;						
    }													
}
/*
*****************************************************************************
* 函数名: LCD_RANG_POST
* 输　入: 
*           INT8U  nVal
* 输  出: 
* 返  回: 
*		
*		
* 功  能: 保护
*****************************************************************************
*/
void LCD_RANG_POST(void)		
{							
	if (2 == g_nProtectFlg)									
    {													
        OSSemPost(g_pSem[SEM_LCDPROTECT_IDX]);			
    }													
    else												
    {													
        g_nProtectFlg = 1;					
    }													
}


