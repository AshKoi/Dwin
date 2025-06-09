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
#include "PulseCol.h"
#include "AppCfg.h"
#include "SysRes.h"
#include "MemDrvExt.h"
#include "SoftTimer.h"
#include "Base.h"
#include "MemTbl.h"
#include "AppCfg.h"
#include "Fun.h"
#include "SecTask.h"
#include "SoftTimer.h"

/**************************************************************************************************
										  ��غ���ʵ��
***************************************************************************************************/
INT8U   g_nCurrFree                 =  FEE_EVEN;
INT64U  g_nTimes[MAX_PULSE_NUM]     = {0};
INT64U  g_nRateTimes[MAX_FEE_NUM][MAX_PULSE_NUM]    = {0};

/********************************************************************************
* ������: PulseColNumProc
* �䡡��:
* �䡡��:
* ��  ��: 
* ��  ��: 
*********************************************************************************/
INT32S PulseColNumProc(void)
{
    INT8S   nRet        = 0;
    INT8U   nTmpVal     = 0;
    INT8U   i           = 0;
    INT8U   nFlg        = 0;            //��λ��ʾλ��
  
    nFlg = GetMcData();                 //�ж��Ƿ�����������
#if 1    
    if (nFlg)
    {
        for (i = 0; i < MAX_PULSE_NUM; i++)
        {
            if (TestBufBit(&nFlg, 1, i))
            {
                //������
                //��
                g_pMem->sXFramData.nPulseNum[i] += 1;
                
                if (g_nCurrFree >= MAX_FEE_NUM)
                {
                    g_nCurrFree = FEE_EVEN;
                }
                
                //��Ӧ����
                g_pMem->sXFramData.nPerRateNum[g_nCurrFree].nPulseNo[i] += 1;
            }
        }
    }
#endif
    
    return 0;
}

/********************************************************************************
* ������: PulseColInit
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ����ɼ������ʼ�����ϵ��ʼ������������м����
*********************************************************************************/
BOOLEAN PulseColInit(void)
{
#if 1
    INT8U       nCurrFee            = FEE_EVEN;
    WATT_PARAM  sWattPrm[MAX_FEE_PERD_NUM];

    memset(&sWattPrm[0], 0, sizeof(sWattPrm));
    g_nCurrFree = FEE_EVEN;
    MemReadOtherPrm(F21, &sWattPrm[0], sizeof(sWattPrm));
    if (RET_ERR_OK == PulseGetCurrFee(&sWattPrm[0], &nCurrFee))
    {
        g_nCurrFree = nCurrFee;
    }
#endif
    return TRUE;
}

/********************************************************************************
* ������: PulseCalcProc
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ������㴦������������β��·���幦�ʡ��������
*********************************************************************************/
void PulseCalcProc(void)
{
    INT8U       nCurrFee    = FEE_EVEN;
    INT8U       nErr        = 0;
    WATT_PARAM  sWattPrm[MAX_FEE_PERD_NUM];
    
    memset(&sWattPrm[0], 0, sizeof(sWattPrm));
    MemReadOtherPrm(F21, &sWattPrm[0], sizeof(sWattPrm));
    if (RET_ERR_OK == PulseGetCurrFee(&sWattPrm[0], &nCurrFee))
    {
        g_nCurrFree = nCurrFee;
    }
    OSSemPend(g_pSem[SEM_STORE_BUF_IDX], 0, &nErr);
    PulsePulseCalcProc();
    OSSemPost(g_pSem[SEM_STORE_BUF_IDX]);
    PulsePowerCalcProc(&g_pMem->sXFramData.nPerRateNum[0], MAX_FEE_NUM);
}

