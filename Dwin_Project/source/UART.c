/***************************************************************
****************************************************************/
#include "config.h"
#include "global.h"
#include "COMM.h"
#include "uart.h"

#define UART_INT_PRIO_LEVEL		BSP_INT_PRIO_HIGHEST
extern const COMM_CFG_STRUCT    g_sCommCfgStruct[];
/**********************************************************************
 *	void _UARTInit( void)
 *
 *	��ʼ������Ӳ�����ã����ж�
 *
 **********************************************************************/
void UARTInit( INT8U port )
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr = 0;
#endif
               
    OS_ENTER_CRITICAL();
    
	if( port == COMM1 )                     	// 4852
	{
		bFM3_GPIO_PFR1_P1 = 1;           		//P11,P12  SIN1_1, SOT1_1
		bFM3_GPIO_PFR1_P2 = 1;
		bFM3_GPIO_EPFR07_SOT1B1 = 1;
		bFM3_GPIO_EPFR07_SOT1B0 = 0;
		bFM3_GPIO_EPFR07_SIN1S1 = 1;
		bFM3_GPIO_EPFR07_SIN1S0 = 0;
	}
	else if( port == COMM2 )                	// 
	{
		bFM3_GPIO_PFR7_P2 = 1;           		//P72,P73   SIN2_0, SOT2_0
		bFM3_GPIO_PFR7_P3 = 1;
		bFM3_GPIO_EPFR07_SOT2B1 = 0;
		bFM3_GPIO_EPFR07_SOT2B0 = 1;
		bFM3_GPIO_EPFR07_SIN2S1 = 0;
		bFM3_GPIO_EPFR07_SIN2S0 = 1;
	}
	else if( port == COMM3 )                	//ɨ��ǹ
	{
		bFM3_GPIO_PFR0_P5 = 1;           		//P05,P06   SIN4_2, SOT4_2
		bFM3_GPIO_PFR0_P6 = 1;
		bFM3_GPIO_EPFR08_SOT4B1 = 1;
		bFM3_GPIO_EPFR08_SOT4B0 = 1;
		bFM3_GPIO_EPFR08_SIN4S1 = 1;
		bFM3_GPIO_EPFR08_SIN4S0 = 1;
		
	}
    else if( port == COMM4 )                	//���� 
	{
		bFM3_GPIO_PFR3_P6 = 1;           		//P36,P37   SIN5_2, SOT5_2
		bFM3_GPIO_PFR3_P7 = 1;
        bFM3_GPIO_PCR3_P6 = 1;                  //������ģ��������������
		bFM3_GPIO_EPFR08_SOT5B1 = 1;
		bFM3_GPIO_EPFR08_SOT5B0 = 1;
		bFM3_GPIO_EPFR08_SIN5S1 = 1;
		bFM3_GPIO_EPFR08_SIN5S0 = 1;
	}
	else if( port == COMM5 )                	//
	{
		bFM3_GPIO_PFR5_P3 = 1;           		//P53,P54   SIN6_0, SOT6_0
		bFM3_GPIO_PFR5_P4 = 1;
		bFM3_GPIO_EPFR08_SOT6B1 = 0;
		bFM3_GPIO_EPFR08_SOT6B0 = 1;
		bFM3_GPIO_EPFR08_SIN6S1 = 0;
		bFM3_GPIO_EPFR08_SIN6S0 = 1;
	}
	else if( port == COMM6 )                	//4851
	{
		bFM3_GPIO_PFR4_PD = 1;           		//P4D,P4E   SIN7_1, SOT7_1
		bFM3_GPIO_PFR4_PE = 1;
		bFM3_GPIO_EPFR08_SOT7B1 = 1;
		bFM3_GPIO_EPFR08_SOT7B0 = 0;
		bFM3_GPIO_EPFR08_SIN7S1 = 1;
		bFM3_GPIO_EPFR08_SIN7S0 = 0;
	}
    else if( port == COMM7 )                	//232�����ӵ�����
	{
		bFM3_GPIO_PFR1_P4 = 1;           		//P14,P15  SIN0_1, SOT0_1
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
    OS_CPU_SR  				cpu_sr 				= 0;
#endif
    pCOMM_CFG_STRUCT		pCommCfgStruct 	= NULL;
	INT8U 					err					= 0;
	COMM_RING_BUF*			pbuf				= NULL;
	INT32U 					iTotal				= 0;
	INT8U					MFS_Ch				= 0;
	MFS_UART0ISRCallbackT  	tUARTIrqHandle;
	IRQn_Type				nIrq;
	
	
	if ((port == 0) || (port > UART_TOTAL_COUNTS))
	{
		return UART_ERR_CHANNEL;
	}
	pCommCfgStruct = (pCOMM_CFG_STRUCT)(&g_sCommCfgStruct[port - 1]);
	MFS_Ch 	= (INT8U)(pCommCfgStruct->nMsCh);
	nIrq   	= pCommCfgStruct->nIrq;
	pbuf 	= pCommCfgStruct->pCommBuf;			        /* Obtain pointer to communications channel */
	
	iTotal = OS_TICKS_PER_SEC * timeout / 1000;	//�����TICKS
    OSSemPend( pbuf->UsedSem, iTotal, &err );	//�õ��źŵ�
    if ( err != OS_NO_ERR )
    {
      return UART_ERR_TIMEOUT;	//��ʱ�ˣ������޷��õ��źŵ�
	}
	if ( baudRate > UART_MAX_BAUD )
	{
		return UART_ERR_BAUD;
	}
	if ( parity > EVEN_PARITY )
	{
		return UART_ERR_PARITY;
	}
	if ( (stopBits != ONE_STOP_BIT) && (stopBits != TWO_STOP_BIT) )
	{
		return UART_ERR_STOPBIT;
	}
	if ( (charLen != SIX_CHAR_BIT) && (charLen != SEVEN_CHAR_BIT) && (charLen != EIGHT_CHAR_BIT) )
	{
		return UART_ERR_CHARLEN;
	}
		
    OS_ENTER_CRITICAL();
    	
	UARTDevSet( port, baudRate, parity, stopBits, charLen );
	
	tUARTIrqHandle = pCommCfgStruct->tUARTIrqHandle;				//���÷��͡������жϴ�����
    MFS_UART0ConfigISRCallback(MFS_Ch, &tUARTIrqHandle); 
    
    if (pCommCfgStruct->nFiFoEn)
    {
    	MFS_UART0ConfigFIFO(MFS_Ch, &tMFS_UARTFIFOConfigT);    //ʹ��UART��FIFO���ܣ�16�ֽ� 
    }
    
    MFS_UART0RXEnable(MFS_Ch);                             		//ʹ�ܽ��չ���
    MFS_UART0TXEnable(MFS_Ch);                             		//ʹ�ܷ��͹���
    MFS_UART0IntRXEnable(MFS_Ch);                          		//ʹ�ܽ����ж�
    NVIC_EnableIRQ((IRQn_Type)(nIrq));               				//ʹ�ܽ����ж����ȼ�
    
	OS_EXIT_CRITICAL();
	return UART_OK;
}



