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
    Real block FIR filter, 32x16-bit, unaligned data and arbitrary M/N allowed
    C code optimized for HiFi4
  IntegrIT, 2006-2018
*/

                                                         
/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fir.h"
/* Common utility and macros declarations. */
#include "common.h"
#include "bkfira32x16_common.h"

/*-------------------------------------------------------------------------
  Real FIR filter.
  Computes a real FIR filter (direct-form) using IR stored in vector h. The 
  real data input is stored in vector x. The filter output result is stored 
  in vector y. The filter calculates N output samples using M coefficients 
  and requires last M-1 samples in the delay line.
  These functions implement FIR filter with no limitation on size of data
  block, alignment and length of impulse response at the cost of increased
  processing complexity.
  NOTE: 
  User application is not responsible for management of delay lines.

  Precision: 
  16x16    16-bit data, 16-bit coefficients, 16-bit outputs
  32x16    32-bit data, 16-bit coefficients, 32-bit outputs
  32x32    32-bit data, 32-bit coefficients, 32-bit outputs
  32x32ep  the same as above but using 72-bit accumulator for intermediate 
           computations
  f        floating point
  Input:
  x[N]     input samples, Q15, Q31, floating point
  h[M]     filter coefficients in normal order, Q15, Q31, floating point
  N        length of sample block
  M        length of filter
  Output:
  y[N]     input samples, Q15, Q31, floating point 

  Restrictions:
  x,y      should not be overlapping
-------------------------------------------------------------------------*/
/* Reserve memory for alignment. */
#define ALIGNED_SIZE( size, align ) \
      ( (size_t)(size) + (align) - 1 )

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
      (void*)( ( (uintptr_t)(addr) + ( (align) - 1 ) ) & ~( (align) - 1 ) )

#define sz_i16   sizeof(int16_t)
#define sz_i32   sizeof(int32_t)

/* Allocation routine for filters. Returns: size of memory in bytes to be allocated */
size_t bkfira32x16_alloc( int M )
{
  NASSERT(  M > 0 );
  return ( ALIGNED_SIZE( sizeof( bkfira32x16_t ), 4 )
           + // Delay line
           ALIGNED_SIZE( ( M + (-M&3) + 8 )*sz_i32, 8 )
           + // Filter coefficients
           ALIGNED_SIZE( ( M + (-M&3) + 4 )*sz_i16, 8 ) );

} // bkfira32x16_alloc()

/* Initialization for filters. Returns: handle to the object */
bkfira32x16_handle_t bkfira32x16_init( void *              objmem, 
                                       int                 M, 
                                 const int16_t * restrict  h )
{
  bkfira32x16_ptr_t bkfir;
  void *            ptr;
  int32_t *         delLine;
  int16_t *         coef;
  int16_t *         p_coef;

  int m, _M;

  ASSERT( objmem  && M > 0 && h );

  //
  // Partition the memory block
  //

  // Complement the filter length to the next multiple of 4.
  _M = M + (-M&3);

  ptr     = objmem;
  bkfir   = (bkfira32x16_ptr_t)ALIGNED_ADDR( ptr, 4 );
  ptr     = bkfir + 1;
  delLine = (int32_t *)ALIGNED_ADDR( ptr, 8 );
  ptr     = delLine + _M + 8;
  coef    = (int16_t *)ALIGNED_ADDR( ptr, 8 );
  ptr     = coef + _M + 4;

  ASSERT( (int8_t*)ptr - (int8_t*)objmem <= (int)bkfira32x16_alloc( M ) );

  //
  // Convert and copy the filter coefficients. The conversion may be described
  // in three steps:
  //  1. Filter length is extended to the next multiple of 4 by appending a
  //     number of zero coefficients after the last tap, which matches the
  //     oldest sample. This is done to ensure that the delay line buffer size
  //     is always a multiple of 8 bytes. New filter length is _M.
  //  2. Pad the impulse response with 4 zeros. 3 zeros go before the first tap,
  //     and 1 zero follows the last tap. The intention here is to eliminate a 
  //     one-sample delay that would otherwise be introduced into the filter
  //     response, because we use _M+8 delay elements instead of _M+8-1 (8 is
  //     the basic samples block length).
  //  3. Finally, the coefficients order is reverted, and they are copied into
  //     the internal storage.
  //

  p_coef = coef;

  for ( m=0; m<(-M&3)+1; m++ )
  {
    *p_coef++ = 0;
  }

  for ( m=0; m<M; m++ )
  {
    *p_coef++ = h[M-1-m];
  }

  for ( m=0; m<3; m++ )
  {
    *p_coef++ = 0;
  }

  //
  // Zero the delay line.
  //

  for ( m=0; m<_M+8; m++ )
  {
    delLine[m] = 0;
  }

  //
  // Initialize the filter instance.
  //

  bkfir->magic     = BKFIRA32X16_MAGIC;
  bkfir->M         = _M;
  bkfir->coef      = coef;
  bkfir->delayLine = delLine;
  bkfir->delayLen  = _M+8;
  bkfir->wrIx      = 0;

  return (bkfir);


} // bkfira32x16_init()

