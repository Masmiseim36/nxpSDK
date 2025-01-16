/**
 * Copyright 2018,2020,2022,2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "lwip/opt.h"

#if LWIP_IPV4 && LWIP_ARP
#include "netif/etharp.h"
#include "usb_ethernetif.h"

#if USE_RTOS && defined(SDK_OS_FREE_RTOS)
#include "FreeRTOS.h"
#include "event_groups.h"
#endif

#include "usb_host_config.h"

#if (!USB_HOST_CONFIG_KHCI) && (!USB_HOST_CONFIG_EHCI) && (!USB_HOST_CONFIG_OHCI) && (!USB_HOST_CONFIG_IP3516HS)
#error Please enable USB_HOST_CONFIG_KHCI, USB_HOST_CONFIG_EHCI, USB_HOST_CONFIG_OHCI, or USB_HOST_CONFIG_IP3516HS in file usb_host_config.
#endif

#include "usb_host.h"
#include "usb_host_cdc.h"
#include "usb_host_devices.h"
#include "usb_host_framework.h"

#if defined(USB_HOST_CONFIG_CDC_ECM) && USB_HOST_CONFIG_CDC_ECM
#include "usb_host_cdc_ecm.h"
#elif defined(USB_HOST_CONFIG_CDC_RNDIS) && USB_HOST_CONFIG_CDC_RNDIS
#include "usb_host_cdc_rndis.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void USB_HostClockInit(void);
extern void USB_HostIsrEnable(void);
extern void USB_HostTaskFn(void *param);

static void USB_HostApplicationInit(uint8_t controllerId, struct netif *netif);
static usb_status_t USB_HostEvent(usb_device_handle deviceHandle,
                                  usb_host_configuration_handle configurationHandle,
                                  uint32_t event_code);

#if LWIP_IPV4 && LWIP_IGMP
static void _multicastIp2MulticastMac(const ip4_addr_t *ip, uint8_t (*mac)[NETIF_MAX_HWADDR_LEN]);
static err_t _macCompare(const uint8_t (*mac1)[NETIF_MAX_HWADDR_LEN], const uint8_t (*mac2)[NETIF_MAX_HWADDR_LEN]);
#endif

#if defined(USB_HOST_CONFIG_CDC_ECM) && USB_HOST_CONFIG_CDC_ECM
static void USB_HostCdcEcmControlCallback(void *param, uint8_t *data, uint32_t dataLength, usb_status_t status);
static void USB_HostCdcEcmInterruptCallback(void *param, uint8_t *data, uint32_t dataLength, usb_status_t status);
static void USB_HostCdcEcmDataInCallback(void *param, uint8_t *data, uint32_t dataLength, usb_status_t status);
static void USB_HostCdcEcmDataOutCallback(void *param, uint8_t *data, uint32_t dataLength, usb_status_t status);
static void USB_HostCdcEcmControlIgmpFilterCallback(void *param,
                                                    uint8_t *data,
                                                    uint32_t dataLength,
                                                    usb_status_t status);
static int _char_atoi16(const char *ch);
static usb_status_t USB_HostCdcEcmUnicodeStrToNum(const uint16_t *strBuf, uint32_t strlength, uint8_t *const numBuf);
static void USB_HostCdcEcmUnicodeMacAddressStrToNum(const uint16_t *strBuf, uint8_t *const macBuf);
#elif defined(USB_HOST_CONFIG_CDC_RNDIS) && USB_HOST_CONFIG_CDC_RNDIS
static void USB_HostCdcRndisControlCallback(void *param, uint8_t *data, uint32_t dataLength, usb_status_t status);
static void USB_HostCdcRndisInterruptCallback(void *param, uint8_t *data, uint32_t dataLength, usb_status_t status);
static void USB_HostCdcRndisDataInCallback(void *param, uint8_t *data, uint32_t dataLength, usb_status_t status);
static void USB_HostCdcRndisDataOutCallback(void *param, uint8_t *data, uint32_t dataLength, usb_status_t status);
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/
usb_host_handle g_HostHandle = {0};

#if defined(USB_HOST_CONFIG_CDC_ECM) && USB_HOST_CONFIG_CDC_ECM
USB_HostCdcEcmInstance_t g_HostCdcEcmInstance                                                     = {0};
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) uint8_t g_OutPutBuffer[CDC_ECM_DATA_BUFFER_LEN]   = {0};
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) uint8_t g_InPutBuffer[CDC_ECM_DATA_BUFFER_LEN]    = {0};
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) uint8_t g_NotifyBuffer[CDC_ECM_NOTIFY_BUFFER_LEN] = {0};
uint8_t g_requestParamBuffer[CDC_ECM_REQUEST_BUFFER_LEN]                                          = {0};
USB_HostCdcEcmRequestParam_t *g_requestParam =
    (USB_HostCdcEcmRequestParam_t *)(&g_HostCdcEcmInstance.requestParamBuffer);
static struct pbuf *s_pbufReceived = NULL;
#elif defined(USB_HOST_CONFIG_CDC_RNDIS) && USB_HOST_CONFIG_CDC_RNDIS
usb_host_rndis_instance_struct_t g_RndisInstance = {0};
/* each g_RndisInstance should have its own's buffer */
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) uint8_t g_SendMessage[RNDIS_CONTROL_MESSAGE];
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) uint8_t g_GetMessage[RNDIS_CONTROL_MESSAGE];
/* used for send/recv data from/to device rndis, the message length should be the sum of max frame packet size and usb
 * rndis header */
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) uint8_t g_OutPutBuffer[RNDIS_DATA_MESSAGE];
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) uint8_t g_InPutBuffer[RNDIS_DATA_MESSAGE];
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
#if defined(USB_HOST_CONFIG_CDC_ECM) && USB_HOST_CONFIG_CDC_ECM
static void USB_HostCdcEcmControlCallback(void *param, uint8_t *data, uint32_t dataLength, usb_status_t status)
{
    USB_HostCdcEcmInstance_t *ecmInstance = (USB_HostCdcEcmInstance_t *)param;

    if (status != kStatus_USB_Success)
    {
        usb_echo("Contorl transfer error (%d).\r\n", status);
        return;
    }

    switch (ecmInstance->runPrevState)
    {
        case USB_HostCdcEcmRunSetControlInterface:
            ecmInstance->runCurState  = USB_HostCdcEcmRunSetDataInterface;
            ecmInstance->runWaitState = USB_HostCdcEcmRunIdle;
            break;

        case USB_HostCdcEcmRunSetDataInterface:
            ecmInstance->runCurState  = USB_HostCdcEcmRunPariseFunctionalDescriptor;
            ecmInstance->runWaitState = (USB_HostCdcEcmRunState_t)(0);
            break;

        case USB_HostCdcEcmRunPariseFunctionalDescriptor:
            if (ecmInstance->runWaitState == 0)
            {
                ecmInstance->runCurState  = USB_HostCdcEcmRunPariseFunctionalDescriptor;
                ecmInstance->runWaitState = (USB_HostCdcEcmRunState_t)(1);
            }
            else if (ecmInstance->runWaitState == 1)
            {
                ecmInstance->runCurState  = USB_HostCdcEcmRunPariseFunctionalDescriptor;
                ecmInstance->runWaitState = (USB_HostCdcEcmRunState_t)(2);
            }
            break;

        case USB_HostCdcEcmRunSetupTraffic:
            ecmInstance->runCurState  = USB_HostCdcEcmRunTransfer;
            ecmInstance->runWaitState = USB_HostCdcEcmRunIdle;
            ecmInstance->netifUsbStateEvent |= CDC_ECM_STATE_XFER_INTERRUPT;
            break;

        default:
            usb_echo("Contorl transfer callback state error (%d).\r\n", ecmInstance->runPrevState);
            break;
    }

    return;
}

static void USB_HostCdcEcmDataInCallback(void *param, uint8_t *data, uint32_t dataLength, usb_status_t status)
{
    USB_HostCdcEcmInstance_t *ecmInstance = (USB_HostCdcEcmInstance_t *)param;

    if (status != kStatus_USB_Success)
    {
        if (status == kStatus_USB_TransferCancel)
        {
            usb_echo("CDC-ECM bulk in transfer cancelled.\r\n");
        }
        else
        {
            ecmInstance->netifUsbStateEvent |= CDC_ECM_STATE_XFER_DATA_IN;
            usb_echo("CDC-ECM bulk in transfer error.\r\n");
        }
        return;
    }

    if (dataLength > 0 && NULL != data)
    {
        if (!s_pbufReceived)
        {
            s_pbufReceived = pbuf_alloc(PBUF_RAW, dataLength, PBUF_POOL);
            if (s_pbufReceived)
            {
                s_pbufReceived->tot_len = dataLength;
                s_pbufReceived->len     = dataLength;

                /* in special case, when polling out packet, in packet maybe finihsed, the in packet will be not
                 * be handled */
                memcpy(s_pbufReceived->payload, data, dataLength);
                ecmInstance->netifUsbStateEvent |= CDC_ECM_STATE_LINK_RX;
            }
        }
        else
        {
            ecmInstance->netifUsbStateEvent |= CDC_ECM_STATE_LINK_RX;
        }
    }

    ecmInstance->netifUsbStateEvent |= CDC_ECM_STATE_XFER_DATA_IN;

    return;
}

