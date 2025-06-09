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
#ifndef LINK_COM_H
#define LINK_COM_H
#include "fifo.h"
/**************************************************************************************************
                                           �ṹ����
***************************************************************************************************/
// ͨ�ñ���
typedef struct _tagCOM_VALIABLE
{
	INT32S					nSock;							//�ͻ���socket���ӳɹ���socket��
	INT32S					nSerSockCli;					//������socket���յ��Ŀͻ�������socket��
}COM_VALIABLE, *pCOM_VALIABLE;

// ��Լ������֡
typedef INT32S (*pLINK_COM_CHK_PACKAGE)(void* pLinkCom);
// ���崦��ִ��֡
typedef INT32S (*pLINK_COM_PROC)(void* pLinkCom);

// ֡������Ϣ
typedef struct _tag_FRM_CLR_INFO
{
	INT16U					nRcvContinueCnt;				//�������ղ�����0
	INT8U                   nBoardAddrFlg;                //�㲥��ַ
}FRM_CLR_INFO, *pFRM_CLR_INFO;

// ֡������Ϣ
typedef struct _tag_FRM_INFO
{
	// �����ʼ��ʱ����0 
	FRM_CLR_INFO			sFrmClrInfo;					//	
}FRM_INFO, *pFRM_INFO;

//ͨ�ſ�֡������Ϣ�ṹ
typedef struct _tag_LINK_COM
{
	INT8U					nCom;
	
    BASE_BUF				sBaseBuf;					    //����������(���ջ�����)
    BASE_BUF_SUB			sFrmBuf;						//֡������
	INT32U					nFrmLen;					    //֡����������(��Ϊ0ʱ��ʾ���ҵ�����֡)
	
	FRM_INFO				sFrmInfo;						//֡��Ϣ
	
	void*               	plinkComChkPackage;		       //��Լ�����
	void*       			plinkComProc;					//��Լ���崦��

	void**                  ppEvent;                       //�¼����ƿ�ָ��
}LINK_COM, *pLINK_COM;




// �����ʼ������
typedef INT32S (*pLINK_COM_INIT_FUNC)(void* pData);
// 
typedef INT32S (*pLINK_COM_STAT_FUNC)(void* pData);
// ����
typedef INT32S (*pLINK_COM_TAIL_FUNC)(void* pData);


/**************************************************************************************************
                                           ��غ�������(�ӿ�)
***************************************************************************************************/
/*
*****************************************************************************
* ������: LinkComTaskProc
* �䡡��: 
*		INT8U 						nCom							ͨ�ſ�
*		void* 						pData							���̲���
*		pLINK_COM_INIT_FUNC 		pLinkComInitFunc				���̳�ʼ������ָ��
*		pLINK_COM_STAT_FUNC 		pLinkComStatFunc				����״̬ˢ�º���ָ��
*		pLINK_COM_TAIL_FUNC 		pLinkComTailFunc				һ�ֽ�βִ�к�������ָ��
* ��  ��: 
* ��  ��: 
*		0�ɹ���-1ʧ��
*       
*       
* ��  ��: �߳�
*****************************************************************************
*/
INT32U 		LinkComTaskProc(INT8U nCom, void* pData, pLINK_COM_INIT_FUNC pLinkComInitFunc, pLINK_COM_STAT_FUNC pLinkComStatFunc, pLINK_COM_TAIL_FUNC pLinkComTailFunc);
/*
*****************************************************************************
* ������: LinkComSend
* �䡡��: 
*		INT8U 						nCom							ͨ�ſ�
*		INT8U* 						pSendbuf
*		INT16U 						nSendlen
* ��  ��: 
* ��  ��: 
*		���ͳɹ��ֽ�����-1ʧ��      
* ��  ��: ��������
*****************************************************************************
*/
INT32S 		LinkComSend(INT8U nCom, INT8U* pSendbuf, INT16U nSendlen);
/*
*****************************************************************************
* ������: COM_POST_SEM
* �䡡��: 
*		INT8U nCom
* ��  ��: 
* ��  ��: 
*       
*       
* ��  ��: urat�ź���
*****************************************************************************
*/
void 		COM_POST_SEM(INT8U nCom);
/*
*****************************************************************************
* ������: COM_PEND_SEM
* �䡡��: 
*		INT8U nCom
* ��  ��: 
* ��  ��: 
*       
*       
* ��  ��: urat�ź���
*****************************************************************************
*/
void	 	COM_PEND_SEM(INT8U nCom);


#endif
