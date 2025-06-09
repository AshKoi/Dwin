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
#include "GB3761SetPrm.h"
#include "Gw3761.h"
#include "MemDrvExt.h"
#include "MemDrv.h"
#include "Table.h"
#include "Fun.h"
#include "Macro.h"
#include "SysRes.h"
#include "Base.h"
#include "ChkCrc.h"
#include "Reset.h"
#include "MemTbl.h"
#include "SoftTimer.h"
#include "meter645.h"
#include "rmotlink.h"
#include "CarrCol.h"

/**************************************************************************************************
                                           相关函数实现
***************************************************************************************************/
/********************************************************************************
* 函数名: GB3761SetParam
* 输　入:
* 输　出:
* 返  回:
* 功  能: 参数设置
*********************************************************************************/
INT8S GB3761SetParam(pAPP_COMM pComm)
{
    INT16U nFn[GB3761_MAX_FN_NUM];
    INT8U  nFnNum = 0;
    INT16U nPn[8];
    INT16U nPnNum  = 0;
    INT16U nOffset = 0;     //数据标识和数据单元 处理长度偏移
    pGB3761_FRM_INFO   pInfo = NULL;

    if (NULL == pComm)
    {
    	return RET_ERR_POINT;
    }
    pInfo = (pGB3761_FRM_INFO)pComm->pFrmInfo;   //获取分析帧信息的指针
    pInfo->nTxDataLen = 5; //部分确认否认 DADT+AFN
    while (nOffset + 4 <= pInfo->nRxDataLen)
    {
        memset(&nPn[0], 0xFFFF, sizeof(nPn));
        memset(&nFn[0], 0, sizeof(nFn));
        if (RET_ERR_OK != GB3761DaDt2PnFn(&pComm->pRxFrmBuf[GB3761_DATA_IDX + nOffset], &nPn[0], &nPnNum, &nFn[0], &nFnNum, &pInfo->bAllMpFlag))  //Dadt转换Pnfn
		{
	        break;
        }
        nOffset += 4;                            //已经处理数据标识和数据单元长度
        if (RET_ERR_OK != GB3761SetParamProc(pComm, &nPn[0], nPnNum, &nFn[0], nFnNum, &nOffset))
        {
            break;
        }
    }
    return RET_ERR_OK;
}

/********************************************************************************
* 函数名: GB3761SetParamProc
* 输　入:
* 输　出:
* 返  回:
* 功  能: 参数设置
*********************************************************************************/
INT8S GB3761SetParamProc(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset)
{
    INT16U 	   i     = 0;                                //循环计数
    INT16U 	   j     = 0;                                //循环计数
	INT16U	   nDataLen = 0;
	INT16U     nMaxDataLen = 0;
	INT8U      nData[5];
    pGB3761_FRM_INFO pInfo = NULL;

    if (NULL == pComm || NULL == pPn || NULL == pFn || NULL == pOffset)
    {
    	return RET_ERR_POINT;
    }

    pInfo = (pGB3761_FRM_INFO)pComm->pFrmInfo;
	nMaxDataLen = pComm->nFrmLen - GB3761_FRM_RES_LEN;
    for (i = 0; i < nPnNum; i++)
    {
        for (j = 0; j < nFnNum; j++)
        {
        	nDataLen = 0;
            memset(&nData[0], 0, sizeof(nData));   //清空数据内容
            nDataLen += GB3761SetParamFnProc(pComm, pPn[i], pFn[j], pOffset, &nData[0], sizeof(nData));
            if (nDataLen > nMaxDataLen || 0 == nDataLen)  //一个Fn项数据长度超过帧长 或 无数据获取 丢弃
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
        }   //End Of for(nFcode = 0; nFcode < GB3761_MAX_FN_NUM; nFcode++)
    }       //End Of for(pn = 0; pn < nMaxMpNum; pn++)
    return RET_ERR_OK;
}

