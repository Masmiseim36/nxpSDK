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
    Real block FIR filter, floating point
    C code optimized for Fusion
*/

/*-------------------------------------------------------------------------
  Real FIR filter.
  Computes a real FIR filter (direct-form) using IR stored in vector h. The 
  real data input is stored in vector x. The filter output result is stored 
  in vector y. The filter calculates N output samples using M coefficients 
  and requires last M-1 samples in the delay line.
  These functions implement FIR filter described in previous chapter with no 
  limitation on size of data block, alignment and length of impulse response 
  for the cost of performance.
  NOTE: 
  User application is not responsible for management of delay lines

  Precision: 
  16x16    16-bit data, 16-bit coefficients, 16-bit outputs
  24x24    24-bit data, 24-bit coefficients, 24-bit outputs
  32x16    32-bit data, 16-bit coefficients, 32-bit outputs
  f        floating point
  Input:
  x[N]      - input samples, Q31, floating point
  h[M]      - filter coefficients in normal order, Q31, Q15, floating point
  N         - length of sample block
  M         - length of filter
  Output:
  y[N]      - input samples, Q31, floating point 

  Restrictions:
  x,y should not be overlapping
-------------------------------------------------------------------------*/
/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common utility and macros declarations. */
#include "common.h"
#include "bkfiraf.h"

#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(size_t, bkfiraf_alloc, (int M))
DISCARD_FUN(bkfirf_handle_t, bkfiraf_init, (void * objmem, int M, const float32_t * h))
#else

/* Calculate the memory block size for an FIR filter with given attributes. */
size_t bkfiraf_alloc( int M )
{
  NASSERT( M > 0 );
  M=(M+3)&~3;
  NASSERT(M%4==0);
  return 2*M*sizeof(float32_t) + sizeof(bkfiraf_t) + 7;
} // bkfiraf_alloc()

/* Initialize the filter structure. The delay line is zeroed. */
bkfirf_handle_t bkfiraf_init( void * objmem, int M, const float32_t * h )
{
  bkfiraf_t* bkfir;
  void *           ptr;
  int M1=(M+3)&~3;
  float32_t * restrict pd;
  float32_t * restrict ph;
  int m;
  NASSERT( objmem && M > 0 && h );
  NASSERT(M1%4==0);
  /* Partition the memory block */
  ptr     = objmem;
  ph = (float32_t*)((((uintptr_t)ptr)+7)&~7);
  pd = ph+M1;
  bkfir=(bkfiraf_t*)(pd+M1);
  bkfir->M   = M1;
  bkfir->h = ph;
  bkfir->d = bkfir->p = pd;
  /* copy coefficients and clean upd delay line */
  for (m=0; m<M; m++) ph[m]=h[m];
  for (; m<M1; m++) ph[m]=0.f;  /* pad remaining coefficients with zeroes */
  for (m=0; m<M1; m++) pd[m]=0.f;
  return bkfir;
} // bkfiraf_init()
#endif
