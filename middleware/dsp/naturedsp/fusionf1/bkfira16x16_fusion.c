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
    Real block FIR filter, 16x16-bit, unaligned data and arbitrary M/N allowed
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

/* Instance pointer validation number. */
#define MAGIC     0x29EDBF10

/* Reserve memory for alignment. */
#define ALIGNED_SIZE( size, align ) \
    ((size_t)(size)+(align)-1)

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
    (void*)(((uintptr_t)(addr)+((align)-1)) & ~((align)-1))

#define sz_i16   sizeof(int16_t)


#if !(XCHAL_HAVE_FUSION_16BIT_BASEBAND)
/* code for no Quad MAC option */
/* Filter instance structure. */
typedef struct tag_bkfira16x16_t
{
  int32_t magic;
  int             M; /* Filter length                   */
  const int16_t * h; /* Filter coefficients             */
        int32_t * d; /* Delay line of length M          */
        int32_t * p; /* Pointer into the delay line     */
} bkfira16x16_t, *bkfir16x16_ptr_t;

/* Calculate the memory block size for an FIR filter with given attributes. */
size_t bkfira16x16_alloc( int M )
{
  NASSERT( M > 0 );
  M=(M+3)&~3;
  return 3*M*sizeof(int32_t) + sizeof(bkfira16x16_t) + 7;
} // bkfira16x16_alloc()

/* Initialize the filter structure. The delay line is zeroed. */
bkfirf_handle_t bkfira16x16_init( void * objmem, int M, const int16_t * h )
{
  bkfira16x16_t* bkfir;
  void *           ptr;
  int32_t * restrict pd;
  int16_t * restrict ph;
  int m,M1;
  NASSERT( objmem && M > 0 && h );
  M1=(M+3)&~3;
  /* Partition the memory block */
  ptr     = objmem;
  ph = (int16_t*)((((uintptr_t)ptr)+7)&~7);
  pd = (int32_t*)(ph+M1);
  bkfir=(bkfira16x16_t*)(pd+M1);
  bkfir->magic=MAGIC;
  bkfir->M = M1;
  bkfir->h = ph;
  bkfir->d = bkfir->p = pd;
  /* copy coefficients and clean upd delay line */
  for (m=0; m<M ; m++) ph[m]=h[m];
  for (   ; m<M1; m++) ph[m]=0;
  for (m=0; m<M1; m++) pd[m]=0;
  return bkfir;
} // bkfira16x16_init()

