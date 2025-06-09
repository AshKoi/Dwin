#include "GB698ProxyRequest.h"
#include "Gw3762.h"
#include "MemDrvExt.h"
#include "Table.h"
#include "Fun.h"
#include "Macro.h"
#include "SysRes.h"
#include "Base.h"
#include "ChkCrc.h"
#include "Guimenu.h"
#include "CarrCol.h"
#include "meter645.h"
#include "GB3761SetPrmExt.h"

/**************************************************************************************************
                                           相关函数实现
***************************************************************************************************/
/********************************************************************************
* 函数名: GB698ProxyRequest
* 输　入:
* 输　出:
* 返  回:
* 功  能: 数据转发
*********************************************************************************/
INT8S GB698ProxyRequest(pAPP_COMM pComm)
{
    INT16U 		nPn[8];
	INT16U 		nFn[GB3761_MAX_FN_NUM];
	INT16U		nPnNum   = 0;
	INT8U		nFnNum   = 0;
	INT16U  	nOffset  = 0;
	pGB3761_FRM_INFO pInfo = NULL;

	if (NULL == pComm)
    {
    	return RET_ERR_POINT;
    }

    pInfo = (pGB3761_FRM_INFO)(pComm->pFrmInfo);
	while (nOffset + 4 <= pInfo->nRxDataLen)
	{
		memset(&nPn[0], 0xFFFF, sizeof(nPn));
        memset(&nFn[0], 0, sizeof(nFn));
		if (RET_ERR_OK != GB3761DaDt2PnFn(&pComm->pRxFrmBuf[GB3761_DATA_IDX + nOffset], &nPn[0], &nPnNum, &nFn[0], &nFnNum, &pInfo->bAllMpFlag))  //Dadt转换Pnfn
		{
            break;
        }
        nOffset += 4;
        if (RET_ERR_OK != GB3761TransDataProc(pComm, &nPn[0], nPnNum, &nFn[0], nFnNum, &nOffset))
        {
            break;
        }
	}
	return RET_ERR_OK;
}