static void USB_HostCdcEcmDataOutCallback(void *param, uint8_t *data, uint32_t dataLength, usb_status_t status)
{
    USB_HostCdcEcmInstance_t *ecmInstance = (USB_HostCdcEcmInstance_t *)param;

    if (status != kStatus_USB_Success)
    {
        if (status == kStatus_USB_TransferCancel)
        {
            usb_echo("CDC-ECM bulk out transfer cancelled.\r\n");
        }
        else
        {
            ecmInstance->netifUsbDataOutEvent |= CDC_ECM_STATE_XFER_DATA_OUT;
            usb_echo("CDC-ECM bulk out transfer error.\r\n");
        }
        return;
    }

    if (ecmInstance->dataState == USB_HostCdcEcmDataXfering)
    {
        ecmInstance->dataState = USB_HostCdcEcmDataXferred;
    }
    else
    {
        usb_echo("CDC-ECM data out callback state error.\r\n");
    }

    ecmInstance->netifUsbDataOutEvent |= CDC_ECM_STATE_XFER_DATA_OUT;

    return;
}

static void USB_HostCdcEcmInterruptCallback(void *param, uint8_t *data, uint32_t dataLength, usb_status_t status)
{
    USB_HostCdcEcmInstance_t *ecmInstance = (USB_HostCdcEcmInstance_t *)param;
    struct netif *netif                   = (struct netif *)ecmInstance->netif;

    if (status != kStatus_USB_Success)
    {
        ecmInstance->netifUsbStateEvent |= CDC_ECM_STATE_XFER_INTERRUPT;
        usb_echo("CDC-ECM interrupt in transfer error.\r\n");
        return;
    }

    uint8_t notify_bNotificationCode = *(data + 1);
    uint16_t notify_wValue           = *(uint16_t *)(data + 2);
    uint8_t *notify_Data             = data + 8;
    switch (notify_bNotificationCode)
    {
        case USB_HOST_CDC_NOTIFICATION_NETWORK_CONNECTION:
            if (notify_wValue != ecmInstance->deviceNetworkConnection)
            {
                ecmInstance->deviceNetworkConnection = (uint8_t)(notify_wValue);
                usb_echo("Device network connection is %s.\r\n",
                         ecmInstance->deviceNetworkConnection ? "connected" : "disconnect");
            }
            break;

        case USB_HOST_CDC_NOTIFICATION_CONNECTION_SPEED_CHANGE:
            if (*(uint32_t *)(notify_Data) != ecmInstance->deviceNetworkDownLinkSpeed ||
                *(uint32_t *)(notify_Data + 4) != ecmInstance->deviceNetworkUpLinkSpeed)
            {
                ecmInstance->deviceNetworkDownLinkSpeed = *(uint32_t *)(notify_Data);
                ecmInstance->deviceNetworkUpLinkSpeed   = *(uint32_t *)(notify_Data + 4);
                usb_echo("Network speed have been changed to DL %d bps / UL %d bps.\r\n",
                         ecmInstance->deviceNetworkDownLinkSpeed, ecmInstance->deviceNetworkUpLinkSpeed);
            }
            if (!netif_is_link_up(netif) && ecmInstance->deviceNetworkConnection &&
                ecmInstance->deviceNetworkDownLinkSpeed && ecmInstance->deviceNetworkUpLinkSpeed)
            {
                ecmInstance->netifUsbStateEvent |= CDC_ECM_STATE_LINK_UP;
            }
            else if (netif_is_link_up(netif) &&
                     (!ecmInstance->deviceNetworkConnection || !ecmInstance->deviceNetworkDownLinkSpeed ||
                      !ecmInstance->deviceNetworkUpLinkSpeed))
            {
                ecmInstance->netifUsbStateEvent |= CDC_ECM_STATE_LINK_DOWN;
            }
            break;

        default:
            usb_echo("Interrupt transfer callback state error (%d).\r\n", notify_bNotificationCode);
            break;
    }

    ecmInstance->netifUsbStateEvent |= CDC_ECM_STATE_XFER_INTERRUPT;

    return;
}

static void USB_HostCdcEcmControlIgmpFilterCallback(void *param,
                                                    uint8_t *data,
                                                    uint32_t dataLength,
                                                    usb_status_t status)
{
    USB_HostCdcEcmInstance_t *ecmInstance = (USB_HostCdcEcmInstance_t *)param;

    if (status != kStatus_USB_Success)
    {
        usb_echo("IGMP Filter Contorl transfer error (%d).\r\n", status);
        return;
    }

    ecmInstance->netifUsbIgmpFilterEvent |= CDC_ECM_STATE_LINK_SET_MCFILTER;

    return;
}

void USB_HostCdcEcmTask(void *param, uint32_t *task_event)
{
    USB_HostCdcEcmInstance_t *ecmInstance = (USB_HostCdcEcmInstance_t *)param;
    struct netif *netif                   = ecmInstance->netif;
    static usb_host_cdc_ethernet_networking_desc_struct_t *ethernetNetworkingDesc;
    static uint8_t macStringDescBuffer[CDC_ECM_STRING_MAC_BUFFER_LEN];

    switch (ecmInstance->devWaitState)
    {
        case USB_DeviceStateIdle:
            break;

        case USB_DeviceStateAttached:
            if (USB_HostCdcInit(ecmInstance->deviceHandle, &ecmInstance->classHandle) != kStatus_USB_Success)
            {
                ecmInstance->runCurState = USB_HostCdcEcmRunIdle;
                usb_echo("USB host init failed.\r\n");
            }
            else
            {
                ecmInstance->devCurState = ecmInstance->devWaitState;
                ecmInstance->runCurState = USB_HostCdcEcmRunSetControlInterface;
                usb_echo("USB CDC-ECM device is attached.\r\n");
            }
            ecmInstance->devWaitState = USB_DeviceStateIdle;
            break;

        case USB_DeviceStateDetached:
            if (netif_is_up(netif))
            {
                netif_set_down(netif);
                netif_set_addr(netif, IP4_ADDR_ANY, IP4_ADDR_ANY, IP4_ADDR_ANY);
                netif_set_up(netif);
            }
            ecmInstance->netifUsbStateEvent |= CDC_ECM_STATE_LINK_DOWN;
            USB_HostCdcDeinit(ecmInstance->deviceHandle, ecmInstance->classHandle);
            ecmInstance->deviceHandle               = NULL;
            ecmInstance->classHandle                = NULL;
            ecmInstance->controlInterfaceHandle     = NULL;
            ecmInstance->dataInterfaceHandle        = NULL;
            ecmInstance->devCurState                = ecmInstance->devWaitState;
            ecmInstance->devWaitState               = USB_DeviceStateIdle;
            ecmInstance->runPrevState               = USB_HostCdcEcmRunIdle;
            ecmInstance->runCurState                = USB_HostCdcEcmRunIdle;
            ecmInstance->runWaitState               = USB_HostCdcEcmRunIdle;
            ecmInstance->dataState                  = USB_HostCdcEcmDataIdle;
            ecmInstance->deviceNetworkConnection    = 0;
            ecmInstance->deviceNetworkDownLinkSpeed = 0;
            ecmInstance->deviceNetworkUpLinkSpeed   = 0;
            usb_echo("USB CDC-ECM device detached.\r\n");
            break;
    }

    switch (ecmInstance->runCurState)
    {
        case USB_HostCdcEcmRunIdle:
        default:
            break;

        case USB_HostCdcEcmRunSetControlInterface:
            ecmInstance->runPrevState = ecmInstance->runCurState;
            ecmInstance->runCurState  = USB_HostCdcEcmRunIdle;
            if (USB_HostCdcSetControlInterface(ecmInstance->classHandle, ecmInstance->controlInterfaceHandle, 0,
                                               USB_HostCdcEcmControlCallback, ecmInstance) != kStatus_USB_Success)
            {
                usb_echo("Set control interface error.\r\n");
            }
            break;

        case USB_HostCdcEcmRunSetDataInterface:
            ecmInstance->runPrevState = ecmInstance->runCurState;
            ecmInstance->runCurState  = USB_HostCdcEcmRunIdle;
            if (USB_HostCdcSetDataInterface(ecmInstance->classHandle, ecmInstance->dataInterfaceHandle, 1,
                                            USB_HostCdcEcmControlCallback, ecmInstance) != kStatus_USB_Success)
            {
                usb_echo("Set data interface error.\r\n");
            }
            break;

        case USB_HostCdcEcmRunPariseFunctionalDescriptor:
            if (ecmInstance->runWaitState == (USB_HostCdcEcmRunState_t)(0))
            {
                ecmInstance->runPrevState = ecmInstance->runCurState;
                ecmInstance->runCurState  = USB_HostCdcEcmRunIdle;
                USB_HostCdcGetEcmDescriptor(ecmInstance->classHandle, NULL, NULL, &ethernetNetworkingDesc);
                ecmInstance->deviceEthernetStatisticsBitmap =
                    *((uint32_t *)ethernetNetworkingDesc->bmEthernetStatistics);
                ecmInstance->deviceMaxSegmentSize = *((uint16_t *)ethernetNetworkingDesc->wMaxSegmentSize);
                ecmInstance->deviceMCFilters      = *((uint16_t *)ethernetNetworkingDesc->wNumberMCFilters) & 0x7FFFU;
                ecmInstance->devicePerfectMCFiltersSupport =
                    (*((uint16_t *)ethernetNetworkingDesc->wNumberMCFilters) & 0x8000U) >> 15;
                ecmInstance->devicePowerFilters = ethernetNetworkingDesc->bNumberPowerFilters;
                if (USB_HostCdcEcmGetMacStringDescriptor(
                        ecmInstance->classHandle, ethernetNetworkingDesc->iMACAddress, 0x0409U, &macStringDescBuffer[0],
                        2, USB_HostCdcEcmControlCallback, ecmInstance) != kStatus_USB_Success)
                {
                    usb_echo("Get string (index %d) descriptor transfer error.\r\n",
                             ethernetNetworkingDesc->iMACAddress);
                }
            }
            else if (ecmInstance->runWaitState == (USB_HostCdcEcmRunState_t)(1))
            {
                ecmInstance->runPrevState = ecmInstance->runCurState;
                ecmInstance->runCurState  = USB_HostCdcEcmRunIdle;
                if (USB_HostCdcEcmGetMacStringDescriptor(
                        ecmInstance->classHandle, ethernetNetworkingDesc->iMACAddress, 0x0409U, &macStringDescBuffer[0],
                        (uint16_t)(((usb_descriptor_common_t *)(&macStringDescBuffer[0]))->bLength),
                        USB_HostCdcEcmControlCallback, ecmInstance) != kStatus_USB_Success)
                {
                    usb_echo("Get string (index %d) descriptor transfer error.\r\n",
                             ethernetNetworkingDesc->iMACAddress);
                }
            }
            else if (ecmInstance->runWaitState == (USB_HostCdcEcmRunState_t)(2))
            {
                ecmInstance->runPrevState               = ecmInstance->runCurState;
                ecmInstance->runCurState                = USB_HostCdcEcmRunSetupTraffic;
                g_requestParam->SetEthernetPacketFilter = ecmInstance->devicePktFilerBitmap;
                USB_HostCdcEcmUnicodeMacAddressStrToNum((uint16_t *)(&macStringDescBuffer[2]),
                                                        ecmInstance->deviceMacAddress);
                memcpy(((struct netif *)(ecmInstance->netif))->hwaddr, ecmInstance->deviceMacAddress,
                       NETIF_MAX_HWADDR_LEN);
                if (ecmInstance->deviceMaxSegmentSize > CDC_ECM_FRAME_MAX_FRAMELEN)
                {
                    ((struct netif *)(ecmInstance->netif))->mtu = CDC_ECM_FRAME_MAX_FRAMELEN - 14;
                }
                else
                {
                    ((struct netif *)(ecmInstance->netif))->mtu = ecmInstance->deviceMaxSegmentSize - 14;
                }
            }
            break;

        case USB_HostCdcEcmRunSetupTraffic:
            ecmInstance->runPrevState = ecmInstance->runCurState;
            ecmInstance->runCurState  = USB_HostCdcEcmRunIdle;
            if (USB_HostCdcEcmSetEthernetPacketFilter(ecmInstance->classHandle, g_requestParam->SetEthernetPacketFilter,
                                                      USB_HostCdcEcmControlCallback,
                                                      ecmInstance) != kStatus_USB_Success)
            {
                usb_echo("Send SetEthernetPacketFilter request error.\r\n");
            }
            break;

        case USB_HostCdcEcmRunTransfer:
            ecmInstance->runPrevState = ecmInstance->runCurState;
            if (ecmInstance->deviceHandle)
            {
                if (*task_event & CDC_ECM_STATE_XFER_DATA_IN && netif_is_link_up(netif))
                {
                    if (USB_HostCdcEcmDataRecv(ecmInstance->classHandle, ecmInstance->dataRecvBuffer,
                                               CDC_ECM_DATA_BUFFER_LEN, USB_HostCdcEcmDataInCallback, ecmInstance))
                    {
                        usb_echo("Receive data error.\r\n");
                    }
                    *task_event &= ~CDC_ECM_STATE_XFER_DATA_IN;
                }

                if (*task_event & CDC_ECM_STATE_XFER_INTERRUPT)
                {
                    if (USB_HostCdcInterruptRecv(ecmInstance->classHandle, ecmInstance->notifyBuffer,
                                                 CDC_ECM_NOTIFY_BUFFER_LEN, USB_HostCdcEcmInterruptCallback,
                                                 ecmInstance) != kStatus_USB_Success)
                    {
                        usb_echo("Receive notification error.\r\n");
                    }
                    *task_event &= ~CDC_ECM_STATE_XFER_INTERRUPT;
                }
            }
            break;
    }
}

