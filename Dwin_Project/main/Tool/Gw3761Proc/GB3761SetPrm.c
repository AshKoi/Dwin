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
#include "GB3761SetPrm.h"
#include "Gw3761.h"
#include "MemDrvExt.h"
#include "MemDrv.h"
#include "Table.h"
#include "Fun.h"
#include "Macro.h"
#include "SysRes.h"
#include "Base.h"
#include "ChkCrc.h"
#include "Reset.h"
#include "MemTbl.h"
#include "SoftTimer.h"
#include "meter645.h"
#include "rmotlink.h"
#include "CarrCol.h"

/**************************************************************************************************
                                           ��غ���ʵ��
***************************************************************************************************/
/********************************************************************************
* ������: GB3761SetParam
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ��������
*********************************************************************************/
INT8S GB3761SetParam(pAPP_COMM pComm)
{
    INT16U nFn[GB3761_MAX_FN_NUM];
    INT8U  nFnNum = 0;
    INT16U nPn[8];
    INT16U nPnNum  = 0;
    INT16U nOffset = 0;     //���ݱ�ʶ�����ݵ�Ԫ ������ƫ��
    pGB3761_FRM_INFO   pInfo = NULL;

    if (NULL == pComm)
    {
    	return RET_ERR_POINT;
    }
    pInfo = (pGB3761_FRM_INFO)pComm->pFrmInfo;   //��ȡ����֡��Ϣ��ָ��
    pInfo->nTxDataLen = 5; //����ȷ�Ϸ��� DADT+AFN
    while (nOffset + 4 <= pInfo->nRxDataLen)
    {
        memset(&nPn[0], 0xFFFF, sizeof(nPn));
        memset(&nFn[0], 0, sizeof(nFn));
        if (RET_ERR_OK != GB3761DaDt2PnFn(&pComm->pRxFrmBuf[GB3761_DATA_IDX + nOffset], &nPn[0], &nPnNum, &nFn[0], &nFnNum, &pInfo->bAllMpFlag))  //Dadtת��Pnfn
		{
	        break;
        }
        nOffset += 4;                            //�Ѿ��������ݱ�ʶ�����ݵ�Ԫ����
        if (RET_ERR_OK != GB3761SetParamProc(pComm, &nPn[0], nPnNum, &nFn[0], nFnNum, &nOffset))
        {
            break;
        }
    }
    return RET_ERR_OK;
}

/********************************************************************************
* ������: GB3761SetParamProc
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ��������
*********************************************************************************/
INT8S GB3761SetParamProc(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset)
{
    INT16U 	   i     = 0;                                //ѭ������
    INT16U 	   j     = 0;                                //ѭ������
	INT16U	   nDataLen = 0;
	INT16U     nMaxDataLen = 0;
	INT8U      nData[5];
    pGB3761_FRM_INFO pInfo = NULL;

    if (NULL == pComm || NULL == pPn || NULL == pFn || NULL == pOffset)
    {
    	return RET_ERR_POINT;
    }

    pInfo = (pGB3761_FRM_INFO)pComm->pFrmInfo;
	nMaxDataLen = pComm->nFrmLen - GB3761_FRM_RES_LEN;
    for (i = 0; i < nPnNum; i++)
    {
        for (j = 0; j < nFnNum; j++)
        {
        	nDataLen = 0;
            memset(&nData[0], 0, sizeof(nData));   //�����������
            nDataLen += GB3761SetParamFnProc(pComm, pPn[i], pFn[j], pOffset, &nData[0], sizeof(nData));
            if (nDataLen > nMaxDataLen || 0 == nDataLen)  //һ��Fn�����ݳ��ȳ���֡�� �� �����ݻ�ȡ ����
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
        }   //End Of for(nFcode = 0; nFcode < GB3761_MAX_FN_NUM; nFcode++)
    }       //End Of for(pn = 0; pn < nMaxMpNum; pn++)
    return RET_ERR_OK;
}

