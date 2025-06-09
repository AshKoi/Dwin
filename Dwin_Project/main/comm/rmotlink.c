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
#include "rmotlink.h"
#include "AppCfg.h"
#include "Version.h"
#include "Base.h"
#include "SysRes.h"
#include "linkcom.h"
#include "linkcomExt.h"
#include "Guiwin.h"
#include "Guimenu.h"
#include "key.h"
#include "Fun.h"
#include "TimerCount.h"
#include "SoftTimer.h"
#include "Gw3761.h"


/**************************************************************************************************
                                           ��
***************************************************************************************************/



/**************************************************************************************************
                                           ����
***************************************************************************************************/
// LINK_CLNT���ͻ�����LINK_SRVC��������
static INT8U				g_nLinkType = 0;		
// �ͻ���
static INT8U				g_nClntCommBuf[1440] 		= {0};			
static INT8U				g_nClntCommBuf2[1440] 		= {0};	

// ������
static INT8U				g_nSrvcCommBuf[1440] 		= {0};
static INT8U				g_nSrvcCommBuf2[1440] 		= {0};

// rmotȫ�ֻ���:�������ݶ�Ҫ�Դ�ŵ��˻���
static INT8U				g_nTotalCommBuf[2500] 	= {0};




// ��������BASE_BUF���ܸĶ�
static BASE_BUF				g_sClntBaseBuf 			= {
		&g_nClntCommBuf[0],
		sizeof(g_nClntCommBuf),
		0,
		0
};
static BASE_BUF				g_sSrvcBaseBuf 			= {
		&g_nSrvcCommBuf[0],
		sizeof(g_nSrvcCommBuf),
		0,
		0
};
static BASE_BUF				g_sTotalBaseBuf 			= {
		&g_nTotalCommBuf[0],
		sizeof(g_nTotalCommBuf),
		0,
		0
};

static BASE_BUF				g_sClntBaseBuf2 			= {
		&g_nClntCommBuf2[0],
		sizeof(g_nClntCommBuf2),
		0,
		0
};
static BASE_BUF				g_sSrvcBaseBuf2 			= {
		&g_nSrvcCommBuf2[0],
		sizeof(g_nSrvcCommBuf2),
		0,
		0
};





static INT32S				g_nTotalRmotBufPtr	= 0;
static INT32S				g_nTotalRmotBufLen	= 0;



// Ĭ��ͨ�Ų���
static RMOT_PARM			g_sRomtParm 			= {
				 		"jsepc.js"//"cmnet"                            //apn
						,{10, 101, 1, 185}               //��ip
						,5600//,5200
						,{10, 101, 1, 187}               //��ip
						,5600//,5200
						,{0, 0, 0, 0}               		//�ն�ip
                        ,{0, 0, 0, 0}                       //��������
                        ,{0, 0, 0, 0}                       //����
						,5100
						,""									//�û���
						,""

						,0									//Tcp
                        ,1									//���
                        ,1                                  //��������
						,600								//�ز�
						,180								//����
						,120								//��ͨ�ŵ���
						,30									//��ʱ
};
pRMOT_PARM					g_pRomtParm 			= &g_sRomtParm;
// �м����
static RMOT_VAL_CLR			g_sRmotValClr;
pRMOT_VAL_CLR				g_pRmotValClr			= &g_sRmotValClr;

static INT8U				g_nModlType			= MODL_TELIT;//ģ������
INT16U						g_nLoginCnt			= 0;	//���Ŵ���


// ģ������Ӧ������
static const INT32U    		g_dwBaudRate[] = {
						UART_BPS_9600,
						UART_BPS_57600,			
};



//ģ�����õ�ͨ��AT������
const 	char 	g_Modl_OK[]           = "NJ\x0c\x09";					//OK\x0D\x0A
const 	char 	g_Modl_ERROR[]        = "DQQNQ";					    //ERROR
const   char    g_Modl_CONNECT[]      = "BNMMDBS";			        	//"CONNECT"
const 	char 	g_Modl_0D0A[]         = "\x0c\x09";				    //\x0D\x0A
const 	char 	g_Modl_TELIT[]        = "221";						    //Telit
const 	char 	g_Modl_NEO[]          = "MDNV@X";					    //NEOWAY


/**************************************************************************************************
                                           ����
***************************************************************************************************/
extern INT32S TelitProc(pBASE_BUF pBaseBuf, pBASE_BUF pBaseBuf1,pBASE_BUF pBaseBuf2,INT8U nLinkType);
extern void TltTcpSend(INT8U nId, INT8U* pData, INT32S nSize);
extern INT8U  nMessageFlg;

/**************************************************************************************************
                                           ��غ���ʵ��
***************************************************************************************************/
#if 0
	/*
		1��Զ��ͨѶ�Ȱ����ݽ��յ�g_sTotalBaseBuf������
	*/
