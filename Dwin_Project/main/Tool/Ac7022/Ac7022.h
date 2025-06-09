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
#ifndef _AC_7022_H_
#define _AC_7022_H_

#include "Os_cpu.h"
#include "Config.h"
#include "MemPrm.h"


/**************************************************************************************************
                                          相关宏定义
***************************************************************************************************/

#define	PosEp			0		//正向有功
#define	NegEp			1		//反向有功
#define	PosEq			2		//正向无功
#define	NegEq			3		//反向无功

#define AC_FLASH_WR_TIME	60//900	//AC实时数据备份时间间隔900S
#define AC_FRAM_WR_TIME		1//5		//AC实时数据写时间间隔180S

//#define RATED_VOLT              2200                         //额定电压
#define RATED_CURR              1500//5                         //额定电流
#define MAX_CURR                6000                         //最大电流

#define VOLT_PH_ERR_RATIO       0.6                         //电压逆向序电压比率
#define VOLT_POWER_FAIL_RATIO   0.6                         //掉电
#define LOSS_VOLT_OC_RATIO      0.78                        //启动电压相对额定电压的比率(失压)
#define LOSS_VOLT_RE_RATIO      0.85                        //返回电压相对额定电压的比率(失压)
#define LOSS_PH_OC_RATIO        0.6//0.78                         //启动电压相对额定电压的比率(断相)
#define LOSS_PH_RE_RATIO        0.85                        //返回电压相对额定电压的比率(断相)
#define LOSS_CURR_RATIO         0.7                         //临近电压相对额定电压的比率(失流)
#define OVER_VOLT_OC_RATIO      1.15                        //启动电压相对额定电压的比率(过压)
#define FAIL_CURR_RATIO         0.6                         //临近电压相对额定电压的比率(断流)



#define OC_CURR                 0.005                       //启动电流相对额定电流的比率
#define RE_CURR                 0.05                        //返回电流相对额定电流的比率
#define LOSS_PH_CURR            0.005                       //断相发生的启动电流比率
#define OV_CURR                 1.2                        //过流相对最大电流的比率


/************************************只写寄存器***************************************************/

#define w_ModeCfg		0x01	//模式相关控制
#define w_PGACtrl		0x02	//ADC增益配置
#define w_EMUCfg		0x03	//EMU单元配置
#define w_PgainA		0x04	//A相有功功率增益
#define w_PgainB		0x05	//B相有功功率增益
#define w_PgainC		0x06	//C相有功功率增益
#define w_QgainA		0x07	//A相无功功率增益
#define w_QgainB		0x08	//B相无功功率增益
#define w_QgainC		0x09	//C相无功功率增益
#define w_SgainA		0x0A	//A相视在功率增益
#define w_SgainB		0x0B	//B相视在功率增益
#define w_SgainC		0x0C	//C相视在功率增益
#define w_PhSregApq0	0x0D	//A相相位校正0
#define w_PhSregBpq0	0x0E	//B相相位校正0
#define w_PhSregCpq0	0x0F	//C相相位校正0
#define w_PhSregApq1	0x10	//A相相位校正1
#define w_PhSregBpq1	0x11	//B相相位校正1
#define w_PhSregCpq1	0x12	//C相相位校正1
#define w_PoffsetA		0x13	//A相有功功率offset校正
#define w_PoffsetB		0x14	//B相有功功率offset校正
#define w_PoffsetC		0x15	//C相有功功率offset校正
#define w_QPhscal		0x16	//无功相位校正
#define w_UgainA		0x17	//A相电压增益
#define w_UgainB		0x18	//B相电压增益
#define w_UgainC		0x19	//C相电压增益
#define w_IgainA		0x1A	//A相电流增益
#define w_IgainB		0x1B	//B相电流增益
#define w_IgainC		0x1C	//C相电流增益
#define w_Istarup		0x1D	//起动电流阈值设置
#define w_Hfconst		0x1E	//高频脉冲输出设置
#define w_FailVoltage	0x1F	//失压阈值设置（三相四线模式）
#define w_GainADC7		0x20	//第七路ADC输入信号增益
#define w_QoffsetA		0x21	//A相无功功率offset校正
#define w_QoffsetB		0x22	//B相无功功率offset校正
#define w_QoffsetC		0x23	//C相无功功率offset校正
#define w_UaRmsoffse	0x24	//A相电压有效值offset校正
#define w_UbRmsoffse	0x25	//B相电压有效值offset校正
#define w_UcRmsoffse	0x26	//C相电压有效值offset校正
#define w_IaRmsoffse	0x27	//A相电流有效值offset校正
#define w_IbRmsoffse	0x28	//B相电流有效值offset校正
#define w_IcRmsoffse	0x29	//C相电流有效值offset校正
#define w_UoffsetA		0x2A	//A相电压通道ADC offset校正
#define w_UoffsetB		0x2B	//B相电压通道ADC offset校正
#define w_UoffsetC		0x2C	//C相电压通道ADC offset校正
#define w_IoffsetA		0x2D	//A相电流通道ADC offset校正
#define w_IoffsetB		0x2E	//B相电流通道ADC offset校正
#define w_IoffsetC		0x2F	//C相电流通道ADC offset校正
#define w_EMUIE			0x30	//中断使能
#define w_ModuleCFG		0x31	//电路模块配置寄存器
#define w_AllGain		0x32	//全通道增益，用于Vref的温度校正
#define w_HFDouble		0x33	//脉冲常数加倍选择
#define w_LineGain		0x34	//基波增益校正
#define w_PinCtrl		0x35	//数字pin上下拉电阻选择控制
#define w_Pstart		0x36	//起动功率设置寄存器
#define w_Iregion		0x37	//相位补偿区域设置寄存器

