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

#include "NatureDSP_types.h"
#include "NatureDSP_Math.h"
#include <stdlib.h>

#if !defined(COMPILER_XTENSA)
#error This code is for Tensilica compiler
#endif

int16_t S_div_ll (int32_t x,int32_t y);
int32_t L_div_ll (int32_t x,int32_t y);
int32_t	L_mpy_ll (int32_t x, int32_t y);

/* shift left with saturation: Qx->Q(x+t)         */
inline_ int32_t L_shl_l (int32_t x,int16_t t)
{
  int32_t a;
  if (t > 0)
  {
    a = (int32_t)(x >> (31 - t));
    if ((a == 0) || (a == -1))
      x <<= t;
    else
      x = (a > 0) ? MAX_INT32 : MIN_INT32;
  }
  else    
  {
	  if (t < -31)	t=-31;
	  x >>= (-t);
  }
  return x;
}   

/* misc conversions  saturation: Q16.15->Q15        */
inline_ int16_t S_sature_l (int32_t x)
{
	return (int16_t)(L_shl_l(x,16)>>16);
}

/* misc conversions  saturation: Q31->Q15 (extract higher 16 bits) */
inline_ int16_t S_extract_l (int32_t x)
{
	return (int16_t)(x>>16);
}

/* misc conversions  saturation: Q15->Q31 (shift left by 16) */
inline_ int32_t L_deposit_s  (int16_t x)
{
	return ((int32_t)x)<<16;
}

/* addition with saturation: Q31+Q31->Q31        */
inline_ int32_t	L_add_ll (int32_t x,int32_t y)
{
  int32_t z;

  z = x + y;

  if ((((x ^ y) & MIN_INT32) == 0L) 
    && ((z ^ x) & MIN_INT32))
      z = (x < 0) ? MIN_INT32 : MAX_INT32;
  return z;
}

/* addition with saturation: Q15+Q15->Q31        */
inline_ int32_t	L_add_ss (int16_t x,int16_t y)
{
	return L_add_ll(L_deposit_s(x),L_deposit_s(y));
}

/* addition with saturation: Q15+Q15->Q15        */
inline_ int16_t	S_add_ss (int16_t x,int16_t y)
{
	return S_sature_l(((int32_t)x) + y);
}

/* subtraction with saturation  Q31-Q31->Q31      */
inline_ int32_t	L_sub_ll (int32_t x,int32_t y)
{
  int32_t z;

  z = x - y;
  if ( ((x ^ y) & MIN_INT32) && 
       ((z ^ x) & MIN_INT32))
  {
      z = (x < 0) ? MIN_INT32 : MAX_INT32;
  }
  return z;
}

/* subtraction with saturation  Q15-Q15->Q31      */
inline_ int32_t	L_sub_ss (int16_t x,int16_t y)
{
	return L_sub_ll(L_deposit_s(x),L_deposit_s(y));
}

/* subtraction with saturation  Q15-Q15->Q15      */
inline_ int16_t	S_sub_ss (int16_t x,int16_t y)
{
	return S_sature_l(((int32_t)x) - y);
}

/* negation -Q15->Q15                            */
inline_ int32_t	L_neg_l (int32_t x)
{
	return (x==MIN_INT32) ? MAX_INT32: -x;
}

/* negation -Q15->Q15                            */
inline_ int16_t	S_neg_s (int16_t x)
{
	return (x==MIN_INT16) ? MAX_INT16: -x;
}

/* absolute value with saturation  Q15->Q15   */
inline_ int16_t	S_abs_s (int16_t x)
{
  if (x == MIN_INT16) return MAX_INT16;
  return (x < 0) ? -x: x;
}

/* absolute value with saturation  Q31->Q31  */
inline_ int32_t	L_abs_l (int32_t x)	
{
  if (x == MIN_INT32) return MAX_INT32;
  return (x < 0) ? -x: x;
}



/* rounding (adding a 0x8000) with saturation: 
Q31->Q31 - add 0x8000 with saturation                             */
inline_ int32_t L_round_l (int32_t x)	// 
{
	return (L_add_ll(x,(int32_t)0x8000U)) & 0xFFFF0000;
}

/* rounding (adding a 0x8000) with saturation: 
Q31->Q15 - add 0x8000 with saturation and shift left by 16       */
inline_ int16_t S_round_l (int32_t x)	
{
	return (int16_t)(L_add_ll(x,(int32_t)0x8000U)>>16);
}

/* getting exponent:exp(Q31), return  0 if x==0                  */
inline_ int16_t S_exp_l (int32_t x)
{
  int16_t z=0;
  if ( x==0 )    return 0;
  while ( (int32_t)(x^(x<<1))>0 ) //MSB != MSB-1
  {
    x<<=1;
    z++;
  }
  return z;
}

