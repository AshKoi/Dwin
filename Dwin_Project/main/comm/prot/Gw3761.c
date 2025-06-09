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
#include "Base.h"
#include "SysRes.h"
#include "linkcomExt.h"
#include "linkcom.h"

#include "Gw3761.h"
#include "AssitProt.h"

#include "GuiwinCfg.h"
#include "Guiwin.h"

#include "Fun.h"
#include "Guimenu.h"
#include "GB3761CtrlCmd.h"
#include "GB3761GetClassI.h"
#include "GB3761GetClassII.h"
#include "GB3761GetPrm.h"
#include "GB3761SetPrm.h"

#include "GB3761GetPrmExt.h"
#include "GB3761SetPrmExt.h"
#include "FileTrans.h"
#include "GB3761Trans.h"

#include "Login.h"

#include "MemTbl.h"

#include "meter645.h"

#include "Reset.h"
#include "AutoTest.h"

#include "SecTask.h"
/**************************************************************************************************
                                           变量定义
***************************************************************************************************/
// 3761规约处理
const GW3761_PROC_AFN g_sGw3761ProcAfn[] = {
	//{0x0A, (void*)Gw3761GetParam},
	//{0x04, (void*)Gw3761SetParam},
    //{0x05, (void*)Gw3761CtrlProc},
    //{0x0C, (void*)Gw3761ReadIData},
    //{0x0D, (void*)Gw3761ReadIIData},
    //{0x0F, (void*)Gw3761FileProc},
    {0xDF, (void*)Gw3761LoginProc},
	//{0x10, (void*)Gw3761TransProc},
    //{0x01, (void*)Gw3761ResetProc},
    //{0xFA, (void*)Gw3761GetParamExt},
    //{0xF4, (void*)Gw3761SetParamExt},
		
	{0xFF}
};


const  INT8U		g_nRead97Buf[] 	= {0x68, 0x98, 0x98, 0x98, 0x98, 0x98, 0x98, 0x68, 0x01, 0x02, 
                                            0x52, 0xC3, 0x78, 0x16}; 
const  INT8U		g_nReadSelfBuf2[] 	= {0x68, 0x98, 0x98, 0x98, 0x98, 0x98, 0x98, 0x68, 0x11, 0x04, 
										  0x34, 0x32, 0x34, 0x33, 0x42, 0x16};
const  INT8U		g_nReadDayData[] 	= {0x68, 0x98, 0x98, 0x98, 0x98, 0x98, 0x98, 0x68, 0x11, 0x04, 
										  0x34, 0x34, 0x39, 0x38, 0x4D, 0x16};
const  INT8U		g_nBroadTimeBuf[] 	= {0x68, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x68, 0x08, 0x06, 
										  0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0xA6, 0x16};





INT8U 				g_nAsynFlg 		= 0;									//异步处理规约标志: 为1时。或者为了组帧时组成广播地址帧
static INT16U		g_nTmpSelfArea		= 0xFFFF;
static INT16U		g_nTmpSelfAddr		= 0xFFFF;
static INT8U    	g_nSeq      		= 0;
static INT8U     	g_nAsynSeq  		= 0;
INT8U				g_nPackAssitFlg	= 0;
INT8U				g_nReport			= 0;									//上报标志

INT8U           Ascend                      = 0;     //上行通讯标志
INT8U           Descend                     = 0;     //下行通讯标志

INT8U           g_nGprsWatchFlg               = 0;  

INT8U           g_GroadAddrFlg                = 0;
INT8U           g_nFileGroadFlg                = 0;

/**************************************************************************************************
                                           函数
***************************************************************************************************/
extern INT32S ReadCommMs(INT8U nCom, INT8U* pBuf, INT32U nNeedRdLen, INT32U nInter, INT32U nDelay);
extern INT32S WriteComm(INT8U nCom, INT8U* pBuf, INT32U nNeedLen);


/**************************************************************************************************
                                           函数
***************************************************************************************************/
/*
*****************************************************************************
* 函数名: Gw3761UpAfnGet
* 输　入: 
* 输  出: 
* 返  回: 
*       
*       
* 功  能: 上行afn转换
*****************************************************************************
*/
static INT8U Gw3761UpAfnGet(INT8U nAfn)
{
    INT32S 		i 					= 0;
	const INT8U	c_nMatchAfn[][2]	= 	{
										    {0x05, 0x00},
										    {0x04, 0x00},
                                            {0x01, 0x00},
											{0x02, 0x00},
                                            {0xF4, 0x00},
										    
										    {0xFF},
										};
    
    while(1)
    {
        if (0xFF == c_nMatchAfn[i][0])
        {
            return nAfn;
        }
        
        if (nAfn == c_nMatchAfn[i][0])
        {
            return c_nMatchAfn[i][1];
        }
        i++;
    }
}

/*
*****************************************************************************
* 函数名: Gw3761FrmWrite
* 输　入: 
* 输  出: 
* 返  回: 
*       
*       
* 功  能: 
*****************************************************************************
*/
INT32S Gw3761FrmWrite(INT8U nCom, INT8U* pData, INT32S nDatalen)
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
* 函数名: Gw3761FrmRead
* 输　入: 
* 输  出: 
* 返  回: 
*       
*       
* 功  能: 有异步规约处理，即编写这样的函数
*****************************************************************************
*/
INT32S Gw3761FrmRead(INT8U nCom, INT8U* pData, INT32S nMaxDatalen, INT32U iInterval, INT32U ms)
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

/*
*******************************************************************************
* 函数名:GB3761PnFn2DADT
* 输　入:pOutBuf 输出DADT，输出格式按照DA1 DA2 DT1 DT2的顺序排列
*        nPn     信息点（n = 0 ~ 2040）
*        nFn     信息类（n = 1 ~ 248）
* 输　出:0  表示输入参数转换成功
*        -1 表示输入参数超范围或者输出缓冲区无效
* 返  回:
* 功  能:根据输入的nPn、nFn转换为DA DT的格式
********************************************************************************
*/
INT32S GB3761PnFn2DADT(INT16U nPn, INT8U nFn, INT8U* pOutBuf)
{
    if(NULL == pOutBuf)
    {
        return -1;
    }
    if (nPn > 2040) //规约规定的最大信息点
    {
        return -1;
    }
    if (nFn > F248) //规约规定的最大功能项
    {
        return -1;
    }
    //有效范围的输入和输出参数，进行数据格式转换
    if (nPn == 0)
    {
        pOutBuf[0] = 0;
        pOutBuf[1] = 0;
    }
    else
    {
        pOutBuf[0] = 1 << ((nPn - 1) % 8);
        pOutBuf[1] = (nPn - 1) / 8 + 1;
    }
    pOutBuf[2] = 1 << (nFn % 8);
    pOutBuf[3] = nFn / 8;
    
    return 0;
}


/********************************************************************************
* 函数名: GB3761DaDt2PnFn
* 输　入:
* 输　出:
* 返  回: RET_ERR_OK正确, 其余错误
* 功  能: Dadt转换Pnfn
*********************************************************************************/
INT8S GB3761DaDt2PnFn(const INT8U *pDaDt, INT16U *pPn, INT16U *pPnNum, INT16U *pFn, INT8U *pFnNum, BOOLEAN *pAllMp)
{
	BOOLEAN bAllMpFlag = FALSE;
    INT8U	nDa1 = 0, nDa2 = 0, nDt1 = 0, nDt2 = 0;
    INT8U	nRow = 0, nCol = 0;
    INT8U   nFnNum = 0;
    INT16U  nPnNum = 0;

    if ((NULL == pDaDt) || (NULL == pPn) || (NULL == pFn) || (NULL == pPnNum) || (NULL == pFnNum) || (NULL == pAllMp))
    {
        return RET_ERR_POINT;
    }

    nDa1 = pDaDt[0];
    nDa2 = pDaDt[1];
    nDt1 = pDaDt[2];
    nDt2 = pDaDt[3];

    if (0 == nDt1)       //DT 有效性判断 DT1大于0
    {
        return RET_ERR_ERROR;
    }

    nRow = nDt2;                          //Fn
    for (nCol = 0; nCol < 8; nCol++)
    {
        if ((nDt1) & (1 << nCol))
        {
            pFn[nFnNum++] = (nRow) * 8 + nCol;
        }
    }

    if ((nDa1 == 0) && ((nDa2) == 0))//P0表示终端
	{
		pPn[nPnNum++] = 0;
	}
//#if (GB3761_ALLMP_TYPE == GB3761_ALLMP_FF_FF)
//    else if ((0xFF == nDa1) && (0xFF == nDa2)) //Da 为 全体测量点 特殊处理
//#elif (GB3761_ALLMP_TYPE == GB3761_ALLMP_FF_00)
//    else if ((0xFF == nDa1) && (0 == nDa2))    //Da 为 全体测量点 特殊处理
//#endif
    //支持2种全体测量点
    else if (((0xFF == nDa1) && (0xFF == nDa2)) || ((0xFF == nDa1) && (0 == nDa2)))
    {
        bAllMpFlag = TRUE;
    }
    else
    {
    #if (TERM_SCH_TYPE == TERM_QGDW3761_SCH)
        nRow = nDa2 - 1;
    #elif (TERM_SCH_TYPE == TERM_QGDW130_SCH)
    	for (nRow = 0; nRow < 8; nRow++)
	    {
	    	if (TEST_BIT(nDa2, nRow))
	    	{
	    		break;
	    	}
	    }
    #endif
        for (nCol = 0; nCol < 8; nCol++)
        {
            if ((nDa1) & (1 << nCol))
            {
                pPn[nPnNum++] = nRow * 8 + nCol + 1;
            }
        }
	}
    if (0 == nFnNum)
    {
        return RET_ERR_ERROR;
    }
    *pFnNum = nFnNum;
    *pAllMp = bAllMpFlag;
    if (!bAllMpFlag)
    {
    	if (0 == nPnNum)
    	{
    		return RET_ERR_ERROR;
    	}
    	*pPnNum = nPnNum;
    }
    return RET_ERR_OK;
}

