/*!
 ******************************************************************************
 **
 ** \file usb_core_fm3.c
 **
 ** \brief This file provides all USB standard request handler functions 
 **
 ** \author FSLA AE Team
 **
 ** \version V0.10
 **
 ** \date 2011-12-18
 **
 ** \attention THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU SEMICONDUCTOR
 **            ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
 **            OMMISSIONS.
 **
 ** (C) Copyright 200x-201x by Fujitsu Semiconductor(Shanghai) Co.,Ltd.
 **
 ******************************************************************************
 **
 ** \note Other information.
 **
 ******************************************************************************
 */
#define USB_CORE_GLOBAL

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "usb_core_fm3.h"

/*---------------------------------------------------------------------------*/
/* local data                                                                */
/*---------------------------------------------------------------------------*/
static USB_DevCtrlT g_UsbDevCtrl;
static uint32_t g_InData;

/*---------------------------------------------------------------------------*/
/* local functions prototypes                                                */
/*---------------------------------------------------------------------------*/
static void USB_StatusEndHandler (USB_EpIndexT EP);
static void USB_AddStatusEndHandler (USB_EpIndexT EP);
static pUSB_StandardEpDescriptorT USB_FindEP(pUSB_StandardInterfaceDescriptorT pInterface,
                                     pUSB_StandardEpDescriptorT pEp);
static pUSB_StandardInterfaceDescriptorT USB_FindInterface (
                      USB_StandardConfigurationDescriptorT * pConfiguration,
                      uint32_t Interface, uint32_t AlternateSetting);
static USB_StandardConfigurationDescriptorT * USB_FindConfiguration(uint32_t Configuration);
static USB_CommStatusT USB_SetConfigurtonState (uint32_t Configuration);
static USB_CommStatusT USB_DevStatus(uint16_t Index);
static USB_CommStatusT USB_InterfaceStatus(uint16_t Index);
static USB_CommStatusT USB_EpStatus(uint16_t Index);
static void USB_GetStatus (void);
static USB_CommStatusT USB_ClearDevFeature(uint16_t Feature,uint16_t Index);
static USB_CommStatusT USB_ClearInterfaceFeature(uint16_t Feature,uint16_t Index);
static USB_CommStatusT USB_ClearEpFeature(uint16_t Feature,uint16_t Index);
static void USB_ClearFeature (void);
static USB_CommStatusT USB_SetDevFeature(uint16_t Feature,uint16_t Index);
static USB_CommStatusT USB_SetInterfaceFeature(uint16_t Feature,uint16_t Index);
static USB_CommStatusT USB_SetEpFeature(uint16_t Feature,uint16_t Index);
static void USB_SetFeature (void);
static void USB_SetAddress (void);
static USB_CommStatusT USB_GetDescriptorDevice(uint8_t Index);
static USB_CommStatusT USB_GetDescriptorConfiguration(uint8_t Index);
static USB_CommStatusT USB_GetDescriptorString(uint8_t Index);
static void USB_GetDescriptor (void);
static void USB_SetDescriptor(void);
static void USB_GetConfiguration(void);
static void USB_SetConfiguration(void);
static void USB_GetInterface (void);
static void USB_SetInterface (void);
static void USB_SynchFrame (void);
static void USB_EpInterfaceCnfg(pUSB_StandardInterfaceDescriptorT pInterface,
                                      uint8_t Configure);
/*!
 ******************************************************************************
 ** \brief Init USB core
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
void USB_CoreInit (void)
{
    g_UsbDevCtrl.State.DS      = UsbDevStatusUnknow;
    g_UsbDevCtrl.State.Suspend = FALSE;
    g_UsbDevCtrl.Feature.RemoteWakeupEnable = FALSE;
    g_UsbDevCtrl.Feature.SelfPowered = USB_SELF_POWERED;
#if USB_HIGH_SPEED > 0
    g_UsbDevCtrl.HighSpeed = 0;
#endif // USB_HIGH_SPEED > 0
    // Init HW
    USB_HwInit();
}

/*!
 ******************************************************************************
 ** \brief Get device states
 **
 ** \param Type USB core request type
 **
 ** \return device states
 **
 ******************************************************************************
 */ 
uint32_t USB_CoreReq (USB_CoreReqTypeT Type)
{
    switch(Type)
    {
        case UsbCoreReqDevState:
            return(g_UsbDevCtrl.State.DS);
        case UsbCoreReqDevSusState:
            return(g_UsbDevCtrl.State.Suspend);
        case UsbCoreReqConfiquration:
            return(g_UsbDevCtrl.Configuration);
        case UsbCoreReqInterface:
            return(g_UsbDevCtrl.Interfaces);
        case UsbCoreReqDevOwnAddress:
            return(g_UsbDevCtrl.DevAdd);
        case UsbCoreReqWakeUpEnableStatus:
            return(g_UsbDevCtrl.Feature.RemoteWakeupEnable);
        case UsbCoreReqSelfPoweredStatus:
            return(USB_SELF_POWERED);
    #if USB_HIGH_SPEED > 0
        case UsbCoreReqSpeed:
            return(g_UsbDevCtrl.HighSpeed);
    #endif // USB_HIGH_SPEED > 0
    }
    return((uint32_t) -1);
}

#if USB_REMOTE_WAKEUP != 0
/*!
 ******************************************************************************
 ** \brief Wake-up device from suspend mode
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */ 
void USB_WakeUp (void)
{
    if(g_UsbDevCtrl.Feature.RemoteWakeupEnable && g_UsbDevCtrl.State.Suspend)
    {
        USB_HwWakeUp();
    }
}
#endif // USB_REMOTE_WAKEUP != 0

/*!
 ******************************************************************************
 ** \brief Set device state
 **
 ** \param DevState Device state
 **
 ** \return None
 **
 ******************************************************************************
 */ 
