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

#ifndef __USB_UART_H__
#define __USB_UART_H__
#include "fsl_device_registers.h"
#if ((BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_UART) || \
     (BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_IUART))
#include "fsl_uart.h"
#elif(BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_LPUART)
#include "fsl_lpuart.h"
#elif(BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_LPSCI)
#include "fsl_lpsci.h"
#elif(BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_FLEXCOMM)
#include "fsl_usart.h"
#endif

#if ((BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_UART) || \
     (BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_IUART))
#define USB_UART_ParityDisabled kUART_ParityDisabled
#define USB_UART_ParityEven kUART_ParityEven
#define USB_UART_ParityOdd kUART_ParityOdd
#elif(BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_LPUART)
#define USB_UART_ParityDisabled kLPUART_ParityDisabled
#define USB_UART_ParityEven kLPUART_ParityEven
#define USB_UART_ParityOdd kLPUART_ParityOdd
#elif(BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_LPSCI)
#define USB_UART_ParityDisabled kLPSCI_ParityDisabled
#define USB_UART_ParityEven kLPSCI_ParityEven
#define USB_UART_ParityOdd kLPSCI_ParityOdd
#elif(BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_FLEXCOMM)
#define USB_UART_ParityDisabled kUSART_ParityDisabled
#define USB_UART_ParityEven kUSART_ParityEven
#define USB_UART_ParityOdd kUSART_ParityOdd
#endif

/*! @brief UART number of bits in a character*/
#if ((BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_UART) || \
     (BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_IUART))
#define USB_UART_8BitsPerChar kUART_8BitsPerChar
#define USB_UART_9BitsPerChar kUART_9BitsPerChar
#elif(BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_LPUART)
#define USB_UART_8BitsPerChar kLPUART_8BitsPerChar
#define USB_UART_9BitsPerChar kLPUART_9BitsPerChar
#define USB_UART_10BitsPerChar kLPUART_10BitsPerChar
#elif(BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_LPSCI)
#define USB_UART_8BitsPerChar kLPSCI_8BitsPerChar
#define USB_UART_9BitsPerChar kLPSCI_9BitsPerChar
#elif(BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_FLEXCOMM)
#define USB_UART_8BitsPerChar kUSART_8BitsPerChar
#define USB_UART_9BitsPerChar kUSART_9BitsPerChar
#endif

/*! @brief UART number of stop bits*/
#if ((BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_UART) || \
     (BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_IUART))
#define USB_UART_OneStopBit kUART_OneStopBit
#define USB_UART_TwoStopBit kUART_TwoStopBit
#elif(BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_LPUART)
#define USB_UART_OneStopBit kLPUART_OneStopBit
#define USB_UART_TwoStopBit kLPUART_TwoStopBit
#elif(BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_LPSCI)
#define USB_UART_OneStopBit kLPSCI_OneStopBit
#define USB_UART_TwoStopBit kLPSCI_TwoStopBit
#elif(BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_FLEXCOMM)
#define USB_UART_OneStopBit kUSART_OneStopBit
#define USB_UART_TwoStopBit kUSART_TwoStopBit
#endif

#if ((BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_UART) || \
     (BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_IUART))
typedef uart_handle_t usb_uart_handle_t;
#elif(BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_LPUART)
typedef lpuart_handle_t usb_uart_handle_t;
#elif(BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_LPSCI)
typedef lpsci_handle_t usb_uart_handle_t;
#elif(BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_FLEXCOMM)
typedef usart_handle_t usb_uart_handle_t;
#endif

typedef struct _usb_xfer
{
    uint8_t *buffer; /*!< The buffer of data to be transfer.*/
    size_t size;     /*!< The byte count to be transfer. */
} usb_xfer_t;

