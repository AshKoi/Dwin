/***************************************************************
****************************************************************/
#include "Global.h"

#define MAX_ATR_SIZE            55

/*   ESAM 数据定义   */
INT8U* g_EsamRecvBuff = NULL;
INT32U g_EsamRecvCount = 0;
INT32U g_EsamRecvByteMax = 0;
INT8U g_DriverEsamID[MAX_ATR_SIZE+1];
INT8U g_DriverEsamIDLen = 0;
INT8U g_IDdata[MAX_ATR_SIZE];


/* SPI INTERFACE */		
#define SPIESAM_CS_EN()				bFM3_GPIO_PDOR5_P5 = 0
#define SPIESAM_CS_DIS()			bFM3_GPIO_PDOR5_P5 = 1

#define TESAM_WD_CLEAR()            WatchDogClear(1)


void SpiEsamInit( void )
{
    uint16_t tBGR;
    uint32_t tSysFrePCLK2;
    
    /* Spi Initialize */
    bFM3_GPIO_PFR5_P6 = 1;
    bFM3_GPIO_PFR5_P7 = 1;
    bFM3_GPIO_PFR5_P8 = 1;
    FM3_GPIO->EPFR07 |= 0x00005400;
    
	bFM3_MFS1_CSIO_SCR_UPCL = 1;
    FM3_MFS1_CSIO->SMR = 0x46;
    FM3_MFS1_CSIO->SCR |= 0x21;
    tSysFrePCLK2 = SystemCoreClock/(1<<((FM3_CRG->APBC2_PSR&0x03)));
    tBGR = (uint16_t)(((tSysFrePCLK2 + (ESAMSPIBAUDRATE / 2)) / (uint32_t)ESAMSPIBAUDRATE) - 1);
    FM3_MFS1_CSIO->BGR = tBGR;    
    FM3_MFS1_CSIO->SSR |= 0x80;
    bFM3_MFS1_CSIO_SCR_TXE = 0;
    bFM3_MFS1_CSIO_SCR_RXE = 0;
	bFM3_MFS1_CSIO_ESCR_WT0 = 0;
	bFM3_MFS1_CSIO_ESCR_WT1 = 0;
	
	bFM3_MFS1_CSIO_SCR_SPI = 0;
    bFM3_MFS1_CSIO_SMR_SCINV = 0;	//SPI传输模式三
    bFM3_MFS1_CSIO_SMR_BDS = 1;
	
    /* SPX_EN Initialize */
    bFM3_GPIO_PFR5_P5 = 0;
    bFM3_GPIO_DDR5_P5 = 1;
    bFM3_GPIO_PDOR5_P5 = 1;
}


INT8U SpiEsamSendByte( INT8U data, INT8U mode )
{
    INT32U delay = 0;
	INT32U i = 200;
	
    bFM3_MFS1_CSIO_SMR_SOE = 1;
    bFM3_MFS1_CSIO_SCR_TXE = 1;
    bFM3_MFS1_CSIO_SCR_RXE = 0;
    FM3_MFS1_CSIO->TDR = data;
	while(i--);
    while( !bFM3_MFS1_CSIO_SSR_TDRE )
    {
        delay++;
        if( delay == 1000 )
            return 0;
    }
    bFM3_MFS1_CSIO_SCR_TXE = 0;
    bFM3_MFS1_CSIO_SMR_SOE = 0;    
    return 1;

}

INT8U SpiEsamRecvByte( INT8U mode )
{
    INT32U delay = 0;
	
    bFM3_MFS1_CSIO_SMR_SOE = 0;
    bFM3_MFS1_CSIO_SCR_TXE = 1;
    bFM3_MFS1_CSIO_SCR_RXE = 1;
    FM3_MFS1_CSIO->TDR = 0;
    while( !bFM3_MFS1_CSIO_SSR_RDRF )
    {
        delay++;
        if( delay == 1000 )
            return 0;
    }
    bFM3_MFS1_CSIO_SCR_TXE = 0;
    bFM3_MFS1_CSIO_SCR_RXE = 0;

    return FM3_MFS1_CSIO->RDR;
}

void ESAMReset( void )
{
#if ESAMSPI
	SpiEsamInit();
#else
	ISO7816Init();
    CommFlush(COMM_ESAM);           //上电的时候会自动发送一次ESAM   ID操作，清空数据
	ISO7816Reset();
#endif
}
INT8U ESAMGetID( INT8U *buff )
{
#if ESAMSPI
	return 0;
#else
	memcpy( buff, g_DriverEsamID, g_DriverEsamIDLen );
	return (g_DriverEsamIDLen);
#endif
}

