/*******************************************************************************
*
* Copyright 2013 - 2016, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
*
* This software is owned or controlled by NXP and may only be used
* strictly in accordance with the applicable license terms.  By expressly
* accepting such terms or by downloading, installing, activating and/or
* otherwise using the software, you are agreeing that you have read, and
* that you agree to comply with and are bound by, such license terms.  If
* you do not agree to be bound by the applicable license terms, then you
* may not retain, install, activate or otherwise use the software.
* 
*
*******************************************************************************/
#ifndef _IEC60730B_CORE_
#define _IEC60730B_CORE_

/******************************************************************************
* Types
******************************************************************************/
#ifndef __ASM__

#include <stdint.h>

#if !defined(__MSL_BUILD__)
#if !defined( __STDINT_DECLS)

/* Integer data types (signed and unsigned) */
/* 8 bits */
#if (!defined(__INT8_T_TYPE__) && !defined(__INT8_TYPE__))
    typedef signed char   int8_t;
    typedef unsigned char uint8_t;
#endif

/* 16 bits */
#if (!defined(__INT16_T_TYPE__) && !defined(__INT16_TYPE__))
    typedef signed short   int16_t;
    typedef unsigned short uint16_t;
#endif

/* 32 bits */
#if (!defined(__INT32_T_TYPE__) && !defined(__INT32_TYPE__))
    typedef signed long   int32_t;
    typedef unsigned long uint32_t;
#endif

/* 64 bits */
#if (!defined(__INT64_T_TYPE__) && !defined(__INT64_TYPE__))
    typedef signed long long   int64_t;
    typedef unsigned long long uint64_t;
#endif

#endif /* __STDINT_DECLS */
#endif /* __MSL_BUILD__ */

/* Result type */
typedef uint32_t FS_RESULT;

/* Boolean data types */
typedef uint16_t bool_t;
#define FALSE    ((bool_t)0)
#define TRUE     ((bool_t)1)

/* Float data types */
typedef float    float_t;

#endif /* __ASM__ */

/******************************************************************************
* Includes
******************************************************************************/
#include "iec60730b_cm4_cm7_flash.h"
#include "iec60730b_cm4_cm7_pc.h"
#include "iec60730b_cm4_cm7_ram.h"
#include "iec60730b_cm4_cm7_reg.h"
#include "iec60730b_cm4_cm7_stack.h"

#endif /* _IEC60730B_CORE_ */
