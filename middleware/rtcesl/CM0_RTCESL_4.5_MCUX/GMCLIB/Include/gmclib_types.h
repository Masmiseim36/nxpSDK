/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
* 
*
****************************************************************************//*!
*
* @brief  GMCLIB Types  
* 
*******************************************************************************/
#ifndef _GMCLIB_TYPES_H_
#define _GMCLIB_TYPES_H_

#if defined(__cplusplus) 
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/ 
#include "mlib_types.h"

/******************************************************************************
* Macros 
******************************************************************************/  
#define GMCLIB_CONST    
  
/* GMCLIB_ONE_DIV_SQRT3_F16 define, the value is: 32768 / sqrt(3) = 18919 = 0x49E7 */
#define GMCLIB_ONE_DIV_SQRT3_F16  (frac16_t)18919
/* GMCLIB_SQRT3_DIV_2_F16 define, the value is: 32768 * sqrt(3) / 2 = 28378 = 0x6EDA */  
#define GMCLIB_SQRT3_DIV_2_F16    (frac16_t)28378

/*******************************************************************************
* Types 
********************************************************************************/ 
/* Integer data types */

/* The GMCLIB_3COOR_T_F16 structure type corresponds to the three-phase stationary
coordinate system based on the A, B, C components. */
typedef struct
{
    frac16_t f16A;
    frac16_t f16B;
    frac16_t f16C;
} GMCLIB_3COOR_T_F16;

/* The GMCLIB_2COOR_ALBE_T_F16 structure type corresponds to the two-phase
stationary coordinate system based on the Alpha and Beta orthogonal components. */
typedef struct
{
    frac16_t f16Alpha;
    frac16_t f16Beta;
} GMCLIB_2COOR_ALBE_T_F16;

/* The GMCLIB_2COOR_DQ_T_F16 structure type corresponds to the two-phase rotating
coordinate system based on the D and Q orthogonal components. */
typedef struct
{
    frac16_t f16D;
    frac16_t f16Q;
} GMCLIB_2COOR_DQ_T_F16;

/* The GMCLIB_2COOR_SINCOS_T_F16 structure type corresponds to the two-phase
coordinate system based on the Sin and Cos components of certain angle. */
typedef struct
{
    frac16_t f16Sin;
    frac16_t f16Cos;
} GMCLIB_2COOR_SINCOS_T_F16;

/* The GMCLIB_2COOR_DQ_T_F32 structure type corresponds to the two-phase rotating
coordinate system based on the D and Q orthogonal components. */
typedef struct
{
    frac32_t f32D;
    frac32_t f32Q;
} GMCLIB_2COOR_DQ_T_F32;

/* The GMCLIB_2COOR_ALBE_T_F16 structure type corresponds to the two-phase
stationary coordinate system based on the Alpha and Beta orthogonal components. */
typedef struct
{
    frac32_t f32Alpha;
    frac32_t f32Beta;
} GMCLIB_2COOR_ALBE_T_F32;

/* The GMCLIB_2COOR_AB_T_F16 structure type corresponds to the two-phase stationary
coordinate system based on the A and B components. */
    
typedef struct
{
    frac16_t f16A;   /* First argument */
    frac16_t f16B;   /* Second argument */
} GMCLIB_2COOR_AB_T_F16;   
  
/* The GMCLIB_2COOR_AB_T_F32 structure type corresponds to the two-phase stationary
coordinate system based on the A and B components. */

typedef struct
{
    frac32_t f32A;   /* First argument */
    frac32_t f32B;   /* Second argument */
} GMCLIB_2COOR_AB_T_F32; 
#if defined(__cplusplus) 
}
#endif

#endif /* _GMCLIB_TYPES_H_ */



