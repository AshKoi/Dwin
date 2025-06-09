/******************************************************************************************************
* Copyright (c) 2010-2011 ��һ�Ƽ��ɷ����޹�˾�з�����
* �ļ���:
* ������: �з�����Ӧ�ÿ�����
* �ա���: 2011/01/24
* �޸���:
* �ա���:
* �衡��:
* �桡��: Version 1.0
*****************************************************************************************************/
#include "GB3761GetClassI.h"
#include "Gw3761.h"
#include "MemDrvExt.h"
#include "Table.h"
#include "Fun.h"
#include "Macro.h"
#include "SysRes.h"
#include "Base.h"
#include "ChkCrc.h"
#include "Reset.h"
#include "MemData.h"
#include "SoftTimer.h"
#include "rmotlink.h"
#include "MemTbl.h"

/**************************************************************************************************
                                           ��غ���ʵ��
***************************************************************************************************/
/********************************************************************************
* ������: GB3761GetClassI
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: һ�����ݲ�ѯ
*********************************************************************************/
INT8S GB3761GetClassI(pAPP_COMM pComm)
{
    INT16U 		nPn[20];
	INT16U 		nFn[GB3761_MAX_FN_NUM];
	INT16U		nPnNum   = 0;
	INT8U		nFnNum   = 0;
	INT16U  	nOffset  = 0;
	pGB3761_FRM_INFO pInfo  = NULL;
	pGB3761_FRM_MAP  pRxMap = NULL;

	if (NULL == pComm)
    {
    	return RET_ERR_POINT;
    }

    pInfo  = (pGB3761_FRM_INFO)pComm->pFrmInfo;   //��ȡ����֡��Ϣ��ָ��
    pRxMap = (pGB3761_FRM_MAP)&pComm->pRxFrmBuf[0];
    while (nOffset + 4 <= pInfo->nRxDataLen)
	{
		memset(&nPn[0], 0xFFFF, sizeof(nPn));
        memset(&nFn[0], 0, sizeof(nFn));
        if (RET_ERR_OK != GB3761DaDt2PnFn(&pComm->pRxFrmBuf[GB3761_DATA_IDX + nOffset], &nPn[0], &nPnNum, &nFn[0], &nFnNum, &pInfo->bAllMpFlag))  //Dadtת��Pnfn
		{
            break;
        }
        if (pInfo->bAllMpFlag)
	    {
            if (RET_ERR_OK != GB3761GetAllMpNo(pRxMap->nAfn, nFn[0], &nPn[0], &nPnNum, MAX_MP))
            {
                break;
            }
        }
        nOffset += 4;
        if (RET_ERR_OK != GB3761GetClassIProc(pComm, &nPn[0], nPnNum, &nFn[0], nFnNum, &nOffset))
        {
            break;
        }
	}
	return RET_ERR_OK;
}

/********************************************************************************
* ������: GB3761GetClassIProc
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: һ�����ݲ�ѯ����
*********************************************************************************/
INT8S GB3761GetClassIProc(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset)
{
	INT16U    	i = 0;
	INT16U    	j = 0;
    INT16U      nDataIdx   = *pOffset;
	INT16U		nDataLen = 0;
	INT8U     	nData[200];        //g_nProtTmpData 1024->200
    INT16U      nMaxDataLen = 0;
	pGB3761_FRM_INFO pInfo = NULL;

    if (NULL == pComm || NULL == pPn || NULL == pFn || NULL == pOffset)
    {
    	return RET_ERR_POINT;
    }

    pInfo = (pGB3761_FRM_INFO)(pComm->pFrmInfo);
    nMaxDataLen = pComm->nFrmLen - GB3761_FRM_RES_LEN;
	for (i = 0; i < nPnNum; i++)
	{
		if (pInfo->bAllMpFlag)
    	{
    		nDataIdx = *pOffset;//�ûس�ʼֵ
    	}
		for (j = 0; j < nFnNum; j++)
		{
            nDataLen = 0;                               //���ݳ������㣬ÿ�ζ�����
            memset(&nData[0], 0, sizeof(nData));        //�����������
			nDataLen += GB3761GetClassIFnProc(pComm, pPn[i], pFn[j], &nDataIdx, &nData[0], sizeof(nData));
            if (nDataLen > nMaxDataLen || 0 == nDataLen)  //һ��Fn�����ݳ��ȳ���֡�� �� �����ݻ�ȡ ����
            {
                continue;
            }
            pInfo->nTxDataLen += MemCopy(&pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen], &nData[0], nDataLen);
		}
	}
	*pOffset = nDataIdx;
	return RET_ERR_OK;
}

