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
/** \file bootloaderapi.c
 **
 ** API to execute bootloader commands
 **
 ** History:
 **   - 2012-03-16    1.0  MSc  First version for MB9BFx18T
 *****************************************************************************/

#include "mcu.h"
#include "bootloaderapi.h"
#include "bootloader.h"
#include "ff.h"
#include "StorageApi.h"
#include "UsbClassSupervisor.h"

static uint8_t pu8Buffer[500];

static void delay(volatile uint32_t u32TimeOut);


/**
 ******************************************************************************
 ** \brief  Delay loop
 **
 ** \param  u32Timeout Timeout value
 ** \return none
 ******************************************************************************/
static void delay(volatile uint32_t u32TimeOut)
{
	volatile uint32_t i;
	WatchDogClear(1);
	u32TimeOut = u32TimeOut * 1000;
	while(u32TimeOut--)
	{
	   for(i=20;i>0;i--)
	   {
	   }
	}
	WatchDogClear(1);
}

/**
 ******************************************************************************
 ** \brief  Jump to user code application
 **
 ** \param  u32Address Address of user code
 ** \return none
 ******************************************************************************/
#ifdef __ICCARM__
  void BootloaderAPI_JumpBoot(uint32_t u32Address)
  {
     __asm("LDR SP, [R0]");		//Load new stack pointer address
     __asm("LDR PC, [R0, #4]");	        //Load new program counter address
  }
