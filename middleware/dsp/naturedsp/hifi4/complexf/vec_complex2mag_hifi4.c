/* ------------------------------------------------------------------------ */
/* Copyright (c) 2018 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs ("Cadence    */
/* Libraries") are the copyrighted works of Cadence Design Systems Inc.	    */
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
/*          Copyright (C) 2015-2018 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
#include "NatureDSP_Signal_complex.h"
#include "common.h"
#include "common_fpu.h"

#if (HAVE_VFPU==0 && HAVE_FPU==0)
DISCARD_FUN(void ,vec_complex2mag,(float32_t  * restrict y, const complex_float  * restrict x, int N))
#elif (HAVE_VFPU)
/*===========================================================================
  Vector matematics:
  vec_complex2invmag     complex magnitude  
===========================================================================*/
/*-------------------------------------------------------------------------
  Complex magnitude
  Routines compute complex magnitude or its reciprocal

  Precision: 
  f     single precision floating point

  Input:
  x[N]  input complex data
  N     length of vector
  Output:
  y[N]  output data

  Restriction:
  none
-------------------------------------------------------------------------*/
#define sz_i32  (int)sizeof(int32_t)
#define sz_f32  (int)sizeof(float32_t)
void       vec_complex2mag (float32_t  * restrict y, const complex_float  * restrict x, int N)
{
  /*
  *  union ufloat32uint32 R, I, T, X, Z, TMP, U0, U1, T2;
  *  float32_t mnt_re, mnt_im;
  *  T.u = 0x7f800000;
  *  T2.u = 0x00400000;
  *  TMP.u = 0x7f000000;
  *
  *  R.f = fabsf( crealf(x) );
  *  I.f = fabsf( cimagf(x) );
  *
  *  if (isnan(R.f) || isnan(I.f)) return NAN;
  *  if (isinf(R.f) || isinf(I.f)) return T.f;
  *
  *
  *  U0.u = R.u&T.u;
  *  U1.u = I.u&T.u;
  *  X.u = MAX(U0.u, U1.u);
  *  T.u = (T.u - X.u );
  *  T.f = fminf(T.f, TMP.f); 
  *  mnt_re = R.f*T.f;
  *  mnt_im = I.f*T.f;
  *  Z.f = sqrtf(mnt_re*mnt_re + mnt_im*mnt_im);
  *  T.u = 0x00800000;
  *  X.u = (X.u - T.u);
  *  X.f = fmaxf(X.f, T2.f);
  *  Z.f = Z.f*X.f;
  *  return Z.f;
  */
 /* Current block index; overall number of blocks; number of values in the current block */
  int blkIx,blkNum,blkLen, _N;
  /* Block size, blkLen <= blkSize */
  const int blkSize = MAX_ALLOCA_SZ/(2*sz_f32);
  /* Allocate a fixed-size scratch area on the stack. */
  float32_t ALIGN(8) scr[2*blkSize];
  const xtfloatx2 * restrict X = (const xtfloatx2 *)x;
  const xtfloatx2 *          S_rd;
        xtfloatx2 * restrict S_wr;
        xtfloatx2 * restrict Y = (xtfloatx2 *)y;
  int n;
  xtfloatx2 x0, x1, y0, z0, xre, xim;
  xtfloat l;
  ae_int32x2 u0, u1, e0, t0, t1, nsa, nsa0;
  xtbool2  b0;
  ae_valign Y_va;
  if (N <= 0) return;

  NASSERT_ALIGN8(scr);

  /*
  * Data are processed in blocks of scratch area size. Further, the algorithm
  * implementation is splitted in order to feed the optimizing compiler with a
  * few loops of managable size.
  */
  _N = N&(~1);
  blkNum = (_N + blkSize - 1) / blkSize;
  for (blkIx = 0; blkIx<blkNum; blkIx++)
  {
    blkLen = XT_MIN(_N - blkIx*blkSize, blkSize);
    {
      S_wr = (xtfloatx2*)scr;

      X = (xtfloatx2*)((uintptr_t)x + 2 * blkIx * blkSize * sz_f32);
      for (n = 0; n<(blkLen >> 1); n++)
      {
        XT_LSX2IP(x0, X, sizeof(complex_float));
        XT_LSX2IP(x1, X, sizeof(complex_float));
        x0 = XT_ABS_SX2(x0);
        x1 = XT_ABS_SX2(x1);

        xre = XT_SEL32_HH_SX2(x0, x1);
        xim = XT_SEL32_LL_SX2(x0, x1);

        t0 = XT_AE_MOVINT32X2_FROMXTFLOATX2(xre);
        t1 = XT_AE_MOVINT32X2_FROMXTFLOATX2(xim);

        nsa = AE_MAX32(t0, t1);
        nsa = AE_SRLI32(nsa, 23);
        nsa = AE_SUB32(nsa, 127);

        nsa = AE_MIN32(nsa, 127);

        e0 = AE_SUB32(127, nsa);
        b0 = AE_EQ32(e0, 0);
        nsa0 = AE_SLLI32(e0, 23);
        AE_MOVT32X2(nsa0, 0x00400000, b0);
        y0 = XT_AE_MOVXTFLOATX2_FROMINT32X2(nsa0);
        xre = XT_MUL_SX2(xre, y0);
        xim = XT_MUL_SX2(xim, y0);
         
        x0 = XT_MUL_SX2(xre, xre);
        x1 = XT_MUL_SX2(xim, xim);
        x0 = XT_ADD_SX2(x0, x1);
        nsa = AE_ADD32(127, nsa);
        b0 = AE_EQ32(nsa, 0);
        u1 = AE_SLLI32(nsa, 23);
        AE_MOVT32X2(u1, 0x00400000, b0);
        x1 = XT_AE_MOVXTFLOATX2_FROMINT32X2(u1);
        XT_SSX2IP(x0, S_wr, 2 * sz_f32);
        XT_SSX2IP(x1, S_wr, 2 * sz_f32);
      }
    }
    __Pragma("no_reorder");
    {
      S_rd = (xtfloatx2*)scr;
      Y = (xtfloatx2*)((uintptr_t)y + blkIx*blkSize*sz_f32);
      Y_va = AE_ZALIGN64();
      for (n = 0; n<(blkLen >> 1); n++)
      {
        XT_LSX2IP(x0, S_rd, 2*sz_f32);
        XT_LSX2IP(y0, S_rd, 2*sz_f32);
        z0 = XT_SQRT_SX2(x0);
        y0 = XT_MUL_SX2(y0, z0);
        XT_SASX2IP(y0, Y_va, Y);
      }
      XT_SASX2POSFP(Y_va, Y);
    }
  }
  if (N & 1)
  {

    XT_LSX2IP(x0, X, sizeof(complex_float));

    x0 = XT_ABS_SX2(x0);
    xre = XT_SEL32_HH_SX2(x0, x0);
    xim = XT_SEL32_LL_SX2(x0, x0);
    t0 = XT_AE_MOVINT32X2_FROMXTFLOATX2(xre);
    t1 = XT_AE_MOVINT32X2_FROMXTFLOATX2(xim);

    nsa = AE_MAX32(t0, t1);
    nsa = AE_SRLI32(nsa, 23);
    nsa = AE_SUB32(nsa, 127);

    nsa = AE_MIN32(nsa, 127);

    e0 = AE_SUB32(127, nsa);
    b0 = AE_EQ32(e0, 0);
    nsa0 = AE_SLLI32(e0, 23);
    AE_MOVT32X2(nsa0, 0x00400000, b0);
    y0 = XT_AE_MOVXTFLOATX2_FROMINT32X2(nsa0);
    xre = XT_MUL_SX2(xre, y0);
    xim = XT_MUL_SX2(xim, y0);

    x0 = XT_MUL_SX2(xre, xre);
    x1 = XT_MUL_SX2(xim, xim);
    x0 = XT_ADD_SX2(x0, x1);

    z0 = XT_SQRT_SX2(x0);

    u0 = XT_AE_MOVINT32X2_FROMXTFLOATX2(z0);
    e0 = AE_ADD32(127, nsa);
    b0 = AE_EQ32(e0, 0);
    u1 = AE_SLLI32(e0, 23);
    AE_MOVT32X2(u1, 0x00400000, b0);
    y0 = XT_AE_MOVXTFLOATX2_FROMINT32X2(u1);
    z0 = XT_AE_MOVXTFLOATX2_FROMINT32X2(u0);
    y0 = XT_MUL_SX2(y0, z0);
    l = XT_LOW_S(y0);
    XT_SSX(l, (xtfloat *)Y, 0);
  }
} /* vec_complex2mag() */
#else
// for scalar FPU
void       vec_complex2mag (float32_t  * restrict y, const complex_float  * restrict x, int N)
{
    const int blkSize = (MAX_ALLOCA_SZ/(sizeof(xtfloat)))/20;
    xtfloat scr[blkSize];
    int n,NN;
    const xtfloat* restrict pX=(const xtfloat*)x;
    const xtfloat* restrict pZrd;
          xtfloat* restrict pZwr;
    const xtfloat* restrict pYrd=(const xtfloat*)y;
          xtfloat* restrict pYwr=(      xtfloat*)y;
    NN=N;
    while(NN>0)
    {
        __Pragma("no_reorder")
        N=XT_MIN(NN,blkSize);
        pZwr=scr;
        pYwr=(      xtfloat*)y;
        for (n=0; n<N; n++)
        {
            xtfloat x0, x1, y0;
            xtfloat xre, xim;
            int t0, t1, nsa;
            int e0;
            int nsa0;

            XT_LSIP(xre,pX,1*sizeof(xtfloat));
            XT_LSIP(xim,pX,1*sizeof(xtfloat));
            xre = XT_ABS_S(xre);
            xim = XT_ABS_S(xim);
            t0 = XT_RFR(xre);
            t1 = XT_RFR(xim);
            nsa = XT_MAX(t0, t1);
            nsa = ((uint32_t)nsa)>> 23;
            nsa = (nsa-127);
            nsa = XT_MIN(nsa, 127);
            e0 = (127-nsa);
            nsa0 = (e0<<23);
            XT_MOVEQZ(nsa0,0x00400000,e0);
            y0 = XT_WFR(nsa0);

            xre = XT_MUL_S(xre, y0);
            xim = XT_MUL_S(xim, y0);

            x0 = XT_MUL_S(xre, xre);
            x1 = XT_MUL_S(xim, xim);

            x0 = XT_ADD_S(x0, x1);
            XT_SSIP(x0,pYwr,sizeof(xtfloat));

            e0 = (127+nsa);
            nsa0 = (e0<<23);
            XT_MOVEQZ(nsa0, 0x00400000, e0);
            x0 = XT_WFR(nsa0);
            XT_SSIP(x0,pZwr,sizeof(xtfloat));
        }
        __Pragma("no_reorder")
        pZrd=scr;
        pYrd=(const xtfloat*)y;
        pYwr=(      xtfloat*)y;
        for (n=0; n<N; n++)
        {
            xtfloat y0,x0;
            XT_LSIP(y0,pYrd,sizeof(xtfloat));
            y0 = XT_SQRT_S(y0);
            XT_LSIP(x0,pZrd,sizeof(xtfloat));
            y0 = XT_MUL_S(y0, x0);    
            XT_SSIP(y0,pYwr,sizeof(xtfloat));
        }
        NN-=N;
        y+=N;
    }
}
#endif
