/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_class.h"
#include "usb_device_video.h"

#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#include "video_camera.h"

#include "fsl_camera_receiver.h"
#include "fsl_camera_device.h"

#include "fsl_device_registers.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"

#include <stdio.h>
#include <stdlib.h>

#if ((defined FSL_FEATURE_SOC_USBPHY_COUNT) && (FSL_FEATURE_SOC_USBPHY_COUNT > 0U))
#include "usb_phy.h"
#endif

#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
#include "fsl_sysmpu.h"
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */

#include "pin_mux.h"
#include "fsl_port.h"
#include "fsl_flexio_camera.h"
#include "fsl_flexio_camera_adapter.h"
#include "fsl_i2c.h"
#include <stdbool.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define CAMERA_FRAME_BYTES                                                                    \
    (USB_VIDEO_CAMERA_UNCOMPRESSED_FRAME_WIDTH * USB_VIDEO_CAMERA_UNCOMPRESSED_FRAME_HEIGHT * \
     USB_VIDEO_CAMERA_UNCOMPRESSED_FRAME_DATA_BITS)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void FLEXIO_Ov7670AsynCallback(void);
void BOARD_InitHardware(void);
void USB_DeviceClockInit(void);
void USB_DeviceIsrEnable(void);
#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle);
#endif

