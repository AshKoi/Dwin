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
#ifndef _GB3761_GET_PRM_H_
#define _GB3761_GET_PRM_H_

#include "Gw3761.h"

/**************************************************************************************************
                                          ��غ�������
***************************************************************************************************/
//������ѯ
INT8S GB3761GetParamProc(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset);
//������ѯ
INT16U GB3761GetParamFnProc(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen);

/*************************************������ѯ����Fn��*************************************************/

//F7 �ն˳���������
INT16U GB3761GetParamF7(INT16U nPn, INT8U *pData, INT16U nMaxDataLen);
//F10 �ն˵��ܱ�/�������ò���
INT16U GB3761GetParamF10(pAPP_COMM pComm, INT16U nPn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen);
INT16U GB3761GetParamF11(pAPP_COMM pComm, INT16U nPn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen);
INT16U GB3761GetParamF25(INT16U nPn, INT8U *pData, INT16U nMaxDataLen);
INT16U GB3761GetParamF33(pAPP_COMM pComm, INT16U nPn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen);
#endif
