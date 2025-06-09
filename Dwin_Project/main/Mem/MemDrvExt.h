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
#ifndef _MEM_DRV_EXT_H_
#define _MEM_DRV_EXT_H_

#include "Os_cpu.h"

/**************************************************************************************************
                                          宏
***************************************************************************************************/
#if 0
//存储介质
#define     MEM_MEDIUM_RAM					0					//RAM存储
#define     MEM_MEDIUM_FLASH				1					//FLASH存储
#define     MEM_MEDIUM_EEROM				2					//EEPROM存储
#define     MEM_MEDIUM_FRAM					3					//FRAM存储
#define     MEM_MEDIUM_INVLD				0xFF				//无效存储
#endif
//存储介质编号
enum _tagMEM_TYPE
{
	MEM_RAM=0,				// RAM
	MEM_FLASH,				// Flash
	MEM_EPON,				// EP
    MEM_FRAM,				// ram

	
	MEM_NUM
    
};



// 读写flash
#define _API_FLASH_Read(buff, addr, len)				MemoryRead(addr, buff, len, MEM_FLASH)		
#define _API_FLASH_EraseWrite(buff, addr, len)		MemoryWrite(addr, buff, len, MEM_FLASH)

// 读写铁电
#define _API_FRAM_Read(addr, buff, len)				MemoryRead(addr, buff, len, MEM_FRAM)		
#define _API_FRAM_Write(addr, buff, len)				MemoryWrite(addr, buff, len, MEM_FRAM)



/**************************************************************************************************
                                          终端读写操作接口声明
***************************************************************************************************/
void 	MemDrvBufSemPend(void);
void 	MemDrvBufSemPost(void);

INT8U*  MemDrvBufGet(void);
INT32U  MemDrvBufSizeGet(void);
INT8U*  MemDrvBufUnitGet(void);
INT32U  MemDrvBufSizeUnitGet(void);

INT32S 	MemoryWrite(INT32U nAddr, const INT8U* pBuff, INT32U nLen , INT8U nType);
INT32S	MemoryRead(INT32U nAddr, INT8U* pBuff, INT32U nLen , INT8U nType);
void 	MemoryClear(INT32U nStart, INT32U nLen, INT8U nVal, INT8U nType);

void 	_API_FLASH_PowerOffOn(void);

INT32S 	MemFindHistIdxSub(INT16U nPreNum, void* pTime, INT16U *pIdx, INT8U nType);

INT32S 	MemFindRealHistIdx(void);
INT32S 	MemFindDayHistIdx(void* pTime);
INT32S 	MemFindMonHistIdx(void* pTime);
INT32S 	MemFindCrvHistIdx(void* pTime);
INT32S 	MemFindRDayHistIdx(void* pTime);




// 
INT8S   MemRead(const char* pFileName, INT8U nMedium, INT32U nAddr, INT32U nLen, void *pBuf);
INT8S   MemWrite(const char* pFileName, INT8U nMedium, INT32U nAddr, INT32U nLen, const void *pBuf);
INT32S	MemCopyFile(char* pDesFilename, INT32U nDesAddr, char* pSrcFilename, INT32U nSrcAddr, INT32U nLen , INT8U nType);
void 	MemClear(char* pFilename, INT32U nStart, INT32U nLen, INT8U nVal, INT8U nType);


/**************************************************************************************************
                                          对外接口函数
***************************************************************************************************/
INT32S	MemReadCommPrm(INT32U nFn, void *pBuf, INT32U nLen);
INT32S	MemWriteCommPrm(INT32U nFn, void *pBuf, INT32U nLen);
INT32S	MemReadMeterPrm(INT16U nPn, INT32U nFn, void *pBuf, INT32U nLen);
INT32S	MemWriteMeterPrm(INT16U nPn, INT32U nFn, void *pBuf, INT32U nLen);
INT32S  MemReadAcCalPrm(INT32U nFn, void *pBuf, INT32U nLen);
INT32S  MemWriteAcCalPrm(INT32U nFn, void *pBuf, INT32U nLen);
INT32S  MemReadAcPrm(INT32U nFn, void *pBuf, INT32U nLen);
INT32S  MemReadGrpPrm(INT32U nFn, INT32U nItmId, void *pBuf, INT32U nLen);
INT32S  MemWriteGrpPrm(INT32U nFn, INT32U nItmId, void *pBuf, INT32U nLen);
INT32S  MemWriteAcPrm(INT32U nFn, void *pBuf, INT32U nLen); 
INT32S  MemReadCtrlPrm(INT32U nFn, INT32U nItmId, void *pBuf, INT32U nLen);
INT32S  MemWriteCtrlPrm(INT32U nFn, INT32U nItmId, void *pBuf, INT32U nLen);
INT32S  MemReadRdPrm(INT32U nFn, INT32U nItmId, void *pBuf, INT32U nLen);
INT32S  MemWriteRdPrm(INT32U nFn, INT32U nItmId, void *pBuf, INT32U nLen);
INT32S 	MemReadOtherPrm(INT32U nFn, void *pBuf, INT32U nLen);
INT32S 	MemWriteOtherPrm(INT32U nFn, void *pBuf, INT32U nLen);

