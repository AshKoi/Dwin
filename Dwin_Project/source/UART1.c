/***************************************************************
****************************************************************/
#include "config.h"
#include "global.h"
#include "COMM.h"
#include "uart.h"

extern	COMM_RING_BUF  Comm1Buf;            //MFS_Ch1   DMA CH1
extern	COMM_RING_BUF  Comm2Buf;            //MFS_Ch2   DMA CH2
extern	COMM_RING_BUF  Comm3Buf;            //MFS_Ch4   DMA CH4
extern	COMM_RING_BUF  Comm4Buf;            //MFS_Ch5   DMA CH5
extern	COMM_RING_BUF  Comm5Buf;            //MFS_Ch6   DMA CH6
extern	COMM_RING_BUF  Comm6Buf;            //MFS_Ch7   DMA CH7


#define UART_INT_PRIO_LEVEL		BSP_INT_PRIO_HIGHEST

/**********************************************************************
 *	void _UARTInit( void)
 *
 *	初始化串口硬件设置，含中断
 *
 **********************************************************************/
void UARTInit( INT8U port )
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr = 0;
#endif
               
    OS_ENTER_CRITICAL();
    
	if( port == COMM1 ) 						// 抄表4852
	{
			bFM3_GPIO_PFR1_P1 = 1;					//P11,P12  SIN1_1, SOT1_1
			bFM3_GPIO_PFR1_P2 = 1;
			bFM3_GPIO_EPFR07_SOT1B1 = 1;
			bFM3_GPIO_EPFR07_SOT1B0 = 0;
			bFM3_GPIO_EPFR07_SIN1S1 = 1;
			bFM3_GPIO_EPFR07_SIN1S0 = 0;
	}
	else if( port == COMM2 )                	// 红外
	{
		bFM3_GPIO_PFR7_P2 = 1;           		//P72,P73   SIN2_0, SOT2_0
		bFM3_GPIO_PFR7_P3 = 1;
		bFM3_GPIO_EPFR07_SOT2B1 = 0;
		bFM3_GPIO_EPFR07_SOT2B0 = 1;
		bFM3_GPIO_EPFR07_SIN2S1 = 0;
		bFM3_GPIO_EPFR07_SIN2S0 = 1;
	}
	else if( port == COMM3 )                	//RS232
	{
		bFM3_GPIO_PFR0_P5 = 1;           		//P05,P06   SIN4_2, SOT4_2
		bFM3_GPIO_PFR0_P6 = 1;
		bFM3_GPIO_EPFR08_SOT4B1 = 1;
		bFM3_GPIO_EPFR08_SOT4B0 = 1;
		bFM3_GPIO_EPFR08_SIN4S1 = 1;
		bFM3_GPIO_EPFR08_SIN4S0 = 1;
	}
		else if( port == COMM4 )					//条形码 载波
	{
		bFM3_GPIO_PFR3_P6 = 1;           		//P36,P37   SIN5_2, SOT5_2
		bFM3_GPIO_PFR3_P7 = 1;
		bFM3_GPIO_EPFR08_SOT5B1 = 1;
		bFM3_GPIO_EPFR08_SOT5B0 = 1;
		bFM3_GPIO_EPFR08_SIN5S1 = 1;
		bFM3_GPIO_EPFR08_SIN5S0 = 1;
	}
	else if( port == COMM5 )                	//GPRS
	{
		bFM3_GPIO_PFR5_P3 = 1;           		//P53,P54   SIN6_0, SOT6_0
		bFM3_GPIO_PFR5_P4 = 1;
		bFM3_GPIO_EPFR08_SOT6B1 = 0;
		bFM3_GPIO_EPFR08_SOT6B0 = 1;
		bFM3_GPIO_EPFR08_SIN6S1 = 0;
		bFM3_GPIO_EPFR08_SIN6S0 = 1;
	}
		else if( port == COMM6 )					//抄表4851
	{
		bFM3_GPIO_PFR4_PD = 1;           		//P4D,P4E   SIN7_1, SOT7_1
		bFM3_GPIO_PFR4_PE = 1;
		bFM3_GPIO_EPFR08_SOT7B1 = 1;
		bFM3_GPIO_EPFR08_SOT7B0 = 0;
		bFM3_GPIO_EPFR08_SIN7S1 = 1;
		bFM3_GPIO_EPFR08_SIN7S0 = 0;
	}
		else if( port == COMM7 )					//232口连接故障板
		{
			bFM3_GPIO_PFR1_P4 = 1;					//P14,P15  SIN0_1, SOT0_1
			bFM3_GPIO_PFR1_P5 = 1;
			bFM3_GPIO_EPFR07_SOT0B1 = 1;
			bFM3_GPIO_EPFR07_SOT0B0 = 0;
			bFM3_GPIO_EPFR07_SIN0S1 = 1;
			bFM3_GPIO_EPFR07_SIN0S0 = 0;
		}
    
    OS_EXIT_CRITICAL();
    return;
}

