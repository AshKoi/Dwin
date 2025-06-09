/*
**********************************************************************
*
*                  ���ϵͳ8.0__Ƕ��ʽϵͳ����        
*
**********************************************************************

----------------------------------------------------------------------
�� �� ��	: at91_pit.c
�ļ�����	: PIT���ڼ�϶�ʱ��Ӧ�ú���
��������	: 
��������	: 2009-9-9
======================================================================
�汾��ʷ	: 
----------------------------------------------------------------------
�޸�����	: 
�޸�����	: 
======================================================================
*/
#include "sys_includes.h"

uint8 Os_Tick_Cnt;							// 10msʱ�ӽ��Ĵ���ͳ��
extern unsigned int web_os_timeout;
/*
**********************************************************************
*	��������:	at91_pit_time_manage
*	��������:	ϵͳ�жϸ澯����ʱ�����(�澯����������ʹ��)
*	��ڲ���:	��
*	���ڲ���:	��
**********************************************************************
*/
void at91_pit_time_manage(void)
{
	Os_Tick_Cnt++;
	// 1sʱ�䵽
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
*	��������:	at91_pit_tick_isr
*	��������:	PIT�ṩϵͳʱ�ӽ����жϷ�����
*	��ڲ���:	��
*	���ڲ���:	��
**********************************************************************
*/
void at91_pit_tick_isr(void)
{	
	if((AT91PS_PITC)AT91C_BASE_PITC->PITC_PISR)
    {
		// ��ȡAIC_IVR�Ĵ��������������ǰ�ж�λ������PITS��λ
		 AT91C_BASE_PITC->PITC_PIVR;
		 OSTimeTick();
	  	 at91_pit_time_manage();
	
	}	
	 
}	
/*
**********************************************************************
*	��������:	at91_pit_init
*	��������:	PIT��ʱ����ʼ��
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
*	��ڲ���:	None.
*	���ڲ���:	None.
**********************************************************************
*/
void at91_pit_init(void)
{
    uint16 piv;

    //ʹ�ܵ�Դ������
    PMC_EnablePeripheral( AT91C_ID_SYS );
	
	//����PIT�ж�Դ
	AIC_ConfigureIT(AT91C_ID_SYS,AT91_AIC_ISR_LEVEL_PIT,at91_aic_sysirq_handle);
	
	//��ϵͳ�ж�
	AIC_EnableIT(AT91C_ID_SYS);

	//���ö�ʱ�����ֵPIV = 0x000074FF	10ms�ж�һ��
	piv = (BSP_MCK/16/OS_TICKS_PER_SEC)-1;
	
	//��PIT�ж� PITC_PIMR = 0x030074FF	ʹ�ܼĴ���	�������ж�
  	AT91C_BASE_PITC->PITC_PIMR = AT91C_PITC_PITEN | AT91C_PITC_PITIEN | piv;
}
