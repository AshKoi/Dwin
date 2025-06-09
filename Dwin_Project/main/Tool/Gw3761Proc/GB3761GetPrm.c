/******************************************************************************************************
* Copyright (c) 2010-2011 ��һ�Ƽ��ɷ����޹�˾�з�����
* �ļ���:
* ������: �з�����Ӧ�ÿ�����
* �ա���: 2011/01/24
* �޸���:
* �ա���:
* �衡��:
* �桡��: Version 1.0
*****************************************************************************************************/
#include "GB3761GetPrm.h"
#include "Gw3761.h"
#include "MemDrvExt.h"
#include "Table.h"
#include "Fun.h"
#include "Macro.h"
#include "SysRes.h"
#include "Base.h"
#include "ChkCrc.h"
#include "Reset.h"
#include "MemData.h"
#include "SoftTimer.h"
#include "rmotlink.h"
#include "MemTbl.h"

/**************************************************************************************************
                                           ��غ���ʵ��
***************************************************************************************************/
/********************************************************************************
* ������: GB3761GetParam
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ������ѯ
*********************************************************************************/
INT8S GB3761GetParam(pAPP_COMM pComm)
{
	INT16U 		        nPn[8];
	INT16U 		        nFn[GB3761_MAX_FN_NUM];
	INT16U		        nPnNum      = 0;
	INT8U		        nFnNum      = 0;
	INT16U  	        nOffset     = 0;
	pGB3761_FRM_INFO    pInfo       = NULL;

	if (NULL == pComm)
    {
    	return RET_ERR_POINT;
    }

    pInfo = (pGB3761_FRM_INFO)pComm->pFrmInfo;   //��ȡ����֡��Ϣ��ָ��
    while (nOffset + 4 <= pInfo->nRxDataLen)
	{
		memset(&nPn[0], 0xFFFF, sizeof(nPn));
        memset(&nFn[0], 0, sizeof(nFn));
        if (RET_ERR_OK != GB3761DaDt2PnFn(&pComm->pRxFrmBuf[GB3761_DATA_IDX + nOffset], &nPn[0], &nPnNum, &nFn[0], &nFnNum, &pInfo->bAllMpFlag))  //Dadtת��Pnfn
        {
	        break;
	    }
        nOffset += 4;            //�Ѿ��������ݱ�ʶ�����ݵ�Ԫ����
        if (RET_ERR_OK != GB3761GetParamProc(pComm, &nPn[0], nPnNum, &nFn[0], nFnNum, &nOffset))
        {
            break;
        }
	}
	return RET_ERR_OK;
}

/********************************************************************************
* ������: GB3761GetParamProc
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ������ѯ
*********************************************************************************/
INT8S GB3761GetParamProc(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset)
{
	INT16U    	            i           = 0;
	INT16U    	            j           = 0;
	INT16U		            nDataLen    = 0;
	INT8U     	            nData[200];                 //g_nProtTmpData 1024->200
    INT16U                  nMaxDataLen = 0;
	pGB3761_FRM_INFO        pInfo       = NULL;

    if (NULL == pComm || NULL == pPn || NULL == pFn || NULL == pOffset)
    {
    	return RET_ERR_POINT;
    }

    pInfo = (pGB3761_FRM_INFO)(pComm->pFrmInfo);
    nMaxDataLen = pComm->nFrmLen - GB3761_FRM_RES_LEN;
	for (i = 0; i < nPnNum; i++)
	{
		for (j = 0; j < nFnNum; j++)
		{
            nDataLen = 0;                               //���ݳ������㣬ÿ�ζ�����
            memset(&nData[0], 0, sizeof(nData));        //�����������
			nDataLen += GB3761GetParamFnProc(pComm, pPn[i], pFn[j], pOffset, &nData[0], sizeof(nData));
            if (0 == nDataLen || nDataLen > nMaxDataLen)  // ������ �� ������󳤶�
            {
                continue;
            }
            /*if ((pInfo->nTxDataLen + nDataLen) > nMaxDataLen)//��֡���ݳ���֡�� ����
            {
                pInfo->bSendFlag = TRUE;
                if (RET_ERR_OK != GB3761FillTxFrm(pComm))
                {
                    return RET_ERR_ERROR;
                }
                if (RET_ERR_OK != GB3761SendTxFrm(pComm))
                {
                    return RET_ERR_ERROR;
                }
                pInfo->nTxDataLen = 0;  //������֡
            }*/
            pInfo->nTxDataLen += MemCopy(&pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen], &nData[0], nDataLen);
		}
	}
	return RET_ERR_OK;
}


