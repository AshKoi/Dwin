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

/** \file UsbInit.h
 **
 ** USB Registers File
 **
 ** History:
 **   - 2011-02-25  1.0  MSc  MCU Specific initializations FR80, FM3
 **   - 2011-03-30  1.1  MSc  Some Corrections
 **   - 2011-04-28  1.2  MSc  Support for MB91F662 template
 **   - 2011-08-12  1.3  MSc  Support for MB9AF3xx series
 **   - 2011-11-14  1.4  MSc  Support for MB9BF61x series
 **   - 2012-01-18  1.5  MSc  Support for MB9BF51x series
 **   - 2012-02-03  1.6  MSc  Support for MB9BF31xK series
 *************************************************************************/

#ifndef __USBINIT_H__
#define __USBINIT_H__
#include "mcu.h"
#include "base_type.h"
#include "UsbFunction.h"
#define USBMCU_MB9BF218S

#define USBMCU_MB96330    1
#define USBMCU_MB91660    2
#define USBMCU_MB91665    3
#define USBMCU_MB9B500    4
#define USBMCU_MB9A310    5
#define USBMCU_MB9B218S    6
#define USBMCU_MB9B510    7

#define WORKSPACE_SOFTUNE 1
#define WORKSPACE_IAR     2
#define WORKSPACE_KEIL    3


#if ((__TID__ >> 4) & 0x0F) == 6   // Cortex-M3
    
#endif

#if __CC_ARM == 1
    #define USED_WORKSPACE WORKSPACE_KEIL  
#endif

#if defined(_MB9AF316N_H_) || defined(_MB9AF315N_H_) || defined(_MB9AF314L_H_) || defined(_MB9AF316L_H_) || defined(_MB9AF315L_H_)
    #define USBMCUTYPE USBMCU_MB9A310
#endif



#if defined(_MB9BF218S_H_)
    #ifndef USE_USB1
        #define USE_USB1
    #endif
    #define USBMCUTYPE USBMCU_MB9B218S
    #define IS_USBMCU_FM3_USB1
#endif


#define IS_USBMCU_FM3  ((USBMCUTYPE == USBMCU_MB9B500) || (USBMCUTYPE == USBMCU_MB9A310) || (USBMCUTYPE == USBMCU_MB9B218S) || (USBMCUTYPE == USBMCU_MB9B510))

#ifdef __IAR_SYSTEMS_ICC__
    #define USED_WORKSPACE WORKSPACE_IAR
#endif



#ifndef USBMCUTYPE
    #error USB Stack does not recognize the used MCU
#endif

#ifndef USED_WORKSPACE
    #error USB Stack does not recognize the used Workspace
#endif

#if (USED_WORKSPACE != WORKSPACE_SOFTUNE) 
    #ifndef __interrupt
        #define __interrupt
    #endif
    #ifndef __wait_nop
        #define __wait_nop()
    #endif
#endif



void UsbInit(void);

#if ((USE_USB_FUNCTION == 1) && (USE_USB_HOST == 1))
  uint8_t UsbInit_HostFunctionSwitch(struct stc_usb_event* stcEvent);
  void UsbInit_SetUsbHostFunctionSwitching();
#endif

