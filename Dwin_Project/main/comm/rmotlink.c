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
#include "rmotlink.h"
#include "AppCfg.h"
#include "Version.h"
#include "Base.h"
#include "SysRes.h"
#include "linkcom.h"
#include "linkcomExt.h"
#include "Guiwin.h"
#include "Guimenu.h"
#include "key.h"
#include "Fun.h"
#include "TimerCount.h"
#include "SoftTimer.h"
#include "Gw3761.h"


/**************************************************************************************************
                                           宏
***************************************************************************************************/



/**************************************************************************************************
                                           变量
***************************************************************************************************/
// LINK_CLNT，客户机；LINK_SRVC，服务器
static INT8U				g_nLinkType = 0;		
// 客户机
static INT8U				g_nClntCommBuf[1440] 		= {0};			
static INT8U				g_nClntCommBuf2[1440] 		= {0};	

// 服务器
static INT8U				g_nSrvcCommBuf[1440] 		= {0};
static INT8U				g_nSrvcCommBuf2[1440] 		= {0};

// rmot全局缓冲:所有数据都要显存放到此缓冲
static INT8U				g_nTotalCommBuf[2500] 	= {0};




// 下面两个BASE_BUF不能改动
static BASE_BUF				g_sClntBaseBuf 			= {
		&g_nClntCommBuf[0],
		sizeof(g_nClntCommBuf),
		0,
		0
};
static BASE_BUF				g_sSrvcBaseBuf 			= {
		&g_nSrvcCommBuf[0],
		sizeof(g_nSrvcCommBuf),
		0,
		0
};
static BASE_BUF				g_sTotalBaseBuf 			= {
		&g_nTotalCommBuf[0],
		sizeof(g_nTotalCommBuf),
		0,
		0
};

static BASE_BUF				g_sClntBaseBuf2 			= {
		&g_nClntCommBuf2[0],
		sizeof(g_nClntCommBuf2),
		0,
		0
};
static BASE_BUF				g_sSrvcBaseBuf2 			= {
		&g_nSrvcCommBuf2[0],
		sizeof(g_nSrvcCommBuf2),
		0,
		0
};





static INT32S				g_nTotalRmotBufPtr	= 0;
static INT32S				g_nTotalRmotBufLen	= 0;



// 默认通信参数
static RMOT_PARM			g_sRomtParm 			= {
				 		"jsepc.js"//"cmnet"                            //apn
						,{10, 101, 1, 185}               //主ip
						,5600//,5200
						,{10, 101, 1, 187}               //备ip
						,5600//,5200
						,{0, 0, 0, 0}               		//终端ip
                        ,{0, 0, 0, 0}                       //子网掩码
                        ,{0, 0, 0, 0}                       //网关
						,5100
						,""									//用户名
						,""

						,0									//Tcp
                        ,1									//混合
                        ,1                                  //永久在线
						,600								//重拨
						,180								//心跳
						,120								//无通信掉线
						,30									//超时
};
pRMOT_PARM					g_pRomtParm 			= &g_sRomtParm;
// 中间变量
static RMOT_VAL_CLR			g_sRmotValClr;
pRMOT_VAL_CLR				g_pRmotValClr			= &g_sRmotValClr;

static INT8U				g_nModlType			= MODL_TELIT;//模块类型
INT16U						g_nLoginCnt			= 0;	//拨号次数


// 模块自适应波特率
static const INT32U    		g_dwBaudRate[] = {
						UART_BPS_9600,
						UART_BPS_57600,			
};



//模块所用的通用AT返回码
const 	char 	g_Modl_OK[]           = "NJ\x0c\x09";					//OK\x0D\x0A
const 	char 	g_Modl_ERROR[]        = "DQQNQ";					    //ERROR
const   char    g_Modl_CONNECT[]      = "BNMMDBS";			        	//"CONNECT"
const 	char 	g_Modl_0D0A[]         = "\x0c\x09";				    //\x0D\x0A
const 	char 	g_Modl_TELIT[]        = "221";						    //Telit
const 	char 	g_Modl_NEO[]          = "MDNV@X";					    //NEOWAY


/**************************************************************************************************
                                           声明
***************************************************************************************************/
extern INT32S TelitProc(pBASE_BUF pBaseBuf, pBASE_BUF pBaseBuf1,pBASE_BUF pBaseBuf2,INT8U nLinkType);
extern void TltTcpSend(INT8U nId, INT8U* pData, INT32S nSize);
extern INT8U  nMessageFlg;

/**************************************************************************************************
                                           相关函数实现
***************************************************************************************************/
#if 0
	/*
		1、远程通讯先把数据接收到g_sTotalBaseBuf缓冲里
	*/
