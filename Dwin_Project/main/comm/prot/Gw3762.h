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
#ifndef GW3762_H
#define GW3762_H

#include "Os_cpu.h"

/**************************************************************************************************
                                           宏及枚举定义
***************************************************************************************************/



/**************************************************************************************************
                                           结构定义
***************************************************************************************************/

//通讯帧结构
typedef struct _tagGW3762_PROT_SUB
{
	INT8U				nInfo[6];
	INT8U               nUseData[1];
}GW3762_PROT_SUB, *pGW3762_PROT_SUB;

//通讯帧结构
typedef struct _tagGW3762_PROT
{
	INT8U				nHead;
	INT8U               nFrmLen[2];
    INT8U               nCtrl;
	INT8U               nAppData[1];
}GW3762_PROT, *pGW3762_PROT;


//3762报文头长度
#define GW3762_PROT_HEAD_LEN 			(sizeof(GW3762_PROT) - 1)					//报头长度:nAppData之前的长度
#define GW3762_PROT_APP_R_LEN 		6
#define GW3762_PROT_APP_ADDR_LEN 	12
#define GW3762_PROT_APP_HEAD_LEN 	(GW3762_PROT_APP_R_LEN + GW3762_PROT_APP_ADDR_LEN + 1 + 2)// r + addr + afn + fn
#define GW3762_PROT_HEAD2_LEN 		(GW3762_PROT_HEAD_LEN + GW3762_PROT_APP_HEAD_LEN)//报头长度
#define GW3762_PROT_HEAD_TAIL_LEN 	(GW3762_PROT_HEAD2_LEN + 2)					//报头报尾长度
#define GW3762_PROT_BASE_LEN 			(GW3762_PROT_HEAD2_LEN + 2)					//报头基本长度








/**************************************************************************************************
                                           调用接口
***************************************************************************************************/
INT32S Gb3762ProtMakeFrm(INT8U* pAddr, INT8U* pData, INT32S nDatalen, INT32S nMaxDatalen, INT8U nDir, INT8U nPrm, INT8U nModl, INT8U nModlSel, INT8U nPhase, INT8U nAfn, INT16U nFn);
INT32S Gw3762Proc(INT8U* pData, INT32S nDatalen, INT32S nMaxDatalen);
#endif
