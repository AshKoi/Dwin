/*
 ******************************************************************************
 **
 ** \note Other information.
 **
 ******************************************************************************
 */

#include "global.h"

extern INT8U g_nHardNo;
extern INT8U g_nTriphase;
extern INT8U g_nSinglephase;

/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/
INT8U FramCmpBuf[128];

/*******************************************************************************
  * *函数名称	FRAMInit
  * *函数说明   初始化
  * *输入参数   无
  * *输出参数   无
  * *返回参数   无
*******************************************************************************/
static void FM24Init( void )
{
	/* 初始化SDA/SCL/WP */
    FM24_SDA_PFR = 0;
    FM24_SDA_DDR = 1;
    FM24_SDA_PDOR = 1;
	
    FM24_SCL_PFR = 0;
    FM24_SCL_DDR = 1;
    FM24_SCL_PDOR = 1;	
	
    FM24_WP_PFR = 0;
    FM24_WP_DDR = 1;
    FM24_WP_PDOR = 1;		
}

/*******************************************************************************
  * *函数名称	  Delay_us
  * *函数说明   插入一段延时时间  在为分频即主频80Mhz的情况下 为1us延时,
  * *输入参数   nCount: 指定延时时间长度  为10时 延时时间为10us
  * *输出参数   无
  * *返回参数   无
*******************************************************************************/
static void Delay_us(INT32U nCount)
{
    int i,j;
    for (i=0; i<nCount; i++)
        for (j=0; j<11; j++);           //80MHz下1us为11          
}

/*******************************************************************************
  * *函数名称	I2C_Start
  * *函数说明   启动I2C
  * *输入参数   无
  * *输出参数   无
  * *返回参数   无
*******************************************************************************/
static void I2C_Start(void)
{	
	FM24_SDA_DDR = 1;				//sda输出
	FM24_SDA_PDOR = 1;				//sda=1
	Delay_us(1);
	FM24_SCL_PDOR = 1;				//scl=1
	Delay_us(FM24CL16BAUDRATE);
	FM24_SDA_PDOR = 0;				//sda=0
	Delay_us(FM24CL16BAUDRATE);
}

/*******************************************************************************
  * *函数名称	I2C_Stop
  * *函数说明   停止I2C
  * *输入参数   无
  * *输出参数   无
  * *返回参数   无
*******************************************************************************/
static void I2C_Stop(void)
{
    FM24_SDA_DDR = 1;				//sda输出	
	FM24_SDA_PDOR = 0;				//sda=0
	Delay_us(1);
	FM24_SCL_PDOR = 1;				//scl=1
	Delay_us(FM24CL16BAUDRATE);
	FM24_SDA_PDOR = 1;				//sda=1
	Delay_us(FM24CL16BAUDRATE);	
}
		
/*******************************************************************************
  * *函数名称	Ack
  * *函数说明   应答位
  * *输入参数   无
  * *输出参数   无
  * *返回参数   无
*******************************************************************************/
static void Ack(void)
{	
    INT16U i = 0;
    FM24_SDA_DDR = 0;				//sda输入
	FM24_SCL_PDOR = 1;				//scl=1
	Delay_us(FM24CL16BAUDRATE);
	while(( FM24_SDA_PDIR ==1 )&&(i<500)) i++;
	FM24_SCL_PDOR = 0;				//scl=0
	Delay_us(FM24CL16BAUDRATE);
}	

/*******************************************************************************
  * *函数名称	MAck
  * *函数说明   应答位
  * *输入参数   无
  * *输出参数   无
  * *返回参数   无
*******************************************************************************/
static void MAck(void)
{
    FM24_SDA_DDR = 1;				//sda输出
	FM24_SDA_PDOR = 0;				//sda=0
	Delay_us(1);
	FM24_SCL_PDOR = 1;				//scl=1
	Delay_us(FM24CL16BAUDRATE);
	FM24_SCL_PDOR = 0;				//scl=0
	Delay_us(FM24CL16BAUDRATE);
}	

/*******************************************************************************
  * *函数名称	No_Ack
  * *函数说明   不应答位
  * *输入参数   无
  * *输出参数   无
  * *返回参数   无
*******************************************************************************/
static void No_Ack(void)
{
    INT32U i = 0;
    FM24_SDA_DDR = 0;				//sda输入
	FM24_SCL_PDOR = 1;				//scl=1
	Delay_us(FM24CL16BAUDRATE);
	while(( FM24_SDA_PDIR ==0 )&&(i<500)) i++;
	FM24_SCL_PDOR = 0;				//scl=0
	Delay_us(FM24CL16BAUDRATE);
}	

///*******************************************************************************
//  * *函数名称	MNo_Ack
//  * *函数说明   不应答位
//  * *输入参数   无
//  * *输出参数   无
//  * *返回参数   无
//*******************************************************************************/
//static void MNo_Ack(void)
//{	
//    FM24_SDA_DDR = 1;				//sda输出
//	FM24_SDA_PDOR = 0;				//sda=1
//	Delay_us(1);
//	FM24_SCL_PDOR = 1;				//scl=1
//	Delay_us(50);
//	FM24_SCL_PDOR = 0;				//scl=0
//	Delay_us(50);
//}

