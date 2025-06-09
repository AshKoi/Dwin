/**************************************************************************************************
* Copyright (c) 2010-2011 光一科技股份有限公司研发中心
* 文件名:
* 创建人: 研发中心
* 日　期: 2010/08/18
* 修改人:
* 日　期:
* 描　述:
* 版　本: Version 1.0
***************************************************************************************************/
#ifndef _GB3761_GET_CLASSII_H_
#define _GB3761_GET_CLASSII_H_

#include "Gw3761.h"
#include "Table.h"

/**************************************************************************************************
                                          相关函数声明
***************************************************************************************************/
INT8S GB3761GetClassII(pAPP_COMM pComm);
INT8S GB3761GetClassIIProc(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset);
INT16U GB3761GetClassIIFnProc(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen);
INT16U GB3761GetClassIIF161(INT16U nPn, const pFORMAT20 pTddTime, INT8U *pData, INT16U nMaxDataLen);
INT16U GB3761GetClassIIF177(INT16U nPn, const pFORMAT21 pTdmTime, INT8U *pData, INT16U nMaxDataLen);



#endif