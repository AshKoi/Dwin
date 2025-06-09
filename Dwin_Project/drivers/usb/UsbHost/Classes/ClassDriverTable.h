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
/** \file ClassDriverTable.h
 **
 ** Part of FSEU USB Host Driver Module
 **
 ** History:
 **   - 2011-03-30    1.0  MSc  First version
 *****************************************************************************/

#ifndef __CLASSDRIVERTABLE_H__
#define __CLASSDRIVERTABLE_H__

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/

#include "Usb.h"

#if (USE_USB_HOST == 1)
#include "UsbMassStorage.h"
/* OTHER USB CLASS DRIVER INCLUDES CAN BE ADDED HERE */

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/

/* DEFINEPARSER(DRIVERTYPES) START */
#define USBCLASSDRIVER_MASSSTORAGE 1

/* DEFINEPARSER(DRIVERTYPES) STOP */

/* OTHER USB CLASS DRIVER DEFINES CAN BE ADDED HERE */

/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/

#ifdef __USBCLASSSUPERVISOR_C__
#define MAX_CLASSHANDLERS 1
    const UsbClassHandler_t UsbClassHandlers[] =
    {
        {MassStorageInitHandler,MassStorageDeinitHandler,0,0,0,0,0,8,6,80,USBCLASSDRIVER_MASSSTORAGE},
       
    };
#endif

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/

#endif /* (USE_USB_HOST == 1) */

#endif /* __CLASSDRIVERTABLE_H__ */


