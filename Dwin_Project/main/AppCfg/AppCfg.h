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
#ifndef _APP_CFG_H_
#define _APP_CFG_H_
#include "Os_cpu.h"
/*
*****************************************************************************************************
第二应用选择开关:如果要选择第一应用，则屏掉下面宏
*****************************************************************************************************
*/
//#define SEC_APP							2


//通用功能开关
//#define GPRS_MOD_DEBUG				1									    //GPRS模块DEMO板
#define AUTO_TEST_EN              		0           							//检测主控程序 0：实际应用程序 1：手掌机测试程序 2：载波仪测试程序
#define ENET_COMM_FUNC_EN				0										//以太网开关
#define SOFT_TIME_EN					0										//软时钟开关
#define GPRS_CMD_PRINT_EN         	    0         								//
#define PUL_CALC_FUNC_EN    			0 				        		        //脉冲计算功能(开启)
#define AC_FUNC_EN                      1                                       //交采功能
#define REMOT_CTRL_EN                   0                                       //遥控

#define SINGLEPHASE_METER               1                                                       //单相表

#define TRIPHASE_METER_220                2                                                      //三相四线(220)
#define TRIPHASE_METER_57                3                                                       //三相四线(57.7)
#define TRIPHASE_METER_100                4                                                       //三相三线(100)
#define TRIPHASE_METER_220ZT                5                                                       //三相四线(220)直通
#define TRIPHASE_METER_57U                6                                                       //三相四线(57.7)20000
#define TRIPHASE_METER_100U               7                                                      //三相三线(100)20000



/*
*****************************************************************************************************
单元类型
*****************************************************************************************************
*/
#define ASSIST_RUN_RS485				0										//485采集单元
#define ASSIST_RUN_CARRIER			1										//载波单元
#define ASSIST_RUN_SIGNAL				2										//信号发生器
#define ASSIST_RUN_HANDSHIP			3										//手持设备
#define ASSIST_RUN_TESTSCJ			4										//测试工具
#define ASSIST_RUN_YUNWEISCJ			5										//运维手抄机
#define ASSIST_RUN_FZZD				6										//仿真终端


/*
*****************************************************************************************************
地区选择
*****************************************************************************************************
*/
#define AREA_JIANGSU					0										//江苏
#define AREA_ZHEJIANG					1										//浙江
#define AREA_SICHUAN					2										//四川


/*
*****************************************************************************************************
功能选择
*****************************************************************************************************
*/
#define ASSIST_RUN						ASSIST_RUN_FZZD							//运行单元选择
#define AREA_RUN						AREA_ZHEJIANG							//地区选择选择                   


#ifndef NULL
#define NULL            				0                
#endif



/**************************************************************************************************
                                          文件定义
***************************************************************************************************/
#define G2U_U2G_FILE					"g2uu2g"								// utf与gbk库

#define SELF_UPDATA_FILE				"updata"								// 本身升级
#define CARR_UPDATA_FILE				"cupdata"								// 从主站下载载波检测仪程序
#define CARR_DOWN_FILE					"cdown"									// 手抄机给载波检测仪升级

#define AC_CAL_FILE					    "accalprm"								// 交采校表参数
#define AC_FILE					        "acprm"								    // 交采参数
#define COMM_PRM_FILE					"commprm"								// 通信参数
#define METER_PRM_FILE					"meterprm"								// 表计参数
#define GRP_PRM_FILE					"grpprm"								// 总加组参数
#define RDMETER_PRM_FILE				"rdmprm"								// 终端运行参数
#define METER_TYPE_I_FILE				"mmmiprm"								// I类大小类
#define METER_TYPE_II_FILE			    "mmmiiprm"								// II类大小类
#define TASK_I_FILE					    "taskiprm"								// I类任务
#define TASK_II_FILE					"taskiiprm"								// II类任务
#define CTRL_FILE						"ctrlprm"								// 控制参数
#define OTHER_PRM_FILE					"otherprm"								// 其他参数
#define EXT_PRM_FILE					"extprm"								// 外扩参数

#define REAL_DATA_FILE					"realdata"								// 实时数据
#define DAY_DATA_FILE					"daydata"								// 日数据
#define MON_DATA_FILE					"mondata"								// 月数据
#define CRV_DATA_FILE					"crvdata"								// 曲线数据
#define RDAY_DATA_FILE					"rdaydata"								// 抄表日数据