/********************************************************************************
* ������:GB3761SetParamFnProc
* �䡡��: pFrmInfoRx ����֡��Ϣ
          nPn        ��Ϣ��
          nFn        ��Ϣ��
* �䡡��: pOffset    ����Fn���ݴ�����
* ��  ��:RET_ERR_OK ִ�гɹ�  RET_ERR_ERROR ִ��ʧ���˳� RET_ERR_RANG ���ݵ�Ԫ��ƫ�ƿ���ִ����һ��FN
* ��  ��:��������3761��׼Fn�������
*********************************************************************************/
INT16U GB3761SetParamFnProc(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen)
{
    INT8U   nErrData    = 0;
	BOOLEAN bSetSuc = FALSE;
    INT8U   nResult  = GB3761_PART_OTHER;//û��ָ�����󣬾�Ϊ��������
    INT16U  nDataLen = 0;
    pGB3761_FRM_INFO pInfo = NULL;
    pGB3761_FRM_MAP  pRxMap = NULL;
    INT32S    nRet  = 0;

    if (NULL == pComm || NULL == pOffset || NULL == pData)
    {
        return 0;
    }
    pInfo = (pGB3761_FRM_INFO)pComm->pFrmInfo;
    pRxMap = (pGB3761_FRM_MAP)pComm->pRxFrmBuf;
    if (RET_ERR_OK != GB3761PnFn2DADT(nPn, nFn, &pData[nDataLen]))
    {
        return 0;
    }
    nDataLen += 4;
    switch (nFn) //����ͬFn��
    {
        case F1:      //F1�ն�ͨ�Ų�������
        {
        	bSetSuc = GB3761SetParamF1(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F3:      //F3 ��վIP��ַ�Ͷ˿�
        {
            bSetSuc = GB3761SetParamF3(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F4:      //F4 ��վ�绰����Ͷ������ĺ���
        {
            bSetSuc = GB3761SetParamF4(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F7:      //�ն˳���������
        {
            bSetSuc = GB3761SetParamF7(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F8:      //F8 �ն�����ͨ�Ź�����ʽ
        {
            bSetSuc = GB3761SetParamF8(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F10:     //�ն˵��ܱ�/��������װ�����ò���
        {
            bSetSuc = GB3761SetParamF10(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset, &nResult);
            break;
        }
        case F11:     //�������(F11)
        {
            bSetSuc = GB3761SetParamF11(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F12:     //�ն�״̬���������
        {
            bSetSuc = GB3761SetParamF12(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F16:     //F16 ����ר�� �û���������
        {
            bSetSuc = GB3761SetParamF16(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F24:     //�������㽭�淶���õ�ַFn��
        {
            bSetSuc = GB3761SetParamF24(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F25:     //�������������
        {
            bSetSuc = GB3761SetParamF25(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F33:     //F33 �ն˳������в�������
        {
            bSetSuc = GB3761SetParamF33(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
        	break;
        }
        case F85:       //�ز�ר��ѡ���־
        {
            bSetSuc = GB3761SetParamF85(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F94:     //�������㽭�淶���õ�ַFn��
        {
            bSetSuc = GB3761SetParamF94(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F130:
        {
            bSetSuc = GB3761SetParamF130(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        
        case F131:
        {
            bSetSuc = GB3761SetParamF131(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        
        case F132:
        {
            bSetSuc = GB3761SetParamF132(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        
        //�ն˽��ɵ�ַ
        case F225:
        {
            bSetSuc = GB3761SetParamF225(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        
        //MAC��ַ
        case F227:
        {
            bSetSuc = GB3761SetParamF227(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        
        default:
        {
            pInfo->bNoDataFlag = TRUE;
            *pOffset = pInfo->nRxDataLen;
            break;
        }
    }
    if (bSetSuc)
    {
    	pInfo->bSucFlag = TRUE;
    	pData[nDataLen++] = GB3761_PART_ACK;
    	//Update3761ParamStat(nFn);                  //�����ն˲���״̬
        //EvtERC3GenPrmChanRecord(pRxMap->uA3.sDet.nMSA, nPn, (nFn + 1));
    }
    else
    {
    	pInfo->bFailFlag = TRUE;
    	pData[nDataLen++] = nResult;
    }
    return nDataLen;
}

//F1 �ն�ͨ�Ų�������
BOOLEAN GB3761SetParamF1(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U       nFnLen      = 0;
	INT16U      nIndex      = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
    nIndex = *pOffset;
	nFnLen = 6;
	if ((nIndex + nFnLen) > nDataLen)  //������Ч�����ж�
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 != nPn)
    {
    	*pOffset += nFnLen;
    	return FALSE;
    }
    MemWriteCommPrm(F1, &pData[nIndex], nFnLen);
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F3 ��վIP��ַ�Ͷ˿�
BOOLEAN GB3761SetParamF3(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U       nFnLen    = 0;
	INT16U      nIndex    = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex    = *pOffset;
	nFnLen = 2 * LEN_IP_ADDR + 4 + LEN_APN;
	if ((nIndex + nFnLen) > nDataLen)  //������Ч�����ж�
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 != nPn)
    {
        *pOffset += nFnLen;
        return FALSE;
    }
    MemWriteCommPrm(F3, &pData[nIndex], nFnLen);
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F4 ��վ�绰����Ͷ������ĺ���
BOOLEAN GB3761SetParamF4(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U       nFnLen    = 0;
	INT16U      nIndex    = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex = *pOffset;
	nFnLen = 8 + 8;
	if ((nIndex + nFnLen) > nDataLen)  //������Ч�����ж�
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 != nPn)
    {
        *pOffset += nFnLen;
        return FALSE;
    }
    MemWriteCommPrm(F4, &pData[nIndex], nFnLen);
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F7 �ն�IP��ַ�Ͷ˿�
BOOLEAN GB3761SetParamF7(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U       nFnLen          = 0;
	INT16U      nIndex          = 0;
    INT8U       nOffset         = 0;
    INT8U       nProxyType      = 0;                     //(F7)��������
    INT8U       nProxyLinkModl  = 0;                 //(F7)�������ӷ�ʽ
    INT8U       nProxyUserLen   = 0;                  //(F7)�����û�������
    INT8U       nProxyPwdLen    = 0;	                //(F7)�����û����볤��
    INT8U       nBuf[100]       ={0};
    INT8U*      pBuf            =&nBuf[0];
    
	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex = *pOffset;
	if ((nIndex + 20) > nDataLen)
	{
		*pOffset = nDataLen;
        return FALSE;
	}
	nProxyUserLen = pData[nIndex + 20];
	if ((nIndex + 21 + nProxyUserLen) > nDataLen)
	{
		*pOffset = nDataLen;
        return FALSE;
	}
	nProxyPwdLen = pData[nIndex + 21 + nProxyUserLen];
	nFnLen = 4 * LEN_IP_ADDR + 8 + nProxyUserLen + nProxyPwdLen;
	if ((nIndex + nFnLen) > nDataLen)  // ������Ч�����ж�
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    nProxyType = pData[nIndex + LEN_IP_ADDR * 3];//��������0-3
    nProxyLinkModl = pData[nIndex + 19];//�������ӷ�ʽ0-1
    if (0 != nPn || nProxyUserLen > LEN_PROXY_USER || nProxyPwdLen > LEN_PROXY_PWD
    	|| nProxyType > 3 || nProxyLinkModl > 1)
    {
    	*pOffset += nFnLen;
        return FALSE;
    }
    
    nIndex += MemCopy(&pBuf[nOffset], &pData[nIndex], 20);
    nOffset += 20;
    nProxyUserLen = pData[nIndex++];
    pBuf[nOffset++] = nProxyUserLen;
    memcpy(&pBuf[nOffset], &pData[nIndex], nProxyUserLen);
    nIndex += nProxyPwdLen;
    nOffset += 20;
    nProxyPwdLen = pData[nIndex++];
    pBuf[nOffset++] = nProxyPwdLen;
    memcpy(&pBuf[nOffset], &pData[nIndex], nProxyPwdLen);
    nIndex += nProxyPwdLen;
    nOffset += 20;
    memcpy(&pBuf[nOffset], &pData[nIndex], 2);
    nOffset += 2; 
    MemWriteCommPrm(F7, pBuf, nOffset);
    *pOffset = nIndex;
    return TRUE;
}

//F8 �ն�����ͨ�Ź�����ʽ
BOOLEAN GB3761SetParamF8(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U       nFnLen    = 0;
	INT16U      nIndex    = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex    = *pOffset;
	nFnLen = 8;
	if ((nIndex + nFnLen) > nDataLen)  //������Ч�����ж�
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 != nPn)
    {
        *pOffset += nFnLen;
        return FALSE;
    }
    MemWriteCommPrm(F8, &pData[nIndex], nFnLen);
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F10 //�ն˵��ܱ�/��������װ�����ò���, ����376.1��׼��Լ
BOOLEAN GB3761SetParamF10(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset, INT8U *pReSult)
{
	BOOLEAN bFlag   = 0;
	INT8U   nFnLen  = 0;
	INT16U  nSetNum = 0;
	INT8U   nSetSucNum = 0;
	INT16U  i = 0;
	INT16U  nSeq    = 0;
	INT16U  nMP     = 0;
	INT16U  nIndex  = 0;
    INT32U  nBaud   = 0;
	//INT16U  nSeqBuf[64];
	//INT16U  nMpBuf[64];
    const INT32U  GB3761BaudAry[8] = {0, 600, 1200, 2400, 4800, 7200, 9600, 19200};//����F10
	MEM_METER_PRM sMtPrm;
	pMEM_METER_PRM pMtPrm = &sMtPrm;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex = *pOffset;
	nSetNum = pData[nIndex] + ((pData[nIndex+1]<<8) & 0xFF00);
	nIndex += 2;
	nFnLen = 27;
	if (0 != nPn)
    {
        nIndex += nSetNum * nFnLen;    //ƫ��������Ŀ
        *pOffset = nIndex;
        return FALSE;
    }

	//memset(&nSeqBuf[0], 0, sizeof(nSeqBuf));
    //memset(&nMpBuf[0], 0, sizeof(nMpBuf));
    for (i = 0; i < nSetNum; i++)
    {                           //������Ч��Ч��
        if ((nIndex + nFnLen) > nDataLen)
        {
        	nIndex = nDataLen;
        	break;
        }
        nSeq = pData[nIndex] + ((pData[nIndex + 1]<<8) & 0xFF00);
        nMP = pData[nIndex + 2] + ((pData[nIndex + 3]<<8) & 0xFF00);
        if (nSeq == 0 || nSeq > MAX_METER_NUM || nMP > MAX_MP)
        {
        	nIndex += nFnLen;
            continue;
        }
        memset(pMtPrm, 0, sizeof(MEM_METER_PRM));
        if (0 == nMP)//ɾ��������µ����в���
        {
            pMtPrm->bVld = FALSE;
            nIndex += nFnLen;
        }
        else
        {
            pMtPrm->bVld = TRUE;
            INT16U2Buf(&pMtPrm->nSeq[0], nSeq);
            INT16U2Buf(&pMtPrm->nMp[0], nMP);
            nIndex += 4;
            
            pMtPrm->nCom = pData[nIndex] & 0x1F;
            nBaud = GB3761BaudAry[(pData[nIndex]>>5) & 0x07];
            Long2Buf(&pMtPrm->nBaud[0], nBaud, 4);
            nIndex++;
            pMtPrm->nProt = pData[nIndex++];
            nIndex += MemCopy(&pMtPrm->nAddr[0], &pData[nIndex], LEN_METER_ADDR);
            nIndex += MemCopy(&pMtPrm->nPwd[0], &pData[nIndex], LEN_METER_PWD);
            pMtPrm->nFeeNum = (pData[nIndex++] & 0x3F);
            pMtPrm->nXsws = (pData[nIndex] & 0x03) + 1;
            pMtPrm->nZsws = ((pData[nIndex]>>2) & 0x03) + 4;
            nIndex++;
            nIndex += MemCopy(&pMtPrm->nColAddr[0], &pData[nIndex], LEN_METER_ADDR);
            pMtPrm->nMainType = (pData[nIndex]>>4) & 0x0F;
            pMtPrm->nPartType = pData[nIndex] & 0x0F;
            nIndex++;
            if (!IsVldMeterPrm(pMtPrm))
            {
            	continue;
            }
            if (IsRedupMtAddr(pMtPrm))
            {
                *pReSult = GB3761_PART_ADDR;//��ַ�ظ�
                continue;
            }
        }
        
        //�ж��ز�����
        //����ز������߽ڵ���Ϣ��Ŀǰÿ�·�һ�ζ���ӣ��������ӽڵ�������ز���������ӣ�
        /*if (g_nCarrZbSelFlg == TYPE_CARR)
        {
            //if (g_nCarrModType != CARR_MOD_DX)
            {
                if (g_nCarrInitFlg == 0)
                {
                    continue;
                }
                if (pMtPrm->nCom == 0x1F)
                {
                    CarrRouteAdd(&pMtPrm->nAddr[0], 2);
                }
            }
        }*/
        if (pMtPrm->nCom == 0x1F)
        {
            //�ز���ʱ��Ҫ�ж�
            if (g_nCarrZbSelFlg == TYPE_CARR)
            {
                if (g_nCarrInitFlg == 0)
                {
                    continue;
                }
                CarrRouteAdd(&pMtPrm->nAddr[0], 2);
            }
        }
        
        if (nMP == 1)
        {
            //MemWriteAcPrm(F19, &pMtPrm->nAddr[0], LEN_METER_ADDR);
        }
        MemWriteMeterPrm(nSeq, F10, pMtPrm, sizeof(MEM_METER_PRM));
        if (nSetSucNum < 64)//���������ͬ������;
        {
            //nSeqBuf[nSetSucNum] = nSeq;
            //nMpBuf[nSetSucNum]  = nMP;
        }
        nSetSucNum++;
    }//End Of for(i = 0; i < nSetNum; i++)
    if (nSetSucNum > 0)  //���������������� ���½����ز��ڵ��б�
    {
        if (nSetSucNum > 64)
        {
            //CleanSameMp(&nSeqBuf[0], &nMpBuf[0], 64, MT_TYPE_DGN); //�����ͬ������ŵĵ�����
        }
        else
        {
            //CleanSameMp(&nSeqBuf[0], &nMpBuf[0], nSetSucNum, MT_TYPE_DGN);
        }
        //TermStatInfoRefresh();  //��Ϣͳ��
        //MpStatInfoRefresh();
        bFlag = TRUE;
        //MemWriteXSaveData(MEM_OBJ_X_SAVE_TERM, 0, MEM_SAVE_TERM_METER_UPDATE_ID, &bFlag, 1);
        //OSQPost(TaskQueue[QUEUE_METER_SCHE_IDX], (void *)(MSG_METER_PRM_UPDATE));
    }
    MeterMpMap(1);
    /*if (g_nCarrZbSelFlg != TYPE_CARR)   //�ز��������ú�������������Ӱ���ز��ڵ����
    {
        RdMeterSemProc();
    }*/
    if (pMtPrm->nCom == 0x1F)
    {
        if (g_nCarrZbSelFlg != TYPE_CARR)   //�ز��������ú�������������Ӱ���ز��ڵ����
        {
            RdMeterSemProc();
        }
    }
    else
    {
        RdMeterSemProc();
    }
    *pOffset = nIndex;
    if (0 == (nSetSucNum - nSetNum))
    {
        return TRUE;
    }
    return FALSE;
}

//F11 �������
BOOLEAN GB3761SetParamF11(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U    i         = 0;
	INT8U    nFnLen    = 0;
	INT8U    nCom      = 0;
	INT8U    nSetNum   = 0;
	INT8U    nSetSucNum = 0;
	INT16U   nIndex    = 0;
	INT16U   nMP       = 0;
	//INT16U   nSeqBuf[64];
	//INT16U   nMpBuf[64];
	MEM_PULSE_PRM sPulPrm;
	pMEM_PULSE_PRM pPulPrm = &sPulPrm;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}

	nIndex = *pOffset;
	nSetNum = pData[nIndex++];               //��������·��0-64
	nFnLen  = 5;
    if (0 != nPn)
    {
    	nIndex += nFnLen * nSetNum;
        *pOffset = nIndex;
        return FALSE;
    }
	//memset(&nSeqBuf[0], 0, sizeof(nSeqBuf));
    //memset(&nMpBuf[0], 0, sizeof(nMpBuf));
    for (i = 0 ; i < nSetNum ; i++ )
    {
        if ((nIndex + nFnLen) > nDataLen)
        {
        	nIndex = nDataLen;
        	break;
        }
        nCom = pData[nIndex];
        nMP  = pData[nIndex + 1];
        if (0 == nCom || nCom > MAX_PULSE_NUM || nMP > 64)
        {
        	nIndex += nFnLen;
            continue;
        }
        memset(pPulPrm, 0, sizeof(MEM_PULSE_PRM));
        if (0 == nMP)
        {
            pPulPrm->bVld = FALSE;
            nIndex += nFnLen;
        }
        else
        {
        	pPulPrm->bVld = TRUE;
            pPulPrm->nPulsePort = nCom;
            pPulPrm->nPulseMp  = nMP;
            nIndex += 2;
            pPulPrm->nPulseAttr = pData[nIndex++];
            memcpy(&pPulPrm->nPulseConst[0], &pData[nIndex],2);
            nIndex += 2;
        }
        MemWriteGrpPrm(F11, nCom - 1, pPulPrm, sizeof(MEM_PULSE_PRM));
        if (nSetSucNum < 64)//���������ͬ������;
        {
            //nSeqBuf[nSetSucNum] = nCom;
            //nMpBuf[nSetSucNum]  = nMP;
        }
        nSetSucNum++;
    }
    if (nSetSucNum > 0)
    {
        if (nSetSucNum <= 64)
        {
            //CleanSameMp(&nSeqBuf[0], &nMpBuf[0], nSetSucNum, MT_TYPE_PULSE); //�����ͬ������ŵĵ�����
        }
        //TermStatInfoRefresh();  //��Ϣͳ��
        //MpStatInfoRefresh();
    }
    *pOffset = nIndex;
    if (0 == (nSetSucNum - nSetNum))
    {
        return TRUE;
    }
    return FALSE;
}

//F12 �ն�״̬���������
BOOLEAN GB3761SetParamF12(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
    INT8U    nFnLen  = 0;
	INT16U   nIndex  = 0;
    
    if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
    nIndex = *pOffset;
	nFnLen = 2;
    if ((nIndex + nFnLen) > nDataLen)  //������Ч�����ж�
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 != nPn)
    {
        *pOffset += nFnLen;
        return FALSE;
    }
    
    MemWriteGrpPrm(F12, 0, &pData[nIndex], nFnLen);
    nIndex += nFnLen; 
    *pOffset = nIndex;
    return TRUE;
    
}

//F16 ����ר�� �û���������
BOOLEAN GB3761SetParamF16(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U   nFnLen = 0;
	INT16U  nIndex = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex = *pOffset;
	nFnLen = 32 + 32;
	if ((nIndex + nFnLen) > nDataLen)  //������Ч�����ж�
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 != nPn)
    {
        *pOffset += nFnLen;
        return FALSE;
    }
	MemWriteCommPrm(F16, &pData[nIndex], nFnLen);
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F24
BOOLEAN GB3761SetParamF24(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
    INT8U    nFnLen  = 0;
	INT16U   nIndex  = 0;
    
    if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
    nIndex = *pOffset;
	nFnLen = 4;
    if ((nIndex + nFnLen) > nDataLen)  //������Ч�����ж�
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 != nPn)
    {
        *pOffset += nFnLen;
        return FALSE;
    }
    
    MemWriteCommPrm(F94, &pData[nIndex], nFnLen);
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F25
BOOLEAN GB3761SetParamF25(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
    INT8U       nFnLen = 0;
    FORMAT07    sTmpVolt;
	INT16U      nIndex = 0;
	MEM_MT_BASE_PRM    sMtBase;
	pMEM_MT_BASE_PRM   pMtBase = &sMtBase;
    
    if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
    nIndex = *pOffset;
	nFnLen = 11;
    if ((nIndex + nFnLen) > nDataLen)  //������Ч�����ж�
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 == nPn || nPn > MAX_MP)
    {
        *pOffset += nFnLen;
        return FALSE;
    }
    memset(pMtBase, 0, sizeof(MEM_MT_BASE_PRM));
    
    memcpy(pMtBase, &pData[nIndex], nFnLen);
    if (MemWriteMeterPrm(nPn, F25, pMtBase, sizeof(MEM_MT_BASE_PRM)) < 0)
    {
        return FALSE;
    }
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}


//F33 �ն˳������в�������
BOOLEAN GB3761SetParamF33(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U    i         = 0;
	INT8U    j         = 0;
	INT8U    nCom      = 0;
	//INT8U    nReadIntrvl = 0;
	INT8U    nReadPerdNum = 0;
	INT8U    nSetNum   = 0;
	INT8U    nSetSucNum = 0;
	INT16U   nFnLen    = 0;
	INT16U   nIndex    = 0;
	MEM_COM_PRM    sComPrm;
	pMEM_COM_PRM   pComPrm = &sComPrm;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex = *pOffset;
	nSetNum = pData[nIndex++];     //���ÿ���1-31
    if (0 != nPn)
    {
    	for (i = 0; i< nSetNum; i++)   //��ȡ���õ�Ԫ��ƫ����
        {
            nReadPerdNum = pData[nIndex + nFnLen + 13]; //������ʱ�� 0-24
            nFnLen += (nReadPerdNum * (sizeof(FORMAT19) + sizeof(FORMAT19)) + 14);
        }
        nIndex += nFnLen;
        *pOffset = nIndex;
        return FALSE;
    }
    for (i = 0; i < nSetNum; i++)
    {
        if ((nIndex + 13) > nDataLen)
        {
        	*pOffset = nDataLen;
        	return FALSE;
        }
        nReadPerdNum = pData[nIndex + 13]; //������ʱ�� 0-24
        nFnLen = (nReadPerdNum * (sizeof(FORMAT19) + sizeof(FORMAT19)) + 14); //����һ��ƫ��һ��
        if ((nIndex + nFnLen) > nDataLen)
        {
        	*pOffset = nDataLen;
        	return FALSE;
        }
        nCom = pData[nIndex];  //�˿ں�1-31
        //nReadIntrvl = pData[nIndex + 9]; //������1-60
        if (0 == nCom || nCom >= MAX_COM_NUM
        	|| nReadPerdNum > MAX_READ_PERD_NUM)
        {
            nIndex += nFnLen;
            continue;
        }
        MemReadRdPrm(F33, nCom - 1, pComPrm, sizeof(MEM_COM_PRM));
        pComPrm->nCom = pData[nIndex++];  //�˿ں�1-31
        pComPrm->bsRunCtrl = pData[nIndex] + (pData[nIndex + 1]<<8);
        nIndex += 2;
        memcpy(&pComPrm->bsReadDay[0], &pData[nIndex], 4);
        nIndex += 4;
        memcpy(&pComPrm->sReadTime, &pData[nIndex], sizeof(FORMAT19));
        nIndex += sizeof(FORMAT19);
        if (CARR_PORT == nCom)
        {
            pComPrm->nReadIntrvl = pData[nIndex++] * 60; //����12�¹淶����λСʱ
        }
        else
        {
            pComPrm->nReadIntrvl = pData[nIndex++]; //������1-60����λ����
        }
        memcpy(&pComPrm->sBoardTime, &pData[nIndex], sizeof(FORMAT18));
        nIndex += sizeof(FORMAT18);
        pComPrm->nReadPerdNum = pData[nIndex++];
        for (j = 0; j < pComPrm->nReadPerdNum; j++)
        {
            memcpy(&pComPrm->sReadPerd[j][0], &pData[nIndex], sizeof(FORMAT19));
            nIndex += sizeof(FORMAT19);
            memcpy(&pComPrm->sReadPerd[j][1], &pData[nIndex], sizeof(FORMAT19));
            nIndex += sizeof(FORMAT19);
        }
        MemWriteRdPrm(F33, nCom - 1, pComPrm, sizeof(MEM_COM_PRM));
        nSetSucNum++;
    }
    *pOffset = nIndex;
    if (0 == (nSetSucNum - nSetNum))
    {
    	return TRUE;
    }
	return FALSE;
}

//F85
BOOLEAN GB3761SetParamF85(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
    INT8U    nFnLen  = 0;
	INT16U   nIndex  = 0;
    INT8U    nSel   = 0;
    
    if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
    nIndex = *pOffset;
	nFnLen = 1;
    if ((nIndex + nFnLen) > nDataLen)  //������Ч�����ж�
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 != nPn)
    {
        *pOffset += nFnLen;
        return FALSE;
    }
    nSel = pData[nIndex];
    MemWriteCommPrm(F85, &nSel, nFnLen);
    
    if (nSel == 1)
    {
        g_nCarrZbSelFlg = TYPE_ZB;
    }
    else
    {
        g_nCarrZbSelFlg = TYPE_CARR;
    }
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F94
BOOLEAN GB3761SetParamF94(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
    INT8U    nFnLen  = 0;
	INT16U   nIndex  = 0;
    
    if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
    nIndex = *pOffset;
	nFnLen = 4;
    if ((nIndex + nFnLen) > nDataLen)  //������Ч�����ж�
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 != nPn)
    {
        *pOffset += nFnLen;
        return FALSE;
    }
    
    MemWriteCommPrm(F94, &pData[nIndex], nFnLen);
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F130
BOOLEAN GB3761SetParamF130(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
    INT8U    nFnLen     = 0;
	INT16U   nIndex     = 0;
    INT8U    nErrData   = 0;
    
    if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
    nIndex = *pOffset;
	nFnLen = 1;
    if ((nIndex + nFnLen) > nDataLen)  //������Ч�����ж�
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 != nPn)
    {
        *pOffset += nFnLen;
        return FALSE;
    }
    nErrData = pData[nIndex];
    if (nErrData == 0xAA)//��
    {
        GprsPowerOff();
    }
    else
    {
        GprsPowerOn();//��
    }
    
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F131
BOOLEAN GB3761SetParamF131(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
    INT8U    nFnLen     = 0;
	INT16U   nIndex     = 0;
    INT8U    nErrData   = 0;
    
    if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
    nIndex = *pOffset;
	nFnLen = 1;
    if ((nIndex + nFnLen) > nDataLen)  //������Ч�����ж�
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 != nPn)
    {
        *pOffset += nFnLen;
        return FALSE;
    }
    nErrData = pData[nIndex];
    if (nErrData == 0xAA) //������
    {
        CarrModeNwork();
    }
    else
    {
        CarrModework();   //����
    }
    
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F132
BOOLEAN GB3761SetParamF132(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
    INT8U    nFnLen     = 0;
	INT16U   nIndex     = 0;
    INT8U    nErrData   = 0;
    
    if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
    nIndex = *pOffset;
	nFnLen = 1;
    if ((nIndex + nFnLen) > nDataLen)  //������Ч�����ж�
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 != nPn)
    {
        *pOffset += nFnLen;
        return FALSE;
    }
    nErrData = pData[nIndex];
    if (nErrData == 0xAA) //��������ͨӍ
    {
        GprsCommAbnormal();
    }
    else
    {
        GprsCommNormal();   //ͨ������
    }
    
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F225
BOOLEAN GB3761SetParamF225(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
    INT8U    nFnLen     = 0;
	INT16U   nIndex     = 0;
    
    if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
    nIndex = *pOffset;
	nFnLen = 6;
    if ((nIndex + nFnLen) > nDataLen)  //������Ч�����ж�
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 != nPn)
    {
        *pOffset += nFnLen;
        return FALSE;
    }
    
    //MemWriteAcPrm(F19, &pData[nIndex], LEN_METER_ADDR);
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F227
BOOLEAN GB3761SetParamF227(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
    INT8U    nFnLen     = 0;
	INT16U   nIndex     = 0;
    
    if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
    nIndex = *pOffset;
	nFnLen = 6;
    if ((nIndex + nFnLen) > nDataLen)  //������Ч�����ж�
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 != nPn)
    {
        *pOffset += nFnLen;
        return FALSE;
    }
    memcpy(&g_pMem->sXFramData.nMacAddr[0], &pData[nIndex], 6);
    MemoryWrite(STR_OFF(pMEM_FRAM_STORE, sXFramData.nMacAddr[0]), (INT8U*)&g_pMem->sXFramData.nMacAddr[0], 6, MEM_FRAM);
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}