void USB_SetDevState (USB_CoreDevStateT DevState)
{
    uint32_t i;
    pUSB_StandardInterfaceDescriptorT pInterface;
    if(g_UsbDevCtrl.State.DS == UsbDevStatusConfigured)
    {
        // deconfigure device
        for (i = 0; i < g_UsbDevCtrl.Interfaces; ++i)
        {
            pInterface = USB_FindInterface(g_UsbDevCtrl.pCnfg,i,g_UsbDevCtrl.AlternateSetting[i]);
            USB_EpInterfaceCnfg(pInterface,FALSE);
        }
    }
    switch (DevState)
    {
        case UsbDevStatusAttached:
        case UsbDevStatusPowered:
        case UsbDevStatusDefault:
            USB_ConfigureHook(NULL);
            g_UsbDevCtrl.DevAdd = g_UsbDevCtrl.Configuration = 0;
            break;
        case UsbDevStatusAddress:
            g_UsbDevCtrl.Configuration = 0;
            break;
        case UsbDevStatusConfigured:
            // configure device
            for (i = 0; i < g_UsbDevCtrl.Interfaces; ++i)
            {
                pInterface = USB_FindInterface(g_UsbDevCtrl.pCnfg,i,g_UsbDevCtrl.AlternateSetting[i]);
                USB_EpInterfaceCnfg(pInterface,TRUE);
            }
            break;
        default:  // unknown state - disconnected or in process of a connection
            USB_ConfigureHook(NULL);
            g_UsbDevCtrl.DevAdd = g_UsbDevCtrl.Configuration = 0;
            break;
    }
    g_UsbDevCtrl.State.DS = DevState;
}

/*!
 ******************************************************************************
 ** \brief USB connect callback
 **
 ** \param Connect indicate whether the device is connected or disconnected
 **
 ** \return None
 **
 ******************************************************************************
 */
void USB_DevConnectCallback (uint8_t Connect)
{
    USB_SetDevState(Connect?UsbDevStatusAttached:UsbDevStatusUnknow);
    USB_ConnectHook(Connect);
}

/*!
 ******************************************************************************
 ** \brief USB suspend callback
 **
 ** \param Suspend Indicate whether the device is suspended or not
 **
 ** \return None
 **
 ******************************************************************************
 */
void USB_DevSuspendCallback (uint8_t Suspend)
{
    g_UsbDevCtrl.State.Suspend = Suspend ? UsbDevStatusSuspend : UsbDevStatusNoSuspend;
    USB_SuspendHook(Suspend);
}

/*!
 ******************************************************************************
 ** \brief USB Reset callback
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
void USB_DevResetCallback (void)
{
    USB_SetDevState(UsbDevStatusDefault);
    USB_ResetHook();
}

#if USB_HIGH_SPEED > 0
/*!
 ******************************************************************************
 ** \brief USB Device high speed callback
 **
 ** \param HighSpeed Indicate the device is high speed or not
 **
 ** \return None
 **
 ******************************************************************************
 */
void USB_DevSpeedCallback(uint8_t HighSpeed)
{
    g_UsbDevCtrl.HighSpeed = HighSpeed;
}
#endif // USB_HIGH_SPEED > 0

/*!
 ******************************************************************************
 ** \brief Get lower from Host expected size and size and set
 **        a flag bZeroPacketPossible when size is lower that host 
 **        expected size
 **
 ** \param Size actual transfer size
 ** \param HostExpSize Expected size of host
 **
 ** \return lower of Size and HostExpSize
 **
 ******************************************************************************
 */
uint32_t USB_CompareSize(uint32_t Size,uint32_t HostExpSize)
{
    uint32_t ActSize = HostExpSize;
    if(Size < HostExpSize)
    {
        ActSize = Size;
        g_EpCnfg[CTRL_ENP_IN].bZeroPacketPossible = 1;
    }
    return(ActSize);
}

/*!
 ******************************************************************************
 ** \brief Setup packet handler
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
void USB_SetupHandler (void)
{
    switch(g_UsbEp0SetupPacket.mRequestType.Type)
    {
        // Standard
        case UsbTypeStandart:
        // Decoding standard request
        switch (g_UsbEp0SetupPacket.bRequest)
        {
            case GET_STATUS:
                USB_GetStatus();
            #ifdef DEBUG_MODE
                printf("GET_STATUS[STD]\n");
            #endif
                break;
            case CLEAR_FEATURE:
                USB_ClearFeature();
            #ifdef DEBUG_MODE
                printf("CLEAR_FEATURE[STD]\n");
            #endif
                break;
            case SET_FEATURE:
                USB_SetFeature();
            #ifdef DEBUG_MODE
                printf("SET_FEATURE[STD]\n");
            #endif
                break;
            case SET_ADDRESS:
                USB_SetAddress();
            #ifdef DEBUG_MODE
                printf("SET_ADDRESS[STD]\n");
            #endif
                break;
            case GET_DESCRIPTOR:
                if(g_UsbEp0SetupPacket.mRequestType.Recipient == UsbRecipientDevice)
                {
                    USB_GetDescriptor();
                }
                // Only get descriptor for device is standard request
                else if ((g_UsbEp0SetupPacket.mRequestType.Dir == UsbDevice2Host) &&
                (USB_GetDescriptorHook(&g_UsbEp0SetupPacket) == UsbPass))
                {
                }
                else
                {
                    USB_CORE_ERROR_REQUEST();
                }
                break;
            case SET_DESCRIPTOR:
                // Optional (only for configuration and string descriptors)
                USB_SetDescriptor();
            #ifdef DEBUG_MODE
                printf("SET_DESCRIPTOR[STD]\n");
            #endif 
                break;
            case GET_CONFIGURATION:
                USB_GetConfiguration();
            #ifdef DEBUG_MODE
                printf("GET_CONFIGURATION[STD]\n");
            #endif
                break;
            case SET_CONFIGURATION:
                USB_SetConfiguration();
            #ifdef DEBUG_MODE
                printf("SET_CONFIGURATION[STD]\n");
            #endif
                break;
            case GET_INTERFACE:
            #ifdef DEBUG_MODE
                printf("GET_INTERFACE[STD]\n");
            #endif
                USB_GetInterface();
                break;
            case SET_INTERFACE:
            #ifdef DEBUG_MODE
                printf("SET_INTERFACE[STD]\n");
            #endif
                USB_SetInterface();
                break;
            case SYNCH_FRAME:
            #ifdef DEBUG_MODE
                printf("SYNCH_FRAME[STD]\n");
            #endif
                USB_SynchFrame();
                break;
            default:
                USB_CORE_ERROR_REQUEST();
        }
        break;
        // Class
        case UsbTypeClass:
            if(USB_ClassRequestHook(&g_UsbEp0SetupPacket) != UsbPass)
            {
                USB_CORE_ERROR_REQUEST();
            }
            else
            {
            }
            break;
        // Vendor
        case UsbTypeVendor:
            if( USB_VendorRequestHook(&g_UsbEp0SetupPacket) != UsbPass)
            {
                USB_CORE_ERROR_REQUEST();
            }
            else
            {
            }
            break;
        // Other
        default:
            USB_CORE_ERROR_REQUEST();
    }
}

/*!
 ******************************************************************************
 ** \brief USB status handler
 **
 ** \param Ep Endpoint index
 **
 ** \return None
 **
 ******************************************************************************
 */