/************************************************************************
 *	UARTConfigure
 ************************************************************************/
INT8U UARTConfigure(INT8U port, INT32U baudRate, INT8U parity, INT8U stopBits, INT8U charLen,INT32U timeout)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr = 0;
#endif
    
    MFS_UART0ISRCallbackT  tUARTIrqHandle;
    
	INT8U err;
	COMM_RING_BUF *pbuf;
	INT32U iTotal;
//    INT32U ScrArry;
	
	pbuf = GetCommBuf( port );			        /* Obtain pointer to communications channel */
	if( pbuf == NULL )
             return (UART_ERR_CHANNEL);
	
	iTotal = OS_TICKS_PER_SEC * timeout / 1000;	//换算成TICKS
    OSSemPend( pbuf->UsedSem, iTotal, &err );	//得到信号灯
    if( err != OS_NO_ERR )
      return UART_ERR_TIMEOUT;	//超时了，还是无法得到信号灯

	if( baudRate > UART_MAX_BAUD )
		return UART_ERR_BAUD;
	if( parity > EVEN_PARITY )
		return UART_ERR_PARITY;
	if( (stopBits != ONE_STOP_BIT) && (stopBits != TWO_STOP_BIT) )
		return UART_ERR_STOPBIT;
	if( (charLen != SIX_CHAR_BIT) && (charLen != SEVEN_CHAR_BIT) && (charLen != EIGHT_CHAR_BIT) )
		return UART_ERR_CHARLEN;
		
    OS_ENTER_CRITICAL();
    	
	UARTDevSet( port, baudRate, parity, stopBits, charLen );
	if( port == COMM1 )
	{
        tUARTIrqHandle.pISRRXCallback = IrqRxHandle_UART1;       //配置发送、接收中断处理函数
        tUARTIrqHandle.pISRTXCallback = IrqTxHandle_UART1;
        MFS_UART0ConfigISRCallback(MFS_Ch1,&tUARTIrqHandle); 
        
        MFS_UART0RXEnable(MFS_Ch1);                             //使能接收功能
        MFS_UART0TXEnable(MFS_Ch1);                             //使能发送功能
        MFS_UART0IntRXEnable(MFS_Ch1);                          //使能接收中断
        NVIC_EnableIRQ((IRQn_Type)(MFS1RX_IRQn));               //使能接收中断优先级
	}
	else if( port == COMM2 )
	{
        tUARTIrqHandle.pISRRXCallback = IrqRxHandle_UART2;       
        tUARTIrqHandle.pISRTXCallback = IrqTxHandle_UART2;
        MFS_UART0ConfigISRCallback(MFS_Ch2,&tUARTIrqHandle);         
        
        MFS_UART0RXEnable(MFS_Ch2);                             
        MFS_UART0TXEnable(MFS_Ch2);                             
        MFS_UART0IntRXEnable(MFS_Ch2);                          
        NVIC_EnableIRQ((IRQn_Type)(MFS2RX_IRQn));              
	}
	else if( port == COMM3 )
	{
		//LED_RXDD_Off();
		//LED_TXDD_Off();
        tUARTIrqHandle.pISRRXCallback = IrqRxHandle_UART3;       
        tUARTIrqHandle.pISRTXCallback = IrqTxHandle_UART3;
        MFS_UART0ConfigISRCallback(MFS_Ch4,&tUARTIrqHandle); 
		MFS_UART0RXDisable(MFS_Ch4);        
        MFS_UART0ConfigFIFO(MFS_Ch4,&tMFS_UARTFIFOConfigT);    //使能UART的FIFO功能，16字节       
        MFS_UART0RXEnable(MFS_Ch4);                             
        MFS_UART0TXEnable(MFS_Ch4);                             
        MFS_UART0IntRXEnable(MFS_Ch4);                          
        NVIC_EnableIRQ((IRQn_Type)(MFS4RX_IRQn));              
	}
	else if( port == COMM4 )
	{
		//LED_RXDU_Off();
		//LED_TXDU_Off();
        tUARTIrqHandle.pISRRXCallback = IrqRxHandle_UART4;       
        tUARTIrqHandle.pISRTXCallback = IrqTxHandle_UART4;
        MFS_UART0ConfigISRCallback(MFS_Ch5,&tUARTIrqHandle);         
        MFS_UART0ConfigFIFO(MFS_Ch5,&tMFS_UARTFIFOConfigT);    //使能UART的FIFO功能，16字节        
        MFS_UART0RXEnable(MFS_Ch5);                             
        MFS_UART0TXEnable(MFS_Ch5);                             
        MFS_UART0IntRXEnable(MFS_Ch5);                          
        NVIC_EnableIRQ((IRQn_Type)(MFS5RX_IRQn));              
	}
	else if( port == COMM5 )
	{
		//LED_RXDD_Off();
		//LED_TXDD_Off();
        tUARTIrqHandle.pISRRXCallback = IrqRxHandle_UART5;       
        tUARTIrqHandle.pISRTXCallback = IrqTxHandle_UART5;
        MFS_UART0ConfigISRCallback(MFS_Ch6,&tUARTIrqHandle);                
        MFS_UART0ConfigFIFO(MFS_Ch6,&tMFS_UARTFIFOConfigT);    //使能UART的FIFO功能，16字节
		
//        DMA_MFS( ch5_Rx, 4, 1, (uint32_t)&ScrArry,(uint32_t)(&pbuf->RingBufRx[0]), (COMM_RX_BUF_SIZE - 1));
//        FM3_INTREQ->DRQSEL &= ~(0x00001000<<10);                //中断发送至CPU
        
        MFS_UART0RXEnable(MFS_Ch6);                             //使能接收功能
        MFS_UART0TXEnable(MFS_Ch6);                             //使能发送功能
        MFS_UART0IntRXEnable(MFS_Ch6);                          //使能接收中断
        NVIC_EnableIRQ((IRQn_Type)(MFS6RX_IRQn));               //使能接收中断优先级
	}
	else if( port == COMM6 )
	{
		//LED_RXDD_Off();
		//LED_TXDD_Off();
        tUARTIrqHandle.pISRRXCallback = IrqRxHandle_UART6;       
        tUARTIrqHandle.pISRTXCallback = IrqTxHandle_UART6;
        MFS_UART0ConfigISRCallback(MFS_Ch7,&tUARTIrqHandle);      
        MFS_UART0ConfigFIFO(MFS_Ch7,&tMFS_UARTFIFOConfigT);    //使能UART的FIFO功能，16字节
		
//        DMA_MFS( ch7_Rx, 0, 1, (uint32_t)&ScrArry,(uint32_t)(&pbuf->RingBufRx), (COM6_RX_BUF_SIZE - 1));
//        FM3_INTREQ->DRQSEL &= ~(0x00001000<<26);                //MFS7 Rx 中断发送至CPU
        
        MFS_UART0RXEnable(MFS_Ch7);                             //使能接收功能
        MFS_UART0TXEnable(MFS_Ch7);                             //使能发送功能
        MFS_UART0IntRXEnable(MFS_Ch7);                          //使能接收中断
        NVIC_EnableIRQ((IRQn_Type)(MFS7RX_IRQn));               //使能接收中断优先级
	}
    else if( port == COMM7 )
	{
		//LED_RXDD_Off();
		//LED_TXDD_Off();
        tUARTIrqHandle.pISRRXCallback = IrqRxHandle_UART7;       
        tUARTIrqHandle.pISRTXCallback = IrqTxHandle_UART7;
        MFS_UART0ConfigISRCallback(MFS_Ch0,&tUARTIrqHandle);      
        MFS_UART0ConfigFIFO(MFS_Ch0,&tMFS_UARTFIFOConfigT);    //使能UART的FIFO功能，16字节
		
//        DMA_MFS( ch0_Rx, 0, 1, (uint32_t)&ScrArry,(uint32_t)(&pbuf->RingBufRx), (COM7_RX_BUF_SIZE - 1));
//        FM3_INTREQ->DRQSEL &= ~(0x00001000<<26);                //MFS0 Rx 中断发送至CPU
        
        MFS_UART0RXEnable(MFS_Ch0);                             //使能接收功能
        MFS_UART0TXEnable(MFS_Ch0);                             //使能发送功能
        MFS_UART0IntRXEnable(MFS_Ch0);                          //使能接收中断
        NVIC_EnableIRQ((IRQn_Type)(MFS0RX_IRQn));               //使能接收中断优先级
	}

	OS_EXIT_CRITICAL();
	return UART_OK;
}



