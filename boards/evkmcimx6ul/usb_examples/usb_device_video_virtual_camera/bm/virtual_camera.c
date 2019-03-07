/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
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
 * o Neither the name of the copyright holder nor the names of its
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

#include "virtual_camera.h"

#include "video_data.h"

#include "fsl_device_registers.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"

#include <stdio.h>
#include <stdlib.h>
#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
#include "fsl_sysmpu.h"
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
#include "usb_phy.h"
#endif

#include "pin_mux.h"
#include "fsl_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* USB clock source and frequency*/
#define USB_HS_PHY_CLK_SRC kCLOCK_Usbphy480M
#define USB_HS_PHY_CLK_FREQ 480000000U
#define USB_HS_CLK_SRC kCLOCK_Usb480M
#define USB_HS_CLK_FREQ 480000000U
#define BOARD_XTAL0_CLK_HZ 0

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
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

extern usb_device_class_struct_t g_UsbDeviceVideoVirtualCameraConfig;

USB_DATA_ALIGNMENT uint8_t s_ImageBuffer[HS_STREAM_IN_PACKET_SIZE];
usb_video_virtual_camera_struct_t g_UsbDeviceVideoVirtualCamera;

usb_device_class_config_struct_t g_UsbDeviceVideoConfig[1] = {{
    USB_DeviceVideoCallback, (class_handle_t)NULL, &g_UsbDeviceVideoVirtualCameraConfig,
}};

