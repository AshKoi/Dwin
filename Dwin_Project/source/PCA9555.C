/***************************************************************
* Copyright(c) 2010 光一科技股份有限公司 研发中心 - 基础技术部
* 文件名: PCA9555.C
* 创建人: lightrain
* 日　期: 2011-10-31
* 修改人:
* 日　期:
* 描　述: 通过GPIO口线模拟IIC总线操作,适用与PCA9555、CAT9555。
* 版　本: Version 1.0
****************************************************************/


#include "global.h"
#include "pca9555.h"

#define IIC_BUS_READ			1
#define IIC_BUS_WRITE			0

volatile INT8U _io_delay = 0;
volatile INT8U  _io_ack_error = 0;
INT32U _io_i=0;
INT8U _io_getbit[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
/////////////////////////////////////////////////////////////////////

#if PCA9555_MODULE
// I2C device code
#define IIC_DEVICE_9555		0x48
#elif CAT9555_MODULE
// I2C device code
#define IIC_DEVICE_9555		0x4C
#endif



/* 启用 IIC 总线 */
void EMULATOR_9555_START( void )
{
	_io_delay++;
	EMULATOR_9555_SDA_UP();
	_io_delay++;
	EMULATOR_9555_SCK_UP();
	_io_delay++;
	EMULATOR_9555_SDA_DWN();
	_io_delay++;
	EMULATOR_9555_SCK_DWN();
}

/* 关闭 IIC 总线 */
 void EMULATOR_9555_STOP( void )
{
	_io_delay++;
	EMULATOR_9555_SCK_DWN();
	_io_delay++;
	EMULATOR_9555_SDA_DWN();
	_io_delay++;
	EMULATOR_9555_SCK_UP();
	_io_delay++;
	EMULATOR_9555_SDA_UP();
	_io_delay++;
}

/* 发送一位数据：高 */
void EMULATOR_9555_BIT_SEND_H( void )
{
	EMULATOR_9555_SDA_UP();
	_io_delay++;
	EMULATOR_9555_SCK_UP();
	_io_delay++;_io_delay++;_io_delay++;
    _io_delay++;_io_delay++;_io_delay++;
	EMULATOR_9555_SCK_DWN();
}

/* 发送一位数据：低 */
void EMULATOR_9555_BIT_SEND_L( void )
{
	EMULATOR_9555_SDA_DWN();
	_io_delay++;
	EMULATOR_9555_SCK_UP();
	_io_delay++;_io_delay++;_io_delay++;
    _io_delay++;_io_delay++;_io_delay++;
	EMULATOR_9555_SCK_DWN();
}

/* 读取一位数据 */
void EMULATOR_9555_BIT_RECV( void )
{
}
	
/* 接收ACK */
void EMULATOR_9555_RECV_ACK( void )
{
	EMULATOR_9555_SDA_UP();
    EMULATOR_9555_INPUT();
    _io_ack_error = 0;
	EMULATOR_9555_SCK_UP();
	_io_delay++;
	if( EMULATOR_9555_SDA_DATA() )
    {
        _io_ack_error =1;
    }
	EMULATOR_9555_SCK_DWN();
	_io_delay++;
    EMULATOR_9555_OUTPUT();
}


/* 发送ACK*/
void EMULATOR_9555_SEND_ACK( void )
{
	_io_delay++;
	EMULATOR_9555_SDA_DWN();
	_io_delay++;
	EMULATOR_9555_SCK_UP();
	_io_delay++;
	EMULATOR_9555_SCK_DWN();
}

/* 发送 NOACK*/
void EMULATOR_9555_SEND_NACK( void )
{
	_io_delay++;
	EMULATOR_9555_SDA_UP();
	_io_delay++;
	EMULATOR_9555_SCK_UP();
	_io_delay++;
	EMULATOR_9555_SCK_DWN();
}

/* 发送一个字节，从低位到高位 */
 void EMULATOR_9555_SEND_BYTE_L2H( INT8U chrsend )
{
    for( int i=0; i<8; i++ )
    {
        if( chrsend & _io_getbit[i] )
        {
            EMULATOR_9555_BIT_SEND_H();
        }
        else
        {
            EMULATOR_9555_BIT_SEND_L();
        }
    }
}

/* 发送一个字节，从高位到低位 */
void EMULATOR_9555_SEND_BYTE_H2L( INT8U chrsend )
{
    for( int i=0; i<8; i++ )
    {
        if( chrsend & _io_getbit[7-i] )
        {
            EMULATOR_9555_BIT_SEND_H();
        }
        else
        {
            EMULATOR_9555_BIT_SEND_L();
        }
    }
}

/* 读入一个字节，从低位到高位 */
INT8U EMULATOR_9555_RECV_BYTE( void )
{
    INT8U data = 0;

    EMULATOR_9555_SDA_UP();
    EMULATOR_9555_INPUT();
    for( int i=0; i<8; i++ )
    {
        EMULATOR_9555_SCK_UP();
        _io_delay++;
        if( EMULATOR_9555_SDA_DATA() )
        {
            data |= _io_getbit[ i ];
        }
        EMULATOR_9555_SCK_DWN();
    }
    EMULATOR_9555_OUTPUT();
    return data;
}


INT8U EMULATOR_9555_RECV_BYTE_H2L( void )
{
    INT8U data = 0;

    EMULATOR_9555_SDA_UP();
    EMULATOR_9555_INPUT();
    for( int i=0; i<8; i++ )
    {
        EMULATOR_9555_SCK_UP();
        _io_delay++;_io_delay++;_io_delay++;
        if( EMULATOR_9555_SDA_DATA() )
        {
            data |= _io_getbit[ 7-i ];
        }
        EMULATOR_9555_SCK_DWN();
        _io_delay++;_io_delay++;_io_delay++;
        _io_delay++;_io_delay++;_io_delay++;
    }
    EMULATOR_9555_OUTPUT();
    return data;
}

OS_EVENT* sem9555Used = NULL;
INT32U    g_9555Timeout = 0;

INT8U PCA9555_Write( INT8U addr, INT8U szWrite[], INT8U len )
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr = 0;
#endif
	INT8U j=0;
	
	OS_ENTER_CRITICAL();

	EMULATOR_9555_INIT();
	EMULATOR_9555_START();
	EMULATOR_9555_SEND_BYTE_H2L( (IIC_DEVICE_9555 | IIC_BUS_WRITE) );		
	EMULATOR_9555_RECV_ACK();
	if( _io_ack_error )
	{
		EMULATOR_9555_STOP();
		OS_EXIT_CRITICAL();
		return 0;
	}
		
	EMULATOR_9555_SEND_BYTE_H2L( addr );
	EMULATOR_9555_RECV_ACK();
	if( _io_ack_error )
	{
		EMULATOR_9555_STOP();
		OS_EXIT_CRITICAL();
		return 0;
	}
		
	for(j=0;j<len;j++)
	{			
		EMULATOR_9555_SEND_BYTE_H2L( szWrite[j] );	// data 0			
		EMULATOR_9555_RECV_ACK();
		if( _io_ack_error )
		{
			EMULATOR_9555_STOP();
			OS_EXIT_CRITICAL();
			return j;
		}
	}
	EMULATOR_9555_STOP();				
	OS_EXIT_CRITICAL();
	
	return (j);
}