/*******************************************************************************
  * *函数名称	I2C_WriteByte
  * *函数说明   I2C写一个字节
  * *输入参数   无
  * *输出参数   无
  * *返回参数   无
*******************************************************************************/
static void I2C_WriteByte( INT8U data )
{
	INT8U i;
	
    FM24_SDA_DDR = 1;				//sda输出	
	FM24_SCL_PDOR = 0;				//scl=0
	Delay_us(FM24CL16BAUDRATE);
	for(i=0;i<8;i++)
	{
		if(data&0x80)
			FM24_SDA_PDOR = 1;				//sda=1
		else
			FM24_SDA_PDOR = 0;				//sda=0
		data=data<<1;
		FM24_SCL_PDOR = 1;				//scl=1
		Delay_us(FM24CL16BAUDRATE);
		FM24_SCL_PDOR = 0;				//scl=0
		Delay_us(FM24CL16BAUDRATE);
	}
		
	FM24_SDA_PDOR = 1;				//sda=1
	Delay_us(FM24CL16BAUDRATE);		
} 	

/*******************************************************************************
  * *函数名称	I2C_ReadByte
  * *函数说明   I2C读一个字节
  * *输入参数   无
  * *输出参数   无
  * *返回参数   temp 
*******************************************************************************/
static INT8U I2C_ReadByte( void )
{
	INT8U i;
	INT8U temp=0;
	
	FM24_SDA_PDOR = 1;				//sda=1
	Delay_us(FM24CL16BAUDRATE);
	
    FM24_SDA_DDR = 0;				//sda输入	
	FM24_SCL_PDOR = 0;				//scl=0
	Delay_us(FM24CL16BAUDRATE);
	for(i=0;i<8;i++)
	{
		FM24_SCL_PDOR = 1;				//scl=1
		if(FM24_SDA_PDIR == 1)
				temp |=0x01;
		else
				temp &=~0x01;
		
		if(7-i)temp=temp<<1;    //第8次的时候就不能再左移了
				
		Delay_us(FM24CL16BAUDRATE);
		FM24_SCL_PDOR = 0;				//scl=0
		Delay_us(FM24CL16BAUDRATE);
	}	
	FM24_SDA_PDOR = 1;				//sda=1
	Delay_us(FM24CL16BAUDRATE);	
	FM24_SCL_PDOR = 0;				//scl=0
	Delay_us(FM24CL16BAUDRATE);		
		
	return temp;
} 		

/*******************************************************************************
  * *函数名称	I2CWriteData
  * *函数说明   I2C指定地址读数据
  * *输入参数   无
  * *输出参数   无
  * *返回参数   data
*******************************************************************************/
static INT32U I2CWriteData(INT8U *pdata, INT16U address, INT32U count)
{
	if (g_nHardNo == 0)
	{
		#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register */
	    //OS_CPU_SR  cpu_sr = 0;
		#endif
		INT32U writenum=0;
		INT32U i;
		
		//	OS_ENTER_CRITICAL();
		I2C_Start();
		I2C_WriteByte(FRAMAddWr|(INT8U)((address & 0x700)>>7) );
	        //I2C_WriteByte(FRAMAddWr);
		Ack();
	        //I2C_WriteByte(address>>8);
		//Ack();
		I2C_WriteByte(address&0xff);
		Ack();
		for(i=0;i<count;i++)
		{
			I2C_WriteByte(*pdata);
			pdata++;
			writenum++;
			Ack();		
		}	
		I2C_Stop();
	//	OS_EXIT_CRITICAL();	
		Delay_us(100);	
		
		return writenum;		
	}
	else
	{
		#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register */
	    //OS_CPU_SR  cpu_sr = 0;
		#endif
		INT32U writenum=0;
		INT32U i;
		
		//	OS_ENTER_CRITICAL();
		I2C_Start();
		//I2C_WriteByte(FRAMAddWr|(INT8U)((address & 0x700)>>7) );
	        I2C_WriteByte(FRAMAddWr);
		Ack();
	        I2C_WriteByte(address>>8);
		Ack();
		I2C_WriteByte(address&0xff);
		Ack();
		for(i=0;i<count;i++)
		{
			I2C_WriteByte(*pdata);
			pdata++;
			writenum++;
			Ack();		
		}	
		I2C_Stop();
	//	OS_EXIT_CRITICAL();	
		Delay_us(100);	
		
		return writenum;		
	}

}

