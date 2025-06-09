/***************************************************************
****************************************************************/
#ifndef _GLOBAL_HEADER_
#define _GLOBAL_HEADER_


#include "config.h"
#include "Io_fm3.h"
#include "mfs_fm3.h"
#include "uart_fm3.h"
#include "dmac_fm3.h"
#include "bt_fm3.h"
#include "COMM.h"
#include "UART.h"
#include "BSP.h"
#include "emac_api.h"
#include "usb.h"
#include "UsbInit.h"
#include "bootloader.h"
#include "bootloaderapi.h"
//#include "Test.h"
#include "apierrno.h"
#include "FM24CL16.h"
#include "RN8209.h"
#include "RX8025T.h"
#include "SST25F032.h"
#include "ISO7816.h"
#include "USBApi.h"
#include "zlib.h"
#include "Key.h"
#include "BU97950FUV.h"
#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>
#include  <stdlib.h>
#include  <stdarg.h>
//#include  <lib_def.h>
//#include  <lib_mem.h>
//#include  <lib_str.h>


#define RT_TICK_PER_SECOND		10


//��ʾ������������100KHz����
#define APB2CLOCK             (SystemCoreClock/(1 << (APBC1_PSR_Val & 0x03)))
#define PW50KBAUND              10000


//#define COMM_Test
//#define DMA_ENABLE
//#define DMA_TIMER_DETECT

//Ӧ����ʼ��ַ
#define USER1_FLASH_START_ADDR     0x00080000
#define USER2_FLASH_START_ADDR     0x00000000

//���Ź�����
#define WDG_PFR                bFM3_GPIO_PFR2_P0
#define WDG_DDR                bFM3_GPIO_DDR2_P0
#define WDG_PDOR               bFM3_GPIO_PDOR2_P0
#define CH_DOG_INCHIP			0		/* Ƭ�ڿ��Ź�δʹ�� */
#define CH_DOG_OUT			    1		/* Ƭ�⿴�Ź� */

//ADC����
#define BAT_AD_PFR             bFM3_GPIO_PFR2_P2
#define BAT_AD_ADE             bFM3_GPIO_ADE_AN31
#define BAT_ADC_SCIS           bFM3_ADC0_SCIS3_AN31

#define BATTERY_AD_PFR             bFM3_GPIO_PFR2_P4
#define BATTERY_AD_ADE             bFM3_GPIO_ADE_AN29
#define BATTERY_ADC_SCIS           bFM3_ADC0_SCIS3_AN29

#define RS485B_AD_PFR             bFM3_GPIO_PFR2_P3
#define RS485B_AD_ADE             bFM3_GPIO_ADE_AN30
#define RS485B_ADC_SCIS           bFM3_ADC0_SCIS3_AN30

#define CLOCK_AD_PFR             bFM3_GPIO_PFR2_P5
#define CLOCK_AD_ADE             bFM3_GPIO_ADE_AN28
#define CLOCK_ADC_SCIS           bFM3_ADC0_SCIS3_AN28


//ESAM����
#define ESAM_RST_PFR           bFM3_GPIO_PFR4_P0
#define ESAM_RST_DDR           bFM3_GPIO_DDR4_P0
#define ESAM_RST_PDOR          bFM3_GPIO_PDOR4_P0

// UART ����
#define UART_TOTAL_COUNTS		7

#define COMM1					1
#define COMM2					2
#define COMM3					3
#define COMM4					4
#define COMM5					5
#define COMM6					6
#define COMM7					7




/**************************************************************************************************
                                     ���ھ��嶨��
***************************************************************************************************/
// ASSIST_RUN_TESTSCJ
#define 	COMM_WH4852            			COMM1      //��Ӧ����29��30���� �����
#define 	COMM_IR              			COMM2
#define 	COMM_WH232            		 	COMM3
#define 	COMM_ZB           			    COMM4
#define 	COMM_GPRS        	    		COMM5
#define 	COMM_WH485       	    		COMM6       //��Ӧ����27��28���� ������
#define 	COMM_ERR      	    		    COMM7

// ASSIST_RUN_DWIN
#define 	COMM_WH4852            			COMM1      	//485-2
#define 	COMM_IR              			COMM2		//
#define 	COMM_SMQ            		 	COMM3		//ɨ��ǹ
#define 	COMM_BLE           			    COMM4		//����
#define 	COMM_NULL        	    		COMM5		//
#define 	COMM_WH485       	    		COMM6       //484-1
#define 	COMM_232      	    		    COMM7		//232


// ASSIST_RUN_CARRIER
#define 	COMM_HOST						COMM4				// ��ͨ��
#define 	COMM_CARR_RCV_DEBUG  			COMM5				// �ɼ���
#define 	COMM_CARR_SND_DEBUG 			COMM3				// ������



// ASSIST_RUN_YUNWEISCJ
//#define 	COMM_IR              			COMM2
#define 	COMM_485_1         	    	COMM3				// 485��





