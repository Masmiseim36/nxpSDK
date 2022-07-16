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
    Complex block FIR filter, 32x32-bit
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
#define MAGIC     0x468AACEE

/* Reserve memory for alignment. */
#define ALIGNED_SIZE( size, align ) \
    ((size_t)(size)+(align)-1)

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
    (void*)(((uintptr_t)(addr)+((align)-1)) & ~((align)-1))

#define sz_i32   sizeof(int32_t)

/* Filter instance structure. */
typedef struct tag_cxfir32x32_t
{
    uint32_t        magic;     // Instance pointer validation number
    int             M;         // Number of complex filter coefficients
    const int32_t * coef;      // M complex coefficients
    int32_t *       delayLine; // Delay line for complex samples
    int             delayLen;  // Delay line length, in complex samples
    int             wrIx;      // Index of the oldest complex sample

} cxfir32x32_t, *cxfir32x32_ptr_t;

/* negation -Q31->Q31                            */
inline_ int32_t	L_neg_l(int32_t x)
{
    return (x == MIN_INT32) ? MAX_INT32 : -x;
}

/* Allocation routine for filters. Returns: size of memory in bytes to be allocated */
size_t cxfir32x32_alloc(int M)
{
    NASSERT(M > 0);
    NASSERT(!(M & 3));
    return (ALIGNED_SIZE(sizeof(cxfir32x32_t), 4)
        + // Delay line
        ALIGNED_SIZE(2 * (M + 4 - 1)*sz_i32, 8)
        + // Filter coefficients
        ALIGNED_SIZE(2 * M*sz_i32, 8) );
} // cxfir32x32_alloc()

/* Initialization for filters. Returns: handle to the object */
cxfir32x32_handle_t cxfir32x32_init( void            * objmem,
                                     int               M,
                               const complex_fract32 * restrict h)
{
    cxfir32x32_ptr_t cxfir;
    void    *        ptr;
    int32_t *        coef;
    int32_t *        delLine;

    int m;

    NASSERT(objmem && M > 0 && h);
    NASSERT(!(M & 3) && IS_ALIGN(h));

    //
    // Partition the memory block.
    //

    ptr     = objmem;
    cxfir   = (cxfir32x32_ptr_t)ALIGNED_ADDR(ptr, 4);
    ptr     = cxfir + 1;
    delLine = (int32_t *)ALIGNED_ADDR(ptr, 8);
    ptr     = delLine + 2 * (M + 4 - 1);

    coef = (int32_t *)ALIGNED_ADDR(ptr, 8);
    ptr = coef + 2 * M;

    NASSERT((int8_t*)ptr - (int8_t*)objmem <= (int)cxfir32x32_alloc(M));

    //
    // Copy the filter coefficients in reverted order and zero the delay line.
    //    
    for (m = 0; m<M; m++)
    {
        coef[2 * m + 0] = h[(M - 1 - m)].s.re;
        coef[2 * m + 1] = L_neg_l(h[(M - 1 - m)].s.im);
    }
    

    for (m = 0; m<M + 2 - 1; m++)
    {
        delLine[2 * m + 0] = 0;
        delLine[2 * m + 1] = 0;
    }

    //
    // Initialize the filter instance.
    //

    cxfir->magic        = MAGIC;
    cxfir->M            = M;
    cxfir->coef         = coef;
    cxfir->delayLine    = delLine;
    cxfir->delayLen     = M + 2 - 1;
    cxfir->wrIx         = 0;

    return (cxfir);
} // cxfir32x32_init()

