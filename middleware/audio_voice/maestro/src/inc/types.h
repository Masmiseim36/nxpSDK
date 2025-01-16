///////////////////////////////////////////////////////////////////////////////
//! \addtogroup cci_type CCI Types and Definitions
//! @{
//
// Copyright 2013-2014 Freescale, Inc.
//
// SPDX-License-Identifier: BSD-3-Clause
//
//! \file types.h
//! \brief Contains standard data types.
///////////////////////////////////////////////////////////////////////////////

#ifndef _TYPES_H
#define _TYPES_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint32_t val[4];
} uint128_t;

/*!
 * \brief CircularBuffer structure
 */
typedef struct __CircularBuffer
{
    unsigned char *pu8HeadPtr;  //!< buffer head pointer
    unsigned char *pu8TailPtr;  //!< buffer tail pointer
    unsigned char *pu8BaseAddr; //!< buffer base address
    unsigned char *pu8EndAddr;  //!< buffer end address
    unsigned int i32BufferSize; //!< buffer size
} CircularBuffer_t;
/*!
 * \brief Buffer Descriptor
 */
typedef struct
{
    CircularBuffer_t ReadBuffer;  //!< read circular buffer
    CircularBuffer_t WriteBuffer; //!< write circular buffer
    uint32_t minInput;            //!< minimum input frame
    uint32_t minOutput;           //!< minimum output space
} tBufferDesc;

#define CONFIGURE_CIRCULAR_DESC(desc, buffer, size)                                            \
    desc.pu8HeadPtr = desc.pu8TailPtr = desc.pu8BaseAddr = (uint8_t *)buffer;                  \
    desc.pu8EndAddr                                      = (uint8_t *)desc.pu8BaseAddr + size; \
    desc.i32BufferSize                                   = size;

#ifndef RETCODE
#define RETCODE int
#endif

#ifdef ERROR
#undef ERROR
#define ERROR -1
#endif

#ifdef NOERROR
#undef NOERROR
#define NOERROR 0
#endif
//------------------------------------------------------------------------------
// All of the following defs are included for compatability.  Please use the
// ANSI C99 defs for all new work.
//------------------------------------------------------------------------------

#ifdef FALSE
#undef FALSE
#define FALSE false
#endif

#ifdef TRUE
#undef TRUE
#define TRUE true
#endif

//! \brief
#ifdef NULL
#undef NULL
#define NULL 0
#endif

//! \brief
typedef bool BOOL;

//! \brief
typedef uint8_t UINT8;
//! \brief
typedef uint8_t BYTE;
//! \brief
typedef uint8_t PACKED_BYTE;

//! \brief
typedef uint16_t UINT16;
//! \brief
typedef uint16_t USHORT;
//! \brief
typedef uint16_t WCHAR;
//! \brief
typedef uint16_t UCS3;
//! \brief
typedef int16_t SHORT;

//! \brief
typedef uint32_t UINT32;
//! \brief
typedef uint32_t WORD;
//! \brief
typedef uint32_t SECTOR_BUFFER;
//! \brief
typedef uint32_t *P_SECTOR_BUFFER;

//! \brief
typedef uint64_t DWORD;
//! \brief
typedef int64_t INT64;
//! \brief
typedef int64_t UINT64;

//! \brief
typedef uint128_t UINT128;

//! \brief
typedef float FLOAT;

//! \brief
#define FRACT _fract
//! \brief
#define CIRC _circ

#ifndef __LANGUAGE_ASM__
#ifndef RT_STATUS_T_DEFINED
#define RT_STATUS_T_DEFINED
typedef int RtStatus_t;
#endif
#endif

//------------------------------------------------------------------------------
// SystemHalt macros in types.h?
//------------------------------------------------------------------------------

#ifdef __THUMB
#define SystemHalt() __asm(" .half 0xbebe")
#elif defined(_WIN32)
#ifdef __cplusplus
extern "C" {
#endif
void _assert(const char *, const char *, const int);
#ifdef __cplusplus
}
#endif
#define SystemHalt() _assert(__FILE__, __FUNCTION__, __LINE__)
#else
#define SystemHalt() __asm(" .word 0xbebebebe");
#endif
// Debug macros in types.h?
//------------------------------------------------------------------------------

//! \brief
#ifdef DEBUG
#define IFDEBUG(x) x
#else
#define IFDEBUG(x)
#endif

//------------------------------------------------------------------------------
// This sets the default build of the target
//------------------------------------------------------------------------------

//! \brief
#if !defined(HW_TARGET_ASIC) && !defined(HW_TARGET_SIMULATOR)
#define HW_TARGET_BRAZOS 1
#endif

//------------------------------------------------------------------------------
// Win32 compatibility?
//------------------------------------------------------------------------------

//! \brief
#ifdef _WIN32
#define inline __inline
#endif

#endif // #ifndef _TYPES_H

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
//! @}