/************************************只读寄存器***************************************************/
#define	r_ChkSum		0x3E	//校表参数校验寄存器

/***瞬时量***/
/***瞬时量***/
#define	r_Pa			0x01	//A相有功功率
#define	r_Pb			0x02	//B相有功功率
#define	r_Pc			0x03	//C相有功功率
#define	r_Pt			0x04	//合相有功功率
#define	r_Qa			0x05	//A相无功功率
#define	r_Qb			0x06	//B相无功功率
#define	r_Qc			0x07	//C相无功功率
#define	r_Qt			0x08	//合相无功功率
#define	r_Sa			0x09	//A相视在功率
#define	r_Sb			0x0A	//B相视在功率
#define	r_Sc			0x0B	//C相视在功率
#define	r_St			0x0C	//合相视在功率
#define	r_UaRms			0x0D	//A相电压有效值
#define	r_UbRms			0x0E	//B相电压有效值
#define	r_UcRms			0x0F	//C相电压有效值
#define	r_IaRms			0x10	//A相电流有效值
#define	r_IbRms			0x11	//B相电流有效值
#define	r_IcRms			0x12	//C相电流有效值
#define	r_ItRms			0x13	//ABC相电流矢量和的有效值
#define	r_Pfa			0x14	//A相功率因数
#define	r_Pfb			0x15	//B相功率因数
#define	r_Pfc			0x16	//C相功率因数
#define	r_Pft			0x17	//合相功率因数
#define	r_Pga			0x18	//A相电流与电压相角
#define	r_Pgb			0x19	//B相电流与电压相角
#define	r_Pgc			0x1A	//C相电流与电压相角
#define	r_INTFlag		0x1B	//中断标志，读后清零
#define	r_Freq			0x1C	//线频率

#define	r_YUaUb			0x26	//Ua与Ub的电压夹角
#define	r_YUaUc			0x27	//Ua与Uc的电压夹角
#define	r_YUbUc			0x28	//Ub与Uc的电压夹角
#define r_I0Rms			0x29	//零线电流I0通道有效值
#define	r_TempD			0x2A	//温度传感器的输出
#define	r_UtRms			0x2B	//三相电压矢量和的有效值
#define	r_Sflag			0x2C	//存放断相、相序、SIG等标志状态

