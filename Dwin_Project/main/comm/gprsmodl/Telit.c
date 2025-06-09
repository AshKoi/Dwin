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
#include "Telit.h"
#include "Base.h"
#include "TimerCount.h"
#include "SoftTimer.h"
#include "Fun.h"
#include "GB3761SetPrmExt.h"
/**************************************************************************************************
                                           宏
***************************************************************************************************/

//INT32S SendAtCmdAndWaitATResp(const char *pcDataAddr,const char *pKeyWord, INT32S nTimeOut, INT8U nCnt, INT16U nSemFlg, INT8U *pBuf, INT32U nSize)


/**************************************************************************************************
                                           变量
***************************************************************************************************/
INT8U        nCSQFlg        = 0;           //信号强度检测标志
INT8U        nSIMReg        = 0;           //sim卡注册标志
INT8U        nAPNConfig     = 0;           //APN配置信息
INT8U        nUserFlg       = 0;           //配置用户名信息标志
INT8U        nPWFlg         = 0;           //配置密码标志
INT8U        nIpPort        = 0;           //获取IP和端口号
INT8U        nDialFlg       = 0;           //拨号，正在连接主站
INT8U        nMessageFlg    = 0;           //登陆接收报文


/**************************************************************************************************
                                           声明
***************************************************************************************************/
const	char	g_Tlt_ClntRcv[] 	  = "RQHMF9\x1f\x30\x2B";			//"SRING: 1,"
const	char	g_Tlt_SrvrRcv2[] 	  = "RQHMF9\x1f\x31\x2B";			//"SRING: 2,"
const	char	g_Tlt_SrvrRcv[] 	  = "RQHMF9\x1f\x31";			//"SRING: 2"
//const	char	g_Tlt_SrvrRcv2[]      = "RH9\x1f\x31\x2B";           //"SI: 2,"

const	char	g_Tlt_SmsInfo[] 	  = "*BLSH9\x1F!RL!+";				//"+CMTI: "SM","
const	char	g_Tlt_SmsHead[] 	  = "*BLFQ9\x1F";					//"+CMGR: "

const	char	g_Tlt_MasterCls[]	  = "MN\x1f\x42@QQHDQ"; 			//"NO CARRIER"			掉线提示	


/**************************************************************************************************
                                           相关函数实现
***************************************************************************************************/
/****************************************************************************
* 函数名:TelitAtCmd
* 输　入:
* 输　出:
* 返  回:
*		
* 功  能: 
****************************************************************************/
static INT32S  TelitStart(void)
{
	pTTIMER	pTTimer 	= NULL;
	INT16U	nTmp		= 0;
	
	if (0 == g_pRmotValClr->nNdStart)
	{
		return 0;		
	}
    //如果设置了GPRS电源灯故障，则不重新上电
    if (g_nGprsLedFlg)
    {
        return 0;
    }
    
	OSTimeDlyHMSM(0,0,5,100);
	for (;;)
	{
        //+++++ 清显示的上行状态
        

        //模块开机
		OSTimeDlyHMSM(0,0,0,100);
		ModlOnOff(FALSE);									//模块OFF：置口线低
		ModlPowerCtrl(FALSE);								//模块电源切断
		OSTimeDlyHMSM(0,0,2,0);									
		
		ModlPowerCtrl(TRUE);								//模块电源供电
		OSTimeDlyHMSM(0,0,1,200);
		ModlOnOff(TRUE);									//模块ON
		
		if (ModlBpsAdpt(10) < 0)								//模块波特率自适应
		{
			return RET_ERR_ERROR;
		}
		ModlGetHardVer();									//获取模块硬件版本

		memset(g_pRmotValClr, 0, sizeof(RMOT_VAL_CLR));

		// 心跳
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
		// 无通信
		pTTimer = &g_pTimer[TIMER_NOCOMM];
		TimerClose(pTTimer);
		pTTimer = &g_pTimer[TIMER_SRVCNOCOMM];
		TimerClose(pTTimer);
		
		// 自检
		pTTimer = &g_pTimer[TIMER_SELFCHK];
		TimerClose(pTTimer);

		// 信号强度
		pTTimer = &g_pTimer[TIMER_SGNL_ITVL];
		nTmp = MODL_RDSIGNAL_INTVL;
		TimerOpen(pTTimer, nTmp);
		break;
	}
	
	g_pRmotValClr->nNdStart = 0;							// 清重启标志
	
	return 0;	
}

