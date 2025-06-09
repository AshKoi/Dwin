/******************************************************************************************************
* 2013-2014 ���Ƽ����޹�˾
* �ļ���: 
* ������: ��һ��
* �ա���: 2013/09/21
* �޸���: 
* �ա���: 
* �衡��: 
* �桡��: V1.0
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
                                              �ڴ�ʵ��
***************************************************************************************************/
// �����ļ����� ������  ��ʱ����
static INT8U	g_nCurrFileIdx							= 0;

// �ļ�����洢����ƫ����Ϣ
INT32U			g_nFlashFileBaseAddr					= (512 * 1024);//FLASH_BLOCK;
// �ļ�����洢����
INT8U			g_nFlashFileSaveType					= MEM_FLASH;
// �ļ�ϵͳ�����Ϣ�б�
FLSAH_FILE 		g_sFlashFile[MAX_FILE_NUM]				= {0};


// flansh���������Ϣ:0~(MAX_FILE_NUM - 1)   VALID_FILE_IDʱ��Ч  ������
INT8U			g_nBlkInfo[MAX_BLK_NUM + 2]			= {VALID_FILE_ID};


#define FLASH_FILE_SAVE_EXT(nIdx)		FLASH_FILE_SAVE(FLASH_FILE_OFFSET(nIdx) + g_nFlashFileBaseAddr, (INT8U*)&g_sFlashFile[nIdx].nFileName[0], sizeof(g_sFlashFile[nIdx]), g_nFlashFileSaveType)
#define FLASH_FILE_QUERY_EXT(nIdx)	FLASH_FILE_QUERY(FLASH_FILE_OFFSET(nIdx) + g_nFlashFileBaseAddr, (INT8U*)&g_sFlashFile[nIdx].nFileName[0], sizeof(g_sFlashFile[nIdx]), g_nFlashFileSaveType)

/**************************************************************************************************
                                          ����
***************************************************************************************************/
/********************************************************************************
* ������:FlashFileInfoGet
* �䡡��:
*			INT8U	nFileIdx    VALID_FILE_IDʱ��ȫ����ȡ
* �䡡��:
* ��  ��:
* ��  ��: 
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
* ������:FlashFileInfoInit
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: 
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
* ������:FlashFileBlkInfoGet
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: 
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
* ������:FlashFileBlkInfoSet
* �䡡��:
*				INT32U				nBlkIdx		���±�
*				INT8U				nFileIdx	�ļ���ʶ�±�
* �䡡��:
* ��  ��:
* ��  ��: 
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
* ������:FlashFileMaxNumGet
* �䡡��:	
* �䡡��:
* ��  ��:
* ��  ��: ��ȡ�ļ������Ŀ������Ч�ļ���ʶ
*********************************************************************************/
INT32U FlashFileMaxNumGet(void)
{
	return MAX_FILE_NUM;
}

/********************************************************************************
* ������:FlashFileBlkMaxNumGet
* �䡡��:	
* �䡡��:
* ��  ��:
* ��  ��: ��ȡ������Ŀ
*********************************************************************************/
INT32U FlashFileBlkMaxNumGet(void)
{
	return MAX_BLK_NUM;
}

/********************************************************************************
* ������:FlashFileBlkSizeGet
* �䡡��:		
* �䡡��:
* ��  ��:
* ��  ��: ��ȡ���С
*********************************************************************************/
INT32U FlashFileBlkSizeGet(void)
{
	return MIN_BLK_SIZE;
}


/********************************************************************************
* ������:FlashFileCurrIdxGet
* �䡡��:	
*				INT8U			nType   0ʱȡram
* �䡡��:
* ��  ��:
* ��  ��: ��ȡ��ǰ���µ��ļ�����±�
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
* ������:FlashFileCurrIdxSet
* �䡡��:	
*				INT8U			nType   0ʱȡram
* �䡡��:
* ��  ��:
* ��  ��: ˢ�µ�ǰ���µ��ļ�����±�
*********************************************************************************/
INT32S FlashFileCurrIdxSet(INT8U nFileCurrIdx)
{
	g_nCurrFileIdx = nFileCurrIdx;

	return 0;
}











/********************************************************************************
* ������:FlashFileBlkChk
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: �����ļ�ϵͳ�Ƿ��ʼ������
*********************************************************************************/
INT32S FlashFileBlkChk(INT8U nType)
{
    INT32U nFileMaxNum		= 0;
	INT32U nBlkMaxNum 		= 0;
	INT32U nMinBlkSize		= 0;
    INT32S nRet         	= 0;
	char** ppFlashFileTbl 	= NULL;

	// �����ļ�ϵͳ����ƫ�ƺʹ�С
    nFileMaxNum = FlashFileMaxNumGet();
	nBlkMaxNum  = FlashFileBlkMaxNumGet();
	nMinBlkSize = FlashFileBlkSizeGet();

	FlashFileInit(FLASH_FLIE_BASE_ADDR, nMinBlkSize * nBlkMaxNum);
    
    nRet = FlashFileBlkInfoGet(nType);
	if (nRet < 0)
	{
		FlashFileInfoInit(nFileMaxNum);
		FlashFileCurrIdxSet(0);
        g_nAcCalPrmInitFlg = 1;//����ļ�ϵͳ��ʼ���ˣ��ͶԽ����Լ�У��������г�ʼ��
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



