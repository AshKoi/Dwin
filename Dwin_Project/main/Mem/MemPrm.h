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
#ifndef _MEM_PRM_H_
#define _MEM_PRM_H_

#include "Os_cpu.h"
#include "Macro.h"
#include "Table.h"
#include "RX8025T.h"


/**************************************************************************************************
                                        1.终端通讯参数存储
***************************************************************************************************/
//终端通讯参数存储结构 MEM_TERM_COMM_PRM
typedef struct _tagMEM_TERM_COMM_PRM
{
    INT8U		nAreaCode[LEN_AREA_CODE];				//(F94)地区代码
    INT8U       nTermAddr[LEN_TERM_ADDR];				//(F94)终端地址
    
    INT8U		nAreaCodeTmp[LEN_AREA_CODE];			//(F95)地区代码
    INT8U       nTermAddrTmp[LEN_TERM_ADDR];			//(F95)终端地址
    
    INT8U       nCarrZbSelect;                          //(F85)载波专变选择0x55专变、0xAA集中器
    
    INT8U		nRadioDlyMs;						    //(F1)终端数传延时(单位：MS)
    INT8U    	nTransDlyS;							    //(F1)终端作为启动站允许发送返回的传输延时时间(单位：S)
    INT8U 		nRespTOS;							    //(F1)终端等待从动站响应的超时时间(单位：S)
    INT8U		nRptTimes;							    //(F1)重发次数
    INT8U    	nReplyFlag;							    //(F1)需要主站确认的通信服务的标志(D0：1类数据自动上报,D1：2类数据自动上报)
    INT8U  	    nBeatCycS;							    //(F1)心跳周期(单位：S)

    INT8U       nMainIP[LEN_IP_ADDR];					//(F3)主用IP地址
    INT8U       nMainIPPort[2];						    //(F3)主用IP端口
    INT8U       nBackIp[LEN_IP_ADDR];					//(F3)备用IP地址
    INT8U       nBackIpPort[2];             			//(F3)备用IP端口
    INT8U       nAPN[LEN_APN];                       	//(F3)APN

    INT8U       nGsmCdma[8];                       	    //(F4)GSM/CDMA
    INT8U       nTelePhonNum[8];                 	    //(F4)电话号码

    INT8U       nTerminalIp[LEN_IP_ADDR];          	    //(F7)终端IP地址
    INT8U       nTerminalMask[4];                  	    //(F7)终端子网掩码
    INT8U       nTerminalGate[4];                		//(F7)终端网关
    INT8U       nTerminalAgent;                  		//(F7)代理类型
    INT8U       nTerminalAddr[4];                 		//(F7)代理服务器地址
    INT8U       nTerminalPorts[2];                 	    //(F7)代理服务器端口
    INT8U       nProxyLinkModl;                         //(F7)代理连接方式
    INT8U       nProxyUserLen;                   		//(F7)代理用户名长度
    INT8U       nUserName[20];                     	    //(F7)代理服务器用户名
    INT8U       nPassWordLen;                      	    //(F7)代理服务器密码长度
    INT8U       nPassWord[20];                       	//(F7)代理服务器密码
	INT8U       nTerminalPort[2];                   	//(F7)终端监听端口

	INT8U       nWorkMode;                        		//(F8)工作模澹(混合模式、客户机模式、服务器模式)
    INT8U       nOnlineMode[2];                     	//(F8)永久在线、时段在线模式重拨间隔(单位:秒)
    INT8U       nActivMode;                         	//(F8)被动激活模式重拨次数(单位:次)
    INT8U       nBreakoutMode;                      	//(F8)被动激活模式连续无通信自动断线时(单位:min)
    INT8U       nAllowOnMode[3];                     	//(F8)时段在线模式允许在线时段标种("1"表示允许在线时段，置"0"表示禁止在线时段)

    INT8U       nVirtuNetUser[32];                 	    //(F16)虚拟专网用户名
    INT8U       nVirtuNetPassw[32];                	    //(F16)虚拟专网密码

	INT8U       nFlowCommun[4];                     	//(F36)月通信流量门限

	INT8U		nChkNum;								//校验
}MEM_TERM_COMM_PRM, *pMEM_TERM_COMM_PRM;