INT32S 	MemReadFlhXmpData(INT32U nFn, INT16U nBlkIdx, void *pBuf, INT32U nLen);
INT32S 	MemWriteFlhXmpData(INT32U nFn, INT16U nBlkIdx, void *pBuf, INT32U nLen);
INT32S  MemReadFramXmpData(INT32U nFn, INT16U nBlkIdx, void *pBuf, INT32U nLen);
INT32S  MemWriteFramXmpData(INT32U nFn, INT16U nBlkIdx, void *pBuf, INT32U nLen);
INT32S  MemReadRamXmpData(INT32U nFn, INT16U nBlkIdx, void *pBuf, INT32U nLen);
INT32S  MemWriteRamXmpData(INT32U nFn, INT16U nBlkIdx, void *pBuf, INT32U nLen);


INT32S MemReadRealData(INT16U nMp, INT32U nFn, void *pBuf, INT32U nLen);
INT32S MemWriteRealData(INT16U nMp, INT32U nFn, void *pBuf, INT32U nLen);
INT32S MemReadDayData(INT16U nMp, INT32U nFn, void* pTime, void *pBuf, INT32U nLen);
INT32S MemWriteDayData(INT16U nMp, INT32U nFn, INT16U nPre, void *pBuf, INT32U nLen);
INT32S MemReadMonData(INT16U nMp, INT32U nFn, void* pTime, void *pBuf, INT32U nLen);
INT32S MemWriteMonData(INT16U nMp, INT32U nFn, INT16U nPre, void *pBuf, INT32U nLen);

//交采
INT32S MemReadAcInstData(INT32U nFn, void *pBuf, INT32U nLen);
INT32S MemWriteAcInstData(INT32U nFn, void *pBuf, INT32U nLen);
INT32S MemReadAcRealData(INT32U nFn, void *pBuf, INT32U nLen);
INT32S MemWriteAcRealData(INT32U nFn, void *pBuf, INT32U nLen);
INT32S MemReadAcRealBackData(INT32U nFn, void *pBuf, INT32U nLen);
INT32S MemWriteAcRealBackData(INT32U nFn, void *pBuf, INT32U nLen);
#if 0
INT32S MemReadAcDayData(INT32U nFn, void *pBuf, INT32U nLen);
INT32S MemWriteAcDayData(INT32U nFn, void *pBuf, INT32U nLen);
INT32S MemReadAcRdDayData(INT32U nFn, void *pBuf, INT32U nLen);
INT32S MemWriteAcRdDayData(INT32U nFn, void *pBuf, INT32U nLen);
INT32S MemReadAcSSDJData(INT32U nFn, void *pBuf, INT32U nLen);
INT32S MemWriteAcSSDJData(INT32U nFn, void *pBuf, INT32U nLen);
INT32S MemReadAcDSDJData(INT32U nFn, void *pBuf, INT32U nLen);
INT32S MemWriteAcDSDJData(INT32U nFn, void *pBuf, INT32U nLen);
INT32S MemReadAcSQDJData(INT32U nFn, void *pBuf, INT32U nLen);
INT32S MemWriteAcSQDJData(INT32U nFn, void *pBuf, INT32U nLen);
INT32S MemReadAcSDDJData(INT32U nFn, void *pBuf, INT32U nLen);
INT32S MemWriteAcSDDJData(INT32U nFn, void *pBuf, INT32U nLen);
#endif


INT32S MemReadAcXBackData(INT32U nFn, void *pBuf, INT32U nLen);
INT32S MemWriteAcXBackData(INT32U nFn, void *pBuf, INT32U nLen);
INT32S MemReadAcXFlhBackData(INT32U nFn, void *pBuf, INT32U nLen);
INT32S MemWriteAcXFlhBackData(INT32U nFn, void *pBuf, INT32U nLen);
INT32S MemReadAcEvtData(INT32U nFn, INT32U nItmId, void *pBuf, INT32U nLen);
INT32S MemWriteAcEvtData(INT32U nFn, INT32U nItmId, void *pBuf, INT32U nLen);
INT32S MemReadAcHistData(INT32U nFn, INT32U nItmId, void *pBuf, INT32U nLen);
INT32S MemWriteAcHistData(INT32U nFn, INT32U nItmId, void *pBuf, INT32U nLen);
INT8S MemFindACDJHistDataIdx(INT8U Fn, INT16U nPreNum, INT16U *pIdx);

INT32S  MemWriteAcEvtProc(INT32U nFn, void *pBuf, INT32U nLen, INT8U nMode);
INT32S  MemReadAcEvtProc(INT32U nFn, INT16U nPreNum, void *pBuf, INT32U nLen);
INT8S MemFindACEvtDataIdx(INT8U Fn, INT16U nPreNum, INT16U *pIdx);
INT8S MemUpdateEventMemInfo(INT8U Fn, INT8U nMode);
INT8S MemDeleteACEvent(INT8U nFn);

INT8S MemUpdateHistDJDataMemInfo(INT8U Fn);

INT32S  MemWriteAcHistDataProc(INT32U nFn, INT16U nPreNum, void *pBuf, INT32U nLen, INT8U nUpdateFlg);
INT32S  MemReadAcHistDataProc(INT32U nFn, INT16U nPreNum, void *pBuf, INT32U nLen);
INT32S  MemReadAcHistDataProtProc(INT32U nFn, INT16U nPreNum, void *pBuf, INT32U nLen);
INT8S MemFindACHistDataIdx(INT8U Fn, INT16U nPreNum, INT16U *pIdx);
INT8S MemUpdateHistDataMemInfo(INT8U Fn);
INT8S MemDeleteACDayData(INT8U nFn);

INT32S  MemReadAcHistDJDataProc(INT32U nFn, INT16U nPreNum, void *pBuf, INT32U nLen);



#endif
