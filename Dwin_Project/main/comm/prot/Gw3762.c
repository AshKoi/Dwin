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
#include "AppHead.h"
#include "Base.h"
#include "Gw3762.h"
  
static INT8U	g_nGw3762Seq = 0;

/**************************************************************************************************
                                           ��غ���ʵ��
***************************************************************************************************/
/*
*****************************************************************************
* ������: Gb3762ProtMakeFrm
* �䡡��: 
*		INT8U*		pAddr
*		INT8U* 		pData						����
*		INT32S 		nDatalen					���ݳ���
*		INT32S 		nMaxDatalen
*		INT8U 		nDir						������:����1������0
*		INT8U  		nPrm						�����Ӷ�:PRM =1����ʾ��֡������������վ��PRM =0����ʾ��֡�������ԴӶ�վ
*		INT8U 		nModl						�ز�1��С����0
*		INT8U 		nModlSel					ͨ��ģ���ʶ:���ز�ģ�鱾�����Ϊ0
*		INT8U 		nPhase						��λ
*		INT8U 		nAfn
*		INT16U 		nFn
* ��  ��: 
*		INT8U* 		pData
* ��  ��: 
*		
*       
*       
* ��  ��: ��֡  ������֡����
*****************************************************************************
*/
INT32S Gb3762ProtMakeFrm(INT8U* pAddr, INT8U* pData, INT32S nDatalen, INT32S nMaxDatalen, INT8U nDir, INT8U nPrm, INT8U nModl, INT8U nModlSel, INT8U nPhase, INT8U nAfn, INT16U nFn)
{
	pGW3762_PROT		pGw3762Prot 	= (pGW3762_PROT)pData;
	pGW3762_PROT_SUB	pGw3762ProtSub	= NULL;
	INT8U				nIdx			= 0;
	INT8U				nAddr[6]		= {0};
	
	if (NULL == pData)
	{
		return -1;
	}
	if (nDatalen < 0)
	{
		return -1;
	}
	if (GW3762_PROT_HEAD_TAIL_LEN + nDatalen > nMaxDatalen)
	{
		return -1;
	}
	pGw3762ProtSub = (pGW3762_PROT_SUB)(pGw3762Prot->nAppData);
	if (nModlSel)
	{
		nIdx = GW3762_PROT_APP_ADDR_LEN;
		if (NULL != pAddr)
		{
			memcpy(&nAddr[0], pAddr, 6);
		}
	}
	memmove(&pGw3762ProtSub->nUseData[nIdx + 3], pData, nDatalen);
	memset(&pGw3762ProtSub->nInfo[0], 0, 6);
	memset(&pGw3762ProtSub->nUseData[0], 0, nIdx + 3);
	nDatalen += 6 + nIdx + 3;									// �û������ݳ���						
	
	pGw3762Prot->nHead	= 0x68;
	INT16U2Buf(&pGw3762Prot->nFrmLen[0], nDatalen + 6);		// �û��� + head + L + c + cs + tail
	pGw3762Prot->nCtrl = 0;
	if (nDir)
	{
		SetBufBit(&pGw3762Prot->nCtrl, 1, 7);					// ������
	}
	if (nPrm)													// �����Ӷ�
	{
		SetBufBit(&pGw3762Prot->nCtrl, 1, 6);
	}
	if (nModl)													// �ز���С����
	{
		SetBufBit(&pGw3762Prot->nCtrl, 1, 0);
	}
	else
	{
		pGw3762Prot->nCtrl |= 0x0a;
	}

	pGw3762ProtSub->nInfo[1] = nPhase;							// �ŵ���ʶ: ��λ
	pGw3762ProtSub->nInfo[5] = g_nGw3762Seq;
	(g_nGw3762Seq >= 0xff) ? (g_nGw3762Seq = 0) : (g_nGw3762Seq += 1);
	if (nModlSel)												// ͨ��ģ���ʶ:0��ʾ�����ڵ�Ĳ�����1��ʾ�Դӽڵ����
	{
		SetBufBit(&pGw3762ProtSub->nInfo[0], 1, 2);	
		memcpy(&pGw3762ProtSub->nUseData[6], &nAddr[0], 6);		//Ŀ�ĵ�ַA3
		//memcpy(&pGw3762Prot->nAppDataBuff[0], (INT8U*)&pFrmInfo->nA1, 6);  //Դ��ַA1
        //memcpy(&pGw3762Prot->nAppDataBuff[0], (INT8U*)&pFrmInfo->nA3, LEN_NODE_ADDR);  //Ŀ�ĵ�ַA3
	}
	pGw3762ProtSub->nUseData[nIdx] = nAfn;
	pGw3762ProtSub->nUseData[nIdx + 1] = 1 << (nFn % 8);
    pGw3762ProtSub->nUseData[nIdx + 2] = nFn / 8;

	pGw3762Prot->nAppData[nDatalen] = CheckSum(pData, 3, nDatalen + 1);
	pGw3762Prot->nAppData[nDatalen + 1] = 0x16;
	
	return (6 + nDatalen);
}




