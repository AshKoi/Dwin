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
#ifndef _MEM_DATA_H_
#define _MEM_DATA_H_

#include "Os_cpu.h"
#include "Macro.h"
#include "Table.h"
#include "global.h"
#include "Ac7022.h"


/**************************************************************************************************
                                        测量点实时数据
***************************************************************************************************/
typedef struct _tagMEM_MP_REAL_DATA
{
	INT8U       	     		nMeterAddr[LEN_METER_ADDR];		    	    //电表地址(标签)
	INT8U						nReadTime[6];             	                //读表时间								//读表时间YY-MM-DD-HH-MM-SS
	
	INT8U						nDate[4];									//电表日期
	INT8U						nTime[3];									//电表时钟
	INT8U						nEnergyPP[MAX_FEE_NUM + 1][4];				//正向有功
	INT8U						nEnergyNP[MAX_FEE_NUM + 1][4];				//反向有功
	INT8U						nEnergyPN[MAX_FEE_NUM + 1][4];				//正向无功
	INT8U						nEnergyNN[MAX_FEE_NUM + 1][4];				//反向无功					
	INT8U                       nQ1Energy[MAX_FEE_NUM + 1][4];				//一象限
	INT8U                       nQ2Energy[MAX_FEE_NUM + 1][4];				//二象限
	INT8U                       nQ3Energy[MAX_FEE_NUM + 1][4];				//三象限
	INT8U                       nQ4Energy[MAX_FEE_NUM + 1][4];				//四象限
	INT8U                       nPPDemand[MAX_FEE_NUM + 1][8];				//正向有功需量				
	INT8U                       nNPDemand[MAX_FEE_NUM + 1][8];				//反向有功需量
	INT8U                       nPNDemand[MAX_FEE_NUM + 1][8];				//正向无功需量
	INT8U                       nNNDemand[MAX_FEE_NUM + 1][8];				//反向无功需量
	INT8U                       nVolt[3][2];								//电压
	INT8U                       nCurr[3][3];								//电流
	INT8U						nPWatt[4][3];								//有功功率
	INT8U						nNWatt[4][3];								//无功功率
	INT8U						nSWatt[4][3];								//视在功率
	INT8U                       nFactor[4][2];								//功率因数
    
    MT_POWER_STAT       	    sDayPowerStat;                		        //日电量统计
    MT_POWER_STAT       	    sMonPowerStat;                		        //月电量统计
}MEM_MP_REAL_DATA,*pMEM_MP_REAL_DATA;


/**************************************************************************************************
                                        测量点日数据
***************************************************************************************************/
typedef struct _tagMEM_MP_DAY_DATA
{
	INT8U       	  			nMeterAddr[LEN_METER_ADDR];		    	//电表地址(标签)
	INT8U						sDataTime[3];							//时间(标签)YY-MM-DD
	INT8U						nReadTime[6];							//读表时间YY-MM-DD-HH-MM-SS
	
	INT8U                       nDEnergyPP[MAX_FEE_NUM + 1][4];			//日正向有功
	INT8U                       nDEnergyNP[MAX_FEE_NUM + 1][4];			//日反向有功
	INT8U                       nDEnergyPN[MAX_FEE_NUM + 1][4];			//日正向无功
	INT8U                       nDEnergyNN[MAX_FEE_NUM + 1][4];			//日反向无功
    INT8U                       nDQ1Energy[MAX_FEE_NUM + 1][4];			//日一象限
	INT8U                       nDQ2Energy[MAX_FEE_NUM + 1][4];			//日二象限
	INT8U                       nDQ3Energy[MAX_FEE_NUM + 1][4];			//日三象限
	INT8U                       nDQ4Energy[MAX_FEE_NUM + 1][4];			//日四象限
	INT8U                       nDPPDemand[MAX_FEE_NUM + 1][8];			//日正向有功需量
	INT8U                       nDNPDemand[MAX_FEE_NUM + 1][8];			//日反向有功需量
	INT8U                       nDPNDemand[MAX_FEE_NUM + 1][8];			//日正向无功需量日
	INT8U                       nDNNDemand[MAX_FEE_NUM + 1][8];			//反向无功需量

}MEM_MP_DAY_DATA,*pMEM_MP_DAY_DATA;