usb_status_t USB_HostCdcEcmEvent(usb_device_handle deviceHandle,
                                 usb_host_configuration_handle configurationHandle,
                                 usb_host_event_t event_code)
{
    usb_status_t status                     = kStatus_USB_Success;
    usb_host_configuration_t *configuration = (usb_host_configuration_t *)configurationHandle;
    usb_host_interface_t *hostInterface;
    uint8_t interface_index;
    uint32_t info_value[3] = {0};

    switch (event_code)
    {
        case kUSB_HostEventAttach:
            for (interface_index = 0U; interface_index < configuration->interfaceCount; ++interface_index)
            {
                hostInterface = &configuration->interfaceList[interface_index];
                if (hostInterface->interfaceDesc->bInterfaceClass == USB_HOST_CDC_ECM_CLASS_CODE &&
                    hostInterface->interfaceDesc->bInterfaceSubClass == USB_HOST_CDC_ECM_SUBCLASS_CODE &&
                    hostInterface->interfaceDesc->bInterfaceProtocol == USB_HOST_CDC_ECM_PROTOCOL_CODE)
                {
                    g_HostCdcEcmInstance.controlInterfaceHandle = hostInterface;
                }
                else if (hostInterface->interfaceDesc->bInterfaceClass == USB_HOST_CDC_DATA_CLASS_CODE &&
                         hostInterface->interfaceDesc->bInterfaceSubClass == USB_HOST_CDC_DATA_SUBCLASS_CODE &&
                         hostInterface->interfaceDesc->bInterfaceProtocol == USB_HOST_CDC_DATA_PROTOCOL_CODE)
                {
                    g_HostCdcEcmInstance.dataInterfaceHandle = hostInterface;
                }
                else
                {
                    continue;
                }
            }
            g_HostCdcEcmInstance.deviceHandle = deviceHandle;
            if (NULL != g_HostCdcEcmInstance.controlInterfaceHandle && NULL != g_HostCdcEcmInstance.dataInterfaceHandle)
            {
                status = kStatus_USB_Success;
            }
            else
            {
                status = kStatus_USB_NotSupported;
            }
            break;

        case kUSB_HostEventNotSupported:
            g_HostCdcEcmInstance.devWaitState = USB_DeviceStateIdle;
            usb_echo("Cannot support the USB device, please check if the device support USB CDC-ECM class.\r\n");
            break;

        case kUSB_HostEventEnumerationDone:
            g_HostCdcEcmInstance.devWaitState = USB_DeviceStateIdle;
            if (g_HostCdcEcmInstance.devCurState != USB_DeviceStateAttached)
            {
                if (g_HostCdcEcmInstance.deviceHandle != NULL && g_HostCdcEcmInstance.dataInterfaceHandle != NULL &&
                    g_HostCdcEcmInstance.controlInterfaceHandle != NULL)
                {
                    g_HostCdcEcmInstance.devWaitState = USB_DeviceStateAttached;
                    USB_HostHelperGetPeripheralInformation(deviceHandle, kUSB_HostGetDevicePID, &info_value[0]);
                    USB_HostHelperGetPeripheralInformation(deviceHandle, kUSB_HostGetDeviceVID, &info_value[1]);
                    USB_HostHelperGetPeripheralInformation(deviceHandle, kUSB_HostGetDeviceAddress, &info_value[2]);
                    usb_echo("Device CDC-ECM attached: PID=0x%X, VID=0x%X, Address=%d.\r\n", info_value[0],
                             info_value[1], info_value[2]);
                }
            }
            else
            {
                usb_echo("No idle CDC-ECM instance.\r\n");
            }
            break;

        case kUSB_HostEventDetach:
            if (g_HostCdcEcmInstance.devCurState == USB_DeviceStateAttached)
            {
                g_HostCdcEcmInstance.devWaitState = USB_DeviceStateDetached;
            }
            break;

        default:
            break;
    }

    return status;
}

static int _char_atoi16(const char *ch)
{
    char a[3] = {0};
    int i     = 0;
    if (ch[0] >= '0' && ch[0] <= '9')
    {
        a[0] = ch[0];
        i    = atoi(a);
    }
    else if (ch[0] >= 'A' && ch[0] <= 'F')
    {
        a[0] = '1';
        a[1] = '0' + ch[0] - 'A';
        i    = atoi(a);
    }
    else if (ch[0] >= 'a' && ch[0] <= 'f')
    {
        a[0] = '1';
        a[1] = '0' + ch[0] - 'a';
        i    = atoi(a);
    }
    else
    {
        i = -1;
    }
    return i;
}

