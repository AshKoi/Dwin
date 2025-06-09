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
#ifndef ETHNETLINK_H
#define ETHNETLINK_H
#include "linkcom.h"
#include "Global.h"
/**************************************************************************************************
                                           相关宏定义
***************************************************************************************************/  
#define SOCKET_ERROR                    (-1)
#define MAX_ENET_SOCKET_NUM             3


/**************************************************************************************************
                                           相关结构定义
***************************************************************************************************/
typedef struct sockaddr_in			ENET_SOCKADDR_IN;
typedef struct sockaddr				ENET_SOCKADDR;
typedef fd_set						ENET_FD_SET;
typedef struct timeval				ENET_TIMEVAL;

//以太网通讯中间状态（初始化需清0）
typedef struct _tagENET_STATE_CLR
{	
	BOOLEAN			bEnetRestartFlg;               	//以太网重启
	BOOLEAN			bEnetInitSuccFlg;					//以太网初始化状态标志：以太网执行过程需要判断此标志
	
	//客户端
	BOOLEAN			bEnetCreatSockSucc;				//客户端socket链接成功标志
	INT16U			nEnetCreatSockCnt;				//客户端socket链接次数
	
	BOOLEAN			bEnetLoginOnSucc;					//登录成功标志
	INT32S			nSock;								//客户端socket链接成功的socket号
	
	//服务器端
	BOOLEAN			bEnetCreatSerSockSucc;			//服务器socket链接成功标志
	INT16U			nEnetCreatSerSockCnt;				//服务器socket链接次数
	INT32S			nSerSock;							//服务器socket链接成功的socket号
	INT32S			nSerSockCli;						//服务器socket接收到的客户端链接socket号
	BOOLEAN			bSerSockCliSucc;					//服务器socket接收到的客户端链接成功标志

    ENET_SOCKADDR_IN	sEnetSockAddrIn;            	//UDP接收所用

	INT8U			nEthCliFailedCnt;
	INT16U			nEthNoCommCnt;
	ENET_PARAM      tEnetParam;
}ENET_STATE_CLR,*pENET_STATE_CLR;


//以太网通讯中间状态（初始化不清0）
typedef struct _tagENET_STATE_KEEP
{		
    INT8U               nTmp;
}ENET_STATE_KEEP,*pENET_STATE_KEEP;



extern ENET_STATE_CLR			g_sEnetStateClr;
/**************************************************************************************************
                                           相关函数定义
***************************************************************************************************/
void EthnetSockSet(INT8U nCom, INT32S nSock);

INT16U EthnetRecv(INT32S nSock, INT8U* pBuf, INT16U nMaxRdLen, INT32S nFlags, INT8U nType);
INT32S EthnetSend(INT32S nSock, const INT8U* pSendBuf, INT32U nSendLen, INT32S nFlags);

#endif
