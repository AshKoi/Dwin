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
/** \file UsbInit.c
 **
 ** USB Registers File
 **
 ** History:
 **   - 2010-09-24  1.0  MSc  MCU Specific initializations FR80, FM3
 **   - 2011-03-30  1.1  MSc  Some Corrections
 **   - 2011-04-28  1.2  MSc  Support for MB91F662 template
 **   - 2011-08-12  1.3  MSc  Support for MB9AF3xx series
 **   - 2011-11-14  1.4  MSc  Support for MB9BF61x series
 **   - 2012-01-18  1.5  MSc  Support for MB9BF51x series
 **   - 2012-02-03  1.6  MSc  Support for MB9BF31xK series
 *************************************************************************/

#include "UsbInit.h"
#if (USBMCUTYPE == USBMCU_MB91665) || (USBMCUTYPE == USBMCU_MB91660)
#define UsbInit() UsbInit_FR80()
void UsbInit_FR80(void)
{
  /****** Flash access wait setting ******/
  IO_FCTLR.bit.FWC = 1;
  
  /****** Clock Set ******/
  IO_CLK.IO_DIVR0.byte = 0x00; /* BCLK:1divid */
  IO_CLK.IO_DIVR1.byte = 0x80; /* TCLK:disable */
  IO_CLK.IO_DIVR2.byte = 0x00; /* PCLK:2divid */

  /****** USB Clock Set ******/
  IO_UCCR.byte		 = 0x00; /* USBCLK:disable */
  #ifdef USBEN
  USBEN = 0x04;  /* USBEN = 0 */
  #else
  USBEN_ = 0x04;  /* USBEN = 0 */
  #endif
  IO_UCCR.byte		 = 0x06; /* USBCLK:PLL-1divid */
  IO_UCCR.byte		 = 0x07; /* USBCLK:enable */
  #ifdef USBEN
  USBEN = 0x05;  /* USBEN = 1 */
  #else
  USBEN_ = 0x05;  /* USBEN = 1 */
  #endif
  IO_CLKG.IO_PLLCR.hword = 0x27F0; /* CLK:8_PLL, USB:24_PLL */

  IO_CLKG.IO_CSELR.bit.PCEN = 1; /* enable PLL */
  while( 1 != IO_CLKG.IO_CMONR.bit.PCRDY ){
		;
  }

  IO_CLKG.IO_CSELR.bit.CKS = 2; /* Clock Select(MCLK => PLL) */
  while( 2 != IO_CLKG.IO_CMONR.bit.CKM ){
		;
  }
}  
#endif

