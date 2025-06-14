/***************************************************************
****************************************************************/
#ifndef UART_H
#define UART_H

#include "config.h"
#include "Global.H"

/*
 *  在串口中断中启用/禁止看门狗功能
 *    1 - 启用
 *    0 - 禁止
 */
#define UART_WD_ENABLE				1

#if UART_WD_ENABLE
#define UART_WATCHDOG_CLEAR()		WatchDogClear( 1 )
#else
#define UART_WATCHDOG_CLEAR()
#endif


/*
 * 扩展串口数据发送方式： 0 -  中断常开
 *                        1 -  仅在发送前打开中断
 */
#define UART45_SEND_INT			1

#define  COM1_RX_BUF_SIZE       1024//512     								// 4852
#define  COM2_RX_BUF_SIZE       256                                   		// 
#define  COM3_RX_BUF_SIZE       1024    	                				// 扫码枪
#define  COM4_RX_BUF_SIZE       512    			                            // 蓝牙
#define  COM5_RX_BUF_SIZE       256                                  		// 
#define  COM6_RX_BUF_SIZE       1024     			        				// 485-1
#define  COM7_RX_BUF_SIZE       (1024 * 2 + 100)                            // 232


#define  COMM_RX_BUF_SIZE      512//1024           /* Number of characters in Rx ring buffer             */
#define  COMM_TX_BUF_SIZE      128                /* Number of characters in Tx ring buffer             */


// UART驱动用数据结构
typedef struct {
    OS_EVENT  *UsedSem;
    INT32U     RingBufRxCtr;                /* Number of characters in the Rx ring buffer              */
    OS_EVENT  *RingBufRxSem;                /* Pointer to Rx semaphore                                 */
    INT8U     *RingBufRxInPtr;              /* Pointer to where next character will be inserted        */
    INT8U     *RingBufRxOutPtr;             /* Pointer from where next character will be extracted     */
//	INT8U      RingBufRx[COMM_RX_BUF_SIZE+1]; 
    INT8U     *RingBufRx;					/* Ring buffer character storage (Rx)                      */
    OS_EVENT  *RingBufTxSem;                /* Pointer to Tx semaphore                                 */
    INT32U    SendSize;						/* number of bytes of data to be sent */
    INT8U *   pSendBuf;						/* pointer to buffer for data to be sent */
	
    INT32U     nCurrFreeSpace;
    INT8U      bRxBufFull;      // ring buffer is full or not
    INT32U     nRxBufSize;      // ring buffer size
} COMM_RING_BUF;

#define MAX_SIZE                16
static MFS_UART0FIFOConfigT tMFS_UARTFIFOConfigT =
{
    MFS_FIFO1TX_FIFO2RX,     //发送选FIFO1，接收选FIFO2  
    0,                       //发送FIFO初始值（一般需置0）
    MAX_SIZE,                //接收FIFO的长度
};

typedef struct _tag_COMM_CFG_STRUCT
{
	INT8U					nCom;
	COMM_RING_BUF*			pCommBuf;
	INT8U*					pRxBuf;
	INT16U					nBufSize;
	
	MFS_UART0ISRCallbackT   tUARTIrqHandle;
	DMA_MFSTrig 			nDmaMfsCh;
	MFS_ChannelNumT			nMsCh;
	IRQn_Type				nIrq;

	INT8U					nFiFoEn;
	void*                   pFm3;
	
}COMM_CFG_STRUCT, *pCOMM_CFG_STRUCT;



#define UART_OK				0
#define UART_ERR_CHANNEL	1
#define UART_ERR_TIMEOUT	2
#define UART_ERR_BAUD		3
#define UART_ERR_PARITY		4
#define UART_ERR_STOPBIT	5
#define UART_ERR_CHARLEN	6
#define UART_ERR_RECV		7

#define UART_ENABLE			0x01
#define UART_RX_ENABLE		0x02
#define UART_TX_ENABLE		0x04
#define UART_DISABLE		0x08
#define UART_RX_DISABLE		0x10
#define UART_TX_DISABLE		0x20

#define TX_MAX_FIFO_SIZE	16
#define RX_MAX_FIFO_SIZE	8


void UARTInit( INT8U port );
INT8U UARTConfigure(INT8U ch, INT32U baudRate, INT8U parity, INT8U stopBits, INT8U charLen,INT32U timeout);
INT8U UARTDevSet(INT8U ch, INT32U baudRate, INT8U parity, INT8U stopBits, INT8U charLen);
void UARTDevControl( INT8U port, INT16U enable);






void IrqTxHandle_UART1(void);
void IrqRxHandle_UART1(void);
void IrqTxHandle_UART2(void);
void IrqRxHandle_UART2(void);
void IrqTxHandle_UART3(void);
void IrqRxHandle_UART3(void);
void IrqTxHandle_UART4(void);
void IrqRxHandle_UART4(void);
void IrqTxHandle_UART5(void);
void IrqRxHandle_UART5(void);
void IrqTxHandle_UART6(void);
void IrqRxHandle_UART6(void);
void IrqTxHandle_UART7(void);
void IrqRxHandle_UART7(void);


#endif
