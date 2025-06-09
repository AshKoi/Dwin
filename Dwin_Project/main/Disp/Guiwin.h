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
#ifndef GUIWIN_H
#define GUIWIN_H
#include "Os_cpu.h"

/**************************************************************************************************
                                             相关宏定义及结构枚举定义
***************************************************************************************************/
#define  EDIT_SELECT_NULL				1             									//不允许任何操作
#define  EDIT_MENU 					2												//编辑型的标志
#define  SELECT_MENU					3												//选择型的标志

#define  EDIT_NUM              		0                      							//数字
#define  EDIT_CHAR             		1												//小写字母
#define  EDIT_CHAR_CAP					2												//大写字母

#define  ONCE_ROW_CHAR_NUM			20												//一行字符数


#ifndef NULL
#define NULL   							0
#endif /* NULL */

#ifndef TEST_BIT
#define  TEST_BIT(a, b)			    ((a) & (1 << (b)))								//测试位([0, 31])
#endif

#ifndef SET_BIT
#define  SET_BIT(a, b)			    	((a) |= (1 << (b)))								//置位([0, 31])
#endif

#ifndef CLR_BIT
#define  CLR_BIT(a, b)			    	((a) &= (~(1 << (b)))) 							//清位([0, 31])
#endif

#ifndef GET_MIN
#define  GET_MIN(a, b)			    	(((a) <= (b)) ? (a) : (b)) 						//获取最小值
#endif

#ifndef GET_MAX
#define	 GET_MAX(a, b)			    	(((a) >= (b)) ? (a) : (b)) 						//获取最大值
#endif

#ifndef STRLEN
#define  STRLEN(a) 						(GET_MIN(strlen((char*)(a)), ONCE_ROW_CHAR_NUM))
#endif




//菜单类型    定义界面菜单时使用
enum _tag_MENU_ITEM_TYPE
{
	MENU_ITEM_NULL = 0,
	MENU_ITEM_SHOW,
	MENU_ITEM_EXE,
	MENU_ITEM_DISCRIB
};

//界面
typedef struct _tag_GUI_WIN
{
	INT32S			nWinLevel;		
    void*			pMenuItem;
	INT32S  		nItemNum;							
    INT32S			nItemSelect;       				
}GUI_WIN, *pGUI_WIN;


//菜单
typedef struct _tag_MENU_ITEM 
{
	INT32S			nMenuOrFunc;						// 1,func;2,page
	INT8S*			pItemName;
	void*			pDown;
	void*			pArg;
}MENU_ITEM, *pMENU_ITEM;


//设置型参数菜单的结构
typedef struct _tag_PARAMSET
{
	INT8S*			pString;
	INT8S  			nMin;								
	INT8S  			nMax;								
	INT8U  			nEditLen;
    INT8U           nXingDispFlg;
	INT8S  			nSelectOrEdit;						//选择型或编辑型的标志
	INT8S*			pResult;							
	INT8S**			ppParamStr;	
	INT8S			nEditBuf[ONCE_ROW_CHAR_NUM + 1];
}PARAMSET, *pPARAMSET;


//动态获得显示菜单执行函数
typedef INT32S (*pDISP_FUNC_EXT)(INT32S nIdx, INT32S nDispRowTotal, INT8U nKey, INT8U nSheetRows);

typedef INT32S (*pMENU_FUNC)(void* pVoid);

