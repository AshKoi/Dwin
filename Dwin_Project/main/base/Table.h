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
#ifndef _TABLE_H_
#define _TABLE_H_
#include "Os_cpu.h"
#include "config.h"
#include "AppCfg.h"
#include "Macro.h"
#include "RX8025T.h"

/**************************************************************************************************
                                          基本数据结构定义I
***************************************************************************************************/
//FORMAT01
typedef struct _tagFORMAT01
{
    INT8U        nSec;                                   //秒
    INT8U        nMin;								    //分
    INT8U        nHour;									//时
    INT8U        nDay;									//日
    INT8U        nMon;									//星期-月
    INT8U        nYear;									//年
}FORMAT01, *pFORMAT01;

//FORMAT02
typedef struct _tagFORMAT02
{
    INT8U       nVal[2];        //个位,十位,百位，符号位
}FORMAT02, *pFORMAT02;

//FORMAT03
typedef struct _tagFORMAT03
{
    INT8U       nVal[4];        //个位-百万位，符号位
}FORMAT03, *pFORMAT03;

//FORMAT04
typedef struct _tagFORMAT04
{
    INT8U       nVal;           //最高位-上浮
}FORMAT04, *pFORMAT04;

//FORMAT05
typedef struct _tagFORMAT05
{
    INT8U       nFrac;           //个位和十分位
    INT8U       nSint;           //十位和百位,最大值为799.9  s表示.7位，s = 0表示正值;s = 1表示负值
}FORMAT05, *pFORMAT05;

//FORMAT06
typedef struct _tagFORMAT06
{
    INT8U       nFrac;           //百分位和十分位
    INT8U       nSint;           //个位和十位,最大值为79.99   s表示.7位，s = 0表示正值;s = 1表示负值
}FORMAT06, *pFORMAT06;

//FORMAT07
typedef struct _tagFORMAT07
{
    INT8U       nFrac;           //个位和十分位
    INT8U       nIntg;           //十位和百位
}FORMAT07, *pFORMAT07;

//FORMAT08
typedef struct _tagFORMAT08
{
    INT8U       nD1;             //个位和十位
    INT8U       nD2;             //百位和千位
}FORMAT08, *pFORMAT08;

//FORMAT09
typedef struct _tagFORMAT09
{
    INT8U       nFrac0;          //万分位、千分位
    INT8U       nFrac1;          //百分位、十分位
    INT8U       nSint;           //个位、十位（s）
}FORMAT09, *pFORMAT09;

//FORMAT10
typedef struct _tagFORMAT10
{
    INT8U       nD1;              //个位、十位
    INT8U       nD2;              //百位、千位
    INT8U       nD3;              //万位、十万位
}FORMAT10, *pFORMAT10;

//FORMAT11
typedef struct _tagFORMAT11
{
    INT8U        nD1;             //百分位、十分位
    INT8U        nD2;             //个位、十位
    INT8U        nD3;             //百位、千位
    INT8U        nD4;             //万位、十万位
}FORMAT11, *pFORMAT11;

//FORMAT12
typedef struct _tagFORMAT12
{
    INT8U        nD1;             //个位、十位
    INT8U        nD2;             //百位、千位
    INT8U        nD3;             //万位、十万位
    INT8U        nD4;             //百万位、千万位
    INT8U        nD5;             //亿位、十亿位
    INT8U        nD6;             //百亿位、千亿位
}FORMAT12, *pFORMAT12;

//FORMAT13
typedef struct _tagFORMAT13
{
    INT8U        nD1;             //万分位、千分位
    INT8U        nD2;             //百分位、十分位
    INT8U        nD3;             //个位、十位
    INT8U        nD4;             //百位、千位
}FORMAT13, *pFORMAT13;

//FORMAT14
typedef struct _tagFORMAT14
{
    INT8U        nD1;             //万分位、千分位
    INT8U        nD2;             //百分位、十分位
    INT8U        nD3;             //个位、十位
    INT8U        nD4;             //百位、千位
    INT8U        nD5;             //万位、十万位
}FORMAT14, *pFORMAT14;

//FORMAT15
typedef struct _tagFORMAT15
{
    INT8U        nMin;			  //分
    INT8U        nHour;			  //时
    INT8U        nDay;			  //日
    INT8U        nMon;			  //月
    INT8U        nYear;			  //年
}FORMAT15, *pFORMAT15;

