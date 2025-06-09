/******************************************************************************************************
* Copyright (c) 2013
* 文件名:
* 创建人: 王一凡
* 日　期: 2013/09/12
* 修改人:
* 日　期:
* 描　述:
* 版　本: Version 1.0
*****************************************************************************************************/
#ifndef METER645_H
#define METER645_H

#include "Os_cpu.h"

extern INT8U    g_MeterInsideWrong[2];
extern INT8U    g_MeterComFaul;
extern INT8U    g_MeterEventWrong[4];
extern INT8U    g_nAcEventBuf[LEN_AC_EVENT];
extern INT8U    g_nSetCtrlcurFlg;
extern INT8U   g_nSetShortcurFlg; //火线短接
/**************************************************************************************************
                                           宏及枚举定义
***************************************************************************************************/
#define         PROT_GB645_97           1
#define         PROT_GB645_07           30


#define DEC2BCD(x)	              ((((x)/10)<<4)+(x)%10)
#define BCD2DEC(x)	              (((x)&0x0F)+((x)>>4)*10)
/**************************************************************************************************
                                           结构定义
***************************************************************************************************/

//主板上行规约帧信息
typedef struct _tag_DB_LINK_PROT
{
	INT8U					nHead1;                        //头     
    INT8U				    nAddr[6];					    //地址
    INT8U					nHead2;                        //头 
	INT8U					nCtrl;						    //控制码    
	INT8U					nDatalen[1];					//长度
	INT8U				    nData[1];						//数据内容
}DB_LINK_PROT, *pDB_LINK_PROT;

#define METER_645_BASE_LEN          (sizeof(DB_LINK_PROT) - 1)


/**************************************************************************************************
                                           调用接口
***************************************************************************************************/
INT32S	Meter645MakeFrm(INT8U* pData, INT32S nDataLen, INT32S nDataBufSize, INT8U* pMeterAddr, INT8U nCtrl);
INT32S 	Meter645CheckPackage(INT8U* pAddr, INT8U* pBuf, INT16U nBufsize);
INT32S 	Meter645Proc(INT8U nCom, INT8U* pData, INT32S nDatalen, INT32S nMaxDatalen);


INT32S 	Meter645AsynCheckPackage(void* pPara);

INT8U   DltJCMakeFrame(const INT8U* pAddr, INT8U* pData, INT16U nLength, INT8U* pSenddata);
INT32S 	Meter645AsynProc(void* pPara);

INT8U   DltJBAnalyse(const INT8U *pValidData, INT8U nValidLen, INT8U *pSendData);
INT8U   DltJBCode1c(const INT8U *pValidBuf, INT8U nValidLen, INT8U *pSendBuf);
INT8U   DltJBCode1d(const INT8U *pValidBuf, INT8U nValidLen, INT8U *pSendBuf);
INT8U   DltJBCode1e(const INT8U *pValidBuf, INT8U nValidLen, INT8U *pSendBuf);
INT8U   DltJBCode1f(const INT8U *pValidBuf, INT8U nValidLen, INT8U *pSendBuf);

INT8U   DltJBZYCal(INT32U nDY, INT32U nDL, INT32U nYGGL);
INT8U   DltJBXWCal(INT32U nYGGL);
INT8U   DltJBOffsetCal(INT32U nYGGL);

INT8S   DltJBParamSupported(const INT8U *pDI01, INT8U nLength);

INT8U   DltJBIDC98F(const INT8U *pdata);
INT8U   DltJBIDC99F(const INT8U *pdata);
INT8U   DltJBIDC9AF(const INT8U *pdata);
INT8U   DltJBIDC03F(const INT8U *pdata);

INT32U  DltJBDYDLcf(INT32U uMeasure, INT32U uStand);
INT32U  DltJBPQcf(INT32U uMeasure, INT32U uStand);
INT32U  DltJBXWcf(INT32U uMeasure, INT32U uStand);
INT32U  DltJBGLOffset(INT32U uMeasure, INT32U uStand);

INT8U   Dlt07Analyse(const INT8U *pValidData, INT32U nValidLen, INT8U *pSendData);
INT8U   Dlt07Code11(const INT8U *pValidBuf, INT8U nValidLen, INT8U *pSendBuf);
INT8U   Dlt07Code12(const INT8U *pValidBuf, INT8U nValidLen, INT8U *pSendBuf);
INT8U   Dlt07Code13(const INT8U *pValidBuf, INT8U nValidLen, INT8U *pSendBuf);
INT8U   Dlt07Code14(const INT8U* pValidBuf, INT32U nValidLen, INT8U* pSendBuf);
INT8U   DltCodeAddress(const INT8U* pValidBuf, INT8U nValidLen, INT8U* pSendBuf, INT8U nType);
INT8U DltCodePassword(const INT8U* pValidBuf, INT8U nValidLen, INT8U* pSendBuf, INT8U nType);

INT8U DltClrDemand(const INT8U* pValidBuf, INT8U nValidLen, INT8U* pSendBuf, INT8U nType);
INT8U DltClrMeter(const INT8U* pValidBuf, INT8U nValidLen, INT8U* pSendBuf, INT8U nType);
INT8U Dlt07Code1b(const INT8U *pValidBuf, INT8U nValidLen, INT8U *pSendBuf);
INT8U Dlt07Code1c(const INT8U* pValidBuf, INT8U nValidLen, INT8U* pSendBuf, INT8U nType);

INT8S Dlt07GetACEventLen(INT8U nTblName, INT8U nSubItem, INT16U *nDataLen, INT16U *nOffset);
INT32S  Dlt07SetDayData(INT8U* pData);


INT8U Dlt97Analyse(const INT8U *pValidData, INT8U nValidLen, INT8U *pSendData);
INT8U Dlt97Code01(const INT8U *pValidBuf, INT8U nValidLen, INT8U *pSendBuf);
INT8U Dlt97SendResp(const INT8U *pDI01, INT8U *pData, INT8U *pIsExist);
INT8U Dlt97Code04(const INT8U *pValidBuf, INT8U nValidLen, INT8U *pSendBuf);
INT8U Dlt97SaveParam(const INT8U *pData, INT8U nLen);
INT8S Dlt97ParamSupported(const INT8U *pDI01, INT8U nLength);
void AcCalcJustPluseConst(INT8U nPlusePersent);
void AcCalcEMUCfgConst(void);
INT8U PulLamChange();
INT32S CheckFauCode(INT8U*nDI01,INT8U* nData,INT8U nRetFlag);
void AcCalcJustPluse(void);
#endif