static void USB_DeviceVideoPrepareVideoData(void);
static usb_status_t USB_DeviceVideoRequest(class_handle_t handle, uint32_t event, void *param);
static usb_status_t USB_DeviceVideoCallback(class_handle_t handle, uint32_t event, void *param);
static void USB_DeviceVideoApplicationSetDefault(void);
static usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param);
static void USB_DeviceApplicationInit(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern camera_device_handle_t cameraDevice;
extern camera_receiver_handle_t cameraReceiver;

extern const unsigned char g_UsbDeviceVideoMjpegData[];
extern const uint32_t g_UsbDeviceVideoMjpegLength;

extern usb_device_class_struct_t g_UsbDeviceVideoFlexioCameraConfig;

usb_video_flexio_camera_struct_t g_UsbDeviceVideoFlexioCamera;

usb_device_class_config_struct_t g_UsbDeviceVideoConfig[1] = {{
    USB_DeviceVideoCallback, (class_handle_t)NULL, &g_UsbDeviceVideoFlexioCameraConfig,
}};

usb_device_class_config_list_struct_t g_UsbDeviceVideoConfigList = {
    g_UsbDeviceVideoConfig, USB_DeviceCallback, 1U,
};

USB_DMA_NONINIT_DATA_ALIGN(((32 > USB_DATA_ALIGN_SIZE) ? 32 : USB_DATA_ALIGN_SIZE))
uint8_t g_FlexioCameraFrameBuffer[2][USB_DATA_ALIGN_SIZE_MULTIPLE(CAMERA_FRAME_BYTES + 32)];

/*******************************************************************************
 * Code
 ******************************************************************************/
extern void FLEXIO_CAMERA_VsyncHandleIRQ(camera_receiver_handle_t *handle);
static void BOARD_FLEXIO_CAMERA_VsyncInit(void)
{
    GPIO_Type *gpioBase[] = GPIO_BASE_PTRS;
    PORT_Type *portBase[] = PORT_BASE_PTRS;
    IRQn_Type portIRQn[]  = PORT_IRQS;
    gpio_pin_config_t pinConfig;
    /* Configure OV7670 VSYN input */
    pinConfig.pinDirection = kGPIO_DigitalInput;

    GPIO_PinInit(gpioBase[BOARD_FLEXIO_VSYNC_PORT_INDEX], BOARD_FLEXIO_VSYNC_PIN_INDEX, &pinConfig);
    PORT_SetPinInterruptConfig(portBase[BOARD_FLEXIO_VSYNC_PORT_INDEX], BOARD_FLEXIO_VSYNC_PIN_INDEX,
                               kPORT_InterruptRisingEdge);
    EnableIRQ(portIRQn[BOARD_FLEXIO_VSYNC_PORT_INDEX]);
}

/* OV7670 connect to FLEXIO. */
static flexio_camera_resource_t flexio_cameraResource = {
    .device =
        {
            .flexioBase      = BOARD_FLEXIO_INSTANCE_BASE,
            .datPinStartIdx  = BOARD_FLEXIO_DATA_PIN_START_INDEX, /* fxio_pin 4 - 11 are used. */
            .pclkPinIdx      = BOARD_FLEXIO_PCLK_PIN_INDEX,       /* fxio_pin 0 is used as pclk pin. */
            .hrefPinIdx      = BOARD_FLEXIO_HREF_PIN_INDEX,       /* flexio_pin 3 is used as href pin. */
            .shifterStartIdx = 0U,                                /* Shifter 0 = 7 are used. */
            .shifterCount    = BOARD_FLEXIO_DATA_PINS,
            .timerIdx        = 0U,
        },
    .dmamuxBase = DMAMUX0,
    .dmaBase    = DMA0,
    .dmaChannel = FSL_FEATURE_FLEXIO_DMA_REQUEST_BASE_CHANNEL,
    .vsyncInit  = BOARD_FLEXIO_CAMERA_VsyncInit,
};

static flexio_camera_private_data_t flexioCameraPrivateData;

camera_receiver_handle_t cameraReceiver = {
    .resource    = &flexio_cameraResource,
    .ops         = &flexio_camera_ops,
    .privateData = &flexioCameraPrivateData,
};

void BOARD_InitCameraResource(void)
{
    GPIO_Type *gpioBase[] = GPIO_BASE_PTRS;
    gpio_pin_config_t pinConfig;

    BOARD_Camera_I2C_Init();

    /* disable OV7670 power down */
    pinConfig.pinDirection = kGPIO_DigitalOutput;
    pinConfig.outputLogic  = 0U;

    GPIO_PinInit(gpioBase[BOARD_FLEXIO_PWDN_PORT_INDEX], BOARD_FLEXIO_PWDN_PIN_INDEX, &pinConfig);

    /* enable OV7670 Reset output */
    pinConfig.pinDirection = kGPIO_DigitalOutput;
    pinConfig.outputLogic  = 0U;

    GPIO_PinInit(gpioBase[BOARD_FLEXIO_RESET_PORT_INDEX], BOARD_FLEXIO_RESET_PIN_INDEX, &pinConfig);

    uint16_t i = 1000;
    while (i--)
    {
        __NOP();
    }
    GPIO_PinWrite(gpioBase[BOARD_FLEXIO_RESET_PORT_INDEX], BOARD_FLEXIO_RESET_PIN_INDEX, 1);
    i = 1000;
    while (i--)
    {
        __NOP();
    }
}

void BOARD_InitFlexioResource(void)
{
    /* Configure DMAMUX */
    DMAMUX_Init(DMAMUX0);

    /* Configure DMA */
    edma_config_t edmaConfig;
    EDMA_GetDefaultConfig(&edmaConfig);
    EDMA_Init(DMA0, &edmaConfig);
}

static ov7670_resource_t ov7670Resource = {
    .i2cSendFunc    = BOARD_Camera_I2C_SendSCCB,
    .i2cReceiveFunc = BOARD_Camera_I2C_ReceiveSCCB,
    .xclock         = kOV7670_InputClock12MHZ,
};

camera_device_handle_t cameraDevice = {
    .resource = &ov7670Resource,
    .ops      = &ov7670_ops,
};

void PORTB_IRQHandler(void)
{
    /* Clear the interrupt flag for PTB2 */
    PORT_ClearPinsInterruptFlags(PORTB, 1U << BOARD_FLEXIO_VSYNC_PIN_INDEX);
    FLEXIO_CAMERA_VsyncHandleIRQ(&cameraReceiver);
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
    exception return operation might vector to incorrect interrupt */
    __DSB();
}

void USB0_IRQHandler(void)
{
    USB_DeviceKhciIsrFunction(g_UsbDeviceVideoFlexioCamera.deviceHandle);
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
    exception return operation might vector to incorrect interrupt */
    __DSB();
}
void USB_DeviceClockInit(void)
{
    SystemCoreClockUpdate();
    CLOCK_EnableUsbfs0Clock(kCLOCK_UsbSrcIrc48M, 48000000U);
}
void USB_DeviceIsrEnable(void)
{
    uint8_t irqNumber;

    uint8_t usbDeviceKhciIrq[] = USB_IRQS;
    irqNumber                  = usbDeviceKhciIrq[CONTROLLER_ID - kUSB_ControllerKhci0];

    /* Install isr, set priority, and enable IRQ. */
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
    EnableIRQ((IRQn_Type)irqNumber);
}
#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle)
{
    USB_DeviceKhciTaskFunction(deviceHandle);
}
#endif

/* Prepare next transfer payload */
static void USB_DeviceVideoPrepareVideoData(void)
{
    usb_device_video_mjpeg_payload_header_struct_t *payloadHeader;
    uint32_t fullBufferAddr;
    uint32_t maxPacketSize;
    uint32_t i;
    uint32_t sendLength;
    uint32_t temp32dwFrameInterval;

    g_UsbDeviceVideoFlexioCamera.currentTime += 10000U;

    if (g_UsbDeviceVideoFlexioCamera.imagePosition < CAMERA_FRAME_BYTES)
    {
        i = (uint32_t)&g_FlexioCameraFrameBuffer[g_UsbDeviceVideoFlexioCamera.fullBufferIndex]
                                                [g_UsbDeviceVideoFlexioCamera.imagePosition -
                                                 sizeof(usb_device_video_mjpeg_payload_header_struct_t) + 32];
        g_UsbDeviceVideoFlexioCamera.imageBuffer = (uint8_t *)i;
    }
    else
    {
        g_UsbDeviceVideoFlexioCamera.imageBuffer = (uint8_t *)&g_UsbDeviceVideoFlexioCamera.image_header[0];
    }
    payloadHeader = (usb_device_video_mjpeg_payload_header_struct_t *)g_UsbDeviceVideoFlexioCamera.imageBuffer;
    for (i = 0; i < sizeof(usb_device_video_mjpeg_payload_header_struct_t); i++)
    {
        g_UsbDeviceVideoFlexioCamera.imageBuffer[i] = 0x00U;
    }
    payloadHeader->bHeaderLength = sizeof(usb_device_video_mjpeg_payload_header_struct_t);
    payloadHeader->headerInfoUnion.bmheaderInfo = 0U;
    payloadHeader->headerInfoUnion.headerInfoBits.frameIdentifier = g_UsbDeviceVideoFlexioCamera.currentFrameId;
    g_UsbDeviceVideoFlexioCamera.imageBufferLength = sizeof(usb_device_video_mjpeg_payload_header_struct_t);

    if (g_UsbDeviceVideoFlexioCamera.stillImageTransmission)
    {
        payloadHeader->headerInfoUnion.headerInfoBits.stillImage = 1U;
        maxPacketSize =
            USB_LONG_FROM_LITTLE_ENDIAN_DATA(g_UsbDeviceVideoFlexioCamera.stillCommitStruct.dwMaxPayloadTransferSize);
    }
    else
    {
        maxPacketSize =
            USB_LONG_FROM_LITTLE_ENDIAN_DATA(g_UsbDeviceVideoFlexioCamera.commitStruct.dwMaxPayloadTransferSize);
    }
    maxPacketSize = maxPacketSize - sizeof(usb_device_video_mjpeg_payload_header_struct_t);
    if (g_UsbDeviceVideoFlexioCamera.waitForNewInterval)
    {
        temp32dwFrameInterval =
            USB_LONG_FROM_LITTLE_ENDIAN_DATA(g_UsbDeviceVideoFlexioCamera.commitStruct.dwFrameInterval);
        if (g_UsbDeviceVideoFlexioCamera.currentTime < temp32dwFrameInterval)
        {
            return;
        }
        else
        {
            g_UsbDeviceVideoFlexioCamera.imagePosition = 0U;
            g_UsbDeviceVideoFlexioCamera.currentTime = 0U;
            g_UsbDeviceVideoFlexioCamera.waitForNewInterval = 0U;
            payloadHeader->headerInfoUnion.headerInfoBits.endOfFrame = 1U;
            g_UsbDeviceVideoFlexioCamera.stillImageTransmission = 0U;
            g_UsbDeviceVideoFlexioCamera.currentFrameId ^= 1U;
            if (USB_DEVICE_VIDEO_STILL_IMAGE_TRIGGER_TRANSMIT_STILL_IMAGE ==
                g_UsbDeviceVideoFlexioCamera.stillImageTriggerControl)
            {
                g_UsbDeviceVideoFlexioCamera.stillImageTriggerControl =
                    USB_DEVICE_VIDEO_STILL_IMAGE_TRIGGER_NORMAL_OPERATION;
                g_UsbDeviceVideoFlexioCamera.stillImageTransmission = 1U;
            }
            return;
        }
    }

    if (g_UsbDeviceVideoFlexioCamera.imagePosition < CAMERA_FRAME_BYTES)
    {
        sendLength = CAMERA_FRAME_BYTES - g_UsbDeviceVideoFlexioCamera.imagePosition;
        if (sendLength > maxPacketSize)
        {
            sendLength = maxPacketSize;
        }
        g_UsbDeviceVideoFlexioCamera.imagePosition += sendLength;

        if (g_UsbDeviceVideoFlexioCamera.imagePosition >= CAMERA_FRAME_BYTES)
        {
            CAMERA_RECEIVER_SubmitEmptyBuffer(
                &cameraReceiver,
                (uint32_t)&g_FlexioCameraFrameBuffer[g_UsbDeviceVideoFlexioCamera.fullBufferIndex][32]);
            CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &fullBufferAddr);
            temp32dwFrameInterval =
                USB_LONG_FROM_LITTLE_ENDIAN_DATA(g_UsbDeviceVideoFlexioCamera.commitStruct.dwFrameInterval);
            if (((uint32_t)fullBufferAddr) == ((uint32_t)&g_FlexioCameraFrameBuffer[0][32]))
            {
                g_UsbDeviceVideoFlexioCamera.fullBufferIndex = 0;
            }
            else
            {
                g_UsbDeviceVideoFlexioCamera.fullBufferIndex = 1;
            }
            if (g_UsbDeviceVideoFlexioCamera.currentTime < temp32dwFrameInterval)
            {
                g_UsbDeviceVideoFlexioCamera.waitForNewInterval = 1U;
            }
            else
            {
                g_UsbDeviceVideoFlexioCamera.imagePosition = 0U;
                g_UsbDeviceVideoFlexioCamera.currentTime = 0U;
                payloadHeader->headerInfoUnion.headerInfoBits.endOfFrame = 1U;
                g_UsbDeviceVideoFlexioCamera.stillImageTransmission = 0U;
                g_UsbDeviceVideoFlexioCamera.currentFrameId ^= 1U;
                if (USB_DEVICE_VIDEO_STILL_IMAGE_TRIGGER_TRANSMIT_STILL_IMAGE ==
                    g_UsbDeviceVideoFlexioCamera.stillImageTriggerControl)
                {
                    g_UsbDeviceVideoFlexioCamera.stillImageTriggerControl =
                        USB_DEVICE_VIDEO_STILL_IMAGE_TRIGGER_NORMAL_OPERATION;
                    g_UsbDeviceVideoFlexioCamera.stillImageTransmission = 1U;
                }
            }
        }
        g_UsbDeviceVideoFlexioCamera.imageBufferLength += sendLength;
    }
}