static usb_status_t USB_HostCdcEcmUnicodeStrToNum(const uint16_t *strBuf, uint32_t strlength, uint8_t *const numBuf)
{
    usb_status_t status = kStatus_USB_Success;
    uint32_t count      = 0U;
    if (strBuf)
    {
        uint8_t str[3] = "";
        int8_t num;
        for (uint32_t index = 0; index < strlength; index++)
        {
            str[0] = *(const uint8_t *)(&strBuf[index]);
            str[1] = *((const uint8_t *)(&strBuf[index]) + 1);
            num    = _char_atoi16((char *)str);
            if (num != -1 && num >= 0)
            {
                numBuf[index] = (uint8_t)(num);
                count++;
            }
        }
    }
    if (count != strlength)
    {
        status = kStatus_USB_Error;
    }

    return status;
}

static void USB_HostCdcEcmUnicodeMacAddressStrToNum(const uint16_t *strBuf, uint8_t *const macBuf)
{
    uint32_t maclength  = 6U;
    uint8_t macByte[12] = {0};
    USB_HostCdcEcmUnicodeStrToNum(strBuf, maclength * 2, macByte);
    for (uint32_t index = 0U; index < maclength; index++)
    {
        macBuf[index] = (macByte[index * 2] << 4) | (macByte[index * 2 + 1]);
    }

    return;
}
#elif defined(USB_HOST_CONFIG_CDC_RNDIS) && USB_HOST_CONFIG_CDC_RNDIS
/*!
 * @brief host cdc data transfer callback.
 *
 * This function is used as callback function for bulk in transfer .
 *
 * @param param    the host cdc instance pointer.
 * @param data     data buffer pointer.
 * @param dataLength data length.
 * @status         transfer result status.
 */
static void USB_HostCdcRndisDataInCallback(void *param, uint8_t *data, uint32_t dataLength, usb_status_t status)
{
    usb_host_rndis_instance_struct_t *rndisInstance = (usb_host_rndis_instance_struct_t *)param;
    struct netif *netif                             = (struct netif *)rndisInstance->netif;
    if (status != kStatus_USB_Success)
    {
        if (status == kStatus_USB_TransferCancel)
        {
            /*usb_echo("cdc transfer cancel\r\n");*/
        }
        else
        {
            /**usb_echo("cdc in transfer retry\r\n");*/
        }
        rndisInstance->runState = kUSB_HostCdcRndisRunDataReceive;
    }
    else
    {
        struct pbuf *pbuf;
        if ((dataLength > 0) && (NULL != data))
        {
            rndis_packet_msg_struct_t *temp = (rndis_packet_msg_struct_t *)data;
            pbuf                            = pbuf_alloc(PBUF_RAW, temp->dataLength, PBUF_POOL);
            if (pbuf)
            {
                temp->dataBuffer[temp->dataLength] = 0;
                pbuf->tot_len                      = temp->dataLength;
                pbuf->len                          = temp->dataLength;

                uint8_t *p = (uint8_t *)(&temp->dataOffset);
                memcpy(pbuf->payload, (p + temp->dataOffset), temp->dataLength);
                /*in special case, when polling out packet, in packet maybe
                 * finihsed, the in packet will be not be handled*/
                if (!rndisInstance->pollingInSending)
                {
                    netif->input(pbuf, netif);
                }
            }
        }
        rndisInstance->runState = kUSB_HostCdcRndisRunDataReceive;
    }
    return;
}

/*!
 * @brief host cdc data transfer callback.
 *
 * This function is used as callback function for bulk out transfer .
 *
 * @param param    the host cdc instance pointer.
 * @param data     data buffer pointer.
 * @param dataLength data length.
 * @status         transfer result status.
 */
static void USB_HostCdcRndisDataOutCallback(void *param, uint8_t *data, uint32_t dataLength, usb_status_t status)
{
    usb_host_rndis_instance_struct_t *rndisInstance = (usb_host_rndis_instance_struct_t *)param;
    if (status != kStatus_USB_Success)
    {
        if (status == kStatus_USB_TransferCancel)
        {
            usb_echo("cdc transfer cancel\r\n");
        }
        else
        {
            /*usb_echo("cdc out transfer error\r\n");*/
        }
    }
    rndisInstance->dataSend = 0U;
    return;
}

/*!
 * @brief host cdc interrupt transfer callback.
 *
 * This function is used as callback function for interrupt transfer . Interrupt
 * transfer is used to implement asynchronous notification of UART status as
 * pstn sepc. This callback suppose the device will return SerialState
 * notification. If there is need to suppose other notification ,please refer
 * pstn spec 6.5 and cdc spec6.3.
 * @param param    the host cdc instance pointer.
 * @param data     data buffer pointer.
 * @param dataLength data length.
 * @status         transfer result status.
 */
static void USB_HostCdcRndisInterruptCallback(void *param, uint8_t *data, uint32_t dataLength, usb_status_t status)
{
    /* usb_host_cdc_acm_state_struct_t *state = (usb_host_cdc_acm_state_struct_t *)data; */
    usb_host_rndis_instance_struct_t *rndisInstance = (usb_host_rndis_instance_struct_t *)param;

    if (status != kStatus_USB_Success)
    {
        if (status == kStatus_USB_TransferCancel)
        {
            usb_echo("cdc transfer cancel\r\n");
        }
        else
        {
            usb_echo("cdc control transfer error\r\n");
        }
    }
    else
    {
        /*more information about SerialState ,please pstn spec 6.5.4 */
        /* usb_echo("get serial state value = %d\r\n", state->bmstate);*/
        rndisInstance->responseAvailable = 1;
        rndisInstance->interruptRunState = kUSB_HostCdcRndisRunInterruptRecvDone;
    }
}

/*!
 * @brief host cdc rndis control transfer callback.
 *
 * This function is used as callback function for control transfer .
 *
 * @param param    the host cdc rndis instance pointer.
 * @param data     data buffer pointer.
 * @param dataLength data length.
 * @status         transfer result status.
 */
static void USB_HostCdcRndisControlCallback(void *param, uint8_t *data, uint32_t dataLength, usb_status_t status)
{
    usb_host_rndis_instance_struct_t *rndisInstance = (usb_host_rndis_instance_struct_t *)param;

    struct netif *netif = (struct netif *)rndisInstance->netif;
    if (status != kStatus_USB_Success)
    {
        usb_echo("data transfer error = %d\r\n", status);
        return;
    }

    if (rndisInstance->runWaitState == kUSB_HostCdcRndisRunWaitSetControlInterface)
    {
        rndisInstance->runState = kUSB_HostCdcRndisRunSetControlInterfaceDone;
    }
    else if (rndisInstance->runWaitState == kUSB_HostCdcRndisRunWaitSetDataInterface)
    {
        rndisInstance->runState = kUSB_HostCdcRndisRunSetDataInterfaceDone;
    }
    else if (rndisInstance->runWaitState == kUSB_HostCdcRndisRunWaitInitMsg)
    {
        rndisInstance->runState = kUSB_HostCdcRndisRunWaitInitMsgDone;
    }
    else if (rndisInstance->runWaitState == kUSB_HostCdcRndisRunWaitMaxmumFrame)
    {
        rndisInstance->runState = kUSB_HostCdcRndisRunWaitMaxmumFrameDone;
    }
    else if (rndisInstance->runWaitState == kUSB_HostCdcRndisRunWaitGetMACAddress)
    {
        rndisInstance->runState = kUSB_HostCdcRndisRunWaitGetMACAddressDone;
    }
    else if (rndisInstance->runWaitState == kUSB_HostCdcRndisRunWaitSetMsg)
    {
        rndisInstance->runState = kUSB_HostCdcRndisRunWaitSetMsgDone;
    }
    else if (rndisInstance->runWaitState == kUSB_HostCdcRndisRunWaitGetEncapsulatedCommand)
    {
        rndisInstance->runState = kUSB_HostCdcRndisRunGetEncapsulatedResponseDone;
        if (rndisInstance->previousRunState == kUSB_HostCdcRndisRunWaitInitMsgDone)
        {
            rndisInstance->runState = kUSB_HostCdcRndisRunWaitMaxmumFrame;
        }
        else if (rndisInstance->previousRunState == kUSB_HostCdcRndisRunWaitMaxmumFrameDone)
        {
            rndisInstance->runState = kUSB_HostCdcRndisRunWaitGetMACAddress;

            rndis_query_cmplt_struct_t *msg = (rndis_query_cmplt_struct_t *)data;
            if (REMOTE_NDIS_QUERY_CMPLT == msg->messageType)
            {
                netif->mtu =
                    USB_SHORT_FROM_LITTLE_ENDIAN_ADDRESS(((uint8_t *)&msg->requestID + msg->informationBufferOffset));
            }
            else
            {
                /*set default value if device doesn't send query complete
                 * message*/
                netif->mtu = 1500U;
            }
        }
        else if (rndisInstance->previousRunState == kUSB_HostCdcRndisRunWaitGetMACAddressDone)
        {
            rndisInstance->runState         = kUSB_HostCdcRndisRunWaitSetMsg;
            rndis_query_cmplt_struct_t *msg = (rndis_query_cmplt_struct_t *)data;

            netif->hwaddr_len = NETIF_MAX_HWADDR_LEN;

            memcpy(netif->hwaddr, (((uint8_t *)&msg->requestID + msg->informationBufferOffset)), NETIF_MAX_HWADDR_LEN);
        }
        else if (rndisInstance->previousRunState == kUSB_HostCdcRndisRunWaitSetMsgDone)
        {
            rndisInstance->runState = kUSB_HostCdcRndisRunGetState;
        }
        rndisInstance->previousRunState = kUSB_HostCdcRndisRunIdle;
    }
    else
    {
    }
}