/****************************************************************************
* 函数名:GB3761GetAllMpNo
* 输　入:
* 输　出:
* 返  回:
* 功  能:获取全体测量点, 根据Fn数量获取Pn数量 在DaDt2PnFn之后执行
****************************************************************************/
INT8S GB3761GetAllMpNo(INT8U nAfn, INT16U nFn, INT16U *pPnID, INT16U *pPnNum, INT16U nMaxLen)
{
    BOOLEAN bVld = FALSE;
    INT16U nMpOrder = 0;
    INT16U nMpNo = 0;
    INT16U nPnNum = 0;
    
    if ((NULL == pPnID) || (NULL== pPnNum))
    {
        return RET_ERR_POINT;
    }
    switch (nAfn)
    {
        case GB3761_AFN_REQ_CLASS_I:
        {
            if ((nFn >= F17 && nFn <= F24) || (nFn >= F81 && nFn <= F84)) //总加组
            {
                
            }
            else if ((nFn >= F25 && nFn <= F67)
                    || (nFn >= F89 && nFn <= F116)
                    || (nFn >= F129 && nFn <= F170))//测量点
            {
                for (nMpOrder = 1; nMpOrder <= MAX_MP; nMpOrder++)
                {
                    
                }
            }
        }
    }
}

/*
*****************************************************************************
* 函数名: Gw3761MakeFrm
* 输　入: 
*           INT8U               nCtrl           
*			INT8U 				nAfn						
*			INT8U* 				pData				组帧的数据内容：包含dadt及内容
*			INT32S 				nDatalen			组帧的数据长度
*			INT32S 				nMaxDatalen			缓冲区最大长度
* 输  出: 
*			INT8U* 				pData				组帧的缓冲
* 返  回: 返回组帧的长度
*       
*       
* 功  能: 组织376.1规约帧
*****************************************************************************
*/
INT32S Gw3761MakeFrm(INT8U nCtrl, INT8U nAfn, INT8U* pData, INT32S nDatalen, INT32S nMaxDatalen)
{
	pGW376_1_PROT 	pGw376Prot 	= (pGW376_1_PROT)pData;
	INT16U			nFrmLen 	= 0;
    INT16U          nTermArea   = 0;
    INT16U          nTermAddr   = 0;
    //INT8U           nTermAreaAddr[4] = {0};
    INT8U           nSeq        = 0;
	INT8U			nPSQ		= 0x60;
	
    if (NULL == pData)
	{
		return -1;
	}
	if (nDatalen < 0)
	{
		return -1;
	}
	if (GW376_1_HEAD_LEN + nDatalen + 2 > nMaxDatalen)						//帧长度不能大于缓冲区最大长度
	{
		return -1;
	}
	nFrmLen = (nDatalen + 8) * 4 + 0x02;									//帧长度等于内容长度(nDatalen + ctrl + addr + afn + seq)
	memmove(&pGw376Prot->nDadtData[0], pData, nDatalen);
	pGw376Prot->nHead1 = 0x68;
	INT16U2Buf(&pGw376Prot->nLen1[0], nFrmLen);
	INT16U2Buf(&pGw376Prot->nLen2[0], nFrmLen);
	pGw376Prot->nHead2 = 0x68;
	pGw376Prot->nCtrl  = nCtrl;
	if (g_nAsynFlg == 1)
    {
        nTermArea = g_nTmpSelfArea;
        nTermAddr = g_nTmpSelfAddr;
        nSeq = g_nAsynSeq;
    }
    else
    {
        //+++++ 
        nTermArea = 0x3205;
        nTermAddr = 0x0001;
        nSeq = g_nSeq;
    }
    INT16U2Buf(&pGw376Prot->nTermAddr[0], nTermArea);
	INT16U2Buf(&pGw376Prot->nTermAddr[2], nTermAddr);
	if ((0xFFFF == nTermArea) || (0xFFFF == nTermAddr))
	{
		pGw376Prot->nMasterAddr = 0x03;
	}
	else
	{
		if (g_nReport)
		{
			pGw376Prot->nMasterAddr = 0;
			nPSQ = 0x70;
		}
		else
		{
			pGw376Prot->nMasterAddr = 0x02;
		}
	}
	pGw376Prot->nAfn = nAfn;
	pGw376Prot->nSeq = nPSQ + (nSeq & 0x0F);
    if (g_nAsynFlg == 1)
    {
      
    }
    else
    {
        g_nSeq++;
    }
	pGw376Prot->nDadtData[nDatalen] = CheckSum(pData, 6, nDatalen + 8);
	pGw376Prot->nDadtData[nDatalen + 1] = 0x16;
	
	return (GW376_1_HEAD_LEN + nDatalen + 2);
}

/*
*****************************************************************************
* 函数名: Gw3761CheckPackage
* 输　入: 
*			INT8U*			pData
*			INT32S			nDatalen			接收到的报文长度
* 输  出: 
*			INT8U*			pData 				如果有完整帧，重新复制到 pData 
* 返  回: 返回完整帧的长度
* 功  能: 376.1规约分析
*****************************************************************************
*/
INT32S Gw3761CheckPackage(INT8U* pData, INT32S nDatalen)
{
	INT32S	nCycCnt 	= (nDatalen - GW376_1_BASE_LEN + 1);
	INT32S	i       	= 0;
	INT16U	nFrmlen1 	= 0;
	INT16U	nFrmlen2 	= 0;
	INT8U	nCs			= 0;
	//INT16U	nTmp		= 0;
	
	if (nDatalen < GW376_1_BASE_LEN)
	{
		return -1;
	}
	for (i = 0; i < nCycCnt; i++)
	{
		if (0x68 != pData[i])
		{
			continue;
		}
		nFrmlen1 = Buf2INT16U(&pData[i + 1]);
		nFrmlen2 = Buf2INT16U(&pData[i + 3]);
		if (nFrmlen1 != nFrmlen2)
		{
			continue;
		}
		nFrmlen1 = nFrmlen1 / 4;
		if (nFrmlen1 + 6 + 2 + i > nDatalen)
		{
			continue;
		}
		if (0x68 != pData[i + 5])
		{
			continue;
		}
		//地址
		
		nCs = CheckSum(&pData[i], 6, nFrmlen1);
		if (nCs != pData[i + 6 + nFrmlen1])
		{
			continue;
		}
		if (0x16 != pData[i + 6 + nFrmlen1 + 1])
		{
			continue;
		}
		
		break;
	}
	if (i >= nCycCnt)
	{
		return -1;
	}
	
	nFrmlen1 = nFrmlen1 + 6 + 2;
	memmove(pData, &pData[i], nFrmlen1);
	
	return (INT32S)nFrmlen1;
}

