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

#ifndef _FLASH_H_
#define _FLASH_H_

#define FLASH_SEQ_1550 ((volatile uint16_t*) 0x00001550) // sequence address 1
#define FLASH_SEQ_0AA8 ((volatile uint16_t*) 0x00000AA8) // sequence address 2

#define FLASH_CHIP_ERASE_1   0x00AA   // chip erase commands
#define FLASH_CHIP_ERASE_2   0x0055
#define FLASH_CHIP_ERASE_3   0x0080
#define FLASH_CHIP_ERASE_4   0x00AA
#define FLASH_CHIP_ERASE_5   0x0055
#define FLASH_CHIP_ERASE_6   0x0010

#define FLASH_SECTOR_ERASE_1 0x00AA   // sector erase commands
#define FLASH_SECTOR_ERASE_2 0x0055
#define FLASH_SECTOR_ERASE_3 0x0080
#define FLASH_SECTOR_ERASE_4 0x00AA
#define FLASH_SECTOR_ERASE_5 0x0055
#define FLASH_SECTOR_ERASE_6 0x0030

#define FLASH_WRITE_1        0x00AA   // flash write commands
#define FLASH_WRITE_2        0x0055 
#define FLASH_WRITE_3        0x00A0

#define FLASH_SECTOR_ERASE_SUSPEND 0x00B0 // Sector erase suspend command
#define FLASH_SECTOR_ERASE_RESTART 0x0030 // Sector erase restart (resume) command

#define FLASH_ADDRESS_SA4  0x00000000     // address of sector SA4
#define FLASH_ADDRESS_SA5  0x00000004     // address of sector SA5
#define FLASH_ADDRESS_SA6  0x00004000     // address of sector SA6
#define FLASH_ADDRESS_SA7  0x00004004     // address of sector SA7
#define FLASH_ADDRESS_SA8  0x00008000     // address of sector SA8
#define FLASH_ADDRESS_SA9  0x00008004     // address of sector SA9
#define FLASH_ADDRESS_SA10 0x00020000     // address of sector SA10
#define FLASH_ADDRESS_SA11 0x00020004     // address of sector SA11
#define FLASH_ADDRESS_SA12 0x00040000     // address of sector SA12
#define FLASH_ADDRESS_SA13 0x00040004     // address of sector SA13
#define FLASH_ADDRESS_SA14 0x00060000     // address of sector SA14
#define FLASH_ADDRESS_SA15 0x00060004     // address of sector SA15
#define FLASH_ADDRESS_SA16 0x00080000     // address of sector SA16
#define FLASH_ADDRESS_SA17 0x00080004     // address of sector SA17
#define FLASH_ADDRESS_SA18 0x000A0000     // address of sector SA18
#define FLASH_ADDRESS_SA19 0x000A0004     // address of sector SA19
#define FLASH_ADDRESS_SA20 0x000C0000     // address of sector SA20
#define FLASH_ADDRESS_SA21 0x000C0004     // address of sector SA21
#define FLASH_ADDRESS_SA22 0x000E0000     // address of sector SA22
#define FLASH_ADDRESS_SA23 0x000E0004     // address of sector SA23

#define FLASH_DQ7 0x0080    // data polling flag bit (DPOL) position
#define FLASH_DQ6 0x0040    // data toggle flag bit (TOGG) position
#define FLASH_DQ5 0x0020    // time limit exceeding flag bit (TLOV) position
#define FLASH_DQ3 0x0008    // sector erase timer flag bit (SETI) position

#define FLASH_TIMEOUT_ERROR   -1
#define FLASH_OK              1

extern int32_t FlashRomEraseSector(uint32_t);
extern int32_t FlashRomProgram(uint32_t, uint32_t);

#endif /* _FLASH_H_ */

