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
    Real block FIR filter, 16x16-bit
    C code optimized for HiFi4
  IntegrIT, 2006-2018
*/

/*-------------------------------------------------------------------------
  Real FIR filter.
  Computes a real FIR filter (direct-form) using IR stored in vector h. The 
  real data input is stored in vector x. The filter output result is stored 
  in vector y. The filter calculates N output samples using M coefficients 
  and requires last M-1 samples in the delay line.
  NOTE: 
  1. User application is not responsible for management of delay lines
  2. User has an option to set IR externally or copy from original location 
     (i.e. from the slower constant memory). In the first case, user is 
     responsible for right alignment, ordering and zero padding of filter 
     coefficients - usually array is composed from zeroes (left padding), 
     reverted IR and right zero padding.


  Precision: 
  16x16    16-bit data, 16-bit coefficients, 16-bit outputs. Ordinary variant 
           and stereo
  32x16    32-bit data, 16-bit coefficients, 32-bit outputs
  32x32    32-bit data, 32-bit coefficients, 32-bit outputs. Ordinary variant 
           and stereo
  32x32ep  the same as above but using 72-bit accumulator for intermediate 
           computations
  f        floating point. Ordinary variant and stereo

  Input:
  x[N*S]   input samples, Q31, Q15, floating point
  h[M]     filter coefficients in normal order, Q31, Q15, floating point
  hl[M]    for stereo filters: filter coefficients for left channel
  hr[M]    for stereo filters: filter coefficients for right channel
  N        length of sample block, should be a multiple of 4
  M        length of filter, should be a multiple of 4
  extIR    if zero, IR is copied from original location, otherwise not
           but user should keep alignment, order of coefficients 
           and zero padding requirements shown below
  S        1 for ordinary (single channel) filters, 2 - for stereo variant
  
  Output:
  y[N*S]   output samples, Q31, Q15, floating point

  Alignment, ordering and zero padding for external IR  (extIR!=0)
  ------------------------+----------+--------------+--------------+----------------
  Function                |Alignment,|Left zero     |   Coefficient| Right zero 
                          | bytes    |padding, bytes|   order      | padding, bytes
  ------------------------+----------+--------------+--------------+----------------
  bkfir16x16_init         |     8    |      2       |  inverted    |  6
  bkfir32x16_init (M>32)  |     8    |     10       |  inverted    |  6
  bkfir32x16_init (M<=32) |     8    |      2       |  inverted    |  6
  bkfir32x32_init         |     8    |      4       |  inverted    |  12
  bkfir32x32ep_init       |     8    |      4       |  inverted    |  12
  bkfirf_init             |     8    |      0       |  direct      |  0
  stereo_bkfir16x16_init  |     8    |      2       |  inverted    |  6
  stereo_bkfir32x32_init  |     8    |      4       |  inverted    |  12
  stereo_bkfirf_init      |     8    |      0       |  direct      |  0
  ------------------------+----------+--------------+--------------+----------------

  Restrictions:
  x, y     should not be overlapping
  x, h     aligned on a 8-bytes boundary
  N, M     multiples of 4 
-------------------------------------------------------------------------*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fir.h"
/* Common utility and macros declarations. */
#include "common.h"

#define MAGIC     0x5CDED6E2

/* Reserve memory for alignment. */
#define ALIGNED_SIZE( size, align ) \
    ((size_t)(size)+(align)-1)

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
    (void*)(((uintptr_t)(addr)+((align)-1)) & ~((align)-1))

#define sz_i16   sizeof(int16_t)
#define sz_i32   sizeof(int32_t)

/* Filter instance structure. */
typedef struct tag_bkfir16x16_t
{
    int32_t           magic;     // Instance pointer validation number
    int               M;         // Number of filter coefficients
    const int16_t   * coef;      // Filter coefficients
    int16_t         * delayLine; // Delay line for samples, aligned
    int               delayLen;  // Delay line length, in samples
    int16_t         * delayPos;  // Delay line slot to be filled next
} bkfir16x16_t, *bkfir16x16_ptr_t;

