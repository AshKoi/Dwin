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
#ifndef _GB3761_GET_CLASSI_H_
#define _GB3761_GET_CLASSI_H_

#include "Gw3761.h"

/**************************************************************************************************
                                          ��غ�������
***************************************************************************************************/
INT8S GB3761GetClassI(pAPP_COMM pComm);
INT8S GB3761GetClassIProc(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset);
INT16U GB3761GetClassIFnProc(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen);


//F2 �ն�����ʱ��
INT16U GB3761GetClassIF2(INT16U nPn, INT8U *pData, INT16U nMaxDataLen);
//F25 ��ǰ���༰����/�޹����ʡ��������أ������ѹ��������������������ڹ���
INT16U GB3761GetClassIF25(INT16U nPn, INT8U *pData, INT16U nMaxDataLen);
//F129  ��ǰ�����й�����ʾֵ
INT16U GB3761GetClassIF129(INT16U nPn, INT8U *pData, INT16U nMaxDataLen);




#endif