/************************************************************************
 *   串行口参数设置
 *
 *   INT8U UARTDevSet(INT8U ch, INT16U baudRate, INT8U parity,
 *                    INT8U stopBits, INT8U charLen)
 ************************************************************************/
INT8U UARTDevSet(INT8U port, INT32U baudRate, INT8U parity, INT8U stopBits, INT8U charLen )
{
    MFS_UART0ModeConfigT UART0ModeConfigT;

    if( baudRate == 0 ) baudRate = 1200 ;

    UART0ModeConfigT.Baudrate = baudRate;
    UART0ModeConfigT.Databits = charLen;
    UART0ModeConfigT.Stopbits = stopBits;
    UART0ModeConfigT.Parity   = parity;
    UART0ModeConfigT.Bitorder = Uart0_BITORDER_LSB;
    UART0ModeConfigT.NRZ      = Uart0_NRZ;
    
    if( port == COMM1 )
    {
        MFS_UART0ConfigMode(MFS_Ch1, &UART0ModeConfigT);       
    }
    else if(port == COMM2)
    {
        MFS_UART0ConfigMode(MFS_Ch2, &UART0ModeConfigT);       
    }
    else if(port == COMM3)
    {
        MFS_UART0ConfigMode(MFS_Ch4, &UART0ModeConfigT);       
    }
    else if(port == COMM4)
    {
        MFS_UART0ConfigMode(MFS_Ch5, &UART0ModeConfigT);       
    }
    else if(port == COMM5)
    {
        MFS_UART0ConfigMode(MFS_Ch6, &UART0ModeConfigT);       
    }
    else if(port == COMM6)
    {
        MFS_UART0ConfigMode(MFS_Ch7, &UART0ModeConfigT);       
    }
	else if(port == COMM7)
    {
        MFS_UART0ConfigMode(MFS_Ch0, &UART0ModeConfigT);       
    }
    return UART_OK;
}