/**************************************************************************************************
                                        测量点月数据
***************************************************************************************************/
typedef struct _tagMEM_MP_MON_DATA
{
	INT8U       	  			nMeterAddr[LEN_METER_ADDR];		    	//电表地址(标签)
	INT8U						sDataTime[2];								//时间(标签)YY-MM
	INT8U						nReadTime[6];								//读表时间YY-MM-DD-HH-MM-SS
	
	INT8U                       nMEnergyPP[MAX_FEE_NUM + 1][4];			//月正向有功
	INT8U                       nMEnergyNP[MAX_FEE_NUM + 1][4];			//月反向有功
	INT8U                       nMEnergyPN[MAX_FEE_NUM + 1][4];			//月正向无功
	INT8U                       nMEnergyNN[MAX_FEE_NUM + 1][4];			//月反向无功
    INT8U                       nMQ1Energy[MAX_FEE_NUM + 1][4];			//月一象限
	INT8U                       nMQ2Energy[MAX_FEE_NUM + 1][4];			//月二象限
	INT8U                       nMQ3Energy[MAX_FEE_NUM + 1][4];			//月三象限
	INT8U                       nMQ4Energy[MAX_FEE_NUM + 1][4];			//月四象限
	INT8U                       nMPPDemand[MAX_FEE_NUM + 1][8];			//月正向有功需量
	INT8U                       nMNPDemand[MAX_FEE_NUM + 1][8];			//月反向有功需量
	INT8U                       nMPNDemand[MAX_FEE_NUM + 1][8];			//月正向无功需量
	INT8U                       nMNNDemand[MAX_FEE_NUM + 1][8];			//月反向无功需量
	

}MEM_MP_MON_DATA,*pMEM_MP_MON_DATA;


/**************************************************************************************************
                                        测量点曲线数据
***************************************************************************************************/
typedef struct _tagMEM_MP_CRV_DATA
{
	INT8U       	  			nMeterAddr[LEN_METER_ADDR];		    	//电表地址(标签)
	INT8U						sDataTime[5];								//时间(标签)YY-MM-DD-HH-MM
	
	INT8U                       nCrvEnergyPP[MAX_FEE_NUM + 1][4];			//月正向有功
	INT8U                       nCrvEnergyNP[MAX_FEE_NUM + 1][4];			//月反向有功
	INT8U                       nCrvEnergyPN[MAX_FEE_NUM + 1][4];			//月正向无功
	INT8U                       nCrvEnergyNN[MAX_FEE_NUM + 1][4];			//月反向无功
    INT8U                       nCrvVolt[3][2];							//电压
	INT8U                       nCrvCurr[3][3];							//电流
	INT8U						nCrvPWatt[4][3];							//有功功率
	INT8U						nCrvNWatt[4][3];							//无功功率
	INT8U						nCrvSWatt[4][3];							//视在功率
	INT8U                       nCrvFactor[4][2];							//功率因数
	
}MEM_MP_CRV_DATA,*pMEM_MP_CRV_DATA;


/**************************************************************************************************
                                        测量点抄表日数据
***************************************************************************************************/
typedef struct _tagMEM_MP_RDAY_DATA
{
	INT8U       	  			nMeterAddr[LEN_METER_ADDR];		    	//电表地址(标签)
	INT8U						sDataTime[3];								//时间(标签)YY-MM-DD
	
	INT8U						nRDEnergyPP[MAX_FEE_NUM + 1][4];			//正向有功
	INT8U						nRDEnergyNP[MAX_FEE_NUM + 1][4];			//反向有功
	INT8U						nRDEnergyPN[MAX_FEE_NUM + 1][4];			//正向无功
	INT8U						nRDEnergyNN[MAX_FEE_NUM + 1][4];			//反向无功	
	
}MEM_MP_RDAY_DATA,*pMEM_MP_RDAY_DATA;





/**************************************************************************************************
                                        (RAM临时交互)
***************************************************************************************************/
typedef struct _tagMEM_X_RAM_DATA
{
	INT8U               nRs485Real;                         //Rs485实时抄标志
    INT8U		        nRs485Day;                          //Rs485过日标志
    INT8U		        nRs485Mon;                          //Rs485过月标志
    INT8U		        nRs485Crv;                          //Rs485曲线标志
    INT8U               nResetFlg;                          //复位标志
    INT8U               nTripFlg[MAX_TURN_NUM];             //跳闸标志
    INT8U               nLockFlg[MAX_TURN_NUM];             //合闸标志
    INT8U		        nRmotSigl;							//远程Link信号强度
    INT8U               nLinkReq;                           //远程链接命令：F38,F39
    
    AC_FRE_STAT		    sAcFreeze;							//AC需量清零标志
	AC_FRE_STAT		    sAcInit;							//AC清零标志
	AC_EVECLR_STAT	    sAcEvent[3];						//AC事件清零标志
    
    INT8U               nRdMtProtFlg;                       //1:645 2400,2:3761 9600,不使用，
}MEM_X_RAM_DATA,*pMEM_X_RAM_DATA;



/**************************************************************************************************
                                        (FLASH临时交互)
***************************************************************************************************/
typedef struct _tagMEM_X_FLH_DATA
{
	INT8U				nRealNext;
	DAY_MEM_INFO		sDayMemInfo;
	MON_MEM_INFO		sMonMemInfo;
	CRV_MEM_INFO		sCrvMemInfo;
	RDAY_MEM_INFO		sRDayMemInfo;
    
    EVT_MEM_INFO        sRcEvtInfo;         //遥控事件信息
    RC_EVT_DATA         sRcEvtData[MAX_RC_EVT_NUM];     //遥控事件记录
    
    INT8U		        nACRunStat[LEN_RUN_STAT_FLAG]; 
}MEM_X_FLH_DATA,*pMEM_X_FLH_DATA;


