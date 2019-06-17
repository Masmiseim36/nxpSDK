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
 * FreeMASTER Communication Driver - MINIUSART low-level driver
 */

#include "freemaster.h"
#include "freemaster_private.h"

/* Numeric identifier to help pre-processor to identify whether our driver is used or not. */
#define FMSTR_SERIAL_MCUX_MINIUSART_ID 1

#if (FMSTR_MK_IDSTR(FMSTR_SERIAL_DRV) == FMSTR_SERIAL_MCUX_MINIUSART_ID) 

#include "freemaster_serial_miniusart.h"

#if (!(FMSTR_DISABLE))

#include "fsl_common.h"
#include "fsl_usart.h"

#include "freemaster_protocol.h"
#include "freemaster_serial.h"

/***********************************
*  local variables
***********************************/

/* Serial base address */
#ifdef FMSTR_SERIAL_BASE
    static USART_Type *fmstr_serialBaseAddr = FMSTR_SERIAL_BASE;
#else
    static USART_Type *fmstr_serialBaseAddr = NULL;
#endif

/***********************************
*  local function prototypes
***********************************/

/* Interface function - Initialization of serial UART driver adapter */
static FMSTR_BOOL _FMSTR_SerialMiniUsartInit(void);
static void _FMSTR_SerialMiniUsartEnableTransmit(FMSTR_BOOL enable);
static void _FMSTR_SerialMiniUsartEnableReceive(FMSTR_BOOL enable);
static void _FMSTR_SerialMiniUsartEnableTransmitInterrupt(FMSTR_BOOL enable);
static void _FMSTR_SerialMiniUsartEnableReceiveInterrupt(FMSTR_BOOL enable);
static FMSTR_BOOL _FMSTR_SerialMiniUsartIsTransmitRegEmpty(void);
static FMSTR_BOOL _FMSTR_SerialMiniUsartIsReceiveRegFull(void);
static FMSTR_BOOL _FMSTR_SerialMiniUsartIsTransmitterActive(void);
static void _FMSTR_SerialMiniUsartPutChar(FMSTR_BCHR  ch);
static FMSTR_BCHR _FMSTR_SerialMiniUsartGetChar(void);
static void _FMSTR_SerialMiniUsartFlush(void);

/***********************************
*  global variables
***********************************/
/* Interface of this serial UART driver */
const FMSTR_SERIAL_DRV_INTF FMSTR_SERIAL_MCUX_MINIUSART =
{
    .Init                       = _FMSTR_SerialMiniUsartInit,
    .EnableTransmit             = _FMSTR_SerialMiniUsartEnableTransmit,
    .EnableReceive              = _FMSTR_SerialMiniUsartEnableReceive,
    .EnableTransmitInterrupt    = _FMSTR_SerialMiniUsartEnableTransmitInterrupt,
    .EnableReceiveInterrupt     = _FMSTR_SerialMiniUsartEnableReceiveInterrupt,
    .IsTransmitRegEmpty         = _FMSTR_SerialMiniUsartIsTransmitRegEmpty,
    .IsReceiveRegFull           = _FMSTR_SerialMiniUsartIsReceiveRegFull,
    .IsTransmitterActive        = _FMSTR_SerialMiniUsartIsTransmitterActive,
    .PutChar                    = _FMSTR_SerialMiniUsartPutChar,
    .GetChar                    = _FMSTR_SerialMiniUsartGetChar,
    .Flush                      = _FMSTR_SerialMiniUsartFlush,

};

/**************************************************************************//*!
*
* @brief    Serial communication initialization
*
******************************************************************************/
static FMSTR_BOOL _FMSTR_SerialMiniUsartInit(void)
{
    /* valid runtime module address must be assigned */
    if(!fmstr_serialBaseAddr)
        return FMSTR_FALSE;

    return FMSTR_TRUE;
}

/**************************************************************************//*!
*
* @brief    Enable/Disable Serial transmitter
*
******************************************************************************/

static void _FMSTR_SerialMiniUsartEnableTransmit(FMSTR_BOOL enable)
{
    USART_EnableTx(fmstr_serialBaseAddr, enable);
}

/**************************************************************************//*!
*
* @brief    Enable/Disable Serial receiver
*
******************************************************************************/

static void _FMSTR_SerialMiniUsartEnableReceive(FMSTR_BOOL enable)
{
    USART_EnableRx(fmstr_serialBaseAddr, enable);
}