void cxfir32x32_process( cxfir32x32_handle_t _cxfir,
                         complex_fract32 * restrict  y,
                   const complex_fract32 * restrict  x, int N)
{
    cxfir32x32_ptr_t cxfir = (cxfir32x32_ptr_t)_cxfir;

    const   ae_f32x2 *            C;
    ae_int32x2 * restrict D_wr;
    const ae_int32x2 *          D_rd0;
    const ae_int32x2 *          D_rd1;
    const ae_int32x2 *          X;
    ae_f32     * restrict Y;
    ae_valign D_va;

    ae_int32x2 t0, t1, t2, t3;
    ae_f32x2   d0, d1;
    ae_int32x2 c0, c1;
    ae_f64     q0r, q1r;
    ae_f64     q0i, q1i;
    ae_f64     z;


    int M, wrIx;
    int m, n;

    if (N <= 0) return;
    NASSERT(cxfir && cxfir->magic == MAGIC && y && x);
    NASSERT_ALIGN8(x);

    M = cxfir->M;
    wrIx = cxfir->wrIx;
    NASSERT(!(M & 3) && !(N & 3));
    NASSERT_ALIGN8(cxfir->delayLine);
    NASSERT_ALIGN8(cxfir->coef);

    //
    // Setup pointers and circular delay line buffer.
    //

    D_wr = (ae_int32x2*)(cxfir->delayLine + 2 * wrIx);
    X = (const ae_int32x2 *)x;
    Y = (ae_f32     *)y;

    WUR_AE_CBEGIN0((uintptr_t)(cxfir->delayLine));
    WUR_AE_CEND0((uintptr_t)(cxfir->delayLine + 2 * cxfir->delayLen));
    z = AE_ZERO64();    

    //
    // Break the input signal into 4-samples blocks. For each block, store 4
    // samples to the delay line and compute the filter response.
    //
    __Pragma("loop_count min=1")
    for (n = 0; n<(N >> 1); n++)
    {
        // Reset the coefficients pointer. Now it looks at the tap corresponding
        // to the oldest sample in the delay line.
        C = (const ae_f32x2*)cxfir->coef;

        // Load 4 input samples.
        // Q31
        AE_L32X2_IP(t0, X, +8);
        AE_L32X2_IP(t1, X, +8);

        // Store 4 samples to the delay line buffer with circular address update.
        // Q31
        AE_S32X2_XC(t0, D_wr, +8);
        AE_S32X2_XC(t1, D_wr, +8);

        // Circular buffer write pointer looks at the oldest sample: M+4 samples
        // back from the newest one. To read the delay buffer separately for
        // even/odd accumulators, the two read pointers are set to M-4th and M-3rd
        // samples. Pointer increment is safe in respect to crossing the circular
        // buffer boundary.
        D_rd0 = D_wr;
        // Load two oldest samples for even/odd accumulators.
        // Q31
        AE_L32X2_XC(t0, D_rd0, +8);
        D_rd1 = D_rd0;
        AE_L32X2_XC(t1, D_rd0, +8);
        AE_LA32X2POS_PC(D_va, D_rd1);

        d0 = (t0);
        d1 = (t1);

        // Zero the accumulators.
        q0r = z; q1r = z;
        q0i = z; q1i = z;

        //
        // Inner loop: process 4 taps for 4 accumulators on each trip. Actually we 
        // perform M+4 MACs for each accumulator, 4 of which fall on zero taps
        // inserted into the impulse response during initialization.
        //
        __Pragma("loop_count min=2")
        for (m = 0; m<(M >> 1); m++)
        {
            // Load next two samples for even accumulators. Altogether we have
            // 4 samples residing in 8 AE registers.
            // Q31
            AE_L32X2_XC(t2, D_rd0, +8);
            AE_L32X2_XC(t3, D_rd0, +8);

            // Load the next 2 tap coefficients.
            // Q31
            AE_L32X2_IP(c0, C, +8);
            AE_L32X2_IP(c1, C, +8);

            // Q17.47 <- QQ17.47 + [ Q31*Q31 ] - 15 with asymmetric rounding
            AE_MULAF32R_HH(q0r, t0, c0);
            AE_MULAF32R_LL(q0r, t0, c0);
            AE_MULAF32R_HH(q0r, t1, c1);
            AE_MULAF32R_LL(q0r, t1, c1);

            AE_MULAF32R_HH(q1r, t1, c0);
            AE_MULAF32R_LL(q1r, t1, c0);
            AE_MULAF32R_HH(q1r, t2, c1);
            AE_MULAF32R_LL(q1r, t2, c1);

            AE_MULSF32R_LH(q0i, c0, t0);
            AE_MULAF32R_LH(q0i, t0, c0);
            AE_MULSF32R_LH(q0i, c1, t1);
            AE_MULAF32R_LH(q0i, t1, c1);

            AE_MULSF32R_LH(q1i, c0, t1);
            AE_MULAF32R_LH(q1i, t1, c0);
            AE_MULSF32R_LH(q1i, c1, t2);
            AE_MULAF32R_LH(q1i, t2, c1);

            t0 = t2;
            t1 = t3;
        }
        // Convert and save 4 outputs.

        AE_S32RA64S_IP(q0r, Y, +4);
        AE_S32RA64S_IP(q0i, Y, +4);
        AE_S32RA64S_IP(q1r, Y, +4);
        AE_S32RA64S_IP(q1i, Y, +4);
    }

    cxfir->wrIx = (int)((int32_t *)D_wr - cxfir->delayLine) >> 1;

} // cxfir32x32_process()
