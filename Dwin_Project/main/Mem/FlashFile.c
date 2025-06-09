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
#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>
#include  <stdlib.h>
#include  <stdarg.h>
#include "FlashFile.h"
#include "FlashFileExt.h"
#include "Fun.h"

/**************************************************************************************************
                                              内存实体
***************************************************************************************************/
// 文件系统大小
static INT32U		g_nFlashFileSize 			= 0;	
// 文件系统偏移
static INT32U		g_nFlashFileOffset		    = 0;



#ifndef GET_MIN
#define GET_MIN(a, b)			    	(((a) <= (b)) ? (a) : (b)) 						//获取最小值
#endif


#define FLASH_FILE_SAVE_EXT(nIdx)		FLASH_FILE_SAVE(FLASH_FILE_OFFSET(nIdx) + g_nFlashFileBaseAddr, (INT8U*)&g_sFlashFile[nIdx].nFileName[0], sizeof(g_sFlashFile[nIdx]), g_nFlashFileSaveType)


/**************************************************************************************************
                                          存储基本信息
***************************************************************************************************/
/********************************************************************************
* 函数名:FlashFileHandGet
* 输　入:
*			cosnt char*		pFileName 					文件名 为空时，获取当前最新g_nCurrFileIdx为基准查找的最新空闲文件句柄
* 输　出:
* 返  回:
* 功  能: 获取文件句柄的下标: 
*********************************************************************************/
static INT32S FlashFileHandGet(const char* pFileName)
{
	INT32S	i 				= 0;
	INT32S	nFileIdx 		= 0;
	INT8U	nFlg			= 0;
	INT8U	nFileCurrIdx 	= 0;
	INT32U 	nFileMaxNum 	= 0;
	
	if (NULL == pFileName)
	{
		nFlg = 1;
	}
	else
	{
		if (strlen(pFileName) >= 20)
		{
			return -1;
		}
	}
	nFileCurrIdx = FlashFileCurrIdxGet(0);
	nFileMaxNum = FlashFileMaxNumGet();
	for (i = 0; i < nFileMaxNum; i++)
	{
		nFileIdx = (nFileCurrIdx + i) % nFileMaxNum;
		if (0 == g_sFlashFile[nFileIdx].nFileName[0])
		{
			if (nFlg)
			{
				return nFileIdx;
			}

			continue;
		}
		if (0 == strcmp(pFileName, &g_sFlashFile[nFileIdx].nFileName[0]))
		{
			return nFileIdx;
		}
	}
	
	return nFileMaxNum;
}

/********************************************************************************
* 函数名:FlashFileStartIdxGet
* 输　入:
*				INT8U	nFileIdx
* 输　出:
* 返  回:
* 功  能: 
*********************************************************************************/
static INT32S FlashFileStartIdxGet(INT8U nFileIdx)
{
	INT32U 	nFileMaxNum 	= 0;
    INT32U 	nBlkMaxNum 	    = 0;
    INT32U  nBlkStartIdx    = 0;
	
	nFileMaxNum = FlashFileMaxNumGet();
	if (nFileIdx >= nFileMaxNum)
	{
		return -1;
	}
    
    nBlkMaxNum 	= FlashFileBlkMaxNumGet();
    nBlkStartIdx = g_sFlashFile[nFileIdx].nBlkStartIdx;
    if (nBlkStartIdx >= nBlkMaxNum)
    {
        return -1;
    }
    
	return nBlkStartIdx;
}

/********************************************************************************
* 函数名:FlashFileBlkFirstGet
* 输　入:
* 输　出:
* 返  回:
* 功  能: 获取块信息的第一个无效块下标
*********************************************************************************/
static INT32S FlashFileBlkFirstGet(void)
{	
	INT32U 	nFileMaxNum 	= 0;
    INT32U 	nBlkMaxNum 	    = 0;
    INT32S 	i 				= 0;
	
	nFileMaxNum = FlashFileMaxNumGet();
    nBlkMaxNum 	= FlashFileBlkMaxNumGet();
	for (i = 0; i < nBlkMaxNum; i++)
	{
		if (nFileMaxNum == g_nBlkInfo[i])//VALID_FILE_ID
		{
			return i;
		}
	}

	return -1;
}

