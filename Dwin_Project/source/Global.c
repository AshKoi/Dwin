/***************************************************************
****************************************************************/
#include "AppHead.h"
#include "Global.h"
#include "config.h"
#include "ATT7022.h"
#include "Ac7022.h"


/***************************************************************
  Driver版本定义
  Driver version definition
***************************************************************/
#define DRIVER_VERSION			"Driver ver 1.3.3 for JZQ-II Fujitsu"


#define  BAT_ADC	1
#define  BATTERY_ADC	2
#define  RS485B_ADC	3
#define  CLOCK_ADC	    4




const INT8U driver_version_string[] =
	DRIVER_VERSION" (" __DATE__ " - " __TIME__ ")";

INT8U* GetApiVersion( void )
{
	return (INT8U*)driver_version_string;
}
/* ===============================================================================
                   口线控制接口扩展函数
 ===============================================================================*/
/*---------------------------------------------------------------------------*/
/* 读取口线状态函数                                                              */
/*---------------------------------------------------------------------------*/
INT8U ReadCpuIoStatusEX( INT8U portpin )
{
    INT8U ret;
    INT8U port;
    INT32U pin;
    port = (portpin>>4)&0x0f;
    pin = 1<<(portpin&0x0f);
    ret = IO_ReadGPIOPin(port, pin);
    return ret;
} 

/*---------------------------------------------------------------------------*/
/* 口线配置函数                                                              */
/*---------------------------------------------------------------------------*/
void GpioConfigEX( INT8U portpin, INT8U mode, INT8U pullup )        // 配置IO为输入或者输出;首先将IO配置为GPIO
{                                                                    // mode :GPIO_MODE_INPUT;   GPIO_MODE_OUTPUT
																														//pullup: IO_PULLUP_OFF;   IO_PULLUP_OPEN
    INT8U port;
    INT32U pin;
    port = (portpin>>4)&0x0f;
    pin = 1<<(portpin&0x0f);
    
    IO_ConfigPort(port, pin, IO_GPIO_MODE);             
    IO_ConfigGPIOPin(port, pin, mode, pullup);
}
/*---------------------------------------------------------------------------*/
/* 口线置1扩展函数                                                              */
/*---------------------------------------------------------------------------*/
void GpioSetOutputEX( INT8U portpin )
{
//    GpioConfig(port, pin, GPIO_MODE_OUTPUT);
		INT8U port;
    INT32U pin;
		port = (portpin>>4)&0x0f;
		pin = 1<<(portpin&0x0f);
    WriteCpuIoStatus(port, pin, IO_BIT_SET);
}
/*---------------------------------------------------------------------------*/
/* 口线置0扩展函数                                                             */
/*---------------------------------------------------------------------------*/
void GpioClearOutputEX( INT8U portpin )
{
//    GpioConfig(port, pin, GPIO_MODE_OUTPUT);
    INT8U port;
    INT32U pin;
    port = (portpin>>4)&0x0f;
    pin = 1<<(portpin&0x0f);
    WriteCpuIoStatus(port, pin, IO_BIT_CLR);
}


/* ===============================================================================
                   GPIO驱动接口
 ===============================================================================*/

void WriteCpuIoStatus( INT8U port, INT32U pin, INT8U status )
{                                                               //port :IO_PORT0..IO_PORTF;
    IO_WriteGPIOPin( port, pin, status );                       //pin:IO_PINx0..IO_PINxF
}                                                               //status:IO_BIT_CLR 低电平;  IO_BIT_SET 高电平

INT8U ReadCpuIoStatus( INT8U port, INT32U pin )
{
    INT8U ret;
    ret = IO_ReadGPIOPin(port, pin);
    return ret;
} 

void GpioConfig( INT8U port, INT32U pin, INT8U mode )       //配置IO为输入或者输出;首先将IO配置为GPIO
{                                                           //mode :GPIO_MODE_INPUT;   GPIO_MODE_OUTPUT
    IO_ConfigPort(port, pin, IO_GPIO_MODE);             
    IO_ConfigGPIOPin(port, pin, mode, IO_PULLUP_OFF);
}

void GpioSetOutput( INT8U port, INT32U pin )
{
//    GpioConfig(port, pin, GPIO_MODE_OUTPUT);
    WriteCpuIoStatus(port, pin, IO_BIT_SET);
}

