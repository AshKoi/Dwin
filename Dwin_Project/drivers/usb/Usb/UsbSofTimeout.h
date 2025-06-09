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
/** \file UsbSofTimeout.h
 **
 ** USB Registers File
 **
 ** History:
 **   - 2010-09-24  1.0  MSc  USB Sof Timeout Feature
 **   - 2011-03-30  1.0  MSc  Some Corrections
 *************************************************************************/

#ifndef __USBSOFTIMEOUT_H__
#define __USBSOFTIMEOUT_H__

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/

#include "base_type.h"

/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/

typedef struct stc_timeout_handler
{
    uint16_t u16TimeOut;
    void (* Handler)(void);
} stc_timeout_handler_t;

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/



void Usb_SetSOFTimeOutHandler(void (** pSofHandler)(uint16_t u16FrameNumber));
void Usb_TimeoutHandler(uint16_t ticks);
boolean_t Usb_AddTimeOut(void (* Handler)(void), uint16_t u16TimeOut);
void Usb_RemoveTimeOut(void (* Handler)(void));

#endif
