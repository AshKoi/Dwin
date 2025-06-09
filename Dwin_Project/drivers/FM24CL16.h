/*
 ******************************************************************************
 **
 ** ��ַ��Χ��0 ~ 0x7FF.
 **
 ******************************************************************************
 */
#ifndef _FM24CL16_H_
#define _FM24CL16_H_

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "board.h"

/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/

#define FM24CL16BAUDRATE			10

//������ַѡ��д��־
#define FRAMAddWr 0xA0 
//������ַѡ�񼰶���־
#define FRAMAddRd 0xA1  

#define FM24_SDA_PFR                bFM3_GPIO_PFRA_P2
#define FM24_SDA_DDR                bFM3_GPIO_DDRA_P2
#define FM24_SDA_PDOR               bFM3_GPIO_PDORA_P2
#define FM24_SDA_PDIR               bFM3_GPIO_PDIRA_P2

#define FM24_SCL_PFR                bFM3_GPIO_PFRA_P1
#define FM24_SCL_DDR                bFM3_GPIO_DDRA_P1
#define FM24_SCL_PDOR               bFM3_GPIO_PDORA_P1

#define FM24_WP_PFR                 bFM3_GPIO_PFRA_P0
#define FM24_WP_DDR                 bFM3_GPIO_DDRA_P0
#define FM24_WP_PDOR                bFM3_GPIO_PDORA_P0

#define FRAMWPEN()                  bFM3_GPIO_PDORA_P0 = 1;
#define FRAMWPDIS()                 bFM3_GPIO_PDORA_P0 = 0;


/* ===============================================================================
                   FRAM�����ӿ�
 ===============================================================================*/
INT32U FRAMInit( void );
INT32U FRAMWrite(INT16U nAddr, const INT8U* pBuf, INT32U nLen );
INT32U FRAMRead( INT16U nAddr, INT8U* pBuf, INT32U nLen );
INT32U FRAMReset( void );


#endif /* _FM24CL16_H_ */
/*****************************************************************************/
/* END OF FILE */