void GpioClearOutput( INT8U port, INT32U pin )
{
//    GpioConfig(port, pin, GPIO_MODE_OUTPUT);
    WriteCpuIoStatus(port, pin, IO_BIT_CLR);
}

INT8U GpioGetInput( INT8U port, INT32U pin )
{
    INT8U ret;
//    GpioConfig(port, pin, GPIO_MODE_INPUT);
    ret = ReadCpuIoStatus(port, pin);
    return ret;
}


/* ===============================================================================
                   看门狗驱动接口
 ===============================================================================*/
void WatchDogEnable( INT8U index )              //index = 0 CPU内部看门狗； index = 1 CPU外部看门狗
{
//	if( index == 0)                             //没有启用内部看门狗
 //       return;
 //   else if(index == 1)
 //   {
 //       WDG_PFR = 0;		            //WDG
 //       WDG_DDR = 1;
 //       WDG_PDOR = 1;
 //   }
}

void WatchDogDisable( INT8U index )
{
//	if ( index == 0 )
//		return;
//	else if ( index == 1 )
//		return;
}

void WatchDogClear(INT8U index)
{
//	static INT8U IOStatus = 0;
//	if( index ==0 )
 //       return;
 //   else if( index == 1)
 //   {
  //      IOStatus = (IOStatus == 0)?1:0;
  //      WDG_PDOR = IOStatus;
//    }
}


/* ===============================================================================
                   GPRS/CDMA通讯模块接口函数
 ===============================================================================*/
//void GPRSInit( INT8U type )
//{
//    
//}
//
//void GPRSReset( void )
//{
//    
//}
//
//BOOLEAN GPRSIsOnline( void )
//{
//    
//    return TRUE;
//}
//
//void GPRSDataEnable( BOOLEAN bEnable )
//{
//    
//}
//
//void GPRSIntEnable( void )
//{
//    
//}
//
//BOOLEAN GPRSGetSMSStatus( void )
//{
//    
//    return TRUE;
//}

/* ===============================================================================
                   获取485A电压
 ===============================================================================*/
INT32U Get485Voltage( INT8U n )
{
    INT32U vbat;
    Init_485_ADC(n);
    vbat = GetVbat();
 
    return vbat;
}

void Init_485_ADC(INT8U n)
{
    
	FM3_ADC0->ADCEN |= 0x01;
   
	FM3_ADC0->ADSS0 = 0x00;	
	FM3_ADC0->ADST0 = 0x43;             
	FM3_ADC0->ADCT  = 0x00;            
	FM3_ADC0->SCCR  = 0x10;	           
	FM3_ADC0->CMPCR = 0x00;	
    //FM3_ADC0->SCCR |= 0x01;	
}


/* ===============================================================================
                   获取电池电压驱动接口
 ===============================================================================*/
INT32U GetAD0Voltage( void )
{
    INT32U vbat;
    //ADC_init(BAT_ADC);
    
    vbat = GetBatVbat();
    return vbat;
}

CPU_INT16U  BSP_ADC_GetStatus (CPU_INT08U  adc)
{
	INT16U adcS;
	adcS = GetAD0Voltage();
	return adcS;
}