void SpiEsamDelay( INT32U time )
{
    volatile INT32U i;
    for( ; time>0; time-- )
    {
//        TESAM_WD_CLEAR();
        for( i=0; i<25; i++ );
    }
}
// 	LRC1的计算方法：对CLA INS P1 P2 Len1 Len2 DATA数据，每个字节的异或值，再取反。
//  对DATA数据，每个字节的异或值，再取反。
INT8U  CalcLRC(INT8U *command, INT16U len)
{
    INT8U data = 0;
    INT16U i = 0;
    if (len < 2)
    {
        return 0;
    }
    data = command[0];
    for(i = 1; i < len; i++)
    {
        data ^= command[i];

    }
    return ~data;
}

BOOLEAN ESAMExecCommand(INT8U *comm, INT16U len, INT8U *resp, INT16U *lenr)
{
#if ESAMSPI
    INT32U i, recvlen,reclen;
    INT8U mode = 3;//SPI_MODE_3;
    INT8U  data = 0;
    const INT8U ch = 0x55;
    INT8U recch = 0;
#define SPIESAM_TIMEOUT         10000
#define SPIESAM_MAX_RESP_LEN    2048
	
	OSSchedLock();		//关任务调度
	TESAM_WD_CLEAR();
    SPIESAM_CS_EN();
    Delayus(85);
    data = CalcLRC(comm,len);
    SpiEsamSendByte(ch,mode);
    Delayus(20);
    // 发送命令
    if (len < 7)
    {
         for( i=0; i<len; i++ )
         {
            SpiEsamSendByte( *comm++, mode );
			Delayus(20);
         }
    }
    else
    {
        for( i=0; i<6; i++ )
        {
            SpiEsamSendByte( *comm++, mode );
			Delayus(20);
        }
//		Delayus(10);
        for (i = 6;i < len;i++)
        {
            TESAM_WD_CLEAR();
            SpiEsamSendByte( *comm++, mode );
			Delayus(20);
        }

    }
    SpiEsamSendByte(data,mode);
    Delayus(20);
    SPIESAM_CS_DIS();

    Delayus(200);
    // 等待接收
    SPIESAM_CS_EN();
    Delayus(85);
    for( i=0; i<SPIESAM_TIMEOUT; i++ )          // 查询必须等待足够长时间 1s
    {
        recch = SpiEsamRecvByte( mode );
		Delayus(500);
		TESAM_WD_CLEAR();
        if( recch == ch )
            break;
    }

    if( i == SPIESAM_TIMEOUT )
    {
        SPIESAM_CS_DIS();
	    Delayus(20);
        resp[0] = 0x69;
        resp[1] = 0x97;
        *lenr = 2;
		OSSchedUnlock();			//恢复任务调度
        return FALSE;
    }

    for( i=0; i<4; i++ )
	{
        resp[i] = SpiEsamRecvByte( mode );
		Delayus(20);
	}
    recvlen = resp[2] * 256 + resp[3];
    if(((recvlen+5+1) > SPIESAM_MAX_RESP_LEN) )
    {
        SPIESAM_CS_DIS();
	    Delayus(20);
        resp[0] = 0x69;
        resp[1] = 0x98;
        *lenr = 2;
		OSSchedUnlock();			//恢复任务调度
        return FALSE;
    }
    reclen = recvlen+5;
    // sw(2)+len(2)+data(len)+lrc
    for( ; i < reclen ; i++ )
	{
        resp[i] = SpiEsamRecvByte( mode );
		Delayus(20);
		TESAM_WD_CLEAR();
	}		
    SPIESAM_CS_DIS();
    Delayus(20);
	OSSchedUnlock();			//恢复任务调度
    data = CalcLRC(&(resp[0]),recvlen +4);
    if (data == resp[i - 1])
    {
        //memcpy(resp,&(resp[1]),i-1);
        *lenr = i-1;
        return TRUE;
    }
    else
    {
        resp[0] = 0x69;
        resp[1] = 0x99;
        *lenr = 2;
        return FALSE;
    }
#else
    BOOLEAN ret;
    ret = ESAM_Exec_Command(comm, len, resp, lenr);
    return ret;
#endif
}

