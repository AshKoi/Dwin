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
#ifndef GUIWIN_H
#define GUIWIN_H
#include "Os_cpu.h"

/**************************************************************************************************
                                             ��غ궨�弰�ṹö�ٶ���
***************************************************************************************************/
#define  EDIT_SELECT_NULL				1             									//�������κβ���
#define  EDIT_MENU 					2												//�༭�͵ı�־
#define  SELECT_MENU					3												//ѡ���͵ı�־

#define  EDIT_NUM              		0                      							//����
#define  EDIT_CHAR             		1												//Сд��ĸ
#define  EDIT_CHAR_CAP					2												//��д��ĸ

#define  ONCE_ROW_CHAR_NUM			20												//һ���ַ���


#ifndef NULL
#define NULL   							0
#endif /* NULL */

#ifndef TEST_BIT
#define  TEST_BIT(a, b)			    ((a) & (1 << (b)))								//����λ([0, 31])
#endif

#ifndef SET_BIT
#define  SET_BIT(a, b)			    	((a) |= (1 << (b)))								//��λ([0, 31])
#endif

#ifndef CLR_BIT
#define  CLR_BIT(a, b)			    	((a) &= (~(1 << (b)))) 							//��λ([0, 31])
#endif

#ifndef GET_MIN
#define  GET_MIN(a, b)			    	(((a) <= (b)) ? (a) : (b)) 						//��ȡ��Сֵ
#endif

#ifndef GET_MAX
#define	 GET_MAX(a, b)			    	(((a) >= (b)) ? (a) : (b)) 						//��ȡ���ֵ
#endif

#ifndef STRLEN
#define  STRLEN(a) 						(GET_MIN(strlen((char*)(a)), ONCE_ROW_CHAR_NUM))
#endif




//�˵�����    �������˵�ʱʹ��
enum _tag_MENU_ITEM_TYPE
{
	MENU_ITEM_NULL = 0,
	MENU_ITEM_SHOW,
	MENU_ITEM_EXE,
	MENU_ITEM_DISCRIB
};

//����
typedef struct _tag_GUI_WIN
{
	INT32S			nWinLevel;		
    void*			pMenuItem;
	INT32S  		nItemNum;							
    INT32S			nItemSelect;       				
}GUI_WIN, *pGUI_WIN;


//�˵�
typedef struct _tag_MENU_ITEM 
{
	INT32S			nMenuOrFunc;						// 1,func;2,page
	INT8S*			pItemName;
	void*			pDown;
	void*			pArg;
}MENU_ITEM, *pMENU_ITEM;


//�����Ͳ����˵��Ľṹ
typedef struct _tag_PARAMSET
{
	INT8S*			pString;
	INT8S  			nMin;								
	INT8S  			nMax;								
	INT8U  			nEditLen;
    INT8U           nXingDispFlg;
	INT8S  			nSelectOrEdit;						//ѡ���ͻ�༭�͵ı�־
	INT8S*			pResult;							
	INT8S**			ppParamStr;	
	INT8S			nEditBuf[ONCE_ROW_CHAR_NUM + 1];
}PARAMSET, *pPARAMSET;


//��̬�����ʾ�˵�ִ�к���
typedef INT32S (*pDISP_FUNC_EXT)(INT32S nIdx, INT32S nDispRowTotal, INT8U nKey, INT8U nSheetRows);

typedef INT32S (*pMENU_FUNC)(void* pVoid);