/* Calculate the memory block size for an FIR filter with given attributes. */
size_t bkfir16x16_alloc(int M, int extIR)
{
    NASSERT(M > 0);
    NASSERT(!(M & 3));

    return (ALIGNED_SIZE(sizeof(bkfir16x16_t), 4)
        + // Delay line
        ALIGNED_SIZE((M + 8)*sz_i16, 16)
        + // Filter coefficients
        (extIR?0:ALIGNED_SIZE((M + 4)*sz_i16, 16)));
} /* bkfir16x16_alloc() */

/* Initialize the filter structure. The delay line is zeroed. */
bkfir16x16_handle_t bkfir16x16_init(void * objmem, int M, int extIR, const int16_t * h)
{
    bkfir16x16_t * bkfir;
    void         * ptr;
    int16_t      * delLine;
    int16_t      * coef;

    int m;

    NASSERT(objmem && (M>0) && h);
    NASSERT(!(M&3) && IS_ALIGN(h));

    //
    // Partition the memory block
    //

    ptr     = objmem;
    bkfir   = (bkfir16x16_ptr_t)ALIGNED_ADDR(ptr, 4);
    ptr     = bkfir + 1;
    delLine = (int16_t *)ALIGNED_ADDR(ptr, 16); /* important alignment on 16-bytes to place delay line and coefficients to different banks for better dual loads */
    ptr     = delLine + M + 8;
    if (extIR)
    {
        coef = (int16_t *)h;
    }
    else
    {
        coef    = (int16_t *)ALIGNED_ADDR(ptr, 16); coef+=4;    /* align on 16-byte but place coefficients in another bank */
        ptr     = coef + M + 4;
    }
    ASSERT((int8_t*)ptr - (int8_t*)objmem <= (int)bkfir16x16_alloc(M, extIR));

    //
    // Copy the filter coefficients and zero the delay line. Original impulse
    // response is padded with zeros: three zeros go before the first tap
    // (corresponds to the newest sample), one zero follows the last tap,
    // which matches the oldest sample. After that the order of filter
    // coefficients is reverted.
    //
    if (extIR == 0)
    {
        coef[0] = 0;

        for (m = 1; m<M + 1; m++)
        {
            coef[m] = h[M - m];
        }

        for (; m<M + 4; m++)
        {
            coef[m] = 0;
        }
    }

    for (m = 0; m<M + 8; m++)
    {
        delLine[m] = 0;
    }

    //
    // Initialize the filter instance.
    //

    bkfir->magic     = MAGIC;
    bkfir->M         = M;
    bkfir->coef      = coef;
    bkfir->delayLine = delLine;
    bkfir->delayLen  = M + 8;
    bkfir->delayPos  = delLine;

    return (bkfir);
} /* bkfir16x16_init() */

