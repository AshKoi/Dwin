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
#include "GB3761GetPrm.h"
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
* 函数名: GB3761GetParam
* 输　入:
* 输　出:
* 返  回:
* 功  能: 参数查询
*********************************************************************************/
INT8S GB3761GetParam(pAPP_COMM pComm)
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
        if (RET_ERR_OK != GB3761GetParamProc(pComm, &nPn[0], nPnNum, &nFn[0], nFnNum, &nOffset))
        {
            break;
        }
	}
	return RET_ERR_OK;
}

/********************************************************************************
* 函数名: GB3761GetParamProc
* 输　入:
* 输　出:
* 返  回:
* 功  能: 参数查询
*********************************************************************************/
INT8S GB3761GetParamProc(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset)
{
	INT16U    	            i           = 0;
	INT16U    	            j           = 0;
	INT16U		            nDataLen    = 0;
	INT8U     	            nData[200];                 //g_nProtTmpData 1024->200
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
			nDataLen += GB3761GetParamFnProc(pComm, pPn[i], pFn[j], pOffset, &nData[0], sizeof(nData));
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
* 函数名:GB3761GetParamFnProc
* 输　入: pAppCom    接收帧信息
          nPn        信息类
          nFn        信息点
* 输　出: pOffset    本次Fn数据处理长度
          pData    查询获取数据指针
          nDataLen    查询回去数据长度
* 返  回:RET_ERR_OK 执行成功  RET_ERR_ERROR 执行失败退出 RET_ERR_RANG 数据单元已偏移可以执行下一个FN
* 功  能:参数查询3761标准Fn处理过程
*********************************************************************************/
INT16U GB3761GetParamFnProc(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen)
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
        case F7:      //终端IP地址和端口
        {
        	nDataLen += GB3761GetParamF7(nPn, &pData[nDataLen], nMaxLen);
            break;
        }
        case F10:     //终端电能表/交流采样装置配置参数
        {
            nDataLen = GB3761GetParamF10(pComm, nPn, pOffset, &pData[0], nMaxDataLen);
            if (nDataLen > 0)
            {
            	return nDataLen;
            }
            break;
        }
        case F11:     //脉冲参数
        {
            nDataLen = GB3761GetParamF11(pComm, nPn, pOffset, &pData[0], nMaxDataLen);
            if (nDataLen > 0)
            {
            	return nDataLen;
            }
            break;
        }
        case F12:      //终端状态量输入参数
        {
        	nDataLen += MemReadGrpPrm(nFn, nPn, &pData[nDataLen], nMaxLen);
        	break;
        }
        
        case F24:      //终端状态量输入参数
        {
        	nDataLen += MemReadCommPrm(F94, &pData[nDataLen], nMaxLen);
        	break;
        }
        case F25:      //测量点基本参数
        {
            nDataLen += GB3761GetParamF25(nPn, &pData[nDataLen], nMaxLen);
            break;
        }
        case F33:      //终端抄表运行参数设置
        {
        	nDataLen = GB3761GetParamF33(pComm, nPn, pOffset, &pData[0], nMaxDataLen);
            if (nDataLen > 0)
            {
            	return nDataLen;
            }
        	break;
        }
        case F225:  //交采地址
        {
          INT8U nAddr[6] = {0};
          
            //MemReadAcPrm(F19, &nAddr[0], LEN_METER_ADDR);
            memcpy(&pData[nDataLen], &nAddr[0], 6);
            nDataLen += 6;
            break;
        }
        case F227:  //MAC地址
        {
            MemoryRead(STR_OFF(pMEM_FRAM_STORE, sXFramData.nMacAddr[0]), (INT8U*)&g_pMem->sXFramData.nMacAddr[0], 6, MEM_FRAM);
            memcpy(&pData[nDataLen], &g_pMem->sXFramData.nMacAddr[0], 6);
            nDataLen += 6;
            break;
        }
        default:
        {
            nRet = MemReadCommPrm(nFn, &pData[nDataLen], nMaxLen);
            if (nRet > 0)
            {
                nDataLen += nRet;
            }
            else
            {
                pInfo->bNoDataFlag = TRUE;
            }
            break;
        }
    }
    if (nDataLen > 4)
    {
    	return nDataLen;
    }
    return 0;
}