void USB_StatusHandler(USB_EpIndexT Ep)
{
    if (g_UsbEp0SetupPacket.mRequestType.Dir == UsbDevice2Host)
    {
        USB_StatusPhase(CTRL_ENP_OUT);
        g_EpCnfg[CTRL_ENP_OUT].pFn = (void *)USB_StatusEndHandler;
    }
    else
    {
        USB_StatusPhase(CTRL_ENP_IN);
        g_EpCnfg[CTRL_ENP_IN].pFn = (g_UsbEp0SetupPacket.bRequest == SET_ADDRESS)
                                   ?(void *)USB_AddStatusEndHandler
                                   :(void *)USB_StatusEndHandler;
    }
}

/*!
 ******************************************************************************
 ** \brief Setup packet status phase end handler
 **
 ** \param Ep Endpoint index
 **
 ** \return None
 **
 ******************************************************************************
 */
static
void USB_StatusEndHandler (USB_EpIndexT EP)
{
    USB_DataTransfer(CTRL_ENP_IN ,NULL,(uint32_t)-1,NULL);
    USB_DataTransfer(CTRL_ENP_OUT,NULL,(uint32_t)-1,NULL);
}

/*!
 ******************************************************************************
 ** \brief Setup packet - set address status phase end handler
 **
 ** \param Ep Endpoint index
 **
 ** \return None
 **
 ******************************************************************************
 */
static
void USB_AddStatusEndHandler (USB_EpIndexT EP)
{
    // Device address is set after status phase of set address request
    g_UsbDevCtrl.DevAdd = g_UsbEp0SetupPacket.wValue.Lo;
    if(g_UsbDevCtrl.DevAdd)
    {
        USB_SetDevState(UsbDevStatusAddress);
    }
    else
    {
        // when address is 0 put device init configuration state
        USB_SetDevState(UsbDevStatusDefault);
    }
    USB_DataTransfer(CTRL_ENP_IN ,NULL,(uint32_t)-1,NULL);
    USB_DataTransfer(CTRL_ENP_OUT,NULL,(uint32_t)-1,NULL);
}

/*!
 ******************************************************************************
 ** \brief Set Interface and  Alternative setting state
 **
 ** \param pConfiguration Point to configuration descriptor
 ** \param Interface Interface number
 ** \param AlternateSetting Alternate setting
 **
 ** \return Interface descriptor
 **
 ******************************************************************************
 */
static
pUSB_StandardInterfaceDescriptorT USB_FindInterface (
                      USB_StandardConfigurationDescriptorT * pConfiguration,
                      uint32_t Interface, uint32_t AlternateSetting)
{
    pUSB_StandardInterfaceDescriptorT pData;
    ASSERT(pConfiguration != NULL);
    // Find Interface
    for( pData = (USB_StandardInterfaceDescriptorT *)pConfiguration;
      (uint8_t*)pData < (uint8_t *)pConfiguration + pConfiguration->wTotalLength;
       pData =(USB_StandardInterfaceDescriptorT *)((uint8_t *)pData + pData->bLength))
    {
        if((pData->bDescriptorType == UsbDescriptorInterface) &&
           (pData->bInterfaceNumber == Interface) &&
           (pData->bAlternateSetting == AlternateSetting))
        {
            return(pData);
        }
    }
    return(NULL);
}


/*!
 ******************************************************************************
 ** \brief Set Interface and  Alternative setting state
 **
 ** \param Configuration Configuration number
 **
 ** \return Configuration descriptor
 **
 ******************************************************************************
 */
static
USB_StandardConfigurationDescriptorT * USB_FindConfiguration(uint32_t Configuration)
{
#if USB_HIGH_SPEED > 0
    const uint8_t ** ppCfgArray = (const uint8_t**)g_UsbDescArray[(g_UsbDevCtrl.HighSpeed?USB_HS_CFG_ARRAY:USB_FS_CFG_ARRAY)];
#else
    const uint8_t ** ppCfgArray = (const uint8_t**)g_UsbDescArray[USB_FS_CFG_ARRAY];
#endif
    pUSB_StandardConfigurationDescriptorT pData = (pUSB_StandardConfigurationDescriptorT)*ppCfgArray;
    while((pData = (pUSB_StandardConfigurationDescriptorT)*ppCfgArray) != NULL)
    {
        if(pData->bDescriptorType == UsbDescriptorConfiguration)
        {
            if(pData->bConfigurationValue == Configuration)
            {
                return(pData);
            }

        }
        ++*ppCfgArray;
    }
    return(NULL);
}

/*!
 ******************************************************************************
 ** \brief Get the pointer to next EP 
 **
 ** \param pInterface Point to interface descriptor
 ** \param pEp Point to endpoint descriptor
 **
 ** \return Pointer to next EP
 **
 ******************************************************************************
 */
