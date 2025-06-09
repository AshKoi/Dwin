/***************************************************************
****************************************************************/
#include "GPRS.h"
#include <stdlib.h>
/*  0 - command
    1 - data
 */
INT8U gModemMode = MODEM_CMD_MODE ;

/* 在字符串string中查找子串 substr，返回子串在string中的位置，若没有找到，返回-1 */
int strFind( const char *substr, const char *string )
{
	int pos = 0;
	int lenSub, len;
	
	lenSub = strlen( substr );
	len    = strlen( string ) - lenSub ;
	
	while( pos <= len )
	{
		if( strncmp( substr, string+pos, lenSub ) == 0 )
			return pos;
		else
			pos++;
	}
	return -1;
}

char* TrimLeft( char* s )
{
	while( *s == ' ' )
		s++;
	return s;
}
void SendAtCmd( char* pData )
{
	INT32U len;
	len = strlen(pData);
	if( len > 0 )
	{
		//CommFlush( MODEM_PORT );
		WriteComm( COMM_GPRS, (INT8U*)pData, len );
		//dbgu_puts( "\n[Send]" );
		//dbgu_puts( (INT8U*)pData );
	}
}
BOOLEAN RecvAndFind( const char* pData, const char* pData2 )
{
static	INT8U  recvBuff[384];
	INT32S ret;
	INT32S p1, p2;
	
	memset( recvBuff, 0, 384 );
	ret = ReadCommMs(COMM_GPRS,recvBuff,400,500, 2000);
	if( ret > 0 )
	{
		p1 = strFind( pData, (char*)recvBuff );
		p2 = strFind( pData2,(char*)recvBuff );
//		OSTimeDlyHMSM(0, 0, 1, 0);
	   	WatchDogClear(1);
		if( (p1>=0) && (p2>=0) )
		{
			return TRUE;
		}
		else
			return FALSE;
	}
	return FALSE;
}

/*BOOLEAN RecvAndFindMs( const char* pData, const char* pData2, INT32U timeout )
{
	INT8U  recvBuff[50];
	INT32S ret;
	INT32S p1, p2;
	
	memset( recvBuff, 0, 50 );
	ret = ReadCommMs( MODEM_PORT, recvBuff, 50, 500, timeout );
	if( ret > 0 )
	{
		//dbgu_puts( (INT8U*)"\n[Recv]" );
		//dbgu_puts( recvBuff );
		
		p1 = strFind( pData, (char*)recvBuff );
		p2 = strFind( pData2,(char*)recvBuff );
		if( (p1>=0) && (p2>=0) )
		{
			return TRUE;
		}
		else
			return FALSE;
	}
	return FALSE;
}*/

void ModemPwrOn( void )
{
    GpioConfig(IO_PORT2, IO_PINx5, GPIO_MODE_OUTPUT);	//GPRS_CTRL
	GpioSetOutput(IO_PORT2, IO_PINx5);
	OSTimeDlyHMSM(0, 0, 0, 500);
	GpioClearOutput(IO_PORT2, IO_PINx5);
	
	//PCTRL与调试脚冲突
	
    GpioConfig(IO_PORTD, IO_PINx0, GPIO_MODE_OUTPUT);	//G_ON/OFF
	GpioClearOutput(IO_PORTD, IO_PINx0);
	OSTimeDlyHMSM(0, 0, 2, 0);
	GpioSetOutput(IO_PORTD, IO_PINx0);

	WatchDogClear(1);
	
}
BOOLEAN ModemInit( void )
{
	INT8U cnt,ret;
	
//    OpenComm( COMM_GPRS, 115200, UartDev_PARITY_NONE, UartDev_STOPBITS_1,UartDev_DATABITS_8 );
	ModemPwrOn();

	cnt = 3;
	while( cnt-- )
	{
		SendAtCmd( "AT\x0d" );
		if( RecvAndFind( "AT", "OK" ) == TRUE )
			break;
		OSTimeDlyHMSM(0, 0, 1, 0);
	}
	if(cnt == 0)
	{
		OpenComm( COMM_GPRS, 9600, UartDev_PARITY_NONE, UartDev_STOPBITS_1,UartDev_DATABITS_8 );
		cnt = 3;
		while( cnt-- )
		{
			SendAtCmd( "AT\x0d" );
			if( RecvAndFind( "AT", "OK" ) == TRUE )
				break;
			OSTimeDlyHMSM(0, 0, 1, 0);
		}
		SendAtCmd( "AT+IPR=115200\x0d" );
		if( RecvAndFind( "", "OK" ) == FALSE )
			return FALSE;
		OpenComm( COMM_GPRS, 115200, UartDev_PARITY_NONE, UartDev_STOPBITS_1,UartDev_DATABITS_8 );
		
	}
	
	OSTimeDlyHMSM(0, 0, 0, 500);
	WatchDogClear(1);
	SendAtCmd( "ATE0\x0d" );
	if( RecvAndFind( "", "OK" ) == FALSE )
		return FALSE;
//	SendAtCmd( "ATI\x0d" );
//	if( RecvAndFind( "", "OK" ) == FALSE )
//		return FALSE;
     
	WatchDogClear(1);
	ModemReqOffline();
	OSTimeDlyHMSM(0, 0, 1, 0);
	ModemReqOnline();
	WatchDogClear(1);
	ret = GPRSIsOnline();
	
	return ret;
}