//FORMAT17
typedef struct _tagFORMAT17
{
    INT8U        nMin;								    //分
    INT8U        nHour;									//时
    INT8U        nDay;									//日
    INT8U        nMon;									//月
}FORMAT17, *pFORMAT17;

//FORMAT18
typedef struct _tagFORMAT18
{
    INT8U       nMin;           //分
    INT8U       nHour;          //时
    INT8U       nDay;           //日
}FORMAT18, *pFORMAT18;

//FORMAT19
typedef struct _tagFORMAT19
{
    INT8U       nMin;           //分
    INT8U       nHour;          //时
}FORMAT19, *pFORMAT19;

//FORMAT20
typedef struct _tagFORMAT20
{
    INT8U       nDay;           //日
    INT8U       nMon;           //月
    INT8U       nYear;          //年
}FORMAT20, *pFORMAT20;

//FORMAT21
typedef struct _tagFORMAT21
{
    INT8U       nMon;           //月
    INT8U       nYear;          //年
}FORMAT21, *pFORMAT21;
//FORMAT22
typedef struct _tagFORMAT22
{
    INT8U       nD1;            //个位和十分位
}FORMAT22, *pFORMAT22;

//FORMAT23
typedef struct _tagFORMAT23
{
    INT8U       nFrac0;         //万分位、千分位
    INT8U       nFrac1;         //百分位、十分位
    INT8U       nIntg;          //个位、十位
}FORMAT23, *pFORMAT23;

//FORMAT24
typedef struct _tagFORMAT24
{
    INT8U       nHour;          //时
    INT8U       nDay;           //日
}FORMAT24, *pFORMAT24;

//FORMAT25
typedef struct _tagFORMAT25
{
    INT8U       nD1;            //千分位、百分位
    INT8U       nD2;            //十分位、个位
    INT8U       nD3;            //十位、百位S
}FORMAT25, *pFORMAT25;

//FORMAT26
typedef struct _tagFORMAT26
{
    INT8U       nD1;            //千分位、百分位
    INT8U       nD2;            //十分位、个位
}FORMAT26, *pFORMAT26;

//FORMAT27
typedef struct _tagFORMAT27
{
    INT8U       nD1;            //个位、十位
    INT8U       nD2;            //百位、千位
    INT8U       nD3;            //万位、十万位
    INT8U       nD4;            //百万位、千万位
}FORMAT27, *pFORMAT27;

//FORMAT40(自扩格式)
typedef struct _tagFORMAT40
{
    INT8U       nDay;           //日
    INT8U       nMon;           //月
}FORMAT40, *pFORMAT40;

//FORMAT41
typedef struct _tagFORMAT41
{
    INT8U        nD1;           //百分位、十分位
    INT8U        nD2;           //个位、十位
    INT8U        nD3;           //百位、千位
}FORMAT41, *pFORMAT41;

//FORMAT42
typedef struct _tagFORMAT42
{
    INT8U        nD1;           //百分位、十分位
    INT8U        nD2;           //个位、十位
}FORMAT42, *pFORMAT42;

//FORMAT43
typedef struct _tagFORMAT43
{
    INT8U        nD1;           //百分位、十分位
    INT8U        nD2;           //个位、十位
    INT8U        nD3;           //百位、符号位
}FORMAT43, *pFORMAT43;

//FORMAT44
typedef struct _tagFORMAT44
{
    INT8U        nD1;           //百分位、千分位
    INT8U        nD2;           //个位、十分位
    INT8U        nD3;           //十位、符号位
}FORMAT44, *pFORMAT44;

//FORMAT45
typedef struct _tagFORMAT45
{
    INT8U       nD1;            //万分位、十万分位
    INT8U       nD2;            //百分位、千分位
    INT8U       nD3;            //个位.十分位
}FORMAT45, *pFORMAT45;

//FORMAT46
typedef struct _tagFORMAT46
{
    INT8U       nD1;            //十分位、个位
    INT8U       nD2;            //十位、百位
    INT8U       nD3;            //千位、万位
    INT8U       nD4;            //十万位、符号位
}FORMAT46, *pFORMAT46;

//FORMAT47
typedef struct _tagFORMAT47
{
    INT8U        nD1;           //个位、十位
    INT8U        nD2;           //百位、千位
    INT8U        nD3;           //万位、十万位
    INT8U        nD4;           //百万位、千万位
    INT8U        nD5;           //亿位、十亿位
}FORMAT47, *pFORMAT47;

