/*
 ******************************************************************************
 **
 ** \note Other information.
 **
 ******************************************************************************
 */
#ifndef _BSP_H_
#define _BSP_H_

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "board.h"

/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/



#define LED_RUN						0
#define LED_ALARM					1
#define LED_RXDU					2
#define LED_TXDU					3
#define LED_RXDD					4
#define LED_TXDD					5
#define LED_CSQ0					6
#define LED_CSQ1					7
#define LED_ONLINE					8
#define LED_ALL						9

#define LED_ON						1
#define LED_OFF						0

#define GPIO_STATUS_LOW             0
#define GPIO_STATUS_HIGH            1
#define GPIO_STATUS_UNKOWN          0xFF

/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
void BspLedInit(void);
void BspLedTurn(INT8U led, INT8U state);

void  LED_Run_On (void);
void  LED_Run_Off (void);
void  LED_Alarm_On (void);
void  LED_Alarm_Off (void);
void  LED_TXDU_On (void);
void  LED_TXDU_Off (void);
void  LED_RXDU_On (void);
void  LED_RXDU_Off (void);
void  LED_TXDD_On (void);
void  LED_TXDD_Off (void);
void  LED_RXDD_On (void);
void  LED_RXDD_Off (void);
void  LED_BLK_On (void);
void  LED_BLK_Off (void);
void  CTRL_DD_On (void);				//使能充电
void  CTRL_DD_Off (void);				//禁止充电
void  GPRS_CTRL_HIGH (void);
void  GPRS_CTRL_LOW (void);
void  GPRS_ONOF_HIGH (void);
void  GPRS_ONOF_LOW (void);
void  WDI_HIGH (void);
void  WDI_LOW (void);
void GPRS_CSQ0_On( void );
void GPRS_CSQ0_Off( void );
void GPRS_CSQ1_On( void );
void GPRS_CSQ1_Off( void );
void  GPRS_RST_HIGH (void);
void  GPRS_RST_LOW (void);
void  GPRS_ONLINE_On (void);
void  GPRS_ONLINE_Off (void);
void GPRS_SIMHEAT_On( void ); //空函数
void GPRS_SIMHEAT_Off( void );//空函数
void  LED_Run_Toggle (void);
void  LED_Alarm_Toggle (void);
void  WDI_Toggle (void);
INT8U GetStatus( INT8U IoSelect );
INT8U BSP_PD_GetStatus( void );



#endif /* _BSP_H_ */
/*****************************************************************************/
/* END OF FILE */