void UsbInit(void)
{
    volatile unsigned char dummy;

    #ifdef USE_USB1
        NVIC_ClearPendingIRQ(USB1F_IRQn); 
        NVIC_EnableIRQ(USB1F_IRQn);
        NVIC_SetPriority(USB1F_IRQn,1);
        NVIC_ClearPendingIRQ(USB1F_USB1H_IRQn); 
        NVIC_EnableIRQ(USB1F_USB1H_IRQn);
        NVIC_SetPriority(USB1F_USB1H_IRQn,1);
    #else
        NVIC_ClearPendingIRQ(USBF_IRQn); 
        NVIC_EnableIRQ(USBF_IRQn);
        NVIC_SetPriority(USBF_IRQn,1);
        NVIC_ClearPendingIRQ(USBF_USBH_IRQn); 
        NVIC_EnableIRQ(USBF_USBH_IRQn);
        NVIC_SetPriority(USBF_USBH_IRQn,1);
    #endif
    /* USB */
    bFM3_GPIO_SPSR_USB1C = 1;       // USB1C = 1 (use USB pins), SUBXC = 0 (Do not use subclock)
        
    FM3_GPIO->EPFR00 |= 0x00002000;  /* USB1PE=1 */
  
    /* USB Pull-up : P61 as UHCONX */
//    FM3_GPIO->PFR6 = 0x0002;
    
    /* set USB clock */
    
    #if ((USBMCUTYPE == USBMCU_MB9B500) || (USBMCUTYPE == USBMCU_MB9A310))
    FM3_USBCLK->UCCR = 0x00;
    FM3_USBCLK->USBEN = 0x04;  /* USBEN = 0 */
    #endif
    #if (USBMCUTYPE == USBMCU_MB9B218S)
    FM3_USBETHERNETCLK->UCCR = 0x00;
    FM3_USBETHERNETCLK->USBEN1 = 0x00;  /* USBEN = 0 */
    #endif    
    
    
    #if (USBMCUTYPE == USBMCU_MB9B500) 
        FM3_USBCLK->UPCR1 = 0x00; /* PLL Disable */
        FM3_USBCLK->UPCR2 = 0x03; /* wait 1ms */
        FM3_USBCLK->UPCR3 = 0x00; /* K=1,M=1 */
        FM3_USBCLK->UPCR4 = 0x17; /* N=24 : 4MHz x 24 = 96MHz */
    #endif
    #if ((USBMCUTYPE == USBMCU_MB9A310) || (USBMCUTYPE == USBMCU_MB9B510))
        FM3_USBCLK->UPCR1 = 0x00; /* PLL Disable */
        FM3_USBCLK->UPCR2 = 0x03; /* wait 1ms */
        FM3_USBCLK->UPCR3 = 0x00; /* K=1,M=1 */
        FM3_USBCLK->UPCR4 = 59;/* N=60 : 4MHz */
        FM3_USBCLK->UPCR5 = 4; /* M=5 : 4MHz */
    #endif
    #if (USBMCUTYPE == USBMCU_MB9B218S)
        FM3_USBETHERNETCLK->UPCR1 = 0x00; /* PLL Disable */
        FM3_USBETHERNETCLK->UPCR2 = 0x03; /* wait 1ms */
        FM3_USBETHERNETCLK->UPCR3 = 0x00; /* K=0 */
        FM3_USBETHERNETCLK->UPCR4 = 59;/* N=60 : 4MHz */
        FM3_USBETHERNETCLK->UPCR5 = 4; /* M=5 : 4MHz */
    #endif    
    #if ((USBMCUTYPE == USBMCU_MB9B500) || (USBMCUTYPE == USBMCU_MB9A310) || (USBMCUTYPE == USBMCU_MB9B510))
    FM3_USBCLK->UPCR1 = 0x01; /* PLL Enable */
    while(!FM3_USBCLK->UP_STR);
  
    FM3_USBCLK->UCCR = 0x03;  /* UCSEL=1 , UCEN=1 */
    /* wait over 5clock */
    dummy = FM3_USBCLK->UCCR;
    dummy = FM3_USBCLK->UCCR;
    dummy = FM3_USBCLK->UCCR;
    dummy = FM3_USBCLK->UCCR;
    dummy = FM3_USBCLK->UCCR;
    FM3_USBCLK->USBEN = 0x05;  /* USBEN = 1 */
    #endif
    
    #if (USBMCUTYPE == USBMCU_MB9B218S)
    FM3_USBETHERNETCLK->UPCR1 = 0x01; /* PLL Enable */
    while(!FM3_USBETHERNETCLK->UP_STR);
    bFM3_USBETHERNETCLK_UCCR_UCSEL0 = 1;
    bFM3_USBETHERNETCLK_UCCR_UCSEL1 = 0;
    bFM3_USBETHERNETCLK_UCCR_UCEN1 = 1;
    /* wait over 5clock */
    dummy = FM3_USBETHERNETCLK->UCCR;
    dummy = FM3_USBETHERNETCLK->UCCR;
    dummy = FM3_USBETHERNETCLK->UCCR;
    dummy = FM3_USBETHERNETCLK->UCCR;
    dummy = FM3_USBETHERNETCLK->UCCR;
    FM3_USBETHERNETCLK->USBEN1 = 0x01;  /* USBEN = 1 */
    #endif
    
}


#if ((USE_USB_FUNCTION == 1) && (USE_USB_HOST == 1))
uint8_t UsbInit_HostFunctionSwitch(usb_event_t* stcEvent)
{
    if (stcEvent->u8Event == USB_EVENT_CONNECT)
    {

    }
    if (stcEvent->u8Event == USB_EVENT_DISCONNECT)
    {
       UsbHost_Init();
    }
}

void UsbInit_SetUsbHostFunctionSwitching()
{
    UsbFunction_SetEventHandler(USB_EVENT_TARGET_MAINAPP,USB_EVENT_CONNECT,0,UsbInit_HostFunctionSwitch);
}

#endif /* (USE_USB_FUNCTION == 1) && (USE_USB_HOST == 1) */

#if (USBMCUTYPE == USBMCU_MB91665) || (USBMCUTYPE == USBMCU_MB91660)
__interrupt void UsbInit_HostFunctionIsr(void)
{
    if (IS_SET(HCNT, _HCNT_HOST))
    {
        #if (USE_USB_HOST == 1)
            UsbHost_ISR();
        #endif
    }
    else
    {
        #if (USE_USB_FUNCTION == 1)
            UsbFunction_StatusControlIsr();
        #endif
    }
}
#endif


#if IS_USBMCU_FM3
    #ifdef USE_USB1
        void USB1_Handler (void)
    #else
        void USB_Handler (void)
    #endif
{
    if (IS_SET(HCNT, _HCNT_HOST))
    {
        #if (USE_USB_HOST == 1)
            UsbHost_ISR();
        #endif
    }
    else
    {
        #if (USE_USB_FUNCTION == 1)
            UsbFunction_StatusControlIsr();
        #endif
    }
}
#endif
