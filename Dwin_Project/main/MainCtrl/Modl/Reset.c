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
#include "Reset.h"
#include "MemTbl.h"
#include "MemDrvExt.h"
#include "MemDrv.h"
#include "Base.h"
#include "SoftTimer.h"
#include "CarrCol.h"
#include "Ac7022.h"
#include "PowerOn.h"
#include "SecTask.h"
/**************************************************************************************************
                                           相关全局变量实现
***************************************************************************************************/
INT8U   g_nInitType  = 0;

/**************************************************************************************************
                                           相关函数实现
***************************************************************************************************/

/********************************************************************************
* 函数名:MemLoadFromFlash
* 输　入:
* 输　出:
* 返  回:
* 功  能: flash数据加载到ram
*********************************************************************************/
INT8S MemLoadFromFlash(INT32U nAddr, INT8U* pBuf, INT32U nSize)
{
    INT32U   nOffset    = 0;
    INT32U   nTmpLen    = 0;

	memset(pBuf, 0, nSize);
	while (nOffset < nSize)
    {
        nTmpLen = GET_MIN((nSize - nOffset),  LEN_4K);
        MemoryRead (nAddr + nOffset, &pBuf[nOffset], nTmpLen, MEM_FLASH);
        nOffset += nTmpLen;
    }

    return RET_ERR_OK;
}

/********************************************************************************
* 函数名:MemInitRamFram
* 输　入:
* 输　出:
* 返  回:
* 功  能: 初始化Fram的数据
*********************************************************************************/
INT8S MemInitRamFram(INT32U nAddr, INT8U* pBuf, INT32U nSize)
{
	memset(pBuf, 0, nSize);
	MemWrite (NULL, MEM_MEDIUM_FRAM, nAddr, nSize, pBuf);
	
    return RET_ERR_OK;
}

/********************************************************************************
* 函数名:MemInitRamFlash
* 输　入:
* 输　出:
* 返  回:
* 功  能: 初始化flash的数据
*********************************************************************************/
INT8S MemInitRamFlash(char* pFilename, INT32U nAddr, INT8U* pBuf, INT32U nSize)
{
    INT32U   nOffset    = 0;
    INT32U   nTmpLen    = 0;

	memset(pBuf, 0, nSize);
	while (nOffset < nSize)
    {
        nTmpLen = GET_MIN((nSize - nOffset),  LEN_4K);
        MemWrite (pFilename, MEM_MEDIUM_FLASH, nAddr + nOffset, nTmpLen, &pBuf[nOffset]);
        nOffset += nTmpLen;
    }

    return RET_ERR_OK;
}

/********************************************************************************
* 函数名:MemCommPrmDefault
* 输　入:
* 输　出:
* 返  回:
* 功  能: 通信参数默认
*********************************************************************************/
INT8S MemCommPrmDefault(void)
{
    //设置F3
    INT8U   MainIp[4]   = {10,101,1,190};
    INT16U  MainPort    = 5200;
    INT8U   BackIp[4]   = {10,101,1,189};
    INT16U  BackPort    = 5200;
    INT8U   nAPN[16]    = {"cdscdl.sc"};
    INT8U   nBuf[50]    = {0};
    INT8U   nIndex      = 0;
    INT8U   nFnLen      = 2 * LEN_IP_ADDR + 4 + LEN_APN;
                 
    memcpy(&nBuf[nIndex], &MainIp[0], 4);
    nIndex += 4;
    INT16U2Buf(&nBuf[nIndex], MainPort);
    nIndex += 2;
    memcpy(&nBuf[nIndex], &BackIp[0], 4);
    nIndex += 4;
    INT16U2Buf(&nBuf[nIndex], BackPort);
    nIndex += 2;
    memcpy(&nBuf[nIndex], &nAPN[0], 16);
    
    MemWriteCommPrm(F3, &nBuf[0], nFnLen);
    
    return 0;
}