/********************************************************************************
* ������:GB3761GetParamFnProc
* �䡡��: pAppCom    ����֡��Ϣ
          nPn        ��Ϣ��
          nFn        ��Ϣ��
* �䡡��: pOffset    ����Fn���ݴ�����
          pData    ��ѯ��ȡ����ָ��
          nDataLen    ��ѯ��ȥ���ݳ���
* ��  ��:RET_ERR_OK ִ�гɹ�  RET_ERR_ERROR ִ��ʧ���˳� RET_ERR_RANG ���ݵ�Ԫ��ƫ�ƿ���ִ����һ��FN
* ��  ��:������ѯ3761��׼Fn�������
*********************************************************************************/
INT16U GB3761GetParamFnProc(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen)
{
    INT32S              nRet        = 0;
	INT16U              nMaxLen     = 0;
	INT16U              nDataLen    = 0;
	pGB3761_FRM_INFO    pInfo       = NULL;

    if (NULL == pComm || NULL == pOffset || NULL == pData || nMaxDataLen < 4)
    {
        return 0;
    }
	pInfo = (pGB3761_FRM_INFO)pComm->pFrmInfo;
    if (RET_ERR_OK != GB3761PnFn2DADT(nPn, nFn, pData))
    {
        return 0;
    }
    nDataLen += 4;
    nMaxLen = nMaxDataLen - 4;
    switch (nFn)
    {
        case F7:      //�ն�IP��ַ�Ͷ˿�
        {
        	nDataLen += GB3761GetParamF7(nPn, &pData[nDataLen], nMaxLen);
            break;
        }
        case F10:     //�ն˵��ܱ�/��������װ�����ò���
        {
            nDataLen = GB3761GetParamF10(pComm, nPn, pOffset, &pData[0], nMaxDataLen);
            if (nDataLen > 0)
            {
            	return nDataLen;
            }
            break;
        }
        case F11:     //�������
        {
            nDataLen = GB3761GetParamF11(pComm, nPn, pOffset, &pData[0], nMaxDataLen);
            if (nDataLen > 0)
            {
            	return nDataLen;
            }
            break;
        }
        case F12:      //�ն�״̬���������
        {
        	nDataLen += MemReadGrpPrm(nFn, nPn, &pData[nDataLen], nMaxLen);
        	break;
        }
        
        case F24:      //�ն�״̬���������
        {
        	nDataLen += MemReadCommPrm(F94, &pData[nDataLen], nMaxLen);
        	break;
        }
        case F25:      //�������������
        {
            nDataLen += GB3761GetParamF25(nPn, &pData[nDataLen], nMaxLen);
            break;
        }
        case F33:      //�ն˳������в�������
        {
        	nDataLen = GB3761GetParamF33(pComm, nPn, pOffset, &pData[0], nMaxDataLen);
            if (nDataLen > 0)
            {
            	return nDataLen;
            }
        	break;
        }
        case F225:  //���ɵ�ַ
        {
          INT8U nAddr[6] = {0};
          
            //MemReadAcPrm(F19, &nAddr[0], LEN_METER_ADDR);
            memcpy(&pData[nDataLen], &nAddr[0], 6);
            nDataLen += 6;
            break;
        }
        case F227:  //MAC��ַ
        {
            MemoryRead(STR_OFF(pMEM_FRAM_STORE, sXFramData.nMacAddr[0]), (INT8U*)&g_pMem->sXFramData.nMacAddr[0], 6, MEM_FRAM);
            memcpy(&pData[nDataLen], &g_pMem->sXFramData.nMacAddr[0], 6);
            nDataLen += 6;
            break;
        }
        default:
        {
            nRet = MemReadCommPrm(nFn, &pData[nDataLen], nMaxLen);
            if (nRet > 0)
            {
                nDataLen += nRet;
            }
            else
            {
                pInfo->bNoDataFlag = TRUE;
            }
            break;
        }
    }
    if (nDataLen > 4)
    {
    	return nDataLen;
    }
    return 0;
}

