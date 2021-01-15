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
    Complex block FIR filter, 32x32-bit with 72-bit accumulator
    for intermediate computations
    C code optimized for HiFi4
  IntegrIT, 2006-2018
*/


/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fir.h"
/* Common utility and macros declarations. */
#include "common.h"
#include "cxfir32x32ep_common.h"

/*-------------------------------------------------------------------------
  Complex Block FIR Filter
  Computes a complex FIR filter (direct-form) using complex IR stored in 
  vector h. The complex data input is stored in vector x. The filter output
  result is stored in vector r. The filter calculates N output samples using 
  M coefficients and requires last M-1 samples in the delay line. Real and
  imaginary parts are interleaved and real parts go first (at even indexes).
  NOTE: 
  1. User application is not responsible for management of delay lines
  2. User has an option to set IR externally or copy from original location 
     (i.e. from the slower constant memory). In the first case, user is 
     responsible for right alignment, ordering and zero padding of filter 
     coefficients - usually array is composed from zeroes (left padding), 
     reverted IR and right zero padding.

  Precision: 
  16x16     16-bit data, 16-bit coefficients, 16-bit outputs
  32x16     32-bit data, 16-bit coefficients, 32-bit outputs
  32x32     32-bit data, 32-bit coefficients, 32-bit outputs
  32x32ep   the same as above but using 72-bit accumulator for intermediate 
            computations
  f         floating point

  Input:
  h[M]      complex filter coefficients; h[0] is to be multiplied with the 
            newest sample, Q15, Q31, floating point
  x[N]      input samples, Q15, Q31, floating point
  N         length of sample block (in complex samples) 
  M         length of filter 
  extIR     if zero, IR is copied from original location, otherwise not
            but user should keep alignment, order of coefficients 
            and zero padding requirements shown below
  Output:			
  y[N]      output samples, Q15, Q31, floating point

  Alignment, ordering and zero padding for external IR  (extIR!=0)
  -----------------+----------+--------------+--------------+----------------
  Function	       |Alignment,|Left zero     |   Coefficient| Right zero 
                   | bytes    |padding, bytes|   order      | padding, bytes
  -----------------+----------+--------------+--------------+----------------
  cxfir16x16_init, |     8    |    4         |  inverted    |  4
  hifi3            |          |              |              |
  cxfir16x16_init, |     8    |  2 before    |  *           |  6 after
  hifi3z/4         |          |  each copy   |              |  each copy
  cxfir32x16_init  |     8    |    4         |  inverted    |  4
  cxfir32x32_init  |     8    |    0         |  inv,conj    |  0
  cxfir32x32ep_init|     8    |    0         |  inv,conj    |  0
  cxfirf_init      |     8    |    0         |  direct      |  0
  -----------------+----------+--------------+--------------+----------------
  * inverted: conjugated copy and (imaginary; real) copy at 4*(M+4) bytes offset

  Restriction:
  x,y       should not overlap
  x,h       aligned on a 8-bytes boundary
  N,M       multiples of 4
-------------------------------------------------------------------------*/
/* Reserve memory for alignment. */
#define ALIGNED_SIZE( size, align ) \
      ( (size_t)(size) + (align) - 1 )

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
      (void*)( ( (uintptr_t)(addr) + ( (align) - 1 ) ) & ~( (align) - 1 ) )

#define sz_i32   sizeof(int32_t)

/* Calculate the memory block size for an FIR filter with given attributes. */
size_t cxfir32x32ep_alloc( int M, int extIR )
{ 
  NASSERT( M > 0 );
  NASSERT( !( M & 3 ) );
  return ( ALIGNED_SIZE( sizeof( cxfir32x32ep_t ), 4 )
           + // Delay line
           ALIGNED_SIZE( 2*( M + 4 - 1 )*sz_i32, 8 )
           + // Filter coefficients
           (extIR?0:ALIGNED_SIZE( 2*M*sz_i32, 8 )) ); 
} // cxfir32x32ep_alloc()

/* negation -Q31->Q31                            */
inline_ int32_t	L_neg_l (int32_t x)
{
	return (x==MIN_INT32) ? MAX_INT32: -x;
}

/* Initialize the filter structure. The delay line is zeroed. */
cxfir32x32ep_handle_t cxfir32x32ep_init( void *         objmem, 
                                     int            M, 
                                     int            extIR,
                               const complex_fract32 * restrict h )
{
  cxfir32x32ep_ptr_t cxfir;
  void *           ptr;
  int32_t *            coef;
  int32_t *            delLine;

  int m;

  NASSERT( objmem &&  M > 0 && h );
  NASSERT(  !( M & 3 ) && IS_ALIGN( h ) );

  //
  // Partition the memory block.
  //

  ptr     = objmem;
  cxfir   = (cxfir32x32ep_ptr_t)ALIGNED_ADDR( ptr, 4 );
  ptr     = cxfir + 1;
  delLine = (int32_t *)ALIGNED_ADDR( ptr, 8 );
  ptr     = delLine + 2*( M + 4 - 1 );
  if(extIR) coef    = (int32_t *)h;
  else
  {
  coef    = (int32_t *)ALIGNED_ADDR( ptr, 8 );
  ptr     = coef + 2*M;
  }

  NASSERT( (int8_t*)ptr - (int8_t*)objmem <= (int)cxfir32x32ep_alloc( M,extIR ) );

  //
  // Copy the filter coefficients in reverted order and zero the delay line.
  //
  if(extIR==0)
  {
      for ( m=0; m<M; m++ )
      {
        coef[2*m+0] = ((const int32_t*)h)[2*(M-1-m)+0];
        coef[2*m+1] = L_neg_l(((const int32_t*)h)[2*(M-1-m)+1]);    
      }
  }

  for ( m=0; m<M+2-1; m++ )
  {
    delLine[2*m+0] = 0;
    delLine[2*m+1] = 0;
  }

  //
  // Initialize the filter instance.
  //

  cxfir->magic     = CXFIR32X32EP_MAGIC;
  cxfir->M         = M;
  cxfir->coef      = coef;
  cxfir->delayLine = delLine;
  cxfir->delayLen  = M + 2 - 1;
  cxfir->wrIx      = 0;

  return (cxfir);    
} // cxfir32x32ep_init()
