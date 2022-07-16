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
	NatureDSP Signal Processing Library. FFT part
  C code optimized for Fusion
*/
/*===========================================================================
Fast Fourier Transforms:
fft_cplx             FFT on Complex Data
fft_real             FFT on Real Data
ifft_cplx            IFFT on Complex Data
ifft_real            Inverse FFT Forming Real Data
fft_cplx<prec>_ie    FFT on Complex Data with optimized memory usage
fft_real<prec>_ie    FFT on Real Data with optimized memory usage
ifft_cplx<prec>_ie   IFFT on Complex Data with optimized memory usage
ifft_real<prec>_ie   Inverse FFT Forming Real Data with optimized memory usage
dct                  Discrete Cosine Transform

There are limited combinations of precision and scaling options available:
----------------+---------------------------------------------------------------
FFT/IFFT        | Scaling options                        | Restrictions on the
                |                                        | input dynamic range
----------------+---------------------------------------------------------------
cplx24x24       | 0 ÿ no scaling                         | input signal < 2^23/(2*N),
                |                                        | N-fft-size
real24x24       | 1 ÿ 24-bit scaling                     |        none
                | 2 ÿ 32-bit scaling on the first stage  |        none
                | and 24-bit scaling later               |        none
                | 3 ÿ fixed scaling before each stage    |        none
------------------------------------------------------------------------------------
cplx32x16       | 3 ÿ fixed scaling before each stage    |        none
cplx16x16       | 3 ÿ fixed scaling before each stage    |        none
cplx32x16_ie    | 3 ÿ fixed scaling before each stage    |        none
cplx24x24_ie    | 3 ÿ fixed scaling before each stage    |        none
real32x16       | 3 ÿ fixed scaling before each stage    |        none
real16x16       | 3 ÿ fixed scaling before each stage    |        none
real32x16_ie    | 3 ÿ fixed scaling before each stage    |        none
real24x24_ie    | 3 ÿ fixed scaling before each stage    |        none
real32x16_ie_24p| 3 ÿ fixed scaling before each stage    |        none
----------------+---------------------------------------------------------------
real24x24_ie_24p| 1 ÿ 24-bit scaling                     |        none
----------------+---------------------------------------------------------------
DCT:            |
----------------+---------------------------------------------------------------
24x24	          | 0 ÿ no scaling                         |        none
32x16	          | 0 ÿ no scaling                         |        none
----------------+---------------------------------------------------------------

===========================================================================*/
#include "NatureDSP_Signal.h"
#include "fft_cplx_twiddles.h"
#include "common.h"
#include "baseop.h"

/*-------------------------------------------------------------------------
  FFT on complex data
  NOTES:
  1. bit-reversing reordering is done here. 
  2. FFT does not make scaling of  input data and it should be done externally 
     to avoid possible overflows.
  3. FFT runs in-place algorithm so INPUT DATA WILL BE DAMAGED after call

  Input:
  x[N*2]    - input signal, Q15. Real and imaginary data are interleaved and 
              real data goes first
  N         - FFT size
  Output:
  y[N*2]    - output spectrum, Q15

  Restrictions:
  arrays should not be overlapping
  x,y - should be aligned by 4 bytes
  N   - may be 16,32,64,128,256 or 512.

  PERFORMANCE NOTES:
  none
-------------------------------------------------------------------------*/



#define IDFT4X1(x0, x1, x2, x3)\
{                                                           \
ae_int32x2 s0, s1, d0, d1 ;                                 \
            s0 = AE_ADD32S(x0, x2);                         \
            d0 = AE_SUB32S(x0, x2);                         \
            s1 = AE_ADD32S(x1, x3);                         \
            d1 = AE_SUB32S(x1, x3);                         \
            d1 = AE_SEL32_LH(d1, d1);                       \
            x0 = AE_ADD32S(s0, s1);                         \
            x2 = AE_SUB32S(s0, s1);                         \
            x3 = AE_ADDSUB32S(d0, d1);                      \
            x1 = AE_SUBADD32S(d0, d1);                      \
}


