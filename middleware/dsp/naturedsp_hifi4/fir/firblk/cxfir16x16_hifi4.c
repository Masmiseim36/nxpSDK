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
    Complex block FIR filter, 16x16-bit
    C code optimized for HiFi4
  IntegrIT, 2006-2018
*/

/*-------------------------------------------------------------------------
  Complex Block FIR Filter
  Computes a complex FIR filter (direct-form) using complex IR stored in 
  vector h. The complex data input is stored in vector x. The filter output
  result is stored in vector r. The filter calculates N output samples using 
  M coefficients and requires last M-1 samples in the delay line. Real and
  imaginary parts are interleaved and real parts go first (at even indexes).
  NOTE: 
  1. User application is not responsible for management of delay lines
  2. User has an option to set IR externally or copy from original location 
     (i.e. from the slower constant memory). In the first case, user is 
     responsible for right alignment, ordering and zero padding of filter 
     coefficients - usually array is composed from zeroes (left padding), 
     reverted IR and right zero padding.

  Precision: 
  16x16     16-bit data, 16-bit coefficients, 16-bit outputs
  32x16     32-bit data, 16-bit coefficients, 32-bit outputs
  32x32     32-bit data, 32-bit coefficients, 32-bit outputs
  32x32ep   the same as above but using 72-bit accumulator for intermediate 
            computations
  f         floating point

  Input:
  h[M]      complex filter coefficients; h[0] is to be multiplied with the 
            newest sample, Q15, Q31, floating point
  x[N]      input samples, Q15, Q31, floating point
  N         length of sample block (in complex samples) 
  M         length of filter 
  extIR     if zero, IR is copied from original location, otherwise not
            but user should keep alignment, order of coefficients 
            and zero padding requirements shown below
  Output:			
  y[N]      output samples, Q15, Q31, floating point

  Alignment, ordering and zero padding for external IR  (extIR!=0)
  -----------------+----------+--------------+--------------+----------------
  Function	       |Alignment,|Left zero     |   Coefficient| Right zero 
                   | bytes    |padding, bytes|   order      | padding, bytes
  -----------------+----------+--------------+--------------+----------------
  cxfir16x16_init, |     8    |    4         |  inverted    |  4
  hifi3            |          |              |              |
  cxfir16x16_init, |     8    |  2 before    |  *           |  6 after
  hifi3z/4         |          |  each copy   |              |  each copy
  cxfir32x16_init  |     8    |    4         |  inverted    |  4
  cxfir32x32_init  |     8    |    0         |  inv,conj    |  0
  cxfir32x32ep_init|     8    |    0         |  inv,conj    |  0
  cxfirf_init      |     8    |    0         |  direct      |  0
  -----------------+----------+--------------+--------------+----------------
  * inverted: conjugated copy and (imaginary; real) copy at 4*(M+4) bytes offset

  Restriction:
  x,y       should not overlap
  x,h       aligned on a 8-bytes boundary
  N,M       multiples of 4
-------------------------------------------------------------------------*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fir.h"
/* Common utility and macros declarations. */
#include "common.h"

/* Instance pointer validation number. */
#define MAGIC     0x6B2D04F2

/* Reserve memory for alignment. */
#define ALIGNED_SIZE( size, align ) \
    ((size_t)(size)+(align)-1)

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
    (void*)(((uintptr_t)(addr)+((align)-1)) & ~((align)-1))

#define sz_i16   sizeof(int16_t)
#define sz_i32   sizeof(int32_t)

/* negation -Q31->Q31                            */
inline_ int16_t L_neg_l(int16_t x)
{
    return (x == MIN_INT16) ? MAX_INT16 : -x;
}

/* Filter instance structure. */
typedef struct tag_cxfir16x16_t
{
    uint32_t        magic;     // Instance pointer validation number
    int             M;         // Number of complex filter coefficients
    const int16_t * coef;      // M complex coefficients
    int16_t *       delayLine; // Delay line for complex samples
    int             delayLen;  // Delay line length, in complex samples
    int             wrIx;      // Index of the oldest complex sample

} cxfir16x16_t, *cxfir16x16_ptr_t;