#define FLASH_XMP_DATA_FILE			    "fxmpdata"								// flash交互数据

#define AC_REAL_BACK_FILE			    "acbkdata"					    //交采实时数据备份
#if 0
#define AC_DAY_FILE			            "acdaydata"					            //交采日数据
#define AC_RD_DAY_FILE			        "acrddaydata"					        //交采抄表日数据
#define AC_DSDJ_FILE			        "acdsdjdata"					        //AC冻结数据
#define AC_SQDJ_FILE			        "acsqdjdata"					        //AC冻结数据
#define AC_SDDJ_FILE			        "acsddjdata"					        //AC冻结数据
#endif

#define AC_HIST_DATA_FILE               "achstdata"                            //AC历史数据
 
#define AC_EVT_FILE                     "acevt"                                 //交采事件
#if 0
#define AC_EVT_AXSY_FILE                "acevtaxsy"                             //失压事件(A)
#define AC_EVT_BXSY_FILE                "acevtbxsy"                             //失压事件(B)
#define AC_EVT_CXSY_FILE                "acevtcxsy"                             //失压事件(C)
#define AC_EVT_AXDX_FILE                "acevtaxdx"                             //断相事件(A)
#define AC_EVT_BXDX_FILE                "acevtbxdx"                             //断相事件(B)
#define AC_EVT_CXDX_FILE                "acevtcxdx"                             //断相事件(C)
#define AC_EVT_QSY_FILE                 "acevtqsy"                              //全失压事件
#define AC_EVT_DYNX_FILE                "acevtdynx"                             //电压逆相事件
#define AC_EVT_AXSL_FILE                "acevtaxsl"                             //失流事件(A)
#define AC_EVT_BXSL_FILE                "acevtbxsl"                             //失流事件(B)
#define AC_EVT_CXSL_FILE                "acevtcxsl"                             //失流事件(C)
#define AC_EVT_DD_FILE                  "acevtdd"                               //掉电事件
#define AC_EVT_XLQL_FILE                "acevtxlql"                             //需量清零事件
#define AC_EVT_DBQL_FILE                "acevtdql"                              //电表清零事件
#define AC_EVT_SJQL_FILE                "acevtsjql"                             //事件清零事件
#define AC_EVT_BC_FILE                  "acevtbc"                               //编程事件
#define AC_EVT_JS_FILE                  "acevtaxjs"                             //校时事件
#endif
//#define AC_EVT_X_BACK_FILE              "acevtxback"                            //交采事件铁电备份
#define AC_EVT_X_BACK_FLH_FILE          "acexbkflh"                         //交采事件flh备份



/**************************************************************************************************
                                     端口逻辑下标定义
***************************************************************************************************/
enum _tagPORT_IDX
{
    AC_PORT_IDX = 0,															//交采
    RS4851_PORT_IDX,															//485_1
    RS4852_PORT_IDX,															//485_2
    RS4853_PORT_IDX,															//485_3
    CARR_PORT_IDX,																//载波
    
    PORT_NUM
};

#define AC_PORT                 		GetPort(AC_PORT_IDX)
#define RS4851_PORT                 	GetPort(RS4851_PORT_IDX)
#define RS4852_PORT                 	GetPort(RS4852_PORT_IDX)
#define RS4853_PORT                  	GetPort(RS4853_PORT_IDX)
#define CARR_PORT                    	GetPort(CARR_PORT_IDX)

#define	BPS_AUTO_SEL    				GetCommBpsAutoSel()					//波特率自适应开关	
#define DISP_ENERGY_SEL              	GetDispEnergySel()                    //下结论时显示电能示值开关



/**************************************************************************************************
                                        终端容量配置
***************************************************************************************************/
#define AREA_MAX_NUM					10										//10个地区

#define MAX_REAL_DATA_NUM			    12										//实时数据数量
#define MAX_DAY_DATA_NUM			    31										//日数据数量
#define MAX_MON_DATA_NUM			    12										//月数据数量
#define MAX_CRV_DATA_NUM			    31										//曲线数据数量
#define MAX_RDAY_DATA_NUM			    31										//抄表日数据数量

#define MAX_RC_EVT_NUM			        10										//遥控跳闸事件次数


