/*
*         Copyright (c), NXP Semiconductors Gratkorn / Austria
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
*particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* Type definitions for Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
*
* Note: If needed, include platform specific header file before
*       this file over-rides other defines and values.
*
*/

#ifndef PH_TYPEDEFS_H
#define PH_TYPEDEFS_H

#ifdef linux
#include <sys/types.h>
#endif

/** \defgroup ph_Typedefs Type Definitions
* \brief Contains definitions for types defined within NxpRdLib
* @{
*/

#ifndef _unaligned_defined
#define _unaligned_defined
#ifdef _WIN64
#define UNALIGNED __unaligned
#else
#define UNALIGNED
#endif
#endif

/* Enable/disable printf statements based on DTA_DEBUG macro */
#if defined NXPBUILD__PH_DEBUG
#   ifdef _WIN32
#      pragma warning(push)           /* PRQA S 3116 */
#      pragma warning(disable:4001)   /* PRQA S 3116 */
#      include <stdio.h>
#      pragma warning(pop)            /* PRQA S 3116 */
#   else
#      include <stdio.h>
#   endif  /* WIN32 */
#   define PRINT(...)          printf(__VA_ARGS__) /* PRQA S 1030 */
#else
#   define PRINT(...)                              /* PRQA S 1030 */
#endif

#if defined(__GNUC__ ) /* Toolchain with StdInt */
#    include <stdint.h>
#    ifdef __CODE_RED
#       include <lpc_types.h>
#    endif
#    include <stdbool.h>
#elif defined(__ICCARM__)
#   include "intrinsics.h"
#    include <stdint.h>

#elif defined(__CC_ARM)
#    include <stdint.h>
#else /* Toolchain not-with StdInt */

/** \name Unsigned Types
*/
/*@{*/
#ifndef __uint8_t_defined
/**
* \brief 8 bit unsigned integer
*/
typedef unsigned char uint8_t;
#endif /* __uint8_t_defined */
#ifndef __uint16_t_defined
/**
* \brief 16 bit unsigned integer
*/
typedef unsigned short uint16_t;
#endif /* __uint16_t_defined */
#ifndef __uint32_t_defined
/**
* \brief 32 bit and 64 bit unsigned integers
*/
#ifdef _WIN32
typedef unsigned long uint32_t;
typedef unsigned long long uint64_t;
#else
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
#endif /* _WIN32 */
#endif /* __uint32_t_defined */

/** \name Signed Types
*/
/*@{*/
#ifndef __int8_t_defined
/**
* \brief 8 bit signed integer
*/
typedef signed char int8_t;
#endif /* __int8_t_defined */
#ifndef __int16_t_defined
/**
* \brief 16 bit signed integer
*/
typedef short int16_t;
#endif /* __int16_t_defined */
#ifndef __int32_t_defined
/**
* \brief 32 bit signed integer
*/
#ifdef _WIN32
typedef long int32_t;
#else
typedef int int32_t;
#endif /* _WIN32 */
#endif /* __int32_t_defined */
/*@}*/

#endif  /* Toolchain with/not-with StdInt */

/** \name Floating-Point Types
*/
/*@{*/
#ifndef __float32_t_defined
/**
* \brief 32 bit floating point
*/
typedef float float32_t;
#endif /* __float32_t_defined */
/*@}*/

#ifndef __handle_defined
/**
*\brief Unsigned handle
*/
typedef uint32_t ph_NfcHandle;
#endif /* __handle_defined */
/*@}*/

/**
 * \brief Pointer to a 32 bits register
 */
typedef volatile uint32_t * pReg32_t;

/** \name Boolean Types
*/
/*@{*/
#ifndef BOOL_DEFINED
#    define BOOL_DEFINED

#ifdef bool
#	define TRUE     (true)
#	define FALSE 	(false)
#else
#	define TRUE     (0 == 0)
#	define FALSE 	(!(TRUE))
#   define bool unsigned char
#endif

/* MISRA 21.1                     */
/* # */ /* ifndef true            */
/* # */ /*     define true 1      */
/* # */ /* endif                  */
/*   */ /*                        */
/* # */ /* ifndef false           */
/* # */ /*     define false 0     */
/* # */ /* endif                  */


#endif /* __BOOL_DEFINED */
/*@}*/

/** \name Other Types
*/
/*@{*/
/**
* \brief phcsBfl_Status_t is a signed short value, using the positive range.
*
* High byte: Category (group) Identifier.\n
* Low byte : Error Specifier.
*/
typedef uint16_t phStatus_t;

/* #ifndef NULL         */
/* #    define NULL 0   */
/* #endif               */
/*@}*/

/** @}
* end of ph_Typedefs group
*/

#endif /* PH_TYPEDEFS_H */
