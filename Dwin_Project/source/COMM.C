/***************************************************************
****************************************************************/
#include "config.h"
#include "global.h"
#include "COMM.h"
#include "UART.h"
#include "AppCfg.h"

COMM_RING_BUF  Comm1Buf;            //MFS_Ch1   DMA CH1
COMM_RING_BUF  Comm2Buf;            //MFS_Ch2   DMA CH2
COMM_RING_BUF  Comm3Buf;            //MFS_Ch4   DMA CH4
COMM_RING_BUF  Comm4Buf;            //MFS_Ch5   DMA CH5
COMM_RING_BUF  Comm5Buf;            //MFS_Ch6   DMA CH6
COMM_RING_BUF  Comm6Buf;            //MFS_Ch7   DMA CH7
COMM_RING_BUF  Comm7Buf;            //MFS_Ch0   DMA CH0


INT8U COM1_RX_BUF[COM1_RX_BUF_SIZE+1];
INT8U COM2_RX_BUF[COM2_RX_BUF_SIZE+1];
INT8U COM3_RX_BUF[COM3_RX_BUF_SIZE+1];
INT8U COM4_RX_BUF[COM4_RX_BUF_SIZE+1];
INT8U COM5_RX_BUF[COM5_RX_BUF_SIZE+1];
INT8U COM6_RX_BUF[COM6_RX_BUF_SIZE+1];
INT8U COM7_RX_BUF[COM7_RX_BUF_SIZE+1];


const COMM_CFG_STRUCT    g_sCommCfgStruct[] = {
	{COMM1,	&Comm1Buf, COM1_RX_BUF, sizeof(COM1_RX_BUF), {IrqRxHandle_UART1, IrqTxHandle_UART1}, ch1_Tx, MFS_Ch1, MFS1RX_IRQn, 0, FM3_MFS1_UART},
	{COMM2,	&Comm2Buf, COM2_RX_BUF, sizeof(COM2_RX_BUF), {IrqRxHandle_UART2, IrqTxHandle_UART2}, ch2_Tx, MFS_Ch2, MFS2RX_IRQn, 0, FM3_MFS2_UART},
	{COMM3,	&Comm3Buf, COM3_RX_BUF, sizeof(COM3_RX_BUF), {IrqRxHandle_UART3, IrqTxHandle_UART3}, ch4_Tx, MFS_Ch4, MFS4RX_IRQn, 1, FM3_MFS4_UART},
	{COMM4,	&Comm4Buf, COM4_RX_BUF, sizeof(COM4_RX_BUF), {IrqRxHandle_UART4, IrqTxHandle_UART4}, ch5_Tx, MFS_Ch5, MFS5RX_IRQn, 1, FM3_MFS5_UART},
	{COMM5,	&Comm5Buf, COM5_RX_BUF, sizeof(COM5_RX_BUF), {IrqRxHandle_UART5, IrqTxHandle_UART5}, ch6_Tx, MFS_Ch6, MFS6RX_IRQn, 1, FM3_MFS6_UART},
	{COMM6,	&Comm6Buf, COM6_RX_BUF, sizeof(COM6_RX_BUF), {IrqRxHandle_UART6, IrqTxHandle_UART6}, ch7_Tx, MFS_Ch7, MFS7RX_IRQn, 1, FM3_MFS7_UART},
    {COMM7,	&Comm7Buf, COM7_RX_BUF, sizeof(COM7_RX_BUF), {IrqRxHandle_UART7, IrqTxHandle_UART7}, ch0_Tx, MFS_Ch0, MFS0RX_IRQn, 0, FM3_MFS0_UART},

};


/* Get comm buffer with port  */
COMM_RING_BUF* GetCommBuf( INT8U port )
{
	COMM_RING_BUF* 		pbuf 				= NULL;
	pCOMM_CFG_STRUCT	pCommCfgStruct		= NULL;

	if ((port == 0) || (port > UART_TOTAL_COUNTS))
	{
		return NULL;
	}
	pCommCfgStruct = (pCOMM_CFG_STRUCT)(&g_sCommCfgStruct[port - 1]);
    pbuf = pCommCfgStruct->pCommBuf;
    return pbuf;
}

