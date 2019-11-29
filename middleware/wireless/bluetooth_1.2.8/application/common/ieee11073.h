/*! *********************************************************************************
 * \addtogroup IEEE 11073
 * @{
 ********************************************************************************** */
/*!
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* file
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _IEEE_11073_DEFINES_H_
#define _IEEE_11073_DEFINES_H_

/************************************************************************************
*************************************************************************************
* Public constants & macros
*************************************************************************************
************************************************************************************/

/*! IEEE 11073-20601 - Not a number 16 bit float */
#define gIeee11073_16Bit_NaN_c      0x07FF

/*! IEEE 11073-20601 - Not a number 32 bit float */
#define gIeee11073_32Bit_NaN_c      0x07FFFFFF

/*! IEEE 11073-20601 - Not at this resolution 16 bit float*/
#define gIeee11073_16Bit_Nres_c     0x0800

/*! IEEE 11073-20601 - Not at this resolution 32 bit float*/
#define gIeee11073_32Bit_Nres_c     0x08000000

/*! IEEE 11073-20601 - Plus Infinity 16 bit float*/
#define gIeee11073_16Bit_PlusInf   0x07FE

/*! IEEE 11073-20601 - Plus Infinity 32 bit float*/
#define gIeee11073_32Bit_PlusInf   0x07FFFFFE

/*! IEEE 11073-20601 - Minus Infinity 16 bit float*/
#define gIeee11073_16Bit_MinusInf  0x0802

/*! IEEE 11073-20601 - Minus Infinity 32 bit float*/
#define gIeee11073_32Bit_MinusInf  0x08000002

/*! IEEE 11073-20601 - Reserved for future use 16 bit float*/
#define gIeee11073_16Bit_Reserved  0x0801

/*! IEEE 11073-20601 - Reserved for future use 32 bit float*/
#define gIeee11073_32Bit_Reserved  0x08000001

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/*! IEEE 11073-20601 - 16 bit float*/
typedef uint16_t ieee11073_16BitFloat_t;

typedef enum
{
    gExponent16_c = 0x000F,
    gMantissa16_c = 0xFFF0
} ieee11073_16BitFloat_tag;

/*! IEEE 11073-20601 - 32 bit float*/
typedef uint32_t ieee11073_32BitFloat_t;

typedef enum
{
    gExponent32_c = 0x000000FF,
    gMantissa32_c = 0xFFFFFF00
} ieee11073_32BitFloat_tag;

#endif /* _IEEE_11073_DEFINES_H_ */

/*! *********************************************************************************
* @}
********************************************************************************** */