/********************************************************************************
* 函数名:GB3761SetParamFnProc
* 输　入: pFrmInfoRx 接收帧信息
          nPn        信息类
          nFn        信息点
* 输　出: pOffset    本次Fn数据处理长度
* 返  回:RET_ERR_OK 执行成功  RET_ERR_ERROR 执行失败退出 RET_ERR_RANG 数据单元已偏移可以执行下一个FN
* 功  能:参数设置3761标准Fn处理过程
*********************************************************************************/
INT16U GB3761SetParamFnProc(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen)
{
    INT8U   nErrData    = 0;
	BOOLEAN bSetSuc = FALSE;
    INT8U   nResult  = GB3761_PART_OTHER;//没有指定错误，就为其他错误
    INT16U  nDataLen = 0;
    pGB3761_FRM_INFO pInfo = NULL;
    pGB3761_FRM_MAP  pRxMap = NULL;
    INT32S    nRet  = 0;

    if (NULL == pComm || NULL == pOffset || NULL == pData)
    {
        return 0;
    }
    pInfo = (pGB3761_FRM_INFO)pComm->pFrmInfo;
    pRxMap = (pGB3761_FRM_MAP)pComm->pRxFrmBuf;
    if (RET_ERR_OK != GB3761PnFn2DADT(nPn, nFn, &pData[nDataLen]))
    {
        return 0;
    }
    nDataLen += 4;
    switch (nFn) //处理不同Fn项
    {
        case F1:      //F1终端通信参数设置
        {
        	bSetSuc = GB3761SetParamF1(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F3:      //F3 主站IP地址和端口
        {
            bSetSuc = GB3761SetParamF3(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F4:      //F4 主站电话号码和短信中心号码
        {
            bSetSuc = GB3761SetParamF4(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F7:      //终端抄表日设置
        {
            bSetSuc = GB3761SetParamF7(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F8:      //F8 终端上行通信工作方式
        {
            bSetSuc = GB3761SetParamF8(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F10:     //终端电能表/交流采样装置配置参数
        {
            bSetSuc = GB3761SetParamF10(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset, &nResult);
            break;
        }
        case F11:     //脉冲参数(F11)
        {
            bSetSuc = GB3761SetParamF11(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F12:     //终端状态量输入参数
        {
            bSetSuc = GB3761SetParamF12(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F16:     //F16 虚拟专网 用户名、密码
        {
            bSetSuc = GB3761SetParamF16(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F24:     //外扩，浙江规范设置地址Fn项
        {
            bSetSuc = GB3761SetParamF24(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F25:     //测量点基本参数
        {
            bSetSuc = GB3761SetParamF25(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F33:     //F33 终端抄表运行参数设置
        {
            bSetSuc = GB3761SetParamF33(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
        	break;
        }
        case F85:       //载波专变选择标志
        {
            bSetSuc = GB3761SetParamF85(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F94:     //外扩，浙江规范设置地址Fn项
        {
            bSetSuc = GB3761SetParamF94(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F130:
        {
            bSetSuc = GB3761SetParamF130(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        
        case F131:
        {
            bSetSuc = GB3761SetParamF131(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        
        case F132:
        {
            bSetSuc = GB3761SetParamF132(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        
        //终端交采地址
        case F225:
        {
            bSetSuc = GB3761SetParamF225(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        
        //MAC地址
        case F227:
        {
            bSetSuc = GB3761SetParamF227(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        
        default:
        {
            pInfo->bNoDataFlag = TRUE;
            *pOffset = pInfo->nRxDataLen;
            break;
        }
    }
    if (bSetSuc)
    {
    	pInfo->bSucFlag = TRUE;
    	pData[nDataLen++] = GB3761_PART_ACK;
    	//Update3761ParamStat(nFn);                  //更新终端参数状态
        //EvtERC3GenPrmChanRecord(pRxMap->uA3.sDet.nMSA, nPn, (nFn + 1));
    }
    else
    {
    	pInfo->bFailFlag = TRUE;
    	pData[nDataLen++] = nResult;
    }
    return nDataLen;
}

//F1 终端通信参数设置
BOOLEAN GB3761SetParamF1(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U       nFnLen      = 0;
	INT16U      nIndex      = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
    nIndex = *pOffset;
	nFnLen = 6;
	if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 != nPn)
    {
    	*pOffset += nFnLen;
    	return FALSE;
    }
    MemWriteCommPrm(F1, &pData[nIndex], nFnLen);
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F3 主站IP地址和端口
BOOLEAN GB3761SetParamF3(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U       nFnLen    = 0;
	INT16U      nIndex    = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex    = *pOffset;
	nFnLen = 2 * LEN_IP_ADDR + 4 + LEN_APN;
	if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 != nPn)
    {
        *pOffset += nFnLen;
        return FALSE;
    }
    MemWriteCommPrm(F3, &pData[nIndex], nFnLen);
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F4 主站电话号码和短信中心号码
BOOLEAN GB3761SetParamF4(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U       nFnLen    = 0;
	INT16U      nIndex    = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex = *pOffset;
	nFnLen = 8 + 8;
	if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 != nPn)
    {
        *pOffset += nFnLen;
        return FALSE;
    }
    MemWriteCommPrm(F4, &pData[nIndex], nFnLen);
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F7 终端IP地址和端口
BOOLEAN GB3761SetParamF7(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U       nFnLen          = 0;
	INT16U      nIndex          = 0;
    INT8U       nOffset         = 0;
    INT8U       nProxyType      = 0;                     //(F7)代理类型
    INT8U       nProxyLinkModl  = 0;                 //(F7)代理连接方式
    INT8U       nProxyUserLen   = 0;                  //(F7)代理用户名长度
    INT8U       nProxyPwdLen    = 0;	                //(F7)代理用户密码长度
    INT8U       nBuf[100]       ={0};
    INT8U*      pBuf            =&nBuf[0];
    
	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex = *pOffset;
	if ((nIndex + 20) > nDataLen)
	{
		*pOffset = nDataLen;
        return FALSE;
	}
	nProxyUserLen = pData[nIndex + 20];
	if ((nIndex + 21 + nProxyUserLen) > nDataLen)
	{
		*pOffset = nDataLen;
        return FALSE;
	}
	nProxyPwdLen = pData[nIndex + 21 + nProxyUserLen];
	nFnLen = 4 * LEN_IP_ADDR + 8 + nProxyUserLen + nProxyPwdLen;
	if ((nIndex + nFnLen) > nDataLen)  // 数据有效长度判断
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    nProxyType = pData[nIndex + LEN_IP_ADDR * 3];//代理类型0-3
    nProxyLinkModl = pData[nIndex + 19];//代理连接方式0-1
    if (0 != nPn || nProxyUserLen > LEN_PROXY_USER || nProxyPwdLen > LEN_PROXY_PWD
    	|| nProxyType > 3 || nProxyLinkModl > 1)
    {
    	*pOffset += nFnLen;
        return FALSE;
    }
    
    nIndex += MemCopy(&pBuf[nOffset], &pData[nIndex], 20);
    nOffset += 20;
    nProxyUserLen = pData[nIndex++];
    pBuf[nOffset++] = nProxyUserLen;
    memcpy(&pBuf[nOffset], &pData[nIndex], nProxyUserLen);
    nIndex += nProxyPwdLen;
    nOffset += 20;
    nProxyPwdLen = pData[nIndex++];
    pBuf[nOffset++] = nProxyPwdLen;
    memcpy(&pBuf[nOffset], &pData[nIndex], nProxyPwdLen);
    nIndex += nProxyPwdLen;
    nOffset += 20;
    memcpy(&pBuf[nOffset], &pData[nIndex], 2);
    nOffset += 2; 
    MemWriteCommPrm(F7, pBuf, nOffset);
    *pOffset = nIndex;
    return TRUE;
}

//F8 终端上行通信工作方式
BOOLEAN GB3761SetParamF8(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U       nFnLen    = 0;
	INT16U      nIndex    = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex    = *pOffset;
	nFnLen = 8;
	if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 != nPn)
    {
        *pOffset += nFnLen;
        return FALSE;
    }
    MemWriteCommPrm(F8, &pData[nIndex], nFnLen);
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F10 //终端电能表/交流采样装置配置参数, 国网376.1标准规约
BOOLEAN GB3761SetParamF10(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset, INT8U *pReSult)
{
	BOOLEAN bFlag   = 0;
	INT8U   nFnLen  = 0;
	INT16U  nSetNum = 0;
	INT8U   nSetSucNum = 0;
	INT16U  i = 0;
	INT16U  nSeq    = 0;
	INT16U  nMP     = 0;
	INT16U  nIndex  = 0;
    INT32U  nBaud   = 0;
	//INT16U  nSeqBuf[64];
	//INT16U  nMpBuf[64];
    const INT32U  GB3761BaudAry[8] = {0, 600, 1200, 2400, 4800, 7200, 9600, 19200};//用于F10
	MEM_METER_PRM sMtPrm;
	pMEM_METER_PRM pMtPrm = &sMtPrm;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex = *pOffset;
	nSetNum = pData[nIndex] + ((pData[nIndex+1]<<8) & 0xFF00);
	nIndex += 2;
	nFnLen = 27;
	if (0 != nPn)
    {
        nIndex += nSetNum * nFnLen;    //偏移配置数目
        *pOffset = nIndex;
        return FALSE;
    }

	//memset(&nSeqBuf[0], 0, sizeof(nSeqBuf));
    //memset(&nMpBuf[0], 0, sizeof(nMpBuf));
    for (i = 0; i < nSetNum; i++)
    {                           //数据有效性效验
        if ((nIndex + nFnLen) > nDataLen)
        {
        	nIndex = nDataLen;
        	break;
        }
        nSeq = pData[nIndex] + ((pData[nIndex + 1]<<8) & 0xFF00);
        nMP = pData[nIndex + 2] + ((pData[nIndex + 3]<<8) & 0xFF00);
        if (nSeq == 0 || nSeq > MAX_METER_NUM || nMP > MAX_MP)
        {
        	nIndex += nFnLen;
            continue;
        }
        memset(pMtPrm, 0, sizeof(MEM_METER_PRM));
        if (0 == nMP)//删除该序号下的所有参数
        {
            pMtPrm->bVld = FALSE;
            nIndex += nFnLen;
        }
        else
        {
            pMtPrm->bVld = TRUE;
            INT16U2Buf(&pMtPrm->nSeq[0], nSeq);
            INT16U2Buf(&pMtPrm->nMp[0], nMP);
            nIndex += 4;
            
            pMtPrm->nCom = pData[nIndex] & 0x1F;
            nBaud = GB3761BaudAry[(pData[nIndex]>>5) & 0x07];
            Long2Buf(&pMtPrm->nBaud[0], nBaud, 4);
            nIndex++;
            pMtPrm->nProt = pData[nIndex++];
            nIndex += MemCopy(&pMtPrm->nAddr[0], &pData[nIndex], LEN_METER_ADDR);
            nIndex += MemCopy(&pMtPrm->nPwd[0], &pData[nIndex], LEN_METER_PWD);
            pMtPrm->nFeeNum = (pData[nIndex++] & 0x3F);
            pMtPrm->nXsws = (pData[nIndex] & 0x03) + 1;
            pMtPrm->nZsws = ((pData[nIndex]>>2) & 0x03) + 4;
            nIndex++;
            nIndex += MemCopy(&pMtPrm->nColAddr[0], &pData[nIndex], LEN_METER_ADDR);
            pMtPrm->nMainType = (pData[nIndex]>>4) & 0x0F;
            pMtPrm->nPartType = pData[nIndex] & 0x0F;
            nIndex++;
            if (!IsVldMeterPrm(pMtPrm))
            {
            	continue;
            }
            if (IsRedupMtAddr(pMtPrm))
            {
                *pReSult = GB3761_PART_ADDR;//地址重复
                continue;
            }
        }
        
        //判断载波抄表
        //添加载波、无线节点信息（目前每下发一次都添加，后续增加节点参数表，载波任务中添加）
        /*if (g_nCarrZbSelFlg == TYPE_CARR)
        {
            //if (g_nCarrModType != CARR_MOD_DX)
            {
                if (g_nCarrInitFlg == 0)
                {
                    continue;
                }
                if (pMtPrm->nCom == 0x1F)
                {
                    CarrRouteAdd(&pMtPrm->nAddr[0], 2);
                }
            }
        }*/
        if (pMtPrm->nCom == 0x1F)
        {
            //载波表时需要判断
            if (g_nCarrZbSelFlg == TYPE_CARR)
            {
                if (g_nCarrInitFlg == 0)
                {
                    continue;
                }
                CarrRouteAdd(&pMtPrm->nAddr[0], 2);
            }
        }
        
        if (nMP == 1)
        {
            //MemWriteAcPrm(F19, &pMtPrm->nAddr[0], LEN_METER_ADDR);
        }
        MemWriteMeterPrm(nSeq, F10, pMtPrm, sizeof(MEM_METER_PRM));
        if (nSetSucNum < 64)//用于清除相同测量点;
        {
            //nSeqBuf[nSetSucNum] = nSeq;
            //nMpBuf[nSetSucNum]  = nMP;
        }
        nSetSucNum++;
    }//End Of for(i = 0; i < nSetNum; i++)
    if (nSetSucNum > 0)  //测量点数据有设置 重新建立载波节点列表
    {
        if (nSetSucNum > 64)
        {
            //CleanSameMp(&nSeqBuf[0], &nMpBuf[0], 64, MT_TYPE_DGN); //清除相同测量点号的电表参数
        }
        else
        {
            //CleanSameMp(&nSeqBuf[0], &nMpBuf[0], nSetSucNum, MT_TYPE_DGN);
        }
        //TermStatInfoRefresh();  //信息统计
        //MpStatInfoRefresh();
        bFlag = TRUE;
        //MemWriteXSaveData(MEM_OBJ_X_SAVE_TERM, 0, MEM_SAVE_TERM_METER_UPDATE_ID, &bFlag, 1);
        //OSQPost(TaskQueue[QUEUE_METER_SCHE_IDX], (void *)(MSG_METER_PRM_UPDATE));
    }
    MeterMpMap(1);
    /*if (g_nCarrZbSelFlg != TYPE_CARR)   //载波抄表配置后不立即抄表，反正影响载波节点添加
    {
        RdMeterSemProc();
    }*/
    if (pMtPrm->nCom == 0x1F)
    {
        if (g_nCarrZbSelFlg != TYPE_CARR)   //载波抄表配置后不立即抄表，反正影响载波节点添加
        {
            RdMeterSemProc();
        }
    }
    else
    {
        RdMeterSemProc();
    }
    *pOffset = nIndex;
    if (0 == (nSetSucNum - nSetNum))
    {
        return TRUE;
    }
    return FALSE;
}

//F11 脉冲参数
BOOLEAN GB3761SetParamF11(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U    i         = 0;
	INT8U    nFnLen    = 0;
	INT8U    nCom      = 0;
	INT8U    nSetNum   = 0;
	INT8U    nSetSucNum = 0;
	INT16U   nIndex    = 0;
	INT16U   nMP       = 0;
	//INT16U   nSeqBuf[64];
	//INT16U   nMpBuf[64];
	MEM_PULSE_PRM sPulPrm;
	pMEM_PULSE_PRM pPulPrm = &sPulPrm;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}

	nIndex = *pOffset;
	nSetNum = pData[nIndex++];               //脉冲配置路数0-64
	nFnLen  = 5;
    if (0 != nPn)
    {
    	nIndex += nFnLen * nSetNum;
        *pOffset = nIndex;
        return FALSE;
    }
	//memset(&nSeqBuf[0], 0, sizeof(nSeqBuf));
    //memset(&nMpBuf[0], 0, sizeof(nMpBuf));
    for (i = 0 ; i < nSetNum ; i++ )
    {
        if ((nIndex + nFnLen) > nDataLen)
        {
        	nIndex = nDataLen;
        	break;
        }
        nCom = pData[nIndex];
        nMP  = pData[nIndex + 1];
        if (0 == nCom || nCom > MAX_PULSE_NUM || nMP > 64)
        {
        	nIndex += nFnLen;
            continue;
        }
        memset(pPulPrm, 0, sizeof(MEM_PULSE_PRM));
        if (0 == nMP)
        {
            pPulPrm->bVld = FALSE;
            nIndex += nFnLen;
        }
        else
        {
        	pPulPrm->bVld = TRUE;
            pPulPrm->nPulsePort = nCom;
            pPulPrm->nPulseMp  = nMP;
            nIndex += 2;
            pPulPrm->nPulseAttr = pData[nIndex++];
            memcpy(&pPulPrm->nPulseConst[0], &pData[nIndex],2);
            nIndex += 2;
        }
        MemWriteGrpPrm(F11, nCom - 1, pPulPrm, sizeof(MEM_PULSE_PRM));
        if (nSetSucNum < 64)//用于清除相同测量点;
        {
            //nSeqBuf[nSetSucNum] = nCom;
            //nMpBuf[nSetSucNum]  = nMP;
        }
        nSetSucNum++;
    }
    if (nSetSucNum > 0)
    {
        if (nSetSucNum <= 64)
        {
            //CleanSameMp(&nSeqBuf[0], &nMpBuf[0], nSetSucNum, MT_TYPE_PULSE); //清除相同测量点号的电表参数
        }
        //TermStatInfoRefresh();  //信息统计
        //MpStatInfoRefresh();
    }
    *pOffset = nIndex;
    if (0 == (nSetSucNum - nSetNum))
    {
        return TRUE;
    }
    return FALSE;
}

//F12 终端状态量输入参数
BOOLEAN GB3761SetParamF12(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
    INT8U    nFnLen  = 0;
	INT16U   nIndex  = 0;
    
    if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
    nIndex = *pOffset;
	nFnLen = 2;
    if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 != nPn)
    {
        *pOffset += nFnLen;
        return FALSE;
    }
    
    MemWriteGrpPrm(F12, 0, &pData[nIndex], nFnLen);
    nIndex += nFnLen; 
    *pOffset = nIndex;
    return TRUE;
    
}

//F16 虚拟专网 用户名、密码
BOOLEAN GB3761SetParamF16(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U   nFnLen = 0;
	INT16U  nIndex = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex = *pOffset;
	nFnLen = 32 + 32;
	if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 != nPn)
    {
        *pOffset += nFnLen;
        return FALSE;
    }
	MemWriteCommPrm(F16, &pData[nIndex], nFnLen);
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F24
BOOLEAN GB3761SetParamF24(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
    INT8U    nFnLen  = 0;
	INT16U   nIndex  = 0;
    
    if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
    nIndex = *pOffset;
	nFnLen = 4;
    if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 != nPn)
    {
        *pOffset += nFnLen;
        return FALSE;
    }
    
    MemWriteCommPrm(F94, &pData[nIndex], nFnLen);
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F25
BOOLEAN GB3761SetParamF25(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
    INT8U       nFnLen = 0;
    FORMAT07    sTmpVolt;
	INT16U      nIndex = 0;
	MEM_MT_BASE_PRM    sMtBase;
	pMEM_MT_BASE_PRM   pMtBase = &sMtBase;
    
    if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
    nIndex = *pOffset;
	nFnLen = 11;
    if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 == nPn || nPn > MAX_MP)
    {
        *pOffset += nFnLen;
        return FALSE;
    }
    memset(pMtBase, 0, sizeof(MEM_MT_BASE_PRM));
    
    memcpy(pMtBase, &pData[nIndex], nFnLen);
    if (MemWriteMeterPrm(nPn, F25, pMtBase, sizeof(MEM_MT_BASE_PRM)) < 0)
    {
        return FALSE;
    }
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}


//F33 终端抄表运行参数设置
BOOLEAN GB3761SetParamF33(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U    i         = 0;
	INT8U    j         = 0;
	INT8U    nCom      = 0;
	//INT8U    nReadIntrvl = 0;
	INT8U    nReadPerdNum = 0;
	INT8U    nSetNum   = 0;
	INT8U    nSetSucNum = 0;
	INT16U   nFnLen    = 0;
	INT16U   nIndex    = 0;
	MEM_COM_PRM    sComPrm;
	pMEM_COM_PRM   pComPrm = &sComPrm;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex = *pOffset;
	nSetNum = pData[nIndex++];     //设置块数1-31
    if (0 != nPn)
    {
    	for (i = 0; i< nSetNum; i++)   //获取设置单元的偏移量
        {
            nReadPerdNum = pData[nIndex + nFnLen + 13]; //允许抄表时段 0-24
            nFnLen += (nReadPerdNum * (sizeof(FORMAT19) + sizeof(FORMAT19)) + 14);
        }
        nIndex += nFnLen;
        *pOffset = nIndex;
        return FALSE;
    }
    for (i = 0; i < nSetNum; i++)
    {
        if ((nIndex + 13) > nDataLen)
        {
        	*pOffset = nDataLen;
        	return FALSE;
        }
        nReadPerdNum = pData[nIndex + 13]; //允许抄表时段 0-24
        nFnLen = (nReadPerdNum * (sizeof(FORMAT19) + sizeof(FORMAT19)) + 14); //处理一组偏移一次
        if ((nIndex + nFnLen) > nDataLen)
        {
        	*pOffset = nDataLen;
        	return FALSE;
        }
        nCom = pData[nIndex];  //端口号1-31
        //nReadIntrvl = pData[nIndex + 9]; //抄表间隔1-60
        if (0 == nCom || nCom >= MAX_COM_NUM
        	|| nReadPerdNum > MAX_READ_PERD_NUM)
        {
            nIndex += nFnLen;
            continue;
        }
        MemReadRdPrm(F33, nCom - 1, pComPrm, sizeof(MEM_COM_PRM));
        pComPrm->nCom = pData[nIndex++];  //端口号1-31
        pComPrm->bsRunCtrl = pData[nIndex] + (pData[nIndex + 1]<<8);
        nIndex += 2;
        memcpy(&pComPrm->bsReadDay[0], &pData[nIndex], 4);
        nIndex += 4;
        memcpy(&pComPrm->sReadTime, &pData[nIndex], sizeof(FORMAT19));
        nIndex += sizeof(FORMAT19);
        if (CARR_PORT == nCom)
        {
            pComPrm->nReadIntrvl = pData[nIndex++] * 60; //国网12新规范，单位小时
        }
        else
        {
            pComPrm->nReadIntrvl = pData[nIndex++]; //抄表间隔1-60，单位分钟
        }
        memcpy(&pComPrm->sBoardTime, &pData[nIndex], sizeof(FORMAT18));
        nIndex += sizeof(FORMAT18);
        pComPrm->nReadPerdNum = pData[nIndex++];
        for (j = 0; j < pComPrm->nReadPerdNum; j++)
        {
            memcpy(&pComPrm->sReadPerd[j][0], &pData[nIndex], sizeof(FORMAT19));
            nIndex += sizeof(FORMAT19);
            memcpy(&pComPrm->sReadPerd[j][1], &pData[nIndex], sizeof(FORMAT19));
            nIndex += sizeof(FORMAT19);
        }
        MemWriteRdPrm(F33, nCom - 1, pComPrm, sizeof(MEM_COM_PRM));
        nSetSucNum++;
    }
    *pOffset = nIndex;
    if (0 == (nSetSucNum - nSetNum))
    {
    	return TRUE;
    }
	return FALSE;
}

//F85
BOOLEAN GB3761SetParamF85(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
    INT8U    nFnLen  = 0;
	INT16U   nIndex  = 0;
    INT8U    nSel   = 0;
    
    if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
    nIndex = *pOffset;
	nFnLen = 1;
    if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 != nPn)
    {
        *pOffset += nFnLen;
        return FALSE;
    }
    nSel = pData[nIndex];
    MemWriteCommPrm(F85, &nSel, nFnLen);
    
    if (nSel == 1)
    {
        g_nCarrZbSelFlg = TYPE_ZB;
    }
    else
    {
        g_nCarrZbSelFlg = TYPE_CARR;
    }
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F94
BOOLEAN GB3761SetParamF94(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
    INT8U    nFnLen  = 0;
	INT16U   nIndex  = 0;
    
    if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
    nIndex = *pOffset;
	nFnLen = 4;
    if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 != nPn)
    {
        *pOffset += nFnLen;
        return FALSE;
    }
    
    MemWriteCommPrm(F94, &pData[nIndex], nFnLen);
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F130
BOOLEAN GB3761SetParamF130(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
    INT8U    nFnLen     = 0;
	INT16U   nIndex     = 0;
    INT8U    nErrData   = 0;
    
    if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
    nIndex = *pOffset;
	nFnLen = 1;
    if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 != nPn)
    {
        *pOffset += nFnLen;
        return FALSE;
    }
    nErrData = pData[nIndex];
    if (nErrData == 0xAA)//灭
    {
        GprsPowerOff();
    }
    else
    {
        GprsPowerOn();//亮
    }
    
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F131
BOOLEAN GB3761SetParamF131(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
    INT8U    nFnLen     = 0;
	INT16U   nIndex     = 0;
    INT8U    nErrData   = 0;
    
    if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
    nIndex = *pOffset;
	nFnLen = 1;
    if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 != nPn)
    {
        *pOffset += nFnLen;
        return FALSE;
    }
    nErrData = pData[nIndex];
    if (nErrData == 0xAA) //不运行
    {
        CarrModeNwork();
    }
    else
    {
        CarrModework();   //运行
    }
    
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F132
BOOLEAN GB3761SetParamF132(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
    INT8U    nFnLen     = 0;
	INT16U   nIndex     = 0;
    INT8U    nErrData   = 0;
    
    if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
    nIndex = *pOffset;
	nFnLen = 1;
    if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 != nPn)
    {
        *pOffset += nFnLen;
        return FALSE;
    }
    nErrData = pData[nIndex];
    if (nErrData == 0xAA) //不能正常通訊
    {
        GprsCommAbnormal();
    }
    else
    {
        GprsCommNormal();   //通信正常
    }
    
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F225
BOOLEAN GB3761SetParamF225(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
    INT8U    nFnLen     = 0;
	INT16U   nIndex     = 0;
    
    if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
    nIndex = *pOffset;
	nFnLen = 6;
    if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 != nPn)
    {
        *pOffset += nFnLen;
        return FALSE;
    }
    
    //MemWriteAcPrm(F19, &pData[nIndex], LEN_METER_ADDR);
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F227
BOOLEAN GB3761SetParamF227(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
    INT8U    nFnLen     = 0;
	INT16U   nIndex     = 0;
    
    if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
    nIndex = *pOffset;
	nFnLen = 6;
    if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 != nPn)
    {
        *pOffset += nFnLen;
        return FALSE;
    }
    memcpy(&g_pMem->sXFramData.nMacAddr[0], &pData[nIndex], 6);
    MemoryWrite(STR_OFF(pMEM_FRAM_STORE, sXFramData.nMacAddr[0]), (INT8U*)&g_pMem->sXFramData.nMacAddr[0], 6, MEM_FRAM);
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}


