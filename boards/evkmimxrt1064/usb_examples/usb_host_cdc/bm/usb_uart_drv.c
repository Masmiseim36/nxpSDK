/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_uart.h"
#include "fsl_common.h"
#include "board.h"
#include "usb_serial_port.h"

typedef struct _uart_status_struct
{
    uart_handle_t uartHandle;
    uint8_t isUsed;
} uart_status_struct_t;

uart_status_struct_t uartStatus[USB_SERIAL_PORT_INSTANCE_COUNT];

/*!
 * @brief uart callback function.
 *
 *This callback will be called if the uart has get specific num(USB_HOST_CDC_UART_RX_MAX_LEN) char.
 *
 * @param instance           instancehandle.
 * @param uartState           callback event code, please reference to enumeration host_event_t.
 *
 */
void UART_SeralPortTransferCallback(UART_Type *base, uart_handle_t *handle, status_t status, void *userData)
{
    usb_serial_port_handle_t *serialPortHandle = (usb_serial_port_handle_t *)userData;
    if (NULL != serialPortHandle->callback.callbackFunction)
    {
        serialPortHandle->callback.callbackFunction(handle, (((uint32_t)status) % 100) + 1,
                                                    serialPortHandle->callback.callbackParam);
    }
}

usb_serial_port_status_t USB_SerialPortInit(uint8_t instance,
                                            const usb_serial_port_config_t *config,
                                            usb_serial_port_callback_struct_t *callback,
                                            uint32_t sourceClockHz,
                                            usb_serial_port_handle_t *handle)
{
    UART_Type *uart[] = UART_BASE_PTRS;
    int i;
    uart_config_t uartConfiguration;

    if (instance >= (sizeof(uart) / sizeof(UART_Type *)))
    {
        return kStatus_USB_SERIAL_PORT_InvalidParameter;
    }

    if ((NULL == handle) || (NULL == callback))
    {
        return kStatus_USB_SERIAL_PORT_InvalidParameter;
    }

    handle->serialPortHandle = NULL;

    for (i = 0; i < USB_SERIAL_PORT_INSTANCE_COUNT; i++)
    {
        if (0 == uartStatus[i].isUsed)
        {
            handle->serialPortHandle = &uartStatus[i];
            uartStatus[i].isUsed = 1U;
            break;
        }
    }
    if (NULL == handle->serialPortHandle)
    {
        return kStatus_USB_SERIAL_PORT_Busy;
    }

    handle->callback.callbackFunction = callback->callbackFunction;
    handle->callback.callbackParam = callback->callbackParam;

    UART_GetDefaultConfig(&uartConfiguration);

    uartConfiguration.baudRate_Bps = config->baudRate_Bps;
    uartConfiguration.enableTx = config->enableTx;
    uartConfiguration.enableRx = config->enableRx;
    UART_Init(uart[instance], &uartConfiguration, sourceClockHz);

    handle->baseReg = uart[instance];
    handle->instance = instance;

    UART_TransferCreateHandle(uart[instance],
                              (uart_handle_t *)&((uart_status_struct_t *)handle->serialPortHandle)->uartHandle,
                              UART_SeralPortTransferCallback, handle);

    return kStatus_USB_SERIAL_PORT_Success;
}

usb_serial_port_status_t USB_SerialPortRecv(usb_serial_port_handle_t *handle,
                                            usb_serial_port_xfer_t *xfer,
                                            size_t *receivedBytes)
{
    return (usb_serial_port_status_t)(
        UART_TransferReceiveNonBlocking(
            handle->baseReg, (uart_handle_t *)&((uart_status_struct_t *)handle->serialPortHandle)->uartHandle,
            (uart_transfer_t *)xfer, receivedBytes) %
            100 +
        1);
}

usb_serial_port_status_t USB_SerialPortSend(usb_serial_port_handle_t *handle, usb_serial_port_xfer_t *xfer)
{
    return (usb_serial_port_status_t)(
        UART_TransferSendNonBlocking(handle->baseReg,
                                     (uart_handle_t *)&((uart_status_struct_t *)handle->serialPortHandle)->uartHandle,
                                     (uart_transfer_t *)xfer) %
            100 +
        1);
}

usb_serial_port_status_t USB_SerialPortDeinit(usb_serial_port_handle_t *handle)
{
    UART_Deinit(handle->baseReg);
    return kStatus_USB_SERIAL_PORT_Success;
}

void USB_SerialPortIRQHandler(usb_serial_port_handle_t *handle)
{
    UART_TransferHandleIRQ(handle->baseReg,
                           (uart_handle_t *)&((uart_status_struct_t *)handle->serialPortHandle)->uartHandle);
   /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping 
     exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}
