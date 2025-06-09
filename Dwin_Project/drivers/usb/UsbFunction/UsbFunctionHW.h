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
/** \file UsbFunction.h
 **
 ** - See README.TXT for project description
 ** - pre release for a simple universal usb function library
 **
 ** History:
 **   - 2010-03-30    1.0  MSc  First version  (works with 16FX,FR80)
 **   - 2011-03-30    1.1  MSc  New HW description style
 *****************************************************************************/

#ifndef __USBFUNCTIONHW_H__
#define __USBFUNCTIONHW_H__

#define DEVICEVBUS_DISABLEISR bFM3_EXTI_ENIR_EN15 = 0
#define DEVICEVBUS_ENABLEISR bFM3_EXTI_ENIR_EN15 = 1
#define DEVICEVBUS_ISRISSET bFM3_EXTI_EIRR_ER15 == 1
#define DEVICEVBUS_CLEARISRFLAG bFM3_EXTI_EICL_ECL15 = 0
#define DEVICEVBUS_SETLOWDETECT bFM3_EXTI_ELVR_LA15 = 0
#define DEVICEVBUS_SETHIGHDETECT bFM3_EXTI_ELVR_LA15 = 1
#define DEVICEVBUS_HIGHDETECT (FM3_GPIO->PDIR6 & 0x01) > 0
#define DEVICEVBUS_INIT bFM3_GPIO_PFR6_P0  = 1; bFM3_GPIO_DDR6_P0 = 0; bFM3_GPIO_EPFR06_EINT15S1 = 1; NVIC_ClearPendingIRQ(EXINT8_31_IRQn); NVIC_EnableIRQ(EXINT8_31_IRQn); NVIC_SetPriority(EXINT8_31_IRQn,1)
#define DEVICEVBUS_ENABLED 1

#define DEVICEHCONX_INIT 
#define DEVICEHCONX_SET 
#define DEVICEHCONX_CLEAR 
#define DEVICEHCONX_ENABLED 1


#endif