/*
*****************************************************************************
* 函数名: Gw3761Proc
* 输　入: 		
*			INT8U*			pData
*			INT32S			nDatalen			帧内容长度
*			INT32S			nMaxDatalen			接收到的报文长度
* 输  出: 
*			INT8U*			pData 				完整帧
* 返  回: 返回完整帧的长度
* 功  能: 376.1规约分析
*****************************************************************************
*/
INT32S Gw3761Proc(INT8U nCom, INT8U* pData, INT32S nDatalen, INT32S nMaxDatalen)
{
	//INT8U			nCom			= 0;
	INT8U			nCnt			= 0;
	INT32S			nTmp			= 0;
	INT32S			nTimeOut		= 3000;
	pGW376_1_PROT	pGw3761Prot	= (pGW376_1_PROT)pData;
    INT8U           nUpAfn        	= 0;
	
	if ((NULL == pData) || (nDatalen < 0) || (nMaxDatalen <= 0))
	{
		return -1;
	}
    
    if (KEY_CANCEL == PrintCommHex(pData, nDatalen))
    {
        return -1;    
    }
	g_nTimeOut[sizeof(g_nTimeOut) - 1] = 0;
	nTimeOut = StringToVal((INT8U*)&g_nTimeOut[0], strlen((char*)&g_nTimeOut[0]));
	nTimeOut = GET_MIN(nTimeOut, 30000);			//不能超过30s
	nTimeOut = GET_MAX(nTimeOut, 1000);			//不能小于1s	
    nUpAfn = Gw3761UpAfnGet(pGw3761Prot->nAfn);
    if (0x10 == pGw3761Prot->nAfn)
	{
		if (31 == pGw3761Prot->nDadtData[4])
		{
			nTimeOut = 20000;
		}
		else
		{
			nTimeOut = 10000;
		}
	} 
	nCom = ProcCommCfgPend(nCom);					
	//如果是测试载波，发送之前包内部规约
#if AUTO_TEST_EN == 2
	AssistMakeFrm(pData, nDatalen, nMaxDatalen, ASSIST_CARRIER, 0, PROT_BASE, 2, 0,0x2130);
    nDatalen += ASSIST_PROT_HEAD_TAIL_LEN;
#else
	if (g_nPackAssitFlg)
	{
		AssistMakeFrm(pData, nDatalen, nMaxDatalen, ASSIST_CARRIER, 0, PROT_BASE, 2, 0,0x2130);
    	nDatalen += ASSIST_PROT_HEAD_TAIL_LEN;
	}
#endif
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
     
        if (KEY_CANCEL == GetKeyVal(1))
        {
            nTmp = CANCEL_VALUE;
            break;
        }
		nTmp = ReadCommMs(nCom, &pData[nDatalen], nMaxDatalen - nDatalen, 50, 50);
		if (nTmp > 0)
		{
			nDatalen += nTmp;
		}
		nTimeOut -= 100;

		if (nDatalen >= GW376_1_BASE_LEN)
		{
			nTmp = Gw3761CheckPackage(pData, nDatalen);
			if (nTmp > 0)
			{
                PrintCommHex( pData, nDatalen);
                
                if (nUpAfn != pGw3761Prot->nAfn)
                {
                    nTmp = -1;
					break; 
                }
                if ((pGw3761Prot->nCtrl & 0x80) != 0x80)
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
* 函数名: Gw3761AsynCheckPackageOther
* 输　入: 
*		
* 输  出: 
* 返  回: 
*		0成功，-1失败
*       
*       
* 功  能: 
*****************************************************************************
*/
static INT32S Gw3761AsynCheckPackageOther(INT8U nCom, pBASE_BUF pBaseBuf)
{
	INT32S		i       		= 0;
	INT32U      nDataNum    	= 0;

	nDataNum = BASEBUF_BYTE_NUM(pBaseBuf);

	if ((nCom == COMM_WH232) && (nDataNum >= 9))
	{
		#define RMOT_CMD_LEN 	strlen((char*)"rmot ")
		if (0 == memcmp(&(BASEBUF_PROC_PBUF(i)), "rmot ", RMOT_CMD_LEN))
		{
			if (0 == memcmp(&(BASEBUF_PROC_PBUF(i + RMOT_CMD_LEN)), "open", strlen((char*)"open")))
			{
				SetRmotCmdPrintFlg(1);	
			}
			else if (0 == memcmp(&(BASEBUF_PROC_PBUF(i + RMOT_CMD_LEN)), "close", strlen((char*)"close")))
			{
				SetRmotCmdPrintFlg(0);
			}
		}
	}
#ifdef GPRS_MOD_DEBUG	
	if (nDataNum > 0)
	{
		for (i = 0; i < nDataNum; i++)
		{
			if (nCom == COMM_WH232)
			{
				Gw3761FrmWrite(COMM_GPRS, &(BASEBUF_PROC_PBUF(i)), 1);
			}
			else if (nCom == COMM_GPRS)
			{
				Gw3761FrmWrite(COMM_WH232, &(BASEBUF_PROC_PBUF(i)), 1);
			}
		}
		pBaseBuf->nProcPtr = BASEBUF_PROC_PTR(nDataNum);
	}
	return -1;
#endif	

	return 0;
}

INT32S Gw3761AsynCheckPackageTmp(void* pPara)
{
#if 1
    //规约切换
    if (g_pMem->sXRamData.nRdMtProtFlg == 2)
    {
        return Gw3761AsynCheckPackage(pPara);
    }
    else
    {
        return Meter645AsynCheckPackage(pPara);
    }
#endif
    return 0;
}

/*
*****************************************************************************
* 函数名: Gw3761AsynCheckPackage
* 输　入: 
*		pLINK_COM pLinkCom
* 输  出: 
* 返  回: 
*		0成功，-1失败
*       
*       
* 功  能: 有完整帧，返回0成功，否则-1    相当于 Gw3761CheckPackage 
*****************************************************************************
*/
INT32S Gw3761AsynCheckPackage(void* pPara)
{
	INT16U		nHeadLen		= STR_OFF(pGW376_1_PROT, nDadtData);
	pLINK_COM   pLinkCom    	= (pLINK_COM)pPara;
    pBASE_BUF	pBaseBuf		= NULL;
	INT32S		i       		= 0;
	INT16U		nFrmlen1 		= 0;
	INT16U		nFrmlen2 		= 0;
	INT8S		nRtn			= -1;
    INT32U      nDataNum    	= 0;
    INT8U       nCs         	= 0;
    INT8U       nTermAreaAddr[4]= {0};
    INT8U       nTermAreaAddrTmp[4]= {0};
    //INT8U       nCtrl        	= 0;
    
	
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
	if (Gw3761AsynCheckPackageOther(pLinkCom->nCom, pBaseBuf) < 0)
	{
		return -1;
	}
	
	nDataNum = BASEBUF_BYTE_NUM(pBaseBuf);
	if (nDataNum < nHeadLen + 4 + 2)
	{
		return -1;
	}

	if (pLinkCom->nFrmLen > 0)
	{
		return 0;
	}
	memset(&nTermAreaAddr[0], 0, 4);
    memset(&nTermAreaAddrTmp[0], 0, 4);
    
    memcpy(&nTermAreaAddr[0], &g_pMem->sTermComm.nAreaCode[0], 2);
    memcpy(&nTermAreaAddr[2], &g_pMem->sTermComm.nTermAddr[0], 2);
    
    //生产编号
    memcpy(&nTermAreaAddrTmp[0], &g_pMem->sTermComm.nAreaCodeTmp[0], 2);
    memcpy(&nTermAreaAddrTmp[2], &g_pMem->sTermComm.nTermAddrTmp[0], 2);
    
	//_API_FRAM_Read(STR_OFF(pFRAM_STORE, nTermAreaAddr), &nTermAreaAddr[0], STR_SIZE(pFRAM_STORE, nTermAreaAddr));      
	//memcpy(&nTermAreaAddr[0], &g_pParam->nTermAddr[0], 4);
    //SwapBufVal(&nTermAreaAddr[0], 2);
	//SwapBufVal(&nTermAreaAddr[2], 2);
	//nTermAreaAddr[0] = 0x05;
	//nTermAreaAddr[1] = 0x32;
	//nTermAreaAddr[2] = 0x01;
	//nTermAreaAddr[3] = 0x00;
	for (i = 0; i < nDataNum - (nHeadLen + 4 + 2 - 1); i++)
	{		
		// 1. 报文头
		if (0x68 != BASEBUF_PROC_PBUF(i + 0))
		{
			continue;	
		}
		nFrmlen1 = MAKEWORD(BASEBUF_PROC_PBUF(i + 1), BASEBUF_PROC_PBUF(i + 2));
		nFrmlen2 = MAKEWORD(BASEBUF_PROC_PBUF(i + 3), BASEBUF_PROC_PBUF(i + 4));
		if (nFrmlen1 != nFrmlen2)
		{
			continue;
		}
		nFrmlen1 = nFrmlen1 / 4;
		if (nFrmlen1 + 6 + 2 + 1 > pBaseBuf->nBufSize)
        {
            continue;	
        }
        else if (nFrmlen1 + 6 + 2 + i > nDataNum)
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
		else if (nFrmlen1 < 8)
		{
			continue;
		}
        else
        {
            pLinkCom->sFrmInfo.sFrmClrInfo.nRcvContinueCnt = 0;    
        }
		if (0x68 != BASEBUF_PROC_PBUF(i + 5))
		{
			continue;	
		}
        
        if (IsAllEqualVal(&BASEBUF_PROC_PBUF(i + 7), 4, 0xFF))
        {
              g_GroadAddrFlg = 1; 
        }
        else
        {
              g_GroadAddrFlg = 0;
        }
        
		// 地址 1.响应全FF，2.如果是生产编号，只可以设置F94
		if (!IsAllEqualVal(&BASEBUF_PROC_PBUF(i + 7), 4, 0xFF))
		{  
            if(0 == memcmp(&BASEBUF_PROC_PBUF(i + 7), &nTermAreaAddrTmp[0], sizeof(nTermAreaAddrTmp)))
            {
                if ((BASEBUF_PROC_PBUF(i + 16) != 0x20) || (BASEBUF_PROC_PBUF(i + 17) != 0x0B))
                {
                    if (0 != memcmp(&BASEBUF_PROC_PBUF(i + 7), &nTermAreaAddr[0], sizeof(nTermAreaAddr)))
                    {
                        continue;
                    }
                }
            }
            else
            {
                if (0 != memcmp(&BASEBUF_PROC_PBUF(i + 7), &nTermAreaAddr[0], sizeof(nTermAreaAddr)))
                {
                    continue;
                }
            }
		}
		
		// 6. CS
		nCs = FifoCalcCSBaseBuf(pBaseBuf->pBuf, pBaseBuf->nBufSize, BASEBUF_PROC_PTR(i + 6), nFrmlen1);
		if (nCs != BASEBUF_PROC_PBUF(i + 6 + nFrmlen1))
		{
			continue;
		}
		// 7. 报文尾
		if (0x16 != BASEBUF_PROC_PBUF(i + 6 + nFrmlen1 + 1))
		{
			continue;
		}
#if 1 == HAVE_FRM_BUFF
		if (nFrmlen1 + 6 + 2 > pLinkCom->sFrmBuf.nBufSize)
		{
            continue;
		}
		pLinkCom->nFrmLen = nFrmlen1 + 6 + 2;
		FifoCopyBaseBuf(pLinkCom->sFrmBuf.pBuf, pLinkCom->sFrmBuf.nBufSize, 0, pBaseBuf->pBuf, pBaseBuf->nBufSize, BASEBUF_PROC_PTR(i + 0), pLinkCom->nFrmLen);
#else
        pLinkCom->nFrmLen = nFrmlen1 + 6 + 2;
		pLinkCom->sFrmBuf.pBuf = &pBaseBuf->pBuf[BASEBUF_PROC_PTR(i + 0)];
        g_nAsynSeq = pBaseBuf->pBuf[BASEBUF_PROC_PTR(i + 13)] & 0x0F;
#endif
		FifoCopyBaseBuf(pBaseBuf->pBuf, pBaseBuf->nBufSize, BASEBUF_PROC_PTR(i + 7), &nTermAreaAddr[0], sizeof(nTermAreaAddr), 0, 4);
		
		nRtn = 0;
		break;
	}
	
	pBaseBuf->nProcPtr = BASEBUF_PROC_PTR(i + 0);
#if 1 == HAVE_FRM_BUFF	
	pBaseBuf->nProcPtr = BASEBUF_PROC_PTR(pLinkCom->nFrmLen);
#endif	
	return nRtn;
}

INT32S Gw3761AsynProcTmp(void* pPara)
{
#if 1
    //规约切换
    if (g_pMem->sXRamData.nRdMtProtFlg == 2)
    {
        return Gw3761AsynProc(pPara);
    }
    else
    {
        return Meter645AsynProc(pPara);
    }
#endif
    return 0;
}


/*
*****************************************************************************
* 函数名: Gw3761AsynProc
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
INT32S Gw3761AsynProc(void* pPara)
{
	pGW376_1_PROT	pUpRcvLinkProt				= NULL;
	pLINK_COM  	 	pLinkCom                    = (pLINK_COM)pPara;
    pBASE_BUF		pBaseBuf					= NULL;
	INT8U*			pSndData					= NULL;
	INT32S			nSndDataLen				    = 0;
	INT8U*			pRcvData					= NULL;
	INT32S			nRcvDataLen				    = 0;		
	INT32S			nSendLen					= 0;
	INT32S			nRet						= 0;
	INT8U			nCtrl						= 0;
	INT8U			nAfn						= 0;
	INT8U			nErr						= 0;
    INT8U           nCom                        = 0;

	LINK_COM_VALID ;	
	if (0 == pLinkCom->nFrmLen)
	{
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
    
    //判断串口是否为GPRS
    if(pLinkCom->nCom == COMM_GPRS)
    {
        if (g_nGprsWatchFlg)
        {
            LinkComSend(COMM_WH485, &pLinkCom->sFrmBuf.pBuf[0], pLinkCom->nFrmLen);
        }
        Descend = 1;
    }

	pUpRcvLinkProt 	= (pGW376_1_PROT)(pLinkCom->sFrmBuf.pBuf);
	pRcvData		= &(pUpRcvLinkProt->nDadtData[0]);
    nRcvDataLen     = Buf2INT16U(&pUpRcvLinkProt->nLen1[0]);
	nRcvDataLen		= nRcvDataLen / 4 - 8;
	nAfn			= pUpRcvLinkProt->nAfn;
	nCtrl			= pUpRcvLinkProt->nCtrl;
	pSndData 		= &g_nGw3761SendBuf[0];
	nSndDataLen 	= GW3761_SENDBUF_SIZE - GW376_1_HEAD_TAIL_LEN;
	OSSemPend(g_pSem[SEM_GW3761_BUF_IDX], 0, &nErr);
	memset(pSndData, 0, GW3761_SENDBUF_SIZE);
    //打印
    nSendLen = Gw3761AsynProcSub(&pLinkCom->sFrmBuf.pBuf[0], pSndData, nSndDataLen);
    //pSndData = &g_nGw3761SendBuf[0];
	// 组帧发送
	//g_nAsynFlg = 1;
	//g_nTmpSelfArea = Buf2INT16U(&pUpRcvLinkProt->nTermAddr[0]);
	//g_nTmpSelfAddr = Buf2INT16U(&pUpRcvLinkProt->nTermAddr[2]);
	//nSendLen = Gw3761MakeFrm(nCtrl, nAfn, pSndData, nSendLen, GW3761_SENDBUF_SIZE);
	//g_nAsynFlg = 0;
	//g_nTmpSelfArea = 0xFFFF;
	//g_nTmpSelfAddr = 0xFFFF;
	if (nSendLen > 0)
	{
        //判断串口是否为GPRS
        if(pLinkCom->nCom == COMM_GPRS)
        {
            if (g_nGprsWatchFlg)
            {
                LinkComSend(COMM_WH485, pSndData, (INT32U)nSendLen);
            }
            Ascend = 1;
        }
		LinkComSend(pLinkCom->nCom, pSndData, (INT32U)nSendLen);
        
        if(1 == g_nFlgProt)
        {
            g_nFlgProt1 = 1;
        }
        g_nFlg_Commu2 = 1;
	}
	OSSemPost(g_pSem[SEM_GW3761_BUF_IDX]);
	
	// 处理结束后，清处理
	LINK_COM_FRAME_CLR ;
	
	return nRet;
}


/*
*****************************************************************************
* 函数名: ForwardMakeFrmSub
* 输　入: 
*		INT8U 			nPort							    穿透的端口
*		INT8U* 			pAddr								NULL时穿透本身
*		INT32U 			nBaud								波特率
*		INT8U 			nTimeOutSec							超时时间 单位秒
*		INT8U* 			pBuf								穿透报文缓冲区
*		INT32S 			nMaxDatalen
* 输  出: 
*		INT8U* 			pBuf								穿透报文缓冲区
* 返  回: 
* 功  能: 形成穿透帧
*****************************************************************************
*/
INT32S ForwardMakeFrmSub(INT8U nBoradFlg, INT8U nPort, INT8U* pAddr, INT32U nBaud, INT8U nTimeOutSec, INT8U* pBuf, INT32S nMaxDatalen)
{
    const INT32U	c_nBaud[] 		= {300, 600, 1200, 2400, 4800, 7200, 9600, 19200};
	INT16U 			nTimeOutDecms	= 100;							//字节超时间固定为1秒即100个10ms
	INT32S			nLen 			= 0;
	INT8U			i				= 0;
    
    if (NULL == pBuf)
	{
		return -1;
	}
    if (nMaxDatalen < ((INT32S)(6 + sizeof(g_nReadSelfBuf2))))					//port bps tos tos2 len 共6字节
	{
		return -1;
	}
    for (i = 0; i < ARR_SIZE(c_nBaud); i++)							//找到相应波特率
	{
		if (c_nBaud[i] == nBaud)
		{
			break;
		}
	}
    if (i >= ARR_SIZE(c_nBaud))
	{
		return -1;
	}
    pBuf[nLen++] = nPort;
	pBuf[nLen++] = ((i << 5) & 0xE0) + 0x0B;
	pBuf[nLen++] = nTimeOutSec | 0x80;
	pBuf[nLen++] = nTimeOutDecms;
    
    if (nBoradFlg)
    {
        INT16U2Buf(&pBuf[nLen], sizeof(g_nBroadTimeBuf));
        nLen += 2; 
        memcpy(&pBuf[nLen], &g_nBroadTimeBuf[0], sizeof(g_nBroadTimeBuf));
        nLen += sizeof(g_nBroadTimeBuf);
        memset(&pBuf[nLen],0,16);
        nLen += 16;  
    }
    else
    {
        INT16U2Buf(&pBuf[nLen], sizeof(g_nReadSelfBuf2));
        nLen += 2;
        memcpy(&pBuf[nLen], &g_nReadSelfBuf2[0], sizeof(g_nReadSelfBuf2));
        if (NULL != pAddr)
        {
            memcpy(&pBuf[nLen + 1], pAddr, 6);
        }
        pBuf[nLen + 14] = CheckSum(&pBuf[nLen], 0, 14);
        nLen += sizeof(g_nReadSelfBuf2);
        memset(&pBuf[nLen],0,16);
        nLen += 16;   
    }
	return nLen;
}


/*
*****************************************************************************
* 函数名: ForwardMakeFrmSub2
* 输　入: 
*		INT8U 			nPort							    穿透的端口
*		INT8U* 			pAddr								NULL时穿透本身
*		INT32U 			nBaud								波特率
*		INT8U 			nTimeOutSec							超时时间 单位秒
*		INT8U* 			pBuf								穿透报文缓冲区
*		INT32S 			nMaxDatalen
* 输  出: 
*		INT8U* 			pBuf								穿透报文缓冲区
* 返  回: 
* 功  能: 形成穿透帧
*****************************************************************************
*/
INT32S ForwardMakeFrmSub2(INT8U nProt, INT8U nPort, INT8U* pAddr, INT32U nBaud, INT8U nTimeOutSec, INT8U* pBuf, INT32S nMaxDatalen)
{
    const INT32U	c_nBaud[] 		= {300, 600, 1200, 2400, 4800, 7200, 9600, 19200};
	INT16U 			nTimeOutDecms	= 100;							//字节超时间固定为1秒即100个10ms
	INT32S			nLen 			= 0;
	INT8U			i				= 0;
    
    if (NULL == pBuf)
	{
		return -1;
	}
    if (nMaxDatalen < ((INT32S)(6 + sizeof(g_nReadSelfBuf2))))					//port bps tos tos2 len 共6字节
	{
		return -1;
	}
    for (i = 0; i < ARR_SIZE(c_nBaud); i++)							//找到相应波特率
	{
		if (c_nBaud[i] == nBaud)
		{
			break;
		}
	}
    if (i >= ARR_SIZE(c_nBaud))
	{
		return -1;
	}
    pBuf[nLen++] = nPort;
	pBuf[nLen++] = ((i << 5) & 0xE0) + 0x0B;
	pBuf[nLen++] = nTimeOutSec | 0x80;
	pBuf[nLen++] = nTimeOutDecms;
    
    if (GB_07_PROT == nProt)
    {
        INT16U2Buf(&pBuf[nLen], sizeof(g_nReadDayData));
        nLen += 2;
        memcpy(&pBuf[nLen], &g_nReadDayData[0], sizeof(g_nReadDayData));
        if (NULL != pAddr)
        {
            memcpy(&pBuf[nLen + 1], pAddr, 6);
        }
        pBuf[nLen + 14] = CheckSum(&pBuf[nLen], 0, 14);
        nLen += sizeof(g_nReadDayData);
        memset(&pBuf[nLen],0,16);
        nLen += 16;
    }
    else
    {
        if (NULL != pAddr)
        {    
            INT16U2Buf(&pBuf[nLen], sizeof(g_nRead97Buf));
            nLen += 2;
            memcpy(&pBuf[nLen], &g_nRead97Buf[0], sizeof(g_nRead97Buf));
            memcpy(&pBuf[nLen + 1], pAddr, 6);
            pBuf[nLen + 12] = CheckSum(&pBuf[nLen], 0, 12);
            nLen += sizeof(g_nRead97Buf);
            memset(&pBuf[nLen],0,16);
            nLen += 16;
        }
        else
        {
            INT16U2Buf(&pBuf[nLen], sizeof(g_nReadSelfBuf2));
            nLen += 2;
            memcpy(&pBuf[nLen], &g_nReadSelfBuf2[0], sizeof(g_nReadSelfBuf2));
            pBuf[nLen + 14] = CheckSum(&pBuf[nLen], 0, 14);
            nLen += sizeof(g_nReadSelfBuf2);
            memset(&pBuf[nLen],0,16);
            nLen += 16;
        }
    }
	return nLen;
}

/*
*****************************************************************************
* 函数名: Gw3761SelfForwardMakeFrm
* 输　入: 
*		INT8U 			nPort							    穿透的终端端口
*		INT8U* 			pData								穿透报文缓冲区
*		INT32S 			nMaxDatalen							缓冲区最大长度
* 输  出: 
*		INT8U* 			pData								穿透本身的报文
* 返  回: 穿透抄读本身的报文长度
* 功  能: 形成穿透本身的帧
*****************************************************************************
*/
INT32S Gw3761SelfForwardMakeFrm(INT8U nPort, INT8U* pBuf, INT32S nMaxDatalen)
{
    INT32S		nDatalen 	= 0;
	INT8U*		pData		= pBuf;
	INT8U		nTimeout	= 10;
    //INT32S      nBaud       = 0;
    
    //nBaud = g_nUartBpsArray[g_nSynBaseUartBPS];
	
	if (NULL == pBuf)
	{
		return -1;
	}
	if (nMaxDatalen < 4)
	{
		return -1;
	}
	GB3761PnFn2DADT(0, F1, pBuf);
	pBuf += 4;
    
    if (31 == nPort)
	{
		nTimeout = 90;
	}
    nDatalen = ForwardMakeFrmSub(0, nPort, NULL, 9600, nTimeout, pBuf, nMaxDatalen - 4);
    if (nDatalen < 0)
	{
		return -1;
	}
    return Gw3761MakeFrm(0x4B, 0x10, pData, nDatalen + 4, nMaxDatalen);
}

/*
*****************************************************************************
* 函数名: Gw3761ForwardMakeFrm
* 输　入: 
*		INT8U 			nPort							    穿透端口
*		INT8U* 			pData								穿透报文缓冲区
*		INT32S 			nMaxDatalen							缓冲区最大长度
* 输  出: 
*		INT8U* 			pData								穿透本身的报文
* 返  回: 穿透抄读本身的报文长度
* 功  能: 形成穿透本身的帧
*****************************************************************************
*/
INT32S Gw3761ForwardMakeFrm(INT8U nProt, INT8U nPort, INT8U* pAddr, INT32U nBaud, INT8U* pBuf, INT32S nMaxDatalen)
{
    INT32S		nDatalen = 0;
	INT8U*		pData	 = pBuf;
	
	if (NULL == pBuf)
	{
		return -1;
	}
	if (nMaxDatalen < 4)
	{
		return -1;
	}
	GB3761PnFn2DADT(0, F1, pBuf);
	pBuf += 4;
	nDatalen = ForwardMakeFrmSub2(nProt, nPort, pAddr, nBaud, 10, pBuf, nMaxDatalen - 4);
	
	return Gw3761MakeFrm(0x4B, 0x10, pData, nDatalen + 4, nMaxDatalen);   
}

/*
*****************************************************************************
* 函数名: Gw3761SelfForwardMakeFrm
* 输　入: 
*		
*		
*		
* 输  出: 
*		
* 返  回: 
* 功  能: 
*****************************************************************************
*/
INT32S GetTermAreaAddr(void)
{
    INT8U           nBuf[10]    = {0};
    
    memset(&nBuf[0], 0, sizeof(nBuf));
    MemReadCommPrm(F94, &nBuf[0], 4);
    g_nTmpSelfArea = Buf2INT16U(&nBuf[0]);
    g_nTmpSelfAddr = Buf2INT16U(&nBuf[2]);
    
    return 0;
    
}

//外扩出厂检测
/********************************************************************************
* 函数名: GB3761TermTest
* 输　入:
* 输　出:
* 返  回:
* 功  能: 内部检测
*********************************************************************************/
INT8S GB3761TermTest(pAPP_COMM pComm)
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
        if (RET_ERR_OK != GB3761TermTestProc(pComm, &nPn[0], nPnNum, &nFn[0], nFnNum, &nOffset))
        {
            break;
        }
	}
	return RET_ERR_OK;
}

/********************************************************************************
* 函数名: GB3761TermTestProc
* 输　入:
* 输　出:
* 返  回:
* 功  能: 内部测试
*********************************************************************************/
INT8S GB3761TermTestProc(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset)
{
    INT16U    	i = 0;
	INT16U    	j = 0;
	INT16U		nDataLen = 0;
	INT8U     	nData[5];
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
		for (j = 0; j < nFnNum; j++)
		{
            nDataLen = 0;                               //数据长度清零，每次都清零
            memset(&nData[0], 0, sizeof(nData));        //清空数据内容
			nDataLen += GB3761TermTestFnProc(pComm, pPn[i], pFn[j], pOffset, &nData[0], sizeof(nData));
            if (nDataLen > nMaxDataLen || 0 == nDataLen)   //一个Fn项数据长度超过帧长 丢弃
            {
                continue;
            }
            pInfo->nTxDataLen += MemCopy(&pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen], &nData[0], nDataLen);
		}
	}
	return RET_ERR_OK;
}

