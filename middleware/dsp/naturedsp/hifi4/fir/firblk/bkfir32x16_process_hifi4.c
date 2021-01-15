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

/*
  NatureDSP Signal Processing Library. FIR part
    Real block FIR filter, 32x16-bit
    C code optimized for HiFi4
  IntegrIT, 2006-2018
*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fir.h"
/* Common utility and macros declarations. */
#include "common.h"
#include "bkfir32x16_common.h"

/*-------------------------------------------------------------------------
  Real FIR filter.
  Computes a real FIR filter (direct-form) using IR stored in vector h. The 
  real data input is stored in vector x. The filter output result is stored 
  in vector y. The filter calculates N output samples using M coefficients 
  and requires last M-1 samples in the delay line.
  NOTE: 
  1. User application is not responsible for management of delay lines
  2. User has an option to set IR externally or copy from original location 
     (i.e. from the slower constant memory). In the first case, user is 
     responsible for right alignment, ordering and zero padding of filter 
     coefficients - usually array is composed from zeroes (left padding), 
     reverted IR and right zero padding.


  Precision: 
  16x16    16-bit data, 16-bit coefficients, 16-bit outputs. Ordinary variant 
           and stereo
  32x16    32-bit data, 16-bit coefficients, 32-bit outputs
  32x32    32-bit data, 32-bit coefficients, 32-bit outputs. Ordinary variant 
           and stereo
  32x32ep  the same as above but using 72-bit accumulator for intermediate 
           computations
  f        floating point. Ordinary variant and stereo

  Input:
  x[N*S]   input samples, Q31, Q15, floating point
  h[M]     filter coefficients in normal order, Q31, Q15, floating point
  hl[M]    for stereo filters: filter coefficients for left channel
  hr[M]    for stereo filters: filter coefficients for right channel
  N        length of sample block, should be a multiple of 4
  M        length of filter, should be a multiple of 4
  extIR    if zero, IR is copied from original location, otherwise not
           but user should keep alignment, order of coefficients 
           and zero padding requirements shown below
  S        1 for ordinary (single channel) filters, 2 - for stereo variant
  
  Output:
  y[N*S]   output samples, Q31, Q15, floating point

  Alignment, ordering and zero padding for external IR  (extIR!=0)
  ------------------------+----------+--------------+--------------+----------------
  Function                |Alignment,|Left zero     |   Coefficient| Right zero 
                          | bytes    |padding, bytes|   order      | padding, bytes
  ------------------------+----------+--------------+--------------+----------------
  bkfir16x16_init         |     8    |      2       |  inverted    |  6
  bkfir32x16_init (M>32)  |     8    |     10       |  inverted    |  6
  bkfir32x16_init (M<=32) |     8    |      2       |  inverted    |  6
  bkfir32x32_init         |     8    |      4       |  inverted    |  12
  bkfir32x32ep_init       |     8    |      4       |  inverted    |  12
  bkfirf_init             |     8    |      0       |  direct      |  0
  stereo_bkfir16x16_init  |     8    |      2       |  inverted    |  6
  stereo_bkfir32x32_init  |     8    |      4       |  inverted    |  12
  stereo_bkfirf_init      |     8    |      0       |  direct      |  0
  ------------------------+----------+--------------+--------------+----------------

  Restrictions:
  x, y     should not be overlapping
  x, h     aligned on a 8-bytes boundary
  N, M     multiples of 4 
-------------------------------------------------------------------------*/
void bkfir32x16_process( bkfir32x16_handle_t handle, 
                         int32_t * restrict  y,
                   const int32_t * restrict  x, int N)
{
    bkfir32x16_ptr_t bkfir = (bkfir32x16_ptr_t)handle;
    NASSERT( bkfir && bkfir->magic == BKFIR32X16_MAGIC && y && x );
    if(N<=0) return;
    bkfir->fnprocess(bkfir,y,x,N);
}

