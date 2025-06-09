/**************************************************************************************************
* Copyright (c) 2010-2011 ��һ�Ƽ��ɷ����޹�˾�з�����
* �ļ���:
* ������: �з�����
* �ա���: 2010/08/18
* �޸���:
* �ա���:
* �衡��:
* �桡��: Version 1.0
***************************************************************************************************/
#include "GB3761CtrlCmd.h"
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
* ������: GB3761CtrlCmd
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ��������
*********************************************************************************/
INT8S GB3761CtrlCmd(pAPP_COMM pComm)
{
	INT8U  nAllMp = 0;
    INT8U  nFnNum = 0;
    INT16U nFn[GB3761_MAX_FN_NUM];
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
        if (RET_ERR_OK != GB3761DaDt2PnFn(&pComm->pRxFrmBuf[GB3761_DATA_IDX + nOffset], &nPn[0], &nPnNum, &nFn[0], &nFnNum, &nAllMp))  //Dadtת��Pnfn
		{
			break;
        }
        nOffset += 4;                            //�Ѿ��������ݱ�ʶ�����ݵ�Ԫ����
        if (RET_ERR_OK != GB3761CtrlCmdProc(pComm, &nPn[0], nPnNum, &nFn[0], nFnNum, &nOffset))
        {
            break;
        }
    }
    return RET_ERR_OK;
}

