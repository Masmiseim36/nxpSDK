/**
 * Copyright 2024 - 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "usb_device_config.h"
#include "usb_device.h"
#include "usb_device_ch9.h"
#include "usb_device_cdc_ecm.h"
#include "usb_device_descriptor.h"
#include "usb_eth_adapter.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void BOARD_InitHardware(void);
extern void USB_DeviceClockInit(void);
extern void USB_DeviceIsrEnable(void);

#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle);
#endif

usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param);

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t BOARD_SystickCount = 0;

usb_eth_nic_t ethNicHandle;

USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t notify_network_connection_req[sizeof(usb_setup_struct_t)];

USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t notify_connection_speed_change_req[sizeof(usb_setup_struct_t) + 8];

USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t dataOutBuffer[APP_ETH_FRAME_MAX_LENGTH];

USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t zlpBuffer;

volatile uint32_t appEvent;

/*******************************************************************************
 * Code
 ******************************************************************************/
void APP_TransferUSB2Ethernet_USBRecv(void)
{
    if (USB_DeviceCdcEcmRecv(ethNicHandle.deviceHandle, USB_DEVICE_CDC_ECM_DATA_BULK_OUT_EP_NUMBER, dataOutBuffer, APP_ETH_FRAME_MAX_LENGTH) != kStatus_USB_Success)
    {
        if (!ethNicHandle.attachStatus)
        {
            (void)USB_DeviceCancel(ethNicHandle.deviceHandle, USB_DEVICE_CDC_ECM_DATA_BULK_OUT_EP_NUMBER | USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_OUT);
        }
    }
#if USB_DEVICE_CONFIG_USE_TASK
    USB_DeviceTaskFn(ethNicHandle.deviceHandle);
#endif
}

void APP_TransferUSB2Ethernet_EthernetSend(void)
{
    eth_adapter_frame_buf_t *frame;

    if (ETH_ADAPTER_FrameQueueGet(&ethNicHandle.ethHandle->txFrameQueue, &frame) == ETH_ADAPTER_OK)
    {
        if (ETH_ADAPTER_SendFrame(frame) == ETH_ADAPTER_OK)
        {
            if (ETH_ADAPTER_FrameQueuePop(&ethNicHandle.ethHandle->txFrameQueue, NULL) != ETH_ADAPTER_OK)
            {
                (void)usb_echo("USB->ENET(ENET TX): Invalid frame was not popped up.\r\n");
            }
        }
    }
}

void APP_DeviceCdcEcmSend(eth_adapter_frame_buf_t *frame)
{
    uint32_t sentLen;
    uint32_t usbDataIdx = 0U;

    while (usbDataIdx < frame->len)
    {
        sentLen = MIN(frame->len - usbDataIdx, USB_DEVICE_CDC_ECM_CLASS_DESCRIPTOR_MAX_SEGMENT_SIZE);
        if (USB_DeviceCdcEcmSend(ethNicHandle.deviceHandle, USB_DEVICE_CDC_ECM_DATA_BULK_IN_EP_NUMBER, &frame->payload[usbDataIdx], sentLen) != kStatus_USB_Success)
        {
            if (!ethNicHandle.attachStatus)
            {
                (void)USB_DeviceCancel(ethNicHandle.deviceHandle, USB_DEVICE_CDC_ECM_DATA_BULK_IN_EP_NUMBER | USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN);
            }

            break;
        }
#if USB_DEVICE_CONFIG_USE_TASK
        USB_DeviceTaskFn(ethNicHandle.deviceHandle);
#endif

        usbDataIdx += sentLen;

        if (usbDataIdx == frame->len)
        {
            if (ETH_ADAPTER_FrameQueuePop(&ethNicHandle.ethHandle->rxFrameQueue, NULL) != ETH_ADAPTER_OK)
            {
                (void)usb_echo("USB(DATA IN): Sent frame was not popped up.\r\n");
            }

            if (sentLen == USB_DEVICE_CDC_ECM_CLASS_DESCRIPTOR_MAX_SEGMENT_SIZE)
            {
                if (USB_DeviceCdcEcmSend(ethNicHandle.deviceHandle, USB_DEVICE_CDC_ECM_DATA_BULK_IN_EP_NUMBER, &zlpBuffer, 0) != kStatus_USB_Success)
                {
                    if (!ethNicHandle.attachStatus)
                    {
                        (void)USB_DeviceCancel(ethNicHandle.deviceHandle, USB_DEVICE_CDC_ECM_DATA_BULK_IN_EP_NUMBER | USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN);
                    }
                }
#if USB_DEVICE_CONFIG_USE_TASK
                USB_DeviceTaskFn(ethNicHandle.deviceHandle);
#endif
            }
        }
    }
}

