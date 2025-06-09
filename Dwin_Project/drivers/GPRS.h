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

/* GPRS ģ���ʼ�� */
void GPRSInit( INT8U type );

/* ��λ GPRS ģ�� */
void GPRSReset( void );

/* ��� GPRS ģ���Ƿ����� */
BOOLEAN GPRSIsOnline( void );

/* ʹ�� GPRS ���ݻ�����ģʽ */
void GPRSDataEnable( BOOLEAN bEnable );

/* GPRS ģ���ж��������� */
void GPRSIntEnable( void );

/* ��ȡ GPRS ģ��Ķ���״̬ */
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