/********************************************************************************
* 函数名: GB3761TermTestFnProc
* 输　入:
* 输　出:
* 返  回: 数据长度
* 功  能: 内部测试
*********************************************************************************/
INT16U GB3761TermTestFnProc(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen)
{
    INT8S  nRet = RET_ERR_OK;
	INT8U  nReset = 0;
	INT16U nDataLen = 0;
    INT8U   nFlg    = 0;
    BOOLEAN bSetSuc = FALSE;
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
        case F1:  //硬件初始化
            nDataLen += GB3761TermTestF1(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset, &pData[nDataLen]);
            break;
        default:
        	pInfo->bNoDataFlag = TRUE;
            return 0;
    }
    if (nDataLen > 4)
    {
    	return nDataLen;
    }
    return 0;
}

//F1
INT32S GB3761TermTestF1(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset, INT8U* pBuf)
{
    INT8U    nFnLen     = 0;
	INT16U   nIndex     = 0;
    INT8U    nCmd       = 0;
    INT8U    nLen       = 0;
    
    if (NULL == pData || NULL == pOffset)
	{
		return -1;
	}
    nIndex = *pOffset;
    
    nCmd = pData[nIndex];
    pBuf[nLen++] = 1;
    if (nCmd == 1)              //启动自检
    {
        g_nTestStep = 0;
        memset(&g_nErrBuf[0], 0, 2);
        g_nAutoTestFlg = 1;
         g_nAutoTestFlg1 = 1;
    }
    else if (nCmd == 2)         //查询状态命令
    {
        pBuf[nLen++] = g_nTestStep;
        memcpy(&pBuf[nLen], &g_nErrBuf[0], 2);
        nLen += 2;
    }
    else if (nCmd == 3)         //置高输出口线
    {
        SetIoStatus();
        g_nIoSetFlg = 1;
    }
    else if (nCmd == 4)         //置低输出口线
    {
        ClrIoStatus();
        g_nIoSetFlg = 2;
    }
    else if (nCmd == 5)         //读取输出口线
    {
        RdOutIoStatus();
        memcpy(&pBuf[nLen],&g_nIoBuf[0], 3);
        nLen += 3;
    }   
    else if (nCmd == 6)         //读取输入口线
    {
        RdInIoStatus();
        memcpy(&pBuf[nLen],&g_nIoBuf[0], 3);    
        nLen += 3;
    }
    else if (nCmd == 7)         //使脉冲处于接收状态、清空原有值
    {
        
    }
    else if (nCmd == 8)         //读取脉冲
    {
        if (g_nPlusCnt >= 2)
        {
            g_nPlusCnt = 0;
        }
        g_nPlusCnt++;
        PlusProc();
        nLen += 1;
    }

    else if (nCmd == 9)
    {
        g_nKeyValue = 0;
        g_nKeyTestFlg = 1;
    }
    
    else if (nCmd == 0x10)
    {
        pBuf[nLen++] = g_nKeyValue;
    }
    return nLen;
}


