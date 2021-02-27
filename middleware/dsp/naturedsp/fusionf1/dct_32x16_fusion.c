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
/*
    NatureDSP Signal Processing Library. FFT part
    C code optimized for Fusion
    DCT 32x16 with scaling option 3
*/

#include "NatureDSP_Signal.h"
#include "baseop.h"
#include "common.h"

/*
    Reference Matlab code for Radix4 DCT32

    y(1:N/2)=x(1:2:N);
    y(N:-1:N/2+1)=x(2:2:N);
    y=y(1:2:N)+i*y(2:2:N);
    Y=fft(y);
    cosi=exp(2*i*pi*(0:N/4-1)/(N));
    w=exp(-i*pi/2*(0:N-1)/N);
    % DCT split algorithm
    Y0=Y(1);
    T0=real(Y0)+imag(Y0);
    T1=real(Y0)-imag(Y0);
    z(1      )= real(T0)*sqrt(2)/2;
    z(N/2+1  )= real(T1)*sqrt(2)/2;
    for k=2:N/4
        Y0=Y(k);
        Y1=Y(N/2+2-k);
        COSI=cosi(k);
        W1=w(k);
        W2=w(N/2+2-k);
        S=Y0+Y1;
        D=Y0-Y1;
        T0=i*real(D)+imag(S);
        T1=i*imag(D)+real(S);
        T1=T1/2;
        Y0=  (imag(T0)*imag(COSI)-real(T0)*real(COSI)) + ...
           i*(real(T0)*imag(COSI)+imag(T0)*real(COSI));
        Y0=Y0/2;
        T0=T1-Y0;
        T1=T1+Y0;
        z(k      )= real(T0)*real(W1)-imag(T0)*imag(W1);
        z(N+2-k  )=-real(T0)*imag(W1)-imag(T0)*real(W1);
        z(N/2+2-k)= real(T1)*real(W2)+imag(T1)*imag(W2);
        z(N/2+k  )=-real(T1)*imag(W2)+imag(T1)*real(W2);
    end
    W1=w(N/4+1);
    T0=Y(N/4+1);
    z(N/4+1  )= real(T0)*real(W1)+imag(T0)*imag(W1);
    z(N+1-N/4)=-real(T0)*imag(W1)+imag(T0)*real(W1);
    z=z*sqrt(2/N);

*/

static
#ifdef COMPILER_MSVC
        ALIGN(8)
#endif
                  const int16_t ae_cosi[]
#ifndef COMPILER_MSVC
                                        ALIGN(8)
#endif
                                                  =
{
  0x18F9, 0x7D8A,
  0x30FC, 0x7642, 0x471D, 0x6A6E,
  0x5A82, 0x5A82, 0x6A6E, 0x471D,
  0x7642, 0x30FC, 0x7D8A, 0x18F9,
  0,      0
};

static
#ifdef COMPILER_MSVC
        ALIGN(8)
#endif
                  const int16_t ae_w[]
#ifndef COMPILER_MSVC
                                        ALIGN(8)
#endif
                                                  =
{
  (int16_t)0x7fff, (int16_t)0x5A82,
  (int16_t)0x7FD9, (int16_t)0xF9B8, (int16_t)0x5ED7, (int16_t)0x55F6,
  (int16_t)0x7F62, (int16_t)0xF374, (int16_t)0x62F2, (int16_t)0x5134,
  (int16_t)0x7E9D, (int16_t)0xED38, (int16_t)0x66D0, (int16_t)0x4C40,
  (int16_t)0x7D8A, (int16_t)0xE707, (int16_t)0x6A6E, (int16_t)0x471D,
  (int16_t)0x7C2A, (int16_t)0xE0E6, (int16_t)0x6DCA, (int16_t)0x41CE,
  (int16_t)0x7A7D, (int16_t)0xDAD8, (int16_t)0x70E3, (int16_t)0x3C57,
  (int16_t)0x7885, (int16_t)0xD4E1, (int16_t)0x73B6, (int16_t)0x36BA,
  (int16_t)0x7642, (int16_t)0x30FC
};

