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
#ifndef _GB3761_GET_PRM_EXT_H_
#define _GB3761_GET_PRM_EXT_H_
#include "Os_cpu.h"
#include "Config.h"

#include "Gw3761.h"



/**************************************************************************************************
                                          ��غ���˵��
***************************************************************************************************/

INT8S GB3761GetParamExt(pAPP_COMM pComm);
INT8S GB3761GetParamProcExt(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset);
INT16U GB3761GetParamFnProcExt(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen);


//F81�����ڹ�Լ
INT16U GB3761GetParamExtF81(INT16U nPn, INT8U *pData, INT16U nMaxDataLen);
//F82GPRS����״̬
INT16U GB3761GetParamExtF82(INT16U nPn, INT8U *pData, INT16U nMaxDataLen);
//F83��̫������״̬
INT16U GB3761GetParamExtF83(INT16U nPn, INT8U *pData, INT16U nMaxDataLen);
//F84���ɱ��
INT16U GB3761GetParamExtF84(INT16U nPn, INT8U *pData, INT16U nMaxDataLen);


#endif