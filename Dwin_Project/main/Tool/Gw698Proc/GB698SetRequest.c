#include "GB698SetRequest.h"
#include "Gw698.h"
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


/**************************************************************************************************
                                           相关函数实现
***************************************************************************************************/
/********************************************************************************
* 函数名: GB698SetRequest
* 输　入:
* 输　出:
* 返  回:
* 功  能: 参数设置
*********************************************************************************/
INT8S GB698SetRequest(pAPP_COMM pComm)
{
    INT8U     	        nData[256];
    INT8U               nDataLen = 0;
    INT8S               nRet;
    pGB698_FRM_MAP      pMap        = NULL;
    pGB698_FRM_INFO    pInfo  = NULL;
    INT32U              nOffset  = 0;
    INT8U               nRequestObject = 0;
    INT8U               i = 0;
    INT8U               nSetReqLen = GW698_APDU_LEN1; 
    INT8U               nAllDataLen   = 0;
    if (NULL == pComm)
    {
    	return RET_ERR_POINT;
    }

    pInfo = (pGB698_FRM_INFO)(pComm->pFrmInfo);
    pMap    = (pGB698_FRM_MAP)&pComm->pRxFrmBuf[0];
    nRequestObject =  pMap->nRequestObject;
    if( 1 == nRequestObject)
    {//返回  OAD+Data 如 40 00 02 00 00 
        nRet = GB698SetRequestSendResp(&pMap->nData[0], &nData[0], &nDataLen);
        pInfo->nAckType = nData[0];
        nAllDataLen += nDataLen;
    }
    else if( 2 == nRequestObject)
    {
        nData[0] = pMap->nData[0];
        nAllDataLen ++;
        for(i = 0; i < pMap->nData[0]; i ++)
        {
            nRet = GB698SetRequestSendResp(&(pMap->nData[1]) + nOffset, &nData[nAllDataLen], &nDataLen);
            nAllDataLen += nDataLen;
            nOffset += nRet;
        }
        pInfo->nAckType = nData[1];                //OAD—Num 
    }
    pInfo->nTxDataLen += MemCopy(&pComm->pTxFrmBuf[g_nHeadLen + nSetReqLen], &nData[0], nAllDataLen);
    if (nDataLen <= 0)
    {
        return RET_ERR_ERROR;
    }
    return RET_ERR_OK;
  
}

