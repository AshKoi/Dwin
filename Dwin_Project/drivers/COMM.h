/***************************************************************
****************************************************************/
#ifndef _COMMRTOS_HEADER_
#define _COMMRTOS_HEADER_

#include "config.h"
#include "Global.H"
#include "uart.h"

#define UART_FIFO_ENABLE	1
#define COMM_SUCC			0
#define COMM_OPEN_FAIL		(-1)	//0xFF	// -1
#define COMM_NOT_EXIST		(-2)	//0xFE	// -2


#define FM3_UART_RCV_NULL    0
#define FM3_UART_RCV_ERR     1
#define FM3_UART_RCV_OK      2

/* Device Defines */
#define UartDev_DATABITS_6                 6
#define UartDev_DATABITS_7                 7
#define UartDev_DATABITS_8                 8
#define UartDev_DATABITS_9                 9
 
#define UartDev_STOPBITS_1                 1
#define UartDev_STOPBITS_2                 2

#define UartDev_PARITY_NONE                0
#define UartDev_PARITY_ODD                 1
#define UartDev_PARITY_EVEN                2

#define UartDev_BITORDER_MSB               1
#define UartDev_BITORDER_LSB               0

#define UartDev_FLAG_BLOCKING              0
#define UartDev_FLAG_NONBLOCKING           1


/*
 *  Register Bit Definition
 */
#define SMR_SOE          0x01U
#define SMR_BDS          0x04U
#define SMR_SBL          0x08U
#define SMR_WUCR         0x10U
#define SMR_MD_UART      0x00U
#define SMR_MD_UART_MP   0x20U
#define SMR_MD_SIO       0x40U
#define SMR_MD_LIN       0x60U
#define SMR_MD_I2C       0x80U

#define SCR_TXE          0x01U
#define SCR_RXE          0x02U
#define SCR_TBIE         0x04U
#define SCR_TIE          0x08U
#define SCR_RIE          0x10U
#define SCR_UPGL         0x80U

#define SSR_TBI          0x01U
#define SSR_TDRE         0x02U
#define SSR_RDRF         0x04U
#define SSR_ORE          0x08U
#define SSR_FRE          0x10U
#define SSR_PE           0x20U
#define SSR_REC          0x80U

#define ESCR_P           0x08U
#define ESCR_PEN         0x10U
#define ESCR_INV         0x20U
#define ESCR_ESBL        0x40U
#define ESCR_FLWEN       0x80U
#define ESCR_DATABITS_8  0x00U
#define ESCR_DATABITS_5  0x01U
#define ESCR_DATABITS_6  0x02U
#define ESCR_DATABITS_7  0x03U
#define ESCR_DATABITS_9  0x04U

#define BGR_EXT          0x8000U

#define FCR1_FSEL        0x01U
#define FCR1_FTIE        0x02U
#define FCR1_FDRQ        0x04U
#define FCR1_FRIIE       0x08U
#define FCR1_FLSTE       0x10U

#define FCR0_FE1         0x01U
#define FCR0_FE2         0x02U
#define FCR0_FCL1        0x04U
#define FCR0_FCL2        0x08U
#define FCR0_FSET        0x10U
#define FCR0_FLD         0x20U
#define FCR0_FLST        0x40U

#define APB1CLOCK             (SystemCoreClock/(1 << (APBC1_PSR_Val & 0x03)))
#define PW38KBAUND              38000
#define PW4MBAUND               3570000


/* ===============================================================================
                UART �����ӿ�
 ===============================================================================*/
/****************************************************************
 * ��ʼ�����п�Ӳ�������ж�����
 ****************************************************************/
void CommInit( INT8U port );


/****************************************************************
 * �򿪴���ͨѶ��
 ****************************************************************/
INT8S  OpenComm( INT8U port,INT32U baudRate,INT8U parity,INT8U stopBits,INT8U charLen );
#define SetComm(a,b,c,d,e) UARTDevSet(a,b,c,d,e)
/****************************************************************
 * �رմ���ͨѶ��
 ****************************************************************/
