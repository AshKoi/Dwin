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
#ifndef GUIWIN_CFG_H
#define GUIWIN_CFG_H
#include "Os_cpu.h"
#include "Key.h"
#include "Lcd.h"



//具体的键值结构
typedef struct _tag_NUMKEY_TAB
{
	INT8U  	nKeyNum;
	INT8U	nKeyVal;
	INT8U  	nCharNum;
	INT8U  	nKeyChar[4];
}NUMKEY_TAB, *pNUMKEY_TAB;


//显示基本相关配置
extern const INT8U	g_nCharWide;
extern const INT8U	g_nCharHeight;
extern const INT16U g_nWide;
extern const INT16U g_nHeight;
extern const INT8U 	g_nMaxRowSheets;

extern INT16U 		g_nBackColour;
extern INT16U 		g_nDrawColour;



//按键值配置
extern const INT8U 	c_nKeyNull;
extern const INT8U 	c_nKeyUp;
extern const INT8U 	c_nKeyDown;
extern const INT8U 	c_nKeyLeft;
extern const INT8U 	c_nKeyRight;
extern const INT8U 	c_nKeyEnter;
extern const INT8U 	c_nKeyCancel;
extern const INT8U 	c_nKeyFun1;
extern const INT8U 	c_nKeyFun2;
extern const INT8U 	c_nKeyFun3;
extern const INT8U 	c_nKeyFun4;
extern const INT8U 	c_nKeyNum0;
extern const INT8U 	c_nKeyNum1;
extern const INT8U 	c_nKeyNum2;
extern const INT8U 	c_nKeyNum3;
extern const INT8U 	c_nKeyNum4;
extern const INT8U 	c_nKeyNum5;
extern const INT8U 	c_nKeyNum6;
extern const INT8U 	c_nKeyNum7;
extern const INT8U 	c_nKeyNum8;
extern const INT8U 	c_nKeyNum9;
extern const INT8U 	c_nKeyC;
extern const INT8U 	c_nKeyTab;
extern const INT8U 	c_nKeyFn;
extern const INT8U 	c_nKeyShift;
extern const INT8U 	c_nKeyAlt;
extern const INT8U 	c_nKeySp;
extern const INT8U 	c_nKeyPoint;

extern const NUMKEY_TAB g_sKeyTbl[];
extern const char   g_nVirKey[];
extern const INT8U  g_nVirKeyRowLen;
extern const INT8U  g_nVirKeyNum;




// 编译开关
extern const INT8U  g_nEditNumKey;
extern const INT8U	g_nHaveFrame;
extern const INT8U	g_nReservHeight;
extern const INT8U	g_nHaveHead;
extern const INT8U	g_nHaveBottom;






/**************************************************************************************************
                                           函数宏
***************************************************************************************************/
//恢复与保护
void 	GuiwinRecDataRd(INT16U nX1, INT16U nY1, INT16U nX2, INT16U nY2);
void 	GuiwinRecDataRestore(INT16U nX1, INT16U nY1, INT16U nX2, INT16U nY2);
//保护
void 	LCD_RANG_PEND(INT8U nVal);
void 	LCD_RANG_POST(void);




// 颜色配置
#define COLOUR_SET(r, g, b)							(((r & 0x1F) << 11) | ((g & 0x3F) << 5) | (b & 0x1F))

#define LCD_INIT()									LcdInit()
#define LCD_REC_CLEAR(x1, y1, x2, y2, nColor)		LcdRecClear(x1, y1, x2, y2, nColor)
#define LCD_CLEAR(nColor)							LcdClear(nColor)
//#define LCD_SET_CURSOR(x1, y1)						LcdSetCursor(x1, y1)
#define LCD_SET_REC(x1, y1, x2, y2)					Address_set(x1, y1, x2, y2)

#define LCD_DRAW_POINT(x1, y1, nColour)				LCD_DrawPoint2(x1, y1, nColour)
#define LCD_DRAW_LINE(x1, y1, x2, y2, nColour)		LCD_DrawLine2(x1, y1, x2, y2, nColour)
#define LCD_DRAW_REC(x1, y1, x2, y2, nColour)		LCD_DrawRectangle2(x1, y1, x2, y2, nColour)
#define LCD_DRAW_XIELINE(x1, y1, x2, y2, nColour)	LCD_DrawXieLine2(x1, y1, x2, y2, nColour)

#define LCD_SHOW_CHAR(x, y, nChar, nB, nD, nMode)	LCD_ShowChar(x, y, nChar, nB, nD, nMode)
#define LCD_SHOW_HZ_CHAR(x, y, pP, nB, nD, nMode)	LCD_ShowHzString(x, y, pP, nB, nD, nMode)
#endif

