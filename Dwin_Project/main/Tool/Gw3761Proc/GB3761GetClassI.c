/******************************************************************************************************
* Copyright (c) 2010-2011 光一科技股份有限公司研发中心
* 文件名:
* 创建人: 研发中心应用开发部
* 日　期: 2011/01/24
* 修改人:
* 日　期:
* 描　述:
* 版　本: Version 1.0
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
                                           相关函数实现
***************************************************************************************************/
/********************************************************************************
* 函数名: GB3761GetClassI
* 输　入:
* 输　出:
* 返  回:
* 功  能: 一类数据查询
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

    pInfo  = (pGB3761_FRM_INFO)pComm->pFrmInfo;   //获取分析帧信息的指针
    pRxMap = (pGB3761_FRM_MAP)&pComm->pRxFrmBuf[0];
    while (nOffset + 4 <= pInfo->nRxDataLen)
	{
		memset(&nPn[0], 0xFFFF, sizeof(nPn));
        memset(&nFn[0], 0, sizeof(nFn));
        if (RET_ERR_OK != GB3761DaDt2PnFn(&pComm->pRxFrmBuf[GB3761_DATA_IDX + nOffset], &nPn[0], &nPnNum, &nFn[0], &nFnNum, &pInfo->bAllMpFlag))  //Dadt转换Pnfn
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
* 函数名: GB3761GetClassIProc
* 输　入:
* 输　出:
* 返  回:
* 功  能: 一类数据查询处理
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
    		nDataIdx = *pOffset;//置回初始值
    	}
		for (j = 0; j < nFnNum; j++)
		{
            nDataLen = 0;                               //数据长度清零，每次都清零
            memset(&nData[0], 0, sizeof(nData));        //清空数据内容
			nDataLen += GB3761GetClassIFnProc(pComm, pPn[i], pFn[j], &nDataIdx, &nData[0], sizeof(nData));
            if (nDataLen > nMaxDataLen || 0 == nDataLen)  //一个Fn项数据长度超过帧长 或 无数据获取 丢弃
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
* 函数名: GB3761GetClassIFnProc
* 输　入: pAppCom    接收帧信息
          nPn        信息类
          nFn        信息点
* 输　出: pOffset    本次Fn数据处理长度
          pData    查询获取数据指针
          nDataLen    查询回去数据长度
* 返  回:RET_ERR_OK 执行成功  RET_ERR_ERROR 执行失败退出 RET_ERR_RANG 数据单元已偏移可以执行下一个FN
* 功  能:获取I类数据3761标准Fn处理过程
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
        case F2:        //F2 终端日历时钟
        {
        	nDataLen += GB3761GetClassIF2(nPn, &pData[nDataLen], nMaxDataLen);
            break;
        }
		case F25:       //当前三相及总有/无功功率、功率因素，三相电压、电流、零序电流、视在功率
        {
        	nDataLen += GB3761GetClassIF25(nPn, &pData[nDataLen], nMaxDataLen);
            break;
        }
        case F129:         //当前正向有功电能示值
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


//F2 终端日历时钟
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

//F25 当前三相及总有/无功功率、功率因素，三相电压、电流、零序电流、视在功率
INT16U GB3761GetClassIF25(INT16U nPn, INT8U *pData, INT16U nMaxDataLen)
{
    FORMAT11    nEnergy[MAX_FEE_NUM + 1] = {0};
	FORMAT09    nWatt[MAX_PHASE_NUM + 1] = {0};
	FORMAT26    nFactor[MAX_PHASE_NUM + 1] = {0};
	FORMAT26    nCurr[MAX_PHASE_NUM] = {0};
	FORMAT07    nVolt[MAX_PHASE_NUM] = {0};
    METER_INFO	sMeterInfo;
	pMETER_INFO	pMeterInfo	= &sMeterInfo;
    INT8U		nReadTime[6];							//读表时间YY-MM-DD-HH-MM-SS
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
    //有功功率
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
    //无功功率
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
    //功率因数
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
    //电压
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
    //电流
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
    //视在功率
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

//F129  当前正向有功电能示值
INT16U GB3761GetClassIF129(INT16U nPn, INT8U *pData, INT16U nMaxDataLen)
{
    FORMAT11    nEnergy[MAX_FEE_NUM + 1] = {0};
	INT16U   nMinLen  = sizeof(FORMAT15) + 1 + (1 + MAX_FEE_NUM) * sizeof(FORMAT14);
	METER_INFO	sMeterInfo;
	pMETER_INFO	pMeterInfo	= &sMeterInfo;
    INT8U		nReadTime[6];							//读表时间YY-MM-DD-HH-MM-SS
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
    //判断抄表时间是否有效
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
            nDataLen += Format11ToFormat14(&nEnergy[i], &pData[nDataLen]);//有功电能示值 末尾加上0x0 或0xEE;
        }
    }
    return nDataLen;
}

