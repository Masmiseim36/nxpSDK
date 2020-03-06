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
    Real block FIR filter, 16x16-bit, unaligned data and arbitrary M/N allowed
    C code optimized for HiFi4
  IntegrIT, 2006-2018
*/

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

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fir.h"
/* Common utility and macros declarations. */
#include "common.h"

/* Instance pointer validation number. */
#define MAGIC     0x29EDBF10

/* Reserve memory for alignment. */
#define ALIGNED_SIZE( size, align ) \
    ((size_t)(size)+(align)-1)

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
    (void*)(((uintptr_t)(addr)+((align)-1)) & ~((align)-1))

#define sz_i16   sizeof(int16_t)

/* Filter instance structure. */
typedef struct tag_bkfira16x16_t
{
    int32_t           magic;     // Instance pointer validation number
    int               M;         // Number of filter coefficients
    const int16_t   * coef;      // Filter coefficients
    int16_t         * delayLine; // Delay line for samples, aligned
    int               delayLen;  // Delay line length, in samples
    int               wrIx;
} bkfira16x16_t, *bkfira16x16_ptr_t;

/* Calculate the memory block size for an FIR filter with given attributes. */
size_t bkfira16x16_alloc( int M )
{
    int _M;
    _M = M + (-M & 3);
    NASSERT(M > 0);

    return (ALIGNED_SIZE(sizeof(bkfira16x16_t), 8)
        + // Delay line
        // Allocated memory for delay line has increased by 2 samples
        // to avoid memory bank conflicts and get the best performance
        ALIGNED_SIZE((_M + 8 + 2)*sz_i16, 16)
        + // Filter coefficients
        ALIGNED_SIZE((_M + 4)*sz_i16, 16));
} /* bkfira16x16_alloc() */

/* Initialize the filter structure. The delay line is zeroed. */
bkfira16x16_handle_t bkfira16x16_init(void * objmem, int M, const int16_t * h)
{
    bkfira16x16_t * bkfir;
    void         * ptr;
    int16_t      * delLine;
    int16_t      * coef;

    int m, _M, n;

    NASSERT(objmem &&  M > 0 && h);
    _M = M + (-M & 3);

    //
    // Partition the memory block
    //
    ptr     = objmem;
    bkfir   = (bkfira16x16_ptr_t)ALIGNED_ADDR(ptr, 8);
    ptr     = bkfir + 1;
    delLine = (int16_t *)ALIGNED_ADDR(ptr, 16); /* important alignment on 16-bytes to place delay line and coefficients to different banks for better dual loads */
    // Allocated memory for delay line has increased by 2 samples
    // to avoid memory bank conflicts and get the best performance
    ptr     = delLine + _M + 8 + 2;
    coef    = (int16_t *)ALIGNED_ADDR(ptr, 16); coef+=4;    /* align on 16-byte but place coefficients in another bank */
    ptr     = coef + _M + 4;

    ASSERT((int8_t*)ptr - (int8_t*)objmem <= (int)bkfira16x16_alloc(M));

    //
    // Copy the filter coefficients in reverted order and zero the delay line.
    //

    //coef[0] = 0;
    for (n = 0; n<(-M & 3) + 1; n++)
    {
        coef[n] = 0;
    }
    for (m = 0; m<M; m++, n++)
    {
        coef[n] = h[M - m - 1];
    }

    for (m = 0; m<3; m++, n++)
    {
        coef[n] = 0;
    }

    for (m = 0; m<_M + 8; m++)
    {
        delLine[m] = 0;
    }

    //
    // Initialize the filter instance.
    //
    bkfir->magic     = MAGIC;
    bkfir->M         = _M;
    bkfir->coef      = coef;
    bkfir->delayLine = delLine;
    bkfir->delayLen  = _M + 8;
    bkfir->wrIx      = 0;

    return (bkfir);
} /* bkfira16x16_init() */

