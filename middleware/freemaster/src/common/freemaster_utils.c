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

/********************************************************
*  optimized memory copy helper macros
********************************************************/

#if FMSTR_PLATFORM_BIG_ENDIAN && FMSTR_MEMCPY_MAX_SIZE > 1
#error Optimized/aligned memory copy assuming little-endian platform in this version
#endif

/* Copy variable from source by bytes to an aligned destination address */
#define COPY_TO_POSTINCREMENT(dest, src, type) { \
        volatile type s = 0; \
        volatile FMSTR_U8 *ps8 = (volatile FMSTR_U8*)src; \
        volatile type* pd = ((volatile type*)(dest)); \
        for(int i=0; i<sizeof(type)*8; i+=8,ps8++)  \
            s |= (type)(*ps8)<<i; \
        *pd++ = s; /* single write access to aligned destination */ \
        dest = pd; /* post increment dest */ \
        src = ps8; /* post increment src */ \
    }

/* Copy variable from an aligned source address to destination by bytes */
#define COPY_FROM_POSTINCREMENT(dest, src, type) { \
        volatile type* ps = ((volatile type*)(src)); \
        volatile type s = *ps++; /* single read access from aligned source */ \
        volatile FMSTR_U8* pd8 = ((volatile FMSTR_U8*)(dest)); \
        for(int i=0; i<sizeof(type)*8; i+=8, pd8++)  \
            *pd8 = (s>>i)&0xFF; \
        dest = pd8; /* post increment dest */ \
        src = ps;   /* post increment src */ \
    }

/* Copy variable from source by bytes into aligned destination with mask */
#define COPY_TO_MASKED_POSTINCREMENT(dest, src, mask, type) { \
        volatile type s = 0; \
        volatile type* pd = ((volatile type*)(dest)); \
        volatile type dx = *pd; /* single read access from aligned destination */ \
        volatile FMSTR_U8 *ps8 = (volatile FMSTR_U8*)src; \
        volatile FMSTR_U8 *pd8 = (volatile FMSTR_U8*)&dx; \
        volatile FMSTR_U8 *pm8 = ((volatile FMSTR_U8*)(mask)); \
        FMSTR_U8 m; \
        for(int i=0; i<sizeof(type)*8; i+=8,ps8++,pd8++,pm8++) { \
            m = *pm8; \
            s |= (type)(((*ps8) & m) | ((*pd8) & (~m)))<<i; \
        } \
        *pd++ = s;  /* single write access to aligned destination */ \
        dest = pd;  /* post increment dest */ \
        src = ps8;  /* post increment src */ \
        mask = pm8; /* post increment mask */ \
    }

/* Pointer ptr should be casted by platform. On 32bit platform it is cast to 32bit pointer! */
#define TEST_MISALIGNED(ptr, bits) (((FMSTR_U32)(ptr)) & ((1<<(bits))-1))

/* in this helper call, we are already sure that the destination pointer is 64-bit aligned */
static void _FMSTR_MemCpyDstAligned(volatile void* dest, volatile void* src, FMSTR_SIZE size)
{
#if FMSTR_MEMCPY_MAX_SIZE >= 8
    /* 64-bit aligned part */
    while(size >= sizeof(FMSTR_U64))
    {
        COPY_TO_POSTINCREMENT(dest, src, FMSTR_U64);
        size -= sizeof(FMSTR_U64);
    }
#endif
#if FMSTR_MEMCPY_MAX_SIZE >= 4
    /* remaining word(s) */
    while(size >= sizeof(FMSTR_U32))
    {
        COPY_TO_POSTINCREMENT(dest, src, FMSTR_U32);
        size -= sizeof(FMSTR_U32);
    }
#endif
#if FMSTR_MEMCPY_MAX_SIZE >= 2
    /* remaining halfword(s) */
    while(size >= sizeof(FMSTR_U16))
    {
        COPY_TO_POSTINCREMENT(dest, src, FMSTR_U16);
        size -= sizeof(FMSTR_U16);
    }
#endif
    {
        volatile FMSTR_U8* ps = (volatile FMSTR_U8*)src;
        volatile FMSTR_U8* pd = (volatile FMSTR_U8*)dest;

        /* remaining byte(s) */
        while(size >= 1)
        {
            *pd++ = *ps++;
            size--;
        }
    }

    FMSTR_ASSERT(size == 0);
}
/* in this helper call, we are already sure that the source pointer is 64-bit aligned */
static void _FMSTR_MemCpySrcAligned(volatile void* dest, volatile void* src, FMSTR_SIZE size)
{
#if FMSTR_MEMCPY_MAX_SIZE >= 8
    /* 64-bit aligned part */
    while(size >= sizeof(FMSTR_U64))
    {
        COPY_FROM_POSTINCREMENT(dest, src, FMSTR_U64);
        size -= sizeof(FMSTR_U64);
    }
#endif
#if FMSTR_MEMCPY_MAX_SIZE >= 4
    /* remaining word */
    while(size >= sizeof(FMSTR_U32))
    {
        COPY_FROM_POSTINCREMENT(dest, src, FMSTR_U32);
        size -= sizeof(FMSTR_U32);
    }
#endif
#if FMSTR_MEMCPY_MAX_SIZE >= 2
    /* remaining halfword */
    while(size >= sizeof(FMSTR_U16))
    {
        COPY_FROM_POSTINCREMENT(dest, src, FMSTR_U16);
        size -= sizeof(FMSTR_U16);
    }
#endif
    {
        volatile FMSTR_U8* ps = (volatile FMSTR_U8*)src;
        volatile FMSTR_U8* pd = (volatile FMSTR_U8*)dest;

        /* remaining byte(s) */
        while(size >= 1)
        {
            *pd++ = *ps++;
            size--;
        }
    }

    FMSTR_ASSERT(size == 0);
}

