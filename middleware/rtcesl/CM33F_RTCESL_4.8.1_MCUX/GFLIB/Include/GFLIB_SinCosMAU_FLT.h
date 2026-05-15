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
#ifndef _GFLIB_SINCOSMAU_FLT_H_
#define _GFLIB_SINCOSMAU_FLT_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_FP.h" 
/*******************************************************************************
* Macros 
*******************************************************************************/
#define GFLIB_SinMAU_FLT_Ci(fltAngle, u8ResReg)                   GFLIB_SinMAU_FLT_FCi(fltAngle, u8ResReg)
#define GFLIB_SinMAU_FLTa_Ci(fltAngle, u8ResReg)                  GFLIB_SinMAU_FLTa_FCi(fltAngle, u8ResReg)
#define GFLIB_CosMAU_FLT_Ci(fltAngle, u8ResReg)                   GFLIB_CosMAU_FLT_FCi(fltAngle, u8ResReg)
#define GFLIB_CosMAU_FLTa_Ci(fltAngle, u8ResReg)                  GFLIB_CosMAU_FLTa_FCi(fltAngle, u8ResReg)
#define GFLIB_SinCosMAU_FLTa_Ci(a32AngleExt, fltSinCos, u8ResRegSin, u8ResRegCos) GFLIB_SinCosMAU_FLTa_FCi(a32AngleExt, fltSinCos, u8ResRegSin, u8ResRegCos)

/****************************************************************************
* Inline functions 
****************************************************************************/

/***************************************************************************//*!
* @brief  Calculates the sine of the given argument using Math Accelerator Unit.
*
* @param  in   float_t fltAngle - Argument in float_t range
*              uint8_t u8ResReg - MAU result register. Valid parameter values are {1, 2, 3, 4}. 
*                                 Any value outside this range will trigger a CPU HardFault exception.
*
* @return This function returns - float_t value 
*       
* @remarks  
*   This function calculates sin(x) using MAU module:
*
*******************************************************************************/
static inline float_t GFLIB_SinMAU_FLT_FCi(register float_t fltAngle, register uint8_t u8ResReg)
{
    register uint32_t addr;

    #if defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wstrict-aliasing"
    #endif
	
    addr = RTCESL_MAU_IND_ADDR((uint32_t)RTCESL_MAU_BASE_PTR, RTCESL_MAU_DT_FLOAT, u8ResReg, RTCESL_MAU_MOPC_SIN);
    RTCESL_MAU_REG_FLOAT(addr) = fltAngle  / FLOAT_PI;
    return RTCESL_MAU_REG_FLOAT((uint32_t)(rtcesl_mau_res_table[u8ResReg].res_addr));
	
    #if defined(__GNUC__)
    #pragma GCC diagnostic pop
    #endif
}

/***************************************************************************//*!
* @brief  Calculates the cosine of the given argument using Math Accelerator Unit.
*
* @param  in   float_t fltAngle - Argument in float_t range
*              uint8_t u8ResReg - MAU result register. Valid parameter values are {1, 2, 3, 4}. 
*                                 Any value outside this range will trigger a CPU HardFault exception.
*
* @return This function returns - float_t value 
*       
* @remarks  
*   This function calculates cos(x) using MAU module:
*
**************************************************************/                                                         
static inline float_t GFLIB_CosMAU_FLT_FCi(register float_t fltAngle, register uint8_t u8ResReg)
{
    register uint32_t addr;
    
    #if defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wstrict-aliasing"
    #endif
	
    addr = RTCESL_MAU_IND_ADDR((uint32_t)RTCESL_MAU_BASE_PTR, RTCESL_MAU_DT_FLOAT, u8ResReg, RTCESL_MAU_MOPC_COS);
    RTCESL_MAU_REG_FLOAT(addr)  = fltAngle / FLOAT_PI;;
    return RTCESL_MAU_REG_FLOAT((uint32_t)(rtcesl_mau_res_table[u8ResReg].res_addr));
	
    #if defined(__GNUC__)
    #pragma GCC diagnostic pop
    #endif
} 

/***************************************************************************//*!
* @brief  Calculates the sine of the given argument using Math Accelerator Unit.
*
* @param  in   acc32 a32AngleExt - Argument in range in acc32_t, fractional part 
*                                  of which corresponds to [-pi; pi)
*              uint8_t u8ResReg - MAU result register. Valid parameter values are {1, 2, 3, 4}. 
*                                 Any value outside this range will trigger a CPU HardFault exception. 
*
* @return This function returns - float_t value 
*       
* @remarks  
*   This function calculates sin(x) using MAU module:
*   x = fractional part of a32AngleExt * pi / 2^15
*
*******************************************************************************/
static inline float_t GFLIB_SinMAU_FLTa_FCi(register acc32_t a32AngleExt, register uint8_t u8ResReg)
{
    register uint32_t addr;
    register float_t fltTemp;    
    fltTemp = (float_t)((float_t)(frac16_t)a32AngleExt / (float_t)32768.0F); /* Converts fractional to float */
    
    #if defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wstrict-aliasing"
    #endif
	
    addr = RTCESL_MAU_IND_ADDR((uint32_t)RTCESL_MAU_BASE_PTR, RTCESL_MAU_DT_FLOAT, u8ResReg, RTCESL_MAU_MOPC_SIN);
    RTCESL_MAU_REG_FLOAT(addr) = fltTemp;
    return RTCESL_MAU_REG_FLOAT((uint32_t)(rtcesl_mau_res_table[u8ResReg].res_addr));
	
    #if defined(__GNUC__)
    #pragma GCC diagnostic pop
    #endif	
}

