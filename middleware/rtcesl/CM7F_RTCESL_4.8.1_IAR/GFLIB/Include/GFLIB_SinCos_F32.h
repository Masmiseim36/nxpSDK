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
* @brief  Sine, Cosine
* 
*******************************************************************************/
#ifndef _GFLIB_SINCOS_A32_H_
#define _GFLIB_SINCOS_A32_H_

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
#define GFLIB_Sin_F16_Asm(f16Angle)              GFLIB_Sin_F16_FAsm(f16Angle, &gsSinCoef)
#define GFLIB_Sin_F16_AsmRam(f16Angle)           GFLIB_Sin_F16_FAsmRam(f16Angle, &gsSinCoef)
#define GFLIB_SinCos_F16_Ci(f16Angle, f16SinCos) GFLIB_SinCos_F16_FCi(f16Angle, f16SinCos)
#define GFLIB_Cos_F16_Asmi(f16Angle)             GFLIB_Cos_F16_FAsmi(f16Angle)

/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{
    frac32_t  f32A[5];    /* Array of fractional coefficients */
} GFLIB_SIN_T_F32;

/*******************************************************************************
* Global variables
*******************************************************************************/  
extern GFLIB_SIN_T_F32 gsSinCoef;

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern frac16_t GFLIB_Sin_F16_FAsm(frac16_t f16Angle, const GFLIB_SIN_T_F32 *psParam);
RAM_FUNC_LIB 
extern frac16_t GFLIB_Sin_F16_FAsmRam(frac16_t f16Angle, GFLIB_SIN_T_F32 *psParam);

/***************************************************************************//*!
* Cosine is calculated using the GFLIB_Sin_F16 function summed with FRAC(0.5)
*******************************************************************************/  
RAM_FUNC_LIB 
RTCESL_INLINE static inline frac16_t GFLIB_Cos_F16_FAsmi(register frac16_t f16Angle)
{
#if (defined(RAM_RELOCATION)) /* placed to RAM */
    return(GFLIB_Sin_F16_AsmRam((f16Angle +(frac16_t)16384)));
#else /* Placed to ROM */
    return(GFLIB_Sin_F16_Asm((f16Angle +(frac16_t)16384)));
#endif
}	
/*******************************************************************************
* Sine and cosine functions
*******************************************************************************/
RAM_FUNC_LIB 
RTCESL_INLINE static inline void GFLIB_SinCos_F16_FCi(register frac16_t f16Angle, register GMCLIB_2COOR_SINCOS_T_F16 *f16SinCos)
{   
#if (defined(RAM_RELOCATION)) /* placed to RAM */
    f16SinCos->f16Sin = GFLIB_Sin_F16_AsmRam(f16Angle);
    f16SinCos->f16Cos = GFLIB_Sin_F16_AsmRam((f16Angle +(frac16_t)16384)); 
#else /* Placed to ROM */
    f16SinCos->f16Sin = GFLIB_Sin_F16_Asm(f16Angle);
    f16SinCos->f16Cos = GFLIB_Sin_F16_Asm((f16Angle +(frac16_t)16384)); 
#endif
} 
 
#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_SINCOS_A32_H_ */