static usb_status_t USB_DeviceVideoRequest(class_handle_t handle, uint32_t event, void *param)
{
    usb_device_control_request_struct_t *request = (usb_device_control_request_struct_t *)param;
    usb_device_video_probe_and_commit_controls_struct_t *probe =
        (usb_device_video_probe_and_commit_controls_struct_t *)(request->buffer);
    usb_device_video_probe_and_commit_controls_struct_t *commit =
        (usb_device_video_probe_and_commit_controls_struct_t *)(request->buffer);
    usb_device_video_still_probe_and_commit_controls_struct_t *still_probe =
        (usb_device_video_still_probe_and_commit_controls_struct_t *)(request->buffer);
    usb_device_video_still_probe_and_commit_controls_struct_t *still_commit =
        (usb_device_video_still_probe_and_commit_controls_struct_t *)(request->buffer);
    uint32_t temp32;
    usb_status_t error = kStatus_USB_Success;

    switch (event)
    {
        /* probe request */
        case USB_DEVICE_VIDEO_SET_CUR_VS_PROBE_CONTROL:
            if ((request->buffer == NULL) || (request->length == 0U))
            {
                return kStatus_USB_InvalidRequest;
            }
            temp32 = USB_LONG_FROM_LITTLE_ENDIAN_DATA(probe->dwFrameInterval);
            if ((temp32 >= USB_VIDEO_CAMERA_UNCOMPRESSED_FRAME_MIN_INTERVAL) &&
                (temp32 <= USB_VIDEO_CAMERA_UNCOMPRESSED_FRAME_MAX_INTERVAL))
            {
                USB_LONG_TO_LITTLE_ENDIAN_DATA(temp32, g_UsbDeviceVideoFlexioCamera.probeStruct.dwFrameInterval);
            }
            temp32 = USB_LONG_FROM_LITTLE_ENDIAN_DATA(probe->dwMaxPayloadTransferSize);
            if ((temp32) && (temp32 < g_UsbDeviceVideoFlexioCamera.currentMaxPacketSize))
            {
                USB_LONG_TO_LITTLE_ENDIAN_DATA(temp32,
                                               g_UsbDeviceVideoFlexioCamera.probeStruct.dwMaxPayloadTransferSize);
            }
            g_UsbDeviceVideoFlexioCamera.probeStruct.bFormatIndex = probe->bFormatIndex;
            g_UsbDeviceVideoFlexioCamera.probeStruct.bFrameIndex = probe->bFrameIndex;
            break;
        case USB_DEVICE_VIDEO_GET_CUR_VS_PROBE_CONTROL:
            request->buffer = (uint8_t *)&g_UsbDeviceVideoFlexioCamera.probeStruct;
            request->length = g_UsbDeviceVideoFlexioCamera.probeLength;
            break;
        case USB_DEVICE_VIDEO_GET_LEN_VS_PROBE_CONTROL:
            request->buffer = &g_UsbDeviceVideoFlexioCamera.probeLength;
            request->length = sizeof(g_UsbDeviceVideoFlexioCamera.probeLength);
            break;
        case USB_DEVICE_VIDEO_GET_INFO_VS_PROBE_CONTROL:
            request->buffer = &g_UsbDeviceVideoFlexioCamera.probeInfo;
            request->length = sizeof(g_UsbDeviceVideoFlexioCamera.probeInfo);
            break;
        /* commit request */
        case USB_DEVICE_VIDEO_SET_CUR_VS_COMMIT_CONTROL:
            if ((request->buffer == NULL) || (request->length == 0U))
            {
                return kStatus_USB_InvalidRequest;
            }
            temp32 = USB_LONG_FROM_LITTLE_ENDIAN_DATA(commit->dwFrameInterval);
            if ((temp32 >= USB_VIDEO_CAMERA_UNCOMPRESSED_FRAME_MIN_INTERVAL) &&
                (temp32 <= USB_VIDEO_CAMERA_UNCOMPRESSED_FRAME_MAX_INTERVAL))
            {
                USB_LONG_TO_LITTLE_ENDIAN_DATA(temp32, g_UsbDeviceVideoFlexioCamera.commitStruct.dwFrameInterval);
            }

            temp32 = USB_LONG_FROM_LITTLE_ENDIAN_DATA(commit->dwMaxPayloadTransferSize);
            if ((temp32) && (temp32 < g_UsbDeviceVideoFlexioCamera.currentMaxPacketSize))
            {
                USB_LONG_TO_LITTLE_ENDIAN_DATA(temp32,
                                               g_UsbDeviceVideoFlexioCamera.commitStruct.dwMaxPayloadTransferSize);
            }
            g_UsbDeviceVideoFlexioCamera.commitStruct.bFormatIndex = commit->bFormatIndex;
            g_UsbDeviceVideoFlexioCamera.commitStruct.bFrameIndex = commit->bFrameIndex;
            break;
        case USB_DEVICE_VIDEO_GET_CUR_VS_COMMIT_CONTROL:
            request->buffer = (uint8_t *)&g_UsbDeviceVideoFlexioCamera.commitStruct;
            request->length = g_UsbDeviceVideoFlexioCamera.commitLength;
            break;
        case USB_DEVICE_VIDEO_GET_LEN_VS_COMMIT_CONTROL:
            request->buffer = &g_UsbDeviceVideoFlexioCamera.commitLength;
            request->length = sizeof(g_UsbDeviceVideoFlexioCamera.commitLength);
            break;
        case USB_DEVICE_VIDEO_GET_INFO_VS_COMMIT_CONTROL:
            request->buffer = &g_UsbDeviceVideoFlexioCamera.commitInfo;
            request->length = sizeof(g_UsbDeviceVideoFlexioCamera.commitInfo);
            break;
        /* still probe request */
        case USB_DEVICE_VIDEO_SET_CUR_VS_STILL_PROBE_CONTROL:
            if ((request->buffer == NULL) || (request->length == 0U))
            {
                return kStatus_USB_InvalidRequest;
            }

            temp32 = USB_LONG_FROM_LITTLE_ENDIAN_DATA(still_probe->dwMaxPayloadTransferSize);
            if ((temp32) && (temp32 < g_UsbDeviceVideoFlexioCamera.currentMaxPacketSize))
            {
                USB_LONG_TO_LITTLE_ENDIAN_DATA(temp32,
                                               g_UsbDeviceVideoFlexioCamera.stillProbeStruct.dwMaxPayloadTransferSize);
            }

            g_UsbDeviceVideoFlexioCamera.stillProbeStruct.bFormatIndex = still_probe->bFormatIndex;
            g_UsbDeviceVideoFlexioCamera.stillProbeStruct.bFrameIndex = still_probe->bFrameIndex;
            break;
        case USB_DEVICE_VIDEO_GET_CUR_VS_STILL_PROBE_CONTROL:
            request->buffer = (uint8_t *)&g_UsbDeviceVideoFlexioCamera.stillProbeStruct;
            request->length = g_UsbDeviceVideoFlexioCamera.stillProbeLength;
            break;
        case USB_DEVICE_VIDEO_GET_LEN_VS_STILL_PROBE_CONTROL:
            request->buffer = &g_UsbDeviceVideoFlexioCamera.stillProbeLength;
            request->length = sizeof(g_UsbDeviceVideoFlexioCamera.stillProbeLength);
            break;
        case USB_DEVICE_VIDEO_GET_INFO_VS_STILL_PROBE_CONTROL:
            request->buffer = &g_UsbDeviceVideoFlexioCamera.stillProbeInfo;
            request->length = sizeof(g_UsbDeviceVideoFlexioCamera.stillProbeInfo);
            break;
        /* still commit request */
        case USB_DEVICE_VIDEO_SET_CUR_VS_STILL_COMMIT_CONTROL:
            if ((request->buffer == NULL) || (request->length == 0U))
            {
                return kStatus_USB_InvalidRequest;
            }
            temp32 = USB_LONG_FROM_LITTLE_ENDIAN_DATA(still_commit->dwMaxPayloadTransferSize);
            if ((temp32) && (temp32 < g_UsbDeviceVideoFlexioCamera.currentMaxPacketSize))
            {
                USB_LONG_TO_LITTLE_ENDIAN_DATA(temp32,
                                               g_UsbDeviceVideoFlexioCamera.stillCommitStruct.dwMaxPayloadTransferSize);
            }

            g_UsbDeviceVideoFlexioCamera.stillCommitStruct.bFormatIndex = still_commit->bFormatIndex;
            g_UsbDeviceVideoFlexioCamera.stillCommitStruct.bFrameIndex = still_commit->bFrameIndex;
            break;
        case USB_DEVICE_VIDEO_GET_CUR_VS_STILL_COMMIT_CONTROL:
            request->buffer = (uint8_t *)&g_UsbDeviceVideoFlexioCamera.stillCommitStruct;
            request->length = g_UsbDeviceVideoFlexioCamera.stillCommitLength;
            break;
        case USB_DEVICE_VIDEO_GET_LEN_VS_STILL_COMMIT_CONTROL:
            request->buffer = &g_UsbDeviceVideoFlexioCamera.stillCommitLength;
            request->length = sizeof(g_UsbDeviceVideoFlexioCamera.stillCommitLength);
            break;
        case USB_DEVICE_VIDEO_GET_INFO_VS_STILL_COMMIT_CONTROL:
            request->buffer = &g_UsbDeviceVideoFlexioCamera.stillCommitInfo;
            request->length = sizeof(g_UsbDeviceVideoFlexioCamera.stillCommitInfo);
            break;
        /* still image trigger request */
        case USB_DEVICE_VIDEO_SET_CUR_VS_STILL_IMAGE_TRIGGER_CONTROL:
            g_UsbDeviceVideoFlexioCamera.stillImageTriggerControl = *(request->buffer);
            break;
        default:
            error = kStatus_USB_InvalidRequest;
            break;
    }
    return error;
}

