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
#ifndef LINK_COM_H
#define LINK_COM_H
#include "fifo.h"
/**************************************************************************************************
                                           结构定义
***************************************************************************************************/
// 通用变量
typedef struct _tagCOM_VALIABLE
{
	INT32S					nSock;							//客户端socket链接成功的socket号
	INT32S					nSerSockCli;					//服务器socket接收到的客户端链接socket号
}COM_VALIABLE, *pCOM_VALIABLE;

// 规约接收找帧
typedef INT32S (*pLINK_COM_CHK_PACKAGE)(void* pLinkCom);
// 具体处理执行帧
typedef INT32S (*pLINK_COM_PROC)(void* pLinkCom);

// 帧处理信息
typedef struct _tag_FRM_CLR_INFO
{
	INT16U					nRcvContinueCnt;				//连续接收不到清0
	INT8U                   nBoardAddrFlg;                //广播地址
}FRM_CLR_INFO, *pFRM_CLR_INFO;

// 帧处理信息
typedef struct _tag_FRM_INFO
{
	// 下面初始化时需清0 
	FRM_CLR_INFO			sFrmClrInfo;					//	
}FRM_INFO, *pFRM_INFO;

//通信口帧处理信息结构
typedef struct _tag_LINK_COM
{
	INT8U					nCom;
	
    BASE_BUF				sBaseBuf;					    //基本缓冲区(接收缓冲区)
    BASE_BUF_SUB			sFrmBuf;						//帧缓冲区
	INT32U					nFrmLen;					    //帧缓冲区长度(不为0时表示已找到完整帧)
	
	FRM_INFO				sFrmInfo;						//帧信息
	
	void*               	plinkComChkPackage;		       //规约包检查
	void*       			plinkComProc;					//规约具体处理

	void**                  ppEvent;                       //事件控制块指针
}LINK_COM, *pLINK_COM;




// 具体初始化函数
typedef INT32S (*pLINK_COM_INIT_FUNC)(void* pData);
// 
typedef INT32S (*pLINK_COM_STAT_FUNC)(void* pData);
// 函数
typedef INT32S (*pLINK_COM_TAIL_FUNC)(void* pData);


/**************************************************************************************************
                                           相关函数声明(接口)
***************************************************************************************************/
/*
*****************************************************************************
* 函数名: LinkComTaskProc
* 输　入: 
*		INT8U 						nCom							通信口
*		void* 						pData							进程参数
*		pLINK_COM_INIT_FUNC 		pLinkComInitFunc				进程初始化函数指针
*		pLINK_COM_STAT_FUNC 		pLinkComStatFunc				进程状态刷新函数指针
*		pLINK_COM_TAIL_FUNC 		pLinkComTailFunc				一轮结尾执行函数函数指针
* 输  出: 
* 返  回: 
*		0成功，-1失败
*       
*       
* 功  能: 线程
*****************************************************************************
*/
INT32U 		LinkComTaskProc(INT8U nCom, void* pData, pLINK_COM_INIT_FUNC pLinkComInitFunc, pLINK_COM_STAT_FUNC pLinkComStatFunc, pLINK_COM_TAIL_FUNC pLinkComTailFunc);
/*
*****************************************************************************
* 函数名: LinkComSend
* 输　入: 
*		INT8U 						nCom							通信口
*		INT8U* 						pSendbuf
*		INT16U 						nSendlen
* 输  出: 
* 返  回: 
*		发送成功字节数，-1失败      
* 功  能: 发送数据
*****************************************************************************
*/
INT32S 		LinkComSend(INT8U nCom, INT8U* pSendbuf, INT16U nSendlen);
/*
*****************************************************************************
* 函数名: COM_POST_SEM
* 输　入: 
*		INT8U nCom
* 输  出: 
* 返  回: 
*       
*       
* 功  能: urat信号量
*****************************************************************************
*/
void 		COM_POST_SEM(INT8U nCom);
/*
*****************************************************************************
* 函数名: COM_PEND_SEM
* 输　入: 
*		INT8U nCom
* 输  出: 
* 返  回: 
*       
*       
* 功  能: urat信号量
*****************************************************************************
*/
void	 	COM_PEND_SEM(INT8U nCom);


#endif