/**************************************************************************************************
                                        (FRAM临时交互)
***************************************************************************************************/
typedef struct _tagMEM_X_FRAM_DATA
{
    INT8U               nMacAddr[6];
    INT8U               nFileInitFlg;
	STD_TIME			sTermClk;
    INT64U              nPulseNum[MAX_PULSE_NUM];
    PULSE_RATE_NUM      nPerRateNum[MAX_FEE_NUM];
    
    PULSE_RATE_NUM      nDayRateNum[MAX_FEE_NUM];
    PULSE_RATE_NUM      nMonRateNum[MAX_FEE_NUM];
    
    INT8U               nPsw[6];
    INT8U               nPswSetFlg;
    INT8U               nGprsFlg;

}MEM_X_FRAM_DATA,*pMEM_X_FRAM_DATA;


/**************************************************************************************************
                                          交采数据
***************************************************************************************************/

/**************************************************************************************************
                                          瞬时数据存储
***************************************************************************************************/
//交采瞬时数据 MEM_OBJ_AC_INSTANT
typedef struct _tagMEM_AC_INST_DATA
{
	AC_INSTANT_DATA	sData;
	//AC_SYNC_DATA	sSyncData;
    AC_AVEARGE_DATA sAver;
}MEM_AC_INST_DATA, *pMEM_AC_INST_DATA;

/**************************************************************************************************
                                          实时数据存储
***************************************************************************************************/
//交采实时数据 MEM_OBJ_AC_REAL
typedef struct _tagMEM_AC_REAL_DATA
{
	MT_DGN_ENERGY_DATA			sAcEnergy;
	MT_DGN_Q_ENERGY_DATA		sAcQEnergy;
	MT_DGN_TRI_ENERGY_DATA		sAcTriEnergy;
	MT_DGN_TRI_Q_EN_DATA		sAcTriQEnergy;
    MT_DGN_C_ENERGY_DATA        sAcCombEnergy;
    INT32U						nENChkSum;
	MT_DGN_DEMAND_DATA			sAcDemand;
	MT_DGN_Q_DE_DATA			sAcQDemand;
	ENERGY_DATA					sEPulse[8];
	PH_ENERGY_DATA				sPhEPulse[MAX_PHASE_NUM];
	PH_ENERGY_DATA				sPhQEPulse[MAX_PHASE_NUM];
    COMB_ENERGY_DATA            sCombEPulse;
	INT32U						nAcDemand[MAX_P_Q_NUM+4][MAX_FEE_NUM+1];
	INT8U						nVer[8];
	MT_DGN_ENERGY_DATA          sAcDamage;
    MT_DGN_ENERGY_DATA          sAcFirmLoss;
    MT_DGN_ENERGY_DATA          sAcCopperLoss;
    MT_DGN_ENERGY_DATA          sAcIronLoss;
    
	INT8U						nBack[4];
}MEM_AC_REAL_DATA, *pMEM_AC_REAL_DATA;



#if 0
/**************************************************************************************************
                                          实时数据存储
***************************************************************************************************/
//交采实时数据 MEM_OBJ_AC_REAL
typedef struct _tagMEM_AC_REAL_DATA
{
	MT_DGN_ENERGY_DATA			sAcEnergy;
	MT_DGN_Q_ENERGY_DATA		sAcQEnergy;
	MT_DGN_TRI_ENERGY_DATA		sAcTriEnergy;
	MT_DGN_TRI_Q_EN_DATA		sAcTriQEnergy;
	ENERGY_DATA					sEPulse[8];
	PH_ENERGY_DATA				sPhEPulse[MAX_PHASE_NUM];
	PH_ENERGY_DATA				sPhQEPulse[MAX_PHASE_NUM];
	INT16U						nEnergyCrc;

    LOSER_ENERGY_DATA			sAcLoserEnergy;
    LOSER_ACCU_DATA	            sLoserAccuData;
    INT16U						nLoserCrc;
    
	MT_DGN_DEMAND_DATA			sAcDemand;
	MT_DGN_Q_DE_DATA			sAcQDemand;
	FP32						fAcDemand[MAX_P_Q_NUM+4][MAX_FEE_NUM+1];
	INT16U						nDemandCrc;
	
	INT8U						nVer[8];
	//INT8U						nBack[130];
}MEM_AC_REAL_DATA, *pMEM_AC_REAL_DATA;
#endif

/**************************************************************************************************
                                          实时数据备份
***************************************************************************************************/
//交采实时数据备份 MEM_OBJ_AC_BACK
typedef struct _tagMEM_AC_REAL_BACK
{
	MT_DGN_ENERGY_DATA			sAcEnergy;
	MT_DGN_Q_ENERGY_DATA		sAcQEnergy;
	MT_DGN_TRI_ENERGY_DATA		sAcTriEnergy;
	MT_DGN_TRI_Q_EN_DATA		sAcTriQEnergy;
	ENERGY_DATA					sEPulse[8];
	PH_ENERGY_DATA				sPhEPulse[MAX_PHASE_NUM];
	PH_ENERGY_DATA				sPhQEPulse[MAX_PHASE_NUM];
	INT16U						nEnergyCrc;
}MEM_AC_REAL_BACK, *pMEM_AC_REAL_BACK;