static
pUSB_StandardEpDescriptorT USB_FindEP(pUSB_StandardInterfaceDescriptorT pInterface,
                                     pUSB_StandardEpDescriptorT pEp)
{
    pUSB_StandardEpDescriptorT pEpNext;
    static uint32_t ep_numbers;
    ASSERT(pInterface != NULL);
    if (pEp == NULL)
    {
        ep_numbers = pInterface->bNumEndpoints;
        pEpNext = (pUSB_StandardEpDescriptorT)(((uint8_t*)pInterface)+pInterface->bLength);
    }
    else
    {
        pEpNext = (pUSB_StandardEpDescriptorT)(((uint8_t*)pEp) + pEp->bLength);
    }
    // Find Interface
    for(;ep_numbers != 0 && pEpNext->bLength;
    pEpNext = (pUSB_StandardEpDescriptorT)(((uint8_t*)pEpNext) + pEpNext->bLength))
    {
        if(pEpNext->bDescriptorType == UsbDescriptorEp)
        {
            --ep_numbers;
            return(pEpNext);
        }
        else if (pEpNext->bDescriptorType == UsbDescriptorInterface ||
        pEpNext->bDescriptorType == UsbDescriptorConfiguration)
        {
            break;
        }
    }
    return(NULL);
}

/*!
 ******************************************************************************
 ** \brief Set Configuration state 
 **
 ** \param Configuration Configuration number
 **
 ** \return USB common status
 **
 ******************************************************************************
 */
static
USB_CommStatusT USB_SetConfigurtonState (uint32_t Configuration)
{
    uint32_t i;
    pUSB_StandardConfigurationDescriptorT pCnfg;
    if(Configuration)
    {
        // Find configuration
        pCnfg = USB_FindConfiguration(Configuration);
        if(pCnfg == NULL)
        {
            return(UsbFault);
        }

        // deinit current configuration
        USB_SetDevState(UsbDevStatusAddress);
        USB_ConfigureHook(NULL);
        g_UsbDevCtrl.pCnfg = pCnfg;

        // Set Interface and Alternative Setting
        g_UsbDevCtrl.Configuration = Configuration;
        g_UsbDevCtrl.Feature.RemoteWakeupEnable = 0;
        g_UsbDevCtrl.Feature.SelfPowered = pCnfg->bmAttributes.SelfPowered?1:0;
        // configure all existing interfaces alternative  setting 0
        // Whether the interface exist into current configuration?
        g_UsbDevCtrl.Interfaces = pCnfg->bNumInterfaces;
        for(i = 0; i < g_UsbDevCtrl.Interfaces; i++)
        {
            g_UsbDevCtrl.AlternateSetting[i] = 0;
        }
        USB_SetDevState(UsbDevStatusConfigured);
        USB_ConfigureHook(&g_UsbDevCtrl);
    }
    else
    {
        USB_ConfigureHook(NULL);
        USB_SetDevState(UsbDevStatusAddress);
    }
    return(UsbPass);
}

/*!
 ******************************************************************************
 ** \brief Get USB device status 
 **
 ** \param Index Indicate whether the parameter is valid or invalid
 **
 ** \return USB common status
 **
 ******************************************************************************
 */
static
USB_CommStatusT USB_DevStatus(uint16_t Index)
{
    if(Index == 0)
    {
        g_InData = ((uint32_t)g_UsbDevCtrl.Feature.Data) & 0xFF;
        USB_DataTransfer(CTRL_ENP_IN,(uint8_t*)&g_InData,2,(void*)USB_StatusHandler);
        return(UsbPass);
    }
    return(UsbFault);
}

/*!
 ******************************************************************************
 ** \brief Get interface status
 **
 ** \param Index Invalid
 **
 ** \return USB common status
 **
 ******************************************************************************
 */
static
USB_CommStatusT USB_InterfaceStatus(uint16_t Index)
{
    g_InData = 0;
    USB_DataTransfer(CTRL_ENP_IN,(uint8_t*)&g_InData,2,(void*)USB_StatusHandler);
    return(UsbPass);
}

/*!
 ******************************************************************************
 ** \brief Get EP status
 **
 ** \param Index Check if the parameter is valid
 **
 ** \return USB common status
 **
 ******************************************************************************
 */
static
USB_CommStatusT USB_EpStatus(uint16_t Index)
{
    if((Index & 0x7F) > 16)
    {
        return(UsbFault);
    }
    g_InData = 0;
    USB_GetStallEP(USB_EpLogToPhysAdd(Index),(uint8_t*)&g_InData);
    USB_DataTransfer(CTRL_ENP_IN,(uint8_t*)&g_InData,2,(void*)USB_StatusHandler);
    return(UsbPass);
}

/*!
 ******************************************************************************
 ** \brief USB Get status implement
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
static
void USB_GetStatus (void)
{
    if((g_UsbDevCtrl.State.DS >= UsbDevStatusAddress) &&
        // Length must be 2
        (g_UsbEp0SetupPacket.wLength.Word == 2) &&
        // Direction of data from device
        (g_UsbEp0SetupPacket.mRequestType.Dir == 1) &&
        // wValue must be 0
        (g_UsbEp0SetupPacket.wValue.Word == 0) )
    {
        // This request is valid only for Address and Configured device's states
        switch(g_UsbEp0SetupPacket.mRequestType.Recipient)
        {
            case UsbRecipientDevice:
                if(USB_DevStatus(g_UsbEp0SetupPacket.wIndex.Word) != UsbPass)
                {
                    USB_CORE_ERROR_REQUEST();
                }
                break;
            case UsbRecipientInterface:
                if(USB_InterfaceStatus(g_UsbEp0SetupPacket.wIndex.Word) != UsbPass)
                {
                    USB_CORE_ERROR_REQUEST();
                }
                break;
            case UsbRecipientEndpoint:
                if(USB_EpStatus(g_UsbEp0SetupPacket.wIndex.Word) != UsbPass)
                {
                    USB_CORE_ERROR_REQUEST();
                }
                break;
            default:
                USB_CORE_ERROR_REQUEST();
        }
    }
    else
    {
        USB_CORE_ERROR_REQUEST();
    }
}

/*!
 ******************************************************************************
 ** \brief USB Clear Device feature implement
 **
 ** \param Feature Indicate the feature selector
 ** \param Index Check if parameter is valid
 **
 ** \return USB common status
 **
 ******************************************************************************
 */
