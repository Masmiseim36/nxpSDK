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
    Real block FIR filter, 32x32-bit
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
#define MAGIC     0x87c9e605

/* Reserve memory for alignment. */
#define ALIGNED_SIZE( size, align ) \
      ( (size_t)(size) + (align) - 1 )

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
      (void*)( ( (uintptr_t)(addr) + ( (align) - 1 ) ) & ~( (align) - 1 ) )

#define sz_i32   sizeof(int32_t)

/* Filter instance structure. */
typedef struct tag_bkfir32x32_t
{
  uint32_t        magic;     // Instance pointer validation number
  int             M;         // Number of filter coefficients
  const int32_t * coef;      // M filter coefficients, aligned
  int32_t *       delayLine; // Delay line for samples, aligned
  int             delayLen;  // Delay line length, in samples
  int32_t *       delayPos;  // Delay line slot to be filled next

} bkfir32x32_t, *bkfir32x32_ptr_t;

/* Calculate the memory block size for an FIR filter with given attributes. */
size_t bkfir32x32_alloc( int M )
{
  NASSERT( M > 0 );
  NASSERT( !( M & 3 ) );

  return ( ALIGNED_SIZE( sizeof( bkfir32x32_t ), 4 )
           + // Delay line
           ALIGNED_SIZE( ( M + 4 )*sz_i32, 8 )
           + // Filter coefficients
           ALIGNED_SIZE( ( M + 4 )*sz_i32, 8 ) );

} // bkfir32x32_alloc()

/* Initialize the filter structure. The delay line is zeroed. */
bkfir32x32_handle_t bkfir32x32_init( void * objmem, int M, const int32_t * h )
{
  bkfir32x32_ptr_t bkfir;
  void *           ptr;
  int32_t *        delLine;
  int32_t *        coef;

  int m;

  NASSERT( objmem && M > 0 && h );
  NASSERT( !( M & 3 ) && IS_ALIGN( h ) );

  //
  // Partition the memory block
  //

  ptr     = objmem;
  bkfir   = (bkfir32x32_ptr_t)ALIGNED_ADDR( ptr, 4 );
  ptr     = bkfir + 1;
  delLine = (int32_t *)ALIGNED_ADDR( ptr, 8 );
  ptr     = delLine + M + 4;
  coef    = (int32_t *)ALIGNED_ADDR( ptr, 8 );
  ptr     = coef + M + 4;

  ASSERT( (int8_t*)ptr - (int8_t*)objmem <= (int)bkfir32x32_alloc( M ) );

  //
  // Copy the filter coefficients and zero the delay line. Original impulse
  // response is padded with zeros: three zeros go before the first tap
  // (corresponds to the newest sample), one zero follows the last tap,
  // which matches the oldest sample. After that the order of filter
  // coefficients is reverted.
  //

  coef[0] = 0;

  for ( m=1; m<M+1; m++ )
  {
    coef[m] = h[M-m];
  }

  for ( ; m<M+4; m++ )
  {
    coef[m] = 0;
  }

  for ( m=0; m<M+4; m++ )
  {
    delLine[m] = 0;
  }

  //
  // Initialize the filter instance.
  //

  bkfir->magic     = MAGIC;
  bkfir->M         = M;
  bkfir->coef      = coef;
  bkfir->delayLine = delLine;
  bkfir->delayLen  = M + 4;
  bkfir->delayPos  = delLine;

  return (bkfir);

} // bkfir32x32_init()

/* Put a chunk of input signal into the delay line and compute the filter
 * response. */
