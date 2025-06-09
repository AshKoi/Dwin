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
#include "MemDrv.h"
#include "MemDrvExt.h"
#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>
#include  <stdlib.h>
#include  <stdarg.h>

/**************************************************************************************************
                                          ���ȫ�ֱ���ʵ��
***************************************************************************************************/
	//���ݿ����У���׼
#define     MEM_CHK_BASE_VAL					0x33					//���ݿ����У���׼


#ifndef NULL
#define     NULL                                0
#endif

#ifndef INVLD_DATA
#define     INVLD_DATA              		    0xFF			//��Ч����
#endif

#ifndef GET_MIN
#define 	GET_MIN(a, b)			    	    (((a) <= (b)) ? (a) : (b)) 						//��ȡ��Сֵ
#endif





extern void*	MemTblGet(void);
extern INT32S  MemObjTblNumGet(void);

/**************************************************************************************************
                                          ��غ���ʵ��
***************************************************************************************************/
/*
***************************************************************************
* ������:GetColMaxNum
* ����:
*       pMEM_OFF_INFO	pCol									������������ݱ�:��g_sTermCommOff
* ���:	
*       INT32U* 		pColMaxNum								����
* ����:RETӦ�ñ�׼����ֵ
* ����:��
* ����:wyf
* ����:2010/04/13
* �汾:Version 1.0
* ��������:��ȡ������������ݱ����Ŀ��
***************************************************************************
*/
static INT8S GetColMaxNum(pMEM_OFF_INFO pCol, INT32U* pColMaxNum)
{
	INT32U	i = 0;
	
	if (NULL == pCol)
	{
		return -1;
	}
	if (NULL == pColMaxNum)
	{
		return -1;
	}

	while(1)
	{
		if (ITEM_COL_END == pCol[i].nItmTblId)
		{
			break;
		}
		i++;
	}

	*pColMaxNum = i;
    
    return 0;
}

/********************************************************************************
* ������: MemChkSumChk
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ���У���Ƿ���������������
*********************************************************************************/
static BOOLEAN MemChkSumChk(const INT8U* const pBuf, INT32U nLen, INT8U nChkVal)
{
	INT32U	i 		= 0;
	INT8U	nChkSum = 0;
	
	if (NULL == pBuf)
	{
		return 0;	
	}
	if (0 == nLen)
	{
		return 0;
	}
	
	for (i = 0; i < nLen; i++)
	{
		nChkSum += pBuf[i];
	}
	
	nChkSum += MEM_CHK_BASE_VAL;
	
	if (nChkVal == nChkSum)
	{
		return 1;
	}
	
	return 0;
}

/********************************************************************************
* ������: MemChkSumGet
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ���У���
*********************************************************************************/
static INT8S MemChkSumGet(const INT8U* const pBuf, INT32U nLen, INT8U* pChkVal)
{
	INT32U	i 		= 0;
	INT8U	nChkSum = 0;
	
	if ((NULL == pBuf) || (NULL == pChkVal))
	{
		return -1;	
	}
	if (0 == nLen)
	{
		return -1;
	}
	
	for (i = 0; i < nLen; i++)
	{
		nChkSum += pBuf[i];
	}
	nChkSum += MEM_CHK_BASE_VAL;
	*pChkVal = nChkSum;
	
	return 0;
}



/********************************************************************************
* ������:
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: �������ݿ��Ż�ȡ���ݿ������±�
*********************************************************************************/
static INT8S MemTblIdToTblIdx(INT32U nTblId, INT32U* pTblIdx)
{
    void*      pMemTblBase         = NULL;
    pMEM_INFO   pMemInfo            = NULL;
    INT32S      i                   = 0;
    INT32S      nMemObjTblNumMax    = 0;
    
    nMemObjTblNumMax = MemObjTblNumGet();
	if (nTblId >= nMemObjTblNumMax)
	{
		return -1;	
	}
	if (NULL == pTblIdx)
	{
		return -1;	
	}
    // *pTblIdx = nTblId;
    
    pMemTblBase = MemTblGet();
    pMemInfo = (pMEM_INFO)pMemTblBase;
    
    while(1)
    {
        if (nMemObjTblNumMax == pMemInfo->nTblId)
        {
            return -1;	
        }
        if (nTblId == pMemInfo->nTblId)
        {
            break;
        }
        i++;
        pMemInfo++;
    }
	*pTblIdx = i;
	
	return 0;
}

#if 1
/*
***************************************************************************
* ������:MemIdToIdx
* ����:
*       INT32U			nTblIdx								    ���ݿ���
*       INT32U 			nObjId									�к�
* ���:	
*       INT32U*			pObjIdx									
* ����:RETӦ�ñ�׼����ֵ
* ����:��
* ����:wyf
* ����:2010/04/13
* �汾:Version 1.0
* ��������:��ȡ���±�
***************************************************************************
*/
static INT8S MemIdToIdx(INT32U nTblIdx, INT32U nObjId, INT32U *pObjIdx)
{
    void*               pMemTblBase         = NULL;
    pMEM_INFO           pMemInfo            = NULL;
    INT32S              nMemObjTblNumMax    = 0;
    
    nMemObjTblNumMax = MemObjTblNumGet();
    if (NULL == pObjIdx)
    {
        return -1;
    }
    if (nTblIdx >= nMemObjTblNumMax)
	{
		return -1;	
	}
    
    pMemTblBase = MemTblGet();
    pMemInfo = (pMEM_INFO)pMemTblBase;
    pMemInfo = &pMemInfo[nTblIdx];
    
    if (nObjId >= pMemInfo->nRowMaxNum)
    {
        return -1;	
    }
    
    *pObjIdx = nObjId;

    return 0;
}
#endif
/********************************************************************************
* ������:
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: �������ݿ�������±꼰�кţ���ȡ���ݿ�����Ϣ�����±�
*********************************************************************************/
static INT8S MemItmTblIdToItmTblIdx(INT32U nTblIdx, INT32U nItmTblId, INT32U* pItmTblIdx)
{
#if 0
    INT32S              nMemObjTblNumMax    = 0;
    
    nMemObjTblNumMax = MemObjTblNumGet();
	if (nTblIdx >= nMemObjTblNumMax)
	{
		return -1;	
	}
	if (NULL == pItmTblIdx)
	{
		return -1;	
	}
	*pItmTblIdx = nItmTblId;
	return 0; 
#else
    void*               pMemTblBase         = NULL;
    pMEM_INFO           pMemInfo            = NULL;
    pMEM_OFF_INFO       pMemOffInfoBase     = NULL;
    pMEM_OFF_INFO       pMemOffInfo         = NULL;
    INT32S              i                   = 0;
    INT32S              nMemObjTblNumMax    = 0;
    
    nMemObjTblNumMax = MemObjTblNumGet();
    if (nTblIdx >= nMemObjTblNumMax)
	{
		return -1;	
	}
	if (NULL == pItmTblIdx)
	{
		return -1;	
	}
    
    pMemTblBase = MemTblGet();
    pMemInfo = (pMEM_INFO)pMemTblBase;
    pMemInfo = &pMemInfo[nTblIdx];
    pMemOffInfoBase = (pMEM_OFF_INFO)(pMemInfo->pCol);
    pMemOffInfo = pMemOffInfoBase;
    while(1)
    {
        if (ITEM_COL_END == pMemOffInfo->nItmTblId)
        {
            if (nItmTblId != ITEM_COL_END)
            {
                return -1;
            }
            break;	
        }
        if (nItmTblId == pMemOffInfo->nItmTblId)
        {
            break;
        }
        i++;
        pMemOffInfo++;
    }
    
	*pItmTblIdx = i;
	return 0; 
#endif
}

