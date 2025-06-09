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
#ifndef _GB3761_GET_PRM_H_
#define _GB3761_GET_PRM_H_

#include "Gw3761.h"

/**************************************************************************************************
                                          相关函数声明
***************************************************************************************************/
//参数查询
INT8S GB3761GetParamProc(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset);
//参数查询
INT16U GB3761GetParamFnProc(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen);

/*************************************参数查询具体Fn项*************************************************/

//F7 终端抄表日设置
INT16U GB3761GetParamF7(INT16U nPn, INT8U *pData, INT16U nMaxDataLen);
//F10 终端电能表/交采配置参数
INT16U GB3761GetParamF10(pAPP_COMM pComm, INT16U nPn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen);
INT16U GB3761GetParamF11(pAPP_COMM pComm, INT16U nPn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen);
INT16U GB3761GetParamF25(INT16U nPn, INT8U *pData, INT16U nMaxDataLen);
INT16U GB3761GetParamF33(pAPP_COMM pComm, INT16U nPn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen);
#endif
