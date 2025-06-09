/******************************************************************************************************
* Copyright (c) 2014
* 文件名:
* 创建人: 王一凡
* 日　期: 2014/01/18
* 修改人:
* 日　期:
* 描　述:
* 版　本: Version 1.0
*****************************************************************************************************/
#ifndef ASSIT_PROT_H
#define ASSIT_PROT_H
#include "Os_cpu.h"


//地址
enum _tag_ASSIST_ADDR
{
	ASSIST_RS485 = 1,
	ASSIST_CARRIER,
	//ASSIST_CARRMEASURE,
	ASSIST_SIGNAL,
	ASSIST_HANDSHIP,
	
	ASSIST_END
};


//类型
enum _tag_TERM_TYPE
{
	TERM_I_JZQ = 1,
	TERM_II_JZQ,
	TERM_I_CJQ,
	TERM_II_CJQ,
	TERM_METER,
	
	TERM_END
};

//协议
enum _tag_PROT_TYPE
{
	PROT_BASE = 1,
	PROT_376_1,
	PROT_645,
	PROT_DIRECT_RD,
	PROT_FORWARD,
	
    PROT_END
};

//主辅机交互协议
typedef struct _tagASSIST_PROT
{
	INT8U					nHead;
	INT8U					nAddr[2];
	INT8U					nType[2];
	INT8U					nProt[2];
	INT8U					nCtrl;
	INT8U					nPn[2];
	INT8U					nDi[2];
	INT8U					nDatalen[2];
	INT8U					nData[1];
}ASSIST_PROT, *pASSIST_PROT;


//内部协议报文头长度
#define ASSIST_PROT_HEAD_LEN 			((INT32S)(sizeof(ASSIST_PROT) - 1))			//报头长度:data之前的长度
#define ASSIST_PROT_HEAD_TAIL_LEN 	(ASSIST_PROT_HEAD_LEN + 2)					//报头报尾长度
#define ASSIST_PROT_BASE_LEN 			(ASSIST_PROT_HEAD_LEN + 2)					//报头基本长度




/**************************************************************************************************
                                           内联函数宏
***************************************************************************************************/
//主辅机协议变量参数定义
#define ASSIST_PROT_VAL_DEFINE(ctrl, di)							\
	INT8U			nCtrl			= ctrl;							\
	INT16U			nDi				= di;							\
	INT32S			nLen			= 0;							\
	INT8U			nBuf[300]		= {0};							\
	INT8U*			pData			= &nBuf[0];


//主辅机协议帧返回
#define ASSIST_PROT_DATA_BACK(nAssistAddr, nProt)				\
	nLen = AssistMakeFrm(pData, nLen, sizeof(nBuf), nAssistAddr,	\
							0, nProt, nCtrl, 0, nDi);				\
	nLen = AssistProc(pData, nLen, sizeof(nBuf));					\
    if (CANCEL_VALUE == nLen){                                      \
        return CANCEL_VALUE;                                        \
    }                                                               \
	if (nLen < ASSIST_PROT_BASE_LEN){								\
		return -1;													\
	}																\
	pData += ASSIST_PROT_HEAD_LEN;


//主辅机协议设置参数的错误信息字返回
#define ASSIST_PROT_ERR_INFO_BACK(nAssistAddr, nProt)  {		\
	ASSIST_PROT_DATA_BACK(nAssistAddr, PROT_BASE)				\
	if (0 != *pData){												\
		*pData = 1;													\
	}																\
	return *pData;													\
}




/**************************************************************************************************
                                             声明
***************************************************************************************************/
INT32S AssistMakeFrm(INT8U* pData, INT32S nDatalen, INT32S nMaxDatalen, INT16U nAssistAddr, INT16U nTermType, INT16U nProt, INT8U nCtrl, INT16U nPn, INT16U nDi);

INT32S AssistProtCheckPackage(INT8U* pData, INT32S nDatalen);
INT32S AssistProc(INT8U* pData, INT32S nDatalen, INT32S nMaxDatalen);

INT32S AssistProtAsynCheckPackage(void* pPara);
INT32S AssistProtAsynProc(void* pPara);


#endif