#define r_Sample_IA     0x2F	//A相电流通道ADC采样数据
#define r_Sample_IB     0x30	//B相电流通道ADC采样数据
#define r_Sample_IC     0x31	//C相电流通道ADC采样数据
#define r_Sample_UA     0x32	//A相电压通道ADC采样数据
#define r_Sample_UB     0x33	//B相电压通道ADC采样数据
#define r_Sample_UC     0x34	//C相电压通道ADC采样数据

#define	r_PFlag			0x3D	//有功和无功功率方向正向为0负向为1

#define	r_Sample_I0		0x3F	//零线电流I0通道采样数据输出
#define	r_LinePa		0x40	//A相基波有功功率
#define	r_LinePb		0x41	//B相基波有功功率
#define	r_LinePc		0x42	//C相基波有功功率
#define	r_LinePt		0x43	//合相基波有功功率

#define	r_LineUaRrms	0x48	//基波A相电压有效值
#define	r_LineUbRrms	0x49	//基波B相电压有效值
#define	r_LineUcRrms	0x4A	//基波C相电压有效值
#define	r_LineIaRrms	0x4B	//基波A相电流有效值
#define	r_LineIbRrms	0x4C	//基波B相电流有效值
#define	r_LineIcRrms	0x4D	//基波C相电流有效值

/****全波电能值*****/
#define	r_EFlag			0x1D	//电能寄存器的工作状态，读后清零

#define	r_Epa			0x1E	//A相有功电能（可配置为读后清零）
#define	r_Epb			0x1F	//B相有功电能（可配置为读后清零）
#define	r_Epc			0x20	//C相有功电能（可配置为读后清零）
#define	r_Ept			0x21	//合相有功电能（可配置为读后清零）
#define	r_Eqa			0x22	//A相无功电能（可配置为读后清零）
#define	r_Eqb			0x23	//B相无功电能（可配置为读后清零）
#define	r_Eqc			0x24	//C相无功电能（可配置为读后清零）
#define	r_Eqt			0x25	//合相无功电能（可配置为读后清零）

#define r_BckReg		0x2D	//通讯数据备份寄存器
#define r_ComChksum     0x2E	//通讯校验和寄存器

#define	r_Esa			0x35	//A相视在电能（可配置为读后清零）
#define	r_Esb			0x36	//B相视在电能（可配置为读后清零）
#define	r_Esc			0x37	//C相视在电能（可配置为读后清零）
#define	r_Est			0x38	//合相视在电能（可配置为读后清零）
#define r_FstCntA		0x39	//A相快速脉冲计数
#define r_FstCntB		0x3A	//B相快速脉冲计数
#define r_FstCntC		0x3B	//C相快速脉冲计数
#define r_FstCntT		0x3C	//合相快速脉冲计数

#define r_LineEpa		0x44	//A相基波有功电能（可配置为读后清零）
#define r_LineEpb		0x45	//B相基波有功电能（可配置为读后清零）
#define r_LineEpc		0x46	//C相基波有功电能（可配置为读后清零）
#define r_LineEpt		0x47	//合相基波有功电能（可配置为读后清零）

#define r_LEFlag		0x4E	//基波电能寄存器的工作状态，读后清零
#define r_PtrWavebuff	0x7E	//冲数据指针，指示内部缓冲buffer已有数据长度
#define r_WaveBuff   	0x7F	//缓冲数据寄存器，内部自增益，重复读取直至读完缓冲数据长度


/**************************************************************************************************
                                             相关结构定义
***************************************************************************************************/
#if 0
typedef	struct
{
	FP32 fVal[CALUCATES_PER_MINUTE];					//做滑差用
	INT8U nSlips;										//计数次数
	INT8U nIsRev;										//是否翻转
}Slip_Data;
#endif

typedef	struct
{
	INT32S nVal[CALUCATES_PER_MINUTE];					//做滑差用
	INT8U nSlips;										//计数次数
	INT8U nIsRev;										//是否翻转
}Slip_Data;

