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
#include "GB3761GetPrmExt.h"
#include "Gw3761.h"
#include "MemDrvExt.h"
#include "Macro.h"
#include "rmotlink.h"
#include "Ethnetlink.h"
#include "Fun.h"
#include "MemTbl.h"


/********************************************************************************
* 函数名: GB3761GetParamExt
* 输　入:
* 输　出:
* 返  回:
* 功  能: 参数查询
*********************************************************************************/
INT8S GB3761GetParamExt(pAPP_COMM pComm)
{
	INT16U 		        nPn[8];
	INT16U 		        nFn[GB3761_MAX_FN_NUM];
	INT16U		        nPnNum      = 0;
	INT8U		        nFnNum      = 0;
	INT16U  	        nOffset     = 0;
	pGB3761_FRM_INFO    pInfo       = NULL;

	if (NULL == pComm)
    {
    	return RET_ERR_POINT;
    }

    pInfo = (pGB3761_FRM_INFO)pComm->pFrmInfo;   //获取分析帧信息的指针
    while (nOffset + 4 <= pInfo->nRxDataLen)
	{
		memset(&nPn[0], 0xFFFF, sizeof(nPn));
        memset(&nFn[0], 0, sizeof(nFn));
        if (RET_ERR_OK != GB3761DaDt2PnFn(&pComm->pRxFrmBuf[GB3761_DATA_IDX + nOffset], &nPn[0], &nPnNum, &nFn[0], &nFnNum, &pInfo->bAllMpFlag))  //Dadt转换Pnfn
        {
	        break;
	    }
        nOffset += 4;            //已经处理数据标识和数据单元长度
        if (RET_ERR_OK != GB3761GetParamProcExt(pComm, &nPn[0], nPnNum, &nFn[0], nFnNum, &nOffset))
        {
            break;
        }
	}
	return RET_ERR_OK;
}

/********************************************************************************
* 函数名: GB3761GetParamProcExt
* 输　入:
* 输　出:
* 返  回:
* 功  能: 参数查询
*********************************************************************************/
INT8S GB3761GetParamProcExt(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset)
{
	INT16U    	            i           = 0;
	INT16U    	            j           = 0;
	INT16U		            nDataLen    = 0;
	INT8U     	            nData[200];                 //g_nProtTmpData
    INT16U                  nMaxDataLen = 0;
	pGB3761_FRM_INFO        pInfo       = NULL;

    if (NULL == pComm || NULL == pPn || NULL == pFn || NULL == pOffset)
    {
    	return RET_ERR_POINT;
    }

    pInfo = (pGB3761_FRM_INFO)(pComm->pFrmInfo);
    nMaxDataLen = pComm->nFrmLen - GB3761_FRM_RES_LEN;
	for (i = 0; i < nPnNum; i++)
	{
		for (j = 0; j < nFnNum; j++)
		{
            nDataLen = 0;                               //数据长度清零，每次都清零
            memset(&nData[0], 0, sizeof(nData));        //清空数据内容
			nDataLen += GB3761GetParamFnProcExt(pComm, pPn[i], pFn[j], pOffset, &nData[0], sizeof(nData));
            if (0 == nDataLen || nDataLen > nMaxDataLen)  // 无数据 或 超过最大长度
            {
                continue;
            }
            /*if ((pInfo->nTxDataLen + nDataLen) > nMaxDataLen)//组帧数据超过帧长 发送
            {
                pInfo->bSendFlag = TRUE;
                if (RET_ERR_OK != GB3761FillTxFrm(pComm))
                {
                    return RET_ERR_ERROR;
                }
                if (RET_ERR_OK != GB3761SendTxFrm(pComm))
                {
                    return RET_ERR_ERROR;
                }
                pInfo->nTxDataLen = 0;  //重新组帧
            }*/
            pInfo->nTxDataLen += MemCopy(&pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen], &nData[0], nDataLen);
		}
	}
	return RET_ERR_OK;
}

