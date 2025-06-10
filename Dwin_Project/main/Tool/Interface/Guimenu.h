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
#ifndef GUIMENU_H
#define GUIMENU_H

#include "Os_cpu.h"

/**************************************************************************************************
											 相关宏定义及结构枚举定义
***************************************************************************************************/
#define SUPER_PSW			"58715883"					// 超级密码

#define M_PSW_RET			0xEC						// 管理员
#define S_PSW_RET			0xED						// 超级密码

#define  nUn                              220.0


/**************************************************************************************************
                                             变量声明
***************************************************************************************************/
extern const INT8S 	g_nUartArray[5];
extern INT8S		g_nSynBaseUartMode;         		// 通讯口,红外

extern const INT32S g_nUartBpsArray[13];
extern INT8S		g_nSynBaseUartBPS;          		// 波特率,1200

extern const INT8S 	g_nParityArray[3];
extern INT8S		g_nSynBaseParity;           		// 奇偶校验,E

extern const INT8S 	g_nStopBitArray[];
extern INT8S		g_nSynBaseStopBit;          		// 停止位,1

extern const INT8S 	g_nCharBitArray[];
extern INT8S 		g_nSynBaseCharBit;          		// 数据位,8

extern const INT8S 	g_nCharDispFrm[];
extern INT8S  		g_nShowHex;		        		// 报文显示

extern const INT8S  g_nCtIRSelct[];               		//穿透时选择红外
extern INT8S   		g_nCtIr;

extern const INT8S  g_nLedBackArray[];
extern INT8S 		g_LedBackCloseTime; 				//背光自动关闭

extern const INT8S  g_nTermCloseArray[];
extern INT8S  		g_TermCloseTime; 					//终端自动关机

extern INT8S   		g_nBeepFlg;						//蜂鸣器
extern INT8S 		g_nContinueKeyFlg;


extern INT8S 		g_nTimeOut[10];					//超时时间


extern INT8U  Data0[35];

// 当前显示页面枚举
typedef enum page{
    PAGE_HOME,
    PAGE_SETTINGS,
    PAGE_MONITOR,
    PAGE_SCANNER,
    PAGE_STORAGE,
    PAGE_DATETIME
} Page_Nsp;

/**************************************************************************************************
                                             函数声明
***************************************************************************************************/
void updateLCDDisplay();
void updateHomePage();


















void	BaseCommInfoCfg(void* pData);
void 	SetCommInfo(void* pData);
void 	OtherParConfig(void);
void 	TermSelfAddrConfig(void);

/*终端管理与维护*/
void RestartRdMeter(void);
void StopRdMeter(void);
void ReturnRdMeter(void);
void ActivateConnect(void);
void Disconnect(void);
void NodeMaintain(void);
void USBFunction(void);
void HardWareReset(void);
void DataReset(void);
void ParaReset(void);


/*遥信状态 ，事件记录*/
void RemoteCommState(void);
void EventLog(void);

/*终端信息*/
void ConcerInfo(void);

/*通讯参数的设置和读取*/
void CommunicationMode(void);
void MessageCenter(void);
void MainIpAddr(void);
void EthernetPara(void);

void CommunicationModeSet(void);
void MessageCenterSet(void);
void MainIpAddrSet(void);
void EthernetParaSet(void);

/*电表参数的设置*/
void MeterParaSet(void);
void MeterParamQuery(void);

/*终端时间*/
void ConCerTimequery(void);
void ConCerTimeSet(void);

void CommSign(void);

/*界面密码*/
INT32S   InterfacePassword(void);
void    InterfacePasswordSet(void);
void    PswCheck(void);

/*终端地址*/
void ConcerNumQuery(void);
void ConcerNumSet(void);

/*测量点数据*/
void RealTimeData( void );
void DayData( void );
void MonData( void );

void    VersionInfo( void );
void    TimeDisp(void);
void    StratCheck(void);
void    GprsTest(void);
void    CtrlProcTest(void);
BOOLEAN BcdToAscii(INT8U *pData, INT8U nDataLen, BOOLEAN bDescFlg, INT8U *pDest);
void LfCr(INT8U * ch);
BOOLEAN TltSimRegExTmp(INT8U* pBuf, INT16U nLen);
void DisSec(void);
void  LcdDisplayTime(void);

void meterPowerOff(void);
INT32S  AutoDisPlayProc(INT8U* nFlgCnt ,INT8U nLenFlg);

void  AcInfo(void);
void AcCalcEventProc(void);
void DataDeal(INT8U*  nBuf,INT32U    nDI,INT8U* Data);
void Time1Deal(INT8U*  nBuf1,INT32U    nDI,INT8U* Data);
void Time2Deal(INT8U*  nBuf1,INT32U    nDI,INT8U* Data);
void DemandTime(INT8U*  nBuf1,INT32U    nDI,INT8U* Data);
void TriphaseEventDisp(void);
void SinglephaseEventDisp(void);

void  DivideTwo(INT8U* pData);
void  DivideTwo2(INT8U* pData);
void  ScreenDisplay(INT8U* nStepCnt);
void  sAcCombEnergyData(INT8U*  nBuf, INT8U* Data);
void  AcCalcEventState(void);
void  AcCalcEventState2(void);

