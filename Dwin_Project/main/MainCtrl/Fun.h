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
#ifndef _FUN_H_
#define _FUN_H_

#include "Os_cpu.h"
#include "Table.h"
#include "MemDrvExt.h"
#include "DataItem.h"
#include "Lcd.h"
#include "MemData.h"


/**************************************************************************************************
                                           宏
***************************************************************************************************/
#define RS4851_LOADRES					0x18							//置高加200欧电阻
#define RS4851_PULLRES					0x17							//置高加200欧电阻

#define RS4852_LOADSRES				0x4B							//置高，不加电阻	
#define RS4852_LOAD1RES				0x4A							//两根线组合选择负载电阻
#define RS4852_LOAD0RES				0x49							//

#define RS4852_PULLSRES				0x76							//置高，不加电阻
#define RS4852_PULL1RES				0x75							//两根线组合选择驱动上拉电阻
#define RS4852_PULL0RES				0x74							//

#define RS4853_LOADSRES				0x58							//置高，不加电阻	
#define RS4853_LOAD1RES				0x57							//两根线组合选择负载电阻
#define RS4853_LOAD0RES				0x56							//

#define RS4853_PULLSRES				0x50							//置高，不加电阻
#define RS4853_PULL1RES				0x51							//两根线组合选择驱动上拉电阻
#define RS4853_PULL0RES				0x52							//


#define RS485_S_VOLT					0x14							//置高，不选择查看电压
#define RS485_1_VOLT					0x13							//
#define RS485_0_VOLT					0x12							//


#define CARR_JZQ1_RST    				0x15							//集中器载波模块复位脚  低电平复位
#define CARR_JZQ2_RST    				0x12							//集中器载波模块复位脚  低电平复位
#define CARR_JZQ3_RST    				0x1D							//集中器载波模块复位脚  低电平复位

#define CARR_CJQ1_RST    				0x52
#define CARR_CJQ2_RST    				0x09
#define CARR_CJQ3_RST    				0x06


#define CARR_JZQ1_SET    				0x16							
#define CARR_JZQ2_SET    				0x13						
#define CARR_JZQ3_SET   				0x1E							

#define CARR_CJQ1_SET    				0x50
#define CARR_CJQ2_SET    				0x07
#define CARR_CJQ3_SET    				0xA5


#define CARR_JZQ1_STA    				0x14	 //CARR_JZQ1_ZD						
#define CARR_JZQ2_STA    				0x11						
#define CARR_JZQ3_STA   				0x1C							

#define CARR_CJQ1_STA    				0x51
#define CARR_CJQ2_STA    				0x08
#define CARR_CJQ3_STA    				0x05




#define CARR_JZQ_MODL_1				0x17
#define CARR_JZQ_MODL_0				0x18

#define CARR_CJQ_MODL_1				0x57
#define CARR_CJQ_MODL_0				0x56

#define CARR_PHASE_1					0xA3
#define CARR_PHASE_0					0xA4









#define	SET_IO(nIO, nVal)				{	\
	if (0 == (nVal)){						\
		GpioClearOutputEX(nIO);			\
	}										\
	else{									\
		GpioSetOutputEX(nIO);				\
	}										\
}


#if AUTO_TEST_EN == 2
//74HC165 define
#define PS_SHLD_PFR                bFM3_GPIO_PFR5_P7
#define PS_SHLD_DDR                bFM3_GPIO_DDR5_P7
#define PS_SHLD_PDOR               bFM3_GPIO_PDOR5_P7
#define PS_SHLD_PDIR               bFM3_GPIO_PDIR5_P7

#define PS_CLK_PFR                bFM3_GPIO_PFR5_P8
#define PS_CLK_DDR                bFM3_GPIO_DDR5_P8
#define PS_CLK_PDOR               bFM3_GPIO_PDOR5_P8
#define PS_CLK_PDIR               bFM3_GPIO_PDIR5_P8

#define PS_CLKINT_PFR                bFM3_GPIO_PFR5_P9
#define PS_CLKINT_DDR                bFM3_GPIO_DDR5_P9
#define PS_CLKINT_PDOR               bFM3_GPIO_PDOR5_P9
#define PS_CLKINT_PDIR               bFM3_GPIO_PDIR5_P9

