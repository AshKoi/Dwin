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
#ifndef FIFO_H
#define FIFO_H
#include "Os_cpu.h"
/**************************************************************************************************
                                           宏及枚举定义
***************************************************************************************************/
// 基本缓冲区的未处理字节数
#define 	BASEBUF_BYTE_NUM(pBaseBuf)		((pBaseBuf->nRecvPtr + pBaseBuf->nBufSize - pBaseBuf->nProcPtr) % pBaseBuf->nBufSize)
// 基本缓冲区的空闲字节数
#define 	BASEBUF_EMPTY_NUM(pBaseBuf)		(pBaseBuf->nBufSize - 1 - BASEBUF_BYTE_NUM(pBaseBuf))
// 基本缓冲区的两点位置长度
#define 	BASEBUF_2P_NUM(pos1, pos2)		((pos2 + pBaseBuf->nBufSize - pBaseBuf->pos1) % pBaseBuf->nBufSize)

// 基本缓冲区的处理基指针前的pos位置
#define 	BASEBUF_PROC_L_PTR(nPos)			((pBaseBuf->nProcPtr + pBaseBuf->nBufSize - (nPos)) % pBaseBuf->nBufSize)
// 基本缓冲区的处理基指针后的pos位置
#define 	BASEBUF_PROC_PTR(nPos)			((pBaseBuf->nProcPtr + (nPos)) % pBaseBuf->nBufSize)
// 基本缓冲区的具体某个字节(基于处理指针)
#define 	BASEBUF_PROC_PBUF(nPos)			(pBaseBuf->pBuf[BASEBUF_PROC_PTR((nPos))])

// 基本缓冲区的接收基指针前的pos位置
#define 	BASEBUF_RECV_L_PTR(nPos)			((pBaseBuf->nRecvPtr + pBaseBuf->nBufSize - (nPos)) % pBaseBuf->nBufSize)
// 基本缓冲区的接收基指针后的pos位置
#define 	BASEBUF_RECV_PTR(nPos)			((pBaseBuf->nRecvPtr + (nPos)) % pBaseBuf->nBufSize)
// 基本缓冲区的具体某个字节(基于接收指针)
#define 	BASEBUF_RECV_PBUF(nPos)			(pBaseBuf->pBuf[BASEBUF_RECV_PTR((nPos))])

// 基本缓冲区的pos位置的某个字节
#define 	BASEBUF_POS_PBUF(nPos)			(pBaseBuf->pBuf[(nPos) % pBaseBuf->nBufSize])

// 基本缓冲有效检查
#define     BASEBUF_INVALID_CHK(pBaseBuf)	((NULL == pBaseBuf) || (NULL == pBaseBuf->pBuf) || (pBaseBuf->nBufSize < 2))
#define     BASEBUF_VALID_CHK(pBaseBuf) 		(!BASEBUF_INVALID_CHK(pBaseBuf))



/**************************************************************************************************
                                           结构定义
***************************************************************************************************/
// 基本缓冲结构
typedef struct _tag_BASE_BUF_SUB
{
	INT8U*				pBuf;							//缓冲区    
	INT32U				nBufSize;						//缓冲区长度
}BASE_BUF_SUB, *pBASE_BUF_SUB;

// 基本缓冲结构
typedef struct _tag_BASE_BUF
{
	INT8U*				pBuf;
	INT32U				nBufSize;
	INT32U				nRecvPtr;						//接收指针
	INT32U				nProcPtr;						//处理指针
}BASE_BUF, *pBASE_BUF;



/******************************************************************************************************
* 接口
*******************************************************************************************************/
/*
*****************************************************************************
* 函数名: FifoMapBaseBuf
* 输　入: 
*		INT8U*		pBuf			缓冲区数组指针
*		INT32U		nBufSize		缓冲区长度
* 输  出: 
*		pBASE_BUF	pBaseBuf		基本缓冲区
* 返  回: 
*		0成功，-1失败
*		
*		
* 功  能: 基本缓冲区映射
*****************************************************************************
*/
INT32S	FifoMapBaseBuf(pBASE_BUF pBaseBuf, INT8U* pBuf, INT32U nBufSize);
/*
*****************************************************************************
* 函数名: FifoInitBaseBuf
* 输　入: 
*		pBASE_BUF 	pBaseBuf		基本缓冲区
* 输  出: 
* 返  回: 
*		0成功，-1失败
*       
*       
* 功  能: 初始缓冲区
*****************************************************************************
*/
INT32S	FifoInitBaseBuf(pBASE_BUF pBaseBuf);
/*
*****************************************************************************
* 函数名: FifoMoveBaseBuf
* 输　入: 
*		pBASE_BUF 	pBaseBuf		基本缓冲区
*		INT32U 		nShift			移动字节数
*		INT8U 		nDir			移动方向：1向右；0向左
* 输  出: 
* 返  回: 
*		0成功，-1失败
*       
*       
* 功  能: 
*****************************************************************************
*/
INT32S	FifoMoveBaseBuf(pBASE_BUF pBaseBuf, INT32U nShift, INT8U nDir);
/*
*****************************************************************************
* 函数名: FifoMoveLeftBaseBuf
* 输　入: 
*		pBASE_BUF 	pBaseBuf		基本缓冲区
*		INT32U 		nShift			移动字节数
* 输  出: 
* 返  回: 
*		0成功，-1失败
*       
*       
* 功  能: 
*****************************************************************************
*/
INT32S	FifoMoveLeftBaseBuf(pBASE_BUF pBaseBuf, INT32U nShift);
/*
*****************************************************************************
* 函数名: FifoCopyBaseBuf
* 输　入: 
*		INT8U* 		pDesBuf				目标缓冲区
*		INT32U 		nDesSize			目标缓冲区大小
*		INT32U 		nDesStart			目标缓冲区起始位置
*		INT8U* 		pSrcBuf				源缓冲区
*		INT32U 		nSrcSize			源缓冲区大小
*		INT32U 		nSrcStart			源缓冲区起始位置
*		INT32U		nCopySize			拷贝长度
* 输  出: 
* 返  回: 
*		0成功，-1失败
*       
*       
* 功  能: 
*****************************************************************************
*/
INT32S 	FifoCopyBaseBuf(INT8U* pDesBuf, INT32U nDesSize, INT32U nDesStart, const INT8U* pSrcBuf, INT32U nSrcSize, INT32U nSrcStart, INT32U nCopySize);
/*
*****************************************************************************
* 函数名: FifoCalcCSBaseBuf
* 输　入: 
*		const INT8U* 	pBuf		
*		INT32U 			nBufSize
*		INT32U 			nStart
*		INT32U 			nCalcLen
* 输  出: 
* 返  回: 
*		
*       
*       
* 功  能: 
*****************************************************************************
*/
INT8U 	FifoCalcCSBaseBuf(const INT8U* pBuf, INT32U nBufSize, INT32U nStart, INT32U nCalcLen);

#endif
