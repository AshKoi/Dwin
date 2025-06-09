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
#include "Telit.h"
#include "Base.h"
#include "TimerCount.h"
#include "SoftTimer.h"
#include "Fun.h"
#include "GB3761SetPrmExt.h"
/**************************************************************************************************
                                           ��
***************************************************************************************************/

//INT32S SendAtCmdAndWaitATResp(const char *pcDataAddr,const char *pKeyWord, INT32S nTimeOut, INT8U nCnt, INT16U nSemFlg, INT8U *pBuf, INT32U nSize)


/**************************************************************************************************
                                           ����
***************************************************************************************************/
INT8U        nCSQFlg        = 0;           //�ź�ǿ�ȼ���־
INT8U        nSIMReg        = 0;           //sim��ע���־
INT8U        nAPNConfig     = 0;           //APN������Ϣ
INT8U        nUserFlg       = 0;           //�����û�����Ϣ��־
INT8U        nPWFlg         = 0;           //���������־
INT8U        nIpPort        = 0;           //��ȡIP�Ͷ˿ں�
INT8U        nDialFlg       = 0;           //���ţ�����������վ
INT8U        nMessageFlg    = 0;           //��½���ձ���


/**************************************************************************************************
                                           ����
***************************************************************************************************/
const	char	g_Tlt_ClntRcv[] 	  = "RQHMF9\x1f\x30\x2B";			//"SRING: 1,"
const	char	g_Tlt_SrvrRcv2[] 	  = "RQHMF9\x1f\x31\x2B";			//"SRING: 2,"
const	char	g_Tlt_SrvrRcv[] 	  = "RQHMF9\x1f\x31";			//"SRING: 2"
//const	char	g_Tlt_SrvrRcv2[]      = "RH9\x1f\x31\x2B";           //"SI: 2,"

const	char	g_Tlt_SmsInfo[] 	  = "*BLSH9\x1F!RL!+";				//"+CMTI: "SM","
const	char	g_Tlt_SmsHead[] 	  = "*BLFQ9\x1F";					//"+CMGR: "

const	char	g_Tlt_MasterCls[]	  = "MN\x1f\x42@QQHDQ"; 			//"NO CARRIER"			������ʾ	


/**************************************************************************************************
                                           ��غ���ʵ��
***************************************************************************************************/
/****************************************************************************
* ������:TelitAtCmd
* �䡡��:
* �䡡��:
* ��  ��:
*		
* ��  ��: 
****************************************************************************/
static INT32S  TelitStart(void)
{
	pTTIMER	pTTimer 	= NULL;
	INT16U	nTmp		= 0;
	
	if (0 == g_pRmotValClr->nNdStart)
	{
		return 0;		
	}
    //���������GPRS��Դ�ƹ��ϣ��������ϵ�
    if (g_nGprsLedFlg)
    {
        return 0;
    }
    
	OSTimeDlyHMSM(0,0,5,100);
	for (;;)
	{
        //+++++ ����ʾ������״̬
        

        //ģ�鿪��
		OSTimeDlyHMSM(0,0,0,100);
		ModlOnOff(FALSE);									//ģ��OFF���ÿ��ߵ�
		ModlPowerCtrl(FALSE);								//ģ���Դ�ж�
		OSTimeDlyHMSM(0,0,2,0);									
		
		ModlPowerCtrl(TRUE);								//ģ���Դ����
		OSTimeDlyHMSM(0,0,1,200);
		ModlOnOff(TRUE);									//ģ��ON
		
		if (ModlBpsAdpt(10) < 0)								//ģ�鲨��������Ӧ
		{
			return RET_ERR_ERROR;
		}
		ModlGetHardVer();									//��ȡģ��Ӳ���汾

		memset(g_pRmotValClr, 0, sizeof(RMOT_VAL_CLR));

		// ����
		pTTimer = &g_pTimer[TIMER_BEAT];
		if (1 == g_pRomtParm->nWorkMode)
		{
			TimerClose(pTTimer);
		}
		else
		{
			nTmp = GET_MIN(GET_MAX(g_pRomtParm->nBeatIntvl, 60), 3600) * 10;
			TimerOpen(pTTimer, nTmp);
		}
		// ��ͨ��
		pTTimer = &g_pTimer[TIMER_NOCOMM];
		TimerClose(pTTimer);
		pTTimer = &g_pTimer[TIMER_SRVCNOCOMM];
		TimerClose(pTTimer);
		
		// �Լ�
		pTTimer = &g_pTimer[TIMER_SELFCHK];
		TimerClose(pTTimer);

		// �ź�ǿ��
		pTTimer = &g_pTimer[TIMER_SGNL_ITVL];
		nTmp = MODL_RDSIGNAL_INTVL;
		TimerOpen(pTTimer, nTmp);
		break;
	}
	
	g_pRmotValClr->nNdStart = 0;							// ��������־
	
	return 0;	
}

/*
***************************************************************************
* ������:TltLedTurnOnWay
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��:
* ������:wyf
* �ա���:2009/10/03
* ��  ��:Version 1.0
* ��������:ģ������ķ�ʽ
***************************************************************************
*/
static INT32S TltLedTurnOnWay(INT8U nWay)
{
    switch  (nWay)
    {
        case TLT_SLED_LOW:
            if (SendAtCmdAndWaitATRespSlip((char*)"AT#SLED=0\x0d", (char*)g_Modl_OK, 1000, 1) >= 0)
            {
                return 0;	
            }
            break;
        case TLT_SLED_HIGH:
            if (SendAtCmdAndWaitATRespSlip((char*)"AT#SLED=1\x0d", (char*)g_Modl_OK, 1000, 1) >= 0)
            {
                return 0;	
            }
            break;
        case TLT_SLED_ALTER:
            if (SendAtCmdAndWaitATRespSlip((char*)"AT#SLED=3,10,10\x0d", (char*)g_Modl_OK, 1000, 1) >= 0)
            {
                return 0;	
            }
            break;
        default:
            if (SendAtCmdAndWaitATRespSlip((char*)"AT#SLED=2\x0d", (char*)g_Modl_OK, 1000, 1) >= 0)
            {
                return 0;	
            }
            break;
    }
    return -1;
}


/*
***************************************************************************
* ������:TltRdSignal
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��:
* ������:wyf
* �ա���:2009/10/03
* ��  ��:Version 1.0
* ��������:��ȡ�ź�ǿ��
***************************************************************************
*/
static INT32S TltRdSignal(void)
{	
	INT8U			nAtCmdLen      	= strlen((char*)"*BRP9\x1f");
	INT8U			nSignalVal1   	= 0;
	INT8U			nSignalVal2    	= 0;
	INT8U			nTmpBuf[100]	= {0};
	
	if (SendAtCmdAndWaitATResp((char*)"AT+CSQ\x0d",(char*)"*BRP9\x1f", 2000, 1, 0, &nTmpBuf[0], sizeof(nTmpBuf)) < 0)//"+CSQ: "
	{
		return -1;
	}
	else
	{
		nSignalVal1 = nTmpBuf[nAtCmdLen + 0];
		nSignalVal2 = nTmpBuf[nAtCmdLen + 1];
		
		if (nSignalVal2 == 0x2C)
		{
			g_pRmotValClr->nSignalVal = nSignalVal1 - 0x30;
			return 0;
		}
		
		nSignalVal1 -= 0x30;
		nSignalVal2 -= 0x30;
		g_pRmotValClr->nSignalVal = nSignalVal1 * 10 + nSignalVal2;
		return 0;
	}
}

