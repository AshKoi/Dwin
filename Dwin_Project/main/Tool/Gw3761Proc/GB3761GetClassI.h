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
#ifndef _GB3761_GET_CLASSI_H_
#define _GB3761_GET_CLASSI_H_

#include "Gw3761.h"

/**************************************************************************************************
                                          相关函数声明
***************************************************************************************************/
INT8S GB3761GetClassI(pAPP_COMM pComm);
INT8S GB3761GetClassIProc(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset);
INT16U GB3761GetClassIFnProc(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen);


//F2 终端日历时钟
INT16U GB3761GetClassIF2(INT16U nPn, INT8U *pData, INT16U nMaxDataLen);
//F25 当前三相及总有/无功功率、功率因素，三相电压、电流、零序电流、视在功率
INT16U GB3761GetClassIF25(INT16U nPn, INT8U *pData, INT16U nMaxDataLen);
//F129  当前正向有功电能示值
INT16U GB3761GetClassIF129(INT16U nPn, INT8U *pData, INT16U nMaxDataLen);




#endif
