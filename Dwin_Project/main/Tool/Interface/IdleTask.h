/******************************************************************************************************
* Copyright (c) 2013
* �ļ���:
* ������: ��һ��
* �ա���: 2013/09/12
* �޸���:
* �ա���:
* �衡��:
* �桡��: Version 1.0
*****************************************************************************************************/
#ifndef _IDLETASK_H
#define _IDLETASK_H

#include "Os_cpu.h"

extern INT8U g_nSetDayDataFlg;
extern INT8U g_nDltSetDayBuf[1024]; //600
extern INT8S g_nUartBpsflg ;
void IdleTask(void *pData);



#endif