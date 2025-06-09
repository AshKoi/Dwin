/***************************************************************
* Copyright(c) 2010 光一科技股份有限公司 研发中心 - 基础技术部
* 文件名: ATT7022.c
* 创建人: lightrain
* 日　期: 2010-08-04
* 修改人:
* 日　期:
* 描　述: ATT7022接口头文件
* 版　本: Version 1.0
****************************************************************/
#ifndef _ATT7022_HEADER_
#define _ATT7022_HEADER_

#include "config.h"

/* SPI MODE definition */
#define SPI_MODE_0			0
#define SPI_MODE_1			1
#define SPI_MODE_2			2
#define SPI_MODE_3			3

#define ATT7022_SPI_CS_PFR                bFM3_GPIO_PFR1_P9
#define ATT7022_SPI_CS_DDR                bFM3_GPIO_DDR1_P9
#define ATT7022_SPI_CS_PDOR               bFM3_GPIO_PDOR1_P9
#define ATT7022_SPI_CS_PDIR               bFM3_GPIO_PDIR1_P9

#define ATT7022_SPI_MISO_PFR                bFM3_GPIO_PFR1_PA
#define ATT7022_SPI_MISO_DDR                bFM3_GPIO_DDR1_PA
#define ATT7022_SPI_MISO_PDOR               bFM3_GPIO_PDOR1_PA
#define ATT7022_SPI_MISO_PDIR               bFM3_GPIO_PDIR1_PA

#define ATT7022_SPI_MOSI_PFR                bFM3_GPIO_PFR1_PB
#define ATT7022_SPI_MOSI_DDR                bFM3_GPIO_DDR1_PB
#define ATT7022_SPI_MOSI_PDOR               bFM3_GPIO_PDOR1_PB
#define ATT7022_SPI_MOSI_PDIR               bFM3_GPIO_PDIR1_PB

#define ATT7022_SPI_CLK_PFR                bFM3_GPIO_PFR1_PC
#define ATT7022_SPI_CLK_DDR                bFM3_GPIO_DDR1_PC
#define ATT7022_SPI_CLK_PDOR               bFM3_GPIO_PDOR1_PC
#define ATT7022_SPI_CLK_PDIR               bFM3_GPIO_PDIR1_PC

#define ATT7022_RST_PFR                bFM3_GPIO_PFR1_PD
#define ATT7022_RST_DDR                bFM3_GPIO_DDR1_PD
#define ATT7022_RST_PDOR               bFM3_GPIO_PDOR1_PD
#define ATT7022_RST_PDIR               bFM3_GPIO_PDIR1_PD


/* ATT7022 pin configuration for SPI */
#define ATT7022_SPI_INIT()		ATT7022_SPI_CS_PFR=0x00;ATT7022_SPI_CS_DDR=0x01;\
	ATT7022_SPI_MISO_PFR=0x00;ATT7022_SPI_MISO_DDR=0x00;\
	ATT7022_SPI_MOSI_PFR=0x00;ATT7022_SPI_MOSI_DDR=0x01;\
	ATT7022_SPI_CLK_PFR=0x00;ATT7022_SPI_CLK_DDR=0x01;\
	ATT7022_RST_PFR=0x00;ATT7022_RST_DDR=0x01


#define ATT7022_SPI_CS_INIT()			ATT7022_SPI_CS_PFR=0x00;ATT7022_SPI_CS_DDR=0x01
#define ATT7022_SPI_CS_EN()				ATT7022_SPI_CS_PDOR=0x00
#define ATT7022_SPI_CS_DIS()			ATT7022_SPI_CS_PDOR=0x01
#define ATT7022_SPI_CLK_HIGH()			ATT7022_SPI_CLK_PDOR=0x01
#define ATT7022_SPI_CLK_LOW()			ATT7022_SPI_CLK_PDOR=0x00
#define ATT7022_SPI_MOSI_HIGH()			ATT7022_SPI_MOSI_PDOR=0x01
#define ATT7022_SPI_MOSI_LOW()			ATT7022_SPI_MOSI_PDOR=0x00
#define ATT7022_SPI_MISO_DATA()			ATT7022_SPI_MISO_PDIR


/* ATT7022 Reset pin configuration */
/*
#define ATT7022_RST_CLK_EN 				BSP_PER_ID_PIOB
#define ATT7022_RST_PIN_CFG				(BSP_GPIO_OPT_PIN_EN | BSP_GPIO_OPT_OUT_EN)
#define ATT7022_RST_PIO					BSP_GPIO_PORT_B
#ifdef CFG_GW_ZBI
#define ATT7022_RST_PIN					DEF_BIT_24
#else
#define ATT7022_RST_PIN					DEF_BIT_31
#endif
*/





void   ATT7022Init( void );

INT32U ATT7022RegRead(  INT8U cmd );
void   ATT7022RegWrite( INT8U cmd, INT32U data );

INT32U ATT7022DataRead( INT32U addr, INT8U addrSize, INT8U *buff, INT32U len );
INT32U ATT7022DataWrite(INT32U addr, INT8U addrSize, INT8U *buff, INT32U len );

////////////////////////////////////////////////////////////////////////////////
INT32U _API_ATT7022_Init( INT32U timeout );
INT32U _API_ATT7022_Reset( INT32U time );
INT32U _API_ATT7022_RegRead( INT8U cmd );
INT32U _API_ATT7022_RegWrite( INT8U cmd, INT32U data );
INT32U _API_ATT7022_DataRead( INT32U addr, INT8U addrSize, INT8U *recvBuf, INT32U len );
INT32U _API_ATT7022_DataWrite(INT32U addr, INT8U addrSize, const INT8U *sendBuf, INT32U len );

INT32U _API_ATT7022_DataInit( INT32U timeout );
INT32U _API_ATT7022_DataReset( INT32U time );

void ATT7022Reset( INT32U time );

#endif