/* processing function for M>32 */
void bkfir32x16_processBig( bkfir32x16_ptr_t bkfir, 
                         int32_t * restrict  y,
                   const int32_t * restrict  x, int N)
{
    const ae_f16x4   *          C;
    const ae_f16x4   *          C_;
          ae_int32x2 * restrict D_wr;
    const ae_int32x2 *          D_rd0;
    const ae_int32x2 *          D_rd1;
    const ae_int32x2 *          X;
        ae_int32     * restrict Y;
    ae_int32x2  t0, t1, t01, t23, t45;
    ae_f64      q0, q1, q2, q3;
    ae_int16x4  c0, c1;
    ae_f64     z; 
    int M;
    int m, n;
    ae_valign C_va;
    NASSERT( bkfir && bkfir->magic == BKFIR32X16_MAGIC && y && x );
    NASSERT( IS_ALIGN( x ) );
    M = bkfir->M;
    NASSERT(N%4==0);
    NASSERT(M%4==0 && M>32);
  //
  // Setup pointers and circular delay line buffer.    
  //

  D_wr = (      ae_int32x2*)(bkfir->delayLine + bkfir->wrIx);
  X    = (const ae_int32x2*)x;
  Y    = (      ae_int32  *)y;

    WUR_AE_CBEGIN0( (uintptr_t)( bkfir->delayLine                   ) );
    WUR_AE_CEND0  ( (uintptr_t)( bkfir->delayLine + bkfir->delayLen ) );
    WUR_AE_CBEGIN1( (uintptr_t)( bkfir->coef                   ) );
    WUR_AE_CEND1  ( (uintptr_t)( bkfir->coef + bkfir->M + 12 ) );

    // Reset the coefficients pointer. Now it looks at the tap corresponding
    // to the oldest sample in the delay line.
    C = (const ae_f16x4*)bkfir->coef;
    
    z = AE_ZERO64();
    //
    // Break the input signal into 8-samples blocks. For each block, store 8
    // samples to the delay line and compute the filter response.
    //
    __Pragma("loop_count min=1")
    __Pragma("concurrent")
    for ( n=0; n<(N>>2); n++ )
    {
        // Load 8 input samples.
        // Q31
        AE_L32X2_IP( t0, X, +16 );
        t1 = AE_L32X2_I( X, -8 );

        // Store 8 samples to the delay line buffer with circular address update.
        // Q31
        AE_S32X2_XC( t0, D_wr, +8 );
        AE_S32X2_XC( t1, D_wr, +8 );

        // Circular buffer pointer looks at the oldest sample: M+8 samples back from
        // the newest one.
        D_rd1 = D_wr;
        D_rd0 = (ae_int32x2*)( D_rd1 + 1 );
        C_ = (const ae_f16x4*)((uintptr_t)C+2*3);
        AE_LA16X4POS_PC1(C_va,C_);

        // Load two oldest samples for even/odd accumulators.
        // Q31
        AE_L32X2_XC  ( t01, D_rd1, +16 );

        q0 = z; q1 = z; q2 = z; q3 = z;
        //
        // Inner loop: process 4 taps for 4 accumulators on each trip. Totally we 
        // perform M+4 MACs for each accumulator, 4 of which fall on zero taps
        // inserted into the impulse response during initialization.
        //

        AE_L16X4_XC1(c0, C, +8);
#if 0
        __Pragma("loop_count min=9")
        __Pragma("concurrent")
      for ( m=0; m<(M>>2)+1; m++ )             
      {
        AE_LA16X4_IC1(c1, C_va, C_);
        AE_L16X4_XC1(c0, C, +8);
        AE_L32X2_XC( t23, D_rd0, +16 );
        AE_L32X2_XC( t45, D_rd1, +16 );            
                                                            
        //Q16.47
        AE_MULAAAAFQ32X16(q0,t01,t23,c0);
        AE_MULAAAAFQ32X16(q1,t01,t23,c1);
        AE_MULAAAAFQ32X16(q2,t23,t45,c0);
        AE_MULAAAAFQ32X16(q3,t23,t45,c1);
        t01 = t45; 
      }
#else
        __Pragma("loop_count min=9")
        __Pragma("concurrent")
        for ( m=0; m<(M>>2)+1; m++ )             
        {
            c1=c0;
            AE_L16X4_XC1(c0, C, +8);
            c1=AE_SEL16_4321(c1,c0);
            __Pragma("ymemory(D_rd1)")
            AE_L32X2_XC( t23, D_rd0, +16 );
            AE_L32X2_XC( t45, D_rd1, +16 );            
                                                            
            //Q16.47
            AE_MULAAAAFQ32X16(q0,t01,t23,c0);
            AE_MULAAAAFQ32X16(q1,t01,t23,c1);
            AE_MULAAAAFQ32X16(q2,t23,t45,c0);
            AE_MULAAAAFQ32X16(q3,t23,t45,c1);
            t01 = t45; 
      }
#endif
        AE_ADDCIRC16X4_XC1(C,+8);

        // Convert and save 4 outputs.
        // 2xQ31 <- 2xQ16.47 - 16 w/ rounding and saturation.
        AE_S32RA64S_IP( q0, Y, +4 );
        AE_S32RA64S_IP( q1, Y, +4 );
        AE_S32RA64S_IP( q2, Y, +4 );
        AE_S32RA64S_IP( q3, Y, +4 );
    }
    bkfir->wrIx = (int)((int32_t *)D_wr - bkfir->delayLine) ;
} // bkfir32x16_processBig()

