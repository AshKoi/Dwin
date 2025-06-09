/******************************************************************************************************
* Copyright (c) 2013
* �ļ���:
* ������: ��һ��
* �ա���: 2013/09/12
* �޸���:
* �ա���:
* �衡��:
* �桡��: Version 1.0
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
                                           �궨��
***************************************************************************************************/

/**************************************************************************************************
                                           ��������
***************************************************************************************************/
// RS485���ݽṹ:��97��07    �ݶ�����485�ӿ�
static COL_DATA						g_sRs485ColData[2];
// RS485�˿����(AC��485���ز�ͳһ�������)��Ӧ�Ĵ��ڡ�ͬʱҲ��RS485���ݽṹ��Ӧ
static RS485PORTIDX_COM				g_sRs485PortIdxCom[] = {
					{RS4851_PORT_IDX, COMM_WH4852},
					{RS4852_PORT_IDX, COMM_WH485},

					{PORT_NUM}
};

/**************************************************************************************************
                                           ��������
***************************************************************************************************/
/********************************************************************************
* ������:Rs485ColStop
* �䡡��:
*			
* �䡡��:
* ��  ��:
* ��  ��:ֹͣ����
*********************************************************************************/
INT32S Rs485ColStop(INT8U nRs485PortIdx)
{
	//INT8U			nCom = 0;
	// nRs485Port ------->  nRs485PortIdx
	// nRs485PortIdx ------> nRs485Com
	

	return 0;
}

/********************************************************************************
* ������:Rs485ColFlgSet
* �䡡��:
*			
* �䡡��:
* ��  ��:
* ��  ��:ֹͣ����
*********************************************************************************/
INT32S Rs485ColFlgSet(INT8U nRs485PortIdx, INT8U nBit)
{
	//INT8U			nCom = 0;
	// nRs485Port ------->  nRs485PortIdx
	// nRs485PortIdx ------> nRs485Com
	

	return 0;
}

/********************************************************************************
* ������:Rs485ColStart
* �䡡��:
*			
* �䡡��:
* ��  ��:
* ��  ��:ֹͣ����
*********************************************************************************/
INT32S Rs485ColStart(INT8U nRs485PortIdx)
{
	//INT8U			nCom = 0;
	// nRs485Port ------->  nRs485PortIdx
	// nRs485PortIdx ------> nRs485Com
	

	return 0;
}




