/*!
 ******************************************************************************
 **
 ** \file usb_desc_fm3.h
 **
 ** \brief USB device descriptor head file
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

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "includes.h"

#ifndef __USB_DESC_H
#define __USB_DESC_H

/*! \brief USB current configuration  */
#define USB_ConfigPower_mA(mA)   ((mA)/2)
/*! \brief Set USB Endpoint out  */
#define USB_EpOut(addr)          ((addr) | 0x00)
/*! \brief Set USB Endpoint in  */
#define USB_EpIn(addr)           ((addr) | 0x80)

/*! \brief USB descriptor length offset  */
#define UsbDescLengthOffset     0

/*!
 ******************************************************************************
 ** \brief USB Descriptor type enumeration
 ******************************************************************************
 */
typedef enum _USB_DescriptorTypeT
{
    UsbDescriptorDevice = 1, UsbDescriptorConfiguration, UsbDescriptorString,
    UsbDescriptorInterface, UsbDescriptorEp, UsbDescriptorDeviceQualifier,
    UsbDescriptorOtherSeedConfiguration, UsbDescriptorInterfacePower
} USB_DescriptorTypeT;

/*!
 ******************************************************************************
 ** \brief USB String index enumeration
 ******************************************************************************
 */
typedef enum _USB_StringsIndexT
{
    iLanguagesStr = 0, iManufacturerStr, iProductStr, iSerialNumberStr,
    iConfigurationStr, iInterface,
} USB_StringsIndexT;

/*!
 ******************************************************************************
 ** \brief USB Device Class enumeration
 ******************************************************************************
 */
typedef enum _USB_DeviceClasssesT
{
    UsbDeviceClassReserved = 0, UsbDeviceClassAudio,
    UsbDeviceClassCommunications,UsbDeviceClassHumanInterface,
    UsbDeviceClassMonitor,UsbDeviceClassPhysicalInterface,
    UsbDeviceClassPower,UsbDeviceClassPrinter,
    UsbDeviceClassStorage,UsbDeviceClassHub,
    UsbDeviceClassVendorSpecific = 0xFF
} USB_DeviceClasssesT;

/*!
 ******************************************************************************
 ** \brief Configuration characteristics structure
 ******************************************************************************
 */
typedef union _Cnfg_ArtrT
{
    uint8_t Data;
    struct
    {
        uint8_t Reserved      : 5;              //!< Reserved
        uint8_t RemoteWakeup  : 1;              //!< Remote wakeup
        uint8_t SelfPowered   : 1;              //!< Self powered flag
        uint8_t MustBeOne     : 1;              //!< Reserved
    };
} Cnfg_ArtrT, * pCnfg_ArtrT;

/*!
 ******************************************************************************
 ** \brief Configuration attribute enumeration
 ******************************************************************************
 */
typedef enum _USB_ConfigurationAttrT
{
    UsbRemoteWakeup = 0x20, UsbSelfPowered = 0x40,  UsbConfigurationCommmonAttr = 0x80
} USB_ConfigurationAttrT;

/*!
 ******************************************************************************
 ** \brief USB Endpoint transfer type enumeration
 ******************************************************************************
 */
typedef enum _USB_EpTransferTypeT
{
    UsbEpTransferControl = 0, UsbEpTransferIsochronous,
    UsbEpTransferBulk,UsbEpTransferInterrupt
}USB_EpTransferTypeT;

/*!
 ******************************************************************************
 ** \brief USB Endpoint synchronization type enumeration
 ******************************************************************************
 */
typedef enum _USB_EpSynchronizationTypeT
{
    UsbEpSynchNoSynchronization = 0, UsbEpSynchAsynchronous = 1<<2,
    UsbEpSynchAdaptive = 2<<2, UsbEpSynchSynchronous = 3<<2
}USB_EpSynchronizationTypeT;

/*!
 ******************************************************************************
 ** \brief USB Endpoint usage enumeration
 ******************************************************************************
 */
typedef enum _USB_EpUsageTypeT
{
    UsbEpUsageData = 0, UsbEpUsageFeedback = 1<<4,
    UsbEpUsageImplicit = 2<<4, UsbEpUsageReserved = 3<<4
}USB_EpUsageTypeT;

/*!
 ******************************************************************************
 ** \brief USB Endpoint attribute enumeration
 ******************************************************************************
 */
typedef union _Ep_AttrT
{
    uint8_t Data;
    struct
    {
        uint8_t TransferType        : 2;    //!< Transfer type
        uint8_t SynchronizationType : 2;    //!< Synchronization Type
        uint8_t UsageType           : 2;    //!< Usage Type
    };
} Ep_AttrT, * pEp_AttrT;