//FORMAT48
typedef struct _tagFORMAT48
{
    INT8U       nD1;            //十分位、个位
    INT8U       nD2;            //十位、百位
    INT8U       nD3;            //千位、万位
    INT8U       nD4;            //十万位、百万位
    INT8U       nD5;            //千万位,符号位
}FORMAT48, *pFORMAT48;

//FORMAT49
typedef struct _tagFORMAT49
{
    INT8U        nSec;			//秒
    INT8U        nMin;			//分
    INT8U        nHour;			//时
    INT8U        nDay;			//日
    INT8U        nMon;			//月
    INT8U        nYear;			//年
}FORMAT49, *pFORMAT49;

//FORMAT50
typedef struct _tagFORMAT50
{
    INT8U        nD1;           //百分位、十分位
    INT8U        nD2;           //个位、十位
    INT8U        nS3;           //符号
}FORMAT50, *pFORMAT50;

//FORMAT51
typedef struct _tagFORMAT51
{
    INT8U        nD1;           //十位、个位
    INT8U        nD2;           //百位、千位(s)
}FORMAT51, *pFORMAT51;

//FORMAT52
typedef struct _tagFORMAT52
{
    INT8U       nD1;            //百分位、十分位
    INT8U       nD2;            //个位、十位
    INT8U       nD3;            //百位、千位
    INT8U       nD4;            //万位、十万位
    INT8U       nS5;            //符号位
}FORMAT52, *pFORMAT52;

//MM_DD_NN
typedef struct _tagMM_DD_NN
{
    INT8U	nNo;
    INT8U       nDay;
    INT8U       nMon;
}MM_DD_NN,*pMM_DD_NN;

//hh_mm_NN
typedef struct _taghh_mm_NN
{
    INT8U	nNo;
    INT8U       nMin;
    INT8U       nHour;
}hh_mm_NN,*phh_mm_NN;

//YY_MM_DD_NN
typedef struct _tagYY_MM_DD_NN
{
    INT8U	nNo;
    INT8U       nDay;
    INT8U       nMon;
    INT8U	nYear;
}YY_MM_DD_NN, *pYY_MM_DD_NN;

//HH_MM_SS
typedef struct _tagHH_MM_SS
{
    INT8U        nSec;			//秒
    INT8U        nMin;			//分
    INT8U        nHour;			//时
}HH_MM_SS, *pHH_MM_SS;

//YY_MM_DD_WW
typedef struct _tagYY_MM_DD_WW
{
    INT8U	    nWeek;
    INT8U       nDay;
    INT8U       nMon;
    INT8U	    nYear;
}YY_MM_DD_WW, *pYY_MM_DD_WW;


//F25
typedef struct _tagF25DATA
{
	INT8U					nTime[5];
	FORMAT09				sPpower[4];
	FORMAT09				sNpower[4];
	FORMAT05				sFact[4];
	FORMAT07				sVolt[3];
	FORMAT25				sCurr[4];
	FORMAT09				sPower[4];
}F25DATA, *pF25DATA;

//交采事件状态
typedef	struct _tagAC_EVENT_STAT
{
	INT8U			nLVolFlag[MAX_PHASE_NUM];					//失压状态
    INT8U			nOVolFlag[MAX_PHASE_NUM];					//过压状态
	INT8U			nLPhaseFlag[MAX_PHASE_NUM];					//断相状态
	INT8U			nLCurrFlag[MAX_PHASE_NUM];					//失流状态
    INT8U			nOCurrFlag[MAX_PHASE_NUM];					//过流状态
    INT8U			nOloadFlag[MAX_PHASE_NUM];					//过载状态
    INT8U			nReflowFlag[MAX_PHASE_NUM];					//潮流状态
	INT8U			nNVolFlag;									//全失压状态
	INT8U			nPFailFlag;									//掉电状态
	INT8U			nVoltPErrFlag;								//电压逆相序状态
	INT8U			nSQNum;										//当前时区表
	INT8U			nSDNum;										//当前时段表
    INT8U			nUVolFlag[MAX_PHASE_NUM];					//欠压状态
    INT8U			nFCurrFlag[MAX_PHASE_NUM];					//断流状态
}AC_EVENT_STAT, *pAC_EVENT_STAT;

//交采清零标志
typedef	struct _tagAC_FRE_STAT
{
	INT8U			nFlag;										//发生标志
	INT8U			nMasterNo[4];								//操作代码
}AC_FRE_STAT, *pAC_FRE_STAT;