/*
*********************************************************************************************************
*                                    REMOVE CHARACTER FROM RING BUFFER
*
*
* Description : This function is called by your application to obtain a character from the communications
*               channel.  The function will wait for a character to be received on the serial channel or
*               until the function times out.
* Arguments   : 'ch'    is the COMM port channel number and can either be:
*                           COMM1
*                           COMM2
*               'to'    is the amount of time (in clock ticks) that the calling function is willing to
*                       wait for a character to arrive.  If you specify a timeout of 0, the function will
*                       wait forever for a character to arrive.
*               'err'   is a pointer to where an error code will be placed:
*                           *err is set to COMM_NO_ERR     if a character has been received
*                           *err is set to COMM_RX_TIMEOUT if a timeout occurred
*                           *err is set to COMM_BAD_CH     if you specify an invalid channel number
* Returns     : The character in the buffer (or NUL if a timeout occurred)
*********************************************************************************************************
*/

INT8U  CommGetChar (INT8U port, INT32U to, INT8U *err)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  			cpu_sr 				= 0;
#endif
	pCOMM_CFG_STRUCT	pCommCfgStruct		= NULL;
    INT8U          		c					= 0;
    INT8U          		oserr				= 0;
    COMM_RING_BUF*		pbuf				= NULL;
    OS_SEM_DATA    		semData;
	
	if ((port == 0) || (port > UART_TOTAL_COUNTS))
	{
		return NULL;
	}
	pCommCfgStruct = (pCOMM_CFG_STRUCT)(&g_sCommCfgStruct[port - 1]);
    pbuf = pCommCfgStruct->pCommBuf;
    if( pbuf == NULL )
    {
		return (UART_ERR_CHANNEL);
    }
    OSSemPend( pbuf->RingBufRxSem, to, &oserr);  	/* Wait for character to arrive             */
    if (oserr == OS_TIMEOUT) 			    		/* See if characters received within timeout*/
    {
        *err = UART_ERR_TIMEOUT;                    /* No, return error code                    */
		return (NULL);
	}
    else
    {
    	if (pbuf->RingBufRxCtr > 0)
    	{
        	OS_ENTER_CRITICAL();
        	pbuf->RingBufRxCtr--;
        	c = *pbuf->RingBufRxOutPtr++;
			if ((INT32U)(pbuf->RingBufRxOutPtr) == (INT32U)(&(pCommCfgStruct->pRxBuf[pCommCfgStruct->nBufSize - 1])))
			{
				pbuf->RingBufRxOutPtr = pCommCfgStruct->pRxBuf;
			}

        	OS_EXIT_CRITICAL();
        	*err = UART_OK;
        	if (pbuf->RingBufRxCtr > 0)
        	{
        		OSSemQuery(pbuf->RingBufRxSem,&semData);
				if (semData.OSCnt == 0)
				{
					OSSemPost(pbuf->RingBufRxSem);
				}
        	}
        	return (c);
        }
        else
        {
        	*err = UART_ERR_TIMEOUT;
        	return (NULL);
        }
    }
}

INT32S ReadCommMs(INT8U port,INT8U *Dest,INT32U iSize,INT32U iInterval, INT32U ms)
{
	INT8U 	ErrInfo;
	INT8U 	Rec;
	INT32U 	iRet=0;
	//INT32U     state;
	INT32U     i,count;
	INT32U     iInter;
	
	iInter = iInterval * OS_TICKS_PER_SEC;
	iInter = iInter / 1000;

	if (0 == iSize)
	{
		return 0;
	}

	if (ms < 10)
	{
		ms = 10;
	}

	count = ms * OS_TICKS_PER_SEC / 1000;
	
	for (i = 0; i < count; i++)
	{
		Rec = CommGetChar( port, 1, &ErrInfo );    //5ms = once
		UART_WATCHDOG_CLEAR();
		if( ErrInfo != UART_ERR_TIMEOUT )
			break;
	}
	
	if ( ErrInfo == UART_ERR_TIMEOUT )
		return 0;
	else
	{
        count = (INT32U)iInter;
        Dest[iRet++] = Rec;
        if( iRet == iSize )
        {
            return (INT32S)iRet;
        }
        for(;;)
        {
            Rec = CommGetChar( port, count, &ErrInfo );
            
            UART_WATCHDOG_CLEAR();
            
            if( ErrInfo == UART_OK )
            {
                Dest[iRet++] = Rec;
                if( iRet == iSize )
                {
                    return (INT32S)iRet;
                }
                continue;
            }
            else
            {
                Dest[iRet] = 0;
                return (INT32S)iRet;
            }
        }
	}
}




