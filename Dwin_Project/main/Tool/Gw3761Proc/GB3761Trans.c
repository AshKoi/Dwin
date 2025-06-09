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
#include "GB3761Trans.h"
#include "Gw3762.h"
#include "MemDrvExt.h"
#include "Table.h"
#include "Fun.h"
#include "Macro.h"
#include "SysRes.h"
#include "Base.h"
#include "ChkCrc.h"
#include "Guimenu.h"
#include "CarrCol.h"
#include "meter645.h"
#include "GB3761SetPrmExt.h"

const INT32U g_nBaudArray[] ={
									UART_BPS_300, 	UART_BPS_600, 	UART_BPS_1200,
									UART_BPS_2400, 	UART_BPS_4800, 	UART_BPS_7200,UART_BPS_9600,UART_BPS_19200
								  };

/**************************************************************************************************
                                           相关函数实现
***************************************************************************************************/
/********************************************************************************
* 函数名: GB3761TransData
* 输　入:
* 输　出:
* 返  回:
* 功  能: 数据转发
*********************************************************************************/
INT8S GB3761TransData(pAPP_COMM pComm)
{
    INT16U 		nPn[8];
	INT16U 		nFn[GB3761_MAX_FN_NUM];
	INT16U		nPnNum   = 0;
	INT8U		nFnNum   = 0;
	INT16U  	nOffset  = 0;
	pGB3761_FRM_INFO pInfo = NULL;

	if (NULL == pComm)
    {
    	return RET_ERR_POINT;
    }

    pInfo = (pGB3761_FRM_INFO)(pComm->pFrmInfo);
	while (nOffset + 4 <= pInfo->nRxDataLen)
	{
		memset(&nPn[0], 0xFFFF, sizeof(nPn));
        memset(&nFn[0], 0, sizeof(nFn));
		if (RET_ERR_OK != GB3761DaDt2PnFn(&pComm->pRxFrmBuf[GB3761_DATA_IDX + nOffset], &nPn[0], &nPnNum, &nFn[0], &nFnNum, &pInfo->bAllMpFlag))  //Dadt转换Pnfn
		{
            break;
        }
        nOffset += 4;
        if (RET_ERR_OK != GB3761TransDataProc(pComm, &nPn[0], nPnNum, &nFn[0], nFnNum, &nOffset))
        {
            break;
        }
	}
	return RET_ERR_OK;
}