/* USB device Video class callback */
static usb_status_t USB_DeviceVideoCallback(class_handle_t handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_Error;

    switch (event)
    {
        case kUSB_DeviceVideoEventStreamSendResponse:
            /* Stream data dent */
            if (g_UsbDeviceVideoFlexioCamera.attach)
            {
                /* Prepare the next stream data */
                USB_DeviceVideoPrepareVideoData();
                error = USB_DeviceVideoSend(
                    g_UsbDeviceVideoFlexioCamera.videoHandle, USB_VIDEO_CAMERA_STREAM_ENDPOINT_IN,
                    g_UsbDeviceVideoFlexioCamera.imageBuffer, g_UsbDeviceVideoFlexioCamera.imageBufferLength);
            }
            break;
        case kUSB_DeviceVideoEventClassRequestBuffer:
            if (param && (g_UsbDeviceVideoFlexioCamera.attach))
            {
                /* Get the class-specific OUT buffer */
                usb_device_control_request_struct_t *request = (usb_device_control_request_struct_t *)param;

                if (request->length <= sizeof(usb_device_video_probe_and_commit_controls_struct_t))
                {
                    request->buffer = (uint8_t *)&g_UsbDeviceVideoFlexioCamera.classRequestBuffer;
                    error = kStatus_USB_Success;
                }
            }
            break;
        default:
            if (param && (event > 0xFFU))
            {
                /* If the event is the class-specific request(Event > 0xFFU), handle the class-specific request */
                error = USB_DeviceVideoRequest(handle, event, param);
            }
            break;
    }

    return error;
}

