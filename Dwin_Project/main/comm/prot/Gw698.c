#include "Gw698.h"
#include "SecTask.h"
INT16U   g_nHeadLen = 0;//֡ͷ����68��֡ͷУ���

/****************************************************************************
* ������: GB3761FillTxFrm
* �䡡��: pRxInfo  ֡��Ϣ
          FrmType     ֡����
* �䡡��: pFrmInfoTx  ֡��Ϣ
* ��  ��: �ӿڱ�׼����ֵ
* ��  ��: ����ǰ���֡��Ϣ
****************************************************************************/
INT8S GB698FillTxFrm(pAPP_COMM pComm)
{
    INT8S   nRet = 0;
    INT8U   nTempBuf[6] = {0};
    BOOLEAN bACDFlag = FALSE;
    STD_TIME sTime;
    pSTD_TIME  pTime = &sTime;

    pGB698_FRM_INFO    pInfo  = NULL;
    pGB698_FRM_MAP     pRxMap = NULL;
    pGB698_FRM_MAP     pTxMap = NULL;
    
    if (NULL == pComm)
    {
    	return RET_ERR_POINT;
    }
    pInfo  = (pGB698_FRM_INFO)pComm->pFrmInfo;
    pRxMap = (pGB698_FRM_MAP)&pComm->pRxFrmBuf[0];
    pTxMap = (pGB698_FRM_MAP)&pComm->pTxFrmBuf[0];

    pTxMap->nHead = pRxMap->nHead;   //����ͷ
    
    if (!pRxMap->uCtrl.sDet.nDir)      //���䷽��λ ����������
    {
        pTxMap->uCtrl = pRxMap->uCtrl;
        pTxMap->uCtrl.sDet.nDir = 1;
    }
    
    pTxMap->nSAFlg = pRxMap->nSAFlg;
    
    MemReadAcPrm(F19, &nTempBuf[0], 6);
    //SwapBufVal(&nTempBuf[0], 6);
    memcpy(pTxMap->nSAAddr, &nTempBuf[0], (pRxMap->nSAFlg & 0x0F)+1);
    
    pTxMap->nCA = pRxMap->nCA;
    pTxMap->nRequestType = pRxMap->nRequestType | 0x80;
    
    pTxMap->nRequestObject = pRxMap->nRequestObject;
    if( 0x10 != pRxMap->nRequestType )
    {
        pTxMap->nPIID = pRxMap->nPIID;
    }
   // memcpy(pTxMap->nOAD, pRxMap->nOAD, 4);

    return RET_ERR_OK;
}

/****************************************************************************
* ������: GB3761MakeFrm
* �䡡��: pFrmInfoTx ����֡��Ϣ
* �䡡��: pBuf ����֡��Ϣ
* ��  ��: �ӿڱ�׼����ֵ
* ��  ��: ���ݷ���֡��Ϣ�е�������װ��һ֡����
****************************************************************************/
INT8S GB698MakeFrmTmp(INT8U *pFrm, INT16U nDataLen, INT16U *pFrmLen)
{
    pGB698_FRM_INFO pInfo  = NULL;
    pGB698_FRM_MAP  pMap   = NULL;
    INT16U   nFremLenTmp;
    INT8U    nHeadlHFS;
    INT8U    nHeadhHFS;
    INT8U    nlHFS;
    INT8U    nhHFS;
    INT16U   nTemp;
    INT8U    nTempBuf[10];

    if (NULL == pFrm)
    {
        return RET_ERR_POINT;
    }

    pMap = (pGB698_FRM_MAP)pFrm;
    if( 0x10 == pMap->nRequestType)
    {
        nTemp = g_nHeadLen -1 + 2 + nDataLen + 4;
    }
    else
    {
        nTemp = g_nHeadLen -1 + GW698_APDU_LEN1 + nDataLen + 4;
    } 
    INT16U2Buf(&nTempBuf[0], nTemp);
    
    nFremLenTmp = nTemp + 2;
    
    pMap->uLen.nWord = nTempBuf[0];
    pMap->nLen = nTempBuf[1];
    
    pFrm[nFremLenTmp - 5] = 0; //nFollwReport
    pFrm[nFremLenTmp - 4] = 0;//nTimeTag
    
    //֡ͷУ��
    getCrcProc(&pFrm[1], &nHeadlHFS, &nHeadhHFS, g_nHeadLen-3);

    pMap->nlHCS = nHeadlHFS;
    pMap->nhHCS = nHeadhHFS;
    //֡βУ��
    getCrcProc(&pFrm[1], &nlHFS, &nhHFS, nTemp - 2);
    
    pFrm[nFremLenTmp - 3] = nlHFS;
    pFrm[nFremLenTmp - 2] = nhHFS;
    pFrm[nFremLenTmp - 1] = 0x16;
    
    *pFrmLen = nFremLenTmp;

    return RET_ERR_OK;
}

