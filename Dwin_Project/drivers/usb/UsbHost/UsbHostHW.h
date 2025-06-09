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
/** \file UsbHostHW.h
 **
 ** USB hardware specific settings
 **
 ** History:
 **   - 2010-10-14    1.0  MSc  First version  (works with 16FX,FR80)
 **   - 2011-03-30    1.1  MSc  Compatible with FSEU Host drivers 2011-03-30
 *****************************************************************************/

#ifndef __USBHOSTHW_H__
#define __USBHOSTHW_H__


#define HOSTVBUS_INIT FM3_GPIO->DDRC |= 0x8000
#define HOSTVBUS_CLEAR FM3_GPIO->PDORC &= ~0x8000
#define HOSTVBUS_SET FM3_GPIO->PDORC |= 0x8000
#define HOSTVBUS_ENABLED 1

#define HOSTOTGPULLDOWN_INIT FM3_GPIO->DDRC |= 0x0200
#define HOSTOTGPULLDOWN_SET FM3_GPIO->PDORC |= 0x0200
#define HOSTOTGPULLDOWN_CLEAR FM3_GPIO->PDORC &= ~0x0200
#define HOSTOTGPULLDOWN_ENABLED 1

#define HOSTOVERCURRENT_DISABLEISR bFM3_EXTI_ENIR_EN30 = 0
#define HOSTOVERCURRENT_ENABLEISR bFM3_EXTI_ENIR_EN30 = 1
#define HOSTOVERCURRENT_CLEARISRFLAG bFM3_EXTI_EICL_ECL30 = 0
#define HOSTOVERCURRENT_ISRISSET bFM3_EXTI_EIRR_ER30 == 1
#define HOSTOVERCURRENT_SETLOWDETECT bFM3_EXTI_ELVR_LA30 = 0
#define HOSTOVERCURRENT_SETHIGHDETECT bFM3_EXTI_ELVR_LA30 = 1
#define HOSTOVERCURRENT_HIGHDETECT (FM3_GPIO->PDIRD & 0x0001) > 0
#define HOSTOVERCURRENT_INIT FM3_GPIO->DDRD &= ~0x0001
#define HOSTOVERCURRENT_ENABLED 1


#endif