/************************************************************************
 *   发送/接收使能控制
 *
 *   void UARTDevControl( INT8U ch, INT16U enable)
 *
 ************************************************************************/
void UARTDevControl( INT8U port, INT16U enable)
{
    INT8U MFS_Ch;
    if( port == COMM1)
        MFS_Ch = MFS_Ch1;
    else if( port == COMM2)
        MFS_Ch = MFS_Ch2;
    else if( port == COMM3)
        MFS_Ch = MFS_Ch4;
    else if( port == COMM4)
        MFS_Ch = MFS_Ch5;
    else if( port == COMM5)
        MFS_Ch = MFS_Ch6;
    else if( port == COMM6)
        MFS_Ch = MFS_Ch7;
	else if( port == COMM7)
        MFS_Ch = MFS_Ch0;
    
	if( (enable & UART_RX_ENABLE) == UART_RX_ENABLE )
        MFS_UART0RXEnable(MFS_Ch);
    else if( (enable & UART_RX_DISABLE) == UART_RX_DISABLE )
        MFS_UART0RXDisable(MFS_Ch);
    else if( (enable & UART_TX_ENABLE) == UART_TX_ENABLE )
        MFS_UART0TXEnable(MFS_Ch);
    else if( (enable & UART_TX_DISABLE) == UART_TX_DISABLE )
        MFS_UART0TXDisable(MFS_Ch);   
}









/****************************************************************
 * 串口中断处理函数 
 ****************************************************************/
