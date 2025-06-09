/*!
 ******************************************************************************
 **
 ** \file usb_core_fm3.c
 **
 ** \brief USB core head file
 **
 ** \author FSLA Team
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


#include "includes.h"

#ifndef __USB_CORE_FM3_H
#define __USB_CORE_FM3_H

#ifdef  USB_CORE_GLOBAL
#define USB_CORE_EXTERN
#else
#define USB_CORE_EXTERN  extern
#endif

/*!
 ******************************************************************************
 ** \brief USB common status enumeration
 ******************************************************************************
 */
typedef enum _USB_CommStatusT
{
  UsbPass = 0, UsbFault, UsbNotSupport,
} USB_CommStatusT;

/*!
 ******************************************************************************
 ** \brief USB Device state enumeration
 ******************************************************************************
 */
typedef enum _USB_CoreDevStateT
{
  UsbDevStatusUnknow = 0, UsbDevStatusAttached, UsbDevStatusPowered,
  UsbDevStatusDefault, UsbDevStatusAddress, UsbDevStatusConfigured
} USB_CoreDevStateT;

/*!
 ******************************************************************************
 ** \brief USB Setup type enumeration
 ******************************************************************************
 */
typedef enum _USB_SetupTypeT
{
  UsbTypeStandart = 0,UsbTypeClass,UsbTypeVendor,UsbTypeReserved
} USB_SetupTypeT;

/*!
 ******************************************************************************
 ** \brief USB Setup standard request enumeration
 ******************************************************************************
 */
typedef enum _USB_SetupStandatRecuestT
{
  GET_STATUS = 0,CLEAR_FEATURE, Reserved0, SET_FEATURE, Reserved1,
  SET_ADDRESS, GET_DESCRIPTOR, SET_DESCRIPTOR, GET_CONFIGURATION,
  SET_CONFIGURATION, GET_INTERFACE, SET_INTERFACE, SYNCH_FRAME
} USB_SetupStandatRequestT;

/*!
 ******************************************************************************
 ** \brief USB Setup Recipient enumeration
 ******************************************************************************
 */
typedef enum _USB_SetupRecipientT
{
  UsbRecipientDevice = 0,UsbRecipientInterface,UsbRecipientEndpoint,
  UsbRecipientOther,UsbRecipientReserved
} USB_SetupRecipientT;

/*!
 ******************************************************************************
 ** \brief USB Feature Selector enumeration
 ******************************************************************************
 */
typedef enum _USB_FeatureSelectorT
{
  UsbEpHaltSelector= 0, UsbDeviceRemoteWakeupSelector, UsbTestModeSelector
} USB_FeatureSelectorT;

/*!
 ******************************************************************************
 ** \brief USB Request type enumeration
 ******************************************************************************
 */
typedef enum _USB_CoreReqTypeT
{
  UsbCoreReqDevState = 0, UsbCoreReqDevSusState, UsbCoreReqConfiquration,
  UsbCoreReqInterface,  UsbCoreReqDevOwnAddress,
  UsbCoreReqWakeUpEnableStatus, UsbCoreReqSelfPoweredStatus,
  UsbCoreReqSpeed,
} USB_CoreReqTypeT;

/*!
 ******************************************************************************
 ** \brief USB user function response enumeration
 ******************************************************************************
 */
typedef enum _USB_UserFuncResponseT
{
  UsbUserError = 0, UsbUserSendPacket, UsbUserReceivePacket, UsbUserSendAckn,
  UsbUserStallCtrlEp,
} USB_UserFuncResponseT;

/*!
 ******************************************************************************
 ** \brief USB data state enumeration
 ******************************************************************************
 */
typedef enum _USB_DataStateT
{
  UsbHost2Device = 0, UsbDevice2Host
} USB_DataStateT;

/*! \brief USB Device status no suspend definition */
#define UsbDevStatusNoSuspend 0
/*! \brief USB Device status suspend definition */
#define UsbDevStatusSuspend   1

#pragma pack(1)
/*!
 ******************************************************************************
 ** \brief USB device state structure
 ******************************************************************************
 */
typedef struct _USB_DevStateT
{
  uint8_t DS          : 3;                       //!< USB Device state 
  uint8_t             : 4;                       //!< Reserve
  uint8_t Suspend     : 1;                       //!< Device Suspend flag 
} USB_DevStateT, * pUSB_DevStateT;

