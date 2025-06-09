/******************************************************************************************************
* Copyright (c) 2013
* 文件名:
* 创建人: 王一凡
* 日　期: 2013/09/12
* 修改人:
* 日　期:
* 描　述:
* 版　本: Version 1.0
*****************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include "Base.h"
#include "Rs485Col.h"
#include "DataItem.h"
#include "Gb07.h"
#include "Gb97.h"
#include "Meter645.h"
#include "SysRes.h"
#include "Fun.h"
#include "SoftTimer.h"
#include "GB3761SetPrmExt.h"


/**************************************************************************************************
                                           宏定义
***************************************************************************************************/

/**************************************************************************************************
                                           变量定义
***************************************************************************************************/
// RS485数据结构:含97与07    暂定两个485接口
static COL_DATA						g_sRs485ColData[2];
// RS485端口序号(AC、485、载波统一定义序号)对应的串口。同时也与RS485数据结构对应
static RS485PORTIDX_COM				g_sRs485PortIdxCom[] = {
					{RS4851_PORT_IDX, COMM_WH4852},
					{RS4852_PORT_IDX, COMM_WH485},

					{PORT_NUM}
};

/**************************************************************************************************
                                           函数定义
***************************************************************************************************/
/********************************************************************************
* 函数名:Rs485ColStop
* 输　入:
*			
* 输　出:
* 返  回:
* 功  能:停止抄表
*********************************************************************************/
INT32S Rs485ColStop(INT8U nRs485PortIdx)
{
	//INT8U			nCom = 0;
	// nRs485Port ------->  nRs485PortIdx
	// nRs485PortIdx ------> nRs485Com
	

	return 0;
}

/********************************************************************************
* 函数名:Rs485ColFlgSet
* 输　入:
*			
* 输　出:
* 返  回:
* 功  能:停止抄表
*********************************************************************************/
INT32S Rs485ColFlgSet(INT8U nRs485PortIdx, INT8U nBit)
{
	//INT8U			nCom = 0;
	// nRs485Port ------->  nRs485PortIdx
	// nRs485PortIdx ------> nRs485Com
	

	return 0;
}

/********************************************************************************
* 函数名:Rs485ColStart
* 输　入:
*			
* 输　出:
* 返  回:
* 功  能:停止抄表
*********************************************************************************/
INT32S Rs485ColStart(INT8U nRs485PortIdx)
{
	//INT8U			nCom = 0;
	// nRs485Port ------->  nRs485PortIdx
	// nRs485PortIdx ------> nRs485Com
	

	return 0;
}




/********************************************************************************
* 函数名:GetRs485DataIdx
* 输　入:
*			INT8U 		nRs485PortIdx			 :    RS4851_PORT_IDX/RS4852_PORT_IDX/RS4853_PORT_IDX
* 输　出:
* 返  回:
小于0，无效；其他为串口号
* 功  能:依据程序里定义的交采、485、载波端口序号获取485数据结构序号
*********************************************************************************/
static INT32S GetRs485DataIdx(INT8U nRs485PortIdx)
{
	pRS485PORTIDX_COM	pRs485PortIdxCom  = NULL;
	INT32S				i 					= 0;

	while(1)
	{
		pRs485PortIdxCom  = (pRS485PORTIDX_COM)&g_sRs485PortIdxCom[i];
		if (PORT_NUM == pRs485PortIdxCom->nRs485PortIdx)
		{
			return -1;
		}
		if (pRs485PortIdxCom->nRs485PortIdx == nRs485PortIdx)
		{
			return i;
		}
		i++;
	}	
}

/********************************************************************************
* 函数名:GetRs485ComFromPortIdx
* 输　入:
*			INT8U 		nRs485PortIdx		 :    RS4851_PORT_IDX/RS4852_PORT_IDX/RS4853_PORT_IDX	
* 输　出:
* 返  回:
小于0，无效；其他为串口号
* 功  能:依据程序里定义的交采、485、载波端口序号获取485的串口
*********************************************************************************/
static INT32S GetRs485ComFromPortIdx(INT8U nRs485PortIdx)
{
	pRS485PORTIDX_COM	pRs485PortIdxCom  = (pRS485PORTIDX_COM)&g_sRs485PortIdxCom[0];
	INT32S 				nRet 				= 0;

	nRet = GetRs485DataIdx(nRs485PortIdx);
	if (nRet < 0)
	{
		return nRet;
	}

	return (INT32S)(pRs485PortIdxCom[nRet].nRs485Com);
}