/********************************************************************************
* 函数名:FlashFileBlkNextGet
* 输　入:
* 输　出:
* 返  回:
* 功  能: 获取块信息的xia一个块下标
*********************************************************************************/
static INT32S FlashFileBlkNextGet(INT8U nFileIdx, INT32U nBaseBlkIdx)
{	
	INT32U 	nFileMaxNum 	= 0;
    INT32U 	nBlkMaxNum 	    = 0;
    INT32S 	i 				= 0;
	
	nFileMaxNum = FlashFileMaxNumGet();
	if (nFileIdx >= nFileMaxNum)
	{
		return -1;
	}
    nBlkMaxNum 	= FlashFileBlkMaxNumGet();
    for (i = 0; i < nBlkMaxNum; i++)
	{
        nBaseBlkIdx = (nBaseBlkIdx + 1) % nBlkMaxNum;
		if (nFileIdx == g_nBlkInfo[nBaseBlkIdx])
		{
			return nBaseBlkIdx;
		}
	}

	return -1;
}


/********************************************************************************
* 函数名:FlashFileBlkNumGet
* 输　入:
* 输　出:
* 返  回:
* 功  能: 获取某个文件的块信息的个数
*********************************************************************************/
static INT32S FlashFileBlkNumGet(INT8U nFileIdx)
{
    INT32U 	nFileMaxNum 	= 0;
    INT32U 	nBlkMaxNum 	    = 0;
	INT32S 	i 				= 0;
	INT32S	nRet			= 0;

	nFileMaxNum = FlashFileMaxNumGet();
	if (nFileIdx >= nFileMaxNum)
	{
		return -1;
	}
    
	nBlkMaxNum 	= FlashFileBlkMaxNumGet();
	for (i = 0; i < nBlkMaxNum; i++)
	{
		if (nFileIdx == g_nBlkInfo[i])
		{
			nRet++;
		}
	}

	return nRet;
}

/********************************************************************************
* 函数名:FlashFileBlkAdd
* 输　入:
* 输　出:
* 返  回:
* 功  能: 增加块信息
*********************************************************************************/
static INT32S FlashFileBlkAdd(INT8U nFileIdx)
{
    INT32U 	nFileMaxNum 	= 0;
    INT32U 	nBlkMaxNum 	    = 0;
	INT32S 	i 				= 0;
	INT32S	nBlkIdx			= 0;
	INT32S	nStart  		= 0;

	nFileMaxNum = FlashFileMaxNumGet();
	if (nFileIdx >= nFileMaxNum)
	{
		return -1;
	}
	nStart = FlashFileStartIdxGet(nFileIdx);
	if (nStart < 0)
	{
		return -1;
	}
	nBlkMaxNum 	= FlashFileBlkMaxNumGet();
	for (i = 0; i < nBlkMaxNum; i++)
	{
		nBlkIdx = (nStart + i) % nBlkMaxNum;
		if (nFileMaxNum == g_nBlkInfo[nBlkIdx]) // VALID_FILE_ID
		{
			FlashFileBlkInfoSet(nBlkIdx, nFileIdx);
			return nBlkIdx;
		}
	}

	return -1;
}