//交采清零标志
typedef	struct _tagAC_EVECLR_STAT
{
	INT8U			nFlag;										//发生标志
	INT8U			nMasterNo[4];								//操作代码
	INT8U			nDataID[4];									//数据标识码
}AC_EVECLR_STAT, *pAC_EVECLR_STAT;

typedef struct _tagMEM_AC_EVT_DATA
{
	INT8U	nData[LEN_AC_EVENT];
}MEM_AC_EVT_DATA, *pMEM_AC_EVT_DATA;


//交采电能量脉冲缓冲结构
typedef	struct _tagENERGY_DATA
{
	INT16S			nEnergy[MAX_FEE_NUM+1];			//总电能 各费率电能
}ENERGY_DATA, *pENERGY_DATA;

//交采分相分象限电能量脉冲缓冲结构
typedef	struct _tagPH_ENERGY_DATA
{
	INT16U			nEnergy[4];						//正向有功电能 反向有功电能 正向无功电能 反向无功电能/四象限电能
}PH_ENERGY_DATA, *pPH_ENERGY_DATA;

/**************************************************************************************************
                                   		   AC业务信息结构
***************************************************************************************************/
//日数据信息
typedef struct _tagAC_DAY_MEM_INFO
{
    INT8U		nNext;
    INT16U		nMemNum;
    //FORMAT20	sTime[MAX_AC_DAY_DATA_NUM];
}AC_DAY_MEM_INFO, *pAC_DAY_MEM_INFO;

//事件存储信息
typedef struct _tagAC_EVT_MEM_INFO
{
	INT8U		nNext;
	INT8U		nMemNum;
}AC_EVT_MEM_INFO, *pAC_EVT_MEM_INFO;

//设置参数
typedef struct _tag_SET_MEM_INFO
{
    INT16U      nBJDL;      //报警电流
    INT16U      nBJWD;      //报警温度
    INT16U      nBJWS;      //报警温升
    INT16U      nQDDL;      //切断电流
    INT16U      nQDWD;      //切断温度
    INT16U      nQDWS;      //切断温升
    INT16U      nJCKG;      //检测开关
}SET_MEM_INFO,*pSET_MEM_INFO;
//多功能表电能示值
typedef struct _tagMT_DGN_ENERGY_DATA
{
    FORMAT11            sPEnergy[1+MAX_FEE_NUM];        //组合有功总电能
    FORMAT11            sPPEnergy[1+MAX_FEE_NUM];       //正向有功电能示值
    FORMAT11            sNPEnergy[1+MAX_FEE_NUM];       //反向有功电能示值
    FORMAT11            sPNEnergy[1+MAX_FEE_NUM];       //正向无功电能示值
    FORMAT11            sNNEnergy[1+MAX_FEE_NUM];       //反向无功电能示值
    FORMAT11            sPAEnergy[1+MAX_FEE_NUM];       //正向视在总电能
    FORMAT11            sNAEnergy[1+MAX_FEE_NUM];       //反向视在总电能
}MT_DGN_ENERGY_DATA, *pMT_DGN_ENERGY_DATA;

//多功能表电能示值
typedef struct _tagMT_DGN_C_ENERGY_DATA
{
    FORMAT11            sPPEnergy[1+MAX_FEE_NUM];       //组合正向有功电能示值
}MT_DGN_C_ENERGY_DATA, *pMT_DGN_C_ENERGY_DATA;

//多功能表电能示值
typedef struct _tagMT_DGN_Q_ENERGY_DATA
{
    FORMAT11            sQ1Energy[1+MAX_FEE_NUM];       //一象限无功电能示值
    FORMAT11            sQ2Energy[1+MAX_FEE_NUM];       //二象限无功电能示值
    FORMAT11            sQ3Energy[1+MAX_FEE_NUM];       //三象限无功电能示值
    FORMAT11            sQ4Energy[1+MAX_FEE_NUM];       //四象限无功电能示值
}MT_DGN_Q_ENERGY_DATA, *pMT_DGN_Q_ENERGY_DATA;

//多功能表三相电能示值
typedef struct _tagMT_DGN_TRI_ENERGY_DATA
{
    FORMAT11            sPPEnergy[MAX_PHASE_NUM];       //正向有功电能示值
    FORMAT11            sNPEnergy[MAX_PHASE_NUM];       //反向有功电能示值
    FORMAT11            sPNEnergy[MAX_PHASE_NUM];       //正向无功电能示值
    FORMAT11            sNNEnergy[MAX_PHASE_NUM];       //反向无功电能示值
}MT_DGN_TRI_ENERGY_DATA, *pMT_DGN_TRI_ENERGY_DATA;

