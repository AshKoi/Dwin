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
#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>
#include  <stdlib.h>
#include  <stdarg.h>
#include "FlashFile.h"
#include "FlashFileExt.h"
#include "Fun.h"

/**************************************************************************************************
                                              �ڴ�ʵ��
***************************************************************************************************/
// �ļ�ϵͳ��С
static INT32U		g_nFlashFileSize 			= 0;	
// �ļ�ϵͳƫ��
static INT32U		g_nFlashFileOffset		    = 0;



#ifndef GET_MIN
#define GET_MIN(a, b)			    	(((a) <= (b)) ? (a) : (b)) 						//��ȡ��Сֵ
#endif


#define FLASH_FILE_SAVE_EXT(nIdx)		FLASH_FILE_SAVE(FLASH_FILE_OFFSET(nIdx) + g_nFlashFileBaseAddr, (INT8U*)&g_sFlashFile[nIdx].nFileName[0], sizeof(g_sFlashFile[nIdx]), g_nFlashFileSaveType)


/**************************************************************************************************
                                          �洢������Ϣ
***************************************************************************************************/
/********************************************************************************
* ������:FlashFileHandGet
* �䡡��:
*			cosnt char*		pFileName 					�ļ��� Ϊ��ʱ����ȡ��ǰ����g_nCurrFileIdxΪ��׼���ҵ����¿����ļ����
* �䡡��:
* ��  ��:
* ��  ��: ��ȡ�ļ�������±�: 
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
* ������:FlashFileStartIdxGet
* �䡡��:
*				INT8U	nFileIdx
* �䡡��:
* ��  ��:
* ��  ��: 
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
* ������:FlashFileBlkFirstGet
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ��ȡ����Ϣ�ĵ�һ����Ч���±�
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
* ������:FlashFileBlkNextGet
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ��ȡ����Ϣ��xiaһ�����±�
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
* ������:FlashFileBlkNumGet
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ��ȡĳ���ļ��Ŀ���Ϣ�ĸ���
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
* ������:FlashFileBlkAdd
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ���ӿ���Ϣ
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
* ������:FlashFileAddrGet
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ��ȡ�����ַ
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
* ������:FlashFileBlkIdxClr
* �䡡��:
*			INT8U	nFileIdx		
* �䡡��:
* ��  ��:
* ��  ��: ��ʼ������ϢΪnFileIdx�Ŀ�
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
* ������:FlashFileInit
* �䡡��:
*			INT32U		nOffset 					�ļ�ϵͳ��ʼλ��
*			INT32U		nSize 						�ļ�ϵͳ��С 
* �䡡��:
* ��  ��: 0�ɹ���-1ʧ��
* ��  ��: �����ļ�ϵͳ
*********************************************************************************/
INT32S FlashFileInit(INT32U nOffset, INT32U nSize)
{
	g_nFlashFileOffset 	= nOffset;
	g_nFlashFileSize 	= nSize;

	return 0;
}




/********************************************************************************
* ������:FlashFileClose
* �䡡��:
*			char*		pFileName 					�ر��ļ��� 
* �䡡��:
* ��  ��: 0�ɹ���-1ʧ��    ��Ч�ļ��ر�ʱ����1
* ��  ��: �ر��ļ�
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
		// �ļ���Ч�˳�����ȥ�ر�
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
* ������:FlashFileOpen
* �䡡��:
*			char*		pFileName 					���ļ��� 
* �䡡��:
* ��  ��: 0�ɹ���-1ʧ��  �Ѿ��򿪵��ļ�����1
* ��  ��: �����ļ�
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
	//�ļ��Ѿ���ʱ����ȥ�ٴ�
	nFileIdx = FlashFileHandGet(pFileName);
	if ((nFileIdx >= 0) && (nFileIdx < nFileMaxNum))
	{
		return 1;
	}
	//��ȡ�µ��ļ�����±�
	nFileIdx = FlashFileHandGet(NULL);
	if (nFileIdx < -1)
	{
		return -1;
	}
	else if (nFileIdx >= nFileMaxNum)
	{
        /*�ļ���ʧ�ܣ������ļ���ʼ����������*/
        FlashFileInfoInit(nFileMaxNum);
		FlashFileCurrIdxSet(0);
        //��λ	
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
		return -3;// ��Ҫ�ر�һ���ļ�
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
* ������:FlashFileWrite
* �䡡��:
*			char*		pFileName 					�ļ��� 
*			INT32U 		nOffset						��ʼλ��
*			INT32S 		nLen						д�볤��
*			INT8U* 		pData						д�������
* �䡡��:
* ��  ��: 0�ɹ���-1ʧ��
* ��  ��: �ѳ���ΪnLen��pData������д���ļ���ΪpFileName��nOffsetλ��Ϊ��ʼ�Ĵ洢�ռ�
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
* ������:FlashFileRead
* �䡡��:
*			char*		pFileName 					���ļ��� 
* �䡡��:
* ��  ��:
* ��  ��: 
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

