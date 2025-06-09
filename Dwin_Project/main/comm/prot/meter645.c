/******************************************************************************************************
* Copyright (c) 2013
* 文件名:
* 创建人: 王一凡
* 日　期: 2013/09/12
* 修改人:
* 日　期:
* 描　述:
* 版　本: Version 1.0
*****************************************************************************************************/
#include "Base.h"
#include "meter645.h"
#include "Key.h"
#include "linkcomExt.h"
#include "linkcom.h"
#include "Guimenu.h"
#include "Guiwin.h"
#include "Fun.h"
#include "Ac7022.h"
#include "SysRes.h"
#include "ATT7022.h"
#include "MemDrv.h"
#include "Reset.h"
#include "Version.h"
#include "MemTbl.h"
#include "SoftTimer.h"
#include "SecTask.h"
#include "AutoTest.h"
#include "PowerOn.h"
#include "IdleTask.h"
#include  "Gw698.h"

// 有功电能示值
const  INT8U		g_nReadSelfBuf[] 	= {0x34, 0x32, 0x34, 0x33, // 01 ff 01 00
										   0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 
										   0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00};

INT8U	g_nSuperPwd[3] = {0x66,0x27,0x84};

INT8U g_nAcEventBuf[LEN_AC_EVENT];

INT8U   g_nOneKeyRecFlg     = 0;

INT8U   g_MeterComFaul = 0;
INT8U   g_MeterComFal[2]         ;   // [0]: 1为485通信故障   [1]:1为红外通信故障
INT8U   g_MeterEventWrong[4]     ;   // [0],[1],[2] :A,B,C三相   失压，断相，失流，逆相序模拟标志 [4]：逆相序
INT8U   g_MeterInsideWrong[2]    ;   // 突变，时间显示，数据显示，死机，数据读取，时钟故障，分合闸，主控电池欠压，时钟电池欠压，脉冲灯
INT8U   g_MeterHardwareWrong[2]  ;   //不启动，白屏，黑屏，花屏，A，B，C无电压，A，B，C无电流 ，A，B，C电压异常，A，B，C电流异常。

INT8U   g_SimMeterCoverFlag   = 0;//模拟开盖标志 0xFF开 0xBF 闭
INT8U   g_SimMeterCoverTimeBuf[6]   = {0};//模拟开盖发生（结束）时刻 
INT8U   g_RealMeterCoverFlag   = 0;//实际开盖标志
INT8U   g_SimMeterPasswordFlag = 0;   //密钥更新

INT8U   g_nSetDataFlg = 0; //设置电表冻结数据
INT8U   g_nSetDataRetFlg = 0; 
INT8U   g_nSetCtrlcurFlg = 0; //模拟无电流时特征字正常走字功能
INT8U   g_nSetShortcurFlg = 0; //火线短接
INT8U       g_nAsyFlg = 0;
INT16U		g_nFrmlen		    = 4;

/**************************************************************************************************
                                           相关函数实现
***************************************************************************************************/

#if 0
/*
*****************************************************************************
* 函数名: Meter645Write
* 输　入: 
* 输  出: 
* 返  回: 
*       
*       
* 功  能: 
*****************************************************************************
*/
static INT32S Meter645Write(INT8U nCom, INT8U* pData, INT32S nDatalen)
{
	INT32S nRet = 0;
	
	if ((NULL == pData) || (nDatalen < 0))
	{
		return -1;
	}
	COM_PEND_SEM(nCom);
	nRet = WriteComm(nCom, pData, nDatalen);
	COM_POST_SEM(nCom);
	return nRet;
}

/*
*****************************************************************************
* 函数名: Meter645Read
* 输　入: 
* 输  出: 
* 返  回: 
*       
*       
* 功  能: 
*****************************************************************************
*/
static INT32S Meter645Read(INT8U nCom, INT8U* pData, INT32S nMaxDatalen, INT32U iInterval, INT32U ms)
{
	INT32S nRet = 0;
	
	if ((NULL == pData) || (nMaxDatalen < 0))
	{
		return -1;
	}
	COM_PEND_SEM(nCom);
	nRet = ReadCommMs(nCom, pData, nMaxDatalen, iInterval, ms);
	COM_POST_SEM(nCom);
	return nRet;
}
#endif

/*
*****************************************************************************
* 函数名: Meter645MakeFrm
* 输　入: 
*		pLINK_COM pLinkCom
* 输  出: 
* 返  回: 
*		
*       
*       
* 功  能: 组帧  返回组帧长度
*****************************************************************************
*/
INT32S Meter645MakeFrm(INT8U* pData, INT32S nDataLen, INT32S nDataBufSize, INT8U* pMeterAddr, INT8U nCtrl)
{
	INT16U				nHeadLen		= STR_OFF(pDB_LINK_PROT, nData);
	pDB_LINK_PROT		pDbLinkProt 	= (pDB_LINK_PROT)pData;
	INT32U				nSendLen 		= 0;

	if ((NULL == pData) || (nDataLen < 0))
	{
		return -1;
	}
	if (nHeadLen + nDataLen + 2 > nDataBufSize)
	{
		return -1;
	}
	
	memmove(&pDbLinkProt->nData[0], pData, nDataLen);
	if (NULL == pMeterAddr)
	{
		memset(&pDbLinkProt->nAddr[0], 0x98, 6);
	}
	else
	{
		memcpy(&pDbLinkProt->nAddr[0], pMeterAddr, 6);
	}
	pDbLinkProt->nHead1 = 0x68;
	pDbLinkProt->nHead2 = 0x68;
	pDbLinkProt->nCtrl  = nCtrl;
	pDbLinkProt->nDatalen[0] = (INT8U)nDataLen;
	nSendLen = nHeadLen + nDataLen;
	pData[nSendLen] = CheckSum(pData, 0, nSendLen);
	nSendLen++;
	pData[nSendLen++] = 0x16;
	
	return nSendLen;
}

/**************************************************************************************************
                                           调用接口
***************************************************************************************************/
/*
*****************************************************************************
* 函数名: Meter645CheckPackage
* 输　入: 
*		INT8U* 				pAddr					表地址
*		INT8U* 				pBuf					接收的缓冲区
*		INT16U 				nBufsize				缓冲区大小
* 输  出: 
*		INT8U* 				pBuf					接收的缓冲区
* 返  回: 
*		0成功，-1失败
*       
*       
* 功  能: 普通缓冲区(非循环缓冲)寻找645帧: 有完整帧，返回0成功，否则-1
*****************************************************************************
*/
INT32S Meter645CheckPackage(INT8U* pAddr, INT8U* pBuf, INT16U nBufsize)
{
	INT16U		nHeadLen		= STR_OFF(pDB_LINK_PROT, nData);
	INT16U 		i        		= 0;
    INT8U* 		pData    		= pBuf;
    INT8U   	nCs     		= 0;
	INT16U		nDataLen		= 0;
	
    if ((NULL == pBuf))
    {
        return -1;
    }
    if (nBufsize < nHeadLen + 2)
    {
        return -1;
    }

    for (i = 0; i < nBufsize - (nHeadLen + 2 - 1); i++)
    {
		pData = &pBuf[i];							// 
		// 1. 报文头
		if (pData[0] != 0x68)
        {
            continue;
        }
		// 2. 6个字节地址
		if (NULL != pAddr)
		{
	        if (0 != memcmp(pAddr, &pData[1], 6))
	        {
	            continue;
	        }
		}
		// 3. 第二个68
        if (pData[7] != 0x68)
        {
            continue;
        }
		// 4. 控制码

		// 5. 数据域长度
        nDataLen = pData[9];
        if (nDataLen > nBufsize - nHeadLen - 2 - i)
        {
            continue;
        }

		// 6. CS
        nCs = CheckSum(&pBuf[0], i, nDataLen + nHeadLen);
        if (nCs != pData[nDataLen + nHeadLen])
        {
            continue;
        }

		// 7. 报文尾
        if (0x16 != pData[nDataLen + nHeadLen + 1])
        {
            continue;
        }

		nDataLen = nDataLen + nHeadLen + 2;
		memmove(&pBuf[0], &pData[0], nDataLen);
        return (INT32S)nDataLen;
    }

    return -1;
}

/*
*****************************************************************************
* 函数名: Meter645Proc
* 输　入: 

* 输  出: 
* 返  回: 
*		
*       
*       
* 功  能: 同步
*****************************************************************************
*/
INT32S Meter645Proc(INT8U nCom, INT8U* pData, INT32S nDatalen, INT32S nMaxDatalen)
{
    //INT8U			nCom			= 0;
    INT8U			nCnt			= 0;
	INT32S			nTmp			= 0;
	INT32S			nTimeOut		= 2000;
	pDB_LINK_PROT   pDbLinkProt     = (pDB_LINK_PROT)pData;
	
	if ((NULL == pData) || (nDatalen < 0) || (nMaxDatalen <= 0))
	{
		return -1;
	}

	nCom = ProcCommCfgPend(nCom);
	WriteComm((INT8U)nCom, pData, nDatalen);

	memset(pData, 0, nMaxDatalen);
	nDatalen = 0;
	while(1)
	{
		if (nTimeOut < 0)
		{
			nTmp = -1;
			break;
		}
		nTmp = 0;
        nCnt++;
       
		nTmp = ReadCommMs(nCom, &pData[nDatalen], nMaxDatalen - nDatalen, 50, 200);
		if (nTmp > 0)
		{
			nDatalen += nTmp;
		}
		nTimeOut -= 100;

		if (nDatalen >= METER_645_BASE_LEN)
		{
			nTmp = Meter645CheckPackage(NULL, pData, nDatalen);
			if (nTmp > 0)
			{
                PrintCommHex( pData, nDatalen);
                if((pDbLinkProt->nCtrl & 0x80) != 0x80)
                {
                    nTmp = -1;
					break;
                }
                if((pDbLinkProt->nCtrl & 0x40) == 0x40)
                {
                    nTmp = -1;
					break;
                }
				nDatalen = nTmp;
				break;
			}
		}
	}
	ProcCommCfgPost(nCom);
	return nTmp;
}

/*
*****************************************************************************
* 函数名: Meter645AsynCheckPackage
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
INT32S Meter645AsynCheckPackage(void* pPara)
{
	INT16U		nHeadLen		= STR_OFF(pDB_LINK_PROT, nData);
	pLINK_COM   pLinkCom    	= (pLINK_COM)pPara;
    pBASE_BUF	pBaseBuf		= NULL;
	INT32U		i				= 0;
    INT32U       ii              = 0;
	INT8S		nRtn			= -1;
	INT16U		nDataLen		= 0;
    INT32U      nDataNum    	= 0;
    INT8U       nCs         	= 0;
    //INT8U       nCtrl        	= 0;
	INT8U		nAddr[6]		= {0};
    INT8U		nAddr2[6]		= {0};
    INT8U		nMemAddr[6]		= {1};
    INT8U       nHeadlHFS;
    INT8U       nHeadhHFS;
    INT8U       nlHFS;
    INT8U       nhHFS;
    INT8U       nAddrLen = 0;
    INT16U		nHeadLenTmp		= 0;
    
	LINK_COM_VALID ;
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
	nDataNum = BASEBUF_BYTE_NUM(pBaseBuf);
	if (nDataNum < nHeadLen + 2)
	{
		return -1;
	}
    
	if (pLinkCom->nFrmLen > 0)
	{
		return 0;
	}
    
    if (0x01 == pLinkCom->nCom)
    {
       memcpy(&nMemAddr[0], &g_nMemProductAddr[0], 6);//交采地址
    }
    else
    {
       memcpy(&nMemAddr[0], &g_nMemAddr[0], 6);//交采地址
    }
    
    for (i = 0; i < nDataNum - (nHeadLen + 2 - 1); i++)
    {		
        // 1. 报文头
        if (0x68 != BASEBUF_PROC_PBUF(i + 0))
        {
            continue;	
        }
        // 2. 6个字节地址
        memset(&nAddr, 0, sizeof(nAddr));
        FifoCopyBaseBuf(&nAddr[0], sizeof(nAddr), 0, pBaseBuf->pBuf, pBaseBuf->nBufSize, BASEBUF_PROC_PTR(i + 1), sizeof(nAddr));
        //判断接受698报文地址与电表地址是否相等
        FifoCopyBaseBuf(&nAddr2[0], sizeof(nAddr2), 0, pBaseBuf->pBuf, pBaseBuf->nBufSize, BASEBUF_PROC_PTR(i + 5), sizeof(nAddr2));
        
        // 地址是电表地址 或广播地址   同时有第二个 68才判定是 645
        if ( (0x68 == BASEBUF_PROC_PBUF(i + 7)) && ( (0 == memcmp(&nMemAddr[0], &nAddr[0], sizeof(nAddr))) || (TRUE == IsAllEqualVal(&nAddr[0], 6, 0xAA)) 
            || (TRUE == IsAllEqualVal(&nAddr[0], 6, 0x99)) || (TRUE == IsAllEqualVal(&nAddr[0], 6, 0x98))||(TRUE ==IsAddrFieldVal(&nAddr[0], 6)) ))
        {
            // 5. 数据域长度
            if(0x80 == (BASEBUF_PROC_PBUF(i + 8)&0x80))
            {
                pLinkCom->sBaseBuf.nRecvPtr = 0;
                pLinkCom->sBaseBuf.nProcPtr = 0;
                pLinkCom->nFrmLen = 0;
                return -1;
            }
            nDataLen = BASEBUF_PROC_PBUF(i + 9);
            if (i + nHeadLen + 2 + nDataLen  > nDataNum)
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
            
            // 6. CS
            nCs = FifoCalcCSBaseBuf(pBaseBuf->pBuf, pBaseBuf->nBufSize, BASEBUF_PROC_PTR(i + 0), 10 + nDataLen);
            if (nCs != BASEBUF_PROC_PBUF(i + 10 + nDataLen))
            {
                pLinkCom->sBaseBuf.nRecvPtr = 0;
                pLinkCom->sBaseBuf.nProcPtr = 0;
                pLinkCom->nFrmLen = 0;
                return -1;
                //continue;
            }
            // 7. 报文尾
            if (0x16 != BASEBUF_PROC_PBUF(i + 10 + nDataLen + 1))
            {
                pLinkCom->sBaseBuf.nRecvPtr = 0;
                pLinkCom->sBaseBuf.nProcPtr = 0;
                pLinkCom->nFrmLen = 0;
                return -1;
                //continue;
            }
            g_nAsyFlg  = 0;
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
        else if(((0x43 == BASEBUF_PROC_PBUF(i+3))&&((BASEBUF_PROC_PBUF(i+4) & 0x0F)==0x05)&&((0 == memcmp(&nMemAddr[0], &BASEBUF_PROC_PBUF(i+5), 6))))||((TRUE ==IsAddrFieldVal(&nAddr2[0], 6))&&((BASEBUF_PROC_PBUF(i+4) & 0x40)==0x40))||(TRUE == IsAllEqualVal(&nAddr2[0], 6,0xAA))
                ||(TRUE == IsAllEqualVal(&nAddr2[0], 6, 0x99)))
        {
            //memcpy(&nMemAddr[0], &g_nMem698Addr[1], 6);
            // g_nFrmlen 存放如果是698 则该报文的 长度
            g_nFrmlen = MAKEWORD(BASEBUF_PROC_PBUF(i + 1), BASEBUF_PROC_PBUF(i + 2));
            if (g_nFrmlen + 2 + 1 > pBaseBuf->nBufSize)//   pBaseBuf->nBufSize  == 2148
            { 
                pLinkCom->sBaseBuf.nRecvPtr = 0;
                pLinkCom->sBaseBuf.nProcPtr = 0;
                pLinkCom->nFrmLen = 0;
                return -1;
                //continue;	
            }
            else if (g_nFrmlen + 2 > nDataNum - i )
            {      //下面这层判断目的是， 防止 g_nFrmlen 超过最大空间
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
            else if (g_nFrmlen < 8)
            {
                continue;
            }
            else
            {
                pLinkCom->sFrmInfo.sFrmClrInfo.nRcvContinueCnt = 0;    
            }
            //nAddrLen = BASEBUF_PROC_PBUF(i+4);// + 2;
            nAddrLen = (BASEBUF_PROC_PBUF(i+4) & 0x0F) + 1;  //低四个字节+1 代表地址长度
            nHeadLenTmp = 3+1+nAddrLen + 1;// 3: 头  1: SA1  nAddrLen.SA地址 1CA   nHeadLenTmp==帧头长度
            //帧头校验
            getCrcProc(&BASEBUF_PROC_PBUF(i+1), &nHeadlHFS, &nHeadhHFS, nHeadLenTmp);
            
            if (nHeadlHFS != BASEBUF_PROC_PBUF(i + nHeadLenTmp+1))
            {
                pLinkCom->sBaseBuf.nRecvPtr = 0;
                pLinkCom->sBaseBuf.nProcPtr = 0;
                pLinkCom->nFrmLen = 0;
                return -1;
                //continue;	
            }
            
            if (nHeadhHFS != BASEBUF_PROC_PBUF(i + nHeadLenTmp+2))
            {
                pLinkCom->sBaseBuf.nRecvPtr = 0;
                pLinkCom->sBaseBuf.nProcPtr = 0;
                pLinkCom->nFrmLen = 0;
                return -1;
                //continue;	
            }
            
            if (0x43 != BASEBUF_PROC_PBUF(i+3))//传输方向
            {
                pLinkCom->sBaseBuf.nRecvPtr = 0;
                pLinkCom->sBaseBuf.nProcPtr = 0;
                pLinkCom->nFrmLen = 0;
                return -1;
                 //continue;
            }
            
            if (0 != memcmp(&nMemAddr[0], &BASEBUF_PROC_PBUF(i+5), nAddrLen))
            {
                FifoCopyBaseBuf(&nAddr[0], sizeof(nAddr), 0, pBaseBuf->pBuf, pBaseBuf->nBufSize, BASEBUF_PROC_PTR(i + 5), sizeof(nAddr));
                if ((TRUE != IsAllEqualVal(&nAddr[0], 6, 0x99))&&(TRUE != IsAllEqualVal(&nAddr[0], 6, 0xAA))&&(TRUE !=IsAddrFieldVal(&nAddr[0], 6)))                             //广播地址
                {
                    pLinkCom->sBaseBuf.nRecvPtr = 0;
                    pLinkCom->sBaseBuf.nProcPtr = 0;
                    pLinkCom->nFrmLen = 0;
                    return -1;
                     //continue;
                }
               
            }
            //帧尾校验
            getCrcProc(&BASEBUF_PROC_PBUF(i+1), &nlHFS, &nhHFS, g_nFrmlen-2);
            
            if (nlHFS != BASEBUF_PROC_PBUF(i + g_nFrmlen-1))
            {
                pLinkCom->sBaseBuf.nRecvPtr = 0;
                pLinkCom->sBaseBuf.nProcPtr = 0;
                pLinkCom->nFrmLen = 0;
                return -1;
                //continue;	
            }
            
            if (nhHFS != BASEBUF_PROC_PBUF(i + g_nFrmlen))
            {
                pLinkCom->sBaseBuf.nRecvPtr = 0;
                pLinkCom->sBaseBuf.nProcPtr = 0;
                pLinkCom->nFrmLen = 0;
                return -1;
                //continue;	
            }
            // 7. 报文尾
            if (0x16 != BASEBUF_PROC_PBUF(i + g_nFrmlen+1))
            {
                pLinkCom->sBaseBuf.nRecvPtr = 0;
                pLinkCom->sBaseBuf.nProcPtr = 0;
                pLinkCom->nFrmLen = 0;
                return -1;
                //continue;	
            }
            g_nAsyFlg  = 1;
    #if 1 == HAVE_FRM_BUFF
            if (nFrmlen1 + 2 > pLinkCom->sFrmBuf.nBufSize)
            {
                continue;
            }
            pLinkCom->nFrmLen = g_nFrmlen + 2;
            FifoCopyBaseBuf(pLinkCom->sFrmBuf.pBuf, pLinkCom->sFrmBuf.nBufSize, 0, pBaseBuf->pBuf, pBaseBuf->nBufSize, BASEBUF_PROC_PTR(i + 0), pLinkCom->nFrmLen);
    #else
            pLinkCom->nFrmLen = g_nFrmlen + 2;
            pLinkCom->sFrmBuf.pBuf = &pBaseBuf->pBuf[BASEBUF_PROC_PTR(i + 0)];
            //g_nAsynSeq = pBaseBuf->pBuf[BASEBUF_PROC_PTR(i + 13)] & 0x0F;
    #endif
            //FifoCopyBaseBuf(pBaseBuf->pBuf, pBaseBuf->nBufSize, BASEBUF_PROC_PTR(i + 7), &nTermAreaAddr[0], sizeof(nTermAreaAddr), 0, 4);
            
            nRtn = 0;
            break;
        }
    }
          
    pBaseBuf->nProcPtr = BASEBUF_PROC_PTR(i + 0);
#if 1 == HAVE_FRM_BUFF	
    pBaseBuf->nProcPtr = BASEBUF_PROC_PTR(pLinkCom->nFrmLen);
#endif	
    return nRtn;
}


/********************************************************************************
* 函数名:DltJCMakeFrame
* 输　入:INT8U* pAddr, INT8U* Data, INT16U Length
* 输　出:INT8U* senddata
* 返  回:发送帧长度
* 功  能:组发送帧
*********************************************************************************/
INT8U DltJCMakeFrame(const INT8U* pAddr, INT8U* pData, INT16U nLength, INT8U* pSenddata)
{
    INT16U	i = 0, j = 0;
	INT16U	nIndex = 0;

	pSenddata[nIndex++] = 0xFE;
	pSenddata[nIndex++] = 0xFE;
	pSenddata[nIndex++] = 0xFE;
	pSenddata[nIndex++] = 0xFE;
	j = nIndex;
	pSenddata[nIndex++] = 0x68;							/*0x68*/
	for(i=0; i<6; i++)
	{
		pSenddata[nIndex++] = *pAddr++;					/*填写表地址*/
	}
	pSenddata[nIndex++] = 0x68;							/*0x68*/
	pSenddata[nIndex++] = pData[0];
	pSenddata[nIndex++] = pData[1];
	for(i=0; i<(nLength-2); i++)						/*填写数据体*/
	{
		pSenddata[nIndex++] = pData[2+i]+0x33;
	}
	pSenddata[nIndex] = CheckSum(pSenddata, j, nIndex-j);
	nIndex++;											/*效验码*/
	pSenddata[nIndex++] = 0x16;							/*0x16*/
	return nIndex;
}

/*
*****************************************************************************
* 函数名: Meter645AsynProc
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
INT32S Meter645AsynProc(void* pPara)
{
	pDB_LINK_PROT	pUpRcvLinkProt				= NULL;
	pLINK_COM  	 	pLinkCom                    = (pLINK_COM)pPara;
    pGW698_PROT	    pUpRcvLinkProt_GW698				= NULL;
    pBASE_BUF		pBaseBuf					= NULL;
	INT32S			nDataLen				    = 0;
    INT8U           nDataBuf[300]                ={0};
	static INT8U	        nSndDataBuf[400]	         ={0};//400  
	//INT8U*			pSndData					= &g_nGw3761SendBuf[0];
    INT8U*			pSndData					= &nSndDataBuf[0];
	INT32S			nSendLen					= 0;
	INT32S			nRet						= 0;
    INT8U			nErr				        = 0;
    INT8U*			pRcvData					= NULL;
	INT32S			nRcvDataLen				    = 0;	
    
    INT8U	        nDltAddr[LEN_METER_ADDR]    = {1};
    
	LINK_COM_VALID ;

	if (0 == pLinkCom->nFrmLen)
	{
		return 0;
	}
    
    if ((g_MeterPowerOff) && ((COMM_ZB == pLinkCom->nCom)||(pLinkCom->nCom==COMM_IR)))
    {
        LINK_COM_FRAME_CLR
        return 0;
    }
    
    if ((pLinkCom->nCom==COMM_WH485)&&(1 == g_MeterComFaul)) //485故障模拟
    {
        g_nFlg_Commu = 0;
        LINK_COM_FRAME_CLR ;   //需加一个485故障恢复
        return 0;
    }
    else if ((pLinkCom->nCom==COMM_ZB)&&(1 == g_ZBPortFault))
    {
        g_nFlg_Commu = 0;
        LINK_COM_FRAME_CLR ;
        return 0;
    } 
    else if((pLinkCom->nCom==COMM_IR)&&(g_MeterComFal[1]==1)) //红外故障模拟
    {
        g_nFlg_Commu2 = 1;
        LINK_COM_FRAME_CLR ;
        return 0;
    }
    
    g_nFlg_Commu = 1;
    if(0x06 == pLinkCom->nCom)         //抄表4851
    {
      g_nFlg_Commu = 2;
    }
    else if(0x01 == pLinkCom->nCom)
    {
      g_nFlg_Commu = 3;
    }
     else if(0x04 == pLinkCom->nCom)
    {
      g_nFlg_Commu = 4;
    }

    //OSSemPend(g_pSem[SEM_GW3761_BUF_IDX], 0, &nErr);
	 if (g_nAsyFlg)
    {
        pUpRcvLinkProt_GW698 	= (pGW698_PROT)(pLinkCom->sFrmBuf.pBuf);//存放帧头和数据
        pRcvData		= &(pUpRcvLinkProt_GW698->nDadtData[0]); //数据域
        nRcvDataLen     = Buf2INT16U(&pUpRcvLinkProt_GW698->nDataLen[0]) + 2;//报文总长度
        pSndData 		= &g_nGw3761SendBuf[0];  // GW3761_SENDBUF_SIZE==1024字节大小的buf
        memset(pSndData, 0, GW3761_SENDBUF_SIZE);
        memset(g_nGw3761SendBuf, 0, GW3761_SENDBUF_SIZE);
    
        //打印  传参： 1.帧头+数据  2.发送的buf  3.收到的数据长度
        nSendLen = Gw698AsynProcSub(&pLinkCom->sFrmBuf.pBuf[0], pSndData, nRcvDataLen);
    }
    else
    {//645抄表部分
      pUpRcvLinkProt = (pDB_LINK_PROT)(pLinkCom->sFrmBuf.pBuf);
      //LinkComSend(COMM_WH4852, pLinkCom->sFrmBuf.pBuf,pLinkCom->nFrmLen);
      if (pUpRcvLinkProt->nCtrl > 0x1b)      
      {        
          if((0x1c == pUpRcvLinkProt->nCtrl) && (0x1C == pLinkCom->nFrmLen))      
          {
             nDataLen = Dlt07Analyse((INT8U*)pUpRcvLinkProt, pLinkCom->nFrmLen, &nDataBuf[0]); 
          }
          else      //校表部分
          {
              if(g_MeterPowerOff)
              {
                    goto END;
              }
              nDataLen = DltJBAnalyse((INT8U*)pUpRcvLinkProt, pLinkCom->nFrmLen, &nDataBuf[0]);
          }
      }
      
      else if(pUpRcvLinkProt->nCtrl > 0x10)	/*07部分*/  
      {
          nDataLen = Dlt07Analyse((INT8U*)pUpRcvLinkProt, pLinkCom->nFrmLen, &nDataBuf[0]); 
      }
      else if(pUpRcvLinkProt->nCtrl < 0x11) /*97部分*/  
      {
          if(g_MeterPowerOff)
              {
                    goto END;
              }
          nDataLen = Dlt97Analyse((INT8U*)pUpRcvLinkProt, pLinkCom->nFrmLen, &nDataBuf[0]);
      }
          
      if (nDataLen > 0)
      {
          //MemReadAcPrm(F19, &nDltAddr[0], 6);  //读取交采地址，转成内存
          memcpy(&nDltAddr[0], &g_nMemAddr, 6);
      }
      
      nSendLen = DltJCMakeFrame(&nDltAddr[0], &nDataBuf[0], nDataLen, pSndData);	
    }
 //发送
    if (nSendLen > 0)
    {
        if (!g_nOneKeyRecFlg) 
        {
            if(pLinkCom->nCom == COMM_WH485)
            {
                LinkComSend(pLinkCom->nCom, pSndData, (INT32U)nSendLen);
                g_nFlg_Commu  = 2;
            }
            else if(pLinkCom->nCom == COMM_WH4852)
            {
                LinkComSend(pLinkCom->nCom, pSndData, (INT32U)nSendLen);
                g_nFlg_Commu  = 3;
            }
            else if(pLinkCom->nCom==COMM_IR)
            {
                //OSTimeDlyHMSM(0,0,0,200);
                LinkComSend(pLinkCom->nCom, pSndData, (INT32U)nSendLen); 
                g_nFlg_Commu  = 2;
            }
            else if(pLinkCom->nCom==COMM_ZB)
            {
                LinkComSend(pLinkCom->nCom, pSndData, (INT32U)nSendLen); 
                g_nFlg_Commu  = 4;
            }
                   
        }
        //LinkComSend(COMM_WH4852, pSndData, (INT32U)nSendLen);
        g_nOneKeyRecFlg = 0;
        
        if(1 == g_nFlgProt)
        {
            g_nFlgProt1 = 1;
        }
        g_nFlg_Commu2 = 1;
    }
    
  END:
	// 处理结束后，清处理
	LINK_COM_FRAME_CLR ;
	
	return nRet;
}



/********************************************************************************
* 函数名:DltJBAnalyse
* 输　入:
* 输　出:
* 返  回:
* 功  能: 校表规约解析
*********************************************************************************/
INT8U DltJBAnalyse(const INT8U *pValidData, INT8U nValidLen, INT8U *pSendData)
{
    INT8U	nCtrlCode = 0;
	INT8U	nSendLen = 0;
    
	nCtrlCode = (pValidData[8]&0x1f);
	switch(nCtrlCode)
	{
		case 0x1c:				/*瞬时数据集合*/
			nSendLen = DltJBCode1c(pValidData, nValidLen, pSendData);
			break;
		case 0x1d:				/*校表系数回零*/
			nSendLen = DltJBCode1d(pValidData, nValidLen, pSendData);
			break;
		case 0x1e:				/*自动校表*/
			nSendLen = DltJBCode1e(pValidData, nValidLen, pSendData);
			break;
        case 0x1f:				/*手动校表*/
			nSendLen = DltJBCode1f(pValidData, nValidLen, pSendData);
			break;
		default:
			nSendLen = 0;
			break;
	}
	return nSendLen;
}


/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 校表招测瞬时数据解析组侦
*********************************************************************************/
INT8U DltJBCode1c(const INT8U *pValidBuf, INT8U nValidLen, INT8U *pSendBuf)
{
    BOOLEAN	bRet        = FALSE;
	INT8S	nSuccflag   = RET_ERR_ERROR;
	INT8U	nErr        = 0;
	INT8U	i;
	INT8U	nData[20];
	INT8U	nCtrlCode   = 0;
	INT8U	nDI01[2];
	INT8U	nIndex      = 0;
	INT16U	nDataLen    = 0;
	INT16U	nDesLen     = 0;
	INT16U	nStart      = 0;
	INT32U	nIstartup   = 0;
	INT32U	nTmpBuff[9];
	INT32U	nTmp;
	FP32	fValue      = 0.0;
    
    memset(nData,0,sizeof(nData));
	memset(nDI01,0,sizeof(nDI01));
	memset(nTmpBuff,0,sizeof(nTmpBuff));
    
    nCtrlCode   = pValidBuf[8];
	nDataLen    = pValidBuf[9];
	if(nDataLen < 2 || nDataLen > (nValidLen-12))
	{
		bRet = FALSE;
	}
    else
    {
        for(i = 0; i < nDataLen; i++)
		{
			nData[i] = pValidBuf[10 + i] - 0x33;
		}
		nDI01[0]=nData[0];
		nDI01[1]=nData[1];
        switch (((nDI01[1]<<8)|nDI01[0]))
        {
			case 0xb7ff:
                OSSemPend(g_pSem[SEM_ACREAD_CTRL_IDX], 0, &nErr);
                nTmpBuff[nIndex++] = ATT7022RegRead(r_UaRms);	//A相电压
				nTmpBuff[nIndex++] = ATT7022RegRead(r_UbRms);	//B相电压
				nTmpBuff[nIndex++] = ATT7022RegRead(r_UcRms);	//C相电压
				nTmpBuff[nIndex++] = ATT7022RegRead(r_IaRms);	//A相电流
				nTmpBuff[nIndex++] = ATT7022RegRead(r_IbRms);	//B相电流
				nTmpBuff[nIndex++] = ATT7022RegRead(r_IcRms);	//C相电流
				nTmpBuff[nIndex++] = ATT7022RegRead(r_Pa);	//A相有功功率
				nTmpBuff[nIndex++] = ATT7022RegRead(r_Pb);	//B相有功功率
				nTmpBuff[nIndex++] = ATT7022RegRead(r_Pc);	//C相有功功率
				OSSemPost(g_pSem[SEM_ACREAD_CTRL_IDX]);
                
                nIndex = 2;
				pSendBuf[nIndex++] = nDI01[0];
				pSendBuf[nIndex++] = nDI01[1];
                
                for (i=0; i<3; i++)
                {
					fValue = AcCalcReadValid(pDataTable->nPulseConst, pDataTable->nHFConst, r_UaRms+i, nTmpBuff[i]);
                    nTmp = (INT32U)(fValue*1000);
                    pSendBuf[nIndex+i*3] = (nTmp%100)&0xFF;
					nTmp /= 100;
					pSendBuf[nIndex+i*3+1] = (nTmp%100)&0xFF;
					pSendBuf[nIndex+i*3+2] = (nTmp/100)&0xFF;
                }
                nIndex += 9;
                for (i=0; i<3; i++)
				{
					fValue = AcCalcReadValid(pDataTable->nPulseConst, pDataTable->nHFConst, r_IaRms+i, nTmpBuff[i+3]);
					nTmp = (INT32U)((fValue/40.0)*10000);
					pSendBuf[nIndex+i*3] = (nTmp%100)&0xFF;
					nTmp /= 100;
					pSendBuf[nIndex+i*3+1] = (nTmp%100)&0xFF;
					pSendBuf[nIndex+i*3+2] = (nTmp/100)&0xFF;
				}
                nIndex += 9;
                for (i=0; i<3; i++)
				{
					fValue = AcCalcReadValid(pDataTable->nPulseConst, pDataTable->nHFConst, r_Pa+i, nTmpBuff[i+6]);
					//nTmp = (INT32U)(fValue*1000000);
					nTmp = (INT32U)(fValue*998500);
					pSendBuf[nIndex+i*4] = (nTmp%100)&0xFF;
					nTmp /= 100;
					pSendBuf[nIndex+i*4+1] = (nTmp%100)&0xFF;
					nTmp /= 100;
					pSendBuf[nIndex+i*4+2] = (nTmp%100)&0xFF;
					pSendBuf[nIndex+i*4+3] = (nTmp/100)&0xFF;
				}
                nIndex += 12;
                for (i=4; i<nIndex; i++)
				{
					pSendBuf[i] = INT8U_TO_BCD(pSendBuf[i]);
				}
                nDesLen = nIndex - 2;
				bRet = TRUE;
				break;
				
			case 0xC035:
				//nSuccflag = MemReadACCalPrm(MEM_OBJ_AC_CAL_PRM, 0, AC_CAL_ISTARUP_ID, &nIstartup, sizeof(nIstartup));
				if (nSuccflag != RET_ERR_OK)
				{
					bRet = FALSE;
            }
				else
				{
					nIndex = 2;
					pSendBuf[nIndex++] = nDI01[0];
					pSendBuf[nIndex++] = nDI01[1];
					
					nStart = (INT16U)(((((FP32)nIstartup) / 39.3216) * 10 + 5) / 10);

					pSendBuf[nIndex++] = INT8U_TO_BCD((INT8U)(nStart%100));
					pSendBuf[nIndex++] = INT8U_TO_BCD((INT8U)(nStart/100));
					nDesLen = nIndex - 2;
					bRet = TRUE;
				}
				break;
				
            default:
                bRet = FALSE;
				break;   
            }
    }
    
    if(bRet == FALSE)
	{
		pSendBuf[0] = nCtrlCode | 0xC0;
		pSendBuf[1] = 1;
	}
	else
	{
		pSendBuf[0] = nCtrlCode | 0x80;
		pSendBuf[1] = nDesLen;
	}
	return (pSendBuf[1]+2);
}


/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 校表参数清零解析
*********************************************************************************/
INT8U DltJBCode1d(const INT8U *pValidBuf, INT8U nValidLen, INT8U *pSendBuf)
{
    INT8U	i;
	INT8S	nSuccflag = RET_ERR_ERROR;
	INT8U	nTmp = 0;
	INT8U	nData[20];
	INT16U	nDataLen=0;
	INT8U	nCtrlCode=0;
	INT8U	nIndex=0;
	INT8U	nPwd[3];
	INT8U	nRight=0xFF;
    INT8U   nErr = 0;
	MEM_AC_CAL_PRM sJB_Param;
    
    memset(nData, 0, sizeof(nData));
	memset(nPwd, 0, 3);
	memset(&sJB_Param, 0, sizeof(MEM_AC_CAL_PRM));
    
    nCtrlCode = pValidBuf[8];
	nDataLen = pValidBuf[9];
	if(nDataLen < 2 || nDataLen>(nValidLen-12))
	{
		nTmp = 0x01;
	}
    else
    {
        for(i=0; i<nDataLen; i++)
		{
			nData[i] = pValidBuf[10 + i] - 0x33;
		}
        nRight=nData[nIndex++];
        if(nRight != 0)									/*0级密码均可设置*/
		{
			nTmp = 0x04;
		}
        else
        {
            memcpy(nPwd,nData+nIndex,3);				/*密码判断*/
			nIndex += 3;
			if(memcmp(nPwd,g_nSuperPwd,3) != 0)
			{
				nTmp = 0x04;
			}
            else
            {
                OSSemPend(g_pSem[SEM_AC_METER_CALC_IDX], 0, &nErr);
                memset(&sJB_Param, 0, sizeof(MEM_AC_CAL_PRM));
                if (RET_ERR_OK == AcCalcJbParamInit(&sJB_Param))
                {
                    if (MemWriteAcCalPrm(FEND, &sJB_Param, sizeof(sJB_Param)) >= 0)
                    {
                        nSuccflag = RET_ERR_OK;
                    }
                    if(nSuccflag == RET_ERR_OK)
                    {
                        //if (RET_ERR_OK != AcCalcJust7022())
                        if (RET_ERR_OK != AcCalcJust7022(&sJB_Param))
                        {
                            nTmp = 0x01;
                        }
                    }
                    else
                    {
                        nTmp = 0x01;
                    }
                }
                else
				{
					nTmp = 0x80;
				}
    
                OSSemPost(g_pSem[SEM_AC_METER_CALC_IDX]);
            }
        }
    }
    
    if(nTmp != 0)
	{
		pSendBuf[0] = nCtrlCode | 0xC0;
		pSendBuf[1] = 1;
		pSendBuf[2] = nTmp;
	}
	else
	{
		pSendBuf[0] = nCtrlCode | 0x80;
		pSendBuf[1] = 0;
	}
	return (pSendBuf[1] + 2);
}


/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能:自动校表
*********************************************************************************/
INT8U DltJBCode1e(const INT8U *pValidBuf, INT8U nValidLen, INT8U *pSendBuf)
{
    INT8U	nTmp    = 0;
    INT8U	nErr    = 0;
	INT8S	nRetFlag = 0;
	INT8U	i       = 0;
	INT8U	nData[50];
	INT16U	nDataLen=0;
	INT8U	nCtrlCode=0;
	INT8U	nDI01[2];
	INT8U	nIndex=0;
	INT8U	nPwd[3];
	INT8U	nRight=0xFF;

	memset(nData, 0, sizeof(nData));
	memset(nDI01, 0, sizeof(nDI01));
	memset(nPwd, 0, 3);

	nCtrlCode = pValidBuf[8];
	nDataLen = pValidBuf[9];
	if(nDataLen < 2 || nDataLen>(nValidLen-12))
	{
		nTmp = 0x01;
	}
    else
    {
        for(i = 0; i < nDataLen; i++)
		{
			nData[i] = pValidBuf[10 + i] - 0x33;
		}
		nDI01[0]=nData[nIndex++];
		nDI01[1]=nData[nIndex++];
        nRight=nData[nIndex++];
		if(nRight != 0)									/*0级密码均可设置*/
		{
			nTmp = 0x04;
		}
        else
        {
        memcpy(nPwd,nData+nIndex,3);				/*密码判断*/
        nIndex += 3;
        if(memcmp(nPwd,g_nSuperPwd,3) != 0)
        {
            nTmp = 0x04;
        }
        else
        {
            nRetFlag = DltJBParamSupported(nDI01, nDataLen - 6);	/*减掉数据标示和密码共6字节*/ 
            if(nRetFlag == RET_ERR_OK)						/*计量支持的参数*/
            {
                OSSemPend(g_pSem[SEM_AC_METER_CALC_IDX], 0, &nErr);
                switch ((((nDI01[1]<<8)|nDI01[0])&0xfff0))
                {
                    case 0xC980:	//增益校正
                        nTmp = DltJBIDC98F(&nData[0]);
                        break;
                    case 0xC990:	//相位校正
                        nTmp = DltJBIDC99F(&nData[0]);
                        break;
                    case 0xC9A0:	//偏差校正
                        nTmp = DltJBIDC9AF(&nData[0]);
                        break;
                    case 0xC030:    //脉冲常数、启动电流
                        nTmp = DltJBIDC03F(&nData[0]);
                        break;
                    default:
                        nTmp = 0x02;
                    break;
                }
                OSSemPost(g_pSem[SEM_AC_METER_CALC_IDX]);
            }
            else
            {
                nTmp = 0x02;
            }
            
        }
    }
    }
    if(nTmp != 0)
	{
		pSendBuf[0] = nCtrlCode | 0xC0;
		pSendBuf[1] = 1;
		pSendBuf[2] = nTmp;
	}
	else
	{
		pSendBuf[0] = nCtrlCode | 0x80;
		pSendBuf[1] = 0;
	}
	return (pSendBuf[1] + 2);
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能:手动校表
*********************************************************************************/
INT8U DltJBCode1f(const INT8U *pValidBuf, INT8U nValidLen, INT8U *pSendBuf)
{
#define		LEN_METER_PASSWORD			3				//645规约密码长度
  
    //BOOLEAN bRet = FALSE;
	INT8U	nTmp = 0;
    INT8U	nErr = 0;
	INT8U	i;
	INT8U	nData[20];
	INT16U	nDataLen=0;
	INT8U	nCtrlCode=0;
	INT8U	nDI01[2];
	INT8U	nIndex=0;
	INT8U	nPwd[LEN_METER_PASSWORD];
	INT8U	nRight = 0xFF;
	INT32U  nChkSum = 0;
	//INT8U 	SuperPwd[3] = {0x66,0x27,0x84};
    
    memset(nData, 0, sizeof(nData));
	memset(nDI01, 0, sizeof(nDI01));
	memset(nPwd, 0, LEN_METER_PASSWORD);
    
    nCtrlCode = pValidBuf[8];
	nDataLen = pValidBuf[9];
	if(nDataLen < 2 || nDataLen>(nValidLen-12))
	{
		nTmp = 0x01;
	}
    else
	{
		for(i = 0; i < nDataLen; i++)
		{
			nData[i] = pValidBuf[10 + i] - 0x33;
		}
		nDI01[0]=nData[nIndex++];
		nDI01[1]=nData[nIndex++];
		nRight=nData[nIndex++];
		if(nRight != 0)									/*0级密码均可设置*/
		{
			nTmp = 0x04;
		}
        else
		{
			memcpy(nPwd,nData+nIndex,3);				/*密码判断*/
			nIndex += 3;
			if(memcmp(nPwd,g_nSuperPwd,3) != 0)
			{
				nTmp = 0x04;
			}
			else
            {
                MemReadAcCalPrm(F31, &nChkSum, sizeof(nChkSum));
                OSSemPend(g_pSem[SEM_AC_METER_CALC_IDX], 0, &nErr);
                if (0x0001E0CD == nChkSum)
                {
                    switch (((nDI01[1]<<8)|nDI01[0]))
					{
						case 0xC98F:	//增益校正
							nTmp = DltJBZYCal(10000, 15000, 1500000);
							break;
						case 0xC99F:	//相位校正
							nTmp = DltJBXWCal(750000);
							break;
						case 0xC9AF:	//偏差校正
							nTmp = DltJBOffsetCal(75000);
							break;
						default:
							nTmp = 0x02;
						break;
					}
                }
                else
				{
					switch (((nDI01[1]<<8)|nDI01[0]))
					{
						case 0xC98F:	//增益校正
							nTmp = DltJBZYCal(22000, 15000, 3300000);
							break;
						case 0xC99F:	//相位校正
							nTmp = DltJBXWCal(1650000);
							break;
						case 0xC9AF:	//偏差校正
							nTmp = DltJBOffsetCal(165000);
							break;
						default:
							nTmp = 0x02;
						break;
					}
				}
                OSSemPost(g_pSem[SEM_AC_METER_CALC_IDX]);
            }
        }
    }
    if(nTmp != 0)
	{
		pSendBuf[0] = nCtrlCode | 0xC0;
		pSendBuf[1] = 1;
		pSendBuf[2] = nTmp;
	}
	else
	{
		pSendBuf[0] = nCtrlCode | 0x80;
		pSendBuf[1] = 0;
	}
	return (pSendBuf[1] + 2);
    
}

/*
*****************************************************************************************************
* 函数名: DltJBZYCal
* 输　入: INT32U nDY, INT32U nDL, INT32U nYGGL
* 输  出:
* 返  回:
* 功能描述:自动校正增益
*****************************************************************************************************
*/
INT8U DltJBZYCal(INT32U nDY, INT32U nDL, INT32U nYGGL)
{
    INT32U	nTmpBuff[12];
	FP32	fTmp = 0.0;
	INT32U	nA1 = 0, nB1 = 0, nC1 = 0;
	INT8U	nIndex = 0;
	INT8U	nErr = 0;
	INT8U	nErrFlag = 0;
	INT8S	nSuccflag = RET_ERR_ERROR;
	INT32U	nChkSum = 0;
	MEM_AC_CAL_PRM sJB_Param;
    
    memset(nTmpBuff, 0, sizeof(nTmpBuff));
	memset(&sJB_Param, 0, sizeof(MEM_AC_CAL_PRM));
    
    if (MemReadAcCalPrm(FEND, &sJB_Param, sizeof(sJB_Param)) > 0)
    {
        nSuccflag = RET_ERR_OK;
    }
    nChkSum = CheckINT32USum((INT32U*)(&sJB_Param), 0, sizeof(MEM_AC_CAL_PRM)/4 - 3);
	if (nChkSum != sJB_Param.nCheckSumEntire)
	{
		return 0x01;
	}
    if(nSuccflag == RET_ERR_OK)
    {
        OSSemPend(g_pSem[SEM_ACREAD_CTRL_IDX], 0, &nErr);
		nTmpBuff[nIndex++] = ATT7022RegRead(r_UaRms);	//A相电压
		nTmpBuff[nIndex++] = ATT7022RegRead(r_UbRms);	//B相电压
		nTmpBuff[nIndex++] = ATT7022RegRead(r_UcRms);	//C相电压
		nTmpBuff[nIndex++] = ATT7022RegRead(r_IaRms);	//A相电流
		nTmpBuff[nIndex++] = ATT7022RegRead(r_IbRms);	//B相电流
		nTmpBuff[nIndex++] = ATT7022RegRead(r_IcRms);	//C相电流
		nTmpBuff[nIndex++] = ATT7022RegRead(r_Pa);		//A相有功功率
		nTmpBuff[nIndex++] = ATT7022RegRead(r_Pb);		//B相有功功率
		nTmpBuff[nIndex++] = ATT7022RegRead(r_Pc);		//C相有功功率
		OSSemPost(g_pSem[SEM_ACREAD_CTRL_IDX]);
		nIndex = 0;
        
        fTmp = AcCalcReadValid(pDataTable->nPulseConst, pDataTable->nHFConst, r_UaRms, nTmpBuff[nIndex++]);
		nA1 = (INT32U)(fTmp*100);
		fTmp = AcCalcReadValid(pDataTable->nPulseConst, pDataTable->nHFConst, r_UbRms, nTmpBuff[nIndex++]);
		nB1 = (INT32U)(fTmp*100);
		fTmp = AcCalcReadValid(pDataTable->nPulseConst, pDataTable->nHFConst, r_UcRms, nTmpBuff[nIndex++]);
		nC1 = (INT32U)(fTmp*100);
        sJB_Param.nUgain[0] = DltJBDYDLcf(nA1, nDY);
		sJB_Param.nUgain[1] = DltJBDYDLcf(nB1, nDY);
		sJB_Param.nUgain[2] = DltJBDYDLcf(nC1, nDY);
        
        fTmp = AcCalcReadValid(pDataTable->nPulseConst, pDataTable->nHFConst, r_IaRms, nTmpBuff[nIndex++]);
		nA1 = (INT32U)(fTmp/40*10000);
		fTmp = AcCalcReadValid(pDataTable->nPulseConst, pDataTable->nHFConst, r_IbRms, nTmpBuff[nIndex++]);
		nB1 = (INT32U)(fTmp/40*10000);
		fTmp = AcCalcReadValid(pDataTable->nPulseConst, pDataTable->nHFConst, r_IcRms, nTmpBuff[nIndex++]);
		nC1 = (INT32U)(fTmp/40*10000);
		sJB_Param.nIgain[0] = DltJBDYDLcf(nA1, nDL);
		sJB_Param.nIgain[1] = DltJBDYDLcf(nB1, nDL);
		sJB_Param.nIgain[2] = DltJBDYDLcf(nC1, nDL);
        
        fTmp = AcCalcReadValid(pDataTable->nPulseConst, pDataTable->nHFConst, r_Pa, nTmpBuff[nIndex++]);
		nA1 = (INT32U)(fTmp*10000000);
		fTmp = AcCalcReadValid(pDataTable->nPulseConst, pDataTable->nHFConst, r_Pb, nTmpBuff[nIndex++]);
		nB1 = (INT32U)(fTmp*10000000);
		fTmp = AcCalcReadValid(pDataTable->nPulseConst, pDataTable->nHFConst, r_Pc, nTmpBuff[nIndex++]);
		nC1 = (INT32U)(fTmp*10000000);
		sJB_Param.nPgain[0] = DltJBPQcf(nA1,nYGGL);
		sJB_Param.nPgain[1] = DltJBPQcf(nB1,nYGGL);
		sJB_Param.nPgain[2] = DltJBPQcf(nC1,nYGGL);
        
        sJB_Param.nQgain[0] = sJB_Param.nPgain[0];
		sJB_Param.nQgain[1] = sJB_Param.nPgain[1];
		sJB_Param.nQgain[2] = sJB_Param.nPgain[2];
		
		sJB_Param.nSgain[0] = sJB_Param.nPgain[0];
		sJB_Param.nSgain[1] = sJB_Param.nPgain[1];
		sJB_Param.nSgain[2] = sJB_Param.nPgain[2];
        
        sJB_Param.nCheckSumEntire = CheckINT32USum((INT32U*)(&sJB_Param), 0, sizeof(MEM_AC_CAL_PRM)/4 - 3);// 所有校表寄存器的累加和(含固定值寄存器)
        
        nSuccflag = MemWriteAcCalPrm(FEND, &sJB_Param, sizeof(sJB_Param));
        
        /*if (MemWriteAcCalPrm(FEND, &sJB_Param, sizeof(sJB_Param)) >= 0)
        {
            nSuccflag = RET_ERR_OK;
        }*/
        if(nSuccflag == RET_ERR_OK)
		{
			if (RET_ERR_OK != AcCalcJust7022(&sJB_Param))
			{
				nErrFlag = 0x01;
			}
		}
		else
		{
			nErrFlag = 0x01;
		}
    }
    else
	{
		nErrFlag = 0x01;
	}
    return nErrFlag;
}

/*
*****************************************************************************************************
* 函数名:DltJBXWCal
* 输　入:INT32U	nYGGL
* 输  出:
* 返  回:
* 功能描述:自动校正相位
*****************************************************************************************************
*/
INT8U DltJBXWCal(INT32U nYGGL)
{
    INT32U	nTmpBuff[3];
	FP32	fTmp = 0.0;
	INT32U	nA1=0, nB1=0, nC1=0;
	INT8U	nIndex = 0;
	INT8U	nErr = 0;
	INT8U	nErrFlag = 0;
	INT8S	nSuccflag = RET_ERR_ERROR;
	INT32U	nChkSum = 0;
	MEM_AC_CAL_PRM sJB_Param;
    
    memset(nTmpBuff, 0, sizeof(nTmpBuff));
	memset(&sJB_Param, 0, sizeof(MEM_AC_CAL_PRM));
    
    if (MemReadAcCalPrm(FEND, &sJB_Param, sizeof(sJB_Param)) > 0)
    {
        nSuccflag = RET_ERR_OK;
    }
    nChkSum = CheckINT32USum((INT32U*)(&sJB_Param), 0, sizeof(MEM_AC_CAL_PRM)/4 - 3);
	if (nChkSum != sJB_Param.nCheckSumEntire)
	{
		return 0x01;
	}
    if(nSuccflag == RET_ERR_OK)
    {
        OSSemPend(g_pSem[SEM_ACREAD_CTRL_IDX], 0, &nErr);
		nTmpBuff[nIndex++] = ATT7022RegRead(r_Pa);		//A相有功功率
		nTmpBuff[nIndex++] = ATT7022RegRead(r_Pb);		//B相有功功率
		nTmpBuff[nIndex++] = ATT7022RegRead(r_Pc);		//C相有功功率
		OSSemPost(g_pSem[SEM_ACREAD_CTRL_IDX]);
		nIndex = 0;
		
		fTmp = AcCalcReadValid(pDataTable->nPulseConst, pDataTable->nHFConst, r_Pa, nTmpBuff[nIndex++]);
		nA1 = (INT32U)(fTmp*10000000);
		fTmp = AcCalcReadValid(pDataTable->nPulseConst, pDataTable->nHFConst, r_Pb, nTmpBuff[nIndex++]);
		nB1 = (INT32U)(fTmp*10000000);
		fTmp = AcCalcReadValid(pDataTable->nPulseConst, pDataTable->nHFConst, r_Pc, nTmpBuff[nIndex++]);
		nC1 = (INT32U)(fTmp*10000000);
		sJB_Param.nPhSregpq0[0] = DltJBXWcf(nA1,nYGGL);
		sJB_Param.nPhSregpq0[1] = DltJBXWcf(nB1,nYGGL);
		sJB_Param.nPhSregpq0[2] = DltJBXWcf(nC1,nYGGL);
		
		sJB_Param.nPhSregpq1[0] = sJB_Param.nPhSregpq0[0];
		sJB_Param.nPhSregpq1[1] = sJB_Param.nPhSregpq0[1];
		sJB_Param.nPhSregpq1[2] = sJB_Param.nPhSregpq0[2];

		sJB_Param.nCheckSumEntire = CheckINT32USum((INT32U*)(&sJB_Param), 0, sizeof(MEM_AC_CAL_PRM)/4 - 3);// 所有校表寄存器的累加和(含固定值寄存器)
        
        nSuccflag = MemWriteAcCalPrm(FEND, &sJB_Param, sizeof(sJB_Param));
        /*if (MemWriteAcCalPrm(FEND, &sJB_Param, sizeof(sJB_Param) >= 0))
        {
            nSuccflag = RET_ERR_OK;
        }*/
        if(nSuccflag == RET_ERR_OK)
		{
			if (RET_ERR_OK != AcCalcJust7022(&sJB_Param))
			{
				nErrFlag = 0x01;
			}
		}
		else
		{
			nErrFlag = 0x01;
		}
    }
    else
	{
		nErrFlag = 0x01;
	}
	return nErrFlag;
}

/*
*****************************************************************************************************
* 函数名:DltJBDYDLCal
* 输　入:INT32U	dy,	INT32U dl
* 输  出:
* 返  回:
* 功能描述:自动校正偏差
*****************************************************************************************************
*/
INT8U DltJBOffsetCal(INT32U nYGGL)
{
    INT32U	nTmpBuff[3];
	FP32	fTmp = 0.0;
	INT32U	nA1=0, nB1=0, nC1=0;
	INT8U	nIndex = 0;
	INT8U	nErr = 0;
	INT8U	nErrFlag = 0;
	INT8S	nSuccflag = RET_ERR_ERROR;
	INT32U	nChkSum = 0;
	MEM_AC_CAL_PRM	sJB_Param;

	memset(nTmpBuff, 0, sizeof(nTmpBuff));
	memset(&sJB_Param, 0, sizeof(MEM_AC_CAL_PRM));
    
    if (MemReadAcCalPrm(FEND, &sJB_Param, sizeof(sJB_Param) ) > 0)
    {
        nSuccflag = RET_ERR_OK;
    }
    nChkSum = CheckINT32USum((INT32U*)(&sJB_Param), 0, sizeof(MEM_AC_CAL_PRM)/4 - 3);
	if (nChkSum != sJB_Param.nCheckSumEntire)
	{
		return 0x01;
	}
    if(nSuccflag == RET_ERR_OK)
    {
        OSSemPend(g_pSem[SEM_ACREAD_CTRL_IDX],	0, &nErr);
		nTmpBuff[nIndex++] = ATT7022RegRead(r_Pa);		//A相有功功率
		nTmpBuff[nIndex++] = ATT7022RegRead(r_Pb);		//B相有功功率
		nTmpBuff[nIndex++] = ATT7022RegRead(r_Pc);		//C相有功功率
		OSSemPost(g_pSem[SEM_ACREAD_CTRL_IDX]);
		nIndex = 0;
		fTmp = AcCalcReadValid(pDataTable->nPulseConst, pDataTable->nHFConst, r_Pa, nTmpBuff[nIndex++]);
		nA1 = (INT32U)(fTmp*10000000);
		fTmp = AcCalcReadValid(pDataTable->nPulseConst, pDataTable->nHFConst, r_Pb, nTmpBuff[nIndex++]);
		nB1 = (INT32U)(fTmp*10000000);
		fTmp = AcCalcReadValid(pDataTable->nPulseConst, pDataTable->nHFConst, r_Pc, nTmpBuff[nIndex++]);
		nC1 = (INT32U)(fTmp*10000000);
		sJB_Param.nPoffset[0] = DltJBGLOffset(nA1,nYGGL);
		sJB_Param.nPoffset[1] = DltJBGLOffset(nB1,nYGGL);
		sJB_Param.nPoffset[2] = DltJBGLOffset(nC1,nYGGL);
		
		sJB_Param.nQoffset[0] = sJB_Param.nPoffset[0];
		sJB_Param.nQoffset[1] = sJB_Param.nPoffset[1];
		sJB_Param.nQoffset[2] = sJB_Param.nPoffset[2];

		sJB_Param.nCheckSumEntire = CheckINT32USum((INT32U*)(&sJB_Param), 0, sizeof(MEM_AC_CAL_PRM)/4 - 3);// 所有校表寄存器的累加和(含固定值寄存器)

        nSuccflag = MemWriteAcCalPrm(FEND, &sJB_Param, sizeof(sJB_Param));
		/*if (MemWriteAcCalPrm(FEND, &sJB_Param, sizeof(sJB_Param) >= 0))
        {
            nSuccflag = RET_ERR_OK;
        }*/
		if(nSuccflag == RET_ERR_OK)
		{
			if (RET_ERR_OK != AcCalcJust7022(&sJB_Param))
			{
				nErrFlag = 0x01;
			}
		}
		else
		{
			nErrFlag = 0x01;
		}
    }
    else
	{
		nErrFlag = 0x01;
	}
	return nErrFlag;
}

/********************************************************************************
* 函数名:DltParamSupported
* 输　入:INT8U *DI01,INT16U Length
* 输　出:
* 返  回:
* 功  能:判断规约参数帧长是否满足
*********************************************************************************/
INT8S DltJBParamSupported(const INT8U *pDI01, INT8U nLength)
{
	INT8S	nTmp = RET_ERR_ERROR;

	switch(((pDI01[1]<<8)|pDI01[0]))
	{
        case 0xC036:
            if(nLength < 1)
			{
				nTmp = RET_ERR_ERROR;
			}
			else
			{
				nTmp = RET_ERR_OK;
			}
            break;

		case 0xC035:
		case 0xC980:case 0xC981:case 0xC982:case 0xC983:case 0xC984:case 0xC985:case 0xC986:case 0xC987:case 0xC988:
		case 0xC990:case 0xC991:case 0xC992:
		case 0xC9A0:case 0xC9A1:case 0xC9A2:
			if(nLength < 2)
			{
				nTmp = RET_ERR_ERROR;
			}
			else
			{
				nTmp = RET_ERR_OK;
			}
			break;
		case 0xC030:
			if(nLength < 3)
			{
				nTmp = RET_ERR_ERROR;
			}
			else
			{
				nTmp = RET_ERR_OK;
			}
			break;
		case 0xC98F:
			if(nLength < 18)
			{
				nTmp = RET_ERR_ERROR;
			}
			else
			{
				nTmp = RET_ERR_OK;
			}
			break;
			
		case 0xC99F:case 0xC9AF:
			if(nLength < 6)
			{
				nTmp = RET_ERR_ERROR;
	}
	else
	{
				nTmp = RET_ERR_OK;
			}
			break;
		default:
			nTmp = RET_ERR_ERROR;
			break;
	}
	return  nTmp;
}


/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 增益校正
*********************************************************************************/
INT8U DltJBIDC98F(const INT8U *pdata)
{
    INT8U	nDI01[2];
	INT8U	nErrFlag = 0;
	INT8S	nSuccflag = RET_ERR_ERROR;
	INT32U	nChkSum = 0;
	MEM_AC_CAL_PRM sJB_Param;

	memset(nDI01, 0, sizeof(nDI01));
	memset(&sJB_Param, 0, sizeof(MEM_AC_CAL_PRM));
    
    if (MemReadAcCalPrm(FEND, &sJB_Param, sizeof(sJB_Param)) > 0)
    {
        nSuccflag = RET_ERR_OK;
    }
    nChkSum = CheckINT32USum((INT32U*)(&sJB_Param), 0, sizeof(MEM_AC_CAL_PRM)/4 - 3);
    if (nChkSum != sJB_Param.nCheckSumEntire)
	{
		return 0x01;
	}
    
    if(nSuccflag == RET_ERR_OK)
	{
		nDI01[0] = pdata[0];
		nDI01[1] = pdata[1];
		switch(((nDI01[1]<<8)|nDI01[0]))
		{
			case 0xC98F:
				sJB_Param.nUgain[0] = pdata[6]+(pdata[7]<<8);
				sJB_Param.nUgain[1] = pdata[8]+(pdata[9]<<8);
				sJB_Param.nUgain[2] = pdata[10]+(pdata[11]<<8);
				
				sJB_Param.nIgain[0] = pdata[12]+(pdata[13]<<8);
				sJB_Param.nIgain[1] = pdata[14]+(pdata[15]<<8);
				sJB_Param.nIgain[2] = pdata[16]+(pdata[17]<<8);
				
				sJB_Param.nPgain[0] = pdata[18]+(pdata[19]<<8);
				sJB_Param.nPgain[1] = pdata[20]+(pdata[21]<<8);
				sJB_Param.nPgain[2] = pdata[22]+(pdata[23]<<8);
				sJB_Param.nQgain[0] = sJB_Param.nPgain[0];
				sJB_Param.nQgain[1] = sJB_Param.nPgain[1];
				sJB_Param.nQgain[2] = sJB_Param.nPgain[2];
				sJB_Param.nSgain[0] = sJB_Param.nPgain[0];
				sJB_Param.nSgain[1] = sJB_Param.nPgain[1];
				sJB_Param.nSgain[2] = sJB_Param.nPgain[2];
				break;
			case 0xC980:
				sJB_Param.nUgain[0] = pdata[6]+(pdata[7]<<8);
				break;
			case 0xC981:
				sJB_Param.nUgain[1] = pdata[6]+(pdata[7]<<8);
				break;
			case 0xC982:
				sJB_Param.nUgain[2] = pdata[6]+(pdata[7]<<8);
				break;
			case 0xC983:
				sJB_Param.nIgain[0] = pdata[6]+(pdata[7]<<8);
				break;
			case 0xC984:
				sJB_Param.nIgain[1] = pdata[6]+(pdata[7]<<8);
				break;
			case 0xC985:
				sJB_Param.nIgain[2] = pdata[6]+(pdata[7]<<8);
				break;
			case 0xC986:
				sJB_Param.nPgain[0] = pdata[6]+(pdata[7]<<8);
				sJB_Param.nQgain[0] = sJB_Param.nPgain[0];
				sJB_Param.nSgain[0] = sJB_Param.nPgain[0];
				break;
			case 0xC987:
				sJB_Param.nPgain[1] = pdata[6]+(pdata[7]<<8);
				sJB_Param.nQgain[1] = sJB_Param.nPgain[1];
				sJB_Param.nSgain[1] = sJB_Param.nPgain[1];
				break;
			case 0xC988:
				sJB_Param.nPgain[2] = pdata[6]+(pdata[7]<<8);
				sJB_Param.nQgain[2] = sJB_Param.nPgain[2];
				sJB_Param.nSgain[2] = sJB_Param.nPgain[2];
				break;
			default:
				nErrFlag = 0x02;
				break;
		}
        sJB_Param.nCheckSumEntire = CheckINT32USum((INT32U*)(&sJB_Param), 0, sizeof(MEM_AC_CAL_PRM)/4 - 3);// 所有校表寄存器的累加和(含固定值寄存器)
        
        nSuccflag = MemWriteAcCalPrm(FEND, &sJB_Param, sizeof(sJB_Param));
        /*if (MemWriteAcCalPrm(FEND, &sJB_Param, sizeof(sJB_Param) >= 0))
        {
            nSuccflag = RET_ERR_OK;
        }*/
        if(nSuccflag == RET_ERR_OK)
		{
			if (RET_ERR_OK != AcCalcJust7022(&sJB_Param))
			{
				nErrFlag = 0x01;
			}
		}
		else
		{
			nErrFlag = 0x01;
		}
    }
    else
	{
		nErrFlag = 0x01;
	}
    return nErrFlag;
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 相位校正
*********************************************************************************/
INT8U DltJBIDC99F(const INT8U *pdata)
{
    INT8U	nDI01[2];
	INT8U	nErrFlag = 0;
	INT8S	nSuccflag = RET_ERR_ERROR;
	INT32U	nChkSum = 0;
	MEM_AC_CAL_PRM sJB_Param;
    
    memset(nDI01, 0, sizeof(nDI01));
	memset(&sJB_Param, 0, sizeof(MEM_AC_CAL_PRM));
    
    if (MemReadAcCalPrm(FEND, &sJB_Param, sizeof(sJB_Param) ) > 0 )
    {
        nSuccflag = RET_ERR_OK;
    }
    nChkSum = CheckINT32USum((INT32U*)(&sJB_Param), 0, sizeof(MEM_AC_CAL_PRM)/4 - 3);
    if (nChkSum != sJB_Param.nCheckSumEntire)
	{
		return 0x01;
	}
    
    if(nSuccflag == RET_ERR_OK)
    {
        nDI01[0] = pdata[0];
		nDI01[1] = pdata[1];
		switch(((nDI01[1]<<8)|nDI01[0]))
		{
            case 0xC99F:
				sJB_Param.nPhSregpq0[0] = pdata[6]+(pdata[7]<<8);
				sJB_Param.nPhSregpq0[1] = pdata[8]+(pdata[9]<<8);
				sJB_Param.nPhSregpq0[2] = pdata[10]+(pdata[11]<<8);
				sJB_Param.nPhSregpq1[0] = sJB_Param.nPhSregpq0[0];
				sJB_Param.nPhSregpq1[1] = sJB_Param.nPhSregpq0[1];
				sJB_Param.nPhSregpq1[2] = sJB_Param.nPhSregpq0[2];
				break;
			case 0xC990:
				sJB_Param.nPhSregpq0[0] = pdata[6]+(pdata[7]<<8);
				sJB_Param.nPhSregpq1[0] = sJB_Param.nPhSregpq0[0];
				break;
			case 0xC991:
				sJB_Param.nPhSregpq0[1] = pdata[6]+(pdata[7]<<8);
				sJB_Param.nPhSregpq1[1] = sJB_Param.nPhSregpq0[1];
				break;
			case 0xC992:
				sJB_Param.nPhSregpq0[2] = pdata[6]+(pdata[7]<<8);
				sJB_Param.nPhSregpq1[2] = sJB_Param.nPhSregpq0[2];
				break;
			default:
				nErrFlag = 0x02;
				break;
        }
        sJB_Param.nCheckSumEntire = CheckINT32USum((INT32U*)(&sJB_Param), 0, sizeof(MEM_AC_CAL_PRM)/4 - 3);// 所有校表寄存器的累加和(含固定值寄存器)
        
        nSuccflag = MemWriteAcCalPrm(FEND, &sJB_Param, sizeof(sJB_Param));
        /*if (MemWriteAcCalPrm(FEND, &sJB_Param, sizeof(sJB_Param) >= 0))
        {
            nSuccflag = RET_ERR_OK;
        }*/
            
        if(nSuccflag == RET_ERR_OK)
		{
			if (RET_ERR_OK != AcCalcJust7022(&sJB_Param))
			{
				nErrFlag = 0x01;
			}
		}
		else
		{
			nErrFlag = 0x01;
		}    
    }
    else
	{
		nErrFlag = 0x01;
	}
    return nErrFlag;
}
            
/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 偏差校正
*********************************************************************************/
INT8U DltJBIDC9AF(const INT8U *pdata)
{
    INT8U	nDI01[2];
	INT8U	nErrFlag = 0;
	INT8S	nSuccflag = RET_ERR_ERROR;
	INT32U	nChkSum = 0;
	MEM_AC_CAL_PRM sJB_Param;
    
    memset(nDI01, 0, sizeof(nDI01));
	memset(&sJB_Param, 0, sizeof(MEM_AC_CAL_PRM));
    		
    if (MemReadAcCalPrm(FEND, &sJB_Param, sizeof(sJB_Param)) > 0)
    {
        nSuccflag = RET_ERR_OK;
    }
    nChkSum = CheckINT32USum((INT32U*)(&sJB_Param), 0, sizeof(MEM_AC_CAL_PRM)/4 - 3);
    if (nChkSum != sJB_Param.nCheckSumEntire)
	{
		return 0x01;
	}
    if(nSuccflag == RET_ERR_OK)
    {
        nDI01[0] = pdata[0];
		nDI01[1] = pdata[1];
		switch(((nDI01[1]<<8)|nDI01[0]))
        {
			case 0xC9AF:
				sJB_Param.nPoffset[0] = pdata[6]+(pdata[7]<<8);
				sJB_Param.nPoffset[1] = pdata[8]+(pdata[9]<<8);
				sJB_Param.nPoffset[2] = pdata[10]+(pdata[11]<<8);
				sJB_Param.nQoffset[0] = sJB_Param.nPoffset[0];
				sJB_Param.nQoffset[1] = sJB_Param.nPoffset[1];
				sJB_Param.nQoffset[2] = sJB_Param.nPoffset[2];
				break;
			case 0xC9A0:
				sJB_Param.nPoffset[0] = pdata[6]+(pdata[7]<<8);
				sJB_Param.nQoffset[0] = sJB_Param.nPoffset[0];
				break;
			case 0xC9A1:
				sJB_Param.nPoffset[1] = pdata[6]+(pdata[7]<<8);
				sJB_Param.nQoffset[1] = sJB_Param.nPoffset[1];
				break;
			case 0xC9A2:
				sJB_Param.nPoffset[2] = pdata[6]+(pdata[7]<<8);
				sJB_Param.nQoffset[2] = sJB_Param.nPoffset[2];
				break;
			default:
				nErrFlag = 0x02;
				break;
		}
        sJB_Param.nCheckSumEntire = CheckINT32USum((INT32U*)(&sJB_Param), 0, sizeof(MEM_AC_CAL_PRM)/4 - 3);// 所有校表寄存器的累加和(含固定值寄存器)
        
        nSuccflag = MemWriteAcCalPrm(FEND, &sJB_Param, sizeof(sJB_Param));
        /*if (MemWriteAcCalPrm(FEND, &sJB_Param, sizeof(sJB_Param) >= 0))
        {
            nSuccflag = RET_ERR_OK;
        }*/
            
        if(nSuccflag == RET_ERR_OK)
		{
			if (RET_ERR_OK != AcCalcJust7022(&sJB_Param))
			{
				nErrFlag = 0x01;
			}
		}
		else
		{
			nErrFlag = 0x01;
		}
    }
    else
	{
		nErrFlag = 0x01;
	}
    return nErrFlag;
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 设置脉冲常数和启动电流
*********************************************************************************/
INT8U DltJBIDC03F(const INT8U *pdata)
            {
    INT8U 	nDI01[2];
	INT8U	nErrFlag = 0;
	INT8S	nSuccflag = RET_ERR_ERROR;
	INT32U	nBuff = 0;
	INT32U	nChkSum = 0;
	MEM_AC_CAL_PRM sJB_Param;
				
	memset(nDI01, 0, sizeof(nDI01));
	memset(&sJB_Param, 0, sizeof(MEM_AC_CAL_PRM));
              
    if (MemReadAcCalPrm(FEND, &sJB_Param, sizeof(sJB_Param))  > 0 )
    {
        nSuccflag = RET_ERR_OK;
    }
    nChkSum = CheckINT32USum((INT32U*)(&sJB_Param), 0, sizeof(MEM_AC_CAL_PRM)/4 - 3);
    if (nChkSum != sJB_Param.nCheckSumEntire)
	{
		return 0x01;
            }
            
    if(nSuccflag == RET_ERR_OK)
	{
		nDI01[0] = pdata[0];
		nDI01[1] = pdata[1];
		switch(((nDI01[1]<<8)|nDI01[0]))
		{
			case 0xC030:
            {
                
				nBuff = BCD_TO_INT8U(pdata[6]) + 100 * BCD_TO_INT8U(pdata[7]) + 10000 * BCD_TO_INT8U(pdata[8]);
				if (nBuff < 3200)
            {
					nBuff = 3200;
            }
				if (nBuff > 9600)
            {
					nBuff = 9600;
            }
				pDataTable->nPulseConst = nPulTable[nBuff/3200 - 1].nPulConst;
   				pSavDataTable->nPulseConst = nPulTable[nBuff/3200 - 1].nPulConst;
				if (0x0001E0CD == sJB_Param.nCheckSumReset)
            {
   					sJB_Param.nHfconst = nPulTable[nBuff/3200 - 1].nHFConst2;
            }
				else
            {
					sJB_Param.nHfconst = nPulTable[nBuff/3200 - 1].nHFConst1;
				}
                break;
            }
			case 0xC035:
            {
				nBuff = BCD_TO_INT8U(pdata[6]) + 100 * BCD_TO_INT8U(pdata[7]);
				if (nBuff < 2)
				{
					nBuff = 2;
            }
				sJB_Param.nIstarup = (INT32U)(39.3216 * nBuff);
				break;
            }
            /*case 0xC036:
                if (0x55 == pdata[6])
            {
                  sJB_Param.nEMUCfg = 0x00003184;
            }
                else if (0xAA == pdata[6])
            {
                  sJB_Param.nEMUCfg = 0x000031C4;
            }
                break;*/
				
			default:
				nErrFlag = 0x02;
				break;
        }
        sJB_Param.nCheckSumEntire = CheckINT32USum((INT32U*)(&sJB_Param), 0, sizeof(MEM_AC_CAL_PRM)/4 - 3);// 所有校表寄存器的累加和(含固定值寄存器)
        
        nSuccflag = MemWriteAcCalPrm(FEND, &sJB_Param, sizeof(sJB_Param));
        /*if (MemWriteAcCalPrm(FEND, &sJB_Param, sizeof(sJB_Param) >= 0))
        {
            nSuccflag = RET_ERR_OK;
        }*/
            
        if(nSuccflag == RET_ERR_OK)
		{
			if (RET_ERR_OK != AcCalcJust7022(&sJB_Param))
			{
				nErrFlag = 0x01;
			}
		}
		else
		{
			nErrFlag = 0x01;
		}
	}
    else
	{
		nErrFlag = 0x01;
	}
    return nErrFlag;
}


/*
*****************************************************************************************************
* 函数名:DltJBDYcf
* 输　入:INT32U uMeasure, INT32U uStand
* 输  出:
* 返  回:电压电流增益
* 功能描述:计算电压电流增益
*****************************************************************************************************
*/
INT32U DltJBDYDLcf(INT32U uMeasure, INT32U uStand)
{
    INT32U	uVal = 0;
	FP32	fTemp = 0.0;
	
	if(0 == uMeasure)
	{
		return 0;
	}
	fTemp = ((FP32)uStand/(FP32)uMeasure-1);
	if(fTemp >= 0)
	{
		uVal = (INT32U)(fTemp*1024*32);
	}
	if(fTemp < 0)
	{
		uVal = (INT32U)(1024*64 + fTemp*1024*32);
	}
	return uVal;
}

/*
*****************************************************************************************************
* 函数名:DltJBPQcf
* 输　入:INT32U uMeasure, INT32U uStand
* 输  出:
* 返  回:功率系数
* 功能描述:计算功率系数
*****************************************************************************************************
*/
INT32U DltJBPQcf(INT32U uMeasure, INT32U uStand)
{
	INT32U	uVal = 0;
	FP32	fTemp = 0.0;
	FP32	fErr = 0.0;
	FP32	fPain = 0.0;
	
	if(0 == uMeasure)
	{
		return 0;
	}
	fTemp = (FP32)uMeasure - (FP32)uStand;
	fErr = fTemp/(FP32)uStand;
	fPain = (0-fErr)/(1+fErr);
	if(fPain >= 0)
	{
		uVal = (INT32U)(fPain*1024*32);
	}
	if(fPain < 0)
	{
		uVal = (INT32U)(1024*64+fPain*1024*32);
	}
	return uVal;
}

/*
*****************************************************************************************************
* 函数名:DltJBXWcf
* 输　入:INT32U uMeasure, INT32U uStand
* 输  出:
* 返  回:相位系数
* 功能描述:计算相位系数
*****************************************************************************************************
*/
INT32U DltJBXWcf(INT32U uMeasure, INT32U uStand)
{
	INT32U uVal = 0;
	FP32 fTemp = 0.0;
	FP32 fErr = 0.0;
	FP32 fPain = 0.0;

	if(0 == uMeasure)
	{
		return 0;
	}
	fTemp = (FP32)uMeasure - (FP32)uStand;
	fErr = fTemp/(FP32)uStand;
	fPain = (0-fErr)/1.732;
	if(fPain >= 0)
	{
		uVal = (INT32U)(fPain*1024*32);
	}
	if(fPain < 0)
	{
		uVal = (INT32U)(1024*64+fPain*1024*32);
	}
	return uVal;
}

/*
*****************************************************************************************************
* 函数名:DltJBGLOffset
* 输　入:INT32U uMeasure, INT32U uStand
* 输  出:
* 返  回:偏差
* 功能描述:计算偏差
*****************************************************************************************************
*/
INT32U DltJBGLOffset(INT32U uMeasure, INT32U uStand)
{
	INT32U uVal = 0;
	FP32 fTemp = 0.0;
	FP32 fErr = 0.0;
	FP32 fPain = 0.0;

	if(0 == uMeasure)
	{
		return 0;
	}
	fTemp = (FP32)uMeasure - (FP32)uStand;
	fErr = fTemp/(FP32)uStand;
	fPain = (uStand*pDataTable->nPulseConst*pDataTable->nHFConst*1024*1024*8*(0-fErr))/(51840000000);
	if(fPain >= 0)
	{
		uVal = (INT32U)(fPain*1024*32);
	}
	if(fPain < 0)
	{
		uVal = (INT32U)(1024*64+fPain*1024*32);
	}
	return uVal;
}

/********************************************************************************
* 函数名:Dlt07Analyse
* 输　入:
* 输　出:
* 返  回:
* 功  能: 07规约解析
*********************************************************************************/
INT8U Dlt07Analyse(const INT8U *pValidData, INT32U nValidLen, INT8U *pSendData)
{
	INT8U	nCtrlCode = 0;
	INT8U	nSendLen = 0;

	nCtrlCode = (pValidData[8]&0x1f);
    
    if(g_MeterPowerOff && nCtrlCode!=0x14)
    {      
        nSendLen = 0;
        return nSendLen;
    }
	switch(nCtrlCode)
	{
        case 0x11:				/*读数据*/
			nSendLen = Dlt07Code11(pValidData, nValidLen, pSendData);
			break;
            
		case 0x13:				/*读通信地址*/
			nSendLen = Dlt07Code13(pValidData, nValidLen, pSendData);
			break;
        case 0x14:				/*设置参数*/
			nSendLen = Dlt07Code14(pValidData, nValidLen, pSendData);
			break;
		case 0x15:				/*写设备地址*/
        {
            //AcPrmPrmInit();         //用于校表时方便，临时添加，正式烧写程序时去除
            nSendLen = DltCodeAddress(pValidData, nValidLen, pSendData, GB_07_PROT);
			break;
        }
        case 0x18:				/*修改密码*/
			nSendLen = DltCodePassword(pValidData, nValidLen, pSendData, PROT_GB645_07);
            break;
        case 0x19:				/*最大需量清零*/
			nSendLen = DltClrDemand(pValidData, nValidLen, pSendData, PROT_GB645_07);
			break;
		case 0x1a:				/*电表清零*/
			nSendLen = DltClrMeter(pValidData, nValidLen, pSendData, PROT_GB645_07);
			break;
		case 0x1b:				/*事件清零*/
			nSendLen = Dlt07Code1b(pValidData, nValidLen, pSendData);
			break;
		case 0x1c:				/*跳合闸、报警、保电*/
			nSendLen = Dlt07Code1c(pValidData, nValidLen, pSendData, PROT_GB645_07);
			break;
		default:
			nSendLen = 0;
		break;
	}
	return nSendLen;
}
/********************************************************************************
* 函数名:Dlt97Analyse
* 输　入:
* 输　出:
* 返  回:
* 功  能: 校表规约解析
*********************************************************************************/
INT8U Dlt97Analyse(const INT8U *pValidData, INT8U nValidLen, INT8U *pSendData)
{
	INT8U	nCtrlCode = 0;
	INT8U	nSendLen = 0;

	nCtrlCode = (pValidData[8] & 0x1f);
	switch(nCtrlCode)
	{
		case 0x01:				/*读数据*/
		case 0x03:
			nSendLen = Dlt97Code01(pValidData, nValidLen, pSendData);
			break;
		case 0x04:				/*设置参数*/
			nSendLen = Dlt97Code04(pValidData, nValidLen, pSendData);
			break;
		case 0x0a:				/*写设备地址*/
			nSendLen = DltCodeAddress(pValidData, nValidLen, pSendData, PROT_GB645_97);
			break;
		case 0x0c:				/*设置通信波特率*/
			//nSendLen = DltCodeBaud(pValidData, nValidLen, pSendData, PROT_GB645_97);
			break;
		case 0x0e:				/*97扩展电表清零*/
			nSendLen = DltClrMeter(pValidData, nValidLen, pSendData, PROT_GB645_97);
			break;
		case 0x0f:				/*修改密码*/
			nSendLen = DltCodePassword(pValidData, nValidLen, pSendData, PROT_GB645_97);
			break;
		case 0x10:				/*最大需量清零*/
			nSendLen = DltClrDemand(pValidData, nValidLen, pSendData, PROT_GB645_97);
			break;
		default:
			nSendLen = 0;
			break;
	}
	return nSendLen;
}
/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 读取电表数据
*********************************************************************************/
INT8U Dlt97Code01(const INT8U *pValidBuf, INT8U nValidLen, INT8U *pSendBuf)
{
	INT8U	nIsExist = 0;
	INT8U	i;
	INT8U	nCtrlCode = 0;
	INT8U	nData[256];
	INT8U	nDataLen = 0;
	INT8U	nSendlen = 0;
	INT8U	nDI01[2];

	memset(nData, 0, sizeof(nData));
	memset(nDI01, 0, sizeof(nDI01));

	nCtrlCode = pValidBuf[8];
	nDataLen = pValidBuf[9];
	if((nDataLen < 2)||(nDataLen > (nValidLen-12)))
	{
		nIsExist = 0x01;
	}
	else
	{
		for(i=0; i<nDataLen; i++)
		{
			nData[i] = pValidBuf[10 + i] - 0x33;
		}
		nDI01[0] = nData[0];
		nDI01[1] = nData[1];
		nSendlen = Dlt97SendResp(nDI01, &pSendBuf[2], &nIsExist);
	}

	if(nIsExist)
	{
		pSendBuf[0] = nCtrlCode|0xC0;
		pSendBuf[1] = 1;
		pSendBuf[2] = nIsExist;
	}
	else
	{
		pSendBuf[0] = nCtrlCode|0x80;
		pSendBuf[1] = nSendlen;
	}
	return (pSendBuf[1] + 2);

}
/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 设置电表参数
*********************************************************************************/
INT8U Dlt97Code04(const INT8U *pValidBuf, INT8U nValidLen, INT8U *pSendBuf)
{
	INT8U   i;
	INT8U	nTmp = 0;
	INT8S   nRetFlag = 0;
	INT8U   nData[256];
	INT8U	nDataLen = 0;
	INT8U   nCtrlCode = 0;
	INT8U   nDI01[2];
	INT8U   nIndex = 0;
	INT8U   nPwd[LEN_METER_PASSWORD];
	INT8U	nPassword[10][LEN_METER_PASSWORD];
	INT8U   nRight = 0xFF;

	memset(nData, 0, sizeof(nData));
	memset(nDI01, 0, sizeof(nDI01));
	memset(nPwd, 0, LEN_METER_PASSWORD);
	memset(nPassword, 0, 9*LEN_METER_PASSWORD);

	nCtrlCode = pValidBuf[8];
	nDataLen  = pValidBuf[9];
	if((nDataLen < 2) || (nDataLen > (nValidLen-12)))
	{
		nTmp = 0x01;
	}
	else
	{
		for(i=0; i<nDataLen; i++)
		{
			nData[i] = pValidBuf[10 + i] - 0x33;
		}
		nDI01[0] = nData[nIndex++];
		nDI01[1] = nData[nIndex++];

		if(!IsAllBcd(&nData[nIndex],(INT16U)(nDataLen-nIndex)))/*BCD码有效性判断*/
    	{
			nTmp = 0x01;
		}
		else
		{
			nRight = nData[nIndex++];
			if(nRight > 5)									/*4级密码均可设置*/
			{
				nTmp = 0x04;
			}
			else
			{
              
				memcpy(nPwd, nData+nIndex, 3);				/*密码判断*/
				nIndex += 3;
                
				if(MemReadAcPrm(F35, &nPassword, sizeof(nPassword)) >= 0)
                {
                    nRetFlag = RET_ERR_OK;
                }
				if(nRetFlag == RET_ERR_OK)
				{
					if(0 == nRight)
					{
						if(0 != (memcmp(nPwd, g_nSuperPwd, 3) != 0)&&(memcmp(nPwd, nPassword[0], 3)))
						{
							nTmp = 0x04;
						}
					}
					else
					{
						if(0 != memcmp(nPwd, nPassword[nRight], 3))
						{
							nTmp = 0x04;
						}
					}
				}
				else
				{
					if(nRight == 0)
					{
						if(0 != memcmp(nPwd, g_nSuperPwd, 3))
						{
							nTmp = 0x04;
						}
					}
				}
                

				if(0 == nTmp)
				{
					nRetFlag = Dlt97ParamSupported(nDI01, nDataLen-6);	/*减掉数据标示和密码共6字节*/
					if(nRetFlag == RET_ERR_OK)						/*计量支持的参数*/
					{
					 	AcCalcMeterPrmChanEve(&nData[0], PROT_GB645_97);
					 	nTmp = Dlt97SaveParam(nData, nDataLen);
					}
					else
					{
						nTmp = 0x02;
					}
				}
			}
		}
	}
	if(nTmp != 0)
	{
		pSendBuf[0] = nCtrlCode | 0xC0;
		pSendBuf[1] = 1;
		pSendBuf[2] = nTmp;
	}
	else
	{
		pSendBuf[0] = nCtrlCode | 0x80;
		pSendBuf[1] = 0;
	}
	return (pSendBuf[1]+2);
}
/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 读取数据上行组侦
*********************************************************************************/
INT8U Dlt97SendResp(const INT8U *pDI01, INT8U *pData, INT8U *pIsExist)
{
	INT8U	i;
	INT8U	nDataLen = 0;
	INT8U	nIndex = 0;
	INT16U	nDI = 0;
    INT32U  nTmp32U    = 0;
    INT8U   nTmpTime[6];
	STD_TIME	sTime;
    FORMAT09	sRealP[MAX_PHASE_NUM+1];
    FORMAT09	sRealQ[MAX_PHASE_NUM+1];
    FORMAT15	sDJTime;
    MEM_AC_PRM	sAc_Param;
	MT_DGN_ENERGY_DATA	sAcEn;
	MT_DGN_Q_ENERGY_DATA  sAcQEn;
	MT_DGN_TRI_ENERGY_DATA  sAcTriEn;
	MT_DGN_TRI_Q_EN_DATA  sAcTriQEn;
    MT_DGN_C_ENERGY_DATA sAcCombEnergy;
	MT_DGN_DEMAND_DATA	sAcDe;
	MT_DGN_Q_DE_DATA	sAcQDe;
    MEM_AC_DAY_DATA     sAcHistDayData;
    MEM_AC_RDAY_DATA     sAcHistData;
    MEM_AC_DJ_DATA      sAcDjData;
    MEM_AC_DJ_DATA      sAcdDjData;
    
    memset(&sRealP, 0, sizeof(sRealP));
    memset(&sRealQ, 0, sizeof(sRealQ));
    memset(&sDJTime, 0, sizeof(sDJTime));
    memset(&sAc_Param, 0, sizeof(sAc_Param));
    memset(&sAcEn, 0, sizeof(sAcEn));
    memset(&sAcQEn, 0, sizeof(sAcQEn));
    memset(&sAcTriEn, 0, sizeof(sAcTriEn));
    memset(&sAcTriQEn, 0, sizeof(sAcTriQEn));
    memset(&sAcDe, 0, sizeof(sAcDe));
    memset(&sAcQDe, 0, sizeof(sAcQDe));
    memset(&sAcHistData, 0, sizeof(sAcHistData));
    memset(&sAcHistDayData, 0, sizeof(sAcHistDayData));
    memset(&sAcDjData, 0, sizeof(sAcDjData));
    memset(&sAcdDjData, 0, sizeof(sAcdDjData));

	if((0x90 == pDI01[1])||(0x91 == pDI01[1])||(0x94 == pDI01[1])||(0x95 == pDI01[1])||(0x98 == pDI01[1])||(0x99 == pDI01[1])
		||(0xA0 == pDI01[1])||(0xA1 == pDI01[1])||(0xA4 == pDI01[1])||(0xA5 == pDI01[1])||(0xA8 == pDI01[1])||(0xA9 == pDI01[1])
			||(0xB0 == pDI01[1])||(0xB1 == pDI01[1])||(0xB4 == pDI01[1])||(0xB5 == pDI01[1])||(0xB8 == pDI01[1])||(0xB9 == pDI01[1]))
	{
		if(((pDI01[0]&0x0f) > MAX_FEE_NUM)&&((pDI01[0]&0x0f) != 0x0f))
		{
			*pIsExist = 0x40;						//费率数超
			return 0;
		}
		if((0x90 == pDI01[1])||(0x91 == pDI01[1]))
		{
			if(((pDI01[0]&0xf0) > 0)&&((pDI01[0]&0xf0) <= 0x20))
			{
				memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
				MemReadAcRealData(F1, &sAcEn, sizeof(sAcEn));
			}
			else
			{
				memset(&sAcQEn, 0, sizeof(MT_DGN_Q_ENERGY_DATA));
				MemReadAcRealData(F2, &sAcQEn, sizeof(sAcQEn));
			}
		}
		else if((0x94 == pDI01[1])||(0x95 == pDI01[1])||(0x98 == pDI01[1])||(0x99 == pDI01[1]))
		{
			if(((pDI01[0]&0xf0) > 0)&&((pDI01[0]&0xf0) <= 0x20))
			{
				memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
                MemReadAcHistDataProtProc(F1, ((pDI01[1]&0x0f)/4), &sAcHistDayData, sizeof(sAcHistDayData));
                memcpy(&sAcEn, &sAcHistDayData.sAcEnergy, sizeof(sAcEn));
				//MemReadACDayData(MEM_OBJ_AC_RDAY, 0, MEM_AC_RDAY_ENERGY_ID, ((pDI01[1]&0x0f)/4), &sAcEn, sizeof(sAcEn));
			}
			else
			{
				memset(&sAcQEn, 0, sizeof(MT_DGN_Q_ENERGY_DATA));
                MemReadAcHistDataProtProc(F2, ((pDI01[1]&0x0f)/4), &sAcHistData, sizeof(sAcHistData));
                memcpy(&sAcQEn, &sAcHistData.sAcQEnergy, sizeof(sAcEn));
				//MemReadACDayData(MEM_OBJ_AC_RDAY, 0, MEM_AC_RDAY_Q_ENERGY_ID, ((pDI01[1]&0x0f)/4), &sAcQEn, sizeof(sAcQEn));
			}
		}
		else if((0xA0 == pDI01[1])||(0xA1 == pDI01[1])||(0xB0 == pDI01[1])||(0xB1 == pDI01[1]))
		{
			if((0x10 == (pDI01[0]&0xf0))||(0x20 == (pDI01[0]&0xf0)))
			{
				memset(&sAcDe, 0, sizeof(MT_DGN_DEMAND_DATA));
				MemReadAcRealData(F7, &sAcDe, sizeof(sAcDe));
			}
			else
			{
				memset(&sAcQDe, 0, sizeof(MT_DGN_Q_DE_DATA));
				MemReadAcRealData(F8, &sAcQDe, sizeof(sAcQDe));
			}
		}
		else if((0xA4 == pDI01[1])||(0xA5 == pDI01[1])||(0xB4 == pDI01[1])||(0xB5 == pDI01[1])
					||(0xA8 == pDI01[1])||(0xA9 == pDI01[1])||(0xB8 == pDI01[1])||(0xB9 == pDI01[1]))
		{
			if((0x10 == (pDI01[0]&0xf0))||(0x20 == (pDI01[0]&0xf0)))
			{
				memset(&sAcDe, 0, sizeof(MT_DGN_DEMAND_DATA));
                MemReadAcHistDataProtProc(F2, ((pDI01[1]&0x0f)/4), &sAcHistData, sizeof(sAcHistData));
                memcpy(&sAcDe, &sAcHistData.sAcDemand, sizeof(sAcDe));
				//MemReadACDayData(MEM_OBJ_AC_RDAY, 0, MEM_AC_RDAY_DEMAND_ID, ((pDI01[1]&0x0f)/4), &sAcDe, sizeof(sAcDe));
			}
			else
			{
				memset(&sAcQDe, 0, sizeof(MT_DGN_Q_DE_DATA));
                MemReadAcHistDataProtProc(F2, ((pDI01[1]&0x0f)/4), &sAcHistData, sizeof(sAcHistData));
                memcpy(&sAcQDe, &sAcHistData.sAcQDemand, sizeof(sAcQDe));
				//MemReadACDayData(MEM_OBJ_AC_RDAY, 0, MEM_AC_RDAY_Q_DEMAND_ID, ((pDI01[1]&0x0f)/4), &sAcQDe, sizeof(sAcQDe));
			}
		}
	}
	else if(0xB6 == pDI01[1])
	{
		if(0x30 == (pDI01[0]&0xf0))
		{
			memset(&sAc_Param, 0, sizeof(MEM_AC_PRM));
			MemReadAcPrm(FEND, &sAc_Param, sizeof(sAc_Param));
		}
	}
	else if(0xC0 == (pDI01[1]&0xf0))
	{
		if((0xC0 == pDI01[1])&&(0x10 == (pDI01[0]&0xf0)))
		{
			memset(&sTime, 0, sizeof(STD_TIME));
			GetSysClk(&sTime);
		}
		else
		{
			memset(&sAc_Param, 0, sizeof(MEM_AC_PRM));
			MemReadAcPrm(FEND, &sAc_Param, sizeof(sAc_Param));
		}
	}

	pData[nIndex++] = pDI01[0];								/*获取低字节*/
	pData[nIndex++] = pDI01[1]; 							/*获取高字节*/
	nDI = ((pDI01[1]<<8)|pDI01[0]);
	switch((nDI&0xfff0))
	{
//召测当前电能示值
		case 0x9010:case 0x9410:case 0x9810:
			if(0x000f == (nDI&0x000f))
			{
				memcpy(pData+nIndex, &sAcEn.sPPEnergy[0], (MAX_FEE_NUM+1)*sizeof(FORMAT11));
				nIndex += ((MAX_FEE_NUM+1)*sizeof(FORMAT11));
			}
			else
			{
				memcpy(pData+nIndex, &sAcEn.sPPEnergy[(INT8U)(nDI&0x000f)], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
			}
			break;
		case 0x9020:case 0x9420:case 0x9820:
			if(0x000f == (nDI&0x000f))
			{
				memcpy(pData+nIndex, &sAcEn.sNPEnergy[0], (MAX_FEE_NUM+1)*sizeof(FORMAT11));
				nIndex += ((MAX_FEE_NUM+1)*sizeof(FORMAT11));
			}
			else
			{
				memcpy(pData+nIndex, &sAcEn.sNPEnergy[(INT8U)(nDI&0x000f)], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
			}
			break;
		case 0x9110:case 0x9510:case 0x9910:
			if(0x000f == (nDI&0x000f))
			{
				memcpy(pData+nIndex, &sAcEn.sPNEnergy[0], (MAX_FEE_NUM+1)*sizeof(FORMAT11));
				nIndex += ((MAX_FEE_NUM+1)*sizeof(FORMAT11));
			}
			else
			{
				memcpy(pData+nIndex, &sAcEn.sPNEnergy[(INT8U)(nDI&0x000f)], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
			}
			break;
		case 0x9120:case 0x9520:case 0x9920:
			if(0x000f == (nDI&0x000f))
			{
				memcpy(pData+nIndex, &sAcEn.sNNEnergy[0], (MAX_FEE_NUM+1)*sizeof(FORMAT11));
				nIndex += ((MAX_FEE_NUM+1)*sizeof(FORMAT11));
			}
			else
			{
				memcpy(pData+nIndex, &sAcEn.sNNEnergy[(INT8U)(nDI&0x000f)], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
			}
			break;
		case 0x9130:case 0x9530:case 0x9930:
			if(0x000f == (nDI&0x000f))
			{
				memcpy(pData+nIndex, &sAcQEn.sQ1Energy[0], (MAX_FEE_NUM+1)*sizeof(FORMAT11));
				nIndex += ((MAX_FEE_NUM+1)*sizeof(FORMAT11));
			}
			else
			{
				memcpy(pData+nIndex, &sAcQEn.sQ1Energy[(INT8U)(nDI&0x000f)], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
			}
			break;
		case 0x9140:case 0x9540:case 0x9940:
			if(0x000f == (nDI&0x000f))
			{
				memcpy(pData+nIndex, &sAcQEn.sQ4Energy[0], (MAX_FEE_NUM+1)*sizeof(FORMAT11));
				nIndex += ((MAX_FEE_NUM+1)*sizeof(FORMAT11));
			}
			else
			{
				memcpy(pData+nIndex, &sAcQEn.sQ4Energy[(INT8U)(nDI&0x000f)], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
			}
			break;
		case 0x9150:case 0x9550:case 0x9950:
			if(0x000f == (nDI&0x000f))
			{
				memcpy(pData+nIndex, &sAcQEn.sQ2Energy[0], (MAX_FEE_NUM+1)*sizeof(FORMAT11));
				nIndex += ((MAX_FEE_NUM+1)*sizeof(FORMAT11));
			}
			else
			{
				memcpy(pData+nIndex, &sAcQEn.sQ2Energy[(INT8U)(nDI&0x000f)], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
			}
			break;
		case 0x9160:case 0x9560:case 0x9960:
			if(0x000f == (nDI&0x000f))
			{
				memcpy(pData+nIndex, &sAcQEn.sQ3Energy[0], (MAX_FEE_NUM+1)*sizeof(FORMAT11));
				nIndex += ((MAX_FEE_NUM+1)*sizeof(FORMAT11));
			}
			else
			{
				memcpy(pData+nIndex, &sAcQEn.sQ3Energy[(INT8U)(nDI&0x000f)], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
			}
			break;

//召测当前需量
		case 0xA010:case 0xA410:case 0xA810:
			if(0x000f == (nDI&0x000f))
			{
				memcpy(pData+nIndex, &sAcDe.sPPDemand[0], (MAX_FEE_NUM+1)*sizeof(FORMAT23));
				nIndex += ((MAX_FEE_NUM+1)*sizeof(FORMAT23));
			}
			else
			{
				memcpy(pData+nIndex, &sAcDe.sPPDemand[(INT8U)(nDI&0x000f)], sizeof(FORMAT23));
				nIndex += sizeof(FORMAT23);
			}
			break;
		case 0xA020:case 0xA420:case 0xA820:
			if(0x000f == (nDI&0x000f))
			{
				memcpy(pData+nIndex, &sAcDe.sNPDemand[0], (MAX_FEE_NUM+1)*sizeof(FORMAT23));
				nIndex += ((MAX_FEE_NUM+1)*sizeof(FORMAT23));
			}
			else
			{
				memcpy(pData+nIndex, &sAcDe.sNPDemand[(INT8U)(nDI&0x000f)], sizeof(FORMAT23));
				nIndex += sizeof(FORMAT23);
			}
			break;
		case 0xA110:case 0xA510:case 0xA910:
			if(0x000f == (nDI&0x000f))
			{
				memcpy(pData+nIndex, &sAcDe.sPNDemand[0], (MAX_FEE_NUM+1)*sizeof(FORMAT23));
				nIndex += ((MAX_FEE_NUM+1)*sizeof(FORMAT23));
			}
			else
			{
				memcpy(pData+nIndex, &sAcDe.sPNDemand[(INT8U)(nDI&0x000f)], sizeof(FORMAT23));
				nIndex += sizeof(FORMAT23);
			}
			break;
		case 0xA120:case 0xA520:case 0xA920:
			if(0x000f == (nDI&0x000f))
			{
				memcpy(pData+nIndex, &sAcDe.sNNDemand[0], (MAX_FEE_NUM+1)*sizeof(FORMAT23));
				nIndex += ((MAX_FEE_NUM+1)*sizeof(FORMAT23));
			}
			else
			{
				memcpy(pData+nIndex, &sAcDe.sNNDemand[(INT8U)(nDI&0x000f)], sizeof(FORMAT23));
				nIndex += sizeof(FORMAT23);
			}
			break;
		case 0xA130:case 0xA530:case 0xA930:
			if(0x000f == (nDI&0x000f))
			{
				memcpy(pData+nIndex, &sAcQDe.sQ1Demand[0], (MAX_FEE_NUM+1)*sizeof(FORMAT23));
				nIndex += ((MAX_FEE_NUM+1)*sizeof(FORMAT23));
			}
			else
			{
				memcpy(pData+nIndex, &sAcQDe.sQ1Demand[(INT8U)(nDI&0x000f)], sizeof(FORMAT23));
				nIndex += sizeof(FORMAT23);
			}
			break;
		case 0xA140:case 0xA540:case 0xA940:
			if(0x000f == (nDI&0x000f))
			{
				memcpy(pData+nIndex, &sAcQDe.sQ4Demand[0], (MAX_FEE_NUM+1)*sizeof(FORMAT23));
				nIndex += ((MAX_FEE_NUM+1)*sizeof(FORMAT23));
			}
			else
			{
				memcpy(pData+nIndex, &sAcQDe.sQ4Demand[(INT8U)(nDI&0x000f)], sizeof(FORMAT23));
				nIndex += sizeof(FORMAT23);
			}
			break;
		case 0xA150:case 0xA550:case 0xA950:
			if(0x000f == (nDI&0x000f))
			{
				memcpy(pData+nIndex, &sAcQDe.sQ2Demand[0], (MAX_FEE_NUM+1)*sizeof(FORMAT23));
				nIndex += ((MAX_FEE_NUM+1)*sizeof(FORMAT23));
			}
			else
			{
				memcpy(pData+nIndex, &sAcQDe.sQ2Demand[(INT8U)(nDI&0x000f)], sizeof(FORMAT23));
				nIndex += sizeof(FORMAT23);
			}
			break;
		case 0xA160:case 0xA560:case 0xA960:
			if(0x000f == (nDI&0x000f))
			{
				memcpy(pData+nIndex, &sAcQDe.sQ3Demand[0], (MAX_FEE_NUM+1)*sizeof(FORMAT23));
				nIndex += ((MAX_FEE_NUM+1)*sizeof(FORMAT23));
			}
			else
			{
				memcpy(pData+nIndex, &sAcQDe.sQ3Demand[(INT8U)(nDI&0x000f)], sizeof(FORMAT23));
				nIndex += sizeof(FORMAT23);
			}
			break;
//召测当前最大需量发生时间
		case 0xB010:case 0xB410:case 0xB810:
			if(0x000f == (nDI&0x000f))
			{
				for(i=0;i<MAX_FEE_NUM+1;i++)
				{
					memcpy(pData+nIndex, &sAcDe.sPPDemandTime[i], 4);
					nIndex += 4;
				}
			}
			else
			{
				memcpy(pData+nIndex, &sAcDe.sPPDemandTime[(INT8U)(nDI&0x000f)], 4);
				nIndex += 4;
			}
			break;
		case 0xB020:case 0xB420:case 0xB820:
			if(0x000f == (nDI&0x000f))
			{
				for(i=0;i<MAX_FEE_NUM+1;i++)
				{
					memcpy(pData+nIndex, &sAcDe.sNPDemandTime[i], 4);
					nIndex += 4;
				}
			}
			else
			{
				memcpy(pData+nIndex, &sAcDe.sNPDemandTime[(INT8U)(nDI&0x000f)], 4);
				nIndex += 4;
			}
			break;
		case 0xB110:case 0xB510:case 0xB910:
			if(0x000f == (nDI&0x000f))
			{
				for(i=0;i<MAX_FEE_NUM+1;i++)
				{
					memcpy(pData+nIndex, &sAcDe.sPNDemandTime[i], 4);
					nIndex += 4;
				}
			}
			else
			{
				memcpy(pData+nIndex, &sAcDe.sPNDemandTime[(INT8U)(nDI&0x000f)], 4);
				nIndex += 4;
			}
			break;
		case 0xB120:case 0xB520:case 0xB920:
			if(0x000f == (nDI&0x000f))
			{
				for(i=0;i<MAX_FEE_NUM+1;i++)
				{
					memcpy(pData+nIndex, &sAcDe.sNNDemandTime[i], 4);
					nIndex += 4;
				}
			}
			else
			{
				memcpy(pData+nIndex, &sAcDe.sNNDemandTime[(INT8U)(nDI&0x000f)], 4);
				nIndex += 4;
			}
			break;
		case 0xB130:case 0xB530:case 0xB930:
			if(0x000f == (nDI&0x000f))
			{
				for(i=0;i<MAX_FEE_NUM+1;i++)
				{
					memcpy(pData+nIndex, &sAcQDe.sQ1DemandTime[i], 4);
					nIndex += 4;
				}
			}
			else
			{
				memcpy(pData+nIndex, &sAcQDe.sQ1DemandTime[(INT8U)(nDI&0x000f)], 4);
				nIndex += 4;
			}
			break;
		case 0xB140:case 0xB540:case 0xB940:
			if(0x000f == (nDI&0x000f))
			{
				for(i=0;i<MAX_FEE_NUM+1;i++)
				{
					memcpy(pData+nIndex, &sAcQDe.sQ4DemandTime[i], 4);
					nIndex += 4;
				}
			}
			else
			{
				memcpy(pData+nIndex, &sAcQDe.sQ4DemandTime[(INT8U)(nDI&0x000f)], 4);
				nIndex += 4;
			}
			break;
		case 0xB150:case 0xB550:case 0xB950:
			if(0x000f == (nDI&0x000f))
			{
				for(i=0;i<MAX_FEE_NUM+1;i++)
				{
					memcpy(pData+nIndex, &sAcQDe.sQ2DemandTime[i], 4);
					nIndex += 4;
				}
			}
			else
			{
				memcpy(pData+nIndex, &sAcQDe.sQ2DemandTime[(INT8U)(nDI&0x000f)], 4);
				nIndex += 4;
			}
			break;
		case 0xB160:case 0xB560:case 0xB960:
			if(0x000f == (nDI&0x000f))
			{
				for(i=0;i<MAX_FEE_NUM+1;i++)
				{
					memcpy(pData+nIndex, &sAcQDe.sQ3DemandTime[i], 4);
					nIndex += 4;
				}
			}
			else
			{
				memcpy(pData+nIndex, &sAcQDe.sQ3DemandTime[(INT8U)(nDI&0x000f)], 4);
				nIndex += 4;
			}
			break;
            
  //事件  失压  断相  电压逆相序  -失流
           
            case 0xB310:case 0xB320:
                  if(0x0011 == (nDI&0x00ff))  //A相断相总次数
                  {		
                        nDataLen = 2;
                        MemReadAcXFlhBackData(F32, &nTmp32U, sizeof(nTmp32U));
                  }
                   else if(0x0012 == (nDI&0x00ff))		//B相断相总次数
                   {
                          nDataLen = 2;
                          MemReadAcXFlhBackData(F33, &nTmp32U, sizeof(nTmp32U));
                   }
                  else if(0x0013 == (nDI&0x00ff))	//C相断相总次数
                  {
                          nDataLen = 2;
                          MemReadAcXFlhBackData(F34, &nTmp32U, sizeof(nTmp32U));
                  }
                 
                   else if(0x0021 == (nDI&0x00ff))		//A相断相总累计时间
                    {
                              nDataLen = 3;
                            MemReadAcXBackData(F5, &nTmp32U, sizeof(nTmp32U));
                            nTmp32U = (nTmp32U / 60);
                    }
                  else if(0x0022 == (nDI&0x00ff))		//B相断相总累计时间
                  {
                            nDataLen = 3;
                          MemReadAcXBackData(F6, &nTmp32U, sizeof(nTmp32U));
                          nTmp32U = (nTmp32U / 60);
                  }      
                  else if(0x0023 == (nDI&0x00ff)) //C相断相总累计时间
                  {
                          nDataLen = 3;
                          MemReadAcXBackData(F7, &nTmp32U, sizeof(nTmp32U));
                          nTmp32U = (nTmp32U / 60);
                  }
                                      
                 if (2 == nDataLen)
                {
                    INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[0]);
                    memcpy(pData + nIndex, &g_nAcEventBuf[0], nDataLen);
                }
                else if (3 == nDataLen)
                {
                    INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[0]);
                    memcpy(pData + nIndex, &g_nAcEventBuf[0], nDataLen);
                }
               // else
            //	{
            //		memcpy(pData + nIndex, &nTmpTime[0], nDataLen);
            //	}
                nIndex += nDataLen;
                
                break;
            
//召测电表瞬时数据
		case 0xB610:
			if(0x000f == (nDI&0x000f))
			{
				for(i=0;i<MAX_PHASE_NUM;i++)
				{
					pData[nIndex++] = (((pDataTable->sRealInstant.sRealVolt[i].nFrac&0xf0)>>4)|((pDataTable->sRealInstant.sRealVolt[i].nIntg&0x0f)<<4));
					pData[nIndex++] = ((pDataTable->sRealInstant.sRealVolt[i].nIntg&0xf0)>>4);
				}
			}
			else
			{
				if(((nDI&0x000f) > 0)&&((nDI&0x000f)<4))
				{
					pData[nIndex++] = (((pDataTable->sRealInstant.sRealVolt[(nDI&0x000f)-1].nFrac&0xf0)>>4)|((pDataTable->sRealInstant.sRealVolt[(nDI&0x000f)-1].nIntg&0x0f)<<4));
					pData[nIndex++] = ((pDataTable->sRealInstant.sRealVolt[(nDI&0x000f)-1].nIntg&0xf0)>>4);
				}
				else
				{
					*pIsExist = 0x02;
				}
			}
			break;
		case 0xB620:
			if(0x000f == (nDI&0x000f))
			{
				for(i=0;i<MAX_PHASE_NUM;i++)
				{
					pData[nIndex++] = (((pDataTable->sRealInstant.sRealCurr[i].nD1&0xf0)>>4)|((pDataTable->sRealInstant.sRealCurr[i].nD2&0x0f)<<4));
					pData[nIndex++] = (((pDataTable->sRealInstant.sRealCurr[i].nD2&0xf0)>>4)|((pDataTable->sRealInstant.sRealCurr[i].nD3&0x0f)<<4));
				}
			}
			else
			{
				if(((nDI&0x000f) > 0)&&((nDI&0x000f)<5))
				{
					pData[nIndex++] = (((pDataTable->sRealInstant.sRealCurr[(nDI&0x000f)-1].nD1&0xf0)>>4)|((pDataTable->sRealInstant.sRealCurr[(nDI&0x000f)-1].nD2&0x0f)<<4));
					pData[nIndex++] = (((pDataTable->sRealInstant.sRealCurr[(nDI&0x000f)-1].nD2&0xf0)>>4)|((pDataTable->sRealInstant.sRealCurr[(nDI&0x000f)-1].nD3&0x0f)<<4));
				}
				else
				{
					*pIsExist = 0x02;
				}
			}
			break;
		case 0xB630:
			if(0x000f == (nDI&0x000f))
			{
				memcpy(pData+nIndex, &pDataTable->sRealInstant.sRealP[0], 3*(MAX_PHASE_NUM+1));
				nIndex += 3*(MAX_PHASE_NUM+1);
				memcpy(pData+nIndex, &sAc_Param.sPosPowLimt.nFrac1, 2);
				nIndex += 2;
				memcpy(pData+nIndex, &sAc_Param.sNegPowLimt.nFrac1, 2);
				nIndex += 2;
			}
			else
			{
				if((nDI&0x000f) < 4)
				{
					memcpy(pData+nIndex, &pDataTable->sRealInstant.sRealP[(nDI&0x000f)], 3);
					nIndex += 3;
				}
				else
				{
					if((nDI&0x000f) == 4)
					{
						memcpy(pData+nIndex, &sAc_Param.sPosPowLimt.nFrac1, 2);
						nIndex += 2;
					}
					else if((nDI&0x000f) == 5)
					{
						memcpy(pData+nIndex, &sAc_Param.sNegPowLimt.nFrac1, 2);
						nIndex += 2;
					}
					else
					{
						*pIsExist = 0x02;
					}
				}
			}
			break;
		case 0xB640:
			if(0x000f == (nDI&0x000f))
			{
				for(i=0;i<(MAX_PHASE_NUM+1);i++)
				{
					memcpy(pData+nIndex, &pDataTable->sRealInstant.sRealQ[i].nFrac1, 2);
					nIndex += 2;
				}
			}
			else
			{
				if((nDI&0x000f) < 4)
				{
					memcpy(pData+nIndex, &pDataTable->sRealInstant.sRealQ[(nDI&0x000f)].nFrac1, 2);
					nIndex += 2;
				}
				else
				{
					*pIsExist = 0x02;
				}
			}
			break;
		case 0xB650:
			if(0x000f == (nDI&0x000f))
			{
				memcpy(pData+nIndex, &pDataTable->sRealInstant.sRealPf[0], 2*(MAX_PHASE_NUM+1));
				nIndex += 2*(MAX_PHASE_NUM+1);
			}
			else
			{
				if((nDI&0x000f) < 4)
				{
					memcpy(pData+nIndex, &pDataTable->sRealInstant.sRealPf[(nDI&0x000f)], 2);
					nIndex += 2;
				}
				else
				{
					*pIsExist = 0x02;
				}
			}
			break;

		case 0xC010:
			if(0x1f == pDI01[0])
			{
				pData[nIndex++] = sTime.nWeek;
				pData[nIndex++] = sTime.nDay;
				pData[nIndex++] = sTime.nMon;
				pData[nIndex++] = sTime.nYear;
				pData[nIndex++] = sTime.nSec;
				pData[nIndex++] = sTime.nMin;
				pData[nIndex++] = sTime.nHour;
			}
			else if(0x10 == pDI01[0])
			{
				pData[nIndex++] = sTime.nWeek;
				pData[nIndex++] = sTime.nDay;
				pData[nIndex++] = sTime.nMon;
				pData[nIndex++] = sTime.nYear;
			}
			else if(0x11 == pDI01[0])
			{
				pData[nIndex++] = sTime.nSec;
				pData[nIndex++] = sTime.nMin;
				pData[nIndex++] = sTime.nHour;
			}
			else
			{
				*pIsExist = 0x02;
			}
			break;

		case 0xC030:
			if(0x3f == pDI01[0])
			{
//				pData[nIndex++] = 0x00;
//				pData[nIndex++] = 0x32;
//				pData[nIndex++] = 0x00;
//				pData[nIndex++] = 0x00;
//				pData[nIndex++] = 0x32;
//				pData[nIndex++] = 0x00;
				INT32U2BCD(pDataTable->nPulseConst, 6, &pData[nIndex]);
				nIndex += 3;
				INT32U2BCD(pDataTable->nPulseConst, 6, &pData[nIndex]);
				nIndex += 3;
				memcpy(&pData[nIndex], &sAc_Param.nMeterAddr[0], LEN_METER_ADDR);
				nIndex += LEN_METER_ADDR;
				memcpy(&pData[nIndex], &sAc_Param.nMeterNumber[0], LEN_METER_ADDR);
				nIndex += LEN_METER_ADDR;
				memcpy(&pData[nIndex], &sAc_Param.nMeterProNum[0], LEN_METER_ADDR);
				nIndex += LEN_METER_GLBH;
			}
			else if((0x30 == pDI01[0])||(0x31 == pDI01[0]))
			{
//				pData[nIndex++] = 0x00;
//				pData[nIndex++] = 0x32;
//				pData[nIndex++] = 0x00;
				INT32U2BCD(pDataTable->nPulseConst, 6, &pData[nIndex]);
				nIndex += 3;
			}
			else if(0x32 == pDI01[0])
			{
				memcpy(&pData[nIndex], &sAc_Param.nMeterAddr[0], LEN_METER_ADDR);
				nIndex += LEN_METER_ADDR;
			}
			else if(0x33 == pDI01[0])
			{
				memcpy(&pData[nIndex], &sAc_Param.nMeterNumber[0], LEN_METER_ADDR);
				nIndex += LEN_METER_ADDR;
			}
			else if(0x34 == pDI01[0])
			{
				memcpy(&pData[nIndex], &sAc_Param.nMeterProNum[0], LEN_METER_ADDR);
				nIndex += LEN_METER_GLBH;
			}
			else
			{
				*pIsExist = 0x02;
			}
			break;

		case 0xC110:
			if(0x1f == pDI01[0])
			{
				pData[nIndex++] = DEC2BCD(sAc_Param.nDemandPeriod);
				pData[nIndex++] = DEC2BCD(sAc_Param.nSlipTime);
				pData[nIndex++] = sAc_Param.nDispTime1;
				pData[nIndex++] = sAc_Param.nDispTime2;
				pData[nIndex++] = sAc_Param.nDispPoint1;
				pData[nIndex++] = sAc_Param.nDispPoint2;
				memcpy(&pData[nIndex], &sAc_Param.sMonthJS1, sizeof(FORMAT24));
				nIndex += sizeof(FORMAT24);
				pData[nIndex++] = sAc_Param.nCurveDay;
				memcpy(&pData[nIndex], &sAc_Param.sPBaseEnergy, sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				memcpy(&pData[nIndex], &sAc_Param.sQBaseEnergy, sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
			}
			else if(0x11 == pDI01[0])
			{
				pData[nIndex++] = DEC2BCD(sAc_Param.nDemandPeriod);
			}
			else if(0x12 == pDI01[0])
			{
				pData[nIndex++] = DEC2BCD(sAc_Param.nSlipTime);
			}
			else if(0x13 == pDI01[0])
			{
				pData[nIndex++] = sAc_Param.nDispTime1;
			}
			else if(0x14 == pDI01[0])
			{
				pData[nIndex++] = sAc_Param.nDispTime2;
			}
			else if(0x15 == pDI01[0])
			{
				pData[nIndex++] = sAc_Param.nDispPoint1;
			}
			else if(0x16 == pDI01[0])
			{
				pData[nIndex++] = sAc_Param.nDispPoint2;
			}
			else if(0x17 == pDI01[0])
			{
				memcpy(&pData[nIndex], &sAc_Param.sMonthJS1, sizeof(FORMAT24));
				nIndex += sizeof(FORMAT24);
			}
			else if(0x18 == pDI01[0])
			{
				pData[nIndex++] = sAc_Param.nCurveDay;
			}
			else if(0x19 == pDI01[0])
			{
				memcpy(&pData[nIndex], &sAc_Param.sPBaseEnergy, sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
			}
			else if(0x1A == pDI01[0])
			{
				memcpy(&pData[nIndex], &sAc_Param.sQBaseEnergy, sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
			}
			else
			{
				*pIsExist = 0x02;
			}
			break;

		case 0xC210:
			if(0x1f == pDI01[0])
			{
				pData[nIndex++] = 0x80;
				pData[nIndex++] = 0x00;
			}
			else if(0x11 == pDI01[0])
			{
				pData[nIndex++] = 0x80;
				pData[nIndex++] = 0x00;
			}
			else if(0x12 == pDI01[0])
			{
			}
			else
			{
				*pIsExist = 0x02;
			}
			break;

		case 0xC310:
			if (sAc_Param.nYearSegmNum > MAX_YEAR_SEGM_NUM)
			{
				sAc_Param.nYearSegmNum = MAX_YEAR_SEGM_NUM;
			}
			if (sAc_Param.nDayTableNum > MAX_DAY_TABLE_NUM)
			{
				sAc_Param.nDayTableNum = MAX_DAY_TABLE_NUM;
			}
			if (sAc_Param.nDaySegmNum > 10)
			{
				sAc_Param.nDaySegmNum = 10;
			}
			if (sAc_Param.nFeeNumber > 14)
			{
				sAc_Param.nFeeNumber = 14;
			}
			if (sAc_Param.nHolidayNumber > 13)
			{
				sAc_Param.nHolidayNumber = 13;
			}

			if(0x1f == pDI01[0])
			{
				pData[nIndex++] = DEC2BCD(sAc_Param.nYearSegmNum);
				pData[nIndex++] = DEC2BCD(sAc_Param.nDayTableNum);
				pData[nIndex++] = DEC2BCD(sAc_Param.nDaySegmNum);
				pData[nIndex++] = DEC2BCD(sAc_Param.nFeeNumber);
				pData[nIndex++] = DEC2BCD((INT8U)(sAc_Param.nHolidayNumber & 0x00ff));
			}
			else if(0x10 == pDI01[0])
			{
				pData[nIndex++] = DEC2BCD(sAc_Param.nYearSegmNum);
			}
			else if(0x11 == pDI01[0])
			{
				pData[nIndex++] = DEC2BCD(sAc_Param.nDayTableNum);
			}
			else if(0x12 == pDI01[0])
			{
				pData[nIndex++] = DEC2BCD(sAc_Param.nDaySegmNum);
			}
			else if(0x13 == pDI01[0])
			{
				pData[nIndex++] = DEC2BCD(sAc_Param.nFeeNumber);
			}
			else if(0x14 == pDI01[0])
			{
				pData[nIndex++] = DEC2BCD(sAc_Param.nHolidayNumber);
			}
			else
			{
				*pIsExist = 0x02;
			}
			break;

		case 0xC320:
			if(0x2f == pDI01[0])
			{
				 if (sAc_Param.nYearSegmNum > MAX_YEAR_SEGM_NUM)
                 {
                 	sAc_Param.nYearSegmNum = MAX_YEAR_SEGM_NUM;
                 }
				memcpy(&pData[nIndex], &sAc_Param.sYearSegmTable[0][0], sAc_Param.nYearSegmNum*sizeof(MM_DD_NN));
				nIndex += sAc_Param.nYearSegmNum*sizeof(MM_DD_NN);
			}
			else if(((pDI01[0]&0xf0) != 0x20)&& ((pDI01[0]&0x0f) < MAX_YEAR_SEGM_NUM))
			{
				memcpy(&pData[nIndex], &sAc_Param.sYearSegmTable[0][(INT8U)(pDI01[0]&0x0F)], sizeof(MM_DD_NN));
				nIndex += sizeof(MM_DD_NN);
			}
			else
			{
				*pIsExist = 0x02;
			}
			break;

		case 0xC330:case 0xC340:case 0xC350:case 0xC360:case 0xC370:case 0xC380:case 0xC390:case 0xC3A0:
			
			if (sAc_Param.nDayTableNum > MAX_DAY_TABLE_NUM)
			{
				sAc_Param.nDayTableNum = MAX_DAY_TABLE_NUM;
			}
			if (sAc_Param.nDaySegmNum > 10)
            {
            	sAc_Param.nDaySegmNum = 10;
            }
			if((0x0f == (pDI01[0]&0x0f))&&(((INT8U)((pDI01[0]&0xF0)>>4)-3) < sAc_Param.nDayTableNum))
			{
				memcpy(&pData[nIndex], &sAc_Param.sDaySegmTable[0][(INT8U)((pDI01[0]&0xF0)>>4)-3][0], sAc_Param.nDaySegmNum*sizeof(hh_mm_NN));
				nIndex += sAc_Param.nDaySegmNum*sizeof(hh_mm_NN);
			}
			else if((pDI01[0]&0x0f != 0x00)&&(((INT8U)((pDI01[0]&0xF0)>>4)-3) < sAc_Param.nDayTableNum)&&(((INT8U)(pDI01[0]&0x0F)-1) < sAc_Param.nDaySegmNum))
			{
				memcpy(&pData[nIndex], &sAc_Param.sDaySegmTable[0][(INT8U)((pDI01[0]&0xF0)>>4)-3][(INT8U)(pDI01[0]&0x0F)-1], sAc_Param.nDaySegmNum*sizeof(hh_mm_NN));
				nIndex += sizeof(hh_mm_NN);
			}
			else
			{
				*pIsExist = 0x02;
			}
			break;

		case 0xC410:
			if (sAc_Param.nHolidayNumber > 13)
			{
				sAc_Param.nHolidayNumber = 13;
			}
			
			if(((pDI01[0]&0x0f) > 0x00)&&((pDI01[0]&0x0f) < 0x0E))
			{
				memcpy(&pData[nIndex], &sAc_Param.sHolidayTable[(INT8U)(pDI01[0]&0x0F)-1], sizeof(YY_MM_DD_NN));
				nIndex += sizeof(YY_MM_DD_NN);
			}
			else
			{
				*pIsExist = 0x02;
			}
			break;

		default:
			*pIsExist = 0x02;
			break;
	}
	if((nIndex <= 2) && (0 == *pIsExist))
	{
		*pIsExist = 0x01;
	}
	nDataLen = nIndex;

	return nDataLen;
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 保存电表参数
*********************************************************************************/
INT8U Dlt97SaveParam(const INT8U *pData, INT8U nLen)
{
	INT8U	nIsExist = 0;
	INT8U	nIndex = 0;
	INT16U	nDI = 0;
	INT8U	nPwd = 0xFF;
	INT8U	nPwdRight = 0xFF;
	INT8S	nSuccflag = RET_ERR_ERROR;
	INT8U	nNum = 0;
	INT8U	nChanger = 0;
    INT8U	nState = 0x55;
	STD_TIME	sTime;
	MEM_AC_PRM	sAc_Param;

	memset(&sAc_Param, 0, sizeof(MEM_AC_PRM));
	if (MemReadAcPrm(FEND, &sAc_Param, sizeof(sAc_Param)) > 0)
    {
        nSuccflag = RET_ERR_OK;
    }
	if(nSuccflag == RET_ERR_OK)
	{
		nDI = ((pData[1]<<8)|pData[0]);
		nIndex += 2;
		nPwdRight = pData[nIndex++];
		pData += 6;
		nIndex = 0;
		switch(nDI)
		{
			case 0xB634:
				sAc_Param.sPosPowLimt.nFrac0 = 0;
				sAc_Param.sPosPowLimt.nFrac1 = pData[nIndex++];
				sAc_Param.sPosPowLimt.nIntg = pData[nIndex++];
				break;
			case 0xB635:
				sAc_Param.sNegPowLimt.nFrac0 = 0;
				sAc_Param.sNegPowLimt.nFrac1 = pData[nIndex++];
				sAc_Param.sNegPowLimt.nIntg = pData[nIndex++];
				break;
			case 0xB63F:
				nIndex += 12;
				sAc_Param.sPosPowLimt.nFrac0 = 0;
				sAc_Param.sPosPowLimt.nFrac1 = pData[nIndex++];
				sAc_Param.sPosPowLimt.nIntg = pData[nIndex++];
				sAc_Param.sNegPowLimt.nFrac0 = 0;
				sAc_Param.sNegPowLimt.nFrac1 = pData[nIndex++];
				sAc_Param.sNegPowLimt.nIntg = pData[nIndex++];
				break;

			case 0xC010:
				if(GetSysClk(&sTime) != RET_ERR_OK)
				{
					nIsExist = 0x01;
				}
				else
				{
					sTime.nWeek = pData[nIndex++];
					sTime.nDay = pData[nIndex++];
					sTime.nMon = pData[nIndex++];
					sTime.nYear = pData[nIndex++];
					if (RET_ERR_OK != SyncSysClk(&sTime))
					{
						nIsExist = 0x01;
					}
				}
				break;
			case 0xC011:
				if(GetSysClk(&sTime) != RET_ERR_OK)
				{
					nIsExist = 0x01;
				}
				else
				{
					sTime.nSec = pData[nIndex++];
					sTime.nMin = pData[nIndex++];
					sTime.nHour = pData[nIndex++];
					if(SyncSysClk(&sTime) != RET_ERR_OK)
					{
						nIsExist = 0x01;
					}
				}
				break;
			case 0xC01F:
				if(GetSysClk(&sTime) != RET_ERR_OK)
				{
					nIsExist = 0x01;
				}
				else
				{
					sTime.nWeek = pData[nIndex++];
					sTime.nDay = pData[nIndex++];
					sTime.nMon = pData[nIndex++];
					sTime.nYear = pData[nIndex++];
					sTime.nSec = pData[nIndex++];
					sTime.nMin = pData[nIndex++];
					sTime.nHour = pData[nIndex++];
					if(SyncSysClk(&sTime) != RET_ERR_OK)
					{
						nIsExist = 0x01;
					}
				}
				break;

			case 0xC022:
				sAc_Param.nHolidayFlag = pData[nIndex++];
				break;
			case 0xC02F:
				nIndex += 2;
				sAc_Param.nHolidayFlag = pData[nIndex++];
				break;

			case 0xC032:/*表号*/
				memcpy(&sAc_Param.nMeterAddr[0], &pData[nIndex], LEN_METER_ADDR);
				nChanger = 0x55;
				break;
			case 0xC033:/*用户号*/
				memcpy(&sAc_Param.nMeterNumber[0], &pData[nIndex], LEN_METER_ADDR);
				break;
			case 0xC034:/*设备码*/
				memcpy(&sAc_Param.nMeterProNum[0], &pData[nIndex], LEN_METER_ADDR);
				break;
			case 0xC03f:
				nIndex += 6;
				memcpy(&sAc_Param.nMeterAddr[0], &pData[nIndex], LEN_METER_ADDR);
				nIndex += LEN_METER_ADDR;
				memcpy(&sAc_Param.nMeterNumber[0], &pData[nIndex], LEN_METER_ADDR);
				nIndex += LEN_METER_ADDR;
				memcpy(&sAc_Param.nMeterProNum[0], &pData[nIndex], LEN_METER_ADDR);
				nIndex += LEN_METER_ADDR;
				nChanger = 0x55;
				break;

			case 0xC111:
				sAc_Param.nDemandPeriod = Bcd2Dec(pData[nIndex++]);
				break;
			case 0xC112:
				sAc_Param.nSlipTime = Bcd2Dec(pData[nIndex++]);
				break;
			case 0xC113:
				sAc_Param.nDispTime1 = pData[nIndex++];
				break;
			case 0xC114:
				sAc_Param.nDispTime2 = pData[nIndex++];
				break;
			case 0xC115:
				sAc_Param.nDispPoint1 = pData[nIndex++];
				break;
			case 0xC116:
				sAc_Param.nDispPoint2 = pData[nIndex++];
				break;
			case 0xC117:
				memcpy(&sAc_Param.sMonthJS1, &pData[nIndex], sizeof(FORMAT24));
				break;
			case 0xC118:
				sAc_Param.nCurveDay = pData[nIndex++];
				break;
			case 0xC119:
				memcpy(&sAc_Param.sPBaseEnergy, &pData[nIndex], sizeof(FORMAT11));
				break;
			case 0xC11A:
				memcpy(&sAc_Param.sQBaseEnergy, &pData[nIndex], sizeof(FORMAT11));
				break;
			case 0xC11F:
				sAc_Param.nDemandPeriod = Bcd2Dec(pData[nIndex++]);
				sAc_Param.nSlipTime = Bcd2Dec(pData[nIndex++]);
				sAc_Param.nDispTime1 = pData[nIndex++];
				sAc_Param.nDispTime2 = pData[nIndex++];
				sAc_Param.nDispPoint1 = pData[nIndex++];
				sAc_Param.nDispPoint2 = pData[nIndex++];
				memcpy(&sAc_Param.sMonthJS1, &pData[nIndex], sizeof(FORMAT24));
				nIndex += sizeof(FORMAT24);
				sAc_Param.nCurveDay = pData[nIndex++];
				memcpy(&sAc_Param.sPBaseEnergy, &pData[nIndex], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				memcpy(&sAc_Param.sQBaseEnergy, &pData[nIndex], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				break;

			case 0xC212:
				nPwd = pData[nIndex++];
				if((nPwdRight <= nPwd)&&(nPwd <= 9))
				{
					memcpy(&sAc_Param.nMeterPWD[nPwd], &pData[nIndex], 3);
				}
				else
				{
					nIsExist = 0x04;
				}
				break;
			case 0xC21F:
				nIndex += 2;
				nPwd = pData[nIndex++];
				if((nPwdRight <= nPwd)&&(nPwd <= 9))
				{
					memcpy(&sAc_Param.nMeterPWD[nPwd], &pData[nIndex], LEN_METER_PASSWORD);
				}
				else
				{
					nIsExist = 0x04;
				}
				break;

			case 0xC310:
				sAc_Param.nYearSegmNum = Bcd2Dec(pData[nIndex++]);
				if (sAc_Param.nYearSegmNum > MAX_YEAR_SEGM_NUM)
				{
					sAc_Param.nYearSegmNum = MAX_YEAR_SEGM_NUM;
				}
				break;
			case 0xC311:
				sAc_Param.nDayTableNum = Bcd2Dec(pData[nIndex++]);
				if (sAc_Param.nDayTableNum > MAX_DAY_TABLE_NUM)
				{
					sAc_Param.nDayTableNum = MAX_DAY_TABLE_NUM;
				}
				break;
			case 0xC312:
				sAc_Param.nDaySegmNum = Bcd2Dec(pData[nIndex++]);
				if (sAc_Param.nDaySegmNum > 10)
				{
					sAc_Param.nDaySegmNum = 10;
				}
				break;
			case 0xC313:
				sAc_Param.nFeeNumber = Bcd2Dec(pData[nIndex++]);
				if (sAc_Param.nFeeNumber > 14)
				{
					sAc_Param.nFeeNumber = 14;
				}
				break;
			case 0xC314:
				sAc_Param.nHolidayNumber = Bcd2Dec(pData[nIndex++]);
				if (sAc_Param.nHolidayNumber > 13)
				{
					sAc_Param.nHolidayNumber = 13;
				}
				break;
			case 0xC31F:
				sAc_Param.nYearSegmNum = Bcd2Dec(pData[nIndex++]);
				sAc_Param.nDayTableNum = Bcd2Dec(pData[nIndex++]);
				sAc_Param.nDaySegmNum = Bcd2Dec(pData[nIndex++]);
				sAc_Param.nFeeNumber = Bcd2Dec(pData[nIndex++]);
				sAc_Param.nHolidayNumber = Bcd2Dec(pData[nIndex++]);
				if (sAc_Param.nYearSegmNum > MAX_YEAR_SEGM_NUM)
				{
					sAc_Param.nYearSegmNum = MAX_YEAR_SEGM_NUM;
				}
				if (sAc_Param.nDayTableNum > MAX_DAY_TABLE_NUM)
				{
					sAc_Param.nDayTableNum = MAX_DAY_TABLE_NUM;
				}
				if (sAc_Param.nDaySegmNum > 10)
				{
					sAc_Param.nDaySegmNum = 10;
				}
				if (sAc_Param.nFeeNumber > 14)
				{
					sAc_Param.nFeeNumber = 14;
				}
				if (sAc_Param.nHolidayNumber > 13)
				{
					sAc_Param.nHolidayNumber = 13;
				}
				break;

			case 0xC321:case 0xC322:case 0xC323:case 0xC324:case 0xC325:case 0xC326:case 0xC327:
			case 0xC328:case 0xC329:case 0xC32A:case 0xC32B:case 0xC32C:case 0xC32D:case 0xC32E:
				memcpy(&sAc_Param.sYearSegmTable[0][(INT8U)(nDI&0x000F)], &pData[nIndex], sizeof(MM_DD_NN));
				break;

			case 0xC32F:
				nNum = (nLen - 6)/sizeof(MM_DD_NN);
				if(nNum > MAX_YEAR_SEGM_NUM)
				{
					nNum = MAX_YEAR_SEGM_NUM;
				}
				memcpy(&sAc_Param.sYearSegmTable[0][0], &pData[nIndex], nNum*sizeof(MM_DD_NN));
				break;

			case 0xC331:case 0xC332:case 0xC333:case 0xC334:case 0xC335:case 0xC336:case 0xC337:case 0xC338:case 0xC339:case 0xC33A:
			case 0xC341:case 0xC342:case 0xC343:case 0xC344:case 0xC345:case 0xC346:case 0xC347:case 0xC348:case 0xC349:case 0xC34A:
			case 0xC351:case 0xC352:case 0xC353:case 0xC354:case 0xC355:case 0xC356:case 0xC357:case 0xC358:case 0xC359:case 0xC35A:
			case 0xC361:case 0xC362:case 0xC363:case 0xC364:case 0xC365:case 0xC366:case 0xC367:case 0xC368:case 0xC369:case 0xC36A:
			case 0xC371:case 0xC372:case 0xC373:case 0xC374:case 0xC375:case 0xC376:case 0xC377:case 0xC378:case 0xC379:case 0xC37A:
			case 0xC381:case 0xC382:case 0xC383:case 0xC384:case 0xC385:case 0xC386:case 0xC387:case 0xC388:case 0xC389:case 0xC38A:
			case 0xC391:case 0xC392:case 0xC393:case 0xC394:case 0xC395:case 0xC396:case 0xC397:case 0xC398:case 0xC399:case 0xC39A:
			case 0xC3A1:case 0xC3A2:case 0xC3A3:case 0xC3A4:case 0xC3A5:case 0xC3A6:case 0xC3A7:case 0xC3A8:case 0xC3A9:case 0xC3AA:
				memcpy(&sAc_Param.sDaySegmTable[0][(INT8U)((nDI&0x00F0)>>4)-3][(INT8U)(nDI&0x000F)-1], &pData[nIndex], sizeof(hh_mm_NN));
				break;

			case 0xC33F:case 0xC34F:case 0xC35F:case 0xC36F:case 0xC37F:case 0xC38F:case 0xC39F:case 0xC3AF:
				nNum = (nLen - 6)/sizeof(hh_mm_NN);
				if(nNum > 10)
				{
					nNum = 10;
				}
				memcpy(&sAc_Param.sDaySegmTable[0][(INT8U)((nDI&0x00F0) >> 4) - 3][0], &pData[nIndex], nNum*sizeof(hh_mm_NN));
				break;

			case 0xC411:case 0xC412:case 0xC413:case 0xC414:case 0xC415:case 0xC416:case 0xC417:case 0xC419:case 0xC41A:case 0xC41B:
			case 0xC41C:case 0xC41D:
				memcpy(&sAc_Param.sHolidayTable[(INT8U)(nDI&0x000F) - 1], &pData[nIndex], 3);
				break;

			case 0xC41E:
				sAc_Param.nHolidayTable = pData[nIndex++];
				break;

			default:
				nIsExist = 0x02;
				break;
		}
	}
	else
	{
		nIsExist = 0x01;
		}

	if(0 == nIsExist)
	{
		nSuccflag = RET_ERR_ERROR;
		if (MemWriteAcPrm(FEND, &sAc_Param, sizeof(sAc_Param)) >= 0)
        {
            nSuccflag = RET_ERR_OK;
	}
		if(RET_ERR_OK != nSuccflag)
	{
			nIsExist = 0x01;
		}
		if (0x55 == nChanger)
		{
			//加地址变化标志
            //MemWriteXTempData(MEM_OBJ_X_TEMP_TERM, 0, MEM_TEMP_AC_UPDATE_ID, &nState, 1);
	}
}

	return nIsExist;
}

/********************************************************************************
* 函数名:DltParamSupported
* 输　入:INT8U* DI01,INT16U Length
* 输　出:
* 返  回:
* 功  能:判断规约参数帧长是否满足
*********************************************************************************/
INT8S Dlt97ParamSupported(const INT8U *pDI01, INT8U nLength)
{
	INT8S	nTmp = RET_ERR_ERROR;
	
	switch(((pDI01[1]<<8)|pDI01[0]))
	{
		case 0xC020:case 0xC021:case 0xC022:
		case 0xC111:case 0xC112:case 0xC113:case 0xC114:case 0xC115:case 0xC116:case 0xC118:
		case 0xC310:case 0xC311:case 0xC312:case 0xC313:case 0xC314:
			if(nLength < 1)
	{
				nTmp = RET_ERR_ERROR;
	}
			else
	{
				nTmp = RET_ERR_OK;
}
			break;

		case 0xC117:
		case 0xB634:case 0xB635:
			if(nLength < 2)
	{
				nTmp = RET_ERR_ERROR;
	}
			else
	{
				nTmp = RET_ERR_OK;
	}
			break;

		case 0xC011:
		case 0xC321:case 0xC322:case 0xC323:case 0xC324:case 0xC325:case 0xC326:case 0xC327:
		case 0xC328:case 0xC329:case 0xC32A:case 0xC32B:case 0xC32C:case 0xC32D:case 0xC32E:
		case 0xC32F://至少含有一个时区
		case 0xC331:case 0xC332:case 0xC333:case 0xC334:case 0xC335:case 0xC336:case 0xC337:case 0xC338:case 0xC339:case 0xC33A:
		case 0xC341:case 0xC342:case 0xC343:case 0xC344:case 0xC345:case 0xC346:case 0xC347:case 0xC348:case 0xC349:case 0xC34A:
		case 0xC351:case 0xC352:case 0xC353:case 0xC354:case 0xC355:case 0xC356:case 0xC357:case 0xC358:case 0xC359:case 0xC35A:
		case 0xC361:case 0xC362:case 0xC363:case 0xC364:case 0xC365:case 0xC366:case 0xC367:case 0xC368:case 0xC369:case 0xC36A:
		case 0xC371:case 0xC372:case 0xC373:case 0xC374:case 0xC375:case 0xC376:case 0xC377:case 0xC378:case 0xC379:case 0xC37A:
		case 0xC381:case 0xC382:case 0xC383:case 0xC384:case 0xC385:case 0xC386:case 0xC387:case 0xC388:case 0xC389:case 0xC38A:
		case 0xC391:case 0xC392:case 0xC393:case 0xC394:case 0xC395:case 0xC396:case 0xC397:case 0xC398:case 0xC399:case 0xC39A:
		case 0xC3A1:case 0xC3A2:case 0xC3A3:case 0xC3A4:case 0xC3A5:case 0xC3A6:case 0xC3A7:case 0xC3A8:case 0xC3A9:case 0xC3AA:
		case 0xC33F:case 0xC34F:case 0xC35F:case 0xC36F:case 0xC37F:case 0xC38F:case 0xC39F:case 0xC3AF://至少含有一个时段数据
		case 0xC411:case 0xC412:case 0xC413:case 0xC414:case 0xC415:case 0xC416:case 0xC417:case 0xC419:case 0xC41A:case 0xC41B:
		case 0xC41C:case 0xC41D:
			if(nLength < 3)
	{
				nTmp = RET_ERR_ERROR;
	}
			else
			{
				nTmp = RET_ERR_OK;
}
			break;

		case 0xC010:
		case 0xC119:case 0xC11A:
		case 0xC212:
			if(nLength < 4)
{
				nTmp = RET_ERR_ERROR;
			}
			else
			{
				nTmp = RET_ERR_OK;
			}
			break;

		case 0xC31F:
			if(nLength < 5)
	{
				nTmp = RET_ERR_ERROR;
	}
			else
	{
				nTmp = RET_ERR_OK;
	}
			break;

		case 0xC032:case 0xC033:case 0xC034:
		case 0xC050:
		case 0xC21F:
			if(nLength < 6)
	{
				nTmp = RET_ERR_ERROR;
	}
			else
			{
				nTmp = RET_ERR_OK;
}
			break;

		case 0xC01F:
			if(nLength < 7)
{
				nTmp = RET_ERR_ERROR;
			}
			else
			{
				nTmp = RET_ERR_OK;
			}
			break;

		case 0xB63F:
			if(nLength < 16)
	{
				nTmp = RET_ERR_ERROR;
	}
			else
	{
				nTmp = RET_ERR_OK;
	}
			break;

		case 0xC11F:
			if(nLength < 17)
	{
				nTmp = RET_ERR_ERROR;
	}
			else
			{
				nTmp = RET_ERR_OK;
}
			break;

		case 0xC9FF:
			if(nLength < 18)
{
				nTmp = RET_ERR_ERROR;
			}
			else
	{
				nTmp = RET_ERR_OK;
			}
			break;
            
		case 0xC03F:
			if(nLength < 24)
        {
				nTmp = RET_ERR_ERROR;
			}
			else
			{
				nTmp = RET_ERR_OK;
        }
			break;
			
		default:
			nTmp = RET_ERR_ERROR;
		break;
	}
	return  nTmp;
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 规约组侦
*********************************************************************************/
INT8U Dlt07SendResp(const INT8U *pDI01, INT8U *pData, INT8U *pIsExist)
{
    INT8U	i;
	INT8U	nIndex     = 0;
	INT32U	nDI 	   = 0;
	INT8U   nTmpTbl    = 0;
    INT8U   nTmpNum    = 0;
	INT8U   nFsskID    = 0;
	INT8U   nJsskID    = 0;
	INT16U  nTmpOffset = 0;
	INT16U  nDataLen   = 0;
    INT32U  nTmp32U    = 0;
    INT8U   nTmpTime[6];
    INT8U   nProductNum[32] = {0};
    INT8U   nTempBuf[100] = {0};
	BOOLEAN bRet       = FALSE;
	STD_TIME	sTime;
    FORMAT09	sRealP[MAX_PHASE_NUM+1];
    FORMAT09	sRealQ[MAX_PHASE_NUM+1];
    FORMAT15	sDJTime;
	MEM_AC_PRM	sAc_Param;
	MT_DGN_ENERGY_DATA	sAcEn;
	MT_DGN_Q_ENERGY_DATA  sAcQEn;
	MT_DGN_TRI_ENERGY_DATA  sAcTriEn;
	MT_DGN_TRI_Q_EN_DATA  sAcTriQEn;
    MT_DGN_C_ENERGY_DATA sAcCombEnergy;
	MT_DGN_DEMAND_DATA	sAcDe;
	MT_DGN_Q_DE_DATA	sAcQDe;
    MEM_AC_DAY_DATA     sAcHistDayData;
    MEM_AC_RDAY_DATA     sAcHistData;
    MEM_AC_DJ_DATA      sAcDjData;
    MEM_AC_DJ_DATA      sAcdDjData;
    
    memset(&sRealP, 0, sizeof(sRealP));
    memset(&sRealQ, 0, sizeof(sRealQ));
    memset(&sDJTime, 0, sizeof(sDJTime));
    memset(&sAc_Param, 0, sizeof(sAc_Param));
    memset(&sAcEn, 0, sizeof(sAcEn));
    memset(&sAcQEn, 0, sizeof(sAcQEn));
    memset(&sAcTriEn, 0, sizeof(sAcTriEn));
    memset(&sAcTriQEn, 0, sizeof(sAcTriQEn));
    memset(&sAcDe, 0, sizeof(sAcDe));
    memset(&sAcQDe, 0, sizeof(sAcQDe));
    memset(&sAcHistData, 0, sizeof(sAcHistData));
    memset(&sAcHistDayData, 0, sizeof(sAcHistDayData));
    memset(&sAcDjData, 0, sizeof(sAcDjData));
    memset(&sAcdDjData, 0, sizeof(sAcdDjData));
    
    
    if ((0 == pDI01[3]) || (1 == pDI01[3]))
    {
        if (((pDI01[1] > MAX_FEE_NUM) || (pDI01[1] > 63)) && (pDI01[1] != 0xff))
        {
            *pIsExist = 0x40;						//费率数超
			return 0;
        }
        if(pDI01[0] > MAX_AC_DAY_DATA_NUM)
		{
			*pIsExist = 0x02;						//无所招测数据
			return 0;
		}
        
        if (0 == pDI01[3])
        {   
            if (0 == pDI01[2])  //组合有功
            {
                if (0 == pDI01[0])
                {
                    MemReadAcRealData(F5, &sAcCombEnergy, sizeof(sAcCombEnergy));
                }
                else //结算日
                {
                    MemReadAcHistDataProtProc(F2, pDI01[0], &sAcHistData, sizeof(sAcHistData));
                    memcpy(&sAcCombEnergy, &sAcHistData.sAcCombEnergy, sizeof(sAcCombEnergy));
                }
            }
            else if ((pDI01[2] > 0)&&(pDI01[2] <= 4))
            {
                memset(&sAcEn, 0, sizeof(MT_DGN_ENERGY_DATA));
                if(0 == pDI01[0])
				{
                    MemReadAcRealData(F1, &sAcEn, sizeof(sAcEn));
				}
				else
				{
                    MemReadAcHistDataProtProc(F2, pDI01[0], &sAcHistData, sizeof(sAcHistData));
                    memcpy(&sAcEn, &sAcHistData.sAcEnergy, sizeof(sAcEn));
				}
            }
            else if ((pDI01[2] > 4)&&(pDI01[2] <= 8))
            {
                memset(&sAcQEn, 0, sizeof(MT_DGN_Q_ENERGY_DATA));
				if(0 == pDI01[0])
				{
					MemReadAcRealData(F2, &sAcQEn, sizeof(sAcQEn));
				}
				else
				{
                    MemReadAcHistDataProtProc(F2, pDI01[0], &sAcHistData, sizeof(sAcHistData));
                    memcpy(&sAcQEn, &sAcHistData.sAcQEnergy, sizeof(sAcEn));
				}
            }
            else if (((pDI01[2] > 0x14)&&(pDI01[2] <= 0x18))||((pDI01[2] > 0x28)&&(pDI01[2] <= 0x2c))||((pDI01[2] > 0x3c)&&(pDI01[2] <= 0x40)))
			{
                memset(&sAcTriEn, 0, sizeof(MT_DGN_TRI_ENERGY_DATA));
				if(0 == pDI01[0])
				{
					MemReadAcRealData(F3, &sAcTriEn, sizeof(sAcTriEn));
				}
				else
				{
					MemReadAcHistDataProtProc(F2, pDI01[0], &sAcHistData, sizeof(sAcHistData));
                    memcpy(&sAcTriEn, &sAcHistData.sAcTriEnergy, sizeof(sAcTriEn));
				}
            }
            else if (((pDI01[2] > 0x18)&&(pDI01[2] <= 0x1c))||((pDI01[2] > 0x2c)&&(pDI01[2] <= 0x30))||((pDI01[2] > 0x40)&&(pDI01[2] <= 0x44)))
			{
                memset(&sAcTriQEn, 0, sizeof(MT_DGN_TRI_Q_EN_DATA));
				if(0 == pDI01[0])
				{
					MemReadAcRealData(F4, &sAcTriEn, sizeof(sAcTriEn));
				}
				else
				{
					MemReadAcHistDataProtProc(F2, pDI01[0], &sAcHistData, sizeof(sAcHistData));
                    memcpy(&sAcTriQEn, &sAcHistData.sAcTriQEnergy, sizeof(sAcTriQEn));
				}
            }
        }
        else
        {
            if ((pDI01[2] > 0) && (pDI01[2] <= 4))
            {
                memset(&sAcDe, 0, sizeof(MT_DGN_DEMAND_DATA));
				if(0 == pDI01[0])
				{
					MemReadAcRealData(F7, &sAcDe, sizeof(sAcDe));
				}
				else
				{
					MemReadAcHistDataProtProc(F2, pDI01[0], &sAcHistData, sizeof(sAcHistData));
                    memcpy(&sAcDe, &sAcHistData.sAcDemand, sizeof(sAcDe));
				}
            }
            else if ((pDI01[2] > 4) && (pDI01[2] <= 8))
			{
				memset(&sAcQDe, 0, sizeof(MT_DGN_Q_DE_DATA));
				if(0 == pDI01[0])
				{
					MemReadAcRealData(F8, &sAcQDe, sizeof(sAcQDe));
				}
				else
				{
					MemReadAcHistDataProtProc(F2, pDI01[0], &sAcHistData, sizeof(sAcHistData));
                    memcpy(&sAcQDe, &sAcHistData.sAcQDemand, sizeof(sAcQDe));
				}
			}
        }
    }
    else if (4 == pDI01[3])
    {
        if ((1 == pDI01[1]) && ((1 == pDI01[0])||(2 == pDI01[0])||(0xff == pDI01[0])))
        {
            memset(&sTime, 0, sizeof(STD_TIME));
			GetSysClk(&sTime);
        }
        else
		{
			memset(&sAc_Param, 0, sizeof(MEM_AC_PRM));
			MemReadAcPrm(FEND, &sAc_Param, sizeof(sAc_Param));
		}
    }
    else if ((5 == pDI01[3])&&(0 == pDI01[2]))  //定时
    {
        if(pDI01[0] > MAX_AC_DSDJ_DATA_NUM)
		{
			*pIsExist = 0x02;						//无所招测数据
			return 0;
		}
        if((pDI01[1] > 0)&&(pDI01[1] <= 4))
        {
            MemReadAcHistDataProtProc(F4, pDI01[0], &sAcDjData, sizeof(sAcDjData));
            memcpy(&sAcEn, &sAcDjData.sAcEnergy, sizeof(sAcEn));
        }
        else if((pDI01[1] > 4)&&(pDI01[1] <= 8))
		{
			MemReadAcHistDataProtProc(F4, pDI01[0], &sAcDjData, sizeof(sAcDjData));
            memcpy(&sAcQEn, &sAcDjData.sAcQEnergy, sizeof(sAcQEn));
		}
        else if((pDI01[1] > 8)&&(pDI01[1] <= 0x0A))
		{
			MemReadAcHistDataProtProc(F4, pDI01[0], &sAcDjData, sizeof(sAcDjData));
            memcpy(&sAcDe, &sAcDjData.sAcDemand, sizeof(sAcDe));
		}
        else if (0x10 == pDI01[1])
		{
            MemReadAcHistDataProtProc(F4, pDI01[0], &sAcDjData, sizeof(sAcDjData));
            memcpy(&sRealP, &sAcDjData.sRealP, sizeof(sRealP));
            memcpy(&sRealQ, &sAcDjData.sRealQ, sizeof(sRealQ));
		}
        else
		{
			MemReadAcHistDataProtProc(F4, pDI01[0], &sAcDjData, sizeof(sAcDjData));
            memcpy(&sDJTime, &sAcDjData.sTime, sizeof(sDJTime));
		}
    }
    else if ((5 == pDI01[3])&&(1 == pDI01[2]))  //瞬时
    {
        if(pDI01[0] > MAX_AC_SSDJ_DATA_NUM)
		{
			*pIsExist = 0x02;						//无所招测数据
			return 0;
		}
        if((pDI01[1] > 0)&&(pDI01[1] <= 4))
		{
			MemReadAcHistDataProtProc(F3, pDI01[0], &sAcDjData, sizeof(sAcDjData));
            memcpy(&sAcEn, &sAcDjData.sAcEnergy, sizeof(sAcEn));
		}
        else if((pDI01[1] > 4)&&(pDI01[1] <= 8))
		{
			MemReadAcHistDataProtProc(F3, pDI01[0], &sAcDjData, sizeof(sAcDjData));
            memcpy(&sAcQEn, &sAcDjData.sAcQEnergy, sizeof(sAcQEn));
		}
        else if((pDI01[1] > 8)&&(pDI01[1] <= 0x0A))
		{
			MemReadAcHistDataProtProc(F3, pDI01[0], &sAcDjData, sizeof(sAcDjData));
            memcpy(&sAcDe, &sAcDjData.sAcDemand, sizeof(sAcDe));
		}
        else if (0x10 == pDI01[1])
		{
            MemReadAcHistDataProtProc(F3, pDI01[0], &sAcDjData, sizeof(sAcDjData));
            memcpy(&sRealP, &sAcDjData.sRealP, sizeof(sRealP));
            memcpy(&sRealQ, &sAcDjData.sRealQ, sizeof(sRealQ));
		}
        else
		{
			MemReadAcHistDataProtProc(F3, pDI01[0], &sAcDjData, sizeof(sAcDjData));
            memcpy(&sDJTime, &sAcDjData.sTime, sizeof(sDJTime));
		}
    }
    else if ((5 == pDI01[3])&&(2 == pDI01[2]))  //时区
    {
        if(pDI01[0] > MAX_AC_SQDJ_DATA_NUM)
		{
			*pIsExist = 0x02;						//无所招测数据
			return 0;
		}
        if((pDI01[1] > 0)&&(pDI01[1] <= 4))
		{
			MemReadAcHistDataProtProc(F5, pDI01[0], &sAcDjData, sizeof(sAcDjData));
            memcpy(&sAcEn, &sAcDjData.sAcEnergy, sizeof(sAcEn));
		}
        else if((pDI01[1] > 4)&&(pDI01[1] <= 8))
		{
			MemReadAcHistDataProtProc(F5, pDI01[0], &sAcDjData, sizeof(sAcDjData));
            memcpy(&sAcQEn, &sAcDjData.sAcQEnergy, sizeof(sAcQEn));
		}
        else if((pDI01[1] > 8)&&(pDI01[1] <= 0x0A))
		{
			MemReadAcHistDataProtProc(F5, pDI01[0], &sAcDjData, sizeof(sAcDjData));
            memcpy(&sAcDe, &sAcDjData.sAcDemand, sizeof(sAcDe));
		}
        else if (0x10 == pDI01[1])
		{
            MemReadAcHistDataProtProc(F5, pDI01[0], &sAcDjData, sizeof(sAcDjData));
            memcpy(&sRealP, &sAcDjData.sRealP, sizeof(sRealP));
            memcpy(&sRealQ, &sAcDjData.sRealQ, sizeof(sRealQ));
		}
        else
		{
			MemReadAcHistDataProtProc(F5, pDI01[0], &sAcDjData, sizeof(sAcDjData));
            memcpy(&sDJTime, &sAcDjData.sTime, sizeof(sDJTime));
		}
    }
    else if ((5 == pDI01[3])&&(3 == pDI01[2]))  //时段
    {
        if(pDI01[0] > MAX_AC_SDDJ_DATA_NUM)
		{
			*pIsExist = 0x02;						//无所招测数据
			return 0;
		}
        if((pDI01[1] > 0)&&(pDI01[1] <= 4))
		{
			MemReadAcHistDataProtProc(F6, pDI01[0], &sAcDjData, sizeof(sAcDjData));
            memcpy(&sAcEn, &sAcDjData.sAcEnergy, sizeof(sAcEn));
		}
        else if((pDI01[1] > 4)&&(pDI01[1] <= 8))
		{
			MemReadAcHistDataProtProc(F6, pDI01[0], &sAcDjData, sizeof(sAcDjData));
            memcpy(&sAcQEn, &sAcDjData.sAcQEnergy, sizeof(sAcQEn));
		}
        else if((pDI01[1] > 8)&&(pDI01[1] <= 0x0A))
		{
			MemReadAcHistDataProtProc(F6, pDI01[0], &sAcDjData, sizeof(sAcDjData));
            memcpy(&sAcDe, &sAcDjData.sAcDemand, sizeof(sAcDe));
		}
        else if (0x10 == pDI01[1])
		{
            MemReadAcHistDataProtProc(F6, pDI01[0], &sAcDjData, sizeof(sAcDjData));
            memcpy(&sRealP, &sAcDjData.sRealP, sizeof(sRealP));
            memcpy(&sRealQ, &sAcDjData.sRealQ, sizeof(sRealQ));
		}
        else
		{
			MemReadAcHistDataProtProc(F6, pDI01[0], &sAcDjData, sizeof(sAcDjData));
            memcpy(&sDJTime, &sAcDjData.sTime, sizeof(sDJTime));
		}
    }
    else if ((5 == pDI01[3])&&(6 == pDI01[2]))
    {
        if(pDI01[0] > MAX_AC_DAY_DATA_NUM)
		{
			*pIsExist = 0x02;						//无所招测数据
			return 0;
		}
        if((pDI01[1] > 0)&&(pDI01[1] <= 4))
		{
			MemReadAcHistDataProtProc(F1, pDI01[0], &sAcHistDayData, sizeof(sAcHistDayData));
            memcpy(&sAcEn, &sAcHistDayData.sAcEnergy, sizeof(sAcEn));
		}
        else if((pDI01[1] > 4)&&(pDI01[1] <= 8))
		{
			MemReadAcHistDataProtProc(F1, pDI01[0], &sAcHistDayData, sizeof(sAcHistDayData));
            memcpy(&sAcQEn, &sAcHistDayData.sAcQEnergy, sizeof(sAcQEn));
		}
        else if((pDI01[1] > 8)&&(pDI01[1] <= 0x0A))
		{
			memset(&sAcDe, 0, sizeof(MT_DGN_DEMAND_DATA));
			MemReadAcHistDataProtProc(F1, pDI01[0], &sAcHistDayData, sizeof(sAcHistDayData));
            memcpy(&sAcDe, &sAcHistDayData.sAcDemand, sizeof(sAcDe));
		}
        else if (0x10 == pDI01[1])
		{
			MemReadAcHistDataProtProc(F1, pDI01[0], &sAcHistDayData, sizeof(sAcHistDayData));
            memcpy(&sRealP, &sAcHistDayData.sRealP, sizeof(sRealP));
            memcpy(&sRealQ, &sAcHistDayData.sRealQ, sizeof(sRealQ));
		}
        else
		{
            //MemReadAcHistDataProc(F1, pDI01[0], &sAcHistDayData, sizeof(sAcHistDayData));
			MemReadAcHistDataProtProc(F1, pDI01[0], &sAcHistDayData, sizeof(sAcHistDayData));
            memcpy(&sDJTime, &sAcHistDayData.sTime, sizeof(sDJTime));
		}
    }
    
    pData[nIndex++] = pDI01[0];
	pData[nIndex++] = pDI01[1];
	pData[nIndex++] = pDI01[2];
	pData[nIndex++] = pDI01[3];
	nDI = ((pDI01[3] << 24) | (pDI01[2] << 16) | (pDI01[1] << 8) | pDI01[0]);
    
    
    //新增事件 0x10--失压 0x13--断相 0x14--电压逆相序 0x18--失流
    if ((0x10 == pDI01[3]) || (0x13 == pDI01[3]) || (0x14 == pDI01[3]) ||(0x18 == pDI01[3]))
    {
        memset(&g_nAcEventBuf[0], 0, sizeof(g_nAcEventBuf));
        if (((pDI01[0] == 0) || (pDI01[1] == 0)) || ((pDI01[2] == 0) && (pDI01[3] != 0x14)))
        {
            nDataLen = 3; //默认三个字节
			switch (nDI)
            {
                case 0x10000001:		//失压总次数
                    MemReadAcXFlhBackData(F25, &nTmp32U, sizeof(nTmp32U));
					break;	
                case 0x10000002:		//失压总累计时间
					MemReadAcXBackData(F1, &nTmp32U, sizeof(nTmp32U));
					nTmp32U = (nTmp32U / 60);
					break;
                case 0x10000101: 		//最近1次失压发生时刻
					MemReadAcXFlhBackData(F26, &nTmpTime[0], sizeof(nTmpTime));
					nDataLen = 6;
					break;
                case 0x10000201:		//最近1次失压结束时刻
					MemReadAcXFlhBackData(F27, &nTmpTime[0], sizeof(nTmpTime));
					nDataLen = 6;
					break;	
                case 0x10010001:		//A相失压总次数
					MemReadAcXFlhBackData(F28, &nTmp32U, sizeof(nTmp32U));
					break;	
                case 0x10010002:		//A相失压总累计时间
					MemReadAcXBackData(F2, &nTmp32U, sizeof(nTmp32U));
					nTmp32U = (nTmp32U / 60);
					break;
                case 0x10020001:		//B相失压总次数
					MemReadAcXFlhBackData(F29, &nTmp32U, sizeof(nTmp32U));
					break;	
                case 0x10020002:		//B相失压总累计时间
					MemReadAcXBackData(F3, &nTmp32U, sizeof(nTmp32U));
					nTmp32U = (nTmp32U / 60);
					break;
                case 0x10030001:		//C相失压总次数
					MemReadAcXFlhBackData(F30, &nTmp32U, sizeof(nTmp32U));
					break;		
				case 0x10030002:		//C相失压总累计时间
					MemReadAcXBackData(F4, &nTmp32U, sizeof(nTmp32U));
					nTmp32U = (nTmp32U / 60);
					break;
                
                
                case 0x12010001:		//A相过压总次数
					MemReadAcXFlhBackData(F52, &nTmp32U, sizeof(nTmp32U));
					break;	
                case 0x12010002:		//A相过压总累计时间
					MemReadAcXBackData(F15, &nTmp32U, sizeof(nTmp32U));
					nTmp32U = (nTmp32U / 60);
					break;
                case 0x12020001:		//B相过压总次数
					MemReadAcXFlhBackData(F53, &nTmp32U, sizeof(nTmp32U));
					break;	
                case 0x12020002:		//B相过压总累计时间
					MemReadAcXBackData(F16, &nTmp32U, sizeof(nTmp32U));
					nTmp32U = (nTmp32U / 60);
					break;
                case 0x12030001:		//C相过压总次数
					MemReadAcXFlhBackData(F54, &nTmp32U, sizeof(nTmp32U));
					break;		
				case 0x12030002:		//C相过压总累计时间
					MemReadAcXBackData(F17, &nTmp32U, sizeof(nTmp32U));
					nTmp32U = (nTmp32U / 60);
					break;
                case 0x13010001:		//A相断相总次数
					MemReadAcXFlhBackData(F32, &nTmp32U, sizeof(nTmp32U));
					break;
				case 0x13010002:		//A相断相总累计时间
					MemReadAcXBackData(F5, &nTmp32U, sizeof(nTmp32U));
					nTmp32U = (nTmp32U / 60);
					break;
                case 0x13020001:		//B相断相总次数
					MemReadAcXFlhBackData(F33, &nTmp32U, sizeof(nTmp32U));
					break;					
				case 0x13020002:		//B相断相总累计时间
					MemReadAcXBackData(F6, &nTmp32U, sizeof(nTmp32U));
					nTmp32U = (nTmp32U / 60);
					break;
                case 0x13030001:		//C相断相总次数
					MemReadAcXFlhBackData(F34, &nTmp32U, sizeof(nTmp32U));
					break;						
				case 0x13030002:		//C相断相总累计时间
					MemReadAcXBackData(F7, &nTmp32U, sizeof(nTmp32U));
					nTmp32U = (nTmp32U / 60);
					break;
                case 0x14000001:		//电压逆相序总次数
					MemReadAcXFlhBackData(F36, &nTmp32U, sizeof(nTmp32U));
					break;					
				case 0x14000002:		//电压逆相序总累计时间
					MemReadAcXBackData(F9, &nTmp32U, sizeof(nTmp32U));
					nTmp32U = (nTmp32U / 60);
					break;
                case 0x18010001:		//A相失流总次数
					MemReadAcXFlhBackData(F38, &nTmp32U, sizeof(nTmp32U));
					break;					
				case 0x18010002:		//A相失流总累计时间
					MemReadAcXBackData(F10, &nTmp32U, sizeof(nTmp32U));
					nTmp32U = (nTmp32U / 60);
					break;	
                case 0x18020001:		//B相失流总次数
					MemReadAcXFlhBackData(F39, &nTmp32U, sizeof(nTmp32U));
					break;						
				case 0x18020002:		//B相失流总累计时间
					MemReadAcXBackData(F11, &nTmp32U, sizeof(nTmp32U));
					nTmp32U = (nTmp32U / 60);
					break;					
				case 0x18030001:		//C相失流总次数
					MemReadAcXFlhBackData(F40, &nTmp32U, sizeof(nTmp32U));
					break;						
				case 0x18030002:		//C相失流总累计时间
					MemReadAcXBackData(F12, &nTmp32U, sizeof(nTmp32U));
					nTmp32U = (nTmp32U / 60);
					break;
                case 0x19010001:		//A相过流总次数
					MemReadAcXFlhBackData(F61, &nTmp32U, sizeof(nTmp32U));
					break;					
				case 0x19010002:		//A相过流总累计时间
					MemReadAcXBackData(F19, &nTmp32U, sizeof(nTmp32U));
					nTmp32U = (nTmp32U / 60);
					break;	
                case 0x19020001:		//B相过流总次数
					MemReadAcXFlhBackData(F62, &nTmp32U, sizeof(nTmp32U));
					break;						
				case 0x19020002:		//B相过流总累计时间
					MemReadAcXBackData(F20, &nTmp32U, sizeof(nTmp32U));
					nTmp32U = (nTmp32U / 60);
					break;					
				case 0x19030001:		//C相过流总次数
					MemReadAcXFlhBackData(F63, &nTmp32U, sizeof(nTmp32U));
					break;						
				case 0x19030002:		//C相过流总累计时间
					MemReadAcXBackData(F21, &nTmp32U, sizeof(nTmp32U));
					nTmp32U = (nTmp32U / 60);
					break;
                case 0x1B010001:		//A相潮流反向总次数
					MemReadAcXFlhBackData(F70, &nTmp32U, sizeof(nTmp32U));
					break;					
				case 0x1B010002:		//A相潮流反向总累计时间
					MemReadAcXBackData(F23, &nTmp32U, sizeof(nTmp32U));
					nTmp32U = (nTmp32U / 60);
					break;
                case 0x1B020001:		//B相潮流反向总次数
					MemReadAcXFlhBackData(F71, &nTmp32U, sizeof(nTmp32U));
					break;						
				case 0x1B020002:		//B相潮流反向总累计时间
					MemReadAcXBackData(F24, &nTmp32U, sizeof(nTmp32U));
					nTmp32U = (nTmp32U / 60);
					break;					
				case 0x1B030001:		//C相潮流反向总次数
					MemReadAcXFlhBackData(F72, &nTmp32U, sizeof(nTmp32U));
					break;						
				case 0x1B030002:		//C相潮流反向总累计时间
					MemReadAcXBackData(F25, &nTmp32U, sizeof(nTmp32U));
					nTmp32U = (nTmp32U / 60);
					break;
                case 0x1C010001:		//A相过载总次数
					MemReadAcXFlhBackData(F79, &nTmp32U, sizeof(nTmp32U));
					break;					
				case 0x1C010002:		//A相过载总累计时间
					MemReadAcXBackData(F27, &nTmp32U, sizeof(nTmp32U));
					nTmp32U = (nTmp32U / 60);
					break;	
                case 0x1C020001:		//B相过载总次数
					MemReadAcXFlhBackData(F80, &nTmp32U, sizeof(nTmp32U));
					break;						
				case 0x1C020002:		//B相过载总累计时间
					MemReadAcXBackData(F28, &nTmp32U, sizeof(nTmp32U));
					nTmp32U = (nTmp32U / 60);
					break;					
				case 0x1C030001:		//C相过载总次数
					MemReadAcXFlhBackData(F81, &nTmp32U, sizeof(nTmp32U));
					break;						
				case 0x1C030002:		//C相过载总累计时间
					MemReadAcXBackData(F29, &nTmp32U, sizeof(nTmp32U));
					nTmp32U = (nTmp32U / 60);
					break;
                
                default:
			    	*pIsExist = 0x02;	//无所招测数据
					return 0;   	
            }
            if (3 == nDataLen)
			{
				INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[0]);
				memcpy(pData + nIndex, &g_nAcEventBuf[0], nDataLen);
			}
            else
			{
				memcpy(pData + nIndex, &nTmpTime[0], nDataLen);
			}
            nIndex += nDataLen;
        }
        else
        {
            bRet = TRUE;
        	nTmpNum = 0;
        	if ((0x10 == pDI01[3]) || (0x13 == pDI01[3]) ||(0x18 == pDI01[3]))
            {
                switch(pDI01[2]) //A/B/C相
                {
                    case 0x01:	 //A相
                        if (0x10 == pDI01[3]) //失压
	                	{
	                		nTmpTbl = F1;
	                		nTmpNum = MAX_AC_SY_NUM;
	                    }
                        if (0x12 == pDI01[3]) //过压
	                	{
	                		nTmpTbl = F18;
	                		nTmpNum = MAX_AC_GY_NUM;
	                    }
                        if (0x13 == pDI01[3]) //断相
	                	{
	                		nTmpTbl = F4;
	                		nTmpNum = MAX_AC_DX_NUM;
	                    }
                        if (0x18 == pDI01[3]) //失流
	                	{
	                		nTmpTbl = F9;
	                		nTmpNum = MAX_AC_SL_NUM;
	                    }
                        if (0x19 == pDI01[3]) //过流
	                	{
	                		nTmpTbl = F21;
	                		nTmpNum = MAX_AC_GL_NUM;
	                    }
                        if (0x1B == pDI01[3]) //潮流反向
	                	{
	                		nTmpTbl = F24;
	                		nTmpNum = MAX_AC_CL_NUM;
	                    }
                        if (0x1C == pDI01[3]) //过载
	                	{
	                		nTmpTbl = F27;
	                		nTmpNum = MAX_AC_GZ_NUM;
	                    }
	                	break;
                    case 0x02:
	                	if (0x10 == pDI01[3]) //失压
	                	{
	                		nTmpTbl = F2;
	                		nTmpNum = MAX_AC_SY_NUM;
	                    }
                        if (0x12 == pDI01[3]) //过压
	                	{
	                		nTmpTbl = F19;
	                		nTmpNum = MAX_AC_GY_NUM;
	                    }
	
	                    if (0x13 == pDI01[3]) //断相
	                	{
	                		nTmpTbl = F5;
	                		nTmpNum = MAX_AC_DX_NUM;
	                    }
	
	                    if (0x18 == pDI01[3]) //失流
	                	{
	                		nTmpTbl = F10;
	                		nTmpNum = MAX_AC_SL_NUM;
	                    }
                        if (0x19 == pDI01[3]) //过流
	                	{
	                		nTmpTbl = F22;
	                		nTmpNum = MAX_AC_GL_NUM;
	                    }
                        if (0x1B == pDI01[3]) //潮流反向
	                	{
	                		nTmpTbl = F25;
	                		nTmpNum = MAX_AC_CL_NUM;
	                    }
                        if (0x1C == pDI01[3]) //过载
	                	{
	                		nTmpTbl = F28;
	                		nTmpNum = MAX_AC_GZ_NUM;
	                    }
	                	break;
	                case 0x03:
	                	if (0x10 == pDI01[3]) //失压
	                	{
	                		nTmpTbl = F3;
	                		nTmpNum = MAX_AC_SY_NUM;
	                    }
                        if (0x12 == pDI01[3]) //过压
	                	{
	                		nTmpTbl = F20;
	                		nTmpNum = MAX_AC_GY_NUM;
	                    }
	
	                    if (0x13 == pDI01[3]) //断相
	                	{
	                		nTmpTbl = F6;
	                		nTmpNum = MAX_AC_DX_NUM;
	                    }
	
	                    if (0x18 == pDI01[3]) //失流
	                	{
	                		nTmpTbl = F11;
	                		nTmpNum = MAX_AC_SL_NUM;
	                    }
                        if (0x19 == pDI01[3]) //过流
	                	{
	                		nTmpTbl = F23;
	                		nTmpNum = MAX_AC_GL_NUM;
	                    }
                        if (0x1B == pDI01[3]) //潮流反向
	                	{
	                		nTmpTbl = F26;
	                		nTmpNum = MAX_AC_CL_NUM;
	                    }
                        if (0x1C == pDI01[3]) //过载
	                	{
	                		nTmpTbl = F29;
	                		nTmpNum = MAX_AC_GZ_NUM;
	                    }
	                	break;
	                default:
	                	bRet = FALSE;
	                	break;
                }
            }
            if (0x14 == pDI01[3])
			{
				nTmpTbl = F8;
                nTmpNum = MAX_AC_DYNX_NUM;
			}
            if ((pDI01[0] < 1) || ((pDI01[0] > 10) && (pDI01[0] != 0xFF))) 			//上几次
            {
            	bRet = FALSE;
            }
            if ((pDI01[1] < 1) || ((pDI01[1] > nTmpNum) && (pDI01[1] != 0xFF))) 	//数据项
            {
            	bRet = FALSE;
            }
            if (bRet)
            {
                Dlt07GetACEventLen(nTmpTbl, 0xFF, &nDataLen, &nTmpOffset);
                if (pDI01[0] != 0xFF)
                {
                    MemReadAcEvtProc(nTmpTbl, pDI01[0], &g_nAcEventBuf[0], nDataLen);
                    if (pDI01[1] != 0xFF)
                    {
                        Dlt07GetACEventLen(nTmpTbl, pDI01[1], &nDataLen, &nTmpOffset);
                        memcpy(pData + nIndex, &g_nAcEventBuf[nTmpOffset], nDataLen);
						nIndex += nDataLen;
                    }
                    if (pDI01[1] == 0xFF)
					{
						memcpy(pData + nIndex, &g_nAcEventBuf[0], nDataLen);
						nIndex += nDataLen;
					}
                }
                else
                {
                    for (i = 1; i < 11; i++) //10次
                    {
                        memset(&g_nAcEventBuf[0], 0, sizeof(g_nAcEventBuf));
                        MemReadAcEvtProc(nTmpTbl, i, &g_nAcEventBuf[0], nDataLen);
                        if (pDI01[1] != 0xFF)
                        {
                            Dlt07GetACEventLen(nTmpTbl, pDI01[1], &nDataLen, &nTmpOffset);
			                memcpy(pData + nIndex, &g_nAcEventBuf[nTmpOffset], nDataLen);
							nIndex += nDataLen;
                        }
                        if (pDI01[1] == 0xFF)
						{
							*pIsExist = 0x01;	//其他错误---终端暂不支持大数据量传输上10次所有数据
							return 0;
					    }
                    }
                }
            }
            else
		    {
				*pIsExist = 0x02;						//无所招测数据
				return 0;
		    }
        }
    }
    
    else if (0x03 == pDI01[3])			//03050001 全失压 03110001 掉电 03300100 清零
    {
        memset(&g_nAcEventBuf[0], 0, sizeof(g_nAcEventBuf));
		if (pDI01[0] > 0x0A)    	//最大只有上10次
		{
			*pIsExist = 0x02;		//无所招测数据
			return 0;
		}
        
        //03010001 失压 03040000 断相  030B0000 失流  03070000 逆相序
		if ((0x01 == pDI01[2]) || (0x04 == pDI01[2]) || (0x0B == pDI01[2]) || (0x07 == pDI01[2]))
        {
            bRet = TRUE;
			if ((0x01 == pDI01[2]) || (0x04 == pDI01[2]) || (0x0B == pDI01[2]))
            {
                switch (pDI01[1])
                {
                    case 0x01:	//A相
						if (0x01 == pDI01[2]) //失压
						{
							nTmpTbl = F1;
						}
                        if (0x03 == pDI01[2]) //过压
						{
							nTmpTbl = F15;
						}
						
						if (0x04 == pDI01[2]) //断相
						{
							nTmpTbl = F4;
						}
						
						if (0x0B == pDI01[2]) //失流
						{
							nTmpTbl = F9;
						}
						break;
					case 0x02:	//B相
						if (0x01 == pDI01[2]) //失压
						{
							nTmpTbl = F2;
						}
                        if (0x03 == pDI01[2]) //过压
						{
							nTmpTbl = F15;
						}
						
						if (0x04 == pDI01[2]) //断相
						{
							nTmpTbl = F5;
						}
						
						if (0x0B == pDI01[2]) //失流
						{
							nTmpTbl = F10;
						}
						break;
					case 0x03:	//C相
						if (0x01 == pDI01[2]) //失压
						{
							nTmpTbl = F3;
						}
                        if (0x03 == pDI01[2]) //过压
						{
							nTmpTbl = F15;
						}
						
						if (0x04 == pDI01[2]) //断相
						{
							nTmpTbl = F6;
						}
						
						if (0x0B == pDI01[2]) //失流
						{
							nTmpTbl = F11;
						}
						break;
				     default:
				     	bRet = FALSE;
				     	break;
                }
            }
            if (0x07 == pDI01[2])
			{
				nTmpTbl = F8;
			}
            if (!bRet)
            {
				*pIsExist = 0x02;		//无所招测数据
				return 0;
            }
            else
            {
                if (pDI01[0] > 0) //上几次
                {
                    if (0x0B == pDI01[2]) //失流
					{
						nTmpNum = 0x20;
						nFsskID = 0x01;
						nJsskID = 0x21;
					}
					
					if ((0x01 == pDI01[2]) || (0x04 == pDI01[2])) //失压和断相
					{
						nTmpNum = 0x24;
						nFsskID = 0x01;
						nJsskID = 0x25;
					}
					
					if (0x07 == pDI01[2]) //逆相序
					{
						nTmpNum = 0x11;
						nFsskID = 0x01;
						nJsskID = 0x12;
					}
                    
                    Dlt07GetACEventLen(nTmpTbl, 0xFF, &nDataLen, &nTmpOffset);
                    MemReadAcEvtProc(nTmpTbl, pDI01[0], &g_nAcEventBuf[0], nDataLen);
                    
                    //发生时刻
		            Dlt07GetACEventLen(nTmpTbl, nFsskID, &nDataLen, &nTmpOffset);
		            memcpy(pData + nIndex, &g_nAcEventBuf[nTmpOffset], nDataLen);
					nIndex += nDataLen;
                    
                    //结束时刻
		            Dlt07GetACEventLen(nTmpTbl, nJsskID, &nDataLen, &nTmpOffset);
		            memcpy(pData + nIndex, &g_nAcEventBuf[nTmpOffset], nDataLen);
					nIndex += nDataLen;
                    
                    memset(&g_nAcEventBuf[0], 0, sizeof(g_nAcEventBuf));//将增量部分全部清为0
					for (i = 2; i < nTmpNum; i++)
					{
						Dlt07GetACEventLen(nTmpTbl, i, &nDataLen, &nTmpOffset);
						memcpy(pData + nIndex, &g_nAcEventBuf[nTmpOffset], nDataLen);
						nIndex += nDataLen;
					}
                }
                else if (0 == pDI01[0]) //总信息
                {
                    nTmpOffset = 0;
					switch (nDI)
                    {
                        case 0x03010000: //失压
						{
							//A相失压总次数
							MemReadAcXFlhBackData(F28, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//A相失压总累计时间
							MemReadAcXBackData(F2, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//B相失压总次数
							MemReadAcXFlhBackData(F29, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//B相失压总累计时间
							MemReadAcXBackData(F3, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//C相失压总次数
							MemReadAcXFlhBackData(F30, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//C相失压总累计时间
							MemReadAcXBackData(F4, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							break;
						}
                        case 0x03030000: //过压
						{
							//A相过压总次数
							MemReadAcXFlhBackData(F52, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//A相过压总累计时间
							MemReadAcXBackData(F15, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//B相过压总次数
							MemReadAcXFlhBackData(F53, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//B相过压总累计时间
							MemReadAcXBackData(F16, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//C相过压总次数
							MemReadAcXFlhBackData(F54, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//C相过压总累计时间
							MemReadAcXBackData(F17, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							break;
						}
                        case 0x03040000: //断相
						{
							//A相断相总次数
							MemReadAcXFlhBackData(F32, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//A相断相总累计时间
							MemReadAcXBackData(F5, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//B相断相总次数
							MemReadAcXFlhBackData(F33, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//B相断相总累计时间
							MemReadAcXBackData(F6, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//C相断相总次数
							MemReadAcXFlhBackData(F34, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//C相断相总累计时间
							MemReadAcXBackData(F7, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							break;
						}
                        case 0x03070000: //逆相序
						{
							//电压逆相序总次数
							MemReadAcXFlhBackData(F36, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//电压逆相序总累计时间
							MemReadAcXBackData(F9, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;	
							break;
						}
                        case 0x030B0000: //失流
						{
							//A相失流总次数
							MemReadAcXFlhBackData(F38, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//A相失流总累计时间
							MemReadAcXBackData(F10, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//B相失流总次数
							MemReadAcXFlhBackData(F39, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//B相失流总累计时间
							MemReadAcXBackData(F11, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//C相失流总次数
							MemReadAcXFlhBackData(F40, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//C相失流总累计时间
							MemReadAcXBackData(F12, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							break;
						}
                         case 0x030C0000: //过流
						{
							//A相过流总次数
							MemReadAcXFlhBackData(F61, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//A相过流总累计时间
							MemReadAcXBackData(F19, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//B相过流总次数
							MemReadAcXFlhBackData(F62, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//B相过流总累计时间
							MemReadAcXBackData(F20, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//C相过流总次数
							MemReadAcXFlhBackData(F63, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//C相过流总累计时间
							MemReadAcXBackData(F21, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;	
							break;
						}
                        case 0x030E0000: //潮流反向
						{
							//A相潮流反向总次数
							MemReadAcXFlhBackData(F70, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//A相潮流反向总累计时间
							MemReadAcXBackData(F22, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//B相潮流反向总次数
							MemReadAcXFlhBackData(F71, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//B相潮流反向总累计时间
							MemReadAcXBackData(F23, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//C相潮流反向总次数
							MemReadAcXFlhBackData(F72, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//C相潮流反向总累计时间
							MemReadAcXBackData(F24, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							break;
						}
                        case 0x030F0000: //过载
						{
							//A相过载总次数
							MemReadAcXFlhBackData(F79, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//A相过载总累计时间
							MemReadAcXBackData(F25, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//B相过载总次数
							MemReadAcXFlhBackData(F80, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//B相过载总累计时间
							MemReadAcXBackData(F26, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//C相过载总次数
							MemReadAcXFlhBackData(F81, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							//C相过载总累计时间
							MemReadAcXBackData(F27, &nTmp32U, sizeof(nTmp32U));
							INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[nTmpOffset]);
							nTmpOffset+=3;
							break;
						}
                        
						default:
							*pIsExist = 0x02;		//无所招测数据
							return 0;	
                    }
                    if (nTmpOffset > 0)
					{
						memcpy(pData + nIndex, &g_nAcEventBuf[0], nTmpOffset);
						nIndex += nTmpOffset;
					}
					else
					{
						*pIsExist = 0x02;	//无所招测数据
						return 0;
					}
                }
            }
        }
        else if (0x05 == pDI01[2]) 	//03050001 全失压
        {
            nDataLen = 15;
            if (0 == pDI01[0])
            {
                MemReadAcXFlhBackData(F35, &nTmp32U, sizeof(nTmp32U));
                INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[0]);
                MemReadAcXBackData(F8, &nTmp32U, sizeof(nTmp32U));
                INT32U2BCD((nTmp32U/60), 6,	&g_nAcEventBuf[3]);
                nDataLen = 6;
            }
            else
            {
                MemReadAcEvtProc(F7, pDI01[0], &g_nAcEventBuf[0], nDataLen);
            }
        }
        else if (0x11 == pDI01[2]) 	//03110001 掉电
        {
            nDataLen = 12;
            if (0 == pDI01[0])
            {
                MemReadAcXFlhBackData(F41, &nTmp32U, sizeof(nTmp32U));
                INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[0]);
                nDataLen = 3;
            }
            else
            {
                MemReadAcEvtProc(F12, pDI01[0], &g_nAcEventBuf[0], nDataLen);
            }
        }
        else if ((0x30 == pDI01[2]) && (0x00 == pDI01[1])) 	//03300000 编程
        {
            nDataLen = 50;
            if (0 == pDI01[0])
            {
                MemReadAcXFlhBackData(F45, &nTmp32U, sizeof(nTmp32U));
                INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[0]);
                nDataLen = 3;
            }
            else
            {
                MemReadAcEvtProc(F16, pDI01[0], &g_nAcEventBuf[0], nDataLen);
            }
        }
        else if ((0x30 == pDI01[2]) && (0x01 == pDI01[1])) 	//03300100 电表清零
        {
            nDataLen = 106;
            if (0 == pDI01[0])
            {
                MemReadAcXFlhBackData(F43, &nTmp32U, sizeof(nTmp32U));
                INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[0]);
                nDataLen = 3;
            }
            else
            {
                MemReadAcEvtProc(F14, pDI01[0], &g_nAcEventBuf[0], nDataLen);
            }
        }
        else if ((0x30 == pDI01[2]) && (0x02 == pDI01[1])) 	//03300200 需量清零
        {
            nDataLen = 202;
            if (0 == pDI01[0])
            {
                MemReadAcXFlhBackData(F42, &nTmp32U, sizeof(nTmp32U));
                INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[0]);
                nDataLen = 3;
            }
            else
            {
                MemReadAcEvtProc(F13, pDI01[0], &g_nAcEventBuf[0], nDataLen);
            }
        }
        else if ((0x30 == pDI01[2]) && (0x03 == pDI01[1])) 	//03300300 事件清零
        {
            nDataLen = 14;
            if (0 == pDI01[0])
            {
                MemReadAcXFlhBackData(F44, &nTmp32U, sizeof(nTmp32U));
                INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[0]);
                nDataLen = 3;
            }
            else
            {
                MemReadAcEvtProc(F15, pDI01[0], &g_nAcEventBuf[0], nDataLen);
            }
        }
        else if ((0x30 == pDI01[2]) && (0x04 == pDI01[1])) 	//03300400 校时记录
        {
            nDataLen = 16;
            if (0 == pDI01[0])
            {
                MemReadAcXFlhBackData(F46, &nTmp32U, sizeof(nTmp32U));
                INT32U2BCD(nTmp32U, 6,	&g_nAcEventBuf[0]);
                nDataLen = 3;
            }
            else
            {
                MemReadAcEvtProc(F17, pDI01[0], &g_nAcEventBuf[0], nDataLen);
            }
        }
        else if ((0x30 == pDI01[2]) && (0x0D == pDI01[1])) 	//033000D00 开盖记录
        {
            nDataLen = 60;
            if (0 == pDI01[0])
            {
               
            }
            else
            {               
                MemReadAcEvtProc(F30, pDI01[0], &g_nAcEventBuf[0], nDataLen);
            }
        }
        if (nDataLen > 0)
        {
            memcpy(pData + nIndex, &g_nAcEventBuf[nTmpOffset], nDataLen);
            nIndex += nDataLen;
        }
        else
        {
            *pIsExist = 0x02;	//无所招测数据
            return 0;
        }
    }
    else if (0x1D == pDI01[3])			//跳闸事件
    {
        if ((0x00 == pDI01[2]) && (0x01 == pDI01[0])) 	
        {
            nDataLen = 60;
            if (0 == pDI01[1])
            {
                //memset(&g_nAcEventBuf,0,sizeof(LEN_AC_EVENT));//cishu有问题后面在改
                MemReadAcXFlhBackData(F88, &g_nAcEventBuf[0], nDataLen);
                memcpy(pData + nIndex, &g_nAcEventBuf[0], nDataLen);
                nIndex += nDataLen;
            }
            else if(0x01 == pDI01[1])//1D 00 01 01 名称:（上1次）跳闸发生时刻
            {
                //memcpy(pData + nIndex, &sAcCombEnergy.sPPEnergy[pDI01[1]], sizeof(FORMAT01));
                MemReadAcEvtProc(F31, pDI01[0], &g_nAcEventBuf[0], nDataLen);
                memcpy(pData + nIndex, &g_nAcEventBuf[0], sizeof(FORMAT01));
                nIndex += sizeof(FORMAT01);
            }
            else if(0x02 == pDI01[1])//1D 00 02 01 名称:（上1次）跳闸操作者代码
            {
                MemReadAcEvtProc(F31, pDI01[0], &g_nAcEventBuf[0], nDataLen);
                memcpy(pData + nIndex, &g_nAcEventBuf[6], 6);
                nIndex += 6;
            }
            else if(0x03 == pDI01[1])// 1D 00 03 01 名称:（上1次）跳闸时正向有功总电能
            {
                MemReadAcEvtProc(F31, pDI01[0], &g_nAcEventBuf[0], nDataLen);
                memcpy(pData + nIndex, &g_nAcEventBuf[12], 4);
                nIndex += 4;
            }
            else if(0x04 == pDI01[1])//  1D 00 04 01 名称:（上1次）跳闸时反向有功总电能
            {
                MemReadAcEvtProc(F31, pDI01[0], &g_nAcEventBuf[0], nDataLen);
                memcpy(pData + nIndex, &g_nAcEventBuf[16], 4);
                nIndex += 4;
            }
            else if(0x05 == pDI01[1])//  1D 00 05 01 名称:（上1次）跳闸时第一象限无功总电能
            {
                MemReadAcEvtProc(F31, pDI01[0], &g_nAcEventBuf[0], nDataLen);
                memcpy(pData + nIndex, &g_nAcEventBuf[20], 4);
                nIndex += 4;
            }
            else if(0x06 == pDI01[1])//  1D 00 06 01 名称:（上1次）跳闸时第二象限无功总电能
            {
                MemReadAcEvtProc(F31, pDI01[0], &g_nAcEventBuf[0], nDataLen);
                memcpy(pData + nIndex, &g_nAcEventBuf[24], 4);
                nIndex += 4;
            }
            else if(0x07 == pDI01[1])//  1D 00 07 01 名称:（上1次）跳闸时第三象限无功总电能
            {
                MemReadAcEvtProc(F31, pDI01[0], &g_nAcEventBuf[0], nDataLen);
                memcpy(pData + nIndex, &g_nAcEventBuf[28], 4);
                nIndex += 4;
            }
            else if(0x08 == pDI01[1])//  1D 00 08 01 名称:（上1次）跳闸时第四象限无功总电能
            {
                MemReadAcEvtProc(F31, pDI01[0], &g_nAcEventBuf[0], nDataLen);
                memcpy(pData + nIndex, &g_nAcEventBuf[32], 4);
                nIndex += 4;
            }
            else//待修改，数据块
            {               
                MemReadAcEvtProc(F31, pDI01[0], &g_nAcEventBuf[0], nDataLen);
                memcpy(pData + nIndex, &g_nAcEventBuf[0], 36);
                nIndex += 36;
            }
        } 
        else if((0x00 == pDI01[2]) && (0xFF == pDI01[0]))//1D 00 01 FF 名称:跳闸发生时刻数据块//待添加数据块
        {
            nDataLen = 60;
            MemReadAcEvtProc(F31, pDI01[0], &g_nAcEventBuf[0], nDataLen);
            if (nDataLen > 0)
            {
              memcpy(pData + nIndex, &g_nAcEventBuf[nTmpOffset], nDataLen);
              nIndex += nDataLen;
            }
        }
        
        else
        {
            *pIsExist = 0x02;	//无所招测数据
            return 0;
        }
    }
    else if (0x1E == pDI01[3])	//合闸事件
    {
        if ((0x00 == pDI01[2]) && (0x01 == pDI01[0])) 	
        {
            nDataLen = 60;
            if (0x00 == pDI01[1])
            {
                MemReadAcXFlhBackData(F90, &g_nAcEventBuf[0], nDataLen);
                memcpy(pData + nIndex, &g_nAcEventBuf[0], nDataLen);
                nIndex += nDataLen;
            }
            else if(0x01 == pDI01[1])
            {
                //memcpy(pData + nIndex, &sAcCombEnergy.sPPEnergy[pDI01[1]], sizeof(FORMAT01));
                MemReadAcEvtProc(F32, pDI01[0], &g_nAcEventBuf[0], nDataLen);
                memcpy(pData + nIndex, &g_nAcEventBuf[0], sizeof(FORMAT01));
                nIndex += sizeof(FORMAT01);
            }
            else if(0x02 == pDI01[1])
            {
                MemReadAcEvtProc(F32, pDI01[0], &g_nAcEventBuf[0], nDataLen);
                memcpy(pData + nIndex, &g_nAcEventBuf[6], 6);
                nIndex += 6;
            }
            else if(0x03 == pDI01[1])
            {
                MemReadAcEvtProc(F32, pDI01[0], &g_nAcEventBuf[0], nDataLen);
                memcpy(pData + nIndex, &g_nAcEventBuf[12], 4);
                nIndex += 4;
            }
            else if(0x04 == pDI01[1])
            {
                MemReadAcEvtProc(F32, pDI01[0], &g_nAcEventBuf[0], nDataLen);
                memcpy(pData + nIndex, &g_nAcEventBuf[16], 4);
                nIndex += 4;
            }
            else if(0x05 == pDI01[1])
            {
                MemReadAcEvtProc(F32, pDI01[0], &g_nAcEventBuf[0], nDataLen);
                memcpy(pData + nIndex, &g_nAcEventBuf[20], 4);
                nIndex += 4;
            }
            else if(0x06 == pDI01[1])
            {
                MemReadAcEvtProc(F32, pDI01[0], &g_nAcEventBuf[0], nDataLen);
                memcpy(pData + nIndex, &g_nAcEventBuf[24], 4);
                nIndex += 4;
            }
            else if(0x07 == pDI01[1])
            {
                MemReadAcEvtProc(F32, pDI01[0], &g_nAcEventBuf[0], nDataLen);
                memcpy(pData + nIndex, &g_nAcEventBuf[28], 4);
                nIndex += 4;
            }
            else if(0x08 == pDI01[1])
            {
                MemReadAcEvtProc(F32, pDI01[0], &g_nAcEventBuf[0], nDataLen);
                memcpy(pData + nIndex, &g_nAcEventBuf[32], 4);
                nIndex += 4;
            }
        }
        else
        {
            *pIsExist = 0x02;	//无所招测数据
            return 0;
        }
    }
        
    else
    {
        switch(nDI&0xffff0000)
        {
            //召测当前电能示值
            case 0x00000000:
                if (0xff == pDI01[1])
                {   
                    if (g_MeterAcErrFlg[0])
                    {
                      for (i=0; i<(MAX_FEE_NUM+1); i++)
                      {
                            memcpy(&nTempBuf[0], &sAcCombEnergy.sPPEnergy[i], sizeof(FORMAT11));
                            if (i < 2)
                            {
                                nTempBuf[0] = 0x66;
                                nTempBuf[3] = 0x78;
                            }
                            memcpy(pData + nIndex, &nTempBuf[0], sizeof(FORMAT11));
                            nIndex += sizeof(FORMAT11);
                      }
                    }
                    else if (g_MeterAcErrFlg[3])
                    {
                      for (i=0; i<(MAX_FEE_NUM+1); i++)
                      {
                            memcpy(&nTempBuf[0], &sAcCombEnergy.sPPEnergy[i], sizeof(FORMAT11));
                            if (0 == i)
                            {
                                nTempBuf[0] = 0x66;
                                nTempBuf[1] = 0x34;
                                nTempBuf[2] = 0x05;
                            }
                            else if(1 == i)
                            {
                                nTempBuf[0] = 0x33;
                                nTempBuf[1] = 0x67;
                                nTempBuf[2] = 0x04;
                            }
                            memcpy(pData + nIndex, &nTempBuf[0], sizeof(FORMAT11));
                            nIndex += sizeof(FORMAT11);
                      }
                    }
                    else
                    {
                        memcpy(pData + nIndex, &sAcCombEnergy.sPPEnergy[0], (MAX_FEE_NUM+1)*sizeof(FORMAT11));
                        nIndex += ((MAX_FEE_NUM + 1) * sizeof(FORMAT11));
                    }
                }
                else
                {
                    if (g_MeterAcErrFlg[0])
                    {
                        memcpy(&nTempBuf[0], &sAcCombEnergy.sPPEnergy[pDI01[1]], sizeof(FORMAT11));
                        if (pDI01[1] < 2)
                        {
                            nTempBuf[0] = 0x66;
                            nTempBuf[3] = 0x78;
                        }
                        memcpy(pData + nIndex, &nTempBuf[0], sizeof(FORMAT11));
                    }
                    else if (g_MeterAcErrFlg[3])
                    {
                      for (i=0; i<(MAX_FEE_NUM+1); i++)
                      {
                         memcpy(&nTempBuf[0], &sAcCombEnergy.sPPEnergy[pDI01[1]], sizeof(FORMAT11));
                        if (0 == i)
                        {
                            nTempBuf[0] = 0x66;
                            nTempBuf[1] = 0x34;
                            nTempBuf[2] = 0x05;
                        }
                        else if(1 == i)
                        {
                            nTempBuf[0] = 0x33;
                            nTempBuf[1] = 0x67;
                            nTempBuf[2] = 0x04;
                        }
                        memcpy(pData + nIndex, &nTempBuf[0], sizeof(FORMAT11));
                      }
                    }
                    else
                    {
                        memcpy(pData + nIndex, &sAcCombEnergy.sPPEnergy[pDI01[1]], sizeof(FORMAT11));
                    }
                    
                    nIndex += sizeof(FORMAT11);
                }
                break;
            case 0x00010000:
                if (0xff == pDI01[1])
                {
                   if (g_MeterAcErrFlg[0])
                    {
                      for (i=0; i<(MAX_FEE_NUM+1); i++)
                      {
                            memcpy(&nTempBuf[0], &sAcEn.sPPEnergy[i], sizeof(FORMAT11));
                            if (i < 2)
                            {
                                nTempBuf[0] = 0x66;
                                nTempBuf[3] = 0x78;
                            }
                            memcpy(pData + nIndex, &nTempBuf[0], sizeof(FORMAT11));
                            nIndex += sizeof(FORMAT11);
                      }
                    }
                    else if (g_MeterAcErrFlg[3])
                    {
                      for (i=0; i<(MAX_FEE_NUM+1); i++)
                      {
                        memcpy(&nTempBuf[0], &sAcEn.sPPEnergy[i], sizeof(FORMAT11));
                        if (0 == i)
                        {
                            nTempBuf[0] = 0x66;
                            nTempBuf[1] = 0x34;
                            nTempBuf[2] = 0x05;
                        }
                        else if(1 == i)
                        {
                            nTempBuf[0] = 0x33;
                            nTempBuf[1] = 0x67;
                            nTempBuf[2] = 0x04;
                        }
                        memcpy(pData + nIndex, &nTempBuf[0], sizeof(FORMAT11));
                        nIndex += sizeof(FORMAT11);
                      }
                    }
                    else
                    {
                        memcpy(pData + nIndex, &sAcEn.sPPEnergy[0], (MAX_FEE_NUM+1)*sizeof(FORMAT11));
                        nIndex += ((MAX_FEE_NUM + 1) * sizeof(FORMAT11));
                    }
                }
                else
                {
                    if (g_MeterAcErrFlg[0])
                    {
                        memcpy(&nTempBuf[0], &sAcEn.sPPEnergy[pDI01[1]], sizeof(FORMAT11));
                        if (pDI01[1] < 2)
                        {
                            nTempBuf[0] = 0x66;
                            nTempBuf[3] = 0x78;
                        }
                        memcpy(pData + nIndex, &nTempBuf[0], sizeof(FORMAT11));
                    }
                    else if (g_MeterAcErrFlg[3])
                    {
                      for (i=0; i<(MAX_FEE_NUM+1); i++)
                      {
                         memcpy(&nTempBuf[0], &sAcEn.sPPEnergy[pDI01[1]], sizeof(FORMAT11));
                        if (0 == i)
                        {
                            nTempBuf[0] = 0x66;
                            nTempBuf[1] = 0x34;
                            nTempBuf[2] = 0x05;
                        }
                        else if(1 == i)
                        {
                            nTempBuf[0] = 0x33;
                            nTempBuf[1] = 0x67;
                            nTempBuf[2] = 0x04;
                        }
                        memcpy(pData + nIndex, &nTempBuf[0], sizeof(FORMAT11));
                      }
                    }
                    else
                    {
                        memcpy(pData + nIndex, &sAcEn.sPPEnergy[pDI01[1]], sizeof(FORMAT11));
                    }
                    nIndex += sizeof(FORMAT11);
                }
                break;
            case 0x00020000:
                if (0xff == pDI01[1])
                {
                    memcpy(pData + nIndex, &sAcEn.sNPEnergy[0], (MAX_FEE_NUM+1)*sizeof(FORMAT11));
                    nIndex += ((MAX_FEE_NUM + 1) * sizeof(FORMAT11));
                }
                else
                {
                    memcpy(pData + nIndex, &sAcEn.sNPEnergy[pDI01[1]], sizeof(FORMAT11));
                    nIndex += sizeof(FORMAT11);
                }
                break;
            case 0x00030000:
                if (0xff == pDI01[1])
                {
                    memcpy(pData + nIndex, &sAcEn.sPNEnergy[0], (MAX_FEE_NUM+1)*sizeof(FORMAT11));
                    nIndex += ((MAX_FEE_NUM + 1) * sizeof(FORMAT11));
                }
                else
                {
                    memcpy(pData + nIndex, &sAcEn.sPNEnergy[pDI01[1]], sizeof(FORMAT11));
                    nIndex += sizeof(FORMAT11);
                }
                break;
            case 0x00040000:
                if (0xff == pDI01[1])
                {
                    memcpy(pData + nIndex, &sAcEn.sNNEnergy[0], (MAX_FEE_NUM+1)*sizeof(FORMAT11));
                    nIndex += ((MAX_FEE_NUM + 1) * sizeof(FORMAT11));
                }
                else
                {
                    memcpy(pData + nIndex, &sAcEn.sNNEnergy[pDI01[1]], sizeof(FORMAT11));
                    nIndex += sizeof(FORMAT11);
                }
                break;
            case 0x00050000:
                if (0xff == pDI01[1])
                {
                    memcpy(pData + nIndex, &sAcQEn.sQ1Energy[0], (MAX_FEE_NUM+1)*sizeof(FORMAT11));
                    nIndex += ((MAX_FEE_NUM+1)*sizeof(FORMAT11));
                }
                else
                {
                    memcpy(pData + nIndex, &sAcQEn.sQ1Energy[pDI01[1]], sizeof(FORMAT11));
                    nIndex += sizeof(FORMAT11);
                }
                break;
            case 0x00060000:
                if (0xff == pDI01[1])
                {
                    memcpy(pData + nIndex, &sAcQEn.sQ2Energy[0], (MAX_FEE_NUM+1)*sizeof(FORMAT11));
                    nIndex += ((MAX_FEE_NUM+1)*sizeof(FORMAT11));
                }
                else
                {
                    memcpy(pData + nIndex, &sAcQEn.sQ2Energy[pDI01[1]], sizeof(FORMAT11));
                    nIndex += sizeof(FORMAT11);
                }
                break;
            case 0x00070000:
                if (0xff == pDI01[1])
                {
                    memcpy(pData + nIndex, &sAcQEn.sQ3Energy[0], (MAX_FEE_NUM+1)*sizeof(FORMAT11));
                    nIndex += ((MAX_FEE_NUM+1)*sizeof(FORMAT11));
                }
                else
                {
                    memcpy(pData + nIndex, &sAcQEn.sQ3Energy[pDI01[1]], sizeof(FORMAT11));
                    nIndex += sizeof(FORMAT11);
                }
                break;
            case 0x00080000:
                if (0xff == pDI01[1])
                {
                    memcpy(pData + nIndex, &sAcQEn.sQ4Energy[0], (MAX_FEE_NUM+1)*sizeof(FORMAT11));
                    nIndex += ((MAX_FEE_NUM+1)*sizeof(FORMAT11));
                }
                else
                {
                    memcpy(pData + nIndex, &sAcQEn.sQ4Energy[pDI01[1]], sizeof(FORMAT11));
                    nIndex += sizeof(FORMAT11);
                }
                break;
            case 0x00150000:
                memcpy(pData + nIndex, &sAcTriEn.sPPEnergy[0], sizeof(FORMAT11));
                nIndex += sizeof(FORMAT11);
                break;
            case 0x00160000:
                memcpy(pData + nIndex, &sAcTriEn.sNPEnergy[0], sizeof(FORMAT11));
                nIndex += sizeof(FORMAT11);
                break;
            case 0x00170000:
                memcpy(pData + nIndex, &sAcTriEn.sPNEnergy[0], sizeof(FORMAT11));
                nIndex += sizeof(FORMAT11);
                break;
            case 0x00180000:
                memcpy(pData + nIndex, &sAcTriEn.sNNEnergy[0], sizeof(FORMAT11));
                nIndex += sizeof(FORMAT11);
                break;
            case 0x00190000:
                memcpy(pData + nIndex, &sAcTriQEn.sQ1Energy[0], sizeof(FORMAT11));
                nIndex += sizeof(FORMAT11);
                break;
            case 0x001A0000:
                memcpy(pData + nIndex, &sAcTriQEn.sQ2Energy[0], sizeof(FORMAT11));
                nIndex += sizeof(FORMAT11);
                break;
            case 0x001B0000:
                memcpy(pData + nIndex, &sAcTriQEn.sQ3Energy[0], sizeof(FORMAT11));
                nIndex += sizeof(FORMAT11);
                break;
            case 0x001C0000:
                memcpy(pData + nIndex, &sAcTriQEn.sQ4Energy[0], sizeof(FORMAT11));
                nIndex += sizeof(FORMAT11);
                break;

            case 0x00290000:
                memcpy(pData + nIndex, &sAcTriEn.sPPEnergy[1], sizeof(FORMAT11));
                nIndex += sizeof(FORMAT11);
                break;
            case 0x002A0000:
                memcpy(pData + nIndex, &sAcTriEn.sNPEnergy[1], sizeof(FORMAT11));
                nIndex += sizeof(FORMAT11);
                break;
            case 0x002B0000:
                memcpy(pData + nIndex, &sAcTriEn.sPNEnergy[1], sizeof(FORMAT11));
                nIndex += sizeof(FORMAT11);
                break;
            case 0x002C0000:
                memcpy(pData + nIndex, &sAcTriEn.sNNEnergy[1], sizeof(FORMAT11));
                nIndex += sizeof(FORMAT11);
                break;
            case 0x002D0000:
                memcpy(pData + nIndex, &sAcTriQEn.sQ1Energy[1], sizeof(FORMAT11));
                nIndex += sizeof(FORMAT11);
                break;
            case 0x002E0000:
                memcpy(pData + nIndex, &sAcTriQEn.sQ2Energy[1], sizeof(FORMAT11));
                nIndex += sizeof(FORMAT11);
                break;
            case 0x002F0000:
                memcpy(pData + nIndex, &sAcTriQEn.sQ3Energy[1], sizeof(FORMAT11));
                nIndex += sizeof(FORMAT11);
                break;
            case 0x00300000:
                memcpy(pData + nIndex, &sAcTriQEn.sQ4Energy[1], sizeof(FORMAT11));
                nIndex += sizeof(FORMAT11);
                break;

            case 0x003D0000:
                memcpy(pData + nIndex, &sAcTriEn.sPPEnergy[2], sizeof(FORMAT11));
                nIndex += sizeof(FORMAT11);
                break;
            case 0x003E0000:
                memcpy(pData + nIndex, &sAcTriEn.sNPEnergy[2], sizeof(FORMAT11));
                nIndex += sizeof(FORMAT11);
                break;
            case 0x003F0000:
                memcpy(pData + nIndex, &sAcTriEn.sPNEnergy[2], sizeof(FORMAT11));
                nIndex += sizeof(FORMAT11);
                break;
            case 0x00400000:
                memcpy(pData + nIndex, &sAcTriEn.sNNEnergy[2], sizeof(FORMAT11));
                nIndex += sizeof(FORMAT11);
                break;
            case 0x00410000:
                memcpy(pData + nIndex, &sAcTriQEn.sQ1Energy[2], sizeof(FORMAT11));
                nIndex += sizeof(FORMAT11);
                break;
            case 0x00420000:
                memcpy(pData + nIndex, &sAcTriQEn.sQ2Energy[2], sizeof(FORMAT11));
                nIndex += sizeof(FORMAT11);
                break;
            case 0x00430000:
                memcpy(pData + nIndex, &sAcTriQEn.sQ3Energy[2], sizeof(FORMAT11));
                nIndex += sizeof(FORMAT11);
                break;
            case 0x00440000:
                memcpy(pData + nIndex, &sAcTriQEn.sQ4Energy[2], sizeof(FORMAT11));
                nIndex += sizeof(FORMAT11);
                break;
           //召测当前需量及最大需量发生时间
                
            case 0x01010000:
                if (0xff == pDI01[1])
                {
                    for (i = 0; i < (MAX_FEE_NUM + 1); i++)
                    {
                        memcpy(pData + nIndex, &sAcDe.sPPDemand[i], sizeof(FORMAT23));
                        nIndex += sizeof(FORMAT23);
                        memcpy(pData + nIndex, &sAcDe.sPPDemandTime[i], sizeof(FORMAT15));
                        nIndex += sizeof(FORMAT15);
                    }
                }
                else
                {
                    memcpy(pData + nIndex, &sAcDe.sPPDemand[pDI01[1]], sizeof(FORMAT23));
                    nIndex += sizeof(FORMAT23);
                    memcpy(pData + nIndex, &sAcDe.sPPDemandTime[pDI01[1]], sizeof(FORMAT15));
                    nIndex += sizeof(FORMAT15);
                }
                break;
            case 0x01020000:
                if (0xff == pDI01[1])
                {
                    for (i = 0; i < (MAX_FEE_NUM + 1); i++)
                    {
                        memcpy(pData + nIndex, &sAcDe.sNPDemand[i], sizeof(FORMAT23));
                        nIndex += sizeof(FORMAT23);
                        memcpy(pData + nIndex, &sAcDe.sNPDemandTime[i], sizeof(FORMAT15));
                        nIndex += sizeof(FORMAT15);
                    }
                }
                else
                {
                    memcpy(pData + nIndex, &sAcDe.sNPDemand[pDI01[1]], sizeof(FORMAT23));
                    nIndex += sizeof(FORMAT23);
                    memcpy(pData + nIndex, &sAcDe.sNPDemandTime[pDI01[1]], sizeof(FORMAT15));
                    nIndex += sizeof(FORMAT15);
                }
                break;
            case 0x01030000:
                if (0xff == pDI01[1])
                {
                    for (i = 0; i < (MAX_FEE_NUM + 1); i++)
                    {
                        memcpy(pData+nIndex, &sAcDe.sPNDemand[i], sizeof(FORMAT23));
                        nIndex += sizeof(FORMAT23);
                        memcpy(pData+nIndex, &sAcDe.sPNDemandTime[i], sizeof(FORMAT15));
                        nIndex += sizeof(FORMAT15);
                    }
                }
                else
                {
                    memcpy(pData + nIndex, &sAcDe.sPNDemand[pDI01[1]], sizeof(FORMAT23));
                    nIndex += sizeof(FORMAT23);
                    memcpy(pData + nIndex, &sAcDe.sPNDemandTime[pDI01[1]], sizeof(FORMAT15));
                    nIndex += sizeof(FORMAT15);
                }
                break;
            case 0x01040000:
                if (0xff == pDI01[1])
                {
                    for(i=0; i<(MAX_FEE_NUM+1); i++)
                    {
                        memcpy(pData + nIndex, &sAcDe.sNNDemand[i], sizeof(FORMAT23));
                        nIndex += sizeof(FORMAT23);
                        memcpy(pData + nIndex, &sAcDe.sNNDemandTime[i], sizeof(FORMAT15));
                        nIndex += sizeof(FORMAT15);
                    }
                }
                else
                {
                    memcpy(pData + nIndex, &sAcDe.sNNDemand[pDI01[1]], sizeof(FORMAT23));
                    nIndex += sizeof(FORMAT23);
                    memcpy(pData + nIndex, &sAcDe.sNNDemandTime[pDI01[1]], sizeof(FORMAT15));
                    nIndex += sizeof(FORMAT15);
                }
                break;
            case 0x01050000:
                if (0xff == pDI01[1])
                {
                    for (i = 0; i < (MAX_FEE_NUM + 1); i++)
                    {
                        memcpy(pData + nIndex, &sAcQDe.sQ1Demand[i], sizeof(FORMAT23));
                        nIndex += sizeof(FORMAT23);
                        memcpy(pData + nIndex, &sAcQDe.sQ1DemandTime[i], sizeof(FORMAT15));
                        nIndex += sizeof(FORMAT15);
                    }
                }
                else
                {
                    memcpy(pData + nIndex, &sAcQDe.sQ1Demand[pDI01[1]], sizeof(FORMAT23));
                    nIndex += sizeof(FORMAT23);
                    memcpy(pData + nIndex, &sAcQDe.sQ1DemandTime[pDI01[1]], sizeof(FORMAT15));
                    nIndex += sizeof(FORMAT15);
                }
                break;
            case 0x01060000:
                if (0xff == pDI01[1])
                {
                    for (i = 0; i < (MAX_FEE_NUM + 1); i++)
                    {
                        memcpy(pData + nIndex, &sAcQDe.sQ2Demand[i], sizeof(FORMAT23));
                        nIndex += sizeof(FORMAT23);
                        memcpy(pData + nIndex, &sAcQDe.sQ2DemandTime[i], sizeof(FORMAT15));
                        nIndex += sizeof(FORMAT15);
                    }
                }
                else
                {
                    memcpy(pData + nIndex, &sAcQDe.sQ2Demand[pDI01[1]], sizeof(FORMAT23));
                    nIndex += sizeof(FORMAT23);
                    memcpy(pData + nIndex, &sAcQDe.sQ2DemandTime[pDI01[1]], sizeof(FORMAT15));
                    nIndex += sizeof(FORMAT15);
                }
                break;
            case 0x01070000:
                if (0xff == pDI01[1])
                {
                    for( i = 0; i < (MAX_FEE_NUM + 1); i++)
                    {
                        memcpy(pData + nIndex, &sAcQDe.sQ3Demand[i], sizeof(FORMAT23));
                        nIndex += sizeof(FORMAT23);
                        memcpy(pData + nIndex, &sAcQDe.sQ3DemandTime[i], sizeof(FORMAT15));
                        nIndex += sizeof(FORMAT15);
                    }
                }
                else
                {
                    memcpy(pData + nIndex, &sAcQDe.sQ3Demand[pDI01[1]], sizeof(FORMAT23));
                    nIndex += sizeof(FORMAT23);
                    memcpy(pData + nIndex, &sAcQDe.sQ3DemandTime[pDI01[1]], sizeof(FORMAT15));
                    nIndex += sizeof(FORMAT15);
                }
                break;
            case 0x01080000:
                if (0xff == pDI01[1])
                {
                    for(i = 0; i < (MAX_FEE_NUM + 1); i++)
                    {
                        memcpy(pData + nIndex, &sAcQDe.sQ4Demand[i], sizeof(FORMAT23));
                        nIndex += sizeof(FORMAT23);
                        memcpy(pData + nIndex, &sAcQDe.sQ4DemandTime[i], sizeof(FORMAT15));
                        nIndex += sizeof(FORMAT15);
                    }
                }
                else
                {
                    memcpy(pData + nIndex, &sAcQDe.sQ4Demand[pDI01[1]], sizeof(FORMAT23));
                    nIndex += sizeof(FORMAT23);
                    memcpy(pData + nIndex, &sAcQDe.sQ4DemandTime[pDI01[1]], sizeof(FORMAT15));
                    nIndex += sizeof(FORMAT15);
                }
                break;
                
                //召测电表日冻结数据、定时、瞬时、约定
                case 0x05060000:
                case 0x05000000:
                case 0x05010000:
                case 0x05020000:
                case 0x05030000:
                    switch(pDI01[1])
                    {
                        case 0x00:
                            memcpy(pData + nIndex, &sDJTime, sizeof(sDJTime));
                            nIndex += sizeof(FORMAT15);
                            break;
                        case 0x01:
                            memcpy(pData + nIndex, &sAcEn.sPPEnergy[0], (MAX_FEE_NUM+1)*sizeof(FORMAT11));
                            nIndex += ((MAX_FEE_NUM + 1) * sizeof(FORMAT11));
                            break;
                        case 0x02:
                            memcpy(pData + nIndex, &sAcEn.sNPEnergy[0], (MAX_FEE_NUM+1)*sizeof(FORMAT11));
                            nIndex += ((MAX_FEE_NUM + 1) * sizeof(FORMAT11));
                            break;
                        case 0x03:
                            memcpy(pData + nIndex, &sAcEn.sPNEnergy[0], (MAX_FEE_NUM+1)*sizeof(FORMAT11));
                            nIndex += ((MAX_FEE_NUM + 1) * sizeof(FORMAT11));
                            break;
                        case 0x04:
                            memcpy(pData + nIndex, &sAcEn.sNNEnergy[0], (MAX_FEE_NUM+1)*sizeof(FORMAT11));
                            nIndex += ((MAX_FEE_NUM + 1) * sizeof(FORMAT11));
                            break;
                        case 0x05:
                            memcpy(pData + nIndex, &sAcQEn.sQ1Energy[0], (MAX_FEE_NUM+1)*sizeof(FORMAT11));
                            nIndex += ((MAX_FEE_NUM+1)*sizeof(FORMAT11));
                            break;
                        case 0x06:
                            memcpy(pData + nIndex, &sAcQEn.sQ2Energy[0], (MAX_FEE_NUM+1)*sizeof(FORMAT11));
                            nIndex += ((MAX_FEE_NUM+1)*sizeof(FORMAT11));
                            break;
                        case 0x07:
                            memcpy(pData + nIndex, &sAcQEn.sQ3Energy[0], (MAX_FEE_NUM+1)*sizeof(FORMAT11));
                            nIndex += ((MAX_FEE_NUM+1)*sizeof(FORMAT11));
                            break;
                        case 0x08:
                            memcpy(pData + nIndex, &sAcQEn.sQ4Energy[0], (MAX_FEE_NUM+1)*sizeof(FORMAT11));
                            nIndex += ((MAX_FEE_NUM+1)*sizeof(FORMAT11));
                            break;
                        case 0x09:
                            for (i = 0; i < (MAX_FEE_NUM + 1); i++)
                            {
                                memcpy(pData + nIndex, &sAcDe.sPPDemand[i], sizeof(FORMAT23));
                                nIndex += sizeof(FORMAT23);
                                memcpy(pData + nIndex, &sAcDe.sPPDemandTime[i], sizeof(FORMAT15));
                                nIndex += sizeof(FORMAT15);
                            }
                            break;
                        case 0x0a:
                            for (i = 0; i < (MAX_FEE_NUM + 1); i++)
                            {
                                memcpy(pData + nIndex, &sAcDe.sNPDemand[i], sizeof(FORMAT23));
                                nIndex += sizeof(FORMAT23);
                                memcpy(pData + nIndex, &sAcDe.sNPDemandTime[i], sizeof(FORMAT15));
                                nIndex += sizeof(FORMAT15);
                            }
                            break;
                        case 0x10:
                            for (i = 0; i < (MAX_PHASE_NUM + 1); i++)
                            {
                                memcpy(pData + nIndex, &sRealP[i], sizeof(FORMAT09));
                                nIndex += sizeof(FORMAT09);
                            }
                            for (i = 0; i < (MAX_PHASE_NUM + 1); i++)
                            {
                                memcpy(pData + nIndex, &sRealQ[i], sizeof(FORMAT09));
                                nIndex += sizeof(FORMAT09);
                            }
                            break;
                        default:
                            *pIsExist = 0x02;
                            break;
                    }
                    break;
                 case 0x0506FF01:
                   {

                            memcpy(pData + nIndex, &sDJTime, sizeof(sDJTime));
                            nIndex += sizeof(FORMAT15);
                            memcpy(pData + nIndex, &sAcEn.sPPEnergy[0], (MAX_FEE_NUM+1)*sizeof(FORMAT11));
                            nIndex += ((MAX_FEE_NUM + 1) * sizeof(FORMAT11));

                            memcpy(pData + nIndex, &sAcEn.sNPEnergy[0], (MAX_FEE_NUM+1)*sizeof(FORMAT11));
                            nIndex += ((MAX_FEE_NUM + 1) * sizeof(FORMAT11));

                            memcpy(pData + nIndex, &sAcEn.sPNEnergy[0], (MAX_FEE_NUM+1)*sizeof(FORMAT11));
                            nIndex += ((MAX_FEE_NUM + 1) * sizeof(FORMAT11));

                            memcpy(pData + nIndex, &sAcEn.sNNEnergy[0], (MAX_FEE_NUM+1)*sizeof(FORMAT11));
                            nIndex += ((MAX_FEE_NUM + 1) * sizeof(FORMAT11));

                            memcpy(pData + nIndex, &sAcQEn.sQ1Energy[0], (MAX_FEE_NUM+1)*sizeof(FORMAT11));
                            nIndex += ((MAX_FEE_NUM+1)*sizeof(FORMAT11));

                            memcpy(pData + nIndex, &sAcQEn.sQ2Energy[0], (MAX_FEE_NUM+1)*sizeof(FORMAT11));
                            nIndex += ((MAX_FEE_NUM+1)*sizeof(FORMAT11));

                            memcpy(pData + nIndex, &sAcQEn.sQ3Energy[0], (MAX_FEE_NUM+1)*sizeof(FORMAT11));
                            nIndex += ((MAX_FEE_NUM+1)*sizeof(FORMAT11));

                            memcpy(pData + nIndex, &sAcQEn.sQ4Energy[0], (MAX_FEE_NUM+1)*sizeof(FORMAT11));
                            nIndex += ((MAX_FEE_NUM+1)*sizeof(FORMAT11));

                            for (i = 0; i < (MAX_FEE_NUM + 1); i++)
                            {
                                memcpy(pData + nIndex, &sAcDe.sPPDemand[i], sizeof(FORMAT23));
                                nIndex += sizeof(FORMAT23);
                                memcpy(pData + nIndex, &sAcDe.sPPDemandTime[i], sizeof(FORMAT15));
                                nIndex += sizeof(FORMAT15);
                            }

                            for (i = 0; i < (MAX_FEE_NUM + 1); i++)
                            {
                                memcpy(pData + nIndex, &sAcDe.sNPDemand[i], sizeof(FORMAT23));
                                nIndex += sizeof(FORMAT23);
                                memcpy(pData + nIndex, &sAcDe.sNPDemandTime[i], sizeof(FORMAT15));
                                nIndex += sizeof(FORMAT15);
                            }

                            for (i = 0; i < (MAX_PHASE_NUM + 1); i++)
                            {
                                memcpy(pData + nIndex, &sRealP[i], sizeof(FORMAT09));
                                nIndex += sizeof(FORMAT09);
                            }
                            for (i = 0; i < (MAX_PHASE_NUM + 1); i++)
                            {
                                memcpy(pData + nIndex, &sRealQ[i], sizeof(FORMAT09));
                                nIndex += sizeof(FORMAT09);
                            }
                   }
                   break;
                //召测电表瞬时数据
                case 0x02010000:					//电压
                    INT8U nBuf[2];
                    if (0xff == pDI01[1])
                    {
                        for (i=0; i<MAX_PHASE_NUM; i++)
                        {
                            if (1 == g_MeterEventWrong[i]&0x01) //|| ((0x20+(1<<i)) == (g_MeterVoltPlusFlg&(0x20+(1<<i))))||(g_MeterVoltPlusFlg == 0x01))
                            {
                                memcpy(nBuf, &pDataTable->sRealInstant.sRealVolt[i], 2);
                                DivideTwo(nBuf);
                                memcpy(pData + nIndex, &nBuf, 2);
                            }
                            else if(((0x20+(1<<i)) == (g_MeterVoltPlusFlg&(0x20+(1<<i))))||(g_MeterVoltPlusFlg == 0x01)) //电压越下限
                            {
                                memcpy(nBuf, &pDataTable->sRealInstant.sRealVolt[i], 2);
                                DivideTwo2(nBuf);
                                memcpy(pData + nIndex, &nBuf, 2);
                            }
                            else if ((0x10+(1<<i)) == (g_MeterVoltPlusFlg&(0x10+(1<<i))))//电压越上限
                            {
                                memcpy(nBuf, &pDataTable->sRealInstant.sRealVolt[i], 2);
                                if (0 != nBuf[1])
                                {
                                   nBuf[1] += 0x05;
                                }
                                memcpy(pData + nIndex, &nBuf, 2);
                                
                            }
                            else if (0x02==(g_MeterEventWrong[i]&0x02))
                            {
                                memset(pData + nIndex, 0, 2 * MAX_PHASE_NUM);
                            }
                            else
                            {
                                memcpy(pData + nIndex, &pDataTable->sRealInstant.sRealVolt[i], 2);
                            }
                            nIndex += 2;
                        }
                    }
                    else
                    {
                        if ((pDI01[1] > 0) && (pDI01[1]<4))
                        {
                            i = pDI01[1]-1;
                            if (1 == g_MeterEventWrong[i]&0x01) //|| ((0x20+(i<<1)) == (g_MeterVoltPlusFlg&(0x20+(i<<1))))||(g_MeterVoltPlusFlg == 0x01))
                            { 
                                memcpy(nBuf, &pDataTable->sRealInstant.sRealVolt[pDI01[1]-1], 2);
                                DivideTwo(nBuf);
                                memcpy(pData + nIndex, &nBuf, 2);
                            }
                            else if (((0x20+(i<<1)) == (g_MeterVoltPlusFlg&(0x20+(i<<1))))||(g_MeterVoltPlusFlg == 0x01))
                            { 
                                memcpy(nBuf, &pDataTable->sRealInstant.sRealVolt[pDI01[1]-1], 2);
                                DivideTwo2(nBuf);
                                memcpy(pData + nIndex, &nBuf, 2);
                            }
                            else if ((0x10+(i<<1)) == (g_MeterVoltPlusFlg&(0x10+(i<<1))))//电压越上限
                            {
                                memcpy(nBuf, &pDataTable->sRealInstant.sRealVolt[pDI01[1] - 1], 2);
                                if (0 != nBuf[1])
                                {
                                   nBuf[1] += 0x05;
                                }
                                memcpy(pData + nIndex, &nBuf, 2);
                            }
                            else if (0x02==(g_MeterEventWrong[i]&0x02))
                            {
                                memset(pData + nIndex, 0, 2);
                            }
                            else
                            {
                                memcpy(pData + nIndex, &pDataTable->sRealInstant.sRealVolt[pDI01[1] - 1], 2);
                            }
                            nIndex += 2;
                        }
                        else
                        {
                            *pIsExist = 0x02;
                        }
                    }
                    break;

                case 0x02800000://零序电流
                    if(g_nSetShortcurFlg == 1)
                    {
                        pData[nIndex++] = 0x96;
                        pData[nIndex++] = 0x29;
                        pData[nIndex++] = 0x00;
                    }
                    else
                    {
                        if ((0x00 == pDI01[1]) && (0x01 == pDI01[0]))
                        {
                            memcpy(pData + nIndex, &pDataTable->sRealInstant.sRealCurr[0], 3);
                            nIndex += 3;
                        }
                    }
                    break;
                    
                case 0x02020000:					//电流
                    if (0xff == pDI01[1])
                    {
                        for(i = 0; i < MAX_PHASE_NUM; i ++)
                        {
                            if (0x04==(g_MeterEventWrong[i]&0x04))
                            {
                                memset(pData + nIndex, 0, 3);
                            }  
                            else
                            {
                                //判断A相电流是否反向
                                if((i == 0)&&(pDataTable->nPFlag&0x01)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[1].nFrac0, 3, 0))))
                                {
                                   pDataTable->sRealInstant.sRealCurr[i].nD3 |= 0x80;
                                }
                                //判断B相电流是否反向
                                if((i == 1)&&(pDataTable->nPFlag&(0x01<<1))&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[2].nFrac0, 3, 0))))  
                                {
                                   pDataTable->sRealInstant.sRealCurr[i].nD3 |= 0x80;
                                }
                                //判断C相电流是否反向
                                if((i == 2)&&(pDataTable->nPFlag&(0x01<<2))&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[3].nFrac0, 3, 0))))
                                {
                                  pDataTable->sRealInstant.sRealCurr[i].nD3 |= 0x80;
                                }
                                memcpy(pData + nIndex, &pDataTable->sRealInstant.sRealCurr[i], 3);
                            } 
                            nIndex += 3;
                        }
                    }
                    else
                    {
                        if ((pDI01[1] > 0)&&(pDI01[1]<4))
                        {
                            i = pDI01[1]-1;
                            if ((0x04==(g_MeterEventWrong[i]&0x04)) || (0x02==(g_MeterEventWrong[i]&0x02)))
                            {
                                memset(pData + nIndex, 0, 3);
                            }  
                            else
                            {
                                //判断A相电流是否反向
                                if((i == 0)&&(pDataTable->nPFlag&0x01)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[1].nFrac0, 3, 0))))
                                {
                                   pDataTable->sRealInstant.sRealCurr[i].nD3 |= 0x80;
                                }
                                //判断B相电流是否反向
                                if((i == 1)&&(pDataTable->nPFlag&(0x01<<1))&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[2].nFrac0, 3, 0))))  
                                {
                                   pDataTable->sRealInstant.sRealCurr[i].nD3 |= 0x80;
                                }
                                //判断C相电流是否反向
                                if((i == 2)&&(pDataTable->nPFlag&(0x01<<2))&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[3].nFrac0, 3, 0))))
                                {
                                  pDataTable->sRealInstant.sRealCurr[i].nD3 |= 0x80;
                                }
                                memcpy(pData + nIndex, &pDataTable->sRealInstant.sRealCurr[pDI01[1] - 1], 3);
                            }
                            nIndex += 3;
                        }
                        else
                        {
                            *pIsExist = 0x02;
                        }
                    }
                    break;
                case 0x02030000:        //瞬时有功功率
                    if (0xff == pDI01[1])
                    {   
                        for(i = 0; i < MAX_PHASE_NUM + 1; i ++)
                        {
                            //瞬时总有功功率
                            if((i == 0)&&(pDataTable->nPFlag&0x08)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[0].nFrac0, 3, 0)))) 
                            {
                               pDataTable->sRealInstant.sRealP[i].nSint |= 0x80;
                            }
                            //瞬时A相有功功率
                            if((i == 1)&&(pDataTable->nPFlag&0x01)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[1].nFrac0, 3, 0))))
                            {
                              pDataTable->sRealInstant.sRealP[i].nSint |= 0x80;
                            }
                            //瞬时B相有功功率
                            if((i == 2)&&(pDataTable->nPFlag&(0x01<<1))&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[2].nFrac0, 3, 0)))) 
                            {
                              pDataTable->sRealInstant.sRealP[i].nSint |= 0x80;
                            }
                            //瞬时C相有功功率
                            if((i == 3)&&(pDataTable->nPFlag&(0x01<<2))&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[3].nFrac0, 3, 0))))
                            {
                               pDataTable->sRealInstant.sRealP[i].nSint |= 0x80;
                            }
                        }
                        
                        memcpy(pData + nIndex, &pDataTable->sRealInstant.sRealP[0], 3 * (MAX_PHASE_NUM + 1));
                        nIndex += 3 * (MAX_PHASE_NUM+1);
                    }
                    else
                    {
                        if (pDI01[1] < 4)
                        {
                            //瞬时总有功功率
                            if((pDI01[1] == 0)&&(pDataTable->nPFlag&0x08)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[0].nFrac0, 3, 0)))) 
                            {
                              pDataTable->sRealInstant.sRealP[pDI01[1]].nSint |= 0x80;
                            }
                            //瞬时A相有功功率方向判断
                            if((pDI01[1] == 1)&&(pDataTable->nPFlag&0x01)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[1].nFrac0, 3, 0))))
                            {
                              pDataTable->sRealInstant.sRealP[pDI01[1]].nSint |= 0x80;
                            }
                            //瞬时B相有功功率方向判断
                            if((pDI01[1] == 2)&&(pDataTable->nPFlag&(0x01<<1))&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[2].nFrac0, 3, 0)))) 
                            {
                               pDataTable->sRealInstant.sRealP[pDI01[1]].nSint |= 0x80;
                            }
                            //瞬时C相有功功率方向判断
                            if((pDI01[1] == 3)&&(pDataTable->nPFlag&(0x01<<2))&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[3].nFrac0, 3, 0)))) 
                            {
                               pDataTable->sRealInstant.sRealP[pDI01[1]].nSint |= 0x80;
                            }
                            memcpy(pData + nIndex, &pDataTable->sRealInstant.sRealP[pDI01[1]], 3);
                            nIndex += 3;
                        }
                        else
                        {
                            *pIsExist = 0x02;
                        }
                    }
                    break;
                case 0x02040000:  //瞬时无功功率
                    if (0xff == pDI01[1])
                    {
                        memcpy(pData + nIndex, &pDataTable->sRealInstant.sRealQ[0], 3 * (MAX_PHASE_NUM + 1));
                        nIndex += 3 * (MAX_PHASE_NUM+1);
                    }
                    else
                    {
                        if (pDI01[1] < 4)
                        {
                            memcpy(pData + nIndex, &pDataTable->sRealInstant.sRealQ[pDI01[1]], 3);
                            nIndex += 3;
                        }
                        else
                        {
                            *pIsExist = 0x02;
                        }
                    }
                    break;
                case 0x02050000:    //瞬时视在功率
                    if (0xff == pDI01[1])
                    {
                        memcpy(pData + nIndex, &pDataTable->sRealInstant.sRealS[0], 3 * (MAX_PHASE_NUM + 1));
                        nIndex += 3 * (MAX_PHASE_NUM + 1);
                    }
                    else
                    {
                        if (pDI01[1] < 4)
                        {
                            memcpy(pData + nIndex, &pDataTable->sRealInstant.sRealS[pDI01[1]], 3);
                            nIndex += 3;
                        }
                        else
                        {
                            *pIsExist = 0x02;
                        }
                    }
                    break;
                case 0x02060000:  //功率因数
                    if (0xff == pDI01[1])
                    {
                        for(i = 0; i < MAX_PHASE_NUM + 1; i ++)
                        {
                            //瞬时总有功功率因数
                            if((i == 0)&&(pDataTable->nPFlag&0x08)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[0].nFrac0, 3, 0)))) 
                            {
                               pDataTable->sRealInstant.sRealPf[i].nSint |= 0x80;
                            }
                            //瞬时A相有功功率因数
                            if((i == 1)&&(pDataTable->nPFlag&0x01)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[1].nFrac0, 3, 0))))
                            {
                               pDataTable->sRealInstant.sRealPf[i].nSint |= 0x80;
                            }
                            //瞬时B相有功功率因数
                            if((i == 2)&&(pDataTable->nPFlag&(0x01<<1))&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[2].nFrac0, 3, 0)))) 
                            {
                               pDataTable->sRealInstant.sRealPf[i].nSint |= 0x80;
                            }
                            //瞬时C相有功功率因数
                            if((i == 3)&&(pDataTable->nPFlag&(0x01<<2))&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[3].nFrac0, 3, 0))))
                            {
                               pDataTable->sRealInstant.sRealPf[i].nSint |= 0x80;
                            }
                        }
                        
                        memcpy(pData + nIndex, &pDataTable->sRealInstant.sRealPf[0], 2 * (MAX_PHASE_NUM + 1));
                        nIndex += 2 * (MAX_PHASE_NUM + 1);
                    }
                    else
                    {
                        if (pDI01[1] < 4)
                        {
                             //瞬时总有功功率因数
                            if((pDI01[1] == 0)&&(pDataTable->nPFlag&0x08)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[0].nFrac0, 3, 0)))) 
                            {
                              pDataTable->sRealInstant.sRealPf[pDI01[1]].nSint |= 0x80;
                            }
                            //瞬时A相有功功率因数
                            if((pDI01[1] == 1)&&(pDataTable->nPFlag&0x01)&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[1].nFrac0, 3, 0))))
                            {
                              pDataTable->sRealInstant.sRealPf[pDI01[1]].nSint |= 0x80;
                            }
                            //瞬时B相有功功率因数
                            if((pDI01[1] == 2)&&(pDataTable->nPFlag&(0x01<<1))&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[2].nFrac0, 3, 0)))) 
                            {
                               pDataTable->sRealInstant.sRealPf[pDI01[1]].nSint |= 0x80;
                            }
                            //瞬时C相有功功率因数
                            if((pDI01[1] == 3)&&(pDataTable->nPFlag&(0x01<<2))&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[3].nFrac0, 3, 0)))) 
                            {
                               pDataTable->sRealInstant.sRealPf[pDI01[1]].nSint |= 0x80;
                            }
                            memcpy(pData + nIndex, &pDataTable->sRealInstant.sRealPf[pDI01[1]], 2);
                            nIndex += 2;
                        }
                        else
                        {
                            *pIsExist = 0x02;
                        }
                    }
                    break;
                case 0x02070000:
                    if (0xff == pDI01[1])
                    {
                        memcpy(pData + nIndex, &pDataTable->sRealInstant.sRealPg[0], 2 * (MAX_PHASE_NUM));//sRealY
                        nIndex += 2 * (MAX_PHASE_NUM);
                    }
                    else
                    {
                        if (pDI01[1] < 4)
                        {
                            memcpy(pData + nIndex, &pDataTable->sRealInstant.sRealPg[pDI01[1]-1], 2);
                            nIndex += 2;
                        }
                        else
                        {
                            *pIsExist = 0x02;
                        }
                    }
                    break;
                case 0x04000000:
                    switch(pDI01[1])
                    {
                        case 0x01:
                            switch(pDI01[0])
                            {
                                case 0xff:
                                    pData[nIndex++] = sTime.nWeek;
                                    if(0x02==(g_MeterInsideWrong[0]&0x02)) //时间显示故障
                                    {
                                        if(sTime.nDay < 0x27)
                                         {
                                            pData[nIndex++] = sTime.nDay+2;
                                         }
                                        else
                                        {
                                            pData[nIndex++] = 3;
                                        }
                                    }
                                    else
                                    {
                                      pData[nIndex++] = sTime.nDay;
                                    }
                                    pData[nIndex++] = sTime.nMon;
                                    pData[nIndex++] = sTime.nYear;
                                    pData[nIndex++] = sTime.nSec;
                                    if(0x02==(g_MeterInsideWrong[0]&0x02)) //时间显示故障
                                    {
                                        if (sTime.nMin < 0x48)
                                         {
                                            pData[nIndex++] = sTime.nMin+0x10;
                                         }
                                        else
                                        {
                                            pData[nIndex++] = 5;
                                        }
                                    }
                                    else
                                    {
                                      pData[nIndex++] = sTime.nMin;
                                    }
                                    pData[nIndex++] = sTime.nHour;
                                    pData[nIndex++] = INT8U_TO_BCD(sAc_Param.nDemandPeriod);
                                    pData[nIndex++] = INT8U_TO_BCD(sAc_Param.nSlipTime);
                                    pData[nIndex++] = 0x90;
                                    pData[nIndex++] = 0x00;
                                    memcpy(&pData[nIndex], &sAc_Param.sYearChangeTime, sizeof(FORMAT15));
                                    nIndex += sizeof(FORMAT15);
                                    memcpy(&pData[nIndex], &sAc_Param.sDayChangeTime, sizeof(FORMAT15));
                                    nIndex += sizeof(FORMAT15);
                                    break;
                                case 0x01:
                                    if(sTime.nWeek == 0x07)
                                    {
                                        pData[nIndex++] = 0x00;
                                    }
                                    else
                                    {
                                        pData[nIndex++] = sTime.nWeek;
                                    }
                                    if(0x02==(g_MeterInsideWrong[0]&0x02)) //时间显示故障
                                    {
                                        if(sTime.nDay < 0x28)
                                         {
                                            pData[nIndex++] +=1;
                                         }
                                        else
                                        {
                                            pData[nIndex++] = 1;
                                        }
                                    }
                                    else
                                    {
                                      pData[nIndex++] = sTime.nDay;
                                    }
                                    pData[nIndex++] = sTime.nMon;
                                    pData[nIndex++] = sTime.nYear;
                                    break;
                                case 0x02:
                                    pData[nIndex++] = sTime.nSec;
                                    if(0x02==(g_MeterInsideWrong[0]&0x02)) //时间显示故障
                                    {
                                        if(sTime.nMin < 0x48)
                                         {
                                            pData[nIndex++] = (sTime.nMin + 0x10);//pData[nIndex++] +=1;  //22.10.11 修改读取时间与显示相同
                                         }
                                        else
                                        {
                                            pData[nIndex++] = 0x05;
                                        }
                                    }
                                    else
                                    {
                                      pData[nIndex++] = sTime.nMin;
                                    }
                                    pData[nIndex++] = sTime.nHour;
                                    break;
                                case 0x03:
                                    pData[nIndex++] = INT8U_TO_BCD(sAc_Param.nDemandPeriod);
                                    break;
                                case 0x04:
                                    pData[nIndex++] = INT8U_TO_BCD(sAc_Param.nSlipTime);
                                    break;
                                case 0x05:
                                    pData[nIndex++] = 0x90;
                                    pData[nIndex++] = 0x00;
                                    break;
                                case 0x06:
                                    memcpy(&pData[nIndex], &sAc_Param.sYearChangeTime, sizeof(FORMAT15));
                                    nIndex += sizeof(FORMAT15);
                                    break;
                                case 0x07:
                                    memcpy(&pData[nIndex], &sAc_Param.sDayChangeTime, sizeof(FORMAT15));
                                    nIndex += sizeof(FORMAT15);
                                    break;
                                default:
                                    *pIsExist = 0x02;
                                    break;
                            }
                            break;
                        case 0x02:
                            if (sAc_Param.nYearSegmNum > MAX_YEAR_SEGM_NUM)
                            {
                                sAc_Param.nYearSegmNum = MAX_YEAR_SEGM_NUM;
                            }
                            if (sAc_Param.nDayTableNum > MAX_DAY_TABLE_NUM)
                            {
                                sAc_Param.nDayTableNum = MAX_DAY_TABLE_NUM;
                            }
                            if (sAc_Param.nDaySegmNum > MAX_DAY_SEGM_NUM)
                            {
                                sAc_Param.nDaySegmNum = MAX_DAY_SEGM_NUM;
                            }
                            if (sAc_Param.nFeeNumber > 63)
                            {
                                sAc_Param.nFeeNumber = 63;
                            }
                            if (sAc_Param.nHolidayNumber > 254)
                            {
                                sAc_Param.nHolidayNumber = 254;
                            }

                            switch(pDI01[0])
                            {
                                case 0xff:
                                    pData[nIndex++] = INT8U_TO_BCD(sAc_Param.nYearSegmNum);
                                    pData[nIndex++] = INT8U_TO_BCD(sAc_Param.nDayTableNum);
                                    pData[nIndex++] = INT8U_TO_BCD(sAc_Param.nDaySegmNum);
                                    pData[nIndex++] = INT8U_TO_BCD(sAc_Param.nFeeNumber);
                                    pData[nIndex++] = INT8U_TO_BCD((INT8U)(sAc_Param.nHolidayNumber%100));
                                    pData[nIndex++] = INT8U_TO_BCD((INT8U)(sAc_Param.nHolidayNumber/100));
                                    pData[nIndex++] = INT8U_TO_BCD(sAc_Param.nHarmcNumber);
                                    break;
                                case 0x01:
                                    pData[nIndex++] = INT8U_TO_BCD(sAc_Param.nYearSegmNum);
                                    break;
                                case 0x02:
                                    pData[nIndex++] = INT8U_TO_BCD(sAc_Param.nDayTableNum);
                                    break;
                                case 0x03:
                                    pData[nIndex++] = INT8U_TO_BCD(sAc_Param.nDaySegmNum);
                                    break;
                                case 0x04:
                                    pData[nIndex++] = INT8U_TO_BCD(sAc_Param.nFeeNumber);
                                    break;
                                case 0x05:
                                    pData[nIndex++] = INT8U_TO_BCD((INT8U)(sAc_Param.nHolidayNumber%100));
                                    pData[nIndex++] = INT8U_TO_BCD((INT8U)(sAc_Param.nHolidayNumber/100));
                                    break;
                                case 0x06:
                                    pData[nIndex++] = INT8U_TO_BCD(sAc_Param.nHarmcNumber);
                                    break;
                                default:
                                    *pIsExist = 0x02;
                                break;
                            }
                            break;
                        case 0x03:
                            switch(pDI01[0])
                            {
                                case 0xff:
                                    pData[nIndex++] = sAc_Param.nDispNum1;
                                    pData[nIndex++] = sAc_Param.nDispTime1;
                                    pData[nIndex++] = sAc_Param.nDispPoint1;
                                    pData[nIndex++] = sAc_Param.nDispPoint2;
                                    pData[nIndex++] = sAc_Param.nDispNum2;
                                    break;
                                case 0x01:
                                    pData[nIndex++] = sAc_Param.nDispNum1;
                                    break;
                                case 0x02:
                                    pData[nIndex++] = sAc_Param.nDispTime1;
                                    break;
                                case 0x03:
                                    pData[nIndex++] = sAc_Param.nDispPoint1;
                                    break;
                                case 0x04:
                                    pData[nIndex++] = sAc_Param.nDispPoint2;
                                    break;
                                case 0x05:
                                    pData[nIndex++] = sAc_Param.nDispNum2;
                                    break;
                                default:
                                    *pIsExist = 0x02;
                                break;
                            }
                            break;
                        case 0x04:
                            switch(pDI01[0])
                            {
                                case 0xff:
                                    memcpy(&pData[nIndex], &sAc_Param.nMeterAddr[0], LEN_METER_ADDR);
                                    nIndex += LEN_METER_ADDR;
                                    memcpy(&pData[nIndex], &sAc_Param.nMeterNumber[0], LEN_METER_ADDR);
                                    nIndex += LEN_METER_ADDR;
                                    //memcpy(&pData[nIndex], &sAc_Param.nMeterProNum[0], LEN_METER_GLBH);
                                    memset(&pData[nIndex], 0, LEN_METER_GLBH);
                                    nIndex += LEN_METER_GLBH;

                                    break;
                                case 0x01:
                                    memcpy(&pData[nIndex], &sAc_Param.nMeterAddr[0], LEN_METER_ADDR);
                                    nIndex += LEN_METER_ADDR;
                                    break;
                                case 0x02:
                                    memcpy(&pData[nIndex], &sAc_Param.nMeterNumber[0], LEN_METER_ADDR);
                                    nIndex += LEN_METER_ADDR;
                                    break;
                                case 0x03:
                                    //memcpy(&pData[nIndex], &sAc_Param.nMeterProNum[0], LEN_METER_GLBH);
                                    memset(&pData[nIndex], 0, LEN_METER_GLBH);
                                    nIndex += LEN_METER_GLBH;
                                    break;
                                case 0x04:
                                    break;
                                case 0x05:
                                    break;
                                case 0x06:
                                    break;
                                case 0x07:
                                    break;
                                case 0x08:
                                    break;
                                case 0x09:
                                case 0x0A:
    //                                pData[nIndex++] = 0x00;
    //                                pData[nIndex++] = 0x32;
    //                                pData[nIndex++] = 0x00;
                                    INT32U2BCD(pDataTable->nPulseConst, 6, &pData[nIndex]);
                                    nIndex += 3;
                                    break;
                                case 0x0B:
                                    break;
                                case 0x0C:
                                    break;
                                case 0x0D:    //协议版本号
                                { 
                                    INT8U       nBuf[20]    = {0};
                                    INT8U*      pBuf        =&nBuf[0];
                                    
                                    memset(pBuf, 0, sizeof(pBuf));
                                    strcat((char*)pBuf, (char*)"VM");
                                    strcat((char*)pBuf, (char*)PRODUCT);
                                    #ifdef MIN_PRODUCT
                                    strcat((char*)pBuf, (char*)MIN_PRODUCT);
                                    #endif
                                    strcat((char*)pBuf, (char*)"-");
                                    strcat((char*)pBuf, (char*)MAIN_VERSION_INFO);
                                    strcat((char*)pBuf, (char*)VERSION_INFO);
                                    #ifdef MIN_VERSION_INFO
                                    strcat((char*)pBuf, (char*)"-");
                                    strcat((char*)pBuf, (char*)MIN_VERSION_INFO);
                                    #endif
                                    memcpy((INT8U*)&pData[nIndex], pBuf, 16);
                                    nIndex +=  16;
                                    break;
                                }
                                default:
                                    *pIsExist = 0x02;
                                break;
                            }
                            break;
                        case 0x05:
                            if(0xff == pDI01[0])
                            {
                                for(i=0; i<7; i++)
                                {
                                    pData[nIndex++] = (INT8U)(pDataTable->sRealInstant.bsRunStat[i]&0x00ff);
                                    pData[nIndex++] = (INT8U)((pDataTable->sRealInstant.bsRunStat[i]&0xff00)/256);
                                }
                            }
                            else
                            {
                                if((pDI01[0] > 0)&&(pDI01[0] <= 7))
                                {
                                    pData[nIndex++] = (INT8U)(pDataTable->sRealInstant.bsRunStat[(pDI01[0] - 1)]&0x00ff);
                                    pData[nIndex++] = (INT8U)((pDataTable->sRealInstant.bsRunStat[(pDI01[0] - 1)]&0xff00)/256);
                                }
                                else
                                {
                                    *pIsExist = 0x02;
                                }
                            }
                            break;
                        case 0x06:
                            switch(pDI01[0])
                            {
                                case 0xff:
                                    pData[nIndex++] = sAc_Param.nEpCompFlag;
                                    pData[nIndex++] = sAc_Param.nEq1CompFlag;
                                    pData[nIndex++] = sAc_Param.nEq2CompFlag;
                                    break;
                                case 0x01:
                                    pData[nIndex++] = sAc_Param.nEpCompFlag;
                                    break;
                                case 0x02:
                                    pData[nIndex++] = sAc_Param.nEq1CompFlag;
                                    break;
                                case 0x03:
                                    pData[nIndex++] = sAc_Param.nEq2CompFlag;
                                    break;
                                default:
                                    *pIsExist = 0x02;
                                break;
                            }
                            break;
                        case 0x07:
                            switch(pDI01[0])
                            {
                                case 0xff:
                                    pData[nIndex++] = sAc_Param.nIR1Baud;
                                    pData[nIndex++] = sAc_Param.nIR2Baud;
                                    pData[nIndex++] = sAc_Param.nCom1Baud;
                                    pData[nIndex++] = sAc_Param.nCom2Baud;
                                    pData[nIndex++] = sAc_Param.nCom3Baud;
                                    break;
                                case 0x01:
                                    pData[nIndex++] = sAc_Param.nIR1Baud;
                                    break;
                                case 0x02:
                                    pData[nIndex++] = sAc_Param.nIR2Baud;
                                    break;
                                case 0x03:
                                    pData[nIndex++] = sAc_Param.nCom1Baud;
                                    break;
                                case 0x04:
                                    pData[nIndex++] = sAc_Param.nCom2Baud;
                                    break;
                                case 0x05:
                                    pData[nIndex++] = sAc_Param.nCom3Baud;
                                    break;
                                default:
                                    *pIsExist = 0x02;
                                break;
                            }
                            break;
                        case 0x08:
                            switch(pDI01[0])
                            {
                                case 0xff:
                                    pData[nIndex++] = sAc_Param.nHolidayFlag;
                                    pData[nIndex++] = sAc_Param.nHolidayTable;
                                    break;
                                case 0x01:
                                    pData[nIndex++] = sAc_Param.nHolidayFlag;
                                    break;
                                case 0x02:
                                    pData[nIndex++] = sAc_Param.nHolidayTable;
                                    break;
                                default:
                                    *pIsExist = 0x02;
                                break;
                            }
                            break;
        //				case 0x09:
        //					switch(pDI01[0])
        //					{
        //						case 0xff:
        //							break;
        //						case 0x01:
        //							break;
        //						case 0x02:
        //							break;
        //						default:
        //							*pIsExist = 0x02;
        //						break;
        //					}
        //					break;
        //				case 0x0A:
        //					switch(pDI01[0])
        //					{
        //						case 0xff:
        //							break;
        //						case 0x01:
        //							break;
        //						case 0x02:
        //							break;
        //						case 0x03:
        //							break;
        //						case 0x04:
        //							break;
        //						case 0x05:
        //							break;
        //						case 0x06:
        //							break;
        //						case 0x07:
        //							break;
        //						default:
        //							*pIsExist = 0x02;
        //						break;
        //					}
        //					break;
                        case 0x0B:
                            switch(pDI01[0])
                            {
                                case 0xff:
                                    memcpy(&pData[nIndex], &sAc_Param.sMonthJS1, sizeof(FORMAT24));
                                    nIndex += sizeof(FORMAT24);
                                    memcpy(&pData[nIndex], &sAc_Param.sMonthJS2, sizeof(FORMAT24));
                                    nIndex += sizeof(FORMAT24);
                                    memcpy(&pData[nIndex], &sAc_Param.sMonthJS3, sizeof(FORMAT24));
                                    nIndex += sizeof(FORMAT24);
                                    break;
                                case 0x01:
                                    memcpy(&pData[nIndex], &sAc_Param.sMonthJS1, sizeof(FORMAT24));
                                    nIndex += sizeof(FORMAT24);
                                    break;
                                case 0x02:
                                    memcpy(&pData[nIndex], &sAc_Param.sMonthJS2, sizeof(FORMAT24));
                                    nIndex += sizeof(FORMAT24);
                                    break;
                                case 0x03:
                                    memcpy(&pData[nIndex], &sAc_Param.sMonthJS3, sizeof(FORMAT24));
                                    nIndex += sizeof(FORMAT24);
                                    break;
                                default:
                                    *pIsExist = 0x02;
                                break;
                            }
                            break;
                        case 0x0E:
                            switch(pDI01[0])
                            {
                                case 0xff:
                                    memcpy(&pData[nIndex], &sAc_Param.sPosPowLimt, sizeof(FORMAT23));
                                    nIndex += sizeof(FORMAT23);
                                    memcpy(&pData[nIndex], &sAc_Param.sNegPowLimt, sizeof(FORMAT23));
                                    nIndex += sizeof(FORMAT23);
                                    memcpy(&pData[nIndex], &sAc_Param.sUpLimtVolt, sizeof(FORMAT07));
                                    nIndex += sizeof(FORMAT07);
                                    memcpy(&pData[nIndex], &sAc_Param.sLowLimtVolt, sizeof(FORMAT07));
                                    nIndex += sizeof(FORMAT07);
                                    break;
                                case 0x01:
                                    memcpy(&pData[nIndex], &sAc_Param.sPosPowLimt, sizeof(FORMAT23));
                                    nIndex += sizeof(FORMAT23);
                                    break;
                                case 0x02:
                                    memcpy(&pData[nIndex], &sAc_Param.sNegPowLimt, sizeof(FORMAT23));
                                    nIndex += sizeof(FORMAT23);
                                    break;
                                case 0x03:
                                    memcpy(&pData[nIndex], &sAc_Param.sUpLimtVolt, sizeof(FORMAT07));
                                    nIndex += sizeof(FORMAT07);
                                    break;
                                case 0x04:
                                    memcpy(&pData[nIndex], &sAc_Param.sLowLimtVolt, sizeof(FORMAT07));
                                    nIndex += sizeof(FORMAT07);
                                    break;
                                default:
                                    *pIsExist = 0x02;
                                break;
                            }
                            break;

                        default:
                            *pIsExist = 0x02;
                            break;
                    }
                    break;
                    case 0x04010000:
                    case 0x04020000:
                        if(pDI01[1] == 0)
                        {
                            if(pDI01[0] == 0)
                            {
                                if (sAc_Param.nYearSegmNum > MAX_YEAR_SEGM_NUM)
                                {
                                    sAc_Param.nYearSegmNum = MAX_YEAR_SEGM_NUM;
                                }
                                memcpy(&pData[nIndex], &sAc_Param.sYearSegmTable[pDI01[2]-1][0], sAc_Param.nYearSegmNum*sizeof(MM_DD_NN));
                                nIndex += sAc_Param.nYearSegmNum*sizeof(MM_DD_NN);
                            }
                            else
                            {
                                if(pDI01[0] <= MAX_DAY_TABLE_NUM)
                                {
                                    if (sAc_Param.nDaySegmNum > MAX_DAY_SEGM_NUM)
                                    {
                                        sAc_Param.nDaySegmNum = MAX_DAY_SEGM_NUM;
                                    }
                                    memcpy(&pData[nIndex], &sAc_Param.sDaySegmTable[pData[2]-1][pData[0]-1][0], sAc_Param.nDaySegmNum*sizeof(hh_mm_NN));
                                    nIndex += sAc_Param.nDaySegmNum*sizeof(hh_mm_NN);
                                }
                                else
                                {
                                    *pIsExist = 0x02;
                                }
                            }
                        }
                        else
                        {
                            *pIsExist = 0x02;
                        }
                        break;
                    case 0x04030000:
                        if (pDI01[1] == 0)
                        {
                            if ((pDI01[0] > 0) && (pDI01[0] < 254))
                            {
                                memcpy(&pData[nIndex], &sAc_Param.sHolidayTable[pDI01[0]-1], sizeof(YY_MM_DD_NN));
                                nIndex += sizeof(YY_MM_DD_NN);
                            }
                        }
                        else
                        {
                            *pIsExist = 0x02;
                        }
                        break;
                        
                    default:
                        *pIsExist = 0x02;
                
                if(nDI == 0xA5000102)
                {                                       //读电表单板测试结果           
                    pData[nIndex] = g_nTestStep;
                    nIndex++;
                    memcpy(&pData[nIndex], &g_nErrBuf[0], 2);
                    nIndex += 2;
                   
                   *pIsExist = 0;
                }
                
                if(nDI == 0xA7000101)
                {                                       //电表生产
                  
                    memset(&nProductNum[0], 0, sizeof(nProductNum));
                    MemReadAcPrm(F21, &nProductNum[0], 32); 
                    memcpy(&pData[nIndex], &nProductNum[0], 6);
                    nIndex += 6;
                   
                   *pIsExist = 0;
                }
                
                if(nDI == 0xA5000103)
                {                                       //读电表功率方向           
                 //   if((pDataTable->sRealInstant.sRealP[1].nSint)&0x80)                                         //判断功率方向
                      if(g_nFlg2)
                      {
                        pData[nIndex] = 1;
                        
                        g_nFlg2 = 0;
                      }
                      else
                      {
                         pData[nIndex] = 0;
                      }
                    nIndex ++;
                   
                   *pIsExist = 0;
                }
                if(nDI == 0xA9000101)
                {
                    //MemReadAcPrm(F51, &g_nHardType, sizeof(g_nHardType));
                    memset(&sAc_Param, 0, sizeof(MEM_AC_PRM));
                    MemReadAcPrm(F44, &sAc_Param.nHardType, sizeof(sAc_Param.nHardType));
                    switch(sAc_Param.nHardType)
                    {
                        case SINGLEPHASE_METER:
                        {
                            pData[nIndex] = 1; break;
                        }
                        case TRIPHASE_METER_220:
                        {
                            pData[nIndex] = 2; break;
                        }
                        case TRIPHASE_METER_57:
                        {
                            pData[nIndex] = 3; break;
                        }
                        case TRIPHASE_METER_100:
                        {
                            pData[nIndex] = 4; break;
                        }
                        case TRIPHASE_METER_220ZT:
                        {
                            pData[nIndex] = 5; break;
                        }
                        case TRIPHASE_METER_57U:
                        {
                            pData[nIndex] = 6; break;
                        }
                        case TRIPHASE_METER_100U:
                        {
                            pData[nIndex] = 7; break;
                        }
                        default:
                            pData[nIndex] = 0xFF;break;
                    }
                     nIndex ++;
                   
                   *pIsExist = 0;
                }
                
               if(nDI == 0xA6000001)
                {                                       //读电表数据，配合单相表错接线仿真仪使用           
                    memcpy(pData + nIndex, &pDataTable->sRealInstant.sRealVolt[0], 2);
                    nIndex += 2;
                    memcpy(pData + nIndex, &pDataTable->sRealInstant.sRealCurr[0], 3);
                    nIndex += 3;
                    memcpy(pData + nIndex, &pDataTable->sRealInstant.sRealCurr[3], 3);
                    nIndex += 3;
                    memcpy(pData + nIndex, &pDataTable->sRealInstant.sRealP[1], 3);
                    nIndex += 3;
                    memcpy(pData + nIndex, &pDataTable->sRealInstant.sRealPf[1], 2);
                    nIndex += 2;
                    *pIsExist = 0;
                }
                
                        break;
        }
          
    }
    //存储器故障模拟
    if(0x10==(g_MeterInsideWrong[0]&0x10))
    {
       if((pDI01[3]==0)||(pDI01[3]==5)) //会清掉一些无关电量的数据
       {
          memset(pData + 4,0,((MAX_FEE_NUM+1)*sizeof(FORMAT11))); 
       }       
    }
    
    
    
    if ((nIndex<=4)&&(*pIsExist == 0))
	{
		*pIsExist = 0x01;
	}
	nDataLen = nIndex;

	return nDataLen;
}



/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 读取电表数据
*********************************************************************************/
INT8U Dlt07Code11(const INT8U *pValidBuf, INT8U nValidLen, INT8U *pSendBuf)
{
        INT8U	nIsExist = 0;
	INT8U	i;
	INT8U	nData[50];
	INT16U	nDataLen = 0;
	INT8U	nSendlen = 0;
	INT8U	nCtrlCode = 0;
	INT8U	nDI01[4];

	memset(nData,0,sizeof(nData));
	memset(nDI01,0,sizeof(nDI01));

	nCtrlCode = pValidBuf[8];
	nDataLen = pValidBuf[9];
    
    if (nDataLen < 4 || nDataLen>(nValidLen-12))
	{
		nIsExist = 0x01;
	}
    else
	{
		for(i=0; i<nDataLen; i++)
		{
			nData[i] = pValidBuf[10 + i] - 0x33;
		}
		nDI01[0] = nData[0];
		nDI01[1] = nData[1];
		nDI01[2] = nData[2];
		nDI01[3] = nData[3];
		nSendlen = Dlt07SendResp(nDI01,&pSendBuf[2],&nIsExist);
	}

	if (nIsExist)
	{
		pSendBuf[0] = nCtrlCode | 0xC0;
		pSendBuf[1] = 1;
		pSendBuf[2] = nIsExist;
	}
	else
	{
		pSendBuf[0] = nCtrlCode | 0x80;
		pSendBuf[1] = nSendlen;
	}
	return (pSendBuf[1]+2);
}



/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 读取电表通信地址
*********************************************************************************/
INT8U Dlt07Code13(const INT8U *pValidBuf, INT8U nValidLen, INT8U *pSendBuf)
{
	INT8U	nTmp = 0;
	INT8U	nCtrlCode=0;
	INT16U	nDataLen=0;
	INT8U	nAddr[LEN_METER_ADDR];
	INT8U	nBroadaddr[LEN_METER_ADDR];
	INT8S	nSuccflag = RET_ERR_ERROR;

	memset(nAddr,0,LEN_METER_ADDR);
	memset(nBroadaddr,0xAA,LEN_METER_ADDR);							/*广播地址*/

	memcpy(nAddr,pValidBuf+1,6);
	nCtrlCode=pValidBuf[8];
	nDataLen=pValidBuf[9];
	if (((memcmp(nAddr,nBroadaddr,LEN_METER_ADDR) != 0)&&(TRUE !=IsAddrFieldVal(&nAddr[0], 6)))||(nDataLen != 0))			/*判断地址是否广播地址和长度是否正确*/
	{
		nTmp = 0x01;
	}
	else
	{
		//nSuccflag = MemReadACPrm(MEM_OBJ_AC_PRM, 0,  AC_COM_ADDR_ID, &nAddr, LEN_METER_ADDR);
        if (MemReadAcPrm(F19, &nAddr[0], LEN_METER_ADDR) > 0)
        {
            nSuccflag = RET_ERR_OK;
        }
        
		if (nSuccflag != RET_ERR_OK)
		{
			nTmp = 0x01;
		}
		else
		{
			memcpy(&pSendBuf[2], &nAddr, LEN_METER_ADDR);
		}
	}
	if (nTmp != 0)
	{
		pSendBuf[0] = nCtrlCode | 0xC0;
		pSendBuf[1] = 1;
		pSendBuf[2] = nTmp;
	}
	else
	{
		pSendBuf[0] = nCtrlCode | 0x80;
		pSendBuf[1] = LEN_METER_ADDR;
	}
	return (pSendBuf[1]+2);
}

/********************************************************************************
* 函数名:DltParamSupported
* 输　入:INT8U* DI01,INT16U Length
* 输　出:
* 返  回:
* 功  能:判断规约参数帧长是否满足
*********************************************************************************/
INT8S Dlt07ParamSupported(const INT8U *pDI01, INT8U nLength)
{
	INT8S	nTmp = RET_ERR_ERROR;

	switch(((pDI01[3]<<24)|(pDI01[2]<<16)|(pDI01[1]<<8)|pDI01[0]))
	{
		case 0x0001FF00:case 0x0002FF00:case 0x0003FF00:case 0x0004FF00:case 0x0005FF00:
		case 0x0006FF00:case 0x0007FF00:case 0x0008FF00:case 0x0000FF00:
        case 0x00010100:case 0x00010200:case 0x00010300:case 0x00010400:
        case 0x00020100:case 0x00020200:case 0x00020300:case 0x00020400:
			if ((nLength < sizeof(FORMAT11)) || (nLength > ((MAX_FEE_NUM + 1) * sizeof(FORMAT11))))
			{
				nTmp = RET_ERR_ERROR;
			}
			else
			{
				nTmp = RET_ERR_OK;
			}
			break;
		
		case 0x04000103:case 0x04000104:
		case 0x04000201:case 0x04000202:case 0x04000203:case 0x04000204:case 0x04000206:
		case 0x04000301:case 0x04000302:case 0x04000303:case 0x04000304:case 0x04000305:
		case 0x04000601:case 0x04000602:case 0x04000603:
		case 0x04000701:case 0x04000702:case 0x04000703:case 0x04000704:case 0x04000705:
        case 0x04000801:case 0x04000802:case 0xA1000101:case 0xA4000101:case 0xA2000101:
        case 0xA7000101:case 0xA8000101:case 0xA9000101:case 0xB2000101:case 0xB2000201:
        case 0xB2000301:case 0xB2000401:case 0xB2000501: case 0xB2000601:case 0xB2000701:
        case 0xB2000801:  
		//case 0x04000901:case 0x04000902:
			if (nLength < 1)
			{
				nTmp = RET_ERR_ERROR;
			}
			else
			{
				nTmp = RET_ERR_OK;
			}
			break;
        case 0xA6000101: case 0xAA000101: case 0xA6000102: case 0xA6000103:
            nTmp = RET_ERR_OK;
            break;
		case 0x04000205:
		//case 0x04000A02:case 0x04000A03:case 0x04000A04:case 0x04000A05:case 0x04000A06:case 0x04000A07:
		case 0x04000B01:case 0x04000B02:case 0x04000B03:
		//case 0x04000D01:case 0x04000D02:case 0x04000D03:case 0x04000D04:case 0x04000D05:case 0x04000D06:
		//case 0x04000D07:case 0x04000D08:case 0x04000D09:case 0x04000D0A:case 0x04000D0B:case 0x04000D0C:
		case 0x04000E03:case 0x04000E04:
		case 0x040008FF:case 0x040009FF:
			if (nLength < 2)
			{
				nTmp = RET_ERR_ERROR;
			}
			else
			{
				nTmp = RET_ERR_OK;
			}
			break;

		case 0x04000102:
		case 0x04000E01:case 0x04000E02:
		case 0x040006FF:
			if (nLength < 3)
			{
				nTmp = RET_ERR_ERROR;
			}
			else
			{
				nTmp = RET_ERR_OK;
			}
			break;

		case 0x04000101:
		case 0x04000A01:
		case 0x04000C01:case 0x04000C02:case 0x04000C03:case 0x04000C04:case 0x04000C05:
		case 0x04000C06:case 0x04000C07:case 0x04000C08:case 0x04000C09:case 0x04000C0A:
			if (nLength < 4)
			{
				nTmp = RET_ERR_ERROR;
			}
			else
			{
				nTmp = RET_ERR_OK;
			}
			break;

		case 0x04000106:case 0x04000107:
		case 0x040003FF:case 0x040007FF:
			if (nLength < 5)
			{
				nTmp = RET_ERR_ERROR;
			}
			else
			{
				nTmp = RET_ERR_OK;
			}
			break;

		case 0x04000401:case 0x04000402:
		case 0x04000BFF:
			if (nLength < 6)
			{
				nTmp = RET_ERR_ERROR;
			}
			else
			{
				nTmp = RET_ERR_OK;
			}
			break;

		case 0x040002FF:
			if (nLength < 7)
			{
				nTmp = RET_ERR_ERROR;
			}
			else
			{
				nTmp = RET_ERR_OK;
			}
			break;

		case 0x04000EFF:
			if (nLength < 10)
			{
				nTmp = RET_ERR_ERROR;
			}
			else
			{
				nTmp = RET_ERR_OK;
			}
			break;

		case 0x04000AFF:
			if (nLength < 16)
			{
				nTmp = RET_ERR_ERROR;
			}
			else
			{
				nTmp = RET_ERR_OK;
			}
			break;

		case 0x040001FF:
			if (nLength < 21)
			{
				nTmp = RET_ERR_ERROR;
			}
			else
			{
				nTmp = RET_ERR_OK;
			}
			break;

		case 0x04000DFF:
			if (nLength < 24)
			{
				nTmp = RET_ERR_ERROR;
			}
			else
			{
				nTmp = RET_ERR_OK;
			}
			break;

		case 0x04000403:
			if (nLength < 32)
			{
				nTmp = RET_ERR_ERROR;
			}
			else
			{
				nTmp = RET_ERR_OK;
			}
			break;

//		case 0x04000CFF:
//			if (nLength < 40)
//			{
//				nTmp = RET_ERR_ERROR;
//			}
//			else
//			{
//				nTmp = RET_ERR_OK;
//			}
//			break;

		case 0x04010000:
		case 0x04010001:case 0x04010002:case 0x04010003:case 0x04010004:
		case 0x04010005:case 0x04010006:case 0x04010007:case 0x04010008:
		case 0x04020000:
		case 0x04020001:case 0x04020002:case 0x04020003:case 0x04020004:
		case 0x04020005:case 0x04020006:case 0x04020007:case 0x04020008:
			if (nLength < 42)
			{
				nTmp = RET_ERR_ERROR;
			}
			else
			{
				nTmp = RET_ERR_OK;
			}
			break;

		case 0x040004FF:
			if (nLength < 112)
			{
				nTmp = RET_ERR_ERROR;
			}
			else
			{
				nTmp = RET_ERR_OK;
			}
			break;
               case 0xA5000101:case 0xA5000103:
                        if (0 == nLength)
			{
				nTmp = RET_ERR_OK;
			}
			else
			{
				nTmp = RET_ERR_ERROR;
			}
			break;
       case 0xB1000108:
         if( nLength > 0 )
          {
             nTmp = RET_ERR_OK;
          }
          else
          {
             nTmp = RET_ERR_ERROR;
          }
          break;
       case 0xB1000001:case 0xB1000002:case 0xB1000006:case 0xB1000007:case 0xB1000008:
       case 0xB1000009:case 0xB100000A:case 0xB100000B:case 0xB100000C:case 0xB100000D:
       case 0xB100000E:case 0xB100000F:case 0xB1000101:case 0xB1000102:case 0xB1000103:
       case 0xB1000109:case 0xB100010A:case 0xB1000205: case 0xB100010B:case 0xB100010C:
       case 0xB1000302: case 0xB1000301:case 0xB1000303: case 0xB1000304: case 0xB1000305: 
          if(nLength==1)
          {
             nTmp = RET_ERR_OK;
          }
          else
          {
             nTmp = RET_ERR_ERROR;
          }
          break;
       case 0xB1000003:case 0xB1000004:case 0xB1000005:case 0xB1000104:case 0xB1000105:
       case 0xB1000106:case 0xB1000107:
          if(nLength==3)
          {
             nTmp = RET_ERR_OK;
          }
          else
          {
             nTmp = RET_ERR_ERROR;
          }
          break;
		default:
			if (((pDI01[3]<<24)|(pDI01[2]<<16)|(pDI01[1]<<8)|0x00) == 0x04030000)
			{
				if (nLength < 4)
				{
					nTmp = RET_ERR_ERROR;
				}
				else
				{
					nTmp = RET_ERR_OK;
				}
			}
			else
			{
				nTmp = RET_ERR_ERROR;
			}
			break;
	}
	return nTmp;
}


INT32S  Dlt07SetDayData(INT8U* pData)
{
    INT8U       nDays       = 0;
    INT8U       nDataType   = 0;
    INT8U       i           = 0;
    INT8U       j           = 0;
    INT8U       Temp        = 0;
    INT8U       nIndex      = 0;
    INT8U       nErr        = 0;
    INT32U      nValue      = 0;
    STD_TIME    sTime;
    pSTD_TIME   pTime = &sTime;
    FORMAT11    sPPEnergy[1+MAX_FEE_NUM];
    STD_TIME    sTmpTime;
    STD_TIME    sTmpTime2;
    MEM_AC_DAY_DATA sAcDayData;
    
    memset(pTime, 0, sizeof(STD_TIME));
    memset(&sTmpTime, 0, sizeof(sTmpTime));
    memset(&sTmpTime2, 0, sizeof(sTmpTime2));
    GetSysClk(&sTime);
    nDays       = pData[nIndex++];      //日天数
    
    //先将日数据清零
    if((nDays&0xF0) == 0x00)
    {
        AcDayDataClr();
    }
    nDays = nDays&0x0F; //22.2.29
    nDataType   = pData[nIndex++];      //数据属性（0：正向有功，1：反向有功）
    for (i = 0; i < nDays; i++)
    {
        memcpy(&sTmpTime.nDay, &pData[nIndex], 3);
        nIndex += 3;
        //获取下一日数据时标
        GetNextDay(&sTmpTime2, &sTmpTime);
        memcpy(&sPPEnergy[0], &pData[nIndex], 20);
        nIndex += 20; 
        for (j = 0; j < MAX_AC_DAY_DATA_NUM; j++)
        {
            memset(&sAcDayData, 0, sizeof(sAcDayData));
            MemReadAcHistDataProc(F1, j, &sAcDayData,sizeof(MEM_AC_DAY_DATA));
            if (0 == memcmp(&sAcDayData.sTime.nDay, &sTmpTime2.nDay, sizeof(FORMAT20)))
            {
                if (IsAllEqualVal((INT8U*)&sPPEnergy[0], 20, 0))
                {
                    //memset(&sAcDayData.sTime, 0, sizeof(FORMAT15));
                }
                //覆盖更新
                if (nDataType)
                {
                    memcpy(&sAcDayData.sAcEnergy.sNPEnergy[0], &sPPEnergy[0], 20);
                }
                else
                {
                    memcpy(&sAcDayData.sAcEnergy.sPPEnergy[0], &sPPEnergy[0], 20);
                }
                
                MemWriteAcHistDataProc(F1, j, &sAcDayData, sizeof(sAcDayData),0);
                break;
            }
        }
        if (j >= MAX_AC_DAY_DATA_NUM)
        {
            memset(&sAcDayData, 0, sizeof(sAcDayData));
            if (nDataType)
            {
                memcpy(&sAcDayData.sAcEnergy.sNPEnergy[0], &sPPEnergy[0], 20);
            }
            else
            {
                memcpy(&sAcDayData.sAcEnergy.sPPEnergy[0], &sPPEnergy[0], 20);
            }
            if (!IsAllEqualVal((INT8U*)&sPPEnergy[0], 20, 0))
            {
                memcpy(&sAcDayData.sTime.nDay, &sTmpTime2.nDay, 3);
            }
            MemWriteAcHistDataProc(F1, 0, &sAcDayData, sizeof(sAcDayData),1);
        }
        
        //设置当前底数
        if (nDays - 1 == i)
        {
            //nValue = sPPEnergy[4] * 1000000 + sPPEnergy[3] * 10000 + sPPEnergy[2] * 100 + sPPEnergy[1];
            //nValue += 200;
            //sPPEnergy[0] = nValue %100;
            //sPPEnergy[1] = (nValue / 100) % 100;
            //sPPEnergy[2] = (nValue / 10000) % 100;
            //sPPEnergy[3] = nValue / 1000000;
            
            OSSemPend(g_pSem[SEM_ACENWR_CTRL_IDX], 0, &nErr);
            memset(&pSavDataTable->sCombEPulse, 0, sizeof(COMB_ENERGY_DATA));
            if (nDataType)
            {
                memset(&pSavDataTable->sEnergyData[2].nEnergy[0], 0, sizeof(ENERGY_DATA));
                memcpy(&pSavDataTable->sAcEnergy.sNPEnergy, &sPPEnergy[0], sizeof(FORMAT11) * (1+MAX_FEE_NUM));
            }
            else
            {
                memset(&pSavDataTable->sEnergyData[1].nEnergy[0], 0, sizeof(ENERGY_DATA));
                memset(&pSavDataTable->sEnergyData[0].nEnergy[0], 0, sizeof(ENERGY_DATA));
                memcpy(&pSavDataTable->sAcEnergy.sPPEnergy, &sPPEnergy[0], sizeof(FORMAT11) * (1+MAX_FEE_NUM));
                memcpy(&pSavDataTable->sAcCombEnergy.sPPEnergy, &sPPEnergy[0], sizeof(FORMAT11) * (1+MAX_FEE_NUM));
            }
            pSavDataTable->nENSumChk = 0x55aa66bb + CheckINT32USum((INT32U*)&pSavDataTable->sAcEnergy, 0, (sizeof(MT_DGN_ENERGY_DATA)
             + sizeof(MT_DGN_Q_ENERGY_DATA) + sizeof(MT_DGN_TRI_ENERGY_DATA) + sizeof(MT_DGN_TRI_Q_EN_DATA) + sizeof(MT_DGN_C_ENERGY_DATA))/4);					
            pSavDataTable->nEPSumChk = CheckINT32USum((INT32U*)&pSavDataTable->sEnergyData[0], 0, (8*sizeof(ENERGY_DATA) +
                                       2*MAX_PHASE_NUM*sizeof(PH_ENERGY_DATA) + sizeof(COMB_ENERGY_DATA))/4);
            WriteAcENData();
            OSSemPost(g_pSem[SEM_ACENWR_CTRL_IDX]);
            //增加立即同步数据FRam（第1备份）至Flash（第2备份）
            ReadAcENData();
        }
    }
    
    return RET_ERR_OK;
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 保存电表参数
*********************************************************************************/
INT8U Dlt07SaveParam(const INT8U *pData, INT8U nLen)
{
    INT8U	nIsExist = 0;
	INT8U	nIndex = 0;
	INT8U	nPwd = 0xFF;
	INT8U	nPwdRight = 0xFF;
	INT8S	nSuccflag = RET_ERR_ERROR;
	INT8U	nNum = 0;
	INT8U	nChanger = 0;
    INT8U	nState = 0x55;
    INT8U   nErr = 0;
    INT32U	nDI = 0;
    INT8U	nBuff[256];
    INT8U   nProductNum[32] = {0};
    INT8U   nNotPrmSetFlg   = 0;
    INT32U   nHardType;
	INT8U   i    = 0;
    INT32U  nTmp = 0;
   	STD_TIME	sTime;
	MEM_AC_PRM	sAc_Param;
    MEM_AC_CAL_PRM  sJbParam;
    
    memset(&sAc_Param, 0, sizeof(MEM_AC_PRM));
	if (MemReadAcPrm(FEND, &sAc_Param, sizeof(sAc_Param)) > 0)
    {
        nSuccflag = RET_ERR_OK;
    }
    if (RET_ERR_OK == nSuccflag)
    {
        nDI = ((pData[3]<<24)|(pData[2]<<16)|(pData[1]<<8)|pData[0]);
		nIndex += 4;
		nPwdRight = pData[nIndex++];
		pData += 12;
		nIndex = 0;
        
        if(g_MeterPowerOff && nDI!=0xB1000102)
        {      
            return 0;
        }
        
        switch(nDI)
        {
            //扩展参数
            case 0xA1000101:        //485口子规约切换
            {
                g_pMem->sXRamData.nRdMtProtFlg = pData[nIndex];
                //存储
            	g_nFlgProt = 1;
                nNotPrmSetFlg = 1;
                break;
            }
            case 0xA4000101:        //校表参数初始化、交采参数初始化
            {
                if (3== pData[nIndex])
                {
                    AcPrmPrmInit();
                }
                else if (2== pData[nIndex])
                {
                    
                }
                else if (1== pData[nIndex])
                {
                    
                }
                nNotPrmSetFlg = 1;
                break;
            }
            case 0xA2000101:        //设置日冻结数据
            {
                g_nSetDayDataFlg = 1;
                memcpy(&g_nDltSetDayBuf[0], &pData[nIndex], sizeof(g_nDltSetDayBuf));
                nNotPrmSetFlg = 1;
                break;
            }
            case 0xA7000101:        //电表生产编号
            {
                memset(&nProductNum[0], 0, sizeof(nProductNum));
                memcpy(&nProductNum[0], &pData[nIndex], 6);
                MemWriteAcPrm(F21, &nProductNum[0], 32);
                nNotPrmSetFlg = 1;
                break;
            }
            case 0xA8000101:        //一键恢复初始状态
            {
                g_nOneKeyRecFlg = 1;
 
                //根据编号恢复表地址
                memset(&nProductNum[0], 0, sizeof(nProductNum));
                MemReadAcPrm(F21, &nProductNum[0], 32);
                if (!IsAllEqualVal(&nProductNum[0], 6, 0))
                {
                    MemWriteAcPrm(F19, &nProductNum[0], 6);
                    MemWriteAcPrm(F20, &nProductNum[0], 6);//同步恢复表号
                    memcpy(&g_nMemAddr, &nProductNum[0], 6);
                }
                
                AcCalcJustPluseConst(0);
                //设置时钟
                sTime.nSec 	= pData[nIndex++];
                sTime.nMin 	= pData[nIndex++];
                sTime.nHour = pData[nIndex++];
                sTime.nDay 	= pData[nIndex++];
                sTime.nMon 	= pData[nIndex++];
                sTime.nYear = pData[nIndex++];
                SyncSysClk(&sTime);
                
                nNotPrmSetFlg = 1;
                break;
            }
            case 0xA9000101:            //电表类型设置
            {                
                switch(pData[nIndex])
                {
                    case 1: g_nHardType = SINGLEPHASE_METER; break;
                    case 2: g_nHardType = TRIPHASE_METER_220; break;
                    case 3: g_nHardType = TRIPHASE_METER_57;  break;
                    case 4: g_nHardType = TRIPHASE_METER_100; break;
                    case 5: g_nHardType = TRIPHASE_METER_220ZT; break;
                    case 6: g_nHardType = TRIPHASE_METER_57U;  break;
                    case 7: g_nHardType = TRIPHASE_METER_100U; break;
                    default:  break;   
                }
                g_nFlgReBoot = 1;   
                sAc_Param.nHardType = g_nHardType;
                MemWriteAcPrm(F44, &g_nHardType, sizeof(g_nHardType));
                AcCalcJustPluse();
                nNotPrmSetFlg = 1;
                break;
            }
            
            case 0xAA000101:            //文件系统格式化
            {    
                FlashFileBlkChk(1);
                //文件系统初始化完后，进行校表参数初始化
                AcPrmPrmInit();
                break;
            }
            
            
            //设置单费率,正向有功
            case 0x00010100:
            case 0x00010200:
            case 0x00010300:
            case 0x00010400:
            {
                memcpy(&nBuff[0], &pData[nIndex], nLen - 12);
                if(!IsAllBcd(nBuff, nLen - 12))		/*BCD码有效性判断*/
				{
					nIsExist = 0x01;
				}
                else
                {
                    nNum = (nDI >> 8 - 0x00000100);
                    OSSemPend(g_pSem[SEM_ACENWR_CTRL_IDX], 0, &nErr);
                    memset(&pSavDataTable->sCombEPulse, 0, sizeof(COMB_ENERGY_DATA));
                    memset(&pSavDataTable->sEnergyData[nNum].nEnergy[0], 0, sizeof(ENERGY_DATA));
                    
                    memcpy(&pSavDataTable->sAcEnergy.sPPEnergy[nNum], &nBuff[0], sizeof(FORMAT11));
                    //同时计算到总里面
                    for (i = 1; i <= 4; i++)
                    {
                        nTmp += BCD2INT32U((INT8U*)&pSavDataTable->sAcEnergy.sPPEnergy[i], 8);
                    }
                    INT32U2BCD(nTmp, 8, (INT8U*)&pSavDataTable->sAcEnergy.sPPEnergy[0]);
                    
                    //计算组合有功
                    for (i = 0; i < (MAX_FEE_NUM + 1); i++)
                    {
                        nTmp = BCD2INT32U((INT8U*)&pSavDataTable->sAcEnergy.sPPEnergy[i], 8);
                        nTmp += BCD2INT32U((INT8U*)&pSavDataTable->sAcEnergy.sNPEnergy[i], 8);
                        INT32U2BCD(nTmp, 8, (INT8U*)&pSavDataTable->sAcCombEnergy.sPPEnergy[i]);
                    }
                    
                    pSavDataTable->nENSumChk = 0x55aa66bb + CheckINT32USum((INT32U*)&pSavDataTable->sAcEnergy, 0, (sizeof(MT_DGN_ENERGY_DATA)
                     + sizeof(MT_DGN_Q_ENERGY_DATA) + sizeof(MT_DGN_TRI_ENERGY_DATA) + sizeof(MT_DGN_TRI_Q_EN_DATA) + sizeof(MT_DGN_C_ENERGY_DATA))/4);					
                    pSavDataTable->nEPSumChk = CheckINT32USum((INT32U*)&pSavDataTable->sEnergyData[0], 0, (8*sizeof(ENERGY_DATA) +
                                               2*MAX_PHASE_NUM*sizeof(PH_ENERGY_DATA) + sizeof(COMB_ENERGY_DATA))/4);
                    WriteAcENData();
                    OSSemPost(g_pSem[SEM_ACENWR_CTRL_IDX]);
                    //增加立即同步数据FRam（第1备份）至Flash（第2备份）
                    ReadAcENData();
                }
                break;
            }
            
            //反向有功
            case 0x00020100:
            case 0x00020200:
            case 0x00020300:
            case 0x00020400:
            {
                memcpy(&nBuff[0], &pData[nIndex], nLen - 12);
                if(!IsAllBcd(nBuff, nLen - 12))		/*BCD码有效性判断*/
				{
					nIsExist = 0x01;
				}
                else
                {
                    nNum = (nDI >> 8 - 0x00000200);
                    OSSemPend(g_pSem[SEM_ACENWR_CTRL_IDX], 0, &nErr);
                    memset(&pSavDataTable->sCombEPulse, 0, sizeof(COMB_ENERGY_DATA));
                    memset(&pSavDataTable->sEnergyData[nNum].nEnergy[0], 0, sizeof(ENERGY_DATA));
                    
                    memcpy(&pSavDataTable->sAcEnergy.sNPEnergy[nNum], &nBuff[0], sizeof(FORMAT11));
                    //同时计算到总里面
                    for (i = 1; i <= 4; i++)
                    {
                        nTmp += BCD2INT32U((INT8U*)&pSavDataTable->sAcEnergy.sNPEnergy[i], 8);
                    }
                    INT32U2BCD(nTmp, 8, (INT8U*)&pSavDataTable->sAcEnergy.sNPEnergy[0]);
                    
                    
                    //计算组合有功
                    for (i = 0; i < (MAX_FEE_NUM + 1); i++)
                    {
                        nTmp = BCD2INT32U((INT8U*)&pSavDataTable->sAcEnergy.sPPEnergy[i], 8);
                        nTmp += BCD2INT32U((INT8U*)&pSavDataTable->sAcEnergy.sNPEnergy[i], 8);
                        INT32U2BCD(nTmp, 8, (INT8U*)&pSavDataTable->sAcCombEnergy.sPPEnergy[i]);
                    }
                    
                    pSavDataTable->nENSumChk = 0x55aa66bb + CheckINT32USum((INT32U*)&pSavDataTable->sAcEnergy, 0, (sizeof(MT_DGN_ENERGY_DATA)
                     + sizeof(MT_DGN_Q_ENERGY_DATA) + sizeof(MT_DGN_TRI_ENERGY_DATA) + sizeof(MT_DGN_TRI_Q_EN_DATA) + sizeof(MT_DGN_C_ENERGY_DATA))/4);					
                    pSavDataTable->nEPSumChk = CheckINT32USum((INT32U*)&pSavDataTable->sEnergyData[0], 0, (8*sizeof(ENERGY_DATA) +
                                               2*MAX_PHASE_NUM*sizeof(PH_ENERGY_DATA) + sizeof(COMB_ENERGY_DATA))/4);
                    WriteAcENData();
                    OSSemPost(g_pSem[SEM_ACENWR_CTRL_IDX]);
                    //增加立即同步数据FRam（第1备份）至Flash（第2备份）
                    ReadAcENData();
                }
                break;
            }
            
            //设置表底度
            case 0x0001FF00:case 0x0002FF00:case 0x0003FF00:case 0x0004FF00:case 0x0005FF00:
			case 0x0006FF00:case 0x0007FF00:case 0x0008FF00://case 0x0000FF00:
                memcpy(&nBuff[0], &pData[nIndex], nLen - 12);
				if(!IsAllBcd(nBuff, nLen - 12))		/*BCD码有效性判断*/
				{
					nIsExist = 0x01;
				}
                else
                {
                    nNum = (INT8U)((nDI&0x00ff0000)>>16);
					OSSemPend(g_pSem[SEM_ACENWR_CTRL_IDX], 0, &nErr);
                    memset(&pSavDataTable->sEnergyData[nNum].nEnergy[0], 0, sizeof(ENERGY_DATA));
                    memset(&pSavDataTable->sCombEPulse, 0, sizeof(COMB_ENERGY_DATA));
                    switch (nNum)
                    {
                        //case 0:
                            //memcpy(&pSavDataTable->sAcCombEnergy.sPPEnergy, &nBuff[0], sizeof(FORMAT11) * (1+MAX_FEE_NUM));
                            //break;
                        case 1:
                            memcpy(&pSavDataTable->sAcEnergy.sPPEnergy, &nBuff[0], sizeof(FORMAT11) * (1+MAX_FEE_NUM));
                            //计算总
                            /*for (i = 1; i <= 4; i++)
                            {
                                nTmp += BCD2INT32U((INT8U*)&pSavDataTable->sAcEnergy.sPPEnergy[i], 8);
                            }
                            INT32U2BCD(nTmp, 8, (INT8U*)&pSavDataTable->sAcEnergy.sPPEnergy[0]);*/
                            break;
                        case 2:
                            memcpy(&pSavDataTable->sAcEnergy.sNPEnergy, &nBuff[0], sizeof(FORMAT11) * (1+MAX_FEE_NUM));
                            //计算总
                            for (i = 1; i <= 4; i++)
                            {
                                nTmp += BCD2INT32U((INT8U*)&pSavDataTable->sAcEnergy.sNPEnergy[i], 8);
                            }
                            INT32U2BCD(nTmp, 8, (INT8U*)&pSavDataTable->sAcEnergy.sNPEnergy[0]);
                            break;
                        case 3:
                            memcpy(&pSavDataTable->sAcEnergy.sPNEnergy, &nBuff[0], sizeof(FORMAT11) * (1+MAX_FEE_NUM));
                            break;
                        case 4:
                            memcpy(&pSavDataTable->sAcEnergy.sNNEnergy, &nBuff[0], sizeof(FORMAT11) * (1+MAX_FEE_NUM));
                            break;
                        case 5:
                            memcpy(&pSavDataTable->sAcQEnergy.sQ1Energy, &nBuff[0], sizeof(FORMAT11) * (1+MAX_FEE_NUM));
                            break;
                        case 6:
                            memcpy(&pSavDataTable->sAcQEnergy.sQ2Energy, &nBuff[0], sizeof(FORMAT11) * (1+MAX_FEE_NUM));
                            break;
                        case 7:
                            memcpy(&pSavDataTable->sAcQEnergy.sQ3Energy, &nBuff[0], sizeof(FORMAT11) * (1+MAX_FEE_NUM));
                            break;
                        case 8:
                            memcpy(&pSavDataTable->sAcQEnergy.sQ4Energy, &nBuff[0], sizeof(FORMAT11) * (1+MAX_FEE_NUM));
                            break;
                        default:
                            break;
                    }
                    
                    //计算组合有功
                    for (i = 0; i < (MAX_FEE_NUM + 1); i++)
                    {
                        nTmp = BCD2INT32U((INT8U*)&pSavDataTable->sAcEnergy.sPPEnergy[i], 8);
                        nTmp += BCD2INT32U((INT8U*)&pSavDataTable->sAcEnergy.sNPEnergy[i], 8);
                        INT32U2BCD(nTmp, 8, (INT8U*)&pSavDataTable->sAcCombEnergy.sPPEnergy[i]);
                    }
                    
                    pSavDataTable->nENSumChk = 0x55aa66bb + CheckINT32USum((INT32U*)&pSavDataTable->sAcEnergy, 0, (sizeof(MT_DGN_ENERGY_DATA)
                     + sizeof(MT_DGN_Q_ENERGY_DATA) + sizeof(MT_DGN_TRI_ENERGY_DATA) + sizeof(MT_DGN_TRI_Q_EN_DATA) + sizeof(MT_DGN_C_ENERGY_DATA))/4);					
                    pSavDataTable->nEPSumChk = CheckINT32USum((INT32U*)&pSavDataTable->sEnergyData[0], 0, (8*sizeof(ENERGY_DATA) +
                                               2*MAX_PHASE_NUM*sizeof(PH_ENERGY_DATA) + sizeof(COMB_ENERGY_DATA))/4);
                    WriteAcENData();
					OSSemPost(g_pSem[SEM_ACENWR_CTRL_IDX]);
                    //增加立即同步数据FRam（第1备份）至Flash（第2备份）
                    ReadAcENData();
                }
                break;
                
            case 0x04000101:
				if (RET_ERR_OK == GetSysClk(&sTime))
				{
					sTime.nWeek = pData[nIndex++];
					sTime.nDay = pData[nIndex++];
					sTime.nMon = pData[nIndex++];
					sTime.nYear = pData[nIndex++];
					if (RET_ERR_OK != SyncSysClk(&sTime))
					{
						nIsExist = 0x01;
					}
				}
				else
				{
					nIsExist = 0x01;
				}
				break;
            case 0x04000102:
				if (RET_ERR_OK == GetSysClk(&sTime))
				{
					sTime.nSec = pData[nIndex++];
					sTime.nMin = pData[nIndex++];
					sTime.nHour = pData[nIndex++];
					if (RET_ERR_OK != SyncSysClk(&sTime))
					{
						nIsExist = 0x01;
					}
				}
				else
				{
					nIsExist = 0x01;
				}
				break;
            case 0x04000103:
				sAc_Param.nDemandPeriod = BCD_TO_INT8U(pData[nIndex++]);
				break;
			case 0x04000104:
				sAc_Param.nSlipTime = BCD_TO_INT8U(pData[nIndex++]);
				break;
            case 0x04000106:
				memcpy(&sAc_Param.sYearChangeTime, &pData[nIndex], sizeof(FORMAT15));
				break;
			case 0x04000107:
				memcpy(&sAc_Param.sDayChangeTime, &pData[nIndex], sizeof(FORMAT15));
				break;
            case 0x040001FF:
				if (RET_ERR_OK == GetSysClk(&sTime))
				{
					sTime.nWeek = pData[nIndex++];
					sTime.nDay = pData[nIndex++];
					sTime.nMon = pData[nIndex++];
					sTime.nYear = pData[nIndex++];
					sTime.nSec = pData[nIndex++];
					sTime.nMin = pData[nIndex++];
					sTime.nHour = pData[nIndex++];
					if (RET_ERR_OK != SyncSysClk(&sTime))
					{
						nIsExist = 0x01;
					}
					else
					{
						sAc_Param.nDemandPeriod = BCD_TO_INT8U(pData[nIndex++]);
						sAc_Param.nSlipTime = BCD_TO_INT8U(pData[nIndex++]);
						nIndex += 2;
						memcpy(&sAc_Param.sYearChangeTime, &pData[nIndex], sizeof(FORMAT15));
						nIndex += sizeof(FORMAT15);
						memcpy(&sAc_Param.sDayChangeTime, &pData[nIndex], sizeof(FORMAT15));
						nIndex += sizeof(FORMAT15);
					}
				}
				else
				{
					nIsExist = 0x01;
				}
				break;
            case 0x04000201:
				sAc_Param.nYearSegmNum = BCD_TO_INT8U(pData[nIndex]);
				if (sAc_Param.nYearSegmNum > MAX_YEAR_SEGM_NUM)
				{
					sAc_Param.nYearSegmNum = MAX_YEAR_SEGM_NUM;
				}
				break;
			case 0x04000202:
				sAc_Param.nDayTableNum = BCD_TO_INT8U(pData[nIndex]);
				if (sAc_Param.nDayTableNum > MAX_DAY_TABLE_NUM)
				{
					sAc_Param.nDayTableNum = MAX_DAY_TABLE_NUM;
				}
				break;
			case 0x04000203:
				sAc_Param.nDaySegmNum = BCD_TO_INT8U(pData[nIndex]);
				if (sAc_Param.nDaySegmNum > MAX_DAY_SEGM_NUM)
				{
					sAc_Param.nDaySegmNum = MAX_DAY_SEGM_NUM;
				}
				break;
			case 0x04000204:
				sAc_Param.nFeeNumber = BCD_TO_INT8U(pData[nIndex]);
				if (sAc_Param.nFeeNumber > 63)
				{
					sAc_Param.nFeeNumber = 63;
				}
				break;
			case 0x04000205:
				nBuff[0] = BCD_TO_INT8U(pData[nIndex++]);
				nBuff[1] = BCD_TO_INT8U(pData[nIndex++]);
				sAc_Param.nHolidayNumber = (INT16U)(nBuff[1]*100+nBuff[0]);
				if (sAc_Param.nHolidayNumber > 254)
				{
					sAc_Param.nHolidayNumber = 254;
				}
				break;
			case 0x04000206:
				sAc_Param.nHarmcNumber = BCD_TO_INT8U(pData[nIndex]);
				break;
			case 0x040002FF:
				sAc_Param.nYearSegmNum = BCD_TO_INT8U(pData[nIndex++]);
				sAc_Param.nDayTableNum = BCD_TO_INT8U(pData[nIndex++]);
				sAc_Param.nDaySegmNum = BCD_TO_INT8U(pData[nIndex++]);
				sAc_Param.nFeeNumber = BCD_TO_INT8U(pData[nIndex++]);
				nBuff[0] = BCD_TO_INT8U(pData[nIndex++]);
				nBuff[1] = BCD_TO_INT8U(pData[nIndex++]);
				sAc_Param.nHolidayNumber = (INT16U)(nBuff[1]*100+nBuff[0]);
				sAc_Param.nYearSegmNum = BCD_TO_INT8U(pData[nIndex]);
				if (sAc_Param.nYearSegmNum > MAX_YEAR_SEGM_NUM)
				{
					sAc_Param.nYearSegmNum = MAX_YEAR_SEGM_NUM;
				}
				if (sAc_Param.nDayTableNum > MAX_DAY_TABLE_NUM)
				{
					sAc_Param.nDayTableNum = MAX_DAY_TABLE_NUM;
				}
				if (sAc_Param.nDaySegmNum > MAX_DAY_SEGM_NUM)
				{
					sAc_Param.nDaySegmNum = MAX_DAY_SEGM_NUM;
				}
				if (sAc_Param.nFeeNumber > 63)
				{
					sAc_Param.nFeeNumber = 63;
				}
				if (sAc_Param.nHolidayNumber > 254)
				{
					sAc_Param.nHolidayNumber = 254;
				}
				sAc_Param.nHarmcNumber = BCD_TO_INT8U(pData[nIndex]);
				break;

			case 0x04000301:
				sAc_Param.nDispNum1 = pData[nIndex++];
				break;
			case 0x04000302:
				sAc_Param.nDispTime1 = pData[nIndex++];
				break;
			case 0x04000303:
				sAc_Param.nDispPoint1 = pData[nIndex++];
				break;
			case 0x04000304:
				sAc_Param.nDispPoint2 = pData[nIndex++];
				break;
			case 0x04000305:
				sAc_Param.nDispNum2 = pData[nIndex++];
				break;
			case 0x040003FF:
				sAc_Param.nDispNum1 = pData[nIndex++];
				sAc_Param.nDispTime1 = pData[nIndex++];
				sAc_Param.nDispPoint1 = pData[nIndex++];
				sAc_Param.nDispPoint2 = pData[nIndex++];
				sAc_Param.nDispNum2 = pData[nIndex++];
				break;

			case 0x04000401:/*表号*/
				memcpy(&sAc_Param.nMeterAddr[0], &pData[nIndex], LEN_METER_ADDR);
				nChanger = 0x55;
				break;
			case 0x04000402:/*用户号*/
				memcpy(&sAc_Param.nMeterNumber[0], &pData[nIndex], LEN_METER_ADDR);
				break;
			case 0x04000403:/*设备码*/
				//memcpy(&sAc_Param.nMeterProNum[0], &pData[nIndex], LEN_METER_GLBH);
				break;
			case 0x040004FF:
				memcpy(&sAc_Param.nMeterAddr[0], &pData[nIndex], LEN_METER_ADDR);
				nIndex += LEN_METER_ADDR;
				memcpy(&sAc_Param.nMeterNumber[0], &pData[nIndex], LEN_METER_ADDR);
				nIndex += LEN_METER_ADDR;
				//memcpy(&sAc_Param.nMeterProNum[0], &pData[nIndex], LEN_METER_GLBH);
				nIndex += LEN_METER_GLBH;
				nChanger = 0x55;
				break;

			case 0x04000601:
				sAc_Param.nEpCompFlag = pData[nIndex++];
				break;
			case 0x04000602:
				sAc_Param.nEq1CompFlag = pData[nIndex++];
				break;
			case 0x04000603:
				sAc_Param.nEq2CompFlag = pData[nIndex++];
				break;
			case 0x040006FF:
				sAc_Param.nEpCompFlag = pData[nIndex++];
				sAc_Param.nEq1CompFlag = pData[nIndex++];
				sAc_Param.nEq2CompFlag = pData[nIndex++];
				break;

			case 0x04000701:
				sAc_Param.nIR1Baud = pData[nIndex++];
				break;
			case 0x04000702:
				sAc_Param.nIR2Baud = pData[nIndex++];
				break;
			case 0x04000703:
				sAc_Param.nCom1Baud = pData[nIndex++];
				break;
			case 0x04000704:
				sAc_Param.nCom2Baud = pData[nIndex++];
				break;
			case 0x04000705:
				sAc_Param.nCom3Baud = pData[nIndex++];
				break;
			case 0x040007FF:
				sAc_Param.nIR1Baud = pData[nIndex++];
				sAc_Param.nIR2Baud = pData[nIndex++];
				sAc_Param.nCom1Baud = pData[nIndex++];
				sAc_Param.nCom2Baud = pData[nIndex++];
				sAc_Param.nCom3Baud = pData[nIndex++];
				break;

			case 0x04000801:
				sAc_Param.nHolidayFlag = pData[nIndex++];
				break;
			case 0x04000802:
				sAc_Param.nHolidayTable = pData[nIndex++];
				break;
			case 0x040008FF:
				sAc_Param.nHolidayFlag = pData[nIndex++];
				sAc_Param.nHolidayTable = pData[nIndex++];
				break;

			case 0x04000B01:
				memcpy(&sAc_Param.sMonthJS1, &pData[nIndex], sizeof(FORMAT24));
				break;
			case 0x04000B02:
				memcpy(&sAc_Param.sMonthJS2, &pData[nIndex], sizeof(FORMAT24));
				break;
			case 0x04000B03:
				memcpy(&sAc_Param.sMonthJS3, &pData[nIndex], sizeof(FORMAT24));
				break;
			case 0x04000BFF:
				memcpy(&sAc_Param.sMonthJS1, &pData[nIndex], sizeof(FORMAT24));
				nIndex += sizeof(FORMAT24);
				memcpy(&sAc_Param.sMonthJS2, &pData[nIndex], sizeof(FORMAT24));
				nIndex += sizeof(FORMAT24);
				memcpy(&sAc_Param.sMonthJS3, &pData[nIndex], sizeof(FORMAT24));
				nIndex += sizeof(FORMAT24);
				break;

			case 0x04000C01:case 0x04000C02:case 0x04000C03:case 0x04000C04:case 0x04000C05:
			case 0x04000C06:case 0x04000C07:case 0x04000C08:case 0x04000C09:case 0x04000C0A:
				nPwd = pData[nIndex++];
				if ((nPwdRight < nPwd)&&(nPwd == pData[0])&&(nPwd <= 9))
				{
					memcpy(&sAc_Param.nMeterPWD[nPwd], &pData[nIndex], LEN_METER_PASSWORD);
				}
				else
				{
					nIsExist = 0x04;
				}
				break;

			case 0x04000E01:
				memcpy(&sAc_Param.sPosPowLimt, &pData[nIndex], sizeof(FORMAT23));
				break;
			case 0x04000E02:
				memcpy(&sAc_Param.sNegPowLimt, &pData[nIndex], sizeof(FORMAT23));
				break;
			case 0x04000E03:
				memcpy(&sAc_Param.sUpLimtVolt, &pData[nIndex], sizeof(FORMAT07));
				break;
			case 0x04000E04:
				memcpy(&sAc_Param.sLowLimtVolt, &pData[nIndex], sizeof(FORMAT07));
				break;
			case 0x04000Eff:
				memcpy(&sAc_Param.sPosPowLimt, &pData[nIndex], sizeof(FORMAT23));
				nIndex += sizeof(FORMAT23);
				memcpy(&sAc_Param.sNegPowLimt, &pData[nIndex], sizeof(FORMAT23));
				nIndex += sizeof(FORMAT23);
				memcpy(&sAc_Param.sUpLimtVolt, &pData[nIndex], sizeof(FORMAT07));
				nIndex += sizeof(FORMAT07);
				memcpy(&sAc_Param.sLowLimtVolt, &pData[nIndex], sizeof(FORMAT07));
				nIndex += sizeof(FORMAT07);
				break;

			case 0x04010000:
			case 0x04020000:
				nNum = (nLen-8)/sizeof(MM_DD_NN);
				if (nNum > MAX_YEAR_SEGM_NUM)
				{
					nNum = MAX_YEAR_SEGM_NUM;
				}
				memcpy(&sAc_Param.sYearSegmTable[pData[2]-1][0], &pData[nIndex], nNum*sizeof(MM_DD_NN));
				break;

			case 0x04010001:case 0x04010002:case 0x04010003:case 0x04010004:
			case 0x04010005:case 0x04010006:case 0x04010007:case 0x04010008:
			case 0x04020001:case 0x04020002:case 0x04020003:case 0x04020004:
			case 0x04020005:case 0x04020006:case 0x04020007:case 0x04020008:
				nNum = (nLen-8)/sizeof(hh_mm_NN);
				if (nNum > MAX_DAY_SEGM_NUM)
				{
					nNum = MAX_DAY_SEGM_NUM;
				}
				memcpy(&sAc_Param.sDaySegmTable[(INT8U)((nDI&0x00ff0000)>>16)-1][(INT8U)(nDI&0x000000ff)-1][0], &pData[nIndex], nNum*sizeof(hh_mm_NN));
				break;
         //故障模拟******************************************************
          
          case  0xB1000001:   //485
                g_MeterComFaul = pData[nIndex];
                break;
          case  0xB1000002:   //红外
                g_MeterComFal[1]=pData[nIndex];   
                break;
          case   0xB1000108: //开盖记录
               if(pData[nIndex])
                 g_SimMeterCoverFlag=0xBF;           
               else
                 g_SimMeterCoverFlag=0xFF;
               nIndex++;
               memcpy(&g_SimMeterCoverTimeBuf[0], &pData[nIndex], 6);
                break;
         case  0xB1000109:   //电压越限
                g_MeterVoltPlusFlg = pData[nIndex];
                break;
         case  0xB100010A:   //电能停走
                if (1 == pData[nIndex])
                {
                    memset(g_MeterAcErrFlg, 0, 4);
                }
                g_MeterAcErrFlg[1] = pData[nIndex];
                break;
          case  0xB1000003:   //失压 
                for(i=0;i<3;i++)
                { 
                   g_MeterEventWrong[i]&=0xFE;
                   g_MeterEventWrong[i]+=pData[nIndex+i];
                }
                break;
          case  0xB1000004:  //断相
                for(i=0;i<3;i++)
                {
                   g_MeterEventWrong[i]&=0xFD;
                   g_MeterEventWrong[i]+=(pData[nIndex+i]<<1);
                }
                break;
          case  0xB1000005: //失流
                
                for(i=0;i<3;i++)
                {  
                   g_MeterEventWrong[i]&=0xFB;
                   g_MeterEventWrong[i]+=(pData[nIndex+i]<<2);
                }
                break;
         case   0xB1000006: //逆相序
                g_MeterEventWrong[3]=pData[nIndex];
                break;
         case   0xB1000007:
                g_MeterAcErrFlg[0] =pData[nIndex];
                break;
         case   0xB1000008: case 0xB1000009:case 0xB100000A:
         case   0xB100000B:case 0xB100000C: case 0xB100000D:case 0xB100000E:
                nNum=((nDI&0x0F)-7);
                INT8U nNum2=(1<<nNum);
                g_MeterInsideWrong[0]&=(0xFF-nNum2);
                g_MeterInsideWrong[0]+=(pData[nIndex]<<nNum);
                break;
         case   0xB100000F:
                g_MeterInsideWrong[1]&=0xFE;
                g_MeterInsideWrong[1]+=pData[nIndex]; 
                break;
         case   0xB1000101: //脉冲灯
                g_MeterInsideWrong[1]&=0xFD;
                g_MeterInsideWrong[1]+=(pData[nIndex]<<1); 
                //PulLamChange();
                AcCalcJustPluseConst(pData[nIndex]<<1);
                break;
         case   0xB1000102:
                g_MeterHardwareWrong[0]&=0xFE;
                g_MeterHardwareWrong[0]+=pData[nIndex];
                
                if (0x01 == (g_MeterHardwareWrong[0]&0x01))
                {
                    GpioClearOutput(IO_PORT5, IO_PINxA);//载波断电
                    g_MeterPowerOff = 1;
                }
                else
                {
                    g_MeterPowerOff = 0;
                    GpioSetOutput(IO_PORT5, IO_PINxA);
                    g_nFlgReBoot = 1;
                }
                //g_MeterPowerOff = pData[nIndex];
                break;
         case   0xB1000103:
                g_MeterHardwareWrong[0]&=0xF1;
                switch(pData[nIndex])
                {    
                     case 0: 
                       //g_nFlgReBoot = 1; 
                       //g_nFlg_Commu2 = 1;
                       break;
                     case 1: 
                       g_MeterHardwareWrong[0]+=(1<<1); break;
                     case 2: 
                       g_MeterHardwareWrong[0]+=(1<<2); break;
                     case 3: 
                       g_MeterHardwareWrong[0]+=(1<<3); break;
                     default:
                       g_MeterHardwareWrong[0]&=0xF1;   break;
                }
                break;
        case   0xB1000104:
                g_MeterHardwareWrong[0]&=0x8F;
                g_MeterHardwareWrong[0]+=(pData[nIndex]<<4);
                g_MeterHardwareWrong[0]+=(pData[nIndex+1]<<5);
                g_MeterHardwareWrong[0]+=(pData[nIndex+2]<<6);
                break;
        case   0xB1000105:
                g_MeterHardwareWrong[0]&=0x7F;
                g_MeterHardwareWrong[1]&=0xFC;
                g_MeterHardwareWrong[0]+=(pData[nIndex]<<7);
                g_MeterHardwareWrong[1]+=pData[nIndex+1];
                g_MeterHardwareWrong[1]+=(pData[nIndex+2]<<1);
                break;
        case   0xB1000106:
                g_MeterHardwareWrong[1]&=0xE3;
                g_MeterHardwareWrong[1]+=(pData[nIndex]<<2);
                g_MeterHardwareWrong[1]+=(pData[nIndex+1]<<3);
                g_MeterHardwareWrong[1]+=(pData[nIndex+2]<<4);
                break;
        case   0xB1000107:
                g_MeterHardwareWrong[1]&=0x1F;
                g_MeterHardwareWrong[1]+=(pData[nIndex]<<5);
                g_MeterHardwareWrong[1]+=(pData[nIndex+1]<<6);
                g_MeterHardwareWrong[1]+=(pData[nIndex+2]<<7);
                break;
         case   0xB1000301://无电流时模拟正常走字计量；有电压，无电流时，控制此操作
                g_nSetCtrlcurFlg = pData[nIndex];
                break;
         case   0xB100010B://飞走
                if (1 == pData[nIndex])
                {
                    memset(g_MeterAcErrFlg, 0, 4);
                }
                g_MeterAcErrFlg[0] = pData[nIndex];
                break;
         case   0xB100010C://倒走
                if (1 == pData[nIndex])
                {
                    memset(g_MeterAcErrFlg, 0, 4);
                }
                g_MeterAcErrFlg[2] = pData[nIndex];
                break;
         case   0xB1000302://误差

                if((pData[nIndex]&0x10) != 0x10)
                {
                    nZFWuCh = 0;
                    if((pData[nIndex]&0x0F) == 0x00)
                    {
                        nWuCh = 0x00;    
                    }
                    else if((pData[nIndex]&0x0F) == 0x01)
                    {
                        nWuCh = 0x01;
                    }
                    else if((pData[nIndex]&0x0F) == 0x02)
                    {
                        nWuCh = 0x02;
                    }
                    else if((pData[nIndex]&0x0F) == 0x03)
                    {
                        nWuCh = 0x03;
                    }
                    else if((pData[nIndex]&0x0F) == 0x04)
                    {
                        nWuCh = 0x04;
                    }
                    else if((pData[nIndex]&0x0F) == 0x05)
                    {
                        nWuCh = 0x05;
                    }
                    else if((pData[nIndex]&0x0F) == 0x06)
                    {
                        nWuCh = 0x06;
                    }
                    else if((pData[nIndex]&0x0F) == 0x07)
                    {
                        nWuCh = 0x07;
                    }
                    else if((pData[nIndex]&0x0F) == 0x08)
                    {
                        nWuCh = 0x08;
                    }
                    else if((pData[nIndex]&0x0F) == 0x09)
                    {
                        nWuCh = 0x09;
                    }
                    else if((pData[nIndex]&0x0F) == 0x0A)
                    {
                        nWuCh = 0x0A;
                    }
                }
                else
                {
                    nZFWuCh = 1;
                    if((pData[nIndex]&0x0F) == 0x01)
                    {
                        
                        nWuCh = 0x09;
                    }
                    else if((pData[nIndex]&0x0F) == 0x02)
                    {
                        nWuCh = 0x08;
                    }
                    else if((pData[nIndex]&0x0F) == 0x03)
                    {
                        nWuCh = 0x07;
                    }
                    else if((pData[nIndex]&0x0F) == 0x04)
                    {
                        nWuCh = 0x06;
                    }
                    else if((pData[nIndex]&0x0F) == 0x05)
                    {
                        nWuCh = 0x05;
                    }
                    else if((pData[nIndex]&0x0F) == 0x06)
                    {
                        nWuCh = 0x04;
                    }
                    else if((pData[nIndex]&0x0F) == 0x07)
                    {
                        nWuCh = 0x03;
                    }
                    else if((pData[nIndex]&0x0F) == 0x08)
                    {
                        nWuCh = 0x02;
                    }
                    else if((pData[nIndex]&0x0F) == 0x09)
                    {
                        nWuCh = 0x01;
                    }
                    else if((pData[nIndex]&0x0F) == 0x0A)
                    {
                        nWuCh = 0x00;
                    }
                }   
                break;
         case   0xB1000303://火线短接零线正常显示
                g_nSetShortcurFlg = pData[nIndex];
                break;
          case  0xB1000304://
                {
                    if ((g_ACOffset<80) || (g_ACOffset>120))
                    {
                        break;
                    }
                    g_ACOffset = pData[nIndex];
                    MemWriteAcXFlhBackData(F112, &g_ACOffset, sizeof(g_ACOffset));
                }
                break;
          case  0xB1000305://设置脉冲常数
                {
					AcCalcJustPluseConst(pData[nIndex]);
	                break;
                }
         default:
				if (0x04030000 == (nDI&0xffffff00))
				{
					memcpy(&sAc_Param.sHolidayTable[(INT8U)(nDI&0x000000ff)-1], &pData[nIndex], sizeof(YY_MM_DD_NN));
				}
				else
				{
					nIsExist = 0x02;
				}
				break;
                                            
                case 0xA5000101:
                    g_nAutoTestFlg1 = 1;  
                    break;
                case 0xA6000101:
                    memset(&sJbParam, 0, sizeof(MEM_AC_CAL_PRM));
                    if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType))
                    {
                        AcCalcJustData1(); 
                    }
                    else if(SINGLEPHASE_METER == g_nHardType)
                    {
                        AcCalcJustData2(); 
                    }
                    else if(TRIPHASE_METER_220ZT == g_nHardType)
                    {
                        AcCalcJustData3(); 
                    }
                    else if (TRIPHASE_METER_57U == g_nHardType)
                    {
                        AcCalcJustData4();
                    }
                    else if(TRIPHASE_METER_100U == g_nHardType)
                    {
                        AcCalcJustData5(); 
                    }
                    else if(TRIPHASE_METER_100 == g_nHardType)
                    {
                        AcCalcJustData6(); 
                    }
                    break;
                case 0xA6000102:
                    AcCalcEMUCfgConst();//特殊命令，为解决逆相序是脉冲灯问题不改校表参数
                    break;
                case 0xA6000103:
                    if(1 == pData[nIndex])
                    {
                        DISPLY_SWITCH = 1;
                        sAc_Param.nDispModel = 1;
                    }
                    else
                    {
                        DISPLY_SWITCH = 0;
                        sAc_Param.nDispModel = 0;
                    }
                    MemWriteAcPrm(F51, &sAc_Param.nDispModel, 1);
                    break;
        }
    }
    else
	{
		nIsExist = 0x01;
	}
    
    if (nNotPrmSetFlg)
    {
        return 0;
    }
    
    if (0 == nIsExist)
	{
		nSuccflag = RET_ERR_ERROR;
		if (MemWriteAcPrm(FEND, &sAc_Param, sizeof(sAc_Param)) >= 0)
        {
            nSuccflag = RET_ERR_OK;
        }
		if (RET_ERR_OK != nSuccflag)
		{
			nIsExist = 0x01;
		}
		if (0x55 == nChanger)
		{
			//加地址变化标志
            //MemWriteXTempData(MEM_OBJ_X_TEMP_TERM, 0, MEM_TEMP_AC_UPDATE_ID, &nState, 1);
		}
	}

	return nIsExist;
}


/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 设置电表参数
*********************************************************************************/
INT8U Dlt07Code14(const INT8U *pValidBuf, INT32U nValidLen, INT8U *pSendBuf)
{
    INT8U   i;
	INT8U	nTmp = 0;
	INT8S   nRetFlag = 0;
	INT8U   nData[256];
	INT16U  nDataLen = 0;
	INT8U   nCtrlCode = 0;
	INT8U   nDI01[4];
	INT8U   nIndex = 0;
	INT8U   nPwd[LEN_METER_PASSWORD];
	INT8U	nPassword[10][LEN_METER_PASSWORD];
	INT8U   nRight = 0xFF;

	memset(nData, 0, sizeof(nData));
	memset(nDI01, 0, sizeof(nDI01));
	memset(nPwd, 0, LEN_METER_PASSWORD);
	memset(nPassword, 0, 9*LEN_METER_PASSWORD);
    
    nCtrlCode = pValidBuf[8];
	nDataLen  = pValidBuf[9];
	if ((nDataLen < 2) || (nDataLen > (nValidLen-12)))
	{
		nTmp = 0x01;
	}
    else
    {
        for(i=0; i<nDataLen; i++)
		{
			nData[i] = pValidBuf[10 + i] - 0x33;
		}
		nDI01[0] = nData[nIndex++];
		nDI01[1] = nData[nIndex++];
		nDI01[2] = nData[nIndex++];
		nDI01[3] = nData[nIndex++];
		
		nRight = nData[nIndex++];
		if (nRight > 5)									/*4级密码均可设置*/
		{
			nTmp = 0x04;
		}
        else
        {
            memcpy(nPwd, nData + nIndex, 3);				/*密码判断*/
			nIndex += 3;
            if(MemReadAcPrm(F35, &nPassword, sizeof(nPassword)) >= 0)
            {
                nRetFlag = RET_ERR_OK;
            }
			if (RET_ERR_OK == nRetFlag)
            {
                if (0 == nRight)
				{
					if (0 != (memcmp(nPwd, g_nSuperPwd, 3))&&(0 != memcmp(nPwd, nPassword[0], 3)))
					{
						nTmp = 0x04;
					}
				}
				else
				{
					if (0 != memcmp(nPwd, nPassword[nRight], 3))
					{
						nTmp = 0x04;
					}
				}
            }
            else
			{
				if (0 == nRight)
				{
					if (0 != memcmp(nPwd, g_nSuperPwd, 3))
					{
						nTmp = 0x04;
					}
				}
			}
            if((0x01 == nDI01[0]) && (0x01 == nDI01[1]) && (0x00 == nDI01[2]) && (0xAA == nDI01[3]))
            {
                nTmp = 0;
            }
            nTmp = 0;//不判密码
            
            if (nTmp == 0)
			{
				nRetFlag = Dlt07ParamSupported(nDI01, nDataLen - 12);	/*减掉数据标示,密码和操作代码共12字节*/
             // nRetFlag = CheckFauCode(nDI01, nData, nRetFlag); //检查故障模拟报文
				if (RET_ERR_OK == nRetFlag)						/*计量支持的参数*/
				{
				 	AcCalcMeterPrmChanEve(&nData[0], PROT_GB645_07);
				 	nTmp = Dlt07SaveParam(nData, nDataLen);
				}
				else
				{
					nTmp = 0x02;
				}
			}
        }
    }
    if (nTmp != 0)
	{
		pSendBuf[0] = nCtrlCode | 0xC0;
		pSendBuf[1] = 1;
		pSendBuf[2] = nTmp;
	}
	else
	{
		pSendBuf[0] = nCtrlCode | 0x80;
		pSendBuf[1] = 0;
	}
	return (pSendBuf[1]+2);
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 广播设置电表通信地址
*********************************************************************************/
INT8U DltCodeAddress(const INT8U* pValidBuf, INT8U nValidLen, INT8U* pSendBuf, INT8U nType)
{
	INT8U	i;
	INT8U	nTmp = 0;
    INT8U   nData[20]  = {0};
	INT8U	nCtrlCode=0;
	INT16U	nDataLen=0;
	INT8U	nSendlen = 0;
	INT8U	nAddr[LEN_METER_ADDR];
	INT8U	nBroadaddr1[LEN_METER_ADDR];
	INT8U	nBroadaddr2[LEN_METER_ADDR];
	INT8U	nIndex = 0;
	INT8S	nSuccflag = RET_ERR_ERROR;
    INT8U	nState = 0x55;

    memset(&nData[0], 0, sizeof(nData));
	memset(nAddr,0,LEN_METER_ADDR);
	memset(nBroadaddr1,0xAA,LEN_METER_ADDR);
	memset(nBroadaddr2,0x99,LEN_METER_ADDR);								/*广播地址*/

	memcpy(nAddr,pValidBuf+1,6);
	nCtrlCode=pValidBuf[8];
	nDataLen=pValidBuf[9];
    
    /*不判断广播地址*/
	/*if(((nType == GB_07_PROT)&&(memcmp(nAddr,nBroadaddr1,LEN_METER_ADDR) != 0))
		||((nType == GB_97_PROT)&&(memcmp(nAddr,nBroadaddr2,LEN_METER_ADDR) != 0))
			|| nDataLen != 6)			//判断地址是否广播和长度是否正确
	{
		nTmp = 0x01;
	}*/
	//else
	{
		for(i=0; i<nDataLen; i++)
		{
			nData[i] = pValidBuf[10 + i] - 0x33;
		}
		if(!IsAllBcd(&nData[nIndex], (INT16U)(nDataLen-nIndex)))		/*BCD码有效性判断*/
		{
			nTmp = 0x01;
		}
		else
		{
			//nSuccflag = MemReadACPrm(MEM_OBJ_AC_PRM, 0, AC_COM_ADDR_ID, &nAddr, LEN_METER_ADDR);
            if (MemReadAcPrm(F19, &nAddr[0], LEN_METER_ADDR) > 0)
            {
                nSuccflag = RET_ERR_OK;
            }
			if (RET_ERR_OK == nSuccflag)
			{
				nSuccflag = RET_ERR_ERROR;
				memcpy(nAddr,nData,6);
				nSuccflag = MemWriteAcPrm(F19, &nAddr[0], LEN_METER_ADDR);
                
                //同时将表号写入
                MemWriteAcPrm(F20, &nAddr[0], LEN_METER_ADDR);
                memcpy(&g_nMemAddr[0], &nAddr[0], LEN_METER_ADDR);
			}
			if(nSuccflag != RET_ERR_OK)
			{
				nTmp = 0x01;
			}
			/*else
			{
				//加地址变化标志
                MemWriteXTempData(MEM_OBJ_X_TEMP_TERM, 0, MEM_TEMP_AC_UPDATE_ID, &nState, 1);
                if(nType == PROT_GB645_07)
				{
					memcpy(&pSendBuf[2],nAddr,LEN_METER_ADDR);
					nSendlen = LEN_METER_ADDR;
				}
			}*/
		}
	}
	if(nTmp != 0)
	{
		pSendBuf[0] = nCtrlCode | 0xC0;
		pSendBuf[1] = 1;
		pSendBuf[2] = nTmp;
	}
	else
	{
		pSendBuf[0] = nCtrlCode | 0x80;
		pSendBuf[1] = nSendlen;
	}
	return (pSendBuf[1]+2);
}
/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 修改电表密码
*********************************************************************************/
INT8U DltCodePassword(const INT8U* pValidBuf, INT8U nValidLen, INT8U* pSendBuf, INT8U nType)
{
	INT8U   i;
	INT8U   nData[256];
	INT16U  nDataLen=0;
	INT8U	nPassword[10][LEN_METER_PASSWORD];
        INT8U	nMeterPWD[10][LEN_METER_PASSWORD]; 
	INT8U   nOldRight=0xFF;
	INT8U   nNewRight=0xFF;
	INT8U   nOldPwd[LEN_METER_PASSWORD];
	INT8U   nNewPwd[LEN_METER_PASSWORD];
	INT8U   nCtrlCode=0;
	INT8S   nSuccflag = RET_ERR_ERROR;
	INT8U	nIsExist = 0;
	INT8U   nIndex=0;
	INT32U	nDI = 0;

	memset(nData,0,256);
	memset(nPassword,0,9*LEN_METER_PASSWORD);
	memset(nOldPwd,0,LEN_METER_PASSWORD);
	memset(nNewPwd,0,LEN_METER_PASSWORD);
    
	nCtrlCode=pValidBuf[8];
	nDataLen=pValidBuf[9];
	if((nType == PROT_GB645_07)&&(nDataLen!=12))
	{
		nIsExist = 0x01;
	}
	else
	{
            for(i=0; i<nDataLen; i++)
            {
                nData[i] = pValidBuf[10 + i] - 0x33;
            }
            if(nType == PROT_GB645_07)
            {
                    nDI = ((nData[3]<<24)|(nData[2]<<16)|(nData[1]<<8)|nData[0]);
                    nIndex += 4;
            }
            nOldRight = nData[nIndex++];
            memcpy(nOldPwd, nData+nIndex, 3);
            nIndex += 3;
            nNewRight = nData[nIndex++];
            memcpy(nNewPwd, nData+nIndex, 3);
            nIndex += 3;
            if((nType == PROT_GB645_07)&&((nDI&0xffffff00) != 0x04000c00))
            {
                    nIsExist = 0x02;
            }
            else
            {
                if((nOldRight > nNewRight)&&(nNewRight <= 9))
                {
                        nIsExist = 0x04;
                }
                else
                {
                  if((nType == PROT_GB645_07)&&((nData[0]-1) != nNewRight))
                  {
                          nIsExist = 0x02;
                  }
                  else
                  {

                    if (MemReadAcPrm(F35, &nMeterPWD, sizeof(nMeterPWD)) >=0 )
                    {
                      nSuccflag = RET_ERR_OK;
                    } 
                    if(nSuccflag == RET_ERR_OK)
                    {
                        if(nOldRight == 0)
                        {
                            if((memcmp(nOldPwd, nMeterPWD[0], 3) != 0)&&(memcmp(nOldPwd, g_nSuperPwd, 3) != 0))
                            {
                                    nIsExist = 0x04;
                            }
                        }
                        else
                        {
                            if(memcmp(nOldPwd, nMeterPWD[nOldRight], 3) != 0)
                            {
                                    nIsExist = 0x04;
                            }
                        }
                    }
                    else
                    {
                            nIsExist = 0x01;
                    }
                }
              }
            }
	}
	if(nIsExist == 0)
	{
            memcpy(&nMeterPWD[nNewRight], nNewPwd, LEN_METER_PASSWORD);
            nSuccflag = MemWriteAcPrm(F35, &nMeterPWD, sizeof(nMeterPWD));
            if(nSuccflag == RET_ERR_OK)
            {
                    pSendBuf[0] = nCtrlCode | 0x80;
                    pSendBuf[1] = 0x04;
                    pSendBuf[2] = nNewRight;
                    memcpy(&pSendBuf[3], nNewPwd, LEN_METER_PASSWORD);
                    g_SimMeterPasswordFlag = 1;//密钥更新事件
            }
            else
            {
              if(nType == PROT_GB645_07)
              {
                      pSendBuf[0] = nCtrlCode | 0xC0;
                      pSendBuf[1] = 0x01;
                      pSendBuf[2] = 0x01;
              }
              else
              {
                      return 0;
              }
            }
	}
	else
	{
          if(nType == PROT_GB645_07)
          {
                  pSendBuf[0] = nCtrlCode | 0xC0;
                  pSendBuf[1] = 0x01;
                  pSendBuf[2] = nIsExist;
                  //g_SimMeterPasswordFlag = 1;//密钥更新事件
          }
          else
          {
                  return 0;
          }
	}
	return (pSendBuf[1]+2);
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 电表需量清零
*********************************************************************************/
INT8U DltClrDemand(const INT8U* pValidBuf, INT8U nValidLen, INT8U* pSendBuf, INT8U nType)
{
    INT8U   i;
	INT8U   nData[256];
	INT16U  nDataLen = 0;
	INT8U   nCtrlCode = 0;
	INT8S   nRetFlag = RET_ERR_ERROR;
	INT8U	nTmp = 0;
	INT8U   nIndex = 0;
	INT8U   nRight = 0xFF;
	INT8U   nPwd[LEN_METER_PASSWORD];
	INT8U	nPassword[10][LEN_METER_PASSWORD];
	AC_FRE_STAT		sAcFreeze;
    
    memset(nData,0,256);
	memset(nPassword,0,9*LEN_METER_PASSWORD);
    
    nCtrlCode=pValidBuf[8];
	nDataLen=pValidBuf[9];
    
    if(((nType == PROT_GB645_97)&&(nDataLen!=4))||((nType == PROT_GB645_07)&&(nDataLen!=8)))
	{
		nTmp = 0x01;
	}
    else
    {
        for(i=0; i<nDataLen; i++)
		{
			nData[i] = pValidBuf[10 + i] - 0x33;
		}
		nRight = nData[nIndex++];
		if(nRight > 5)									/*5级密码均可设置*/
		{
			nTmp = 0x04;
		}
        else
        {
            memcpy(nPwd, nData+nIndex, 3);				/*密码判断*/
			nIndex += 3;
            
            if (MemReadAcPrm(F35, &nPassword, sizeof(nPassword)) >=0 )
            {
                nRetFlag = RET_ERR_OK;
            }
            if(nRetFlag == RET_ERR_OK)
            {
                if(nRight == 0)
				{
					if((memcmp(nPwd, g_nSuperPwd, 3) != 0)&&(memcmp(nPwd, nPassword[0], 3) != 0))
					{
						nTmp = 0x04;
					}
				}
                else
				{
					if(memcmp(nPwd, nPassword[nRight], 3) != 0)
					{
						nTmp = 0x04;
					}
				}
            }
            else
			{
				if(nRight == 0)
				{
					if(memcmp(nPwd, g_nSuperPwd, 3) != 0)
					{
						nTmp = 0x04;
					}
				}
			}
            if(nTmp == 0)
            {
                memset(&sAcFreeze, 0, sizeof(AC_FRE_STAT));
				sAcFreeze.nFlag = 0x55;
				memcpy(&sAcFreeze.nMasterNo[0], &nData[4], 4);
                MemWriteRamXmpData(F10, 0, &sAcFreeze, sizeof(AC_FRE_STAT));
            }
        }
    }
    if(nTmp != 0)
	{
		pSendBuf[0] = nCtrlCode | 0xC0;
		pSendBuf[1] = 1;
		pSendBuf[2] = nTmp;
	}
	else
	{
		pSendBuf[0] = nCtrlCode | 0x80;
		pSendBuf[1] = 0;
	}
	return (pSendBuf[1]+2);
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 电表清零
*********************************************************************************/
INT8U DltClrMeter(const INT8U* pValidBuf, INT8U nValidLen, INT8U* pSendBuf, INT8U nType)
{
    INT8U   i,j;
	INT8U   nData[256];
	INT16U  nDataLen=0;
	INT8U   nCtrlCode=0;
	INT8S   nRetFlag = RET_ERR_ERROR;
	INT8U	nTmp = 0;
	INT8U   nIndex=0;
	INT8U   nRight = 0xFF;
	INT8U   nPwd[LEN_METER_PASSWORD];
	INT8U	nPassword[10][LEN_METER_PASSWORD];
	AC_FRE_STAT		sAcInit;
    
    MEM_AC_DAY_DATA sAcDayData;
    memset(nData,0,256);
	memset(nPassword,0,9*LEN_METER_PASSWORD);

	nCtrlCode=pValidBuf[8];
	nDataLen=pValidBuf[9];
	if(((PROT_GB645_97 == nType) && (nDataLen != 4))||((PROT_GB645_07 == nType) && (nDataLen != 8)))
	{
		nTmp = 0x01;
	}
    else
    {
        for(i=0; i<nDataLen; i++)
		{
			nData[i] = pValidBuf[10 + i] - 0x33;
		}
		nRight = nData[nIndex++];
		if(nRight > 3)									/*1级密码均可设置*/
		{
			nTmp = 0x04;
		}
        else
        {
            memcpy(nPwd, nData+nIndex, 3);				/*密码判断*/
			nIndex += 3;
            if (MemReadAcPrm(F35, &nPassword, sizeof(nPassword)) >=0 )
            {
                nRetFlag = RET_ERR_OK;
            }
            if(RET_ERR_OK == nRetFlag)
            {
                if(0 == nRight)
				{
					if((memcmp(nPwd, g_nSuperPwd, 3) != 0)&&(memcmp(nPwd, nPassword[0], 3) != 0))
					{
						nTmp = 0x04;
					}
				}
				else
				{
					if(memcmp(nPwd, nPassword[nRight], 3) != 0)
					{
						nTmp = 0x04;
					}
				}
            }
            else
			{
				if(0 == nRight)
				{
					if(memcmp(nPwd, g_nSuperPwd, 3) != 0)
					{
						nTmp = 0x04;
					}
				}
			}
            
            if(0 == nTmp)
			{
				memset(&sAcInit, 0, sizeof(AC_FRE_STAT));
				sAcInit.nFlag = 0x55;
				memcpy(&sAcInit.nMasterNo[0], &nData[4], 4);
				MemWriteRamXmpData(F11, 0, &sAcInit, sizeof(AC_FRE_STAT));
                
               
			}
        }
    }
    if(nTmp != 0)
	{
		pSendBuf[0] = nCtrlCode | 0xC0;
		pSendBuf[1] = 1;
		pSendBuf[2] = nTmp;
	}
	else
	{
		pSendBuf[0] = nCtrlCode | 0x80;
		pSendBuf[1] = 0;
	}
	return (pSendBuf[1]+2);
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 事件清零
*********************************************************************************/
INT8U Dlt07Code1b(const INT8U *pValidBuf, INT8U nValidLen, INT8U *pSendBuf)
{
    INT8S	nRetFlag = RET_ERR_ERROR;
	INT8U	nIsExist = 0;
	INT8U	i = 0;
	INT16U	nDataLen = 0;
	INT8U	nCtrlCode = 0;
	INT8U	nData[256];
	INT8U	nAddr[LEN_METER_ADDR];
	INT8U	nBroad[6];
	INT8U   nPwd[LEN_METER_PASSWORD];
	INT8U	nPassword[9][LEN_METER_PASSWORD];
	INT8U   nRight = 0xFF;
	AC_EVECLR_STAT	sAcEvent[3];
    
    memset(nData,0,256);
	memset(nAddr,0,LEN_METER_ADDR);
	memset(nBroad,0x99,6);							/*广播地址*/
	
	memcpy(nAddr,pValidBuf+1,6);
	nCtrlCode=pValidBuf[8];
	nDataLen=pValidBuf[9];
    if (nDataLen != 12)
	{
		nIsExist = 0x01;
	}
    else
    {
        for(i=0; i<nDataLen; i++)
		{
			nData[i] = pValidBuf[10 + i] - 0x33;
		}
		nRight = nData[0];
		if (nRight > 3)									/*4级密码均可设置*/
		{
			nIsExist = 0x04;
		}
        else
        {
            memcpy(nPwd, nData + 1, 3);				/*密码判断*/
            if (MemReadAcPrm(F35, &nPassword, sizeof(nPassword)) >=0 )
            {
                nRetFlag = RET_ERR_OK;
            }
            if (RET_ERR_OK == nRetFlag)
            {
                if (0 == nRight)
				{
					if (0 != (memcmp(nPwd, g_nSuperPwd, 3))&&(0 != memcmp(nPwd, nPassword[0], 3)))
					{
						nIsExist = 0x04;
					}
				}
				else
				{
					if (0 != memcmp(nPwd, nPassword[nRight], 3))
					{
						nIsExist = 0x04;
					}
				}
            }
            else
            {
                if (0 == nRight)
				{
					if (0 != memcmp(nPwd, g_nSuperPwd, 3))
					{
						nIsExist = 0x04;
					}
				}
            }
            if (0 == nIsExist)
            {
                if (0xff == nData[8])
                {
                    memset(&sAcEvent, 0, 3 * sizeof(AC_EVECLR_STAT));
                    MemReadRamXmpData(F12, 0, &sAcEvent, 3 * sizeof(AC_EVECLR_STAT));
                    for(i=0; i<3; i++)
                    {
                        if (sAcEvent[i].nFlag != 0x55)
                        {
                            sAcEvent[i].nFlag = 0x55;
							memcpy(&sAcEvent[i].nMasterNo[0], &nData[4], 4);
							memcpy(&sAcEvent[i].nDataID[0], &nData[8], 4);
                            MemWriteRamXmpData(F12, 0, &sAcEvent, 3 * sizeof(AC_EVECLR_STAT));
                            break;
                        }
                    }
                    if (i >= 3)
                    {
                        nIsExist = 0x01;
                    }
                }
            }
        }
    }
    if (memcmp(nAddr,nBroad,6) != 0)
	{
		if (nIsExist != 0)
		{
			pSendBuf[0] = nCtrlCode | 0xC0;
			pSendBuf[1] = 1;
			pSendBuf[2] = nIsExist;
		}
		else
		{
			pSendBuf[0] = nCtrlCode | 0x80;
			pSendBuf[1] = 0;
		}
		return (pSendBuf[1]+2);
	}
	else
	{
		return 0;
	}
}

/********************************************************************************
* 函数名: Dlt07Code1c
* 输　入:
* 输　出:
* 返  回:
* 功  能: 跳合闸、报警、保电
*********************************************************************************/

INT8U Dlt07Code1c(const INT8U* pValidBuf, INT8U nValidLen, INT8U* pSendBuf, INT8U nType)
{
    INT8U   nCtrlCode         = 0;
    INT8U   nDataLen          = 0;
    INT8U   nTmp             = 0;
    INT8U   nData[256];
    INT8U   i                 = 0;
    INT8U   nEveData = 0;       //QJBL
    EVT_TRIP_DATA		    tEvtTrip;		//跳闸状态
    EVT_ALARM_DATA          tAlarm; 
    EVT_GUARPOWER_DATA	    tGuarantPower;		//保电状态
    
    nCtrlCode = pValidBuf[8];
	nDataLen = pValidBuf[9];
    
    if((PROT_GB645_07 == nType) && (nDataLen != 16))
    {
        nTmp = 1;
    }
    else
    {
        for(i=0; i<nDataLen; i++)
		{
			nData[i] = pValidBuf[10 + i] - 0x33;
		}
        switch(nData[8])
        {
            case 0x1a:  //跳闸
                MemReadAcXFlhBackData(F87, &tGuarantPower, sizeof(tGuarantPower));
                if (tGuarantPower.nGuarPower)
                {
                    nTmp = 2;    
                }
                else
                {
                    g_MeterInsideWrong[0] |= 0x40;
                    tEvtTrip.nEvtTrip = 1;
                    memcpy(&tEvtTrip.sTime, &nData[10], 6);
                    MemWriteAcXFlhBackData(F85, &tEvtTrip, sizeof(tEvtTrip));
                    g_RealMeterSwitchOnFlag =0xBF;
                    //g_SimMeterTripFlag = 0xFF;
                }
                break;
            case 0x1b:  //合闸允许
                g_MeterInsideWrong[0] &= ~0x40;
                tEvtTrip.nEvtTrip = 0;
                memcpy(&tEvtTrip.sTime, &nData[10], 6);
                MemWriteAcXFlhBackData(F85, &tEvtTrip, sizeof(tEvtTrip));
                g_RealMeterSwitchOnFlag = 0xFF;
                break;
            /*
            case 0x2A:  //报警
                tAlarm.nAlarm = 1;
                memcpy(&tAlarm.sTime, &nData[10], 6);
                MemWriteAcXFlhBackData(F86, &tAlarm, sizeof(tAlarm));
                break;
            case 0x2B:  //报警解除
                tAlarm.nAlarm = 0;
                memcpy(&tAlarm.sTime, &nData[10], 6);
                MemWriteAcXFlhBackData(F86, &tAlarm, sizeof(tAlarm));
                break;
            */
            case 0x3A:  //保电
                g_MeterInsideWrong[0] &= ~0x40;
                tGuarantPower.nGuarPower = 1;
                memcpy(&tGuarantPower.sTime, &nData[10], 6);
                MemWriteAcXFlhBackData(F87, &tGuarantPower, sizeof(tGuarantPower));
                break;
            case 0x3B:  //保电解除
                tGuarantPower.nGuarPower = 0;
                memcpy(&tGuarantPower.sTime, &nData[10], 6);
                MemWriteAcXFlhBackData(F87, &tGuarantPower, sizeof(tGuarantPower));
                break;
          default:
            nTmp = 2;   
            break;
        }
    }
    
     if(nTmp != 0)
	{
		pSendBuf[0] = nCtrlCode | 0xC0;
		pSendBuf[1] = 1;
		pSendBuf[2] = nTmp;
	}
	else
	{
		pSendBuf[0] = nCtrlCode | 0x80;
		pSendBuf[1] = 0;
	}
	return (pSendBuf[1]+2);  
}


/********************************************************************************
* 函数名:Dlt07GetACEventLen
* 输　入:INT8U nTblName, INT8U nSubItem
* 输　出:
* 返  回:
* 功  能:获取事件分项长度以及其偏移量
*********************************************************************************/
INT8S Dlt07GetACEventLen(INT8U nTblName, INT8U nSubItem, INT16U *nDataLen, INT16U *nOffset)
{
    INT8U  i = 0;
    INT8S  nRet = RET_ERR_OK;
	
	if (nSubItem < 1)
	{
		return RET_ERR_ERROR;	
	}

    *nDataLen = 0;
	*nOffset  = 0;
    
    switch(nTblName)
    {
        case F1:
		case F2:
		case F3:
            if(0xFF == nSubItem)
		    {
		    	for (i = 0; i < MAX_AC_SY_NUM; i++)
		    	{
		    		*nDataLen += g_nACSY[i];
		    	}
		    }
            else
            {
                *nDataLen = g_nACSY[nSubItem -1];
                if (nSubItem > 1)
		    	{
			    	for (i = 0; i < (nSubItem - 1); i++)
			    	{
			    		*nOffset += g_nACSY[i];
			    	}
		    	}
		    	else
		    	{
		    		*nOffset = 0;
		    	}
            }
            break;
        case F9:	//失流事件(A)
		case F10:	//失流事件(B)
		case F11:	//失流事件(C)
			if 	(0xFF == nSubItem)
		    {
				for (i = 0; i < MAX_AC_SL_NUM; i++)
		    	{
		    		*nDataLen += g_nACSL[i];
		    	}
	    	}
		    else
		    {
		    	*nDataLen = g_nACSL[nSubItem -1];
		    	if (nSubItem > 1)
		    	{
			    	for (i = 0; i < (nSubItem - 1); i++)
			    	{
			    		*nOffset += g_nACSL[i-1];
			    	}
		    	}
		    	else
		    	{
		    		*nOffset = 0;
		    	}
		    }
			break;
        case F4:	//断相事件(A)
		case F5:	//断相事件(B)
		case F6:	//断相事件(C)
			if 	(0xFF == nSubItem)
		    {
				for (i = 0; i < MAX_AC_DX_NUM; i++)
		    	{
		    		*nDataLen += g_nACDX[i];
		    	}
	    	}
		    else
		    {
		    	*nDataLen = g_nACDX[nSubItem -1];
		    	if (nSubItem > 1)
		    	{
			    	for (i = 0; i < (nSubItem - 1); i++)
			    	{
			    		*nOffset += g_nACDX[i];
			    	}
		    	}
		    	else
		    	{
		    		*nOffset = 0;
		    	}
		    }
			break;
		case F8:	//电压逆相序
			if 	(0xFF == nSubItem)
		    {
				for (i = 0; i < MAX_AC_DYNX_NUM; i++)
		    	{
		    		*nDataLen += g_nACDYNX[i];
		    	}
	    	}
		    else
		    {
		    	*nDataLen = g_nACDYNX[nSubItem -1];
		    	if (nSubItem > 1)
		    	{
			    	for (i = 0; i < (nSubItem - 1); i++)
			    	{
			    		*nOffset += g_nACDYNX[i];
			    	}
		    	}
		    	else
		    	{
		    		*nOffset = 0;
		    	}
		    }
			break;
	    default:
	    	nRet = RET_ERR_ERROR;
	    	break;
    }
}

INT32S CheckFauCode(INT8U*nDI01,INT8U* nData,INT8U nRetFlag)
{  
   INT8U i =0;
   if(nDI01[3]==0xB1)
    {
       if(  (nDI01[1]==0&&(nDI01[0]==3||nDI01[0]==4||nDI01[0]==5))
          ||(nDI01[1]==1&&(nDI01[0]==4||nDI01[0]==5||nDI01[0]==6||nDI01[0]==7)) )
       {
           for(i=0;i<3;i++)
           {
              if(nData[12+i]>1)
              {
                 return RET_ERR_ERROR;
              }
           }
       }
       else
       {
          if((nDI01[0]!=3)&&(nDI01[1]!=1))
          {
              if(nData[12]>1)
              {
                 return RET_ERR_ERROR;
               }
          }
       }
       return nRetFlag;
    }
   else
   {
       return nRetFlag;
   }
}

void AcCalcJustPluseConst(INT8U nPlusePersent)
{
    INT32U	nChkSum = 0;
    INT32U  nHFconsttemp = 0;
	MEM_AC_CAL_PRM sJB_Param;
	
	memset(&sJB_Param, 0, sizeof(MEM_AC_CAL_PRM));
              
    if (MemReadAcCalPrm(FEND, &sJB_Param, sizeof(sJB_Param))  > 0 )
    {
        nChkSum = RET_ERR_OK;
    }
    nChkSum = CheckINT32USum((INT32U*)(&sJB_Param), 0, sizeof(MEM_AC_CAL_PRM)/4 - 3);
    
    if (nChkSum != sJB_Param.nCheckSumEntire)
	{
		return;
    }
    
    if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_100 == g_nHardType))
    {
      nHFconsttemp = 0x000000B6;
    }

    else if(SINGLEPHASE_METER == g_nHardType)  
    {
      nHFconsttemp = 147;
    }
    else if(TRIPHASE_METER_220ZT == g_nHardType)  
    {
      nHFconsttemp = 874;
    }
    else if (TRIPHASE_METER_57U == g_nHardType)
    {
      nHFconsttemp = 58;
    }
    else if(TRIPHASE_METER_100U == g_nHardType)  
    {
      nHFconsttemp = 85;
    }
    
    
	if (nPlusePersent)
    {
        sJB_Param.nHfconst = (nHFconsttemp * 100) / nPlusePersent;
    
    }else{
      
        sJB_Param.nHfconst = nHFconsttemp;
    }
    
    sJB_Param.nCheckSumEntire = CheckINT32USum((INT32U*)(&sJB_Param), 0, sizeof(MEM_AC_CAL_PRM)/4 - 3);
    MemWriteAcCalPrm(FEND, &sJB_Param, sizeof(sJB_Param));  
}

void AcCalcJustPluse(void)
{
    INT32U	nChkSum = 0;
    INT32U  nHFconsttemp = 0;
	MEM_AC_CAL_PRM sJB_Param;
	
	memset(&sJB_Param, 0, sizeof(MEM_AC_CAL_PRM));
              
    if (MemReadAcCalPrm(FEND, &sJB_Param, sizeof(sJB_Param))  > 0 )
    {
        nChkSum = RET_ERR_OK;
    }
    nChkSum = CheckINT32USum((INT32U*)(&sJB_Param), 0, sizeof(MEM_AC_CAL_PRM)/4 - 3);
    
    if (nChkSum != sJB_Param.nCheckSumEntire)
	{
		return;
    }
    
    if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType))
    {
        nHFconsttemp = 0x000000B6;
    }

    else if(SINGLEPHASE_METER == g_nHardType)  
    {
        nHFconsttemp = 147;
    }
    else if(TRIPHASE_METER_220ZT == g_nHardType)  
    {
        nHFconsttemp = 874;
    }
    else if (TRIPHASE_METER_57U == g_nHardType)
    {
        nHFconsttemp = 58;
    }
    else if(TRIPHASE_METER_100U == g_nHardType)  
    {
        nHFconsttemp = 82;
    }
    else if(TRIPHASE_METER_100 == g_nHardType)  
    {
        nHFconsttemp = 221;
    }
    

    sJB_Param.nHfconst = nHFconsttemp;

    
    sJB_Param.nCheckSumEntire = CheckINT32USum((INT32U*)(&sJB_Param), 0, sizeof(MEM_AC_CAL_PRM)/4 - 3);
    MemWriteAcCalPrm(FEND, &sJB_Param, sizeof(sJB_Param));  
}

void AcCalcEMUCfgConst(void)
{
    INT32U	nChkSum = 0;
    INT32U  nEMUCfg = 0;
	MEM_AC_CAL_PRM sJB_Param;
	
	memset(&sJB_Param, 0, sizeof(MEM_AC_CAL_PRM));
              
    if (MemReadAcCalPrm(FEND, &sJB_Param, sizeof(sJB_Param))  > 0 )
    {
        nChkSum = RET_ERR_OK;
    }
    nChkSum = CheckINT32USum((INT32U*)(&sJB_Param), 0, sizeof(MEM_AC_CAL_PRM)/4 - 3);
    
    if (nChkSum != sJB_Param.nCheckSumEntire)
	{
		return;
    }
    
    if ((TRIPHASE_METER_100U == g_nHardType)||(TRIPHASE_METER_100 == g_nHardType))
    {
        nEMUCfg = 12676;
    }
    else 
    {
        nEMUCfg = 12740;  //12676
    }
    
    sJB_Param.nEMUCfg = nEMUCfg;
    
    sJB_Param.nCheckSumEntire = CheckINT32USum((INT32U*)(&sJB_Param), 0, sizeof(MEM_AC_CAL_PRM)/4 - 3);
    MemWriteAcCalPrm(FEND, &sJB_Param, sizeof(sJB_Param));  
}


INT8U PulLamChange() //脉冲灯故障
{
  
    if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType))
    {
      
      if((g_MeterInsideWrong[1]&0x02))
      {
         AcCalcJustData3();
      }
      else
      {
         AcCalcJustData1(); //6400
      }
    }
    else if(SINGLEPHASE_METER == g_nHardType)
    {
      
      if((g_MeterInsideWrong[1]&0x02))
      { 
         AcCalcJustData5(); 
      }
      else
      {
         AcCalcJustData2(); //1200
      }
    }
    else if(TRIPHASE_METER_220ZT == g_nHardType)
    {
      
      if((g_MeterInsideWrong[1]&0x02))
      {
         AcCalcJustData2(); 
      }
      else
      {
         AcCalcJustData3(); //400
      }  
    }
    if (TRIPHASE_METER_57U == g_nHardType)
    {
      
      if((g_MeterInsideWrong[1]&0x02))
      { 
         AcCalcJustData1(); 
      }
      else
      {
        AcCalcJustData4();//20000
      }
    }
    else if(TRIPHASE_METER_100U == g_nHardType)
    {
      
      if((g_MeterInsideWrong[1]&0x02))
      { 
         AcCalcJustData1(); 
      }
      else
      {
        AcCalcJustData5(); //20000
      }
    }
    else if(TRIPHASE_METER_100 == g_nHardType)
    {
      if((g_MeterInsideWrong[1]&0x02))
      {
         AcCalcJustData2(); 
      }
      else
      {
         AcCalcJustData6(); //6400
      }        
    }

}