//F7 终端IP地址和端口
INT16U GB3761GetParamF7(INT16U nPn, INT8U *pData, INT16U nMaxDataLen)
{
    INT16U      nMinLen         = LEN_IP_ADDR * 4 + 8 + LEN_PROXY_USER + LEN_PROXY_PWD;
	INT8U       nUserPasWLen    = 0;
    INT8U       nDataLen        = 0;
    INT8U       nIndex          = 0;
    INT8U       nOffset         = 0;
    INT32S      nRet            = 0;
    INT8U       nBuf[100]       ={0};
    INT8U*      pBuf            =&nBuf[0];
    
    if (NULL == pData || 0 != nPn || nMaxDataLen < nMinLen)
	{
		return 0;
	}
    nRet = MemReadCommPrm(F7, pBuf, nMaxDataLen);
    if (nRet >= 0)
    {
        memcpy(&pData[nDataLen], &pBuf[nIndex], 20);
        if (TRUE ==IsAllEqualVal(&pData[nDataLen], 4, 0))
        {
            memcpy(&pData[nDataLen], &g_pRomtParm->nTermIp[0], 4);
        }
        
        nIndex += 20;
        nDataLen += 20;
        nUserPasWLen = pBuf[nIndex++];
        pData[nDataLen++] = nUserPasWLen;
        memcpy(&pData[nDataLen], &pBuf[nIndex], nUserPasWLen);
        nIndex += 20;
        nDataLen += nUserPasWLen;
        nUserPasWLen = pBuf[nIndex++];
        pData[nDataLen++] = nUserPasWLen;
        memcpy(&pData[nDataLen], &pBuf[nIndex], nUserPasWLen);
        nIndex += 20;
        nDataLen += nUserPasWLen;
        memcpy(&pData[nDataLen], &pBuf[nIndex], 2);
        nDataLen += 2;
    }
    return nDataLen;
}