/*******************************************************************************
  * *函数名称	I2CReadData
  * *函数说明   I2C指定地址读数据
  * *输入参数   无
  * *输出参数   无
  * *返回参数   data
*******************************************************************************/
static INT32U I2CReadData(INT8U *pdata, INT16U address, INT32U count)
{
	if (g_nHardNo == 0)
	{
		#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register */
	    //OS_CPU_SR  cpu_sr = 0;
		#endif
		INT32U readnum=0;
		INT32U i;
		
		//	OS_ENTER_CRITICAL();
		I2C_Start();
		I2C_WriteByte(FRAMAddWr|(INT8U)((address & 0x700)>>7) );
	        //I2C_WriteByte(FRAMAddWr);
		Ack();
	        //I2C_WriteByte(address>>8);
		//Ack();
		I2C_WriteByte(address&0xff);
		Ack();
	        
		I2C_Start();
		
		I2C_WriteByte(FRAMAddRd|(INT8U)((address & 0x700)>>7) );
	        //I2C_WriteByte(FRAMAddRd);
		Ack();
		for(i=1;i<count;i++)
		{
			*pdata=I2C_ReadByte();
			MAck();		
			pdata++;
			readnum++;
		}
		*pdata=I2C_ReadByte();
		readnum++;
		FM24_SCL_PDOR = 0;				//scl=0
	    No_Ack();
		I2C_Stop();
		//	OS_EXIT_CRITICAL();	
		Delay_us(100);	
		
		return readnum;	
	}
	else
	{
		#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register */
	    //OS_CPU_SR  cpu_sr = 0;
		#endif
		INT32U readnum=0;
		INT32U i;
		
		//	OS_ENTER_CRITICAL();
		I2C_Start();
		//I2C_WriteByte(FRAMAddWr|(INT8U)((address & 0x700)>>7) );
	        I2C_WriteByte(FRAMAddWr);
		Ack();
	        I2C_WriteByte(address>>8);
		Ack();
		I2C_WriteByte(address&0xff);
		Ack();
	        
		I2C_Start();
		//I2C_WriteByte(FRAMAddRd|(INT8U)((address & 0x700)>>7) );
	        I2C_WriteByte(FRAMAddRd);
		Ack();
		for(i=1;i<count;i++)
		{
			*pdata=I2C_ReadByte();
			MAck();		
			pdata++;
			readnum++;
		}
		*pdata=I2C_ReadByte();
		readnum++;
		FM24_SCL_PDOR = 0;				//scl=0
	    No_Ack();
		I2C_Stop();
		//	OS_EXIT_CRITICAL();	
		Delay_us(100);	
		
		return readnum;	
	}

}

			 
////////////////////////////////////////////////////////////////////////////////
/* FRAM操作集合 */
INT32U FRAMInit( void )
{	
	FM24Init();
	return FRAM_SUCC;
}

INT32U FRAMRead( INT16U nAddr, INT8U* pBuf, INT32U nLen )
{
#if 1			//由于读写未关中断，加入读正确判断
	INT32U i;
	INT32U nBytesRead;
	INT32U nCurrBytes;
	
	for(i=0;i<3;i++)
	{
		nBytesRead = 0;
		while( nBytesRead < nLen )
		{
			nCurrBytes = ( (nLen-nBytesRead) >= 128 ) ? 128 : (nLen-nBytesRead);			
			I2CReadData( FramCmpBuf, nAddr, nCurrBytes );
			I2CReadData( pBuf, nAddr, nCurrBytes );
			if( memcmp( pBuf, FramCmpBuf, nCurrBytes ) != 0 )
			{
				break;	
			}	
			nAddr += nCurrBytes;
			pBuf += nCurrBytes;
			nBytesRead += nCurrBytes;			
		}
		if(nBytesRead == nLen)
			return nBytesRead;				
	}
	return FRAM_ERROR_READ; 
#else
	INT32U ret;
	ret = I2CReadData( pBuf, nAddr, nLen );
	if( ret == nLen )
		return ret;
	else
		return FRAM_ERROR_READ; 
#endif
}

INT32U FRAMWrite(INT16U nAddr, const INT8U* pBuf, INT32U nLen )
{
#if 0			//由于读写未关中断，加入写正确判断
	INT32U i;
	INT32U nBytesWrited;
	INT32U nCurrBytes;
		
	for(i=0;i<3;i++)
	{
		nBytesWrited = 0;
		while( nBytesWrited < nLen )
		{
			nCurrBytes = ( (nLen-nBytesWrited) >= 128 ) ? 128 : (nLen-nBytesWrited);
			FRAMWPDIS();
			I2CWriteData( (INT8U*)pBuf, nAddr, nCurrBytes );
			I2CReadData( FramCmpBuf, nAddr, nCurrBytes );
			FRAMWPEN();
			if( memcmp( pBuf, FramCmpBuf, nCurrBytes ) != 0 )
			{
				break;	
			}	
			nAddr += nCurrBytes;
			pBuf += nCurrBytes;
			nBytesWrited += nCurrBytes;
		}
		if(nBytesWrited == nLen)
			return nBytesWrited;			
	}
	return FRAM_ERROR_READ;
#else
	INT32U ret;
	
	FRAMWPDIS();
	ret = I2CWriteData( (INT8U*)pBuf, nAddr, nLen );
	FRAMWPEN();
	if( ret == nLen )
		return ret;
	else
		return FRAM_ERROR_READ;	
#endif
}

INT32U FRAMReset( void )
{
    
    return FRAM_SUCC;
}
