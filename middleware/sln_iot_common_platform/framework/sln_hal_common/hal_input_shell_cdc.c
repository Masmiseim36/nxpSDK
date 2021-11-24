/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 *
 */

/*
 * @brief shell over USB CDC HAL driver implementation.
 */

#include "board_define.h"
#include <FreeRTOS.h>

#include "fsl_component_serial_manager.h"
#include "fsl_component_serial_port_usb.h"

#include "fsl_shell.h"

#include "fwk_input_manager.h"
#include "fwk_common.h"
#include "fwk_log.h"
#include "hal_event_descriptor_face_rec.h"
#include "hal_input_dev.h"
#include "hal_lpm_dev.h"

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_class.h"
#include "usb_device_cdc_acm.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"
#include "sln_flash_config.h"
#include "composite.h"
#include "fica_definition.h"
#include "hal_vision_algo_oasis_lite.h"
#include "virtual_com.h"
#include "sln_device_utils.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define SHELL_PROMPT         "SHELL>> "
#define SHELL_NAME           "shell_usb"
#define USB_RING_BUFFER_SIZE (1024U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static hal_input_status_t HAL_InputDev_ShellUsb_Init(input_dev_t *dev, input_dev_callback_t callback);
static hal_input_status_t HAL_InputDev_ShellUsb_Deinit(const input_dev_t *dev);
static hal_input_status_t HAL_InputDev_ShellUsb_Start(const input_dev_t *dev);
static hal_input_status_t HAL_InputDev_ShellUsb_Stop(const input_dev_t *dev);
static hal_input_status_t HAL_InputDev_ShellUsb_InputNotify(const input_dev_t *dev, void *param);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static serial_usb_cdc_state_t s_UsbDeviceCDC;
AT_NONCACHEABLE_SECTION(static SERIAL_MANAGER_HANDLE_DEFINE(s_USBSerialHandleBuffer));
static serial_handle_t s_USBSerialHandle = (serial_handle_t)&s_USBSerialHandleBuffer[0];

AT_NONCACHEABLE_SECTION_ALIGN(static SHELL_HANDLE_DEFINE(s_USBShellHandleBuffer), 4);
static shell_handle_t s_USBShellHandle = (shell_handle_t)&s_USBShellHandleBuffer[0];

AT_NONCACHEABLE_SECTION_ALIGN(uint8_t s_readRingBuffer[USB_RING_BUFFER_SIZE], 4);

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
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_CurrRecvBuf[DATA_BUFF_SIZE];

extern usb_device_endpoint_struct_t g_cdcVcomDicEndpoints[];
extern usb_device_endpoint_struct_t g_cdcVcomCicEndpoints[];
extern usb_device_class_struct_t g_UsbDeviceCdcVcomConfig[];

const static input_dev_operator_t s_InputDev_ShellUsbOps = {
    .init        = HAL_InputDev_ShellUsb_Init,
    .deinit      = HAL_InputDev_ShellUsb_Deinit,
    .start       = HAL_InputDev_ShellUsb_Start,
    .stop        = HAL_InputDev_ShellUsb_Stop,
    .inputNotify = HAL_InputDev_ShellUsb_InputNotify,
};

static input_dev_t s_InputDev_ShellUsb = {
    .id = 1, .name = SHELL_NAME, .ops = &s_InputDev_ShellUsbOps, .cap = {.callback = NULL}};

serial_manager_status_t Serial_UsbCdcInit(serial_handle_t serialHandle, void *serialConfig)
{
    *(uint32_t *)serialHandle = (uint32_t)&s_UsbDeviceCDC;
    return kStatus_SerialManager_Success;
}