/********************************************************************************
* 函数名:FlashFileAddrGet
* 输　入:
* 输　出:
* 返  回:
* 功  能: 获取物理地址
*********************************************************************************/
static INT32S FlashFileAddrGet(INT8U nFileIdx, INT32U nOffset, INT32U* pAddr)
{
    INT32U 	nFileMaxNum 	= 0;
    INT32U 	nBlkMaxNum 	    = 0;
    INT32S	nStart  		= 0;
    INT32U	nMinBlkSize	    = 0;
	INT32U	nBlkIdxBase		= 0;
	INT32U	nBlkOffset		= 0;
	INT32S 	i 				= 0;
	INT32S	nBlkIdx			= 0;
    
    nFileMaxNum = FlashFileMaxNumGet();
	if (nFileIdx >= nFileMaxNum)
	{
		return -1;
	}
    nStart = FlashFileStartIdxGet(nFileIdx);
	if (nStart < 0)
	{
		return -1;
	}
    nBlkIdx = nStart;
	
    nBlkMaxNum  = FlashFileBlkMaxNumGet();
	nMinBlkSize = FlashFileBlkSizeGet();
	nBlkIdxBase = nOffset / nMinBlkSize;
	nBlkOffset  = nOffset % nMinBlkSize;
    
    if (nBlkIdxBase >= nBlkMaxNum)
    {
        return -2;
    }
    else if (nBlkIdxBase > 0)
    {
        for (i = 0; i < nBlkIdxBase; i++)
        {
            nBlkIdx = FlashFileBlkNextGet(nFileIdx, nBlkIdx);
            if (nBlkIdx < 0)
            {
                return -1;
            }
        }
    }

	if (NULL != pAddr)
	{
		*pAddr = g_nFlashFileOffset + nBlkIdx * nMinBlkSize + nBlkOffset;
	}

	return 0;
}

#if 1
/********************************************************************************
* 函数名:FlashFileBlkIdxClr
* 输　入:
*			INT8U	nFileIdx		
* 输　出:
* 返  回:
* 功  能: 初始化块信息为nFileIdx的块
*********************************************************************************/
static INT32S FlashFileBlkIdxClr(INT8U nFileIdx)
{
    INT32U nFileMaxNum	= 0;
    INT32U nBlkMaxNum 	= 0;
    //INT32U nMinBlkSize	= 0;
	INT32S i 			= 0;

	nFileMaxNum = FlashFileMaxNumGet();
	if (nFileIdx >= nFileMaxNum)
	{
		return -1;
	}
    //nMinBlkSize = FlashFileBlkSizeGet();
	nBlkMaxNum = FlashFileBlkMaxNumGet();
	for (i = 0; i < nBlkMaxNum; i++)
	{
		if (nFileIdx == g_nBlkInfo[i])
		{
			FlashFileBlkInfoSet(i, nFileMaxNum);//VALID_FILE_ID
            //FLASH_CLEAR(g_nFlashFileOffset + i * nMinBlkSize, nMinBlkSize, 0xFF);
		}
	}

	return 0;
}
#endif





/********************************************************************************
* 函数名:FlashFileInit
* 输　入:
*			INT32U		nOffset 					文件系统起始位置
*			INT32U		nSize 						文件系统大小 
* 输　出:
* 返  回: 0成功；-1失败
* 功  能: 定义文件系统
*********************************************************************************/
INT32S FlashFileInit(INT32U nOffset, INT32U nSize)
{
	g_nFlashFileOffset 	= nOffset;
	g_nFlashFileSize 	= nSize;

	return 0;
}




/********************************************************************************
* 函数名:FlashFileClose
* 输　入:
*			char*		pFileName 					关闭文件名 
* 输　出:
* 返  回: 0成功；-1失败    无效文件关闭时返回1
* 功  能: 关闭文件
*********************************************************************************/
INT32S FlashFileClose(const char* pFileName)
{
	INT32S	nFileIdx 		= 0;
	INT8U	nFileCurrIdx	= 0;
	INT32U	nFileMaxNum	    = 0;
	
	if (NULL == pFileName)
	{
		return -1;
	}
	if (strlen(pFileName) >= 20)
	{
		return -1;
	}
	nFileMaxNum = FlashFileMaxNumGet();
	nFileIdx = FlashFileHandGet(pFileName);
	if ((nFileIdx >= 0) && (nFileIdx < nFileMaxNum))
	{

	}
	else
	{
		// 文件无效退出，不去关闭
		return 1;
	}

	nFileCurrIdx = FlashFileCurrIdxGet(0);
	FlashFileBlkIdxClr(nFileIdx);
	if (nFileIdx == nFileCurrIdx)
	{
		nFileCurrIdx = (nFileCurrIdx + nFileMaxNum - 1) % nFileMaxNum;
		FlashFileCurrIdxSet(nFileCurrIdx);
	}
    memset(&g_sFlashFile[nFileIdx].nFileName[0], 0, sizeof(g_sFlashFile[nFileIdx].nFileName));
    g_sFlashFile[nFileIdx].nBlkStartIdx = 0xFFFFFFFF;
    FLASH_FILE_SAVE_EXT(nFileIdx);
    
	return 0;
}