#endif
/*
*****************************************************************************
* ������: BaseBufRecv
* �䡡��: 
*			INT8U*		pBuf				������
*			INT32S		nLen				����
* ��  ��: 
* ��  ��: 
*		
*		
*		
* ��  ��: ��pBuf�����ݴ���pBaseBuf
*****************************************************************************
*/
static INT32S BaseBufRecv(pBASE_BUF pBaseBuf, INT8U* pBuf, INT32S nLen)
{
	INT32S	i = 0;

	if (NULL == pBaseBuf)
	{
		return -1;
	}
	if (NULL == pBuf)
	{
		return -1;
	}
	if (nLen <= 0)
	{
		return -1;
	}
	
	for (i = 0; i < nLen; i++)
	{
		if (BASEBUF_RECV_PTR(1) == pBaseBuf->nProcPtr)
		{
			pBaseBuf->nProcPtr = BASEBUF_PROC_PTR(1);							// ����ָ������ƶ�1λ
		}
		BASEBUF_RECV_PBUF(0) = pBuf[i]; 							// (���ڽ���ָ���)���������ݽ���
		
		pBaseBuf->nRecvPtr = BASEBUF_RECV_PTR(1);								// ����ָ������ƶ�1λ
	}

	return 0;
}

/*
*****************************************************************************
* ������: RmotTotalBufRecv
* �䡡��: 
*			INT8U*		pBuf				������
*			INT32S		nRdLen				��ȡ����
* ��  ��: 
* ��  ��: 
*		
*		
*		
* ��  ��: 
*****************************************************************************
*/
static INT32S RmotTotalBufRecv(pBASE_BUF pBaseBuf)
{
	INT32S		i			= 0;
	INT8U		nTmpBuf[MAX_RECV_ONCE_SIZE] = {0};
	INT32S		nRcvLen 	= 0;
	INT32S		nRet		= 0;

	if (NULL == pBaseBuf)
	{
		return -1;
	}
	for (i = 0; i < (pBaseBuf->nBufSize / MAX_RECV_ONCE_SIZE) + 1; i++)
	{
		memset(&nTmpBuf[0], 0, MAX_RECV_ONCE_SIZE);
		nRcvLen = ExtReadCommMs(COMM_GPRS, &nTmpBuf[0], MAX_RECV_ONCE_SIZE, 20, 100);
		if (nRcvLen <= 0)
		{   
            if (nRet > 0)
            {
                OSTimeDlyHMSM(0, 0, 0, 150);
                memset(&nTmpBuf[0], 0, MAX_RECV_ONCE_SIZE);
                nRcvLen = ExtReadCommMs(COMM_GPRS, &nTmpBuf[0], MAX_RECV_ONCE_SIZE, 20, 100);
                if (nRcvLen <= 0)
                {
                    break;
                }
            }
            else
            {
                break;
            }
		}
        if (GetRmotCmdPrintFlg())
        {
            Gw3761FrmWrite(COMM_WH232, &nTmpBuf[0], nRcvLen);
        }
		BaseBufRecv(pBaseBuf, &nTmpBuf[0], nRcvLen);
		nRet += nRcvLen;

		if (nRet >= pBaseBuf->nBufSize)
		{
			break;
		}
	}

	return nRet;
}

/*
*****************************************************************************
* ������: UseCurrBaseBuf
* �䡡��: 
*			pBASE_BUF		pBaseBuf				g_sLinkCom������BASE_BUF
*			INT8U			nLinkType				LINK_CLNT���ͻ�����LINK_SRVC��������
* ��  ��: 
* ��  ��: 
*		
*		
*		
* ��  ��: ѡ��ǰʹ�õ�BASE_BUF
*****************************************************************************
*/
static INT32S	UseCurrBaseBuf(pBASE_BUF pBaseBuf, INT8U nLinkType)
{
	if (NULL == pBaseBuf)
	{
		return -1;
	}
	if (LINK_CLNT == nLinkType)
	{
		memcpy(pBaseBuf, &g_sClntBaseBuf, sizeof(BASE_BUF));
	}
	else
	{
		memcpy(pBaseBuf, &g_sSrvcBaseBuf, sizeof(BASE_BUF));	
	}

	return 0;
}

/*
*****************************************************************************
* ������: SaveCurrBaseBuf
* �䡡��: 
*			pBASE_BUF		pBaseBuf				g_sLinkCom������BASE_BUF
* ��  ��: 
* ��  ��: 
*		
*		
*		
* ��  ��: ���浱ǰʹ�õ�BASE_BUF
*****************************************************************************
*/
static INT32S	SaveCurrBaseBuf(pBASE_BUF pBaseBuf)
{
	if (NULL == pBaseBuf)
	{
		return -1;
	}
	if ((INT32U)(pBaseBuf->pBuf) == (INT32U)(g_sClntBaseBuf.pBuf))
	{
		g_sClntBaseBuf.nProcPtr = pBaseBuf->nProcPtr;
		g_sClntBaseBuf.nRecvPtr = pBaseBuf->nRecvPtr;
	}
	else
	{
		g_sSrvcBaseBuf.nProcPtr = pBaseBuf->nProcPtr;
		g_sSrvcBaseBuf.nRecvPtr = pBaseBuf->nRecvPtr;
	}
	return 0;
}

