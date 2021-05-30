/*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*******************************************************************************
*
* @brief  mlib_types.h
* 
*******************************************************************************/
#ifndef _MLIB_TYPES_H_
#define _MLIB_TYPES_H_

#if defined(__cplusplus)
extern "C" {
#endif

/******************************************************************************
* Includes
******************************************************************************/
#include <stdint.h>                        /* stdint.h include for data types */

/* Include intrinsic functions */
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler   */
    #include <intrinsics.h>                /* To use intrinsic functions */
#endif

/*! Definition of inline. */
#if defined(__CC_ARM)
    #define inline __inline
#endif

/*******************************************************************************
* Types
*******************************************************************************/
/* Boolean data types */
typedef unsigned short bool_t;

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

/* Fractional data types */
typedef signed char             frac8_t; 
typedef signed short            frac16_t;           
typedef signed long             frac32_t;           
typedef signed long long        frac64_t;           

/* Accumulator data types */
typedef signed long             acc32_t;            
typedef signed long long        acc64_t;          

/* Float data types */
typedef float                   float_t;          

/*******************************************************************************
* Macros
*******************************************************************************/  
#define MLIB_CONST 

/* bool */
#ifndef FALSE
#define FALSE    ((bool_t)0)                       
#endif

#ifndef TRUE
#define TRUE     ((bool_t)1)                        
#endif

/* unsigned min, max */
#ifndef INT16_MIN
#define INT16_MIN               ((int16_t) 0x8000)
#endif

#ifndef INT16_MAX
#define INT16_MAX               ((int16_t) 0x7fff)
#endif

#ifndef INT32_MIN
#define INT32_MIN               ((int32_t) 0x80000000)
#endif

#ifndef INT32_MAX
#define INT32_MAX               ((int32_t) 0x7fffffff)
#endif

#ifndef INT64_MIN
#define INT64_MIN               ((int32_t) 0x8000000000000000)
#endif

#ifndef INT64_MAX
#define INT64_MAX               ((int32_t) 0x7fffffffffffffff)
#endif

/* unsigned min, max */
#ifndef UINT16_MAX
#define UINT16_MAX              ((uint16_t) 0x8000U)
#endif

#ifndef UINT32_MAX
#define UINT32_MAX              ((uint32_t) 0x80000000U)
#endif


/* Fractional conversion macros */
#if !defined(FRAC8)
#define FRAC8(x) ((frac8_t)((x) < 0.9921875 ? ((x) >= -1 ? (x)*0x80 : 0x80) : 0x7F))
#endif //FRAC8

#if !defined(FRAC16)
#define FRAC16(x) ((frac16_t)((x) < 0.999969482421875 ? ((x) >= -1 ? (x)*0x8000 : 0x8000) : 0x7FFF))
#endif //FRAC16

#if !defined(FRAC32)
#define FRAC32(x) ((frac32_t)((x) < 1 ? ((x) >= -1 ? (x)*0x80000000 : 0x80000000) : 0x7FFFFFFF))
#endif //FRAC32

#if !defined(FRAC64)
#define FRAC64(x) ((frac64_t)((x) < 1 ? ((x) >= -1 ? (x)*0x8000000000000000 : 0x8000000000000000) : 0x7FFFFFFFFFFFFFFF))
#endif //FRAC64

/* Accumulator conversion macros */
#if !defined(ACC16)
#define ACC16(x) ((acc16_t)((x) < 255.9921875 ? ((x) >= -256 ? (x)*0x80 : 0x8000) : 0x7FFF))
#endif //ACC16

#if !defined(ACC32)
#define ACC32(x) ((acc32_t)((x) < 65535.999969482421875 ? ((x) >= -65536 ? (x)*0x8000 : 0x80000000) : 0x7FFFFFFF))
#endif //ACC32

#if !defined(ACC64)
#define ACC64(x) ((acc64_t)((x) < 4294967295.9999999995343387126923 ? ((x) >= -4294967296 ? (x)*0x80000000 : 0x8000000000000000) : 0x7FFFFFFFFFFFFFFF))
#endif //ACC64

#if defined(__cplusplus)
}
#endif

#endif  /*_MLIB_TYPES_H_*/
