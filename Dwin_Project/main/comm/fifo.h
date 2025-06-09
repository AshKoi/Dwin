/******************************************************************************************************
* Copyright (c) 2013
* �ļ���:
* ������: ��һ��
* �ա���: 2013/09/12
* �޸���:
* �ա���:
* �衡��:
* �桡��: Version 1.0
*****************************************************************************************************/
#ifndef FIFO_H
#define FIFO_H
#include "Os_cpu.h"
/**************************************************************************************************
                                           �꼰ö�ٶ���
***************************************************************************************************/
// ������������δ�����ֽ���
#define 	BASEBUF_BYTE_NUM(pBaseBuf)		((pBaseBuf->nRecvPtr + pBaseBuf->nBufSize - pBaseBuf->nProcPtr) % pBaseBuf->nBufSize)
// �����������Ŀ����ֽ���
#define 	BASEBUF_EMPTY_NUM(pBaseBuf)		(pBaseBuf->nBufSize - 1 - BASEBUF_BYTE_NUM(pBaseBuf))
// ����������������λ�ó���
#define 	BASEBUF_2P_NUM(pos1, pos2)		((pos2 + pBaseBuf->nBufSize - pBaseBuf->pos1) % pBaseBuf->nBufSize)

// �����������Ĵ����ָ��ǰ��posλ��
#define 	BASEBUF_PROC_L_PTR(nPos)			((pBaseBuf->nProcPtr + pBaseBuf->nBufSize - (nPos)) % pBaseBuf->nBufSize)
// �����������Ĵ����ָ����posλ��
#define 	BASEBUF_PROC_PTR(nPos)			((pBaseBuf->nProcPtr + (nPos)) % pBaseBuf->nBufSize)
// �����������ľ���ĳ���ֽ�(���ڴ���ָ��)
#define 	BASEBUF_PROC_PBUF(nPos)			(pBaseBuf->pBuf[BASEBUF_PROC_PTR((nPos))])

// �����������Ľ��ջ�ָ��ǰ��posλ��
#define 	BASEBUF_RECV_L_PTR(nPos)			((pBaseBuf->nRecvPtr + pBaseBuf->nBufSize - (nPos)) % pBaseBuf->nBufSize)
// �����������Ľ��ջ�ָ����posλ��
#define 	BASEBUF_RECV_PTR(nPos)			((pBaseBuf->nRecvPtr + (nPos)) % pBaseBuf->nBufSize)
// �����������ľ���ĳ���ֽ�(���ڽ���ָ��)
#define 	BASEBUF_RECV_PBUF(nPos)			(pBaseBuf->pBuf[BASEBUF_RECV_PTR((nPos))])

// ������������posλ�õ�ĳ���ֽ�
#define 	BASEBUF_POS_PBUF(nPos)			(pBaseBuf->pBuf[(nPos) % pBaseBuf->nBufSize])

// ����������Ч���
#define     BASEBUF_INVALID_CHK(pBaseBuf)	((NULL == pBaseBuf) || (NULL == pBaseBuf->pBuf) || (pBaseBuf->nBufSize < 2))
#define     BASEBUF_VALID_CHK(pBaseBuf) 		(!BASEBUF_INVALID_CHK(pBaseBuf))



/**************************************************************************************************
                                           �ṹ����
***************************************************************************************************/
// ��������ṹ
typedef struct _tag_BASE_BUF_SUB
{
	INT8U*				pBuf;							//������    
	INT32U				nBufSize;						//����������
}BASE_BUF_SUB, *pBASE_BUF_SUB;

// ��������ṹ
typedef struct _tag_BASE_BUF
{
	INT8U*				pBuf;
	INT32U				nBufSize;
	INT32U				nRecvPtr;						//����ָ��
	INT32U				nProcPtr;						//����ָ��
}BASE_BUF, *pBASE_BUF;



/******************************************************************************************************
* �ӿ�
*******************************************************************************************************/
/*
*****************************************************************************
* ������: FifoMapBaseBuf
* �䡡��: 
*		INT8U*		pBuf			����������ָ��
*		INT32U		nBufSize		����������
* ��  ��: 
*		pBASE_BUF	pBaseBuf		����������
* ��  ��: 
*		0�ɹ���-1ʧ��
*		
*		
* ��  ��: ����������ӳ��
*****************************************************************************
*/
INT32S	FifoMapBaseBuf(pBASE_BUF pBaseBuf, INT8U* pBuf, INT32U nBufSize);
/*
*****************************************************************************
* ������: FifoInitBaseBuf
* �䡡��: 
*		pBASE_BUF 	pBaseBuf		����������
* ��  ��: 
* ��  ��: 
*		0�ɹ���-1ʧ��
*       
*       
* ��  ��: ��ʼ������
*****************************************************************************
*/
INT32S	FifoInitBaseBuf(pBASE_BUF pBaseBuf);
/*
*****************************************************************************
* ������: FifoMoveBaseBuf
* �䡡��: 
*		pBASE_BUF 	pBaseBuf		����������
*		INT32U 		nShift			�ƶ��ֽ���
*		INT8U 		nDir			�ƶ�����1���ң�0����
* ��  ��: 
* ��  ��: 
*		0�ɹ���-1ʧ��
*       
*       
* ��  ��: 
*****************************************************************************
*/
INT32S	FifoMoveBaseBuf(pBASE_BUF pBaseBuf, INT32U nShift, INT8U nDir);
/*
*****************************************************************************
* ������: FifoMoveLeftBaseBuf
* �䡡��: 
*		pBASE_BUF 	pBaseBuf		����������
*		INT32U 		nShift			�ƶ��ֽ���
* ��  ��: 
* ��  ��: 
*		0�ɹ���-1ʧ��
*       
*       
* ��  ��: 
*****************************************************************************
*/
INT32S	FifoMoveLeftBaseBuf(pBASE_BUF pBaseBuf, INT32U nShift);
/*
*****************************************************************************
* ������: FifoCopyBaseBuf
* �䡡��: 
*		INT8U* 		pDesBuf				Ŀ�껺����
*		INT32U 		nDesSize			Ŀ�껺������С
*		INT32U 		nDesStart			Ŀ�껺������ʼλ��
*		INT8U* 		pSrcBuf				Դ������
*		INT32U 		nSrcSize			Դ��������С
*		INT32U 		nSrcStart			Դ��������ʼλ��
*		INT32U		nCopySize			��������
* ��  ��: 
* ��  ��: 
*		0�ɹ���-1ʧ��
*       
*       
* ��  ��: 
*****************************************************************************
*/
INT32S 	FifoCopyBaseBuf(INT8U* pDesBuf, INT32U nDesSize, INT32U nDesStart, const INT8U* pSrcBuf, INT32U nSrcSize, INT32U nSrcStart, INT32U nCopySize);
/*
*****************************************************************************
* ������: FifoCalcCSBaseBuf
* �䡡��: 
*		const INT8U* 	pBuf		
*		INT32U 			nBufSize
*		INT32U 			nStart
*		INT32U 			nCalcLen
* ��  ��: 
* ��  ��: 
*		
*       
*       
* ��  ��: 
*****************************************************************************
*/
INT8U 	FifoCalcCSBaseBuf(const INT8U* pBuf, INT32U nBufSize, INT32U nStart, INT32U nCalcLen);

#endif