usb_device_class_config_list_struct_t g_UsbDeviceVideoConfigList = {
    g_UsbDeviceVideoConfig, USB_DeviceCallback, 1U,
};

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Prepare next transfer payload */
static void USB_DeviceVideoPrepareVideoData(void)
{
    usb_device_video_mjpeg_payload_header_struct_t *payloadHeader;
    uint32_t maxPacketSize;
    uint32_t temp32dwFrameInterval;

    g_UsbDeviceVideoVirtualCamera.currentTime += 10000U;

    payloadHeader = (usb_device_video_mjpeg_payload_header_struct_t *)&g_UsbDeviceVideoVirtualCamera.imageBuffer[0];

    payloadHeader->bHeaderLength = sizeof(usb_device_video_mjpeg_payload_header_struct_t);
    payloadHeader->headerInfoUnion.bmheaderInfo = 0U;
    payloadHeader->headerInfoUnion.headerInfoBits.frameIdentifier = g_UsbDeviceVideoVirtualCamera.currentFrameId;
    g_UsbDeviceVideoVirtualCamera.imageBufferLength = sizeof(usb_device_video_mjpeg_payload_header_struct_t);

    if (g_UsbDeviceVideoVirtualCamera.stillImageTransmission)
    {
        payloadHeader->headerInfoUnion.headerInfoBits.stillImage = 1U;
        maxPacketSize = g_UsbDeviceVideoVirtualCamera.stillCommitStruct.dwMaxPayloadTransferSize;
    }
    else
    {
        maxPacketSize = g_UsbDeviceVideoVirtualCamera.commitStruct.dwMaxPayloadTransferSize;
    }

    if (g_UsbDeviceVideoVirtualCamera.waitForNewInterval)
    {
        USB_ASSIGN_VALUE_ADDRESS_LONG_BY_BYTE(temp32dwFrameInterval,
                                              g_UsbDeviceVideoVirtualCamera.commitStruct.dwFrameInterval);
        if (g_UsbDeviceVideoVirtualCamera.currentTime < temp32dwFrameInterval)
        {
            return;
        }
        else
        {
            g_UsbDeviceVideoVirtualCamera.currentTime = 0U;
            g_UsbDeviceVideoVirtualCamera.waitForNewInterval = 0U;
            payloadHeader->headerInfoUnion.headerInfoBits.endOfFrame = 1U;
            g_UsbDeviceVideoVirtualCamera.stillImageTransmission = 0U;
            g_UsbDeviceVideoVirtualCamera.currentFrameId ^= 1U;
            if (USB_DEVICE_VIDEO_STILL_IMAGE_TRIGGER_TRANSMIT_STILL_IMAGE ==
                g_UsbDeviceVideoVirtualCamera.stillImageTriggerControl)
            {
                g_UsbDeviceVideoVirtualCamera.stillImageTriggerControl =
                    USB_DEVICE_VIDEO_STILL_IMAGE_TRIGGER_NORMAL_OPERATION;
                g_UsbDeviceVideoVirtualCamera.stillImageTransmission = 1U;
            }
            return;
        }
    }

    for (; g_UsbDeviceVideoVirtualCamera.imageBufferLength < maxPacketSize;
         g_UsbDeviceVideoVirtualCamera.imageBufferLength++)
    {
        g_UsbDeviceVideoVirtualCamera.imageBuffer[g_UsbDeviceVideoVirtualCamera.imageBufferLength] =
            g_UsbDeviceVideoMjpegData[g_UsbDeviceVideoVirtualCamera.imageIndex];
        g_UsbDeviceVideoVirtualCamera.imageIndex++;

        if ((0xFFU == g_UsbDeviceVideoMjpegData[g_UsbDeviceVideoVirtualCamera.imageIndex - 2]) &&
            (0xD9U == g_UsbDeviceVideoMjpegData[g_UsbDeviceVideoVirtualCamera.imageIndex - 1U]))
        {
            USB_ASSIGN_VALUE_ADDRESS_LONG_BY_BYTE(temp32dwFrameInterval,
                                                  g_UsbDeviceVideoVirtualCamera.commitStruct.dwFrameInterval);
            if (g_UsbDeviceVideoVirtualCamera.imageIndex >= g_UsbDeviceVideoMjpegLength)
            {
                g_UsbDeviceVideoVirtualCamera.imageIndex = 0U;
            }
            if (g_UsbDeviceVideoVirtualCamera.currentTime < temp32dwFrameInterval)
            {
                g_UsbDeviceVideoVirtualCamera.waitForNewInterval = 1U;
            }
            else
            {
                g_UsbDeviceVideoVirtualCamera.currentTime = 0U;
                payloadHeader->headerInfoUnion.headerInfoBits.endOfFrame = 1U;
                g_UsbDeviceVideoVirtualCamera.stillImageTransmission = 0U;
                g_UsbDeviceVideoVirtualCamera.currentFrameId ^= 1U;
                if (USB_DEVICE_VIDEO_STILL_IMAGE_TRIGGER_TRANSMIT_STILL_IMAGE ==
                    g_UsbDeviceVideoVirtualCamera.stillImageTriggerControl)
                {
                    g_UsbDeviceVideoVirtualCamera.stillImageTriggerControl =
                        USB_DEVICE_VIDEO_STILL_IMAGE_TRIGGER_NORMAL_OPERATION;
                    g_UsbDeviceVideoVirtualCamera.stillImageTransmission = 1U;
                }
            }
            g_UsbDeviceVideoVirtualCamera.imageBufferLength++;
            break;
        }
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
    uint32_t temp32dwMaxPayloadTransferSize;
    usb_status_t error = kStatus_USB_Success;

    switch (event)
    {
        /* probe request */
        case USB_DEVICE_VIDEO_SET_CUR_VS_PROBE_CONTROL:
            if ((request->buffer == NULL) || (request->length == 0U))
            {
                return kStatus_USB_InvalidRequest;
            }
            USB_ASSIGN_VALUE_ADDRESS_LONG_BY_BYTE(temp32dwMaxPayloadTransferSize, probe->dwMaxPayloadTransferSize);

            if ((probe->dwFrameInterval >= USB_VIDEO_VIRTUAL_CAMERA_MJPEG_FRAME_MIN_INTERVAL) &&
                (probe->dwFrameInterval <= USB_VIDEO_VIRTUAL_CAMERA_MJPEG_FRAME_MAX_INTERVAL))
            {
                g_UsbDeviceVideoVirtualCamera.probeStruct.dwFrameInterval = probe->dwFrameInterval;
            }

            if ((temp32dwMaxPayloadTransferSize) &&
                (temp32dwMaxPayloadTransferSize < g_UsbDeviceVideoVirtualCamera.currentMaxPacketSize))
            {
                g_UsbDeviceVideoVirtualCamera.probeStruct.dwMaxPayloadTransferSize = probe->dwMaxPayloadTransferSize;
            }
            g_UsbDeviceVideoVirtualCamera.probeStruct.bFormatIndex = probe->bFormatIndex;
            g_UsbDeviceVideoVirtualCamera.probeStruct.bFrameIndex = probe->bFrameIndex;
            break;
        case USB_DEVICE_VIDEO_GET_CUR_VS_PROBE_CONTROL:
            request->buffer = (uint8_t *)&g_UsbDeviceVideoVirtualCamera.probeStruct;
            request->length = g_UsbDeviceVideoVirtualCamera.probeLength;
            break;
        case USB_DEVICE_VIDEO_GET_LEN_VS_PROBE_CONTROL:
            request->buffer = &g_UsbDeviceVideoVirtualCamera.probeLength;
            request->length = sizeof(g_UsbDeviceVideoVirtualCamera.probeLength);
            break;
        case USB_DEVICE_VIDEO_GET_INFO_VS_PROBE_CONTROL:
            request->buffer = &g_UsbDeviceVideoVirtualCamera.probeInfo;
            request->length = sizeof(g_UsbDeviceVideoVirtualCamera.probeInfo);
            break;
        /* commit request */
        case USB_DEVICE_VIDEO_SET_CUR_VS_COMMIT_CONTROL:
            if ((request->buffer == NULL) || (request->length == 0U))
            {
                return kStatus_USB_InvalidRequest;
            }
            if ((commit->dwFrameInterval >= USB_VIDEO_VIRTUAL_CAMERA_MJPEG_FRAME_MIN_INTERVAL) &&
                (commit->dwFrameInterval <= USB_VIDEO_VIRTUAL_CAMERA_MJPEG_FRAME_MAX_INTERVAL))
            {
                USB_ASSIGN_VALUE_ADDRESS_LONG_BY_BYTE(g_UsbDeviceVideoVirtualCamera.commitStruct.dwFrameInterval,
                                                      commit->dwFrameInterval);
            }
            USB_ASSIGN_VALUE_ADDRESS_LONG_BY_BYTE(temp32dwMaxPayloadTransferSize, commit->dwMaxPayloadTransferSize);

            if ((temp32dwMaxPayloadTransferSize) &&
                (temp32dwMaxPayloadTransferSize < g_UsbDeviceVideoVirtualCamera.currentMaxPacketSize))
            {
                g_UsbDeviceVideoVirtualCamera.commitStruct.dwMaxPayloadTransferSize = commit->dwMaxPayloadTransferSize;
            }
            g_UsbDeviceVideoVirtualCamera.commitStruct.bFormatIndex = commit->bFormatIndex;
            g_UsbDeviceVideoVirtualCamera.commitStruct.bFrameIndex = commit->bFrameIndex;
            break;
        case USB_DEVICE_VIDEO_GET_CUR_VS_COMMIT_CONTROL:
            request->buffer = (uint8_t *)&g_UsbDeviceVideoVirtualCamera.commitStruct;
            request->length = g_UsbDeviceVideoVirtualCamera.commitLength;
            break;
        case USB_DEVICE_VIDEO_GET_LEN_VS_COMMIT_CONTROL:
            request->buffer = &g_UsbDeviceVideoVirtualCamera.commitLength;
            request->length = sizeof(g_UsbDeviceVideoVirtualCamera.commitLength);
            break;
        case USB_DEVICE_VIDEO_GET_INFO_VS_COMMIT_CONTROL:
            request->buffer = &g_UsbDeviceVideoVirtualCamera.commitInfo;
            request->length = sizeof(g_UsbDeviceVideoVirtualCamera.commitInfo);
            break;
        /* still probe request */
        case USB_DEVICE_VIDEO_SET_CUR_VS_STILL_PROBE_CONTROL:
            if ((request->buffer == NULL) || (request->length == 0U))
            {
                return kStatus_USB_InvalidRequest;
            }
            if ((still_probe->dwMaxPayloadTransferSize) &&
                (still_probe->dwMaxPayloadTransferSize < g_UsbDeviceVideoVirtualCamera.currentMaxPacketSize))
            {
                g_UsbDeviceVideoVirtualCamera.stillProbeStruct.dwMaxPayloadTransferSize =
                    still_probe->dwMaxPayloadTransferSize;
            }

            g_UsbDeviceVideoVirtualCamera.stillProbeStruct.bFormatIndex = still_probe->bFormatIndex;
            g_UsbDeviceVideoVirtualCamera.stillProbeStruct.bFrameIndex = still_probe->bFrameIndex;
            break;
        case USB_DEVICE_VIDEO_GET_CUR_VS_STILL_PROBE_CONTROL:
            request->buffer = (uint8_t *)&g_UsbDeviceVideoVirtualCamera.stillProbeStruct;
            request->length = g_UsbDeviceVideoVirtualCamera.stillProbeLength;
            break;
        case USB_DEVICE_VIDEO_GET_LEN_VS_STILL_PROBE_CONTROL:
            request->buffer = &g_UsbDeviceVideoVirtualCamera.stillProbeLength;
            request->length = sizeof(g_UsbDeviceVideoVirtualCamera.stillProbeLength);
            break;
        case USB_DEVICE_VIDEO_GET_INFO_VS_STILL_PROBE_CONTROL:
            request->buffer = &g_UsbDeviceVideoVirtualCamera.stillProbeInfo;
            request->length = sizeof(g_UsbDeviceVideoVirtualCamera.stillProbeInfo);
            break;
        /* still commit request */
        case USB_DEVICE_VIDEO_SET_CUR_VS_STILL_COMMIT_CONTROL:
            if ((request->buffer == NULL) || (request->length == 0U))
            {
                return kStatus_USB_InvalidRequest;
            }
            if ((still_commit->dwMaxPayloadTransferSize) &&
                (still_commit->dwMaxPayloadTransferSize < g_UsbDeviceVideoVirtualCamera.currentMaxPacketSize))
            {
                g_UsbDeviceVideoVirtualCamera.stillCommitStruct.dwMaxPayloadTransferSize =
                    still_commit->dwMaxPayloadTransferSize;
            }

            g_UsbDeviceVideoVirtualCamera.stillCommitStruct.bFormatIndex = still_commit->bFormatIndex;
            g_UsbDeviceVideoVirtualCamera.stillCommitStruct.bFrameIndex = still_commit->bFrameIndex;
            break;
        case USB_DEVICE_VIDEO_GET_CUR_VS_STILL_COMMIT_CONTROL:
            request->buffer = (uint8_t *)&g_UsbDeviceVideoVirtualCamera.stillCommitStruct;
            request->length = g_UsbDeviceVideoVirtualCamera.stillCommitLength;
            break;
        case USB_DEVICE_VIDEO_GET_LEN_VS_STILL_COMMIT_CONTROL:
            request->buffer = &g_UsbDeviceVideoVirtualCamera.stillCommitLength;
            request->length = sizeof(g_UsbDeviceVideoVirtualCamera.stillCommitLength);
            break;
        case USB_DEVICE_VIDEO_GET_INFO_VS_STILL_COMMIT_CONTROL:
            request->buffer = &g_UsbDeviceVideoVirtualCamera.stillCommitInfo;
            request->length = sizeof(g_UsbDeviceVideoVirtualCamera.stillCommitInfo);
            break;
        /* still image trigger request */
        case USB_DEVICE_VIDEO_SET_CUR_VS_STILL_IMAGE_TRIGGER_CONTROL:
            g_UsbDeviceVideoVirtualCamera.stillImageTriggerControl = *(request->buffer);
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
            if (g_UsbDeviceVideoVirtualCamera.attach)
            {
                /* Prepare the next stream data */
                USB_DeviceVideoPrepareVideoData();
                error = USB_DeviceVideoSend(
                    g_UsbDeviceVideoVirtualCamera.videoHandle, USB_VIDEO_VIRTUAL_CAMERA_STREAM_ENDPOINT_IN,
                    g_UsbDeviceVideoVirtualCamera.imageBuffer, g_UsbDeviceVideoVirtualCamera.imageBufferLength);
            }
            break;
        case kUSB_DeviceVideoEventClassRequestBuffer:
            if (param && (g_UsbDeviceVideoVirtualCamera.attach))
            {
                /* Get the class-specific OUT buffer */
                usb_device_control_request_struct_t *request = (usb_device_control_request_struct_t *)param;

                if (request->length <= sizeof(usb_device_video_probe_and_commit_controls_struct_t))
                {
                    request->buffer = (uint8_t *)&g_UsbDeviceVideoVirtualCamera.classRequestBuffer;
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
    g_UsbDeviceVideoVirtualCamera.speed = USB_SPEED_FULL;
    g_UsbDeviceVideoVirtualCamera.attach = 0U;
    g_UsbDeviceVideoVirtualCamera.currentMaxPacketSize = HS_STREAM_IN_PACKET_SIZE;
    g_UsbDeviceVideoVirtualCamera.imageBuffer = s_ImageBuffer;

    g_UsbDeviceVideoVirtualCamera.probeStruct.bFormatIndex = USB_VIDEO_VIRTUAL_CAMERA_MJPEG_FORMAT_INDEX;
    g_UsbDeviceVideoVirtualCamera.probeStruct.bFrameIndex = USB_VIDEO_VIRTUAL_CAMERA_MJPEG_FRAME_INDEX;
    g_UsbDeviceVideoVirtualCamera.probeStruct.dwFrameInterval = USB_VIDEO_VIRTUAL_CAMERA_MJPEG_FRAME_DEFAULT_INTERVAL;
    USB_ASSIGN_VALUE_ADDRESS_LONG_BY_BYTE(g_UsbDeviceVideoVirtualCamera.probeStruct.dwMaxPayloadTransferSize,
                                          g_UsbDeviceVideoVirtualCamera.currentMaxPacketSize);
    USB_ASSIGN_MACRO_VALUE_ADDRESS_LONG_BY_BYTE(g_UsbDeviceVideoVirtualCamera.probeStruct.dwMaxVideoFrameSize,
                                                USB_VIDEO_VIRTUAL_CAMERA_MJPEG_FRAME_MAX_FRAME_SIZE);

    g_UsbDeviceVideoVirtualCamera.commitStruct.bFormatIndex = USB_VIDEO_VIRTUAL_CAMERA_MJPEG_FORMAT_INDEX;
    g_UsbDeviceVideoVirtualCamera.commitStruct.bFrameIndex = USB_VIDEO_VIRTUAL_CAMERA_MJPEG_FRAME_INDEX;
    USB_ASSIGN_MACRO_VALUE_ADDRESS_LONG_BY_BYTE(g_UsbDeviceVideoVirtualCamera.commitStruct.dwFrameInterval,
                                                USB_VIDEO_VIRTUAL_CAMERA_MJPEG_FRAME_DEFAULT_INTERVAL);

    g_UsbDeviceVideoVirtualCamera.commitStruct.dwMaxPayloadTransferSize =
        g_UsbDeviceVideoVirtualCamera.currentMaxPacketSize;
    g_UsbDeviceVideoVirtualCamera.commitStruct.dwMaxVideoFrameSize =
        USB_VIDEO_VIRTUAL_CAMERA_MJPEG_FRAME_MAX_FRAME_SIZE;

    g_UsbDeviceVideoVirtualCamera.probeInfo = 0x03U;
    g_UsbDeviceVideoVirtualCamera.probeLength = 26U;
    g_UsbDeviceVideoVirtualCamera.commitInfo = 0x03U;
    g_UsbDeviceVideoVirtualCamera.commitLength = 26U;

    g_UsbDeviceVideoVirtualCamera.stillProbeStruct.bFormatIndex = USB_VIDEO_VIRTUAL_CAMERA_MJPEG_FORMAT_INDEX;
    g_UsbDeviceVideoVirtualCamera.stillProbeStruct.bFrameIndex = USB_VIDEO_VIRTUAL_CAMERA_MJPEG_FRAME_INDEX;
    g_UsbDeviceVideoVirtualCamera.stillProbeStruct.bCompressionIndex = 0x01U;
    USB_ASSIGN_VALUE_ADDRESS_LONG_BY_BYTE(g_UsbDeviceVideoVirtualCamera.stillProbeStruct.dwMaxPayloadTransferSize,
                                          g_UsbDeviceVideoVirtualCamera.currentMaxPacketSize);
    USB_ASSIGN_MACRO_VALUE_ADDRESS_LONG_BY_BYTE(g_UsbDeviceVideoVirtualCamera.stillProbeStruct.dwMaxVideoFrameSize,
                                                USB_VIDEO_VIRTUAL_CAMERA_MJPEG_FRAME_MAX_FRAME_SIZE);

    g_UsbDeviceVideoVirtualCamera.stillCommitStruct.bFormatIndex = USB_VIDEO_VIRTUAL_CAMERA_MJPEG_FORMAT_INDEX;
    g_UsbDeviceVideoVirtualCamera.stillCommitStruct.bFrameIndex = USB_VIDEO_VIRTUAL_CAMERA_MJPEG_FRAME_INDEX;
    g_UsbDeviceVideoVirtualCamera.stillCommitStruct.bCompressionIndex = 0x01U;
    USB_ASSIGN_VALUE_ADDRESS_LONG_BY_BYTE(g_UsbDeviceVideoVirtualCamera.stillCommitStruct.dwMaxPayloadTransferSize,
                                          g_UsbDeviceVideoVirtualCamera.currentMaxPacketSize);
    USB_ASSIGN_MACRO_VALUE_ADDRESS_LONG_BY_BYTE(g_UsbDeviceVideoVirtualCamera.stillCommitStruct.dwMaxVideoFrameSize,
                                                USB_VIDEO_VIRTUAL_CAMERA_MJPEG_FRAME_MAX_FRAME_SIZE);

    g_UsbDeviceVideoVirtualCamera.stillProbeInfo = 0x03U;
    g_UsbDeviceVideoVirtualCamera.stillProbeLength = sizeof(g_UsbDeviceVideoVirtualCamera.stillProbeStruct);
    g_UsbDeviceVideoVirtualCamera.stillCommitInfo = 0x03U;
    g_UsbDeviceVideoVirtualCamera.stillCommitLength = sizeof(g_UsbDeviceVideoVirtualCamera.stillCommitStruct);

    g_UsbDeviceVideoVirtualCamera.currentTime = 0U;
    g_UsbDeviceVideoVirtualCamera.currentFrameId = 0U;
    g_UsbDeviceVideoVirtualCamera.currentStreamInterfaceAlternateSetting = 0U;
    g_UsbDeviceVideoVirtualCamera.imageBufferLength = 0U;
    g_UsbDeviceVideoVirtualCamera.imageIndex = 0U;
    g_UsbDeviceVideoVirtualCamera.waitForNewInterval = 0U;
    g_UsbDeviceVideoVirtualCamera.stillImageTransmission = 0U;
    g_UsbDeviceVideoVirtualCamera.stillImageTriggerControl = USB_DEVICE_VIDEO_STILL_IMAGE_TRIGGER_NORMAL_OPERATION;
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
#if (defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)) || \
    (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
            /* Get USB speed to configure the device, including max packet size and interval of the endpoints. */
            if (kStatus_USB_Success == USB_DeviceGetStatus(g_UsbDeviceVideoVirtualCamera.deviceHandle,
                                                           kUSB_DeviceStatusSpeed,
                                                           &g_UsbDeviceVideoVirtualCamera.speed))
            {
                USB_DeviceSetSpeed(g_UsbDeviceVideoVirtualCamera.deviceHandle, g_UsbDeviceVideoVirtualCamera.speed);
            }

            if (USB_SPEED_HIGH == g_UsbDeviceVideoVirtualCamera.speed)
            {
                g_UsbDeviceVideoVirtualCamera.currentMaxPacketSize = HS_STREAM_IN_PACKET_SIZE;
            }
#endif
        }
        break;
        case kUSB_DeviceEventSetConfiguration:
            if (USB_VIDEO_VIRTUAL_CAMERA_CONFIGURE_INDEX == (*temp8))
            {
                /* Set the configuration request */
                g_UsbDeviceVideoVirtualCamera.attach = 1U;
                g_UsbDeviceVideoVirtualCamera.currentConfiguration = *temp8;
            }
            break;
        case kUSB_DeviceEventSetInterface:
            if ((g_UsbDeviceVideoVirtualCamera.attach) && param)
            {
                /* Set alternateSetting of the interface request */
                uint8_t interface = (uint8_t)((*temp16 & 0xFF00U) >> 0x08U);
                uint8_t alternateSetting = (uint8_t)(*temp16 & 0x00FFU);

                if (g_UsbDeviceVideoVirtualCamera.currentInterfaceAlternateSetting[interface] != alternateSetting)
                {
                    if (!g_UsbDeviceVideoVirtualCamera.currentInterfaceAlternateSetting[interface])
                    {
                        if (USB_VIDEO_VIRTUAL_CAMERA_STREAM_INTERFACE_INDEX == interface)
                        {
                            USB_DeviceVideoPrepareVideoData();
                            error = USB_DeviceSendRequest(g_UsbDeviceVideoVirtualCamera.deviceHandle,
                                                          USB_VIDEO_VIRTUAL_CAMERA_STREAM_ENDPOINT_IN,
                                                          g_UsbDeviceVideoVirtualCamera.imageBuffer,
                                                          g_UsbDeviceVideoVirtualCamera.imageBufferLength);
                        }
                    }
                    g_UsbDeviceVideoVirtualCamera.currentInterfaceAlternateSetting[interface] = alternateSetting;
                }
            }
            break;
        case kUSB_DeviceEventGetConfiguration:
            if (param)
            {
                /* Get the current configuration request */
                *temp8 = g_UsbDeviceVideoVirtualCamera.currentConfiguration;
                error = kStatus_USB_Success;
            }
            break;
        case kUSB_DeviceEventGetInterface:
            if (param)
            {
                /* Set the alternateSetting of the interface request */
                uint8_t interface = (uint8_t)((*temp16 & 0xFF00U) >> 0x08U);
                if (interface < USB_VIDEO_VIRTUAL_CAMERA_INTERFACE_COUNT)
                {
                    *temp16 =
                        (*temp16 & 0xFF00U) | g_UsbDeviceVideoVirtualCamera.currentInterfaceAlternateSetting[interface];
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
    USB_DeviceEhciIsrFunction(g_UsbDeviceVideoVirtualCamera.deviceHandle);
}
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 1U)
#if defined(FSL_FEATURE_USBHS_EHCI_COUNT) && (FSL_FEATURE_USBHS_EHCI_COUNT > 1U)
void USB1_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(g_UsbDeviceVideoVirtualCamera.deviceHandle);
}
#endif
#endif
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
void USB0_IRQHandler(void)
{
    USB_DeviceKhciIsrFunction(g_UsbDeviceVideoVirtualCamera.deviceHandle);
}
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
void USB0_IRQHandler(void)
{
    USB_DeviceLpcIp3511IsrFunction(g_UsbDeviceVideoVirtualCamera.deviceHandle);
}
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
void USB1_IRQHandler(void)
{
    USB_DeviceLpcIp3511IsrFunction(g_UsbDeviceVideoVirtualCamera.deviceHandle);
}
#endif

static void USB_DeviceApplicationInit(void)
{
    uint8_t irqNumber;
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    uint8_t usbDeviceEhciIrq[] = USBHS_IRQS;
    irqNumber = usbDeviceEhciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL, BOARD_USB_PHY_TXCAL45DP, BOARD_USB_PHY_TXCAL45DM,
    };

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 1U)
    if (CONTROLLER_ID == kUSB_ControllerEhci0)
    {
        CLOCK_EnableUsbhs0PhyPllClock(USB_HS_PHY_CLK_SRC, USB_HS_PHY_CLK_FREQ);
        CLOCK_EnableUsbhs0Clock(USB_HS_CLK_SRC, USB_HS_CLK_FREQ);
    }
    else
    {
        CLOCK_EnableUsbhs1PhyPllClock(USB_HS_PHY_CLK_SRC, USB_HS_PHY_CLK_FREQ);
        CLOCK_EnableUsbhs1Clock(USB_HS_CLK_SRC, USB_HS_CLK_FREQ);
    }
#else
    CLOCK_EnableUsbhs0PhyPllClock(USB_HS_PHY_CLK_SRC, USB_HS_PHY_CLK_FREQ);
    CLOCK_EnableUsbhs0Clock(USB_HS_CLK_SRC, USB_HS_CLK_FREQ);
#endif

    USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ, &phyConfig);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    uint8_t usbDeviceKhciIrq[] = USB_IRQS;
    irqNumber = usbDeviceKhciIrq[CONTROLLER_ID - kUSB_ControllerKhci0];

    SystemCoreClockUpdate();

    CLOCK_EnableUsbfs0Clock(USB_FS_CLK_SRC, USB_FS_CLK_FREQ);
#endif

#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
    uint8_t usbDeviceIP3511Irq[] = USB_IRQS;
    irqNumber = usbDeviceIP3511Irq[CONTROLLER_ID - kUSB_ControllerLpcIp3511Fs0];

    /* enable USB IP clock */
    CLOCK_EnableUsbfs0DeviceClock(USB_FS_CLK_SRC, USB_FS_CLK_FREQ);
#endif

#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
    uint8_t usbDeviceIP3511Irq[] = USBHSD_IRQS;
    irqNumber = usbDeviceIP3511Irq[CONTROLLER_ID - kUSB_ControllerLpcIp3511Hs0];
    /* enable USB IP clock */
    CLOCK_EnableUsbhs0DeviceClock(USB_HS_CLK_SRC, USB_HS_CLK_FREQ);
#endif

#if (((defined(USB_DEVICE_CONFIG_LPCIP3511FS)) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)) || \
     ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)))
