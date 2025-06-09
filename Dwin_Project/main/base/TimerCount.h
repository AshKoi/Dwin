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
#ifndef _TIMERCOUNT_H_
#define _TIMERCOUNT_H_
#include "Os_cpu.h"
/**************************************************************************************************
                                          宏
***************************************************************************************************/
#define TIMER_TYPE_SEC_FWARD				0				//秒级递增
#define TIMER_TYPE_SEC_BACK				1				//秒级递减
//#define TIMER_TYPE_MIN_FWARD				2				//分级递增
//#define TIMER_TYPE_MIN_BACK				3				//分级递减


//计时器
typedef enum _tagTIMER_IDX
{
    TIMER_TMP = 0,							//主动发起AT命令超时时间
    TIMER_SGNL_ITVL,						//信号
	TIMER_LOGIN_ITVL,						//重拨间隔
	TIMER_NOCOMM,							//无通讯掉线
	TIMER_SRVCNOCOMM,						//服务器无通讯掉线
	TIMER_BEAT,
	TIMER_SELFCHK,							//自检
	
    TIMER_NUM						//数量
}TIMER_IDX;



/**************************************************************************************************
                                          结构
***************************************************************************************************/
//计时器
typedef struct _tagTTIMER
{
	BOOLEAN		bOn;		//状态
	INT8U		nType;		//类型
	INT16U		nValue;		//计数值
}TTIMER, *pTTIMER;


//****************************************计时器***************************************************
extern	pTTIMER		g_pTimer;										//计时器



/**************************************************************************************************
                                          声明
***************************************************************************************************/
INT32S 	TimerStatGet(pTTIMER pTimer);
INT32S 	TimerStatSet(pTTIMER pTimer, INT8U nOn);
INT32S 	TimerValGet(pTTIMER pTimer);
INT32S 	TimerValSet(pTTIMER pTimer, INT16U nVal);
INT32S 	TimerOpen(pTTIMER pThis, INT16U nInitValue);
INT32S 	TimerClose(pTTIMER pThis);



INT32S 	TimerInit(pTTIMER pThis, INT8U nType);
void 	TimerCtrlProc(INT8U nSecMinFlg);

#endif

