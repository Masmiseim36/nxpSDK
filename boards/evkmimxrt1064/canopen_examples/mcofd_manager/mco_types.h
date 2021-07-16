/**************************************************************************
MODULE:    MCO_TYPES.H
CONTAINS:  Data types used by MicroCANopen
COPYRIGHT: (c) Embedded Systems Academy (EmSA) 2002-2020
           All rights reserved. www.em-sa.com/nxp
DISCLAIM:  Read and understand our disclaimer before using this code!
           www.esacademy.com/disclaim.htm
           This software was written in accordance to the guidelines at
           www.esacademy.com/software/softwarestyleguide.pdf
LICENSE:   THIS IS THE NXP SDK VERSION OF MICROCANOPEN PLUS
           Licensed under a modified BSD License. See LICENSE.INFO
           file in the project root for full license information.
VERSION:   7.10, ESA 20-SEP-02
           $LastChangedDate: 2020-09-03 22:04:52 +0200 (Thu, 03 Sep 2020) $
           $LastChangedRevision: 5038 $
***************************************************************************/

#ifndef _MCO_TYPES_H
#define _MCO_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************
DEFINES: MEMORY TYPE OPTIMIZATION
**************************************************************************/

// CONST Object Dictionary Data
#define MEM_CONST const

// Process data
#define MEM_PROC

// buffers
#define MEM_BUF

// non-process data
#define MEM_FAR

/**************************************************************************
DEFINES: TRUE AND FALSE
**************************************************************************/
#ifndef TRUE
#define TRUE (1 == 1)
#endif
#ifndef FALSE
#define FALSE (!TRUE)
#endif

#ifndef NOT_SET
#define NOT_SET 2
#endif

/**************************************************************************
TYPEDEF: CANOPEN DATA TYPES
**************************************************************************/
#if !defined(UINTMAX_C) // if not stdint.h is already used in the project
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long int uint64_t;
typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int int64_t;
#endif // !defined(UINTMAX_C)

/**************************************************************************
TYPEDEF: CAN IDENTIFIER TYPE
         Plain CANopen does not use 29-bit IDs, use 16 here for memory
         optimization.
**************************************************************************/
#define CAN_ID_SIZE 16

#ifdef __cplusplus
}
#endif

#endif // _MCO_TYPES_H
