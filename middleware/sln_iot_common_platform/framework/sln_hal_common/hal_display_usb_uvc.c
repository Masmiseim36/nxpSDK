/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
@brief display over usb using UVC class.

*/

#include <FreeRTOS.h>
#include <event_groups.h>
#include <queue.h>

#include "board.h"

#include "display_support.h"

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_class.h"
#include "usb_device_video.h"

#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#include "fwk_log.h"
#include "fwk_message.h"
#include "fwk_display_manager.h"
#include "hal_display_dev.h"

#if ((defined FSL_FEATURE_SOC_USBPHY_COUNT) && (FSL_FEATURE_SOC_USBPHY_COUNT > 0U))
#include "usb_phy.h"
#endif
#include "hal_event_descriptor_common.h"
#include "virtual_camera.h"
#include "composite.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/
#define CLIP(X) ((X) > 255 ? 255 : (X) < 0 ? 0 : X)

#define DISPLAY_NAME "usb_uvc"

#define RGB565_RED   0xf800
#define RGB565_GREEN 0x07e0
#define RGB565_BLUE  0x001f

uint16_t colors[] = {RGB565_RED, RGB565_GREEN, RGB565_BLUE};
uint16_t *rgb565iamge;

// RGB -> YUV
#define RGB2Y(R, G, B) CLIP(((66 * (R) + 129 * (G) + 25 * (B) + 128) >> 8) + 16)
#define RGB2U(R, G, B) CLIP(((-38 * (R)-74 * (G) + 112 * (B) + 128) >> 8) + 128)
#define RGB2V(R, G, B) CLIP(((112 * (R)-94 * (G)-18 * (B) + 128) >> 8) + 128)

/* LCD input frame buffer is RGB565, converted by PXP. */
#define DISPLAY_USB_BUFFER_COUNT 2

#define DEBUG_UVC 0

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static hal_display_status_t HAL_DisplayDev_UsbUvc_InputNotify(const display_dev_t *dev, void *data);
static hal_display_status_t HAL_DisplayDev_UsbUvc_Init(
    display_dev_t *dev, int width, int height, display_dev_callback_t callback, void *param);
static hal_display_status_t HAL_DisplayDev_UsbUvc_Uninit(const display_dev_t *dev);
static hal_display_status_t HAL_DisplayDev_UsbUvc_Start(const display_dev_t *dev);
static hal_display_status_t HAL_DisplayDev_UsbUvc_Blit(const display_dev_t *dev, void *frame, int width, int height);

static void USB_DeviceVideoPrepareVideoData(void);
static usb_status_t USB_DeviceVideoRequest(class_handle_t handle, uint32_t event, void *param);
static usb_status_t USB_DeviceVideoCallback(class_handle_t handle, uint32_t event, void *param);
static void USB_DeviceVideoApplicationSetDefault(void);
static usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param);

extern usb_device_class_struct_t g_UsbDeviceVideoVirtualCameraConfig;
extern unsigned int Time_Current();

USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static usb_device_video_probe_and_commit_controls_struct_t s_ProbeStruct;
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static usb_device_video_probe_and_commit_controls_struct_t s_CommitStruct;
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static usb_device_video_still_probe_and_commit_controls_struct_t s_StillProbeStruct;
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static usb_device_video_still_probe_and_commit_controls_struct_t s_StillCommitStruct;
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint32_t s_ClassRequestBuffer[(sizeof(usb_device_video_probe_and_commit_controls_struct_t) >> 2U) + 1U];
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_ImageBuffer[HS_STREAM_IN_PACKET_SIZE];
static usb_video_virtual_camera_struct_t s_UsbDeviceVideoVirtualCamera;

usb_device_class_config_struct_t g_UsbDeviceVideoConfig[1] = {{
    USB_DeviceVideoCallback,
    (class_handle_t)NULL,
    &g_UsbDeviceVideoVirtualCameraConfig,
}};

/*TODO Camera needs to be scaled */
AT_NONCACHEABLE_SECTION_ALIGN(
    static uint8_t s_usbBuffer[DISPLAY_USB_BUFFER_COUNT][USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_HEIGHT]
                              [USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_WIDTH *
                               USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_DATA_BITS],
    FRAME_BUFFER_ALIGN);

static SemaphoreHandle_t s_DisplayFull, s_DisplayEmpty;

const static display_dev_operator_t s_DisplayDev_UsbUVCOps = {
    .init        = HAL_DisplayDev_UsbUvc_Init,
    .deinit      = HAL_DisplayDev_UsbUvc_Uninit,
    .start       = HAL_DisplayDev_UsbUvc_Start,
    .blit        = HAL_DisplayDev_UsbUvc_Blit,
    .inputNotify = HAL_DisplayDev_UsbUvc_InputNotify,
};