INT8U  CommPutStr (INT8U port, INT8U* data, INT32U dataLen , INT32U to)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR 		 	cpu_sr 				= 0;
#endif
	pCOMM_CFG_STRUCT	pCommCfgStruct		= NULL;
    INT32U 				Des					= 0;
    INT8U          		oserr				= 0;
    DMA_MFSTrig 		DMA_MFS_Ch;
    INT8U 				MFS_Ch_Num			= 0;
	INT8U				DMA_CH				= port;
    COMM_RING_BUF*		pbuf				= NULL;

	if ((port == 0) || (port > UART_TOTAL_COUNTS))
	{
		return NULL;
	}
	pCommCfgStruct = (pCOMM_CFG_STRUCT)(&g_sCommCfgStruct[port - 1]);
    pbuf = pCommCfgStruct->pCommBuf;
	DMA_MFS_Ch = pCommCfgStruct->nDmaMfsCh;
	MFS_Ch_Num = (INT8U)(pCommCfgStruct->nMsCh);

    OS_ENTER_CRITICAL();
    
    DMA_IrqCallback[1] = DMAIrqCallbackCh1;	
    DMA_IrqCallback[2] = DMAIrqCallbackCh2;	
    DMA_IrqCallback[3] = DMAIrqCallbackCh3;	
    DMA_IrqCallback[4] = DMAIrqCallbackCh4;	
    DMA_IrqCallback[5] = DMAIrqCallbackCh5;	
    DMA_IrqCallback[6] = DMAIrqCallbackCh6;	
	DMA_IrqCallback[7] = DMAIrqCallbackCh7;
    
    pbuf->SendSize = dataLen;
    pbuf->pSendBuf = data;

    DMA_MFS( DMA_MFS_Ch, DMA_CH, 0, (uint32_t)pbuf->pSendBuf,(uint32_t)&Des, (pbuf->SendSize - 1));
    MFS_UART0IntTXEnable(MFS_Ch_Num);                   /* Enable the TX int */

    OS_EXIT_CRITICAL();
    OSSemPend( pbuf->RingBufTxSem, to, &oserr );         /* Wait for space in Tx buffer              */

    if( oserr == OS_TIMEOUT )
        return (UART_ERR_TIMEOUT);                          /* Timed out, return error code */ 
    else
    	return (UART_OK);
}


/****************************************************************
 * 写数据到串行通讯口
 ****************************************************************/
INT32S WriteComm( INT8U port, INT8U *buffer, INT32U bufsize )
{
	INT8U	i;
	INT32S  ret;
	
	if( bufsize == 0 )
		return 0;
	if (port == COMM1)
	{
		//MFS_UART0RXDisable(MFS_Ch1);      //ESAM    发送数据的时候关闭接收，发完再开启接收  
	}
	else if (port == COMM2)
	{
		//#ifndef TEST_IR_TXRX             	//红外在测试的时候是自发自收
        //MFS_UART0RXDisable(MFS_Ch2);      //红外  发送数据的时候关闭接收，发完再开启接收  
        //#endif        
        IrCommPwmOut( ENABLE );
	}
	else if((port == COMM6))
	{
        OSTimeDlyHMSM(0, 0, 0 , 25);
		GpioSetOutput(IO_PORT4, IO_PINxC);	//初始为高电平状态
        OSTimeDlyHMSM(0, 0, 0 , 10);
        
	}
		
	ret = CommPutStr( port, buffer, bufsize, OS_TICKS_PER_SEC*3 );
    if(port == COMM1)       				//ESAM  在发送完成后开启接收
    {
        /*while(MFS_UART0StatusGetTXBusIdle(MFS_Ch1) != SET)
		{
			OSTimeDlyHMSM(0, 0, 0, 10); 	//TX发送完成   才开始接收数据
			i++;
			if(i==10)
				return 0;
		}
        MFS_UART0RXEnable(MFS_Ch1); */                              
    }
    else if(port == COMM2)      			//红外 在发送完成后开启接收
    {
        OSTimeDlyHMSM(0, 0, 0, 30);
        IrCommPwmOut( DISABLE );
        
        //#ifndef TEST_IR_TXRX   
        //MFS_UART0RXEnable(MFS_Ch2);
        //#endif       
    }
	else if((port == COMM6))				//485
	{
        OSTimeDlyHMSM(0, 0, 0 , 20);
		GpioClearOutput(IO_PORT4, IO_PINxC);	
        OSTimeDlyHMSM(0, 0, 0 , 10);
	}

	if( ret == UART_OK )
		return (INT32S)bufsize;
	else
		return 0;
}

