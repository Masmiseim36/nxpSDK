/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief  display over usb_cdc implementation.
 */

#include <FreeRTOS.h>
#include "event_groups.h"
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"
#include "usb_device_class.h"
#include "usb_device_cdc_acm.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#include "fwk_log.h"
#include "fwk_display_manager.h"
#include "hal_display_dev.h"

#include "composite.h"
#include "virtual_com.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DISPLAY_NAME                        "diplay usb cdc"
#define DISPLAY_3D_IR_FRAME_WIDTH           540
#define DISPLAY_3D_IR_FRAME_HEIGHT          640
#define DISPLAY_3D_IR_FRAME_BYTES_PER_PIXEL 3

#define DISPLAY_3D_DEPTH_FRAME_WIDTH           540
#define DISPLAY_3D_DEPTH_FRAME_HEIGHT          640
#define DISPLAY_3D_DEPTH_FRAME_BYTES_PER_PIXEL 2

#define DISPLAY_2D_RGB_FRAME_WIDTH           480
#define DISPLAY_2D_RGB_FRAME_HEIGHT          640
#define DISPLAY_2D_RGB_FRAME_BYTES_PER_PIXEL 3

#define DISPLAY_2D_IR_FRAME_WIDTH           480
#define DISPLAY_2D_IR_FRAME_HEIGHT          640
#define DISPLAY_2D_IR_FRAME_BYTES_PER_PIXEL 3

#define DISPLAY_MAX_FRAME_WIDTH           540
#define DISPLAY_MAX_FRAME_HEIGHT          640
#define DISPLAY_MAX_FRAME_BYTES_PER_PIXEL 3
#define DISPLAY_FRAME_BUFFER_COUNT        2

#define MAX_SEND_SIZE (32768U)

/* Line coding of cdc device */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t s_lineCoding[LINE_CODING_SIZE] = {
    /* E.g. 0x00,0xC2,0x01,0x00 : 0x0001C200 is 115200 bits per second */
    (LINE_CODING_DTERATE >> 0U) & 0x000000FFU,
    (LINE_CODING_DTERATE >> 8U) & 0x000000FFU,
    (LINE_CODING_DTERATE >> 16U) & 0x000000FFU,
    (LINE_CODING_DTERATE >> 24U) & 0x000000FFU,
    LINE_CODING_CHARFORMAT,
    LINE_CODING_PARITYTYPE,
    LINE_CODING_DATABITS};

/* Abstract state of cdc device */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t s_abstractState[COMM_FEATURE_DATA_SIZE] = {(STATUS_ABSTRACT_STATE >> 0U) & 0x00FFU,
                                                          (STATUS_ABSTRACT_STATE >> 8U) & 0x00FFU};

/* Country code of cdc device */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t s_countryCode[COMM_FEATURE_DATA_SIZE] = {(COUNTRY_SETTING >> 0U) & 0x00FFU,
                                                        (COUNTRY_SETTING >> 8U) & 0x00FFU};

/* CDC ACM information */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static usb_cdc_acm_info_t s_usbCdcAcmInfo = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0, 0, 0, 0, 0,
};
/* Data buffer for receiving and sending*/
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_currRecvBuf[DATA_BUFF_SIZE];
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_htUnit[14];

extern usb_device_endpoint_struct_t g_cdcVcomDicEndpoints[];
extern usb_device_endpoint_struct_t g_cdcVcomCicEndpoints[];
extern usb_device_class_struct_t g_UsbDeviceCdcVcomConfig[];

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif
static hal_display_status_t HAL_DisplayDev_UsbCdc_Init(
    display_dev_t *dev, int width, int height, display_dev_callback_t callback, void *param);
static hal_display_status_t HAL_DisplayDev_UsbCdc_DeInit(const display_dev_t *dev);
static hal_display_status_t HAL_DisplayDev_UsbCdc_Start(const display_dev_t *dev);
static hal_display_status_t HAL_DisplayDev_UsbCdc_Blit(const display_dev_t *dev, void *frame, int width, int height);
#if defined(__cplusplus)
}
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/
static serial_usb_cdc_state_t s_UsbDeviceCDC;

