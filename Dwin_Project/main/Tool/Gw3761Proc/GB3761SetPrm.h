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
#ifndef _GB3761_SET_PRM_H_
#define _GB3761_SET_PRM_H_

#include "Gw3761.h"

/**************************************************************************************************
                                          ��غ�������
***************************************************************************************************/
//��������
INT8S GB3761SetParamProc(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset);
//��������
INT16U GB3761SetParamFnProc(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen);

/*************************************�������þ���Fn��*************************************************/
//F1 �ն�ͨ�Ų�������
BOOLEAN GB3761SetParamF1(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F3 ��վIP��ַ�Ͷ˿�
BOOLEAN GB3761SetParamF3(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F4 ��վ�绰����Ͷ������ĺ���
BOOLEAN GB3761SetParamF4(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F7 �ն˳���������
BOOLEAN GB3761SetParamF7(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F8 �ն�����ͨ�Ź�����ʽ
BOOLEAN GB3761SetParamF8(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F10 //�ն˵��ܱ�/��������װ�����ò���
BOOLEAN GB3761SetParamF10(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset, INT8U *pReSult);
//F11 �������
BOOLEAN GB3761SetParamF11(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F12 �ն�״̬���������
BOOLEAN GB3761SetParamF12(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F16 ����ר�� �û���������
BOOLEAN GB3761SetParamF16(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F24
BOOLEAN GB3761SetParamF24(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F25
BOOLEAN GB3761SetParamF25(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F33 �ն˳������в�������
BOOLEAN GB3761SetParamF33(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F85
BOOLEAN GB3761SetParamF85(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
BOOLEAN GB3761SetParamF94(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F130
BOOLEAN GB3761SetParamF130(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F131
BOOLEAN GB3761SetParamF131(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F132
BOOLEAN GB3761SetParamF132(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F225
BOOLEAN GB3761SetParamF225(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F227
BOOLEAN GB3761SetParamF227(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
#endif