/********************************************************************************
* 函数名:MemMeterPrmDefault
* 输　入:
* 输　出:
* 返  回:
* 功  能: 交采电表参数默认
*********************************************************************************/
INT8S MemMeterPrmDefault(void)
{
    INT16U  nSeq        = 1;
    INT16U  nMP         = 1;
    INT32U  nBaud       =2400;
    INT8U   nAddr[6]    ={1};
    
    MEM_METER_PRM sMtPrm;
	pMEM_METER_PRM pMtPrm = &sMtPrm;
    
    memset(pMtPrm, 0, sizeof(MEM_METER_PRM));
    
    pMtPrm->bVld = TRUE;
    INT16U2Buf(&pMtPrm->nSeq[0], nSeq);
    INT16U2Buf(&pMtPrm->nMp[0], nMP);
    pMtPrm->nCom = 1;
    Long2Buf(&pMtPrm->nBaud[0], nBaud, 4);
    pMtPrm->nProt = 0x1E;
    MemCopy(&pMtPrm->nAddr[0], &nAddr[0], LEN_METER_ADDR);
    pMtPrm->nPwd[0] = 1;
    pMtPrm->nFeeNum = 4;
    pMtPrm->nXsws = 3;
    pMtPrm->nZsws = 4;
    pMtPrm->nMainType = 0;
    pMtPrm->nPartType = 2;
    
    MemWriteMeterPrm(nSeq, F10, pMtPrm, sizeof(MEM_METER_PRM));
    
    //写交采地址
    //MemWriteAcPrm(F19, &nAddr[0], LEN_METER_ADDR);
    return 0;
}

/********************************************************************************
* 函数名:AcPrmPrmInit
* 输　入:
* 输　出:
* 返  回:
* 功  能: 交采参数以及校表参数初始化
*********************************************************************************/
INT32S AcPrmPrmInit(void)
{
#if 0
    MEM_AC_CAL_PRM  sAcCalPrm;
    MEM_AC_PRM      sAcMem;
    
    memset(&sAcCalPrm, 0, sizeof(MEM_AC_CAL_PRM));
    memset(&sAcMem, 0, sizeof(MEM_AC_PRM));
    
    MemWriteAcCalPrm(FEND, &sAcCalPrm, sizeof(MEM_AC_CAL_PRM));
    MemWriteAcPrm(FEND, &sAcMem, sizeof(MEM_AC_PRM));
#endif
    INT8U	        nMeterAddr[LEN_METER_ADDR];
    INT8U	        nMeterNumber[LEN_METER_ADDR];
    INT8U	        nMeterProNum[LEN_METER_ADDR];
    INT32U          nHardType;
    MEM_AC_PRM      sAcMem;
    
    memset(&sAcMem, 0, sizeof(MEM_AC_PRM));
    
    MemReadAcPrm(FEND, &sAcMem, sizeof(sAcMem));
   // MemClear(AC_FILE, 0, sizeof(sAcMem), 0, MEM_MEDIUM_FLASH);
    memcpy(&nMeterAddr[0], &sAcMem.nMeterAddr[0], LEN_METER_ADDR);
    memcpy(&nMeterNumber[0], &sAcMem.nMeterNumber[0], LEN_METER_ADDR);
    memcpy(&nMeterProNum[0], &sAcMem.nMeterProNum[0], LEN_METER_ADDR);
    memcpy(&nHardType, &sAcMem.nHardType, sizeof(sAcMem.nHardType));
    
    if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType))
    {
      AcCalcJustData1(); 
    }
    else if(SINGLEPHASE_METER == g_nHardType)
    {
      AcCalcJustData2(); 
    }
    else if(TRIPHASE_METER_220ZT == g_nHardType)
    {
      AcCalcJustData3(); 
    }
    if (TRIPHASE_METER_57U == g_nHardType)
    {
      AcCalcJustData4();
    }
    else if(TRIPHASE_METER_100U == g_nHardType)
    {
      AcCalcJustData5(); 
    }
    else if(TRIPHASE_METER_100 == g_nHardType)
    {
      AcCalcJustData6(); 
    }
    
    memcpy(&sAcMem.nMeterAddr[0], &nMeterAddr[0], LEN_METER_ADDR);
    memcpy(&sAcMem.nMeterNumber[0], &nMeterNumber[0], LEN_METER_ADDR);
    memcpy(&sAcMem.nMeterProNum[0], &nMeterProNum[0], LEN_METER_ADDR);
    memcpy(&sAcMem.nHardType, &nHardType, sizeof(sAcMem.nHardType));
    
    MemWriteAcPrm(FEND, &sAcMem, sizeof(MEM_AC_PRM));
    
   // memset(&sAcMem, 0, sizeof(MEM_AC_PRM));
    
  //  MemReadAcPrm(FEND, &sAcMem, sizeof(sAcMem));
    
    ResetDelayHardInit(000);//复位  切换电表类型后
    
    return 0;
}


/********************************************************************************
* 函数名:
* 输　入:				  
* 输　出:
* 返  回:
* 功  能: 上电初始化
*********************************************************************************/
INT32S MemPowerOnInit(void)
{
	//1.数据库表核查
	if (MemInfoChk() < 0)              
    {
        return FALSE;
    }
	return TRUE;
}