/* in this helper call, we are already sure that the required pointer is 64-bit aligned */
static void _FMSTR_MemCpyMaskedDstAligned(volatile void* dest, volatile void* src, volatile void* mask, FMSTR_SIZE size)
{
#if FMSTR_MEMCPY_MAX_SIZE >= 8
    /* 64-bit aligned part */
    while(size >= sizeof(FMSTR_U64))
    {
        COPY_TO_MASKED_POSTINCREMENT(dest, src, mask, FMSTR_U64);
        size -= sizeof(FMSTR_U64);
    }
#endif
#if FMSTR_MEMCPY_MAX_SIZE >= 4
    /* remaining word */
    while(size >= sizeof(FMSTR_U32))
    {
        COPY_TO_MASKED_POSTINCREMENT(dest, src, mask, FMSTR_U32);
        size -= sizeof(FMSTR_U32);
    }
#endif
#if FMSTR_MEMCPY_MAX_SIZE >= 2
    /* remaining halfword */
    while(size >= sizeof(FMSTR_U16))
    {
        COPY_TO_MASKED_POSTINCREMENT(dest, src, mask, FMSTR_U16);
        size -= sizeof(FMSTR_U16);
    }
#endif
    {
        volatile FMSTR_U8* ps = (volatile FMSTR_U8*)src;
        volatile FMSTR_U8* pd = (volatile FMSTR_U8*)dest;
        volatile FMSTR_U8* pm = (volatile FMSTR_U8*)mask;
        FMSTR_U8 m, s;

        /* remaining byte(s) */
        while(size >= 1)
        {
            m = *pm++;
            s = *ps++ & m;
            s |= *pd & (~m);
            *pd++ = s;
            size--;
        }
    }

    FMSTR_ASSERT(size == 0);
}

/**************************************************************************//*!
*
* @brief  Copy data from address in memory into buffer. Read from memory is aligned
*         to 64, 32 or 16 bytes - depends on platform.
*
* @param  destBuff - pointer to destination memory in communication buffer
* @param  srcAddr  - source memory address
* @param  size      - buffer size (always in bytes)
*
* @return This function returns a pointer to next byte in comm. buffer
*
******************************************************************************/

FMSTR_WEAK void FMSTR_MemCpyFrom(volatile void* destBuff, volatile void* srcAddr, FMSTR_SIZE size)
{
#if FMSTR_MEMCPY_MAX_SIZE >= 2
    /* misaligned odd byte */
    if(TEST_MISALIGNED(srcAddr, 1) && size >= sizeof(FMSTR_U8))
    {
        COPY_FROM_POSTINCREMENT(destBuff, srcAddr, FMSTR_U8);
        size -= sizeof(FMSTR_U8);
    }
#endif
#if FMSTR_MEMCPY_MAX_SIZE >= 4
    /* misaligned odd halfword */
    if(TEST_MISALIGNED(srcAddr, 2) && size >= sizeof(FMSTR_U16))
    {
        COPY_FROM_POSTINCREMENT(destBuff, srcAddr, FMSTR_U16);
        size -= sizeof(FMSTR_U16);
    }
#endif
#if FMSTR_MEMCPY_MAX_SIZE >= 8
    /* misaligned odd word */
    if(TEST_MISALIGNED(srcAddr, 3) && size >= sizeof(FMSTR_U32))
    {
        COPY_FROM_POSTINCREMENT(destBuff, srcAddr, FMSTR_U32);
        size -= sizeof(FMSTR_U32);
    }
#endif
    /* the rest is already aligned */
    _FMSTR_MemCpySrcAligned(destBuff, srcAddr, size);
}

