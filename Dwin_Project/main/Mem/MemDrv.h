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
#ifndef _MEM_DRV_H_
#define _MEM_DRV_H_

#include "Os_cpu.h"

/**************************************************************************************************
                                          相关宏定义
***************************************************************************************************/
//存储介质
#define     MEM_MEDIUM_RAM					0					//RAM存储
#define     MEM_MEDIUM_FLASH				1					//FLASH存储
#define     MEM_MEDIUM_EEROM				2					//EEPROM存储
#define     MEM_MEDIUM_FRAM				    3					//FRAM存储
#define     MEM_MEDIUM_INVLD				0xFF				//无效存储
		


//介质数目
#define 	MEM_MAX_BACK_NUM				3					//备份数量

#define     ITEM_COL_END                    0xFFFF


//MEM_OFF_INFO表定义时使用 
#define MEM_OFF_INFO_DEFINE(nFn, pStr, nUnit)			{nFn, 1, STR_OFF(pStr, nUnit), 0, 0}
#define MEM_OFF_INFO_DEFINE_CHK(nFn, pStr, nUnit)		{nFn, 1, STR_OFF(pStr, nUnit), 0, 1}  

#define MEM_OFF_INFO_DEFINE_EXT(nFn, nNum, pStr, nUnit)			{nFn, nNum, STR_OFF(pStr, nUnit), 0, 0}

/**************************************************************************************************
                                          相关数据结构定义
***************************************************************************************************/
//存储偏移信息:具体数据库块使用
typedef struct _tagMEM_OFF_INFO
{
    INT32U      nItmTblId;              	//项标识号
    INT32U    	nItmTblNum;			        //此项数目
    INT32U      nAddr;						//地址
    INT32S      nLen;						//长度
    BOOLEAN		bChk;						//是否校验	
}MEM_OFF_INFO, *pMEM_OFF_INFO;




//存储基址信息子结构
typedef struct _tagMEM_BASE_INFO_SUB
{
    INT8U				nMedium;			//介质
	INT32U				nAddr;				//基址
	const char* 		pFileName;			//文件名
}MEM_BASE_INFO_SUB, *pMEM_BASE_INFO_SUB;
//存储基本信息
typedef struct _tagMEM_INFO
{
    INT32U				nTblId;				//数据库表号
	INT32U				nRowMaxNum;
	pMEM_OFF_INFO		pCol;
	
    MEM_BASE_INFO_SUB	sMemBaseInfoSub[MEM_MAX_BACK_NUM];
}MEM_INFO, *pMEM_INFO;




//存储定位信息
typedef struct _tagMEM_LOC_INFO
{
	INT8U			nMedium;
	char* 			pFileName;				//文件名
	
	INT32U			nAddr;
	INT32U			nLen;

}MEM_LOC_INFO, *pMEM_LOC_INFO;





/**************************************************************************************************
                                          对外接口函数
***************************************************************************************************/
/*
***************************************************************************
* 函数名:MemReadDbf
* 输入:
*       INT32U			nObjTblId								数据库表标识号
*       INT32U 			nObjId									某一表的第几个:不同的表，有的从0；有的从1；其他等等
*       INT32U 			nItmTblId								表的项标识号
*       INT32U          nItmId                                  某一项的第几个:从0开始
*       INT32U 			nOffset									基准偏移
*		INT32U 			nHistSize								一天所有点的所有测量点数据长度
*       INT16U 			nHistIdx								日块指示下标
*		INT32U			nPointSize								一个点的所有测量点曲线长度  曲线时使用
*       INT32U 			nCrvIdx									曲线点(从0起始)
*       void* 			pBuf									缓冲
*       INT32U 			nLen									缓冲长度
* 输出:	
*       INT32U* 		pRetLen									返回长度：(空指针时，不返回长度)
* 返回:RET应用标准返回值
* 调用:无
* 作者:wyf
* 日期:2010/04/13
* 版本:Version 1.0
* 功能描述:读接口
***************************************************************************
*/
INT8S MemReadDbf(INT32U nObjTblId, INT32U nObjId, INT32U nItmTblId, INT32U nItmId, INT32U nOffset, INT32U nHistSize, INT16U nHistIdx, INT32U nPointSize, INT32U nCrvIdx, void *pBuf, INT32U nLen, INT32U* pRetLen);

