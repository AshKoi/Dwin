/**************************************************************************************************
* Copyright (c) 2010-2011 光一科技股份有限公司研发中心
* 文件名:
* 创建人: 研发中心
* 日　期: 2010/08/18
* 修改人:
* 日　期:
* 描　述:
* 版　本: Version 1.0
***************************************************************************************************/
#include "AppCfg.h"
#include "DataItem.h"
#include "Base.h"
#include "MemDrvExt.h"
#include "Meter645.h"
#include "MemTbl.h"

/**************************************************************************************************
                                          相关全局变量定义
***************************************************************************************************/
#if 0
#endif
//普通用户表抄收数据项配置
const INT16U g_nNormUserMtCfg[] =
{
    //1.实时
    RD_ZXYGJH,			    //(901F)正向有功电能示值集合
    RD_FXYGJH,


	RD_DBRQ,            	//(C010)电表日期
    RD_DBSJ,            	//(C011)电表时间

    RD_DBYXZTZ,			    //(C020)电表运行状态字
    RD_DBBCJL,				//(B21F)电表编程记录

    //2.日
    DD_ZXYGJH,              //日正向有功电能示值集合

    //3.月
    MD_ZXYGJH,              //(941f)上月正向有功电量示值


    INVLD_DATA_ITEM    
};
//重点用户表抄收数据项配置
const INT16U g_nImpUserMtCfg[] =
{
    //1.实时
    RD_ZXYGJH,			    //(901F)正向有功电能示值集合
    RD_FXYGJH,

    RD_SYJH,				//(E133)失压次数
    RD_SLJH,				//(E122)失流次数
    RD_DLFXJH,				//(E142)电流反向次数

	RD_DBRQ,            	//(C010)电表日期
    RD_DBSJ,            	//(C011)电表时间

    RD_DBYXZTZ,			    //(C020)电表运行状态字
    RD_DBBCJL,				//(B21F)电表编程记录

    //2.日
    DD_ZXYGJH,              //日正向有功电能示值集合

    //3.月
    MD_ZXYGJH,              //(941f)上月正向有功电量示值


    INVLD_DATA_ITEM    
};



#if 0
#endif
//485表抄收数据项配置
const INT16U g_nRs485MtCfg[] =
{
    //1.实时
    RD_ZXYGJH,			    //(901F)正向有功电能示值集合
    RD_FXYGJH,
	RD_DBRQ,            	//(C010)电表日期
    RD_DBSJ,            	//(C011)电表时间

    //2.日
    DD_ZXYGJH,              //日正向有功电能示值集合

    //3.月
    MD_ZXYGJH,              //(941f)上月正向有功电量示值


    INVLD_DATA_ITEM    
};

//carrier表抄收数据项配置
const INT16U g_nCarrierMtCfg[] =
{
    //1.实时
    RD_ZXYGJH,			    //(901F)正向有功电能示值集合
    
	RD_DBRQ,            	//(C010)电表日期
    RD_DBSJ,            	//(C011)电表时间

    //2.日
    DD_ZXYGJH,              //日正向有功电能示值集合

    //3.月
    MD_ZXYGJH,              //(941f)上月正向有功电量示值


    INVLD_DATA_ITEM    
};

#if 0
#endif
//实时数据抄收数据项配置
const INT16U g_nRealMtCfg[] =
{
    RD_ZXYGJH,			    //(901F)正向有功电能示值集合
    RD_FXYGJH,

    INVLD_DATA_ITEM

};

//日数据抄收数据项配置
const INT16U g_nDayMtCfg[] =
{
    DD_ZXYGJH,              //日正向有功电能示值集合
    DD_FXYGJH,
    
    RD_DBRQ,            	//(C010)电表日期
    RD_DBSJ,            	//(C011)电表时间

    INVLD_DATA_ITEM

};


//月数据抄收数据项配置
const INT16U g_nMonMtCfg[] =
{
    MD_ZXYGJH,              //(941f)上月正向有功电量示值
    MD_FXYGJH,
    
	RD_DBRQ,            	//(C010)电表日期
    RD_DBSJ,            	//(C011)电表时间

    INVLD_DATA_ITEM

};

#if 0
//曲线数据抄收数据项配置
const INT16U g_nCrvMtCfg[] =
{
    RD_ZXYGJH,			    //(901F)正向有功电能示值集合

    INVLD_DATA_ITEM

};
#endif

//抄表日数据抄收数据项配置
const INT16U g_nRDayMtCfg[] =
{
    MD_ZXYGJH,              //(941f)上月正向有功电量示值

    INVLD_DATA_ITEM

};