//多功能表三相电能示值
typedef struct _tagMT_DGN_TRI_Q_EN_DATA
{
    FORMAT11            sQ1Energy[MAX_PHASE_NUM];       //一象限无功电能示值
    FORMAT11            sQ2Energy[MAX_PHASE_NUM];       //二象限无功电能示值
    FORMAT11            sQ3Energy[MAX_PHASE_NUM];       //三象限无功电能示值
    FORMAT11            sQ4Energy[MAX_PHASE_NUM];       //四象限无功电能示值
}MT_DGN_TRI_Q_EN_DATA, *pMT_DGN_TRI_Q_EN_DATA;

//变压器损耗电能示值
typedef struct _tagLOSER_ENERGY_DATA
{
    FORMAT11            sCUEnergy;       //铜损有功总电能示值
    FORMAT11            sFEEnergy;       //铁损有功总电能示值
}LOSER_ENERGY_DATA, *pLOSER_ENERGY_DATA;

//交采变压器损耗电能量缓冲结构
typedef	struct _tagLOSER_ACCU_DATA
{
	FP32	nEnergy[2];						//总电能
}LOSER_ACCU_DATA, *pLOSER_ACCU_DATA;

//多功能表需量示值
typedef struct _tagMT_DGN_DEMAND_DATA
{
    FORMAT23    		sPPDemand[1+MAX_FEE_NUM];       //正向有功最大需量
    FORMAT15    		sPPDemandTime[1+MAX_FEE_NUM];   //正向有功最大需量发生时间
    FORMAT23    		sNPDemand[1+MAX_FEE_NUM];       //反向有功最大需量
    FORMAT15    		sNPDemandTime[1+MAX_FEE_NUM];   //反向有功最大需量发生时间
    FORMAT23    		sPNDemand[1+MAX_FEE_NUM];       //正向无功最大需量
    FORMAT15    		sPNDemandTime[1+MAX_FEE_NUM];   //正向无功最大需量发生时间
    FORMAT23    		sNNDemand[1+MAX_FEE_NUM];       //反向无功最大需量
    FORMAT15    		sNNDemandTime[1+MAX_FEE_NUM];   //反向无功最大需量发生时间
}MT_DGN_DEMAND_DATA, *pMT_DGN_DEMAND_DATA;

//多功能表四象限需量
typedef struct _tagMT_DGN_Q_DE_DATA
{
    FORMAT23    		sQ1Demand[1+MAX_FEE_NUM];       //正向有功最大需量
    FORMAT15    		sQ1DemandTime[1+MAX_FEE_NUM];   //正向有功最大需量发生时间
    FORMAT23    		sQ2Demand[1+MAX_FEE_NUM];       //反向有功最大需量
    FORMAT15    		sQ2DemandTime[1+MAX_FEE_NUM];   //反向有功最大需量发生时间
    FORMAT23    		sQ3Demand[1+MAX_FEE_NUM];       //正向无功最大需量
    FORMAT15    		sQ3DemandTime[1+MAX_FEE_NUM];   //正向无功最大需量发生时间
    FORMAT23    		sQ4Demand[1+MAX_FEE_NUM];       //反向无功最大需量
    FORMAT15    		sQ4DemandTime[1+MAX_FEE_NUM];   //反向无功最大需量发生时间
}MT_DGN_Q_DE_DATA, *pMT_DGN_Q_DE_DATA;

//交采组合电能脉冲缓冲结构
typedef	struct _tagCOMB_ENERGY_DATA
{
    INT16S          nCombEpEnergy[MAX_FEE_NUM+1];
}COMB_ENERGY_DATA, *pCOMB_ENERGY_DATA;


/**************************************************************************************************
                                          基本数据结构定义II
***************************************************************************************************/
//IP结构
typedef struct _tagTCPIP_IP_PORT
{
    INT8U       nIp[4];             				//四个字节的二进制码的IP地址
    INT8U       nPort[2];                      //
}TCPIP_IP_PORT, *pTCPIP_IP_PORT;

// 通信口配置
typedef struct _tagCOM_CFG_SUB
{
	INT8U					nCom;
	INT32U 				    nCommByte;
}COM_CFG_SUB, *pCOM_CFG_SUB;