/**************************************************************************************************
                                           ����
***************************************************************************************************/
/********************************************************************************
* ������:GuiwinInit
* �䡡��:
*			
* �䡡��:
* ��  ��:
* ��  ��:��ʼ��
*********************************************************************************/
void GuiwinInit(void);
/*
*****************************************************************************
* ������:GuiwinLcdClearExt
* �䡡��:
*			INT16U		 Color
* �䡡��:
* ��  ��:
* ��  ��:����(���˱������͵���)
*****************************************************************************
*/
void GuiwinLcdClearExt(INT16U Color);
/*
*****************************************************************************
* ������: GuiwinSheetsget
* �䡡��: 
* ��  ��: 
* ��  ��: 
*       
*       
* ��  ��: ��ȡ��ǰ���༭����
*****************************************************************************
*/
INT8U GuiwinSheetsget(void);
/*
*****************************************************************************
* ������: GuiwinCurrLevalSet
* �䡡��: 
* ��  ��: 
* ��  ��: 
*       
*       
* ��  ��: ��ȡ��ǰ����ļ���
*****************************************************************************
*/
INT16U GuiwinCurrLevalSet(INT16U nLeval);
/*
*****************************************************************************
* ������: GuiwinCurrLevalGet
* �䡡��: 
* ��  ��: 
* ��  ��: 
*       
*       
* ��  ��: ��ȡ��ǰ����ļ���
*****************************************************************************
*/
INT16U GuiwinCurrLevalGet(void);
/*
*****************************************************************************
* ������: GuiwinGetNext
* �䡡��: 
*			pGUI_WIN pCurr
* ��  ��: 
* ��  ��: 
*		
*		
* ��  ��: ��ȡ��ǰ����һ������
*****************************************************************************
*/
pGUI_WIN GuiwinGetNext(pGUI_WIN pCurr);
/*
*****************************************************************************
* ������: GuiwinGetPre
* �䡡��: 
*			pGUI_WIN pCurr
* ��  ��: 
* ��  ��: 
*		
*		
* ��  ��: ��ȡ��ǰ����һ������
*****************************************************************************
*/
pGUI_WIN GuiwinGetPre(pGUI_WIN pCurr);
/*
*****************************************************************************
* ������: GuiwinRootCreat
* �䡡��: 
*			void* pMainMenu 			���˵�
*			INT8U nLeval				�˵����漶��
* ��  ��: 
* ��  ��: ���˵�����
*       
*       
* ��  ��: ������˵���Ŀ���ݵĴ���  ��ʾ������������Ҫ�ȵ��ô˺����������˵�
*****************************************************************************
*/
pGUI_WIN GuiwinRootCreat(void* pMainMenu, INT8U nLeval);
/*
*****************************************************************************
* ������: GuiwinInsert
* �䡡��: 
*			pGUI_WIN pFather					������
*			pGUI_WIN pInsert					�ӽ���
* ��  ��: 
* ��  ��: 
*       
*       
* ��  ��: �µĽ���˵���Ŀ���ݵĴ���    �½�������ݸ��� Guimenu.c�ļ��Ĳ˵��������д
*****************************************************************************
*/
INT32S GuiwinInsert(pGUI_WIN pFather, pGUI_WIN pInsert);
/*
*****************************************************************************
* ������: GuiwinSelectMenuList
* �䡡��: 
*			pGUI_WIN 	pCurr
*			INT8U* 		pKey			�������ּ������ת��Ϊȷ�ϼ�
* ��  ��: 
* ��  ��: 
*       
*       
* ��  ��: ͨ�ù������˵�����,���ػ�õĲ˵�ID��
*****************************************************************************
*/
INT32S GuiwinSelectMenuList(pGUI_WIN pCurr, INT8U* pKey);
/*
*****************************************************************************
* ������: GuiwinFillCharToStr
* �䡡��: 
*			INT8S* 	pString						Ĭ���ַ���	
*			INT32S 	nNum						��Ҫ����ĳ���
*			INT8S 	nChar						������ַ�
*			INT8S	nDir						����:0Ϊǰ���ַ�,1Ϊ���ַ�
* ��  ��: 
* ��  ��: 
*       
*       
* ��  ��: 
*****************************************************************************
*/
void GuiwinFillCharToStr(INT8S* pString, INT32S nNum, INT8S nChar, INT8S nDir);
/*
*****************************************************************************
* ������: GuiwinSetParamMenu
* �䡡��: 
*			INT8S* 		pTitle
*			pPARAMSET 	pParamCfg
* ��  ��: 
* ��  ��: 
*       
*       
* ��  ��: �����Ͳ����˵�����ʾ�Ͷ�̬���޸�
*****************************************************************************
*/
INT32S GuiwinSetParamMenu(INT8S* pTitle, pPARAMSET pParamCfg);
/*
*****************************************************************************
* ������: GuiwinSetNullCfgParam
* �䡡��: 
*			pPARAMSET*  ppCfgParam		    ��ǰ�в˵�   ִ�����ָ����һ��
*			INT8S* 	    pTitle				���б���
* ��  ��: 
* ��  ��: 
*       
*       
* ��  ��: ���Ͳ˵���ֵ
*****************************************************************************
*/
INT32S GuiwinSetNullCfgParam(pPARAMSET* ppCfgParam, char* pTitle);
/*
*****************************************************************************
* ������: GuiwinSetSelectCfgParam
* �䡡��: 
*			pPARAMSET* 		ppCfgParam			��ǰ�в˵�   ִ�����ָ����һ��
*			INT8S* 			pTitle				���б���
*			INT8S* 			pResult				Ĭ��ѡ��ڼ���
*			const INT8S** 	ppParamStr			ѡ�����б�
* ��  ��: 
* ��  ��: 0�ɹ�; ����ʧ��
* ��  ��: ѡ���Ͳ˵���ֵ
*****************************************************************************
*/
INT32S GuiwinSetSelectCfgParam(pPARAMSET* ppCfgParam, char* pTitle, INT8S* pResult, const INT8S** ppParamStr);
/*
*****************************************************************************
* ������: GuiwinSetEditCfgParam
* �䡡��: 
*			pPARAMSET* 		ppCfgParam			��ǰ�в˵�   ִ�����ָ����һ��
*			INT8S* 			pTitle				���б���
*			INT8S* 			pEditDefault		Ĭ�ϱ༭ֵ
*			INT8U 			nEditLen			�༭����:����100ʱ��ʾ�Ǻ�
* ��  ��: 
* ��  ��: 0�ɹ�; ����ʧ��
* ��  ��: �༭�Ͳ˵���ֵ
*****************************************************************************
*/
INT32S GuiwinSetEditCfgParam(pPARAMSET* ppCfgParam, char* pTitle, INT8S* pEditDefault, INT8U nEditLen);
/*
*****************************************************************************
* ������: GuiwinTextOut
* �䡡��: 
*			INT16U 		nX			����
*			INT16U 		nY			
*			INT8U 		nMode		��ӡ����ģʽ
*			const char* pFormat
* ��  ��: 
* ��  ��: 
*		
*		
* ��  ��: ��ʾ
*****************************************************************************
*/
INT32S GuiwinTextOut(INT16U nX, INT16U nY, INT8U nMode, const INT8S* pFormat, ... );
/*
*****************************************************************************
* ������: GuiwinTextOutAuto(const INT8S* pFormat, ... )
* �䡡��: 
*			const char* pFormat
* ��  ��: 
* ��  ��: 
*		
*		
* ��  ��: ��ʾ
*****************************************************************************
*/
INT32S GuiwinTextOutAuto(const INT8S* pFormat, ... );
/*
*****************************************************************************
* ������: GuiwinAutoDispProc
* �䡡��: 
*			INT8U* 	pTitle		����
*			INT8U* 	pNote		��ʾ
*			INT32S	nIdxBase	��ʼ��ʾ��
*			INT8U*	pKey		����ֵ 			Ϊ1ʱֻ��ʾ���ȴ���������
* ��  ��: 
* ��  ��: 
*		
*		
* ��  ��: �Զ���ʾ
*****************************************************************************
*/
INT32S GuiwinAutoDispProc(INT8U* pTitle, INT8U* pNote, INT32S	nIdxBase, INT8U* pKey);
/*
*****************************************************************************
* ������: GuiwinDispMenuRowCreate
* �䡡��: 
*			INT32S				nIdx				�ڼ���  ��0��ʼ
*			INT8U				nFlg				1ʱ���в���Ϊ����ѡ��
* ��  ��: 
* ��  ��: 
*		
*		
* ��  ��: ������ʱ��ʾ��
*****************************************************************************
*/
INT32S GuiwinDispMenuRowCreate(INT32S nIdx, INT8U nFlg, const INT8S* pFormat, ... );
/*
*****************************************************************************
* ������: GuiwinDispMenuProc
* �䡡��: 
*			INT8U* 	pTitle		����
*			INT32S	nIdx		
*			INT8U*	pKey		��ʱֻ��ʾ,���ȴ�����
*			void* 	pFunc		����ָ��
* ��  ��: 
* ��  ��: 
*		
*		
* ��  ��: 
*****************************************************************************
*/
INT32S GuiwinDispMenuProc(INT8U* pTitle, INT32S nIdx, INT8U* pKey, void* pFunc);

