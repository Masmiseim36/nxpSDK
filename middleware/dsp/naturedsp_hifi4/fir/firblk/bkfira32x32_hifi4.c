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
    Real block FIR filter, 32x32-bit, unaligned data and arbitrary M/N allowed
    C code optimized for HiFi4
	Integrit, 2006-2018
*/

#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fir.h"
#include "common.h"

#include "bkfira32x32_common.h"
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

#define sz_i32   sizeof(int32_t)

/* Allocation routine for filters. Returns: size of memory in bytes to be allocated */
size_t bkfira32x32_alloc( int M )
{
  
  int _M;
  _M = M + (-M&3);
  NASSERT(  M > 0 );

  return ( ALIGNED_SIZE( sizeof( bkfira32x32_t ), 8 )
           + // Delay line
             // Allocated memory for delay line has increased by 2 samples
             // to avoid memory bank conflicts and get the best performance
           ALIGNED_SIZE( ( _M + 4 + 2 )*sz_i32,  8)
           + // Filter coefficients
           ALIGNED_SIZE( (_M+4)*sz_i32, 8 ) );

} // bkfira32x32_alloc()

/* Initialization for filters. Returns: handle to the object */
bkfira32x32_handle_t bkfira32x32_init( void *         objmem, 
                                       int            M, 
                                 const f24 * restrict h )
{
  bkfira32x32_ptr_t bkfir;
  void *           ptr;
  int32_t *        delLine;
  int32_t *        coef;

  int m, _M, n;

  NASSERT( objmem &&  M > 0 && h );
  _M = M + (-M&3);
  //
  // Partition the memory block
  //

  ptr     = objmem;
  bkfir   = (bkfira32x32_ptr_t)ALIGNED_ADDR( ptr, 8 );
  ptr     = bkfir + 1;
  delLine = (int32_t *)ALIGNED_ADDR( ptr, 8 );
  // Allocated memory for delay line has increased by 2 samples
  // to avoid memory bank conflicts and get the best performance
  ptr     = delLine + _M + 4 + 2;
  coef    = (int32_t *)ALIGNED_ADDR( ptr, 8 );
  ptr     = coef + _M + 4;

  ASSERT( (int8_t*)ptr - (int8_t*)objmem <= (int)bkfira32x32_alloc( M ) );

  //
  // Copy the filter coefficients in reverted order and zero the delay line.
  //

  //coef[0] = 0;
  for ( n=0; n<(-M&3)+1; n++ )
  {
    coef[n] = 0;
  }
  for ( m=0; m<M; m++,n++ )
  {
    coef[n] = h[M-m-1];
  }

  for ( m=0; m<3; m++,n++ )
  {
    coef[n] = 0;
  }

  for ( m=0; m<_M+4; m++ )
  {
    delLine[m] = 0;
  }

  //
  // Initialize the filter instance.
  //

  bkfir->magic     = BKFIRA32X32_MAGIC;
  bkfir->M         = _M;
  bkfir->coef      = coef;
  bkfir->delayLine = delLine;
  bkfir->delayLen  = _M+4;
  bkfir->wrIx      = 0;

  return (bkfir);
} // bkfira32x32_init()