/**************************************************************************************************
                                       2.电表参数存储
***************************************************************************************************/
//电表参数存储结构 MEM_METER_PRM
typedef struct _tagMEM_METER_EXT_PRM
{
    MEM_METER_PRM		sMeterPrm;						//(F10)
    //MEM_PULSE_PRM       sPulsePrm;   
    MEM_MT_BASE_PRM     sMtBasePrm; 
}MEM_METER_EXT_PRM, *pMEM_METER_EXT_PRM;


/**************************************************************************************************
                                       3.总加组参数存储
***************************************************************************************************/
//总加组参数存储
typedef struct _tagMEM_GRP_PRM
{
    MEM_PULSE_PRM       sPulsePrm[MAX_PULSE_NUM];       //（F11）脉冲参数 
    
    INT8U       nStats;                                 //（F12）状态量接入标志位
    INT8U       nAttr;                                  //（F12）状态量接入属性位
  
    INT8U       nGrpSeq;								//(F14)总加组序号
    INT8U       nGrpMpNum;								//(F14)总加组测量点数量
    INT8U       nGrpMpAttr[MAX_GRP_MP_NUM];			    //(F14)总加组测量点号及总价标志
    
	INT8U		nWattTurnBS;							//(F45)轮次
	
	INT8U		nWattCtrlSlip;							//(F43)滑差时间
	
    INT8U       nPeriodPrjBS;							//(F41)时段控方案标志
    WATT_CTRL_PERIOD_PRM	sWattCtrlPeriodPrm[3];	    //(F41)时段定值

	INT8U		nFactoryDef[2];						    //(F42)厂休控定值
	FORMAT19	sFactoryTime;							//限电时间
	INT8U		nFactoryCntTimes;						//持续时间
	INT8U		nFactoryDateBS;						    //限电标志日

	FORMAT20	sBusinessTimeS;						    //(F44)营业报停控起始时间
	FORMAT20	sBusinessTimeE;						    //营业报停控结束时间
	INT8U		nBusinessDef[2];						//定值

	INT8U		nEnergyTurnBS;							//(F48)轮次
	
	INT8U       nEnergyDef[4];                    		//(F46)月电能量控定值
	INT8U       nEnergyAlmCoff[4];                      //(F46)报警系数
	
    INT8U       nBuyEnergyNo[4];						//(F47)购电单号
    INT8U		nBuyRfresh;							    //(F47)刷新标志
	INT8U		nBuyEnergy[4];							//(F47)购电量
	INT8U		nBuyEnergyAlm[4];						//报警
	INT8U		nBuyEnergySwitch;						//跳闸

}MEM_GRP_PRM, *pMEM_GRP_PRM;

/**************************************************************************************************
                                       4.抄表运行参数存储
***************************************************************************************************/
//抄表运行参数存储
typedef struct _tagMEM_RDMETER_PRM
{
    MEM_COM_PRM     sComPrm[MAX_COM_NUM];
}MEM_RDMETER_PRM, *pMEM_RDMETER_PRM;

/**************************************************************************************************
                                       5.6.大小类号参数存储
***************************************************************************************************/
//大小类号参数存储
typedef struct _tagMEM_METER_TYPE_PRM
{
    METER_TYPE_INFO_PRM		sMeterTypeInfoPrm[MAX_METER_MINTYPE_NUM];		//(F38)信息点参数
}MEM_METER_TYPE_PRM, *pMEM_METER_TYPE_PRM;

/**************************************************************************************************
                                       7.8.任务参数存储
***************************************************************************************************/
//任务参数
typedef struct _tagMEM_TASK_PRM
{
	INT8U		nTaskValid;							//F67
	
	INT8U		nCycleUnit;							//(F65)定时上报周期及单位
    FORMAT01    sReportBaseTime;						//上报基准
    INT8U		nRate;									//曲线倍率
    INT8U		nDADTNum;								//数据单元数目
    INT8U		nDADT[255];							//数据单元
}MEM_TASK_PRM, *pMEM_TASK_PRM;