/********************************************************************************
* ������:
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: �������ݿ�������±꼰�кţ���ȡ���ݿ�����Ϣ�����±�
*********************************************************************************/
static INT8S MemItmIdToItmIdx(INT32U nTblIdx, INT32U nItmTblIdx, INT32U nItmId, INT32U* pItmIdx)
{
    void*                   pMemTblBase     = NULL;
	pMEM_INFO 				pMemInfo		= NULL;
	pMEM_OFF_INFO 			pMemOffInfo		= NULL;
	pMEM_OFF_INFO 			pMemOffInfoBase	= NULL;
	INT32U					nColMaxNum		= 0;
    INT32S                  nMemObjTblNumMax= 0;
    
    nMemObjTblNumMax = MemObjTblNumGet();
	
	if (NULL == pItmIdx)
	{
		return -1;	
	}
	if (nTblIdx >= nMemObjTblNumMax)
	{
		return -1;	
	}
    pMemTblBase = MemTblGet();
    pMemInfo = (pMEM_INFO)pMemTblBase;
	pMemInfo = (pMEM_INFO)(&pMemInfo[nTblIdx]);
	if (GetColMaxNum(pMemInfo->pCol, &nColMaxNum) < 0)
	{
		return -1;	
	}
	if (nItmTblIdx > (nColMaxNum)) // �˴����� >= 
	{
	    return -1;
	}
    else if (nItmTblIdx == nColMaxNum)
    {
        *pItmIdx = nItmId;
        return 0; 
    }
	pMemOffInfoBase = (pMEM_OFF_INFO)(pMemInfo->pCol);
	pMemOffInfo     = &pMemOffInfoBase[nItmTblIdx];
	if (nItmId >= pMemOffInfo->nItmTblNum)
	{
	    return -2;
	}
	
	*pItmIdx = nItmId;
	
	return 0; 	
}

/********************************************************************************
* ������:
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: �������ݿ��š��кŻ�ȡ���ݿ������±ꡢ���±�
*********************************************************************************/
static INT8S MemAllIdToIdx(INT32U nObjTblId, INT32U nObjId, INT32U nObjItmTblId, INT32U nObjItmId, INT32U* pObjTblIdx, INT32U* pObjIdx, INT32U* pObjItmTblIdx, INT32U* pObjItmIdx)
{
	INT8S	nRet		= -1;

	//1.��ŵ����±��
    if (0 != (nRet = MemTblIdToTblIdx(nObjTblId, pObjTblIdx)))
	{
		return nRet;		
	}	
	//2.��ȡ�ñ�ĵڼ���
    if (0 != (nRet = MemIdToIdx(*pObjTblIdx, nObjId, pObjIdx)))
    {
        return nRet;
    }
	//3.��ŵ����±��
    if (0 != (nRet = MemItmTblIdToItmTblIdx(*pObjTblIdx, nObjItmTblId, pObjItmTblIdx)))
    {
        return nRet;
    }
    //4.��ȡ����ĵڼ���
	return MemItmIdToItmIdx(*pObjTblIdx, *pObjItmTblIdx, nObjItmId, pObjItmIdx);
}

/**************************************************************************************************
                                          ��λ����
***************************************************************************************************/



