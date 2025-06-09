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
/** \file Usb.h
 **
 ** USB Descriptors File
 **
 ** History:
 **   - 2010-03-30  1.0  MSc  USB global settings
 **   - 2011-03-30  1.1  MSc  Some Corrections
 *****************************************************************************/

#ifndef __USB_H__
#define __USB_H__

#define USE_USB_HOST 1
#define USE_USB_FUNCTION 0
#define ENABLE_USB_HOST_FUNCION_SWITCHING 0

#if (USE_USB_HOST == 1)
#include "UsbHost.h"
#endif

#if (USE_USB_FUNCTION == 1)
#include "UsbFunction.h"
#endif

#if (ENABLE_USB_HOST_FUNCION_SWITCHING == 1) || ((USE_USB_HOST == 1) && (USE_USB_FUNCTION == 1))
//#error Host and Function in one application: This feature is not yet supported but planed
#endif

#endif /* __USB_H__ */
