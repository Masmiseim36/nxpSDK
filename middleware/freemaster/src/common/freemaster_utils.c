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

#include "freemaster.h"
#include "freemaster_private.h"
#include "freemaster_utils.h"

#if !(FMSTR_DISABLE)


/**************************************************************************//*!
*
* @brief    FreeMASTER Utility - Decode unsigned LEB field
*
******************************************************************************/

FMSTR_BPTR FMSTR_UlebDecode(FMSTR_BPTR in, FMSTR_SIZE* res)
{
    FMSTR_SIZE result = 0;
    FMSTR_SIZE shift = 0;
    FMSTR_BCHR b;

    do
    {
        in = FMSTR_ValueFromBuffer8(&b, in);
        result |= (b & 0x7F) << shift;
        shift += 7;
    } while(b & 0x80);

    *res = result;

    return in;
}

/**************************************************************************//*!
*
* @brief    FreeMASTER Utility - Decode unsigned LEB field into 64 bit variable
*
******************************************************************************/

FMSTR_BPTR FMSTR_UlebDecode64(FMSTR_BPTR in, FMSTR_U64* res)
{
    FMSTR_U64 result = 0;
    FMSTR_SIZE shift = 0;
    FMSTR_BCHR b;

    do
    {
        in = FMSTR_ValueFromBuffer8(&b, in);
        result |= (b & 0x7F) << shift;
        shift += 7;
    } while(b & 0x80);

    *res = result;

    return in;
}

/**************************************************************************//*!
*
* @brief    FreeMASTER Utility - Decode unsigned LEB field byte by byte; Initialization
*
******************************************************************************/

void FMSTR_UlebDecodeInit(FMSTR_ULEB_CTX *ctx)
{
    ctx->result = 0;
    ctx->shift = 0;
}

/**************************************************************************//*!
*
* @brief    FreeMASTER Utility - Decode unsigned LEB field byte by byte; Process
*
******************************************************************************/

FMSTR_BOOL FMSTR_UlebDecodeProcess(FMSTR_ULEB_CTX *ctx, FMSTR_BCHR b)
{
    ctx->result |= (b & 0x7F) << ctx->shift;
    ctx->shift += 7;

    return (b & 0x80);
}

/**************************************************************************//*!
*
* @brief    FreeMASTER Utility - Decode unsigned LEB field byte by byte; Get result
*
******************************************************************************/

FMSTR_SIZE FMSTR_UlebDecodeGet(FMSTR_ULEB_CTX *ctx)
{
    return ctx->result;
}

/**************************************************************************//*!
*
* @brief    FreeMASTER Utility - Encode unsigned LEB field
*
******************************************************************************/

FMSTR_BPTR FMSTR_UlebEncode(FMSTR_BPTR out, FMSTR_SIZE max_len, FMSTR_SIZE val)
{
    FMSTR_INDEX cnt = 0;
    FMSTR_BCHR b;

    do
    {
        b = val & 0x7f ;
        val >>= 7;
        if (val != 0) /* Will follow some additional bytes */
            b |= 0x80;
        out = FMSTR_ValueToBuffer8(out, b);
        if(++cnt >= max_len)
            break;
    } while (val != 0);

    return out;
}

/**************************************************************************//*!
*
* @brief    FreeMASTER Utility - Decode signed LEB field
*
******************************************************************************/

FMSTR_BPTR FMSTR_SlebDecode(FMSTR_BPTR in, FMSTR_INDEX *res)
{
    FMSTR_INDEX result = 0;
    FMSTR_SIZE shift = 0;
    FMSTR_BCHR b;

    do
    {
        in = FMSTR_ValueFromBuffer8(&b, in);
        result |= (b & 0x7F) << shift;
        shift += 7;
    } while(b & 0x80);

    /* sign bit of byte is second high order bit (0x40) */
    if ((shift < (sizeof(FMSTR_INDEX) * 8)) && (b & 0x40))
        result |= (~0 << shift);

    *res = result;

    return in;
}

/**************************************************************************//*!
*
* @brief    FreeMASTER Utility - Decode signed LEB field into 64-bit signed variable
*
******************************************************************************/

