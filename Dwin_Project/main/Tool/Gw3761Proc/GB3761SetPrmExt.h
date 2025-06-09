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
#ifndef _GB3761_SET_PRM_EXT_H_
#define _GB3761_SET_PRM_EXT_H_
#include "Os_cpu.h"
#include "Config.h"

#include "Gw3761.h"

extern INT8U   g_nSignalErrFlg;
extern INT8U   g_nCarrErrFlg;
extern INT8U   g_nRs485ErrFlg;
extern INT8U   g_nRemotCtrlFlg;
extern INT8U   g_nGprsLedFlg;
/**************************************************************************************************
                                          ��غ���˵��
***************************************************************************************************/
INT8S GB3761SetParamExt(pAPP_COMM pComm);
INT8S GB3761SetParamProcExt(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset);
INT16U GB3761SetParamFnProcExt(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen);


//F1GPRS��Դ��
BOOLEAN GB3761SetParamExtF1(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F2 GPRSͨѶ�Ʋ���˸
BOOLEAN GB3761SetParamExtF2(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F3 GPRSģ���ź�ǿ��
BOOLEAN GB3761SetParamExtF3(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F4 �ն�ͨ���쳣
BOOLEAN GB3761SetParamExtF4(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F5 SIM���쳣
BOOLEAN GB3761SetParamExtF5(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F9 �ز�ģ�鲻����
BOOLEAN GB3761SetParamExtF9(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F10 �ز�ģ���Ʋ�����ʧ
BOOLEAN GB3761SetParamExtF10(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F17 �ն�����
BOOLEAN GB3761SetParamExtF17(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F18 �ն˵�Դ����
BOOLEAN GB3761SetParamExtF18(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F25�γɶ�������
BOOLEAN GB3761SetParamExtF25(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F26 �γɱ�ƶ�������(�γɲ�׼ȷ����)
BOOLEAN GB3761SetParamExtF26(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F27��鶳������״̬
BOOLEAN GB3761SetParamExtF27(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F33��̨����
BOOLEAN GB3761SetParamExtF33(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F34������ӿڰ彻������
BOOLEAN GB3761SetParamExtF34(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F35������ӿڰ彻������
BOOLEAN GB3761SetParamExtF35(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F36 ����ڹ��� 
BOOLEAN GB3761SetParamExtF36(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F37 485�˿ڹ���
BOOLEAN GB3761SetParamExtF37(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F38 ң�ض˿ڹ���
BOOLEAN GB3761SetParamExtF38(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F39 ң�Ŷ˿ڹ���
BOOLEAN GB3761SetParamExtF39(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F81 �����ڹ�Լ
BOOLEAN GB3761SetParamExtF81(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F84 �������
BOOLEAN GB3761SetParamExtF84(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F86 ������������
BOOLEAN GB3761SetParamExtF86(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
//F87 ���г�ʼ��
BOOLEAN GB3761SetParamExtF87(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);
BOOLEAN GB3761SetParamExtF89(INT16U nPn, INT8U *pData, INT16U nDataLen, INT16U *pOffset);












#endif