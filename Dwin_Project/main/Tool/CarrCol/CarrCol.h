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
#ifndef _CARRCOL_H
#define _CARRCOL_H

#include "Os_cpu.h"

extern INT8U	g_nCarrModType;
extern INT8U   g_nCarrNodeRef;
extern INT8U   g_nCarrInitFlg;
extern INT8U   g_nCarrZbSelFlg;

/**************************************************************************************************
											 相关宏定义及结构枚举定义
***************************************************************************************************/

enum _tagCARR_MOD_TYPE
{
	CARR_MOD_XC = 1			,				   	// 晓程
	CARR_MOD_DR 			,				    // 东软 
	CARR_MOD_DX				,				   	// 鼎信   
	CARR_MOD_LHW			,				    // 力合微 
	CARR_MOD_RSK			,				    // 瑞斯康
	CARR_MOD_XN				,					// 讯能
	CARR_MOD_GY				,					// 光一晓程
    CARR_MOD_YXD			,				    //友讯达微功率无线
    CARR_MOD_0FDM           ,                   //宽带载波
	CARR_MOD_NUM			
};

#define GW3762_DOWN_FRM					0			// 下行
#define GW3762_UP_FRM					1			// 上行

#define GW3762_START_PRM				1			// 启动
#define GW3762_SON_PRM					0			// 从动

#define GW3762_CARR						1			// 载波
#define GW3762_WLAN					    0			// 小无线

#define GW3762_MAIN_SELF				0			// 主节点
#define GW3762_METER_NODE				1			// congjiedian



#define TYPE_CARR                       0
#define TYPE_ZB                         1


/**************************************************************************************************
                                           结构定义
***************************************************************************************************/
typedef struct _tag_CARR_MODE_INFO{
	INT8U				 nModeType;
    INT8U                nReadType;						//抄表模式（0x55表示集中器主动，0xaa表示模块主动）
    INT8U                nSetNodeRouter;				//是否下发节点到路由模块（0x55下发 0xaa不下发）
    INT8U                nSetColNode;					//下发采集器地址或电表地址（0x55为采集器地址 0xaa为电表地址）
	INT8U                nTranAfn;						//抄表使用的AFN
	INT32U				 nBaud;							//波特率
}CARR_MODE_INFO, *pCARR_MODE_INFO;



/**************************************************************************************************
                                             变量声明
***************************************************************************************************/







/**************************************************************************************************
                                             函数声明
***************************************************************************************************/
INT32S  CarrPointRd(INT8U* pData, INT32S nLen);
INT32S CarrColInit(void);

INT32S WrilessInit(void);
INT32S CarrRouteAdd(INT8U* pAddr, INT8U nProt);
INT32S CarrRouteInit(void);


#endif

