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
#ifndef _FILETRANS_H_
#define _FILETRANS_H_

#include "Os_cpu.h"
#include "Gw3761.h"

/**************************************************************************************************
                                             �궨��
***************************************************************************************************/
#ifndef FLASH_BLOCK
#define FLASH_BLOCK							4096											//������С
#endif

//flash��ַ����
#define FLASH_BASE_DOWNLOAD					0												//flash��������ַ


#define MEM_FLH_UPPROGRAM_ADDR				(FLASH_BASE_DOWNLOAD + FLASH_BLOCK)				//����֡�洢��ַ����һ֡ǰ16���ֽ��Ǳ�־��
#define MEM_FLH_DEBUG_UPDATA_SIZE			(((320 * 1024 - 1) / FLASH_BLOCK + 1) * FLASH_BLOCK)//��������С�������ն˳���Ĵ�С����

#define	GB3761_LEN_FILETRANS_HEAD       	16												//�����ļ������ͷ


/**************************************************************************************************
                                             ��������
***************************************************************************************************/
INT8S GB3761FileTrans(pAPP_COMM pComm);
INT8S GB3761FileTransProc(pAPP_COMM pComm, const INT16U *pPn, INT16U nPnNum, const INT16U *pFn, INT8U nFnNum, INT16U *pOffset);
INT16U GB3761FileTransFnProc(pAPP_COMM pComm, INT16U nPn, INT16U nFn, INT16U *pOffset, INT8U *pData, INT16U nMaxDataLen);
INT16U GB3761FileTransGWF1(INT16U nPn, INT8U *pRcv, INT16U nRcvlen, INT16U *pOffset, INT8U *pSnd, INT16U nSndlen);
#endif