static
USB_CommStatusT USB_ClearDevFeature(uint16_t Feature,uint16_t Index)
{
    if((Index != 0) || (Feature != UsbDeviceRemoteWakeupSelector))
    {
        return(UsbFault);
    }
    g_UsbDevCtrl.Feature.RemoteWakeupEnable = 0;
    return(UsbPass);
}

/*!
 ******************************************************************************
 ** \brief Clear interface feature
 **
 ** \param Feature Indicate the feature selector
 ** \param Index Check if parameter is valid
 **
 ** \return USB common status
 **
 ******************************************************************************
 */
static
USB_CommStatusT USB_ClearInterfaceFeature(uint16_t Feature,uint16_t Index)
{
    return(UsbFault);
}

/*!
 ******************************************************************************
 ** \brief USB Clear EP feature implement
 **
 ** \param Feature Indicate the feature selector
 ** \param Index Check if parameter is valid
 **
 ** \return USB common status
 **
 ******************************************************************************
 */
static
USB_CommStatusT USB_ClearEpFeature(uint16_t Feature,uint16_t Index)
{
    USB_EpIndexT UsbEpAdd;
    if(((Index & 0x7F) > 16) || (Feature != UsbEpHaltSelector))
    {
        return(UsbFault);
    }
    UsbEpAdd = USB_EpLogToPhysAdd(Index);
    USB_SetStallEP(UsbEpAdd,FALSE);
    USB_EpUnstallHook(UsbEpAdd);
    return(UsbPass);
}

/*!
 ******************************************************************************
 ** \brief USB Clear feature implement
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
static
void USB_ClearFeature (void)
{
    if((g_UsbDevCtrl.State.DS >= UsbDevStatusAddress) &&
        // Length must be 0
        (g_UsbEp0SetupPacket.wLength.Word == 0))
    {
        switch(g_UsbEp0SetupPacket.mRequestType.Recipient)
        {
            case UsbRecipientDevice:
                if(USB_ClearDevFeature(g_UsbEp0SetupPacket.wValue.Word,g_UsbEp0SetupPacket.wIndex.Word) == UsbPass)
                {
                    USB_StatusHandler(CTRL_ENP_OUT);
                }
                else
                {
                    USB_CORE_ERROR_REQUEST();
                }
                break;
            case UsbRecipientInterface:
                if((g_UsbDevCtrl.State.DS > UsbDevStatusAddress) &&
                    (USB_ClearInterfaceFeature(g_UsbEp0SetupPacket.wValue.Word,g_UsbEp0SetupPacket.wIndex.Word) == UsbPass))
                {
                    USB_DataTransfer(CTRL_ENP_IN,NULL,0,(void*)USB_StatusHandler);
                }
                else
                {
                    USB_CORE_ERROR_REQUEST();
                }
                break;
            case UsbRecipientEndpoint:
                if(((g_UsbDevCtrl.State.DS > UsbDevStatusAddress) || ((g_UsbEp0SetupPacket.wIndex.Word & 0x7F) == 0)) &&
                (USB_ClearEpFeature(g_UsbEp0SetupPacket.wValue.Word,g_UsbEp0SetupPacket.wIndex.Word) == UsbPass))
                {
                    USB_StatusHandler(CTRL_ENP_OUT);
                }
                else
                {
                    USB_CORE_ERROR_REQUEST();
                }
                break;
            default:
                USB_CORE_ERROR_REQUEST();
        }
    }
    else
    {
        USB_CORE_ERROR_REQUEST();
    }
}

/*!
 ******************************************************************************
 ** \brief USB Set Device feature implement
 **
 ** \param Feature Indicate the feature selector
 ** \param Index Check if parameter is valid
 **
 ** \return USB common status
 **
 ******************************************************************************
 */
static
USB_CommStatusT USB_SetDevFeature(uint16_t Feature,uint16_t Index)
{
    if((Index != 0) || (Feature != UsbDeviceRemoteWakeupSelector))
    {
        return(UsbFault);
    }
    g_UsbDevCtrl.Feature.RemoteWakeupEnable = 1;
    return(UsbPass);
}

/*!
 ******************************************************************************
 ** \brief USB Set Interface feature implement
 **
 ** \param Feature Indicate the feature selector
 ** \param Index Check if parameter is valid
 **
 ** \return USB common status
 **
 ******************************************************************************
 */
static
USB_CommStatusT USB_SetInterfaceFeature(uint16_t Feature,uint16_t Index)
{
    return(UsbFault);
}

/*!
 ******************************************************************************
 ** \brief USB Set EP feature implement
 **
 ** \param Feature Indicate the feature selector
 ** \param Index Check if parameter is valid
 **
 ** \return USB common status
 **
 ******************************************************************************
 */
static
USB_CommStatusT USB_SetEpFeature(uint16_t Feature,uint16_t Index)
{
    USB_EpIndexT UsbEpAdd;
    if(((Index & 0x7F) > 16) || (Feature != UsbEpHaltSelector))
    {
        return(UsbFault);
    }

    UsbEpAdd = USB_EpLogToPhysAdd(Index);
    USB_SetStallEP(UsbEpAdd,TRUE);
    USB_EpStallHook(UsbEpAdd);
    return(UsbPass);
}