/* process block of samples */
void bkfira16x16_process( bkfira16x16_handle_t handle, 
                         int16_t * restrict  y,
                   const int16_t * restrict  x, int N )
{
    bkfira16x16_ptr_t bkfir = (bkfira16x16_ptr_t)handle;

    const ae_int16x4 *          C;
          ae_int16x4 * restrict D_wr;
          ae_int16   * restrict D_s_wr;
    const ae_int16x4 *          D_rd0;
    const ae_int16x4 *          D_rd0_1;
    const ae_int16x4 *          D_rd1;
    const ae_int16x4 *          D_rd2;
    const ae_int16x4 *          D_rd3;
    const ae_int16   *          D_s_rd;
    const ae_int16   *          X_s;
    const ae_int16x4 *          X;
    ae_int16   * restrict Y;

    ae_valign D_va0, D_va1, D_va2, D_va3;
    ae_valign ay;
    ae_valign x_align;
    
    ae_int16x4 d01, d12, d23, d34;
    ae_int64   q0, q1, q2, q3;
    ae_int64   q4, q5, q6, q7;
    ae_int64   z;
    ae_f32x2   t0, t1;
    ae_int16x4 c0;
    
    int M, wrIx;
    int m, n;
    int ua_cnt;

    if (N <= 0) return;
    NASSERT(bkfir && ((bkfir->magic) == MAGIC) && y && x);
    NASSERT(IS_ALIGN(bkfir->delayLine) &&
            IS_ALIGN(bkfir->coef) &&
            IS_ALIGN(bkfir->coef + bkfir->M + 4) &&
            IS_ALIGN(bkfir->delayLine + bkfir->delayLen));

    M    = bkfir->M;
    wrIx = bkfir->wrIx;

    //
    // Setup pointers and circular delay line buffer.
    //
    D_wr = (      ae_int16x4 *)(bkfir->delayLine + wrIx);
    X    = (const ae_int16x4 *)x;
    Y    = (      ae_int16   *)y;
    WUR_AE_CBEGIN0((uintptr_t)(bkfir->delayLine));
    WUR_AE_CEND0  ((uintptr_t)(bkfir->delayLine + bkfir->delayLen));

    z       = AE_CVTQ56A32S(0);
    
    ua_cnt = (4-wrIx) & 3;
    ua_cnt = XT_MIN(ua_cnt, N);

    //process first 0..3 samples (until D_wr is aligned to 8 bytes)
    if (ua_cnt)
    {
        // Reset the coefficients pointer. Now it looks at the tap corresponding
        // to the oldest sample in the delay line.
        C = (const ae_int16x4*)bkfir->coef;
        D_s_wr = (ae_int16*)D_wr;

        // Insert 0..3 input samples into the delay line one-by-one.
        for (n = 0; n < (ua_cnt); n++)
        {
            AE_L16_IP(d01, castxcc(ae_int16, X), +2);
            AE_S16_0_XC(d01, D_s_wr, +2);
        }
        D_s_rd = D_s_wr;
        // Perform dummy reads to skip 8-ua_cnt oldest samples.
        for (n = ua_cnt; n < 8; n++)
        {
            AE_L16_XC(d01, D_s_rd, +2);
        }
        D_wr = (ae_int16x4 *) D_s_wr;
        D_rd2 = D_rd1 = D_rd0 = (const ae_int16x4*)D_s_rd;
        AE_ADDCIRC_XC(castxcc(ae_int64, D_rd1), 1*sizeof(int16_t));
        AE_ADDCIRC_XC(castxcc(ae_int64, D_rd2), 2*sizeof(int16_t));

        AE_LA16X4POS_PC(D_va0, D_rd0);
        AE_LA16X4POS_PC(D_va1, D_rd1);
        AE_LA16X4POS_PC(D_va2, D_rd2);

        // Zero the accumulators.
        q0 = z; q1 = z; q2 = z;

        __Pragma("loop_count min=2")
        for (m = 0; m < (M >> 2)+1; m++)
        {
            AE_LA16X4_IC(d01, D_va0, D_rd0);
            AE_LA16X4_IC(d12, D_va1, D_rd1);
            AE_LA16X4_IC(d23, D_va2, D_rd2);
            AE_L16X4_IP(c0, C, +8);
            AE_MULAAAAQ16(q0, d01, c0);
            AE_MULAAAAQ16(q1, d12, c0);
            AE_MULAAAAQ16(q2, d23, c0);
        }

        t0 = AE_TRUNCA32X2F64S(q1, q0, 33);
        t1 = AE_TRUNCA32X2F64S(q2, q2, 33);
        d01 = AE_ROUND16X4F32SASYM(t1, t0);
  
        // Convert and save 1..3 outputs.
        AE_S16_0_IP(d01, Y, +2);
        if (ua_cnt > 1) { d01 = AE_SEL16_4321(d01, d01); AE_S16_0_IP(d01, Y, +2); }
        if (ua_cnt > 2) { d01 = AE_SEL16_4321(d01, d01); AE_S16_0_IP(d01, Y, +2); }
        N -= ua_cnt;
    }
    
    x_align = AE_LA64_PP(X);
    ay      = AE_ZALIGN64();
    // Break the input signal into 8-samples blocks. For each block, store 8
    // samples to the delay line and compute the filter response.
    for (n = 0; n < (N>>3); n++)
    {
        // Load 8 input samples.
        AE_LA16X4_IP(d01, x_align, X);
        AE_LA16X4_IP(d12, x_align, X);

        // Store 8 samples to the delay line buffer with circular address update.
        AE_S16X4_XC(d01, D_wr, +8);
        AE_S16X4_XC(d12, D_wr, +8);

        D_rd3 = D_rd2 = D_rd1 = D_rd0 = D_rd0_1 = D_wr;
        AE_ADDCIRC_XC(castxcc(ae_int64, D_rd1), 1*sizeof(int16_t));
        AE_ADDCIRC_XC(castxcc(ae_int64, D_rd2), 2*sizeof(int16_t));
        AE_ADDCIRC_XC(castxcc(ae_int64, D_rd3), 3*sizeof(int16_t));
        AE_ADDCIRC_XC(castxcc(ae_int64, D_rd0_1), 4*sizeof(int16_t));

        AE_LA16X4POS_PC(D_va1, D_rd1);
        AE_LA16X4POS_PC(D_va2, D_rd2);
        AE_LA16X4POS_PC(D_va3, D_rd3);

        // Zero the accumulators.
        q0 = z; q1 = z; q2 = z; q3 = z;
        q4 = z; q5 = z; q6 = z; q7 = z;

        // Reset the coefficients pointer. Now it looks at the tap corresponding
        // to the oldest sample in the delay line.
        C = (const ae_int16x4*)bkfir->coef;

        AE_LA16X4_IC(d12, D_va1, D_rd1);
        AE_LA16X4_IC(d23, D_va2, D_rd2);
        AE_LA16X4_IC(d34, D_va3, D_rd3);

        __Pragma("loop_count min=2")
        for (m = 0; m < (M >> 2) + 1; m++)
        {
            AE_L16X4_IP(c0, C, +8);
            AE_L16X4_XC(d01, D_rd0, 8);

            AE_MULAAAAQ16(q0, d01, c0);
            AE_MULAAAAQ16(q1, d12, c0);
            AE_MULAAAAQ16(q2, d23, c0);
            AE_MULAAAAQ16(q3, d34, c0);

            d01=d23;
            d12=d34;
            AE_LA16X4_IC(d23, D_va2, D_rd2);
            AE_LA16X4_IC(d34, D_va3, D_rd3);
            d12=AE_SEL16_5432(d12,d34);
            d01=AE_SEL16_5432(d01,d23);

            AE_MULAAAAQ16(q4, d01, c0);
            AE_MULAAAAQ16(q5, d12, c0);
            AE_MULAAAAQ16(q6, d23, c0);
            AE_MULAAAAQ16(q7, d34, c0);
        }

        t0 = AE_TRUNCA32X2F64S(q0, q1, 33);
        t1 = AE_TRUNCA32X2F64S(q2, q3, 33);
        AE_SA16X4_IP(AE_ROUND16X4F32SASYM(t0, t1), ay, castxcc(ae_int16x4, Y));

        t0 = AE_TRUNCA32X2F64S(q4, q5, 33);
        t1 = AE_TRUNCA32X2F64S(q6, q7, 33);
        AE_SA16X4_IP(AE_ROUND16X4F32SASYM(t0, t1), ay, castxcc(ae_int16x4, Y));
    }
    N &= 7;
    if (N & 4)
    {
        // Load 4 input samples.
        AE_LA16X4_IP(d01, x_align, X);

        // Store 4 samples to the delay line buffer with circular address update.
        AE_S16X4_XC(d01, D_wr, +8);

        // Circular buffer write pointer looks at the oldest sample: M+4 samples
        // back from the newest one. To read the delay buffer separately for
        // even/odd accumulators, read pointers are set from M-4th to M-1st
        // samples. Pointer increment is safe in respect to crossing the circular
        // buffer boundary.
        D_rd0 = D_wr;
        AE_ADDCIRC_XC(castxcc(ae_int64, D_rd0), 4*sizeof(int16_t));
        D_rd3 = D_rd2 = D_rd1 = D_rd0;
        AE_ADDCIRC_XC(castxcc(ae_int64, D_rd1), 1*sizeof(int16_t));
        AE_ADDCIRC_XC(castxcc(ae_int64, D_rd2), 2*sizeof(int16_t));
        AE_ADDCIRC_XC(castxcc(ae_int64, D_rd3), 3*sizeof(int16_t));

        AE_LA16X4POS_PC(D_va1, D_rd1);
        AE_LA16X4POS_PC(D_va2, D_rd2);
        AE_LA16X4POS_PC(D_va3, D_rd3);

        // Reset the coefficients pointer. Now it looks at the tap corresponding
        // to the oldest sample in the delay line.
        C = (const ae_int16x4*)bkfir->coef;

        // Inner loop: process 4 taps for 4 accumulators on each trip. Actually we 
        // perform M+4 MACs for each accumulator, 4 of which fall on zero taps
        // inserted into the impulse response during initialization.
        //
        // Zero the accumulators.
        q0 = z; q1 = z; q2 = z; q3 = z;

        __Pragma("loop_count min=2")
        for (m = 0; m < (M >> 2)+1; m++)
        {
            AE_L16X4_IP(c0, C, +8);
            AE_L16X4_XC(d01, D_rd0, +8);
            AE_LA16X4_IC(d12, D_va1, D_rd1);
            AE_LA16X4_IC(d23, D_va2, D_rd2);
            AE_LA16X4_IC(d34, D_va3, D_rd3);

            AE_MULAAAAQ16(q0, d01, c0);
            AE_MULAAAAQ16(q1, d12, c0);
            AE_MULAAAAQ16(q2, d23, c0);
            AE_MULAAAAQ16(q3, d34, c0);
        }

        t0 = AE_TRUNCA32X2F64S(q0, q1, 33);
        t1 = AE_TRUNCA32X2F64S(q2, q3, 33);
        d01 = AE_ROUND16X4F32SASYM(t0, t1);
        AE_SA16X4_IP(d01, ay, castxcc(ae_int16x4, Y));
    }
    AE_SA64POS_FP(ay, Y);

    X_s    = (const ae_int16*)X;
    D_s_wr = (ae_int16*)D_wr;
    N &= 3;
    if (N)
    {
        // Reset the coefficients pointer. Now it looks at the tap corresponding
        // to the oldest sample in the delay line.
        C = (const ae_int16x4*)bkfir->coef;

        // Insert 0..3 input samples into the delay line one-by-one.
        for (n = 0; n < N; n++)
        {
            AE_L16_IP(d01, X_s, +2);
            AE_S16_0_XC(d01, D_s_wr, +2);
        }
        D_s_rd = D_s_wr;
        // Perform dummy reads to skip 8-(N&3) oldest samples.
        for (n = N; n < 8; n++)
        {
            AE_L16_XC(d01, D_s_rd, +2);
        }

        D_rd2 = D_rd1 = D_rd0 = (const ae_int16x4*)D_s_rd;
        AE_ADDCIRC_XC(castxcc(ae_int64, D_rd1), 1*sizeof(int16_t));
        AE_ADDCIRC_XC(castxcc(ae_int64, D_rd2), 2*sizeof(int16_t));

        AE_LA16X4POS_PC(D_va1, D_rd1);
        AE_LA16X4POS_PC(D_va2, D_rd2);

        {
            AE_L16X4_XC(d01, D_rd0, +8);
            AE_LA16X4_IC(d12, D_va1, D_rd1);
            AE_LA16X4_IC(d23, D_va2, D_rd2);
            AE_L16X4_IP(c0, C, +8);
            q0 = AE_MULZAAAAQ16(d01, c0);
            q1 = AE_MULZAAAAQ16(d12, c0);
            q2 = AE_MULZAAAAQ16(d23, c0);
        }
        for (m = 0; m < (M >> 2); m++)
        {
            AE_L16X4_XC(d01, D_rd0, +8);
            AE_LA16X4_IC(d12, D_va1, D_rd1);
            AE_LA16X4_IC(d23, D_va2, D_rd2);
            AE_L16X4_IP(c0, C, +8);
            AE_MULAAAAQ16(q0, d01, c0);
            AE_MULAAAAQ16(q1, d12, c0);
            AE_MULAAAAQ16(q2, d23, c0);
        }

        t0 = AE_TRUNCA32X2F64S(q1, q0, 33);
        t1 = AE_TRUNCA32X2F64S(q2, q2, 33);
        d01 = AE_ROUND16X4F32SASYM(t1, t0);
  
        // Convert and save 1..3 outputs.
        AE_S16_0_IP(d01, Y, +2);
        if ((N & 3) > 1) { d01 = AE_SEL16_4321(d01, d01); AE_S16_0_IP(d01, Y, +2); }
        if ((N & 3) > 2) { d01 = AE_SEL16_4321(d01, d01); AE_S16_0_IP(d01, Y, +2); }
    }

    bkfir->wrIx = (int)((int16_t *)D_s_wr - bkfir->delayLine);

} /* bkfira16x16_process() */