#endif
/*
*****************************************************************************
* 函数名: BaseBufRecv
* 输　入: 
*			INT8U*		pBuf				缓冲区
*			INT32S		nLen				长度
* 输  出: 
* 返  回: 
*		
*		
*		
* 功  能: 把pBuf的数据存入pBaseBuf
*****************************************************************************
*/
static INT32S BaseBufRecv(pBASE_BUF pBaseBuf, INT8U* pBuf, INT32S nLen)
{
	INT32S	i = 0;

	if (NULL == pBaseBuf)
	{
		return -1;
	}
	if (NULL == pBuf)
	{
		return -1;
	}
	if (nLen <= 0)
	{
		return -1;
	}
	
	for (i = 0; i < nLen; i++)
	{
		if (BASEBUF_RECV_PTR(1) == pBaseBuf->nProcPtr)
		{
			pBaseBuf->nProcPtr = BASEBUF_PROC_PTR(1);							// 处理指针向后移动1位
		}
		BASEBUF_RECV_PBUF(0) = pBuf[i]; 							// (基于接收指针的)缓冲区数据接收
		
		pBaseBuf->nRecvPtr = BASEBUF_RECV_PTR(1);								// 接收指针向后移动1位
	}

	return 0;
}

/*
*****************************************************************************
* 函数名: RmotTotalBufRecv
* 输　入: 
*			INT8U*		pBuf				缓冲区
*			INT32S		nRdLen				读取长度
* 输  出: 
* 返  回: 
*		
*		
*		
* 功  能: 
*****************************************************************************
*/
static INT32S RmotTotalBufRecv(pBASE_BUF pBaseBuf)
{
	INT32S		i			= 0;
	INT8U		nTmpBuf[MAX_RECV_ONCE_SIZE] = {0};
	INT32S		nRcvLen 	= 0;
	INT32S		nRet		= 0;

	if (NULL == pBaseBuf)
	{
		return -1;
	}
	for (i = 0; i < (pBaseBuf->nBufSize / MAX_RECV_ONCE_SIZE) + 1; i++)
	{
		memset(&nTmpBuf[0], 0, MAX_RECV_ONCE_SIZE);
		nRcvLen = ExtReadCommMs(COMM_GPRS, &nTmpBuf[0], MAX_RECV_ONCE_SIZE, 20, 100);
		if (nRcvLen <= 0)
		{   
            if (nRet > 0)
            {
                OSTimeDlyHMSM(0, 0, 0, 150);
                memset(&nTmpBuf[0], 0, MAX_RECV_ONCE_SIZE);
                nRcvLen = ExtReadCommMs(COMM_GPRS, &nTmpBuf[0], MAX_RECV_ONCE_SIZE, 20, 100);
                if (nRcvLen <= 0)
                {
                    break;
                }
            }
            else
            {
                break;
            }
		}
        if (GetRmotCmdPrintFlg())
        {
            Gw3761FrmWrite(COMM_WH232, &nTmpBuf[0], nRcvLen);
        }
		BaseBufRecv(pBaseBuf, &nTmpBuf[0], nRcvLen);
		nRet += nRcvLen;

		if (nRet >= pBaseBuf->nBufSize)
		{
			break;
		}
	}

	return nRet;
}

/*
*****************************************************************************
* 函数名: UseCurrBaseBuf
* 输　入: 
*			pBASE_BUF		pBaseBuf				g_sLinkCom变量的BASE_BUF
*			INT8U			nLinkType				LINK_CLNT，客户机；LINK_SRVC，服务器
* 输  出: 
* 返  回: 
*		
*		
*		
* 功  能: 选择当前使用的BASE_BUF
*****************************************************************************
*/
static INT32S	UseCurrBaseBuf(pBASE_BUF pBaseBuf, INT8U nLinkType)
{
	if (NULL == pBaseBuf)
	{
		return -1;
	}
	if (LINK_CLNT == nLinkType)
	{
		memcpy(pBaseBuf, &g_sClntBaseBuf, sizeof(BASE_BUF));
	}
	else
	{
		memcpy(pBaseBuf, &g_sSrvcBaseBuf, sizeof(BASE_BUF));	
	}

	return 0;
}

/*
*****************************************************************************
* 函数名: SaveCurrBaseBuf
* 输　入: 
*			pBASE_BUF		pBaseBuf				g_sLinkCom变量的BASE_BUF
* 输  出: 
* 返  回: 
*		
*		
*		
* 功  能: 保存当前使用的BASE_BUF
*****************************************************************************
*/
static INT32S	SaveCurrBaseBuf(pBASE_BUF pBaseBuf)
{
	if (NULL == pBaseBuf)
	{
		return -1;
	}
	if ((INT32U)(pBaseBuf->pBuf) == (INT32U)(g_sClntBaseBuf.pBuf))
	{
		g_sClntBaseBuf.nProcPtr = pBaseBuf->nProcPtr;
		g_sClntBaseBuf.nRecvPtr = pBaseBuf->nRecvPtr;
	}
	else
	{
		g_sSrvcBaseBuf.nProcPtr = pBaseBuf->nProcPtr;
		g_sSrvcBaseBuf.nRecvPtr = pBaseBuf->nRecvPtr;
	}
	return 0;
}