/****************************************************************************
* ������:TltRdSignalExt
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��:
* ������:wyf
* �ա���:2009/10/03
* ��  ��:Version 1.0
* ��������:�ź���
****************************************************************************/
static INT32S TltRdSignalExt(void)
{	 	
	pTTIMER	pTTimer 	= &g_pTimer[TIMER_SGNL_ITVL];
	INT16U	nTmp		= MODL_RDSIGNAL_INTVL;

	if (0 != TimerValGet(pTTimer))
	{
		return 0;
	}
	// ˢ���ز���ʱ��
	TimerValSet(pTTimer, nTmp);

	return TltRdSignal();
}


/****************************************************************************
* ������:TltSimReg
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��:
* ������:wyf
* �ա���:2009/10/03
* ��  ��:Version 1.0
* ��������:SIM��ע��
***************************************************************************/
static INT32S TltSimReg(void)
{	
	INT8U			nAtCmdLen      = strlen((char*)"*BQDF9\x1f");
	INT8U			nTemp		   	= 0;
	INT8U			nTmpBuf[100]	= {0};
	
	if (SendAtCmdAndWaitATResp((char*)"AT+CREG?\x0d", (char*)"*BQDF9\x1f", 2000, 1, 0, &nTmpBuf[0], sizeof(nTmpBuf)))//"+CREG: "
	{
		nTemp = nTmpBuf[nAtCmdLen + 2];
		if ((nTemp == 0x31) || (nTemp == 0x35))
		{
            nSIMReg = 1;
			return 0;
		}	
	}

	return -1;
}

/****************************************************************************
* ������:TltSimRegExt
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��:
* ������:wyf
* �ա���:2009/10/03
* ��  ��:Version 1.0
* ��������:SIM��ע����չ
****************************************************************************/
static INT32S TltSimRegExt(void)
{	 	
	pTTIMER	pTTimer = &g_pTimer[TIMER_TMP];
	
	TimerOpen(pTTimer, 90);
	for (;;)
	{
		if (0 == TimerValGet(pTTimer))
	   	{
	   		break;	
	   	}
		if (TltSimReg() >= 0)
		{								
			TimerClose(pTTimer);
			return 0;
		}

		OSTimeDlyHMSM(0,0,1,0);		
	}

	TimerClose(pTTimer);
	return -1;
}

/****************************************************************************
* ������:TelitBaseCmd
* �䡡��:
* �䡡��:
* ��  ��:
*		
* ��  ��: 
****************************************************************************/
static INT32S  TelitBaseCmd(void)
{
	INT8U			 	i				= 0;
	INT8U				nTmpBuf[100] 	= {0};

	if (g_pRmotValClr->nNdStart)
	{
		return -1;
	}
	if (g_pRmotValClr->nBaseCmd)
	{
		return 0;
	}

	if (SendAtCmdAndWaitATRespSlip((char*)"ATE0\x0d", (char*)g_Modl_OK, 1000, 1) < 0)
	{
		return -1;	
	}
	if (SendAtCmdAndWaitATRespSlip((char*)"AT+CMEE=1\x0d",(char*)g_Modl_OK, 1000, 1) < 0)
	{
		return -1;	
	}
	
    if (TltLedTurnOnWay(TLT_SLED_ALTER) < 0)
    {
        return -1;
    }
	
	if (SendAtCmdAndWaitATRespSlip((char*)"AT+IFC=0,0\r\n", (char*)g_Modl_OK, 1000, 1) < 0)
	{
		return -1;
	}

	memset(&nTmpBuf[0], 0, sizeof(nTmpBuf));
	if (SendAtCmdAndWaitATResp((char*)"AT+GMR\x0d", (char*)g_Modl_OK, 1000, 1, 0, &nTmpBuf[0], FOUR_BYTE_HIGH_BIT + sizeof(nTmpBuf)) < 0)//���汾
	{
		//return -1;	//�汾����ͨ��Ӱ�칦�ܣ��ʲ��˳�
	}
	else
	{
		g_pRmotValClr->nModlVer[0] = 'v';
		memcpy(&(g_pRmotValClr->nModlVer[1]), &(nTmpBuf[2]), 2);
		memcpy(&(g_pRmotValClr->nModlVer[3]), &(nTmpBuf[5]), 2);
		memcpy(&(g_pRmotValClr->nModlVer[5]), &(nTmpBuf[8]), 3);
	}
	#define CSQ_FIRST_RD_CNT 10
	for (i = 0; i < CSQ_FIRST_RD_CNT; i++)
	{
		if (SendAtCmdAndWaitATRespSlip((char*)"AT+CSQ\x0d", (char*)"*BRP9\x1f\x38\x38\x2b\x38\x38"/*"+CSQ: 99,99"*/, 1000, 1) < 0)//"+CSQ:99,99"
		{
			break;
		}
		OSTimeDlyHMSM(0,0,1,0);
	}
    nCSQFlg = 1;
	if (i >= CSQ_FIRST_RD_CNT)
	{
		//GYDebugSetCurrState(DEBUG_RMOT_PARAll_STATE_NOR,DEBUG_RMOT_STATE_CSQ);
		//return;	//waveģ�鲻������ִ�У�Telit�Ŀ���ִ�У��з��Ĵ��ԣ���ʱ��telit��һ��
	}
	#undef CSQ_FIRST_RD_CNT
	
	if (TltSimRegExt() < 0)
	{		
        g_pRmotValClr->nNdStart = 1;
		return -1;
	}
	
	g_pRmotValClr->nBaseCmd = 1;
	return 0;
}

/****************************************************************************
* ������:TltSetAPN
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��:
* ������:wyf
* �ա���:2009/10/03
* ��  ��:Version 1.0
* ��������:����̩����APN
****************************************************************************/
static INT32S TltSetAPN(void)
{
	INT8U nIndex   = 0;
	INT8U nBuf[100];
	
	//SendAtCmd("AT+CGDCONT=1,"IP","CMNET"");	
	memset(&nBuf[0],0,sizeof(nBuf));
	strcpy((char*)(&nBuf[0]),"AT+CGDCONT=1,\"IP\",\"");
	nIndex = strlen((char*)(&nBuf[0]));
	memcpy((INT8U*)(&nBuf[nIndex]), (INT8U*)(&g_pRomtParm->nApn[0]), sizeof(g_pRomtParm->nApn));	
	nIndex = strlen((char*)(&nBuf[0]));
	nBuf[nIndex++] = '"';
	nBuf[nIndex++] = 0x0d;
	nBuf[nIndex++] = 0x00;
					
	if (SendAtCmdAndWaitATRespSlip((char*)(&nBuf[0]), (char*)g_Modl_OK, 1000, 1) < 0)
	{
		return -1;	
	}
    nAPNConfig = 1;
	return 0;	
}