// 通信口配置
typedef struct _tagCOM_CFG
{
	INT8U					nCom;
	INT32U 				    nBaudRate;
	INT8U 					nParity;
	INT8U 					nStopBits;
	INT8U 					nCharLen;
}COM_CFG, *pCOM_CFG;

//终端类型通讯参数
typedef struct _tagOBJ_COMM_PRM
{
	INT8U						nObj;
	INT8U						nCom;
	INT32U						nBaud;
	INT8U						nParity;
	
}OBJ_COMM_PRM, *pOBJ_COMM_PRM;


//操作员参数
typedef struct _tagOPERTOR_PRM
{
	INT8U					nOpertor[4];
	INT8U					nOperPsw[3];
}OPERTOR_PRM, *pOPERTOR_PRM;


//参数结构
typedef struct _tag_PARAM
{
	//INT8U					nTermAddr[LEN_TERMADDR];                      // 
	INT8U					nAreaRun;
	INT8U					nCarrierCom;
	INT8U					nWorkOrder;
	INT8U					nJcMenu;
	INT8U					nDefaultServ;
	OPERTOR_PRM				sOpertor;
	INT32S					nCurrWorkidAttr;								//当前工单号属性:-1非法;0xEC管理员;数字,操作员

}PARAM, *pPARAM;


//下载：存铁电
typedef struct _tagDOWNLOAD_STR
{	
	INT32U			nDownSize;
	INT16U			nLastId;			//上一帧号
	INT16U			nLastLen;			//上一帧长度
	BOOLEAN			bSucc;				//下载成功标志
}DOWNLOAD_STR, *pDOWNLOAD_STR;


/**************************************************************************************************
                                          基本数据结构定义III
***************************************************************************************************/
//日数据信息
typedef struct _tagDAY_MEM_INFO
{
    INT8U		nNext;
    INT8U		nMemNum;
    FORMAT20	sTime[MAX_DAY_DATA_NUM];
}DAY_MEM_INFO, *pDAY_MEM_INFO;

//月数据信息
typedef struct _tagMON_MEM_INFO
{
    INT8U		nNext;
    INT8U		nMemNum;
    FORMAT21	sTime[MAX_MON_DATA_NUM];
}MON_MEM_INFO, *pMON_MEM_INFO;

//曲线数据信息
typedef struct _tagCRV_MEM_INFO
{
    INT8U		nNext;
    INT8U		nMemNum;
    FORMAT20	sTime[MAX_CRV_DATA_NUM];
}CRV_MEM_INFO, *pCRV_MEM_INFO;

//抄表日数据信息
typedef struct _tagRDAY_MEM_INFO
{
    INT8U		nNext;
    INT8U		nMemNum;
    FORMAT20	sTime[MAX_RDAY_DATA_NUM];
}RDAY_MEM_INFO, *pRDAY_MEM_INFO;

//事件数据信息
typedef struct _tagEVT_MEM_INFO
{
    INT8U		nNext;
    INT8U		nMemNum;
}EVT_MEM_INFO, *pEVT_MEM_INFO;

typedef struct _tagRC_EVT_DATA
{
    INT8U		nTurn;
    STD_TIME    sTime;
}RC_EVT_DATA, *pRC_EVT_DATA;

typedef struct _tagEVT_TRIP_DATA
{
    INT8U		nEvtTrip;
    FORMAT49    sTime;
}EVT_TRIP_DATA, *pEVT_TRIP_DATA;

typedef struct _tagEVT_GUARPOWER_DATA
{
    INT8U		nGuarPower;
    FORMAT49    sTime;
}EVT_GUARPOWER_DATA, *pEVT_GUARPOWER_DATA;

typedef struct _tagEVT_ALARM_DATA
{
    INT8U		nAlarm;
    FORMAT49    sTime;
}EVT_ALARM_DATA, *pEVT_ALARM_DATA;

typedef  struct  _tag698FZDBFN
{
    INT16U   nDI;
    INT16U   nFn;
}MT_698FZDBFN,*pMT_698FZDBFN;


//标准事件上报状态
typedef struct _tagMT_REPORTPASS_DATA
{
     //FORMAT46           sOAD;
     INT8U              nSTate;    
}MT_REPORTPASS_DATA, *pMT_REPORTPASS_DATA;