inline_ void _cmult32x32j(ae_int32x2 *result, ae_int32x2 *x, ae_int32x2 *y)
{

#if 0
    int32_t re = L_add_ll(L_mpy_ll(x->_[0], y->_[0]), L_mpy_ll(x->_[1], y->_[1]));
    int32_t im = L_sub_ll(L_mpy_ll(x->_[1], y->_[0]), L_mpy_ll(x->_[0], y->_[1]));
    result->_[0] = re;
    result->_[1] = im;
#else
#if 0
    ae_f32x2   Xswap;
    ae_f32x2 _YimYim, YreYre, r;

    YreYre = AE_SEL32_HH(*y, *y);
    _YimYim = AE_SEL32_LL(AE_NEG32S(*y), *y);
    Xswap = AE_INTSWAP(*x);

    r = AE_MULFP32X2RAS(*x, YreYre);
    AE_MULAFP32X2RAS(r, Xswap, _YimYim);
    *result = r;
#else
    {
        ae_f64 re, im;
        ae_f32x2 r;

        re = AE_MULF32S_HH(*x, *y);
        im = AE_MULF32S_LH(*x, *y);
        AE_MULAF32S_LL(re, *x, *y);
        AE_MULSF32S_LH(im, *y, *x);

        r = AE_ROUND32X2F64SASYM(re, im);
        *result = r;
    }
#endif

#endif
}

#define FIRST_STAGE_SCALE 3