/****************************************************************************
* ������:TltRefreshLocalIpAndPort
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��:
* ������:wyf
* �ա���:2009/10/03
* ��  ��:Version 1.0
* ��������:��ȡSIM��IP
****************************************************************************/
static void TltRefreshLocalIpAndPort(const INT8U* pBuf)
{	
#define MAX_QUERY_IPPORT_LEN 	50	
	INT8U			 nCode		    = 0;
	INT8U			 i				= 0;
	INT8U			 j				= 0;
	INT8U            k				= 0;
	INT16U			 wi 			= 0;
	INT16U 			 nRecePtr1   	= 0;
	INT16U 			 nRecePtr2   	= 0;
	INT16U 			 nTempIpVal	= 0;
	BOOLEAN			 bFlg		    = FALSE;
	INT8U  			 nTempStr[5];
	INT8U			 nTermIp[4] 	= {0};
	
	if (NULL == pBuf)
	{
		return;		
	}
	memset(&g_pRomtParm->nTermIp[0], 0, sizeof(g_pRomtParm->nTermIp));
	
	nCode = 0x20;
	for (wi = 0; wi < MAX_QUERY_IPPORT_LEN; wi++)
	{		
		bFlg = (nCode == pBuf[wi]);
		if (TRUE == bFlg)
		{
			nRecePtr1 = wi;	
			break;
		}
	}
	nRecePtr1 += 1;
	if ((nRecePtr1 + 6) > MAX_QUERY_IPPORT_LEN)
	{
		return;		
	}
	
	nCode = 0x0D;
	for (wi = nRecePtr1; wi < MAX_QUERY_IPPORT_LEN; wi++)
	{
		if (nCode == pBuf[wi])
		{
			nRecePtr2 = wi;
			break;	 		
		}
	}
	bFlg = ((nRecePtr2 > (nRecePtr1 + 6)) && (nRecePtr2 < (nRecePtr1 + 16)));
	if (FALSE == bFlg)
	{
	 	return;	
	}
	
	nTempIpVal = 0;
	for (wi = nRecePtr1; wi < nRecePtr2 + 1; wi++)
	{
		if (i >= sizeof(nTempStr))
		{
			return;	
		}
		if ((pBuf[wi] >= 0x30) && (pBuf[wi] <= 0x39) && (wi < (nRecePtr1 + 16)))
		{
			nTempStr[i++] = pBuf[wi] - 0x30;
		}
		else if (('.' == pBuf[wi]) || (nCode == pBuf[wi]) || ((16 + nRecePtr1) == wi))
		{
			for (j = 0; j < i; j++)
			{
				nTempIpVal = nTempIpVal * 10 + nTempStr[j];
			}
			
			if (k >= 4)
			{
				break; 		
			}
			nTermIp[k++] = (INT8U)nTempIpVal;
			nTempIpVal   = 0;
			i            = 0;
			
			if (nCode == pBuf[wi])
			{
				break;
			}
		}		
	}

	if (k < 4)//IP��ַ��4���֣�û�л��4�������˳�
	{
		return;	 	
	}
	memcpy(&g_pRomtParm->nTermIp[0], &nTermIp[0], 4);
    nIpPort = 1;
#undef MAX_QUERY_IPPORT_LEN
}

/****************************************************************************
* ������:TltSetUserName
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��:
* ������:wyf
* �ա���:2009/10/03
* ��  ��:Version 1.0
* ��������:����̩�����û���
****************************************************************************/
static INT32S TltSetUserName(void)
{
	INT8U nIndex 		= 0;
	INT8U nBuf[100] 	= {0};
	
	//SendAtCmd("AT#USERID=\"card\"");	
	memset(&nBuf[0],0,sizeof(nBuf));
	strcpy((char*)(&nBuf[0]),"AT#USERID=\"");
	nIndex = strlen((char*)(&nBuf[0]));
	memcpy((INT8U*)(&nBuf[nIndex]),(INT8U*)(&g_pRomtParm->nLoginUserName[0]), sizeof(g_pRomtParm->nLoginUserName));
	nIndex = strlen((char*)(&nBuf[0]));
	nBuf[nIndex++] = '"';
	nBuf[nIndex++] = 0x0d;
	nBuf[nIndex++] = 0x00;
	if (SendAtCmdAndWaitATRespSlip((char*)(&nBuf[0]), (char*)g_Modl_OK, 1000, 1) < 0)
	{
		return -1;	
	}
    nUserFlg = 1;
	return 0;
}

/****************************************************************************
* ������:TltSetUserPwd
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��:
* ������:wyf
* �ա���:2009/10/03
* ��  ��:Version 1.0
* ��������:����̩��������
****************************************************************************/
static INT32S TltSetUserPwd(void)
{
	INT8U nIndex 		= 0;
	INT8U nBuf[100]	= {0};
	
	//SendAtCmd("AT#PASSW=\"card\"");	
	memset(&nBuf[0],0,sizeof(nBuf));
	strcpy((char*)(&nBuf[0]),"AT#PASSW=\"");
	nIndex = strlen((char*)(&nBuf[0]));
	memcpy((INT8U*)(&nBuf[nIndex]), (INT8U*)(&g_pRomtParm->nLoginPswd[0]), sizeof(g_pRomtParm->nLoginPswd));
	nIndex = strlen((char*)(&nBuf[0]));
	nBuf[nIndex++] = '"';
	nBuf[nIndex++] = 0x0d;
	nBuf[nIndex++] = 0x00;
	if (SendAtCmdAndWaitATRespSlip((char*)(&nBuf[0]), (char*)g_Modl_OK, 1000, 1) < 0)
	{
		return -1;	
	}
    nPWFlg = 1;
	return 0;
}