/**************************************************************************************************
                                         AC事件增补数据项
***************************************************************************************************/
//失压表
const INT8U g_nACSY[53] =
{
	6,//（上1次）A相失压发生时刻                   01
	4,//（上1次）A相失压发生时刻正向有功总电能     02
	4,//（上1次）A相失压发生时刻反向有功总电能     03
	4,//（上1次）A相失压发生时刻组合无功1总电能    04
	4,//（上1次）A相失压发生时刻组合无功2总电能    05
	4,//（上1次）A相失压发生时刻A相正向有功电能    06
	4,//（上1次）A相失压发生时刻A相反向有功电能    07
	4,//（上1次）A相失压发生时刻A相组合无功1电能   08
	4,//（上1次）A相失压发生时刻A相组合无功2电能   09
	2,//（上1次）A相失压发生时刻A相电压            0A
	3,//（上1次）A相失压发生时刻A相电流            0B
	3,//（上1次）A相失压发生时刻A相有功功率        0C
	3,//（上1次）A相失压发生时刻A相无功功率        0D
	2,//（上1次）A相失压发生时刻A相功率因数        0E
	4,//（上1次）A相失压发生时刻B相正向有功电能    0F
	4,//（上1次）A相失压发生时刻B相反向有功电能    10
	4,//（上1次）A相失压发生时刻B相组合无功1电能   11
	4,//（上1次）A相失压发生时刻B相组合无功2电能   12
	2,//（上1次）A相失压发生时刻B相电压            13
	3,//（上1次）A相失压发生时刻B相电流            14
	3,//（上1次）A相失压发生时刻B相有功功率        15
	3,//（上1次）A相失压发生时刻B相无功功率        16
	2,//（上1次）A相失压发生时刻B相功率因数        17
	4,//（上1次）A相失压发生时刻C相正向有功电能    18
	4,//（上1次）A相失压发生时刻C相反向有功电能    19
	4,//（上1次）A相失压发生时刻C相组合无功1电能   1A
	4,//（上1次）A相失压发生时刻C相组合无功2电能   1B
	2,//（上1次）A相失压发生时刻C相电压            1C
	3,//（上1次）A相失压发生时刻C相电流            1D
	3,//（上1次）A相失压发生时刻C相有功功率        1E
	3,//（上1次）A相失压发生时刻C相无功功率        1F
	2,//（上1次）A相失压发生时刻C相功率因数        20
	4,//（上1次）A相失压期间总安时数               21
	4,//（上1次）A相失压期间A相安时数              22
	4,//（上1次）A相失压期间B相安时数              23
	4,//（上1次）A相失压期间C相安时数              24
	6,//（上1次）A相失压结束时刻                   25
	4,//（上1次）A相失压结束时刻正向有功总电能     26
	4,//（上1次）A相失压结束时刻反向有功总电能     27
	4,//（上1次）A相失压结束时刻组合无功1总电能    28
	4,//（上1次）A相失压结束时刻组合无功2总电能    29
	4,//（上1次）A相失压结束时刻A相正向有功电能    2A
	4,//（上1次）A相失压结束时刻A相反向有功电能    2B
	4,//（上1次）A相失压结束时刻A相组合无功1电能   2C
	4,//（上1次）A相失压结束时刻A相组合无功2电能   2D
	4,//（上1次）A相失压结束时刻B相正向有功电能    2E
	4,//（上1次）A相失压结束时刻B相反向有功电能    2F
	4,//（上1次）A相失压结束时刻B相组合无功1电能   30
	4,//（上1次）A相失压结束时刻B相组合无功2电能   31
	4,//（上1次）A相失压结束时刻C相正向有功电能    32
	4,//（上1次）A相失压结束时刻C相反向有功电能    33
	4,//（上1次）A相失压结束时刻C相组合无功1电能   34
	4 //（上1次）A相失压结束时刻C相组合无功2电能   35
};

//失流表
const INT8U g_nACSL[49] =
{
	6,//（上1次）A相失流发生时刻                     01
	4,//（上1次）A相失流发生时刻正向有功总电能       02
	4,//（上1次）A相失流发生时刻反向有功总电能       03
	4,//（上1次）A相失流发生时刻组合无功1总电能      04
	4,//（上1次）A相失流发生时刻组合无功2总电能      05
	4,//（上1次）A相失流发生时刻A相正向有功电能      06
	4,//（上1次）A相失流发生时刻A相反向有功电能      07
	4,//（上1次）A相失流发生时刻A相组合无功1电能     08
	4,//（上1次）A相失流发生时刻A相组合无功2电能     09
	2,//（上1次）A相失流发生时刻A相电压              0A
	3,//（上1次）A相失流发生时刻A相电流              0B
	3,//（上1次）A相失流发生时刻A相有功功率          0C
	3,//（上1次）A相失流发生时刻A相无功功率          0D
	2,//（上1次）A相失流发生时刻A相功率因数          0E
	4,//（上1次）A相失流发生时刻B相正向有功电能      0F
	4,//（上1次）A相失流发生时刻B相反向有功电能      10
	4,//（上1次）A相失流发生时刻B相组合无功1电能     11
	4,//（上1次）A相失流发生时刻B相组合无功2电能     12
	2,//（上1次）A相失流发生时刻B相电压              13
	3,//（上1次）A相失流发生时刻B相电流              14
	3,//（上1次）A相失流发生时刻B相有功功率          15
	3,//（上1次）A相失流发生时刻B相无功功率          16
	2,//（上1次）A相失流发生时刻B相功率因数          17
	4,//（上1次）A相失流发生时刻C相正向有功电能      18
	4,//（上1次）A相失流发生时刻C相反向有功电能      19
	4,//（上1次）A相失流发生时刻C相组合无功1电能     1A
	4,//（上1次）A相失流发生时刻C相组合无功2电能     1B
	2,//（上1次）A相失流发生时刻C相电压              1C
	3,//（上1次）A相失流发生时刻C相电流              1D
	3,//（上1次）A相失流发生时刻C相有功功率          1E
	3,//（上1次）A相失流发生时刻C相无功功率          1F
	2,//（上1次）A相失流发生时刻C相功率因数          20
	6,//（上1次）A相失流结束时刻                     21
	4,//（上1次）A相失流结束时刻正向有功总电能       22
	4,//（上1次）A相失流结束时刻反向有功总电能       23
	4,//（上1次）A相失流结束时刻组合无功1总电能      24
	4,//（上1次）A相失流结束时刻组合无功2总电能      25
	4,//（上1次）A相失流结束时刻A相正向有功电能      26
	4,//（上1次）A相失流结束时刻A相反向有功电能      27
	4,//（上1次）A相失流结束时刻A相组合无功1电能     28
	4,//（上1次）A相失流结束时刻A相组合无功2电能     29
	4,//（上1次）A相失流结束时刻B相正向有功电能      2A
	4,//（上1次）A相失流结束时刻B相反向有功电能      2B
	4,//（上1次）A相失流结束时刻B相组合无功1电能     2C
	4,//（上1次）A相失流结束时刻B相组合无功2电能     2D
	4,//（上1次）A相失流结束时刻C相正向有功电能      2E
	4,//（上1次）A相失流结束时刻C相反向有功电能      2F
	4,//（上1次）A相失流结束时刻C相组合无功1电能     30
	4,//（上1次）A相失流结束时刻C相组合无功2电能     31
};