void IrqTxHandle_UART1(void)                 //COMM1 Tx中断处理程序
{
}
void IrqRxHandle_UART1(void)                 //COMM1 Rx中断处理程序
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr = 0;
#endif
    OS_SEM_DATA     semData;
    INT8U ssr;
	
    COMM_RING_BUF *pbuf;
    pbuf = &Comm1Buf;			        /* Obtain pointer to communications channel */
	
	ssr = FM3_MFS1_UART->SSR;	
    if( ( ssr & (UART0_SSR_PE|UART0_SSR_FRE|UART0_SSR_ORE)) != 0 )      //接收错误的时候的处理
    {
        MFS_UART0ErrorClr(MFS_Ch1);
        MFS_UART0RXOneData(MFS_Ch1);
        return;
    }	
	
    OS_ENTER_CRITICAL();
	if( ssr & UART0_SSR_RDRF )
    {        
        *pbuf->RingBufRxInPtr = MFS_UART0RXOneData(MFS_Ch1);
        
        //如果串口缓冲区已经满了，会冲掉最开始的数据，保留新接收的数据
        if( (pbuf->RingBufRxInPtr == pbuf->RingBufRxOutPtr) && (pbuf->RingBufRxCtr == pbuf->nRxBufSize) )
        {
            pbuf->RingBufRxOutPtr++;   
            if( pbuf->RingBufRxOutPtr >= &pbuf->RingBufRx[pbuf->nRxBufSize] )
                pbuf->RingBufRxOutPtr = pbuf->RingBufRx;
        }
        
        pbuf->RingBufRxCtr++;
        if(pbuf->RingBufRxCtr >= pbuf->nRxBufSize )
            pbuf->RingBufRxCtr = pbuf->nRxBufSize;
        pbuf->RingBufRxInPtr++;
        if( pbuf->RingBufRxInPtr >= &pbuf->RingBufRx[pbuf->nRxBufSize] )
        {
            pbuf->RingBufRxInPtr = pbuf->RingBufRx;
        }
        OSSemQuery( pbuf->RingBufRxSem, &semData );
		if( semData.OSCnt == 0 )
			OSSemPost( pbuf->RingBufRxSem );
    }
    OS_EXIT_CRITICAL();
}
void IrqTxHandle_UART2(void)                 //COMM2 Tx中断处理程序
{
}
void IrqRxHandle_UART2(void)                 //COMM2 Rx中断处理程序
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr = 0;
#endif
    OS_SEM_DATA     semData;
	INT8U ssr;
	
    COMM_RING_BUF *pbuf;
    pbuf = &Comm2Buf;			        /* Obtain pointer to communications channel */

	ssr = FM3_MFS2_UART->SSR;	
    if( ( ssr & (UART0_SSR_PE|UART0_SSR_FRE|UART0_SSR_ORE)) != 0 )      //接收错误的时候的处理
    {
        MFS_UART0ErrorClr(MFS_Ch2);
        MFS_UART0RXOneData(MFS_Ch2);
        return;
    }	
	
    OS_ENTER_CRITICAL();
	if( ssr & UART0_SSR_RDRF )
    {        
        *pbuf->RingBufRxInPtr = MFS_UART0RXOneData(MFS_Ch2);
        
        //如果串口缓冲区已经满了，会冲掉最开始的数据，保留新接收的数据
        if( (pbuf->RingBufRxInPtr == pbuf->RingBufRxOutPtr) && (pbuf->RingBufRxCtr == pbuf->nRxBufSize) )
        {
            pbuf->RingBufRxOutPtr++;   
            if( pbuf->RingBufRxOutPtr >= &pbuf->RingBufRx[pbuf->nRxBufSize] )
                pbuf->RingBufRxOutPtr = pbuf->RingBufRx;
        }
        pbuf->RingBufRxCtr++;  
        if(pbuf->RingBufRxCtr >= pbuf->nRxBufSize )
            pbuf->RingBufRxCtr = pbuf->nRxBufSize;       
        pbuf->RingBufRxInPtr++;
        if( pbuf->RingBufRxInPtr >= &pbuf->RingBufRx[pbuf->nRxBufSize] )
        {
            pbuf->RingBufRxInPtr = pbuf->RingBufRx;
        }
        OSSemQuery( pbuf->RingBufRxSem, &semData );
		if( semData.OSCnt == 0 )
			OSSemPost( pbuf->RingBufRxSem );
    }
    OS_EXIT_CRITICAL();
}