AT_NONCACHEABLE_SECTION_ALIGN(
    static uint8_t s_FrameBuffer[DISPLAY_FRAME_BUFFER_COUNT][DISPLAY_MAX_FRAME_HEIGHT * DISPLAY_MAX_FRAME_WIDTH *
                                                             DISPLAY_MAX_FRAME_BYTES_PER_PIXEL],
    64);
static volatile bool s_HasFinished = true;
static display_dev_t *s_pDisplayDevUSBCDC;

static usb_status_t USB_DeviceCdcVcomCallback(class_handle_t handle, uint32_t event, void *param)
{
    uint32_t len;
    uint8_t *uartBitmap;
    usb_cdc_acm_info_t *acmInfo;
    usb_device_cdc_acm_request_param_struct_t *acmReqParam;
    usb_device_endpoint_callback_message_struct_t *epCbParam;
    usb_status_t error = kStatus_USB_Error;
    acmReqParam        = (usb_device_cdc_acm_request_param_struct_t *)param;
    epCbParam          = (usb_device_endpoint_callback_message_struct_t *)param;

    if (handle != s_UsbDeviceCDC.cdcAcmHandle)
    {
        return error;
    }

    acmInfo = s_UsbDeviceCDC.usbCdcAcmInfo;
    switch (event)
    {
        case kUSB_DeviceCdcEventSendResponse:
        {
            if ((epCbParam->length != 0) && (!(epCbParam->length % s_UsbDeviceCDC.bulkInEndpointMaxPacketSize)))
            {
                /* If the last packet is the size of endpoint, then send also zero-ended packet,
                 ** meaning that we want to inform the host that we do not have any additional
                 ** data, so it can flush the output.
                 */
                error = USB_DeviceCdcAcmSend(handle, s_UsbDeviceCDC.bulkInEndpoint, NULL, 0);
            }
            else if ((1 == s_UsbDeviceCDC.attach) && (1 == s_UsbDeviceCDC.startTransactions))
            {
                if ((epCbParam->buffer != NULL) || ((epCbParam->buffer == NULL) && (epCbParam->length == 0)))
                {
                    s_HasFinished = true;
                }
            }
            else
            {
            }
        }
        break;
        case kUSB_DeviceCdcEventRecvResponse:
        {
            if ((1 == s_UsbDeviceCDC.attach) && (1 == s_UsbDeviceCDC.startTransactions))
            {
                s_UsbDeviceCDC.recvSize = epCbParam->length;

                if (!s_UsbDeviceCDC.recvSize)
                {
                    /* Schedule buffer for next receive event */
                    error = USB_DeviceCdcAcmRecv(handle, s_UsbDeviceCDC.bulkOutEndpoint, s_UsbDeviceCDC.currRecvBuf,
                                                 s_UsbDeviceCDC.bulkOutEndpointMaxPacketSize);
                }
            }
        }
        break;
        case kUSB_DeviceCdcEventSerialStateNotif:
            ((usb_device_cdc_acm_struct_t *)handle)->hasSentState = 0;
            error                                                 = kStatus_USB_Success;
            break;
        case kUSB_DeviceCdcEventSendEncapsulatedCommand:
            break;
        case kUSB_DeviceCdcEventGetEncapsulatedResponse:
            break;
        case kUSB_DeviceCdcEventSetCommFeature:
            if (USB_DEVICE_CDC_FEATURE_ABSTRACT_STATE == acmReqParam->setupValue)
            {
                if (1 == acmReqParam->isSetup)
                {
                    *(acmReqParam->buffer) = s_UsbDeviceCDC.abstractState;
                }
                else
                {
                    *(acmReqParam->length) = 0;
                }
            }
            else if (USB_DEVICE_CDC_FEATURE_COUNTRY_SETTING == acmReqParam->setupValue)
            {
                if (1 == acmReqParam->isSetup)
                {
                    *(acmReqParam->buffer) = s_UsbDeviceCDC.countryCode;
                }
                else
                {
                    *(acmReqParam->length) = 0;
                }
            }
            else
            {
            }
            error = kStatus_USB_Success;
            break;
        case kUSB_DeviceCdcEventGetCommFeature:
            if (USB_DEVICE_CDC_FEATURE_ABSTRACT_STATE == acmReqParam->setupValue)
            {
                *(acmReqParam->buffer) = s_UsbDeviceCDC.abstractState;
                *(acmReqParam->length) = COMM_FEATURE_DATA_SIZE;
            }
            else if (USB_DEVICE_CDC_FEATURE_COUNTRY_SETTING == acmReqParam->setupValue)
            {
                *(acmReqParam->buffer) = s_UsbDeviceCDC.countryCode;
                *(acmReqParam->length) = COMM_FEATURE_DATA_SIZE;
            }
            else
            {
            }
            error = kStatus_USB_Success;
            break;
        case kUSB_DeviceCdcEventClearCommFeature:
            break;
        case kUSB_DeviceCdcEventGetLineCoding:
            *(acmReqParam->buffer) = s_UsbDeviceCDC.lineCoding;
            *(acmReqParam->length) = LINE_CODING_SIZE;
            error                  = kStatus_USB_Success;
            break;
        case kUSB_DeviceCdcEventSetLineCoding:
        {
            if (1 == acmReqParam->isSetup)
            {
                *(acmReqParam->buffer) = s_UsbDeviceCDC.lineCoding;
            }
            else
            {
                *(acmReqParam->length) = 0;
            }
        }
            error = kStatus_USB_Success;
            break;
        case kUSB_DeviceCdcEventSetControlLineState:
        {
            s_UsbDeviceCDC.usbCdcAcmInfo->dteStatus = acmReqParam->setupValue;
            /* activate/deactivate Tx carrier */
            if (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_CARRIER_ACTIVATION)
            {
                acmInfo->uartState |= USB_DEVICE_CDC_UART_STATE_TX_CARRIER;
            }
            else
            {
                acmInfo->uartState &= (uint16_t)~USB_DEVICE_CDC_UART_STATE_TX_CARRIER;
            }

            /* activate carrier and DTE. Com port of terminal tool running on PC is open now */
            if (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_DTE_PRESENCE)
            {
                acmInfo->uartState |= USB_DEVICE_CDC_UART_STATE_RX_CARRIER;
            }
            /* Com port of terminal tool running on PC is closed now */
            else
            {
                acmInfo->uartState &= (uint16_t)~USB_DEVICE_CDC_UART_STATE_RX_CARRIER;
            }

            /* Indicates to DCE if DTE is present or not */
            acmInfo->dtePresent = (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_DTE_PRESENCE) ? true : false;

            /* Initialize the serial state buffer */
            acmInfo->serialStateBuf[0] = NOTIF_REQUEST_TYPE;                /* bmRequestType */
            acmInfo->serialStateBuf[1] = USB_DEVICE_CDC_NOTIF_SERIAL_STATE; /* bNotification */
            acmInfo->serialStateBuf[2] = 0x00;                              /* wValue */
            acmInfo->serialStateBuf[3] = 0x00;
            acmInfo->serialStateBuf[4] = 0x00; /* wIndex */
            acmInfo->serialStateBuf[5] = 0x00;
            acmInfo->serialStateBuf[6] = UART_BITMAP_SIZE; /* wLength */
            acmInfo->serialStateBuf[7] = 0x00;
            /* Notify to host the line state */
            acmInfo->serialStateBuf[4] = acmReqParam->interfaceIndex;
            /* Lower byte of UART BITMAP */
            uartBitmap    = (uint8_t *)&acmInfo->serialStateBuf[NOTIF_PACKET_SIZE + UART_BITMAP_SIZE - 2];
            uartBitmap[0] = acmInfo->uartState & 0xFFu;
            uartBitmap[1] = (acmInfo->uartState >> 8) & 0xFFu;
            len           = (uint32_t)(NOTIF_PACKET_SIZE + UART_BITMAP_SIZE);
            if (0 == ((usb_device_cdc_acm_struct_t *)handle)->hasSentState)
            {
                error = USB_DeviceCdcAcmSend(handle, s_UsbDeviceCDC.interruptEndpoint, acmInfo->serialStateBuf, len);
                if (kStatus_USB_Success != error)
                {
                    usb_echo("kUSB_DeviceCdcEventSetControlLineState error!");
                }
                ((usb_device_cdc_acm_struct_t *)handle)->hasSentState = 1;
            }

            /* Update status */
            if (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_CARRIER_ACTIVATION)
            {
                /*  To do: CARRIER_ACTIVATED */
            }
            else
            {
                /* To do: CARRIER_DEACTIVATED */
            }
            if (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_DTE_PRESENCE)
            {
                /* DTE_ACTIVATED */
                if (1 == s_UsbDeviceCDC.attach)
                {
                    s_UsbDeviceCDC.startTransactions = 1;
                    for (int i = 0; i < 2; i++)
                    {
                        if ((s_pDisplayDevUSBCDC != NULL) && (s_pDisplayDevUSBCDC[i].cap.callback != NULL))
                        {
                            s_pDisplayDevUSBCDC[i].cap.callback(&s_pDisplayDevUSBCDC[i], kDisplayEvent_RequestFrame,
                                                                s_pDisplayDevUSBCDC[i].cap.frameBuffer, true);
                        }
                    }
                }
            }
            else
            {
                /* DTE_DEACTIVATED */
                if (1 == s_UsbDeviceCDC.attach)
                {
                    s_HasFinished                    = true;
                    s_UsbDeviceCDC.startTransactions = 0;
                }
            }
        }
        break;
        case kUSB_DeviceCdcEventSendBreak:
            break;
        default:
            break;
    }

    return error;
}

