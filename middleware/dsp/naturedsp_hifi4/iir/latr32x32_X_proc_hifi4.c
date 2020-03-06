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
    Lattice Block Real IIR, 32x32-bit, M>8
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
void latr32x32_X_proc(       int32_t * restrict r,     // r[N]     [out   ] Q31
                       const int32_t * restrict x,     // x[N]     [in    ] Q31
                             int32_t * restrict delLine, // dline[M] [in/out] Q30
                       const int32_t *  restrict coef,  // coef[M]  [in    ] Q31
                             int32_t scale, int N, int M )  // scale    [in    ] Q31
{
    const ae_int32* restrict px=(const ae_int32*)x;
          ae_int32* restrict pr=(      ae_int32*)r;
    const ae_int32* restrict pc;
    const ae_int32* restrict pdr;
          ae_int32* restrict pdw;
    ae_int32x2 t,xn,rn,s,c;
    ae_f32x2 d,_t0;
    ae_f64 q0;
    int m,n;

    NASSERT(M>8);
    NASSERT_ALIGN8(coef);
    NASSERT_ALIGN8(delLine);
    rn=0;
    pc =(const ae_int32*)&coef[M-1];
    pdr=(const ae_int32*)&delLine[M-1];
    pdw=(      ae_int32*)&delLine[M-1];
    s=AE_MOVDA32(scale>>1);
    __Pragma("loop_count min=1")
    for ( n=0; n<N; n++ )
    {
        AE_L32_IP(xn,px,4);
        q0=AE_MULF32S_LL(xn,s);
        AE_L32_IP(t,pdr,-4);d=t;
        AE_L32_IP(c,pc ,-4);
        AE_MULSF32S_HH(q0,d,c);
        __Pragma("loop_count min=7")
        for ( m=M-1; m>0; m-- )
        {
            AE_L32_IP(t,pdr,-4);d=t;
            AE_L32_IP(c,pc ,-4);
            AE_MULSF32S_HH(q0,d,c);
            _t0=AE_ROUND32X2F64SASYM(q0,q0);
            AE_MULAFP32X2RS(d,_t0,c);
            AE_S32_L_IP(d,pdw,-4);
        }
        _t0=AE_ROUND32X2F64SASYM(q0,q0);
        AE_S32_L_XP(_t0,pdw,-4+M*4);
        pc+=M;
        pdr+=M;
        rn=AE_TRUNCI32F64S_L(rn,q0,1);  // Make the output sample.
        AE_S32_L_IP(rn,pr,4);
    }
}