typedef struct
{
	FP32 fVal[PARAM_MAX_SLIP_TIME];//做滑差用
	INT8U nSlips;					//计数次数
	INT8U nIsRev;					//是否翻转
}DemandSlip_Data1;

typedef struct
{
	FP32 fVal[PARAM_MAX_DEMAND_PERIOD];//做滑差用
	INT8U nSlips;						//计数次数
	INT8U nIsRev;						//是否翻转
}DemandSlip_Data2;

//交采瞬时数据
typedef struct _tagAC_INSTANT_DATA
{
	FORMAT07		sRealVolt[MAX_PHASE_NUM];   //二次侧电压
	FORMAT25		sRealCurr[MAX_PHASE_NUM+1]; //二次侧电流
	FORMAT09		sRealP[MAX_PHASE_NUM+1];    //二次侧有功功率
	FORMAT09		sRealQ[MAX_PHASE_NUM+1];    //二次侧无功功率
	FORMAT09		sRealS[MAX_PHASE_NUM+1];    //二次侧视在功率
	FORMAT05		sRealPf[MAX_PHASE_NUM+1];   //功率因数
	FORMAT07		sRealY[MAX_PHASE_NUM];      //电压相角
	FORMAT07		sRealPg[MAX_PHASE_NUM];     //电流相角
    FORMAT11		sRealVolt1[MAX_PHASE_NUM];  //一次侧电压
    FORMAT11		sRealCurr1[MAX_PHASE_NUM];  //一次侧电流
    FORMAT46		sRealP1[MAX_PHASE_NUM+1];   //一次侧有功功率
	FORMAT46		sRealQ1[MAX_PHASE_NUM+1];   //一次侧无功功率
	FORMAT46		sRealS1[MAX_PHASE_NUM+1];   //一次侧视在功率
	INT16U			bsRunStat[7];
}AC_INSTANT_DATA, *pAC_INSTANT_DATA;

typedef struct _tagAC_STAT_DATA
{
	FORMAT07		sStatVolt[MAX_PHASE_NUM];  //二次侧电压
	FORMAT25		sStatCurr[MAX_PHASE_NUM+1]; //二次侧电流
	FORMAT09		sStatP[MAX_PHASE_NUM+1];   //二次侧有功功率
	FORMAT09		sStatQ[MAX_PHASE_NUM+1];   //二次侧无功功率
	FORMAT09		sStatS[MAX_PHASE_NUM+1];   //二次侧视在功率
	FORMAT05		sStatPf[MAX_PHASE_NUM+1];  //功率因数
	FORMAT07		sStatY[MAX_PHASE_NUM];     //电压相角
	FORMAT07		sStatPg[MAX_PHASE_NUM];    //电流相角
    FORMAT04		sStatTp;                    //温度
}AC_AVEARGE_DATA, *pAC_AVERAGE_DATA;

typedef struct
{
	FP32 fVal[PARAM_MAX_DEMAND_PERIOD];//做滑差用
    INT16U nStep;						//步长
	INT8U nSlips;						//计数次数
	INT8U nIsRev;						//是否翻转
}DemandSlip_Data;

