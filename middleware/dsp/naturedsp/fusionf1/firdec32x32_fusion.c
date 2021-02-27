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
    Decimating block real FIR filter, 32x32-bit
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

#include "firdec32x32_common.h"

/* Instance pointer validation number. */
#define MAGIC     0x3dd08a15

/* Reserve memory for alignment. */
#define ALIGNED_SIZE( size, align ) \
    ((size_t)(size)+(align)-1)

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
    (void*)(((uintptr_t)(addr)+((align)-1)) & ~((align)-1))

#define sz_i32    sizeof(int32_t)

/* Decimator instance structure. */
typedef struct tag_firdec32x32_t
{
    uint32_t        magic;     // Instance pointer validation number
    int             D;         // Decimation factor
    int             M;         // length of filter
    const int32_t * coef;      // Filter coefficients
    proc_fxn_t *    procFxn;   // Filter data processing function
    int32_t *       delayLine; // Delay line
    int             delayLen;  // Delay line length, in samples
    int             wrIx;      // Index of the oldest sample

} firdec32x32_t, *firdec32x32_ptr_t;

/* Allocation routine for decimators. returns: size of memory in bytes to be allocated */
size_t firdec32x32_alloc(int D, int M)
{
    int bM;
    int delayLen, coefNum;
    NASSERT(D > 1 && M > 0);

    bM = M + (-M & 3);
    coefNum = bM + 4;
    delayLen = bM + 4 * D;

    return (ALIGNED_SIZE(sizeof(firdec32x32_t), 4)
        + // Delay line
        ALIGNED_SIZE((delayLen)*sz_i32, 8)
        + // Coefficients
        ALIGNED_SIZE(coefNum*sz_i32, 8));
} // firdec32x32_alloc()

/* Initialization of decimator. Returns: handle to the decimator object */
firdec32x32_handle_t firdec32x32_init(void * objmem, int D,
    int M, const int32_t * restrict h)
{
    firdec32x32_ptr_t firdec;
    void *            ptr;
    int32_t *         delLine;
    int               delLen;
    int               coefNum;
    int32_t *         coef;
    int32_t *         coefB;
    proc_fxn_t *      procFxn;

    int bM;
    int m;

    NASSERT(objmem && D > 1 && M > 0 && h);

    NASSERT(IS_ALIGN(h));

    //
    // Select the processing function, delay line length and coefficients
    // block layout.
    //

    bM = M + (-M & 3);

    coefNum = bM + 4;
    delLen = bM + 4 * D;
    procFxn = (D == 2 ? &fir32x32_D2_proc :
        D == 3 ? &fir32x32_D3_proc :
        D == 4 ? &fir32x32_D4_proc :
        &fir32x32_DX_proc);
    //
    // Partition the memory block.
    //

    ptr = objmem;
    firdec = (firdec32x32_ptr_t)(ALIGNED_ADDR(ptr, 4));
    ptr = firdec + 1;
    delLine = (int32_t*)ALIGNED_ADDR(ptr, 8);
    ptr = delLine + delLen;
    coef = (int32_t*)ALIGNED_ADDR(ptr, sz_i32);
    ptr = coef + coefNum;

    NASSERT((int8_t*)ptr - (int8_t*)objmem <= (int)firdec32x32_alloc(D, M));

    //
    // Copy the filter coefficients in reverted order and zero the delay line.
    //

    coefB = coef;

    // Begin by a few zeros to make the number of coefficients a multiple of 4.
    for (m = M; m<bM; m++)
    {
        *coefB++ = 0;
    }

    // Pad the coefficients with 4 zeros (one at the beginning, three following 
    // the end) to avoid a 1-sample delay of filter response.
    coefB[0] = coefB[M + 1] = coefB[M + 2] = coefB[M + 3] = 0;

    // Copy coefficients in reverted order.
    for (m = 1; m <= M; m++)
    {
        coefB[m] = h[M - m];
    }

    //
    // Zero the delay line.
    //

    for (m = 0; m<delLen; m++)
    {
        delLine[m] = 0;
    }

    firdec->magic = MAGIC;
    firdec->D = D;
    firdec->M = bM;
    firdec->procFxn = procFxn;
    firdec->coef = coef;
    firdec->delayLine = delLine;
    firdec->delayLen = delLen;
    firdec->wrIx = 0;

    return (firdec);
} // firdec32x32_init()

void firdec32x32_process( firdec32x32_handle_t _firdec,
                          int32_t * restrict       y,
                    const int32_t *                x, int N)
{
    firdec32x32_ptr_t firdec = (firdec32x32_ptr_t)_firdec;

    if (N <= 0) return;
    NASSERT(N % 8 == 0);

    //
    // Call filter's data processing function. It will store the block of input 
    // samples to the delay line, and compute the filter response. Returns the
    // updated next position pointer into the delay line buffer.
    //

    NASSERT(firdec->procFxn);

    firdec->wrIx = (*firdec->procFxn)(
        y,
        firdec->delayLine,
        firdec->delayLen,
        x,
        firdec->coef,
        firdec->wrIx,
        firdec->D,
        N,
        firdec->M);
} // firdec32x32_process()
