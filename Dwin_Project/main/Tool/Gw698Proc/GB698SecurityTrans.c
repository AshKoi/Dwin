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
INT8S GB3761SecurityTrans(pAPP_COMM pComm)
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
    INT8U               nContextLen   = 0;
    if (NULL == pComm)
    {
    	return RET_ERR_POINT;
    }

    pInfo = (pGB698_FRM_INFO)(pComm->pFrmInfo);
    pMap    = (pGB698_FRM_MAP)&pComm->pRxFrmBuf[0];
    
    nContextLen = pMap->nPIID;
    
    nRet = GB698GetRecordResp(&pMap->nData[3], &nData[4], &nDataLen);
    nData[1] = 0x85;
    nData[2] = 0x03;
    nData[3] = 0x00;
    nDataLen += 3;
    nData[0] = nDataLen;
    nDataLen ++;
    nData[nDataLen ++] = 0x01;
    nData[nDataLen ++] = 0x00;
    nData[nDataLen ++] = 0x04;                          //MAC地址Byte长度 。待修正  
    nData[nDataLen ++] = 0x14;                        //MAC地址。待修正  4Byte
    nData[nDataLen ++] = 0x24;
    nData[nDataLen ++] = 0x34;
    nData[nDataLen ++] = 0x44;
    
    pInfo->nTxDataLen += MemCopy(&pComm->pTxFrmBuf[g_nHeadLen + 2], &nData[0], nDataLen);
    if (nDataLen <= 0)
    {
        return RET_ERR_ERROR;
    }
    return RET_ERR_OK;
  
}