/**************************************************************************************************
                                       9.控制参数存储
***************************************************************************************************/
//控制类参数
typedef struct _tagMEM_CTRL_PRM
{
	CTRL_TURN_PRM   sCtrlTurnPrm[MAX_TURN_NUM];        //(F1)遥控跳闸

	CTRL_GRP_PRM	sCtrlGrpPrm[MAX_GRP_NUM];			//(F9~F16)控制投入

	KEEP_POWER_PRM  sKeepPowerPrm;                      //F25
    
	INT8U		nPressValid;							//(F26/F34)催肥投入/解除
	INT8U		nCommValid;							    //(F27/F35)终端与主站通话投入/解除
	INT8U		nRejectValid;							//(F28/F36)剔除投入/解除
	INT8U		nReportValid;							//(F29/F37)主动上报投入/解除
	INT8U		nSelfFindMeterValid;					//(F149)自动维护功能
    
	
}MEM_CTRL_PRM, *pMEM_CTRL_PRM;


/**************************************************************************************************
                                       10.其他参数存储
***************************************************************************************************/
//其他参数存储结构 MEM_OTHER_PRM
typedef struct _tagMEM_OTHER_PRM
{
	INT8U		nSlaveCommPort; 						//(F37)级联通信端口
	INT8U		nSlaveCommCtrl; 						//(F37)级联通信控制字
	INT8U		nSlaveFrmTOS;							//(F37)报文超时时间
	INT8U		nSlaveByteTOS;							//(F37)字节超时时间
	INT8U		nSlaveRpt;								//(F37)失败重发次数
	INT8U		nSlaveItvl; 							//(F37)巡测周期
	INT8U		nSlaveNumBS;							//(F37)级联标志及个数
	INT8U		nSlave[3][4];							//(F37)级联地址

	
	

	INT8U		nSafeNo;								//(F5)消息认证号
	INT8U		nSafePrm;								//(F5)消息认参数
	INT8U		nTermGrpAddr[8][2];					    //(F6)组地址
	INT8U		nErcRecordBS[8];						//(F9)记录标志
	INT8U		nErcGradeBS[8];						//(F9)等级标志
	INT8U		nStateVarFlgs;							//(F12)状态量接入标志位（对应1～8路状态量"1"：接入，置"0"：未接入)
	INT8U		nStateVarproper;						//(F12)状态量属性标志位（对应1～8路状态量"1"：接入，置"0"：未接入)
	INT8U		nTermDispMeterAddr[12];				//(F29)终端当地显示电能表号
	INT8U		nStopRdMeter;							//(F30)台区集中抄表停抄/投抄
	INT8U		nMeterValidBS[256];					//(F150)测量点有效标志
	INT8U		nMeterChangeBS[256];					//(F150)测量点参数变更标志	
	INT8U		nAnlogBS;								//(F61)直流模拟量接入
	INT8U		nAnlogValS[2];							//(F81)直流模拟量量程起始
	INT8U		nAnlogValE[2];							//(F81)直流模拟量量程结束
	INT8U		nAnlogLimitUp[2];						//(F82)直流模拟量上限
	INT8U		nAnlogLimitDown[2];					//(F82)直流模拟量下限
	INT8U		nAnlogFrez;							//(F83)直流模拟量冻结参数
	



	
	INT8U		nMeterDiff1;							//(F59)电能表超差
	INT8U		nMeterDiff2;							//(F59)电能表飞走
	INT8U		nMeterStop15Min;						//(F59)电能表停走
	INT8U		nMeterAdjustMin;						//(F59)电能表校时



	
	
	INT8U       nPressFeeAla[3];                    	//(F23)催费告警允许∕禁止标志位
	INT8U       nSoundBS[3];                        	//(F57)声音告警
	INT8U       nProtectMin;                        	//(F58)保电时间




	INT8U       nWattSafeDef[2];                     	//(F17)保安定值
	INT8U       nWattDefCoeff;                        	//(F19)时段功控定值浮动系数(单位:%)
	INT8U       nWattPeriodBS[12];                 	//(F18)每半小时以两位编码表示四种控制状态：取值0～3依次表示不控制、控制1、控制2、保留。    
	INT8U       nTurnAlmTime[MAX_TURN_NUM];           	//(F49)功控轮次告警时间



	
    INT8U       nEnergyDefCoeff;                    	//(F20)月电能量控定值浮动系数(单位:%)
	//INT8U       nEnergyPeriodBS[12];            		//(F21)每30分钟为一个时段，各时段费率号取值0～47依次表示费率1～费率48，其他值无效。
    //INT8U       nEnergyMFeeNum1;                		//(F21)费率数：取值范围1～48，其他无效。
    WATT_PARAM	sPowerPerd[MAX_FEE_PERD_NUM];			//(F21)终端电能量费率时段(APP)
	INT8U       nEnergyMFeeNum2;                 		//(F22)费率数M(单位:个)
    INT8U       nEnergyFeeVal[MAX_FEE_NUM][3];       	//(F22)费率(单位:厘/元)
    



    //PULSE_PRM	sPulsePrm[MAX_PULSE_NUM];				//(F11)脉冲参数
    ANLOG_PRM	sAnlogPrm[MAX_ANLOG_NUM];				//(F13)电压/电流模拟量
	DIFF_PRM	sDiffPrm[MAX_DIFF_NUM];				//(F15)差动组
	DOWN_MODL_PRM	sDownModlPrm[PORT_NUM];			//(F34)下行端口参数配置


	
	
	INT8U       nImMeterNum;							//(F35)重点电表数目
    INT8U       nImSeq[MAX_IM_METER_NUM][2];			//(F35)重点电表序号
	INT8U       nRelayTermNum;							//(F2)中继终端数目
    INT8U       nRelayTermAddr[MAX_RELAY_TERM_NUM][2];//(F2)中继终端
    INT8U       nColNodeNum;							//(F31)从节点附属节点数目
    INT8U       nColNodeAddr[MAX_RELAY_TERM_NUM][6];	//(F31)从节点附属节点
    
}MEM_OTHER_PRM, *pMEM_OTHER_PRM;

