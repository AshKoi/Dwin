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
/** \file bootloaderapi.h
 **
 ** API to execute bootloader commands
 **
 ** History:
 **   - 2012-03-16    1.0  MSc  First version for MB9BFx18T
 *****************************************************************************/

#ifndef __BOOTLOADERAPI_H__
#define __BOOTLOADERAPI_H__

#include "mcu.h"
#include "global.h"
#include "base_type.h"
#include "srecordparser.h"


void BootloaderAPI_JumpBoot(uint32_t u32Address);
void BootloaderAPI_ExecuteUserApplication(void);
boolean_t BootloaderAPI_UserCodeValid(void);
boolean_t BootloaderAPI_CheckEmpty(void);
void BootloaderAPI_EraseChip(void);
boolean_t BootloaderAPI_DetectUsbStick(void);
void BootloaderAPI_WaitUsbDtickDisconnected(void);
boolean_t BootloaderAPI_InitMassStorage(void);
boolean_t BootloaderAPI_DetectFile(const char_t* FileName);
void BootloaderAPI_ExecuteSoftwareReset(void);
boolean_t BootloaderAPI_FlashHexFile(char_t* FileName);


#endif
