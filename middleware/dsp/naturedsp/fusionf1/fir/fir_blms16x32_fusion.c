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
	NatureDSP Signal Processing Library. FIR part
    Blockwise Adaptive LMS Algorithm for Real Data, 16x32-bit
    C code optimized for Fusion
*/

/*-------------------------------------------------------------------------
  Blockwise Adaptive LMS Algorithm for Real Data
  Blockwise LMS algorithm performs filtering of input samples x[N+M-1],
  computation of error e[N] over a block of reference data r[N] and makes
  blockwise update of IR to minimize the error output.
  Algorithm includes FIR filtering, calculation of correlation between the 
  error output e[N] and reference signal x[N+M-1] and IR taps update based
  on that correlation.
NOTES: 
  1. The algorithm must be provided with the normalization factor, which is
     the power of the input signal times N - the number of samples in a
     data block. This can be calculated using the vec_power24x24() or 
     vec_power16x16() function. In order to avoid the saturation of the 
     normalization factor, it may be biased, i.e. shifted to the right.
     If it's the case, then the adaptation coefficient must be also shifted
     to the right by the same number of bit positions.
  2. this algorithm consumes less CPU cycles per block than single 
     sample algorithm at similar convergence rate.
  3. Right selection of N depends on the change rate of impulse response:
     on static or slow varying channels convergence rate depends on
     selected mu and M, but not on N.
  4. 16x16 routine may converge slower on small errors due to roundoff 
     errors. In that cases, 16x32 rountine will give better results although 
     convergence rate on bigger errors is the same

  Precision: 
  16x16   16-bit coefficients, 16-bit data, 16-bit output
  24x24   24-bit coefficients, 24-bit data, 32-bit output
  16x32   32-bit coefficients, 16-bit data, 16-bit output
  32x32   32-bit coefficients, 32-bit data, 32-bit output
  f       floating point
  Input:
  h[M]     impulse response, Q15, Q31 or floating point
  r[N]     reference data vector (near end). First in time value is in 
           r[0], 24-bit or 16-bit, Qx or floating point
  x[N+M-1] input vector (far end). First in time value is in x[0],  
           24-bit or 16-bit, Qx or floating point
  norm     normalization factor: power of signal multiplied by N, Q15, 
           Q31 or floating point
           Fixed-point format for the 24x24-bit variant: Q(2*x-31-bias)
           Fixed-point format for the 32x16-bit variant: Q(2*x+1-bias)
  mu       adaptation coefficient (LMS step), Q(31-bias) or Q(15-bias)
  N        length of data block
  M        length of h
  Output:
  e[N]     estimated error, Q31,Q15 or floating point
  h[M]     updated impulse response, Q15, Q31 or floating point

  Restriction:
  x,r,h,e - should not overlap
  x,r,h,e - aligned on a 8-bytes boundary
  N,M     - multiples of 8 and >0
-------------------------------------------------------------------------*/

#include "NatureDSP_types.h"
#include "NatureDSP_Signal.h"
#include "common.h"
#include "baseop.h"

void fir_blms16x32( int32_t * restrict e,
                    int32_t * restrict h,
              const int16_t * restrict r,
              const int16_t * restrict x,
              int32_t norm, int16_t mu,
              int N, int M )