//F7 �ն�IP��ַ�Ͷ˿�
INT16U GB3761GetParamF7(INT16U nPn, INT8U *pData, INT16U nMaxDataLen)
{
    INT16U      nMinLen         = LEN_IP_ADDR * 4 + 8 + LEN_PROXY_USER + LEN_PROXY_PWD;
	INT8U       nUserPasWLen    = 0;
    INT8U       nDataLen        = 0;
    INT8U       nIndex          = 0;
    INT8U       nOffset         = 0;
    INT32S      nRet            = 0;
    INT8U       nBuf[100]       ={0};
    INT8U*      pBuf            =&nBuf[0];
    
    if (NULL == pData || 0 != nPn || nMaxDataLen < nMinLen)
	{
		return 0;
	}
    nRet = MemReadCommPrm(F7, pBuf, nMaxDataLen);
    if (nRet >= 0)
    {
        memcpy(&pData[nDataLen], &pBuf[nIndex], 20);
        if (TRUE ==IsAllEqualVal(&pData[nDataLen], 4, 0))
        {
            memcpy(&pData[nDataLen], &g_pRomtParm->nTermIp[0], 4);
        }
        
        nIndex += 20;
        nDataLen += 20;
        nUserPasWLen = pBuf[nIndex++];
        pData[nDataLen++] = nUserPasWLen;
        memcpy(&pData[nDataLen], &pBuf[nIndex], nUserPasWLen);
        nIndex += 20;
        nDataLen += nUserPasWLen;
        nUserPasWLen = pBuf[nIndex++];
        pData[nDataLen++] = nUserPasWLen;
        memcpy(&pData[nDataLen], &pBuf[nIndex], nUserPasWLen);
        nIndex += 20;
        nDataLen += nUserPasWLen;
        memcpy(&pData[nDataLen], &pBuf[nIndex], 2);
        nDataLen += 2;
    }
    return nDataLen;
}