/**************************************************************************************************
                                          日数据存储
***************************************************************************************************/
//交采日数据 MEM_OBJ_AC_DAY
typedef struct _tagMEM_AC_DAY_DATA
{
	MT_DGN_ENERGY_DATA			sAcEnergy;
	MT_DGN_Q_ENERGY_DATA		sAcQEnergy;
	MT_DGN_TRI_ENERGY_DATA		sAcTriEnergy;
	MT_DGN_TRI_Q_EN_DATA		sAcTriQEnergy;
	MT_DGN_DEMAND_DATA			sAcDemand;
	MT_DGN_Q_DE_DATA			sAcQDemand;
	FORMAT09					sRealP[MAX_PHASE_NUM+1];
	FORMAT09					sRealQ[MAX_PHASE_NUM+1];
	FORMAT15					sTime;
    INT32U	                    sPriority;        //记录序号 23.2.24
}MEM_AC_DAY_DATA, *pMEM_AC_DAY_DATA;

/**************************************************************************************************
                                          分钟数据存储
***************************************************************************************************/
//交采分钟数据 MEM_OBJ_AC_DAY
typedef struct _tagMEM_AC_MIN_DATA
{
	MT_DGN_ENERGY_DATA			sAcEnergy;
	MT_DGN_Q_ENERGY_DATA		sAcQEnergy;
	MT_DGN_DEMAND_DATA			sAcDemand;
	FORMAT09					sRealP[MAX_PHASE_NUM+1];
	FORMAT09					sRealQ[MAX_PHASE_NUM+1];
	FORMAT15					sTime;   
    FORMAT07		            sRealVolt[MAX_PHASE_NUM];  
	FORMAT25		            sRealCurr[MAX_PHASE_NUM+1]; 
    INT32U	                    sPriority;        //记录序号
}MEM_AC_MIN_DATA, *pMEM_AC_MIN_DATA;

typedef struct _tagMEM_EVE_DATA
{
    STD_TIME   sEveStartClk;
    STD_TIME   sEveFinshClk;

}MEM_EVE_DATA, *pMEM_EVE_DATA;

/**************************************************************************************************
                                          抄表日数据存储
***************************************************************************************************/
//交采结算日数据 MEM_OBJ_AC_RDAY
typedef struct _tagMEM_AC_RDAY_DATA
{
	MT_DGN_ENERGY_DATA			sAcEnergy;
	MT_DGN_Q_ENERGY_DATA		sAcQEnergy;
	MT_DGN_TRI_ENERGY_DATA		sAcTriEnergy;
	MT_DGN_TRI_Q_EN_DATA		sAcTriQEnergy;
    MT_DGN_C_ENERGY_DATA        sAcCombEnergy;
	MT_DGN_DEMAND_DATA			sAcDemand;
	MT_DGN_Q_DE_DATA			sAcQDemand;
	FORMAT09					sRealP[MAX_PHASE_NUM+1];
	FORMAT09					sRealQ[MAX_PHASE_NUM+1];
	FORMAT15					sTime;
}MEM_AC_RDAY_DATA, *pMEM_AC_RDAY_DATA;

/**************************************************************************************************
                                          冻结数据存储
***************************************************************************************************/
//冻结数据 MEM_OBJ_AC_SSDJ, MEM_OBJ_AC_DSDJ, MEM_OBJ_AC_SQDJ, MEM_OBJ_AC_SDDJ
typedef struct _tagMEM_AC_DJ_DATA
{
	MT_DGN_ENERGY_DATA			sAcEnergy;
	MT_DGN_Q_ENERGY_DATA		sAcQEnergy;
	MT_DGN_DEMAND_DATA			sAcDemand;
    MT_DGN_C_ENERGY_DATA        sAcCombEnergy;
	FORMAT09					sRealP[MAX_PHASE_NUM+1];
	FORMAT09					sRealQ[MAX_PHASE_NUM+1];
	FORMAT15					sTime;
}MEM_AC_DJ_DATA, *pMEM_AC_DJ_DATA;

/**************************************************************************************************
                                           历史数据存储
***************************************************************************************************/
typedef struct _tagMEM_AC_HIST_DATA
{
    MEM_AC_DAY_DATA     sDayData[MAX_AC_DAY_DATA_NUM];
    MEM_AC_RDAY_DATA    sRdDayData[MAX_AC_RDAY_DATA_NUM];
    MEM_AC_DJ_DATA      sSsdjData[MAX_AC_SSDJ_DATA_NUM];
    MEM_AC_DJ_DATA      sDsdjData[MAX_AC_DSDJ_DATA_NUM];
    MEM_AC_MIN_DATA     sSqdjData[MAX_AC_SQDJ_DATA_NUM];
    MEM_AC_DJ_DATA      sSddjData[MAX_AC_SDDJ_DATA_NUM];
    MEM_AC_DJ_DATA      sMonDayData[MAX_AC_MON_DATA_NUM];
    MEM_AC_DJ_DATA      sNULLDayData[1];
}MEM_AC_HIST_DATA, *pMEM_AC_HIST_DATA;


