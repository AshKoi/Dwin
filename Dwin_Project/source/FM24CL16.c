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
  * *��������	FRAMInit
  * *����˵��   ��ʼ��
  * *�������   ��
  * *�������   ��
  * *���ز���   ��
*******************************************************************************/
static void FM24Init( void )
{
	/* ��ʼ��SDA/SCL/WP */
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
  * *��������	  Delay_us
  * *����˵��   ����һ����ʱʱ��  ��Ϊ��Ƶ����Ƶ80Mhz������� Ϊ1us��ʱ,
  * *�������   nCount: ָ����ʱʱ�䳤��  Ϊ10ʱ ��ʱʱ��Ϊ10us
  * *�������   ��
  * *���ز���   ��
*******************************************************************************/
static void Delay_us(INT32U nCount)
{
    int i,j;
    for (i=0; i<nCount; i++)
        for (j=0; j<11; j++);           //80MHz��1usΪ11          
}

/*******************************************************************************
  * *��������	I2C_Start
  * *����˵��   ����I2C
  * *�������   ��
  * *�������   ��
  * *���ز���   ��
*******************************************************************************/
static void I2C_Start(void)
{	
	FM24_SDA_DDR = 1;				//sda���
	FM24_SDA_PDOR = 1;				//sda=1
	Delay_us(1);
	FM24_SCL_PDOR = 1;				//scl=1
	Delay_us(FM24CL16BAUDRATE);
	FM24_SDA_PDOR = 0;				//sda=0
	Delay_us(FM24CL16BAUDRATE);
}

/*******************************************************************************
  * *��������	I2C_Stop
  * *����˵��   ֹͣI2C
  * *�������   ��
  * *�������   ��
  * *���ز���   ��
*******************************************************************************/
static void I2C_Stop(void)
{
    FM24_SDA_DDR = 1;				//sda���	
	FM24_SDA_PDOR = 0;				//sda=0
	Delay_us(1);
	FM24_SCL_PDOR = 1;				//scl=1
	Delay_us(FM24CL16BAUDRATE);
	FM24_SDA_PDOR = 1;				//sda=1
	Delay_us(FM24CL16BAUDRATE);	
}
		
/*******************************************************************************
  * *��������	Ack
  * *����˵��   Ӧ��λ
  * *�������   ��
  * *�������   ��
  * *���ز���   ��
*******************************************************************************/
static void Ack(void)
{	
    INT16U i = 0;
    FM24_SDA_DDR = 0;				//sda����
	FM24_SCL_PDOR = 1;				//scl=1
	Delay_us(FM24CL16BAUDRATE);
	while(( FM24_SDA_PDIR ==1 )&&(i<500)) i++;
	FM24_SCL_PDOR = 0;				//scl=0
	Delay_us(FM24CL16BAUDRATE);
}	

/*******************************************************************************
  * *��������	MAck
  * *����˵��   Ӧ��λ
  * *�������   ��
  * *�������   ��
  * *���ز���   ��
*******************************************************************************/
static void MAck(void)
{
    FM24_SDA_DDR = 1;				//sda���
	FM24_SDA_PDOR = 0;				//sda=0
	Delay_us(1);
	FM24_SCL_PDOR = 1;				//scl=1
	Delay_us(FM24CL16BAUDRATE);
	FM24_SCL_PDOR = 0;				//scl=0
	Delay_us(FM24CL16BAUDRATE);
}	

/*******************************************************************************
  * *��������	No_Ack
  * *����˵��   ��Ӧ��λ
  * *�������   ��
  * *�������   ��
  * *���ز���   ��
*******************************************************************************/
static void No_Ack(void)
{
    INT32U i = 0;
    FM24_SDA_DDR = 0;				//sda����
	FM24_SCL_PDOR = 1;				//scl=1
	Delay_us(FM24CL16BAUDRATE);
	while(( FM24_SDA_PDIR ==0 )&&(i<500)) i++;
	FM24_SCL_PDOR = 0;				//scl=0
	Delay_us(FM24CL16BAUDRATE);
}	

///*******************************************************************************
//  * *��������	MNo_Ack
//  * *����˵��   ��Ӧ��λ
//  * *�������   ��
//  * *�������   ��
//  * *���ز���   ��
//*******************************************************************************/
//static void MNo_Ack(void)
//{	
//    FM24_SDA_DDR = 1;				//sda���
//	FM24_SDA_PDOR = 0;				//sda=1
//	Delay_us(1);
//	FM24_SCL_PDOR = 1;				//scl=1
//	Delay_us(50);
//	FM24_SCL_PDOR = 0;				//scl=0
//	Delay_us(50);
//}

/*******************************************************************************
  * *��������	I2C_WriteByte
  * *����˵��   I2Cдһ���ֽ�
  * *�������   ��
  * *�������   ��
  * *���ز���   ��
*******************************************************************************/
static void I2C_WriteByte( INT8U data )
{
	INT8U i;
	
    FM24_SDA_DDR = 1;				//sda���	
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
  * *��������	I2C_ReadByte
  * *����˵��   I2C��һ���ֽ�
  * *�������   ��
  * *�������   ��
  * *���ز���   temp 
*******************************************************************************/
static INT8U I2C_ReadByte( void )
{
	INT8U i;
	INT8U temp=0;
	
	FM24_SDA_PDOR = 1;				//sda=1
	Delay_us(FM24CL16BAUDRATE);
	
    FM24_SDA_DDR = 0;				//sda����	
	FM24_SCL_PDOR = 0;				//scl=0
	Delay_us(FM24CL16BAUDRATE);
	for(i=0;i<8;i++)
	{
		FM24_SCL_PDOR = 1;				//scl=1
		if(FM24_SDA_PDIR == 1)
				temp |=0x01;
		else
				temp &=~0x01;
		
		if(7-i)temp=temp<<1;    //��8�ε�ʱ��Ͳ�����������
				
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
  * *��������	I2CWriteData
  * *����˵��   I2Cָ����ַ������
  * *�������   ��
  * *�������   ��
  * *���ز���   data
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
  * *��������	I2CReadData
  * *����˵��   I2Cָ����ַ������
  * *�������   ��
  * *�������   ��
  * *���ز���   data
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
/* FRAM�������� */
INT32U FRAMInit( void )
{	
	FM24Init();
	return FRAM_SUCC;
}

INT32U FRAMRead( INT16U nAddr, INT8U* pBuf, INT32U nLen )
{
#if 1			//���ڶ�дδ���жϣ��������ȷ�ж�
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
#if 0			//���ڶ�дδ���жϣ�����д��ȷ�ж�
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
