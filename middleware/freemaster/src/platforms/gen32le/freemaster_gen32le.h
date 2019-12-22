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

#ifndef _FREEMASTER_GEN32LE_H
#define _FREEMASTER_GEN32LE_H

#include <string.h>
#include <stdlib.h>

/******************************************************************************
 * platform-specific default configuration
 ******************************************************************************/

#define FMSTR_CFG_BUS_WIDTH         1
#define FMSTR_TSA_FLAGS             0
#define FMSTR_PLATFORM_BIG_ENDIAN   0
#define FMSTR_MEMCPY_MAX_SIZE       4

#ifndef FMSTR_PLATFORM_BASE_ADDRESS
	/* Use Cortex-M memory base address as a default for optimized access. */
	#define FMSTR_PLATFORM_BASE_ADDRESS 0x20000000
#endif

#ifdef __cplusplus
  extern "C" {
#endif

/******************************************************************************
* Types definition
******************************************************************************/

/* FreeMASTER types used */
typedef unsigned char* FMSTR_ADDR;      /* CPU address type (4bytes) */
typedef unsigned int   FMSTR_SIZE;      /* general size type (at least 16 bits) */
typedef unsigned char  FMSTR_SIZE8;     /* one-byte size value */
typedef unsigned long  FMSTR_SIZE32;    /* general size type (at least size of address (typicaly 32bits)) */
typedef unsigned int   FMSTR_BOOL;      /* general boolean type  */

typedef unsigned char  FMSTR_U8;        /* smallest memory entity */
typedef unsigned short FMSTR_U16;       /* 16bit value */
typedef unsigned long  FMSTR_U32;       /* 32bit value */
typedef unsigned long long FMSTR_U64;   /* 64bit value */

typedef signed char    FMSTR_S8;        /* signed 8bit value */
typedef signed short   FMSTR_S16;       /* signed 16bit value */
typedef signed long    FMSTR_S32;       /* signed 32bit value */
typedef signed long long FMSTR_S64;     /* signed 64bit value */

typedef float          FMSTR_FLOAT;     /* float value */
typedef double         FMSTR_DOUBLE;    /* double value */

typedef unsigned char  FMSTR_FLAGS;     /* type to be union-ed with flags (at least 8 bits) */
typedef signed int     FMSTR_INDEX;     /* general for-loop index (must be signed) */

typedef unsigned char  FMSTR_BCHR;      /* type of a single character in comm.buffer */
typedef unsigned char* FMSTR_BPTR;      /* pointer within a communication buffer */

typedef char           FMSTR_CHAR;      /* regular character, part of string */


#ifdef __cplusplus
  }
#endif

/*********************************************************************************
* Platform depending functionalities
*********************************************************************************/

/* Redirect to standard C functions */
#define FMSTR_StrCmp(str1, str2)        strcmp((const char*)str1, (const char*)str2)
#define FMSTR_StrLen(str)               strlen((const char*)str)
#define FMSTR_Rand()                    rand()
#define FMSTR_MemSet(dest, mask, size)  memset((void*)dest, mask, size)
#define FMSTR_MemCmp(b1, b2, size)      memcmp((void*)b1, (void*)b2, size)

/****************************************************************************************
* Other helper macros
*****************************************************************************************/

/* This macro assigns C pointer to FMSTR_ADDR-typed variable */
#define FMSTR_PTR2ADDR(tmpAddr,ptr) ( tmpAddr = (FMSTR_ADDR) (FMSTR_U8*) ptr )
#define FMSTR_ARR2ADDR FMSTR_PTR2ADDR

#endif /* _FREEMASTER_GEN32LE_H */
