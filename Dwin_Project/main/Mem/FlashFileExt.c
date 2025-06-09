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
#include "FlashFile.h"
#include "FlashFileExt.h"
#include "ChkCrc.h"
#include "global.h"
#include "PowerOn.h"
#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>
#include  <stdlib.h>
#include  <stdarg.h>

/**************************************************************************************************
                                              内存实体
***************************************************************************************************/
// 最新文件索引 存铁电  暂时不用
static INT8U	g_nCurrFileIdx							= 0;

// 文件句柄存储基础偏移信息
INT32U			g_nFlashFileBaseAddr					= (512 * 1024);//FLASH_BLOCK;
// 文件句柄存储介质
INT8U			g_nFlashFileSaveType					= MEM_FLASH;
// 文件系统句柄信息列表
FLSAH_FILE 		g_sFlashFile[MAX_FILE_NUM]				= {0};


// flansh具体各块信息:0~(MAX_FILE_NUM - 1)   VALID_FILE_ID时无效  存铁电
INT8U			g_nBlkInfo[MAX_BLK_NUM + 2]			= {VALID_FILE_ID};


#define FLASH_FILE_SAVE_EXT(nIdx)		FLASH_FILE_SAVE(FLASH_FILE_OFFSET(nIdx) + g_nFlashFileBaseAddr, (INT8U*)&g_sFlashFile[nIdx].nFileName[0], sizeof(g_sFlashFile[nIdx]), g_nFlashFileSaveType)
#define FLASH_FILE_QUERY_EXT(nIdx)	FLASH_FILE_QUERY(FLASH_FILE_OFFSET(nIdx) + g_nFlashFileBaseAddr, (INT8U*)&g_sFlashFile[nIdx].nFileName[0], sizeof(g_sFlashFile[nIdx]), g_nFlashFileSaveType)

/**************************************************************************************************
                                          声明
***************************************************************************************************/
/********************************************************************************
* 函数名:FlashFileInfoGet
* 输　入:
*			INT8U	nFileIdx    VALID_FILE_ID时，全部读取
* 输　出:
* 返  回:
* 功  能: 
*********************************************************************************/
static INT32S FlashFileInfoGet(INT8U nFileIdx)
{
	INT32U nFileMaxNum 	= 0;
	INT32U i				= 0;
	
	nFileMaxNum = FlashFileMaxNumGet();
	if (nFileIdx >= nFileMaxNum)
	{
		for (i = 0; i < nFileMaxNum; i++)
		{
			FLASH_FILE_QUERY_EXT(i);
		}
	}
	else
	{
		FLASH_FILE_QUERY_EXT(nFileIdx);
	}

	return 0;
}
/********************************************************************************
* 函数名:FlashFileInfoInit
* 输　入:
* 输　出:
* 返  回:
* 功  能: 
*********************************************************************************/
//static INT32S FlashFileInfoInit(INT8U nFileIdx)
INT32S FlashFileInfoInit(INT8U nFileIdx)
{
	INT32U nFileMaxNum 	= 0;
	INT32U i				= 0;
	
	nFileMaxNum = FlashFileMaxNumGet();
	if (nFileIdx >= nFileMaxNum)
	{
        for (i = 0; i < nFileMaxNum; i++)
        {
            memset(&g_sFlashFile[i], 0, sizeof(g_sFlashFile[i]));
            g_sFlashFile[i].nBlkStartIdx = 0xFFFFFFFF;
        }
        FLASH_FILE_SAVE(g_nFlashFileBaseAddr, (INT8U*)&g_sFlashFile[0], sizeof(g_sFlashFile), MEM_FLASH);
	}
	else
	{
		memset(&g_sFlashFile[nFileIdx], 0, sizeof(g_sFlashFile[nFileIdx]));
        g_sFlashFile[nFileIdx].nBlkStartIdx = 0xFFFFFFFF;
		FLASH_FILE_SAVE_EXT(nFileIdx);
	}

	return 0;
}

/********************************************************************************
* 函数名:FlashFileBlkInfoGet
* 输　入:
* 输　出:
* 返  回:
* 功  能: 
*********************************************************************************/
static INT32S FlashFileBlkInfoGet(INT8U nType)
{
	INT32U nFileMaxNum 	= 0;
	
	nFileMaxNum = FlashFileMaxNumGet();

	if (1 != nType)
	{
		memset(&g_nBlkInfo[0], nFileMaxNum, sizeof(g_nBlkInfo));
		_API_FRAM_Read(FLASH_FILE_SAVE_FRAM_OFFSET, &g_nBlkInfo[0], sizeof(g_nBlkInfo));
		if (IsCrc16Good(&g_nBlkInfo[0], sizeof(g_nBlkInfo)))
		{
			return 0;
		}
	}
	memset(&g_nBlkInfo[0], nFileMaxNum, sizeof(g_nBlkInfo));//VALID_FILE_ID
	PutCrc16Proc(&g_nBlkInfo[0], sizeof(g_nBlkInfo));
	_API_FRAM_Write(FLASH_FILE_SAVE_FRAM_OFFSET, &g_nBlkInfo[0], sizeof(g_nBlkInfo));
	
	return -1;
}




