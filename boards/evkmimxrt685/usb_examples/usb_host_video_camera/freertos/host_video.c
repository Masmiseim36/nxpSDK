/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "usb_host_config.h"
#include "usb_host.h"
#include "usb_host_video.h"
#include "host_video.h"
#include "fsl_sd.h"
#include "ff.h"
#include "diskio.h"
#include "fsl_sd_disk.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief usb host video command complete callback.
 *
 * This function is used as callback function for completed command.
 *
 * @param param    the host video instance pointer.
 * @param data      data buffer pointer.
 * @param dataLen data length.
 * @param status         transfer result status.
 */
static void USB_HostVideoCommandCallback(void *param, uint8_t *data, uint32_t dataLen, usb_status_t status);

/*!
 * @brief usb host video control transfer callback.
 *
 * This function is used as callback function for control transfer .
 *
 * @param param    the host video instance pointer.
 * @param data      data buffer pointer.
 * @param dataLen data length.
 * @param status         transfer result status.
 */
static void USB_HostVideoControlCallback(void *param, uint8_t *data, uint32_t dataLen, usb_status_t status);

/*!
 * @brief host video stream iso in transfer callback.
 *
 * This function is used as callback function when call USB_HosVideoStreamRecv .
 *
 * @param param    the host video instance pointer.
 * @param data      data buffer pointer.
 * @param dataLen data length.
 * @param status    transfer result status.
 */
static void USB_HostVideoStreamDataInCallback(void *param, uint8_t *data, uint32_t dataLen, usb_status_t status);

/*******************************************************************************
 * Variables
 ******************************************************************************/
usb_device_handle g_VideoDeviceHandle;
usb_host_video_camera_instance_t g_Video;
/*the stream interface handle , this handle is init in the class init function*/
usb_host_interface_handle g_VideoStreamInterfaceHandle;
/*the control interface handle , this handle is init in the class init function*/
usb_host_interface_handle g_VideoControlInterfaceHandle;
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) usb_host_video_probe_commit_controls_t g_VideoProbe;
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) uint8_t g_StreamBuffer[USB_VIDEO_STREAM_BUFFER_COUNT][1024];
uint32_t *g_PictureBuffer[2];
uint32_t g_PictureBufferDataLength[2] = {0};
volatile uint32_t g_PictureIndex = 0;

const uint8_t g_YUY2FormatGUID[] = {0x59, 0x55, 0x59, 0x32, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71};
const uint8_t g_NV12FormatGUID[] = {0x4E, 0x56, 0x31, 0x32, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71};
const uint8_t g_M420FormatGUID[] = {0x4D, 0x34, 0x32, 0x30, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71};
const uint8_t g_I420FormatGUID[] = {0x49, 0x34, 0x32, 0x30, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71};

/*! @brief SDMMC host detect card configuration */
static const sdmmchost_detect_card_t s_sdCardDetect = {
#ifndef BOARD_SD_DETECT_TYPE
    .cdType = kSDMMCHOST_DetectCardByGpioCD,
#else
    .cdType = BOARD_SD_DETECT_TYPE,
#endif
    .cdTimeOut_ms = (~0U),
};
static FATFS g_fileSystem; /* File system object */
const TCHAR g_DriverNumberBuffer[] = {SDDISK + '0', ':', '/', '\0'};
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief get proper endpoint maxpacksize and the subordinate interface alternate.
*/
static uint8_t USB_HostGetProperEndpointInterfaceInfo(usb_host_class_handle classHandle,
                                                      uint16_t expectMaxPacketSize,
                                                      uint8_t *interfaceAlternate,
                                                      uint16_t *unMultipleIsoPacketSize)
{
    usb_host_video_instance_struct_t *videoInstance = (usb_host_video_instance_struct_t *)classHandle;
    usb_host_interface_t *streamInterface;
    streamInterface = (usb_host_interface_t *)videoInstance->streamIntfHandle;
    usb_host_video_descriptor_union_t descriptor;
    uint8_t epCount = 0;
    uint16_t length = 0;
    uint8_t alternate = 0;

    if (NULL == streamInterface)
    {
        return kStatus_USB_InvalidHandle;
    }
    if (NULL == streamInterface->interfaceDesc)
    {
        return kStatus_USB_InvalidParameter;
    }

    descriptor.bufr = streamInterface->interfaceExtension;
    length = 0;
    while (length < streamInterface->interfaceExtensionLength)
    {
        if (descriptor.common->bDescriptorType == USB_DESCRIPTOR_TYPE_INTERFACE)
        {
            break;
        }
        length += descriptor.common->bLength;
        descriptor.bufr += descriptor.common->bLength;
    }

    while (length < streamInterface->interfaceExtensionLength)
    {
        if (descriptor.common->bDescriptorType == USB_DESCRIPTOR_TYPE_INTERFACE)
        {
            alternate = descriptor.interface->bAlternateSetting;
            epCount = descriptor.interface->bNumEndpoints;
            while (epCount)
            {
                if (descriptor.endpoint->bDescriptorType == USB_DESCRIPTOR_TYPE_ENDPOINT)
                {
                    if ((USB_SHORT_FROM_LITTLE_ENDIAN_DATA(descriptor.endpoint->wMaxPacketSize) <=
                         expectMaxPacketSize) &&
                        (*unMultipleIsoPacketSize <
                         USB_SHORT_FROM_LITTLE_ENDIAN_DATA(descriptor.endpoint->wMaxPacketSize)))
                    {
                        *unMultipleIsoPacketSize =
                            USB_SHORT_FROM_LITTLE_ENDIAN_DATA(descriptor.endpoint->wMaxPacketSize);
                        /* save interface alternate for the current proper endpoint */
                        *interfaceAlternate = alternate;
                    }
                    epCount--;
                }
                length += descriptor.common->bLength;
                descriptor.bufr += descriptor.common->bLength;
            }
        }
        else
        {
            length += descriptor.common->bLength;
            descriptor.bufr += descriptor.common->bLength;
        }
    }
    if ((0 == *unMultipleIsoPacketSize) || (0 == *interfaceAlternate))
    {
        return kStatus_USB_Error;
    }
    else
    {
        return kStatus_USB_Success;
    }
}