/**************************************************************************************************
                                       11.校表参数
***************************************************************************************************/
typedef struct _tagMEM_AC_CAL_PRM
{
	INT32U	nModeCfg;			//模式相关控制
	INT32U	nPGActrl;			//ADC增益配置
	INT32U	nEMUCfg;			//EMU单元配置
	INT32U	nPgain[3];			//有功功率增益
	INT32U	nQgain[3];			//无功功率增益
	INT32U	nSgain[3];			//视在功率增益
	INT32U	nPhSregpq0[3];  	//相位校正0
	INT32U	nPhSregpq1[3];		//相位校正1
	INT32U	nPoffset[3];		//有功功率offset校正
	INT32U	nQPhscal;			//无功相位校正
	INT32U	nUgain[3];			//电压增益
	INT32U	nIgain[3];			//电流增益
	INT32U	nIstarup;			//起动电流阀值设置
	INT32U	nHfconst;			//高频脉冲输出设置
	INT32U	nFailVoltage;		//失压阀值设置
	INT32U	nGainADC7;			//第七路ADC输入信号增益
	INT32U	nQoffset[3];		//无功功率offset校正
	INT32U	nURmsoffse[3];		//电压有效值offset校正
	INT32U	nIRmsoffse[3];		//电流有效值offset校正
	INT32U	nUoffset[3];		//电压通道ADC offset校正
	INT32U	nIoffset[3];		//电流通道ADC offset校正
	INT32U	nEMUIE;				//中断使能
	INT32U	nModuleCFG;			//电路模块配置寄存器
	INT32U	nAllGain;			//全通道增益，用于Vref的温度校正
	INT32U	nHFDouble;			//脉冲常数加倍选择
	INT32U	nLineGain;			//基波增益校正
	INT32U	nPineCtrl;			//数字Pin上下拉电阻选择控制
	INT32U	nPstart;			//起动功率设置寄存器
	INT32U	nIregion;			//相位补偿区域设置寄存器
	INT32U	nReserved[2];		//reserved

	INT32U	nCheckSumEntire;	// 所有校表寄存器的累加和
	INT32U	nCheckSumReset;		// 默认校表寄存器值
	INT32U	nChkCode;			// Checksum code
    
    //INT8U   nChkSum;            
}MEM_AC_CAL_PRM, *pMEM_AC_CAL_PRM;