static display_dev_t s_DisplayDev_UsbUVC = {.id   = 0,
                                            .name = DISPLAY_NAME,
                                            .ops  = &s_DisplayDev_UsbUVCOps,
                                            .cap  = {.width  = USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_WIDTH,
                                                    .height = USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_HEIGHT,
                                                    .pitch  = USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_WIDTH *
                                                             USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_DATA_BITS,
                                                    .left      = 0,
                                                    .top       = 0,
                                                    .right     = USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_WIDTH - 1,
                                                    .bottom    = USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_HEIGHT - 1,
                                                    .rotate    = 0,
                                                    .format    = kPixelFormat_RGB565,
                                                    .srcFormat = kPixelFormat_UYVY1P422_RGB,
                                                    .frameBuffer = NULL,
                                                    .callback    = NULL,
                                                    .param       = NULL}};

static void RGB5652YUV(uint16_t source1, uint16_t source2, uint16_t *dest1, uint16_t *dest2)
{
    uint8_t R, G, B, Y0, Y1, U0, V0, U1, V1;
    R      = ((source1 & RGB565_RED) >> 11) << 3;
    G      = ((source1 & RGB565_GREEN) >> 5) << 2;
    B      = ((source1 & RGB565_BLUE)) << 3;
    Y0     = RGB2Y(R, G, B);
    U0     = RGB2U(R, G, B);
    V0     = RGB2V(R, G, B);
    R      = ((source2 & RGB565_RED) >> 11) << 3;
    G      = ((source2 & RGB565_GREEN) >> 5) << 2;
    B      = ((source2 & RGB565_BLUE)) << 3;
    Y1     = RGB2Y(R, G, B);
    U1     = RGB2U(R, G, B);
    V1     = RGB2V(R, G, B);
    U0     = (U0 + U1) / 2;
    V0     = (V0 + V1) / 2;
    *dest2 = (Y1 << 8) | V0;
    *dest1 = (Y0 << 8) | U0;
}

static void ConvertRGB2YUV(uint16_t *sourceLcdBuffer, uint16_t *destLcdBuffer, int width, int height)
{
    int line, col;

    for (line = 0; line < height; line++)
        for (col = 0; col < width - 1; col = col + 2)
        {
#if DEBUG_UVC
            RGB5652YUV(RGB565_RED, RGB565_RED, (destLcdBuffer + line * width + col),
                       (destLcdBuffer + line * width + col + 1));
#else

            RGB5652YUV(*(sourceLcdBuffer + line * width + col), *(sourceLcdBuffer + line * width + col + 1),
                       (destLcdBuffer + line * width + col), (destLcdBuffer + line * width + col + 1));
#endif
        }
}