INT8S CloseComm( INT8U port );

/****************************************************************
 * �Ӵ���ͨѶ�ڶ�����
 ****************************************************************/
INT32S ReadCommMs( INT8U port, INT8U *Dest, INT32U iSize,
                   INT32U iInterval, INT32U ms );
					
/****************************************************************
 * д���ݵ�����ͨѶ��
 ****************************************************************/
INT32S WriteComm( INT8U port, INT8U *buffer, INT32U bufsize );


/****************************************************************
 * ʹ�ܽ����ж�
 ****************************************************************/
void EnableCommRx( INT8U port );

/****************************************************************
 * ���ν����ж�
 ****************************************************************/
void DisableCommRx( INT8U port );


/****************************************************************
 * ʹ�ܷ����ж�
 ****************************************************************/
void EnableCommTx( INT8U port );

/****************************************************************
 * ���η����ж�
 ****************************************************************/
void DisableCommTx( INT8U port );


/****************************************************************
 * ��ս��ջ�������
 ****************************************************************/
void CommFlush( INT8U port );

/****************************************************************
 * �õ����ջ�������δ�����ݵĳ��ȡ�
 ****************************************************************/
INT32S CommPending( INT8U port );

/****************************************************************
 * �жϽ��ջ������Ƿ�������
 ****************************************************************/
INT32S CommFull( INT8U port );


/****************************************************************
 * ����485����
 ****************************************************************/
void Enable485Rx( INT8U port );

/****************************************************************
 * ����485����
 ****************************************************************/
void Enable485Tx( INT8U port );

/****************************************************************
 * ����/�رպ���ͨѶ�����38KHz�ز��ź�
 ****************************************************************/
void IrCommPwmOut( INT8U bEnable );

/* ===============================================================================
                    END
 ===============================================================================*/





/****************************************************************
 * ��UART1��RTS������Ϊ�߼��͵�ƽ���շ���̨����ǰ����ʹ�á�
 ****************************************************************/
void EnableCommRts( void );

/****************************************************************
 * ��UART1��RTS������Ϊ�߼��ߵ�ƽ���շ���̨���ݺ����ʹ�á�
 ****************************************************************/
void DisableCommRts( void );

/****************************************************************
 * ����ͨ��ѡ��(UART1)
 ****************************************************************/
void DownCommSel( INT8U port );

/****************************************************************
 * ����ͨ��ѡ��(UART0)
 ****************************************************************/
void UpCommSel( INT8U port );





/****************************************************************
 * �رպ����ж�����
 *
 ****************************************************************/
void CloseIRCommReq( void );

/****************************************************************
 * ʹ�ܺ������
 ****************************************************************/
void EnableIRRx( void );

/****************************************************************
 * ��ֹ�������
 ****************************************************************/
void DisableIRRx( void );








void ResetComm( INT8U port );

COMM_RING_BUF* GetCommBuf( INT8U port );
INT32U GetCommBufSize(INT8U port);

void ExtCommIntProc( void );

INT8U   CommGetChar( INT8U port, INT32U to, INT8U *err );
INT8U   CommPutStr(  INT8U port, INT8U* data, INT32U dataLen , INT32U to );
INT32S  CommIsEmpty( INT8U port );

static void DMAIrqCallbackCh1(uint8_t inttype);
static void DMAIrqCallbackCh2(uint8_t inttype);
static void DMAIrqCallbackCh3(uint8_t inttype);
static void DMAIrqCallbackCh4(uint8_t inttype);
static void DMAIrqCallbackCh5(uint8_t inttype);
static void DMAIrqCallbackCh6(uint8_t inttype);  
static void DMAIrqCallbackCh7(uint8_t inttype); 


void ESAMPwmOut( INT8U bEnable );
 

////////////////////////////////////////////////////////////////////////////////
#endif
