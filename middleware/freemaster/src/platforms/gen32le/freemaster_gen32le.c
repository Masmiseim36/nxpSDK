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
 * FreeMASTER Communication Driver - Generic 32-bit little endian platform specific code
 */

#include "freemaster.h"
#include "freemaster_private.h"
#include "freemaster_utils.h"

#if !(FMSTR_DISABLE)

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
        for(int i=0; i<sizeof(type)*8; i+=8,ps8++,pd8++,pm8++)  \
            s |= (type)(((*ps8) & (*pm8)) | ((*pd8) & ~(*pm8)))<<i; \
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
    /* 64-bit aligned part */
    while(size >= sizeof(FMSTR_U64))
    {
        COPY_TO_POSTINCREMENT(dest, src, FMSTR_U64);
        size -= sizeof(FMSTR_U64);
    }
    /* remaining word */
    if(size >= sizeof(FMSTR_U32))
    {
        COPY_TO_POSTINCREMENT(dest, src, FMSTR_U32);
        size -= sizeof(FMSTR_U32);
    }
    /* remaining halfword */
    if(size >= sizeof(FMSTR_U16))
    {
        COPY_TO_POSTINCREMENT(dest, src, FMSTR_U16);
        size -= sizeof(FMSTR_U16);
    }
    /* remaining byte */
    if(size >= sizeof(FMSTR_U8))
    {
        COPY_TO_POSTINCREMENT(dest, src, FMSTR_U8);
        size -= sizeof(FMSTR_U8);
    }

    FMSTR_ASSERT(size == 0);
}
/* in this helper call, we are already sure that the suorce pointer is 64-bit aligned */
static void _FMSTR_MemCpySrcAligned(volatile void* dest, volatile void* src, FMSTR_SIZE size)
{
    /* 64-bit aligned part */
    while(size >= sizeof(FMSTR_U64))
    {
        COPY_FROM_POSTINCREMENT(dest, src, FMSTR_U64);
        size -= sizeof(FMSTR_U64);
    }
    /* remaining word */
    if(size >= sizeof(FMSTR_U32))
    {
        COPY_FROM_POSTINCREMENT(dest, src, FMSTR_U32);
        size -= sizeof(FMSTR_U32);
    }
    /* remaining halfword */
    if(size >= sizeof(FMSTR_U16))
    {
        COPY_FROM_POSTINCREMENT(dest, src, FMSTR_U16);
        size -= sizeof(FMSTR_U16);
    }
    /* remaining byte */
    if(size >= sizeof(FMSTR_U8))
    {
        COPY_FROM_POSTINCREMENT(dest, src, FMSTR_U8);
        size -= sizeof(FMSTR_U8);
    }

    FMSTR_ASSERT(size == 0);
}

/* in this helper call, we are already sure that the required pointer is 64-bit aligned */
static void _FMSTR_MemCpyMaskedDstAligned(volatile void* dest, volatile void* src, volatile void* mask, FMSTR_SIZE size)
{
    /* 64-bit aligned part */
    while(size >= sizeof(FMSTR_U64))
    {
        COPY_TO_MASKED_POSTINCREMENT(dest, src, mask, FMSTR_U64);
        size -= sizeof(FMSTR_U64);
    }
    /* remaining word */
    if(size >= sizeof(FMSTR_U32))
    {
        COPY_TO_MASKED_POSTINCREMENT(dest, src, mask, FMSTR_U32);
        size -= sizeof(FMSTR_U32);
    }
    /* remaining halfword */
    if(size >= sizeof(FMSTR_U16))
    {
        COPY_TO_MASKED_POSTINCREMENT(dest, src, mask, FMSTR_U16);
        size -= sizeof(FMSTR_U16);
    }
    /* remaining byte */
    if(size >= sizeof(FMSTR_U8))
    {
        COPY_TO_MASKED_POSTINCREMENT(dest, src, mask, FMSTR_U8);
        size -= sizeof(FMSTR_U8);
    }

    FMSTR_ASSERT(size == 0);
}

/**************************************************************************//*!
*
* @brief  Copy data from address in memory into buffer. Read from memory is aligned to 64, 32 or 16 bytes - depends on platform.
*
* @param  destBuff - pointer to destination memory in communication buffer
* @param  srcAddr  - source memory address
* @param  size      - buffer size (always in bytes)
*
* @return This function returns a pointer to next byte in comm. buffer
*
******************************************************************************/

