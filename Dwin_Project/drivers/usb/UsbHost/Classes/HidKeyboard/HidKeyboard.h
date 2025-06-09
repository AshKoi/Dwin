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
/** \file HidKeyboard.h
 **
 ** Part of FSEU USB Host Driver Module
 **
 ** History:
 **   - 2010-12-13    1.0  MSc  First version  (works with 16FX)
 **   - 2010-01-05    1.1  MSc  API Updates
 **   - 2011-03-30    1.2  MSc  Public Release
 **   - 2011-08-24    1.3  MSc  Bug fixes
 *****************************************************************************/

#ifndef __HIDKEYBOARD_H__
#define __HIDKEYBOARD_H__

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/

#include "mcu.h"
#include "base_type.h"
#include "UsbHost.h"

#if (USE_USB_HOST == 1)
#include "HidKeyboard.h"

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/

#define KEYCODE_ASCII(keycode)  u8Keycodes[keycode]

/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/


typedef struct KeyboardData
{
    uint8_t u8ModifierKeys;
    uint8_t u8Reserved;
    uint8_t u8KeyCode1;
    uint8_t u8KeyCode2;
    uint8_t u8KeyCode3;
    uint8_t u8KeyCode4;
    uint8_t u8KeyCode5;
    uint8_t u8KeyCode6;
} KeyboardData_t;



/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/

#ifndef __HIDKEYBOARD_C__
    extern const uint8_t u8Keycodes[];
#endif

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/

void HidKeyboard_Init();
boolean_t HidKeyboardInitHandler(uint8_t* pu8Configuration, uint32_t u32Length);
boolean_t HidKeyboardDeinitHandler();
void HidKeyboard_Init();
void HidKeyboard_DataReceivedHandler();
void HidKeyboard_SetEventHandler(void (*Handler)(KeyboardData_t *stcData));
void HidKeyboard_RemoveEventHandler();
char_t HidKeyBoard_GetCh();

void HidKeyboard_RemoveEvent();
void UMH_StallExecutionUserCallback(unsigned long time);
void Idle(void);

#endif /* (USE_USB_HOST == 1) */
#endif /* __HidKeyboard_H__*/
