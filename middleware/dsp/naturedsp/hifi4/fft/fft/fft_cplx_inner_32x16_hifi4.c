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
/*          Copyright (C) 2015-2016 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
/*
	NatureDSP_Signal library. FFT part
    Inner stages of complex-valued FFT: 32-bit data, 16-bit twiddle factors
    C code optimized for HiFi4
	IntegrIT, 2006-2014
*/

/* Common utility macros. */
#include "common.h"
/* Inner FFT stages shared between forward and inverse FFT kernels. */
#include "fft_cplx_common.h"

/*------------------------------------------------------------------------------
  Inner radix-4 stages of complex-valued FFT of size 2^n, n=4..12.

  Notes:
    1. Inner means second through the next to last stages.
    2. Data are not permuted.
    3. At each stage data are automatically downscaled to avoid overflows.

  Precision: 
    32x32  32-bit input/output data, 32-bit twiddle factors
    32x16  32-bit input/output data, 16-bit twiddle factors
    16x16  16-bit input/output data, 16-bit twiddle factors

  Input:
    x[2*N]/y[2*N]  complex input data. Real and imaginary data are interleaved 
                   and real data goes first.
                   16x16,32x16:                  input data in y[2*N]
                   32x32, (log2(N)+1)/2 is even: input data in y[2*N]
                   32x32, (log2(N)+1)/2 is odd:  input data in x[2*N]
   twd[2*N*3/4]    complex twiddle factor table

  Output:
    y[2*N]         complex output data. Real and imaginary data are interleaved 
                   and real data goes first.

  Input/Output:
    bexp           common block exponent, that is the minimum number of
                   redundant sign bits over input (output) data

  Returned value: total right shift amount applied to dynamically scale the data

  Restrictions:
    x[],y[],twd[] - must not overlap
    x[],y[],twd[] - must be aligned on 8-byte boundary

------------------------------------------------------------------------------*/