//标准事件记录表
typedef struct _tagMT_REPORT_DATA
{
    INT8U               sOrderNum; //   INT8U    
    FORMAT01            sStartTimestamp;       
    FORMAT01            sFinishTimestamp;       
    //INT8U               sReason;      
    MT_REPORTPASS_DATA  sChannelState;       
    FORMAT11            sPAData[30];
    FORMAT11            sPAFinshData[30]; 
}MT_REPORT_DATA, *pMT_REPORT_DATA;

typedef struct _tagMT_OAD_DATA
{
    FORMAT27            sOAD[64];
}MT_OAD_DATA, *pMT_OAD_DATA;

/**************************************************************************************************
                                          基本数据结构定义IV
***************************************************************************************************/
//电表参数存储结构 MEM_METER_PRM
typedef struct _tagMEM_METER_PRM
{
    BOOLEAN     bVld;									//有效标志
	INT8U		nSeq[2];								//(F10)测量点序号
	INT8U		nMp[2]; 								//(F10)测量点号
	INT8U		nCom;									//(F10)测量点端口
	INT8U		nBaud[4];								//(F10)测量点波特率
	INT8U		nProt;									//(F10)测量点通讯规约
	INT8U		nAddr[LEN_METER_ADDR];					//(F10)测量点通讯地址
	INT8U		nPwd[LEN_METER_PWD];					//(F10)测量点密码
	INT8U		nFeeNum;								//(F10)测量点费率个数
	INT8U		nXsws;									//(F10)测量点小数位数
	INT8U		nZsws;									//(F10)测量点整数位数
	INT8U		nColAddr[LEN_METER_ADDR];				//(F10)测量点采集终端地址
	INT8U		nMainType;								//(F10)测量点大类
	INT8U		nPartType;								//(F10)测量点小类

}MEM_METER_PRM, *pMEM_METER_PRM;

//F25基本参数 MEM_MT_BASE_PRM
typedef struct _tagMEM_MT_BASE_PRM
{
    INT8U				nPt[2];							//(F25)电压倍率
	INT8U				nCt[2];							//(F25)电流倍率
	INT8U				nStdVolt[2];					//(F25)额定电压
	INT8U				nStdCurr;						//(F25)额定电流
	INT8U				nStdWatt[3];					//(F25)额定功率
	INT8U				nPhaseMod;						//(F25)接线方式

}MEM_MT_BASE_PRM, *pMEM_MT_BASE_PRM;

//脉冲参数存储
typedef struct _tagMEM_PULSE_PRM
{
    BOOLEAN     bVld;								//有效标志
    INT8U       nPulsePort;							//(F11)端口号
    INT8U       nPulseMp;						    //(F11)所属测量点号
    INT8U       nPulseAttr;							//(F11)脉冲属性
    INT8U       nPulseConst[2];						//(F11)脉冲常数
}MEM_PULSE_PRM, *pMEM_PULSE_PRM;

typedef struct _tagPULSE_RATE_NUM
{
	INT64U nPulseNo[MAX_PULSE_NUM];
}PULSE_RATE_NUM, *pPULSE_RATE_NUM;

//模拟量参数存储
typedef struct _tagANLOG_PRM
{
    INT8U       nAnlogPort;							//(F13)端口号
    INT8U       nAnlogMp;								//(F13)所属测量点号
    INT8U       nAnlogAttr;							//(F13)属性
}ANLOG_PRM, *pANLOG_PRM;

//差动参数存储
typedef struct _tagDIFF_PRM
{
    INT8U       nDiffSeq;								//(F15)端口号
    INT8U       nCmpGrpSeq;							//(F15)对比总加组号
    INT8U       nBaseGrpSeq;							//(F15)参照总加组号
    INT8U		nDiffAttr;								//参与差动的电能量的时间区间及对比方法标志
	INT8U		nDiffVal;								//相对偏差值
	INT8U		nDiffAbsVal[4];						//绝对值
}DIFF_PRM, *pDIFF_PRM;


//终端端口参数存储结构 MEM_OBJ_PRM_COM
typedef struct _tagMEM_COM_PRM
{
    INT8U       nCom;									//端口号
    INT16U		bsRunCtrl;								//(F33)抄表运行控制字
    INT8U		bsReadDay[4];							//(F33)抄表日期
    FORMAT19	sReadTime;								//(F33)抄表时间
    INT16U      nReadIntrvl;							//(F33)抄表间隔
    FORMAT18	sBoardTime;								//(F33)广播校时定时时间
    INT8U       nReadPerdNum;							//(F33)抄表时段数
    FORMAT19	sReadPerd[MAX_READ_PERD_NUM][2];		//(F33)抄表时段
    INT8U       nCtrlWord;								//(F34)通信控制字
    INT32U      nBaud;									//(F34)通信速率
    INT16U      nReReadIntrvl;                          //补抄间隔
    BOOLEAN     bComStop;								//(控制 F49/F50)命令指定通信端口暂停/恢复
}MEM_COM_PRM, *pMEM_COM_PRM;