__interrupt void UsbInit_HostFunctionIsr(void);
  #ifdef IS_USBMCU_FM3_USB0  
        #define UDCC FM3_USB0->UDCC
        #define EP0C FM3_USB0->EP0C
        #define EP1C FM3_USB0->EP1C
        #define EP2C FM3_USB0->EP2C
        #define EP3C FM3_USB0->EP3C
        #define EP4C FM3_USB0->EP4C
        #define EP5C FM3_USB0->EP5C
        #define TMSP FM3_USB0->TMSP
        #define UDCS FM3_USB0->UDCS
        #define UDCIE FM3_USB0->UDCIE
        #define EP0IS FM3_USB0->EP0IS
        #define EP0OS FM3_USB0->EP0OS
        #define EP1S FM3_USB0->EP1S
        #define EP2S FM3_USB0->EP2S
        #define EP3S FM3_USB0->EP3S
        #define EP4S FM3_USB0->EP4S
        #define EP5S FM3_USB0->EP5S
        #define EP0DT  FM3_USB0->EP0DT
        #define EP0DTL FM3_USB0->EP0DTL
        #define EP0DTH FM3_USB0->EP0DTH
        #define EP1DT  FM3_USB0->EP1DT
        #define EP1DTL FM3_USB0->EP1DTL
        #define EP1DTH FM3_USB0->EP1DTH
        #define EP2DT  FM3_USB0->EP2DT
        #define EP2DTL FM3_USB0->EP2DTL
        #define EP2DTH FM3_USB0->EP2DTH
        #define EP3DT  FM3_USB0->EP3DT
        #define EP3DTL FM3_USB0->EP3DTL
        #define EP3DTH FM3_USB0->EP3DTH
        #define EP4DT  FM3_USB0->EP4DT
        #define EP4DTL FM3_USB0->EP4DTL
        #define EP4DTH FM3_USB0->EP4DTH
        #define EP5DT  FM3_USB0->EP5DT
        #define EP5DTL FM3_USB0->EP5DTL
        #define EP5DTH FM3_USB0->EP5DTH
        #define HCNT  FM3_USB0->HCNT
        #define HIRQ  FM3_USB0->HIRQ
        #define HERR  FM3_USB0->HERR
        #define HSTATE  FM3_USB0->HSTATE
        #define HFCOMP  FM3_USB0->HFCOMP
        #define HRTIMER0  FM3_USB0->HRTIMER0
        #define HRTIMER1  FM3_USB0->HRTIMER1
        #define HRTIMER2  FM3_USB0->HRTIMER2
        #define HADR  FM3_USB0->HADR
        #define HEOF (FM3_USB0->HEOF)
        #define HFRAME  FM3_USB0->HFRAME
        #define HTOKEN  FM3_USB0->HTOKEN
  #endif

  #ifdef IS_USBMCU_FM3_USB1  
        #define UDCC FM3_USB1->UDCC
        #define EP0C FM3_USB1->EP0C
        #define EP1C FM3_USB1->EP1C
        #define EP2C FM3_USB1->EP2C
        #define EP3C FM3_USB1->EP3C
        #define EP4C FM3_USB1->EP4C
        #define EP5C FM3_USB1->EP5C
        #define TMSP FM3_USB1->TMSP
        #define UDCS FM3_USB1->UDCS
        #define UDCIE FM3_USB1->UDCIE
        #define EP0IS FM3_USB1->EP0IS
        #define EP0OS FM3_USB1->EP0OS
        #define EP1S FM3_USB1->EP1S
        #define EP2S FM3_USB1->EP2S
        #define EP3S FM3_USB1->EP3S
        #define EP4S FM3_USB1->EP4S
        #define EP5S FM3_USB1->EP5S
        #define EP0DT  FM3_USB1->EP0DT
        #define EP0DTL FM3_USB1->EP0DTL
        #define EP0DTH FM3_USB1->EP0DTH
        #define EP1DT  FM3_USB1->EP1DT
        #define EP1DTL FM3_USB1->EP1DTL
        #define EP1DTH FM3_USB1->EP1DTH
        #define EP2DT  FM3_USB1->EP2DT
        #define EP2DTL FM3_USB1->EP2DTL
        #define EP2DTH FM3_USB1->EP2DTH
        #define EP3DT  FM3_USB1->EP3DT
        #define EP3DTL FM3_USB1->EP3DTL
        #define EP3DTH FM3_USB1->EP3DTH
        #define EP4DT  FM3_USB1->EP4DT
        #define EP4DTL FM3_USB1->EP4DTL
        #define EP4DTH FM3_USB1->EP4DTH
        #define EP5DT  FM3_USB1->EP5DT
        #define EP5DTL FM3_USB1->EP5DTL
        #define EP5DTH FM3_USB1->EP5DTH
        #define HCNT  FM3_USB1->HCNT
        #define HIRQ  FM3_USB1->HIRQ
        #define HERR  FM3_USB1->HERR
        #define HSTATE  FM3_USB1->HSTATE
        #define HFCOMP  FM3_USB1->HFCOMP
        #define HRTIMER0  FM3_USB1->HRTIMER0
        #define HRTIMER1  FM3_USB1->HRTIMER1
        #define HRTIMER2  FM3_USB1->HRTIMER2
        #define HADR  FM3_USB1->HADR
        #define HEOF (FM3_USB1->HEOF)
        #define HFRAME  FM3_USB1->HFRAME
        #define HTOKEN  FM3_USB1->HTOKEN
  #endif


