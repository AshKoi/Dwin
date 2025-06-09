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
#include "ethnetlink.h"
#include "Macro.h"
#include "Base.h"
#include "Fun.h"
#include "SysRes.h"
#include "Linkcom.h"
#include "linkcomExt.h"
#include "rmotlink.h"
#include "MemTbl.h"

/**************************************************************************************************
                                           相关变量
***************************************************************************************************/
INT8U					g_nEthnetMod	 = 0;				// 0客户机；1服务器 
INT8U					g_nIpPortChgGlg  = 0;

#if 0
typedef struct _tagENET_PARAM{
    INT8U               MACAddr[6];			            /*MAC地址*/		
    INT8U               TermIP[4];                      /*终端IP地址*/
    INT8U               SubnetIP[4];                    /*子网掩码地址*/
    INT8U               GateIP[4];                      /*网关地址*/
    OS_EVENT	        *semInitLwIP;                   /*信号量*/
    INT32U              nTimeout;                       /*初始化超时时间（秒）: 取值范围 10 ~ 5*60*60秒，默认为60秒 */
    INT8U               nDebugLevel;                    /*调试级别: 0 - 无调试信息
                                                                   1 - FATAL
                                                                   2 - ERROR
                                                                   3 - WARNING
                                                                   4 - INFO     */
}ENET_PARAM,*pENET_PARAM;


typedef struct _tagENET_INTERFACE{
	EnetInit;
	EnetFree;
	setsockopt;
	close;
	socket;
	connect;
	htons;
	accept;
	
}ENET_INTERFACE, *pENET_INTERFACE;

#endif
static ENET_PARAM       g_sEnetParam = {
	{0x20, 0xEF, 0xC6, 0x33, 0x44, 0x01},  // MAC 地址
	{192, 168, 0, 	 3},
	{255, 255, 255, 0},
	{192, 168, 0, 	 1},
	NULL,
	10,
	4
};

ENET_STATE_CLR			g_sEnetStateClr;
ENET_STATE_KEEP	        g_sEnetStateKeep;

// 以太网soket列表变量
COM_VALIABLE	g_sComValiable;


/**************************************************************************************************
                                           相关函数实现
***************************************************************************************************/	
/*
*****************************************************************************
* 函数名: EthnetSockSet
* 输　入: 
*		INT8U nCom
* 输  出: 
* 返  回: 
*		0成功，-1失败
*		
*		
* 功  能: 信号量
*****************************************************************************
*/
void EthnetSockSet(INT8U nCom, INT32S nSock)
{
	if (nCom == COMETNET)
	{
		g_sComValiable.nSock = nSock;
	}
	else if (nCom == COMETNETSRV)
	{
		g_sComValiable.nSerSockCli = nSock;
	}
}

