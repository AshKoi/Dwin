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
#ifndef _GB3761_CTRL_CMD_H_
#define _GB3761_CTRL_CMD_H_

#include "Gw3761.h"

/**************************************************************************************************
                                          ��غ�������
***************************************************************************************************/
//��������
INT8S GB3761CtrlCmdProc(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset);
//��������
INT16U GB3761CtrlCmdFnProc(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen);

/*************************************�����������Fn��*************************************************/
//F1  ң����բ
BOOLEAN GB3761CtrlCmdF1(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F2  �����բ
BOOLEAN GB3761CtrlCmdF2(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F25  �ն��Զ��������
BOOLEAN GB3761CtrlCmdF25(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F31 ��ʱ����
BOOLEAN GB3761CtrlCmdF31(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F33  �ն��Զ�������
BOOLEAN GB3761CtrlCmdF33(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F38  �����ն�������վ
BOOLEAN GB3761CtrlCmdF38(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F39  �����ն˶Ͽ�����
BOOLEAN GB3761CtrlCmdF39(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F49 ����ָ��ͨ�Ŷ˿���ͣ����
BOOLEAN GB3761CtrlCmdF49(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F50 ����ָ��ͨ�Ŷ˿ڻָ�����
BOOLEAN GB3761CtrlCmdF50(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F51 ����ָ��ͨ�Ŷ˿����³���
BOOLEAN GB3761CtrlCmdF51(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
#endif