/* processing function for M>4 && M<=32 */
void bkfir32x16_processSmall( bkfir32x16_ptr_t bkfir, 
                         int32_t * restrict  y,
                   const int32_t * restrict  x, int N)
{
    const ae_f16x4   *          C;
        ae_int32x2 * restrict D_wr;
    const ae_int32x2 *          D_rd0;
    const ae_int32x2 *          D_rd1;
    const ae_int32x2 *          X;
        ae_int32     * restrict Y;
    ae_int32x2  t0, t1, t2, t3, t01, t23, t45;
    ae_f64    q0, q1, q2, q3;
    ae_int16x4  c;
    ae_f64    q4, q5, q6, q7; 
    ae_int32x2 t12, t34, t56, t67, t89, t9a, t78;
    ae_f64     z; 
    int M;
    int m, n;
    ae_valign D_va;
    NASSERT( bkfir && bkfir->magic == BKFIR32X16_MAGIC && y && x );
    NASSERT( IS_ALIGN( x ) );
    M = bkfir->M;
    NASSERT(N%4==0);
    NASSERT(M%4==0 && M>4 && M<=32);


    //
    // Setup pointers and circular delay line buffer.    
    //

    D_wr = (      ae_int32x2*)(bkfir->delayLine + bkfir->wrIx);
    X    = (const ae_int32x2*)x;
    Y    = (      ae_int32  *)y;

    WUR_AE_CBEGIN0( (uintptr_t)( bkfir->delayLine                   ) );
    WUR_AE_CEND0  ( (uintptr_t)( bkfir->delayLine + bkfir->delayLen ) );
    WUR_AE_CBEGIN1( (uintptr_t)( bkfir->coef                   ) );
    WUR_AE_CEND1  ( (uintptr_t)( bkfir->coef + bkfir->M + 4 ) );

    // Reset the coefficients pointer. Now it looks at the tap corresponding
    // to the oldest sample in the delay line.
    C = (const ae_f16x4*)bkfir->coef;
    z = AE_ZERO64();
    //
    // Break the input signal into 8-samples blocks. For each block, store 8
    // samples to the delay line and compute the filter response.
    //
    for ( n=0; n<(N>>3); n++ )
    {
     
        // Load 8 input samples.
        // Q31
        AE_L32X2_IP( t0, X, +8 );
        AE_L32X2_IP( t1, X, +8 );
        AE_L32X2_IP( t2, X, +8 );
        AE_L32X2_IP( t3, X, +8 );

        // Store 8 samples to the delay line buffer with circular address update.
        // Q31
        AE_S32X2_XC( t0, D_wr, +8 );
        AE_S32X2_XC( t1, D_wr, +8 );
        AE_S32X2_XC( t2, D_wr, +8 );
        AE_S32X2_XC( t3, D_wr, +8 );

        // Circular buffer pointer looks at the oldest sample: M+8 samples back from
        // the newest one.
        D_rd0 = D_wr;
        D_rd1 = (ae_int32x2*)( (int32_t*)D_rd0 + 1 );

        AE_LA32X2POS_PC( D_va, D_rd1 );

        // Load two oldest samples for even/odd accumulators.
        // Q31
        AE_L32X2_XC  ( t01, D_rd0, +8 );
        AE_L32X2_XC  ( t23, D_rd0, +8 );
        AE_L32X2_XC  ( t45, D_rd0, +8 );
        AE_LA32X2_IC( t12, D_va, D_rd1 );
        AE_LA32X2_IC( t34, D_va, D_rd1 );
        AE_LA32X2_IC( t56, D_va, D_rd1 );

        q0 = z; q1 = z; q2 = z; q3 = z;
        q4 = z; q5 = z; q6 = z; q7 = z;

        //
        // Inner loop: process 4 taps for 8 accumulators on each trip. Totally we 
        // perform M+4 MACs for each accumulator, 4 of which fall on zero taps
        // inserted into the impulse response during initialization.
        //
        __Pragma("loop_count min=3")
        for ( m=0; m<(M>>2)+1; m++ )             
        {
        AE_L32X2_XC( t67, D_rd0, +8 );
        AE_L32X2_XC( t89, D_rd0, +8 );

        // Load next two samples for odd accumulators. 
        // Q31
        AE_LA32X2_IC( t78, D_va, D_rd1 );
        AE_LA32X2_IC( t9a, D_va, D_rd1 );

        // Load the next 4 tap coefficients.
        // Q31
        AE_L16X4_XC1(c, C, +8);

        //Q16.47
        AE_MULAAAAFQ32X16(q0,t01,t23,c);
        AE_MULAAAAFQ32X16(q1,t12,t34,c);
        AE_MULAAAAFQ32X16(q2,t23,t45,c);
        AE_MULAAAAFQ32X16(q3,t34,t56,c);

        AE_MULAAAAFQ32X16(q4,t45,t67,c);
        AE_MULAAAAFQ32X16(q5,t56,t78,c);
        AE_MULAAAAFQ32X16(q6,t67,t89,c);
        AE_MULAAAAFQ32X16(q7,t78,t9a,c);

        t01 = t45; t23 = t67; t45 = t89; 
        t12 = t56; t34 = t78; t56 = t9a;
        }

        // Convert and save 8 outputs.
        // 2xQ31 <- 2xQ16.47 - 16 w/ rounding and saturation.
        AE_S32RA64S_IP( q0, Y, +4 );
        AE_S32RA64S_IP( q1, Y, +4 );
        AE_S32RA64S_IP( q2, Y, +4 );
        AE_S32RA64S_IP( q3, Y, +4 );
        AE_S32RA64S_IP( q4, Y, +4 );
        AE_S32RA64S_IP( q5, Y, +4 );
        AE_S32RA64S_IP( q6, Y, +4 );
        AE_S32RA64S_IP( q7, Y, +4 );
    }
    //
    // If the signal chunk length N is not a multiple of 8, process the last
    // quadruple: store it to the delay line and compute the filter response.
    //

    if ( N & 4 )
    {
        // Load 8 input samples.
        // Q31
        AE_L32X2_IP( t0, X, +8 );
        AE_L32X2_IP( t1, X, +8 );

        // Store 8 samples to the delay line buffer with circular address update.
        // Q31
        AE_S32X2_XC( t0, D_wr, +8 );
        AE_S32X2_XC( t1, D_wr, +8 );

        // Circular buffer pointer looks at the oldest sample: M+8 samples back from
        // the newest one.
        D_rd0 = D_wr;
        D_rd1 = (ae_int32x2*)( (int32_t*)D_rd0 + 1 );

        AE_LA32X2POS_PC( D_va, D_rd1 );

        // Load two oldest samples for even/odd accumulators.
        // Q31
        AE_L32X2_XC  ( t01, D_rd0, +8 );
        AE_L32X2_XC  ( t01, D_rd0, +8 );
        AE_L32X2_XC  ( t01, D_rd0, +8 );

        AE_LA32X2_IC( t12, D_va, D_rd1 );
        AE_LA32X2_IC( t12, D_va, D_rd1 );
        AE_LA32X2_IC( t12, D_va, D_rd1 );


        q0 = z; q1 = z; q2 = z; q3 = z;

        //
        // Inner loop: process 4 taps for 8 accumulators on each trip. Totally we 
        // perform M+4 MACs for each accumulator, 4 of which fall on zero taps
        // inserted into the impulse response during initialization.
        //
        __Pragma("loop_count min=3")
        for ( m=0; m<(M>>2)+1; m++ )             
        {
        AE_L32X2_XC( t23, D_rd0, +8 );
        AE_L32X2_XC( t45, D_rd0, +8 );

        // Load next two samples for odd accumulators. 
        // Q31
        AE_LA32X2_IC( t34, D_va, D_rd1 );
        AE_LA32X2_IC( t56, D_va, D_rd1 );

        // Load the next 4 tap coefficients.
        // Q31
        AE_L16X4_XC1(c, C, +8);

        //Q16.47
        AE_MULAAAAFQ32X16(q0,t01,t23,c);
        AE_MULAAAAFQ32X16(q1,t12,t34,c);
        AE_MULAAAAFQ32X16(q2,t23,t45,c);
        AE_MULAAAAFQ32X16(q3,t34,t56,c);


        t01 = t45; 
        t12 = t56; 
        }

        // Convert and save 8 outputs.
        // 2xQ31 <- 2xQ16.47 - 16 w/ rounding and saturation.
        AE_S32RA64S_IP( q0, Y, +4 );
        AE_S32RA64S_IP( q1, Y, +4 );
        AE_S32RA64S_IP( q2, Y, +4 );
        AE_S32RA64S_IP( q3, Y, +4 );
    }
    bkfir->wrIx = (int)((int32_t *)D_wr - bkfir->delayLine) ;
} // bkfir32x16_processSmall()