/*
***************************************************************************
* ������:MemGetLocInfo
* ����:
*       INT32U 			nHistSize							        ����ƫ��
*       INT8U 		    nBackIdx							        ���ݻ���ַ�����±�
*       INT32U			nObjTblIdx								    ���ݿ���ʶ���±�
*       INT32U 			nObjIdx									    ĳһ��ĵڼ����±�
*       INT32U 			nObjItmTblIdx								������ʶ���±�
*       INT32U          nObjItmIdx                                  ĳһ��ĵڼ����±�
* ���:	
*       pMEM_LOC_INFO			pInfo								��λ��Ϣ
* ����:RETӦ�ñ�׼����ֵ
* ����:��
* ����:wyf
* ����:2010/04/13
* �汾:Version 1.0
* ��������:��ȡ��λ��Ϣ
***************************************************************************/
static INT8S MemGetLocInfo(INT32U nHistSize, INT8U nBackIdx, INT32U nObjTblIdx, INT32U nObjIdx, INT32U nObjItmTblIdx, INT32U nObjItmIdx, pMEM_LOC_INFO pInfo)
{
    //INT8S               nRet            = RET_ERR_ERROR;
    INT32U				nObjTblId		= 0;
    INT32U              nObjItmTblIdLen = 0;
	pMEM_INFO			pMemInfo		= NULL;
	pMEM_BASE_INFO_SUB	pMemBaseInfoSub	= NULL;
	INT32U				nColMaxNum		= 0;
	INT32U				nColSize		= 0;
    INT32U              nObjItmIdNum    = 0;
    void*               pMemTblBase     = NULL;
    INT32S              nMemObjTblNumMax= 0;
    INT32U              nRowAddr        = 0;
    
    nMemObjTblNumMax = MemObjTblNumGet();
    
    if ((NULL == pInfo))
    {
        return -1;
    }

    if (nObjTblIdx >= nMemObjTblNumMax)
    {
        return -2;
    }
    pMemTblBase = MemTblGet();
    pMemInfo    = (pMEM_INFO)pMemTblBase;
    pMemInfo    = (pMEM_INFO)(&(pMemInfo[nObjTblIdx]));
	if (GetColMaxNum(pMemInfo->pCol, &nColMaxNum) < 0)
	{
		return -2;
	}
	nColSize = pMemInfo->pCol[nColMaxNum].nLen;
    if (nColSize == 0)
    {
        return -2;
    }
	if (nObjItmTblIdx > (nColMaxNum))
	{
		return -2;
	}
    nObjTblId = pMemInfo->nTblId;						//���,�����������
    if (nBackIdx >= MEM_MAX_BACK_NUM/*pMemInfo->nBackNum*/)
    {
    	return -2;
    }
    pMemBaseInfoSub = &(pMemInfo->sMemBaseInfoSub[nBackIdx]);
    
    //��ȡ��λ��Ϣ
    pInfo->nMedium    = pMemBaseInfoSub->nMedium;
    nRowAddr   = pMemBaseInfoSub->nAddr
                            + nHistSize
                            + nColSize * nObjIdx;
    pInfo->pFileName  = (char*)(pMemBaseInfoSub->pFileName);
    
    nObjItmIdNum = pMemInfo->pCol[nObjItmTblIdx].nItmTblNum;
    if (0 == nObjItmIdNum)
    {
        nObjItmIdNum = 1;
    }
    if (nObjItmIdx >= nObjItmIdNum)
    {
        return -2;
    }
    //nObjItmTblIdLen   = pMemInfo->pCol[nObjItmTblIdx].nLen;
    if (nObjItmTblIdx == nColMaxNum)
    {
        if (pMemInfo->pCol[nColMaxNum].bChk)
        {
            nObjItmTblIdLen = nColSize - 1;
        }
        else
        {
            nObjItmTblIdLen = nColSize;
        }
    }
    else if (nObjItmTblIdx + 1 == nColMaxNum)  
    {
        if (pMemInfo->pCol[nColMaxNum].bChk)
        {
            nObjItmTblIdLen = nColSize - 1 - pMemInfo->pCol[nObjItmTblIdx].nAddr;
        }
        else
        {
            nObjItmTblIdLen = nColSize - pMemInfo->pCol[nObjItmTblIdx].nAddr;
        }
    }
    else
    {
        nObjItmTblIdLen = pMemInfo->pCol[nObjItmTblIdx + 1].nAddr - pMemInfo->pCol[nObjItmTblIdx].nAddr;
    }
    nObjItmTblIdLen = nObjItmTblIdLen / nObjItmIdNum;
    /*
    if (0 == nObjItmTblIdx)
    {
        pInfo->nAddr  = nRowAddr + pMemInfo->pCol[nObjItmTblIdx].nAddr;
    }
    else
    {
        pInfo->nAddr  = nRowAddr + pMemInfo->pCol[nObjItmTblIdx].nAddr + nObjItmTblIdLen * nObjItmIdx;
    }
    pInfo->nLen       = pMemInfo->pCol[nObjItmTblIdx].nLen;
    */
    pInfo->nAddr  = nRowAddr + pMemInfo->pCol[nObjItmTblIdx].nAddr + nObjItmTblIdLen * nObjItmIdx;
    pInfo->nLen   = nObjItmTblIdLen;
    //pInfo->nRowMaxNum = pMemInfo->nRowMaxNum;
    //pInfo->pCol       = pMemInfo->pCol;
    
    return (MEM_MEDIUM_INVLD == pInfo->nMedium) ? (-1) : (0);
}






#if 0
#endif
/*
***************************************************************************
* ������:MemGetDbfLocInfo
* ����:
*       INT8U			nBackIdx								    ���ݽ��������±�
*       INT32U			nObjTblIdx								    ���ݿ���ʶ���±�
*       INT32U 			nObjIdx									    ĳһ��ĵڼ����±�
*       INT32U 			nObjItmTblIdx								������ʶ���±�
*       INT32U          nObjItmIdx                                  ĳһ��ĵڼ����±�
* ���:	
*       pMEM_LOC_INFO	pInfo									
* ����:RETӦ�ñ�׼����ֵ
* ����:��
* ����:wyf
* ����:2010/04/13
* �汾:Version 1.0
* ��������:д�ӿ�
***************************************************************************
*/
static INT8S MemGetDbfLocInfo(INT8U nBackIdx, INT32U nObjTblIdx, INT32U nObjIdx, INT32U nObjItmTblIdx, INT32U nObjItmIdx, INT32U nHistSize, INT16U nHistIdx, INT32U nPointSize, INT32U nCrvIdx, pMEM_LOC_INFO pInfo)
{
	
	return MemGetLocInfo((nHistSize * nHistIdx + nPointSize * nCrvIdx), nBackIdx, nObjTblIdx, nObjIdx, nObjItmTblIdx, nObjItmIdx, pInfo);
    //return MemGetLocInfo(0, nBackIdx, nObjTblIdx, nObjIdx, nObjItmTblIdx, nObjItmIdx, pInfo);
}

