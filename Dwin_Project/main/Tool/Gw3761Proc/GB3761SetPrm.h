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
#ifndef _GB3761_SET_PRM_H_
#define _GB3761_SET_PRM_H_

#include "Gw3761.h"

/**************************************************************************************************
                                          相关函数声明
***************************************************************************************************/
//参数设置
INT8S GB3761SetParamProc(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset);
//参数设置
INT16U GB3761SetParamFnProc(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen);

/*************************************参数设置具体Fn项*************************************************/
//F1 终端通信参数设置
BOOLEAN GB3761SetParamF1(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F3 主站IP地址和端口
BOOLEAN GB3761SetParamF3(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F4 主站电话号码和短信中心号码
BOOLEAN GB3761SetParamF4(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F7 终端抄表日设置
BOOLEAN GB3761SetParamF7(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F8 终端上行通信工作方式
BOOLEAN GB3761SetParamF8(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F10 //终端电能表/交流采样装置配置参数
BOOLEAN GB3761SetParamF10(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset, INT8U *pReSult);
//F11 脉冲参数
BOOLEAN GB3761SetParamF11(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F12 终端状态量输入参数
BOOLEAN GB3761SetParamF12(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F16 虚拟专网 用户名、密码
BOOLEAN GB3761SetParamF16(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F24
BOOLEAN GB3761SetParamF24(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F25
BOOLEAN GB3761SetParamF25(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F33 终端抄表运行参数设置
BOOLEAN GB3761SetParamF33(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F85
BOOLEAN GB3761SetParamF85(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
BOOLEAN GB3761SetParamF94(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F130
BOOLEAN GB3761SetParamF130(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F131
BOOLEAN GB3761SetParamF131(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F132
BOOLEAN GB3761SetParamF132(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F225
BOOLEAN GB3761SetParamF225(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F227
BOOLEAN GB3761SetParamF227(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
#endif