/*!
 * @brief host rndis task function.
 *
 * This function implements the host cdc action, it is used to create task.
 *
 * @param param the host rndis instance pointer.
 */
void USB_HostCdcRndisTask(void *param)
{
    usb_status_t status                             = kStatus_USB_Success;
    usb_host_rndis_instance_struct_t *rndisInstance = (usb_host_rndis_instance_struct_t *)param;
    uint32_t filtertype;
    struct netif *netif;
    /* device state changes */
    if (rndisInstance->deviceState != rndisInstance->previousState)
    {
        rndisInstance->previousState = rndisInstance->deviceState;
        switch (rndisInstance->deviceState)
        {
            case USB_DeviceStateIdle:
                break;
            case USB_DeviceStateAttached:
                rndisInstance->runState = kUSB_HostCdcRndisRunSetControlInterface;
                status                  = USB_HostCdcInit(rndisInstance->deviceHandle, &rndisInstance->classHandle);
                usb_echo("rndis device attached\r\n");
                break;
            case USB_DeviceStateDetached:
                rndisInstance->deviceState = USB_DeviceStateIdle;
                rndisInstance->runState    = kUSB_HostCdcRndisRunIdle;
                USB_HostCdcDeinit(rndisInstance->deviceHandle, rndisInstance->classHandle);
                rndisInstance->dataInterfaceHandle    = NULL;
                rndisInstance->classHandle            = NULL;
                rndisInstance->controlInterfaceHandle = NULL;
                rndisInstance->deviceHandle           = NULL;
                rndisInstance->interruptRunState      = kUSB_HostCdcRndisRunIdle;
                usb_echo("rndis device detached\r\n");
                break;
            default:
                break;
        }
    }

    /* run state */
    switch (rndisInstance->runState)
    {
        case kUSB_HostCdcRndisRunIdle:
            break;
        case kUSB_HostCdcRndisRunSetControlInterface:
            rndisInstance->runWaitState = kUSB_HostCdcRndisRunWaitSetControlInterface;
            rndisInstance->runState     = kUSB_HostCdcRndisRunIdle;
            if (USB_HostCdcSetControlInterface(rndisInstance->classHandle, rndisInstance->controlInterfaceHandle, 0,
                                               USB_HostCdcRndisControlCallback, rndisInstance) != kStatus_USB_Success)
            {
                usb_echo("set control interface error\r\n");
            }
            break;
        case kUSB_HostCdcRndisRunSetControlInterfaceDone:
            rndisInstance->runWaitState = kUSB_HostCdcRndisRunWaitSetDataInterface;
            rndisInstance->runState     = kUSB_HostCdcRndisRunIdle;
            if (USB_HostCdcSetDataInterface(rndisInstance->classHandle, rndisInstance->dataInterfaceHandle, 0,
                                            USB_HostCdcRndisControlCallback, rndisInstance) != kStatus_USB_Success)
            {
                usb_echo("set data interface error\r\n");
            }
            break;
        case kUSB_HostCdcRndisRunSetDataInterfaceDone:
            rndisInstance->runWaitState = kUSB_HostCdcRndisRunWaitInitMsg;
            rndisInstance->runState     = kUSB_HostCdcRndisRunIdle;
            if (USB_HostRndisInitMsg(rndisInstance->classHandle, rndisInstance->sendMessage, RNDIS_CONTROL_MESSAGE,
                                     USB_HostCdcRndisControlCallback, rndisInstance))
            {
                usb_echo("Error in Init message\r\n");
            }
            break;
        case kUSB_HostCdcRndisRunWaitInitMsgDone:

            if (rndisInstance->interruptRunState == kUSB_HostCdcRndisRunIdle)
            {
                rndisInstance->interruptRunState = kUSB_HostCdcRndisRunInterruptRecvPrime;
                if (USB_HostCdcInterruptRecv(rndisInstance->classHandle, (uint8_t *)&rndisInstance->state,
                                             RNDIS_RESPONSE_AVAILABLE, USB_HostCdcRndisInterruptCallback,
                                             rndisInstance) != kStatus_USB_Success)
                {
                    usb_echo("Error in USB_HostCdcInterruptRecv: %x\r\n", status);
                }
            }
            else if (rndisInstance->interruptRunState == kUSB_HostCdcRndisRunInterruptRecvDone)
            {
                rndisInstance->interruptRunState = kUSB_HostCdcRndisRunIdle;
                /*remember the previous set encapsulated command*/
                rndisInstance->previousRunState = kUSB_HostCdcRndisRunWaitInitMsgDone;
                rndisInstance->runWaitState     = kUSB_HostCdcRndisRunWaitGetEncapsulatedCommand;
                rndisInstance->runState         = kUSB_HostCdcRndisRunIdle;

                if (USB_HostCdcGetEncapsulatedResponse(rndisInstance->classHandle, rndisInstance->getMessage,
                                                       RNDIS_CONTROL_MESSAGE, USB_HostCdcRndisControlCallback,
                                                       rndisInstance))
                {
                    usb_echo("Error in Init message\r\n");
                }
            }
            break;
        case kUSB_HostCdcRndisRunWaitMaxmumFrame:
            rndisInstance->previousRunState = kUSB_HostCdcRndisRunWaitMaxmumFrame;
            rndisInstance->runWaitState     = kUSB_HostCdcRndisRunWaitMaxmumFrame;
            rndisInstance->runState         = kUSB_HostCdcRndisRunIdle;

            if (USB_HostRndisQueryMsg(rndisInstance->classHandle, OID_GEN_MAXIMUM_FRAME_SIZE,
                                      rndisInstance->sendMessage, RNDIS_CONTROL_MESSAGE, 0, 0, NULL,
                                      USB_HostCdcRndisControlCallback, rndisInstance))
            {
                usb_echo("Error in Init message\r\n");
            }
            break;

        case kUSB_HostCdcRndisRunWaitMaxmumFrameDone:

            if (rndisInstance->interruptRunState == kUSB_HostCdcRndisRunIdle)
            {
                rndisInstance->interruptRunState = kUSB_HostCdcRndisRunInterruptRecvPrime;
                if (USB_HostCdcInterruptRecv(rndisInstance->classHandle, (uint8_t *)&rndisInstance->state,
                                             RNDIS_RESPONSE_AVAILABLE, USB_HostCdcRndisInterruptCallback,
                                             rndisInstance) != kStatus_USB_Success)
                {
                    usb_echo("Error in USB_HostCdcInterruptRecv: %x\r\n", status);
                }
            }
            else if (rndisInstance->interruptRunState == kUSB_HostCdcRndisRunInterruptRecvDone)
            {
                rndisInstance->interruptRunState = kUSB_HostCdcRndisRunIdle;
                rndisInstance->previousRunState  = kUSB_HostCdcRndisRunWaitMaxmumFrameDone;
                rndisInstance->runWaitState      = kUSB_HostCdcRndisRunWaitGetEncapsulatedCommand;
                rndisInstance->runState          = kUSB_HostCdcRndisRunIdle;

                if (USB_HostCdcGetEncapsulatedResponse(rndisInstance->classHandle, rndisInstance->getMessage,
                                                       RNDIS_CONTROL_MESSAGE, USB_HostCdcRndisControlCallback,
                                                       rndisInstance))
                {
                    usb_echo("Error in Init message\r\n");
                }
            }
            break;
        case kUSB_HostCdcRndisRunWaitGetMACAddress:
            rndisInstance->previousRunState = kUSB_HostCdcRndisRunWaitGetMACAddress;
            rndisInstance->runWaitState     = kUSB_HostCdcRndisRunWaitGetMACAddress;
            rndisInstance->runState         = kUSB_HostCdcRndisRunIdle;

            if (USB_HostRndisQueryMsg(rndisInstance->classHandle, OID_802_3_CURRENT_ADDRESS, rndisInstance->sendMessage,
                                      RNDIS_CONTROL_MESSAGE, 0, 0, NULL, USB_HostCdcRndisControlCallback,
                                      rndisInstance))
            {
                usb_echo("Error in Init message\r\n");
            }
            break;

        case kUSB_HostCdcRndisRunWaitGetMACAddressDone:
            if (rndisInstance->interruptRunState == kUSB_HostCdcRndisRunIdle)
            {
                rndisInstance->interruptRunState = kUSB_HostCdcRndisRunInterruptRecvPrime;
                rndisInstance->interruptRunState = kUSB_HostCdcRndisRunInterruptRecvPrime;
                if (USB_HostCdcInterruptRecv(rndisInstance->classHandle, (uint8_t *)&rndisInstance->state,
                                             RNDIS_RESPONSE_AVAILABLE, USB_HostCdcRndisInterruptCallback,
                                             rndisInstance) != kStatus_USB_Success)
                {
                    usb_echo("Error in USB_HostCdcInterruptRecv: %x\r\n", status);
                }
            }
            else if (rndisInstance->interruptRunState == kUSB_HostCdcRndisRunInterruptRecvDone)
            {
                rndisInstance->interruptRunState = kUSB_HostCdcRndisRunIdle;
                rndisInstance->responseAvailable = 0;
                rndisInstance->previousRunState  = kUSB_HostCdcRndisRunWaitGetMACAddressDone;
                rndisInstance->runWaitState      = kUSB_HostCdcRndisRunWaitGetEncapsulatedCommand;
                rndisInstance->runState          = kUSB_HostCdcRndisRunIdle;

                if (USB_HostCdcGetEncapsulatedResponse(rndisInstance->classHandle, rndisInstance->getMessage,
                                                       RNDIS_CONTROL_MESSAGE, USB_HostCdcRndisControlCallback,
                                                       rndisInstance))
                {
                    usb_echo("Error in Init message\r\n");
                }
            }
            break;
        case kUSB_HostCdcRndisRunWaitSetMsg:
            rndisInstance->previousRunState = kUSB_HostCdcRndisRunWaitSetMsg;
            rndisInstance->runWaitState     = kUSB_HostCdcRndisRunWaitSetMsg;
            rndisInstance->runState         = kUSB_HostCdcRndisRunIdle;
            filtertype                      = NDIS_PACKET_TYPE_DIRECTED;
            if (USB_HostRndisSetMsg(rndisInstance->classHandle, OID_GEN_CURRENT_PACKET_FILTER,
                                    rndisInstance->sendMessage, RNDIS_CONTROL_MESSAGE, 20U, (sizeof(filtertype)),
                                    &filtertype, USB_HostCdcRndisControlCallback, rndisInstance))
            {
                usb_echo("Error in set message\r\n");
            }
            break;
        case kUSB_HostCdcRndisRunWaitSetMsgDone:
            if (rndisInstance->interruptRunState == kUSB_HostCdcRndisRunIdle)
            {
                rndisInstance->interruptRunState = kUSB_HostCdcRndisRunInterruptRecvPrime;
                rndisInstance->interruptRunState = kUSB_HostCdcRndisRunInterruptRecvPrime;
                if (USB_HostCdcInterruptRecv(rndisInstance->classHandle, (uint8_t *)&rndisInstance->state,
                                             RNDIS_RESPONSE_AVAILABLE, USB_HostCdcRndisInterruptCallback,
                                             rndisInstance) != kStatus_USB_Success)
                {
                    usb_echo("Error in USB_HostCdcInterruptRecv: %x\r\n", status);
                }
            }
            else if (rndisInstance->interruptRunState == kUSB_HostCdcRndisRunInterruptRecvDone)
            {
                rndisInstance->responseAvailable = 0;
                rndisInstance->previousRunState  = kUSB_HostCdcRndisRunWaitSetMsgDone;
                rndisInstance->runWaitState      = kUSB_HostCdcRndisRunWaitGetEncapsulatedCommand;
                rndisInstance->runState          = kUSB_HostCdcRndisRunIdle;

                if (USB_HostCdcGetEncapsulatedResponse(rndisInstance->classHandle, rndisInstance->getMessage,
                                                       RNDIS_CONTROL_MESSAGE, USB_HostCdcRndisControlCallback,
                                                       rndisInstance))
                {
                    usb_echo("Error in Init message\r\n");
                }
            }
            break;
        case kUSB_HostCdcRndisRunGetState:
            rndisInstance->attach   = 1;
            rndisInstance->dataSend = 0;
            netif                   = (struct netif *)rndisInstance->netif;
            netif_set_link_up(netif);
        case kUSB_HostCdcRndisRunDataReceive:
            rndisInstance->runState = kUSB_HostCdcRndisRunIdle;
            USB_HostRndisRecvDataMsg(rndisInstance->classHandle, rndisInstance->inPutBuffer, RNDIS_DATA_MESSAGE,
                                     USB_HostCdcRndisDataInCallback, rndisInstance);
            break;
        default:
            break;
    }
}