/*
***************************************************************************
* ������:MemReadDbf
* ����:
*       INT32U			nObjTblId								���ݿ���ʶ��
*       INT32U 			nObjId									ĳһ��ĵڼ���:��ͬ�ı��еĴ�0���еĴ�1�������ȵ�
*       INT32U 			nItmTblId								������ʶ��
*       INT32U          nItmId                                  ĳһ��ĵڼ���:��0��ʼ
*       INT32U 			nOffset									��׼ƫ��
*		INT32U 			nHistSize								һ�����е�����в��������ݳ���
*       INT16U 			nHistIdx								�տ�ָʾ�±�
*		INT32U			nPointSize								һ��������в��������߳���  ����ʱʹ��
*       INT32U 			nCrvIdx									���ߵ�(��0��ʼ)
*       void* 			pBuf									����
*       INT32U 			nLen									���峤��
* ���:	
*       INT32U* 		pRetLen									���س��ȣ�(��ָ��ʱ�������س���)
* ����:RETӦ�ñ�׼����ֵ
* ����:��
* ����:wyf
* ����:2010/04/13
* �汾:Version 1.0
* ��������:���ӿ�
***************************************************************************
*/
INT8S MemReadDbf(INT32U nObjTblId, INT32U nObjId, INT32U nItmTblId, INT32U nItmId, INT32U nOffset, INT32U nHistSize, INT16U nHistIdx, INT32U nPointSize, INT32U nCrvIdx, void *pBuf, INT32U nLen, INT32U* pRetLen)
{
    INT8S           nRet            = -1;
    INT8U			i				= 0;
	INT8U			j				= 0;
	//INT8U			nMediumCnt		= 0;
	//INT8U     		nErr 			= 0;
	INT32U			nExcLen 		= 0;
	INT32U			nTblIdx 		= 0;
	INT32U 			nObjIdx 		= 0;
    INT32U			nItmTblIdx 		= 0;
	INT32U			nItmIdx 		= 0;
	pMEM_INFO		pMemInfo		= NULL;
	pMEM_OFF_INFO	pMemOffInfo 	= NULL;
	INT8U*			pData			= NULL;
    INT32U          nDrvSize        = 0;
	MEM_LOC_INFO  	sInfo;
	INT32U			nColMaxNum		= 0;
    void*           pMemTblBase     = NULL;
    INT8U           nChkFlg         = 0;
    pMEM_OFF_INFO	pMemOffInfoTmp 	= NULL;
    
    if ((NULL == pBuf))
	{
		return -1;
	}
    pData = MemDrvBufGet();
    nDrvSize = MemDrvBufSizeGet();
    if (0 != (nRet = MemAllIdToIdx(nObjTblId, nObjId, nItmTblId, nItmId, &nTblIdx, &nObjIdx, &nItmTblIdx, &nItmIdx)))
	{
		return nRet;	
	}
    pMemTblBase = MemTblGet();
    pMemInfo    = (pMEM_INFO)pMemTblBase;
    pMemInfo	= (pMEM_INFO)(&(pMemInfo[nTblIdx]));
    pMemOffInfo = (pMEM_OFF_INFO)(&(pMemInfo->pCol[nItmTblIdx]));
	if (GetColMaxNum(pMemInfo->pCol, &nColMaxNum) < 0)
	{
		return -1;
	}
    
    // Ŀǰ���������У��:��ȡ���һ�е��������Ϣ
    pMemOffInfoTmp = (pMEM_OFF_INFO)(&(pMemInfo->pCol[nColMaxNum]));
    for (i = 0; i < MEM_MAX_BACK_NUM; i++)
    {
        if (MEM_MEDIUM_INVLD == pMemInfo->sMemBaseInfoSub[i].nMedium)
		{
            break;	
		}
        memset(&sInfo, 0, sizeof(MEM_LOC_INFO));
        if (0 != MemGetDbfLocInfo(i, nTblIdx, nObjIdx, nColMaxNum, 0, nHistSize, nHistIdx, nPointSize, nCrvIdx, &sInfo))
		{
			return -1;
		}	
        if (pMemOffInfoTmp->bChk)
        {
            if ((sInfo.nLen + 1) > nDrvSize)
			{
				return -1;
			}
            MemDrvBufSemPend();  //����	
			memset(pData, 0, nDrvSize);
			MemRead(sInfo.pFileName, sInfo.nMedium, sInfo.nAddr, sInfo.nLen + 1, pData);
            if (MemChkSumChk(pData, sInfo.nLen, pData[sInfo.nLen]))
            {
                nChkFlg = 1;
                if (i > 0)
                {
                    for (j = 0; j < i; j++)
                    {
                        if (0 != MemGetDbfLocInfo(j, nTblIdx, nObjIdx, nColMaxNum, 0, nHistSize, nHistIdx, nPointSize, nCrvIdx, &sInfo))
                        {
                            continue;
                        }
                        MemWrite(sInfo.pFileName, sInfo.nMedium, sInfo.nAddr, sInfo.nLen + 1, pData);		
                    }
                }
            }
            MemDrvBufSemPost();	//����
            if (nChkFlg)
            {
                break;
            }
        }
        else
        {
            nChkFlg = 1;
            break;
        }
    }
    
    if (1 != nChkFlg)
    {
        if (ITEM_COL_END != nItmTblId)
        {
            return -1;
        }
    }
    
    for (i = 0; i < MEM_MAX_BACK_NUM; i++)
    {
        if (MEM_MEDIUM_INVLD == pMemInfo->sMemBaseInfoSub[i].nMedium)
		{
			//nMediumCnt++;
			//continue;
            break;	
		}
        memset(&sInfo, 0, sizeof(MEM_LOC_INFO));
        if (0 != MemGetDbfLocInfo(i, nTblIdx, nObjIdx, nItmTblIdx, nItmIdx, nHistSize, nHistIdx, nPointSize, nCrvIdx, &sInfo))
		{
			return -1;
		}
        if (nOffset >= sInfo.nLen)
		{
			return -1;	
		}
        nExcLen = GET_MIN((sInfo.nLen - nOffset), nLen);	
        MemRead(sInfo.pFileName, sInfo.nMedium, sInfo.nAddr + nOffset, nExcLen, pBuf);
        if (NULL != pRetLen)
        {
            *pRetLen = nExcLen;
        }
        return 0;
    }
    
    return -1;	
#if 0         
    if (ITEM_COL_END == nItmTblId)
	{
        INT32U 			nItmTblLoop     = 0;
        INT32U          nItmLoop        = 0;
		BOOLEAN			bFlg	 		= -1;
		

		for (nItmTblLoop = 0; nItmTblLoop < nColMaxNum; nItmTblLoop++)
        {
            pMemOffInfoTmp = (pMEM_OFF_INFO)(&(pMemInfo->pCol[nItmTblLoop]));
            if (!(pMemOffInfoTmp->bChk))
            {
                continue;
            }
            for (nItmLoop = 0; nItmLoop < pMemOffInfoTmp->nItmTblNum; nItmLoop++)
            {
                bFlg = 0;
                for (i = 0; i < MEM_MAX_BACK_NUM; i++)
                {
                    if (MEM_MEDIUM_INVLD == pMemInfo->sMemBaseInfoSub[i].nMedium)
                    {
                        //continue;	
                        break;
                    }
                    memset(&sInfo, 0, sizeof(MEM_LOC_INFO));
                    if (0 != MemGetDbfLocInfo(i, nTblIdx, nObjIdx, nItmTblLoop, nItmLoop, nHistSize, nHistIdx, nPointSize, nCrvIdx, &sInfo))
                    {
                        return -1;
                    }

                    MemDrvBufSemPend();  //����	
                    memset(pData, 0, nDrvSize);
                    MemRead(sInfo.pFileName, sInfo.nMedium, sInfo.nAddr, sInfo.nLen + 1, pData);
                    if (MemChkSumChk(pData, sInfo.nLen, pData[sInfo.nLen]))
                    {
                        bFlg = 1;
                        if (i > 0)
                        {
                            for (j = 0; j < i; j++)
                            {
                                if (0 != MemGetDbfLocInfo(j, nTblIdx, nObjIdx, nItmTblLoop, nItmLoop, nHistSize, nHistIdx, nPointSize, nCrvIdx, &sInfo))
                                {
                                    continue;
                                }
                                MemWrite(sInfo.pFileName, sInfo.nMedium, sInfo.nAddr, sInfo.nLen + 1, pData);	
                            }
                        }
                    }
                    MemDrvBufSemPost();	//����
                    if (bFlg)
                    {
                        break;
                    }
                }
                if (!bFlg)
                {
                    return -1;
                }
            }
        }
        
        if (pMemOffInfo->bChk)
		{
			for (i = 0; i < MEM_MAX_BACK_NUM; i++)
			{
				if (MEM_MEDIUM_INVLD == pMemInfo->sMemBaseInfoSub[i].nMedium)
				{
					//continue;
                    break;	
				}
				memset(&sInfo, 0, sizeof(MEM_LOC_INFO));
				if (0 != MemGetDbfLocInfo(i, nTblIdx, nObjIdx, 0, 0, nHistSize, nHistIdx, nPointSize, nCrvIdx, &sInfo))
				{
					return -1;
				}
				MemDrvBufSemPend();  //����	
				memset(pData, 0, nDrvSize);
				MemRead(sInfo.pFileName, sInfo.nMedium, sInfo.nAddr, sInfo.nLen + 1, pData);
				if (!MemChkSumChk(pData, sInfo.nLen, pData[sInfo.nLen]))
				{
					MemChkSumGet(pData, sInfo.nLen, &pData[sInfo.nLen]);
					MemWrite(sInfo.pFileName, sInfo.nMedium, sInfo.nAddr + sInfo.nLen, 1, &pData[sInfo.nLen]);
				}
				MemDrvBufSemPost();	//����
			}
		}
        
	}
  
    for (i = 0; i < MEM_MAX_BACK_NUM; i++)
    {
        if (MEM_MEDIUM_INVLD == pMemInfo->sMemBaseInfoSub[i].nMedium)
		{
			//nMediumCnt++;
			//continue;
            break;	
		}
        memset(&sInfo, 0, sizeof(MEM_LOC_INFO));
        if (0 != MemGetDbfLocInfo(i, nTblIdx, nObjIdx, nItmTblIdx, nItmIdx, nHistSize, nHistIdx, nPointSize, nCrvIdx, &sInfo))
		{
			return -1;
		}
        if (nOffset >= sInfo.nLen)
		{
			return -1;	
		}
        nExcLen = GET_MIN((sInfo.nLen - nOffset), nLen);	
        if (pMemOffInfo->bChk)
        {
            if ((sInfo.nLen + 1) > nDrvSize)
			{
				return -1;
			}
            MemDrvBufSemPend();  //����	
			memset(pData, 0, nDrvSize);
			MemRead(sInfo.pFileName, sInfo.nMedium, sInfo.nAddr, sInfo.nLen + 1, pData);
            if (MemChkSumChk(pData, sInfo.nLen, pData[sInfo.nLen]))
            {
                if (i > 0)
                {
                    for (j = 0; j < i; j++)
                    {
                        if (0 != MemGetDbfLocInfo(j, nTblIdx, nObjIdx, nItmTblIdx, nItmIdx, nHistSize, nHistIdx, nPointSize, nCrvIdx, &sInfo))
                        {
                            continue;
                        }
                        MemWrite(sInfo.pFileName, sInfo.nMedium, sInfo.nAddr, sInfo.nLen + 1, pData);		
                    }
                }
                memcpy(pBuf, &pData[nOffset], nExcLen);
                if (NULL != pRetLen)
                {
                    *pRetLen = nExcLen;
                }
                MemDrvBufSemPost();	//����
                return 0;
            }
            MemDrvBufSemPost();	//����
        }
        else
        {
            MemRead(sInfo.pFileName, sInfo.nMedium, sInfo.nAddr + nOffset, nExcLen, pBuf);
			if (NULL != pRetLen)
			{
				*pRetLen = nExcLen;
			}
		    return 0;
        }
    }
    return -1;	
 #endif 
}