/* process block of samples */
void bkfira16x16_process( bkfir16x16_handle_t _bkfir, 
                         int16_t * restrict  y,
                   const int16_t * restrict  x, int N )
{
    const int32_t* restrict pH;
    const ae_int16x4* restrict pX;
          ae_int16x4* restrict pY;
    const ae_int32x2  * pD01;
    const ae_int32x2  * pD12;
    ae_int32x2* p;
    ae_valign  ay,ax;
    ae_f64       q0, q1, q2, q3;
    ae_int32x2   t,X01,X12,X23,X34,XN0,XN1;
    ae_f16x4     H01,H23;
    ae_int16x4   XX;
    ae_valign    ad01,ad12;
    int n,m,M;
    const int16_t* h;

    bkfira16x16_t* bkfir;
    NASSERT(_bkfir);
    bkfir=(bkfira16x16_t*)_bkfir;
    NASSERT(bkfir->magic==MAGIC);
    NASSERT(bkfir->h);
    NASSERT(bkfir->d);
    NASSERT(bkfir->p);
    NASSERT_ALIGN(bkfir->h,8);
    NASSERT_ALIGN(bkfir->d,8);
    NASSERT((bkfir->M%4)==0);
    NASSERT(x);
    NASSERT(y);
    if(N<=0) return;
    M=bkfir->M;
    NASSERT(N>0);
    NASSERT(M>0);
    h=bkfir->h;
    pX=(const ae_int16x4*)x;
    ax=AE_LA64_PP(pX);
    WUR_AE_CBEGIN0( (uintptr_t)( bkfir->d            ) );
    WUR_AE_CEND0  ( (uintptr_t)( bkfir->d + bkfir->M ) );
    ay=AE_ZALIGN64();
    pY=(ae_int16x4*)y;
    p=(ae_int32x2*)(bkfir->p);
    /* process by 4 samples */
    for (n=0; n<(N&~3); n+=4)
    {
        pH=(const int32_t* )h;
        pD01=(const ae_int32x2*)(p);
        pD12=(const ae_int32x2*)(((int32_t*)p)+1);
        AE_LA32X2NEG_PC(ad01,pD01);
        AE_LA32X2NEG_PC(ad12,pD12);
        AE_LA32X2_RIC(X12,ad12,pD12);
        AE_LA32X2_RIC(t,ad01,pD01);
        AE_LA16X4_IP(XX,ax,pX);
        XN0=AE_CVT32X2F16_32(XX);
        XN1=AE_CVT32X2F16_10(XX);
        X12=(AE_INTSWAP(XN0));
        X34=(AE_INTSWAP(XN1));
        X01=AE_SEL32_HL(XN0,t);
        X23=AE_SEL32_HL(XN1,XN0);

        AE_L32_IP(t, castxcc(ae_int32, pH), sizeof(int32_t)); H01 = AE_MOVF16X4_FROMINT32X2(t);
        q0=q1=AE_ZERO();
        q2=AE_MULZAAFD32X16_H2_L3(X23,H01);
        q3=AE_MULZAAFD32X16_H2_L3(X34,H01);
        __Pragma("loop_count min=1")
        for (m=0; m<M-2; m+=2)
        {
            AE_L32_IP(t, castxcc(ae_int32, pH), sizeof(int32_t)); H23 = AE_MOVF16X4_FROMINT32X2(t);
            AE_MULAAFD32X16_H2_L3(q0,X01,H01);
            AE_MULAAFD32X16_H2_L3(q1,X12,H01);
            AE_MULAAFD32X16_H2_L3(q2,X01,H23);
            AE_MULAAFD32X16_H2_L3(q3,X12,H23);
            H01=H23;
            AE_LA32X2_RIC(X01,ad01,pD01);
            AE_LA32X2_RIC(X12,ad12,pD12);
        }
        AE_MULAAFD32X16_H2_L3(q0,X01,H01);
        AE_MULAAFD32X16_H2_L3(q1,X12,H01);

        AE_S32_L_XC(AE_INTSWAP(XN0), castxcc(ae_int32, p), 4); AE_S32_L_XC(XN0, castxcc(ae_int32, p), 4);
        AE_S32_L_XC(AE_INTSWAP(XN1), castxcc(ae_int32, p), 4); AE_S32_L_XC(XN1, castxcc(ae_int32, p), 4);
        XN0 = AE_TRUNCA32X2F64S(q0,q1,  16);
        XN1 = AE_TRUNCA32X2F64S(q2,q3,  16);
        XX = AE_ROUND16X4F32SASYM(XN0, XN1);
        AE_SA16X4_IP(XX,ay,pY);
    }
    AE_SA64POS_FP(ay,pY);
    /* process last 1...3 samples */
    N&=3;
    if(N)
    {
        ae_int32x2 t0;
        pH=(const int32_t* )h;
        pD01=(const ae_int32x2*)(p);
        pD12=(const ae_int32x2*)(((int32_t*)p)+1);
        AE_LA32X2NEG_PC(ad01,pD01);
        AE_LA32X2NEG_PC(ad12,pD12);
        AE_LA32X2_RIC(X12,ad12,pD12);
        AE_LA32X2_RIC(t,ad01,pD01);
        AE_LA16X4_IP(XX,ax,pX);
        XN0=AE_CVT32X2F16_32(XX);
        XN1=AE_CVT32X2F16_10(XX);
        X12=(AE_INTSWAP(XN0));
        X34=(AE_INTSWAP(XN1));
        X01=AE_SEL32_HL(XN0,t);
        X23=AE_SEL32_HL(XN1,XN0);

        AE_L32_IP(t, castxcc(ae_int32, pH), sizeof(int32_t)); H01 = AE_MOVF16X4_FROMINT32X2(t);
        q0=q1=AE_ZERO();
        q2=AE_MULZAAFD32X16_H2_L3(X23,H01);
        q3=AE_MULZAAFD32X16_H2_L3(X34,H01);
        __Pragma("loop_count min=1")
        for (m=0; m<M-2; m+=2)
        {
            AE_L32_IP(t,castxcc(ae_int32, pH),sizeof(int32_t)); H23=AE_MOVF16X4_FROMINT32X2(t);
            AE_MULAAFD32X16_H2_L3(q0,X01,H01);
            AE_MULAAFD32X16_H2_L3(q1,X12,H01);
            AE_MULAAFD32X16_H2_L3(q2,X01,H23);
            AE_MULAAFD32X16_H2_L3(q3,X12,H23);
            H01=H23;
            AE_LA32X2_RIC(X01,ad01,pD01);
            AE_LA32X2_RIC(X12,ad12,pD12);
        }
        AE_MULAAFD32X16_H2_L3(q0,X01,H01);
        AE_MULAAFD32X16_H2_L3(q1,X12,H01);
                    t0 = AE_TRUNCA32X2F64S(q0,q0,  16); XX = AE_ROUND16X4F32SASYM(t0, t0); AE_S16_0_IP(XX,castxcc(ae_int16, pY), sizeof(int16_t)); AE_S32_L_XC(AE_INTSWAP(XN0), castxcc(ae_int32, p), 4);
        if (N>1) {  t0 = AE_TRUNCA32X2F64S(q1,q1,  16); XX = AE_ROUND16X4F32SASYM(t0, t0); AE_S16_0_IP(XX,castxcc(ae_int16, pY), sizeof(int16_t)); AE_S32_L_XC(XN0, castxcc(ae_int32, p), 4); }
        if (N>2) {  t0 = AE_TRUNCA32X2F64S(q2,q2,  16); XX = AE_ROUND16X4F32SASYM(t0, t0); AE_S16_0_IP(XX,castxcc(ae_int16, pY), sizeof(int16_t)); AE_S32_L_XC(AE_INTSWAP(XN1), castxcc(ae_int32, p), 4); }
    }
    bkfir->p=(int32_t*)p;
} /* bkfira16x16_process() */