usb_status_t USB_HostCdcRndisEvent(usb_device_handle deviceHandle,
                                   usb_host_configuration_handle configurationHandle,
                                   usb_host_event_t event_code)
{
    usb_status_t status;
    uint8_t classId;
    uint8_t subclassId;
    uint8_t protocolId;
    usb_host_configuration_t *configuration;
    uint8_t interface_index;
    usb_host_interface_t *hostInterface;
    uint32_t info_value = 0U;
    struct netif *netif;
    status = kStatus_USB_Success;

    switch (event_code)
    {
        case kUSB_HostEventAttach:
            /* judge whether is configurationHandle supported */
            configuration = (usb_host_configuration_t *)configurationHandle;

            for (interface_index = 0; interface_index < configuration->interfaceCount; ++interface_index)
            {
                hostInterface = &configuration->interfaceList[interface_index];
                classId       = hostInterface->interfaceDesc->bInterfaceClass;
                subclassId    = hostInterface->interfaceDesc->bInterfaceSubClass;
                protocolId    = hostInterface->interfaceDesc->bInterfaceProtocol;

                if ((classId == USB_HOST_CDC_RNDIS_CLASS_CODE) && (subclassId == USB_HOST_CDC_RNDIS_SUBCLASS_CODE) &&
                    (protocolId == USB_HOST_CDC_RNDIS_PROTOCOL_CODE))
                {
                    g_RndisInstance.controlInterfaceHandle = hostInterface;
                }
                else if ((classId == USB_HOST_WC_RNDIS_CLASS_CODE) && (subclassId == USB_HOST_WC_RNDIS_SUBCLASS_CODE) &&
                         (protocolId == USB_HOST_WC_RNDIS_PROTOCOL_CODE))
                {
                    g_RndisInstance.controlInterfaceHandle = hostInterface;
                }
                else if ((classId == USB_HOST_MISC_RNDIS_CLASS_CODE) &&
                         (subclassId == USB_HOST_MISC_RNDIS_SUBCLASS_CODE) &&
                         (protocolId == USB_HOST_MISC_RNDIS_PROTOCOL_CODE))
                {
                    g_RndisInstance.controlInterfaceHandle = hostInterface;
                }
                else if ((classId == USB_HOST_CDC_DATA_CLASS_CODE) && (subclassId == USB_HOST_CDC_DATA_SUBCLASS_CODE) &&
                         (protocolId == USB_HOST_CDC_DATA_PROTOCOL_CODE))
                {
                    g_RndisInstance.dataInterfaceHandle = hostInterface;
                }
                else
                {
                    /* no action */
                }
            }

            if ((NULL != g_RndisInstance.dataInterfaceHandle) && (NULL != g_RndisInstance.controlInterfaceHandle))
            {
                /* The interface is supported by the application */
                g_RndisInstance.deviceHandle = deviceHandle;
                status                       = kStatus_USB_Success;
            }
            else
            {
                status = kStatus_USB_NotSupported;
            }
            break;

        case kUSB_HostEventNotSupported:
            usb_echo(
                "  the usb tethering featue is not enabled, please turn on usb "
                "tethering in mobile phone\r\n ");
            break;

        case kUSB_HostEventEnumerationDone:
            if (g_RndisInstance.deviceState == USB_DeviceStateIdle)
            {
                if ((g_RndisInstance.deviceHandle != NULL) && (g_RndisInstance.dataInterfaceHandle != NULL) &&
                    (g_RndisInstance.controlInterfaceHandle != NULL))
                {
                    g_RndisInstance.deviceState = USB_DeviceStateAttached;

                    USB_HostHelperGetPeripheralInformation(deviceHandle, kUSB_HostGetDevicePID, &info_value);
                    usb_echo("device cdc attached:\r\npid=0x%x", info_value);
                    USB_HostHelperGetPeripheralInformation(deviceHandle, kUSB_HostGetDeviceVID, &info_value);
                    usb_echo("vid=0x%x ", info_value);
                    USB_HostHelperGetPeripheralInformation(deviceHandle, kUSB_HostGetDeviceAddress, &info_value);
                    usb_echo("address=%d\r\n", info_value);
                }
            }
            else
            {
                usb_echo("not idle rndis instance\r\n");
            }
            break;

        case kUSB_HostEventDetach:
            if (g_RndisInstance.deviceState != USB_DeviceStateIdle)
            {
                g_RndisInstance.attach   = 0;
                g_RndisInstance.dataSend = 0;
                netif                    = (struct netif *)g_RndisInstance.netif;
                netif_set_link_down(netif);
                g_RndisInstance.deviceState = USB_DeviceStateDetached;
            }
            break;

        default:
            break;
    }
    return status;
}
#endif

/**
 * @brief host callback function for device attach/detach
 * @param deviceHandle device handle.
 * @param configurationHandle attached device's configuration descriptor information.
 * @param event_code callback event code
 * @return USB status code
 */
