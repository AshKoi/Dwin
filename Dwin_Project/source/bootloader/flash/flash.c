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

#include "mcu.h"
#include "flash.h"

#ifdef __ICCARM__
  #pragma section = ".flash_ram_code"
#endif

/**
 * \brief  Flash Erase Sector Function
 *
 * \param  u32SectorEraseAddress Address of sector to be erased
 * \return ok: FLASH_OK, error: FLASH_TIMEOUT
 *
 */
#ifdef __ICCARM__
  __ramfunc
#elif  __CC_ARM
  __attribute__ ((section (".ramfunc")))
#else
  #error Please check compiler and linker settings for RAM code
#endif
int32_t FlashRomEraseSector(uint32_t u32SectorEraseAddress)
{
  volatile int32_t  i32FlashFlag = 0;
  volatile uint32_t u32DummyRead;
  
  FM3_FLASH_IF->FASZR &= 0xFFFD;      // ASZ[1:0] = 2'b01
  FM3_FLASH_IF->FASZR |= 1;
  u32DummyRead = FM3_FLASH_IF->FASZR; // dummy read of FASZR
  
  *(FLASH_SEQ_1550) = FLASH_SECTOR_ERASE_1;
  *(FLASH_SEQ_0AA8) = FLASH_SECTOR_ERASE_2;
  *(FLASH_SEQ_1550) = FLASH_SECTOR_ERASE_3;
  *(FLASH_SEQ_1550) = FLASH_SECTOR_ERASE_4;
  *(FLASH_SEQ_0AA8) = FLASH_SECTOR_ERASE_5;
  *(volatile uint16_t*)u32SectorEraseAddress = FLASH_SECTOR_ERASE_6;
  
  // sector erase timer ready?
  while ((*(volatile uint16_t *)u32SectorEraseAddress & FLASH_DQ3) != FLASH_DQ3);  

  while(0 == i32FlashFlag)
  {
    // Flash timeout?
    if((*(volatile uint16_t *)u32SectorEraseAddress & FLASH_DQ5) == FLASH_DQ5) 
    {
      i32FlashFlag = FLASH_TIMEOUT_ERROR;
    }

    // Data correct?
    if((*(volatile uint16_t *)u32SectorEraseAddress & FLASH_DQ7) == FLASH_DQ7)
    {
      i32FlashFlag = FLASH_OK;
    }
  }

  FM3_FLASH_IF->FASZR &= 0xFFFE;      // ASZ[1:0] = 2'b10
  FM3_FLASH_IF->FASZR |= 0x2;
  u32DummyRead = FM3_FLASH_IF->FASZR; // dummy read of FASZR
  
  return (i32FlashFlag);  
}

/**
 * \brief  Data Polling alogithm
 *
 * \param  u32PollAddress Address to be polled
 * \param  u16PollData  Data to be polled
 * \return ok: FLASH_OK, error: FLASH_TIMEOUT
 *
 */
#ifdef __ICCARM__
  __ramfunc
#elif  __CC_ARM
  __attribute__ ((section (".ramfunc")))
#else
  #error Please check compiler and linker settings for RAM code
#endif
int32_t FlashDataPolling (uint32_t u32PollAddress, uint16_t u16PollData)
{
  volatile int32_t  i32FlashFlag = 0;
  volatile uint16_t u16DummyRead;
  
  u16DummyRead = *(volatile uint16_t *)u32PollAddress;
  while(0 == i32FlashFlag)
  {
    // Flash timeout?
    if((*(volatile uint16_t *)u32PollAddress & FLASH_DQ5) == FLASH_DQ5) 
    {
      i32FlashFlag = FLASH_TIMEOUT_ERROR;
    }

    // Data correct?
    if((*(volatile uint16_t *)u32PollAddress & FLASH_DQ7) == (u16PollData & FLASH_DQ7))
    {
      i32FlashFlag = FLASH_OK;
    }
  }
  
  return i32FlashFlag;
}
    
    
/**
 * \brief  Flash 16-Bit Write Function
 *
 * \param u32ProgramAddress Address of 32-Bit-word
 * \param u32ProgamData 32-bit word
 * \return ok: FLASH_OK, error: FLASH_TIMEOUT
 *
 */
#ifdef __ICCARM__
  __ramfunc
#elif  __CC_ARM
  __attribute__ ((section (".ramfunc")))
#else
  #error Please check compiler and linker settings for RAM code
#endif
int32_t FlashRomProgram(uint32_t u32ProgramAddress, uint32_t u32ProgamData)
{
  volatile int32_t  i32FlashFlag = 0;
  volatile uint32_t u32DummyRead;
  volatile uint16_t u16HalfData;
  
  FM3_FLASH_IF->FASZR &= 0xFFFD;      // ASZ[1:0] = 2'b01
  FM3_FLASH_IF->FASZR |= 1;
  u32DummyRead = FM3_FLASH_IF->FASZR; // dummy read of FASZR
  
  // Data [0:15]
  u16HalfData = (uint16_t) (u32ProgamData & 0x0000FFFF);
  *(FLASH_SEQ_1550) = FLASH_WRITE_1;
  *(FLASH_SEQ_0AA8) = FLASH_WRITE_2;
  *(FLASH_SEQ_1550) = FLASH_WRITE_3;
  *(volatile uint16_t*)u32ProgramAddress = u16HalfData;  
  i32FlashFlag = FlashDataPolling(u32ProgramAddress, u16HalfData);
  
  if (FLASH_OK == i32FlashFlag)
  {
    // Data [16:31] (Set ECC Flash cells)
    u16HalfData = (uint16_t) ((u32ProgamData >> 16) & 0x0000FFFF);
    *(FLASH_SEQ_1550) = FLASH_WRITE_1;
    *(FLASH_SEQ_0AA8) = FLASH_WRITE_2;
    *(FLASH_SEQ_1550) = FLASH_WRITE_3;
    *(volatile uint16_t*)(u32ProgramAddress + 2) = u16HalfData;  
    i32FlashFlag = FlashDataPolling((u32ProgramAddress + 2), u16HalfData);
  }
  
  FM3_FLASH_IF->FASZR &= 0xFFFE;      // ASZ[1:0] = 2'b10
  FM3_FLASH_IF->FASZR |= 0x2;
  u32DummyRead = FM3_FLASH_IF->FASZR; // dummy read of FASZR
  
  return (i32FlashFlag);  
}