/***************************************************************************//*!
* @brief  Calculates the cosine of the given argument using Math Accelerator Unit.
*
* @param  in   acc32 a32AngleExt - Argument in range in acc32_t, fractional part 
*                                  of which corresponds to [-pi; pi)
*               uint8_t u8ResReg - MAU result register. Valid parameter values are {1, 2, 3, 4}. 
*                                  Any value outside this range will trigger a CPU HardFault exception.
*
* @return This function returns - float_t value 
*       
* @remarks  
*   This function calculates cos(x) using MAU module:
*   x = fractional part of a32AngleExt * pi / 2^15
*
**************************************************************/                                                         
static inline float_t GFLIB_CosMAU_FLTa_FCi(register acc32_t a32AngleExt, register uint8_t u8ResReg)
{
    register uint32_t addr;
    register float_t fltTemp;    
    fltTemp = (float_t)((float_t)(frac16_t)a32AngleExt / (float_t)32768.0F); /* Converts fractional to float */
    
    #if defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wstrict-aliasing"
    #endif
	
    addr = RTCESL_MAU_IND_ADDR((uint32_t)RTCESL_MAU_BASE_PTR, RTCESL_MAU_DT_FLOAT, u8ResReg, RTCESL_MAU_MOPC_COS);
    RTCESL_MAU_REG_FLOAT(addr) = fltTemp;
    return RTCESL_MAU_REG_FLOAT((uint32_t)(rtcesl_mau_res_table[u8ResReg].res_addr));
	
    #if defined(__GNUC__)
    #pragma GCC diagnostic pop
    #endif	
} 

/***************************************************************************//*!
* @brief  Calculates the sine and cosine of the given argument using Math Accelerator Unit.
*
* @param  in   acc32 a32AngleExt - Argument in range in acc32_t, fractional part 
*                                  of which corresponds to [-pi; pi)
*               
*              GMCLIB_2COOR_SINCOS_T_FLT *fltSinCos - pointer to structure where the values of sine and cosine are stored
*
*              uint8_t u8ResRegSin - MAU result register. Valid parameter values are {1, 2, 3, 4}. 
*                                    Any value outside this range will trigger a CPU HardFault exception.
*                                    Shall be different from Cos result register for valid SinCos result.
*              uint8_t u8ResRegCos - MAU result register. Valid parameter values are {1, 2, 3, 4}. 
*                                    Any value outside this range will trigger a CPU HardFault exception.
*                                    Shall be different from Sin result register for valid SinCos result.
*
* @return This function returns - float_t value 
*       
* @remarks  
*   This function calculates sin(x) and cos(x) using MAU module.
*
*******************************************************************************/ 
static inline void GFLIB_SinCosMAU_FLTa_FCi(register acc32_t a32AngleExt, GMCLIB_2COOR_SINCOS_T_FLT *fltSinCos, register uint8_t u8ResRegSin, register uint8_t u8ResRegCos)
{   
    register uint32_t addr;
    register float_t fltTemp;    
    fltTemp = (float_t)((float_t)(frac16_t)a32AngleExt / (float_t)32768.0F); /* Converts fractional to float */
    
    #if defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wstrict-aliasing"
    #endif
	
    addr = RTCESL_MAU_IND_ADDR((uint32_t)RTCESL_MAU_BASE_PTR, RTCESL_MAU_DT_FLOAT, u8ResRegSin, RTCESL_MAU_MOPC_SIN);
    RTCESL_MAU_REG_FLOAT(addr) = fltTemp;
    addr = RTCESL_MAU_IND_ADDR((uint32_t)RTCESL_MAU_BASE_PTR, RTCESL_MAU_DT_FLOAT, u8ResRegCos, RTCESL_MAU_MOPC_COS);
    RTCESL_MAU_REG_FLOAT(addr) = fltTemp;
    
    fltSinCos->fltSin = RTCESL_MAU_REG_FLOAT((uint32_t)(rtcesl_mau_res_table[u8ResRegSin].res_addr));
    fltSinCos->fltCos = RTCESL_MAU_REG_FLOAT((uint32_t)(rtcesl_mau_res_table[u8ResRegCos].res_addr)); 
    
    #if defined(__GNUC__)
    #pragma GCC diagnostic pop
    #endif	   
}    


#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_SINCOSMAU_FLT_H_ */

