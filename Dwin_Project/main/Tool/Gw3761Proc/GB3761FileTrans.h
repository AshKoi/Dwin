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
#ifndef _GB3761_FILE_TRANS_H_
#define _GB3761_FILE_TRANS_H_

#include "GB3761.h"

/**************************************************************************************************
                                          ��غ�������
***************************************************************************************************/
//�ļ�����
INT8S GB3761FileTransProc(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset);
//�ļ�����
INT16U GB3761FileTransFnProc(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen);

/*************************************�ļ����ؾ���Fn��*************************************************/
//����Զ������F1�������
INT16U GB3761FileTransGWF1(INT16U nPn, INT8U *pSrcData, INT16U nSrcDataLen, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen);
#endif