#define PS_QDA_PFR                bFM3_GPIO_PFR5_PA
#define PS_QDA_DDR                bFM3_GPIO_DDR5_PA
#define PS_QDA_PDOR               bFM3_GPIO_PDOR5_PA
#define PS_QDA_PDIR               bFM3_GPIO_PDIR5_PA

#define PS_QDB_PFR                bFM3_GPIO_PFR5_PB
#define PS_QDB_DDR                bFM3_GPIO_DDR5_PB
#define PS_QDB_PDOR               bFM3_GPIO_PDOR5_PB
#define PS_QDB_PDIR               bFM3_GPIO_PDIR5_PB
//end 74HC165 define
#endif

#define TXM_TRIG_PFR                	bFM3_GPIO_PFR3_P8
#define TXM_TRIG_DDR                	bFM3_GPIO_DDR3_P8
#define TXM_TRIG_PDOR               	bFM3_GPIO_PDOR3_P8
#define TXM_TRIG_PDIR               	bFM3_GPIO_PDIR3_P8

#define TXM_WAKE_PFR                	bFM3_GPIO_PFR3_P9
#define TXM_WAKE_DDR                	bFM3_GPIO_DDR3_P9
#define TXM_WAKE_PDOR               	bFM3_GPIO_PDOR3_P9
#define TXM_WAKE_PDIR               	bFM3_GPIO_PDIR3_P9

#define TXM_PD_PFR                		bFM3_GPIO_PFR5_PB
#define TXM_PD_DDR                		bFM3_GPIO_DDR5_PB
#define TXM_PD_PDOR               		bFM3_GPIO_PDOR5_PB
#define TXM_PD_PDIR               		bFM3_GPIO_PDIR5_PB

#define TXM_CTS_PFR                	bFM3_GPIO_PFR5_PB
#define TXM_CTS_DDR                	bFM3_GPIO_DDR5_PB
#define TXM_CTS_PDOR               	bFM3_GPIO_PDOR5_PB
#define TXM_CTS_PDIR               	bFM3_GPIO_PDIR5_PB

#define TXM_RTS_PFR                	bFM3_GPIO_PFR5_PB
#define TXM_RTS_DDR                	bFM3_GPIO_DDR5_PB
#define TXM_RTS_PDOR               	bFM3_GPIO_PDOR5_PB
#define TXM_RTS_PDIR               	bFM3_GPIO_PDIR5_PB

#define BAK_SHRG_PFR                	bFM3_GPIO_PFRA_P3
#define BAK_SHRG_DDR                	bFM3_GPIO_DDRA_P3
#define BAK_SHRG_PDOR               	bFM3_GPIO_PDORA_P3
#define BAK_SHRG_PDIR               	bFM3_GPIO_PDIRA_P3

#define BAK_CHRG_PFR                	bFM3_GPIO_PFRA_P4
#define BAK_CHRG_DDR                	bFM3_GPIO_DDRA_P4
#define BAK_CHRG_PDOR               	bFM3_GPIO_PDORA_P4
#define BAK_CHRG_PDIR               	bFM3_GPIO_PDIRA_P4


#define BAK_CHG_RED					BAK_CHRG_PDIR
#define BAK_CHG_GRN					BAK_SHRG_PDIR

#define RS485_DR_PDOR               	bFM3_GPIO_PDOR4_PA

#define RS485_DR_EN_PDOR            	bFM3_GPIO_PDOR4_PA
#define RS485_DR_CH_PDOR            	bFM3_GPIO_PDOR4_PB

#define RS485_LR_EN_PDOR            	bFM3_GPIO_PDOR7_P0
#define RS485_LR_CH_PDOR            	bFM3_GPIO_PDOR7_P1

//红外通信和条型码扫描器电源开关
#define V1_IRTXM_OFF()		  			IO_GPIOWriteData(GPIO_PORT7,GPIO_PIN_A,0x0000)
#define V1_IRTXM_ON()		  			IO_GPIOWriteData(GPIO_PORT7,GPIO_PIN_A,0xFFFF)

