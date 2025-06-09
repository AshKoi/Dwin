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
#include "AppCfg.h"
#include "MemTbl.h"
#include "MemDrv.h"

/**************************************************************************************************
                                              内存实体
***************************************************************************************************/
static RAM_MEM		g_sMem;
pRAM_MEM		    g_pMem = &g_sMem;


/**************************************************************************************************
                                        参数存储偏移信息
***************************************************************************************************/
extern    const    MEM_OFF_INFO   g_sTermCommOff[];		        //终端通讯参数
extern    const    MEM_OFF_INFO   g_sMeterOff[];		        	//终端表计参数
extern	  const    MEM_OFF_INFO   g_sGrpOff[];						//总加组
extern	  const    MEM_OFF_INFO   g_sRdMeterOff[];					//终端运行
extern	  const    MEM_OFF_INFO   g_sMeterTypeIOff[];				//I类大小类
extern	  const    MEM_OFF_INFO   g_sMeterTypeIIOff[];			//II类大小类
extern	  const    MEM_OFF_INFO   g_sTaskIOff[];					//I类任务
extern	  const    MEM_OFF_INFO   g_sTaskIIOff[];					//II类任务
extern	  const    MEM_OFF_INFO   g_sCtrlOff[];					//控制参数
extern	  const    MEM_OFF_INFO   g_sAcCalPrmOff[];						//交采校表参数
extern	  const    MEM_OFF_INFO   g_sAcPrmOff[];						//交采参数
extern   const    MEM_OFF_INFO   g_sOtherPrmOff[];				//其他参数
extern	  const    MEM_OFF_INFO   g_sExtOff[];						//外扩类参数


extern    const    MEM_OFF_INFO   g_sMpRealDataOff[];		        //实时数据
extern    const    MEM_OFF_INFO   g_sMpDayDataOff[];		        //日数据
extern    const    MEM_OFF_INFO   g_sMpMonDataOff[];		        //月数据
extern    const    MEM_OFF_INFO   g_sMpCrvDataOff[];		        //曲线数据
extern    const    MEM_OFF_INFO   g_sMpRDayDataOff[];		        //抄表日数据

extern    const    MEM_OFF_INFO   g_sXRamDataOff[];		        //RAM临时交互数据
extern    const    MEM_OFF_INFO   g_sXFlhDataOff[];		        //FLH临时交互数据
extern    const    MEM_OFF_INFO   g_sXFramDataOff[];		        //FRAM临时交互数据

extern    const    MEM_OFF_INFO   g_sACInstDataOff[];
extern    const    MEM_OFF_INFO   g_sACRealDataOff[];	
extern    const    MEM_OFF_INFO   g_sACRealBackOff[];

#if 0
extern    const    MEM_OFF_INFO   g_sACDayDataOff[];	
extern    const    MEM_OFF_INFO   g_sACRDayDataOff[];	
extern    const    MEM_OFF_INFO   g_sACDjDataOff[];
#endif

extern    const    MEM_OFF_INFO   g_sACHistDataOff[];
extern    const    MEM_OFF_INFO   g_sACEvtDataOff[];
extern    const    MEM_OFF_INFO   g_sACBackDataOff[];
extern    const    MEM_OFF_INFO   g_sACBackFlhDataOff[];





