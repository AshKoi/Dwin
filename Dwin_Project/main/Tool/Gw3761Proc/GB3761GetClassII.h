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
#ifndef _GB3761_GET_CLASSII_H_
#define _GB3761_GET_CLASSII_H_

#include "Gw3761.h"
#include "Table.h"

/**************************************************************************************************
                                          ��غ�������
***************************************************************************************************/
INT8S GB3761GetClassII(pAPP_COMM pComm);
INT8S GB3761GetClassIIProc(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset);
INT16U GB3761GetClassIIFnProc(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen);
INT16U GB3761GetClassIIF161(INT16U nPn, const pFORMAT20 pTddTime, INT8U *pData, INT16U nMaxDataLen);
INT16U GB3761GetClassIIF177(INT16U nPn, const pFORMAT21 pTdmTime, INT8U *pData, INT16U nMaxDataLen);



#endif