#pragma pack(1)
/*!
 ******************************************************************************
 ** \brief USB Standard Device Descriptor Structure
 ******************************************************************************
 */
typedef struct _USB_StandardDeviceDescriptorStrT
{
    uint8_t       bLength;                    //!< Descriptor length
    uint8_t       bDescriptorType;            //!< Descriptor type
    uint16_t      bcdUSB;                     //!< USB Specification Release Version
    uint8_t       bDeviceClass;               //!< Device class
    uint8_t       bDeviceSubClass;            //!< Device subclass
    uint8_t       bDeviceProtocol;            //!< Device protocol
    uint8_t       bMaxPacketSize0;            //!< Maximum packet size
    uint16_t      idVendor;                   //!< Vendor ID
    uint16_t      idProduct;                  //!< Product ID
    uint16_t      bcdDevice;                  //!< Device Release Number
    uint8_t       iManufacturer;              //!< Manufacturer String descriptor
    uint8_t       iProduct;                   //!< Product string descriptor
    uint8_t       iSerialNumber;              //!< Serial Number
    uint8_t       bNumConfigurations;         //!< Configuration number
} USB_StandardDeviceDescriptorStrT, * pUSB_StandardDeviceDescriptorStrT;

/*!
 ******************************************************************************
 ** \brief USB Stanard Configuration descriptor structure
 ******************************************************************************
 */
typedef struct _USB_StandardConfigurationDescriptorT
{
    uint8_t       bLength;                    //!< Descriptor length
    uint8_t       bDescriptorType;            //!< Descriptor type
    uint16_t      wTotalLength;               //!< Descriptor total length
    uint8_t       bNumInterfaces;             //!< Interfaces number
    uint8_t       bConfigurationValue;        //!< Value to use as an argument
    uint8_t       iConfiguration;             //!< Index of string descriptor
    Cnfg_ArtrT    bmAttributes;               //!< Configuration characteristics
    uint8_t       bMaxPower;                  //!< Maximum power consumption of USB device
} USB_StandardConfigurationDescriptorT, * pUSB_StandardConfigurationDescriptorT;

/*!
 ******************************************************************************
 ** \brief USB Standard interface descriptor structure
 ******************************************************************************
 */
typedef struct _USB_StandardInterfaceDescriptorT
{
    uint8_t       bLength;                    //!< Descriptor length
    uint8_t       bDescriptorType;            //!< Descriptor type
    uint8_t       bInterfaceNumber;           //!< Number of interface
    uint8_t       bAlternateSetting;          //!< Alternate setting number for the interface
    uint8_t       bNumEndpoints;              //!< Number of endpoints used by this interface
    uint8_t       bInterfaceClass;            //!< Class code
    uint8_t       bInterfaceSubClass;         //!< Subclass code
    uint8_t       bInterfaceProtocol;         //!< Protocol code
    uint8_t       iInterface;                 //!< Index of string descriptor
} USB_StandardInterfaceDescriptorT, * pUSB_StandardInterfaceDescriptorT;

/*!
 ******************************************************************************
 ** \brief USB Standard endpoint descriptor structure
 ******************************************************************************
 */
typedef struct _USB_StandardEpDescriptorT
{
    uint8_t       bLength;                    //!< Descriptor length
    uint8_t       bDescriptorType;            //!< Descriptor type
    uint8_t       bEndpointAddress;           //!< The address of the endpoint on the USB device
    Ep_AttrT      bmAttributes;               //!< Describes the endpoint's transfer mode
    uint16_t      wMaxPacketSize;             //!< Maximum packet size
    uint8_t       bInterval;                  //!< Interval for polling endpoint
} USB_StandardEpDescriptorT, * pUSB_StandardEpDescriptorT;

/*!
 ******************************************************************************
 ** \brief USB Standard device qualifier descriptor structure
 ******************************************************************************
 */
#if USB_HIGH_SPEED > 0
typedef struct _USB_StandardDeviceQualifierDescriptorT
{
    uint8_t       bLength;                    //!< Descriptor length
    uint8_t       bDescriptorType;            //!< Descriptor type
    uint16_t      bcdUSB;                     //!< USB Specification Release Version
    uint8_t       bDeviceClass;               //!< Device Class
    uint8_t       bDeviceSubClass;            //!< Device Subclass
    uint8_t       bDeviceProtocol;            //!< Device protocol
    uint8_t       bMaxPacketSize0;            //!< Maximum packet size
    uint8_t       bNumConfigurations;         //!< Configuration number
    uint8_t       bReserve;                   //!< Reserved
} UsbStandardDeviceQualifierDescriptorT, *pUsbStandardDeviceQualifierDescriptorT;
#endif // USB_HIGH_SPEED > 0
#pragma pack()

#endif //__USB_DESC_H