/*
	in-place split part of DCT:
	y[2*N]	input
	z[2*N]	output
	N		size of FFT
*/
static void dct_split_32_32x16(int32_t * z, const int32_t * y)
{
  const ae_int32x2  * restrict p_y0,  * restrict p_y1;
  const ae_int16x4  * restrict p_cos, * restrict p_w;
  ae_int32x2        * restrict p_z0,  * restrict p_z1;

  ae_f32x2    vF0, vF1;
  ae_f16x4    vC0, vW0, vW1;
  ae_int16x4  vST;
  ae_int32x2  vA0, vA1, vB0, vB1, vR0;

  const int step_back = -(int)sizeof(ae_int32x2);

  int i;

  p_y0  = (const ae_int32x2 *)(y);
  p_y1  = (const ae_int32x2 *)(y+30);
  p_cos = (const ae_int16x4 *)ae_cosi;
  p_w   = (const ae_int16x4 *)ae_w;
  p_z0  = (      ae_int32x2 *)(z);
  p_z1  = (      ae_int32x2 *)(z+30);

  /* load data and prepare pointers for pre-increment
     first and last samples */
  AE_L32X2_IP(vA0, p_y0, 8);
  AE_L16X4_IP(vST, p_w, 8);
  vW0 = (vST);
  vA1 = AE_SEL32_LH(vA0, vA0);
  vB0 = AE_ADDSUB32S(vA1, vA0);
  vR0 = AE_MULFP32X16X2RAS_H(vB0, vST);

  AE_L32X2_IP(vA0, p_y0, 8);
  AE_L32X2_XP(vA1, p_y1, step_back);
  AE_L16X4_IP(vST, p_cos, 8);
  vC0 = (vST);
  AE_L16X4_IP(vST, p_w, 8);
  vW1 = (vST);

  vB0 = AE_SUBADD32S(vA0, vA1);
  vB1 = AE_ADDSUB32S(vA0, vA1);
  vA0 = AE_MULFC32X16RAS_H(vB0, vC0);

  vA0 = AE_SRAI32(vA0, 1);
  vA1 = AE_SRAI32(vB1, 1);

  vB0 = AE_SUB32S(vA1, vA0);
  vB1 = AE_ADD32S(vA1, vA0);

  /*
    z(k      )= real(T0)*real(W1)-imag(T0)*imag(W1);
    z(N+2-k  )=-real(T0)*imag(W1)-imag(T0)*real(W1);
    z(N/2+2-k)= real(T1)*real(W2)+imag(T1)*imag(W2);
    z(N/2+k  )=-real(T1)*imag(W2)+imag(T1)*real(W2);
  */
  vA0 = AE_MULFC32X16RAS_L(vB0, vW0);
  vA1 = AE_NEG32S(vA0);
  vB1 = AE_MULFC32X16RAS_H(vB1, vW1);

  vA0 = AE_SEL32_HH(vR0, vA0);
  vB0 = AE_SEL32_LL(vR0, vB1);
  vR0 = AE_SEL32_HL(vB1, vA1);

  AE_S32X2_IP(vA0, p_z0, sizeof(*p_z0));
  AE_S32X2_I(vB0, p_z0, 7*sizeof(*p_z0));

  for (i = 1; i < (32>>3); i++)
  {
    /*
      Y0=Y(k);
      Y1=Y(N/2+2-k);
      COSI=cosi(k);
      W1=w(k);
      W2=w(N/2+2-k);
      S=Y0+Y1;
      D=Y0-Y1;
      T0=i*real(D)+imag(S);
      T1=i*imag(D)+real(S);
      T1=T1/2;
      Y0=  (imag(T0)*imag(COSI)-real(T0)*real(COSI)) + ...
            i*(real(T0)*imag(COSI)+imag(T0)*real(COSI));
      Y0=Y0/2;
      T0=T1-Y0;
      T1=T1+Y0;
    */
    AE_L32X2_IP(vA0, p_y0, 8);
    AE_L32X2_XP(vA1, p_y1, step_back);
    AE_L16X4_IP(vST, p_w, 8);
    vW0 = (vST);

    vB0 = AE_SUBADD32S(vA0, vA1);
    vB1 = AE_ADDSUB32S(vA0, vA1);
    vA0 = AE_MULFC32X16RAS_L(vB0, vC0);
    vA0 = AE_SRAI32(vA0, 1);
    vA1 = AE_SRAI32(vB1, 1);

    vB0 = AE_SUB32S(vA1, vA0);
    vB1 = AE_ADD32S(vA1, vA0);
 
    /*
      z(k      )= real(T0)*real(W1)-imag(T0)*imag(W1);
      z(N+2-k  )=-real(T0)*imag(W1)-imag(T0)*real(W1);
      z(N/2+2-k)= real(T1)*real(W2)+imag(T1)*imag(W2);
      z(N/2+k  )=-real(T1)*imag(W2)+imag(T1)*real(W2);
    */
    vA0 = AE_MULFC32X16RAS_L(vB0, vW1);
    vA1 = AE_NEG32S(vA0);
    vB1 = AE_MULFC32X16RAS_H(vB1, vW0);

    vA1 = AE_SEL32_LL(vA1, vR0);
    vB0 = AE_SEL32_HH(vB1, vR0);
    vR0 = AE_SEL32_HL(vA0, vB1);

    AE_S32X2_I(vB0, p_z1, -8*(int)sizeof(*p_z0));
    AE_S32X2_XP(vA1, p_z1, step_back);

    AE_L32X2_IP(vA0, p_y0, 8);
    AE_L32X2_XP(vA1, p_y1, step_back);
    AE_L16X4_IP(vST, p_cos, 8);
    vC0 = (vST);
    AE_L16X4_IP(vST, p_w, 8);
    vW1 = (vST);

    vB0 = AE_SUBADD32S(vA0, vA1);
    vB1 = AE_ADDSUB32S(vA0, vA1);
    vA0 = AE_MULFC32X16RAS_H(vB0, vC0);
 
    vA0 = AE_SRAI32(vA0, 1);
    vA1 = AE_SRAI32(vB1, 1);

    vB0 = AE_SUB32S(vA1, vA0);
    vB1 = AE_ADD32S(vA1, vA0);

    vF0 = (vB0);
    vF1 = (vB1);

    /*
      z(k      )= real(T0)*real(W1)-imag(T0)*imag(W1);
      z(N+2-k  )=-real(T0)*imag(W1)-imag(T0)*real(W1);
      z(N/2+2-k)= real(T1)*real(W2)+imag(T1)*imag(W2);
      z(N/2+k  )=-real(T1)*imag(W2)+imag(T1)*real(W2);
    */
    vA0 = AE_MULFC32X16RAS_L(vB0, vW0);
    vA1 = AE_NEG32S(vA0);
    vB1 = AE_MULFC32X16RAS_H(vB1, vW1);

    vA0 = AE_SEL32_HH(vR0, vA0);
    vB0 = AE_SEL32_LL(vR0, vB1);
    vR0 = AE_SEL32_HL(vB1, vA1);

    AE_S32X2_IP(vA0, p_z0, sizeof(*p_z0));
    AE_S32X2_I(vB0, p_z0, 7*sizeof(*p_z0));
  }

  /*** middle sample ***/
  /*
    W1=w(N/4+1);
    T0=Y(N/4+1);
    z(N/4+1  )= real(T0)*real(W1)+imag(T0)*imag(W1);
    z(N+1-N/4)=-real(T0)*imag(W1)+imag(T0)*real(W1);
  */
  AE_L32X2_IP(vA0, p_y0, 8);

  vB0 = AE_MULFC32X16RAS_L(vA0, vW1);

  vA0 = AE_SEL32_HH(vB0, vR0);
  vA1 = AE_SEL32_LL(vB0, vR0);

  AE_S32X2_I(vA0, p_z0, 0);
  AE_S32X2_I(vA1, p_z1, 0);
}