INT8S GB698SetRequestSendResp(INT8U*  pRecData, INT8U *pData, INT8U *pDatalen)
{
    INT16U      nOI;

    INT8U       nRetlen = 0;
    INT32S      nDatalen  = 0;
    INT8U       nOffset = 0;
    INT8U       nWay = 0;
    INT8U       nErrType;
    INT16U      nOI2;
    INT16U      nOI3;
    INT8U       ii;
    INT8U       nCount=0;
    MT_OAD_DATA nOAD;
    
    
    nOI = (((INT16U)(pRecData[0]) << 8) | pRecData[1]);
    nOffset += 4;
    nWay = pRecData[2];
    memcpy(pData, pRecData, OAD_LEN);
    nRetlen += 4;
    

    
    switch (nOI)
    {
        case 0x4000:
        {
            nDatalen = GB698Set4000Proc(&pRecData[nOffset], nWay, &nErrType);
            break;
        }
        case 0x4001:
        {
            nDatalen = GB698Set4001Proc(&pRecData[nOffset], nWay, &nErrType);
            break;
        }
        case 0x4002:
        {
            nDatalen = GB698Set4002Proc(&pRecData[nOffset], nWay, &nErrType);
            break;
        }
        case 0x4003:
        {
            nDatalen = GB698Set4003Proc(&pRecData[nOffset], nWay, &nErrType);
            break;
        }
        
        
        default:
            nErrType = e_NoObject;
            break;
    }
    
    pData[nRetlen++] = nErrType;
    
    *pDatalen = nRetlen;
    return (nDatalen + OAD_LEN);            //返回已处理数据长度+OAD长度
    
    
}
//时间
INT8S   GB698Set4000Proc(INT8U*   pData, INT8U  nWay, INT8U*  pErr)
{
    INT8U       nOffset = 0;
    INT8U       nTemp[2] = {0};
    STD_TIME    sTime;
    pSTD_TIME   pTime = &sTime;

    memset(pTime, 0, sizeof(STD_TIME));
    if(pData[nOffset ++] != e_date_time_s)
    {
        *pErr = e_TypeErr;
        return  -1;
    }
    memcpy(&nTemp[0], &pData[nOffset], 2);
    nOffset += 2;
    sTime.nYear = INT8U_TO_BCD((INT8U)((INT16U)(nTemp[0]) * 256 + (INT16U)nTemp[1] - 2000));
    sTime.nMon = INT8U_TO_BCD(pData[nOffset]);
    nOffset ++;
    sTime.nDay = INT8U_TO_BCD(pData[nOffset]);
    nOffset ++;
    sTime.nHour = INT8U_TO_BCD(pData[nOffset]);
    nOffset ++;
    sTime.nMin = INT8U_TO_BCD(pData[nOffset]);
    nOffset ++;
    sTime.nSec = INT8U_TO_BCD(pData[nOffset]);
    
    if (!IsVldStdTime(&sTime))
    {
        *pErr = e_BeyondLim;
        return  -1;
    }
    if (RET_ERR_OK != SyncSysClk(&sTime))
    {
        *pErr = e_BeyondLim;
        return  -1;
    }
    
    *pErr = e_Suc;
    return   1;
}
//地址
INT8S   GB698Set4001Proc(INT8U*   pData, INT8U  nWay, INT8U*  pErr)
{
    INT8U       nOffset = 0;
    INT8U       nTemp[18] = {0};
    INT8U       i = 0;
    INT8U       nSuccflag=0;  INT8U  nAddrLen =0;
    if(pData[nOffset ++] != e_octet_string)
    {
        *pErr = e_TypeErr;
        return  -1;
    }
    
    if( (nAddrLen = pData[nOffset])!=6 )
    {
        *pErr = e_TypeErr;
        return  -1;
    }
    for( i = pData[nOffset++]; i > 0; i --)
    {
        nTemp[i-1] = pData[ nOffset++ ];
    }
    if( ( nSuccflag = MemWriteAcPrm(F19, &nTemp[0], LEN_METER_ADDR) )!=0 )
    { 
        *pErr = e_TypeErr;
        return  -1;
    }
	//同时将表号写入
	MemWriteAcPrm(F20, &nTemp[0], LEN_METER_ADDR);
	memcpy(&g_nMemAddr[0], &nTemp[0], LEN_METER_ADDR);
    
    *pErr = e_Suc;
    return  1;
}

//地址
INT8S   GB698Set4002Proc(INT8U*   pData, INT8U  nWay, INT8U*  pErr)
{
    INT8U       nOffset = 0;
    INT8U       nTemp[18] = {0};
    INT8U       i = 0;
    INT8U       nSuccflag=0;  INT8U  nAddrLen =0;
    if(pData[nOffset ++] != e_octet_string)
    {
        *pErr = e_TypeErr;
        return  -1;
    }
    
    if( (nAddrLen = pData[nOffset])!=6 )
    {
        *pErr = e_TypeErr;
        return  -1;
    }
    for( i = pData[nOffset++]; i > 0; i --)
    {
        nTemp[i-1] = pData[ nOffset++ ];
    }
    if( ( nSuccflag = MemWriteAcPrm(F21, &nTemp[0], LEN_METER_ADDR) )!=0 )
    { 
        *pErr = e_TypeErr;
        return  -1;
    }
	memcpy(&g_nMemProductAddr[0], &nTemp[0], LEN_METER_ADDR);
    
    *pErr = e_Suc;
    return  1;
}

//生产编号
INT8S   GB698Set4003Proc(INT8U*   pData, INT8U  nWay, INT8U*  pErr)
{
    INT8U       nOffset = 0;
    INT8U       nTemp[18] = {0};
    INT8U       i = 0;
    if(pData[nOffset ++] != e_octet_string)
    {
        *pErr = e_TypeErr;
        return  -1;
    }
    nTemp[0] = pData[nOffset];
    for( i = pData[nOffset]; i > 0; i --)
    {
        nTemp[i] = pData[ ++ nOffset];
    }
    MemWriteAcPrm(F61, &nTemp[0], nTemp[0] + 1);
    *pErr = e_Suc;
    return  1;
}
