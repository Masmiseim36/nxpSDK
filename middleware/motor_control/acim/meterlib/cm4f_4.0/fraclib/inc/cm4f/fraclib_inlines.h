/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ****************************************************************************//*!
 * @file      fraclib_inlines.h
 * @author    R55013
 * @version   1.3.0.0
 * @date      Feb-08-2015
 * @brief     Source file containing routines for calculation 32-bit fractional
 *            arithmetic.
 *******************************************************************************/
#ifndef __FRACLIB_INLINES_H
#define __FRACLIB_INLINES_H

/*! USE_MMAU source code type selector definition. Comment it out if you want  */
/*! to built fracional library without MMAU support. Note that fraclib.a must  */
/*! be rebuilt after this constant changes.                                    */
//#define USE_MMAU

/*! USE_FRACLIB_MUL32 source code type selector definition. Comment it out     */
/*! if C-version of the 32x32=64 bit multiplications shall be used instead     */
/*! of assembler functions. Note that fraclib.a must be rebuilt after this     */
/*! constant changes.                                                          */
//#define USE_FRACLIB_MUL32

#include "types.h"
#if defined(USE_FRACLIB_MUL32)
  #include "fraclib_mul32.h"
#endif
#if defined(USE_MMAU)
  #include "mmau.h"
#endif


/****************************************************************************//*!
 * @brief   Extracts the 16 MSBs of a 32-bit fractional value.
 *          Corresponds to truncation when applied to fractional values.
 * @param   lsrc    - Input 32-bit fractional value.
 * @return  Function returns 16-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
static inline frac16 S_extract_hi (register frac32 lsrc)
{
  return (frac16)((frac32)lsrc>>16);
}

/****************************************************************************//*!
 * @brief   Extracts the 32 MSBs of a 64-bit fractional value.
 *          Corresponds to truncation when applied to fractional values.
 * @param   llsrc    - Input 64-bit fractional value.
 * @return  Function returns 32-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
static inline frac32 L_extract_hi (register frac64 llsrc)
{
  return (frac32)((frac64)llsrc>>32);
}

/****************************************************************************//*!
 * @brief   Absolute value of a 32-bit fractional value, returning a 32-bit result.
 * @param   lsrc    - Input 32-bit fractional value.
 * @return  Function returns 32-bit value in fractional format.
 * @remarks Implementation in C-language. Where the input is 0x80000000, saturation
 *          occurs and 0x7fffffff is returned.
 *******************************************************************************/
static inline frac32 L_abs (register frac32 lsrc)
{
  register frac32 mask = lsrc>>31;
  if (lsrc == 0x80000000) { return       0x7fffffff;  }
  else                    { return (lsrc^mask)-mask;  }
}

/****************************************************************************//*!
 * @brief   Absolute value of a 64-bit fractional value, returning a 64-bit result.
 * @param   llsrc    - Input 64-bit fractional value.
 * @return  Function returns 64-bit value in fractional format.
 * @remarks Implementation in C-language. Where the input is 0x8000000000000000,
 *          saturation occurs and 0x7fffffffffffffff is returned.
 *******************************************************************************/
static inline frac64 LL_abs (register frac64 llsrc)
{
  register frac64 mask = llsrc>>63;
  if (llsrc == 0x8000000000000000) { return 0x7fffffffffffffff; }
  else                             { return  (llsrc^mask)-mask; }
}

