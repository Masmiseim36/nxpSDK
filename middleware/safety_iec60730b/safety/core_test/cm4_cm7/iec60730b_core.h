/************************************************************************************************
 * Copyright 2015 Freescale Semiconductor, Inc.
 * Copyright 2015-2019 NXP.
 *
 * License: LA_OPT_NXP_Software_License
 *
 * This software is owned or controlled by NXP and may
 * only be used strictly in accordance with the applicable license terms.
 * By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that
 * you have read, and that you agree to comply with and are bound by,
 * such license terms.  If you do not agree to be bound by the applicable
 * license terms, then you may not retain, install, activate or otherwise
 * use the software.
 *
 *
 * @version   4.0
 *
 * @brief     IEC60730 Class B Safety Routines core header file CM4, CM7
 *
 ***********************************************************************************************/

#ifndef _IEC60730B_CORE_
#define _IEC60730B_CORE_

/******************************************************************************
* Types
******************************************************************************/
#ifndef __ASM__

#include <stdint.h>

#if !defined(__MSL_BUILD__)
#if !defined( __STDINT_DECLS)

/* Signed and Unsigned integer data types */
/* 8 bits */
#if !defined( __int8_t_defined)
#if !defined(__INT8_T_TYPE__)
typedef signed char             int8_t;
#endif /* __INT8_T_TYPE__ */
#if !defined(__INT8_T_TYPE__)
typedef unsigned char           uint8_t;
#endif /* __INT8_T_TYPE__ */
#define __int8_t_defined        1
#endif /* __int8_t_defined */

/* 16 bits */
#if !defined( __int16_t_defined)
#if !defined(__INT16_T_TYPE__)
typedef signed short            int16_t;
#endif /* __INT16_T_TYPE__ */
#if !defined(__INT16_T_TYPE__)
typedef unsigned short          uint16_t;
#endif /* __INT16_T_TYPE__ */
#define __int16_t_defined       1
#endif /* __int16_t_defined */

/* 32 bits */
#if !defined( __int32_t_defined)
#if !defined(__STDINT_H_INCLUDED)
#if !defined(__INT32_T_TYPE__)
typedef signed long             int32_t;
#endif /* __INT32_T_TYPE__ */
#if !defined(__INT32_T_TYPE__)
typedef unsigned long           uint32_t;
#endif /* __INT32_T_TYPE__ */
#define __int32_t_defined       1
#endif /* __STDINT_H_INCLUDED */
#endif /* __int32_t_defined */

/* 64 bits */
#if !defined( __int64_t_defined)
#if !defined(__INT64_T_TYPE__)
typedef signed long long        int64_t;
#endif /* __INT64_T_TYPE__ */
#if !defined(__INT64_T_TYPE__)
typedef unsigned long long      uint64_t;
#endif /* __INT64_T_TYPE__ */
#define __int64_t_defined       1
#endif /* __int64_t_defined */

#endif /* __STDINT_DECLS */
#endif /* __MSL_BUILD__ */

/* Result type */
typedef uint32_t FS_RESULT;

/* Boolean data types */
typedef unsigned short bool_t;

#ifndef FALSE
#define FALSE    ((bool_t)0)
#endif
#ifndef TRUE
#define TRUE     ((bool_t)1)
#endif

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