/**************************************************************************************************
                                           事件数据存储
***************************************************************************************************/
typedef struct _tagMEM_AC_EVT
{
	MEM_AC_EVT_DATA			sACEvtSyA[MAX_AC_EVT_DATA_NUM];			//失压事件(A)
    MEM_AC_EVT_DATA			sACEvtSyB[MAX_AC_EVT_DATA_NUM];			//失压事件(B)
	MEM_AC_EVT_DATA			sACEvtSyC[MAX_AC_EVT_DATA_NUM];			//失压事件(C)
	MEM_AC_EVT_DATA			sACEvtDxA[MAX_AC_EVT_DATA_NUM];			//断相事件(A)
	MEM_AC_EVT_DATA			sACEvtDxB[MAX_AC_EVT_DATA_NUM];			//断相事件(B)
	MEM_AC_EVT_DATA			sACEvtDxC[MAX_AC_EVT_DATA_NUM];			//断相事件(C)
	MEM_AC_EVT_DATA			sACEvtQsy[MAX_AC_EVT_DATA_NUM];			//全失压事件
	MEM_AC_EVT_DATA			sACEvtDynx[MAX_AC_EVT_DATA_NUM];		//电压逆相事件
	MEM_AC_EVT_DATA			sACEvtSlA[MAX_AC_EVT_DATA_NUM];			//失流事件(A)
	MEM_AC_EVT_DATA			sACEvtSlB[MAX_AC_EVT_DATA_NUM];			//失流事件(B)
	MEM_AC_EVT_DATA			sACEvtSlC[MAX_AC_EVT_DATA_NUM];			//失流事件(C)
	MEM_AC_EVT_DATA			sACEvtDd[MAX_AC_EVT_DATA_NUM];			//掉电事件
	MEM_AC_EVT_DATA			sACEvtXlql[MAX_AC_EVT_DATA_NUM];		//需量清零事件
	MEM_AC_EVT_DATA			sACEvtDbql[MAX_AC_EVT_DATA_NUM];		//电表清零事件
	MEM_AC_EVT_DATA			sACEvtSjql[MAX_AC_EVT_DATA_NUM];		//事件清零事件
	MEM_AC_EVT_DATA			sACEvtBc[MAX_AC_EVT_DATA_NUM];			//编程事件
	MEM_AC_EVT_DATA			sACEvtJs[MAX_AC_EVT_DATA_NUM];			//校时事件
    
    MEM_AC_EVT_DATA			sACEvtGyA[MAX_AC_EVT_DATA_NUM];			//过压事件(A)
    MEM_AC_EVT_DATA			sACEvtGyB[MAX_AC_EVT_DATA_NUM];			//过压事件(B)
	MEM_AC_EVT_DATA			sACEvtGyC[MAX_AC_EVT_DATA_NUM];			//过压事件(C)
    
    MEM_AC_EVT_DATA			sACEvtGlA[MAX_AC_EVT_DATA_NUM];			//过流事件(A)
    MEM_AC_EVT_DATA			sACEvtGlB[MAX_AC_EVT_DATA_NUM];			//过流事件(B)
	MEM_AC_EVT_DATA			sACEvtGlC[MAX_AC_EVT_DATA_NUM];			//过流事件(C)
    
    MEM_AC_EVT_DATA			sACEvtClA[MAX_AC_EVT_DATA_NUM];			//潮流反向事件(A)
    MEM_AC_EVT_DATA			sACEvtClB[MAX_AC_EVT_DATA_NUM];			//潮流反向事件(B)
	MEM_AC_EVT_DATA			sACEvtClC[MAX_AC_EVT_DATA_NUM];			//潮流反向事件(C)
    
    MEM_AC_EVT_DATA			sACEvtGzA[MAX_AC_EVT_DATA_NUM];			//过载事件(A)
    MEM_AC_EVT_DATA			sACEvtGzB[MAX_AC_EVT_DATA_NUM];			//过载事件(B)
	MEM_AC_EVT_DATA			sACEvtGzC[MAX_AC_EVT_DATA_NUM];			//过载事件(C)
    
    MEM_AC_EVT_DATA         sACEvtKG[MAX_AC_EVT_DATA_NUM];
    
    MEM_AC_EVT_DATA         sACEvtTZ[MAX_AC_EVT_DATA_NUM];          //跳闸事件22.2.16
    MEM_AC_EVT_DATA         sACEvtHZ[MAX_AC_EVT_DATA_NUM];          //合闸事件22.2.17
    MEM_AC_EVT_DATA         sACEvtMY[MAX_AC_EVT_DATA_NUM];          //密钥事件22.2.17
    MEM_AC_EVT_DATA         sACEvtFH[MAX_AC_EVT_DATA_NUM];          //负荷事件22.2.17
    MEM_AC_EVT_DATA         sACEvtDY[MAX_AC_EVT_DATA_NUM];          //电源事件22.2.17
    MEM_AC_EVT_DATA         sACEvtSZ[MAX_AC_EVT_DATA_NUM];          //时钟事件22.2.17
    MEM_AC_EVT_DATA         sACEvtJL[MAX_AC_EVT_DATA_NUM];          //计量芯片事件22.2.17
    MEM_AC_EVT_DATA         sACEvtglA[MAX_AC_EVT_DATA_NUM];          //
    MEM_AC_EVT_DATA         sACEvtglB[MAX_AC_EVT_DATA_NUM];          //
    MEM_AC_EVT_DATA         sACEvtglC[MAX_AC_EVT_DATA_NUM];          //
    MEM_AC_EVT_DATA         sACEvtBSKG[MAX_AC_EVT_DATA_NUM];          //
    MEM_AC_EVT_DATA         sACEvtBSTZ[MAX_AC_EVT_DATA_NUM];          //
    MEM_AC_EVT_DATA         sACEvtBSHZ[MAX_AC_EVT_DATA_NUM];          //
    MEM_AC_EVT_DATA         sACEvtBSMY[MAX_AC_EVT_DATA_NUM];          //
    MEM_AC_EVT_DATA         sACEvtBSFH[MAX_AC_EVT_DATA_NUM];          //
    MEM_AC_EVT_DATA         sACEvtBSDY[MAX_AC_EVT_DATA_NUM];          //
    MEM_AC_EVT_DATA         sACEvtBSSZ[MAX_AC_EVT_DATA_NUM];          //
    MEM_AC_EVT_DATA         sACEvtBSJL[MAX_AC_EVT_DATA_NUM];          //
    
    MEM_AC_EVT_DATA			sACEvtQyA[MAX_AC_EVT_DATA_NUM];			//欠压事件(A)
    MEM_AC_EVT_DATA			sACEvtQyB[MAX_AC_EVT_DATA_NUM];			//欠压事件(B)
	MEM_AC_EVT_DATA			sACEvtQyC[MAX_AC_EVT_DATA_NUM];			//欠压事件(C)
    MEM_AC_EVT_DATA			sACEvtDlA[MAX_AC_EVT_DATA_NUM];			//断流事件(A)
	MEM_AC_EVT_DATA			sACEvtDlB[MAX_AC_EVT_DATA_NUM];			//断流事件(B)
	MEM_AC_EVT_DATA			sACEvtDlC[MAX_AC_EVT_DATA_NUM];			//断流事件(C)
}MEM_AC_EVT, *pMEM_AC_EVT;