FMSTR_BPTR FMSTR_SlebDecode64(FMSTR_BPTR in, FMSTR_S64 *res)
{
    FMSTR_S64 result = 0;
    FMSTR_SIZE shift = 0;
    FMSTR_BCHR b;

    do
    {
        in = FMSTR_ValueFromBuffer8(&b, in);
        result |= (b & 0x7F) << shift;
        shift += 7;
    } while(b & 0x80);

    /* sign bit of byte is second high order bit (0x40) */
    if ((shift < (sizeof(FMSTR_S64) * 8)) && (b & 0x40))
        result |= (~0ULL << shift);

    *res = result;

    return in;
}

/**************************************************************************//*!
*
* @brief    FreeMASTER Utility - Encode signed LEB field
*
******************************************************************************/

FMSTR_BPTR FMSTR_SlebEncode(FMSTR_BPTR out, FMSTR_SIZE max_len, FMSTR_INDEX val)
{
    FMSTR_INDEX cnt = 0;
    FMSTR_BCHR b;
    FMSTR_BOOL goahead = FMSTR_TRUE;
    do
    {
        b = val & 0x7f ;
        val >>= 7;

        if((val == 0 && ((b & 0x40) == 0x00)) || (val == -1 && ((b & 0x40) == 0x40)))
            goahead = 0;
        else
            b |= 0x80;

        out = FMSTR_ValueToBuffer8(out, b);
        if(++cnt >= max_len)
            break;

    } while(goahead);

    return out;
}

/**************************************************************************//*!
*
* @brief  Skip in buffer the LEB field (it doesn't matter if signed or unsigned)
*
* @param  dest - Pointer to LEB field
*
* @return  pointer to buffer just behind LEB field
*
******************************************************************************/
FMSTR_BPTR FMSTR_SkipInBufferLeb(FMSTR_BPTR dest)
{
    FMSTR_BCHR b;
    do
    {
        b = *(dest++);
    } while(b & 0x80);

    return dest;
}

/**************************************************************************//*!
*
* @brief    FreeMASTER Utility - Function gets the string from incomming buffer
*
******************************************************************************/
FMSTR_BPTR FMSTR_StringFromBuffer(FMSTR_BPTR in, FMSTR_U8** pStr)
{
    FMSTR_BCHR b;

    *pStr = in;
    do
    {
        in = FMSTR_ValueFromBuffer8(&b, in);
    } while(b);

    return in;
}

/**************************************************************************//*!
*
* @brief    FreeMASTER Utility - Function copy the string from memory to outcomming buffer
*
******************************************************************************/
FMSTR_BPTR FMSTR_StringCopyToBuffer(FMSTR_BPTR out, const FMSTR_CHAR* pStr)
{
    while(*pStr)
    {
        out = FMSTR_ValueToBuffer8(out, (FMSTR_U8) *(pStr++));
    }

    out = FMSTR_ValueToBuffer8(out, (FMSTR_U8)0);
    return out;
}

/**************************************************************************//*!
*
* @brief    FreeMASTER Utility - The function initialize CRC16 calculation
*
******************************************************************************/

void FMSTR_Crc16Init(FMSTR_U16 *crc)
{
    *crc = FMSTR_CRC16_CCITT_SEED;
}

/**************************************************************************//*!
*
* @brief    FreeMASTER Utility - The function add new byte to CRC16 calculation
*
******************************************************************************/

void FMSTR_Crc16AddByte(FMSTR_U16 *crc, FMSTR_U8 data)
{
    FMSTR_INDEX x;

    *crc ^= data << 8;                               /* XOR hi-byte of CRC w/dat    */
    for (x = 8; x; --x)                             /* Then, for 8 bit shifts...   */
    {
        if (*crc & 0x8000)                        /* Test hi order bit of CRC    */
            *crc = *crc << 1 ^ 0x1021;          /* if set, shift & XOR w/$1021 */
        else
            *crc <<= 1;                        /* Else, just shift left once. */
    }
}

/**************************************************************************//*!
*
* @brief    FreeMASTER Utility - The function initialize CRC8 calculation
*
******************************************************************************/

void FMSTR_Crc8Init(FMSTR_U8 *crc)
{
    *crc = FMSTR_CRC8_CCITT_SEED;
}

/**************************************************************************//*!
*
* @brief    FreeMASTER Utility - The function add new byte to CRC8 calculation
*
******************************************************************************/