static
#ifdef COMPILER_MSVC
        ALIGN(8)
#endif
                  const int16_t ae_twd16[20]
#ifndef COMPILER_MSVC
                                              ALIGN(8)
#endif
                                                        =
{
                       0x7642,  -0x30fc,
   0x5a82,  -0x5a82,   0x30fc,  -0x7642,
   0x5a82,  -0x5a82,   0x0000,   MIN_INT16,
  -0x5a82,  -0x5a82,   0x30fc,  -0x7642,
  -0x5a82,  -0x5a82,  -0x7642,   0x30fc,
  0,        0
};

// scaled fft16
// NOTE: y is input and output, x - temporary
static void fft16_32x16(int32_t *y, int32_t *x)
{
  ae_int32x2  * p_x0, * p_x1, * restrict p_y0;
  const ae_int16x4  * restrict p_twd;
  ae_int32x2  vA0, vA1, vA2, vA3, vB0, vB1, vB2, vB3;
  ae_f16x4    vT1, vT2;
  ae_int16x4  vTT;

  const int   step_right  = (int)sizeof(ae_int32x2);
  const int   step_left   = -step_right;
  const int   step_down   = 4*step_right;
  const int   step_nextc  = step_right - 3*step_down;
  const int   step_hupleft= 7*step_left;
  const int   step_upleft = 15*step_left;

  p_x0  = (ae_int32x2 *)(x);
  p_x1  = (ae_int32x2 *)(x+2*15);
  p_y0  = (ae_int32x2 *)(y);
  p_twd = (const ae_int16x4 *)(ae_twd16);

  /*** reordering ***/
  AE_L32X2_XP(vA0, p_y0, step_right);
  AE_L32X2_XP(vA1, p_y0, step_right);
  AE_L32X2_XP(vA2, p_y0, step_right);
  AE_L32X2_XP(vA3, p_y0, step_right);

  vB0 = AE_SEL32_HH(vA0, vA1);
  vB3 = AE_SEL32_LL(vA1, vA0);
  AE_L32X2_XP(vA0, p_y0, step_right);
  AE_L32X2_XP(vA1, p_y0, step_right);
  vB1 = AE_SEL32_HH(vA2, vA3);
  vB2 = AE_SEL32_LL(vA3, vA2);
  AE_L32X2_XP(vA2, p_y0, step_right);
  AE_L32X2_XP(vA3, p_y0, step_right);
  AE_S32X2_XP(vB0, p_x0, step_right);
  AE_S32X2_XP(vB3, p_x1, step_left);
  AE_S32X2_XP(vB1, p_x0, step_right);
  AE_S32X2_XP(vB2, p_x1, step_left);

  vB0 = AE_SEL32_HH(vA0, vA1);
  vB3 = AE_SEL32_LL(vA1, vA0);
  AE_L32X2_XP(vA0, p_y0, step_right);
  AE_L32X2_XP(vA1, p_y0, step_right);
  vB1 = AE_SEL32_HH(vA2, vA3);
  vB2 = AE_SEL32_LL(vA3, vA2);
  AE_L32X2_XP(vA2, p_y0, step_right);
  AE_L32X2_XP(vA3, p_y0, step_right);
  AE_S32X2_XP(vB0, p_x0, step_right);
  AE_S32X2_XP(vB3, p_x1, step_left);
  AE_S32X2_XP(vB1, p_x0, step_right);
  AE_S32X2_XP(vB2, p_x1, step_left);

  vB0 = AE_SEL32_HH(vA0, vA1);
  vB3 = AE_SEL32_LL(vA1, vA0);
  AE_L32X2_XP(vA0, p_y0, step_right);
  AE_L32X2_XP(vA1, p_y0, step_right);
  vB1 = AE_SEL32_HH(vA2, vA3);
  vB2 = AE_SEL32_LL(vA3, vA2);
  AE_L32X2_XP(vA2, p_y0, step_right);
  AE_L32X2_XP(vA3, p_y0, step_upleft);
  AE_S32X2_XP(vB0, p_x0, step_right);
  AE_S32X2_XP(vB3, p_x1, step_left);
  AE_S32X2_XP(vB1, p_x0, step_right);
  AE_S32X2_XP(vB2, p_x1, step_left);

  vB0 = AE_SEL32_HH(vA0, vA1);
  vB3 = AE_SEL32_LL(vA1, vA0);
  vB1 = AE_SEL32_HH(vA2, vA3);
  vB2 = AE_SEL32_LL(vA3, vA2);
  AE_S32X2_XP(vB0, p_x0, step_right);
  AE_S32X2_XP(vB3, p_x1, step_left);
  AE_S32X2_XP(vB1, p_x0, step_hupleft);
  AE_S32X2_XP(vB2, p_x1, 0);

  /*** first radix4 stage ***/
  /* Elements 0, 4, 8, 12 */
  vA0 = AE_L32X2_I(p_x1, (0-8)*(int)sizeof(ae_int32x2));
  vA1 = AE_L32X2_I(p_x1, (4-8)*(int)sizeof(ae_int32x2));
  vA2 = AE_L32X2_I(p_x1, (8-8)*(int)sizeof(ae_int32x2));
  vA3 = AE_L32X2_I(p_x1, (12-8)*(int)sizeof(ae_int32x2));

  vA0 = AE_SRAI32(vA0, 3);
  vA1 = AE_SRAI32(vA1, 3);
  vA2 = AE_SRAI32(vA2, 3);
  vA3 = AE_SRAI32(vA3, 3);

  vB0 = AE_ADD32S(vA0, vA2);
  vB2 = AE_SUB32S(vA0, vA2);
  vB1 = AE_ADD32S(vA1, vA3);
  vA2 = AE_SUB32S(vA3, vA1);
  vB3 = AE_SUB32S(vA1, vA3);
  vB3 = AE_SEL32_LH(vA2, vB3);

  vA0 = AE_ADD32S(vB0, vB1);
  vA1 = AE_SUB32S(vB0, vB1);
  vA2 = AE_ADD32S(vB2, vB3);
  vA3 = AE_SUB32S(vB2, vB3);

  vB0 = AE_SRAI32(vA0, 2);
  vB1 = AE_SRAI32(vA1, 2);
  vB2 = AE_SRAI32(vA2, 2);
  vB3 = AE_SRAI32(vA3, 2);

  AE_S32X2_I(vB0, p_x1, (0-8)*(int)sizeof(ae_int32x2));
  AE_S32X2_I(vB3, p_x1, (4-8)*(int)sizeof(ae_int32x2));
  AE_S32X2_I(vB1, p_x1, (8-8)*(int)sizeof(ae_int32x2));
  AE_S32X2_I(vB2, p_x1, (12-8)*(int)sizeof(ae_int32x2));

  /* Elements 1, 5, 9, 13 */
  vA0 = AE_L32X2_I(p_x1, (1-8)*(int)sizeof(ae_int32x2));
  vA1 = AE_L32X2_I(p_x1, (5-8)*(int)sizeof(ae_int32x2));
  vA2 = AE_L32X2_I(p_x1, (9-8)*(int)sizeof(ae_int32x2));
  vA3 = AE_L32X2_I(p_x1, (13-8)*(int)sizeof(ae_int32x2));

  vA0 = AE_SRAI32(vA0, 3);
  vA1 = AE_SRAI32(vA1, 3);
  vA2 = AE_SRAI32(vA2, 3);
  vA3 = AE_SRAI32(vA3, 3);

  vB0 = AE_ADD32S(vA0, vA2);
  vB2 = AE_SUB32S(vA0, vA2);
  vB1 = AE_ADD32S(vA1, vA3);
  vA2 = AE_SUB32S(vA3, vA1);
  vB3 = AE_SUB32S(vA1, vA3);
  vB3 = AE_SEL32_LH(vA2, vB3);

  vA0 = AE_ADD32S(vB0, vB1);
  vA1 = AE_SUB32S(vB0, vB1);
  vA2 = AE_ADD32S(vB2, vB3);
  vA3 = AE_SUB32S(vB2, vB3);

  AE_L16X4_IP(vTT, p_twd, 8);
  vT1 = (vTT);
  AE_L16X4_IP(vTT, p_twd, 8);
  vT2 = (vTT);

  vA3 = AE_MULFC32X16RAS_H(vA3, vT1);
  vA1 = AE_MULFC32X16RAS_L(vA1, vT1);
  vA2 = AE_MULFC32X16RAS_H(vA2, vT2);

  vB0 = AE_SRAI32(vA0, 2);
  vB1 = AE_SRAI32(vA1, 2);
  vB2 = AE_SRAI32(vA2, 2);
  vB3 = AE_SRAI32(vA3, 2);

  AE_S32X2_I(vB0, p_x1, (1-8)*(int)sizeof(ae_int32x2));
  AE_S32X2_I(vB3, p_x1, (5-8)*(int)sizeof(ae_int32x2));
  AE_S32X2_I(vB1, p_x1, (9-8)*(int)sizeof(ae_int32x2));
  AE_S32X2_I(vB2, p_x1, (13-8)*(int)sizeof(ae_int32x2));

  /* Elements 2, 6, 10, 14 */
  vA0 = AE_L32X2_I(p_x1, (2-8)*(int)sizeof(ae_int32x2));
  vA1 = AE_L32X2_I(p_x1, (6-8)*(int)sizeof(ae_int32x2));
  vA2 = AE_L32X2_I(p_x1, (10-8)*(int)sizeof(ae_int32x2));
  vA3 = AE_L32X2_I(p_x1, (14-8)*(int)sizeof(ae_int32x2));

  vA0 = AE_SRAI32(vA0, 3);
  vA1 = AE_SRAI32(vA1, 3);
  vA2 = AE_SRAI32(vA2, 3);
  vA3 = AE_SRAI32(vA3, 3);

  vB0 = AE_ADD32S(vA0, vA2);
  vB2 = AE_SUB32S(vA0, vA2);
  vB1 = AE_ADD32S(vA1, vA3);
  vA2 = AE_SUB32S(vA3, vA1);
  vB3 = AE_SUB32S(vA1, vA3);
  vB3 = AE_SEL32_LH(vA2, vB3);

  vA0 = AE_ADD32S(vB0, vB1);
  vA1 = AE_SUB32S(vB0, vB1);
  vA2 = AE_ADD32S(vB2, vB3);
  vA3 = AE_SUB32S(vB2, vB3);

  AE_L16X4_IP(vTT, p_twd, 8);
  vT1 = (vTT);

  vA3 = AE_MULFC32X16RAS_L(vA3, vT2);
  vA1 = AE_MULFC32X16RAS_H(vA1, vT1);
  vA2 = AE_MULFC32X16RAS_L(vA2, vT1);

  vB0 = AE_SRAI32(vA0, 2);
  vB1 = AE_SRAI32(vA1, 2);
  vB2 = AE_SRAI32(vA2, 2);
  vB3 = AE_SRAI32(vA3, 2);

  AE_S32X2_I(vB0, p_x1, (2-8)*(int)sizeof(ae_int32x2));
  AE_S32X2_I(vB3, p_x1, (6-8)*(int)sizeof(ae_int32x2));
  AE_S32X2_I(vB1, p_x1, (10-8)*(int)sizeof(ae_int32x2));
  AE_S32X2_I(vB2, p_x1, (14-8)*(int)sizeof(ae_int32x2));

  /* Elements 3, 7, 11, 15 */
  vA0 = AE_L32X2_I(p_x1, (3-8)*(int)sizeof(ae_int32x2));
  vA1 = AE_L32X2_I(p_x1, (7-8)*(int)sizeof(ae_int32x2));
  vA2 = AE_L32X2_I(p_x1, (11-8)*(int)sizeof(ae_int32x2));
  vA3 = AE_L32X2_I(p_x1, (15-8)*(int)sizeof(ae_int32x2));

  vA0 = AE_SRAI32(vA0, 3);
  vA1 = AE_SRAI32(vA1, 3);
  vA2 = AE_SRAI32(vA2, 3);
  vA3 = AE_SRAI32(vA3, 3);

  vB0 = AE_ADD32S(vA0, vA2);
  vB2 = AE_SUB32S(vA0, vA2);
  vB1 = AE_ADD32S(vA1, vA3);
  vA2 = AE_SUB32S(vA3, vA1);
  vB3 = AE_SUB32S(vA1, vA3);
  vB3 = AE_SEL32_LH(vA2, vB3);

  vA0 = AE_ADD32S(vB0, vB1);
  vA1 = AE_SUB32S(vB0, vB1);
  vA2 = AE_ADD32S(vB2, vB3);
  vA3 = AE_SUB32S(vB2, vB3);

  AE_L16X4_IP(vTT, p_twd, 8);
  vT1 = (vTT);
  AE_L16X4_IP(vTT, p_twd, 8);
  vT2 = (vTT);

  vA3 = AE_MULFC32X16RAS_H(vA3, vT1);
  vA1 = AE_MULFC32X16RAS_L(vA1, vT1);
  vA2 = AE_MULFC32X16RAS_H(vA2, vT2);

  vB0 = AE_SRAI32(vA0, 2);
  vB1 = AE_SRAI32(vA1, 2);
  vB2 = AE_SRAI32(vA2, 2);
  vB3 = AE_SRAI32(vA3, 2);

  AE_S32X2_I(vB0, p_x1, (3-8)*(int)sizeof(ae_int32x2));
  AE_S32X2_I(vB3, p_x1, (7-8)*(int)sizeof(ae_int32x2));
  AE_S32X2_I(vB1, p_x1, (11-8)*(int)sizeof(ae_int32x2));
  AE_S32X2_I(vB2, p_x1, (15-8)*(int)sizeof(ae_int32x2));

  /*** last radix4 stage ***/
  AE_L32X2_XP(vA0, p_x0, step_right);
  AE_L32X2_XP(vA1, p_x0, step_right);
  AE_L32X2_XP(vA2, p_x0, step_right);
  AE_L32X2_XP(vA3, p_x0, step_right);

  vB0 = AE_ADD32S(vA0, vA2);
  vB2 = AE_SUB32S(vA0, vA2);
  vB1 = AE_ADD32S(vA1, vA3);
  vA2 = AE_SUB32S(vA3, vA1);
  vB3 = AE_SUB32S(vA1, vA3);
  vB3 = AE_SEL32_LH(vA2, vB3);

  vA0 = AE_ADD32S(vB0, vB1);
  vA2 = AE_SUB32S(vB0, vB1);
  vA1 = AE_SUB32S(vB2, vB3);
  vA3 = AE_ADD32S(vB2, vB3);

  AE_S32X2_XP(vA0, p_y0, step_down);
  AE_S32X2_XP(vA1, p_y0, step_down);
  AE_S32X2_XP(vA2, p_y0, step_down);
  AE_S32X2_XP(vA3, p_y0, step_nextc);

  AE_L32X2_XP(vA0, p_x0, step_right);
  AE_L32X2_XP(vA1, p_x0, step_right);
  AE_L32X2_XP(vA2, p_x0, step_right);
  AE_L32X2_XP(vA3, p_x0, step_right);

  vB0 = AE_ADD32S(vA0, vA2);
  vB2 = AE_SUB32S(vA0, vA2);
  vB1 = AE_ADD32S(vA1, vA3);
  vA2 = AE_SUB32S(vA3, vA1);
  vB3 = AE_SUB32S(vA1, vA3);
  vB3 = AE_SEL32_LH(vA2, vB3);

  vA0 = AE_ADD32S(vB0, vB1);
  vA2 = AE_SUB32S(vB0, vB1);
  vA1 = AE_SUB32S(vB2, vB3);
  vA3 = AE_ADD32S(vB2, vB3);

  AE_S32X2_XP(vA0, p_y0, step_down);
  AE_S32X2_XP(vA1, p_y0, step_down);
  AE_S32X2_XP(vA2, p_y0, step_down);
  AE_S32X2_XP(vA3, p_y0, step_nextc);

  AE_L32X2_XP(vA0, p_x0, step_right);
  AE_L32X2_XP(vA1, p_x0, step_right);
  AE_L32X2_XP(vA2, p_x0, step_right);
  AE_L32X2_XP(vA3, p_x0, step_right);

  vB0 = AE_ADD32S(vA0, vA2);
  vB2 = AE_SUB32S(vA0, vA2);
  vB1 = AE_ADD32S(vA1, vA3);
  vA2 = AE_SUB32S(vA3, vA1);
  vB3 = AE_SUB32S(vA1, vA3);
  vB3 = AE_SEL32_LH(vA2, vB3);

  vA0 = AE_ADD32S(vB0, vB1);
  vA2 = AE_SUB32S(vB0, vB1);
  vA1 = AE_SUB32S(vB2, vB3);
  vA3 = AE_ADD32S(vB2, vB3);

  AE_S32X2_XP(vA0, p_y0, step_down);
  AE_S32X2_XP(vA1, p_y0, step_down);
  AE_S32X2_XP(vA2, p_y0, step_down);
  AE_S32X2_XP(vA3, p_y0, step_nextc);

  AE_L32X2_XP(vA0, p_x0, step_right);
  AE_L32X2_XP(vA1, p_x0, step_right);
  AE_L32X2_XP(vA2, p_x0, step_right);
  AE_L32X2_XP(vA3, p_x0, step_right);

  vB0 = AE_ADD32S(vA0, vA2);
  vB2 = AE_SUB32S(vA0, vA2);
  vB1 = AE_ADD32S(vA1, vA3);
  vA2 = AE_SUB32S(vA3, vA1);
  vB3 = AE_SUB32S(vA1, vA3);
  vB3 = AE_SEL32_LH(vA2, vB3);

  vA0 = AE_ADD32S(vB0, vB1);
  vA2 = AE_SUB32S(vB0, vB1);
  vA1 = AE_SUB32S(vB2, vB3);
  vA3 = AE_ADD32S(vB2, vB3);

  AE_S32X2_XP(vA0, p_y0, step_down);
  AE_S32X2_XP(vA1, p_y0, step_down);
  AE_S32X2_XP(vA2, p_y0, step_down);
  AE_S32X2_I(vA3, p_y0, 0);
}
/*-------------------------------------------------------------------------
  Discrete Cosine Transform :
  These functions apply DCT (Type II) to input
  NOTES:
  1. DCT runs in-place algorithm so INPUT DATA WILL APPEAR DAMAGED after 
     the call.

  Precision: 
  32x32  32-bit input/outputs, 32-bit twiddles
  24x24  24-bit input/outputs, 24-bit twiddles
  32x16  32-bit input/outputs, 16-bit twiddles
  16x16  16-bit input/outputs, 16-bit twiddles
  f      floating point

  Input:
  x[N]          input signal
  N             DCT size
  scalingOpt    scaling option
  Output:
  y[N]          transform output
  return ed value:
  total number of right shifts occurred during scaling procedure (always 5 
  for fixed point functions and 0 for floating point function)
  Restriction:
  x,y should not overlap
  x,y - aligned on 8-bytes boundary
  N   - 32 for fixed point routines, 32 or 64 for floating point

-------------------------------------------------------------------------*/
int dct_32x16( 
              int32_t* y,
              int32_t* x,
              int N,
              int scalingOpt)
{
    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);
    NASSERT(scalingOpt==3);
    NASSERT(N==32);

    fft16_32x16(x, y);    /* fft of half-size with reordering */
    dct_split_32_32x16(y,x);
    return 5;
}