//终端复位
/********************************************************************************
* 函数名: GB3761TermReset
* 输　入:
* 输　出:
* 返  回:
* 功  能: 终端复位
*********************************************************************************/
INT8S GB3761TermReset(pAPP_COMM pComm)
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
        if (RET_ERR_OK != GB3761TermResetProc(pComm, &nPn[0], nPnNum, &nFn[0], nFnNum, &nOffset))
        {
            break;
        }
	}
	return RET_ERR_OK;
}

/********************************************************************************
* 函数名: GB3761TermResetProc
* 输　入:
* 输　出:
* 返  回:
* 功  能: 终端复位
*********************************************************************************/
INT8S GB3761TermResetProc(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset)
{
    INT16U    	i = 0;
	INT16U    	j = 0;
	INT16U		nDataLen = 0;
	INT8U     	nData[5];
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
		for (j = 0; j < nFnNum; j++)
		{
            nDataLen = 0;                               //数据长度清零，每次都清零
            memset(&nData[0], 0, sizeof(nData));        //清空数据内容
			nDataLen += GB3761TermResetFnProc(pComm, pPn[i], pFn[j], pOffset, &nData[0], sizeof(nData));
            if (nDataLen > nMaxDataLen || 0 == nDataLen)   //一个Fn项数据长度超过帧长 丢弃
            {
                continue;
            }
            pInfo->nTxDataLen += MemCopy(&pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen], &nData[0], nDataLen);
		}
	}
	return RET_ERR_OK;
}

