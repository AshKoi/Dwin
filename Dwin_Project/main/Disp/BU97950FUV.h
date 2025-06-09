/*
 ******************************************************************************
 **
 ** 地址范围：0 ~ 0x7FF.
 **
 ******************************************************************************
 */
#ifndef _BU97950FUV_H_
#define _BU97950FUV_H_

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "board.h"

/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/

#define BU97950FUVBAUDRATE			1


//器件地址选择及写标志
#define BU979AddWr 0x7C 
//器件地址选择及读标志
#define BU979AddRd 0x7D  

#define BU979_SDA_PFR                bFM3_GPIO_PFR7_P9
#define BU979_SDA_DDR                bFM3_GPIO_DDR7_P9
#define BU979_SDA_PDOR               bFM3_GPIO_PDOR7_P9
#define BU979_SDA_PDIR               bFM3_GPIO_PDIR7_P9

#define BU979_SCL_PFR                bFM3_GPIO_PFR7_P8
#define BU979_SCL_DDR                bFM3_GPIO_DDR7_P8
#define BU979_SCL_PDOR               bFM3_GPIO_PDOR7_P8

//#define BU979_WP_PFR                 bFM3_GPIO_PFRA_P0
//#define BU979_WP_DDR                 bFM3_GPIO_DDRA_P0
//#define BU979_WP_PDOR                bFM3_GPIO_PDORA_P0

//#define BU979WPEN()                  bFM3_GPIO_PDORA_P0 = 1;
//#define BU979WPDIS()                 bFM3_GPIO_PDORA_P0 = 0;


/* ===============================================================================
                   FRAM驱动接口
 ===============================================================================*/
INT32U BULCDInit( void );
INT32U BULCDWrite(INT16U nAddr, const INT8U* pBuf, INT32U nLen );
INT32U BULCDWrite1(INT16U nAddr, const INT8U* pBuf, INT32U nLen );
INT32U BULCDRead( INT16U nAddr, INT8U* pBuf, INT32U nLen );
INT32U BU979Reset( void );


#endif /* _FM24CL16_H_ */
/*****************************************************************************/
/* END OF FILE */