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
#ifndef _PULSE_COL_H_
#define _PULSE_COL_H_

#include "Os_cpu.h"
#include "Config.h"
#include "MemData.h"

/**************************************************************************************************
                                             ��������
***************************************************************************************************/
INT32S PulseColNumProc(void);
BOOLEAN PulseColInit(void);
void PulseCalcProc(void);
INT32S PulsePulseCalcProc(void);
void PulsePowerCalcProc(const pPULSE_RATE_NUM pPerDayPulRate, INT8U nMaxFeeNum);
INT8S PulseCalDiveRtPower(const pPULSE_RATE_NUM pPulseNum, INT8U nFeeIdx);
INT32S RealWattProc(INT8U nAttr, FP32 fSrc, pMEM_MP_REAL_DATA pPulseRtData);
INT32S RealFactorProc(pMEM_MP_REAL_DATA pPulseRtData);
INT32S GetMcData(void);
INT32S PulseGetCurrFee(const pWATT_PARAM pWattPrm, INT8U* pCurrFee);
INT32S PulseCurrEnergy(INT8U nAttr, INT8U nFeeIdx, const pFORMAT11 pSrcFt11, pMEM_MP_REAL_DATA pPulseData);
INT32S PulseDayPower(INT8U nAttr, INT8U nFeeIdx, const pFORMAT13 pSrcFt13, pMEM_MP_REAL_DATA pPulseData);
INT32S PulseMonPower(INT8U nAttr, INT8U nFeeIdx, const pFORMAT13 pSrcFt13, pMEM_MP_REAL_DATA pPulseData);




#endif