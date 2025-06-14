/************************************************************************/
/*               (C) Fujitsu Semiconductor Europe GmbH (FSEU)           */
/*                                                                      */
/* The following software deliverable is intended for and must only be  */
/* used for reference and in an evaluation laboratory environment.      */
/* It is provided on an as-is basis without charge and is subject to    */
/* alterations.                                                         */
/* It is the user's obligation to fully test the software in its        */
/* environment and to ensure proper functionality, qualification and    */
/* compliance with component specifications.                            */
/*                                                                      */
/* In the event the software deliverable includes the use of open       */
/* source components, the provisions of the governing open source       */
/* license agreement shall apply with respect to such software          */
/* deliverable.                                                         */
/* FSEU does not warrant that the deliverables do not infringe any      */
/* third party intellectual property right (IPR). In the event that     */
/* the deliverables infringe a third party IPR it is the sole           */
/* responsibility of the customer to obtain necessary licenses to       */
/* continue the usage of the deliverable.                               */
/*                                                                      */
/* To the maximum extent permitted by applicable law FSEU disclaims all */
/* warranties, whether express or implied, in particular, but not       */
/* limited to, warranties of merchantability and fitness for a          */
/* particular purpose for which the deliverable is not designated.      */
/*                                                                      */
/* To the maximum extent permitted by applicable law, FSEU's liability  */
/* is restricted to intentional misconduct and gross negligence.        */
/* FSEU is not liable for consequential damages.                        */
/*                                                                      */
/* (V1.5)                                                               */
/************************************************************************/

#include "mcu.h"
#include "global.h"

/** \file system_mb9bf21x.c
 **
 ** FM3 system initialization functions
 ** All adjustments can be done in belonging header file.
 **
 ** History:
 ** 2011-07-07 V1.0 MWi original version
 ** 2012-05-30 V1.1 MWi Modified reading mask of "PLL_CTL2" register
 **                     in function "SystemCoreClockUpdate"
 ******************************************************************************/

//#define VECTOR_TABLE_ADDRESS        0x80000

/**
 ******************************************************************************
 ** System Clock Frequency (Core Clock) Variable according CMSIS
 ******************************************************************************/
uint32_t SystemCoreClock = __HCLK;

/**
 ******************************************************************************
 ** \brief  Update the System Core Clock with current core Clock retrieved from
 ** cpu registers.
 ** \param  none
 ** \return none
 ******************************************************************************/
void SystemCoreClockUpdate (void) {
  uint32_t masterClk;
  uint32_t u32RegisterRead; // Workaround variable for MISRA C rule conformance

  switch ((FM3_CRG->SCM_CTL >> 5) & 0x07) {
    case 0:                                 /* internal High-speed Cr osc.    */
      masterClk = __CLKHC;
      break;

    case 1:                                 /* external main osc.             */
      masterClk = __CLKMO;
      break;

    case 2:                                 /* PLL clock                      */
  // Workaround for preventing MISRA C:1998 Rule 46 (MISRA C:2004 Rule 12.2)
  // violation:
  //   "Unordered accesses to a volatile location"
      u32RegisterRead = (__CLKMO  * (((FM3_CRG->PLL_CTL2) & 0x3F) + 1));
      masterClk = (u32RegisterRead / (((FM3_CRG->PLL_CTL1 >> 4) & 0x0F) + 1));
      break;

    case 4:                                 /* internal Low-speed CR osc.     */
      masterClk = __CLKLC;
      break;

    case 5:                                 /* external Sub osc.              */
      masterClk = __CLKSO;
      break;

    default:
      masterClk = 0Ul;
      break;
  }

  switch (FM3_CRG->BSC_PSR & 0x07) {
    case 0:
      SystemCoreClock = masterClk;
      break;

    case 1:
      SystemCoreClock = masterClk / 2;
      break;

    case 2:
      SystemCoreClock = masterClk / 3;
      break;

    case 3:
      SystemCoreClock = masterClk / 4;
      break;

    case 4:
      SystemCoreClock = masterClk / 6;
      break;

    case 5:
      SystemCoreClock = masterClk /8;
      break;

    case 6:
      SystemCoreClock = masterClk /16;
      break;

    default:
      SystemCoreClock = 0Ul;
      break;
  }

}

/**
 ******************************************************************************
 ** \brief  Setup the microcontroller system. Initialize the System and update
 ** the SystemCoreClock variable.
 **
 ** \param  none
 ** \return none
 ******************************************************************************/