/*
*****************************************************************************
* 函数名: GetTotalBaseBufPtr
* 输　入: 
*			pBASE_BUF		pBaseBuf				g_sLinkCom变量的BASE_BUF
*			INT8U			nLinkType				LINK_CLNT，客户机；LINK_SRVC，服务器
* 输  出: 
* 返  回: 
*		
*		
*		
* 功  能: 选择当前使用的BASE_BUF
*****************************************************************************
*/
INT32S	GetTotalBaseBufPtr(INT32U* pProcPtr, INT32U* pRecvPtr)
{
	if (NULL == pProcPtr)
	{
		return -1;
	}
	if (NULL == pRecvPtr)
	{
		return -1;
	}
	*pProcPtr = g_sTotalBaseBuf.nProcPtr;
	*pRecvPtr = g_sTotalBaseBuf.nRecvPtr;
	return 0;
}




#if 0
/*
	远程参数
*/
#endif
/*
*****************************************************************************
* 函数名: RmotParamTrans
* 输　入: 
* 输  出: 
* 返  回: 
*		
*		
*		
* 功  能: 把规约层的缓冲内容转换为远程通讯参数  ---->  g_pRomtParm
*****************************************************************************
*/
INT32S RmotParamTrans(void)
{	
	pRMOT_PARM 	pRmotParm 	= g_pRomtParm;
	INT8U		nErr		= 0;
	INT8U		nBuf[100]  = {0};
	INT32S		nDataSize	= sizeof(nBuf);
	INT32S		nLen		= sizeof(nBuf);
	INT8U*		pData		= &nBuf[0];
		
	if (NULL == pData)
	{
		return -1;
	}
	if (nDataSize < 0)
	{
		return -1;
	}
	OSSemPend(g_pSem[SEM_RMOT_PARM], 0, &nErr);
	//+++++
    //F3主站IP地址和端口
	memset(pData, 0, nDataSize);
	if (MemReadCommPrm(F3, pData, nLen) >= 0)
    {
        memcpy(&pRmotParm->nMainIp[0], &pData[0], 4);
        pRmotParm->nMainPort = Buf2INT16U(&pData[4]);
        memcpy(&pRmotParm->nBakIp[0], &pData[6], 4);
        pRmotParm->nBakPort = Buf2INT16U(&pData[10]);
        memcpy(&pRmotParm->nApn, &pData[12], LEN_APN);
    }
	
    //F1通讯参数
    memset(pData, 0, nDataSize);
	if (MemReadCommPrm(F1, pData, nLen) > 0)
    {
        pRmotParm->nBeatIntvl = pData[5] * 60;  //心跳周期
    }
    
    //F7 终端IP地址和端口
    memset(pData, 0, nDataSize);
	if (MemReadCommPrm(F7, pData, nLen) > 0)
    {
        //终端IP
        memcpy(&pRmotParm->nTermIp[0], &pData[0], 4);
        //子网掩码
        memcpy(&pRmotParm->nTerminalMask[0], &pData[4], 4);
        //网关
        memcpy(&pRmotParm->nTerminalGate[0], &pData[8], 4);
        //帧听端口
        pRmotParm->nTermPort = Buf2INT16U(&pData[62]); 
    }
    
    
    //F8终端上行通信工作方式
    memset(pData, 0, nDataSize);
	if (MemReadCommPrm(F8, pData, nLen) > 0)
    {
        pRmotParm->nNetProt = (pData[0] & 0x80) >> 7;        //TCP\UDP 0：TCP 1:UDP
        pRmotParm->nWorkMode = (pData[0] & 0x30) >> 4;       //工作模式：0～2依次表示混合模式、客户机模式、服务器模式
        pRmotParm->nOnlineType = (pData[0] & 0x03);          //在线模式：在线模式1～3依次表示永久在线模式、被动激活模式、时段在线模式。
        pRmotParm->nLoginIntvl = Buf2INT16U(&pData[1]);      //重播间隔
    }
    
	
	OSSemPost(g_pSem[SEM_RMOT_PARM]);
	
	return 0;
}