/*
***************************************************************************
* ������:MemWriteDbf
* ����:
*       INT32U			nObjTblId								���ݿ���ʶ��
*       INT32U 			nObjId									ĳһ��ĵڼ���
*       INT32U 			nItmTblId								������ʶ��
*       INT32U          nItmId                                  ĳһ��ĵڼ���
*       INT32U 			nOffset									��׼ƫ��
*		INT32U 			nHistSize								һ�����е�����в��������ݳ���
*       INT16U 			nHistIdx								�տ�ָʾ�±�
*		INT32U			nPointSize								һ��������в��������߳���  ����ʱʹ��
*       INT32U 			nCrvIdx									���ߵ�(��0��ʼ)
*       void* 			pBuf									����
*       INT32U 			nLen									���峤��
* ���:	
*       INT32U* 		pRetLen									���س��ȣ�(��ָ��ʱ�������س���)
* ����:RETӦ�ñ�׼����ֵ
* ����:��
* ����:wyf
* ����:2010/04/13
* �汾:Version 1.0
* ��������:д�ӿ�
***************************************************************************
*/
INT8S MemWriteDbf(INT32U nObjTblId, INT32U nObjId, INT32U nItmTblId, INT32U nItmId, INT32U nOffset, INT32U nHistSize, INT16U nHistIdx, INT32U nPointSize, INT32U nCrvIdx, const void *pBuf, INT32U nLen, INT32U* pRetLen)
{
    INT8S           nRet            = -1;
    INT8U		 	i		  		= 0;
    INT8U           j               = 0;
    INT8U		 	nMediumCnt 		= 0;
	//INT8U		 	nErr 	  		= 0;
	INT32U		  	nExcLen 		= 0;
	INT32U			nTblIdx 		= 0;
	INT32U 			nObjIdx 		= 0;
    INT32U			nItmTblIdx 		= 0;
	INT32U			nItmIdx 		= 0;
	pMEM_INFO		pMemInfo		= NULL;
	pMEM_OFF_INFO	pMemOffInfo 	= NULL;
	INT8U*		  	pData	  		= NULL;
    INT32U          nDrvSize        = 0;
	MEM_LOC_INFO  	sInfo;
    MEM_LOC_INFO  	sInfo2;
	INT32U			nColMaxNum		= 0;
    void*           pMemTblBase     = NULL;
    INT8U           nChkFlg         = 0;
    pMEM_OFF_INFO	pMemOffInfoTmp 	= NULL;
	
	if ((NULL == pBuf))
	{
		return -1;
	}
    pData = MemDrvBufGet();
    nDrvSize = MemDrvBufSizeGet();
	if (0 != (nRet = MemAllIdToIdx(nObjTblId, nObjId, nItmTblId, nItmId, &nTblIdx, &nObjIdx, &nItmTblIdx, &nItmIdx)))
	{
		return nRet;	
	}
	pMemTblBase = MemTblGet();
    pMemInfo    = (pMEM_INFO)pMemTblBase;
	pMemInfo		= (pMEM_INFO)(&(pMemInfo[nTblIdx]));
    pMemOffInfo     = (pMEM_OFF_INFO)(&(pMemInfo->pCol[nItmTblIdx]));		

	if (GetColMaxNum(pMemInfo->pCol, &nColMaxNum) < 0)
	{
		return -1;
	}
    // Ŀǰ���������У��:��ȡ���һ�е��������Ϣ
    pMemOffInfoTmp = (pMEM_OFF_INFO)(&(pMemInfo->pCol[nColMaxNum]));
    for (i = 0; i < MEM_MAX_BACK_NUM; i++)
	{
		if (MEM_MEDIUM_INVLD == pMemInfo->sMemBaseInfoSub[i].nMedium)
		{
			nMediumCnt++;
			//continue;	
            break;
		}
        if (pMemOffInfoTmp->bChk)
        {
            memset(&sInfo, 0, sizeof(MEM_LOC_INFO));
            if (0 != MemGetDbfLocInfo(i, nTblIdx, nObjIdx, nColMaxNum, 0, nHistSize, nHistIdx, nPointSize, nCrvIdx, &sInfo))
            {
                return -1;
            }
            if ((sInfo.nLen + 1) > nDrvSize)
			{
				return -1;
			}			
			MemDrvBufSemPend(); //����
			memset(pData, 0, nDrvSize);
			MemRead(sInfo.pFileName, sInfo.nMedium, sInfo.nAddr, sInfo.nLen + 1, pData);
            if (MemChkSumChk(pData, sInfo.nLen, pData[sInfo.nLen]))
            {
                nChkFlg = 1;
                if (i > 0)
                {
                    for (j = 0; j < i; j++)
                    {
                        if (0 != MemGetDbfLocInfo(j, nTblIdx, nObjIdx, nColMaxNum, 0, nHistSize, nHistIdx, nPointSize, nCrvIdx, &sInfo))
                        {
                            continue;
                        }
                        MemWrite(sInfo.pFileName, sInfo.nMedium, sInfo.nAddr, sInfo.nLen + 1, pData);		
                    }
                }
            }
            MemDrvBufSemPost();	//����
            if (nChkFlg)
            {
                break;
            }
        }
        else
        {
            nChkFlg = 1;
            break;
        }
	}
    if (1 != nChkFlg)
    {
        if (ITEM_COL_END != nItmTblId)
        {
            return -1;
        }
    }
    
    nChkFlg = 0;
    for (i = 0; i < MEM_MAX_BACK_NUM; i++)
	{
		if (MEM_MEDIUM_INVLD == pMemInfo->sMemBaseInfoSub[i].nMedium)
		{
			nMediumCnt++;
			//continue;	
            break;
		}
        if (pMemOffInfoTmp->bChk)
        {
            memset(&sInfo, 0, sizeof(MEM_LOC_INFO));
            if (0 != MemGetDbfLocInfo(i, nTblIdx, nObjIdx, nColMaxNum, 0, nHistSize, nHistIdx, nPointSize, nCrvIdx, &sInfo))
            {
                return -1;
            }
            if ((sInfo.nLen + 1) > nDrvSize)
			{
				return -1;
			}
            
            memset(&sInfo2, 0, sizeof(MEM_LOC_INFO));
            if (0 != MemGetDbfLocInfo(i, nTblIdx, nObjIdx, nItmTblIdx, nItmIdx, nHistSize, nHistIdx, nPointSize, nCrvIdx, &sInfo2))
            {
                return -1;
            }
            if (nOffset >= sInfo2.nLen)
            {
                return -1;	
            }
            nExcLen = GET_MIN((sInfo2.nLen - nOffset), nLen);
			
			MemDrvBufSemPend(); //����
			memset(pData, 0, nDrvSize);
			MemRead(sInfo.pFileName, sInfo.nMedium, sInfo.nAddr, sInfo.nLen + 1, pData);
            memcpy(&pData[sInfo2.nAddr - sInfo.nAddr + nOffset], pBuf, nExcLen);
			MemChkSumGet(pData, sInfo.nLen, &pData[sInfo.nLen]);
            for (j = i; j < MEM_MAX_BACK_NUM; j++)
            {
                if (MEM_MEDIUM_INVLD == pMemInfo->sMemBaseInfoSub[j].nMedium)
                {
                    continue;	
                }
                memset(&sInfo, 0, sizeof(MEM_LOC_INFO));
                if (0 != MemGetDbfLocInfo(j, nTblIdx, nObjIdx, nColMaxNum, 0, nHistSize, nHistIdx, nPointSize, nCrvIdx, &sInfo))
                {
                    continue;	
                }
                MemWrite(sInfo.pFileName, sInfo.nMedium, sInfo.nAddr, sInfo.nLen + 1, pData);
            }
			MemDrvBufSemPost();	//����
            break;
        }
        else
        {
            memset(&sInfo, 0, sizeof(MEM_LOC_INFO));
            if (0 != MemGetDbfLocInfo(i, nTblIdx, nObjIdx, nItmTblIdx, nItmIdx, nHistSize, nHistIdx, nPointSize, nCrvIdx, &sInfo))
            {
                return -1;
            }
            if (nOffset >= sInfo.nLen)
            {
                return -1;	
            }
            nExcLen = GET_MIN((sInfo.nLen - nOffset), nLen);
            nExcLen = nLen; /////��ʱ
            MemWrite(sInfo.pFileName, sInfo.nMedium, sInfo.nAddr + nOffset, nExcLen, pBuf);
        }
	}
	
	if (nMediumCnt >= MEM_MAX_BACK_NUM)
	{
		return -1;
	}
	
	if (NULL != pRetLen)
	{
		*pRetLen = nExcLen;
	}
    
#if 0    
    if (ITEM_COL_END == nItmTblId)														//���ű����
	{
		INT32U 			nItmTblLoop     = 0;
        INT32U          nItmLoop        = 0;
		INT32U			nLenTmp			= 0;
		INT32U			nOffsetTmp		= 0;
		pMEM_OFF_INFO	pMemOffInfoTmp 	= NULL;
        INT8U*		  	pBufTmp         = (INT8U*)pBuf;
        INT8U*		  	pData2	  		= NULL;
        INT32U          nDrvBufUnitSize = 0;
	    MEM_LOC_INFO  	sInfo2;
		
		pData2 = MemDrvBufUnitGet();
        nDrvBufUnitSize = MemDrvBufSizeUnitGet();
        for (i = 0; i < MEM_MAX_BACK_NUM; i++)
		{
			if (MEM_MEDIUM_INVLD == pMemInfo->sMemBaseInfoSub[i].nMedium)
			{
				nMediumCnt++;												//�洢����
				//continue;
                break;
			}
			memset(&sInfo, 0, sizeof(MEM_LOC_INFO));				
            if (0 != MemGetDbfLocInfo(i, nTblIdx, nObjIdx, 0, 0, nHistSize, nHistIdx, nPointSize, nCrvIdx, &sInfo))
			{
				return -1;
			}
			
			nOffsetTmp = 0;
			nLenTmp    = 0;
			MemDrvBufSemPend();  				//����
			memset(pData, 0, nDrvSize);
			MemRead(sInfo.pFileName, sInfo.nMedium, sInfo.nAddr, sInfo.nLen + 1, pData);		//��ȡ���ű������
			for (nItmTblLoop = 0; nItmTblLoop < nColMaxNum; nItmTblLoop++) //ѭ����ȡ������
            {
                pMemOffInfoTmp = (pMEM_OFF_INFO)(&(pMemInfo->pCol[nItmTblLoop]));
                memset(&sInfo2, 0, sizeof(MEM_LOC_INFO));
                for (nItmLoop = 0; nItmLoop < pMemOffInfoTmp->nItmTblNum; nItmLoop++)
                {
                    if (0 != MemGetDbfLocInfo(i, nTblIdx, nObjIdx, nItmTblLoop, nItmLoop, nHistSize, nHistIdx, nPointSize, nCrvIdx, &sInfo2))
                    {	
                        MemDrvBufSemPost();					//����
                        return -1;
                    }
                    if ((sInfo2.nLen + 1) > nDrvBufUnitSize)
                    {
                        MemDrvBufSemPost();					//����
                        return -1;
                    }
                    if (nLenTmp >= nLen)										//��д���ݴ����������
                    {
                        break;
                    }
                    memset(pData2, 0, nDrvBufUnitSize);					//��ȡĳ������:ע����һ���ֽ�
                    MemRead(sInfo2.pFileName, sInfo2.nMedium, sInfo2.nAddr, sInfo2.nLen + 1, pData2);
                    if (pMemOffInfoTmp->bChk)									//ĳ����У��
                    {
                        if ((nOffsetTmp <= nOffset) && ((nOffsetTmp + (sInfo2.nLen + 1)) > nOffset))
                        {
                            nExcLen = GET_MIN(((sInfo2.nLen + 1) - (nOffset - nOffsetTmp)), (nLen - nLenTmp));
                            memcpy(&pData2[nOffset - nOffsetTmp], &pBufTmp[nLenTmp], GET_MIN(((sInfo2.nLen + 1) - (nOffset - nOffsetTmp)), (nLen - nLenTmp)));
                            MemChkSumGet(pData2, sInfo2.nLen, &pData2[sInfo2.nLen]);
                            memcpy(&pData[nOffsetTmp], pData2, sInfo2.nLen + 1);
                            nLenTmp += nExcLen;
                            nOffsetTmp += (sInfo2.nLen + 1);
                        }
                        else if ((nOffsetTmp > nOffset) && (nLenTmp < nLen))
                        {
                            nExcLen = GET_MIN((sInfo2.nLen + 1), (nLen - nLenTmp));
                            memcpy(&pData2[0], &pBufTmp[nLenTmp], nExcLen);
                            MemChkSumGet(pData2, sInfo2.nLen, &pData2[sInfo2.nLen]);
                            memcpy(&pData[nOffsetTmp], pData2, sInfo2.nLen + 1);
                            nLenTmp += nExcLen;
                            nOffsetTmp += (sInfo2.nLen + 1);
                        }
                        else
                        {
                            nOffsetTmp += (sInfo2.nLen + 1);
                        }
                    }
                    else
                    {
                        if ((nOffsetTmp <= nOffset) && ((nOffsetTmp + (sInfo2.nLen)) > nOffset))
                        {
                            nExcLen = GET_MIN(((sInfo2.nLen) - (nOffset - nOffsetTmp)), (nLen - nLenTmp));
                            memcpy(&pData2[nOffset - nOffsetTmp], &pBufTmp[nLenTmp], GET_MIN(((sInfo2.nLen) - (nOffset - nOffsetTmp)), (nLen - nLenTmp)));
                            memcpy(&pData[nOffsetTmp], pData2, sInfo2.nLen);
                            nLenTmp += nExcLen;
                            nOffsetTmp += (sInfo2.nLen);
                        }
                        else if ((nOffsetTmp > nOffset) && (nLenTmp < nLen))
                        {
                            nExcLen = GET_MIN((sInfo2.nLen), (nLen - nLenTmp));
                            memcpy(&pData2[0], &pBufTmp[nLenTmp], nExcLen);
                            memcpy(&pData[nOffsetTmp], pData2, sInfo2.nLen);
                            nLenTmp += nExcLen;
                            nOffsetTmp += (sInfo2.nLen);
                        }
                        else
                        {
                            nOffsetTmp += (sInfo2.nLen);
                        }
                    }
                }
            }
			if (pMemOffInfo->bChk)
			{
				MemChkSumGet(pData, sInfo.nLen, &pData[sInfo.nLen]);
				MemWrite(sInfo.pFileName, sInfo.nMedium, sInfo.nAddr, sInfo.nLen + 1, pData);
			}
			else
			{
				MemWrite(sInfo.pFileName, sInfo.nMedium, sInfo.nAddr, sInfo.nLen, pData);
			}
			MemDrvBufSemPost();	//����
		}
		if (nMediumCnt >= MEM_MAX_BACK_NUM)
		{
			return -1;
		}
		
		if (NULL != pRetLen)
		{
			*pRetLen = nLenTmp;
		}
		
		return 0;
	}
  
    for (i = 0; i < MEM_MAX_BACK_NUM; i++)
	{
		if (MEM_MEDIUM_INVLD == pMemInfo->sMemBaseInfoSub[i].nMedium)
		{
			nMediumCnt++;
			//continue;	
            break;
		}
		memset(&sInfo, 0, sizeof(MEM_LOC_INFO));
		if (0 != MemGetDbfLocInfo(i, nTblIdx, nObjIdx, nItmTblIdx, nItmIdx, nHistSize, nHistIdx, nPointSize, nCrvIdx, &sInfo))
		{
			return -1;
		}
		if (nOffset >= sInfo.nLen)
		{
			return -1;	
		}
		nExcLen = GET_MIN((sInfo.nLen - nOffset), nLen);
		if (pMemOffInfo->bChk)
		{
			if ((sInfo.nLen + 1) > nDrvSize)
			{
				return -1;
			}			
			MemDrvBufSemPend(); //����
			memset(pData, 0, nDrvSize);
			MemRead(sInfo.pFileName, sInfo.nMedium, sInfo.nAddr, sInfo.nLen + 1, pData);
			memcpy(&pData[nOffset], pBuf, nExcLen);
			MemChkSumGet(pData, sInfo.nLen, &pData[sInfo.nLen]);
			MemWrite(sInfo.pFileName, sInfo.nMedium, sInfo.nAddr, sInfo.nLen + 1, pData);
			MemDrvBufSemPost();	//����
		}
		else
		{
			MemWrite(sInfo.pFileName, sInfo.nMedium, sInfo.nAddr + nOffset, nExcLen, pBuf);
		}
	}
	
	if (nMediumCnt >= MEM_MAX_BACK_NUM)
	{
		return -1;
	}
	
	if (NULL != pRetLen)
	{
		*pRetLen = nExcLen;
	}
#endif 	
	return 0;
}



