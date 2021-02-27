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
    Decimating block real FIR filter, floating point
    C code optimized for Fusion
*/

/*-------------------------------------------------------------------------
  Decimating Block Real FIR Filter
  Computes a real FIR filter (direct-form) with decimation using IR stored 
  in vector h. The real data input is stored in vector x. The filter output 
  result is stored in vector r. The filter calculates N output samples using
  M coefficients and requires last D*N+M-1 samples on the delay line.
  NOTE:
  - To avoid aliasing IR should be synthesized in such a way to be narrower 
    than input sample rate divided to 2D.
  - user application is not responsible for management of delay lines

  Precision: 
  24x24     24-bit data, 24-bit coefficients, 24-bit outputs
  32x16     32-bit data, 16-bit coefficients, 32-bit outputs
  f         floating point

  Input:
  h[M]          filter coefficients; h[0] is to be multiplied with the newest 
                sample, Q31, Q15, floating point
  D             decimation factor 
  N             length of output sample block
  M             length of filter
  x[D*N]        input samples , Q15, Q31 or floating point
  Output:
  y[N]          output samples, Q15, Q31 or floating point

  Restriction:
  x,h,r should not overlap
  x, h - aligned on an 8-bytes boundary
  N - multiple of 8
  D should exceed 1

  PERFORMANCE NOTE:
  for optimum performance follow rules:
  D - 2, 3 or 4
-------------------------------------------------------------------------*/
/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common utility and macros declarations. */
#include "common.h"
#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(size_t, firdecf_alloc, (int D, int M))
DISCARD_FUN(firdecf_handle_t, firdecf_init, (void * objmem, int D,
int M, const float32_t * restrict h))
DISCARD_FUN(void, firdecf_process, (firdecf_handle_t _firdec,
                  float32_t * restrict       y,
                  const float32_t *                x, int N))
#else
#include "fir_decimaf_2x.h"
#include "fir_decimaf_3x.h"
#include "fir_decimaf_4x.h"
#include "fir_decimaf_Dx.h"

/* Decimator instance structure. */
typedef struct 
{
  int               M; /* Filter length                   */
  int               D; /* Interpolation/decimation factor */
  const float32_t * h; /* Filter coefficients             */
        float32_t * d; /* Delay line of length M          */
        float32_t * p; /* Pointer into the delay line     */
} firdecf_t;

/* Calculate the memory block size for a decimator with given attributes. */
size_t firdecf_alloc( int D, int M )
{
  NASSERT( D > 1 && M > 0 );
  M=(M+1)&~1;
  return sizeof(firdecf_t)+2*M*sizeof(float32_t)+7;

} // firdecf_alloc()

/* Initialize the decimator structure. The delay line is zeroed. */
firdecf_handle_t firdecf_init( void * objmem, int D, 
                               int M, const float32_t * restrict h )
{
    firdecf_t* firdec;
    void *           ptr;
    int M1=(M+1)&~1;
    float32_t * restrict pd;
    float32_t * restrict ph;
    int m;
    NASSERT( objmem && D > 1 && M > 0 && h );
    NASSERT_ALIGN( h,8 );
    NASSERT(h);
    /* partition the memblock */
    ptr     = objmem;
    ph = (float32_t*)((((uintptr_t)ptr)+7)&~7);
    pd = ph+M1;
    firdec=(firdecf_t*)(pd+M1);
    firdec->M   = M1;
    firdec->D   = D;
    firdec->h = ph;
    firdec->d = firdec->p = pd;
    /* copy coefficients and clean up delay line */
    for (m=0; m<M; m++) ph[m]=h[m];
    for (;m<M1; m++) ph[m]=0.f;
    for (m=0; m<M1; m++) pd[m]=0.f;
    return firdec;
} // firdecf_init()

/* process block of samples */
void firdecf_process( firdecf_handle_t _firdec, 
                          float32_t * restrict       y, 
                    const float32_t *                x, int N )
{
    typedef float32_t* (*fn_decima)(float32_t * , float32_t * , float32_t * , const float32_t * , const float32_t * ,  int , int );
    static const fn_decima fn[3]={&fir_decimaf_2x,&fir_decimaf_3x,&fir_decimaf_4x};
    firdecf_t *state;
    float32_t* p;
    float32_t* d;
    const float32_t* h;
    int M,D;

    NASSERT(_firdec);
    NASSERT_ALIGN(x,8);
    NASSERT(N%8==0);
    NASSERT(x);
    NASSERT(y);
    if(N<=0) return;
    state = (firdecf_t*)_firdec;
    NASSERT(state->h);
    NASSERT(state->d);
    NASSERT(state->p);
    NASSERT_ALIGN(state->d,8);
    M=state->M;
    D=state->D;
    NASSERT(N>0);
    NASSERT(M>0);
    NASSERT(D>1);
    NASSERT(M%2==0);
    h=state->h;
    p=state->p;
    d=state->d;
    //N=N/D;
    if (D<=4) 
    {
        p=fn[D-2](y,d, p, h, x, M, N); 
    }
    else
    {
        p=fir_decimaf_Dx(y,d, p, h, x, M, D, N); 
    }
    state->p=p;
} // firdecf_process()

#endif
