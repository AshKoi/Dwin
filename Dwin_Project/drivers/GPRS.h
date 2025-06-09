/***************************************************************
****************************************************************/
#ifndef _GPRS_HEADER_
#define _GPRS_HEADER_

#include "config.h"
#include "global.h"

/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/

#define GPRSCOMM 1
#define NEOWAY  1
#define MODEM_DATA_MODE		1
#define MODEM_CMD_MODE		0

/* GPRS 模块初始化 */
void GPRSInit( INT8U type );

/* 复位 GPRS 模块 */
void GPRSReset( void );

/* 检测 GPRS 模块是否在线 */
BOOLEAN GPRSIsOnline( void );

/* 使能 GPRS 数据或命令模式 */
void GPRSDataEnable( BOOLEAN bEnable );

/* GPRS 模块中断输入允许 */
void GPRSIntEnable( void );

/* 获取 GPRS 模块的短信状态 */
BOOLEAN GPRSGetSMSStatus( void );
BOOLEAN ModemInit( void );
INT8U ModemGetSignal( void );
BOOLEAN ModemReqOnline( void );
BOOLEAN ModemReqOffline( void );
void ModemTest(void);
INT8U ModemSendData( char* data );
BOOLEAN RecvAndFind( const char* pData, const char* pData2 );

void TestONU(void);

#endif