/********************************************************************************
* ������: PulseCalcProc
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ������㴦���������ӵ���
*********************************************************************************/
INT32S PulsePulseCalcProc(void)
{ 
    INT16U              i                   = 0;
    INT8U               j                   = 0;
    INT8U               k                   = 0;
    INT64U              nCaclPulseNum       = 0;
    INT64U              nPulseNum           = 0;
    INT64U              nCaclTimes          = 0;
    INT64U              nTimes              = 0;
    INT8U               nCurrFee            = FEE_EVEN;
    INT16U              nPulseConst         = 0;
    INT16U              nCt                 = 0;
    INT16U              nPt                 = 0;
    INT16U              nMp                 = 0;
    FP32                fCurrWattVal        = 0;
    FP32                fTmpVal             = 0;
    MEM_PULSE_PRM       sPulsePrm;
    MEM_MT_BASE_PRM     sMtBasePrm;
    pMEM_MP_REAL_DATA   pRealData           = (pMEM_MP_REAL_DATA)(&g_nMemDataBuf[0]);
    
    memset(&sPulsePrm, 0, sizeof(sPulsePrm));
    memset(&sMtBasePrm, 0, sizeof(sMtBasePrm));
    memset(pRealData, 0, sizeof(pRealData));

    
    for (i = 0; i < MAX_METER_NUM; i++)
    {
        memset(&sPulsePrm, 0, sizeof(sPulsePrm));
        if (GetPulsePrm(i + 1, &sPulsePrm) < 0)
        {
            continue;
        }
        j++;
        k = j - 1;
        MemoryRead(STR_OFF(pMEM_FRAM_STORE, sXFramData.nPulseNum[k]), (INT8U*)&nPulseNum, sizeof(nPulseNum), MEM_FRAM);
        nCaclPulseNum = g_pMem->sXFramData.nPulseNum[k] - nPulseNum;
        nCaclTimes = OSTimeGet() - g_nTimes[k];
        g_nTimes[k] = OSTimeGet();
        nPulseConst = Buf2INT16U(&sPulsePrm.nPulseConst[0]);
        if ((0 != nPulseConst) && (0 != nCaclPulseNum))
        {
            if (nCaclTimes <= 0)
            {
                return -1;
            }
            if (nCaclPulseNum > 1)
            {
                // 5msΪ��λ���Ŵ�200��
                fCurrWattVal = (FP32)3600 * 200/nCaclTimes * (nCaclPulseNum - 1);
            }
            else
            {
                fCurrWattVal = (FP32)3600 * 200/nCaclTimes * (nCaclPulseNum);
            }
            //���β�ֵ
            fCurrWattVal = fCurrWattVal / nPulseConst;      //����
#if 0
            //��ʾ��·���幦��ʱҪ��Ϊһ�β�ֵ
            MemReadMeterPrm(nMp, F25, &sMtBasePrm, sizeof(sMtBasePrm));
            nCt = Buf2INT16U(&sMtBasePrm.nCt[0]);
            nPt = Buf2INT16U(&sMtBasePrm.nPt[0]);
            
            if ((0 != nCt) && (0 != nPt))
            {
                fTmpVal = fCurrWattVal * (FP32)nPt;
                fTmpVal *= (FP32)nCt;               //��·������һ�β๦��
            }
#endif
        }
        //�����·���幦��
        RealWattProc(sPulsePrm.nPulseAttr, fCurrWattVal, pRealData);
        //������������
        RealFactorProc(pRealData);
        //�洢ʵʱ����
        MemWriteRealData(nMp, FEND, pRealData, sizeof(pRealData));
        //������ˢ������
        MemoryWrite(STR_OFF(pMEM_FRAM_STORE, sXFramData.nPulseNum[k]), (INT8U*)&g_pMem->sXFramData.nPulseNum[k], sizeof(nPulseNum), MEM_FRAM);
    }
    
    return 0; 
}