int fft_cplx_inner_32x16( int32_t * restrict y,
                          int32_t * restrict x,
                    const int16_t *          twd,
                          int N )
{
  const ae_int32x2 *          X0;
  const ae_int32x2 *          X1;
  const ae_int32x2 *          X2;
  const ae_int32x2 *          X3;
        ae_int32x2 * restrict Y0;
        ae_int32x2 * restrict Y1;
        ae_int32x2 * restrict Y2;
        ae_int32x2 * restrict Y3;
  const ae_int16x4   *          TWD;

  int m, n;
  int logN, step, stride;
  int isNext2LastInplace;
  int shift, shiftSum;

  NASSERT_ALIGN8( y   );
  NASSERT_ALIGN8( x   );
  NASSERT_ALIGN8( twd );

  NASSERT( N >= 16 && N <= 4096 );

  logN = 30 - NSA( N );
  shiftSum = 0;

  //
  // Inner stages I/O outline:
  //  - first stage: in-place
  //  - second through the second to the last stages: out-of-place
  //  - next to last stage: in-place or out-of-place (see below)
  //  - last stage: out-of-place
  //
  // Next to last stage selects either in-place or out-of-place operation
  // in such a way that the last stage always reads input data from x[] and
  // stores output data to y[].
  //

  isNext2LastInplace = ( ( (logN+1) & 2 ) != 0 );

  //----------------------------------------------------------------------------
  // Perform second through the second to last stages.

  {
    int32_t * X = y;
    int32_t * Y = x;

    ae_int32x2 a0, a1, a2, a3;
    ae_int32x2 b0, b1, b2, b3;
    ae_f32x2   c0, c1, c2, c3;
    ae_f16x4 tw_12, tw_3_;

    shift = 3;
    WUR_AE_SAR( shift );

    for ( stride=N/16, step=4; stride>4; stride/=4, step*=4 )
    {
      shiftSum += shift;

      X0 = (ae_int32x2*)( (uintptr_t)X + 0*stride*8 );
      X1 = (ae_int32x2*)( (uintptr_t)X + 1*stride*8 );
      X2 = (ae_int32x2*)( (uintptr_t)X + 2*stride*8 );
      X3 = (ae_int32x2*)( (uintptr_t)X + 3*stride*8 );

      Y0 = (ae_int32x2*)( (uintptr_t)Y + 0*stride*8 );
      Y1 = (ae_int32x2*)( (uintptr_t)Y + 1*stride*8 );
      Y2 = (ae_int32x2*)( (uintptr_t)Y + 2*stride*8 );
      Y3 = (ae_int32x2*)( (uintptr_t)Y + 3*stride*8 );

      __Pragma( "loop_count min=1" );
      for ( m=0; m*(4*4*stride)<N; m++ )
      {
        TWD = (ae_int16x4*)twd;

        __Pragma( "ymemory( X0 )" );
        __Pragma( "ymemory( X1 )" );
        __Pragma( "ymemory( X2 )" );
        __Pragma( "ymemory( X3 )" );
        __Pragma( "loop_count min=4, factor=4" );
        for ( n=0; n<stride; n++ )
        {
          tw_3_ = AE_L16X4_I( TWD,       2*4 );
          AE_L16X4_XP( tw_12, TWD, +3*step*4 );

          //
          // Group 0
          //

          AE_L32X2_XP( a0, X0, +4*stride*8 );
          AE_L32X2_XP( a1, X1, +4*stride*8 );
          AE_L32X2_XP( a2, X2, +4*stride*8 );
          AE_L32X2_XP( a3, X3, +4*stride*8 );

          AE_ADDANDSUBRNG32( b0, b2, a0, a2 );
          AE_ADDANDSUBRNG32( b1, b3, a3, a1 );

          b3 = AE_MUL32JS( b3 );

          AE_ADDANDSUB32S( c0, c2, b0, b1 );
          AE_ADDANDSUB32S( c1, c3, b2, b3 );

          c1 = AE_MULFC32X16RAS_H( c1, tw_12 );
          c2 = AE_MULFC32X16RAS_L( c2, tw_12 );
          c3 = AE_MULFC32X16RAS_H( c3, tw_3_ );

          AE_S32X2_XP( c0, Y0, +4*stride*8 );
          AE_S32X2_XP( c2, Y1, +4*stride*8 );
          AE_S32X2_XP( c1, Y2, +4*stride*8 );
          AE_S32X2_XP( c3, Y3, +4*stride*8 );

          //
          // Group 1
          //

          AE_L32X2_XP( a0, X0, +4*stride*8 );
          AE_L32X2_XP( a1, X1, +4*stride*8 );
          AE_L32X2_XP( a2, X2, +4*stride*8 );
          AE_L32X2_XP( a3, X3, +4*stride*8 );

          AE_ADDANDSUBRNG32( b0, b2, a0, a2 );
          AE_ADDANDSUBRNG32( b1, b3, a3, a1 );

          b3 = AE_MUL32JS( b3 );

          AE_ADDANDSUB32S( c0, c2, b0, b1 );
          AE_ADDANDSUB32S( c1, c3, b2, b3 );

          c1 = AE_MULFC32X16RAS_H( c1, tw_12 );
          c2 = AE_MULFC32X16RAS_L( c2, tw_12 );
          c3 = AE_MULFC32X16RAS_H( c3, tw_3_ );

          AE_S32X2_XP( c0, Y0, +4*stride*8 );
          AE_S32X2_XP( c2, Y1, +4*stride*8 );
          AE_S32X2_XP( c1, Y2, +4*stride*8 );
          AE_S32X2_XP( c3, Y3, +4*stride*8 );

          //
          // Group 2
          //

          AE_L32X2_XP( a0, X0, +4*stride*8 );
          AE_L32X2_XP( a1, X1, +4*stride*8 );
          AE_L32X2_XP( a2, X2, +4*stride*8 );
          AE_L32X2_XP( a3, X3, +4*stride*8 );

          AE_ADDANDSUBRNG32( b0, b2, a0, a2 );
          AE_ADDANDSUBRNG32( b1, b3, a3, a1 );

          b3 = AE_MUL32JS( b3 );

          AE_ADDANDSUB32S( c0, c2, b0, b1 );
          AE_ADDANDSUB32S( c1, c3, b2, b3 );

          c1 = AE_MULFC32X16RAS_H( c1, tw_12 );
          c2 = AE_MULFC32X16RAS_L( c2, tw_12 );
          c3 = AE_MULFC32X16RAS_H( c3, tw_3_ );

          AE_S32X2_XP( c0, Y0, +4*stride*8 );
          AE_S32X2_XP( c2, Y1, +4*stride*8 );
          AE_S32X2_XP( c1, Y2, +4*stride*8 );
          AE_S32X2_XP( c3, Y3, +4*stride*8 );

          //
          // Group 3
          //

          AE_L32X2_XP( a0, X0, -3*4*stride*8 + 8 );
          AE_L32X2_XP( a1, X1, -3*4*stride*8 + 8 );
          AE_L32X2_XP( a2, X2, -3*4*stride*8 + 8 );
          AE_L32X2_XP( a3, X3, -3*4*stride*8 + 8 );

          AE_ADDANDSUBRNG32( b0, b2, a0, a2 );
          AE_ADDANDSUBRNG32( b1, b3, a3, a1 );

          b3 = AE_MUL32JS( b3 );

          AE_ADDANDSUB32S( c0, c2, b0, b1 );
          AE_ADDANDSUB32S( c1, c3, b2, b3 );

          c1 = AE_MULFC32X16RAS_H( c1, tw_12 );
          c2 = AE_MULFC32X16RAS_L( c2, tw_12 );
          c3 = AE_MULFC32X16RAS_H( c3, tw_3_ );

          AE_S32X2_XP( c0, Y0, -3*4*stride*8 + 8 );
          AE_S32X2_XP( c2, Y1, -3*4*stride*8 + 8 );
          AE_S32X2_XP( c1, Y2, -3*4*stride*8 + 8 );
          AE_S32X2_XP( c3, Y3, -3*4*stride*8 + 8 );
        }
        X0 += 15*stride;
        X1 += 15*stride;
        X2 += 15*stride;
        X3 += 15*stride;

        Y0 += 15*stride;
        Y1 += 15*stride;
        Y2 += 15*stride;
        Y3 += 15*stride;
      }

      {
        // Swap input/output buffers between successive stages.
        int32_t * T = X; X = Y; Y = T;
      }

    }
  }

  __Pragma( "no_reorder" );

  //----------------------------------------------------------------------------
  // Perform the next to last stage.

  {
    ae_int32x2 a0, a1, a2, a3;
    ae_int32x2 b0, b1, b2, b3;
    ae_f32x2   c0, c1, c2, c3;

    TWD = (const ae_int16x4*)( (uintptr_t)twd + 3*step*4 );

    X0 = (ae_int32x2*)( isNext2LastInplace ? y : x );
    Y0 = (ae_int32x2*)y;

    if ( stride == 4 )
    {
      //
      // Next to last stage for FFT size an even power of two.
      //

      ae_f16x4 tw11, tw12;
      ae_f16x4 tw21, tw22;
      ae_f16x4 tw31, tw32;

      shiftSum += shift;

      tw12 = AE_L16X4_I( TWD,       2*4 );
      AE_L16X4_XP( tw11, TWD, +3*step*4 );
      tw22 = AE_L16X4_I( TWD,       2*4 );
      AE_L16X4_XP( tw21, TWD, +3*step*4 );
      tw32 = AE_L16X4_I( TWD,       2*4 );
      AE_L16X4_XP( tw31, TWD, +3*step*4 );

      __Pragma( "loop_count min=4, factor=4" );
      for ( n=0; n<N/(4*4); n++ )
      {
        //
        // Group 0
        //

        AE_L32X2_IP( a0, X0,  +4*8 );
        AE_L32X2_IP( a1, X0,  +4*8 );
        AE_L32X2_IP( a2, X0,  +4*8 );
        AE_L32X2_XP( a3, X0, -11*8 );

        AE_ADDANDSUBRNG32( b0, b2, a0, a2 );
        AE_ADDANDSUBRNG32( b1, b3, a3, a1 );

        b3 = AE_MUL32JS( b3 );

        AE_ADDANDSUB32S( c0, c2, b0, b1 );
        AE_ADDANDSUB32S( c1, c3, b2, b3 );

        AE_S32X2_IP( c0, Y0,  +4*8 );
        AE_S32X2_IP( c2, Y0,  +4*8 );
        AE_S32X2_IP( c1, Y0,  +4*8 );
        AE_S32X2_XP( c3, Y0, -11*8 );

        //
        // Group 1
        //

        AE_L32X2_IP( a0, X0,  +4*8 );
        AE_L32X2_IP( a1, X0,  +4*8 );
        AE_L32X2_IP( a2, X0,  +4*8 );
        AE_L32X2_XP( a3, X0, -11*8 );

        AE_ADDANDSUBRNG32( b0, b2, a0, a2 );
        AE_ADDANDSUBRNG32( b1, b3, a3, a1 );

        b3 = AE_MUL32JS( b3 );

        AE_ADDANDSUB32S( c0, c2, b0, b1 );
        AE_ADDANDSUB32S( c1, c3, b2, b3 );

        c1 = AE_MULFC32X16RAS_H( c1, tw11 );
        c2 = AE_MULFC32X16RAS_L( c2, tw11 );
        c3 = AE_MULFC32X16RAS_H( c3, tw12 );

        AE_S32X2_IP( c0, Y0,  +4*8 );
        AE_S32X2_IP( c2, Y0,  +4*8 );
        AE_S32X2_IP( c1, Y0,  +4*8 );
        AE_S32X2_XP( c3, Y0, -11*8 );

        //
        // Group 2
        //

        AE_L32X2_IP( a0, X0,  +4*8 );
        AE_L32X2_IP( a1, X0,  +4*8 );
        AE_L32X2_IP( a2, X0,  +4*8 );
        AE_L32X2_XP( a3, X0, -11*8 );

        AE_ADDANDSUBRNG32( b0, b2, a0, a2 );
        AE_ADDANDSUBRNG32( b1, b3, a3, a1 );

        b3 = AE_MUL32JS( b3 );

        AE_ADDANDSUB32S( c0, c2, b0, b1 );
        AE_ADDANDSUB32S( c1, c3, b2, b3 );

        c1 = AE_MULFC32X16RAS_H( c1, tw21 );
        c2 = AE_MULFC32X16RAS_L( c2, tw21 );
        c3 = AE_MULFC32X16RAS_H( c3, tw22 );

        AE_S32X2_IP( c0, Y0,  +4*8 );
        AE_S32X2_IP( c2, Y0,  +4*8 );
        AE_S32X2_IP( c1, Y0,  +4*8 );
        AE_S32X2_XP( c3, Y0, -11*8 );

        //
        // Group 3
        //

        AE_L32X2_IP( a0, X0, +4*8 );
        AE_L32X2_IP( a1, X0, +4*8 );
        AE_L32X2_IP( a2, X0, +4*8 );
        AE_L32X2_IP( a3, X0, +1*8 );

        AE_ADDANDSUBRNG32( b0, b2, a0, a2 );
        AE_ADDANDSUBRNG32( b1, b3, a3, a1 );

        b3 = AE_MUL32JS( b3 );

        AE_ADDANDSUB32S( c0, c2, b0, b1 );
        AE_ADDANDSUB32S( c1, c3, b2, b3 );

        c1 = AE_MULFC32X16RAS_H( c1, tw31 );
        c2 = AE_MULFC32X16RAS_L( c2, tw31 );
        c3 = AE_MULFC32X16RAS_H( c3, tw32 );

        AE_S32X2_IP( c0, Y0, +4*8 );
        AE_S32X2_IP( c2, Y0, +4*8 );
        AE_S32X2_IP( c1, Y0, +4*8 );
        AE_S32X2_IP( c3, Y0, +1*8 );
      }

    }
    else if ( stride == 2 )
    {
      //
      // Next to last stage for FFT size an odd power of two.
      //

      ae_f16x4 tw1, tw2;

      shiftSum += shift;

      tw1 = AE_L16X4_I( TWD, 0*4 );
      tw2 = AE_L16X4_I( TWD, 2*4 );

      __Pragma( "loop_count min=4, factor=4" );
      for ( n=0; n<N/(4*2); n++ )
      {
        //
        // Group 0
        //

        AE_L32X2_IP( a0, X0, +2*8 );
        AE_L32X2_IP( a1, X0, +2*8 );
        AE_L32X2_IP( a2, X0, +2*8 );
        AE_L32X2_XP( a3, X0, -5*8 );

        AE_ADDANDSUBRNG32( b0, b2, a0, a2 );
        AE_ADDANDSUBRNG32( b1, b3, a3, a1 );

        b3 = AE_MUL32JS( b3 );

        AE_ADDANDSUB32S( c0, c2, b0, b1 );
        AE_ADDANDSUB32S( c1, c3, b2, b3 );

        AE_S32X2_IP( c0, Y0, +2*8 );
        AE_S32X2_IP( c2, Y0, +2*8 );
        AE_S32X2_IP( c1, Y0, +2*8 );
        AE_S32X2_XP( c3, Y0, -5*8 );

        //
        // Group 1
        //

        AE_L32X2_IP( a0, X0, +2*8 );
        AE_L32X2_IP( a1, X0, +2*8 );
        AE_L32X2_IP( a2, X0, +2*8 );
        AE_L32X2_XP( a3, X0, +1*8 );

        AE_ADDANDSUBRNG32( b0, b2, a0, a2 );
        AE_ADDANDSUBRNG32( b1, b3, a3, a1 );

        b3 = AE_MUL32JS( b3 );

        AE_ADDANDSUB32S( c0, c2, b0, b1 );
        AE_ADDANDSUB32S( c1, c3, b2, b3 );

        c1 = AE_MULFC32X16RAS_H( c1, tw1 );
        c2 = AE_MULFC32X16RAS_L( c2, tw1 );
        c3 = AE_MULFC32X16RAS_H( c3, tw2 );

        AE_S32X2_IP( c0, Y0, +2*8 );
        AE_S32X2_IP( c2, Y0, +2*8 );
        AE_S32X2_IP( c1, Y0, +2*8 );
        AE_S32X2_XP( c3, Y0, +1*8 );
      }

    }
  }

  return (shiftSum);

} // fft_cplx_inner_32x16()
