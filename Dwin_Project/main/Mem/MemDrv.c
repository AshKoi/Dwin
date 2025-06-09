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
#include "MemDrv.h"
#include "MemDrvExt.h"
#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>
#include  <stdlib.h>
#include  <stdarg.h>

/**************************************************************************************************
                                          相关全局变量实现
***************************************************************************************************/
	//数据库计算校验基准
#define     MEM_CHK_BASE_VAL					0x33					//数据库计算校验基准


#ifndef NULL
#define     NULL                                0
#endif

#ifndef INVLD_DATA
#define     INVLD_DATA              		    0xFF			//无效数据
#endif

#ifndef GET_MIN
#define 	GET_MIN(a, b)			    	    (((a) <= (b)) ? (a) : (b)) 						//获取最小值
#endif





extern void*	MemTblGet(void);
extern INT32S  MemObjTblNumGet(void);

/**************************************************************************************************
                                          相关函数实现
***************************************************************************************************/
/*
***************************************************************************
* 函数名:GetColMaxNum
* 输入:
*       pMEM_OFF_INFO	pCol									具体参数或数据表:如g_sTermCommOff
* 输出:	
*       INT32U* 		pColMaxNum								返回
* 返回:RET应用标准返回值
* 调用:无
* 作者:wyf
* 日期:2010/04/13
* 版本:Version 1.0
* 功能描述:获取具体参数或数据表的项目数
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
* 函数名: MemChkSumChk
* 输　入:
* 输　出:
* 返  回:
* 功  能: 检查校验是否相符：相符返回真
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
* 函数名: MemChkSumGet
* 输　入:
* 输　出:
* 返  回:
* 功  能: 获得校验和
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
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 根据数据库表号获取数据库数组下标
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
* 函数名:MemIdToIdx
* 输入:
*       INT32U			nTblIdx								    数据库表号
*       INT32U 			nObjId									行号
* 输出:	
*       INT32U*			pObjIdx									
* 返回:RET应用标准返回值
* 调用:无
* 作者:wyf
* 日期:2010/04/13
* 版本:Version 1.0
* 功能描述:获取行下标
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
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 根据数据库表数组下标及列号，获取数据库列信息数组下标
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
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 根据数据库表数组下标及列号，获取数据库列信息数组下标
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
	if (nItmTblIdx > (nColMaxNum)) // 此处不是 >= 
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
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: 根据数据库表号、列号获取数据库数组下标、列下标
*********************************************************************************/
static INT8S MemAllIdToIdx(INT32U nObjTblId, INT32U nObjId, INT32U nObjItmTblId, INT32U nObjItmId, INT32U* pObjTblIdx, INT32U* pObjIdx, INT32U* pObjItmTblIdx, INT32U* pObjItmIdx)
{
	INT8S	nRet		= -1;

	//1.表号到表下标号
    if (0 != (nRet = MemTblIdToTblIdx(nObjTblId, pObjTblIdx)))
	{
		return nRet;		
	}	
	//2.获取该表的第几个
    if (0 != (nRet = MemIdToIdx(*pObjTblIdx, nObjId, pObjIdx)))
    {
        return nRet;
    }
	//3.项号到项下标号
    if (0 != (nRet = MemItmTblIdToItmTblIdx(*pObjTblIdx, nObjItmTblId, pObjItmTblIdx)))
    {
        return nRet;
    }
    //4.获取该项的第几个
	return MemItmIdToItmIdx(*pObjTblIdx, *pObjItmTblIdx, nObjItmId, pObjItmIdx);
}

/**************************************************************************************************
                                          定位函数
***************************************************************************************************/



