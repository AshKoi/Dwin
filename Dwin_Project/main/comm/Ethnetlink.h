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
#ifndef ETHNETLINK_H
#define ETHNETLINK_H
#include "linkcom.h"
#include "Global.h"
/**************************************************************************************************
                                           ��غ궨��
***************************************************************************************************/  
#define SOCKET_ERROR                    (-1)
#define MAX_ENET_SOCKET_NUM             3


/**************************************************************************************************
                                           ��ؽṹ����
***************************************************************************************************/
typedef struct sockaddr_in			ENET_SOCKADDR_IN;
typedef struct sockaddr				ENET_SOCKADDR;
typedef fd_set						ENET_FD_SET;
typedef struct timeval				ENET_TIMEVAL;

//��̫��ͨѶ�м�״̬����ʼ������0��
typedef struct _tagENET_STATE_CLR
{	
	BOOLEAN			bEnetRestartFlg;               	//��̫������
	BOOLEAN			bEnetInitSuccFlg;					//��̫����ʼ��״̬��־����̫��ִ�й�����Ҫ�жϴ˱�־
	
	//�ͻ���
	BOOLEAN			bEnetCreatSockSucc;				//�ͻ���socket���ӳɹ���־
	INT16U			nEnetCreatSockCnt;				//�ͻ���socket���Ӵ���
	
	BOOLEAN			bEnetLoginOnSucc;					//��¼�ɹ���־
	INT32S			nSock;								//�ͻ���socket���ӳɹ���socket��
	
	//��������
	BOOLEAN			bEnetCreatSerSockSucc;			//������socket���ӳɹ���־
	INT16U			nEnetCreatSerSockCnt;				//������socket���Ӵ���
	INT32S			nSerSock;							//������socket���ӳɹ���socket��
	INT32S			nSerSockCli;						//������socket���յ��Ŀͻ�������socket��
	BOOLEAN			bSerSockCliSucc;					//������socket���յ��Ŀͻ������ӳɹ���־

    ENET_SOCKADDR_IN	sEnetSockAddrIn;            	//UDP��������

	INT8U			nEthCliFailedCnt;
	INT16U			nEthNoCommCnt;
	ENET_PARAM      tEnetParam;
}ENET_STATE_CLR,*pENET_STATE_CLR;


//��̫��ͨѶ�м�״̬����ʼ������0��
typedef struct _tagENET_STATE_KEEP
{		
    INT8U               nTmp;
}ENET_STATE_KEEP,*pENET_STATE_KEEP;



extern ENET_STATE_CLR			g_sEnetStateClr;
/**************************************************************************************************
                                           ��غ�������
***************************************************************************************************/
void EthnetSockSet(INT8U nCom, INT32S nSock);

INT16U EthnetRecv(INT32S nSock, INT8U* pBuf, INT16U nMaxRdLen, INT32S nFlags, INT8U nType);
INT32S EthnetSend(INT32S nSock, const INT8U* pSendBuf, INT32U nSendLen, INT32S nFlags);

#endif