//F10 //终端电能表/交采配置参数
INT16U GB3761GetParamF10(pAPP_COMM pComm, INT16U nPn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen)
{
    INT16U              nMinLen     = 9 + LEN_METER_ADDR + LEN_METER_PWD + LEN_METER_ADDR;
	INT8U               nDaDt[4];
	INT8U               *pNumPos    = NULL;
	INT16U              nSucNum     = 0;             //查询电表数量
	INT16U              nQueryNum   = 0;
	INT16U              nSeq        = 0;
    INT16U              nMp         = 0;
	INT16U              nDataLen    = 0;
	INT16U              nIndex      = *pOffset;
	INT16U              i           = 0;
    INT32U              nBaud       = 0;
	pGB3761_FRM_INFO    pInfo       = NULL;
    MEM_METER_PRM       sMtPrm;
    pMEM_METER_PRM      pMeterPrm = &sMtPrm;
    
    if (NULL == pComm || NULL == pOffset || NULL == pData || 0 != nPn || nMaxDataLen < nMinLen)
	{
		return 0;
	}
    
    pInfo = (pGB3761_FRM_INFO)pComm->pFrmInfo;
    memset(&nDaDt[0], 0, sizeof(nDaDt));
    memcpy(&nDaDt[0], &pData[0], 4); //暂存DADT
    nQueryNum = pComm->pRxFrmBuf[GB3761_DATA_IDX + nIndex] + ((pComm->pRxFrmBuf[GB3761_DATA_IDX + nIndex + 1]<<8) & 0xFF00);
    nIndex += 2;
    for (i = 0; i < nQueryNum; i++)
    {
        //判断数据有效性
		if ((nIndex + 2) > pInfo->nRxDataLen)
        {
        	*pOffset = pInfo->nRxDataLen;
        	if (nSucNum > 0)     //判断是否填充电表参数
		    {
		        pNumPos[0] = (INT8U)nSucNum;   //更新电表数量
		        pNumPos[1] = (nSucNum>>8);
			    return nDataLen;
		    }
		    return 0;
        }
        nSeq = pComm->pRxFrmBuf[GB3761_DATA_IDX + nIndex] + ((pComm->pRxFrmBuf[GB3761_DATA_IDX + nIndex + 1]<<8) & 0xFF00);
        nIndex += 2;
        memset(pMeterPrm, 0, sizeof(MEM_METER_PRM));
        MemReadMeterPrm(nSeq, F10, pMeterPrm, sizeof(MEM_METER_PRM));
        nSeq = Buf2INT16U(&pMeterPrm->nSeq[0]);
        nMp = Buf2INT16U(&pMeterPrm->nMp[0]);
        if ((pMeterPrm->bVld == 0) || (nSeq == 0) || (nMp == 0))
        {
            continue;
        }
        
        //拷贝数据
        if (0 == nSucNum) //第一个电表
    	{
            pInfo->nTxDataLen += MemCopy(&pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen], &nDaDt[0], 4);
            pNumPos = &pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen];//保存获取数量的位置
            pInfo->nTxDataLen += 2;
    	}
        if (nDataLen > 0)
        {
        	pInfo->nTxDataLen += MemCopy(&pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen], &pData[0], nDataLen); //复制电表参数数据
            memset(&pData[0], 0, nDataLen);
            nDataLen = 0;  //数据长度计数
        }
        memcpy(&pData[nDataLen], &pMeterPrm->nSeq[0], 2);
        nDataLen += 2;
        memcpy(&pData[nDataLen], &pMeterPrm->nMp[0], 2);
        nDataLen += 2;
        nBaud = Buf2Long(&pMeterPrm->nBaud[0], 4);
        pData[nDataLen++] = (pMeterPrm->nCom & 0x1F) | ((BaudChange(nBaud)<<5) & 0xE0);
        pData[nDataLen++] = pMeterPrm->nProt;
        nDataLen += MemCopy(&pData[nDataLen], &pMeterPrm->nAddr[0], LEN_METER_ADDR);
        nDataLen += MemCopy(&pData[nDataLen], &pMeterPrm->nPwd[0], LEN_METER_PWD);
        
        pData[nDataLen++] = pMeterPrm->nFeeNum;
        pData[nDataLen++] = ((pMeterPrm->nXsws-1) & 0x03) | (((pMeterPrm->nZsws - 4)<<2) & 0x0C);
        nDataLen += MemCopy(&pData[nDataLen], &pMeterPrm->nColAddr[0], LEN_METER_ADDR);
        pData[nDataLen++] = ((pMeterPrm->nMainType<<4) & 0xF0) | (pMeterPrm->nPartType & 0x0F);
        nSucNum++;    //电表数量加1
        //判断数据长度
        /*if ((pInfo->nTxDataLen + nDataLen) > (pComm->nFrmLen - GB3761_FRM_RES_LEN))     //已有的长度 + 缓冲区里面的长度是否已经大于最大的发送长度
        {
            pInfo->bSendFlag = TRUE;
            if (nSucNum > 1)               //如果获取过电表参数 更新电表数量
            {
                pNumPos[0] = (INT8U)(nSucNum - 1);
                pNumPos[1] = ((nSucNum - 1)>>8);
                nSucNum = 1;
            }
            if (RET_ERR_OK != GB3761FillTxFrm(pComm))
            {
            	*pOffset = nIndex;
                return 0;
            }
            if (RET_ERR_OK != GB3761SendTxFrm(pComm))
            {
            	*pOffset = nIndex;
                return 0;
            }
            pInfo->nTxDataLen = 0; //数据单元长度清零
        	pInfo->nTxDataLen += MemCopy(&pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen], &nDaDt[0], 4);
            pNumPos = &pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen];//保存获取数量的位置
            pInfo->nTxDataLen += 2;
        }*/
        
    }
    *pOffset = nIndex;
    if (nSucNum > 0)     //判断是否填充电表参数
    {
        pNumPos[0] = (INT8U)nSucNum;   //更新电表数量
		pNumPos[1] = (nSucNum>>8);
	    return nDataLen;
    }
    return 0;
}