/* Prepare next transfer payload */
static void USB_DeviceVideoPrepareVideoData(void)
{
    usb_device_video_mjpeg_payload_header_struct_t *payloadHeader;
    uint32_t maxPacketSize;

    payloadHeader = (usb_device_video_mjpeg_payload_header_struct_t *)&s_UsbDeviceVideoVirtualCamera.imageBuffer[0];
    memset(payloadHeader, 0, sizeof(usb_device_video_mjpeg_payload_header_struct_t));
    payloadHeader->bHeaderLength                = sizeof(usb_device_video_mjpeg_payload_header_struct_t);
    payloadHeader->headerInfoUnion.bmheaderInfo = 0U;
    payloadHeader->headerInfoUnion.headerInfoBits.frameIdentifier = s_UsbDeviceVideoVirtualCamera.currentFrameId;
    s_UsbDeviceVideoVirtualCamera.imageBufferLength = sizeof(usb_device_video_mjpeg_payload_header_struct_t);

    if (s_UsbDeviceVideoVirtualCamera.stillImageTransmission)
    {
        payloadHeader->headerInfoUnion.headerInfoBits.stillImage = 1U;
        maxPacketSize =
            USB_LONG_FROM_LITTLE_ENDIAN_DATA(s_UsbDeviceVideoVirtualCamera.stillCommitStruct->dwMaxPayloadTransferSize);
    }
    else
    {
        maxPacketSize =
            USB_LONG_FROM_LITTLE_ENDIAN_DATA(s_UsbDeviceVideoVirtualCamera.commitStruct->dwMaxPayloadTransferSize);
    }

    if (s_UsbDeviceVideoVirtualCamera.waitForNewInterval)
    {
        if (xSemaphoreTakeFromISR(s_DisplayFull, NULL) == pdFALSE)
        {
            return;
        }
        else
        {
            s_UsbDeviceVideoVirtualCamera.imageIndex                 = 0U;
            s_UsbDeviceVideoVirtualCamera.currentTime                = 0U;
            s_UsbDeviceVideoVirtualCamera.waitForNewInterval         = 0U;
            payloadHeader->headerInfoUnion.headerInfoBits.endOfFrame = 1U;
            s_UsbDeviceVideoVirtualCamera.stillImageTransmission     = 0U;
            s_UsbDeviceVideoVirtualCamera.currentFrameId ^= 1U;

            if (USB_DEVICE_VIDEO_STILL_IMAGE_TRIGGER_TRANSMIT_STILL_IMAGE ==
                s_UsbDeviceVideoVirtualCamera.stillImageTriggerControl)
            {
                s_UsbDeviceVideoVirtualCamera.stillImageTriggerControl =
                    USB_DEVICE_VIDEO_STILL_IMAGE_TRIGGER_NORMAL_OPERATION;
                s_UsbDeviceVideoVirtualCamera.stillImageTransmission = 1U;
            }

            return;
        }
    }

    if (s_UsbDeviceVideoVirtualCamera.imageIndex < USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_HEIGHT *
                                                       USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_WIDTH *
                                                       USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_DATA_BITS)
    {
        uint32_t sendLength = USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_HEIGHT *
                                  USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_WIDTH *
                                  USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_DATA_BITS -
                              s_UsbDeviceVideoVirtualCamera.imageIndex;
        maxPacketSize -= sizeof(usb_device_video_mjpeg_payload_header_struct_t);

        if (sendLength > maxPacketSize)
        {
            sendLength = maxPacketSize;
        }

        /* TODO */
        /* If frames are in a not cached area try without memcpy or try to invalidate cache before*/

        memcpy((&s_UsbDeviceVideoVirtualCamera.imageBuffer[0] + sizeof(usb_device_video_mjpeg_payload_header_struct_t)),
               (uint8_t *)((uint8_t *)s_usbBuffer[1] + s_UsbDeviceVideoVirtualCamera.imageIndex), sendLength);
        s_UsbDeviceVideoVirtualCamera.imageBuffer = (uint8_t *)payloadHeader;
        s_UsbDeviceVideoVirtualCamera.imageIndex += sendLength;
        s_UsbDeviceVideoVirtualCamera.imageBufferLength += sendLength;

        if (s_UsbDeviceVideoVirtualCamera.imageIndex >=
            (USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_HEIGHT * USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_WIDTH *
             USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_DATA_BITS))
        {
            // Frame send
            xSemaphoreGiveFromISR(s_DisplayEmpty, NULL);

            // Tell Display manager that the frame was sent
            if (s_DisplayDev_UsbUVC.cap.callback != NULL)
            {
                uint8_t fromISR = __get_IPSR();
                s_DisplayDev_UsbUVC.cap.callback(&s_DisplayDev_UsbUVC, kDisplayEvent_RequestFrame, NULL, fromISR);
            }

            s_UsbDeviceVideoVirtualCamera.waitForNewInterval = 1U;
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

            if ((temp32 >= USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_MIN_INTERVAL) &&
                (temp32 <= USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_MAX_INTERVAL))
            {
                USB_LONG_TO_LITTLE_ENDIAN_DATA(temp32, s_UsbDeviceVideoVirtualCamera.probeStruct->dwFrameInterval);
            }

            temp32 = USB_LONG_FROM_LITTLE_ENDIAN_DATA(probe->dwMaxPayloadTransferSize);

            if ((temp32) && (temp32 < s_UsbDeviceVideoVirtualCamera.currentMaxPacketSize))
            {
                USB_LONG_TO_LITTLE_ENDIAN_DATA(temp32,
                                               s_UsbDeviceVideoVirtualCamera.probeStruct->dwMaxPayloadTransferSize);
            }

            s_UsbDeviceVideoVirtualCamera.probeStruct->bFormatIndex = probe->bFormatIndex;
            s_UsbDeviceVideoVirtualCamera.probeStruct->bFrameIndex  = probe->bFrameIndex;
            break;

        case USB_DEVICE_VIDEO_GET_CUR_VS_PROBE_CONTROL:
            request->buffer = (uint8_t *)s_UsbDeviceVideoVirtualCamera.probeStruct;
            request->length = s_UsbDeviceVideoVirtualCamera.probeLength;
            break;

        case USB_DEVICE_VIDEO_GET_LEN_VS_PROBE_CONTROL:
            request->buffer = &s_UsbDeviceVideoVirtualCamera.probeLength;
            request->length = sizeof(s_UsbDeviceVideoVirtualCamera.probeLength);
            break;

        case USB_DEVICE_VIDEO_GET_INFO_VS_PROBE_CONTROL:
            request->buffer = &s_UsbDeviceVideoVirtualCamera.probeInfo;
            request->length = sizeof(s_UsbDeviceVideoVirtualCamera.probeInfo);
            break;

        /* commit request */
        case USB_DEVICE_VIDEO_SET_CUR_VS_COMMIT_CONTROL:
            if ((request->buffer == NULL) || (request->length == 0U))
            {
                return kStatus_USB_InvalidRequest;
            }

            temp32 = USB_LONG_FROM_LITTLE_ENDIAN_DATA(commit->dwFrameInterval);

            if ((temp32 >= USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_MIN_INTERVAL) &&
                (temp32 <= USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_MAX_INTERVAL))
            {
                USB_LONG_TO_LITTLE_ENDIAN_DATA(temp32, s_UsbDeviceVideoVirtualCamera.commitStruct->dwFrameInterval);
            }

            temp32 = USB_LONG_FROM_LITTLE_ENDIAN_DATA(commit->dwMaxPayloadTransferSize);

            if ((temp32) && (temp32 < s_UsbDeviceVideoVirtualCamera.currentMaxPacketSize))
            {
                USB_LONG_TO_LITTLE_ENDIAN_DATA(temp32,
                                               s_UsbDeviceVideoVirtualCamera.commitStruct->dwMaxPayloadTransferSize);
            }

            s_UsbDeviceVideoVirtualCamera.commitStruct->bFormatIndex = commit->bFormatIndex;
            s_UsbDeviceVideoVirtualCamera.commitStruct->bFrameIndex  = commit->bFrameIndex;
            break;

        case USB_DEVICE_VIDEO_GET_CUR_VS_COMMIT_CONTROL:
            request->buffer = (uint8_t *)s_UsbDeviceVideoVirtualCamera.commitStruct;
            request->length = s_UsbDeviceVideoVirtualCamera.commitLength;
            break;

        case USB_DEVICE_VIDEO_GET_LEN_VS_COMMIT_CONTROL:
            request->buffer = &s_UsbDeviceVideoVirtualCamera.commitLength;
            request->length = sizeof(s_UsbDeviceVideoVirtualCamera.commitLength);
            break;

        case USB_DEVICE_VIDEO_GET_INFO_VS_COMMIT_CONTROL:
            request->buffer = &s_UsbDeviceVideoVirtualCamera.commitInfo;
            request->length = sizeof(s_UsbDeviceVideoVirtualCamera.commitInfo);
            break;

        /* still probe request */
        case USB_DEVICE_VIDEO_SET_CUR_VS_STILL_PROBE_CONTROL:
            if ((request->buffer == NULL) || (request->length == 0U))
            {
                return kStatus_USB_InvalidRequest;
            }

            temp32 = USB_LONG_FROM_LITTLE_ENDIAN_DATA(still_probe->dwMaxPayloadTransferSize);

            if ((temp32) && (temp32 < s_UsbDeviceVideoVirtualCamera.currentMaxPacketSize))
            {
                USB_LONG_TO_LITTLE_ENDIAN_DATA(
                    temp32, s_UsbDeviceVideoVirtualCamera.stillProbeStruct->dwMaxPayloadTransferSize);
            }

            s_UsbDeviceVideoVirtualCamera.stillProbeStruct->bFormatIndex = still_probe->bFormatIndex;
            s_UsbDeviceVideoVirtualCamera.stillProbeStruct->bFrameIndex  = still_probe->bFrameIndex;
            break;

        case USB_DEVICE_VIDEO_GET_CUR_VS_STILL_PROBE_CONTROL:
            request->buffer = (uint8_t *)s_UsbDeviceVideoVirtualCamera.stillProbeStruct;
            request->length = s_UsbDeviceVideoVirtualCamera.stillProbeLength;
            break;

        case USB_DEVICE_VIDEO_GET_LEN_VS_STILL_PROBE_CONTROL:
            request->buffer = &s_UsbDeviceVideoVirtualCamera.stillProbeLength;
            request->length = sizeof(s_UsbDeviceVideoVirtualCamera.stillProbeLength);
            break;

        case USB_DEVICE_VIDEO_GET_INFO_VS_STILL_PROBE_CONTROL:
            request->buffer = &s_UsbDeviceVideoVirtualCamera.stillProbeInfo;
            request->length = sizeof(s_UsbDeviceVideoVirtualCamera.stillProbeInfo);
            break;

        /* still commit request */
        case USB_DEVICE_VIDEO_SET_CUR_VS_STILL_COMMIT_CONTROL:
            if ((request->buffer == NULL) || (request->length == 0U))
            {
                return kStatus_USB_InvalidRequest;
            }

            temp32 = USB_LONG_FROM_LITTLE_ENDIAN_DATA(still_commit->dwMaxPayloadTransferSize);

            if ((temp32) && (temp32 < s_UsbDeviceVideoVirtualCamera.currentMaxPacketSize))
            {
                USB_LONG_TO_LITTLE_ENDIAN_DATA(
                    temp32, s_UsbDeviceVideoVirtualCamera.stillCommitStruct->dwMaxPayloadTransferSize);
            }

            s_UsbDeviceVideoVirtualCamera.stillCommitStruct->bFormatIndex = still_commit->bFormatIndex;
            s_UsbDeviceVideoVirtualCamera.stillCommitStruct->bFrameIndex  = still_commit->bFrameIndex;
            break;

        case USB_DEVICE_VIDEO_GET_CUR_VS_STILL_COMMIT_CONTROL:
            request->buffer = (uint8_t *)s_UsbDeviceVideoVirtualCamera.stillCommitStruct;
            request->length = s_UsbDeviceVideoVirtualCamera.stillCommitLength;
            break;

        case USB_DEVICE_VIDEO_GET_LEN_VS_STILL_COMMIT_CONTROL:
            request->buffer = &s_UsbDeviceVideoVirtualCamera.stillCommitLength;
            request->length = sizeof(s_UsbDeviceVideoVirtualCamera.stillCommitLength);
            break;

        case USB_DEVICE_VIDEO_GET_INFO_VS_STILL_COMMIT_CONTROL:
            request->buffer = &s_UsbDeviceVideoVirtualCamera.stillCommitInfo;
            request->length = sizeof(s_UsbDeviceVideoVirtualCamera.stillCommitInfo);
            break;

        /* still image trigger request */
        case USB_DEVICE_VIDEO_SET_CUR_VS_STILL_IMAGE_TRIGGER_CONTROL:
            s_UsbDeviceVideoVirtualCamera.stillImageTriggerControl = *(request->buffer);
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
            if (s_UsbDeviceVideoVirtualCamera.attach)
            {
                /* Prepare the next stream data */
                USB_DeviceVideoPrepareVideoData();
                error = USB_DeviceVideoSend(
                    s_UsbDeviceVideoVirtualCamera.videoHandle, USB_VIDEO_VIRTUAL_CAMERA_STREAM_ENDPOINT_IN,
                    s_UsbDeviceVideoVirtualCamera.imageBuffer, s_UsbDeviceVideoVirtualCamera.imageBufferLength);
            }

            break;

        case kUSB_DeviceVideoEventClassRequestBuffer:
            if (param && (s_UsbDeviceVideoVirtualCamera.attach))
            {
                /* Get the class-specific OUT buffer */
                usb_device_control_request_struct_t *request = (usb_device_control_request_struct_t *)param;

                if (request->length <= sizeof(usb_device_video_probe_and_commit_controls_struct_t))
                {
                    request->buffer = (uint8_t *)s_UsbDeviceVideoVirtualCamera.classRequestBuffer;
                    error           = kStatus_USB_Success;
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
    s_UsbDeviceVideoVirtualCamera.speed                = USB_SPEED_FULL;
    s_UsbDeviceVideoVirtualCamera.attach               = 0U;
    s_UsbDeviceVideoVirtualCamera.currentMaxPacketSize = HS_STREAM_IN_PACKET_SIZE;
    s_UsbDeviceVideoVirtualCamera.imageBuffer          = s_ImageBuffer;
    s_UsbDeviceVideoVirtualCamera.probeStruct          = &s_ProbeStruct;
    s_UsbDeviceVideoVirtualCamera.commitStruct         = &s_CommitStruct;
    s_UsbDeviceVideoVirtualCamera.stillProbeStruct     = &s_StillProbeStruct;
    s_UsbDeviceVideoVirtualCamera.stillCommitStruct    = &s_StillCommitStruct;
    s_UsbDeviceVideoVirtualCamera.classRequestBuffer   = &s_ClassRequestBuffer[0];
    for (uint8_t i = 0; i < USB_VIDEO_VIRTUAL_CAMERA_INTERFACE_COUNT; i++)
    {
        s_UsbDeviceVideoVirtualCamera.currentInterfaceAlternateSetting[i] = 0;
    }

    s_UsbDeviceVideoVirtualCamera.probeStruct->bFormatIndex = USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FORMAT_INDEX;
    s_UsbDeviceVideoVirtualCamera.probeStruct->bFrameIndex  = USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_INDEX;
    USB_LONG_TO_LITTLE_ENDIAN_DATA(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_DEFAULT_INTERVAL,
                                   s_UsbDeviceVideoVirtualCamera.probeStruct->dwFrameInterval);
    USB_LONG_TO_LITTLE_ENDIAN_DATA(s_UsbDeviceVideoVirtualCamera.currentMaxPacketSize,
                                   s_UsbDeviceVideoVirtualCamera.probeStruct->dwMaxPayloadTransferSize);
    USB_LONG_TO_LITTLE_ENDIAN_DATA(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_MAX_INTERVAL,
                                   s_UsbDeviceVideoVirtualCamera.probeStruct->dwMaxVideoFrameSize);

    s_UsbDeviceVideoVirtualCamera.commitStruct->bFormatIndex = USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FORMAT_INDEX;
    s_UsbDeviceVideoVirtualCamera.commitStruct->bFrameIndex  = USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_INDEX;
    USB_LONG_TO_LITTLE_ENDIAN_DATA(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_DEFAULT_INTERVAL,
                                   s_UsbDeviceVideoVirtualCamera.commitStruct->dwFrameInterval);
    USB_LONG_TO_LITTLE_ENDIAN_DATA(s_UsbDeviceVideoVirtualCamera.currentMaxPacketSize,
                                   s_UsbDeviceVideoVirtualCamera.commitStruct->dwMaxPayloadTransferSize);
    USB_LONG_TO_LITTLE_ENDIAN_DATA(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_MAX_FRAME_SIZE,
                                   s_UsbDeviceVideoVirtualCamera.commitStruct->dwMaxVideoFrameSize);

    s_UsbDeviceVideoVirtualCamera.probeInfo    = 0x03U;
    s_UsbDeviceVideoVirtualCamera.probeLength  = 26U;
    s_UsbDeviceVideoVirtualCamera.commitInfo   = 0x03U;
    s_UsbDeviceVideoVirtualCamera.commitLength = 26U;

    s_UsbDeviceVideoVirtualCamera.stillProbeStruct->bFormatIndex = USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FORMAT_INDEX;
    s_UsbDeviceVideoVirtualCamera.stillProbeStruct->bFrameIndex  = USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_INDEX;
    s_UsbDeviceVideoVirtualCamera.stillProbeStruct->bCompressionIndex = 0x01U;
    USB_LONG_TO_LITTLE_ENDIAN_DATA(s_UsbDeviceVideoVirtualCamera.currentMaxPacketSize,
                                   s_UsbDeviceVideoVirtualCamera.stillProbeStruct->dwMaxPayloadTransferSize);
    USB_LONG_TO_LITTLE_ENDIAN_DATA(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_MAX_FRAME_SIZE,
                                   s_UsbDeviceVideoVirtualCamera.stillProbeStruct->dwMaxVideoFrameSize);

    s_UsbDeviceVideoVirtualCamera.stillCommitStruct->bFormatIndex = USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FORMAT_INDEX;
    s_UsbDeviceVideoVirtualCamera.stillCommitStruct->bFrameIndex  = USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_INDEX;
    s_UsbDeviceVideoVirtualCamera.stillCommitStruct->bCompressionIndex = 0x01U;
    USB_LONG_TO_LITTLE_ENDIAN_DATA(s_UsbDeviceVideoVirtualCamera.currentMaxPacketSize,
                                   s_UsbDeviceVideoVirtualCamera.stillCommitStruct->dwMaxPayloadTransferSize);
    USB_LONG_TO_LITTLE_ENDIAN_DATA(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_MAX_FRAME_SIZE,
                                   s_UsbDeviceVideoVirtualCamera.stillCommitStruct->dwMaxVideoFrameSize);

    s_UsbDeviceVideoVirtualCamera.stillProbeInfo    = 0x03U;
    s_UsbDeviceVideoVirtualCamera.stillProbeLength  = sizeof(s_StillProbeStruct);
    s_UsbDeviceVideoVirtualCamera.stillCommitInfo   = 0x03U;
    s_UsbDeviceVideoVirtualCamera.stillCommitLength = sizeof(s_StillCommitStruct);

    s_UsbDeviceVideoVirtualCamera.currentTime                            = 0U;
    s_UsbDeviceVideoVirtualCamera.currentFrameId                         = 0U;
    s_UsbDeviceVideoVirtualCamera.currentStreamInterfaceAlternateSetting = 0U;
    s_UsbDeviceVideoVirtualCamera.imageBufferLength                      = 0U;
    s_UsbDeviceVideoVirtualCamera.imageIndex                             = 0U;
    s_UsbDeviceVideoVirtualCamera.waitForNewInterval                     = 0U;
    s_UsbDeviceVideoVirtualCamera.stillImageTransmission                 = 0U;
    s_UsbDeviceVideoVirtualCamera.stillImageTriggerControl = USB_DEVICE_VIDEO_STILL_IMAGE_TRIGGER_NORMAL_OPERATION;
}

/* TODO Move this function outside */

static usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_Success;
    uint8_t *temp8     = (uint8_t *)param;
    uint16_t *temp16   = (uint16_t *)param;

    switch (event)
    {
        case kUSB_DeviceClassInit:
        {
            s_UsbDeviceVideoVirtualCamera.deviceHandle = handle;
            s_UsbDeviceVideoVirtualCamera.videoHandle  = (class_handle_t)param;
        }
        break;
        case kUSB_DeviceEventBusReset:
        {
            /* The device BUS reset signal detected */
            USB_DeviceVideoApplicationSetDefault();
            s_UsbDeviceVideoVirtualCamera.attach               = 0U;
            s_UsbDeviceVideoVirtualCamera.currentConfiguration = 0U;
#if (defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)) || \
    (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))

            /* Get USB speed to configure the device, including max packet size and interval of the endpoints. */
            if (kStatus_USB_Success == USB_DeviceClassGetSpeed(CONTROLLER_ID, &s_UsbDeviceVideoVirtualCamera.speed))

                if (USB_SPEED_HIGH == s_UsbDeviceVideoVirtualCamera.speed)
                {
                    s_UsbDeviceVideoVirtualCamera.currentMaxPacketSize = HS_STREAM_IN_PACKET_SIZE;
                }
                else
                {
                    s_UsbDeviceVideoVirtualCamera.currentMaxPacketSize = FS_STREAM_IN_PACKET_SIZE;
                }
            USB_LONG_TO_LITTLE_ENDIAN_DATA(s_UsbDeviceVideoVirtualCamera.currentMaxPacketSize,
                                           s_UsbDeviceVideoVirtualCamera.probeStruct->dwMaxPayloadTransferSize);
            USB_LONG_TO_LITTLE_ENDIAN_DATA(s_UsbDeviceVideoVirtualCamera.currentMaxPacketSize,
                                           s_UsbDeviceVideoVirtualCamera.commitStruct->dwMaxPayloadTransferSize);
            USB_LONG_TO_LITTLE_ENDIAN_DATA(s_UsbDeviceVideoVirtualCamera.currentMaxPacketSize,
                                           s_UsbDeviceVideoVirtualCamera.stillProbeStruct->dwMaxPayloadTransferSize);
            USB_LONG_TO_LITTLE_ENDIAN_DATA(s_UsbDeviceVideoVirtualCamera.currentMaxPacketSize,
                                           s_UsbDeviceVideoVirtualCamera.stillCommitStruct->dwMaxPayloadTransferSize);

#endif
        }
        break;

        case kUSB_DeviceEventSetConfiguration:
            if (0 == (*temp8))
            {
                s_UsbDeviceVideoVirtualCamera.attach               = 0U;
                s_UsbDeviceVideoVirtualCamera.currentConfiguration = 0U;
            }
            else if (USB_VIDEO_VIRTUAL_CAMERA_CONFIGURE_INDEX == (*temp8))
            {
                /* Set the configuration request */
                s_UsbDeviceVideoVirtualCamera.attach               = 1U;
                s_UsbDeviceVideoVirtualCamera.currentConfiguration = *temp8;
            }
            else
            {
                error = kStatus_USB_InvalidRequest;
            }

            break;

        case kUSB_DeviceEventSetInterface:
            if ((s_UsbDeviceVideoVirtualCamera.attach) && param)
            {
                /* Set alternateSetting of the interface request */
                uint8_t interface        = (uint8_t)((*temp16 & 0xFF00U) >> 0x08U);
                uint8_t alternateSetting = (uint8_t)(*temp16 & 0x00FFU);

                uint8_t first_video_interface = MIN(USB_VIDEO_VIRTUAL_CAMERA_CONTROL_INTERFACE_INDEX,
                                                    USB_VIDEO_VIRTUAL_CAMERA_STREAM_INTERFACE_INDEX);
                uint8_t last_video_interface  = MAX(USB_VIDEO_VIRTUAL_CAMERA_CONTROL_INTERFACE_INDEX,
                                                   USB_VIDEO_VIRTUAL_CAMERA_STREAM_INTERFACE_INDEX);
                if (interface >= first_video_interface && interface <= last_video_interface)
                {
                    interface -= first_video_interface;

                    if (s_UsbDeviceVideoVirtualCamera.currentInterfaceAlternateSetting[interface] != alternateSetting)
                    {
                        if (!s_UsbDeviceVideoVirtualCamera.currentInterfaceAlternateSetting[interface])
                        {
                            USB_DeviceVideoPrepareVideoData();
                            error = USB_DeviceSendRequest(s_UsbDeviceVideoVirtualCamera.deviceHandle,
                                                          USB_VIDEO_VIRTUAL_CAMERA_STREAM_ENDPOINT_IN,
                                                          s_UsbDeviceVideoVirtualCamera.imageBuffer,
                                                          s_UsbDeviceVideoVirtualCamera.imageBufferLength);
                        }

                        s_UsbDeviceVideoVirtualCamera.currentInterfaceAlternateSetting[interface] = alternateSetting;
                    }
                }
            }

            break;

        case kUSB_DeviceEventGetConfiguration:
            error = kStatus_USB_Success;
            break;

        case kUSB_DeviceEventGetInterface:

            error = kStatus_USB_Success;
            break;

        case kUSB_DeviceEventGetDeviceDescriptor:

            error = kStatus_USB_Success;
            break;

        case kUSB_DeviceEventGetConfigurationDescriptor:
            error = kStatus_USB_Success;
            break;

        case kUSB_DeviceEventGetStringDescriptor:
            error = kStatus_USB_Success;
            break;

        default:
            break;
    }

    return error;
}

static hal_display_status_t HAL_DisplayDev_UsbUvc_Init(
    display_dev_t *dev, int width, int height, display_dev_callback_t callback, void *param)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    dev->cap.width           = width;
    dev->cap.height          = height;
    dev->cap.frameBuffer     = (void *)s_usbBuffer[0];
    dev->cap.callback        = callback;

    s_DisplayFull  = xSemaphoreCreateCounting(1, 0);
    s_DisplayEmpty = xSemaphoreCreateCounting(1, 1);

    if (USB_DeviceApplicationInit())
    {
        LOGE("USB device video virtual camera failed");
        ret = kStatus_HAL_DisplayError;
    }

    return ret;
}

static hal_display_status_t HAL_DisplayDev_UsbUvc_Uninit(const display_dev_t *dev)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    USB_DeviceIsrDisable();
    USB_DeviceClassDeinit(CONTROLLER_ID);
    LOGD("USB device video uninit");
    return ret;
}