/* processing function for M==4 */
void bkfir32x16_process4( bkfir32x16_ptr_t bkfir, 
                         int32_t * restrict  y,
                   const int32_t * restrict  x, int N)
{
        ae_int32x2   * restrict D_wr;
    const ae_int32x2 *          D_rd0;
    const ae_int32x2 *          D_rd1;
    const ae_int32x2 * restrict X;
        ae_int32     * restrict Y;

    ae_int32x2 t12, t34, t56;
    ae_int32x2  t0, t1,  t01, t23, t45;
    ae_f64    q0, q1, q2, q3;
    ae_int16x4  c0, c1;
    ae_f64     z; 
    int M;
    int n;
    ae_valign  D_va;
    NASSERT( bkfir && bkfir->magic == BKFIR32X16_MAGIC && y && x );
    NASSERT( IS_ALIGN( x ) );
    M = bkfir->M;
    NASSERT(N%4==0);
    NASSERT(M==4);
    //
    // Setup pointers and circular delay line buffer.    
    //
    D_wr = (      ae_int32x2*)(bkfir->delayLine + bkfir->wrIx);
    X    = (const ae_int32x2*)x;
    Y    = (      ae_int32  *)y;
    WUR_AE_CBEGIN0( (uintptr_t)( bkfir->delayLine                   ) );
    WUR_AE_CEND0  ( (uintptr_t)( bkfir->delayLine + bkfir->delayLen ) );
    // Reset the coefficients pointer. Now it looks at the tap corresponding
    // to the oldest sample in the delay line.
    c0=AE_L16X4_I((const ae_int16x4*)bkfir->coef, 0);
    c1=AE_L16X4_I((const ae_int16x4*)bkfir->coef, 1*sizeof(ae_int16x4));

    z = AE_ZERO64();
    //
    // Break the input signal into 8-samples blocks. For each block, store 8
    // samples to the delay line and compute the filter response.
    //
    __Pragma("loop_count min=1")
    for ( n=0; n<(N>>2); n++ )
    {
        // Load 8 input samples.
        AE_L32X2_IP( t0, X, +8 );
        AE_L32X2_IP( t1, X, +8 );
        // Store 8 samples to the delay line buffer with circular address update.
        AE_S32X2_XC( t0, D_wr, +8 );
        AE_S32X2_XC( t1, D_wr, +8 );

        // Circular buffer pointer looks at the oldest sample: M+8 samples back from
        // the newest one.
        D_rd0 = D_wr;
        D_rd1 = (ae_int32x2*)( (int32_t*)D_rd0 + 1 );

        AE_LA32X2POS_PC( D_va, D_rd1 );
        AE_L32X2_XC  ( t01, D_rd0, +8 );
        AE_L32X2_XC  ( t01, D_rd0, +8 );
        AE_L32X2_XC  ( t01, D_rd0, +8 );

        AE_LA32X2_IC( t12, D_va, D_rd1 );
        AE_LA32X2_IC( t12, D_va, D_rd1 );
        AE_LA32X2_IC( t12, D_va, D_rd1 );

        AE_L32X2_XC( t23, D_rd0, +8 );
        AE_L32X2_XC( t45, D_rd0, +8 );
        AE_LA32X2_IC( t34, D_va, D_rd1 );
        AE_LA32X2_IC( t56, D_va, D_rd1 );
        q0=AE_MULZAAAAFQ32X16(t01,t23,c0);
        q1=AE_MULZAAAAFQ32X16(t12,t34,c0);
        q2=AE_MULZAAAAFQ32X16(t23,t45,c0);
        q3=AE_MULZAAAAFQ32X16(t34,t56,c0);
        t01 = t45; 
        t12 = t56; 
        AE_L32X2_XC( t23, D_rd0, +8 );
        AE_L32X2_XC( t45, D_rd0, +8 );
        AE_LA32X2_IC( t34, D_va, D_rd1 );
        AE_LA32X2_IC( t56, D_va, D_rd1 );
        AE_MULAAAAFQ32X16(q0,t01,t23,c1);
        AE_MULAAAAFQ32X16(q1,t12,t34,c1);
        AE_MULAAAAFQ32X16(q2,t23,t45,c1);
        AE_MULAAAAFQ32X16(q3,t34,t56,c1);
        AE_S32RA64S_IP( q0, Y, +4 );
        AE_S32RA64S_IP( q1, Y, +4 );
        AE_S32RA64S_IP( q2, Y, +4 );
        AE_S32RA64S_IP( q3, Y, +4 );
    }
    bkfir->wrIx = (int)((int32_t *)D_wr - bkfir->delayLine) ;

} // bkfir32x16_process4()