/********************************************************************************
* 函数名: GB3761TransDataProc
* 输　入:
* 输　出:
* 返  回:
* 功  能: 数据转发
*********************************************************************************/
INT8S GB3761TransDataProc(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset)
{
	INT16U    	i = 0;
	INT16U    	j = 0;
	INT16U		nDataLen = 0;
	INT8U     	nData[200];         //g_nProtTmpData
    INT16U      nMaxDataLen = 0;
	pGB3761_FRM_INFO pInfo = NULL;

    if (NULL == pComm || NULL == pOffset)
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
			nDataLen += GB3761TransDataFnProc(pComm, pPn[i], pFn[j], pOffset, &nData[0], sizeof(nData));
            if (nDataLen > nMaxDataLen || 0 == nDataLen)   //一个Fn项数据长度超过帧长 丢弃
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
* 函数名: GB3761TransDataFnProc
* 输　入:
* 输　出:
* 返  回:
* 功  能: 数据转发
*********************************************************************************/
INT16U GB3761TransDataFnProc(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen)
{
    
    INT16U      nIndex      = *pOffset;
    INT32S		nDataLen	    = 0;
    INT8U       nCom            = 0;
    INT8U       nTmpCom         = 0;
    INT8U       nTmpCom2        = 0;
    INT32U      nBaud           = 0;
    INT8U       nBaudIdx        = 0;
    INT32S      nLen            = 0;
    INT8U       i               = 0;
    INT8U       nBuf[300]       = {0};
    INT8U       nAddr[6]        ={0};
    INT8U       *pBuf         = &nBuf[0];
    INT8U       nProt           = 0;
    INT8U       nDi[4]          = {0};
    INT16U      nTransLen   = 0;                                //转发字节数
    pGB3761_FRM_INFO pInfo = NULL;

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
    
    switch (nFn)
    {
        case F1:
        {
            //F1透明转发
            nTransLen = 6 + pComm->pRxFrmBuf[GB3761_DATA_IDX + nIndex + 4] + ((pComm->pRxFrmBuf[GB3761_DATA_IDX + nIndex + 5]<<8) & 0xFF00); //透明转发字节数
            if ((nIndex + nTransLen) > pInfo->nRxDataLen)
            {
                *pOffset = pInfo->nRxDataLen;
                return 0;
            }
            nTmpCom    = pComm->pRxFrmBuf[GB3761_DATA_IDX + nIndex];                               //终端通信端口号
            nIndex++;
            if ((0 == nTmpCom) || (nTmpCom >= MAX_COM_NUM))
            {
                *pOffset += nTransLen;
                return 0;
            }
            if (nTmpCom == 2)
            {
                nCom = COMM_WH4852;
                nTmpCom2 = 3;
            }
            if (nTmpCom == 3)
            {
                nCom = COMM_WH485;
                nTmpCom2 = 1;
            }
            if ((nTmpCom == 31) || (nTmpCom == 29))
            {
                nCom = COMM_ZB;
                nTmpCom2 = 4;
            }
            nBaud   = g_nBaudArray[((pComm->pRxFrmBuf[GB3761_DATA_IDX + nIndex] >> 5) & 0x07)];   //透明转发通信控制字
            for (i = 0; i < 13; i++)
            {
                if (g_nUartBpsArray[i] == nBaud)
                {
                    nBaudIdx = i;
                    break;
                }
            }
            nIndex++;
            if (nCom == COMM_ZB)
            {
                if (nTmpCom == 29)
                {
                    SetCommCfg(nTmpCom2, nBaudIdx, 0);
                    //SetCommCfg(nTmpCom2, 6, 0);
                }
                else
                {
                    SetCommCfg(nTmpCom2, nBaudIdx, 2);
                    //SetCommCfg(nTmpCom2, 6, 2);
                }
            }
            else
            {
                SetCommCfg(nTmpCom2, nBaudIdx, 2);                      //设置通讯口
                //SetCommCfg(nTmpCom2, 4, 2);                             //485默认2400
            }
            
            nIndex++;                                               //透明转发接收等待报文超时时间
            nIndex++;                                               //透明转发接收等待字节超时时间
            //nLen = Buf2INT16U(&pData[nIndex]);                       //透明转发内容字节数k
            //nLen = nTransLen;
            nLen = nTransLen - 6;
            nIndex += 2;
            memcpy(&pBuf[0], &pComm->pRxFrmBuf[GB3761_DATA_IDX + nIndex], nLen);
            
            //转发
            if (nCom == COMM_ZB)
            {
                if (nTmpCom == 29)
                {
                    //CtrlProc(pData, nLen);
                    nLen = Meter645Proc(nCom, pBuf, nLen, 100);
                }
                else
                {
                    nLen = Meter645CheckPackage(NULL, &pBuf[0], nLen);
                    nLen = CarrPointRd(&pBuf[0], nLen);
                }
                if (nLen < 0)
                {
                    return -1;
                }
            }
            else
            {
                if (g_nRs485ErrFlg)
                {
                    return -1;
                }
                nLen = Meter645Proc(nCom, pBuf, nLen, 100);
                if (nLen < 0)
                {
                    return -1;
                }
            }
            
            //通信端口号
            pData[nDataLen++] = nTmpCom;
            
            //透明转发内容数
            INT16U2Buf(&pData[nDataLen], nLen);
            nDataLen += 2;
            memcpy(&pData[nDataLen], pBuf, nLen);
            nDataLen += nLen;
            break;
        }
        
        case F9:
        {
            nTransLen = 6 + pComm->pRxFrmBuf[GB3761_DATA_IDX + nIndex + 4] + ((pComm->pRxFrmBuf[GB3761_DATA_IDX + nIndex + 5]<<8) & 0xFF00); //透明转发字节数
            if ((nIndex + nTransLen) > pInfo->nRxDataLen)
            {
                *pOffset = pInfo->nRxDataLen;
                return 0;
            }
            nTmpCom    = pComm->pRxFrmBuf[GB3761_DATA_IDX + nIndex];                               //终端通信端口号
            nIndex++;
            if ((0 == nTmpCom) || (nTmpCom >= MAX_COM_NUM))
            {
                *pOffset += nTransLen;
                return 0;
            }
            if (nTmpCom == 2)
            {
                nCom = COMM_WH4852;
                nTmpCom2 = 3;
            }
            if (nTmpCom == 3)
            {
                nCom = COMM_WH485;
                nTmpCom2 = 1;
            }
            if ((nTmpCom == 31) || (nTmpCom == 29))
            {
                nCom = COMM_ZB;
                nTmpCom2 = 4;
            }
            
            if (nCom == COMM_ZB)
            {
                if (nTmpCom == 29)
                {
                    SetCommCfg(nTmpCom2, 6, 0);
                    //SetCommCfg(nTmpCom2, 6, 0);
                }
                else
                {
                    SetCommCfg(nTmpCom2, 6, 2);
                    //SetCommCfg(nTmpCom2, 6, 2);
                }
            }
            else
            {
                SetCommCfg(nTmpCom2, 4, 2);                      //设置通讯口
                //SetCommCfg(nTmpCom2, 4, 2);                             //485默认2400
            }
            
            nIndex++;                                               //中继级数
            memcpy(&nAddr[0], &pComm->pRxFrmBuf[GB3761_DATA_IDX + nIndex], 6);
            nIndex += 6;
            nProt = pComm->pRxFrmBuf[GB3761_DATA_IDX + nIndex];  //规约
            nIndex ++;
            memcpy(&nDi[0], &pComm->pRxFrmBuf[GB3761_DATA_IDX + nIndex], 4);
            for (i = 0; i < 4; i++)
            {
                pComm->pRxFrmBuf[GB3761_DATA_IDX + nIndex + i] += 0x33;
            }
            memcpy(pBuf, &pComm->pRxFrmBuf[GB3761_DATA_IDX + nIndex], 4);
            nIndex += 4;
            nLen = 4;
            nLen = Meter645MakeFrm(pBuf, nLen, sizeof(nBuf), &nAddr[0], 0x11);
            
            //转发
            if (nCom == COMM_ZB)
            {
                if (nTmpCom == 29)
                {
                    //CtrlProc(pData, nLen);
                    nLen = Meter645Proc(nCom, pBuf, nLen, 100);
                }
                else
                {
                    nLen = Meter645CheckPackage(NULL, &pBuf[0], nLen);
                    nLen = CarrPointRd(&pBuf[0], nLen);
                }
                if (nLen < 0)
                {
                    return -1;
                }
            }
            else
            {
                if (g_nRs485ErrFlg)
                {
                    return -1;
                }
                nLen = Meter645Proc(nCom, pBuf, nLen, 100);
                if (nLen < 0)
                {
                    return -1;
                }
            }
            
            
            
            //通信端口号
            pData[nDataLen++] = nTmpCom;
            memcpy(&pData[nDataLen], &nAddr[0], 6);
            nDataLen += 6;
            pData[nDataLen++] = 0x03;
            pData[nDataLen++] = nLen + 4;
            memcpy(&pData[nDataLen], &nDi[0], 4);
            nDataLen += 4;
            memcpy(&pData[nDataLen], pBuf, nLen);
            nDataLen += nLen;
            break;
        }
        default:
        {
            pInfo->bNoDataFlag = TRUE;
            return 0;
        }
    }
    *pOffset = nIndex;
    if (nDataLen > 4)
    {
    	return nDataLen;
    }
	return 0;
}

 /********************************************************************************
* 函数名: CtrlProc
* 输　入:
* 输　出:
* 返  回:
* 功  能: 控制板转发
*********************************************************************************/
INT32S CtrlProc(INT8U* pData, INT32S nLen)
{
    INT8U	l_buff[30]   = {0x68, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x68, 0x15, 0x02, 0x32, 0x32, 0x47, 0x16};
    INT8U   l_Tmpbuff[30] = {0};
    INT32S  l_ret = 0;
    memset(&l_Tmpbuff[0], 0, sizeof(l_Tmpbuff));
    
    V1_IRTXM_ON();
    
    
    //WriteComm(COMM_ZB, pData, nLen);
    WriteComm(COMM_ZB, l_buff, 14);
    l_ret = ReadCommMs(COMM_ZB, l_Tmpbuff, 100, 500, 500);
    if (l_ret > 0)
    {
        return l_ret;
    }
    return -1;
}










