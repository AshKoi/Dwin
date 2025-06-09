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
#ifndef _GB3761_FILE_TRANS_H_
#define _GB3761_FILE_TRANS_H_

#include "GB3761.h"

/**************************************************************************************************
                                          相关函数声明
***************************************************************************************************/
//文件传输
INT8S GB3761FileTransProc(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset);
//文件传输
INT16U GB3761FileTransFnProc(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen);

/*************************************文件下载具体Fn项*************************************************/
//国网远程下载F1处理过程
INT16U GB3761FileTransGWF1(INT16U nPn, INT8U *pSrcData, INT16U nSrcDataLen, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen);
#endif