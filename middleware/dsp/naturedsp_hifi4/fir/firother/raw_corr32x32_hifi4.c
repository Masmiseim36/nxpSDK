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
    NatureDSP Signal Processing Library. FIR part
    Real data circular cross-correlation, 32x32-bit, no requirements on vectors 
    length and alignment.
    C code optimized for HiFi4
    IntegrIT, 2006-2018
*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Common utility and macros declarations. */
#include "common.h"
#include "raw_corr32x32.h"

/*
    Helper for circular correlation/convolution with originally non-aligned inputs
    This function takes aligned inputs (allocated from the scratch) with circularly 
    duplicated inputs

    raw linear correlation
    input:
    x[N+M-1 ]  - input . should be aligned on 8-byte boundary
    y[M+3]     - input padded with at least 3 zeroes. should be aligned on 8-byte boundary
    output:
    r[N]       - correlations
*/
void raw_corr32x32(int32_t* restrict r, const int32_t* restrict  x,const int32_t* restrict y, int N,int M)
{
  const ae_int32x2 *          S;
  const ae_int32x2 *          X;
  const ae_int32x2 *          Y;
        ae_int32x2 * restrict R;
        ae_int32   * restrict R_s;

  ae_valign R_va;

  ae_f64     q0, q1, q2, q3, q4, q5, q6, q7;
  ae_int32x2   x0, x1, x2, x3, x4, x5;
  ae_int32x2   y0, y1;
  ae_int32x2   r0, r1, r2, r3;

  int _M;
  int n, m;

  X = (const ae_int32x2 *)x;
  R = (      ae_int32x2 *)r;

  R_va = AE_ZALIGN64();

  _M = M + ( -M & 3 );

  //
  // Process vector x data in 8-entries blocks.
  //

  for ( n=0; n<(N>>3); n++ )
  {
    // Load 8 left-most unprocessed x[] entries, the first is x[8*n].
    // Q31
    AE_L32X2_IP( x0, X, +8 );
    AE_L32X2_IP( x1, X, +8 );
    AE_L32X2_IP( x2, X, +8 );
    AE_L32X2_IP( x3, X, +8 );

    // Use the shuttle pointer when computing the correlation. Preserve the X
    // pointer for the next iteration.
    S = X;

    //
    // Inner loop prologue: process first 4 y[] entries for 8 accumulators.
    //

    // Load 4 more x[] entries. Altogether we have 12 x[] entries stored in
    // 6 AE registers.
    // Q31
    AE_L32X2_IP( x4, S, +8 );
    AE_L32X2_IP( x5, S, +8 );

    Y = (const ae_int32x2 *)y;

    // Load y[0]..y[3].
    // Q31
    AE_L32X2_IP( y0, Y, +8 );
    AE_L32X2_IP( y1, Y, +8 );
    // Q16.47 <- [ Q31*Q31 - 15] + [ Q31*Q31 - 15 ]
    AE_MULFD32X2RA_FIR_H( q0, q1, x0, x1, y0 );
    AE_MULFD32X2RA_FIR_H( q2, q3, x1, x2, y0 );
    AE_MULFD32X2RA_FIR_H( q4, q5, x2, x3, y0 );
    AE_MULFD32X2RA_FIR_H( q6, q7, x3, x4, y0 );

    // Q16.47 <- Q16.47 + [ Q31*Q31 - 15] + [ Q31*Q31 - 15 ]
    AE_MULAFD32X2RA_FIR_H( q0, q1, x1, x2, y1 );
    AE_MULAFD32X2RA_FIR_H( q2, q3, x2, x3, y1 );
    AE_MULAFD32X2RA_FIR_H( q4, q5, x3, x4, y1 );
    AE_MULAFD32X2RA_FIR_H( q6, q7, x4, x5, y1 );

    // 4 x[] entries are done, shift them out of the registers.
    x0 = x2; x1 = x3; x2 = x4; x3 = x5;

    //
    // Inner loop kernel: process 4 y[] entries for 8 accumulators. 12-entries 
    // register delay line is updated similarly to the loop prologue.
    //

    for ( m=0; m<(_M>>2)-1; m++ )
    {
      // Q31
      AE_L32X2_IP( x4, S, +8 );
      AE_L32X2_IP( x5, S, +8 );

      // Load y[4*(m+1)]..y[4*(m+1)+3].
      // Q31
      AE_L32X2_IP( y0, Y, +8 );
      AE_L32X2_IP( y1, Y, +8 );

      // Q16.47 <- Q16.47 + [ Q31*Q31 - 15] + [ Q31*Q31 - 15 ]
      AE_MULAFD32X2RA_FIR_H( q0, q1, x0, x1, y0 );
      AE_MULAFD32X2RA_FIR_H( q2, q3, x1, x2, y0 );
      AE_MULAFD32X2RA_FIR_H( q4, q5, x2, x3, y0 );
      AE_MULAFD32X2RA_FIR_H( q6, q7, x3, x4, y0 );

      // Q16.47 <- Q16.47 + [ Q31*Q31 - 15] + [ Q31*Q31 - 15 ]
      AE_MULAFD32X2RA_FIR_H( q0, q1, x1, x2, y1 );
      AE_MULAFD32X2RA_FIR_H( q2, q3, x2, x3, y1 );
      AE_MULAFD32X2RA_FIR_H( q4, q5, x3, x4, y1 );
      AE_MULAFD32X2RA_FIR_H( q6, q7, x4, x5, y1 );

      // 4 x[] entries are done, shift them out of the registers.
      x0 = x2; x1 = x3; x2 = x4; x3 = x5;
    }

    //
    // Convert and save 8 correlation results.
    //

    // Q31 <- Q16.47 - 16 w/ rounding and saturation.
    r0 = AE_ROUND32X2F48SASYM( q0, q1 );
    r1 = AE_ROUND32X2F48SASYM( q2, q3 );
    r2 = AE_ROUND32X2F48SASYM( q4, q5 );
    r3 = AE_ROUND32X2F48SASYM( q6, q7 );

    // Q31
    AE_SA32X2_IP( r0, R_va, R );
    AE_SA32X2_IP( r1, R_va, R );
    AE_SA32X2_IP( r2, R_va, R );
    AE_SA32X2_IP( r3, R_va, R );
  }

  //----------------------------------------------------------------------------
  // (N&~7) correlation results are done by now. If N - (N&~7) >= 4, then
  // compute a block of 4 results.

  if ( N & 4 )
  {
    // Load 4 left-most unprocessed x[] entries.
    // Q31
    AE_L32X2_IP( x0, X, +8 );
    AE_L32X2_IP( x1, X, +8 );

    // Use the shuttle pointer when computing the correlation. Preserve the X
    // pointer for the last block of (N&3) results.
    S = X;

    //
    // Inner loop prologue: process first 4 y[] entries for 4 accumulators.
    //

    // Load 4 more x[] entries. Altogether we have 8 x[] entries residing in
    // 4 AE registers.
    // Q31
    AE_L32X2_IP( x2, S, +8 );
    AE_L32X2_IP( x3, S, +8 );

    Y = (const ae_int32x2 *)y;

    // Load y[0]..y[3].
    // Q31
    AE_L32X2_IP( y0, Y, +8 );
    AE_L32X2_IP( y1, Y, +8 );

    // Q16.47 <- [ Q31*Q31 - 15] + [ Q31*Q31 - 15 ]
    AE_MULFD32X2RA_FIR_H( q0, q1, x0, x1, y0 );
    AE_MULFD32X2RA_FIR_H( q2, q3, x1, x2, y0 );

    // Q16.47 <- Q16.47 + [ Q31*Q31 - 15] + [ Q31*Q31 - 15 ]
    AE_MULAFD32X2RA_FIR_H( q0, q1, x1, x2, y1 );
    AE_MULAFD32X2RA_FIR_H( q2, q3, x2, x3, y1 );

    // 4 x[] entries are done, shift them out of the registers.
    x0 = x2; x1 = x3;

    //
    // Inner loop kernel: process 4 y[] entries for 4 accumulators. 8-entries 
    // register delay line is updated similarly to the loop prologue.
    //

    for ( m=0; m<(_M>>2)-1; m++ )
    {
      // Q31
      AE_L32X2_IP( x2, S, +8 );
      AE_L32X2_IP( x3, S, +8 );

      // Load y[4*(m+1)]..y[4*(m+1)+3].
      // Q31
      AE_L32X2_IP( y0, Y, +8 );
      AE_L32X2_IP( y1, Y, +8 );

      // Q16.47 <- Q16.47 + [ Q31*Q31 - 15] + [ Q31*Q31 - 15 ]
      AE_MULAFD32X2RA_FIR_H( q0, q1, x0, x1, y0 );
      AE_MULAFD32X2RA_FIR_H( q2, q3, x1, x2, y0 );

      // Q16.47 <- Q16.47 + [ Q31*Q31 - 15] + [ Q31*Q31 - 15 ]
      AE_MULAFD32X2RA_FIR_H( q0, q1, x1, x2, y1 );
      AE_MULAFD32X2RA_FIR_H( q2, q3, x2, x3, y1 );

      // 4 x[] entries are done, shift them out of the registers.
      x0 = x2; x1 = x3;
    }

    //
    // Convert and save 4 correlation results.
    //

    // Q31 <- Q16.47 - 16 w/ rounding and saturation.
    r0 = AE_ROUND32X2F48SASYM( q0, q1 );
    r1 = AE_ROUND32X2F48SASYM( q2, q3 );

    // Q31
    AE_SA32X2_IP( r0, R_va, R );
    AE_SA32X2_IP( r1, R_va, R );
  }

  AE_SA64POS_FP( R_va, R );

  //----------------------------------------------------------------------------
  // (N&~3) correlation results are done by now. If N is not a multiple of 4,
  // compute the last (N&3) results.

  if ( N & 3 )
  {
    // Load 4 left-most unprocessed x[] entries.
    // Q31
    AE_L32X2_IP( x0, X, +8 );
    AE_L32X2_IP( x1, X, +8 );

    //
    // Inner loop prologue: process first 4 y[] entries for 4 accumulators.
    //

    // Load 4 more x[] entries. Altogether we have 8 x[] entries residing in
    // 4 AE registers.
    // Q31
    AE_L32X2_IP( x2, X, +8 );
    x3=AE_ZERO32();
    if((_M>>2)>1) AE_L32X2_IP( x3, X, +8 ); 

    Y = (const ae_int32x2 *)y;

    // Load y[0]..y[3].
    // Q31
    AE_L32X2_IP( y0, Y, +8 );
    AE_L32X2_IP( y1, Y, +8 );

    // Q16.47 <- [ Q31*Q31 - 15] + [ Q31*Q31 - 15 ]
    AE_MULFD32X2RA_FIR_H( q0, q1, x0, x1, y0 );
    AE_MULFD32X2RA_FIR_H( q2, q3, x1, x2, y0 );

    // Q16.47 <- Q16.47 + [ Q31*Q31 - 15] + [ Q31*Q31 - 15 ]
    AE_MULAFD32X2RA_FIR_H( q0, q1, x1, x2, y1 );
    AE_MULAFD32X2RA_FIR_H( q2, q3, x2, x3, y1 );

    // 4 x[] entries are done, shift them out of the registers.
    x0 = x2; x1 = x3;

    //
    // Inner loop kernel: process 4 y[] entries for 4 accumulators. 8-entries 
    // register delay line is updated similarly to the loop prologue.
    //

    for ( m=0; m<(_M>>2)-1; m++ )
    {
      // Q31
      AE_L32X2_IP( x2, X, +8 );
      AE_L32X2_IP( x3, X, +8 );

      // Load y[4*(m+1)]..y[4*(m+1)+3].
      // Q31
      AE_L32X2_IP( y0, Y, +8 );
      AE_L32X2_IP( y1, Y, +8 );

      // Q16.47 <- [ Q31*Q31 - 15] + [ Q31*Q31 - 15 ]
      AE_MULAFD32X2RA_FIR_H( q0, q1, x0, x1, y0 );
      AE_MULAFD32X2RA_FIR_H( q2, q3, x1, x2, y0 );

      // Q16.47 <- Q16.47 + [ Q31*Q31 - 15] + [ Q31*Q31 - 15 ]
      AE_MULAFD32X2RA_FIR_H( q0, q1, x1, x2, y1 );
      AE_MULAFD32X2RA_FIR_H( q2, q3, x2, x3, y1 );

      // 4 x[] entries are done, shift them out of the registers.
      x0 = x2; x1 = x3;
    }

    //
    // Convert and save (N&3) correlation results.
    //

    R_s = (ae_int32 *)( r + N-1 );
    r0 = AE_ROUND32X2F48SASYM( q0,q0 );
    r1 = AE_ROUND32X2F48SASYM( q1,q1 );
    r2 = AE_ROUND32X2F48SASYM( q2,q2 );
    switch ( N & 3 )
    {
    case 3: AE_S32_L_IP( r2, castxcc(ae_int32,R_s), -4 );
    case 2: AE_S32_L_IP( r1, castxcc(ae_int32,R_s), -4 );
    }
    AE_S32_L_IP( r0, castxcc(ae_int32,R_s), -4 );
  }
}