void FMSTR_MemCpyFrom(volatile void* destBuff, volatile void* srcAddr, FMSTR_SIZE size)
{
    /* misaligned odd byte */
    if(TEST_MISALIGNED(srcAddr, 1) && size >= sizeof(FMSTR_U8))
    {
        COPY_FROM_POSTINCREMENT(destBuff, srcAddr, FMSTR_U8);
        size -= sizeof(FMSTR_U8);
    }
    /* misaligned odd halfword */
    if(TEST_MISALIGNED(srcAddr, 2) && size >= sizeof(FMSTR_U16))
    {
        COPY_FROM_POSTINCREMENT(destBuff, srcAddr, FMSTR_U16);
        size -= sizeof(FMSTR_U16);
    }
    /* misaligned odd word */
    if(TEST_MISALIGNED(srcAddr, 3) && size >= sizeof(FMSTR_U32))
    {
        COPY_FROM_POSTINCREMENT(destBuff, srcAddr, FMSTR_U32);
        size -= sizeof(FMSTR_U32);
    }
    /* the rest is already aligned */
    _FMSTR_MemCpySrcAligned(destBuff, srcAddr, size);
}

/**************************************************************************//*!
*
* @brief  Copy data from buffer into specific address in memory. Write into memory is aligned to 64, 32 or 16 bytes - depends on platform.
*
* @param  destAddr - destination memory address
* @param  srcBuff  - pointer to source memory in communication buffer
* @param  size     - buffer size (always in bytes)
*
* @return This function returns a pointer to next byte in comm. buffer
*
******************************************************************************/

void FMSTR_MemCpyTo(volatile void* destAddr, volatile void* srcBuff, FMSTR_SIZE size)
{
    /* misaligned odd byte */
    if(TEST_MISALIGNED(destAddr, 1) && size >= sizeof(FMSTR_U8))
    {
        COPY_TO_POSTINCREMENT(destAddr, srcBuff, FMSTR_U8);
        size -= sizeof(FMSTR_U8);
    }
    /* misaligned odd halfword */
    if(TEST_MISALIGNED(destAddr, 2) && size >= sizeof(FMSTR_U16))
    {
        COPY_TO_POSTINCREMENT(destAddr, srcBuff, FMSTR_U16);
        size -= sizeof(FMSTR_U16);
    }
    /* misaligned odd word */
    if(TEST_MISALIGNED(destAddr, 3) && size >= sizeof(FMSTR_U32))
    {
        COPY_TO_POSTINCREMENT(destAddr, srcBuff, FMSTR_U32);
        size -= sizeof(FMSTR_U32);
    }
    /* the rest is already aligned */
    _FMSTR_MemCpyDstAligned(destAddr, srcBuff, size);
}

