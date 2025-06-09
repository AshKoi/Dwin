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
#ifndef _GB3761_SET_PRM_EXT_H_
#define _GB3761_SET_PRM_EXT_H_
#include "Os_cpu.h"
#include "Config.h"

#include "Gw3761.h"

extern INT8U   g_nSignalErrFlg;
extern INT8U   g_nCarrErrFlg;
extern INT8U   g_nRs485ErrFlg;
extern INT8U   g_nRemotCtrlFlg;
extern INT8U   g_nGprsLedFlg;
/**************************************************************************************************
                                          相关函数说明
***************************************************************************************************/
INT8S GB3761SetParamExt(pAPP_COMM pComm);
INT8S GB3761SetParamProcExt(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset);
INT16U GB3761SetParamFnProcExt(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen);


//F1GPRS电源灯
BOOLEAN GB3761SetParamExtF1(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F2 GPRS通讯灯不闪烁
BOOLEAN GB3761SetParamExtF2(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F3 GPRS模块信号强度
BOOLEAN GB3761SetParamExtF3(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F4 终端通信异常
BOOLEAN GB3761SetParamExtF4(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F5 SIM卡异常
BOOLEAN GB3761SetParamExtF5(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F9 载波模块不工作
BOOLEAN GB3761SetParamExtF9(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F10 载波模块表计参数丢失
BOOLEAN GB3761SetParamExtF10(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F17 终端死机
BOOLEAN GB3761SetParamExtF17(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F18 终端电源故障
BOOLEAN GB3761SetParamExtF18(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F25形成冻结数据
BOOLEAN GB3761SetParamExtF25(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F26 形成表计冻结数据(形成不准确数据)
BOOLEAN GB3761SetParamExtF26(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F27检查冻结数据状态
BOOLEAN GB3761SetParamExtF27(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F33电台故障
BOOLEAN GB3761SetParamExtF33(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F34主板与接口板交互故障
BOOLEAN GB3761SetParamExtF34(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F35主板与接口板交互故障
BOOLEAN GB3761SetParamExtF35(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F36 脉冲口故障 
BOOLEAN GB3761SetParamExtF36(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F37 485端口故障
BOOLEAN GB3761SetParamExtF37(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F38 遥控端口故障
BOOLEAN GB3761SetParamExtF38(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F39 遥信端口故障
BOOLEAN GB3761SetParamExtF39(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F81 被抄口规约
BOOLEAN GB3761SetParamExtF81(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F84 生产编号
BOOLEAN GB3761SetParamExtF84(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F86 交采数据清零
BOOLEAN GB3761SetParamExtF86(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F87 所有初始化
BOOLEAN GB3761SetParamExtF87(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
BOOLEAN GB3761SetParamExtF89(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);












#endif