void FMSTR_Crc8AddByte(FMSTR_U8 *crc, FMSTR_U8 data)
{
    FMSTR_INDEX x;

    *crc ^= data;                               /* XOR hi-byte of CRC w/dat    */
    for (x = 8; x; --x)                         /* Then, for 8 bit shifts...   */
    {
        if (*crc & 0x80)                        /* Test hi order bit of CRC    */
            *crc = (*crc << 1) ^ 0x07;          /* if set, shift & XOR w/$07 */
        else
            *crc <<= 1;                        /* Else, just shift left once. */
    }
}

/**************************************************************************//*!
*
* @brief    FreeMASTER Utility - The function returns array of random numbers
*
******************************************************************************/

FMSTR_BPTR FMSTR_RandomNumbersToBuffer(FMSTR_U8 *out, FMSTR_SIZE lenght)
{
    FMSTR_INDEX i;
    FMSTR_SIZE sz;
    FMSTR_U32 r;
    FMSTR_U8* dest = out;
    FMSTR_U8* ret = out;

    for(i=0; i<lenght; i+=4)
    {
        r = FMSTR_Rand();

        sz = lenght - i;
        if(sz > 4)
            sz = 4;

        ret = FMSTR_CopyToBuffer(&dest[i], (FMSTR_ADDR)&r, sz);
    }

    return ret;
}

/**************************************************************************//*!
*
* @brief    FreeMASTER Utility - The function prepares ring buffer
*
******************************************************************************/

void _FMSTR_RingBuffCreate(FMSTR_RING_BUFFER * ringBuff, FMSTR_BPTR buffer, FMSTR_U32 size)
{
    FMSTR_ASSERT(ringBuff != NULL);
    FMSTR_ASSERT(buffer != NULL);

    FMSTR_MemSet(ringBuff, 0, sizeof(FMSTR_RING_BUFFER));

    ringBuff->buffer = buffer;
    ringBuff->size = size;
    ringBuff->rp = buffer;
    ringBuff->wp = buffer;
}

/**************************************************************************//*!
*
* @brief    FreeMASTER Utility - The function adds character into ring buffer
*
******************************************************************************/

void _FMSTR_RingBuffPut(FMSTR_RING_BUFFER * ringBuff, FMSTR_BCHR nRxChar)
{
    FMSTR_ASSERT(ringBuff != NULL);

    /* future value of write pointer */
    FMSTR_BPTR wpnext = ringBuff->wp + 1;

    if(wpnext >= (ringBuff->buffer + ringBuff->size))
    {
        wpnext = ringBuff->buffer;
    }

    /* any space in queue? */
    if(wpnext != ringBuff->rp)
    {
        *ringBuff->wp = (FMSTR_U8) nRxChar;
        ringBuff->wp = wpnext;
    }
}

/**************************************************************************//*!
*
* @brief    FreeMASTER Utility - The function gets character from ring buffer
*
******************************************************************************/

FMSTR_BCHR _FMSTR_RingBuffGet(FMSTR_RING_BUFFER * ringBuff)
{
    FMSTR_BCHR nChar = 0U;

    FMSTR_ASSERT(ringBuff != NULL);

    /* get all queued characters */
    if(ringBuff->rp != ringBuff->wp)
    {
        nChar = *ringBuff->rp++;

        if(ringBuff->rp >= (ringBuff->buffer + ringBuff->size))
        {
            ringBuff->rp = ringBuff->buffer;
        }
    }

    return nChar;
}

/**************************************************************************//*!
*
* @brief    FreeMASTER Utility - The function returns true, when is space in ring buffer
*
******************************************************************************/

FMSTR_BOOL _FMSTR_RingBuffIsSpace(FMSTR_RING_BUFFER * ringBuff)
{
    FMSTR_BPTR wpnext = ringBuff->wp + 1;

    FMSTR_ASSERT(ringBuff != NULL);

    /* Is any space in buffer? */
    if(wpnext != ringBuff->rp)
        return FMSTR_TRUE;

    return FMSTR_FALSE;
}

/**************************************************************************//*!
*
* @brief    FreeMASTER Utility - The function returns true, when some data in ring buffer
*
******************************************************************************/

FMSTR_BOOL _FMSTR_RingBuffHasData(FMSTR_RING_BUFFER * ringBuff)
{
    FMSTR_ASSERT(ringBuff != NULL);

    /* Is any data available to get from buffer? */
    if(ringBuff->rp != ringBuff->wp)
        return FMSTR_TRUE;

    return FMSTR_FALSE;
}


#endif /* !FMSTR_DISABLE */