/********************************************************************************
* 
* 
* ESAM 部分代码
* 
* 
*********************************************************************************/
INT8U ISO7816Init( void )
{
	OpenComm(COMM_ESAM, UART_BPS_9600, EVEN_PARITY, ONE_STOP_BIT, EIGHT_CHAR_BIT);         //ESAM
    ESAMPwmOut(ENABLE);                 //4MHz
    ESAM_RST_PFR = 0;			//ESAM_RST
    ESAM_RST_DDR = 1;
    ESAM_RST_PDOR = 1;
    OSTimeDlyHMSM(0, 0, 0, 200);
    return 1;
}

BOOLEAN ISO7816Reset( void )
{
    INT8U data[MAX_ATR_SIZE],num;
	INT8U pos;

//    OSSchedLock();

    ESAM_RST_PDOR = 0;
    OSTimeDlyHMSM(0, 0, 0, 500);
	ESAM_RST_PDOR = 1;
    OSTimeDlyHMSM(0, 0, 0, 100);

/*	g_EsamRecvBuff = g_IDdata;
	g_EsamRecvByteMax = 2;
	EsamResetState( 0 );
	EsamTimerEnable( 20 );
	while( (g_bRecvNone == FALSE) && (g_bRecvError == FALSE) );*/

    g_EsamRecvCount = ReadCommMs(COMM_ESAM, data, 512, 200, 1000);


//	memcpy( data, g_IDdata, g_EsamRecvCount );
	if( data[0] != 0x3B )	goto _ErrExit;	

	pos = 0;
	num = data[1] & 0xF0;
	if( num & 0x80 )	pos++;
	if( num & 0x40 )	pos++;
	if( num & 0x30 )	pos++;
	if( num & 0x10 )	pos++;

//	g_EsamRecvBuff = g_IDdata;
	g_EsamRecvByteMax = (data[1] & 0x0F) + pos;
/*	EsamResetState( 0 );
	EsamTimerEnable( 20 );
	while( (g_bRecvNone == FALSE) && (g_bRecvError == FALSE) );
	memcpy( data+2, g_IDdata, g_EsamRecvCount );	*/
	
	memset( g_DriverEsamID, 0, MAX_ATR_SIZE );
	g_DriverEsamIDLen = data[1] & 0x0F;
	memcpy( g_DriverEsamID, data+pos+2, g_DriverEsamIDLen );
	
	if( g_EsamRecvCount != (pos + g_DriverEsamIDLen + 2) )
	{
		g_DriverEsamIDLen = 0;
		goto _ErrExit;
	}
//	OSSchedUnlock();
	return TRUE;
	
_ErrExit:
//	OSSchedUnlock();
	return FALSE;
}