/*
***************************************************************************
* 函数名:TltLedTurnOnWay
* 输　入:
* 输　出:
* 返  回:
* 调  用:
* 作　者:wyf
* 日　期:2009/10/03
* 版  本:Version 1.0
* 功能描述:模块灯亮的方式
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
* 函数名:TltRdSignal
* 输　入:
* 输　出:
* 返  回:
* 调  用:
* 作　者:wyf
* 日　期:2009/10/03
* 版  本:Version 1.0
* 功能描述:读取信号强度
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
* 函数名:TltRdSignalExt
* 输　入:
* 输　出:
* 返  回:
* 调  用:
* 作　者:wyf
* 日　期:2009/10/03
* 版  本:Version 1.0
* 功能描述:信号量
****************************************************************************/
static INT32S TltRdSignalExt(void)
{	 	
	pTTIMER	pTTimer 	= &g_pTimer[TIMER_SGNL_ITVL];
	INT16U	nTmp		= MODL_RDSIGNAL_INTVL;

	if (0 != TimerValGet(pTTimer))
	{
		return 0;
	}
	// 刷新重拨定时器
	TimerValSet(pTTimer, nTmp);

	return TltRdSignal();
}


/****************************************************************************
* 函数名:TltSimReg
* 输　入:
* 输　出:
* 返  回:
* 调  用:
* 作　者:wyf
* 日　期:2009/10/03
* 版  本:Version 1.0
* 功能描述:SIM卡注册
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
* 函数名:TltSimRegExt
* 输　入:
* 输　出:
* 返  回:
* 调  用:
* 作　者:wyf
* 日　期:2009/10/03
* 版  本:Version 1.0
* 功能描述:SIM卡注册扩展
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
* 函数名:TelitBaseCmd
* 输　入:
* 输　出:
* 返  回:
*		
* 功  能: 
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
	if (SendAtCmdAndWaitATResp((char*)"AT+GMR\x0d", (char*)g_Modl_OK, 1000, 1, 0, &nTmpBuf[0], FOUR_BYTE_HIGH_BIT + sizeof(nTmpBuf)) < 0)//读版本
	{
		//return -1;	//版本读不通不影响功能，故不退出
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
		//return;	//wave模块不能往下执行，Telit的可以执行，有方的待试，暂时和telit的一样
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
* 函数名:TltSetAPN
* 输　入:
* 输　出:
* 返  回:
* 调  用:
* 作　者:wyf
* 日　期:2009/10/03
* 版  本:Version 1.0
* 功能描述:配置泰利特APN
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
* 函数名:TltRefreshLocalIpAndPort
* 输　入:
* 输　出:
* 返  回:
* 调  用:
* 作　者:wyf
* 日　期:2009/10/03
* 版  本:Version 1.0
* 功能描述:获取SIM卡IP
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

	if (k < 4)//IP地址有4部分：没有获得4部分则退出
	{
		return;	 	
	}
	memcpy(&g_pRomtParm->nTermIp[0], &nTermIp[0], 4);
    nIpPort = 1;
#undef MAX_QUERY_IPPORT_LEN
}

/****************************************************************************
* 函数名:TltSetUserName
* 输　入:
* 输　出:
* 返  回:
* 调  用:
* 作　者:wyf
* 日　期:2009/10/03
* 版  本:Version 1.0
* 功能描述:配置泰利特用户名
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
* 函数名:TltSetUserPwd
* 输　入:
* 输　出:
* 返  回:
* 调  用:
* 作　者:wyf
* 日　期:2009/10/03
* 版  本:Version 1.0
* 功能描述:配置泰利特密码
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
* 函数名:TltPPPCfg
* 输　入:
* 输　出:
* 返  回:
* 调  用:
* 作　者:wyf
* 日　期:2009/10/03
* 版  本:Version 1.0
* 功能描述:模块初始的基本配置
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
* 函数名:TltPPPCfgCls
* 输　入:
* 输　出:
* 返  回:
* 调  用:
* 作　者:wyf
* 日　期:2009/10/03
* 版  本:Version 1.0
* 功能描述:模块PPP关闭
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
* 函数名:TltTcpOpenPort
* 输　入:
* 输　出:
* 返  回:
* 调  用:
* 作　者:wyf
* 日　期:2009/10/03
* 版  本:Version 1.0
* 功能描述:TCP侦听端口打开
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
	ValToAscii(&nBuf[nIndex], (INT32U)nDetectPort, 5);//端口不会超过5位
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
* 函数名:TltTcpOpenPortCls
* 输　入:
* 输　出:
* 返  回:
* 调  用:
* 作　者:wyf
* 日　期:2009/10/03
* 版  本:Version 1.0
* 功能描述:TCP侦听端口关闭
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
	ValToAscii(&nBuf[nIndex], (INT32U)nDetectPort, 5);//端口不会超过5位
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
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 检查IP及端口合法性
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
* 函数名:TltAtSendIPport
* 输　入:
* 输　出:
* 返  回:
* 调  用:
* 作　者:wyf
* 日　期:2009/10/03
* 版  本:Version 1.0
* 功能描述:形成泰利特的IP和端口
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
		nIndex += ValToAscii(&pOutBuf[nIndex], (INT32U)(pBuf[i]), 3);//ip不会超过3位
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
* 函数名:TltClntTcpSckt
* 输　入:
* 输　出:
* 返  回:
* 调  用:
* 作　者:wyf
* 日　期:2009/10/03
* 版  本:Version 1.0
* 功能描述:客户机TCP链接
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
* 函数名:TltClntTcpScktCls
* 输　入:
* 输　出:
* 返  回:
* 调  用:
* 作　者:wyf
* 日　期:2009/10/03
* 版  本:Version 1.0
* 功能描述:客户机TCP链接关闭
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
* 函数名:TltClntTcpScktExt
* 输　入:
* 输　出:
* 返  回:
* 调  用:
* 作　者:wyf
* 日　期:2009/10/03
* 版  本:Version 1.0
* 功能描述:客户机TCP链接
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
	
	// 客户机模式
	if (1 == g_pRomtParm->nWorkMode)
	{
		// 登录成功时,不去登录
		if (0 == g_pRmotValClr->nLoginSucc)
		{
            MemReadRamXmpData(F9, 0, &nLinkReq, sizeof(nLinkReq));
            if (nLinkReq == 1)//F38
            {
                nLinkReq = 0;
                MemWriteRamXmpData(F9, 0, &nLinkReq, sizeof(nLinkReq));
                // 拨号
                nRet = TltClntTcpSckt();
            }
            else
            {
                if (1 == g_pRomtParm->nOnlineType)//永久在线
                {
                    // 检查重拨定时器，必须打开
                    if (0 == TimerStatGet(pTTimer))
                    {
                        TimerOpen(pTTimer, nTmp);
                    }
                    // 重播间隔到，拨号。同时刷新重拨定时器
                    if (0 != TimerValGet(pTTimer))
                    {
                        // 重拨间隔未到,直接退出
                        return -1;		
                    }
                    // 刷新重拨定时器
                    TimerValSet(pTTimer, nTmp);

                    // 拨号
                    nRet = TltClntTcpSckt();
                }
            }	
		}
		else
		{
			TimerClose(pTTimer);
			if (1 == g_pRmotValClr->nLoginSucc)
			{
				// 登录报文
				TcpLinkFrame(F1);
				g_pRmotValClr->nLoginSucc = 2;
			}
			return 0;	
		}
	}
	else
	{
		// 登录成功时,不去登录
		if (g_pRmotValClr->nLoginSucc)
		{
			TimerClose(pTTimer);

			// 也可以放到TelitAtCmd里面的"4. 无通讯2分钟断开链接"
			pTTimer = &g_pTimer[TIMER_NOCOMM];
			if (0 == TimerStatGet(pTTimer))
			{
				TimerOpen(pTTimer, nTmp);
			}
			return 0;	
		}

		// 重播间隔到，拨号。同时刷新重拨定时器
		if (0 != TimerValGet(pTTimer))
	   	{
	   		return -1;	
	   	}
		TimerValSet(pTTimer, nTmp);

		// 拨号
		nRet = TltClntTcpSckt();
		// 登录报文
		TcpLinkFrame(F1);
        
        //
        //ModeChangeAT();
        
		//g_pRmotValClr->nLoginSucc = 2;
		
		// 混合模式下，只登录3次。关闭定时器
		if (g_nLoginCnt >= 3)
		{
			TimerClose(pTTimer);		
		}		
	}
	return nRet;
}

/****************************************************************************
* 函数名:TltTcpBeat
* 输　入:
* 输　出:
* 返  回:
* 调  用:
* 作　者:wyf
* 日　期:2009/10/03
* 版  本:Version 1.0
* 功能描述:心跳
****************************************************************************/
static INT32S TltTcpBeat(void)
{
	pTTIMER			pTTimer 	= &g_pTimer[TIMER_BEAT];
	INT16U			nTmp 		= 0;
	
	// 客户机模式下，心跳报文
	if (1 == g_pRomtParm->nWorkMode)
	{
		nTmp = GET_MIN(GET_MAX(g_pRomtParm->nBeatIntvl, 60), 900);
		if (2 == g_pRmotValClr->nLoginSucc)
		{
			// 心跳
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
	// 混合模式
	else
	{
		// 打开在初始化里面已经打开计时器
		if (0 != TimerValGet(pTTimer))
		{
			return 0;
		}
		// 在规约层处理中，发现有心跳报文，置 TimerOpen(pTTimer, nTmp);
		
		g_pRmotValClr->nNdStart = 1;
		
		return 0;
	}
    
    //return 0;
}


/****************************************************************************
* 函数名:TelitAtCmd
* 输　入:
*			pBASE_BUF			pBaseBuf      全局缓冲
* 输　出:
* 返  回:
*		
* 功  能: 
****************************************************************************/
static INT32S TelitAtCmd(pBASE_BUF pBaseBuf)
{
	pTTIMER			pTTimer 		= NULL;
	STD_TIME		sStdTime;
	static INT16U	s_nSignalCnt 	= 0;
    INT8U           nLinkReq        = 0; 	

	pBaseBuf = pBaseBuf;
		 
	// 1. 上电启动
	TelitStart();

	// 1.1. 基本、PPP、信号强度
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

	// 2. 侦听
	TltTcpOpenPort();
	
	// 3. 拨号
	TltClntTcpScktExt();
	

	// 4. 无通讯2分钟断开链接
	if (1 == g_pRomtParm->nWorkMode)
	{
        MemReadRamXmpData(F9, 0, &nLinkReq, sizeof(nLinkReq));
        // 客户机模式
        if (nLinkReq == 2)//F39
        {
            nLinkReq = 0;
            MemWriteRamXmpData(F9, 0, &nLinkReq, sizeof(nLinkReq));
            TltClntTcpScktCls();
        }
        else
        {
            
            if (2 == g_pRomtParm->nOnlineType)//被动激活模式
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
		// 混合模式
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
	// 5. 心跳
	TltTcpBeat();
	// 6、自检
	pTTimer = &g_pTimer[TIMER_SELFCHK];
	if (0 == TimerValGet(pTTimer))
	{
		TltTcpOpenPortCls();
		TltTcpOpenPort();
	}
	
	// 7、一天不上线
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
* 函数名:TelitInfoDataProc
* 输　入:
*			pBASE_BUF			pBaseBuf      全局缓冲
* 输　出:
* 返  回:
*		
* 功  能: 
	1. 链接断开标志
	2. 服务器链接标志
****************************************************************************/
static INT32S TelitInfoDataProc(pBASE_BUF pBaseBuf)
{
	INT8U* 	pLabel			= NULL;
	INT32S	nBasePtr		= -1;
	INT8U	nFlg			= 0;
	pTTIMER	pTTimer 		= NULL;
	INT16U	nTmp 			= 0;
    INT32U  nLabelLen       = 0;

	// 1. 链接断开标志
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
	// 2. 服务器链接标志
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
            
            // AT#SA=2,1    返回 SRING: 2
            ModeChangeData(3);
		}
        
	}
#endif	
	return 0;
}

/****************************************************************************
* 函数名:TelitRcvDataTimerProc
* 输　入:
*				INT8U			nLinkType
* 输　出:
* 返  回:
*		
* 功  能:  接收到有效数据时，处理基本定时器
****************************************************************************/
static INT32S TelitRcvDataTimerProc(INT8U nLinkType)
{
	pTTIMER	pTTimer 		= NULL;
	INT16U	nTmp 			= 0;
	
	// 心跳
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

	// 无通信
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

	// 自检
	pTTimer = &g_pTimer[TIMER_SELFCHK];
	nTmp = MODL_SELF_INTVL;
	TimerOpen(pTTimer, nTmp);

	// 信号
	pTTimer = &g_pTimer[TIMER_SGNL_ITVL];
	nTmp = MODL_RDSIGNAL_INTVL;
	TimerOpen(pTTimer, nTmp);

	return 0;
}

/****************************************************************************
* 函数名:TelitRcvData
* 输　入:
*			pBASE_BUF			pBaseBuf      	全局缓冲
*			INT8U				nLinkType		客户/服务
* 输　出:
* 返  回:
*		
* 功  能: 
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
            // 计算接受长度
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
            // 长度合法性判断
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
            // 记录住接收到的客户机/服务器通道的信息:g_nTotalRmotBufPtr  g_nTotalRmotBufLen   通过RmotRecv接收数据到具体链路应用
                //RmotBufValidDataInfoMake((nBasePtr + nLabelLen + j + 1) % pBaseBuf->nBufSize, nLen);
                // 从pBaseBuf(nBasePtr + nLabelLen + j + 1, nLen) -----> pBaseBuf1
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

            // 接收到有效数据时，处理基本定时器
            TelitRcvDataTimerProc(nLinkType);
        }
	}


	

	return nRet;
}



#if 0
#endif
/****************************************************************************
* 函数名:TelitProc
* 输　入:
*			pBASE_BUF			pBaseBuf      全局缓冲
* 输　出:
* 返  回:
*		
* 功  能: 
****************************************************************************/
INT32S TelitProc(pBASE_BUF pBaseBuf, pBASE_BUF pBaseBuf1,pBASE_BUF pBaseBuf2,INT8U nLinkType)
{
	// 1. AT命令
	TelitAtCmd(pBaseBuf);
	
	// 2. 读取相应数据
	// 2.1 提示语
	TelitInfoDataProc(pBaseBuf);
	// 2.2 客户机/服务器数据
	TelitRcvData(pBaseBuf, pBaseBuf1, pBaseBuf2, nLinkType);

	return 0;
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: TCP链路发送处理
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
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 模式切换
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
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 模式切换
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