/*
*****************************************************************************
* 函数名: RmotBufValidDataInfoMake
* 输　入: 
*			INT32S 				nPtr				在total缓冲区里的客户机/服务器有效数据的起始下标				
*			INT32S				nLen				数据长度
* 输  出: 
* 返  回: 
*		
*		
*		
* 功  能: 在获取到RCV(S)后的数据后调用
*****************************************************************************
*/
void RmotBufValidDataInfoMake(INT32S nPtr, INT32S nLen)
{
	g_nTotalRmotBufPtr = nPtr;
	g_nTotalRmotBufLen = nLen;
}

/*
*****************************************************************************
* 函数名: RmotAppBufLenGet
* 输　入: 
*			INT8U				nLinkType							
* 输  出: 
* 返  回: 
*		
*		
*		
* 功  能: 获取客户机或服务器的缓冲长度
*****************************************************************************
*/
INT32S RmotAppBufLenGet(INT8U nLinkType)
{
	INT32S nRet = -1;
	
	if (LINK_CLNT == g_nLinkType)
	{
		nRet = sizeof(g_nClntCommBuf);
	}
	else
	{
		nRet = sizeof(g_nSrvcCommBuf);
	}

	return nRet;
}


/*
*****************************************************************************
* 函数名: RmotRecv
* 输　入: 
*			INT8U*		pBuf				缓冲区
*			INT32S		nRdLen				读取长度
* 输  出: 
* 返  回: 
*		
*		
*		
* 功  能: 读取客户机或者服务器的数据  被linkcom调用
*****************************************************************************
*/
INT32S RmotRecv(INT8U* pBuf, INT32S nRdLen)
{
	//pBASE_BUF  	pBaseBuf	= &g_sTotalBaseBuf;
	pBASE_BUF  	pBaseBuf   = NULL;
	INT32U		nSize		= 0;//pBaseBuf->nBufSize;
	INT32S		i 			= 0;
	INT32S		nValidLen	= 0;
	INT32S		nTmp		= 0;
	
	if (NULL == pBuf)
	{
		return -1;
	}
	if (nRdLen < 0)
	{
		return -1;
	}
	else if (nRdLen == 0)
	{
		return 0;
	}
    
    if (LINK_CLNT == g_nLinkType)
	{
		pBaseBuf = &g_sClntBaseBuf2;
	}
	else
	{
		pBaseBuf = &g_sSrvcBaseBuf2;
	}
    nSize	= pBaseBuf->nBufSize;

	if (FALSE == ChkRange(pBaseBuf->nRecvPtr, 0, nSize - 1))
	{
		pBaseBuf->nRecvPtr = 0;
	}
	if (FALSE == ChkRange(pBaseBuf->nProcPtr, 0, nSize - 1))
	{
		pBaseBuf->nProcPtr = 0;
	}

	/*if (pBaseBuf->nProcPtr == pBaseBuf->nRecvPtr)
	{
		g_nTotalRmotBufPtr = 0;
		g_nTotalRmotBufLen = 0;
		return 0;
	}*/
	
	nTmp = (pBaseBuf->nRecvPtr + nSize - pBaseBuf->nProcPtr) % nSize;
	nValidLen = RmotAppBufLenGet(g_nLinkType);
	/*if (FALSE == ChkRange(g_nTotalRmotBufPtr, 0, nSize - 1))
	{
		return -1;
	}
	if (FALSE == ChkRange(g_nTotalRmotBufLen, 0, nValidLen - 1))
	{
		return -1;
	}

	for (i = 0; i < GET_MIN(nRdLen, g_nTotalRmotBufLen); i++)
	{
		pBuf[i] = BASEBUF_POS_PBUF(g_nTotalRmotBufPtr);
		g_nTotalRmotBufPtr = (g_nTotalRmotBufPtr + 1) % nSize;
		g_nTotalRmotBufLen--;
	}*/
	for (i = 0; i < GET_MIN(GET_MIN(nRdLen, nTmp), nValidLen); i++)
	{
		pBuf[i] = pBaseBuf->pBuf[pBaseBuf->nProcPtr % pBaseBuf->nBufSize];
        //pBaseBuf->pBuf[pBaseBuf->nProcPtr % pBaseBuf->nBufSize] = 0;
		pBaseBuf->nProcPtr = (pBaseBuf->nProcPtr + 1) % pBaseBuf->nBufSize;
	}

	return i;
}

/*
*****************************************************************************
* 函数名: RmotSend
* 输　入: 
*			INT8U*		pBuf				缓冲区
*			INT32S		nWrLen				读取长度
* 输  出: 
* 返  回: 
*		
*		
*		
* 功  能: 发送客户机或者服务器的数据  被linkcom调用
*****************************************************************************
*/
INT32S RmotSend(INT8U* pBuf, INT32S nWrLen)
{
	if (MODL_TELIT == g_nModlType)
	{
		if (LINK_CLNT == g_nLinkType)
		{
			TltTcpSend(1, pBuf, nWrLen);
		}
		else
		{
			TltTcpSend(2, pBuf, nWrLen);
		}
	}

	return 0;
}