/*
*****************************************************************************
* ������: GetTotalBaseBufPtr
* �䡡��: 
*			pBASE_BUF		pBaseBuf				g_sLinkCom������BASE_BUF
*			INT8U			nLinkType				LINK_CLNT���ͻ�����LINK_SRVC��������
* ��  ��: 
* ��  ��: 
*		
*		
*		
* ��  ��: ѡ��ǰʹ�õ�BASE_BUF
*****************************************************************************
*/
INT32S	GetTotalBaseBufPtr(INT32U* pProcPtr, INT32U* pRecvPtr)
{
	if (NULL == pProcPtr)
	{
		return -1;
	}
	if (NULL == pRecvPtr)
	{
		return -1;
	}
	*pProcPtr = g_sTotalBaseBuf.nProcPtr;
	*pRecvPtr = g_sTotalBaseBuf.nRecvPtr;
	return 0;
}




#if 0
/*
	Զ�̲���
*/
#endif
/*
*****************************************************************************
* ������: RmotParamTrans
* �䡡��: 
* ��  ��: 
* ��  ��: 
*		
*		
*		
* ��  ��: �ѹ�Լ��Ļ�������ת��ΪԶ��ͨѶ����  ---->  g_pRomtParm
*****************************************************************************
*/
INT32S RmotParamTrans(void)
{	
	pRMOT_PARM 	pRmotParm 	= g_pRomtParm;
	INT8U		nErr		= 0;
	INT8U		nBuf[100]  = {0};
	INT32S		nDataSize	= sizeof(nBuf);
	INT32S		nLen		= sizeof(nBuf);
	INT8U*		pData		= &nBuf[0];
		
	if (NULL == pData)
	{
		return -1;
	}
	if (nDataSize < 0)
	{
		return -1;
	}
	OSSemPend(g_pSem[SEM_RMOT_PARM], 0, &nErr);
	//+++++
    //F3��վIP��ַ�Ͷ˿�
	memset(pData, 0, nDataSize);
	if (MemReadCommPrm(F3, pData, nLen) >= 0)
    {
        memcpy(&pRmotParm->nMainIp[0], &pData[0], 4);
        pRmotParm->nMainPort = Buf2INT16U(&pData[4]);
        memcpy(&pRmotParm->nBakIp[0], &pData[6], 4);
        pRmotParm->nBakPort = Buf2INT16U(&pData[10]);
        memcpy(&pRmotParm->nApn, &pData[12], LEN_APN);
    }
	
    //F1ͨѶ����
    memset(pData, 0, nDataSize);
	if (MemReadCommPrm(F1, pData, nLen) > 0)
    {
        pRmotParm->nBeatIntvl = pData[5] * 60;  //��������
    }
    
    //F7 �ն�IP��ַ�Ͷ˿�
    memset(pData, 0, nDataSize);
	if (MemReadCommPrm(F7, pData, nLen) > 0)
    {
        //�ն�IP
        memcpy(&pRmotParm->nTermIp[0], &pData[0], 4);
        //��������
        memcpy(&pRmotParm->nTerminalMask[0], &pData[4], 4);
        //����
        memcpy(&pRmotParm->nTerminalGate[0], &pData[8], 4);
        //֡���˿�
        pRmotParm->nTermPort = Buf2INT16U(&pData[62]); 
    }
    
    
    //F8�ն�����ͨ�Ź�����ʽ
    memset(pData, 0, nDataSize);
	if (MemReadCommPrm(F8, pData, nLen) > 0)
    {
        pRmotParm->nNetProt = (pData[0] & 0x80) >> 7;        //TCP\UDP 0��TCP 1:UDP
        pRmotParm->nWorkMode = (pData[0] & 0x30) >> 4;       //����ģʽ��0��2���α�ʾ���ģʽ���ͻ���ģʽ��������ģʽ
        pRmotParm->nOnlineType = (pData[0] & 0x03);          //����ģʽ������ģʽ1��3���α�ʾ��������ģʽ����������ģʽ��ʱ������ģʽ��
        pRmotParm->nLoginIntvl = Buf2INT16U(&pData[1]);      //�ز����
    }
    
	
	OSSemPost(g_pSem[SEM_RMOT_PARM]);
	
	return 0;
}

