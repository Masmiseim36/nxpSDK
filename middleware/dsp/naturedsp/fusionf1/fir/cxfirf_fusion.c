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
    Complex block FIR filter, floating point
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
#include "cxfirf.h"

#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(size_t, cxfirf_alloc, (int M))
DISCARD_FUN(cxfirf_handle_t, cxfirf_init, (void *             objmem,
                            int                M,
                            const complex_float * restrict h))
#else

/* Calculate the memory block size for an FIR filter with given attributes. */
size_t cxfirf_alloc( int M )
{
  NASSERT( M > 0 );
  NASSERT(M%4==0);
  return 2*M*sizeof(complex_float) + sizeof(cxfirf_t) + 7;
} // cxfirf_alloc()

/* Initialize the filter structure. The delay line is zeroed. */
cxfirf_handle_t cxfirf_init( void *             objmem, 
                                     int                M, 
                               const complex_float * restrict h )
{
  cxfirf_t* cxfir;
  void *           ptr;
  complex_float * restrict pd;
  complex_float * restrict ph;
  union { struct {float re,im;}s; complex_float c;} zero={{0.f,0.f}};
  int m;
  NASSERT( objmem && M > 0 && h );
  NASSERT(M%4==0);
  NASSERT_ALIGN(h,8);
  /* Partition the memory block */
  ptr     = objmem;
  ph = (complex_float*)((((uintptr_t)ptr)+7)&~7);
  pd = ph+M;
  cxfir=(cxfirf_t*)(pd+M);
  cxfir->M   = M;
  cxfir->h = ph;
  cxfir->d = cxfir->p = pd;
  /* copy coefficients and clean upd delay line */
  for (m=0; m<M; m++) ph[m]=h[m];
  for (m=0; m<M; m++) pd[m]=zero.c;
  return cxfir;
} // cxfirf_init()

#endif