static usb_status_t USB_HostEvent(usb_device_handle deviceHandle,
                                  usb_host_configuration_handle configurationHandle,
                                  uint32_t event_code)
{
    usb_status_t status = kStatus_USB_Success;

#if defined(USB_HOST_CONFIG_CDC_ECM) && USB_HOST_CONFIG_CDC_ECM
    USB_HostEventFcn_t USB_HostClassEvent = USB_HostCdcEcmEvent;
#elif defined(USB_HOST_CONFIG_CDC_RNDIS) && USB_HOST_CONFIG_CDC_RNDIS
    USB_HostEventFcn_t USB_HostClassEvent = USB_HostCdcRndisEvent;
#endif

    switch (event_code)
    {
        case kUSB_HostEventAttach:
            status = USB_HostClassEvent(deviceHandle, configurationHandle, (usb_host_event_t)event_code);
            break;

        case kUSB_HostEventNotSupported:
            status = USB_HostClassEvent(deviceHandle, configurationHandle, (usb_host_event_t)event_code);
            usb_echo("USB device is not supported.\r\n");
            break;

        case kUSB_HostEventEnumerationDone:
            status = USB_HostClassEvent(deviceHandle, configurationHandle, (usb_host_event_t)event_code);
            break;

        case kUSB_HostEventDetach:
            status = USB_HostClassEvent(deviceHandle, configurationHandle, (usb_host_event_t)event_code);
            break;

        default:
            break;
    }
    return status;
}

/**
 * @brief USB host application initialization function
 * @param controllerId The controller ID of the USB IP
 * @param netif the LwIP network interface structure for this ethernetif
 */
static void USB_HostApplicationInit(uint8_t controllerId, struct netif *netif)
{
    USB_HostClockInit();
    if (USB_HostInit(controllerId, &g_HostHandle, USB_HostEvent) != kStatus_USB_Success)
    {
        usb_echo("USB host init error.\r\n");
        return;
    }
    USB_HostIsrEnable();
    usb_echo("USB host init success.\r\n");

#if defined(USB_HOST_CONFIG_CDC_ECM) && USB_HOST_CONFIG_CDC_ECM
    netif->state                                    = (void *)&g_HostCdcEcmInstance;
    g_HostCdcEcmInstance.netif                      = (void *)netif;
    g_HostCdcEcmInstance.hostHandle                 = g_HostHandle;
    g_HostCdcEcmInstance.dataRecvBuffer             = &g_InPutBuffer[0];
    g_HostCdcEcmInstance.dataSendBuffer             = &g_OutPutBuffer[0];
    g_HostCdcEcmInstance.notifyBuffer               = &g_NotifyBuffer[0];
    g_HostCdcEcmInstance.requestParamBuffer         = &g_requestParamBuffer[0];
    g_HostCdcEcmInstance.deviceNetworkConnection    = 0;
    g_HostCdcEcmInstance.deviceNetworkDownLinkSpeed = 0;
    g_HostCdcEcmInstance.deviceNetworkUpLinkSpeed   = 0;
    g_HostCdcEcmInstance.netifUsbStateEvent         = 0;
    g_HostCdcEcmInstance.netifUsbDataOutEvent       = 0;
    g_HostCdcEcmInstance.netifUsbIgmpFilterEvent    = 0;
#elif defined(USB_HOST_CONFIG_CDC_RNDIS) && USB_HOST_CONFIG_CDC_RNDIS
    netif->state                          = (void *)&g_RndisInstance;
    g_RndisInstance.netif                 = (void *)netif;
    g_RndisInstance.hostHandle            = g_HostHandle;
    g_RndisInstance.sendMessage           = &g_SendMessage[0];
    g_RndisInstance.getMessage            = &g_GetMessage[0];
    g_RndisInstance.outPutBuffer          = &g_OutPutBuffer[0];
    g_RndisInstance.inPutBuffer           = &g_InPutBuffer[0];

    while (!g_RndisInstance.attach)
    {
        USB_HostTaskFn(g_RndisInstance.hostHandle);
        USB_HostCdcRndisTask(&g_RndisInstance);
    }
#endif
}

err_t USB_EthernetIfInIt(struct netif *netif)
{
    err_t status = ERR_IF;

    netif->name[0] = IFNAME0;
    netif->name[1] = IFNAME1;
#if LWIP_IPV4
    netif->output = etharp_output;
#endif
    netif->linkoutput = USB_EthernetIfOutPut;

    /* set MAC hardware address length */
    netif->hwaddr_len = ETH_HWADDR_LEN;

    ethernetifConfig_t *config;
    if (netif->state)
    {
        config = (ethernetifConfig_t *)netif->state;
        USB_HostApplicationInit(config->controllerId, netif);

        /* USB ENET card is ready */
        netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;

#if defined(USB_HOST_CONFIG_CDC_RNDIS) && USB_HOST_CONFIG_CDC_RNDIS
        netif->flags |= NETIF_FLAG_LINK_UP;
#endif

#if defined(USB_HOST_CONFIG_CDC_ECM) && USB_HOST_CONFIG_CDC_ECM
        g_HostCdcEcmInstance.devicePktFilerBitmap =
            CDC_ECM_ETH_PACKET_FILTER_DIRECTED | CDC_ECM_ETH_PACKET_FILTER_BROADCAST;
#endif

#if LWIP_IPV4 && LWIP_IGMP
        netif_set_igmp_mac_filter(netif, USB_EthernetIfIgmpMacFilter);
        netif->flags |= NETIF_FLAG_IGMP;
#endif

        status = ERR_OK;
    }

    return status;
}

#if LWIP_IPV4 && LWIP_IGMP
static void _multicastIp2MulticastMac(const ip4_addr_t *ip, uint8_t (*mac)[NETIF_MAX_HWADDR_LEN])
{
    uint8_t *p = (uint8_t *)mac;
    *p++       = LL_IP4_MULTICAST_ADDR_0;
    *p++       = LL_IP4_MULTICAST_ADDR_1;
    *p++       = LL_IP4_MULTICAST_ADDR_2;
    *p++       = ip4_addr2_16(ip) & 0x7FU;
    *p++       = ip4_addr3_16(ip);
    *p++       = ip4_addr4_16(ip);
}

static err_t _macCompare(const uint8_t (*mac1)[NETIF_MAX_HWADDR_LEN], const uint8_t (*mac2)[NETIF_MAX_HWADDR_LEN])
{
    if (memcmp(mac1, mac2, NETIF_MAX_HWADDR_LEN))
    {
        return ERR_MEM;
    }
    else
    {
        return ERR_OK;
    }
}

err_t USB_EthernetIfIgmpMacFilter(struct netif *netif, const ip4_addr_t *group, enum netif_mac_filter_action action)
{
#if defined(USB_HOST_CONFIG_CDC_ECM) && USB_HOST_CONFIG_CDC_ECM
    USB_HostCdcEcmInstance_t *ecmInstance = (USB_HostCdcEcmInstance_t *)netif->state;
    static uint32_t usedFilters           = 0;
    static uint8_t multicastFilters[CDC_ECM_MAX_SUPPORT_MULTICAST_FILTERS][NETIF_MAX_HWADDR_LEN];
    uint8_t filter[CDC_ECM_MAX_SUPPORT_MULTICAST_FILTERS][NETIF_MAX_HWADDR_LEN];
    int filterLen  = 0;
    int filterFind = 0;
    uint8_t mac[NETIF_MAX_HWADDR_LEN];
    _multicastIp2MulticastMac(group, &mac);
#endif

    switch (action)
    {
        case NETIF_DEL_MAC_FILTER:
#if defined(USB_HOST_CONFIG_CDC_ECM) && USB_HOST_CONFIG_CDC_ECM
            if (usedFilters-- == 0)
            {
                usb_echo("MAC filter is none and cannot delete entry.\r\n");
                return ERR_IF;
            }

            for (int i = 0; i < usedFilters + 1; i++)
            {
                if (_macCompare((uint8_t(*)[NETIF_MAX_HWADDR_LEN])multicastFilters[i], &mac))
                {
                    memcpy(filter[filterLen++], multicastFilters[i], NETIF_MAX_HWADDR_LEN);
                    if (filterFind && ecmInstance->deviceMCFilters >= usedFilters)
                    {
                        memcpy(multicastFilters[i - 1], multicastFilters[i], NETIF_MAX_HWADDR_LEN);
                    }
                }
                else
                {
                    filterFind = 1;
                }
            }
#endif

            break;

        case NETIF_ADD_MAC_FILTER:
#if defined(USB_HOST_CONFIG_CDC_ECM) && USB_HOST_CONFIG_CDC_ECM
            if (usedFilters++ == CDC_ECM_MAX_SUPPORT_MULTICAST_FILTERS)
            {
                usb_echo("MAC filter is full and cannot add entry.\r\n");
                return ERR_IF;
            }

            for (int i = 0; i < usedFilters; i++)
            {
                if (i == usedFilters - 1)
                {
                    memcpy(multicastFilters[i], mac, NETIF_MAX_HWADDR_LEN);
                    memcpy(filter[filterLen++], multicastFilters[i], NETIF_MAX_HWADDR_LEN);
                }
                else
                {
                    if (_macCompare((uint8_t(*)[NETIF_MAX_HWADDR_LEN])multicastFilters[i], &mac))
                    {
                        memcpy(filter[filterLen++], multicastFilters[i], NETIF_MAX_HWADDR_LEN);
                    }
                }
            }
#endif

            break;

        default:
            break;
    }

#if defined(USB_HOST_CONFIG_CDC_ECM) && USB_HOST_CONFIG_CDC_ECM
    if (netif_is_link_up((struct netif *)ecmInstance->netif))
    {
        if (ecmInstance->deviceMCFilters >= usedFilters && ecmInstance->deviceMCFilters)
        {
            if (!(ecmInstance->devicePktFilerBitmap & CDC_ECM_ETH_PACKET_FILTER_MULTICAST))
            {
                ecmInstance->devicePktFilerBitmap &= ~CDC_ECM_ETH_PACKET_FILTER_ALL_MULTICAST;
                ecmInstance->devicePktFilerBitmap |= CDC_ECM_ETH_PACKET_FILTER_MULTICAST;
                if (USB_HostCdcEcmSetEthernetPacketFilter(ecmInstance->classHandle, ecmInstance->devicePktFilerBitmap,
                                                          USB_HostCdcEcmControlIgmpFilterCallback,
                                                          ecmInstance) != kStatus_USB_Success)
                {
                    usb_echo("Send SetEthernetPacketFilter request error.\r\n");
                }
                while (!(ecmInstance->netifUsbIgmpFilterEvent & CDC_ECM_STATE_LINK_SET_MCFILTER))
                {
                    USB_HostTaskFn(ecmInstance->hostHandle);
                }
                ecmInstance->netifUsbIgmpFilterEvent &= ~CDC_ECM_STATE_LINK_SET_MCFILTER;
            }

            if (USB_HostCdcEcmSetEthernetMulticastFilters(ecmInstance->classHandle, filterLen, (uint8_t(*)[6])filter,
                                                          USB_HostCdcEcmControlIgmpFilterCallback,
                                                          ecmInstance) != kStatus_USB_Success)
            {
                usb_echo("Send SetEthernetMulticastFilters request error.\r\n");
            }
            while (!(ecmInstance->netifUsbIgmpFilterEvent & CDC_ECM_STATE_LINK_SET_MCFILTER))
            {
                USB_HostTaskFn(ecmInstance->hostHandle);
            }
            ecmInstance->netifUsbIgmpFilterEvent &= ~CDC_ECM_STATE_LINK_SET_MCFILTER;
        }
        else if (usedFilters)
        {
            ecmInstance->devicePktFilerBitmap |= CDC_ECM_ETH_PACKET_FILTER_ALL_MULTICAST;
            if (USB_HostCdcEcmSetEthernetPacketFilter(ecmInstance->classHandle, ecmInstance->devicePktFilerBitmap,
                                                      USB_HostCdcEcmControlIgmpFilterCallback,
                                                      ecmInstance) != kStatus_USB_Success)
            {
                usb_echo("Send SetEthernetPacketFilter request error.\r\n");
            }
            while (!(ecmInstance->netifUsbIgmpFilterEvent & CDC_ECM_STATE_LINK_SET_MCFILTER))
            {
                USB_HostTaskFn(ecmInstance->hostHandle);
            }
            ecmInstance->netifUsbIgmpFilterEvent &= ~CDC_ECM_STATE_LINK_SET_MCFILTER;
        }
    }
#endif

    return ERR_OK;
}
#endif