/*********************************************************************************
 函数名： BOOLEAN ESAMExecCommand(INT8U *comm, INT16U len, INT8U *resp, INT16U *lenr)
 输入：   len为命令长度
          comm为命令数据指针
          lenr为返回数据长度指针
          resp为返回数据指针
 输出：   *lenr为返回数据长度，范围0--255
          *resp为返回数据
 返回：   TRUE为正确
          FALSE为不正确
 描述：   向ESAM发送命令并接收返回信息
*********************************************************************************/
BOOLEAN ESAM_Exec_Command(INT8U *comm, INT16U len, INT8U *resp, INT16U *lenr)
{
	INT8U INS, buflen, rcv, Lc, Le, i, j, Buf[5];
	INT8U *p;
    INT8U RevBuf[128];
    INT8U recvlen;

	if (len < 5)
		return FALSE;

	buflen = len;
	for (i = 0; i < 5; i++)
		Buf[i] = *(comm + i);
	*lenr = 0;
	
	p = resp;

command_begin:
	INS = Buf[1];
	if (buflen > 5)
	{
		Lc = Buf[4];
		if (len > 5 + Lc)
			Le = Buf[5 + Lc];
		else Le = 0;
	}
	else
	{
		Le = Buf[4];
		Lc = 0;
	}
	
	//
/*	for (i = 0; i < 5; i++)                                //  CLA INS P1 P2 P3
	{
		ISO7816ByteTx( Buf[i] );
	}*/
    WriteComm(COMM_ESAM, Buf, 5);
command_a:
/*	do
	{
		if (ISO7816ByteRx(&rcv, 1) == FALSE)
		{
			//printf( "ERR:2" );
			goto RET_ERR;
	}
	}
	while (rcv == 0x60);*/
    recvlen = ReadCommMs( COMM_ESAM, RevBuf, 300, 300, 1000) ;
    if( recvlen == 0)
        goto RET_ERR;
    
    for(j=0; j<recvlen; j++)
    {
        if( RevBuf[j] != 0x60 )
            break;
        if(j >= recvlen -1)
            goto RET_ERR;
    }
    rcv = RevBuf[j];
        
	if (rcv == INS)
	{
		//dgbg_putc('N');//printf( "INS\n\r" );
		if (Lc == 0)
		{
/*			for (i = 0; i < Le + 2; i++)
			{
				if (ISO7816ByteRx(&rcv, 1) == FALSE )
					goto RET_ERR;
				*p++ = rcv;
				(*lenr)++;
			}*/
			for (i = 0; i < Le + 2; i++)
            {
                *p++ = RevBuf[j+i+1];
                (*lenr)++;
            }

			
			if ((*(p-2) == 0x90) && (*(p-1) == 0x00))         // SW1, SW2
				goto RET_OK;
			else if( (*(p-2) == 0x61) ) 					// 61 xx -> recv again
			{
				Delayms(100);		//_ISO7816_Wait_CLK(250);						//
				Buf[0] = 0x0;
				Buf[1] = 0xc0;
				Buf[2] = 0x0;
				Buf[3] = 0x0;
				Buf[4] = *(p-1);
				buflen = 5;
				p -= 2;
				*lenr -= 2;
				goto command_begin;
			}
			else
				goto RET_ERR;
		}
		else
		{
			Delayms(5);	//_ISO7816_Wait_CLK(50);                            // 反向保护时间，比规范宽1ETU
/*			for (i = 0; i < Lc; i++)
			{
				if (ISO7816ByteTx(*(comm + 5 + i)) == FALSE)
					goto RET_ERR;
			}*/
            WriteComm(COMM_ESAM, (comm+5), Lc);
			Lc = 0;
			goto command_a;
		}
	}
	else
	{
		switch (rcv)
		{
		case 0x61:
		{
/*			if (ISO7816ByteRx(&rcv, 1) == FALSE)
				goto RET_ERR;*/
			if ( 1 /*bRESPAUTOFlag*/)                // 自动取响应
			{
				Delayms(100);		//_ISO7816_Wait_CLK(250);              // 反向保护时间，比规范宽1ETU
				Buf[0] = 0x0;
				Buf[1] = 0xc0;
				Buf[2] = 0x0;
				Buf[3] = 0x0;
                Buf[4] = RevBuf[j+1];
				buflen = 5;
				goto command_begin;
			}
			else
			{
				*p++ = 0x61;
				*p++ = RevBuf[j+1];
				*lenr += 2;
				goto RET_OK;
			}
		}
		case 0x6c:
		{
/*			if (ISO7816ByteRx(&rcv, 1) == FALSE)
				goto RET_ERR;*/
			Buf[4] = RevBuf[j+1];
			buflen = 5;
			Delayms(10);	//_ISO7816_Wait_CLK(50);
			goto command_begin;
		}
		default:
		{
			*p++ = RevBuf[j];
			(*lenr)++;
/*			if (ISO7816ByteRx(&rcv, 1) == FALSE)
				goto RET_ERR;*/
			*p++ = RevBuf[j+1];
			(*lenr)++;
			if ((*(p-2) == 0x90) && (*(p-1) == 0x00))         // SW1, SW2
				goto RET_OK;
			else goto RET_ERR;
		}
		//break;
		}
	}

RET_OK:
	//ISO7816_DEBUG_OUTPUT_HIGH( BSP_GPIO_PORT_B, DEF_BIT_30 );
	return TRUE;
RET_ERR:
	//ISO7816_DEBUG_OUTPUT_HIGH( BSP_GPIO_PORT_B, DEF_BIT_30 );
	return FALSE;
}

