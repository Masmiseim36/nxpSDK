/* ------------------------------------------------------------------------ */
/* Copyright (c) 2020 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
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
/*          Copyright (C) 2015-2020 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
#include "NatureDSP_Signal_fft.h"
#include "common.h"
#include "imgfft_common.h"
#include "fft_x16_common.h"

/*-----------------------------------------------------------
Apply RFFT to rows, find common scale and rescale results
Input:
x[M*N]  real data
Output:
y[M*N]  spectrum
Temporary:
scr[N+M]
Returns common scale factor
-----------------------------------------------------------*/
int ApplyRFFT_toRows(complex_fract16* scr, complex_fract16 *y, int16_t *x, int M, int N)
{
    ae_int16x4 * restrict py;
    ae_int16x4 * restrict px;
    ae_int16x4 y0;
    /* Temporary buffer for N complex samples */
    complex_fract16 *scrY = scr;
    /*  Temporary buffer for M exponents */
    int32_t *Exp = (int32_t*)(scr + N);
    int i, j, maxExpRows = -100;
    fft_handle_t h;
    /* Set complex element [N/2+1] to zero for avoid ferret warning */
    *(int32_t*)&scrY[N / 2 + 1] = 0;

    NASSERT_ALIGN(x   , HIFI_SIMD_WIDTH);
    NASSERT_ALIGN(y   , HIFI_SIMD_WIDTH);
    NASSERT_ALIGN(scrY, HIFI_SIMD_WIDTH);
    NASSERT_ALIGN(Exp , HIFI_SIMD_WIDTH);

    switch (N)
    {
        case 64:   
            h = rfft16_64;   
            break;
        case 128:
            h = rfft16_128;
            break;
        case 256:
            h = rfft16_256;
            break;
        case 512:
            h = rfft16_512;
            break;
        case 1024:
            h = rfft16_1024;
            break;
        case 2048:
            h = rfft16_2048;
            break;
        case 640:
            h = rnfft16_640;
            break;
        case 320:
            h = rnfft16_320;
            break;
        case 480:
            h = rnfft16_480;
            break;
        case 288:
            h = rnfft16_288;
            break;
        case 576:
            h = rnfft16_576;
            break;
        case 144:
            h = rnfft16_144;
            break;
        case 384:
            h = rnfft16_384;
            break;
        case 240:
            h = rnfft16_240;
            break;
        case 352:
            h = rnfft16_352;
            break;
        case 96:
            h = rnfft16_96;
            break;
        case 176:
            h = rnfft16_176;
            break;

        default:
            h = NULL;
            ASSERT(!"ERROR : ApplyRFFT_toRows invalid N");
    } /* switch (N) */

    // Apply FFT to rows, transform size is N
    __Pragma("loop_count min=1");
    for (i = 0; i < M; i++)
    {
        px = (ae_int16x4 *)scrY;
        py = (ae_int16x4 *)&y[i * N];

        Exp[i] = fft_real16x16((int16_t*)scrY, &x[2 * i*N], h, 2);

        maxExpRows = XT_MAX(maxExpRows, Exp[i]);

        /* Copy N/2+2 complex samples into main buffer */
        __Pragma("loop_count min=1");
        for (j = 0; j < N / 4 + 1; j++)
        {
            AE_L16X4_IP(y0, px, sizeof(y0));
            AE_S16X4_IP(y0, py, sizeof(y0));
        }
    }
    __Pragma("loop_count min=1"); 
    for (i = 0; i < M; i++)
    {
        int s = maxExpRows - Exp[i];
        py = (ae_int16x4 *)&y[i * N]; 
        px = (ae_int16x4 *)&y[i * N]; 

        /* Rescale N/2+2 complex samples */
        __Pragma("loop_count min=1");
        for (j = 0; j < N / 4 + 1; j++)
        { /*  2 cycles per pipeline stage in steady state with unroll=1 */
            AE_L16X4_IP(y0, px, sizeof(y0)); 
            y0 = AE_SRAA16RS(y0, s); 
            AE_S16X4_IP(y0, py, sizeof(y0)); 
        }
    }
    return maxExpRows;
} /* ApplyRFFT_toRows */