/* Set to default state */
static void USB_DeviceVideoApplicationSetDefault(void)
{
    g_UsbDeviceVideoFlexioCamera.speed = USB_SPEED_FULL;
    g_UsbDeviceVideoFlexioCamera.attach = 0U;
    g_UsbDeviceVideoFlexioCamera.currentMaxPacketSize = FS_STREAM_IN_PACKET_SIZE;
    g_UsbDeviceVideoFlexioCamera.fullBufferIndex = 0U;
    g_UsbDeviceVideoFlexioCamera.imagePosition = 0U;

    g_UsbDeviceVideoFlexioCamera.probeStruct.bFormatIndex = USB_VIDEO_CAMERA_UNCOMPRESSED_FORMAT_INDEX;
    g_UsbDeviceVideoFlexioCamera.probeStruct.bFrameIndex = USB_VIDEO_CAMERA_UNCOMPRESSED_FRAME_INDEX;
    USB_LONG_TO_LITTLE_ENDIAN_DATA(USB_VIDEO_CAMERA_UNCOMPRESSED_FRAME_DEFAULT_INTERVAL,
                                   g_UsbDeviceVideoFlexioCamera.probeStruct.dwFrameInterval);
    USB_LONG_TO_LITTLE_ENDIAN_DATA(g_UsbDeviceVideoFlexioCamera.currentMaxPacketSize,
                                   g_UsbDeviceVideoFlexioCamera.probeStruct.dwMaxPayloadTransferSize);
    USB_LONG_TO_LITTLE_ENDIAN_DATA(USB_VIDEO_CAMERA_UNCOMPRESSED_FRAME_MAX_FRAME_SIZE,
                                   g_UsbDeviceVideoFlexioCamera.probeStruct.dwMaxVideoFrameSize);

    g_UsbDeviceVideoFlexioCamera.commitStruct.bFormatIndex = USB_VIDEO_CAMERA_UNCOMPRESSED_FORMAT_INDEX;
    g_UsbDeviceVideoFlexioCamera.commitStruct.bFrameIndex = USB_VIDEO_CAMERA_UNCOMPRESSED_FRAME_INDEX;
    USB_LONG_TO_LITTLE_ENDIAN_DATA(USB_VIDEO_CAMERA_UNCOMPRESSED_FRAME_DEFAULT_INTERVAL,
                                   g_UsbDeviceVideoFlexioCamera.commitStruct.dwFrameInterval);
    USB_LONG_TO_LITTLE_ENDIAN_DATA(g_UsbDeviceVideoFlexioCamera.currentMaxPacketSize,
                                   g_UsbDeviceVideoFlexioCamera.commitStruct.dwMaxPayloadTransferSize);
    USB_LONG_TO_LITTLE_ENDIAN_DATA(USB_VIDEO_CAMERA_UNCOMPRESSED_FRAME_MAX_FRAME_SIZE,
                                   g_UsbDeviceVideoFlexioCamera.commitStruct.dwMaxVideoFrameSize);

    g_UsbDeviceVideoFlexioCamera.probeInfo = 0x03U;
    g_UsbDeviceVideoFlexioCamera.probeLength = 26U;
    g_UsbDeviceVideoFlexioCamera.commitInfo = 0x03U;
    g_UsbDeviceVideoFlexioCamera.commitLength = 26U;

    g_UsbDeviceVideoFlexioCamera.stillProbeStruct.bFormatIndex = USB_VIDEO_CAMERA_UNCOMPRESSED_FORMAT_INDEX;
    g_UsbDeviceVideoFlexioCamera.stillProbeStruct.bFrameIndex = USB_VIDEO_CAMERA_UNCOMPRESSED_FRAME_INDEX;
    g_UsbDeviceVideoFlexioCamera.stillProbeStruct.bCompressionIndex = 0x01U;
    USB_LONG_TO_LITTLE_ENDIAN_DATA(g_UsbDeviceVideoFlexioCamera.currentMaxPacketSize,
                                   g_UsbDeviceVideoFlexioCamera.stillProbeStruct.dwMaxPayloadTransferSize);
    USB_LONG_TO_LITTLE_ENDIAN_DATA(USB_VIDEO_CAMERA_UNCOMPRESSED_FRAME_MAX_FRAME_SIZE,
                                   g_UsbDeviceVideoFlexioCamera.stillProbeStruct.dwMaxVideoFrameSize);

    g_UsbDeviceVideoFlexioCamera.stillCommitStruct.bFormatIndex = USB_VIDEO_CAMERA_UNCOMPRESSED_FORMAT_INDEX;
    g_UsbDeviceVideoFlexioCamera.stillCommitStruct.bFrameIndex = USB_VIDEO_CAMERA_UNCOMPRESSED_FRAME_INDEX;
    g_UsbDeviceVideoFlexioCamera.stillCommitStruct.bCompressionIndex = 0x01U;
    USB_LONG_TO_LITTLE_ENDIAN_DATA(g_UsbDeviceVideoFlexioCamera.currentMaxPacketSize,
                                   g_UsbDeviceVideoFlexioCamera.stillCommitStruct.dwMaxPayloadTransferSize);
    USB_LONG_TO_LITTLE_ENDIAN_DATA(USB_VIDEO_CAMERA_UNCOMPRESSED_FRAME_MAX_FRAME_SIZE,
                                   g_UsbDeviceVideoFlexioCamera.stillCommitStruct.dwMaxVideoFrameSize);

    g_UsbDeviceVideoFlexioCamera.stillProbeInfo = 0x03U;
    g_UsbDeviceVideoFlexioCamera.stillProbeLength = sizeof(g_UsbDeviceVideoFlexioCamera.stillProbeStruct);
    g_UsbDeviceVideoFlexioCamera.stillCommitInfo = 0x03U;
    g_UsbDeviceVideoFlexioCamera.stillCommitLength = sizeof(g_UsbDeviceVideoFlexioCamera.stillCommitStruct);

    g_UsbDeviceVideoFlexioCamera.currentTime = 0U;
    g_UsbDeviceVideoFlexioCamera.currentFrameId = 0U;
    g_UsbDeviceVideoFlexioCamera.currentStreamInterfaceAlternateSetting = 0U;
    g_UsbDeviceVideoFlexioCamera.imageBufferLength = 0U;
    g_UsbDeviceVideoFlexioCamera.imageIndex = 0U;
    g_UsbDeviceVideoFlexioCamera.waitForNewInterval = 0U;
    g_UsbDeviceVideoFlexioCamera.stillImageTransmission = 0U;
    g_UsbDeviceVideoFlexioCamera.stillImageTriggerControl = USB_DEVICE_VIDEO_STILL_IMAGE_TRIGGER_NORMAL_OPERATION;
}