/*!
 * @brief usb host video command complete callback.
 *
 * This function is used as callback function for completed command.
 *
 * @param param    the host video instance pointer.
 * @param data      data buffer pointer.
 * @param dataLen data length.
 * @param status         transfer result status.
 */
static void USB_HostVideoCommandCallback(void *param, uint8_t *data, uint32_t dataLen, usb_status_t status)
{
    usb_host_video_camera_instance_t *videoInstance = (usb_host_video_camera_instance_t *)param;
    videoInstance->isControlTransferring = 0;
}

/*!
 * @brief usb host video control transfer callback.
 *
 * This function is used as callback function for control transfer .
 *
 * @param param    the host video instance pointer.
 * @param data      data buffer pointer.
 * @param dataLen data length.
 * @param status         transfer result status.
 */
static void USB_HostVideoControlCallback(void *param, uint8_t *data, uint32_t dataLen, usb_status_t status)
{
    usb_host_video_camera_instance_t *videoInstance = (usb_host_video_camera_instance_t *)param;
    if (status != kStatus_USB_Success)
    {
        usb_echo("data transfer error = %d , status \r\n");
        return;
    }

    if (videoInstance->runState == kUSB_HostVideoRunIdle)
    {
        if (videoInstance->runWaitState == kUSB_HostVideoRunSetControlInterface)
        {
            videoInstance->runState = kUSB_HostVideoRunSetControlInterfaceDone;
        }
        else if (videoInstance->runWaitState == kUSB_HostVideoRunWaitSetStreamInterface)
        {
            videoInstance->runState = kUSB_HostVideoRunSetInterfaceDone;
        }
        else if (videoInstance->runWaitState == kUSB_HostVideoRunWaitGetSetProbeCommit)
        {
            videoInstance->runState = kUSB_HostVideoRunGetSetProbeCommitDone;
        }
    }
}

static void USB_HostVideoSdcardBufferState(usb_host_video_camera_instance_t *videoAppInstance,
                                           uint8_t index,
                                           uint8_t state)
{
    OSA_SR_ALLOC();
    OSA_ENTER_CRITICAL();
    videoAppInstance->pictureBufferState[index] = state;
    OSA_EXIT_CRITICAL();
}

/*!
 * @brief host video stream iso in transfer callback.
 *
 * This function is used as callback function when call USB_HosVideoStreamRecv .
 *
 * @param param    the host video instance pointer.
 * @param data      data buffer pointer.
 * @param dataLen data length.
 * @param status    transfer result status.
 */
static void USB_HostVideoStreamDataInCallback(void *param, uint8_t *data, uint32_t dataLen, usb_status_t status)
{
    static uint8_t g_DiscardFrame = 0;
    usb_host_video_camera_instance_t *videoAppInstance = (usb_host_video_camera_instance_t *)param;
    uint32_t headLength;
    uint32_t dataSize;
    uint8_t endOfFrame;
    OSA_SR_ALLOC();

    if (videoAppInstance->devState != kStatus_DEV_Attached)
    {
        return;
    }

    endOfFrame = ((usb_host_video_payload_header_t *)g_StreamBuffer[videoAppInstance->streamBufferIndex])
                     ->HeaderInfo.bitMap.end_of_frame;
    headLength =
        ((usb_host_video_payload_header_t *)g_StreamBuffer[videoAppInstance->streamBufferIndex])->bHeaderLength;
    if (dataLen > 0)
    {
        if (dataLen > 11)
        {
            dataSize = dataLen - headLength;
            if (dataSize)
            {
                if ((videoAppInstance->pictureBufferState[videoAppInstance->pictureBufferIndex] &&
                     (!g_DiscardFrame))) /* two picture buffers are not available, now discard the receiving picture */
                {
                    g_DiscardFrame = 1;
                }
                else if (!g_DiscardFrame)
                {
                    if (dataSize > (videoAppInstance->videoCameraPictureBufferSize -
                                    g_PictureBufferDataLength[videoAppInstance->pictureBufferIndex]))
                    {
                        dataSize = (videoAppInstance->videoCameraPictureBufferSize -
                                    g_PictureBufferDataLength[videoAppInstance->pictureBufferIndex]);
                    }
                    /* copy data to picture buffer */
                    memcpy((void *)(((uint8_t *)&g_PictureBuffer[videoAppInstance->pictureBufferIndex][0]) +
                                    g_PictureBufferDataLength[videoAppInstance->pictureBufferIndex]),
                           (void *)(((uint8_t *)&g_StreamBuffer[videoAppInstance->streamBufferIndex][0]) + headLength),
                           dataSize);
                    OSA_ENTER_CRITICAL();
                    g_PictureBufferDataLength[videoAppInstance->pictureBufferIndex] += dataSize;
                    OSA_EXIT_CRITICAL();
                }
                else
                {
                }
            }
            else
            {
            }

            if (g_DiscardFrame) /* this picture is discarded */
            {
                if (endOfFrame) /* the last frame of one picture */
                {
                    if (g_PictureBufferDataLength[videoAppInstance->pictureBufferIndex] != 0)
                    {
                        videoAppInstance->discardPictureCount++;
                    }
                    g_DiscardFrame = 0;
                }
            }

            else if (endOfFrame) /* can save this picture and get the last frame */
            {
                if (g_PictureBufferDataLength[videoAppInstance->pictureBufferIndex] != 0)
                {
                    USB_HostVideoSdcardBufferState(videoAppInstance, videoAppInstance->pictureBufferIndex, 1);
                    /* switch to another buffer to save picture frame */
                    videoAppInstance->pictureBufferIndex = 1 - videoAppInstance->pictureBufferIndex;
                }
                else
                {
                }
            }
        }
    }
    USB_HosVideoStreamRecv(videoAppInstance->classHandle,
                           (uint8_t *)&g_StreamBuffer[videoAppInstance->streamBufferIndex][0],
                           videoAppInstance->unMultipleIsoMaxPacketSize,
                           USB_HostVideoStreamDataInCallback, videoAppInstance);
    videoAppInstance->streamBufferIndex++;
    if (videoAppInstance->streamBufferIndex == USB_VIDEO_STREAM_BUFFER_COUNT)
    {
        videoAppInstance->streamBufferIndex = 0;
    }
}

