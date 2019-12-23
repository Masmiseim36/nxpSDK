/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      FRACLIB.h
 * @author    R55013
 * @version   1.3.0.0
 * @date      Feb-08-2015
 * @brief     Header file containing common data types, macros and list of
 *            exported functions supporting 32-bit fractional arithmetic.
 ******************************************************************************/
#ifndef __FRACLIB_H
#define __FRACLIB_H

/******************************************************************************
 * include header files                                                       *
 ******************************************************************************/
#include "fraclib_inlines.h"

/******************************************************************************
 * exported function prototypes                                               *
 ******************************************************************************/

/****************************************************************************//*!
 * @brief   Division of a 32-bit fractional value by a 32-bit fractional value
 *          returning 16-bit fractional result.
 * @param   num    - Input 32-bit fractional value.
 * @param   den    - Input 32-bit fractional value.
 * @return  Function returns 16-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
#if defined(USE_MMAU)
  #define S_div_ll(num,den) S_extract_hi (l_divs_ll (num,den))
#else
  extern frac16 S_div_ll (register frac32 num, register frac32 den);
#endif

/****************************************************************************//*!
 * @brief   Division of a 32-bit fractional value by a 32-bit fractional value
 *          returning 32-bit fractional result.
 * @param   num    - Input 32-bit fractional value.
 * @param   den    - Input 32-bit fractional value.
 * @return  Function returns 32-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
#if defined(USE_MMAU)
  #define L_div(num,den) l_divs_ll (num,den)
#else
  extern frac32 L_div (register frac32 num, register frac32 den);
#endif

/****************************************************************************//*!
 * @brief   Division of a 64-bit fractional value by a 32-bit fractional value
 *          returning 32-bit fractional result.
 * @param   num    - Input 64-bit fractional value.
 * @param   den    - Input 32-bit fractional value.
 * @return  Function returns 32-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
#if defined(USE_MMAU)
  #define L_div_lll(num,den)  l_divs_dl (num,den)
#else
  extern frac32 L_div_lll (register frac64 num, register frac32 den);
#endif

/****************************************************************************//*!
 * @brief   Division of a 64-bit fractional value by a 64-bit fractional value
 *          returning 16-bit fractional result.
 * @param   num    - Input 64-bit fractional value.
 * @param   den    - Input 64-bit fractional value.
 * @return  Function returns 16-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
extern frac16 S_div_llll (register frac64 num, register frac64 den);

/****************************************************************************//*!
 * @brief   Division of a 64-bit fractional value by a 64-bit fractional value
 *          returning 64-bit fractional result.
 * @param   num    - Input 64-bit fractional value.
 * @param   den    - Input 64-bit fractional value.
 * @return  Function returns 64-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
extern frac64 LL_div (register frac64 num, register frac64 den);

/****************************************************************************//*!
 * @brief   Square root value of a 32-bit fractional value returning a 16-bit
 *          result.
 * @param   x - Input 32-bit fractional value.
 * @return  Function returns 16-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
#if defined(USE_MMAU)
  #define S_sqr_l(x) s_sqr_l (x)
#else
  extern frac16 S_sqr_l (register frac32 x);
#endif

/****************************************************************************//*!
 * @brief   Square root value of a 32-bit fractional value returning a 32-bit
 *          result.
 * @param   x - Input 32-bit fractional value.
 * @return  Function returns 32-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
#if defined(USE_MMAU)
  #define L_sqr(x) l_sqr_l (x)
#else
  extern frac32 L_sqr (register frac32 x);
#endif

/****************************************************************************//*!
 * @brief   Square root value of a 64-bit fractional value returning a 32-bit
 *          result.
 * @param   x - Input 64-bit fractional value.
 * @return  Function returns 32-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
#if defined(USE_MMAU)
  #define L_sqr_ll(x) l_sqr_d (x)
#else
  extern frac32 L_sqr_ll  (register frac64 x);
#endif

/****************************************************************************//*!
 * @brief   Square root value of a 64-bit fractional value returning a 64-bit
 *          result.
 * @param   x - Input 64-bit fractional value.
 * @return  Function returns 64-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
extern frac64 LL_sqr    (register frac64 x);

/****************************************************************************//*!
 * @brief   Execute finite impulse response filter (FIR) iteration using the
 *          following equation:
 *          y = b(1)*x(n) + b(2)*x(n-1) + ... + b(nb+1)*x(n-nb)
 *          Internal accumulations don't saturate. The FIR filter output is within
 *          32-bit fractional range from 0x80000000 to 0x7fffffff.
 * @param   x        - Input fractional value represented in 16-bit fractional
 *                     format "x(n)".
 * @param   *px      - Pointer to previous input values represented in 16-bit
 *                     fractional format "x(n-1) ... x(n-nb)".
 * @param   *pcoef   - Pointer to filter constants represented in 16-bit fractional
 *                     format "b(1) ... b(nb+1)".
 * @param   len      - Filter length "nb".
 * @return  Function returns 32-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
extern frac32 L_fir_ss  (register frac16 x, register frac16 *px, const frac16 *pcoef,
                         register uint16 len);

/****************************************************************************//*!
 * @brief   Execute finite impulse response filter (FIR) iteration using the
 *          following equation:
 *          y = b(1)*x(n) + b(2)*x(n-1) + ... + b(nb+1)*x(n-nb)
 *          Internal accumulations don't saturate. The FIR filter output is within
 *          32-bit fractional range from 0x80000000 to 0x7fffffff.
 * @param   x        - Input fractional value represented in 16-bit fractional
 *                     format "x(n)".
 * @param   *px      - Pointer to previous input values represented in 16-bit
 *                     fractional format "x(n-1) ... x(n-nb)".
 * @param   *pcoef   - Pointer to filter constants represented in 32-bit fractional
 *                     format "b(1) ... b(nb+1)".
 * @param   len      - Filter length "nb".
 * @return  Function returns 32-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
extern frac32 L_fir_sl  (register frac16 x, register frac16 *px, const frac32 *pcoef,
                         register uint16 len);

/****************************************************************************//*!
 * @brief   Execute finite impulse response filter (FIR) iteration using the
 *          following equation:
 *          y = b(1)*x(n) + b(2)*x(n-1) + ... + b(nb+1)*x(n-nb)
 *          Internal accumulations don't saturate. The FIR filter output is within
 *          32-bit fractional range from 0x80000000 to 0x7fffffff.
 * @param   x        - Input fractional value represented in 32-bit fractional
 *                     format "x(n)".
 * @param   *px      - Pointer to previous input values represented in 32-bit
 *                     fractional format "x(n-1) ... x(n-nb)".
 * @param   *pcoef   - Pointer to filter constants represented in 16-bit fractional
 *                     format "b(1) ... b(nb+1)".
 * @param   len      - Filter length "nb".
 * @return  Function returns 32-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
extern frac32 L_fir_ls  (register frac32 x, register frac32 *px, const frac16 *pcoef,
                         register uint16 len);

/****************************************************************************//*!
 * @brief   Execute finite impulse response filter (FIR) iteration using the
 *          following equation:
 *          y = b(1)*x(n) + b(2)*x(n-1) + ... + b(nb+1)*x(n-nb)
 *          Internal accumulations don't saturate. The FIR filter output is within
 *          32-bit fractional range from 0x80000000 to 0x7fffffff.
 * @param   x        - Input fractional value represented in 32-bit fractional
 *                     format "x(n)".
 * @param   *px      - Pointer to previous input values represented in 32-bit
 *                     fractional format "x(n-1) ... x(n-nb)".
 * @param   *pcoef   - Pointer to filter constants represented in 32-bit fractional
 *                     format "b(1) ... b(nb+1)".
 * @param   len      - Filter length "nb".
 * @return  Function returns 32-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
extern frac32 L_fir     (register frac32 x, register frac32 *px, const frac32 *pcoef,
                         register uint16 len);

/****************************************************************************//*!
 * @brief   Execute finite impulse response filter (FIR) iteration using the
 *          following equation:
 *          y = b(1)*x(n) + b(2)*x(n-1) + ... + b(nb+1)*x(n-nb)
 *          Internal accumulations don't saturate. The FIR filter output is within
 *          64-bit fractional range from 0x8000000000000000 to 0x7ffffffffffffff.
 * @param   x        - Input fractional value represented in 32-bit fractional
 *                     format "x(n)".
 * @param   *px      - Pointer to previous input values represented in 32-bit
 *                     fractional format "x(n-1) ... x(n-nb)".
 * @param   *pcoef   - Pointer to filter constants represented in 32-bit fractional
 *                     format "b(1) ... b(nb+1)".
 * @param   len      - Filter length "nb".
 * @return  Function returns 64-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
extern frac64 LL_fir_ll (register frac32 x, register frac32 *px, const frac32 *pcoef,
                         register uint16 len);

/****************************************************************************//*!
 * @brief   Execute finite impulse response filter (FIR) iteration using the
 *          following equation:
 *          y = b(1)*x(n) + b(2)*x(n-1) + ... + b(nb+1)*x(n-nb)
 *          Internal accumulations don't saturate. The FIR filter output is within
 *          64-bit fractional range from 0x8000000000000000 to 0x7ffffffffffffff.
 * @param   x        - Input fractional value represented in 64-bit fractional
 *                     format "x(n)".
 * @param   *px      - Pointer to previous input values represented in 64-bit
 *                     fractional format "x(n-1) ... x(n-nb)".
 * @param   *pcoef   - Pointer to filter constants represented in 32-bit fractional
 *                     format "b(1) ... b(nb+1)".
 * @param   len      - Filter length "nb".
 * @return  Function returns 64-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
extern frac64 LL_fir_lll(register frac64 x, register frac64 *px, const frac32 *pcoef,
                         register uint16 len);

/****************************************************************************//*!
 * @brief   Execute nth order infinite impulse response filter (IIR) iteration
 *          using the following equation:
 *          y(n) = (b(1)*x(n) + b(2)*x(n-1) - a(na+1)*y(n-na)) << scale
 *          Internal accumulations don't saturate. The IIR filter output is within
 *          32-bit fractional range from 0x80000000 to 0x7fffffff..
 * @param   x       - Input fractional value represented in 16-bit fractional
 *                    format "x(n)".
 * @param   *px     - Pointer to previous input values represented in 16-bit
 *                    fractional format "x(n-1) ... x(n-nb)".
 * @param   len_x   - Filter length "nb".
 * @param   *py     - Pointer to previous output values represented in 16-bit
 *                    fractional format "y(n-1) ... y(n-na)".
 * @param   len_y   - Filter length "na".
 * @param   *pcoef  - Pointer to filter constants represented in 16-bit fractional
 *                    format "b(1) ... b(nb+1), a(2) ... a(na+1)".
 * @param   scale   - Filter constants scaling.
 * @return  Function returns 32-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
extern frac32 L_iir_ss  (register frac16 x, register frac16 *px, register uint16 len_x,
                         register frac32 *py, register uint16 len_y, const frac16 *pcoef,
                         register uint16 scale);

/****************************************************************************//*!
 * @brief   Execute nth order infinite impulse response filter (IIR) iteration
 *          using the following equation:
 *          y(n) = (b(1)*x(n) + b(2)*x(n-1) - a(na+1)*y(n-na)) << scale
 *
 *          Internal accumulations don't saturate. The IIR filter output is within
 *          32-bit fractional range from 0x80000000 to 0x7fffffff..
 * @param   x       - Input fractional value represented in 32-bit fractional
 *                    format "x(n)".
 * @param   *px     - Pointer to previous input values represented in 32-bit
 *                    fractional format "x(n-1) ... x(n-nb)".
 * @param   len_x   - Filter length "nb".
 * @param   *py     - Pointer to previous output values represented in 32-bit
 *                    fractional format "y(n-1) ... y(n-na)".
 * @param   len_y   - Filter length "na".
 * @param   *pcoef  - Pointer to filter constants represented in 32-bit fractional
 *                    format "b(1) ... b(nb+1), a(2) ... a(na+1)".
 * @param   scale   - Filter constants scaling.
 * @return  Function returns 32-bit value in fractional format.
 * @remarks Implementation in C-language.
 *******************************************************************************/
extern frac32 L_iir     (register frac32 x, register frac32 *px, register uint16 len_x,
                         register frac32 *py, register uint16 len_y, const frac32 *pcoef,
                         register uint16 scale);

#endif /* __FRACLIB_H */