//RS232和RS485电源开关
#define V2_TX_OFF()		  			IO_GPIOWriteData(GPIO_PORT7,GPIO_PIN_9,0x0000)
#define V2_TX_ON()		  	  			IO_GPIOWriteData(GPIO_PORT7,GPIO_PIN_9,0xFFFF)

//液晶电源开关
#define V3_LCD_OFF()		  			IO_GPIOWriteData(GPIO_PORT7,GPIO_PIN_8,0x0000)
#define V3_LCD_ON()		  			IO_GPIOWriteData(GPIO_PORT7,GPIO_PIN_8,0xFFFF)

//外部存储器电源开关
#define V4_FLASH_OFF()		  			IO_GPIOWriteData(GPIO_PORT7,GPIO_PIN_7,0x0000)
#define V4_FLASH_ON()		  			IO_GPIOWriteData(GPIO_PORT7,GPIO_PIN_7,0xFFFF)

//gprs开关
#define V5_GPRS_ON()					IO_GPIOWriteData(GPIO_PORT5,GPIO_PIN_9,0xFFFF);			\
										OSTimeDlyHMSM(0, 0, 0, 100);

#define V5_GPRS_OFF()					IO_GPIOWriteData(GPIO_PORT5,GPIO_PIN_9,0x0000);			\
										OSTimeDlyHMSM(0, 0, 0, 100);

//主电源开关
#define VCC_OFF()		  				IO_GPIOWriteData(GPIO_PORTA,GPIO_PIN_5,0x0000)
#define VCC_ON()		  				IO_GPIOWriteData(GPIO_PORTA,GPIO_PIN_5,0xFFFF)


enum _tag_WEB_POST_ID
{
	POST_ID = 0,
	POST1_ID,
	POST2_ID,
	POST3_ID,
	POST4_ID,
	POST5_ID,
	WEB_POST_NUM
};
                                        
                                        
enum _tag_CTRL_UNIT
{
	PROTECT_ID = 1,
	PM_ID,
	TX2_ID,
	RX2_ID,
	TX1_ID,
	RX1_ID,
    TURN1_ID,
    TURN2_ID,
    TURN3_ID,
	CTRL_UNIT_NUM
};     



/**************************************************************************************************
                                           变量
***************************************************************************************************/
//extern  INT8U           g_nRdMtProt;
extern  INT8U           g_nDispErr;
extern  INT8U           g_nMemDataBuf[512];
//extern  INT32U          g_nCtrlStartSecs;
//extern  INT32U          g_nCtrlEndSecs;
extern  INT8U   g_nTurnSwitckCnt[MAX_TURN_NUM];
extern  INT8U   g_nTripFistFlg[MAX_TURN_NUM];
extern  INT8U   g_nCycelCnt[MAX_TURN_NUM];
extern  INT32U  g_nUART_BPS_NUM;
/**************************************************************************************************
                                           结构
***************************************************************************************************/
//web参数
typedef struct _tagWEBSERVICEPRM
{
	INT8U					nIp[100];
	INT8U					nPort[6];
	INT8U					nApn[35];
	INT8U					nPost[512];
	INT8U					nPost1[512];				// 远程下载配置
	INT8U					nPost2[512];				// 远程下载
	INT8U					nPost3[512];				// 远程下载:载波仪
	INT8U					nPost4[512];				// 下装工单
	INT8U					nPost5[512];				// 上装工单
	INT8U					nPost6[512];				// 退还工单
}WEBSERVICEPRM, *pWEBSERVICEPRM;

//flash结构
typedef struct _tagFLASH_STORE
{
	OPERTOR_PRM				sManagerOpertor;
    INT8U                   nTmpBuf[64];
	INT16U					nOpertorIdx;
	WEBSERVICEPRM			sWebPrm;
	OPERTOR_PRM				sOpertor[30];	
}FLASH_STORE, *pFLASH_STORE;


