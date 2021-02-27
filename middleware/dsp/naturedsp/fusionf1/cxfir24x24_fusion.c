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
    Complex block FIR filter, 24x24-bit
    C code optimized for Fusion
*/

/*-------------------------------------------------------------------------
  Complex Block FIR Filter
  Computes a complex FIR filter (direct-form) using complex IR stored in 
  vector h. The complex data input is stored in vector x. The filter output
  result is stored in vector r. The filter calculates N output samples using 
  M coefficients and requires last M-1 samples in the delay line. Real and
  imaginary parts are interleaved and real parts go first (at even indexes).
  NOTE: 
  User application is not responsible for management of delay lines

  Precision: 
  16x16     16-bit data, 16-bit coefficients, 16-bit outputs
  24x24     24-bit data, 24-bit coefficients, 24-bit outputs
  32x16     32-bit data, 16-bit coefficients, 32-bit outputs
  32x32     32-bit data, 32-bit coefficients, 32-bit outputs
  f         floating point
  Input:
  h[M] complex filter coefficients; h[0] is to be multiplied with the newest 
       sample , Q31, Q15, floating point
  x[N] input samples , Q15, Q31 or floating point
  N    length of sample block (in complex samples) 
  M    length of filter 
  Output:
  y[N] output samples , Q15, Q31 or floating point

  Restriction:
  x,y - should not overlap
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
#define MAGIC     0xe18b13e6

/* Reserve memory for alignment. */
#define ALIGNED_SIZE( size, align ) \
      ( (size_t)(size) + (align) - 1 )

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
      (void*)( ( (uintptr_t)(addr) + ( (align) - 1 ) ) & ~( (align) - 1 ) )

#define sz_f24   sizeof(f24)

/* Filter instance structure. */
typedef struct tag_cxfir24x24_t
{
  uint32_t    magic;     // Instance pointer validation number
  int         M;         // Number of complex filter coefficients
  const f24 * coef;      // M complex coefficients
  f24 *       delayLine; // Delay line for complex samples
  int         delayLen;  // Delay line length, in complex samples
  f24 *       delayPos;  // Delay line slot to be filled next

} cxfir24x24_t, *cxfir24x24_ptr_t;

/* Calculate the memory block size for an FIR filter with given attributes. */
size_t cxfir24x24_alloc( int M )
{
  ASSERT( M > 0 );

  ASSERT( !( M & 1 ) );

  return ( ALIGNED_SIZE( sizeof( cxfir24x24_t ), 4 )
           + // Delay line
           ALIGNED_SIZE( 2*( M + 3 )*sz_f24, 8 )
           + // Filter coefficients
           ALIGNED_SIZE( 2*M*sz_f24, 8 ) );

} // cxfir24x24_alloc()

/* Initialize the filter structure. The delay line is zeroed. */
cxfir24x24_handle_t cxfir24x24_init( void * objmem, int M, const complex_fract32* restrict h )
{
  cxfir24x24_ptr_t cxfir;
  void *           ptr;
  f24 *            coef;
  f24 *            delLine;

  int m;

  ASSERT( objmem && M > 0 && h );

  ASSERT( !( M & 1 ) );

  //
  // Partition the memory block.
  //

  ptr     = objmem;
  cxfir   = (cxfir24x24_ptr_t)ALIGNED_ADDR( ptr, 4 );
  ptr     = cxfir + 1;
  delLine = (f24*)ALIGNED_ADDR( ptr, 8 );
  ptr     = delLine + 2*( M + 3 );
  coef    = (f24*)ALIGNED_ADDR( ptr, 8 );
  ptr     = coef + 2*M;

  ASSERT( (int8_t*)ptr - (int8_t*)objmem <= (int)cxfir24x24_alloc(  M ) );

  //
  // Copy the filter coefficients in reverted order, and zero the delay line.
  //

  for ( m=0; m<M; m++ )
  {
    coef[2*m+0] = h[(M-1-m)].s.re;
    coef[2*m+1] = h[(M-1-m)].s.im;
  }

  for ( m=0; m<M+3; m++ )
  {
    delLine[2*m+0] = 0;
    delLine[2*m+1] = 0;
  }

  //
  // Initialize the filter instance.
  //

  cxfir->magic     = MAGIC;
  cxfir->M         = M;
  cxfir->coef      = coef;
  cxfir->delayLine = delLine;
  cxfir->delayLen  = M + 3;
  cxfir->delayPos  = delLine;

  return (cxfir);

} // cxfir24x24_init()