/*-----------------------------------------------------------
Apply FFT to columns, find common scale and rescale results
Input:
x[M*N]  row spectra
Output:
y[M*N]  spectrum
Temporary:
scr[2*M+N/2+1]
Returns common scale factor
-----------------------------------------------------------*/
int ApplyFFT_toCols(complex_fract16* scr, complex_fract16 *y, complex_fract16 *x, int M, int N)
{
    xtbool2 mask = 0x1;
    fft_handle_t h;
    int i, j, maxExp = -100;
    ae_int16x4 * restrict px1;
    ae_int16x4 * restrict px0;
    ae_int16x4 * restrict py0;
    ae_int16x4 * restrict py1;
    ae_p16x2s * restrict px = (ae_p16x2s *)y;
    ae_p16x2s * restrict py = (ae_p16x2s *)y;
    ae_p16x2s * restrict pz = (ae_p16x2s *)y;
    ae_int32x2 x0, x1;

    complex_fract16 *scrX0 = scr;
    complex_fract16 *scrX1 = scr + M;
    complex_fract16 *scrY0 = scr + 2*M;
    complex_fract16 *scrY1 = scr + 3*M;

    int* Exp = (int*)(scr + 4 * M);

    NASSERT_ALIGN(x,HIFI_SIMD_WIDTH);
    NASSERT_ALIGN(y,HIFI_SIMD_WIDTH);
    NASSERT_ALIGN(scrX0,HIFI_SIMD_WIDTH);
    NASSERT_ALIGN(scrY0,HIFI_SIMD_WIDTH);
    NASSERT_ALIGN(scrX1, HIFI_SIMD_WIDTH);
    NASSERT_ALIGN(scrY1, HIFI_SIMD_WIDTH);
    NASSERT_ALIGN(Exp, HIFI_SIMD_WIDTH);

    switch (M)
    {
        case 64:
            h = cfft16_64;
            break;
        case 128:
            h = cfft16_128;
            break;
        case 256:
            h = cfft16_256;
            break;
        case 512:
            h = cfft16_512;
            break;
        case 1024:
            h = cfft16_1024;
            break;
        case 2048:
            h = cfft16_2048;
            break;
        case 320:
            h = cnfft16_320;
            break;
        case 480:
            h = cnfft16_480;
            break;
        case 144:
            h = cnfft16_144;
            break;
        case 288:
            h = cnfft16_288;
            break;
        case 96:
            h = cnfft16_96;
            break;
        case 240:
            h = cnfft16_240;
            break;
        case 176:
            h = cnfft16_176;
            break;
        case 352:
            h = cnfft16_352;
            break;
        case 576:
            h = cnfft16_576;
            break;
        case 640:
            h = cnfft16_640;
            break;
        case 384:
            h = cnfft16_384;
            break;
        default:
            h = NULL;
            ASSERT(!"ERROR : ApplyCFFT_toCols invalid N");
    } /* switch (M) */


    // Apply FFT to columns, transform size is M
    __Pragma("loop_count min=2 factor=2");
    for (j = 0; j < N / 2; j+=2)
    {
        ae_int16x4 x10, x32, y10, y32;

        px0 = (ae_int16x4 *)&x[j];
        py0 = (ae_int16x4 *)scrX0;
        py1 = (ae_int16x4 *)scrX1;
        /* Store two columns into temporary buffers */

        __Pragma("loop_count min=2 factor=2");
        for (i = 0; i < M; i+=2)
        {
#if 0
            // Reference code 
            scrX0[i] = x[j + i*N];                      // x0
            scrX0[(i + 1)] = x[j + (i + 1)*N];          // x2
            scrX1[i] = x[(j + 1) + i*N];                // x1
            scrX1[(i + 1)] = x[(j + 1) + (i + 1)*N];    // x3
#else
            AE_L16X4_XP(x10, px0, N * sizeof(complex_fract16));
            AE_L16X4_XP(x32, px0, N * sizeof(complex_fract16));

            y32 = AE_SEL16_5410(x10, x32);
            y10 = AE_SEL16_7632(x10, x32);

            AE_S16X4_IP(y10, py0, 2 * sizeof(complex_fract16));
            AE_S16X4_IP(y32, py1, 2 * sizeof(complex_fract16));
#endif
        }
        /* Apply complex FFT */
        Exp[j] = fft_cplx16x16((int16_t*)scrY0, (int16_t*)scrX0, h, 2);
        Exp[(j + 1)] = fft_cplx16x16((int16_t*)scrY1, (int16_t*)scrX1, h, 2);


        px0 = (ae_int16x4 *)scrY0;
        px1 = (ae_int16x4 *)scrY1;
        py0 = (ae_int16x4 *)&y[j];

        /* Store two columns into output buffer */
        __Pragma("loop_count min=2 factor=2"); 
        for (i = 0; i < M; i+=2)
        {
#if 0
            // Reference code
            y[(j + 1) + i*N] = scrY1[i];
            y[j + i*N] = scrY0[i];


            y[(j + 1) + (i + 1)*N] = scrY1[(i + 1)];
            y[j + (i + 1)*N] = scrY0[(i + 1)];
#else
            AE_L16X4_XP(x10, px0, 2 * sizeof(complex_fract16));
            AE_L16X4_XP(x32, px1, 2 * sizeof(complex_fract16));

            y32 = AE_SEL16_5410(x10, x32);
            y10 = AE_SEL16_7632(x10, x32);

            AE_S16X4_XP(y10, py0, N * sizeof(complex_fract16));
            AE_S16X4_XP(y32, py0, N * sizeof(complex_fract16));
#endif
        }
        maxExp = XT_MAX(maxExp, Exp[j]);
        maxExp = XT_MAX(maxExp, Exp[(j + 1)]);
    }
    /* Process last column */
    {
        __Pragma("loop_count min=2 factor=2");
        for (i = 0; i < M; i++)
        {
            scrX0[i] = x[j + i*N];
        }

        Exp[j] = fft_cplx16x16((int16_t*)scrY0, (int16_t*)scrX0, h, 2);

        __Pragma("loop_count min=2 factor=2");
        for (i = 0; i < M; i++)
        {
            y[j + i*N] = scrY0[i];
        }

        maxExp = XT_MAX(maxExp, Exp[j]);
    }



#if 0
    // Reference code
    y[0].s.re = S_shr_s(y[0].s.re, maxExp - Exp[0]);
    y[0].s.im = S_shr_s(y[0].s.im, maxExp - Exp[0]);
    y[N / 2].s.re = S_shr_s(y[N / 2].s.re, maxExp - Exp[N / 2]);
    y[N / 2].s.im = S_shr_s(y[N / 2].s.im, maxExp - Exp[N / 2]);
#else
    x0 = AE_L16X2M_X(px, 0   );
    x1 = AE_L16X2M_X(px, N/2 *sizeof(complex_fract16) );

    x0 = AE_SRAA32(x0, maxExp - Exp[0]);
    x1 = AE_SRAA32(x1, maxExp - Exp[N / 2]);

    AE_S16X2M_I(x0, py, 0); 
    AE_S16X2M_X(x1, py, N / 2 * sizeof(complex_fract16));
#endif

#if 0
    // Reference code
    for (i = 1; i < M; i++)
    {     
        // j = 0, j = N/2
        y[i*N + 0].s.re = S_shr_s(y[i*N + 0].s.re, maxExp - Exp[0]);
        y[i*N + 0].s.im = S_shr_s(y[i*N + 0].s.im, maxExp - Exp[0]);
        y[i*N + N / 2].s.re = S_shr_s(y[i*N + N / 2].s.re, maxExp - Exp[N / 2]);
        y[i*N + N / 2].s.im = S_shr_s(y[i*N + N / 2].s.im, maxExp - Exp[N / 2]);
    }
#else
    __Pragma("loop_count min=1");
    for (i = 1; i < M; i++)
    {
        // j = 0, j = N/2
        x0 = AE_L16X2M_X(px, i*N*sizeof(complex_fract16)          );
        x1 = AE_L16X2M_X(px, (N / 2 +i*N)* sizeof(complex_fract16));

        x0 = AE_SRAA32(x0, maxExp - Exp[0]);
        x1 = AE_SRAA32(x1, maxExp - Exp[N / 2]);

        AE_S16X2M_X(x0, py, i*N*sizeof(complex_fract16));
        AE_S16X2M_X(x1, py, (N / 2 + i*N)* sizeof(complex_fract16));
    }
#endif

#if 0
    // Reference code
    for (j = 1; j < N / 2; j++)
    {
        // i = 0
        int16_t re, im; 
        re = S_shr_s(y[j].s.re, maxExp - Exp[j]); 
        im = S_shr_s(y[j].s.im, maxExp - Exp[j]);
        y[j].s.re = re;
        y[j].s.im = im;
        y[N-j].s.re = re;
        y[N-j].s.im = S_neg_s( im );
    }
#else
    __Pragma("loop_count min=1");
    for (j = 1; j < N / 2; j++)
    {
        // i = 0
        x0 = AE_L16X2M_X(px, j * sizeof(complex_fract16));
        x0 = AE_SRAA32(x0, maxExp - Exp[j]);
        AE_S16X2M_X(x0, py, j*sizeof(complex_fract16));
        /* x0 =  conj (x0) */
        AE_MOVT32X2(x0, AE_NEG32S(x0), mask);
        AE_S16X2M_X(x0, py, (N - j)*sizeof(complex_fract16));
    }
#endif
#if 0
    // Reference code
    for (j = 1; j < N / 2; j++)
    {
        int shift = maxExp - Exp[N / 2 - j]; 
        for (i = 1; i < M; i++)
        {
            int s = (M - i)*N + N/2 - j; 
            int d = i*N + (j + N / 2);
            int16_t re = S_shr_s(y[s].s.re, shift);
            int16_t im = S_shr_s(y[s].s.im, shift);
            y[s].s.re = re; 
            y[s].s.im = im;
            y[d].s.re = re;
            y[d].s.im = S_neg_s(im);
        }
    }
#else
    __Pragma("loop_count min=1"); 
    for (j = 1; j < N / 2; j++)
    {
        int shift = maxExp - Exp[N / 2 - j];
        px = (ae_p16x2s*)(y + M*N + N / 2 - j); 
        py = (ae_p16x2s*)(y + M*N + N / 2 - j);
        pz = (ae_p16x2s*)(y + 0*N + (j + N / 2));

        __Pragma("loop_count min=1");
        for (i = 1; i < M; i++)
        {
            ae_int32x2 x;
            AE_L16X2M_XU(x, px, -N*sizeof(complex_fract16)); 

            x = AE_SRAA32(x, shift); 
            AE_S16X2M_XU(x, py, -N*sizeof(complex_fract16));
            AE_MOVT32X2(x, AE_NEG32S(x), mask);
            AE_S16X2M_XU(x, pz, N*sizeof(complex_fract16));
        }
    }
#endif
    return maxExp;
} /* ApplyFFT_toCols */