//F11 脉冲参数
INT16U GB3761GetParamF11(pAPP_COMM pComm, INT16U nPn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen)
{
	INT16U nMinLen  = 5;
	INT8U  nDaDt[4];
	INT8U *pNumPos = NULL;
	INT8U  nSucNum   = 0;             //查询数量
	INT8U  nQueryNum = 0;
	INT8U  nCom      = 0;
	INT16U nDataLen  = 0;
	INT16U nIndex  = *pOffset;
	INT16U i = 0;
	pGB3761_FRM_INFO pInfo = NULL;
    MEM_PULSE_PRM   sPulsePrm;
	pMEM_PULSE_PRM  pPulsePrm = &sPulsePrm;  //终端端口参数

	if (NULL == pComm || NULL == pOffset || NULL == pData || 0 != nPn || nMaxDataLen < nMinLen)
	{
		return 0;
	}
    pInfo = (pGB3761_FRM_INFO)pComm->pFrmInfo;
    memset(&nDaDt[0], 0, sizeof(nDaDt));
    memcpy(&nDaDt[0], &pData[0], sizeof(nDaDt)); //暂存DADT
	nQueryNum = pComm->pRxFrmBuf[GB3761_DATA_IDX + nIndex++];
    for (i = 0; i < nQueryNum; i++)
    {
    	if ((nIndex + 1) > pInfo->nRxDataLen)
        {
        	*pOffset = pInfo->nRxDataLen;
        	if (nSucNum > 0)           //如果获取过参数 更新数量
		    {
		        pNumPos[0] = nSucNum;   //更新数量
			    return nDataLen;
		    }
		    return 0;
        }
        nCom = pComm->pRxFrmBuf[GB3761_DATA_IDX + nIndex++];
        if (0 == nCom || nCom > MAX_PULSE_NUM)
        {
        	continue;
        }
        memset(pPulsePrm, 0, sizeof(MEM_PULSE_PRM));
        if (MemReadGrpPrm(F11, nCom - 1, pPulsePrm, sizeof(MEM_PULSE_PRM)) < 0)
        {
            continue;
        }
        if (!pPulsePrm->bVld)
        {
            continue;
        }
        //拷贝数据
        if (0 == nSucNum)   //判断是不是第一个参数
        {
			pInfo->nTxDataLen += MemCopy(&pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen], &nDaDt[0], 4);
            pNumPos = &pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen];//保存获取数量的位置
            pInfo->nTxDataLen += 1;
        }
        if (nDataLen > 0)
        {
        	pInfo->nTxDataLen += MemCopy(&pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen], &pData[0], nDataLen); //复制参数数据
            memset(&pData[0], 0, nDataLen);
            nDataLen = 0;  //数据长度计数
        }
        //获取数据
        pData[nDataLen++] = pPulsePrm->nPulsePort;
        pData[nDataLen++] = pPulsePrm->nPulseMp;
        pData[nDataLen++] = pPulsePrm->nPulseAttr;
        memcpy(&pData[nDataLen], &pPulsePrm->nPulseConst[0], 2);
        nDataLen += 2;
        nSucNum++;    //电表数量加1
        //判断数据长度
        /*if ((pInfo->nTxDataLen + nDataLen) > (pComm->nFrmLen - GB3761_FRM_RES_LEN))     //已有的长度 + 缓冲区里面的长度是否已经大于最大的发送长度
        {
            pInfo->bSendFlag = TRUE;
            if (nSucNum > 1)               //如果获取过参数 更新数量
            {
                pNumPos[0] = nSucNum - 1;
                nSucNum = 1;
            }
            if (RET_ERR_OK != GB3761FillTxFrm(pComm))
            {
            	*pOffset = nIndex;
                return 0;
            }
            if (RET_ERR_OK != GB3761SendTxFrm(pComm))
            {
            	*pOffset = nIndex;
                return 0;
            }
            pInfo->nTxDataLen = 0; //数据单元长度清零
        	pInfo->nTxDataLen += MemCopy(&pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen], &nDaDt[0], 4);
            pNumPos = &pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen];//保存获取数量的位置
            pInfo->nTxDataLen += 1;
        }*/
    }//End Of for (i = 1; i <= GB3761_MAX_MP_MEM_NUM; i++)
    *pOffset = nIndex;
    if (nSucNum > 0)           //如果获取过参数 更新数量
    {
        pNumPos[0] = nSucNum;
	    return nDataLen;
    }
    return 0;
}