/****************************************************************************
* ������:TltPPPCfg
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��:
* ������:wyf
* �ա���:2009/10/03
* ��  ��:Version 1.0
* ��������:ģ���ʼ�Ļ�������
****************************************************************************/
static INT32S TltPPPCfg(void)
{	
	INT8U nTmpBuf[100] = {0};

	if (g_pRmotValClr->nNdStart)
	{
		return -1;
	}
	if (0 == g_pRmotValClr->nBaseCmd)
	{
		g_pRmotValClr->nPPPCmd = 0;
		return -1;
	}
	if (g_pRmotValClr->nPPPCmd)
	{
		return 0;
	}
	if (SendAtCmdAndWaitATRespSlip((char*)"AT#GPRS=0\r\n", (char*)g_Modl_OK, 2000, 3) < 0)
	{
		//return;	
	}
    OSTimeDlyHMSM(0,0,0,150);
    SendAtCmdAndWaitATRespSlip((char*)"AT#FRWL=0,\"0.0.0.0\",\"0.0.0.0\"\r\n", (char*)g_Modl_OK,1000, 1);
    OSTimeDlyHMSM(0,0,0,150);
    if (SendAtCmdAndWaitATRespSlip((char*)"AT#FRWL=1,\"0.0.0.0\",\"0.0.0.0\"\r\n", (char*)g_Modl_OK, 5000, 1) < 0)
	{
		return -1;
	}

	if (TltSetAPN() < 0)
	{								
		return -1;
	}
	if (SendAtCmdAndWaitATRespSlip((char*)"AT#SKIPESC=0\r\n", (char*)g_Modl_OK, 5000, 1) < 0)
	{
		return -1;	
	}
	OSTimeDlyHMSM(0,0,3,0);
	if (SendAtCmdAndWaitATRespSlip((char*)"AT+CGATT=1\r\n", (char*)g_Modl_OK, 8000, 5) < 0)
	{
		return -1;	
	}
	do
	{
        if (SendAtCmdAndWaitATResp((char*)"AT#SGACT=1,1\r\n",(char*)g_Modl_OK,8000,1,0,&nTmpBuf[0], FOUR_BYTE_HIGH_BIT + sizeof(nTmpBuf)) >= 0)
		//if (SendAtCmdAndWaitATRespSlip((char*)"AT#SGACT=1,1\r\n", (char*)g_Modl_OK, 5000, 1) < 0)
		{
			break;	
		}
		if (SendAtCmdAndWaitATRespSlip((char*)"AT#SGACT?\r\n", (char*)"RF@BS9\x1f\x30\x2b\x30"/*"SGACT: 1,1"*/, 5000, 1) < 0)
		{
			return -1;	
		}
	}while(0);
	TltRefreshLocalIpAndPort(&nTmpBuf[0]);
		
	if (TltSetUserName() < 0)
	{								
		return -1;
	}
	if (TltSetUserPwd() < 0)
	{								
		return -1;
	}
	OSTimeDlyHMSM(0,0,2,0);
	//#if 0
	if (SendAtCmdAndWaitATRespSlip((char*)"AT#SCFG=1,1,1200,0,600,1\r\n", (char*)g_Modl_OK, 5000, 1) < 0)
	{
		return -1;	
	}
	if (SendAtCmdAndWaitATRespSlip((char*)"AT#SCFG=2,1,1200,0,600,1\r\n", (char*)g_Modl_OK, 5000, 1) < 0)
	{
		return -1;	
	}	
	//#else
	//if (SendAtCmdAndWaitATRespSlip((char*)"AT#SCFGEXT=1,2,0,0,1,0\r\n", (char*)g_Modl_OK, 5000, 1) < 0)
	{
		//return -1;	
	}
	//if (SendAtCmdAndWaitATRespSlip((char*)"AT#SCFGEXT=2,2,0,200,1,1\r\n", (char*)g_Modl_OK, 5000, 1) < 0)
	{
		//return -1;	
	}
    
    if (SendAtCmdAndWaitATRespSlip((char*)"AT#SCFGEXT=1,2,0,120,0,0\r\n", (char*)g_Modl_OK, 5000, 1) < 0)
	{
		return -1;	
	}
	if (SendAtCmdAndWaitATRespSlip((char*)"AT#SCFGEXT=2,2,0,120,0,0\r\n", (char*)g_Modl_OK, 5000, 1) < 0)
	{
		return -1;	
	}
    
    //if (SendAtCmdAndWaitATRespSlip((char*)"AT#SCFGEXT2=2,0,0,1200,0,1\r\n", (char*)g_Modl_OK, 5000, 1) < 0)
	{
		//return -1;	
	}
    
    
	//#endif
	g_pRmotValClr->nPPPCmd = 1;

	return 0;
}

/***************************************************************************
* ������:TltPPPCfgCls
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��:
* ������:wyf
* �ա���:2009/10/03
* ��  ��:Version 1.0
* ��������:ģ��PPP�ر�
****************************************************************************/
static INT32S TltPPPCfgCls(void)
{	
	if (g_pRmotValClr->nNdStart)
	{
		return -1;
	}
	
	if (SendAtCmdAndWaitATRespSlip((char*)"AT#GPRS=0\r\n", (char*)g_Modl_OK, 5000, 3) < 0)
	{
		g_pRmotValClr->nNdStart = 1;		
	}

	g_pRmotValClr->nPPPCmd = 0;
	return 0;	
}




/****************************************************************************
* ������:TltTcpOpenPort
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��:
* ������:wyf
* �ա���:2009/10/03
* ��  ��:Version 1.0
* ��������:TCP�����˿ڴ�
****************************************************************************/
static INT32S TltTcpOpenPort(void)
{	
	INT8U      		nIndex       	= 0;
	INT16U			nDetectPort  	= 0;
	INT8U 			nBuf[50];
	pTTIMER			pTTimer 		= &g_pTimer[TIMER_SELFCHK];
	INT16U			nTmp			= MODL_SELF_INTVL;
	
	if (g_pRmotValClr->nNdStart)
	{
		return -1;
	}
	if (0 == g_pRmotValClr->nPPPCmd)
	{
		g_pRmotValClr->nOpenPortCmd = 0;
		return -1;
	}
	if (g_pRmotValClr->nOpenPortCmd)
	{
		if (1 != TimerStatGet(pTTimer))
		{
			TimerOpen(pTTimer, nTmp);
		}
		return 0;
	}

	memset(&nBuf[0],0,sizeof(nBuf));
	strcpy((char*)(&nBuf[0]), "AT#SL=2,1,");
	nIndex = strlen((char*)(&nBuf[0]));
	nDetectPort = g_pRomtParm->nTermPort;
	if (0 == nDetectPort)
	{
		nDetectPort = 5100; 	
	}
	ValToAscii(&nBuf[nIndex], (INT32U)nDetectPort, 5);//�˿ڲ��ᳬ��5λ
	nIndex = strlen((char*)(&nBuf[0]));
	nBuf[nIndex++] = 0x0d;
	nBuf[nIndex++] = 0x00;
			
	if (SendAtCmdAndWaitATRespSlip((char*)(&nBuf[0]), (char*)g_Modl_OK, 5000, 2) < 0)
	{
		return -1;	
	}
    
#if 0
    if (SendAtCmdAndWaitATRespSlip((char*)"AT#SCFGEXT=2,2,0,0,0,0\r\n", (char*)g_Modl_OK, 5000, 1) < 0)
	{
		return -1;	
	}
#endif

	TimerOpen(pTTimer, nTmp);

	g_pRmotValClr->nOpenPortCmd = 1;
	return 0;
}

