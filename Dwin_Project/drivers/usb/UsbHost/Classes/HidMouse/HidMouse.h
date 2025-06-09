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
/** \file HidMouse.h
 **
 ** Part of FSEU USB Host Driver Module
 **
 ** History:
 **   - 2011-03-30    1.0  MSc  First version 
 **   - 2011-08-24    1.1  MSc  Some fixes in X/Y calculation
 *****************************************************************************/


#ifndef __HIDMOUSE_H__
#define __HIDMOUSE_H__

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/

#include "mcu.h"
#include "base_type.h"
#include "UsbHost.h"

#if (USE_USB_HOST == 1)
#include "HidMouse.h"

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/

/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/
typedef uint8_t Buttons_t;

typedef struct Point
{
    uint32_t u32X;
    uint32_t u32Y;
} Point_t;

typedef struct Diff
{
    int8_t i8X;
    int8_t i8Y;
} Diff_t;

typedef struct MouseData
{
    Point_t   stcPosition;
    Buttons_t tButtons;
    Diff_t    stcDiffXY;
    uint32_t  u32ScrollPosition;
    int8_t    i8Scrolling;    
} MouseData_t;


typedef uint8_t MouseEventType_t;
#define MOUSEEVENT_POSITIONX_CHANGED 1
#define MOUSEEVENT_POSITIONY_CHANGED 2
#define MOUSEEVENT_BUTTON_CHANGED    4
#define MOUSEEVENT_SCROLLING_CHANGED  8
#define MOUSEEVENT_ANY_CHANGE        0xFF
/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/



/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/

void HidMouse_Init();
boolean_t HidMouseInitHandler(uint8_t* pu8Configuration, uint32_t u32Length);
boolean_t HidMouseDeinitHandler();
void HidMouse_Init();
void HidMouse_DataReceivedHandler();

void HidMouse_SetEvent(void (*Handler)(MouseEventType_t stcEventType, MouseData_t MouseData));
void HidMouse_RemoveEvent();

Point_t HidMouse_GetCurrentPosition();
void HidMouse_SetCurrentPosition(uint32_t u32X, uint32_t u32Y);
Buttons_t HidMouse_GetButtons();
uint32_t HidMouse_GetCurrentScrollPosition();
boolean_t HidMouse_Moved();

void UMH_StallExecutionUserCallback(unsigned long time);
void Idle(void);

#endif /* (USE_USB_HOST == 1) */
#endif /* __HIDMOUSE_H__*/