INT8U ModemGetSignal( void )
{
	INT8U  recvBuff[64];
	INT32S ret;
	INT32S p1, p2, p3;
	char *s, *p;
	INT8U signal = 0;
	
	if( gModemMode == MODEM_DATA_MODE )
	{
		SendAtCmd("+++");
		if( RecvAndFind( "", "OK" ) == FALSE  )
			return 0;
		gModemMode = MODEM_CMD_MODE;
	}
	s = (char*)malloc( 20 );
	memset( recvBuff, 0, 50 );

	SendAtCmd( "AT+CSQ\x0d" );
	ret = ReadCommMs(COMM_GPRS,recvBuff,200,300, 500);
	SendAtCmd( "AT+CSQ\x0d" );
	OSTimeDlyHMSM(0, 0, 0, 200);
	ret = ReadCommMs(COMM_GPRS,recvBuff,200,300, 500);
	if( ret > 0 )
	{
		p1 = strFind( "+CSQ:", (char*)recvBuff );
		p2 = strFind( "OK", (char*)recvBuff );
		p3 = strFind( ",",  (char*)recvBuff );
		if( (p1>=0) && (p2>=0) && (p3>=0) )
		{
			strncpy( s, (char*)(recvBuff+p1+5), p3-p1-5 );
			p = TrimLeft( s );
			signal = atoi( p );
			if( (signal < 10) || (signal == 99) )
				bFM3_GPIO_PDOR3_P9 = 0;
			else if( (signal>=10) && (signal<20) )
			{
				bFM3_GPIO_PDOR3_P8 = 0;
				bFM3_GPIO_PDOR3_P9 = 0;
			}
			else 
				bFM3_GPIO_PDOR3_P8 = 0;
				
		}
		else
			bFM3_GPIO_PDOR3_P9 = 0;
		
	}
	free( s );
	return signal;
}

