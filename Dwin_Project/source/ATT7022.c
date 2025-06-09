/***************************************************************
* Copyright(c) 2010 ��һ�Ƽ��ɷ����޹�˾ �з����� - ����������
* �ļ���: ATT7022.c
* ������: lightrain
* �ա���: 2010-08-04
* �޸���:
* �ա���:
* �衡��: ATT7022�ӿ�ʵ�֣���Ƶ��SPI����Ƶ��ԼΪ250K.
* �桡��: Version 1.0
****************************************************************/
#include "global.h"
#include "ATT7022.h"

#define ATT7022_FRAM_ID		0xA4

#define ATT7022_FRAM_MAX_DATA_LEN	512
#define ATT7022_FRAM_MAX_PAGE_LEN	256

#define ATT7022_FRAM_BYTE_PAGE_LEN	16

#define SPI_CHL_0			0
#define SPI_CHL_1			1

// SPI�ӿں�������ֵ
#define SPI_SUCC			0
#define SPI_ERR_READ		-1
#define SPI_ERR_WRITE		-2
#define SPI_ERR_UNKOWN		-100

volatile INT32U  att7022_spi_delay = 0;

#define  ATT7022_SPI_DELAY()   att7022_spi_delay++;  \
                             att7022_spi_delay++
void _7022_SPIInit( void )
{
	ATT7022_SPI_INIT();
	ATT7022_SPI_CS_INIT();
	ATT7022_SPI_CS_DIS();
	ATT7022_SPI_CLK_LOW();
}

INT8U _7022_SPISend( INT8U ch, INT8U data, INT8U mode )
{
	volatile INT32U i;
	if( mode == SPI_MODE_2 )
	{
		ATT7022_SPI_CLK_LOW();
		ATT7022_SPI_DELAY(); ATT7022_SPI_DELAY();
		for( i=0; i<8; i++ )
		{
#if 1
			ATT7022_SPI_CLK_HIGH();
			if( data & 0x80 )
				ATT7022_SPI_MOSI_HIGH();
			else
				ATT7022_SPI_MOSI_LOW();
#else
            /* modi for spi-rw-error */
			if( data & 0x80 )
				ATT7022_SPI_MOSI_HIGH();
			else
				ATT7022_SPI_MOSI_LOW();
			ATT7022_SPI_CLK_HIGH();
            ATT7022_SPI_DELAY();
#endif
			ATT7022_SPI_DELAY();
			ATT7022_SPI_CLK_LOW();
			ATT7022_SPI_DELAY();
			data <<= 1;
		}
	}
	else if( mode == SPI_MODE_0 )
	{
		ATT7022_SPI_CLK_LOW();
		ATT7022_SPI_DELAY();
		for( i=0; i<8; i++ )
		{
			if( data & 0x80 )
				ATT7022_SPI_MOSI_HIGH();
			else
				ATT7022_SPI_MOSI_LOW();
			ATT7022_SPI_DELAY();
			ATT7022_SPI_CLK_HIGH();
			ATT7022_SPI_DELAY();
			data <<= 1;
			ATT7022_SPI_CLK_LOW();
		}
	}
	for(i=0;i<100;i++);
	return 0;
}

INT8U _7022_SPIRead( INT8U ch, INT8U reg, INT8U mode )
{
	volatile INT32U i;
	INT8U ret = 0;
	if( mode == SPI_MODE_2 )
	{
		for( i=0; i<8; i++ )
		{
			ret <<= 1;
			ATT7022_SPI_CLK_HIGH();
			ATT7022_SPI_DELAY();
            ATT7022_SPI_DELAY(); /* add for spi-rw-error */
			if( ATT7022_SPI_MISO_DATA() )
			{
				ret |= 0x01;
			}
			ATT7022_SPI_CLK_LOW();
			ATT7022_SPI_DELAY();
		}
	}
	else if( mode == SPI_MODE_0 )
	{
		ATT7022_SPI_CLK_LOW();
		ATT7022_SPI_DELAY(); ATT7022_SPI_DELAY();
		for( i=0; i<8; i++ )
		{
			ret <<= 1;
			ATT7022_SPI_CLK_LOW();
			ATT7022_SPI_DELAY();
			if( ATT7022_SPI_MISO_DATA() )
			{
				ret |= 0x01;
			}
			ATT7022_SPI_CLK_HIGH();
			ATT7022_SPI_DELAY();
		}
		ATT7022_SPI_CLK_LOW();
	}
	for(i=0;i<100;i++);
	return ret;
}

////////////////////////////////////////////////////////////////////////////////
OS_EVENT* g_ATT7022Locer = NULL;
#define ATT7022_LOCKER_TIMEOUT		100

void ATT7022Init( void )
{
	_7022_SPIInit();
}

void ATT7022Reset( INT32U time )
{

	ATT7022_RST_PFR=0x00;ATT7022_RST_DDR=0x01;
	
	// J_RST
	ATT7022_RST_PDOR=0x00;
	OSTimeDlyHMSM(0, 0, 0, time);
	ATT7022_RST_PDOR=0x01;
}