/****************************************************************************
* ������:TltTcpOpenPortCls
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��:
* ������:wyf
* �ա���:2009/10/03
* ��  ��:Version 1.0
* ��������:TCP�����˿ڹر�
****************************************************************************/
static INT32S  TltTcpOpenPortCls(void)
{	
    INT8U 			nBuf[50]		= {0};
    INT8U      		nIndex       	= 0;
	INT16U			nDetectPort  	= 0;
	pTTIMER			pTTimer 		= NULL;

	if (g_pRmotValClr->nNdStart)
	{
		return -1;
	}
	
    memset(&nBuf[0],0,sizeof(nBuf));
	strcpy((char*)(&nBuf[0]), "AT#SL=2,0,");
	nIndex = strlen((char*)(&nBuf[0]));
	nDetectPort = g_pRomtParm->nTermPort;
	if (0 == nDetectPort)
	{
		nDetectPort = 5100; 	
	}
	ValToAscii(&nBuf[nIndex], (INT32U)nDetectPort, 5);//�˿ڲ��ᳬ��5λ
	nIndex = strlen((char*)(&nBuf[0]));
	nBuf[nIndex++] = 0x0d;
	nBuf[nIndex++] = 0x00;

	if (SendAtCmdAndWaitATRespSlip((char*)(&nBuf[0]), (char*)g_Modl_OK, 5000, 2) < 0)
	{
		g_pRmotValClr->nNdStart = 1;	
	}

	pTTimer = &g_pTimer[TIMER_SELFCHK];
	TimerClose(pTTimer);
	
	pTTimer = &g_pTimer[TIMER_SRVCNOCOMM];
	TimerClose(pTTimer);

	g_pRmotValClr->nOpenPortCmd = 0;
	return 0;	
}

/********************************************************************************
* ������:
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ���IP���˿ںϷ���
*********************************************************************************/
static BOOLEAN CheckIpPortIsValid(const INT8U *pBuf,INT16U nPort)
{
	if (NULL == pBuf)
	{
		return FALSE;
	}
	if (0 == nPort)
	{
		return FALSE;
	}
	if (0 == pBuf[0])
	{
		return FALSE;
	}
	else if (0xFF == pBuf[0])
	{
		return FALSE;
	}
	
	return TRUE;
}
/****************************************************************************
* ������:TltAtSendIPport
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��:
* ������:wyf
* �ա���:2009/10/03
* ��  ��:Version 1.0
* ��������:�γ�̩���ص�IP�Ͷ˿�
****************************************************************************/
static INT32S TltAtSendIPport(const INT8U *pBuf, INT16U nPort, INT8U *pOutBuf)
{
	INT8U			 i				= 0;
	INT8U            nIndex       	= 0;
	
	if ((NULL == pBuf) || (NULL == pOutBuf))
	{
		return -1;		
	}
	// "AT#SD=1,0,5213,"218.206.106.26""
	// AT#SD=1,0,5600,"218.94.157.54",0,0,1
	if (LOGIN_UDP == g_pRomtParm->nNetProt)
	{
		strcpy((char*)(&pOutBuf[0]), (char*)"AT#SD=1,1,");
	}
	else
	{
		strcpy((char*)(&pOutBuf[0]), (char*)"AT#SD=1,0,");
	}
	nIndex = strlen((char*)(&pOutBuf[0]));
	nIndex += ValToAscii(&pOutBuf[nIndex], (INT32U)(nPort), 5);
	pOutBuf[nIndex++] = ',';
	pOutBuf[nIndex++] = '"';
	for (i = 0; i < 4; i++)
	{
		nIndex += ValToAscii(&pOutBuf[nIndex], (INT32U)(pBuf[i]), 3);//ip���ᳬ��3λ
		if (i + 1 < 4)
		{
			pOutBuf[nIndex++] = '.';	
		}
	}
	pOutBuf[nIndex++] = '"';
	strcpy((char*)(&pOutBuf[nIndex]), (char*)",0,0,1");
	nIndex += strlen((char*)",0,0,1");
	pOutBuf[nIndex++] = 0x0d;
	pOutBuf[nIndex++] = 0x00;	

	return 0;
}

/****************************************************************************
* ������:TltClntTcpSckt
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��:
* ������:wyf
* �ա���:2009/10/03
* ��  ��:Version 1.0
* ��������:�ͻ���TCP����
****************************************************************************/
static INT32S TltClntTcpSckt(void)
{	
    INT8U             	i             	= 0;
	INT8U				nIp[2][4]		= {0};
	INT16U				nPort[2]		= {0};
	INT8U 			 	nBuf[50]		= {0};

	if (g_pRmotValClr->nNdStart)
	{
		return -1;
	}
	if (0 == g_pRmotValClr->nPPPCmd)
	{
		g_pRmotValClr->nLoginSucc = 0;
		return -1;
	}
	if (g_pRmotValClr->nLoginSucc)
	{
		return 0;	
	}

	memcpy(&nIp[0][0], &g_pRomtParm->nMainIp[0], 4);
	memcpy(&nIp[1][0], &g_pRomtParm->nBakIp[0], 4);
	nPort[0] = g_pRomtParm->nMainPort;
	nPort[1] = g_pRomtParm->nBakPort;
    for (i = 0; i < 2; i++)
    {
        if (!CheckIpPortIsValid(&nIp[i][0], nPort[i]))
        {
			continue;
        }

        memset(&nBuf[0],0,sizeof(nBuf));
        TltAtSendIPport(&nIp[i][0], nPort[i], &nBuf[0]);	
        if (SendAtCmdAndWaitATRespSlip((char*)(&nBuf[0]),(char*)g_Modl_OK/*(char*)"BNMMDBS"*/,60000,1) >= 0)//"CONNECT"
        //if (SendAtCmdAndWaitATRespSlip((char*)(&nBuf[0]),(char*)"BNMMDBS",60000,1) >= 0)//"CONNECT"
        {
            nDialFlg = 1;
            g_pRmotValClr->nLoginSucc = 1;
            return 0;
        }
        OSTimeDlyHMSM(0,0,5,0);
    }
    return -1;
}

/****************************************************************************
* ������:TltClntTcpScktCls
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��:
* ������:wyf
* �ա���:2009/10/03
* ��  ��:Version 1.0
* ��������:�ͻ���TCP���ӹر�
****************************************************************************/
static INT32S TltClntTcpScktCls(void)
{	
	pTTIMER			pTTimer 		= NULL;
	
	if (g_pRmotValClr->nNdStart)
	{
		return -1;
	}
	if (SendAtCmdAndWaitATRespSlip((char*)"AT#SH=1\r\n", (char*)g_Modl_OK, 5000, 1) < 0)
	{
		g_pRmotValClr->nNdStart = 1;
		return 0;	
	}

	//g_pRmotValClr->nLoginSucc = 0;

	pTTimer = &g_pTimer[TIMER_NOCOMM];
	TimerClose(pTTimer);
	
	return 0;
}