//F25 测量点基本参数
INT16U GB3761GetParamF25(INT16U nPn, INT8U *pData, INT16U nMaxDataLen)
{
    INT16U nMinLen  = 6 + sizeof(FORMAT07) + sizeof(FORMAT23);
	INT16U nDataLen = 0;
	MEM_MT_BASE_PRM  sMtBase;
	pMEM_MT_BASE_PRM pMtBase = &sMtBase;
    
    if (NULL == pData || 0 == nPn || nPn > MAX_MP || nMaxDataLen < nMinLen)
	{
		return 0;
	}
    memset(pMtBase, 0, sizeof(MEM_MT_BASE_PRM));
    
    if (MemReadMeterPrm(nPn, F25, pMtBase, sizeof(MEM_MT_BASE_PRM)) < 0)
    {
        return 0;
    }
    memcpy(&pData[nDataLen], pMtBase, 11);
    nDataLen += 11;
    return nDataLen;
}

//F33 终端抄表运行参数设置
INT16U GB3761GetParamF33(pAPP_COMM pComm, INT16U nPn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen)
{
	INT16U nMinLen  = 9 + sizeof(FORMAT19) + sizeof(FORMAT18) + 2 * MAX_READ_PERD_NUM * sizeof(FORMAT19);
	INT8U  nDaDt[4];
	INT8U *pNumPos = NULL;
	INT8U  nSucNum  = 0;             //查询数量
	INT8U  nQueryNum = 0;
	INT8U  nCom     = 0;
	INT16U nDataLen = 0;
	INT16U nIndex  = *pOffset;
	INT8U  i = 0;
	INT8U  j = 0;
	pGB3761_FRM_INFO pInfo = NULL;
    MEM_COM_PRM   sComPrm;
	pMEM_COM_PRM  pComPrm = &sComPrm;  //终端端口参数

	if (NULL == pComm || NULL == pOffset || NULL == pData || 0 != nPn || nMaxDataLen < nMinLen)
	{
		return 0;
	}
    pInfo = (pGB3761_FRM_INFO)pComm->pFrmInfo;
    memset(&nDaDt[0], 0, sizeof(nDaDt));
    memcpy(&nDaDt[0], &pData[0], sizeof(nDaDt)); //暂存DADT
	nQueryNum = pComm->pRxFrmBuf[GB3761_DATA_IDX + nIndex++];
    for (i = 0; i < nQueryNum; i++)
    {
    	//判断数据有效性
    	if ((nIndex + 1) > pInfo->nRxDataLen)
        {
        	*pOffset = pInfo->nRxDataLen;
        	if (nSucNum > 0)           //判断是否填充参数
		    {
		        pNumPos[0] = nSucNum;   //更新数量
			    return nDataLen;
		    }
		    return 0;
        }
        nCom = pComm->pRxFrmBuf[GB3761_DATA_IDX + nIndex++];
        if (0 == nCom || nCom >= MAX_COM_NUM)
        {
        	continue;
        }
        memset(pComPrm, 0, sizeof(MEM_COM_PRM));
        if (MemReadRdPrm(F33, nCom - 1, pComPrm, sizeof(MEM_COM_PRM)) < 0)
        {
            continue;
        }
        /*if (RET_ERR_OK != MemReadParam(MEM_OBJ_PRM_COM, nCom, 0, pComPrm, sizeof(MEM_COM_PRM)))
        {
        	continue;
        }*/
        //拷贝数据
        if (0 == nSucNum)  //判断是不是第一个参数
        {
            pInfo->nTxDataLen += MemCopy(&pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen], &nDaDt[0], 4);
            pNumPos = &pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen];//保存获取数量的位置
            pInfo->nTxDataLen += 1;
        }
        if (nDataLen > 0)
        {
        	pInfo->nTxDataLen += MemCopy(&pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen], &pData[0], nDataLen); //复制参数数据
            memset(&pData[0], 0, nDataLen);
            nDataLen = 0;
        }
        //获取数据
        pData[nDataLen++] = pComPrm->nCom;
        pData[nDataLen++] = (INT8U)pComPrm->bsRunCtrl;
        pData[nDataLen++] = pComPrm->bsRunCtrl>>8;
        
        nDataLen += MemCopy(&pData[nDataLen], &pComPrm->bsReadDay[0], 4);
        nDataLen += MemCopy(&pData[nDataLen], &pComPrm->sReadTime, sizeof(FORMAT19));
        
        if (CARR_PORT == nCom)
        {
            pComPrm->nReadIntrvl /= 60;
            pData[nDataLen++] = (pComPrm->nReadIntrvl > 0xFF) ? (0xFF) : (pComPrm->nReadIntrvl); //国网12新规范，单位小时
        }
        else
        {
            pData[nDataLen++] = (pComPrm->nReadIntrvl > 0xFF) ? (0xFF) : (pComPrm->nReadIntrvl); //抄表间隔1-60，单位分钟
        }	
        nDataLen += MemCopy(&pData[nDataLen], &pComPrm->sBoardTime, sizeof(FORMAT18));
        if (pComPrm->nReadPerdNum > MAX_READ_PERD_NUM)
        {
        	pComPrm->nReadPerdNum = MAX_READ_PERD_NUM;
        }
        pData[nDataLen++] = pComPrm->nReadPerdNum;
        for (j = 0; j < pComPrm->nReadPerdNum; j++)
        {
        	nDataLen += MemCopy(&pData[nDataLen], &pComPrm->sReadPerd[j][0], sizeof(FORMAT19));
        	nDataLen += MemCopy(&pData[nDataLen], &pComPrm->sReadPerd[j][1], sizeof(FORMAT19));
        }
        nSucNum++;                  //数量加1
        //判断数据长度
        /*if ((pInfo->nTxDataLen + nDataLen) > (pComm->nFrmLen - GB3761_FRM_RES_LEN))     //已有的长度 + 缓冲区里面的长度是否已经大于最大的发送长度
        {
            pInfo->bSendFlag = TRUE;
            if (nSucNum > 1)               //如果获取过参数 更新数量
            {
                pNumPos[0] = nSucNum - 1;
                nSucNum = 1;
            }
            if (RET_ERR_OK != GB3761FillTxFrm(pComm))
            {
            	*pOffset = nIndex;
                return 0;
            }
            if (RET_ERR_OK != GB3761SendTxFrm(pComm))
            {
            	*pOffset = nIndex;
                return 0;
            }
            pInfo->nTxDataLen = 0; //数据单元长度清零
        	pInfo->nTxDataLen += MemCopy(&pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen], &nDaDt[0], 4);
            pNumPos = &pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen];//保存获取数量的位置
            pInfo->nTxDataLen += 1;
        }*/
    }//End Of for (i = 1; i <= GB3761_MAX_MP_MEM_NUM; i++)
    *pOffset = nIndex;
    if (nSucNum > 0)           //判断是否填充参数
    {
        pNumPos[0] = nSucNum;   //更新数量
	    return nDataLen;
    }
    return 0;
}



















