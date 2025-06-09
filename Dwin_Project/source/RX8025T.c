/*
 ******************************************************************************
 **
 ** \note Other information.
 **
 ******************************************************************************
 */

#include "global.h"

extern INT8U g_nHardNo;

/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/
//#define CFG_RTC_RX8025

/*******************************************************************************
  * *��������	RX8025TInit
  * *����˵��   ��ʼ��
  * *�������   ��
  * *�������   ��
  * *���ز���   ��
*******************************************************************************/
void RX8025TInit( void )
{
	/* ��ʼ��SCL/SDA */
    RX8025T_SDA_PFR = 0;
    RX8025T_SDA_DDR = 1;
    RX8025T_SDA_PDOR = 1;
    RX8025T_SCL_PFR = 0;
    RX8025T_SCL_DDR = 1;
    RX8025T_SCL_PDOR = 1;	
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
        for (j=0; j<11; j++);          //80M����ֵ11
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
	RX8025T_SDA_DDR = 1;				//sda���
	RX8025T_SDA_PDOR = 1;				//sda=1
	Delay_us(1);
	RX8025T_SCL_PDOR = 1;				//scl=1
	Delay_us(RX8025TBAUDRATE);
	RX8025T_SDA_PDOR = 0;				//sda=0
	Delay_us(RX8025TBAUDRATE);
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
    RX8025T_SDA_DDR = 1;				//sda���	
	RX8025T_SDA_PDOR = 0;				//sda=0
	Delay_us(1);
	RX8025T_SCL_PDOR = 1;				//scl=1
	Delay_us(RX8025TBAUDRATE);
	RX8025T_SDA_PDOR = 1;				//sda=1
	Delay_us(RX8025TBAUDRATE);	
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
    INT32U i = 0;
    RX8025T_SDA_DDR = 0;				//sda����
	RX8025T_SCL_PDOR = 1;				//scl=1
	Delay_us(RX8025TBAUDRATE);
	while((RX8025T_SDA_PDIR ==1 )&&(i<500)) i++;
	RX8025T_SCL_PDOR = 0;				//scl=0
	Delay_us(RX8025TBAUDRATE);
}	

///*******************************************************************************
//  * *��������	MAck
//  * *����˵��   Ӧ��λ
//  * *�������   ��
//  * *�������   ��
//  * *���ز���   ��
//*******************************************************************************/
//static void MAck(void)
//{	
//    RX8025T_SDA_DDR = 1;				//sda���
//	RX8025T_SDA_PDOR = 0;				//sda=0
//	Delay_us(1);
//	RX8025T_SCL_PDOR = 1;				//scl=1
//	Delay_us(50);
//	RX8025T_SCL_PDOR = 0;				//scl=0
//	Delay_us(50);
//}	

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
    RX8025T_SDA_DDR = 0;				//sda����
	RX8025T_SCL_PDOR = 1;				//scl=1
	Delay_us(RX8025TBAUDRATE);
	while((RX8025T_SDA_PDIR ==0 )&&(i<500)) i++;
	RX8025T_SCL_PDOR = 0;				//scl=0
	Delay_us(RX8025TBAUDRATE);
}	

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
	
    RX8025T_SDA_DDR = 1;				//sda���	
	RX8025T_SCL_PDOR = 0;				//scl=0
	Delay_us(RX8025TBAUDRATE);
	for(i=0;i<8;i++)
	{
		if(data&0x80)
			RX8025T_SDA_PDOR = 1;				//sda=1
		else
			RX8025T_SDA_PDOR = 0;				//sda=0
		data=data<<1;
		RX8025T_SCL_PDOR = 1;				//scl=1
		Delay_us(RX8025TBAUDRATE);
		RX8025T_SCL_PDOR = 0;				//scl=0
		Delay_us(RX8025TBAUDRATE);
	}
		
	RX8025T_SDA_PDOR = 1;				//sda=1
	Delay_us(RX8025TBAUDRATE);		
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
	
	RX8025T_SDA_PDOR = 1;				//sda=1
	Delay_us(RX8025TBAUDRATE);
	
    RX8025T_SDA_DDR = 0;				//sda����	
	RX8025T_SCL_PDOR = 0;				//scl=0
	Delay_us(RX8025TBAUDRATE);
	for(i=0;i<8;i++)
	{
		RX8025T_SCL_PDOR = 1;				//scl=1
		if(RX8025T_SDA_PDIR == 1)
				temp |=0x01;
		else
				temp &=~0x01;
		
		if(7-i)temp=temp<<1;    //��8�ε�ʱ��Ͳ�����������
				
		Delay_us(RX8025TBAUDRATE);
		RX8025T_SCL_PDOR = 0;				//scl=0
		Delay_us(RX8025TBAUDRATE);
	}	
	RX8025T_SDA_PDOR = 1;				//sda=1
	Delay_us(RX8025TBAUDRATE);	
	RX8025T_SCL_PDOR = 0;				//scl=0
	Delay_us(RX8025TBAUDRATE);		
		
	return temp;
} 		
	