/********************************************************************************
* 函数名:GB3761GetParamFnProcExt
* 输　入: pAppCom    接收帧信息
          nPn        信息类
          nFn        信息点
* 输　出: pOffset    本次Fn数据处理长度
          pData    查询获取数据指针
          nDataLen    查询回去数据长度
* 返  回:RET_ERR_OK 执行成功  RET_ERR_ERROR 执行失败退出 RET_ERR_RANG 数据单元已偏移可以执行下一个FN
* 功  能:参数查询3761标准Fn处理过程
*********************************************************************************/
INT16U GB3761GetParamFnProcExt(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen)
{
    INT32S              nRet        = 0;
	INT16U              nMaxLen     = 0;
	INT16U              nDataLen    = 0;
	pGB3761_FRM_INFO    pInfo       = NULL;

    if (NULL == pComm || NULL == pOffset || NULL == pData || nMaxDataLen < 4)
    {
        return 0;
    }
	pInfo = (pGB3761_FRM_INFO)pComm->pFrmInfo;
    if (RET_ERR_OK != GB3761PnFn2DADT(nPn, nFn, pData))
    {
        return 0;
    }
    nDataLen += 4;
    nMaxLen = nMaxDataLen - 4;
    switch (nFn)
    {
        case F81:      //F81被抄口规约
        {
        	nDataLen += GB3761GetParamExtF81(nPn, &pData[nDataLen], nMaxLen);
            break;
        }
        case F82:      //F82GPRS在线状态
        {
        	nDataLen += GB3761GetParamExtF82(nPn, &pData[nDataLen], nMaxLen);
            break;
        }
        case F83:      //F83以太网在线状态
        {
        	nDataLen += GB3761GetParamExtF83(nPn, &pData[nDataLen], nMaxLen);
            break;
        }
        case F84:      //F84生成编号
        {
        	nDataLen += GB3761GetParamExtF84(nPn, &pData[nDataLen], nMaxLen);
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


//F81被抄口规约
INT16U GB3761GetParamExtF81(INT16U nPn, INT8U *pData, INT16U nMaxDataLen)
{
    INT8U   nDataLen = 0;
    
    if (NULL == pData || 0 != nPn)
	{
		return 0;
	}
    
    pData[nDataLen++] = g_pMem->sXRamData.nRdMtProtFlg;
    return nDataLen;
}

//F82GPRS在线状态
INT16U GB3761GetParamExtF82(INT16U nPn, INT8U *pData, INT16U nMaxDataLen)
{
    INT8U   nDataLen = 0;
    
    if (NULL == pData || 0 != nPn)
	{
		return 0;
	}
    
    if (g_pRmotValClr->nLoginSucc > 0)
    {
        pData[nDataLen++] = 1;
        SendAtCmdAndWaitATRespSlip((char*)"AT#SH=1\r\n", (char*)g_Modl_OK, 1000, 1);
    }
    else
    {
        pData[nDataLen++] = 0;
    }
    return nDataLen;
}

//F83以太网在线状态
INT16U GB3761GetParamExtF83(INT16U nPn, INT8U *pData, INT16U nMaxDataLen)
{
    INT8U   nDataLen = 0;
    
    if (NULL == pData || 0 != nPn)
	{
		return 0;
	}
    
    if (g_sEnetStateClr.bEnetCreatSockSucc)
    {
        if (g_sEnetStateClr.nSock >= 0)
        {
            //close(g_sEnetStateClr.nSock);
        }
    }
    pData[nDataLen++] = g_sEnetStateClr.bEnetCreatSockSucc;
    return nDataLen;
}

//F84生成编号
INT16U GB3761GetParamExtF84(INT16U nPn, INT8U *pData, INT16U nMaxDataLen)
{
    INT8U   nDataLen = 0;
    
    if (NULL == pData || 0 != nPn)
	{
		return 0;
	}
    
    nDataLen += MemReadCommPrm(F95, &pData[nDataLen], 8);
    return nDataLen;
}