/******************************************************************************************************
* 2013-2014 ���Ƽ����޹�˾
* �ļ���: 
* ������: ��һ��
* �ա���: 2013/09/21
* �޸���: 
* �ա���: 
* �衡��: 
* �桡��: V1.0
*****************************************************************************************************/
#ifndef _LOGIN_H_
#define _LOGIN_H_

#include "Os_cpu.h"

extern INT8U       g_nIoBuf[3];    //IO��״̬
extern INT8U       g_nIoSetFlg;
extern INT8U       g_nPlusCnt;
extern INT8U       g_nPlusValue;
extern INT8U       g_nKeyValue;
extern INT8U       g_nKeyTestFlg;

/**************************************************************************************************
                                           ��غ�������
***************************************************************************************************/
INT32S Gw3761LoginProc(INT16U nPn, INT8U nFn, INT8U* pRcv, INT32S nRcvlen, INT8U* pSnd, INT32S nSndlen);


void SetIoStatus(void);
void ClrIoStatus(void);
void RdOutIoStatus(void);
void RdInIoStatus(void);
INT8U PlusProc(void);
void KeyTestProc(INT8U nKeyVal);


#endif
