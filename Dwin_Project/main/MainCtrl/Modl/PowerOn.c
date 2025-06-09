/******************************************************************************************************
* 2013-2014 ���Ƽ����޹�˾
* �ļ���: 
* ������: ��һ��
* �ա���: 2013/09/21
* �޸���: 
* �ա���: 
* �衡��: 
* �桡��: V1.0
*****************************************************************************************************/
#include "PowerOn.h"
#include "Base.h"
#include "SysRes.h"
#include "Guimenu.h"
#include "Fun.h"
#include "SoftTimer.h"
#include "TimerCount.h"
#include "DataItem.h"
#include "MemTbl.h"
#include "Main.h"
#include  "Lcd.h"
#include  "GuiwinCfg.h"
#include "MemPrm.h"
#include "MemData.h"
#include "SecTask.h"


INT8U       g_nAcCalPrmInitFlg = 0;
/**************************************************************************************************
                                           ��������
***************************************************************************************************/
extern INT8U g_nHardNo;
extern INT32S FlashFileBlkChk(INT8U nType);
extern INT8U  g_MeterInsideWrong[2];
INT8U g_nTriphase;
INT8U g_nSinglephase;


INT8U                DISPLY_SWITCH   	= 0;										//��ʾ�л���0������ 1������

INT32U  g_nAcPulseConst;
INT32U  g_nVolt;

//1.���� 2.��������220��3.��������(57.7) 4.��������(100V)��
INT32U  g_nHardType = SINGLEPHASE_METER;
INT8U   g_nMemAddr[6]		= {0};
INT8U   g_nMemProductAddr[6]		= {0};
INT8U   g_nMem698Addr[6]		= {0};
/**************************************************************************************************
                                           ��غ���ʵ��
***************************************************************************************************/
/********************************************************************************
* ������: GetHardNo
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ��ȡӲ���汾
*********************************************************************************/
static void GetHardNo(void)
{
    INT8U   nP1 = 0;
    INT8U   nP2 = 0;
    
    nP1 = ReadCpuIoStatusEX(0x3E);
    nP2 = ReadCpuIoStatusEX(0x3F);
    if (nP1 != 0)
    {
        nP1 = 1;    
    }
    if (nP2 != 0)
    {
        nP2 = 1;    
    }
    //g_nHardNo = ((nP2 << 1) | nP1); 
    g_nHardNo = 1;
}

/********************************************************************************
* ������: GetHardType
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ��ȡӲ���汾
*********************************************************************************/
static void GetHardType(void)
{
    //INT8U   nTriphase = 0;
   // INT8U   nSinglephase = 0;
    INT8U   nT1;
    INT8U   nT2;
    INT8U   nT3;
    MEM_AC_PRM	sAc_Param;
   
    nT1 = ReadCpuIoStatusEX(0x3E);  
    nT2 = ReadCpuIoStatusEX(0x3F);
    nT3 = ReadCpuIoStatusEX(0xF5);
    
    if ((0 == nT1) && (0 == nT2) && ((0 == nT3)||(1 == nT3)))
    {
        g_nHardType = SINGLEPHASE_METER; 
    }
    else if ((1 == nT1) && (0 == nT2) && ((0 == nT3)||(1 == nT3)))
    {
        g_nHardType = TRIPHASE_METER_220; 
    }
    else if ((0 == nT1) && (1 == nT2) && (0 == nT3))
    {
        g_nHardType = TRIPHASE_METER_57; 
    }
    else if ((1 == nT1) && (1 == nT2) && (0 == nT3))
    {
        g_nHardType = TRIPHASE_METER_100; 
    }
    
    
    memset(&sAc_Param, 0, sizeof(MEM_AC_PRM));
    if (MemReadAcPrm(F44, &sAc_Param.nHardType, sizeof(sAc_Param.nHardType)) >= 0)
    {
        if ((0xFF != sAc_Param.nHardType)&&(0 != sAc_Param.nHardType ))
        {
            g_nHardType = sAc_Param.nHardType;
        }
    }
    
    MemReadAcPrm(F19, &g_nMemAddr[0], 6);           //��ȡ���ɵ�ַ��ת���ڴ�
    MemReadAcPrm(F21, &g_nMemProductAddr[0], 6);    //��ȡ���ɵ�ַ��ת���ڴ�
    
   
   if((SINGLEPHASE_METER == g_nHardType)||(TRIPHASE_METER_220 == g_nHardType) || (TRIPHASE_METER_220ZT == g_nHardType))
   {
        g_nVolt = 2200;
   }
   else if((TRIPHASE_METER_57 == g_nHardType)|| (TRIPHASE_METER_57U == g_nHardType))
   {
        g_nVolt = 577;
   }
   else if((TRIPHASE_METER_100 == g_nHardType) || (TRIPHASE_METER_100U == g_nHardType))
   {
        g_nVolt = 1000;
   }
   
  
   if(SINGLEPHASE_METER == g_nHardType)
   {
        g_nAcPulseConst	= 1200;
   }
   else if(TRIPHASE_METER_220 == g_nHardType)
   {
        g_nAcPulseConst	= 6400;
   }
   else if (TRIPHASE_METER_57 == g_nHardType)
   {
        g_nAcPulseConst	= 6400;
   }
   else if(TRIPHASE_METER_100 == g_nHardType)
   {
        g_nAcPulseConst	= 6400;
   }
   else if(TRIPHASE_METER_220ZT == g_nHardType)
   {
        g_nAcPulseConst	= 400;
   }
   else if (TRIPHASE_METER_57U == g_nHardType)
   {
        g_nAcPulseConst	= 20000;
   }
   else if(TRIPHASE_METER_100U == g_nHardType)
   {
        g_nAcPulseConst	= 20000;
   }
}