INT32U ATT7022RegRead( INT8U cmd )
{
	INT32U data = 0;
        
#if 0	
    ATT7022_SPI_CS_DIS();
    ATT7022_SPI_CLK_LOW();
	ATT7022_SPI_CS_EN();
	_7022_SPISend( SPI_CHL_1, cmd, SPI_MODE_2 );
	data |= _7022_SPIRead( SPI_CHL_1, 0, SPI_MODE_2 );
	data <<= 8;
	data |= _7022_SPIRead( SPI_CHL_1, 0, SPI_MODE_2 );
	data <<= 8;
	data |= _7022_SPIRead( SPI_CHL_1, 0, SPI_MODE_2 );
	ATT7022_SPI_CS_DIS();
   
#else
        
   volatile INT32U i; 	

    ATT7022_SPI_CS_DIS();
    ATT7022_SPI_CLK_LOW();
	ATT7022_SPI_CS_EN();
    ATT7022_SPI_DELAY();
    for( i=0; i<8; i++ )
    {	
        if( cmd & 0x80 )
            ATT7022_SPI_MOSI_HIGH();
        else
            ATT7022_SPI_MOSI_LOW();

        ATT7022_SPI_CLK_HIGH();
        ATT7022_SPI_DELAY();
        ATT7022_SPI_CLK_LOW();
        ATT7022_SPI_DELAY();
        cmd <<= 1;
    }
                
    for(i=0;i<100;i++);
                
    for( i=0; i<24; i++ )
    {
        data <<= 1;
        ATT7022_SPI_CLK_HIGH();
        ATT7022_SPI_DELAY();
        if( ATT7022_SPI_MISO_DATA() )
        {
            data |= 0x01;
        }
        ATT7022_SPI_CLK_LOW();
        ATT7022_SPI_DELAY();
    }

    ATT7022_SPI_CS_DIS();
#endif        
	return data;
}

void   ATT7022RegWrite( INT8U cmd, INT32U data )
{
	ATT7022_SPI_CS_EN();
	_7022_SPISend( SPI_CHL_1, cmd, SPI_MODE_2 );
	_7022_SPISend( SPI_CHL_1, (INT8U)(data>>16), SPI_MODE_2 );
	_7022_SPISend( SPI_CHL_1, (INT8U)(data>>8), SPI_MODE_2 );
	_7022_SPISend( SPI_CHL_1, (INT8U)(data>>0), SPI_MODE_2 );
	ATT7022_SPI_CS_DIS();
}

INT32U ATT7022DataRead( INT32U addr, INT8U addrSize, INT8U* buff, INT32U len )
{
	INT32U k, pos, offset, count;
	INT8U cmd;
	
	pos = 0;
	while( pos < len )
	{
		// ����256�ֽ��ڵ�ƫ���������õ�������Ҫд���ֽڵĳ���
		offset = (addr+pos) % ATT7022_FRAM_MAX_PAGE_LEN ;
		if( (len - pos) >= (ATT7022_FRAM_MAX_PAGE_LEN - offset) )
			count = ATT7022_FRAM_MAX_PAGE_LEN - offset ;
		else
			count = ( len - pos ) % ATT7022_FRAM_MAX_PAGE_LEN;
		
		// ����д��ƫ����ѡ����ʵ�����
		cmd = ATT7022_FRAM_ID;
		if( (addr+pos) >= ATT7022_FRAM_MAX_PAGE_LEN )
			cmd += 0x02;
		
		k = TWIRead( cmd>>1, addr+pos, addrSize, buff+pos, count );
		if( k != count )	goto _ReadStop;
		
		pos += count;
		OSTimeDlyHMSM( 0,0,0,10 );
	}
	return pos;
	
_ReadStop:
	return (pos+k);
}

INT32U ATT7022DataWrite(INT32U addr, INT8U addrSize, INT8U* buff, INT32U len )
{
	INT32U k, pos, offset, count;
	INT8U cmd;
	
	pos = 0;
	while( pos < len )
	{
		// ����16�ֽ��ڵ�ƫ���������õ�������Ҫд���ֽڵĳ���
		offset = (addr+pos) % ATT7022_FRAM_BYTE_PAGE_LEN ;
		if( (len - pos) >= (ATT7022_FRAM_BYTE_PAGE_LEN - offset) )
			count = ATT7022_FRAM_BYTE_PAGE_LEN - offset ;
		else
			count = ( len - pos ) % ATT7022_FRAM_BYTE_PAGE_LEN;
		
		// ����д��ƫ����ѡ����ʵ�����
		cmd = ATT7022_FRAM_ID;
		if( (addr+pos) >= ATT7022_FRAM_MAX_PAGE_LEN )
			cmd += 0x02;
		
		k = TWIWrite( cmd>>1, addr+pos, addrSize, buff+pos, count );
		if( k != count )	goto _WriteStop;
		
		pos += count;
		OSTimeDlyHMSM( 0,0,0,10 );
	}
	return pos;
	
_WriteStop:
	return (pos+k);
}