/********************************************************************************
* ������: PulsePowerCalcProc
* �䡡��: 
* �䡡��:
* ��  ��:
* ��  ��: ��������
*********************************************************************************/
void PulsePowerCalcProc(const pPULSE_RATE_NUM pPerDayPulRate, INT8U nMaxFeeNum)
{
    INT8U   kk = 0;
    INT8U   jj = 0;
    INT8U   nErr = 0;
    PULSE_RATE_NUM sPulseSrc[1 + MAX_FEE_NUM];
    
    if (NULL == pPerDayPulRate || nMaxFeeNum < MAX_FEE_NUM)
    {
        return;
    }
    memset(&sPulseSrc[0], 0, sizeof(sPulseSrc));
    for (kk = 0; kk < MAX_PULSE_NUM; kk++)
    {
        for (jj = 0; jj < MAX_FEE_NUM; jj++)
        {
            sPulseSrc[0].nPulseNo[kk] += pPerDayPulRate[jj].nPulseNo[kk];
            sPulseSrc[jj + 1].nPulseNo[kk] = pPerDayPulRate[jj].nPulseNo[kk];
        }
    }
    for (kk = 0; kk < (1 + MAX_FEE_NUM); kk++)
    {
        OSSemPend(g_pSem[SEM_STORE_BUF_IDX], 0, &nErr);
        PulseCalDiveRtPower(&sPulseSrc[0], kk);
        OSSemPost(g_pSem[SEM_STORE_BUF_IDX]);
    }
}

/********************************************************************************
* ������: PulseCalDiveRtPower
* �䡡��: 
*         
* �䡡��:
* ��  ��:
* ��  ��: 
*********************************************************************************/
INT8S PulseCalDiveRtPower(const pPULSE_RATE_NUM pPulseNum, INT8U nFeeIdx)
{
    INT8U               i   = 0;
    INT8U               j   = 0;
    INT8U               k   = 0;
    INT16U              nPulseConst = 0;
    INT64U              nCaclPulseNum       = 0;
    INT64U              nPulseNum           = 0;
    INT64U              nCaclTimes          = 0;
    INT64U              nTimes              = 0;
    INT64U              nDayPulseNum    = 0;
    INT64U              nMonPulseNum    = 0;
    FP32                fTransVal = 0;
    FP32                fCurrEnergy     = 0;
    FP32                fDayPower       = 0;
    FP32                fMonPower       = 0;
    FORMAT13            sTmpFt13;
    FORMAT11            sTmpFt11;
    STD_TIME            sTime;
    MEM_PULSE_PRM       sPulsePrm;
	pMEM_MP_REAL_DATA   pRealData   = (pMEM_MP_REAL_DATA)(&g_nMemDataBuf[0]);
    
    if ((NULL == pPulseNum) || ((1 + MAX_FEE_NUM) < nFeeIdx))
    {
        return RET_ERR_ERROR;
    }
    
    memset(&sTime, 0, sizeof(sTime));
    GetSysClk(&sTime);
    
    for (i = 0; i < MAX_METER_NUM; i++)
    {
        memset(&sPulsePrm, 0, sizeof(sPulsePrm));
        memset(pRealData, 0, sizeof(pRealData));
        if (GetPulsePrm(i + 1, &sPulsePrm) < 0)
        {
            continue;
        }
        if (RET_ERR_OK != MemReadRealData(i + 1, FEND, pRealData, sizeof(pRealData)))
        {
            continue;
        }
        j++;
        k = j - 1;
        MemoryRead(STR_OFF(pMEM_FRAM_STORE, sXFramData.nPerRateNum[nFeeIdx].nPulseNo[k]), (INT8U*)&nPulseNum, sizeof(nPulseNum), MEM_FRAM);
        nCaclPulseNum = g_pMem->sXFramData.nPerRateNum[nFeeIdx].nPulseNo[k] - nPulseNum;
        nCaclTimes = OSTimeGet() - g_nRateTimes[nFeeIdx][k];
        g_nRateTimes[nFeeIdx][k] = OSTimeGet();
        
        nPulseConst = Buf2INT16U(&sPulsePrm.nPulseConst[0]);
        if ((0 != nPulseConst) && (0 != nCaclPulseNum))
        {
            fTransVal = (FP32)nCaclPulseNum/(FP32)nPulseConst;
            
            //1.��ǰ����ʾֵ
            fCurrEnergy = (FP32)((FP32)g_pMem->sXFramData.nPerRateNum[nFeeIdx].nPulseNo[k] / (FP32)nPulseConst);
            
            //2.���յ���
            MemoryRead(STR_OFF(pMEM_FRAM_STORE, sXFramData.nDayRateNum[nFeeIdx].nPulseNo[k]), (INT8U*)&nDayPulseNum, sizeof(nDayPulseNum), MEM_FRAM);
            fDayPower = (FP32)(g_pMem->sXFramData.nPerRateNum[nFeeIdx].nPulseNo[k] - nDayPulseNum) / (FP32)(nPulseConst);
            
            //3.���µ���
            MemoryRead(STR_OFF(pMEM_FRAM_STORE, sXFramData.nMonRateNum[nFeeIdx].nPulseNo[k]), (INT8U*)&nMonPulseNum, sizeof(nMonPulseNum), MEM_FRAM);
            fMonPower = (FP32)(g_pMem->sXFramData.nPerRateNum[nFeeIdx].nPulseNo[k] - nMonPulseNum) / (FP32)(nPulseConst);
            
            Float2Format11(fCurrEnergy, &sTmpFt11);
            PulseCurrEnergy(sPulsePrm.nPulseAttr, nFeeIdx, &sTmpFt11, pRealData);
            Float2Format13(fDayPower, &sTmpFt13);
            PulseDayPower(sPulsePrm.nPulseAttr, nFeeIdx, &sTmpFt13, pRealData);
            Float2Format13(fMonPower, &sTmpFt13);
            PulseMonPower(sPulsePrm.nPulseAttr, nFeeIdx, &sTmpFt13, pRealData);
            
            MemWriteRealData(i + 1, FEND, pRealData, sizeof(pRealData));
        }
        //������ˢ������
        MemoryWrite(STR_OFF(pMEM_FRAM_STORE, sXFramData.nPerRateNum[nFeeIdx].nPulseNo[k]), (INT8U*)&g_pMem->sXFramData.nPerRateNum[nFeeIdx].nPulseNo[k], sizeof(nPulseNum), MEM_FRAM);
    
    }
    
    return 0;
}