/**************************************************************************//*!
*
* @brief  Copy data from buffer into specific address in memory. Write into memory is
*         aligned to 64, 32 or 16 bytes - depends on platform.
*
* @param  destAddr - destination memory address
* @param  srcBuff  - pointer to source memory in communication buffer
* @param  size     - buffer size (always in bytes)
*
* @return This function returns a pointer to next byte in comm. buffer
*
******************************************************************************/

FMSTR_WEAK void FMSTR_MemCpyTo(volatile void* destAddr, volatile void* srcBuff, FMSTR_SIZE size)
{
#if FMSTR_MEMCPY_MAX_SIZE >= 2
    /* misaligned odd byte */
    if(TEST_MISALIGNED(destAddr, 1) && size >= sizeof(FMSTR_U8))
    {
        COPY_TO_POSTINCREMENT(destAddr, srcBuff, FMSTR_U8);
        size -= sizeof(FMSTR_U8);
    }
#endif
#if FMSTR_MEMCPY_MAX_SIZE >= 4
    /* misaligned odd halfword */
    if(TEST_MISALIGNED(destAddr, 2) && size >= sizeof(FMSTR_U16))
    {
        COPY_TO_POSTINCREMENT(destAddr, srcBuff, FMSTR_U16);
        size -= sizeof(FMSTR_U16);
    }
#endif
#if FMSTR_MEMCPY_MAX_SIZE >= 8
    /* misaligned odd word */
    if(TEST_MISALIGNED(destAddr, 3) && size >= sizeof(FMSTR_U32))
    {
        COPY_TO_POSTINCREMENT(destAddr, srcBuff, FMSTR_U32);
        size -= sizeof(FMSTR_U32);
    }
#endif
    /* the rest is already aligned */
    _FMSTR_MemCpyDstAligned(destAddr, srcBuff, size);
}

/**************************************************************************//*!
*
* @brief  Copy data from buffer into specific address in memory (register). Write into memory is
*         aligned to 64, 32 or 16 bytes - depends on platform.
*
* @param  destAddr - destination memory address (register)
* @param  srcBuff  - pointer to source memory in communication buffer
* @param  size     - buffer size (always in bytes)
*
* @return This function returns a pointer to next byte in comm. buffer
*
******************************************************************************/
FMSTR_WEAK void FMSTR_MemCpyToMasked(volatile void* destAddr, volatile void* srcBuff, volatile void* maskBuff, FMSTR_SIZE size)
{
#if FMSTR_MEMCPY_MAX_SIZE >= 2
    /* misaligned odd byte */
    if(TEST_MISALIGNED(destAddr, 1) && size >= sizeof(FMSTR_U8))
    {
        COPY_TO_MASKED_POSTINCREMENT(destAddr, srcBuff, maskBuff, FMSTR_U8);
        size -= sizeof(FMSTR_U8);
    }
#endif
#if FMSTR_MEMCPY_MAX_SIZE >= 4
    /* misaligned odd halfword */
    if(TEST_MISALIGNED(destAddr, 2) && size >= sizeof(FMSTR_U16))
    {
        COPY_TO_MASKED_POSTINCREMENT(destAddr, srcBuff, maskBuff, FMSTR_U16);
        size -= sizeof(FMSTR_U16);
    }
#endif
#if FMSTR_MEMCPY_MAX_SIZE >= 8
    /* misaligned odd word */
    if(TEST_MISALIGNED(destAddr, 3) && size >= sizeof(FMSTR_U32))
    {
        COPY_TO_MASKED_POSTINCREMENT(destAddr, srcBuff, maskBuff, FMSTR_U32);
        size -= sizeof(FMSTR_U32);
    }
#endif
    /* the rest is already aligned */
    _FMSTR_MemCpyMaskedDstAligned(destAddr, srcBuff, maskBuff, size);
}