/**************************************************************************************************
                                           ���ýӿ�
***************************************************************************************************/
/*
*****************************************************************************
* ������: Gw3762FrmCheck
* �䡡��: 
*			INT8U*			pData
*			INT32S			nDatalen			���յ��ı��ĳ���
* ��  ��: 
*			INT8U*			pData 				���������֡�����¸��Ƶ� pData 
* ��  ��: ��������֡�ĳ���
* ��  ��: 376.2��Լ����
*****************************************************************************
*/
INT32S Gw3762FrmCheck(INT8U* pData, INT32S nDatalen)
{
	INT32S	nCycCnt 	= (nDatalen - 15 + 1);
	INT32S	i       	= 0;
	INT16U	nFrmlen1 	= 0;
	INT8U	nCs			= 0;

	if (NULL == pData)
	{
		return -1;
	}
	if (nDatalen < 15)
	{
		return -1;
	}
	for (i = 0; i < nCycCnt; i++)
	{
		if (0x68 != pData[i])
		{
			continue;
		}
		nFrmlen1 = Buf2INT16U(&pData[i + 1]);
		if (nFrmlen1 < 5)
		{
			continue;
		}
		else if (nFrmlen1 + i > nDatalen)
		{
			continue;
		}
		
		nCs = CheckSum(&pData[i], 3, nFrmlen1 - 5);
		if (nCs != pData[i + nFrmlen1 - 2])
		{
			continue;
		}
		if (0x16 != pData[i + nFrmlen1 - 1])
		{
			continue;
		}
		
		break;
	}
	if (i >= nCycCnt)
	{
		return -1;
	}
	
	memmove(pData, &pData[i], nFrmlen1);
	
	return (INT32S)nFrmlen1;
}


#define GW3762FRM_CHECK_DATA 										\
	INT8U				nAfn			= 0;						\
	INT16U				nFn				= 0;						\
	INT8U				nIdx			= 0;						\
	pGW3762_PROT		pGw3762Prot 	= (pGW3762_PROT)pData;		\
	pGW3762_PROT_SUB	pGw3762ProtSub	= NULL;						\
	pGw3762ProtSub = (pGW3762_PROT_SUB)(pGw3762Prot->nAppData);		\
	if (TestBufBit(&pGw3762ProtSub->nInfo[0], 1, 2))				\
	{																\
		nIdx = 12;													\
	}																\
	nAfn = pGw3762ProtSub->nUseData[nIdx];							\
	nFn	 = pGw3762ProtSub->nUseData[nIdx + 2] * 8;					\
	for (i = 0; i < 8; i++)											\
	{																\
		if (pGw3762ProtSub->nUseData[nIdx + 1] & (1 << i))			\
		{															\
			nFn += i;												\
			break;													\
		}															\
	}
	

/*
*****************************************************************************
* ������: Gw3762FrmCheckData
* �䡡��: 
*			INT8U*			pData
*			INT32S			nDatalen			���յ��ı��ĳ���
* ��  ��: 
*			INT8U*			pData 				���������֡�����¸��Ƶ� pData 
* ��  ��: 
* ��  ��: 376.2��Լ����
*****************************************************************************
*/
INT32S Gw3762FrmCheckData(INT8U* pData, INT32S nDatalen, INT8U nAfnD, INT16U nFnD)
{
	INT8U				i				= 0;

	if (NULL == pData)
	{
		return -1;
	}
	if (nDatalen < 15)
	{
		return -1;
	}
	GW3762FRM_CHECK_DATA ;
	if (i >= 8)
	{
		return -1;
	}
	
	if (0 == nAfn)
	{
		if (nFn > F2)
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		if ((nAfn != nAfnD) || (nFn != nFnD))
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}
}

/*
*****************************************************************************
* ������: Gw3762Proc
* �䡡��: 		
*			INT8U*			pData
*			INT32S			nDatalen			֡���ݳ���
*			INT32S			nMaxDatalen			���յ��ı��ĳ���
* ��  ��: 
*			INT8U*			pData 				����֡
* ��  ��: ��������֡�ĳ���
* ��  ��: 376.2��Լ����
*****************************************************************************
*/
INT32S Gw3762Proc(INT8U* pData, INT32S nDatalen, INT32S nMaxDatalen)
{
	INT8U	nCom		= COMM_ZB; 
    INT32S  nLen   	 	= 0;
	INT16U	i 			= 0;
	INT32S	nRet		= 0;
	const INT8U	g_nGw3762AckAfn[] = {0x01, 0x04, 0x05, 0x11, 0x12};
	
	if ((NULL == pData) || (nDatalen < 0) || (nMaxDatalen <= 0))
	{
		return -1;
	}
    SetCommCfg(4, 6, 2);  

	ProcCommCfgPend(nCom);
	
	WriteComm(nCom, pData, nDatalen);
	
	GW3762FRM_CHECK_DATA ;

	for (i = 0; i < sizeof(g_nGw3762AckAfn); i++)
	{
		if (nAfn == g_nGw3762AckAfn[i])
		{
			nAfn = 0x00;
			nFn = F1;
			break;
		}
	}
	
	//for (i = 0; i < 10; i++)
    for (i = 0; i < 25; i++)
	{
		memset(pData, 0, nMaxDatalen);
		nLen = ReadCommMs(nCom, pData, nMaxDatalen, 500, 2000/*100, 300*/);
        nLen = Gw3762FrmCheck(pData, nLen);
        if (nLen >= 0)
        {
            if ((pGw3762Prot->nCtrl & 0x80) != 0x80)
            {
                nLen = -1;
            }
            nRet = Gw3762FrmCheckData(pData, nLen, nAfn, nFn);
            break;
        }
	}
	if (i >= 25)
	{
		nLen = -1;
	}
	ProcCommCfgPost(nCom);
	return nLen;
}