//F10 //�ն˵��ܱ�/�������ò���
INT16U GB3761GetParamF10(pAPP_COMM pComm, INT16U nPn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen)
{
    INT16U              nMinLen     = 9 + LEN_METER_ADDR + LEN_METER_PWD + LEN_METER_ADDR;
	INT8U               nDaDt[4];
	INT8U               *pNumPos    = NULL;
	INT16U              nSucNum     = 0;             //��ѯ�������
	INT16U              nQueryNum   = 0;
	INT16U              nSeq        = 0;
    INT16U              nMp         = 0;
	INT16U              nDataLen    = 0;
	INT16U              nIndex      = *pOffset;
	INT16U              i           = 0;
    INT32U              nBaud       = 0;
	pGB3761_FRM_INFO    pInfo       = NULL;
    MEM_METER_PRM       sMtPrm;
    pMEM_METER_PRM      pMeterPrm = &sMtPrm;
    
    if (NULL == pComm || NULL == pOffset || NULL == pData || 0 != nPn || nMaxDataLen < nMinLen)
	{
		return 0;
	}
    
    pInfo = (pGB3761_FRM_INFO)pComm->pFrmInfo;
    memset(&nDaDt[0], 0, sizeof(nDaDt));
    memcpy(&nDaDt[0], &pData[0], 4); //�ݴ�DADT
    nQueryNum = pComm->pRxFrmBuf[GB3761_DATA_IDX + nIndex] + ((pComm->pRxFrmBuf[GB3761_DATA_IDX + nIndex + 1]<<8) & 0xFF00);
    nIndex += 2;
    for (i = 0; i < nQueryNum; i++)
    {
        //�ж�������Ч��
		if ((nIndex + 2) > pInfo->nRxDataLen)
        {
        	*pOffset = pInfo->nRxDataLen;
        	if (nSucNum > 0)     //�ж��Ƿ���������
		    {
		        pNumPos[0] = (INT8U)nSucNum;   //���µ������
		        pNumPos[1] = (nSucNum>>8);
			    return nDataLen;
		    }
		    return 0;
        }
        nSeq = pComm->pRxFrmBuf[GB3761_DATA_IDX + nIndex] + ((pComm->pRxFrmBuf[GB3761_DATA_IDX + nIndex + 1]<<8) & 0xFF00);
        nIndex += 2;
        memset(pMeterPrm, 0, sizeof(MEM_METER_PRM));
        MemReadMeterPrm(nSeq, F10, pMeterPrm, sizeof(MEM_METER_PRM));
        nSeq = Buf2INT16U(&pMeterPrm->nSeq[0]);
        nMp = Buf2INT16U(&pMeterPrm->nMp[0]);
        if ((pMeterPrm->bVld == 0) || (nSeq == 0) || (nMp == 0))
        {
            continue;
        }
        
        //��������
        if (0 == nSucNum) //��һ�����
    	{
            pInfo->nTxDataLen += MemCopy(&pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen], &nDaDt[0], 4);
            pNumPos = &pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen];//�����ȡ������λ��
            pInfo->nTxDataLen += 2;
    	}
        if (nDataLen > 0)
        {
        	pInfo->nTxDataLen += MemCopy(&pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen], &pData[0], nDataLen); //���Ƶ���������
            memset(&pData[0], 0, nDataLen);
            nDataLen = 0;  //���ݳ��ȼ���
        }
        memcpy(&pData[nDataLen], &pMeterPrm->nSeq[0], 2);
        nDataLen += 2;
        memcpy(&pData[nDataLen], &pMeterPrm->nMp[0], 2);
        nDataLen += 2;
        nBaud = Buf2Long(&pMeterPrm->nBaud[0], 4);
        pData[nDataLen++] = (pMeterPrm->nCom & 0x1F) | ((BaudChange(nBaud)<<5) & 0xE0);
        pData[nDataLen++] = pMeterPrm->nProt;
        nDataLen += MemCopy(&pData[nDataLen], &pMeterPrm->nAddr[0], LEN_METER_ADDR);
        nDataLen += MemCopy(&pData[nDataLen], &pMeterPrm->nPwd[0], LEN_METER_PWD);
        
        pData[nDataLen++] = pMeterPrm->nFeeNum;
        pData[nDataLen++] = ((pMeterPrm->nXsws-1) & 0x03) | (((pMeterPrm->nZsws - 4)<<2) & 0x0C);
        nDataLen += MemCopy(&pData[nDataLen], &pMeterPrm->nColAddr[0], LEN_METER_ADDR);
        pData[nDataLen++] = ((pMeterPrm->nMainType<<4) & 0xF0) | (pMeterPrm->nPartType & 0x0F);
        nSucNum++;    //���������1
        //�ж����ݳ���
        /*if ((pInfo->nTxDataLen + nDataLen) > (pComm->nFrmLen - GB3761_FRM_RES_LEN))     //���еĳ��� + ����������ĳ����Ƿ��Ѿ��������ķ��ͳ���
        {
            pInfo->bSendFlag = TRUE;
            if (nSucNum > 1)               //�����ȡ�������� ���µ������
            {
                pNumPos[0] = (INT8U)(nSucNum - 1);
                pNumPos[1] = ((nSucNum - 1)>>8);
                nSucNum = 1;
            }
            if (RET_ERR_OK != GB3761FillTxFrm(pComm))
            {
            	*pOffset = nIndex;
                return 0;
            }
            if (RET_ERR_OK != GB3761SendTxFrm(pComm))
            {
            	*pOffset = nIndex;
                return 0;
            }
            pInfo->nTxDataLen = 0; //���ݵ�Ԫ��������
        	pInfo->nTxDataLen += MemCopy(&pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen], &nDaDt[0], 4);
            pNumPos = &pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen];//�����ȡ������λ��
            pInfo->nTxDataLen += 2;
        }*/
        
    }
    *pOffset = nIndex;
    if (nSucNum > 0)     //�ж��Ƿ���������
    {
        pNumPos[0] = (INT8U)nSucNum;   //���µ������
		pNumPos[1] = (nSucNum>>8);
	    return nDataLen;
    }
    return 0;
}

