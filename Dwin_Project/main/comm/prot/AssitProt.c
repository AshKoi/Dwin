/******************************************************************************************************
* Copyright (c) 2014
* 文件名:
* 创建人: 王一凡
* 日　期: 2014/01/18
* 修改人:
* 日　期:
* 描　述:
* 版　本: Version 1.0
*****************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "AssitProt.h"
#include "key.h"
#include "Base.h"
#include "Fun.h"
#include "Guimenu.h"
#include "Linkcom.h"

/**************************************************************************************************
                                             变量
***************************************************************************************************/
extern INT32S ReadCommMs(INT8U nCom, INT8U* pBuf, INT32U nNeedRdLen, INT32U nInter, INT32U nDelay);
extern INT32S WriteComm(INT8U nCom, INT8U* pBuf, INT32U nNeedLen);
/*
主辅机协议解析：
0x10	1	报头
Addr	2	485检测模块：1；
			载波检测模块：2；
			信号发生器无线：3；
			手抄机无线：4；
Type	2	I型集中器：1；
			II型集中器：2；
			I采：3；
			II采：4；
			表计：5；
			其他：0
Prot	2	基本协议：1；
			376.1协议：2；
			表协议：3；
			点抄：4；
			透明协议：5。	
Ctrl	1	控制码：读1，写2，主动上报3。（只针对基本协议使用，其他协议时为0）
			D7位表示上下行：1为上行
Pn		2	信息点：逻辑口
			本身：0；（基本协议时为0）
			485_1：1；
			485_2：2；
			485_3：3；
Di		2	数据标识：（只针对基本协议，其他协议时恒为0）
Len		2	数据内容长度
Data	x	Len
Cs		1	校验和（从报头至此的求和）
0x16	1	1
*/

/*
*****************************************************************************
* 函数名: AssistReadComm
* 输　入: 
* 输  出: 
* 返  回: 
*		
*		
* 功  能: 
*****************************************************************************
*/
static INT32U AssistReadComm(INT8U nCom, INT8U* pData, INT32S nLen, INT32S nInter, INT32S nDelay)
{
	INT32S	nRetLen = 0;
	
	nRetLen = ReadCommMs(nCom, pData, (INT32U)nLen, (INT32U)nInter, (INT32U)nDelay);
	return nRetLen;
}

/*
*****************************************************************************
* 函数名: AssistWriteComm
* 输　入: 
* 输  出: 
* 返  回: 
*		
*		
* 功  能: 
*****************************************************************************
*/
static INT32U AssistWriteComm(INT8U nCom, INT8U* pData, INT32S nLen)
{
	INT32S	nRetLen = 0;
	
	nRetLen = WriteComm(nCom, pData, (INT32U)nLen);	
	return nRetLen;
}


/*
*****************************************************************************
* 函数名: AssistMakeFrm
* 输　入: 
*			INT8U*			pData									缓冲区
*			INT32S			nDatalen
*			INT32S			nMaxDatalen
*			INT16U			nAssistAddr								辅机地址：区分不同的辅机
*			INT16U			nTermType								终端类型：I型、II型集中器等
*			INT16U			nProt									基本协议、376.1等
*			INT8U			nCtrl									控制码
*			INT16U			nPn										信息点
*			INT16U			nDi										数据标识
* 输  出: 
*			INT8U*			pData
* 返  回: 主辅机协议报文帧长度  
* 功  能: 
*****************************************************************************
*/
INT32S AssistMakeFrm(INT8U* pData, INT32S nDatalen, INT32S nMaxDatalen, INT16U nAssistAddr, INT16U nTermType, INT16U nProt, INT8U nCtrl, INT16U nPn, INT16U nDi)
{
	pASSIST_PROT		pAssistProt = (pASSIST_PROT)pData;
	
	if (NULL == pData)
	{
		return -1;
	}
	if (nDatalen < 0)
	{
		return -1;
	}
	if (ASSIST_PROT_HEAD_TAIL_LEN + nDatalen > nMaxDatalen)
	{
		return -1;
	}
	memmove(&pAssistProt->nData[0], pData, nDatalen);
	pAssistProt->nHead	= 0x10;
	INT16U2Buf(&pAssistProt->nAddr[0], nAssistAddr);
	INT16U2Buf(&pAssistProt->nType[0], nTermType);
	INT16U2Buf(&pAssistProt->nProt[0], nProt);
	pAssistProt->nCtrl	= nCtrl;
	INT16U2Buf(&pAssistProt->nPn[0], nPn);
	INT16U2Buf(&pAssistProt->nDi[0], nDi);
	INT16U2Buf(&pAssistProt->nDatalen[0], nDatalen);
	pAssistProt->nData[nDatalen] = CheckSum(pData, 0, nDatalen + ASSIST_PROT_HEAD_LEN);
	pAssistProt->nData[nDatalen + 1] = 0x16;
	
	return  (ASSIST_PROT_HEAD_TAIL_LEN + nDatalen);
}

