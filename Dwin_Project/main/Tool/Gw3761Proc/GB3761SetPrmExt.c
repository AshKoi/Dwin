/******************************************************************************************************
* 2013-2014 益邦科技有限公司
* 文件名: 
* 创建人: 王一凡
* 日　期: 2013/09/21
* 修改人: 
* 日　期: 
* 描　述: 
* 版　本: V1.0
*****************************************************************************************************/
#include "GB3761SetPrmExt.h"
#include "Gw3761.h"
#include "MemDrvExt.h"
#include "Table.h"
#include "Fun.h"
#include "Macro.h"
#include "SysRes.h"
#include "Base.h"
#include "ChkCrc.h"
#include "Reset.h"
#include "CarrCol.h"
#include "SoftTimer.h"
#include "MemTbl.h"



INT8U   g_nSignalErrFlg     = 0;
INT8U   g_nCarrErrFlg       = 0;
INT8U   g_nRs485ErrFlg      = 0;
INT8U   g_nRemotCtrlFlg     = 0;

INT8U   g_nGprsLedFlg       = 0;



/********************************************************************************
* 函数名: GB3761SetParamExt
* 输　入:
* 输　出:
* 返  回:
* 功  能: 参数设置扩展
*********************************************************************************/
INT8S GB3761SetParamExt(pAPP_COMM pComm)
{
    INT16U nFn[GB3761_MAX_FN_NUM];
    INT8U  nFnNum = 0;
    INT16U nPn[8];
    INT16U nPnNum  = 0;
    INT16U nOffset = 0;     //数据标识和数据单元 处理长度偏移
    pGB3761_FRM_INFO   pInfo = NULL;

    if (NULL == pComm)
    {
    	return RET_ERR_POINT;
    }
    pInfo = (pGB3761_FRM_INFO)pComm->pFrmInfo;   //获取分析帧信息的指针
    pInfo->nTxDataLen = 5; //部分确认否认 DADT+AFN
    while (nOffset + 4 <= pInfo->nRxDataLen)
    {
        memset(&nPn[0], 0xFFFF, sizeof(nPn));
        memset(&nFn[0], 0, sizeof(nFn));
        if (RET_ERR_OK != GB3761DaDt2PnFn(&pComm->pRxFrmBuf[GB3761_DATA_IDX + nOffset], &nPn[0], &nPnNum, &nFn[0], &nFnNum, &pInfo->bAllMpFlag))  //Dadt转换Pnfn
		{
	        break;
        }
        nOffset += 4;                            //已经处理数据标识和数据单元长度
        if (RET_ERR_OK != GB3761SetParamProcExt(pComm, &nPn[0], nPnNum, &nFn[0], nFnNum, &nOffset))
        {
            break;
        }
    }
    return RET_ERR_OK;
}

/********************************************************************************
* 函数名: GB3761SetParamProcExt
* 输　入:
* 输　出:
* 返  回:
* 功  能: 参数设置
*********************************************************************************/
INT8S GB3761SetParamProcExt(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset)
{
    INT16U 	   i     = 0;                                //循环计数
    INT16U 	   j     = 0;                                //循环计数
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
            memset(&nData[0], 0, sizeof(nData));   //清空数据内容
            nDataLen += GB3761SetParamFnProcExt(pComm, pPn[i], pFn[j], pOffset, &nData[0], sizeof(nData));
            if (nDataLen > nMaxDataLen || 0 == nDataLen)  //一个Fn项数据长度超过帧长 或 无数据获取 丢弃
            {
                continue;
            }
            /*if ((pInfo->nTxDataLen + nDataLen) > nMaxDataLen)//组帧数据超过帧长 发送
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
                pInfo->nTxDataLen = 0;  //重新组帧
            }*/
            pInfo->nTxDataLen += MemCopy(&pComm->pTxFrmBuf[GB3761_DATA_IDX + pInfo->nTxDataLen], &nData[0], nDataLen);
        }   //End Of for(nFcode = 0; nFcode < GB3761_MAX_FN_NUM; nFcode++)
    }       //End Of for(pn = 0; pn < nMaxMpNum; pn++)
    return RET_ERR_OK;
}