INT32U GetBatVbat(void)
{
    INT8U   i       =   0;
    INT32U  sum     =   0;
    FP32    vbat    =   0;
    INT32U  ADC_VAL =  0;
    INT32U  AD_Data_temp = 0;
    
    //ADC_init(BAT_ADC);
    ADC_init(BATTERY_ADC);
    OSTimeDlyHMSM( 0, 0, 0, 100 );
    FM3_ADC0->SCCR |= 0x04;     
	FM3_ADC0->SCCR |= 0x01;         	/* A/DC start */
    
    OSTimeDlyHMSM( 0, 0, 0, 100 );
    for (i = 0; i < 50; i++)
    {
        if(FM3_ADC0->ADCR & 0x80)
        {
             OSTimeDlyHMSM( 0, 0, 0, 10 );
             AD_Data_temp =FM3_ADC0->SCFD;
        }
    }
    
    for (i = 0; i < 16; i++)
    {
        if(FM3_ADC0->ADCR & 0x80)
        {
             OSTimeDlyHMSM( 0, 0, 0, 10 );
             AD_Data_temp =FM3_ADC0->SCFD;
			 ADC_VAL = AD_Data_temp >> 20;
             sum += ADC_VAL;
        }
    }
    
    FM3_ADC0->SCCR &= ~0x04;   
	FM3_ADC0->ADCEN &= ~0x01;           //关闭ADC
    
    sum = sum / i;
    vbat = (FP32)(sum *3300) / 4096;
    vbat *= 3.63;
    sum = (INT32U)vbat;
    
    return sum;
}
INT32U GetClockVbat(void)
{
    INT8U   i       =   0;
    INT32U  sum     =   0;
    FP32    vbat    =   0;
    INT32U  ADC_VAL =  0;
    INT32U  AD_Data_temp = 0;
    
    //ADC_init(BAT_ADC);
    ADC_init(CLOCK_ADC);
    OSTimeDlyHMSM( 0, 0, 0, 100 );
    FM3_ADC0->SCCR |= 0x04;     
	FM3_ADC0->SCCR |= 0x01;         	/* A/DC start */
    
    OSTimeDlyHMSM( 0, 0, 0, 100 );
    for (i = 0; i < 50; i++)
    {
        if(FM3_ADC0->ADCR & 0x80)
        {
             OSTimeDlyHMSM( 0, 0, 0, 10 );
             AD_Data_temp =FM3_ADC0->SCFD;
        }
    }
    
    for (i = 0; i < 16; i++)
    {
        if(FM3_ADC0->ADCR & 0x80)
        {
             OSTimeDlyHMSM( 0, 0, 0, 10 );
             AD_Data_temp =FM3_ADC0->SCFD;
			 ADC_VAL = AD_Data_temp >> 20;
             sum += ADC_VAL;
        }
    }
    
    FM3_ADC0->SCCR &= ~0x04;   
	FM3_ADC0->ADCEN &= ~0x01;           //关闭ADC
    
    sum = sum / i;
    vbat = (FP32)(sum *3300) / 4096;
    vbat *= 3.63;
    sum = (INT32U)vbat;
    
    return sum;
}
/*void Init_ADC(void)
{
  
	BAT_AD_ADE = 1;	
	BAT_AD_PFR = 1;   
	FM3_ADC0->ADCEN |= 0x01;//操作使能
	BAT_ADC_SCIS = 1;       //选择输入寄存器:SCIS1,AN8~AN15
	FM3_ADC0->ADSS0 = 0x00;//采样时间选择寄存器
	FM3_ADC0->ADST0 = 0x43;//采样时间设定寄存器
	FM3_ADC0->ADCT  = 0x00;//比较时间设定寄存器：分频
	FM3_ADC0->SCCR  = 0x10;//d0位转换启动在启动，d4位为1清零fifo        
	FM3_ADC0->CMPCR = 0x00;	//比较功能寄存器：禁止
    //FM3_ADC0->SCCR |= 0x01;	
}*/
/*void Init_ADC(void)
{
	//BAT_A D_ADE = 1;	
    
                                
    //FM3_GPIO->PFR1        &= ~(0x01 << 8);          // 第8引脚复用 p18
	FM3_GPIO->PFR1        |= (0x01 << 8);           // 第8引脚复用
   // FM3_GPIO->ADE         &= ~(0x01 << 8);           // ADE[8] AN08 作为模拟输入 
    FM3_GPIO->ADE         |= (0x01 << 8);           // ADE[8] AN08 作为模拟输入  
      
    FM3_ADC0->ADCEN       |= 0x01;                 // 使能     
    while(!(FM3_ADC0->ADCEN & 0x02));
	FM3_ADC0->SCIS1       |= 0x01;                 // 8
 	//BAT_AD_PFR = 1;             // PFR1
	//FM3_ADC0->ADCEN |= 0x01;
	//BAT_ADC_SCIS = 1;
	FM3_ADC0->ADSS0      = 0x00;	
	FM3_ADC0->ADST0      = 0x99;  // 2f            
	FM3_ADC0->ADCT       = 0x02;  
    //FM3_ADC0->SCCR       &= ~0x10;
	FM3_ADC0->SCCR       |= 0x10;	           
	FM3_ADC0->CMPCR      = 0x00;	
    FM3_ADC0->ADSR       &=~(0x40);
}*/