/****************************************************************************//*!
 * @brief   Addition of two 32-bit fractional values, returning a 32-bit result.
 *          Doesn't perform saturation.
 * @param   lsrc1   - Input 32-bit fractional value.
 * @param   lsrc2   - Input 32-bit fractional value.
 * @return  Function returns 32-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
static inline frac32 L_add (register frac32 lsrc1, register frac32 lsrc2)
{
  return lsrc1+lsrc2;
}

/****************************************************************************//*!
 * @brief   Addition of two 64-bit fractional values, returning a 64-bit result.
 *          Doesn't perform saturation.
 * @param   llsrc1  - Input 64-bit fractional value.
 * @param   llsrc2  - Input 64-bit fractional value.
 * @return  Function returns 64-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
static inline frac64 LL_add (register frac64 llsrc1, register frac64 llsrc2)
{
  return llsrc1+llsrc2;
}

/****************************************************************************//*!
 * @brief   Subtraction of two 32-bit fractional values, returning a 32-bit
 *          result. Doesn't perform saturation.
 * @param   lsrc1   - Input 32-bit fractional value.
 * @param   lsrc2   - Input 32-bit fractional value.
 * @return  Function returns 32-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
static inline frac32 L_sub (register frac32 lsrc1, register frac32 lsrc2)
{
  return lsrc1-lsrc2;
}

/****************************************************************************//*!
 * @brief   Subtraction of two 64-bit fractional values, returning a 64-bit
 *          result. Doesn't perform saturation.
 * @param   llsrc1   - Input 64-bit fractional value.
 * @param   llsrc2   - Input 64-bit fractional value.
 * @return  Function returns 64-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
static inline frac64 LL_sub (register frac64 llsrc1, register frac64 llsrc2)
{
  return llsrc1-llsrc2;
}

/****************************************************************************//*!
 * @brief   Multiply two 16-bit fractional values generating 32-bit fractional
 *          result.
 * @param   ssrc1   - Input 16-bit fractional value.
 * @param   ssrc2   - Input 16-bit fractional value.
 * @return  Function returns 32-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
static inline frac32 L_mul_ss (register frac16 ssrc1, register frac16 ssrc2)
{
  return ((frac32)ssrc1*(frac32)ssrc2)<<1;
}

/****************************************************************************//*!
 * @brief   Multiply 16-bit fractional values with 32-bit fractional value
 *          and return 32-bit fractional result.
 * @param   lsrc1   - Input 32-bit fractional value.
 * @param   ssrc1   - Input 16-bit fractional value.
 * @return  Function returns 32-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
#if defined(USE_MMAU)
  #define  L_mul_ls(lsrc1,ssrc1) (frac32)l_muls_ll(lsrc1,((frac32)ssrc1<<16))
#else
  static inline frac32 L_mul_ls (register frac32 lsrc1, register frac16 ssrc1)
  {
    return (((frac32)(lsrc1&0xffff)*(frac32)ssrc1)>>15) +
           (((frac32)(lsrc1>>16)*(frac32)ssrc1)<<1);
  }
#endif

/****************************************************************************//*!
 * @brief   Multiply two 32-bit fractional values generating 32-bit fractional
 *          result.
 * @param   lsrc1   - Input 32-bit fractional value.
 * @param   lsrc2   - Input 32-bit fractional value.
 * @return  Function returns 32-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
#if defined(USE_MMAU)
  #define  L_mul(lsrc1,lsrc2) (frac32)l_muls_ll(lsrc1,lsrc2)
#else
  #if defined(USE_FRACLIB_MUL32)
    #define L_mul(lsrc1,lsrc2) (frac32)(FRACLIB_FFMUL32(lsrc1,lsrc2)>>32)
  #else
    static inline frac32 L_mul (register frac32 lsrc1, register frac32 lsrc2)
    {
      register frac64 tmp = ((frac64)lsrc1*(frac64)lsrc2);
      return (tmp+tmp)>>32;
    }
  #endif
#endif

/****************************************************************************//*!
 * @brief   Multiply two 32-bit fractional values generating 64-bit fractional
 *          result.
 * @param   lsrc1   - Input 32-bit fractional value.
 * @param   lsrc2   - Input 32-bit fractional value.
 * @return  Function returns 64-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
#if defined(USE_MMAU)
  #define LL_mul_ll(lsrc1,lsrc2) (frac64)d_muls_ll(lsrc1,lsrc2)
#else
  #if defined(USE_FRACLIB_MUL32)
    #define LL_mul_ll(lsrc1,lsrc2) (frac64)FRACLIB_FFMUL32(lsrc1,lsrc2)
  #else
    static inline frac64 LL_mul_ll (register frac32 lsrc1, register frac32 lsrc2)
    {
      register frac64 tmp = ((frac64)lsrc1*(frac64)lsrc2);
      return tmp+tmp;
    }
  #endif
#endif

/****************************************************************************//*!
 * @brief   Multiply 32-bit fractional value with 64-bit fractional value
 *          and return 64-bit fractional result.
 * @param   llsrc1  - Input 64-bit fractional value.
 * @param   lsrc1   - Input 32-bit fractional value.
 * @return  Function returns 64-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
#if defined(USE_MMAU)
  #define LL_mul_lll(llsrc1,lsrc2) (frac64)d_muls_dl(llsrc1,lsrc2)
#else
  #if defined(USE_FRACLIB_MUL32)
    static inline frac64 LL_mul_lll (register frac64 llsrc1, register frac32 lsrc1)
    {
      return ((FRACLIB_USMUL32 ((frac32)llsrc1,lsrc1)>>31) +
               FRACLIB_FFMUL32 ((frac32)(llsrc1>>32),lsrc1));
    }
  #else
    static inline frac64 LL_mul_lll (register frac64 llsrc1, register frac32 lsrc1)
    {
      return (((frac64)(llsrc1&0xffffffff)*(frac64)lsrc1)>>31) +
              (((frac64)(llsrc1>>32)*(frac64)lsrc1)<<1);
    }
  #endif
#endif

/****************************************************************************//*!
 * @brief   Multiply two 16-bit fractional values and add to 32-bit fractional
 *          value. Doesn't perform saturation during accumulation.
 * @param   lsrc1    - Input 32-bit fractional value.
 * @param   ssrc1    - Input 16-bit fractional value.
 * @param   ssrc2    - Input 16-bit fractional value.
 * @return  Function returns 32-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
static inline frac32 L_mac_ss (register frac32 lsrc1, register frac16 ssrc1,
                               register frac16 ssrc2)
{
  return lsrc1+L_mul_ss(ssrc1,ssrc2);
}

/****************************************************************************//*!
 * @brief   Multiply two 16-bit fractional values and add to 64-bit fractional
 *          value. Doesn't perform saturation during accumulation.
 * @param   llsrc1   - Input 64-bit fractional value.
 * @param   ssrc1    - Input 16-bit fractional value.
 * @param   ssrc2    - Input 16-bit fractional value.
 * @return  Function returns 64-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
static inline frac64 LL_mac_ss (register frac64 llsrc1, register frac16 ssrc1,
                                register frac16 ssrc2)
{
  return llsrc1+(frac64)L_mul_ss(ssrc1,ssrc2);
}

/****************************************************************************//*!
 * @brief   Multiply 32-bit fractional value with 16-bit fractional value
 *          and add result to 32-bit fractional value. Accumulation doesn't
 *          saturate.
 * @param   lsrc1    - Input 32-bit fractional value.
 * @param   lsrc2    - Input 32-bit fractional value.
 * @param   ssrc1    - Input 16-bit fractional value.
 * @return  Function returns 32-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
static inline frac32 L_mac_ls (register frac32 lsrc1, register frac32 lsrc2,
                               register frac16 ssrc1)
{
  return lsrc1+L_mul_ls(lsrc2,ssrc1);
}

/****************************************************************************//*!
 * @brief   Multiply two 32-bit fractional values and add to 32-bit fractional
 *          value. Doesn't perform saturation during accumulation.
 * @param   lsrc1    - Input 32-bit fractional value.
 * @param   lsrc2    - Input 32-bit fractional value.
 * @param   lsrc3    - Input 32-bit fractional value.
 * @return  Function returns 32-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
static inline frac32 L_mac (register frac32 lsrc1, register frac32 lsrc2,
                            register frac32 lsrc3)
{
  return lsrc1+L_mul(lsrc2,lsrc3);
}

/****************************************************************************//*!
 * @brief   Multiply two 32-bit fractional values and add to 64-bit fractional
 *          value. Doesn't perform saturation during accumulation.
 * @param   llsrc1   - Input 64-bit fractional value.
 * @param   lsrc1    - Input 32-bit fractional value.
 * @param   lsrc2    - Input 32-bit fractional value.
 * @return  Function returns 64-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
static inline frac64 LL_mac_ll (register frac64 llsrc1, register frac32 lsrc1,
                                register frac32 lsrc2)
{
  return llsrc1+LL_mul_ll(lsrc1,lsrc2);
}

/****************************************************************************//*!
 * @brief   Multiply 64-bit fractional value with 32-bit fractional value and
 *          add result to 64-bit fractional value. Doesn't perform saturation
 *          during accumulation.
 * @param   llsrc1   - Input 64-bit fractional value.
 * @param   llsrc2   - Input 64-bit fractional value.
 * @param   lsrc1    - Input 32-bit fractional value.
 * @return  Function returns 64-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
static inline frac64 LL_mac_lll (register frac64 llsrc1, register frac64 llsrc2,
                                 register frac32 lsrc1)
{
  return llsrc1+LL_mul_lll(llsrc2,lsrc1);
}

/****************************************************************************//*!
 * @brief   Multiply two 16-bit fractional values and subtract from 32-bit
 *          fractional value. Doesn't perform saturation during accumulation.
 * @param   lsrc1    - Input 32-bit fractional value.
 * @param   ssrc1    - Input 16-bit fractional value.
 * @param   ssrc2    - Input 16-bit fractional value.
 * @return  Function returns 32-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
static inline frac32 L_msu_ss (register frac32 lsrc1, register frac16 ssrc1,
                               register frac16 ssrc2)
{
  return lsrc1-L_mul_ss(ssrc1,ssrc2);
}

/****************************************************************************//*!
 * @brief   Multiply two 16-bit fractional values and subtract from 64-bit
 *          fractional value. Doesn't perform saturation during accumulation.
 * @param   llsrc1   - Input 64-bit fractional value.
 * @param   ssrc1    - Input 16-bit fractional value.
 * @param   ssrc2    - Input 16-bit fractional value.
 * @return  Function returns 64-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
static inline frac64 LL_msu_ss (register frac64 llsrc1, register frac16 ssrc1,
                                register frac16 ssrc2)
{
  return llsrc1-(frac64)L_mul_ss(ssrc1,ssrc2);
}

/****************************************************************************//*!
 * @brief   Multiply two 32-bit fractional value with 16-bit fractional value
 *          and subtract result from 32-bit fractional value. Doesn't perform
 *          saturation during accumulation.
 * @param   lsrc1    - Input 32-bit fractional value.
 * @param   lsrc2    - Input 32-bit fractional value.
 * @param   ssrc1    - Input 16-bit fractional value.
 * @return  Function returns 32-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
static inline frac32 L_msu_ls (register frac32 lsrc1, register frac32 lsrc2,
                               register frac16 ssrc1)
{
  return lsrc1-L_mul_ls(lsrc2,ssrc1);
}

/****************************************************************************//*!
 * @brief   Multiply two 32-bit fractional values and subtract it from
 *          32-bit fractional value. Doesn't perform saturation during
 *          accumulation.
 * @param   lsrc1    - Input 32-bit fractional value.
 * @param   lsrc2    - Input 32-bit fractional value.
 * @param   lsrc3    - Input 32-bit fractional value.
 * @return  Function returns 32-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
static inline frac32 L_msu (register frac32 lsrc1, register frac32 lsrc2,
                            register frac32 lsrc3)
{
  return lsrc1-L_mul(lsrc2,lsrc3);
}

/****************************************************************************//*!
 * @brief   Multiply two 32-bit fractional values and subtract it from 64-bit
 *          fractional value. Doesn't perform saturation during accumulation.
 * @param   llsrc1   - Input 64-bit fractional value.
 * @param   lsrc1    - Input 32-bit fractional value.
 * @param   lsrc2    - Input 32-bit fractional value.
 * @return  Function returns 64-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
static inline frac64 LL_msu_ll (register frac64 llsrc1, register frac32 lsrc1,
                                register frac32 lsrc2)
{
  return llsrc1-LL_mul_ll(lsrc1,lsrc2);
}

/****************************************************************************//*!
 * @brief   Multiply 64-bit fractional value with 32-bit fractional value and
 *          subtract it from 64-bit fractional value. Doesn't perform saturation
 *          during accumulation.
 * @param   llsrc1   - Input 64-bit fractional value.
 * @param   llsrc2   - Input 64-bit fractional value.
 * @param   lsrc1    - Input 32-bit fractional value.
 * @return  Function returns 64-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
static inline frac64 LL_msu_lll (register frac64 llsrc1, register frac64 llsrc2,
                                 register frac32 lsrc1)
{
  return llsrc1-LL_mul_lll(llsrc2,lsrc1);
}

/****************************************************************************//*!
 * @brief   Execute 1st order infinite impulse response filter (IIR) iteration
 *          using the following equation:
 *          y(n) = b(1)*x(n) + b(2)*x(n-1) - a(2)*y(n-na)
 *          Internal accumulations don't saturate. The IIR filter output is
 *          within 32-bit fractional range from 0x80000000 to 0x7fffffff..
 * @param   x       - Input fractional value represented in 16-bit fractional
 *                    format "x(n)".
 * @param   *px     - Pointer to previous input values represented in 16-bit
 *                    fractional format "x(n-1)".
 * @param   *py     - Pointer to previous output values represented in 16-bit
 *                    fractional format "y(n-1)".
 * @param   *pcoef  - Pointer to filter constants represented in 16-bit
 *                    fractional format "b(1), b(2) and a(2)".
 * @return  Function returns 32-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
static inline frac32 L_iir_1ord_ss (register frac16 x, register frac16 *px,
                                    register frac32 *py, const frac16 *pcoef)
{
  register frac32 tmp;

  tmp = L_msu_ls(L_mac_ss(L_mul_ss(x,*pcoef),*px,*(pcoef+1)),*py,*(pcoef+2));
  *px = x;

  return *py = tmp;
}

/****************************************************************************//*!
 * @brief   Execute 1st order infinite impulse response filter (IIR) iteration
 *          using the following equation:
 *          y(n) = b(1)*x(n) + b(2)*x(n-1) - a(2)*y(n-1)
 *          Internal accumulations don't saturate. The IIR filter output is
 *          within 32-bit fractional range from 0x80000000 to 0x7fffffff.
 * @param   x       - Input fractional value represented in 32-bit fractional
 *                    format "x(n)".
 * @param   *px     - Pointer to previous input values represented in 32-bit
 *                    fractional format "x(n-1)".
 * @param   *py     - Pointer to previous output values represented in 32-bit
 *                    fractional format "y(n-1)".
 * @param   *pcoef  - Pointer to filter constants represented in 16-bit
 *                    fractional format "b(1), b(2) and a(2)".
 * @return  Function returns 32-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
static inline frac32 L_iir_1ord_ls (register frac32 x, register frac32 *px,
                                    register frac32 *py, const frac16 *pcoef)
{
#if defined(USE_MMAU)
  muls_ll(x,*pcoef<<16); macs_ll(*px,*(pcoef+1)<<16);
  *px = x;
  return *py = l_macs_ll((~(*py)+1l),*(pcoef+2)<<16);
#else
  register frac32 tmp;

  tmp = L_msu_ls(L_mac_ls(L_mul_ls(x,*pcoef),*px,*(pcoef+1)),*py,*(pcoef+2));
  *px = x;

  return *py = tmp;
#endif
}

/****************************************************************************//*!
 * @brief   Execute 1st order infinite impulse response filter (IIR) iteration
 *          using the following equation:
 *          y(n) = b(1)*x(n) + b(2)*x(n-1) - a(2)*y(n-1)
 *          Internal accumulations don't saturate. The IIR filter output is
 *          within 32-bit fractional range from 0x80000000 to 0x7fffffff.
 * @param   x       - Input fractional value represented in 16-bit fractional
 *                    format "x(n)".
 * @param   *px     - Pointer to previous input values represented in 16-bit
 *                    fractional format "x(n-1)".
 * @param   *py     - Pointer to previous output values represented in 32-bit
 *                    fractional format "y(n-1)".
 * @param   *pcoef  - Pointer to filter constants represented in 32-bit
 *                    fractional format "b(1), b(2) and a(2)".
 * @return  Function returns 32-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
static inline frac32 L_iir_1ord_sl (register frac16 x, register frac16 *px,
                                    register frac32 *py, const frac32 *pcoef)
{
#if defined(USE_MMAU)
  muls_ll(x<<16,*pcoef); macs_ll(*px<<16,*(pcoef+1));
  *px = x;
  return *py = l_macs_ll((~(*py)+1l),*(pcoef+2));
#else
  register frac32 tmp;

  tmp = L_msu(L_mac_ls(L_mul_ls(*pcoef,x),*(pcoef+1),*px),*py,*(pcoef+2));
  *px = x;

  return *py = tmp;
#endif
}

/****************************************************************************//*!
 * @brief   Execute 1st order infinite impulse response filter (IIR) iteration
 *          using the following equation:
 *          y(n) = b(1)*x(n) + b(2)*x(n-1) - a(2)*y(n-1)
 *
 *          Internal accumulations don't saturate. The FIR filter output is
 *          within 32-bit fractional range from 0x80000000 to 0x7fffffff.
 * @param   x        - Input fractional value represented in 32-bit fractional
 *                     format "x(n)".
 * @param   *px      - Pointer to previous input values represented in 32-bit
 *                     fractional format "x(n-1)".
 * @param   *py      - Pointer to previous output values represented in 32-bit
 *                     fractional format "y(n-1)".
 * @param   *pcoef   - Pointer to filter constants represented in 32-bit
 *                     fractional format "b(1), b(2) and a(2)".
 * @return  Function returns 32-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
static inline frac32 L_iir_1ord (register frac32 x, register frac32 *px,
                                 register frac32 *py, const frac32 *pcoef)
{
#if defined(USE_MMAU)
  muls_ll(x,*pcoef); macs_ll(*px,*(pcoef+1));
  *px = x;
  return *py = l_macs_ll((~(*py)+1l),*(pcoef+2));
#else
  register frac32 tmp;

  tmp = L_msu(L_mac(L_mul(x,*pcoef),*px,*(pcoef+1)),*py,*(pcoef+2));
  *px = x;

  return *py = tmp;
#endif
}

/****************************************************************************//*!
 * @brief   Execute 1st order infinite impulse response filter (IIR) iteration
 *          using the following equation:
 *          y(n) = b(1)*x(n) + b(2)*x(n-1) - a(2)*y(n-1)
 *
 *          Internal accumulations don't saturate. The IIR filter output is within
 *          64-bit fractional range from 0x800000000000000 to 0x7fffffffffffff.
 * @param   x        - Input fractional value represented in 32-bit fractional
 *                     format "x(n)".
 * @param   *px      - Pointer to previous input values represented in 32-bit
 *                     fractional format "x(n-1)".
 * @param   *py      - Pointer to previous output values represented in 64-bit
 *                     fractional format "y(n-1)".
 * @param   *pcoef   - Pointer to filter constants represented in 32-bit fractional
 *                     format "b(1), b(2) and a(2)".
 * @return  Function returns 64-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
static inline frac64 LL_iir_1ord_ll (register frac32 x, register frac32 *px,
                                     register frac64 *py, const frac32 *pcoef)
{
#if defined(USE_MMAU)
  muls_ll(x,*pcoef); macs_ll(*px,*(pcoef+1));
  *px = x;
  return *py = d_macs_dl((~(*py)+1ll),*(pcoef+2));
#else
  register frac64 tmp;

  tmp = LL_sub(LL_mac_ll(LL_mul_ll(x,*pcoef),*px,*(pcoef+1)),LL_mul_lll(*py,*(pcoef+2)));
  *px = x;

  return *py = tmp;
#endif
}

/****************************************************************************//*!
 * @brief   Execute 1st order infinite impulse response filter (IIR) iteration
 *          using the following equation:
 *          y(n) = b(1)*x(n) + b(2)*x(n-1) - a(2)*y(n-1)
 *          Internal accumulations don't saturate. The IIR filter output is
 *          within 64-bit fractional range from 0x800000000000000 to
 *          0x7fffffffffffff.
 * @param   x        - Input fractional value represented in 64-bit fractional
 *                     format "x(n)".
 * @param   *px      - Pointer to previous input values represented in 32-bit
 *                     fractional format "x(n-1)".
 * @param   *py      - Pointer to previous output values represented in 64-bit
 *                     fractional format "y(n-1)".
 * @param   *pcoef   - Pointer to filter constants represented in 32-bit
 *                     fractional format "b(1), b(2) and a(2)".
 * @return  Function returns 64-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
static inline frac64 LL_iir_1ord (register frac64 x, register frac64 *px,
                                  register frac64 *py, const frac32 *pcoef)
{
#if defined(USE_MMAU)
  muls_dl(x,*pcoef); macs_dl(*px,*(pcoef+1));
  *px = x;
  return *py = d_macs_dl((~(*py)+1ll),*(pcoef+2));
#else
  register frac64 tmp;

  tmp = LL_sub(LL_mac_lll(LL_mul_lll(x,*pcoef),*px,*(pcoef+1)),LL_mul_lll(*py,*(pcoef+2)));
  *px = x;

  return *py = tmp;
#endif
}
#endif /* __FRACLIB_INLINES_H */