/*!
 * @brief write picture into sd card.
 */
static void USB_HostVideoWriteSDCard(void *param)
{
    usb_host_video_camera_instance_t *videoAppInstance = (usb_host_video_camera_instance_t *)param;
    static uint8_t s_FileOpened = 0;
    static FIL fileObj;
    char fileName[32];
    FRESULT fileStatus;
    uint32_t writeSize = 0;
    OSA_SR_ALLOC();

    if (videoAppInstance->pictureBufferState[videoAppInstance->sdcardWriteBufferIndex])
    {
        if (!s_FileOpened)
        {
            if (g_PictureIndex > USB_VIDEO_PICTURE_COUNT) /* finished pictures, start overlay from the first pic */
            {
                g_PictureIndex = 1;
            }
            if (videoAppInstance->cameraDeviceFormatType == USB_HOST_DESC_SUBTYPE_VS_FORMAT_MJPEG)
            {
                sprintf(fileName, "%spic%ld.jpg", &g_DriverNumberBuffer[0], g_PictureIndex);
            }
            else if (videoAppInstance->cameraDeviceFormatType == USB_HOST_DESC_SUBTYPE_VS_FORMAT_UNCOMPRESSED)
            {
                sprintf(fileName, "%spic%ld.yuv", &g_DriverNumberBuffer[0], g_PictureIndex);
            }
            else
            {
            }

            fileStatus = f_open(&fileObj, _T(fileName), FA_WRITE | FA_CREATE_ALWAYS);
            if (FR_OK != fileStatus)
            {
                usb_echo("sdcard operate fail\r\n");
                return;
            }
            s_FileOpened = 1;
        }

        fileStatus = f_write(&fileObj, (uint8_t *)&g_PictureBuffer[videoAppInstance->sdcardWriteBufferIndex][0],
                             g_PictureBufferDataLength[videoAppInstance->sdcardWriteBufferIndex], (UINT *)&writeSize);
        if ((fileStatus != FR_OK) || (g_PictureBufferDataLength[videoAppInstance->sdcardWriteBufferIndex] != writeSize))
        {
            usb_echo("sdcard operate fail\r\n");
        }

        OSA_ENTER_CRITICAL();
        USB_HostVideoSdcardBufferState(videoAppInstance, videoAppInstance->sdcardWriteBufferIndex, 0);
        g_PictureBufferDataLength[videoAppInstance->sdcardWriteBufferIndex] = 0;
        OSA_EXIT_CRITICAL();

        /* one frame picture finihsed and sync it */
        s_FileOpened = 0;
        fileStatus = f_sync(&fileObj);
        if (FR_OK != fileStatus)
        {
            usb_echo("file sync fail\r\n");
            return;
        }
        g_PictureIndex++;

        videoAppInstance->sdcardWriteBufferIndex = 1 - videoAppInstance->sdcardWriteBufferIndex;
    }
}

static usb_status_t USB_HostVideoAppSdcardWaitCardInsert(void)
{
#if defined(__GIC_PRIO_BITS)
    GIC_SetPriority(SD_HOST_IRQ, (USB_HOST_INTERRUPT_PRIORITY - 1U));
#else
    NVIC_SetPriority(SD_HOST_IRQ, (USB_HOST_INTERRUPT_PRIORITY - 1U));
#endif
    /* Save host information. */
    g_sd.host.base = SD_HOST_BASEADDR;
    g_sd.host.sourceClock_Hz = SD_HOST_CLK_FREQ;
    /* card detect type */
    g_sd.usrParam.cd = &s_sdCardDetect;
    g_sd.usrParam.pwr = NULL;
    /* SD host init function */
    if (SD_HostInit(&g_sd) != kStatus_Success)
    {
        return kStatus_USB_Error;
    }
    /* power off card */
    SD_PowerOffCard(g_sd.host.base, g_sd.usrParam.pwr);
    /* wait card insert */
    if (SD_WaitCardDetectStatus(SD_HOST_BASEADDR, &s_sdCardDetect, true) == kStatus_Success)
    {
        /* power on the card */
        SD_PowerOnCard(g_sd.host.base, g_sd.usrParam.pwr);
    }
    else
    {
        return kStatus_USB_Error;
    }

    return kStatus_USB_Success;
}

/*!
 * @brief host usb video task function.
 *
 * This function implements the host video action, it is used to create task.
 *
 * @param param  the host video instance pointer.
 */
