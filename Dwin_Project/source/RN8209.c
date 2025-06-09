/*
 ******************************************************************************
 **
 ** \note Other information.
 **
 ******************************************************************************
 */

#include "global.h"

/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/

/*!
 ******************************************************************************
 ** \brief Initialize the RN8209 
 **
 ** \param None
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RN8209_Init( void )
{
    uint16_t tBGR;
    uint32_t tSysFrePCLK2;
    
    /* Spi Initialize */
    bFM3_GPIO_PFR2_P1 = 1;
    bFM3_GPIO_PFR2_P2 = 1;
    bFM3_GPIO_PFR2_P3 = 1;
    FM3_GPIO->EPFR07 |= 0x00000150;
    
	bFM3_MFS0_CSIO_SCR_UPCL = 1;
    FM3_MFS0_CSIO->SMR = 0x46;
    FM3_MFS0_CSIO->SCR |= 0x21;
    tSysFrePCLK2 = SystemCoreClock/(1<<((FM3_CRG->APBC2_PSR&0x03)));
    tBGR = (uint16_t)(((tSysFrePCLK2 + (RN8209BAUDRATE / 2)) / (uint32_t)RN8209BAUDRATE) - 1);
    FM3_MFS0_CSIO->BGR = tBGR;    
    FM3_MFS0_CSIO->SSR |= 0x80;
    bFM3_MFS0_CSIO_SCR_TXE = 0;
    bFM3_MFS0_CSIO_SCR_RXE = 0;
	bFM3_MFS0_CSIO_ESCR_WT0 = 0;
	bFM3_MFS0_CSIO_ESCR_WT1 = 0;

    bFM3_MFS0_CSIO_SMR_SCINV = 0;
    bFM3_MFS0_CSIO_SMR_BDS = 1;
	
    /* RN_CS Initialize */
    bFM3_GPIO_PFRA_P4 = 0;
    bFM3_GPIO_DDRA_P4 = 1;
    bFM3_GPIO_PDORA_P4 = 1;
}

/*!
 ******************************************************************************
 ** \brief Send single byte  
 **
 ** \param data
 **         
 ** \return send byte number
 **
 ******************************************************************************
 */
static INT8U SpiSendByte( INT8U data )
{   
    INT32U delay = 0;
	
    bFM3_MFS0_CSIO_SMR_SOE = 1;
    bFM3_MFS0_CSIO_SCR_TXE = 1;
    bFM3_MFS0_CSIO_SCR_RXE = 0;
    FM3_MFS0_CSIO->TDR = data;
	Delayms(8);
    while( !bFM3_MFS0_CSIO_SSR_TDRE )
    {
        delay++;
        if( delay == 20000 )
            return 0;
    }
    bFM3_MFS0_CSIO_SCR_TXE = 0;
    bFM3_MFS0_CSIO_SMR_SOE = 0;    
    return 1;
}

/*!
 ******************************************************************************
 ** \brief receive single byte  
 **
 ** \param data
 **         
 ** \return send byte number
 **
 ******************************************************************************
 */
static INT8U SpiGetByte( void )
{
    INT32U delay = 0;
	INT8U ret;

    bFM3_MFS0_CSIO_SMR_SOE = 0;
    bFM3_MFS0_CSIO_SCR_TXE = 1;
    bFM3_MFS0_CSIO_SCR_RXE = 1;
    FM3_MFS0_CSIO->TDR = 0x1F;
	Delayms(8);
    while( !bFM3_MFS0_CSIO_SSR_RDRF )
    {
        delay++;
        if( delay == 20000 )
            return 0;
    }
    bFM3_MFS0_CSIO_SCR_TXE = 0;
    bFM3_MFS0_CSIO_SCR_RXE = 0;
	ret = FM3_MFS0_CSIO->RDR;
    return ret;
}

/*!
 ******************************************************************************
 ** \brief Write RN8209 Reg  
 **
 ** \param address:Reg address
 *         *pt:The write data buffer in idata
 *         len:The amount of bytes to be accessed.
 **         
 ** \return send bytes number
 **
 ******************************************************************************
 */
INT32U RN8209Write( INT8U* buff, INT8U addr, INT32U len )
{
//#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register */
//    OS_CPU_SR  cpu_sr = 0;
//#endif
	INT8U i;
	
//	OS_ENTER_CRITICAL();
	RN8209CSEN();
	Delayus(200);
	SpiSendByte(addr|0x80);
	Delayus(600);
	for(i=0;i<len;i++)
	{
		SpiSendByte(buff[len-i-1]);		
		Delayus(600);
	}	
	RN8209CSDIS();
//	OS_EXIT_CRITICAL();	
	
	return len;
}

/*!
 ******************************************************************************
 ** \brief Read RN8209 Reg  
 **
 ** \param address:Reg address
 *         *pt:The read data buffer in idata
 *         len:The amount of bytes to be accessed.
 **         
 ** \return read bytes number
 **
 ******************************************************************************
 */
INT32U RN8209Read( INT8U* buff, INT8U addr, INT32U len )
{
//#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register */
//    OS_CPU_SR  cpu_sr = 0;
//#endif
	INT8U i;
	
//	OS_ENTER_CRITICAL();
	RN8209CSEN();
	Delayus(200);
	SpiSendByte(addr&0x7f);
	Delayus(600);
	for(i=0;i<len;i++)
	{
		*(buff+len-i-1)=SpiGetByte();	
		Delayus(600);
	}	
	RN8209CSDIS();
//	OS_EXIT_CRITICAL();		
	return len;
}


////////////////////////////////////////////////////////////////////////////////
/* RN8209操作集合 */
OS_EVENT* g_RN8209Locker;
#define RN8209_LOCKER_TIMEOUT		100

INT32U RN8209Init( void )
{        
	RN8209_Init();
	g_RN8209Locker = OSSemCreate( 1 );
	if( g_RN8209Locker == NULL )
		return RN8209_ERROR_INIT;
	
	return RN8209_SUCC;
}  

/************************************************************
 *  读取RN8209
 ************************************************************/
INT32U RN8209DateRead(INT8U address,INT8U *pt,INT8U len)
{
	INT32U retCode= RN8209_SUCC;
	INT8U err;
	OS_SEM_DATA	semData;
        
	if( g_RN8209Locker == NULL )	
		return RN8209_ERROR_INIT;
		
	OSSemPend( g_RN8209Locker, RN8209_LOCKER_TIMEOUT, &err );
	if( err != OS_ERR_NONE )
           return RN8209_ERROR_TIMEOUT;
	RN8209Read(pt,address,len);  
	OSSemQuery( g_RN8209Locker, &semData );
	if( semData.OSCnt == 0 )
		OSSemPost( g_RN8209Locker );
	
	return retCode;
}    