/*!
 * @brief Virtual COM device set configuration function.
 * This function sets configuration for CDC class.
 * @param handle The CDC ACM class handle.
 * @param configure The CDC ACM class configure index.
 * @return A USB error code or kStatus_USB_Success.
 */
static usb_status_t USB_DeviceCdcVcomSetConfigure(class_handle_t handle, uint8_t configure)
{
    if (USB_COMPOSITE_CONFIGURE_INDEX == configure)
    {
        /*endpoint information for cdc 1*/
        s_UsbDeviceCDC.attach = 1;

        s_UsbDeviceCDC.interruptEndpoint              = USB_CDC_VCOM_CIC_INTERRUPT_IN_ENDPOINT;
        s_UsbDeviceCDC.interruptEndpointMaxPacketSize = g_cdcVcomCicEndpoints[0].maxPacketSize;

        s_UsbDeviceCDC.bulkInEndpoint              = USB_CDC_VCOM_DIC_BULK_IN_ENDPOINT;
        s_UsbDeviceCDC.bulkInEndpointMaxPacketSize = g_cdcVcomDicEndpoints[0].maxPacketSize;

        s_UsbDeviceCDC.bulkOutEndpoint              = USB_CDC_VCOM_DIC_BULK_OUT_ENDPOINT;
        s_UsbDeviceCDC.bulkOutEndpointMaxPacketSize = g_cdcVcomDicEndpoints[1].maxPacketSize;

        /* Schedule buffer for receive */
        USB_DeviceCdcAcmRecv(s_UsbDeviceCDC.cdcAcmHandle, s_UsbDeviceCDC.bulkOutEndpoint, s_currRecvBuf,
                             s_UsbDeviceCDC.bulkOutEndpointMaxPacketSize);
    }
    return kStatus_USB_Success;
}