#if 0
/*
	rmotbase操作
*/
#endif
/*
*****************************************************************************
* 函数名: BaseBufChkLabel
* 输　入: 
*			pBASE_BUF 	pBaseBuf	
*			INT32S		nPosBase					0时，从nProcPtr开始查起
*			INT8U* 		pLabel	
*			INT8U		nVal						
*			INT8U		nClr						1时，清掉标签
* 输  出: 
* 返  回: 
*		返回的关键字起始位置
*		
*		
* 功  能: 检查标签
*****************************************************************************
*/
INT32S BaseBufChkLabel(pBASE_BUF pBaseBuf, INT32S nPosBase, INT8U* pLabel, INT8U nVal, INT8U nClr)
{
	INT8U	nLabelLen 		= 0;
	INT32S	i				= 0;
	INT32S	j				= 0;
	INT32S	nValidDataLen	= 0;
	INT32S	nPos			= 0;

	if (NULL == pLabel)
	{
		return -1;
	}
	if (NULL == pBaseBuf)
	{
		return -1;
	}
	if (NULL == pBaseBuf->pBuf)
	{
		return -1;
	}
	if (0 == pBaseBuf->nBufSize)
	{
		return -1;
	}
	if (!ChkRange(pBaseBuf->nProcPtr, 0, pBaseBuf->nBufSize - 1))
	{
		return -1;
	}
	if (!ChkRange(pBaseBuf->nRecvPtr, 0, pBaseBuf->nBufSize - 1))
	{
		return -1;
	}
	if (pBaseBuf->nRecvPtr == pBaseBuf->nProcPtr)
	{
		return -1;
	}
	nValidDataLen = BASEBUF_BYTE_NUM(pBaseBuf);

	nLabelLen = strlen((char*)pLabel);
	if (nValidDataLen < nLabelLen + nPosBase)
	{
		return -1;
	}
	
	for (i = 0; i <= nValidDataLen - nLabelLen - nPosBase; i++)
	{
		for (j = 0; j < nLabelLen; j++)
		{
			nPos = nPosBase + i + j;
			if (BASEBUF_PROC_PBUF(nPos) != pLabel[j] + nVal)
			{
				break;
			}
		}
		if (j >= nLabelLen)
		{		
			// 清提示语
			if (nClr)
			{
				for (j = 0; j < nLabelLen; j++)
				{
					nPos = nPosBase + i + j;
					BASEBUF_PROC_PBUF(nPos) = 0;
				}
			}
			//基准下标(以procptr为基准)
			nPos = nPosBase + i;
			//return BASEBUF_PROC_PTR(nPos);
            return nPos;
		}
	}

	return -1;
}


/*
*****************************************************************************
* 函数名: SendAtData
* 输　入: 
* 输  出: 
* 返  回: 
*       
*       
* 功  能: 发送
*****************************************************************************
*/
static void SendAtData(const INT8U *pData,INT16U nLen)
{
	if (NULL == pData)
	{
		return;		
	}
	if (0 == nLen)
	{
		return;		
	}
	
	WriteComm(COMM_GPRS,(INT8U*)(pData),nLen);
	if (GetRmotCmdPrintFlg())
	{
		Gw3761FrmWrite(COMM_WH232, (INT8U*)(pData), nLen);
	}
}

/*
*****************************************************************************
* 函数名: SendAtOrder
* 输　入: 
* 输  出: 
* 返  回: 
*       
*       
* 功  能: AT发送
*****************************************************************************
*/
static void SendAtOrder(const char *pcDataAddr)
{
	INT8U	nSendLen = 0;
	if (NULL == pcDataAddr)
	{
		return;		
	}

	nSendLen = strlen(pcDataAddr);
	if (0 == nSendLen)
	{
		return;	
	}

	SendAtData((INT8U*)pcDataAddr,nSendLen);
}