/* The device callback */
static usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_Success;
    uint8_t *temp8 = (uint8_t *)param;
    uint16_t *temp16 = (uint16_t *)param;

    switch (event)
    {
        case kUSB_DeviceEventBusReset:
        {
            /* The device BUS reset signal detected */
            USB_DeviceVideoApplicationSetDefault();
            g_UsbDeviceVideoFlexioCamera.attach = 0U;
            g_UsbDeviceVideoFlexioCamera.currentConfiguration = 0U;
#if (defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)) || \
    (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
            /* Get USB speed to configure the device, including max packet size and interval of the endpoints. */
            if (kStatus_USB_Success == USB_DeviceGetStatus(g_UsbDeviceVideoFlexioCamera.deviceHandle,
                                                           kUSB_DeviceStatusSpeed, &g_UsbDeviceVideoFlexioCamera.speed))
            {
                USB_DeviceSetSpeed(g_UsbDeviceVideoFlexioCamera.deviceHandle, g_UsbDeviceVideoFlexioCamera.speed);
            }

            if (USB_SPEED_HIGH == g_UsbDeviceVideoFlexioCamera.speed)
            {
                g_UsbDeviceVideoFlexioCamera.currentMaxPacketSize = HS_STREAM_IN_PACKET_SIZE;
            }
#endif
        }
        break;
        case kUSB_DeviceEventSetConfiguration:
            if (0 == (*temp8))
            {
                g_UsbDeviceVideoFlexioCamera.attach = 0U;
                g_UsbDeviceVideoFlexioCamera.currentConfiguration = 0U;
            }
            else if (USB_VIDEO_CAMERA_CONFIGURE_INDEX == (*temp8))
            {
                /* Set the configuration request */
                g_UsbDeviceVideoFlexioCamera.attach = 1U;
                g_UsbDeviceVideoFlexioCamera.currentConfiguration = *temp8;
            }
            else
            {
                error = kStatus_USB_InvalidRequest;
            }
            break;
        case kUSB_DeviceEventSetInterface:
            if ((g_UsbDeviceVideoFlexioCamera.attach) && param)
            {
                /* Set alternateSetting of the interface request */
                uint8_t interface = (uint8_t)((*temp16 & 0xFF00U) >> 0x08U);
                uint8_t alternateSetting = (uint8_t)(*temp16 & 0x00FFU);

                if (g_UsbDeviceVideoFlexioCamera.currentInterfaceAlternateSetting[interface] != alternateSetting)
                {
                    if (!g_UsbDeviceVideoFlexioCamera.currentInterfaceAlternateSetting[interface])
                    {
                        if (USB_VIDEO_CAMERA_STREAM_INTERFACE_INDEX == interface)
                        {
                            USB_DeviceVideoPrepareVideoData();
                            error = USB_DeviceSendRequest(g_UsbDeviceVideoFlexioCamera.deviceHandle,
                                                          USB_VIDEO_CAMERA_STREAM_ENDPOINT_IN,
                                                          g_UsbDeviceVideoFlexioCamera.imageBuffer,
                                                          g_UsbDeviceVideoFlexioCamera.imageBufferLength);
                        }
                    }
                    g_UsbDeviceVideoFlexioCamera.currentInterfaceAlternateSetting[interface] = alternateSetting;
                }
            }
            break;
        case kUSB_DeviceEventGetConfiguration:
            if (param)
            {
                /* Get the current configuration request */
                *temp8 = g_UsbDeviceVideoFlexioCamera.currentConfiguration;
                error = kStatus_USB_Success;
            }
            break;
        case kUSB_DeviceEventGetInterface:
            if (param)
            {
                /* Set the alternateSetting of the interface request */
                uint8_t interface = (uint8_t)((*temp16 & 0xFF00U) >> 0x08U);
                if (interface < USB_VIDEO_CAMERA_INTERFACE_COUNT)
                {
                    *temp16 =
                        (*temp16 & 0xFF00U) | g_UsbDeviceVideoFlexioCamera.currentInterfaceAlternateSetting[interface];
                    error = kStatus_USB_Success;
                }
                else
                {
                    error = kStatus_USB_InvalidRequest;
                }
            }
            break;
        case kUSB_DeviceEventGetDeviceDescriptor:
            if (param)
            {
                /* Get the device descriptor request */
                error = USB_DeviceGetDeviceDescriptor(handle, (usb_device_get_device_descriptor_struct_t *)param);
            }
            break;
        case kUSB_DeviceEventGetConfigurationDescriptor:
            if (param)
            {
                /* Get the configuration descriptor request */
                error = USB_DeviceGetConfigurationDescriptor(handle,
                                                             (usb_device_get_configuration_descriptor_struct_t *)param);
            }
            break;
        case kUSB_DeviceEventGetStringDescriptor:
            if (param)
            {
                /* Get the string descriptor request */
                error = USB_DeviceGetStringDescriptor(handle, (usb_device_get_string_descriptor_struct_t *)param);
            }
            break;
        default:
            break;
    }

    return error;
}

