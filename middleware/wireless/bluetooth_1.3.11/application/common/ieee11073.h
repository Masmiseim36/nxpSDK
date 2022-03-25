/*! *********************************************************************************
 * \addtogroup IEEE 11073
 * @{
 ********************************************************************************* */
/*! *********************************************************************************
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef IEEE_11073_DEFINES_H
#define IEEE_11073_DEFINES_H

/************************************************************************************
*************************************************************************************
* Public constants & macros
*************************************************************************************
************************************************************************************/

/*! IEEE 11073-20601 - Not a number 16 bit float */
#define gIeee11073_16Bit_NaN_c      0x07FFU

/*! IEEE 11073-20601 - Not a number 32 bit float */
#define gIeee11073_32Bit_NaN_c      0x07FFFFFFU

/*! IEEE 11073-20601 - Not at this resolution 16 bit float*/
#define gIeee11073_16Bit_Nres_c     0x0800U

/*! IEEE 11073-20601 - Not at this resolution 32 bit float*/
#define gIeee11073_32Bit_Nres_c     0x08000000U

/*! IEEE 11073-20601 - Plus Infinity 16 bit float*/
#define gIeee11073_16Bit_PlusInf   0x07FEU

/*! IEEE 11073-20601 - Plus Infinity 32 bit float*/
#define gIeee11073_32Bit_PlusInf   0x07FFFFFEU

/*! IEEE 11073-20601 - Minus Infinity 16 bit float*/
#define gIeee11073_16Bit_MinusInf  0x0802U

/*! IEEE 11073-20601 - Minus Infinity 32 bit float*/
#define gIeee11073_32Bit_MinusInf  0x08000002U

/*! IEEE 11073-20601 - Reserved for future use 16 bit float*/
#define gIeee11073_16Bit_Reserved  0x0801U

/*! IEEE 11073-20601 - Reserved for future use 32 bit float*/
#define gIeee11073_32Bit_Reserved  0x08000001U

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
    gMantissa32_c = 0xFFFFFF00U
} ieee11073_32BitFloat_tag;

#endif /* IEEE_11073_DEFINES_H */

/*! *********************************************************************************
* @}
********************************************************************************** */
