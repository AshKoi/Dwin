#ifndef _GB698_GET_REQUEST_H_
#define _GB698_GET_REQUEST_H_

#include "Gw698.h"

/**************************************************************************************************
                                          相关函数声明
***************************************************************************************************/
//参数查询
INT8S GB698GetRequest(pAPP_COMM pComm);

INT8S GB698GetRecordResp(INT8U *pOAD, INT8U *pData, INT16U *pDatalen);
INT32S  GB698RecordDataProc(INT8U nOAD_Arr[][5], INT8U	nOadLen, INT32U  nOAD, pSTD_TIME pDjTime, INT8U  *pData);
INT8S GB698GetRequestMakeFrm(INT8U *OAD, INT8U *pData, INT32U  nMasterOAD, pSTD_TIME pDjTime);
INT8S GB698GetEventMakeFrm(INT8U nFn, INT8U *pData,INT8U nFn2);
INT8S GB698GetFXEventMakeFrm(INT8U nFn, INT8U *pData,INT8U nNum);
INT8S GB698GetRecordEventMakeFrm(INT8U nFn, INT8U *pData);

#endif