static void stage_inner_DFT4(int32_t *x, int32_t *y, const int32_t *twd, int N, int stride_bytes, int shift)
{

    ae_int32x2 * restrict px = (ae_int32x2 *)x;
    ae_int32x2 * restrict py = (ae_int32x2 *)y;
    ae_int32x2 * restrict ptw = (ae_int32x2 *)twd;

    int M = 1 << (XT_NSA(stride_bytes) - XT_NSA(N) + 1);    /* (N*sizeof(complex_fract32) / (4 * stride_bytes)); */
    int twstep = M;                                         /* N / (stride_bytes / 2); */
    int j;

    ae_int32x2 x0, x1, x2, x3;
    ae_int32x2 t1, t2, t3;
    WUR_AE_CBEGIN0((unsigned)twd);
    WUR_AE_CEND0((unsigned)(twd + 3 * N / 4 / M*twstep * 2));
    ptw = (ae_int32x2 *)twd;
    px = (ae_int32x2 *)x;
    py = (ae_int32x2 *)y;

    __Pragma("loop_count min=2");
    for (j = 0; j < (N >> 2); j++)
    {
        /* 24 cycles per pipeline stage in steady state with unroll=1   */
        /* 25 cycles per pipeline stage in steady state with unroll=1 merged loops !!*/
        int inc = 8;

        AE_L32X2_XP(t1, ptw, 8);
        AE_L32X2_XP(t2, ptw, 8);
        AE_L32X2_XC(t3, ptw, twstep * 24 - 16);

        XT_MOVEQZ(inc, stride_bytes * 3 + 8, (uintptr_t)ptw - (uintptr_t)twd);


        x1 = AE_L32X2_X(px, stride_bytes);
        x3 = AE_L32X2_X(px, stride_bytes * 3);
        x2 = AE_L32X2_X(px, stride_bytes * 2);
        x0 = AE_L32X2_I(px, 0); //  AE_L32X2_XP(x0, px, inc);

        IDFT4X1(x0, x1, x2, x3);

        _cmult32x32j(&x1, &x1, &t1);
        _cmult32x32j(&x2, &x2, &t2);
        _cmult32x32j(&x3, &x3, &t3);

        x0 = AE_SRAA32RS(x0, shift);
        x1 = AE_SRAA32RS(x1, shift);
        x2 = AE_SRAA32RS(x2, shift);
        x3 = AE_SRAA32RS(x3, shift);

        px = (ae_int32x2*)(inc + (uintptr_t)px);
        AE_S32X2_X(x2, py, stride_bytes);
        AE_S32X2_X(x3, py, stride_bytes * 3);
        AE_S32X2_X(x1, py, stride_bytes * 2);
        AE_S32X2_XP(x0, py, inc);
    }
}
/*-------------------------------------------------------------------------
  These functions make inverse FFT on complex data.
  NOTES:
  1. Bit-reversing permutation is done here. 
  2. FFT runs in-place algorithm so INPUT DATA WILL APPEAR DAMAGED after 
     the call

  Precision: 
  32x32  32-bit input/outputs, 32-bit twiddles
  24x24  24-bit input/outputs, 24-bit twiddles
  32x16  32-bit input/outputs, 16-bit twiddles
  16x16  16-bit input/outputs, 16-bit twiddles

  Input:
  x[2*N]     complex input spectrum. Real and imaginary data are interleaved 
             and real data goes first
  N          FFT size
  scalingOpt scaling option
  Output:
  y[2*N]     complex output signal. Real and imaginary data are interleaved 
             and real data goes first

  Returned value: total number of right shifts occurred during scaling 
                  procedure

  Restrictions:
  x,y should not overlap
  x,y - aligned on a 8-bytes boundary
  N   - 2^m: 16...4096

-------------------------------------------------------------------------*/
int ifft_cplx32x32( int32_t* y,int32_t* x,fft_handle_t h,int scalingOption)
{


    ae_int32x2 * restrict px0;
    ae_int32x2 * restrict px1;
    ae_int32x2 * restrict px2;
    ae_int32x2 * restrict px3;
    ae_int32x2 * restrict py0;
    ae_int32x2 * restrict py1;
    ae_int32x2 * restrict py2;
    ae_int32x2 * restrict py3;
    const ae_int32x2 * restrict ptwd;

    const tFftDescr* pDescr = (const tFftDescr*)h;
    int stride, scale = FIRST_STAGE_SCALE;
    int N;
    int i;


    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);
    NASSERT(scalingOption == 3);


    N = pDescr->N;

    stride = N; // The stride is quartered with every iteration of the outer loop.

    px0 = (ae_int32x2 *)(x + 0 * N / 4 * 2);
    px1 = (ae_int32x2 *)(x + 1 * N / 4 * 2);
    px2 = (ae_int32x2 *)(x + 2 * N / 4 * 2);
    px3 = (ae_int32x2 *)(x + 3 * N / 4 * 2);
    py0 = (ae_int32x2 *)(x + 0 * N / 4 * 2);
    py1 = (ae_int32x2 *)(x + 1 * N / 4 * 2);
    py2 = (ae_int32x2 *)(x + 2 * N / 4 * 2);
    py3 = (ae_int32x2 *)(x + 3 * N / 4 * 2);

    ptwd = (const ae_int32x2 *)(pDescr->twd);

    /* 23 cycles per pipeline stage in steady state with unroll=1 */
    __Pragma("loop_count min=2");
    for (i = 0; i < N / 4; i++)
    {
        ae_int32x2 x0, x1, x2, x3;
        ae_int32x2 tw1, tw2, tw3;

        AE_L32X2_IP(tw1, ptwd, sizeof(ae_int32x2));
        AE_L32X2_IP(tw2, ptwd, sizeof(ae_int32x2));
        AE_L32X2_IP(tw3, ptwd, sizeof(ae_int32x2));

        AE_L32X2_IP(x0, px0, sizeof(ae_int32x2));
        AE_L32X2_IP(x1, px1, sizeof(ae_int32x2));
        AE_L32X2_IP(x2, px2, sizeof(ae_int32x2));
        AE_L32X2_IP(x3, px3, sizeof(ae_int32x2));

        x0 = AE_SRAI32(x0, 3);
        x1 = AE_SRAI32(x1, 3);
        x2 = AE_SRAI32(x2, 3);
        x3 = AE_SRAI32(x3, 3);

        IDFT4X1(x0, x1, x2, x3);

        _cmult32x32j(&x1, &x1, &tw1);
        _cmult32x32j(&x2, &x2, &tw2);
        _cmult32x32j(&x3, &x3, &tw3);

        x0 = AE_SRAA32RS(x0, 2);
        x1 = AE_SRAA32RS(x1, 2);
        x2 = AE_SRAA32RS(x2, 2);
        x3 = AE_SRAA32RS(x3, 2);

        AE_S32X2_IP(x0, py0, sizeof(ae_int32x2));
        AE_S32X2_IP(x2, py1, sizeof(ae_int32x2));
        AE_S32X2_IP(x1, py2, sizeof(ae_int32x2));
        AE_S32X2_IP(x3, py3, sizeof(ae_int32x2));
    }


    scale += 2;
    stride >>= 2;

    while (stride >= 8)
    {
        int current_scale = (stride != 8) ? 2 : 1;
        stage_inner_DFT4(x, x, pDescr->twd, N, stride * 2, current_scale);
        scale += current_scale;
        stride >>= 2;
    }


    {
        int32_t i, i0, ai;

        px0 = (ae_int32x2 *)(x);
        py0 = (ae_int32x2 *)(y);
        py1 = (py0 + (N >> 2));
        py2 = (py1 + (N >> 2));
        py3 = (py2 + (N >> 2));

        i = AE_NSAZ32_L(N) + 1;
        ai = ((int32_t)0x1) << i;
        i0 = 0;
        if (stride == 2)
        {
            ae_int32x2  vA0, vA1, vA2, vA3, vB0, vB1, vB2, vB3;
            //--------------------------------------------------------------------------
            // last stage is RADIX2 !!!
            //--------------------------------------------------------------------------
            /*9 cycles per pipeline stage in steady state with unroll=1*/
            __Pragma("loop_count min=2");
            for (i = 0; i < (N >> 2); i++)
            {

                vA1 = AE_L32X2_I(px0, 8);
                vA2 = AE_L32X2_I(px0, 16);
                vA3 = AE_L32X2_I(px0, 24);
                AE_L32X2_XP(vA0, px0, 32);

                vB0 = AE_ADD32S(vA0, vA1);
                vB2 = AE_SUB32S(vA0, vA1);
                vB1 = AE_ADD32S(vA2, vA3);
                vB3 = AE_SUB32S(vA2, vA3);

                AE_S32X2_X(vB0, py0, i0);
                AE_S32X2_X(vB1, py1, i0);
                AE_S32X2_X(vB2, py2, i0);
                AE_S32X2_X(vB3, py3, i0);
                i0 = AE_ADDBRBA32(i0, ai);
            }

        }
        else
        {
            ae_int32x2  vA0, vA1, vA2, vA3;
            //--------------------------------------------------------------------------
            // last stage is RADIX4 !!!
            //--------------------------------------------------------------------------

            /* 9 cycles per pipeline stage in steady state with unroll=1 */
            __Pragma("loop_count min=2");
            for (i = 0; i < (N >> 2); i++)
            {

                vA1 = AE_L32X2_I(px0, 8);
                vA2 = AE_L32X2_I(px0, 16);
                vA3 = AE_L32X2_I(px0, 24);
                AE_L32X2_XP(vA0, px0, 32);

                IDFT4X1(vA0, vA1, vA2, vA3);

                AE_S32X2_X(vA0, py0, i0);
                AE_S32X2_X(vA1, py1, i0);
                AE_S32X2_X(vA2, py2, i0);
                AE_S32X2_X(vA3, py3, i0);

                i0 = AE_ADDBRBA32(i0, ai);

            }
        }
    }

    return scale;
}