#if defined(FSL_FEATURE_USBHSD_USB_RAM) && (FSL_FEATURE_USBHSD_USB_RAM)
    for (int i = 0; i < FSL_FEATURE_USBHSD_USB_RAM; i++)
    {
        ((uint8_t *)FSL_FEATURE_USBHSD_USB_RAM_BASE_ADDRESS)[i] = 0x00U;
    }
#endif
#endif

#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
    SYSMPU_Enable(SYSMPU, 0);
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */

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
        USB_DeviceClassInit(CONTROLLER_ID, &g_UsbDeviceVideoConfigList, &g_UsbDeviceVideoVirtualCamera.deviceHandle))
    {
        usb_echo("USB device video virtual camera failed\r\n");
        return;
    }
    else
    {
        usb_echo("USB device video virtual camera demo\r\n");
        g_UsbDeviceVideoVirtualCamera.videoHandle = g_UsbDeviceVideoConfigList.config->classHandle;
    }

#if defined(__GIC_PRIO_BITS)
    GIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
#else
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
#endif
    EnableIRQ((IRQn_Type)irqNumber);

    USB_DeviceRun(g_UsbDeviceVideoVirtualCamera.deviceHandle);
}

#if defined(__CC_ARM) || defined(__GNUC__)
int main(void)
#else
void main(void)
#endif
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    /*  BOARD_InitMemory();  */ /*USB code doesn't fully support cached in this release */
    BOARD_InitDebugConsole();

    /* Install IRQ Handler */
    SystemInitIrqTable();
    /* Tricky here: As IRQ handler in example doesn't use parameters, just ignore them */
    SystemInstallIrqHandler(USB_OTG1_IRQn, (system_irq_handler_t)(uint32_t)USBHS_IRQHandler, NULL);

    USB_DeviceApplicationInit();

    while (1U)
    {
#if USB_DEVICE_CONFIG_USE_TASK
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
        USB_DeviceEhciTaskFunction(g_UsbDeviceVideoVirtualCamera.deviceHandle);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
        USB_DeviceKhciTaskFunction(g_UsbDeviceVideoVirtualCamera.deviceHandle);
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
        USB_DeviceLpcIp3511TaskFunction(g_UsbDeviceVideoVirtualCamera.deviceHandle);
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
        USB_DeviceLpcIp3511TaskFunction(g_UsbDeviceVideoVirtualCamera.deviceHandle);
#endif
#endif
    }
}
