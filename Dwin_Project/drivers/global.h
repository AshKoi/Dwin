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


//显示口线驱动生成100KHz方波
#define APB2CLOCK             (SystemCoreClock/(1 << (APBC1_PSR_Val & 0x03)))
#define PW50KBAUND              10000


//#define COMM_Test
//#define DMA_ENABLE
//#define DMA_TIMER_DETECT

//应用起始地址
#define USER1_FLASH_START_ADDR     0x00080000
#define USER2_FLASH_START_ADDR     0x00000000

//看门狗配置
#define WDG_PFR                bFM3_GPIO_PFR2_P0
#define WDG_DDR                bFM3_GPIO_DDR2_P0
#define WDG_PDOR               bFM3_GPIO_PDOR2_P0
#define CH_DOG_INCHIP			0		/* 片内看门狗未使用 */
#define CH_DOG_OUT			    1		/* 片外看门狗 */

//ADC配置
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


//ESAM配置
#define ESAM_RST_PFR           bFM3_GPIO_PFR4_P0
#define ESAM_RST_DDR           bFM3_GPIO_DDR4_P0
#define ESAM_RST_PDOR          bFM3_GPIO_PDOR4_P0

// UART 总数
#define UART_TOTAL_COUNTS		7

#define COMM1					1
#define COMM2					2
#define COMM3					3
#define COMM4					4
#define COMM5					5
#define COMM6					6
#define COMM7					7




/**************************************************************************************************
                                     串口具体定义
***************************************************************************************************/
// ASSIST_RUN_TESTSCJ
#define 	COMM_WH4852            			COMM1      //对应板子29、30端子 抄表口
#define 	COMM_IR              			COMM2
#define 	COMM_WH232            		 	COMM3
#define 	COMM_ZB           			    COMM4
#define 	COMM_GPRS        	    		COMM5
#define 	COMM_WH485       	    		COMM6       //对应单板27、28端子 被抄口
#define 	COMM_ERR      	    		    COMM7

// ASSIST_RUN_DWIN
#define 	COMM_WH4852            			COMM1      	//485-2
#define 	COMM_IR              			COMM2		//
#define 	COMM_SMQ            		 	COMM3		//扫码枪
#define 	COMM_BLE           			    COMM4		//蓝牙
#define 	COMM_NULL        	    		COMM5		//
#define 	COMM_WH485       	    		COMM6       //484-1
#define 	COMM_232      	    		    COMM7		//232


// ASSIST_RUN_CARRIER
#define 	COMM_HOST						COMM4				// 主通道
#define 	COMM_CARR_RCV_DEBUG  			COMM5				// 采集器
#define 	COMM_CARR_SND_DEBUG 			COMM3				// 集中器



// ASSIST_RUN_YUNWEISCJ
//#define 	COMM_IR              			COMM2
#define 	COMM_485_1         	    	COMM3				// 485口





//复位源
#define RESET_SOURCE_SRST		0x04	// 软件复位
#define RESET_SOURCE_FCSR		0x80	// 主晶振频率异常复位
#define RESET_SOURCE_CSVR		0x40	// 主/副晶振故障复位
#define RESET_SOURCE_HWDT		0x20	// 内部硬件看门狗复位
#define RESET_SOURCE_SWDT		0x10	// 软件看门狗复位
#define RESET_SOURCE_INITX	    0x02	// 外部复位脚复位
#define RESET_SOURCE_PONR		0x01	// 上电复位

// 波特率
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

// 奇偶校验
#define NO_PARITY       		0
#define ODD_PARITY      		1       //奇
#define EVEN_PARITY     		2       //偶
#define MARK_PARITY			    3
#define SPACE_PARITY			4
#define UART_MAX_PARITY		    SPACE_PARITY

// 停止位
#define ONE_STOP_BIT			1
#define TWO_STOP_BIT			2
#define UART_MAX_STOP			TWO_STOP_BIT

// 数据位
#define FIVE_CHAR_BIT			5
#define SIX_CHAR_BIT			6
#define SEVEN_CHAR_BIT			7
#define EIGHT_CHAR_BIT			8
#define UART_MAX_CHAR			EIGHT_CHAR_BIT

// UART 模式
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
                   底层驱动接口
 ===============================================================================*/
INT8U* GetApiVersion( void );		//获取版本号
void SelAppAddr(INT32U addr);
/*  SPI Flash驱动接口见SST25F032.h  */

/*  铁电驱动接口见FM24CL16.h    */

/* ===============================================================================
                   看门狗驱动接口
 ===============================================================================*/
void WatchDogEnable( INT8U index );
void WatchDogDisable( INT8U index );
void WatchDogClear( INT8U index);

/* ===============================================================================
                   定时器驱动接口
 ===============================================================================*/
void Timer3Init( void );
void Timer3Handler(void);            
void TimerStart( INT32U time );
INT32U TimerStop( void );

/*  以太网驱动接口见emac_api.h  */

/*  USB HOST驱动接口见USBAPI.h  */

/*  UART驱动接口见COMM.h    */
INT8U GetResetSrc(void);

/* ===============================================================================
                   AD转换驱动接口
 ===============================================================================*/
void Init_485_ADC(INT8U n);
INT32U Get485Voltage( INT8U n );

INT32U GetAD0Voltage( void );
CPU_INT16U  BSP_ADC_GetStatus (CPU_INT08U  adc);
/* ===============================================================================
                   GPIO驱动接口
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
                   GPIO驱动接口扩展函数
 ===============================================================================*/
INT8U ReadCpuIoStatusEX( INT8U portpin );
void GpioConfigEX( INT8U portpin, INT8U mode, INT8U pullup ) ;
void GpioClearOutputEX( INT8U portpin );
void GpioSetOutputEX( INT8U portpin );

#endif