//系统参数
typedef struct _tagSYS_PARAM
{
    INT8U   				nAreaRun;
    INT8U   				nSimpleMode;							//暂不使用
    OBJ_COMM_PRM			sObjCommPrm[3];
	INT8U 					nCarrierCom;					
	INT8U					nBackClsTime;
	INT8U					nTermClsTime;
	INT8U					nConKeyFlg;
	INT8U					nWorkOrder;
	INT8U					nJcMenu;
	INT8U					nDefaultServ;
	INT8U					nSysParamBuf[61];
}SYS_PARAM, *pSYS_PARAM;

//铁电结构
typedef struct _tagFRAM_STORE
{
	//INT8U					nDownload[30];
    INT8U               	nTermAreaAddr[4];
	INT8U					nLoginBuf[6];
	OPERTOR_PRM				sCurrOpertor;
	INT8U					nCurrWorkId[16];
	INT8U                   nTmpBuf[64];
	SYS_PARAM           	sSysParam;
	
    INT8U               	nPos;
	
    DOWNLOAD_STR        	sUpgrade;
	DOWNLOAD_STR			sCarrUpgrade;
    DOWNLOAD_STR        	sInportMeter;
    DOWNLOAD_STR        	sInportTerm;
	
    INT16U			    	nMeterRecordRdPtr; 
    INT16U			    	nMeterRecordWrPtr; 
    //INT32U			    	nMeterInportFileLen; 
    
    //INT16U			    	nTermRecordRdPtr; 
    //INT16U			    	nTermRecordWrPtr; 
    //INT32U			    	nTermInportFileLen;

	INT16U					nWaveTermRdPtr;
	INT16U					nWaveTermWrPtr;

	INT16U					nOrderIdxRdPtr;
	INT16U					nOrderIdxWrPtr;
}FRAM_STORE, *pFRAM_STORE;


/**************************************************************************************************
                                           声明
***************************************************************************************************/
INT32S OpenCommPort( INT8U port );

//异步串口初始化
INT32S 			AllCommInit(void);
INT32S	 		AsynCommInit(INT8U nCom);
pOBJ_COMM_PRM 	GetObjCommPrmType(INT8U nType);
INT32S 			GetCommPrmFromObj(INT8U nType, INT8S* pMode, INT8S* pBaud, INT8S* pParity);
INT32S 			SetObjCommPrm(INT8S nMode, INT8S nBaud, INT8S nParity);
void            RestoreComPrmCfg( void );
INT8U			SetCommCfg(INT8S nMode, INT8S nBaud, INT8S nParity);
INT8U 			ProcCommCfgPend(INT8U nCom);
INT8U 			ProcCommCfgPost(INT8U nCom);
void    		SetCommCfgExt(INT8U nTermType, INT8U nIrFlg);

INT8U			AreaRunGet(INT8U nType);
void			AreaRunSet(INT8U nAreaRun);
INT8U 			BackClsTimeGet(void);
void 			BackClsTimeSet(INT8U nBackClsTime);
INT8U 			TermClsTimeGet(void);
void 			TermClsTimeSet(INT8U nTermClsTime);
INT8U 			ConKeyFlgGet(void);
void 			ConKeyFlgSet(INT8U nConKeyFlg);
INT8U			MenuModeGet(void);
void			MenuModeSet(INT8U nMenuMode);
INT8U			CarrierComGet(INT8U nType);
void			CarrierComSet(INT8U nCarrierCom);
INT8U           WorkOrderGet(INT8U nType);
void 			WorkOrderSet(INT8U nOrder);
INT8U 			JcMenuGet(INT8U nType);
void 			JcMenuSet(INT8U nJcMenu);
INT8U 			DefaultServGet(INT8U nType);
void 			DefaultServSet(INT8U nDefaultServ);

void 			LoadParmFromFram(void);
void            LoadParmFromFram0(void);
void 			ResetHardInit(void);
void 			ResetDelayHardInit(INT32U nDelay);
void 			GuiwinRecDataRdExt(void);
void 			GuiwinRecDataRestoreExt(void);
INT32S 			DispPrintProcess(FP32 fPercent);
INT8U 			PrintCommHex(INT8U *pBuff, INT32S nDataLen);
INT32S  		ReadCode(INT8U* pBuf);
INT8U 			BAK_stat(void);

