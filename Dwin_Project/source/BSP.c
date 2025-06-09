
#include "global.h"
#include "BSP.h"


/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/

/*******************************************************************************
  * *函数名称	BspLedInit
  * *函数说明   初始化
  * *输入参数   无
  * *输出参数   无
  * *返回参数   无
*******************************************************************************/
void BspLedInit(void)
{
	//配置所有使用IO引脚
#if 0
	FM3_GPIO->PFR3 &= 0x2FFF;
	FM3_GPIO->PFR4 &= 0xFFC0;
	FM3_GPIO->DDR3 |= 0xD000;
	FM3_GPIO->DDR4 |= 0x003F;
	FM3_GPIO->PDOR3 |= 0xD000;
	FM3_GPIO->PDOR4 |= 0x003F;
#else
	FM3_GPIO->ADE = 0;
	
    GpioConfig(IO_PORT1, IO_PINx3, GPIO_MODE_INPUT);	//ST1
    GpioConfig(IO_PORT2, IO_PINx0, GPIO_MODE_OUTPUT);	//WDI
    GpioConfig(IO_PORTC, IO_PINxF, GPIO_MODE_OUTPUT);	//GPRS_RST
    GpioConfig(IO_PORTD, IO_PINx0, GPIO_MODE_OUTPUT);	//GPRS_ONOFF
    GpioConfig(IO_PORTF, IO_PINx5, GPIO_MODE_INPUT);	//STATE0
    GpioConfig(IO_PORTC, IO_PINxE, GPIO_MODE_INPUT);	//STATE1
    GpioConfig(IO_PORT6, IO_PINx2, GPIO_MODE_INPUT);	//STATE2
    GpioConfig(IO_PORTC, IO_PINx0, GPIO_MODE_OUTPUT);	//PCRTL
    GpioConfig(IO_PORT2, IO_PINx5, GPIO_MODE_OUTPUT);	//GPRS_CTRL
    GpioConfig(IO_PORT2, IO_PINx4, GPIO_MODE_INPUT);	//pfo
	
	bFM3_GPIO_EPFR00_JTAGEN0B = 1;						//PHY_RST
	bFM3_GPIO_EPFR00_JTAGEN1S = 0;
    GpioConfig(IO_PORT0, IO_PINx2, GPIO_MODE_OUTPUT);

	GPRS_ONOF_LOW();
	GPRS_RST_HIGH();

	
    GpioConfig(IO_PORT3, IO_PINx7, GPIO_MODE_OUTPUT);
    GpioConfig(IO_PORT3, IO_PINx8, GPIO_MODE_OUTPUT);
    GpioConfig(IO_PORT3, IO_PINx9, GPIO_MODE_OUTPUT);
    GpioConfig(IO_PORT3, IO_PINxC, GPIO_MODE_OUTPUT);
    GpioConfig(IO_PORT3, IO_PINxD, GPIO_MODE_OUTPUT);
    GpioConfig(IO_PORT3, IO_PINxE, GPIO_MODE_OUTPUT);
    GpioConfig(IO_PORT3, IO_PINxF, GPIO_MODE_OUTPUT);
    GpioConfig(IO_PORT4, IO_PINx1, GPIO_MODE_OUTPUT);
    GpioConfig(IO_PORT4, IO_PINx2, GPIO_MODE_OUTPUT);
	
	LED_RXDD_Off();
	LED_TXDD_Off();
#endif
}

/*******************************************************************************
  * *函数名称	BspLedTurn
  * *函数说明   LED灯亮或灭
  * *输入参数   无
  * *输出参数   无
  * *返回参数   无
*******************************************************************************/
void BspLedTurn(INT8U led, INT8U state)
{
    if( state == LED_ON)
    { 
        GpioClearOutput(IO_PORT3, IO_PINx7);
        GpioClearOutput(IO_PORT3, IO_PINx8);
        GpioClearOutput(IO_PORT3, IO_PINx9);
        GpioClearOutput(IO_PORT3, IO_PINxC);
        GpioClearOutput(IO_PORT3, IO_PINxD);
        GpioClearOutput(IO_PORT3, IO_PINxE);
        GpioClearOutput(IO_PORT3, IO_PINxF);
        GpioClearOutput(IO_PORT4, IO_PINx1);
        GpioClearOutput(IO_PORT4, IO_PINx2);
    }
    else
    {
        GpioSetOutput(IO_PORT3, IO_PINx7);
        GpioSetOutput(IO_PORT3, IO_PINx8);
        GpioSetOutput(IO_PORT3, IO_PINx9);
        GpioSetOutput(IO_PORT3, IO_PINxC);
        GpioSetOutput(IO_PORT3, IO_PINxD);
        GpioSetOutput(IO_PORT3, IO_PINxE);
        GpioSetOutput(IO_PORT3, IO_PINxF);
        GpioSetOutput(IO_PORT4, IO_PINx1);
        GpioSetOutput(IO_PORT4, IO_PINx2);
    }
    
}

