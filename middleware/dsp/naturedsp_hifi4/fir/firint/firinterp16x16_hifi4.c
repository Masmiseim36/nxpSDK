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
    Interpolating block real FIR filter, 16x16-bit
    C code optimized for HiFi4
  IntegrIT, 2006-2018
*/

/*-------------------------------------------------------------------------
  Interpolating Block Real FIR Filter
  Computes a real FIR filter (direct-form) with interpolation using IR stored 
  in vector h. The real data input is stored in vector x. The filter output 
  result is stored in vector y. The filter calculates N*D output samples 
  using M*D coefficients and requires last N+M*D-1 samples on the delay line.
  NOTE:
  user application is not responsible for management of delay lines

  Precision: 
  16x16     16-bit data, 16-bit coefficients, 16-bit outputs
  32x16     32-bit data, 16-bit coefficients, 32-bit outputs
  32x32     32-bit data, 32-bit coefficients, 32-bit outputs
  32x32ep   the same as above but using 72-bit accumulator for intermediate 
            computations
  f         floating point

  Input:
  h[M*D]    filter coefficients; h[0] is to be multiplied with the 
            newest sample, Q15, Q31, floating point
  D         interpolation ratio
  N         length of input sample block
  M         length of subfilter. Total length of filter is M*D
  x[N]      input samples, Q15, Q31, floating point
  Output:
  y[N*D]    output samples, Q15, Q31, floating point

  Restrictions:
  x,h,y     should not overlap
  x,h       aligned on an 8-bytes boundary
  N         multiple of 8
  M         multiple of 4
  D         should be >1

  PERFORMANCE NOTE:
  for optimum performance follow rules:
  D - 2, 3 or 4
-------------------------------------------------------------------------*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fir.h"
/* Common utility and macros declarations. */
#include "common.h"
#include "firinterp16x16_common.h"

/* Instance pointer validation number. */
#define MAGIC     0x589FE2D6

/* Reserve memory for alignment. */
#define ALIGNED_SIZE( size, align ) \
    ((size_t)(size)+(align)-1)

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
    (void*)(((uintptr_t)(addr)+((align)-1)) & ~((align)-1))

#define sz_i16    sizeof(int16_t)

typedef struct tag_firinterpf_t
{
    uint32_t        magic;     // Instance pointer validation number
    int             D;         // Interpolation factor
    int             M;         // Number of filter coefficients
    proc_fxn_t    * procFxn;   // Filter data processing function
    const int16_t * coef;      // Filter coefficients
    int16_t *       delayLine; // Delay line for complex samples
    int             delayLen;  // Delay line length, in complex samples
    int             wrIx;      // Index of the oldest sample

} firinterp16x16_t, *firinterp16x16_ptr_t;
/* Calculate the memory block size for an interpolator with given
* attributes. */
size_t firinterp16x16_alloc(int D, int M)
{
    int delayLen, coefNum;
    NASSERT( (D > 1) && (M > 0));

    NASSERT(!(M & 3));
    if (D > 4)
        delayLen = (M + 8);
    else
        delayLen = (M + 4);

    coefNum = (M + 4)*D;

    return (ALIGNED_SIZE(sizeof(firinterp16x16_t), 4)
        + // Delay line
        // Allocated memory for delay line has increased by 2 samples
        // to avoid memory bank conflicts and get the best performance
        ALIGNED_SIZE((delayLen + 2)*sz_i16, 8)
        + // Coefficients
        ALIGNED_SIZE(coefNum*sz_i16, 8));
} // firinterp16x16_alloc()

/* Initialize the interpolator structure. The delay line is zeroed. */
firinterp16x16_handle_t firinterp16x16_init( void * objmem,
                                             int D, int M,
                                       const int16_t * restrict h)
{
    firinterp16x16_ptr_t firinterp;
    void            *    ptr;
    int16_t         *    delLine;
    int                  delLen;
    int16_t         *    coef;
    int16_t         *    coefB;
    int d, m;
    proc_fxn_t      *    procFxn;
    NASSERT(objmem && D > 1 && M > 0 && h);

    NASSERT(!(M & 3) && IS_ALIGN(h));

    if (D > 4)
        delLen = (M + 8);
    else
        delLen = (M + 4);

    procFxn = ( D == 2 ? &firinterp16x16_D2_proc :
                D == 3 ? &firinterp16x16_D3_proc :
                D == 4 ? &firinterp16x16_D4_proc :
                         &firinterp16x16_DX_proc);

    //
    // Partition the memory block.
    //
    ptr         = objmem;
    firinterp   = (firinterp16x16_ptr_t)ALIGNED_ADDR(ptr, 4);
    ptr         = firinterp + 1;
    delLine     = (int16_t*)ALIGNED_ADDR(ptr, 8);
    // Allocated memory for delay line has increased by 2 samples
    // to avoid memory bank conflicts and get the best performance
    ptr         = delLine + delLen + 2;
    coef        = (int16_t*)ALIGNED_ADDR(ptr, 8);
    ptr         = coef + M*D;

    NASSERT((int8_t*)ptr - (int8_t*)objmem <= (int)firinterp16x16_alloc(D, M));


    //
    // Break the impulse response into D coefficients banks and copy them in
    // reverted order.
    //

    for (d = 0; d<D; d++)
    {
        coefB = coef + d*(M + 4);

        // To avoid a 1-sample delay, insert a zero coefficient that will match the
        // oldest sample in the delay line. To keep the filter length a multiple of
        // 4, append 3 zeros after the last coefficient.
        coefB[0] = coefB[M + 1] = coefB[M + 2] = coefB[M + 3] = 0;

        // Copy bank's coefficients in reverted order.
        for (m = 1; m<M + 1; m++)
        {
            coefB[m] = h[D*(M - m) + d];
        }
    }

    //
    // Zero the delay line.
    //

    for (m = 0; m<delLen; m++)
    {
        delLine[m] = 0;
    }

    //
    // Initialize the interpolator instance.
    //

    firinterp->magic      = MAGIC;
    firinterp->D          = D;
    firinterp->M          = M;
    firinterp->coef       = coef;
    firinterp->procFxn    = procFxn;
    firinterp->delayLine  = delLine;
    firinterp->delayLen   = delLen;
    firinterp->wrIx       = 0;

    return (firinterp);
} // firinterp16x16_init()


/* Put a chunk of input signal into the delay line and compute the filter
* response. */
void firinterp16x16_process(firinterp16x16_handle_t  handle,
                             int16_t * restrict      y,
                       const int16_t * restrict      x, int N)
{
    firinterp16x16_ptr_t firinterp = (firinterp16x16_ptr_t)handle;
    NASSERT(firinterp && firinterp->magic == MAGIC && y && x);
    if (N <= 0) return;

    NASSERT(N % 8 == 0);
    NASSERT(IS_ALIGN(x));
    //
    // Call filter's data processing function. It will store the block of input 
    // samples to the delay line, and compute the filter response. Returns the
    // updated next position pointer into the delay line buffer.
    //

    NASSERT(firinterp->procFxn);

    firinterp->wrIx = (*firinterp->procFxn)(
        y,
        firinterp->delayLine,
        firinterp->delayLen,
        x,
        firinterp->coef,
        firinterp->wrIx,
        firinterp->D,
        N,
        firinterp->M);
} // firinterp16x16_process()
