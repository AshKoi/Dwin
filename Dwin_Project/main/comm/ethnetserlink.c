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
#include "ethnetserlink.h"
#include "Macro.h"
#include "Base.h"
#include "Linkcom.h"
#include "linkcomExt.h"

/**************************************************************************************************
                                           ���ȫ�ֱ���ʵ��
***************************************************************************************************/
extern INT8U                    g_nEthnetMod;

extern ENET_STATE_CLR			g_sEnetStateClr;
extern ENET_STATE_KEEP	        g_sEnetStateKeep;
extern volatile  INT8U          PortCorrect;
/**************************************************************************************************
                                           ��������
***************************************************************************************************/
extern INT8S        EthnetSetTcpSocketParam(INT32S nSock);
extern void 		EthnetErrProc(INT32S nSock, INT8U nType, INT8U bFlg);
extern INT8U        g_nEthNoCommCnt ;
extern INT32S EthnetSockStateCheck(INT32S nSock, INT8U* pFlg);
/**************************************************************************************************
                                           ��غ���ʵ��
***************************************************************************************************/
/*
*******************************************************************************
* ������:EthnetSerTCPSocketCreate
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ����TCP������Socket����
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
	    // 1. ����������socket
	    if ((g_sEnetStateClr.nSerSock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	    {
	    	bFlg = TRUE;
		   	break;
	    }	
	    // 2. ��ʼ��Ԥ���ӵķ���˵�ַ
		//sEnetSockAddrIn.sin_len 		= sizeof(sEnetSockAddrIn);
		sEnetSockAddrIn.sin_family 		= AF_INET;
		sEnetSockAddrIn.sin_port 		= htons(nDetectPort);
		sEnetSockAddrIn.sin_addr.s_addr = INADDR_ANY;
		memset(&(sEnetSockAddrIn.sin_zero), 8, sizeof(sEnetSockAddrIn.sin_zero));		
		// 3. ��socket������˵�ַ
       // LWIP_ERROR("shankr", 0, 1);
       
		if (bind(g_sEnetStateClr.nSerSock, (ENET_SOCKADDR*)&sEnetSockAddrIn, sizeof(ENET_SOCKADDR)) == -1)
		{
			close(g_sEnetStateClr.nSerSock);
			bFlg = TRUE;
		   	break;
		}			
		// 4. ��socket�Ͻ��м���
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
            close(g_sEnetStateClr.nSerSock);    //����lwip_close
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
* ������:
* �䡡��:
* �䡡��:
* ��  ��:RET_ERR_OK����
* ��  ��: �жϷ������Ƿ��Ѿ����Խ��շ���
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
	    g_sEnetStateClr.bEnetRestartFlg = TRUE;	//socket�쳣�����ܽ�������  ��Ҫ������̫�� 
	    
		return RET_ERR_ERROR;
	}
	else if (0 == nFlg)               				//socket������û�����ݣ����ܽ���
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
		/* ����һ���ͻ�������socket�����������������������ʽ�� */
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
* ������:
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: Enet��·�����������ʼ��
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
* ������:
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ��̫����������·����������
********************************************************************************
*/
static INT32S EnetSerLinkTaskProcStat(void* pData)
{
	//g_nDogEhnet = 0;
	if (g_sEnetStateClr.bEnetRestartFlg)
	{
		return -1;
	}
	// 1. ��ʼ�����ɹ�������-1
	if (FALSE == g_sEnetStateClr.bEnetInitSuccFlg)
    {
    	return -1;
    }
	// 2. �ͻ���ģʽ������-1
    if (0 == g_nEthnetMod)
    {
        return -1; 
    }
    // 3. �������Ƿ����ӳɹ�:  ���õ� accept ����Ϊ������,�ڴ˴��п��ܽ�������
	if (RET_ERR_OK != EnetSerIsSendOrRecv())
	{
		return -1;
	}
	// 4. ��¼���������ӳɹ���sock   
	EthnetSockSet(COMETNETSRV, g_sEnetStateClr.nSerSockCli);	
	return 0;
}

/*
*******************************************************************************
* ������:
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ��̫����������·����������
********************************************************************************
*/
void EthnetLinkSerTask(void *pData)
{
	LinkComTaskProc(COMETNETSRV, pData, EthnetSerLinkTaskInit, EnetSerLinkTaskProcStat, NULL);	
}