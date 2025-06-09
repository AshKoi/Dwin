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
/** \file UsbClassSupervisor.c
 **
 ** Part of FSEU USB Host Driver Module
 **
 ** History:
 **   - 2011-03-30    1.0  MSc  First version
 *****************************************************************************/


/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/

#define __USBCLASSSUPERVISOR_C__
#include "UsbClassSupervisor.h"

#if (USE_USB_HOST == 1)

/*****************************************************************************/
/* Local pre-processor symbols/macros ('#define')                            */
/*****************************************************************************/


/*****************************************************************************/
/* function */
/*****************************************************************************/


/*****************************************************************************/
/* Global variable definitions (declared in header file with 'extern')       */
/*****************************************************************************/

UsbClassHandler_t stcMatchClass;
uint8_t u8SupervisorState = USBCLASSSVR_IDLE;
uint8_t* pu8Configuration;
uint32_t u32ConfigurationSize;
UsbClassDriver_t CurrentDriver = 0;
void (*InternalErrorHandler)(UsbClassHandler_t stcMatchClass) = 0;

/*****************************************************************************/
/* Local type definitions ('typedef')                                        */
/*****************************************************************************/

/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/


/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/

/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

void UsbClassSupervisor_Init()
{
    u8SupervisorState = USBCLASSSVR_IDLE;
    Usb_RemoveTimeOut(UsbClassSupervisor_SysTickHandler);
    Usb_AddTimeOut(UsbClassSupervisor_SysTickHandler,200);
}

void UsbClassSupervisor_Deinit()
{
    uint8_t i;
    CurrentDriver = 0;
    for(i=0;i<MAX_CLASSHANDLERS;i++)
    {
        if (UsbSupervisor_UsbClassMatch((UsbClassHandler_t*)&UsbClassHandlers[i],&stcMatchClass))
        {
            UsbClassHandlers[i].DeviceDisconnection();
            break;
        }
    }
}

void UsbClassSupervisor_SysTickHandler()
{
    uint8_t i;
    if ((UsbHost_GetDeviceStatus() == USBHOST_DEVICE_ADDRESSED) && (UsbHost_GetDeviceStatus() != USBHOST_DEVICE_CONFIGURED))
    {
        switch(u8SupervisorState)
        {
            case USBCLASSSVR_IDLE:
               dbg("Requesting Class Informations\n");
               u8SupervisorState = USBCLASSSVR_REQUEST_DESCRIPTORS;
               UsbHost_SetupRequest((UsbRequest_t *)&stcDeviceDescriptorRequest,ReturnedDeviceDescriptor);
               break;
            case USBCLASSSVR_CLASSCHECK:
               u8SupervisorState = USBCLASSSVR_IDLE;
               for(i=0;i<MAX_CLASSHANDLERS;i++)
               {
                   if (UsbSupervisor_UsbClassMatch((UsbClassHandler_t*)&UsbClassHandlers[i],&stcMatchClass))
                   {
                       if (UsbClassHandlers[i].InitializeClass(pu8Configuration,u32ConfigurationSize) == TRUE)
                       {
                           dbg("Setting Configuration (1)\n");
                           UsbHost_SetConfigurationDescriptor(1);
                           CurrentDriver = UsbClassHandlers[i].stcDriverType;
                           break;
                       }
                   }
               }
               if (CurrentDriver == 0)
               {
                   u8SupervisorState = USBCLASSSVR_ERROR;
                   if (InternalErrorHandler != 0)
                   {
                       InternalErrorHandler(stcMatchClass);
                   }
               }
               break;
            default:
               break;
        }

    }
    if (UsbHost_GetDeviceStatus() != USBHOST_DEVICE_CONFIGURED)
    {
        Usb_AddTimeOut(UsbClassSupervisor_SysTickHandler,100);
    }
}

void UsbClassSupervisor_SetErrorHandler(void (*Handler)(UsbClassHandler_t stcMatchClass))
{
    InternalErrorHandler = Handler;
}

UsbClassDriver_t UsbClassSupervisor_GetActiveDriver()
{
    return CurrentDriver;
}

void ReturnedDeviceDescriptor(uint8_t* pu8Buffer, uint32_t u32DataSize)
{
    UsbRequest_t stcSetup = {0x80,0x06,0x0200,0x00,0x09};
    Usb_RemoveTimeOut(UsbClassSupervisor_SysTickHandler);
    Usb_AddTimeOut(UsbClassSupervisor_SysTickHandler,200);
    u8SupervisorState = USBCLASSSVR_IDLE;
    stcMatchClass.u16IdVendor = (uint16_t)((uint16_t)pu8Buffer[8] + ((uint16_t)pu8Buffer[9] << 8));
    stcMatchClass.u16IdProduct = (uint16_t)((uint16_t)pu8Buffer[10] + ((uint16_t)pu8Buffer[11] << 8));
    dbg("Device Descriptor Returned\n");
    if (pu8Buffer[4] != 0)
    {
        stcMatchClass.u8MatchDeviceClass    = pu8Buffer[4];
        stcMatchClass.u8MatchDeviceSubClass = pu8Buffer[5];
        stcMatchClass.u8MatchDeviceProtocol = pu8Buffer[6];
    }
    //stcSetup.wValue |= 1;
    dbg("Requesting 9 byte of Configuration\n");
    UsbHost_SetupRequest((UsbRequest_t *)&stcSetup,ReturnedConfigurationDescriptor);
}