/*
*****************************************************************************
* 函数名: AssistProtCheckPackage
* 输　入: 
*			INT8U*			pData
*			INT32S			nDatalen			接收到的报文长度
* 输  出: 
*			INT8U*			pData 				如果有完整帧，重新复制到 pData 
* 返  回: 返回完整帧的长度
* 功  能: 内部规约分析
*****************************************************************************
*/
INT32S AssistProtCheckPackage(INT8U* pData, INT32S nDatalen)
{
	INT16U	nFrmlen1 	= 0;
	INT16U	nCycCnt 	= 0;
	INT32S	i       	= 0;
	INT8U	nCs			= 0;

	if (NULL == pData)
	{
		return -1;
	}
	if (nDatalen < ASSIST_PROT_BASE_LEN)
	{
		return -1;
	}
	nCycCnt = (nDatalen - ASSIST_PROT_BASE_LEN + 1);
	for (i = 0; i < nCycCnt; i++)
	{
		if (0x10 != pData[i])
		{
			continue;
		}
		nFrmlen1 = Buf2INT16U(&pData[i + ASSIST_PROT_HEAD_LEN - 2]);  			//i + 12
		if (nFrmlen1 + ASSIST_PROT_HEAD_TAIL_LEN + i > nDatalen)
		{
			continue;
		}
		
		nCs = CheckSum(&pData[i], 0, nFrmlen1 + ASSIST_PROT_HEAD_LEN);
		if (nCs != pData[i + ASSIST_PROT_HEAD_LEN + nFrmlen1])
		{
			continue;
		}
		if (0x16 != pData[i + ASSIST_PROT_HEAD_LEN + nFrmlen1 + 1])
		{
			continue;
		}
		
		break;
	}
	if (i >= nCycCnt)
	{
		return -1;
	}
	
	nFrmlen1 = nFrmlen1 + ASSIST_PROT_HEAD_TAIL_LEN;
	memmove(pData, &pData[i], nFrmlen1);
	
	return (INT32S)nFrmlen1;
}

/*
*****************************************************************************
* 函数名: AssistProc
* 输　入: 			
*			INT8U*			pData
*			INT32S			nDatalen			帧内容长度
*			INT32S			nMaxDatalen			接收到的报文长度
* 输  出: 
*			INT8U*			pData 				完整帧
* 返  回: 返回完整帧的长度
* 功  能: 内部规约分析
*****************************************************************************
*/
INT32S AssistProc(INT8U* pData, INT32S nDatalen, INT32S nMaxDatalen)
{	
	pASSIST_PROT 	pAssistProt	= (pASSIST_PROT)pData;
	//INT16U			nAddr			= 0;
	INT32S			nCom 			= 0;
	INT32S			nTmp			= 0;
	INT32S			nTimeOut		= 10000;
    INT16U          nDi             = 0;
	
	if ((NULL == pData) || (nDatalen < 0) || (nMaxDatalen <= 0))
	{
		return -1;
	}
    
    if (KEY_CANCEL == PrintCommHex(pData, nDatalen))
    {
        return -1;    
    }
    
    nDi = MAKEWORD(pAssistProt->nDi[0], pAssistProt->nDi[1]);
    if (nDi == 0x2104)
    {
        nTimeOut = 20000;   
    }

	//nAddr = Buf2INT16U(&pAssistProt->nAddr[0]);
	if (0 == CarrierComGet(0))
	{
		nCom = SetCommCfg(1, 6, 2);
	}
	else
	{
		nCom = SetCommCfg(2, 10, 2);//SetCommCfg(2, 12, 2);
	}
    nCom = ProcCommCfgPend(nCom);
	AssistWriteComm(nCom, pData, nDatalen);
	memset(pData, 0, nMaxDatalen);
	nDatalen = 0;
	while(1)
	{
        if (KEY_CANCEL == GetKeyVal(1))
        {
            nTmp = CANCEL_VALUE;
            break;
        }
		if (nTimeOut < 0)
		{
			nTmp = -1;
			break;
		}
		nTmp = 0;
		nTmp = AssistReadComm(nCom, &pData[nDatalen], nMaxDatalen - nDatalen, 10, 500);
		if (nTmp > 0)
		{
			nDatalen += nTmp;
		}
		nTimeOut -= 500;

		if (nDatalen >= ASSIST_PROT_BASE_LEN)
		{
			nTmp = AssistProtCheckPackage(pData, nDatalen);
			if (nTmp > 0)
			{
                PrintCommHex( pData, nDatalen);
                if (nDi == MAKEWORD(pAssistProt->nDi[0], pAssistProt->nDi[1]))
                {
                    nDatalen = nTmp;
				    break;   
                }
                else
                {
                    nDatalen -= nTmp;
                    nTmp    = 0;
                }
			}
		}
	}
	ProcCommCfgPost(nCom);
	return nTmp;
}