/*
*****************************************************************************
* 函数名: WaitATResp
* 输　入: 

*					nType    0时,找到关键字，以关键字位置为起始，存储接收到的数据；1时,找到关键字，存储所有接收到的数据
* 输  出: 
* 返  回: 
*       
			-1				失败
			接收数据字节数: 成功  0也是成功
* 功  能: AT接收
*****************************************************************************
*/
static INT32S WaitATResp(const char *pKeyWord, INT32S nTimeOut, INT8U *pBuf, INT32U nBufSize, INT8U nType)
{
	pBASE_BUF	pBaseBuf 		= &g_sTotalBaseBuf;
	INT16U 		nRecvLen 		= 0;
	INT16U		nOnceLen		= 0;
	INT8U		nBuf[100]		= {0};
	INT16U		nSize			= 0;
	INT8U*		pData			= NULL;
	INT8U		nKeyWordLen 	= 0;
	INT16U		i 				= 0;
	INT16U		nRPtr       	= 0;
	INT8U		nFlg			= 0;

	//检测关键字
	nKeyWordLen = strlen(pKeyWord);
	if (0 == nKeyWordLen)
    {
    	return -1;
    }
	if (NULL == pBuf)
	{
		pData = &nBuf[0];
		nSize = sizeof(nBuf);
	}
	else
	{
		pData = pBuf;
		nSize = nBufSize;
	}

	memset(&pData[0], 0, nSize);
	while(1)
	{
		if (nTimeOut < 0)
		{
			break;
		}
		nRPtr 		= 0;
		nOnceLen 	= 0;
		nOnceLen 	= ReadCommMs(COMM_GPRS, &pData[nRecvLen], nSize - 2 - nRecvLen, 10, 500);
		if (nOnceLen > 0)
		{
			if (GetRmotCmdPrintFlg())
			{
				Gw3761FrmWrite(COMM_WH232, &pData[nRecvLen], nOnceLen);
			}
			BaseBufRecv(pBaseBuf, &pData[nRecvLen], nOnceLen);
			nRecvLen += nOnceLen;
		}
		if (nFlg)
		{
			if (NULL == pBuf)
			{
				return 0;
			}
			else
			{
				return nRecvLen;
			}
		}
		nTimeOut -= 500;
		nOnceLen = nRecvLen;
		while (nOnceLen >= nKeyWordLen)
		{
			for (i = 0; i < nKeyWordLen; i++)
			{
				if( pData[nRPtr + i ] != (((INT8U)pKeyWord[i]) + 1) )
				{
					break;
				}
			}	
			if (i < nKeyWordLen)		
			{
				nRPtr++;
				nOnceLen--;
			}
			else
			{
				if (0 == nType)
				{
					memmove(pData, &pData[nRPtr], nOnceLen);
					memset(&pData[nOnceLen], 0, nRPtr);
					nRecvLen = nOnceLen;
				}
				if (NULL == pBuf)
				{
					return 0;
				}
				nFlg = 1;	
				break;
			}
		}
	}
	return -1;
}


/*
*****************************************************************************
* 函数名: SendAtCmdAndWaitATResp
* 输　入: 

*					INT16U 		nSemFlg					0时调用串口保护信号量
*					INT32U		nSize					pBuf为NULL时，nSize不起作用
														为FOUR_BYTE_HIGH_BIT + 时，表示接收返回的数据；
														  正常长度时，返回关键字后面的数据
* 输  出: 
*					INT8U*		pBuf					如果和pcDataAddr同一个，则只能执行一次，即nCnt按照1来执行
* 返  回: 
*       
*      		 -1				失败
			 0或者接收数据字节数: 成功 
* 功  能: AT发送接收
*****************************************************************************
*/
INT32S SendAtCmdAndWaitATResp(const char *pcDataAddr,const char *pKeyWord, INT32S nTimeOut, INT8U nCnt, INT16U nSemFlg, INT8U *pBuf, INT32U nSize)
{
    INT8U		i 		= 0;
    INT8U       nNum	= nCnt;
    INT32S      nRet 	= 0;
	INT8U		nType	= 0;
	
	if (0 == nNum)
	{
		return -1;		
	}
	if (pcDataAddr == NULL)
	{
		return -1;
	}
	if ((INT32U)pcDataAddr == (INT32U)pBuf)
	{
		nNum = 1;													//只能执行一次
	}
	if (NULL != pBuf)
	{
		if (nSize & FOUR_BYTE_HIGH_BIT)
		{
			nSize &= ~FOUR_BYTE_HIGH_BIT;
			nType = 1;
		}
	}

	for (i = 0; i < nNum; i++)
	{
		if (!nSemFlg)
		{
			COM_PEND_SEM(COMM_GPRS);
		}
		SendAtOrder(pcDataAddr);
		if (NULL != pBuf)
		{
			memset(pBuf, 0, nSize);
		}
        nRet = WaitATResp(pKeyWord, nTimeOut, pBuf, nSize, nType);
		if (!nSemFlg)
		{
			COM_POST_SEM(COMM_GPRS);
		}
        if (nRet >= 0)
        {
            return (INT32U)nRet;    
        }
	}

	return -1;
}






#if 0

#endif
/****************************************************************************
* 函数名:ModlPowerCtrl
* 输　入:
* 输　出:
* 返  回:
* 调  用:
* 作　者:wyf
* 日　期:2009/10/03
* 版  本:Version 1.0
* 功能描述:模块电源开关:FALSE时关掉，TRUE打开
****************************************************************************/
INT32S ModlPowerCtrl(INT8U dir)
{
   	if (dir)
	{
     	//V5_GPRS_ON();   	
	}
	else
	{
		//V5_GPRS_OFF();    
	}

    return 0;
}