/********************************************************************************
* 函数名:
* 输　入:				  
* 输　出:
* 返  回:
* 功  能: 数据初始化
*********************************************************************************/
INT32S MemDataInit(void)
{
    STD_TIME        sStdTimeSys;
    DAY_MEM_INFO    sDayInfo;
    MON_MEM_INFO    sMonInfo;
    INT8U           nRealIdx    = 0;
    
	//1.RAM临时交互数据	初始化
	memset(&g_pMem->sXRamData, 0, sizeof(g_pMem->sXRamData));

	//2.FLASH临时交互数据初始化
    memset(&g_pMem->sXFramData, 0, sizeof(g_pMem->sXFramData));

	//3.FRAM临时交互数据初始化
    memset(&g_pMem->sXFlhData, 0, sizeof(g_pMem->sXFlhData));
    //MemInitRamFram(STR_OFF(pMEM_FRAM_STORE, sXFramData), &g_pMem->sXFramData, sizeof(g_pMem->sXFramData));
    
    //日、月、曲线信息处理
    memset(&sDayInfo, 0, sizeof(sDayInfo));
    memset(&sMonInfo, 0, sizeof(sMonInfo));
    
    memset(&sStdTimeSys,0,sizeof(sStdTimeSys));
    GetSysClk(&sStdTimeSys);
    
    sDayInfo.nMemNum = 1;
    memcpy(&(sDayInfo.sTime[0].nDay), &(sStdTimeSys.nDay), sizeof(FORMAT20));
    MemWriteFlhXmpData(F2, 0, &sDayInfo, sizeof(sDayInfo));
    
    sMonInfo.nMemNum = 1;
    memcpy(&(sMonInfo.sTime[0].nMon), &(sStdTimeSys.nMon), sizeof(FORMAT21));
    MemWriteFlhXmpData(F3, 0, &sMonInfo, sizeof(sMonInfo));
    
    //清除实时数据
    MemClear(REAL_DATA_FILE, 0, sizeof(MEM_MP_REAL_DATA) * MAX_METER_NUM , 0, MEM_MEDIUM_FLASH);
    MemWriteDbfExt(MEM_OBJ_X_FLH_DATA, 0, F1, &nRealIdx, 1, NULL);
    
    return 0;
}


/********************************************************************************
* 函数名:
* 输　入:				  
* 输　出:
* 返  回:
* 功  能: 参数初始化(除通信参数) + 数据初始化
*********************************************************************************/
INT32S MemPartPrmInit(void)
{
	//初始化处通信参数外的参数
    //1.电表参数
    MemClear(METER_PRM_FILE, 0, MAX_METER_NUM * sizeof(MEM_METER_EXT_PRM), 0, MEM_MEDIUM_FLASH);
    //2.总加组参数
    MemClear(GRP_PRM_FILE, 0, MAX_GRP_MP_NUM * sizeof(MEM_METER_EXT_PRM), 0, MEM_MEDIUM_FLASH);
    //3.抄表运行参数
    MemClear(RDMETER_PRM_FILE, 0, sizeof(MEM_RDMETER_PRM), 0, MEM_MEDIUM_FLASH);
    //4.大小类号参数
    MemClear(METER_TYPE_I_FILE, 0, sizeof(MEM_METER_TYPE_PRM), 0, MEM_MEDIUM_FLASH);
    //5.任务参数
    MemClear(TASK_I_FILE, 0, sizeof(MEM_TASK_PRM), 0, MEM_MEDIUM_FLASH);
    //6.控制参数
    MemClear(CTRL_FILE, 0, sizeof(MEM_CTRL_PRM), 0, MEM_MEDIUM_FLASH);
    //6.其他参数
    MemClear(OTHER_PRM_FILE, 0, sizeof(MEM_OTHER_PRM), 0, MEM_MEDIUM_FLASH);

    //需要赋值默认值的赋值
    //+++++
    
    //数据初始化
    MemDataInit();
return 0;	
}

