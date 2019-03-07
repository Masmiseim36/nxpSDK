/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
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

#include "fsl_common.h"
#include "board.h"
#include "usb_uart_drv.h"
extern void LPUART_HandleIRQ(LPUART_Type *base, lpuart_handle_t *handle);


void USB_UartInit(USB_UartType *base, const usb_uartConfiguration *config, uint32_t sourceClockHz)
{
    LPUART_Init(base, config, sourceClockHz);
}


void USB_UartGetDefaultConfiguratoion(usb_uartConfiguration *configuration)
{
    LPUART_GetDefaultConfig(configuration);
}


status_t USB_UartReceiveDataIRQ(USB_UartType *base, usb_uart_handle_t *handle, usb_xfer_t *xfer, size_t *receivedBytes)
{
    return LPUART_TransferReceiveNonBlocking(base, (lpuart_handle_t *)handle, (lpuart_transfer_t *)xfer, receivedBytes);
}


void USB_UartSendDataPolling(USB_UartType *base, const uint8_t *buffer, uint32_t length)
{
    LPUART_WriteBlocking(base, buffer, length);
}


status_t USB_UartSendNonBlocking(USB_UartType *base, usb_uart_handle_t *handle, usb_xfer_t *xfer)
{
    return LPUART_TransferSendNonBlocking(base, handle, (lpuart_transfer_t *)xfer);
}


void USB_UartDeinit(USB_UartType *base)
{
    LPUART_Deinit(base);
}
void USB_UartCreateHandle(USB_UartType *base,
                          usb_uart_handle_t *handle,
                          usb_uart_rx_callback_t callback,
                          void *userData)
{
    LPUART_TransferCreateHandle(base, (lpuart_handle_t *)handle, callback, userData);
}

void USB_UartIRQHandler(USB_UartType *base, usb_uart_handle_t *handle)
{
    LPUART_TransferHandleIRQ(base, (lpuart_handle_t *)handle);
}