//断相表--同失压表
const INT8U g_nACDX[53] =
{
	6,//（上1次）A相失压发生时刻                   01
	4,//（上1次）A相失压发生时刻正向有功总电能     02
	4,//（上1次）A相失压发生时刻反向有功总电能     03
	4,//（上1次）A相失压发生时刻组合无功1总电能    04
	4,//（上1次）A相失压发生时刻组合无功2总电能    05
	4,//（上1次）A相失压发生时刻A相正向有功电能    06
	4,//（上1次）A相失压发生时刻A相反向有功电能    07
	4,//（上1次）A相失压发生时刻A相组合无功1电能   08
	4,//（上1次）A相失压发生时刻A相组合无功2电能   09
	2,//（上1次）A相失压发生时刻A相电压            0A
	3,//（上1次）A相失压发生时刻A相电流            0B
	3,//（上1次）A相失压发生时刻A相有功功率        0C
	3,//（上1次）A相失压发生时刻A相无功功率        0D
	2,//（上1次）A相失压发生时刻A相功率因数        0E
	4,//（上1次）A相失压发生时刻B相正向有功电能    0F
	4,//（上1次）A相失压发生时刻B相反向有功电能    10
	4,//（上1次）A相失压发生时刻B相组合无功1电能   11
	4,//（上1次）A相失压发生时刻B相组合无功2电能   12
	2,//（上1次）A相失压发生时刻B相电压            13
	3,//（上1次）A相失压发生时刻B相电流            14
	3,//（上1次）A相失压发生时刻B相有功功率        15
	3,//（上1次）A相失压发生时刻B相无功功率        16
	2,//（上1次）A相失压发生时刻B相功率因数        17
	4,//（上1次）A相失压发生时刻C相正向有功电能    18
	4,//（上1次）A相失压发生时刻C相反向有功电能    19
	4,//（上1次）A相失压发生时刻C相组合无功1电能   1A
	4,//（上1次）A相失压发生时刻C相组合无功2电能   1B
	2,//（上1次）A相失压发生时刻C相电压            1C
	3,//（上1次）A相失压发生时刻C相电流            1D
	3,//（上1次）A相失压发生时刻C相有功功率        1E
	3,//（上1次）A相失压发生时刻C相无功功率        1F
	2,//（上1次）A相失压发生时刻C相功率因数        20
	4,//（上1次）A相失压期间总安时数               21
	4,//（上1次）A相失压期间A相安时数              22
	4,//（上1次）A相失压期间B相安时数              23
	4,//（上1次）A相失压期间C相安时数              24
	6,//（上1次）A相失压结束时刻                   25
	4,//（上1次）A相失压结束时刻正向有功总电能     26
	4,//（上1次）A相失压结束时刻反向有功总电能     27
	4,//（上1次）A相失压结束时刻组合无功1总电能    28
	4,//（上1次）A相失压结束时刻组合无功2总电能    29
	4,//（上1次）A相失压结束时刻A相正向有功电能    2A
	4,//（上1次）A相失压结束时刻A相反向有功电能    2B
	4,//（上1次）A相失压结束时刻A相组合无功1电能   2C
	4,//（上1次）A相失压结束时刻A相组合无功2电能   2D
	4,//（上1次）A相失压结束时刻B相正向有功电能    2E
	4,//（上1次）A相失压结束时刻B相反向有功电能    2F
	4,//（上1次）A相失压结束时刻B相组合无功1电能   30
	4,//（上1次）A相失压结束时刻B相组合无功2电能   31
	4,//（上1次）A相失压结束时刻C相正向有功电能    32
	4,//（上1次）A相失压结束时刻C相反向有功电能    33
	4,//（上1次）A相失压结束时刻C相组合无功1电能   34
	4 //（上1次）A相失压结束时刻C相组合无功2电能   35
};