/*
*****************************************************************************
* 函数名: EthnetAppInit
* 输　入: 
* 输  出: 
* 返  回: 
*		真假
*       
*       
* 功  能: 调用底层EnetInit，初始化以太网环境及软件变量。bEnetNdRestart标志
*****************************************************************************
*/
static INT8S EthnetAppInit(void)
{
    pENET_PARAM     pEnetParam   = &g_sEnetParam;
    //INT16U           nIpCs        = 0;
    if (g_sEnetStateClr.bEnetInitSuccFlg)
    {
    	return RET_ERR_OK;
    }
    //赋值F7
    //MAC地址（根据唯一标识来区分）
    memcpy(&pEnetParam->MACAddr[0], &g_pMem->sXFramData.nMacAddr[0], 6);
    //终端IP
    memcpy(&pEnetParam->TermIP[0], &g_pRomtParm->nTermIp[0], 4);
    //子网掩码
    memcpy(&pEnetParam->SubnetIP[0], &g_pRomtParm->nTerminalMask[0], 4);
    //网关
    memcpy(&pEnetParam->GateIP[0], &g_pRomtParm->nTerminalGate[0], 4);
	
	RenetInit(pEnetParam);
	memset(&g_sEnetStateClr,0,sizeof(g_sEnetStateClr));
	g_sEnetStateClr.nSock 				= -1;
	g_sEnetStateClr.nSerSock 			= -1;
	g_sEnetStateClr.nSerSockCli 		= -1;
	g_sEnetStateClr.bEnetInitSuccFlg 	= TRUE;
	memcpy(&g_sEnetStateClr.tEnetParam, pEnetParam, sizeof(ENET_PARAM));
	return RET_ERR_OK;
}
/******************************************************************************
**
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 以太网释放处理标志检查
*******************************************************************************
**/
static void EthnetFreeCheckCondition(void)
{
	BOOLEAN   bFlg  = FALSE;
	
	if (g_sEnetStateClr.bEnetRestartFlg)
	{
		bFlg = TRUE;
	}
	
	if (g_sEnetStateClr.nEnetCreatSockCnt > MAX_ENET_SOCKET_NUM)
	{
		bFlg = TRUE;
	}
	
	if (g_sEnetStateClr.nEnetCreatSerSockCnt > MAX_ENET_SOCKET_NUM)
	{
		bFlg = TRUE;
	}
	
	if (bFlg)
	{
		//ResetDelayHardInit(0);
       if (g_sEnetStateClr.nSerSock >= 0)
        {   
                close(g_sEnetStateClr.nSerSock);//就是lwip_close
        }
        if (g_sEnetStateClr.nSerSockCli >= 0)
        {   
                close(g_sEnetStateClr.nSerSockCli);//就是lwip_close
        }
       
		g_sEnetStateClr.bEnetRestartFlg      	= FALSE;
		g_sEnetStateClr.nEnetCreatSockCnt 	= 0;
		g_sEnetStateClr.nEnetCreatSerSockCnt 	= 0;
		g_sEnetStateClr.bEnetInitSuccFlg    	= FALSE;
		OSSemPost(g_pSem[SEM_ENET_FREE_IDX]);	 
		ResetDelayHardInit(10 * 1000);
	}
}




/**************************************************************************************************
                                           TCP链接创建相关
***************************************************************************************************/
/********************************************************************************
* 函数名:EthnetSetTcpSocketParam
* 输　入:
* 输　出:
* 返  回:
* 功  能: 设置TCPSocket基本参数
*********************************************************************************/
INT8S EthnetSetTcpSocketParam(INT32S nSock)
{
	INT32S   nKeepAlive 	= 1;         	//启动 KeepAlive 机制
	INT32S   nKeepIdle 		= 5;          	//首次探测开始前的tcp无数据收发空闲时间
    INT32S   nKeepInterval 	= 3;      		//每次探测的间隔时间(秒)
   	INT32S   nKeepCount 	= 2;         	//探测次数

    if (-1 == nSock)
	{
		return RET_ERR_ERROR;
	}

    if( setsockopt( nSock, SOL_SOCKET, SO_KEEPALIVE, (void*)&nKeepAlive, sizeof(nKeepAlive) ) == SOCKET_ERROR )
    {
    	return RET_ERR_ERROR;
    }
    if( setsockopt( nSock, IPPROTO_TCP, TCP_KEEPIDLE, (void*)&nKeepIdle, sizeof(nKeepIdle) ) == SOCKET_ERROR)
    {
    	return RET_ERR_ERROR;
    }
    if( setsockopt( nSock, IPPROTO_TCP, TCP_KEEPINTVL, (void*)&nKeepInterval, sizeof(nKeepInterval) ) == SOCKET_ERROR)
    {
    	return RET_ERR_ERROR;
    }
    if( setsockopt( nSock, IPPROTO_TCP, TCP_KEEPCNT, (void*)&nKeepCount, sizeof(nKeepCount) ) == SOCKET_ERROR)
    {
    	return RET_ERR_ERROR;
    }

    return RET_ERR_OK;
}

