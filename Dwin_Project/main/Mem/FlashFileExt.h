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
#ifndef _FLASH_FILE_EXT_H_
#define _FLASH_FILE_EXT_H_

#include "Os_cpu.h"
#include "MemDrvExt.h"



/**************************************************************************************************
                                          宏定义及结构体
***************************************************************************************************/
#ifndef FLASH_BLOCK
#define FLASH_BLOCK						4096											//扇区大小
#endif


// 文件系统定义的flash起始基地址:必须为MIN_BLK_SIZE的整数倍
#define FLASH_FLIE_BASE_ADDR				(520 * 1024)
// 最小分块大小
#define MIN_BLK_SIZE						(FLASH_BLOCK * 1)
// 文件系统分块最大数
#define MAX_BLK_NUM						(2 * 1024)
// 文件最大数目
#define MAX_FILE_NUM						0xFD
// 无效的文件句柄标识
#define VALID_FILE_ID						MAX_FILE_NUM


// flash块信息存储铁电偏移地址
//FRAM存储大小信息
#define MEM_FRAM_SIZE				    (8 * 1024)                  	//Fram大小
#define FLASH_FILE_SAVE_FRAM_OFFSET		(MEM_FRAM_SIZE - sizeof(g_nBlkInfo))
#define FLASH_FILE_OFFSET(nIdx)	 		((INT32U)(&g_sFlashFile[nIdx].nFileName[0]) - (INT32U)(&g_sFlashFile[0].nFileName[0]))



// flash读写
#define FLASH_WRITE(nAddr, pBuff, nLen)					MemoryWrite(nAddr, pBuff, nLen, MEM_FLASH)
#define FLASH_READ(nAddr, pBuff, nLen)					MemoryRead(nAddr, pBuff, nLen, MEM_FLASH)
#define FLASH_CLEAR(nStart, nLen, nVal)					MemoryClear(nStart, nLen, nVal, MEM_FLASH)


// 文件系统句柄存储
#define FLASH_FILE_SAVE(nAddr, pBuff, nLen, nType)		MemoryWrite(nAddr, pBuff, nLen, nType)
#define FLASH_FILE_QUERY(nAddr, pBuff, nLen, nType)		MemoryRead(nAddr, pBuff, nLen, nType)




// 文件句柄结构
typedef struct _tagFLASH_FILE
{
	char			nFileName[10];
	INT32U			nBlkStartIdx;
	
}FLSAH_FILE, *pFLSAH_FILE;


/**************************************************************************************************
                                          外部声明
***************************************************************************************************/
extern  FLSAH_FILE 		g_sFlashFile[MAX_FILE_NUM]	;
extern  INT8U			g_nBlkInfo[MAX_BLK_NUM + 2];

extern 	INT32U			g_nFlashFileBaseAddr;
extern  INT8U			g_nFlashFileSaveType;


/**************************************************************************************************
                                          相关函数说明
***************************************************************************************************/
INT32S FlashFileBlkInfoSet(INT32U nBlkIdx, INT8U nFileIdx);

INT32U FlashFileMaxNumGet(void);
INT32U FlashFileBlkMaxNumGet(void);
INT32U FlashFileBlkSizeGet(void);

INT8U  FlashFileCurrIdxGet(INT8U nType);
INT32S FlashFileCurrIdxSet(INT8U nFileCurrIdx);

INT32S FlashFileInfoInit(INT8U nFileIdx);

INT32S FlashFileBlkChk(INT8U nType);


#endif