/*
*****************************************************************************
* ������: Guiwin1MboxCreate
* �䡡��:
*			INT8U*				pKeySting			1����ť����
*			INT8U* 				pTitle				����
* ��  ��: 
* ��  ��: enter����
*		
*		
* ��  ��: ������ʱ��:����һ������ͻָ�����
*****************************************************************************
*/
INT32S Guiwin1MboxCreate(INT8U* pKeySting, INT8U* pTitle, ...);
/*
*****************************************************************************
* ������: Guiwin2MboxCreate
* �䡡��:
*			INT8U*				pKeySting1			��1����ť����
*			INT8U*				pKeySting2			��2����ť����
*			INT8U* 				pTitle				����
* ��  ��: 
* ��  ��: 0����һ��ѡ��;1���ڶ���ѡ��
*		
*		
* ��  ��: ������ʱ��:����һ������ͻָ�����
*****************************************************************************
*/
INT32S Guiwin2MboxCreate(INT8U* pKeySting1, INT8U* pKeySting2, INT8U* pTitle, ...);
/*
*****************************************************************************
* ������: GuiwinDynMboxCreate
* �䡡��:
*			INT8U				nCnt				
*			INT8U* 				pTitle				����
* ��  ��: 
* ��  ��: 
*		
*		
* ��  ��: ������̬��ʱ��:nCnt��Ϊ0ʱ,pTitle��������NULL
*****************************************************************************
*/
INT32S GuiwinDynMboxCreate(INT8U nCnt, INT8U* pTitle, ...);

/*
*****************************************************************************
* ������: GuiwinDynMboxCreate

*		
*		
* ��  ��: �������߳�ʼ��
*****************************************************************************
*/

void GpioCfgJCY(void);


void LCDCommPwmOut( INT8U bEnable );

#endif

