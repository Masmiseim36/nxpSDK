/*******************************************************************************
*
* Copyright 2013-2016, Freescale Semiconductor, Inc.
* Copyright 2024 NXP
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
#ifndef _GFLIB_SINCOSMAU_A32_H_
#define _GFLIB_SINCOSMAU_A32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib.h" 
  
/*******************************************************************************
* Macros 
*******************************************************************************/
#define GFLIB_SinMAU_F16_Ci(f16Angle)               GFLIB_SinMAU_F16_FCi(f16Angle)
#define GFLIB_SinCosMAU_F16_Ci(f16Angle, f16SinCos) GFLIB_SinCosMAU_F16_FCi(f16Angle, f16SinCos)
#define GFLIB_CosMAU_F16_Ci(f16Angle)               GFLIB_CosMAU_F16_FCi(f16Angle)
/****************************************************************************
* Inline functions 
****************************************************************************/ 

/***************************************************************************//*!
* @brief  Calculates the sine of the given argument using Math Accelerator Unit.
*
* @param  in   frac16_t f16Angle - Argument in frac16_t range 
*
* @return This function returns - frac16_t value 
*       
* @remarks  
*   This function calculates sin(x) using MAU module:
*
*******************************************************************************/
static inline frac16_t GFLIB_SinMAU_F16_FCi(register frac16_t f16Angle)
{
    #if defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wstrict-aliasing"
    #endif
    
    register uint32_t addr = RTCESL_MAU_IND_ADDR((uint32_t)RTCESL_MAU_BASE_PTR, RTCESL_MAU_DT_Q1X, 0, RTCESL_MAU_MOPC_SIN);
    RTCESL_MAU_REG_Q15(addr) = f16Angle;
    return ((RTCESL_MAU_RES0))>>0;
    
    #if defined(__GNUC__)
    #pragma GCC diagnostic pop
    #endif
}

/***************************************************************************//*!
* @brief  Calculates the cosine of the given argument using Math Accelerator Unit.
*
* @param  in   float_t frac16_t - Argument in frac16_t range 
*
* @return This function returns - frac16_t value 
*       
* @remarks  
*   This function calculates cos(x) using MAU module:
*
*******************************************************************************/ 
static inline frac16_t GFLIB_CosMAU_F16_FCi(register frac16_t f16Angle)
{
    #if defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wstrict-aliasing"
    #endif

    register uint32_t addr = RTCESL_MAU_IND_ADDR((uint32_t)RTCESL_MAU_BASE_PTR, RTCESL_MAU_DT_Q1X, 1, RTCESL_MAU_MOPC_COS);
    RTCESL_MAU_REG_Q15(addr) = f16Angle;
    return (RTCESL_MAU_RES1);    
	
    #if defined(__GNUC__)
    #pragma GCC diagnostic pop
    #endif
}

/*******************************************************************************
* Sine and cosine functions
*******************************************************************************/
static inline void GFLIB_SinCosMAU_F16_FCi(register frac16_t f16Angle, GMCLIB_2COOR_SINCOS_T_F16 *f16SinCos)
{   
    register uint32_t addr;
    
    #if defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wstrict-aliasing"
    #endif
	
    addr = RTCESL_MAU_IND_ADDR((uint32_t)RTCESL_MAU_BASE_PTR, RTCESL_MAU_DT_Q1X, 0, RTCESL_MAU_MOPC_SIN);
    RTCESL_MAU_REG_Q15(addr) = f16Angle;
    addr = RTCESL_MAU_IND_ADDR((uint32_t)RTCESL_MAU_BASE_PTR, RTCESL_MAU_DT_Q1X, 1, RTCESL_MAU_MOPC_COS);
    RTCESL_MAU_REG_Q15(addr) = f16Angle;
    
    f16SinCos->f16Sin = RTCESL_MAU_RES0;
    f16SinCos->f16Cos = RTCESL_MAU_RES1; 
    
    #if defined(__GNUC__)
    #pragma GCC diagnostic pop
    #endif	   
}    


#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_SINCOSMAU_A32_H_ */

