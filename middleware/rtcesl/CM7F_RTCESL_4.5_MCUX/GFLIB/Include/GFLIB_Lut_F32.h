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
* @brief  Look-up table algorithm using linear interpolation
* 
*******************************************************************************/
#ifndef _GFLIB_Lut1D_F32_H_
#define _GFLIB_Lut1D_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib.h"
#include "gflib_types.h"

/*******************************************************************************
* Macros 
*******************************************************************************/  
#define GFLIB_Lut1D_F32_C(f32X, pf32Table, u16TableSize)                       \
        GFLIB_Lut1D_F32_FC(f32X, pf32Table, u16TableSize)  

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern frac32_t GFLIB_Lut1D_F32_FC(frac32_t f32X,
                                   const frac32_t *pf32Table,
                                   uint16_t u16TableSize);

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_Lut1D_F32_H_ */