/*!
 * @brief Virtual COM device initialization function.
 * This function initializes the device with the composite device class information.
 * @param deviceComposite The pointer to the composite device structure.
 * @return A USB error code or kStatus_USB_Success.
 */
static usb_status_t USB_DeviceCdcVcomInit()
{
    s_UsbDeviceCDC.lineCoding    = (uint8_t *)s_lineCoding;
    s_UsbDeviceCDC.abstractState = (uint8_t *)s_abstractState;
    s_UsbDeviceCDC.countryCode   = (uint8_t *)s_countryCode;
    s_UsbDeviceCDC.usbCdcAcmInfo = &s_usbCdcAcmInfo;
    s_UsbDeviceCDC.currRecvBuf   = (uint8_t *)&s_currRecvBuf[0];

    return kStatus_USB_Success;
}

static usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_Success;
    uint8_t *temp8     = (uint8_t *)param;
    uint16_t *temp16   = (uint16_t *)param;

    switch (event)
    {
        case kUSB_DeviceClassInit:
        {
            s_UsbDeviceCDC.deviceHandle = handle;
            s_UsbDeviceCDC.cdcAcmHandle = (class_handle_t)param;
            USB_DeviceCdcVcomInit();
        }
        break;
        case kUSB_DeviceEventBusReset:
        {
            error                               = kStatus_USB_Success;
            s_UsbDeviceCDC.attach               = 0;
            s_UsbDeviceCDC.currentConfiguration = 0U;
            s_UsbDeviceCDC.recvSize             = 0;
            s_UsbDeviceCDC.sendSize             = 0;
        }
        break;
        case kUSB_DeviceEventSetConfiguration:
            if (0U == (*temp8))
            {
                s_UsbDeviceCDC.attach               = 0U;
                s_UsbDeviceCDC.currentConfiguration = 0U;
                s_UsbDeviceCDC.recvSize             = 0;
                s_UsbDeviceCDC.sendSize             = 0;
            }
            else if (USB_COMPOSITE_CONFIGURE_INDEX == (*temp8))
            {
                USB_DeviceCdcVcomSetConfigure(s_UsbDeviceCDC.cdcAcmHandle, *temp8);
                s_UsbDeviceCDC.currentConfiguration = *temp8;
                error                               = kStatus_USB_Success;
            }
            else
            {
                error = kStatus_USB_InvalidRequest;
            }
            break;
        case kUSB_DeviceEventSetInterface:
            error = kStatus_USB_Success;
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
#if (defined(USB_DEVICE_CONFIG_CV_TEST) && (USB_DEVICE_CONFIG_CV_TEST > 0U))
        case kUSB_DeviceEventGetDeviceQualifierDescriptor:
            if (param)
            {
                /* Get device descriptor request */
                error = USB_DeviceGetDeviceQualifierDescriptor(
                    handle, (usb_device_get_device_qualifier_descriptor_struct_t *)param);
            }
            break;
#endif
        case kUSB_DeviceEventGetStringDescriptor:
            error = kStatus_USB_Success;
            break;
        default:
            break;
    }

    return error;
}