/*!
 ******************************************************************************
 ** \brief USB request type structure
 ******************************************************************************
 */
typedef union _USB_RequestTypeT
{
  uint8_t mRequestTypeData;                     //!< Request type byte
  struct
  {
    uint8_t Recipient : 5;                      //!< Recipent type
    uint8_t Type      : 2;                      //!< Request type
    uint8_t Dir       : 1;                      //!< Direction
  };
}USB_RequestTypeT, * pUSB_RequestTypeT;

/*!
 ******************************************************************************
 ** \brief Two bytes data structure
 ******************************************************************************
 */
typedef union _TwoBytes_t
{
  uint16_t Word;                               //!< Word 
  struct
  {
    uint8_t Lo;                                //!< Low byte 
    uint8_t Hi;                                //!< high byte 
  };
}TwoBytes_t, * pTwoBytes_t;

/*!
 ******************************************************************************
 ** \brief USB setup packet structure
 ******************************************************************************
 */
typedef union _UsbSetupPacketT
{
  uint8_t Data[8];                             //!< 8 bytes data 
  struct {
    USB_RequestTypeT  mRequestType;            //!< Request type 
    uint8_t             bRequest;              //!< Request command 
    TwoBytes_t        wValue;                  //!< two bytes value 
    TwoBytes_t        wIndex;                  //!< two bytes index 
    TwoBytes_t        wLength;                 //!< two bytes length 
  };
} USB_SetupPacketT, * pUSB_SetupPacketT;
#pragma pack()

/*!
 ******************************************************************************
 ** \brief USB device feature structure
 ******************************************************************************
 */
typedef union _USB_DefFeatureT
{
  uint8_t Data;                                //!< Device feature byte
  struct
  {
    uint8_t SelfPowered         :1;            //!< Self-powered flag
    uint8_t RemoteWakeupEnable  :1;            //!< Remote wakeup flag
    uint8_t                     :6;            //!< Reserved
  };
} USB_DefFeatureT, * pUSB_DefFeatureT;

/*!
 ******************************************************************************
 ** \brief USB device control structure
 ******************************************************************************
 */
typedef struct _UsbDevCtrl_t
{
  USB_DevStateT       State;                          //!< Device state
  uint32_t            Configuration;                  //!< Device configuration number
  pUSB_StandardConfigurationDescriptorT pCnfg;        //!< Device configuration structure
  uint32_t            Interfaces;                     //!< Device interfaces
  uint32_t            AlternateSetting[USB_MAX_INTERFACE]; //!< Device alternate setting
  uint32_t            DevAdd;                         //!< Device address
  USB_DefFeatureT   Feature;                          //!< Device feature
#if USB_HIGH_SPEED > 0
  uint32_t HighSpeed;                                 //!< Device high speed flag
#endif // USB_HIGH_SPEED > 0
} USB_DevCtrlT, * pUSB_DevCtrlT;

/*! \brief USB core error handler */
#define USB_CORE_ERROR_REQUEST() g_EpCnfg[CTRL_ENP_OUT].Status = STALLED

/*! \brief USB Setup Packet declare */
USB_CORE_EXTERN USB_SetupPacketT g_UsbEp0SetupPacket;

/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
void USB_CoreInit (void);
uint32_t USB_CoreReq (USB_CoreReqTypeT Type);
#if USB_REMOTE_WAKEUP != 0
void UsbWakeUp (void);
#endif // USB_REMOTE_WAKEUP != 0
void USB_SetDevState (USB_CoreDevStateT DevState);
#if USB_HIGH_SPEED > 0
void UsbDevHighSpeedCallback (uint8_t Connect);
#endif // USB_HIGH_SPEED > 0
void USB_DevConnectCallback (uint8_t Connect);
void USB_DevSuspendCallback (uint8_t Suspend);
void USB_DevResetCallback (void);
#if USB_HIGH_SPEED > 0
void USB_DevSpeedCallback(uint8_t HighSpeed);
#endif // USB_HIGH_SPEED > 0
uint32_t USB_CompareSize(uint32_t Size,uint32_t HostExpSize);
void USB_SetupHandler (void);
void USB_StatusHandler (USB_EpIndexT Ep);
#ifdef __cplusplus
}
#endif

#endif //__USB_T9_H
/*****************************************************************************/
/* END OF FILE */
