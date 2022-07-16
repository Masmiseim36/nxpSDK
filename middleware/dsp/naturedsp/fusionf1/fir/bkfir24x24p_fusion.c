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
    Real block FIR filter, 24x24-bit, packed delay line and coefficient storage
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
#define MAGIC     0x68c08c65

/* Filter instance structure. */
typedef struct tag_bkfir24x24_t
{
  int32_t magic;
  int             M; /* Filter length                   */
  const uint8_t * h; /* Filter coefficients             */
        uint8_t * d; /* Delay line of length M          */
        uint8_t * p; /* Pointer into the delay line     */
} bkfir24x24p_t, *bkfir24x24p_ptr_t;


/* Calculate the memory block size for an FIR filter with given attributes. */
size_t bkfir24x24p_alloc( int M )
{
  NASSERT( M > 0 );
  NASSERT(M%4==0);
  M=(M+7)&~7;
  return 2*M*3 + sizeof(bkfir24x24p_t) + 7;
} // bkfir24x24p_alloc()

/* Initialize the filter structure. The delay line is zeroed. */
bkfirf_handle_t bkfir24x24p_init( void * objmem, int M, const f24 * h )
{
  ae_f24x2 *restrict pD;
  const ae_f24x2 *restrict pH;
  ae_valign a;
  bkfir24x24p_t* bkfirp;
  void *           ptr;
  uint8_t * restrict pd;
  uint8_t * restrict ph;
  int m,M1;
  M1=(M+7)&~7;
  NASSERT( objmem && M > 0 && h );
  NASSERT(M1%8==0);
  NASSERT_ALIGN(h,8);
  /* Partition the memory block */
  ptr     = objmem;
  pd = (uint8_t*)((((uintptr_t)ptr)+7)&~7);
  ph = pd+3*M1;
  bkfirp=(bkfir24x24p_t*)(ph+3*M1);
  bkfirp->magic=MAGIC;
  bkfirp->M   = M1;
  bkfirp->h = ph;
  bkfirp->d = bkfirp->p = pd;
  /* copy coefficients and clean upd delay line */
  a=AE_ZALIGN64();
  pH=(const ae_f24x2*)h;
  pD=(ae_f24x2*)ph;
  for (m=0; m<M/2; m++) 
  {
      ae_f24x2 t;
      AE_L32X2F24_IP(t,pH,2*sizeof(int32_t));
      AE_SA24X2_IP(t,a,pD);
  }
  for (m=M; m<M1; m++)
  {
      AE_SA24_IP(AE_ZERO24(),a,pD);
  }
  AE_SA64POS_FP(a,pD);

  pD=(ae_f24x2*)pd;
  a=AE_ZALIGN64();
  for (m=0; m<M1/2; m++) AE_SA24X2_IP(AE_ZERO24(),a,pD);
  AE_SA64POS_FP(a,pD);
  return bkfirp;
} // bkfir24x24p_init()

/* process block of samples */
void bkfir24x24p_process( bkfir24x24p_handle_t _bkfir, 
                         f24 * restrict  y,
                   const f24 * restrict  x, int N )
{
    const ae_f24x2  * restrict pH;
    const ae_f24x2  * pD01;
    const ae_f24x2  * pD12;
    const ae_f24x2  * restrict pX;
    const ae_f24x2  * restrict pY;
    uint8_t *p;
    ae_int24x2 H01,H23;
    ae_valign aH,ad01,ad12,ay,ap;
    ae_int24x2 X12,X01,X23,X34,t,XN0,XN1;
    ae_f24x2 tt;
    int n,m,M;   
    const uint8_t* h;
    bkfir24x24p_t* bkfirp;
    NASSERT(_bkfir);
    bkfirp=(bkfir24x24p_t*)_bkfir;
    NASSERT(bkfirp->magic==MAGIC);
    NASSERT(bkfirp->h);
    NASSERT(bkfirp->d);
    NASSERT(bkfirp->p);
    NASSERT_ALIGN(bkfirp->d,8);
    NASSERT(N%4==0);
    NASSERT_ALIGN(x,8);
    NASSERT((bkfirp->M%8)==0); /* to align both ends of circular buffers on 8-byte boundaries */
    NASSERT(x);
    NASSERT(y);
    if(N<=0) return;
    M=bkfirp->M;
    NASSERT(N>0);
    NASSERT(M>0);
    h=bkfirp->h;
    WUR_AE_CBEGIN0( (uintptr_t)( bkfirp->d                ));
    WUR_AE_CEND0  ( (uintptr_t)( bkfirp->d) + 3*bkfirp->M  );
    ay=AE_ZALIGN64();
    pY=(ae_f24x2*)y;
    p =bkfirp->p;
    pX=(const ae_f24x2*)(x);
    for (n=0; n<N; n+=4)
    {
        ae_f64 q0,q1,q2,q3;
        pH=(const ae_f24x2*)h;
        aH=AE_LA64_PP(pH);
        pD01=(const ae_f24x2*)((uint8_t*)(p)+0+2);
        pD12=(const ae_f24x2*)((uint8_t*)(p)+3+2);
        AE_LA24X2NEG_PC(ad01,pD01);
        AE_LA24X2NEG_PC(ad12,pD12);

        AE_LA24X2_RIC(X12,ad12,pD12);
        AE_LA24X2_RIC(t,ad01,pD01);
        XN0=AE_L32X2F24_I (pX,0*sizeof(int32_t));
        XN1=AE_L32X2F24_I (pX,2*sizeof(int32_t));
        X12=AE_MOVINT24X2_FROMINT32X2(AE_INTSWAP(XN0));
        X34=AE_MOVINT24X2_FROMINT32X2(AE_INTSWAP(XN1));
        X01=AE_SEL24_HL(XN0,t);
        X23=AE_SEL24_HL(XN1,XN0);

        AE_LA24X2_IP(H01, aH, pH);
        q0=q1=AE_ZERO();
        q2=AE_MULZAAFD24_HH_LL(H01,X23);
        q3=AE_MULZAAFD24_HH_LL(H01,X34);
        __Pragma("loop_count min=1")
        for (m=0; m<M-2; m+=2)
        {
            AE_LA24X2_IP(H23, aH, pH);
            AE_MULAAFD24_HH_LL(q0,H01,X01);
            AE_MULAAFD24_HH_LL(q1,H01,X12);
            AE_MULAAFD24_HH_LL(q2,H23,X01);
            AE_MULAAFD24_HH_LL(q3,H23,X12);
            H01=H23;
            AE_LA24X2_RIC(X01,ad01,pD01);
            AE_LA24X2_RIC(X12,ad12,pD12);
        }
        AE_MULAAFD24_HH_LL(q0,H01,X01);
        AE_MULAAFD24_HH_LL(q1,H01,X12);

        ap=AE_ZALIGN64();
        AE_L32X2F24_IP(tt,pX,2*sizeof(int32_t)); AE_SA24X2_IC(tt,ap,p);
        AE_L32X2F24_IP(tt,pX,2*sizeof(int32_t)); AE_SA24X2_IC(tt,ap,p);
        AE_SA64POS_FP(ap,p);
        // Store 4 filter outputs.
        AE_SA32X2F24_IP( AE_ROUND24X2F48SASYM( q0, q1 ), ay, castxcc(ae_f24x2, pY) );
        AE_SA32X2F24_IP(AE_ROUND24X2F48SASYM(q2, q3), ay, castxcc(ae_f24x2, pY));
    }
    AE_SA64POS_FP(ay, castxcc(void, pY));
   bkfirp->p=p;
} /* bkfir24x24p_process*/