/**************************************************************************//*!
*
* @brief  Write-into the communication buffer memory
*
* @param  destBuff - pointer to destination memory in communication buffer
* @param  srcAddr  - source memory address
* @param  size      - buffer size (always in bytes)
*
* @return This function returns a pointer to next byte in comm. buffer
*
******************************************************************************/

FMSTR_WEAK FMSTR_BPTR FMSTR_CopyToBuffer(FMSTR_BPTR destBuff, FMSTR_ADDR srcAddr, FMSTR_SIZE size)
{
    FMSTR_MemCpyFrom(destBuff, srcAddr, size);
    return (FMSTR_BPTR) (((FMSTR_U8*)destBuff)+size);
}

/**************************************************************************//*!
*
* @brief  Read-out memory from communication buffer
*
* @param  destAddr - destination memory address
* @param  srcBuff  - pointer to source memory in communication buffer
* @param  size     - buffer size (always in bytes)
*
* @return This function returns a pointer to next byte in comm. buffer
*
******************************************************************************/

FMSTR_WEAK FMSTR_BPTR FMSTR_CopyFromBuffer(FMSTR_ADDR destAddr, FMSTR_BPTR srcBuff, FMSTR_SIZE size)
{
    FMSTR_MemCpyTo(destAddr, srcBuff, size);
    return (FMSTR_BPTR) (((FMSTR_U8*)srcBuff)+size);
}

FMSTR_WEAK FMSTR_BPTR FMSTR_ValueFromBuffer16(FMSTR_U16* dest, FMSTR_BPTR src)
{
    return FMSTR_CopyFromBuffer((FMSTR_ADDR)(FMSTR_U8*)dest, src, 2U);
}

FMSTR_WEAK FMSTR_BPTR FMSTR_ValueFromBuffer32(FMSTR_U32* dest, FMSTR_BPTR src)
{
    return FMSTR_CopyFromBuffer((FMSTR_ADDR)(FMSTR_U8*)dest, src, 4U);
}

FMSTR_WEAK FMSTR_BPTR FMSTR_ValueToBuffer16(FMSTR_BPTR dest, FMSTR_U16 src)
{
    return FMSTR_CopyToBuffer(dest, (FMSTR_ADDR)(FMSTR_U8*)&src, 2U);
}

FMSTR_WEAK FMSTR_BPTR FMSTR_ValueToBuffer32(FMSTR_BPTR dest, FMSTR_U32 src)
{
    return FMSTR_CopyToBuffer(dest, (FMSTR_ADDR)(FMSTR_U8*)&src, 4U);
}

/**************************************************************************//*!
*
* @brief  Read-out memory from communication buffer, perform AND-masking
*
* @param  destAddr - destination memory address
* @param  srcBuff  - source memory in communication buffer, mask follows data
* @param  size     - buffer size (always in bytes)
*
******************************************************************************/

FMSTR_WEAK void FMSTR_CopyFromBufferWithMask(FMSTR_ADDR destAddr, FMSTR_BPTR srcBuff, FMSTR_SIZE size)
{
    FMSTR_MemCpyToMasked(destAddr, srcBuff, srcBuff+size, size);
}

/******************************************************************************/

/**************************************************************************//*!
*
* @brief  Store address in LEB format to communication buffer.
*
******************************************************************************/

FMSTR_WEAK FMSTR_BPTR FMSTR_AddressToBuffer(FMSTR_BPTR dest, FMSTR_ADDR addr)
{
    return FMSTR_UlebEncode(dest, &addr, sizeof(addr));
}

/**************************************************************************//*!
*
* @brief  Fetch address in LEB format from communication buffer
*
******************************************************************************/

FMSTR_WEAK FMSTR_BPTR FMSTR_AddressFromBuffer(FMSTR_ADDR* paddr, FMSTR_BPTR src)
{
    return FMSTR_UlebDecode(src, paddr, sizeof(*paddr));
}

/**************************************************************************//*!
*
* @brief  Store size in LEB format to communication buffer.
*
******************************************************************************/

FMSTR_WEAK FMSTR_BPTR FMSTR_SizeToBuffer(FMSTR_BPTR dest, FMSTR_SIZE size)
{
    return FMSTR_UlebEncode(dest, &size, sizeof(size));
}

/**************************************************************************//*!
*
* @brief  Fetch size in LEB format from communication buffer
*
******************************************************************************/