/**************************************************************************//*!
*
* @brief  Copy data from buffer into specific address in memory (register). Write into memory is aligned to 64, 32 or 16 bytes - depends on platform.
*
* @param  destAddr - destination memory address (register)
* @param  srcBuff  - pointer to source memory in communication buffer
* @param  size     - buffer size (always in bytes)
*
* @return This function returns a pointer to next byte in comm. buffer
*
******************************************************************************/
void FMSTR_MemCpyToMasked(volatile void* destAddr, volatile void* srcBuff, volatile void* maskBuff, FMSTR_SIZE size)
{
    /* misaligned odd byte */
    if(TEST_MISALIGNED(destAddr, 1) && size >= sizeof(FMSTR_U8))
    {
        COPY_TO_MASKED_POSTINCREMENT(destAddr, srcBuff, maskBuff, FMSTR_U8);
        size -= sizeof(FMSTR_U8);
    }
    /* misaligned odd halfword */
    if(TEST_MISALIGNED(destAddr, 2) && size >= sizeof(FMSTR_U16))
    {
        COPY_TO_MASKED_POSTINCREMENT(destAddr, srcBuff, maskBuff, FMSTR_U16);
        size -= sizeof(FMSTR_U16);
    }
    /* misaligned odd word */
    if(TEST_MISALIGNED(destAddr, 3) && size >= sizeof(FMSTR_U32))
    {
        COPY_TO_MASKED_POSTINCREMENT(destAddr, srcBuff, maskBuff, FMSTR_U32);
        size -= sizeof(FMSTR_U32);
    }
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

FMSTR_BPTR FMSTR_CopyToBuffer(FMSTR_BPTR destBuff, FMSTR_ADDR srcAddr, FMSTR_SIZE size)
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

FMSTR_BPTR FMSTR_CopyFromBuffer(FMSTR_ADDR destAddr, FMSTR_BPTR srcBuff, FMSTR_SIZE size)
{
    FMSTR_MemCpyTo(destAddr, srcBuff, size);
    return (FMSTR_BPTR) (((FMSTR_U8*)srcBuff)+size);
}

FMSTR_BPTR FMSTR_ValueFromBuffer16(FMSTR_U16* dest, FMSTR_BPTR src)
{
    return FMSTR_CopyFromBuffer((FMSTR_ADDR)(FMSTR_U8*)dest, src, 2U);
}

FMSTR_BPTR FMSTR_ValueFromBuffer32(FMSTR_U32* dest, FMSTR_BPTR src)
{
    return FMSTR_CopyFromBuffer((FMSTR_ADDR)(FMSTR_U8*)dest, src, 4U);
}

FMSTR_BPTR FMSTR_ValueToBuffer16(FMSTR_BPTR dest, FMSTR_U16 src)
{
    return FMSTR_CopyToBuffer(dest, (FMSTR_ADDR)(FMSTR_U8*)&src, 2U);
}

FMSTR_BPTR FMSTR_ValueToBuffer32(FMSTR_BPTR dest, FMSTR_U32 src)
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

void FMSTR_CopyFromBufferWithMask(FMSTR_ADDR destAddr, FMSTR_BPTR srcBuff, FMSTR_SIZE size)
{
    FMSTR_MemCpyToMasked(destAddr, srcBuff, srcBuff+size, size);
}

/******************************************************************************/

/**************************************************************************//*!
*
* @brief  Store address in LEB format to communication buffer.
*
******************************************************************************/

FMSTR_BPTR FMSTR_AddressToBuffer(FMSTR_BPTR dest, FMSTR_ADDR addr)
{
    return FMSTR_UlebEncode(dest, 5, (FMSTR_SIZE)addr);
}

/**************************************************************************//*!
*
* @brief  Fetch address in LEB format from communication buffer
*
******************************************************************************/

FMSTR_BPTR FMSTR_AddressFromBuffer(FMSTR_ADDR* paddr, FMSTR_BPTR src)
{
    return FMSTR_UlebDecode(src, (FMSTR_SIZE*)paddr);
}

/**************************************************************************//*!
*
* @brief  Store size in LEB format to communication buffer.
*
******************************************************************************/

FMSTR_BPTR FMSTR_SizeToBuffer(FMSTR_BPTR dest, FMSTR_SIZE size)
{
    return FMSTR_UlebEncode(dest, 5, size);
}

/**************************************************************************//*!
*
* @brief  Fetch size in LEB format from communication buffer
*
******************************************************************************/

FMSTR_BPTR FMSTR_SizeFromBuffer(FMSTR_SIZE* psize, FMSTR_BPTR src)
{
    return FMSTR_UlebDecode(src, psize);
}

/**************************************************************************//*!
*
* @brief  Store index in signed LEB format to communication buffer.
*
******************************************************************************/

FMSTR_BPTR FMSTR_IndexToBuffer(FMSTR_BPTR dest, FMSTR_INDEX index)
{
    return FMSTR_SlebEncode(dest, 5, index);
}

/**************************************************************************//*!
*
* @brief  Fetch index in signed LEB format from communication buffer
*
******************************************************************************/

FMSTR_BPTR FMSTR_IndexFromBuffer(FMSTR_INDEX* pindex, FMSTR_BPTR src)
{
    return FMSTR_SlebDecode(src, pindex);
}

/**************************************************************************//*!
*
* @brief  Return number of bytes that given address needs to add in order to
*         get properly aligned.
*
******************************************************************************/

FMSTR_SIZE FMSTR_GetAlignmentCorrection(FMSTR_ADDR addr, FMSTR_SIZE size)
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

#endif /* !(FMSTR_DISABLE) */