/*******************************************************************************
  * *��������	Addr_Write
  * *����˵��   I2Cָ����ַд����
  * *�������   ��
  * *�������   ��
  * *���ز���   temp 
*******************************************************************************/
static INT8U Addr_Write(INT32U address,INT8U data)
{
	if (g_nHardNo == 1)
	{
		#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register */
	    //OS_CPU_SR  cpu_sr = 0;
		#endif
		
		//	OS_ENTER_CRITICAL();
		I2C_Start();
		I2C_WriteByte(RX8025TAddWr);
		Ack();
		//I2C_WriteByte((address&0xff)<<4);
	        I2C_WriteByte(address&0xff);
		Ack();
		I2C_WriteByte(data);
		Ack();
		I2C_Stop();
		//	OS_EXIT_CRITICAL();	
		Delay_us(1000);	
		return 1;		
	}
	else
	{
		#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register */
	    //OS_CPU_SR  cpu_sr = 0;
		#endif
		
		//	OS_ENTER_CRITICAL();
		I2C_Start();
		I2C_WriteByte(RX8025TAddWr);
		Ack();
		I2C_WriteByte((address&0xff)<<4);
		Ack();
		I2C_WriteByte(data);
		Ack();
		I2C_Stop();
		//	OS_EXIT_CRITICAL();	
		Delay_us(1000);	
		return 1;		
	}

}	

/*******************************************************************************
  * *��������	I2CWriteData
  * *����˵��   I2Cָ����ַ������
  * *�������   ��
  * *�������   ��
  * *���ز���   data
*******************************************************************************/
static INT8U I2CWriteData(INT8U *pdata, INT32U address, INT32U count)
{
	INT32U writenum =0;
	INT8U i;
	
	for(i=0;i<count;i++)
  	{
		Addr_Write(address,*pdata);
		pdata++;
		address++;
		writenum++;
	}
	return writenum;
}

/*******************************************************************************
  * *��������	Addr_Read
  * *����˵��   I2Cָ����ַ������
  * *�������   ��
  * *�������   ��
  * *���ز���   data
*******************************************************************************/	
static INT8U Addr_Read( INT32U address )
{
	if (g_nHardNo == 1)
	{
		#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register */
	    //OS_CPU_SR  cpu_sr = 0;
		#endif
		INT8U data;
		
		//	OS_ENTER_CRITICAL();
		I2C_Start();
		I2C_WriteByte(RX8025TAddWr);
		Ack();
		//I2C_WriteByte((address&0xff)<<4);
	        I2C_WriteByte(address&0xff);
		Ack();
		I2C_Start();
		I2C_WriteByte(RX8025TAddRd);
		Ack();
		data=I2C_ReadByte();
		RX8025T_SCL_PDOR = 0;				//scl=0
	    No_Ack();
		I2C_Stop();
		//	OS_EXIT_CRITICAL();	
		Delay_us(1000);	
	
		return data;	
	}
	else
	{
		#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register */
	    //OS_CPU_SR  cpu_sr = 0;
		#endif
		INT8U data;
		
		//	OS_ENTER_CRITICAL();
		I2C_Start();
		I2C_WriteByte(RX8025TAddWr);
		Ack();
		I2C_WriteByte((address&0xff)<<4);
		Ack();
		I2C_Start();
		I2C_WriteByte(RX8025TAddRd);
		Ack();
		data=I2C_ReadByte();
		RX8025T_SCL_PDOR = 0;				//scl=0
	    No_Ack();
		I2C_Stop();
		//	OS_EXIT_CRITICAL();	
		Delay_us(1000);	
	
		return data;	
	}

}

