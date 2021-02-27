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
    Real block FIR filter, 24x24-bit, unaligned data and arbitrary M/N allowed
    C code optimized for Fusion
*/

/*-------------------------------------------------------------------------
  Real FIR filter.
  Computes a real FIR filter (direct-form) using IR stored in vector h. The 
  real data input is stored in vector x. The filter output result is stored 
  in vector y. The filter calculates N output samples using M coefficients 
  and requires last M-1 samples in the delay line.
  These functions implement FIR filter described in previous chapter with no 
  limitation on size of data block, alignment and length of impulse response 
  for the cost of performance.
  NOTE: 
  User application is not responsible for management of delay lines

  Precision: 
  16x16    16-bit data, 16-bit coefficients, 16-bit outputs
  24x24    24-bit data, 24-bit coefficients, 24-bit outputs
  32x16    32-bit data, 16-bit coefficients, 32-bit outputs
  f        floating point
  Input:
  x[N]      - input samples, Q31, floating point
  h[M]      - filter coefficients in normal order, Q31, Q15, floating point
  N         - length of sample block
  M         - length of filter
  Output:
  y[N]      - input samples, Q31, floating point 

  Restrictions:
  x,y should not be overlapping
-------------------------------------------------------------------------*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common utility and macros declarations. */
#include "common.h"

/* Instance pointer validation number. */
#define MAGIC     0xe96e9f66

/* Filter instance structure. */
typedef struct tag_bkfira24x24_t
{
  int32_t magic;
  int             M; /* Filter length                   */
  const int32_t * h; /* Filter coefficients             */
        int32_t * d; /* Delay line of length M          */
        int32_t * p; /* Pointer into the delay line     */
} bkfira24x24_t, *bkfira24x24_ptr_t;

/* Calculate the memory block size for an FIR filter with given attributes. */
size_t bkfira24x24_alloc( int M )
{
  NASSERT( M > 0 );
  M=(M+3)&~3;
  return 2*M*sizeof(int32_t) + sizeof(bkfira24x24_t) + 7;
} // bkfira24x24_alloc()

/* Initialize the filter structure. The delay line is zeroed. */
bkfirf_handle_t bkfira24x24_init( void * objmem, int M, const f24 * h )
{
  bkfira24x24_t* bkfir;
  void *           ptr;
  int32_t * restrict pd;
  int32_t * restrict ph;
  int m,M1;
  NASSERT( objmem && M > 0 && h );
  M1=(M+3)&~3;
  /* Partition the memory block */
  ptr     = objmem;
  ph = (int32_t*)((((uintptr_t)ptr)+7)&~7);
  pd = ph+M1;
  bkfir=(bkfira24x24_t*)(pd+M1);
  bkfir->magic=MAGIC;
  bkfir->M   = M1;
  bkfir->h = ph;
  bkfir->d = bkfir->p = pd;
  /* copy coefficients and clean upd delay line */
  for (m=0; m<M; m++) ph[m]=h[m];
  for (; m<M1; m++) ph[m]=0;
  for (m=0; m<M1; m++) pd[m]=0;
  return bkfir;
} // bkfira24x24_init()