#else
/* code for Quad MAC option */
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
size_t bkfira16x16_alloc(int M)
{
    int _M;
    _M = M + (-M & 3);
    NASSERT(M > 0);

    return (ALIGNED_SIZE(sizeof(bkfira16x16_t), 8)
        + // Delay line
        // Allocated memory for delay line has increased by 2 samples
        // to avoid memory bank conflicts and get the best performance
        ALIGNED_SIZE((_M + 4 + 2)*sz_i16, 8)
        + // Filter coefficients
        ALIGNED_SIZE((_M + 4)*sz_i16, 8));
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
    delLine = (int16_t *)ALIGNED_ADDR(ptr, 8);
    // Allocated memory for delay line has increased by 2 samples
    // to avoid memory bank conflicts and get the best performance
    ptr     = delLine + _M + 4 + 2;
    coef    = (int16_t *)ALIGNED_ADDR(ptr, 8);
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

    for (m = 0; m<_M + 4; m++)
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
    bkfir->delayLen  = _M + 4;
    bkfir->wrIx      = 0;

    return (bkfir);
} /* bkfira16x16_init() */

/* process block of samples */
void bkfira16x16_process( bkfira16x16_handle_t _bkfir,
                          int16_t * restrict  y,
                    const int16_t * restrict  x, int N)
{
    bkfira16x16_ptr_t bkfir = (bkfira16x16_ptr_t)_bkfir;

    const ae_int16x4 *          C;
          ae_int16x4 * restrict D_wr;
          ae_int16   * restrict D_s_wr;
    const ae_int16x4 *          D_rd0;
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

    ae_int16x4 d01, d23, d12, d34;
    ae_int64   q0, q1, q2, q3;
    ae_int64   z;
    ae_f32x2   t0, t1;
    ae_int16x4 c0;
    
    int M, wrIx;
    int m, n;

    if (N <= 0) return;
    NASSERT(bkfir && bkfir->magic == MAGIC && y && x);
    NASSERT(IS_ALIGN(bkfir->delayLine) &&
        IS_ALIGN(bkfir->coef) &&
        IS_ALIGN(bkfir->coef + bkfir->M + 4) &&
        IS_ALIGN(bkfir->delayLine + bkfir->delayLen));

    M    = bkfir->M;
    wrIx = bkfir->wrIx;

    //
    // Setup pointers and circular delay line buffer.
    //
    D_wr = (ae_int16x4*)(bkfir->delayLine + wrIx);
    X    = (const ae_int16x4 *)x;
    Y    = (ae_int16     *)y;

    WUR_AE_CBEGIN0((uintptr_t)(bkfir->delayLine));
    WUR_AE_CEND0((uintptr_t)(bkfir->delayLine + bkfir->delayLen));

    z       = AE_CVTQ56A32S(0);
    ay      = AE_ZALIGN64();
    x_align = AE_LA64_PP(X);

    // Break the input signal into 4-samples blocks. For each block, store 4
    // samples to the delay line and compute the filter response.
    //
    for (n = 0; n < (N >> 2); n++)
    {
        // Load 4 input samples.
        AE_LA16X4_IP(d01, x_align, X);

        D_va0 = AE_ZALIGN64();
        // Store 4 samples to the delay line buffer with circular address update.
        AE_SA16X4_IC(d01, D_va0, D_wr);
        AE_SA64POS_FP(D_va0, D_wr);

        // Circular buffer write pointer looks at the oldest sample: M+4 samples
        // back from the newest one. To read the delay buffer separately for
        // even/odd accumulators, the two read pointers are set to M-4th and M-3rd
        // samples. Pointer increment is safe in respect to crossing the circular
        // buffer boundary.
        D_rd0 = D_wr;

        /*
        D_rd1 = (ae_int16x4*)((int16_t*)D_rd0 + 1);
        D_rd2 = (ae_int16x4*)((int16_t*)D_rd0 + 2);
        D_rd3 = (ae_int16x4*)((int16_t*)D_rd0 + 3);
         */
        D_rd1 = D_rd0; AE_L16_XC(d01, castxcc(ae_int16, D_rd1), +2);
        D_rd2 = D_rd1; AE_L16_XC(d01, castxcc(ae_int16, D_rd2), +2);
        D_rd3 = D_rd2; AE_L16_XC(d01, castxcc(ae_int16, D_rd3), +2);

        AE_LA16X4POS_PC(D_va0, D_rd0);
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
            AE_LA16X4_IC(d01, D_va0, D_rd0);
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

    X_s    = (const ae_int16*)X;
    D_s_wr = (ae_int16*)D_wr;
    if (N & 3)
    {
        // Reset the coefficients pointer. Now it looks at the tap corresponding
        // to the oldest sample in the delay line.
        C = (const ae_int16x4*)bkfir->coef;

        // Insert 0..3 input samples into the delay line one-by-one.
        for (n = 0; n < (N & 3); n++)
        {
            AE_L16_IP(d01, X_s, +2);
            AE_S16_0_XC(d01, D_s_wr, +2);
        }
        D_s_rd = D_s_wr;
        // Perform dummy reads to skip 4-(N&3) oldest samples.
        for (n = 0; n < (-N & 3); n++)
        {
            AE_L16_XC(d01, D_s_rd, +2);
        }

        D_rd0 = (const ae_int16x4*)D_s_rd;

        /*
        D_rd1 = (const ae_int16x4*)((int16_t*)D_rd0 + 1);
        D_rd2 = (const ae_int16x4*)((int16_t*)D_rd0 + 2);
         */
        D_rd1 = D_rd0; AE_L16_XC(d01, castxcc(ae_int16, D_rd1), +2);
        D_rd2 = D_rd1; AE_L16_XC(d01, castxcc(ae_int16, D_rd2), +2);

        AE_LA16X4POS_PC(D_va0, D_rd0);
        AE_LA16X4POS_PC(D_va1, D_rd1);
        AE_LA16X4POS_PC(D_va2, D_rd2);

        // Zero the accumulators.
        q0 = z; q1 = z; q2 = z;

        for (m = 0; m < (M >> 2) + 1; m++)
        {
            AE_LA16X4_IC(d01, D_va0, D_rd0);
            AE_LA16X4_IC(d12, D_va1, D_rd1);
            AE_LA16X4_IC(d23, D_va2, D_rd2);
            AE_L16X4_IP(c0, C, +8);

            AE_MULAAAAQ16(q0, d01, c0);
            AE_MULAAAAQ16(q1, d12, c0);
            AE_MULAAAAQ16(q2, d23, c0);
        }

        Y = (ae_int16*)(y + N - 1);

        t0 = AE_TRUNCA32X2F64S(q1, q0, 33);
        t1 = AE_TRUNCA32X2F64S(q2, q2, 33);
        d01 = AE_ROUND16X4F32SASYM(t1, t0);
  
        // Convert and save 1..3 outputs.
        switch (N & 3)
        {
        case 3: d12 = AE_SEL16_5432(d01, d01);  AE_S16_0_IP(d12, Y, -2);
        case 2: d23 = AE_SEL16_4321(d01, d01);  AE_S16_0_IP(d23, Y, -2);
        }
        AE_S16_0_IP(d01, Y, -2);  
    }

    bkfir->wrIx = (int)((int16_t *)D_s_wr - bkfir->delayLine);

} /* bkfira16x16_process() */
#endif