typedef	struct
{
	AC_INSTANT_DATA		sRealInstant;                           //实时
    AC_AVEARGE_DATA     sStatAverage;                           //分钟滑差
    INT32U				nPulseConst;							//交采脉冲常数
	INT32U	    		nHFConst;		    					//高频输出参数
    INT8U               nCombEp;                                //组合有功控制字
    INT8U               nCombEq1;                               //组合无功1控制字
    INT8U               nCombEq2;                               //组合无功2控制字
    INT32U				nCheckSumReset;							//初始校验，用于模式判断
	INT32U				nCheckSumEntire;						//校后校验，用于状态判断
	
	INT16U				nPulsePCounter;							//有功脉冲计数器
	INT16U				nPulseQCounter;							//有功脉冲计数器
	INT16U				nFramCounter;							//铁电写计数
	INT16U				nFlashCounter;							//闪存写计数
    INT32U				nACRunStat;								//交采状态字，d0内存、d1铁电校验、d2闪存校验、d3铁电读、d4闪存读

    INT16U				nPowerPulse[MAX_P_Q_NUM+4][MAX_PHASE_NUM+1];	//分方向有功无功功率脉冲
    INT16U				nPowerPulse1[MAX_P_Q_NUM+4][MAX_PHASE_NUM+1];	//分方向有功无功功率脉冲
    INT16U				nCurrHFPulse;
    INT16U				nSaveHFPulse[MAX_P_Q_NUM+4][2];
	Slip_Data			sVoltSlip[MAX_PHASE_NUM];				//三相电压
	FP32				fVolt[MAX_PHASE_NUM];
	Slip_Data			sCurrSlip[MAX_PHASE_NUM+1];				//三相电流 零序电流
	FP32				fCurr[MAX_PHASE_NUM+1];
	Slip_Data			sPSlip[MAX_PHASE_NUM+1];				//三相和总有功功率
	Slip_Data			sQSlip[MAX_PHASE_NUM+1];				//三相和总无功功率
	Slip_Data			sSSlip[MAX_PHASE_NUM+1];				//三相和总视在功率
	Slip_Data			sPfSlip[MAX_PHASE_NUM+1];				//三相和总功率因数
	Slip_Data			sTpSlip;                 //温度
    
	INT8U				nPFlag;									//有功和无功功率方向正向为0负向为1
	INT8U				nSFlag;									//相序 失压标志
	INT16U				nRealEnergy[MAX_P_Q_NUM+2][MAX_PHASE_NUM+1];
	INT8U				nRateNo;								//当前费率号
	INT8U				nSQNum;									//当前时区表
	INT8U				nSDNum;									//当前时段表
    INT8U               nAEMode;
    INT8U               nPREMode;
    INT8U               nNREMode;
	INT8U				nReadFlag;
	DemandSlip_Data 	sDemandSlipData1[MAX_P_Q_NUM+4];		//总需量滑差临时变量
	DemandSlip_Data 	sDemandSlipData2[MAX_P_Q_NUM+4];		//分费率需量滑差临时变量
    FP32				fFeAPower[MAX_PHASE_NUM+1];	//铁损有功
    FP32				fFeRPower[MAX_PHASE_NUM+1];	//铁损无功
    FP32				fCuAPower[MAX_PHASE_NUM+1];	//铜损有功
    FP32				fCuRPower[MAX_PHASE_NUM+1];	//铜损无功
}REAL_DATA, *pREAL_DATA;

//最大需量
typedef	struct
{
	INT32U			nDemand[MAX_FEE_NUM+1];						//各费率及总最大需量
	FORMAT15		sTimeData[MAX_FEE_NUM+1];					//发生时间
}AC_DEMAND_DATA;

//电表抄读数据项
typedef struct _tagPULSE_CONST
{
	INT32U	    nPulConst;	        //脉冲常数
	INT32U	    nHFConst1;		    //对应三相四线高频输出参数
	INT32U      nHFConst2;		    //对应三相三线高频输出参数
}PULSE_CONST, *pPULSE_CONST;

#if 0
typedef	struct
{
	ENERGY_DATA		sEnergyData[8];								//正向有功电能 反向有功电能	正向无功电能 反向无功电能 四象限电能
	PH_ENERGY_DATA	sPhEnergyData[MAX_PHASE_NUM];
	PH_ENERGY_DATA	sPhQEnergyData[MAX_PHASE_NUM];

	AC_DEMAND_DATA	sDemandData[8];								//最大需量
	INT16U			nENCrcChk;
	INT16U			nDECrcChk;
	INT32U			nPulseConst;								//交采脉冲常数备份
	INT32U	    	nHFConst;		    						//高频输出参数备份
}SAVE_REAL_DATA, *pSAVE_REAL_DATA;
#endif