//电压逆相序表
const INT8U g_nACDYNX[34] =
{
	6,// （上1次）电压逆相序发生时刻                    01
	4,// （上1次）电压逆相序发生时刻正向有功总电能      02
	4,// （上1次）电压逆相序发生时刻反向有功总电能      03
	4,// （上1次）电压逆相序发生时刻组合无功1总电能     04
	4,// （上1次）电压逆相序发生时刻组合无功2总电能     05
	4,// （上1次）电压逆相序发生时刻A相正向有功电能     06
	4,// （上1次）电压逆相序发生时刻A相反向有功电能     07
	4,// （上1次）电压逆相序发生时刻A相组合无功1电能    08
	4,// （上1次）电压逆相序发生时刻A相组合无功2电能    09
	4,// （上1次）电压逆相序发生时刻B相正向有功电能     0A
	4,// （上1次）电压逆相序发生时刻B相反向有功电能     0B
	4,// （上1次）电压逆相序发生时刻B相组合无功1电能    0C
	4,// （上1次）电压逆相序发生时刻B相组合无功2电能    0D
	4,// （上1次）电压逆相序发生时刻C相正向有功电能     0E
	4,// （上1次）电压逆相序发生时刻C相反向有功电能     0F
	4,// （上1次）电压逆相序发生时刻C相组合无功1电能    10
	4,// （上1次）电压逆相序发生时刻C相组合无功2电能    11
	6,// （上1次）电压逆相序结束时刻                    12
	4,// （上1次）电压逆相序结束时刻正向有功总电能      13
	4,// （上1次）电压逆相序结束时刻反向有功总电能      14
	4,// （上1次）电压逆相序结束时刻组合无功1总电能     15
	4,// （上1次）电压逆相序结束时刻组合无功2总电能     16
	4,// （上1次）电压逆相序结束时刻A相正向有功电能     17
	4,// （上1次）电压逆相序结束时刻A相反向有功电能     18
	4,// （上1次）电压逆相序结束时刻A相组合无功1电能    19
	4,// （上1次）电压逆相序结束时刻A相组合无功2电能    1A
	4,// （上1次）电压逆相序结束时刻B相正向有功电能     1B
	4,// （上1次）电压逆相序结束时刻B相反向有功电能     1C
	4,// （上1次）电压逆相序结束时刻B相组合无功1电能    1D
	4,// （上1次）电压逆相序结束时刻B相组合无功2电能    1E
	4,// （上1次）电压逆相序结束时刻C相正向有功电能     1F
	4,// （上1次）电压逆相序结束时刻C相反向有功电能     20
	4,// （上1次）电压逆相序结束时刻C相组合无功1电能    21
	4 // （上1次）电压逆相序结束时刻C相组合无功2电能    22
};



/**************************************************************************************************
                                           
***************************************************************************************************/
static INT16U  	g_nMeterMp[MAX_METER_NUM]						= {0};
static INT16U  	g_nImMeterSeq[MAX_IM_METER_NUM]				= {0};

static INT8U  	g_nMeterRdFlags[MAX_METER_NUM]					= {0};
static INT8U	g_nMeterBlock[(MAX_METER_NUM - 1) / 8 + 1]	= {0};				//0,尝试抄读；1块抄



/**************************************************************************************************
                                         函数
***************************************************************************************************/
#if 0

#endif

/********************************************************************************
* 函数名:GetItemMapRdTdPtr
* 输　入:
*		INT8U					nRdTd 		业务数据类型：是按照位值  RD_REAL、RD_DAY
* 输  出:
* 返  回:错误编码
* 功  能:
*********************************************************************************/
static INT8S GetItemMapRdTdPtr(INT8U nRdTd, INT8U* pItemMap)
{
	INT32S			i 		= 0;
	INT16U*			pMtCfg	= NULL;
	
	if ((NULL == pItemMap))
	{
		return -1;
	}

	if (nRdTd == RD_REAL)
	{
		pMtCfg = (INT16U*)g_nRealMtCfg;
		while(1)
		{
			if (pMtCfg[i] == INVLD_DATA_ITEM)
			{
				return 0;
			}
			SetBufBit(pItemMap, DATA_ITEM_MAP_LEN, pMtCfg[i]);
			
			i++;
		}
	}
	else if (nRdTd == RD_DAY)
	{
		pMtCfg = (INT16U*)g_nDayMtCfg;
		while(1)
		{
			if (pMtCfg[i] == INVLD_DATA_ITEM)
			{
				return 0;
			}
			SetBufBit(pItemMap, DATA_ITEM_MAP_LEN, pMtCfg[i]);
			
			i++;
		}
	}
	else if (nRdTd == RD_MON)
	{
		pMtCfg = (INT16U*)g_nMonMtCfg;
		while(1)
		{
			if (pMtCfg[i] == INVLD_DATA_ITEM)
			{
				return 0;
			}
			SetBufBit(pItemMap, DATA_ITEM_MAP_LEN, pMtCfg[i]);
			
			i++;
		}
	}
	#if 0
	else if (nRdTd == RD_CRV)
	{
		pMtCfg = (INT16U*)g_nCrvMtCfg;
		while(1)
		{
			if (pMtCfg[i] == INVLD_DATA_ITEM)
			{
				return 0;
			}
			SetBufBit(pItemMap, DATA_ITEM_MAP_LEN, pMtCfg[i]);
			
			i++;
		}
	}
	#endif
	else if (nRdTd == RD_RDAY)
	{
		pMtCfg = (INT16U*)g_nRDayMtCfg;
		while(1)
		{
			if (pMtCfg[i] == INVLD_DATA_ITEM)
			{
				return 0;
			}
			SetBufBit(pItemMap, DATA_ITEM_MAP_LEN, pMtCfg[i]);
			
			i++;
		}
	}
	else
	{
		return -1;
	}
}


