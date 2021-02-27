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
    Real block FIR filter, 32x32-bit, unaligned data and arbitrary M/N allowed
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

#include <string.h>

#include "NatureDSP_types.h"
#include "NatureDSP_Signal.h"
#include "common.h"

/* Instance pointer validation number. */
#define MAGIC     0x689dfea4

/* Reserve memory for alignment. */
#define ALIGNED_SIZE( size, align ) \
      ( (size_t)(size) + (align) - 1 )

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
      (void*)( ( (uintptr_t)(addr) + ( (align) - 1 ) ) & ~( (align) - 1 ) )

#define sz_i32   sizeof(int32_t)

/* Filter instance structure. */
typedef struct tag_bkfira32x32_t
{
  uint32_t    magic;     // Instance pointer validation number
  int         M;         // Number of filter coefficients
  const int32_t * coef;      // M filter coefficients, reverted
  int32_t *       delayLine; // Delay line for samples
  int         delayLen;  // Delay line length, in samples
  int         wrIx;      // Index of the oldest sample

} bkfira32x32_t, *bkfira32x32_ptr_t;

/* Allocation routine for filters. Returns: size of memory in bytes to be allocated */
size_t bkfira32x32_alloc(int M)
{

    int _M;
    _M = M + (-M&3);
    NASSERT(  M > 0 );

    return ( ALIGNED_SIZE( sizeof( bkfira32x32_t ), 8 )
        + // Delay line
        // Allocated memory for delay line has increased by 2 samples
        // to avoid memory bank conflicts and get the best performance
        ALIGNED_SIZE( ( _M + 4 + 2 )*sz_i32,  8)
        + // Filter coefficients
        ALIGNED_SIZE( (_M+4)*sz_i32, 8 ) );

} // bkfira32x32_alloc()

/* Initialization for filters. Returns: handle to the object */
bkfira32x32_handle_t bkfira32x32_init( void *         objmem, 
    int            M, 
    const f24 * restrict h )
{
    bkfira32x32_ptr_t bkfir;
    void *           ptr;
    int32_t *        delLine;
    int32_t *        coef;

    int m, _M, n;

    NASSERT( objmem &&  M > 0 && h );
    _M = M + (-M&3);
    //
    // Partition the memory block
    //

    ptr     = objmem;
    bkfir   = (bkfira32x32_ptr_t)ALIGNED_ADDR( ptr, 8 );
    ptr     = bkfir + 1;
    delLine = (int32_t *)ALIGNED_ADDR( ptr, 8 );
    // Allocated memory for delay line has increased by 2 samples
    // to avoid memory bank conflicts and get the best performance
    ptr     = delLine + _M + 4 + 2;
    coef    = (int32_t *)ALIGNED_ADDR( ptr, 8 );
    ptr     = coef + _M + 4;

    ASSERT( (int8_t*)ptr - (int8_t*)objmem <= (int)bkfira32x32_alloc( M ) );

    //
    // Copy the filter coefficients in reverted order and zero the delay line.
    //

    //coef[0] = 0;
    for ( n=0; n<(-M&3)+1; n++ )
    {
        coef[n] = 0;
    }
    for ( m=0; m<M; m++,n++ )
    {
        coef[n] = h[M - m - 1];
    }

    for (m = 0; m<3; m++, n++)
    {
        coef[n] = 0;
    }

    for (m = 0; m<_M + 4; m++)
    {
        delLine[m] = 0;
    }

    //
    // Initialize the filter instance.
    //

    bkfir->magic = MAGIC;
    bkfir->M = _M;
    bkfir->coef = coef;
    bkfir->delayLine = delLine;
    bkfir->delayLen = _M + 4;
    bkfir->wrIx = 0;

    return (bkfir);
} // bkfira32x32_init()

