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
/** \file HidMouse.c
 **
 ** Part of FSEU USB Host Driver Module
 **
 ** History:
 **   - 2011-03-30    1.0  MSc  First version 
 **   - 2011-08-24    1.1  MSc  Some fixes in X/Y calculation
 *****************************************************************************/


/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/

#include "Usb.h"

#if (USE_USB_HOST == 1)
#include "HidMouse.h"


/*****************************************************************************/
/* Local pre-processor symbols/macros ('#define')                            */
/*****************************************************************************/


/*****************************************************************************/
/* function */
/*****************************************************************************/


/*****************************************************************************/
/* Global variable definitions (declared in header file with 'extern')       */
/*****************************************************************************/
static HostEndpoint_t *EndpointIN;
static uint8_t        u8MouseData[5];
static uint8_t        u8EndpointIN;
static uint16_t       u16MaxPackageSizeIN;
void (*MouseEventHandler)(MouseEventType_t stcEventType, MouseData_t MouseData) = 0;
static MouseData_t stcMouseData;
static boolean_t bMouseMoved = FALSE;
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

boolean_t HidMouseInitHandler(uint8_t* pu8Configuration, uint32_t u32Length)
{
    uint8_t* pu8Buffer;
    uint8_t u8NumberOfInterfaces;
    uint8_t u8NumberOfEndpoints;
    uint8_t u8EndpointNumber;
    uint8_t u8InterfaceNumber;
    uint8_t u8Interval;
    
    dbg("Yeah, called if a HID Device was connected ;-)\n");
    
    u8NumberOfInterfaces=pu8Configuration[4];
    for(u8InterfaceNumber=0;u8InterfaceNumber < u8NumberOfInterfaces;u8InterfaceNumber++)
    {
        dbgval("Interface Index: ",u8InterfaceNumber);
        pu8Buffer = UsbHost_GetUsbInterfaceDescriptor(pu8Configuration,u32Length,u8InterfaceNumber);
        if (pu8Buffer == 0)
        {
            dbg("Error: Could not Request Interface Descriptor.\n");
            return FALSE;
        }
        u8NumberOfEndpoints = pu8Buffer[4];
        
        if ((pu8Buffer[6] == 0x01) && (pu8Buffer[7] == 0x2)) //SubClass & Protocol
        {
            for(u8EndpointNumber=1;u8EndpointNumber<=u8NumberOfEndpoints;u8EndpointNumber++)
            {
                dbgval("Endpoint Index: ",u8EndpointNumber);
                pu8Buffer = UsbHost_GetUsbEndpointDescriptor(pu8Configuration,u32Length,u8InterfaceNumber,u8EndpointNumber);
                if (pu8Buffer[2] & USB_IN_DIRECTION)
                {
                    u8EndpointIN = pu8Buffer[2];
                    u16MaxPackageSizeIN = (uint16_t)(pu8Buffer[4] + (pu8Buffer[5] << 8));
                    u8Interval = pu8Buffer[6]; 
                }
                if (u8EndpointIN != 0)
                {
                    break;
                }
            }
            if (u8EndpointIN == 0)
            {
                dbg("Hm, error while resolving endpoints...");
                return FALSE;
            }
            dbgval("EP-IN:",u8EndpointIN);

            if (u8EndpointIN != 0)
            {
                UsbHost_AddHostEndpoint(&EndpointIN,u8EndpointIN,u16MaxPackageSizeIN,u8Interval,HidMouse_DataReceivedHandler);
                EndpointIN->pu8Buffer = u8MouseData;
                EndpointIN->u32DataSize = 4;
            } 
            dbgval("Endpoint-IN  Addr: ",u8EndpointIN);
            dbgval("Endpoint-IN  Size: ",u16MaxPackageSizeIN);
            break;
        }
    }     
    if (u8EndpointIN == 0)
    {
        dbg("Hm, error while resolving endpoints...");
        return FALSE;
    }
    return TRUE;                       
}

void HidMouse_SetEvent(void (*Handler)(MouseEventType_t stcEventType, MouseData_t MouseData))
{
    MouseEventHandler = Handler;
}

void HidMouse_RemoveEvent()
{
    MouseEventHandler = 0;
}
 
