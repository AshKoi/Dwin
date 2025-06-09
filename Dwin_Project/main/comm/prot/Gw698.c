#include "Gw698.h"
#include "SecTask.h"
INT16U   g_nHeadLen = 0;//帧头长度68到帧头校验后

/****************************************************************************
* 函数名: GB3761FillTxFrm
* 输　入: pRxInfo  帧信息
          FrmType     帧类型
* 输　出: pFrmInfoTx  帧信息
* 返  回: 接口标准返回值
* 功  能: 发送前填充帧信息
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

    pTxMap->nHead = pRxMap->nHead;   //报文头
    
    if (!pRxMap->uCtrl.sDet.nDir)      //传输方向位 下行置上行
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
* 函数名: GB3761MakeFrm
* 输　入: pFrmInfoTx 发送帧信息
* 输　出: pBuf 发送帧信息
* 返  回: 接口标准返回值
* 功  能: 根据发送帧信息中的内容组装成一帧报文
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
    
    //帧头校验
    getCrcProc(&pFrm[1], &nHeadlHFS, &nHeadhHFS, g_nHeadLen-3);

    pMap->nlHCS = nHeadlHFS;
    pMap->nhHCS = nHeadhHFS;
    //帧尾校验
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
    {    // 请求类型 5-读取
        case GB698_REQUEST_CONNECT:     //建立应用连接请求
            break;
        case GB698_REQUEST_RELEASE:     //断开应用连接请求
            break;
        case GB698_REQUEST_GET:         //读取请求
            nRet = GB698GetRequest(pComm);
            break;
        case GB698_REQUEST_SET:         //设置请求
            nRet = GB698SetRequest(pComm);
            break;
        case GB698_REQUEST_ACTION:      //操作请求
            nRet = GB698ActionRequest(pComm);
            break;
        case GB698_REQUEST_REPORT:      //上报应答
            //nRet = GB698ReportRequest(pComm);
            break;
        case GB698_REQUEST_PROXY:       //代理请求
            nRet = GB698ProxyRequest(pComm);
            break;
        case GB3761_AFN_RESET:                        //终端复位
            nRet = GB3761TermReset(pComm);
            break;
        case GB3761_AFN_SET_PARAM:                    //参数设置
            nRet = GB3761SetParam(pComm);
            break;
        case GB3761_AFN_SET_PRM_EXT:                 //扩展参数设置
            nRet = GB3761SetParamExt(pComm);
            break;
        case GB3761_AFN_QUERY_PRM_EXT:                 //扩展参数查询
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
* 函数名: GB698ChkRxFrm
* 输　入:
* 输　出:
* 返  回:
* 功  能:
*********************************************************************************/
INT8S GB698ChkRxFrm(pAPP_COMM pComm)
{
	pGB698_FRM_MAP  pRxMap  = NULL;
    
    if (NULL == pComm)
    {
    	return RET_ERR_POINT;
    }
    
    pRxMap = (pGB698_FRM_MAP)pComm->pRxFrmBuf;  //表图
	
    //通讯方向判断
    if (0 != pRxMap->uCtrl.sDet.nDir)
    {
    	return RET_ERR_ERROR;
    }

    return RET_ERR_OK;
}

INT32S Gw698AsynProcSub(INT8U* pRcv, INT8U* pSnd, INT32S nSndlen)
{           //传参： 1.帧头+数据  2.发送的buf  3.收到的数据长度
    pGW698_PROT	        pUpRcvLinkProt	= NULL;
    INT8U               nFrmInfo[30]    = {0};
    INT8U*              pFrmInfo        =&nFrmInfo[0];//申请的30个字节 存放帧信息
    INT16U              nFrmLen         = 0;  
    pGB698_FRM_INFO     pInfo           = NULL;//698帧信息
    APP_COMM            sTmpAppComm;   //通讯中帧信息
    pAPP_COMM           pTmpAppComm     = &sTmpAppComm;
    INT8S               nRet            = RET_ERR_ERROR;
    INT8U               nSndDataLen     = 0;

    pTmpAppComm->pRxFrmBuf = pRcv; //接受到的数据 保存在 pRxFrmBuf
    pTmpAppComm->pTxFrmBuf = pSnd; //发送的数据
    pTmpAppComm->pFrmInfo  = pFrmInfo;//存放帧信息--指向刚申请的30字节空间
    
    pUpRcvLinkProt 	= (pGW698_PROT)(pTmpAppComm->pRxFrmBuf);
    
    g_nHeadLen = (pUpRcvLinkProt->nMeterAddrSAFlg & 0x0F) + 9;//帧头长度68到帧头校验后  9+nAddr
    
    pInfo  = (pGB698_FRM_INFO)pTmpAppComm->pFrmInfo;   //获取分析帧信息的指针
    
    memset(pInfo, 0, sizeof(pGB698_FRM_INFO));
    pInfo->bAllMpFlag       = FALSE;
    pInfo->bSendFlag        = FALSE;
    pInfo->bFirstFlag       = TRUE;
    pInfo->bSucFlag         = FALSE;
    pInfo->bFailFlag        = FALSE;
    pInfo->bNoDataFlag      = FALSE;
    
    pTmpAppComm->nFrmLen    = 1024;
    //传入 pTmpAppComm
    if (RET_ERR_OK != ( nRet = GB698ChkRxFrm(pTmpAppComm) ) )
	{//通讯方向判断
		return nRet;
	}
    if ( RET_ERR_OK != ( nRet = GB698AlyRxFrm(pTmpAppComm) ) )
	{//具体分情况处理所需要的数据  得到    OAD和数据长度 如 40 00 02 00  00
		return nRet;
	}
    if ( RET_ERR_OK != ( nRet = GB698FillTxFrm(pTmpAppComm) ) )
	{//修饰一下  除了两个校验
		return nRet;
	}
    //算一些校验  --  //nFollwReport  nTimeTag 写死为0
    GB698MakeFrmTmp(&pTmpAppComm->pTxFrmBuf[0], pInfo->nTxDataLen, &nFrmLen);
    
    return nFrmLen;
}