void USB_HostVideoTask(void *param)
{
    static usb_status_t status = kStatus_USB_Success;
    static usb_host_video_probe_commit_controls_t probe[3];
    usb_host_video_stream_payload_frame_common_desc_t *frameDesc = NULL;
    usb_host_video_camera_instance_t *videoAppInstance = (usb_host_video_camera_instance_t *)param;
    uint32_t index, count;
    uint32_t minResolution = 0xFFFFFFFF;
    uint8_t minSolutionFrameIndex = 0xFF;
    uint32_t frameInterval = 0;
    uint32_t resolution = 0;
    static uint32_t step = 0;
    uint32_t speed;
    uint8_t i = 0;

    /* device state changes */
    if (videoAppInstance->devState != videoAppInstance->prevState)
    {
        videoAppInstance->prevState = videoAppInstance->devState;
        switch (videoAppInstance->devState)
        {
            case kStatus_DEV_Idle:
                break;

            case kStatus_DEV_Attached:
                videoAppInstance->runState = kUSB_HostVideoRunSetControlInterface;
                videoAppInstance->isControlTransferring = 0;
                videoAppInstance->isStreamTransferring = 0;
                videoAppInstance->streamBufferIndex = 0;
                videoAppInstance->pictureBufferIndex = 0;
                videoAppInstance->sdcardWriteBufferIndex = 0;
                videoAppInstance->discardPictureCount = 0;
                videoAppInstance->pictureBufferState[0] = 0;
                videoAppInstance->pictureBufferState[1] = 0;
                g_PictureIndex = 1; /* the first picture index is 1 */
                USB_HostVideoInit(videoAppInstance->deviceHandle, &videoAppInstance->classHandle);
                usb_echo("USB video attached\r\n");
                break;

            case kStatus_DEV_Detached:
                videoAppInstance->devState = kStatus_DEV_Idle;
                videoAppInstance->runState = kUSB_HostVideoRunIdle;
                /* free the two picture buffer */
                vPortFree((void *)&g_PictureBuffer[0][0]);
                vPortFree((void *)&g_PictureBuffer[1][0]);
                USB_HostVideoDeinit(videoAppInstance->deviceHandle, videoAppInstance->classHandle);
                videoAppInstance->classHandle = NULL;
                usb_echo("USB video detached\r\n");
                if (g_PictureIndex > 0)
                {
                    usb_echo("the last saved picture index is %d\r\n", g_PictureIndex - 1);
                }
                usb_echo("the total discarded picture count is %d\r\n", videoAppInstance->discardPictureCount);
                break;

            default:
                break;
        }
    }

    /* run state */
    switch (videoAppInstance->runState)
    {
        case kUSB_HostVideoRunIdle:
            break;

        case kUSB_HostVideoRunSetControlInterface:
            videoAppInstance->runWaitState = kUSB_HostVideoRunSetControlInterface;
            videoAppInstance->runState = kUSB_HostVideoRunIdle;
            if (USB_HostVideoControlSetInterface(videoAppInstance->classHandle, videoAppInstance->controlIntfHandle, 0,
                                                 USB_HostVideoControlCallback, &g_Video) != kStatus_USB_Success)
            {
                usb_echo("set interface error\r\n");
            }
            break;
        case kUSB_HostVideoRunSetControlInterfaceDone:
            videoAppInstance->runWaitState = kUSB_HostVideoRunWaitSetStreamInterface;
            videoAppInstance->runState = kUSB_HostVideoRunIdle;
            videoAppInstance->currentStreamInterfaceAlternate = 0;
            if (USB_HostVideoStreamSetInterface(videoAppInstance->classHandle, videoAppInstance->streamIntfHandle,
                                                videoAppInstance->currentStreamInterfaceAlternate,
                                                USB_HostVideoControlCallback, &g_Video) != kStatus_USB_Success)
            {
                usb_echo("set interface error\r\n");
            }
            break;
        case kUSB_HostVideoRunSetInterfaceDone:
            videoAppInstance->runState = kUSB_HostVideoRunFindOptimalSetting;
            break;
        case kUSB_HostVideoRunFindOptimalSetting:
            /* Firstly get MJPEG format descriptor */
            status = USB_HostVideoStreamGetFormatDescriptor(videoAppInstance->classHandle,
                                                            USB_HOST_DESC_SUBTYPE_VS_FORMAT_MJPEG,
                                                            (void *)&videoAppInstance->videoStreamFormatDescriptor);
            if (status == kStatus_USB_InvalidHandle)
            {
                usb_echo("videoAppInstance->classHandle is invalid\r\n");
            }
            else if (status == kStatus_USB_Error)
            {
                /* the camera device doesn't support MJPEG format, try to get UNCOMPRESSED format */
                status = USB_HostVideoStreamGetFormatDescriptor(videoAppInstance->classHandle,
                                                                USB_HOST_DESC_SUBTYPE_VS_FORMAT_UNCOMPRESSED,
                                                                (void *)&videoAppInstance->videoStreamFormatDescriptor);
                if (status == kStatus_USB_InvalidHandle)
                {
                    usb_echo("videoAppInstance->classHandle is invalid\r\n");
                }
                else if (status == kStatus_USB_Error)
                {
                    usb_echo(" host can't support this format camera device\r\n");
                    videoAppInstance->runState = kUSB_HostVideoRunIdle;
                }
                else
                {
                }
            }
            else
            {
            }

            /* Successfully get MJPEG or UNCOMPRESSED format descriptor */
            if (status == kStatus_USB_Success)
            {
                count = videoAppInstance->videoStreamFormatDescriptor->bNumFrameDescriptors;
                videoAppInstance->cameraDeviceFormatType =
                    videoAppInstance->videoStreamFormatDescriptor->bDescriptorSubtype;
                if (videoAppInstance->cameraDeviceFormatType ==
                    USB_HOST_DESC_SUBTYPE_VS_FORMAT_MJPEG) /* camera device supports mjpeg format */
                {
                    videoAppInstance->videoStreamMjpegFormatDescriptor =
                        (usb_host_video_stream_payload_mjpeg_format_desc_t *)
                            videoAppInstance->videoStreamFormatDescriptor;
                }
                else if (videoAppInstance->cameraDeviceFormatType ==
                         USB_HOST_DESC_SUBTYPE_VS_FORMAT_UNCOMPRESSED) /* camera device supports uncompressed format */
                {
                    videoAppInstance->videoStreamUncompressedFormatDescriptor =
                        (usb_host_video_stream_payload_uncompressed_format_desc_t *)
                            videoAppInstance->videoStreamFormatDescriptor;
                }
                /* Choose a minimum resolution video stream frame descriptor */
                for (index = 1; index <= count; index++)
                {
                    /* Get the subordinate frame descriptor */
                    if (videoAppInstance->cameraDeviceFormatType == USB_HOST_DESC_SUBTYPE_VS_FORMAT_MJPEG)
                    {
                        status = USB_HostVideoStreamGetFrameDescriptor(
                            videoAppInstance->classHandle, videoAppInstance->videoStreamFormatDescriptor,
                            USB_HOST_DESC_SUBTYPE_VS_FRAME_MJPEG, index, (void *)&frameDesc);
                    }
                    else if (videoAppInstance->cameraDeviceFormatType == USB_HOST_DESC_SUBTYPE_VS_FORMAT_UNCOMPRESSED)
                    {
                        status = USB_HostVideoStreamGetFrameDescriptor(
                            videoAppInstance->classHandle, videoAppInstance->videoStreamFormatDescriptor,
                            USB_HOST_DESC_SUBTYPE_VS_FRAME_UNCOMPRESSED, index, (void *)&frameDesc);
                    }

                    if ((kStatus_USB_Success == status) && (NULL != frameDesc))
                    {
                        resolution = ((uint32_t)(USB_SHORT_FROM_LITTLE_ENDIAN_ADDRESS(frameDesc->wHeight))) *
                                     ((uint32_t)(USB_SHORT_FROM_LITTLE_ENDIAN_ADDRESS(frameDesc->wWitd)));
                        if (minResolution > resolution)
                        {
                            minResolution = resolution;
                            minSolutionFrameIndex = index;
                            if (videoAppInstance->cameraDeviceFormatType == USB_HOST_DESC_SUBTYPE_VS_FORMAT_MJPEG)
                            {
                                videoAppInstance->videoStreamMjpegFrameDescriptor =
                                    (usb_host_video_stream_payload_mjpeg_frame_desc_t *)frameDesc;
                                videoAppInstance->videoStreamUncompressedFrameDescriptor = NULL;
                            }
                            else if (videoAppInstance->cameraDeviceFormatType ==
                                     USB_HOST_DESC_SUBTYPE_VS_FORMAT_UNCOMPRESSED)
                            {
                                videoAppInstance->videoStreamUncompressedFrameDescriptor =
                                    (usb_host_video_stream_payload_uncompressed_frame_desc_t *)frameDesc;
                                videoAppInstance->videoStreamMjpegFrameDescriptor = NULL;
                            }
                        }
                        else
                        {
                        }
                    }
                }
                if (minSolutionFrameIndex != 0xFF)
                {
                    videoAppInstance->runState = kUSB_HostVideoRunGetSetProbeCommit;
                    step = 0;
                }
                else
                {
                    status = kStatus_USB_Error;
                    videoAppInstance->runState = kUSB_HostVideoRunIdle;
                }
            }
            break;
        case kUSB_HostVideoRunGetSetProbeCommit:
            switch (step)
            {
                case 0:
                    if (videoAppInstance->isControlTransferring == 0)
                    {
                        videoAppInstance->isControlTransferring = 1;
                        status = USB_HostVideoGetProbe(videoAppInstance->classHandle, USB_HOST_VIDEO_GET_CUR,
                                                       (void *)&probe[0], USB_HostVideoCommandCallback, &g_Video);
                        step++;
                    }
                    break;
                case 1:
                    if (videoAppInstance->isControlTransferring == 0)
                    {
                        videoAppInstance->isControlTransferring = 1;
                        status = USB_HostVideoGetProbe(videoAppInstance->classHandle, USB_HOST_VIDEO_GET_MAX,
                                                       (void *)&probe[1], USB_HostVideoCommandCallback, &g_Video);
                        step++;
                    }
                    break;
                case 2:
                    if (videoAppInstance->isControlTransferring == 0)
                    {
                        videoAppInstance->isControlTransferring = 1;
                        status = USB_HostVideoGetProbe(videoAppInstance->classHandle, USB_HOST_VIDEO_GET_MIN,
                                                       (void *)&probe[2], USB_HostVideoCommandCallback, &g_Video);
                        step++;
                    }
                    break;
                case 3:
                    if (videoAppInstance->isControlTransferring == 0)
                    {
                        videoAppInstance->isControlTransferring = 1;
                        if (videoAppInstance->cameraDeviceFormatType == USB_HOST_DESC_SUBTYPE_VS_FORMAT_MJPEG)
                        {
                            /* choose the maximum frame interval */
                            if (videoAppInstance->videoStreamMjpegFrameDescriptor->bFrameIntervalType > 0)
                            {
                                frameInterval = USB_LONG_FROM_LITTLE_ENDIAN_ADDRESS(
                                        ((uint8_t *)&(videoAppInstance->videoStreamMjpegFrameDescriptor->dwMinFrameInterval[0]) +
                                         (videoAppInstance->videoStreamMjpegFrameDescriptor->bFrameIntervalType - 1) * 4));
                            }
                            else
                            {
                                frameInterval = USB_LONG_FROM_LITTLE_ENDIAN_ADDRESS(
                                        ((uint8_t *)&(videoAppInstance->videoStreamMjpegFrameDescriptor->dwMinFrameInterval[0]) + 4));
                            }
                            USB_LONG_TO_LITTLE_ENDIAN_DATA(frameInterval, g_VideoProbe.dwFrameInterval);
                            g_VideoProbe.bFormatIndex =
                                videoAppInstance->videoStreamMjpegFormatDescriptor->bFormatIndex;
                            g_VideoProbe.bFrameIndex = videoAppInstance->videoStreamMjpegFrameDescriptor->bFrameIndex;
                        }
                        else if (videoAppInstance->cameraDeviceFormatType ==
                                 USB_HOST_DESC_SUBTYPE_VS_FORMAT_UNCOMPRESSED)
                        {
                            /* choose the maximum frame interval */                            
                            if (videoAppInstance->videoStreamUncompressedFrameDescriptor->bFrameIntervalType > 0)
                            {
                                frameInterval = USB_LONG_FROM_LITTLE_ENDIAN_ADDRESS(
                                        ((uint8_t *)&(videoAppInstance->videoStreamUncompressedFrameDescriptor->dwMinFrameInterval[0]) +
                                         (videoAppInstance->videoStreamUncompressedFrameDescriptor->bFrameIntervalType - 1) * 4));
                            }
                            else
                            {
                                frameInterval = USB_LONG_FROM_LITTLE_ENDIAN_ADDRESS(
                                        ((uint8_t *)&(videoAppInstance->videoStreamUncompressedFrameDescriptor->dwMinFrameInterval[0]) + 4));
                            }
                            USB_LONG_TO_LITTLE_ENDIAN_DATA(frameInterval, g_VideoProbe.dwFrameInterval);
                            g_VideoProbe.bFormatIndex =
                                videoAppInstance->videoStreamUncompressedFormatDescriptor->bFormatIndex;
                            g_VideoProbe.bFrameIndex =
                                videoAppInstance->videoStreamUncompressedFrameDescriptor->bFrameIndex;
                        }
                        videoAppInstance->isControlTransferring = 1;
                        status = USB_HostVideoSetProbe(videoAppInstance->classHandle, USB_HOST_VIDEO_SET_CUR,
                                                       (void *)&g_VideoProbe, USB_HostVideoCommandCallback, &g_Video);
                        step++;
                    }
                    break;
                case 4:
                case 5:
                case 6:
                    if (videoAppInstance->isControlTransferring == 0)
                    {
                        videoAppInstance->isControlTransferring = 1;
                        if (step == 4)
                        {
                            status = USB_HostVideoGetProbe(videoAppInstance->classHandle, USB_HOST_VIDEO_GET_CUR,
                                                           (void *)&probe[0], USB_HostVideoCommandCallback, &g_Video);
                        }
                        else if (step == 5)
                        {
                            status = USB_HostVideoGetProbe(videoAppInstance->classHandle, USB_HOST_VIDEO_GET_MAX,
                                                           (void *)&probe[1], USB_HostVideoCommandCallback, &g_Video);
                        }
                        else
                        {
                            status = USB_HostVideoGetProbe(videoAppInstance->classHandle, USB_HOST_VIDEO_GET_MIN,
                                                           (void *)&probe[2], USB_HostVideoCommandCallback, &g_Video);
                        }
                        step++;
                    }
                    break;
                case 7:
                    if (videoAppInstance->isControlTransferring == 0)
                    {
                        videoAppInstance->isControlTransferring = 1;
                        memcpy((void *)&g_VideoProbe, (void *)&probe[0], 22);
                        status = USB_HostVideoSetProbe(videoAppInstance->classHandle, USB_HOST_VIDEO_SET_CUR,
                                                       (void *)&g_VideoProbe, USB_HostVideoCommandCallback, &g_Video);
                        step++;
                    }
                    break;
                case 8:
                    if (videoAppInstance->isControlTransferring == 0)
                    {
                        videoAppInstance->isControlTransferring = 1;
                        status = USB_HostVideoGetProbe(videoAppInstance->classHandle, USB_HOST_VIDEO_GET_CUR,
                                                       (void *)&probe[0], USB_HostVideoCommandCallback, &g_Video);
                        step++;
                    }
                    break;
                case 9:
                    if (videoAppInstance->isControlTransferring == 0)
                    {
                        videoAppInstance->isControlTransferring = 1;
                        memcpy((void *)&g_VideoProbe, (void *)&probe[0], 26);
                        status = USB_HostVideoSetProbe(videoAppInstance->classHandle, USB_HOST_VIDEO_SET_CUR,
                                                       (void *)&g_VideoProbe, USB_HostVideoCommandCallback, &g_Video);
                        step++;
                    }
                    break;
                case 10:
                    if (videoAppInstance->isControlTransferring == 0)
                    {
                        videoAppInstance->isControlTransferring = 1;
                        status = USB_HostVideoGetProbe(videoAppInstance->classHandle, USB_HOST_VIDEO_GET_CUR,
                                                       (void *)&probe[0], USB_HostVideoCommandCallback, &g_Video);
                        step++;
                    }
                    break;
                case 11:
                    if (videoAppInstance->isControlTransferring == 0)
                    {
                        videoAppInstance->isControlTransferring = 1;
                        memcpy((void *)&g_VideoProbe, (void *)&probe[0], 26);
                        status = USB_HostVideoSetCommit(videoAppInstance->classHandle, USB_HOST_VIDEO_SET_CUR,
                                                        (void *)&g_VideoProbe, USB_HostVideoCommandCallback, &g_Video);
                        step++;
                    }
                    break;
                case 12:
                    if (videoAppInstance->isControlTransferring == 0)
                    {
                        USB_HostHelperGetPeripheralInformation(videoAppInstance->deviceHandle, kUSB_HostGetDeviceSpeed,
                                                               &speed);
                        /* According to the device speed mode, choose a proper video stream interface */
                        videoAppInstance->unMultipleIsoMaxPacketSize = 0;
                        if (USB_SPEED_FULL == speed)
                        {
                            /* if device is full speed, the selected endpoint's maxPacketSize is maximum device supports
                             * and is not more than 1023B */
                            USB_HostGetProperEndpointInterfaceInfo(videoAppInstance->classHandle,
                                                                   FULL_SPEED_ISO_MAX_PACKET_SIZE,
                                                                   &videoAppInstance->currentStreamInterfaceAlternate,
                                                                   &videoAppInstance->unMultipleIsoMaxPacketSize);
                        }
                        else if (USB_SPEED_HIGH == speed)
                        {
                            /* if device is high speed, the selected endpoint's maxPacketSize is maximum device supports
                             * and is not more than 1024B */
                            USB_HostGetProperEndpointInterfaceInfo(videoAppInstance->classHandle,
                                                                   HIGH_SPEED_ISO_MAX_PACKET_SIZE_ZERO_ADDITION,
                                                                   &videoAppInstance->currentStreamInterfaceAlternate,
                                                                   &videoAppInstance->unMultipleIsoMaxPacketSize);
                        }
                        /* set interface by the proper alternate setting */
                        videoAppInstance->runWaitState = kUSB_HostVideoRunWaitGetSetProbeCommit;
                        videoAppInstance->runState = kUSB_HostVideoRunIdle;
                        if (USB_HostVideoStreamSetInterface(
                                videoAppInstance->classHandle, videoAppInstance->streamIntfHandle,
                                videoAppInstance->currentStreamInterfaceAlternate, USB_HostVideoControlCallback,
                                &g_Video) != kStatus_USB_Success)
                        {
                            usb_echo("set interface error\r\n");
                        }
                    }
                    break;
                default:
                    break;
            }
            break;
        case kUSB_HostVideoRunGetSetProbeCommitDone:
        {
            OSA_SR_ALLOC();
            
            /* malloc enough buffer for one raw data or mjpeg picture */
            if (videoAppInstance->cameraDeviceFormatType == USB_HOST_DESC_SUBTYPE_VS_FORMAT_MJPEG)
            {
                /* print the camera device format info*/
                usb_echo("Camera setting is: %d(w)*%d(h)@%dfps\r\n", ((uint32_t)(USB_SHORT_FROM_LITTLE_ENDIAN_ADDRESS(videoAppInstance->videoStreamMjpegFrameDescriptor->wWitd))),
                                                              ((uint32_t)(USB_SHORT_FROM_LITTLE_ENDIAN_ADDRESS(videoAppInstance->videoStreamMjpegFrameDescriptor->wHeight))),
                                                              10000000 / USB_LONG_FROM_LITTLE_ENDIAN_ADDRESS(g_VideoProbe.dwFrameInterval));

                videoAppInstance->videoCameraPictureBufferSize =
                    ((uint32_t)(USB_SHORT_FROM_LITTLE_ENDIAN_ADDRESS(
                        videoAppInstance->videoStreamMjpegFrameDescriptor->wHeight))) *
                    ((uint32_t)(USB_SHORT_FROM_LITTLE_ENDIAN_ADDRESS(
                        videoAppInstance->videoStreamMjpegFrameDescriptor->wWitd)));
                videoAppInstance->videoCameraPictureBufferSize = videoAppInstance->videoCameraPictureBufferSize / 100U;
                videoAppInstance->videoCameraPictureBufferSize =
                    videoAppInstance->videoCameraPictureBufferSize * USB_MJPEG_COMPRESSION_RATIO;
                g_PictureBuffer[0] = (uint32_t *)pvPortMalloc(videoAppInstance->videoCameraPictureBufferSize);
                g_PictureBuffer[1] = (uint32_t *)pvPortMalloc(videoAppInstance->videoCameraPictureBufferSize);
                usb_echo("picture format is MJPEG\r\n");
            }
            else if (videoAppInstance->cameraDeviceFormatType == USB_HOST_DESC_SUBTYPE_VS_FORMAT_UNCOMPRESSED)
            {
                usb_echo("Camera setting is: %d(w)*%d(h)@%dfps\r\n", ((uint32_t)(USB_SHORT_FROM_LITTLE_ENDIAN_ADDRESS(videoAppInstance->videoStreamUncompressedFrameDescriptor->wWitd))),
                                                              ((uint32_t)(USB_SHORT_FROM_LITTLE_ENDIAN_ADDRESS(videoAppInstance->videoStreamUncompressedFrameDescriptor->wHeight))),
                                                             10000000 / USB_LONG_FROM_LITTLE_ENDIAN_ADDRESS(g_VideoProbe.dwFrameInterval));
                videoAppInstance->videoCameraPictureBufferSize =
                    ((uint32_t)(videoAppInstance->videoStreamUncompressedFormatDescriptor->bBitsPerPixel / 8U *
                                USB_SHORT_FROM_LITTLE_ENDIAN_ADDRESS(
                                    videoAppInstance->videoStreamUncompressedFrameDescriptor->wHeight))) *
                    ((uint32_t)(USB_SHORT_FROM_LITTLE_ENDIAN_ADDRESS(
                        videoAppInstance->videoStreamUncompressedFrameDescriptor->wWitd)));
                videoAppInstance->videoCameraPictureBufferSize += videoAppInstance->unMultipleIsoMaxPacketSize;
                g_PictureBuffer[0] = (uint32_t *)pvPortMalloc(videoAppInstance->videoCameraPictureBufferSize);
                g_PictureBuffer[1] = (uint32_t *)pvPortMalloc(videoAppInstance->videoCameraPictureBufferSize);

                usb_echo("picture format is ");
                if (memcmp(&videoAppInstance->videoStreamUncompressedFormatDescriptor->guidFormat[0], &g_YUY2FormatGUID[0], 16) ==0)
                {
                    usb_echo("YUY2\r\n");
                }
                else if (memcmp(&videoAppInstance->videoStreamUncompressedFormatDescriptor->guidFormat[0], &g_NV12FormatGUID[0], 16) ==0)
                {
                    usb_echo("NV12\r\n");
                }
                else if (memcmp(&videoAppInstance->videoStreamUncompressedFormatDescriptor->guidFormat[0], &g_M420FormatGUID[0], 16) ==0)
                {
                    usb_echo("M420\r\n");
                }
                else if (memcmp(&videoAppInstance->videoStreamUncompressedFormatDescriptor->guidFormat[0], &g_I420FormatGUID[0], 16) ==0)
                {
                    usb_echo("I420\r\n");
                }
                else
                {
                    /* directly print GUID*/
                    for (uint8_t index = 0; index < 16; ++index)
                    {
                        usb_echo("%x ", videoAppInstance->videoStreamUncompressedFormatDescriptor->guidFormat[index]);
                    }
                    usb_echo("\r\n");
                }
            }
            if ((g_PictureBuffer[0] == NULL) || (g_PictureBuffer[1] == NULL))
            {
                usb_echo("picture buffer malloc failed\r\n");
                return;
            }

            /* delay to make sure the device camera is ready */
            uint32_t delayForCameraResolutionReady = 100000000;
            while (delayForCameraResolutionReady--)
            {
                __ASM("nop");
            }
            usb_echo("Start getting %d pictures from device camera and write them into sdcard\r\n",
                     USB_VIDEO_PICTURE_COUNT);
            usb_echo("usb host firstly choose mjepg format, if device can't support, will choose raw data format\r\n");
            usb_echo("if finish %d pictures, overlay from the first picture\r\n", USB_VIDEO_PICTURE_COUNT);
            OSA_ENTER_CRITICAL();
            for (i = 0; i < USB_VIDEO_STREAM_BUFFER_COUNT; i++)
            {
                USB_HosVideoStreamRecv(videoAppInstance->classHandle, (uint8_t *)&g_StreamBuffer[i][0],
                                       videoAppInstance->unMultipleIsoMaxPacketSize, USB_HostVideoStreamDataInCallback, &g_Video);
            }
            OSA_EXIT_CRITICAL();
            videoAppInstance->runState = kUSB_HostVideoStart;
            break;
        }
        case kUSB_HostVideoStart:
            USB_HostVideoWriteSDCard(param);
            break;
        default:
            break;
    }
}

