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
#ifndef _SYS_RES_H_
#define _SYS_RES_H_
#include <stdio.h>
#include <string.h>
#include "Os_cpu.h"
#include "config.h"
#include "global.h"
#include "AppCfg.h"
#include "Macro.h"
#include "Table.h"
#include "Ac7022.h"
/**************************************************************************************************
                                          ��غ궨��
***************************************************************************************************/
//�������ȼ�
#define		TASK_10MSEC_PRIO			4				    //10���뼶
#define		TASK_MSEC_PRIO				5				    //���뼶
#define		TASK_SEC_PRIO				6				    //�뼶
#define		TASK_DISP_PRIO				13				    //��ʾ��
#define		TASK_LOCAL_LINK_PRIO		8           	    //����232
#define		TASK_LOCAL_LINK2_PRIO		11           	    //����ά��485
#define		TASK_BLE_LINK_PRIO		    9           	    //��������
#define		TASK_RS485_COL_PRIO		    14           	    //485�ɼ�
#define		TASK_CARR_COL2_PRIO		    10           	    //�ز��ɼ�
#define		TASK_AC_COL_PRIO		    7           	    //���������ɼ�
#define		TASK_IDLE_PRIO		        20           	    //��������

//#define		TASK_RMOT_LINK_PRIO		    10           	    //Զ��GPRS
//#define		TASK_ENETCLI_LINK_PRIO	    9                   //��̫���ͻ���
//#define		TASK_ENETSER_LINK_PRIO	    10                  //��̫�������
//#define		TASK_REMOT_CTRL_PRIO	    16                  //ң��
//#define		TASK_CARR_COL_PRIO		    15           	    //�ز��ɼ�

//���̶�ջ����
#define		TASK_STK_SIZE				256			
#define		TASK_10MSEC_STK			    (TASK_STK_SIZE / 2)		//10���뼶
#define		TASK_MSEC_STK				(TASK_STK_SIZE / 2)		//���뼶
#define		TASK_SEC_STK				(TASK_STK_SIZE * 4)		//�뼶
#define		TASK_DISP_STK				(TASK_STK_SIZE * 3)		//��ʾ
#define		TASK_LOCAL_LINK_STK		    (TASK_STK_SIZE * 6)		//����232
#define		TASK_LOCAL_LINK2_STK	    (TASK_STK_SIZE * 6)	    //(TASK_STK_SIZE * 1 + TASK_STK_SIZE / 2)		//����ά��485
#define		TASK_BLE_LINK_STK		    (TASK_STK_SIZE * 6)     //(TASK_STK_SIZE * 1 + TASK_STK_SIZE / 2)		//���غ���
#define		TASK_CARR_COL2_STK		    (TASK_STK_SIZE * 6)     //(TASK_STK_SIZE * 1 + TASK_STK_SIZE / 2)		//�ز��ɼ�
#define     TASK_AC_COL_STK             (TASK_STK_SIZE * 8)     //0x2000  8k?
#define     TASK_IDLE_STK               (TASK_STK_SIZE * 3)  

//#define		TASK_RMOT_LINK_STK		    (TASK_STK_SIZE * 3)		//Զ��GPRS
//#define		TASK_ENETCLI_LINK_STK		(TASK_STK_SIZE * 3)     //��̫���ͻ���
//#define		TASK_ENETSER_LINK_STK		(TASK_STK_SIZE * 3)     //��̫��������
//#define		TASK_REMOT_CTRL_STK		    (TASK_STK_SIZE * 1)     //ң��
//#define		TASK_RS485_COL_STK		    (TASK_STK_SIZE * 1 + TASK_STK_SIZE / 2)		//485�ɼ�
//#define		TASK_CARR_COL_STK		    (TASK_STK_SIZE * 1 + TASK_STK_SIZE / 2)		//�ز��ɼ�

//���̶���
#define     QUEUE_MSG_NUM       		1				//��������Ϣ����

//���б��
enum _tagQUEUE_IDX
{
    QUEUE_RS485_IDX=0,
    QUEUE_NUM
};

//������
enum _tagMBOX_IDX
{
	MBOX_FILE_TRANS_IDX=0,	//��������ʱʹ�ã�����洫�������ص�֡���
    MBOX_SELF_ADDR_IDX,      	//�·��ʲ���
    
	MBOX_NUM
};

//�ź������
enum _tagSEM_IDX
{
	SEM_10MSEC_IDX=0,			    //10���뼯����
	SEM_MSEC_IDX,				    //100���뼯����
	SEM_SEC_IDX,				    //�뼶����
    SEM_TASK_CHECK_IDX,		        //���̼���
	SEM_SOFTTIMER_VAL,		        //��ʱ��
	SEM_RMOT_PARM,				    //Զ��ͨ�Ų���
    
	// ֪ͨĳ���̿��Ե���ĳ�������ź���
    SEM_DISP_IDX,             	    //��ʾ��ʼ����ϱ�־   home������Ҫpostһ��
    SEM_WAIT_FILE_IDX,		        //���ص�������ʹ�ã���Ҫ�������ؽ����²ſ�������    home������Ҫclear