/**************************************************************************************************
                                          存储基本信息
***************************************************************************************************/
//基址定位信息
const MEM_INFO g_sTblMem[] =
{
    //数据库表号				//行数					//列信息			//介质信息(介质、基址、文件名)
	
	//参数
	{MEM_OBJ_PRM_AC_CAL,		1,						(pMEM_OFF_INFO)(g_sAcCalPrmOff), 	
																		{ 	{MEM_MEDIUM_RAM, (INT32U)(&g_sMem.sAcCalPrm)},
                                                                            {MEM_MEDIUM_FLASH, 0, AC_CAL_FILE},   	
										                                	{MEM_MEDIUM_FRAM, STR_OFF(pMEM_FRAM_STORE, sAcCalPrm)} 
                                                                        }	},
    {MEM_OBJ_PRM_AC,			1,						(pMEM_OFF_INFO)(g_sAcPrmOff), 	
																		{ 	{MEM_MEDIUM_RAM, (INT32U)(&g_sMem.sAcPrm)},
                                                                            {MEM_MEDIUM_FLASH, 0, AC_FILE},   	
										                                	{MEM_MEDIUM_FRAM, STR_OFF(pMEM_FRAM_STORE, sAcPrm)} 
                                                                        }	},
	{MEM_OBJ_PRM_TERM_COMM,		1,						(pMEM_OFF_INFO)(g_sTermCommOff), 	
																		{ 	{MEM_MEDIUM_RAM, (INT32U)(&g_sMem.sTermComm)},    		
					                                						{MEM_MEDIUM_FLASH, 0, COMM_PRM_FILE},
					                                						{MEM_MEDIUM_FRAM, STR_OFF(pMEM_FRAM_STORE, sTermCommPrm)} 
                                                                        }	},
    {MEM_OBJ_PRM_METER,			MAX_METER_NUM,			(pMEM_OFF_INFO)(g_sMeterOff), 		
																		{ 	{MEM_MEDIUM_FLASH, 0, METER_PRM_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},
	{MEM_OBJ_PRM_GRP,			MAX_GRP_NUM,			(pMEM_OFF_INFO)(g_sGrpOff), 		
																		{ 	{MEM_MEDIUM_FLASH, 0, GRP_PRM_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},
	{MEM_OBJ_PRM_RDMETER,		PORT_NUM,				(pMEM_OFF_INFO)(g_sRdMeterOff), 	
																		{ 	{MEM_MEDIUM_FLASH, 0, RDMETER_PRM_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},
	{MEM_OBJ_PRM_METER_TYPE_I,	MAX_METER_MAXTYPE_NUM,	(pMEM_OFF_INFO)(g_sMeterTypeIOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, METER_TYPE_I_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},
	{MEM_OBJ_PRM_METER_TYPE_II,	MAX_METER_MAXTYPE_NUM,	(pMEM_OFF_INFO)(g_sMeterTypeIIOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, METER_TYPE_II_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},
	{MEM_OBJ_PRM_TASK_I,		MAX_TASK_NUM,			(pMEM_OFF_INFO)(g_sTaskIOff), 		
																		{ 	{MEM_MEDIUM_FLASH, 0, TASK_I_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},
	{MEM_OBJ_PRM_TASK_II,		MAX_TASK_NUM,			(pMEM_OFF_INFO)(g_sTaskIIOff), 	
																		{ 	{MEM_MEDIUM_FLASH, 0, TASK_II_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},
	{MEM_OBJ_PRM_CTRL,			1,						(pMEM_OFF_INFO)(g_sCtrlOff),
                                                                        {    {MEM_MEDIUM_RAM, (INT32U)(&g_sMem.sCtrlPrm)},
																		 	{MEM_MEDIUM_FLASH, 0, CTRL_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	}, 
    {MEM_OBJ_PRM_OTHER,			1,						(pMEM_OFF_INFO)(g_sOtherPrmOff), 	
																		{ 	{MEM_MEDIUM_FLASH, 0, OTHER_PRM_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},                                                                       
	{MEM_OBJ_PRM_EXT,			1,						(pMEM_OFF_INFO)(g_sExtOff), 	
																		{ 	{MEM_MEDIUM_FLASH, 0, EXT_PRM_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},
																				
	//数据
	//实时
	{MEM_OBJ_REAL_DATA,			MAX_METER_NUM,			(pMEM_OFF_INFO)(g_sMpRealDataOff),
																		{ 	{MEM_MEDIUM_FLASH, 0, REAL_DATA_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},
	//日
	{MEM_OBJ_DAY_DATA,			MAX_METER_NUM,			(pMEM_OFF_INFO)(g_sMpDayDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, DAY_DATA_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},
	//月
	{MEM_OBJ_MON_DATA,			MAX_METER_NUM,			(pMEM_OFF_INFO)(g_sMpMonDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, MON_DATA_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},
	//曲线
	{MEM_OBJ_CRV_DATA,			MAX_METER_NUM,			(pMEM_OFF_INFO)(g_sMpCrvDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, CRV_DATA_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},																		
	//抄表日
	{MEM_OBJ_RDAY_DATA,			MAX_METER_NUM,			(pMEM_OFF_INFO)(g_sMpRDayDataOff),
																		{ 	{MEM_MEDIUM_FLASH, 0, RDAY_DATA_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},	
																			
	//交互数据
	{MEM_OBJ_X_RAM_DATA,		1,						(pMEM_OFF_INFO)(g_sXRamDataOff), 	
																		{ 	{MEM_MEDIUM_RAM, (INT32U)(&g_sMem.sXRamData)},
										                                	{MEM_MEDIUM_INVLD} }	},
	{MEM_OBJ_X_FLH_DATA,		1,						(pMEM_OFF_INFO)(g_sXFlhDataOff), 	
																		{ 	{MEM_MEDIUM_RAM, (INT32U)(&g_sMem.sXFlhData)},     		
										                                	{MEM_MEDIUM_FLASH, 0, FLASH_XMP_DATA_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},
	{MEM_OBJ_X_FRAM_DATA,		1,						(pMEM_OFF_INFO)(g_sXFramDataOff), 
																		{ 	{MEM_MEDIUM_RAM, (INT32U)(&g_sMem.sXFramData)},     		
										                                	{MEM_MEDIUM_FRAM, STR_OFF(pMEM_FRAM_STORE, sXFramData)},   	
										                                	{MEM_MEDIUM_INVLD} }	},
                                                                                                                                                                                                                     
    //交采数据
    //AC瞬时数据
    {MEM_OBJ_AC_INSTANT,		1,						(pMEM_OFF_INFO)(g_sACInstDataOff), 
																		{ 	{MEM_MEDIUM_RAM, (INT32U)(&g_sMem.sACInstData)},     		
										                                	{MEM_MEDIUM_INVLD},   	
										                                	{MEM_MEDIUM_INVLD} }	},
     //AC实时数据
    {MEM_OBJ_AC_REAL,		    1,						(pMEM_OFF_INFO)(g_sACRealDataOff), 
																		{ 	{MEM_MEDIUM_RAM, (INT32U)(&g_sMem.sACRealData)},     		
										                                	{MEM_MEDIUM_FRAM, STR_OFF(pMEM_FRAM_STORE, sAcRealData)},   
										                                	{MEM_MEDIUM_INVLD} }	},
    //AC实时备份
    {MEM_OBJ_AC_BACK,		    1,						(pMEM_OFF_INFO)(g_sACRealBackOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_REAL_BACK_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},                                                                      
#if 0
     //AC日数据
    {MEM_OBJ_AC_DAY,		    1,						(pMEM_OFF_INFO)(g_sACDayDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_DAY_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},                                                                       
    //AC抄表日数据
    {MEM_OBJ_AC_RDAY,		    1,						(pMEM_OFF_INFO)(g_sACRDayDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_RD_DAY_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},
                                                                            
    //AC冻结数据
    {MEM_OBJ_AC_DSDJ,		    1,						(pMEM_OFF_INFO)(g_sACDjDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_DSDJ_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},
    //AC冻结数据
    {MEM_OBJ_AC_SQDJ,		    1,						(pMEM_OFF_INFO)(g_sACDjDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_SQDJ_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},  
    //AC冻结数据
    {MEM_OBJ_AC_SDDJ,		    1,						(pMEM_OFF_INFO)(g_sACDjDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_SDDJ_FILE},   	
									                                	{MEM_MEDIUM_INVLD} }	}, 
#endif
    //AC冻结数据
    {MEM_OBJ_AC_HIST_DATA,		    1,						(pMEM_OFF_INFO)(g_sACHistDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_HIST_DATA_FILE},   	
									                                	{MEM_MEDIUM_INVLD} }	},
    //失压事件(A)
    {MEM_OBJ_AC_EVT,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},
#if 0    
    //失压事件(A)
    {MEM_OBJ_AC_EVT_AXSY,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_AXSY_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	}, 
    //失压事件(B)
    {MEM_OBJ_AC_EVT_BXSY,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_BXSY_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	}, 
    //失压事件(C)
    {MEM_OBJ_AC_EVT_CXSY,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_CXSY_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	}, 
    //断相事件(A)
    {MEM_OBJ_AC_EVT_AXDX,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_AXDX_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	}, 
    //断相事件(B)
    {MEM_OBJ_AC_EVT_BXDX,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_BXDX_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	}, 
    //断相事件(C)
    {MEM_OBJ_AC_EVT_CXDX,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_CXDX_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},
    //全失压事件
    {MEM_OBJ_AC_EVT_QSY,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_QSY_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	}, 
    //电压逆相事件
    {MEM_OBJ_AC_EVT_DYNX,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_DYNX_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	}, 
    //失流事件(A)
    {MEM_OBJ_AC_EVT_AXSL,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_AXSL_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	}, 
    //失流事件(B)
    {MEM_OBJ_AC_EVT_BXSL,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_BXSL_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	}, 
    //失流事件(C)
    {MEM_OBJ_AC_EVT_CXSL,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_CXSL_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	}, 
    //掉电事件
    {MEM_OBJ_AC_EVT_DD,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_DD_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},  
    //需量清零事件
    {MEM_OBJ_AC_EVT_XLQL,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_XLQL_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	}, 
    //电表清零事件
    {MEM_OBJ_AC_EVT_DBQL,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_DBQL_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	}, 
    //事件清零事件
    {MEM_OBJ_AC_EVT_SJQL,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_SJQL_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	}, 
    //编程事件
    {MEM_OBJ_AC_EVT_BC,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_BC_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	}, 
    //校时事件
    {MEM_OBJ_AC_EVT_JS,		    1,						(pMEM_OFF_INFO)(g_sACEvtDataOff), 
																		{ 	{MEM_MEDIUM_FLASH, 0, AC_EVT_JS_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},                                                                          
#endif
    //(FRAM备份)交互数据
    {MEM_OBJ_AC_X_BACK,		    1,						(pMEM_OFF_INFO)(g_sACBackDataOff), 
                                                                        { 	{MEM_MEDIUM_RAM, (INT32U)(&g_sMem.sACBackData)},
																		 	{MEM_MEDIUM_FRAM, STR_OFF(pMEM_FRAM_STORE, sACBackData)},   	
										                                	{MEM_MEDIUM_INVLD} }	},
    //(FLASH备份)交互数据
    {MEM_OBJ_AC_X_BACK_FLH,		    1,						(pMEM_OFF_INFO)(g_sACBackFlhDataOff), 
                                                                        { 	{MEM_MEDIUM_RAM, (INT32U)(&g_sMem.sACBackFlhData)},
																		 	{MEM_MEDIUM_FLASH, 0, AC_EVT_X_BACK_FLH_FILE},   	
										                                	{MEM_MEDIUM_INVLD} }	},                                                                        
    {MEM_OBJ_TBL_NUM}
};


#if 0
#endif

/********************************************************************************
* 函数名: MemInfoChk
* 输　入:
* 输　出:
* 返  回:
* 功  能: 核查
*********************************************************************************/
void*	MemTblGet(void)
{
	return (void*)(&g_sTblMem[0]);
}

/********************************************************************************
* 函数名: MemObjTblNumGet
* 输　入:
* 输　出:
* 返  回:
* 功  能: 核查
*********************************************************************************/
INT32S  MemObjTblNumGet(void)
{
	return 	MEM_OBJ_TBL_NUM;
}


/********************************************************************************
* 函数名: MemInfoChk
* 输　入:
* 输　出:
* 返  回:
* 功  能: 核查
*********************************************************************************/
static INT32S	MemOffChk(pMEM_OFF_INFO pMemOff)
{
	INT32S		i	= 0;
	INT32S		j	= 0;

	while(1)
	{
		if (pMemOff[i].nItmTblId == FEND)
		{
			break;
		}
		
		j = i + 1;
		while(1)
		{
			if (pMemOff[j].nItmTblId == FEND)
			{
				break;
			}
			//1.nFn不能相等
			if (pMemOff[i].nItmTblId == pMemOff[j].nItmTblId)
			{
				return -1;
			}
			j++;
		}

		//2.前一个偏移地址要小于后一个
		if (pMemOff[i + 1].nItmTblId != FEND)
		{
			if (pMemOff[i].nAddr >= pMemOff[i + 1].nAddr)
			{
				return -1;
			}
		}
		
		i++;
	}
	return 0;
}


/********************************************************************************
* 函数名: MemInfoChk
* 输　入:
* 输　出:
* 返  回:
* 功  能: 核查
*********************************************************************************/
INT32S MemInfoChk(void)
{
	pMEM_INFO					pMemInfo1		= (pMEM_INFO)(&g_sTblMem[0]);
	pMEM_INFO					pMemInfo2		= NULL;
	INT32S						i				= 0;

   	while(1)
	{
		if (pMemInfo1->nTblId == MEM_OBJ_TBL_NUM)
		{
			break;
		}
		
		//1.ID必须按顺序排列
		pMemInfo2 = pMemInfo1;
		while(1)
		{
			pMemInfo2++;
			if (pMemInfo2->nTblId == MEM_OBJ_TBL_NUM)
			{
				break;
			}
			if (pMemInfo1->nTblId == pMemInfo2->nTblId)
			{
				return -1;
			}
		}
		//2.备份数量不能为0，不能大于介质最大数3
		/*
		if ((0 == pMemInfo1->nBackNum) || (pMemInfo1->nBackNum > MEM_MAX_BACK_NUM))
		{
			return -1;
		}
        */
		//3.行数不能为0
		if (0 == pMemInfo1->nRowMaxNum)
		{
			return -1;
		}
		//4.每张表各自偏移信息核查
		if (MemOffChk(pMemInfo1->pCol) < 0)
		{
			return -1;
		}
		//5.有文件名时，介质为flash的基地址为0
		for (i = 0; i < MEM_MAX_BACK_NUM/*pMemInfo1->nBackNum*/; i++)
		{
			if (MEM_MEDIUM_FLASH == pMemInfo1->sMemBaseInfoSub[i].nMedium)
			{
				if (NULL != pMemInfo1->sMemBaseInfoSub[i].pFileName)
				{
					if (0 != pMemInfo1->sMemBaseInfoSub[i].nAddr)
					{
						return -1;
					}
				}
			}
		}
		
		pMemInfo1++;
	}
	return 0;
}


