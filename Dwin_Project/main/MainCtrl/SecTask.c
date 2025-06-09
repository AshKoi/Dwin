/******************************************************************************************************
* 2013-2014 益邦科技有限公司
* 文件名: 
* 创建人: 王一凡
* 日　期: 2013/09/21
* 修改人: 
* 日　期: 
* 描　述: 
* 版　本: V1.0
*****************************************************************************************************/

#include "SecTask.h"
#include "SysRes.h"
#include "Base.h"
#include "Fun.h"
#include "PowerOn.h"
#include "SoftTimer.h"
#include "MemDrvExt.h"
#include "MemDrv.h"
#include "MemTbl.h"
#include "DataItem.h"
#include "AutoTest.h"
#include "Main.h"
#include "Ac7022.h"
#include "Guimenu.h"
#include "FileTrans.h"
#include "PulseCol.h"
#include "Key.h"

/**************************************************************************************************
                                          相关全局变量实现
***************************************************************************************************/
extern const INT8S   g_nLedBackArray[];
extern const INT8S   g_nTermCloseArray[];
extern INT8S   g_LedBackCloseTime;
extern INT8S   g_TermCloseTime;

extern INT8U   g_MeterHardwareWrong[2];
extern INT8U   g_MeterInsideWrong[2];


INT8S   g_nUartBpsflg = 3;

INT8U   g_IsNewMonCalcRDayMon = 0;
INT8U   g_IsNewMonCalcRDayFlg = 0;

extern  INT8U  Data1[35];
extern  INT8U  Data2[23];

INT8U                g_nFlg;
INT8U                g_nAutoTime;
INT8U                g_nFlgUa;
INT8U                g_nFlgUb;
INT8U                g_nFlgUc;
INT8U               g_nFlgVoltPhase;
INT8U                g_nFlgIa;
INT8U                g_nFlgIb;
INT8U                g_nFlgIc;
INT8U                   g_nFlg_Commu;
INT8U                   g_nFlg_Commu2;
INT8U                    g_nFlg_Alarm;

INT8U                    g_nFlg_Pro;

INT8U                    g_nFlgProt;
INT8U                    g_nFlgProt1;
INT8U                    g_nFlg_Disp;

INT8U                   g_nAutoTestFlg1;
INT8U                   g_nFlg2;

INT8U                g_nFlgBattery ;
INT8U               g_nFlgnRateNo;
STD_TIME    g_sSysTime;
pSTD_TIME   g_pSysTime = &g_sSysTime;

INT8U           g_ZBPortFault=0;
INT8U           g_MeterAcErrFlg[4] = {0}; //1:飞走 2:停走 3:倒走 4: 不平衡 
INT8U           g_MeterVoltPlusFlg = 0;

INT8U           g_MeterPowerOff=0;