#if 0
/*
*****************************************************************************
* 函数名: AssistFrmCheckPackage
* 输　入: 
*		pLINK_COM pLinkCom
* 输  出: 
* 返  回: 
*		0成功，-1失败
*       
*       
* 功  能: 有完整帧，返回0成功，否则-1
*****************************************************************************
*/
INT32S AssistProtAsynCheckPackage(void* pPara)
{
	INT16U		nHeadLen		= STR_OFF(pASSIST_PROT, nData);
	pLINK_COM   pLinkCom    	= (pLINK_COM)pPara;
    pBASE_BUF	pBaseBuf		= NULL;
	INT32U		i				= 0;
	INT8S		nRtn			= -1;
	INT16U		nDataLen		= 0;
    INT32U      nDataNum    	= 0;
    INT8U       nCs         	= 0;
	
	LINK_COM_VALID ;
	
	nDataNum = BASEBUF_BYTE_NUM(pBaseBuf);
	if (nDataNum < nHeadLen + 2)
	{
		return -1;
	}
#if 1 == HAVE_FRM_BUFF
#else	
	if (BASEBUF_RECV_PTR(0) < BASEBUF_PROC_PTR(0))
	{
		if (0 != FifoMoveLeftBaseBuf(pBaseBuf, BASEBUF_PROC_PTR(0)))
		{
			return -1;
		}
	}
#endif
	
	if (pLinkCom->nFrmLen > 0)
	{
		return 0;
	}
	
	for (i = 0; i < nDataNum - (nHeadLen + 2 - 1); i++)
	{	
		// 1. 报文头
		if (0x10 != BASEBUF_PROC_PBUF(i + 0))
		{
			continue;	
		}
		
		// 2. 长度
		nDataLen = MAKEWORD(BASEBUF_PROC_PBUF(i + nHeadLen - 2), BASEBUF_PROC_PBUF(i + nHeadLen - 1));  			//i + 12
        if (nDataLen + ASSIST_PROT_HEAD_TAIL_LEN + i > nDataNum)
		{
			pLinkCom->sFrmInfo.sFrmClrInfo.nRcvContinueCnt++;
			if (pLinkCom->sFrmInfo.sFrmClrInfo.nRcvContinueCnt > MAX_RCV_CONTINUE_NUM)
			{
				pLinkCom->sFrmInfo.sFrmClrInfo.nRcvContinueCnt = 0;
                continue;
			}
			else
			{
				break;
			}
		}
		else 
        {
            pLinkCom->sFrmInfo.sFrmClrInfo.nRcvContinueCnt = 0; 
        }
		
		// 3. CS
		nCs = FifoCalcCSBaseBuf(pBaseBuf->pBuf, pBaseBuf->nBufSize, BASEBUF_PROC_PTR(i + 0), nHeadLen + nDataLen);
		if (nCs != BASEBUF_PROC_PBUF(i + nHeadLen + nDataLen))
		{
			continue;
		}
		// 7. 报文尾
		if (0x16 != BASEBUF_PROC_PBUF(i + nHeadLen + nDataLen + 1))
		{
			continue;
		}
#if 1 == HAVE_FRM_BUFF
		if (nHeadLen + 2 + nDataLen > pLinkCom->sFrmBuf.nBufSize)
		{
            continue;
		}
		pLinkCom->nFrmLen = nDataLen + nHeadLen + 2;
		FifoCopyBaseBuf(pLinkCom->sFrmBuf.pBuf, pLinkCom->sFrmBuf.nBufSize, 0, pBaseBuf->pBuf, pBaseBuf->nBufSize, BASEBUF_PROC_PTR(i + 0), pLinkCom->nFrmLen);
#else
        pLinkCom->nFrmLen = nDataLen + nHeadLen + 2;
		pLinkCom->sFrmBuf.pBuf = &pBaseBuf->pBuf[BASEBUF_PROC_PTR(i + 0)];
#endif
		nRtn = 0;
		break;
	}
	
	pBaseBuf->nProcPtr = BASEBUF_PROC_PTR(i + 0);
#if 1 == HAVE_FRM_BUFF	
	pBaseBuf->nProcPtr = BASEBUF_PROC_PTR(pLinkCom->nFrmLen);
#endif		
	return nRtn;
}

