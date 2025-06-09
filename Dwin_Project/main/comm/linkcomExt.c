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
#include "AppHead.h"
#include "ethnetlink.h"

/**************************************************************************************************
                                           内联函数宏
***************************************************************************************************/



// 链路结构核查有效     // #define LINK_COM_VALID		
// 基本缓冲处理指针清0处理  // #define LINK_COM_FRAME_CLR

//通信口声明所用
#define LIST_BASE_BUF_NULL		{NULL, 0, 0, 0}			        			//
#define LIST_BASE_BUF(nBuf)		{&nBuf[0], 	sizeof(nBuf), 0, 0}			//


#define LIST_BASE_BUF_SUB_NULL	{NULL, 0}			        				//
#define LIST_BASE_BUF_SUB(nBuf)	{&nBuf[0], 	sizeof(nBuf)}					//


#define LIST_LINK_COM_NULL(nCom)	{                                           \
	nCom, 							                                            \
	LIST_BASE_BUF_NULL, 			                                            \
	LIST_BASE_BUF_SUB_NULL,														\
    0, 								                                            \
	{{0}}, 			                                                			\
	NULL,																		\
	NULL,																		\
	NULL,                                                                       \
}/////////////////////////////////////////////////////////////////////////////////////

#define LIST_LINK_COM(nCom, nBuf, nFrmBuf, pLinkComCheckPackage, pLinkComProc, ppEvent)   {	\
	nCom, 																		\
	LIST_BASE_BUF(nBuf), 														\
	LIST_BASE_BUF_SUB(nFrmBuf),												\
 	0, 																			\
	{{0}}, 			                                                			\
	(void*)pLinkComCheckPackage,												\
 	(void*)pLinkComProc,														\
 	ppEvent,                                                                   \
}/////////////////////////////////////////////////////////////////////////////////////




/**************************************************************************************************
                                           变量声明定义
***************************************************************************************************/
INT8U					g_nGw3761SendBuf[GW3761_SENDBUF_SIZE]	= {0};

// COMM_HOST 接收缓冲区与帧缓冲区
static INT8U 			g_nUpRecvCom1Buf[MAX_RECV_BUF1_SIZE];
static INT8U 			g_nFrmCom1Buf[MAX_FRM_BUFF_SIZE];

static INT8U 			g_nUpRecvCom2Buf[MAX_RECV_BUF2_SIZE];
static INT8U 			g_nFrmCom2Buf[MAX_FRM_BUFF_SIZE];

static INT8U 			g_nUpRecvComIrBuf[MAX_RECV_BUFIR_SIZE];
static INT8U 			g_nFrmComIrBuf[MAX_FRM_BUFF_SIZE];

//static INT8U 			g_nUpRecvComEnetBuf[1024];
//static INT8U 			g_nFrmComEnetBuf[MAX_FRM_BUFF_SIZE];

//static INT8U 			g_nUpRecvComEnetSrvBuf[1024];
//static INT8U 			g_nFrmComEnetSrvBuf[MAX_FRM_BUFF_SIZE];

static INT8U 			g_nUpRecvCom4Buf[MAX_RECV_BUFZB_SIZE];
static INT8U 			g_nFrmCom4Buf[MAX_FRM_BUFF_SIZE];

// 串口信息列表
LINK_COM		g_sLinkCom[] = {

    LIST_LINK_COM(COMM_WH485,				g_nUpRecvCom1Buf,	g_nFrmCom1Buf,		Gw3761AsynCheckPackageTmp, 		Gw3761AsynProcTmp, (void**)(&g_pSem[SEM_TRANS_COM485_IDX])) ,
    LIST_LINK_COM(COMM_WH4852,				g_nUpRecvCom2Buf,	g_nFrmCom2Buf,		Gw3761AsynCheckPackageTmp, 		Gw3761AsynProcTmp, (void**)(&g_pSem[SEM_TRANS_COM4852_IDX])) ,
    LIST_LINK_COM(COMM_SMQ,				    g_nUpRecvComIrBuf,	g_nFrmComIrBuf,		Meter645AsynCheckPackage, 		Meter645AsynProc, (void**)(&g_pSem[SEM_TRANS_COMIR_IDX])) ,
	LIST_LINK_COM(COMM_BLE,				    g_nUpRecvCom4Buf,	g_nFrmCom4Buf,		Meter645AsynCheckPackage, 		Meter645AsynProc, (void**)(&g_pSem[SEM_TRANS_COMZB_IDX])) ,
    LIST_LINK_COM(COMM_232,				    g_nUpRecvCom4Buf,	g_nFrmCom4Buf,		DwinAsynCheckPackage, 		DwinAsynProc, (void**)(&g_pSem[SEM_TRANS_COMZB_IDX])) ,
	LIST_LINK_COM_NULL(0xFF) ,
};
 
extern COM_VALIABLE	g_sComValiable;
/**************************************************************************************************
                                           函数
***************************************************************************************************/
/*
*****************************************************************************
* 函数名: IsAysnCom
* 输　入: 
*		INT8U nCom
* 输  出: 
* 返  回: 
*		
*		
* 功  能: 判断通讯口是否有异步通讯
*****************************************************************************
*/
BOOLEAN IsAysnCom(INT8U nCom)
{
	pLINK_COM		pLinkCom	= (pLINK_COM)&g_sLinkCom[0];

	for (;;)
	{
		if (pLinkCom->nCom == 0xFF)
		{
			return FALSE;
		}

		if (pLinkCom->nCom == nCom)
		{
			return TRUE;
		}
		pLinkCom++;
	}
}


