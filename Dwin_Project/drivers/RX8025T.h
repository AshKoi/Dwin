/*
 ******************************************************************************
 **
 ** \note Other information.
 **
 ******************************************************************************
 */
#ifndef _RX8025T_H_
#define _RX8025T_H_

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "board.h"

/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/
#define RX8025TBAUDRATE    5

//器件地址选择及写标志
#define RX8025TAddWr 0x64 
//器件地址选择及读标志
#define RX8025TAddRd 0x65  

#define RX8025T_SDA_PFR                bFM3_GPIO_PFR2_P8
#define RX8025T_SDA_DDR                bFM3_GPIO_DDR2_P8
#define RX8025T_SDA_PDOR               bFM3_GPIO_PDOR2_P8
#define RX8025T_SDA_PDIR               bFM3_GPIO_PDIR2_P8

#define RX8025T_SCL_PFR                bFM3_GPIO_PFR2_P9
#define RX8025T_SCL_DDR                bFM3_GPIO_DDR2_P9
#define RX8025T_SCL_PDOR               bFM3_GPIO_PDOR2_P9


// 8025T内部地址
#define TIME_8025T_SEC               0x00
#define TIME_8025T_MIN               0x01
#define TIME_8025T_HOUR              0x02
#define TIME_8025T_WEEK              0x03
#define TIME_8025T_DAY               0x04
#define TIME_8025T_MONTH             0x05
#define TIME_8025T_YEAR              0x06
#define TIME_8025T_RAM               0x07
#define TIME_8025T_MIN_ALARM         0x08
#define TIME_8025T_HOUR_ALARM        0x09
#define TIME_8025T_WEEK_ALARM        0x0a
#define TIME_8025T_DAY_ALARM         0x0a
#define TIME_8025T_COUNTER_0         0x0b
#define TIME_8025T_COUNTER_1         0x0c
#define TIME_8025T_EXT_REGISTER      0x0d
#define TIME_8025T_FALG_REGISTER     0x0e
#define TIME_8025T_CTL_REGISTER      0x0f

/* Register definitions */
#define RX8025_REG_SEC		0x00
#define RX8025_REG_MIN		0x01
#define RX8025_REG_HOUR		0x02
#define RX8025_REG_WDAY		0x03
#define RX8025_REG_MDAY		0x04
#define RX8025_REG_MONTH	0x05
#define RX8025_REG_YEAR		0x06
#define RX8025_REG_DIGOFF	0x07
#define RX8025_REG_ALWMIN	0x08
#define RX8025_REG_ALWHOUR	0x09
#define RX8025_REG_ALWWDAY	0x0a
#define RX8025_REG_ALDMIN	0x0b
#define RX8025_REG_ALDHOUR	0x0c
/* 0x0d is reserved */
#define RX8025_REG_CTRL1	0x0e
#define RX8025_REG_CTRL2	0x0f

/* BCD格式 */
typedef struct _tagSTD_TIME
{
	INT8U nSec;					/* 取值范围：0 ~ 59 */
	INT8U nMin;					/* 取值范围：0 ~ 59 */
	INT8U nHour;				/* 取值范围：0 ~ 23 */
	INT8U nDay;					/* 取值范围：1 ~ 31 */
	INT8U nMon;					/* 取值范围：1 ~ 12 */
	INT8U nYear;				/* 取值范围：0 ~ 99 */
	INT8U nWeek;				/* 取值范围：1-7，分别表示星期一、星期二。。。星期日 */
}STD_TIME, *pSTD_TIME;


/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
void RX8025TInit( void );
INT8U DayOfWeek( INT16U year, INT16U month, INT16U date);
////////////////////////////////////////////////////////////////////////////////
/* ===============================================================================
                   时钟驱动接口
 ===============================================================================*/
INT32U RTCInit( void );
INT32U RTCDateTimeWrite( const STD_TIME* pTime );
INT32U RTCDateTimeRead( STD_TIME* pTime );


#endif /* _RX8025T_H_ */
/*****************************************************************************/
/* END OF FILE */