INT32S 			PasswordChk(INT8U* pOperInput, INT8U* pPswInput, INT8S nType);
INT32S 			PasswordSave(INT8U* pOperInput, INT8U* pPswInput, INT8S nType);
INT32S 			CurrWorkidAttrProc(void);
INT32S 			ChkCurrWorkidProc(INT8U* pOperator);

BOOLEAN         IsVldMeterPrm(const pMEM_METER_PRM pMtPrm);
BOOLEAN         IsRedupMtAddr(const pMEM_METER_PRM pMtPrm);
INT8U           BaudChange(INT32U nBaud);

INT32S          RealDataMakeAndStore(pCOL_DATA pRs485ColData, INT16U nSeq);
INT32S          CrvDataMakeAndStore(pCOL_DATA pRs485ColData, INT16U nSeq);
INT32S          DayDataMakeAndStore(pCOL_DATA pRs485ColData, INT16U nSeq, INT8U nPre);
INT32S          MonDataMakeAndStore(pCOL_DATA pRs485ColData, INT16U nSeq, INT8U nPre);
INT32S          RdDataMakeAndStore(pCOL_DATA pRs485ColData, INT16U nSeq);
INT32S          AcRealDataMakeAndStore(pMEM_AC_REAL_DATA pAcRealData);
INT32S          AcRealDataMakeAndStoreProc(pMEM_AC_REAL_DATA pAcRealData);
INT32S          AcDayDataMakeAndStore(pSTD_TIME pTime);
INT32S          AcDayDataMakeAndStoreProc(pSTD_TIME pTime);
INT8S           CheckCol2DayData(INT8U nMeterType, pSTD_TIME pMeterTime, pCOL_DATA pRs485ColData);
INT8S           CheckCol2MonData(INT8U nMeterType, pSTD_TIME pMeterTime, pCOL_DATA pRs485ColData);
INT8U           Format11ToFormat14(const pFORMAT11 pData, INT8U *pBuf);
INT8U           Format26ToFormat25(const pFORMAT26 pData, INT8U *pBuf);

INT8S           FunCalcSinglePhaseSWatt(const pFORMAT09 pPWatt, const pFORMAT09 pQWatt, pFORMAT09 pSWatt);

void            RmotCtrlProcSub(pSTD_TIME pTime, INT8U nTrun);
void            RmotCtrlProc(void);
void            RcSetCtrl(pSTD_TIME pTime, INT8U nType, INT8U nTurns);
INT32S          RcEvtProc(pSTD_TIME pTime, INT8U nTurns);
void            DispBlackCtrl(void);
void            DispWhilteCtrl(void);
void            DispRecCtrl(void);
void            GprsPowerOn(void);
void            GprsPowerOff(void);


void 			GpioModlCfg(void);


void           GprsLightUnflash(void);
void           GprsLightFlash(void);
void           GprsCommAbnormal(void);
void           GprsCommNormal(void);
void           TermOffline(void);
void           TermOnline(void);
void           SimAbnormal(void);
void           SimNormal(void);
void           CarrModeNwork(void);
void           CarrModework(void);
void           CarrCopyAbnRd(void);
void           CarrCopyRd(void);
void           BlackScreen(void);
void           WhiteScreen(void);
void           Buttonrespond(void);
void           LightAbnormal(void);
void           TermCommNrespond(void);
void           Restore(void);
void           TremNoPower(void);
void           ScreenNoPower(void);
void           InterfaceNoPower(void);
void           TwelveVoltFault(void);
void           FiveVoltFault(void);
void           Recover(void);
void           RadioAbnormal(void);
void           RadioRunNormal(void);
void           ScreenFault(void);
void           AnchorFault(void);
void           InterfaceNoFault(void);
void           PulseMouthFault(void);
void           PulseState(void);
void           PortFault(void);
void           PortOpenCircuit(void);
void           PortState(void);
void           RemoteFault(void);
void           RemoteOpenCircuit(void);
void           RemoteState(void);
void           ShakeFault(void);
void           ShakeOpenCircuit(void);
void           ShakeState(void);

INT8S  PowerDataFault(INT8U     nType);

INT8S  DemandFault(INT8U     nType);

#endif
