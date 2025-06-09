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
#include "GB3761GetPrmExt.h"
#include "Gw3761.h"
#include "MemDrvExt.h"
#include "Macro.h"
#include "rmotlink.h"
#include "Ethnetlink.h"
#include "Fun.h"
#include "MemTbl.h"


/********************************************************************************
* ������: GB3761GetParamExt
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ������ѯ
*********************************************************************************/
INT8S GB3761GetParamExt(pAPP_COMM pComm)
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
        if (RET_ERR_OK != GB3761GetParamProcExt(pComm, &nPn[0], nPnNum, &nFn[0], nFnNum, &nOffset))
        {
            break;
        }
	}
	return RET_ERR_OK;
}

/********************************************************************************
* ������: GB3761GetParamProcExt
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ������ѯ
*********************************************************************************/
INT8S GB3761GetParamProcExt(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset)
{
	INT16U    	            i           = 0;
	INT16U    	            j           = 0;
	INT16U		            nDataLen    = 0;
	INT8U     	            nData[200];                 //g_nProtTmpData
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
			nDataLen += GB3761GetParamFnProcExt(pComm, pPn[i], pFn[j], pOffset, &nData[0], sizeof(nData));
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
* ������:GB3761GetParamFnProcExt
* �䡡��: pAppCom    ����֡��Ϣ
          nPn        ��Ϣ��
          nFn        ��Ϣ��
* �䡡��: pOffset    ����Fn���ݴ�����
          pData    ��ѯ��ȡ����ָ��
          nDataLen    ��ѯ��ȥ���ݳ���
* ��  ��:RET_ERR_OK ִ�гɹ�  RET_ERR_ERROR ִ��ʧ���˳� RET_ERR_RANG ���ݵ�Ԫ��ƫ�ƿ���ִ����һ��FN
* ��  ��:������ѯ3761��׼Fn�������
*********************************************************************************/
INT16U GB3761GetParamFnProcExt(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen)
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
        case F81:      //F81�����ڹ�Լ
        {
        	nDataLen += GB3761GetParamExtF81(nPn, &pData[nDataLen], nMaxLen);
            break;
        }
        case F82:      //F82GPRS����״̬
        {
        	nDataLen += GB3761GetParamExtF82(nPn, &pData[nDataLen], nMaxLen);
            break;
        }
        case F83:      //F83��̫������״̬
        {
        	nDataLen += GB3761GetParamExtF83(nPn, &pData[nDataLen], nMaxLen);
            break;
        }
        case F84:      //F84���ɱ��
        {
        	nDataLen += GB3761GetParamExtF84(nPn, &pData[nDataLen], nMaxLen);
            break;
        }
        default:
        {
            pInfo->bNoDataFlag = TRUE;
            break;
        }
    }
    if (nDataLen > 4)
    {
    	return nDataLen;
    }
    return 0;
}


//F81�����ڹ�Լ
INT16U GB3761GetParamExtF81(INT16U nPn, INT8U *pData, INT16U nMaxDataLen)
{
    INT8U   nDataLen = 0;
    
    if (NULL == pData || 0 != nPn)
	{
		return 0;
	}
    
    pData[nDataLen++] = g_pMem->sXRamData.nRdMtProtFlg;
    return nDataLen;
}

//F82GPRS����״̬
INT16U GB3761GetParamExtF82(INT16U nPn, INT8U *pData, INT16U nMaxDataLen)
{
    INT8U   nDataLen = 0;
    
    if (NULL == pData || 0 != nPn)
	{
		return 0;
	}
    
    if (g_pRmotValClr->nLoginSucc > 0)
    {
        pData[nDataLen++] = 1;
        SendAtCmdAndWaitATRespSlip((char*)"AT#SH=1\r\n", (char*)g_Modl_OK, 1000, 1);
    }
    else
    {
        pData[nDataLen++] = 0;
    }
    return nDataLen;
}

//F83��̫������״̬
INT16U GB3761GetParamExtF83(INT16U nPn, INT8U *pData, INT16U nMaxDataLen)
{
    INT8U   nDataLen = 0;
    
    if (NULL == pData || 0 != nPn)
	{
		return 0;
	}
    
    if (g_sEnetStateClr.bEnetCreatSockSucc)
    {
        if (g_sEnetStateClr.nSock >= 0)
        {
            //close(g_sEnetStateClr.nSock);
        }
    }
    pData[nDataLen++] = g_sEnetStateClr.bEnetCreatSockSucc;
    return nDataLen;
}

//F84���ɱ��
INT16U GB3761GetParamExtF84(INT16U nPn, INT8U *pData, INT16U nMaxDataLen)
{
    INT8U   nDataLen = 0;
    
    if (NULL == pData || 0 != nPn)
	{
		return 0;
	}
    
    nDataLen += MemReadCommPrm(F95, &pData[nDataLen], 8);
    return nDataLen;
}