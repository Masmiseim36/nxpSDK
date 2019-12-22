/*
 * Copyright (c) 2007-2015 Freescale Semiconductor, Inc.
 * Copyright 2018-2019 NXP
 *
 * License: NXP LA_OPT_NXP_Software_License
 *
 * NXP Confidential. This software is owned or controlled by NXP and may
 * only be used strictly in accordance with the applicable license terms.
 * By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that
 * you have read, and that you agree to comply with and are bound by,
 * such license terms.  If you do not agree to be bound by the applicable
 * license terms, then you may not retain, install, activate or otherwise
 * use the software.  This code may only be used in a microprocessor,
 * microcontroller, sensor or digital signal processor ("NXP Product")
 * supplied directly or indirectly from NXP.  See the full NXP Software
 * License Agreement in license/LA_OPT_NXP_Software_License.pdf
 *
 * FreeMASTER Communication Driver - low-level driver hooking the CDC class driver in USB stack
 */

#include "freemaster.h"
#include "freemaster_private.h"
#include "freemaster_utils.h"

/* Numeric identifier to help pre-processor to identify whether our driver is used or not. */
#define FMSTR_SERIAL_MCUX_USB_ID 1

#if (FMSTR_MK_IDSTR(FMSTR_SERIAL_DRV) == FMSTR_SERIAL_MCUX_USB_ID)

#include "freemaster_serial_usb.h"

#if (!(FMSTR_DISABLE))

#include "freemaster_protocol.h"
#include "freemaster_serial.h"

/* Include all those USB header files to get proper size of the USB buffer */
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"
#include "usb_device_class.h"
#include "freemaster_usb_device_descriptor.h"
#include "freemaster_usb.h"

/* Make the buffers size by USB bulk endpoint size */
#define FMSTR_USB_BUFFER_SIZE USB_DATA_BUFF_SIZE

/******************************************************************************
* Adapter configuration
******************************************************************************/
#if defined(FMSTR_SHORT_INTR) && FMSTR_SHORT_INTR
    #error The USB/CDC driver does not support short interrupt.
#endif

#if FMSTR_SERIAL_SINGLEWIRE
    #error The USB/CDC driver does not support single wire configuration of UART communication.
#endif

/******************************************************************************
* Local functions
******************************************************************************/

static FMSTR_BOOL _FMSTR_SerialUsbInit(void);
static void _FMSTR_SerialUsbEnableTx(FMSTR_BOOL enable);
static void _FMSTR_SerialUsbEnableRx(FMSTR_BOOL enable);
static void _FMSTR_SerialUsbEnableTxInterrupt(FMSTR_BOOL enable);
static void _FMSTR_SerialUsbEnableRxInterrupt(FMSTR_BOOL enable);
static FMSTR_BOOL _FMSTR_SerialUsbIsTransmitRegEmpty(void);
static FMSTR_BOOL _FMSTR_SerialUsbIsReceiveRegFull(void);
static FMSTR_BOOL _FMSTR_SerialUsbIsTransmitterActive(void);
static void _FMSTR_SerialUsbPutChar(FMSTR_BCHR  ch);
static FMSTR_BCHR _FMSTR_SerialUsbGetChar(void);
static void _FMSTR_SerialUsbFlush(void);
/******************************************************************************
* Type definitions
******************************************************************************/

/* Context structure of USB/CDC interface */
typedef struct FMSTR_USB_CTX_S
{
    volatile FMSTR_BOOL     txEnabled;                              /* Transmit is enabled */
    FMSTR_BCHR              txBuffer[FMSTR_USB_BUFFER_SIZE];        /* Transmit buffer */
    FMSTR_BCHR              rxBuffer[FMSTR_USB_BUFFER_SIZE];        /* Receive buffer (It is for ring buffer object) */
    volatile uint32_t       txSize;                                 /* Data size in transmit buffer */
    volatile FMSTR_BOOL     txActive;                               /* Transmit status (true = transmitting) */
    volatile FMSTR_BOOL     rxEnabled;                              /* Receiver enabled */

    FMSTR_RING_BUFFER       rxQueue;                                /* Receive ring buffer object */
} FMSTR_USB_CTX;

