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
#ifndef LINK_COM_EXT_H
#define LINK_COM_EXT_H
#include "fifo.h"
#include "linkcom.h"
/**************************************************************************************************
                                           �꼰ö�ٶ���
***************************************************************************************************/
#define HAVE_FRM_BUFF                     		0                                           // ֡����������б��뿪��
	
// һ������ȡ�ֽ���
#define MAX_RECV_ONCE_SIZE					1											// ���߸�Ϊ 32
#define MAX_FRAME_SIZE							(4 * 1024)										// ���֡�ֽ�
#define MAX_RCV_CONTINUE_NUM					(MAX_FRAME_SIZE / MAX_RECV_ONCE_SIZE + 1)


// ÿ��ͨ�ſڵĻ�����������С
#define MAX_RECV_BUF1_SIZE					(MAX_FRAME_SIZE - 2 * 1024 + 100)			// ���ǵ�����ÿ֡����,���Ӧ��COMM.c�ļ����жϽ��ջ�������������Ӧ
#define MAX_RECV_BUF2_SIZE					(MAX_FRAME_SIZE - 2 * 1024 + 100)//1024
#define MAX_RECV_BUFIR_SIZE					1024//256
#define MAX_RECV_BUFZB_SIZE					256

#if 1 == HAVE_FRM_BUFF
#define MAX_FRM_BUFF_SIZE                 	1024
#else
#define MAX_FRM_BUFF_SIZE                     1
#endif

#ifndef ETHNET_CLI_COMM
#define	ETHNET_CLI_COMM			0											//��̫���ͻ�����ʽͨ��
#define	ETHNET_SRV_COMM			1											//��̫����������ʽͨ��
#endif

// gw3761��ԼӦ�ô������С
#define GW3761_SENDBUF_SIZE					1024//512//MAX_FRAME_SIZE

// ͨ�ſڶ���: ���ÿ��Ӳ���ڶ�Ӧ���ײ��ṩ��6��uratӲ���ڶ�Ӧֵ��COM1~COM6��ͬ��
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
                                           ����������
***************************************************************************************************/
// ��·�ṹ�˲���Ч
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
// �������崦��ָ����0����
#define LINK_COM_FRAME_CLR                  {                                 \
    memset(pLinkCom->sFrmBuf.pBuf, 0, pLinkCom->sFrmBuf.nBufSize);              \
    pLinkCom->sFrmInfo.sFrmClrInfo.nBoardAddrFlg = 0;							\
    pLinkCom->nFrmLen = 0;                                                      \
}/////////////////////////////////////////////////////////////////////////////////////

#else
// �������崦��ָ����0����
#define LINK_COM_FRAME_CLR					{                               \
    memset(pLinkCom->sFrmBuf.pBuf, 0, pLinkCom->nFrmLen);                       \
    pLinkCom->sFrmInfo.sFrmClrInfo.nBoardAddrFlg = 0;							\
    pLinkCom->sBaseBuf.nProcPtr = BASEBUF_PROC_PTR(pLinkCom->nFrmLen);        \
    pLinkCom->sFrmBuf.pBuf	  = pLinkCom->sBaseBuf.pBuf;                        \
    pLinkCom->nFrmLen = 0;                                                      \
}/////////////////////////////////////////////////////////////////////////////////////

#endif




/**************************************************************************************************
                                           ������������
***************************************************************************************************/
extern INT8U		g_nGw3761SendBuf[GW3761_SENDBUF_SIZE];
// ������Ϣ�б�
extern LINK_COM			g_sLinkCom[];




/**************************************************************************************************
                                         ��������
***************************************************************************************************/
BOOLEAN IsAysnCom(INT8U nCom);

INT16U ExtReadCommMs(INT8U nCom, INT8U *pBuf, INT16U nMaxLen, INT32U nInter, INT16U nDelay);
INT32S LinkExtRecvProc(INT8U nCom, INT8U* pBuf, INT32S nRdOnce);
INT32S LinkExtSendProc(INT8U nCom, INT8U* pSendbuf, INT32S nSendlen);

void RecvProcExt(INT8U nCom, INT32S nRecvLen);
void WriteProcExtPre(INT8U nCom);
void WriteProcExtBack(INT8U nCom);

#endif