void bkfir16x16_process( bkfir16x16_handle_t handle,
                         int16_t * restrict  y,
                   const int16_t * restrict  x, int N)
{
    bkfir16x16_ptr_t bkfir = (bkfir16x16_ptr_t)handle;

    const ae_int16x4 *          C;
          ae_int16x4 * restrict D_wr;
    const ae_int16x4 *          D_rd0;
    const ae_int16x4 *          D_rd0_1;
    const ae_int16x4 *          D_rd1;
    const ae_int16x4 *          D_rd2;
    const ae_int16x4 *          D_rd3;
    const ae_int16x4 *          X;
    ae_int16   * restrict Y;

    ae_valign D_va1, D_va2, D_va3;
    ae_valign ay;

    ae_int16x4 d01, d23, d12, d34;
    ae_int64   q0, q1, q2, q3;
    ae_int64   q4, q5, q6, q7;
    ae_int64   z;
    ae_f32x2   t0, t1;
    ae_int16x4 c0;

    int M;
    int m, n;

    NASSERT(bkfir && bkfir->magic == MAGIC && y && x);
    M = bkfir->M;

    NASSERT_ALIGN((x), 8);
    NASSERT_ALIGN((bkfir->delayLine), 8);
    NASSERT_ALIGN((bkfir->delayLine + bkfir->delayLen), 8);
    NASSERT_ALIGN((bkfir->delayPos), 8);
    NASSERT_ALIGN((bkfir->coef), 8);

    NASSERT(N % 4 == 0);
    NASSERT(M % 4 == 0);
    if (N <= 0) return;

    //
    // Setup pointers and circular delay line buffer.
    //
    D_wr = (ae_int16x4 *)bkfir->delayPos;
    X = (const ae_int16x4 *)x;
    Y = (ae_int16     *)y;

    WUR_AE_CBEGIN0((uintptr_t)(bkfir->delayLine));
    WUR_AE_CEND0((uintptr_t)(bkfir->delayLine + bkfir->delayLen));

    z = AE_CVTQ56A32S(0);
    ay = AE_ZALIGN64();

    for (n = 0; n < (N >> 3); n++)
    {
        // Load 8 input samples.
        AE_L16X4_IP(d01, X, +8);
        AE_L16X4_IP(d12, X, +8);

        // Store 8 samples to the delay line buffer with circular address update.
        AE_S16X4_XC(d01, D_wr, +8);
        AE_S16X4_XC(d12, D_wr, +8);

        D_rd0 = D_wr;
        D_rd0_1 = D_wr;

        AE_ADDCIRC_XC(castxcc(ae_int64, D_rd0_1), 4*sizeof(int16_t));
        D_rd1 = (ae_int16x4*)((int16_t*)D_rd0 + 1);
        D_rd2 = (ae_int16x4*)((int16_t*)D_rd0 + 2);
        D_rd3 = (ae_int16x4*)((int16_t*)D_rd0 + 3);

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

    if (N & 4)
    {
        // Load 4 input samples.
        AE_L16X4_IP(d01, X, +8);

        // Store 4 samples to the delay line buffer with circular address update.
        AE_S16X4_XC(d01, D_wr, +8);

        // Circular buffer write pointer looks at the oldest sample: M+4 samples
        // back from the newest one. To read the delay buffer separately for
        // even/odd accumulators, the two read pointers are set to M-4th and M-3rd
        // samples. Pointer increment is safe in respect to crossing the circular
        // buffer boundary.
        D_rd0 = D_wr;
        AE_ADDCIRC_XC(castxcc(ae_int64, D_rd0), 4*sizeof(int16_t));
        D_rd1 = (ae_int16x4*)((int16_t*)D_rd0 + 1);
        D_rd2 = (ae_int16x4*)((int16_t*)D_rd0 + 2);
        D_rd3 = (ae_int16x4*)((int16_t*)D_rd0 + 3);

        AE_LA16X4POS_PC(D_va1, D_rd1);
        AE_LA16X4POS_PC(D_va2, D_rd2);
        AE_LA16X4POS_PC(D_va3, D_rd3);

        // Zero the accumulators.
        q0 = z; q1 = z; q2 = z; q3 = z;

        // Reset the coefficients pointer. Now it looks at the tap corresponding
        // to the oldest sample in the delay line.
        C = (const ae_int16x4*)bkfir->coef;

        // Inner loop: process 4 taps for 4 accumulators on each trip. Actually we 
        // perform M+4 MACs for each accumulator, 4 of which fall on zero taps
        // inserted into the impulse response during initialization.
        //
        __Pragma("loop_count min=2")
        for (m = 0; m < (M >> 2) + 1; m++)
        {
            AE_L16X4_XC(d01, D_rd0, 8);
            AE_LA16X4_IC(d12, D_va1, D_rd1);
            AE_LA16X4_IC(d23, D_va2, D_rd2);
            AE_LA16X4_IC(d34, D_va3, D_rd3);
            AE_L16X4_IP(c0, C, +8);

            AE_MULAAAAQ16(q0, d01, c0);
            AE_MULAAAAQ16(q1, d12, c0);
            AE_MULAAAAQ16(q2, d23, c0);
            AE_MULAAAAQ16(q3, d34, c0);
        }

        t0 = AE_TRUNCA32X2F64S(q0, q1, 33);
        t1 = AE_TRUNCA32X2F64S(q2, q3, 33);
        AE_SA16X4_IP(AE_ROUND16X4F32SASYM(t0, t1), ay, castxcc(ae_int16x4, Y));
    }

    AE_SA64POS_FP(ay, Y);
    bkfir->delayPos = (int16_t*)D_wr;
} /* bkfir16x16_process() */