/******************************************************************************
* Local variables
******************************************************************************/
/* Context data of USB/CDC interface */
static FMSTR_USB_CTX  fmstr_usbCtx;
/* Transmit function for USB/CDC interface */
static FMSTR_TX_FUNC fmstr_txFunc = NULL;
/******************************************************************************
* Driver interface
******************************************************************************/

const FMSTR_SERIAL_DRV_INTF FMSTR_SERIAL_MCUX_USB =
{
    .Init = _FMSTR_SerialUsbInit,
    .EnableTransmit = _FMSTR_SerialUsbEnableTx,
    .EnableReceive = _FMSTR_SerialUsbEnableRx,
    .EnableTransmitInterrupt = _FMSTR_SerialUsbEnableTxInterrupt,
    .EnableReceiveInterrupt = _FMSTR_SerialUsbEnableRxInterrupt,
    .IsTransmitRegEmpty = _FMSTR_SerialUsbIsTransmitRegEmpty,
    .IsReceiveRegFull = _FMSTR_SerialUsbIsReceiveRegFull,
    .IsTransmitterActive = _FMSTR_SerialUsbIsTransmitterActive,
    .PutChar = _FMSTR_SerialUsbPutChar,
    .GetChar = _FMSTR_SerialUsbGetChar,
    .Flush = _FMSTR_SerialUsbFlush,
};

/**************************************************************************//*!
*
* @brief    Serial communication initialization
*
******************************************************************************/

static FMSTR_BOOL _FMSTR_SerialUsbInit(void)
{
    FMSTR_MemSet(&fmstr_usbCtx, 0, sizeof(fmstr_usbCtx));

    if(fmstr_txFunc == NULL)
        return FMSTR_FALSE;

    _FMSTR_RingBuffCreate(&fmstr_usbCtx.rxQueue, fmstr_usbCtx.rxBuffer, FMSTR_USB_BUFFER_SIZE);

    return FMSTR_TRUE;
}

/**************************************************************************//*!
*
* @brief    Enable/Disable Serial transmitter
*
******************************************************************************/

static void _FMSTR_SerialUsbEnableTx(FMSTR_BOOL enable)
{
    fmstr_usbCtx.txEnabled = enable;
}

/**************************************************************************//*!
*
* @brief    Enable/Disable Serial receiver
*
******************************************************************************/

static void _FMSTR_SerialUsbEnableRx(FMSTR_BOOL enable)
{
    fmstr_usbCtx.rxEnabled = enable;
}

/**************************************************************************//*!
*
* @brief    Enable/Disable interrupt from transmit register empty event
*
******************************************************************************/

static void _FMSTR_SerialUsbEnableTxInterrupt(FMSTR_BOOL enable)
{
}

/**************************************************************************//*!
*
* @brief    Enable/Disable interrupt from receive register full event
*
******************************************************************************/

static void _FMSTR_SerialUsbEnableRxInterrupt(FMSTR_BOOL enable)
{
}

/**************************************************************************//*!
*
* @brief    Returns TRUE if the transmit register is empty, and it's possible to put next char
*
******************************************************************************/

static FMSTR_BOOL _FMSTR_SerialUsbIsTransmitRegEmpty(void)
{
    /* Check if transmit is enabled or if it is sending data */
    if(!fmstr_usbCtx.txEnabled || fmstr_usbCtx.txActive)
        return FMSTR_FALSE;

    /* Check, if buffer is not full */
    if(fmstr_usbCtx.txSize < FMSTR_USB_BUFFER_SIZE)
        return FMSTR_TRUE;

    /* Send buffer is full -> flush data */
    _FMSTR_SerialUsbFlush();

    /* Check again, if buffer is not full */
    if(fmstr_usbCtx.txSize < FMSTR_USB_BUFFER_SIZE)
        return FMSTR_TRUE;

    /* Return false, when buffer is full and upper layer cannot call putChar function. */
    return FMSTR_FALSE;
}

/**************************************************************************//*!
*
* @brief    Returns TRUE if the receive register is full, and it's possible to get received char
*
******************************************************************************/

static FMSTR_BOOL _FMSTR_SerialUsbIsReceiveRegFull(void)
{
    if(fmstr_usbCtx.rxEnabled && _FMSTR_RingBuffHasData(&fmstr_usbCtx.rxQueue))
        return FMSTR_TRUE;

    return FMSTR_FALSE;
}

/**************************************************************************//*!
*
* @brief    Returns TRUE if the transmitter is still active
*
******************************************************************************/