void ADC_init(INT8U num)
{

	FM3_ADC0->ADCEN &= ~0x01;           //关闭ADC

	BAT_AD_ADE = 1;	
	BAT_AD_PFR = 1;   

	BATTERY_AD_ADE = 1;	
	BATTERY_AD_PFR = 1;  

	RS485B_AD_ADE = 1;	
	RS485B_AD_PFR = 1;  

	CLOCK_AD_ADE = 1;	
	CLOCK_AD_PFR = 1; 
	
	if (num == BAT_ADC)//12
	{
		BAT_ADC_SCIS = 1;
		BATTERY_ADC_SCIS = 0;
		RS485B_ADC_SCIS = 0;
		CLOCK_ADC_SCIS = 0;
	}
	else if (num == BATTERY_ADC)//3.3
	{
		BAT_ADC_SCIS = 0;
		BATTERY_ADC_SCIS = 1;
		RS485B_ADC_SCIS = 0;
		CLOCK_ADC_SCIS = 0;
	}
	else if (num == RS485B_ADC)//8
	{
		BAT_ADC_SCIS = 0;
		BATTERY_ADC_SCIS = 0;
		RS485B_ADC_SCIS = 1;
		CLOCK_ADC_SCIS = 0;
	}
	else if (num == CLOCK_ADC)//5
	{
		BAT_ADC_SCIS = 0;
		BATTERY_ADC_SCIS = 0;
		RS485B_ADC_SCIS = 0;
        CLOCK_ADC_SCIS = 1;
	}
	
        
    FM3_ADC0->SFNS = 0x0F;

	FM3_ADC0->ADSS0 = 0x00;	
	FM3_ADC0->ADST0 = 0x43;             
	FM3_ADC0->ADCT  = 0x00;            
	FM3_ADC0->SCCR  = 0x10;	           
	FM3_ADC0->CMPCR = 0x00;	

	FM3_ADC0->ADCEN |= 0x01;
        
        //OSTimeDlyHMSM( 0, 0, 0, 500 );
}



#if 0
INT32U GetVbat(void)
{
    INT32U j = 0;
	volatile INT32U AD_Data_temp;
    FP32 vbat=0;

    FM3_ADC0->SCCR |= 0x01;         	/* A/DC start */
    while( (FM3_ADC0->ADSR&0x1) && (j < 10000) )j++;//扫描状态
    AD_Data_temp =FM3_ADC0->SCFD;                   //12位AD时，高12位为数据
    AD_Data_temp = AD_Data_temp >> 20;
    vbat = (FP32)(AD_Data_temp *3300) / 4096;
    
    //vbat *= 2;
    //vbat = vbat * 147 / 100;
    //FM3_ADC0->ADCEN &= ~0x01;           //关闭ADC
    FM3_ADC0->ADCEN = 0;           //关闭ADC
	return (INT32U)vbat;
}
#endif

INT32U GetVbat(void)
{
    INT32U j = 0;
	volatile INT32U AD_Data_temp;
    INT32U vbat=0;
    INT8U CH = 0;
    j = FM3_ADC0->ADCEN;
    if(j !=3)
    {
        return 0;
    }
    FM3_ADC0->SCCR |= 0x01 ;         	/* A/DC start */  
    while(FM3_ADC0->ADSR&0x01);
    AD_Data_temp = FM3_ADC0->SCFD;
    
    if((AD_Data_temp >> 12)&0x01)      /* 数据有效 */
        return 0;    
    CH           = AD_Data_temp & 0x1F;
    if(CH == 8)
    {
        AD_Data_temp = (AD_Data_temp >> 20);
        vbat = (INT32U)(AD_Data_temp *3300) / 4096;
    }
    FM3_ADC0->ADCEN &= ~0x01;           //关闭ADC

	return vbat;
    
}

INT8U GetResetSrc(void)
{
	INT16U rststr;
	INT8U rstsrc;
	INT8U retcode=0;
	
	rststr = FM3_CRG->RST_STR;
	rstsrc = ((rststr&0x0100)>>7)|rststr;
	if(rstsrc&RESET_SOURCE_PONR)
		retcode |= RESET_SOURCE_PONR;
	else if(rstsrc&RESET_SOURCE_INITX)
		retcode |= RESET_SOURCE_INITX;
	else if(rstsrc&RESET_SOURCE_SRST)
		retcode |= RESET_SOURCE_SRST;
	else if(rstsrc&RESET_SOURCE_CSVR)
		retcode |= RESET_SOURCE_CSVR;
	else if(rstsrc&RESET_SOURCE_FCSR)
		retcode |= RESET_SOURCE_FCSR;
	return retcode;
}