/*!
 ******************************************************************************
 ** \brief USB Set feature implement
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
static
void USB_SetFeature (void)
{
    if((g_UsbDevCtrl.State.DS >= UsbDevStatusAddress) &&
        // Length must be 0
        (g_UsbEp0SetupPacket.wLength.Word == 0))
    {
        switch(g_UsbEp0SetupPacket.mRequestType.Recipient)
        {
            case UsbRecipientDevice:
                if(USB_SetDevFeature(g_UsbEp0SetupPacket.wValue.Word,g_UsbEp0SetupPacket.wIndex.Word) == UsbPass)
                {
                    USB_StatusHandler(CTRL_ENP_OUT);
                }
                else
                {
                    USB_CORE_ERROR_REQUEST();
                }
                break;
            case UsbRecipientInterface:
                if((g_UsbDevCtrl.State.DS > UsbDevStatusAddress) &&
                    (USB_SetInterfaceFeature(g_UsbEp0SetupPacket.wValue.Word,g_UsbEp0SetupPacket.wIndex.Word) == UsbPass))
                {
                    USB_StatusHandler(CTRL_ENP_OUT);
                }
                else
                {
                    USB_CORE_ERROR_REQUEST();
                }
                break;
            case UsbRecipientEndpoint:
                if(((g_UsbDevCtrl.State.DS > UsbDevStatusAddress) || ((g_UsbEp0SetupPacket.wIndex.Word & 0x7F) == 0)) &&
                    (USB_SetEpFeature(g_UsbEp0SetupPacket.wValue.Word,g_UsbEp0SetupPacket.wIndex.Word) == UsbPass))
                {
                    USB_StatusHandler(CTRL_ENP_OUT);
                }
                else
                {
                    USB_CORE_ERROR_REQUEST();
                }
                break;
            default:
                USB_CORE_ERROR_REQUEST();
        }
    }
    else
    {
        USB_CORE_ERROR_REQUEST();
    }
}

/*!
 ******************************************************************************
 ** \brief USB Set address request implement
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
static
void USB_SetAddress (void)
{
    if((g_UsbDevCtrl.State.DS >= UsbDevStatusDefault) &&
        (g_UsbEp0SetupPacket.mRequestType.Recipient == 0) &&
        (g_UsbEp0SetupPacket.wIndex.Word  == 0) &&
        (g_UsbEp0SetupPacket.wLength.Word == 0) &&
        (g_UsbEp0SetupPacket.wValue.Word < 128))
    {
        USB_StatusHandler(CTRL_ENP_IN);
    }
    else
    {
        USB_CORE_ERROR_REQUEST();
    }
}

/*!
 ******************************************************************************
 ** \brief USB get device's descriptor request implement
 **
 ** \param Index Check if index is valid
 **
 ** \return USB common status
 **
 ******************************************************************************
 */
static
USB_CommStatusT USB_GetDescriptorDevice(uint8_t Index)
{
    if(Index == 0)
    {
        USB_DataTransfer(CTRL_ENP_IN,
	                   (uint8_t *)&g_UsbStandardDeviceDescriptorStr,
	                    USB_CompareSize(g_UsbStandardDeviceDescriptorStr.bLength,g_UsbEp0SetupPacket.wLength.Word),
	                   (void*)USB_StatusHandler);
    #ifdef DEBUG_MODE
        printf("GET_DEV_DESC[STD]\n");
    #endif
        return(UsbPass);
    }
    return(UsbFault);
}

/*!
 ******************************************************************************
 ** \brief USB get configuration's descriptor request implement
 **
 ** \param Index Check if index is valid
 **
 ** \return USB common status
 **
 ******************************************************************************
 */
static
USB_CommStatusT USB_GetDescriptorConfiguration(uint8_t Index)
{
    pUSB_StandardConfigurationDescriptorT pCfg;
    if (Index >= g_UsbStandardDeviceDescriptorStr.bNumConfigurations)
    {
        return(UsbFault);
    }
#if USB_HIGH_SPEED > 0
    pCfg = (pUSB_StandardConfigurationDescriptorT)(*(uint8_t **)g_UsbDescArray[(g_UsbDevCtrl.HighSpeed?USB_HS_CFG_ARRAY:USB_FS_CFG_ARRAY)] + Index);
#else
    pCfg = (pUSB_StandardConfigurationDescriptorT)(*(uint8_t **)g_UsbDescArray[USB_FS_CFG_ARRAY] + Index);
#endif // USB_HIGH_SPEED > 0
    USB_DataTransfer(CTRL_ENP_IN,
		             (uint8_t*)pCfg,
		              USB_CompareSize(pCfg->wTotalLength,g_UsbEp0SetupPacket.wLength.Word),
		             (void*)USB_StatusHandler);
#ifdef DEBUG_MODE
    printf("GET_CNFG_DESC[STD]\n");
#endif
    return(UsbPass);
}

/*!
 ******************************************************************************
 ** \brief USB get string's descriptor request implement
 **
 ** \param Index Check if index is valid
 **
 ** \return USB common status
 **
 ******************************************************************************
 */
