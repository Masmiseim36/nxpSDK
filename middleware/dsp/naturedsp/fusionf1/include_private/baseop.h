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
#ifndef __BASEOP_H__
#define __BASEOP_H__

/* ------------------------------------------------------------------------ */
/*          Basic operations -- needed for reference code only              */
/* ------------------------------------------------------------------------ */ 

#include "NatureDSP_types.h"

typedef struct tagInt128_s
{
  uint64_t lo;
  int64_t hi;
} int128_t ;

// HiFi3 primitives
/* truncate Q47 to f24 */
inline_ f24 truncQ47_f24(int64_t x)   {    int32_t y=(int32_t)(x>>24); return (y<<8); }
inline_ f24 roundQ47_f24(int64_t x)   {    int32_t y=(int32_t)((x+(1<<23))>>24); return (y<<8); }
inline_ f24 satQ47_f24  (int64_t x)   {    int32_t y=(int32_t)(x>>24); 
                                           if (y>(int32_t)0x007FFFFF) y=(int32_t)0x007FFFFF;
                                           if (y<(int32_t)0xFF800000) y=(int32_t)0xFF800000;
                                           return (y<<8); 
                                      }


// saturate Q31 to 32 bit
inline_ int32_t satQ31  (int64_t x)   {    
                                           if (x>(int64_t)0x000000007FFFFFFFLL) x=0x7FFFFFFF;
                                           if (x<(int64_t)0xFFFFFFFF80000000LL) x=0x80000000;
                                           return (int32_t)x; 
                                      }
// saturate double to 64 bit
inline_ int64_t satQ63  (double x)   {  
                                           int64_t z = (int64_t)x;
                                           if (x>=((double)0x7FFFFFFFFFFFFFFFLL)) z=(int64_t)0x7FFFFFFFFFFFFFFFLL;
                                           if (x<(-(double)0x7FFFFFFFFFFFFFFFLL)) z=(int64_t)0x8000000000000000ULL;
                                           return z; 
                                      }
/* store f24 number */
inline_ int32_t store_f24(f24 x)      {    return (x & 0xFFFFFF00); }
/* load f24 number */
inline_ f24     load_f24(int32_t x)   {    return (x & 0xFFFFFF00); }
/* saturate on 24 bits */
inline_ int32_t sat24s(int32_t x)
{
    if(x>(int32_t)0x007fffffL) x=(int32_t)0x007fffffL;
    if(x<(int32_t)0xff800000L) x=(int32_t)0xff800000L;
    return x;
}

/* fractional non-saturating 24x24 multiply with asymmetric rounding */
inline_ int32_t mulf24ra(int32_t x,int32_t y)
{
    x=(x<<8)>>8;    /* sign extension */
    y=(y<<8)>>8;
    return (int32_t)(((((int64_t)x)*y)+(1<<22))>>23);
}


/* fractional multiple f24xf24 forming Q47 result */
inline_ int64_t mpy_f24f24 (f24 x,f24 y)
{
    int64_t z=(int64_t)x*y;
    if (((uint32_t ) x)==0x80000000 && ((uint32_t) y)==0x80000000) return (int64_t)0x7fffffffffffffffLL>>16;
    return (z>>16)<<1;
}
/* fractional multiple Q31xQ15 forming Q16.47 result */
inline_ int64_t mpy_Q31Q15( int32_t x, int16_t y )
{
  return ( ((int64_t)x*y) << 1 );
}
inline_ int64_t mul_Q31Q15( int32_t x, int16_t y )
{
  return ( ((int64_t)x*y) );
}
/* fractional multiple Q31xf24 forming Q55 result */
inline_ int64_t mpy_Q31f24 (int32_t x,f24 y)
{
    int64_t z=(int64_t)x*y;
    if (((uint32_t) x)==0x80000000 && ((uint32_t) y)==0x80000000) return (int64_t)0x7fffffffffffffffLL;
    return (z>>8)<<1;
}

/* Fractional mupltiplication Q31xQ31 forming Q63 result */
inline_ int64_t mpy_Q31Q31( int32_t x, int32_t y )
{
    int64_t z = (int64_t)x*y;

    if ( (uint32_t)x == 0x80000000 && (uint32_t)y == 0x80000000 )
    {
      return (0x7fffffffffffffffLL);
    }

    return (z<<1);
}

/* addition with saturation: Q63+Q63->Q63        */
inline_ int64_t add_Q63Q63( int64_t x, int64_t y )
{
  int64_t z;

  z = x + y;

  if ( ( ( (x^y) & MIN_INT64 ) == 0LL ) 
       &&
       ( ( (z^x) & MIN_INT64 ) != 0LL ) )
  {
      z = ( x < 0 ? MIN_INT64 : MAX_INT64 );
  }

  return z;
}
/* truncate Q63 to Q31 */
inline_ int32_t roundQ63_Q31(int64_t x)   {    return (int32_t)(add_Q63Q63(x,((int64_t)1)<<31)>>32); }

// extract from "NatureDSP_Math.h"
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
	return (x==(int16_t)MIN_INT16) ? MAX_INT16: -x;
}

/* absolute value with saturation  Q15->Q15   */
inline_ int16_t	S_abs_s (int16_t x)
{
  if (x == (int16_t)MIN_INT16) return MAX_INT16;
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
  if ((x != (int16_t)MIN_INT16) || (y != (int16_t)MIN_INT16))
    return (z << 1);
  return MAX_INT32;
}