/**************************************************************************************************
                                           函数
***************************************************************************************************/
/********************************************************************************
* 函数名:GuiwinInit
* 输　入:
*			
* 输　出:
* 返  回:
* 功  能:初始化
*********************************************************************************/
void GuiwinInit(void);
/*
*****************************************************************************
* 函数名:GuiwinLcdClearExt
* 输　入:
*			INT16U		 Color
* 输　出:
* 返  回:
* 功  能:清屏(除了标题栏和底栏)
*****************************************************************************
*/
void GuiwinLcdClearExt(INT16U Color);
/*
*****************************************************************************
* 函数名: GuiwinSheetsget
* 输　入: 
* 输  出: 
* 返  回: 
*       
*       
* 功  能: 获取当前屏编辑行数
*****************************************************************************
*/
INT8U GuiwinSheetsget(void);
/*
*****************************************************************************
* 函数名: GuiwinCurrLevalSet
* 输　入: 
* 输  出: 
* 返  回: 
*       
*       
* 功  能: 获取当前界面的级数
*****************************************************************************
*/
INT16U GuiwinCurrLevalSet(INT16U nLeval);
/*
*****************************************************************************
* 函数名: GuiwinCurrLevalGet
* 输　入: 
* 输  出: 
* 返  回: 
*       
*       
* 功  能: 获取当前界面的级数
*****************************************************************************
*/
INT16U GuiwinCurrLevalGet(void);
/*
*****************************************************************************
* 函数名: GuiwinGetNext
* 输　入: 
*			pGUI_WIN pCurr
* 输  出: 
* 返  回: 
*		
*		
* 功  能: 获取当前的下一个界面
*****************************************************************************
*/
pGUI_WIN GuiwinGetNext(pGUI_WIN pCurr);
/*
*****************************************************************************
* 函数名: GuiwinGetPre
* 输　入: 
*			pGUI_WIN pCurr
* 输  出: 
* 返  回: 
*		
*		
* 功  能: 获取当前的下一个界面
*****************************************************************************
*/
pGUI_WIN GuiwinGetPre(pGUI_WIN pCurr);
/*
*****************************************************************************
* 函数名: GuiwinRootCreat
* 输　入: 
*			void* pMainMenu 			根菜单
*			INT8U nLeval				菜单界面级数
* 输  出: 
* 返  回: 根菜单界面
*       
*       
* 功  能: 根界面菜单条目内容的创建  显示进程启动后需要先调用此函数创建根菜单
*****************************************************************************
*/
pGUI_WIN GuiwinRootCreat(void* pMainMenu, INT8U nLeval);
/*
*****************************************************************************
* 函数名: GuiwinInsert
* 输　入: 
*			pGUI_WIN pFather					父界面
*			pGUI_WIN pInsert					子界面
* 输  出: 
* 返  回: 
*       
*       
* 功  能: 新的界面菜单条目内容的创建    新界面的内容根据 Guimenu.c文件的菜单定义表填写
*****************************************************************************
*/
INT32S GuiwinInsert(pGUI_WIN pFather, pGUI_WIN pInsert);
/*
*****************************************************************************
* 函数名: GuiwinSelectMenuList
* 输　入: 
*			pGUI_WIN 	pCurr
*			INT8U* 		pKey			若是数字键，则会转换为确认键
* 输  出: 
* 返  回: 
*       
*       
* 功  能: 通用滚动主菜单函数,返回获得的菜单ID号
*****************************************************************************
*/
INT32S GuiwinSelectMenuList(pGUI_WIN pCurr, INT8U* pKey);
/*
*****************************************************************************
* 函数名: GuiwinFillCharToStr
* 输　入: 
*			INT8S* 	pString						默认字符串	
*			INT32S 	nNum						需要处理的长度
*			INT8S 	nChar						补充的字符
*			INT8S	nDir						方向:0为前补字符,1为后补字符
* 输  出: 
* 返  回: 
*       
*       
* 功  能: 
*****************************************************************************
*/
void GuiwinFillCharToStr(INT8S* pString, INT32S nNum, INT8S nChar, INT8S nDir);
/*
*****************************************************************************
* 函数名: GuiwinSetParamMenu
* 输　入: 
*			INT8S* 		pTitle
*			pPARAMSET 	pParamCfg
* 输  出: 
* 返  回: 
*       
*       
* 功  能: 设置型参数菜单的显示和动态的修改
*****************************************************************************
*/
INT32S GuiwinSetParamMenu(INT8S* pTitle, pPARAMSET pParamCfg);
/*
*****************************************************************************
* 函数名: GuiwinSetNullCfgParam
* 输　入: 
*			pPARAMSET*  ppCfgParam		    当前行菜单   执行完后指向下一个
*			INT8S* 	    pTitle				此行标题
* 输  出: 
* 返  回: 
*       
*       
* 功  能: 空型菜单赋值
*****************************************************************************
*/
INT32S GuiwinSetNullCfgParam(pPARAMSET* ppCfgParam, char* pTitle);
/*
*****************************************************************************
* 函数名: GuiwinSetSelectCfgParam
* 输　入: 
*			pPARAMSET* 		ppCfgParam			当前行菜单   执行完后指向下一个
*			INT8S* 			pTitle				此行标题
*			INT8S* 			pResult				默认选择第几个
*			const INT8S** 	ppParamStr			选择项列表
* 输  出: 
* 返  回: 0成功; 其他失败
* 功  能: 选择型菜单赋值
*****************************************************************************
*/
INT32S GuiwinSetSelectCfgParam(pPARAMSET* ppCfgParam, char* pTitle, INT8S* pResult, const INT8S** ppParamStr);
/*
*****************************************************************************
* 函数名: GuiwinSetEditCfgParam
* 输　入: 
*			pPARAMSET* 		ppCfgParam			当前行菜单   执行完后指向下一个
*			INT8S* 			pTitle				此行标题
*			INT8S* 			pEditDefault		默认编辑值
*			INT8U 			nEditLen			编辑长度:大于100时显示星号
* 输  出: 
* 返  回: 0成功; 其他失败
* 功  能: 编辑型菜单赋值
*****************************************************************************
*/
INT32S GuiwinSetEditCfgParam(pPARAMSET* ppCfgParam, char* pTitle, INT8S* pEditDefault, INT8U nEditLen);
/*
*****************************************************************************
* 函数名: GuiwinTextOut
* 输　入: 
*			INT16U 		nX			坐标
*			INT16U 		nY			
*			INT8U 		nMode		打印反显模式
*			const char* pFormat
* 输  出: 
* 返  回: 
*		
*		
* 功  能: 显示
*****************************************************************************
*/
INT32S GuiwinTextOut(INT16U nX, INT16U nY, INT8U nMode, const INT8S* pFormat, ... );
/*
*****************************************************************************
* 函数名: GuiwinTextOutAuto(const INT8S* pFormat, ... )
* 输　入: 
*			const char* pFormat
* 输  出: 
* 返  回: 
*		
*		
* 功  能: 显示
*****************************************************************************
*/
INT32S GuiwinTextOutAuto(const INT8S* pFormat, ... );
/*
*****************************************************************************
* 函数名: GuiwinAutoDispProc
* 输　入: 
*			INT8U* 	pTitle		标题
*			INT8U* 	pNote		提示
*			INT32S	nIdxBase	开始显示行
*			INT8U*	pKey		按键值 			为1时只显示不等待按键处理
* 输  出: 
* 返  回: 
*		
*		
* 功  能: 自动显示
*****************************************************************************
*/
INT32S GuiwinAutoDispProc(INT8U* pTitle, INT8U* pNote, INT32S	nIdxBase, INT8U* pKey);
/*
*****************************************************************************
* 函数名: GuiwinDispMenuRowCreate
* 输　入: 
*			INT32S				nIdx				第几行  从0开始
*			INT8U				nFlg				1时此行不做为反显选择
* 输  出: 
* 返  回: 
*		
*		
* 功  能: 创建临时显示行
*****************************************************************************
*/
INT32S GuiwinDispMenuRowCreate(INT32S nIdx, INT8U nFlg, const INT8S* pFormat, ... );
/*
*****************************************************************************
* 函数名: GuiwinDispMenuProc
* 输　入: 
*			INT8U* 	pTitle		标题
*			INT32S	nIdx		
*			INT8U*	pKey		空时只显示,不等待按键
*			void* 	pFunc		函数指针
* 输  出: 
* 返  回: 
*		
*		
* 功  能: 
*****************************************************************************
*/
INT32S GuiwinDispMenuProc(INT8U* pTitle, INT32S nIdx, INT8U* pKey, void* pFunc);

