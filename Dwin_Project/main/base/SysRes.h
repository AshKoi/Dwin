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
                                          相关宏定义
***************************************************************************************************/
//进程优先级
#define		TASK_10MSEC_PRIO			4				    //10毫秒级
#define		TASK_MSEC_PRIO				5				    //毫秒级
#define		TASK_SEC_PRIO				6				    //秒级
#define		TASK_DISP_PRIO				13				    //显示级
#define		TASK_LOCAL_LINK_PRIO		8           	    //本地232
#define		TASK_LOCAL_LINK2_PRIO		11           	    //本地维护485
#define		TASK_BLE_LINK_PRIO		    9           	    //本地蓝牙
#define		TASK_RS485_COL_PRIO		    14           	    //485采集
#define		TASK_CARR_COL2_PRIO		    10           	    //载波采集
#define		TASK_AC_COL_PRIO		    7           	    //交流采样采集
#define		TASK_IDLE_PRIO		        20           	    //空闲任务

//#define		TASK_RMOT_LINK_PRIO		    10           	    //远程GPRS
//#define		TASK_ENETCLI_LINK_PRIO	    9                   //以太网客户端
//#define		TASK_ENETSER_LINK_PRIO	    10                  //以太网服务端
//#define		TASK_REMOT_CTRL_PRIO	    16                  //遥控
//#define		TASK_CARR_COL_PRIO		    15           	    //载波采集

//进程堆栈长度
#define		TASK_STK_SIZE				256			
#define		TASK_10MSEC_STK			    (TASK_STK_SIZE / 2)		//10毫秒级
#define		TASK_MSEC_STK				(TASK_STK_SIZE / 2)		//毫秒级
#define		TASK_SEC_STK				(TASK_STK_SIZE * 4)		//秒级
#define		TASK_DISP_STK				(TASK_STK_SIZE * 3)		//显示
#define		TASK_LOCAL_LINK_STK		    (TASK_STK_SIZE * 6)		//本地232
#define		TASK_LOCAL_LINK2_STK	    (TASK_STK_SIZE * 6)	    //(TASK_STK_SIZE * 1 + TASK_STK_SIZE / 2)		//本地维护485
#define		TASK_BLE_LINK_STK		    (TASK_STK_SIZE * 6)     //(TASK_STK_SIZE * 1 + TASK_STK_SIZE / 2)		//本地红外
#define		TASK_CARR_COL2_STK		    (TASK_STK_SIZE * 6)     //(TASK_STK_SIZE * 1 + TASK_STK_SIZE / 2)		//载波采集
#define     TASK_AC_COL_STK             (TASK_STK_SIZE * 8)     //0x2000  8k?
#define     TASK_IDLE_STK               (TASK_STK_SIZE * 3)  

//#define		TASK_RMOT_LINK_STK		    (TASK_STK_SIZE * 3)		//远程GPRS
//#define		TASK_ENETCLI_LINK_STK		(TASK_STK_SIZE * 3)     //以太网客户端
//#define		TASK_ENETSER_LINK_STK		(TASK_STK_SIZE * 3)     //以太网服务器
//#define		TASK_REMOT_CTRL_STK		    (TASK_STK_SIZE * 1)     //遥控
//#define		TASK_RS485_COL_STK		    (TASK_STK_SIZE * 1 + TASK_STK_SIZE / 2)		//485采集
//#define		TASK_CARR_COL_STK		    (TASK_STK_SIZE * 1 + TASK_STK_SIZE / 2)		//载波采集

//进程队列
#define     QUEUE_MSG_NUM       		1				//队列内消息数量

//队列编号
enum _tagQUEUE_IDX
{
    QUEUE_RS485_IDX=0,
    QUEUE_NUM
};

//邮箱编号
enum _tagMBOX_IDX
{
	MBOX_FILE_TRANS_IDX=0,	//本地下载时使用，向界面传递已下载的帧序号
    MBOX_SELF_ADDR_IDX,      	//下发资产号
    
	MBOX_NUM
};

//信号量编号
enum _tagSEM_IDX
{
	SEM_10MSEC_IDX=0,			    //10毫秒集任务
	SEM_MSEC_IDX,				    //100毫秒集任务
	SEM_SEC_IDX,				    //秒级任务
    SEM_TASK_CHECK_IDX,		        //进程监视
	SEM_SOFTTIMER_VAL,		        //软时钟
	SEM_RMOT_PARM,				    //远程通信参数
    
	// 通知某进程可以调用某函数的信号量
    SEM_DISP_IDX,             	    //显示初始化完毕标志   home键后需要post一下
    SEM_WAIT_FILE_IDX,		        //本地档案下载使用，需要进入下载界面下才可以下载    home键后需要clear

	// 原子过程，不能打断    home键后需pend
	SEM_LCDPROTECT_IDX,		        //显示操作保护信号
	SEM_KEY_IDX,				    //按键
    SEM_FLASH_IDX,				    //flash操作信号，杀死显示任务时需要调用
    SEM_FRAM_IDX,				    //fram操作信号
    SEM_DWN_IDX,				    //下装操作信号

