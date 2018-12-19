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
* @brief  Vector limit functions
* 
*******************************************************************************/
#ifndef _GFLIB_VECTORLIMIT_F16_H_
#define _GFLIB_VECTORLIMIT_F16_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "gflib.h"  
 
/*******************************************************************************
* Macros 
*******************************************************************************/  
#define GFLIB_VectorLimit_F16_C(psVectorIn, f16Lim, psVectorOut)               \
        GFLIB_VectorLimit_F16_FC(psVectorIn, f16Lim, psVectorOut)              
#define GFLIB_VectorLimit1_F16_C(psVectorIn, f16Lim, psVectorOut)              \
        GFLIB_VectorLimit1_F16_FC(psVectorIn, f16Lim, psVectorOut)
        
/*******************************************************************************
* Types
*******************************************************************************/  
typedef struct
{
    frac16_t f16A;   /* First argument */
    frac16_t f16B;   /* Second argument */
} GFLIB_VECTORLIMIT_T_F16;
  
/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern bool_t GFLIB_VectorLimit_F16_FC(const GFLIB_VECTORLIMIT_T_F16 *psVectorIn, 
                                       frac16_t f16Lim, 
                                       GFLIB_VECTORLIMIT_T_F16 *psVectorOut);
extern bool_t GFLIB_VectorLimit1_F16_FC(const GFLIB_VECTORLIMIT_T_F16 *psVectorIn, 
                                       frac16_t f16Lim, 
                                       GFLIB_VECTORLIMIT_T_F16 *psVectorOut);
#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_VECTORLIMIT_F16_H_ */
