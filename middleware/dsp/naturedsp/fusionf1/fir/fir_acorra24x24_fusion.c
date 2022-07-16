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
    Real data circular auto-correlation, 24x24-bit, no requirements on vectors
    length and alignment.
    C code optimized for Fusion
*/

/*-------------------------------------------------------------------------
  Circular Autocorrelation 
  Estimates the auto-correlation of vector x. Returns autocorrelation of 
  length N.
  These functions implement the circular autocorrelation algorithm described
  in the previous chapter with no limitations on x vector length and
  alignment at the cost of increased processing complexity. In addition, this
  implementation variant requires scratch memory area.

  Precision: 
  16x16   16-bit data, 16-bit outputs
  24x24   24-bit data, 24-bit outputs
  32x32   32-bit data, 32-bit outputs
  f       floating point

  Input:
  s[]       scratch area of
            FIR_ACORRA16X16_SCRATCH_SIZE( N )
            FIR_ACORRA24X24_SCRATCH_SIZE( N )
            FIR_ACORRA32X32_SCRATCH_SIZE( N )
            FIR_ACORRAF_SCRATCH_SIZE( N )    
            bytes
  x[N]      input data Q31, Q15 or floating point
  N         length of x
  Output:
  r[N]      output data, Q31, Q15 or floating point

  Restrictions:
  x,r,s should not overlap
  N >0
  s   - aligned on an 8-bytes boundary
-------------------------------------------------------------------------*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common utility and macros declarations. */
#include "common.h"

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
      (void*)( ( (uintptr_t)(addr) + ( (align) - 1 ) ) & ~( (align) - 1 ) )

