/***************************************************************
* Copyright(c) 2010 光一科技股份有限公司 研发中心 - 基础技术部
* 文件名: PCA9555.H
* 创建人: lightrain
* 日　期: 2011-10-31
* 修改人:
* 日　期:
* 描　述: 通过GPIO口线模拟IIC总线操作，适用与PCA9555、CAT9555。
* 版　本: Version 1.0
****************************************************************/
#ifndef _9555_HEADER_
#define _9555_HEADER_

#include "global.h"

#define CAT9555_MODULE		0x01

#define KEY_9555_SDA_PFR                bFM3_GPIO_PFRA_P4
#define KEY_9555_SDA_DDR                bFM3_GPIO_DDRA_P4
#define KEY_9555_SDA_PDOR               bFM3_GPIO_PDORA_P4
#define KEY_9555_SDA_PDIR               bFM3_GPIO_PDIRA_P4

#define KEY_9555_SCL_PFR                bFM3_GPIO_PFRA_P3
#define KEY_9555_SCL_DDR                bFM3_GPIO_DDRA_P3
#define KEY_9555_SCL_PDOR               bFM3_GPIO_PDORA_P3
#define KEY_9555_SCL_PDIR               bFM3_GPIO_PDIRA_P3

#define EMULATOR_9555_INIT()         KEY_9555_SDA_PFR=0x00;KEY_9555_SCL_PFR=0x00;KEY_9555_SDA_DDR=0x01;KEY_9555_SCL_DDR=0x01
#define EMULATOR_9555_OUTPUT()       KEY_9555_SDA_DDR=0x01
#define EMULATOR_9555_INPUT()        KEY_9555_SDA_DDR=0x00
#define EMULATOR_9555_SCK_UP()       KEY_9555_SCL_PDOR=0x01
#define EMULATOR_9555_SCK_DWN()      KEY_9555_SCL_PDOR=0x00
#define EMULATOR_9555_SDA_UP()       KEY_9555_SDA_PDOR=0x01
#define EMULATOR_9555_SDA_DWN()      KEY_9555_SDA_PDOR=0x00
#define EMULATOR_9555_SDA_DATA()     KEY_9555_SDA_PDIR


INT8U PCA9555_Write( INT8U addr, INT8U szWrite[], INT8U len );
INT8U PCA9555_Read( INT8U addr, INT8U szRead[], INT8U len );

/* */
BOOLEAN PCA9555Init( INT8U port, INT32U timeout );

/* */
INT8U PCA9555Read( INT8U port, INT8U addr );

/**/
void PCA9555Write( INT8U port, INT8U addr, INT8U data );

BOOLEAN PCA9555Reinit( INT8U port );

void RefreshKey( void );

BOOLEAN PCA9555_Probe( INT8U slaveaddr );

#endif