/********************************************************************************
* ������: RealWattProc
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: 
*********************************************************************************/
INT32S RealWattProc(INT8U nAttr, FP32 fSrc, pMEM_MP_REAL_DATA pPulseRtData)
{
    FORMAT09    sTmpFt09Val;
    STD_TIME    sTime;

    if (NULL == pPulseRtData)
    {
        return RET_ERR_ERROR;
    }
    memset(&sTmpFt09Val, 0, sizeof(sTmpFt09Val));
    if (0 != fSrc)
    {
        Float2Format09(fSrc, &sTmpFt09Val);
    }
    if ((PUL_ATTR_PP == (nAttr & 0x03)) || (PUL_ATTR_NP == (nAttr & 0x03)))
    {
        memcpy(&pPulseRtData->nPWatt[0], &sTmpFt09Val, sizeof(FORMAT09));
    }
    else
    {
        memcpy(&pPulseRtData->nNWatt[0], &sTmpFt09Val, sizeof(FORMAT09));
    }
    
    //���ڹ��ʼ���
    FunCalcSinglePhaseSWatt((pFORMAT09)&pPulseRtData->nPWatt[0], (pFORMAT09)&pPulseRtData->nNWatt[0], (pFORMAT09)&pPulseRtData->nSWatt[0]);
    GetSysClk(&sTime);
    if (!IsNewDay(&sTime))
    {
        memcpy(&pPulseRtData->nReadTime, &sTime.nSec, 6);
    }
    return RET_ERR_OK;
}