/********************************************************************************
* 函数名:GetItemMapRdFlagsPtr
* 输　入:
*		INT8U					RdFlags 		业务数据类型：是按照位?
* 输  出:
* 返  回:错误编码
* 功  能:
*********************************************************************************/
static INT8S GetItemMapRdFlagsPtr(INT8U nRdFlags, INT8U* pItemMap)
{
	INT32S	i				= 0;

	if ((NULL == pItemMap))
	{
		return -1;
	}

	memset(pItemMap,0 ,DATA_ITEM_MAP_LEN);
	for (i = 0; i < RD_TD_NUM; i++)
	{
		if (TestBufBit(&nRdFlags, 1, i))
		{
			GetItemMapRdTdPtr(i, pItemMap);
		}
	}

	return 0;
}


/********************************************************************************
* 函数名:GetItemMapImUserPtr
* 输　入:
*		INT8U					nImUser 		
* 输  出:
* 返  回:错误编码
* 功  能:
*********************************************************************************/
static INT8S GetItemMapImUserPtr(INT8U nImUser, INT8U* pItemMap)
{
	INT32S			i 		= 0;
	INT16U*			pMtCfg	= NULL;
	
	if ((NULL == pItemMap))
	{
		return -1;
	}

	memset(pItemMap,0 ,DATA_ITEM_MAP_LEN);
	if (nImUser)
	{
		pMtCfg = (INT16U*)g_nImpUserMtCfg;
		while(1)
		{
			if (pMtCfg[i] == INVLD_DATA_ITEM)
			{
				return 0;
			}
			SetBufBit(pItemMap, DATA_ITEM_MAP_LEN, pMtCfg[i]);
			
			i++;
		}
	}
	else
	{
		pMtCfg = (INT16U*)g_nNormUserMtCfg;
		while(1)
		{
			if (pMtCfg[i] == INVLD_DATA_ITEM)
			{
				return 0;
			}
			SetBufBit(pItemMap, DATA_ITEM_MAP_LEN, pMtCfg[i]);
			
			i++;
		}
	}
}



/********************************************************************************
* 函数名:GetItemMapRs485Ptr
* 输　入:
*		INT8U					nRs485 		业务数据类型：
* 输  出:
* 返  回:错误编码
* 功  能:
*********************************************************************************/
static INT8S GetItemMapRs485Ptr(INT8U nRs485, INT8U* pItemMap)
{
	INT32S			i 		= 0;
	INT16U*			pMtCfg	= NULL;
	
	if ((NULL == pItemMap))
	{
		return -1;
	}

	memset(pItemMap,0 ,DATA_ITEM_MAP_LEN);
	if (nRs485)
	{
		pMtCfg = (INT16U*)g_nRs485MtCfg;
		while(1)
		{
			if (pMtCfg[i] == INVLD_DATA_ITEM)
			{
				return 0;
			}
			SetBufBit(pItemMap, DATA_ITEM_MAP_LEN, pMtCfg[i]);
			
			i++;
		}
	}
	else
	{
		pMtCfg = (INT16U*)g_nCarrierMtCfg;
		while(1)
		{
			if (pMtCfg[i] == INVLD_DATA_ITEM)
			{
				return 0;
			}
			SetBufBit(pItemMap, DATA_ITEM_MAP_LEN, pMtCfg[i]);
			
			i++;
		}
	}
}


/********************************************************************************
* 函数名:GetTmpDataItem
* 输　入:
*       pDATA_ITEM 				pDataItemS       	当前位图
*		pDATA_ITEM 				pDataItemD	
* 输  出:
* 返  回:错误编码
* 功  能:不是时-1;是时0  pDataItem ----> pDataItemTmp
*********************************************************************************/
static INT32S GetTmpDataItem(const pDATA_ITEM pDataItemS, pDATA_ITEM pDataItemD)
{
	pDATA_ITEM	pDataItem1 = pDataItemS;
	
	if (NULL == pDataItemS)
	{
		return -1;
	}
	if (NULL == pDataItemD)
	{
		return -1;
	}
	if (COL_END_ITEM == pDataItemS->nStdItem)
	{
		return -1;
	}
	memmove(pDataItemD, pDataItemS, sizeof(DATA_ITEM));
	pDataItem1 += pDataItemS->nItemNum;
	pDataItemD->nItemLen = pDataItem1->nOffset - pDataItemS->nOffset;

	return 0;
}



#if 0
#endif
/********************************************************************************
* 函数名:GetItemMap
* 输　入:
*       INT8U                  	nRdFlags         	业务数据类型：按照值来区分（不是按照位）
*		INT8U					nIsImUser			
*		INT8U					nIsRs485
* 输  出:pItems      数据项列表
* 返  回:错误编码
* 功  能:获得测量点抄读的标准数据项列表
*********************************************************************************/
INT32S GetItemMap(INT8U nRdFlags, INT8U nIsImUser, INT8U nIsRs485, INT8U* pItemMap)
{
    INT16U          i           					= 0;
    INT8U           bsItemMap[DATA_ITEM_MAP_LEN] 	= {0};
	INT16U			nSize							= sizeof(bsItemMap);

    if( (NULL == pItemMap))
    {
        return -1;
    }

    memset(pItemMap, 0, nSize);

	GetItemMapRdFlagsPtr(nRdFlags, &bsItemMap[0]);
	memcpy(pItemMap, &bsItemMap[0], nSize);
	
	GetItemMapImUserPtr(nIsImUser, &bsItemMap[0]);
	for (i = 0; i < nSize; i++)
	{
		pItemMap[i] &= bsItemMap[i];
	}
	
	GetItemMapRs485Ptr(nIsRs485, &bsItemMap[0]);
	for (i = 0; i < nSize; i++)
	{
		pItemMap[i] &= bsItemMap[i];
	}

	return 0;
}


