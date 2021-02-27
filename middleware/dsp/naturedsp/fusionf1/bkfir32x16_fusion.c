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
    Real block FIR filter, 32x16-bit
    C code optimized for Fusion
*/

/*-------------------------------------------------------------------------
  Real FIR filter.
  Computes a real FIR filter (direct-form) using IR stored in vector h. The 
  real data input is stored in vector x. The filter output result is stored 
  in vector y. The filter calculates N output samples using M coefficients 
  and requires last M-1 samples in the delay line.
  NOTE: 
   user application is not responsible for management of delay lines


  Precision: 
  16x16  16-bit data, 16-bit coefficients, 16-bit outputs
  24x24  24-bit data, 24-bit coefficients, 24-bit outputs
  24x24p use 24-bit data packing for internal delay line buffer
         and internal coefficients storage
  32x16    32-bit data, 16-bit coefficients, 32-bit outputs
  32x32    32-bit data, 32-bit coefficients, 32-bit outputs
  f        floating point

  Input:
  x[N]      - input samples, Q31, Q15, floating point
  h[M]      - filter coefficients in normal order, Q31, Q15, floating point
  N         - length of sample block, should be a multiple of 4
  M         - length of filter, should be a multiple of 4
  Output:
  y[N]      - input samples, Q31, Q15, floating point

  Restrictions:
  x,y should not be overlapping
  x,h - aligned on a 8-bytes boundary
  N,M - multiples of 4 
-------------------------------------------------------------------------*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common utility and macros declarations. */
#include "common.h"

/* Instance pointer validation number. */
#define MAGIC     0xc59d803e

/* Filter instance structure. */
typedef struct tag_bkfir32x16_t
{
  int32_t magic;
  int             M; /* Filter length                   */
  const int16_t * h; /* Filter coefficients             */
        int32_t * d; /* Delay line of length M          */
        int32_t * p; /* Pointer into the delay line     */
} bkfir32x16_t, *bkfir32x16_ptr_t;

/* Calculate the memory block size for an FIR filter with given attributes. */
size_t bkfir32x16_alloc( int M )
{
  NASSERT( M > 0 );
  NASSERT(M%4==0);
  return 3*M*sizeof(int32_t) + sizeof(bkfir32x16_t) + 7;
} // bkfir32x16_alloc()

/* Initialize the filter structure. The delay line is zeroed. */
bkfirf_handle_t bkfir32x16_init( void * objmem, int M, const int16_t * h )
{
  bkfir32x16_t* bkfir;
  void *           ptr;
  int32_t * restrict pd;
  int16_t * restrict ph;
  int m;
  NASSERT( objmem && M > 0 && h );
  NASSERT(M%4==0);
  NASSERT_ALIGN(h,8);
  /* Partition the memory block */
  ptr     = objmem;
  ph = (int16_t*)((((uintptr_t)ptr)+7)&~7);
  pd = (int32_t*)(ph+M);
  bkfir=(bkfir32x16_t*)(pd+M);
  bkfir->magic=MAGIC;
  bkfir->M   = M;
  bkfir->h = ph;
  bkfir->d = bkfir->p = pd;
  /* copy coefficients and clean upd delay line */
  for (m=0; m<M; m++) ph[m]=h[m];
  for (m=0; m<M; m++) pd[m]=0;
  return bkfir;
} // bkfir32x16_init()

/* process block of samples */
void bkfir32x16_process( bkfir32x16_handle_t _bkfir, 
                         int32_t * restrict  y,
                   const int32_t * restrict  x, int N )
{
    const int32_t* restrict pH;
    const ae_int32x2* restrict pX;
          ae_int32x2* restrict pY;
    const ae_int32x2  * pD01;
    const ae_int32x2  * pD12;
    ae_int32x2* p;
    ae_valign  ay;
    int n,m,M;
    const int16_t* h;
    bkfir32x16_t* bkfir;
    NASSERT(_bkfir);
    bkfir=(bkfir32x16_t*)_bkfir;
    NASSERT(bkfir->magic==MAGIC);
    NASSERT(bkfir->h);
    NASSERT(bkfir->d);
    NASSERT(bkfir->p);
    NASSERT_ALIGN(bkfir->h,8);
    NASSERT_ALIGN(bkfir->d,8);
    NASSERT_ALIGN(bkfir->p,8);
    NASSERT(N%4==0);
    NASSERT_ALIGN(x,8);
    NASSERT((bkfir->M%4)==0);
    NASSERT(x);
    NASSERT(y);
    if(N<=0) return;
    M=bkfir->M;
    NASSERT(N>0);
    NASSERT(M>0);
    h=bkfir->h;
    pX=(const ae_int32x2*)x;
    WUR_AE_CBEGIN0( (uintptr_t)( bkfir->d            ) );
    WUR_AE_CEND0  ( (uintptr_t)( bkfir->d + bkfir->M ) );
    ay=AE_ZALIGN64();
    pY=(ae_int32x2*)y;
    p=(ae_int32x2*)(bkfir->p);

    for (n=0; n<N; n+=4)
    {
        ae_f64       q0, q1, q2, q3;
        ae_int32x2   t,X01,X12,X23,X34,XN0,XN1;
        ae_f16x4     H01,H23;
        ae_valign    ad01,ad12;

        pH=(const int32_t* )h;
        pD01=(const ae_int32x2*)(p);
        pD12=(const ae_int32x2*)(((int32_t*)p)+1);
        AE_LA32X2NEG_PC(ad01,pD01);
        AE_LA32X2NEG_PC(ad12,pD12);
        AE_LA32X2_RIC(X12,ad12,pD12);
        AE_LA32X2_RIC(t,ad01,pD01);
        XN0=AE_L32X2_I (pX,0*sizeof(int32_t));
        XN1=AE_L32X2_I (pX,2*sizeof(int32_t));
        X12=(AE_INTSWAP(XN0));
        X34=(AE_INTSWAP(XN1));
        X01=AE_SEL32_HL(XN0,t);
        X23=AE_SEL32_HL(XN1,XN0);

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
            AE_LA32X2_RIC(X01,ad01,pD01);
            AE_LA32X2_RIC(X12,ad12,pD12);
        }
        AE_MULAAFD32X16_H2_L3(q0,X01,H01);
        AE_MULAAFD32X16_H2_L3(q1,X12,H01);

        AE_L32X2_IP(XN0,pX,2*sizeof(int32_t));
        AE_L32X2_IP(XN1,pX,2*sizeof(int32_t));
        AE_S32X2_XC(XN0,p,8);
        AE_S32X2_XC(XN1,p,8);
        AE_S32RA64S_IP(q0, castxcc(ae_f32, pY), 4);
        AE_S32RA64S_IP(q1, castxcc(ae_f32, pY), 4);
        AE_S32RA64S_IP(q2, castxcc(ae_f32, pY), 4);
        AE_S32RA64S_IP(q3, castxcc(ae_f32, pY), 4);

    }
    AE_SA64POS_FP(ay,pY);
    bkfir->p=(int32_t*)p;
} /* bkfir32x16_process */