/* Put a chunk of input signal into the delay line and compute the filter
 * response. */
void cxfir24x24_process( cxfir24x24_handle_t handle, complex_fract32 * restrict y, const complex_fract32 * restrict x , int N)
{
  cxfir24x24_ptr_t cxfir = (cxfir24x24_ptr_t)handle;

  const ae_f24x2 *          X;
        ae_f24x2 * restrict Y;
  const ae_f24x2 *          C;
        ae_f24x2 * restrict D_wr;
  const ae_f24x2 *          D_rd;

  ae_valign Y_va;

  ae_f64   q0r, q0i, q1r, q1i, q2r, q2i, q3r, q3i;
  ae_f24x2 d0ri, d1ri, d2ri, d3ri, d4ri;
  ae_f24x2 c0ri, c1ri;

  int M;
  int m, n;

  ASSERT( cxfir && cxfir->magic == MAGIC && y && x );

  ASSERT( IS_ALIGN( x ) );

  M = cxfir->M;

  NASSERT ( !( N & 3 ) && !( M & 1 ) );
  NASSERT_ALIGN(( cxfir->delayLine                     ),8);
  NASSERT_ALIGN(( cxfir->delayLine + 2*cxfir->delayLen ),8);
  NASSERT_ALIGN(( cxfir->delayPos                      ),8);
  NASSERT_ALIGN(( cxfir->coef                          ),8);

  //
  // Setup pointers and circular delay line buffer.
  //

  X    = (const ae_f24x2 *)x;
  Y    = (      ae_f24x2 *)y;
  D_wr = (      ae_f24x2 *)cxfir->delayPos;

  WUR_AE_CBEGIN0( (uintptr_t)( cxfir->delayLine                     ) );
  WUR_AE_CEND0  ( (uintptr_t)( cxfir->delayLine + 2*cxfir->delayLen ) );

  Y_va = AE_ZALIGN64();

  //
  // Break the input signal into 4-samples blocks. For each block, store 4
  // samples to the delay line and compute the filter response.
  //

  for ( n=0; n<(N>>2); n++ )
  {
    // Load 4 complex input samples.
    // Q23 <- Q(23+8) - 8
    AE_L32X2F24_IP( d0ri, X, +8 );
    AE_L32X2F24_IP( d1ri, X, +8 );
    AE_L32X2F24_IP( d2ri, X, +8 );
    AE_L32X2F24_IP( d3ri, X, +8 );

    // Store 4 complex samples to the delay line buffer with circular address
    // update.
    // Q(23+8) <- Q23 + 8
    AE_S32X2F24_XC( d0ri, D_wr, +8 );
    AE_S32X2F24_XC( d1ri, D_wr, +8 );
    AE_S32X2F24_XC( d2ri, D_wr, +8 );
    AE_S32X2F24_XC( d3ri, D_wr, +8 );

    // Circular buffer pointer looks at the oldest sample: M+3 samples back from
    // the newest one.
    D_rd = D_wr;

    // Load 3 oldest samples from the delay line.
    // Q23 <- Q(23+8) - 8
    AE_L32X2F24_XC( d0ri, D_rd, +8 );
    AE_L32X2F24_XC( d1ri, D_rd, +8 );
    AE_L32X2F24_XC( d2ri, D_rd, +8 );

    //
    // Inner loop prologue: process the first 2 taps for each of 4 complex
    // accumulators.
    //

    // Load next 2 samples. Altogether we have 5 complex samples residing in
    // 5 AE registers.
    // Q23 <- Q(23+8) - 8
    AE_L32X2F24_XC( d3ri, D_rd, +8 );
    AE_L32X2F24_XC( d4ri, D_rd, +8 );

    // Reset the coefficients pointer. Now it looks at the tap corresponding
    // to the oldest sample in the delay line.
    C = (const ae_f24x2*)cxfir->coef;

    // Load 2 complex tap coefficients.
    // Q23 <- Q(23+8) - 8
    AE_L32X2F24_IP( c0ri, C, +8 );
    AE_L32X2F24_IP( c1ri, C, +8 );

    // Q16.47 <- Q16.47 + [ Q23*Q23 + 1 ] + [ Q23*Q23 + 1 ]
    q0r = AE_MULZASFD24_HH_LL( d0ri, c0ri ); // re*re-im*im
    q1r = AE_MULZASFD24_HH_LL( d1ri, c0ri ); // re*re-im*im

    // Q16.47 <- Q16.47 + [ Q23*Q23 + 1 ] + [ Q23*Q23 + 1 ]
    q0i = AE_MULZAAFD24_HL_LH( d0ri, c0ri ); // re*im+im*re
    q1i = AE_MULZAAFD24_HL_LH( d1ri, c0ri ); // re*im+im*re

    // Q16.47 <- Q16.47 + [ Q23*Q23 + 1 ] + [ Q23*Q23 + 1 ]
    q2r = AE_MULZASFD24_HH_LL( d2ri, c0ri ); // re*re-im*im
    q3r = AE_MULZASFD24_HH_LL( d3ri, c0ri ); // re*re-im*im

    // Q16.47 <- Q16.47 + [ Q23*Q23 + 1 ] + [ Q23*Q23 + 1 ]
    q2i = AE_MULZAAFD24_HL_LH( d2ri, c0ri ); // re*im+im*re
    q3i = AE_MULZAAFD24_HL_LH( d3ri, c0ri ); // re*im+im*re
                                                           
    // Q16.47 <- Q16.47 + [ Q23*Q23 + 1 ] + [ Q23*Q23 + 1 ]
    AE_MULASFD24_HH_LL( q0r, d1ri, c1ri ); // re*re-im*im
    AE_MULASFD24_HH_LL( q1r, d2ri, c1ri ); // re*re-im*im

    // Q16.47 <- Q16.47 + [ Q23*Q23 + 1 ] + [ Q23*Q23 + 1 ]
    AE_MULAAFD24_HL_LH( q0i, d1ri, c1ri ); // re*im+im*re
    AE_MULAAFD24_HL_LH( q1i, d2ri, c1ri ); // re*im+im*re
                                                           
    // Q16.47 <- Q16.47 + [ Q23*Q23 + 1 ] + [ Q23*Q23 + 1 ]
    AE_MULASFD24_HH_LL( q2r, d3ri, c1ri ); // re*re-im*im
    AE_MULASFD24_HH_LL( q3r, d4ri, c1ri ); // re*re-im*im

    // Q16.47 <- Q16.47 + [ Q23*Q23 + 1 ] + [ Q23*Q23 + 1 ]
    AE_MULAAFD24_HL_LH( q2i, d3ri, c1ri ); // re*im+im*re
    AE_MULAAFD24_HL_LH( q3i, d4ri, c1ri ); // re*im+im*re

    // 2 taps are done. Move 2 complex input samples out of the registers file
    d0ri = d2ri; d1ri = d3ri; d2ri = d4ri;

    //
    // Inner loop: process 2 taps for 4 complex accumulators on each trip.
    //

    for ( m=0; m<(M>>1)-1; m++ )
    {
      // Load next 2 samples. Altogether we have 5 complex samples residing in
      // 5 AE registers.
      // Q23 <- Q(23+8) - 8
      AE_L32X2F24_XC( d3ri, D_rd, +8 );
      AE_L32X2F24_XC( d4ri, D_rd, +8 );

      // Load 2 complex tap coefficients.
      // Q23 <- Q(23+8) - 8
      AE_L32X2F24_IP( c0ri, C, +8 );
      AE_L32X2F24_IP( c1ri, C, +8 );

      // Q16.47 <- Q16.47 + [ Q23*Q23 + 1 ] + [ Q23*Q23 + 1 ]
      AE_MULASFD24_HH_LL( q0r, d0ri, c0ri ); // re*re-im*im
      AE_MULASFD24_HH_LL( q1r, d1ri, c0ri ); // re*re-im*im

      // Q16.47 <- Q16.47 + [ Q23*Q23 + 1 ] + [ Q23*Q23 + 1 ]
      AE_MULAAFD24_HL_LH( q0i, d0ri, c0ri ); // re*im+im*re
      AE_MULAAFD24_HL_LH( q1i, d1ri, c0ri ); // re*im+im*re

      // Q16.47 <- Q16.47 + [ Q23*Q23 + 1 ] + [ Q23*Q23 + 1 ]
      AE_MULASFD24_HH_LL( q2r, d2ri, c0ri ); // re*re-im*im
      AE_MULASFD24_HH_LL( q3r, d3ri, c0ri ); // re*re-im*im

      // Q16.47 <- Q16.47 + [ Q23*Q23 + 1 ] + [ Q23*Q23 + 1 ]
      AE_MULAAFD24_HL_LH( q2i, d2ri, c0ri ); // re*im+im*re
      AE_MULAAFD24_HL_LH( q3i, d3ri, c0ri ); // re*im+im*re
                                                             
      // Q16.47 <- Q16.47 + [ Q23*Q23 + 1 ] + [ Q23*Q23 + 1 ]
      AE_MULASFD24_HH_LL( q0r, d1ri, c1ri ); // re*re-im*im
      AE_MULASFD24_HH_LL( q1r, d2ri, c1ri ); // re*re-im*im

      // Q16.47 <- Q16.47 + [ Q23*Q23 + 1 ] + [ Q23*Q23 + 1 ]
      AE_MULAAFD24_HL_LH( q0i, d1ri, c1ri ); // re*im+im*re
      AE_MULAAFD24_HL_LH( q1i, d2ri, c1ri ); // re*im+im*re
                                                             
      // Q16.47 <- Q16.47 + [ Q23*Q23 + 1 ] + [ Q23*Q23 + 1 ]
      AE_MULASFD24_HH_LL( q2r, d3ri, c1ri ); // re*re-im*im
      AE_MULASFD24_HH_LL( q3r, d4ri, c1ri ); // re*re-im*im

      // Q16.47 <- Q16.47 + [ Q23*Q23 + 1 ] + [ Q23*Q23 + 1 ]
      AE_MULAAFD24_HL_LH( q2i, d3ri, c1ri ); // re*im+im*re
      AE_MULAAFD24_HL_LH( q3i, d4ri, c1ri ); // re*im+im*re

      // 2 taps are done. Move 2 complex input samples out of the registers file
      d0ri = d2ri; d1ri = d3ri; d2ri = d4ri;
    }

    // 2xQ23 <- 2xQ16.47 - 24 w/ rounding and saturation.
    d0ri = AE_ROUND24X2F48SASYM( q0r, q0i );
    d1ri = AE_ROUND24X2F48SASYM( q1r, q1i );
    d2ri = AE_ROUND24X2F48SASYM( q2r, q2i );
    d3ri = AE_ROUND24X2F48SASYM( q3r, q3i );

    // Store 4 complex outputs to the output array.
    // 2xQ(23+8) <- 2xQ23 + 8
    AE_SA32X2F24_IP( d0ri, Y_va, Y );
    AE_SA32X2F24_IP( d1ri, Y_va, Y );
    AE_SA32X2F24_IP( d2ri, Y_va, Y );
    AE_SA32X2F24_IP( d3ri, Y_va, Y );
  }

  AE_SA64POS_FP( Y_va, Y );

  // Save current position in the delay line buffer.
  cxfir->delayPos = (f24*)D_wr;

} // cxfir24x24_process()