//��λԴ
#define RESET_SOURCE_SRST		0x04	// �����λ
#define RESET_SOURCE_FCSR		0x80	// ������Ƶ���쳣��λ
#define RESET_SOURCE_CSVR		0x40	// ��/��������ϸ�λ
#define RESET_SOURCE_HWDT		0x20	// �ڲ�Ӳ�����Ź���λ
#define RESET_SOURCE_SWDT		0x10	// ������Ź���λ
#define RESET_SOURCE_INITX	    0x02	// �ⲿ��λ�Ÿ�λ
#define RESET_SOURCE_PONR		0x01	// �ϵ縴λ

// ������
#define UART_BPS_300			300
#define	UART_BPS_600			600
#define	UART_BPS_900			900
#define UART_BPS_1200			1200
#define	UART_BPS_2400			2400
#define UART_BPS_4800			4800
#define UART_BPS_7200			7200
#define	UART_BPS_9600			9600
#define	UART_BPS_14400			14400
#define UART_BPS_19200			19200
#define UART_BPS_28800			28800
#define	UART_BPS_38400			38400
#define UART_BPS_57600			57600
#define	UART_BPS_115200		    115200
#define UART_MAX_BAUD			UART_BPS_115200

// ��żУ��
#define NO_PARITY       		0
#define ODD_PARITY      		1       //��
#define EVEN_PARITY     		2       //ż
#define MARK_PARITY			    3
#define SPACE_PARITY			4
#define UART_MAX_PARITY		    SPACE_PARITY

// ֹͣλ
#define ONE_STOP_BIT			1
#define TWO_STOP_BIT			2
#define UART_MAX_STOP			TWO_STOP_BIT

// ����λ
#define FIVE_CHAR_BIT			5
#define SIX_CHAR_BIT			6
#define SEVEN_CHAR_BIT			7
#define EIGHT_CHAR_BIT			8
#define UART_MAX_CHAR			EIGHT_CHAR_BIT

// UART ģʽ
#define UART_MODE_RS232		    0
#define UART_MODE_RS485		    1
#define UART_MODE_INFRA		    2
#define UART_MAX_MODE			UART_MODE_INFRA
#define UART_MODE_NORMAL		UART_MODE_RS232


/*---------------------------------------------------------------------------*/
/* Timer brief                                                               */
/*---------------------------------------------------------------------------*/

/*! \brief User BT channel */
#define USER_BT_CH         	(BT_CH_0)
/*! \brief User timer channel */
#define USER_TIMER_MODE    	(BT_32BIT_TIMER)

#define GPIO_MODE_INPUT		IO_DIR_INPUT
#define GPIO_MODE_OUTPUT  	IO_DIR_OUTPUT

/* ===============================================================================
                   �ײ������ӿ�
 ===============================================================================*/
INT8U* GetApiVersion( void );		//��ȡ�汾��
void SelAppAddr(INT32U addr);
/*  SPI Flash�����ӿڼ�SST25F032.h  */

/*  ���������ӿڼ�FM24CL16.h    */

/* ===============================================================================
                   ���Ź������ӿ�
 ===============================================================================*/
void WatchDogEnable( INT8U index );
void WatchDogDisable( INT8U index );
void WatchDogClear( INT8U index);

/* ===============================================================================
                   ��ʱ�������ӿ�
 ===============================================================================*/
void Timer3Init( void );
void Timer3Handler(void);            
void TimerStart( INT32U time );
INT32U TimerStop( void );

/*  ��̫�������ӿڼ�emac_api.h  */

/*  USB HOST�����ӿڼ�USBAPI.h  */

/*  UART�����ӿڼ�COMM.h    */
INT8U GetResetSrc(void);

/* ===============================================================================
                   ADת�������ӿ�
 ===============================================================================*/
void Init_485_ADC(INT8U n);
INT32U Get485Voltage( INT8U n );

INT32U GetAD0Voltage( void );
CPU_INT16U  BSP_ADC_GetStatus (CPU_INT08U  adc);
/* ===============================================================================
                   GPIO�����ӿ�
 ===============================================================================*/
void WriteCpuIoStatus( INT8U port, INT32U pin, INT8U status );
INT8U ReadCpuIoStatus( INT8U port, INT32U pin );
void GpioConfig( INT8U port, INT32U pin, INT8U mode );
void GpioSetOutput( INT8U port, INT32U pin );
void GpioClearOutput( INT8U port, INT32U pin );
INT8U GpioGetInput( INT8U port, INT32U pin );
/* ===============================================================================
                   END
 ===============================================================================*/
void System_Init( void );
INT32U TargetInit(void);

//void Init_ADC(void);
void ADC_init(INT8U num);
INT32U GetBatVbat(void);
INT32U GetVbat(void);
INT32U GetClockVbat();

void Delayus( INT32U dly ); 
void Delayms( INT32U dly );

int TextOut(const char* String);
void Uart_Puts(uint8_t u8Uart, char* String);
void Uart_Putch(uint8_t u8Uart, char c);

INT8U Dec2BCD( INT8U data );
INT8U BCD2Dec( INT8U data );

/* ===============================================================================
                   GPIO�����ӿ���չ����
 ===============================================================================*/
INT8U ReadCpuIoStatusEX( INT8U portpin );
void GpioConfigEX( INT8U portpin, INT8U mode, INT8U pullup ) ;
void GpioClearOutputEX( INT8U portpin );
void GpioSetOutputEX( INT8U portpin );

#endif