static void USB_DeviceApplicationInit(void)
{
    USB_DeviceClockInit();
#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
    SYSMPU_Enable(SYSMPU, 0);
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */

    USB_DeviceVideoApplicationSetDefault();

    if (kStatus_USB_Success !=
        USB_DeviceClassInit(CONTROLLER_ID, &g_UsbDeviceVideoConfigList, &g_UsbDeviceVideoFlexioCamera.deviceHandle))
    {
        usb_echo("USB device video camera failed\r\n");
        return;
    }
    else
    {
        usb_echo("USB device video camera demo\r\n");
        g_UsbDeviceVideoFlexioCamera.videoHandle = g_UsbDeviceVideoConfigList.config->classHandle;
    }

    USB_DeviceIsrEnable();

    const camera_config_t cameraConfig = {
        .pixelFormat = kVIDEO_PixelFormatRGB565,
        .bytesPerPixel = 2,
        .resolution = kVIDEO_ResolutionQQVGA,
        .framePerSec = 25,
    };

    CAMERA_DEVICE_Init(&cameraDevice, &cameraConfig);

    CAMERA_DEVICE_Start(&cameraDevice);

    CAMERA_RECEIVER_Init(&cameraReceiver, &cameraConfig, NULL, NULL);

    CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, (uint32_t)&g_FlexioCameraFrameBuffer[1][32]);

    CAMERA_RECEIVER_Start(&cameraReceiver);

    USB_DeviceRun(g_UsbDeviceVideoFlexioCamera.deviceHandle);
}