//F11 �������
INT16U GB3761GetParamF11(pAPP_COMM pComm, INT16U nPn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen)
{
	INT16U nMinLen  = 5;
	INT8U  nDaDt[4];
	INT8U *pNumPos = NULL;
	INT8U  nSucNum   = 0;             //��ѯ����
	INT8U  nQueryNum = 0;
	INT8U  nCom      = 0;
	INT16U nDataLen  = 0;
	INT16U nIndex  = *pOffset;
	INT16U i = 0;
	pGB3761_FRM_INFO pInfo = NULL;
    MEM_PULSE_PRM   sPulsePrm;
	pMEM_PULSE_PRM  pPulsePrm = &sPulsePrm;  //�ն˶˿ڲ���

	if (NULL == pComm || NULL == pOffset || NULL == pData || 0 != nPn || nMaxDataLen < nMinLen)
	{
		return 0;
	}
    pInfo = (pGB3761_FRM_INFO)pComm->pFrmInfo;
    memset(&nDaDt[0], 0, sizeof(nDaDt));
    memcpy(&nDaDt[0], &pData[0], sizeof(nDaDt)); //�ݴ�DADT
	nQueryNum = pComm->pRxFrmBuf[GB3761_DATA_IDX + nIndex++];
    for (i = 0; i < nQueryNum; i++)
    {
    	if ((nIndex + 1) > pInfo->nRxDataLen)
        {
        	*pOffset = pInfo->nRxDataLen;
        	if (nSucNum > 0)           //�����ȡ������ ��������
		    {
		        pNumPos[0] = nSucNum;   //��������
			    return nDataLen;
		    }
		    return 0;
        }
        nCom = pComm->pRxFrmBuf[GB3761_DATA_IDX + nIndex++];
        if (0 == nCom || nCom > MAX_PULSE_NUM)
        {
        	continue;
        }
        memset(pPulsePrm, 0, sizeof(MEM_PULSE_PRM));
        if (MemReadGrpPrm(F11, nCom - 1, pPulsePrm, sizeof(MEM_PULSE_PRM)) < 0)
        {
            continue;
        }
        if (!pPulsePrm->bVld)
        {
            continue;
        }
        //��������
        if (0 == nSucNum)   //�ж��ǲ��ǵ�һ������
        {
			pInfo->nTxDataLen += MemCopy(&pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen], &nDaDt[0], 4);
            pNumPos = &pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen];//�����ȡ������λ��
            pInfo->nTxDataLen += 1;
        }
        if (nDataLen > 0)
        {
        	pInfo->nTxDataLen += MemCopy(&pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen], &pData[0], nDataLen); //���Ʋ�������
            memset(&pData[0], 0, nDataLen);
            nDataLen = 0;  //���ݳ��ȼ���
        }
        //��ȡ����
        pData[nDataLen++] = pPulsePrm->nPulsePort;
        pData[nDataLen++] = pPulsePrm->nPulseMp;
        pData[nDataLen++] = pPulsePrm->nPulseAttr;
        memcpy(&pData[nDataLen], &pPulsePrm->nPulseConst[0], 2);
        nDataLen += 2;
        nSucNum++;    //���������1
        //�ж����ݳ���
        /*if ((pInfo->nTxDataLen + nDataLen) > (pComm->nFrmLen - GB3761_FRM_RES_LEN))     //���еĳ��� + ����������ĳ����Ƿ��Ѿ��������ķ��ͳ���
        {
            pInfo->bSendFlag = TRUE;
            if (nSucNum > 1)               //�����ȡ������ ��������
            {
                pNumPos[0] = nSucNum - 1;
                nSucNum = 1;
            }
            if (RET_ERR_OK != GB3761FillTxFrm(pComm))
            {
            	*pOffset = nIndex;
                return 0;
            }
            if (RET_ERR_OK != GB3761SendTxFrm(pComm))
            {
            	*pOffset = nIndex;
                return 0;
            }
            pInfo->nTxDataLen = 0; //���ݵ�Ԫ��������
        	pInfo->nTxDataLen += MemCopy(&pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen], &nDaDt[0], 4);
            pNumPos = &pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen];//�����ȡ������λ��
            pInfo->nTxDataLen += 1;
        }*/
    }//End Of for (i = 1; i <= GB3761_MAX_MP_MEM_NUM; i++)
    *pOffset = nIndex;
    if (nSucNum > 0)           //�����ȡ������ ��������
    {
        pNumPos[0] = nSucNum;
	    return nDataLen;
    }
    return 0;
}


