/*
 ******************************************************************************
 **
 **系统共配置了2片串行Flash，地址分配如下：
 *
 *             Chip 1  -  0x0      ~ 0x3FFFFF
 *             Chip 2  -  0x400000 ~ 0x7FFFFF
 *
 *          如果函数参数内包含地址，则通过以上规则计算后选择相应的片选信号；
 *          否则通过参数chip进行片选。 
 ******************************************************************************
 */
#ifndef _SST25F032_H_
#define _SST25F032_H_

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "board.h"

/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/
#define SST25_WRITE_AAI					0

#define SST25BAUDRATE    				2000000//4000000

#define SST25_SPI_FLASH_PAGE_SIZE		0x100
#define SST25_SPI_FLASH_BLOCK_SIZE		0x1000
#define SST25_SPI_FLASH_CHIP_SIZE		0x400000

/* SST Flash 命令表 */
#define SST25_CMD_READ					0x03
#define SST25_CMD_READ_HIGH				0x0B
#define SST25_CMD_WRITE_ENABLE          0x06
#define SST25_CMD_WRITE_DISABLE         0x04
#define SST25_CMD_ERASE_CHIPALL			0xC7
#define SST25_CMD_ERASE_BLOCK			0x20
#define SST25_CMD_ERASE_SECTOR			0x52
#define SST25_CMD_ERASE_TWOSECTOR		0xD8
#define SST25_CMD_STATUS_READ			0x05
#define SST25_CMD_ID_READ				0x90
#define SST25_CMD_JEDEC_ID				0x9F
#define SST25_CMD_PROGRAM_BYTE			0x02
#define SST25_CMD_PROGRAM_AAI			0xAD
#define SST25_CMD_EWSR					0x50
#define SST25_CMD_WRSR					0x01
#define SST25_CMD_EBSY					0x70
#define SST25_CMD_DBSY					0x80

#define SST25_ERASE_TYPE_CHIP			0	// 4MB
#define SST25_ERASE_TYPE_BLOCK			1	// 4kB
#define SST25_ERASE_TYPE_SECTOR			2	// 32KB
#define SST25_ERASE_TYPE_TWOSECTOR	    3	// 64KB

#define SST25_STATUS_BUSY				0x1

#define SST25SCS1EN()                bFM3_GPIO_PDOR0_P9 = 0;
#define SST25SCS1DIS()               bFM3_GPIO_PDOR0_P9 = 1;
#define SST25SCS2EN()                bFM3_GPIO_PDOR0_P8 = 0;
#define SST25SCS2DIS()               bFM3_GPIO_PDOR0_P8 = 1;
#define SST25WP1EN()                 bFM3_GPIO_PDOR0_P7 = 0;
#define SST25WP1DIS()                bFM3_GPIO_PDOR0_P7 = 1;


//外部存储器电源开关
#ifndef V4_FLASH_OFF
#define V4_FLASH_OFF()		  			IO_GPIOWriteData(GPIO_PORT7,GPIO_PIN_7,0x0000)
#endif
#ifndef V4_FLASH_ON
#define V4_FLASH_ON()		  			IO_GPIOWriteData(GPIO_PORT7,GPIO_PIN_7,0xFFFF)
#endif
/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
void SST25Init( void );
INT8U SpiSendByte( INT8U data );
INT8U SpiGetByte( void );
void SST25WriteEnable(  INT8U chip, BOOLEAN bEnable);
void SST25ChipEnable( INT8U chip, BOOLEAN bEnable );
INT8U SST25ReadStatus( INT8U chip );
void  SST25WRSR( INT8U chip, INT8U state );
INT32U SST25ReadID( INT8U chip );
INT32U SST25SPIFlashRead( INT8U* buff, INT32U addr, INT32U len );
void SST25SPIFlashErase( INT8U type, INT32U addr );
INT32U SST25SPIFlashWrite( INT8U* pStr, INT32U addr, INT32U len );
void SST25SPIFlashWriteByte( INT32U WriteAddr, INT8U WriteData );
INT32U SST25SPIFlashWritePage(INT8U* pBuffer, INT32U WriteAddr, INT32U NumByteToWrite);
INT32U SST25SPIFlashWriteBuffer(INT8U* pBuffer, INT32U WriteAddr, INT32U NumByteToWrite);
INT32U SST25SPIFlashEraseWrite( INT8U* buff, INT32U addr, INT32U len );

/* ===============================================================================
                   SPI Flash驱动接口
 ===============================================================================*/
INT32U FlashInit( void );
INT32U FlashReadID( INT32U* chipID );
INT32U FlashRead(  INT32U addr, INT8U* buff, INT32U len );
INT32U FlashWrite( INT32U addr, const INT8U* buff, INT32U len );
INT32U FlashEraseWrite( INT32U addr, const INT8U* buff, INT32U len );
INT32U FlashErase( INT8U type, INT32U addr );
INT32U FlashReset( INT32U time );
void FlashPowerOffOn( void );

#define _API_FLASH_Init()  FlashInit()
#define _API_FLASH_Reset()  FlashReset()
#define _API_FLASH_ReadID(c)  FlashReadID(c)
//#define _API_FLASH_Read(b,a,l)  FlashRead(a,b,l)
#define _API_FLASH_Write(b,a,l)  FlashWrite(a,b,l)
//#define _API_FLASH_EraseWrite(b,a,l) FlashEraseWrite(a,b,l)
//#define _API_FLASH_PowerOffOn()     FlashPowerOffOn()

#endif /* _SST25F032_H_ */
/*****************************************************************************/
/* END OF FILE */