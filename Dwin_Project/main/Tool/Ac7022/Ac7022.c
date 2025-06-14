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
#include "ATT7022.h"
#include "Ac7022.h"
#include "AppCfg.h"
#include "SysRes.h"
#include "MemDrvExt.h"
#include "SoftTimer.h"
#include "MemData.h"
#include "Base.h"
#include "Version.h"
#include "Fun.h"
#include "PowerOn.h"
#include "SecTask.h"
//INT32U      g_nConstant = g_nAcPulseConst;
INT32U      g_nHFConst    = 182;

INT8U g_SimMeterTripFlag = 0;           //模拟跳闸标志 0xFF开 0xBF 闭
INT8U g_RealMeterTripFlag = 0;          //实际跳闸标志

INT8U g_SimMeterSwitchOnFlag = 0;       //模拟合闸标志 0xFF开 0xBF 闭
INT8U g_RealMeterSwitchOnFlag= 0;       //实际合闸标志
INT8U g_SimLoadSwitchAcFlag = 0;        //模拟负荷开关误动作标志 0xFF开 0xBF 闭
INT8U g_RealLoadSwitchAcFlag= 0;        //实际负荷开关误动作标志
INT8U g_SimPowerAbnlFlag = 0;           //模拟电源异常标志 0xFF开 0xBF 闭
INT8U g_RealPowerAbnlFlag= 0;           //实际电源异常标志
INT8U g_SimClockTrbFlag = 0;            //模拟时钟故障标志 0xFF开 0xBF 闭
INT8U g_RealClockTrbFlag= 0;            //实际时钟故障标志
INT8U g_SimChipTrbFlag = 0;             //模拟计量芯片故障标志 0xFF开 0xBF 闭
INT8U g_RealChipTrbFlag= 0;             //实际计量芯片故障标志
INT8U g_SimOCPTrbFlag = 0;              //模拟过流故障标志 0xFF开 0xBF 闭
INT8U g_RealOCPTrbFlag= 0;              //实际过流故障标志

INT8U g_ACOffset = 100; 

INT8U       g_CoverRunFlag = 0;
INT8U       g_CoverRunFlag2 = 0;        //时钟故障事件
INT8U       g_CoverRunFlag3 = 0;        //计量芯片故障事件
INT8U       g_CoverRunFlag4 = 0;        //跳闸事件
INT8U       g_CoverRunFlag5 = 0;        //合闸事件
INT8U       g_CoverRunFlag6 = 0;        //负荷开关事件
INT8U       g_CoverRunFlag7 = 0;        //电源异常事件
INT8U       g_CoverRunFlag8 = 0;        //过流事件
INT8U       g_CoverRunFlag9 = 0;        //上电初始化数据库标识
INT8U  g_pflagcount = 0;
INT8U       nWuCh = 0;                //误差%比
INT8U       nZFWuCh = 0;             //误差正负

#define     AC_ENG_CAL_METHOD       AC_ENG_CAL_ALG

BOOLEAN g_bAcInitFail = FALSE;  		    //交采初始化失败标志:TRUE表示失败

extern INT8U g_MeterInsideWrong[2];

PULSE_CONST nPulTable[] = 
{
	{3200,  365, 803},
    
       {0, 0, 0},

	{9600,  121, 267},
};

const   MT_698FZDBFN    g_n698FZDBOIt2Fn[] = {

  {0x0000,	F1},
  {0x0010,	F1},
  {0x0020,	F1},
  {0x0050,	F2},
  {0x0060,	F2},
  {0x0070,	F2},
  {0x0080,	F2},
  {0x2000,	0xFF},
  {0x2001,	0xFF},

  {0xFF, 0xFF},
};


void Pulse(void)
{
    if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType))
    {
        nPulTable[1].nPulConst = 6400;
        nPulTable[1].nHFConst1 = 182;
        nPulTable[1].nHFConst2 = 401;
    }
    else if(SINGLEPHASE_METER == g_nHardType)
    {
        nPulTable[1].nPulConst = 1200;
        nPulTable[1].nHFConst1 = 147;
        nPulTable[1].nHFConst2 = 401; 
    }
    else if (TRIPHASE_METER_100 == g_nHardType)
    {
        nPulTable[1].nPulConst = 6400;
        nPulTable[1].nHFConst1 = 221;
        nPulTable[1].nHFConst2 = 125;
    }
    else if(TRIPHASE_METER_220ZT == g_nHardType)
    {
        nPulTable[1].nPulConst = 400;
        nPulTable[1].nHFConst1 = 874;
        nPulTable[1].nHFConst2 = 401;
    }
    else if (TRIPHASE_METER_57U == g_nHardType)
    {
        nPulTable[1].nPulConst = 20000;
        nPulTable[1].nHFConst1 = 58;
        nPulTable[1].nHFConst2 = 401;  
    }   
    else if (TRIPHASE_METER_100U == g_nHardType)
    {
        nPulTable[1].nPulConst = 20000;
        nPulTable[1].nHFConst1 = 82;
        nPulTable[1].nHFConst2 = 57;
    } 
    
}

 
/*
  const PULSE_CONST nPulTable[] = 
  {
    {3200,  365, 803},
    
    {1200,  147, 401},

	{9600,  121, 267},
  };
*/
FP32    fMulFlag = 0.0;					    //交采电压放大系数
INT8U   AcEventTime[8][MAX_PHASE_NUM];	    //失压，过压，断相，失流发生统计时间
INT8U   AcEventRecTime[8][MAX_PHASE_NUM];

REAL_DATA   sDataTable;
pREAL_DATA  pDataTable = &sDataTable;

SAVE_REAL_DATA  sSavDataTable;
pSAVE_REAL_DATA pSavDataTable = &sSavDataTable;

INT8U nAcTmpBuf[1024] = {0};
//INT8U g_nFreeChgFlg = 0;     //费率切换时清除电能量标志，只有在断电后从存储中获取到当前市值后，才可以清零


/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 交流采样采集任务
*********************************************************************************/
void AcColTask(void *pData)
{
    INT8U	nErr = 0;
    AcCalcInit();
    for(; ;)
    {
        OSSemPend(g_pSem[SEM_COL_AC_CALC_IDX], 0, &nErr);
        
        AcCalcProc();
    }
}

/**************************************************************************************************
										  相关函数实现
***************************************************************************************************/

BOOLEAN CkeckACCalPrm(void)
{
    INT8U  i=0;
    INT8U	nErr = 0;
    INT8S  bSuccflag = TRUE;
	INT32U nChkSum = 0, nChkSum1 = 0;
    MEM_AC_CAL_PRM  sJbParam;

    memset(&sJbParam, 0, sizeof(MEM_AC_CAL_PRM));
    MemReadAcCalPrm(FEND, &sJbParam, sizeof(sJbParam));                                    

    MemReadAcCalPrm(F31, &nChkSum, sizeof(nChkSum1));
	OSSemPend(g_pSem[SEM_ACREAD_CTRL_IDX], 0, &nErr);
	nChkSum1 = ATT7022RegRead(r_ChkSum);
	OSSemPost(g_pSem[SEM_ACREAD_CTRL_IDX]);
    
    //if ((nChkSum1 & 0x00FFFFFF) != ((nChkSum & 0x00FFFFFF) + 0x00009000))
    if ((nChkSum1 & 0x00FFFFFF) != nChkSum)
    {
        bSuccflag = FALSE;
		//if (g_pDbgTbl->bAcMonit)
        {
            //DebugPrintf("ChkSum False");
        }
        while (RET_ERR_OK != AcCalcJust7022(&sJbParam)&&(++i<3))
		{
		}
        if(i==3)
        {
           bSuccflag = FALSE;
           return (bSuccflag);
        }
        //AcCalDataInit();

		bSuccflag = TRUE;
        //DebugPrintf("ChkSum ok");
    }
    return (bSuccflag);
}

/********************************************************************************
* 函数名:AcCalDataInit
* 输　入:
* 输  出:
* 返  回:
* 功  能:交采数据区初始化
*********************************************************************************/
void AcCalDataInit(void)
{
    STD_TIME sTime;

	memset(pSavDataTable, 0x00,	sizeof(SAVE_REAL_DATA));
    Pulse();
    
    if (RET_ERR_OK != ReadAcCombPrm())
    {
        
    }
    if (RET_ERR_OK != ReadAcPluPrm())
	{
		
	}
	if (RET_ERR_OK != ReadAcENData())
	{
		
	}
	if (RET_ERR_OK != ReadAcDEData())
	{
		
	}
    //获取系统时钟
    memset(&sTime, 0, sizeof(STD_TIME));
    if (RET_ERR_OK != GetSysClk(&sTime))
    {
        pDataTable->nRateNo = 3;
    }
    else
    {
		if(RET_ERR_OK != AcCalcPerChanged(&sTime))
		{
			pDataTable->nRateNo = 3;
		}
	}
    OSTimeDlyHMSM(0,0,0,800);
}

/********************************************************************************
* 函数名:ReadAcPluPrm
* 输　入:
* 输  出:
* 返  回:错误编码
* 功  能:读出交采脉冲常数
*********************************************************************************/
INT8S ReadAcPluPrm(void)
{
    INT8S   nRet = RET_ERR_ERROR;
   	INT16U  ii = 0;
    MEM_AC_CAL_PRM sJbParam;
    
    memset(&sJbParam, 0, sizeof(MEM_AC_CAL_PRM));
    pDataTable->nPulseConst = g_nAcPulseConst;
    
    if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType))
    {
      pDataTable->nHFConst = 0x000000B6;
    }

    else if(SINGLEPHASE_METER == g_nHardType)  
    {
      pDataTable->nHFConst = 147;
    }
    else if(TRIPHASE_METER_220ZT == g_nHardType)  
    {
        pDataTable->nHFConst = 874;
    }
    else if (TRIPHASE_METER_57U == g_nHardType)
    {
        pDataTable->nHFConst = 58;
    }
    else if(TRIPHASE_METER_100U == g_nHardType)  
    {
        pDataTable->nHFConst = 82;
    }
    else if(TRIPHASE_METER_100 == g_nHardType)  
    {
        pDataTable->nHFConst = 221;
    }
    
    if (MemReadAcCalPrm(FEND, &sJbParam, sizeof(sJbParam)) >= 0)
    {
        nRet = RET_ERR_OK;
    }
    if(RET_ERR_OK != nRet)
    {
        return RET_ERR_ERROR;
    }
    pDataTable->nCheckSumReset = sJbParam.nCheckSumReset;
	//pDataTable->nCheckSumEntire = (sJbParam.nCheckSumEntire & 0x00FFFFFF) + 0x00009000;
    pDataTable->nCheckSumEntire = (sJbParam.nCheckSumEntire & 0x00FFFFFF);
    
    for(ii = 0; ii < 3; ii++)
   	{
   		/*if (((nPulTable[ii].nHFConst2 == sJbParam.nHfconst) && (0x0001F0CD == sJbParam.nCheckSumReset))
   			||((nPulTable[ii].nHFConst1 == sJbParam.nHfconst) && (0x0001E4CD == sJbParam.nCheckSumReset)))*/
          
        if (((nPulTable[ii].nHFConst2 == sJbParam.nHfconst) && (0x0001E0CD == sJbParam.nCheckSumReset))
   			||((nPulTable[ii].nHFConst1 == sJbParam.nHfconst) && (0x0001D4CD == sJbParam.nCheckSumReset)))
   		{
   			pDataTable->nPulseConst = nPulTable[ii].nPulConst;
   			pSavDataTable->nPulseConst = nPulTable[ii].nPulConst;
   			pDataTable->nHFConst = sJbParam.nHfconst;
   			pSavDataTable->nHFConst = sJbParam.nHfconst;
   			break;
   		}
	}

    return RET_ERR_OK;
}

/********************************************************************************
* 函数名:ReadAcComPrm
* 输　入:
* 输  出:
* 返  回:错误编码
* 功  能:读出交采组合电能控制字
*********************************************************************************/
INT8S ReadAcCombPrm(void)
{
    //INT8S nRet = RET_ERR_ERROR;
    //INT8U i = 0;
    //INT8U nComb[3] = {0, 0, 0};
    
    //取默认值，参数暂时不使用

    pDataTable->nCombEp = 0x05;
    pSavDataTable->nCombEp = 0x05;
    pDataTable->nCombEq1 = 0x05;
    pSavDataTable->nCombEq1 = 0x05;
    pDataTable->nCombEq2 = 0x50;
    pSavDataTable->nCombEq2 = 0x50;
    
#if 0
    for (i = 0; i < 3; i++)
    {
        nRet = MemReadAcPrm(F22 + i, &nComb[i], 1);
        if(RET_ERR_OK != nRet)
        {
            return RET_ERR_ERROR;
        }
    }
    pDataTable->nCombEp = nComb[0];
    pSavDataTable->nCombEp = nComb[0];
    pDataTable->nCombEq1 = nComb[1];
    pSavDataTable->nCombEq1 = nComb[1];
    pDataTable->nCombEq2 = nComb[2];
    pSavDataTable->nCombEq2 = nComb[2];
    
#endif

    return RET_ERR_OK;
}


/*
*****************************************************************************************************
* 函数名: AcCalcJbParamInit
* 输　入:
* 输  出:
* 返  回:
* 功能描述:交采参数初始化
*****************************************************************************************************
*/
INT8S AcCalcJbParamInit(pMEM_AC_CAL_PRM pJB_Param)
{
    INT8U ii;
	INT32U nChkSum = 0;
    FP32  nTmp = 0;
    FP32  nTmp2 = 0;
    FP32  nTmp3 = 0;

	for(ii = 0; ii < 4; ii++)
	{
		/*ATT7022Reset(200);
		OSTimeDlyHMSM(0, 0, 0, 200);
		ATT7022RegWrite(0xC3, 0x00000000);
		OSTimeDlyHMSM(0, 0, 0, 200);*/
		nChkSum = ATT7022RegRead(r_ChkSum);
		
		//if((0x0001E4CD == nChkSum) || (0x0001F0CD == nChkSum))
        if((0x0001D4CD == nChkSum) || (0x0001E0CD == nChkSum))
		{
			pJB_Param->nCheckSumReset = nChkSum;
			break;
		}
        
        ATT7022Reset(200);
		OSTimeDlyHMSM(0, 0, 0, 200);
		ATT7022RegWrite(0xC3, 0x00000000);
		OSTimeDlyHMSM(0, 0, 0, 200);
	}
    
	if(ii == 3)
	{
		return RET_ERR_ERROR;
	}
    
    pDataTable->nPulseConst = g_nAcPulseConst;
	pSavDataTable->nPulseConst = g_nAcPulseConst;
	/*
	pJB_Param->nModeCfg = 0x0000B9FE;
	pJB_Param->nEMUCfg = 0x00003184;
    
    nTmp = 6.6;
    nTmp2 = 3.34;
    nTmp3 = 1.8;
    
	if (0x0001E0CD == nChkSum)
	{
		pJB_Param->nHfconst = 0x00273AD3/pDataTable->nPulseConst;
        
        if(SINGLEPHASE_METER == g_nHardType)
        {
            pJB_Param->nHfconst = (INT32U)((FP32)pJB_Param->nHfconst / nTmp);
        }
        else if(TRIPHASE_METER_100U == g_nHardType)  
        {
            pJB_Param->nHfconst = (INT32U)((FP32)pJB_Param->nHfconst / nTmp3);
        }
		pJB_Param->nFailVoltage = 0x00001200;
	}
	else
	{
		pJB_Param->nHfconst = 0x0011D4EB/pDataTable->nPulseConst;//1161689
        if(SINGLEPHASE_METER == g_nHardType)
        {
          pJB_Param->nHfconst = (INT32U)((FP32)pJB_Param->nHfconst / nTmp);
        }
       else if(TRIPHASE_METER_220ZT == g_nHardType)
       {
            pJB_Param->nHfconst = (INT32U)((FP32)pJB_Param->nHfconst / nTmp2);
       }
		pJB_Param->nFailVoltage = 0x00000600;
	}*/
    
    if ((TRIPHASE_METER_220 == g_nHardType)||(TRIPHASE_METER_57 == g_nHardType))
    {
        pJB_Param->nModeCfg = 0x0000B9FE;
        pJB_Param->nEMUCfg = 12740;
        pJB_Param->nHfconst = 0x000000B6;
        pJB_Param->nFailVoltage = 0x00000600;
    }
    else if(SINGLEPHASE_METER == g_nHardType)  
    {
        pJB_Param->nModeCfg = 0x0000B9FE;
        pJB_Param->nEMUCfg = 12740;
        pJB_Param->nHfconst = 147;
        pJB_Param->nFailVoltage = 0x00000600;
    }
    else if(TRIPHASE_METER_220ZT == g_nHardType)  
    {
        pJB_Param->nModeCfg = 0x0000B9FE;
        pJB_Param->nEMUCfg = 12740;
        pJB_Param->nHfconst = 874;
        pJB_Param->nFailVoltage = 0x00000600;
    }
    else if (TRIPHASE_METER_57U == g_nHardType)
    {
        pJB_Param->nModeCfg = 0x0000B9FE;
        pJB_Param->nEMUCfg = 12740;
        pJB_Param->nHfconst = 58;
        pJB_Param->nFailVoltage = 0x00000600;
    }
    else if(TRIPHASE_METER_100U == g_nHardType)  
    {
        pJB_Param->nModeCfg = 0x0000B9FE;
        pJB_Param->nEMUCfg = 0x00003184;
        pJB_Param->nHfconst = 82;
        pJB_Param->nFailVoltage = 0x00001200;
    }
    else if (TRIPHASE_METER_100 == g_nHardType)
    {
        pJB_Param->nModeCfg = 0x0000B9FE;
        pJB_Param->nEMUCfg = 0x00003184;
        pJB_Param->nHfconst = 221;
        pJB_Param->nFailVoltage = 0x00001200;
    }
    
    pJB_Param->nIstarup = AC_ULI_STARTUP;
	
	pJB_Param->nEMUIE = 0x00000001;
	pJB_Param->nModuleCFG = 0x00003436;//使能温度检测 2013.01.28
	pJB_Param->nLineGain = 0x00002C59;
	pJB_Param->nPineCtrl = 0x0000000F;
	pJB_Param->nPstart = 0x00000030;
	pJB_Param->nIregion =0x00007FFF;
	pJB_Param->nReserved[0] = 0x00001000;
	pJB_Param->nReserved[1] = 0x00004500;

	pJB_Param->nCheckSumEntire = CheckINT32USum((INT32U*)(pJB_Param), 0, sizeof(MEM_AC_CAL_PRM)/4 - 3);// 所有校表寄存器的累加和(含固定值寄存器)

	return RET_ERR_OK;
    
}

/*
*****************************************************************************************************
* 函数名: AcCalcJust7022
* 输　入:
* 输  出:
* 返  回:
* 功能描述:软件校表
*****************************************************************************************************
*/
INT8S AcCalcJust7022(pMEM_AC_CAL_PRM pJB_Param)
{
    INT8U  ii;
	INT8S  nSuccflag = RET_ERR_ERROR;
	INT8U  nErr = 0;
	INT32U nChkSum = 0;
    MEM_AC_CAL_PRM  sJbParam;
  
    memset(&sJbParam, 0, sizeof(MEM_AC_CAL_PRM));
    
    if (MemReadAcCalPrm(FEND, &sJbParam, sizeof(sJbParam) ) > 0 )
    {
        nSuccflag = RET_ERR_OK;
    }
    
    memcpy(pJB_Param, &sJbParam, sizeof(MEM_AC_CAL_PRM));
    
    
    if (RET_ERR_OK == nSuccflag)
    {
        nChkSum = CheckINT32USum((INT32U*)(pJB_Param), 0, sizeof(MEM_AC_CAL_PRM)/4 - 3);
		if (nChkSum != pJB_Param->nCheckSumEntire)
		{
			return RET_ERR_ERROR;
		}
        OSSemPend(g_pSem[SEM_ACREAD_CTRL_IDX], 0, &nErr);
		ATT7022Reset(200);
		OSTimeDlyHMSM(0, 0, 0, 200);
		ATT7022RegWrite(0xC3, 0x00000000);
		OSTimeDlyHMSM(0, 0, 0, 200);
		nChkSum = ATT7022RegRead(r_ChkSum);
		OSSemPost(g_pSem[SEM_ACREAD_CTRL_IDX]);
		if (nChkSum != pJB_Param->nCheckSumReset)
		{
			return RET_ERR_ERROR;
		}
        
        pDataTable->nCheckSumEntire = pJB_Param->nCheckSumEntire;
    
        OSSemPend(g_pSem[SEM_ACREAD_CTRL_IDX], 0, &nErr);
        //开 校表寄存器写操作
        ATT7022RegWrite(0xc9, 0x0000005a);
        ATT7022RegWrite((w_ModeCfg | 0x80), pJB_Param->nModeCfg);
        ATT7022RegWrite((w_EMUCfg | 0x80), pJB_Param->nEMUCfg);//
            
        ATT7022RegWrite((w_ModuleCFG | 0x80), pJB_Param->nModuleCFG);
        ATT7022RegWrite((w_QPhscal | 0x80), 0x00000000);
        ATT7022RegWrite((w_Hfconst | 0x80), pJB_Param->nHfconst);
        ATT7022RegWrite((w_Istarup | 0x80), pJB_Param->nIstarup);
            
        for(ii=0; ii<3; ii++)
        {
            ATT7022RegWrite((w_PgainA+ii)|0x80, pJB_Param->nPgain[ii]);
            ATT7022RegWrite((w_QgainA+ii)|0x80, pJB_Param->nQgain[ii]);
            ATT7022RegWrite((w_SgainA+ii)|0x80, pJB_Param->nSgain[ii]);
            ATT7022RegWrite((w_PhSregApq0+ii)|0x80, pJB_Param->nPhSregpq0[ii]);
            ATT7022RegWrite((w_PhSregApq1+ii)|0x80, pJB_Param->nPhSregpq1[ii]);
            ATT7022RegWrite((w_PoffsetA+ii)|0x80, pJB_Param->nPoffset[ii]);
            ATT7022RegWrite((w_UgainA+ii)|0x80, pJB_Param->nUgain[ii]);
            ATT7022RegWrite((w_IgainA+ii)|0x80, pJB_Param->nIgain[ii]);
            ATT7022RegWrite((w_QoffsetA+ii)|0x80, pJB_Param->nQoffset[ii]);
            ATT7022RegWrite((w_UaRmsoffse+ii)|0x80, pJB_Param->nURmsoffse[ii]);
            ATT7022RegWrite((w_IaRmsoffse+ii)|0x80, pJB_Param->nIRmsoffse[ii]);
        }
        //关校表寄存器写操作
        ATT7022RegWrite(0xc9, 0x00000000);
        OSSemPost(g_pSem[SEM_ACREAD_CTRL_IDX]);
    }
    
    return nSuccflag;
}

/*
*****************************************************************************************************
* 函数名: AcCalcReset7022
* 输　入:
* 输  出:
* 返  回:
* 功能描述:软件校表清零
*****************************************************************************************************
*/
INT8S AcCalcReset7022(pMEM_AC_CAL_PRM pJB_Param)
{
    INT8U  ii;
	INT8S  nSuccflag = RET_ERR_ERROR;
	INT8U  nErr = 0;
	INT32U nChkSum = 0;
    MEM_AC_CAL_PRM  sJbParam;
  
    memset(&sJbParam, 0, sizeof(MEM_AC_CAL_PRM));
    
    if (MemReadAcCalPrm(FEND, &sJbParam, sizeof(sJbParam) ) > 0 )
    {
        nSuccflag = RET_ERR_OK;
    }
    
    memcpy(pJB_Param, &sJbParam, sizeof(MEM_AC_CAL_PRM));
    
    
    if (RET_ERR_OK == nSuccflag)
    {
        nChkSum = CheckINT32USum((INT32U*)(pJB_Param), 0, sizeof(MEM_AC_CAL_PRM)/4 - 3);
		if (nChkSum != pJB_Param->nCheckSumEntire)
		{
			return RET_ERR_ERROR;
		}
        pDataTable->nCheckSumEntire = pJB_Param->nCheckSumEntire;
        
        OSSemPend(g_pSem[SEM_ACREAD_CTRL_IDX], 0, &nErr);
		ATT7022Reset(200);
		OSTimeDlyHMSM(0, 0, 0, 200);
		ATT7022RegWrite(0xC3, 0x00000000);
		OSTimeDlyHMSM(0, 0, 0, 200);
		nChkSum = ATT7022RegRead(r_ChkSum);
		OSSemPost(g_pSem[SEM_ACREAD_CTRL_IDX]);
		if (nChkSum != pJB_Param->nCheckSumReset)
		{
			return RET_ERR_ERROR;
		}
    
        OSSemPend(g_pSem[SEM_ACREAD_CTRL_IDX], 0, &nErr);
        //开 校表寄存器写操作
        ATT7022RegWrite(0xc9, 0x0000005a);
        ATT7022RegWrite((w_ModeCfg | 0x80), pJB_Param->nModeCfg);
        ATT7022RegWrite((w_EMUCfg | 0x80), pJB_Param->nEMUCfg);//
            
        ATT7022RegWrite((w_ModuleCFG | 0x80), pJB_Param->nModuleCFG);
        ATT7022RegWrite((w_QPhscal | 0x80), 0x00000000);
        ATT7022RegWrite((w_Hfconst | 0x80), pJB_Param->nHfconst);
        ATT7022RegWrite((w_Istarup | 0x80), pJB_Param->nIstarup);
            
        for(ii=0; ii<3; ii++)
        {
            ATT7022RegWrite((w_PgainA+ii)|0x80, pJB_Param->nPgain[ii]);
            ATT7022RegWrite((w_QgainA+ii)|0x80, pJB_Param->nQgain[ii]);
            ATT7022RegWrite((w_SgainA+ii)|0x80, pJB_Param->nSgain[ii]);
            ATT7022RegWrite((w_PhSregApq0+ii)|0x80, pJB_Param->nPhSregpq0[ii]);
            ATT7022RegWrite((w_PhSregApq1+ii)|0x80, pJB_Param->nPhSregpq1[ii]);
            ATT7022RegWrite((w_PoffsetA+ii)|0x80, pJB_Param->nPoffset[ii]);
            ATT7022RegWrite((w_UgainA+ii)|0x80, pJB_Param->nUgain[ii]);
            ATT7022RegWrite((w_IgainA+ii)|0x80, pJB_Param->nIgain[ii]);
            ATT7022RegWrite((w_QoffsetA+ii)|0x80, pJB_Param->nQoffset[ii]);
            ATT7022RegWrite((w_UaRmsoffse+ii)|0x80, pJB_Param->nURmsoffse[ii]);
            ATT7022RegWrite((w_IaRmsoffse+ii)|0x80, pJB_Param->nIRmsoffse[ii]);
        }
        //关校表寄存器写操作
        ATT7022RegWrite(0xc9, 0x00000000);
        OSSemPost(g_pSem[SEM_ACREAD_CTRL_IDX]);
        
    }
    
    return nSuccflag;
}

/********************************************************************************
* 函数名:AcCalcInit
* 输　入:
* 输　出:
* 返  回:
* 功  能:交采初始化
*********************************************************************************/
BOOLEAN AcCalcInit(void)
{
    INT32U nChkSum = 0;
    INT8U  nFlg  = 0;
    
    ATT7022Init();
    ATT7022Reset( 200 );
    OSTimeDlyHMSM(0, 0, 0, 500);
    nChkSum = ATT7022RegRead(r_ChkSum);
    AcCalDataInit();
    g_bAcInitFail = CkeckACCalPrm();
    
    MemReadAcXFlhBackData(F112, &g_ACOffset, sizeof(g_ACOffset));
    //针对脉冲常数为20000的强制写入
    if (nFlg)
    {
        AcCalcJustData4();
    }
    
    return(g_bAcInitFail);
}

/********************************************************************************
* 函数名:AcValueSwitch
* 输　入:
* 输　出:
* 返  回:
* 功  能:将从7022读到的原始数据转换成计算所需要的格式
*********************************************************************************/
FP32 AcValueSwitch(INT32U uConstant, INT32U uHFConst, INT8U uR_ID, INT32U uData)
{
    INT32U	uDat7022;
	FP32	fDat1 = 0.0;

	uDat7022 = uData;//read_7022(uR_ID);
	
	if ((0 == uConstant) || (0 == uHFConst))
	{
		return (0);
	}
	
	//补码转换
	if(uDat7022&0x00800000)
	{
		uDat7022 = 0x01000000-uDat7022;
	}

	if(0 == uDat7022)
	{
		return (0);
	}

	//不同类型数据处理
	switch(uR_ID)
	{
		case r_UaRms:
		case r_UbRms:
		case r_UcRms:
		case r_IaRms:
		case r_IbRms:
		case r_IcRms:
		case r_ItRms:
		case r_Freq:				//线频率转换
			fDat1 = (FP32)uDat7022;
			fDat1 /= (1024*8);		//有效值转换
			break;
    	
		case r_Pa:
		case r_Pb:
		case r_Pc:
		case r_Qa:
		case r_Qb:
		case r_Qc:
		case r_Sa:
		case r_Sb:
		case r_Sc:
			fDat1 = (FP32)uDat7022;
			fDat1 = fDat1*25920000000/8388608;		//分相功率转换
			fDat1 = fDat1/((FP32)uConstant);
			fDat1 = fDat1/((FP32)uHFConst);
			fDat1 = fDat1/1000;
			break;
		case r_Pt:
		case r_Qt:
		case r_St:
			fDat1 = (FP32)uDat7022;
			fDat1 = fDat1*2*25920000000/8388608;		//合相功率转换
			fDat1 = fDat1/((FP32)uConstant);
			fDat1 = fDat1/((FP32)uHFConst);
			fDat1 = fDat1/1000;
			break;
    	
		case r_Pfa:
		case r_Pfb:
		case r_Pfc:
		case r_Pft:
			fDat1 = (FP32)uDat7022;
			fDat1 /= (1024*1024*8);		//功率因数转换
			break;
			
    	case r_YUaUb:
		case r_YUaUc:
		case r_YUbUc:				//电压夹角转换
		case r_Pga:
		case r_Pgb:
		case r_Pgc:
			fDat1 = (FP32)uDat7022;
			fDat1 /= (1024*1024);
			fDat1 *= 180;
			break;
    	
		default:
			fDat1 = (FP32)uDat7022;
			break;
	}
	return (fDat1);
}

/********************************************************************************
* 函数名:AcCalcProc
* 输　入:
* 输　出:
* 返  回:
* 功  能:交采处理
*********************************************************************************/
void AcCalcProc(void)
{
    if (g_MeterPowerOff)
    {
        return;
    }
        
    pDataTable->nFramCounter++;
	pDataTable->nFlashCounter++;
	AcCalcRead7022();
    AcCalcBaseReal();
	AcCalcEnergy();
    AcCalcEventProc();
	AcCalaClrProc();
    OpenMeterCover();
    SecretKeyEvent();
    ClockTrbEvent();
    TripEvent();
    SwitchOnEvent();
    LoadSwitchActEvent();
    PowerAbnlEvent();
    ChipTrbEvent();
    OverCurrentEvent();

}

void AcCalcJustData1(void)
{
    //INT32U	nTmpBuff[128];
    MEM_AC_CAL_PRM  sJbParam;
    
    memset(&sJbParam, 0, sizeof(MEM_AC_CAL_PRM));

    sJbParam.nModeCfg = 47614; 
    
    sJbParam.nEMUCfg = 12740;  //12676
    
    sJbParam.nPgain[0] = 7521;
    sJbParam.nPgain[1] = 7594;
    sJbParam.nPgain[2] = 7392;
    
    sJbParam.nQgain[0] = 7521;
    sJbParam.nQgain[1] = 7594;
    sJbParam.nQgain[2] = 7392;
    
    sJbParam.nSgain[0] = 7521;
    sJbParam.nSgain[1] = 7594;
    sJbParam.nSgain[2] = 7392;
    
    sJbParam.nPhSregpq0[0] = 254;
    sJbParam.nPhSregpq0[1] = 271;
    sJbParam.nPhSregpq0[2] = 0;
    
    sJbParam.nPhSregpq1[0] = 254;
    sJbParam.nPhSregpq1[1] = 271;
    sJbParam.nPhSregpq1[2] = 0;
    
    sJbParam.nUgain[0] = 65435;
    sJbParam.nUgain[1] = 65524;
    sJbParam.nUgain[2] = 65414;
    
    sJbParam.nIgain[0] = 1486;
    sJbParam.nIgain[1] = 1454;
    sJbParam.nIgain[2] = 1399;
    
    sJbParam.nIstarup = 393;
    sJbParam.nHfconst = 182;
    sJbParam.nFailVoltage = 1536;
    
    sJbParam.nEMUIE = 1;
    sJbParam.nModuleCFG = 13366;
    
    sJbParam.nLineGain = 11353;
    sJbParam.nPineCtrl = 15;
    
    sJbParam.nPstart = 48;
    sJbParam.nIregion = 32767;
    
    sJbParam.nReserved[0] = 4096;
    sJbParam.nReserved[1] = 17664;
    
    //sJbParam.nCheckSumEntire = 411854;
    sJbParam.nCheckSumEntire = CheckINT32USum((INT32U*)(&sJbParam), 0, sizeof(MEM_AC_CAL_PRM)/4 - 3);
    sJbParam.nCheckSumReset = 120013;
    
    MemWriteAcCalPrm(FEND, &sJbParam, sizeof(sJbParam));  
    
}

void AcCalcJustData2(void)
{
     //INT32U	nTmpBuff[128];
    MEM_AC_CAL_PRM  sJbParam;
    
    memset(&sJbParam, 0, sizeof(MEM_AC_CAL_PRM));

    sJbParam.nModeCfg = 47614; 
    sJbParam.nEMUCfg = 12740;
    
    sJbParam.nPgain[0] = 8838;
    
    sJbParam.nQgain[0] = 8838;

    sJbParam.nSgain[0] = 8838;
  
    sJbParam.nPhSregpq0[0] = 65118;
    
    sJbParam.nPhSregpq1[0] = 65118;
    
    sJbParam.nUgain[0] = 392;
    
    sJbParam.nIgain[0] = 1571;
    
    sJbParam.nIstarup = 393;
    sJbParam.nHfconst = 147;
    sJbParam.nFailVoltage = 1536;
    
    sJbParam.nEMUIE = 1;
    sJbParam.nModuleCFG = 13366;
    
    sJbParam.nLineGain = 11353;
    sJbParam.nPineCtrl = 15;
    
    sJbParam.nPstart = 48;
    sJbParam.nIregion = 32767;
    
    sJbParam.nReserved[0] = 4096;
    sJbParam.nReserved[1] = 17664;
    
    //sJbParam.nCheckSumEntire = 411854;
    sJbParam.nCheckSumEntire = CheckINT32USum((INT32U*)(&sJbParam), 0, sizeof(MEM_AC_CAL_PRM)/4 - 3);
    sJbParam.nCheckSumReset = 120013;
    
    MemWriteAcCalPrm(FEND, &sJbParam, sizeof(sJbParam));
}

void AcCalcJustData3(void)
{
    //INT32U	nTmpBuff[128];
    MEM_AC_CAL_PRM  sJbParam;
    
    memset(&sJbParam, 0, sizeof(MEM_AC_CAL_PRM));

    sJbParam.nModeCfg = 47614; 
    sJbParam.nEMUCfg = 12740;
    
    sJbParam.nPgain[0] = 7211;
    sJbParam.nPgain[1] = 7634;
    sJbParam.nPgain[2] = 8178;
    
    sJbParam.nQgain[0] = 7211;
    sJbParam.nQgain[1] = 7634;
    sJbParam.nQgain[2] = 8178;
    
    sJbParam.nSgain[0] = 7211;
    sJbParam.nSgain[1] = 7634;
    sJbParam.nSgain[2] = 8178;
    
    sJbParam.nPhSregpq0[0] = 233;
    sJbParam.nPhSregpq0[1] = 212;
    sJbParam.nPhSregpq0[2] = 296;
    
    sJbParam.nPhSregpq1[0] = 233;
    sJbParam.nPhSregpq1[1] = 212;
    sJbParam.nPhSregpq1[2] = 296;
    
    sJbParam.nUgain[0] = 65054;
    sJbParam.nUgain[1] = 65422;
    sJbParam.nUgain[2] = 287;
    
    sJbParam.nIgain[0] = 1435;
    sJbParam.nIgain[1] = 1407;
    sJbParam.nIgain[2] = 1447;
    
    sJbParam.nIstarup = 393;
    sJbParam.nHfconst = 874;
    sJbParam.nFailVoltage = 1536;
    
    sJbParam.nEMUIE = 1;
    sJbParam.nModuleCFG = 13366;
    
    sJbParam.nLineGain = 11353;
    sJbParam.nPineCtrl = 15;
    
    sJbParam.nPstart = 48;
    sJbParam.nIregion = 32767;
    
    sJbParam.nReserved[0] = 4096;
    sJbParam.nReserved[1] = 17664;
    
    //sJbParam.nCheckSumEntire = 411854;
    sJbParam.nCheckSumEntire = CheckINT32USum((INT32U*)(&sJbParam), 0, sizeof(MEM_AC_CAL_PRM)/4 - 3);
    sJbParam.nCheckSumReset = 120013;
    
    MemWriteAcCalPrm(FEND, &sJbParam, sizeof(sJbParam));  
    
}


void AcCalcJustData4(void)
{
    //INT32U	nTmpBuff[128];
    MEM_AC_CAL_PRM  sJbParam;
    
    memset(&sJbParam, 0, sizeof(MEM_AC_CAL_PRM));

    sJbParam.nModeCfg = 47614; 
    sJbParam.nEMUCfg = 12740;
    
    sJbParam.nPgain[0] = 7912;
    sJbParam.nPgain[1] = 8364;
    sJbParam.nPgain[2] = 7961;
    
    sJbParam.nQgain[0] = 7912;
    sJbParam.nQgain[1] = 8364;
    sJbParam.nQgain[2] = 7961;
    
    sJbParam.nSgain[0] = 7912;
    sJbParam.nSgain[1] = 8364;
    sJbParam.nSgain[2] = 7961;
    
    sJbParam.nPhSregpq0[0] = 141;
    sJbParam.nPhSregpq0[1] = 178;
    sJbParam.nPhSregpq0[2] = 176;
    
    sJbParam.nPhSregpq1[0] = 141;
    sJbParam.nPhSregpq1[1] = 178;
    sJbParam.nPhSregpq1[2] = 176;
    
    sJbParam.nUgain[0] = 244;
    sJbParam.nUgain[1] = 782;
    sJbParam.nUgain[2] = 215;
    
    sJbParam.nIgain[0] = 1598;
    sJbParam.nIgain[1] = 1423;
    sJbParam.nIgain[2] = 1671;
    
    sJbParam.nIstarup = 393;
    sJbParam.nHfconst = 58;
    sJbParam.nFailVoltage = 1536;
    
    sJbParam.nEMUIE = 1;
    sJbParam.nModuleCFG = 13366;
    
    sJbParam.nLineGain = 11353;
    sJbParam.nPineCtrl = 15;
    
    sJbParam.nPstart = 48;
    sJbParam.nIregion = 32767;
    
    sJbParam.nReserved[0] = 4096;
    sJbParam.nReserved[1] = 17664;
    
    //sJbParam.nCheckSumEntire = 411854;
    sJbParam.nCheckSumEntire = CheckINT32USum((INT32U*)(&sJbParam), 0, sizeof(MEM_AC_CAL_PRM)/4 - 3);
    sJbParam.nCheckSumReset = 120013;
    
    MemWriteAcCalPrm(FEND, &sJbParam, sizeof(sJbParam));  
    
}

void AcCalcJustData5(void)
{
    //INT32U	nTmpBuff[128];
    MEM_AC_CAL_PRM  sJbParam;
    
    memset(&sJbParam, 0, sizeof(MEM_AC_CAL_PRM));

    sJbParam.nModeCfg = 47614; 
    sJbParam.nEMUCfg = 12676;
    
    sJbParam.nPgain[0] = 26832;
    sJbParam.nPgain[1] = 0;
    sJbParam.nPgain[2] = 28221;
    
    sJbParam.nQgain[0] = 26832;
    sJbParam.nQgain[1] = 0;
    sJbParam.nQgain[2] = 28221;
    
    sJbParam.nSgain[0] = 26832;
    sJbParam.nSgain[1] = 0;
    sJbParam.nSgain[2] = 28221;
    
    sJbParam.nPhSregpq0[0] = 459;
    sJbParam.nPhSregpq0[1] = 0;
    sJbParam.nPhSregpq0[2] = 627;
    
    sJbParam.nPhSregpq1[0] = 459;
    sJbParam.nPhSregpq1[1] = 0;
    sJbParam.nPhSregpq1[2] = 627;
    
    sJbParam.nUgain[0] = 227;
    sJbParam.nUgain[1] = 0;
    sJbParam.nUgain[2] = 996;
    
    sJbParam.nIgain[0] = 1602;
    sJbParam.nIgain[1] = 0;
    sJbParam.nIgain[2] = 1601;
    
    sJbParam.nIstarup = 393;
    sJbParam.nHfconst = 82;
    sJbParam.nFailVoltage = 4608;
    
    sJbParam.nEMUIE = 1;
    sJbParam.nModuleCFG = 13366;
    
    sJbParam.nLineGain = 11353;
    sJbParam.nPineCtrl = 15;
    
    sJbParam.nPstart = 48;
    sJbParam.nIregion = 32767;
    
    sJbParam.nReserved[0] = 4096;
    sJbParam.nReserved[1] = 17664;
    
    //sJbParam.nCheckSumEntire = 411854;
    sJbParam.nCheckSumEntire = CheckINT32USum((INT32U*)(&sJbParam), 0, sizeof(MEM_AC_CAL_PRM)/4 - 3);
    sJbParam.nCheckSumReset = 123085;
    
    MemWriteAcCalPrm(FEND, &sJbParam, sizeof(sJbParam));  
    
}

void AcCalcJustData6(void)
{
    //INT32U	nTmpBuff[128];
    MEM_AC_CAL_PRM  sJbParam;
    
    memset(&sJbParam, 0, sizeof(MEM_AC_CAL_PRM));

    sJbParam.nModeCfg = 47614; 
    sJbParam.nEMUCfg = 12676;
    
    sJbParam.nPgain[0] = 9166;
    sJbParam.nPgain[1] = 0;
    sJbParam.nPgain[2] = 8613;
    
    sJbParam.nQgain[0] = 9166;
    sJbParam.nQgain[1] = 0;
    sJbParam.nQgain[2] = 8613;
    
    sJbParam.nSgain[0] = 9166;
    sJbParam.nSgain[1] = 0;
    sJbParam.nSgain[2] = 8613;
    
    sJbParam.nPhSregpq0[0] = 490;
    sJbParam.nPhSregpq0[1] = 0;
    sJbParam.nPhSregpq0[2] = 537;
    
    sJbParam.nPhSregpq1[0] = 490;
    sJbParam.nPhSregpq1[1] = 0;
    sJbParam.nPhSregpq1[2] = 537;
    
    sJbParam.nUgain[0] = 1067;
    sJbParam.nUgain[1] = 0;
    sJbParam.nUgain[2] = 546;
    
    sJbParam.nIgain[0] = 1650;
    sJbParam.nIgain[1] = 0;
    sJbParam.nIgain[2] = 1726;
    
    sJbParam.nIstarup = 393;
    sJbParam.nHfconst = 221; //401
    sJbParam.nFailVoltage = 4608;
    
    sJbParam.nEMUIE = 1;
    sJbParam.nModuleCFG = 13366;
    
    sJbParam.nLineGain = 11353;
    sJbParam.nPineCtrl = 15;
    
    sJbParam.nPstart = 48;
    sJbParam.nIregion = 32767;
    
    sJbParam.nReserved[0] = 4096;
    sJbParam.nReserved[1] = 17664;
    
    //sJbParam.nCheckSumEntire = 411854;
    sJbParam.nCheckSumEntire = CheckINT32USum((INT32U*)(&sJbParam), 0, sizeof(MEM_AC_CAL_PRM)/4 - 3);
    sJbParam.nCheckSumReset = 123085;
    
    MemWriteAcCalPrm(FEND, &sJbParam, sizeof(sJbParam));  
    
}


/*
*****************************************************************************************************
* 函数名:AcCalcRead7022
* 输　入:
* 输  出:
* 返  回:
* 功能描述:从7022读入数据，并转换成所需要的格式
*****************************************************************************************************
*/
void AcCalcRead7022(void)
{
    INT8U	ii,jj;
	INT8U	nIndex = 0;
	INT8U	nErr = 0;
	INT32U	nTmpBuff[128];
	INT32U	nTmp;
	FP32	fData = 0.0;// fData1 = 0.0;
	INT8U	nUStatus[3] = {0,0,0};
    INT8U	nIStatus[4] = {0,0,0,0};
	FP32	fYab = 0.0, fYac = 0.0;// fS = 0.0;
	FP32	fPga = 0.0, fPgb = 0.0, fPgc = 0.0;
    FP32    fG[3]={1.00,1.00,1.00},fB[3]={1.00,1.00,1.00},fR[3]={1.00,1.00,1.00},fX[3]={1.00,1.00,1.00};
	INT32U  nChkSum = 0;
    MEM_AC_CAL_PRM  sJbParam;
    
    OSSemPend(g_pSem[SEM_AC_METER_CALC_IDX], 0, &nErr);
    memset(nTmpBuff, 0, sizeof(nTmpBuff));
    OSSemPend(g_pSem[SEM_ACREAD_CTRL_IDX], 0, &nErr);
	nChkSum = ATT7022RegRead(r_ChkSum);
	OSSemPost(g_pSem[SEM_ACREAD_CTRL_IDX]);
    
    
    if((nChkSum & 0x00FFFFFF) != pDataTable->nCheckSumEntire)
    {
        memset(&sJbParam, 0, sizeof(MEM_AC_CAL_PRM));
        MemReadAcCalPrm(FEND, &sJbParam, sizeof(sJbParam)); 
        
        /*if (IsAllEqualVal((INT8U*)&sJbParam.nUgain[0], 4, 0))
        {
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
           else if (TRIPHASE_METER_57U == g_nHardType)
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
        }*/

        g_bAcInitFail = FALSE;
        while (RET_ERR_OK != AcCalcJust7022(&sJbParam))
        {
            OSSemPend(g_pSem[SEM_COL_AC_CALC_IDX], 0, &nErr);
            //存在问题，无法释放SEM_AC_METER_CALC_IDX
			ATT7022Init();
			ATT7022Reset(100);
        }
        ReadAcPluPrm();
		g_bAcInitFail = TRUE;
    }
    OSSemPost(g_pSem[SEM_AC_METER_CALC_IDX]);
    AcCalDataInit();
    if ((pDataTable->nPulseConst !=pSavDataTable->nPulseConst) || (0 == pDataTable->nPulseConst)
		|| (pDataTable->nHFConst !=pSavDataTable->nHFConst) || (0 == pDataTable->nHFConst))  //如果临时区脉冲常数出错则重新读取脉冲常数
	{
		if (RET_ERR_OK != ReadAcPluPrm())
		{
			
		}
	}
    
    if ((pDataTable->nCombEp != pSavDataTable->nCombEp) || (pDataTable->nCombEq1 != pSavDataTable->nCombEq1)
        || (pDataTable->nCombEq2 != pSavDataTable->nCombEq2))
    {
        if (RET_ERR_OK != ReadAcCombPrm())
		{
			
		}
    }
    
    OSSemPend(g_pSem[SEM_ACREAD_CTRL_IDX], 0, &nErr);
    {
        if(g_nSetCtrlcurFlg)
        {
            g_pflagcount++;
            if(g_pflagcount > 70)
            {
              g_pflagcount = 0;
            }
            if((SINGLEPHASE_METER == g_nHardType))
            {
                nTmpBuff[nIndex++] = ATT7022RegRead(r_UaRms);	//A相电压
                nTmpBuff[nIndex++] = ATT7022RegRead(r_UbRms);	//B相电压
                nTmpBuff[nIndex++] = ATT7022RegRead(r_UcRms);	//C相电压
                nTmpBuff[nIndex++] = 147757;	//A相电流
                nTmpBuff[nIndex++] = 441;	//B相电流
                nTmpBuff[nIndex++] = 441;	//C相电流
                nTmpBuff[nIndex++] = 73641;	//零序电流

                nTmpBuff[nIndex++] = 16785;		//A相有功功率
                nTmpBuff[nIndex++] = 16777215;		//B相有功功率
                nTmpBuff[nIndex++] = 0;		//C相有功功率
                nTmpBuff[nIndex++] = 8392;		//合相有功功率
                nTmpBuff[nIndex++] = 16748023;		//A相无功功率
                nTmpBuff[nIndex++] = 16777215;		//B相无功功率
                nTmpBuff[nIndex++] = 16777215;		//C相无功功率
                nTmpBuff[nIndex++] = 16762619;		//合相无功功率
                nTmpBuff[nIndex++] = 33674;		//A相视在功率
                nTmpBuff[nIndex++] = 0;		//B相视在功率
                nTmpBuff[nIndex++] = 0;		//C相视在功率
                nTmpBuff[nIndex++] = 16836;		//合相视在功率
                nTmpBuff[nIndex++] = 4181350;		//A相功率因数
                nTmpBuff[nIndex++] = 8388608;		//A相功率因数
                nTmpBuff[nIndex++] = 8388608;		//A相功率因数
                nTmpBuff[nIndex++] = 4181349;		//合相功率因数

                nTmpBuff[nIndex++] = 242;	//pflag
            
                nTmpBuff[nIndex++] = 1;		//A有功电能
                nTmpBuff[nIndex++] = 0;		//B有功电能
                nTmpBuff[nIndex++] = 0;		//C有功电能
                if(g_pflagcount<61)
                {
                   nTmpBuff[nIndex++] = 0;		//合相有功电能       
                }
                else
                {
                    nTmpBuff[nIndex++] = 1;		//合相有功电能   
                }
                nTmpBuff[nIndex++] = 0;		//A无功电能
                nTmpBuff[nIndex++] = 0;		//B无功电能
                nTmpBuff[nIndex++] = 0;		//C无功电能
                nTmpBuff[nIndex++] = 0;		//合相无功电能
            
                nTmpBuff[nIndex++] = 30;	//sflag

                nTmpBuff[nIndex++] = 1572751;	//Ua与Ub的电压夹角
                nTmpBuff[nIndex++] = 1572751;	//Ua与Uc的电压夹角
                nTmpBuff[nIndex++] = 16423062;		//A相电流与电压相角
                nTmpBuff[nIndex++] = 0;		//B相电流与电压相角
                nTmpBuff[nIndex++] = 0;		//C相电流与电压相角
                nTmpBuff[nIndex++] = 255;   //温度传感器
        
            }
            else if(TRIPHASE_METER_100 == g_nHardType)
            {
                nTmpBuff[nIndex++] = ATT7022RegRead(r_UaRms);	//A相电压
                nTmpBuff[nIndex++] = ATT7022RegRead(r_UbRms);	//B相电压
                nTmpBuff[nIndex++] = ATT7022RegRead(r_UcRms);	//C相电压
                nTmpBuff[nIndex++] = 982523;	//A相电流
                nTmpBuff[nIndex++] = 441;	//B相电流
                nTmpBuff[nIndex++] = 996009;	//C相电流
                nTmpBuff[nIndex++] = 501483;	//零序电流

                nTmpBuff[nIndex++] = 399079;		//A相有功功率
                nTmpBuff[nIndex++] = 16777215;		//B相有功功率
                nTmpBuff[nIndex++] = 393461;		//C相有功功率
                nTmpBuff[nIndex++] = 396270;		//合相有功功率
                nTmpBuff[nIndex++] = 215220;		//A相无功功率
                nTmpBuff[nIndex++] = 16777215;		//B相无功功率
                nTmpBuff[nIndex++] = 16542370;		//C相无功功率
                nTmpBuff[nIndex++] = 16767403;		//合相无功功率
                nTmpBuff[nIndex++] = 453415;		//A相视在功率
                nTmpBuff[nIndex++] = 0;		//B相视在功率
                nTmpBuff[nIndex++] = 458222;		//C相视在功率
                nTmpBuff[nIndex++] = 396391;		//合相视在功率
                nTmpBuff[nIndex++] = 7383340;		//A相功率因数
                nTmpBuff[nIndex++] = 7383340;		//A相功率因数
                nTmpBuff[nIndex++] = 7203037;		//A相功率因数
                nTmpBuff[nIndex++] = 8386047;		//合相功率因数

                nTmpBuff[nIndex++] = 226;	//pflag
            
                nTmpBuff[nIndex++] = 1;		//A有功电能
                nTmpBuff[nIndex++] = 0;		//B有功电能
                nTmpBuff[nIndex++] = 1;		//C有功电能
                nTmpBuff[nIndex++] = 2;		//合相有功电能
                nTmpBuff[nIndex++] = 1;		//A无功电能
                nTmpBuff[nIndex++] = 0;		//B无功电能
                nTmpBuff[nIndex++] = 0;		//C无功电能
                nTmpBuff[nIndex++] = 0;		//合相无功电能
            
                nTmpBuff[nIndex++] = 8258;	//sflag

                nTmpBuff[nIndex++] = 1572793;	//Ua与Ub的电压夹角
                nTmpBuff[nIndex++] = 1750843;	//Ua与Uc的电压夹角
                nTmpBuff[nIndex++] = 162481;		//A相电流与电压相角
                nTmpBuff[nIndex++] = 0;		//B相电流与电压相角
                nTmpBuff[nIndex++] = 16594214;		//C相电流与电压相角
                nTmpBuff[nIndex++] = 254;   //温度传感器
            }
            else //if(TRIPHASE_METER_220 == g_nHardType)
            {
                nTmpBuff[nIndex++] = ATT7022RegRead(r_UaRms);	//A相电压
                nTmpBuff[nIndex++] = ATT7022RegRead(r_UbRms);	//B相电压
                nTmpBuff[nIndex++] = ATT7022RegRead(r_UcRms);	//C相电压
                nTmpBuff[nIndex++] = 974548;	//A相电流
                nTmpBuff[nIndex++] = 1011480;	//B相电流
                nTmpBuff[nIndex++] = 967723;	//C相电流
                nTmpBuff[nIndex++] = 27834;	//零序电流

                nTmpBuff[nIndex++] = 250309;		//A相有功功率
                nTmpBuff[nIndex++] = 259799;		//B相有功功率
                nTmpBuff[nIndex++] = 247624;		//C相有功功率
                nTmpBuff[nIndex++] = 378866;		//合相有功功率
                nTmpBuff[nIndex++] = 16772431;		//A相无功功率
                nTmpBuff[nIndex++] = 16773778;		//B相无功功率
                nTmpBuff[nIndex++] = 16772633;		//C相无功功率
                nTmpBuff[nIndex++] = 16770814;		//合相无功功率
                nTmpBuff[nIndex++] = 250354;		//A相视在功率
                nTmpBuff[nIndex++] = 259821;		//B相视在功率
                nTmpBuff[nIndex++] = 247665;		//C相视在功率
                nTmpBuff[nIndex++] = 378920;		//合相视在功率
                nTmpBuff[nIndex++] = 8387100;		//A相功率因数
                nTmpBuff[nIndex++] = 8387897;		//A相功率因数
                nTmpBuff[nIndex++] = 8387219;		//A相功率因数
                nTmpBuff[nIndex++] = 8387412;		//合相功率因数

                nTmpBuff[nIndex++] = 240;	//pflag
            
                nTmpBuff[nIndex++] = 1;		//A有功电能
                nTmpBuff[nIndex++] = 2;		//B有功电能
                nTmpBuff[nIndex++] = 1;		//C有功电能
                nTmpBuff[nIndex++] = 5;		//合相有功电能
                nTmpBuff[nIndex++] = 0;		//A无功电能
                nTmpBuff[nIndex++] = 0;		//B无功电能
                nTmpBuff[nIndex++] = 0;		//C无功电能
                nTmpBuff[nIndex++] = 0;		//合相无功电能
            
                nTmpBuff[nIndex++] = 8256;	//sflag

                nTmpBuff[nIndex++] = 701080;	//Ua与Ub的电压夹角
                nTmpBuff[nIndex++] = 1404553;	//Ua与Uc的电压夹角
                nTmpBuff[nIndex++] = 16766402;		//A相电流与电压相角
                nTmpBuff[nIndex++] = 16768053;		//B相电流与电压相角
                nTmpBuff[nIndex++] = 16767512;		//C相电流与电压相角
                nTmpBuff[nIndex++] = 250;   //温度传感器
            }
        }
        else
        {
            nTmpBuff[nIndex++] = ATT7022RegRead(r_UaRms);	//A相电压
            nTmpBuff[nIndex++] = ATT7022RegRead(r_UbRms);	//B相电压
            nTmpBuff[nIndex++] = ATT7022RegRead(r_UcRms);	//C相电压
            nTmpBuff[nIndex++] = ATT7022RegRead(r_IaRms)*((1-nZFWuCh)+0.1*nWuCh);	//A相电流
            nTmpBuff[nIndex++] = ATT7022RegRead(r_IbRms)*((1-nZFWuCh)+0.1*nWuCh);	//B相电流
            nTmpBuff[nIndex++] = ATT7022RegRead(r_IcRms)*((1-nZFWuCh)+0.1*nWuCh);	//C相电流
            nTmpBuff[nIndex++] = ATT7022RegRead(r_ItRms);	//零序电流

            nTmpBuff[nIndex++] = ATT7022RegRead(r_Pa)*((1-nZFWuCh)+0.1*nWuCh);		//A相有功功率
            nTmpBuff[nIndex++] = ATT7022RegRead(r_Pb)*((1-nZFWuCh)+0.1*nWuCh);		//B相有功功率
            nTmpBuff[nIndex++] = ATT7022RegRead(r_Pc)*((1-nZFWuCh)+0.1*nWuCh);		//C相有功功率
            nTmpBuff[nIndex++] = ATT7022RegRead(r_Pt)*((1-nZFWuCh)+0.1*nWuCh);		//合相有功功率
            nTmpBuff[nIndex++] = ATT7022RegRead(r_Qa);		//A相无功功率
            nTmpBuff[nIndex++] = ATT7022RegRead(r_Qb);		//B相无功功率
            nTmpBuff[nIndex++] = ATT7022RegRead(r_Qc);		//C相无功功率
            nTmpBuff[nIndex++] = ATT7022RegRead(r_Qt);		//合相无功功率
            nTmpBuff[nIndex++] = ATT7022RegRead(r_Sa);		//A相视在功率
            nTmpBuff[nIndex++] = ATT7022RegRead(r_Sb);		//B相视在功率
            nTmpBuff[nIndex++] = ATT7022RegRead(r_Sc);		//C相视在功率
            nTmpBuff[nIndex++] = ATT7022RegRead(r_St);		//合相视在功率
            nTmpBuff[nIndex++] = ATT7022RegRead(r_Pfa);		//A相功率因数
            nTmpBuff[nIndex++] = ATT7022RegRead(r_Pfb);		//A相功率因数
            nTmpBuff[nIndex++] = ATT7022RegRead(r_Pfc);		//A相功率因数
            nTmpBuff[nIndex++] = ATT7022RegRead(r_Pft);		//合相功率因数

            nTmpBuff[nIndex++] = ATT7022RegRead(r_PFlag);	//pflag
        
            nTmpBuff[nIndex++] = ATT7022RegRead(r_Epa);		//A有功电能
            nTmpBuff[nIndex++] = ATT7022RegRead(r_Epb);		//B有功电能
            nTmpBuff[nIndex++] = ATT7022RegRead(r_Epc);		//C有功电能
            nTmpBuff[nIndex++] = ATT7022RegRead(r_Ept);		//合相有功电能
            nTmpBuff[nIndex++] = ATT7022RegRead(r_Eqa);		//A无功电能
            nTmpBuff[nIndex++] = ATT7022RegRead(r_Eqb);		//B无功电能
            nTmpBuff[nIndex++] = ATT7022RegRead(r_Eqc);		//C无功电能
            nTmpBuff[nIndex++] = ATT7022RegRead(r_Eqt);		//合相无功电能
        
            nTmpBuff[nIndex++] = ATT7022RegRead(r_Sflag);	//sflag

            nTmpBuff[nIndex++] = ATT7022RegRead(r_YUaUb);	//Ua与Ub的电压夹角
            nTmpBuff[nIndex++] = ATT7022RegRead(r_YUaUc);	//Ua与Uc的电压夹角
            nTmpBuff[nIndex++] = ATT7022RegRead(r_Pga);		//A相电流与电压相角
            nTmpBuff[nIndex++] = ATT7022RegRead(r_Pgb);		//B相电流与电压相角
            nTmpBuff[nIndex++] = ATT7022RegRead(r_Pgc);		//C相电流与电压相角
            nTmpBuff[nIndex++] = ATT7022RegRead(r_TempD);   //温度传感器
        }
    }
    OSSemPost(g_pSem[SEM_ACREAD_CTRL_IDX]);
    
    nIndex = 0;
	//三相电压
    for (ii = 0; ii < 3; ii++)
    {
        fData = AcCalcReadValid(pDataTable->nPulseConst, pDataTable->nHFConst, r_UaRms+ii, nTmpBuff[nIndex++]);
        pDataTable->fFeAPower[ii+1] = fG[ii]*fData*fData;
        pDataTable->fFeRPower[ii+1] = fB[ii]*fData*fData;
        nTmp = (INT32U) (10*fData+0.5);
       
        if((fData)<6)
	    {
		    nTmp = 0;
            nUStatus[ii]=0x01;
	    } 
        pDataTable->fVolt[ii] = nTmp;
        pDataTable->sVoltSlip[ii].nVal[pDataTable->sVoltSlip[ii].nSlips++] = nTmp;
        INT32U2BCD(nTmp,4,&pDataTable->sRealInstant.sRealVolt[ii].nFrac);
    }
    pDataTable->fFeAPower[0] = pDataTable->fFeAPower[1] + pDataTable->fFeAPower[2] + pDataTable->fFeAPower[3];
    pDataTable->fFeRPower[0] = pDataTable->fFeRPower[1] + pDataTable->fFeRPower[2] + pDataTable->fFeRPower[3];
    
    if ((g_ACOffset<80) || (g_ACOffset>120))
    {
        g_ACOffset = 100;
    }
    
    //电流
    for(ii=0;ii<4;ii++)
    {
        /*if(g_nSetCtrlcurFlg)
        {
            //fData = 1.22435302E+2;//设置此数值会使电能表飞走
        }*/
        
        fData = AcCalcReadValid(pDataTable->nPulseConst, pDataTable->nHFConst, r_IaRms+ii, nTmpBuff[nIndex++]);
        
        if(ii==3)
	    {
             fData/=20.0;
	    }
        else
	    {
	         fData/=40.0;
             //fData/=6.0;
             pDataTable->fCuAPower[ii+1] = fR[ii]*fData*fData;
             pDataTable->fCuRPower[ii+1] = fX[ii]*fData*fData;
	    }
        nTmp = (INT32U) (1000*fData+0.5);
        if((fData)<0.003)
	    {
		    nTmp = 0;
            nIStatus[ii]=0x01;
	    }
        
        nTmp *= g_ACOffset/100.0;
        
        pDataTable->fCurr[ii] = nTmp;
        pDataTable->sCurrSlip[ii].nVal[pDataTable->sCurrSlip[ii].nSlips++] = nTmp;
        INT32U2BCD(nTmp,6,&pDataTable->sRealInstant.sRealCurr[ii].nD1);
    }
    
    pDataTable->fCuAPower[0] = pDataTable->fCuAPower[1] + pDataTable->fCuAPower[2] + pDataTable->fCuAPower[3];
    pDataTable->fCuRPower[0] = pDataTable->fCuRPower[1] + pDataTable->fCuRPower[2] + pDataTable->fCuRPower[3];
	
    //功率
    for(ii=0;ii<4;ii++)
	{
		fData = AcCalcReadValid(pDataTable->nPulseConst, pDataTable->nHFConst, r_Pa+ii, nTmpBuff[nIndex++]);
		if(ii == 3)
		{
			
            nTmp =  (INT32S)(fData*10000);
            
            nTmp *= g_ACOffset/100.0;
            
            pDataTable->sPSlip[0].nVal[pDataTable->sPSlip[0].nSlips++] = nTmp;
            INT32U2BCD(abs(nTmp),6,&pDataTable->sRealInstant.sRealP[0].nFrac0);
            if ((fData < 0)&&(nTmp!=0))
            {
                pDataTable->sRealInstant.sRealP[0].nSint |= 0x80;
            }
            else
            {
                pDataTable->sRealInstant.sRealP[0].nSint &= ~0x80;
            }
		}
		else
		{
			nTmp = (INT32S)(fData*10000) + 1;
            if (nTmp < 5)
            {
               nTmp = 0;
            }
            
            nTmp *= g_ACOffset/100.0;
            
            pDataTable->sPSlip[ii+1].nVal[pDataTable->sPSlip[ii+1].nSlips++] = nTmp;
            INT32U2BCD(abs(nTmp),6,&pDataTable->sRealInstant.sRealP[ii+1].nFrac0);
            if ((fData < 0)&&(nTmp!=0))
            {

                pDataTable->sRealInstant.sRealP[ii+1].nSint |= 0x80;
                if(nIStatus[ii] == 0x00)
                {
                   pDataTable->sRealInstant.sRealCurr[ii].nD3 |= 0x80;
                }

            }
            else
            {
                pDataTable->sRealInstant.sRealP[ii+1].nSint &= ~0x80;
                pDataTable->sRealInstant.sRealCurr[ii].nD3 &= ~0x80;
            }
		}
	}
    //功率因数
    for(ii=0;ii<4;ii++)
	{
		fData = AcCalcReadValid(pDataTable->nPulseConst, pDataTable->nHFConst, r_Qa+ii, nTmpBuff[nIndex++]);
		if(ii == 3)
		{
			nTmp = (INT32S)(fData*10000);
            pDataTable->sQSlip[0].nVal[pDataTable->sQSlip[0].nSlips++] = nTmp;
            INT32U2BCD(abs(nTmp),6,&pDataTable->sRealInstant.sRealQ[0].nFrac0);
            if ((fData < 0)&&(nTmp!=0))
            {
                pDataTable->sRealInstant.sRealQ[0].nSint |= 0x80;
            }
            else
            {
                pDataTable->sRealInstant.sRealQ[0].nSint &= ~0x80;
            }
		}
		else
		{
			
            nTmp =  (INT32S)(fData*10000);
            pDataTable->sQSlip[ii+1].nVal[pDataTable->sQSlip[ii+1].nSlips++] = nTmp;
            INT32U2BCD(abs(nTmp),6,&pDataTable->sRealInstant.sRealQ[ii+1].nFrac0);
            if ((fData < 0)&&(nTmp!=0))
            {
                pDataTable->sRealInstant.sRealQ[ii+1].nSint |= 0x80;
            }
            else
            {
                pDataTable->sRealInstant.sRealQ[ii+1].nSint &= ~0x80;
            }
		}
	}
    
    for(ii=0;ii<4;ii++)
	{
		fData = AcCalcReadValid(pDataTable->nPulseConst, pDataTable->nHFConst, r_Sa+ii, nTmpBuff[nIndex++]);
		if(ii == 3)
		{
			nTmp =  (INT32S)(fData*10000);
            pDataTable->sSSlip[0].nVal[pDataTable->sSSlip[0].nSlips++] = nTmp;
            INT32U2BCD(abs(nTmp),6,&pDataTable->sRealInstant.sRealS[0].nFrac0);
            if ((fData < 0)&&(nTmp!=0))
            {
                pDataTable->sRealInstant.sRealS[0].nSint |= 0x80;
		    }
		    else
		    {
                pDataTable->sRealInstant.sRealS[0].nSint &= ~0x80;
		    }
	    }
		else
		{
			nTmp =  (INT32S)(fData*10000);
            pDataTable->sSSlip[ii+1].nVal[pDataTable->sSSlip[ii+1].nSlips++] = nTmp;
            INT32U2BCD(abs(nTmp),6,&pDataTable->sRealInstant.sRealS[ii+1].nFrac0);
            if ((fData < 0)&&(nTmp!=0))
	        {
                pDataTable->sRealInstant.sRealS[ii+1].nSint |= 0x80;
	        }
	        else
	        {
                pDataTable->sRealInstant.sRealS[ii+1].nSint &= ~0x80;
	        }
        }
	}
    
    for(ii=0;ii<3;ii++)
	{
	    if((nUStatus[ii] == 1) || (nIStatus[ii] == 1))
	    {
		    fData = 1.0;
		    nIndex++;
	    }
	    else
	    {
		    fData = AcCalcReadValid(pDataTable->nPulseConst, pDataTable->nHFConst, r_Pfa+ii, nTmpBuff[nIndex++]);
	    }
        
        nTmp = (INT32S)(fData*1000);
        pDataTable->sPfSlip[ii+1].nVal[pDataTable->sPfSlip[ii+1].nSlips++] = nTmp;
        INT32U2BCD(abs(nTmp),4,&pDataTable->sRealInstant.sRealPf[ii+1].nFrac);
        if ((fData < 0)&&(nTmp!=0))
	    {
            pDataTable->sRealInstant.sRealPf[ii+1].nSint |= 0x80;
	    }
	    else
	    {
            pDataTable->sRealInstant.sRealPf[ii+1].nSint &= ~0x80;
        }
	}
	if (((nUStatus[0] == 1) || (nIStatus[0] == 1))
      &&((nUStatus[1] == 1) || (nIStatus[1] == 1))
      &&((nUStatus[2] == 1) || (nIStatus[2] == 1)))
	{
		fData = 1.0;
		nIndex++;
	}
	else
	{
		fData = AcCalcReadValid(pDataTable->nPulseConst, pDataTable->nHFConst, r_Pft, nTmpBuff[nIndex++]);
	}
    
    nTmp = (INT32S)(fData*1000);
    pDataTable->sPfSlip[0].nVal[pDataTable->sPfSlip[0].nSlips++] = nTmp;
    INT32U2BCD( abs(nTmp),4,&pDataTable->sRealInstant.sRealPf[0].nFrac);
    if ((fData < 0)&&(nTmp!=0))
    {
        pDataTable->sRealInstant.sRealPf[0].nSint |= 0x80;
    }
    else
    {
        pDataTable->sRealInstant.sRealPf[0].nSint &= ~0x80;
	}
	pDataTable->nPFlag = (INT8U)(nTmpBuff[nIndex++]);
    
    if (g_MeterAcErrFlg[1])//停走
    {
        return;
    }
    //有功电能
  	for(ii=0;ii<4;ii++)
	{
		fData = 0;

		fData = AcCalcReadValid(pDataTable->nPulseConst, pDataTable->nHFConst, r_Epa+ii, nTmpBuff[nIndex++]);
        
        //pDataTable->nPowerPulse[PosEp][ii] = 0;
        //pDataTable->nPowerPulse[NegEp][ii] = 0;
        //pDataTable->nPowerPulse1[PosEp][ii] = 0;
        //pDataTable->nPowerPulse1[NegEp][ii] = 0;
        pDataTable->nRealEnergy[NegEp][ii] = 0;
        pDataTable->nRealEnergy[PosEp][ii] = 0;
        
		if (fData < 500.0)
		{
            if(ii==3)
            {
                if(pDataTable->sDemandSlipData1[PosEp].nStep++ == 0)
                {
                    pDataTable->nPowerPulse[PosEp][ii] = 0;
                   
                }
                if(pDataTable->sDemandSlipData1[NegEp].nStep++ == 0)
                {
                    pDataTable->nPowerPulse[NegEp][ii] = 0;
                   
                }
                if(pDataTable->sDemandSlipData2[PosEp].nStep++ == 0)
                {
                    pDataTable->nPowerPulse1[PosEp][ii] = 0;
                  
                }
                if(pDataTable->sDemandSlipData2[NegEp].nStep++ == 0)
                {
                    pDataTable->nPowerPulse1[NegEp][ii] = 0;
                   
                }
            }
			if ((pDataTable->nPFlag & (0x01 << ii)) != 0)
			{
				pDataTable->nRealEnergy[NegEp][ii] = (INT16U)fData;
                pDataTable->nPowerPulse[NegEp][ii] += (INT16U)fData;
                pDataTable->nPowerPulse1[NegEp][ii] += (INT16U)fData;
			}
			else
			{
				pDataTable->nRealEnergy[PosEp][ii] = (INT16U)fData;
                pDataTable->nPowerPulse[PosEp][ii] += (INT16U)fData;
                pDataTable->nPowerPulse1[PosEp][ii] += (INT16U)fData;
			}
		}
	}

//无功电能
	for(ii=0;ii<4;ii++)
	{
		fData = 0;
		fData = AcCalcReadValid(pDataTable->nPulseConst, pDataTable->nHFConst, r_Eqa+ii, nTmpBuff[nIndex++]);
        
        pDataTable->nRealEnergy[2][ii] = 0;
        pDataTable->nRealEnergy[3][ii] = 0;
        pDataTable->nRealEnergy[4][ii] = 0;
        pDataTable->nRealEnergy[5][ii] = 0;
        
		if (fData < 500.0)
		{
            if(ii==3)
            {
                 for(jj=2;jj<8;jj++)
                 {
                      if(pDataTable->sDemandSlipData1[jj].nStep++ == 0)
                      {
                          pDataTable->nPowerPulse[jj][ii] = 0;
                        
                      }
                      if(pDataTable->sDemandSlipData2[jj].nStep++ == 0)
                      {
                          pDataTable->nPowerPulse1[jj][ii] = 0;
                         
                      }
                 }
            }

			if ((pDataTable->nPFlag & (0x01 << ii + 4)) != 0)
			{
                pDataTable->nPowerPulse[NegEq][ii] += (INT16U)fData;
                pDataTable->nPowerPulse1[NegEq][ii] += (INT16U)fData;
                if ((pDataTable->nPFlag & (0x01 << ii)) != 0)
                {
                    pDataTable->nRealEnergy[4][ii] = (INT16U)fData;
                    pDataTable->nPowerPulse[6][ii] += (INT16U)fData;
                    pDataTable->nPowerPulse1[6][ii] += (INT16U)fData;
                }
                else
                {
                    pDataTable->nRealEnergy[5][ii] = (INT16U)fData;
                    pDataTable->nPowerPulse[7][ii] += (INT16U)fData;
                    pDataTable->nPowerPulse1[7][ii] += (INT16U)fData;
                }
			}
			else
			{
                pDataTable->nPowerPulse[PosEq][ii] += (INT16U)fData;
                pDataTable->nPowerPulse1[PosEq][ii] += (INT16U)fData;
                if ((pDataTable->nPFlag & (0x01 << ii)) != 0)
                {
                    pDataTable->nRealEnergy[3][ii] = (INT16U)fData;
                    pDataTable->nPowerPulse[5][ii] += (INT16U)fData;
                    pDataTable->nPowerPulse1[5][ii] += (INT16U)fData;
                }
                else
                {
                    pDataTable->nRealEnergy[2][ii] = (INT16U)fData;
                    pDataTable->nPowerPulse[4][ii] += (INT16U)fData;
                    pDataTable->nPowerPulse1[4][ii] += (INT32U)fData;
                }
			}
		}
	}
	
	pDataTable->nPulsePCounter += (pDataTable->nRealEnergy[NegEp][3] + pDataTable->nRealEnergy[PosEp][3]);
	pDataTable->nPulseQCounter += (pDataTable->nRealEnergy[NegEq][3] + pDataTable->nRealEnergy[PosEq][3]);

	pDataTable->nSFlag = (INT8U)(nTmpBuff[nIndex++]);
    //电压相角
    nTmp = 0;
    INT32U2BCD(nTmp, 4, &pDataTable->sRealInstant.sRealY[0].nFrac);
	if (nUStatus[1] == 1)
	{
		fYab = 0.0;
		nIndex++;
	}
	else
	{
		fData = AcCalcReadValid(pDataTable->nPulseConst, pDataTable->nHFConst, r_YUaUb, nTmpBuff[nIndex++]);
        fYab = 360.0 - fData;
	}
    nTmp = (INT32U)(fYab * 10);
    INT32U2BCD(nTmp, 4, &pDataTable->sRealInstant.sRealY[1].nFrac);
	if ((nUStatus[2] == 1))
	{
		fYac = 0.0;
		nIndex++;
	}
	else
	{
		fData = AcCalcReadValid(pDataTable->nPulseConst, pDataTable->nHFConst, r_YUaUc, nTmpBuff[nIndex++]);
        fYac = 360.0 - fData;
	}
    nTmp = (INT32U)(fYac * 10);
    INT32U2BCD(nTmp, 4, &pDataTable->sRealInstant.sRealY[2].nFrac);
    //电流相角
	nTmp = nTmpBuff[nIndex];
	
	fPga = AcCalcReadValid(pDataTable->nPulseConst, pDataTable->nHFConst, r_Pga, nTmpBuff[nIndex++]);
	if(nTmp&0x800000)
    {
        fPga = 360.0 - fPga;
    }
	if (0x0001F0CD == pDataTable->nCheckSumReset)
	{
		fPga -= 60.0;
	}
		
	nTmp = nTmpBuff[nIndex];
	
	fPgb = AcCalcReadValid(pDataTable->nPulseConst, pDataTable->nHFConst, r_Pgb, nTmpBuff[nIndex++]);
    if(nTmp&0x800000)
    {
        fPgb = 360.0 - fPgb;
    }
	nTmp = nTmpBuff[nIndex];
	
	fPgc = AcCalcReadValid(pDataTable->nPulseConst, pDataTable->nHFConst, r_Pgc, nTmpBuff[nIndex++]);
	if(nTmp&0x800000)
    {
        fPgc = 360.0 - fPgc;
    }
    
	if (0x0001F0CD == pDataTable->nCheckSumReset)
	{
		fPgc += 60.0;
	}

    if((nUStatus[0] == 1) || (nIStatus[0] == 1))
	{
		fData = 0.0;
	}
	else
	{
		//fData = AcCalcChangePg(0, fPga, 0.0, pDataTable->nPFlag);
	}
    nTmp = (INT32U)(fPga * 10);
    INT32U2BCD(nTmp, 4, &pDataTable->sRealInstant.sRealPg[0].nFrac);
	if((nUStatus[1] == 1) || (nIStatus[1] == 1))
	{
		fData = 0.0;
	}
	else
	{
		//fData = AcCalcChangePg(1, fPgb, fYab, pDataTable->nPFlag);
	}
    nTmp = (INT32U)(fPgb * 10);
    INT32U2BCD(nTmp, 4, &pDataTable->sRealInstant.sRealPg[1].nFrac);
	if((nUStatus[2] == 1) || (nIStatus[2] == 1))
	{
		fData = 0.0;
	}
	else
	{
		//fData = AcCalcChangePg(2, fPgc, fYac, pDataTable->nPFlag);
	}
    nTmp = (INT32U)(fPgc*10);
    INT32U2BCD(nTmp,4,&pDataTable->sRealInstant.sRealPg[2].nFrac);
    
    //温度传感器的输出
    fData = AcCalcReadValid(pDataTable->nPulseConst, pDataTable->nHFConst, r_TempD, nTmpBuff[nIndex++]);
    nTmp= (INT32S)(25.0-0.726*fData);
    nTmp= (7*nTmp+44)/8;
    pDataTable->sTpSlip.nVal[pDataTable->sTpSlip.nSlips++] = nTmp;
    //电表运行状态字
	
	if((pDataTable->nPFlag&0x08))
	{
		pDataTable->sRealInstant.bsRunStat[0] |= 0x0010;
	}
	else
	{
		pDataTable->sRealInstant.bsRunStat[0] &= 0xffef;
	}
	if((pDataTable->nPFlag&0x80))
	{
		pDataTable->sRealInstant.bsRunStat[0] |= 0x0020;
	}
	else
	{
		pDataTable->sRealInstant.bsRunStat[0] &= 0xffdf;
	}
	for(ii = 0; ii < 3; ii++)
	{
		if((pDataTable->nPFlag&(1<<ii)))
		{
			pDataTable->sRealInstant.bsRunStat[1] |= (0x0001<<ii);
		}
		else
		{
			pDataTable->sRealInstant.bsRunStat[1] &= (~(0x0001<<ii));
		}
		if((pDataTable->nPFlag&(1<<(ii+4))))
		{
			pDataTable->sRealInstant.bsRunStat[1] |= (0x0001<<(ii+4));
	}
	else
	{
			pDataTable->sRealInstant.bsRunStat[1] &= (~(0x0001<<(ii+4)));
		}
	}
    if(pDataTable->nSDNum == 1)
	{
        pDataTable->sRealInstant.bsRunStat[2] |= 0x0001;
	}
	else
	{
        pDataTable->sRealInstant.bsRunStat[2] &= ~0x0001;
    }
    if(pDataTable->nSQNum == 1)
    {
        pDataTable->sRealInstant.bsRunStat[2] |= 0x0020;
	}
    else
	{
        pDataTable->sRealInstant.bsRunStat[2] &= ~0x0020;
    }
	if((pDataTable->nSFlag & 0x08)) //电压逆相序
	{
		pDataTable->sRealInstant.bsRunStat[6] |= 0x0001;
	}
	else
	{
		pDataTable->sRealInstant.bsRunStat[6] &= 0xfffe;
	}
	if((pDataTable->nSFlag & 0x10))//电流逆相序
	{
		//pDataTable->sRealInstant.bsRunStat[6] |= 0x0002;
	}
	else
	{
		pDataTable->sRealInstant.bsRunStat[6] &= 0xfffd;
	}
   
    MemWriteAcInstData(F1, &pDataTable->sRealInstant, sizeof(pDataTable->sRealInstant));

}

/*
*****************************************************************************************************
* 函数名: AcCalcBaseReal
* 输　入:
* 输  出:
* 返  回:
* 功能描述:滑差计算各基本量
*****************************************************************************************************
*/
void AcCalcBaseReal(void)
{
	INT8U	i,j;
	INT32S	nTmp;
	INT32S	nVolt[MAX_PHASE_NUM];
	INT32S	nCurr[MAX_PHASE_NUM+1];
	INT32S	nP[MAX_PHASE_NUM+1];
	INT32S	nQ[MAX_PHASE_NUM+1];
	INT32S	nS[MAX_PHASE_NUM+1];
	INT32S	nPf[MAX_PHASE_NUM+1];
    INT32S  nTp;
//A相电压 B相电压 C相电压
	for	(i=0;i<MAX_PHASE_NUM; i++)
	{
		nVolt[i] = 0;
		if(pDataTable->sVoltSlip[i].nSlips >= CALUCATES_PER_MINUTE)
		{
			pDataTable->sVoltSlip[i].nSlips = 0;
			pDataTable->sVoltSlip[i].nIsRev = 1;
		}
		if(pDataTable->sVoltSlip[i].nIsRev == 1)
		{
			nTmp = 0;
			for(j = 0; j < CALUCATES_PER_MINUTE; j++)
			{
				nTmp += pDataTable->sVoltSlip[i].nVal[j];
			}
			nVolt[i] = nTmp/CALUCATES_PER_MINUTE;
		}
		else
		{
			if(pDataTable->sVoltSlip[i].nSlips > 0)
			{
				nTmp = 0;
				for(j = 0; j < pDataTable->sVoltSlip[i].nSlips; j++)
				{
					nTmp += pDataTable->sVoltSlip[i].nVal[j];
				}
				nVolt[i] = nTmp/pDataTable->sVoltSlip[i].nSlips;
			}
		}
		
		nTmp = (INT32U)(nVolt[i]);
		if (nTmp < 60)
		{
			nTmp = 0;
		}
		INT32U2BCD(nTmp,4,&pDataTable->sStatAverage.sStatVolt[i].nFrac);
	}
	//A相电流 B相电流 C相电流 零序电流
	for(i = 0; i < MAX_PHASE_NUM + 1; i++)
	{
		nCurr[i] = 0;
		if(pDataTable->sCurrSlip[i].nSlips >= CALUCATES_PER_MINUTE)
		{
			pDataTable->sCurrSlip[i].nSlips = 0;
			pDataTable->sCurrSlip[i].nIsRev = 1;
		}

		if(pDataTable->sCurrSlip[i].nIsRev == 1)
		{
			nTmp = 0;
			for(j = 0; j < CALUCATES_PER_MINUTE; j++)
			{
				nTmp += pDataTable->sCurrSlip[i].nVal[j];
			}
			nCurr[i] = nTmp/CALUCATES_PER_MINUTE;
		}
		else
		{
			if(pDataTable->sCurrSlip[i].nSlips > 0)
			{
				nTmp = 0;
				for(j = 0; j < pDataTable->sCurrSlip[i].nSlips; j++)
				{
					nTmp += pDataTable->sCurrSlip[i].nVal[j];
				}
				nCurr[i] = nTmp/pDataTable->sCurrSlip[i].nSlips;
			}
		}
		
		nTmp = abs(nCurr[i] );
		if (nTmp < 2)
		{
			nTmp = 0;
		}
		INT32U2BCD(nTmp,6,&pDataTable->sStatAverage.sStatCurr[i].nD1);

        if(nCurr[i] < 0)//反向
	    {
            pDataTable->sStatAverage.sStatCurr[i].nD3 |= 0x80;
        }
        else
		{
            pDataTable->sStatAverage.sStatCurr[i].nD3 &= ~0x80;
	    }
	}

	//总及三相有功功率
	for(i = 0; i < MAX_PHASE_NUM + 1; i++)
	{
		nP[i] = 0;
		if(pDataTable->sPSlip[i].nSlips >= CALUCATES_PER_MINUTE)
		{
			pDataTable->sPSlip[i].nSlips = 0;
			pDataTable->sPSlip[i].nIsRev = 1;
		}
		nTmp = 0;
		if(pDataTable->sPSlip[i].nIsRev == 1)
		{
			for(j = 0; j < CALUCATES_PER_MINUTE; j++)
			{
				nTmp += pDataTable->sPSlip[i].nVal[j];
			}
			nP[i] = nTmp / CALUCATES_PER_MINUTE;
		}
		else
		{
			if(pDataTable->sPSlip[i].nSlips > 0)
			{
				for(j = 0; j < pDataTable->sPSlip[i].nSlips; j++)
		        {
					nTmp += pDataTable->sPSlip[i].nVal[j];
				}
				nP[i] = nTmp/pDataTable->sPSlip[i].nSlips;
			}
		}
		
		nTmp = (abs)(nP[i] );
        INT32U2BCD(nTmp,6,&pDataTable->sStatAverage.sStatP[i].nFrac0);
        if( nP[i] <0)
		{
            pDataTable->sStatAverage.sStatP[i].nSint |= 0x80;
		}
        else
        {
            pDataTable->sStatAverage.sStatP[i].nSint &= ~0x80;
		}
	}

	//总及三相无功功率
	for(i = 0; i < MAX_PHASE_NUM + 1; i++)
	{
		nQ[i] = 0;
		if(pDataTable->sQSlip[i].nSlips >= CALUCATES_PER_MINUTE)
		{
			pDataTable->sQSlip[i].nSlips = 0;
			pDataTable->sQSlip[i].nIsRev = 1;
		}
		nTmp = 0;
		if(pDataTable->sQSlip[i].nIsRev == 1)
		{
			for(j = 0; j < CALUCATES_PER_MINUTE; j++)
			{
				nTmp += pDataTable->sQSlip[i].nVal[j];
			}
			nQ[i] = nTmp/CALUCATES_PER_MINUTE;
		}
		else
		{
			if(pDataTable->sQSlip[i].nSlips > 0)
			{
				for(j = 0; j < pDataTable->sQSlip[i].nSlips; j++)
				{
					nTmp += pDataTable->sQSlip[i].nVal[j];
				}
				nQ[i] = nTmp/pDataTable->sQSlip[i].nSlips;
			}
		}
		nTmp = abs( nQ[i] );
		INT32U2BCD(nTmp,6,&pDataTable->sStatAverage.sStatQ[i].nFrac0);
        if( nQ[i] <0)
		{
            pDataTable->sStatAverage.sStatQ[i].nSint |= 0x80;
		}
        else
        {
            pDataTable->sStatAverage.sStatQ[i].nSint &= ~0x80;
		}
	}

	//总及三相视在功率
	for(i = 0; i < MAX_PHASE_NUM + 1; i++)
	{
		nS[i] = 0;
		if(pDataTable->sSSlip[i].nSlips >= CALUCATES_PER_MINUTE)
		{
			pDataTable->sSSlip[i].nSlips = 0;
			pDataTable->sSSlip[i].nIsRev = 1;
		}
		nTmp = 0;
		if(pDataTable->sSSlip[i].nIsRev == 1)
		{
			for(j = 0; j < CALUCATES_PER_MINUTE; j++)
			{
				nTmp += pDataTable->sSSlip[i].nVal[j];
			}
			nS[i] = nTmp/CALUCATES_PER_MINUTE;
		}
		else
		{
			if(pDataTable->sSSlip[i].nSlips > 0)
			{
				for(j = 0; j < pDataTable->sSSlip[i].nSlips; j++)
				{
					nTmp += pDataTable->sSSlip[i].nVal[j];
				}
				nS[i] = nTmp/pDataTable->sSSlip[i].nSlips;
			}
		}
		nTmp = abs(nS[i] );
		INT32U2BCD(nTmp,6,&pDataTable->sStatAverage.sStatS[i].nFrac0);
        if( nS[i] <0)
		{
            pDataTable->sStatAverage.sStatS[i].nSint |= 0x80;
		}
		else
		{
            pDataTable->sStatAverage.sStatS[i].nSint &= ~0x80;
		}
	}

	//总及三相功率因数
	for(i = 0; i < MAX_PHASE_NUM + 1; i++)
	{
		nPf[i] = 0;
		if(pDataTable->sPfSlip[i].nSlips >= CALUCATES_PER_MINUTE)
		{
			pDataTable->sPfSlip[i].nSlips = 0;
			pDataTable->sPfSlip[i].nIsRev = 1;
		}
		nTmp = 0;
		if(pDataTable->sPfSlip[i].nIsRev == 1)
		{
			for(j=0; j<CALUCATES_PER_MINUTE; j++)
			{
				nTmp += pDataTable->sPfSlip[i].nVal[j];
			}
			nPf[i] = nTmp/CALUCATES_PER_MINUTE;
		}
		else
	    {
			if(pDataTable->sPfSlip[i].nSlips > 0)
		    {
				for(j=0; j<pDataTable->sPfSlip[i].nSlips; j++)
	            {
					nTmp += pDataTable->sPfSlip[i].nVal[j];
		        }
				nPf[i] = nTmp/pDataTable->sPfSlip[i].nSlips;
		    }
	    }
		nTmp = abs(nPf[i]);
		INT32U2BCD(nTmp,4,&pDataTable->sStatAverage.sStatPf[i].nFrac);
        if( nPf[i] <0)
		{
            pDataTable->sStatAverage.sStatPf[i].nSint |= 0x80;
		}
		else
		{
            pDataTable->sStatAverage.sStatPf[i].nSint &= ~0x80;
		}
	}
    nTp = 0;
	if(pDataTable->sTpSlip.nSlips >= CALUCATES_PER_MINUTE)
	{
		pDataTable->sTpSlip.nSlips = 0;
		pDataTable->sTpSlip.nIsRev = 1;
	}
	nTmp = 0;
	if(pDataTable->sTpSlip.nIsRev == 1)
	{
		for(j=0; j<CALUCATES_PER_MINUTE; j++)
		{
			nTmp += pDataTable->sTpSlip.nVal[j];
		}
		nTp = nTmp/CALUCATES_PER_MINUTE;
	}
	else
	{
		if(pDataTable->sTpSlip.nSlips > 0)
	    {
			for(j=0; j<pDataTable->sTpSlip.nSlips; j++)
	        {
				nTmp += pDataTable->sTpSlip.nVal[j];
	        }
			nTp = nTmp/pDataTable->sTpSlip.nSlips;
	    }
	}
	nTmp = abs(nTp);
	INT32U2BCD(nTmp,2,&pDataTable->sStatAverage.sStatTp.nVal);
    if( nTp <0)
	{
        pDataTable->sStatAverage.sStatTp.nVal |= 0x80;
	}
	else
	{
        pDataTable->sStatAverage.sStatTp.nVal &= ~0x80;
	}
	MemWriteAcInstData(F3, &pDataTable->sStatAverage, sizeof(pDataTable->sStatAverage));
	
}

/*
*****************************************************************************************************
* 函数名: AcCalcEnergy
* 输　入:
* 输  出:
* 返  回:
* 功能描述:计算电能量数据
*****************************************************************************************************
*/
void AcCalcEnergy(void)
{
    INT8U  i = 0;
    INT8U  j = 0;
    INT8U   nTemp = 0;
    INT8U  nCnt = 0;
	INT8U  nErr = 0;
    INT8U  nEnAccFlag = 0;
    INT16U nPulseCnt = 0;
    INT32U nEPSum = 0;
	INT32U nENSum = 0;
    INT8U  nACRunStat[LEN_RUN_STAT_FLAG];
    
    if (g_MeterAcErrFlg[1])//停走
    {
        return;
    }
    
    memset(&nACRunStat[0], 0, sizeof(nACRunStat));
    OSSemPend(g_pSem[SEM_ACENWR_CTRL_IDX], 0, &nErr);
    nEPSum = CheckINT32USum((INT32U*)&pSavDataTable->sEnergyData[0], 0, (8*sizeof(ENERGY_DATA) + 2*MAX_PHASE_NUM*sizeof(PH_ENERGY_DATA) + sizeof(COMB_ENERGY_DATA))/4);
	nENSum = 0x55aa66bb + CheckINT32USum((INT32U*)&pSavDataTable->sAcEnergy, 0, (sizeof(MT_DGN_ENERGY_DATA) + sizeof(MT_DGN_Q_ENERGY_DATA)
             + sizeof(MT_DGN_TRI_ENERGY_DATA) + sizeof(MT_DGN_TRI_Q_EN_DATA) + sizeof(MT_DGN_C_ENERGY_DATA))/4);
    if ((pSavDataTable->nENSumChk != nENSum) || (pSavDataTable->nEPSumChk != nEPSum))
    {
#if 0
        MemReadFlhXmpData(F8, 0, &nACRunStat[0], sizeof(nACRunStat));
        if (!TestBufBit(nACRunStat, 8, 0))
        {
            SetBufBit(nACRunStat, 8, 0);
            MemWriteFlhXmpData(F8, 0, &nACRunStat[0], sizeof(nACRunStat));
        }
        if (!TEST_BIT(pDataTable->nACRunStat, 0))
        {
            SET_BIT(pDataTable->nACRunStat, 0);
        }
        if (RET_ERR_OK != ReadAcENData())
        {
            OSSemPost(g_pSem[SEM_ACENWR_CTRL_IDX]);
		    return;
        }
#endif
    }
    
    if (TEST_BIT(pDataTable->nACRunStat, 0))
    {
        CLR_BIT(pDataTable->nACRunStat, 0);
    }
    nPulseCnt = pDataTable->nPulseConst/100;
    
    if(1 == g_nFlgnRateNo)          //费率切换
    {
      memset(&pSavDataTable->sEnergyData[0], 0, 8 * sizeof(ENERGY_DATA));
      memset(&pSavDataTable->sPhEnergyData[0], 0, 3 * sizeof(PH_ENERGY_DATA));
      memset(&pSavDataTable->sPhQEnergyData[0], 0, 3 * sizeof(PH_ENERGY_DATA));
      memset(&pSavDataTable->sCombEPulse, 0, sizeof(COMB_ENERGY_DATA));
      
      g_nFlgnRateNo = 0;
    }  
    for (i = 0; i < 2; i++)             //有功
    {
        
        pSavDataTable->sEnergyData[i].nEnergy[pDataTable->nRateNo] += pDataTable->nRealEnergy[i][3];
        if (pSavDataTable->sEnergyData[i].nEnergy[pDataTable->nRateNo] >= nPulseCnt)
		{
            nEnAccFlag = 0x55;
		}
        pSavDataTable->sEnergyData[i].nEnergy[0] += pDataTable->nRealEnergy[i][3];
        if(pSavDataTable->sEnergyData[i].nEnergy[0] >= nPulseCnt)
        {
            nEnAccFlag = 0x55;
        }
    }
    if (TEST_BIT(pSavDataTable->nCombEp, 0))    //组合有功
    {
        
        pSavDataTable->sCombEPulse.nCombEpEnergy[pDataTable->nRateNo] += pDataTable->nRealEnergy[0][3];
        pSavDataTable->sCombEPulse.nCombEpEnergy[0] += pDataTable->nRealEnergy[0][3];
    }
    if (TEST_BIT(pSavDataTable->nCombEp, 1))
    {
        pSavDataTable->sCombEPulse.nCombEpEnergy[pDataTable->nRateNo] -= pDataTable->nRealEnergy[0][3];
        pSavDataTable->sCombEPulse.nCombEpEnergy[0] -= pDataTable->nRealEnergy[0][3];
    }
    if (TEST_BIT(pSavDataTable->nCombEp, 2))
    {
        pSavDataTable->sCombEPulse.nCombEpEnergy[pDataTable->nRateNo] += pDataTable->nRealEnergy[1][3];
        pSavDataTable->sCombEPulse.nCombEpEnergy[0] += pDataTable->nRealEnergy[1][3];
    }
    if (TEST_BIT(pSavDataTable->nCombEp, 3))
    {
        pSavDataTable->sCombEPulse.nCombEpEnergy[pDataTable->nRateNo] -= pDataTable->nRealEnergy[1][3];
        pSavDataTable->sCombEPulse.nCombEpEnergy[0] -= pDataTable->nRealEnergy[1][3];
    }
    if (abs(pSavDataTable->sCombEPulse.nCombEpEnergy[pDataTable->nRateNo]) >= nPulseCnt)
    {
        nEnAccFlag = 0x55;
    }
    if (abs(pSavDataTable->sCombEPulse.nCombEpEnergy[0]) >= nPulseCnt)
    {
        nEnAccFlag = 0x55;
    }
    
    if (TEST_BIT(pSavDataTable->nCombEq1, 0))    //组合无功1
    {
        pSavDataTable->sEnergyData[2].nEnergy[pDataTable->nRateNo] += pDataTable->nRealEnergy[2][3];
        pSavDataTable->sEnergyData[2].nEnergy[0] += pDataTable->nRealEnergy[2][3];
    }
    if (TEST_BIT(pSavDataTable->nCombEq1, 1))
    {
        pSavDataTable->sEnergyData[2].nEnergy[pDataTable->nRateNo] -= pDataTable->nRealEnergy[2][3];
        pSavDataTable->sEnergyData[2].nEnergy[0] -= pDataTable->nRealEnergy[2][3];
    }
    if (TEST_BIT(pSavDataTable->nCombEq1, 2))
    {
        pSavDataTable->sEnergyData[2].nEnergy[pDataTable->nRateNo] += pDataTable->nRealEnergy[3][3];
        pSavDataTable->sEnergyData[2].nEnergy[0] += pDataTable->nRealEnergy[3][3];
    }
    if (TEST_BIT(pSavDataTable->nCombEq1, 3))
    {
        pSavDataTable->sEnergyData[2].nEnergy[pDataTable->nRateNo] -= pDataTable->nRealEnergy[3][3];
        pSavDataTable->sEnergyData[2].nEnergy[0] -= pDataTable->nRealEnergy[3][3];
    }
    if (abs(pSavDataTable->sEnergyData[2].nEnergy[pDataTable->nRateNo]) >= nPulseCnt)
    {
        nEnAccFlag = 0x55;
    }
    if (abs(pSavDataTable->sEnergyData[2].nEnergy[0]) >= nPulseCnt)
    {
        nEnAccFlag = 0x55;
    }
    
    if (TEST_BIT(pSavDataTable->nCombEq2, 4))    //组合无功2
    {
        pSavDataTable->sEnergyData[3].nEnergy[pDataTable->nRateNo] += pDataTable->nRealEnergy[4][3];
        pSavDataTable->sEnergyData[3].nEnergy[0] += pDataTable->nRealEnergy[4][3];
    }
    if (TEST_BIT(pSavDataTable->nCombEq2, 5))
    {
        pSavDataTable->sEnergyData[3].nEnergy[pDataTable->nRateNo] -= pDataTable->nRealEnergy[4][3];
        pSavDataTable->sEnergyData[3].nEnergy[0] -= pDataTable->nRealEnergy[4][3];
    }
    if (TEST_BIT(pSavDataTable->nCombEq2, 6))
    {
        pSavDataTable->sEnergyData[3].nEnergy[pDataTable->nRateNo] += pDataTable->nRealEnergy[5][3];
        pSavDataTable->sEnergyData[3].nEnergy[0] += pDataTable->nRealEnergy[5][3];
    }
    if (TEST_BIT(pSavDataTable->nCombEq2, 7))
    {
        pSavDataTable->sEnergyData[3].nEnergy[pDataTable->nRateNo] -= pDataTable->nRealEnergy[5][3];
        pSavDataTable->sEnergyData[3].nEnergy[0] -= pDataTable->nRealEnergy[5][3];
    }
    if (abs(pSavDataTable->sEnergyData[3].nEnergy[pDataTable->nRateNo]) >= nPulseCnt)
    {
        nEnAccFlag = 0x55;
    }
    if (abs(pSavDataTable->sEnergyData[3].nEnergy[0]) >= nPulseCnt)
    {
        nEnAccFlag = 0x55;
    }
    
    for (i = 0; i < 4; i++)  //四象限
    {
        pSavDataTable->sEnergyData[i + 4].nEnergy[pDataTable->nRateNo] += pDataTable->nRealEnergy[i + 2][3];
        if (pSavDataTable->sEnergyData[i + 4].nEnergy[pDataTable->nRateNo] >= nPulseCnt)
		{
            nEnAccFlag = 0x55;
		}
        pSavDataTable->sEnergyData[i + 4].nEnergy[0] += pDataTable->nRealEnergy[i + 2][3];
        if(pSavDataTable->sEnergyData[i + 4].nEnergy[0] >= nPulseCnt)
        {
            nEnAccFlag = 0x55;
        }
    }
    
    for (j = 0; j < MAX_PHASE_NUM; j++)				//分相电能
    {
		for (i = 0; i < 2; i++)		//正向有功电能 反向有功电能	
		{
			pSavDataTable->sPhEnergyData[j].nEnergy[i] += pDataTable->nRealEnergy[i][j];
            if (pSavDataTable->sPhEnergyData[j].nEnergy[i] >= nPulseCnt)
            {
                nEnAccFlag = 0x55;
            }
	    }

        for (i = 0; i < 4; i++)                 //一象限 二象限 三象限 四象限
        {
            pSavDataTable->sPhQEnergyData[j].nEnergy[i] += pDataTable->nRealEnergy[i + 2][j];
            if(pSavDataTable->sPhQEnergyData[j].nEnergy[i] >= nPulseCnt)
		    {
                nEnAccFlag = 0x55;
		    }
        }
	}
    
    for (i = 0; i < MAX_PHASE_NUM; i++)             //分相组合无功
    {
        if (TEST_BIT(pSavDataTable->nCombEq1, 0))
        {
            pSavDataTable->sPhEnergyData[i].nEnergy[2] += pDataTable->nRealEnergy[2][i];
        }
        if (TEST_BIT(pSavDataTable->nCombEq1, 1))
        {
            pSavDataTable->sPhEnergyData[i].nEnergy[2] -= pDataTable->nRealEnergy[2][i];
        }
        if (TEST_BIT(pSavDataTable->nCombEq1, 2))
        {
            pSavDataTable->sPhEnergyData[i].nEnergy[2] += pDataTable->nRealEnergy[3][i];
        }
        if (TEST_BIT(pSavDataTable->nCombEq1, 3))
        {
            pSavDataTable->sPhEnergyData[i].nEnergy[2] -= pDataTable->nRealEnergy[3][i];
        }
        if (abs(pSavDataTable->sPhEnergyData[i].nEnergy[2]) >= nPulseCnt)
        {
            nEnAccFlag = 0x55;
        }

        if (TEST_BIT(pSavDataTable->nCombEq2, 4))
        {
            pSavDataTable->sPhEnergyData[i].nEnergy[3] += pDataTable->nRealEnergy[4][i];
        }
        if (TEST_BIT(pSavDataTable->nCombEq2, 5))
        {
            pSavDataTable->sPhEnergyData[i].nEnergy[3] -= pDataTable->nRealEnergy[4][i];
        }
        if (TEST_BIT(pSavDataTable->nCombEq2, 6))
        {
            pSavDataTable->sPhEnergyData[i].nEnergy[3] += pDataTable->nRealEnergy[5][i];
        }
        if (TEST_BIT(pSavDataTable->nCombEq2, 7))
        {
            pSavDataTable->sPhEnergyData[i].nEnergy[3] -= pDataTable->nRealEnergy[5][i];
        }
        if (abs(pSavDataTable->sPhEnergyData[i].nEnergy[3]) >= nPulseCnt)
        {
            nEnAccFlag = 0x55;
        }
    }
    
    if ((nEnAccFlag == 0x55) || (pDataTable->nFramCounter >= AC_FRAM_WR_TIME))
    {
        pDataTable->nFramCounter = 0;
        WriteAcENData();
    }
    
    pSavDataTable->nEPSumChk = CheckINT32USum((INT32U*)&pSavDataTable->sEnergyData[0], 0, (8*sizeof(ENERGY_DATA)
                               + 2*MAX_PHASE_NUM*sizeof(PH_ENERGY_DATA) + sizeof(COMB_ENERGY_DATA))/4);
    if (pDataTable->nFlashCounter >= AC_FLASH_WR_TIME)
    {
		pDataTable->nFlashCounter = 0;
		BackupAcData();
	}
	OSSemPost(g_pSem[SEM_ACENWR_CTRL_IDX]);
    
    
}

/*
*****************************************************************************************************
* 函数名: AcCalcEventProc
* 输　入:
* 输  出:
* 返  回:
* 功能描述:交采事件判断处理
*****************************************************************************************************
*/
void AcCalcEventProc(void)
{
    AC_EVENT_STAT sEventStat;
	AC_EVENT_STAT sEventStatBack;
	STD_TIME sTime;
    
    //获取系统时钟
    memset(&sTime, 0, sizeof(STD_TIME));
    if (RET_ERR_OK != GetSysClk(&sTime))
	{
        return;
	}
    memset(&sEventStat, 0, sizeof(AC_EVENT_STAT));
	memset(&sEventStatBack, 0, sizeof(AC_EVENT_STAT));
    
    MemReadAcXBackData(F13, &sEventStat, sizeof(AC_EVENT_STAT));
    
    memcpy(&sEventStatBack, &sEventStat, sizeof(AC_EVENT_STAT));
    
    if (4.0 == fMulFlag)			//三线
	{
		
	}
    else
    {
        if ((pDataTable->fVolt[0] > (g_nVolt * VOLT_PH_ERR_RATIO)) && (pDataTable->fVolt[1] > (g_nVolt * VOLT_PH_ERR_RATIO)) && (pDataTable->fVolt[2] > (g_nVolt * VOLT_PH_ERR_RATIO)))
		{
			AcCalcVoltPhaseErr(&sEventStat, &sTime);
		}
        if ((pDataTable->fVolt[0] < (g_nVolt * VOLT_POWER_FAIL_RATIO)) && (pDataTable->fVolt[1] < (g_nVolt * VOLT_POWER_FAIL_RATIO)) && (pDataTable->fVolt[2] < (g_nVolt * VOLT_POWER_FAIL_RATIO)))
        {
            if ((pDataTable->fCurr[0] > (RATED_CURR * RE_CURR)) || (pDataTable->fCurr[1] > (RATED_CURR * RE_CURR)) || (pDataTable->fCurr[2] > (RATED_CURR * RE_CURR)))
            {
                AcCalcNoVoltage(1, &sEventStat, &sTime);                        //全失压
				AcCalcPowerFail(0, &sEventStat, &sTime);
                AcCalcLossOfPhase(&sEventStat, &sTime);                             //断相
                AcCalcLossOfVoltage(&sEventStat, &sTime);                           //失压
                AcCalcLossOfCurrent(&sEventStat, &sTime);                           //失流
                AcCalcOverOfCurrent(&sEventStat, &sTime);                           //过流
                AcCalcReflow(&sEventStat, &sTime);                           //潮流反向
                AcCalcOverload(&sEventStat, &sTime);                           //过载
                AcCalcUnderVoltage(&sEventStat, &sTime);                           //欠压
                AcCalcCurrentFailure(&sEventStat, &sTime);                           //断流
            }
            else
            {
                AcCalcNoVoltage(0, &sEventStat, &sTime);
				AcCalcPowerFail(1, &sEventStat, &sTime);                        //掉电
            }
        }
        else
		{
			AcCalcPowerFail(0, &sEventStat, &sTime);
			AcCalcNoVoltage(0, &sEventStat, &sTime);
            AcCalcLossOfPhase(&sEventStat, &sTime);                             //断相
            AcCalcOverVoltage(&sEventStat, &sTime);                           //过压
            AcCalcLossOfVoltage(&sEventStat, &sTime);                           //失压
            AcCalcLossOfCurrent(&sEventStat, &sTime);                           //失流
            AcCalcOverOfCurrent(&sEventStat, &sTime);                           //过流
            AcCalcReflow(&sEventStat, &sTime);                           //潮流反向
            AcCalcOverload(&sEventStat, &sTime);                           //过载
            AcCalcUnderVoltage(&sEventStat, &sTime);                           //欠压
            AcCalcCurrentFailure(&sEventStat, &sTime);                           //断流
            
            MemWriteAcInstData(F1, &pDataTable->sRealInstant, sizeof(pDataTable->sRealInstant));
            MemReadAcInstData(F1, &pDataTable->sRealInstant, sizeof(pDataTable->sRealInstant));
		}
        
        
    }
    if(0 != memcmp(&sEventStat, &sEventStatBack, sizeof(AC_EVENT_STAT)))
	{
		MemWriteAcXBackData(F13, &sEventStat, sizeof(AC_EVENT_STAT));
	}
}

/*
*****************************************************************************************************
* 函数名: AcCalaClrProc
* 输　入:
* 输  出:
* 返  回:
* 功能描述:交采清零相关处理
*****************************************************************************************************
*/
void AcCalaClrProc(void)
{
    INT8U ii,j;
	INT8S   nRetFlag = RET_ERR_ERROR;
	AC_FRE_STAT		sAcFreeze;
	AC_EVECLR_STAT	sAcEvent[3];
	AC_FRE_STAT		sAcInit;
    
     MEM_AC_DAY_DATA sAcDayData;
    
    MemReadRamXmpData(F10, 0, &sAcFreeze, sizeof(AC_FRE_STAT));
    if (0x55 == sAcFreeze.nFlag)
    {
        AcCalcClrDemandEve(&sAcFreeze.nMasterNo[0]);
		AcCalcDemInitProc();
		memset(&sAcFreeze, 0, sizeof(AC_FRE_STAT));
        MemWriteRamXmpData(F10, 0, &sAcFreeze, sizeof(AC_FRE_STAT));
        
    }
    
    MemReadRamXmpData(F12, 0, &sAcEvent, 3 * sizeof(AC_EVECLR_STAT));
    if ((0x55 == sAcEvent[0].nFlag) || (0x55 == sAcEvent[1].nFlag) || (0x55 == sAcEvent[2].nFlag))
    {
        for (ii = 0; ii < 3; ii++)
		{
            if (0x55 == sAcEvent[ii].nFlag)
            {
                nRetFlag = AcCalcEventInitProc(&sAcEvent[ii].nDataID[0]);
				if (RET_ERR_OK == nRetFlag)
				{
					AcCalcClrEventEve(&sAcEvent[ii].nMasterNo[0]);
				}
				memset(&sAcEvent[ii], 0, sizeof(AC_EVECLR_STAT));
            }
        }
        MemWriteRamXmpData(F12, 0, &sAcEvent, 3 * sizeof(AC_EVECLR_STAT));
    }
    
    MemReadRamXmpData(F11, 0, &sAcInit, sizeof(AC_FRE_STAT));
    if (0x55 == sAcInit.nFlag)
    {
        AcCalcClrMeterEve(&sAcInit.nMasterNo[0]);
		AcCalcDataInitProc();
		memset(&sAcInit, 0, sizeof(AC_FRE_STAT));
		MemWriteRamXmpData(F11, 0, &sAcInit, sizeof(AC_FRE_STAT));
        
        //AcDayDataClr();
        for (j = 0; j < MAX_AC_DAY_DATA_NUM; j++)
        {
            MemReadAcHistDataProc(F1, j, &sAcDayData,sizeof(MEM_AC_DAY_DATA));
            memset(&sAcDayData, 0, sizeof(sAcDayData));
            MemWriteAcHistData(F1, j, &sAcDayData, sizeof(MEM_AC_DAY_DATA));
        }
    }
}

/*
*****************************************************************************************************
* 函数名: AcDayDataClr
* 输　入:
* 输  出:
* 返  回:
* 功能描述:日数据信息、数据清零
*****************************************************************************************************
*/
INT32S  AcDayDataClr(void)
{
    INT8U           j;
    MEM_AC_DAY_DATA sAcDayData;
    
    for (j = 0; j < MAX_AC_DAY_DATA_NUM; j++)
    {
        MemReadAcHistDataProc(F1, j, &sAcDayData,sizeof(MEM_AC_DAY_DATA));
        memset(&sAcDayData, 0, sizeof(sAcDayData));
        MemWriteAcHistData(F1, j, &sAcDayData, sizeof(MEM_AC_DAY_DATA));
    }
    
    MemDeleteACDayData(F1);
    return RET_ERR_OK;
}

/*
*****************************************************************************************************
* 函数名: AcRDayDataClr
* 输　入:
* 输  出:
* 返  回:
* 功能描述:结算日 数据清零
*****************************************************************************************************
*/
INT32S  AcRDayDataClr(void)
{
    INT8U               j;
    MEM_AC_RDAY_DATA    sAcRDayData;
    
    for (j = 0; j < MAX_AC_DAY_DATA_NUM; j++)
    {
        MemReadAcHistDataProc(F2, j, &sAcRDayData,sizeof(MEM_AC_RDAY_DATA));
        memset(&sAcRDayData, 0, sizeof(sAcRDayData));
        MemWriteAcHistData(F2, j, &sAcRDayData, sizeof(MEM_AC_RDAY_DATA));
    }
    
    MemDeleteACDayData(F2);
    return RET_ERR_OK;
}


/*
*****************************************************************************************************
* 函数名: AcCalcVoltPhaseErr
* 输　入:
* 输  出:
* 返  回:
* 功能描述:电压逆相序事件处理
*****************************************************************************************************
*/
void AcCalcVoltPhaseErr(pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime)
{
    INT8U   jj                  = 0;
	INT8U   nIndex              = 0;
	INT32U  nNum                = 0;
	INT32U  nVoltPhaseErrTime   = 0;
    INT8U   nVoltReverseTime    = 0;
    INT8U   nVoltReverseRecTime = 0;
	INT8U   nEveData[LEN_AC_EVENT];             //QJBL
	MT_DGN_ENERGY_DATA sAcEnergy;
	MT_DGN_TRI_ENERGY_DATA sAcTriEnergy;
    
    if ((pDataTable->nSFlag & 0x08) != 0)
    {
        if(0x55 != pEventStat->nVoltPErrFlag)//电压逆相序
        {
            if (++AcEventTime[3][0] > 10)           //判事件发生持续时间为10s
            {
                memset(&nEveData, 0, LEN_AC_EVENT);
			 	memset(&sAcEnergy, 0, sizeof(MT_DGN_ENERGY_DATA));
				memset(&sAcTriEnergy, 0, sizeof(MT_DGN_TRI_ENERGY_DATA));
                
                MemReadAcRealData(F1, &sAcEnergy, sizeof(MT_DGN_ENERGY_DATA));
                MemReadAcRealData(F3, &sAcTriEnergy, sizeof(MT_DGN_TRI_ENERGY_DATA));
                
                memcpy(&nEveData[nIndex], &pTime->nSec, 6);
    			nIndex += 6;
                memcpy(&nEveData[nIndex], &sAcEnergy.sPPEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				memcpy(&nEveData[nIndex], &sAcEnergy.sNPEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				memcpy(&nEveData[nIndex], &sAcEnergy.sPNEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				memcpy(&nEveData[nIndex], &sAcEnergy.sNNEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
                
                for (jj = 0; jj < MAX_PHASE_NUM; jj++)
                {
                    memcpy(&nEveData[nIndex], &sAcTriEnergy.sPPEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sNPEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sPNEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sNNEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
                }
                
                MemReadAcXFlhBackData(F36, &nNum, sizeof(nNum));
                nNum++;
                MemReadAcXFlhBackData(F36, &nNum, sizeof(nNum));
                
                //写事件
                MemWriteAcEvtProc(F8, &nEveData, LEN_AC_EVENT, 1);
                pEventStat->nVoltPErrFlag = 0x55;
                pDataTable->sRealInstant.bsRunStat[6] |= 0x0001;
                
            }
        }
        else
		{
			AcEventTime[3][0] = 0;
		}
    }
    else
    {
        AcEventTime[3][0] = 0;
        if (0x55 == pEventStat->nVoltPErrFlag)      //事件恢复时不需要持续时间
        {
            memset(&nEveData, 0, LEN_AC_EVENT);
            MemReadAcEvtProc(F8, 1, &nEveData, LEN_AC_EVENT);
            
            nIndex = 70;
            memset(&sAcEnergy, 0, sizeof(MT_DGN_ENERGY_DATA));
			memset(&sAcTriEnergy, 0, sizeof(MT_DGN_TRI_ENERGY_DATA));
            
            MemReadAcRealData(F1, &sAcEnergy, sizeof(MT_DGN_ENERGY_DATA));
            MemReadAcRealData(F3, &sAcTriEnergy, sizeof(MT_DGN_TRI_ENERGY_DATA));
            
            memcpy(&nEveData[nIndex], &pTime->nSec, 6);
    		nIndex += 6;
            memcpy(&nEveData[nIndex], &sAcEnergy.sPPEnergy[0], sizeof(FORMAT11));
			nIndex += sizeof(FORMAT11);
			memcpy(&nEveData[nIndex], &sAcEnergy.sNPEnergy[0], sizeof(FORMAT11));
			nIndex += sizeof(FORMAT11);
			memcpy(&nEveData[nIndex], &sAcEnergy.sPNEnergy[0], sizeof(FORMAT11));
			nIndex += sizeof(FORMAT11);
			memcpy(&nEveData[nIndex], &sAcEnergy.sNNEnergy[0], sizeof(FORMAT11));
			nIndex += sizeof(FORMAT11);
            
            for (jj = 0; jj < MAX_PHASE_NUM; jj++)
			{
				memcpy(&nEveData[nIndex], &sAcTriEnergy.sPPEnergy[jj], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				memcpy(&nEveData[nIndex], &sAcTriEnergy.sNPEnergy[jj], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				memcpy(&nEveData[nIndex], &sAcTriEnergy.sPNEnergy[jj], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				memcpy(&nEveData[nIndex], &sAcTriEnergy.sNNEnergy[jj], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
			}
            //写事件
            MemWriteAcEvtProc(F8, &nEveData, LEN_AC_EVENT, 0);
            pEventStat->nVoltPErrFlag = 0xAA;
             pDataTable->sRealInstant.bsRunStat[6] &= 0xfffe;
        }
    }
    if (0x55 == pEventStat->nVoltPErrFlag)
    {
        MemReadAcXBackData(F9, &nVoltPhaseErrTime, sizeof(nVoltPhaseErrTime));
        nVoltPhaseErrTime++;
        MemReadAcXBackData(F9, &nVoltPhaseErrTime, sizeof(nVoltPhaseErrTime));
    }
}

/*
*****************************************************************************************************
* 函数名: AcCalcNoVoltage
* 输　入:
* 输  出:
* 返  回:
* 功能描述:全失压事件处理
*****************************************************************************************************
*/
void AcCalcNoVoltage(INT8U nFlag, pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime)
{
    INT8U   nIndex      = 0;
	INT32U  nNum        = 0;
	INT32U  nTmp        = 0;
	FP32    fDat1       = 0.0;
	INT32U  nNoVoltTime = 0;
	INT8U   nEveData[LEN_AC_EVENT]; //DBQL
    
    if (1 == nFlag)
    {
        if (0x55 != pEventStat->nNVolFlag)
        {
            memset(&nEveData, 0, LEN_AC_EVENT);
		 	//存发生数据
		 	memcpy(&nEveData[nIndex], &pTime->nSec, 6);
    		nIndex += 6;
    		
    		fDat1 = (pDataTable->fCurr[0] + pDataTable->fCurr[1] + pDataTable->fCurr[2]) / 3;
    		nTmp = (INT32U)(fDat1 * 1000);
			nEveData[nIndex++] = INT8U_TO_BCD((nTmp % 100) & 0xFF);
			nTmp /= 100;
			nEveData[nIndex++] = INT8U_TO_BCD((nTmp % 100) & 0xFF);
			nEveData[nIndex++] = INT8U_TO_BCD((nTmp / 100) & 0xFF);
            
            //写事件
            MemWriteAcEvtProc(F7, &nEveData, LEN_AC_EVENT, 1);
            
            MemReadAcXFlhBackData(F35, &nNum, sizeof(nNum));
            nNum++;
            MemReadAcXFlhBackData(F35, &nNum, sizeof(nNum));
            
            pEventStat->nNVolFlag = 0x55;
        }
    }
    else
    {
        if (0x55 == pEventStat->nNVolFlag)
		{
			memset(&nEveData, 0, LEN_AC_EVENT);
			MemReadAcEvtProc(F7, 1, &nEveData, LEN_AC_EVENT);
			nIndex = 9;
			//存恢复数据
			memcpy(&nEveData[nIndex], &pTime->nSec, 6);
    		nIndex += 6;
    		
			//写事件
            MemWriteAcEvtProc(F7, &nEveData, LEN_AC_EVENT, 0);
			pEventStat->nNVolFlag = 0xAA;
		}
    }
    if (0x55 == pEventStat->nNVolFlag)
	{
        MemReadAcXBackData(F8, &nNoVoltTime, sizeof(nNoVoltTime));
        nNoVoltTime++;
        MemReadAcXBackData(F8, &nNoVoltTime, sizeof(nNoVoltTime));
	}
}

/*
*****************************************************************************************************
* 函数名: AcCalcPowerFail
* 输　入:
* 输  出:
* 返  回:
* 功能描述:掉电事件处理
*****************************************************************************************************
*/
void AcCalcPowerFail(INT8U nFlag, pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime)
{
    INT8U   nIndex  = 0;
	INT32U  nNum    = 0;
	INT8U   nEveData[LEN_AC_EVENT]; //QJBL
    
    if (1 == nFlag)
    {
        if (0x55 != pEventStat->nPFailFlag)
        {
            memset(&nEveData, 0xFF, LEN_AC_EVENT);
		 	//存发生数据
		 	memcpy(&nEveData[nIndex], &pTime->nSec, 6);
    		nIndex += 6;
    		
    		MemWriteAcEvtProc(F12, &nEveData, LEN_AC_EVENT, 1);
    		
    		MemReadAcXFlhBackData(F41, &nNum, sizeof(nNum));
            nNum++;
            MemWriteAcXFlhBackData(F41, &nNum, sizeof(nNum));
    		
		 	pEventStat->nPFailFlag = 0x55;
		 	pDataTable->sRealInstant.bsRunStat[6] |= 0x0020;
        }
    }
    else
	{
		if (0x55 == pEventStat->nPFailFlag)
		{
			memset(&nEveData, 0xFF, LEN_AC_EVENT);
			MemReadAcEvtProc(F12, 1, &nEveData, LEN_AC_EVENT);
			nIndex = 6;
			//存恢复数据
			memcpy(&nEveData[nIndex], &pTime->nSec, 6);
    		nIndex += 6;
			
			MemWriteAcEvtProc(F12, &nEveData, LEN_AC_EVENT, 0);
			pEventStat->nPFailFlag = 0xAA;
			pDataTable->sRealInstant.bsRunStat[6] &= 0xffdf;
		}
	}
}

/*
*****************************************************************************************************
* 函数名: AcCalcLossOfVoltage
* 输　入:
* 输  出:
* 返  回:
* 功能描述:失压事件处理
*****************************************************************************************************
*/
void AcCalcLossOfVoltage(pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime)
{
    INT8U ii = 0;
	INT8U jj = 0;
	INT8U nIndex = 0;
	INT32U nNum;
	INT32U nLossOfVoltTime;
	INT8U nEveData[LEN_AC_EVENT];
	MT_DGN_ENERGY_DATA sAcEnergy;
	MT_DGN_TRI_ENERGY_DATA sAcTriEnergy;        //QJBL
    
    for(ii = 0; ii < 3; ii++)
    {
        if ((1 == ii) && (4.0 == fMulFlag))                 //当为三相三线，B相无电流不判断失压
        {
            continue;
        }
        if ((pDataTable->fVolt[ii] < (g_nVolt * LOSS_VOLT_OC_RATIO))&&(pDataTable->fCurr[ii] > (RATED_CURR * OC_CURR))) 
        {
            if (0x55 != pEventStat->nLVolFlag[ii])
            {
                if (++AcEventTime[0][ii] > 3)      //失压发生持续时间为60s
                {
                    memset(&nEveData, 0, LEN_AC_EVENT);
					memset(&sAcEnergy, 0, sizeof(MT_DGN_ENERGY_DATA));
					memset(&sAcTriEnergy, 0, sizeof(MT_DGN_TRI_ENERGY_DATA));
					//存发生数据
					MemReadAcRealData(F1, &sAcEnergy, sizeof(MT_DGN_ENERGY_DATA));
                    MemReadAcRealData(F3, &sAcTriEnergy, sizeof(MT_DGN_TRI_ENERGY_DATA));
                    
                    //各相失压次数
                    nNum = 0;
                    MemReadAcXFlhBackData(F28 + ii, &nNum, sizeof(nNum));
					nNum ++;
					MemWriteAcXFlhBackData(F28 + ii, &nNum, sizeof(nNum));
                    
                    nIndex = 0;
					memcpy(&nEveData[nIndex], &pTime->nSec, 6);
    				nIndex += 6;
    				memcpy(&nEveData[nIndex], &sAcEnergy.sPPEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcEnergy.sNPEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcEnergy.sPNEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcEnergy.sNNEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
                    
                    for (jj = 0; jj < MAX_PHASE_NUM; jj++)
					{
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sPPEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sNPEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sPNEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sNNEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealVolt[jj], sizeof(FORMAT07));
						nIndex += sizeof(FORMAT07);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealCurr[jj], sizeof(FORMAT25));
						nIndex += sizeof(FORMAT25);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealP[jj + 1], sizeof(FORMAT09));
						nIndex += sizeof(FORMAT09);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealQ[jj + 1], sizeof(FORMAT09));
						nIndex += sizeof(FORMAT09);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealPf[jj + 1], sizeof(FORMAT05));
						nIndex += sizeof(FORMAT05);
					}
                    memcpy(&nEveData[nIndex], &nNum, sizeof(nNum));
                    //nIndex += sizeof(nNum);
                    MemWriteAcEvtProc(F1 + ii, &nEveData, LEN_AC_EVENT, 1);
                    

                    //失压总次数
                    nNum = 0;
					MemReadAcXFlhBackData(F25, &nNum, sizeof(nNum));
					nNum ++;
					MemWriteAcXFlhBackData(F25, &nNum, sizeof(nNum));
                    
                    MemWriteAcXFlhBackData(F26, pTime, sizeof(STD_TIME));
                    
                    pEventStat->nLVolFlag[ii] = 0x55;
                    //if (pDataTable->fCurr[ii] > (RATED_CURR * OC_CURR))
                    {
                       pDataTable->sRealInstant.bsRunStat[3 + ii] |= 0x0003;	//失压 
                    }
                    //else
                    {
                    //    pDataTable->sRealInstant.bsRunStat[3 + ii] |= 0x0002;	//欠压
                    }
					
                }
            }
        }
        else
        {
            AcEventTime[0][ii] = 0;
        }
        
        if ((pDataTable->fVolt[ii] > (g_nVolt * LOSS_VOLT_RE_RATIO)))
        {
            AcEventTime[0][ii] = 0;
            if (0x55 == pEventStat->nLVolFlag[ii])          //恢复不需要持续时间
            {
                memset(&nEveData, 0, LEN_AC_EVENT);
				MemReadAcEvtProc(F1 + ii, 1, &nEveData, LEN_AC_EVENT);
				
				nIndex = 125;
				memset(&sAcEnergy, 0, sizeof(MT_DGN_ENERGY_DATA));
				memset(&sAcTriEnergy, 0, sizeof(MT_DGN_TRI_ENERGY_DATA));
				//存恢复数据
				MemReadAcRealData(F1, &sAcEnergy, sizeof(MT_DGN_ENERGY_DATA));
                MemReadAcRealData(F3, &sAcTriEnergy, sizeof(MT_DGN_TRI_ENERGY_DATA));
                
                memcpy(&nEveData[nIndex], &pTime->nSec, 6);
    			nIndex += 6;
    			memcpy(&nEveData[nIndex], &sAcEnergy.sPPEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				memcpy(&nEveData[nIndex], &sAcEnergy.sNPEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				memcpy(&nEveData[nIndex], &sAcEnergy.sPNEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				memcpy(&nEveData[nIndex], &sAcEnergy.sNNEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
                
                for (jj = 0; jj < MAX_PHASE_NUM; jj++)
				{
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sPPEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sNPEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sPNEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sNNEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
				}
                MemWriteAcEvtProc(F1 + ii, &nEveData, LEN_AC_EVENT, 0);
                
                MemWriteAcXFlhBackData(F27, pTime, sizeof(STD_TIME));
                
                pEventStat->nLVolFlag[ii] = 0xAA;
				pDataTable->sRealInstant.bsRunStat[3 + ii]  &= 0xfffc;
            }
        }
        
        if (0x55 == pEventStat->nLVolFlag[ii])
        {
            nLossOfVoltTime = 0;
			MemReadAcXBackData(F2+ ii, &nLossOfVoltTime, sizeof(nLossOfVoltTime));
			nLossOfVoltTime++;
			MemWriteAcXBackData(F2+ ii, &nLossOfVoltTime, sizeof(nLossOfVoltTime));
        }
    }
}

/*
*****************************************************************************************************
* 函数名: AcCalcLossOfVoltage
* 输　入:
* 输  出:
* 返  回:
* 功能描述:欠压事件处理
*****************************************************************************************************
*/
void AcCalcUnderVoltage(pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime)
{
    INT8U ii = 0;
	INT8U jj = 0;
	INT8U nIndex = 0;
	INT32U nNum;
	INT32U nLossOfVoltTime;
	INT8U nEveData[LEN_AC_EVENT];
	MT_DGN_ENERGY_DATA sAcEnergy;
	MT_DGN_TRI_ENERGY_DATA sAcTriEnergy;        //QJBL
    
    for(ii = 0; ii < 3; ii++)
    {
        if ((1 == ii) && (4.0 == fMulFlag))                 //当为三相三线，B相无电流不判断失压
        {
            continue;
        }
        if (pDataTable->fVolt[ii] < (g_nVolt * LOSS_VOLT_OC_RATIO)) 
        {
            if (0x55 != pEventStat->nUVolFlag[ii])
            {
                if (++AcEventTime[0][ii] > 3)      //欠压发生持续时间为60s
                {
                    memset(&nEveData, 0, LEN_AC_EVENT);
					memset(&sAcEnergy, 0, sizeof(MT_DGN_ENERGY_DATA));
					memset(&sAcTriEnergy, 0, sizeof(MT_DGN_TRI_ENERGY_DATA));
					//存发生数据
					MemReadAcRealData(F1, &sAcEnergy, sizeof(MT_DGN_ENERGY_DATA));
                    MemReadAcRealData(F3, &sAcTriEnergy, sizeof(MT_DGN_TRI_ENERGY_DATA));
                    
                    //各相欠压次数
                    nNum = 0;
                    MemReadAcXFlhBackData(F105 + ii, &nNum, sizeof(nNum));
					nNum ++;
					MemWriteAcXFlhBackData(F105 + ii, &nNum, sizeof(nNum));
                    
                    nIndex = 0;
					memcpy(&nEveData[nIndex], &pTime->nSec, 6);
    				nIndex += 6;
    				memcpy(&nEveData[nIndex], &sAcEnergy.sPPEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcEnergy.sNPEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcEnergy.sPNEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcEnergy.sNNEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
                    
                    for (jj = 0; jj < MAX_PHASE_NUM; jj++)
					{
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sPPEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sNPEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sPNEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sNNEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealVolt[jj], sizeof(FORMAT07));
						nIndex += sizeof(FORMAT07);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealCurr[jj], sizeof(FORMAT25));
						nIndex += sizeof(FORMAT25);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealP[jj + 1], sizeof(FORMAT09));
						nIndex += sizeof(FORMAT09);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealQ[jj + 1], sizeof(FORMAT09));
						nIndex += sizeof(FORMAT09);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealPf[jj + 1], sizeof(FORMAT05));
						nIndex += sizeof(FORMAT05);
					}
                    memcpy(&nEveData[252], &nNum, sizeof(nNum));
                    //nIndex += sizeof(nNum);
                    MemWriteAcEvtProc(F49 + ii, &nEveData, LEN_AC_EVENT, 1);

                    //欠压总次数
                    nNum = 0;
					MemReadAcXFlhBackData(F102, &nNum, sizeof(nNum));
					nNum ++;
					MemWriteAcXFlhBackData(F102, &nNum, sizeof(nNum));
                    
                    MemWriteAcXFlhBackData(F103, pTime, sizeof(STD_TIME));
                    
                    pEventStat->nUVolFlag[ii] = 0x55;
                    //if (pDataTable->fCurr[ii] > (RATED_CURR * OC_CURR))
                    {
                     //  pDataTable->sRealInstant.bsRunStat[3 + ii] |= 0x0003;	//失压 
                    }
                    //else
                    {
                        pDataTable->sRealInstant.bsRunStat[3 + ii] |= 0x0002;	//欠压
                    }
					
                }
            }
        }
        else
        {
            AcEventTime[0][ii] = 0;
        }
        
        if ((pDataTable->fVolt[ii] > (g_nVolt * LOSS_VOLT_RE_RATIO)))
        {
            AcEventTime[0][ii] = 0;
            if (0x55 == pEventStat->nUVolFlag[ii])          //恢复不需要持续时间
            {
                memset(&nEveData, 0, LEN_AC_EVENT);
				MemReadAcEvtProc(F1 + ii, 1, &nEveData, LEN_AC_EVENT);
				
				nIndex = 125;
				memset(&sAcEnergy, 0, sizeof(MT_DGN_ENERGY_DATA));
				memset(&sAcTriEnergy, 0, sizeof(MT_DGN_TRI_ENERGY_DATA));
				//存恢复数据
				MemReadAcRealData(F1, &sAcEnergy, sizeof(MT_DGN_ENERGY_DATA));
                MemReadAcRealData(F3, &sAcTriEnergy, sizeof(MT_DGN_TRI_ENERGY_DATA));
                
                memcpy(&nEveData[nIndex], &pTime->nSec, 6);
    			nIndex += 6;
    			memcpy(&nEveData[nIndex], &sAcEnergy.sPPEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				memcpy(&nEveData[nIndex], &sAcEnergy.sNPEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				memcpy(&nEveData[nIndex], &sAcEnergy.sPNEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				memcpy(&nEveData[nIndex], &sAcEnergy.sNNEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
                
                for (jj = 0; jj < MAX_PHASE_NUM; jj++)
				{
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sPPEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sNPEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sPNEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sNNEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
				}
                MemWriteAcEvtProc(F49 + ii, &nEveData, LEN_AC_EVENT, 0);
                
                MemWriteAcXFlhBackData(F104, pTime, sizeof(STD_TIME));
                
                pEventStat->nUVolFlag[ii] = 0xAA;
				pDataTable->sRealInstant.bsRunStat[3 + ii]  &= 0xfffc;
            }
        }
        
        if (0x55 == pEventStat->nUVolFlag[ii])
        {
            nLossOfVoltTime = 0;
			MemReadAcXBackData(F2, &nLossOfVoltTime, sizeof(nLossOfVoltTime));
			nLossOfVoltTime++;
			MemWriteAcXBackData(F2, &nLossOfVoltTime, sizeof(nLossOfVoltTime));
        }
    }
}

/*
*****************************************************************************************************
* 函数名: AcCalcLossOfVoltage
* 输　入:
* 输  出:
* 返  回:
* 功能描述:过压事件处理
*****************************************************************************************************
*/
void AcCalcOverVoltage(pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime)
{
    INT8U ii = 0;
	INT8U jj = 0;
	INT8U nIndex = 0;
	INT32U nNum;
	INT32U nOverVoltTime;
	INT8U nEveData[LEN_AC_EVENT];
	MT_DGN_ENERGY_DATA sAcEnergy;
	MT_DGN_TRI_ENERGY_DATA sAcTriEnergy;        //QJBL

    for(ii = 0; ii < 3; ii++)
    {
        if ((1 == ii) && (4.0 == fMulFlag))                 //当为三相三线，B相无电流不判断过压
        {
            continue;
        }
        if ((pDataTable->fVolt[ii] > (g_nVolt * OVER_VOLT_OC_RATIO)))
        {
            if (0x55 != pEventStat->nOVolFlag[ii])
            {
                if (++AcEventTime[4][ii] > 3)      //过压发生持续时间为60s
                {
                    memset(&nEveData, 0, LEN_AC_EVENT);
					memset(&sAcEnergy, 0, sizeof(MT_DGN_ENERGY_DATA));
					memset(&sAcTriEnergy, 0, sizeof(MT_DGN_TRI_ENERGY_DATA));
					//存发生数据
					MemReadAcRealData(F1, &sAcEnergy, sizeof(MT_DGN_ENERGY_DATA));
                    MemReadAcRealData(F3, &sAcTriEnergy, sizeof(MT_DGN_TRI_ENERGY_DATA));
                    
                    //各相过压次数
                    nNum = 0;
                    MemReadAcXFlhBackData(F52 + ii, &nNum, sizeof(nNum));
					nNum ++;
					MemWriteAcXFlhBackData(F52 + ii, &nNum, sizeof(nNum));
                    
                    nIndex = 0;
					memcpy(&nEveData[nIndex], &pTime->nSec, 6);
    				nIndex += 6;
    				memcpy(&nEveData[nIndex], &sAcEnergy.sPPEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcEnergy.sNPEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcEnergy.sPNEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcEnergy.sNNEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);

                    for (jj = 0; jj < MAX_PHASE_NUM; jj++)
					{
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sPPEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sNPEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sPNEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sNNEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealVolt[jj], sizeof(FORMAT07));
						nIndex += sizeof(FORMAT07);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealCurr[jj], sizeof(FORMAT25));
						nIndex += sizeof(FORMAT25);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealP[jj + 1], sizeof(FORMAT09));
						nIndex += sizeof(FORMAT09);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealQ[jj + 1], sizeof(FORMAT09));
						nIndex += sizeof(FORMAT09);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealPf[jj + 1], sizeof(FORMAT05));
						nIndex += sizeof(FORMAT05);
					}
                    memcpy(&nEveData[252], &nNum, sizeof(nNum));
                    //nIndex += sizeof(nNum);
                    MemWriteAcEvtProc(F18 + ii, &nEveData, LEN_AC_EVENT, 1);

                    //过压总次数
                    nNum = 0;
					MemReadAcXFlhBackData(F49, &nNum, sizeof(nNum));
					nNum ++;
					MemWriteAcXFlhBackData(F49, &nNum, sizeof(nNum));
                    
                    MemWriteAcXFlhBackData(F50, pTime, sizeof(STD_TIME));
                    
                    pEventStat->nOVolFlag[ii] = 0x55;
					pDataTable->sRealInstant.bsRunStat[3 + ii] |= 0x0004;	//过压
                }
            }
        }
        else
        {
            AcEventTime[4][ii] = 0;
        }
        
        if (pDataTable->fVolt[ii] < (g_nVolt * OVER_VOLT_OC_RATIO))
        {
            AcEventTime[4][ii] = 0;
            if (0x55 == pEventStat->nOVolFlag[ii])          //恢复不需要持续时间
            {
                memset(&nEveData, 0, LEN_AC_EVENT);
				MemReadAcEvtProc(F18 + ii, 1, &nEveData, LEN_AC_EVENT);
				
				nIndex = 125;
				memset(&sAcEnergy, 0, sizeof(MT_DGN_ENERGY_DATA));
				memset(&sAcTriEnergy, 0, sizeof(MT_DGN_TRI_ENERGY_DATA));
				//存恢复数据
				MemReadAcRealData(F1, &sAcEnergy, sizeof(MT_DGN_ENERGY_DATA));
                MemReadAcRealData(F3, &sAcTriEnergy, sizeof(MT_DGN_TRI_ENERGY_DATA));
                
                memcpy(&nEveData[nIndex], &pTime->nSec, 6);
    			nIndex += 6;
    			memcpy(&nEveData[nIndex], &sAcEnergy.sPPEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				memcpy(&nEveData[nIndex], &sAcEnergy.sNPEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				memcpy(&nEveData[nIndex], &sAcEnergy.sPNEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				memcpy(&nEveData[nIndex], &sAcEnergy.sNNEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
                
                for (jj = 0; jj < MAX_PHASE_NUM; jj++)
				{
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sPPEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sNPEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sPNEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sNNEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
				}
                MemWriteAcEvtProc(F18 + ii, &nEveData, LEN_AC_EVENT, 0);
                
                MemWriteAcXFlhBackData(F51, pTime, sizeof(STD_TIME));
                
                pEventStat->nOVolFlag[ii] = 0xAA;
				pDataTable->sRealInstant.bsRunStat[3 + ii]  &= 0xfffb;
            }
        }
        
        if (0x55 == pEventStat->nOVolFlag[ii])
        {
            nOverVoltTime = 0;
			MemReadAcXBackData(F15, &nOverVoltTime, sizeof(nOverVoltTime));
			nOverVoltTime++;
			MemWriteAcXBackData(F15, &nOverVoltTime, sizeof(nOverVoltTime));
        }
    }
}
/*
*****************************************************************************************************
* 函数名: AcCalcLossOfPhase
* 输　入:
* 输  出:
* 返  回:
* 功能描述:断相事件处理
*****************************************************************************************************
*/
void AcCalcLossOfPhase(pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime)
{
    INT8U ii = 0;
	INT8U jj = 0;
	INT8U nIndex = 0;
	INT32U nNum;
	INT32U nLossOfPhaseTime;
	INT8U nEveData[LEN_AC_EVENT];
	MT_DGN_ENERGY_DATA sAcEnergy;
	MT_DGN_TRI_ENERGY_DATA sAcTriEnergy;
    
    for(ii = 0; ii < MAX_PHASE_NUM; ii++)
    {
        if ((1 == ii) && (4.0 == fMulFlag))                 //当为三相三线，B相无电流不判断断相
        {
            continue;
        }
        if ((pDataTable->fVolt[ii] < (g_nVolt * LOSS_PH_OC_RATIO)) && (pDataTable->fCurr[ii] < (RATED_CURR * LOSS_PH_CURR)))
        {
            if (0x55 != pEventStat->nLPhaseFlag[ii])
            {
                if (++AcEventTime[1][ii] > 3) //(++AcEventTime[1][ii] > 3)
                {
                    memset(&nEveData, 0, LEN_AC_EVENT);
					memset(&sAcEnergy, 0, sizeof(MT_DGN_ENERGY_DATA));
					memset(&sAcTriEnergy, 0, sizeof(MT_DGN_TRI_ENERGY_DATA));
					//存发生数据
					MemReadAcRealData(F1, &sAcEnergy, sizeof(MT_DGN_ENERGY_DATA));
                    MemReadAcRealData(F3, &sAcTriEnergy, sizeof(MT_DGN_TRI_ENERGY_DATA));
                    
                    nNum = 0;
					MemReadAcXFlhBackData(F32 + ii, &nNum, sizeof(nNum));
					nNum ++;
					MemWriteAcXFlhBackData(F32 + ii, &nNum, sizeof(nNum));
                    
                    nIndex = 0;
					memcpy(&nEveData[nIndex], &pTime->nSec, 6);
    				nIndex += 6;
    				memcpy(&nEveData[nIndex], &sAcEnergy.sPPEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcEnergy.sNPEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcEnergy.sPNEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcEnergy.sNNEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					
					for (jj = 0; jj < MAX_PHASE_NUM; jj++)
					{
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sPPEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sNPEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sPNEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sNNEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealVolt[jj], sizeof(FORMAT07));
						nIndex += sizeof(FORMAT07);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealCurr[jj], sizeof(FORMAT25));
						nIndex += sizeof(FORMAT25);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealP[jj + 1], sizeof(FORMAT09));
						nIndex += sizeof(FORMAT09);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealQ[jj + 1], sizeof(FORMAT09));
						nIndex += sizeof(FORMAT09);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealPf[jj + 1], sizeof(FORMAT05));
						nIndex += sizeof(FORMAT05);
					}
                    memcpy(&nEveData[252], &nNum, sizeof(nNum));
                    //nIndex += sizeof(nNum);
                    MemWriteAcEvtProc(F4 + ii, &nEveData, LEN_AC_EVENT, 1);
					
					nNum = 0;
					MemReadAcXFlhBackData(F31 , &nNum, sizeof(nNum));//+ ii
					nNum ++;
					MemWriteAcXFlhBackData(F31 , &nNum, sizeof(nNum));//+ ii
					pEventStat->nLPhaseFlag[ii] = 0x55;
					pDataTable->sRealInstant.bsRunStat[3 + ii] |= 0x0080;	//断相
                }
            }
        }
        else
        {
            AcEventTime[1][ii] = 0;
        }
        
        if ((pDataTable->fVolt[ii] > (g_nVolt * LOSS_PH_RE_RATIO)))
        {
            AcEventTime[1][ii] = 0;
            if (0x55 == pEventStat->nLPhaseFlag[ii])
            {
                memset(&nEveData, 0, LEN_AC_EVENT);
				MemReadAcEvtProc(F4 + ii, 1, &nEveData, LEN_AC_EVENT);
                
                nIndex = 125;
				memset(&sAcEnergy, 0, sizeof(MT_DGN_ENERGY_DATA));
				memset(&sAcTriEnergy, 0, sizeof(MT_DGN_TRI_ENERGY_DATA));
				//存恢复数据
				MemReadAcRealData(F1, &sAcEnergy, sizeof(MT_DGN_ENERGY_DATA));
                MemReadAcRealData(F3, &sAcTriEnergy, sizeof(MT_DGN_TRI_ENERGY_DATA));
                
                memcpy(&nEveData[nIndex], &pTime->nSec, 6);
    			nIndex += 6;
    			memcpy(&nEveData[nIndex], &sAcEnergy.sPPEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				memcpy(&nEveData[nIndex], &sAcEnergy.sNPEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				memcpy(&nEveData[nIndex], &sAcEnergy.sPNEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				memcpy(&nEveData[nIndex], &sAcEnergy.sNNEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
                
                for (jj = 0; jj < MAX_PHASE_NUM; jj++)
				{
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sPPEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sNPEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sPNEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sNNEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
				}
                MemWriteAcEvtProc(F4 + ii, &nEveData, LEN_AC_EVENT, 0);
                pEventStat->nLPhaseFlag[ii] = 0xaa;
				pDataTable->sRealInstant.bsRunStat[3 + ii] &= 0xff7f;
            }
        }
        if (0x55 == pEventStat->nLPhaseFlag[ii])
		{
			nLossOfPhaseTime = 0;
			MemReadAcXBackData(F5 + ii, &nLossOfPhaseTime, sizeof(nLossOfPhaseTime));
			nLossOfPhaseTime++;
			MemWriteAcXBackData(F5 + ii, &nLossOfPhaseTime, sizeof(nLossOfPhaseTime));
		}
    }
}

/*
*****************************************************************************************************
* 函数名: AcCalcLossOfCurrent
* 输　入:
* 输  出:
* 返  回:
* 功能描述:失流事件处理
*****************************************************************************************************
*/
void AcCalcLossOfCurrent(pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime)
{
    INT8U ii = 0;
	INT8U jj = 0;
	INT8U nIndex = 0;
	INT32U nNum;
	INT32U nLossOfCurrTime;
	INT8U nEveData[LEN_AC_EVENT];
	MT_DGN_ENERGY_DATA sAcEnergy;
	MT_DGN_TRI_ENERGY_DATA sAcTriEnergy;
    
    for(ii = 0; ii < MAX_PHASE_NUM; ii++)
    {
        if ((((pDataTable->fVolt[0] > (g_nVolt * LOSS_CURR_RATIO)) && (pDataTable->fVolt[1] > (g_nVolt * LOSS_CURR_RATIO)) && (pDataTable->fVolt[2] > (g_nVolt * LOSS_CURR_RATIO)))
			&& ((pDataTable->fCurr[0] > (RATED_CURR * RE_CURR)) || (pDataTable->fCurr[1] > (RATED_CURR * RE_CURR)) || (pDataTable->fCurr[2] > (RATED_CURR * RE_CURR))))
				&&(pDataTable->fCurr[ii] < RATED_CURR * OC_CURR))
        {
            if (0x55 != pEventStat->nLCurrFlag[ii])
            {
                if (++AcEventTime[2][ii] > 3)
                {
                    memset(&nEveData, 0, LEN_AC_EVENT);
					memset(&sAcEnergy, 0, sizeof(MT_DGN_ENERGY_DATA));
					memset(&sAcTriEnergy, 0, sizeof(MT_DGN_TRI_ENERGY_DATA));
					//存发生数据
					MemReadAcRealData(F1, &sAcEnergy, sizeof(MT_DGN_ENERGY_DATA));
                    MemReadAcRealData(F3, &sAcTriEnergy, sizeof(MT_DGN_TRI_ENERGY_DATA));
                    
                    nNum = 0;
					MemReadAcXFlhBackData(F38 + ii, &nNum, sizeof(nNum));
					nNum ++;
					MemWriteAcXFlhBackData(F38 + ii, &nNum, sizeof(nNum));
                    
                    nIndex = 0;
					memcpy(&nEveData[nIndex], &pTime->nSec, 6);
    				nIndex += 6;
    				memcpy(&nEveData[nIndex], &sAcEnergy.sPPEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcEnergy.sNPEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcEnergy.sPNEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcEnergy.sNNEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
                    
                    for (jj = 0; jj < MAX_PHASE_NUM; jj++)
					{
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sPPEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sNPEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sPNEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sNNEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealVolt[jj], sizeof(FORMAT07));
						nIndex += sizeof(FORMAT07);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealCurr[jj], sizeof(FORMAT25));
						nIndex += sizeof(FORMAT25);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealP[jj + 1], sizeof(FORMAT09));
						nIndex += sizeof(FORMAT09);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealQ[jj + 1], sizeof(FORMAT09));
						nIndex += sizeof(FORMAT09);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealPf[jj + 1], sizeof(FORMAT05));
						nIndex += sizeof(FORMAT05);
					}
                    memcpy(&nEveData[252], &nNum, sizeof(nNum));
                    //nIndex += sizeof(nNum);
                    MemWriteAcEvtProc(F9 + ii, &nEveData, LEN_AC_EVENT, 1);

					nNum = 0;
					MemReadAcXFlhBackData(F37 , &nNum, sizeof(nNum));//+ ii
					nNum ++;
					MemWriteAcXFlhBackData(F37 , &nNum, sizeof(nNum));//+ ii
					pEventStat->nLCurrFlag[ii] = 0x55;
					
					pDataTable->sRealInstant.bsRunStat[3 + ii] |= 0x0008;	//失流
                }
            }
        }
        else
        {
            AcEventTime[2][ii] = 0;
        }
        
        if (!((((pDataTable->fVolt[0] > (g_nVolt * LOSS_CURR_RATIO)) || (pDataTable->fVolt[1] > (g_nVolt * LOSS_CURR_RATIO)) || (pDataTable->fVolt[2] > (g_nVolt * LOSS_CURR_RATIO)))
			&& ((pDataTable->fCurr[0] > (RATED_CURR * RE_CURR)) || (pDataTable->fCurr[1] > (RATED_CURR * RE_CURR)) || (pDataTable->fCurr[2] > (RATED_CURR * RE_CURR))))
				&&(pDataTable->fCurr[ii] < RATED_CURR * OC_CURR)))
        {
            AcEventTime[2][ii] = 0;
            if (0x55 == pEventStat->nLCurrFlag[ii])
            {
                memset(&nEveData, 0, LEN_AC_EVENT);
                MemReadAcEvtProc(F9 + ii, 1, &nEveData, LEN_AC_EVENT);
                
                nIndex = 125;//109;
				memset(&sAcEnergy, 0, sizeof(MT_DGN_ENERGY_DATA));
				memset(&sAcTriEnergy, 0, sizeof(MT_DGN_TRI_ENERGY_DATA));
				//存恢复数据
				MemReadAcRealData(F1, &sAcEnergy, sizeof(MT_DGN_ENERGY_DATA));
                MemReadAcRealData(F3, &sAcTriEnergy, sizeof(MT_DGN_TRI_ENERGY_DATA));
                
                memcpy(&nEveData[nIndex], &pTime->nSec, 6);
    			nIndex += 6;
    			memcpy(&nEveData[nIndex], &sAcEnergy.sPPEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				memcpy(&nEveData[nIndex], &sAcEnergy.sNPEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				memcpy(&nEveData[nIndex], &sAcEnergy.sPNEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				memcpy(&nEveData[nIndex], &sAcEnergy.sNNEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				
				for (jj = 0; jj < MAX_PHASE_NUM; jj++)
				{
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sPPEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sNPEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sPNEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sNNEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
				}
                
                MemWriteAcEvtProc(F9 + ii, &nEveData, LEN_AC_EVENT, 0);
				pEventStat->nLCurrFlag[ii] = 0xAA;
				pDataTable->sRealInstant.bsRunStat[3 + ii] &= 0xfff7;
            }
        }
        if (0x55 == pEventStat->nLCurrFlag[ii])
		{
			nLossOfCurrTime = 0;
			MemReadAcXBackData(F10 + ii, &nLossOfCurrTime, sizeof(nLossOfCurrTime));
			nLossOfCurrTime++;
			MemWriteAcXBackData(F10 + ii, &nLossOfCurrTime, sizeof(nLossOfCurrTime));
		}
    }
}

/*
*****************************************************************************************************
* 函数名: AcCalcLossOfCurrent
* 输　入:
* 输  出:
* 返  回:
* 功能描述:断流事件处理
*****************************************************************************************************
*/
void AcCalcCurrentFailure(pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime)
{
    INT8U ii = 0;
	INT8U jj = 0;
	INT8U nIndex = 0;
	INT32U nNum;
	INT32U nLossOfCurrTime;
	INT8U nEveData[LEN_AC_EVENT];
	MT_DGN_ENERGY_DATA sAcEnergy;
	MT_DGN_TRI_ENERGY_DATA sAcTriEnergy;
    
    for(ii = 0; ii < MAX_PHASE_NUM; ii++)
    {
        if ((((pDataTable->fVolt[0] > (g_nVolt * FAIL_CURR_RATIO)) && (pDataTable->fVolt[1] > (g_nVolt * FAIL_CURR_RATIO)) && (pDataTable->fVolt[2] > (g_nVolt * FAIL_CURR_RATIO))))
				&&(pDataTable->fCurr[ii] < RATED_CURR * OC_CURR))   //			&& ((pDataTable->fCurr[0] > (RATED_CURR * RE_CURR)) || (pDataTable->fCurr[1] > (RATED_CURR * RE_CURR)) || (pDataTable->fCurr[2] > (RATED_CURR * RE_CURR)))
        {
            if (0x55 != pEventStat->nFCurrFlag[ii])
            {
                if (++AcEventTime[2][ii] > 3)
                {
                    memset(&nEveData, 0, LEN_AC_EVENT);
					memset(&sAcEnergy, 0, sizeof(MT_DGN_ENERGY_DATA));
					memset(&sAcTriEnergy, 0, sizeof(MT_DGN_TRI_ENERGY_DATA));
					//存发生数据
					MemReadAcRealData(F1, &sAcEnergy, sizeof(MT_DGN_ENERGY_DATA));
                    MemReadAcRealData(F3, &sAcTriEnergy, sizeof(MT_DGN_TRI_ENERGY_DATA));
                    
                    nNum = 0;
					MemReadAcXFlhBackData(F109 + ii, &nNum, sizeof(nNum));
					nNum ++;
					MemWriteAcXFlhBackData(F109 + ii, &nNum, sizeof(nNum));
                    
                    nIndex = 0;
					memcpy(&nEveData[nIndex], &pTime->nSec, 6);
    				nIndex += 6;
    				memcpy(&nEveData[nIndex], &sAcEnergy.sPPEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcEnergy.sNPEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcEnergy.sPNEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcEnergy.sNNEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
                    
                    for (jj = 0; jj < MAX_PHASE_NUM; jj++)
					{
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sPPEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sNPEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sPNEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sNNEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealVolt[jj], sizeof(FORMAT07));
						nIndex += sizeof(FORMAT07);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealCurr[jj], sizeof(FORMAT25));
						nIndex += sizeof(FORMAT25);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealP[jj + 1], sizeof(FORMAT09));
						nIndex += sizeof(FORMAT09);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealQ[jj + 1], sizeof(FORMAT09));
						nIndex += sizeof(FORMAT09);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealPf[jj + 1], sizeof(FORMAT05));
						nIndex += sizeof(FORMAT05);
					}
                    memcpy(&nEveData[252], &nNum, sizeof(nNum));
                    //nIndex += sizeof(nNum);
                    MemWriteAcEvtProc(F52 + ii, &nEveData, LEN_AC_EVENT, 1);
                    
					nNum = 0;
					MemReadAcXFlhBackData(F108 , &nNum, sizeof(nNum));//+ ii
					nNum ++;
					MemWriteAcXFlhBackData(F108 , &nNum, sizeof(nNum));//+ ii
					pEventStat->nFCurrFlag[ii] = 0x55;
					
					pDataTable->sRealInstant.bsRunStat[3 + ii] |= 0x0100;	//
                }
            }
        }
        else
        {
            AcEventTime[2][ii] = 0;
        }
        
        if (!((((pDataTable->fVolt[0] > (g_nVolt * FAIL_CURR_RATIO)) || (pDataTable->fVolt[1] > (g_nVolt * FAIL_CURR_RATIO)) || (pDataTable->fVolt[2] > (g_nVolt * FAIL_CURR_RATIO))))
				&&(pDataTable->fCurr[ii] < RATED_CURR * OC_CURR))) //			&& ((pDataTable->fCurr[0] > (RATED_CURR * RE_CURR)) || (pDataTable->fCurr[1] > (RATED_CURR * RE_CURR)) || (pDataTable->fCurr[2] > (RATED_CURR * RE_CURR)))
        {
            AcEventTime[2][ii] = 0;
            if (0x55 == pEventStat->nFCurrFlag[ii])
            {
                memset(&nEveData, 0, LEN_AC_EVENT);
                MemReadAcEvtProc(F9 + ii, 1, &nEveData, LEN_AC_EVENT);
                
                nIndex = 125;//109;
				memset(&sAcEnergy, 0, sizeof(MT_DGN_ENERGY_DATA));
				memset(&sAcTriEnergy, 0, sizeof(MT_DGN_TRI_ENERGY_DATA));
				//存恢复数据
				MemReadAcRealData(F1, &sAcEnergy, sizeof(MT_DGN_ENERGY_DATA));
                MemReadAcRealData(F3, &sAcTriEnergy, sizeof(MT_DGN_TRI_ENERGY_DATA));
                
                memcpy(&nEveData[nIndex], &pTime->nSec, 6);
    			nIndex += 6;
    			memcpy(&nEveData[nIndex], &sAcEnergy.sPPEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				memcpy(&nEveData[nIndex], &sAcEnergy.sNPEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				memcpy(&nEveData[nIndex], &sAcEnergy.sPNEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				memcpy(&nEveData[nIndex], &sAcEnergy.sNNEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				
				for (jj = 0; jj < MAX_PHASE_NUM; jj++)
				{
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sPPEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sNPEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sPNEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sNNEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
				}
                
                MemWriteAcEvtProc(F52 + ii, &nEveData, LEN_AC_EVENT, 0);
				pEventStat->nFCurrFlag[ii] = 0xAA;
				pDataTable->sRealInstant.bsRunStat[3 + ii] &= 0xfeff;
            }
        }
        if (0x55 == pEventStat->nFCurrFlag[ii])
		{
			nLossOfCurrTime = 0;
			MemReadAcXBackData(F10 + ii, &nLossOfCurrTime, sizeof(nLossOfCurrTime));
			nLossOfCurrTime++;
			MemWriteAcXBackData(F10 + ii, &nLossOfCurrTime, sizeof(nLossOfCurrTime));
		}
    }
}

/*
*****************************************************************************************************
* 函数名: AcCalcLossOfCurrent
* 输　入:
* 输  出:
* 返  回:
* 功能描述:过流事件处理
*****************************************************************************************************
*/
void AcCalcOverOfCurrent(pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime)
{
    INT8U ii = 0;
	INT8U jj = 0;
	INT8U nIndex = 0;
	INT32U nNum;
	INT32U nOverOfCurrTime;
	INT8U nEveData[LEN_AC_EVENT];
	MT_DGN_ENERGY_DATA sAcEnergy;
	MT_DGN_TRI_ENERGY_DATA sAcTriEnergy;
    
    for(ii = 0; ii < MAX_PHASE_NUM; ii++)
    {
        if (((pDataTable->fVolt[0] > (g_nVolt * LOSS_CURR_RATIO)) || (pDataTable->fVolt[1] > (g_nVolt * LOSS_CURR_RATIO)) || (pDataTable->fVolt[2] > (g_nVolt * LOSS_CURR_RATIO)))
			&& (pDataTable->fCurr[ii] > (MAX_CURR * OV_CURR)))
        {
            if (0x55 != pEventStat->nOCurrFlag[ii])
            {
                if (++AcEventTime[5][ii] > 3)
                {
                    memset(&nEveData, 0, LEN_AC_EVENT);
					memset(&sAcEnergy, 0, sizeof(MT_DGN_ENERGY_DATA));
					memset(&sAcTriEnergy, 0, sizeof(MT_DGN_TRI_ENERGY_DATA));
					//存发生数据
					MemReadAcRealData(F1, &sAcEnergy, sizeof(MT_DGN_ENERGY_DATA));
                    MemReadAcRealData(F3, &sAcTriEnergy, sizeof(MT_DGN_TRI_ENERGY_DATA));
                    
                    nNum = 0;
					MemReadAcXFlhBackData(F61 + ii, &nNum, sizeof(nNum));
					nNum ++;
					MemWriteAcXFlhBackData(F61 + ii, &nNum, sizeof(nNum));
                    
                    nIndex = 0;
					memcpy(&nEveData[nIndex], &pTime->nSec, 6);
    				nIndex += 6;
    				memcpy(&nEveData[nIndex], &sAcEnergy.sPPEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcEnergy.sNPEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcEnergy.sPNEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcEnergy.sNNEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
                    
                    for (jj = 0; jj < MAX_PHASE_NUM; jj++)
					{
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sPPEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sNPEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sPNEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sNNEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealVolt[jj], sizeof(FORMAT07));
						nIndex += sizeof(FORMAT07);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealCurr[jj], sizeof(FORMAT25));
						nIndex += sizeof(FORMAT25);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealP[jj + 1], sizeof(FORMAT09));
						nIndex += sizeof(FORMAT09);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealQ[jj + 1], sizeof(FORMAT09));
						nIndex += sizeof(FORMAT09);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealPf[jj + 1], sizeof(FORMAT05));
						nIndex += sizeof(FORMAT05);
					}
                    memcpy(&nEveData[252], &nNum, sizeof(nNum));
                    //nIndex += sizeof(nNum);
                    MemWriteAcEvtProc(F21 + ii, &nEveData, LEN_AC_EVENT, 1);

					nNum = 0;
					MemReadAcXFlhBackData(F60 , &nNum, sizeof(nNum));//+ ii
					nNum ++;
					MemWriteAcXFlhBackData(F60 , &nNum, sizeof(nNum));//+ ii
					pEventStat->nOCurrFlag[ii] = 0x55;
					
					pDataTable->sRealInstant.bsRunStat[3 + ii] |= 0x0010;	//过流
                }
            }
        }
        else
        {
            AcEventTime[5][ii] = 0;
        }
        
        if (((pDataTable->fVolt[0] > (g_nVolt * LOSS_CURR_RATIO)) || (pDataTable->fVolt[1] > (g_nVolt * LOSS_CURR_RATIO)) || (pDataTable->fVolt[2] > (g_nVolt * LOSS_CURR_RATIO)))
			&& (pDataTable->fCurr[ii] < (MAX_CURR * OV_CURR)))
        {
            AcEventTime[5][ii] = 0;
            if (0x55 == pEventStat->nOCurrFlag[ii])
            {
                memset(&nEveData, 0, LEN_AC_EVENT);
                MemReadAcEvtProc(F21 + ii, 1, &nEveData, LEN_AC_EVENT);
                
                nIndex = 125;//109;
				memset(&sAcEnergy, 0, sizeof(MT_DGN_ENERGY_DATA));
				memset(&sAcTriEnergy, 0, sizeof(MT_DGN_TRI_ENERGY_DATA));
				//存恢复数据
				MemReadAcRealData(F1, &sAcEnergy, sizeof(MT_DGN_ENERGY_DATA));
                MemReadAcRealData(F3, &sAcTriEnergy, sizeof(MT_DGN_TRI_ENERGY_DATA));
                
                memcpy(&nEveData[nIndex], &pTime->nSec, 6);
    			nIndex += 6;
    			memcpy(&nEveData[nIndex], &sAcEnergy.sPPEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				memcpy(&nEveData[nIndex], &sAcEnergy.sNPEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				memcpy(&nEveData[nIndex], &sAcEnergy.sPNEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				memcpy(&nEveData[nIndex], &sAcEnergy.sNNEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				
				for (jj = 0; jj < MAX_PHASE_NUM; jj++)
				{
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sPPEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sNPEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sPNEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sNNEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
				}
                
                MemWriteAcEvtProc(F21 + ii, &nEveData, LEN_AC_EVENT, 0);
				pEventStat->nOCurrFlag[ii] = 0xAA;
				pDataTable->sRealInstant.bsRunStat[3 + ii] &= 0xffef;
            }
        }
        if (0x55 == pEventStat->nOCurrFlag[ii])
		{
			nOverOfCurrTime = 0;
			MemReadAcXBackData(F19 + ii, &nOverOfCurrTime, sizeof(nOverOfCurrTime));
			nOverOfCurrTime++;
			MemWriteAcXBackData(F19 + ii, &nOverOfCurrTime, sizeof(nOverOfCurrTime));
		}
    }
}

/*
*****************************************************************************************************
* 函数名: AcCalcLossOfCurrent
* 输　入:
* 输  出:
* 返  回:
* 功能描述:潮流反向事件处理
*****************************************************************************************************
*/
void AcCalcReflow(pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime)
{
    INT8U ii = 0;
	INT8U jj = 0;
	INT8U nIndex = 0;
	INT32U nNum;
    INT32U	nTmp;
	INT32U nReflowTime;
	INT8U nEveData[LEN_AC_EVENT];
	MT_DGN_ENERGY_DATA sAcEnergy;
   	MT_DGN_TRI_ENERGY_DATA sAcTriEnergy;
    MEM_AC_REAL_DATA            sRealData;             
    pMEM_AC_REAL_DATA           pRealData = &sRealData;
    
    for(ii = 0; ii < MAX_PHASE_NUM; ii++)
    {
        MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
       // if((pDataTable->nPFlag&(0x1<<ii))&&(!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[2].nFrac0, 3, 0))))  
        if ((!(IsAllEqualVal(&pDataTable->sRealInstant.sRealP[ii + 1].nFrac0, 3, 0)))&&(pDataTable->nPFlag&0x08))
        {
            if (0x55 != pEventStat->nReflowFlag[ii])
            {
                if (++AcEventTime[6][ii] > 3)
                {
                    memset(&nEveData, 0, LEN_AC_EVENT);
					memset(&sAcEnergy, 0, sizeof(MT_DGN_ENERGY_DATA));
					memset(&sAcTriEnergy, 0, sizeof(MT_DGN_TRI_ENERGY_DATA));
					//存发生数据
					MemReadAcRealData(F1, &sAcEnergy, sizeof(MT_DGN_ENERGY_DATA));
                    MemReadAcRealData(F3, &sAcTriEnergy, sizeof(MT_DGN_TRI_ENERGY_DATA));
                    
                    nIndex = 0;
					memcpy(&nEveData[nIndex], &pTime->nSec, 6);
    				nIndex += 6;
    				memcpy(&nEveData[nIndex], &sAcEnergy.sPPEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcEnergy.sNPEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcEnergy.sPNEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcEnergy.sNNEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
                    
                    for (jj = 0; jj < MAX_PHASE_NUM; jj++)
					{
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sPPEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sNPEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sPNEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sNNEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealVolt[jj], sizeof(FORMAT07));
						nIndex += sizeof(FORMAT07);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealCurr[jj], sizeof(FORMAT25));
						nIndex += sizeof(FORMAT25);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealP[jj + 1], sizeof(FORMAT09));
						nIndex += sizeof(FORMAT09);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealQ[jj + 1], sizeof(FORMAT09));
						nIndex += sizeof(FORMAT09);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealPf[jj + 1], sizeof(FORMAT05));
						nIndex += sizeof(FORMAT05);
					}
                    
                    MemWriteAcEvtProc(F24 + ii, &nEveData, LEN_AC_EVENT, 1);
                    
                    nNum = 0;
					MemReadAcXFlhBackData(F70 + ii, &nNum, sizeof(nNum));
					nNum ++;
					MemWriteAcXFlhBackData(F70 + ii, &nNum, sizeof(nNum));
					
					nNum = 0;
					MemReadAcXFlhBackData(F69 + ii, &nNum, sizeof(nNum));
					nNum ++;
					MemWriteAcXFlhBackData(F69 + ii, &nNum, sizeof(nNum));
					pEventStat->nReflowFlag[ii] = 0x55;
					
					pDataTable->sRealInstant.bsRunStat[3 + ii] |= 0x0040;	//潮流反向
                }
            }
        }
        else
        {
            AcEventTime[6][ii] = 0;
        }
        
        if (!(pDataTable->nPFlag&0x08))
        {
            AcEventTime[6][ii] = 0;
            if (0x55 == pEventStat->nReflowFlag[ii])
            {
                memset(&nEveData, 0, LEN_AC_EVENT);
                MemReadAcEvtProc(F24 + ii, 1, &nEveData, LEN_AC_EVENT);
                
                nIndex = 109;
				memset(&sAcEnergy, 0, sizeof(MT_DGN_ENERGY_DATA));
				memset(&sAcTriEnergy, 0, sizeof(MT_DGN_TRI_ENERGY_DATA));
				//存恢复数据
				MemReadAcRealData(F1, &sAcEnergy, sizeof(MT_DGN_ENERGY_DATA));
                MemReadAcRealData(F3, &sAcTriEnergy, sizeof(MT_DGN_TRI_ENERGY_DATA));
                
                memcpy(&nEveData[nIndex], &pTime->nSec, 6);
    			nIndex += 6;
    			memcpy(&nEveData[nIndex], &sAcEnergy.sPPEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				memcpy(&nEveData[nIndex], &sAcEnergy.sNPEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				memcpy(&nEveData[nIndex], &sAcEnergy.sPNEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				memcpy(&nEveData[nIndex], &sAcEnergy.sNNEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				
				for (jj = 0; jj < MAX_PHASE_NUM; jj++)
				{
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sPPEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sNPEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sPNEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sNNEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
				}
                
                MemWriteAcEvtProc(F24 + ii, &nEveData, LEN_AC_EVENT, 0);
				pEventStat->nReflowFlag[ii] = 0xAA;
				pDataTable->sRealInstant.bsRunStat[3 + ii] &= 0xffbf;
            }
        }
        if (0x55 == pEventStat->nReflowFlag[ii])
		{
			nReflowTime = 0;
			MemReadAcXBackData(F23 + ii, &nReflowTime, sizeof(nReflowTime));
			nReflowTime++;
			MemWriteAcXBackData(F23 + ii, &nReflowTime, sizeof(nReflowTime));
		}
    }
}

/*
*****************************************************************************************************
* 函数名: AcCalcLossOfCurrent
* 输　入:
* 输  出:
* 返  回:
* 功能描述:过载事件处理
*****************************************************************************************************
*/
void AcCalcOverload(pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime)
{
    INT8U ii = 0;
	INT8U jj = 0;
	INT8U nIndex = 0;
	INT32U nNum;
    INT32U	nTmp = 0;
	INT32U nOverloadTime;
	INT8U nEveData[LEN_AC_EVENT];
	MT_DGN_ENERGY_DATA sAcEnergy;
   	MT_DGN_TRI_ENERGY_DATA sAcTriEnergy;
    MEM_AC_REAL_DATA            sRealData;             
    pMEM_AC_REAL_DATA           pRealData = &sRealData;

    for(ii = 0; ii < MAX_PHASE_NUM; ii++)
    {
        MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA));
        pDataTable->sRealInstant.sRealP[ii + 1].nSint &= ~0x80;
        nTmp = BCD2INT32U(&pDataTable->sRealInstant.sRealP[ii + 1].nFrac0, 6) * 1000;
        if (nTmp > (g_nVolt * MAX_CURR * OV_CURR))
        {
            if (0x55 != pEventStat->nOloadFlag[ii])
            {
                if (++AcEventTime[7][ii] > 3)
                {
                    memset(&nEveData, 0, LEN_AC_EVENT);
					memset(&sAcEnergy, 0, sizeof(MT_DGN_ENERGY_DATA));
					memset(&sAcTriEnergy, 0, sizeof(MT_DGN_TRI_ENERGY_DATA));
					//存发生数据
					MemReadAcRealData(F1, &sAcEnergy, sizeof(MT_DGN_ENERGY_DATA));
                    MemReadAcRealData(F3, &sAcTriEnergy, sizeof(MT_DGN_TRI_ENERGY_DATA));
                    
                     nNum = 0;
					MemReadAcXFlhBackData(F79 + ii, &nNum, sizeof(nNum));
					nNum ++;
					MemWriteAcXFlhBackData(F79 + ii, &nNum, sizeof(nNum));
                    
                    nIndex = 0;
					memcpy(&nEveData[nIndex], &pTime->nSec, 6);
    				nIndex += 6;
    				memcpy(&nEveData[nIndex], &sAcEnergy.sPPEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcEnergy.sNPEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcEnergy.sPNEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcEnergy.sNNEnergy[0], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
                    
                    for (jj = 0; jj < MAX_PHASE_NUM; jj++)
					{
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sPPEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sNPEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sPNEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						memcpy(&nEveData[nIndex], &sAcTriEnergy.sNNEnergy[jj], sizeof(FORMAT11));
						nIndex += sizeof(FORMAT11);
						
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealVolt[jj], sizeof(FORMAT07));
						nIndex += sizeof(FORMAT07);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealCurr[jj], sizeof(FORMAT25));
						nIndex += sizeof(FORMAT25);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealP[jj + 1], sizeof(FORMAT09));
						nIndex += sizeof(FORMAT09);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealQ[jj + 1], sizeof(FORMAT09));
						nIndex += sizeof(FORMAT09);
						memcpy(&nEveData[nIndex], &pDataTable->sRealInstant.sRealPf[jj + 1], sizeof(FORMAT05));
						nIndex += sizeof(FORMAT05);
					}
                    memcpy(&nEveData[252], &nNum, sizeof(nNum));
                    //nIndex += sizeof(nNum);
                    MemWriteAcEvtProc(F27 + ii, &nEveData, LEN_AC_EVENT, 1);
                    
					nNum = 0;
					MemReadAcXFlhBackData(F78 + ii, &nNum, sizeof(nNum));
					nNum ++;
					MemWriteAcXFlhBackData(F78 + ii, &nNum, sizeof(nNum));
					pEventStat->nOloadFlag[ii] = 0x55;
					
					pDataTable->sRealInstant.bsRunStat[3 + ii] |= 0x0020;	//过载
                }
            }
        }
        else
        {
            AcEventTime[7][ii] = 0;
        }
        
        if (nTmp < (g_nVolt * MAX_CURR * OV_CURR))
        {
            AcEventTime[7][ii] = 0;
            if (0x55 == pEventStat->nOloadFlag[ii])
            {
                memset(&nEveData, 0, LEN_AC_EVENT);
                MemReadAcEvtProc(F27 + ii, 1, &nEveData, LEN_AC_EVENT);
                
                nIndex = 125;//109;
				memset(&sAcEnergy, 0, sizeof(MT_DGN_ENERGY_DATA));
				memset(&sAcTriEnergy, 0, sizeof(MT_DGN_TRI_ENERGY_DATA));
				//存恢复数据
				MemReadAcRealData(F1, &sAcEnergy, sizeof(MT_DGN_ENERGY_DATA));
                MemReadAcRealData(F3, &sAcTriEnergy, sizeof(MT_DGN_TRI_ENERGY_DATA));
                
                memcpy(&nEveData[nIndex], &pTime->nSec, 6);
    			nIndex += 6;
    			memcpy(&nEveData[nIndex], &sAcEnergy.sPPEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				memcpy(&nEveData[nIndex], &sAcEnergy.sNPEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				memcpy(&nEveData[nIndex], &sAcEnergy.sPNEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				memcpy(&nEveData[nIndex], &sAcEnergy.sNNEnergy[0], sizeof(FORMAT11));
				nIndex += sizeof(FORMAT11);
				
				for (jj = 0; jj < MAX_PHASE_NUM; jj++)
				{
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sPPEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sNPEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sPNEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
					memcpy(&nEveData[nIndex], &sAcTriEnergy.sNNEnergy[jj], sizeof(FORMAT11));
					nIndex += sizeof(FORMAT11);
				}
                
                MemWriteAcEvtProc(F27 + ii, &nEveData, LEN_AC_EVENT, 0);
				pEventStat->nOloadFlag[ii] = 0xAA;
				pDataTable->sRealInstant.bsRunStat[3 + ii] &= 0xffdf;
            }
        }
        if (0x55 == pEventStat->nOloadFlag[ii])
		{
			nOverloadTime = 0;
			MemReadAcXBackData(F27 + ii, &nOverloadTime, sizeof(nOverloadTime));
			nOverloadTime++;
			MemWriteAcXBackData(F27 + ii, &nOverloadTime, sizeof(nOverloadTime));
		}
    }
}

void OverCurrentEvent()//过流事件
{
    INT8U nEveData2[6];
    INT8U ii = 0;
    INT8U jj = 0;
    INT8U nIndex = 0;
    INT32U nOverOfCurrTime;
    MT_OAD_DATA nOAD;
    
    //INT8U nEveData[7];
    MT_REPORT_DATA          sAcReport;
    MT_REPORT_DATA          sAcReport2;

    MT_DGN_ENERGY_DATA sAcEnergy;
    MT_DGN_TRI_ENERGY_DATA sAcTriEnergy;
    
    STD_TIME sTime;
    pSTD_TIME pTime=&sTime;
    memset(&sTime, 0, sizeof(STD_TIME));
    if (RET_ERR_OK != GetSysClk(&sTime))
    {
        return;
    }
    if(0xFF == g_SimOCPTrbFlag) 
        g_RealOCPTrbFlag = 0xFF;
      else if(0xBF == g_SimOCPTrbFlag)
        g_RealOCPTrbFlag =0xBF;
      
    memset(&nEveData2, 0, 7);  

    nEveData2[0] = BCD_TO_INT8U(pTime->nSec);
    nEveData2[1] = BCD_TO_INT8U(pTime->nMin);
    nEveData2[2] = BCD_TO_INT8U(pTime->nHour);
    nEveData2[3] = BCD_TO_INT8U(pTime->nDay);
    nEveData2[4] = BCD_TO_INT8U(pTime->nMon);
    nEveData2[5] = pTime->nYear;
            
    if(g_RealOCPTrbFlag == 0xFF)
    {
        if(0 == g_CoverRunFlag8)
        {
            g_CoverRunFlag8 = 1;
            
            for(ii = 0; ii < MAX_PHASE_NUM; ii++)
            {
                memset(&sAcReport, 0, sizeof(MT_REPORT_DATA));
                memset(&sAcEnergy, 0, sizeof(MT_DGN_ENERGY_DATA));
                memset(&sAcTriEnergy, 0, sizeof(MT_DGN_TRI_ENERGY_DATA));
                //存发生数据
                MemReadAcRealData(F1, &sAcEnergy, sizeof(MT_DGN_ENERGY_DATA));
                MemReadAcRealData(F3, &sAcTriEnergy, sizeof(MT_DGN_TRI_ENERGY_DATA));

                sAcReport.sOrderNum= 0x01;
                //事件发生时间
                memcpy(&sAcReport.sStartTimestamp, &nEveData2, 6);       //c存储此次发生时间
                memset(&sAcReport.sFinishTimestamp, 0xFE, 6);
                /*
                nIndex = 0;
                memcpy(&sAcReport.sPAData[nIndex], &sAcEnergy.sPPEnergy[0], sizeof(FORMAT11));
                nIndex += 1;
                memcpy(&sAcReport.sPAData[nIndex], &sAcEnergy.sNPEnergy[0], sizeof(FORMAT11));
                nIndex += 1;
                memcpy(&sAcReport.sPAData[nIndex], &sAcEnergy.sPNEnergy[0], sizeof(FORMAT11));
                nIndex += 1;
                memcpy(&sAcReport.sPAData[nIndex], &sAcEnergy.sNNEnergy[0], sizeof(FORMAT11));
                nIndex += 1;

                for (jj = 0; jj < MAX_PHASE_NUM; jj++)
                {
                    memcpy(&sAcReport.sPAData[nIndex], &sAcTriEnergy.sPPEnergy[jj], sizeof(FORMAT11));
                    nIndex += 1;
                    memcpy(&sAcReport.sPAData[nIndex], &sAcTriEnergy.sNPEnergy[jj], sizeof(FORMAT11));
                    nIndex += 1;
                    memcpy(&sAcReport.sPAData[nIndex], &sAcTriEnergy.sPNEnergy[jj], sizeof(FORMAT11));
                    nIndex += 1;
                    memcpy(&sAcReport.sPAData[nIndex], &sAcTriEnergy.sNNEnergy[jj], sizeof(FORMAT11));
                    nIndex += 1;

                    memcpy(&sAcReport.sPAData[nIndex], &pDataTable->sRealInstant.sRealVolt[jj], sizeof(FORMAT07));
                    nIndex += 1;
                    memcpy(&sAcReport.sPAData[nIndex], &pDataTable->sRealInstant.sRealCurr[jj], sizeof(FORMAT25));
                    nIndex += 1;
                    //memcpy(&sAcReport.sPAData[nIndex], &pDataTable->sRealInstant.sRealP[ii + 1], sizeof(FORMAT09));
                    //nIndex += 1;
                    //memcpy(&sAcReport.sPAData[nIndex], &pDataTable->sRealInstant.sRealQ[ii + 1], sizeof(FORMAT09));
                    //nIndex += 1;
                    //memcpy(&sAcReport.sPAData[nIndex], &pDataTable->sRealInstant.sRealPf[ii + 1], sizeof(FORMAT05));
                    //nIndex += 1;
                }
                */
                MemWriteAcEvtProc(F38 + ii, &sAcReport, sizeof(MT_REPORT_DATA), 1);
            }          
        }
    }
    else if(g_RealOCPTrbFlag == 0xBF)
    {
        if( 1 == g_CoverRunFlag8 ) 
        {    
            g_CoverRunFlag8 = 0;
            
            for(ii = 0; ii < MAX_PHASE_NUM; ii++)
            {
                memset(&sAcReport, 0, sizeof(MT_REPORT_DATA));
                MemReadAcEvtProc(F38 + ii, 1, &sAcReport, sizeof(MT_REPORT_DATA));
                
                nIndex = 0;
                memset(&sAcEnergy, 0, sizeof(MT_DGN_ENERGY_DATA));
                memset(&sAcTriEnergy, 0, sizeof(MT_DGN_TRI_ENERGY_DATA));
                //存恢复数据
                MemReadAcRealData(F1, &sAcEnergy, sizeof(MT_DGN_ENERGY_DATA));
                MemReadAcRealData(F3, &sAcTriEnergy, sizeof(MT_DGN_TRI_ENERGY_DATA));
                
                memcpy(&sAcReport.sFinishTimestamp, &nEveData2, 6);
                /*
                memcpy(&sAcReport.sPAFinshData[nIndex], &sAcEnergy.sPPEnergy[0], sizeof(FORMAT11));
                nIndex += 1;
                memcpy(&sAcReport.sPAFinshData[nIndex], &sAcEnergy.sNPEnergy[0], sizeof(FORMAT11));
                nIndex += 1;
                memcpy(&sAcReport.sPAFinshData[nIndex], &sAcEnergy.sPNEnergy[0], sizeof(FORMAT11));
                nIndex += 1;
                memcpy(&sAcReport.sPAFinshData[nIndex], &sAcEnergy.sNNEnergy[0], sizeof(FORMAT11));
                nIndex += 1;
                
                for (jj = 0; jj < MAX_PHASE_NUM; jj++)
                {
                    memcpy(&sAcReport.sPAFinshData[nIndex], &sAcTriEnergy.sPPEnergy[jj], sizeof(FORMAT11));
                    nIndex += 1;
                    memcpy(&sAcReport.sPAFinshData[nIndex], &sAcTriEnergy.sNPEnergy[jj], sizeof(FORMAT11));
                    nIndex += 1;
                    memcpy(&sAcReport.sPAFinshData[nIndex], &sAcTriEnergy.sPNEnergy[jj], sizeof(FORMAT11));
                    nIndex += 1;
                    memcpy(&sAcReport.sPAFinshData[nIndex], &sAcTriEnergy.sNNEnergy[jj], sizeof(FORMAT11));
                    nIndex += 1;
                    memcpy(&sAcReport.sPAFinshData[nIndex], &pDataTable->sRealInstant.sRealVolt[jj], sizeof(FORMAT07));
                    nIndex += 1;
                    memcpy(&sAcReport.sPAFinshData[nIndex], &pDataTable->sRealInstant.sRealCurr[jj], sizeof(FORMAT25));
                    nIndex += 1;
                }
                */
                MemWriteAcEvtProc(F38 + ii, &sAcReport, sizeof(MT_REPORT_DATA), 0);
            }
        }
    }
    
}

//开盖事件处理
void OpenMeterCover()
{
      if(0xBF == g_SimMeterCoverFlag) //开盖
        g_RealMeterCoverFlag = 0xBF;
      else if(0xFF == g_SimMeterCoverFlag)
        g_RealMeterCoverFlag =0xFF;
    
      if(g_RealMeterCoverFlag == 0xBF) 
      {
        if(0 == g_CoverRunFlag)
        {   
            g_CoverRunFlag = 1;

            GB698GetStartEventResp(F30,F41);
        }
      }
      else if(g_RealMeterCoverFlag == 0xFF)
      {
        if( 1 == g_CoverRunFlag ) 
        {    
            g_CoverRunFlag = 0;
            GB698GetFinshEventResp(F30,F41);
        } 
      }
    /*
	INT8U nIndex = 0;
	INT32U nNum;
    INT8U nEveData[LEN_AC_EVENT];
	MT_DGN_ENERGY_DATA sAcEnergy;
	MT_DGN_TRI_ENERGY_DATA sAcTriEnergy;        
    MT_DGN_Q_ENERGY_DATA sAcQEnergy;
    STD_TIME sTime;
    pSTD_TIME pTime=&sTime;
    memset(&sTime, 0, sizeof(STD_TIME));
    if (RET_ERR_OK != GetSysClk(&sTime))
	{
        return;
	}
    if(0xFF == g_SimMeterCoverFlag) //开盖
        g_RealMeterCoverFlag = 0xFF;
    else if(0xBF == g_SimMeterCoverFlag)
        g_RealMeterCoverFlag =0xBF;
        
    memset(&nEveData, 0, LEN_AC_EVENT);
    memset(&sAcEnergy, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcTriEnergy, 0, sizeof(MT_DGN_TRI_ENERGY_DATA));
    memset(&sAcQEnergy, 0, sizeof(MT_DGN_Q_ENERGY_DATA));
    //存放数据a;
    MemReadAcRealData(F1, &sAcEnergy, sizeof(MT_DGN_ENERGY_DATA));
    MemReadAcRealData(F2, &sAcQEnergy, sizeof(MT_DGN_Q_ENERGY_DATA));
    MemReadAcRealData(F3, &sAcTriEnergy, sizeof(MT_DGN_TRI_ENERGY_DATA));
    
    if(0xFF == g_RealMeterCoverFlag) 
    {
        if(0 == g_CoverRunFlag)
        {   
            g_CoverRunFlag = 1;
            
            nIndex = 0;
            //memcpy(&nEveData[nIndex], &pTime->nSec, 6);
            memcpy(&nEveData[nIndex], &g_SimMeterCoverTimeBuf[0], 6); 
            memset(&g_SimMeterCoverTimeBuf[0], 0, 6);
            nIndex += 12;
            
            memcpy(&nEveData[nIndex], &sAcEnergy.sPPEnergy[0], sizeof(FORMAT11));
            nIndex += sizeof(FORMAT11);
            memcpy(&nEveData[nIndex], &sAcEnergy.sNPEnergy[0], sizeof(FORMAT11));
            nIndex += sizeof(FORMAT11);
            
            memcpy(&nEveData[nIndex], &sAcQEnergy.sQ1Energy[0], sizeof(FORMAT11));
            nIndex += sizeof(FORMAT11);
            memcpy(&nEveData[nIndex], &sAcQEnergy.sQ2Energy[0], sizeof(FORMAT11));
            nIndex += sizeof(FORMAT11);
            memcpy(&nEveData[nIndex], &sAcQEnergy.sQ3Energy[0], sizeof(FORMAT11));
            nIndex += sizeof(FORMAT11);
            memcpy(&nEveData[nIndex], &sAcQEnergy.sQ4Energy[0], sizeof(FORMAT11));
            nIndex += sizeof(FORMAT11);
            
            MemWriteAcEvtProc(F30, &nEveData, LEN_AC_EVENT, 1);
                            
        }
    }
    else if ( 0xBF==g_RealMeterCoverFlag )
    {
       if( 1 == g_CoverRunFlag ) 
       {    
            memset(&nEveData, 0, LEN_AC_EVENT);
            MemReadAcEvtProc(F30,1, &nEveData, LEN_AC_EVENT);
            nIndex = 6;
            //memcpy(&nEveData[nIndex], &pTime->nSec, 6);
            memcpy(&nEveData[nIndex], &g_SimMeterCoverTimeBuf[0], 6); 
            memset(&g_SimMeterCoverTimeBuf[0], 0, 6);
            
            nIndex = 36;            
            memcpy(&nEveData[nIndex], &sAcEnergy.sPPEnergy[0], sizeof(FORMAT11));
            nIndex += sizeof(FORMAT11);
            memcpy(&nEveData[nIndex], &sAcEnergy.sNPEnergy[0], sizeof(FORMAT11));
            nIndex += sizeof(FORMAT11);
            
            memcpy(&nEveData[nIndex], &sAcQEnergy.sQ1Energy[0], sizeof(FORMAT11));
            nIndex += sizeof(FORMAT11);
            memcpy(&nEveData[nIndex], &sAcQEnergy.sQ2Energy[0], sizeof(FORMAT11));
            nIndex += sizeof(FORMAT11);
            memcpy(&nEveData[nIndex], &sAcQEnergy.sQ3Energy[0], sizeof(FORMAT11));
            nIndex += sizeof(FORMAT11);
            memcpy(&nEveData[nIndex], &sAcQEnergy.sQ4Energy[0], sizeof(FORMAT11));
            nIndex += sizeof(FORMAT11);
            
            nNum = 0;
            MemReadAcXFlhBackData(F86, &nNum, sizeof(nNum));
            nNum ++;
            MemWriteAcXFlhBackData(F86, &nNum, sizeof(nNum));
            
            MemWriteAcEvtProc(F30, &nEveData, LEN_AC_EVENT, 0);
            memset(&nEveData,0,sizeof(LEN_AC_EVENT));
            
            g_SimMeterCoverFlag=0;
            g_CoverRunFlag = 0;
       }
    }*/
    
}

/*
*****************************************************************************************************
* 函数名: TripEvent
* 输　入:
* 输  出:
* 返  回:
* 功能描述:电能表跳闸事件 22.2.16
*****************************************************************************************************
*/
void TripEvent()
{
    EVT_TRIP_DATA		    tEvtTrip;		//跳闸状态
    if((0xFF == g_SimMeterTripFlag)||(tEvtTrip.nEvtTrip == 1) )
        g_RealMeterTripFlag = 0xFF;
    else if((0xBF == g_SimMeterTripFlag)||(tEvtTrip.nEvtTrip == 0) )
        g_RealMeterTripFlag =0xBF;
            
    if(g_RealMeterTripFlag == 0xFF)
    {
        if(0 == g_CoverRunFlag4)
        {
            g_CoverRunFlag4 = 1;
            GB698GetStartEventResp(F31,F42);
        }
    }
    else if(g_RealMeterTripFlag == 0xBF)
    {
        if( 1 == g_CoverRunFlag4 ) 
        {    
            g_CoverRunFlag4 = 0;
            
            GB698GetFinshEventResp(F31,F42);
        }
    }
    
}

void GB698GetFinshEventResp(INT8U nFn,INT8U nFn2)
{
    //INT16U      nOI2;
    INT16U      nOI3;
    INT8U       i;
    INT8U       ii;
    INT8U       iii;
    INT8U       j;
    INT8U       jj;
    INT8U       nCount=0;
    INT8U       nCount2=0;
    MT_OAD_DATA nOAD;
    
    INT8U nEveData[6];
    MT_REPORT_DATA          sAcReport;
    MT_REPORT_DATA          sAcReport2;
    MT_DGN_ENERGY_DATA      sAcEnergy;
    //MT_DGN_TRI_ENERGY_DATA  sAcTriEnergy;        
    MT_DGN_Q_ENERGY_DATA    sAcQEnergy;
    STD_TIME sTime;
    pSTD_TIME pTime=&sTime;
    memset(&sTime, 0, sizeof(STD_TIME));
    if (RET_ERR_OK != GetSysClk(&sTime))
    {
        return;
    }
    memset(&nEveData, 0, sizeof(6)); 
    memset(&sAcEnergy, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&nOAD, 0, sizeof(MT_OAD_DATA));
    //memset(&sAcTriEnergy, 0, sizeof(MT_DGN_TRI_ENERGY_DATA));
    memset(&sAcQEnergy, 0, sizeof(MT_DGN_Q_ENERGY_DATA));
    memset(&sAcReport, 0, sizeof(MT_REPORT_DATA));
    memset(&sAcReport2, 0, sizeof(MT_REPORT_DATA));

    nEveData[0] = BCD_TO_INT8U(pTime->nSec);
    nEveData[1] = BCD_TO_INT8U(pTime->nMin);
    nEveData[2] = BCD_TO_INT8U(pTime->nHour);
    nEveData[3] = BCD_TO_INT8U(pTime->nDay);
    nEveData[4] = BCD_TO_INT8U(pTime->nMon);
    nEveData[5] = pTime->nYear;
    
    MemReadAcEvtProc(nFn,1, &sAcReport2, sizeof(sAcReport));
    //事件发生时间
    //memcpy(&sAcReport2.sStartTimestamp, &pTime->nSec, 6);
    memcpy(&sAcReport2.sFinishTimestamp, &nEveData, 6);
    MemReadAcEvtProc(nFn2,1, &nOAD, sizeof(MT_OAD_DATA));
    /*
    for(j =0;j<64;j++)
    {
        if((nOAD.sOAD[j].nD1 !=0x00)&&(nOAD.sOAD[j].nD2 !=0x00)&&(nOAD.sOAD[j].nD3 !=0x00))
        {
            nCount2++;
        }
    }
    /*
    if(nCount2==64)
    {
        memset(&nOAD, 0, sizeof(MT_OAD_DATA));
    }*/
    //nCount = nCount2;
    /*
    for(ii=0;ii<64;ii++)
    {
        nOI3 = (((INT16U)(nOAD.sOAD[ii].nD1 << 8) | nOAD.sOAD[ii].nD2));
        if((nOI3 == 0x00)&&(nOAD.sOAD[ii].nD3 == 0x00))
        {
            continue;
        }
        for(jj=0;jj<sizeof(g_n698FZDBOIt2Fn)/sizeof(g_n698FZDBOIt2Fn[0]);jj++)
        {
            if(g_n698FZDBOIt2Fn[jj].nDI ==nOI3)
            {
                switch(nOI3)
                {
                  case 0x0000:
                  case 0x0010:
                  case 0x0020: 
                      MemReadAcRealData(g_n698FZDBOIt2Fn[jj].nFn, &sAcEnergy, sizeof(MT_DGN_ENERGY_DATA));
                      //MemReadAcEvtProc(g_n698FZDBOIt2Fn[jj].nFn,1, &sAcEnergy,sizeof(MT_DGN_ENERGY_DATA));
                      if(nOI3== 0x0000)
                      {
                      
                      }
                      else if(nOI3== 0x0010)
                      {
                          if(nOAD.sOAD[ii].nD4 == 0)
                          {
                              for(i = 0; i <= MAX_FEE_NUM; i ++)
                              {
                                  memcpy(&sAcReport2.sPAFinshData[nCount], &sAcEnergy.sPPEnergy[i], sizeof(FORMAT11));
                                  nCount++;
                              }
                          }
                          else
                          {
                            iii = nOAD.sOAD[ii].nD4-1;
                            memcpy(&sAcReport2.sPAFinshData[nCount], &sAcEnergy.sPPEnergy[iii], sizeof(FORMAT11));
                            nCount++;
                          }
                      }
                      else if(nOI3== 0x0020)
                      {
                          if(nOAD.sOAD[ii].nD4 == 0)
                          {
                              for(i = 0; i <= MAX_FEE_NUM; i ++)
                              {
                                  memcpy(&sAcReport2.sPAFinshData[nCount], &sAcEnergy.sNPEnergy[i], sizeof(FORMAT11));
                                  nCount++;
                              }
                          }
                          else
                          {
                            iii = nOAD.sOAD[ii].nD4-1;
                            memcpy(&sAcReport2.sPAFinshData[nCount], &sAcEnergy.sPPEnergy[iii], sizeof(FORMAT11));
                            nCount++;
                          }
                      }
                      break;
                  case 0x0050:
                  case 0x0060:
                  case 0x0070:
                  case 0x0080:
                      MemReadAcRealData(g_n698FZDBOIt2Fn[jj].nFn, &sAcQEnergy, sizeof(MT_DGN_Q_ENERGY_DATA));
                      //MemReadAcEvtProc(g_n698FZDBOIt2Fn[jj].nFn,1, &sAcQEnergy,sizeof(MT_DGN_Q_ENERGY_DATA));
                      if(nOI3== 0x0050)
                      {
                          if(nOAD.sOAD[ii].nD4 == 0)
                          {
                              for(i = 0; i <= MAX_FEE_NUM; i ++)
                              {
                                  memcpy(&sAcReport2.sPAFinshData[nCount], &sAcQEnergy.sQ1Energy[i], sizeof(FORMAT11));
                                  nCount++;
                              }
                          }
                          else
                          {
                            iii = nOAD.sOAD[ii].nD4-1;
                            memcpy(&sAcReport2.sPAFinshData[nCount], &sAcQEnergy.sQ1Energy[iii], sizeof(FORMAT11));
                            nCount++;
                          }
                      }
                      else if(nOI3== 0x0060)
                      {
                          if(nOAD.sOAD[ii].nD4 == 0)
                          {
                              for(i = 0; i <= MAX_FEE_NUM; i ++)
                              {
                                  memcpy(&sAcReport2.sPAFinshData[nCount], &sAcQEnergy.sQ2Energy[i], sizeof(FORMAT11));
                                  nCount++;
                              }
                          }
                          else
                          {
                            iii = nOAD.sOAD[ii].nD4-1;
                            memcpy(&sAcReport2.sPAFinshData[nCount], &sAcQEnergy.sQ2Energy[iii], sizeof(FORMAT11));
                            nCount++;
                          }
                      }
                      else if(nOI3== 0x0070)
                      {
                          if(nOAD.sOAD[ii].nD4 == 0)
                          {
                              for(i = 0; i <= MAX_FEE_NUM; i ++)
                              {
                                  memcpy(&sAcReport2.sPAFinshData[nCount], &sAcQEnergy.sQ3Energy[i], sizeof(FORMAT11));
                                  nCount++;
                              }
                          }
                          else
                          {
                            iii = nOAD.sOAD[ii].nD4-1;
                            memcpy(&sAcReport2.sPAFinshData[nCount], &sAcQEnergy.sQ3Energy[iii], sizeof(FORMAT11));
                            nCount++;
                          }
                      }
                      else if(nOI3== 0x0080)
                      {
                          if(nOAD.sOAD[ii].nD4 == 0)
                          {
                              for(i = 0; i <= MAX_FEE_NUM; i ++)
                              {
                                  memcpy(&sAcReport2.sPAFinshData[nCount], &sAcQEnergy.sQ4Energy[i], sizeof(FORMAT11));
                                  nCount++;
                              }
                          }
                          else
                          {
                            iii = nOAD.sOAD[ii].nD4-1;
                            memcpy(&sAcReport2.sPAFinshData[nCount], &sAcQEnergy.sQ4Energy[iii], sizeof(FORMAT11));
                            nCount++;
                          }
                      }
                      
                      break;
                  case 0x2000:
                      if(nOAD.sOAD[ii].nD4 == 0)
                      {
                          for (i = 0; i < MAX_PHASE_NUM; i++)
                          {
                              memcpy(&sAcReport2.sPAFinshData[nCount], &pDataTable->sRealInstant.sRealVolt[i], sizeof(FORMAT07));
                              nCount++;
                          }
                      }
                      else
                      {
                          iii = nOAD.sOAD[ii].nD4-1;
                          memcpy(&sAcReport2.sPAFinshData[nCount], &pDataTable->sRealInstant.sRealVolt[iii], sizeof(FORMAT07));
                          nCount++;
                      }
                      break;
                  case 0x2001:
                      if(nOAD.sOAD[ii].nD4 == 0)
                      {
                          for (i = 0; i < MAX_PHASE_NUM; i++)
                          {
                              memcpy(&sAcReport2.sPAFinshData[nCount], &pDataTable->sRealInstant.sRealCurr[i], sizeof(FORMAT25));
                              nCount++;
                          }
                      }
                      else
                      {
                          iii = nOAD.sOAD[ii].nD4-1;
                          memcpy(&sAcReport2.sPAFinshData[nCount], &pDataTable->sRealInstant.sRealCurr[iii], sizeof(FORMAT25));
                          nCount++;
                      }
                      break;
                  default:
                      memcpy(&sAcReport2.sPAFinshData[nCount], 0, sizeof(FORMAT11));
                      nCount++;
                      break;
                }
            }
        }
    }*/
    MemWriteAcEvtProc(nFn, &sAcReport2, sizeof(sAcReport2), 0);
    memset(&sAcReport,0,sizeof(sAcReport2));
    memset(&sAcReport2,0,sizeof(sAcReport2));
}

void GB698GetStartEventResp(INT8U nFn,INT8U nFn2)
{
    //INT16U      nOI2;
    INT16U      nOI3;
    INT8U       i;
    INT8U       ii;
    INT8U       iii;
    INT8U       j;
    INT8U       jj;
    INT8U       nCount=0;
    //INT8U       nCount2=0;
    MT_OAD_DATA nOAD;
    
    INT8U nEveData[6];
    MT_REPORT_DATA          sAcReport;
    MT_REPORT_DATA          sAcReport2;
    MT_DGN_ENERGY_DATA      sAcEnergy;
     
    MT_DGN_Q_ENERGY_DATA    sAcQEnergy;
    STD_TIME sTime;
    pSTD_TIME pTime=&sTime;
    memset(&sTime, 0, sizeof(STD_TIME));
    if (RET_ERR_OK != GetSysClk(&sTime))
    {
        return;
    }
    memset(&nEveData, 0, sizeof(6)); 
    memset(&sAcEnergy, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&nOAD, 0, sizeof(MT_OAD_DATA));
    memset(&sAcQEnergy, 0, sizeof(MT_DGN_Q_ENERGY_DATA));
    memset(&sAcReport, 0, sizeof(MT_REPORT_DATA));
    memset(&sAcReport2, 0, sizeof(MT_REPORT_DATA));

    nEveData[0] = BCD_TO_INT8U(pTime->nSec);
    nEveData[1] = BCD_TO_INT8U(pTime->nMin);
    nEveData[2] = BCD_TO_INT8U(pTime->nHour);
    nEveData[3] = BCD_TO_INT8U(pTime->nDay);
    nEveData[4] = BCD_TO_INT8U(pTime->nMon);
    nEveData[5] = pTime->nYear;
   //事件上报序号32bit
    MemReadAcEvtProc(nFn,1, &sAcReport, sizeof(sAcReport));          //读取上一次的记录序号
    sAcReport2.sOrderNum = 1;              //加1
    //事件发生时间
    memcpy(&sAcReport2.sStartTimestamp, &nEveData, 6);       //c存储此次发生时间
    memset(&sAcReport2.sFinishTimestamp, 0xFE, 6);                  //FF为无效时间
    //时间发生源
    //sAcReport2.sReason = 0x00;                                     //  为NULL
    //事件上报状态
    //sAcReport2.sChannelState = 0x00;                               //待定， 485，载波等,
    //关联数据
    memset(&sAcReport,0,sizeof(sAcReport));
    MemReadAcEvtProc(nFn2,1, &nOAD, sizeof(MT_OAD_DATA));
    /*
    for(ii=0;ii<64;ii++)
    {
        nOI3 = (((INT16U)(nOAD.sOAD[ii].nD1 << 8) | nOAD.sOAD[ii].nD2));
        if((nOI3 == 0x00)&&(nOAD.sOAD[ii].nD3 == 0x00))
        {
            continue;
        }
        for(jj=0;jj<sizeof(g_n698FZDBOIt2Fn)/sizeof(g_n698FZDBOIt2Fn[0]);jj++)
        {
            if(g_n698FZDBOIt2Fn[jj].nDI ==nOI3)
            {
                switch(nOI3)
                {
                  case 0x0000:
                  case 0x0010:
                  case 0x0020: 
                      MemReadAcRealData(g_n698FZDBOIt2Fn[jj].nFn, &sAcEnergy, sizeof(MT_DGN_ENERGY_DATA));
                      //MemReadAcEvtProc(g_n698FZDBOIt2Fn[jj].nFn,1, &sAcEnergy,sizeof(MT_DGN_ENERGY_DATA));
                      if(nOI3== 0x0000)
                      {
                      
                      }
                      else if(nOI3== 0x0010)
                      {
                          if(nOAD.sOAD[ii].nD4 == 0)
                          {
                              for(i = 0; i <= MAX_FEE_NUM; i ++)
                              {
                                  memcpy(&sAcReport2.sPAData[nCount], &sAcEnergy.sPPEnergy[i], sizeof(FORMAT11));
                                  nCount++;
                              }
                          }
                          else
                          {
                            iii = nOAD.sOAD[ii].nD4-1;
                            memcpy(&sAcReport2.sPAData[nCount], &sAcEnergy.sPPEnergy[iii], sizeof(FORMAT11));
                            nCount++;
                          }
                      }
                      else if(nOI3== 0x0020)
                      {
                          if(nOAD.sOAD[ii].nD4 == 0)
                          {
                              for(i = 0; i <= MAX_FEE_NUM; i ++)
                              {
                                  memcpy(&sAcReport2.sPAData[nCount], &sAcEnergy.sNPEnergy[i], sizeof(FORMAT11));
                                  nCount++;
                              }
                          }
                          else
                          {
                            iii = nOAD.sOAD[ii].nD4-1;
                            memcpy(&sAcReport2.sPAData[nCount], &sAcEnergy.sPPEnergy[iii], sizeof(FORMAT11));
                            nCount++;
                          }
                      }
                      break;
                  case 0x0050:
                  case 0x0060:
                  case 0x0070:
                  case 0x0080:
                      MemReadAcRealData(g_n698FZDBOIt2Fn[jj].nFn, &sAcQEnergy, sizeof(MT_DGN_Q_ENERGY_DATA));
                      //MemReadAcEvtProc(g_n698FZDBOIt2Fn[jj].nFn,1, &sAcQEnergy,sizeof(MT_DGN_Q_ENERGY_DATA));
                      if(nOI3== 0x0050)
                      {
                          if(nOAD.sOAD[ii].nD4 == 0)
                          {
                              for(i = 0; i <= MAX_FEE_NUM; i ++)
                              {
                                  memcpy(&sAcReport2.sPAData[nCount], &sAcQEnergy.sQ1Energy[i], sizeof(FORMAT11));
                                  nCount++;
                              }
                          }
                          else
                          {
                            iii = nOAD.sOAD[ii].nD4-1;
                            memcpy(&sAcReport2.sPAData[nCount], &sAcQEnergy.sQ1Energy[iii], sizeof(FORMAT11));
                            nCount++;
                          }
                      }
                      else if(nOI3== 0x0060)
                      {
                          if(nOAD.sOAD[ii].nD4 == 0)
                          {
                              for(i = 0; i <= MAX_FEE_NUM; i ++)
                              {
                                  memcpy(&sAcReport2.sPAData[nCount], &sAcQEnergy.sQ2Energy[i], sizeof(FORMAT11));
                                  nCount++;
                              }
                          }
                          else
                          {
                            iii = nOAD.sOAD[ii].nD4-1;
                            memcpy(&sAcReport2.sPAData[nCount], &sAcQEnergy.sQ2Energy[iii], sizeof(FORMAT11));
                            nCount++;
                          }
                      }
                      else if(nOI3== 0x0070)
                      {
                          if(nOAD.sOAD[ii].nD4 == 0)
                          {
                              for(i = 0; i <= MAX_FEE_NUM; i ++)
                              {
                                  memcpy(&sAcReport2.sPAData[nCount], &sAcQEnergy.sQ3Energy[i], sizeof(FORMAT11));
                                  nCount++;
                              }
                          }
                          else
                          {
                            iii = nOAD.sOAD[ii].nD4-1;
                            memcpy(&sAcReport2.sPAData[nCount], &sAcQEnergy.sQ3Energy[iii], sizeof(FORMAT11));
                            nCount++;
                          }
                      }
                      else if(nOI3== 0x0080)
                      {
                          if(nOAD.sOAD[ii].nD4 == 0)
                          {
                              for(i = 0; i <= MAX_FEE_NUM; i ++)
                              {
                                  memcpy(&sAcReport2.sPAData[nCount], &sAcQEnergy.sQ4Energy[i], sizeof(FORMAT11));
                                  nCount++;
                              }
                          }
                          else
                          {
                            iii = nOAD.sOAD[ii].nD4-1;
                            memcpy(&sAcReport2.sPAData[nCount], &sAcQEnergy.sQ4Energy[iii], sizeof(FORMAT11));
                            nCount++;
                          }
                      }
                      
                      break;
                  case 0x2000:
                      if(nOAD.sOAD[ii].nD4 == 0)
                      {
                          for (i = 0; i < MAX_PHASE_NUM; i++)
                          {
                              memcpy(&sAcReport2.sPAData[nCount], &pDataTable->sRealInstant.sRealVolt[i], sizeof(FORMAT07));
                              nCount++;
                          }
                      }
                      else
                      {
                          iii = nOAD.sOAD[ii].nD4-1;
                          memcpy(&sAcReport2.sPAData[nCount], &pDataTable->sRealInstant.sRealVolt[iii], sizeof(FORMAT07));
                          nCount++;
                      }
                      break;
                  case 0x2001:
                      if(nOAD.sOAD[ii].nD4 == 0)
                      {
                          for (i = 0; i < MAX_PHASE_NUM; i++)
                          {
                              memcpy(&sAcReport2.sPAData[nCount], &pDataTable->sRealInstant.sRealCurr[i], sizeof(FORMAT25));
                              nCount++;
                          }
                      }
                      else
                      {
                          iii = nOAD.sOAD[ii].nD4-1;
                          memcpy(&sAcReport2.sPAData[nCount], &pDataTable->sRealInstant.sRealCurr[iii], sizeof(FORMAT25));
                          nCount++;
                      }
                      break;
                  default:
                      memcpy(&sAcReport2.sPAData[nCount], 0, sizeof(FORMAT11));
                      nCount++;
                      break;
                }
            }
        }
    }*/
    MemWriteAcEvtProc(nFn, &sAcReport2, sizeof(sAcReport2), 1);
    //memset(&sAcReport,0,sizeof(sAcReport));
    memset(&sAcReport2,0,sizeof(sAcReport2));
}
/*
void DIDEvent()//初始化数据库标识
{
  if(g_CoverRunFlag9 > 8)
  {
    //INT8U  nData[4] = {0x00,0x10,0x02,0x00};
    MT_OAD_DATA nOAD;
    //memset(&nOAD, 0, sizeof(MT_OAD_DATA));
    

    //memcpy(&nOAD.sOAD[0].nD1,&nData[0],4);
    memset(&nOAD, 0, sizeof(MT_OAD_DATA));
    MemWriteAcEvtProc(F41+g_CoverRunFlag9, &nOAD, sizeof(MT_OAD_DATA), 0);
    //MemWriteAcEvtProc(F42, &nOAD, sizeof(MT_OAD_DATA), 0);
    //MemWriteAcEvtProc(F43, &nOAD, sizeof(MT_OAD_DATA), 0);
    //MemWriteAcEvtProc(F44, &nOAD, sizeof(MT_OAD_DATA), 0);
    //MemWriteAcEvtProc(F45, &nOAD, sizeof(MT_OAD_DATA), 0);
    //MemWriteAcEvtProc(F46, &nOAD, sizeof(MT_OAD_DATA), 0);
    //MemWriteAcEvtProc(F47, &nOAD, sizeof(MT_OAD_DATA), 0);
    //MemWriteAcEvtProc(F48, &nOAD, sizeof(MT_OAD_DATA), 0);
    g_CoverRunFlag9 += 1; 
  }
}*/



//合闸事件
void SwitchOnEvent()
{ 
    EVT_TRIP_DATA		    tEvtTrip;		//跳闸状态
    if(0xFF == g_SimMeterSwitchOnFlag) 
        g_RealMeterSwitchOnFlag = 0xFF;
    else if(0xBF == g_SimMeterSwitchOnFlag)
        g_RealMeterSwitchOnFlag =0xBF;

    if(g_RealMeterSwitchOnFlag == 0xFF)
    {
        if( 0 == g_CoverRunFlag5)
        {
            g_CoverRunFlag5 = 1;
            GB698GetStartEventResp(F32,F43);
        } 
    }
    else if(g_RealMeterSwitchOnFlag == 0xBF)
    {
        if( 1 == g_CoverRunFlag5)
        {
            g_CoverRunFlag5 = 0;
            GB698GetFinshEventResp(F32,F43);
        }
    }
}

//电能表密钥更新事件(待修改；)
void SecretKeyEvent()
{
    INT8U nEveData[7];
    MT_REPORT_DATA          sAcReport;
    MT_REPORT_DATA          sAcReport2;
    STD_TIME sTime;
    pSTD_TIME pTime=&sTime;
    memset(&sTime, 0, sizeof(STD_TIME));
    memset(&nEveData, 0, sizeof(7)); 
    memset(&sAcReport, 0, sizeof(MT_REPORT_DATA));
    memset(&sAcReport2, 0, sizeof(MT_REPORT_DATA));
    
    if (RET_ERR_OK != GetSysClk(&sTime))
	{
        return;
	}
    
    if(g_SimMeterPasswordFlag)
    {
            //事件上报序号32bit
        MemReadAcEvtProc(F33,1, &sAcReport, sizeof(sAcReport));
        sAcReport2.sOrderNum = 1;
        //事件发生时间
        //nIndex = 4;
        nEveData[0] = BCD_TO_INT8U(pTime->nSec);
        nEveData[1] = BCD_TO_INT8U(pTime->nMin);
        nEveData[2] = BCD_TO_INT8U(pTime->nHour);
        nEveData[3] = BCD_TO_INT8U(pTime->nDay);
        nEveData[4] = BCD_TO_INT8U(pTime->nMon);
        nEveData[5] = pTime->nYear;
        memcpy(&sAcReport2.sStartTimestamp, &nEveData, 6);
        memcpy(&sAcReport2.sFinishTimestamp, &nEveData, 6);
        //时间发生源
        //sAcReport2.sReason = 0x00;
        //事件上报状态
        //sAcReport2.sChannelState = 0x00;
        MemWriteAcEvtProc(F33, &sAcReport2, sizeof(sAcReport), 1);
        memset(&sAcReport,0,sizeof(sAcReport));
        memset(&sAcReport2,0,sizeof(sAcReport2));
        g_SimMeterPasswordFlag = 0;
    }
    
}

//电能表负荷开关误动作事件
void LoadSwitchActEvent()
{
    if(0xFF == g_SimLoadSwitchAcFlag) 
        g_RealLoadSwitchAcFlag = 0xFF;
    else if(0xBF == g_SimLoadSwitchAcFlag)
        g_RealLoadSwitchAcFlag =0xBF;
    
    if(g_RealLoadSwitchAcFlag == 0xFF) //待改
    {
        if(0 == g_CoverRunFlag6)//待改
        {   
            g_CoverRunFlag6 = 1;

            GB698GetStartEventResp(F34,F45);
        }
    }
    else if(g_RealLoadSwitchAcFlag == 0xBF)
    {
        if( 1 == g_CoverRunFlag6 ) 
        {    
            g_CoverRunFlag6 = 0;
            GB698GetFinshEventResp(F34,F45);
        } 
    }  
}

//电能表电源异常事件
void PowerAbnlEvent()
{
    if(0xFF == g_SimPowerAbnlFlag) 
      g_RealPowerAbnlFlag = 0xFF;
    else if(0xBF == g_SimPowerAbnlFlag)
      g_RealPowerAbnlFlag =0xBF;
  
    if(g_RealPowerAbnlFlag == 0xFF) 
    {
        if(0 == g_CoverRunFlag7)
        {   
            g_CoverRunFlag7 = 1;

           GB698GetStartEventResp(F35,F46);
        }
    }
    else if(g_RealPowerAbnlFlag == 0xBF)
    {
        if( 1 == g_CoverRunFlag7 ) 
        {    
            g_CoverRunFlag7 = 0;
            GB698GetFinshEventResp(F35,F46);
        } 
    }  
}

//电能表时钟故障事件
void ClockTrbEvent()
{
    if(0xFF == g_SimClockTrbFlag) 
        g_RealClockTrbFlag = 0xFF;
    else if(0xBF == g_SimClockTrbFlag)
        g_RealClockTrbFlag =0xBF;
      
    if((g_RealClockTrbFlag == 0xFF)||(0x20==(g_MeterInsideWrong[0]&0x20))) //时钟故障)
    {
        if(0 == g_CoverRunFlag2)//待改
        {   
            g_CoverRunFlag2 = 1;

            GB698GetStartEventResp(F36,F47);
        }
    }
    else if((g_RealClockTrbFlag ==0xBF)||(0x20!=(g_MeterInsideWrong[0]&0x20)))
    {
        if( 1 == g_CoverRunFlag2 ) 
        {    
            g_CoverRunFlag2 = 0;
            GB698GetFinshEventResp(F36,F47);
        } 
    }
  
}

//电能表计量芯片故障事件
void ChipTrbEvent()
{
    if(0xFF == g_SimChipTrbFlag) 
        g_RealChipTrbFlag = 0xFF;
    else if(0xBF == g_SimChipTrbFlag)
        g_RealChipTrbFlag =0xBF;
    
    if(g_RealChipTrbFlag == 0xFF) //计量芯片故障
    {
        if(0 == g_CoverRunFlag3)
        {   
            g_CoverRunFlag3 = 1;
            GB698GetStartEventResp(F37,F48);
        }
    }
    else if(g_RealChipTrbFlag == 0xBF)
    {
        if( 1 == g_CoverRunFlag3 ) 
        {    
            g_CoverRunFlag3 = 0;
            GB698GetFinshEventResp(F37,F48);
        } 
    }
}

/*
*****************************************************************************************************
* 函数名: AcCalcClrMeterEve
* 输　入:
* 输  出:
* 返  回:
* 功能描述:电表清零事件处理
*****************************************************************************************************
*/
void AcCalcClrMeterEve(const INT8U* pData)
{
    INT8U ii = 0;
	INT8U nIndex = 0;
	INT32U nNum = 0;
	INT8U nEveData[LEN_AC_EVENT];       //QJBL
	MT_DGN_ENERGY_DATA sAcEnergy;
	MT_DGN_Q_ENERGY_DATA sAcQEnergy;
	MT_DGN_TRI_ENERGY_DATA sAcTriEnergy;
	MT_DGN_TRI_Q_EN_DATA sAcTriQEnergy;
	STD_TIME sTime;
    
    //获取系统时钟
    memset(&sTime, 0, sizeof(STD_TIME));
    if (RET_ERR_OK != GetSysClk(&sTime))
    {
        return;
    }
    
    memset(&nEveData, 0, LEN_AC_EVENT);
    memset(&sAcEnergy, 0, sizeof(MT_DGN_ENERGY_DATA));
    memset(&sAcQEnergy, 0, sizeof(MT_DGN_Q_ENERGY_DATA));
    memset(&sAcTriEnergy, 0, sizeof(MT_DGN_TRI_ENERGY_DATA));
    memset(&sAcTriQEnergy, 0, sizeof(MT_DGN_TRI_Q_EN_DATA));
    
    MemReadAcRealData(F1, &sAcEnergy, sizeof(MT_DGN_ENERGY_DATA));
    MemReadAcRealData(F2, &sAcTriEnergy, sizeof(MT_DGN_Q_ENERGY_DATA));
    MemReadAcRealData(F3, &sAcTriEnergy, sizeof(MT_DGN_TRI_ENERGY_DATA));
    MemReadAcRealData(F4, &sAcTriEnergy, sizeof(MT_DGN_TRI_Q_EN_DATA));
    
    memcpy(&nEveData[nIndex], &sTime.nSec, 6);
    nIndex += 6;
	memcpy(&nEveData[nIndex], pData, 4);
	nIndex += 4;
	//合相
	memcpy(&nEveData[nIndex], &sAcEnergy.sPPEnergy[0], sizeof(FORMAT11));
	nIndex += sizeof(FORMAT11);
	memcpy(&nEveData[nIndex], &sAcEnergy.sNPEnergy[0], sizeof(FORMAT11));
	nIndex += sizeof(FORMAT11);
	memcpy(&nEveData[nIndex], &sAcQEnergy.sQ1Energy[0], sizeof(FORMAT11));
	nIndex += sizeof(FORMAT11);
	memcpy(&nEveData[nIndex], &sAcQEnergy.sQ2Energy[0], sizeof(FORMAT11));
	nIndex += sizeof(FORMAT11);
	memcpy(&nEveData[nIndex], &sAcQEnergy.sQ3Energy[0], sizeof(FORMAT11));
	nIndex += sizeof(FORMAT11);
	memcpy(&nEveData[nIndex], &sAcQEnergy.sQ4Energy[0], sizeof(FORMAT11));
	nIndex += sizeof(FORMAT11);
	for (ii = 0; ii < MAX_PHASE_NUM; ii++)
	{
		memcpy(&nEveData[nIndex], &sAcTriEnergy.sPPEnergy[ii], sizeof(FORMAT11));
		nIndex += sizeof(FORMAT11);
		memcpy(&nEveData[nIndex], &sAcTriEnergy.sNPEnergy[ii], sizeof(FORMAT11));
		nIndex += sizeof(FORMAT11);
		memcpy(&nEveData[nIndex], &sAcTriQEnergy.sQ1Energy[ii], sizeof(FORMAT11));
		nIndex += sizeof(FORMAT11);
		memcpy(&nEveData[nIndex], &sAcTriQEnergy.sQ2Energy[ii], sizeof(FORMAT11));
		nIndex += sizeof(FORMAT11);
		memcpy(&nEveData[nIndex], &sAcTriQEnergy.sQ3Energy[ii], sizeof(FORMAT11));
		nIndex += sizeof(FORMAT11);
		memcpy(&nEveData[nIndex], &sAcTriQEnergy.sQ4Energy[ii], sizeof(FORMAT11));
		nIndex += sizeof(FORMAT11);
	}
    
    MemWriteAcEvtProc(F14, &nEveData, LEN_AC_EVENT, 1);
    
    MemReadAcXFlhBackData(F43, &nNum, sizeof(nNum));
	nNum ++;
	MemWriteAcXFlhBackData(F43, &nNum, sizeof(nNum));
}


/********************************************************************************
* 函数名:AcCalcDataInitProc
* 输　入:
* 输　出:
* 返  回:
* 功  能: AC数据清零
*********************************************************************************/
void AcCalcDataInitProc(void)
{
    INT8U   i       = 0;
    INT32U  nNum    = 0;
    INT8U   nCombEp;
    INT8U   nCombEq1;
    INT8U   nCombEq2;
    INT32U  nHFConst;
    INT32U  nACRunStat;
    INT32U	nPulseConst;
    INT32U	nCheckSumReset;
	INT32U	nCheckSumEntire;
    STD_TIME sTime;
    MEM_AC_REAL_DATA sRealData;
    MEM_AC_BACK_DATA sAcBackBuf;
    MEM_AC_BACK_FLH_DATA sAcBackFlash;

    nCombEp = sDataTable.nCombEp;
    nCombEq1 = sDataTable.nCombEq1;
    nCombEq2 = sDataTable.nCombEq2;
    nHFConst = sDataTable.nHFConst;
    nACRunStat = sDataTable.nACRunStat;
    nPulseConst = sDataTable.nPulseConst;
    nCheckSumReset = sDataTable.nCheckSumReset;
    nCheckSumEntire = sDataTable.nCheckSumEntire;
    
    memset(&sSavDataTable, 0, sizeof(sSavDataTable));
    memset(&sDataTable, 0, sizeof(sDataTable));
    
    sDataTable.nCombEp = nCombEp;
    sDataTable.nCombEq1 = nCombEq1;
    sDataTable.nCombEq2 = nCombEq2;
    sDataTable.nHFConst = nHFConst;
    sDataTable.nACRunStat = nACRunStat;
    sDataTable.nPulseConst = nPulseConst;
    sDataTable.nCheckSumReset = nCheckSumReset;
    sDataTable.nCheckSumEntire = nCheckSumEntire;
    sSavDataTable.nCombEp = sDataTable.nCombEp;
    sSavDataTable.nCombEq1 = sDataTable.nCombEq1;
    sSavDataTable.nCombEq2 = sDataTable.nCombEq2;
    sSavDataTable.nHFConst = sDataTable.nHFConst;
    sSavDataTable.nPulseConst = sDataTable.nPulseConst;
    sSavDataTable.nENSumChk = 0x55aa66bb;
    
    memset(&sRealData, 0, sizeof(MEM_AC_REAL_DATA));
    memset(&sAcBackBuf, 0, sizeof(sAcBackBuf));
    memset(&sAcBackFlash, 0, sizeof(sAcBackFlash));
    
    sRealData.nENChkSum = 0x55aa66bb;
    MemWriteAcRealData(FEND, &sRealData, sizeof(sRealData));
    
    MemWriteAcXFlhBackData(F48, &sRealData, sizeof(sRealData));
    
    //清除冻结数据（数据信息）
    for (i = 0; i < 6; i++)
    {
        MemDeleteACDayData(F1 + i);
    }
    //
    
    //清除事件数据
    for (i = 0; i < 17; i++)
    {
        if (i == F14)   //电表清零不清除
        {
            continue;
        }
        MemDeleteACEvent(F1 + i);
    }
    
    MemWriteAcXBackData(FEND, &sAcBackBuf, sizeof(MEM_AC_BACK_DATA));
    
    memset(&sTime, 0, sizeof(STD_TIME));
    //清次数
    for (i = 0; i < 17; i++)
    {
        if ((F25 + i) == F43)   //电表清零次数不清除
        {
            continue;
        }
        if (((F25 + i) == F26) || ((F25 + i) == F27))   //电表清零次数不清除
        {
            MemWriteAcXFlhBackData(F25 + i, &sTime, 6);
        }
        else
        {
            MemWriteAcXFlhBackData(F25 + i, &nNum, sizeof(nNum));
        }
        
    }
    
    //获取系统时钟
    memset(&sTime, 0, sizeof(STD_TIME));
    if (RET_ERR_OK != GetSysClk(&sTime))
    {
        return;
    }
    if(RET_ERR_OK != AcCalcPerChanged(&sTime))
	{
		pDataTable->nRateNo = 3;
	}
}

/*
*****************************************************************************************************
* 函数名: AcCalcClrDemandEve
* 输　入:
* 输  出:
* 返  回:
* 功能描述:需量清零事件处理
*****************************************************************************************************
*/
void AcCalcClrDemandEve(const INT8U* pData)
{
    INT8U nIndex = 0;
	INT32U nNum = 0;
	INT8U nEveData[LEN_AC_EVENT];       //QJBL
	MT_DGN_DEMAND_DATA sAcDemand;
	MT_DGN_Q_DE_DATA sAcQDemand;
	STD_TIME sTime;
    
    //获取系统时钟
    memset(&sTime, 0, sizeof(STD_TIME));
    if (RET_ERR_OK != GetSysClk(&sTime))
    {
        return;
    }
    memset(&nEveData, 0, LEN_AC_EVENT);
    memset(&sAcDemand, 0, sizeof(MT_DGN_DEMAND_DATA));
    memset(&sAcQDemand, 0, sizeof(MT_DGN_Q_DE_DATA));
    
    MemReadAcRealData(F7, &sAcDemand, sizeof(MT_DGN_DEMAND_DATA));
    MemReadAcRealData(F8, &sAcQDemand, sizeof(MT_DGN_Q_DE_DATA));
    
    memcpy(&nEveData[nIndex], &sTime.nSec, 6);
    nIndex += 6;
	memcpy(&nEveData[nIndex], pData, 4);
	nIndex += 4;
	//合相
	memcpy(&nEveData[nIndex], &sAcDemand.sPPDemand[0], sizeof(FORMAT23));
	nIndex += sizeof(FORMAT23);
	memcpy(&nEveData[nIndex], &sAcDemand.sPPDemandTime[0], sizeof(FORMAT15));
	nIndex += sizeof(FORMAT15);
	
	memcpy(&nEveData[nIndex], &sAcDemand.sNPDemand[0], sizeof(FORMAT23));
	nIndex += sizeof(FORMAT23);
	memcpy(&nEveData[nIndex], &sAcDemand.sNPDemandTime[0], sizeof(FORMAT15));
	nIndex += sizeof(FORMAT15);
	
	memcpy(&nEveData[nIndex], &sAcQDemand.sQ1Demand[0], sizeof(FORMAT23));
	nIndex += sizeof(FORMAT23);
	memcpy(&nEveData[nIndex], &sAcQDemand.sQ1DemandTime[0], sizeof(FORMAT15));
	nIndex += sizeof(FORMAT15);
	
	memcpy(&nEveData[nIndex], &sAcQDemand.sQ2Demand[0], sizeof(FORMAT23));
	nIndex += sizeof(FORMAT23);
	memcpy(&nEveData[nIndex], &sAcQDemand.sQ2DemandTime[0], sizeof(FORMAT15));
	nIndex += sizeof(FORMAT15);
	
	memcpy(&nEveData[nIndex], &sAcQDemand.sQ3Demand[0], sizeof(FORMAT23));
	nIndex += sizeof(FORMAT23);
	memcpy(&nEveData[nIndex], &sAcQDemand.sQ3DemandTime[0], sizeof(FORMAT15));
	nIndex += sizeof(FORMAT15);
	
	memcpy(&nEveData[nIndex], &sAcQDemand.sQ4Demand[0], sizeof(FORMAT23));
	nIndex += sizeof(FORMAT23);
	memcpy(&nEveData[nIndex], &sAcQDemand.sQ4DemandTime[0], sizeof(FORMAT15));
	nIndex += sizeof(FORMAT15);
    
    MemWriteAcEvtProc(F13, &nEveData, LEN_AC_EVENT, 0);
    
    MemReadAcXFlhBackData(F42, &nNum, sizeof(nNum));
	nNum ++;
	MemWriteAcXFlhBackData(F42, &nNum, sizeof(nNum));
}

/********************************************************************************
* 函数名:BackupAcData
* 输　入:
* 输  出:
* 返  回:错误编码
* 功  能:备份电表实时数据
*********************************************************************************/
void BackupAcData(void)
{
    INT8U	i;
    INT32U	nENSum = 0;
    MEM_AC_REAL_DATA sRealData;
    
    nENSum = 0x55aa66bb + CheckINT32USum((INT32U*)&pSavDataTable->sAcEnergy, 0, (sizeof(MT_DGN_ENERGY_DATA) + sizeof(MT_DGN_Q_ENERGY_DATA)
             + sizeof(MT_DGN_TRI_ENERGY_DATA) + sizeof(MT_DGN_TRI_Q_EN_DATA) + sizeof(MT_DGN_C_ENERGY_DATA))/4);
	if (pSavDataTable->nENSumChk != nENSum)
	{
		return;
	}
    memset(&sRealData, 0, sizeof(MEM_AC_REAL_DATA));
    memcpy(&sRealData.sAcEnergy, &pSavDataTable->sAcEnergy, sizeof(MT_DGN_ENERGY_DATA) + sizeof(MT_DGN_Q_ENERGY_DATA)
           + sizeof(MT_DGN_TRI_ENERGY_DATA) + sizeof(MT_DGN_TRI_Q_EN_DATA) + sizeof(MT_DGN_C_ENERGY_DATA));
    memcpy(&sRealData.sEPulse[0], &pSavDataTable->sEnergyData[0], 8*sizeof(ENERGY_DATA) + MAX_PHASE_NUM*sizeof(PH_ENERGY_DATA)
           +  MAX_PHASE_NUM*sizeof(PH_ENERGY_DATA) + sizeof(COMB_ENERGY_DATA));
    
    sRealData.nENChkSum = nENSum;
    for (i = 0; i < (MAX_P_Q_NUM + 4); i++)
   	{
   		 memcpy(&sRealData.nAcDemand[i][0], &pSavDataTable->sDemandData[i].nDemand[0], (MAX_FEE_NUM+1)*sizeof(INT32U));
   	}
    //需量示值及发生时间
    for(i=0;i<5;i++)
    {
        INT32U2BCD(pSavDataTable->sDemandData[0].nDemand[i],6,&sRealData.sAcDemand.sPPDemand[i].nFrac0);
        memcpy(&sRealData.sAcDemand.sPPDemandTime[i], &pSavDataTable->sDemandData[0].sTimeData[i], sizeof(FORMAT15));
        INT32U2BCD(pSavDataTable->sDemandData[1].nDemand[i],6,&sRealData.sAcDemand.sNPDemand[i].nFrac0);
        memcpy(&sRealData.sAcDemand.sNPDemandTime[i], &pSavDataTable->sDemandData[1].sTimeData[i], sizeof(FORMAT15));
        INT32U2BCD(pSavDataTable->sDemandData[2].nDemand[i],6,&sRealData.sAcDemand.sPNDemand[i].nFrac0);
        memcpy(&sRealData.sAcDemand.sPNDemandTime[i], &pSavDataTable->sDemandData[2].sTimeData[i], sizeof(FORMAT15));
        INT32U2BCD(pSavDataTable->sDemandData[3].nDemand[i],6,&sRealData.sAcDemand.sNNDemand[i].nFrac0);
        memcpy(&sRealData.sAcDemand.sNNDemandTime[i], &pSavDataTable->sDemandData[3].sTimeData[i], sizeof(FORMAT15));

        INT32U2BCD(pSavDataTable->sDemandData[4].nDemand[i],6,&sRealData.sAcQDemand.sQ1Demand[i].nFrac0);
        memcpy(&sRealData.sAcQDemand.sQ1DemandTime[i], &pSavDataTable->sDemandData[4].sTimeData[i], sizeof(FORMAT15));
        INT32U2BCD(pSavDataTable->sDemandData[5].nDemand[i],6,&sRealData.sAcQDemand.sQ2Demand[i].nFrac0);
        memcpy(&sRealData.sAcQDemand.sQ2DemandTime[i], &pSavDataTable->sDemandData[5].sTimeData[i], sizeof(FORMAT15));
        INT32U2BCD(pSavDataTable->sDemandData[6].nDemand[i],6,&sRealData.sAcQDemand.sQ3Demand[i].nFrac0);
        memcpy(&sRealData.sAcQDemand.sQ3DemandTime[i], &pSavDataTable->sDemandData[6].sTimeData[i], sizeof(FORMAT15));
        INT32U2BCD(pSavDataTable->sDemandData[7].nDemand[i],6,&sRealData.sAcQDemand.sQ4Demand[i].nFrac0);
        memcpy(&sRealData.sAcQDemand.sQ4DemandTime[i], &pSavDataTable->sDemandData[7].sTimeData[i], sizeof(FORMAT15));
    }
    MemWriteAcXFlhBackData(F48, &sRealData, sizeof(sRealData));
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: AC需量清零处理
*********************************************************************************/
void AcCalcDemInitProc(void)
{
    MEM_AC_RDAY_DATA sAcRDayData;
    STD_TIME sTime;
	MT_DGN_DEMAND_DATA sAcDemand;          //需量数据
    MT_DGN_Q_DE_DATA sAcQDemand;         //分象限需量数据
    INT32U nAcDemand[MAX_P_Q_NUM+4][MAX_FEE_NUM+1];
    
    memset(&sTime, 0, sizeof(STD_TIME));
    if (RET_ERR_OK == GetSysClk(&sTime))
    {
        memset(&sAcRDayData, 0, sizeof(MEM_AC_RDAY_DATA));
        
        MemReadAcRealData(F1, &sAcRDayData.sAcEnergy, sizeof(MT_DGN_ENERGY_DATA));
        MemReadAcRealData(F2, &sAcRDayData.sAcQEnergy, sizeof(MT_DGN_Q_ENERGY_DATA));
        MemReadAcRealData(F3, &sAcRDayData.sAcTriEnergy, sizeof(MT_DGN_TRI_ENERGY_DATA));
        MemReadAcRealData(F4, &sAcRDayData.sAcTriQEnergy, sizeof(MT_DGN_TRI_Q_EN_DATA));
        MemReadAcRealData(F7, &sAcRDayData.sAcDemand, sizeof(MT_DGN_DEMAND_DATA));
        MemReadAcRealData(F8, &sAcRDayData.sAcQDemand, sizeof(MT_DGN_Q_DE_DATA));
        
        memcpy(&sAcRDayData.sRealP[0], &pDataTable->sRealInstant.sRealP[0], (MAX_PHASE_NUM + 1) * sizeof(FORMAT09));
    	memcpy(&sAcRDayData.sRealQ[0], &pDataTable->sRealInstant.sRealQ[0], (MAX_PHASE_NUM + 1) * sizeof(FORMAT09));
    	memcpy(&sAcRDayData.sTime, &sTime.nMin, sizeof(FORMAT15));
        
        MemWriteAcHistDataProc(F2, 0, &sAcRDayData, sizeof(MEM_AC_RDAY_DATA),1);
        
        memset(&sAcDemand, 0, sizeof(sAcDemand));
    	memset(&sAcQDemand, 0, sizeof(sAcQDemand));
        
        MemWriteAcRealData(F7, &sAcDemand, sizeof(MT_DGN_DEMAND_DATA));
        MemWriteAcRealData(F8, &sAcQDemand, sizeof(MT_DGN_Q_DE_DATA));
        
        memset(&pSavDataTable->sDemandData[0], 0, (MAX_P_Q_NUM + 4) * sizeof(AC_DEMAND_DATA));
        memset(&pDataTable->sDemandSlipData1[0], 0, (MAX_P_Q_NUM + 4) * sizeof(DemandSlip_Data));
		memset(&pDataTable->sDemandSlipData2[0], 0, (MAX_P_Q_NUM + 4) * sizeof(DemandSlip_Data));
        
        BackupAcData();
    }
}

/*
*****************************************************************************************************
* 函数名: AcCalcClrEventEve
* 输　入:
* 输  出:
* 返  回:
* 功能描述:事件清零事件处理
*****************************************************************************************************
*/
void AcCalcClrEventEve(const INT8U* pData)
{
    INT8U nIndex = 0;
	INT32U nNum = 0;
	INT8U nEveData[LEN_AC_EVENT];       //QJBL
	
	STD_TIME sTime;
    
    memset(&nEveData, 0, LEN_AC_EVENT);
	//获取系统时钟
    memset(&sTime, 0, sizeof(STD_TIME));
    if (RET_ERR_OK != GetSysClk(&sTime))
    {
        return;
    }
    
    memcpy(&nEveData[nIndex], &sTime.nSec, 6);
    nIndex += 6;
	memcpy(&nEveData[nIndex], pData, 8);
	nIndex += 8;
    
    MemWriteAcEvtProc(F15, &nEveData, LEN_AC_EVENT, 0);
    
    MemReadAcXFlhBackData(F44, &nNum, sizeof(nNum));
	nNum ++;
	MemWriteAcXFlhBackData(F44, &nNum, sizeof(nNum));

}

/********************************************************************************
* 函数名:AcCalcEventInitProc
* 输　入:
* 输　出:
* 返  回:
* 功  能: AC事件清零
*********************************************************************************/
INT8S AcCalcEventInitProc(const INT8U* pData)
{
    INT8S nSuccflag = RET_ERR_OK;
	INT32U nDI = 0;
	INT32U nNum = 0;
	INT32U nTime = 0;
    INT8U   i    = 0;
	STD_TIME sSyTime;
	MEM_AC_BACK_DATA sAcBackBuf;
    MEM_AC_BACK_FLH_DATA sAcBackFlash;
    
    
    memset(&sSyTime, 0, sizeof(STD_TIME));
	nDI = ((pData[3]<<24)|(pData[2]<<16)|(pData[1]<<8)|pData[0]);
	switch(nDI)
    {
        case 0xffffffff:
        {
            for (i = 0; i < 17; i++)
            {
                if ((i == F14) || (i == F15))   //电表清零、事件清零不清除
                {
                    continue;
                }
                MemDeleteACEvent(F1 + i);
            }
            memset(&sAcBackBuf,   0, sizeof(sAcBackBuf));
    		memset(&sAcBackFlash, 0, sizeof(sAcBackFlash));
            
            MemWriteAcXBackData(FEND, &sAcBackBuf, sizeof(MEM_AC_BACK_DATA));
            
            //清次数
            for (i = 0; i < 17; i++)
            {
                if (((F25 + i) == F43) || ((F25 + i) == F44))   //电表清零次数、事件清零次数不清除
                {
                    continue;
                }
                MemWriteAcXFlhBackData(F25 + i, &nNum, sizeof(nNum));
            }
            break;
        }
        case 0x030100ff:
        {
            MemWriteAcXBackData(F2, &nTime, sizeof(nTime));
            MemWriteAcXFlhBackData(F28, &nTime, sizeof(nTime));
            MemWriteAcXBackData(F2, &nTime, sizeof(nTime));
            MemWriteAcXFlhBackData(F29, &nTime, sizeof(nTime));
            MemWriteAcXBackData(F2, &nTime, sizeof(nTime));
            MemWriteAcXFlhBackData(F28, &nTime, sizeof(nTime));
            break;
        }
        
        case 0x030101ff:
        {
            MemDeleteACEvent(F1);
            break;
        }
        //还有分项的清除
        default:
			nSuccflag = RET_ERR_ERROR;
			break;
        
    }
    return nSuccflag;
}



/*
*****************************************************************************************************
* 函数名:AcCalcReadValid
* 输　入:INT32U uConstant, INT8U uR_ID, INT32U uData
* 输  出:
* 返  回:
* 功能描述:将从7022读到的原始数据转换成计算所需要的格式
*****************************************************************************************************
*/
FP32 AcCalcReadValid(INT32U uConstant, INT32U uHFConst, INT8U uR_ID, INT32U uData)
{
	INT32S	sDat7022;
	FP32	fDat1 = 0.0;

	sDat7022 = uData;//read_7022(uR_ID);
	
	if ((0 == uConstant) || (0 == uHFConst))
	{
		return (0);
	}
    
    
    //补码转换
	if(sDat7022&0x00800000)
	{
		sDat7022 = 0x01000000-sDat7022;
	}
	


	if(0 == sDat7022)
	{
		return (0);
	}

	//不同类型数据处理
	switch(uR_ID)
	{
		case r_UaRms:
		case r_UbRms:
		case r_UcRms:
		case r_Freq:				//线频率转换
			fDat1 = (FP32)sDat7022;
			fDat1 /= (1024*8);		//有效值转换
			break;
                 
		case r_IaRms:
		case r_IbRms:
		case r_IcRms:
		case r_ItRms:
			fDat1 = (FP32)sDat7022;
			fDat1 /= (1024*8);		//有效值转换
                  if(SINGLEPHASE_METER == g_nHardType)
                  {
                    fDat1 *= 6.7;
                  }
                  if(TRIPHASE_METER_220ZT == g_nHardType)
                  {
                      fDat1 *= 3.35;
                  }
			break;
    	
		case r_Pa:
		case r_Pb:
		case r_Pc:
		case r_Qa:
		case r_Qb:
		case r_Qc:
		case r_Sa:
		case r_Sb:
		case r_Sc:
          //补码转换
	        if(sDat7022&0x00800000)
	        {
		        sDat7022 = sDat7022-0x01000000;
	        }
			fDat1 = (FP32)sDat7022;
			fDat1 = fDat1*25920000000/8388608;		//分相功率转换
			fDat1 = fDat1/((FP32)uConstant);
			fDat1 = fDat1/((FP32)uHFConst);
			fDat1 = fDat1/1000;
                  /*#if  SINGLEPHASE_METER
                        fDat1 = fDat1 * 7;
                  #endif*/
			break;
		case r_Pt:
		case r_Qt:
		case r_St:
            if(sDat7022&0x00800000)
	        {
		        sDat7022 = sDat7022-0x01000000;
	        }
			fDat1 = (FP32)sDat7022;
			fDat1 = fDat1*2*25920000000/8388608;		//合相功率转换
			fDat1 = fDat1/((FP32)uConstant);
			fDat1 = fDat1/((FP32)uHFConst);
			fDat1 = fDat1/1000;
                   /*#if  SINGLEPHASE_METER
                        fDat1 = fDat1 * 7;
                    #endif*/
			break;
    	
		case r_Pfa:
		case r_Pfb:
		case r_Pfc:
		case r_Pft:
            if(sDat7022&0x00800000)
	        {
		        sDat7022 = sDat7022-0x01000000;
	        }
			fDat1 = (FP32)sDat7022;
			fDat1 /= (1024*1024*8);		//功率因数转换
			break;
			
    	case r_YUaUb:
		case r_YUaUc:
		case r_YUbUc:				//电压夹角转换
		case r_Pga:
		case r_Pgb:
		case r_Pgc:
             if(sDat7022&0x00800000)
	        {
		        sDat7022 = sDat7022-0x01000000;
	        }
			fDat1 = (FP32)sDat7022;
			fDat1 /= (1024*1024);
			fDat1 *= 180;
			break;
    case r_TempD:
            if(sDat7022&0x00000080)
	        {
		        sDat7022 = sDat7022-0x0100;
	        }
            fDat1 = (FP32)sDat7022;
            break;
		default:
			fDat1 = (FP32)sDat7022;
			break;
	}
	return (fDat1);
}

/****************************************************************************
* 函数名:CheckINT32USum
* 输　入:INT8U*       pData             缓冲数据
*        INT16U       nStart            计算效验的开始点
*        INT16U       nLeng             计算长度
* 输　出:
* 返  回:效验和
* 功  能:计算效验和
* 版本：Version 1.0
****************************************************************************/
INT32U CheckINT32USum(const INT32U *pData, INT16U nStart, INT16U nLen)
{
	INT32U nCs = 0;
	INT16U i  = 0;

    if(NULL == pData)
    {
        return 0;
    }
	for(i = nStart; i < (nStart + nLen); i++)
	{
		nCs = nCs + pData[i];
	}

    return nCs;
}

/********************************************************************************
* 函数名:ReadAcENData
* 输　入:
* 输  出:
* 返  回:错误编码
* 功  能:读出实时电能数据至交采临时区并进行BCD与BIN转换
*********************************************************************************/
INT8S ReadAcENData(void)
{
    INT8U	ii;
    INT8U   nErr = 0;
   	INT8S   nRet = RET_ERR_ERROR;
    INT32U  nENSum = 0;
    BOOLEAN bFramZero = FALSE;
    BOOLEAN bFlashZero = FALSE;
    MEM_AC_REAL_DATA sRealData;
    INT8U   nACRunStat[LEN_RUN_STAT_FLAG];

    memset(&nACRunStat[0], 0, sizeof(nACRunStat));
    
    pSavDataTable->nENSumChk = 0;
	memset(&sRealData, 0, sizeof(MEM_AC_REAL_DATA));
    
    for (ii = 0; ii < 5; ii++)
    {
        if (MemReadAcRealData(FEND, &sRealData, sizeof(sRealData)) > 0)
        {
            nRet = RET_ERR_OK;
        }
    	if(RET_ERR_OK == nRet)
    	{
    		break;
    	}
    	else
    	{
    		OSSemPend(g_pSem[SEM_COL_AC_CALC_IDX], 0, &nErr);
    	}
    }
    if(RET_ERR_OK == nRet)
    {
        if (TEST_BIT(pDataTable->nACRunStat, 3))
        {
            CLR_BIT(pDataTable->nACRunStat, 3);
        }
        memcpy(&pSavDataTable->sAcEnergy, &sRealData.sAcEnergy, sizeof(MT_DGN_ENERGY_DATA) + sizeof(MT_DGN_Q_ENERGY_DATA)
               + sizeof(MT_DGN_TRI_ENERGY_DATA) + sizeof(MT_DGN_TRI_Q_EN_DATA) + sizeof(MT_DGN_C_ENERGY_DATA));
        memcpy(&pSavDataTable->sEnergyData[0], &sRealData.sEPulse[0], 8*sizeof(ENERGY_DATA) + MAX_PHASE_NUM*sizeof(PH_ENERGY_DATA)
               +  MAX_PHASE_NUM*sizeof(PH_ENERGY_DATA) + sizeof(COMB_ENERGY_DATA));
        nENSum = 0x55aa66bb + CheckINT32USum((INT32U*)&pSavDataTable->sAcEnergy, 0, (sizeof(MT_DGN_ENERGY_DATA) + sizeof(MT_DGN_Q_ENERGY_DATA)
                + sizeof(MT_DGN_TRI_ENERGY_DATA) + sizeof(MT_DGN_TRI_Q_EN_DATA) + sizeof(MT_DGN_C_ENERGY_DATA))/4);
#if 0
        if (IsAllEqualVal((INT8U*)&sRealData.sAcEnergy, sizeof(MT_DGN_ENERGY_DATA) + sizeof(MT_DGN_Q_ENERGY_DATA) + sizeof(MT_DGN_TRI_ENERGY_DATA)
                    + sizeof(MT_DGN_TRI_Q_EN_DATA) + sizeof(MT_DGN_C_ENERGY_DATA), 0))
        {
            //bFramZero = TRUE;
        }
        if ((nENSum != sRealData.nENChkSum) || (bFramZero))
        {
            if (nENSum != sRealData.nENChkSum)
            {
                MemReadFlhXmpData(F8, 0, &nACRunStat[0], sizeof(nACRunStat));
                if (!TestBufBit(nACRunStat, 8, 1))
                {
                    SetBufBit(nACRunStat, 8, 1);
                    MemWriteFlhXmpData(F8, 0, &nACRunStat[0], sizeof(nACRunStat));
                }
                if (!TEST_BIT(pDataTable->nACRunStat, 1))
                {
                    SET_BIT(pDataTable->nACRunStat, 1);
                }
            }
            else
            {
                
            }
            
            nENSum = 0;
            memset(&sRealData, 0, sizeof(MEM_AC_REAL_DATA));
            for(ii = 0; ii < 5; ii++)
            {
                if (MemReadAcXFlhBackData(F48, &sRealData, sizeof(sRealData)) > 0)
                {
                    nRet = RET_ERR_OK;
                }
                if(RET_ERR_OK == nRet)
                {
                    break;
                }
                else
                {
                    OSSemPend(g_pSem[SEM_COL_AC_CALC_IDX], 0, &nErr);
                }
            }
            if (RET_ERR_OK == nRet)
            {
                if (TEST_BIT(pDataTable->nACRunStat, 4))
                {
                    CLR_BIT(pDataTable->nACRunStat, 4);
                }
                memcpy(&pSavDataTable->sAcEnergy, &sRealData.sAcEnergy, sizeof(MT_DGN_ENERGY_DATA) + sizeof(MT_DGN_Q_ENERGY_DATA)
                       + sizeof(MT_DGN_TRI_ENERGY_DATA) + sizeof(MT_DGN_TRI_Q_EN_DATA) + sizeof(MT_DGN_C_ENERGY_DATA));
	            memcpy(&pSavDataTable->sEnergyData[0], &sRealData.sEPulse[0], 8*sizeof(ENERGY_DATA) + MAX_PHASE_NUM*sizeof(PH_ENERGY_DATA)
                       +  MAX_PHASE_NUM*sizeof(PH_ENERGY_DATA) + sizeof(COMB_ENERGY_DATA));

                nENSum = 0x55aa66bb + CheckINT32USum((INT32U*)&pSavDataTable->sAcEnergy, 0, (sizeof(MT_DGN_ENERGY_DATA) + sizeof(MT_DGN_Q_ENERGY_DATA)
                         + sizeof(MT_DGN_TRI_ENERGY_DATA) + sizeof(MT_DGN_TRI_Q_EN_DATA) + sizeof(MT_DGN_C_ENERGY_DATA))/4);
                if (IsAllEqualVal((INT8U*)&sRealData.sAcEnergy, sizeof(MT_DGN_ENERGY_DATA) + sizeof(MT_DGN_Q_ENERGY_DATA) + sizeof(MT_DGN_TRI_ENERGY_DATA)
                            + sizeof(MT_DGN_TRI_Q_EN_DATA) + sizeof(MT_DGN_C_ENERGY_DATA), 0))
                {
                    //bFlashZero = TRUE;
                }
                
                if ((nENSum != sRealData.nENChkSum) || (bFlashZero))
                {
                    if (nENSum != sRealData.nENChkSum)
                    {
                        if (!TestBufBit(nACRunStat, 8, 2))
                        {
                            SetBufBit(nACRunStat, 8, 2);
                            MemWriteFlhXmpData(F8, 0, &nACRunStat[0], sizeof(nACRunStat));
                        }
                        if (!TEST_BIT(pDataTable->nACRunStat, 2))
                        {
                            SET_BIT(pDataTable->nACRunStat, 2);
                        }
                        return RET_ERR_ERROR; 	//Flash校验出错

                    }
                    else
                    {
                        
                    }
                }
                else
                {
                    MemWriteAcRealData(FEND, &sRealData, sizeof(sRealData));
                }
            }
            else
            {
                if (!TestBufBit(nACRunStat, 8, 2))
                {
                    SetBufBit(nACRunStat, 8, 2);
                    MemWriteFlhXmpData(F8, 0, &nACRunStat[0], sizeof(nACRunStat));
                }
                if (!TEST_BIT(pDataTable->nACRunStat, 4))
                {
                    SET_BIT(pDataTable->nACRunStat, 4);
                    
                }
                return RET_ERR_ERROR;	//Flash读出错
            }
        }
#endif
    }
    else
    {
#if 0
        MemReadFlhXmpData(F8, 0, &nACRunStat[0], sizeof(nACRunStat));
        if (!TestBufBit(nACRunStat, 8, 1))
        {
            SetBufBit(nACRunStat, 8, 1);
            MemWriteFlhXmpData(F8, 0, &nACRunStat[0], sizeof(nACRunStat));
        }
        if (!TEST_BIT(pDataTable->nACRunStat, 3))
        {
            SET_BIT(pDataTable->nACRunStat, 3);
        }
        
        for(ii = 0; ii < 5; ii++)
        {
            if (MemReadAcXFlhBackData(F48, &sRealData, sizeof(sRealData)) > 0)
            {
                nRet = RET_ERR_OK;
            }
            if(RET_ERR_OK == nRet)
            {
                break;
            }
            else
            {
                OSSemPend(g_pSem[SEM_COL_AC_CALC_IDX], 0, &nErr);
            }
        }
        
        if (RET_ERR_OK == nRet)
        {
            if (TEST_BIT(pDataTable->nACRunStat, 4))
            {
                 CLR_BIT(pDataTable->nACRunStat, 4);
            }
            memcpy(&pSavDataTable->sAcEnergy, &sRealData.sAcEnergy, sizeof(MT_DGN_ENERGY_DATA) + sizeof(MT_DGN_Q_ENERGY_DATA)
                   + sizeof(MT_DGN_TRI_ENERGY_DATA) + sizeof(MT_DGN_TRI_Q_EN_DATA) + sizeof(MT_DGN_C_ENERGY_DATA));
            memcpy(&pSavDataTable->sEnergyData[0], &sRealData.sEPulse[0], 8*sizeof(ENERGY_DATA) + MAX_PHASE_NUM*sizeof(PH_ENERGY_DATA)
                   +  MAX_PHASE_NUM*sizeof(PH_ENERGY_DATA) + sizeof(COMB_ENERGY_DATA));

            nENSum = 0x55aa66bb + CheckINT32USum((INT32U*)&pSavDataTable->sAcEnergy, 0, (sizeof(MT_DGN_ENERGY_DATA) + sizeof(MT_DGN_Q_ENERGY_DATA)
                     + sizeof(MT_DGN_TRI_ENERGY_DATA) + sizeof(MT_DGN_TRI_Q_EN_DATA) + sizeof(MT_DGN_C_ENERGY_DATA))/4);
            if(nENSum != sRealData.nENChkSum)
            {
                if (!TestBufBit(nACRunStat, 8, 2))
                {
                    SetBufBit(nACRunStat, 8, 2);
                    MemWriteFlhXmpData(F8, 0, &nACRunStat[0], sizeof(nACRunStat));
                }
                if (!TEST_BIT(pDataTable->nACRunStat, 2))
                {
                    SET_BIT(pDataTable->nACRunStat, 2);
                }
                return RET_ERR_ERROR;	//Flash校验出错
            }
            else
            {
                MemWriteAcRealData(FEND, &sRealData, sizeof(sRealData));
            }
        }
        else
        {
            if (!TestBufBit(nACRunStat, 8, 2))
            {
                SetBufBit(nACRunStat, 8, 2);
                MemWriteFlhXmpData(F8, 0, &nACRunStat[0], sizeof(nACRunStat));
            }
            if (!TEST_BIT(pDataTable->nACRunStat, 4))
            {
                SET_BIT(pDataTable->nACRunStat, 4);
            }
            return RET_ERR_ERROR;	//Flash读出错
        }
#endif
    }
    
    //根据条件备份数据至第2备份（Flash）
	pDataTable->nFlashCounter = 0;
    MemWriteAcRealData(FEND, &sRealData, sizeof(sRealData));
    
    pSavDataTable->nENSumChk = 0x55aa66bb + CheckINT32USum((INT32U*)&pSavDataTable->sAcEnergy, 0, (sizeof(MT_DGN_ENERGY_DATA) + sizeof(MT_DGN_Q_ENERGY_DATA)
                               + sizeof(MT_DGN_TRI_ENERGY_DATA) + sizeof(MT_DGN_TRI_Q_EN_DATA) + sizeof(MT_DGN_C_ENERGY_DATA))/4);
    pSavDataTable->nEPSumChk = CheckINT32USum((INT32U*)&pSavDataTable->sEnergyData[0], 0, (8*sizeof(ENERGY_DATA)
                               + 2*MAX_PHASE_NUM*sizeof(PH_ENERGY_DATA) + sizeof(COMB_ENERGY_DATA))/4);
    return RET_ERR_OK;
}

/********************************************************************************
* 函数名:WriteAcENData
* 输　入:
* 输  出:
* 返  回:错误编码
* 功  能:将交采临时区电能数据写入实时电能数据并进行BCD与BIN转换
*********************************************************************************/
INT8S WriteAcENData(void)
{
    INT8U   i = 0;
    INT16U  nPulseCnt = 0;;
    INT32U  nTmp = 0;
    INT32U  nENSum = 0;

    nPulseCnt = pDataTable->nPulseConst/100;
    if (0 == nPulseCnt)
    {
        return RET_ERR_ERROR;
    }
    
    //电能示值
    for (i = 0; i < (MAX_FEE_NUM + 1); i++)
    {
        CombEnergyCal(&pSavDataTable->sAcCombEnergy.sPPEnergy[i], &pSavDataTable->sCombEPulse.nCombEpEnergy[i], nPulseCnt);
        
        nTmp = BCD2INT32U((INT8U*)&pSavDataTable->sAcEnergy.sPPEnergy[i], 8);
        if (g_MeterAcErrFlg[2])//倒走
        {
            if (nTmp < (INT32U)(pSavDataTable->sEnergyData[0].nEnergy[i]/nPulseCnt))
            {
                nTmp = 0;
            }
            else
            {
                nTmp -= (INT32U)(pSavDataTable->sEnergyData[0].nEnergy[i]/nPulseCnt);
            }                                                             
        }
        else
        {
            nTmp += (INT32U)(pSavDataTable->sEnergyData[0].nEnergy[i]/nPulseCnt);
        }
        
        INT32U2BCD(nTmp, 8, (INT8U*)&pSavDataTable->sAcEnergy.sPPEnergy[i]);
        pSavDataTable->sEnergyData[0].nEnergy[i] = (INT16U)(pSavDataTable->sEnergyData[0].nEnergy[i]%nPulseCnt);

        nTmp = BCD2INT32U((INT8U*)&pSavDataTable->sAcEnergy.sNPEnergy[i], 8);
        nTmp += (INT32U)(pSavDataTable->sEnergyData[1].nEnergy[i]/nPulseCnt);
        INT32U2BCD(nTmp, 8, (INT8U*)&pSavDataTable->sAcEnergy.sNPEnergy[i]);
        pSavDataTable->sEnergyData[1].nEnergy[i] = (INT16U)(pSavDataTable->sEnergyData[1].nEnergy[i]%nPulseCnt);

        CombEnergyCal(&pSavDataTable->sAcEnergy.sPNEnergy[i], &pSavDataTable->sEnergyData[2].nEnergy[i], nPulseCnt);
        CombEnergyCal(&pSavDataTable->sAcEnergy.sNNEnergy[i], &pSavDataTable->sEnergyData[3].nEnergy[i], nPulseCnt);

        nTmp = BCD2INT32U((INT8U*)&pSavDataTable->sAcQEnergy.sQ1Energy[i], 8);
        nTmp += (INT32U)(pSavDataTable->sEnergyData[4].nEnergy[i]/nPulseCnt);
        INT32U2BCD(nTmp, 8, (INT8U*)&pSavDataTable->sAcQEnergy.sQ1Energy[i]);
        pSavDataTable->sEnergyData[4].nEnergy[i] = (INT16U)(pSavDataTable->sEnergyData[4].nEnergy[i]%nPulseCnt);

        nTmp = BCD2INT32U((INT8U*)&pSavDataTable->sAcQEnergy.sQ2Energy[i], 8);
        nTmp += (INT32U)(pSavDataTable->sEnergyData[5].nEnergy[i]/nPulseCnt);
        INT32U2BCD(nTmp, 8, (INT8U*)&pSavDataTable->sAcQEnergy.sQ2Energy[i]);
        pSavDataTable->sEnergyData[5].nEnergy[i] = (INT16U)(pSavDataTable->sEnergyData[5].nEnergy[i]%nPulseCnt);

        nTmp = BCD2INT32U((INT8U*)&pSavDataTable->sAcQEnergy.sQ3Energy[i], 8);
        nTmp += (INT32U)(pSavDataTable->sEnergyData[6].nEnergy[i]/nPulseCnt);
        INT32U2BCD(nTmp, 8, (INT8U*)&pSavDataTable->sAcQEnergy.sQ3Energy[i]);
        pSavDataTable->sEnergyData[6].nEnergy[i] = (INT16U)(pSavDataTable->sEnergyData[6].nEnergy[i]%nPulseCnt);

        nTmp = BCD2INT32U((INT8U*)&pSavDataTable->sAcQEnergy.sQ4Energy[i], 8);
        nTmp += (INT32U)(pSavDataTable->sEnergyData[7].nEnergy[i]/nPulseCnt);
        INT32U2BCD(nTmp, 8, (INT8U*)&pSavDataTable->sAcQEnergy.sQ4Energy[i]);
        pSavDataTable->sEnergyData[7].nEnergy[i] = (INT16U)(pSavDataTable->sEnergyData[7].nEnergy[i]%nPulseCnt);
    }
    
    MemWriteAcRealData(F1, &pSavDataTable->sAcEnergy, sizeof(pSavDataTable->sAcEnergy));
    MemWriteAcRealData(F2, &pSavDataTable->sAcQEnergy, sizeof(pSavDataTable->sAcQEnergy));
    MemWriteAcRealData(F5, &pSavDataTable->sAcCombEnergy, sizeof(pSavDataTable->sAcCombEnergy));
    
    
    //分相电能总示值
    for(i = 0; i < MAX_PHASE_NUM; i++)
    {
        nTmp = BCD2INT32U((INT8U*)&pSavDataTable->sAcTriEnergy.sPPEnergy[i], 8);
        nTmp += (INT32U)(pSavDataTable->sPhEnergyData[i].nEnergy[0]/nPulseCnt);
        INT32U2BCD(nTmp, 8, (INT8U*)&pSavDataTable->sAcTriEnergy.sPPEnergy[i]);
        pSavDataTable->sPhEnergyData[i].nEnergy[0] = (INT16U)(pSavDataTable->sPhEnergyData[i].nEnergy[0]%nPulseCnt);

        nTmp = BCD2INT32U((INT8U*)&pSavDataTable->sAcTriEnergy.sNPEnergy[i], 8);
        nTmp += (INT32U)(pSavDataTable->sPhEnergyData[i].nEnergy[1]/nPulseCnt);
        INT32U2BCD(nTmp, 8, (INT8U*)&pSavDataTable->sAcTriEnergy.sNPEnergy[i]);
        pSavDataTable->sPhEnergyData[i].nEnergy[1] = (INT16U)(pSavDataTable->sPhEnergyData[i].nEnergy[1]%nPulseCnt);

        CombEnergyCal(&pSavDataTable->sAcTriEnergy.sPNEnergy[i], &pSavDataTable->sPhEnergyData[i].nEnergy[2], nPulseCnt);
        CombEnergyCal(&pSavDataTable->sAcTriEnergy.sNNEnergy[i], &pSavDataTable->sPhEnergyData[i].nEnergy[3], nPulseCnt);
    }
    MemWriteAcRealData(F3, &pSavDataTable->sAcTriEnergy, sizeof(pSavDataTable->sAcTriEnergy));
    
    //分相四象限电能示值
    for(i = 0; i < MAX_PHASE_NUM; i++)
    {
        nTmp = BCD2INT32U((INT8U*)&pSavDataTable->sAcTriQEnergy.sQ1Energy[i], 8);
        nTmp += (INT32U)(pSavDataTable->sPhQEnergyData[i].nEnergy[0]/nPulseCnt);
        INT32U2BCD(nTmp, 8, (INT8U*)&pSavDataTable->sAcTriQEnergy.sQ1Energy[i]);
        pSavDataTable->sPhQEnergyData[i].nEnergy[0] = (INT16U)(pSavDataTable->sPhQEnergyData[i].nEnergy[0]%nPulseCnt);

        nTmp = BCD2INT32U((INT8U*)&pSavDataTable->sAcTriQEnergy.sQ2Energy[i], 8);

        nTmp += (INT32U)(pSavDataTable->sPhQEnergyData[i].nEnergy[1]/nPulseCnt);
        INT32U2BCD(nTmp, 8, (INT8U*)&pSavDataTable->sAcTriQEnergy.sQ2Energy[i]);
        pSavDataTable->sPhQEnergyData[i].nEnergy[1] = (INT16U)(pSavDataTable->sPhQEnergyData[i].nEnergy[1]%nPulseCnt);

        nTmp = BCD2INT32U((INT8U*)&pSavDataTable->sAcTriQEnergy.sQ3Energy[i], 8);
        nTmp += (INT32U)(pSavDataTable->sPhQEnergyData[i].nEnergy[2]/nPulseCnt);
        INT32U2BCD(nTmp, 8, (INT8U*)&pSavDataTable->sAcTriQEnergy.sQ3Energy[i]);
        pSavDataTable->sPhQEnergyData[i].nEnergy[2] = (INT16U)(pSavDataTable->sPhQEnergyData[i].nEnergy[2]%nPulseCnt);

        nTmp = BCD2INT32U((INT8U*)&pSavDataTable->sAcTriQEnergy.sQ4Energy[i], 8);
        nTmp += (INT32U)(pSavDataTable->sPhQEnergyData[i].nEnergy[3]/nPulseCnt);
        INT32U2BCD(nTmp, 8, (INT8U*)&pSavDataTable->sAcTriQEnergy.sQ4Energy[i]);
        pSavDataTable->sPhQEnergyData[i].nEnergy[3] = (INT16U)(pSavDataTable->sPhQEnergyData[i].nEnergy[3]%nPulseCnt);
    }
    
    MemWriteAcRealData(F4, &pSavDataTable->sAcTriQEnergy, sizeof(pSavDataTable->sAcTriQEnergy));
    MemWriteAcRealData(F9, &pSavDataTable->sEnergyData, sizeof(pSavDataTable->sEnergyData));
    MemWriteAcRealData(F10, &pSavDataTable->sPhEnergyData, sizeof(pSavDataTable->sPhEnergyData));
    MemWriteAcRealData(F11, &pSavDataTable->sPhQEnergyData, sizeof(pSavDataTable->sPhQEnergyData));
    MemWriteAcRealData(F12, &pSavDataTable->sCombEPulse, sizeof(pSavDataTable->sCombEPulse));
    
    nENSum = 0x55aa66bb + CheckINT32USum((INT32U*)&pSavDataTable->sAcEnergy, 0, (sizeof(MT_DGN_ENERGY_DATA) + sizeof(MT_DGN_Q_ENERGY_DATA)
             + sizeof(MT_DGN_TRI_ENERGY_DATA) + sizeof(MT_DGN_TRI_Q_EN_DATA) + sizeof(MT_DGN_C_ENERGY_DATA))/4);
    pSavDataTable->nENSumChk = nENSum;
    
    MemWriteAcRealData(F6, &nENSum, 4);
    
    return RET_ERR_OK;
}

/********************************************************************************
* 函数名:ReadAcDEData
* 输　入:
* 输  出:
* 返  回:错误编码
* 功  能:读出实时需量数据至交采临时区并进行BCD与BIN转换
*********************************************************************************/
INT8S ReadAcDEData(void)
{
    INT8S   nRet = RET_ERR_ERROR;
    INT8U   nErr = 0;
   	INT16U  ii = 0;
   	INT8U   nACRunStat[LEN_RUN_STAT_FLAG];
   	AC_EVENT_STAT sEventStat;
    MEM_AC_REAL_DATA sRealData;
    
    memset(&sEventStat, 0, sizeof(AC_EVENT_STAT));
	memset(&sRealData, 0, sizeof(MEM_AC_REAL_DATA));
	memset(&nACRunStat[0], 0, sizeof(nACRunStat));
    
    for(ii = 0; ii < 5; ii++)
    {
        if (MemReadAcRealData(FEND, &sRealData, sizeof(sRealData)) > 0)
        {
            nRet = RET_ERR_OK;
        }
    	if(RET_ERR_OK == nRet)
        {
    		break;
        }
    	else
        {
    		OSSemPend(g_pSem[SEM_COL_AC_CALC_IDX], 0, &nErr);
        }
    }
    if(RET_ERR_OK != nRet)
    {
        MemReadFlhXmpData(F8, 0, &nACRunStat[0], sizeof(nACRunStat));
        if (!TestBufBit(nACRunStat, 8, 1))
        {
			SetBufBit(nACRunStat, 8, 1);
            MemWriteFlhXmpData(F8, 0, &nACRunStat[0], sizeof(nACRunStat)); 
        }
        if (!TEST_BIT(pDataTable->nACRunStat, 3))
        {
            SET_BIT(pDataTable->nACRunStat, 3);
        }
        for(ii = 0; ii < 5; ii++)
        {
    		if (MemReadAcXFlhBackData(F48, &sRealData, sizeof(sRealData)) > 0)
            {
                nRet = RET_ERR_OK;
            }
    		if(RET_ERR_OK == nRet)
            {
	    		break;
            }
	    	else
            {
	    		OSSemPend(g_pSem[SEM_COL_AC_CALC_IDX], 0, &nErr);
            }
        }
        
        if(RET_ERR_OK != nRet)
        {
            if (!TestBufBit(nACRunStat, 8, 2))
            {
				SetBufBit(nACRunStat, 8, 2);
                MemWriteFlhXmpData(F8, 0, &nACRunStat[0], sizeof(nACRunStat));
            }
            if (!TEST_BIT(pDataTable->nACRunStat, 4))
            {
                SET_BIT(pDataTable->nACRunStat, 4);
            }
            return RET_ERR_ERROR;
        }
        else
        {
            if (TEST_BIT(pDataTable->nACRunStat, 4))
            {
                CLR_BIT(pDataTable->nACRunStat, 4);
            }
        }
    }
    else
    {
        if (TEST_BIT(pDataTable->nACRunStat, 3))
        {
            CLR_BIT(pDataTable->nACRunStat, 3);
        }
    }
    
    //需量示值及发生时间
    for(ii=0; ii<5; ii++)
    {
        memcpy(&pSavDataTable->sDemandData[0].sTimeData[ii], &sRealData.sAcDemand.sPPDemandTime[ii], sizeof(FORMAT15));
        memcpy(&pSavDataTable->sDemandData[1].sTimeData[ii], &sRealData.sAcDemand.sNPDemandTime[ii], sizeof(FORMAT15));
        memcpy(&pSavDataTable->sDemandData[2].sTimeData[ii], &sRealData.sAcDemand.sPNDemandTime[ii], sizeof(FORMAT15));
        memcpy(&pSavDataTable->sDemandData[3].sTimeData[ii], &sRealData.sAcDemand.sNNDemandTime[ii], sizeof(FORMAT15));
        memcpy(&pSavDataTable->sDemandData[4].sTimeData[ii], &sRealData.sAcQDemand.sQ1DemandTime[ii], sizeof(FORMAT15));
        memcpy(&pSavDataTable->sDemandData[5].sTimeData[ii], &sRealData.sAcQDemand.sQ2DemandTime[ii], sizeof(FORMAT15));
        memcpy(&pSavDataTable->sDemandData[6].sTimeData[ii], &sRealData.sAcQDemand.sQ3DemandTime[ii], sizeof(FORMAT15));
        memcpy(&pSavDataTable->sDemandData[7].sTimeData[ii], &sRealData.sAcQDemand.sQ4DemandTime[ii], sizeof(FORMAT15));
    }
    for(ii = 0; ii < 8; ii++)
    {
   		 memcpy(&pSavDataTable->sDemandData[ii].nDemand[0], &sRealData.nAcDemand[ii][0], (MAX_FEE_NUM+1)*sizeof(INT32U));
    }
    
    pSavDataTable->nDESumChk = CheckSum((INT8U*)&pSavDataTable->sDemandData[0], 0, 8*sizeof(AC_DEMAND_DATA));
    MemReadAcXBackData(F13, &sEventStat, sizeof(AC_EVENT_STAT));
	pDataTable->nSQNum = sEventStat.nSQNum;
	pDataTable->nSDNum = sEventStat.nSDNum;

    return RET_ERR_OK;
}

/********************************************************************************
* 函数名:WriteAcDEData
* 输　入:
*
* 输  出:
*
* 返  回:错误编码
* 功  能:将交采临时区需量数据写入实时需量数据并进行BCD与BIN转换
*********************************************************************************/
INT8S WriteAcDEData(void)
{
    INT16U	i    = 0;
    MT_DGN_DEMAND_DATA sACDeData;
    MT_DGN_Q_DE_DATA   sAcQDeData;
    INT32U nDeBuff[MAX_P_Q_NUM+4][MAX_FEE_NUM+1];

    memset(&sACDeData,0,sizeof(MT_DGN_DEMAND_DATA));
    memset(&sAcQDeData,0,sizeof(MT_DGN_Q_DE_DATA));
    
    pSavDataTable->nDESumChk = CheckSum((INT8U*)&pSavDataTable->sDemandData[0], 0, 8*sizeof(AC_DEMAND_DATA));
    
    for (i = 0; i < (MAX_P_Q_NUM + 4); i++)
   	{
   		 memcpy(&nDeBuff[i][0], &pSavDataTable->sDemandData[i].nDemand[0], (MAX_FEE_NUM+1)*sizeof(INT32U));
   	}
    MemWriteAcRealData(F13, &nDeBuff, sizeof(nDeBuff));
    
    //需量示值及发生时间
    for(i=0;i<5;i++)
    {
        INT32U2BCD(pSavDataTable->sDemandData[0].nDemand[i],6,&sACDeData.sPPDemand[i].nFrac0);
        memcpy(&sACDeData.sPPDemandTime[i], &pSavDataTable->sDemandData[0].sTimeData[i], sizeof(FORMAT15));
        INT32U2BCD(pSavDataTable->sDemandData[1].nDemand[i],6,&sACDeData.sNPDemand[i].nFrac0);
        memcpy(&sACDeData.sNPDemandTime[i], &pSavDataTable->sDemandData[1].sTimeData[i], sizeof(FORMAT15));
        INT32U2BCD(pSavDataTable->sDemandData[2].nDemand[i],6,&sACDeData.sPNDemand[i].nFrac0);
        memcpy(&sACDeData.sPNDemandTime[i], &pSavDataTable->sDemandData[2].sTimeData[i], sizeof(FORMAT15));
        INT32U2BCD(pSavDataTable->sDemandData[3].nDemand[i],6,&sACDeData.sNNDemand[i].nFrac0);
        memcpy(&sACDeData.sNNDemandTime[i], &pSavDataTable->sDemandData[3].sTimeData[i], sizeof(FORMAT15));

        INT32U2BCD(pSavDataTable->sDemandData[4].nDemand[i],6,&sAcQDeData.sQ1Demand[i].nFrac0);
        memcpy(&sAcQDeData.sQ1DemandTime[i], &pSavDataTable->sDemandData[4].sTimeData[i], sizeof(FORMAT15));
        INT32U2BCD(pSavDataTable->sDemandData[5].nDemand[i],6,&sAcQDeData.sQ2Demand[i].nFrac0);
        memcpy(&sAcQDeData.sQ2DemandTime[i], &pSavDataTable->sDemandData[5].sTimeData[i], sizeof(FORMAT15));
        INT32U2BCD(pSavDataTable->sDemandData[6].nDemand[i],6,&sAcQDeData.sQ3Demand[i].nFrac0);
        memcpy(&sAcQDeData.sQ3DemandTime[i], &pSavDataTable->sDemandData[6].sTimeData[i], sizeof(FORMAT15));
        INT32U2BCD(pSavDataTable->sDemandData[7].nDemand[i],6,&sAcQDeData.sQ4Demand[i].nFrac0);
        memcpy(&sAcQDeData.sQ4DemandTime[i], &pSavDataTable->sDemandData[7].sTimeData[i], sizeof(FORMAT15));
    }
    MemWriteAcRealData(F7, &sACDeData, sizeof(sACDeData));
    MemWriteAcRealData(F8, &sAcQDeData, sizeof(sAcQDeData));
    return RET_ERR_OK;
}

/*
*****************************************************************************************************
* 函数名:AcCalcPerChanged
* 输　入:
* 输  出:
* 返  回:
* 功能描述:判断电表是否切换时段
*****************************************************************************************************
*/
INT8S AcCalcPerChanged(const pSTD_TIME pTime)
{
    INT8U	nRate = 0;
#if 1
    INT16U	i;
	INT16U	nBuff[2] = {0,0};
	INT8U	nDay = 0;
	INT8U	nYearSegmNum = 0;
	INT8U	nDaySegmNum = 0;
	INT32S	nSuccflag = RET_ERR_ERROR;
	INT8U	nYearSegmFlag = 0;
	INT8U	nDaySegmFlag = 0;
	FORMAT15	sChTime;
	MM_DD_NN	sYearSegmTable[2][MAX_YEAR_SEGM_NUM];
	hh_mm_NN	sDaySegmTable[2][MAX_DAY_TABLE_NUM][MAX_DAY_SEGM_NUM];
	YY_MM_DD_NN	sHolidayTable[MAX_HOLIDAY_NUM];
    
    memset(&sChTime, 0, sizeof(FORMAT15));
	memset(&sYearSegmTable, 0, 2*MAX_YEAR_SEGM_NUM*sizeof(MM_DD_NN));
	memset(&sDaySegmTable, 0, 2*MAX_DAY_TABLE_NUM*MAX_DAY_SEGM_NUM*sizeof(hh_mm_NN));
	memset(&sHolidayTable, 0, MAX_HOLIDAY_NUM*sizeof(YY_MM_DD_NN));
    
    nSuccflag = MemReadAcPrm(F11, &sChTime, sizeof(sChTime));
    if (nSuccflag >= 0)
    {
        if ((sChTime.nMon != 0)&&(sChTime.nDay != 0))
        {
            if (pTime->nYear == sChTime.nYear)
            {
                if (pTime->nMon == sChTime.nMon)
				{
					if (pTime->nDay == sChTime.nDay)
					{
						nBuff[0] = (INT32U)((pTime->nHour/16)*600 + (pTime->nHour%16)*60 + (pTime->nMin/16)
								+ (pTime->nMin%16));
						nBuff[1] = (INT32U)((sChTime.nHour/16)*600 + (sChTime.nHour%16)*60 + (sChTime.nMin/16)
								+ (sChTime.nMin%16));

						nYearSegmFlag = (nBuff[0]<nBuff[1])?0:1;
					}
					else
					{
						nYearSegmFlag = (pTime->nDay<sChTime.nDay)?0:1;
					}
				}
				else
				{
					nYearSegmFlag = (pTime->nMon<sChTime.nMon)?0:1;
				}
            }
            else
			{
				nYearSegmFlag = (pTime->nYear<sChTime.nYear)?0:1;
			}
        }
        else
		{
			nYearSegmFlag = 0;
		}
    }
    else
	{
		//return RET_ERR_ERROR;
	}
    
    memset(&sChTime, 0, sizeof(FORMAT15));
	nSuccflag = RET_ERR_ERROR;
	nSuccflag = MemReadAcPrm(F12, &sChTime, sizeof(sChTime));
	if(nSuccflag >= 0)
    {
        if((sChTime.nMon != 0)&&(sChTime.nDay != 0))
		{
			if(pTime->nYear == sChTime.nYear)
			{
				if(pTime->nMon == sChTime.nMon)
				{
					if(pTime->nDay == sChTime.nDay)
					{
						nBuff[0] = (INT32U)((pTime->nHour/16)*36000 + (pTime->nHour%16)*3600 + (pTime->nMin/16)*600
								+ (pTime->nMin%16)*60);
						nBuff[1] = (INT32U)((sChTime.nHour/16)*36000 + (sChTime.nHour%16)*3600 + (sChTime.nMin/16)*600
								+ (sChTime.nMin%16)*60);

						nDaySegmFlag = (nBuff[0]<nBuff[1])?0:1;
					}
					else
					{
						nDaySegmFlag = (pTime->nDay<sChTime.nDay)?0:1;
					}
				}
				else
				{
					nDaySegmFlag = (pTime->nMon<sChTime.nMon)?0:1;
				}
			}
			else
			{
				nDaySegmFlag = (pTime->nYear<sChTime.nYear)?0:1;
			}
		}
		else
		{
			nDaySegmFlag = 0;
		}
    }
    else
	{
		//return RET_ERR_ERROR;
	}
    
    nSuccflag = RET_ERR_ERROR;
	nSuccflag = MemReadAcPrm(F42, &sHolidayTable, sizeof(sHolidayTable));
	if(nSuccflag >= 0)
    {
        for(i = 0; i < MAX_HOLIDAY_NUM; i++)
        {
            if((sHolidayTable[i].nMon == pTime->nMon) && (sHolidayTable[i].nDay == pTime->nDay))
            {
                nDay = sHolidayTable[i].nNo;//获取公共假日日时段表号
            }
        }
    }
    
    if(nDay == 0)
    {
        nSuccflag = RET_ERR_ERROR;
		nSuccflag = MemReadAcPrm(F40, &sYearSegmTable, sizeof(sYearSegmTable));
        if(nSuccflag >= 0)
        {
            for(i = 0; i < (MAX_YEAR_SEGM_NUM - 1); i++) //判断时区表有效时区数
			{
				if (sYearSegmTable[nYearSegmFlag][i].nMon > sYearSegmTable[nYearSegmFlag][i + 1].nMon)
				{
					nYearSegmNum = i + 1;
					break;
				}
				else if (sYearSegmTable[nYearSegmFlag][i].nMon == sYearSegmTable[nYearSegmFlag][i + 1].nMon)
				{
					if (sYearSegmTable[nYearSegmFlag][i].nDay >= sYearSegmTable[nYearSegmFlag][i + 1].nDay)
					{
						nYearSegmNum = i + 1;
						break;
					}
				}
			}

			nBuff[0] = 0;
			nBuff[0] = GetDaysFrom2MMDD(pTime->nYear, pTime->nMon, pTime->nDay);
			for(i = 0; i < nYearSegmNum; i++)
			{
				nBuff[1] = 0;
				nBuff[1] = GetDaysFrom2MMDD(pTime->nYear, sYearSegmTable[nYearSegmFlag][i].nMon, sYearSegmTable[nYearSegmFlag][i].nDay);
				if(nBuff[0] < nBuff[1])
				{
					break;
				}
			}
			if(i == 0)
			{
				nDay = sYearSegmTable[nYearSegmFlag][nYearSegmNum-1].nNo;
			}
			else
			{
				nDay = sYearSegmTable[nYearSegmFlag][i-1].nNo;
			}
		}
		else
		{
			//return RET_ERR_ERROR;
		}
    }
    
    if ((nDay > MAX_DAY_TABLE_NUM) || (nDay == 0))
	{
		nDay = 1;
	}
    
    nSuccflag = RET_ERR_ERROR;
	nSuccflag = MemReadAcPrm(F41, &sDaySegmTable, sizeof(sDaySegmTable));
    if(nSuccflag >= 0)
    {
        for(i = 0; i < (MAX_DAY_SEGM_NUM - 1); i++) //判断时段表有效时段数
		{
			if (sDaySegmTable[nDaySegmFlag][nDay - 1][i].nHour > sDaySegmTable[nDaySegmFlag][nDay - 1][i + 1].nHour)
			{
				nDaySegmNum = i + 1;
				break;
			}
			else if (sDaySegmTable[nDaySegmFlag][nDay - 1][i].nHour == sDaySegmTable[nDaySegmFlag][nDay - 1][i + 1].nHour)
			{
				if (sDaySegmTable[nDaySegmFlag][nDay - 1][i].nMin >= sDaySegmTable[nDaySegmFlag][nDay - 1][i + 1].nMin)
				{
					nDaySegmNum = i + 1;
					break;
				}
			}
		}
		nBuff[0] = 0;
		nBuff[0] = (INT32U)((pTime->nHour/16)*600 + (pTime->nHour%16)*60 + (pTime->nMin/16)*10
				+ (pTime->nMin%16));
		for(i = 0; i < nDaySegmNum; i++)
		{
			nBuff[1] = 0;
			nBuff[1] = (INT32U)((sDaySegmTable[nDaySegmFlag][nDay-1][i].nHour/16)*600 + (sDaySegmTable[nDaySegmFlag][nDay-1][i].nHour%16)*60
				+ (sDaySegmTable[nDaySegmFlag][nDay-1][i].nMin/16)*10 + (sDaySegmTable[nDaySegmFlag][nDay-1][i].nMin%16));
			if(nBuff[0] < nBuff[1])
			{
				break;
			}
		}
		if(i == 0)
		{
			nRate = sDaySegmTable[nDaySegmFlag][nDay-1][nDaySegmNum-1].nNo;
		}
		else
		{
			nRate = sDaySegmTable[nDaySegmFlag][nDay-1][i-1].nNo;
		}
    }
    else
	{
		//return RET_ERR_ERROR;
	}
    
    if((nRate > MAX_FEE_NUM) || (nRate == 0))
	{
            //nRate = 3;
    if(( pTime->nHour == 0x19) || (pTime->nHour == 0x20 ) || (pTime->nHour == 0x11 ))        //当前时间尖、峰、平、谷     
      { 
        nRate = 1;
      }
    else if(( pTime->nHour == 0x09) || (pTime->nHour == 0x10)||( pTime->nHour == 0x21)||(pTime->nHour == 0x22) || (pTime->nHour == 0x18))            
      {
        nRate = 2;
      }
     else if((( pTime->nHour == 0x07) || (pTime->nHour == 0x08 ))|| (( pTime->nHour >= 0x12)&&(pTime->nHour <= 0x17 )))         
      { 
        nRate = 3;
      }
      else 
              { 
                  nRate = 4;
              }
      }
#endif
    
    if(pDataTable->nRateNo != nRate)
    {
        g_nFlgnRateNo = 1;
    }

	pDataTable->nRateNo = nRate;

	return RET_ERR_OK;
}


/*
*****************************************************************************************************
* 函数名:AcCalcDemand
* 输　入:
* 输  出:
* 返  回:
* 功能描述:电表需量统计
*****************************************************************************************************
*/
INT8S AcCalcDemand(const pSTD_TIME pTime)
{
    INT8U	i,j;
	INT8U	nDemandPeriod = 0;
	INT8U	nSlipTime = 0;
	INT8U	nDeTime = 0;
	INT8U	nCs = 0;
	INT8S	nSuccflag = RET_ERR_ERROR;
    INT8U   nDeChange = 0;
	INT32U	nTmp1;
	INT32U	nTmp2;
	FP32	fTmp;
	INT32U	nTmpDemandData[MAX_P_Q_NUM+4];
    INT32U	nTmpDemandData1[MAX_P_Q_NUM+4];
    INT8U   nACRunStat[LEN_RUN_STAT_FLAG];

    memset(&nACRunStat[0], 0, sizeof(nACRunStat));
    
    nCs = CheckSum((INT8U*)&pSavDataTable->sDemandData[0], 0, 8*sizeof(AC_DEMAND_DATA));
    if ((pSavDataTable->nDESumChk != nCs) || (0 == pSavDataTable->nDESumChk) || (0 == nCs))
    {
        if (pSavDataTable->nDESumChk != nCs)
        {
            MemReadFlhXmpData(F8, 0, &nACRunStat[0], sizeof(nACRunStat));
            if (!TestBufBit(nACRunStat, 8, 0))
			{
				SetBufBit(nACRunStat, 8, 0);
                MemWriteFlhXmpData(F8, 0, &nACRunStat[0], sizeof(nACRunStat));
			}
            if (!TEST_BIT(pDataTable->nACRunStat, 0))
            {
                SET_BIT(pDataTable->nACRunStat, 0);
            }
        }
        
        if (RET_ERR_OK != ReadAcDEData())
        {
            return RET_ERR_ERROR;
        }
    }
    
    if (MemReadAcPrm(F1, &nDemandPeriod, sizeof(nDemandPeriod)) >= 0)
    {
        nSuccflag = RET_ERR_OK;
    }
	if((nSuccflag != RET_ERR_OK)||((nDemandPeriod%5) != 0)||(nDemandPeriod > 15)||(nDemandPeriod == 0))
	{
		nDemandPeriod = 15;
	}
    
    nSuccflag = RET_ERR_ERROR;
	if (MemReadAcPrm(F2, &nSlipTime, sizeof(nSlipTime)) >= 0)
    {
        nSuccflag= RET_ERR_OK;
    }
	if((nSuccflag != RET_ERR_OK) || ((nDemandPeriod%nSlipTime) != 0) || (nSlipTime > 5) || (nSlipTime == 0))
	{
		nSlipTime = 1;
	}
    
    nDeTime = nDemandPeriod / nSlipTime;
	memset(nTmpDemandData, 0x00, MAX_P_Q_NUM*sizeof(INT32U));
    
    for(i = 0; i < (MAX_P_Q_NUM + 4); i++)
	{
		
		if(pDataTable->sDemandSlipData1[i].nStep >= 59*nSlipTime)
		{
			pDataTable->sDemandSlipData1[i].fVal[pDataTable->sDemandSlipData1[i].nSlips++] = ((FP32) pDataTable->nPowerPulse[i][3]/pDataTable->nPulseConst ) *3600/pDataTable->sDemandSlipData1[i].nStep;
            pDataTable->sDemandSlipData1[i].nStep = 0;
			if (pDataTable->sDemandSlipData1[i].nSlips >= nDeTime)
		    {
			    pDataTable->sDemandSlipData1[i].nSlips = 0;
			    pDataTable->sDemandSlipData1[i].nIsRev = 1;
		    }
            fTmp = 0.0;
            if (pDataTable->sDemandSlipData1[i].nIsRev == 1)
		    {
				for (j = 0; j < nDeTime; j++)
				{
					fTmp += pDataTable->sDemandSlipData1[i].fVal[j];
				}
				fTmp /= nDeTime;
                nTmpDemandData[i] = (INT32U)(fTmp*10000);
				
			}
		}
		
		if(pDataTable->sDemandSlipData2[i].nStep >= 59*nSlipTime)
		{
			pDataTable->sDemandSlipData2[i].fVal[pDataTable->sDemandSlipData2[i].nSlips++] = ( (FP32)pDataTable->nPowerPulse1[i][3]/pDataTable->nPulseConst ) *3600/pDataTable->sDemandSlipData2[i].nStep;
            pDataTable->sDemandSlipData2[i].nStep = 0;
		    if (pDataTable->sDemandSlipData2[i].nSlips >= nDeTime)
		    {
			    pDataTable->sDemandSlipData2[i].nSlips = 0;
			    pDataTable->sDemandSlipData2[i].nIsRev = 1;
		    }
            fTmp = 0.0;
			if (pDataTable->sDemandSlipData2[i].nIsRev == 1)
			{
				for (j = 0; j < nDeTime; j++)
				{
					fTmp += pDataTable->sDemandSlipData2[i].fVal[j];
				}
				fTmp /= nDeTime;
                nTmpDemandData1[i] = (INT32U)(fTmp*10000);
				
			}
		}
	}
    if(1 == g_nFlgnRateNo)          //费率切换
    {
      memset(&pSavDataTable->sDemandData, 0, sizeof(pSavDataTable->sDemandData));  
     // g_nFlgnRateNo = 0;
    }  

	for(i = 0; i < (MAX_P_Q_NUM + 4); i++)
	{
		nTmp1 = nTmpDemandData[i];
		nTmp2 = pSavDataTable->sDemandData[i].nDemand[0];
		if(nTmp1 > nTmp2)
		{
			pSavDataTable->sDemandData[i].nDemand[0] = nTmpDemandData[i];
			memcpy(&pSavDataTable->sDemandData[i].sTimeData[0], &pTime->nMin, sizeof(FORMAT15));
			nDeChange = 0x55;
		}
        nTmp1 = nTmpDemandData1[i];
		nTmp2 = pSavDataTable->sDemandData[i].nDemand[pDataTable->nRateNo];
        
		if(nTmp1 > nTmp2)
		{
			pSavDataTable->sDemandData[i].nDemand[pDataTable->nRateNo] = nTmpDemandData1[i];
			memcpy(&pSavDataTable->sDemandData[i].sTimeData[pDataTable->nRateNo], &pTime->nMin, sizeof(FORMAT15));
			nDeChange = 0x55;
		}
	}
	if (0x55 == nDeChange)
	{
		WriteAcDEData();
	}
	return RET_ERR_OK;
}

/*
*****************************************************************************************************
* 函数名:AcCalcDJData
* 输　入:
* 输  出:
* 返  回:
* 功能描述:冻结处理
*****************************************************************************************************
*/
void AcCalcDJData(pMEM_AC_DJ_DATA pAcdDjData, const pSTD_TIME pTime)
{
    MemReadAcRealData(F1, &pAcdDjData->sAcEnergy, sizeof(MT_DGN_ENERGY_DATA));
    MemReadAcRealData(F2, &pAcdDjData->sAcQEnergy, sizeof(MT_DGN_Q_ENERGY_DATA));
    MemReadAcRealData(F7, &pAcdDjData->sAcDemand, sizeof(MT_DGN_DEMAND_DATA));
    
    memcpy(&pAcdDjData->sRealP[0], &pDataTable->sRealInstant.sRealP[0], (MAX_PHASE_NUM + 1) * sizeof(FORMAT09));
	memcpy(&pAcdDjData->sRealQ[0], &pDataTable->sRealInstant.sRealQ[0], (MAX_PHASE_NUM + 1) * sizeof(FORMAT09));
    memcpy(&pAcdDjData->sTime, &pTime->nMin, sizeof(FORMAT15));
}

/*
*****************************************************************************************************
* 函数名:ACMinCalcProc
* 输　入:
* 输  出:
* 返  回:
* 功能描述:电表分钟处理（时段切换，需量统计）
*****************************************************************************************************
*/
void ACMinCalcProc(void)
{
    STD_TIME sTime;
	
	//获取系统时钟
    memset(&sTime, 0, sizeof(STD_TIME));
    if (RET_ERR_OK != GetSysClk(&sTime))
    {
        return;
    }
    //AC分钟冻结处理 23.2.20  借用F5约定冻结数据空间，重新开空间失败？后期查看
    AcMinFreezeDataRdyProc(&sTime);
    //AC定时冻结处理
	AcCalcPerDataRdyProc(&sTime);

    //AC日冻结处理
	AcCalcDayDataRdyProc(&sTime);
    
    //AC月冻结处理
    AcCalcMonDataRdyProc(&sTime);

	//AC结算日处理
	AcCalcRDayDataRdyProc(&sTime);

    //时段切换处理
	if(RET_ERR_OK != AcCalcPerChanged(&sTime))
	{
		pDataTable->nRateNo = 3;
	}
	
	//需量统计处理
	AcCalcDemand(&sTime);
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: AC分钟冻结处理
*********************************************************************************/
void AcMinFreezeDataRdyProc(const pSTD_TIME pTime)
{
    INT32S nEndSec   = 0;
    INT32S nStartSec = 0;
    STD_TIME		     sTime;
	pSTD_TIME		     pStart = &sTime;
    INT32U			     pRcvNumb =0;
    INT32U			     nPriority =0;
    FORMAT17             sPeriodicityMinDJ;
    MEM_AC_MIN_DATA       sAcdDjData;
    
    memset(&sPeriodicityMinDJ, 0, sizeof(FORMAT17));
    MemReadAcPrm(F43, &sPeriodicityMinDJ, sizeof(FORMAT17));
    if(((0x00 == sPeriodicityMinDJ.nMon)||(0xff == sPeriodicityMinDJ.nMon)|| (pTime->nMon == sPeriodicityMinDJ.nMon))
    	&& ((0x00 == sPeriodicityMinDJ.nDay) ||(0xff == sPeriodicityMinDJ.nDay) || (pTime->nDay == sPeriodicityMinDJ.nDay))
    		&& ((0x00 == sPeriodicityMinDJ.nHour) ||(0xff == sPeriodicityMinDJ.nHour) || (pTime->nHour == sPeriodicityMinDJ.nHour))
    			&& ((0x00 == sPeriodicityMinDJ.nMin) ||(0xff == sPeriodicityMinDJ.nMin) ||(pTime->nMin == sPeriodicityMinDJ.nMin)))
    {
        memset(&sAcdDjData, 0, sizeof(sAcdDjData));
        MemReadAcHistDataProc(F5, 1, &sAcdDjData, sizeof(MEM_AC_MIN_DATA));
        if ((0 != memcmp(&sAcdDjData.sTime.nMin, &pTime->nMin, 5)))
        {
            memcpy(&pStart->nMin, &sAcdDjData.sTime.nMin, sizeof(FORMAT15));
            nEndSec = GetMinsFrom2KYear(pTime);
            nStartSec = GetMinsFrom2KYear(&sTime);
            pRcvNumb =nEndSec-nStartSec;
            if(pRcvNumb>=0x0f)
            {
                nPriority = sAcdDjData.sPriority;
                memset(&sAcdDjData, 0, sizeof(MEM_AC_MIN_DATA));
                //AcCalcDJData(&sAcdDjData, pTime);
                MemReadAcRealData(F1, &sAcdDjData.sAcEnergy, sizeof(MT_DGN_ENERGY_DATA));
                MemReadAcRealData(F2, &sAcdDjData.sAcQEnergy, sizeof(MT_DGN_Q_ENERGY_DATA));
                MemReadAcRealData(F7, &sAcdDjData.sAcDemand, sizeof(MT_DGN_DEMAND_DATA));
                
                memcpy(&sAcdDjData.sRealP[0], &pDataTable->sRealInstant.sRealP[0], (MAX_PHASE_NUM + 1) * sizeof(FORMAT09));
                memcpy(&sAcdDjData.sRealQ[0], &pDataTable->sRealInstant.sRealQ[0], (MAX_PHASE_NUM + 1) * sizeof(FORMAT09));
                memcpy(&sAcdDjData.sTime, &pTime->nMin, sizeof(FORMAT15));
                memcpy(&sAcdDjData.sRealVolt[0], &pDataTable->sRealInstant.sRealVolt[0], (MAX_PHASE_NUM ) * sizeof(FORMAT07));
                memcpy(&sAcdDjData.sRealCurr[0], &pDataTable->sRealInstant.sRealCurr[0], (MAX_PHASE_NUM + 1) * sizeof(FORMAT25));
                sAcdDjData.sPriority = nPriority + 1;
                MemWriteAcHistDataProc(F5, 0, &sAcdDjData, sizeof(MEM_AC_MIN_DATA),1);
            }
        }
    }
}
/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: AC定时冻结处理
*********************************************************************************/
void AcCalcPerDataRdyProc(const pSTD_TIME pTime)
{
    FORMAT17 sPeriodicityDJ;
    MEM_AC_DJ_DATA sAcdDjData;
    
    memset(&sPeriodicityDJ, 0, sizeof(FORMAT17));
    MemReadAcPrm(F43, &sPeriodicityDJ, sizeof(FORMAT17));
    if((0x99 == sPeriodicityDJ.nMon)
    	&& ((0x99 == sPeriodicityDJ.nDay) || (pTime->nDay == sPeriodicityDJ.nDay))
    		&& ((0x99 == sPeriodicityDJ.nHour) || (pTime->nHour == sPeriodicityDJ.nHour))
    			&& (pTime->nMin == sPeriodicityDJ.nMin))
    {
        memset(&sAcdDjData, 0, sizeof(sAcdDjData));
        MemReadAcHistDataProc(F4, 1, &sAcdDjData, sizeof(MEM_AC_DJ_DATA));
        if (0 != memcmp(&sAcdDjData.sTime.nHour, &pTime->nHour, 4))
        {
            memset(&sAcdDjData, 0, sizeof(MEM_AC_DJ_DATA));
            AcCalcDJData(&sAcdDjData, pTime);
            MemWriteAcHistDataProc(F4, 0, &sAcdDjData, sizeof(MEM_AC_DJ_DATA),1);
        }
    }
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: AC日冻结处理
*********************************************************************************/
void AcCalcDayDataRdyProc(const pSTD_TIME pTime)
{
    MEM_AC_DAY_DATA sAcDayData;
    MEM_AC_DAY_DATA sAcHistDayData;
    STD_TIME        sTmpTime;
    FORMAT15        sStoreTime;
    INT32S          nRet    = 0;
    INT32S          nSecs   = 0;
    INT8U           nBuf[10] = {0x11,0x22,0x33};
    INT8U           *pData   = &nBuf[0];
    INT32U			nPriority =0;
    INT8U           ii = 0;
    INT8U           i = 0;
    
    memset(&sAcDayData, 0, sizeof(MEM_AC_DAY_DATA));
    memset(&sAcHistDayData, 0, sizeof(MEM_AC_DAY_DATA));
    memset(&sTmpTime, 0, sizeof(STD_TIME));
    memset(&sStoreTime, 0, sizeof(STD_TIME));
    
    if (0 != memcmp(&g_pSysTime->nDay, &pTime->nDay, 3))
    {
        //判断是不是只过了一天
        GetLastDay(&sTmpTime, pTime);
        if (0 == memcmp(&g_pSysTime->nDay, &sTmpTime.nDay, 3))
        {
            MemReadAcHistDataProc(F1, 1, &sAcDayData, sizeof(MEM_AC_DAY_DATA));
            if(sAcDayData.sPriority == 0xFF)
            {
                nPriority =0;
            }
            else
            {
              nPriority = sAcDayData.sPriority;
            }
            //AC过日常规处理
            memset(&sAcDayData, 0, sizeof(MEM_AC_DAY_DATA));
            
            MemReadAcRealData(F1, &sAcDayData.sAcEnergy, sizeof(MT_DGN_ENERGY_DATA));
            MemReadAcRealData(F2, &sAcDayData.sAcQEnergy, sizeof(MT_DGN_Q_ENERGY_DATA));
            MemReadAcRealData(F3, &sAcDayData.sAcTriEnergy, sizeof(MT_DGN_TRI_ENERGY_DATA));
            MemReadAcRealData(F4, &sAcDayData.sAcTriQEnergy, sizeof(MT_DGN_TRI_Q_EN_DATA));
            MemReadAcRealData(F7, &sAcDayData.sAcDemand, sizeof(MT_DGN_DEMAND_DATA));
            MemReadAcRealData(F8, &sAcDayData.sAcQDemand, sizeof(MT_DGN_Q_DE_DATA));
            
            memcpy(&sAcDayData.sRealP[0], &pDataTable->sRealInstant.sRealP[0], (MAX_PHASE_NUM + 1) * sizeof(FORMAT09));
            memcpy(&sAcDayData.sRealQ[0], &pDataTable->sRealInstant.sRealQ[0], (MAX_PHASE_NUM + 1) * sizeof(FORMAT09));
            
            memcpy(&sStoreTime.nDay, &pTime->nDay, 3);//将冻结数据小时、分去掉
            //memcpy(&sStoreTime.nDay, &g_pSysTime->nDay, 3);//将冻结数据小时、分去掉
            memcpy(&sAcDayData.sTime, &sStoreTime, sizeof(FORMAT15));
            for(int ii = 0; ii<31; ii++)
            {
                memset(&sAcHistDayData, 0, sizeof(sAcHistDayData));
                MemReadAcHistData(F1, ii, &sAcHistDayData, sizeof(sAcHistDayData));
                if( 0 == memcmp(&sAcDayData.sTime.nDay,&sAcHistDayData.sTime.nDay,3))
                {
                    i = ii;
                    ii = 33;
                }
            }

            if( 0 == memcmp(&sAcDayData.sTime.nDay,&sAcHistDayData.sTime.nDay,3))
            {
                sAcDayData.sPriority =  sAcHistDayData.sPriority;
                MemWriteAcHistData(F1, i, &sAcDayData, sizeof(MEM_AC_DAY_DATA));
                //MemWriteAcHistDataProc(F1, 31 - i, &sAcDayData, sizeof(MEM_AC_DAY_DATA), 0);
            }
            else
            {
                sAcDayData.sPriority =  nPriority +1;
                MemWriteAcHistDataProc(F1, 0, &sAcDayData, sizeof(MEM_AC_DAY_DATA), 1);
            }
            
        }
        
    }
    //重新赋值全局变量
    //memcpy(&g_pSysTime->nSec, &pTime->nSec, 6);
    //写铁电
    //MemWriteAcXBackData(F30, g_pSysTime, sizeof(STD_TIME));
    
#if 0
    nRet = MemReadAcHistDataProc(F1, 1, &sAcDayData,sizeof(MEM_AC_DAY_DATA));
    if (nRet < 0)
    {
        return;
    }
    //AC过日处理
    if (0 != memcmp(&sAcDayData.sTime.nDay, &pTime->nDay, sizeof(FORMAT20)))
    {
        //AC过日常规处理
        memset(&sAcDayData, 0, sizeof(MEM_AC_DAY_DATA));
        
        MemReadAcRealData(F1, &sAcDayData.sAcEnergy, sizeof(MT_DGN_ENERGY_DATA));
        MemReadAcRealData(F2, &sAcDayData.sAcQEnergy, sizeof(MT_DGN_Q_ENERGY_DATA));
        MemReadAcRealData(F3, &sAcDayData.sAcTriEnergy, sizeof(MT_DGN_TRI_ENERGY_DATA));
        MemReadAcRealData(F4, &sAcDayData.sAcTriQEnergy, sizeof(MT_DGN_TRI_Q_EN_DATA));
        MemReadAcRealData(F7, &sAcDayData.sAcDemand, sizeof(MT_DGN_DEMAND_DATA));
        MemReadAcRealData(F8, &sAcDayData.sAcQDemand, sizeof(MT_DGN_Q_DE_DATA));
        
        memcpy(&sAcDayData.sRealP[0], &pDataTable->sRealInstant.sRealP[0], (MAX_PHASE_NUM + 1) * sizeof(FORMAT09));
		memcpy(&sAcDayData.sRealQ[0], &pDataTable->sRealInstant.sRealQ[0], (MAX_PHASE_NUM + 1) * sizeof(FORMAT09));
    	memcpy(&sAcDayData.sTime, &pTime->nMin, sizeof(FORMAT15));
        
        MemWriteAcHistDataProc(F1, 0, &sAcDayData, sizeof(MEM_AC_DAY_DATA), 1);
    }
#endif
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: AC月冻结处理
*********************************************************************************/
void AcCalcMonDataRdyProc(const pSTD_TIME pTime)
{
    MEM_AC_DJ_DATA  sAcMonData;
    MEM_AC_DJ_DATA  sAcHistMonData;
    STD_TIME        sTmpTime;
    FORMAT15        sStoreTime;
    INT32S          nRet    = 0;
    INT32S          nSecs   = 0;
    INT8U           nBuf[10] = {0x11,0x22,0x33};
    INT8U           *pData   = &nBuf[0];
    INT32U			nPriority =0;
    INT8U           ii = 0;
    INT8U           i = 0;
    
    memset(&sAcMonData, 0, sizeof(MEM_AC_DJ_DATA));
    memset(&sAcHistMonData, 0, sizeof(MEM_AC_DJ_DATA));
    memset(&sTmpTime, 0, sizeof(STD_TIME));
    memset(&sStoreTime, 0, sizeof(STD_TIME));
    
    if (0 != memcmp(&g_pSysTime->nMon, &pTime->nMon, 2))
    {
        //判断是不是只过了一月
        GetLastMon(&sTmpTime, pTime);
        if (0 == memcmp(&g_pSysTime->nMon, &sTmpTime.nMon, 2))
        {
            MemReadAcHistDJDataProc(F7, 1, &sAcMonData, sizeof(MEM_AC_DJ_DATA));

            //AC过月常规处理
            memset(&sAcMonData, 0, sizeof(MEM_AC_DJ_DATA));
            
            MemReadAcRealData(F1, &sAcMonData.sAcEnergy, sizeof(MT_DGN_ENERGY_DATA));
            MemReadAcRealData(F2, &sAcMonData.sAcQEnergy, sizeof(MT_DGN_Q_ENERGY_DATA));
            //MemReadAcRealData(F3, &sAcMonData.sAcTriEnergy, sizeof(MT_DGN_TRI_ENERGY_DATA));
            //MemReadAcRealData(F4, &sAcMonData.sAcTriQEnergy, sizeof(MT_DGN_TRI_Q_EN_DATA));
            MemReadAcRealData(F7, &sAcMonData.sAcDemand, sizeof(MT_DGN_DEMAND_DATA));
            //MemReadAcRealData(F8, &sAcMonData.sAcQDemand, sizeof(MT_DGN_Q_DE_DATA));
            
            memcpy(&sAcMonData.sRealP[0], &pDataTable->sRealInstant.sRealP[0], (MAX_PHASE_NUM + 1) * sizeof(FORMAT09));
            memcpy(&sAcMonData.sRealQ[0], &pDataTable->sRealInstant.sRealQ[0], (MAX_PHASE_NUM + 1) * sizeof(FORMAT09));
            
            memcpy(&sStoreTime.nDay, &pTime->nDay, 3);//将冻结数据小时、分去掉
            //memcpy(&sStoreTime.nDay, &g_pSysTime->nDay, 3);//将冻结数据小时、分去掉
            memcpy(&sAcMonData.sTime, &sStoreTime, sizeof(FORMAT15));
            for(int ii = 0; ii<12; ii++)
            {
                memset(&sAcHistMonData, 0, sizeof(sAcHistMonData));
                MemReadAcHistDJDataProc(F7, ii, &sAcHistMonData, sizeof(MEM_AC_DJ_DATA));
                if( 0 == memcmp(&sAcMonData.sTime.nMon,&sAcHistMonData.sTime.nMon,2))
                {
                    i = ii;
                    ii = 33;
                }
            }

            if( 0 == memcmp(&sAcMonData.sTime.nMon,&sAcHistMonData.sTime.nMon,2))
            {
                //MemWriteAcHistData(F7, i, &sAcMonData, sizeof(MEM_AC_DJ_DATA));
                MemWriteAcDJHistDataProc(F7, i, &sAcMonData, sizeof(MEM_AC_DJ_DATA), 0);
            }
            else
            {
                MemWriteAcDJHistDataProc(F7, 0, &sAcMonData, sizeof(MEM_AC_DJ_DATA), 1);
            }
            
        }
        
    }
    //重新赋值全局变量
    memcpy(&g_pSysTime->nSec, &pTime->nSec, 6);
    //写铁电
    MemWriteAcXBackData(F30, g_pSysTime, sizeof(STD_TIME));
    
#if 0
    nRet = MemReadAcHistDataProc(F1, 1, &sAcDayData,sizeof(MEM_AC_DAY_DATA));
    if (nRet < 0)
    {
        return;
    }
    //AC过日处理
    if (0 != memcmp(&sAcDayData.sTime.nDay, &pTime->nDay, sizeof(FORMAT20)))
    {
        //AC过日常规处理
        memset(&sAcDayData, 0, sizeof(MEM_AC_DAY_DATA));
        
        MemReadAcRealData(F1, &sAcDayData.sAcEnergy, sizeof(MT_DGN_ENERGY_DATA));
        MemReadAcRealData(F2, &sAcDayData.sAcQEnergy, sizeof(MT_DGN_Q_ENERGY_DATA));
        MemReadAcRealData(F3, &sAcDayData.sAcTriEnergy, sizeof(MT_DGN_TRI_ENERGY_DATA));
        MemReadAcRealData(F4, &sAcDayData.sAcTriQEnergy, sizeof(MT_DGN_TRI_Q_EN_DATA));
        MemReadAcRealData(F7, &sAcDayData.sAcDemand, sizeof(MT_DGN_DEMAND_DATA));
        MemReadAcRealData(F8, &sAcDayData.sAcQDemand, sizeof(MT_DGN_Q_DE_DATA));
        
        memcpy(&sAcDayData.sRealP[0], &pDataTable->sRealInstant.sRealP[0], (MAX_PHASE_NUM + 1) * sizeof(FORMAT09));
		memcpy(&sAcDayData.sRealQ[0], &pDataTable->sRealInstant.sRealQ[0], (MAX_PHASE_NUM + 1) * sizeof(FORMAT09));
    	memcpy(&sAcDayData.sTime, &pTime->nMin, sizeof(FORMAT15));
        
        MemWriteAcHistDataProc(F1, 0, &sAcDayData, sizeof(MEM_AC_DAY_DATA), 1);
    }
#endif
}


/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: AC结算日处理
*********************************************************************************/
void AcCalcRDayDataRdyProc(const pSTD_TIME pTime)
{
    FORMAT24 sAcRDTime[3];
    MEM_AC_RDAY_DATA sAcRDayData;
	MT_DGN_DEMAND_DATA sAcDemand;          //需量数据
    MT_DGN_Q_DE_DATA sAcQDemand;         //分象限需量数据
    FP32 fAcDemand[MAX_P_Q_NUM+4][MAX_FEE_NUM+1];
    STD_TIME    sTmpTime;

    memset(&sTmpTime, 0, sizeof(STD_TIME));
    MemReadAcPrm(F32, &sAcRDTime[0], sizeof(FORMAT24));
    MemReadAcPrm(F33, &sAcRDTime[1], sizeof(FORMAT24));
    MemReadAcPrm(F34, &sAcRDTime[2], sizeof(FORMAT24));
    
    GetLastMon(&sTmpTime, pTime);
    
    if((sAcRDTime[0].nDay == pTime->nDay && sAcRDTime[0].nHour == pTime->nHour)
      ||(sAcRDTime[1].nDay == pTime->nDay && sAcRDTime[1].nHour == pTime->nHour)
       ||(sAcRDTime[2].nDay == pTime->nDay && sAcRDTime[2].nHour == pTime->nHour)
           || (g_IsNewMonCalcRDayFlg))
    {
        g_IsNewMonCalcRDayFlg = 0;
        memset(&sAcRDayData, 0, sizeof(MEM_AC_RDAY_DATA));
        
        MemReadAcHistDataProc(F2, 1, &sAcRDayData, sizeof(MEM_AC_RDAY_DATA));
        if (0 == memcmp(&sAcRDayData.sTime.nDay, &pTime->nDay, sizeof(FORMAT20)))
        {
            //return;
        }
        //AC抄表日常规处理
        memset(&sAcRDayData, 0, sizeof(MEM_AC_RDAY_DATA));
        MemReadAcRealData(F1, &sAcRDayData.sAcEnergy, sizeof(MT_DGN_ENERGY_DATA));
        MemReadAcRealData(F2, &sAcRDayData.sAcQEnergy, sizeof(MT_DGN_Q_ENERGY_DATA));
        MemReadAcRealData(F3, &sAcRDayData.sAcTriEnergy, sizeof(MT_DGN_TRI_ENERGY_DATA));
        MemReadAcRealData(F4, &sAcRDayData.sAcTriQEnergy, sizeof(MT_DGN_TRI_Q_EN_DATA));
        MemReadAcRealData(F7, &sAcRDayData.sAcDemand, sizeof(MT_DGN_DEMAND_DATA));
        MemReadAcRealData(F8, &sAcRDayData.sAcQDemand, sizeof(MT_DGN_Q_DE_DATA));
        
        memcpy(&sAcRDayData.sRealP[0], &pDataTable->sRealInstant.sRealP[0], (MAX_PHASE_NUM + 1) * sizeof(FORMAT09));
    	memcpy(&sAcRDayData.sRealQ[0], &pDataTable->sRealInstant.sRealQ[0], (MAX_PHASE_NUM + 1) * sizeof(FORMAT09));
    	memcpy(&sAcRDayData.sTime, &pTime->nMin, sizeof(FORMAT15));
        
        MemWriteAcHistDataProc(F2, 0, &sAcRDayData, sizeof(MEM_AC_RDAY_DATA), 1);
        
        memset(&sAcDemand, 0, sizeof(sAcDemand));
    	memset(&sAcQDemand, 0, sizeof(sAcQDemand));
    	memset(&fAcDemand, 0, sizeof(fAcDemand));
        
        MemWriteAcRealData(F7, &sAcDemand, sizeof(sAcDemand));
        MemWriteAcRealData(F8, &sAcQDemand, sizeof(sAcQDemand));
        MemWriteAcRealData(F13, &fAcDemand, sizeof(fAcDemand));
        
        memset(&pSavDataTable->sDemandData[0], 0, (MAX_P_Q_NUM + 4) * sizeof(AC_DEMAND_DATA));
		memset(&pDataTable->sDemandSlipData1[0], 0, (MAX_P_Q_NUM + 4) * sizeof(DemandSlip_Data1));
		memset(&pDataTable->sDemandSlipData1[0], 0, (MAX_P_Q_NUM + 4) * sizeof(DemandSlip_Data2));
    }
}

/*
*****************************************************************************************************
* 函数名: AcCalcMeterPrmChanEve
* 输　入:
* 输  出:
* 返  回:
* 功能描述:电表编程事件处理(包含校时事件处理)
*****************************************************************************************************
*/
void AcCalcMeterPrmChanEve(const INT8U* pData, INT8U nType)
{
    
}


//组合电能计算
void CombEnergyCal(pFORMAT11 pEnergy, INT16S *pPulse, INT16U nPulseCnt)
{
    INT8U nFlag = 0;
    INT32U nTmp = 0;
    INT32S nComb = 0;
    FORMAT11 sComb;

    memset(&sComb, 0, sizeof(FORMAT11));
    memcpy(&sComb, pEnergy, sizeof(FORMAT11));
    if (sComb.nD4 & 0x80)
    {
        nFlag = 1;
    }
    sComb.nD4 &= 0x7F;
    nComb = BCD2INT32U((INT8U*)&sComb, 8);
    if (nFlag)
    {
        nComb = 0 - nComb;
    }
    if (g_MeterAcErrFlg[2])//倒走
    {
        if (nComb < (INT32S)(*pPulse/nPulseCnt))
        {
            nComb = 0;
        }
        else
        {
            nComb -= (INT32S)(*pPulse/nPulseCnt);
        }
    }
    else
    {
        nComb += (INT32S)(*pPulse/nPulseCnt);
    }
    
    if (nComb < 0)
    {
        nFlag = 1;
    }
    else
    {
        nFlag = 0;
    }
    nTmp = abs(nComb) % 80000000;
    INT32U2BCD(nTmp, 8, (INT8U*)pEnergy);
    if (nFlag)
    {
        pEnergy->nD4 |= 0x80;
    }
    *pPulse = (INT16S)(*pPulse % nPulseCnt);
}

/*
*****************************************************************************************************
* 函数名:AcCalcChangePg
* 输　入:INT8U Phase, FP32	Pg,	FP32 Y, INT8U uPFlag
* 输  出:
* 返  回:相位角
* 功能描述:计算相位角
*****************************************************************************************************
*/
FP32 AcCalcChangePg(INT8U uPhase, FP32 fPg, FP32 fY, INT8U uPFlag)
{
	INT32U	nTmp;

	fPg = 360.0+fPg+fY;
	nTmp = (INT32U)(fPg*10);
	nTmp = nTmp%3600;
	fPg = (FP32)nTmp/10;
	return fPg;
}


/********************************************************************************
* 函数名:FirstWriteAcENData
* 输　入:
* 输  出:
* 返  回:
* 功  能:上电第一次写
*********************************************************************************/
INT8S FirstWriteAcENData(void)
{
    INT16U   i    = 0;
    INT8U       nErr    = 0;
    INT32U   nTmp = 0;
    //INT32U   nTmp1 = 0;
    //MEM_AC_REAL_DATA sRealData;             //使用全局变量
    //pMEM_AC_REAL_DATA    pRealData = &sRealData;
    pMEM_AC_REAL_DATA    pRealData = (pMEM_AC_REAL_DATA)&nAcTmpBuf[0];
    
    //OSSemPend(g_pSem[SEM_AC_BUF_IDX], 0, &nErr);
    memset(pRealData, 0, sizeof(MEM_AC_REAL_DATA));
    
    if (MemReadAcRealData(FEND, pRealData, sizeof(MEM_AC_REAL_DATA)) < 0)
    {
        //OSSemPost(g_pSem[SEM_AC_BUF_IDX]);
        return RET_ERR_ERROR;
    }
    
    AcRealDataMakeAndStoreProc(pRealData);
    //OSSemPost(g_pSem[SEM_AC_BUF_IDX]);
    return 0;
}


          
          