void System_Init( void )
{
    CommInit( COMM1 );
    CommInit( COMM2 );
    CommInit( COMM3 );
    CommInit( COMM4 );
    CommInit( COMM5 );
    CommInit( COMM6 );
	CommInit( COMM7 );
}

/*************************************

三相表口线初始化

***********************************/
void GpioCfgJCY(void)
{
  
	//RUN_LED
	//GpioConfig(IO_PORT3, IO_PINxC, GPIO_MODE_OUTPUT);
	GpioConfig(IO_PORT3, IO_PINxD, GPIO_MODE_OUTPUT);
	//GpioSetOutput(IO_PORT3, IO_PINxC);
	//GpioClearOutput(IO_PORT3, IO_PINxD);
    GpioSetOutput(IO_PORT3, IO_PINxD);
	//single or three check
	GpioConfig(IO_PORT3, IO_PINxF, GPIO_MODE_INPUT);     //单相表识别口线   
    GpioConfig(IO_PORT3, IO_PINxE, GPIO_MODE_INPUT);    //三相表识别口线
	//KEY SET-UP DOWN
	GpioConfig(IO_PORTC, IO_PINxE, GPIO_MODE_INPUT);
	GpioConfig(IO_PORTC, IO_PINxF, GPIO_MODE_INPUT);
       
	//OPEN CHECK 1 AND 2
	GpioConfig(IO_PORT4, IO_PINx3, GPIO_MODE_INPUT);//开表盖检测口
	GpioConfig(IO_PORT4, IO_PINx4, GPIO_MODE_INPUT);//开接线盒盖检测口
	GpioConfig(IO_PORT7, IO_PINx4, GPIO_MODE_INPUT);
	//LCD BACKGROUND -LOW-LIGHTING
	GpioConfig(IO_PORT7, IO_PINx5, GPIO_MODE_OUTPUT);
	GpioSetOutput(IO_PORT7, IO_PINx5);
        
  // GpioClearOutput(IO_PORT7, IO_PINx5);
         
       
	//BATTERY CONTRUAL
	GpioConfig(IO_PORT2, IO_PINx1, GPIO_MODE_OUTPUT);
	//GpioClearOutput(IO_PORT2, IO_PINx1);
     GpioSetOutput(IO_PORT2, IO_PINx1);
	//FLASH PW LOW POWER
	GpioConfig(IO_PORTA, IO_PINx5, GPIO_MODE_OUTPUT);
	GpioClearOutput(IO_PORTA, IO_PINx5);
	//MULTI FUN
	GpioConfig(IO_PORT4, IO_PINx5, GPIO_MODE_OUTPUT);
	GpioSetOutput(IO_PORT4, IO_PINx5);
	// BJ-OPEN TZ-OPEN
	GpioConfig(IO_PORT4, IO_PINx1, GPIO_MODE_OUTPUT);
	GpioConfig(IO_PORT4, IO_PINx2,GPIO_MODE_OUTPUT);
	GpioSetOutput(IO_PORT4, IO_PINx1);
	GpioSetOutput(IO_PORT4, IO_PINx2);
        
        
  
}


void LCDCommPwmOut( INT8U bEnable )
{
	
	if( bEnable == ENABLE )
	{// 启动PWM
		// 
        INT16U  nTemp;
        INT16U  nHtemp;
        /* Set Basetimer ch11 IO port */    
        bFM3_GPIO_PFR3_PC = 0x01;            // Use P58   
        bFM3_GPIO_EPFR04_TIOA2E1 = 0x01;    // Use TIOA11_2  
        bFM3_GPIO_EPFR04_TIOA2E0 = 0x00;  
        
        /* Set IO Mode */
        FM3_BTIOSEL03->BTSEL0123 = (FM3_BTIOSEL03->BTSEL0123&0x08)|(BT_IO_MODE_0<<3);

        FM3_BT2_PWM->TMCR = 0;
        FM3_BT2_PWM->TMCR |= 0x0010;        //16-bit PWM timer
        FM3_BT2_PWM->TMCR |= 0X0800;
        FM3_BT2_PWM->TMCR2 = 0x00;
        FM3_BT2_PWM->STC = 0;
        nTemp = (APB2CLOCK/PW50KBAUND)-1;  /* Set baud rate */ 
        nHtemp = nTemp/2;
        FM3_BT2_PWM->PCSR = nTemp;
        FM3_BT2_PWM->PDUT = nHtemp;
        FM3_BT2_PWM->TMCR |= 0x0003;
	}
	else
	{// 关闭PWM
        FM3_BT2_PWM->TMCR &= ~0x0003;       //关闭PWM计数器
	}
}