/********************************************************************************
* 函数名: GB3761TermResetFnProc
* 输　入:
* 输　出:
* 返  回: 数据长度
* 功  能: 终端复位
*********************************************************************************/
INT16U GB3761TermResetFnProc(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen)
{
    INT8S  nRet = RET_ERR_OK;
	INT8U  nReset = 0;
	INT16U nDataLen = 0;
    INT8U   nFlg    = 0;
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
        case F1:  //硬件初始化
            nFlg = 1;
            MemWriteDbfExt(MEM_OBJ_X_RAM_DATA, 0, F5, &nFlg, 1, NULL);
            break;
        case F2:  //数据初始化
            //MemDataInit();
            g_nInitType = MEM_INIT_DATA;
            break;
        case F3:  //出厂初始化
            //MemFactoryInit();
            g_nInitType = MEM_INIT_FACTORY;
            break;
        case F4:  //参数初始化
            //MemPartPrmInit();
            g_nInitType = MEM_INIT_PART_PRM;
            break;
        default:
        	pInfo->bNoDataFlag = TRUE;
            return 0;
    }
    if (RET_ERR_OK == nRet)
    {
    	pInfo->bSucFlag = TRUE;
    	pData[nDataLen++] = 0;
	}
	else
	{
		pInfo->bFailFlag = TRUE;
    	pData[nDataLen++] = 1;
	}
	return nDataLen;
}

/********************************************************************************
* 函数名: GB3761InitTxFrm
* 输　入:
* 输　出:
* 返  回:
* 功  能:
*********************************************************************************/
INT8S GB3761InitTxFrm(pAPP_COMM pComm)
{
    pGB3761_FRM_INFO pInfo  = NULL;
	pGB3761_FRM_MAP  pTxMap = NULL;

	if (NULL == pComm)
    {
    	return RET_ERR_POINT;
    }
	pInfo  = (pGB3761_FRM_INFO)pComm->pFrmInfo;
	pTxMap = (pGB3761_FRM_MAP)&pComm->pTxFrmBuf[0];
	memset(pTxMap, 0, sizeof(GB3761_FRM_MAP));
    memset(pInfo, 0, sizeof(GB3761_FRM_INFO));
    pInfo->bAllMpFlag = FALSE;
    pInfo->bSendFlag  = FALSE;
    pInfo->bFirstFlag = TRUE;
    pInfo->bSucFlag   = FALSE;
    pInfo->bFailFlag  = FALSE;
    pInfo->bNoDataFlag  = FALSE;

    return RET_ERR_OK;
}



/****************************************************************************
* 函数名:GB3761CheckTp
* 输　入: pRxInfo 发送帧信息
* 输　出: pRxInfo 发送帧信息
* 返  回: 接口标准返回值
* 功  能: 报文时效性比对
****************************************************************************/
INT8S GB3761CheckTp(INT8U nDelay, pSTD_TIME pTime)
{
    STD_TIME sSysTime;
    pSTD_TIME pSysTime = &sSysTime;
    INT32S nDelta = 0;

    if (NULL == pTime)
    {
        return RET_ERR_POINT;
    }
    memset(pSysTime, 0, sizeof(STD_TIME));

    if (RET_ERR_OK != GetSysClk(pSysTime))     //从终端获取时间
    {
        return RET_ERR_ERROR;
    }
    pTime->nMon = pSysTime->nMon;
    pTime->nYear = pSysTime->nYear;
    if (!IsVldStdTime(pTime))
    {
        return RET_ERR_ERROR;
    }
    if (RET_ERR_OK != GetDiffTime3(pSysTime, pTime, &nDelta))
    {
        return RET_ERR_ERROR;
    }
    if (abs(nDelta) > (nDelay * 60))
    {
        return RET_ERR_ERROR;
    }
    return RET_ERR_OK;
}