/*
*****************************************************************************
* ������: RmotBufValidDataInfoMake
* �䡡��: 
*			INT32S 				nPtr				��total��������Ŀͻ���/��������Ч���ݵ���ʼ�±�				
*			INT32S				nLen				���ݳ���
* ��  ��: 
* ��  ��: 
*		
*		
*		
* ��  ��: �ڻ�ȡ��RCV(S)������ݺ����
*****************************************************************************
*/
void RmotBufValidDataInfoMake(INT32S nPtr, INT32S nLen)
{
	g_nTotalRmotBufPtr = nPtr;
	g_nTotalRmotBufLen = nLen;
}

/*
*****************************************************************************
* ������: RmotAppBufLenGet
* �䡡��: 
*			INT8U				nLinkType							
* ��  ��: 
* ��  ��: 
*		
*		
*		
* ��  ��: ��ȡ�ͻ�����������Ļ��峤��
*****************************************************************************
*/
INT32S RmotAppBufLenGet(INT8U nLinkType)
{
	INT32S nRet = -1;
	
	if (LINK_CLNT == g_nLinkType)
	{
		nRet = sizeof(g_nClntCommBuf);
	}
	else
	{
		nRet = sizeof(g_nSrvcCommBuf);
	}

	return nRet;
}


/*
*****************************************************************************
* ������: RmotRecv
* �䡡��: 
*			INT8U*		pBuf				������
*			INT32S		nRdLen				��ȡ����
* ��  ��: 
* ��  ��: 
*		
*		
*		
* ��  ��: ��ȡ�ͻ������߷�����������  ��linkcom����
*****************************************************************************
*/
INT32S RmotRecv(INT8U* pBuf, INT32S nRdLen)
{
	//pBASE_BUF  	pBaseBuf	= &g_sTotalBaseBuf;
	pBASE_BUF  	pBaseBuf   = NULL;
	INT32U		nSize		= 0;//pBaseBuf->nBufSize;
	INT32S		i 			= 0;
	INT32S		nValidLen	= 0;
	INT32S		nTmp		= 0;
	
	if (NULL == pBuf)
	{
		return -1;
	}
	if (nRdLen < 0)
	{
		return -1;
	}
	else if (nRdLen == 0)
	{
		return 0;
	}
    
    if (LINK_CLNT == g_nLinkType)
	{
		pBaseBuf = &g_sClntBaseBuf2;
	}
	else
	{
		pBaseBuf = &g_sSrvcBaseBuf2;
	}
    nSize	= pBaseBuf->nBufSize;

	if (FALSE == ChkRange(pBaseBuf->nRecvPtr, 0, nSize - 1))
	{
		pBaseBuf->nRecvPtr = 0;
	}
	if (FALSE == ChkRange(pBaseBuf->nProcPtr, 0, nSize - 1))
	{
		pBaseBuf->nProcPtr = 0;
	}

	/*if (pBaseBuf->nProcPtr == pBaseBuf->nRecvPtr)
	{
		g_nTotalRmotBufPtr = 0;
		g_nTotalRmotBufLen = 0;
		return 0;
	}*/
	
	nTmp = (pBaseBuf->nRecvPtr + nSize - pBaseBuf->nProcPtr) % nSize;
	nValidLen = RmotAppBufLenGet(g_nLinkType);
	/*if (FALSE == ChkRange(g_nTotalRmotBufPtr, 0, nSize - 1))
	{
		return -1;
	}
	if (FALSE == ChkRange(g_nTotalRmotBufLen, 0, nValidLen - 1))
	{
		return -1;
	}

	for (i = 0; i < GET_MIN(nRdLen, g_nTotalRmotBufLen); i++)
	{
		pBuf[i] = BASEBUF_POS_PBUF(g_nTotalRmotBufPtr);
		g_nTotalRmotBufPtr = (g_nTotalRmotBufPtr + 1) % nSize;
		g_nTotalRmotBufLen--;
	}*/
	for (i = 0; i < GET_MIN(GET_MIN(nRdLen, nTmp), nValidLen); i++)
	{
		pBuf[i] = pBaseBuf->pBuf[pBaseBuf->nProcPtr % pBaseBuf->nBufSize];
        //pBaseBuf->pBuf[pBaseBuf->nProcPtr % pBaseBuf->nBufSize] = 0;
		pBaseBuf->nProcPtr = (pBaseBuf->nProcPtr + 1) % pBaseBuf->nBufSize;
	}

	return i;
}

/*
*****************************************************************************
* ������: RmotSend
* �䡡��: 
*			INT8U*		pBuf				������
*			INT32S		nWrLen				��ȡ����
* ��  ��: 
* ��  ��: 
*		
*		
*		
* ��  ��: ���Ϳͻ������߷�����������  ��linkcom����
*****************************************************************************
*/
INT32S RmotSend(INT8U* pBuf, INT32S nWrLen)
{
	if (MODL_TELIT == g_nModlType)
	{
		if (LINK_CLNT == g_nLinkType)
		{
			TltTcpSend(1, pBuf, nWrLen);
		}
		else
		{
			TltTcpSend(2, pBuf, nWrLen);
		}
	}

	return 0;
}