/* getting exponent:exp(Q31), return 31 if x==0                  */
inline_ int16_t S_exp0_l (int32_t x)
{
  int16_t z=0;
  if ( x==0 )  return 0x1F;
  while ( (int32_t)(x^(x<<1))>0 ) //MSB != MSB-1
  {
    x<<=1;
    z++;
  }
  return z;
}

/* getting exponent: exp(Q15), return  0 if x==0                 */
inline_ int16_t S_exp_s (int16_t x)
{
	return S_exp_l(L_deposit_s(x));
}

/* getting exponent: exp(Q15), return 15 if x==0                 */
inline_ int16_t S_exp0_s (int16_t x)
{
	return S_exp0_l((int32_t)x)-16;
}


/* shift left with saturation: Qx->Q(x+t-16)      */
inline_ int32_t L_shl_s (int16_t x,int16_t t)
{
	return L_shl_l(L_deposit_s(x),t);
}

/* shift left with saturation: Qx->Q(x+t)         */
inline_ int16_t S_shl_s (int16_t x,int16_t t)
{
	return (int16_t)(L_shl_l(L_deposit_s(x),t)>>16);
}

/* shift right with saturation: Qx->Q(x-t)        */
inline_ int32_t L_shr_l (int32_t x,int16_t t)
{
	return L_shl_l(x,(int16_t)-t);
}

/* shift right with saturation: Qx->Q(x-t-16)     */
inline_ int32_t L_shr_s (int16_t x,int16_t t)
{
	return L_shl_s(x,(int16_t)-t);
}

/* shift right with saturation: Qx->Q(x-t)        */
inline_ int16_t S_shr_s (int16_t x,int16_t t)
{
	return S_shl_s(x,(int16_t)-t);
}

/* fractional multiplication: Q15xQ15->Q31         */
inline_ int32_t	L_mpy_ss (int16_t x, int16_t y)
{
  int32_t z;
  z = (int32_t)x * (int32_t)y;
  if ((x != MIN_INT16) || (y != MIN_INT16))
    return (z << 1);
  return MAX_INT32;
}

/* fractional multiplication: Q15xQ15->Q15         */
inline_ int16_t	S_mpy_ss (int16_t x, int16_t y)	
{
	return (int16_t)(L_mpy_ss(x,y)>>16);
}

/* fractional multiplication: Q31xQ15->Q15         */
inline_ int32_t	L_mpy_ls (int32_t x, int16_t y)
{
  int32_t z;
  int16_t t;

  t  = ((int16_t)(x>>1)) & 0x7FFF;
  z  = L_mpy_ss(y, t) >> 15;
  z  = L_add_ll(z,L_mpy_ss(y, S_extract_l(x)));
  return z;
}


/* multiplication : no saturation and 
 overflow control                     */
inline_ int32_t	L_mul_ss (int16_t x, int16_t y) { 	return ((int32_t)x)*y; }
inline_ int16_t	S_mul_ss (int16_t x, int16_t y)	{ 	return x*y; }
inline_ int32_t	L_mul_ls (int32_t x, int16_t y) { 	return x*y; }
inline_ int32_t	L_mul_ll (int32_t x, int32_t y) { 	return x*y; }

/* MAC operation z=z+x*y: Q31+Q15xQ15->Q31            */
inline_ int32_t	L_mac_ss (int32_t z, int16_t x, int16_t y)
{
	return L_add_ll(z,L_mpy_ss(x,y));
}
/* MAC operation z=z+x*y: Q31+Q15xQ15->Q31            */
inline_ int32_t	L_mac_ls (int32_t z, int32_t x, int16_t y)
{
	return L_add_ll(z,L_mpy_ls(x,y));
}

/* MAC operation z=z+x*y: Q31+Q31xQ31->Q31            */
inline_ int32_t	L_mac_ll (int32_t z, int32_t x, int32_t y)
{
	return L_add_ll(z,L_mpy_ll(x,y));
}

/* MAS operation z=z-x*y: Q31-Q15xQ15->Q31            */
inline_ int32_t	L_mas_ss (int32_t z, int16_t x, int16_t y)
{
	return L_sub_ll(z,L_mpy_ss(x,y));
}
/* MAS operation z=z-x*y: Q31-Q15xQ15->Q31            */
inline_ int32_t	L_mas_ls (int32_t z, int32_t x, int16_t y)
{
	return L_sub_ll(z,L_mpy_ls(x,y));
}

/* MAS operation z=z-x*y: Q31-Q31xQ31->Q31            */
inline_ int32_t	L_mas_ll (int32_t z, int32_t x, int32_t y)
{
	return L_sub_ll(z,L_mpy_ll(x,y));
}

/* fast fractional division with saturation  Q31/Q15 -> Q15 */
inline_ int16_t S_div_ls (int32_t x,int16_t y)
{
	return S_div_ll(x,L_deposit_s(y));
}