/********************************************************************************
* 函数名: GB3761ChkRxFrm
* 输　入:
* 输　出:
* 返  回:
* 功  能:
*********************************************************************************/
INT8S GB3761ChkRxFrm(pAPP_COMM pComm)
{
    INT8U   i      = 0;
	INT8U   nGrpNo = 0;
	INT16U  nLen   = 0;
	INT8U	nArea[LEN_AREA_CODE];
    INT8U	nAddr[LEN_TERM_ADDR];
	INT8U   nGroupAddr[MAX_TERM_GRP_NUM][LEN_TERM_ADDR];
    STD_TIME sTime;
    INT8U   nPwd[GB3761_PWD_LEN];
	pGB3761_FRM_MAP  pRxMap  = NULL;
	pGB3761_FRM_INFO pInfo = NULL;
    INT8U nKeyPlanPrm[LEN_KEY_PLAN_PRM];
    INT8U nLow2Pwd[LEN_KEY_PLAN_PRM];           //16个字节密码域取低两个字节
    INT8U nSrcData[LEN_KEY_DATA_PRM];           //自3761帧数据域取12个字节
    INT8U nDstPwd[LEN_KEY_PLAN_PRM];
    
    if (NULL == pComm)
    {
    	return RET_ERR_POINT;
    }
    
    pRxMap = (pGB3761_FRM_MAP)pComm->pRxFrmBuf;
	pInfo  = (pGB3761_FRM_INFO)pComm->pFrmInfo;
    
    //通讯方向判断
    if (0 != pRxMap->uCtrl.sDet.nDir)
    {
    	return RET_ERR_ERROR;
    }
    
    //地址判断
    if (g_GroadAddrFlg)
    {
        if (GB3761_AFN_TRANS_FILE == pRxMap->nAfn)
        {
                g_nFileGroadFlg = 1;
        }
    }
    
    nLen = pRxMap->uLen1.sDet.nL1 + (pRxMap->nLen1 << 6);
    //时间标签
    //时间标签
	if (1 == pRxMap->uSEQ.sDet.nTpv)
    {
        if (nLen < GB3761_TP_LEN)
		{
			return RET_ERR_ERROR;
		}
        nLen -= GB3761_TP_LEN;
		memset(&sTime, 0, sizeof(STD_TIME));
        pInfo->nPFC   = pComm->pRxFrmBuf[GB3761_HEAD_LEN + nLen];
        memcpy(&sTime, &pComm->pRxFrmBuf[GB3761_HEAD_LEN + nLen + 1], 4);
        pInfo->nDelay = pComm->pRxFrmBuf[GB3761_HEAD_LEN + nLen + 5];
        //判断是不是对时
        if (0x0 != pInfo->nDelay)  //判断报文时效性
        {
            if ((GB3761_AFN_SET_PARAM == pRxMap->nAfn)
	           || (GB3761_AFN_RESET == pRxMap->nAfn)
	           || (GB3761_AFN_CTRL == pRxMap->nAfn))
            {
                if ((GB3761_AFN_CTRL != pRxMap->nAfn)
					|| (0 != pComm->pRxFrmBuf[GB3761_DATA_IDX])
					|| (0 != pComm->pRxFrmBuf[GB3761_DATA_IDX + 1])
					|| (0x40 != pComm->pRxFrmBuf[GB3761_DATA_IDX + 2]) //对时命令不判断時标
					|| (3 != pComm->pRxFrmBuf[GB3761_DATA_IDX + 3]))
                {
                    //if (RET_ERR_OK != GB3761CheckTp(pInfo->nDelay, &sTime))
					{
					    //return RET_ERR_ERROR;
					}
                }
            }
        }
    }
    
    //密码域
    if ((GB3761_AFN_RESET == pRxMap->nAfn)
	    || (GB3761_AFN_SET_PARAM == pRxMap->nAfn)
        || (GB3761_AFN_SET_PRM_EXT == pRxMap->nAfn)
	    || (GB3761_AFN_CTRL == pRxMap->nAfn)
        || (GB3761_AFN_CHECK_PWD == pRxMap->nAfn)
	    || (GB3761_AFN_TRANS_FILE == pRxMap->nAfn)
	    || (GB3761_AFN_TRANS_DATA == pRxMap->nAfn))//是否是带密码的AFN
    {
        if (nLen < GB3761_PWD_LEN)
		{
			return RET_ERR_ERROR;
		}
        nLen -= GB3761_PWD_LEN;
		memset(&nPwd[0], 0, sizeof(nPwd));
        memcpy(&nPwd[0], &pComm->pRxFrmBuf[GB3761_HEAD_LEN + nLen], GB3761_PWD_LEN);
    }
    if (nLen < 8)     //控制 地址 Afn SEQ
	{
		return RET_ERR_ERROR;
	}
    nLen -= 8;
	pInfo->nRxDataLen = nLen;
    return RET_ERR_OK;
}

INT8S GB3761AlyRxFrm(pAPP_COMM pComm)
{
    INT8S               nRet        = RET_ERR_ERROR;
	INT16U              nDataLen    = 0;
    INT8U               nLen        = 0;
    INT8U               nPW[GB3761_PWD_LEN];
    pGB3761_FRM_MAP     pMap        = NULL;
	pGB3761_FRM_INFO    pInfo       = NULL;
    
    if (NULL == pComm)
	{
		return RET_ERR_POINT;
	}
    pMap    = (pGB3761_FRM_MAP)&pComm->pRxFrmBuf[0];
	pInfo   = (pGB3761_FRM_INFO)pComm->pFrmInfo;

    switch (pMap->nAfn)
    {
        case GB3761_AFN_TEST_EXT:                     //内部测试
            nRet = GB3761TermTest(pComm);
            break;
        case GB3761_AFN_RESET:                        //终端复位
            nRet = GB3761TermReset(pComm);
            break;
        case GB3761_AFN_SET_PARAM:                    //参数设置
            nRet = GB3761SetParam(pComm);
            break;
        case GB3761_AFN_CTRL:                         //控制命令
            nRet = GB3761CtrlCmd(pComm);
            break;
        case GB3761_AFN_QUERY_PARAM:                  //参数查询
            nRet = GB3761GetParam(pComm);
            break;
        case GB3761_AFN_REQ_CLASS_I:                  //一类数据查询
            nRet = GB3761GetClassI(pComm);
            break;
        case GB3761_AFN_REQ_CLASS_II:                 //二类数据查询
            nRet = GB3761GetClassII(pComm);
            break;
        case GB3761_AFN_TRANS_FILE:                   //文件传输
            nRet = GB3761FileTrans(pComm);
            if (g_nFileGroadFlg)
            {
                nRet = -1;
            }
            break;
        case GB3761_AFN_TRANS_DATA:                   //数据转发
            nRet = GB3761TransData(pComm);
            break;
        case GB3761_AFN_SET_PRM_EXT:                 //扩展参数设置
            nRet = GB3761SetParamExt(pComm);
            break;
        case GB3761_AFN_QUERY_PRM_EXT:                 //扩展参数查询
            nRet = GB3761GetParamExt(pComm);
            break;
        default:
            nRet = RET_ERR_ERROR;
            break;
    }
    return nRet;
}

/****************************************************************************
* 函数名:GB3761CheckDenyAck
* 输　入:bSuc     有成功设置
         bFail    有失败设置
         bNoData  有不存在Fn
* 输　出:
* 返  回:接口标准返回值
* 功  能:根据设置结果返回确认否认部分确认
****************************************************************************/
INT8S GB3761CheckDenyAck(const BOOLEAN bSuc, const BOOLEAN bFail, const BOOLEAN bNoData)
{
    if (bSuc)
    {
        if (!bFail)
        {
            return GB3761_RESP_ACK;
        }
        else
        {
            return GB3761_RESP_PART;
        }
    }
    else
    {
        if (bNoData)
        {
            return GB3761_RESP_NO_DATA;
        }
        else
        {
            return GB3761_RESP_DENY;
        }
    }
}

/********************************************************************************
* 函数名:GB3761GetUpLfn
* 输　入:nAfn  应用层功能码
         nAckType  帧类型
* 输　出:
* 返  回:链路层功能码
* 功  能:根据应用层功能码和响应帧类型 获取 上行链路层功能码
*********************************************************************************/
INT8S GB3761GetUpLfn(INT8U nAfn, INT8U nAckType)
{
    INT8S nRet = GB3761_LFN_RESP_NULL;

    //if ((nAfn > 0x10) || (0x07 == nAfn))           //超出AFN范围
    {
        //return RET_ERR_ERROR;
    }
    switch (nAckType)
    {
        case GB3761_RESP_ACK:
        case GB3761_RESP_DENY:
        case GB3761_RESP_PART:
            if (GB3761_AFN_RESET == nAfn)
            {
                nRet = GB3761_LFN_ACK;
            }
            else if (GB3761_AFN_LINK == nAfn)
            {
                nRet = GB3761_LFN_RESP_LINK;
            }
            else
            {
                nRet = GB3761_LFN_RESP_DATA;
            }
            break;
        case GB3761_RESP_NO_DATA:
            nRet = GB3761_LFN_RESP_DENY;
            break;
        case GB3761_RESP_DATA:
            nRet = GB3761_LFN_RESP_DATA;
            break;
        case GB3761_RESP_PWD_DENY:
            nRet = GB3761_LFN_RESP_DATA;
            break;
        default:
        	nRet = GB3761_LFN_RESP_NULL;
            break;
    }
    return nRet;
}