/* process block of samples */
void bkfira24x24_process( bkfira24x24_handle_t _bkfir, 
                         f24 * restrict  y,
                   const f24 * restrict  x, int N )
{
    const ae_f24x2* restrict pH;
    const ae_f24x2* restrict pX;
          ae_f24x2* restrict pY;
    const ae_f24x2  * pD01;
    const ae_f24x2  * pD12;
    ae_f24x2* p;
    ae_valign  ay,ax;
    int n,m,M;
    const int32_t* h;
    bkfira24x24_t* bkfir;
    NASSERT(_bkfir);
    bkfir=(bkfira24x24_t*)_bkfir;
    NASSERT(bkfir->magic==MAGIC);
    NASSERT(bkfir->h);
    NASSERT(bkfir->d);
    NASSERT(bkfir->p);
    NASSERT_ALIGN(bkfir->h,8);
    NASSERT_ALIGN(bkfir->d,8);
    NASSERT((bkfir->M%4)==0);
    NASSERT(x);
    NASSERT(y);
    if(N<=0) return;
    M=bkfir->M;
    NASSERT(N>0);
    NASSERT(M>0);
    h=bkfir->h;
    pX=(const ae_f24x2*)x;
    WUR_AE_CBEGIN0( (uintptr_t)( bkfir->d            ) );
    WUR_AE_CEND0  ( (uintptr_t)( bkfir->d + bkfir->M ) );
    ay=AE_ZALIGN64();
    pY=(ae_f24x2*)y;
    p=(ae_f24x2*)(bkfir->p);
    ax=AE_LA64_PP(pX);

    for (n=0; n<(N&~3); n+=4)
    {
        ae_f64     q0, q1, q2, q3;
        ae_f24x2   t,H01,H23,X01,X12,X23,X34,XN0,XN1,d0;
        ae_valign  ad01,ad12;

        pH=(ae_f24x2* )h;
        pD01=(const ae_f24x2*)(p);
        pD12=(const ae_f24x2*)(((int32_t*)p)+1);
        AE_LA32X2F24NEG_PC(ad01,pD01);
        AE_LA32X2F24NEG_PC(ad12,pD12);
        AE_LA32X2F24_RIC(X12,ad12,pD12);
        AE_LA32X2F24_RIC(t,ad01,pD01);
        AE_LA32X2F24_IP(XN0,ax,pX);
        AE_LA32X2F24_IP(XN1,ax,pX);
        X12=AE_MOVF24X2_FROMINT32X2(AE_INTSWAP(XN0));
        X34=AE_MOVF24X2_FROMINT32X2(AE_INTSWAP(XN1));
        X01=AE_SEL24_HL(XN0,t);
        X23=AE_SEL24_HL(XN1,XN0);

        AE_L32X2F24_IP(H01,pH,2*sizeof(int32_t));
        q0=q1=AE_ZERO();
        q2=AE_MULZAAFD24_HH_LL(H01,X23);
        q3=AE_MULZAAFD24_HH_LL(H01,X34);
        __Pragma("loop_count min=1")
        for (m=0; m<M-2; m+=2)
        {
            AE_L32X2F24_IP(H23,pH,2*sizeof(int32_t));
            AE_MULAAFD24_HH_LL(q0,H01,X01);
            AE_MULAAFD24_HH_LL(q1,H01,X12);
            AE_MULAAFD24_HH_LL(q2,H23,X01);
            AE_MULAAFD24_HH_LL(q3,H23,X12);
            H01=H23;
            AE_LA32X2F24_RIC(X01,ad01,pD01);
            AE_LA32X2F24_RIC(X12,ad12,pD12);
        }
        AE_MULAAFD24_HH_LL(q0,H01,X01);
        AE_MULAAFD24_HH_LL(q1,H01,X12);

        AE_S32F24_L_XC(AE_MOVF24X2_FROMINT32X2(AE_INTSWAP(XN0)), castxcc(ae_f24, p), 4); AE_S32F24_L_XC(XN0, castxcc(ae_f24, p), 4);
        AE_S32F24_L_XC(AE_MOVF24X2_FROMINT32X2(AE_INTSWAP(XN1)), castxcc(ae_f24, p), 4); AE_S32F24_L_XC(XN1, castxcc(ae_f24, p), 4);

        d0 = AE_ROUND24X2F48SASYM( q0, q1 );
        AE_SA32X2F24_IP( d0, ay, pY );
        d0 = AE_ROUND24X2F48SASYM( q2, q3 );
        AE_SA32X2F24_IP( d0, ay, pY );
    }
    AE_SA64POS_FP(ay,pY);
    /* process tail (1...3 samples) */
    N&=3;
    if(N)
    {
        ae_f64     q0, q1, q2, q3;
        ae_f24x2   t,H01,H23,X01,X12,X23,X34,XN0,XN1,d0;
        ae_valign  ad01,ad12;

        pH=(ae_f24x2* )h;
        pD01=(const ae_f24x2*)(p);
        pD12=(const ae_f24x2*)(((int32_t*)p)+1);
        AE_LA32X2F24NEG_PC(ad01,pD01);
        AE_LA32X2F24NEG_PC(ad12,pD12);
        AE_LA32X2F24_RIC(X12,ad12,pD12);
        AE_LA32X2F24_RIC(t,ad01,pD01);
        AE_LA32X2F24_IP(XN0,ax,pX);
        AE_LA32X2F24_IP(XN1,ax,pX);
        X12=AE_MOVF24X2_FROMINT32X2(AE_INTSWAP(XN0));
        X34=AE_MOVF24X2_FROMINT32X2(AE_INTSWAP(XN1));
        X01=AE_SEL24_HL(XN0,t);
        X23=AE_SEL24_HL(XN1,XN0);

        AE_L32X2F24_IP(H01,pH,2*sizeof(int32_t));
        q0=q1=AE_ZERO();
        q2=AE_MULZAAFD24_HH_LL(H01,X23);
        q3=AE_MULZAAFD24_HH_LL(H01,X34);
        __Pragma("loop_count min=1")
        for (m=0; m<M-2; m+=2)
        {
            AE_L32X2F24_IP(H23,pH,2*sizeof(int32_t));
            AE_MULAAFD24_HH_LL(q0,H01,X01);
            AE_MULAAFD24_HH_LL(q1,H01,X12);
            AE_MULAAFD24_HH_LL(q2,H23,X01);
            AE_MULAAFD24_HH_LL(q3,H23,X12);
            H01=H23;
            AE_LA32X2F24_RIC(X01,ad01,pD01);
            AE_LA32X2F24_RIC(X12,ad12,pD12);
        }
        AE_MULAAFD24_HH_LL(q0,H01,X01);
        AE_MULAAFD24_HH_LL(q1,H01,X12);

        d0 = AE_ROUND24X2F48SASYM( q0, q1 );
        AE_S32F24_L_XC(AE_MOVF24X2_FROMINT32X2(AE_INTSWAP(XN0)), castxcc(ae_f24, p), 4);
        AE_S32F24_L_IP(AE_MOVF24X2_FROMINT32X2(AE_INTSWAP(d0)), castxcc(ae_f24, pY), 4);
        if(N>1)
        {
          AE_S32F24_L_XC(XN0, castxcc(ae_f24, p), 4);
          AE_S32F24_L_XC(d0, castxcc(ae_f24, pY), 4);
        }
        d0 = AE_ROUND24X2F48SASYM( q2, q3 );
        if(N>2)
        {
          AE_S32F24_L_XC(AE_MOVF24X2_FROMINT32X2(AE_INTSWAP(XN1)), castxcc(ae_f24, p), 4);
          AE_S32F24_L_IP(AE_MOVF24X2_FROMINT32X2(AE_INTSWAP(d0)), castxcc(ae_f24, pY), 4);
        }
    }
    bkfir->p=(int32_t*)p;
} /* bkfira24x24_process() */
