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
    Interpolating block real FIR filter, floating point
    C code optimized for Fusion
*/

/*-------------------------------------------------------------------------
  Interpolating Block Real/Complex FIR Filter
  Computes a real FIR filter (direct-form) with interpolation using IR stored 
  in vector h. The real data input is stored in vector x. The filter output 
  result is stored in vector y. The filter calculates N*D output samples 
  using M*D coefficients and requires last N+M*D-1 samples on the delay line.
  NOTE:
  user application is not responsible for management of delay lines

  Precision: 
  16x16     16-bit real data, 16-bit coefficients, 16-bit real outputs 
  16x16     16-bit complex data, 16-bit coefficients, 16-bit complex outputs
  24x24     24-bit real data, 24-bit coefficients, 24-bit real outputs 
  32x16     32-bit real data, 16-bit coefficients, 32-bit real outputs 
  32x32     32-bit real data, 32-bit coefficients, 32-bit real outputs 
  f         floating point

  Input:
  h[M*D]        filter coefficients; h[0] is to be multiplied with the 
                newest sample,Q31, Q15, floating point
  D             interpolation ratio
  N             length of input sample block
  M             length of subfilter. Total length of filter is M*D
  x[N]          input samples,Q15, Q31 or floating point
  Output:
  y[N*D]        output samples, Q15, Q31 or floating point

  Restrictions:
  x,h,y should not overlap
  x,h - aligned on an 8-bytes boundary
  N   - multiple of 8
  M   - multiple of 4
  D should be >1

  PERFORMANCE NOTE:
  for optimum performance follow rules:
  D   - 2, 3 or 4

-------------------------------------------------------------------------*/
/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common utility and macros declarations. */
#include "common.h"
#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(size_t, firinterpf_alloc, (int D, int M))
DISCARD_FUN(firinterpf_handle_t, firinterpf_init, (void * objmem,
                          int D, int M,
                          const float32_t * restrict h))
DISCARD_FUN(void, firinterpf_process, (firinterpf_handle_t _firinterp,
                  float32_t * restrict      y,
                  const float32_t * restrict      x, int N))
#else

#include "fir_interpf_2x.h"
#include "fir_interpf_3x.h"
#include "fir_interpf_4x.h"
#include "fir_interpf_Dx.h"

/* Interpolator instance structure. */
typedef struct tag_firinterpf_t
{
  int               M; /* Filter length                   */
  int               D; /* Interpolation/decimation factor */
  const float32_t * h; /* Filter coefficients (M*D)       */
        float32_t * d; /* Delay line of length (M*D)      */
        float32_t * p; /* Pointer into the delay line     */

} firinterpf_t;

/* Calculate the memory block size for an interpolator with given 
 * attributes. */
size_t firinterpf_alloc( int D, int M )
{
  NASSERT( D > 1 && M > 0 );
  NASSERT( M%4==0);
  return M*(D+1)*sizeof(float32_t) + sizeof(firinterpf_t) + 7;
} // firinterpf_alloc()

/* Initialize the interpolator structure. The delay line is zeroed. */
firinterpf_handle_t firinterpf_init( void * objmem, 
                                     int D, int M, 
                                     const float32_t * restrict h )
{
    firinterpf_t* firinterp;
    void *           ptr;
    float32_t * restrict pd;
    float32_t * restrict ph;
    int m,d;
    NASSERT( D > 1 && M > 0 );
    NASSERT( M%4==0);
    NASSERT(h);
    NASSERT(objmem);
    NASSERT_ALIGN(h,8);
    /* partition the memblock */
    ptr     = objmem;
    ph = (float32_t*)((((uintptr_t)ptr)+7)&~7);
    pd = ph+M*D;
    firinterp=(firinterpf_t*)(pd+M);
    firinterp->M   = M;
    firinterp->D   = D;
    firinterp->h = ph;
    firinterp->d = firinterp->p = pd;
    /* copy/rearrange coefficients and clean up delay line */
    for (m=0; m<M; m++) 
    for (d=0; d<D; d++) 
        ph[d*M+m]=h[m*D+d]*D;
    for (m=0; m<M; m++) pd[m]=0.f;
    return firinterp;

} // firinterpf_init()

/* Put a chunk of input signal into the delay line and compute the filter
 * response. */
void firinterpf_process( firinterpf_handle_t _firinterp, 
                             float32_t * restrict      y,
                       const float32_t * restrict      x, int N )
{
    typedef float32_t* (*fn_interp)(float32_t *, float32_t *, float32_t *, const float32_t *, const float32_t *, int, int);
    static const fn_interp fn[3] = { &fir_interpf_2x, &fir_interpf_3x, &fir_interpf_4x };

    firinterpf_t *state;
    int M, D;
    float32_t* d;
    float32_t* p;
    const float32_t* h;

    NASSERT(_firinterp);
    state=(firinterpf_t *)_firinterp;
    NASSERT(x);
    NASSERT(y);
    NASSERT(state);
    NASSERT_ALIGN(state->d,8);
    M = state->M;
    D = state->D;
    if(N<=0) return;
    NASSERT(N>0);
    NASSERT(M>0);
    NASSERT(D>1);
    NASSERT(M%4==0);
    h = state->h;
    p = state->p;
    d = state->d;
    if (D <= 4)
    {
    p = fn[D - 2](y, d, p, h, x, M, N);
    }
    else
    {
    p = fir_interpf_Dx(y, d, p, h, x, M, D, N);
    }
    state->p = p;
} // firinterpf_process()

#endif