INT8U PCA9555_Read( INT8U addr, INT8U szRead[], INT8U len )
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr = 0;
#endif
	INT8U j;
		
	OS_ENTER_CRITICAL();	

    EMULATOR_9555_INIT();
	EMULATOR_9555_START();
	EMULATOR_9555_SEND_BYTE_H2L( (IIC_DEVICE_9555 | IIC_BUS_WRITE) );		
	EMULATOR_9555_RECV_ACK();
	if( _io_ack_error )
	{
		EMULATOR_9555_STOP();
		OS_EXIT_CRITICAL();
		return 0;
	}
		
	EMULATOR_9555_SEND_BYTE_H2L( addr );		
	EMULATOR_9555_RECV_ACK();
	if( _io_ack_error )
	{
		EMULATOR_9555_STOP();
		OS_EXIT_CRITICAL();
		return 0;
	}
		
	EMULATOR_9555_START();
	EMULATOR_9555_SEND_BYTE_H2L( (IIC_DEVICE_9555 | IIC_BUS_READ) );		
	EMULATOR_9555_RECV_ACK();
	if( _io_ack_error )
	{
		EMULATOR_9555_STOP();
		OS_EXIT_CRITICAL();
		return 0;
	}
		
	for(j=0;j<len;j++)
	{
		_io_delay++;		
		szRead[j] = EMULATOR_9555_RECV_BYTE_H2L( );
		
		if(j!=(len-1) )
		{
			EMULATOR_9555_SEND_ACK();
		}
		else
		{
			EMULATOR_9555_SEND_NACK();
		}
	}

	EMULATOR_9555_STOP();
	OS_EXIT_CRITICAL();
	
	return j;
}

#ifdef CFG_GW_ZBI
#define PORT1_I2C_DEV_CODE      0x48    // A2A1A0 = 100
#else
#define PORT1_I2C_DEV_CODE      0x4C    // A2A1A0 = 110
                                        // PCA9555 - 0x4C
                                        // PCA9539 - 0xEC
#endif