#if 1
typedef	struct
{
    MT_DGN_ENERGY_DATA			sAcEnergy;
	MT_DGN_Q_ENERGY_DATA		sAcQEnergy;
	MT_DGN_TRI_ENERGY_DATA		sAcTriEnergy;
	MT_DGN_TRI_Q_EN_DATA		sAcTriQEnergy;
    MT_DGN_C_ENERGY_DATA        sAcCombEnergy;
    INT32U				        nENSumChk;
	ENERGY_DATA			        sEnergyData[8];								//正向有功电能 反向有功电能	正向无功电能 反向无功电能 四象限电能
	PH_ENERGY_DATA		        sPhEnergyData[MAX_PHASE_NUM];
	PH_ENERGY_DATA		        sPhQEnergyData[MAX_PHASE_NUM];
    COMB_ENERGY_DATA            sCombEPulse;
    INT32U                      nEPSumChk;
	AC_DEMAND_DATA		        sDemandData[8];								//最大需量
	INT32U				        nDESumChk;
	INT32U				        nPulseConst;								//交采脉冲常数备份
	INT32U	    		        nHFConst;		    						//高频输出参数备份
    INT8U                       nCombEp;                                    //组合有功控制字
    INT8U                       nCombEq1;                                   //组合无功1控制字
    INT8U                       nCombEq2;                                   //组合无功2控制字
}SAVE_REAL_DATA, *pSAVE_REAL_DATA;
#endif



/**************************************************************************************************
                                             变量声明
***************************************************************************************************/
extern INT32U      g_nConstant;
extern INT32U      g_nHFConst;
extern INT8U    g_nSetCtrlcurFlg;
extern pREAL_DATA pDataTable;
extern  PULSE_CONST nPulTable[];
extern pSAVE_REAL_DATA pSavDataTable;
extern INT8U nAcTmpBuf[1024];//随着结构的大小而改变，最好换一种方式定义
//extern INT8U g_nFreeChgFlg;
extern     INT8U g_SimMeterCoverFlag;
extern     INT8U g_RealMeterCoverFlag;
extern     INT8U g_SimMeterCoverTimeBuf[];
extern     INT8U g_SimMeterPasswordFlag;

extern     INT8U g_SimMeterTripFlag;//模拟跳闸标志 0xFF开 0xBF 闭
extern     INT8U g_RealMeterTripFlag;//实际跳闸标志

extern     INT8U g_SimMeterSwitchOnFlag;//模拟合闸标志 0xFF开 0xBF 闭
extern     INT8U g_RealMeterSwitchOnFlag;//实际合闸标志
extern     INT8U g_SimLoadSwitchAcFlag;//模拟负荷开关误动作标志 0xFF开 0xBF 闭
extern     INT8U g_RealLoadSwitchAcFlag;//实际负荷开关误动作标志
extern     INT8U g_SimPowerAbnlFlag;//模拟电源异常标志 0xFF开 0xBF 闭
extern     INT8U g_RealPowerAbnlFlag;//实际电源异常标志
extern     INT8U g_SimClockTrbFlag ;//模拟时钟故障标志 0xFF开 0xBF 闭
extern     INT8U g_RealClockTrbFlag;//实际时钟故障标志
extern     INT8U g_SimChipTrbFlag;//模拟计量芯片故障标志 0xFF开 0xBF 闭
extern     INT8U g_RealChipTrbFlag;//实际计量芯片故障标志
extern     INT8U g_SimOCPTrbFlag;//模拟过流故障标志 0xFF开 0xBF 闭
extern     INT8U g_RealOCPTrbFlag;//实际过流故障标志
extern     INT8U       nWuCh;                //误差%比
extern     INT8U       nZFWuCh;             //误差正负
extern     INT8U g_ACOffset;
/**************************************************************************************************
                                             函数声明
***************************************************************************************************/
void AcColTask(void *pData);