/********************************************************************************
* 函数名:IsCurrRdItem
* 输　入:
*       INT8U* 					pItemMap        	当前位图
*		INT32U					nStdItem		
* 输  出:
* 返  回:错误编码
* 功  能:不是时-1;是时0
*********************************************************************************/
INT32S IsCurrRdItem(INT8U* pItemMap, INT32U nStdItem)
{
	if (TestBufBit(pItemMap, DATA_ITEM_MAP_LEN, nStdItem))
	{
		return 0;
	}
	else
	{
		return -1;	
	}
}


#if 0
#endif
/********************************************************************************
* 函数名:Gb645ToColData
* 输　入:

* 输  出:
* 返  回:错误编码
* 功  能:
*********************************************************************************/
INT32S Gb645ToColData(INT8U nProt, INT8U* pGb645Frm, void* pDataItem, void* pColData)
{
	pDATA_ITEM		pDataItemTmp 	= (pDATA_ITEM)pDataItem;
	DATA_ITEM		sDataItem;	
	INT8U*			pColDataTmp 	= (INT8U*)pColData;
	INT8U			nDiLen			= 0;
    INT8U           i               = 0;
	
	if (NULL == pGb645Frm)
	{
		return -1;
	}
	if (NULL == pDataItem)
	{
		return -1;
	}
	if (NULL == pColData)
	{
		return -1;
	}
	if (GB_97_PROT == nProt)
	{
		nDiLen = 2;
	}
	else
	{
		nDiLen = 4;
	}
	if (GetTmpDataItem(pDataItemTmp, &sDataItem) < 0)
	{
		return -1;
	}
    
    for (i = 0; i < sDataItem.nItemLen; i++)
    {
        pGb645Frm[METER_645_BASE_LEN + nDiLen + i] -= 0x33;
    }

	memmove(&pColDataTmp[sDataItem.nOffset], &pGb645Frm[METER_645_BASE_LEN + nDiLen], sDataItem.nItemLen);
	
	return sDataItem.nItemLen;
}

/********************************************************************************
* 函数名:MeterMpInit
* 输　入:
* 输　出:
* 返  回:0
* 功  能: 参数初始化时调用
*********************************************************************************/
INT32S MeterMpInit(void)
{
	memset(&g_nMeterMp[0], 0, sizeof(g_nMeterMp));
	memset(&g_nImMeterSeq[0], 0, sizeof(g_nImMeterSeq));
	memset(&g_nMeterRdFlags[0], 0, sizeof(g_nMeterRdFlags));
	memset(&g_nMeterBlock[0], 0, sizeof(g_nMeterBlock));
	
	return 0;
}

