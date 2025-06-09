/******************************************************************************************************
* Copyright (c) 2014
* �ļ���:
* ������: ��һ��
* �ա���: 2014/01/18
* �޸���:
* �ա���:
* �衡��:
* �桡��: Version 1.0
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
                                             ����
***************************************************************************************************/
extern INT32S ReadCommMs(INT8U nCom, INT8U* pBuf, INT32U nNeedRdLen, INT32U nInter, INT32U nDelay);
extern INT32S WriteComm(INT8U nCom, INT8U* pBuf, INT32U nNeedLen);
/*
������Э�������
0x10	1	��ͷ
Addr	2	485���ģ�飺1��
			�ز����ģ�飺2��
			�źŷ��������ߣ�3��
			�ֳ������ߣ�4��
Type	2	I�ͼ�������1��
			II�ͼ�������2��
			I�ɣ�3��
			II�ɣ�4��
			��ƣ�5��
			������0
Prot	2	����Э�飺1��
			376.1Э�飺2��
			��Э�飺3��
			�㳭��4��
			͸��Э�飺5��	
Ctrl	1	�����룺��1��д2�������ϱ�3����ֻ��Ի���Э��ʹ�ã�����Э��ʱΪ0��
			D7λ��ʾ�����У�1Ϊ����
Pn		2	��Ϣ�㣺�߼���
			����0��������Э��ʱΪ0��
			485_1��1��
			485_2��2��
			485_3��3��
Di		2	���ݱ�ʶ����ֻ��Ի���Э�飬����Э��ʱ��Ϊ0��
Len		2	�������ݳ���
Data	x	Len
Cs		1	У��ͣ��ӱ�ͷ���˵���ͣ�
0x16	1	1
*/

/*
*****************************************************************************
* ������: AssistReadComm
* �䡡��: 
* ��  ��: 
* ��  ��: 
*		
*		
* ��  ��: 
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
* ������: AssistWriteComm
* �䡡��: 
* ��  ��: 
* ��  ��: 
*		
*		
* ��  ��: 
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
* ������: AssistMakeFrm
* �䡡��: 
*			INT8U*			pData									������
*			INT32S			nDatalen
*			INT32S			nMaxDatalen
*			INT16U			nAssistAddr								������ַ�����ֲ�ͬ�ĸ���
*			INT16U			nTermType								�ն����ͣ�I�͡�II�ͼ�������
*			INT16U			nProt									����Э�顢376.1��
*			INT8U			nCtrl									������
*			INT16U			nPn										��Ϣ��
*			INT16U			nDi										���ݱ�ʶ
* ��  ��: 
*			INT8U*			pData
* ��  ��: ������Э�鱨��֡����  
* ��  ��: 
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
* ������: AssistProtCheckPackage
* �䡡��: 
*			INT8U*			pData
*			INT32S			nDatalen			���յ��ı��ĳ���
* ��  ��: 
*			INT8U*			pData 				���������֡�����¸��Ƶ� pData 
* ��  ��: ��������֡�ĳ���
* ��  ��: �ڲ���Լ����
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
* ������: AssistProc
* �䡡��: 			
*			INT8U*			pData
*			INT32S			nDatalen			֡���ݳ���
*			INT32S			nMaxDatalen			���յ��ı��ĳ���
* ��  ��: 
*			INT8U*			pData 				����֡
* ��  ��: ��������֡�ĳ���
* ��  ��: �ڲ���Լ����
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
* ������: AssistFrmCheckPackage
* �䡡��: 
*		pLINK_COM pLinkCom
* ��  ��: 
* ��  ��: 
*		0�ɹ���-1ʧ��
*       
*       
* ��  ��: ������֡������0�ɹ�������-1
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
		// 1. ����ͷ
		if (0x10 != BASEBUF_PROC_PBUF(i + 0))
		{
			continue;	
		}
		
		// 2. ����
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
		// 7. ����β
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
* ������: AssistProtProc
* �䡡��: 
*		pLINK_COM pLinkCom
* ��  ��: 
* ��  ��: 
*		0�ɹ���-1ʧ��
*       
*       
* ��  ��: ������֡������0�ɹ�������-1
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
	nType = Buf2INT16U(&pUpRcvLinkProt->nType[0]);					//�ݲ�ʹ��
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
	
	// ��֡����
	nCtrl |= 0x80;
	nSendLen = AssistMakeFrm(pSndData, nSendLen, sizeof(nSendBuf), nAddr, nType, nProt, nCtrl, nPn, nDi);
	if (nSendLen < 0)
	{
		goto NEXT_POS;
	}
    OSTimeDlyHMSM(0, 0, 0, 200);
	LinkComSend(pLinkCom->nCom, pSndData, (INT32U)nSendLen);
	
NEXT_POS:	
	// ����������崦��
	LINK_COM_FRAME_CLR ;

	return 0;
}

#endif