	// ԭ�ӹ��̣����ܴ��    home������pend
	SEM_LCDPROTECT_IDX,		        //��ʾ���������ź�
	SEM_KEY_IDX,				    //����
    SEM_FLASH_IDX,				    //flash�����źţ�ɱ����ʾ����ʱ��Ҫ����
    SEM_FRAM_IDX,				    //fram�����ź�
    SEM_DWN_IDX,				    //��װ�����ź�

	// home�����Դ�ϣ���Ҫ����post		
	SEM_TRANS_COM485_IDX	,
    SEM_TRANS_COM4852_IDX	,
    SEM_TRANS_COMIR_IDX     ,
    SEM_TRANS_COMZB_IDX	,
    SEM_GW3761_BUF_IDX,		        //3761��Լ���ͻ�����
    SEM_STORE_BUF_IDX,		        //����ת�滺����
    //SEM_AC_BUF_IDX,                 //�������ݻ�����
    
    //�������û���������
    SEM_DRV_CASH_IDX,               //�������û������������ź���
    
    SEM_CLEAR_BUF_IDX,		        //��flash������
    
    SEM_CARR_IDX,                   //�ز�
    
    SEM_RS485_1_IDX,      	        //485_1
    SEM_RS485_2_IDX,      	        //485_2
    
    //
    SEM_COL_AC_CALC_IDX,		    //AC����
    SEM_ACREAD_CTRL_IDX,            //����оƬ��д����
    SEM_ACENWR_CTRL_IDX,			//���ɵ������ݿ�д����
    SEM_AC_METER_CALC_IDX,		    //ACУ�����
	SEM_ENET_FREE_IDX,		        //��̫����ʼ����Ϣ�����ź���
	
	SEM_NUM
    
};


//������
typedef enum _tagTASK_CREATE_IDX
{
	TASK_SEC = 0				,                  //�뼶
	TASK_10MSEC				    ,                  //10���뼶 
	TASK_MSEC					,                  //���뼶   
	TASK_DISP					,                  //��ʾ 
    TASK_LLINK					,                  //����232
    TASK2_LLINK					,                  //����4851
	//TASK_RLINK					,                  //Զ��GPRS
    TASK_BLELINK					,                  //����
    //TASK_RS485_COL				,                  //485�ɼ�
    //TASK_CARR_COL				,                  //�ز��ɼ�
    TASK_CARR2_COL				,                  //�ز��ɼ�
#if ENET_COMM_FUNC_EN == 1
	TASK_ENETCLILINK			,				   //��̫���ͻ���
	TASK_ENETSRVLINK			,				   //��̫��������
#endif
#if REMOT_CTRL_EN == 1
    TASK_REMOT_CTRL             ,                  //ң��
#endif
#if AC_FUNC_EN == 1
    TASK_AC_COL             ,                       //����
#endif
    TASK_IDLE               ,
    
    MAX_USER_TASK_NUM			
}TASK_CREATE_IDX;

//�����û���Ϣ
typedef struct _tagTASK_USER_DATA
{
    INT8U     nTaskName[16];				//������
    INT16U    nTaskCtr;				        //�������д���
    STD_TIME  sLastRunTime;        	        //�����������ʱ��(������ʱ����)
    INT32U    nTaskExecTime;        		//���񱾴�����ʱ��
    INT32U    nTaskTotExecTime;         	//����������ʱ��
    INT8U	  nTaskCheckTOS;				//�ж�����ʱʱ��

    INT32U	  nTaskRunVal;					//ÿ������һ��������ȴ�ʱ��
}TASK_USER_DATA, *pTASK_USER_DATA;

typedef void 		(*pTASK_FUNC)(void* pArg);
typedef OS_STK*		pOS_STK;
typedef void*		pVOID;
//���񴴽��ṹ
typedef struct _tagTASK_CREATE
{
	const INT16U   		nId;				//�������
	const char*	const	pName;			//��������
	
	const pTASK_FUNC	pTaskFunc;			//���񴴽�����
    const pVOID			pArg;				//�������
    const pOS_STK		pStack;				//����ջָ��
    const INT8U    		nPrio;				//�������ȼ�
    const INT32U		nStackSize;			//����ջ��С
    
    const pVOID			pExt;				//������չ��ָ��
    const INT8U			nCheckTOS;			//�����Ӻ����Լ쳬ʱʱ��
    const INT32U		nRunOnceTOS;		//��������һ�����ʱ��
}TASK_CREATE,*pTASK_CREATE;
/**************************************************************************************************
                                           ���ȫ�ֱ�������
***************************************************************************************************/
//������Դ
extern	OS_EVENT	*g_pQueue[];									//����
extern	void		*g_pQueueMsg[][QUEUE_MSG_NUM];					//������Ϣ�ռ�
extern	OS_EVENT  	*g_pMBox[];										//����
extern	OS_EVENT  	*g_pSem[];										//�ź���



//****************************************����***************************************************
extern TASK_USER_DATA 	g_sTaskUserData[MAX_USER_TASK_NUM];
extern TASK_CREATE 		g_sTaskCreateTbl[MAX_USER_TASK_NUM];





//****************************************���񴴽�����***************************************************
INT8S SysSetTaskRunOnceTOS(INT8U nId);
INT8S SysGetTaskRunOnceTOS(INT8U nId, INT32U* pTos, INT8U nFlg);
INT8S SysTaskCreate(INT8U nId);
INT8S SysSetTaskInit(INT8U nId);
#endif