BOOLEAN     CkeckACCalPrm(void);
void        AcCalDataInit(void);
INT8S       ReadAcPluPrm(void);
INT8S       AcCalcJbParamInit(pMEM_AC_CAL_PRM pJB_Param);
INT8S       AcCalcJust7022(pMEM_AC_CAL_PRM pJB_Param);
BOOLEAN     AcCalcInit(void);
FP32        AcValueSwitch(INT32U uConstant, INT32U uHFConst, INT8U uR_ID, INT32U uData);
void        AcCalcProc(void);
void        AcCalcRead7022(void);
void        AcCalcBaseReal(void);
void        AcCalcEnergy(void);
void AcCalcEventProc(void);
void AcCalaClrProc(void);
void AcCalcClrMeterEve(const INT8U* pData);
void AcCalcDataInitProc(void);
void AcCalcClrDemandEve(const INT8U* pData);
void AcCalcDemInitProc(void);
void AcCalcClrEventEve(const INT8U* pData);
INT8S AcCalcEventInitProc(const INT8U* pData);
INT32S  AcDayDataClr(void);
INT32S  AcDayRDataClr(void);
void        AcCalcVoltPhaseErr(pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime);
void        AcCalcNoVoltage(INT8U nFlag, pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime);
void        AcCalcPowerFail(INT8U nFlag, pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime);
void        AcCalcLossOfVoltage(pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime);
void        AcCalcUnderVoltage(pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime);
void        AcCalcLossOfPhase(pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime);
void        AcCalcLossOfCurrent(pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime);
void        AcCalcOverVoltage(pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime);
void        AcCalcOverOfCurrent(pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime);
void        AcCalcOverload(pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime);
void        AcCalcReflow(pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime);
void        AcCalcCurrentFailure(pAC_EVENT_STAT pEventStat, const pSTD_TIME pTime);

void        OpenMeterCover();
void        TripEvent();
void        SwitchOnEvent();
void        SecretKeyEvent();
void        ClockTrbEvent();
void        LoadSwitchActEvent();
void        PowerAbnlEvent();
void        ChipTrbEvent();
void        OverCurrentEvent();//过流事件
void        GB698GetStartEventResp(INT8U nFn,INT8U nFn2);
void        GB698GetFinshEventResp(INT8U nFn,INT8U nFn2);

FP32        AcCalcReadValid(INT32U uConstant, INT32U uHFConst, INT8U uR_ID, INT32U uData);
INT32U      CheckINT32USum(const INT32U *pData, INT16U nStart, INT16U nLen);

INT8S       ReadAcENData(void);
INT8S       WriteAcENData(void);
INT8S   ReadAcDEData(void);
INT8S   WriteAcDEData(void);
INT8S AcCalcPerChanged(const pSTD_TIME pTime);
INT8S    AcCalcDemand(const pSTD_TIME pTime);

void ACMinCalcProc(void);
void AcCalcPerDataRdyProc(const pSTD_TIME pTime);
void AcMinFreezeDataRdyProc(const pSTD_TIME pTime);
void AcCalcDayDataRdyProc(const pSTD_TIME pTime);
void AcCalcRDayDataRdyProc(const pSTD_TIME pTime);
void AcCalcMeterPrmChanEve(const INT8U* pData, INT8U nType);
void AcCalcMonDataRdyProc(const pSTD_TIME pTime);
void CombEnergyCal(pFORMAT11 pEnergy, INT16S *pPulse, INT16U nPulseCnt);

void BackupAcData(void);
FP32 AcCalcChangePg(INT8U uPhase, FP32 fPg, FP32 fY, INT8U uPFlag);

INT8S ReadAcCombPrm(void);

INT8S       FirstWriteAcENData(void);

INT8S AcCalcReset7022(pMEM_AC_CAL_PRM pJB_Param);

void AcCalcJustData1(void);

void AcCalcJustData2(void);

void AcCalcJustData3(void);

void AcCalcJustData4(void);

void AcCalcJustData5(void);

void AcCalcJustData6(void);
#endif
