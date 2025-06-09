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
#ifndef _LOGIN_H_
#define _LOGIN_H_

#include "Os_cpu.h"

extern INT8U       g_nIoBuf[3];    //IO口状态
extern INT8U       g_nIoSetFlg;
extern INT8U       g_nPlusCnt;
extern INT8U       g_nPlusValue;
extern INT8U       g_nKeyValue;
extern INT8U       g_nKeyTestFlg;

/**************************************************************************************************
                                           相关函数声明
***************************************************************************************************/
INT32S Gw3761LoginProc(INT16U nPn, INT8U nFn, INT8U* pRcv, INT32S nRcvlen, INT8U* pSnd, INT32S nSndlen);


void SetIoStatus(void);
void ClrIoStatus(void);
void RdOutIoStatus(void);
void RdInIoStatus(void);
INT8U PlusProc(void);
void KeyTestProc(INT8U nKeyVal);


#endif