#define _UDCC_RST	(1 << 7) // Function reset bit
#define _UDCC_RESUM	(1 << 6) // Resume setting bit
#define _UDCC_HCONX	(1 << 5) // Host connection bit
#define _UDCC_USTP	(1 << 4) // USB operation clock stop bit
#define _UDCC_RFBK	(1 << 1) // Data toggle mode bit (Rate feedback mode)
#define _UDCC_PWC	(1 << 0) // Power supply control bit

#define _EP0C_PKS0	0x7F     // EP0 packet size setting bits
#define _EP0C_STAL  (1 << 9) //STALL set bit

#define _EP1C_PKS1	0x1FF     // EP1 packet size setting bits
#define _EP1C_STAL  (1 << 9)  // STALL set bit
#define _EP1C_NULE  (1 << 10) // NULL automatic transfer enable bit
#define _EP1C_DMAE  (1 << 11) // DMA automatic transfer enable bit
#define _EP1C_DIR   (1 << 12) // Endpoint direction selection bit
#define _EP1C_TYPE  0x6000    // Endpoint transfer type bit
#define _EP1C_EPEN  (1 << 15) // Endpoint Enable bit

#define _EP2C_PKS2	0xFF      // EP2 packet size setting bits
#define _EP2C_STAL  (1 << 9)  // STALL set bit
#define _EP2C_NULE  (1 << 10) // NULL automatic transfer enable bit
#define _EP2C_DMAE  (1 << 11) // DMA automatic transfer enable bit
#define _EP2C_DIR   (1 << 12) // Endpoint direction selection bit
#define _EP2C_TYPE  0x6000    // Endpoint transfer type bit
#define _EP2C_EPEN  (1 << 15) // Endpoint Enable bit

#define _EP3C_PKS3	0xFF      // EP3 packet size setting bits
#define _EP3C_STAL  (1 << 9)  // STALL set bit
#define _EP3C_NULE  (1 << 10) // NULL automatic transfer enable bit
#define _EP3C_DMAE  (1 << 11) // DMA automatic transfer enable bit
#define _EP3C_DIR   (1 << 12) // Endpoint direction selection bit
#define _EP3C_TYPE  0x6000    // Endpoint transfer type bit
#define _EP3C_EPEN  (1 << 15) // Endpoint Enable bit

#define _EP4C_PKS4	0xFF      // EP4 packet size setting bits
#define _EP4C_STAL  (1 << 9)  // STALL set bit
#define _EP4C_NULE  (1 << 10) // NULL automatic transfer enable bit
#define _EP4C_DMAE  (1 << 11) // DMA automatic transfer enable bit
#define _EP4C_DIR   (1 << 12) // Endpoint direction selection bit
#define _EP4C_TYPE  0x6000    // Endpoint transfer type bit
#define _EP4C_EPEN  (1 << 15) // Endpoint Enable bit

#define _EP5C_PKS5	0xFF      // EP  packet size setting bits
#define _EP5C_STAL  (1 << 9)  // STALL set bit
#define _EP5C_NULE  (1 << 10) // NULL automatic transfer enable bit
#define _EP5C_DMAE  (1 << 11) // DMA automatic transfer enable bit
#define _EP5C_DIR   (1 << 12) // Endpoint direction selection bit
#define _EP5C_TYPE  0x6000    // Endpoint transfer type bit
#define _EP5C_EPEN  (1 << 15) // Endpoint Enable bit