/********************************************************************************
* 函数名:FlashFileOpen
* 输　入:
*			char*		pFileName 					打开文件名 
* 输　出:
* 返  回: 0成功；-1失败  已经打开的文件返回1
* 功  能: 创建文件
*********************************************************************************/
INT32S FlashFileOpen(const char* pFileName)
{
    INT32U	nFileMaxNum	    = 0;
    INT32U	nMinBlkSize	    = 0;
	//INT8U	nFileCurrIdx	= 0;
	INT32S	nStart			= 0;
    INT32S	nFileIdx 		= 0;

	if (NULL == pFileName)
	{
		return -1;
	}
	if (strlen(pFileName) >= sizeof(g_sFlashFile[0].nFileName))
	{
		return -1;
	}
	nFileMaxNum = FlashFileMaxNumGet();
	//文件已经打开时，不去再打开
	nFileIdx = FlashFileHandGet(pFileName);
	if ((nFileIdx >= 0) && (nFileIdx < nFileMaxNum))
	{
		return 1;
	}
	//获取新的文件句柄下标
	nFileIdx = FlashFileHandGet(NULL);
	if (nFileIdx < -1)
	{
		return -1;
	}
	else if (nFileIdx >= nFileMaxNum)
	{
        /*文件打开失败，重新文件初始化，并重启*/
        FlashFileInfoInit(nFileMaxNum);
		FlashFileCurrIdxSet(0);
        //复位	
        ResetHardInit();
		//return -2; //
        /*
        nFileCurrIdx = FlashFileCurrIdxGet(0);
		nFileIdx = (nFileCurrIdx + 1) % nFileMaxNum;
		FlashFileBlkIdxClr(nFileIdx);
        */
	}

	nStart = FlashFileBlkFirstGet();
	if (nStart < 0)
	{
		return -3;// 需要关闭一个文件
	}
    nMinBlkSize = FlashFileBlkSizeGet();
    
	memset(&g_sFlashFile[nFileIdx].nFileName[0], 0, sizeof(g_sFlashFile[nFileIdx].nFileName));
	strcpy(&g_sFlashFile[nFileIdx].nFileName[0], pFileName);
    g_sFlashFile[nFileIdx].nBlkStartIdx = nStart;
    FLASH_FILE_SAVE_EXT(nFileIdx);
    
    FLASH_CLEAR(g_nFlashFileOffset + nStart * nMinBlkSize, nMinBlkSize, 0xFF);
    
	FlashFileCurrIdxSet(nFileIdx);
    FlashFileBlkInfoSet(nStart, nFileIdx);
	
	return 0;
}