void HidMouse_DataReceivedHandler()
{
    MouseEventType_t tEvent = 0;
    bMouseMoved = TRUE;
    if (EndpointIN->pu8Buffer[0] != stcMouseData.tButtons)
    {
        tEvent |= MOUSEEVENT_BUTTON_CHANGED;
    }
    if (EndpointIN->pu8Buffer[1] != 0)
    {
        tEvent |= MOUSEEVENT_POSITIONX_CHANGED;
    }
    if (EndpointIN->pu8Buffer[2] != 0)
    {
        tEvent |= MOUSEEVENT_POSITIONY_CHANGED;
    }
    if (EndpointIN->pu8Buffer[3] != 0)
    {
        tEvent |= MOUSEEVENT_SCROLLING_CHANGED;
    }
    
    if (((int8_t)EndpointIN->pu8Buffer[1]) > 0)
    {
        if (((int8_t)EndpointIN->pu8Buffer[1]) > (0xFFFFFFFF - stcMouseData.stcPosition.u32X))
        {
            stcMouseData.stcPosition.u32X = 0xFFFFFFFF;
        }
        else
        {
            stcMouseData.stcPosition.u32X += ((int8_t)EndpointIN->pu8Buffer[1]);
        }
    }
    else if (((int8_t)EndpointIN->pu8Buffer[1]) < 0)
    {
        if ((-((int8_t)EndpointIN->pu8Buffer[1])) > stcMouseData.stcPosition.u32X)
        {
            stcMouseData.stcPosition.u32X = 0;
        }
        else
        {
            stcMouseData.stcPosition.u32X += ((int8_t)EndpointIN->pu8Buffer[1]);
        }
    }
    
    if (((int8_t)EndpointIN->pu8Buffer[2]) > 0)
    {
        if (((int8_t)EndpointIN->pu8Buffer[2]) > (0xFFFFFFFF - stcMouseData.stcPosition.u32Y))
        {
            stcMouseData.stcPosition.u32Y = 0xFFFFFFFF;
        }
        else
        {
            stcMouseData.stcPosition.u32Y += ((int8_t)EndpointIN->pu8Buffer[2]);
        }
    }
    else if (((int8_t)EndpointIN->pu8Buffer[2]) < 0)
    {
        if ((-((int8_t)EndpointIN->pu8Buffer[2])) > stcMouseData.stcPosition.u32Y)
        {
            stcMouseData.stcPosition.u32Y = 0;
        }
        else
        {
            stcMouseData.stcPosition.u32Y += ((int8_t)EndpointIN->pu8Buffer[2]);
        }
    }
    stcMouseData.tButtons = EndpointIN->pu8Buffer[0];
    stcMouseData.i8Scrolling = (int8_t)(EndpointIN->pu8Buffer[3]);
    
    if (stcMouseData.i8Scrolling > 0)
    {
        stcMouseData.u32ScrollPosition++;
    }
    if (stcMouseData.i8Scrolling < 0)
    {
        stcMouseData.u32ScrollPosition--;
    }
    
    if ((tEvent) && (MouseEventHandler))
    {
        MouseEventHandler(tEvent,stcMouseData);
    }
    EndpointIN->CompletionHandler = HidMouse_DataReceivedHandler;
}

Point_t HidMouse_GetCurrentPosition()
{
    return stcMouseData.stcPosition;
}

void HidMouse_SetCurrentPosition(uint32_t u32X, uint32_t u32Y)
{
    stcMouseData.stcPosition.u32X = u32X;
    stcMouseData.stcPosition.u32Y = u32Y;
}

uint32_t HidMouse_GetCurrentScrollPosition()
{
    return stcMouseData.u32ScrollPosition;
}

void HidMouse_SetCurrentScrollPosition(uint32_t u32ScrollPosition)
{
    stcMouseData.u32ScrollPosition = u32ScrollPosition;
}

Buttons_t HidMouse_GetButtons()
{
    return stcMouseData.tButtons;
}

boolean_t HidMouse_Moved()
{
    if (bMouseMoved)
    {
        bMouseMoved = FALSE;
        return TRUE;
    }
    return FALSE;
}



boolean_t HidMouseDeinitHandler()
{
    return TRUE;
}

void HidMouse_Init()
{

}

#endif /* (USE_USB_HOST == 1) */
