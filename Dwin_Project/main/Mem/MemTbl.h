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
#ifndef _MEM_TBL_H_
#define _MEM_TBL_H_

//#include "AppHead.h"
//#include "AppCfg.h"
#include "MemPrm.h"
#include "MemData.h"


/**************************************************************************************************
                                          存储对象定义
***************************************************************************************************/
enum _tagMEM_OBJ_TBL
{
    //参数
    MEM_OBJ_PRM_AC_CAL=0,                   //校表参数
    MEM_OBJ_PRM_AC,                         //交采参数
	MEM_OBJ_PRM_TERM_COMM,				    //终端通讯参数
    MEM_OBJ_PRM_METER,					    //电表参数
    MEM_OBJ_PRM_GRP,						//总加组参数
    MEM_OBJ_PRM_RDMETER,					//终端运行参数
    MEM_OBJ_PRM_METER_TYPE_I,			    //大小类
    MEM_OBJ_PRM_METER_TYPE_II,			    //大小类
    MEM_OBJ_PRM_TASK_I,					    //一类任务
    MEM_OBJ_PRM_TASK_II,					//二类任务
    MEM_OBJ_PRM_CTRL,						//控制类参数
    
    MEM_OBJ_PRM_OTHER,					    //其他参数
    MEM_OBJ_PRM_EXT,						//外扩类参数

	//数据
	MEM_OBJ_REAL_DATA,					    //实时数据
	MEM_OBJ_DAY_DATA,						//日数据
	MEM_OBJ_MON_DATA,						//月数据
	MEM_OBJ_CRV_DATA,						//曲线数据
	MEM_OBJ_RDAY_DATA,					    //抄表日数据

	//交互数据
	MEM_OBJ_X_RAM_DATA,					    //RAM交互
	MEM_OBJ_X_FLH_DATA,					    //FLASH交互
	MEM_OBJ_X_FRAM_DATA,					//FRAM交互

//事件记录ID必须按顺排放，这中间不能添加其它ID
    //交采数据
	MEM_OBJ_AC_INSTANT,		            //AC瞬时数据
	MEM_OBJ_AC_REAL,                    //AC实时数据
	MEM_OBJ_AC_BACK,					//AC实时备份
#if 0
	MEM_OBJ_AC_DAY,         			//日数据
    MEM_OBJ_AC_RDAY,        			//抄表日数据
    MEM_OBJ_AC_SSDJ,        			//瞬时冻结数据
    MEM_OBJ_AC_DSDJ,        			//定时冻结数据
    MEM_OBJ_AC_SQDJ,					//约定冻结数据
    MEM_OBJ_AC_SDDJ,					//约定冻结数据
#endif
    
    MEM_OBJ_AC_HIST_DATA,
    
//事件记录ID必须按顺排放，这中间不能添加其它ID
    MEM_OBJ_AC_EVT,	                    //交采事件
#if 0
    MEM_OBJ_AC_EVT_AXSY,	            //失压事件(A)
	MEM_OBJ_AC_EVT_BXSY,	            //失压事件(B)
	MEM_OBJ_AC_EVT_CXSY,	            //失压事件(C)
	MEM_OBJ_AC_EVT_AXDX,	            //断相事件(A)
	MEM_OBJ_AC_EVT_BXDX,	            //断相事件(B)
	MEM_OBJ_AC_EVT_CXDX,	            //断相事件(C)
	MEM_OBJ_AC_EVT_QSY,		            //全失压事件
	MEM_OBJ_AC_EVT_DYNX,	            //电压逆相事件
	MEM_OBJ_AC_EVT_AXSL,	            //失流事件(A)
	MEM_OBJ_AC_EVT_BXSL,	            //失流事件(B)
	MEM_OBJ_AC_EVT_CXSL,	            //失流事件(C)
	MEM_OBJ_AC_EVT_DD,		            //掉电事件
	MEM_OBJ_AC_EVT_XLQL,                //需量清零事件
	MEM_OBJ_AC_EVT_DBQL,                //电表清零事件
	MEM_OBJ_AC_EVT_SJQL,                //事件清零事件
	MEM_OBJ_AC_EVT_BC,                  //编程事件
	MEM_OBJ_AC_EVT_JS,                  //校时事件
#endif
    MEM_OBJ_AC_X_BACK,		            //(FRAM备份)交互数据
	MEM_OBJ_AC_X_BACK_FLH,	            //(FLASH备份)交互数据
    
	
    MEM_OBJ_TBL_NUM 						//数量
};


/**************************************************************************************************
                                        数据结构存储封装定义
***************************************************************************************************/

/**************************************************************************************************
                                          SDRAM存储镜像定义
***************************************************************************************************/
//SDRAM存储镜像
typedef struct _tagRAM_MEM
{
	//终端参数
    MEM_AC_CAL_PRM          sAcCalPrm;                              	//交采校表参数
    MEM_AC_PRM              sAcPrm;                                 	//交采参数
	MEM_TERM_COMM_PRM  		sTermComm;									//1.终端通讯参数
    //MEM_METER_PRM			sMeter[MAX_METER_NUM];				        //2.电表参数
    MEM_CTRL_PRM            sCtrlPrm;                              	    //控制参数
    
	//交互数据
	MEM_X_RAM_DATA			sXRamData;									//内存
	MEM_X_FLH_DATA			sXFlhData;									//FLASH
	MEM_X_FRAM_DATA			sXFramData;								    //FRAM
    
    //交采数据
    MEM_AC_INST_DATA		sACInstData;							    //AC瞬时数据：不需要掉电保存，只在ram存储即可
  	MEM_AC_REAL_DATA		sACRealData;							    //AC实时数据：操作频繁且需保存，在铁电备份;同时定时在flash保存
    
    MEM_AC_BACK_DATA		sACBackData;							//(FRAM备份)AC交互数据
    MEM_AC_BACK_FLH_DATA	sACBackFlhData;							//(FLASH备份)AC交互数据
}RAM_MEM, *pRAM_MEM;

/**************************************************************************************************
                                          Flash存储镜像定义
***************************************************************************************************/
//Flash终端数据存储镜像    使用了文件系统，不需要镜像结构
typedef struct _tagMEM_FLH_DATA_MAP
{
	INT8U					nData;					
    
}MEM_FLH_DATA_MAP, *pMEM_FLH_DATA_MAP;


/**************************************************************************************************
                                         FRAM存储镜像定义
***************************************************************************************************/
//FRAM存储镜像
typedef struct _tagMEM_FRAM_STORE
{
	//INT8U					nData[8];											//
    MEM_AC_CAL_PRM          sAcCalPrm;                                          //交采校表参数
    MEM_AC_PRM              sAcPrm;                                             //交采参数
    MEM_TERM_COMM_PRM       sTermCommPrm;                                       //终端通信参数
	MEM_X_FRAM_DATA			sXFramData;     									//备份数据（铁电）sXBackData
    
    MEM_AC_REAL_DATA        sAcRealData;                                        //交采实时数据
    MEM_AC_BACK_DATA		sACBackData;							            //(FRAM备份)AC交互数据
}MEM_FRAM_STORE, *pMEM_FRAM_STORE;




/**************************************************************************************************
                                          外部声明
***************************************************************************************************/
extern pRAM_MEM				g_pMem;											//ram镜像


/**************************************************************************************************
                                          相关函数说明
***************************************************************************************************/
void*	MemTblGet(void);
INT32S  MemObjTblNumGet(void);


INT32S 	MemInfoChk(void);


#endif