#define MAX_METER_NUM		            20		                        		//电表数量
#define MAX_MP		                	999		            					//最大测量点号
#define MAX_IM_METER_NUM		        20		            					//重点电表数量
#define	MAX_COM_NUM						32								        //端口数量

#define MAX_CRV_POINT_NUM				96										//曲线点数

#define MAX_FEE_NUM             		4               						//最大费率数目
#define MAX_PHASE_NUM	            	3                   					//三相


#define MAX_PULSE_NUM					4										//脉冲
#define MAX_ANLOG_NUM					2										//模拟量
#define MAX_GRP_MP_NUM					8										//总加组测量点数目
#define MAX_GRP_NUM					    8										    //总加组数目
#define MAX_DIFF_NUM					8										//差动组


#define MAX_METER_MAXTYPE_NUM			16										//大类号数目
#define MAX_METER_MINTYPE_NUM			16										//小类号数目

#define MAX_READ_PERD_NUM				24								//抄表时段

#define MAX_TURN_NUM					2										//轮次

#define MAX_RELAY_TERM_NUM			8										//中继终端数

#define MAX_TASK_NUM					64										//任务

//交流计量配置

//交流合相电能示值计算方法
#define AC_ENG_CAL_ALG                  0                                   //交流合相电能代数和计算
#define AC_ENG_CAL_ABS                  1                                   //交流合相电能绝对值计算
/*
#if  SINGLEPHASE_METER  
#define	AC_PULSE_CONST	                1200                                //脉冲常数初始化之后为6400
#endif

#if  TRIPHASE_METER  
#define	AC_PULSE_CONST	                6400                                //脉冲常数初始化之后为6400
#endif
*/


#define AC_ULI_STARTUP                  0x000189                        //启动电流 0.2%Ib Class 0.5

#define	CALUCATES_PER_MINUTE	        60		                        //瞬时数据滑差周期(sec)
#define	PARAM_MAX_SLIP_TIME		        5		                        //最大滑差时间(min)
#define	PARAM_MAX_DEMAND_PERIOD	        15		                        //最大需量周期(min)
#define MAX_P_Q_NUM		            	4                   			//PQ数量


/**************************************************************************************************
                                        交采容量配置
***************************************************************************************************/
#define MAX_AC_DAY_DATA_NUM				31//10								//AC日数据数量
#define MAX_AC_RDAY_DATA_NUM			10								//AC抄表日数据数量
#define MAX_AC_SSDJ_DATA_NUM			10								//AC瞬时冻结数据数量
#define MAX_AC_DSDJ_DATA_NUM      		10								//AC定时冻结数据数量
#define MAX_AC_SQDJ_DATA_NUM      		10								//AC约定冻结时区数据数量
#define MAX_AC_SDDJ_DATA_NUM      		10								//AC约定冻结时段数据数量
#define MAX_AC_CRV_DATA_NUM				12								//AC曲线数据数量
#define	MAX_AC_EVT_DATA_NUM				10								//AC事件数据数量
#define MAX_P_Q_NUM		            	4                   			//PQ数量
#define	MAX_AC_MON_DATA_NUM				12								//AC月数据数量
/**************************************************************************************************
                                           结构定义
***************************************************************************************************/
typedef struct _tag_AREA_CFG{
	INT8U				 nArea;                 								//地区
	INT8S				 nAreaString[5];
    INT8U                nBpsAutoSel;           								//波特率自适应开关 1打开，0关闭
    INT8U                nPortBuf[6];           								//
    INT8U                nDispEnergySel;                                      //下结论时显示电能示值
}AREA_CFG, *pAREA_CFG;

typedef INT8S (*pAREA_STRING)[5];
/*
*****************************************************************************************************
功能表
*****************************************************************************************************
*/
extern const INT8S*	g_sAreaString[AREA_MAX_NUM];
extern INT8U		    g_nAutoTestFlg;
extern INT8U           g_nZbDispFlg;


/*
*****************************************************************************************************
声明
*****************************************************************************************************
*/
char** 	FlashFileTblGet(void);

void 	AreaStringMake(void);


INT32S	GetCommBpsAutoSel(void);
INT32S	GetDispEnergySel(void);

INT32S  GetPortIdx(INT8U nPort);
INT32S  GetPort(INT8U nPortIdx);


INT32S 	GetRmotCmdPrintFlg(void);
void 	SetRmotCmdPrintFlg(INT8U nFlg);




#endif