const static display_dev_operator_t s_DisplayDev_USBCDC3DOps[2] = {{
                                                                       .init        = HAL_DisplayDev_UsbCdc_Init,
                                                                       .deinit      = HAL_DisplayDev_UsbCdc_DeInit,
                                                                       .start       = HAL_DisplayDev_UsbCdc_Start,
                                                                       .blit        = HAL_DisplayDev_UsbCdc_Blit,
                                                                       .inputNotify = NULL,
                                                                   },
                                                                   {
                                                                       .init        = HAL_DisplayDev_UsbCdc_Init,
                                                                       .deinit      = HAL_DisplayDev_UsbCdc_DeInit,
                                                                       .start       = NULL,
                                                                       .blit        = HAL_DisplayDev_UsbCdc_Blit,
                                                                       .inputNotify = NULL,
                                                                   }};

const static display_dev_operator_t s_DisplayDev_USBCDC2DOps[2] = {{
                                                                       .init        = HAL_DisplayDev_UsbCdc_Init,
                                                                       .deinit      = HAL_DisplayDev_UsbCdc_DeInit,
                                                                       .start       = HAL_DisplayDev_UsbCdc_Start,
                                                                       .blit        = HAL_DisplayDev_UsbCdc_Blit,
                                                                       .inputNotify = NULL,
                                                                   },
                                                                   {
                                                                       .init        = HAL_DisplayDev_UsbCdc_Init,
                                                                       .deinit      = HAL_DisplayDev_UsbCdc_DeInit,
                                                                       .start       = NULL,
                                                                       .blit        = HAL_DisplayDev_UsbCdc_Blit,
                                                                       .inputNotify = NULL,
                                                                   }};