/********************************************************************************
* 函数名:GB3761SetParamFnProcExt
* 输　入: pFrmInfoRx 接收帧信息
          nPn        信息类
          nFn        信息点
* 输　出: pOffset    本次Fn数据处理长度
* 返  回:RET_ERR_OK 执行成功  RET_ERR_ERROR 执行失败退出 RET_ERR_RANG 数据单元已偏移可以执行下一个FN
* 功  能:参数设置3761标准Fn处理过程
*********************************************************************************/
INT16U GB3761SetParamFnProcExt(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen)
{
    INT8U   nErrData    = 0;
	BOOLEAN bSetSuc = FALSE;
    INT8U   nResult  = GB3761_PART_OTHER;//没有指定错误，就为其他错误   INT16U  nDataLen = 0;
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
    switch (nFn) //处理不同Fn项
    {
        case F1:      
        {
            bSetSuc = GB3761SetParamExtF1(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F2:      
        {
            bSetSuc = GB3761SetParamExtF2(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F3:      
        {
            bSetSuc = GB3761SetParamExtF3(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F4:      
        {
            bSetSuc = GB3761SetParamExtF4(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F5:      
        {
            bSetSuc = GB3761SetParamExtF5(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F9:      
        {
            bSetSuc = GB3761SetParamExtF9(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F10:      
        {
            bSetSuc = GB3761SetParamExtF10(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F17:      
        {
            bSetSuc = GB3761SetParamExtF17(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F18:      
        {
            bSetSuc = GB3761SetParamExtF18(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F25:      
        {
            bSetSuc = GB3761SetParamExtF25(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F26:      
        {
            bSetSuc = GB3761SetParamExtF26(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F27:     
        {
            bSetSuc = GB3761SetParamExtF27(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F33:      
        {
            bSetSuc = GB3761SetParamExtF33(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F34:      
        {
            bSetSuc = GB3761SetParamExtF34(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        
        case F35:      
        
            bSetSuc = GB3761SetParamExtF35(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F36:      
        {
            bSetSuc = GB3761SetParamExtF36(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F37:      
        {
            bSetSuc = GB3761SetParamExtF37(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F38:      
        {
            bSetSuc = GB3761SetParamExtF38(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F39:      
        {
            bSetSuc = GB3761SetParamExtF39(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F81:      
        {
            bSetSuc = GB3761SetParamExtF81(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F84:      
        {
            bSetSuc = GB3761SetParamExtF84(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F86:      
        {
            bSetSuc = GB3761SetParamExtF86(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F87:      
        {
            bSetSuc = GB3761SetParamExtF87(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
            break;
        }
        case F89:      
        {
            bSetSuc = GB3761SetParamExtF89(nPn, &pComm->pRxFrmBuf[GB3761_DATA_IDX], pInfo->nRxDataLen, pOffset);
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
    	//Update3761ParamStat(nFn);                  //更新终端参数状态
        //EvtERC3GenPrmChanRecord(pRxMap->uA3.sDet.nMSA, nPn, (nFn + 1));
    }
    else
    {
    	pInfo->bFailFlag = TRUE;
    	pData[nDataLen++] = nResult;
    }
    return nDataLen;
}

//F1GPRS电源灯
BOOLEAN GB3761SetParamExtF1(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U       nFnLen      = 0;
	INT16U      nIndex      = 0;
    INT8U       nErrData    = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex    = *pOffset;
	nFnLen = 1;
	if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
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
    if (nErrData == 0xAA)//灭
    {
        GprsPowerOff();
    }
    else
    {
        GprsPowerOn();//亮
    }
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F2 GPRS通讯灯不闪烁
BOOLEAN GB3761SetParamExtF2(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U       nFnLen      = 0;
	INT16U      nIndex      = 0;
    INT8U       nErrData    = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex    = *pOffset;
	nFnLen = 1;
	if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
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
    if (nErrData == 0xAA)//不闪烁
    {
        GprsLightUnflash();
    }
    else
    {
        GprsLightFlash();//闪烁
    }
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F3 GPRS模块信号强度
BOOLEAN GB3761SetParamExtF3(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U       nFnLen      = 0;
	INT16U      nIndex      = 0;
    INT8U       nErrData    = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex    = *pOffset;
	nFnLen = 1;
	if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
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
    if (nErrData == 0xAA) //不能正常通訊
    {
        GprsCommAbnormal();
    }
    else
    {
        GprsCommNormal();   //通信正常
    }
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F4 终端通信异常
BOOLEAN GB3761SetParamExtF4(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U       nFnLen      = 0;
	INT16U      nIndex      = 0;
    INT8U       nErrData    = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex    = *pOffset;
	nFnLen = 1;
	if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
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
    if (nErrData == 1) //不能上綫
    {
        TermOffline();
    }
    else
    {
        TermOnline();   //正常上綫
    }
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F5 SIM卡异常
BOOLEAN GB3761SetParamExtF5(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U       nFnLen      = 0;
	INT16U      nIndex      = 0;
    INT8U       nErrData    = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex    = *pOffset;
	nFnLen = 1;
	if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
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
    if (nErrData == 0xAA) //异常
    {
        SimAbnormal();
    }
    else
    {
        SimNormal();   //正常
    }
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F9 载波模块不工作
BOOLEAN GB3761SetParamExtF9(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U       nFnLen      = 0;
	INT16U      nIndex      = 0;
    INT8U       nErrData    = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex    = *pOffset;
	nFnLen = 1;
	if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
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
    if (nErrData == 0xAA) //不运行
    {
        CarrModeNwork();
    }
    else
    {
        CarrModework();   //运行
    }
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F10 载波模块表计参数丢失
BOOLEAN GB3761SetParamExtF10(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U       nFnLen      = 0;
	INT16U      nIndex      = 0;
    INT8U       nErrData    = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex    = *pOffset;
	nFnLen = 1;
	if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
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
    if (nErrData == 0xAA) //不能载波抄读
    {
        CarrCopyAbnRd();
    }
    else
    {
        CarrCopyRd();   //正常抄读
    }
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F17 终端死机
BOOLEAN GB3761SetParamExtF17(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U       nFnLen      = 0;
	INT16U      nIndex      = 0;
    INT8U       nErrData    = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex    = *pOffset;
	nFnLen = 1;
	if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
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
    if (nErrData == 1) //黑屏
    {
        BlackScreen();
    }
    else if(nErrData == 2)//白屏
    {
        WhiteScreen();
    }
    else if(nErrData == 3)//按键不响应
    {
        Buttonrespond();
    }
    else if(nErrData == 4)//指示灯异常
    {
        LightAbnormal();
    }
    else if(nErrData == 5)//终端通讯不响应
    {
        TermCommNrespond();
    }
    else
    {
        Restore(); //恢复
    }
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F18 终端电源故障
BOOLEAN GB3761SetParamExtF18(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U       nFnLen      = 0;
	INT16U      nIndex      = 0;
    INT8U       nErrData    = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex    = *pOffset;
	nFnLen = 1;
	if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
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
    if (nErrData == 1) //专变终端无电
    {
        TremNoPower();
    }
    else if(nErrData == 2)//显示板无电
    {
        ScreenNoPower();
    }
    else if(nErrData == 3)//I/O接口板无电
    {
        InterfaceNoPower();
    }
    else if(nErrData == 4)//12v输出故障
    {
        TwelveVoltFault();
    }
    else if(nErrData == 5)//5v输出故障
    {
        FiveVoltFault();
    }
    else
    {
        Recover(); //恢复
    }
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F25形成冻结数据
BOOLEAN GB3761SetParamExtF25(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U       nFnLen      = 0;
	INT16U      nIndex      = 0;
    INT8U       nErrData    = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex    = *pOffset;
	nFnLen = 1;
	if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
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
    if (nErrData == 1) //形成日数据
    {
        //DayData();
    }
    else if(nErrData == 2)//形成月数据
    {
        //MonData();
    }
    else if(nErrData == 0xFF)//形成日和月数据
    {
        //MonAndDayData();
    }
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F26 形成表计冻结数据(形成不准确数据)
BOOLEAN GB3761SetParamExtF26(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U       nFnLen      = 0;
	INT16U      nIndex      = 0;
    INT8U       nErrData    = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex    = *pOffset;
	nFnLen = 1;
	if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
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
    if (nErrData == 1) //形成冻结无效数据
    {
        //FrezIvadData();
    }
    else if(nErrData == 2)//形成冻结数据为0
    {
        //FrezZeroData();
    }
    else if(nErrData == 3)//形成突变数据
    {
        //TripData();
    }
    else if (nErrData == 4) //形成比之前冻结数据小
    {
        //LittleFrezData();
    }
    else if(nErrData == 5)//形成费率数据异常
    {
        //FeeAbnormal();
    }
    else if(nErrData == 6)//形成缺费率数据
    {
        //LakeRateData();
    }
    else if(nErrData == 0xFF)//按照输入冻结数据值配置
    {
        //InputDataConfig();
    }
    else    //按照终端形成相应数据
    {
        //TerminalData();
    }
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F27检查冻结数据状态
BOOLEAN GB3761SetParamExtF27(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U       nFnLen      = 0;
	INT16U      nIndex      = 0;
    INT8U       nErrData    = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex    = *pOffset;
	nFnLen = 1;
	if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 != nPn)
    {
        *pOffset += nFnLen;
        return FALSE;
    }
    
    *pOffset += nFnLen;
    return FALSE;
        
    nErrData = pData[nIndex];
    if (1 == TestBufBit(&pData[1 + 6 * nErrData], 1, 0)) //D0位置1，日数据不成功
    {
        //DayDataFail();
    }
    if (1 == TestBufBit(&pData[1 + 6 * nErrData], 1, 1)) //D1位置1，月数据不成功
    {
        //MonDataFail();
    }
    
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F33电台故障
BOOLEAN GB3761SetParamExtF33(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U       nFnLen      = 0;
	INT16U      nIndex      = 0;
    INT8U       nErrData    = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex    = *pOffset;
	nFnLen = 1;
	if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
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
    if (nErrData == 0xAA) //终端有接收信号，无发射信号
    {
        RadioAbnormal();
    }
    else
    {
        RadioRunNormal(); //正常运行
    }
    
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F34主板与接口板交互故障
BOOLEAN GB3761SetParamExtF34(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U       nFnLen      = 0;
	INT16U      nIndex      = 0;
    INT8U       nErrData    = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex    = *pOffset;
	nFnLen = 1;
	if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
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
    if (nErrData == 1) //与显示板通信故障
    {
        ScreenFault();
    }
    else if(nErrData == 2) //与锚板通信故障
    {
        AnchorFault();
    }
    else
    {
        InterfaceNoFault(); //无故障
    }
    
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F35主板与接口板交互故障
BOOLEAN GB3761SetParamExtF35(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U       nFnLen      = 0;
	INT16U      nIndex      = 0;
    INT8U       nErrData    = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex    = *pOffset;
	nFnLen = 1;
	if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
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
    if (nErrData == 0)//恢复
    {
        DispRecCtrl(); 
    }
    else if (nErrData == 1)//白屏
    {
        DispWhilteCtrl();
    }
    else if (nErrData == 2)//黑屏
    {
        DispBlackCtrl();
    }
    else if (nErrData == 3)//花屏
    {
        
    }
    
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F36 脉冲口故障 
BOOLEAN GB3761SetParamExtF36(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U       nFnLen      = 0;
	INT16U      nIndex      = 0;
    INT8U       nErrData    = 0;
    INT8U       nSecData    = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex    = *pOffset;
	nFnLen = 2;
	if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
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
    nSecData = pData[nIndex + 1];
    if (nErrData == 1) //短路
    {
        PulseMouthFault();
    }
    if (nSecData == 0)
    {
        PulseState();     //状态
    }
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F37 485端口故障
BOOLEAN GB3761SetParamExtF37(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U       nFnLen      = 0;
	INT16U      nIndex      = 0;
    INT8U       nErrData    = 0;
    INT8U       nSecData    = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex    = *pOffset;
	nFnLen = 2;
	if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
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
    nSecData = pData[nIndex + 1];
    if (nErrData == 1) //短路
    {
        PortFault();
    }
    if (nSecData == 2)
    {
        PortOpenCircuit();     //开路
    }
    else
    {
        PortState();      //默认
    }
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F38 遥控端口故障
BOOLEAN GB3761SetParamExtF38(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U       nFnLen      = 0;
	INT16U      nIndex      = 0;
    INT8U       nErrData    = 0;
    INT8U       nSecData    = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex    = *pOffset;
	nFnLen = 2;
	if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
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
    nSecData = pData[nIndex + 1];
    if (nErrData == 1) //短路
    {
        RemoteFault();
    }
    if (nSecData == 2)
    {
        RemoteOpenCircuit();     //开路
    }
    else
    {
        RemoteState();    //默认
    }
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F39 遥信端口故障
BOOLEAN GB3761SetParamExtF39(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U       nFnLen      = 0;
	INT16U      nIndex      = 0;
    INT8U       nErrData    = 0;
    INT8U       nSecData    = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex    = *pOffset;
	nFnLen = 2;
	if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
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
    nSecData = pData[nIndex + 1];
    if (nErrData == 1) //短路
    {
        ShakeFault();
    }
    if (nSecData == 2)
    {
        ShakeOpenCircuit();     //开路
    }
    else
    {
        ShakeState();    //默认
    }
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F81 被抄口规约
BOOLEAN GB3761SetParamExtF81(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U       nFnLen      = 0;
	INT16U      nIndex      = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex    = *pOffset;
	nFnLen = 1;
	if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 != nPn)
    {
        *pOffset += nFnLen;
        return FALSE;
    }
    g_pMem->sXRamData.nRdMtProtFlg = pData[nIndex];
    if (g_pMem->sXRamData.nRdMtProtFlg == 1)
    {
        SetCommCfg(1, 3, 2);  //SetCommCfg(1, 4, 2);      //2400 偶校验
        OpenCommPort(COMM_WH485);
        
        SetCommCfg(1, 3, 2);    //SetCommCfg(1, 4, 2);    //2400 偶校验
        OpenCommPort(COMM_WH4852);
    }
    else
    {
        SetCommCfg(1, 6, 2);        //9600 偶校验
        OpenCommPort(COMM_WH485);
        
        SetCommCfg(1, 6, 2);        //9600 偶校验
        OpenCommPort(COMM_WH4852);
    }
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F84 生产编号
BOOLEAN GB3761SetParamExtF84(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U       nFnLen      = 0;
	INT16U      nIndex      = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex    = *pOffset;
	nFnLen = 8;
	if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 != nPn)
    {
        *pOffset += nFnLen;
        return FALSE;
    }
    MemWriteCommPrm(F95, &pData[nIndex], 8);
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F86 交采数据清零
BOOLEAN GB3761SetParamExtF86(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U       nFnLen      = 0;
	INT16U      nIndex      = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex    = *pOffset;
	nFnLen = 0;
	if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 != nPn)
    {
        *pOffset += nFnLen;
        return FALSE;
    }
    AcEnergyInit();
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}

//F87 所有初始化
BOOLEAN GB3761SetParamExtF87(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
	INT8U       nFnLen      = 0;
	INT16U      nIndex      = 0;

	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
	nIndex    = *pOffset;
	nFnLen = 0;
	if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    if (0 != nPn)
    {
        *pOffset += nFnLen;
        return FALSE;
    }
    MemInitAllExt();
    nIndex += nFnLen;
    *pOffset = nIndex;
    return TRUE;
}


//F89 形成日冻结数据
BOOLEAN GB3761SetParamExtF89(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset)
{
#if 1
    INT16U      nTmpLen     = 0;
    INT8U       nFnLen      = 0;
	INT16U      nIndex      = 0;
    INT8U       nDataType   = 0;
    INT8U       nMeterNum   = 0;
    INT8U       i           = 0;
    INT8U       nSeq        = 0;
    INT8U       nErr        = 0;
    INT8U       nAddr[6]    ={0};
    FORMAT11    sEnergy[5]  ={0};
    FORMAT20    sDayTime;
    STD_TIME    sTime;
    STD_TIME    sTmpTime;
    pMEM_MP_DAY_DATA    pDayData     = (pMEM_MP_DAY_DATA)(&g_nMemDataBuf[0]);
    
	if (NULL == pData || NULL == pOffset)
	{
		return FALSE;
	}
    memset(&sTime, 0, sizeof(sTime));
    memset(&sTmpTime, 0, sizeof(sTmpTime));
	nIndex    = *pOffset;
    nMeterNum = pData[nIndex + 4];
    nFnLen = 5 + nMeterNum * 26;
    nTmpLen = nIndex + nFnLen;
    if ((nIndex + nFnLen) > nDataLen)  //数据有效长度判断
    {
        *pOffset = nDataLen;
        return FALSE;
    }
    
    memcpy(&sDayTime, &pData[nIndex], 3);
    nIndex += 3;
    nDataType = pData[nIndex++];
    nMeterNum = pData[nIndex++];
    nFnLen = 5 + nMeterNum * 26;
    memcpy(&sTime.nDay, &sDayTime, sizeof(sDayTime));
    NewDayMemInfoProc(&sTime);
    GetNextDay(&sTmpTime, &sTime);
    for (i = 0; i < nMeterNum; i++)
    {
        memset(pDayData, 0, sizeof(MEM_MP_DAY_DATA));
        memcpy(&nAddr[0], &pData[nIndex], 6);
        nIndex += 6;
        memcpy(&sEnergy[0], &pData[nIndex], 20);
        nIndex += 20;
        if (IsAllEqualVal(&nAddr[0], 6, 0x98))
        {
            nSeq = 1;
        }
        else
        {
            nSeq = GetSeqFromAddr(&nAddr[0]);
            if (nSeq < 0)
            {
                continue;
            }
        }
        
        OSSemPend(g_pSem[SEM_STORE_BUF_IDX], 0, &nErr);
        memcpy(&pDayData->nMeterAddr[0],    &nAddr[0],   LEN_METER_ADDR);           //电表地址(标签)
        memcpy(&pDayData->sDataTime,        &sDayTime,   sizeof(sDayTime));	        //日时标标签
        memcpy(&pDayData->nReadTime[0],     &sTmpTime.nSec,       6);                    //读表时间
        memcpy(&pDayData->nDEnergyPP[0], &sEnergy[0], 20);
  
        MemWriteDayData(nSeq, FEND, 0, pDayData, sizeof(MEM_MP_DAY_DATA));
        OSSemPost(g_pSem[SEM_STORE_BUF_IDX]);
    } 
#endif 
    //nIndex += nFnLen;
    *pOffset = nTmpLen;
    return TRUE;
}