static usb_status_t USB_DeviceCdcVcomCallback(class_handle_t handle, uint32_t event, void *param)
{
    uint32_t len;
    uint8_t *uartBitmap;
    usb_cdc_acm_info_t *acmInfo;
    usb_device_cdc_acm_request_param_struct_t *acmReqParam;
    usb_device_endpoint_callback_message_struct_t *epCbParam;
    serial_manager_callback_message_t msg;
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
            if ((1 == s_UsbDeviceCDC.attach))
            {
                if ((epCbParam->length != 0) && (!(epCbParam->length % s_UsbDeviceCDC.bulkInEndpointMaxPacketSize)))
                {
                    /* If the last packet is the size of endpoint, then send also zero-ended packet,
                     ** meaning that we want to inform the host that we do not have any additional
                     ** data, so it can flush the output.
                     */
                    error = USB_DeviceCdcAcmSend(handle, s_UsbDeviceCDC.bulkInEndpoint, NULL, 0);
                }
                else
                {
                    s_UsbDeviceCDC.tx.busy = 0;

                    if ((NULL != s_UsbDeviceCDC.tx.callback))
                    {
                        serial_manager_status_t serialManagerStatus = kStatus_SerialManager_Success;
                        msg.buffer                                  = s_UsbDeviceCDC.tx.buffer;
                        msg.length                                  = s_UsbDeviceCDC.tx.length;
                        if (USB_UNINITIALIZED_VAL_32 == epCbParam->length)
                        {
                            serialManagerStatus = kStatus_SerialManager_Canceled;
                        }
                        s_UsbDeviceCDC.tx.callback(s_UsbDeviceCDC.tx.callbackParam, &msg, serialManagerStatus);
                    }
                }
            }
        }

        break;
        case kUSB_DeviceCdcEventRecvResponse:
        {
            s_UsbDeviceCDC.rx.busy = 0U;
            if ((1 == s_UsbDeviceCDC.attach) && (1 == s_UsbDeviceCDC.startTransactions))
            {
                serial_manager_status_t callbackStatus = kStatus_SerialManager_Success;

                if ((NULL != s_UsbDeviceCDC.rx.callback))
                {
                    msg.buffer = epCbParam->buffer;
                    msg.length = epCbParam->length;
                    if (USB_UNINITIALIZED_VAL_32 == msg.length)
                    {
                        msg.length     = 0U;
                        callbackStatus = kStatus_SerialManager_Canceled;
                    }
                    s_UsbDeviceCDC.rx.callback(s_UsbDeviceCDC.rx.callbackParam, &msg, callbackStatus);
                }
            }
        }
        break;

        case kUSB_DeviceCdcEventSerialStateNotif:
            ((usb_device_cdc_acm_struct_t *)handle)->hasSentState = 0;
            if ((s_UsbDeviceCDC.startTransactions) && (s_UsbDeviceCDC.tx.waiting4Prime))
            {
                s_UsbDeviceCDC.tx.waiting4Prime = 0;
                s_UsbDeviceCDC.tx.busy          = 1;
                if (kStatus_USB_Success != USB_DeviceCdcAcmSend(s_UsbDeviceCDC.cdcAcmHandle,
                                                                s_UsbDeviceCDC.bulkInEndpoint, s_UsbDeviceCDC.tx.buffer,
                                                                s_UsbDeviceCDC.tx.length))
                {
                    s_UsbDeviceCDC.tx.busy = 0;
                }
            }
            error = kStatus_USB_Success;
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
                }
            }
            else
            {
                /* DTE_DEACTIVATED */
                if (1 == s_UsbDeviceCDC.attach)
                {
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
 *
 * This function sets configuration for CDC class.
 *
 * @param handle The CDC ACM class handle.
 * @param configure The CDC ACM class configure index.
 *
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
        USB_DeviceCdcAcmRecv(s_UsbDeviceCDC.cdcAcmHandle, s_UsbDeviceCDC.bulkOutEndpoint, s_CurrRecvBuf,
                             s_UsbDeviceCDC.bulkOutEndpointMaxPacketSize);
    }
    return kStatus_USB_Success;
}