/*******************************************************************************
  * *��������	I2CReadData
  * *����˵��   I2Cָ����ַ������
  * *�������   ��
  * *�������   ��
  * *���ز���   data
*******************************************************************************/
static INT8U I2CReadData(INT8U *pdata, INT32U address, INT32U count)
{
	INT32U readnum = 0;
	INT8U i;
	
	for(i=0;i<count;i++)
  	{
		*pdata++=Addr_Read(address);
		address++;
		readnum++;
	}
	return readnum;
}


/************************************************************
 *  ��������������������ڼ����������ڿ�ʼ��2000��1��1�յ�����
 ************************************************************/
INT8U DayOfWeek( INT16U year, INT16U month, INT16U date)
{
	INT16U dayfrom2000;
	INT16U i,j;

	dayfrom2000 = 0;
	for( i=2000; i<year; i++)
	{
		if( i%4 == 0 )
			dayfrom2000 += 366;
		else
			dayfrom2000 += 365;
	}
	for(j=1;j<month;j++)
	{
		switch(j)
		{
			case 1:
			case 3:
			case 5:
			case 7:
			case 8:
			case 10:
			case 12:	// not go here
				dayfrom2000+=31;
				break;
			case 4:
			case 6:
			case 9:
			case 11:
				dayfrom2000+=30;
				break;
			case 2:
				if(year%4 ==0)
					dayfrom2000+=29;
				else
					dayfrom2000+=28;
				break;
		}
	}

	
	dayfrom2000 += date-1;
	dayfrom2000 += 6;	// 2000.01.01  is  Saterday  , 6
	
	return (dayfrom2000%7);
}
			 
////////////////////////////////////////////////////////////////////////////////
OS_EVENT* g_RTCLocker = NULL;
#define RTC_LOCKER_TIMEOUT		100

/* ===============================================================================
                   ʱ�Ӳ�������
 ===============================================================================*/
INT32U RTCInit( void )
{
	INT8U sbuf[16];
	
	RX8025TInit();	

#if 0
    res = I2CReadData( sbuf, RX8025_REG_SEC, 16 );
	if( res == 16 )
	{
		
        if(sbuf[14] != 0x20)
        {
            sbuf[0] = 0x20;
            sbuf[1] = 0x00;
            I2CWriteData(sbuf, RX8025_REG_CTRL1, 2);
        }
	}
#else
	sbuf[0] = 0x20;
	sbuf[1] = 0x00;
	I2CWriteData(sbuf, RX8025_REG_CTRL1, 2);
#endif        
	g_RTCLocker = OSSemCreate( 1 );
	if( g_RTCLocker == NULL )
		return RTC_ERROR_INIT;
	return RTC_SUCC;
}

/************************************************************
 *  ��ȡRTC���ں�ʱ��
 ************************************************************/