#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTask(void *handle)
{
    while (handle)
    {
        USB_DeviceTaskFn(handle);
    }
}
#endif

void APP_task(void *handle)
{
    USB_DeviceApplicationInit();

#if USB_DEVICE_CONFIG_USE_TASK
    if (g_UsbDeviceVideoFlexioCamera.deviceHandle)
    {
        if (xTaskCreate(USB_DeviceTask,                                /* pointer to the task */
                        "usb device task",                             /* task name for kernel awareness debugging */
                        5000L / sizeof(portSTACK_TYPE),                /* task stack size */
                        g_UsbDeviceVideoFlexioCamera.deviceHandle,     /* optional task startup argument */
                        5U,                                            /* initial priority */
                        &g_UsbDeviceVideoFlexioCamera.deviceTaskHandle /* optional task handle to create */
                        ) != pdPASS)
        {
            usb_echo("usb device task create failed!\r\n");
            return;
        }
    }
#endif

    while (1U)
    {
    }
}

#if defined(__CC_ARM) || (defined(__ARMCC_VERSION)) || defined(__GNUC__)
int main(void)
#else
void main(void)
#endif
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitCameraResource();
    BOARD_InitFlexioResource();

    CLOCK_SetClkOutClock(0x06);
    /* CLOCK_SetFlexio0Clock(0x01);*/
    /* CLOCK_EnableClock(kCLOCK_Flexio0);*/

    if (xTaskCreate(APP_task,                                           /* pointer to the task */
                    "app task",                                         /* task name for kernel awareness debugging */
                    5000L / sizeof(portSTACK_TYPE),                     /* task stack size */
                    &g_UsbDeviceVideoFlexioCamera,                      /* optional task startup argument */
                    4U,                                                 /* initial priority */
                    &g_UsbDeviceVideoFlexioCamera.applicationTaskHandle /* optional task handle to create */
                    ) != pdPASS)
    {
        usb_echo("app task create failed!\r\n");
#if (defined(__CC_ARM) || (defined(__ARMCC_VERSION)) || defined(__GNUC__))
        return 1U;
#else
        return;
#endif
    }

    vTaskStartScheduler();

#if (defined(__CC_ARM) || (defined(__ARMCC_VERSION)) || defined(__GNUC__))
    return 1U;
#endif
}