void APP_TransferEthernet2USB_USBSend(void)
{
    eth_adapter_frame_buf_t *data;

    if (ETH_ADAPTER_FrameQueueGet(&ethNicHandle.ethHandle->rxFrameQueue, &data) == ETH_ADAPTER_OK)
    {
        if (!data->len)
        {
            if (ETH_ADAPTER_FrameQueueDrop(&ethNicHandle.ethHandle->rxFrameQueue, NULL) != ETH_ADAPTER_OK)
            {
                (void)usb_echo("ENET->USB(USB DATA IN): Empty frame was not dropped.\r\n");
            }
        }
        else
        {
            APP_DeviceCdcEcmSend(data);
        }
    }
}

void APP_TransferEthernet2USB_EthernetRecv(void)
{
    eth_adapter_frame_buf_t *data;

    if (ETH_ADAPTER_FrameQueueAlloc(&ethNicHandle.ethHandle->rxFrameQueue, &data) == ETH_ADAPTER_OK)
    {
        if (ETH_ADAPTER_RecvFrame(data, APP_ETH_FRAME_MAX_LENGTH) != ETH_ADAPTER_OK)
        {
            if (ETH_ADAPTER_FrameQueueDrop(&ethNicHandle.ethHandle->rxFrameQueue, NULL) != ETH_ADAPTER_OK)
            {
                (void)usb_echo("ENET->USB(ENET RX): Allocated empty frame was not dropped.\r\n");
            }
        }
        else
        {
            if (!data->len)
            {
                if (ETH_ADAPTER_FrameQueueDrop(&ethNicHandle.ethHandle->rxFrameQueue, NULL) != ETH_ADAPTER_OK)
                {
                    (void)usb_echo("ENET->USB(ENET RX): Empty frame was not dropped.\r\n");
                }
            }
        }
    }
}