static hal_display_status_t HAL_DisplayDev_UsbUvc_Start(const display_dev_t *dev)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    USB_DeviceIsrEnable();
    Time_Current();
    vTaskDelay(200);
    USB_DeviceRun(s_UsbDeviceVideoVirtualCamera.deviceHandle);
    return ret;
}

static hal_display_status_t HAL_DisplayDev_UsbUvc_Blit(const display_dev_t *dev, void *frame, int width, int height)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;

    LOGI("USB new frame");
#if DEBUG_UVC
    static int frames30 = 0;

    if (frames30 == 0)
    {
        LOGD("Time before 30 frames sent = %d", Time_Current());
    }

    frames30++;
#endif

    /* TODO */
    /* Use the callback attached to make it a non blocking function and return immediately */

    if (xSemaphoreTake(s_DisplayEmpty, 0) == pdFALSE)
    {
        ret = kStatus_HAL_DisplayTxBusy;
    }
    else
    {
        ConvertRGB2YUV((uint16_t *)frame, (uint16_t *)s_usbBuffer[1], width, height);
        xSemaphoreGive(s_DisplayFull);
        LOGI("Time after conversion = %d", Time_Current());
        ret = kStatus_HAL_DisplayNonBlocking;
    }

#if DEBUG_UVC

    if (frames30 == 30)
    {
        LOGD("Time after 30 frames sent = %d", Time_Current());
        frames30 = 0;
    }