static display_dev_t s_DisplayDev_USBCDC3D[2] = {
    {.id   = 0,
     .name = DISPLAY_NAME,
     .ops  = &s_DisplayDev_USBCDC3DOps[0],
     .cap  = {.width       = DISPLAY_3D_IR_FRAME_WIDTH,
             .height      = DISPLAY_3D_IR_FRAME_HEIGHT,
             .pitch       = DISPLAY_3D_IR_FRAME_WIDTH * DISPLAY_3D_IR_FRAME_BYTES_PER_PIXEL,
             .left        = 0,
             .top         = 0,
             .right       = DISPLAY_3D_IR_FRAME_WIDTH - 1,
             .bottom      = DISPLAY_3D_IR_FRAME_HEIGHT - 1,
             .rotate      = kCWRotateDegree_0,
             .format      = kPixelFormat_BGR,
             .srcFormat   = kPixelFormat_Gray16,
             .frameBuffer = (void *)&s_FrameBuffer[0],
             .callback    = NULL,
             .param       = NULL}},
    {.id   = 1,
     .name = DISPLAY_NAME,
     .ops  = &s_DisplayDev_USBCDC3DOps[1],
     .cap  = {.width       = DISPLAY_3D_DEPTH_FRAME_WIDTH,
             .height      = DISPLAY_3D_DEPTH_FRAME_HEIGHT,
             .pitch       = DISPLAY_3D_DEPTH_FRAME_WIDTH * DISPLAY_3D_DEPTH_FRAME_BYTES_PER_PIXEL,
             .left        = 0,
             .top         = 0,
             .right       = DISPLAY_3D_DEPTH_FRAME_WIDTH - 1,
             .bottom      = DISPLAY_3D_DEPTH_FRAME_HEIGHT - 1,
             .rotate      = kCWRotateDegree_0,
             .format      = kPixelFormat_Depth16,
             .srcFormat   = kPixelFormat_Depth16,
             .frameBuffer = (void *)&s_FrameBuffer[1],
             .callback    = NULL,
             .param       = NULL}},
};

static display_dev_t s_DisplayDev_USBCDC2D[2] = {
    {.id   = 0,
     .name = DISPLAY_NAME,
     .ops  = &s_DisplayDev_USBCDC2DOps[0],
     .cap  = {.width       = DISPLAY_2D_IR_FRAME_WIDTH,
             .height      = DISPLAY_2D_IR_FRAME_HEIGHT,
             .pitch       = DISPLAY_2D_IR_FRAME_WIDTH * DISPLAY_2D_IR_FRAME_BYTES_PER_PIXEL,
             .left        = 0,
             .top         = 0,
             .right       = DISPLAY_2D_IR_FRAME_WIDTH - 1,
             .bottom      = DISPLAY_2D_IR_FRAME_HEIGHT - 1,
             .rotate      = kCWRotateDegree_0,
             .format      = kPixelFormat_BGR,
             .srcFormat   = kPixelFormat_UYVY1P422_Gray,
             .frameBuffer = (void *)&s_FrameBuffer[0],
             .callback    = NULL,
             .param       = NULL}},
    {.id   = 1,
     .name = DISPLAY_NAME,
     .ops  = &s_DisplayDev_USBCDC2DOps[1],
     .cap  = {.width       = DISPLAY_2D_RGB_FRAME_WIDTH,
             .height      = DISPLAY_2D_RGB_FRAME_HEIGHT,
             .pitch       = DISPLAY_2D_RGB_FRAME_WIDTH * DISPLAY_2D_RGB_FRAME_BYTES_PER_PIXEL,
             .left        = 0,
             .top         = 0,
             .right       = DISPLAY_2D_RGB_FRAME_WIDTH - 1,
             .bottom      = DISPLAY_2D_RGB_FRAME_HEIGHT - 1,
             .rotate      = kCWRotateDegree_0,
             .format      = kPixelFormat_BGR,
             .srcFormat   = kPixelFormat_UYVY1P422_RGB,
             .frameBuffer = (void *)&s_FrameBuffer[1],
             .callback    = NULL,
             .param       = NULL}},
};

static const uint8_t TU_MAGIC[] = {0x53, 0x79, 0x4c};

/*******************************************************************************
 * Code
 ******************************************************************************/

static usb_status_t HAL_DisplayDev_UsbCdc_SendBlocking(void *data, uint32_t size)
{
    usb_status_t error = kStatus_USB_Error;
    s_HasFinished      = false;

    do
    {
        error = USB_DeviceCdcAcmSend(s_UsbDeviceCDC.cdcAcmHandle, s_UsbDeviceCDC.bulkInEndpoint, data, size);
    } while (error == kStatus_USB_Busy);

    while ((s_HasFinished == false) && (s_UsbDeviceCDC.startTransactions != 0))
    {
    }

    if (error != kStatus_USB_Success)
    {
        LOGE("Failed to send over cdc with error %d", error);
    }

    return error;
}