void USB_EthernetIfInput(struct netif *netif)
{
#if defined(USB_HOST_CONFIG_CDC_ECM) && USB_HOST_CONFIG_CDC_ECM
    USB_HostCdcEcmInstance_t *ecmInstance = (USB_HostCdcEcmInstance_t *)netif->state;

    USB_HostTaskFn(ecmInstance->hostHandle);
    uint32_t *task_event = &ecmInstance->netifUsbStateEvent;

    if (netif_is_up(netif))
    {
        if ((*task_event & CDC_ECM_STATE_LINK_RX) && s_pbufReceived)
        {
            netif->input(s_pbufReceived, netif);
            s_pbufReceived = NULL;
            *task_event &= ~CDC_ECM_STATE_LINK_RX;
        }

        if (*task_event & CDC_ECM_STATE_LINK_UP)
        {
            netif_set_link_up(netif);
            *task_event |= (CDC_ECM_STATE_XFER_DATA_IN | CDC_ECM_STATE_LINK_RX);
            *task_event &= ~CDC_ECM_STATE_LINK_UP;
        }

        if (*task_event & CDC_ECM_STATE_LINK_DOWN)
        {
            netif_set_link_down(netif);
            *task_event &= ~(CDC_ECM_STATE_XFER_DATA_IN | CDC_ECM_STATE_LINK_RX);
            *task_event &= ~CDC_ECM_STATE_LINK_DOWN;
        }
    }

    USB_HostCdcEcmTask(ecmInstance, task_event);
#elif defined(USB_HOST_CONFIG_CDC_RNDIS) && USB_HOST_CONFIG_CDC_RNDIS
    usb_host_rndis_instance_struct_t *rndisInstance = (usb_host_rndis_instance_struct_t *)netif->state;

    USB_HostTaskFn(rndisInstance->hostHandle);
    USB_HostCdcRndisTask(rndisInstance);
#endif
}

err_t USB_EthernetIfOutPut(struct netif *netif, struct pbuf *p)
{
    err_t status = ERR_OK;

#if defined(USB_HOST_CONFIG_CDC_ECM) && USB_HOST_CONFIG_CDC_ECM
    USB_HostCdcEcmInstance_t *ecmInstance = (USB_HostCdcEcmInstance_t *)netif->state;

    if (!ecmInstance->deviceHandle)
    {
        status = ERR_CONN;
    }
    else
    {
        if (!netif_is_link_up(netif))
        {
            return ERR_CONN;
        }

        if (p->tot_len >= p->len)
        {
            uint32_t total        = p->tot_len;
            uint32_t transferDone = 0U;
            uint32_t buflen       = 0U;

            if (ecmInstance->dataState == USB_HostCdcEcmDataXfering)
            {
                /*discard current data if send flag is not cleared*/
                return ERR_BUF;
            }

            while (total)
            {
                ecmInstance->dataState = USB_HostCdcEcmDataXfering;
                buflen                 = total;
                if (total > ecmInstance->deviceMaxSegmentSize)
                {
                    buflen = ecmInstance->deviceMaxSegmentSize;
                }
                USB_HostCdcEcmDataSend(ecmInstance->classHandle, ((uint8_t *)p->payload + transferDone), buflen,
                                       ecmInstance->deviceMaxSegmentSize, USB_HostCdcEcmDataOutCallback, ecmInstance);
                transferDone += buflen;
                total -= buflen;

                while (!(ecmInstance->netifUsbDataOutEvent & CDC_ECM_STATE_XFER_DATA_OUT))
                {
                    USB_HostTaskFn(ecmInstance->hostHandle);
                }
                ecmInstance->netifUsbDataOutEvent &= ~CDC_ECM_STATE_XFER_DATA_OUT;
            }
        }
    }
#elif defined(USB_HOST_CONFIG_CDC_RNDIS) && USB_HOST_CONFIG_CDC_RNDIS
    usb_host_rndis_instance_struct_t *rndisInstance = (usb_host_rndis_instance_struct_t *)netif->state;
    if (rndisInstance->attach)
    {
        if (p->tot_len == p->len)
        {
            uint32_t total;
            total                 = p->tot_len;
            uint32_t transferDone = 0;

            while (total)
            {
                rndisInstance->pollingInSending = 1;
                if (rndisInstance->dataSend)
                {
                    /*discard current data if send flag is not cleared*/
                    return ERR_BUF;
                }
                rndisInstance->pollingInSending = 0;

                rndisInstance->dataSend = 1;
                if (total <= RNDIS_FRAME_MAX_FRAMELEN)
                {
                    USB_HostRndisSendDataMsg(rndisInstance->classHandle, rndisInstance->outPutBuffer,
                                             RNDIS_FRAME_MAX_FRAMELEN, 0, 0, 0, 0, 0,
                                             ((uint8_t *)p->payload + transferDone), total,
                                             USB_HostCdcRndisDataOutCallback, rndisInstance);
                    transferDone += total;
                    total = 0U;
                }
                else
                {
                    USB_HostRndisSendDataMsg(rndisInstance->classHandle, rndisInstance->outPutBuffer,
                                             RNDIS_FRAME_MAX_FRAMELEN, 0, 0, 0, 0, 0,
                                             ((uint8_t *)p->payload + transferDone), RNDIS_FRAME_MAX_FRAMELEN,
                                             USB_HostCdcRndisDataOutCallback, rndisInstance);
                    transferDone += RNDIS_FRAME_MAX_FRAMELEN;
                    total -= RNDIS_FRAME_MAX_FRAMELEN;
                }
            }
        }
        else
        {
            if (p->tot_len < RNDIS_FRAME_MAX_FRAMELEN)
            {
                uint8_t *buf = &((rndis_packet_msg_struct_t *)rndisInstance->outPutBuffer)->dataBuffer[0];
                u16_t uCopied = pbuf_copy_partial(p, buf, p->tot_len, 0);
                LWIP_ASSERT("uCopied != p->tot_len", uCopied == p->tot_len);

                rndisInstance->pollingInSending = 1;
                while (rndisInstance->dataSend)
                {
                    USB_HostTaskFn(g_HostHandle);
                }
                rndisInstance->pollingInSending = 0;
                USB_HostRndisSendDataMsg(rndisInstance->classHandle, rndisInstance->outPutBuffer,
                                         RNDIS_FRAME_MAX_FRAMELEN, 0, 0, 0, 0, 0, buf,
                                         p->tot_len, USB_HostCdcRndisDataOutCallback, rndisInstance);
            }
            else
            {
                /*TO DO*/
                return ERR_BUF;
            }
        }
    }
    else
    {
        status = ERR_CONN;
        usb_echo("USB Rndis device is not attached\r\n");
    }
#endif

    return status;
}
#endif
