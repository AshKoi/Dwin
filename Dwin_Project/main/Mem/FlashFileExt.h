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
#ifndef _FLASH_FILE_EXT_H_
#define _FLASH_FILE_EXT_H_

#include "Os_cpu.h"
#include "MemDrvExt.h"



/**************************************************************************************************
                                          �궨�弰�ṹ��
***************************************************************************************************/
#ifndef FLASH_BLOCK
#define FLASH_BLOCK						4096											//������С
#endif


// �ļ�ϵͳ�����flash��ʼ����ַ:����ΪMIN_BLK_SIZE��������
#define FLASH_FLIE_BASE_ADDR				(520 * 1024)
// ��С�ֿ��С
#define MIN_BLK_SIZE						(FLASH_BLOCK * 1)
// �ļ�ϵͳ�ֿ������
#define MAX_BLK_NUM						(2 * 1024)
// �ļ������Ŀ
#define MAX_FILE_NUM						0xFD
// ��Ч���ļ������ʶ
#define VALID_FILE_ID						MAX_FILE_NUM


// flash����Ϣ�洢����ƫ�Ƶ�ַ
//FRAM�洢��С��Ϣ
#define MEM_FRAM_SIZE				    (8 * 1024)                  	//Fram��С
#define FLASH_FILE_SAVE_FRAM_OFFSET		(MEM_FRAM_SIZE - sizeof(g_nBlkInfo))
#define FLASH_FILE_OFFSET(nIdx)	 		((INT32U)(&g_sFlashFile[nIdx].nFileName[0]) - (INT32U)(&g_sFlashFile[0].nFileName[0]))



// flash��д
#define FLASH_WRITE(nAddr, pBuff, nLen)					MemoryWrite(nAddr, pBuff, nLen, MEM_FLASH)
#define FLASH_READ(nAddr, pBuff, nLen)					MemoryRead(nAddr, pBuff, nLen, MEM_FLASH)
#define FLASH_CLEAR(nStart, nLen, nVal)					MemoryClear(nStart, nLen, nVal, MEM_FLASH)


// �ļ�ϵͳ����洢
#define FLASH_FILE_SAVE(nAddr, pBuff, nLen, nType)		MemoryWrite(nAddr, pBuff, nLen, nType)
#define FLASH_FILE_QUERY(nAddr, pBuff, nLen, nType)		MemoryRead(nAddr, pBuff, nLen, nType)




// �ļ�����ṹ
typedef struct _tagFLASH_FILE
{
	char			nFileName[10];
	INT32U			nBlkStartIdx;
	
}FLSAH_FILE, *pFLSAH_FILE;


/**************************************************************************************************
                                          �ⲿ����
***************************************************************************************************/
extern  FLSAH_FILE 		g_sFlashFile[MAX_FILE_NUM]	;
extern  INT8U			g_nBlkInfo[MAX_BLK_NUM + 2];

extern 	INT32U			g_nFlashFileBaseAddr;
extern  INT8U			g_nFlashFileSaveType;


/**************************************************************************************************
                                          ��غ���˵��
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









