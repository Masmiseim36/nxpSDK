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
    DCT 24x24 with scaling option 3
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
        Y0= (imag(T0)*imag(COSI)-real(T0)*real(COSI)) + ...
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
                  const int32_t ae_cosi[]
#ifndef COMPILER_MSVC
                                        ALIGN(8)
#endif
                                                  =
{
  0x18F8B83C, 0x7D8A5F40,
  0x30FBC54D, 0x7641AF3D, 0x471CECE7, 0x6A6D98A4,
  0x5A82799A, 0x5A82799A, 0x6A6D98A4, 0x471CECE7,
  0x7641AF3D, 0x30FBC54D, 0x7D8A5F40, 0x18F8B83C
};

static
#ifdef COMPILER_MSVC
        ALIGN(8)
#endif
                  const int32_t ae_w[]
#ifndef COMPILER_MSVC
                                        ALIGN(8)
#endif
                                                  =
{
  //(int32_t)0x5A82799A, (int32_t)0x5A82799A,
  (int32_t)0x7fffffff, (int32_t)0x5A82799A,
  (int32_t)0x7FD8878E, (int32_t)0xF9B82684, (int32_t)0x5ED77C8A, (int32_t)0x55F5A4D2,
  (int32_t)0x7F62368F, (int32_t)0xF3742CA2, (int32_t)0x62F201AC, (int32_t)0x5133CC94,
  (int32_t)0x7E9D55FC, (int32_t)0xED37EF91, (int32_t)0x66CF8120, (int32_t)0x4C3FDFF4,
  (int32_t)0x7D8A5F40, (int32_t)0xE70747C4, (int32_t)0x6A6D98A4, (int32_t)0x471CECE7,
  (int32_t)0x7C29FBEE, (int32_t)0xE0E60685, (int32_t)0x6DCA0D14, (int32_t)0x41CE1E65,
  (int32_t)0x7A7D055B, (int32_t)0xDAD7F3A2, (int32_t)0x70E2CBC6, (int32_t)0x3C56BA70,
  (int32_t)0x78848414, (int32_t)0xD4E0CB15, (int32_t)0x73B5EBD1, (int32_t)0x36BA2015,
  (int32_t)0x7641AF3D, (int32_t)0x30FBC54D
};

