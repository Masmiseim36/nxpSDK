/*******************************************************************************
*
* Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
* Copyright 2016-2021, 2024 NXP
*
* NXP Proprietary. This software is owned or controlled by NXP and may
* only be used strictly in accordance with the applicable license terms. 
* By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that
* you have read, and that you agree to comply with and are bound by,
* such license terms.  If you do not agree to be bound by the applicable
* license terms, then you may not retain, install, activate or otherwise
* use the software.
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