void TestESAMLongPacket( void )
{
	static INT8U resp[100];
	static INT16U resplen = 0;

	INT8U random[] = {0x00, 0x84, 0x00, 0x00, 0x08 };
	INT8U cmdBuf1[] = {
	  0x90, 0x40, 0x00, 0x00, 0x92, 0x62, 0x90, 0x50, 0x4E, 0x7A, 0xF4, 0x2E, 0x2B, 0x4D, 0x80, 0x41
	, 0x7E, 0xD0, 0x16, 0x72, 0xB0, 0xED, 0xFC, 0xEF, 0x56, 0xEC, 0x23, 0x43, 0xAD, 0x73, 0x12, 0x8A
	, 0xEC, 0x57, 0x62, 0x96, 0x29, 0x92, 0x8C, 0x7F, 0x42, 0xD9, 0x60, 0xE0, 0xCE, 0x5E, 0xF5, 0x1D
	, 0xF1, 0xEF, 0x62, 0xAC, 0x1D, 0xFA, 0x53, 0x3C, 0x64, 0x1F, 0x1D, 0x96, 0x8D, 0xDF, 0x41, 0x41
	, 0x51, 0x11, 0xB2, 0x89, 0x3E, 0x03, 0x8B, 0x28, 0x7F, 0xCD, 0x43, 0x3C, 0x2B, 0x87, 0x39, 0xA8
	, 0x61, 0x8C, 0xF5, 0xA2, 0x20, 0x48, 0x8B, 0x07, 0x20, 0xF4, 0xF6, 0xC6, 0x84, 0xC4, 0x6F, 0x2E
	, 0x1E, 0x9D, 0xF3, 0xC6, 0x0D, 0x33, 0xE0, 0xC7, 0x1E, 0x2D, 0x27, 0x4A, 0xD7, 0x95, 0x78, 0x8F
	, 0x75, 0xBF, 0x24, 0x26, 0xD2, 0x00, 0x08, 0x9B, 0x23, 0x9B, 0x0D, 0x01, 0xDB, 0x25, 0xA1, 0xD7
	, 0x84, 0xB7, 0xCF, 0xD1, 0x95, 0x24, 0xB1, 0xC2, 0x7B, 0x1A, 0x4B, 0xC2, 0xDD, 0x75, 0x66, 0x2E
	, 0x86, 0x83, 0xAB, 0xE0, 0xED, 0x3C, 0x15};
	INT8U cmdBuf2[] = {
	0x80, 0x40, 0x00, 0x00, 0x82, 0x60, 0x80, 0x57, 0x27, 0xE4, 0xAD, 0xFC, 0x37, 0xB4, 0x7E, 0x50
	, 0x79, 0x1C, 0xA2, 0xE2, 0x95, 0x9A, 0xF7, 0xE8, 0xB6, 0x3B, 0x06, 0xA3, 0x74, 0xC0, 0xCB, 0x11
	, 0xEB, 0x02, 0xD1, 0xD5, 0x89, 0x3C, 0x00, 0x45, 0xF1, 0xE2, 0x2D, 0x9B, 0x6C, 0x69, 0xA0, 0x62
	, 0x92, 0xBE, 0x37, 0x31, 0x3E, 0x97, 0x49, 0x84, 0xA8, 0x1A, 0x6E, 0x2B, 0xAE, 0x46, 0xAE, 0x91
	, 0x26, 0xF8, 0x1E, 0xF6, 0xFF, 0x18, 0xE2, 0xEE, 0x5A, 0x32, 0x1B, 0x7E, 0x0A, 0xD9, 0x83, 0xB6
	, 0xBC, 0x3A, 0xAE, 0x4E, 0xD0, 0x19, 0x2C, 0xA5, 0x29, 0x34, 0x6C, 0x75, 0x24, 0xFA, 0x14, 0x4E
	, 0x01, 0xBF, 0x63, 0xDD, 0x65, 0x49, 0x4D, 0xED, 0x77, 0xF2, 0xED, 0xD0, 0xAC, 0x7A, 0xDB, 0xAD
	, 0x0A, 0x4B, 0xCB, 0xAB, 0x9C, 0x4F, 0x16, 0x56, 0xCE, 0x66, 0x2D, 0x12, 0x4F, 0x13, 0xF6, 0x1B
	, 0xB3, 0x9C, 0x0C, 0x86, 0xF6, 0x00, 0x1D};
	
	memset( resp, 0, sizeof(resp) );
	ESAMExecCommand( random, sizeof(random), resp, &resplen );
	
	memset( resp, 0, sizeof(resp) );
	ESAMExecCommand( cmdBuf1, sizeof(cmdBuf1), resp, &resplen );
	
	memset( resp, 0, sizeof(resp) );
	ESAMExecCommand( cmdBuf2, sizeof(cmdBuf2), resp, &resplen );	
}

 