void IrqTxHandle_UART3(void)                 //COMM3 Tx中断处理程序
{
}
void IrqRxHandle_UART3(void)                 //COMM3 Rx中断处理程序
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr = 0;
#endif
    OS_SEM_DATA     semData;
    INT8U Fbytes=0;
	INT8U ssr,i;
    COMM_RING_BUF *pbuf;
    pbuf = &Comm3Buf;			        /* Obtain pointer to communications channel */
	
	ssr = FM3_MFS4_UART->SSR;
    if( ( ssr & (UART0_SSR_PE|UART0_SSR_FRE|UART0_SSR_ORE)) != 0 )      //接收错误的时候的处理
    {
        MFS_UART0ErrorClr(MFS_Ch4);
        MFS_UART0RXOneData(MFS_Ch4);
        MFS_UART0ConfigFIFO(MFS_Ch4,&tMFS_UARTFIFOConfigT);    //使能UART的FIFO功能，16字节       
        return;
    }	
	
    OS_ENTER_CRITICAL();
	if( ssr & UART0_SSR_RDRF)
	{
#if UART_FIFO_ENABLE 
		Fbytes = FM3_MFS4_UART->FBYTE2;
	
		for(i=0;i<Fbytes;i++)
		{
#endif
			*pbuf->RingBufRxInPtr = MFS_UART0RXOneData(MFS_Ch4);
			
			//如果串口缓冲区已经满了，会冲掉最开始的数据，保留新接收的数据
			if( (pbuf->RingBufRxInPtr == pbuf->RingBufRxOutPtr) && (pbuf->RingBufRxCtr == pbuf->nRxBufSize) )
			{
				pbuf->RingBufRxOutPtr++;   
				if( pbuf->RingBufRxOutPtr >= &pbuf->RingBufRx[pbuf->nRxBufSize] )
					pbuf->RingBufRxOutPtr = pbuf->RingBufRx;
			}
			
			pbuf->RingBufRxCtr++;
			if(pbuf->RingBufRxCtr >= pbuf->nRxBufSize )
				pbuf->RingBufRxCtr = pbuf->nRxBufSize;
			
			pbuf->RingBufRxInPtr++;
			if( pbuf->RingBufRxInPtr >= &pbuf->RingBufRx[pbuf->nRxBufSize] )
			{
				pbuf->RingBufRxInPtr = pbuf->RingBufRx;
			}
#if UART_FIFO_ENABLE 
		}
#endif
        OSSemQuery( pbuf->RingBufRxSem, &semData );
		if( semData.OSCnt == 0 )
			OSSemPost( pbuf->RingBufRxSem );
    }
    OS_EXIT_CRITICAL();
}

