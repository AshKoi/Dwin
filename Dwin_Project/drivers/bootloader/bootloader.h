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
/** \file bootloader.c
 **
 ** Global file for bootloader settings
 **
 ** History:
 **   - 2012-03-16    1.0  MSc  First version  SK-FM3-176PMC-ETHERNET
 *****************************************************************************/

#ifndef __BOOTLOADER_H__
#define __BOOTLOADER_H__

#include "flash.h"
#include "bootloaderapi.h"
#include "base_type.h"


#define USER_FLASH_START 0x00008000
#define USER_FLASH_END   0x00101004
#define USER_FLASH_CHECK_START 0x00008000
#define USER_FLASH_CHECK_STOP  0x000FFFFF

#define USER_HEX_FILE    "app.mhx"
#define USER_CREAT_FILE    "whr.mhx"


// #define BOOTLOADER_ACTIVITY_TOGGLE_COMMAND           bFM3_GPIO_PDOR7_P7 = !bFM3_GPIO_PDOR7_P7
// #define BOOTLOADER_ACTIVE_COMMAND                    SEVENSEG_NUMBER(1)
// #define BOOTLOADER_USBSTICK_FOUND_COMMAND            SEVENSEG_NUMBER(2)
// #define BOOTLOADER_HEX_FILE_FOUND_COMMAND            SEVENSEG_NUMBER(3)
// #define BOOTLOADER_START_ERASEING_COMMAND            SEVENSEG_NUMBER(4)
// #define BOOTLOADER_START_FLASHING_COMMAND            SEVENSEG_NUMBER(5)
// #define BOOTLOADER_SUCCESS_REMOVE_USBSTICK_COMMAND   SEVENSEG_NUMBER(0)
// #define BOOTLOADER_ERROR_ERASE_COMMAND               SEVENSEG_NUMBER(11)
// #define BOOTLOADER_ERROR_FLASHING_COMMAND            SEVENSEG_NUMBER(12)

#endif