#if (BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_UART)
/*! @brief Error codes for the UART driver. */
typedef enum _usb_uart_status
{
    kStatus_USB_UART_TxBusy = MAKE_STATUS(kStatusGroup_UART, 0),
    kStatus_USB_UART_RxBusy = MAKE_STATUS(kStatusGroup_UART, 1),
    kStatus_USB_UART_TxIdle = MAKE_STATUS(kStatusGroup_UART, 2),
    kStatus_USB_UART_RxIdle = MAKE_STATUS(kStatusGroup_UART, 3),
    kStatus_USB_UART_TxWatermarkTooLarge = MAKE_STATUS(kStatusGroup_UART, 4),
    kStatus_USB_UART_RxWatermarkTooLarge = MAKE_STATUS(kStatusGroup_UART, 5),
    kStatus_USB_UART_kStatus_UART_FlagCannotClearManually = MAKE_STATUS(kStatusGroup_UART, 6),
    kStatus_USB_UART_Error = MAKE_STATUS(kStatusGroup_UART, 7),
    kStatus_USB_UART_RxRingBufferOverrun = MAKE_STATUS(kStatusGroup_UART, 8),
    kStatus_USB_UART_RxHardwareOverrun = MAKE_STATUS(kStatusGroup_UART, 9),
} usb_uart_status_t;
#elif(BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_IUART)
/*! @brief Error codes for the UART driver. */
typedef enum _usb_uart_status
{
    kStatus_USB_UART_TxBusy = MAKE_STATUS(kStatusGroup_IUART, 0),
    kStatus_USB_UART_RxBusy = MAKE_STATUS(kStatusGroup_IUART, 1),
    kStatus_USB_UART_TxIdle = MAKE_STATUS(kStatusGroup_IUART, 2),
    kStatus_USB_UART_RxIdle = MAKE_STATUS(kStatusGroup_IUART, 3),
    kStatus_USB_UART_TxWatermarkTooLarge = MAKE_STATUS(kStatusGroup_IUART, 4),
    kStatus_USB_UART_RxWatermarkTooLarge = MAKE_STATUS(kStatusGroup_IUART, 5),
    kStatus_USB_UART_kStatus_UART_FlagCannotClearManually = MAKE_STATUS(kStatusGroup_IUART, 6),
    kStatus_USB_UART_Error = MAKE_STATUS(kStatusGroup_IUART, 7),
    kStatus_USB_UART_RxRingBufferOverrun = MAKE_STATUS(kStatusGroup_IUART, 8),
    kStatus_USB_UART_RxHardwareOverrun = MAKE_STATUS(kStatusGroup_IUART, 9),
} usb_uart_status_t;
#elif(BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_LPUART)
/*! @brief Error codes for the UART driver. */
typedef enum _usb_uart_status
{
    kStatus_USB_UART_TxBusy = MAKE_STATUS(kStatusGroup_LPUART, 0),
    kStatus_USB_UART_RxBusy = MAKE_STATUS(kStatusGroup_LPUART, 1),
    kStatus_USB_UART_TxIdle = MAKE_STATUS(kStatusGroup_LPUART, 2),
    kStatus_USB_UART_RxIdle = MAKE_STATUS(kStatusGroup_LPUART, 3),
    kStatus_USB_UART_TxWatermarkTooLarge = MAKE_STATUS(kStatusGroup_LPUART, 4),
    kStatus_USB_UART_RxWatermarkTooLarge = MAKE_STATUS(kStatusGroup_LPUART, 5),
    kStatus_USB_UART_kStatus_UART_FlagCannotClearManually = MAKE_STATUS(kStatusGroup_LPUART, 6),
    kStatus_USB_UART_Error = MAKE_STATUS(kStatusGroup_LPUART, 7),
    kStatus_USB_UART_RxRingBufferOverrun = MAKE_STATUS(kStatusGroup_LPUART, 8),
    kStatus_USB_UART_RxHardwareOverrun = MAKE_STATUS(kStatusGroup_LPUART, 9),

} usb_uart_status_t;
#elif(BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_LPSCI)
/*! @brief Error codes for the UART driver. */
typedef enum _usb_uart_status
{
    kStatus_USB_UART_TxBusy = MAKE_STATUS(kStatusGroup_LPSCI, 0),
    kStatus_USB_UART_RxBusy = MAKE_STATUS(kStatusGroup_LPSCI, 1),
    kStatus_USB_UART_TxIdle = MAKE_STATUS(kStatusGroup_LPSCI, 2),
    kStatus_USB_UART_RxIdle = MAKE_STATUS(kStatusGroup_LPSCI, 3),
    kStatus_USB_UART_TxWatermarkTooLarge = MAKE_STATUS(kStatusGroup_LPSCI, 4),
    kStatus_USB_UART_RxWatermarkTooLarge = MAKE_STATUS(kStatusGroup_LPSCI, 5),
    kStatus_USB_UART_kStatus_UART_FlagCannotClearManually = MAKE_STATUS(kStatusGroup_LPSCI, 6),
    kStatus_USB_UART_Error = MAKE_STATUS(kStatusGroup_LPSCI, 7),
    kStatus_USB_UART_RxRingBufferOverrun = MAKE_STATUS(kStatusGroup_LPSCI, 8),
    kStatus_USB_UART_RxHardwareOverrun = MAKE_STATUS(kStatusGroup_LPSCI, 9),

} usb_uart_status_t;
#elif(BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_FLEXCOMM)
/*! @brief Error codes for the UART driver. */
typedef enum _usb_uart_status
{
    kStatus_USB_UART_TxBusy = MAKE_STATUS(kStatusGroup_LPC_USART, 0),
    kStatus_USB_UART_RxBusy = MAKE_STATUS(kStatusGroup_LPC_USART, 1),
    kStatus_USB_UART_TxIdle = MAKE_STATUS(kStatusGroup_LPC_USART, 2),
    kStatus_USB_UART_RxIdle = MAKE_STATUS(kStatusGroup_LPC_USART, 3),
    kStatus_USB_UART_RxRingBufferOverrun = MAKE_STATUS(kStatusGroup_LPC_USART, 8),
    kStatus_USB_UART_NoiseError = MAKE_STATUS(kStatusGroup_LPC_USART, 10),
    kStatus_USB_UART_FramingError = MAKE_STATUS(kStatusGroup_LPC_USART, 11),
    kStatus_USB_UART_ParityError = MAKE_STATUS(kStatusGroup_LPC_USART, 12),
    kStatus_USB_UART_BaudrateNotSupport = MAKE_STATUS(kStatusGroup_LPC_USART, 13),
} usb_uart_status_t;
#endif

