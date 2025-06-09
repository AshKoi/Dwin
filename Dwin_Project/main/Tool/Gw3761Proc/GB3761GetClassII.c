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
#include "GB3761GetClassII.h"
#include "Gw3761.h"
#include "MemDrvExt.h"
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
* 函数名: GB3761GetClassII
* 输　入:
* 输　出:
* 返  回:
* 功  能: 二类数据查询
*********************************************************************************/
INT8S GB3761GetClassII(pAPP_COMM pComm)
{
	INT16U 		nPn[20];
	INT16U 		nFn[GB3761_MAX_FN_NUM];
	INT16U		nPnNum   = 0;
	INT8U		nFnNum   = 0;
	INT16U  	nOffset  = 0;
	pGB3761_FRM_INFO pInfo = NULL;
	pGB3761_FRM_MAP  pRxMap = NULL;

	if (NULL == pComm)
    {
    	return RET_ERR_POINT;
    }

    pInfo = (pGB3761_FRM_INFO)pComm->pFrmInfo;   //获取分析帧信息的指针
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
        if (RET_ERR_OK != GB3761GetClassIIProc(pComm, &nPn[0], nPnNum, &nFn[0], nFnNum, &nOffset))
        {
            break;
        }
	}
	return RET_ERR_OK;
}