/****************************************************************************
* 函数名: GB3761FillTxFrm
* 输　入: pRxInfo  帧信息
          FrmType     帧类型
* 输　出: pFrmInfoTx  帧信息
* 返  回: 接口标准返回值
* 功  能: 发送前填充帧信息
****************************************************************************/
INT8S GB3761FillTxFrm(pAPP_COMM pComm)
{
    INT8S   nRet = 0;
    BOOLEAN bACDFlag = FALSE;
    STD_TIME sTime;
    pSTD_TIME  pTime = &sTime;
    pGB3761_FRM_INFO    pInfo  = NULL;
    pGB3761_FRM_MAP     pRxMap = NULL;
    pGB3761_FRM_MAP     pTxMap = NULL;
    
    if (NULL == pComm)
    {
    	return RET_ERR_POINT;
    }
    
    pInfo  = (pGB3761_FRM_INFO)pComm->pFrmInfo;
    pRxMap = (pGB3761_FRM_MAP)&pComm->pRxFrmBuf[0];
    pTxMap = (pGB3761_FRM_MAP)&pComm->pTxFrmBuf[0];
    
    if (pInfo->bFirstFlag)
    {
        pInfo->bFirstFlag = FALSE;
        
        //初始化SEQ帧类型为单帧
    	pTxMap->uSEQ.sDet.nFir = 1;
    	pTxMap->uSEQ.sDet.nFin = 1;
        
        pTxMap->nHead2 = 0x68;   //报文头
    	pTxMap->nHead1 = 0x68;
    	pTxMap->uLen1.sDet.nProtId = pRxMap->uLen1.sDet.nProtId;//协议
    	pTxMap->uLen2.sDet.nProtId = pRxMap->uLen2.sDet.nProtId;
        
        if (!pRxMap->uCtrl.sDet.nDir)      //传输方向位 下行置上行
	    {
	        pTxMap->uCtrl.sDet.nDir = 1;
	    }
        
        memcpy(&pTxMap->nArea[0], &pRxMap->nArea[0], LEN_AREA_CODE);//行政区码
	    memcpy(&pTxMap->nAddr[0], &pRxMap->nAddr[0], LEN_TERM_ADDR);//终端地址
        
        pTxMap->uA3.sDet.nMSA = pRxMap->uA3.sDet.nMSA;         //MSA
	    //SEQ
    	pTxMap->uSEQ.sDet.nPRSeq = pRxMap->uSEQ.sDet.nPRSeq;
        
        //确认否认帧
		if (GB3761_AFN_SET_PARAM == pRxMap->nAfn
			|| GB3761_AFN_CTRL == pRxMap->nAfn
			|| GB3761_AFN_RESET == pRxMap->nAfn
            || GB3761_AFN_SET_PRM_EXT == pRxMap->nAfn
            || pInfo->nAckType > 0
			|| 0 == pInfo->nTxDataLen)
        {
            pTxMap->nAfn = GB3761_AFN_ACK;             //置确认否认AFN
            if (0 == pInfo->nAckType)
            {
                pInfo->nAckType = GB3761CheckDenyAck(pInfo->bSucFlag, pInfo->bFailFlag, pInfo->bNoDataFlag);
            }
            nRet = GB3761GetUpLfn(pRxMap->nAfn, pInfo->nAckType); //获取链路功能码
            if (GB3761_LFN_RESP_NULL == nRet)
		    {
		        return RET_ERR_ERROR;
		    }
            pTxMap->uCtrl.sDet.nFun = nRet;
            if (GB3761_RESP_NO_DATA == pInfo->nAckType) //无所召唤数据 Fn转换
		    {
		        pInfo->nAckType = GB3761_RESP_DENY;
		    }
            pComm->pTxFrmBuf[GB3761_DATA_IDX] = 0x0;                //置P0
		    pComm->pTxFrmBuf[GB3761_DATA_IDX + 1] = 0x0;
		    pComm->pTxFrmBuf[GB3761_DATA_IDX + 2] = pInfo->nAckType;//置Fn
		    pComm->pTxFrmBuf[GB3761_DATA_IDX + 3] = 0x0;
            if (GB3761_RESP_PART == pInfo->nAckType)
            {
                pComm->pTxFrmBuf[GB3761_DATA_IDX + 4] = pRxMap->nAfn;
            }
            if ((pInfo->nAckType != GB3761_RESP_PART) && (pInfo->nAckType != GB3761_RESP_PWD_DENY)) //不是部分确认否认报文 和 身份认证否认报文
		    {
		        pInfo->nTxDataLen = 4;  //修正报文长度DADT
		    }
            pInfo->nAckType = 0;//清除标志
        }
        //回复数据
		else
		{
            pTxMap->nAfn = pRxMap->nAfn;//置AFN
            pTxMap->uCtrl.sDet.nFun = GB3761_LFN_RESP_DATA; //置控制码 上行响应帧链路功能码
		}
    }
    else
	{
		pTxMap->uSEQ.sDet.nPRSeq++;
	}
    if (pTxMap->uSEQ.sDet.nFir != pTxMap->uSEQ.sDet.nFin)
    {
    	pTxMap->uSEQ.sDet.nFir = 0;
    }
	if (pInfo->bSendFlag) //内部发送
    {
        pInfo->bSendFlag = FALSE; //发送后清除
        pTxMap->uSEQ.sDet.nFin = 0;
    }
    else
    {
        pTxMap->uSEQ.sDet.nFin = 1;
    }
    return RET_ERR_OK;
}

/****************************************************************************
* 函数名: GB3761MakeFrm
* 输　入: pFrmInfoTx 发送帧信息
* 输　出: pBuf 发送帧信息
* 返  回: 接口标准返回值
* 功  能: 根据发送帧信息中的内容组装成一帧报文
****************************************************************************/
INT8S GB3761MakeFrmTmp(INT8U *pFrm, INT16U nFrmLen)
{
    INT8U  nCS = 0;
    INT16U nTmpLen = 0;
    pGB3761_FRM_MAP pMap = NULL;

    if (NULL == pFrm)
    {
        return RET_ERR_POINT;
    }
    if (nFrmLen < GB3761_MIN_FRM_LEN)  //最短帧长
    {
        return RET_ERR_ERROR;
    }
    pMap = (pGB3761_FRM_MAP)pFrm;
    nTmpLen = nFrmLen - GB3761_CTRL_IDX - GB3761_TAIL_LEN;//减去头和尾
    nCS = CheckSum(pFrm, GB3761_CTRL_IDX, nTmpLen);
    pMap->uLen1.sDet.nL1 = nTmpLen;
    pMap->nLen1 = (nTmpLen>>6) & 0x00FF;
    pMap->uLen2.sDet.nL2 = nTmpLen;
    pMap->nLen2 = (nTmpLen>>6) & 0x00FF;
    //填充帧尾
    pFrm[nFrmLen - 2] = nCS;
    pFrm[nFrmLen - 1] = 0x16;

    return RET_ERR_OK;
}

INT32S Gw3761AsynProcSub(INT8U* pRcv, INT8U* pSnd, INT32S nSndlen)
{
    INT8U               nFrmInfo[30]   = {0};
    INT8U*              pFrmInfo        =&nFrmInfo[0];
    INT16U              nFrmLen         = 0;  
    pGB3761_FRM_INFO    pInfo           = NULL;
    APP_COMM            sTmpAppComm;
    pAPP_COMM           pTmpAppComm     = &sTmpAppComm;
    INT8S               nRet            = RET_ERR_ERROR;

    pTmpAppComm->pRxFrmBuf = pRcv;
    pTmpAppComm->pTxFrmBuf = pSnd;
    pTmpAppComm->pFrmInfo  = pFrmInfo;
    
    pInfo  = (pGB3761_FRM_INFO)pTmpAppComm->pFrmInfo;   //获取分析帧信息的指针
    
    pTmpAppComm->nFrmLen    = 1024;
    memset(pInfo, 0, sizeof(GB3761_FRM_INFO));
    pInfo->bAllMpFlag       = FALSE;
    pInfo->bSendFlag        = FALSE;
    pInfo->bFirstFlag       = TRUE;
    pInfo->bSucFlag         = FALSE;
    pInfo->bFailFlag        = FALSE;
    pInfo->bNoDataFlag      = FALSE;
    
    if (RET_ERR_OK != (nRet = GB3761ChkRxFrm(pTmpAppComm)))
	{
		return nRet;
	}
    if (RET_ERR_OK != (nRet = GB3761AlyRxFrm(pTmpAppComm)))
	{
		return nRet;
	}
    if (RET_ERR_OK != (nRet = GB3761FillTxFrm(pTmpAppComm)))
	{
		return nRet;
	}
    
    nFrmLen = GB3761_DATA_IDX + pInfo->nTxDataLen + GB3761_TAIL_LEN;
    GB3761MakeFrmTmp(&pTmpAppComm->pTxFrmBuf[0], nFrmLen);
    
    return nFrmLen;
}































