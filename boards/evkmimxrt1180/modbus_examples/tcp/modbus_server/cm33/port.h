/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <assert.h>

#ifndef _PORT_H
#define _PORT_H
  
#define	INLINE                    inline
#define PR_BEGIN_EXTERN_C         extern "C" {
#define	PR_END_EXTERN_C           }

/*! \brief If Modbus ASCII support is enabled. */
#define MB_ASCII_ENABLED                        (  0 )

/*! \brief If Modbus RTU support is enabled. */
#define MB_RTU_ENABLED                          (  0 )

/*! \brief If Modbus TCP support is enabled. */
#define MB_TCP_ENABLED                          (  1 )
  
#define ENTER_CRITICAL_SECTION( ) 
#define EXIT_CRITICAL_SECTION( )  
  
typedef int8_t    BOOL;
  
typedef uint8_t   UCHAR;
typedef int8_t    CHAR;
  
typedef uint16_t  USHORT;
typedef int16_t   SHORT;
  
typedef uint32_t  ULONG;
typedef int32_t   LONG;
  
#ifndef TRUE
#define TRUE      1
#endif
  
#ifndef FALSE
#define FALSE     0
#endif
  
#endif