/**************************************************************************************************
                                             交采备份数据存储
***************************************************************************************************/
//(FRAM备份)交互数据 MEM_OBJ_AC_X_BACK
typedef struct _tagMEM_AC_BACK_DATA
{
	INT32U				nSyTime;			//失压总累计时间
	INT32U				nAxsyTime;			//A相失压累计时间
	INT32U				nBxsyTime;			//B相失压累计时间
	INT32U				nCxsyTime;			//C相失压累计时间
	INT32U				nAxdxTime;			//A相断相累计时间
	INT32U				nBxdxTime;			//B相断相累计时间
	INT32U				nCxdxTime;			//C相断相累计时间
	INT32U				nQsyTime;			//全失压累计时间
	INT32U				nDynxTime;			//电压逆相累计时间
	INT32U				nAxslTime;			//A相失流累计时间
	INT32U				nBxslTime;			//B相失流累计时间
	INT32U				nCxslTime;			//C相失流累计时间
	AC_EVENT_STAT		sEventStat;			//交采事件状态
    
    
    INT32U				nGyTime;			//过压总累计时间
    INT32U				nAxgyTime;			//A相过压累计时间
	INT32U				nBxgyTime;			//B相过压累计时间
	INT32U				nCxgyTime;			//C相过压累计时间
    INT32U				nGlTime;			//过流总累计时间
    INT32U				nAxglTime;			//A相过流累计时间
	INT32U				nBxglTime;			//B相过流累计时间
	INT32U				nCxglTime;			//C相过流累计时间
    INT32U				nClTime;			//潮流反向总累计时间
    INT32U				nAxclTime;			//A相潮流反向累计时间
	INT32U				nBxclTime;			//B相潮流反向累计时间
	INT32U				nCxclTime;			//C相潮流反向累计时间
    INT32U				nGzTime;			//过载总累计时间
    INT32U				nAxgzTime;			//A相过载累计时间
	INT32U				nBxgzTime;			//B相过载累计时间
	INT32U				nCxgzTime;			//C相过载累计时间

    STD_TIME			sTermClk;           //表计时钟
    
    INT32U				nQyTime;			//欠压总累计时间
	INT32U				nAxqyTime;			//A相欠压累计时间
	INT32U				nBxqyTime;			//B相欠压累计时间
	INT32U				nCxqyTime;			//C相欠压累计时间
    
    INT32U				nDlTime;			//断流总累计时间
	INT32U				nAxDlTime;			//A相断流累计时间
	INT32U				nBxDlTime;			//B相断流累计时间
	INT32U				nCxDlTime;			//C相断流累计时间
    
    INT32U				nRTCTime;			//时钟电池总累计时间
    STD_TIME			sBcClk;             //最近一次编程时间
}MEM_AC_BACK_DATA, *pMEM_AC_BACK_DATA;