void  LED_Run_On (void)
{
	GpioClearOutput(IO_PORT3, IO_PINx7);
}
void  LED_Run_Off (void)
{
	GpioSetOutput(IO_PORT3, IO_PINx7);
}
void  LED_Alarm_On (void)
{
	GpioClearOutput(IO_PORT4, IO_PINx2);
}
void  LED_Alarm_Off (void)
{
	GpioSetOutput(IO_PORT4, IO_PINx2);
}
void  LED_TXDU_On (void)
{
	GpioClearOutput(IO_PORT4, IO_PINx1);
}
void  LED_TXDU_Off (void)
{
	GpioSetOutput(IO_PORT4, IO_PINx1);
}
void  LED_RXDU_On (void)
{
	GpioClearOutput(IO_PORT3, IO_PINxF);
}
void  LED_RXDU_Off (void)
{
	GpioSetOutput(IO_PORT3, IO_PINxF);
}
void  LED_TXDD_On (void)
{
	GpioClearOutput(IO_PORT3, IO_PINxE);
}
void  LED_TXDD_Off (void)
{
	GpioSetOutput(IO_PORT3, IO_PINxE);
}
void  LED_RXDD_On (void)
{
	GpioClearOutput(IO_PORT3, IO_PINxD);
}
void  LED_RXDD_Off (void)
{
	GpioSetOutput(IO_PORT3, IO_PINxD);
}
void  CTRL_DD_On (void)
{
    GpioConfig(IO_PORTC, IO_PINx1, GPIO_MODE_OUTPUT);
	GpioClearOutput(IO_PORTC, IO_PINx1);
}
void  CTRL_DD_Off (void)
{
    GpioConfig(IO_PORTC, IO_PINx1, GPIO_MODE_INPUT);
}
void  LED_BLK_On (void)
{
}
void  LED_BLK_Off (void)
{
}
void  GPRS_CTRL_HIGH (void)
{
	GpioSetOutput(IO_PORT2, IO_PINx5);
}
void  GPRS_CTRL_LOW (void)
{
	GpioClearOutput(IO_PORT2, IO_PINx5);
}
void  GPRS_ONOF_HIGH (void)
{
	GpioSetOutput(IO_PORTD, IO_PINx0);
}
void  GPRS_ONOF_LOW (void)
{
	GpioClearOutput(IO_PORTD, IO_PINx0);
}
void  WDI_HIGH (void)
{
	GpioSetOutput(IO_PORT2, IO_PINx0);
}
void  WDI_LOW (void)
{
	GpioClearOutput(IO_PORT2, IO_PINx0);
}
void GPRS_CSQ0_On( void )
{
    // CSQ0
	GpioClearOutput(IO_PORT3, IO_PINx8);
}
void GPRS_CSQ0_Off( void )
{
    // CSQ0
	GpioSetOutput(IO_PORT3, IO_PINx8);
}
void GPRS_CSQ1_On( void )
{
    // CSQ1
	GpioClearOutput(IO_PORT3, IO_PINx9);
}
void GPRS_CSQ1_Off( void )
{
    // CSQ1
	GpioSetOutput(IO_PORT3, IO_PINx9);
}
void  GPRS_RST_HIGH (void)
{
	GpioSetOutput(IO_PORTC, IO_PINxF);
}
void  GPRS_RST_LOW (void)
{
	GpioClearOutput(IO_PORTC, IO_PINxF);
}
void  GPRS_ONLINE_On (void)
{
	GpioClearOutput(IO_PORT3, IO_PINxC);
}
void  GPRS_ONLINE_Off (void)
{
	GpioSetOutput(IO_PORT3, IO_PINxC);
}

void GPRS_SIMHEAT_On( void )
{
	GpioSetOutput(IO_PORTC, IO_PINx0);
}

void GPRS_SIMHEAT_Off( void )
{
	GpioClearOutput(IO_PORTC, IO_PINx0);
}

void  LED_Run_Toggle (void)
{
    INT8U  IOStatus;
	IOStatus = bFM3_GPIO_PDOR3_P7;
	IOStatus = (IOStatus == 0)?1:0;
	bFM3_GPIO_PDOR3_P7 = IOStatus;
}

void  LED_Alarm_Toggle (void)
{
    INT8U  IOStatus;
	IOStatus = bFM3_GPIO_PDOR4_P2;
	IOStatus = (IOStatus == 0)?1:0;
	bFM3_GPIO_PDOR4_P2 = IOStatus;
}

void  WDI_Toggle (void)
{
    INT8U  IOStatus;
	IOStatus = bFM3_GPIO_PDOR2_P0;
	IOStatus = (IOStatus == 0)?1:0;
	bFM3_GPIO_PDOR2_P0 = IOStatus;
}

/* 遥信状态获取*/
INT8U GetStatus( INT8U IoSelect )
{
  //此处需增加根据参数获取状态
    INT8U ret = GPIO_STATUS_UNKOWN;
    switch( IoSelect )
    {
    case 0:
        // 1路遥信
        ret = GpioGetInput( IO_PORT1, IO_PINx3 );
        break;
    case 1:
        //3路模块识别
        ret = 0;
        if( GpioGetInput( IO_PORTF, IO_PINx5 ) == GPIO_STATUS_HIGH )	//STATE0
            ret |= 1;
        if( GpioGetInput( IO_PORTC, IO_PINxE ) == GPIO_STATUS_HIGH )	//STATE1
            ret |= 2;
        if( GpioGetInput( IO_PORT6, IO_PINx2 ) == GPIO_STATUS_HIGH )	//STATE2
            ret |= 4;
        break;
    default:
        break;
    }
    return ret;
}

INT8U BSP_PD_GetStatus( void )
{
	if(GpioGetInput( IO_PORT2, IO_PINx4 )==1)
		return 0;
	else
		return 1;
}