/********************************************************************************
* 函数名:Rs485RdProc
* 输　入:
*			
* 输　出:
* 返  回:
			0, 抄表成功
			-1,抄表失败
			1,退出抄表
* 功  能:抄表
*********************************************************************************/
static INT32S Rs485RdCmdProc(INT8U nRd485Com, INT8U nProt, INT8U* pAddr, INT32U nDi, INT8U* pData, INT32S nSize)
{
	INT8U				nCtrl					= 0;
	INT32S				i						= 0;
	INT8U				nDiLen					= 0;
	INT32S				nLen					= 0;
    
    SetCommCfg(3, 3, 2);//SetCommCfg(3, 4, 2);

	if (GB_97_PROT == nProt)
	{
		nCtrl = 0x01;
		nDiLen = 2;
	}
	else
	{
		nCtrl = 0x11;
		nDiLen = 4;
	}
	// 具体抄表
	memset(&pData[0], 0, nSize);
	Long2Buf(pData, nDi, nDiLen);
    for (i = 0; i < nDiLen; i++)
    {
        pData[i] += 0x33;
    }
	nLen = nDiLen;
	nLen = Meter645MakeFrm(pData, nLen, nSize, pAddr, nCtrl);
    nLen = Meter645Proc(nRd485Com, pData, nLen, nSize);

	return nLen;
}
extern INT32S CarrRdCmdProc(INT8U nCom, INT8U nProt, INT8U* pAddr, INT32U nDi, INT8U* pData, INT32S nSize);