void fir_acorra24x24( void * restrict s,
                      f24  * restrict r,
                const f24  * restrict x,
                int N )
{
    //
    // Circular autocorrelation algorithm:
    //
    //   r[n] = sum( x[mod(n+m,N)]*x[m] )
    //        m=0..N-1
    //
    // where n = 0..N-1. Throughout the algorithm implementation, x[mod(n+m,N)],
    // m = 0..N-1 is referred to as the left-hand array, while x[m] is called
    // the right-hand array, and it is accessed through the Y pointer.
    void * s_ptr;
    f24  * x_buf;
    f24  * y_buf;
    const ae_f24x2 *          S;
        ae_f24x2 *          D;
        ae_f24   *          D_s;

    ae_valign S_va, D_va;
    ae_f24x2   x0, z_f24x2;

    const ae_f24x2 * restrict pX0;
    const ae_f24x2 * restrict pX1;
    const ae_f24x2 * restrict pY;
            ae_f24x2 * restrict pR;
    ae_f24x2 X01,X12,Y01,d0;
    ae_f64 a0,a1,a2,a3;
    ae_valign ax0,ar,ay;

    int M;
    int n, m;

    NASSERT(s);
    NASSERT(r);
    NASSERT(x);
    NASSERT_ALIGN(s,8);
    NASSERT(N>0);

    //
    // Partition the scratch memory.
    s_ptr = s;
    x_buf = (f24*)ALIGNED_ADDR( s_ptr, 8 );
    s_ptr = x_buf + 2*N + 3;
    ASSERT( (int8_t *)s_ptr - (int8_t *)s <= 
            (int)FIR_ACORRA24X24_SCRATCH_SIZE( N ) );
    y_buf = x_buf + N;
    //----------------------------------------------------------------------------
    // Copy x[N] into the scratch are in a way that simplifies the 
    // autocorrelation computation:
    //   x[0]..x[N-1] x[0]..x[N-1] 0 0 0
    // First copy of x[N].
    S = (const ae_f24x2 *)x;
    D = (      ae_f24x2 *)x_buf;
    S_va = AE_LA64_PP( S );
#ifdef COMPILER_XTENSA
  #pragma concurrent
#endif
    for ( n=0; n<((N+1)>>1); n++ )
    {
        AE_LA32X2F24_IP( x0, S_va, S );
        AE_S32X2F24_IP( x0, D, +8 );
    }
    // Second copy of x[N].
    S = (const ae_f24x2 *)x;
    D = (      ae_f24x2 *)( (f24 *)D - ( N & 1 ) );
    S_va = AE_LA64_PP( S );
    D_va = AE_ZALIGN64();
#ifdef COMPILER_XTENSA
  #pragma concurrent
#endif
    for ( n=0; n<((N+1)>>1); n++ )
    {
        AE_LA32X2F24_IP( x0, S_va, S );
        AE_SA32X2F24_IP( x0, D_va, D );
    }
    AE_SA64POS_FP( D_va, D );
    // Append 3 zeros to allow block-4 processing for the inner loop of the 
    // autocorrelation algorithm.
    D_s = (ae_f24 *)( (f24 *)D - ( N & 1 ) );
    z_f24x2 = 0;
    for ( n=0; n<3; n++ )
    {
        AE_S32F24_L_IP( z_f24x2, D_s, +4 );
    }

  __Pragma("no_reorder")
  /* compute N&~3 correlation results */
  M=(N+1)&~1;
    ar=AE_ZALIGN64();
    pR=(ae_f24x2*)r;
    for ( n=0; n<(N&~3); n+=4 )
    {
        a0=a1=a2=a3=AE_ZERO();
        pY=(const ae_f24x2*)y_buf;
        pX0=(const ae_f24x2*)(x_buf+n);
        pX1=(const ae_f24x2*)(x_buf+n+1);
        ax0=AE_LA64_PP(pX1);
        ay=AE_LA64_PP(pY);
        AE_L32X2F24_IP (X01,pX0,sizeof(int32_t)*2);
        AE_LA32X2F24_IP(X12,ax0,pX1);
        __Pragma("loop_count min=1")
        for ( m=0; m<M; m+=2 )
        {
            AE_LA32X2F24_IP(Y01,ay,pY);
            AE_MULAAFD24_HH_LL(a0,Y01,X01);
            AE_MULAAFD24_HH_LL(a1,Y01,X12);
            AE_L32X2F24_IP (X01,pX0,sizeof(int32_t)*2);
            AE_LA32X2F24_IP(X12,ax0,pX1);
            AE_MULAAFD24_HH_LL(a2,Y01,X01);
            AE_MULAAFD24_HH_LL(a3,Y01,X12);
        }
        d0 = AE_ROUND24X2F48SASYM( a0, a1 ); AE_SA32X2F24_IP( d0, ar,pR);
        d0 = AE_ROUND24X2F48SASYM( a2, a3 ); AE_SA32X2F24_IP( d0, ar,pR);
    }
    AE_SA64POS_FP(ar,pR);
    /* process last 1...3 samples */
    N&=3;
    if(N)
    {
        a0=a1=a2=a3=AE_ZERO();
        pY=(const ae_f24x2*)y_buf;
        pX0=(const ae_f24x2*)(x_buf+n);
        pX1=(const ae_f24x2*)(x_buf+n+1);
        ay=AE_LA64_PP(pY);
        ax0=AE_LA64_PP(pX1);
        AE_L32X2F24_IP (X01,pX0,sizeof(int32_t)*2);
        AE_LA32X2F24_IP(X12,ax0,pX1);
        __Pragma("loop_count min=1")
        for ( m=0; m<M; m+=2 )
        {
            AE_LA32X2F24_IP(Y01,ay,pY);
            AE_MULAAFD24_HH_LL(a0,Y01,X01);
            AE_MULAAFD24_HH_LL(a1,Y01,X12);
            AE_L32X2F24_IP (X01,pX0,sizeof(int32_t)*2);
            AE_LA32X2F24_IP(X12,ax0,pX1);
            AE_MULAAFD24_HH_LL(a2,Y01,X01);
            AE_MULAAFD24_HH_LL(a3,Y01,X12);
        }
        d0 = AE_ROUND24X2F48SASYM( a0, a1 ); 
        AE_S32F24_L_IP(AE_MOVF24X2_FROMINT32X2(AE_INTSWAP(d0)), castxcc(ae_f24, pR), 4);
        if (N>1) AE_S32F24_L_IP(d0, castxcc(ae_f24, pR), 4);
        d0 = AE_ROUND24X2F48SASYM( a2, a3 ); 
        if (N>2) AE_S32F24_L_IP(AE_MOVF24X2_FROMINT32X2(AE_INTSWAP(d0)), castxcc(ae_f24, pR), 4);
    }
} // fir_acorra24x24()