static FMSTR_BOOL _FMSTR_SerialUsbIsTransmitterActive(void)
{
    return fmstr_usbCtx.txActive;
}

/**************************************************************************//*!
*
* @brief    The function puts the char for transmit
*
******************************************************************************/

static void _FMSTR_SerialUsbPutChar(FMSTR_BCHR  ch)
{
    /* Check free size in TX buffer */
    if(fmstr_usbCtx.txSize >= FMSTR_USB_BUFFER_SIZE)
        return;

    fmstr_usbCtx.txBuffer[fmstr_usbCtx.txSize] = ch;
    fmstr_usbCtx.txSize++;

    /* Send buffer is full -> flush data */
    if(fmstr_usbCtx.txSize == FMSTR_USB_BUFFER_SIZE)
        _FMSTR_SerialUsbFlush();
}

/**************************************************************************//*!
*
* @brief    The function gets the received char
*
******************************************************************************/

static FMSTR_BCHR _FMSTR_SerialUsbGetChar(void)
{
    /* Any data in receive buffer? */
    if(!_FMSTR_RingBuffHasData(&fmstr_usbCtx.rxQueue))
        return 0;

    return _FMSTR_RingBuffGet(&fmstr_usbCtx.rxQueue);
}

/**************************************************************************//*!
*
* @brief    The function send buffered data
*
******************************************************************************/

static void _FMSTR_SerialUsbFlush(void)
{
    if(fmstr_usbCtx.txSize <= 0)
        return;

    /* Send data */
    if(fmstr_txFunc != NULL)
    {
        fmstr_usbCtx.txActive = FMSTR_TRUE;
        if(!fmstr_txFunc(fmstr_usbCtx.txBuffer, fmstr_usbCtx.txSize))
            fmstr_usbCtx.txActive = FMSTR_FALSE;
    }
}

/**************************************************************************//*!
*
* @brief    Register read/write callbacks
*
******************************************************************************/

void FMSTR_SerialUsbRegisterAppFunctions(FMSTR_TX_FUNC tx_func)
{
    fmstr_txFunc = tx_func;
}

/**************************************************************************//*!
*
* @brief    Calling from USB driver when data was received/sent
*
******************************************************************************/

void FMSTR_SerialUsbProcessEvent(FMSTR_SERIAL_USBCDC_EVENT_TYPE type, FMSTR_U8 * data, FMSTR_U32 size)
{
    /* Data was sent */
    if(type == FMSTR_SERIAL_USBCDC_EVENT_TYPE_SENT)
    {
        if(fmstr_usbCtx.txActive)
        {
            fmstr_usbCtx.txSize = 0;
            fmstr_usbCtx.txActive = FMSTR_FALSE;
        }
    }
    /* Data received */
    else if(type == FMSTR_SERIAL_USBCDC_EVENT_TYPE_RECEIVED)
    {
        if(data != NULL)
        {
            for(int i=0; i<size; i++)
            {
                if(_FMSTR_RingBuffIsSpace(&fmstr_usbCtx.rxQueue))
                    _FMSTR_RingBuffPut(&fmstr_usbCtx.rxQueue, data[i]);
            }

            /* If the data are received by USB (ussually at once) it MUST be called Process serial twice
                to allow freemaster decode packat and also prepared the response and send it. */
            #if FMSTR_LONG_INTR
                FMSTR_ProcessSerial();
            #endif
        }
    }

#if FMSTR_LONG_INTR
    FMSTR_ProcessSerial();
#endif
}

#else /* (!(FMSTR_DISABLE)) */

void FMSTR_SerialUsbRegisterAppFunctions(FMSTR_TX_FUNC tx_func)
{
    FMSTR_UNUSED(tx_func);
}

void FMSTR_SerialUsbProcessEvent(FMSTR_SERIAL_USBCDC_EVENT_TYPE type, FMSTR_U8 * data, FMSTR_U32 size)
{
    FMSTR_UNUSED(type);
    FMSTR_UNUSED(data);
    FMSTR_UNUSED(size);
}

void FMSTR_SerialIsr(void)
{
}

#endif /* (!(FMSTR_DISABLE)) */
#endif /* (FMSTR_MK_IDSTR(FMSTR_SERIAL_DRV) == FMSTR_SERIAL_MCUX_USB_ID) */