typedef INT32S (*pRD_PROC_SUB)(INT8U nRdCom, INT8U nProt, INT8U* pAddr, INT32U nDi, INT8U* pData, INT32S nSize);
/********************************************************************************
* 函数名:Rs485RdProcSub
* 输　入:
*			INT8U					nRdCom			抄读串口号
*			INT8U					nProt				协议
*			INT8U* 					pAddr				表地址	
*			INT8U* 					pItemMap			抄表位图	
*			void* 					pData				抄表数据的结构
* 输　出:
* 返  回:
			0, 抄表成功
			-1,抄表失败
			1,退出抄表
* 功  能:抄表
			根据协议规约获取表计信息列表
			获取表计信息列表的某行
			判断是否结尾
			核查是否处于位图表所允许的抄表项
			抄读
			判断表计是否抄读成功
*********************************************************************************/
static INT32S Rs485RdProcSub(INT8U nRdCom, INT16U nSeq, INT8U nProt, INT8U* pAddr, INT8U* pItemMap, void* pData)
{
	INT8U*				pRs485Data				= (INT8U*)pData;
	pCOL_DATA			pColData				= (pCOL_DATA)pData;
	pCOL_07_DATA		pCol07Data				= &pColData->uCol07Data;
	pCOL_97_DATA		pCol97Data				= &pColData->uCol97Data;
	pDATA_ITEM			pDataItemTmp			= NULL;
	pDATA_ITEM			pDataItem				= NULL;
	INT8U				nBuf[300]				= {0};
	INT8U*				pBufTmp					= &nBuf[0];
	INT32S				nSize					= sizeof(nBuf);
	INT32S				nLen					= 0;
	INT32S				nRet					= -1;
	INT8U				nCnt					= 0;
	INT32U				nStdItem				= 0;
	INT8U				nPPEnergyFlg			= 0;
    STD_TIME            sTime;
	pRD_PROC_SUB		pRdProcSub				= NULL;
    
    memset(&sTime, 0, sizeof(sTime));
	
	if (GB_97_PROT == nProt)
	{
		pDataItem = (pDATA_ITEM)(GetGb97IdTbl());
	}
	else
	{
		pDataItem = (pDATA_ITEM)(GetGb07IdTbl());
	}

	if (COMM_CARR_SND_DEBUG == nRdCom)
	{
		pRdProcSub = CarrRdCmdProc;
	}
	else
	{
		pRdProcSub = Rs485RdCmdProc;
	}

	while(1)
	{
		if (COL_END_ITEM == pDataItem->nStdItem)
		{
			break;
		}
		
		// 1. 获取表计信息单元
		pDataItemTmp = pDataItem;
		nStdItem = pDataItemTmp->nStdItem;

		// 2. 下一轮的表计信息单元
		pDataItem += pDataItem->nItemNum;

		// 3. 检查当前表计信息单元是否抄读
		if (IsCurrRdItem(pItemMap, pDataItemTmp->nStdItem) < 0)
		{
			continue;
		}

		// 4. 连续三个大轮数据标识抄读不通，退出抄表
		nCnt++;
		if (nCnt > 3)
		{
			break;
		}

		// 5. 一轮的小项抄读处理
		nPPEnergyFlg = 0;
		while(1)
		{
			OSTimeDlyHMSM(0, 0, 0, 100);
			nLen = pRdProcSub(nRdCom, nProt, pAddr, pDataItemTmp->nProtItem, pBufTmp, nSize);			
			if (nLen >= 0)
			{
				// 5.1 大轮数据标识个数清0
				nCnt = 0;										
				// 5.2 抄读成功，转换至采样数据结构
				Gb645ToColData(nProt, pBufTmp, pDataItemTmp, pRs485Data);
				/* 5.3 指向下一个数据标识: 若是在901F处，直接跳至第6项，在"6. 检查下一个数据标识标准项是否
																				已跳至下一轮数据标准项，若是
																				直接跳出循环"
					处会跳出循环。
				*/
				pDataItemTmp += pDataItemTmp->nItemNum;
				// 5.4 判断是否块抄标志
				if (RD_ZXYGJH == nStdItem)
				{
					// 5.5 块抄成功，置块抄标志. 此处本应直接跳出循环，不过在5.3处已经置了跳出循环的条件
					if (0 == nPPEnergyFlg)
					{
						SetBlockFlg(nSeq, 1);
					}
				}
			}
			else
			{
				// 5.6 指向下一个数据标识:抄读小项
				pDataItemTmp++;
				if (RD_ZXYGJH == nStdItem)
				{
					// 5.7 小项的总抄读不成功，直接跳出循环 
					if (1 == nPPEnergyFlg)
					{
						break;
					}
				}
			}
			nPPEnergyFlg++;
			
			if (COL_END_ITEM == pDataItemTmp->nStdItem)
			{
				break;
			}
			// 6. 检查下一个数据标识标准项是否已跳至下一轮数据标准项，若是直接跳出循环
			if (pDataItemTmp->nStdItem == pDataItem->nStdItem)
			{
				break;
			}
			// 7. 07规约，直接跳出
			if (GB_07_PROT == nProt)
			{
				break;
			}
			// 8. 97表计检查块抄标志，若是块抄标志，直接跳出循环
			if ((RD_DYJH != pDataItemTmp->nStdItem) && (RD_DLJH != pDataItemTmp->nStdItem))
			{
				if (GetBlockFlg(nSeq))
				{
					break;
				}
			}
		}

		// 8. 判断抄表是否成功
		if (nLen < 0)
		{
			if (RD_ZXYGJH == nStdItem)
			{
				break;
			}
		}
	}

	if (GB_97_PROT == nProt)
	{
		if (IsAllBcd(&pCol97Data->nEnergyPP[0][0], 4))
		{
            //记录抄表时间
            GetSysClk(&sTime);
            memcpy(&pCol97Data->sReadTime, &sTime, sizeof(sTime));
			nRet = 0;
		}
	}
	else
	{
		// 日数据与实时数据
		if ((IsAllBcd(&pCol07Data->nDEnergyPP[0][0], 4)) || (IsAllBcd(&pCol07Data->nEnergyPP[0][0], 4)))
		{
            //记录抄表时间
            GetSysClk(&sTime);
            memcpy(&pCol07Data->sReadTime, &sTime.nSec, 6);
			nRet = 0;
		}
	}
	
	return nRet;
}