/* fractional multiplication: Q15xQ15->Q15         */
inline_ int16_t	S_mpy_ss (int16_t x, int16_t y)	
{
	return (int16_t)(L_mpy_ss(x,y)>>16);
}

/* fractional multiplication: Q31xQ15->Q31         */
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

/* fractional multiplication: Q31xQ31->Q31         */
inline_ int32_t	L_mpy_ll (int32_t x, int32_t y)
{
  int16_t xl,yl;
  int16_t xh,yh;
  int32_t  Acc;

  xl = (((int16_t)(x))>>1)&0x7FFF;
  yl = (((int16_t)(y))>>1)&0x7FFF;
  xh = S_extract_l(x);
  yh = S_extract_l(y);

  Acc = (L_mpy_ss(xh, yl)>>1) +
        (L_mpy_ss(yh, xl)>>1) +
        (L_mpy_ss(xl,  yl)>>16);
  Acc = Acc >> 14;
  Acc += L_mpy_ss(xh, yh);
  return Acc;
}
// addition without saturation
inline_ int128_t int128_add( int128_t a, int128_t b)
{
    int128_t r = { a.lo + b.lo, a.hi + b.hi };
    r.hi += (r.lo < a.lo) || (r.lo < b.lo);
    return r;
}

//  shift right 
inline_ int128_t int128_rsh(int128_t x, int a)   { 
  int128_t y;
  int64_t tmp;
  y.hi=0;y.lo=(int64_t)1<<(a-1);
  x = int128_add(x,y);
  tmp = x.hi&&((1<<a) - 1);
  y.hi = x.hi>>a;
  x.lo = x.lo>>a;
  y.lo = x.lo|(tmp<<(64-a));
  return y;
}

typedef struct 
{
  uint64_t lo;
    int8_t hi;
} 
int72_t ;

// add 64 or 72-bit number to 72-bit number
inline_ int72_t add72(int72_t a,int72_t b)
{
    int72_t c;
    c.lo=a.lo+b.lo;
    c.hi=a.hi+b.hi;
    if (c.lo<a.lo) c.hi++;
    return c;
}

inline_ int72_t add72x64(int72_t a,int64_t b)
{
    int72_t c;
    c.lo=(uint64_t)b;
    c.hi=(  int8_t)(b>>63);
    c=add72(a,c);
    return c;
}

inline_ int64_t sra72(int72_t a,int shift)
{
    if(shift)
    {
        a.lo>>=shift;
        a.lo|=((uint64_t)a.hi)<<(64-shift);
    }
    return (int64_t)a.lo;
}

inline_ int72_t sla72(int64_t a,int shift)
{
    int72_t b;
    if(shift==0)
    {
        b.lo=(uint64_t)(a);
        b.hi=(int8_t)(a>>63);
    }
    else
    {
        b.lo=(uint64_t)(a<<shift);
        b.hi=(int8_t)(a>>(64-shift));
    }
    return b;
}

inline_ int64_t sat72(int72_t a)
{
    int sgn=(int)sra72(a,63);   // if no overflow, this should be 0 or -1
    if (sgn>0 || sgn<-1) return sgn>0 ? (int64_t)0x7fffffffffffffffLL:(int64_t)0x8000000000000000LL;
    return (int64_t)a.lo;
}

/* take the sign of floating-point number */
inline_ int takesignf(float32_t x)
{
    union 
    {
        float32_t f;
        uint32_t  u;
    }y;
    y.f=x;
    return (int)(y.u>>31);
}
/* set the sign of floating-point number */
inline_ float32_t setsignf(float32_t x, int sign)
{
    union 
    {
        float32_t f;
        uint32_t  u;
    }y;
    y.f=x;
    y.u&=0x7fffffff;
    sign=sign?1:0;
    y.u|=((uint32_t)sign)<<31;
    return y.f;
}
/* change the sign of floating-point number */
inline_ float32_t changesignf(float32_t x, int sign)
{
    union 
    {
        float32_t f;
        uint32_t  u;
    }y;
    y.f=x;
    sign=sign?1:0;
    y.u^=((uint32_t)sign)<<31;
    return y.f;
}

/* Reverse the bit order for bit positions 0..(nbits-1) */
inline_ int bitrev( int x, int nbits )
{
  int y, n;

  for ( y=0, n=1; n<=nbits; n++, x>>=1 )
  {
    y |= ( (x&1) << (nbits-n) );
  }

  return (y);

} /* bitrev() */

/* Complex floating-point multiplication, single precision. */
inline_ complex_float mul_fl32c( complex_float x, complex_float y )
{
  union
  {
        complex_float z;
        struct {float32_t re,im;}s;
  }z;
  z.s.re= crealf(x)*crealf(y) - cimagf(x)*cimagf(y);
  z.s.im= cimagf(x)*crealf(y) + crealf(x)*cimagf(y);
  return (z.z);
}

/* Complex floating-point conjugation, single precision. */
inline_ complex_float conj_fl32c( complex_float x )
{
    union
    {
        complex_float z;
        struct { float32_t re,im; }s;
    }z;
  z.s.re= crealf(x);
  z.s.im=-cimagf(x);
  return (z.z);
}

/* return non-zero if argument is a signalling NaN */
inline_ int is_snanf( float32_t x )
{
    union 
    {
        float32_t f;
        uint32_t  u;
    }y;
    y.f = x;
    return ( ( y.u & 0x7f800000 ) == 0x7f800000 &&
             ( y.u & 0x00400000 ) == 0          &&
             ( y.u & 0x003fffff ) != 0 );
}

#endif