/*
	in-place split part of DCT:
	y[2*N]	input
	z[2*N]	output
	N		size of FFT
*/
static void dct_split_32_24x24(int32_t * z, const int32_t * y)
{
  const ae_f24x2  * restrict p_y0, * restrict p_y1, * restrict p_cos, * restrict p_w;
  ae_f24x2        * restrict p_z0, * restrict p_z1;

  ae_f24x2    vF0, vF1, vC0, vW0, vW1;
  ae_f32x2    vFR;
  ae_int32x2  vA0, vA1, vB0, vB1, vR0;

  const int step_back = -(int)sizeof(ae_f24x2);

  int i;

  p_y0  = (const ae_f24x2 *)(y);
  p_y1  = (const ae_f24x2 *)(y+30);
  p_cos = (const ae_f24x2 *)ae_cosi;
  p_w   = (const ae_f24x2 *)ae_w;
  p_z0  = (      ae_f24x2 *)(z);
  p_z1  = (      ae_f24x2 *)(z+30);

  /* load data and prepare pointers for pre-increment
     first and last samples */
  AE_L32X2F24_IP(vF0, p_y0, 8);
  AE_L32X2F24_IP(vW0, p_w, 8);
  vA0 = (vF0);
  vA1 = AE_SEL32_LH(vA0, vA0);
  vB0 = AE_ADDSUB32S(vA1, vA0);
  
  vF0 = AE_MOVF24X2_FROMINT32X2(vB0);
  vFR = AE_MULFP24X2RA(vF0, vW0);
  vR0 = (vFR);


  AE_L32X2F24_IP(vF0, p_y0, 8);
  AE_L32X2F24_XP(vF1, p_y1, step_back);
  AE_L32X2F24_IP(vC0, p_cos, 8);
  AE_L32X2F24_IP(vW0, p_w, 8);
  AE_L32X2F24_IP(vW1, p_w, 8);

  vB0 = AE_SUBADD32S(vF0, vF1);
  vB1 = AE_ADDSUB32S(vF0, vF1);

  vF0 = AE_MOVF24X2_FROMINT32X2(vB0);
  vFR = AE_MULFC24RA(vF0, vC0);

  vA0 = AE_SRAI32(vFR, 1);
  vA1 = AE_SRAI32(vB1, 1);

  vB0 = AE_SUB32S(vA1, vA0);
  vB1 = AE_ADD32S(vA1, vA0);

  vF0 = AE_MOVF24X2_FROMINT32X2(vB0);
  vF1 = AE_MOVF24X2_FROMINT32X2(vB1);

  /*
    z(k      )= real(T0)*real(W1)-imag(T0)*imag(W1);
    z(N+2-k  )=-real(T0)*imag(W1)-imag(T0)*real(W1);
    z(N/2+2-k)= real(T1)*real(W2)+imag(T1)*imag(W2);
    z(N/2+k  )=-real(T1)*imag(W2)+imag(T1)*real(W2);
  */
  vA0 = AE_MULFC24RA(vF0, vW0);
  vA1 = AE_NEG24S(vA0);
  vB1 = AE_MULFC24RA(vF1, vW1);

  vA0 = AE_SEL32_HH(vR0, vA0);
  vB0 = AE_SEL32_LL(vR0, vB1);
  vR0 = AE_SEL32_HL(vB1, vA1);

  vF0 = AE_MOVF24X2_FROMINT32X2(vA0);
  vF1 = AE_MOVF24X2_FROMINT32X2(vB0);

  AE_S32X2F24_IP(vF0, p_z0, sizeof(*p_z0));
  AE_S32X2F24_I(vF1, p_z0, 7*sizeof(*p_z0));

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
      Y0= (imag(T0)*imag(COSI)-real(T0)*real(COSI)) + ...
            i*(real(T0)*imag(COSI)+imag(T0)*real(COSI));
      Y0=Y0/2;
      T0=T1-Y0;
      T1=T1+Y0;
    */
    AE_L32X2F24_IP(vF0, p_y0, 8);
    AE_L32X2F24_XP(vF1, p_y1, step_back);
    AE_L32X2F24_IP(vC0, p_cos, 8);
    AE_L32X2F24_IP(vW0, p_w, 8);
    AE_L32X2F24_IP(vW1, p_w, 8);

    vB0 = AE_SUBADD32S(vF0, vF1);
    vB1 = AE_ADDSUB32S(vF0, vF1);

    vF0 = AE_MOVF24X2_FROMINT32X2(vB0);
    vA0 = AE_MULFC24RA(vF0, vC0);

    vA0 = AE_SRAI32(vA0, 1);
    vA1 = AE_SRAI32(vB1, 1);

    vB0 = AE_SUB32S(vA1, vA0);
    vB1 = AE_ADD32S(vA1, vA0);

    vF0 = AE_MOVF24X2_FROMINT32X2(vB0);
    vF1 = AE_MOVF24X2_FROMINT32X2(vB1);

    /*
      z(k      )= real(T0)*real(W1)-imag(T0)*imag(W1);
      z(N+2-k  )=-real(T0)*imag(W1)-imag(T0)*real(W1);
      z(N/2+2-k)= real(T1)*real(W2)+imag(T1)*imag(W2);
      z(N/2+k  )=-real(T1)*imag(W2)+imag(T1)*real(W2);
    */
    vA0 = AE_MULFC24RA(vF0, vW0);
    vA1 = AE_NEG24S(vA0);

    vB1 = AE_MULFC24RA(vF1, vW1);

    vA1 = AE_SEL32_LL(vA1, vR0);
    vB0 = AE_SEL32_HH(vB1, vR0);
    vR0 = AE_SEL32_HL(vA0, vB1);

    vF0 = AE_MOVF24X2_FROMINT32X2(vB0);
    vF1 = AE_MOVF24X2_FROMINT32X2(vA1);
    AE_S32X2F24_I(vF0, p_z1, -8*(int)sizeof(*p_z0));
    AE_S32X2F24_XP(vF1, p_z1, step_back);

    AE_L32X2F24_IP(vF0, p_y0, 8);
    AE_L32X2F24_XP(vF1, p_y1, step_back);
    AE_L32X2F24_IP(vC0, p_cos, 8);
    AE_L32X2F24_IP(vW0, p_w, 8);
    AE_L32X2F24_IP(vW1, p_w, 8);

    vB0 = AE_SUBADD32S(vF0, vF1);
    vB1 = AE_ADDSUB32S(vF0, vF1);

    vF0 = AE_MOVF24X2_FROMINT32X2(vB0);
    vA0 = AE_MULFC24RA(vF0, vC0);

    vA0 = AE_SRAI32(vA0, 1);
    vA1 = AE_SRAI32(vB1, 1);

    vB0 = AE_SUB32S(vA1, vA0);
    vB1 = AE_ADD32S(vA1, vA0);

    vF0 = AE_MOVF24X2_FROMINT32X2(vB0);
    vF1 = AE_MOVF24X2_FROMINT32X2(vB1);

    /*
      z(k      )= real(T0)*real(W1)-imag(T0)*imag(W1);
      z(N+2-k  )=-real(T0)*imag(W1)-imag(T0)*real(W1);
      z(N/2+2-k)= real(T1)*real(W2)+imag(T1)*imag(W2);
      z(N/2+k  )=-real(T1)*imag(W2)+imag(T1)*real(W2);
    */
    vA0 = AE_MULFC24RA(vF0, vW0);
    vA1 = AE_NEG24S(vA0);

    vB1 = AE_MULFC24RA(vF1, vW1);
 
    vA0 = AE_SEL32_HH(vR0, vA0);
    vB0 = AE_SEL32_LL(vR0, vB1);
    vR0 = AE_SEL32_HL(vB1, vA1);

    vF0 = AE_MOVF24X2_FROMINT32X2(vA0);
    vF1 = AE_MOVF24X2_FROMINT32X2(vB0);

    AE_S32X2F24_IP(vF0, p_z0, sizeof(*p_z0));
    AE_S32X2F24_I(vF1, p_z0, 7*sizeof(*p_z0));
  }

  /*** middle sample ***/
  /*
    W1=w(N/4+1);
    T0=Y(N/4+1);
    z(N/4+1  )= real(T0)*real(W1)+imag(T0)*imag(W1);
    z(N+1-N/4)=-real(T0)*imag(W1)+imag(T0)*real(W1);
  */
  AE_L32X2F24_IP(vF0, p_y0, 8);
  AE_L32X2F24_IP(vW0, p_w, 8);

  vB0 = AE_MULFC24RA(vF0, vW0);

  vA0 = AE_SEL32_HH(vB0, vR0);
  vA1 = AE_SEL32_LL(vB0, vR0);

  vF0 = AE_MOVF24X2_FROMINT32X2(vA0);
  vF1 = AE_MOVF24X2_FROMINT32X2(vA1);

  AE_S32X2F24_I(vF0, p_z0, 0);
  AE_S32X2F24_I(vF1, p_z1, 0);
}