#if 0
#endif
/********************************************************************************
* ������:MemReadDbfExt
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��:
*********************************************************************************/
INT32S MemReadDbfExt(INT32U nObjTblId, INT32U nObjId, INT32U nItmTblId, void *pBuf, INT32U nLen, INT32U* pRetLen)
{
    INT8S 	nRet 		= -1;
	
	if (NULL == pBuf)
	{
	    return -1;
	}
	
	if (0 != (nRet = MemReadDbf(nObjTblId, nObjId, nItmTblId, 0, 0, 0, 0, 0, 0, pBuf, nLen, pRetLen)))
	{
		memset(pBuf, INVLD_DATA, nLen);
	}
	return (INT32S)nRet;
}

/********************************************************************************
* ������:MemWriteDbfExt
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��:
*********************************************************************************/
INT32S MemWriteDbfExt(INT32U nObjTblId, INT32U nObjId, INT32U nItmTblId, const void *pBuf, INT32U nLen, INT32U* pRetLen)
{
	INT8S nRet = 0;
    nRet = MemWriteDbf(nObjTblId, nObjId, nItmTblId, 0, 0, 0, 0, 0, 0, pBuf, nLen, pRetLen);

	return (INT32S)nRet;
}



/*
***************************************************************************
* ������:MemReadHistDataExt
* ����:
*       INT32U			nObjTblId								���ݿ���
*       INT32U 			nObjId									�к�
*       INT32U 			nItmTblId								�к�
*		INT32U 			nHistSize								һ�����е�����в��������ݳ���
*       INT16U 			nHistIdx								�տ�ָʾ�±�
*		INT32U			nPointSize								һ��������в��������߳���  ����ʱʹ��
*       INT32U 			nCrvIdx									���ߵ�(��0��ʼ)
*       void* 			pBuf									����
*       INT32U 			nLen									���峤��
* ���:	
* ����:RETӦ�ñ�׼����ֵ
* ����:��
* ����:wyf
* ����:2010/04/13
* �汾:Version 1.0
* ��������:����ʷ���ݽӿ�
***************************************************************************
*/
INT32S MemReadHistDataExt(INT32U nObjTblId, INT32U nObjId, INT32U nItmTblId, INT32U nHistSize, INT16U nHistIdx, INT32U nPointSize, INT32U nCrvIdx, void *pBuf, INT32U nLen, INT32U* pRetLen)
{
	INT8S nRet = -1;
	
	if (NULL == pBuf)
	{
	    return -1;
	}
	
	if (0 != (nRet = MemReadDbf(nObjTblId, nObjId, nItmTblId, 0, 0, nHistSize, nHistIdx, nPointSize, nCrvIdx, pBuf, nLen, pRetLen)))
	{
		memset(pBuf, INVLD_DATA, nLen);
	}
	return (INT32S)nRet;
}