/**************************************************************************************************
                                       12.交采参数
***************************************************************************************************/
typedef struct _tagMEM_AC_PRM
{
	INT8U		nDemandPeriod; 
    INT8U		nSlipTime;
	INT8U		nDispTime1;                                           
	INT8U		nDispNum1;                                        
	INT8U		nDispNum2;                                           
	INT8U		nDispTime2;                                    
	INT8U		nDispPoint1;                                    
	INT8U		nDispPoint2;                                       
	INT8U		nCurveDay;                                         
	FORMAT11	sPBaseEnergy;                                          
	FORMAT11	sQBaseEnergy;                                        
                                                                      
	FORMAT15	sYearChangeTime;                                     
	FORMAT15	sDayChangeTime;                                      
	INT8U		nYearSegmNum;                                       
	INT8U		nDayTableNum;                                          
	INT8U		nDaySegmNum;                                         
	INT8U		nFeeNumber;                                           
	INT16U		nHolidayNumber;                                        
	INT8U		nHarmcNumber;                                         
	INT8U       nMeterAddr[LEN_METER_ADDR];                           
	INT8U		nMeterNumber[LEN_METER_ADDR];                        
	INT8U		nMeterProNum[LEN_METER_GLBH];                         
	INT8U		nEpCompFlag;                                         
	INT8U		nEq1CompFlag;                                         
	INT8U		nEq2CompFlag;                                         
	INT8U		nIR1Baud;                                          
	INT8U		nIR2Baud;                                            
	INT8U		nCom1Baud;                                           
	INT8U		nCom2Baud;                                            
	INT8U		nCom3Baud;                                            
	INT8U		nHolidayFlag;                                        
	INT8U		nHolidayTable;                                        
	FORMAT24	sMonthJS1;                                           
	FORMAT24	sMonthJS2;                                            
	FORMAT24	sMonthJS3;                                           
	INT8U		nMeterPWD[10][LEN_METER_PASSWORD];                  
	FORMAT23	sPosPowLimt;                   
	FORMAT23	sNegPowLimt;                                         
	FORMAT07	sUpLimtVolt;                                         
	FORMAT07	sLowLimtVolt;                                         
	MM_DD_NN	sYearSegmTable[2][MAX_YEAR_SEGM_NUM];                 
	hh_mm_NN	sDaySegmTable[2][MAX_DAY_TABLE_NUM][MAX_DAY_SEGM_NUM];
	YY_MM_DD_NN	sHolidayTable[MAX_HOLIDAY_NUM];
	FORMAT17	sPeriodicityDJ;
    //INT32U      nObjId; 
    INT32U		nHardType;
    INT16U      nCTRate;
    INT16U      nPTRate; 
    FP32        pTransR[3];
    FP32        pTransX[3]; 
    FP32        pTransG[3]; 
    FP32        pTransB[3]; 
    INT8U		nDispModel;          //轮显模式：0：江苏  1：湖南
    //FORMAT17    sPeriodicityMinDJ;
    //参数添加不可超过1024，因为数据库中限定了1024的最大字节数
  //  INT8U       nMeter698Addr[LEN_METER_ADDR];
   // INT8U       nMeter698ProNum[LEN_METER_ADDR];
    INT8U		        nChkNum;								//校验
}MEM_AC_PRM, *pMEM_AC_PRM;

/**************************************************************************************************
                                       12.外扩参数存储
***************************************************************************************************/
//外扩类参数
typedef struct _tagMEM_EXT_PRM
{
	INT8U		nTmp;							//(F1)

	
}MEM_EXT_PRM, *pMEM_EXT_PRM;


#endif