/****************************************************************************
* 函数名:ModlOnOff
* 输　入:
* 输　出:
* 返  回:
* 调  用:
* 作　者:wyf
* 日　期:2009/10/03
* 版  本:Version 1.0
* 功能描述:模块OnOff开关:FALSE时关掉，TRUE打开
****************************************************************************/
INT32S ModlOnOff(INT8U dir)
{
    if (dir)
	{
		
	}
	else
	{
		
	}

    return 0;
}


/****************************************************************************
* 函数名:ModlBpsAdpt
* 输　入:
* 输　出:
* 返  回:
* 调  用:
* 作　者:wyf
* 日　期:2009/10/03
* 版  本:Version 1.0
* 功能描述:模块波特率自适应
****************************************************************************/
INT32S ModlBpsAdpt(INT8U nCnt)
{
	INT8U               i				= 0;
    INT8U               nComm         	= COMM_GPRS;
	INT32S				nRet			= -1;
	INT32S				nSetBpsSucc	= -1;
	
	if (0 == nCnt)
	{
		return 0;		
	}
	nRet = SendAtCmdAndWaitATRespSlip("AT\x0d", (char*)g_Modl_OK, 1000, nCnt);
	if (nRet < 0)								//需要重新设置波特率
	{	
		for (i = 0; i < sizeof(g_dwBaudRate) / sizeof(INT32U); i++)
		{
			CloseComm(nComm);
 			OpenComm(nComm, g_dwBaudRate[i], NO_PARITY, ONE_STOP_BIT, EIGHT_CHAR_BIT);
 			CommFlush(nComm);
			nSetBpsSucc = SendAtCmdAndWaitATRespSlip("AT+IPR=115200\x0d", (char*)g_Modl_OK, 1000, nCnt);
			if (nSetBpsSucc >= 0)
			{
				break;	
			}
		}
		
		CloseComm(nComm);
		OpenComm(nComm, UART_BPS_115200, NO_PARITY, ONE_STOP_BIT, EIGHT_CHAR_BIT);	
		CommFlush(nComm);
		nRet = SendAtCmdAndWaitATRespSlip("AT\x0d", (char*)g_Modl_OK, 1000, nCnt);
	}

	return nRet;		
}


/****************************************************************************
* 函数名:ModlGetHardVer
* 输　入:
* 输　出:
* 返  回:
* 调  用:
* 作　者:wyf
* 日　期:2009/10/03
* 版  本:Version 1.0
* 功能描述:获取模块硬件版本
****************************************************************************/
INT32S ModlGetHardVer(void)
{
	INT32S					nRet			= -1;
	pTTIMER					pTTimer			= &g_pTimer[TIMER_TMP];
	pBASE_BUF				pBaseBuf 		= &g_sTotalBaseBuf;
	
	//总超时时间
	TimerOpen(pTTimer, 10);
	for (;;)
	{
		nRet = 0;
		if(0 == TimerValGet(pTTimer))
		{
			g_nModlType = MODL_TELIT;
			break;
		}
		if (SendAtCmdAndWaitATRespSlip((char*)"ATI\x0d", (char*)g_Modl_TELIT, 1000, 1) >= 0)
		{
			g_nModlType = MODL_TELIT;
			BaseBufChkLabel(pBaseBuf, 0, (INT8U*)g_Modl_TELIT, 1, 1);
			break;
		}
		else
		{
			if (BaseBufChkLabel(pBaseBuf, 0, (INT8U*)g_Modl_NEO, 1, 1) >= 0)
			{
				g_nModlType = MODL_NEO;	
				break;
			}
		}
		nRet = -1;
	}	
	
	return nRet;		
}


/********************************************************************************
* 函数名: ModlSignalRecord
* 输　入:

*		
* 输　出:
* 返  回:
* 功  能: 记录显示交互的信号强度
*********************************************************************************/
void ModlSignalRecord(INT8U nSignal)
{
    if (nSignal > 40)
    {
        nSignal = 0;
    }
    nSignal = nSignal / 6;
    if (nSignal > 4)
    {
        nSignal = 4;
    }
    MemWriteRamXmpData(F8, 0, &nSignal, sizeof(nSignal));
}