/**************************************************************************//*!
*
* @brief    Enable/Disable interrupt from transmit register empty event
*
******************************************************************************/

static void _FMSTR_SerialMiniUsartEnableTransmitInterrupt(FMSTR_BOOL enable)
{
    if(enable)
        USART_EnableInterrupts(fmstr_serialBaseAddr, kUSART_TxReadyInterruptEnable);
    else
        USART_DisableInterrupts(fmstr_serialBaseAddr, kUSART_TxReadyInterruptEnable);

}

/**************************************************************************//*!
*
* @brief    Enable/Disable interrupt from receive register full event
*
******************************************************************************/

static void _FMSTR_SerialMiniUsartEnableReceiveInterrupt(FMSTR_BOOL enable)
{
     if(enable)
        USART_EnableInterrupts(fmstr_serialBaseAddr, kUSART_RxReadyInterruptEnable);
    else
        USART_DisableInterrupts(fmstr_serialBaseAddr, kUSART_RxReadyInterruptEnable);
}

/**************************************************************************//*!
*
* @brief    Returns TRUE if the transmit register is empty, and it's possible to put next char
*
******************************************************************************/

static FMSTR_BOOL _FMSTR_SerialMiniUsartIsTransmitRegEmpty(void)
{
    uint32_t sr = USART_GetStatusFlags(fmstr_serialBaseAddr);

    return (sr & kUSART_TxReady);
}

/**************************************************************************//*!
*
* @brief    Returns TRUE if the receive register is full, and it's possible to get received char
*
******************************************************************************/

static FMSTR_BOOL _FMSTR_SerialMiniUsartIsReceiveRegFull(void)
{
    uint32_t sr = USART_GetStatusFlags(fmstr_serialBaseAddr);

    return (sr & kUSART_RxReady);
}

/**************************************************************************//*!
*
* @brief    Returns TRUE if the transmitter is still active
*
******************************************************************************/

static FMSTR_BOOL _FMSTR_SerialMiniUsartIsTransmitterActive(void)
{
    uint32_t sr = USART_GetStatusFlags(fmstr_serialBaseAddr);

    return !(sr & kUSART_TxIdleFlag);
}

/**************************************************************************//*!
*
* @brief    The function puts the char for transmit
*
******************************************************************************/

static void _FMSTR_SerialMiniUsartPutChar(FMSTR_BCHR  ch)
{
    USART_WriteByte(fmstr_serialBaseAddr, ch);
}

/**************************************************************************//*!
*
* @brief    The function gets the received char
*
******************************************************************************/
static FMSTR_BCHR _FMSTR_SerialMiniUsartGetChar(void)
{
    return USART_ReadByte(fmstr_serialBaseAddr);
}

/**************************************************************************//*!
*
* @brief    The function sends buffered data
*
******************************************************************************/
static void _FMSTR_SerialMiniUsartFlush(void)
{

}

/**************************************************************************//*!
*
* @brief    Assigning FreeMASTER communication module base address
*
******************************************************************************/
void FMSTR_SerialSetBaseAddress(USART_Type *base)
{
    fmstr_serialBaseAddr = base;
}

/**************************************************************************//*!
*
* @brief    API: Interrupt handler call
*
* This Interrupt Service Routine handles the UART  interrupts for the FreeMASTER
* driver. In case you want to handle the interrupt in the application yourselves,
* call the FMSTR_ProcessSerial function which
* does the same job but is not compiled as an Interrupt Service Routine.
*
* In poll-driven mode (FMSTR_POLL_DRIVEN) this function does nothing.
*
******************************************************************************/

void FMSTR_SerialIsr()
{
    /* process incomming or just transmitted byte */
    #if (FMSTR_LONG_INTR) || (FMSTR_SHORT_INTR)
        FMSTR_ProcessSerial();
    #endif
}

#else /* (!(FMSTR_DISABLE)) */

void FMSTR_SerialSetBaseAddress(USART_Type *base)
{
    FMSTR_UNUSED(base);
}

void FMSTR_SerialIsr(void)
{
}

#endif /* (!(FMSTR_DISABLE)) */
#endif /* (FMSTR_MK_IDSTR(FMSTR_SERIAL_DRV) == FMSTR_SERIAL_MCUX_MINIUSART_ID) */