#define _EPC_TYPE_BULK       0x4000
#define _EPC_TYPE_INTERRUPT  0x6000

#define _UDCS_CONF	(1 << 0) // Configuration detection bit
#define _UDCS_SETP	(1 << 1) // Setup stage detection bit
#define _UDCS_WKUP	(1 << 2) // Wake-up detection bit
#define _UDCS_BRST	(1 << 3) // Bus reset detection bit
#define _UDCS_SOF	(1 << 4) // SOF reception detection bit
#define _UDCS_SUSP	(1 << 5) // Suspend detection bit

#define _UDCIE_CONFIE (1 << 0) // Configuration interrupt enable bit
#define _UDCIE_CONFN	 (1 << 1) // Configuration number bit
#define _UDCIE_WKUPIE (1 << 2) // Wake-up interrupt enable bit
#define _UDCIE_BRSTIE (1 << 3) // Bus reset interrupt enable bit
#define _UDCIE_SOFIE  (1 << 4) // SOF receive interrupt enable bit
#define _UDCIE_SUSPIE (1 << 5) // Suspend interrupt enable bit

#define _EP0IS_DRQI   (1 << 10) // Transmit Data interrupt request bit
#define _EP0IS_DRQIIE (1 << 14) // Transmit Data Interrupt enable bit
#define _EP0IS_BFINI  (1 << 15) // Transmission buffer initialization bit

#define _EP0OS_SIZE        0x7F  // Packet size bits
#define _EP0OS_SPK    (1 <<  9)  // Short Packet interrupt flag bit
#define _EP0OS_DRQO   (1 << 10)  // Received data interrupt flag bit
#define _EP0OS_SPKIE  (1 << 13)  // Short packet interrupt enable bit
#define _EP0OS_DRQOIE (1 << 14)  // Receive Data Interrupt enable bit
#define _EP0OS_BFINI  (1 << 15)  // Receive buffer initialization bit

#define _EP1S_SIZE        0x1FF  // Packet size bits
#define _EP1S_SPK     (1 <<  9)  // Short Packet interrupt flag bit
#define _EP1S_DRQ     (1 << 10)  // Packet transfer interrupt flag bit
#define _EP1S_BUSY    (1 << 11)  // Busy flag bit
#define _EP1S_SPKIE   (1 << 13)  // Short packet interrupt enable bit
#define _EP1S_DRQIE   (1 << 14)  // DRQ Interrupt enable bit
#define _EP1S_BFINI   (1 << 15)  // Transmission/Reception buffer initialization bit

#define _EP2S_SIZE         0xFF  // Packet size bits
#define _EP2S_SPK     (1 <<  9)  // Short Packet interrupt flag bit
#define _EP2S_DRQ     (1 << 10)  // Packet transfer interrupt flag bit
#define _EP2S_BUSY    (1 << 11)  // Busy flag bit
#define _EP2S_SPKIE   (1 << 13)  // Short packet interrupt enable bit
#define _EP2S_DRQIE   (1 << 14)  // DRQ Interrupt enable bit
#define _EP2S_BFINI   (1 << 15)  // Transmission/Reception buffer initialization bit

#define _EP3S_SIZE         0xFF  // Packet size bits
#define _EP3S_SPK     (1 <<  9)  // Short Packet interrupt flag bit
#define _EP3S_DRQ     (1 << 10)  // Packet transfer interrupt flag bit
#define _EP3S_BUSY    (1 << 11)  // Busy flag bit
#define _EP3S_SPKIE   (1 << 13)  // Short packet interrupt enable bit
#define _EP3S_DRQIE   (1 << 14)  // DRQ Interrupt enable bit
#define _EP3S_BFINI   (1 << 15)  // Transmission/Reception buffer initialization bit