/********************************************************************************
* ������: RealWattProc
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: 
*********************************************************************************/
INT32S RealFactorProc(pMEM_MP_REAL_DATA pPulseRtData)
{
    FP32 fPWatt = 0;
    FP32 fSWatt = 1;
    FP32 fFactor = 0;

    if (NULL == pPulseRtData)
    {
        return RET_ERR_ERROR;
    }
    if (RET_VLD != CheckVldFormat09((pFORMAT09)&pPulseRtData->nSWatt[0]))
    {
        return RET_ERR_ERROR;
    }
    if (RET_INVLD == CheckVldFormat09((pFORMAT09)&pPulseRtData->nPWatt[0]))
    {
        return RET_ERR_ERROR;
    }
    fSWatt = Format092Float((pFORMAT09)&pPulseRtData->nSWatt[0]);
    fPWatt = Format092Float((pFORMAT09)&pPulseRtData->nPWatt[0]);
    if (0 != fSWatt)
    {
        fFactor = (fPWatt / fSWatt) * 100;
        Float2Format05(fFactor, (pFORMAT05)&pPulseRtData->nFactor[0]);
    }
    return RET_ERR_OK;
}



/********************************************************************************
* ������: GetMcData
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ��ȡ��������
*********************************************************************************/
INT32S GetMcData(void)
{
    INT8U               nStatus     = 0;
    static      INT8U   nCnt        = 0;
    //����һ·����Ϊ��
    nStatus = ReadCpuIoStatusEX(0x40); 
    
    if (nStatus == 0)
    {
        nCnt++;
        if ((nCnt >= 5) && (nCnt <= 10))
        {
            nStatus = ReadCpuIoStatusEX(0x40); 
            if (nStatus == 1)
            {
                nCnt = 0;
                return 1;
            }
        }
        if (nCnt > 10)
        {
            nCnt = 0;
        }
    }
    else
    {
        nCnt = 0;
    }
    
    return 0;
}

/********************************************************************************
* ������: PulseGetCurrFee
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ����F21��ȡ��ǰʱ�䴦���ĸ�����ʱ��
*********************************************************************************/
INT32S PulseGetCurrFee(const pWATT_PARAM pWattPrm, INT8U* pCurrFee)
{
    INT8U       ii = 0;
    STD_TIME    sTimes;

    if ((NULL == pCurrFee) || (NULL == pWattPrm))
    {
        return RET_ERR_ERROR;
    }
    memset(&sTimes, 0, sizeof(sTimes));
    if(RET_ERR_ERROR == GetSysClk(&sTimes))
    {
        *pCurrFee = FEE_EVEN;
        return RET_ERR_ERROR;
    }
    for (ii = 0; ii < MAX_FEE_PERD_NUM; ii++)
    {
        if ((pWattPrm[ii].nStrHour == 0) && (pWattPrm[ii].nStrMin == 0)
         && (pWattPrm[ii].nEndHour == 0) && (pWattPrm[ii].nEndMin == 0))
        {
            *pCurrFee = FEE_EVEN;
            break;
        }
        if ((0xFF == pWattPrm[ii].nStrHour) || (0xFF == pWattPrm[ii].nStrMin))
        {
            *pCurrFee = FEE_EVEN;
            break;
        }
        if (((sTimes.nHour > pWattPrm[ii].nStrHour) || ((sTimes.nHour == pWattPrm[ii].nStrHour) && (sTimes.nMin >= pWattPrm[ii].nStrMin)))
         && ((sTimes.nHour < pWattPrm[ii].nEndHour) || ((sTimes.nHour == pWattPrm[ii].nEndHour) && (sTimes.nMin < pWattPrm[ii].nEndMin))))
        {
            *pCurrFee = pWattPrm[ii].nKind % MAX_FEE_NUM;
            break;
        }
    }
    return RET_ERR_OK;
}


