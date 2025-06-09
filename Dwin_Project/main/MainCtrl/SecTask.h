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

#ifndef _SEC_TASK_H_
#define _SEC_TASK_H_

#include "Os_cpu.h"
#include "RX8025T.h"

	
//extern INT8S                g_nAlterUartBpsflg;

extern INT8U                g_nFlg;
extern INT8U                g_nAutoTime;

extern INT8U                g_nFlgUa;
extern INT8U                g_nFlgUb;
extern INT8U                g_nFlgUc;
extern INT8U                g_nFlgVoltPhase;                                   
extern INT8U                g_nFlgIa;
extern INT8U                g_nFlgIb;
extern INT8U                g_nFlgIc;
extern INT8U                g_nFlg_Commu;
extern INT8U                g_nFlg_Commu2;
extern INT8U                g_nFlg_Alarm;

extern INT8U                    g_nFlg_Pro;	
extern INT8U                  g_nFlgProt;
extern INT8U                  g_nFlgProt1;
extern INT8U                   g_nFlg_Disp;
extern INT8U                   g_nAutoTestFlg1;

extern INT8U                g_nFlg2;

extern INT8U                g_nFlgnRateNo;

extern INT8U                g_nFlgBattery;
extern pSTD_TIME           g_pSysTime;
 
extern INT8U           g_ZBPortFault;
extern INT8U            g_MeterAcErrFlg[4];
extern INT8U           g_MeterVoltPlusFlg;

extern INT8U           g_MeterPowerOff;
 
extern INT8U   g_nFlgReBoot;//g_nFlgHardType
extern INT8U Wrongtime1; 
extern INT8U second1;
extern INT8U second2;
extern INT8U second3;
extern INT8U second4;
extern INT8U second5;

extern INT8U g_IsNewMonCalcRDayMon;
extern INT8U g_IsNewMonCalcRDayFlg;
extern INT8U g_ClockWrong;
extern INT8U g_ClockWrong2;
extern INT8U g_MeterPowerOff;
/**************************************************************************************************
                                          相关函数声明
***************************************************************************************************/
//秒级任务
void SecTask(void *pData);
BOOLEAN IsNewDay(const pSTD_TIME pTime);
BOOLEAN IsNewMon(const pSTD_TIME pTime);
BOOLEAN IsNewMonCalcRDay();
INT32U CheckCom1Baud(void);

#endif