#endif
    return ret;
}

static hal_display_status_t HAL_DisplayDev_UsbUvc_InputNotify(const display_dev_t *dev, void *data)
{
    hal_display_status_t error           = kStatus_HAL_DisplaySuccess;
    event_base_t eventBase               = *(event_base_t *)data;
    event_status_t event_response_status = kEventStatus_Ok;

    if (eventBase.eventId == kEventID_SetDisplayOutputSource)
    {
        event_common_t event              = *(event_common_t *)data;
        s_DisplayDev_UsbUVC.cap.srcFormat = event.displayOutput.displayOutputSource;
        if (eventBase.respond != NULL)
        {
            eventBase.respond(eventBase.eventId, &event.displayOutput, event_response_status, true);
        }
        LOGI("[HAL_DisplayDev_UsbUvc_InputNotify]: kEventID_SetDisplayOutputSource devID %d, srcFormat %d", dev->id,
             event.displayOutput.displayOutputSource);
    }
    else if (eventBase.eventId == kEventID_GetDisplayOutputSource)
    {
        display_output_event_t display;
        display.displayOutputSource = s_DisplayDev_UsbUVC.cap.srcFormat;
        if (eventBase.respond != NULL)
        {
            eventBase.respond(eventBase.eventId, &display, event_response_status, true);
        }
        LOGI("[HAL_DisplayDev_UsbUvc_InputNotify]: kEventID_GetDisplayOutputSource devID %d, srcFormat %d", dev->id,
             display.displayOutputSource);
    }

    return error;
}

int HAL_DisplayDev_UsbUvc_Register()
{
    int ret = 0;
    LOGD("display_dev_usb_register");
    s_UsbDeviceVideoVirtualCamera.videoHandle = NULL;
    USB_RegisterUVC();
    USB_CompositeClassRegister(USB_DeviceCallback, USB_DeviceVideoCallback, &g_UsbDeviceVideoVirtualCameraConfig);

    ret = FWK_DisplayManager_DeviceRegister(&s_DisplayDev_UsbUVC);
    return ret;
}