/********************************************************************************
* 函数名:MeterMpMap
* 输　入:
*			INT8U			nType				1时，调用SetRdMeterFlags函数置实时抄读
* 输　出:
* 返  回:0
* 功  能: 上电时调用、更改F10参数调用
*********************************************************************************/
INT32S MeterMpMap(INT8U nType)
{
	INT32S				i					= 0;
	INT16U				nSeq				= 0;
	INT8U				nBuf[GET_MAX(sizeof(METER_INFO), MAX_IM_METER_NUM * 2 + 1)] = {0};
	INT32U				nSize				= sizeof(nBuf);
	pMETER_INFO			pMeterInfo			= (pMETER_INFO)(&nBuf[0]);
	INT8U*				pData 				= &nBuf[0];
	
	MeterMpInit();
	for (i = 0; i < MAX_METER_NUM; i++)
	{
		nSeq = i + 1;
		if (GetMeterInfo(nSeq, pMeterInfo) < 0)
		{
			continue;
		}
		g_nMeterMp[i] = pMeterInfo->nMp;	

		if (nType)
		{
			SetRdMeterFlags(nSeq, RD_REAL);
		}
	}
    //RdMeterSemProc();

	memset(pData, 0, nSize);
	MemReadOtherPrm(F35, pData, nSize);
	memset(&g_nImMeterSeq[0], 0, sizeof(g_nImMeterSeq));
	for (i = 0; i < GET_MIN(MAX_IM_METER_NUM, pData[0]); i++)
	{
		g_nImMeterSeq[i] = Buf2INT16U(&pData[1 + i * 2]);
	}

	return 0;
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 全部测量点
*********************************************************************************/
INT8S SetRdMeterFlagsAllMp(INT8U nRdTd)
{
	INT16U				i 				= 0;
    INT16U              nSeq            = 0;
	METER_INFO			sMeterInfo;
	pMETER_INFO			pMeterInfo		= &sMeterInfo;

	for (i = 0; i < MAX_METER_NUM; i++)
	{
		nSeq = i + 1;
		if (GetMeterInfo(nSeq, pMeterInfo) < 0)
		{
			continue;
		}
		SetRdMeterFlags(nSeq, nRdTd);
	}

	return RET_ERR_OK;
}

/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 全部测量点
*********************************************************************************/
INT8S ClrRdMeterFlagsAllMp(INT8U nRdTd)
{
	INT16U				i 				= 0;
    INT16U              nSeq            = 0;
	METER_INFO			sMeterInfo;
	pMETER_INFO			pMeterInfo		= &sMeterInfo;

	for (i = 0; i < MAX_METER_NUM; i++)
	{
		nSeq = i + 1;
		if (GetMeterInfo(nSeq, pMeterInfo) < 0)
		{
			continue;
		}
		ClrRdMeterFlags(nSeq, nRdTd);
	}

	return RET_ERR_OK;
}



/********************************************************************************
* 函数名:SetRdMeterFlags
* 输　入:
*				INT16U 				nSeq			序号
*				INT8U				nRdTd			RD_REAL/RD_DAY
* 输　出:
* 返  回:0
* 功  能: 
*********************************************************************************/
INT32S SetRdMeterFlags(INT16U nSeq, INT8U nRdTd)
{
	INT8U nRdFlags = 0;

	if (!ChkRange(nSeq, 1, MAX_METER_NUM))
	{
		return -1;
	}
	
	nRdFlags = g_nMeterRdFlags[nSeq - 1];
	SetBufBit(&nRdFlags, 1, nRdTd);
	g_nMeterRdFlags[nSeq - 1] = nRdFlags;

	return 0;
}

/********************************************************************************
* 函数名:ClrRdMeterFlags
* 输　入:
*				INT16U 				nSeq			序号
*				INT8U				nRdTd			RD_REAL/RD_DAY
* 输　出:
* 返  回:0
* 功  能: 
*********************************************************************************/
INT32S ClrRdMeterFlags(INT16U nSeq, INT8U nRdTd)
{
	INT8U nRdFlags = 0;

	if (!ChkRange(nSeq, 1, MAX_METER_NUM))
	{
		return -1;
	}
	
	nRdFlags = g_nMeterRdFlags[nSeq - 1];
	ClrBufBit(&nRdFlags, 1, nRdTd);
	g_nMeterRdFlags[nSeq - 1] = nRdFlags;

	return 0;
}

/********************************************************************************
* 函数名:GetRdMeterFlags
* 输　入:
*				INT16U 				nSeq			序号
* 输　出:
* 返  回:0
* 功  能: 
*********************************************************************************/
INT32S GetRdMeterFlags(INT16U nSeq, INT8U* pRdFlags)
{
	if (!ChkRange(nSeq, 1, MAX_METER_NUM))
	{
		return -1;
	}
	if (NULL != pRdFlags)
	{
		*pRdFlags = g_nMeterRdFlags[nSeq - 1];
	}
	
	return 0;
}


/********************************************************************************
* 函数名:SetBlockFlg
* 输　入:
*				INT16U 				nSeq			序号
* 输　出:
* 返  回:0
* 功  能: 
*********************************************************************************/
INT32S SetBlockFlg(INT16U nSeq, INT8U nBlk)
{
	//INT8U nRdFlags = 0;

	if (!ChkRange(nSeq, 1, MAX_METER_NUM))
	{
		return -1;
	}
	if (nBlk)
	{
		SetBufBit(&g_nMeterBlock[0], sizeof(g_nMeterBlock), nSeq - 1);
	}
	else
	{
		ClrBufBit(&g_nMeterBlock[0], sizeof(g_nMeterBlock), nSeq - 1);
	}
	//g_nMeterBlock[nSeq - 1] = nBlk;

	return 0;
}

/********************************************************************************
* 函数名:GetBlockFlg
* 输　入:
*				INT16U 				nSeq			序号
* 输　出:
* 返  回:0,尝试；1，块抄
* 功  能: 
*********************************************************************************/
INT32S GetBlockFlg(INT16U nSeq)
{
	INT8U nRdFlags = 0;

	if (!ChkRange(nSeq, 1, MAX_METER_NUM))
	{
		return 0;
	}
	//nRdFlags = g_nMeterBlock[nSeq - 1];
	//if (nRdFlags > 1)
	//{
	//	nRdFlags = 0;
	//}
	if (TestBufBit(&g_nMeterBlock[0], sizeof(g_nMeterBlock), nSeq - 1))
	{
		nRdFlags = 1;
	}
	else
	{
		nRdFlags = 0;
	}

	return nRdFlags;
}

/********************************************************************************
* 函数名:GetSeqFromAddr
* 输　入:
*				INT16U 				nMp			测量点
* 输　出:
* 返  回:序号   0,无效
* 功  能: 
*********************************************************************************/
INT32S GetSeqFromAddr(INT8U* pAddr)
{
	INT16U          i = 0;
    INT16U			nSeq	= 0;
    MEM_METER_PRM   sMeterPrm;
    pMEM_METER_PRM  pMeterPrm = &sMeterPrm;

	for (i = 0; i < MAX_METER_NUM; i++)
    {
        nSeq = i + 1;
        MemReadMeterPrm(nSeq, F10, pMeterPrm, sizeof(MEM_METER_PRM));
        if(0 == memcmp(pAddr, &pMeterPrm->nAddr[0], 6))
        {
            return nSeq;
        }
    }
	return -1;
}

/********************************************************************************
* 函数名:GetMpFromSeq
* 输　入:
*				INT16U 				nMp			测量点
* 输　出:
* 返  回:序号   0,无效
* 功  能: 
*********************************************************************************/
INT16U GetMpFromSeq(INT16U nSeq)
{
	INT16U			nMp	= 0;

	if (!ChkRange(nSeq, 1, MAX_METER_NUM))
	{
		return 0;
	}
	// 读取Ram  上电时形成映射
	nMp = g_nMeterMp[nSeq - 1];
	if (!ChkRange(nMp, 1, MAX_MP))
	{
		return 0;
	}
	
	return nMp;
}


/********************************************************************************
* 函数名:GetSeqFromMp
* 输　入:
*				INT16U 				nMp			测量点
* 输　出:
* 返  回:序号   0,无效
* 功  能: 
*********************************************************************************/
INT16U GetSeqFromMp(INT16U nMp)
{
	INT32S			i 		= 0;
	INT16U			nMpTmp	= 0;
	INT16U			nSeq	= 0;
	
	if (!ChkRange(nMp, 1, MAX_MP))
	{
		return 0;
	}

	for (i = 0; i < MAX_METER_NUM; i++)
	{
		nSeq = i + 1;
		nMpTmp = GetMpFromSeq(nSeq);
		if (nMpTmp == nMp)
		{
			return nSeq;
		}
	}

	return 0;
}

/********************************************************************************
* 函数名:GetSeqFromMp
* 输　入:
*				INT16U 				nMp			测量点
* 输　出:
* 返  回:序号   0,无效
* 功  能: 
*********************************************************************************/
INT16U GetImSeqFromSeq(INT16U nSeq)
{
	INT32S			i 			= 0;
	INT16U			nSeqTmp		= 0;
	
	if (!ChkRange(nSeq, 1, MAX_METER_NUM))
	{
		return 0;
	}

	for (i = 0; i < MAX_IM_METER_NUM; i++)
	{
		nSeqTmp = g_nImMeterSeq[i];
		if (nSeqTmp == nSeq)
		{
			return i + 1;
		}
	}

	return 0;
}


/********************************************************************************
* 函数名:GetMeterInfo
* 输　入:
*				INT16U 				nSeq			序号
* 输　出:
*				void* 				pInfo			pMETER_INFO
* 返  回:0
* 功  能: 
*********************************************************************************/
INT32S GetMeterInfo(INT16U nSeq, void* pInfo)
{
	pMETER_INFO		pMeterInfo = (pMETER_INFO)(pInfo);
	INT8U 			nRdFlags 	= 0;
	MEM_METER_PRM	sMeterPrm;
	pMEM_METER_PRM	pMeterPrm	= &sMeterPrm;
	INT16U			nTmp		= 0;
    INT32U          nBaud       = 0;
	
	memset(pMeterInfo, 0, sizeof(METER_INFO));

	if (!ChkRange(nSeq, 1, MAX_METER_NUM))
	{
		return -1;
	}
	memset(pMeterPrm, 0, sizeof(MEM_METER_PRM));
	// 读表计参数
	MemReadMeterPrm(nSeq, F10, pMeterPrm, sizeof(MEM_METER_PRM));
	nTmp = Buf2INT16U(&pMeterPrm->nSeq[0]);
	if (nTmp != nSeq)
	{
		return -1;
	}
	nTmp = Buf2INT16U(&pMeterPrm->nMp[0]);
	if (!ChkRange(nTmp, 1, MAX_MP))
	{
		return -1;
	}
	pMeterInfo->nMp  = nTmp;
	pMeterInfo->nSeq = nSeq;
	memcpy(&pMeterInfo->nMeterAddr[0], &pMeterPrm->nAddr[0], sizeof(pMeterInfo->nMeterAddr));
	//pMeterInfo->nProt	= 0;
	//pMeterInfo->nPort	= 2;
    pMeterInfo->nProt = pMeterPrm->nProt;
    pMeterInfo->nPort = pMeterPrm->nCom;

	// 读铁电
	GetRdMeterFlags(nSeq, &nRdFlags);
	pMeterInfo->nRdFlgs = nRdFlags;
    
    nBaud = Buf2Long(&pMeterPrm->nBaud[0], 4);

	return nBaud;
}


/********************************************************************************
* 函数名:GetPulsePrm
* 输　入:
*				INT16U 				nSeq			序号
* 输　出:
*				
* 返  回:
* 功  能: 
*********************************************************************************/
INT32S GetPulsePrm(INT16U nSeq, pMEM_PULSE_PRM pPulsePrm)
{
    INT16U			nTmp		= 0;
    
    if (!ChkRange(nSeq, 1, MAX_METER_NUM))
	{
		return -1;
	}
    
    memset(pPulsePrm, 0, sizeof(MEM_PULSE_PRM));
	//读脉冲参数
	MemReadGrpPrm(F11, nSeq, pPulsePrm, sizeof(MEM_PULSE_PRM));
    if (!pPulsePrm->bVld)
    {
        return -1;
    }
	//nTmp = Buf2INT16U(&pPulsePrm->nPulseMp[0]);
    nTmp = pPulsePrm->nPulseMp;
	if (!ChkRange(nTmp, 1, MAX_MP))
	{
		return -1;
	}
    return 0;
}