void bkfir32x32_process( bkfir32x32_handle_t _bkfir, 
                         int32_t * restrict  y,
                   const int32_t * restrict  x, int N )
{
  bkfir32x32_ptr_t bkfir = (bkfir32x32_ptr_t)_bkfir;

  const ae_f32x2 *            C;
        ae_int32x2 * restrict D_wr;
  const ae_int32x2 *          D_rd0;
  const ae_int32x2 *          D_rd1;
  const ae_int32x2 *          X;
        ae_f32     * restrict Y;

  ae_valign D_va;

  ae_int32x2 t01, t23, t45;
  ae_int32x2 t12, t34, t56;
  ae_f32x2   d01, d23, d45;
  ae_f32x2   d12, d34, d56;
  ae_f32x2   c0, c1;
  ae_f64     q0, q1, q2, q3;
  ae_f64     z;

  int M;
  int m, n;

  NASSERT( bkfir && bkfir->magic == MAGIC && y && x );
  M = bkfir->M;

  NASSERT_ALIGN(( x                                  ),8);
  NASSERT_ALIGN(( bkfir->delayLine                   ),8);
  NASSERT_ALIGN(( bkfir->delayLine + bkfir->delayLen ),8);
  NASSERT_ALIGN(( bkfir->delayPos                    ),8);
  NASSERT_ALIGN(( bkfir->coef                        ),8);

  NASSERT(N%4==0);
  NASSERT(M%4==0);
  if(N<=0) return;
  //
  // Setup pointers and circular delay line buffer.
  //

  D_wr = (      ae_int32x2 *)bkfir->delayPos;
  X    = (const ae_int32x2 *)x;
  Y    = (      ae_f32     *)y;

  WUR_AE_CBEGIN0( (uintptr_t)( bkfir->delayLine                   ) );
  WUR_AE_CEND0  ( (uintptr_t)( bkfir->delayLine + bkfir->delayLen ) );

  z = AE_CVTQ56A32S( 0 );

  //
  // Break the input signal into 4-samples blocks. For each block, store 4
  // samples to the delay line and compute the filter response.
  //

  for ( n=0; n<(N>>2); n++ )
  {
    // Load 4 input samples.
    // Q31
    AE_L32X2_IP( t01, X, +8 );
    AE_L32X2_IP( t23, X, +8 );

    // Store 4 samples to the delay line buffer with circular address update.
    // Q31
    AE_S32X2_XC( t01, D_wr, +8 );
    AE_S32X2_XC( t23, D_wr, +8 );

    // Circular buffer write pointer looks at the oldest sample: M+4 samples
    // back from the newest one. To read the delay buffer separately for
    // even/odd accumulators, the two read pointers are set to M-4th and M-3rd
    // samples. Pointer increment is safe in respect to crossing the circular
    // buffer boundary.
    D_rd0 = D_wr;
    D_rd1 = (ae_int32x2*)( (int32_t*)D_rd0 + 1 );

    AE_LA32X2POS_PC( D_va, D_rd1 );

    // Load two oldest samples for even/odd accumulators.
    // Q31
    AE_L32X2_XC  ( t01, D_rd0, +8 );
    AE_LA32X2_IC( t12, D_va, D_rd1 );

    d01 = ( t01 );
    d12 = ( t12 );

    // Zero the accumulators.
    q0 = z; q1 = z; q2 = z; q3 = z;

    // Reset the coefficients pointer. Now it looks at the tap corresponding
    // to the oldest sample in the delay line.
    C = (const ae_f32x2*)bkfir->coef;

    //
    // Inner loop: process 4 taps for 4 accumulators on each trip. Actually we 
    // perform M+4 MACs for each accumulator, 4 of which fall on zero taps
    // inserted into the impulse response during initialization.
    //

    for ( m=0; m<(M>>2)+1; m++ )
    {
      // Load next two samples for even accumulators. Altogether we have
      // 6 samples residing in 3 AE registers.
      // Q31
      AE_L32X2_XC( t23, D_rd0, +8 );
      AE_L32X2_XC( t45, D_rd0, +8 );

      // Load next two samples for odd accumulators. 
      // Q31
      AE_LA32X2_IC( t34, D_va, D_rd1 );
      AE_LA32X2_IC( t56, D_va, D_rd1 );

      d23 = ( t23 );
      d45 = ( t45 );
      d34 = ( t34 );
      d56 = ( t56 );

      // Load the next 4 tap coefficients.
      // Q31
      ae_f32x2_loadip( c0, C, +8 );
      ae_f32x2_loadip( c1, C, +8 );

      // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
      AE_MULAF32R_HH   ( q0, d01, c0 );
      AE_MULAF32R_LL   ( q0, d01, c0 );
      AE_MULAF32R_HH   ( q0, d23, c1 );
      AE_MULAF32R_LL   ( q0, d23, c1 );

      // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
      AE_MULAF32R_HH   ( q1, d12, c0 );
      AE_MULAF32R_LL   ( q1, d12, c0 );
      AE_MULAF32R_HH   ( q1, d34, c1 );
      AE_MULAF32R_LL   ( q1, d34, c1 );

      // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
      AE_MULAF32R_HH   ( q2, d23, c0 );
      AE_MULAF32R_LL   ( q2, d23, c0 );
      AE_MULAF32R_HH   ( q2, d45, c1 );
      AE_MULAF32R_LL   ( q2, d45, c1 );

      // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
      AE_MULAF32R_HH   ( q3, d34, c0 );
      AE_MULAF32R_LL   ( q3, d34, c0 );
      AE_MULAF32R_HH   ( q3, d56, c1 );
      AE_MULAF32R_LL   ( q3, d56, c1 );

      d01 = d45;
      d12 = d56;
    }

    // Convert and save 8 outputs.
    // 2xQ31 <- 2xQ16.47 - 16 w/ rounding and saturation.
    AE_S32RA64S_IP( q0, Y, +4 );
    AE_S32RA64S_IP( q1, Y, +4 );
    AE_S32RA64S_IP( q2, Y, +4 );
    AE_S32RA64S_IP( q3, Y, +4 );
  }

  bkfir->delayPos = (int32_t*)D_wr;

} // bkfir32x32_process()