{
// const int32_t   * restrict pH;

        ae_int32x2   * restrict pe = (      ae_int32x2 *)e;
        ae_int32x2   * restrict ph = (      ae_int32x2 *)h;
  const ae_int16x4   * restrict px = (const ae_int16x4 *)x;
  const ae_int16x4   * restrict pr = (const ae_int16x4 *)r;
  const ae_int16x4   * restrict px1; 
  const ae_int16     * restrict px2; 
  const ae_int32x2   * restrict ph1;  
  ae_valign      x_align;
  int         n, m, s_exp, ind, x_nsa, mu_exp;
  ae_f64      A0, A1, A2, A3 ,A4, A5, A6, A7;
  ae_int64    B0, B1, B2, B3, B4, B5, B6, B7, z1, z2;
  ae_int32x2  zl, za, zb, vxw, y1, vnw, zc, vxa, vxb, y0, vmw, vrw, y2, y3;
  ae_int32x2  r0, r1, r2, r3;
  ae_f32x2    vxf, vaf, vbf, vcf, vdf;
  int32_t     uh;
  ae_int16x4  x0, x1,nnn;
  ae_f16x4    f0, f1, f2;
  ae_f32x2    val, vbl;

  zl = AE_MOVDA32X2(0,1);//1
  zb = AE_MOVDA32X2(0x80000000,0x80000000);
  zc  = AE_MOVDA32X2(0xFFFFFFFC,0xFFFFFFFC);

  vnw = AE_MOVDA32X2(16384, 16384);
  za = AE_MOVDA32X2(0, 0x8000);
  vmw = AE_MOVDA32X2(0xE0000000, 0xE0000000); //-1<<(29)
  z1 = AE_MOVINT64_FROMINT32X2(zl);
  z2 = AE_MOVINT64_FROMINT32X2(za);
  nnn = AE_MOVDA16(0x8000);

  NASSERT(e);
  NASSERT(h);
  NASSERT(r);
  NASSERT(x);
  NASSERT_ALIGN(e,8);
  NASSERT_ALIGN(h,8);
  NASSERT_ALIGN(r,8);
  NASSERT_ALIGN(x,8);
  NASSERT(N>0 && M>0);
  NASSERT(M%8==0 && N%8==0);
  //
  // Pass the reference signal x[] through the adaptive filter to obtain the
  // predicted signal and calculate the error, i.e. the distance to the 
  // actual input signal r[].
  //
#if 1
  ind = (M>>2);
  for ( n=0; n<N; n+=8 )
  {
    ph = (      ae_int32x2 *)h + (M/2)-1;
    px2 = (const ae_int16 *)px + 1;
    x_align = AE_LA64_PP(px2);
    A0 = z2;
    A1 = z2;
    A2 = z2;
    A3 = z2;
    A4 = z2;
    A5 = z2;
    A6 = z2;
    A7 = z2;

    AE_L16X4_IP(x0, px, +8);
    AE_L16X4_IP(x1, px, +8);
    px1 = (const ae_int16x4 *)px;    
        
    for (m=0; m<ind; m++ )
    {
      f0 = x0;
      f1 = x1;
      AE_L32X2_RIP(vxa, ph);
      AE_L32X2_RIP(vxb, ph);
      val = vxa;
      vbl = vxb;
      AE_MULSSFD32X16_H3_L2(A0,val,f0);
      AE_MULSSFD32X16_H1_L0(A2,val,f0);
      AE_MULSSFD32X16_H3_L2(A4,val,f1);
      AE_MULSSFD32X16_H1_L0(A6,val,f1);
      AE_MULSSFD32X16_H1_L0(A0,vbl,f0);
      AE_MULSSFD32X16_H3_L2(A2,vbl,f1);
      AE_MULSSFD32X16_H1_L0(A4,vbl,f1);
      x0 = x1;
      AE_L16X4_IP(x1, px1, +8);
      f1 = x1;
      AE_MULSSFD32X16_H3_L2(A6,vbl,f1);
    }
    ph = (      ae_int32x2 *)h + (M/2)-1;


    AE_LA16X4_IP(x0, x_align,  castxcc(ae_int16x4,px2));
    AE_LA16X4_IP(x1, x_align,  castxcc(ae_int16x4,px2));
    
    for (m=0; m<ind; m++ )
    {
      f0 = x0;
      f1 = x1;
      AE_L32X2_RIP(vxa, ph);
      AE_L32X2_RIP(vxb, ph);
      val = vxa;
      vbl = vxb;
      AE_MULSSFD32X16_H3_L2(A1,val,f0);
      AE_MULSSFD32X16_H1_L0(A3,val,f0);

      AE_MULSSFD32X16_H3_L2(A5,val,f1);
      AE_MULSSFD32X16_H1_L0(A7,val,f1);
      AE_MULSSFD32X16_H1_L0(A1,vbl,f0);
      AE_MULSSFD32X16_H3_L2(A3,vbl,f1);

      AE_MULSSFD32X16_H1_L0(A5,vbl,f1);
      x0 = x1;
      AE_LA16X4_IP(x1, x_align,  castxcc(ae_int16x4,px2));
      f1 = x1;
      AE_MULSSFD32X16_H3_L2(A7,vbl,f1);
    }
   
    B0 = A0;
    B1 = A1;
    B2 = A2;
    B3 = A3;
    B4 = A4;
    B5 = A5;
    B6 = A6;
    B7 = A7;
    y0 = AE_TRUNCA32X2F64S(B0,B1,16);
    y1 = AE_TRUNCA32X2F64S(B2,B3,16);
    y2 = AE_TRUNCA32X2F64S(B4,B5,16);
    y3 = AE_TRUNCA32X2F64S(B6,B7,16);
    AE_L16X4_IP(x0, pr, +8);
    AE_L16X4_IP(x1, pr, +8);
    vaf = y0;
    vbf = y1;
    vcf = y2;
    vdf = y3;
    vxf = zb;
    f0 = x0;
    f1 = x1;
    f2 = nnn;
    AE_MULSF16X4SS(vaf,vbf,f2,f0);
    AE_MULSF16X4SS(vcf,vdf,f2,f1);

    y0 = vaf;
    y1 = vbf;
    y2 = vcf;
    y3 = vdf;

    AE_S32X2_IP(y0, pe, +8);
    AE_S32X2_IP(y1, pe, +8);
    AE_S32X2_IP(y2, pe, +8);
    AE_S32X2_IP(y3, pe, +8);
  }
#else
  for ( n=0; n<N; n+=4 )
  {
        int64_t a[4];
        int32_t t[4];
        int32_t h01[2],h23[2];
        int16_t x01[2],x12[2],x23[2],x34[2];

        pH = (const int32_t*) (h+M-2);
        a[0]=a[1]=a[2]=a[3]=0;
        for ( m=0; m<M; m+=2)
        {
            x01[0]=x[n+m+0];x01[1]=x[n+m+1];
            x12[0]=x[n+m+1];x12[1]=x[n+m+2];
            x23[0]=x[n+m+2];x23[1]=x[n+m+3];
            x34[0]=x[n+m+3];x34[1]=x[n+m+4];
            h01[0]=h[M-1-m];h01[1]=h[M-1-m-1];

            a[0] += mpy_Q31Q15( h01[0],x01[0]  )+mpy_Q31Q15( h01[1],x01[1]  );
            a[1] += mpy_Q31Q15( h01[0],x12[0]  )+mpy_Q31Q15( h01[1],x12[1]  );
            a[2] += mpy_Q31Q15( h01[0],x23[0]  )+mpy_Q31Q15( h01[1],x23[1]  );
            a[3] += mpy_Q31Q15( h01[0],x34[0]  )+mpy_Q31Q15( h01[1],x34[1]  );
        }
        t[0] = ( satQ31( ( -a[0] + (1LL<<15) ) >> 16 ) );
        t[1] = ( satQ31( ( -a[1] + (1LL<<15) ) >> 16 ) );
        t[2] = ( satQ31( ( -a[2] + (1LL<<15) ) >> 16 ) );
        t[3] = ( satQ31( ( -a[3] + (1LL<<15) ) >> 16 ) );
        e[n+0] = L_sub_ll( t[0], L_shl_l( -(int32_t)r[n+0], 16 ) );
        e[n+1] = L_sub_ll( t[1], L_shl_l( -(int32_t)r[n+1], 16 ) );
        e[n+2] = L_sub_ll( t[2], L_shl_l( -(int32_t)r[n+2], 16 ) );
        e[n+3] = L_sub_ll( t[3], L_shl_l( -(int32_t)r[n+3], 16 ) );
  }
#endif
  //
  // Compute the reciprocal for the normalization factor.
  //
  vxw = AE_MOVDA32X2(norm,norm);
  s_exp = AE_NSAZ32_L(vxw);
  vxw = AE_SLAA32(vxw, s_exp);
  /* reciprocal of Q31 in Q30: 6 LSB accuracy */
  {
        ae_int32x2 y,e,x,_1Q30;
        ae_f64 q;
        x=vxw;
        _1Q30=AE_MOVDA32((int)0x40000000);
        y=AE_SUB32 (_1Q30,x);/* no saturation here!!! */
        y=AE_ADD32 (_1Q30,y);
        y=AE_INT32X2_ADD32S(_1Q30,y);

        q=AE_MULF32S_HH(x,y);          
        e=AE_SUB32S(_1Q30,AE_ROUND32X2F64SASYM(q,q));
        q=AE_MULF32S_HH(e,y);  e=AE_ROUND32X2F64SASYM(q,q);
        y=AE_ADD32 (y,e);
        y=AE_ADD32 (y,e);

        q=AE_MULF32S_HH(x,y);          
        e=AE_SUB32S(_1Q30,AE_ROUND32X2F64SASYM(q,q));
        q=AE_MULF32S_HH(e,y);  e=AE_ROUND32X2F64SASYM(q,q);
        y=AE_ADD32 (y,e);
        y=AE_ADD32 (y,e);

        q=AE_MULF32S_HH(x,y);          
        e=AE_SUB32S(_1Q30,AE_ROUND32X2F64SASYM(q,q));
        q=AE_MULF32S_HH(e,y);  e=AE_ROUND32X2F64SASYM(q,q);
        y=AE_ADD32 (y,e);
        y=AE_ADD32 (y,e);

        q=AE_MULF32S_HH(x,y);          
        e=AE_SUB32S(_1Q30,AE_ROUND32X2F64SASYM(q,q));
        q=AE_MULF32S_HH(e,y);  e=AE_ROUND32X2F64SASYM(q,q);
        y=AE_ADD32 (y,e);
        y=AE_ADD32 (y,e);
        vrw=y;
  }
  vxa = AE_SEL32_LL(vrw, vrw);
  x_nsa = AE_NSAZ32_L(vxa);
  vxa = AE_SLAA32(vxa, x_nsa);
/*-------------------------------------------------*/

  // Q(30-s_exp) <- Q61 / Q(31+s_exp)

  uh = (int16_t) mu;
  x0 = AE_MOVDA16(uh);
  mu_exp = AE_NSAZ16_0(x0);
  vxw = AE_SEXT32X2D16_32(x0);
  vxw = AE_SLAA32S(vxw, mu_exp);
  B0 = AE_MUL32_LL(vxw,vxa);
  vxw = AE_TRUNCA32F64S(B0,17);

  s_exp -= mu_exp; // -15..30
  //
  // Calculate the cross-correlation between the error signal and the 
  // reference signal. Scale the result and update the estimation of the
  // impulse response.
  //
  px = (const ae_int16x4 *)x;
  z1 = AE_SLAA64(z1,(30-s_exp));
  ph = (      ae_int32x2 *)h + (M/2)-1;
  ind = (N>>2);
  for ( m=0; m<M; m+=8 )
  {
    pe = (      ae_int32x2 *)e ;
    A0 = z2;
    A1 = z2;
    A2 = z2;
    A3 = z2;
    A4 = z2;
    A5 = z2;
    A6 = z2;
    A7 = z2;
    px2 = (const ae_int16 *)px + 1;
    x_align = AE_LA64_PP(px2);
    AE_L16X4_IP(x0, px, +8);
    AE_L16X4_IP(x1, px, +8);
    px1 = (const ae_int16x4 *)px;
    ph1 = (ae_int32x2 *) ph;
    
    for ( n=0; n<ind; n++ )
    {
      f0 = x0;
      f1 = x1;
      AE_L32X2_IP(vxa, pe, +8);
      AE_L32X2_IP(vxb, pe, +8);
      val = vxa;
      vbl = vxb;
      AE_MULAAFD32X16_H3_L2(A0,val,f0);
      AE_MULAAFD32X16_H1_L0(A2,val,f0);
      AE_MULAAFD32X16_H3_L2(A4,val,f1);
      AE_MULAAFD32X16_H1_L0(A6,val,f1);
      AE_MULAAFD32X16_H1_L0(A0,vbl,f0);
      AE_MULAAFD32X16_H3_L2(A2,vbl,f1);
      AE_MULAAFD32X16_H1_L0(A4,vbl,f1);
      x0 = x1;
      AE_L16X4_IP(x1, px1, +8);
      f1 = x1;
      AE_MULAAFD32X16_H3_L2(A6,vbl,f1);
    }
    pe = (      ae_int32x2 *)e ;
    AE_LA16X4_IP(x0, x_align, castxcc(ae_int16x4, px2));
    AE_LA16X4_IP(x1, x_align, castxcc(ae_int16x4, px2));
    f0 = x0;
    f1 = x1;

    for (n=0; n<ind; n++ )
    {
      AE_L32X2_IP(vxa, pe, +8);
      AE_L32X2_IP(vxb, pe, +8);
      val = vxa;
      vbl = vxb;
      AE_MULAAFD32X16_H3_L2(A1,val,f0);
      AE_MULAAFD32X16_H1_L0(A3,val,f0);

      AE_MULAAFD32X16_H3_L2(A5,val,f1);
      AE_MULAAFD32X16_H1_L0(A7,val,f1);
      AE_MULAAFD32X16_H1_L0(A1,vbl,f0);
      AE_MULAAFD32X16_H3_L2(A3,vbl,f1);

      AE_MULAAFD32X16_H1_L0(A5,vbl,f1);
      f0 = f1;
      AE_LA16X4_IP(x1, x_align, castxcc(ae_int16x4,px2));
      f1 = x1;
      AE_MULAAFD32X16_H3_L2(A7,vbl,f1);
    }
    B0 = A0;
    B1 = A1;
    B2 = A2;
    B3 = A3;
    B4 = A4;
    B5 = A5;
    B6 = A6;
    B7 = A7;
    y0 = AE_TRUNCA32X2F64S(B0,B1,16);
    y1 = AE_TRUNCA32X2F64S(B2,B3,16);
    y2 = AE_TRUNCA32X2F64S(B4,B5,16);
    y3 = AE_TRUNCA32X2F64S(B6,B7,16);

    vaf = y0;
    vbf = y1;
    vcf = y2;
    vdf = y3;
    vxf = vxw;
    A0 = z1;
    A1 = z1;
    A2 = z1;
    A3 = z1;
    A4 = z1;
    A5 = z1;
    A6 = z1;
    A7 = z1;
    AE_MULAF32S_HH(A0,vxf,vaf);
    AE_MULAF32S_LL(A1,vxf,vaf);
    AE_MULAF32S_HH(A2,vxf,vbf);
    AE_MULAF32S_LL(A3,vxf,vbf);
    AE_MULAF32S_HH(A4,vxf,vcf);
    AE_MULAF32S_LL(A5,vxf,vcf);
    AE_MULAF32S_HH(A6,vxf,vdf);
    AE_MULAF32S_LL(A7,vxf,vdf);
    B0 = A0;
    B1 = A1;
    B2 = A2;
    B3 = A3;
    B4 = A4;
    B5 = A5;
    B6 = A6;
    B7 = A7;

    B0 = AE_SRAA64(B0, 31 - s_exp);
    B1 = AE_SRAA64(B1, 31 - s_exp);
    B2 = AE_SRAA64(B2, 31 - s_exp);
    B3 = AE_SRAA64(B3, 31 - s_exp);
    B4 = AE_SRAA64(B4, 31 - s_exp);
    B5 = AE_SRAA64(B5, 31 - s_exp);
    B6 = AE_SRAA64(B6, 31 - s_exp);
    B7 = AE_SRAA64(B7, 31 - s_exp);
    y0 = AE_TRUNCA32X2F64S(B0,B1,32);
    y1 = AE_TRUNCA32X2F64S(B2,B3,32);
    y2 = AE_TRUNCA32X2F64S(B4,B5,32);
    y3 = AE_TRUNCA32X2F64S(B6,B7,32);
    
    AE_L32X2_RIP(r0, ph1);
    AE_L32X2_RIP(r1, ph1);
    AE_L32X2_RIP(r2, ph1);
    AE_L32X2_RIP(r3, ph1);
    y0 = AE_ADD32S(r0,y0);
    y1 = AE_ADD32S(r1,y1);
    y2 = AE_ADD32S(r2,y2);
    y3 = AE_ADD32S(r3,y3);

    AE_S32X2_RIP(y0,ph); 
    AE_S32X2_RIP(y1,ph);
    AE_S32X2_RIP(y2,ph);
    AE_S32X2_RIP(y3,ph);

  }
} /* fir_blms16x32() */