#if 0
#endif
/****************************************************************************
* 函数名:TcpLinkFrame
* 输　入:
* 输　出:
* 返  回:
* 调  用:
* 作　者:wyf
* 日　期:2009/10/03
* 版  本:Version 1.0
* 功能描述:登录报文
****************************************************************************/
INT32S TcpLinkFrame(INT8U nFn)
{
	INT8U 			nData[50] 	= {0};
	INT8U*			pData		= &nData[0];
	INT32S			nDatalen	= 0;
	// 1. 上报报文
	g_nReport = 1;
	GB3761PnFn2DADT(0, nFn, pData);
    //获取行政区码，终端地址
    g_nAsynFlg = 1;
    GetTermAreaAddr();
    nDatalen = Gw3761MakeFrm(0xC9, 0x02, pData, 4, sizeof(nData));
    g_nAsynFlg = 0;
	if (MODL_TELIT == g_nModlType)
	{
		TltTcpSend(1, &nData[0], nDatalen);
	}
	g_nReport = 0;
    nMessageFlg = 1;    //接收成功标志
    
	return 0;
}

/****************************************************************************
* 函数名:TcpLinkFrame
* 输　入:
* 输　出:
* 返  回:
* 调  用:
* 作　者:wyf
* 日　期:2009/10/03
* 版  本:Version 1.0
* 功能描述:登录报文
****************************************************************************/
INT32S NetLinkFrame(INT32S nSock, INT8U nFn)
{
	INT8U 			nData[50] 	= {0};
	INT8U*			pData		= &nData[0];
	INT32S			nDatalen	= 0;
	// 1. 上报报文
	GB3761PnFn2DADT(0, nFn, pData);
    //获取行政区码，终端地址
    g_nAsynFlg = 1;
    GetTermAreaAddr();
    nDatalen = Gw3761MakeFrm(0xC9, 0x02, pData, 4, sizeof(nData));
    g_nAsynFlg = 0;
    EthnetSend(nSock, pData, nDatalen, 0);
	return 0;
}

#if 0
#endif
/*
*****************************************************************************
* 函数名: RmotLinkInit
* 输　入: 
*				void *				pData				通信参数
* 输  出: 
* 返  回: 
*		真假
*		
*		
* 功  能: 
*****************************************************************************
*/
static INT32S RmotLinkInit(void *pData)
{
	pTTIMER pTTimer = NULL;
	
	g_nLoginCnt = 0;
	memset(g_pRmotValClr, 0, sizeof(RMOT_VAL_CLR));
	g_pRmotValClr->nNdStart = 1;									// 置重启标志
    RmotParamTrans();

	// 重拨
	pTTimer = &g_pTimer[TIMER_LOGIN_ITVL];
	TimerOpen(pTTimer, 0);
	return 0;
}

/*
*****************************************************************************
* 函数名: RmotLinkStat
* 输　入: 
* 输  出: 
* 返  回: 
*		真假
*		
*		
* 功  能: 
1、客户机、服务器全局标志切换
2、接收数据，往TotalBuf存放。接收数据使用不等待超时的ExtReadCommMs函数
3、把客户机g_sClntBaseBuf缓冲与服务器g_sSrvcBaseBuf缓冲基本值赋给LinkCom的BaseBuf
4、按照客户机、服务器全局标志解析TotalBuf，找到符合客户机或者服务器的数据(找到TotalBuf的符合要求的数据的起始位置pos与长度len)
*****************************************************************************
*/
static INT32S RmotLinkStat(void* pLinkBaseBuf)
{	
#ifdef GPRS_MOD_DEBUG

#else
	pBASE_BUF  	pBaseBuf 	= &g_sTotalBaseBuf;	
	
	if (NULL == pLinkBaseBuf)
	{
		return -1;
	}

	// 1. 客户机、服务器切换标志
	g_nLinkType = (g_nLinkType + 1) % LINK_NUM;
	
	// 2. 接收到TotalBuf
	RmotTotalBufRecv(pBaseBuf);

	// 3. 获取当前使用的LinkCom的BaseBuf
	UseCurrBaseBuf((pBASE_BUF)pLinkBaseBuf, g_nLinkType);

	// 4. 具体模块函数
	TelitProc(pBaseBuf, &g_sClntBaseBuf2, &g_sSrvcBaseBuf2, g_nLinkType);
#endif

	return 0;
}

/*
*****************************************************************************
* 函数名: RmotLinkTailProc
* 输　入: 
* 输  出: 
* 返  回: 
*		真假
*		
*		
* 功  能: 
*****************************************************************************
*/
static INT32S RmotLinkTailProc(void* pBaseBuf)
{
	return SaveCurrBaseBuf(pBaseBuf);
}


/*
*****************************************************************************
* 函数名: RmotLinkTask
* 输　入: 
* 输  出: 
* 返  回: 
*		真假
*       
*       
* 功  能: 
*****************************************************************************
*/
void RmotLinkTask(void *pData)
{
	LinkComTaskProc(COMM_GPRS, pData, RmotLinkInit, RmotLinkStat, RmotLinkTailProc); 
}











