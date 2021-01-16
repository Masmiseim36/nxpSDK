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
 * FreeMASTER Communication Driver - utility code
 */

#ifndef __FREEMASTER_UTILS_H
#define __FREEMASTER_UTILS_H

/******************************************************************************
 * Required header files include check
 ******************************************************************************/
#ifndef __FREEMASTER_H
#error Please include the freemaster.h master header file before the freemaster_serial_uart.h
#endif

/******************************************************************************
 * Macro definitions
 ******************************************************************************/
#define FMSTR_CRC16_CCITT_SEED 0xFFFFU
#define FMSTR_CRC8_CCITT_SEED  0x00U

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * Types definitions
 ******************************************************************************/
/* Context struct for decoding unsigned LEB format byte by byte. */
typedef struct
{
    FMSTR_SIZE result;
    FMSTR_SIZE shift;
} FMSTR_ULEB_CTX;

/* Context structure for ring buffer */
typedef struct
{
    FMSTR_BPTR buffer; /* Ring buffer pointer */
    FMSTR_U32 size;    /* Size of ring buffer */
    FMSTR_BPTR rp;     /* Read pointer */
    FMSTR_BPTR wp;     /* Write pointer */
} FMSTR_RING_BUFFER;

/******************************************************************************
 * Inline functions
 ******************************************************************************/

/******************************************************************************
 * Global API functions
 ******************************************************************************/

/* Decode unsigned LEB field. */
FMSTR_BPTR FMSTR_UlebDecode(FMSTR_BPTR in, void *result, FMSTR_SIZE size);
/* Encode unsigned LEB field. */
FMSTR_BPTR FMSTR_UlebEncode(FMSTR_BPTR out, void *source, FMSTR_SIZE size);
/* Decode signed LEB field. */
FMSTR_BPTR FMSTR_SlebDecode(FMSTR_BPTR in, void *result, FMSTR_SIZE size);
/* Skip one LEB/SLEB field in buffer. */
FMSTR_BPTR FMSTR_SkipInBufferLeb(FMSTR_BPTR dest);

/* Copy string from buffer to memory. */
FMSTR_BPTR FMSTR_StringFromBuffer(FMSTR_BPTR in, FMSTR_CHAR *pStr, FMSTR_SIZE maxSize);
/* Copy string from memory to buffer. */
FMSTR_BPTR FMSTR_StringCopyToBuffer(FMSTR_BPTR out, const FMSTR_CHAR *pStr);

/* Initialize CRC16 calculation. */
void FMSTR_Crc16Init(FMSTR_U16 *crc);
/* Add new byte to CRC16 calculation. */
void FMSTR_Crc16AddByte(FMSTR_U16 *crc, FMSTR_U8 data);
/* Initialize CRC8 calculation. */
void FMSTR_Crc8Init(FMSTR_U8 *crc);
/* Add new byte to CRC8 calculation. */
void FMSTR_Crc8AddByte(FMSTR_U8 *crc, FMSTR_U8 data);

/* Get array of random numbers */
FMSTR_BPTR FMSTR_RandomNumbersToBuffer(FMSTR_BPTR out, FMSTR_SIZE length);
/* Increase entropy of random number generator */
void FMSTR_Randomize(FMSTR_U32 entropy);

/* Prepare ring buffer */
void _FMSTR_RingBuffCreate(FMSTR_RING_BUFFER *ringBuff, FMSTR_BPTR buffer, FMSTR_U32 size);
/* Add character into ring buffer */
void _FMSTR_RingBuffPut(FMSTR_RING_BUFFER *ringBuff, FMSTR_BCHR nRxChar);
/* Get character from ring buffer */
FMSTR_BCHR _FMSTR_RingBuffGet(FMSTR_RING_BUFFER *ringBuff);
/* Returns true, when is space in ring buffer */
FMSTR_BOOL _FMSTR_RingBuffIsSpace(FMSTR_RING_BUFFER *ringBuff);
/* Returns true, when any data in ring buffer */
FMSTR_BOOL _FMSTR_RingBuffHasData(FMSTR_RING_BUFFER *ringBuff);

/******************************************************************************
 * Global variable declaration
 ******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* __FREEMASTER_UTILS_H */