/********************************************************************************
* ������: TermHardInit
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: �ն�Ӳ��������ʼ��
*********************************************************************************/
static BOOLEAN	TermHardInit(void)
{
	SysTick_Config( SystemCoreClock / OS_TICKS_PER_SEC);// init systick
    
    OSStatInit();                           				/* start stat task  */
    if (TargetInit())
    {
		return FALSE;
    }
    
    GetHardNo();
    GetHardType();
	
    AllCommInit();
	WatchDogClear(CH_DOG_OUT);
	if (AsynCommInit(0xFD))
	{
		return FALSE;
	}
	WatchDogClear(CH_DOG_OUT);
#ifdef GPRS_MOD_DEBUG
    OSTimeDlyHMSM(0, 0, 0, 500);
    WatchDogClear(CH_DOG_OUT);
    //V5_GPRS_ON();
    WatchDogClear(CH_DOG_OUT);
    OSTimeDlyHMSM(0, 0, 0, 500);
    WatchDogClear(CH_DOG_OUT);
#endif
	return TRUE;
}

/********************************************************************************
* ������: TermResCreate
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: �ն���Դ����
*********************************************************************************/
static BOOLEAN	TermResCreate(void)
{
	INT8U   i    = 0;
	INT8U   bsWd = 0;
	BOOLEAN bRet = FALSE;

	//��Ϣ������Դ����
	for (i = 0; i < QUEUE_NUM; i++)
	{
		if (NULL == (g_pQueue[i] = OSQCreate(&g_pQueueMsg[i][0], QUEUE_MSG_NUM)))
		{
			SET_BIT(bsWd, 0);
		}
	}
    WatchDogClear(CH_DOG_OUT);

	//��Ϣ������Դ����
	for (i = 0; i < MBOX_NUM; i++)
	{
		if (NULL == (g_pMBox[i] = OSMboxCreate(NULL)))
		{
			SET_BIT(bsWd, 1);
		}
	}
    WatchDogClear(CH_DOG_OUT);

	//�ź�����Դ����
	for (i = 0; i < SEM_NUM; i++)
	{
		if (NULL == (g_pSem[i] = OSSemCreate(1)))
		{
			SET_BIT(bsWd, 2);
		}
	}
	WatchDogClear(CH_DOG_OUT);

	//��������Դ����
	for (i = 0; i < TIMER_NUM; i++)
	{
		if (TimerInit(&g_pTimer[i], 1) < 0)
		{
			SET_BIT(bsWd, 3);
		}
	}
	WatchDogClear(CH_DOG_OUT);

	(0x00 != bsWd) ? (bRet = FALSE) : (bRet = TRUE);
	return bRet;
}

/********************************************************************************
* ������: TermSoftInit
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: �ն����������ʼ��
*********************************************************************************/
static BOOLEAN	TermSoftInit(void)
{	
    INT16U  i    	 	        = 0;
    INT8U   nFileInitFlg        = 0;
 
#ifdef SEC_APP
	INT8U	nSecFlg		= 0;
	//��boot��Ҫ���³����־(1Ϊ���³������)�Լ���ǰ16���ֽ�д����Ӧflashλ��	
	_API_FLASH_Read(&nSecFlg, 0, 1);
	if (nSecFlg != 4)
	{
		nSecFlg = 4;
		_API_FLASH_EraseWrite(&nSecFlg, 0, 1);
	}
#endif
    LoadParmFromFram0();
    MemReadFramXmpData(F1, 0, &nFileInitFlg, 1);
    //nFileInitFlg = 1; //��ʱ��Ӻ��ڱ���ɾ��
    if (nFileInitFlg != 1)
    {
        nFileInitFlg = 0;
    }
	//flash�ļ���ʼ��
	FlashFileBlkChk(nFileInitFlg);
    nFileInitFlg = 0;
    MemWriteFramXmpData(F1, 0, &nFileInitFlg, 1);

	//��ʱ���ϵ��ʼ��
	SyncSysClk(NULL);

	//�ϵ��ȡ�洢�����еĲ���
	LoadParmFromFram();
    
	//�γɵ����ַ�
	AreaStringMake();

    if (MemReadAcPrm(F51, &DISPLY_SWITCH, 1) < 0)
    {
        DISPLY_SWITCH = 0;
    }
    if((DISPLY_SWITCH > 1)||(SINGLEPHASE_METER == g_nHardType ))
    {
        DISPLY_SWITCH = 0;
    }
	//���������ӳ��,�ó���ʵʱ���ݱ�־
	MeterMpMap(1);

    for (i = 0; i < (sizeof(g_sTaskCreateTbl) / sizeof(TASK_CREATE)); i++)
	{
		if (RET_ERR_OK != SysSetTaskInit(i))
		{
			return FALSE;
		}
		WatchDogClear(CH_DOG_OUT);
	}
      
    //��ȡϵͳʱ�丳ֵ��ȫ��ʱ��
    //��ȡϵͳʱ�丳ֵ��ȫ��ʱ��
    memset(g_pSysTime, 0, sizeof(STD_TIME));
    MemReadAcXBackData(F30, g_pSysTime, sizeof(STD_TIME));
    if (!IsVldStdTime(g_pSysTime))
    {
        memset(g_pSysTime, 0, sizeof(STD_TIME));
        GetSysClk(g_pSysTime);
    }

	return TRUE;
}

/********************************************************************************
* ������: TermTaskCreate
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: �ն˽��̴���
*********************************************************************************/
static BOOLEAN	TermTaskCreate(void)
{
    INT8U	i 		 = 0;

	for (i = 0; i < (sizeof(g_sTaskCreateTbl) / sizeof(TASK_CREATE)); i++)
	{
		if (TASK_SEC == g_sTaskCreateTbl[i].nId)
		{
			continue;												//�Ѿ�������
		}
		if (RET_ERR_OK != SysTaskCreate(g_sTaskCreateTbl[i].nId))	
		{
			return FALSE;
		}
	}
	
	return TRUE;
}



