/******************************************************************************************************
* Copyright (c) 2013
* 文件名:
* 创建人: 王一凡
* 日　期: 2013/09/12
* 修改人:
* 日　期:
* 描　述:
* 版　本: Version 1.0
*****************************************************************************************************/
#ifndef AUTO_TEST_H
#define AUTO_TEST_H

#include "Os_cpu.h"

extern INT8U           g_nErrBuf[2];
extern INT8U           g_nTestStep;

enum _tag_TEST_ITEM
{
	FLASH_TEST = 0,
    FRAM_TEST,
    CLOCK_TEST,
    IR_TEST,
    GPRS_TEST,
    ERR_TEST,
    CTRL_TEST,
    RS4851_TEST,
    RS4852_TEST,
    KEY_UP_TEST,
    KEY_DOWN_TEST,
    KEY_BG_TEST,
    KEY_JG_TEST,
    END_FLG,
	TEST_ITEM_NUM
};

INT32S  AutoTestProc(void);
INT32S  AutoTestProc1(void);
INT32S KEY_test(INT8U nKey);

INT32S WriteCommTest( INT8U port, INT8U *buffer, INT32U bufsize );

#endif