/********************************************************************************
* 函数名: GB3761GetClassIIProc
* 输　入:
* 输　出:
* 返  回:
* 功  能: 二类数据查询处理
*********************************************************************************/
INT8S GB3761GetClassIIProc(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset)
{
	INT16U    	i = 0;
	INT16U    	j = 0;
	INT16U		nDataLen = 0;
	INT8U     	nData[200];        //g_nProtTmpData 1024->200
    INT16U      nDataIdx   = *pOffset;
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
			nDataLen += GB3761GetClassIIFnProc(pComm, pPn[i], pFn[j], &nDataIdx, &nData[0], sizeof(nData));
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
* 函数名:GB3761GetClassIIFnProc
* 输　入: pAppCom   接收帧信息
          nPn        信息类
          nFn        信息点
* 输　出: pOffset    本次Fn数据处理长度
          pData       查询获取数据指针
          nDataLen    查询回去数据长度
* 返  回:RET_ERR_OK 执行成功  RET_ERR_ERROR 执行失败退出 RET_ERR_RANG 数据单元已偏移可以执行下一个FN
* 功  能:获取II类数据3761标准Fn处理过程
*********************************************************************************/
INT16U GB3761GetClassIIFnProc(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen)
{
	INT8U   nCurveDen = 0;
	INT8U   nFirstNo = 0;
	INT8U   nNumPerDay[12];
	INT8U   nDays    = 0;
    INT16U  nDataLen = 0;
    INT16U  nIndex   = 0;
    pGB3761_FRM_INFO pInfo = NULL;

    if (NULL == pComm || NULL == pOffset || NULL == pData)
    {
        return 0;
    }
    pInfo = (pGB3761_FRM_INFO)pComm->pFrmInfo;
    memset(&nNumPerDay[0], 0, sizeof(nNumPerDay));
    if (RET_ERR_OK != GB3761PnFn2DADT(nPn, nFn, &pData[nDataLen]))
    {
        return 0;
    }
    nDataLen += 4;
    nIndex = *pOffset;
    switch (nFn)
    {
        case F161:         //日冻结正向有功电能示值
        {
            nDataLen += GB3761GetClassIIF161(nPn, (pFORMAT20)(&pComm->pRxFrmBuf[GB3761_DATA_IDX + nIndex]), &pData[nDataLen], nMaxDataLen);
            break;
        }
        case F177:         //月冻结正向有功电能示值
        {
        	nDataLen += GB3761GetClassIIF177(nPn, (pFORMAT21)(&pComm->pRxFrmBuf[GB3761_DATA_IDX + nIndex]), &pData[nDataLen], nMaxDataLen);
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

//F161 日冻结正向有功电能示值
INT16U GB3761GetClassIIF161(INT16U nPn, const pFORMAT20 pTddTime, INT8U *pData, INT16U nMaxDataLen)
{
	INT16U nMinLen = sizeof(FORMAT20) + sizeof(FORMAT15) + 1 + (1 + MAX_FEE_NUM) * sizeof(FORMAT14);
	INT32S		nDataLen	= 0;
    INT16U      nSeq        = 0;   
    INT8U       i           = 0;
    FORMAT11    nEnergy[MAX_FEE_NUM + 1] = {0};
    METER_INFO	sMeterInfo;
	pMETER_INFO	pMeterInfo	= &sMeterInfo;
    STD_TIME    sTime;
    STD_TIME    sProtTime;
    FORMAT20    sHistTime;
    INT8U       nMeterAddr[LEN_METER_ADDR] = {0};		    	//电表地址(标签)
    INT8U		sDataTime[3] = {0};							//时间(标签)YY-MM-DD
    INT8U		nReadTime[6] = {0};							//读表时间YY-MM-DD-HH-MM-SS

	if (NULL == pTddTime || NULL == pData || nMaxDataLen < nMinLen)
	{
		return 0;
	}
    
    nSeq = GetSeqFromMp(nPn);
    if (GetMeterInfo(nSeq, pMeterInfo) < 0)
    {
        return 0;
    }
    memcpy(&sProtTime.nDay, pTddTime, 3);
    //读取日数据
    //判断表地址
    MemReadDayData(nSeq, F1, &sProtTime.nDay, &nMeterAddr[0], 6);
    if (0 != memcmp(&nMeterAddr[0], &pMeterInfo->nMeterAddr[0],6))
    {
        return 0;
    }
    //判断时间
    if (MemReadDayData(nSeq, F2, &sProtTime.nDay, &sDataTime[0], 3) < 0)
    {
        return 0;
    }
    if (0 != memcmp(&sProtTime.nDay, &sDataTime[0],3))
    {
        return 0;
    }
    memcpy(&pData[nDataLen], &sDataTime[0], 3);
    nDataLen += 3;
    MemReadDayData(nSeq, F3, &sProtTime.nDay, &nReadTime[0], 6);
    memcpy(&pData[nDataLen], &nReadTime[1], 5);
    nDataLen += 5;
    pData[nDataLen++] = MAX_FEE_NUM;
    if (MemReadDayData(nSeq, F4, &sProtTime.nDay, &nEnergy[0], sizeof(nEnergy)) < 0)
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

//F177 月冻结正向有功电能示值
INT16U GB3761GetClassIIF177(INT16U nPn, const pFORMAT21 pTdmTime, INT8U *pData, INT16U nMaxDataLen)
{
	INT16U nMinLen = sizeof(FORMAT21) + sizeof(FORMAT15) + 1 + (1 + MAX_FEE_NUM) * sizeof(FORMAT14);
	INT32S		nDataLen	= 0;
    INT16U      nSeq        = 0;   
    INT8U       i           = 0;
    FORMAT11    nEnergy[MAX_FEE_NUM + 1] = {0};
    METER_INFO	sMeterInfo;
	pMETER_INFO	pMeterInfo	= &sMeterInfo;
    STD_TIME    sTime;
    STD_TIME    sProtTime;
    FORMAT20    sHistTime;
    INT8U       nMeterAddr[LEN_METER_ADDR];		    	//电表地址(标签)
    INT8U		sDataTime[3];							//时间(标签)YY-MM-DD
    INT8U		nReadTime[6];							//读表时间YY-MM-DD-HH-MM-SS
    
	if (NULL == pTdmTime || NULL == pData || nMaxDataLen < nMinLen)
	{
		return 0;
	}
    
    nSeq = GetSeqFromMp(nPn);
    if (GetMeterInfo(nSeq, pMeterInfo) < 0)
    {
        return 0;
    }
    
    memcpy(&sProtTime.nMon, pTdmTime, 2);
    
    //读取日数据
    //判断表地址
    MemReadMonData(nSeq, F1, &sProtTime.nMon, &nMeterAddr[0], 6);
    if (0 != memcmp(&nMeterAddr[0], &pMeterInfo->nMeterAddr[0],6))
    {
        return 0;
    }
    //判断时间
    if (MemReadMonData(nSeq, F2, &sProtTime.nMon, &sDataTime[0], 2) < 0)
    {
        return 0;
    }
    if (0 != memcmp(&sProtTime.nMon, &sDataTime[0],2))
    {
        return 0;
    }
    memcpy(&pData[nDataLen], &sDataTime[0], 2);
    nDataLen += 2;
    MemReadMonData(nSeq, F3, &sProtTime.nMon, &nReadTime[0], 6);
    memcpy(&pData[nDataLen], &nReadTime[1], 5);
    nDataLen += 5;
    pData[nDataLen++] = MAX_FEE_NUM;
    if (MemReadMonData(nSeq, F4, &sProtTime.nMon, &nEnergy[0], sizeof(nEnergy)) < 0)
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

