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
* @brief  Clarke, Inverse Clarke Transformation
* 
*******************************************************************************/
#ifndef _GMCLIB_CLARK_FLT_H_
#define _GMCLIB_CLARK_FLT_H_

#if defined(__cplusplus) 
extern "C" {
#endif
  
/******************************************************************************
* Includes
******************************************************************************/
#include "mlib_FP.h"
#include "gmclib_types.h"      
  
/******************************************************************************
* Macros 
******************************************************************************/ 
#define GMCLIB_Clark_FLT_Ci(psIn, psOut)    GMCLIB_Clark_FLT_FCi(psIn, psOut)                                     
#define GMCLIB_ClarkInv_FLT_Ci(psIn, psOut) GMCLIB_ClarkInv_FLT_FCi(psIn, psOut)
  
/***************************************************************************//*!
*
* @brief   The function calculates Clarke Transformation which is used 
*          for transforming values (current, voltage, flux) from the 
*          three phase stationary coordination system to alpha-beta 
*          stationary orthogonal coordination system.
*
* @param   ptr - GMCLIB_3COOR_T_FLT *psIn - IN - pointer to structure containing  
*                data of three phase stationary system
*              - GMCLIB_2COOR_ALBE_T_FLT *psOut - OUT - pointer to structure  
*                containing data of two phase stationary orthogonal system
*
* @remarks Modifies the structure pointed by psOut pointer 
*          according to the following equations:
*
*          alpha = a
*          beta  = b * 1 / sqrt(3)  - c * 1 / sqrt(3) 
* 
****************************************************************************/  
static inline void GMCLIB_Clark_FLT_FCi(const GMCLIB_3COOR_T_FLT *psIn,
                                        GMCLIB_2COOR_ALBE_T_FLT *psOut)
{    
    /* Calculate Alpha orthogonal coordinate [alpha = a] */
    psOut->fltAlpha = psIn->fltA;
    
    /* Calculate Beta orthogonal coordinate [beta  = b * 1 / sqrt(3)  - c * 1 / sqrt(3] 
    and right shift to 16-bit range */ 
    psOut->fltBeta = MLIB_Mul_FLT(MLIB_Sub_FLT(psIn->fltB, psIn->fltC), GMCLIB_ONE_DIV_SQRT3_FLT); 
    
    return;
}
 
/****************************************************************************
*
* @brief        The function calculates Inverse Clarke Transformation which is used
*               for transforming values (current, voltage, flux) from alpha-beta 
*               stationary orthogonal coordination system to three phase 
*               stationary coordination system
*
* @param   ptr - GMCLIB_2COOR_ALBE_T_FLT *psOut - OUT - pointer to structure  
*                containing data of two phase stationary orthogonal system 
*              - GMCLIB_3COOR_T_FLT *psIn - IN - pointer to structure containing  
*                data of three phase stationary system                  
*
* @remarks      Modifies the structure pointed by p_abc pointer 
*               according to the following equations:
*
*               a = alpha
*               b = -0.5 * alpha + sgrt(3) / 2 * beta
*               c = -0.5 * alpha - sgrt(3) / 2 * beta
*
****************************************************************************/ 
static inline void GMCLIB_ClarkInv_FLT_FCi(const GMCLIB_2COOR_ALBE_T_FLT *psIn,
                                           GMCLIB_3COOR_T_FLT *psOut)
{    
    /* Store constant alpha * (-0.5) in flt range as fltM2 */
    register float_t fltM1 = MLIB_Mul_FLT(psIn->fltAlpha, -0.5F);
    /* Store constant beta * sqrt(3) / 2 in flt range as fltM2 */
    register float_t fltM2 = MLIB_Mul_FLT(psIn->fltBeta, GMCLIB_SQRT3_DIV_2_FLT); 
    
    /* Calculate a coordinate [a = alpha] */
    psOut->fltA = psIn->fltAlpha;   
    
    /* Calculate b coordinate [b = -alpha/2 + beta * sqrt(3) / 2  ] */              
    psOut->fltB = MLIB_Add_FLT(fltM1, fltM2);
    
    /* Calculate c coordinate [c = -alpha/2 - beta * sqrt(3) / 2  ] */  
    psOut->fltC = MLIB_Sub_FLT(fltM1, fltM2);  

    return;
}
 
#if defined(__cplusplus) 
}
#endif

#endif /* _GMCLIB_CLARK_FLT_H_ */