/********************************************************************************
* 函数名:EthnetCliTCPSocketCreate
* 输　入:
* 输　出:
* 返  回:
* 功  能: 创建TCP客户端Socket对象
*********************************************************************************/
static INT8S EthnetCliTCPSocketCreate(void)
{
	BOOLEAN				bFlg			= FALSE;
	INT8U				i				= 0;
	INT32U				nPort			= 5600;
	INT8U				nIP[4]		={192,168,0,55};
	INT8U   			nTmpBuff[15];
	ENET_SOCKADDR_IN	sEnetSockAddrIn;
	
	if (g_sEnetStateClr.bEnetCreatSockSucc)
	{
		return RET_ERR_OK;
	}
	do
	{
		// 1. 创建一个socket，类型是SOCKET_STREAM，TCP类型
		if ((g_sEnetStateClr.nSock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			/* 创建socket失败 */
			bFlg = TRUE;
		   	break;
		}
		// 2. 给sock配置参数
		if (RET_ERR_OK !=  EthnetSetTcpSocketParam(g_sEnetStateClr.nSock))
		{
		    bFlg = TRUE;
		   	break;
		}	

		memcpy(&nIP[0], &g_pRomtParm->nMainIp[0], 4);
		nPort = g_pRomtParm->nMainPort;
		memset(&nTmpBuff[0],0,sizeof(nTmpBuff));
		sprintf((char*)nTmpBuff,"%d.%d.%d.%d", nIP[0], nIP[1], nIP[2], nIP[3]);
		
		/* 初始化预连接的服务端地址 */
		sEnetSockAddrIn.sin_len 		= sizeof(sEnetSockAddrIn);
		sEnetSockAddrIn.sin_family 		= AF_INET;
		sEnetSockAddrIn.sin_port 		= htons(nPort);
		sEnetSockAddrIn.sin_addr.s_addr = inet_addr( (char*)nTmpBuff );
		//memset(&(sEnetSockAddrIn.sin_zero), 0, sizeof(sEnetSockAddrIn.sin_zero));
		
	#define ENET_CLI_CONNECT_CNT 1
		for (i = 0; i < ENET_CLI_CONNECT_CNT; i++)
		{
			// 3. 连接到服务端
			if (connect(g_sEnetStateClr.nSock, (ENET_SOCKADDR*)&sEnetSockAddrIn, sizeof(ENET_SOCKADDR)) == -1)
			{
				close(g_sEnetStateClr.nSock);
			}
			else
			{
				break;
			}
		}
		if (i >= ENET_CLI_CONNECT_CNT)
		{
			bFlg = TRUE;
		}
	#undef ENET_CLI_CONNECT_CNT
	}while(0);

	if (bFlg)
	{
		g_sEnetStateClr.nSock = -1;
		return RET_ERR_ERROR;
	}

	g_sEnetStateClr.bEnetCreatSockSucc = TRUE;
    
    //
    NetLinkFrame(g_sEnetStateClr.nSock, F1);
    
	return RET_ERR_OK;
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:RET_ERR_OK可以
* 功  能: 判断客户端是否已经可以接收发送
*********************************************************************************/
static INT8S EthnetCliIsSendOrRecv(void)
{	
	if (RET_ERR_OK != EthnetCliTCPSocketCreate())
	{
		g_sEnetStateClr.nEnetCreatSockCnt++;
		return RET_ERR_ERROR;
	}

	if (!g_sEnetStateClr.bEnetCreatSockSucc)
	{
		return RET_ERR_ERROR;
	}
	
	return RET_ERR_OK;
}


/**************************************************************************************************
                                           TCP接受数据相关
***************************************************************************************************/
/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 获取已有的最大socket
*********************************************************************************/
static INT32S EthnetGetMaxSocket(void)
{
	return GET_MAX3(g_sEnetStateClr.nSock, g_sEnetStateClr.nSerSockCli, g_sEnetStateClr.nSerSock);
}

/*
*****************************************************************************
* 函数名: EthnetSockStateCheck
* 输　入: 
*			INT32S		nSock
* 输  出: 
*			INT8U* 	pFlg						 // 1时socket正常，有数据，可以接收；
													0时socket正常，没有数据，不能接收；
													2时socket异常，不能接收
* 返  回: 
*       
*       
* 功  能: 检查socket状态:返回大于0才是正常可以发送或接收
*****************************************************************************
*/
INT32S EthnetSockStateCheck(INT32S nSock, INT8U* pFlg)
{
	INT32S			nRet	 = 0;
	ENET_FD_SET 	fdReadSet;
	ENET_FD_SET 	fdWriteSet;
	ENET_TIMEVAL	sTimeOut;
	
	if (-1 == nSock)
	{
		return 0;
	}
    if (NULL == pFlg)
	{
		return 0;
	}
	sTimeOut.tv_sec  = 2;
	sTimeOut.tv_usec = 0;
	
	FD_ZERO(&fdReadSet);
	FD_ZERO(&fdWriteSet);
	FD_SET(nSock, &fdReadSet);
	FD_SET(nSock, &fdWriteSet);	
	nRet = select(EthnetGetMaxSocket() + 1, &fdReadSet, &fdWriteSet, 0, &sTimeOut);	
	if (nRet > 0)
	{
		if (FD_ISSET(nSock, &fdReadSet))
        {
            *pFlg = 1;          //socket正常，有数据，可以接收
        }
        else
		{
			nRet  = 0;
            *pFlg = 0;          //socket正常，没有数据，不能接收
		}
	}
    else
    {
        *pFlg = 2;              //socket异常，不能接收
    }

	return nRet;
}

/*
*****************************************************************************
* 函数名: EthnetErrProc
* 输　入: 
*			INT32S		nSock
*			INT8U		nType					//以太网客户机ETHNET_CLI_COMM、服务器
* 输  出: 
* 返  回: 
*       
*       
* 功  能: 
*****************************************************************************
*/
void EthnetErrProc(INT32S nSock, INT8U nType, INT8U bFlg)
{
     close(nSock);//就是lwip_close
    if (ETHNET_CLI_COMM == nType)
	{
		g_sEnetStateClr.nSock               	= -1;
		g_sEnetStateClr.bEnetCreatSockSucc   	= FALSE;
		g_sEnetStateClr.bEnetLoginOnSucc   	= FALSE;
		g_sEnetStateClr.bEnetRestartFlg 		= TRUE;//需要重新以太网
	}
	else
	{
		g_sEnetStateClr.nSerSockCli       		= -1;
        g_sEnetStateClr.bSerSockCliSucc 		= FALSE;
        //g_sEnetStateClr.nSerFailCnt++;
		if (2 == bFlg)               
		{
			//g_sEnetStateClr.nSerFailCnt = 0;
			if (g_sEnetStateClr.nSerSock >= 0)
			{   
				close(g_sEnetStateClr.nSerSock);//就是lwip_close
			}
			g_sEnetStateClr.nSerSock            	= -1;
			g_sEnetStateClr.bEnetCreatSerSockSucc	= FALSE;
			g_sEnetStateClr.bEnetRestartFlg 		= TRUE;//需要重新以太网
		}
	}
}




// 发送、接收函数  EthnetSend   EthnetRecv
/*
*****************************************************************************
* 函数名: EthnetRecv
* 输　入: 
*			INT32S		nSock
*			INT16U 	nMaxRdLen				// 
*			INT32S		nFlags					
*			INT8U		nType					// 以太网客户机、服务器
* 输  出: 
*			INT8U* 	pBuf					// 接收缓冲区
* 返  回: 
*       
*       
* 功  能: 以太网链路接收数据
*****************************************************************************
*/
INT16U EthnetRecv(INT32S nSock, INT8U* pBuf, INT16U nMaxRdLen, INT32S nFlags, INT8U nType)
{
    int 	nRecvLen 	= 0;
    INT8U	nFlg     	= 0;
    INT8U	bFlg     	= FALSE;
	
	if (NULL == pBuf)
	{
		return 0;
	}
	if (0 == nMaxRdLen)
	{
		return 0;
	}
	if (-1 == nSock)
	{
		return 0;
	}
	
    do
    {   
        EthnetSockStateCheck(nSock, &nFlg);
        if (nFlg == 2)
        {
            bFlg = TRUE;                   //socket异常，不能接收数据
			break;
        }
        else if (0 == nFlg)               //socket正常，没有数据，不能接收
        {   
			break;
        }     
		//socket正常，有数据，可以接收
        nRecvLen = recv(nSock, pBuf, nMaxRdLen, nFlags);
        if (nRecvLen == 0)				   //可以接收数据时，接收长度为0，则socket异常
        {   
            bFlg = TRUE;                   // socket 断开
            break;
        }
        else if (nRecvLen < 0)
        {
            bFlg = 2;                   // socket 断开
            break;
        }
        else
        {    
            g_sEnetStateClr.nEthNoCommCnt 	= 0;            // socket 连接有数据到来
        }
    }while(0);
    if (bFlg)
    {
        EthnetErrProc(nSock, nType, bFlg);
        return 0;
    }

	return nRecvLen;
}

/********************************************************************************
* 函数名:EthnetSend
* 输　入:
* 输　出:
* 返  回:
* 功  能:
*********************************************************************************/
INT32S EthnetSend(INT32S nSock, const INT8U* pSendBuf, INT32U nSendLen, INT32S nFlags)
{
    return send(nSock, pSendBuf, nSendLen, nFlags);
}

/**************************************************************************************************
                                           以太网TCP线程
***************************************************************************************************/
/*
*****************************************************************************
* 函数名: EthnetLinkTaskInit
* 输　入: 
* 输  出: 
* 返  回: 
*       
*       
* 功  能: 
*****************************************************************************
*/
static INT32S EthnetLinkTaskInit(void *pData)
{    
	pData = pData;

	// 清掉初始化成功标志
	g_sEnetStateClr.bEnetInitSuccFlg 	= FALSE;
	
    return RET_ERR_OK;
}

/*
*****************************************************************************
* 函数名: EthnetLinkTaskProcStat
* 输　入: 
* 输  出: 
* 返  回: 
*       
*       
* 功  能: 
*************s****************************************************************
*/
static INT32S EthnetLinkTaskProcStat(void* pData)
{	
	// 1. 以太网初始化及释放标志判断处理
#if 1
 	if ((g_sEnetStateClr.nEthNoCommCnt >= 150) || g_nIpPortChgGlg)
	{  
		g_sEnetStateClr.nEthNoCommCnt = 0;	
	    g_nIpPortChgGlg = 0;
		g_sEnetStateClr.bEnetRestartFlg = TRUE;        
	}
#endif 
	EthnetFreeCheckCondition();
    EthnetAppInit();
	// 3. 链路状态判断
	// 3.1. 初始化不成功，返回-1
    if (FALSE == g_sEnetStateClr.bEnetInitSuccFlg)
    {
    	return -1;
    }
	// 3.2. 服务器模式
	
    if (1 == g_nEthnetMod)
    {
      	return -1;  
    } 
    
#if 1    
 //   OSTimeDlyHMSM(0, 0, 10, 0);
    /* 3.2 判断服务器准备完毕 */
	if (!g_sEnetStateClr.bEnetCreatSerSockSucc)
    {
    	//return -1;
    } 

	// 3.3. 客户机是否连接成功
	//if (g_sEnetStateClr.nSelfLinkCnt > 30)
	{
	    if (RET_ERR_OK == EthnetCliIsSendOrRecv())
	    {      
#if 0
			//g_sEnetStateClr.bSelfLinkFlg = 1;
			g_sEnetStateClr.nEthCliFailedCnt = 0;
			//EthnetSend(g_sEnetStateClr.nSock, &g_sEnetStateClr.nEthCliFailedCnt, 1, 0);
			OSTimeDlyHMSM(0, 0, 0, 500);
			if (g_sEnetStateClr.nSerSockCli >= 0)
			{
				close(g_sEnetStateClr.nSerSockCli);
				g_sEnetStateClr.nSerSockCli = -1;
				g_sEnetStateClr.bSerSockCliSucc = FALSE;
			}
			/*if (g_sEnetStateClr.nSock >= 0)
			{
				close(g_sEnetStateClr.nSock);
				g_sEnetStateClr.nSock = -1;
			}*/
			//g_sEnetStateClr.bEnetCreatSockSucc = FALSE;
			//g_sEnetStateClr.bSelfLinkFlg = 0;
	        OSTimeDlyHMSM(0, 0, 0, 100);
			//g_sEnetStateClr.nSelfLinkCnt = 0;
#endif

	    }
		else
		{
#if 0
			g_sEnetStateClr.nEthCliFailedCnt ++;
			if(g_sEnetStateClr.nEthCliFailedCnt >= 2)         /* 1分半钟连接自连不上重启以太网 */
			{
				//g_sEnetStateClr.nSelfLinkCnt = 0;
                g_sEnetStateClr.nEthCliFailedCnt = 0;
				g_sEnetStateClr.bEnetRestartFlg = TRUE;
			}
              OSTimeDlyHMSM(0, 0, 0, 100);
#endif
		}

	}
#endif
    
	// 3.4. 记录客户机链接成功的sock
	EthnetSockSet(COMETNET, g_sEnetStateClr.nSock);

    return 0;
}





/*
*****************************************************************************
* 函数名: EthnetLinkTask
* 输　入: 
* 输  出: 
* 返  回: 
*       
*       
* 功  能: 以太网链路任务处理（阻塞）
*****************************************************************************
*/
void EthnetLinkTask(void *pData)
{
	LinkComTaskProc(COMETNET, pData, EthnetLinkTaskInit, EthnetLinkTaskProcStat, NULL);	
}


