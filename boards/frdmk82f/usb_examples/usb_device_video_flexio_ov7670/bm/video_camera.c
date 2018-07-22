/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_class.h"
#include "usb_device_video.h"

#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#include "video_camera.h"

#include "flexio_ov7670.h"

#include "fsl_device_registers.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"

#include <stdio.h>
#include <stdlib.h>

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
#include "usb_phy.h"
#endif

#include "pin_mux.h"
#include "fsl_port.h"
#include <stdbool.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void FLEXIO_Ov7670AsynCallback(void);
void BOARD_InitHardware(void);

static void USB_DeviceVideoPrepareVideoData(void);
static usb_status_t USB_DeviceVideoRequest(class_handle_t handle, uint32_t event, void *param);
static usb_status_t USB_DeviceVideoCallback(class_handle_t handle, uint32_t event, void *param);
static void USB_DeviceVideoApplicationSetDefault(void);
static usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param);
static void USB_DeviceApplicationInit(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

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

extern volatile uint8_t g_FlexioCameraFrameBuffer[2][OV7670_FRAME_BYTES + 32];

/*******************************************************************************
 * Code
 ******************************************************************************/

void PORTB_IRQHandler(void)
{
    /* Clear the interrupt flag for PTB2 */
    PORT_ClearPinsInterruptFlags(PORTB, 1U << BOARD_FLEXIO_VSYNC_PIN_INDEX);
    FLEXIO_Ov7670AsynCallback();
}

/* Prepare next transfer payload */
static void USB_DeviceVideoPrepareVideoData(void)
{
    usb_device_video_mjpeg_payload_header_struct_t *payloadHeader;
    uint32_t maxPacketSize;
    uint32_t i;
    uint32_t sendLength;

    g_UsbDeviceVideoFlexioCamera.currentTime += 10000U;

    if (g_UsbDeviceVideoFlexioCamera.imagePosition < OV7670_FRAME_BYTES)
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
        maxPacketSize = g_UsbDeviceVideoFlexioCamera.stillCommitStruct.dwMaxPayloadTransferSize;
    }
    else
    {
        maxPacketSize = g_UsbDeviceVideoFlexioCamera.commitStruct.dwMaxPayloadTransferSize;
    }
    maxPacketSize = maxPacketSize - sizeof(usb_device_video_mjpeg_payload_header_struct_t);
    if (g_UsbDeviceVideoFlexioCamera.waitForNewInterval)
    {
        if (g_UsbDeviceVideoFlexioCamera.currentTime < g_UsbDeviceVideoFlexioCamera.commitStruct.dwFrameInterval)
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

    if (g_UsbDeviceVideoFlexioCamera.imagePosition < OV7670_FRAME_BYTES)
    {
        sendLength = OV7670_FRAME_BYTES - g_UsbDeviceVideoFlexioCamera.imagePosition;
        if (sendLength > maxPacketSize)
        {
            sendLength = maxPacketSize;
        }
        g_UsbDeviceVideoFlexioCamera.imagePosition += sendLength;

        if (g_UsbDeviceVideoFlexioCamera.imagePosition >= OV7670_FRAME_BYTES)
        {
            FLEXIO_Ov7670StartCapture(g_UsbDeviceVideoFlexioCamera.fullBufferIndex + 1);
            g_UsbDeviceVideoFlexioCamera.fullBufferIndex = 1 - g_UsbDeviceVideoFlexioCamera.fullBufferIndex;
            if (g_UsbDeviceVideoFlexioCamera.currentTime < g_UsbDeviceVideoFlexioCamera.commitStruct.dwFrameInterval)
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
    usb_status_t error = kStatus_USB_Success;

    switch (event)
    {
        /* probe request */
        case USB_DEVICE_VIDEO_SET_CUR_VS_PROBE_CONTROL:
            if ((request->buffer == NULL) || (request->length == 0U))
            {
                return kStatus_USB_InvalidRequest;
            }
            if ((probe->dwFrameInterval >= USB_VIDEO_CAMERA_UNCOMPRESSED_FRAME_MIN_INTERVAL) &&
                (probe->dwFrameInterval <= USB_VIDEO_CAMERA_UNCOMPRESSED_FRAME_MAX_INTERVAL))
            {
                g_UsbDeviceVideoFlexioCamera.probeStruct.dwFrameInterval = probe->dwFrameInterval;
            }

            if ((probe->dwMaxPayloadTransferSize) &&
                (probe->dwMaxPayloadTransferSize < g_UsbDeviceVideoFlexioCamera.currentMaxPacketSize))
            {
                g_UsbDeviceVideoFlexioCamera.probeStruct.dwMaxPayloadTransferSize = probe->dwMaxPayloadTransferSize;
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
            if ((commit->dwFrameInterval >= USB_VIDEO_CAMERA_UNCOMPRESSED_FRAME_MIN_INTERVAL) &&
                (commit->dwFrameInterval <= USB_VIDEO_CAMERA_UNCOMPRESSED_FRAME_MAX_INTERVAL))
            {
                g_UsbDeviceVideoFlexioCamera.commitStruct.dwFrameInterval = commit->dwFrameInterval;
            }

            if ((commit->dwMaxPayloadTransferSize) &&
                (commit->dwMaxPayloadTransferSize < g_UsbDeviceVideoFlexioCamera.currentMaxPacketSize))
            {
                g_UsbDeviceVideoFlexioCamera.commitStruct.dwMaxPayloadTransferSize = commit->dwMaxPayloadTransferSize;
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
            if ((still_probe->dwMaxPayloadTransferSize) &&
                (still_probe->dwMaxPayloadTransferSize < g_UsbDeviceVideoFlexioCamera.currentMaxPacketSize))
            {
                g_UsbDeviceVideoFlexioCamera.stillProbeStruct.dwMaxPayloadTransferSize =
                    still_probe->dwMaxPayloadTransferSize;
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
            if ((still_commit->dwMaxPayloadTransferSize) &&
                (still_commit->dwMaxPayloadTransferSize < g_UsbDeviceVideoFlexioCamera.currentMaxPacketSize))
            {
                g_UsbDeviceVideoFlexioCamera.stillCommitStruct.dwMaxPayloadTransferSize =
                    still_commit->dwMaxPayloadTransferSize;
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
    g_UsbDeviceVideoFlexioCamera.probeStruct.dwFrameInterval = USB_VIDEO_CAMERA_UNCOMPRESSED_FRAME_DEFAULT_INTERVAL;
    g_UsbDeviceVideoFlexioCamera.probeStruct.dwMaxPayloadTransferSize =
        g_UsbDeviceVideoFlexioCamera.currentMaxPacketSize;
    g_UsbDeviceVideoFlexioCamera.probeStruct.dwMaxVideoFrameSize = USB_VIDEO_CAMERA_UNCOMPRESSED_FRAME_MAX_FRAME_SIZE;

    g_UsbDeviceVideoFlexioCamera.commitStruct.bFormatIndex = USB_VIDEO_CAMERA_UNCOMPRESSED_FORMAT_INDEX;
    g_UsbDeviceVideoFlexioCamera.commitStruct.bFrameIndex = USB_VIDEO_CAMERA_UNCOMPRESSED_FRAME_INDEX;
    g_UsbDeviceVideoFlexioCamera.commitStruct.dwFrameInterval = USB_VIDEO_CAMERA_UNCOMPRESSED_FRAME_DEFAULT_INTERVAL;
    g_UsbDeviceVideoFlexioCamera.commitStruct.dwMaxPayloadTransferSize =
        g_UsbDeviceVideoFlexioCamera.currentMaxPacketSize;
    g_UsbDeviceVideoFlexioCamera.commitStruct.dwMaxVideoFrameSize = USB_VIDEO_CAMERA_UNCOMPRESSED_FRAME_MAX_FRAME_SIZE;

    g_UsbDeviceVideoFlexioCamera.probeInfo = 0x03U;
    g_UsbDeviceVideoFlexioCamera.probeLength = 26U;
    g_UsbDeviceVideoFlexioCamera.commitInfo = 0x03U;
    g_UsbDeviceVideoFlexioCamera.commitLength = 26U;

    g_UsbDeviceVideoFlexioCamera.stillProbeStruct.bFormatIndex = USB_VIDEO_CAMERA_UNCOMPRESSED_FORMAT_INDEX;
    g_UsbDeviceVideoFlexioCamera.stillProbeStruct.bFrameIndex = USB_VIDEO_CAMERA_UNCOMPRESSED_FRAME_INDEX;
    g_UsbDeviceVideoFlexioCamera.stillProbeStruct.bCompressionIndex = 0x01U;
    g_UsbDeviceVideoFlexioCamera.stillProbeStruct.dwMaxPayloadTransferSize =
        g_UsbDeviceVideoFlexioCamera.currentMaxPacketSize;
    g_UsbDeviceVideoFlexioCamera.stillProbeStruct.dwMaxVideoFrameSize =
        USB_VIDEO_CAMERA_UNCOMPRESSED_FRAME_MAX_FRAME_SIZE;

    g_UsbDeviceVideoFlexioCamera.stillCommitStruct.bFormatIndex = USB_VIDEO_CAMERA_UNCOMPRESSED_FORMAT_INDEX;
    g_UsbDeviceVideoFlexioCamera.stillCommitStruct.bFrameIndex = USB_VIDEO_CAMERA_UNCOMPRESSED_FRAME_INDEX;
    g_UsbDeviceVideoFlexioCamera.stillCommitStruct.bCompressionIndex = 0x01U;
    g_UsbDeviceVideoFlexioCamera.stillCommitStruct.dwMaxPayloadTransferSize =
        g_UsbDeviceVideoFlexioCamera.currentMaxPacketSize;
    g_UsbDeviceVideoFlexioCamera.stillCommitStruct.dwMaxVideoFrameSize =
        USB_VIDEO_CAMERA_UNCOMPRESSED_FRAME_MAX_FRAME_SIZE;

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
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
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
            if (USB_VIDEO_CAMERA_CONFIGURE_INDEX == (*temp8))
            {
                /* Set the configuration request */
                g_UsbDeviceVideoFlexioCamera.attach = 1U;
                g_UsbDeviceVideoFlexioCamera.currentConfiguration = *temp8;
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

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
void USBHS_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(g_UsbDeviceVideoFlexioCamera.deviceHandle);
}
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
void USB0_IRQHandler(void)
{
    USB_DeviceKhciIsrFunction(g_UsbDeviceVideoFlexioCamera.deviceHandle);
}
#endif

static void USB_DeviceApplicationInit(void)
{
    uint8_t irqNumber;
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    uint8_t usbDeviceEhciIrq[] = USBHS_IRQS;
    irqNumber = usbDeviceEhciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];

    CLOCK_EnableUsbhs0Clock(kCLOCK_UsbSrcPll0, CLOCK_GetFreq(kCLOCK_PllFllSelClk));
    USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    uint8_t usbDeviceKhciIrq[] = USB_IRQS;
    irqNumber = usbDeviceKhciIrq[CONTROLLER_ID - kUSB_ControllerKhci0];

    SystemCoreClockUpdate();

#if ((defined FSL_FEATURE_USB_KHCI_IRC48M_MODULE_CLOCK_ENABLED) && (FSL_FEATURE_USB_KHCI_IRC48M_MODULE_CLOCK_ENABLED))
    CLOCK_EnableUsbfs0Clock(kCLOCK_UsbSrcIrc48M, 48000000U);
#else
    CLOCK_EnableUsbfs0Clock(kCLOCK_UsbSrcPll0, CLOCK_GetFreq(kCLOCK_PllFllSelClk));
#endif /* FSL_FEATURE_USB_KHCI_IRC48M_MODULE_CLOCK_ENABLED */
#endif
#if (defined(FSL_FEATURE_SOC_MPU_COUNT) && (FSL_FEATURE_SOC_MPU_COUNT > 0U))
    MPU->CESR = 0;
#endif /* FSL_FEATURE_SOC_MPU_COUNT */

       /*
        * If the SOC has USB KHCI dedicated RAM, the RAM memory needs to be clear after
        * the KHCI clock is enabled. When the demo uses USB EHCI IP, the USB KHCI dedicated
        * RAM can not be used and the memory can't be accessed.
        */
#if (defined(FSL_FEATURE_USB_KHCI_USB_RAM) && (FSL_FEATURE_USB_KHCI_USB_RAM > 0U))
#if (defined(FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS) && (FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS > 0U))
    for (int i = 0; i < FSL_FEATURE_USB_KHCI_USB_RAM; i++)
    {
        ((uint8_t *)FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS)[i] = 0x00U;
    }
#endif /* FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS */
#endif /* FSL_FEATURE_USB_KHCI_USB_RAM */

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

    NVIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
    NVIC_EnableIRQ((IRQn_Type)irqNumber);

    FLEXIO_Ov7670Init();

    USB_DeviceRun(g_UsbDeviceVideoFlexioCamera.deviceHandle);
}

#if defined(__CC_ARM) || defined(__GNUC__)
int main(void)
#else
void main(void)
#endif
{
    gpio_pin_config_t pinConfig;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetClkOutClock(0x06);
    /* CLOCK_SetFlexio0Clock(0x01);*/
    /* CLOCK_EnableClock(kCLOCK_Flexio0);*/

    /* enable OV7670 Reset output */
    pinConfig.pinDirection = kGPIO_DigitalOutput;
    pinConfig.outputLogic = 1U;

    GPIO_PinInit(GPIOC, 8U, &pinConfig);

    USB_DeviceApplicationInit();

    while (1U)
    {
#if USB_DEVICE_CONFIG_USE_TASK
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
        USB_DeviceEhciTaskFunction(g_UsbDeviceVideoFlexioCamera.deviceHandle);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
        USB_DeviceKhciTaskFunction(g_UsbDeviceVideoFlexioCamera.deviceHandle);
#endif
#endif
    }

}
