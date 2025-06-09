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
#ifndef _GB3761_CTRL_CMD_H_
#define _GB3761_CTRL_CMD_H_

#include "Gw3761.h"

/**************************************************************************************************
                                          相关函数声明
***************************************************************************************************/
//控制命令
INT8S GB3761CtrlCmdProc(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset);
//控制命令
INT16U GB3761CtrlCmdFnProc(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen);

/*************************************控制命令具体Fn项*************************************************/
//F1  遥控跳闸
BOOLEAN GB3761CtrlCmdF1(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F2  允许合闸
BOOLEAN GB3761CtrlCmdF2(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F25  终端自动保电参数
BOOLEAN GB3761CtrlCmdF25(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F31 对时命令
BOOLEAN GB3761CtrlCmdF31(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F33  终端自动保电解除
BOOLEAN GB3761CtrlCmdF33(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F38  激活终端连接主站
BOOLEAN GB3761CtrlCmdF38(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F39  命令终端断开连接
BOOLEAN GB3761CtrlCmdF39(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F49 命令指定通信端口暂停抄表
BOOLEAN GB3761CtrlCmdF49(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F50 命令指定通信端口恢复抄表
BOOLEAN GB3761CtrlCmdF50(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F51 命令指定通信端口重新抄表
BOOLEAN GB3761CtrlCmdF51(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
#endif