void  LcdDisplayPage1(void);   
void  LcdDisplayPage2(void); 
void  LcdDisplayPage3(void);
void  LcdDisplayPage4(void); 
void  LcdDisplayPage5(void);
void  LcdDisplayPage6(void);
void  LcdDisplayPage7(void);   
void  LcdDisplayPage8(void);  
void  LcdDisplayPage9(void); 
void  LcdDisplayPage10(void); 
void  LcdDisplayPage11(void); 
void  LcdDisplayPage12(void); 
void  LcdDisplayPage13(void); 
void  LcdDisplayPage14(void);
void  LcdDisplayPage15(void); 
void  LcdDisplayPage16(void); 
void  LcdDisplayPage17(void); 
void  LcdDisplayPage18(void); 
void  LcdDisplayPage19(void); 
void  LcdDisplayPage20(void);
void  LcdDisplayPage21(void);
void  LcdDisplayPage22(void); 
void  LcdDisplayPage23(void); 
void  LcdDisplayPage24(void);
void  LcdDisplayPage25(void);
void  LcdDisplayPage26(void);
void  LcdDisplayPage27(void); 
void  LcdDisplayPage28(void);
void  LcdDisplayPage29(void);
void  LcdDisplayPage30(void);
void  LcdDisplayPage31(void); 
void  LcdDisplayPage32(void);
void  LcdDisplayPage33(void);
void  LcdDisplayPage34(void);
void  LcdDisplayPage35(void);
void  LcdDisplayPage36(void);
void  LcdDisplayPage37(void);
void  LcdDisplayPage38(void);
void  LcdDisplayPage39(void);
void  LcdDisplayPage40(void);
void  LcdDisplayPage41(void); 
void  LcdDisplayPage42(void);
void  LcdDisplayPage43(void);
void  LcdDisplayPage44(void);
void  LcdDisplayPage45(void);
void  LcdDisplayPage46(void);
void  LcdDisplayPage47(void);
void  LcdDisplayPage48(void);
void  LcdDisplayPage49(void);
void  LcdDisplayPage50(void);
void  LcdDisplayPage51(void); 
void  LcdDisplayPage52(void);
void  LcdDisplayPage53(void);
void  LcdDisplayPage54(void);
void  LcdDisplayPage55(void);
void  LcdDisplayPage56(void);
void  LcdDisplayPage57(void);
void  LcdDisplayPage58(void);
void  LcdDisplayPage59(void);
void  LcdDisplayPage60(void);
void  LcdDisplayPage61(void);
void  LcdDisplayPage62(void);
void  LcdDisplayPage63(void);
void  LcdDisplayPage64(void); 
void  LcdDisplayPage65(void); 
void  LcdDisplayPage66(void);
void  LcdDisplayPage67(void);
void  LcdDisplayPage68(void);
void  LcdDisplayPage69(void);
void  LcdDisplayPage70(void);
void  LcdDisplayPage71(void);
void  LcdDisplayPage72(void);
void  LcdDisplayPage73(void);
void  LcdDisplayPage74(void); 
void  LcdDisplayPage75(void);
void  LcdDisplayPage76(void);
void  LcdDisplayPage77(void);
void  LcdDisplayPage78(void);
void  LcdDisplayPage79(void);
void  LcdDisplayPage80(void);
void  LcdDisplayPage81(void);
void  LcdDisplayPage82(void);
void  LcdDisplayPage83(void);
void  LcdDisplayPage84(void);
void  LcdDisplayPage85(void);
void  LcdDisplayPage86(void);
void  LcdDisplayPage87(void);  
void  LcdDisplayPage88(void); 
void  LcdDisplayPage89(void); 
void  LcdDisplayPage90(void);
void  LcdDisplayPage91(void); 
void  LcdDisplayPage92(void);
void  LcdDisplayPage93(void); 
void  LcdDisplayPage94(void);
void  LcdDisplayPage95(void);
void  LcdDisplayPage96(void);
void  LcdDisplayPage97(void);
void  LcdDisplayPage98(void);
void  LcdDisplayPage99(void);
void  LcdDisplayPage100(void);
void  LcdDisplayPage101(void);
void  LcdDisplayPage102(void);
void  LcdDisplayPage103(void);
void  LcdDisplayPage104(void);
void  LcdDisplayPage105(void);
void  LcdDisplayPage106(void);
void  LcdDisplayPage107(void);
void  LcdDisplayPage108(void);
void  LcdDisplayPage109(void);
void  LcdDisplayPage110(void);
void  LcdDisplayPage111(void);
void  LcdDisplayPage112(void);
void  LcdDisplayPage113(void);

void CheckMeterFault();
void CheckMistake();
void DisplyWrong2(INT8U nStepCnt);
void DisplyWrong(INT8U nStepCnt);

void  LcdDisplayPage_1(void);   
void  LcdDisplayPage_2(void); 
void  LcdDisplayPage_3(void);
void  LcdDisplayPage_4(void); 
void  LcdDisplayPage_5(void);
void  LcdDisplayPage_6(void);
void  LcdDisplayPage_7(void);   
void  LcdDisplayPage_8(void);  
void  LcdDisplayPage_9(void); 
void  LcdDisplayPage_10(void); 
void  LcdDisplayPage_11(void); 
void  LcdDisplayPage_12(void); 
void  LcdDisplayPage_13(void); 
void  LcdDisplayPage_14(void);
void  LcdDisplayPage_15(void); 
void  LcdDisplayPage_16(void); 
void  LcdDisplayPage_17(void); 
void  LcdDisplayPage_18(void); 
void  LcdDisplayPage_19(void); 
void  LcdDisplayPage_20(void);
void  LcdDisplayPage_21(void);
void  LcdDisplayPage_22(void); 
void  LcdDisplayPage_23(void);
void  LcdDisplayPage_24(void);



#endif