/*
***************************************************************************
* 函数名:MemGetLocInfo
* 输入:
*       INT32U 			nHistSize							        整块偏移
*       INT8U 		    nBackIdx							        备份基地址数组下标
*       INT32U			nObjTblIdx								    数据库表标识号下标
*       INT32U 			nObjIdx									    某一表的第几个下标
*       INT32U 			nObjItmTblIdx								表的项标识号下标
*       INT32U          nObjItmIdx                                  某一项的第几个下标
* 输出:	
*       pMEM_LOC_INFO			pInfo								定位信息
* 返回:RET应用标准返回值
* 调用:无
* 作者:wyf
* 日期:2010/04/13
* 版本:Version 1.0
* 功能描述:获取定位信息
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
    nObjTblId = pMemInfo->nTblId;						//表号,不是数组序号
    if (nBackIdx >= MEM_MAX_BACK_NUM/*pMemInfo->nBackNum*/)
    {
    	return -2;
    }
    pMemBaseInfoSub = &(pMemInfo->sMemBaseInfoSub[nBackIdx]);
    
    //获取定位信息
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
* 函数名:MemGetDbfLocInfo
* 输入:
*       INT8U			nBackIdx								    备份介质数组下标
*       INT32U			nObjTblIdx								    数据库表标识号下标
*       INT32U 			nObjIdx									    某一表的第几个下标
*       INT32U 			nObjItmTblIdx								表的项标识号下标
*       INT32U          nObjItmIdx                                  某一项的第几个下标
* 输出:	
*       pMEM_LOC_INFO	pInfo									
* 返回:RET应用标准返回值
* 调用:无
* 作者:wyf
* 日期:2010/04/13
* 版本:Version 1.0
* 功能描述:写接口
***************************************************************************
*/
static INT8S MemGetDbfLocInfo(INT8U nBackIdx, INT32U nObjTblIdx, INT32U nObjIdx, INT32U nObjItmTblIdx, INT32U nObjItmIdx, INT32U nHistSize, INT16U nHistIdx, INT32U nPointSize, INT32U nCrvIdx, pMEM_LOC_INFO pInfo)
{
	
	return MemGetLocInfo((nHistSize * nHistIdx + nPointSize * nCrvIdx), nBackIdx, nObjTblIdx, nObjIdx, nObjItmTblIdx, nObjItmIdx, pInfo);
    //return MemGetLocInfo(0, nBackIdx, nObjTblIdx, nObjIdx, nObjItmTblIdx, nObjItmIdx, pInfo);
}

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
    
    // 目前仅是整块表校验:先取最后一行的整块表信息
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
            MemDrvBufSemPend();  //加锁	
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
            MemDrvBufSemPost();	//解锁
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

                    MemDrvBufSemPend();  //加锁	
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
                    MemDrvBufSemPost();	//解锁
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
				MemDrvBufSemPend();  //加锁	
				memset(pData, 0, nDrvSize);
				MemRead(sInfo.pFileName, sInfo.nMedium, sInfo.nAddr, sInfo.nLen + 1, pData);
				if (!MemChkSumChk(pData, sInfo.nLen, pData[sInfo.nLen]))
				{
					MemChkSumGet(pData, sInfo.nLen, &pData[sInfo.nLen]);
					MemWrite(sInfo.pFileName, sInfo.nMedium, sInfo.nAddr + sInfo.nLen, 1, &pData[sInfo.nLen]);
				}
				MemDrvBufSemPost();	//解锁
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
            MemDrvBufSemPend();  //加锁	
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
                MemDrvBufSemPost();	//解锁
                return 0;
            }
            MemDrvBufSemPost();	//解锁
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
    // 目前仅是整块表校验:先取最后一行的整块表信息
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
			MemDrvBufSemPend(); //加锁
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
            MemDrvBufSemPost();	//解锁
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
			
			MemDrvBufSemPend(); //加锁
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
			MemDrvBufSemPost();	//解锁
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
            nExcLen = nLen; /////临时
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
    if (ITEM_COL_END == nItmTblId)														//整张表操作
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
				nMediumCnt++;												//存储介质
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
			MemDrvBufSemPend();  				//加锁
			memset(pData, 0, nDrvSize);
			MemRead(sInfo.pFileName, sInfo.nMedium, sInfo.nAddr, sInfo.nLen + 1, pData);		//读取整张表的数据
			for (nItmTblLoop = 0; nItmTblLoop < nColMaxNum; nItmTblLoop++) //循环读取各列项
            {
                pMemOffInfoTmp = (pMEM_OFF_INFO)(&(pMemInfo->pCol[nItmTblLoop]));
                memset(&sInfo2, 0, sizeof(MEM_LOC_INFO));
                for (nItmLoop = 0; nItmLoop < pMemOffInfoTmp->nItmTblNum; nItmLoop++)
                {
                    if (0 != MemGetDbfLocInfo(i, nTblIdx, nObjIdx, nItmTblLoop, nItmLoop, nHistSize, nHistIdx, nPointSize, nCrvIdx, &sInfo2))
                    {	
                        MemDrvBufSemPost();					//解锁
                        return -1;
                    }
                    if ((sInfo2.nLen + 1) > nDrvBufUnitSize)
                    {
                        MemDrvBufSemPost();					//解锁
                        return -1;
                    }
                    if (nLenTmp >= nLen)										//待写数据处理完毕跳出
                    {
                        break;
                    }
                    memset(pData2, 0, nDrvBufUnitSize);					//读取某列数据:注意多读一个字节
                    MemRead(sInfo2.pFileName, sInfo2.nMedium, sInfo2.nAddr, sInfo2.nLen + 1, pData2);
                    if (pMemOffInfoTmp->bChk)									//某列有校验
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
			MemDrvBufSemPost();	//解锁
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
			MemDrvBufSemPend(); //加锁
			memset(pData, 0, nDrvSize);
			MemRead(sInfo.pFileName, sInfo.nMedium, sInfo.nAddr, sInfo.nLen + 1, pData);
			memcpy(&pData[nOffset], pBuf, nExcLen);
			MemChkSumGet(pData, sInfo.nLen, &pData[sInfo.nLen]);
			MemWrite(sInfo.pFileName, sInfo.nMedium, sInfo.nAddr, sInfo.nLen + 1, pData);
			MemDrvBufSemPost();	//解锁
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
* 函数名:MemReadDbfExt
* 输　入:
* 输　出:
* 返  回:
* 功  能:
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
* 函数名:MemWriteDbfExt
* 输　入:
* 输　出:
* 返  回:
* 功  能:
*********************************************************************************/
INT32S MemWriteDbfExt(INT32U nObjTblId, INT32U nObjId, INT32U nItmTblId, const void *pBuf, INT32U nLen, INT32U* pRetLen)
{
	INT8S nRet = 0;
    nRet = MemWriteDbf(nObjTblId, nObjId, nItmTblId, 0, 0, 0, 0, 0, 0, pBuf, nLen, pRetLen);

	return (INT32S)nRet;
}



/*
***************************************************************************
* 函数名:MemReadHistDataExt
* 输入:
*       INT32U			nObjTblId								数据库表号
*       INT32U 			nObjId									行号
*       INT32U 			nItmTblId								列号
*		INT32U 			nHistSize								一天所有点的所有测量点数据长度
*       INT16U 			nHistIdx								日块指示下标
*		INT32U			nPointSize								一个点的所有测量点曲线长度  曲线时使用
*       INT32U 			nCrvIdx									曲线点(从0起始)
*       void* 			pBuf									缓冲
*       INT32U 			nLen									缓冲长度
* 输出:	
* 返回:RET应用标准返回值
* 调用:无
* 作者:wyf
* 日期:2010/04/13
* 版本:Version 1.0
* 功能描述:读历史数据接口
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
* 函数名:MemWriteHistDataExt
* 输入:
*       INT32U			nObjTblId								数据库表号
*       INT32U 			nObjId									行号
*       INT32U 			nItmTblId								列号
*		INT32U 			nHistSize								一天所有点的所有测量点数据长度
*       INT16U 			nHistIdx								日块指示下标
*		INT32U			nPointSize								一个点的所有测量点曲线长度  曲线时使用
*       INT32U 			nCrvIdx									曲线点(从0起始)
*       void* 			pBuf									缓冲
*       INT32U 			nLen									缓冲长度
* 输出:	
* 返回:RET应用标准返回值
* 调用:无
* 作者:wyf
* 日期:2010/04/13
* 版本:Version 1.0
* 功能描述:读历史数据接口
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