#if 0
/*
	rmotbase����
*/
#endif
/*
*****************************************************************************
* ������: BaseBufChkLabel
* �䡡��: 
*			pBASE_BUF 	pBaseBuf	
*			INT32S		nPosBase					0ʱ����nProcPtr��ʼ����
*			INT8U* 		pLabel	
*			INT8U		nVal						
*			INT8U		nClr						1ʱ�������ǩ
* ��  ��: 
* ��  ��: 
*		���صĹؼ�����ʼλ��
*		
*		
* ��  ��: ����ǩ
*****************************************************************************
*/
INT32S BaseBufChkLabel(pBASE_BUF pBaseBuf, INT32S nPosBase, INT8U* pLabel, INT8U nVal, INT8U nClr)
{
	INT8U	nLabelLen 		= 0;
	INT32S	i				= 0;
	INT32S	j				= 0;
	INT32S	nValidDataLen	= 0;
	INT32S	nPos			= 0;

	if (NULL == pLabel)
	{
		return -1;
	}
	if (NULL == pBaseBuf)
	{
		return -1;
	}
	if (NULL == pBaseBuf->pBuf)
	{
		return -1;
	}
	if (0 == pBaseBuf->nBufSize)
	{
		return -1;
	}
	if (!ChkRange(pBaseBuf->nProcPtr, 0, pBaseBuf->nBufSize - 1))
	{
		return -1;
	}
	if (!ChkRange(pBaseBuf->nRecvPtr, 0, pBaseBuf->nBufSize - 1))
	{
		return -1;
	}
	if (pBaseBuf->nRecvPtr == pBaseBuf->nProcPtr)
	{
		return -1;
	}
	nValidDataLen = BASEBUF_BYTE_NUM(pBaseBuf);

	nLabelLen = strlen((char*)pLabel);
	if (nValidDataLen < nLabelLen + nPosBase)
	{
		return -1;
	}
	
	for (i = 0; i <= nValidDataLen - nLabelLen - nPosBase; i++)
	{
		for (j = 0; j < nLabelLen; j++)
		{
			nPos = nPosBase + i + j;
			if (BASEBUF_PROC_PBUF(nPos) != pLabel[j] + nVal)
			{
				break;
			}
		}
		if (j >= nLabelLen)
		{		
			// ����ʾ��
			if (nClr)
			{
				for (j = 0; j < nLabelLen; j++)
				{
					nPos = nPosBase + i + j;
					BASEBUF_PROC_PBUF(nPos) = 0;
				}
			}
			//��׼�±�(��procptrΪ��׼)
			nPos = nPosBase + i;
			//return BASEBUF_PROC_PTR(nPos);
            return nPos;
		}
	}

	return -1;
}


/*
*****************************************************************************
* ������: SendAtData
* �䡡��: 
* ��  ��: 
* ��  ��: 
*       
*       
* ��  ��: ����
*****************************************************************************
*/
static void SendAtData(const INT8U *pData,INT16U nLen)
{
	if (NULL == pData)
	{
		return;		
	}
	if (0 == nLen)
	{
		return;		
	}
	
	WriteComm(COMM_GPRS,(INT8U*)(pData),nLen);
	if (GetRmotCmdPrintFlg())
	{
		Gw3761FrmWrite(COMM_WH232, (INT8U*)(pData), nLen);
	}
}

/*
*****************************************************************************
* ������: SendAtOrder
* �䡡��: 
* ��  ��: 
* ��  ��: 
*       
*       
* ��  ��: AT����
*****************************************************************************
*/
static void SendAtOrder(const char *pcDataAddr)
{
	INT8U	nSendLen = 0;
	if (NULL == pcDataAddr)
	{
		return;		
	}

	nSendLen = strlen(pcDataAddr);
	if (0 == nSendLen)
	{
		return;	
	}

	SendAtData((INT8U*)pcDataAddr,nSendLen);
}