INT8U   g_nFlgReBoot = 0;//g_nFlgHardType
INT8U Wrongtime1; //用于判断1s
INT8U second1;   //电流不平衡时间标志
INT8U second2;  //功率因数超限时间标志
INT8U second3;  //超有功需量时间标志
INT8U second4;  //有功电能反向时间标志
INT8U second5;//过压时间标志
/**************************************************************************************************
                                          相关函数实现
***************************************************************************************************/
extern INT8S SysQueryTaskPrio(INT8U nId, INT8U* pPrio);
extern void  DispSecProc(void);
extern void  DispMinProc(void);
extern void  SaveYouhuaIdx(void);

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 下载成功复位处理
*********************************************************************************/
static void SecOtherDownloadProc(void)
{

	DOWNLOAD_STR	sDownloadStr;
	INT8U			nTmpBuf[16] 	= {0};
	INT32U			nLen			= 0;
	
	DownloadStrRd(1, (INT8U*)&sDownloadStr);
	if (TRUE == sDownloadStr.bSucc)
	{
		//清掉下载成功标志
		sDownloadStr.bSucc = FALSE;
		DownloadStrWr(1, (INT8U*)&sDownloadStr);
		
		//置boot需要更新程序标志(1为有新程序更新)以及把前16个字节写到相应flash位置	
		_API_FLASH_Read(&nTmpBuf[0], MEM_FLH_UPPROGRAM_ADDR, 16);
		//MemRead(SELF_UPDATA_FILE, MEM_FLASH, 0, 16, &nTmpBuf[0]);
		nTmpBuf[0] = 1;
		_API_FLASH_EraseWrite(&nTmpBuf[0], 0, 16);
		
		//复位	
		ResetDelayHardInit(5 * 1000);
	}
#if 0
	DownloadStrRd(CARR_FILE_ID, (INT8U*)&sDownloadStr);
	if (TRUE == sDownloadStr.bSucc)
	{
		//清掉下载成功标志
		sDownloadStr.bSucc = FALSE;
		DownloadStrWr(CARR_FILE_ID, (INT8U*)&sDownloadStr);

		memset(&nTmpBuf[0], 0, 16);
		//_API_FLASH_Read(&nTmpBuf[0], MEM_FLH_CARR_UP_ADDR, 16);
		MemRead(CARR_UPDATA_FILE, MEM_FLASH, 0, 16, &nTmpBuf[0]);
		nLen = Buf2Long(&nTmpBuf[8], 4) + 16;
		
		MemCopyFile(CARR_DOWN_FILE, MEM_FLH_CARR_SV_ADDR, CARR_UPDATA_FILE, MEM_FLH_CARR_UP_ADDR, GET_MIN(nLen, MEM_FLH_CARR_UP_SIZE), MEM_FLASH);
		nTmpBuf[0] = 1;
		//_API_FLASH_EraseWrite(&nTmpBuf[0], MEM_FLH_CARR_SV_ADDR, 1);
		MemRead(CARR_DOWN_FILE, MEM_FLASH, 0, 1, &nTmpBuf[0]);
	}

	DownloadStrRd(G2U_FILE_ID, (INT8U*)&sDownloadStr);
	if (TRUE == sDownloadStr.bSucc)
	{
		//清掉下载成功标志
		sDownloadStr.bSucc = FALSE;
		DownloadStrWr(G2U_FILE_ID, (INT8U*)&sDownloadStr);

	}
#endif
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 以太网处理
*********************************************************************************/
static void SecOtherEnetProc(void)
{
#if ENET_COMM_FUNC_EN == 1 	
	INT8U	  		nPrio 	= 0;
    INT8U     		nErr  	= 0;
    INT16U    		nCnt	= 0;
	static OS_TCB   s_sOsTcb;
	
	nCnt = OSSemAccept(g_pSem[SEM_ENET_FREE_IDX]);
	if (0 == nCnt)
	{
		return;
	}
	
	//以太网服务器
	if (RET_ERR_OK != SysQueryTaskPrio(TASK_ENETSRVLINK, &nPrio))
	{
		ResetDelayHardInit(0);
	}
	memset(&s_sOsTcb, 0, sizeof(s_sOsTcb));
	nErr = OSTaskQuery(nPrio, &s_sOsTcb);
	if(OS_NO_ERR == nErr)
	{
		OSTaskDel(nPrio);
	}
	
	//以太网客户机
	if (RET_ERR_OK != SysQueryTaskPrio(TASK_ENETCLILINK, &nPrio))
	{
		ResetDelayHardInit(0);
	}
	memset(&s_sOsTcb, 0, sizeof(s_sOsTcb));
	nErr = OSTaskQuery(nPrio, &s_sOsTcb);
	if(OS_NO_ERR == nErr)
	{
		#if 0
        if (IsNeedPPPoE())
        {
            //pEthernet->PPPoEDisconnect();
        }
		#endif
		OSTaskDel(nPrio);
	}
	
	if (RET_ERR_OK != SysTaskCreate(TASK_ENETCLILINK))	
	{
		ResetDelayHardInit(0);
	}
	if (RET_ERR_OK != SysTaskCreate(TASK_ENETSRVLINK))	
	{
		ResetDelayHardInit(0);
	}
#endif	
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 秒
*********************************************************************************/
static void SecTaskSecProcPre(void)
{
    INT8U   nFlg    = 0;
    static INT8U ii       = 0;

     MEM_AC_REAL_DATA            sRealData;             //使用全局变量
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    MemReadDbfExt(MEM_OBJ_X_RAM_DATA, 0, F5, &nFlg, 1, NULL);
    if (nFlg)
    {
        nFlg = 0;
        MemWriteDbfExt(MEM_OBJ_X_RAM_DATA, 0, F5, &nFlg, 1, NULL);
        ResetDelayHardInit(1000);
    }
    //if ((ii++) % 2)
    {
        LED_RUN_ON();
    }
  //  else
    {
        LED_RUN_OFF();
    }
    //显示时间
    //LcdDisplayTime();
 //   DisSec();
    
    SecOtherEnetProc();
	SecOtherDownloadProc();
	DispSecProc();
        
   //
    Wrongtime1++;
    if(Wrongtime1==1)
    {
    Wrongtime1=0;
    second1=1;
    second2=1; 
    second3=1; 
    second4=1; 
    second5=1;
    }
   g_nCmt++;
   if(((g_MeterHardwareWrong[0]&0x02)==0x02)||((g_MeterInsideWrong[0]&0x08)==0x08)) //白屏及死机命令
   {
      g_nCmt = 10;//
   }
   if(g_nCmt >= 0xFE) 
   {
     g_nCmt = 41;
   }
   if (g_nCmt > AutoDisplayCmt)
   {     GpioSetOutput(IO_PORT7, IO_PINx5);       
       GpioClearOutput(IO_PORT2, IO_PINx1); //取消电池供电
		g_nFlg_Disp = 1;
        g_nAutoTime++;
        if (g_nAutoTime >= 10)
        {
            g_nAutoTime = 0;
            g_nFlg = 1;
        }
   }
   
  if((1 == g_nFlgReBoot)&&(1 == g_nFlg_Commu2))
  {
      g_nFlgReBoot = 0;
      	
      ResetDelayHardInit(000);//复位  切换电表类型后
  }
      
  if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType)||(TRIPHASE_METER_57U == g_nHardType)||(TRIPHASE_METER_100 == g_nHardType) ||(TRIPHASE_METER_100U == g_nHardType)|| (TRIPHASE_METER_220ZT == g_nHardType))
  {      
     TriphaseEventDisp();
  } 
  else if(SINGLEPHASE_METER == g_nHardType)
  {
   SinglephaseEventDisp();
  }
   
	
    if(1 == g_nFlgProt1)
   {
      g_nFlgProt1 = 0;
     
      MemWriteRamXmpData(F8, 0, &g_pMem->sXRamData.nRdMtProtFlg, 1);
      
      if(g_pMem->sXRamData.nRdMtProtFlg == 1)
      {
          SetCommCfg(1, 3, 2);   //SetCommCfg(1, 4, 2);     //2400 偶校验
          OpenCommPort(COMM_WH485);
          
          SetCommCfg(1, 3, 2);   //SetCommCfg(1, 4, 2);     //2400 偶校验
          OpenCommPort(COMM_WH4852);
      }
      else if (g_pMem->sXRamData.nRdMtProtFlg == 2)
      {
          SetCommCfg(1, 6, 2);        //9600 偶校验
          OpenCommPort(COMM_WH485);
          
          SetCommCfg(1, 6, 2);        //9600 偶校验
          OpenCommPort(COMM_WH4852);
      }
      else if(g_pMem->sXRamData.nRdMtProtFlg == 3)
      {
          SetCommCfg(1, 6, 2);        //9600 偶校验
          OpenCommPort(COMM_WH485);
          
          SetCommCfg(1, 6, 2);        //9600 偶校验
          OpenCommPort(COMM_WH4852);
      } 
   }

}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 秒
*********************************************************************************/
INT8U g_ClockWrong=0;
INT8U g_ClockWrong2=0;
static void SecTaskSecProcBack(void)
{
    INT32U nVolbat  = 0;
    INT32U nVolbat2 = 0;
    INT32U nTmp32U  = 0;  
    static INT8U  nAddNum  = 0; 

    nVolbat = GetClockVbat();
    nVolbat2 = GetAD0Voltage();
    
    if(nVolbat<4400) //电压小于3.3V
    {
        g_ClockWrong =1;
    }
    else
    {
        g_ClockWrong = 0;
        nAddNum++;
        if(nAddNum == 34)
        {
            MemReadAcXBackData(F39, &nTmp32U, sizeof(nTmp32U));
            nTmp32U ++;
            MemWriteAcXBackData(F39, &nTmp32U, sizeof(nTmp32U));
            nAddNum = 0;
        }
        
    }

    //主控电池
    if(nVolbat2<4000) //电压小于3.3V
    {
        g_ClockWrong2 =1;
    }
    else
    {
        g_ClockWrong2 =0;
    }
    
}