void SystemInit (void) {

  static uint32_t u32IoRegisterRead;  // Workaround variable for MISRA C rule conformance

#if (HWWD_DISABLE)                                 /* HW Watchdog Disable */
  FM3_HWWDT->WDG_LCK = 0x1ACCE551;                 /* HW Watchdog Unlock */
  FM3_HWWDT->WDG_LCK = 0xE5331AAE;
  FM3_HWWDT->WDG_CTL = 0;                          /* HW Watchdog stop */
#endif
  
//    WatchDogEnable(1);
//	Delayms(30);
//	WatchDogClear(1);
//	Delayms(30);
//	WatchDogClear(1);
//	Delayms(30);
//	WatchDogClear(1);
  
#if (CLOCK_SETUP)                                   /* Clock Setup */
  FM3_CRG->BSC_PSR   = BSC_PSR_Val;                /* set System Clock presacaler */
  FM3_CRG->APBC0_PSR = APBC0_PSR_Val;              /* set APB0 presacaler */
  FM3_CRG->APBC1_PSR = APBC1_PSR_Val;              /* set APB1 presacaler */
  FM3_CRG->APBC2_PSR = APBC2_PSR_Val;              /* set APB2 presacaler */
  FM3_CRG->SWC_PSR   = SWC_PSR_Val | (1UL << 7);   /* set SW Watchdog presacaler */
  FM3_CRG->TTC_PSR   = TTC_PSR_Val;                /* set Trace Clock presacaler */

  FM3_CRG->CSW_TMR   = CSW_TMR_Val;                /* set oscillation stabilization wait time */

  if (SCM_CTL_Val & (1UL << 1)) {                    /* Main clock oscillator enabled ? */
    FM3_CRG->SCM_CTL |= (1UL << 1);                /* enable main oscillator */
    while (!(FM3_CRG->SCM_STR & (1UL << 1)));      /* wait for Main clock oscillation stable */
  }

  if (SCM_CTL_Val & (1UL << 3)) {                    /* Sub clock oscillator enabled ? */
    FM3_CRG->SCM_CTL |= (1UL << 3);                /* enable sub oscillator */
    while (!(FM3_CRG->SCM_STR & (1UL << 3)));      /* wait for Sub clock oscillation stable */
  }

  FM3_CRG->PSW_TMR   = PSW_TMR_Val;                /* set PLL stabilization wait time */
  FM3_CRG->PLL_CTL1  = PLL_CTL1_Val;               /* set PLLM and PLLK */
  FM3_CRG->PLL_CTL2  = PLL_CTL2_Val;               /* set PLLN          */

  if (SCM_CTL_Val & (1UL << 4)) {                    /* PLL enabled ? */
    FM3_CRG->SCM_CTL  |= (1UL << 4);               /* enable PLL */
    while (!(FM3_CRG->SCM_STR & (1UL << 4)));      /* wait for PLL stable */
  }

  FM3_CRG->SCM_CTL  |= (SCM_CTL_Val & 0xE0);       /* Set Master Clock switch */

  // Workaround for preventing MISRA C:1998 Rule 46 (MISRA C:2004 Rule 12.2)
  // violations:
  //   "Unordered reads and writes to or from same location" and
  //   "Unordered accesses to a volatile location"
  do
  {
    u32IoRegisterRead = (FM3_CRG->SCM_CTL & 0xE0);
  }while ((FM3_CRG->SCM_STR & 0xE0) != u32IoRegisterRead);
#endif // (CLOCK_SETUP)

#if (CR_TRIM_SETUP)
  /* CR Trimming Data  */
  if( 0x000003FF != (FM3_FLASH_IF->CRTRMM & 0x000003FF) )
  {
    /* UnLock (MCR_FTRM) */
    FM3_CRTRIM->MCR_RLR = 0x1ACCE554;
    /* Set MCR_FTRM */
    FM3_CRTRIM->MCR_FTRM = FM3_FLASH_IF->CRTRMM;
    /* Lock (MCR_FTRM) */
    FM3_CRTRIM->MCR_RLR = 0x00000000;
  }
#endif // (CR_TRIM_SETUP)
  
//  SCB->VTOR = VECTOR_TABLE_ADDRESS ;          // 修改
}

void  hard_fault_handler_c(unsigned int * hardfault_args)
{   
#if 0
    unsigned int stacked_r0,stacked_r1,stacked_r2,stacked_r3; 
	unsigned int stacked_r12,stacked_lr, stacked_pc, stacked_psr; 
	INT8U g_cDataBuf[256] = {0};
    int offset = 0;
	stacked_r0 = ((unsigned long) hardfault_args[0]); 
	stacked_r1 = ((unsigned long) hardfault_args[1]); 
	stacked_r2 = ((unsigned long) hardfault_args[2]); 
	stacked_r3 = ((unsigned long) hardfault_args[3]); 
	stacked_r12 = ((unsigned long) hardfault_args[4]); 
	stacked_lr = ((unsigned long) hardfault_args[5]); 
	stacked_pc = ((unsigned long) hardfault_args[6]); 
	stacked_psr = ((unsigned long) hardfault_args[7]); 
	offset += sprintf(g_cDataBuf + offset ,"The task pri id = 0x%0.8x\n", OSPrioCur);
 
	offset += sprintf(g_cDataBuf + offset,"SP = 0x%0.8x\n", hardfault_args);		//堆栈地址
	
    offset += sprintf(g_cDataBuf + offset,"R0 = 0x%0.8x\n", stacked_r0); 
	
	offset += sprintf(g_cDataBuf + offset ,"R1 = 0x%0.8x\n", stacked_r1);
	
    offset += sprintf(g_cDataBuf + offset,"R2 = 0x%0.8x\n", stacked_r2); 
	
    offset += sprintf(g_cDataBuf + offset,"R3 = 0x%0.8x\n", stacked_r3);

	offset += sprintf(g_cDataBuf + offset,"R12 = 0x%0.8x\n", stacked_r12); 
	
	offset += sprintf(g_cDataBuf + offset,"LR = 0x%0.8x\n", stacked_lr);

	offset += sprintf(g_cDataBuf + offset,"PC = 0x%0.8x\n", stacked_pc); 
	
	offset += sprintf(g_cDataBuf + offset,"PSR = 0x%0.8x\n", stacked_psr);
	WriteComm(3 ,g_cDataBuf ,sizeof(g_cDataBuf));
#endif

	/* 复位 */

	//__set_FAULTMASK(1);          			// 关闭所有中断
	//NVIC_SystemReset();        			    // 复位 
    return ;

} 



