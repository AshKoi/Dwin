/*
**********************************************************************
*
*                  监控系统8.0__嵌入式系统构件        
*
**********************************************************************

----------------------------------------------------------------------
文 件 名	: at91_pit.c
文件描述	: PIT周期间断定时器应用函数
创建作者	: 
创建日期	: 2009-9-9
======================================================================
版本历史	: 
----------------------------------------------------------------------
修改日期	: 
修改内容	: 
======================================================================
*/
#include "sys_includes.h"

uint8 Os_Tick_Cnt;							// 10ms时钟节拍次数统计
extern unsigned int web_os_timeout;
/*
**********************************************************************
*	函数名称:	at91_pit_time_manage
*	函数功能:	系统中断告警机制时间管理(告警与数据批采使用)
*	入口参数:	无
*	出口参数:	无
**********************************************************************
*/
void at91_pit_time_manage(void)
{
	Os_Tick_Cnt++;
	// 1s时间到
	if(Os_Tick_Cnt == OS_TICKS_PER_SEC)
	{
		Os_Tick_Cnt = 0;
		if(web_os_timeout > 0)
		{
			web_os_timeout--;
		}
	}

}

/*
**********************************************************************
*	函数名称:	at91_pit_tick_isr
*	函数功能:	PIT提供系统时钟节拍中断服务函数
*	入口参数:	无
*	出口参数:	无
**********************************************************************
*/
void at91_pit_tick_isr(void)
{	
	if((AT91PS_PITC)AT91C_BASE_PITC->PITC_PISR)
    {
		// 读取AIC_IVR寄存器，将会清除当前中断位，并将PITS复位
		 AT91C_BASE_PITC->PITC_PIVR;
		 OSTimeTick();
	  	 at91_pit_time_manage();
	
	}	
	 
}	
/*
**********************************************************************
*	函数名称:	at91_pit_init
*	函数功能:	PIT定时器初始化
*				This function is called to initialize uC/OS-II's tick 
*				source which uses the PIT(typically a timer generating
*				interrupts every 1 to 100 mS).
*	Note(s)     : 1) PIT Interrupt frequency:
*
*                         MCLK        1
*               Freq =    ---- * -----------
*                          16     (PIV + 1)
*
*                         MCLK      1
*               PIV  =  ( ---- * ------ ) - 1
*                          16     Freq
*
*               Where:
*                    MCLK = 48 MHz (i.e 48,000,000)
*                    Freq = Desired frequency (i.e. OS_TICKS_PER_SEC) 
*	入口参数:	None.
*	出口参数:	None.
**********************************************************************
*/
void at91_pit_init(void)
{
    uint16 piv;

    //使能电源管理器
    PMC_EnablePeripheral( AT91C_ID_SYS );
	
	//配置PIT中断源
	AIC_ConfigureIT(AT91C_ID_SYS,AT91_AIC_ISR_LEVEL_PIT,at91_aic_sysirq_handle);
	
	//开系统中断
	AIC_EnableIT(AT91C_ID_SYS);

	//设置定时器溢出值PIV = 0x000074FF	10ms中断一次
	piv = (BSP_MCK/16/OS_TICKS_PER_SEC)-1;
	
	//开PIT中断 PITC_PIMR = 0x030074FF	使能寄存器	允许触发中断
  	AT91C_BASE_PITC->PITC_PIMR = AT91C_PITC_PITEN | AT91C_PITC_PITIEN | piv;
}