/*
*********************************************************************************************************
*                                  INITIALIZE COMMUNICATIONS MODULE
*
*
* Description : This function is called by your application to initialize the communications module.  You
*               must call this function before calling any other functions.
* Arguments   : none
*********************************************************************************************************
*/

void CommInit( INT8U port )
{
	pCOMM_CFG_STRUCT	pCommCfgStruct		= NULL;
    COMM_RING_BUF*		pbuf				= NULL;
	
	if ((port == 0) || (port > UART_TOTAL_COUNTS))
	{
		return;
	}
	UARTInit( port );
	
	pCommCfgStruct = (pCOMM_CFG_STRUCT)(&g_sCommCfgStruct[port - 1]);
    pbuf = pCommCfgStruct->pCommBuf;
	pbuf->RingBufRx		  = pCommCfgStruct->pRxBuf;	
	pbuf->nRxBufSize	  = pCommCfgStruct->nBufSize - 1;
    if( pbuf != NULL )
    {
	    pbuf->UsedSem         = OSSemCreate(1);
	    pbuf->RingBufRxCtr    = 0;
	    pbuf->RingBufRxInPtr  = pbuf->RingBufRx;
	    pbuf->RingBufRxOutPtr = pbuf->RingBufRx;
	    pbuf->RingBufRxSem    = OSSemCreate(0);
	    pbuf->SendSize    	  = 0;
	    pbuf->pSendBuf        = NULL;
	    pbuf->RingBufTxSem    = OSSemCreate(0);
    }
		
}

INT8S OpenComm( INT8U port, INT32U baudRate, INT8U parity, INT8U stopBits, INT8U charLen )
{
	if ( port > UART_TOTAL_COUNTS )
		return ((INT8S)COMM_NOT_EXIST);
		     	
	if ( UARTConfigure(port,baudRate,parity, stopBits, charLen,0) != UART_OK )
		return ((INT8S)COMM_OPEN_FAIL);
	else	
        return COMM_SUCC;
}

/****************************************************************
 * 关闭串行通讯口
 ****************************************************************/
INT8S CloseComm( INT8U port )
{
	COMM_RING_BUF *pbuf;
	OS_SEM_DATA  semData;
	
	if( port > UART_TOTAL_COUNTS )
		return( (INT8S)COMM_NOT_EXIST );
	
	pbuf = GetCommBuf( port );
	if( pbuf == NULL )
		return (COMM_NOT_EXIST);
	
	OSSemQuery(pbuf->UsedSem,&semData);
	if(semData.OSCnt==0)
		OSSemPost(pbuf->UsedSem);

	return COMM_SUCC;
}


/*
*********************************************************************************************************
*                                   SEE IF RX CHARACTER BUFFER IS EMPTY
*
*
* Description : This function is called by your application to see if any character is available from the
*               communications channel.  If at least one character is available, the function returns
*               FALSE otherwise, the function returns TRUE.
* Arguments   : 'ch'    is the COMM port channel number and can either be:
*                           COMM1
*                           COMM2
* Returns     : TRUE    if the buffer IS empty.
*               FALSE   if the buffer IS NOT empty or you have specified an incorrect channel.
*********************************************************************************************************
*/

INT32S  CommIsEmpty( INT8U port )
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr = 0;
#endif
    BOOLEAN        empty;
    COMM_RING_BUF *pbuf;


	pbuf = GetCommBuf( port );
	if( pbuf == NULL )
		return (COMM_NOT_EXIST);
	
    OS_ENTER_CRITICAL();
    if (pbuf->RingBufRxCtr > 0) {                          /* See if buffer is empty                   */
        empty = FALSE;                                     /* Buffer is NOT empty                      */
    } else {
        empty = TRUE;                                      /* Buffer is empty                          */
    }
    OS_EXIT_CRITICAL();
    return (empty);
}

INT32U GetCommBufSize(INT8U port)
{
	pCOMM_CFG_STRUCT	pCommCfgStruct		= NULL;
    INT32U 				bufsize 			= 0;
	
	if ((port == 0) || (port > UART_TOTAL_COUNTS))
	{
		return NULL;
	}
	pCommCfgStruct = (pCOMM_CFG_STRUCT)(&g_sCommCfgStruct[port - 1]);
	bufsize = pCommCfgStruct->nBufSize - 1;
	return bufsize;
}

