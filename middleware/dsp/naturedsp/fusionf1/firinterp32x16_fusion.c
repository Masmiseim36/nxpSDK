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
    Interpolating block real FIR filter, 32x16-bit
    C code optimized for Fusion
*/

/*-------------------------------------------------------------------------
  Interpolating Block Real/Complex FIR Filter
  Computes a real FIR filter (direct-form) with interpolation using IR stored 
  in vector h. The real data input is stored in vector x. The filter output 
  result is stored in vector y. The filter calculates N*D output samples 
  using M*D coefficients and requires last N+M*D-1 samples on the delay line.
  NOTE:
  user application is not responsible for management of delay lines

  Precision: 
  16x16     16-bit real data, 16-bit coefficients, 16-bit real outputs 
  16x16     16-bit complex data, 16-bit coefficients, 16-bit complex outputs
  24x24     24-bit real data, 24-bit coefficients, 24-bit real outputs 
  32x16     32-bit real data, 16-bit coefficients, 32-bit real outputs 
  32x32     32-bit real data, 32-bit coefficients, 32-bit real outputs 
  f         floating point

  Input:
  h[M*D]        filter coefficients; h[0] is to be multiplied with the 
                newest sample,Q31, Q15, floating point
  D             interpolation ratio
  N             length of input sample block
  M             length of subfilter. Total length of filter is M*D
  x[N]          input samples,Q15, Q31 or floating point
  Output:
  y[N*D]        output samples, Q15, Q31 or floating point

  Restrictions:
  x,h,y should not overlap
  x,h - aligned on an 8-bytes boundary
  N   - multiple of 8
  M   - multiple of 4
  D should be >1

  PERFORMANCE NOTE:
  for optimum performance follow rules:
  D   - 2, 3 or 4

-------------------------------------------------------------------------*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common utility and macros declarations. */
#include "common.h"

/* Instance pointer validation number. */
#define MAGIC     0x54352160

/* Interpolator instance structure. */
typedef struct tag_firinterpf_t
{
  int32_t magic;
  int               M; /* Filter length                   */
  int               D; /* Interpolation/decimation factor */
  const int16_t   * h; /* Filter coefficients (M*D)       */
        int32_t   * d; /* Delay line of length (M*D)      */
        int32_t   * p; /* Pointer into the delay line     */

} firinterp32x16_t;

/* Calculate the memory block size for an interpolator with given 
 * attributes. */
size_t firinterp32x16_alloc( int D, int M )
{
  NASSERT( D > 1 && M > 0 );
  NASSERT( M%4==0);
  return M*D*sizeof(int16_t) + M*sizeof(int32_t) + sizeof(firinterp32x16_t) + 7;
} // firinterp32x16_alloc()

/* Initialize the interpolator structure. The delay line is zeroed. */
firinterpf_handle_t firinterp32x16_init( void * objmem, 
                                             int D, int M, 
                                             const int16_t * restrict h )
{
    firinterp32x16_t* firinterp;
    void *           ptr;
    int32_t * restrict pd;
    int16_t * restrict ph;
    int m,d;
    NASSERT( D > 1 && M > 0 );
    NASSERT( M%4==0);
    NASSERT(h);
    NASSERT(objmem);
    NASSERT_ALIGN(h,8);
    /* partition the memblock */
    ptr     = objmem;
    ph = (int16_t*)((((uintptr_t)ptr)+7)&~7);
    pd = (int32_t*)(ph+M*D);
    firinterp=(firinterp32x16_t*)(pd+M);
    firinterp->magic=MAGIC;
    firinterp->M   = M;
    firinterp->D   = D;
    firinterp->h = ph;
    firinterp->d = firinterp->p = pd;
    /* copy/rearrange coefficients and clean up delay line */
    for (m=0; m<M; m++) 
    for (d=0; d<D; d++) 
        ph[d*M+m]=h[m*D+d];
    for (m=0; m<M; m++) pd[m]=0;
    return firinterp;

} // firinterp32x16_init()

/* Put a chunk of input signal into the delay line and compute the filter
 * response. */