FMSTR_WEAK FMSTR_BPTR FMSTR_SizeFromBuffer(FMSTR_SIZE* psize, FMSTR_BPTR src)
{
    return FMSTR_UlebDecode(src, psize, sizeof(*psize));
}

/**************************************************************************//*!
*
* @brief  Fetch index in signed LEB format from communication buffer
*
******************************************************************************/

FMSTR_WEAK FMSTR_BPTR FMSTR_IndexFromBuffer(FMSTR_INDEX* pindex, FMSTR_BPTR src)
{
    return FMSTR_SlebDecode(src, pindex, sizeof(*pindex));
}

/**************************************************************************//*!
*
* @brief  Store generic U32 number to communication buffer as ULEB
*
******************************************************************************/

FMSTR_BPTR FMSTR_ULebToBuffer(FMSTR_BPTR dest, FMSTR_U32 num)
{
    return FMSTR_UlebEncode(dest, &num, sizeof(num));
}

/**************************************************************************//*!
*
* @brief  Fetch generic U32 value as ULEB from communication buffer
*
******************************************************************************/

FMSTR_BPTR FMSTR_ULebFromBuffer(FMSTR_U32* pnum, FMSTR_BPTR src)
{
    return FMSTR_UlebDecode(src, pnum, sizeof(*pnum));
}

/**************************************************************************//*!
*
* @brief  Return number of bytes that given address needs to add in order to
*         get properly aligned.
*
******************************************************************************/

FMSTR_WEAK FMSTR_SIZE FMSTR_GetAlignmentCorrection(FMSTR_ADDR addr, FMSTR_SIZE size)
{
    FMSTR_U32 addrn = (FMSTR_U32)addr;
    FMSTR_U32 aligned = addrn;

    FMSTR_ASSERT(size == 0 || size == 1 || size == 2 || size == 4 || size == 8);

    if(size > 0)
    {
        aligned += size-1;
        aligned &= ~(size-1);
    }

    return (FMSTR_SIZE)(aligned - addrn);
}

/**************************************************************************//*!
*
* @brief Decode LEB number to destination variable
*
* @param in Pointer to input data
* @param result Pointer to destination variable
* @param size Size of the destination variable
* @param sleb True when decoding SLEB format
*
******************************************************************************/

static FMSTR_BPTR FMSTR_LebDecode(FMSTR_BPTR in, void* result, FMSTR_SIZE size, FMSTR_BOOL sleb)
{
    FMSTR_BCHR b;
    FMSTR_U8 v;
    FMSTR_U8* dest;
    FMSTR_INDEX dadd;
    FMSTR_INDEX shift = 0;

    /* Initialize result to 0 value */
    FMSTR_MemSet(result, 0, size);

#if FMSTR_PLATFORM_BIG_ENDIAN
    dest = ((FMSTR_U8*)result) + size - 1;
    dadd = -1;
#else
    dest = (FMSTR_U8*)result;
    dadd = 1;
#endif

    do
    {
        b = *in++;
        v = b & 0x7f;

        if(size > 0)
        {
            *dest |= (FMSTR_U8)((v << shift));
            shift += 7;

            if(shift >= 8)
            {
                shift -= 8;
                dest += dadd;
                size--;

                if(size > 0 && shift > 0)
                    *dest |= (FMSTR_U8)(v >> (7-shift));
            }
        }
    } while(b & 0x80);

    /* negative number? */
    if(sleb && (b & 0x40))
    {
        if(size-- > 0)
        {
            *dest |= 0xff << shift;
            dest += dadd;
        }

        while(size-- > 0)
        {
            *dest = 0xff;
            dest += dadd;
        }
    }

    return in;
}

FMSTR_BPTR FMSTR_UlebDecode(FMSTR_BPTR in, void* result, FMSTR_SIZE size)
{
    return FMSTR_LebDecode(in, result, size, FMSTR_FALSE);
}

FMSTR_BPTR FMSTR_SlebDecode(FMSTR_BPTR in, void* result, FMSTR_SIZE size)
{
    return FMSTR_LebDecode(in, result, size, FMSTR_TRUE);
}

/**************************************************************************//*!
*
* @brief Encode unsigned variable to ULEB record
*
******************************************************************************/