/*
*****************************************************************************
* ������: WaitATResp
* �䡡��: 

*					nType    0ʱ,�ҵ��ؼ��֣��Թؼ���λ��Ϊ��ʼ���洢���յ������ݣ�1ʱ,�ҵ��ؼ��֣��洢���н��յ�������
* ��  ��: 
* ��  ��: 
*       
			-1				ʧ��
			���������ֽ���: �ɹ�  0Ҳ�ǳɹ�
* ��  ��: AT����
*****************************************************************************
*/
static INT32S WaitATResp(const char *pKeyWord, INT32S nTimeOut, INT8U *pBuf, INT32U nBufSize, INT8U nType)
{
	pBASE_BUF	pBaseBuf 		= &g_sTotalBaseBuf;
	INT16U 		nRecvLen 		= 0;
	INT16U		nOnceLen		= 0;
	INT8U		nBuf[100]		= {0};
	INT16U		nSize			= 0;
	INT8U*		pData			= NULL;
	INT8U		nKeyWordLen 	= 0;
	INT16U		i 				= 0;
	INT16U		nRPtr       	= 0;
	INT8U		nFlg			= 0;

	//���ؼ���
	nKeyWordLen = strlen(pKeyWord);
	if (0 == nKeyWordLen)
    {
    	return -1;
    }
	if (NULL == pBuf)
	{
		pData = &nBuf[0];
		nSize = sizeof(nBuf);
	}
	else
	{
		pData = pBuf;
		nSize = nBufSize;
	}

	memset(&pData[0], 0, nSize);
	while(1)
	{
		if (nTimeOut < 0)
		{
			break;
		}
		nRPtr 		= 0;
		nOnceLen 	= 0;
		nOnceLen 	= ReadCommMs(COMM_GPRS, &pData[nRecvLen], nSize - 2 - nRecvLen, 10, 500);
		if (nOnceLen > 0)
		{
			if (GetRmotCmdPrintFlg())
			{
				Gw3761FrmWrite(COMM_WH232, &pData[nRecvLen], nOnceLen);
			}
			BaseBufRecv(pBaseBuf, &pData[nRecvLen], nOnceLen);
			nRecvLen += nOnceLen;
		}
		if (nFlg)
		{
			if (NULL == pBuf)
			{
				return 0;
			}
			else
			{
				return nRecvLen;
			}
		}
		nTimeOut -= 500;
		nOnceLen = nRecvLen;
		while (nOnceLen >= nKeyWordLen)
		{
			for (i = 0; i < nKeyWordLen; i++)
			{
				if( pData[nRPtr + i ] != (((INT8U)pKeyWord[i]) + 1) )
				{
					break;
				}
			}	
			if (i < nKeyWordLen)		
			{
				nRPtr++;
				nOnceLen--;
			}
			else
			{
				if (0 == nType)
				{
					memmove(pData, &pData[nRPtr], nOnceLen);
					memset(&pData[nOnceLen], 0, nRPtr);
					nRecvLen = nOnceLen;
				}
				if (NULL == pBuf)
				{
					return 0;
				}
				nFlg = 1;	
				break;
			}
		}
	}
	return -1;
}


/*
*****************************************************************************
* ������: SendAtCmdAndWaitATResp
* �䡡��: 

*					INT16U 		nSemFlg					0ʱ���ô��ڱ����ź���
*					INT32U		nSize					pBufΪNULLʱ��nSize��������
														ΪFOUR_BYTE_HIGH_BIT + ʱ����ʾ���շ��ص����ݣ�
														  ��������ʱ�����عؼ��ֺ��������
* ��  ��: 
*					INT8U*		pBuf					�����pcDataAddrͬһ������ֻ��ִ��һ�Σ���nCnt����1��ִ��
* ��  ��: 
*       
*      		 -1				ʧ��
			 0���߽��������ֽ���: �ɹ� 
* ��  ��: AT���ͽ���
*****************************************************************************
*/
INT32S SendAtCmdAndWaitATResp(const char *pcDataAddr,const char *pKeyWord, INT32S nTimeOut, INT8U nCnt, INT16U nSemFlg, INT8U *pBuf, INT32U nSize)
{
    INT8U		i 		= 0;
    INT8U       nNum	= nCnt;
    INT32S      nRet 	= 0;
	INT8U		nType	= 0;
	
	if (0 == nNum)
	{
		return -1;		
	}
	if (pcDataAddr == NULL)
	{
		return -1;
	}
	if ((INT32U)pcDataAddr == (INT32U)pBuf)
	{
		nNum = 1;													//ֻ��ִ��һ��
	}
	if (NULL != pBuf)
	{
		if (nSize & FOUR_BYTE_HIGH_BIT)
		{
			nSize &= ~FOUR_BYTE_HIGH_BIT;
			nType = 1;
		}
	}

	for (i = 0; i < nNum; i++)
	{
		if (!nSemFlg)
		{
			COM_PEND_SEM(COMM_GPRS);
		}
		SendAtOrder(pcDataAddr);
		if (NULL != pBuf)
		{
			memset(pBuf, 0, nSize);
		}
        nRet = WaitATResp(pKeyWord, nTimeOut, pBuf, nSize, nType);
		if (!nSemFlg)
		{
			COM_POST_SEM(COMM_GPRS);
		}
        if (nRet >= 0)
        {
            return (INT32U)nRet;    
        }
	}

	return -1;
}






#if 0