/* Calculate the memory block size for an FIR filter with given attributes. */
size_t cxfir16x16_alloc( int M, int extIR )
{
    NASSERT(M > 0);
    NASSERT(!(M & 3));
    return (ALIGNED_SIZE(sizeof(cxfir16x16_t), 4)
        + // Delay line
        ALIGNED_SIZE(2 * (M + 4)*sz_i16, 8)
        + // Filter coefficients
        (extIR?0:ALIGNED_SIZE(2 * 2 * (M + 4)*sz_i16, 8)));
} /* cxfir16x16_alloc() */

/* Initialize the filter structure. The delay line is zeroed. */
cxfir16x16_handle_t cxfir16x16_init( void *             objmem,
                                     int                M,
                                     int extIR,
                                const complex_fract16 * restrict h)
{
    cxfir16x16_ptr_t cxfir;
    void    *        ptr;
    int16_t *        coef;
    int16_t *        coefB;
    int16_t *        delLine;

    int m;

    NASSERT(objmem && (M > 0) && h);
    NASSERT(!(M & 3) && IS_ALIGN(h));

    //
    // Partition the memory block.
    //

    ptr     = objmem;
    cxfir   = (cxfir16x16_ptr_t)ALIGNED_ADDR(ptr, 4);
    ptr     = cxfir + 1;
    delLine = (int16_t *)ALIGNED_ADDR(ptr, 8);
    ptr     = delLine + 2 * (M + 4);
    if (extIR)
    {
      coef = (int16_t *)h;
    }
    else
    {
      coef    = (int16_t *)ALIGNED_ADDR(ptr, 8);
      ptr     = coef + 2 * 2 * (M + 4);
    }
    NASSERT((int8_t*)ptr - (int8_t*)objmem <= (int)cxfir16x16_alloc(M, extIR));

    //
    // Copy the filter coefficients in reverted order and zero the delay line.
    //
    if (extIR == 0)
    {
      coefB = coef + 2 * (M + 4);

      coef[0] = coef[1] = coefB[0] = coefB[1] = 0;

    
      for (m = 1; m<M + 1; m++)
      {
          coef[2 * m + 0] = h[(M - m)].s.re;
          coef[2 * m + 1] = L_neg_l(h[(M - m)].s.im);

          coefB[2 * m + 0] = h[(M - m)].s.im;
          coefB[2 * m + 1] = h[(M - m)].s.re;
      }

      for (; m<M + 4; m++)
      {
          coef [2 * m + 0]  = coef[2 * m + 1] = 0;
          coefB[2 * m + 0] = coefB[2 * m + 1] = 0;
      }
    }

    for (m = 0; m<M + 4; m++)
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
    cxfir->delayLen     = M + 4;
    cxfir->wrIx         = 0;

    return (cxfir);
} /* cxfir16x16_init() */

/* Put a chunk of input signal into the delay line and compute the filter
 * response. */