INT32S CommFull( INT8U port )
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr = 0;
#endif
	COMM_RING_BUF *pbuf;
	INT32U bufsize;
	
	pbuf = GetCommBuf( port );
	if( pbuf == NULL )
		return (UART_ERR_CHANNEL);
	
    OS_ENTER_CRITICAL();
	
	bufsize = GetCommBufSize(port);
	
    if( (pbuf->RingBufRxOutPtr == pbuf->RingBufRxInPtr + 1) ||
        (pbuf->RingBufRxInPtr - pbuf->RingBufRxOutPtr + 1 == bufsize) )
    {
    	OS_EXIT_CRITICAL();
    	return TRUE;
    }    	
    OS_EXIT_CRITICAL();	
	return FALSE;
}

void CommFlush( INT8U port )
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr = 0;
#endif
	COMM_RING_BUF *pbuf;
	
	pbuf = GetCommBuf( port );
	if( pbuf == NULL )
             return ;

    OS_ENTER_CRITICAL();
    pbuf->RingBufRxCtr    = 0;
	pbuf->RingBufRxInPtr  = pbuf->RingBufRx;
	pbuf->RingBufRxOutPtr = pbuf->RingBufRx;
    OS_EXIT_CRITICAL();
}

INT32S CommPending( INT8U port )
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr = 0;
#endif
	INT16U  len;
	COMM_RING_BUF *pbuf;
	
	pbuf = GetCommBuf( port );
	if( pbuf == NULL )
		return (COMM_NOT_EXIST);

    OS_ENTER_CRITICAL();
    len = pbuf->RingBufRxCtr ;
    OS_EXIT_CRITICAL();
    return ((INT32S)len);
}
/****************************************************************
 * 使能红外接收
 ****************************************************************/
void EnableIRRx( void )
{
	
}

/****************************************************************
 * 禁止红外接收
 ****************************************************************/
void DisableIRRx( void )
{
	
}

/****************************************************************
 * 激活485接收
 ****************************************************************/
void Enable485Rx( INT8U port )      //硬件自动控制485芯片
{
    
}

/****************************************************************
 * 激活485发送
 ****************************************************************/
void Enable485Tx( INT8U port )      //硬件自动控制
{
    
}


/****************************************************************
 * 使能接收中断
 ****************************************************************/
void EnableCommRx( INT8U port )
{
	UARTDevControl( port, UART_RX_ENABLE );
}

/****************************************************************
 * 屏蔽接收中断
 ****************************************************************/
void DisableCommRx( INT8U port )
{
	UARTDevControl( port, UART_RX_DISABLE );
}


/****************************************************************
 * 使能发送中断
 ****************************************************************/
void EnableCommTx( INT8U port )
{
	UARTDevControl( port, UART_TX_ENABLE );
}

/****************************************************************
 * 屏蔽发送中断
 ****************************************************************/
void DisableCommTx( INT8U port )
{
	UARTDevControl( port, UART_TX_DISABLE );
}


/********************************************************************************
* 函数名: DAMIrqHandle
* 输　入:port ：COMM1、COMM2...COMM6
         MFS_Ch_Num ：MFS_Ch0、MFS_Ch1、MFS_Ch2、MFS_Ch3、MFS_Ch5、MFS_Ch7
         IntType： 中断类型
*
* 输　出:
* 返  回: 成功:true 失败:false
* 功  能: DMA中断处理函数
********************************************************************************/
INT32S DAMIrqHandle(INT8U port, INT8U MFS_Ch_Num, INT8U IntType)
{
    uint8_t type;
    type=IntType;
    COMM_RING_BUF* pbuf;
	OS_SEM_DATA semData;
    pbuf = GetCommBuf( port );
	if( pbuf == NULL )
		return (COMM_NOT_EXIST);


    switch (type) 
    {
        case ErrAddOverflow:
            break;
        case StopRqst:
            break;
        case ErrSrsAddErr:
            break;
        case ErrDesAddErr:
            break;
        case Success:
                MFS_UART0IntTXDisable(MFS_Ch_Num);
                OSSemQuery( pbuf->RingBufTxSem, &semData );
                if(semData.OSCnt==0)
                    OSSemPost(pbuf->RingBufTxSem);
            break;
        case Pause:
            break;
        default:
            break;
    }    
    return COMM_SUCC;
}

/****************************************************************
 *DMA中断处理函数       串口发送均使用DMA完成 分别用于COMM1，COMM2，COMM3，COMM4，COMM5，COMM6的发送
 ****************************************************************/