/********************************************************************************
* ������: PulseCurrEnergy
* �䡡��: 
* �䡡��:
* ��  ��:
* ��  ��: 
*********************************************************************************/
INT32S PulseCurrEnergy(INT8U nAttr, INT8U nFeeIdx, const pFORMAT11 pSrcFt11, pMEM_MP_REAL_DATA pPulseData)
{
    if ((NULL == pSrcFt11) || (NULL == pPulseData) || (nFeeIdx > MAX_FEE_NUM))
    {
        return RET_ERR_ERROR;
    }
    if (PUL_ATTR_PP == (nAttr & 0x03))
    {
        memcpy(&pPulseData->nEnergyPP[nFeeIdx], pSrcFt11, sizeof(FORMAT11));
    }
    else if (PUL_ATTR_PN == (nAttr & 0x03))
    {
        memcpy(&pPulseData->nEnergyPN[nFeeIdx], pSrcFt11, sizeof(FORMAT11));
    }
    else if (PUL_ATTR_NP == (nAttr & 0x03))
    {
        memcpy(&pPulseData->nEnergyNP[nFeeIdx], pSrcFt11, sizeof(FORMAT11));
    }
    else
    {
        memcpy(&pPulseData->nEnergyNN[nFeeIdx], pSrcFt11, sizeof(FORMAT11));
    }
    return RET_ERR_OK;
}


/********************************************************************************
* ������: PulseDayPower
* �䡡��: 
* �䡡��:
* ��  ��:
* ��  ��: 
*********************************************************************************/
INT32S PulseDayPower(INT8U nAttr, INT8U nFeeIdx, const pFORMAT13 pSrcFt13, pMEM_MP_REAL_DATA pPulseData)
{
    if ((NULL == pSrcFt13) || (NULL == pPulseData) || (nFeeIdx > MAX_FEE_NUM))
    {
        return RET_ERR_ERROR;
    }
    if (PUL_ATTR_PP == (nAttr & 0x03))
    {
        memcpy(&pPulseData->sDayPowerStat.sPPPower[nFeeIdx], pSrcFt13, sizeof(FORMAT13));
    }
    else if (PUL_ATTR_PN == (nAttr & 0x03))
    {
        memcpy(&pPulseData->sDayPowerStat.sPNPower[nFeeIdx], pSrcFt13, sizeof(FORMAT13));
    }
    else if (PUL_ATTR_NP == (nAttr & 0x03))
    {
        memcpy(&pPulseData->sDayPowerStat.sNPPower[nFeeIdx], pSrcFt13, sizeof(FORMAT13));
    }
    else
    {
        memcpy(&pPulseData->sDayPowerStat.sNNPower[nFeeIdx], pSrcFt13, sizeof(FORMAT13));
    }
    return RET_ERR_OK;
}

/********************************************************************************
* ������: PulseMonPower
* �䡡��: 
* �䡡��:
* ��  ��:
* ��  ��: 
*********************************************************************************/
INT32S PulseMonPower(INT8U nAttr, INT8U nFeeIdx, const pFORMAT13 pSrcFt13, pMEM_MP_REAL_DATA pPulseData)
{
    if ((NULL == pSrcFt13) || (NULL == pPulseData) || (nFeeIdx > MAX_FEE_NUM))
    {
        return RET_ERR_ERROR;
    }
    if (PUL_ATTR_PP == (nAttr & 0x03))
    {
        memcpy(&pPulseData->sMonPowerStat.sPPPower[nFeeIdx], pSrcFt13, sizeof(FORMAT13));
    }
    else if (PUL_ATTR_PN == (nAttr & 0x03))
    {
        memcpy(&pPulseData->sMonPowerStat.sPNPower[nFeeIdx], pSrcFt13, sizeof(FORMAT13));
    }
    else if (PUL_ATTR_NP == (nAttr & 0x03))
    {
        memcpy(&pPulseData->sMonPowerStat.sNPPower[nFeeIdx], pSrcFt13, sizeof(FORMAT13));
    }
    else
    {
        memcpy(&pPulseData->sMonPowerStat.sNNPower[nFeeIdx], pSrcFt13, sizeof(FORMAT13));
    }
    return RET_ERR_OK;
}