void bkfira32x32_process( bkfira32x32_handle_t _bkfir,
                          int32_t * restrict       y,
                    const int32_t * restrict       x, int N)
{
    bkfira32x32_ptr_t bkfir = (bkfira32x32_ptr_t)_bkfir;

    const   ae_f32x2 *            C;
    ae_int32x2 * restrict D_wr;
    ae_int32   * restrict D_s_wr;
    const ae_int32x2 *          D_rd0;
    const ae_int32   *          D_s_rd;
    const ae_int32x2 *          X;
    const ae_int32   *          X_s;
    ae_f32     * restrict Y;

    ae_valign D_va;

    ae_int32x2 t01, t23, t45, t67;
    ae_int32x2 c0, c1;
    ae_f64     q0, q1, q2, q3;
    ae_valign  x_align;
    int M, wrIx;
    int m, n;

    if (N <= 0) return;
    NASSERT(bkfir && bkfir->magic == MAGIC && y && x);
    NASSERT(IS_ALIGN(bkfir->delayLine) &&
        IS_ALIGN(bkfir->coef) &&
        IS_ALIGN(bkfir->coef + bkfir->M + 4) &&
        IS_ALIGN(bkfir->delayLine + bkfir->delayLen));

    M = bkfir->M;
    wrIx = bkfir->wrIx;

    //
    // Setup pointers and circular delay line buffer.
    //

    D_wr = (ae_int32x2*)(bkfir->delayLine + wrIx);
    X = (const ae_int32x2 *)x;
    Y = (ae_f32     *)y;

    WUR_AE_CBEGIN0((uintptr_t)(bkfir->delayLine));
    WUR_AE_CEND0((uintptr_t)(bkfir->delayLine + bkfir->delayLen));

    x_align = AE_LA64_PP(X);

    //
    // Break the input signal into 4-samples blocks. For each block, store 4
    // samples to the delay line and compute the filter response.
    //
    for (n = 0; n<(N >> 2); n++)
    {
        // Reset the coefficients pointer. Now it looks at the tap corresponding
        // to the oldest sample in the delay line.
        C = (const ae_f32x2*)bkfir->coef;

        // Load 4 input samples.
        // Q31
        AE_LA32X2_IP(t01, x_align, X);
        AE_LA32X2_IP(t23, x_align, X);
        D_va = AE_ZALIGN64();
        // Store 4 samples to the delay line buffer with circular address update.
        // Q31
        AE_SA32X2_IC(t01, D_va, D_wr);
        AE_SA32X2_IC(t23, D_va, D_wr);
        AE_SA64POS_FP(D_va, D_wr);
        // Circular buffer write pointer looks at the oldest sample: M+4 samples
        // back from the newest one. 
        D_rd0 = D_wr;
        AE_LA32X2POS_PC(D_va, D_rd0);

        // Load two oldest samples for even/odd accumulators.
        // Q31
        AE_LA32X2_IC(t01, D_va, D_rd0);
        AE_LA32X2_IC(t23, D_va, D_rd0);

        // Load next two samples for even accumulators. Altogether we have
        // 8 samples residing in 4 AE registers.
        // Q31
        AE_LA32X2_IC(t45, D_va, D_rd0);
        AE_LA32X2_IC(t67, D_va, D_rd0);

        // Load the next 4 tap coefficients.
        // Q31
        AE_L32X2_IP(c0, C, +8);
        AE_L32X2_IP(c1, C, +8);

        // Q17.47 <- QQ17.47 + [ Q31*Q31 ] - 15 with asymmetric rounding
        q0 = AE_MULF32R_HH(t01, c0);
        AE_MULAF32R_LL(q0, t01, c0);
        AE_MULAF32R_HH(q0, t23, c1);
        AE_MULAF32R_LL(q0, t23, c1);

        q1 = AE_MULF32R_LH(t01, c0);
        AE_MULAF32R_LH(q1, c0, t23);
        AE_MULAF32R_LH(q1, t23, c1);
        AE_MULAF32R_LH(q1, c1, t45);

        q2 = AE_MULF32R_HH(t23, c0);
        AE_MULAF32R_LL(q2, t23, c0);
        AE_MULAF32R_HH(q2, t45, c1);
        AE_MULAF32R_LL(q2, t45, c1);

        q3 = AE_MULF32R_LH(t23, c0);
        AE_MULAF32R_LH(q3, c0, t45);
        AE_MULAF32R_LH(q3, t45, c1);
        AE_MULAF32R_LH(q3, c1, t67);

        t01 = t45;
        t23 = t67;

        //
        // Inner loop: process 4 taps for 4 accumulators on each trip. Actually we 
        // perform M+4 MACs for each accumulator, 4 of which fall on zero taps
        // inserted into the impulse response during initialization.
        //
        __Pragma("loop_count min=1")
        for (m = 0; m<(M >> 2); m++)
        {
            // Load next two samples for even accumulators. Altogether we have
            // 8 samples residing in 4 AE registers.
            // Q31
            AE_LA32X2_IC(t45, D_va, D_rd0);
            AE_LA32X2_IC(t67, D_va, D_rd0);

            // Load the next 4 tap coefficients.
            // Q31
            AE_L32X2_IP(c0, C, +8);
            AE_L32X2_IP(c1, C, +8);

            // Q17.47 <- QQ17.47 + [ Q31*Q31 ] - 15 with asymmetric rounding
            AE_MULAF32R_HH(q0, t01, c0);
            AE_MULAF32R_LL(q0, t01, c0);
            AE_MULAF32R_HH(q0, t23, c1);
            AE_MULAF32R_LL(q0, t23, c1);

            AE_MULAF32R_LH(q1, t01, c0);
            AE_MULAF32R_LH(q1, c0, t23);
            AE_MULAF32R_LH(q1, t23, c1);
            AE_MULAF32R_LH(q1, c1, t45);

            AE_MULAF32R_HH(q2, t23, c0);
            AE_MULAF32R_LL(q2, t23, c0);
            AE_MULAF32R_HH(q2, t45, c1);
            AE_MULAF32R_LL(q2, t45, c1);

            AE_MULAF32R_LH(q3, t23, c0);
            AE_MULAF32R_LH(q3, c0, t45);
            AE_MULAF32R_LH(q3, t45, c1);
            AE_MULAF32R_LH(q3, c1, t67);

            t01 = t45;
            t23 = t67;
        }

        // Convert and save 4 outputs.
        AE_S32RA64S_IP(q0, Y, +4);
        AE_S32RA64S_IP(q1, Y, +4);
        AE_S32RA64S_IP(q2, Y, +4);
        AE_S32RA64S_IP(q3, Y, +4);
    }
    X_s = (const ae_int32*)X;
    D_s_wr = (ae_int32*)D_wr;
    if (N & 3)
    {
        // Reset the coefficients pointer. Now it looks at the tap corresponding
        // to the oldest sample in the delay line.
        C = (const ae_f32x2*)bkfir->coef;

        // Insert 0..3 input samples into the delay line one-by-one.
        for (n = 0; n<(N & 3); n++)
        {
            AE_L32_IP(t01, X_s, +4);
            AE_S32_L_XC(t01, D_s_wr, +4);
        }
        D_s_rd = D_s_wr;
        // Perform dummy reads to skip 4-(N&3) oldest samples.
        for (n = 0; n<(-N & 3); n++)
        {
            AE_L32_XC(t01, D_s_rd, +4);
        }

        D_rd0 = (const ae_int32x2*)D_s_rd;

        AE_LA32X2POS_PC(D_va, D_rd0);

        // Load 4 oldest samples from the delay line.
        // Q31
        AE_LA32X2_IC(t01, D_va, D_rd0);
        AE_LA32X2_IC(t23, D_va, D_rd0);

        // Load next two samples for even accumulators. Altogether we have
        // 8 samples residing in 4 AE registers.
        // Q31
        AE_LA32X2_IC(t45, D_va, D_rd0);
        AE_LA32X2_IC(t67, D_va, D_rd0);

        // Load the next 4 tap coefficients.
        // Q31
        AE_L32X2_IP(c0, C, +8);
        AE_L32X2_IP(c1, C, +8);

        // Q17.47 <- QQ17.47 + [ Q31*Q31 ] - 15 with asymmetric rounding
        q0 = AE_MULF32R_HH(t01, c0);
        AE_MULAF32R_LL(q0, t01, c0);
        AE_MULAF32R_HH(q0, t23, c1);
        AE_MULAF32R_LL(q0, t23, c1);

        q1 = AE_MULF32R_LH(t01, c0);
        AE_MULAF32R_LH(q1, c0, t23);
        AE_MULAF32R_LH(q1, t23, c1);
        AE_MULAF32R_LH(q1, c1, t45);

        q2 = AE_MULF32R_HH(t23, c0);
        AE_MULAF32R_LL(q2, t23, c0);
        AE_MULAF32R_HH(q2, t45, c1);
        AE_MULAF32R_LL(q2, t45, c1);

        t01 = t45;
        t23 = t67;

        __Pragma("loop_count min=1")
        for (m = 0; m<(M >> 2); m++)
        {
            // Load next two samples for even accumulators. Altogether we have
            // 8 samples residing in 4 AE registers.
            // Q31
            AE_LA32X2_IC(t45, D_va, D_rd0);
            AE_LA32X2_IC(t67, D_va, D_rd0);

            // Load the next 4 tap coefficients.
            // Q31
            AE_L32X2_IP(c0, C, +8);
            AE_L32X2_IP(c1, C, +8);

            // Q17.47 <- QQ17.47 + [ Q31*Q31 ] - 15 with asymmetric rounding
            AE_MULAF32R_HH(q0, t01, c0);
            AE_MULAF32R_LL(q0, t01, c0);
            AE_MULAF32R_HH(q0, t23, c1);
            AE_MULAF32R_LL(q0, t23, c1);

            AE_MULAF32R_LH(q1, t01, c0);
            AE_MULAF32R_LH(q1, c0, t23);
            AE_MULAF32R_LH(q1, t23, c1);
            AE_MULAF32R_LH(q1, c1, t45);

            AE_MULAF32R_HH(q2, t23, c0);
            AE_MULAF32R_LL(q2, t23, c0);
            AE_MULAF32R_HH(q2, t45, c1);
            AE_MULAF32R_LL(q2, t45, c1);

            t01 = t45;
            t23 = t67;
        }
        Y = (ae_f32*)(y + N - 1);
        // Convert and save 1..3 outputs.
        switch (N & 3)
        {
        case 3: AE_S32RA64S_IP(q2, Y, -4);
        case 2: AE_S32RA64S_IP(q1, Y, -4);
        }

        AE_S32RA64S_IP(q0, Y, -4);
    }
    bkfir->wrIx = (int)((int32_t *)D_s_wr - bkfir->delayLine);

} // bkfira32x32_process()