//(FLASH备份)交互数据 MEM_OBJ_AC_X_BACK_FLH
typedef struct _tagMEM_AC_BACK_FLH_DATA
{
	INT8U				nXBackBuff[14];		//备份缓存保护区14字节

	AC_DAY_MEM_INFO		sDayInfo;			//日数据信息
	AC_DAY_MEM_INFO		sRDayInfo;			//抄表日数据信息
	AC_DAY_MEM_INFO		sSsdjInfo;			//瞬时冻结数据信息
	AC_DAY_MEM_INFO		sDsdjInfo;      	//定时冻结数据信息
	AC_DAY_MEM_INFO		sSqdjInfo;      	//约定冻结数据信息
	AC_DAY_MEM_INFO		sSddjInfo;      	//约定冻结数据信息
    AC_DAY_MEM_INFO		sMONInfo;			//月数据信息
    
	AC_EVT_MEM_INFO		sEvtAxsyInfo;		//A相失压事件信息
	AC_EVT_MEM_INFO		sEvtBxsyInfo;		//B相失压事件信息
	AC_EVT_MEM_INFO		sEvtCxsyInfo;		//C相失压事件信息
	AC_EVT_MEM_INFO		sEvtAxdxInfo;		//A相断相事件信息
	AC_EVT_MEM_INFO		sEvtBxdxInfo;		//B相断相事件信息
	AC_EVT_MEM_INFO		sEvtCxdxInfo;		//C相断相事件信息
	AC_EVT_MEM_INFO		sEvtQsyInfo; 		//全失压事件信息
	AC_EVT_MEM_INFO		sEvtDynxInfo;		//电压逆相事件信息
	AC_EVT_MEM_INFO		sEvtAxslInfo;		//A相失流次数信息
	AC_EVT_MEM_INFO		sEvtBxslInfo;		//B相失流次数信息
	AC_EVT_MEM_INFO		sEvtCxslInfo;		//C相失流次数信息
	AC_EVT_MEM_INFO		sEvtDDInfo;     	//掉电事件信息
	AC_EVT_MEM_INFO		sEvtXlqlInfo;		//需量清零事件信息
	AC_EVT_MEM_INFO		sEvtDbqlInfo;		//电表清零事件信息
	AC_EVT_MEM_INFO		sEvtSjqlInfo;		//事件清零事件信息
	AC_EVT_MEM_INFO		sEvtBcInfo;     	//编程事件信息
	AC_EVT_MEM_INFO		sEvtJsInfo;     	//校时事件信息	

	INT32U     			nSyNUM;				//总失压次数
	STD_TIME			sSyFssk;			//最近1次失压发生时刻(字节Week无效)
	STD_TIME			sSyJssk;			//最近1次失压结束时刻(字节Week无效)
	INT32U				nAxsyNUM;			//A相失压次数
	INT32U				nBxsyNUM;			//B相失压次数
	INT32U				nCxsyNUM;			//C相失压次数
	INT32U     			nDxNUM;				//总断相次数
	INT32U				nAxdxNUM;			//A相断相次数
	INT32U				nBxdxNUM;			//B相断相次数
	INT32U				nCxdxNUM;			//C相断相次数
	INT32U				nQsyNUM;			//全失压总次数
	INT32U				nDynxNum;			//电压逆相次数
	INT32U     			nSlNUM;				//总失流次数
	INT32U				nAxslNUM;			//A相失流次数
	INT32U				nBxslNUM;			//B相失流次数
	INT32U				nCxslNUM;			//C相失流次数
	INT32U				nDdNum;				//掉电次数
	INT32U				nXlqlNum;			//需量清零次数
	INT32U				nDbqlNum;			//电表清零次数
	INT32U				nSjqlNum;			//事件清零次数
	INT32U				nBcNum;				//编程次数
	INT32U				nJsNum;				//校时次数
	INT8U				nBroadJsFlag;		//0x55为广播校时发生
    MEM_AC_REAL_DATA    sRealData;		    //实时数据
    
    INT32U     			nGyNUM;				//总过压次数
	STD_TIME			sGyFssk;			//最近1次过压发生时刻(字节Week无效)
	STD_TIME			sGyJssk;			//最近1次过压结束时刻(字节Week无效)
	INT32U				nAxgyNUM;			//A相过压次数
	INT32U				nBxgyNUM;			//B相过压次数
	INT32U				nCxgyNUM;			//C相过压次数
    AC_EVT_MEM_INFO		sEvtAxgyInfo;		//A相过压事件信息
	AC_EVT_MEM_INFO		sEvtBxgyInfo;		//B相过压事件信息
	AC_EVT_MEM_INFO		sEvtCxgyInfo;		//C相过压事件信息
    
    INT32U     			nGlNUM;				//总过流次数
	STD_TIME			sGlFssk;			//最近1次过流发生时刻(字节Week无效)
	STD_TIME			sGlJssk;			//最近1次过流结束时刻(字节Week无效)
	INT32U				nAxglNUM;			//A相过流次数
	INT32U				nBxglNUM;			//B相过流次数
	INT32U				nCxglNUM;			//C相过流次数
    AC_EVT_MEM_INFO		sEvtAxglInfo;		//A相过流事件信息
	AC_EVT_MEM_INFO		sEvtBxglInfo;		//B相过流事件信息
	AC_EVT_MEM_INFO		sEvtCxglInfo;		//C相过流事件信息
    
    INT32U     			nClNUM;				//总潮流反向次数
	STD_TIME			sClFssk;			//最近1次潮流反向发生时刻(字节Week无效)
	STD_TIME			sClJssk;			//最近1次潮流反向结束时刻(字节Week无效)
	INT32U				nAxclNUM;			//A相潮流反向次数
	INT32U				nBxclNUM;			//B相潮流反向次数
	INT32U				nCxclNUM;			//C相潮流反向次数
    AC_EVT_MEM_INFO		sEvtAxclInfo;		//A相潮流反向事件信息
	AC_EVT_MEM_INFO		sEvtBxclInfo;		//B相潮流反向事件信息
	AC_EVT_MEM_INFO		sEvtCxclInfo;		//C相潮流反向事件信息
    
    INT32U     			nGzNUM;				//总过载次数
	STD_TIME			sGzFssk;			//最近1次过载发生时刻(字节Week无效)
	STD_TIME			sGzJssk;			//最近1次过载结束时刻(字节Week无效)
	INT32U				nAxgzNUM;			//A相过载次数
	INT32U				nBxgzNUM;			//B相过载次数
	INT32U				nCxgzNUM;			//C相过载次数
    AC_EVT_MEM_INFO		sEvtAxgzInfo;		//A相过载事件信息
	AC_EVT_MEM_INFO		sEvtBxgzInfo;		//B相过载事件信息
	AC_EVT_MEM_INFO		sEvtCxgzInfo;		//C相过载事件信息
    
    INT8U				nACEvtTrip;		//跳闸状态
    
    INT32U				nOpenCoverNum;			//开盖次数
    AC_EVT_MEM_INFO     sOpenCoverInfo;        //开盖信息 
    
    INT32U              nACEvtTripNum;          //跳闸次数
    AC_EVT_MEM_INFO     sACEvtTripInfo;        //跳闸信息
    
    INT32U              nEvtSwitchOnNum;          //跳闸次数
    AC_EVT_MEM_INFO     sEvtSwitchOnInfo;        //跳闸信息
    
    INT32U              nEvtSecretKeyNum;          //密钥次数
    AC_EVT_MEM_INFO     sEvtSecretKeyInfo;        //密钥信息
    
    INT32U              nEvtLoadSwitchNum;          //负荷开关次数
    AC_EVT_MEM_INFO     sEvtLoadSwitchInfo;        //负荷开关信息
    
    INT32U              nEvtPowerAbnlNum;          //电源异常次数
    AC_EVT_MEM_INFO     sEvtPowerAbnlInfo;        //电源异常信息 
    
    INT32U              nEvtClockTrbNum;          //时钟异常次数
    AC_EVT_MEM_INFO     sEvtClockTrbInfo;        
    
    INT32U              nEvtChipTrbNum;          //芯片异常次数
    AC_EVT_MEM_INFO     sEvtChipTrbInfo;     
    
    INT32U     			nQyNUM;				//总欠压次数
	STD_TIME			sQyFssk;			//最近1次欠压发生时刻(字节Week无效)
	STD_TIME			sQyJssk;			//最近1次欠压结束时刻(字节Week无效)
	INT32U				nAxqyNUM;			//A相欠压次数
	INT32U				nBxqyNUM;			//B相欠压次数
	INT32U				nCxqyNUM;			//C相欠压次数
    
    INT32U     			nDlNUM;				//总断流次数
	INT32U				nAxdlNUM;			//A相断流次数
	INT32U				nBxdlNUM;			//B相断流次数
	INT32U				nCxdlNUM;			//C相断流次数
    INT8U				nACOffset;		//交采偏差
    SET_MEM_INFO        sSetParametersInfo; 
    

}MEM_AC_BACK_FLH_DATA, *pMEM_AC_BACK_FLH_DATA;


#endif