/*
*****************************************************************************
* 函数名: ExtReadCommMs
* 输　入: 
* 输  出: 
* 返  回: 
*       
*       
* 功  能: 接收数据
*****************************************************************************
*/
INT16U ExtReadCommMs(INT8U nCom, INT8U *pBuf, INT16U nMaxLen, INT32U nInter, INT16U nDelay)
{
    INT32S nRecvLen   = 0;
    INT32S nNoReadLen = 0;

    if((NULL == pBuf) || (0 == nMaxLen))
    {
		return 0;
	}

    nNoReadLen = CommPending(nCom);
#if 0
    if (nNoReadLen > 0)//调试用
    {
        if (COMM_CARR_RCV_DEBUG == nCom)
        {
            nRecvLen++;
        }
    }
#endif
    if (0 == nNoReadLen)
	{
        return 0;
	}

    nRecvLen = ReadCommMs(nCom, pBuf, nMaxLen, nInter, nDelay);
    if (nRecvLen < 0)
    {
    	nRecvLen = 0;
    }
    else if (nRecvLen > 0)
    {
    	
    }

    return nRecvLen;
}

/*
*****************************************************************************
* 函数名: LinkExtRecvProc
* 输　入: 
*		INT8U nCom
* 输  出: 
* 返  回: 
*		空指针,失败
*		
*		
* 功  能: 
*****************************************************************************
*/
INT32S LinkExtRecvProc(INT8U nCom, INT8U* pBuf, INT32S nRdOnce)
{
	INT32S		nRcvLen 					 = 0;
	
	if (nCom == COMETNET)
	{
#if 1 == ENET_COMM_FUNC_EN
		nRcvLen = EthnetRecv(g_sComValiable.nSock, &pBuf[0], nRdOnce, 0, ETHNET_CLI_COMM);
#endif
	}
	else if (nCom == COMETNETSRV)
	{
#if 1 == ENET_COMM_FUNC_EN
		nRcvLen = EthnetRecv(g_sComValiable.nSerSockCli, &pBuf[0], nRdOnce, 0, ETHNET_SRV_COMM);
#endif
	}
	else if (nCom == COMWLAN)
	{
#if ASSIST_RUN == ASSIST_RUN_SIGNAL
		nRcvLen = ReadWlanBuf(&pBuf[0], nRdOnce);
#endif
	}
	else if (nCom == COMM_GPRS)
	{
		nRcvLen = RmotRecv(&pBuf[0], nRdOnce);
	}
	else
	{
		nRcvLen = ExtReadCommMs(nCom, &pBuf[0], nRdOnce, 20, 100);
	}
	
	return nRcvLen;
}

/*
*****************************************************************************
* 函数名: LinkExtSendProc
* 输　入: 
*		INT8U nCom
* 输  出: 
* 返  回: 
*		空指针,失败
*		
*		
* 功  能: 
*****************************************************************************
*/
INT32S LinkExtSendProc(INT8U nCom, INT8U* pSendbuf, INT32S nSendlen)
{
	INT32S		nRet						 = 0;
	
	if (nCom == COMETNET)
	{
#if 1 == ENET_COMM_FUNC_EN
		nRet = EthnetSend(g_sComValiable.nSock, pSendbuf, nSendlen, 0);
#endif
	}
	else if (nCom == COMETNETSRV)
	{
#if 1 == ENET_COMM_FUNC_EN
		nRet = EthnetSend(g_sComValiable.nSerSockCli,pSendbuf, nSendlen, 0);
#endif
	}
	else if (nCom == COMWLAN)
	{
#if ASSIST_RUN == ASSIST_RUN_SIGNAL
		nRet = WriteWlanBuf(pSendbuf, nSendlen);
#endif
	}
	else if (nCom == COMM_GPRS)
	{
		nRet = RmotSend(pSendbuf, nSendlen);
	}
	else
	{
		nRet = WriteComm(nCom, pSendbuf, nSendlen);
	}	
	
	return nRet;
}


/*
*****************************************************************************
* 函数名: RecvProcExt
* 输　入: 
*		INT8U nCom
* 输  出: 
* 返  回: 
*		空指针,失败
*		
*		
* 功  能: 
*****************************************************************************
*/
void RecvProcExt(INT8U nCom, INT32S nRecvLen)
{

}

/*
*****************************************************************************
* 函数名: WriteProcExtPre
* 输　入: 
*		INT8U nCom
* 输  出: 
* 返  回: 
*		空指针,失败
*		
*		
* 功  能: 
*****************************************************************************
*/
void WriteProcExtPre(INT8U nCom)
{

}

/*
*****************************************************************************
* 函数名: WriteProcExtBack
* 输　入: 
*		INT8U nCom
* 输  出: 
* 返  回: 
*		空指针,失败
*		
*		
* 功  能: 
*****************************************************************************
*/
void WriteProcExtBack(INT8U nCom)
{

}