/********************************************************************************
* ������: DebugHard
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: Ӳ������
*********************************************************************************/
static void DebugHard(void)
{
#if 0
	OSTimeDlyHMSM(0,0,0,500);
    WatchDogClear(CH_DOG_OUT);
    OSTimeDlyHMSM(0,0,0,500);
    WatchDogClear(CH_DOG_OUT);
    OSTimeDlyHMSM(0,0,0,500);
    WatchDogClear(CH_DOG_OUT);
    OSTimeDlyHMSM(0,0,0,500);
    WatchDogClear(CH_DOG_OUT);
	TimeDisp();

	while(1)
	{
		GpioSetOutputEX(SPI_WLAN_SO);
		delay_us(2);
		GpioClearOutputEX(SPI_WLAN_SO);
		delay_us(2);
	}
	//GpioSetOutputEX(WLAN_SDN);
	
	//GpioClearOutputEX(WLAN_SDN);
#endif
#if 0
    static INT8U   nBuf[10] = {0};
    static INT16U  i = 0;
    INT8U			nCom = 0;
    
    while(1)
    {
        for (i = 0; i < sizeof(nBuf); i++)
        {
            nBuf[i] = i + 1;
        }
    	nCom = SetCommCfg(1, 10, 2);//SetCommCfg(1, 12, 2);
		ProcCommCfgPend();
        WriteComm(nCom, &nBuf[0], sizeof(nBuf));
        
        memset(&nBuf[0], 0, sizeof(nBuf));
		if (ReadCommMs(nCom, &nBuf[0], sizeof(nBuf), 200, 1000) > 0)
		{
			WatchDogClear(CH_DOG_OUT);
		}
		ProcCommCfgPost();
		WatchDogClear(CH_DOG_OUT);
    }
    
    WatchDogClear(CH_DOG_OUT);
    
#endif



}
/********************************************************************************
* ������: DebugSoft
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: �������
*********************************************************************************/
static void DebugSoft(void)
{
#if 0 
    STD_TIME sTime;
    memset(&sTime, 0, sizeof(STD_TIME));
    MemReadAcXBackData(F30, &sTime, sizeof(STD_TIME));
    
    GetSysClk(&sTime);
    
    MemWriteAcXBackData(F30, &sTime, sizeof(STD_TIME));
    
    memset(&sTime, 0, sizeof(STD_TIME));
    MemReadAcXBackData(F30, &sTime, sizeof(STD_TIME));
    
    memset(&sTime, 0, sizeof(STD_TIME));
    MemReadAcXBackData(F30, &sTime, sizeof(STD_TIME));
    
#endif
#if 0
    INT32U  nSize = 0;
    
    nSize += sizeof(MEM_AC_PRM);
    nSize +=  sizeof(MT_DGN_ENERGY_DATA);
    nSize +=  sizeof(MT_DGN_Q_ENERGY_DATA);
    nSize +=  sizeof(MT_DGN_TRI_ENERGY_DATA);
    nSize +=  sizeof(MT_DGN_TRI_Q_EN_DATA);
    nSize +=  sizeof(MT_DGN_C_ENERGY_DATA);
    nSize +=  sizeof(MT_DGN_DEMAND_DATA);
    nSize +=  sizeof(MT_DGN_Q_DE_DATA);
    nSize +=  sizeof(MEM_AC_DAY_DATA);
    nSize +=  sizeof(MEM_AC_RDAY_DATA);
    nSize +=  sizeof(MEM_AC_DJ_DATA);
    nSize +=  sizeof(MEM_AC_DJ_DATA);
    
#endif
#if 0
    INT8U nEveData[300] = {0};
    MEM_AC_DAY_DATA sAcDayData;
    INT8U   ii = 0;
    INT8U   nBuf[10] = {0};
    
    nBuf[0] = 2;
    nBuf[1] = 4;
    
    memset(&nBuf[0], 0, sizeof(nBuf));
    MemReadGrpPrm(F12, 0, &nBuf[0], 2);
    nBuf[0] = 2;
    nBuf[1] = 4;
    MemWriteGrpPrm(F12, 0, &nBuf[0], 2);
    memset(&nBuf[0], 0, sizeof(nBuf));
    MemReadGrpPrm(F12, 0, &nBuf[0], 2);
    
    //memset(&nEveData[0], 0, LEN_AC_EVENT);
    //FlashRead(618496, &nEveData[0], LEN_AC_EVENT);
    //memset(&nEveData[0], 0x11, LEN_AC_EVENT);
    //FlashWrite(618496, &nEveData[0], LEN_AC_EVENT);
    //memset(&nEveData[0], 0, LEN_AC_EVENT);
    //FlashRead(618496, &nEveData[0], LEN_AC_EVENT);
    
    memset(&nEveData[0], 0, LEN_AC_EVENT);
    MemReadAcEvtProc(F1, 1, &nEveData, LEN_AC_EVENT);
    memset(&nEveData[0], 0x22, LEN_AC_EVENT);
    MemWriteAcEvtProc(F1, &nEveData, LEN_AC_EVENT, 1);
    memset(&nEveData[0], 0, LEN_AC_EVENT);
    MemReadAcEvtProc(F1, 1, &nEveData, LEN_AC_EVENT);
    
    memset(&sAcDayData, 0, sizeof(MEM_AC_DAY_DATA));
    MemReadAcHistDataProc(F1, 1, &sAcDayData,sizeof(MEM_AC_DAY_DATA));
    memset(&sAcDayData, 0x33, sizeof(MEM_AC_DAY_DATA));
    MemWriteAcHistDataProc(F1, &sAcDayData, sizeof(MEM_AC_DAY_DATA));
    memset(&sAcDayData, 0, sizeof(MEM_AC_DAY_DATA));
    MemReadAcHistDataProc(F1, 1, &sAcDayData,sizeof(MEM_AC_DAY_DATA));
    
    ii++;
    
#endif
#if 0
    AcPrmPrmInit();
#endif
#if 0
    INT8U nSyNUM = 0;
    INT8U nEveData[LEN_AC_EVENT];
    INT8U   i           = 0;
    
    nSyNUM = 0x55;
    MemWriteAcXFlhBackData(F25, &nSyNUM, sizeof(nSyNUM));
    nSyNUM = 0;
    MemReadAcXFlhBackData(F25, &nSyNUM, sizeof(nSyNUM));
    
    memset(&nEveData[0], 0, sizeof(nEveData));
    nEveData[0] = 0x11;
    nEveData[1] = 0x22;
    MemWriteAcEvtProc(F1, &nEveData[0], LEN_AC_EVENT, 1);
    memset(&nEveData[0], 0, sizeof(nEveData));
    MemReadAcEvtProc(F1, 1, &nEveData[0], LEN_AC_EVENT);
    
    
    i = 6;
#endif
#if 0
    INT8U nFileInitFlg  = 1;
    MemWriteFramXmpData(F1, 0, &nFileInitFlg, 1);
    
    nFileInitFlg = 6;
    MemReadFramXmpData(F1, 0, &nFileInitFlg, 1);
    
    nFileInitFlg = 1;
    
    _API_FRAM_Write(STR_OFF(pMEM_FRAM_STORE, sXFramData.nFileInitFlg), &nFileInitFlg, 1);
    
    nFileInitFlg = 6;
    
    _API_FRAM_Read(STR_OFF(pMEM_FRAM_STORE, sXFramData.nFileInitFlg), &nFileInitFlg, 1);
    
    nFileInitFlg = 2;
#endif
#if 0
    INT32U          nTmp            = 0;
    
    while(1)
    {
        nTmp = GetAD0Voltage();
        OSTimeDlyHMSM(0, 0, 0, 500);
    }
    
#endif
  
  
#if 0
    MEM_AC_CAL_PRM sJB_Param;
    INT32S          nLen    = 0;
    
    memset(&sJB_Param, 0, sizeof(MEM_AC_CAL_PRM));
    
    //sJB_Param.nModeCfg = 0xAA;
    //sJB_Param.nPGActrl = 0xBB;
    //sJB_Param.nUgain[0] = 0x11;
    //sJB_Param.nUgain[1] = 0x22;
    //sJB_Param.nUgain[2] = 0x33;
    
    //nLen = MemWriteAcCalPrm(FEND, &sJB_Param, sizeof(sJB_Param));
    
    //memset(&sJB_Param, 0, sizeof(MEM_AC_CAL_PRM));                 
    nLen = MemReadAcCalPrm(FEND, &sJB_Param, sizeof(sJB_Param));
                    
    memset(&sJB_Param, 0, sizeof(MEM_AC_CAL_PRM));
    
    _API_FRAM_Read(STR_OFF(pMEM_FRAM_STORE, sAcCalPrm), (INT8U*)&sJB_Param, sizeof(sJB_Param));
    
    nLen++;
    
                     
#endif
#if 0
	
	INT8U 	nData[30] 		= {0};
	INT16U 	i 				= 0;
    
	//char**	ppFlashFileTbl	= NULL;

	//ppFlashFileTbl = FlashFileTblGet();
    
    //FlashFileOpen("prm");
    
    //FlashFileOpen("data");

	for (i = 0; i < sizeof(nData); i++)
	{
		nData[i] = i;
	}
	FlashFileWrite("prm", 0, nData, sizeof(nData));
	WatchDogClear(CH_DOG_OUT);
	memset(nData, 0, sizeof(nData));
	FlashFileRead("prm", 0, nData, sizeof(nData));
	WatchDogClear(CH_DOG_OUT);

	for (i = 0; i < sizeof(nData); i++)
	{
		nData[i] = i + 1;
	}
	FlashFileWrite("data", 0, nData, sizeof(nData));
	WatchDogClear(CH_DOG_OUT);
	
	memset(nData, 0, sizeof(nData));
	FlashFileRead("data", 0, nData, sizeof(nData));
	WatchDogClear(CH_DOG_OUT);
	
	memset(nData, 0, sizeof(nData));
	FlashFileRead("prm", 0, nData, sizeof(nData));
	WatchDogClear(CH_DOG_OUT);
	
	memset(nData, 0, sizeof(nData));
	FlashFileRead("data", 0, nData, sizeof(nData));
	WatchDogClear(CH_DOG_OUT);


	for (i = 0; i < sizeof(nData); i++)
	{
		nData[i] = i + 2;
	}
	FlashFileWrite("prm", 4096 - 10, nData, sizeof(nData));
	WatchDogClear(CH_DOG_OUT);
	memset(nData, 0, sizeof(nData));
	FlashFileRead("prm", 4096 - 10, nData, sizeof(nData));
	WatchDogClear(CH_DOG_OUT);

	memset(nData, 0, sizeof(nData));
	FlashFileRead("data", 0, nData, sizeof(nData));
	WatchDogClear(CH_DOG_OUT);

	FlashFileClose("prm");
    memset(nData, 0, sizeof(nData));
	FlashFileRead("prm", 4096 - 10, nData, sizeof(nData));
    WatchDogClear(CH_DOG_OUT);
    
    FlashFileOpen("prm");
	memset(nData, 0, sizeof(nData));
	FlashFileRead("prm", 45, nData, sizeof(nData));
    WatchDogClear(CH_DOG_OUT);
    
    for (i = 0; i < sizeof(nData); i++)
	{
		nData[i] = i + 3;
	}
	FlashFileWrite("prm", 45, nData, sizeof(nData));
	WatchDogClear(CH_DOG_OUT);
	memset(nData, 0, sizeof(nData));
	FlashFileRead("prm", 45, nData, sizeof(nData));
	WatchDogClear(CH_DOG_OUT);
    
    memset(nData, 0, sizeof(nData));
	FlashFileRead("prm", 4096, nData, sizeof(nData));
    WatchDogClear(CH_DOG_OUT);
    for (i = 0; i < sizeof(nData); i++)
	{
		nData[i] = i + 4;
	}
	FlashFileWrite("prm", 4096, nData, sizeof(nData) - 5);
	WatchDogClear(CH_DOG_OUT);
	memset(nData, 0, sizeof(nData));
	FlashFileRead("prm", 4096, nData, sizeof(nData));
	WatchDogClear(CH_DOG_OUT);

	FlashFileClose("prm");

	FlashFileClose("data");
	
#endif

    
//��Ӳ������
#if 0
    INT16U  a[6] = {1,2,3,4,5,6};
    INT8U   b = 0;
    
    INT16U * p = (INT16U*)&a;
    
    b = *p++;
    b = *p++;
    b = *p++;
    b = *p++;
    b = *p++;
    b = *p++;
#endif
    
//1.�洢��FLASH������
#if 0
    INT8U       nBuf[10] = {0};
    INT8U       i   = 0;
    
    for(i = 0; i < 10; i++)
    {
        nBuf[i] = i;
    }
    
    /*_API_FLASH_EraseWrite(&nBuf[0], 4096 * 7 - 5, sizeof(nBuf));
    _API_FLASH_EraseWrite(&nBuf[0], 4096 * 7 - 15, sizeof(nBuf));

	memset(&nBuf[0], 0, sizeof(nBuf));
    _API_FLASH_Read(&nBuf[0], 4096 * 7 - 5, sizeof(nBuf));
    
    memset(&nBuf[0], 0, sizeof(nBuf));
    _API_FLASH_Read(&nBuf[0], 4096 * 7 - 15, sizeof(nBuf));*/

    /*FlashRead(4096 * 9 - 5, &nBuf[0], sizeof(nBuf));
	
    SST25SPIFlashErase( SST25_ERASE_TYPE_TWOSECTOR, 4096 * 9 - 5 );
    SST25SPIFlashErase( SST25_ERASE_TYPE_CHIP, 0 );
 
    FlashRead(4096 * 9 - 5, &nBuf[0], sizeof(nBuf));
    
    for(i = 0; i < 10; i++)
    {
        nBuf[i] = i;
    }
    
    FlashWrite(4096 * 9 - 5, &nBuf[0], sizeof(nBuf));
    //FlashWrite(4096 * 9 - 15, &nBuf[0], sizeof(nBuf));
    
    memset(&nBuf[0], 0, sizeof(nBuf));
    FlashRead(4096 * 9 - 5, &nBuf[0], sizeof(nBuf));
    
    //memset(&nBuf[0], 0, sizeof(nBuf));
    FlashRead(4096 * 9 - 5, &nBuf[0], sizeof(nBuf));
    
    memset(&nBuf[0], 0, sizeof(nBuf));*/
    
    _API_FLASH_EraseWrite(&nBuf[0], 4096 - 5, sizeof(nBuf)); 
	_API_FRAM_Write(50, &nBuf[0], sizeof(nBuf));
    
    _API_FLASH_EraseWrite(&nBuf[0], 50, sizeof(nBuf)); 
	_API_FRAM_Write(50, &nBuf[0], sizeof(nBuf));
    
    memset(&nBuf[0], 0, sizeof(nBuf));
    _API_FLASH_Read(&nBuf[0], 4096 - 5, sizeof(nBuf));
    
    memset(&nBuf[0], 0, sizeof(nBuf));
    _API_FLASH_Read(&nBuf[0], 50, sizeof(nBuf));

	memset(&nBuf[0], 0, sizeof(nBuf));
	_API_FRAM_Read(50, &nBuf[0], sizeof(nBuf));

	_API_FLASH_PowerOffOn();

	for(i = 0; i < 10; i++)
    {
        nBuf[i] = i;
    }
	_API_FLASH_EraseWrite(&nBuf[0], 0x400000 - 5, sizeof(nBuf));   
	_API_FRAM_Write(50, &nBuf[0], sizeof(nBuf));
    
    memset(&nBuf[0], 0, sizeof(nBuf));
    _API_FLASH_Read(&nBuf[0], 0x400000 - 5, sizeof(nBuf));

	memset(&nBuf[0], 0, sizeof(nBuf));
	_API_FRAM_Read(50, &nBuf[0], sizeof(nBuf));
	
#endif
 
//2.ʱ��
#if 0
    STD_TIME        sTime;
    
    memset(&sTime, 0, sizeof(sTime));
    
    //g_nHardNo = 0;
    
    sTime.nYear = 0x15;
    sTime.nMon = 0x11;
    sTime.nDay = 0x24;
    sTime.nHour = 0x10;
    sTime.nMin = 0x30;
    sTime.nSec = 0x30;
    
    
    RTCDateTimeWrite(&sTime);
    
    memset(&sTime, 0, sizeof(sTime));
    
    RTCDateTimeRead(&sTime);
    memset(&sTime, 0, sizeof(sTime));
    
#endif
  
//3.���еơ��澯��    
#if 0
    
    GpioConfig(IO_PORT3, IO_PINxC, GPIO_MODE_OUTPUT);
    
    LED_RUN_OFF();
    
    LED_RUN_ON();
    
    LED_ERR_OFF();
    
    LED_ERR_ON();
#endif
       
//4.485��   
#if 0
    INT8U	l_buff[30] = {0};
	INT8U	l_ret = 0;
    INT8U   i       = 0;
    V2_TX_ON();
    
    
    for (i = 0; i < 10; i++)
    {
        l_buff[i] = i + 1;         
    }
    while(1)
    {
        l_ret = ReadCommMs(COMM_WH485, l_buff, 100, 500, 500);
        if (l_ret > 0)
        {
            OSTimeDlyHMSM( 0, 0, 0, 500 );
            l_ret = WriteComm(COMM_WH485, l_buff, 10); 
            
            //memset(&l_buff[0], 0, sizeof(l_buff));
        //l_ret = ReadCommMs(COMM_WH485, l_buff, 100, 500, 500);
            if(l_ret > 0)
            {
                l_ret++;   
            }     
        }
        
    }
#endif
       
//4.1 485�� 2  
#if 0
    INT8U	l_buff[30] = {0};
	INT8U	l_ret = 0;
    INT8U   i       = 0;
    V2_TX_ON();
    
    
    for (i = 0; i < 10; i++)
    {
        l_buff[i] = i + 1;         
    }
    while(1)
    {
        //l_ret = ReadCommMs(COMM_WH4852, l_buff, 100, 500, 500);
        //if (l_ret > 0)
        {
            OSTimeDlyHMSM( 0, 0, 0, 500 );
            l_ret = WriteComm(COMM_WH4852, l_buff, 10); 
            
            //memset(&l_buff[0], 0, sizeof(l_buff));
            //l_ret = ReadCommMs(COMM_WH4852, l_buff, 100, 500, 500);
            if(l_ret > 0)
            {
                l_ret++;   
            }     
        }
        
    }
#endif

//5.232��
#if 0
    INT8U	l_buff[30] = {0};
	INT8U	l_ret = 0;
    INT8U   i       = 0;
    V2_TX_ON();
    
    
    for (i = 0; i < 30; i++)
    {
        l_buff[i] = i + 1;         
    }
    while(1)
    {
        l_ret = ReadCommMs(COMM_WH232, l_buff, 100, 500, 500);
        if (l_ret > 0)
        {
            OSTimeDlyHMSM( 0, 0, 0, 500 );
            l_ret = WriteComm(COMM_WH232, l_buff, 30); 
            if(l_ret > 0)
            {
                l_ret++;   
            }     
        }
        
    }
#endif
    
//6.�����Է�����
#if 0
    INT8U	l_buff[300] = {0};
	INT8U	l_ret = 0;
    

	V1_IRTXM_ON();
	while(1)
    {
        l_ret = ReadCommMs(COMM_IR, l_buff, 200, 100, 5000);
        if(l_ret > 0)
        {
            l_ret = ReadCommMs(COMM_IR, l_buff, 100, 500, 5000);
            WriteComm(COMM_IR, l_buff, l_ret);
            OSTimeDlyHMSM( 0, 0, 0, 500 );
            memset(&l_buff[0], 0, sizeof(l_buff));
            l_ret = ReadCommMs(COMM_IR, l_buff, 100, 500, 500);
            
            OSTimeDlyHMSM( 0, 0, 0, 500 );
            WriteComm(COMM_WH232, l_buff, l_ret);
        }
    }
#endif
    
//7.GPRS����
#if 0
    INT8U   nCom  = COMM_GPRS;
    INT8U   nLen  = 0;
    INT8U   nBuf[100] = {0};
    INT32S  ret = -1;
    //V5_GPRS_ON();
    //CloseComm( nCom );
    //ret = OpenComm( nCom,
			        //UART_BPS_115200,
			        //EVEN_PARITY,
			        //ONE_STOP_BIT,
			        //EIGHT_CHAR_BIT );
    //if( ret==COMM_SUCC )
    {
       // CommFlush( nCom );
	 	//return ret;    
    }
    //CloseComm( port );
    
    while(1)
    {
        //memset(&nBuf[0], 3, 100);
        //nLen = WriteComm(nCom, &nBuf[0], 10);
        //WriteComm( COMM_GPRS, "AT\x0d", 4);
        WriteComm( COMM_GPRS, "AT\r", 3);
		OSTimeDlyHMSM(0, 0, 0, 300);
        
        memset(nBuf,0,sizeof(nBuf));
		nLen = ReadCommMs(COMM_GPRS, nBuf, 100, 500, 500);
        
        //OSTimeDlyHMSM(0,0,0,500);
        //nLen = ReadCommMs(nCom, &nBuf[0], 100, 500, 2000);
        WriteComm(COMM6, &nBuf[0], nLen);
        /*if(nLen > 0)
        {
            nLen++;    
        }*/
    }
#endif
    

    
//8.�ز����ԣ���ȡ�汾�� �ز��Ĳ�������9600����У�飬   
#if 0
    GpioConfigEX(0x38, GPIO_MODE_OUTPUT, IO_PULLUP_ON );
    GpioConfigEX(0x39, GPIO_MODE_INPUT, IO_PULLUP_ON );
    OSTimeDlyHMSM(0, 0, 3, 0);
    
    SET_IO(0x38, 1);
    OSTimeDlyHMSM(0, 0, 2, 0);
    CarrReadVer();
#endif
    
//9.����
#if 0
    INT8U       nIo[5]     = 0;
    INT8U       i           = 0;
    
    GpioConfigEX(0x40, GPIO_MODE_INPUT, IO_PULLUP_ON );
    GpioConfigEX(0x41, GPIO_MODE_INPUT, IO_PULLUP_ON );
    GpioConfigEX(0x42, GPIO_MODE_INPUT, IO_PULLUP_ON );
    GpioConfigEX(0x43, GPIO_MODE_INPUT, IO_PULLUP_ON );
    GpioConfigEX(0x44, GPIO_MODE_INPUT, IO_PULLUP_ON );
    
    OSTimeDlyHMSM(0, 0, 3, 0);
    for (i = 0; i < 5; i++)
    {
        nIo[i] =  ReadCpuIoStatusEX(0x40 + i);  
    }
    i = 0;
#endif
    
//10.���ϰ�̵�������
#if 0
    GpioConfigEX(0x24, GPIO_MODE_OUTPUT, IO_PULLUP_ON );
    GpioConfigEX(0x25, GPIO_MODE_OUTPUT, IO_PULLUP_ON );
    GpioConfigEX(0x26, GPIO_MODE_OUTPUT, IO_PULLUP_ON );
    GpioConfigEX(0x27, GPIO_MODE_OUTPUT, IO_PULLUP_ON );
    GpioConfigEX(0x28, GPIO_MODE_OUTPUT, IO_PULLUP_ON );
    GpioConfigEX(0x29, GPIO_MODE_OUTPUT, IO_PULLUP_ON );
    GpioConfigEX(0x36, GPIO_MODE_OUTPUT, IO_PULLUP_ON );
    GpioConfigEX(0x37, GPIO_MODE_OUTPUT, IO_PULLUP_ON );
    OSTimeDlyHMSM(0, 0, 2, 0);
    
    SET_IO(0x24, 0);
    SET_IO(0x24, 1);
    SET_IO(0x24, 0);
    
    SET_IO(0x25, 0);
    SET_IO(0x25, 1);
    SET_IO(0x25, 0);
    
    SET_IO(0x26, 0);
    SET_IO(0x26, 1);
    SET_IO(0x26, 0);
    
    SET_IO(0x27, 0);
    SET_IO(0x27, 1);
    SET_IO(0x27, 0);
    
    SET_IO(0x28, 0);
    SET_IO(0x28, 1);
    SET_IO(0x28, 0);
    
    SET_IO(0x29, 0);
    SET_IO(0x29, 1);
    SET_IO(0x29, 0);
    
    SET_IO(0x36, 0);
    SET_IO(0x36, 1);
    SET_IO(0x36, 0);
    
    SET_IO(0x37, 0);
    SET_IO(0x37, 1);
    SET_IO(0x37, 0);
#endif
    
//11.���ϰ崮�ڲ���
#if 0
    INT8U	l_buff[30] = {0};
    INT8U	l_Tmpbuff[30] = {0};
	INT8U	l_ret = 0;
    INT8U   i       = 0;
    V2_TX_ON();
    
    
    for (i = 0; i < 30; i++)
    {
        l_buff[i] = i + 1;         
    }
    while(1)
    {
        l_ret = ReadCommMs(COMM_ERR, l_Tmpbuff, 100, 500, 500);
        //memset(&l_buff[0], 0, sizeof(l_buff));
        if (l_ret > 0)
        {
            OSTimeDlyHMSM( 0, 0, 1, 0 );
            l_ret = WriteComm(COMM_ERR, l_buff, 30); 
            if(l_ret > 0)
            {
                l_ret++;   
            }     
        }
    
    }
#endif
#if 0
    INT8U	l_buff[30]   = {0x68, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x68, 0x15, 0x02, 0x32, 0x32, 0x47, 0x16};
    INT8U   l_Tmpbuff[30] = {0};
    INT32S  l_ret = 0;
    memset(&l_Tmpbuff[0], 0, sizeof(l_Tmpbuff));
    
    V1_IRTXM_ON();
    
    
    //WriteComm(COMM_ZB, pData, nLen);
    WriteComm(COMM_ZB, l_buff, 14);
    l_ret = ReadCommMs(COMM_ZB, l_Tmpbuff, 100, 500, 500);
    if (l_ret > 0)
    {
        //return l_ret;
    }
    //return -1;
#endif
    
#if 0
    INT8U	l_buff[30] = {0};
	INT8U	l_ret = 0;
    INT8U   i       = 0;
    V2_TX_ON();
    
    
    for (i = 0; i < 10; i++)
    {
        l_buff[i] = i + 1;         
    }
    while(1)
    {
        l_ret = ReadCommMs(COMM0, l_buff, 100, 500, 500);
        if (l_ret > 0)
        {
            OSTimeDlyHMSM( 0, 0, 0, 500 );
            l_ret = WriteComm(COMM0, l_buff, 10); 
            if(l_ret > 0)
            {
                l_ret++;   
            }     
        }
        
    }
#endif
//12.���
#if 0
   GpioConfigEX(0x21, GPIO_MODE_OUTPUT, IO_PULLUP_ON ); 
   OSTimeDlyHMSM(0, 0, 2, 0);
   
   SET_IO(0x21, 1);
#endif
    
#if 0
   INT8U    nBuf[30] ={0};
   
   nBuf[0] = 0x06;
   nBuf[1] = 0x33;
   nBuf[2] = 0x02;
   nBuf[3] = 0x01;
   FlashFileOpen(COMM_PRM_FILE);
   MemWriteCommPrm(F94, &nBuf[0], 4);
   //FlashFileWrite(COMM_PRM_FILE, 0, nBuf, 4);
   
   memset(&nBuf[0], 0, sizeof(nBuf));
   FlashFileRead(COMM_PRM_FILE, 0, nBuf, 4);
   memset(&nBuf[0], 0, sizeof(nBuf));
   MemReadCommPrm(F94, &nBuf[0], 5);
   
   nBuf[0] = 0x05;
   nBuf[1] = 0x32;
   nBuf[2] = 0x01;
   nBuf[3] = 0x00;
   MemWriteCommPrm(F94, &nBuf[0], 4);
   //FlashFileWrite(COMM_PRM_FILE, 0, nBuf, 4);
   
   memset(&nBuf[0], 0, sizeof(nBuf));
   FlashFileRead(COMM_PRM_FILE, 0, nBuf, 4);
   memset(&nBuf[0], 0, sizeof(nBuf));
   MemReadCommPrm(F94, &nBuf[0], 5);
   
   OSTimeDlyHMSM(0, 0, 2, 0);
#endif
   
#if 0
   INT8U    *p = "ABC����";
   
   while(1)
   {
        OSTimeDlyHMSM(0, 0, 0, 100);
  	//g_nBackColour = COLOUR_SET(0, 0, 31);
	//g_nDrawColour = COLOUR_SET(31, 63, 31);
    //g_nBackColour = 0x0F;
	
	LCD_INIT();		   			//tft��ʼ��
    ClearScreenUC1698U(0);
    
    GuiMenuCreatSub((void*)g_sJcTestSimpleMenu);
   
#if 0 
    ClearScreenUC1698U();
    
    GuiwinTextOut(8, 8, 1, p);
    
    GuiwinTextOut(64, 64, 0, p);
    
    LcdRecClear(8, 8, 64, 64, 1);
    LcdRecClear(8, 8, 64, 64, 0);
    
    ClearScreenUC1698U();
    
    LCD_DrawRectangle(8, 8, 64, 64, 1);
    
    LCD_DrawRectangle(8, 8, 64, 64, 0);
    
    ClearScreenUC1698U();
	
	ClearScreenUC1698U();
    
    LCD_ShowHzString(0, 0, p, 0, 0xFF, 1);
    LCD_ShowHzString(0, 128, p, 0, 0xFF, 0);
    
    ClearScreenUC1698U();
    
    
    
    ClearScreenUC1698U();
    GuiwinDynMboxCreate(0, "�����γ���װ����");
    
    ClearScreenUC1698U();
    Guiwin1MboxCreate("ȷ��", "��͸�ɹ�,��˲��ն�ʱ���Ƿ�����");
    
    ClearScreenUC1698U();
    Guiwin2MboxCreate("��","��","�Ƿ�ȷ��ѡ��?");
    
    
    //LcdRecClear(8, 8, 64, 64, 1);
    
    
    /*LCD_WR_DATA2(6, 0, 1);
    LCD_WR_DATA2(6, 64, 1);
    LCD_WR_DATA2(7, 0, 1);
    LCD_WR_DATA2(7, 64, 1);
    LCD_WR_DATA2(8, 0, 1);
    LCD_WR_DATA2(8, 64, 1);
    LCD_WR_DATA2(9, 0, 1);
    LCD_WR_DATA2(9, 64, 1);
    LCD_WR_DATA2(6, 0, 1);
    LCD_WR_DATA2(6, 64, 1);
    LCD_WR_DATA2(7, 0, 1);
    LCD_WR_DATA2(7, 64, 1);
    
    LCD_WR_DATA2(6, 0, 0);
    LCD_WR_DATA2(6, 64, 0);
    LCD_WR_DATA2(7, 0, 0);
    LCD_WR_DATA2(7, 64, 0);
    LCD_WR_DATA2(159, 0, 0);
    LCD_WR_DATA2(159, 64, 0);*/
    //LCD_DrawPoint(0, 0, g_nBackColour);
    
    //ClearScreenUC1698U();
    //LCD_DrawPoint(0, 0, g_nBackColour);
    
    
    //LCD_DrawLine(8, 8, 64, 64, 1);
    //LCD_DrawXieLine(100, 100, 145, 145, 1);
    //LCD_DrawRectangle(8, 8, 64, 64, g_nBackColour);
    //LCD_DrawPoint(64, 64, g_nBackColour);
    
    //ClearScreenUC1698U();
    //LcdRecClear(8, 8, 64, 64, g_nBackColour);
    //LcdClear(g_nBackColour);
    
    
    //LCD_ShowHzString(0, 0, p, 0, 0xFF, 1);
    
    //LCD_ShowHzString(0, 128, p, 0, 0xFF, 0);
    
    //LCD_ShowHzString(128, 0, p, 0, 0xFF, 1);
    
    //LCD_ShowHzString(128, 128, p, 0, 0xFF, 0);
   
            
    OSTimeDlyHMSM(0, 0, 3, 100);
#endif
   
   }
   
#endif
   
#if 0
    MemPartPrmInit();
#endif
#if 0
    INT8U   nSize = 0;
    
    nSize = sizeof(long);
    nSize = sizeof(long long);
    nSize = 0;
#endif
    
#if 0
    INT32U   nTime[10] = {0};
    
    nTime[0] = OSTimeGet();
    OSTimeDlyHMSM(0, 0, 2, 0);
    nTime[1] = OSTimeGet();
    nTime[2] = OSTimeGet();
    nTime[3] = 0;
#endif
#if 0
    CTRL_TURN_PRM   sCtrlPrm;
    STD_TIME        sTime;
    INT8U           i   = 0;
    
    memset(&sCtrlPrm, 0, sizeof(sCtrlPrm));
    memset(&sTime, 0, sizeof(sTime));
    
    GetSysClk(&sTime);
    memcpy(&sCtrlPrm.sCtrlCmdSetTime, &sTime.nSec, 6);
    sCtrlPrm.nAlmMin = 0x02;
    sCtrlPrm.nAstPower30Min = 0x03; 
    
    MemWriteCtrlPrm(F1, 0, &sCtrlPrm, sizeof(sCtrlPrm));
    
    memset(&sCtrlPrm, 0, sizeof(sCtrlPrm));
    
    MemReadCtrlPrm(F1, 0, &sCtrlPrm, sizeof(sCtrlPrm));
    
    i = 0;
    
#endif
    
#if 0
    MEM_AC_REAL_DATA    sRealData;
    FORMAT11            sF;
    
    memset(&sRealData, 0, sizeof(MEM_AC_REAL_DATA));
    memset(&sF, 0x22, sizeof(sF));
    
    memcpy(&sRealData.sAcEnergy.sPEnergy[0], &sF, 4);
    
    MemWriteAcRealData(FEND, &sRealData, sizeof(MEM_AC_REAL_DATA));
    memset(&sRealData, 0, sizeof(MEM_AC_REAL_DATA));
    MemReadAcRealData(FEND, &sRealData, sizeof(MEM_AC_REAL_DATA));
    
    
    memset(&sF, 0x22, sizeof(sF));
    
#endif
   
}

/********************************************************************************
* ������: PowerOnProc
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ��ʼ���ϵ紦��(�����ι������)
*********************************************************************************/
BOOLEAN PowerOnProc(void)
{
	//1.���ݿ��˲�
	if (MemInfoChk() < 0)              
    {
        return FALSE;
    }
	WatchDogClear(CH_DOG_OUT);
	
    //�ն�Ӳ��������ʼ��
    if (!TermHardInit())
    {
        return FALSE;
    }
    WatchDogClear(CH_DOG_OUT);
    
    //�ն���Դ����
    if (!TermResCreate())
    {
    	return FALSE;
    }
    WatchDogClear(CH_DOG_OUT);

    //����Ӳ������
    DebugHard();

    //�ն����������ʼ��
    if (!TermSoftInit())
    {
    	return FALSE;
    }
    WatchDogClear(CH_DOG_OUT);

    //�����������
    DebugSoft();

    //�ն˽��̴���
    if (!TermTaskCreate())
    {
    	return FALSE;
    }
    WatchDogClear(CH_DOG_OUT);

    return TRUE;
}