#elif  __CC_ARM
  __asm void BootloaderAPI_JumpBoot(uint32_t u32Address)
  {
     LDR SP, [R0]		;Load new stack pointer address
     LDR PC, [R0, #4]	        ;Load new program counter address
  }
#else
  #error Please check compiler and linker settings for RAM code
#endif

/**
 ******************************************************************************
 ** \brief  Execute main application
 **
 ** \param  none
 ** \return none
 ******************************************************************************/
void BootloaderAPI_ExecuteUserApplication(void)
{
    SCB->VTOR = USER_FLASH_START ; //& 0x1FFFFF80Change the Vector Table to the USER_FLASH_START 
    BootloaderAPI_JumpBoot(USER_FLASH_START);
}

/**
 ******************************************************************************
 ** \brief  Check user code flash is not empty
 **
 ** \param  none
 ** \return TRUE, is it is not empty and user application exists
 ******************************************************************************/
boolean_t BootloaderAPI_UserCodeValid(void)
{
    return !BootloaderAPI_CheckEmpty();
}

/**
 ******************************************************************************
 ** \brief  Check user code area is empty
 **
 ** \param  none
 ** \return TRUE, if it is empty
 ******************************************************************************/
boolean_t BootloaderAPI_CheckEmpty(void)
{
    uint32_t* pu32Address;
    pu32Address = (uint32_t*)USER_FLASH_CHECK_START;
    while(pu32Address <= (uint32_t*)USER_FLASH_CHECK_STOP)
    {
        if (*pu32Address != 0xFFFFFFFF)
        {
            return FALSE;
        }
        pu32Address++;
    }
    return TRUE;
}

/**
 ******************************************************************************
 ** \brief  Erase user code flash sections
 **
 ** \param  none
 ** \return none
 ******************************************************************************/
void BootloaderAPI_EraseChip(void)
{
  
    __disable_irq();
    
    /* Bootloader located here, do not erase
    FlashRomEraseSector(FLASH_ADDRESS_SA4);    // 0x0000_0000 -> 0x0003_3FFF, 8KByte (odd words)
    FlashRomEraseSector(FLASH_ADDRESS_SA5);    // 0x0000_0004 -> 0x0000_4003, 8KByte (even words)    
    */
    
  //  FlashRomEraseSector(FLASH_ADDRESS_SA6);    //  8KB
  //  FlashRomEraseSector(FLASH_ADDRESS_SA7);    //  8KB
    FlashRomEraseSector(FLASH_ADDRESS_SA8);    // 48KB
    FlashRomEraseSector(FLASH_ADDRESS_SA9);    // 48KB
    FlashRomEraseSector(FLASH_ADDRESS_SA10);   // 64KB
    FlashRomEraseSector(FLASH_ADDRESS_SA11);   // 64KB
    FlashRomEraseSector(FLASH_ADDRESS_SA12);   // 64KB
    FlashRomEraseSector(FLASH_ADDRESS_SA13);   // 64KB
    FlashRomEraseSector(FLASH_ADDRESS_SA14);   // 64KB
    FlashRomEraseSector(FLASH_ADDRESS_SA15);   // 64KB
    
    FlashRomEraseSector(FLASH_ADDRESS_SA16);   // 64KB
    FlashRomEraseSector(FLASH_ADDRESS_SA17);   // 64KB
    FlashRomEraseSector(FLASH_ADDRESS_SA18);   // 64KB
    FlashRomEraseSector(FLASH_ADDRESS_SA19);   // 64KB
    FlashRomEraseSector(FLASH_ADDRESS_SA20);   // 64KB
    FlashRomEraseSector(FLASH_ADDRESS_SA21);   // 64KB
    FlashRomEraseSector(FLASH_ADDRESS_SA22);   // 64KB
    FlashRomEraseSector(FLASH_ADDRESS_SA23);   // 64KB
    __enable_irq();
}

/**
 ******************************************************************************
 ** \brief  Detect USB Stick
 **
 ** \param  none
 ** \return TRUE on success
 ******************************************************************************/
boolean_t BootloaderAPI_DetectUsbStick(void)
{
#if 1
	volatile uint32_t u32Timeout = 100000;
	uint32_t count=0;
    while((UsbClassSupervisor_GetActiveDriver() == 0) && (count < 100))
    {
        u32Timeout--;
		if(u32Timeout==0)
		{
			u32Timeout = 100000;
			count++;
			WatchDogClear(1);
		}
    } 
#else
    volatile uint32_t u32Timeout = 10000000;
    while((UsbClassSupervisor_GetActiveDriver() == 0) && (u32Timeout > 0))
    {
        u32Timeout--;
    } 
#endif
    return (UsbClassSupervisor_GetActiveDriver() > 0);
}

/**
 ******************************************************************************
 ** \brief  Wait as long the USB Stick is connected
 **
 ** \param  none
 ** \return none
 ******************************************************************************/
void BootloaderAPI_WaitUsbDtickDisconnected(void)
{
    while((UsbClassSupervisor_GetActiveDriver() != 0));
}

/**
 ******************************************************************************
 ** \brief  Initialize mass storage driver
 **
 ** \param  none
 ** \return TRUE on success
 ******************************************************************************/
boolean_t BootloaderAPI_InitMassStorage(void)
{
    delay(100); // wait a moment
    while((StorageInit() != 0) && (UsbHost_GetDeviceStatus() != USBHOST_DEVICE_IDLE))
    {
        UsbHost_DeInit(); // if USB Mass Storage must be resetted...
        delay(200);     // wait a moment
        UsbHost_Init();
        delay(200);     // wait a moment
    }
    if (UsbHost_GetDeviceStatus() != USBHOST_DEVICE_IDLE)
    {
        return TRUE;
    }
    return FALSE;
}

/**
 ******************************************************************************
 ** \brief  Search for file
 **
 ** \param  FileName name of file
 ** \return TRUE if file exists
 ******************************************************************************/
boolean_t BootloaderAPI_DetectFile(const char_t* FileName)
{
    FATFS fatfs[1];
    FIL fil;

    f_mount(0,fatfs);
    if (f_open(&fil,FileName,FA_OPEN_EXISTING | FA_READ) == 0)	// does a dummy open file
    {
        f_close(&fil);
        return TRUE;
    }
    return FALSE;
}

/**
 ******************************************************************************
 ** \brief  Execute software reset
 **
 ** \param  none
 ** \return none
 ******************************************************************************/
void BootloaderAPI_ExecuteSoftwareReset(void)
{
    *(volatile unsigned long*)(0xE000ED0C)=0x05FA0004; //SW Reset
}

/**
 ******************************************************************************
** \brief  Flash hex file (S-Record file: *.mhx or *.srec)
 **
 ** \param  FileName filename
 ** \return TRUE on success
 ******************************************************************************/
boolean_t BootloaderAPI_FlashHexFile(char_t* FileName)
{
    uint8_t u8DataLength = 0;
    uint32_t u32Address = 0;
    uint32_t u32Data = 0;
    uint8_t i;
	SRecordParser_OpenFile(FileName);
    while(1)
    {
        
        u32Address = SRecordParser_ReadLine(pu8Buffer,&u8DataLength);
        if ((u32Address == SRECORDPARSER_ENDOFFILE) && (u8DataLength == 0))
        {
            SRecordParser_CloseFile(FileName);
            return TRUE;
        }
        if ((u32Address == SRECORDPARSER_CRCERROR) && (u8DataLength == 0))
        {
            SRecordParser_CloseFile(FileName);
            return FALSE;
        }
        
        delay(10);
        __disable_irq();

        for(i = 0;i < u8DataLength; i += 4)
        {
            u32Data = *((uint32_t*)(pu8Buffer + i));
            
            if ((u32Address >= USER_FLASH_START) && (u32Address <= USER_FLASH_END))
            {
                FlashRomProgram(u32Address + i,u32Data);
            }
        }
        
        delay(10);
        __enable_irq();
        delay(10);
//        BOOTLOADER_ACTIVITY_TOGGLE_COMMAND;
    }
}