/********************************************************************************
* 函数名:FlashFileBlkInfoSet
* 输　入:
*				INT32U				nBlkIdx		块下标
*				INT8U				nFileIdx	文件标识下标
* 输　出:
* 返  回:
* 功  能: 
*********************************************************************************/
INT32S FlashFileBlkInfoSet(INT32U nBlkIdx, INT8U nFileIdx)
{
	INT32U nBlkMaxNum 		= 0;
	INT32U nFileMaxNum 	= 0;

	nBlkMaxNum = FlashFileBlkMaxNumGet();
	nFileMaxNum = FlashFileMaxNumGet();
	if (nBlkIdx >= nBlkMaxNum)
	{
		return -1;
	}
	if (nFileIdx >= nFileMaxNum)
	{
		nFileIdx = nFileMaxNum;//VALID_FILE_ID
	}
	g_nBlkInfo[nBlkIdx] = nFileIdx;
	PutCrc16Proc(&g_nBlkInfo[0], sizeof(g_nBlkInfo));
	_API_FRAM_Write(FLASH_FILE_SAVE_FRAM_OFFSET, &g_nBlkInfo[0], sizeof(g_nBlkInfo));

	return 0;
}










/********************************************************************************
* 函数名:FlashFileMaxNumGet
* 输　入:	
* 输　出:
* 返  回:
* 功  能: 获取文件最大数目或者无效文件标识
*********************************************************************************/
INT32U FlashFileMaxNumGet(void)
{
	return MAX_FILE_NUM;
}

/********************************************************************************
* 函数名:FlashFileBlkMaxNumGet
* 输　入:	
* 输　出:
* 返  回:
* 功  能: 获取最大块数目
*********************************************************************************/
INT32U FlashFileBlkMaxNumGet(void)
{
	return MAX_BLK_NUM;
}

/********************************************************************************
* 函数名:FlashFileBlkSizeGet
* 输　入:		
* 输　出:
* 返  回:
* 功  能: 获取块大小
*********************************************************************************/
INT32U FlashFileBlkSizeGet(void)
{
	return MIN_BLK_SIZE;
}


/********************************************************************************
* 函数名:FlashFileCurrIdxGet
* 输　入:	
*				INT8U			nType   0时取ram
* 输　出:
* 返  回:
* 功  能: 获取当前最新的文件句柄下标
*********************************************************************************/
INT8U FlashFileCurrIdxGet(INT8U nType)
{
	if (nType)
	{
		//g_nCurrFileIdx = 
	}
	g_nCurrFileIdx = 0;
	return g_nCurrFileIdx;
}

/********************************************************************************
* 函数名:FlashFileCurrIdxSet
* 输　入:	
*				INT8U			nType   0时取ram
* 输　出:
* 返  回:
* 功  能: 刷新当前最新的文件句柄下标
*********************************************************************************/
INT32S FlashFileCurrIdxSet(INT8U nFileCurrIdx)
{
	g_nCurrFileIdx = nFileCurrIdx;

	return 0;
}











/********************************************************************************
* 函数名:FlashFileBlkChk
* 输　入:
* 输　出:
* 返  回:
* 功  能: 检验文件系统是否初始化启动
*********************************************************************************/
INT32S FlashFileBlkChk(INT8U nType)
{
    INT32U nFileMaxNum		= 0;
	INT32U nBlkMaxNum 		= 0;
	INT32U nMinBlkSize		= 0;
    INT32S nRet         	= 0;
	char** ppFlashFileTbl 	= NULL;

	// 定义文件系统基础偏移和大小
    nFileMaxNum = FlashFileMaxNumGet();
	nBlkMaxNum  = FlashFileBlkMaxNumGet();
	nMinBlkSize = FlashFileBlkSizeGet();

	FlashFileInit(FLASH_FLIE_BASE_ADDR, nMinBlkSize * nBlkMaxNum);
    
    nRet = FlashFileBlkInfoGet(nType);
	if (nRet < 0)
	{
		FlashFileInfoInit(nFileMaxNum);
		FlashFileCurrIdxSet(0);
        g_nAcCalPrmInitFlg = 1;//如果文件系统初始化了，就对交采以及校表参数进行初始化
	}

	FlashFileInfoGet(nFileMaxNum);
	ppFlashFileTbl = FlashFileTblGet();
	while(1)
	{
		if (NULL == *ppFlashFileTbl)
		{
			break;
		}
		nRet = FlashFileOpen(*ppFlashFileTbl);
        WatchDogClear(CH_DOG_OUT);
		ppFlashFileTbl++;
	}
	//FlashFileInfoGet(nFileMaxNum);
	
	return 0;
}