/************************************************************************
 *   ���пڲ�������
 *
 *   INT8U UARTDevSet(INT8U ch, INT16U baudRate, INT8U parity,
 *                    INT8U stopBits, INT8U charLen)
 ************************************************************************/
INT8U UARTDevSet(INT8U port, INT32U baudRate, INT8U parity, INT8U stopBits, INT8U charLen )
{
    MFS_UART0ModeConfigT 	UART0ModeConfigT;
    pCOMM_CFG_STRUCT		pCommCfgStruct 	= NULL;
    INT8U 					MFS_Ch 				= 0;
	
	if ((port == 0) || (port > UART_TOTAL_COUNTS))
	{
		return 0;
	}
	pCommCfgStruct 	= (pCOMM_CFG_STRUCT)(&g_sCommCfgStruct[port - 1]);
	MFS_Ch 			= (INT8U)pCommCfgStruct->nMsCh;
	
    if ( baudRate == 0 ) 
	{
		baudRate = 1200 ;
    }
    UART0ModeConfigT.Baudrate = baudRate;
    UART0ModeConfigT.Databits = charLen;
    UART0ModeConfigT.Stopbits = stopBits;
    UART0ModeConfigT.Parity   = parity;
    UART0ModeConfigT.Bitorder = Uart0_BITORDER_LSB;
    UART0ModeConfigT.NRZ      = Uart0_NRZ;
    
    MFS_UART0ConfigMode(MFS_Ch, &UART0ModeConfigT); 
    return UART_OK;
}

/************************************************************************
 *   ����/����ʹ�ܿ���
 *
 *   void UARTDevControl( INT8U ch, INT16U enable)
 *
 ************************************************************************/
void UARTDevControl( INT8U port, INT16U enable)
{
	pCOMM_CFG_STRUCT		pCommCfgStruct 	= NULL;
    INT8U 					MFS_Ch 				= 0;

	if ((port == 0) || (port > UART_TOTAL_COUNTS))
	{
		return;
	}
    pCommCfgStruct = (pCOMM_CFG_STRUCT)(&g_sCommCfgStruct[port - 1]);  
	MFS_Ch 	= (INT8U)(pCommCfgStruct->nMsCh);
    
	if ( (enable & UART_RX_ENABLE) == UART_RX_ENABLE )
	{
        MFS_UART0RXEnable(MFS_Ch);
	}
    else if ( (enable & UART_RX_DISABLE) == UART_RX_DISABLE )
    {
        MFS_UART0RXDisable(MFS_Ch);
    }
    else if ( (enable & UART_TX_ENABLE) == UART_TX_ENABLE )
    {
        MFS_UART0TXEnable(MFS_Ch);
    }
    else if ( (enable & UART_TX_DISABLE) == UART_TX_DISABLE )
    {
        MFS_UART0TXDisable(MFS_Ch);   
    }
}