/*!
 * @brief Virtual COM device initialization function.
 *
 * This function initializes the device with the composite device class information.
 *
 * @param deviceComposite The pointer to the composite device structure.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
static usb_status_t USB_DeviceCdcVcomInit()
{
    s_UsbDeviceCDC.instance      = CONTROLLER_ID;
    s_UsbDeviceCDC.lineCoding    = (uint8_t *)s_lineCoding;
    s_UsbDeviceCDC.abstractState = (uint8_t *)s_abstractState;
    s_UsbDeviceCDC.countryCode   = (uint8_t *)s_countryCode;
    s_UsbDeviceCDC.usbCdcAcmInfo = &s_usbCdcAcmInfo;
    s_UsbDeviceCDC.currRecvBuf   = (uint8_t *)&s_CurrRecvBuf[0];

    return kStatus_USB_Success;
}

static usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_Success;
    uint8_t *temp8     = (uint8_t *)param;

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

/*******************************************************************************
 * Code
 ******************************************************************************/

__attribute__((weak)) void APP_InputDev_Shell_RegisterShellCommands(shell_handle_t shellContextHandle,
                                                                    input_dev_t *shellDev,
                                                                    input_dev_callback_t callback)
{
}

static hal_input_status_t HAL_InputDev_ShellUsb_Init(input_dev_t *dev, input_dev_callback_t callback)
{
    hal_input_status_t error                  = kStatus_HAL_InputSuccess;
    s_InputDev_ShellUsb.cap.callback          = callback;
    s_USBShellHandle                          = &s_USBShellHandleBuffer[0];
    serial_port_usb_cdc_config_t usbCdcConfig = {
        .controllerIndex = (serial_port_usb_cdc_controller_index_t)CONTROLLER_ID,
    };
    serial_manager_config_t config;
    memset(&config, 0, sizeof(serial_manager_config_t));
    config.type = kSerialPort_UsbCdc, config.ringBuffer = s_readRingBuffer;
    config.ringBufferSize = USB_RING_BUFFER_SIZE;
    config.portConfig     = &usbCdcConfig;

    SerialManager_Init(s_USBSerialHandle, &config);

    SHELL_Init(s_USBShellHandle, s_USBSerialHandle, (char *)SHELL_PROMPT);
    /* Register externally-defined commands w/ the shell */
    APP_InputDev_Shell_RegisterShellCommands(s_USBShellHandle, dev, callback);

    if (USB_DeviceApplicationInit())
    {
        LOGE("USB device shell over cdc failed");
        error = kStatus_HAL_InputError;
    }
    return error;
}

static hal_input_status_t HAL_InputDev_ShellUsb_Deinit(const input_dev_t *dev)
{
    hal_input_status_t error = kStatus_HAL_InputSuccess;
    return error;
}

static hal_input_status_t HAL_InputDev_ShellUsb_Start(const input_dev_t *dev)
{
    hal_input_status_t error = kStatus_HAL_InputSuccess;
    LOGD("++HAL_InputDev_ShellUsb_start");
    USB_DeviceIsrEnable();
    Time_Current();
    vTaskDelay(200);
    USB_DeviceRun(s_UsbDeviceCDC.deviceHandle);

    LOGD("--HAL_InputDev_ShellUsb_start");
    return error;
}

static hal_input_status_t HAL_InputDev_ShellUsb_Stop(const input_dev_t *dev)
{
    hal_input_status_t error = 0;
    LOGD("++HAL_InputDev_ShellUsb_stop");

    LOGD("--HAL_InputDev_ShellUsb_stop");
    return error;
}

static hal_input_status_t HAL_InputDev_ShellUsb_InputNotify(const input_dev_t *dev, void *param)
{
    hal_input_status_t error = kStatus_HAL_InputSuccess;
    return error;
}

int HAL_InputDev_ShellUsb_Register()
{
    int error = 0;
    LOGD("SLN_ShellDevUSBCDCRegister");
    s_UsbDeviceCDC.cdcAcmHandle = NULL;
    USB_RegisterCDC();
    USB_CompositeClassRegister(USB_DeviceCallback, USB_DeviceCdcVcomCallback, &g_UsbDeviceCdcVcomConfig[0]);

    error = FWK_InputManager_DeviceRegister(&s_InputDev_ShellUsb);
    return error;
}