/********************************************************************************
* ������: GB3761GetClassIFnProc
* �䡡��: pAppCom    ����֡��Ϣ
          nPn        ��Ϣ��
          nFn        ��Ϣ��
* �䡡��: pOffset    ����Fn���ݴ�����
          pData    ��ѯ��ȡ����ָ��
          nDataLen    ��ѯ��ȥ���ݳ���
* ��  ��:RET_ERR_OK ִ�гɹ�  RET_ERR_ERROR ִ��ʧ���˳� RET_ERR_RANG ���ݵ�Ԫ��ƫ�ƿ���ִ����һ��FN
* ��  ��:��ȡI������3761��׼Fn�������
*********************************************************************************/
INT16U GB3761GetClassIFnProc(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen)
{
	INT8U   nCurveDen = 0;
	INT8U   nFirstNo  = 0;
	INT16U  nDataLen  = 0;
    pGB3761_FRM_INFO pInfo = NULL;
    STD_TIME  sTime;

    if (NULL == pComm || NULL == pOffset || NULL == pData)
    {
        return 0;
    }
    pInfo = (pGB3761_FRM_INFO)pComm->pFrmInfo;

    if (RET_ERR_OK != GB3761PnFn2DADT(nPn, nFn, &pData[nDataLen]))
    {
        return 0;
    }
    nDataLen += 4;
	memset(&sTime, 0, sizeof(STD_TIME));

    switch (nFn)
    {
        case F2:        //F2 �ն�����ʱ��
        {
        	nDataLen += GB3761GetClassIF2(nPn, &pData[nDataLen], nMaxDataLen);
            break;
        }
		case F25:       //��ǰ���༰����/�޹����ʡ��������أ������ѹ��������������������ڹ���
        {
        	nDataLen += GB3761GetClassIF25(nPn, &pData[nDataLen], nMaxDataLen);
            break;
        }
        case F129:         //��ǰ�����й�����ʾֵ
        {
        	nDataLen += GB3761GetClassIF129(nPn, &pData[nDataLen], nMaxDataLen);
            break;
        }
        
        default:
        {
        	pInfo->bNoDataFlag = TRUE;
        	break;
        }
    }
    if (nDataLen > 4)
    {
    	return nDataLen;
    }
    return 0;
}


//F2 �ն�����ʱ��
INT16U GB3761GetClassIF2(INT16U nPn, INT8U *pData, INT16U nMaxDataLen)
{
	INT16U nDataLen = 0;
	STD_TIME  sTime;
    STD_TIME  sTmpTime;

	if (NULL == pData || 0 != nPn)
	{
		return 0;
	}

    memset(&sTime, 0, sizeof(STD_TIME));
    memset(&sTmpTime, 0, sizeof(STD_TIME));
    RTCDateTimeRead(&sTmpTime);
    if (RET_ERR_OK != GetSysClk(&sTime))
    {
        return 0;
    }
    sTime.nWeek = sTmpTime.nWeek;
    pData[nDataLen++] = sTime.nSec;
    pData[nDataLen++] = sTime.nMin;
    pData[nDataLen++] = sTime.nHour;
    pData[nDataLen++] = sTime.nDay;
    pData[nDataLen++] = (sTime.nMon & 0x1F) | ((sTime.nWeek<<5) & 0xE0);
    pData[nDataLen++] = sTime.nYear;
    return nDataLen;
}