	// home键可以打断，需要重新post		
	SEM_TRANS_COM485_IDX	,
    SEM_TRANS_COM4852_IDX	,
    SEM_TRANS_COMIR_IDX     ,
    SEM_TRANS_COMZB_IDX	,
    SEM_GW3761_BUF_IDX,		        //3761规约发送缓冲区
    SEM_STORE_BUF_IDX,		        //数据转存缓冲区
    //SEM_AC_BUF_IDX,                 //交采数据缓冲区
    
    //驱动所用缓冲区变量
    SEM_DRV_CASH_IDX,               //驱动所用缓冲区变量的信号量
    
    SEM_CLEAR_BUF_IDX,		        //清flash缓冲区
    
    SEM_CARR_IDX,                   //载波
    
    SEM_RS485_1_IDX,      	        //485_1
    SEM_RS485_2_IDX,      	        //485_2
    
    //
    SEM_COL_AC_CALC_IDX,		    //AC计算
    SEM_ACREAD_CTRL_IDX,            //交采芯片读写控制
    SEM_ACENWR_CTRL_IDX,			//交采电能数据块写控制
    SEM_AC_METER_CALC_IDX,		    //AC校表计算
	SEM_ENET_FREE_IDX,		        //以太网初始化信息传递信号量
	
	SEM_NUM
    
};


//任务编号
typedef enum _tagTASK_CREATE_IDX
{
	TASK_SEC = 0				,                  //秒级
	TASK_10MSEC				    ,                  //10毫秒级 
	TASK_MSEC					,                  //毫秒级   
	TASK_DISP					,                  //显示 
    TASK_LLINK					,                  //本地232
    TASK2_LLINK					,                  //本地4851
	//TASK_RLINK					,                  //远程GPRS
    TASK_BLELINK					,                  //蓝牙
    //TASK_RS485_COL				,                  //485采集
    //TASK_CARR_COL				,                  //载波采集
    TASK_CARR2_COL				,                  //载波采集
#if ENET_COMM_FUNC_EN == 1
	TASK_ENETCLILINK			,				   //以太网客户机
	TASK_ENETSRVLINK			,				   //以太网服务器
#endif
#if REMOT_CTRL_EN == 1
    TASK_REMOT_CTRL             ,                  //遥控
#endif
#if AC_FUNC_EN == 1
    TASK_AC_COL             ,                       //交采
#endif
    TASK_IDLE               ,
    
    MAX_USER_TASK_NUM			
}TASK_CREATE_IDX;

//进程用户信息
typedef struct _tagTASK_USER_DATA
{
    INT8U     nTaskName[16];				//任务名
    INT16U    nTaskCtr;				        //任务运行次数
    STD_TIME  sLastRunTime;        	        //任务最后运行时间(年月日时分秒)
    INT32U    nTaskExecTime;        		//任务本次运行时间
    INT32U    nTaskTotExecTime;         	//任务总运行时间
    INT8U	  nTaskCheckTOS;				//判断任务超时时间

    INT32U	  nTaskRunVal;					//每个任务一轮运行最长等待时间
}TASK_USER_DATA, *pTASK_USER_DATA;

typedef void 		(*pTASK_FUNC)(void* pArg);
typedef OS_STK*		pOS_STK;
typedef void*		pVOID;
//任务创建结构
typedef struct _tagTASK_CREATE
{
	const INT16U   		nId;				//任务序号
	const char*	const	pName;			//任务名称
	
	const pTASK_FUNC	pTaskFunc;			//任务创建函数
    const pVOID			pArg;				//任务参数
    const pOS_STK		pStack;				//任务栈指针
    const INT8U    		nPrio;				//任务优先级
    const INT32U		nStackSize;			//任务栈大小
    
    const pVOID			pExt;				//任务扩展用指针
    const INT8U			nCheckTOS;			//任务钩子函数自检超时时间
    const INT32U		nRunOnceTOS;		//任务运行一轮最大时间
}TASK_CREATE,*pTASK_CREATE;
/**************************************************************************************************
                                           相关全局变量声明
***************************************************************************************************/
//进程资源
extern	OS_EVENT	*g_pQueue[];									//队列
extern	void		*g_pQueueMsg[][QUEUE_MSG_NUM];					//队列消息空间
extern	OS_EVENT  	*g_pMBox[];										//邮箱
extern	OS_EVENT  	*g_pSem[];										//信号量



//****************************************任务***************************************************
extern TASK_USER_DATA 	g_sTaskUserData[MAX_USER_TASK_NUM];
extern TASK_CREATE 		g_sTaskCreateTbl[MAX_USER_TASK_NUM];





//****************************************任务创建函数***************************************************
INT8S SysSetTaskRunOnceTOS(INT8U nId);
INT8S SysGetTaskRunOnceTOS(INT8U nId, INT32U* pTos, INT8U nFlg);
INT8S SysTaskCreate(INT8U nId);
INT8S SysSetTaskInit(INT8U nId);
#endif