void cxfir16x16_process( cxfir16x16_handle_t handle,
                         complex_fract16 * restrict  y,
                   const complex_fract16 * restrict  x, int N)
{
    cxfir16x16_ptr_t cxfir = (cxfir16x16_ptr_t)handle;

    const ae_int16x4 *          C;
          ae_int16x4 * restrict D_wr;
    const ae_int16x4 *          D_rd0;
    const ae_int16x4 *          D_rd1;
    const ae_int16x4 *          D_rd2;
    const ae_int16x4 *          D_rd3;
    const ae_int16x4 *          X;
          ae_int16   * restrict Y;
    
    ae_valign D_va1, D_va3;
    ae_valign ay;

    ae_int16x4 d01, d12, d23, d34;
    ae_int64   q0r, q1r, q2r, q3r;
    ae_int64   q0i, q1i, q2i, q3i;
    ae_int64   z;
    ae_int16x4 c0, c0i;
    ae_f32x2   t0, t1;

    int M, wrIx;
    int m, n;

    if (N <= 0) return;
    NASSERT(cxfir && ((cxfir->magic) == MAGIC) && y && x);
    NASSERT_ALIGN8(x);

    M    = cxfir->M;
    wrIx = cxfir->wrIx;
    NASSERT(!(M&3) && !(N&3));
    NASSERT_ALIGN8(cxfir->delayLine);
    NASSERT_ALIGN8(cxfir->coef);

    //
    // Setup pointers and circular delay line buffer.
    //

    D_wr = (      ae_int16x4 *)(cxfir->delayLine + (2 * wrIx));
    X    = (const ae_int16x4 *)x;
    Y    = (      ae_int16   *)y;

    WUR_AE_CBEGIN0((uintptr_t)(cxfir->delayLine));
    WUR_AE_CEND0  ((uintptr_t)(cxfir->delayLine + (2 * cxfir->delayLen)));
    z = AE_ZERO64();

    ay = AE_ZALIGN64();

    //
    // Break the input signal into 4-samples blocks. For each block, store 4
    // samples to the delay line and compute the filter response.
    //
    __Pragma("loop_count min=1")
    for (n = 0; n < (N >> 2); n++)
    {
        // Reset the coefficients pointer. Now it looks at the tap corresponding
        // to the oldest sample in the delay line.
        C = (const ae_int16x4*)cxfir->coef;

        // Load 4 complex input samples.
        AE_L16X4_IP(d01, X, +8);
        AE_L16X4_IP(d12, X, +8);

        // Store 4 complex samples to the delay line buffer with circular address update.
        AE_S16X4_XC(d01, D_wr, +8);
        AE_S16X4_XC(d12, D_wr, +8);

        D_rd0 = D_wr;
        D_rd1 = (const ae_int16x4*)((int16_t *)D_wr + 2);
        D_rd2 = (const ae_int16x4*)((int16_t *)D_wr + 4);
        D_rd3 = (const ae_int16x4*)((int16_t *)D_wr + 6);

        // Zero the accumulators.
        q0r = z; q1r = z; q2r = z; q3r = z;
        q0i = z; q1i = z; q2i = z; q3i = z;

        AE_LA16X4POS_PC(D_va1, D_rd1);
        AE_LA16X4POS_PC(D_va3, D_rd3);

        __Pragma("loop_count min=3")
        for (m = 0; m < (M >> 1) + 1; m++)
        {
            AE_L16X4_XC (d01, D_rd0, +8);
            AE_LA16X4_IC(d12, D_va1, D_rd1);
            AE_L16X4_XC (d23, D_rd2, +8);
            AE_LA16X4_IC(d34, D_va3, D_rd3);

            c0i = AE_L16X4_X(C, +2*2*(M + 4));
            AE_L16X4_IP (c0, C, +8);

            AE_MULAAAAQ16(q0r, d01, c0);
            AE_MULAAAAQ16(q1r, d12, c0);
            AE_MULAAAAQ16(q2r, d23, c0);
            AE_MULAAAAQ16(q3r, d34, c0);

            AE_MULAAAAQ16(q0i, d01, c0i);
            AE_MULAAAAQ16(q1i, d12, c0i);
            AE_MULAAAAQ16(q2i, d23, c0i);
            AE_MULAAAAQ16(q3i, d34, c0i);
        }

        t0 = AE_TRUNCA32X2F64S(q0r, q0i, 33);
        t1 = AE_TRUNCA32X2F64S(q1r, q1i, 33);
        AE_SA16X4_IP(AE_ROUND16X4F32SASYM(t0, t1), ay, castxcc(ae_int16x4, Y));
        t0 = AE_TRUNCA32X2F64S(q2r, q2i, 33);
        t1 = AE_TRUNCA32X2F64S(q3r, q3i, 33);
        AE_SA16X4_IP(AE_ROUND16X4F32SASYM(t0, t1), ay, castxcc(ae_int16x4, Y));
    }
    AE_SA64POS_FP(ay, Y);
    cxfir->wrIx = (int)((int16_t *)D_wr - cxfir->delayLine) >> 1;
} /* cxfir16x16_process */