/********************************************************************************
* 函数名:Rs485RdProc
* 输　入:
*			INT16U					nSeq				表计序号
*			INT8U					nCom				具体485序号对应的串口号
*			INT8U					nChkPort			具体485序号对应的端口号   通过判断nSeq序号的表计参数的端口号是否与此一致，来确定是否此序号485的表计
*			void* 					pData				抄表数据的结构
* 输　出:
* 返  回:
			0, 抄表成功
			-1,抄表失败
* 功  能:单个表计抄表处理 :
		根据seq获得表计信息
		检查标记端口是否与nChkPort一致
		检查nRdFlags是否为0
		获取表计位图
		抄读表计数据项
		存储
*********************************************************************************/
// static 型
INT32S Rs485RdProc(INT16U nSeq, INT8U nCom, INT8U nChkPort, void* pData)
{
	INT32S				nRet							= 0;
	METER_INFO			sMeterInfo;
	pMETER_INFO			pMeterInfo						= &sMeterInfo;
	pCOL_DATA			pRs485ColData					= (pCOL_DATA)pData;
	INT8U           	bsItemMap[DATA_ITEM_MAP_LEN] 	= {0};
    STD_TIME            sTime;
    INT8U               nErr                            = 0;
    INT32S              nBaud                           = 0;
    INT8U               nIsRs485                        = 1;
    
    memset(&sTime, 0, sizeof(sTime));
    
    if (g_nRs485ErrFlg)
    {
        return -1;
    }
    
	// 若是停抄，会停止pMeterInfo->nRdFlgs，清为0
	if ((nBaud = GetMeterInfo(nSeq, pMeterInfo)) < 0)
	{
		return -1;
	}
	// 表计端口号与实际的端口号不匹配,不是此485的表计
	if (nChkPort != pMeterInfo->nPort)
	{
		return -1;
	}
	// 停抄时判断到nRdFlgs为0即退出  若是停抄，pMeterInfo->nRdFlgs为0,bsItemMap就为0
	if (0 == pMeterInfo->nRdFlgs)
	{
		return -1;
	}
    
    if (nBaud == 2400)
    {
        if (pMeterInfo->nProt != GB_07_PROT)
        {
            return -1;
        }
    }
    else if (nBaud == 1200)
    {
        if (pMeterInfo->nProt != GB_97_PROT)
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
	
    //
    if (31 == pMeterInfo->nPort)
    {
        nIsRs485 = 0;
    }
	// 获取抄表位图
	GetItemMap(pMeterInfo->nRdFlgs, pMeterInfo->nImUser, nIsRs485, &bsItemMap[0]);
	if (IsAllEqualVal(&bsItemMap[0], DATA_ITEM_MAP_LEN, 0))
	{
		return -1;
	}

	// 具体抄读表计数据项
	memset(pRs485ColData, INVLD_DATA, sizeof(COL_DATA));
	nRet = Rs485RdProcSub(nCom, nSeq, pMeterInfo->nProt, &pMeterInfo->nMeterAddr[0], &bsItemMap[0], pRs485ColData);
	// 抄表成功
	if (nRet == 0)
	{
		// 转化、存储
		// 实时
		if (TestBufBit(&pMeterInfo->nRdFlgs, 1, RD_REAL))
		{
            OSSemPend(g_pSem[SEM_STORE_BUF_IDX], 0, &nErr);
			if (RET_ERR_OK == RealDataMakeAndStore(pRs485ColData, nSeq))
            {
                ClrRdMeterFlags(nSeq, RD_REAL);
            }
            OSSemPost(g_pSem[SEM_STORE_BUF_IDX]);
		}
		// 日
		if (TestBufBit(&pMeterInfo->nRdFlgs, 1, RD_DAY))
		{
            OSSemPend(g_pSem[SEM_STORE_BUF_IDX], 0, &nErr);
			if (RET_ERR_OK == DayDataMakeAndStore(pRs485ColData, nSeq, 1))
            {
                ClrRdMeterFlags(nSeq, RD_DAY);
            }
            OSSemPost(g_pSem[SEM_STORE_BUF_IDX]);
		} 
		// 月
		if (TestBufBit(&pMeterInfo->nRdFlgs, 1, RD_MON))
		{
            OSSemPend(g_pSem[SEM_STORE_BUF_IDX], 0, &nErr);
			if (RET_ERR_OK == MonDataMakeAndStore(pRs485ColData, nSeq, 1))
            {
                ClrRdMeterFlags(nSeq, RD_MON);
            }
            OSSemPost(g_pSem[SEM_STORE_BUF_IDX]);
		} 
        // 曲线
		if (TestBufBit(&pMeterInfo->nRdFlgs, 1, RD_CRV))
		{
            OSSemPend(g_pSem[SEM_STORE_BUF_IDX], 0, &nErr);
			if (RET_ERR_OK == CrvDataMakeAndStore(pRs485ColData, nSeq))
            {
                ClrRdMeterFlags(nSeq, RD_CRV);
            }
            OSSemPost(g_pSem[SEM_STORE_BUF_IDX]);
		} 
		// 抄表日
		if (TestBufBit(&pMeterInfo->nRdFlgs, 1, RD_RDAY))
		{
            OSSemPend(g_pSem[SEM_STORE_BUF_IDX], 0, &nErr);
			if (RET_ERR_OK == RdDataMakeAndStore(pRs485ColData, nSeq))
            {
                ClrRdMeterFlags(nSeq, RD_RDAY);
            }
            OSSemPost(g_pSem[SEM_STORE_BUF_IDX]);
		} 
	}
	else if (nRet < 0)
	{
		//抄表失败
        //ClrRdMeterFlags(nSeq, RD_REAL);
        //ClrRdMeterFlags(nSeq, RD_DAY);
        //ClrRdMeterFlags(nSeq, RD_MON);
	}
	return 0;
}

/********************************************************************************
* 函数名:TaskRs485ColProc
* 输　入:
*			INT8U			nRs485PortIdx  :    RS4851_PORT_IDX/RS4852_PORT_IDX/RS4853_PORT_IDX
* 输　出:
* 返  回:
* 功  能:抄表处理 :
					根据nRs485PortIdx，获取属于485序号自身的串口号、规约端口号、数据采集结构指针
*********************************************************************************/
static INT32S TaskRs485ColProc(INT8U nRs485PortIdx)
{
	INT32S				nRet			= 0;
	INT32S				i 				= 0;
	INT8U				nDataIdx		= 0;
	INT8U				nPort			= 0;
	INT8U				nCom			= 0;
	pCOL_DATA			pRs485ColData	= NULL;

	//获取待存储的485采样数据结构指针
	nRet = GetRs485DataIdx(nRs485PortIdx);
	if (nRet < 0)
	{
		return -1;
	}
	nDataIdx = (INT8U)nRet;
	pRs485ColData = (pCOL_DATA)&g_sRs485ColData[nDataIdx];

	//获取485的对应串口号
	nRet = GetRs485ComFromPortIdx(nRs485PortIdx);
	if (nRet < 0)
	{
		return -1;
	}
	nCom  = (INT8U)nRet;

	//获取对应的485端口号
	nRet = GetPortIdx(nRs485PortIdx);	
	if (nRet < 0)
	{
		return -1;
	}
	nPort = (INT8U)nRet;
    
	for (i = 0; i < MAX_METER_NUM; i++)
	{
		Rs485RdProc(i + 1, nCom, nPort, pRs485ColData);
	}

	return 0;
}

#if 0
#endif
/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: Rs485采集初始化
*********************************************************************************/
INT32S TaskRs485ColInit(INT8U nRs485PortIdx)
{

	return 0;
}


/********************************************************************************
* 函数名:
* 输　入:
* 输　出:
* 返  回:
* 功  能: Rs485采集任务
*********************************************************************************/
void Rs485ColTask(void *pData)
{
	INT8U 		nErr 			= 0;
	INT8U 		nRs485PortIdx	= 0;
	INT8U		nPtr			= 0;

	nRs485PortIdx = (INT8U)((INT32U)(pData));
    if (TaskRs485ColInit(nRs485PortIdx) < 0)
	{
		return;
	}
	if (RS4851_PORT_IDX == nRs485PortIdx)
	{
		nPtr = 0;
	}
	else
	{
		nPtr = 1;
	}
	for (; ;)
	{
		OSSemPend(g_pSem[SEM_RS485_1_IDX + nPtr], 0, &nErr);
        //OSTimeDlyHMSM( 0,0,0,200);
        TaskRs485ColProc(nRs485PortIdx);
		OSTimeDlyHMSM(0, 0, 0, 100);
        //SysSetTaskRunOnceTOS(TASK_RS485_1_COL_IDX + nPtr);
	}
}