//F25 �������������
INT16U GB3761GetParamF25(INT16U nPn, INT8U *pData, INT16U nMaxDataLen)
{
    INT16U nMinLen  = 6 + sizeof(FORMAT07) + sizeof(FORMAT23);
	INT16U nDataLen = 0;
	MEM_MT_BASE_PRM  sMtBase;
	pMEM_MT_BASE_PRM pMtBase = &sMtBase;
    
    if (NULL == pData || 0 == nPn || nPn > MAX_MP || nMaxDataLen < nMinLen)
	{
		return 0;
	}
    memset(pMtBase, 0, sizeof(MEM_MT_BASE_PRM));
    
    if (MemReadMeterPrm(nPn, F25, pMtBase, sizeof(MEM_MT_BASE_PRM)) < 0)
    {
        return 0;
    }
    memcpy(&pData[nDataLen], pMtBase, 11);
    nDataLen += 11;
    return nDataLen;
}

//F33 �ն˳������в�������
INT16U GB3761GetParamF33(pAPP_COMM pComm, INT16U nPn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen)
{
	INT16U nMinLen  = 9 + sizeof(FORMAT19) + sizeof(FORMAT18) + 2 * MAX_READ_PERD_NUM * sizeof(FORMAT19);
	INT8U  nDaDt[4];
	INT8U *pNumPos = NULL;
	INT8U  nSucNum  = 0;             //��ѯ����
	INT8U  nQueryNum = 0;
	INT8U  nCom     = 0;
	INT16U nDataLen = 0;
	INT16U nIndex  = *pOffset;
	INT8U  i = 0;
	INT8U  j = 0;
	pGB3761_FRM_INFO pInfo = NULL;
    MEM_COM_PRM   sComPrm;
	pMEM_COM_PRM  pComPrm = &sComPrm;  //�ն˶˿ڲ���

	if (NULL == pComm || NULL == pOffset || NULL == pData || 0 != nPn || nMaxDataLen < nMinLen)
	{
		return 0;
	}
    pInfo = (pGB3761_FRM_INFO)pComm->pFrmInfo;
    memset(&nDaDt[0], 0, sizeof(nDaDt));
    memcpy(&nDaDt[0], &pData[0], sizeof(nDaDt)); //�ݴ�DADT
	nQueryNum = pComm->pRxFrmBuf[GB3761_DATA_IDX + nIndex++];
    for (i = 0; i < nQueryNum; i++)
    {
    	//�ж�������Ч��
    	if ((nIndex + 1) > pInfo->nRxDataLen)
        {
        	*pOffset = pInfo->nRxDataLen;
        	if (nSucNum > 0)           //�ж��Ƿ�������
		    {
		        pNumPos[0] = nSucNum;   //��������
			    return nDataLen;
		    }
		    return 0;
        }
        nCom = pComm->pRxFrmBuf[GB3761_DATA_IDX + nIndex++];
        if (0 == nCom || nCom >= MAX_COM_NUM)
        {
        	continue;
        }
        memset(pComPrm, 0, sizeof(MEM_COM_PRM));
        if (MemReadRdPrm(F33, nCom - 1, pComPrm, sizeof(MEM_COM_PRM)) < 0)
        {
            continue;
        }
        /*if (RET_ERR_OK != MemReadParam(MEM_OBJ_PRM_COM, nCom, 0, pComPrm, sizeof(MEM_COM_PRM)))
        {
        	continue;
        }*/
        //��������
        if (0 == nSucNum)  //�ж��ǲ��ǵ�һ������
        {
            pInfo->nTxDataLen += MemCopy(&pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen], &nDaDt[0], 4);
            pNumPos = &pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen];//�����ȡ������λ��
            pInfo->nTxDataLen += 1;
        }
        if (nDataLen > 0)
        {
        	pInfo->nTxDataLen += MemCopy(&pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen], &pData[0], nDataLen); //���Ʋ�������
            memset(&pData[0], 0, nDataLen);
            nDataLen = 0;
        }
        //��ȡ����
        pData[nDataLen++] = pComPrm->nCom;
        pData[nDataLen++] = (INT8U)pComPrm->bsRunCtrl;
        pData[nDataLen++] = pComPrm->bsRunCtrl>>8;
        
        nDataLen += MemCopy(&pData[nDataLen], &pComPrm->bsReadDay[0], 4);
        nDataLen += MemCopy(&pData[nDataLen], &pComPrm->sReadTime, sizeof(FORMAT19));
        
        if (CARR_PORT == nCom)
        {
            pComPrm->nReadIntrvl /= 60;
            pData[nDataLen++] = (pComPrm->nReadIntrvl > 0xFF) ? (0xFF) : (pComPrm->nReadIntrvl); //����12�¹淶����λСʱ
        }
        else
        {
            pData[nDataLen++] = (pComPrm->nReadIntrvl > 0xFF) ? (0xFF) : (pComPrm->nReadIntrvl); //������1-60����λ����
        }	
        nDataLen += MemCopy(&pData[nDataLen], &pComPrm->sBoardTime, sizeof(FORMAT18));
        if (pComPrm->nReadPerdNum > MAX_READ_PERD_NUM)
        {
        	pComPrm->nReadPerdNum = MAX_READ_PERD_NUM;
        }
        pData[nDataLen++] = pComPrm->nReadPerdNum;
        for (j = 0; j < pComPrm->nReadPerdNum; j++)
        {
        	nDataLen += MemCopy(&pData[nDataLen], &pComPrm->sReadPerd[j][0], sizeof(FORMAT19));
        	nDataLen += MemCopy(&pData[nDataLen], &pComPrm->sReadPerd[j][1], sizeof(FORMAT19));
        }
        nSucNum++;                  //������1
        //�ж����ݳ���
        /*if ((pInfo->nTxDataLen + nDataLen) > (pComm->nFrmLen - GB3761_FRM_RES_LEN))     //���еĳ��� + ����������ĳ����Ƿ��Ѿ��������ķ��ͳ���
        {
            pInfo->bSendFlag = TRUE;
            if (nSucNum > 1)               //�����ȡ������ ��������
            {
                pNumPos[0] = nSucNum - 1;
                nSucNum = 1;
            }
            if (RET_ERR_OK != GB3761FillTxFrm(pComm))
            {
            	*pOffset = nIndex;
                return 0;
            }
            if (RET_ERR_OK != GB3761SendTxFrm(pComm))
            {
            	*pOffset = nIndex;
                return 0;
            }
            pInfo->nTxDataLen = 0; //���ݵ�Ԫ��������
        	pInfo->nTxDataLen += MemCopy(&pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen], &nDaDt[0], 4);
            pNumPos = &pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen];//�����ȡ������λ��
            pInfo->nTxDataLen += 1;
        }*/
    }//End Of for (i = 1; i <= GB3761_MAX_MP_MEM_NUM; i++)
    *pOffset = nIndex;
    if (nSucNum > 0)           //�ж��Ƿ�������
    {
        pNumPos[0] = nSucNum;   //��������
	    return nDataLen;
    }
    return 0;
}



















