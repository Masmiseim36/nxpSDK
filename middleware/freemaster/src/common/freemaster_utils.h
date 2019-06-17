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
#define FMSTR_CRC16_CCITT_SEED  0xFFFF
#define FMSTR_CRC8_CCITT_SEED   0x00

#ifdef __cplusplus
  extern "C" {
#endif

/******************************************************************************
* Types definitions
******************************************************************************/
/* Context struct for decoding unsigned LEB format byte by byte. */
typedef struct
{
    FMSTR_SIZE      result;
    FMSTR_SIZE      shift;
} FMSTR_ULEB_CTX;

/* Context structure for ring buffer */
typedef struct
{
    FMSTR_BPTR  buffer;        /* Ring buffer pointer */
    FMSTR_U32   size;           /* Size of ring buffer */
    FMSTR_BPTR  rp;             /* Read pointer */
    FMSTR_BPTR  wp;             /* Write pointer */
} FMSTR_RING_BUFFER;

/******************************************************************************
* Inline functions
******************************************************************************/

/******************************************************************************
* Global API functions
******************************************************************************/
/* FreeMASTER Utility - Decode unsigned LEB field byte by byte; Initialization. */
void FMSTR_UlebDecodeInit(FMSTR_ULEB_CTX *ctx);
/* FreeMASTER Utility - Decode unsigned LEB field byte by byte; Process. */
FMSTR_BOOL FMSTR_UlebDecodeProcess(FMSTR_ULEB_CTX *ctx, FMSTR_BCHR b);
/* FreeMASTER Utility - Decode unsigned LEB field byte by byte; Get result. */
FMSTR_SIZE FMSTR_UlebDecodeGet(FMSTR_ULEB_CTX *ctx);
/* FreeMASTER Utility - Decode unsigned LEB field. */
FMSTR_BPTR FMSTR_UlebDecode(FMSTR_BPTR in, FMSTR_SIZE* res);
/* FreeMASTER Utility - Decode unsigned LEB 64-bit field. */
FMSTR_BPTR FMSTR_UlebDecode64(FMSTR_BPTR in, FMSTR_U64* res);
/* FreeMASTER Utility - Encode unsigned LEB field. */
FMSTR_BPTR FMSTR_UlebEncode(FMSTR_BPTR out, FMSTR_SIZE max_len, FMSTR_SIZE val);
/* FreeMASTER Utility - Decode signed LEB field. */
FMSTR_BPTR FMSTR_SlebDecode(FMSTR_BPTR in, FMSTR_INDEX *res);
/* FreeMASTER Utility - Decode signed LEB 64-bit field. */
FMSTR_BPTR FMSTR_SlebDecode64(FMSTR_BPTR in, FMSTR_S64 *res);
/* FreeMASTER Utility - Encode signed LEB field. */
FMSTR_BPTR FMSTR_SlebEncode(FMSTR_BPTR out, FMSTR_SIZE max_len, FMSTR_INDEX val);
/* FreeMASTER Utility - Skip one LEB/SLEB field in buffer. */
FMSTR_BPTR FMSTR_SkipInBufferLeb(FMSTR_BPTR dest);

/* FreeMASTER Utility - Get the string pointer from buffer. */
FMSTR_BPTR FMSTR_StringFromBuffer(FMSTR_BPTR in, FMSTR_U8** pStr);
/* FreeMASTER Utility - Function copy the string from memory to outcomming buffer. */
FMSTR_BPTR FMSTR_StringCopyToBuffer(FMSTR_BPTR out, const FMSTR_CHAR* pStr);

/* FreeMASTER Utility - The function initialize CRC16 calculation. */
void FMSTR_Crc16Init(FMSTR_U16 *crc);
/* FreeMASTER Utility - The function add new byte to CRC16 calculation. */
void FMSTR_Crc16AddByte(FMSTR_U16 *crc, FMSTR_U8 data);
/* FreeMASTER Utility - The function initialize CRC8 calculation. */
void FMSTR_Crc8Init(FMSTR_U8 *crc);
/* FreeMASTER Utility - The function add new byte to CRC8 calculation. */
void FMSTR_Crc8AddByte(FMSTR_U8 *crc, FMSTR_U8 data);

/* FreeMASTER Utility - The function returns array of random numbers */
FMSTR_BPTR FMSTR_RandomNumbersToBuffer(FMSTR_U8 *out, FMSTR_SIZE lenght);

/* FreeMASTER Utility - The function prepares ring buffer */
void _FMSTR_RingBuffCreate(FMSTR_RING_BUFFER * ringBuff, FMSTR_BPTR buffer, FMSTR_U32 size);
/* FreeMASTER Utility - The function adds character into ring buffer */
void _FMSTR_RingBuffPut(FMSTR_RING_BUFFER * ringBuff, FMSTR_BCHR nRxChar);
/* FreeMASTER Utility - The function gets character from ring buffer */
FMSTR_BCHR _FMSTR_RingBuffGet(FMSTR_RING_BUFFER * ringBuff);
/* FreeMASTER Utility - The function returns true, when is space in ring buffer */
FMSTR_BOOL _FMSTR_RingBuffIsSpace(FMSTR_RING_BUFFER * ringBuff);
/* FreeMASTER Utility - The function returns true, when some data in ring buffer */
FMSTR_BOOL _FMSTR_RingBuffHasData(FMSTR_RING_BUFFER * ringBuff);
/******************************************************************************
* Global variable declaration
******************************************************************************/

#ifdef __cplusplus
  }
#endif

#endif /* __FREEMASTER_UTILS_H */