#define _EP4S_SIZE         0xFF  // Packet size bits
#define _EP4S_SPK     (1 <<  9)  // Short Packet interrupt flag bit
#define _EP4S_DRQ     (1 << 10)  // Packet transfer interrupt flag bit
#define _EP4S_BUSY    (1 << 11)  // Busy flag bit
#define _EP4S_SPKIE   (1 << 13)  // Short packet interrupt enable bit
#define _EP4S_DRQIE   (1 << 14)  // DRQ Interrupt enable bit
#define _EP4S_BFINI   (1 << 15)  // Transmission/Reception buffer initialization bit

#define _EP5S_SIZE         0xFF  // Packet size bits
#define _EP5S_SPK     (1 <<  9)  // Short Packet interrupt flag bit
#define _EP5S_DRQ     (1 << 10)  // Packet transfer interrupt flag bit
#define _EP5S_BUSY    (1 << 11)  // Busy flag bit
#define _EP5S_SPKIE   (1 << 13)  // Short packet interrupt enable bit
#define _EP5S_DRQIE   (1 << 14)  // DRQ Interrupt enable bit
#define _EP5S_BFINI   (1 << 15)  // Transmission/Reception buffer initialization bit

#define _HCNT_HOST    (1 <<  0) // HOST Mode bit
#define _HCNT_URST	  (1 <<  1) // USB Bus Reset bit
#define _HCNT_SOFIRE  (1 <<  2) // SOF interrupt Enable bit
#define _HCNT_DIRE    (1 <<  3) // Disconnection interrupt Enable bit
#define _HCNT_CNNIRE  (1 <<  4) // Connection interrupt Enable bit
#define _HCNT_CMPIRE  (1 <<  5) // Completion interrupt Enable bit
#define _HCNT_URIRE   (1 <<  6) // USB bus Reset interrupt Enable bit
#define _HCNT_RWKIRE  (1 <<  7) // Remote Wake-up interrupt Enable bit
#define _HCNT_RETRY   (1 <<  8) // Retry enable bit
#define _HCNT_CANCEL  (1 <<  9) // Token cancel enable bit
#define _HCNT_SOFSTEP (1 << 10) // SOF interrupt trigger bit

#define _HIRQ_SOFIRQ  (1 <<  0) // HOST Mode bit
#define _HIRQ_DIRQ	  (1 <<  1) // USB Bus Reset bit
#define _HIRQ_CNNIRQ  (1 <<  2) // SOF interrupt Enable bit
#define _HIRQ_CMPIRQ  (1 <<  3) // Disconnection interrupt Enable bit
#define _HIRQ_URIRQ   (1 <<  4) // Connection interrupt Enable bit
#define _HIRQ_RWKIRQ  (1 <<  5) // Completion interrupt Enable bit
#define _HIRQ_TCAN    (1 <<  7) // Remote Wake-up interrupt Enable bit

#define _HERR_HS           0x03 // Handshake Status
#define _HERR_STUFF	  (1 <<  2) // Stuffing Error bit
#define _HERR_TGERR   (1 <<  3) // Toggle Error bit
#define _HERR_CRC     (1 <<  4) // CRC Error bit
#define _HERR_TOUT    (1 <<  5) // Time Out bit
#define _HERR_RERR    (1 <<  6) // Receive Error bit
#define _HERR_LSTSOF  (1 <<  7) // Lost SOF bit

#define _HSTATE_CSTAT   (1 <<  0) // Connection status bit
#define _HSTATE_TMODE	(1 <<  1) // Transfer Mode bit
#define _HSTATE_SUSP    (1 <<  2) // Suspend bit
#define _HSTATE_SOFBUSY (1 <<  3) // SOF timer operation bit
#define _HSTATE_CLKSEL  (1 <<  4) // Clock Selection bit (Full / Low Speed)
#define _HSTATE_ALIVE   (1 <<  5) // keep alive bit (for low speed)

#endif


