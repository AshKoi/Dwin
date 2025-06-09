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
                                           ��ر���
***************************************************************************************************/
INT8U					g_nEthnetMod	 = 0;				// 0�ͻ�����1������ 
INT8U					g_nIpPortChgGlg  = 0;

#if 0
typedef struct _tagENET_PARAM{
    INT8U               MACAddr[6];			            /*MAC��ַ*/		
    INT8U               TermIP[4];                      /*�ն�IP��ַ*/
    INT8U               SubnetIP[4];                    /*���������ַ*/
    INT8U               GateIP[4];                      /*���ص�ַ*/
    OS_EVENT	        *semInitLwIP;                   /*�ź���*/
    INT32U              nTimeout;                       /*��ʼ����ʱʱ�䣨�룩: ȡֵ��Χ 10 ~ 5*60*60�룬Ĭ��Ϊ60�� */
    INT8U               nDebugLevel;                    /*���Լ���: 0 - �޵�����Ϣ
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
	{0x20, 0xEF, 0xC6, 0x33, 0x44, 0x01},  // MAC ��ַ
	{192, 168, 0, 	 3},
	{255, 255, 255, 0},
	{192, 168, 0, 	 1},
	NULL,
	10,
	4
};

ENET_STATE_CLR			g_sEnetStateClr;
ENET_STATE_KEEP	        g_sEnetStateKeep;

// ��̫��soket�б����
COM_VALIABLE	g_sComValiable;


/**************************************************************************************************
                                           ��غ���ʵ��
***************************************************************************************************/	
/*
*****************************************************************************
* ������: EthnetSockSet
* �䡡��: 
*		INT8U nCom
* ��  ��: 
* ��  ��: 
*		0�ɹ���-1ʧ��
*		
*		
* ��  ��: �ź���
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
* ������: EthnetAppInit
* �䡡��: 
* ��  ��: 
* ��  ��: 
*		���
*       
*       
* ��  ��: ���õײ�EnetInit����ʼ����̫�����������������bEnetNdRestart��־
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
    //��ֵF7
    //MAC��ַ������Ψһ��ʶ�����֣�
    memcpy(&pEnetParam->MACAddr[0], &g_pMem->sXFramData.nMacAddr[0], 6);
    //�ն�IP
    memcpy(&pEnetParam->TermIP[0], &g_pRomtParm->nTermIp[0], 4);
    //��������
    memcpy(&pEnetParam->SubnetIP[0], &g_pRomtParm->nTerminalMask[0], 4);
    //����
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
* ������:
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ��̫���ͷŴ����־���
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
                close(g_sEnetStateClr.nSerSock);//����lwip_close
        }
        if (g_sEnetStateClr.nSerSockCli >= 0)
        {   
                close(g_sEnetStateClr.nSerSockCli);//����lwip_close
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
                                           TCP���Ӵ������
***************************************************************************************************/
/********************************************************************************
* ������:EthnetSetTcpSocketParam
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ����TCPSocket��������
*********************************************************************************/
INT8S EthnetSetTcpSocketParam(INT32S nSock)
{
	INT32S   nKeepAlive 	= 1;         	//���� KeepAlive ����
	INT32S   nKeepIdle 		= 5;          	//�״�̽�⿪ʼǰ��tcp�������շ�����ʱ��
    INT32S   nKeepInterval 	= 3;      		//ÿ��̽��ļ��ʱ��(��)
   	INT32S   nKeepCount 	= 2;         	//̽�����

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
* ������:EthnetCliTCPSocketCreate
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ����TCP�ͻ���Socket����
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
		// 1. ����һ��socket��������SOCKET_STREAM��TCP����
		if ((g_sEnetStateClr.nSock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			/* ����socketʧ�� */
			bFlg = TRUE;
		   	break;
		}
		// 2. ��sock���ò���
		if (RET_ERR_OK !=  EthnetSetTcpSocketParam(g_sEnetStateClr.nSock))
		{
		    bFlg = TRUE;
		   	break;
		}	

		memcpy(&nIP[0], &g_pRomtParm->nMainIp[0], 4);
		nPort = g_pRomtParm->nMainPort;
		memset(&nTmpBuff[0],0,sizeof(nTmpBuff));
		sprintf((char*)nTmpBuff,"%d.%d.%d.%d", nIP[0], nIP[1], nIP[2], nIP[3]);
		
		/* ��ʼ��Ԥ���ӵķ���˵�ַ */
		sEnetSockAddrIn.sin_len 		= sizeof(sEnetSockAddrIn);
		sEnetSockAddrIn.sin_family 		= AF_INET;
		sEnetSockAddrIn.sin_port 		= htons(nPort);
		sEnetSockAddrIn.sin_addr.s_addr = inet_addr( (char*)nTmpBuff );
		//memset(&(sEnetSockAddrIn.sin_zero), 0, sizeof(sEnetSockAddrIn.sin_zero));
		
	#define ENET_CLI_CONNECT_CNT 1
		for (i = 0; i < ENET_CLI_CONNECT_CNT; i++)
		{
			// 3. ���ӵ������
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
* ������:
* �䡡��:
* �䡡��:
* ��  ��:RET_ERR_OK����
* ��  ��: �жϿͻ����Ƿ��Ѿ����Խ��շ���
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
                                           TCP�����������
***************************************************************************************************/
/********************************************************************************
* ������:
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ��ȡ���е����socket
*********************************************************************************/
static INT32S EthnetGetMaxSocket(void)
{
	return GET_MAX3(g_sEnetStateClr.nSock, g_sEnetStateClr.nSerSockCli, g_sEnetStateClr.nSerSock);
}

/*
*****************************************************************************
* ������: EthnetSockStateCheck
* �䡡��: 
*			INT32S		nSock
* ��  ��: 
*			INT8U* 	pFlg						 // 1ʱsocket�����������ݣ����Խ��գ�
													0ʱsocket������û�����ݣ����ܽ��գ�
													2ʱsocket�쳣�����ܽ���
* ��  ��: 
*       
*       
* ��  ��: ���socket״̬:���ش���0�����������Է��ͻ����
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
            *pFlg = 1;          //socket�����������ݣ����Խ���
        }
        else
		{
			nRet  = 0;
            *pFlg = 0;          //socket������û�����ݣ����ܽ���
		}
	}
    else
    {
        *pFlg = 2;              //socket�쳣�����ܽ���
    }

	return nRet;
}

/*
*****************************************************************************
* ������: EthnetErrProc
* �䡡��: 
*			INT32S		nSock
*			INT8U		nType					//��̫���ͻ���ETHNET_CLI_COMM��������
* ��  ��: 
* ��  ��: 
*       
*       
* ��  ��: 
*****************************************************************************
*/
void EthnetErrProc(INT32S nSock, INT8U nType, INT8U bFlg)
{
     close(nSock);//����lwip_close
    if (ETHNET_CLI_COMM == nType)
	{
		g_sEnetStateClr.nSock               	= -1;
		g_sEnetStateClr.bEnetCreatSockSucc   	= FALSE;
		g_sEnetStateClr.bEnetLoginOnSucc   	= FALSE;
		g_sEnetStateClr.bEnetRestartFlg 		= TRUE;//��Ҫ������̫��
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
				close(g_sEnetStateClr.nSerSock);//����lwip_close
			}
			g_sEnetStateClr.nSerSock            	= -1;
			g_sEnetStateClr.bEnetCreatSerSockSucc	= FALSE;
			g_sEnetStateClr.bEnetRestartFlg 		= TRUE;//��Ҫ������̫��
		}
	}
}




// ���͡����պ���  EthnetSend   EthnetRecv
/*
*****************************************************************************
* ������: EthnetRecv
* �䡡��: 
*			INT32S		nSock
*			INT16U 	nMaxRdLen				// 
*			INT32S		nFlags					
*			INT8U		nType					// ��̫���ͻ�����������
* ��  ��: 
*			INT8U* 	pBuf					// ���ջ�����
* ��  ��: 
*       
*       
* ��  ��: ��̫����·��������
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
            bFlg = TRUE;                   //socket�쳣�����ܽ�������
			break;
        }
        else if (0 == nFlg)               //socket������û�����ݣ����ܽ���
        {   
			break;
        }     
		//socket�����������ݣ����Խ���
        nRecvLen = recv(nSock, pBuf, nMaxRdLen, nFlags);
        if (nRecvLen == 0)				   //���Խ�������ʱ�����ճ���Ϊ0����socket�쳣
        {   
            bFlg = TRUE;                   // socket �Ͽ�
            break;
        }
        else if (nRecvLen < 0)
        {
            bFlg = 2;                   // socket �Ͽ�
            break;
        }
        else
        {    
            g_sEnetStateClr.nEthNoCommCnt 	= 0;            // socket ���������ݵ���
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
* ������:EthnetSend
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��:
*********************************************************************************/
INT32S EthnetSend(INT32S nSock, const INT8U* pSendBuf, INT32U nSendLen, INT32S nFlags)
{
    return send(nSock, pSendBuf, nSendLen, nFlags);
}

/**************************************************************************************************
                                           ��̫��TCP�߳�
***************************************************************************************************/
/*
*****************************************************************************
* ������: EthnetLinkTaskInit
* �䡡��: 
* ��  ��: 
* ��  ��: 
*       
*       
* ��  ��: 
*****************************************************************************
*/
static INT32S EthnetLinkTaskInit(void *pData)
{    
	pData = pData;

	// �����ʼ���ɹ���־
	g_sEnetStateClr.bEnetInitSuccFlg 	= FALSE;
	
    return RET_ERR_OK;
}

/*
*****************************************************************************
* ������: EthnetLinkTaskProcStat
* �䡡��: 
* ��  ��: 
* ��  ��: 
*       
*       
* ��  ��: 
*************s****************************************************************
*/
static INT32S EthnetLinkTaskProcStat(void* pData)
{	
	// 1. ��̫����ʼ�����ͷű�־�жϴ���
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
	// 3. ��·״̬�ж�
	// 3.1. ��ʼ�����ɹ�������-1
    if (FALSE == g_sEnetStateClr.bEnetInitSuccFlg)
    {
    	return -1;
    }
	// 3.2. ������ģʽ
	
    if (1 == g_nEthnetMod)
    {
      	return -1;  
    } 
    
#if 1    
 //   OSTimeDlyHMSM(0, 0, 10, 0);
    /* 3.2 �жϷ�����׼����� */
	if (!g_sEnetStateClr.bEnetCreatSerSockSucc)
    {
    	//return -1;
    } 

	// 3.3. �ͻ����Ƿ����ӳɹ�
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
			if(g_sEnetStateClr.nEthCliFailedCnt >= 2)         /* 1�ְ���������������������̫�� */
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
    
	// 3.4. ��¼�ͻ������ӳɹ���sock
	EthnetSockSet(COMETNET, g_sEnetStateClr.nSock);

    return 0;
}





/*
*****************************************************************************
* ������: EthnetLinkTask
* �䡡��: 
* ��  ��: 
* ��  ��: 
*       
*       
* ��  ��: ��̫����·������������
*****************************************************************************
*/
void EthnetLinkTask(void *pData)
{
	LinkComTaskProc(COMETNET, pData, EthnetLinkTaskInit, EthnetLinkTaskProcStat, NULL);	
}


