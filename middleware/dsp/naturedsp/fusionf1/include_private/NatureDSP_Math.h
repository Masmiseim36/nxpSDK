/* ------------------------------------------------------------------------ */
/* Copyright (c) 2016 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (“Cadence    */
/* Libraries”) are the copyrighted works of Cadence Design Systems Inc.	    */
/* Cadence IP is licensed for use with Cadence processor cores only and     */
/* must not be used for any other processors and platforms. Your use of the */
/* Cadence Libraries is subject to the terms of the license agreement you   */
/* have entered into with Cadence Design Systems, or a sublicense granted   */
/* to you by a direct Cadence licensee.                                     */
/* ------------------------------------------------------------------------ */
/*  IntegrIT, Ltd.   www.integrIT.com, info@integrIT.com                    */
/*                                                                          */
/* DSP Library                                                              */
/*                                                                          */
/* This library contains copyrighted materials, trade secrets and other     */
/* proprietary information of IntegrIT, Ltd. This software is licensed for  */
/* use with Cadence processor cores only and must not be used for any other */
/* processors and platforms. The license to use these sources was given to  */
/* Cadence, Inc. under Terms and Condition of a Software License Agreement  */
/* between Cadence, Inc. and IntegrIT, Ltd.                                 */
/* ------------------------------------------------------------------------ */
/*          Copyright (C) 2015-2016 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
#ifndef NATUREDSP_MATH_H 
#define NATUREDSP_MATH_H 
#include "NatureDSP_types.h"

#if defined(COMPILER_ANSI)
/*======================================
	 basic arithmetics
======================================*/
/* addition with saturation           */
int16_t	S_add_ss (int16_t x,int16_t y);	// Q15+Q15->Q15 
int32_t	L_add_ss (int16_t x,int16_t y);	// Q15+Q15->Q31
int32_t	L_add_ll (int32_t x,int32_t y); // Q31+Q31->Q31 

/* subtraction with saturation        */
int16_t	S_sub_ss (int16_t x,int16_t y);	// Q15-Q15->Q15 
int32_t	L_sub_ss (int16_t x,int16_t y);	// Q15-Q15->Q31
int32_t	L_sub_ll (int32_t x,int32_t y);	// Q31-Q31->Q31

/* negation                           */
int16_t	S_neg_s (int16_t x);		// -Q15->Q15
int32_t	L_neg_l (int32_t x);		// -Q31->Q31

/* absolute value with saturation     */
int16_t	S_abs_s (int16_t x);		// Q15->Q15
int32_t	L_abs_l (int32_t x);		// Q31->Q31

/* misc conversions                   */
int16_t S_sature_l (int32_t x);		// saturation: Q16.15->Q15
int16_t S_extract_l (int32_t x);	// Q31->Q15 (extract higher 16 bits)
int32_t L_deposit_s  (int16_t x);	// Q15->Q31 (shift left by 16)

/* rounding (adding a 0x8000) with 
saturation                            */
int32_t L_round_l (int32_t x);	// Q31->Q31 - add 0x8000 with saturation 
int16_t S_round_l (int32_t x);	// Q31->Q15 - add 0x8000 with saturation and shift left by 16 

/* getting exponent                   */
int16_t S_exp_l (int32_t x);	// exp(Q31), return  0 if x==0
int16_t S_exp_s (int16_t x);	// exp(Q15), return  0 if x==0
int16_t S_exp0_l (int32_t x);	// exp(Q31), return 31 if x==0
int16_t S_exp0_s (int16_t x);	// exp(Q15), return 15 if x==0

/* shift with saturation              */
int32_t L_shl_s (int16_t x,int16_t t);	// Qx->Q(x+t-16)
int32_t L_shl_l (int32_t x,int16_t t);	// Qx->Q(x+t)
int16_t S_shl_s (int16_t x,int16_t t);	// Qx->Q(x+t)
int32_t L_shr_s (int16_t x,int16_t t);	// Qx->Q(x-t-16)
int32_t L_shr_l (int32_t x,int16_t t);	// Qx->Q(x-t)
int16_t S_shr_s (int16_t x,int16_t t);	// Qx->Q(x-t)

/* fractional multiplication          */
int32_t	L_mpy_ss (int16_t x, int16_t y);	// Q15xQ15->Q31
int16_t	S_mpy_ss (int16_t x, int16_t y);	// Q15xQ15->Q15
int32_t	L_mpy_ls (int32_t x, int16_t y);	// Q31xQ15->Q31
int32_t	L_mpy_ll (int32_t x, int32_t y);	// Q31xQ31->Q31

/* multiplication : no saturation and 
 overflow control                     */
int32_t	L_mul_ss (int16_t x, int16_t y);	
int16_t	S_mul_ss (int16_t x, int16_t y);	
int32_t	L_mul_ls (int32_t x, int16_t y);	
int32_t	L_mul_ll (int32_t x, int32_t y);	

/* MAC/MAS operations                 */
int32_t	L_mac_ss (int32_t z, int16_t x, int16_t y);	// Q15xQ15->Q31
int32_t	L_mac_ls (int32_t z, int32_t x, int16_t y);	// Q31xQ15->Q31
int32_t	L_mac_ll (int32_t z, int32_t x, int32_t y);	// Q31xQ31->Q31
int32_t	L_mas_ss (int32_t z, int16_t x, int16_t y);	// Q15xQ15->Q31
int32_t	L_mas_ls (int32_t z, int32_t x, int16_t y);	// Q31xQ15->Q31
int32_t	L_mas_ll (int32_t z, int32_t x, int32_t y);	// Q31xQ31->Q31

/* fast fractional division with 
saturation                            */
int16_t S_div_ls (int32_t x,int16_t y);	// Q31/Q15 -> Q15
int16_t S_div_ll (int32_t x,int32_t y);	// Q31/Q31 -> Q15
int32_t L_div_ll (int32_t x,int32_t y);	// Q31/Q31 -> Q16.15
#elif defined(COMPILER_MSVC)
#include "NatureDSP_baseopMSVC.h"
#elif defined(COMPILER_C55)	
#include "NatureDSP_baseopC55.h"
#elif defined(COMPILER_C64)	
#include "NatureDSP_baseopC64.h"
#elif defined(COMPILER_GNU)	
#include "NatureDSP_baseopGCC.h"
#elif defined(COMPILER_GNUARM)	
#include "NatureDSP_baseopGCCARM.h"
#elif defined(COMPILER_ADSP_BLACKFIN)
#include "NatureDSP_baseopBlackfin.h"
#elif defined(COMPILER_CEARM9E)
#include "NatureDSP_baseopcearm9e.h"
#elif defined(COMPILER_XTENSA)
#include "NatureDSP_baseopXtensa.h"
#else
#error Code for another platforms available upon request: support@integrit.com
#endif //defined(COMPILER_MSVC)

#endif// NATUREDSP_MATH_H 