/********************************************************************************
* ������: GB3761CtrlCmdProc
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��: ��������
*********************************************************************************/
INT8S GB3761CtrlCmdProc(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset)
{
    INT16U 	   i     = 0;                                //ѭ������
    INT16U 	   j     = 0;                                //ѭ������
	INT16U	   nDataLen = 0;
	INT8U      nData[50];               //g_nProtTmpData
    INT16U     nMaxDataLen = 0;
    pGB3761_FRM_INFO pInfo = NULL;

    if (NULL == pComm || NULL == pPn || NULL == pFn || NULL == pOffset)
    {
    	return RET_ERR_POINT;
    }

    pInfo = (pGB3761_FRM_INFO)pComm->pFrmInfo;
	nMaxDataLen = pComm->nFrmLen - GB3761_FRM_RES_LEN;
    for (i = 0; i < nPnNum; i++)   //���������PnFnִ�в������������
    {
        for (j = 0; j < nFnNum; j++)
        {
        	nDataLen = 0;
            memset(&nData[0], 0, sizeof(nData));   //�����������
            nDataLen += GB3761CtrlCmdFnProc(pComm, pPn[i], pFn[j], pOffset, &nData[0], sizeof(nData));
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
* ������:GB3761CtrlCmdFnProc
* �䡡��: pRxInfo ����֡��Ϣ
          nPn        ��Ϣ��
          nFn        ��Ϣ��
* �䡡��: pOffset    ����Fn���ݴ�����
* ��  ��:RET_ERR_OK ִ�гɹ�  RET_ERR_ERROR ִ��ʧ���˳� RET_ERR_RANG ���ݵ�Ԫ��ƫ�ƿ���ִ����һ��FN
* ��  ��:��������3761��׼Fn�������
*********************************************************************************/
INT16U GB3761CtrlCmdFnProc(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen)
{
	BOOLEAN bSetSuc = FALSE;
    INT16U  nDataLen = 0;
    pGB3761_FRM_INFO pInfo = NULL;

    if (NULL == pComm || NULL == pOffset || NULL == pData)
    {
        return 0;
    }
    pInfo = (pGB3761_FRM_INFO)pComm->pFrmInfo;
	if (RET_ERR_OK != GB3761PnFn2DADT(nPn, nFn, &pData[nDataLen]))
    {
        return 0;
    }
    nDataLen += 4;
    switch (nFn) //����ͬFn��
    {
    	case F1:     //ң����բ
        {
        	bSetSuc = GB3761CtrlCmdF1(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F2:     //�����բ
        {
        	bSetSuc = GB3761CtrlCmdF2(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F25:     //�ն��Զ��������
        {
        	bSetSuc = GB3761CtrlCmdF25(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F31:     //��ʱ����
        {
        	bSetSuc = GB3761CtrlCmdF31(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F33:     //�ն��Զ�������
        {
        	bSetSuc = GB3761CtrlCmdF33(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F38:     //�����ն�������վ
        {
        	bSetSuc = GB3761CtrlCmdF38(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F39:     //�����ն˶Ͽ�����
        {
        	bSetSuc = GB3761CtrlCmdF39(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F49:  //����ָ��ͨ�Ŷ˿���ͣ����
        {
        	bSetSuc = GB3761CtrlCmdF49(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F50:  //����ָ��ͨ�Ŷ˿ڻָ�����
        {
        	bSetSuc = GB3761CtrlCmdF50(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
		case F51:  //����ָ��ͨ�Ŷ˿����³���
        {
        	bSetSuc = GB3761CtrlCmdF51(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        default:
        {
        	pInfo->bNoDataFlag = TRUE;
            break;
        }
    }
	if (bSetSuc)
    {
    	pInfo->bSucFlag = TRUE;
    	pData[nDataLen++] = 0;
    }
    else
    {
    	pInfo->bFailFlag = TRUE;
    	pData[nDataLen++] = 1;
    }
    return nDataLen;
}

//F1  ң����բ
BOOLEAN GB3761CtrlCmdF1(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT16U  nIndex      = 0;
	INT8U   nFnLen      = 0;
    INT8U   nCtrlData   = 0;
    INT8U   nTurns      = 0;
    STD_TIME        sTime;
    CTRL_TURN_PRM   sCtrlPrm;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
    memset(&sTime, 0, sizeof(STD_TIME));
    memset(&sCtrlPrm, 0, sizeof(sCtrlPrm));
    nIndex = *pOffset;
	nFnLen = 1;
	if ((nIndex + nFnLen) > nDataLen)  //������Ч�����ж�
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 == nPn || nPn > MAX_TURN_NUM)
    {
    	*pOffset += nFnLen;
    	return FALSE;
    }
    
    nCtrlData = pData[nIndex];
    GetSysClk(&sTime);
    memcpy(&sCtrlPrm.sCtrlCmdSetTime, &sTime.nSec, 6);
    sCtrlPrm.nAlmMin = (nCtrlData >> 4) & 0x0F;
    sCtrlPrm.nAstPower30Min = nCtrlData & 0x0F; 
    nTurns = nPn - 1;
    //д����
    MemWriteCtrlPrm(F1, nTurns, &sCtrlPrm, sizeof(sCtrlPrm));
    g_nTripFistFlg[nTurns] = 0;
    g_nTurnSwitckCnt[nTurns] = 0;
    g_nCycelCnt[nTurns] = 0;
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F2  �����բ
BOOLEAN GB3761CtrlCmdF2(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT16U  nIndex = 0;
	INT8U   nFnLen = 0;
    INT8U   nTurns  = 0;
    STD_TIME        sTime;
    CTRL_TURN_PRM   sCtrlPrm;
	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
    
    memset(&sTime, 0, sizeof(STD_TIME));
    memset(&sCtrlPrm, 0, sizeof(sCtrlPrm));
    nIndex = *pOffset;
	nFnLen = 0;
	if ((nIndex + nFnLen) > nDataLen)  //������Ч�����ж�
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 == nPn || nPn > MAX_TURN_NUM)
    {
    	*pOffset += nFnLen;
    	return FALSE;
    }
    nTurns = nPn - 1;
    memset(&sCtrlPrm, 0xFF, sizeof(sCtrlPrm)); 
    MemWriteCtrlPrm(F1, nTurns, &sCtrlPrm, sizeof(sCtrlPrm));
    return TRUE;
}


//F25  �ն��Զ��������
BOOLEAN GB3761CtrlCmdF25(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT16U  nIndex      = 0;
	INT8U   nFnLen      = 0;
    INT8U   nCtrlData   = 0;
    STD_TIME        sTime;
    KEEP_POWER_PRM  sKeepPowerPrm;

    memset(&sTime, 0, sizeof(sTime));
    memset(&sKeepPowerPrm, 0, sizeof(sKeepPowerPrm));
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
    nCtrlData = pData[nIndex];     //����Ͷ��ʱ��
    GetSysClk(&sTime);
    sKeepPowerPrm.nKeepPowerFlg = 0x55;
    memcpy(&sKeepPowerPrm.sKeepStartTime, &sTime, sizeof(sTime));
    sKeepPowerPrm.nKeepPowerTime = nCtrlData;
    MemWriteCtrlPrm(F25, 0, &sKeepPowerPrm, sizeof(sKeepPowerPrm));
    
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F31 ��ʱ����
BOOLEAN GB3761CtrlCmdF31(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U       nFnLen      = 0;
	INT16U      nIndex      = 0;
	STD_TIME    sTime;
    STD_TIME    sLTime;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
    nIndex = *pOffset;
	nFnLen = 6;
	if ((nIndex + nFnLen) > nDataLen)  //11-01-04 ����������Ч�����ж�
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 != nPn)
    {
    	*pOffset += nFnLen;
    	return FALSE;
    }
    
    sTime.nSec  = pData[nIndex++];
    sTime.nMin  = pData[nIndex++];
    sTime.nHour = pData[nIndex++];
    sTime.nDay  = pData[nIndex++];
    sTime.nMon  = pData[nIndex] & 0x1F;
    sTime.nWeek = (pData[nIndex++]>>5) & 0x07;
    sTime.nYear = pData[nIndex++];
    if (!IsVldStdTime(&sTime))
    {
        return FALSE;
    }
    if (RET_ERR_OK != SyncSysClk(&sTime))
    {
        return FALSE;
    }
    
    nIndex += nFnLen;
    *pOffset = nIndex;
	return TRUE;
}

//F33  �ն��Զ�������
BOOLEAN GB3761CtrlCmdF33(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT16U  nIndex = 0;
	INT8U   nFnLen = 0;
    KEEP_POWER_PRM  sKeepPowerPrm;
    
    memset(&sKeepPowerPrm, 0, sizeof(sKeepPowerPrm));
	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
    nIndex = *pOffset;
	nFnLen = 0;
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
    sKeepPowerPrm.nKeepPowerFlg = 0xAA;
    MemWriteCtrlPrm(F25, 0, &sKeepPowerPrm, sizeof(sKeepPowerPrm));
    return TRUE;
}

//F38  �����ն�������վ
BOOLEAN GB3761CtrlCmdF38(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U   nFnLen = 0;
	INT16U  nIndex = 0;
	INT8U	nRmotWorkReq = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
    nIndex = *pOffset;
	nFnLen = 0;
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
    nRmotWorkReq = 1;
    MemWriteRamXmpData(F9, 0, &nRmotWorkReq, sizeof(nRmotWorkReq));
    return TRUE;
}

//F39  �����ն˶Ͽ�����
BOOLEAN GB3761CtrlCmdF39(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U   nFnLen = 0;
	INT16U  nIndex = 0;
	INT8U	nRmotWorkReq = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
    nIndex = *pOffset;
	nFnLen = 0;
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
    nRmotWorkReq = 0;
    MemWriteRamXmpData(F9, 0, &nRmotWorkReq, sizeof(nRmotWorkReq));
    return TRUE;
}

//F49 ����ָ��ͨ�Ŷ˿���ͣ����
BOOLEAN GB3761CtrlCmdF49(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	BOOLEAN     bStop   = FALSE;
	INT8U       nCom    = 0;
	INT8U       nFnLen  = 0;
	INT16U      nIndex  = 0;
    INT8U       i       = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
    nIndex = *pOffset;
	nFnLen = 1;
	if ((nIndex + nFnLen) > nDataLen)  //11-01-04 ����������Ч�����ж�
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 != nPn)
    {
    	*pOffset += nFnLen;
    	return FALSE;
    }
    nCom = pData[nIndex++];  //�˿ں� 1-31
    *pOffset = nIndex;
    if (0 == nCom || nCom >= MAX_COM_NUM)
    {
    	return FALSE;
    }
    for (i = 0; i < RD_TD_NUM; i++)
    {
        ClrRdMeterFlagsAllMp(i);
    }
    //bStop = TRUE;
    //MemWriteParam(MEM_OBJ_PRM_COM, nCom, MEM_COM_STOP_ID, &bStop, 1);
	return TRUE;
}

//F50 ����ָ��ͨ�Ŷ˿ڻָ�����
BOOLEAN GB3761CtrlCmdF50(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	BOOLEAN     bStop   = FALSE;
	INT8U       nCom    = 0;
	INT8U       nFnLen  = 0;
	INT16U      nIndex  = 0;
    INT8U       i       = 0;

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
    nCom = pData[nIndex++];  //�˿ں� 1-31
    *pOffset = nIndex;
    if (0 == nCom || nCom >= MAX_COM_NUM)
    {
    	return FALSE;
    }
    //bStop = FALSE;
    //MemWriteParam(MEM_OBJ_PRM_COM, nCom, MEM_COM_STOP_ID, &bStop, 1);
    //for (i = 0; i < RD_TD_NUM; i++)
    {
        SetRdMeterFlagsAllMp(i);
        RdMeterSemProc();
    }
	return TRUE;
}

//F51 ����ָ��ͨ�Ŷ˿����³���
BOOLEAN GB3761CtrlCmdF51(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
    BOOLEAN     bStop   = FALSE;
    INT32U	    bsReadInfo = 0;	//������Ϣ
	INT8U       nCom    = 0;
	INT8U       nFnLen  = 0;
	INT16U      nIndex  = 0;
    INT8U       i       = 0;

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
    nCom = pData[nIndex++];  //�˿ں� 1-31
    *pOffset = nIndex;
    if (0 == nCom || nCom >= MAX_COM_NUM)
    {
    	return FALSE;
    }
    //for (i = 0; i < RD_TD_NUM; i++)
    {
        SetRdMeterFlagsAllMp(i);
        RdMeterSemProc();
    }
	return TRUE;
}