/********************************************************************************
* 函数名:FlashFileWrite
* 输　入:
*			char*		pFileName 					文件名 
*			INT32U 		nOffset						起始位置
*			INT32S 		nLen						写入长度
*			INT8U* 		pData						写入的数据
* 输　出:
* 返  回: 0成功；-1失败
* 功  能: 把长度为nLen的pData的数据写入文件名为pFileName的nOffset位置为起始的存储空间
*********************************************************************************/
INT32S FlashFileWrite(const char* pFileName, INT32U nOffset, const INT8U* pData, INT32S nLen)
{
    INT32U	nFileMaxNum		= 0;
    INT32U	nMinBlkSize	    = 0;
    INT32U	nCurrBlkNum	    = 0;
	INT32U	nTmpBlkNum		= 0;
	INT32S	nFileIdx 		= 0;
    INT32S	nBlkAddIdx 		= 0;
	INT32U	nAddr			= 0;	
	INT32U	nTmpLen			= 0;
	INT32U	nOnceLen		= 0;
	INT32S	i				= 0;
	
	if (NULL == pFileName)
	{
		return -1;
	}
	if (nLen < 0)
	{
		return -1;
	}
	if ((nOffset + nLen) >= g_nFlashFileSize)
	{
		return -1;
	}
	nFileMaxNum = FlashFileMaxNumGet();
	nFileIdx = FlashFileHandGet(pFileName);
	if ((nFileIdx >= 0) && (nFileIdx < nFileMaxNum))
	{
		
	}
	else
	{
		return -1;//FlashFileOpen(pFileName);
	}

	nMinBlkSize = FlashFileBlkSizeGet();
	nCurrBlkNum = FlashFileBlkNumGet(nFileIdx);
	nTmpBlkNum  = ((nOffset + nLen)) / nMinBlkSize + 1;
	if (nCurrBlkNum < nTmpBlkNum)
	{
		for (i = nCurrBlkNum; i < nTmpBlkNum; i++)
		{
			nBlkAddIdx = FlashFileBlkAdd(nFileIdx);
            if (nBlkAddIdx < 0)
            {
                return -1;
            }
            FLASH_CLEAR(g_nFlashFileOffset + nBlkAddIdx * nMinBlkSize, nMinBlkSize, 0xFF);
		}
	}

	while(nTmpLen < nLen)
	{
		FlashFileAddrGet(nFileIdx, nOffset, &nAddr);
		nOnceLen = GET_MIN(((nOffset / nMinBlkSize + 1) * nMinBlkSize - nOffset), nLen - nTmpLen);
		FLASH_WRITE(nAddr, &pData[nTmpLen], nOnceLen);
		nTmpLen += nOnceLen;
        nOffset += nOnceLen;
	}
	
	return 0;
}

/********************************************************************************
* 函数名:FlashFileRead
* 输　入:
*			char*		pFileName 					打开文件名 
* 输　出:
* 返  回:
* 功  能: 
*********************************************************************************/
INT32S FlashFileRead(const char* pFileName, INT32U nOffset, INT8U* pData, INT32S nLen)
{
    INT32U	nFileMaxNum		= 0;
    INT32U	nMinBlkSize	    = 0;
    INT32U	nCurrBlkNum	    = 0;
	INT32U	nTmpBlkNum		= 0;
	INT32S	nFileIdx 		= 0;
	INT32U	nAddr			= 0;	
	INT32U	nTmpLen			= 0;
	INT32U	nOnceLen		= 0;
	
	if (NULL == pFileName)
	{
		return -1;
	}
	if (nLen < 0)
	{
		return -1;
	}
	if ((nOffset + nLen) >= g_nFlashFileSize)
	{
		return -1;
	}
	nFileMaxNum = FlashFileMaxNumGet();
	nFileIdx = FlashFileHandGet(pFileName);
	if ((nFileIdx >= 0) && (nFileIdx < nFileMaxNum))
	{
		
	}
	else
	{
		return -1;
	}

	nMinBlkSize = FlashFileBlkSizeGet();
	nCurrBlkNum = FlashFileBlkNumGet(nFileIdx);
	#if 0
	nTmpBlkNum = ((nOffset + nLen)) / nMinBlkSize + 1;
	if (nCurrBlkNum < nTmpBlkNum)
	{
		for (i = nCurrBlkNum; i < nTmpBlkNum; i++)
		{
			FlashFileBlkAdd(nFileIdx);
		}
	}
	#else
	nTmpBlkNum = ((nOffset + nLen)) / nMinBlkSize + 1;
	if (nCurrBlkNum < nTmpBlkNum)
	{
		return -1;
	}
	#endif

	while(nTmpLen < nLen)
	{
		FlashFileAddrGet(nFileIdx, nOffset, &nAddr);
		nOnceLen = GET_MIN(((nOffset / nMinBlkSize + 1) * nMinBlkSize - nOffset), nLen - nTmpLen);
		FLASH_READ(nAddr, &pData[nTmpLen], nOnceLen);
		nTmpLen += nOnceLen;
        nOffset += nOnceLen;
	}
	
	return 0;
}