static hal_display_status_t HAL_DisplayDev_UsbCdc_Init(
    display_dev_t *dev, int width, int height, display_dev_callback_t callback, void *param)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    LOGD("HAL_DisplayDev_UsbCdc_Init");

    dev->cap.width    = width;
    dev->cap.height   = height;
    dev->cap.callback = callback;

    if (USB_DeviceApplicationInit() < 0)
    {
        LOGE("USB device display over cdc failed");
        ret = kStatus_HAL_DisplayError;
    }

    return ret;
}

static hal_display_status_t HAL_DisplayDev_UsbCdc_DeInit(const display_dev_t *dev)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    LOGD("HAL_DisplayDev_UsbCdc_DeInit");
    return ret;
}

static hal_display_status_t HAL_DisplayDev_UsbCdc_Start(const display_dev_t *dev)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    LOGD("HAL_DisplayDev_UsbCdc_Start");
    USB_DeviceIsrEnable();
    Time_Current();
    vTaskDelay(200);
    USB_DeviceRun(s_UsbDeviceCDC.deviceHandle);
    return ret;
}

static hal_display_status_t HAL_DisplayDev_UsbCdc_Blit(const display_dev_t *dev, void *frame, int width, int height)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    usb_status_t error       = kStatus_USB_Error;
    uint32_t size            = (dev->cap.height * dev->cap.pitch);

    LOGD("HAL_DisplayDev_UsbCdc_Blit dev name %s.", dev->name);
    if (s_UsbDeviceCDC.startTransactions == 0)
    {
        return kStatus_HAL_DisplayTxBusy;
    }

    memcpy(s_htUnit, TU_MAGIC, sizeof(TU_MAGIC));
    s_htUnit[3] = dev->cap.format;
    s_htUnit[4] = dev->cap.srcFormat;
    s_htUnit[5] = 0;
    memcpy(s_htUnit + 6, &dev->cap.width, 4);
    memcpy(s_htUnit + 10, &dev->cap.height, 4);

    error = HAL_DisplayDev_UsbCdc_SendBlocking(s_htUnit, 14);
    if (error != kStatus_USB_Success)
    {
        LOGE("Failed to send header usbcdc %d", dev->id);
        return kStatus_HAL_DisplayError;
    }

    while (size != 0)
    {
        uint16_t sendSize = MIN(size, MAX_SEND_SIZE);
        size -= sendSize;
        error = HAL_DisplayDev_UsbCdc_SendBlocking(frame, sendSize);
        frame += sendSize;

        if (s_UsbDeviceCDC.startTransactions == 0)
        {
            ret = kStatus_HAL_DisplaySuccess;
            break;
        }

        if (error != kStatus_USB_Success)
        {
            LOGE("Failed to send for display %d", dev->id);
            ret = kStatus_HAL_DisplayError;
            break;
        }
    }

    return ret;
}

int HAL_DisplayDev_UsbCdc3D_Register()
{
    int error = 0;
    LOGD("HAL_DisplayDev_UsbCdc3D_Register");
    s_UsbDeviceCDC.cdcAcmHandle = NULL;
    USB_RegisterCDC();
    USB_CompositeClassRegister(USB_DeviceCallback, USB_DeviceCdcVcomCallback, &g_UsbDeviceCdcVcomConfig[0]);

    error = FWK_DisplayManager_DeviceRegister(&s_DisplayDev_USBCDC3D[0]);
    if (error == 0)
    {
        error = FWK_DisplayManager_DeviceRegister(&s_DisplayDev_USBCDC3D[1]);
    }
    s_pDisplayDevUSBCDC = &s_DisplayDev_USBCDC3D[0];
    return error;
}

int HAL_DisplayDev_UsbCdc2D_Register()
{
    int error = 0;
    LOGD("HAL_DisplayDev_UsbCdc2D_Register");
    s_UsbDeviceCDC.cdcAcmHandle = NULL;
    USB_RegisterCDC();
    USB_CompositeClassRegister(USB_DeviceCallback, USB_DeviceCdcVcomCallback, &g_UsbDeviceCdcVcomConfig[0]);

    error = FWK_DisplayManager_DeviceRegister(&s_DisplayDev_USBCDC2D[0]);
    if (error == 0)
    {
        error = FWK_DisplayManager_DeviceRegister(&s_DisplayDev_USBCDC2D[1]);
    }
    s_pDisplayDevUSBCDC = &s_DisplayDev_USBCDC2D[0];
    return error;
}