#endif
/****************************************************************************
* ������:ModlPowerCtrl
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��:
* ������:wyf
* �ա���:2009/10/03
* ��  ��:Version 1.0
* ��������:ģ���Դ����:FALSEʱ�ص���TRUE��
****************************************************************************/
INT32S ModlPowerCtrl(INT8U dir)
{
   	if (dir)
	{
     	//V5_GPRS_ON();   	
	}
	else
	{
		//V5_GPRS_OFF();    
	}

    return 0;
}


/****************************************************************************
* ������:ModlOnOff
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��:
* ������:wyf
* �ա���:2009/10/03
* ��  ��:Version 1.0
* ��������:ģ��OnOff����:FALSEʱ�ص���TRUE��
****************************************************************************/
INT32S ModlOnOff(INT8U dir)
{
    if (dir)
	{
		
	}
	else
	{
		
	}

    return 0;
}


/****************************************************************************
* ������:ModlBpsAdpt
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��:
* ������:wyf
* �ա���:2009/10/03
* ��  ��:Version 1.0
* ��������:ģ�鲨��������Ӧ
****************************************************************************/
INT32S ModlBpsAdpt(INT8U nCnt)
{
	INT8U               i				= 0;
    INT8U               nComm         	= COMM_GPRS;
	INT32S				nRet			= -1;
	INT32S				nSetBpsSucc	= -1;
	
	if (0 == nCnt)
	{
		return 0;		
	}
	nRet = SendAtCmdAndWaitATRespSlip("AT\x0d", (char*)g_Modl_OK, 1000, nCnt);
	if (nRet < 0)								//��Ҫ�������ò�����
	{	
		for (i = 0; i < sizeof(g_dwBaudRate) / sizeof(INT32U); i++)
		{
			CloseComm(nComm);
 			OpenComm(nComm, g_dwBaudRate[i], NO_PARITY, ONE_STOP_BIT, EIGHT_CHAR_BIT);
 			CommFlush(nComm);
			nSetBpsSucc = SendAtCmdAndWaitATRespSlip("AT+IPR=115200\x0d", (char*)g_Modl_OK, 1000, nCnt);
			if (nSetBpsSucc >= 0)
			{
				break;	
			}
		}
		
		CloseComm(nComm);
		OpenComm(nComm, UART_BPS_115200, NO_PARITY, ONE_STOP_BIT, EIGHT_CHAR_BIT);	
		CommFlush(nComm);
		nRet = SendAtCmdAndWaitATRespSlip("AT\x0d", (char*)g_Modl_OK, 1000, nCnt);
	}

	return nRet;		
}


/****************************************************************************
* ������:ModlGetHardVer
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��:
* ������:wyf
* �ա���:2009/10/03
* ��  ��:Version 1.0
* ��������:��ȡģ��Ӳ���汾
****************************************************************************/
INT32S ModlGetHardVer(void)
{
	INT32S					nRet			= -1;
	pTTIMER					pTTimer			= &g_pTimer[TIMER_TMP];
	pBASE_BUF				pBaseBuf 		= &g_sTotalBaseBuf;
	
	//�ܳ�ʱʱ��
	TimerOpen(pTTimer, 10);
	for (;;)
	{
		nRet = 0;
		if(0 == TimerValGet(pTTimer))
		{
			g_nModlType = MODL_TELIT;
			break;
		}
		if (SendAtCmdAndWaitATRespSlip((char*)"ATI\x0d", (char*)g_Modl_TELIT, 1000, 1) >= 0)
		{
			g_nModlType = MODL_TELIT;
			BaseBufChkLabel(pBaseBuf, 0, (INT8U*)g_Modl_TELIT, 1, 1);
			break;
		}
		else
		{
			if (BaseBufChkLabel(pBaseBuf, 0, (INT8U*)g_Modl_NEO, 1, 1) >= 0)
			{
				g_nModlType = MODL_NEO;	
				break;
			}
		}
		nRet = -1;
	}	
	
	return nRet;		
}


/********************************************************************************
* ������: ModlSignalRecord
* �䡡��:

*		
* �䡡��:
* ��  ��:
* ��  ��: ��¼��ʾ�������ź�ǿ��
*********************************************************************************/
void ModlSignalRecord(INT8U nSignal)
{
    if (nSignal > 40)
    {
        nSignal = 0;
    }
    nSignal = nSignal / 6;
    if (nSignal > 4)
    {
        nSignal = 4;
    }
    MemWriteRamXmpData(F8, 0, &nSignal, sizeof(nSignal));
}