/********************************************************************************
* ������:GetRs485DataIdx
* �䡡��:
*			INT8U 		nRs485PortIdx			 :    RS4851_PORT_IDX/RS4852_PORT_IDX/RS4853_PORT_IDX
* �䡡��:
* ��  ��:
С��0����Ч������Ϊ���ں�
* ��  ��:���ݳ����ﶨ��Ľ��ɡ�485���ز��˿���Ż�ȡ485���ݽṹ���
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
* ������:GetRs485ComFromPortIdx
* �䡡��:
*			INT8U 		nRs485PortIdx		 :    RS4851_PORT_IDX/RS4852_PORT_IDX/RS4853_PORT_IDX	
* �䡡��:
* ��  ��:
С��0����Ч������Ϊ���ں�
* ��  ��:���ݳ����ﶨ��Ľ��ɡ�485���ز��˿���Ż�ȡ485�Ĵ���
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
* ������:Rs485RdProc
* �䡡��:
*			
* �䡡��:
* ��  ��:
			0, ����ɹ�
			-1,����ʧ��
			1,�˳�����
* ��  ��:����
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
	// ���峭��
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
* ������:Rs485RdProcSub
* �䡡��:
*			INT8U					nRdCom			�������ں�
*			INT8U					nProt				Э��
*			INT8U* 					pAddr				���ַ	
*			INT8U* 					pItemMap			����λͼ	
*			void* 					pData				�������ݵĽṹ
* �䡡��:
* ��  ��:
			0, ����ɹ�
			-1,����ʧ��
			1,�˳�����
* ��  ��:����
			����Э���Լ��ȡ�����Ϣ�б�
			��ȡ�����Ϣ�б��ĳ��
			�ж��Ƿ��β
			�˲��Ƿ���λͼ��������ĳ�����
			����
			�жϱ���Ƿ񳭶��ɹ�
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
		
		// 1. ��ȡ�����Ϣ��Ԫ
		pDataItemTmp = pDataItem;
		nStdItem = pDataItemTmp->nStdItem;

		// 2. ��һ�ֵı����Ϣ��Ԫ
		pDataItem += pDataItem->nItemNum;

		// 3. ��鵱ǰ�����Ϣ��Ԫ�Ƿ񳭶�
		if (IsCurrRdItem(pItemMap, pDataItemTmp->nStdItem) < 0)
		{
			continue;
		}

		// 4. ���������������ݱ�ʶ������ͨ���˳�����
		nCnt++;
		if (nCnt > 3)
		{
			break;
		}

		// 5. һ�ֵ�С�������
		nPPEnergyFlg = 0;
		while(1)
		{
			OSTimeDlyHMSM(0, 0, 0, 100);
			nLen = pRdProcSub(nRdCom, nProt, pAddr, pDataItemTmp->nProtItem, pBufTmp, nSize);			
			if (nLen >= 0)
			{
				// 5.1 �������ݱ�ʶ������0
				nCnt = 0;										
				// 5.2 �����ɹ���ת�����������ݽṹ
				Gb645ToColData(nProt, pBufTmp, pDataItemTmp, pRs485Data);
				/* 5.3 ָ����һ�����ݱ�ʶ: ������901F����ֱ��������6���"6. �����һ�����ݱ�ʶ��׼���Ƿ�
																				��������һ�����ݱ�׼�����
																				ֱ������ѭ��"
					��������ѭ����
				*/
				pDataItemTmp += pDataItemTmp->nItemNum;
				// 5.4 �ж��Ƿ�鳭��־
				if (RD_ZXYGJH == nStdItem)
				{
					// 5.5 �鳭�ɹ����ÿ鳭��־. �˴���Ӧֱ������ѭ����������5.3���Ѿ���������ѭ��������
					if (0 == nPPEnergyFlg)
					{
						SetBlockFlg(nSeq, 1);
					}
				}
			}
			else
			{
				// 5.6 ָ����һ�����ݱ�ʶ:����С��
				pDataItemTmp++;
				if (RD_ZXYGJH == nStdItem)
				{
					// 5.7 С����ܳ������ɹ���ֱ������ѭ�� 
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
			// 6. �����һ�����ݱ�ʶ��׼���Ƿ���������һ�����ݱ�׼�����ֱ������ѭ��
			if (pDataItemTmp->nStdItem == pDataItem->nStdItem)
			{
				break;
			}
			// 7. 07��Լ��ֱ������
			if (GB_07_PROT == nProt)
			{
				break;
			}
			// 8. 97��Ƽ��鳭��־�����ǿ鳭��־��ֱ������ѭ��
			if ((RD_DYJH != pDataItemTmp->nStdItem) && (RD_DLJH != pDataItemTmp->nStdItem))
			{
				if (GetBlockFlg(nSeq))
				{
					break;
				}
			}
		}

		// 8. �жϳ����Ƿ�ɹ�
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
            //��¼����ʱ��
            GetSysClk(&sTime);
            memcpy(&pCol97Data->sReadTime, &sTime, sizeof(sTime));
			nRet = 0;
		}
	}
	else
	{
		// ��������ʵʱ����
		if ((IsAllBcd(&pCol07Data->nDEnergyPP[0][0], 4)) || (IsAllBcd(&pCol07Data->nEnergyPP[0][0], 4)))
		{
            //��¼����ʱ��
            GetSysClk(&sTime);
            memcpy(&pCol07Data->sReadTime, &sTime.nSec, 6);
			nRet = 0;
		}
	}
	
	return nRet;
}