/*! @brief UART receive callback function type */
#if ((BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_UART) || \
     (BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_IUART))
typedef UART_Type USB_UartType;
typedef uart_config_t usb_uartConfiguration;
typedef void (*usb_uart_rx_callback_t)(USB_UartType *base, usb_uart_handle_t *handle, status_t status, void *userData);
#elif(BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_LPUART)
typedef LPUART_Type USB_UartType;
typedef lpuart_config_t usb_uartConfiguration;
typedef void (*usb_uart_rx_callback_t)(USB_UartType *base, usb_uart_handle_t *handle, status_t status, void *userData);
#elif(BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_LPSCI)
typedef UART0_Type USB_UartType;
typedef lpsci_config_t usb_uartConfiguration;
typedef void (*usb_uart_rx_callback_t)(USB_UartType *base, usb_uart_handle_t *handle, status_t status, void *userData);
#elif(BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_FLEXCOMM)
typedef USART_Type USB_UartType;
typedef usart_config_t usb_uartConfiguration;
typedef void (*usb_uart_rx_callback_t)(USB_UartType *base, usb_uart_handle_t *handle, status_t status, void *userData);
#endif

/*******************************************************************************
 * API
 ******************************************************************************/
/*The function in this head file is just a wrapper , it will call the uart, lpuart or lpsci corresponding function.*/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initializes UART configure structure.
 *
 * This function initializes the UART configure structure to default value.
 *
 * @param configuration Pointer to master configuration structure.
 * @see UART_Configure
 */
void USB_UartGetDefaultConfiguratoion(usb_uartConfiguration *configuration);

/*!
 * @brief UART IRQ handler function.
 *
 * This function process the UART transmit and receive IRQ requestion.
 *
 * @param base The UART base point.
 * @param handle UART handle pointer.
 */