BOOLEAN ModemReqOnline( void )
{
	INT8U err;
	//INT8U cnt;
	
	if( gModemMode == MODEM_DATA_MODE )
	{
		SendAtCmd("+++");
		if( RecvAndFind( "", "OK" ) == FALSE  )
			return FALSE;
		gModemMode = MODEM_CMD_MODE;
	}
#ifdef NEOWAY
    SendAtCmd( "AT+CREG?\x0d" );
	OSTimeDlyHMSM(0, 0, 0, 100);
	WatchDogClear(1);
	while( RecvAndFind( "0", "1" ) == FALSE )
	{
	    SendAtCmd( "AT+CREG?\x0d" );	
		OSTimeDlyHMSM(0, 0, 1, 0);
	   	WatchDogClear(1);
	}

    SendAtCmd( "AT$MYTYPE?\x0d" );
	if( RecvAndFind( "", "" ) == FALSE )
	{	err = '1';	goto _Error;	}
	
    SendAtCmd( "AT$MYNETCON=0,\"conType\",\"GPRS\"\x0d" );
	if( RecvAndFind( "", "OK" ) == FALSE )
	{	err = '2';	goto _Error;	}
	
    SendAtCmd( "AT$MYNETCON=0,\"apn\",\"cmnet\"\x0d" );
	if( RecvAndFind( "", "OK" ) == FALSE )
	{	err = '2';	goto _Error;	}
    SendAtCmd( "AT$MYNETCON=0,userpwd,\"GSM:1234\"\x0d" );
	if( RecvAndFind( "", "OK" ) == FALSE )
	{	err = '2';	goto _Error;	}
    SendAtCmd( "AT$MYNETCON=0,\"auth\",1\x0d" );
	if( RecvAndFind( "", "OK" ) == FALSE )
	{	err = '2';	goto _Error;	}
		
	OSTimeDlyHMSM(0, 0, 1, 0);
	WatchDogClear(1);
//	SendAtCmd( "AT$MYNETCON=2,\"USERPWD\",\"None,None\"\x0d" );
//	if( RecvAndFind( "", "OK" ) == FALSE )
//	{	err = '3';	goto _Error;	}

	OSTimeDlyHMSM(0, 0, 1, 0);
	WatchDogClear(1);
    SendAtCmd( "AT$MYNETACT=0,1\x0d" );
	if( RecvAndFind( "", "OK" ) == FALSE )
	{	err = '4';	goto _Error;	}

	OSTimeDlyHMSM(0, 0, 3, 0);
	WatchDogClear(1);
	SendAtCmd( "AT$MYNETACT?\x0d" );
	RecvAndFind( "", "OK" );
	OSTimeDlyHMSM(0, 0, 1, 0);
	WatchDogClear(1);
    SendAtCmd( "AT$MYNETSRV=2,1,0,0,\"112.2.33.30:5000\"\x0d" );
	if( RecvAndFind( "", "OK" ) == FALSE )
	{	err = '5';		}
	
    SendAtCmd( "AT$MYNETOPEN=1\x0d" );	
	if( RecvAndFind( "", "OK" ) == FALSE )
	{	err = '6';		}
	
    SendAtCmd( "AT$MYNETOPEN?\x0d" );	
	if( RecvAndFind( "", "OK" ) == FALSE )
	{	err = '6';		}

	
/*	OSTimeDlyHMSM(0, 0, 1, 0);
	WatchDogClear(1);
    SendAtCmd( "AT+XISP=0\x0d" );
	if( RecvAndFind( "", "OK" ) == FALSE )
	{	err = '2';	goto _Error;	}
	
	OSTimeDlyHMSM(0, 0, 1, 0);
	WatchDogClear(1);
	SendAtCmd( "AT+CGDCONT=1,IP,CMNET\x0d" );
	if( RecvAndFind( "", "OK" ) == FALSE )
	{	err = '3';	goto _Error;	}

	OSTimeDlyHMSM(0, 0, 1, 0);
	WatchDogClear(1);
    SendAtCmd( "AT+XIIC=1\x0d" );
	if( RecvAndFind( "", "OK" ) == FALSE )
	{	err = '4';	goto _Error;	}

	OSTimeDlyHMSM(0, 0, 1, 0);
	WatchDogClear(1);
    SendAtCmd( "AT+XIIC?\x0d" );
	if( RecvAndFind( "", "OK" ) == FALSE )
	{	err = '5';	goto _Error;	}

	OSTimeDlyHMSM(0, 0, 1, 0);
	WatchDogClear(1);
    SendAtCmd( "AT+TCPSETUP=1,112.2.33.30,5000\x0d" );
	if( RecvAndFind( "", "OK" ) == FALSE )
	{	err = '5';	goto _Error;	}
	*/
#else
	SendAtCmd( "AT#SKIPESC=1\x0d" );
	if( RecvAndFind( "", "OK" ) == FALSE )
	{	err = '1';	goto _Error;	}
	
	SendAtCmd( "AT+CGDCONT=1,\"IP\",\"CMNET\"\x0d" );
	if( RecvAndFind( "", "OK" ) == FALSE )
	{	err = '2';	goto _Error;	}

	cnt = 3;
	while( 1 )
	{
		SendAtCmd( "AT+CGATT=1\x0d" );
		if( RecvAndFind( "", "OK" ) )
			break;
		OSTimeDlyHMSM(0, 0, 1, 0);
	}
	if(cnt==0) 
	 {	err = '4';	goto _Error;	} 
//	OSTimeDly( 3*OS_TICKS_PER_SEC );
//	SendAtCmd( "AT+CGATT=1\x0d" );
//	if( RecvAndFind( "", "OK" ) == FALSE )
//	{	err = '3';	goto _Error;	}
	
	while( 1 )
	{
		SendAtCmd( "AT#SGACT=1,1,"",""\x0d" );
		if( RecvAndFind( "", "OK" ) )
			break;
		OSTimeDlyHMSM(0, 0, 1, 0);
	}
	
	SendAtCmd( "AT#SCFG=1,1,1500,0,600,50\x0d" );
	if( RecvAndFind( "", "OK" ) == FALSE )
	{	err = '6';	goto _Error;	}
	
	SendAtCmd( "AT#SD=1,0,5000,\"112.2.33.30\"\x0d" );
	if( RecvAndFind( "CONNECT", "" ) == FALSE )
	{	err = '7';	goto _Error;	}
	
#endif	
	
_Error:
	err = err;
	//dbgu_putc( (INT8U)'<' );
	//dbgu_putc( err );
	//dbgu_putc( (INT8U)'>' );
	return FALSE;
}