INT8S GB698AlyRxFrm(pAPP_COMM pComm)
{
    INT8S               nRet        = RET_ERR_ERROR;
	INT16U              nDataLen    = 0;
    INT8U               nLen        = 0;
    pGB698_FRM_MAP      pMap        = NULL;
    
    if (NULL == pComm)
	{
		return RET_ERR_POINT;
	}
    pMap    = (pGB698_FRM_MAP)&pComm->pRxFrmBuf[0];
    
    if(g_MeterPowerOff && pMap->nRequestType != GB698_REQUEST_ACTION)
    {
        return RET_ERR_ERROR;
    }
    
    switch (pMap->nRequestType)
    {    // �������� 5-��ȡ
        case GB698_REQUEST_CONNECT:     //����Ӧ����������
            break;
        case GB698_REQUEST_RELEASE:     //�Ͽ�Ӧ����������
            break;
        case GB698_REQUEST_GET:         //��ȡ����
            nRet = GB698GetRequest(pComm);
            break;
        case GB698_REQUEST_SET:         //��������
            nRet = GB698SetRequest(pComm);
            break;
        case GB698_REQUEST_ACTION:      //��������
            nRet = GB698ActionRequest(pComm);
            break;
        case GB698_REQUEST_REPORT:      //�ϱ�Ӧ��
            //nRet = GB698ReportRequest(pComm);
            break;
        case GB698_REQUEST_PROXY:       //��������
            nRet = GB698ProxyRequest(pComm);
            break;
        case GB3761_AFN_RESET:                        //�ն˸�λ
            nRet = GB3761TermReset(pComm);
            break;
        case GB3761_AFN_SET_PARAM:                    //��������
            nRet = GB3761SetParam(pComm);
            break;
        case GB3761_AFN_SET_PRM_EXT:                 //��չ��������
            nRet = GB3761SetParamExt(pComm);
            break;
        case GB3761_AFN_QUERY_PRM_EXT:                 //��չ������ѯ
            nRet = GB3761GetParamExt(pComm);
            break;
        case  GB698_REQUEST_SECURITY:
            nRet = GB3761SecurityTrans(pComm);
        default:
            nRet = RET_ERR_ERROR;
            break;
    }
    return nRet;
}

/********************************************************************************
* ������: GB698ChkRxFrm
* �䡡��:
* �䡡��:
* ��  ��:
* ��  ��:
*********************************************************************************/
INT8S GB698ChkRxFrm(pAPP_COMM pComm)
{
	pGB698_FRM_MAP  pRxMap  = NULL;
    
    if (NULL == pComm)
    {
    	return RET_ERR_POINT;
    }
    
    pRxMap = (pGB698_FRM_MAP)pComm->pRxFrmBuf;  //��ͼ
	
    //ͨѶ�����ж�
    if (0 != pRxMap->uCtrl.sDet.nDir)
    {
    	return RET_ERR_ERROR;
    }

    return RET_ERR_OK;
}

INT32S Gw698AsynProcSub(INT8U* pRcv, INT8U* pSnd, INT32S nSndlen)
{           //���Σ� 1.֡ͷ+����  2.���͵�buf  3.�յ������ݳ���
    pGW698_PROT	        pUpRcvLinkProt	= NULL;
    INT8U               nFrmInfo[30]    = {0};
    INT8U*              pFrmInfo        =&nFrmInfo[0];//�����30���ֽ� ���֡��Ϣ
    INT16U              nFrmLen         = 0;  
    pGB698_FRM_INFO     pInfo           = NULL;//698֡��Ϣ
    APP_COMM            sTmpAppComm;   //ͨѶ��֡��Ϣ
    pAPP_COMM           pTmpAppComm     = &sTmpAppComm;
    INT8S               nRet            = RET_ERR_ERROR;
    INT8U               nSndDataLen     = 0;

    pTmpAppComm->pRxFrmBuf = pRcv; //���ܵ������� ������ pRxFrmBuf
    pTmpAppComm->pTxFrmBuf = pSnd; //���͵�����
    pTmpAppComm->pFrmInfo  = pFrmInfo;//���֡��Ϣ--ָ��������30�ֽڿռ�
    
    pUpRcvLinkProt 	= (pGW698_PROT)(pTmpAppComm->pRxFrmBuf);
    
    g_nHeadLen = (pUpRcvLinkProt->nMeterAddrSAFlg & 0x0F) + 9;//֡ͷ����68��֡ͷУ���  9+nAddr
    
    pInfo  = (pGB698_FRM_INFO)pTmpAppComm->pFrmInfo;   //��ȡ����֡��Ϣ��ָ��
    
    memset(pInfo, 0, sizeof(pGB698_FRM_INFO));
    pInfo->bAllMpFlag       = FALSE;
    pInfo->bSendFlag        = FALSE;
    pInfo->bFirstFlag       = TRUE;
    pInfo->bSucFlag         = FALSE;
    pInfo->bFailFlag        = FALSE;
    pInfo->bNoDataFlag      = FALSE;
    
    pTmpAppComm->nFrmLen    = 1024;
    //���� pTmpAppComm
    if (RET_ERR_OK != ( nRet = GB698ChkRxFrm(pTmpAppComm) ) )
	{//ͨѶ�����ж�
		return nRet;
	}
    if ( RET_ERR_OK != ( nRet = GB698AlyRxFrm(pTmpAppComm) ) )
	{//����������������Ҫ������  �õ�    OAD�����ݳ��� �� 40 00 02 00  00
		return nRet;
	}
    if ( RET_ERR_OK != ( nRet = GB698FillTxFrm(pTmpAppComm) ) )
	{//����һ��  ��������У��
		return nRet;
	}
    //��һЩУ��  --  //nFollwReport  nTimeTag д��Ϊ0
    GB698MakeFrmTmp(&pTmpAppComm->pTxFrmBuf[0], pInfo->nTxDataLen, &nFrmLen);
    
    return nFrmLen;
}