/****************************************************************
 * �����жϴ����� 
****************************************************************/
void IrqRxHandle_UART(INT8U port)                 
{
#if OS_CRITICAL_METHOD == 3                      	/* Allocate storage for CPU status register           */
    OS_CPU_SR  				cpu_sr 				= 0;
#endif
	pCOMM_CFG_STRUCT		pCommCfgStruct 		= NULL;
	INT16U 					Fbytes 				= 1;
	INT8U 					ssr					= 0;
	INT8U					i					= 0;
    COMM_RING_BUF*			pbuf				= NULL;
    FM3_MFS03_UART_TypeDef* pSsr1 				= NULL;
    FM3_MFS47_UART_TypeDef* pSsr2 				= NULL;
    INT8U					nMsCh				= 0;
    OS_SEM_DATA 			semData;

	if ((port == 0) || (port > UART_TOTAL_COUNTS))
	{
		return;
	}
    pCommCfgStruct = (pCOMM_CFG_STRUCT)(&g_sCommCfgStruct[port - 1]);
    pbuf 	= pCommCfgStruct->pCommBuf;			        /* Obtain pointer to communications channel */
	pSsr1 	= (FM3_MFS03_UART_TypeDef*)(pCommCfgStruct->pFm3);
    pSsr2 	= (FM3_MFS47_UART_TypeDef*)(pCommCfgStruct->pFm3);
	nMsCh 	= (INT8U)(pCommCfgStruct->nMsCh);
	
    if (pCommCfgStruct->nFiFoEn)
    {
	    ssr = pSsr2->SSR;
    }
    else
    {
        ssr = pSsr1->SSR;
    }
    if( ( ssr & (UART0_SSR_PE | UART0_SSR_FRE | UART0_SSR_ORE)) != 0 )      //���մ����ʱ��Ĵ���
    {
        MFS_UART0ErrorClr(nMsCh);
        MFS_UART0RXOneData(nMsCh);
        if (pCommCfgStruct->nFiFoEn)
        {
        	MFS_UART0ConfigFIFO(nMsCh, &tMFS_UARTFIFOConfigT);    //ʹ��UART��FIFO���ܣ�16�ֽ�   
        }    
        return;
    }
    
    OS_ENTER_CRITICAL();	
	if( ssr & UART0_SSR_RDRF)
	{
		if (pCommCfgStruct->nFiFoEn)
		{
			Fbytes = pSsr2->FBYTE2;
		}
		for (i = 0; i < Fbytes; i++)
		{
			*pbuf->RingBufRxInPtr = MFS_UART0RXOneData(nMsCh);
			//������ڻ������Ѿ����ˣ������ʼ�����ݣ������½��յ�����
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
		}

		OSSemQuery( pbuf->RingBufRxSem, &semData );
		if( semData.OSCnt == 0 )
		{
			OSSemPost( pbuf->RingBufRxSem );
		}
	}
    OS_EXIT_CRITICAL();
}

void IrqTxHandle_UART1(void)                 //COMM1 Tx�жϴ������
{
}
void IrqRxHandle_UART1(void)                 //COMM1 Rx�жϴ������
{
	IrqRxHandle_UART(COMM1);
}
void IrqTxHandle_UART2(void)                 //COMM2 Tx�жϴ������
{
}
void IrqRxHandle_UART2(void)                 //COMM2 Rx�жϴ������
{
	IrqRxHandle_UART(COMM2);
}

void IrqTxHandle_UART3(void)                 //COMM3 Tx�жϴ������
{
}
void IrqRxHandle_UART3(void)                 //COMM3 Rx�жϴ������
{
	IrqRxHandle_UART(COMM3);
}

void IrqTxHandle_UART4(void)                 //COMM4 Tx�жϴ������
{
}
void IrqRxHandle_UART4(void)                 //COMM4 Rx�жϴ������
{
	IrqRxHandle_UART(COMM4);
}
void IrqTxHandle_UART5(void)                 //COMM5 Tx�жϴ������
{
}
void IrqRxHandle_UART5(void)                 //COMM5 Rx�жϴ������
{
	IrqRxHandle_UART(COMM5);
}

void IrqTxHandle_UART6(void)                 //COMM6 Tx�жϴ������
{ 
}

void IrqRxHandle_UART6(void)                 //COMM6 Rx�жϴ������
{
	IrqRxHandle_UART(COMM6);
}

void IrqTxHandle_UART7(void)                 //COMM7 Tx�жϴ������
{ 
}

void IrqRxHandle_UART7(void)                 //COMM7 Rx�жϴ������
{
	IrqRxHandle_UART(COMM7);
}