/*
***************************************************************************
* ������:MemWriteHistDataExt
* ����:
*       INT32U			nObjTblId								���ݿ���
*       INT32U 			nObjId									�к�
*       INT32U 			nItmTblId								�к�
*		INT32U 			nHistSize								һ�����е�����в��������ݳ���
*       INT16U 			nHistIdx								�տ�ָʾ�±�
*		INT32U			nPointSize								һ��������в��������߳���  ����ʱʹ��
*       INT32U 			nCrvIdx									���ߵ�(��0��ʼ)
*       void* 			pBuf									����
*       INT32U 			nLen									���峤��
* ���:	
* ����:RETӦ�ñ�׼����ֵ
* ����:��
* ����:wyf
* ����:2010/04/13
* �汾:Version 1.0
* ��������:����ʷ���ݽӿ�
***************************************************************************
*/
INT32S MemWriteHistDataExt(INT32U nObjTblId, INT32U nObjId, INT32U nItmTblId, INT32U nHistSize, INT16U nHistIdx, INT32U nPointSize, INT32U nCrvIdx, void *pBuf, INT32U nLen, INT32U* pRetLen)
{
	INT8S nRet = -1;
	
	if (NULL == pBuf)
	{
	    return -1;
	}
	
	nRet = MemWriteDbf(nObjTblId, nObjId, nItmTblId, 0, 0, nHistSize, nHistIdx, nPointSize, nCrvIdx, pBuf, nLen, pRetLen);
	return (INT32S)nRet;
}


