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
 * FreeMASTER Communication Driver - msCAN low-level driver
 */

#include "freemaster.h"
#include "freemaster_private.h"

/* Numeric identifier to help pre-processor to identify whether our driver is used or not. */
#define FMSTR_CAN_MCUX_MSCAN_ID 1

#if (FMSTR_MK_IDSTR(FMSTR_CAN_DRV) == FMSTR_CAN_MCUX_MSCAN_ID) 

#include "freemaster_mscan.h"

#if (!(FMSTR_DISABLE))

#include "freemaster_can.h"
#include "fsl_mscan.h"

/******************************************************************************
* Local macros
******************************************************************************/

/******************************************************************************
* Local functions
******************************************************************************/

static FMSTR_BOOL _FMSTR_MsCAN_Init(FMSTR_U32 idRx, FMSTR_U32 idTx);  /* Initialize CAN module on a given base address. */
static void _FMSTR_MsCAN_EnableTxInterrupt(FMSTR_BOOL enable);     /* Enable CAN Transmit interrupt. */
static void _FMSTR_MsCAN_EnableRxInterrupt(FMSTR_BOOL enable);     /* Enable CAN Receive interrupt. */
static void _FMSTR_MsCAN_EnableRx(void);                           /* Enable/re-initialize Receiver buffer. */
static FMSTR_SIZE8 _FMSTR_MsCAN_GetRxFrameLen(void);               /* Return size of received CAN frame, or 0 if no Rx frame is available. */
static FMSTR_BCHR _FMSTR_MsCAN_GetRxFrameByte(FMSTR_SIZE8 index);  /* Get data byte at index (0..8). */
static void _FMSTR_MsCAN_AckRxFrame(void);                         /* Discard received frame and enable receiving a next one. */
static FMSTR_BOOL _FMSTR_MsCAN_PrepareTxFrame(void);               /* Initialize transmit buffer; return false when Tx buffer is not available. */
static void _FMSTR_MsCAN_PutTxFrameByte(FMSTR_SIZE8 index, FMSTR_BCHR data);   /* Fill one byte of transmit data. */
static void _FMSTR_MsCAN_SendTxFrame(FMSTR_SIZE8 len);             /* Send the Tx buffer. */
/******************************************************************************
* Local variables
******************************************************************************/

/* Serial base address */
#ifdef FMSTR_CAN_BASE
    static CAN_Type *fmstr_canBaseAddr = FMSTR_CAN_BASE;
#else
    static CAN_Type *fmstr_canBaseAddr = NULL;
#endif

/******************************************************************************
* Driver interface
******************************************************************************/

const FMSTR_CAN_DRV_INTF FMSTR_CAN_MCUX_MSCAN =
{
    .Init = _FMSTR_MsCAN_Init,
    .EnableTxInterrupt = _FMSTR_MsCAN_EnableTxInterrupt,
    .EnableRxInterrupt = _FMSTR_MsCAN_EnableRxInterrupt,
    .EnableRx =        _FMSTR_MsCAN_EnableRx,
    .GetRxFrameLen =   _FMSTR_MsCAN_GetRxFrameLen,
    .GetRxFrameByte =  _FMSTR_MsCAN_GetRxFrameByte,
    .AckRxFrame =      _FMSTR_MsCAN_AckRxFrame,
    .PrepareTxFrame =  _FMSTR_MsCAN_PrepareTxFrame,
    .PutTxFrameByte =  _FMSTR_MsCAN_PutTxFrameByte,
    .SendTxFrame =     _FMSTR_MsCAN_SendTxFrame,
};

/******************************************************************************
* Implementation
******************************************************************************/

static FMSTR_BOOL _FMSTR_MsCAN_Init(FMSTR_U32 idRx, FMSTR_U32 idTx)
{

}

static void _FMSTR_MsCAN_EnableTxInterrupt(FMSTR_BOOL enable)
{

}

static void _FMSTR_MsCAN_EnableRxInterrupt(FMSTR_BOOL enable)
{

}

static void _FMSTR_MsCAN_EnableRx(void)
{

}

static FMSTR_SIZE8 _FMSTR_MsCAN_GetRxFrameLen(void)
{

}

static FMSTR_BCHR _FMSTR_MsCAN_GetRxFrameByte(FMSTR_SIZE8 index)
{

}

static void _FMSTR_MsCAN_AckRxFrame(void)
{

}

static FMSTR_BOOL _FMSTR_MsCAN_PrepareTxFrame(void)
{

}

static void _FMSTR_MsCAN_PutTxFrameByte(FMSTR_SIZE8 index, FMSTR_BCHR data)
{

}

static void _FMSTR_MsCAN_SendTxFrame(FMSTR_SIZE8 len)
{

}

#if FMSTR_LONG_INTR || FMSTR_SHORT_INTR
void FMSTR_CanIsr(void)
{
    
}
#endif /* FMSTR_LONG_INTR || FMSTR_SHORT_INTR */

/**************************************************************************//*!
*
* @brief    Assigning FreeMASTER communication module base address
*
******************************************************************************/
void FMSTR_CanSetBaseAddress(CAN_Type *base)
{
    fmstr_canBaseAddr = base;
}

#if FMSTR_LONG_INTR || FMSTR_SHORT_INTR
void FMSTR_CanIsr(void)
{

}
#endif

#else /* (!(FMSTR_DISABLE)) */

void FMSTR_CanSetBaseAddress(CAN_Type *base)
{
    FMSTR_UNUSED(base);
}

#if FMSTR_LONG_INTR || FMSTR_SHORT_INTR
void FMSTR_CanIsr(void)
{
}
#endif /* FMSTR_LONG_INTR || FMSTR_SHORT_INTR */

#endif /* (!(FMSTR_DISABLE)) */
#endif /* (FMSTR_MK_IDSTR(FMSTR_CAN_DRV) == FMSTR_CAN_MCUX_MSCAN_ID) */