INT32U TargetInit(void)
{
	FM3_GPIO->ADE = 0x00; 

	GpioModlCfg();
    //BULCDInit();
        
    //GpioCfgJCY();
    //LCDCommPwmOut(ENABLE);

	FlashPowerOffOn();
    OSTimeDlyHMSM(0, 0, 0, 100);

    if (FRAMInit())
    {
		return 1;
    }
	if (RTCInit())
	{
		return 1;
	}
  
    if (FlashInit())
    {
		return 1;
    }
	
	Timer3Init();
	__enable_irq();

	return FLASH_SUCC;
}

void SelAppAddr(INT32U addr)
{
	SCB->VTOR = addr ;          // 修改中断向量表地址
}





/********************************************************************************
* DEBUG  输出
*********************************************************************************/
#define     DebugComm       MFS_Ch4
int TextOut(const char* String)
{
    Uart_Puts(DebugComm,(char*)String);
    return 0;
}

void Uart_Puts(uint8_t u8Uart, char* String)
{
	while (*String != '\0')
	{ 
        if(*String == '\n') Uart_Putch(u8Uart,'\r');
        Uart_Putch(u8Uart,*String++);        /* send every char of string */
    }
}

void Uart_Putch(uint8_t u8Uart, char c)
{
    while(!( MFS_UART0StatusGetTXBusIdle(u8Uart) ));
    MFS_UART0TXOneData(u8Uart, (uint16_t)c);
        
}


/********************************************************************************
* 函数名: TimerInit
* 输　入:
* 输　出:
* 返  回:
* 功  能: 初始化Timer
*********************************************************************************/
void Timer3Init( void )
{
    BT_RTInitRegT reg;   
	INT32U time = 10;
    INT8U ch;
    
    /* Set requested channel,If 32-bit mode used, use CH 0,2,4,6 */
    ch = USER_BT_CH ; 
    /* Enable Bastimner Interrupt */
    NVIC_EnableIRQ(BTIM0_7_IRQn);
    /* Set initialization data */
    reg.Cycle = (INT32U)( (time*(APB1CLOCK/1000))/512 );   //Cycle = T*(m+1) 默认1ms
    reg.Clock = BT_CLK_DIV_512;   //BT_CLK_DIV_1: T 
    reg.TimerMode = USER_TIMER_MODE;
    reg.InputEdge = BT_TRG_DISABLE;
    reg.Polarity = BT_POLARITY_NORMAL;
    reg.Mode = BT_MODE_CONTINUOUS; 
    /* RT register initialization */
    BT_RTInit(ch,&reg); 
    /* Enable Interrupt */
    BT_RTEnableUnderflowInt(ch,Timer3Handler);

    /* Enable count operatoin */
    BT_RTEnableCount(ch);  
    /* Start triggered by software */
    BT_RTStartSoftTrig(ch);  
	         
}