FMSTR_BPTR FMSTR_UlebEncode(FMSTR_BPTR out, void* source, FMSTR_SIZE size)
{
    FMSTR_BCHR b;
    FMSTR_U8 v;
    FMSTR_U8* src;
    FMSTR_INDEX sadd;
    FMSTR_INDEX shift = 0;
    FMSTR_SIZE zeroes = 0;

#if FMSTR_PLATFORM_BIG_ENDIAN
    src = (FMSTR_U8*)source;
    while(zeroes < size && !*src++)
        zeroes++;
    src = ((FMSTR_U8*)source) + size - 1;
    sadd = -1;
#else
    src = ((FMSTR_U8*)source) + size - 1;
    while(zeroes < size && !*src--)
        zeroes++;
    src = (FMSTR_U8*)source;
    sadd = 1;
#endif

    /* now: 'zeroes' is number of useless most-significant zero bytes
     *      'src' points to least-significant byte and 'sadd' is a direction */
    if(zeroes < size)
    {
        /* we will not encode the zero bytes */
        size -= zeroes;

        /* start with zero (will be or-ing to it) */
        *out = 0;

        while(size--)
        {
            v = *src;
            src += sadd;

            b = (FMSTR_BCHR)((v << shift) & 0x7F);
            *out |= b;

            // shift is number of bits remaining in v
            v >>= (7-shift);

            // other bits to the next out byte
            if(size || v)
            {
                *out++ |= 0x80;
                *out = v & 0x7f;
            }

            shift++;

            if(shift >= 8)
            {
                v >>= 7;

                if(size || v)
                {
                    *out++ |= 0x80;
                    *out = v & 0x7f;
                }

                shift = 1;
            }
        }

        out++;
    }
    else
    {
        /* variable is equal to 0, this encodes as a single 0 byte */
        *out++ = 0;
    }

    return out;
}

/**************************************************************************//*!
*
* @brief  Skip the LEB field in buffer (it doesn't matter if signed or unsigned)
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
* @brief Get string from incomming buffer
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
* @brief Copy string from memory to outcomming buffer
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
* @brief Initialize CRC16 calculation
*
******************************************************************************/

void FMSTR_Crc16Init(FMSTR_U16 *crc)
{
    *crc = FMSTR_CRC16_CCITT_SEED;
}

/**************************************************************************//*!
*
* @brief Add new byte to CRC16 calculation
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
* @brief Initialize CRC8 calculation
*
******************************************************************************/

void FMSTR_Crc8Init(FMSTR_U8 *crc)
{
    *crc = FMSTR_CRC8_CCITT_SEED;
}

/**************************************************************************//*!
*
* @brief Add new byte to CRC8 calculation
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
* @brief Get array of random numbers
*
******************************************************************************/

#if FMSTR_CFG_F1_RESTRICTED_ACCESS

FMSTR_BPTR FMSTR_RandomNumbersToBuffer(FMSTR_U8 *out, FMSTR_SIZE length)
{
    FMSTR_INDEX i;
    FMSTR_SIZE sz;
    FMSTR_U32 r;
    FMSTR_U8* dest = out;
    FMSTR_U8* ret = out;
    FMSTR_U8 div;

    for(i=0; i<length; i+=4)
    {
        /* TODO: this generator uses stdlib rand implemntation by default, which is weak.
         * replace this by defining your own FMSTR_Rand() macro and replace it by
         * a true random number engine. */

        /* Achieve somewhat better entropy by skipping random number of sequenced numbers */
        div = (FMSTR_U8)FMSTR_Rand();
        while(div-- > 0)
            r = FMSTR_Rand();

        sz = length - i;
        if(sz > 4)
            sz = 4;

        ret = FMSTR_CopyToBuffer(&dest[i], (FMSTR_ADDR)&r, sz);
    }

    return ret;
}

#endif /* FMSTR_CFG_F1_RESTRICTED_ACCESS */

/**************************************************************************//*!
*
* @brief Get array of random numbers
*
******************************************************************************/

#if FMSTR_CFG_F1_RESTRICTED_ACCESS

void FMSTR_Randomize(FMSTR_U32 entropy)
{
    /* skip next few numbers in rand sequence to achieve a better behavior */
    entropy &= 15;

    do
    {
        FMSTR_Rand();
    } while(entropy-- > 0);
}

#endif /* FMSTR_CFG_F1_RESTRICTED_ACCESS */

/**************************************************************************//*!
*
* @brief The function prepares ring buffer
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
* @brief The function adds character into ring buffer
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
* @brief The function gets character from ring buffer
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
* @brief The function returns true, when is space in ring buffer
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
* @brief The function returns true, when some data in ring buffer
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
