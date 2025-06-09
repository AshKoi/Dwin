/******************************************************************************************************
* 2013-2014 益邦科技有限公司
* 文件名: 
* 创建人: 王一凡
* 日　期: 2013/09/21
* 修改人: 
* 日　期: 
* 描　述: 
* 版　本: V1.0
*****************************************************************************************************/
#ifndef _GB3761_GET_PRM_EXT_H_
#define _GB3761_GET_PRM_EXT_H_
#include "Os_cpu.h"
#include "Config.h"

#include "Gw3761.h"



/**************************************************************************************************
                                          相关函数说明
***************************************************************************************************/

INT8S GB3761GetParamExt(pAPP_COMM pComm);
INT8S GB3761GetParamProcExt(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset);
INT16U GB3761GetParamFnProcExt(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen);


//F81被抄口规约
INT16U GB3761GetParamExtF81(INT16U nPn, INT8U *pData, INT16U nMaxDataLen);
//F82GPRS在线状态
INT16U GB3761GetParamExtF82(INT16U nPn, INT8U *pData, INT16U nMaxDataLen);
//F83以太网在线状态
INT16U GB3761GetParamExtF83(INT16U nPn, INT8U *pData, INT16U nMaxDataLen);
//F84生成编号
INT16U GB3761GetParamExtF84(INT16U nPn, INT8U *pData, INT16U nMaxDataLen);


#endif