#if 0
#endif
/****************************************************************************
* ������:TltClntTcpScktExt
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��:
* ������:wyf
* �ա���:2009/10/03
* ��  ��:Version 1.0
* ��������:�ͻ���TCP����
****************************************************************************/
static INT32S TltClntTcpScktExt(void)
{
	pTTIMER	pTTimer 	= &g_pTimer[TIMER_LOGIN_ITVL];
	INT16U	nTmp		= GET_MIN(GET_MAX(g_pRomtParm->nLoginIntvl, 30), 900);
	INT32S	nRet		= 0;
    INT8U   nLinkReq    = 0;

	if (g_pRmotValClr->nNdStart)
	{
		return -1;
	}
	if (0 == g_pRmotValClr->nPPPCmd)
	{
		g_pRmotValClr->nLoginSucc = 0;
		return -1;
	}
	
	// �ͻ���ģʽ
	if (1 == g_pRomtParm->nWorkMode)
	{
		// ��¼�ɹ�ʱ,��ȥ��¼
		if (0 == g_pRmotValClr->nLoginSucc)
		{
            MemReadRamXmpData(F9, 0, &nLinkReq, sizeof(nLinkReq));
            if (nLinkReq == 1)//F38
            {
                nLinkReq = 0;
                MemWriteRamXmpData(F9, 0, &nLinkReq, sizeof(nLinkReq));
                // ����
                nRet = TltClntTcpSckt();
            }
            else
            {
                if (1 == g_pRomtParm->nOnlineType)//��������
                {
                    // ����ز���ʱ���������
                    if (0 == TimerStatGet(pTTimer))
                    {
                        TimerOpen(pTTimer, nTmp);
                    }
                    // �ز�����������š�ͬʱˢ���ز���ʱ��
                    if (0 != TimerValGet(pTTimer))
                    {
                        // �ز����δ��,ֱ���˳�
                        return -1;		
                    }
                    // ˢ���ز���ʱ��
                    TimerValSet(pTTimer, nTmp);

                    // ����
                    nRet = TltClntTcpSckt();
                }
            }	
		}
		else
		{
			TimerClose(pTTimer);
			if (1 == g_pRmotValClr->nLoginSucc)
			{
				// ��¼����
				TcpLinkFrame(F1);
				g_pRmotValClr->nLoginSucc = 2;
			}
			return 0;	
		}
	}
	else
	{
		// ��¼�ɹ�ʱ,��ȥ��¼
		if (g_pRmotValClr->nLoginSucc)
		{
			TimerClose(pTTimer);

			// Ҳ���Էŵ�TelitAtCmd�����"4. ��ͨѶ2���ӶϿ�����"
			pTTimer = &g_pTimer[TIMER_NOCOMM];
			if (0 == TimerStatGet(pTTimer))
			{
				TimerOpen(pTTimer, nTmp);
			}
			return 0;	
		}

		// �ز�����������š�ͬʱˢ���ز���ʱ��
		if (0 != TimerValGet(pTTimer))
	   	{
	   		return -1;	
	   	}
		TimerValSet(pTTimer, nTmp);

		// ����
		nRet = TltClntTcpSckt();
		// ��¼����
		TcpLinkFrame(F1);
        
        //
        //ModeChangeAT();
        
		//g_pRmotValClr->nLoginSucc = 2;
		
		// ���ģʽ�£�ֻ��¼3�Ρ��رն�ʱ��
		if (g_nLoginCnt >= 3)
		{
			TimerClose(pTTimer);		
		}		
	}
	return nRet;
}

/****************************************************************************
* ������:TltTcpBeat
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��:
* ������:wyf
* �ա���:2009/10/03
* ��  ��:Version 1.0
* ��������:����
****************************************************************************/
static INT32S TltTcpBeat(void)
{
	pTTIMER			pTTimer 	= &g_pTimer[TIMER_BEAT];
	INT16U			nTmp 		= 0;
	
	// �ͻ���ģʽ�£���������
	if (1 == g_pRomtParm->nWorkMode)
	{
		nTmp = GET_MIN(GET_MAX(g_pRomtParm->nBeatIntvl, 60), 900);
		if (2 == g_pRmotValClr->nLoginSucc)
		{
			// ����
			if (0 == TimerStatGet(pTTimer))
			{
				TimerOpen(pTTimer, nTmp);
			}
		}
		else
		{
			TimerClose(pTTimer);
		}
		
		if (0 != TimerValGet(pTTimer))
		{
			return 0;
		}

		TcpLinkFrame(F3);
		
		TimerOpen(pTTimer, nTmp);
		return 0;
	}
	// ���ģʽ
	else
	{
		// ���ڳ�ʼ�������Ѿ��򿪼�ʱ��
		if (0 != TimerValGet(pTTimer))
		{
			return 0;
		}
		// �ڹ�Լ�㴦���У��������������ģ��� TimerOpen(pTTimer, nTmp);
		
		g_pRmotValClr->nNdStart = 1;
		
		return 0;
	}
    
    //return 0;
}


/****************************************************************************
* ������:TelitAtCmd
* �䡡��:
*			pBASE_BUF			pBaseBuf      ȫ�ֻ���
* �䡡��:
* ��  ��:
*		
* ��  ��: 
****************************************************************************/
static INT32S TelitAtCmd(pBASE_BUF pBaseBuf)
{
	pTTIMER			pTTimer 		= NULL;
	STD_TIME		sStdTime;
	static INT16U	s_nSignalCnt 	= 0;
    INT8U           nLinkReq        = 0; 	

	pBaseBuf = pBaseBuf;
		 
	// 1. �ϵ�����
	TelitStart();

	// 1.1. ������PPP���ź�ǿ��
	TelitBaseCmd();
	TltPPPCfg();
	if (TltRdSignalExt() < 0)
	{
		s_nSignalCnt++;
	}
	if (s_nSignalCnt > 3)
	{
		s_nSignalCnt = 0;
		g_pRmotValClr->nNdStart = 1;
	}
    ModlSignalRecord(g_pRmotValClr->nSignalVal);

	// 2. ����
	TltTcpOpenPort();
	
	// 3. ����
	TltClntTcpScktExt();
	

	// 4. ��ͨѶ2���ӶϿ�����
	if (1 == g_pRomtParm->nWorkMode)
	{
        MemReadRamXmpData(F9, 0, &nLinkReq, sizeof(nLinkReq));
        // �ͻ���ģʽ
        if (nLinkReq == 2)//F39
        {
            nLinkReq = 0;
            MemWriteRamXmpData(F9, 0, &nLinkReq, sizeof(nLinkReq));
            TltClntTcpScktCls();
        }
        else
        {
            
            if (2 == g_pRomtParm->nOnlineType)//��������ģʽ
            {
                pTTimer = &g_pTimer[TIMER_NOCOMM];
                if (0 == TimerValGet(pTTimer))
                {
                    TltClntTcpScktCls();
                }
            }
        }
		
        
	}
	else
	{
		// ���ģʽ
		pTTimer = &g_pTimer[TIMER_NOCOMM];
		if (0 == TimerValGet(pTTimer))
		{
			TltClntTcpScktCls();
		}
		pTTimer = &g_pTimer[TIMER_SRVCNOCOMM];
		if (0 == TimerValGet(pTTimer))
		{
			TltTcpOpenPortCls();
			TltTcpOpenPort();
		}
	}
	// 5. ����
	TltTcpBeat();
	// 6���Լ�
	pTTimer = &g_pTimer[TIMER_SELFCHK];
	if (0 == TimerValGet(pTTimer))
	{
		TltTcpOpenPortCls();
		TltTcpOpenPort();
	}
	
	// 7��һ�첻����
	if (1 == g_pRomtParm->nWorkMode)
	{
		memset(&sStdTime, 0, sizeof(sStdTime));
		GetSysClk(&sStdTime);
		if ((sStdTime.nHour == 0x23) && (sStdTime.nMin == 0x10))
		{
			if (2 != g_pRmotValClr->nLoginSucc)
			{
				ResetDelayHardInit(70 * 1000);
			}
		}
	}
    
    return 0;
}