/*
***************************************************************************
* 函数名:MemWriteDbf
* 输入:
*       INT32U			nObjTblId								数据库表标识号
*       INT32U 			nObjId									某一表的第几个
*       INT32U 			nItmTblId								表的项标识号
*       INT32U          nItmId                                  某一项的第几个
*       INT32U 			nOffset									基准偏移
*		INT32U 			nHistSize								一天所有点的所有测量点数据长度
*       INT16U 			nHistIdx								日块指示下标
*		INT32U			nPointSize								一个点的所有测量点曲线长度  曲线时使用
*       INT32U 			nCrvIdx									曲线点(从0起始)
*       void* 			pBuf									缓冲
*       INT32U 			nLen									缓冲长度
* 输出:	
*       INT32U* 		pRetLen									返回长度：(空指针时，不返回长度)
* 返回:RET应用标准返回值
* 调用:无
* 作者:wyf
* 日期:2010/04/13
* 版本:Version 1.0
* 功能描述:写接口
***************************************************************************
*/
INT8S MemWriteDbf(INT32U nObjTblId, INT32U nObjId, INT32U nItmTblId, INT32U nItmId, INT32U nOffset, INT32U nHistSize, INT16U nHistIdx, INT32U nPointSize, INT32U nCrvIdx, const void *pBuf, INT32U nLen, INT32U* pRetLen);

/*
***************************************************************************
* 函数名:MemReadDbfExt
* 输入:
*       INT32U			nObjTblId								数据库表标识号
*       INT32U 			nObjId									某一表的第几个:不同的表，有的从0；有的从1；其他等等
*       INT32U 			nItmTblId								表的项标识号
*       void* 			pBuf									缓冲
*       INT32U 			nLen									缓冲长度
* 输出:	
*       INT32U* 		pRetLen									返回长度：(空指针时，不返回长度)
* 返回:RET应用标准返回值
* 调用:无
* 作者:wyf
* 日期:2010/04/13
* 版本:Version 1.0
* 功能描述:读接口
***************************************************************************
*/
INT32S 	MemReadDbfExt(INT32U nObjTblId, INT32U nObjId, INT32U nItmTblId, void *pBuf, INT32U nLen, INT32U* pRetLen);

/*
***************************************************************************
* 函数名:MemWriteDbfExt
* 输入:
*       INT32U			nObjTblId								数据库表标识号
*       INT32U 			nObjId									某一表的第几个
*       INT32U 			nItmTblId								表的项标识号
*       void* 			pBuf									缓冲
*       INT32U 			nLen									缓冲长度
* 输出:	
*       INT32U* 		pRetLen									返回长度：(空指针时，不返回长度)
* 返回:RET应用标准返回值
* 调用:无
* 作者:wyf
* 日期:2010/04/13
* 版本:Version 1.0
* 功能描述:写接口
***************************************************************************
*/
INT32S 	MemWriteDbfExt(INT32U nObjTblId, INT32U nObjId, INT32U nItmTblId, const void *pBuf, INT32U nLen, INT32U* pRetLen);

/*
***************************************************************************
* 函数名:MemReadHistDataExt
* 输入:
*       INT32U			nObjTblId								数据库表标识号
*       INT32U 			nObjId									某一表的第几个:不同的表，有的从0；有的从1；其他等等
*       INT32U 			nItmTblId								表的项标识号
*       INT32U          nItmId                                  某一项的第几个:从0开始
*		INT32U 			nHistSize								一天所有点的所有测量点数据长度
*       INT16U 			nHistIdx								日块指示下标
*		INT32U			nPointSize								一个点的所有测量点曲线长度  曲线时使用
*       INT32U 			nCrvIdx									曲线点(从0起始)
*       void* 			pBuf									缓冲
*       INT32U 			nLen									缓冲长度
* 输出:	
*       INT32U* 		pRetLen									返回长度：(空指针时，不返回长度)
* 返回:RET应用标准返回值
* 调用:无
* 作者:wyf
* 日期:2010/04/13
* 版本:Version 1.0
* 功能描述:读接口
***************************************************************************
*/
INT32S 	MemReadHistDataExt(INT32U nObjTblId, INT32U nObjId, INT32U nItmTblId, INT32U nHistSize, INT16U nHistIdx, INT32U nPointSize, INT32U nCrvIdx, void *pBuf, INT32U nLen, INT32U* pRetLen);

/*
***************************************************************************
* 函数名:MemWriteHistDataExt
* 输入:
*       INT32U			nObjTblId								数据库表标识号
*       INT32U 			nObjId									某一表的第几个
*       INT32U 			nItmTblId								表的项标识号
*		INT32U 			nHistSize								一天所有点的所有测量点数据长度
*       INT16U 			nHistIdx								日块指示下标
*		INT32U			nPointSize								一个点的所有测量点曲线长度  曲线时使用
*       INT32U 			nCrvIdx									曲线点(从0起始)
*       void* 			pBuf									缓冲
*       INT32U 			nLen									缓冲长度
* 输出:	
*       INT32U* 		pRetLen									返回长度：(空指针时，不返回长度)
* 返回:RET应用标准返回值
* 调用:无
* 作者:wyf
* 日期:2010/04/13
* 版本:Version 1.0
* 功能描述:写接口
***************************************************************************
*/
INT32S 	MemWriteHistDataExt(INT32U nObjTblId, INT32U nObjId, INT32U nItmTblId, INT32U nHistSize, INT16U nHistIdx, INT32U nPointSize, INT32U nCrvIdx, void *pBuf, INT32U nLen, INT32U* pRetLen);



#endif