usb_status_t USB_HostVideoEvent(usb_device_handle deviceHandle,
                                usb_host_configuration_handle configurationHandle,
                                uint32_t eventCode)
{
    usb_status_t status = kStatus_USB_Success;
    uint8_t id;
    usb_host_configuration_t *configuration;
    uint8_t interface_index;
    usb_host_interface_t *hostInterface;
    uint32_t info_value;

    switch (eventCode)
    {
        case kUSB_HostEventAttach:
            /* judge whether is configurationHandle supported */
            configuration = (usb_host_configuration_t *)configurationHandle;
            g_VideoDeviceHandle = NULL;
            g_VideoControlInterfaceHandle = NULL;
            g_VideoStreamInterfaceHandle = NULL;

            for (interface_index = 0U; interface_index < configuration->interfaceCount; ++interface_index)
            {
                hostInterface = &configuration->interfaceList[interface_index];
                id = hostInterface->interfaceDesc->bInterfaceClass;
                if (id != USB_HOST_VIDEO_CLASS_CODE)
                {
                    continue;
                }
                id = hostInterface->interfaceDesc->bInterfaceSubClass;
                if (id == USB_HOST_VIDEO_SUBCLASS_CODE_CONTROL)
                {
                    g_VideoDeviceHandle = deviceHandle;
                    g_VideoControlInterfaceHandle = hostInterface;
                    continue;
                }
                else if (id == USB_HOST_VIDEO_SUBCLASS_CODE_STREAM)
                {
                    g_VideoDeviceHandle = deviceHandle;
                    g_VideoStreamInterfaceHandle = hostInterface;
                    continue;
                }
                else
                {
                }
            }
            if (g_VideoDeviceHandle != NULL)
            {
                return kStatus_USB_Success;
            }
            status = kStatus_USB_NotSupported;
            break;

        case kUSB_HostEventNotSupported:
            break;

        case kUSB_HostEventEnumerationDone:
            if (g_Video.devState == kStatus_DEV_Idle)
            {
                if ((g_VideoDeviceHandle != NULL) && (g_VideoControlInterfaceHandle != NULL))
                {
                    g_Video.devState = kStatus_DEV_Attached;
                    g_Video.deviceHandle = g_VideoDeviceHandle;
                    g_Video.controlIntfHandle = g_VideoControlInterfaceHandle;
                    g_Video.streamIntfHandle = g_VideoStreamInterfaceHandle;

                    USB_HostHelperGetPeripheralInformation(deviceHandle, kUSB_HostGetDevicePID, &info_value);
                    usb_echo("video device attached:pid=0x%x", info_value);
                    USB_HostHelperGetPeripheralInformation(deviceHandle, kUSB_HostGetDeviceVID, &info_value);
                    usb_echo("vid=0x%x ", info_value);
                    USB_HostHelperGetPeripheralInformation(deviceHandle, kUSB_HostGetDeviceAddress, &info_value);
                    usb_echo("address=%d\r\n", info_value);
                }
            }
            else
            {
                usb_echo("not idle vide instance\r\n");
            }
            break;

        case kUSB_HostEventDetach:
            if (g_Video.devState != kStatus_DEV_Idle)
            {
                g_Video.devState = kStatus_DEV_Detached;
            }
            break;

        default:
            break;
    }
    return status;
}