BOOLEAN ModemReqOffline( void )
{
	if( gModemMode == MODEM_DATA_MODE )
	{
		SendAtCmd("+++");
		if( RecvAndFind( "", "OK" ) == FALSE  )
			return FALSE;
		gModemMode = MODEM_CMD_MODE;
	}
	
#ifdef NEOWAY
	OSTimeDlyHMSM(0, 0, 0, 200);
	WatchDogClear(1);
    SendAtCmd("AT+TCPCLOSE=1\x0d");
	if( RecvAndFind( "", "OK" ) == FALSE )
		goto _Error;
#else
	SendAtCmd("AT#SH=1\x0d");
	if( RecvAndFind( "", "OK" ) == FALSE )
		goto _Error;
#endif
	
_Error:
	return FALSE;
}

INT8U ModemSendData( char* data )
{
	
#ifdef NEOWAY
    char sTemp[324];
	INT32U len;
	static INT32U count;
	
//	signal = ModemGetSignal();
	OSTimeDlyHMSM(0, 0, 0, 200);
	len = strlen(data);
    sprintf( sTemp, "AT+TCPSEND=1,%d\x0d",(len+1) );
	WatchDogClear(1);
    SendAtCmd( sTemp );
	OSTimeDlyHMSM(0, 0, 0, 400);
	WatchDogClear(1);
	if( RecvAndFind( ">", "" ) == FALSE )
        return FALSE;
	memset(sTemp,0,384);
    sprintf( sTemp, "%4d%s\x0d",count,data);
	count++;
	OSTimeDlyHMSM(0, 0, 0, 400);
	WatchDogClear(1);
    SendAtCmd( sTemp );
	if( RecvAndFind( "Error", "" ) == TRUE )
        return FALSE;
#else
//	if( gModemMode == MODEM_CMD_MODE )
//	{
//		SendAtCmd( "AT#SD=1,0,5000,\"112.2.33.30\"\x0d" );
//	
//		if( RecvAndFind( "CONNECT", "" ) == FALSE )
//			return FALSE;
//		gModemMode = MODEM_DATA_MODE;
//	}	
//	memset(sTemp,0,324);
//    sprintf( sTemp, "%4d%s",count,data);
//	count++;
	SendAtCmd( data );
	
#endif
	return TRUE;
	
}


void ModemGetIP( void )
{
}

/* GPRS 模块初始化 */
void GPRSInit( INT8U type )
{
  
}

/* 复位 GPRS 模块 */
void GPRSReset( void )
{}

/* 检测 GPRS 模块是否在线 */
BOOLEAN GPRSIsOnline( void )
{
	INT8U i;
	for(i=0;i<3;i++)
	{
		SendAtCmd( "AT+IPSTATUS=1\x0d" );
//		OSTimeDlyHMSM(0, 0, 0, 200);
		if( RecvAndFind( "IPSTATUS", "DISCONNECT" ) == FALSE )
		{
			bFM3_GPIO_PDOR3_PC = 0;
			return TRUE;		
		}		
		OSTimeDlyHMSM(0, 0, 1, 0);
	   	WatchDogClear(1);
	}

	bFM3_GPIO_PDOR3_PC = 1;
    return FALSE; 
}

/* 使能 GPRS 数据或命令模式 */
void GPRSDataEnable( BOOLEAN bEnable )
{}

/* GPRS 模块中断输入允许 */
void GPRSIntEnable( void )
{}

/* 获取 GPRS 模块的短信状态 */
BOOLEAN GPRSGetSMSStatus( void )
{
    return FALSE; 
}

void ModemTest(void)
{	
	INT8U ret;
	
	while(1)
	{
		OSTimeDlyHMSM(0, 0, 10, 0);
		ret = ModemSendData("NEOWAY-M590 is online! Signal is ");		
		if(ret==FALSE)
		{
			ModemReqOffline();
			ModemInit();
			OSTimeDlyHMSM(0, 0, 1, 0);
			ModemReqOnline();
			OSTimeDlyHMSM(0, 0, 1, 0);
		}
			
	}
}

void TestONU(void)
{
    SetComm(COMM_GPRS, UART_BPS_9600, EVEN_PARITY, ONE_STOP_BIT, EIGHT_CHAR_BIT);
	SendAtCmd( "AT+UARTSET=1?\x0d" );
	OSTimeDlyHMSM(0, 0, 1, 0);
	if( RecvAndFind( "1", "8" )==TRUE)
	    SetComm(COMM_GPRS, UART_BPS_115200, NO_PARITY, ONE_STOP_BIT, EIGHT_CHAR_BIT);
	SendAtCmd( "AT+UARTSET=1,115200,8,none,1" );
	
	while(1)
	{
		OSTimeDlyHMSM(0, 0, 1, 0);
	}
}