void firinterp32x16_process( firinterpf_handle_t _firinterp, 
                             int32_t * restrict      y,
                       const int32_t * restrict      x, int N )
{
    const ae_int32x2* restrict pX;
    const ae_int32x2* restrict pX0;
    const ae_int32x2* restrict pX1;
    const int32_t* restrict pH;
    ae_int32x2* pWr;
    firinterp32x16_t *state;
    int n, m, M, D, j;
    const int16_t* h;
    ae_f32x2* pY;
    ae_valign ay;
    int g_exp;
    int q_frac_int ;
    ae_p24x2s g_frac;
    ae_int32x2 X01,X23,X12,X34,t;
    ae_f16x4 H01,H23;
    ae_valign aX0,aX1;
    ae_f64 q0,q1,q2,q3;

    NASSERT(_firinterp);
    state=(firinterp32x16_t *)_firinterp;
    NASSERT(x);
    NASSERT(y);
    NASSERT(state);
    NASSERT(state->magic==MAGIC);
    NASSERT_ALIGN(state->d,8);
    M = state->M;
    D = state->D;
    if(N<=0) return;
    NASSERT(N>0);
    NASSERT(M>0);
    NASSERT(D>1);
    NASSERT(M%2==0);
    h = state->h;
    /* compute scale factor represented as mantissa and exponent */
    g_exp  = 31 - AE_NSAZ32_L( D );
    q_frac_int =  D << ( 15 + 8 - g_exp );
    g_frac = *(ae_p24s *) &q_frac_int;
    WUR_AE_SAR( g_exp );

    ay=AE_ZALIGN64();
    pWr=(ae_int32x2*)state->p;
    pX =(const ae_int32x2*)x;
    /* setup circular buffer boundaries */
    WUR_AE_CBEGIN0( (uintptr_t)( state->d     ) );
    WUR_AE_CEND0  ( (uintptr_t)( state->d + M ) );
    /* process by 4 samples */
    for (n = 0; n<N; n += 4)
    {
        pH =(int32_t*)h;
        pY =(ae_f32x2*)y;
        for (j = 0; j<D; j++)
        {
            /* filtering loop */
            pX0=(const ae_int32x2*)(pWr);
            pX1=(const ae_int32x2*)(((int32_t*)pWr)+1);
            AE_LA32X2NEG_PC(aX0,pX0);
            AE_LA32X2NEG_PC(aX1,pX1);
            AE_LA32X2_RIC(X12,aX1,pX1);
            AE_LA32X2_RIC(t  ,aX0,pX0);
            X12=AE_L32X2_I (pX,0*sizeof(int32_t));
            X34=AE_L32X2_I (pX,2*sizeof(int32_t));
            X12=(AE_INTSWAP(X12));
            X34=(AE_INTSWAP(X34));
            X01=AE_SEL32_LL(X12,t);
            X23=AE_SEL32_LH(X34,X12);

            AE_L32_IP(t,castxcc(ae_int32,pH),sizeof(int32_t)); H01=AE_MOVF16X4_FROMINT32X2(t);
            q0=q1=AE_ZERO();
            q2=AE_MULZAAFD32X16_H2_L3(X23,H01);
            q3=AE_MULZAAFD32X16_H2_L3(X34,H01);
            __Pragma("loop_count min=1")
            for (m=0; m<M-2; m+=2)
            {
              AE_L32_IP(t, castxcc(ae_int32, pH), sizeof(int32_t)); H23 = AE_MOVF16X4_FROMINT32X2(t);
                AE_MULAAFD32X16_H2_L3(q0,X01,H01);
                AE_MULAAFD32X16_H2_L3(q1,X12,H01);
                AE_MULAAFD32X16_H2_L3(q2,X01,H23);
                AE_MULAAFD32X16_H2_L3(q3,X12,H23);
                H01=H23;
                AE_LA32X2_RIC(X01,aX0,pX0);
                AE_LA32X2_RIC(X12,aX1,pX1);
            }
            AE_MULAAFD32X16_H2_L3(q0,X01,H01);
            AE_MULAAFD32X16_H2_L3(q1,X12,H01);
            /* scale qx by D and save */
            q0 = AE_SLAS64S(AE_MULF48Q32SP16S_L( AE_ROUNDSQ32F48ASYM(q0), g_frac ));
            q1 = AE_SLAS64S(AE_MULF48Q32SP16S_L( AE_ROUNDSQ32F48ASYM(q1), g_frac ));
            q2 = AE_SLAS64S(AE_MULF48Q32SP16S_L( AE_ROUNDSQ32F48ASYM(q2), g_frac ));
            q3 = AE_SLAS64S(AE_MULF48Q32SP16S_L( AE_ROUNDSQ32F48ASYM(q3), g_frac ));
            AE_S32RA64S_XP( q0, castxcc(ae_f32, pY), 4 * D);
            AE_S32RA64S_XP( q1, castxcc(ae_f32, pY), 4*D );
            AE_S32RA64S_XP( q2, castxcc(ae_f32, pY), 4*D );
            AE_S32RA64S_XP( q3, castxcc(ae_f32, pY), (1-3*D)*4 );
        }
        /* update delay line */
        AE_L32X2_IP(X01,pX,8); 
        AE_L32X2_IP(X23,pX,8); 
        AE_S32X2_XC(X01,pWr,sizeof(int32_t)*2);
        AE_S32X2_XC(X23,pWr,sizeof(int32_t)*2);
        state->p =(int32_t*)pWr;
        y+=4*D;
    }
    state->p =(int32_t*)pWr;
} // firinterp32x16_process()