/********************************************************************************
* ������:Rs485RdProc
* �䡡��:
*			INT16U					nSeq				������
*			INT8U					nCom				����485��Ŷ�Ӧ�Ĵ��ں�
*			INT8U					nChkPort			����485��Ŷ�Ӧ�Ķ˿ں�   ͨ���ж�nSeq��ŵı�Ʋ����Ķ˿ں��Ƿ����һ�£���ȷ���Ƿ�����485�ı��
*			void* 					pData				�������ݵĽṹ
* �䡡��:
* ��  ��:
			0, ����ɹ�
			-1,����ʧ��
* ��  ��:������Ƴ����� :
		����seq��ñ����Ϣ
		����Ƕ˿��Ƿ���nChkPortһ��
		���nRdFlags�Ƿ�Ϊ0
		��ȡ���λͼ
		�������������
		�洢
*********************************************************************************/
// static ��
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
    
	// ����ͣ������ֹͣpMeterInfo->nRdFlgs����Ϊ0
	if ((nBaud = GetMeterInfo(nSeq, pMeterInfo)) < 0)
	{
		return -1;
	}
	// ��ƶ˿ں���ʵ�ʵĶ˿ںŲ�ƥ��,���Ǵ�485�ı��
	if (nChkPort != pMeterInfo->nPort)
	{
		return -1;
	}
	// ͣ��ʱ�жϵ�nRdFlgsΪ0���˳�  ����ͣ����pMeterInfo->nRdFlgsΪ0,bsItemMap��Ϊ0
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
	// ��ȡ����λͼ
	GetItemMap(pMeterInfo->nRdFlgs, pMeterInfo->nImUser, nIsRs485, &bsItemMap[0]);
	if (IsAllEqualVal(&bsItemMap[0], DATA_ITEM_MAP_LEN, 0))
	{
		return -1;
	}

	// ���峭�����������
	memset(pRs485ColData, INVLD_DATA, sizeof(COL_DATA));
	nRet = Rs485RdProcSub(nCom, nSeq, pMeterInfo->nProt, &pMeterInfo->nMeterAddr[0], &bsItemMap[0], pRs485ColData);
	// ����ɹ�
	if (nRet == 0)
	{
		// ת�����洢
		// ʵʱ
		if (TestBufBit(&pMeterInfo->nRdFlgs, 1, RD_REAL))
		{
            OSSemPend(g_pSem[SEM_STORE_BUF_IDX], 0, &nErr);
			if (RET_ERR_OK == RealDataMakeAndStore(pRs485ColData, nSeq))
            {
                ClrRdMeterFlags(nSeq, RD_REAL);
            }
            OSSemPost(g_pSem[SEM_STORE_BUF_IDX]);
		}
		// ��
		if (TestBufBit(&pMeterInfo->nRdFlgs, 1, RD_DAY))
		{
            OSSemPend(g_pSem[SEM_STORE_BUF_IDX], 0, &nErr);
			if (RET_ERR_OK == DayDataMakeAndStore(pRs485ColData, nSeq, 1))
            {
                ClrRdMeterFlags(nSeq, RD_DAY);
            }
            OSSemPost(g_pSem[SEM_STORE_BUF_IDX]);
		} 
		// ��
		if (TestBufBit(&pMeterInfo->nRdFlgs, 1, RD_MON))
		{
            OSSemPend(g_pSem[SEM_STORE_BUF_IDX], 0, &nErr);
			if (RET_ERR_OK == MonDataMakeAndStore(pRs485ColData, nSeq, 1))
            {
                ClrRdMeterFlags(nSeq, RD_MON);
            }
            OSSemPost(g_pSem[SEM_STORE_BUF_IDX]);
		} 
        // ����
		if (TestBufBit(&pMeterInfo->nRdFlgs, 1, RD_CRV))
		{
            OSSemPend(g_pSem[SEM_STORE_BUF_IDX], 0, &nErr);
			if (RET_ERR_OK == CrvDataMakeAndStore(pRs485ColData, nSeq))
            {
                ClrRdMeterFlags(nSeq, RD_CRV);
            }
            OSSemPost(g_pSem[SEM_STORE_BUF_IDX]);
		} 
		// ������
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
		//����ʧ��
        //ClrRdMeterFlags(nSeq, RD_REAL);
        //ClrRdMeterFlags(nSeq, RD_DAY);
        //ClrRdMeterFlags(nSeq, RD_MON);
	}
	return 0;
}

/********************************************************************************
* ������:TaskRs485ColProc
* �䡡��:
*			INT8U			nRs485PortIdx  :    RS4851_PORT_IDX/RS4852_PORT_IDX/RS4853_PORT_IDX
* �䡡��:
* ��  ��:
* ��  ��:������ :
					����nRs485PortIdx����ȡ����485�������Ĵ��ںš���Լ�˿ںš����ݲɼ��ṹָ��
*********************************************************************************/
static INT32S TaskRs485ColProc(INT8U nRs485PortIdx)
{
	INT32S				nRet			= 0;
	INT32S				i 				= 0;
	INT8U				nDataIdx		= 0;
	INT8U				nPort			= 0;
	INT8U				nCom			= 0;
	pCOL_DATA			pRs485ColData	= NULL;

	//��ȡ���洢��485�������ݽṹָ��
	nRet = GetRs485DataIdx(nRs485PortIdx);
	if (nRet < 0)
	{
		return -1;
	}
	nDataIdx = (INT8U)nRet;
	pRs485ColData = (pCOL_DATA)&g_sRs485ColData[nDataIdx];

	//��ȡ485�Ķ�Ӧ���ں�
	nRet = GetRs485ComFromPortIdx(nRs485PortIdx);
	if (nRet < 0)
	{
		return -1;
	}
	nCom  = (INT8U)nRet;

	//��ȡ��Ӧ��485�˿ں�
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
* ������:
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: Rs485�ɼ���ʼ��
*********************************************************************************/
INT32S TaskRs485ColInit(INT8U nRs485PortIdx)
{

	return 0;
}


/********************************************************************************
* ������:
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: Rs485�ɼ�����
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