/****************************************************************************
* ������:TelitInfoDataProc
* �䡡��:
*			pBASE_BUF			pBaseBuf      ȫ�ֻ���
* �䡡��:
* ��  ��:
*		
* ��  ��: 
	1. ���ӶϿ���־
	2. ���������ӱ�־
****************************************************************************/
static INT32S TelitInfoDataProc(pBASE_BUF pBaseBuf)
{
	INT8U* 	pLabel			= NULL;
	INT32S	nBasePtr		= -1;
	INT8U	nFlg			= 0;
	pTTIMER	pTTimer 		= NULL;
	INT16U	nTmp 			= 0;
    INT32U  nLabelLen       = 0;

	// 1. ���ӶϿ���־
	pLabel	= (INT8U*)g_Tlt_MasterCls;
	nFlg	= 0;
	while(1)
	{
		nBasePtr = BaseBufChkLabel(pBaseBuf, 0, pLabel, 1, 1);
		if (nBasePtr < 0)
		{
			break;	
		}
		else
		{
			if (0 == nFlg)
			{
				TltTcpOpenPortCls();
				//TltClntTcpScktCls();
			}
			nFlg = 1;
		}
	}
#if 1
	// 2. ���������ӱ�־
    pLabel	= (INT8U*)g_Tlt_SrvrRcv2;
	nLabelLen = strlen((char*)pLabel);
	nFlg	= 0;
	//while(1)
	{
		nBasePtr = BaseBufChkLabel(pBaseBuf, 0, pLabel, 1, 0);
		if (nBasePtr >= 0)
		{
			return 0;	
		}
        
	}
    
    
	pLabel	= (INT8U*)g_Tlt_SrvrRcv;
	nLabelLen = strlen((char*)pLabel);
	nFlg	= 0;
	//while(1)
	{
		nBasePtr = BaseBufChkLabel(pBaseBuf, 0, pLabel, 1, 1);
		if (nBasePtr < 0)
		{
			//break;	
		}
		else
		{
			pTTimer = &g_pTimer[TIMER_SRVCNOCOMM];
			nTmp = GET_MIN(GET_MAX(g_pRomtParm->nUnCommTimes, 30), 600);
			TimerOpen(pTTimer, nTmp);
            
            
            
            // AT#SI=2 
            //SendAtCmdAndWaitATRespSlip((char*)"AT#SI=2\r\n", (char*)g_Modl_OK, 1000, 1);
            
            // AT#SA=2,1    ���� SRING: 2
            ModeChangeData(3);
		}
        
	}
#endif	
	return 0;
}

/****************************************************************************
* ������:TelitRcvDataTimerProc
* �䡡��:
*				INT8U			nLinkType
* �䡡��:
* ��  ��:
*		
* ��  ��:  ���յ���Ч����ʱ�����������ʱ��
****************************************************************************/
static INT32S TelitRcvDataTimerProc(INT8U nLinkType)
{
	pTTIMER	pTTimer 		= NULL;
	INT16U	nTmp 			= 0;
	
	// ����
	pTTimer = &g_pTimer[TIMER_BEAT];
	if (1 == g_pRomtParm->nWorkMode)
	{
		nTmp = GET_MIN(GET_MAX(g_pRomtParm->nBeatIntvl, 60), 900);
	}
	else
	{
		nTmp = GET_MIN(GET_MAX(g_pRomtParm->nBeatIntvl, 60), 3600) * 10;
	}
	TimerOpen(pTTimer, nTmp);

	// ��ͨ��
	if (LINK_CLNT == nLinkType)
	{
		pTTimer = &g_pTimer[TIMER_NOCOMM];
	}
	else
	{
		pTTimer = &g_pTimer[TIMER_SRVCNOCOMM];
        //ModeChangeData(2);
	}
	nTmp = GET_MIN(GET_MAX(g_pRomtParm->nUnCommTimes, 30), 600);
	TimerOpen(pTTimer, nTmp);

	// �Լ�
	pTTimer = &g_pTimer[TIMER_SELFCHK];
	nTmp = MODL_SELF_INTVL;
	TimerOpen(pTTimer, nTmp);

	// �ź�
	pTTimer = &g_pTimer[TIMER_SGNL_ITVL];
	nTmp = MODL_RDSIGNAL_INTVL;
	TimerOpen(pTTimer, nTmp);

	return 0;
}