/*
*****************************************************************************
* 函数名: Guiwin1MboxCreate
* 输　入:
*			INT8U*				pKeySting			1个按钮描述
*			INT8U* 				pTitle				标题
* 输  出: 
* 返  回: enter按键
*		
*		
* 功  能: 创建临时框:包含一屏保存和恢复函数
*****************************************************************************
*/
INT32S Guiwin1MboxCreate(INT8U* pKeySting, INT8U* pTitle, ...);
/*
*****************************************************************************
* 函数名: Guiwin2MboxCreate
* 输　入:
*			INT8U*				pKeySting1			第1个按钮描述
*			INT8U*				pKeySting2			第2个按钮描述
*			INT8U* 				pTitle				标题
* 输  出: 
* 返  回: 0，第一个选择;1，第二个选择
*		
*		
* 功  能: 创建临时框:包含一屏保存和恢复函数
*****************************************************************************
*/
INT32S Guiwin2MboxCreate(INT8U* pKeySting1, INT8U* pKeySting2, INT8U* pTitle, ...);
/*
*****************************************************************************
* 函数名: GuiwinDynMboxCreate
* 输　入:
*			INT8U				nCnt				
*			INT8U* 				pTitle				标题
* 输  出: 
* 返  回: 
*		
*		
* 功  能: 创建动态临时框:nCnt不为0时,pTitle可以输入NULL
*****************************************************************************
*/
INT32S GuiwinDynMboxCreate(INT8U nCnt, INT8U* pTitle, ...);

/*
*****************************************************************************
* 函数名: GuiwinDynMboxCreate

*		
*		
* 功  能: 三相表口线初始化
*****************************************************************************
*/

void GpioCfgJCY(void);


void LCDCommPwmOut( INT8U bEnable );

#endif

