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
#include "ethnetserlink.h"
#include "Macro.h"
#include "Base.h"
#include "Linkcom.h"
#include "linkcomExt.h"

/**************************************************************************************************
                                           相关全局变量实现
***************************************************************************************************/
extern INT8U                    g_nEthnetMod;

extern ENET_STATE_CLR			g_sEnetStateClr;
extern ENET_STATE_KEEP	        g_sEnetStateKeep;
extern volatile  INT8U          PortCorrect;
/**************************************************************************************************
                                           函数声明
***************************************************************************************************/
extern INT8S        EthnetSetTcpSocketParam(INT32S nSock);
extern void 		EthnetErrProc(INT32S nSock, INT8U nType, INT8U bFlg);
extern INT8U        g_nEthNoCommCnt ;
extern INT32S EthnetSockStateCheck(INT32S nSock, INT8U* pFlg);
/**************************************************************************************************
                                           相关函数实现
***************************************************************************************************/
/*
*******************************************************************************
* 函数名:EthnetSerTCPSocketCreate
* 输　入:
* 输　出:
* 返  回:
* 功  能: 创建TCP服务器Socket对象
********************************************************************************
*/
static INT8S EthnetSerTCPSocketCreate(void)
{
    BOOLEAN				bFlg			        = FALSE;
    INT32U 				nDetectPort 	        = 5000;
    ENET_SOCKADDR_IN	sEnetSockAddrIn;   
  
    if (g_sEnetStateClr.bEnetCreatSerSockSucc)
	{
		return RET_ERR_OK;
	}
	if (g_sEnetStateClr.bSerSockCliSucc)
	{
        return RET_ERR_OK;
	}
	
	do
	{
	    // 1. 创建服务器socket
	    if ((g_sEnetStateClr.nSerSock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	    {
	    	bFlg = TRUE;
		   	break;
	    }	
	    // 2. 初始化预连接的服务端地址
		//sEnetSockAddrIn.sin_len 		= sizeof(sEnetSockAddrIn);
		sEnetSockAddrIn.sin_family 		= AF_INET;
		sEnetSockAddrIn.sin_port 		= htons(nDetectPort);
		sEnetSockAddrIn.sin_addr.s_addr = INADDR_ANY;
		memset(&(sEnetSockAddrIn.sin_zero), 8, sizeof(sEnetSockAddrIn.sin_zero));		
		// 3. 绑定socket到服务端地址
       // LWIP_ERROR("shankr", 0, 1);
       
		if (bind(g_sEnetStateClr.nSerSock, (ENET_SOCKADDR*)&sEnetSockAddrIn, sizeof(ENET_SOCKADDR)) == -1)
		{
			close(g_sEnetStateClr.nSerSock);
			bFlg = TRUE;
		   	break;
		}			
		// 4. 在socket上进行监听
		if (listen(g_sEnetStateClr.nSerSock, 1) == -1)
		{     
            bFlg = TRUE;
			break;
		}	
	}while(0);
	
    if (bFlg)
	{   
        if (g_sEnetStateClr.nSerSock >= 0)
        {
            close(g_sEnetStateClr.nSerSock);    //就是lwip_close
            g_sEnetStateClr.nSerSock = -1;
        }
		return RET_ERR_ERROR;
	}
	if (RET_ERR_OK != EthnetSetTcpSocketParam(g_sEnetStateClr.nSerSock))
	{
	    close(g_sEnetStateClr.nSerSock);
	    g_sEnetStateClr.nSerSock = -1;
       	return RET_ERR_ERROR;
    }
   
	g_sEnetStateClr.bEnetCreatSerSockSucc = TRUE;
	return RET_ERR_OK;
}

/*
*******************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:RET_ERR_OK可以
* 功  能: 判断服务器是否已经可以接收发送
********************************************************************************
*/
static INT8S EnetSerIsSendOrRecv(void)
{
	u32_t 				nLen 			= 0;
	ENET_SOCKADDR_IN 	sRemoteCliAddr;
	INT8U				nFlg			= 0;

	if (RET_ERR_OK != EthnetSerTCPSocketCreate())
	{
		g_sEnetStateClr.nEnetCreatSerSockCnt++;
		return RET_ERR_ERROR;
	}

	if (!g_sEnetStateClr.bEnetCreatSerSockSucc)
	{
		return RET_ERR_ERROR;
	}
#if 1
	EthnetSockStateCheck(g_sEnetStateClr.nSerSock, &nFlg);
	if (nFlg == 2)
	{
	    g_sEnetStateClr.bEnetRestartFlg = TRUE;	//socket异常，不能接收数据  需要重新以太网 
	    
		return RET_ERR_ERROR;
	}
	else if (0 == nFlg)               				//socket正常，没有数据，不能接收
	{   
		return RET_ERR_OK;
	} 
#endif	
	//if (!g_sEnetStateClr.bSerSockCliSucc)
	{
        if (g_sEnetStateClr.nSerSockCli >= 0)
        {
            close(g_sEnetStateClr.nSerSockCli);
        }
		/* 接受一个客户端连接socket的请求，这个函数调用是阻塞式的 */
		nLen = sizeof(sRemoteCliAddr);
    	g_sEnetStateClr.nSerSockCli =  accept(g_sEnetStateClr.nSerSock, (ENET_SOCKADDR*)&sRemoteCliAddr, &nLen);
		if (RET_ERR_OK != EthnetSetTcpSocketParam(g_sEnetStateClr.nSerSockCli))
		{   
		   // EthnetErrProc(g_sEnetStateClr.nSerSockCli, ETHNET_SRV_COMM, 2); /* BUG */
 		    return RET_ERR_ERROR;
		} 

		g_sEnetStateClr.bSerSockCliSucc = TRUE;
	}
      
	return RET_ERR_OK;
}


/*
*******************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: Enet链路服务器任务初始化
********************************************************************************
*/
static INT32S EthnetSerLinkTaskInit(void *pData)
{
	pData = pData;

	OSTimeDlyHMSM(0, 0, 10, 100);

	return RET_ERR_OK;
}
//extern INT8U		g_nDogEhnet;
/*
*******************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 以太网服务器链路任务（阻塞）
********************************************************************************
*/
static INT32S EnetSerLinkTaskProcStat(void* pData)
{
	//g_nDogEhnet = 0;
	if (g_sEnetStateClr.bEnetRestartFlg)
	{
		return -1;
	}
	// 1. 初始化不成功，返回-1
	if (FALSE == g_sEnetStateClr.bEnetInitSuccFlg)
    {
    	return -1;
    }
	// 2. 客户机模式，返回-1
    if (0 == g_nEthnetMod)
    {
        return -1; 
    }
    // 3. 服务器是否连接成功:  调用的 accept 函数为阻塞型,在此处有可能进入阻塞
	if (RET_ERR_OK != EnetSerIsSendOrRecv())
	{
		return -1;
	}
	// 4. 记录服务器连接成功的sock   
	EthnetSockSet(COMETNETSRV, g_sEnetStateClr.nSerSockCli);	
	return 0;
}

/*
*******************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 以太网服务器链路任务（阻塞）
********************************************************************************
*/
void EthnetLinkSerTask(void *pData)
{
	LinkComTaskProc(COMETNETSRV, pData, EthnetSerLinkTaskInit, EnetSerLinkTaskProcStat, NULL);	
}