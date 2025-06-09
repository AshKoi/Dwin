/******************************************************************************************************
* 2013-2014 ���Ƽ����޹�˾
* �ļ���: 
* ������: ��һ��
* �ա���: 2013/09/21
* �޸���: 
* �ա���: 
* �衡��: 
* �桡��: V1.0
*****************************************************************************************************/
#ifndef _TIMERCOUNT_H_
#define _TIMERCOUNT_H_
#include "Os_cpu.h"
/**************************************************************************************************
                                          ��
***************************************************************************************************/
#define TIMER_TYPE_SEC_FWARD				0				//�뼶����
#define TIMER_TYPE_SEC_BACK				1				//�뼶�ݼ�
//#define TIMER_TYPE_MIN_FWARD				2				//�ּ�����
//#define TIMER_TYPE_MIN_BACK				3				//�ּ��ݼ�


//��ʱ��
typedef enum _tagTIMER_IDX
{
    TIMER_TMP = 0,							//��������AT���ʱʱ��
    TIMER_SGNL_ITVL,						//�ź�
	TIMER_LOGIN_ITVL,						//�ز����
	TIMER_NOCOMM,							//��ͨѶ����
	TIMER_SRVCNOCOMM,						//��������ͨѶ����
	TIMER_BEAT,
	TIMER_SELFCHK,							//�Լ�
	
    TIMER_NUM						//����
}TIMER_IDX;



/**************************************************************************************************
                                          �ṹ
***************************************************************************************************/
//��ʱ��
typedef struct _tagTTIMER
{
	BOOLEAN		bOn;		//״̬
	INT8U		nType;		//����
	INT16U		nValue;		//����ֵ
}TTIMER, *pTTIMER;


//****************************************��ʱ��***************************************************
extern	pTTIMER		g_pTimer;										//��ʱ��



/**************************************************************************************************
                                          ����
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

