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
#define GFLIB_SinMAU_F16_Ci(f16Angle, u8ResReg)               GFLIB_SinMAU_F16_FCi(f16Angle, u8ResReg)
#define GFLIB_CosMAU_F16_Ci(f16Angle, u8ResReg)               GFLIB_CosMAU_F16_FCi(f16Angle, u8ResReg)
#define GFLIB_SinCosMAU_F16_Ci(f16Angle, f16SinCos, u8ResRegSin, u8ResRegCos) GFLIB_SinCosMAU_F16_FCi(f16Angle, f16SinCos, u8ResRegSin, u8ResRegCos)
/****************************************************************************
* Inline functions 
****************************************************************************/ 

/***************************************************************************//*!
* @brief  Calculates the sine of the given argument using Math Accelerator Unit.
*
* @param  in   frac16_t f16Angle - Argument in frac16_t range
*               uint8_t u8ResReg - MAU result register. Valid parameter values are {1, 2, 3, 4}. 
*                                  Any value outside this range will trigger a CPU HardFault exception.
*
* @return This function returns - frac16_t value 
*       
* @remarks  
*   This function calculates sin(x) using MAU module:
*
*******************************************************************************/
static inline frac16_t GFLIB_SinMAU_F16_FCi(register frac16_t f16Angle, register uint8_t u8ResReg)
{
    #if defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wstrict-aliasing"
    #endif
    
    register uint32_t addr = RTCESL_MAU_IND_ADDR((uint32_t)RTCESL_MAU_BASE_PTR, RTCESL_MAU_DT_Q1X, u8ResReg, RTCESL_MAU_MOPC_SIN);
    RTCESL_MAU_REG_Q15(addr) = f16Angle;
    return ((*(volatile uint32_t*)(rtcesl_mau_res_table[u8ResReg].res_addr)))>>0;
    
    #if defined(__GNUC__)
    #pragma GCC diagnostic pop
    #endif
}

/***************************************************************************//*!
* @brief  Calculates the cosine of the given argument using Math Accelerator Unit.
*
* @param  in   frac16_t f16Angle - Argument in frac16_t range
*              uint8_t u8ResReg  - MAU result register. Valid parameter values are {1, 2, 3, 4}. 
*                                  Any value outside this range will trigger a CPU HardFault exception.
*
* @return This function returns - frac16_t value 
*       
* @remarks  
*   This function calculates cos(x) using MAU module:
*
*******************************************************************************/ 
static inline frac16_t GFLIB_CosMAU_F16_FCi(register frac16_t f16Angle, register uint8_t u8ResReg)
{
    #if defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wstrict-aliasing"
    #endif

    register uint32_t addr = RTCESL_MAU_IND_ADDR((uint32_t)RTCESL_MAU_BASE_PTR, RTCESL_MAU_DT_Q1X, u8ResReg, RTCESL_MAU_MOPC_COS);
    RTCESL_MAU_REG_Q15(addr) = f16Angle;
    return (*(volatile uint32_t*)(rtcesl_mau_res_table[u8ResReg].res_addr));    
	
    #if defined(__GNUC__)
    #pragma GCC diagnostic pop
    #endif
}

/***************************************************************************//*!
* @brief  Calculates the sine and cosine of the given argument using Math Accelerator Unit.
*
* @param  in   frac16_t f16Angle   - Argument in frac16_t range
*               
*              GMCLIB_2COOR_SINCOS_T_F16 *f16SinCos - pointer to structure where the values of sine and cosine are stored
*
*              uint8_t u8ResRegSin - MAU result register. Valid parameter values are {1, 2, 3, 4}. 
*                                    Any value outside this range will trigger a CPU HardFault exception.
*                                    Shall be different from Cos result register for valid SinCos result.
*              uint8_t u8ResRegCos - MAU result register. Valid parameter values are {1, 2, 3, 4}. 
*                                    Any value outside this range will trigger a CPU HardFault exception.
*                                    Shall be different from Sin result register for valid SinCos result.
*
* @return This function returns - frac16_t value 
*       
* @remarks  
*   This function calculates cos(x) using MAU module:
*
*******************************************************************************/ 
static inline void GFLIB_SinCosMAU_F16_FCi(register frac16_t f16Angle, GMCLIB_2COOR_SINCOS_T_F16 *f16SinCos, register uint8_t u8ResRegSin, register uint8_t u8ResRegCos)
{   
    register uint32_t addr;
    
    #if defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wstrict-aliasing"
    #endif
	
    addr = RTCESL_MAU_IND_ADDR((uint32_t)RTCESL_MAU_BASE_PTR, RTCESL_MAU_DT_Q1X, u8ResRegSin, RTCESL_MAU_MOPC_SIN);
    RTCESL_MAU_REG_Q15(addr) = f16Angle;
    addr = RTCESL_MAU_IND_ADDR((uint32_t)RTCESL_MAU_BASE_PTR, RTCESL_MAU_DT_Q1X, u8ResRegCos, RTCESL_MAU_MOPC_COS);
    RTCESL_MAU_REG_Q15(addr) = f16Angle;
    
    f16SinCos->f16Sin = (*(volatile uint32_t*)(rtcesl_mau_res_table[u8ResRegSin].res_addr));
    f16SinCos->f16Cos = (*(volatile uint32_t*)(rtcesl_mau_res_table[u8ResRegCos].res_addr)); 
    
    #if defined(__GNUC__)
    #pragma GCC diagnostic pop
    #endif	   
}    


#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_SINCOSMAU_A32_H_ */