void ReturnedConfigurationDescriptor(uint8_t* pu8Buffer, uint32_t u32DataSize)
{
    uint8_t i;
    uint8_t u8NumberOfInterfaces;
    uint8_t* pu8Pointer;
    UsbRequest_t stcSetup = {0x80,0x06,0x0200,0x00,9};
    Usb_RemoveTimeOut(UsbClassSupervisor_SysTickHandler);
    Usb_AddTimeOut(UsbClassSupervisor_SysTickHandler,200);
    u8SupervisorState = USBCLASSSVR_IDLE;
    
    if (pu8Buffer[1] != USB_CONFIGURATION_DESCRIPTOR_TYPE) 
    {
        dbg("Error while receiving configuration!\n");
        dbg("Requesting 9 byte of Configuration\n");
        UsbHost_SetupRequest((UsbRequest_t *)&stcSetup,ReturnedConfigurationDescriptor);
        return;
    }
    if (u32DataSize == 9)
    {
        dbg("Requesting 9 byte of Configuration received\n");
        //stcSetup.wValue |= 1;
        stcSetup.wLength = ((uint8_t)pu8Buffer[2]) | (((uint8_t)pu8Buffer[3]) << 8);
        dbg("Requesting full Configuration\n");
        UsbHost_SetupRequest((UsbRequest_t *)&stcSetup,ReturnedConfigurationDescriptor);
        return;
    }
   
    dbg("Configuration received.\n");
    pu8Configuration = pu8Buffer;
    u32ConfigurationSize = u32DataSize;
    u8NumberOfInterfaces = pu8Buffer[4];
    for(i = 0;i<u8NumberOfInterfaces;i++)
    {
        pu8Pointer = UsbHost_GetUsbInterfaceDescriptor(pu8Buffer,(uint16_t)u32DataSize,i);
        if (pu8Pointer != 0)
        {
            if (pu8Pointer[5] != 0)
            {
                stcMatchClass.u8MatchInterfaceClass = pu8Pointer[5];
                stcMatchClass.u8MatchInterfaceSubClass = pu8Pointer[6];
                stcMatchClass.u8MatchInterfaceProtocol = pu8Pointer[7];
                u8SupervisorState = USBCLASSSVR_CLASSCHECK;
                return;
            }
        }
    }
}

boolean_t UsbSupervisor_UsbClassMatch(UsbClassHandler_t* stcA, UsbClassHandler_t* stcB)
{
    if ((stcA->u16IdVendor != 0) && (stcB->u16IdVendor != 0))
    {
       if (stcA->u16IdVendor != stcB->u16IdVendor)
       {
          return FALSE;
       }
    }
    
    if ((stcA->u16IdProduct != 0) && (stcB->u16IdProduct != 0))
    {
       if (stcA->u16IdProduct != stcB->u16IdProduct)
       {
          return FALSE;
       }
    }
    
    if ((stcA->u8MatchDeviceClass != 0) && (stcB->u8MatchDeviceClass != 0))
    {
       if (stcA->u8MatchDeviceClass != stcB->u8MatchDeviceClass)
       {
          return FALSE;
       }
    }
    

    if ((stcA->u8MatchDeviceSubClass != 0) && (stcB->u8MatchDeviceSubClass != 0))
    {
       if (stcA->u8MatchDeviceSubClass != stcB->u8MatchDeviceSubClass)
       {
          return FALSE;
       }
    }
    
    
    if ((stcA->u8MatchDeviceProtocol != 0) && (stcB->u8MatchDeviceProtocol != 0))
    {
       if (stcA->u8MatchDeviceProtocol != stcB->u8MatchDeviceProtocol)
       {
          return FALSE;
       }
    }


    if ((stcA->u8MatchInterfaceClass != 0) && (stcB->u8MatchInterfaceClass != 0))
    {
       if (stcA->u8MatchInterfaceClass != stcB->u8MatchInterfaceClass)
       {
          return FALSE;
       }
    }
    

    if ((stcA->u8MatchInterfaceSubClass != 0) && (stcB->u8MatchInterfaceSubClass != 0))
    {
       if (stcA->u8MatchInterfaceSubClass != stcB->u8MatchInterfaceSubClass)
       {
          return FALSE;
       }
    }
    

    if ((stcA->u8MatchInterfaceProtocol != 0) && (stcB->u8MatchInterfaceProtocol != 0))
    {
       if (stcA->u8MatchInterfaceProtocol != stcB->u8MatchInterfaceProtocol)
       {
          return FALSE;
       }
    }
    return TRUE;
}

#endif /* (USE_USB_HOST == 1) */