//下行模块通讯参数存储
typedef struct _tagDOWN_MODL_PRM
{
    INT8U       nCommPort;								//(F34)通信端口号
    INT8U       nCommCtrl;								//(F34)通信控制字
    INT8U       nCommBps[4];							//(F34)通信波特率
}DOWN_MODL_PRM, *pDOWN_MODL_PRM;


//大小类号参数的信息点参数存储
typedef struct _tagMETER_TYPE_INFO_PRM
{
    INT8U       nInfoNum;								//(F38)信息点数目
    INT8U       nInfoAttr[31];							//(F38)信息单元标志
}METER_TYPE_INFO_PRM, *pMETER_TYPE_INFO_PRM;


//时段控方案参数
typedef struct _tagWATT_CTRL_PERIOD_PRM
{
    INT8U       nPeriodBS;								//(F41)时段标志
    INT8U       nPeriodDef[8][2];						//(F41)时段定值
}WATT_CTRL_PERIOD_PRM, *pWATT_CTRL_PERIOD_PRM;

//控制类总加组参数
typedef struct _tagCTRL_GRP_PRM
{
	INT8U		nPeriodBS;								//(F9/F17)时段控投入标志 
	INT8U		nPeriodPrj;							//时段控方案

	INT8U		nFactory;								//(F10/F18)厂休控投入标志
	INT8U		nBussiness;							//(F11/F19)营业报停控投入标志

	INT8U		nDownCtrl;								//(F12/F20)
	FORMAT01	sDownCtrlCmdSetTime;					//(F12)下浮空
	INT8U		nDownSlipTime;							//滑差时间
	INT8U		nDownCoff;								//下浮细数
	INT8U		nDownDelayMin;							//冻结延时时间
	INT8U		nDownCtrlTimes30Min;					//持续时间
	INT8U		nDownAlmMin[4];						//轮次告警时间

	INT8U		nMonCtrl;								//(F15/F23)月电控投入/解除
	INT8U		nBuyCtrl;								//(F16/F24)购电控投入/解除
}CTRL_GRP_PRM, *pCTRL_GRP_PRM;

//控制类轮次参数
typedef struct _tagCTRL_TURN_PRM
{
	FORMAT01	sCtrlCmdSetTime;						//(F1)遥控跳闸
    INT8U       nAlmMin;								//告警延时时间
    INT8U       nAstPower30Min;						    //遥控跳闸 限电时间 
}CTRL_TURN_PRM, *pCTRL_TURN_PRM;

//保电投入解除参数
typedef struct _tagKEEP_POWER_PRM
{
	INT8U		nKeepPowerFlg;							//(F25/F33)保电投入/解除
    STD_TIME    sKeepStartTime;                         //开始保电时间
    INT8U		nKeepPowerTime;							//(F25/F33)保电时间 
}KEEP_POWER_PRM, *pKEEP_POWER_PRM;


// 功率定值和时段设置
typedef struct _tagWATT_PARAM
{
    INT8U       nStrHour;                            	//起始BCD 码小时  ----- = 0xFF : 表示无效时段
    INT8U       nStrMin;                            	//起始BCD 码分钟
    INT8U       nEndHour;                              	//结束BCD 码小时
    INT8U       nEndMin;                            	//结束BCD 码分钟
    INT8U       nKind;                                 	//功率时段：0 不控 1 控制 电量时段：费率1-48
}WATT_PARAM, *pWATT_PARAM;

//电表电量统计
typedef struct _tagMT_POWER_STAT
{
    FORMAT13            sPPPower[1+MAX_FEE_NUM];	    //正向有功电能量
    FORMAT13            sPNPower[1+MAX_FEE_NUM];	    //正向无功电能量
    FORMAT13	        sNPPower[1+MAX_FEE_NUM];	    //反向有功电能量
    FORMAT13	        sNNPower[1+MAX_FEE_NUM];	    //反向无功电能量
}MT_POWER_STAT, *pMT_POWER_STAT;

extern pPARAM              g_pParam;
#endif