//F25 ��ǰ���༰����/�޹����ʡ��������أ������ѹ��������������������ڹ���
INT16U GB3761GetClassIF25(INT16U nPn, INT8U *pData, INT16U nMaxDataLen)
{
    FORMAT11    nEnergy[MAX_FEE_NUM + 1] = {0};
	FORMAT09    nWatt[MAX_PHASE_NUM + 1] = {0};
	FORMAT26    nFactor[MAX_PHASE_NUM + 1] = {0};
	FORMAT26    nCurr[MAX_PHASE_NUM] = {0};
	FORMAT07    nVolt[MAX_PHASE_NUM] = {0};
    METER_INFO	sMeterInfo;
	pMETER_INFO	pMeterInfo	= &sMeterInfo;
    INT8U		nReadTime[6];							//����ʱ��YY-MM-DD-HH-MM-SS
    INT16U      nSeq        = 0;
    INT8U       i           = 0;
    INT32S		nDataLen	= 0;
    
	nSeq = GetSeqFromMp(nPn);
    if (GetMeterInfo(nSeq, pMeterInfo) < 0)
    {
        return 0;
    }
    MemReadRealData(nSeq, F2, &nReadTime[0], sizeof(nReadTime));
    memcpy(&pData[nDataLen], &nReadTime[1], 5);
    nDataLen += 5;
    //�й�����
    if (MemReadRealData(nSeq, F19, &nWatt[0], sizeof(nWatt)) < 0)
    {
        memset(&pData[nDataLen], 0xEE, 4 * MAX_PHASE_NUM);
        nDataLen += 4 * MAX_PHASE_NUM;
    }
    else
    {
        for (i = 0; i < (1 + MAX_PHASE_NUM); i++)
        {
            memcpy(&pData[nDataLen] , &nWatt[i] ,sizeof(FORMAT09));
            nDataLen += sizeof(FORMAT09);
        }
    }
    //�޹�����
    if (MemReadRealData(nSeq, F20, &nWatt[0], sizeof(nWatt)) < 0)
    {
        memset(&pData[nDataLen], 0xEE, 4 * MAX_PHASE_NUM);
        nDataLen += 4 * MAX_PHASE_NUM;
    }
    else
    {
        for (i = 0; i < (1 + MAX_PHASE_NUM); i++)
        {
            memcpy(&pData[nDataLen] , &nWatt[i] ,sizeof(FORMAT09));
            nDataLen += sizeof(FORMAT09);
        }
    }
    //��������
    if (MemReadRealData(nSeq, F22, &nFactor[0], sizeof(nFactor)) < 0)
    {
        memset(&pData[nDataLen], 0xEE, 4 * MAX_ANLOG_NUM);
        nDataLen += 4 * MAX_ANLOG_NUM;
    }
    else
    {
        for (i = 0; i < (2 + MAX_ANLOG_NUM); i++)
        {
            memcpy(&pData[nDataLen] , &nFactor[i] ,sizeof(FORMAT26));
            nDataLen += sizeof(FORMAT26);
        }
    }
    //��ѹ
    if (MemReadRealData(nSeq, F17, &nVolt[0], sizeof(nVolt)) < 0)
    {
        memset(&pData[nDataLen], 0xEE, 3 * MAX_ANLOG_NUM);
        nDataLen += 3 * MAX_ANLOG_NUM;
    }
    else
    {
        for (i = 0; i < (1 + MAX_ANLOG_NUM); i++)
        {
            memcpy(&pData[nDataLen] , &nVolt[i] ,sizeof(FORMAT07));
            nDataLen += sizeof(FORMAT07);
        }
    }
    //����
    if (MemReadRealData(nSeq, F18, &nCurr[0], sizeof(nCurr)) < 0)
    {
        memset(&pData[nDataLen], 0xEE, 4 * MAX_PHASE_NUM);
        nDataLen += 4 * MAX_PHASE_NUM;
    }
    else
    {
        for (i = 0; i < (2 + MAX_ANLOG_NUM); i++)
        {
            nDataLen += Format26ToFormat25(&nCurr[i] ,&pData[nDataLen]);
        }
    }
    //���ڹ���
    if (MemReadRealData(nSeq, F21, &nWatt[0], sizeof(nWatt)) < 0)
    {
        memset(&pData[nDataLen], 0xEE, 4 * MAX_PHASE_NUM);
        nDataLen += 4 * MAX_PHASE_NUM;
    }
    else
    {
        for (i = 0; i < (1 + MAX_PHASE_NUM); i++)
        {
            memcpy(&pData[nDataLen] , &nWatt[i] ,sizeof(FORMAT09));
            nDataLen += sizeof(FORMAT09);
        }
    }
    return nDataLen;
}

//F129  ��ǰ�����й�����ʾֵ
INT16U GB3761GetClassIF129(INT16U nPn, INT8U *pData, INT16U nMaxDataLen)
{
    FORMAT11    nEnergy[MAX_FEE_NUM + 1] = {0};
	INT16U   nMinLen  = sizeof(FORMAT15) + 1 + (1 + MAX_FEE_NUM) * sizeof(FORMAT14);
	METER_INFO	sMeterInfo;
	pMETER_INFO	pMeterInfo	= &sMeterInfo;
    INT8U		nReadTime[6];							//����ʱ��YY-MM-DD-HH-MM-SS
    INT16U      nSeq        = 0;
    INT32S		nDataLen	= 0;
    INT8U       i           = 0;
    STD_TIME    sTime;
    
    memset(&sTime, 0 ,sizeof(sTime));

	if (NULL == pData || nMaxDataLen < nMinLen)
	{
		return 0;
	}
	nSeq = GetSeqFromMp(nPn);
    if (GetMeterInfo(nSeq, pMeterInfo) < 0)
    {
        return 0;
    }
    MemReadRealData(nSeq, F2, &nReadTime[0], 6);
    memcpy(&sTime.nSec, &nReadTime[0], 6);
    //�жϳ���ʱ���Ƿ���Ч
    if(!IsVldStdTime(&sTime))
    {
        return 0;
    }
    memcpy(&pData[nDataLen], &nReadTime[1], 5);
    nDataLen += 5;
    
    pData[nDataLen++] = MAX_FEE_NUM;
    if (MemReadRealData(nSeq, F5, &nEnergy[0], sizeof(nEnergy)) < 0)
    {
        memset(&pData[nDataLen], 0xEE, 5 * MAX_FEE_NUM);
        nDataLen += 5 * MAX_FEE_NUM;
    }
    else
    {
        for (i = 0; i < (1 + MAX_FEE_NUM); i++)
        {
            nDataLen += Format11ToFormat14(&nEnergy[i], &pData[nDataLen]);//�й�����ʾֵ ĩβ����0x0 ��0xEE;
        }
    }
    return nDataLen;
}