static void DMAIrqCallbackCh1(uint8_t inttype)
{
    DAMIrqHandle(COMM1, MFS_Ch1, inttype); 
}

static void DMAIrqCallbackCh2(uint8_t inttype)
{
    DAMIrqHandle(COMM2, MFS_Ch2, inttype); 
}

static void DMAIrqCallbackCh3(uint8_t inttype)
{
    DAMIrqHandle(COMM3, MFS_Ch4, inttype); 
}
static void DMAIrqCallbackCh4(uint8_t inttype)
{
    DAMIrqHandle(COMM4, MFS_Ch5, inttype); 
}

static void DMAIrqCallbackCh5(uint8_t inttype)
{
	static INT8U i=0;
	i++;
    DAMIrqHandle(COMM5, MFS_Ch6, inttype); 
}

static void DMAIrqCallbackCh6(uint8_t inttype)
{
    DAMIrqHandle(COMM6, MFS_Ch7, inttype);
}

static void DMAIrqCallbackCh7(uint8_t inttype)
{
    DAMIrqHandle(COMM7, MFS_Ch0, inttype);
}

/****************************************************************
 * 产生/关闭红外通讯所需的38KHz载波信号
 ****************************************************************/
void IrCommPwmOut( INT8U bEnable )
{
	
	if( bEnable == ENABLE )
	{// 启动PWM
		// 
        INT16U  nTemp;
        INT16U  nHtemp;
        /* Set Basetimer ch11 IO port */    
        bFM3_GPIO_PFR5_P8 = 0x01;            // Use P58   
        bFM3_GPIO_EPFR12_TIOA11E1 = 0x01;    // Use TIOA11_2  
        bFM3_GPIO_EPFR12_TIOA11E0 = 0x01;  
        
        /* Set IO Mode */
        FM3_BTIOSEL8B->BTSEL89AB = (FM3_BTIOSEL8B->BTSEL89AB&0x0f)|(BT_IO_MODE_0<<4);

        FM3_BT11_PWM->TMCR = 0;
        FM3_BT11_PWM->TMCR |= 0x0010;        //16-bit PWM timer
        FM3_BT11_PWM->TMCR |= 0X0800;
        FM3_BT11_PWM->TMCR2 = 0x00;
        FM3_BT11_PWM->STC = 0;
        nTemp = (APB1CLOCK/PW38KBAUND)-1;  /* Set baud rate */ 
        nHtemp = nTemp/2;
        FM3_BT11_PWM->PCSR = nTemp;
        FM3_BT11_PWM->PDUT = nHtemp;
        FM3_BT11_PWM->TMCR |= 0x0003;
	}
	else
	{// 关闭PWM
        FM3_BT11_PWM->TMCR &= ~0x0003;       //关闭PWM计数器
	}
}

/****************************************************************
 * 产生/关闭ESAM通讯所需的4MHz载波信号
 ****************************************************************/
void ESAMPwmOut( INT8U bEnable )
{
	
	if( bEnable == ENABLE )
	{// 启动PWM
		// 
        INT16U  nTemp;
        INT16U  nHtemp;
        /* Set Basetimer ch11 IO port */    
        bFM3_GPIO_PFR5_P8 = 0x01;            // Use P58   
        bFM3_GPIO_EPFR12_TIOA11E1 = 0x01;    // Use TIOA11_2  
        bFM3_GPIO_EPFR12_TIOA11E0 = 0x01;  
        
        /* Set IO Mode */
        FM3_BTIOSEL8B->BTSEL89AB = (FM3_BTIOSEL8B->BTSEL89AB&0x0f)|(BT_IO_MODE_0<<4);

        FM3_BT11_PWM->TMCR = 0;
        FM3_BT11_PWM->TMCR |= 0x0010;        //16-bit PWM timer
        FM3_BT11_PWM->TMCR |= 0X0800;
        FM3_BT11_PWM->TMCR2 = 0x00;
        FM3_BT11_PWM->STC = 0;
        nTemp = (APB1CLOCK/PW4MBAUND)-1;  /* Set baud rate */ 
        nHtemp = nTemp/2;
        FM3_BT11_PWM->PCSR = nTemp;
        FM3_BT11_PWM->PDUT = nHtemp;
        FM3_BT11_PWM->TMCR |= 0x0003;
	}
	else
	{// 关闭PWM
        FM3_BT11_PWM->TMCR &= ~0x0003;       //关闭PWM计数器
	}
}