/* */
BOOLEAN PCA9555Init( INT8U port, INT32U timeout )
{
	INT8U buff[2];
	INT8U ret = 0, err;
	
    if( port == 0 )
    {
        sem9555Used = OSSemCreate( 1 );
        if( sem9555Used == NULL )
            return FALSE;

        g_9555Timeout = timeout;

        // 0 - 输出
        // 1 - 输入
#if CFG_GW_JZQ
        // IO0 - Key, IO1 - Status/Remote Signal
        buff[0] = 0xFF;
        buff[1] = 0xFF;
#elif (CFG_GW_ZBIII | CFG_GW_ZBII)
        // IO0 - Remote Signal, IO1 - Status
        buff[0] = 0xFF;
        buff[1] = 0xFF;
#elif CFG_NW_JZQ
        // IO0 - Remote Signal, IO1 - key
        buff[0] = 0x3F;  //SHUGW 2013.05.06 buff[0] = 0xFF;
        buff[1] = 0xFF;
#else
        return FALSE;
#endif
        OSSemPend( sem9555Used, g_9555Timeout, &err );
        ret = PCA9555_Write( 6, buff, 2 );
        OSSemPost( sem9555Used );
    }
    else
    {
        // 0 - 输出
        // 1 - 输入
#if (CFG_GW_ZBIII | CFG_GW_ZBII)
        #if TEST_ON_JZQ
        // IO0 - Key, IO1 - Status/Remote Signal
        buff[0] = 0xFF;
        buff[1] = 0xFF;
        #else
        // IO0 - LED, IO1 - Key
        buff[0] = 0x00;
        buff[1] = 0xFF;

    #ifdef CFG_GW2012
        // 高2位用于功控，电控指示灯
        buff[1] = 0x3F;
    #endif
        #endif
#elif CFG_GW_ZBI
        // IO0 - Area Code, IO1 - Baudrate, key
        buff[0] = 0xFF;
        buff[1] = 0xFF;
#else
        return FALSE;
#endif
       // ret = TWIWrite( (PORT1_I2C_DEV_CODE>>1), 6, 1, buff, 2 );
    }

	if( ret != 2 )
		return FALSE;

	return TRUE;
}

INT8U g_nDriverTwiReadState = 0;
/* */
INT8U PCA9555Read( INT8U port, INT8U addr )
{
	INT8U ret, err;
	INT8U data = 0;

    if( port == 0 )
    {
#ifndef CFG_GW_ZBI
        OSSemPend( sem9555Used, g_9555Timeout, &err );
        ret = PCA9555_Read( addr, &data, 1 );
        OSSemPost( sem9555Used );
#endif
	}
    else
    {
        //ret = TWIRead( PORT1_I2C_DEV_CODE>>1, addr, 1, &data, 1 );
        g_nDriverTwiReadState = ret;
    }

    if( ret != 1 )
    {
        return 0xFF;
    }
	return data;
}

/**/
void PCA9555Write( INT8U port, INT8U addr, INT8U data )
{
	INT8U err;
	
    if( port == 0 )
    {
#ifndef CFG_GW_ZBI
        OSSemPend( sem9555Used, g_9555Timeout, &err );
        PCA9555_Write( addr, &data, 1 );
        OSSemPost( sem9555Used );
#endif
    }
    else
    {
        //TWIWrite( PORT1_I2C_DEV_CODE>>1, addr, 1, &data, 1 );
    }
}

BOOLEAN PCA9555Reinit( INT8U port )
{
	INT8U buff[2];
	INT8U ret = 0, err;
	
    if( port == 0 )
    {
        if( sem9555Used == NULL )
            return FALSE;

        // 0 - 输出
        // 1 - 输入
#if CFG_GW_JZQ
        // IO0 - Key, IO1 - Status/Remote Signal
        buff[0] = 0xFF;
        buff[1] = 0xFF;
#elif CFG_GW_ZBIII
        // IO0 - Remote Signal, IO1 - Status
        buff[0] = 0xFF;
        buff[1] = 0xFF;
#elif CFG_NW_JZQ
        // IO0 - Remote Signal, IO1 - key
        buff[0] = 0x3F;
        buff[1] = 0xFF;
#else
        return FALSE;
#endif
        OSSemPend( sem9555Used, g_9555Timeout, &err );
        ret = PCA9555_Write( 6, buff, 2 );
        OSSemPost( sem9555Used );
    }
    else
    {
        // 0 - 输出
        // 1 - 输入
#if CFG_GW_ZBIII
        // IO0 - LED, IO1 - Key
        buff[0] = 0x00;
        buff[1] = 0xFF;
#elif CFG_GW_ZBI
        // IO0 - Area Code, IO1 - Baudrate, key
        buff[0] = 0xFF;
        buff[1] = 0xFF;
    #ifdef CFG_GW2012
        // 高2位用于功控，电控指示灯
        buff[1] = 0x3F;
    #endif
#else
        return FALSE;
#endif
       //ret = TWIWrite( (PORT1_I2C_DEV_CODE>>1), 6, 1, buff, 2 );
    }

	if( ret != 2 )
		return FALSE;

	return TRUE;
}

void RefreshKey( void )
{
#if (CFG_GW_JZQ | CFG_NW_JZQ)
    PCA9555Reinit( 0 );
#elif CFG_GW_ZBIII
    PCA9555Reinit( 1 );
#endif
}

// 侦测I2C总线上是否具有指定的设备
BOOLEAN PCA9555_Probe( INT8U slaveaddr )
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr = 0;
#endif
	BOOLEAN bRet = TRUE;
	
	OS_ENTER_CRITICAL();

	EMULATOR_9555_INIT();
	EMULATOR_9555_START();
	EMULATOR_9555_SEND_BYTE_H2L( (slaveaddr | IIC_BUS_WRITE) );		
	EMULATOR_9555_RECV_ACK();
	if( _io_ack_error )
	{
        bRet = FALSE;
	}
	EMULATOR_9555_STOP();
	OS_EXIT_CRITICAL();

    return bRet;
}		