/****************************************************************************
* ������:TelitRcvData
* �䡡��:
*			pBASE_BUF			pBaseBuf      	ȫ�ֻ���
*			INT8U				nLinkType		�ͻ�/����
* �䡡��:
* ��  ��:
*		
* ��  ��: 
****************************************************************************/
static INT32S TelitRcvData(pBASE_BUF pBaseBuf, pBASE_BUF pBaseBuf1, pBASE_BUF pBaseBuf2, INT8U nLinkType)
{
	// RCV(S):1,100,
	INT8U* 	pLabel			= NULL;
	INT8U	nLabelLen 		= 0;
	INT32S	nLen			= 0;
	INT32S	j				= 0;
    INT32S	k				= 0;
	INT32S	nPos			= 0;
	INT32S	nBasePtr		= -1;       
	pBASE_BUF pBaseBufTmp   = NULL;
    INT32S  nRet            = 0;
    INT32S	nPos1			= 0;
    INT32S	nPos2			= 0;
    //static INT8U  nClntCnt  = 0;
    //static INT8U  nSrvtCnt  = 0;
    //INT8U*  pCnt            = NULL;

	if (LINK_CLNT == nLinkType)
	{
		pLabel	= (INT8U*)g_Tlt_ClntRcv;
		pBaseBufTmp = pBaseBuf1;
        //pCnt = &nClntCnt;
	}
	else
	{
		pLabel	= (INT8U*)g_Tlt_SrvrRcv2;
		pBaseBufTmp = pBaseBuf2;
        //pCnt = &nSrvtCnt;
	}
	nLabelLen = strlen((char*)pLabel);
    /*
    do
    {
        nPos1 = BaseBufChkLabel(pBaseBuf, 0, pLabel, 1, 0);
        if (nPos1 >= 0)
        {
            nPos1 += nLabelLen;
            nPos2 = BaseBufChkLabel(pBaseBuf, nPos1, pLabel, 1, 0);
            if (nPos2 >= 0)
            {
                nRet = 1;
            }
            else
            {
                nRet = 2;
            }
        }
    }while(0);
    
    if (nRet == 0)
    {
        (*pCnt) = 0;
        return -1;
    }
    else if (nRet == 1)
    {
        (*pCnt) = 0;
    }
    else
    {
        (*pCnt)++;
        if ((*pCnt) > 1)
        {
            (*pCnt) = 0;
        }
        else
        {
            return -1;
        }
    }
    */
    nRet = 0;
	while(1)
	{
        nBasePtr = BaseBufChkLabel(pBaseBuf, 0, pLabel, 1, 1);
        if (nBasePtr < 0)
        {   
            nRet = -1;
            break;
            //return -1;
        }
        else
        {
            if (LINK_CLNT == nLinkType)
            {
              
            }
            else
            {
                nLen = 0;
            }
            //nBasePtr = BASEBUF_PROC_PTR(nBasePtr);
            // ������ܳ���
            nLen = 0;
            for (j = 0; j < 4; j++)
            {
                nPos = nBasePtr + nLabelLen + j;
                //BASEBUF_POS_PBUF
                if (BASEBUF_PROC_PBUF(nPos) == ',')
                {
                    break;
                }
                nLen = nLen * 10 + (BASEBUF_PROC_PBUF(nPos) - '0');
            }
            // ���ȺϷ����ж�
            if (nLen >= RmotAppBufLenGet(nLinkType))
            {
                nRet = -1;
                break;
                //return -1;
            }
            if (nLen > BASEBUF_BYTE_NUM(pBaseBuf))					
            {
                nRet = -1;
                break;
                //return -1;
            }
            // ��¼ס���յ��Ŀͻ���/������ͨ������Ϣ:g_nTotalRmotBufPtr  g_nTotalRmotBufLen   ͨ��RmotRecv�������ݵ�������·Ӧ��
                //RmotBufValidDataInfoMake((nBasePtr + nLabelLen + j + 1) % pBaseBuf->nBufSize, nLen);
                // ��pBaseBuf(nBasePtr + nLabelLen + j + 1, nLen) -----> pBaseBuf1
            for (k = 0; k < nLen; k++)
            {
                if (pBaseBufTmp->nRecvPtr >= pBaseBufTmp->nBufSize)
                {
                    pBaseBufTmp->nRecvPtr = pBaseBufTmp->nRecvPtr % pBaseBufTmp->nBufSize;
                }
                if (((pBaseBufTmp->nRecvPtr + 1) % pBaseBufTmp->nBufSize) == ((pBaseBufTmp->nProcPtr) % pBaseBufTmp->nBufSize))
                {
                    pBaseBufTmp->nProcPtr = (pBaseBufTmp->nProcPtr + 1) % pBaseBufTmp->nBufSize;
                }
                pBaseBufTmp->pBuf[pBaseBufTmp->nRecvPtr] = BASEBUF_PROC_PBUF(nBasePtr + nLabelLen + j + 1 + k);
                  //pBaseBuf->pBuf[(nBasePtr + nLabelLen + j + 1 + k) % pBaseBuf->nBufSize];
                pBaseBufTmp->nRecvPtr++;
                if (pBaseBufTmp->nRecvPtr >= pBaseBufTmp->nBufSize)
                {
                    pBaseBufTmp->nRecvPtr = pBaseBufTmp->nRecvPtr % pBaseBufTmp->nBufSize;
                }
            }

            // ���յ���Ч����ʱ�����������ʱ��
            TelitRcvDataTimerProc(nLinkType);
        }
	}


	

	return nRet;
}



#if 0
#endif
/****************************************************************************
* ������:TelitProc
* �䡡��:
*			pBASE_BUF			pBaseBuf      ȫ�ֻ���
* �䡡��:
* ��  ��:
*		
* ��  ��: 
****************************************************************************/
INT32S TelitProc(pBASE_BUF pBaseBuf, pBASE_BUF pBaseBuf1,pBASE_BUF pBaseBuf2,INT8U nLinkType)
{
	// 1. AT����
	TelitAtCmd(pBaseBuf);
	
	// 2. ��ȡ��Ӧ����
	// 2.1 ��ʾ��
	TelitInfoDataProc(pBaseBuf);
	// 2.2 �ͻ���/����������
	TelitRcvData(pBaseBuf, pBaseBuf1, pBaseBuf2, nLinkType);

	return 0;
}

/********************************************************************************
* ������:
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: TCP��·���ʹ���
*********************************************************************************/
void TltTcpSend(INT8U nId, INT8U* pData, INT32S nSize)
{
	INT8U  		 	nIndex   		= 0;
	INT8U  		 	nSendBuf[30]	= {0};	
	INT8U  		 	nTempBuf[5]	= {0};

	if (NULL == pData)
	{
		return ;
	}
	if (0 == nSize)
	{
		return ;
	}
	//AT#SSENDEXT=1,100
	memset(nSendBuf, 0 , sizeof(nSendBuf));
	if (1 == nId)
	{
		strcat((char*)&nSendBuf[0], (char*)"AT#SSENDEXT=1,");
	}
	else
	{
		strcat((char*)&nSendBuf[0], (char*)"AT#SSENDEXT=2,");
	}
	nIndex = strlen((char*)&nSendBuf[0]);
	memset(&nTempBuf[0], 0, sizeof(nTempBuf));
	ValToAscii(&nTempBuf[0],(INT32U)nSize, sizeof(nTempBuf));
	strcat((char*)&nSendBuf[0], (char*)&nTempBuf[0]);
    nIndex = strlen((char*)&nSendBuf[0]);
	nSendBuf[nIndex++] = 0x0D;
		
	if (SendAtCmdAndWaitATResp((char*)nSendBuf, (char*)"=", 10000, 1, 1, NULL, 0) < 0)
	{
		return;	
	}
	WriteComm(COMM_GPRS, (INT8U*)(pData), nSize);
    
	if (GetRmotCmdPrintFlg())
	{
		Gw3761FrmWrite(COMM_WH232, (INT8U*)(pData), nSize);
	}
	else
    {
        //OSTimeDlyHMSM(0, 0, 0, 100);
    }
	return;
}

/********************************************************************************
* ������:
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ģʽ�л�
*********************************************************************************/
INT32S ModeChangeData(INT8U nData)
{
    INT32S   nRet    = 0;
    
    if (nData == 1)
    {
        if (SendAtCmdAndWaitATRespSlip((char*)"AT#SO=1\r\n", (char*)g_Modl_OK, 5000, 1) < 0)
        {
            nRet = -1;	
        }
    }
    else if (nData == 2)
    {
        if (SendAtCmdAndWaitATRespSlip((char*)"AT#SO=2\r\n", (char*)g_Modl_OK, 5000, 1) < 0)
        {
            nRet = -1;
        }
    }
    else if (nData == 3)
    {
        if (SendAtCmdAndWaitATRespSlip((char*)"AT#SA=2,1\r\n", (char*)g_Tlt_SrvrRcv, 5000, 1) < 0)
        {
            nRet = -1;
        }
    }
    return nRet;
}

/********************************************************************************
* ������:
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ģʽ�л�
*********************************************************************************/
INT32S ModeChangeAT(void)
{
    INT32S  nRet    = 0;
    
    if (SendAtCmdAndWaitATRespSlip((char*)"+++", (char*)g_Modl_OK, 5000, 1) < 0)
	{
		nRet = -1;	
	}
    return nRet;
}