static
USB_CommStatusT USB_GetDescriptorString(uint8_t Index)
{
    uint8_t **ppData;
#if USB_HIGH_SPEED > 0
    uint8_t* pLangStr = (uint8_t*)g_UsbDescArray[(g_UsbDevCtrl.HighSpeed?USB_HS_LANG_STR:USB_FS_LANG_STR)];
#else
    uint8_t* pLangStr = (uint8_t*)g_UsbDescArray[USB_FS_LANG_STR];
#endif // USB_HIGH_SPEED > 0
    uint32_t i;
    if(Index == 0)
    {
        USB_DataTransfer(CTRL_ENP_IN,
                        pLangStr,
                        USB_CompareSize(pLangStr[UsbDescLengthOffset],g_UsbEp0SetupPacket.wLength.Word),
                        (void*)USB_StatusHandler);
    #ifdef DEBUG_MODE
        printf("GET_STR_DESC[STD]\n");
    #endif
    }
    else
    {
        uint32_t LanguagesOffset = 0;
        // Find Languages index
        for(i = 2; i < pLangStr[UsbDescLengthOffset]; i+=2)
        {
            if((pLangStr[i] == g_UsbEp0SetupPacket.wIndex.Lo) &&
                (pLangStr[i+1] == g_UsbEp0SetupPacket.wIndex.Hi))
            {
                break;
            }
            ++LanguagesOffset;
        }
        if(i >= pLangStr[UsbDescLengthOffset])
        {
            return(UsbFault);
        }
        // Find String index
#if USB_HIGH_SPEED > 0
        for(ppData = (*((uint8_t***)g_UsbDescArray[(g_UsbDevCtrl.HighSpeed?USB_HS_STR_ARRAY:USB_FS_STR_ARRAY)] + LanguagesOffset));
#else
        for(ppData = (*((uint8_t***)g_UsbDescArray[USB_FS_STR_ARRAY] + LanguagesOffset));
#endif // USB_HIGH_SPEED > 0
            ppData ;
            ++ppData)
        {
            if(--Index == 0)
            {
                break;
            }
        }
        if(ppData == NULL)
        {
            return(UsbFault);
        }

        USB_DataTransfer(CTRL_ENP_IN,
	                   (uint8_t*)*ppData,
	                    USB_CompareSize(*(*ppData+UsbDescLengthOffset),g_UsbEp0SetupPacket.wLength.Word),
	                   (void*)USB_StatusHandler);
    #ifdef DEBUG_MODE
        printf("GET_STR_DESC[STD]\n");
    #endif
    }
    return(UsbPass);
}

#if USB_HIGH_SPEED > 0
/*!
 ******************************************************************************
 ** \brief USB get device's qualifier descriptor request implement
 **
 ** \param Index Check if index is valid
 **
 ** \return USB common status
 **
 ******************************************************************************
 */
static
USB_CommStatusT UsbGetQualifierDescriptor(uint8_t Index)
{
    USB_DataTransfer(CTRL_ENP_IN,
		             (uint8_t *)&g_UsbQualifierDescriptor,
		              USB_CompareSize(g_UsbQualifierDescriptor.bLength,g_UsbEp0SetupPacket.wLength.Word),
		             (void*)USB_StatusHandler);
    #ifdef DEBUG_MODE
        printf("GET_DEVQLF_DESC[STD]\n");
    #endif
    return(UsbPass);
}

/*!
 ******************************************************************************
 ** \brief USB get device's other descriptor request implement
 **
 ** \param Index Check if index is valid
 **
 ** \return USB common status
 **
 ******************************************************************************
 */
static
USB_CommStatusT UsbGetOtherSpeedDescriptorConfiguration(uint8_t Index)
{
    pUSB_StandardConfigurationDescriptorT pCfg;
    if (Index >= g_UsbStandardDeviceDescriptorStr.bNumConfigurations)
    {
        return(UsbFault);
    }
    pCfg = (pUSB_StandardConfigurationDescriptorT)(*(uint8_t **)g_UsbDescArray[(g_UsbDevCtrl.HighSpeed?USB_HS_OTHER_CFG_ARRAY:USB_FS_OTHER_CFG_ARRAY)] + Index);
    USB_DataTransfer(CTRL_ENP_IN,
		             (uint8_t*)pCfg,
		              USB_CompareSize(pCfg->wTotalLength,g_UsbEp0SetupPacket.wLength.Word),
		             (void*)USB_StatusHandler);
    #ifdef DEBUG_MODE
        printf("GET_SPDCNFG_DESC[STD]\n");
    #endif
    return(UsbPass);
}

#endif // USB_HIGH_SPEED > 0


/*!
 ******************************************************************************
 ** \brief USB get descriptor request implement
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
static
void USB_GetDescriptor (void)
{
    if(g_UsbDevCtrl.State.DS >= UsbDevStatusDefault)
    {
        switch(g_UsbEp0SetupPacket.wValue.Hi)
        {
            case UsbDescriptorDevice:
                if(USB_GetDescriptorDevice(g_UsbEp0SetupPacket.wValue.Lo) != UsbPass)
                {
                    USB_CORE_ERROR_REQUEST();
                }
                else
                {
                }
                break;
            case UsbDescriptorConfiguration:
                if(USB_GetDescriptorConfiguration(g_UsbEp0SetupPacket.wValue.Lo) != UsbPass)
                {
                    USB_CORE_ERROR_REQUEST();
                }
                else
                {
                }
                break;
            case UsbDescriptorString:
                if(USB_GetDescriptorString(g_UsbEp0SetupPacket.wValue.Lo) != UsbPass)
                {
                    USB_CORE_ERROR_REQUEST();
                }
                else
                {
                }
                break;
        #if USB_HIGH_SPEED > 0
            case UsbDescriptorDeviceQualifier:
                if(UsbGetQualifierDescriptor(g_UsbEp0SetupPacket.wValue.Lo) != UsbPass)
                {
                    USB_CORE_ERROR_REQUEST();
                }
                else
                {
                }
                break;
            case UsbDescriptorOtherSeedConfiguration:
                if(UsbGetOtherSpeedDescriptorConfiguration(g_UsbEp0SetupPacket.wValue.Lo) != UsbPass)
                {
                    USB_CORE_ERROR_REQUEST();
                }
                else
                {
                }
                break;
        #endif // USB_HIGH_SPEED > 0
            default:
                if (USB_GetDescriptorHook(&g_UsbEp0SetupPacket) != UsbPass)
                {
                    USB_CORE_ERROR_REQUEST();
                }
                else
                {
                }
                break;
        }
    }
    else
    {
        USB_CORE_ERROR_REQUEST();
    }
}

/*!
 ******************************************************************************
 ** \brief USB Set descriptor
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
static
void USB_SetDescriptor(void)
{
    if(g_UsbDevCtrl.State.DS >= UsbDevStatusAddress)
    {
        // This request is valid only for Address state and Configured state.
        // If supported
        USB_CORE_ERROR_REQUEST();
    }
    else
    {
        USB_CORE_ERROR_REQUEST();
    }
}

/*!
 ******************************************************************************
 ** \brief USB Get configuration implement
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
static
void USB_GetConfiguration(void)
{
    if((g_UsbDevCtrl.State.DS >= UsbDevStatusAddress) &&
     (g_UsbEp0SetupPacket.wValue.Word == 0) &&
     (g_UsbEp0SetupPacket.wIndex.Word == 0) &&
     (g_UsbEp0SetupPacket.wLength.Word == 1))
    {
        g_InData = USB_CoreReq(UsbCoreReqConfiquration);
        USB_DataTransfer(CTRL_ENP_IN,(uint8_t*)&g_InData,1,(void*)USB_StatusHandler);
    }
    else
    {
        USB_CORE_ERROR_REQUEST();
    }
}

/*!
 ******************************************************************************
 ** \brief USB Set configuration implement
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
static
void USB_SetConfiguration(void)
{
    if((g_UsbDevCtrl.State.DS >= UsbDevStatusAddress) &&
        (g_UsbEp0SetupPacket.wIndex.Word == 0) &&
        (g_UsbEp0SetupPacket.wLength.Word == 0) &&
        (USB_SetConfigurtonState(g_UsbEp0SetupPacket.wValue.Lo) == UsbPass))
    {
        USB_StatusHandler(CTRL_ENP_OUT);
    }
    else
    {
        USB_CORE_ERROR_REQUEST();
    }
}

/*!
 ******************************************************************************
 ** \brief Implement get interface request
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
static
void USB_GetInterface (void)
{
    if((g_UsbDevCtrl.State.DS == UsbDevStatusConfigured) &&
        (g_UsbEp0SetupPacket.wIndex.Word <= g_UsbDevCtrl.Interfaces) &&
        (g_UsbEp0SetupPacket.wLength.Word == 1) &&
        (g_UsbEp0SetupPacket.wValue.Word == 0))
    {
        USB_DataTransfer(CTRL_ENP_IN,
                   (uint8_t*)&g_UsbDevCtrl.AlternateSetting[g_UsbEp0SetupPacket.wIndex.Word],
                    1,
                   (void*)USB_StatusHandler);
    }
    else
    {
        USB_CORE_ERROR_REQUEST();
    }
}

/*!
 ******************************************************************************
 ** \brief Implement set interface request
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
static
void USB_SetInterface (void)
{
    pUSB_StandardInterfaceDescriptorT pInterface, pInterfaceCurrent;

    if((g_UsbDevCtrl.State.DS == UsbDevStatusConfigured) &&
        (g_UsbEp0SetupPacket.wLength.Word == 0))
    {

        // Whether the interface is exist into current configuration?
        if((pInterface = USB_FindInterface(g_UsbDevCtrl.pCnfg,
            g_UsbEp0SetupPacket.wIndex.Word,
            g_UsbEp0SetupPacket.wValue.Word)) == NULL)
        {
            USB_CORE_ERROR_REQUEST();
            return;
        }

        // deconfigure current alternative setting EPs
        pInterfaceCurrent =  USB_FindInterface(g_UsbDevCtrl.pCnfg,
        g_UsbEp0SetupPacket.wIndex.Word,
        g_UsbDevCtrl.AlternateSetting[g_UsbEp0SetupPacket.wIndex.Word]);
        ASSERT(pInterfaceCurrent);
        USB_EpInterfaceCnfg(pInterfaceCurrent,FALSE);
        // set new alternative settings
        g_UsbDevCtrl.AlternateSetting[g_UsbEp0SetupPacket.wIndex.Word] = g_UsbEp0SetupPacket.wValue.Word;
        // configure EPs
        USB_EpInterfaceCnfg(pInterface,TRUE);

        USB_ConfigureHook(&g_UsbDevCtrl);
        USB_DataTransfer(CTRL_ENP_IN,NULL,0,(void*)USB_StatusHandler);
    }
    else
    {
        USB_CORE_ERROR_REQUEST();
    }
}


/*!
 ******************************************************************************
 ** \brief Configure/deconfigure EPs of a certain interface
 **
 ** \param EP Endpoint index
 **
 ** \return None
 **
 ******************************************************************************
 */
const UsbEP_ExtDataT * USB_EP_ExtFind(uint8_t EP)
{
#if USB_HIGH_SPEED > 0
    const UsbEP_ExtDescT ** pEP_ExtCfg = ((const UsbEP_ExtDescT **)g_UsbDescArray[(g_UsbDevCtrl.HighSpeed?USB_HS_EP_EXT_ARRAY:USB_FS_EP_EXT_ARRAY)]);
#else
    UsbEP_ExtDescT ** pEP_ExtCfg = ((UsbEP_ExtDescT **)g_UsbDescArray[USB_FS_EP_EXT_ARRAY]);
#endif
    while(*pEP_ExtCfg)
    {
        if ((*pEP_ExtCfg)->Configuration == g_UsbDevCtrl.Configuration &&
            (*pEP_ExtCfg)->EndpointAddress == EP)
        {
            return(&((*pEP_ExtCfg)->EP_ExtData));
        }
        ++pEP_ExtCfg;
    }
    return(NULL);
}

/*!
 ******************************************************************************
 ** \brief Configure/deconfigure EPs of a certain interface
 **
 ** \param pInterface Point to interface descriptor
 ** \param Configure Indicate whether configure the endpoint ot not
 **
 ** \return None
 **
 ******************************************************************************
 */
static
void USB_EpInterfaceCnfg(pUSB_StandardInterfaceDescriptorT pInterface,
                        uint8_t Configure)
{
    pUSB_StandardEpDescriptorT pEp = NULL;
    const UsbEP_ExtDataT * pUsbEPExtData;

    ASSERT(pInterface);
    while((pEp = USB_FindEP(pInterface,pEp)) != NULL)
    {
        pUsbEPExtData = USB_EP_ExtFind(pEp->bEndpointAddress);
        ASSERT(pUsbEPExtData);
        USB_RealizeEp(pEp,pUsbEPExtData,Configure);
    }
}

/*!
 ******************************************************************************
 ** \brief Implement synch frame request
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
static
void USB_SynchFrame (void)
{
    if( USB_SOF_EVENT &&
        (g_UsbDevCtrl.State.DS == UsbDevStatusConfigured) &&
        (g_UsbEp0SetupPacket.wLength.Word == 2) &&
        (g_UsbEp0SetupPacket.wValue.Word == 0))
    {
    #if USB_SOF_EVENT > 0
        g_InData = USB_GetFrameNumb();
    #else
        g_InData = 0;
    #endif
        USB_DataTransfer(CTRL_ENP_IN,(uint8_t*)&g_InData,2,(void*)USB_StatusHandler);
    }
    else
    {
        USB_CORE_ERROR_REQUEST();
    }
}


/*****************************************************************************/
/* END OF FILE */