/*!
* @brief host video sdcard initialization function.
*
* This function implements the sdcard detect and initialization
*
*/
usb_status_t USB_HostVideoAppSDcardInit(void)
{
    FATFS *fs;
    FIL fileObj;
    uint32_t freeClusterNumber;
    uint32_t freeMemorySizeMB;
    char fileName[25];

    usb_echo("please insert SD card\r\n");
    if (USB_HostVideoAppSdcardWaitCardInsert() != kStatus_USB_Success)
    {
        usb_echo("sdcard init fail\r\n");
        return kStatus_USB_Error;
    }
    else
    {
        usb_echo("sdcard inserted\r\n");
    }

    if (f_mount(&g_fileSystem, g_DriverNumberBuffer, 1U))
    {
        usb_echo("f_mount failed.\r\n");
        return kStatus_USB_Error;
    }

    if (f_getfree(&g_DriverNumberBuffer[0], (DWORD *)&freeClusterNumber, &fs))
    {
        usb_echo("f_getfree failed.\r\n");
        return kStatus_USB_Error;
    }
    else
    {
        freeMemorySizeMB = freeClusterNumber * (fs->csize) / 2048;
        usb_echo("sdcard free size: %d MB\r\n", freeMemorySizeMB);
    }

    /* this operation is to make sure the sdcard and fatfs system is ready fully */
    sprintf(fileName, "%svideo.txt", &g_DriverNumberBuffer[0]);
    if (f_open(&fileObj, _T(fileName), FA_WRITE | FA_CREATE_ALWAYS) == FR_OK)
    {
        f_close(&fileObj);
        f_unlink(fileName);
    }
    return kStatus_USB_Success;
}