#if 0
#endif
/****************************************************************************
* ������:TcpLinkFrame
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��:
* ������:wyf
* �ա���:2009/10/03
* ��  ��:Version 1.0
* ��������:��¼����
****************************************************************************/
INT32S TcpLinkFrame(INT8U nFn)
{
	INT8U 			nData[50] 	= {0};
	INT8U*			pData		= &nData[0];
	INT32S			nDatalen	= 0;
	// 1. �ϱ�����
	g_nReport = 1;
	GB3761PnFn2DADT(0, nFn, pData);
    //��ȡ�������룬�ն˵�ַ
    g_nAsynFlg = 1;
    GetTermAreaAddr();
    nDatalen = Gw3761MakeFrm(0xC9, 0x02, pData, 4, sizeof(nData));
    g_nAsynFlg = 0;
	if (MODL_TELIT == g_nModlType)
	{
		TltTcpSend(1, &nData[0], nDatalen);
	}
	g_nReport = 0;
    nMessageFlg = 1;    //���ճɹ���־
    
	return 0;
}

/****************************************************************************
* ������:TcpLinkFrame
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��:
* ������:wyf
* �ա���:2009/10/03
* ��  ��:Version 1.0
* ��������:��¼����
****************************************************************************/
INT32S NetLinkFrame(INT32S nSock, INT8U nFn)
{
	INT8U 			nData[50] 	= {0};
	INT8U*			pData		= &nData[0];
	INT32S			nDatalen	= 0;
	// 1. �ϱ�����
	GB3761PnFn2DADT(0, nFn, pData);
    //��ȡ�������룬�ն˵�ַ
    g_nAsynFlg = 1;
    GetTermAreaAddr();
    nDatalen = Gw3761MakeFrm(0xC9, 0x02, pData, 4, sizeof(nData));
    g_nAsynFlg = 0;
    EthnetSend(nSock, pData, nDatalen, 0);
	return 0;
}

#if 0
#endif
/*
*****************************************************************************
* ������: RmotLinkInit
* �䡡��: 
*				void *				pData				ͨ�Ų���
* ��  ��: 
* ��  ��: 
*		���
*		
*		
* ��  ��: 
*****************************************************************************
*/
static INT32S RmotLinkInit(void *pData)
{
	pTTIMER pTTimer = NULL;
	
	g_nLoginCnt = 0;
	memset(g_pRmotValClr, 0, sizeof(RMOT_VAL_CLR));
	g_pRmotValClr->nNdStart = 1;									// ��������־
    RmotParamTrans();

	// �ز�
	pTTimer = &g_pTimer[TIMER_LOGIN_ITVL];
	TimerOpen(pTTimer, 0);
	return 0;
}

/*
*****************************************************************************
* ������: RmotLinkStat
* �䡡��: 
* ��  ��: 
* ��  ��: 
*		���
*		
*		
* ��  ��: 
1���ͻ�����������ȫ�ֱ�־�л�
2���������ݣ���TotalBuf��š���������ʹ�ò��ȴ���ʱ��ExtReadCommMs����
3���ѿͻ���g_sClntBaseBuf�����������g_sSrvcBaseBuf�������ֵ����LinkCom��BaseBuf
4�����տͻ�����������ȫ�ֱ�־����TotalBuf���ҵ����Ͽͻ������߷�����������(�ҵ�TotalBuf�ķ���Ҫ������ݵ���ʼλ��pos�볤��len)
*****************************************************************************
*/
static INT32S RmotLinkStat(void* pLinkBaseBuf)
{	
#ifdef GPRS_MOD_DEBUG

#else
	pBASE_BUF  	pBaseBuf 	= &g_sTotalBaseBuf;	
	
	if (NULL == pLinkBaseBuf)
	{
		return -1;
	}

	// 1. �ͻ������������л���־
	g_nLinkType = (g_nLinkType + 1) % LINK_NUM;
	
	// 2. ���յ�TotalBuf
	RmotTotalBufRecv(pBaseBuf);

	// 3. ��ȡ��ǰʹ�õ�LinkCom��BaseBuf
	UseCurrBaseBuf((pBASE_BUF)pLinkBaseBuf, g_nLinkType);

	// 4. ����ģ�麯��
	TelitProc(pBaseBuf, &g_sClntBaseBuf2, &g_sSrvcBaseBuf2, g_nLinkType);
#endif

	return 0;
}

/*
*****************************************************************************
* ������: RmotLinkTailProc
* �䡡��: 
* ��  ��: 
* ��  ��: 
*		���
*		
*		
* ��  ��: 
*****************************************************************************
*/
static INT32S RmotLinkTailProc(void* pBaseBuf)
{
	return SaveCurrBaseBuf(pBaseBuf);
}


/*
*****************************************************************************
* ������: RmotLinkTask
* �䡡��: 
* ��  ��: 
* ��  ��: 
*		���
*       
*       
* ��  ��: 
*****************************************************************************
*/
void RmotLinkTask(void *pData)
{
	LinkComTaskProc(COMM_GPRS, pData, RmotLinkInit, RmotLinkStat, RmotLinkTailProc); 
}











