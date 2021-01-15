/* ------------------------------------------------------------------------ */
/* Copyright (c) 2018 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
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
/*          Copyright (C) 2015-2018 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
/*
  NatureDSP Signal Processing Library. IIR part
    Lattice Block Real IIR, 32x32-bit, M=8
    C code optimized for HiFi4
  IntegrIT, 2006-2018
*/
#include "NatureDSP_types.h"
#include "common.h"
#include "latr32x32_common.h"

/*-------------------------------------------------------------------------
  Lattice Block Real IIR
  Computes a real cascaded lattice autoregressive IIR filter using reflection 
  coefficients stored in vector k. The real data input are stored in vector x.
  The filter output result is stored in vector r.  Input scaling is done before 
  the first cascade for normalization and overflow protection..

  Precision: 
  32x32  32-bit data, 32-bit coefficients
  32x16  32-bit data, 16-bit coefficients

  Input:
  N      length of input sample block
  M      number of reflection coefficients
  scale  input scale factor g, Q31 or Q15
  k[M]   reflection coefficients, Q31 or Q15
  x[N]   input samples, Q31
  Output:
  r[N]   output data, Q31

  Restriction:
  x,r,k should not overlap

  PERFORMANCE NOTE:
  for optimum performance follow rules:
  M - from the range 1...8
-------------------------------------------------------------------------*/

void latr32x32_8_proc(       int32_t * restrict r,     // r[N]     [out   ] Q31
                       const int32_t * restrict x,     // x[N]     [in    ] Q31
                             int32_t * restrict delLine, // dline[M] [in/out] Q30
                       const int32_t *  restrict coef,  // coef[M]  [in    ] Q31
                             int32_t scale, int N, int M )  // scale    [in    ] Q31
{
    const ae_int32x2* restrict pc=(const ae_int32x2*)coef;
    int _scale=scale>>1;
    const ae_int32* restrict px=(const ae_int32*)x;
          ae_int32* restrict pr=(      ae_int32*)r;
    ae_int32x2 s,c01,c12,c23,c34,c45,c67,c56,c_0,xn,rn;
    ae_f32x2 d67,d45,d23,d01,t0;
    ae_f64 q0,q1;

    int n;
    NASSERT(M==8);
    NASSERT_ALIGN8(coef);
    NASSERT_ALIGN8(delLine);
    rn=0;
    c01=AE_L32X2_I(pc,0);
    c23=AE_L32X2_I(pc,8);
    c45=AE_L32X2_I(pc,16);
    c67=AE_L32X2_I(pc,24);
    c_0=AE_SEL32_HH(AE_MOVDA32(0x7fffffff),c01);
    c56=AE_SEL32_LH(c45,c67);
    c34=AE_SEL32_LH(c23,c45);
    c12=AE_SEL32_LH(c01,c23);
    d01=AE_L32X2_I((const ae_int32x2*)delLine,0);
    d23=AE_L32X2_I((const ae_int32x2*)delLine,8);
    d45=AE_L32X2_I((const ae_int32x2*)delLine,16);
    d67=AE_L32X2_I((const ae_int32x2*)delLine,24);
    
    __Pragma("loop_count min=1")
    for ( n=0; n<N; n++ )
    {
        XT_MOVEQZ(_scale,_scale,_scale); // cache s in AR
        s=AE_MOVDA32(_scale);
        AE_L32_IP(xn,px,4);
        q0=AE_MULF32S_LL(xn,s);
        AE_MULSSFD32S_HH_LL(q0,d67,c67);
        q1=q0;
        AE_MULSF32S_LL(q1,d45,c45);
        t0=AE_ROUND32X2F64SASYM(q1,q0);
        d67=AE_SEL32_LH(d45,d67);
        AE_MULAFP32X2RS(d67,t0,c56);

        AE_MULSSFD32S_HH_LL(q0,d45,c45);
        q1=q0;
        AE_MULSF32S_LL(q0,d23,c23);
        t0=AE_ROUND32X2F64SASYM(q0,q1);
        d45=AE_SEL32_LH(d23,d45);
        AE_MULAFP32X2RS(d45,t0,c34);

        AE_L32X2_IP(c01,pc,0);      // dummy reload c01
        AE_MULSSFD32S_HH_LL(q1,d23,c23);
        q0=q1;
        AE_MULSF32S_LL(q1,d01,c01);
        t0=AE_ROUND32X2F64SASYM(q1,q0);
        d23=AE_SEL32_LH(d01,d23);
        AE_MULAFP32X2RS(d23,t0,c12);
        s=AE_XOR32(s,s);    // just 0 !
        AE_MULSF32S_HL(q1,d01,c_0);
        d01=AE_SEL32_HH(s,d01);
        t0=AE_ROUND32X2F64SASYM(q1,q1);
        AE_MULAFP32X2RS(d01,t0,c_0);
        rn=AE_TRUNCI32F64S_L(rn,q1,1);  // Make the output sample.
        AE_S32_L_IP(rn,pr,4);
    }
    AE_S32X2_I(d01,(ae_int32x2*)delLine,0);
    AE_S32X2_I(d23,(ae_int32x2*)delLine,8);
    AE_S32X2_I(d45,(ae_int32x2*)delLine,16);
    AE_S32X2_I(d67,(ae_int32x2*)delLine,24);
}