INT32U RTCDateTimeRead( STD_TIME* pTime )
{
	INT8U j,res;
	INT8U szTemp[8];
	INT8U mask_table[] = { 0x7F, 0x7F, 0x3F, 0x7F, 0x3F, 0x1F, 0xFF };
	INT8U err;
	OS_SEM_DATA	semData;
	INT32U retCode = RTC_SUCC;
	
	if( g_RTCLocker == NULL )	return RTC_ERROR_INIT;
	if( pTime == NULL )			return RTC_ERROR_ARGUMENTS;
	memset( pTime, 0, sizeof(STD_TIME) );
	
	OSSemPend( g_RTCLocker, RTC_LOCKER_TIMEOUT, &err );
	if( err != OS_ERR_NONE )
      return RTC_ERROR_TIMEOUT;
	
	// ��ȡ״̬�Ĵ���
	#if 0
	res = I2CReadData( szTemp, TIME_8025T_FALG_REGISTER, 1 );
	if( res == 1 )
	{
		if( szTemp[0] & 0x03 )
			retCode = RTC_ERROR_POWERDOWN;				// ������ֹͣ������֤׼ȷ�����ڡ�ʱ��
	}
	else
	{
		retCode = RTC_ERROR_STATUS_READ;				// ��״̬�Ĵ�������
		goto _ReadExit;
	}
	#else
	
	#endif
	res = I2CReadData( szTemp, TIME_8025T_SEC, 7 );
	if( res != 7 ) 
		retCode = RTC_ERROR_TIME_READ;
	for( j=0; j<6; j++)
		szTemp[j] &= mask_table[j] ;
    
    szTemp[3] = DayOfWeek( BCD2Dec(szTemp[6])+2000, BCD2Dec(szTemp[5]), BCD2Dec(szTemp[4]));        //ͨ������ó���������ֵ
	pTime->nYear   = (szTemp[6]);
	pTime->nMon    = (szTemp[5]);
	pTime->nDay    = (szTemp[4]);
	pTime->nWeek   = (szTemp[3]);
	pTime->nHour   = (szTemp[2]);
	pTime->nMin    = (szTemp[1]);
	pTime->nSec    = (szTemp[0]);
    if (pTime->nWeek == 0)
        pTime->nWeek = 7;
    
#ifdef CFG_RTC_RX8025
	for (j = 0; j < 7;j++)
    {
        if (((pTime->nWeek) & (1 << j)))
        {
            pTime->nWeek = j;
            break;
        }
    }
#endif
 
	
//_ReadExit:
	OSSemQuery( g_RTCLocker, &semData );
	if( semData.OSCnt == 0 )
		OSSemPost( g_RTCLocker );
	return retCode;
}

/************************************************************
 *  ����RTC���ں�ʱ��
 ************************************************************/
INT32U RTCDateTimeWrite( const STD_TIME* pTime )
{
	static INT8U res;
	INT8U buf[9];
	INT8U sbuf[2];
	INT16U year, month, day;
	INT32U retCode;
	INT8U err;
	OS_SEM_DATA	semData;
	
	if( g_RTCLocker == NULL )	return RTC_ERROR_INIT;
	if( pTime == NULL )			return RTC_ERROR_ARGUMENTS;	
	
	if( pTime->nYear>0x99 ) 						return RTC_ERROR_ARGUMENTS;	// �����ʱ���ʽ	
	if( (pTime->nMon>0x12) || (pTime->nMon<0x01) )	return RTC_ERROR_ARGUMENTS;
	if( (pTime->nDay>0x31) || (pTime->nDay<0x01) )  return RTC_ERROR_ARGUMENTS;
	if( pTime->nHour>0x23  ) 						return RTC_ERROR_ARGUMENTS;
	if( pTime->nMin>0x59) 							return RTC_ERROR_ARGUMENTS;
	if( pTime->nSec>0x59) 							return RTC_ERROR_ARGUMENTS;
	
	OSSemPend( g_RTCLocker, RTC_LOCKER_TIMEOUT, &err );
	if( err != OS_ERR_NONE )
      return RTC_ERROR_TIMEOUT;

	/* ����״̬�Ĵ��� */
	sbuf[0] = 0x20;
	res = I2CWriteData( sbuf, TIME_8025T_FALG_REGISTER, 1 );
	if( !res )	
	{
		retCode = RTC_ERROR_STATUS_WRITE;
		goto _WriteExit;
	}
	
	buf[6] = pTime->nYear;
	buf[5] = pTime->nMon;
	buf[4] = pTime->nDay;
	buf[2] = pTime->nHour;
	buf[1] = pTime->nMin;
	buf[0] = pTime->nSec;
	
	// ����week
	year = BCD2Dec( pTime->nYear ) + 2000;
	month= BCD2Dec( pTime->nMon );
	day  = BCD2Dec( pTime->nDay );
	buf[3] = 1 << DayOfWeek( year, month, day );        //
	if( buf[3] == 0 ) buf[3] = 7;
	
	/* д��ʱ�� */
	res = I2CWriteData( buf, TIME_8025T_SEC, 7 );
	if( res != 7 )
		retCode = RTC_ERROR_TIME_WRITE;				// д����̳��ִ���д��ʧ��
	else
		retCode = RTC_SUCC;							// д��ɹ�

_WriteExit:
	OSSemQuery( g_RTCLocker, &semData );
	if( semData.OSCnt == 0 )
		OSSemPost( g_RTCLocker );
	return retCode;
}