void IrqTxHandle_UART4(void)                 //COMM4 Tx中断处理程序
{
}
void IrqRxHandle_UART4(void)                 //COMM4 Rx中断处理程序
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr = 0;
#endif
    OS_SEM_DATA     semData;
    INT8U Fbytes=0;
	INT8U ssr,i;
    COMM_RING_BUF *pbuf;
    pbuf = &Comm4Buf;			        /* Obtain pointer to communications channel */
	
	ssr = FM3_MFS5_UART->SSR;
    if( ( ssr & (UART0_SSR_PE|UART0_SSR_FRE|UART0_SSR_ORE)) != 0 )      //接收错误的时候的处理
    {
        MFS_UART0ErrorClr(MFS_Ch5);
        MFS_UART0RXOneData(MFS_Ch5);
        MFS_UART0ConfigFIFO(MFS_Ch5,&tMFS_UARTFIFOConfigT);    //使能UART的FIFO功能，16字节       
        return;
    }
	
    OS_ENTER_CRITICAL();
	if( ssr & UART0_SSR_RDRF)
	{
#if UART_FIFO_ENABLE 
		Fbytes = FM3_MFS5_UART->FBYTE2;	
		for(i=0;i<Fbytes;i++)
		{
#endif
			*pbuf->RingBufRxInPtr = MFS_UART0RXOneData(MFS_Ch5);
			
			//如果串口缓冲区已经满了，会冲掉最开始的数据，保留新接收的数据
			if( (pbuf->RingBufRxInPtr == pbuf->RingBufRxOutPtr) && (pbuf->RingBufRxCtr == pbuf->nRxBufSize) )
			{
				pbuf->RingBufRxOutPtr++;   
				if( pbuf->RingBufRxOutPtr >= &pbuf->RingBufRx[pbuf->nRxBufSize] )
					pbuf->RingBufRxOutPtr = pbuf->RingBufRx;
			}        
			
			pbuf->RingBufRxCtr++;
			if(pbuf->RingBufRxCtr >= pbuf->nRxBufSize )
				pbuf->RingBufRxCtr = pbuf->nRxBufSize;     
			pbuf->RingBufRxInPtr++;
			if( pbuf->RingBufRxInPtr >= &pbuf->RingBufRx[pbuf->nRxBufSize] )
			{
				pbuf->RingBufRxInPtr = pbuf->RingBufRx;
			}
#if UART_FIFO_ENABLE 
		}
#endif
        OSSemQuery( pbuf->RingBufRxSem, &semData );
		if( semData.OSCnt == 0 )
			OSSemPost( pbuf->RingBufRxSem );
    }
    OS_EXIT_CRITICAL();
}
void IrqTxHandle_UART5(void)                 //COMM5 Tx中断处理程序
{
}
void IrqRxHandle_UART5(void)                 //COMM5 Rx中断处理程序
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr = 0;
#endif
    INT8U Fbytes=0;
	INT8U ssr,i;
    OS_SEM_DATA     semData;   
    COMM_RING_BUF *pbuf;
    pbuf = &Comm5Buf;			        /* Obtain pointer to communications channel */
	
	ssr = FM3_MFS6_UART->SSR;
    if( ( ssr & (UART0_SSR_PE|UART0_SSR_FRE|UART0_SSR_ORE)) != 0 )      //接收错误的时候的处理
    {
        MFS_UART0ErrorClr(MFS_Ch6);
        MFS_UART0RXOneData(MFS_Ch6);
        MFS_UART0ConfigFIFO(MFS_Ch6,&tMFS_UARTFIFOConfigT);    //使能UART的FIFO功能，16字节       
        return;
    }
    
    OS_ENTER_CRITICAL();
	if( ssr & UART0_SSR_RDRF)
	{
#if UART_FIFO_ENABLE 
		Fbytes = FM3_MFS6_UART->FBYTE2;	
		for(i=0;i<Fbytes;i++)
		{
#endif
            *pbuf->RingBufRxInPtr = MFS_UART0RXOneData(MFS_Ch6);
            
            //如果串口缓冲区已经满了，会冲掉最开始的数据，保留新接收的数据
            if( (pbuf->RingBufRxInPtr == pbuf->RingBufRxOutPtr) && (pbuf->RingBufRxCtr == pbuf->nRxBufSize) )
            {
                pbuf->RingBufRxOutPtr++;   
                if( pbuf->RingBufRxOutPtr >= &pbuf->RingBufRx[pbuf->nRxBufSize] )
                    pbuf->RingBufRxOutPtr = pbuf->RingBufRx;
            }
            
            pbuf->RingBufRxInPtr++;
            pbuf->RingBufRxCtr++;
            if(pbuf->RingBufRxCtr >= pbuf->nRxBufSize )
                pbuf->RingBufRxCtr = pbuf->nRxBufSize;
            
            if( pbuf->RingBufRxInPtr >= &pbuf->RingBufRx[pbuf->nRxBufSize] )
            {
                pbuf->RingBufRxInPtr = pbuf->RingBufRx;
            }
#if UART_FIFO_ENABLE 
        }
#endif
        OSSemQuery( pbuf->RingBufRxSem, &semData );
		if( semData.OSCnt == 0 )
			OSSemPost( pbuf->RingBufRxSem );
    }
    OS_EXIT_CRITICAL();
}

void IrqTxHandle_UART6(void)                 //COMM6 Tx中断处理程序
{ 
}

void IrqRxHandle_UART6(void)                 //COMM6 Rx中断处理程序
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr = 0;
#endif
	INT16U Fbytes = 0;
	INT8U ssr,i;
    OS_SEM_DATA     semData;
    COMM_RING_BUF *pbuf;
    pbuf = &Comm6Buf;			        /* Obtain pointer to communications channel */
	
	ssr = FM3_MFS7_UART->SSR;
    if( ( ssr & (UART0_SSR_PE|UART0_SSR_FRE|UART0_SSR_ORE)) != 0 )      //接收错误的时候的处理
    {
        MFS_UART0ErrorClr(MFS_Ch7);
        MFS_UART0RXOneData(MFS_Ch7);
        MFS_UART0ConfigFIFO(MFS_Ch7,&tMFS_UARTFIFOConfigT);    //使能UART的FIFO功能，16字节       
        return;
    }
    
    OS_ENTER_CRITICAL();