/********************************************************************************
* 函数名:
* 输　入:				  
* 输　出:
* 返  回:
* 功  能: 参数初始化 + 数据初始化
*********************************************************************************/
INT32S MemFactoryInit(void)
{
    MEM_TERM_COMM_PRM sCommPrm;
    
    
    memset(&sCommPrm, 0, sizeof(MEM_TERM_COMM_PRM));
    MemReadCommPrm(FEND, &sCommPrm, sizeof(MEM_TERM_COMM_PRM));
    memset(&sCommPrm.nRadioDlyMs, 0, sizeof(MEM_TERM_COMM_PRM) - STR_OFF(pMEM_TERM_COMM_PRM, nRadioDlyMs));
    
	//1.普通参数初始化
    //1.电表参数
    MemClear(METER_PRM_FILE, 0, MAX_METER_NUM * sizeof(MEM_METER_EXT_PRM), 0, MEM_MEDIUM_FLASH);
    //2.总加组参数
    MemClear(GRP_PRM_FILE, 0, MAX_GRP_MP_NUM * sizeof(MEM_METER_EXT_PRM), 0, MEM_MEDIUM_FLASH);
    //3.抄表运行参数
    MemClear(RDMETER_PRM_FILE, 0, sizeof(MEM_RDMETER_PRM), 0, MEM_MEDIUM_FLASH);
    //4.大小类号参数
    MemClear(METER_TYPE_I_FILE, 0, sizeof(MEM_METER_TYPE_PRM), 0, MEM_MEDIUM_FLASH);
    //5.任务参数
    MemClear(TASK_I_FILE, 0, sizeof(MEM_TASK_PRM), 0, MEM_MEDIUM_FLASH);
    //6.控制参数
    MemClear(CTRL_FILE, 0, sizeof(MEM_CTRL_PRM), 0, MEM_MEDIUM_FLASH);
    //6.其他参数
    MemClear(OTHER_PRM_FILE, 0, sizeof(MEM_OTHER_PRM), 0, MEM_MEDIUM_FLASH);
    
    //2.2通信参数初始化
    //MemClear(COMM_PRM_FILE, STR_OFF(pMEM_TERM_COMM_PRM, nRadioDlyMs[0]), sizeof(MEM_TERM_COMM_PRM) - STR_OFF(pMEM_TERM_COMM_PRM, nRadioDlyMs), 0, MEM_MEDIUM_FLASH);
    MemWriteCommPrm(FEND, &sCommPrm, sizeof(MEM_TERM_COMM_PRM));
    MemCommPrmDefault();
    //需要赋值默认值的赋值
    //+++++
    MemMeterPrmDefault();
    
    //数据初始化
    MemDataInit();
    
    //载波模块初始化
    if(g_nCarrZbSelFlg == TYPE_CARR)
    {
        CarrRouteInit();
    }
    
    return 0;
}

/********************************************************************************
* 函数名:MemInitAllExt
* 输　入:				  
* 输　出:
* 返  回:
* 功  能: 所有参数初始化--包括校表参数以及交采参数
*********************************************************************************/
INT32S MemInitAllExt(void)
{
    INT8U   nFileInitFlg = 0;
    INT8U   nFlg    = 0;
    
    nFlg = 1;
    MemWriteDbfExt(MEM_OBJ_X_RAM_DATA, 0, F5, &nFlg, 1, NULL);
    
    nFileInitFlg = 1;
    MemWriteFramXmpData(F1, 0, &nFileInitFlg, 1);
    //MemFactoryInit();
    //AcPrmPrmInit();
    return 0;
}


/********************************************************************************
* 函数名:
* 输　入:		nOpt
            1:  上电初始化;
            2:  F2数据初始化;
            3:  F4除通信参数外所以初始化;
            4:  F3参数及数据区初始化(即恢复至出厂设置);
            
* 输　出:
* 返  回:
* 功  能: 初始化接口
*********************************************************************************/
INT32S MemInit(INT8U nOpt)
{
	INT8S	nRet	= RET_ERR_ERROR;

	switch (nOpt)
	{
		case MEM_INIT_POWER_ON:					//上电
			nRet = MemPowerOnInit();
			break;

		case MEM_INIT_DATA:  					//数据区复位（清除历史数据）
			nRet = MemDataInit();
            g_nInitType = 0;
			break;
		case MEM_INIT_FACTORY:  				//恢复出厂设置
			nRet = MemFactoryInit();
            g_nInitType = 0;
			break;   
        case MEM_INIT_PART_PRM:  				//所有可写的参数（通信参数除外）
			nRet = MemPartPrmInit();
            g_nInitType = 0;
			break; 
		default:								//默认
			nRet = RET_ERR_OPT;
			break;
	}
	return nRet;
	
}

void AcEnergyInit(void)
{
    //清除铁电数据（另扩命令）
    memset(&g_pMem->sACRealData, 0, sizeof(g_pMem->sACRealData));
	MemoryWrite(STR_OFF(pMEM_FRAM_STORE, sAcRealData), (INT8U*)&g_pMem->sACRealData, sizeof(g_pMem->sACRealData), MEM_FRAM);
    
}





