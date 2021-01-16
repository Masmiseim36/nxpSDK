/*
 * Copyright (c) 2007-2015 Freescale Semiconductor, Inc.
 * Copyright 2018-2020 NXP
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
 * FreeMASTER Communication Driver - CAN communication
 */

#ifndef __FREEMASTER_CAN_H
#define __FREEMASTER_CAN_H

/******************************************************************************
 * Required header files include check
 ******************************************************************************/
#ifndef __FREEMASTER_H
#error Please include the freemaster.h master header file before the freemaster_serial_uart.h
#endif

/******************************************************************************
 * Serial transport related configuration checks
 ******************************************************************************/

/* obsolete transport and driver macros */
#if defined(FMSTR_USE_CAN) || defined(FMSTR_USE_FLEXCAN) || defined(FMSTR_USE_FLEXCAN32) || \
    defined(FMSTR_USE_MSCAN) || defined(FMSTR_USE_MCAN)
#warning Obsolete configuration option detected. Define FMSTR_TRANSPORT as FMSTR_CAN, and define also FMSTR_CAN_DRV option.
#endif

/* transport MUST be defined in configuration */
#if !defined(FMSTR_CAN_DRV)
#error No FreeMASTER communication can driver interface is enabled. Please choose the driver (FMSTR_CAN_DRV) or set FMSTR_DISABLE option to 1.
#endif

/* CAN-related constants */
#ifdef FMSTR_CAN_EXTID
#if FMSTR_CAN_EXTID != 0x80000000U
#error FMSTR_CAN_EXTID must be defined as 0x80000000U
#undef FMSTR_CAN_EXTID
#endif
#endif

#ifndef FMSTR_CAN_EXTID
#define FMSTR_CAN_EXTID 0x80000000U
#endif

/* obsolete dynamic address selection */
#if defined(FMSTR_CAN_BASE_DYNAMIC)
#warning The FMSTR_SCI_BASE_DYNAMIC is now obsolete and may be removed.
#endif

/* obsolete dynamic ID selection */
#if defined(FMSTR_CAN_CMDID_DYNAMIC)
#warning The FMSTR_CAN_CMDID_DYNAMIC is now obsolete and may be removed.
#endif

/* incoming (command) CAN message ID */
#ifndef FMSTR_CAN_CMDID
#define FMSTR_CAN_CMDID 0x7aa
#endif

/* obsolete dynamic ID selection */
#if defined(FMSTR_CAN_RESPID_DYNAMIC)
#warning The FMSTR_CAN_RESPID_DYNAMIC is now obsolete and may be removed.
#endif

/* response CAN message ID, may be the same as command ID */
#ifndef FMSTR_CAN_RESPID
#define FMSTR_CAN_RESPID 0x7AAU
#endif

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * Types definition
 ******************************************************************************/

/* FreeMASTER Driver interface between the CAN layer and hardware driver */
typedef struct FMSTR_CAN_DRV_INTF_S
{
    FMSTR_BOOL (*Init)(FMSTR_U32 idRx, FMSTR_U32 idTx); /* Initialize CAN module. */
    void (*EnableTxInterrupt)(FMSTR_BOOL enable);       /* Enable CAN Transmit interrupt. */
    void (*EnableRxInterrupt)(FMSTR_BOOL enable);       /* Enable CAN Receive interrupt. */

    /* receiver */
    void (*EnableRx)(void);             /* Enable/re-initialize Receiver buffer. */
    FMSTR_SIZE8 (*GetRxFrameLen)(void); /* Return size of received CAN frame, or 0 if no Rx frame is available. */
    FMSTR_BCHR (*GetRxFrameByte)(FMSTR_SIZE8 index); /* Get data byte at index (0..8). */
    void (*AckRxFrame)(void);                        /* Discard received frame and enable receiving a next one. */

    /* transmitter */
    FMSTR_BOOL (*PrepareTxFrame)(void); /* Initialize transmit buffer; return false when Tx buffer is not available. */
    void (*PutTxFrameByte)(FMSTR_SIZE8 index, FMSTR_BCHR data); /* Fill one byte of transmit data. */
    void (*SendTxFrame)(FMSTR_SIZE8 len);                       /* Send the Tx buffer. */

} FMSTR_CAN_DRV_INTF;

/******************************************************************************
 * Inline functions
 ******************************************************************************/

/******************************************************************************
 * Global CAN-specific API functions
 ******************************************************************************/

void FMSTR_SetCanCmdID(FMSTR_U32 canID);
void FMSTR_SetCanRespID(FMSTR_U32 canID);
void FMSTR_ProcessCanRx(void);
void FMSTR_ProcessCanTx(void);

#ifdef FMSTR_CAN_DRV
extern const FMSTR_CAN_DRV_INTF FMSTR_CAN_DRV;
#endif

#ifdef __cplusplus
}
#endif

#endif /* __FREEMASTER_CAN_H */