void APP_Init(void)
{
    USB_DeviceClockInit();

    ethNicHandle.deviceHandle = NULL;
    ethNicHandle.ethHandle = &ethAdapterHandle;

    if (ETH_ADAPTER_Init() != ETH_ADAPTER_OK)
    {
        (void)usb_echo("ETH_ADAPTER_Init() occurs error.\r\n");

        return;
    }

    if (USB_DeviceInit(CONTROLLER_ID, USB_DeviceCallback, &ethNicHandle.deviceHandle) != kStatus_USB_Success)
    {
        (void)usb_echo("USB_DeviceClassInit() occurs error.\r\n");

        return;
    }
    else
    {
        USB_FillStringDescriptorBuffer();
        (void)usb_echo("USB CDC-ECM NIC Device\r\n");
    }

    USB_DeviceIsrEnable();

    /* Add one delay here to make the DP pull down long enough to allow host to detect the previous disconnection. */
    SDK_DelayAtLeastUs(5000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    USB_DeviceRun(ethNicHandle.deviceHandle);
}

void APP_EncapsulateUSBRequest(uint8_t *buffer, usb_setup_struct_t *setup, uint8_t *data, uint32_t length)
{
    uint8_t offset = sizeof(usb_setup_struct_t);

    if (buffer)
    {
        memcpy(buffer, setup, offset);
    }

    if (data && (buffer + offset))
    {
        memcpy(buffer + offset, data, length);
    }
}

void APP_CheckLinkChange(void)
{
    bool link = false;
    (void)ETH_ADAPTER_GetLinkStatus(&link);
    if (ethNicHandle.linkStatus != link)
    {
        ethNicHandle.linkStatus = link;
        APP_ETH_NIC_EVENT_SET(appEvent, kAPP_NotifyNetworkChange);
    }
}

void APP_NotifyLinkStatus(void)
{
    usb_setup_struct_t req;
    uint32_t speedMap[2];

    req.bmRequestType = USB_REQUEST_TYPE_DIR_IN | USB_REQUEST_TYPE_TYPE_CLASS | USB_REQUEST_TYPE_RECIPIENT_INTERFACE;
    req.bRequest = USB_DEVICE_CDC_NETWORK_CONNECTION;
    req.wValue = (uint16_t)ethNicHandle.linkStatus;
    req.wIndex = USB_DEVICE_CDC_ECM_COMM_INTERFACE_NUMBER + 1;
    req.wLength = 0;

    APP_EncapsulateUSBRequest(notify_network_connection_req, &req, NULL, 0);
    while (USB_DeviceCdcEcmSend(ethNicHandle.deviceHandle, USB_DEVICE_CDC_ECM_COMM_INTERRUPT_IN_EP_NUMBER, notify_network_connection_req, 8) != kStatus_USB_Success)
    {
#if USB_DEVICE_CONFIG_USE_TASK
        USB_DeviceTaskFn(ethNicHandle.deviceHandle);
#endif
    }

    req.bmRequestType = USB_REQUEST_TYPE_DIR_IN | USB_REQUEST_TYPE_TYPE_CLASS | USB_REQUEST_TYPE_RECIPIENT_INTERFACE;
    req.bRequest = USB_DEVICE_CDC_CONNECTION_SPEED_CHANGE;
    req.wValue = 0;
    req.wIndex = USB_DEVICE_CDC_ECM_COMM_INTERFACE_NUMBER + 1;
    req.wLength = 8;

    if (!ethNicHandle.linkStatus || (ETH_ADAPTER_GetLinkSpeed(&ethNicHandle.linkSpeed) != ETH_ADAPTER_OK))
    {
        ethNicHandle.linkSpeed = 0;
    }

    speedMap[0] = ethNicHandle.linkSpeed;
    speedMap[1] = ethNicHandle.linkSpeed;

    APP_EncapsulateUSBRequest(notify_connection_speed_change_req, &req, (uint8_t *)speedMap, 8);
    while (USB_DeviceCdcEcmSend(ethNicHandle.deviceHandle, USB_DEVICE_CDC_ECM_COMM_INTERRUPT_IN_EP_NUMBER, notify_connection_speed_change_req, 16) != kStatus_USB_Success)
    {
#if USB_DEVICE_CONFIG_USE_TASK
        USB_DeviceTaskFn(ethNicHandle.deviceHandle);
#endif
    }

    if (ethNicHandle.linkStatus)
    {
        APP_ETH_NIC_EVENT_SET(appEvent, kAPP_UsbDataXfer);
    }
    else
    {
        APP_ETH_NIC_EVENT_UNSET(appEvent, kAPP_UsbDataXfer);
    }
}

usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param)
{
    usb_status_t status = kStatus_USB_Error;

    switch (event)
    {
        case kUSB_DeviceEventBusReset:
#if (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
#if !((defined FSL_FEATURE_SOC_USBPHY_COUNT) && (FSL_FEATURE_SOC_USBPHY_COUNT > 0U))
            /* The work-around is used to fix the HS device Chirping issue.
             * Please refer to the implementation for the detail information.
             */
            USB_DeviceHsPhyChirpIssueWorkaround();
#endif
#endif

            if (USB_DeviceControlPipeInit(ethNicHandle.deviceHandle) != kStatus_USB_Success)
            {
                break;
            }

#if (defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)) || \
    (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
            /* Get USB speed to configure the device, including max packet size and interval of the endpoints. */
            if (USB_DeviceGetStatus(ethNicHandle.deviceHandle, kUSB_DeviceStatusSpeed, &ethNicHandle.deviceSpeed) == kStatus_USB_Success)
            {
                USB_DeviceSetSpeed(handle, ethNicHandle.deviceSpeed);
            }
#endif

            if (ETH_ADAPTER_FrameQueueClear(&ethNicHandle.ethHandle->txFrameQueue) != ETH_ADAPTER_OK)
            {
                break;
            }

            if (ETH_ADAPTER_FrameQueueClear(&ethNicHandle.ethHandle->rxFrameQueue) != ETH_ADAPTER_OK)
            {
                break;
            }

            ethNicHandle.configuration = 0U;
            ethNicHandle.attachStatus = 0U;
            ethNicHandle.linkStatus = 0U;

            APP_ETH_NIC_EVENT_CLEAR(appEvent);

            status = kStatus_USB_Success;
            break;

#if (defined(USB_DEVICE_CONFIG_DETACH_ENABLE) && (USB_DEVICE_CONFIG_DETACH_ENABLE > 0U))
        case kUSB_DeviceEventDetach:
            ethNicHandle.attachStatus = 0U;
            APP_ETH_NIC_EVENT_CLEAR(appEvent);
            status = kStatus_USB_Success;
            break;
#endif

        case kUSB_DeviceEventSetConfiguration:
        {
            uint8_t configuration = *((uint8_t *)param);
            if (configuration == USB_DEVICE_CDC_ECM_CONFIGURATION_VALUE)
            {
                usb_device_endpoint_init_struct_t epInitStruct;
                usb_device_endpoint_callback_struct_t epCallback;

                ethNicHandle.configuration = configuration;

                /* Initiailize endpoint for interrupt pipe */
                epCallback.callbackFn = USB_DeviceCdcEcmInterruptIn;
                epCallback.callbackParam = NULL;

                epInitStruct.zlt = 0U;
                epInitStruct.transferType = USB_ENDPOINT_INTERRUPT;
                epInitStruct.endpointAddress = USB_DEVICE_CDC_ECM_COMM_INTERRUPT_IN_EP_NUMBER | USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN;
                epInitStruct.maxPacketSize = USB_DEIVCE_CDC_ECM_COMM_INTERRUPT_IN_EP_MAXPKT_SIZE;
                if (USB_SPEED_HIGH == ethNicHandle.deviceSpeed)
                {
                    epInitStruct.interval = USB_DEIVCE_CDC_ECM_COMM_INTERRUPT_IN_EP_INTERVAL_HS;
                }
                else
                {
                    epInitStruct.interval = USB_DEIVCE_CDC_ECM_COMM_INTERRUPT_IN_EP_INTERVAL_FS;
                }

                USB_DeviceInitEndpoint(ethNicHandle.deviceHandle, &epInitStruct, &epCallback);

                /* Initiailize endpoints for bulk pipe */
                epCallback.callbackFn = USB_DeviceCdcEcmBulkIn;
                epCallback.callbackParam = NULL;

                epInitStruct.zlt = 0U;
                epInitStruct.transferType = USB_ENDPOINT_BULK;
                epInitStruct.interval = USB_DEIVCE_CDC_ECM_DATA_BULK_IN_EP_INTERVAL;
                epInitStruct.endpointAddress = USB_DEVICE_CDC_ECM_DATA_BULK_IN_EP_NUMBER | USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN;
                if (USB_SPEED_HIGH == ethNicHandle.deviceSpeed)
                {
                    epInitStruct.maxPacketSize = USB_DEVICE_CDC_ECM_DATA_BULK_IN_EP_MAXPKT_SIZE_HS;
                }
                else
                {
                    epInitStruct.maxPacketSize = USB_DEVICE_CDC_ECM_DATA_BULK_IN_EP_MAXPKT_SIZE_FS;
                }

                USB_DeviceInitEndpoint(ethNicHandle.deviceHandle, &epInitStruct, &epCallback);

                epCallback.callbackFn = USB_DeviceCdcEcmBulkOut;
                epCallback.callbackParam = NULL;

                epInitStruct.zlt = 0U;
                epInitStruct.transferType = USB_ENDPOINT_BULK;
                epInitStruct.interval = USB_DEIVCE_CDC_ECM_DATA_BULK_OUT_EP_INTERVAL;
                epInitStruct.endpointAddress = USB_DEVICE_CDC_ECM_DATA_BULK_OUT_EP_NUMBER | USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_OUT;
                if (USB_SPEED_HIGH == ethNicHandle.deviceSpeed)
                {
                    epInitStruct.maxPacketSize = USB_DEVICE_CDC_ECM_DATA_BULK_OUT_EP_MAXPKT_SIZE_HS;
                }
                else
                {
                    epInitStruct.maxPacketSize = USB_DEVICE_CDC_ECM_DATA_BULK_OUT_EP_MAXPKT_SIZE_FS;
                }

                USB_DeviceInitEndpoint(ethNicHandle.deviceHandle, &epInitStruct, &epCallback);

                status = kStatus_USB_Success;
            }
        }
        break;

        case kUSB_DeviceEventGetConfiguration:
            status = kStatus_USB_Success;
            break;

        case kUSB_DeviceEventSetInterface:
            status = kStatus_USB_Success;
            break;

        case kUSB_DeviceEventGetInterface:
            status = kStatus_USB_Success;
            break;

        default:
            status = kStatus_USB_InvalidRequest;
            break;
    }

    return status;
}

#if defined(__CC_ARM) || (defined(__ARMCC_VERSION)) || defined(__GNUC__)
int main(void)
#else
void main(void)
#endif
{
    BOARD_InitHardware();
    APP_Init();

    while (1U)
    {
        if (ethNicHandle.attachStatus)
        {
            if (APP_ETH_NIC_EVENT_GET(appEvent, kAPP_CheckLinkChange))
            {
                APP_ETH_NIC_EVENT_UNSET(appEvent, kAPP_CheckLinkChange);
                APP_CheckLinkChange();
            }

            if (APP_ETH_NIC_EVENT_GET(appEvent, kAPP_NotifyNetworkChange))
            {
                APP_ETH_NIC_EVENT_UNSET(appEvent, kAPP_NotifyNetworkChange);
                APP_NotifyLinkStatus();
            }

            if (APP_ETH_NIC_EVENT_GET(appEvent, kAPP_UsbDataXfer))
            {
                APP_TransferUSB2Ethernet_USBRecv();
                APP_TransferUSB2Ethernet_EthernetSend();
                APP_TransferEthernet2USB_EthernetRecv();
                APP_TransferEthernet2USB_USBSend();
            }
        }

#if USB_DEVICE_CONFIG_USE_TASK
        USB_DeviceTaskFn(ethNicHandle.deviceHandle);
#endif
    }
}