/*        bytes_received = FM3_MFS7_UART->FBYTE2;
	FM3_INTREQ->DRQSEL |= (0x00001000<<14);
	while(FM3_MFS7_UART->FBYTE2);
	FM3_INTREQ->DRQSEL &= ~(0x00001000<<14);
*/
			
	if( ssr & UART0_SSR_RDRF)
	{
#if UART_FIFO_ENABLE 
		Fbytes = FM3_MFS7_UART->FBYTE2;
		for(i=0;i<Fbytes;i++)
		{
#endif
			*pbuf->RingBufRxInPtr = MFS_UART0RXOneData(MFS_Ch7);
			//如果串口缓冲区已经满了，会冲掉最开始的数据，保留新接收的数据
			if( (pbuf->RingBufRxInPtr == pbuf->RingBufRxOutPtr) && (pbuf->RingBufRxCtr == pbuf->nRxBufSize) )
			{
				pbuf->RingBufRxOutPtr++;   
				if( pbuf->RingBufRxOutPtr >= &pbuf->RingBufRx[pbuf->nRxBufSize] )
					pbuf->RingBufRxOutPtr = pbuf->RingBufRx;
			}
			
			pbuf->RingBufRxInPtr++;
			pbuf->RingBufRxCtr++;
			if(pbuf->RingBufRxCtr >= pbuf->nRxBufSize )
				pbuf->RingBufRxCtr = pbuf->nRxBufSize;
			
			if( pbuf->RingBufRxInPtr >= &pbuf->RingBufRx[pbuf->nRxBufSize] )
			{
				pbuf->RingBufRxInPtr = pbuf->RingBufRx;
			}
#if UART_FIFO_ENABLE 	
		}
#endif

	OSSemQuery( pbuf->RingBufRxSem, &semData );
	if( semData.OSCnt == 0 )
		OSSemPost( pbuf->RingBufRxSem );
	}
    OS_EXIT_CRITICAL();
}


void IrqTxHandle_UART7(void)                 //COMM7 Tx中断处理程序
{ 
}

void IrqRxHandle_UART7(void)                 //COMM7 Rx中断处理程序
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr = 0;
#endif
	INT16U Fbytes = 0;
	INT8U ssr,i;
    OS_SEM_DATA     semData;
    COMM_RING_BUF *pbuf;
    pbuf = &Comm7Buf;			        /* Obtain pointer to communications channel */
	
	ssr = FM3_MFS0_UART->SSR;
    if( ( ssr & (UART0_SSR_PE|UART0_SSR_FRE|UART0_SSR_ORE)) != 0 )      //接收错误的时候的处理
    {
        MFS_UART0ErrorClr(MFS_Ch0);
        MFS_UART0RXOneData(MFS_Ch0);
        MFS_UART0ConfigFIFO(MFS_Ch0,&tMFS_UARTFIFOConfigT);    //使能UART的FIFO功能，16字节       
        return;
    }
    
    OS_ENTER_CRITICAL();

/*        bytes_received = FM3_MFS7_UART->FBYTE2;
	FM3_INTREQ->DRQSEL |= (0x00001000<<14);
	while(FM3_MFS7_UART->FBYTE2);
	FM3_INTREQ->DRQSEL &= ~(0x00001000<<14);
*/
			
	if( ssr & UART0_SSR_RDRF)
	{
#if UART_FIFO_ENABLE 
		Fbytes = FM3_MFS0_UART->FBYTE2;
		for(i=0;i<Fbytes;i++)
		{
#endif
			*pbuf->RingBufRxInPtr = MFS_UART0RXOneData(MFS_Ch0);
			//如果串口缓冲区已经满了，会冲掉最开始的数据，保留新接收的数据
			if( (pbuf->RingBufRxInPtr == pbuf->RingBufRxOutPtr) && (pbuf->RingBufRxCtr == pbuf->nRxBufSize) )
			{
				pbuf->RingBufRxOutPtr++;   
				if( pbuf->RingBufRxOutPtr >= &pbuf->RingBufRx[pbuf->nRxBufSize] )
					pbuf->RingBufRxOutPtr = pbuf->RingBufRx;
			}
			
			pbuf->RingBufRxInPtr++;
			pbuf->RingBufRxCtr++;
			if(pbuf->RingBufRxCtr >= pbuf->nRxBufSize )
				pbuf->RingBufRxCtr = pbuf->nRxBufSize;
			
			if( pbuf->RingBufRxInPtr >= &pbuf->RingBufRx[pbuf->nRxBufSize] )
			{
				pbuf->RingBufRxInPtr = pbuf->RingBufRx;
			}
#if UART_FIFO_ENABLE 	
		}
#endif

	OSSemQuery( pbuf->RingBufRxSem, &semData );
	if( semData.OSCnt == 0 )
		OSSemPost( pbuf->RingBufRxSem );
	}
    OS_EXIT_CRITICAL();
}





