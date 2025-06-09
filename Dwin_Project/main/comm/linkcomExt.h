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
#ifndef LINK_COM_EXT_H
#define LINK_COM_EXT_H
#include "fifo.h"
#include "linkcom.h"
/**************************************************************************************************
                                           宏及枚举定义
***************************************************************************************************/
#define HAVE_FRM_BUFF                     		0                                           // 帧缓冲独立具有编译开关
	
// 一次最多读取字节数
#define MAX_RECV_ONCE_SIZE					1											// 或者改为 32
#define MAX_FRAME_SIZE							(4 * 1024)										// 最大帧字节
#define MAX_RCV_CONTINUE_NUM					(MAX_FRAME_SIZE / MAX_RECV_ONCE_SIZE + 1)


// 每个通信口的基本缓冲区大小
#define MAX_RECV_BUF1_SIZE					(MAX_FRAME_SIZE - 2 * 1024 + 100)			// 考虑到升级每帧长度,与对应的COMM.c文件的中断接收缓冲区长度相适应
#define MAX_RECV_BUF2_SIZE					(MAX_FRAME_SIZE - 2 * 1024 + 100)//1024
#define MAX_RECV_BUFIR_SIZE					1024//256
#define MAX_RECV_BUFZB_SIZE					256

#if 1 == HAVE_FRM_BUFF
#define MAX_FRM_BUFF_SIZE                 	1024
#else
#define MAX_FRM_BUFF_SIZE                     1
#endif

#ifndef ETHNET_CLI_COMM
#define	ETHNET_CLI_COMM			0											//以太网客户机方式通信
#define	ETHNET_SRV_COMM			1											//以太网服务器方式通信
#endif

// gw3761规约应用处理缓冲大小
#define GW3761_SENDBUF_SIZE					1024//512//MAX_FRAME_SIZE

// 通信口定义: 针对每个硬件口对应。底层提供的6个urat硬件口对应值与COM1~COM6相同。
enum _tag_COM_ID
{
	COM1 = 1,
	COM2,
	COM3,
	COM4,
	COM5,
	COM6,
	COM7,
	COMNUM_VAL,
    COMETNET,
	COMETNETSRV,
	COMWLAN
};
#define	COMNUM 									(COMNUM_VAL - COM1)


/**************************************************************************************************
                                           内联函数宏
***************************************************************************************************/
// 链路结构核查有效
#define LINK_COM_VALID		{									                \
	if (NULL == pLinkCom)			                                            \
	{									                                        \
		return -1;						                                        \
	}									                                        \
	pBaseBuf = &(pLinkCom->sBaseBuf);	                                        \
	if (!BASEBUF_VALID_CHK(pBaseBuf))	                                        \
	{									                                        \
		return -1;						                                        \
	}									                                        \
}/////////////////////////////////////////////////////////////////////////////////////

#if 1 == HAVE_FRM_BUFF
// 基本缓冲处理指针清0处理
#define LINK_COM_FRAME_CLR                  {                                 \
    memset(pLinkCom->sFrmBuf.pBuf, 0, pLinkCom->sFrmBuf.nBufSize);              \
    pLinkCom->sFrmInfo.sFrmClrInfo.nBoardAddrFlg = 0;							\
    pLinkCom->nFrmLen = 0;                                                      \
}/////////////////////////////////////////////////////////////////////////////////////

#else
// 基本缓冲处理指针清0处理
#define LINK_COM_FRAME_CLR					{                               \
    memset(pLinkCom->sFrmBuf.pBuf, 0, pLinkCom->nFrmLen);                       \
    pLinkCom->sFrmInfo.sFrmClrInfo.nBoardAddrFlg = 0;							\
    pLinkCom->sBaseBuf.nProcPtr = BASEBUF_PROC_PTR(pLinkCom->nFrmLen);        \
    pLinkCom->sFrmBuf.pBuf	  = pLinkCom->sBaseBuf.pBuf;                        \
    pLinkCom->nFrmLen = 0;                                                      \
}/////////////////////////////////////////////////////////////////////////////////////

#endif




/**************************************************************************************************
                                           变量声明定义
***************************************************************************************************/
extern INT8U		g_nGw3761SendBuf[GW3761_SENDBUF_SIZE];
// 串口信息列表
extern LINK_COM			g_sLinkCom[];




/**************************************************************************************************
                                         函数声明
***************************************************************************************************/
BOOLEAN IsAysnCom(INT8U nCom);

INT16U ExtReadCommMs(INT8U nCom, INT8U *pBuf, INT16U nMaxLen, INT32U nInter, INT16U nDelay);
INT32S LinkExtRecvProc(INT8U nCom, INT8U* pBuf, INT32S nRdOnce);
INT32S LinkExtSendProc(INT8U nCom, INT8U* pSendbuf, INT32S nSendlen);

void RecvProcExt(INT8U nCom, INT32S nRecvLen);
void WriteProcExtPre(INT8U nCom);
void WriteProcExtBack(INT8U nCom);

#endif