void Timer3Handler(void)             
{
#if 0
static uint32_t Pre_DMA_TC = COMM_RX_BUF_SIZE - 1;
//    CntIntUnder++;
    INT32U CurDMATC1, CurDMATC2, cnt;
    cnt = 10;
    COMM_RING_BUF *pbuf;
    pbuf = GetCommBuf( COMM1 );			        /* Obtain pointer to communications channel */
    if( pbuf == NULL ) 
    {
        OSIntExit();
        return;
    }

    CurDMATC1 = (FM3_DMAC->DMACA0 & 0x0000FFFF);
    while(cnt--);
    CurDMATC2 = (FM3_DMAC->DMACA0 & 0x0000FFFF);
    if(CurDMATC1 == CurDMATC2)
    {
        if(CurDMATC1 != Pre_DMA_TC)
        {
            pbuf->RingBufRxCtr += (Pre_DMA_TC - CurDMATC1 );
            pbuf->RingBufRxInPtr += (Pre_DMA_TC - CurDMATC1 );
            Pre_DMA_TC = CurDMATC1;
        }

    }
    else if(CurDMATC1 > CurDMATC2)
    {
        return;        
    }
#endif   
    extern void IntProc(void);
    //CPU_SR      cpu_sr;
    
    OSIntEnter();
    //CPU_CRITICAL_ENTER();                                       /* Tell uC/OS-II that we are starting an ISR                */
    //OSIntNesting++;
    //CPU_CRITICAL_EXIT(); 
	    
	// 应用层中断调用
	IntProc();
    OSIntExit();                                                /* Tell uC/OS-II that we are leaving the ISR                */
	
	
}

/********************************************************************************
* 函数名: TimerStart
* 输　入:
		time -- 定时器计数间隔时间，默认为 1 毫秒
* 输　出:
* 返  回:
* 功  能: 启动定时器，用作精确定时
*********************************************************************************/
void TimerStart( INT32U time )
{
    BT_RTInitRegT reg;   
    uint8_t ch;

    /* Set requested channel,If 32-bit mode used, use CH 0,2,4,6 */
    ch = BT_CH_2 ; 
    /* Enable Bastimner Interrupt */
    NVIC_EnableIRQ(BTIM0_7_IRQn);
    /* Set initialization data */
    reg.Clock = BT_CLK_DIV_512;   //BT_CLK_DIV_1: T   (1/CLK)* DI
    reg.Cycle = (INT32U)( (time*(APB1CLOCK/1000))/512 );   //Cycle = T*(m+1) 默认1ms 
    reg.TimerMode = USER_TIMER_MODE;
    reg.InputEdge = BT_TRG_DISABLE;
    reg.Polarity = BT_POLARITY_NORMAL;
    reg.Mode = BT_MODE_CONTINUOUS; 
    /* RT register initialization */
    BT_RTInit(ch,&reg); 
    /* Disable Interrupt */
    BT_RTDisableUnderflowInt(ch);

    /* Enable count operatoin */
    BT_RTEnableCount(ch);  
    /* Start triggered by software */
    BT_RTStartSoftTrig(ch);   
}

/********************************************************************************
* 函数名: TimerStop
*输　入:
* 输　出:
* 返  回:
* 功  能: 停止定时器，返回当前计时值
*********************************************************************************/
INT32U TimerStop( void )
{
	INT8U ch;
	INT32U time,count,ms;
	
    /* Set requested channel,If 32-bit mode used, use CH 0,2,4,6 */
    ch = BT_CH_2 ; 	
    BT_RTDisableCount( ch );
    time = BT_RTGet32bitPCSR( ch );
	count = time - BT_RTGet32bitCount( ch );
	ms = count*512/(APB1CLOCK/1000) + 1;
	return ms;
}
 


/* 十进制数据 -> BCD码 */
INT8U Dec2BCD( INT8U data )
{
	INT8U i,j;
	if( data > 99 ) data =0;
	i = (data/10)<<4;
	j = data%10 ;
	return( i+j);
}

/* BCD码 -> 十进制数据 */
INT8U BCD2Dec( INT8U data )
{
	INT8U i,j;
	if( data>0x99 ) data=0;
	i = data &0x0F;
	j = (data &0xF0)>>4;
	return( i + j *10);
}




/*********************************************************************************************************
** 函数名称: 	DelayMS
** 功能描述: 	长软件延时
** 输　入: i :	延时参数，值越大时延时越久
**
** 输　出: 无
********************************************************************************************************/
void Delayus( INT32U dly )
{
    INT32U i;

    if (0 == dly)
    {
        return;
    }
    else if (dly > 10000)
    {
        dly = 10000;
    }
    while( dly-- )
    {
        for( i=0; i<11; i++ )
        {
          
        }
    }
}

void Delayms( INT32U dly )
{
    INT32U i;

    if (0 == dly)
    {
        return;
    }
    else if (dly > 10000)
    {
        dly = 10000;
    }
    while( dly-- )
    {
        for( i=0; i<8000; i++ )
        {
           
        }
    }
}