void USB_UartIRQHandler(USB_UartType *base, usb_uart_handle_t *handle);
/*!
 * @brief Initializes a UART instance.
 *
 * This function initializes the UART module with user-defined settings.
 * This example shows how to set up the uart_state_t and the
 * uart_config_t parameters and how to call the UART_Configure function.
 *
 *
 * @param base The UART base point.
 * @param config The user configuration structure of type usb_uart_config_t. The user
 *  populates the members of this structure and passes the pointer of this structure
 *  to this function.
 * @param sourceClockHz UART clock source frequency in Hz.
 * @return 0 succeed; Others failed.
 */
void USB_UartInit(USB_UartType *base, const usb_uartConfiguration *config, uint32_t sourceClockHz);

/*!
 * @brief Sends data using polling.
 *
 * This function sends data using polling. This is a blocking function, which does not return
 * until all data have been sent out.
 *
 * @param base The UART base point.
 * @param buffer buff for send data.
 * @param length length to send.
 */
void USB_UartSendDataPolling(USB_UartType *base, const uint8_t *buffer, uint32_t length);

/*!
 * @brief Transmits a buffer of data using the interrupt method.
 *
 * This function sends data using an interrupt method. This is a non-blocking function, which
 * returns without waiting for all data to be written to the TX register. When
 * all data is written to the TX register in the ISR, the UART driver calls the callback
 * function and passes @ref kStatus_UART_TxIdle as status parameter.
 *
 * @note The kStatus_UART_TxIdle is passed to the upper layer when all data written
 * to TX register, but does not ensure that all the data sent out. So before disabling the TX,
 * check the kUART_TransmissionCompleteFlag to ensure that the TX is finished.
 *
 * @param base UART peripheral base address.
 * @param handle UART handle pointer.
 * @param xfer UART transfer structure; See the #uart_transfer_t.
 * @retval kStatus_Success Successfully starts the data transmission.
 * @retval kStatus_UART_TxBusy Previous transmission still not finished, data not all written to TX register yet.
 */
status_t USB_UartSendNonBlocking(USB_UartType *base, usb_uart_handle_t *handle, usb_xfer_t *xfer);

/*!
 * @brief Receives data using IRQ.
 *
 * This function receives data using IRQ. This is a non-blocking function, which returns
 * right away. When all data is received, the receive callback function is called.
 *
 * @param base The UART base point.
 * @param handle UART handle.
 * @param xfer UART transfer structure; See the #usb_xfer_t.
 * @return kUART_Succeed succeed, others failed; See the #usb_xfer_status_t.
 */
status_t USB_UartReceiveDataIRQ(USB_UartType *base, usb_uart_handle_t *handle, usb_xfer_t *xfer, size_t *receivedBytes);

/*!
 * @brief Sets the IRQ transfer callback.
 *
 * This function sets the IRQ transfer callback.
 *
 * @param state UART state structure.
 * @param callback UART callback.
 * @param param callback function parameter.
 */
void USB_UartSetCallback(usb_uart_handle_t *state, usb_uart_rx_callback_t callback, void *param);
/*!
 * @brief Deinitializes an UART instance.
 *
 * This function gates the UART module clock and sets all register values to reset values.
 *
 * @param base The UART base point.
 */
void USB_UartDeinit(USB_UartType *base);
/*!
 * @brief Initializes the UART handle.
 *
 * This function initializes the UART handle, which can be used for other UART transactional APIs.  Usually, for a
 * specified UART instance, call this API once to get the initialized handle.
 *
 * @param base UART peripheral base address.
 * @param handle UART handle pointer.
 * @param callback The callback function.
 * @param userData The parameter of the callback function.
 */
void USB_UartCreateHandle(USB_UartType *base,
                          usb_uart_handle_t *handle,
                          usb_uart_rx_callback_t callback,
                          void *userData);

/*!
 * @brief Gets the sent data status which is using IRQ.
 *
 * This function gets the sent data status which using IRQ.
 *
 * @param base The UART base point.
 * @param state UART state structure. See the #usb_uart_handle_t.
 * @param remaingBytes The bytes remaining not send out.
 * @return kUART_Succeed Succeed, others failed; See the #usb_xfer_status_t.
 */
usb_uart_status_t USB_UART_DRV_GetTransmitStatus(USB_UartType *base, usb_uart_handle_t *state, uint32_t *remaingBytes);

#if defined(__cplusplus)
}
#endif

#endif /* __USB_UART_H__*/