static
#ifdef COMPILER_MSVC
        ALIGN(8)
#endif
                  const int32_t ae_twd16[18]
#ifndef COMPILER_MSVC
                                              ALIGN(8)
#endif
                                                        =
{
                              0x7641af00,  -0x30fbc500,
  0x5a827900,  -0x5a827900,   0x30fbc500,  -0x7641af00,
  0x5a827900,  -0x5a827900,   0x00000000,  MIN_INT32,
 -0x5a827900,  -0x5a827900,   0x30fbc500,  -0x7641af00,
 -0x5a827900,  -0x5a827900,  -0x7641af00,   0x30fbc500,
};

// scaled fft16
// NOTE: y is input and output, x - temporary
static void fft16_24x24(int32_t *y, int32_t *x)
{
  ae_f24x2    * p_x0, * p_x1, * restrict p_y0;
  const ae_f24x2 * restrict p_twd;
  ae_int24x2  vA0, vA1, vA2, vA3, vB0, vB1, vB2, vB3;
  ae_f24x2    vF0, vF1, vF2, vF3, vT1, vT2, vT3;
  ae_f32x2    vFR;

  const int   step_right  = (int)sizeof(ae_f24x2);
  const int   step_left   = -step_right;
  const int   step_down   = 4*step_right;
  const int   step_nextc  = step_right - 3*step_down;
  const int   step_hupleft= 7*step_left;
  const int   step_upleft = 15*step_left;

  p_x0  = (ae_f24x2 *)(x);
  p_x1  = (ae_f24x2 *)(x+2*15);
  p_y0  = (ae_f24x2 *)(y);
  p_twd = (const ae_f24x2 *)(ae_twd16);

  /*** reordering ***/
  AE_L32X2F24_XP(vF0, p_y0, step_right);
  AE_L32X2F24_XP(vF1, p_y0, step_right);
  AE_L32X2F24_XP(vF2, p_y0, step_right);
  AE_L32X2F24_XP(vF3, p_y0, step_right);
  vA0 = (vF0);
  vA1 = (vF1);
  vA2 = (vF2);
  vA3 = (vF3);

  vB0 = AE_SELP24_HH(vA0, vA1);
  vB3 = AE_SELP24_LL(vA1, vA0);
  AE_L32X2F24_XP(vF0, p_y0, step_right);
  AE_L32X2F24_XP(vF1, p_y0, step_right);
  vA0 = (vF0);
  vA1 = (vF1);
  vB1 = AE_SELP24_HH(vA2, vA3);
  vB2 = AE_SELP24_LL(vA3, vA2);
  AE_L32X2F24_XP(vF2, p_y0, step_right);
  AE_L32X2F24_XP(vF3, p_y0, step_right);
  vA2 = (vF2);
  vA3 = (vF3);
  vF0 = (vB0);
  vF3 = (vB3);
  vF1 = (vB1);
  vF2 = (vB2);
  AE_S32X2F24_XP(vF0, p_x0, step_right);
  AE_S32X2F24_XP(vF3, p_x1, step_left);
  AE_S32X2F24_XP(vF1, p_x0, step_right);
  AE_S32X2F24_XP(vF2, p_x1, step_left);

  vB0 = AE_SELP24_HH(vA0, vA1);
  vB3 = AE_SELP24_LL(vA1, vA0);
  AE_L32X2F24_XP(vF0, p_y0, step_right);
  AE_L32X2F24_XP(vF1, p_y0, step_right);
  vA0 = (vF0);
  vA1 = (vF1);
  vB1 = AE_SELP24_HH(vA2, vA3);
  vB2 = AE_SELP24_LL(vA3, vA2);
  AE_L32X2F24_XP(vF2, p_y0, step_right);
  AE_L32X2F24_XP(vF3, p_y0, step_right);
  vA2 = (vF2);
  vA3 = (vF3);
  vF0 = (vB0);
  vF3 = (vB3);
  vF1 = (vB1);
  vF2 = (vB2);
  AE_S32X2F24_XP(vF0, p_x0, step_right);
  AE_S32X2F24_XP(vF3, p_x1, step_left);
  AE_S32X2F24_XP(vF1, p_x0, step_right);
  AE_S32X2F24_XP(vF2, p_x1, step_left);

  vB0 = AE_SELP24_HH(vA0, vA1);
  vB3 = AE_SELP24_LL(vA1, vA0);
  AE_L32X2F24_XP(vF0, p_y0, step_right);
  AE_L32X2F24_XP(vF1, p_y0, step_right);
  vA0 = (vF0);
  vA1 = (vF1);
  vB1 = AE_SELP24_HH(vA2, vA3);
  vB2 = AE_SELP24_LL(vA3, vA2);
  AE_L32X2F24_XP(vF2, p_y0, step_right);
  AE_L32X2F24_XP(vF3, p_y0, step_upleft);
  vA2 = (vF2);
  vA3 = (vF3);
  vF0 = (vB0);
  vF3 = (vB3);
  vF1 = (vB1);
  vF2 = (vB2);
  AE_S32X2F24_XP(vF0, p_x0, step_right);
  AE_S32X2F24_XP(vF3, p_x1, step_left);
  AE_S32X2F24_XP(vF1, p_x0, step_right);
  AE_S32X2F24_XP(vF2, p_x1, step_left);

  vB0 = AE_SELP24_HH(vA0, vA1);
  vB3 = AE_SELP24_LL(vA1, vA0);
  vB1 = AE_SELP24_HH(vA2, vA3);
  vB2 = AE_SELP24_LL(vA3, vA2);
  vF0 = (vB0);
  vF3 = (vB3);
  vF1 = (vB1);
  vF2 = (vB2);
  AE_S32X2F24_XP(vF0, p_x0, step_right);
  AE_S32X2F24_XP(vF3, p_x1, step_left);
  AE_S32X2F24_XP(vF1, p_x0, step_hupleft);
  AE_S32X2F24_I(vF2, p_x1, 0);

  /*** first radix4 stage ***/
  /* Elements 0, 4, 8, 12 */
  vF0 = AE_L32X2F24_I(p_x1, (0-8)*(int)sizeof(ae_f24x2));
  vF1 = AE_L32X2F24_I(p_x1, (4-8)*(int)sizeof(ae_f24x2));
  vF2 = AE_L32X2F24_I(p_x1, (8-8)*(int)sizeof(ae_f24x2));
  vF3 = AE_L32X2F24_I(p_x1, (12-8)*(int)sizeof(ae_f24x2));
  vA0 = (vF0);
  vA1 = (vF1);
  vA2 = (vF2);
  vA3 = (vF3);

  vA0 = AE_SRAIP24(vA0, 3);
  vA1 = AE_SRAIP24(vA1, 3);
  vA2 = AE_SRAIP24(vA2, 3);
  vA3 = AE_SRAIP24(vA3, 3);

  vB0 = AE_ADDSP24S(vA0, vA2);
  vB2 = AE_SUBSP24S(vA0, vA2);
  vB1 = AE_ADDSP24S(vA1, vA3);
  vA2 = AE_SUBSP24S(vA3, vA1);
  vB3 = AE_SUBSP24S(vA1, vA3);
  vB3 = AE_SELP24_LH(vA2, vB3);

  vA0 = AE_ADDSP24S(vB0, vB1);
  vA1 = AE_SUBSP24S(vB0, vB1);
  vA2 = AE_ADDSP24S(vB2, vB3);
  vA3 = AE_SUBSP24S(vB2, vB3);

  vB0 = AE_SRAIP24(vA0, 2);
  vB1 = AE_SRAIP24(vA1, 2);
  vB2 = AE_SRAIP24(vA2, 2);
  vB3 = AE_SRAIP24(vA3, 2);

  vF0 = (vB0);
  vF1 = (vB1);
  vF2 = (vB2);
  vF3 = (vB3);

  AE_S32X2F24_I(vF0, p_x1, (0-8)*(int)sizeof(ae_f24x2));
  AE_S32X2F24_I(vF3, p_x1, (4-8)*(int)sizeof(ae_f24x2));
  AE_S32X2F24_I(vF1, p_x1, (8-8)*(int)sizeof(ae_f24x2));
  AE_S32X2F24_I(vF2, p_x1, (12-8)*(int)sizeof(ae_f24x2));

  /* Elements 1, 5, 9, 13 */
  vF0 = AE_L32X2F24_I(p_x1, (1-8)*(int)sizeof(ae_f24x2));
  vF1 = AE_L32X2F24_I(p_x1, (5-8)*(int)sizeof(ae_f24x2));
  vF2 = AE_L32X2F24_I(p_x1, (9-8)*(int)sizeof(ae_f24x2));
  vF3 = AE_L32X2F24_I(p_x1, (13-8)*(int)sizeof(ae_f24x2));
  vA0 = (vF0);
  vA1 = (vF1);
  vA2 = (vF2);
  vA3 = (vF3);

  vA0 = AE_SRAIP24(vA0, 3);
  vA1 = AE_SRAIP24(vA1, 3);
  vA2 = AE_SRAIP24(vA2, 3);
  vA3 = AE_SRAIP24(vA3, 3);

  vB0 = AE_ADDSP24S(vA0, vA2);
  vB2 = AE_SUBSP24S(vA0, vA2);
  vB1 = AE_ADDSP24S(vA1, vA3);
  vA2 = AE_SUBSP24S(vA3, vA1);
  vB3 = AE_SUBSP24S(vA1, vA3);
  vB3 = AE_SELP24_LH(vA2, vB3);

  vA0 = AE_ADDSP24S(vB0, vB1);
  vA1 = AE_SUBSP24S(vB0, vB1);
  vA2 = AE_ADDSP24S(vB2, vB3);
  vA3 = AE_SUBSP24S(vB2, vB3);

  AE_L32X2F24_IP(vT1, p_twd, 8);
  AE_L32X2F24_IP(vT2, p_twd, 8);
  AE_L32X2F24_IP(vT3, p_twd, 8);

  vF3 = (vA3);
  vFR = AE_MULFC24RA(vF3, vT1);
  vA3 = AE_MOVINT24X2_FROMF32X2(vFR);

  vF1 = (vA1);
  vFR = AE_MULFC24RA(vF1, vT2);
  vA1 = AE_MOVINT24X2_FROMF32X2(vFR);

  vF2 = (vA2);
  vFR = AE_MULFC24RA(vF2, vT3);
  vA2 = AE_MOVINT24X2_FROMF32X2(vFR);

  vB0 = AE_SRAIP24(vA0, 2);
  vB1 = AE_SRAIP24(vA1, 2);
  vB2 = AE_SRAIP24(vA2, 2);
  vB3 = AE_SRAIP24(vA3, 2);

  vF0 = (vB0);
  vF1 = (vB1);
  vF2 = (vB2);
  vF3 = (vB3);

  AE_S32X2F24_I(vF0, p_x1, (1-8)*(int)sizeof(ae_f24x2));
  AE_S32X2F24_I(vF3, p_x1, (5-8)*(int)sizeof(ae_f24x2));
  AE_S32X2F24_I(vF1, p_x1, (9-8)*(int)sizeof(ae_f24x2));
  AE_S32X2F24_I(vF2, p_x1, (13-8)*(int)sizeof(ae_f24x2));

  /* Elements 2, 6, 10, 14 */
  vF0 = AE_L32X2F24_I(p_x1, (2-8)*(int)sizeof(ae_f24x2));
  vF1 = AE_L32X2F24_I(p_x1, (6-8)*(int)sizeof(ae_f24x2));
  vF2 = AE_L32X2F24_I(p_x1, (10-8)*(int)sizeof(ae_f24x2));
  vF3 = AE_L32X2F24_I(p_x1, (14-8)*(int)sizeof(ae_f24x2));
  vA0 = (vF0);
  vA1 = (vF1);
  vA2 = (vF2);
  vA3 = (vF3);

  vA0 = AE_SRAIP24(vA0, 3);
  vA1 = AE_SRAIP24(vA1, 3);
  vA2 = AE_SRAIP24(vA2, 3);
  vA3 = AE_SRAIP24(vA3, 3);

  vB0 = AE_ADDSP24S(vA0, vA2);
  vB2 = AE_SUBSP24S(vA0, vA2);
  vB1 = AE_ADDSP24S(vA1, vA3);
  vA2 = AE_SUBSP24S(vA3, vA1);
  vB3 = AE_SUBSP24S(vA1, vA3);
  vB3 = AE_SELP24_LH(vA2, vB3);

  vA0 = AE_ADDSP24S(vB0, vB1);
  vA1 = AE_SUBSP24S(vB0, vB1);
  vA2 = AE_ADDSP24S(vB2, vB3);
  vA3 = AE_SUBSP24S(vB2, vB3);

  AE_L32X2F24_IP(vT1, p_twd, 8);
  AE_L32X2F24_IP(vT2, p_twd, 8);
  AE_L32X2F24_IP(vT3, p_twd, 8);

  vF3 = (vA3);
  vFR = AE_MULFC24RA(vF3, vT1);
  vA3 = AE_MOVINT24X2_FROMF32X2(vFR);

  vF1 = (vA1);
  vFR = AE_MULFC24RA(vF1, vT2);
  vA1 = AE_MOVINT24X2_FROMF32X2(vFR);

  vF2 = (vA2);
  vFR = AE_MULFC24RA(vF2, vT3);
  vA2 = AE_MOVINT24X2_FROMF32X2(vFR);

  vB0 = AE_SRAIP24(vA0, 2);
  vB1 = AE_SRAIP24(vA1, 2);
  vB2 = AE_SRAIP24(vA2, 2);
  vB3 = AE_SRAIP24(vA3, 2);

  vF0 = (vB0);
  vF1 = (vB1);
  vF2 = (vB2);
  vF3 = (vB3);

  AE_S32X2F24_I(vF0, p_x1, (2-8)*(int)sizeof(ae_f24x2));
  AE_S32X2F24_I(vF3, p_x1, (6-8)*(int)sizeof(ae_f24x2));
  AE_S32X2F24_I(vF1, p_x1, (10-8)*(int)sizeof(ae_f24x2));
  AE_S32X2F24_I(vF2, p_x1, (14-8)*(int)sizeof(ae_f24x2));

  /* Elements 3, 7, 11, 15 */
  vF0 = AE_L32X2F24_I(p_x1, (3-8)*(int)sizeof(ae_f24x2));
  vF1 = AE_L32X2F24_I(p_x1, (7-8)*(int)sizeof(ae_f24x2));
  vF2 = AE_L32X2F24_I(p_x1, (11-8)*(int)sizeof(ae_f24x2));
  vF3 = AE_L32X2F24_I(p_x1, (15-8)*(int)sizeof(ae_f24x2));
  vA0 = (vF0);
  vA1 = (vF1);
  vA2 = (vF2);
  vA3 = (vF3);

  vA0 = AE_SRAIP24(vA0, 3);
  vA1 = AE_SRAIP24(vA1, 3);
  vA2 = AE_SRAIP24(vA2, 3);
  vA3 = AE_SRAIP24(vA3, 3);

  vB0 = AE_ADDSP24S(vA0, vA2);
  vB2 = AE_SUBSP24S(vA0, vA2);
  vB1 = AE_ADDSP24S(vA1, vA3);
  vA2 = AE_SUBSP24S(vA3, vA1);
  vB3 = AE_SUBSP24S(vA1, vA3);
  vB3 = AE_SELP24_LH(vA2, vB3);

  vA0 = AE_ADDSP24S(vB0, vB1);
  vA1 = AE_SUBSP24S(vB0, vB1);
  vA2 = AE_ADDSP24S(vB2, vB3);
  vA3 = AE_SUBSP24S(vB2, vB3);

  AE_L32X2F24_IP(vT1, p_twd, 8);
  AE_L32X2F24_IP(vT2, p_twd, 8);
  AE_L32X2F24_IP(vT3, p_twd, 8);

  vF3 = (vA3);
  vFR = AE_MULFC24RA(vF3, vT1);
  vA3 = AE_MOVINT24X2_FROMF32X2(vFR);

  vF1 = (vA1);
  vFR = AE_MULFC24RA(vF1, vT2);
  vA1 = AE_MOVINT24X2_FROMF32X2(vFR);

  vF2 = (vA2);
  vFR = AE_MULFC24RA(vF2, vT3);
  vA2 = AE_MOVINT24X2_FROMF32X2(vFR);

  vB0 = AE_SRAIP24(vA0, 2);
  vB1 = AE_SRAIP24(vA1, 2);
  vB2 = AE_SRAIP24(vA2, 2);
  vB3 = AE_SRAIP24(vA3, 2);

  vF0 = (vB0);
  vF1 = (vB1);
  vF2 = (vB2);
  vF3 = (vB3);

  AE_S32X2F24_I(vF0, p_x1, (3-8)*(int)sizeof(ae_f24x2));
  AE_S32X2F24_I(vF3, p_x1, (7-8)*(int)sizeof(ae_f24x2));
  AE_S32X2F24_I(vF1, p_x1, (11-8)*(int)sizeof(ae_f24x2));
  AE_S32X2F24_I(vF2, p_x1, (15-8)*(int)sizeof(ae_f24x2));

  /*** last radix4 stage ***/
  AE_L32X2F24_XP(vF0, p_x0, step_right);
  AE_L32X2F24_XP(vF1, p_x0, step_right);
  AE_L32X2F24_XP(vF2, p_x0, step_right);
  AE_L32X2F24_XP(vF3, p_x0, step_right);
  vA0 = (vF0);
  vA1 = (vF1);
  vA2 = (vF2);
  vA3 = (vF3);

  vB0 = AE_ADDSP24S(vA0, vA2);
  vB2 = AE_SUBSP24S(vA0, vA2);
  vB1 = AE_ADDSP24S(vA1, vA3);
  vA2 = AE_SUBSP24S(vA3, vA1);
  vB3 = AE_SUBSP24S(vA1, vA3);
  vB3 = AE_SELP24_LH(vA2, vB3);

  vA0 = AE_ADDSP24S(vB0, vB1);
  vA2 = AE_SUBSP24S(vB0, vB1);
  vA1 = AE_SUBSP24S(vB2, vB3);
  vA3 = AE_ADDSP24S(vB2, vB3);

  vF0 = (vA0);
  vF1 = (vA1);
  vF2 = (vA2);
  vF3 = (vA3);
  AE_S32X2F24_XP(vF0, p_y0, step_down);
  AE_S32X2F24_XP(vF1, p_y0, step_down);
  AE_S32X2F24_XP(vF2, p_y0, step_down);
  AE_S32X2F24_XP(vF3, p_y0, step_nextc);

  AE_L32X2F24_XP(vF0, p_x0, step_right);
  AE_L32X2F24_XP(vF1, p_x0, step_right);
  AE_L32X2F24_XP(vF2, p_x0, step_right);
  AE_L32X2F24_XP(vF3, p_x0, step_right);
  vA0 = (vF0);
  vA1 = (vF1);
  vA2 = (vF2);
  vA3 = (vF3);

  vB0 = AE_ADDSP24S(vA0, vA2);
  vB2 = AE_SUBSP24S(vA0, vA2);
  vB1 = AE_ADDSP24S(vA1, vA3);
  vA2 = AE_SUBSP24S(vA3, vA1);
  vB3 = AE_SUBSP24S(vA1, vA3);
  vB3 = AE_SELP24_LH(vA2, vB3);

  vA0 = AE_ADDSP24S(vB0, vB1);
  vA2 = AE_SUBSP24S(vB0, vB1);
  vA1 = AE_SUBSP24S(vB2, vB3);
  vA3 = AE_ADDSP24S(vB2, vB3);

  vF0 = (vA0);
  vF1 = (vA1);
  vF2 = (vA2);
  vF3 = (vA3);
  AE_S32X2F24_XP(vF0, p_y0, step_down);
  AE_S32X2F24_XP(vF1, p_y0, step_down);
  AE_S32X2F24_XP(vF2, p_y0, step_down);
  AE_S32X2F24_XP(vF3, p_y0, step_nextc);

  AE_L32X2F24_XP(vF0, p_x0, step_right);
  AE_L32X2F24_XP(vF1, p_x0, step_right);
  AE_L32X2F24_XP(vF2, p_x0, step_right);
  AE_L32X2F24_XP(vF3, p_x0, step_right);
  vA0 = (vF0);
  vA1 = (vF1);
  vA2 = (vF2);
  vA3 = (vF3);

  vB0 = AE_ADDSP24S(vA0, vA2);
  vB2 = AE_SUBSP24S(vA0, vA2);
  vB1 = AE_ADDSP24S(vA1, vA3);
  vA2 = AE_SUBSP24S(vA3, vA1);
  vB3 = AE_SUBSP24S(vA1, vA3);
  vB3 = AE_SELP24_LH(vA2, vB3);

  vA0 = AE_ADDSP24S(vB0, vB1);
  vA2 = AE_SUBSP24S(vB0, vB1);
  vA1 = AE_SUBSP24S(vB2, vB3);
  vA3 = AE_ADDSP24S(vB2, vB3);

  vF0 = (vA0);
  vF1 = (vA1);
  vF2 = (vA2);
  vF3 = (vA3);
  AE_S32X2F24_XP(vF0, p_y0, step_down);
  AE_S32X2F24_XP(vF1, p_y0, step_down);
  AE_S32X2F24_XP(vF2, p_y0, step_down);
  AE_S32X2F24_XP(vF3, p_y0, step_nextc);

  AE_L32X2F24_XP(vF0, p_x0, step_right);
  AE_L32X2F24_XP(vF1, p_x0, step_right);
  AE_L32X2F24_XP(vF2, p_x0, step_right);
  AE_L32X2F24_XP(vF3, p_x0, step_right);
  vA0 = (vF0);
  vA1 = (vF1);
  vA2 = (vF2);
  vA3 = (vF3);

  vB0 = AE_ADDSP24S(vA0, vA2);
  vB2 = AE_SUBSP24S(vA0, vA2);
  vB1 = AE_ADDSP24S(vA1, vA3);
  vA2 = AE_SUBSP24S(vA3, vA1);
  vB3 = AE_SUBSP24S(vA1, vA3);
  vB3 = AE_SELP24_LH(vA2, vB3);

  vA0 = AE_ADDSP24S(vB0, vB1);
  vA2 = AE_SUBSP24S(vB0, vB1);
  vA1 = AE_SUBSP24S(vB2, vB3);
  vA3 = AE_ADDSP24S(vB2, vB3);

  vF0 = (vA0);
  vF1 = (vA1);
  vF2 = (vA2);
  vF3 = (vA3);
  AE_S32X2F24_XP(vF0, p_y0, step_down);
  AE_S32X2F24_XP(vF1, p_y0, step_down);
  AE_S32X2F24_XP(vF2, p_y0, step_down);
  AE_S32X2F24_I(vF3, p_y0, 0);
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
int dct_24x24( 
              int32_t* y,
              int32_t* x,
              int N,
              int scalingOpt)
{
  NASSERT_ALIGN8(x);
  NASSERT_ALIGN8(y);
  NASSERT(scalingOpt==3);
  NASSERT(N==32);

  fft16_24x24(x, y);    /* fft of half-size with reordering */

  dct_split_32_24x24(y,x);

  return 5;
}