/*
*****************************************************************************
* 函数名: AssistProtProc
* 输　入: 
*		pLINK_COM pLinkCom
* 输  出: 
* 返  回: 
*		0成功，-1失败
*       
*       
* 功  能: 有完整帧，返回0成功，否则-1
*****************************************************************************
*/
INT32S AssistProtAsynProc(void* pPara)
{
	INT16U			nHeadLen					= STR_OFF(pASSIST_PROT, nData);
	pASSIST_PROT	pUpRcvLinkProt				= NULL;
	pLINK_COM  	 	pLinkCom                   = (pLINK_COM)pPara;
    pBASE_BUF		pBaseBuf					= NULL;
	INT8U*			pRcvData					= NULL;
	INT32S			nRcvDataLen				= 0;
	INT8U*			pSndData					= NULL;
	INT32S			nSndDataLen				= 0;
	INT16U			nAddr						= 0;
	INT16U			nType						= 0;
	INT16U			nProt						= 0;
	INT8U			nCtrl						= 0;
	INT16U			nPn							= 0;
	INT16U			nDi							= 0;
	INT8U			nSendBuf[500]	            = {0};
	INT32S			nSendLen					= 0;
	//pASSIST_PROT_PROC pAssistProtProc			= NULL;
	
	LINK_COM_VALID ;	
	if (0 == pLinkCom->nFrmLen)
	{
		return 0;
	}
	
	pUpRcvLinkProt = (pASSIST_PROT)(pLinkCom->sFrmBuf.pBuf);
	nAddr = Buf2INT16U(&pUpRcvLinkProt->nAddr[0]);
	nType = Buf2INT16U(&pUpRcvLinkProt->nType[0]);					//暂不使用
	nProt = Buf2INT16U(&pUpRcvLinkProt->nProt[0]);
	nCtrl = pUpRcvLinkProt->nCtrl;
	nPn   = Buf2INT16U(&pUpRcvLinkProt->nPn[0]);
	nDi   = Buf2INT16U(&pUpRcvLinkProt->nDi[0]);
	nRcvDataLen = Buf2INT16U(pUpRcvLinkProt->nDatalen);
	pRcvData    = pUpRcvLinkProt->nData;

	pSndData    = &nSendBuf[0];
	nSndDataLen = sizeof(nSendBuf) - nHeadLen - 2;
	
	//if (ASSIST_RS485 != nAddr)
	//{
	//	goto NEXT_POS;
	//}
	//switch(nProt)
	//{
		//case PROT_BASE:
		//case PROT_DIRECT_RD:
			//pAssistProtProc = AssistProtProcBase;
			//break;
		//case PROT_376_1:
		//case PROT_645:
		//case PROT_FORWARD:
			//pAssistProtProc = AssistProtProcForward;
			//break;
		//default:
			//pAssistProtProc = NULL;
	//}
	//if (NULL == pAssistProtProc)
	{
		//goto NEXT_POS;
	}
	//nSendLen = (*pAssistProtProc)(nType, nProt, nCtrl, nPn, nDi, pRcvData, nRcvDataLen, pSndData, nSndDataLen);
	
	// 组帧发送
	nCtrl |= 0x80;
	nSendLen = AssistMakeFrm(pSndData, nSendLen, sizeof(nSendBuf), nAddr, nType, nProt, nCtrl, nPn, nDi);
	if (nSendLen < 0)
	{
		goto NEXT_POS;
	}
    OSTimeDlyHMSM(0, 0, 0, 200);
	LinkComSend(pLinkCom->nCom, pSndData, (INT32U)nSendLen);
	
NEXT_POS:	
	// 处理结束后，清处理
	LINK_COM_FRAME_CLR ;

	return 0;
}

#endif