/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 是否过日
*********************************************************************************/
BOOLEAN IsNewDay(const pSTD_TIME pTime)
{
	FORMAT20    sHistTime;

    if (NULL == pTime)
    {
        return FALSE;
    }
    //获取时标信息表存储的当前日期
    if (RET_ERR_OK != MemFindHistIdxSub(0, &sHistTime, NULL, 1))
    {
        return FALSE;
    }
    if (0 != memcmp(&sHistTime, &pTime->nDay, sizeof(FORMAT20)))
    {
    	return TRUE;
    }
    return FALSE;
	
}


/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 是否过月
*********************************************************************************/
BOOLEAN IsNewMon(const pSTD_TIME pTime)
{
	FORMAT21    sHistTime;
	INT16U      nHistIdx  = 0;
    STD_TIME    sTmpTime;
    INT32S      nSecs   = 0;

    if (NULL == pTime)
    {
        return FALSE;
    }
    
    memset(&sTmpTime, 0, sizeof(sTmpTime));
    sTmpTime.nDay = 1;
    
    //获取时标信息表存储的当前日期
    if (RET_ERR_OK != MemFindHistIdxSub(0, &sHistTime, &nHistIdx, 2))
    {
        return FALSE;
    }
    if (0 != memcmp(&sHistTime, &pTime->nMon, sizeof(FORMAT21)))
    {
    	//同时当前时间要大于存储的时间算过日
        memcpy(&sTmpTime.nMon, &sHistTime, 2);
        GetDiffTimeSecs(pTime, &sTmpTime, &nSecs);
        if (nSecs > 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 过日数据存储信息处理
*********************************************************************************/
INT8S NewDayMemInfoProc(const pSTD_TIME pTime)
{
    DAY_MEM_INFO 	sDayInfo;

    if (NULL == pTime)
    {
        return RET_ERR_POINT;
    }

    //获取日数据存储信息
    memset(&sDayInfo, 0, sizeof(sDayInfo));
    if (RET_ERR_OK != MemReadDbfExt(MEM_OBJ_X_FLH_DATA, 0, F2, &sDayInfo, sizeof(DAY_MEM_INFO), NULL))
    {
        return RET_ERR_ERROR;
    }
    sDayInfo.nNext = (sDayInfo.nNext + 1) % MAX_DAY_DATA_NUM;
    (sDayInfo.nMemNum < MAX_DAY_DATA_NUM) ? (sDayInfo.nMemNum++) : (sDayInfo.nMemNum = MAX_DAY_DATA_NUM);
    memcpy(&sDayInfo.sTime[sDayInfo.nNext], &pTime->nDay, sizeof(FORMAT20));
    MemWriteDbfExt(MEM_OBJ_X_FLH_DATA, 0, F2, &sDayInfo, sizeof(DAY_MEM_INFO), NULL);

    return RET_ERR_OK;
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 过月数据存储信息处理
*********************************************************************************/
INT8S NewMonMemInfoProc(const pSTD_TIME pTime)
{
    MON_MEM_INFO 	sMonInfo;

    if (NULL == pTime)
    {
        return RET_ERR_POINT;
    }

    //获取日数据存储信息
    memset(&sMonInfo, 0, sizeof(sMonInfo));
    if (RET_ERR_OK != MemReadDbfExt(MEM_OBJ_X_FLH_DATA, 0, F3, &sMonInfo, sizeof(MON_MEM_INFO), NULL))
    {
        return RET_ERR_ERROR;
    }
    sMonInfo.nNext = (sMonInfo.nNext + 1) % MAX_MON_DATA_NUM;
    (sMonInfo.nMemNum < MAX_MON_DATA_NUM) ? (sMonInfo.nMemNum++) : (sMonInfo.nMemNum = MAX_MON_DATA_NUM);
    memcpy(&sMonInfo.sTime[sMonInfo.nNext], &pTime->nMon, sizeof(FORMAT21));
    MemWriteDbfExt(MEM_OBJ_X_FLH_DATA, 0, F3, &sMonInfo, sizeof(MON_MEM_INFO), NULL);

    return RET_ERR_OK;
}

/********************************************************************************
* 函数名:PulseDayProc
* 输　入:
* 输　出:
* 返  回:
* 功  能: 
*********************************************************************************/
INT8S PulseDayProc(void)
{
    INT8U       i   = 0;
    INT8U       j   = 0;
    INT8U       k   = 0;
    MEM_PULSE_PRM   sPulsePrm;
    
    for (i = 0; i < MAX_METER_NUM; i++)
    {
        memset(&sPulsePrm, 0, sizeof(sPulsePrm));
        if (GetPulsePrm(i + 1, &sPulsePrm) < 0)
        {
            continue;
        }
        j++;
        k = j - 1;
        
        //将当前各费率、各路脉冲个数转成日冻结示值进行存储
    }
    return 0;
}


/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 过日常规处理
*********************************************************************************/
INT8S NewDayDataProc(const pSTD_TIME pTime)
{
	//1.过日脉冲处理
#if PUL_CALC_FUNC_EN == 1
    PulseDayProc();
#endif
    //交采日数据转存
    AcDayDataMakeAndStoreProc(pTime);
    return 0;
}

/********************************************************************************
* 函数名:PulseMonProc
* 输　入:
* 输　出:
* 返  回:
* 功  能: 
*********************************************************************************/
INT8S PulseMonProc(void)
{
    INT8U       i   = 0;
    INT8U       j   = 0;
    INT8U       k   = 0;
    MEM_PULSE_PRM   sPulsePrm;
    
    for (i = 0; i < MAX_METER_NUM; i++)
    {
        memset(&sPulsePrm, 0, sizeof(sPulsePrm));
        if (GetPulsePrm(i + 1, &sPulsePrm) < 0)
        {
            continue;
        }
        j++;
        k = j - 1;
        
        //将当前各费率、各路脉冲个数转成月冻结示值进行存储
    }
    return 0;
}


/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 过月常规处理
*********************************************************************************/
INT8S NewMonDataProc(const pSTD_TIME pTime)
{
    //1.过月转结脉冲个数
#if PUL_CALC_FUNC_EN == 1
    PulseMonProc();
#endif
    return 0;
}



/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 实时抄表处理
*********************************************************************************/
INT8S RealDataRdProc(const pSTD_TIME pTime)
{
	INT16U				nReadIntv		= 0;
	INT16U				nMins			= 0;
	
	if (NULL == pTime)
    {
        return RET_ERR_ERROR;
    }

	nMins = BCD_TO_INT8U(pTime->nHour) * 60 + BCD_TO_INT8U(pTime->nMin);
	nReadIntv = 60;		//根据端口号获取到相应的抄表间隔,默认都是1个小时
	
	if (0 == nReadIntv)
    {
    	return RET_ERR_ERROR;
    }

	if (0 != nMins % nReadIntv)
    {
        return RET_ERR_ERROR;
    }

	SetRdMeterFlagsAllMp(RD_REAL);
    RdMeterSemProc();
    
	return RET_ERR_OK;
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 日抄表处理
*********************************************************************************/
INT8S DayDataRdProc(const pSTD_TIME pTime)
{
	if (NULL == pTime)
    {
        return -1;
    }

	//过日判断
    if (IsNewDay(pTime))
    {
		NewDayMemInfoProc(pTime);  		    //过日数据存储信息处理
        NewDayDataProc(pTime);  			//过日常规处理	

		//SetRdMeterFlagsAllMp(RD_DAY);
        //RdMeterSemProc();
    }
    return 0;
}


/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 月抄表处理
*********************************************************************************/
INT8S MonDataRdProc(const pSTD_TIME pTime)
{
	if (NULL == pTime)
    {
        return -1;
    }

	//过日判断
    if (IsNewMon(pTime))
    {
		NewMonMemInfoProc(pTime);  		//过月数据存储信息处理
    	NewMonDataProc(pTime);  			//过月常规处理	

		//SetRdMeterFlagsAllMp(RD_MON);
        //RdMeterSemProc();
    }
    return 0;
}


/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 曲线抄表处理
*********************************************************************************/
INT8S CrvDataRdProc(const pSTD_TIME pTime)
{
	if (NULL == pTime)
    {
        return -1;
    }

	return 0;
}



/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 
*********************************************************************************/
static void MinProc(const pSTD_TIME pTime)
{
	static INT8U nMinCnt = 0;
	
    
	DispMinProc();
    
	if (++nMinCnt >= 5)
	{
		//同步时钟
		SyncSysClk(NULL);
		nMinCnt = 0;
	}
	//实时抄表
	RealDataRdProc(pTime);
	//日
	DayDataRdProc(pTime);
	//月
	MonDataRdProc(pTime);
	//曲线抄表
	CrvDataRdProc(pTime);
	//抄表日
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 初始化处理
*********************************************************************************/
static void ResetProc(void)
{
    //MemInit(g_nInitType);
}

BOOLEAN IsNewMonCalcRDay()
{
    STD_TIME 		sTime;
    
    GetSysClk(&sTime);    

    if ((0x23 == sTime.nHour) && (0x59 == sTime.nMin) && (0x59 == sTime.nSec))
    {
        g_IsNewMonCalcRDayMon = sTime.nMon;
    }
    
    if ((!sTime.nSec) && (!sTime.nMin) && (!sTime.nHour))
    {
        if (g_IsNewMonCalcRDayMon != sTime.nMon)
        {
            g_IsNewMonCalcRDayFlg = 1;
            return TRUE;
        }
    }
    
    return FALSE;
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 秒
*********************************************************************************/
static void SecTaskMinProc(void)
{
    //初始化处理
    //ResetProc();
#if 1
	static STD_TIME	s_sLastMin;
	STD_TIME 		sTermTime;
	
    memset(&sTermTime, 0, sizeof(STD_TIME));
    GetSysClk(&sTermTime);
   
        //结算日
    IsNewMonCalcRDay();
    
    if (0 == memcmp(&sTermTime.nMin, &s_sLastMin.nMin, 5))
    {
    	return;	
    }
	memcpy(&s_sLastMin, &sTermTime, sizeof(STD_TIME));
#else
	static INT8U	s_nSecCnt = 0;

	s_nSecCnt++;
	if (s_nSecCnt < 60)
	{
		return;
	}
	s_nSecCnt = 0;
#endif
    
#if (1 == PUL_CALC_FUNC_EN)
    //脉冲计算
    PulseCalcProc();
#endif
  
#if (1 == AC_FUNC_EN)
     ACMinCalcProc();
#endif
        
	//分级处理
	MinProc(&sTermTime);
}


/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 秒级任务初始化
*********************************************************************************/
static BOOLEAN SecTaskInit(void)
{
    INT8U   nMemAddr[6] = {0};
    INT8U   nHardType   = 0;
    INT8U   i   = 0;
    INT8U   nComWH485   = 0;
    MEM_AC_PRM	sAc_Param;
    
    //memset(&sAc_Param, 0, sizeof(MEM_AC_PRM));
    OSSemAccept(g_pSem[SEM_ENET_FREE_IDX]);
    OSSemAccept(g_pSem[SEM_WAIT_FILE_IDX]);
    OSSemAccept(g_pSem[SEM_DISP_IDX]);
    OSMboxAccept(g_pMBox[MBOX_SELF_ADDR_IDX]);
    
#if (AC_FUNC_EN==1)
    //AcCalcInit();
#endif
#if PUL_CALC_FUNC_EN == 1
    PulseColInit();
#endif
    if (g_nAcCalPrmInitFlg)
    {
        g_nAcCalPrmInitFlg = 0;
        //AcPrmPrmInit();//正常运行时关闭
    }
    
    MemReadAcPrm(F44, &nHardType, sizeof(nHardType));
    
    if (MemReadAcPrm(F19, &nMemAddr[0], 6) < 0)
    {
        //AcPrmPrmInit();//在做出厂检测时放开,正常运行时需关闭
    }
	
	/*开机接线检查*/
	
    
	return TRUE;
}

INT32U CheckCom1Baud(void)
  {
    INT8U   i   = 0;
    INT8U   nComWH485   = 3;
    INT32U  Com1Baud =0;
    MEM_AC_PRM	sAc_Param;
    if(MemReadAcPrm(F27, &sAc_Param.nCom1Baud, sizeof(sAc_Param.nCom1Baud))<0)
    {
         i =  sAc_Param.nCom1Baud;
         if(i != 0x08)
         { 
             switch (i)
            {
              case 0x02:
                nComWH485 = 1;
                break;
              case 0x04:
                nComWH485 = 2;
                break;
              case 0x10:
                nComWH485 = 4;
                break;
              case 0x20:
                nComWH485 = 6;
                break;
              case 0x40:
                nComWH485 = 10;
                break;
              default:
                nComWH485 = 3;        //g_nUART_BPS_NUM = e_UART_BPS_2400;
                break;
            }
        //SetCommCfg(1,nComWH485, 2);        //2400 偶校验
        //OpenCommPort(COMM_WH485);
        }
    }
    
    Com1Baud=g_nUartBpsArray[nComWH485];
    return Com1Baud;
  }

INT8U       g_nBatterPower = 0;
/********************************************************************************
* 函数名:  BatterProc
* 输　入:
* 输　出:
* 返  回:
* 功  能: 电池处理
*********************************************************************************/
static void BatterProc(void)
{
    static INT8U    nTimeCnt    = 0;
    //static INT8U    ii    = 0;
    INT8U   nStatus = 0;
    
    nStatus = ReadCpuIoStatusEX(0x1E);
    
    if (nStatus == 0)//判断终端由电池供电
    {
        nTimeCnt++;
        g_nBatterPower = 1;
        /*if ((ii++) % 2)
        {
            LED_RUN_ON();
        }
        else
        {
            LED_RUN_OFF();
        }
        WriteComm(COMM_WH232, &nTimeCnt, 1);*/
    
        if (nTimeCnt > 30)  //一分钟后关闭电池供电
        {
            LED_ERR_ON();
            nTimeCnt = 0;
            SET_IO(0x21, 0);
            OSTimeDlyHMSM(0, 0, 0, 200);
        }
    }
    else
    {
        //做一次交采初始化
        if (g_nBatterPower)
        {
            g_nBatterPower = 0;
            ResetDelayHardInit(1000);
        }
        //LED_ERR_ON();
        nTimeCnt = 0;
    }
   
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 秒级任务处理
*********************************************************************************/
static void SecTaskProc(void)
{	
  
    //启动自检(终端）
    if (g_nAutoTestFlg)
    {
        //AutoTestProc();
    }
    //启动自检(电表）
    if (g_nAutoTestFlg1)
    {
       AutoTestProc1();

    }
    
	//秒级处理
	SecTaskSecProcPre();
	
    //分级处理
    SecTaskMinProc();

	//秒级处理//(时钟)
	SecTaskSecProcBack();
#if (AC_FUNC_EN == 1)
      //AcCalcProc();
#endif
    //电池处理
    //BatterProc();
    
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 秒级任务
*********************************************************************************/
void SecTask(void *pData)
{
	INT8U nErr = 0;
	
	pData = pData;

    if (!PowerOnProc())  //上电初始化
    {
    	return;
    }
	if (!SecTaskInit())
	{
		return;
	}

	for (; ;)
	{
		OSSemPend(g_pSem[SEM_SEC_IDX], 0, &nErr);
		SecTaskProc();
		SysSetTaskRunOnceTOS(TASK_SEC);
	}
}






















