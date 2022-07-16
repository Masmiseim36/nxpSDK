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
Apply IFFT to columns, find common scale and rescale results
Input:
x[M*N]  row spectra
Output:
y[M*N]  spectrum
Temporary:
scr[2*M+N/2+1]
Returns common scale factor
-----------------------------------------------------------*/
int ApplyIFFT_toCols(complex_fract16* scr, complex_fract16 *y, complex_fract16 *x, int M, int N)
{
    fft_handle_t h;
    int i, j, maxExp = -100;
    ae_int16x4 * restrict px1;
    ae_int16x4 * restrict px0;
    ae_int16x4 * restrict py0;
    ae_int16x4 * restrict py1;
    ae_p16x2s * restrict px = (ae_p16x2s *)y;
    ae_p16x2s * restrict py = (ae_p16x2s *)y;

    complex_fract16 *scrX0 = scr;
    complex_fract16 *scrX1 = scr + M;
    complex_fract16 *scrY0 = scr + 2 * M;
    complex_fract16 *scrY1 = scr + 3 * M;

    int* Exp = (int*)(scr + 4 * M);

    NASSERT_ALIGN(x, HIFI_SIMD_WIDTH);
    NASSERT_ALIGN(y, HIFI_SIMD_WIDTH);
    NASSERT_ALIGN(scrX0, HIFI_SIMD_WIDTH);
    NASSERT_ALIGN(scrY0, HIFI_SIMD_WIDTH);
    NASSERT_ALIGN(scrX1, HIFI_SIMD_WIDTH);
    NASSERT_ALIGN(scrY1, HIFI_SIMD_WIDTH);
    NASSERT_ALIGN(Exp, HIFI_SIMD_WIDTH);

    switch (M)
    {
        case 64:
            h = cifft16_64;
            break;
        case 128:
            h = cifft16_128;
            break;
        case 256:
            h = cifft16_256;
            break;
        case 512:
            h = cifft16_512;
            break;
        case 1024:
            h = cifft16_1024;
            break;
        case 2048:
            h = cifft16_2048;
            break;
        case 320:
            h = cinfft16_320;
            break;
        case 480:
            h = cinfft16_480;
            break;
        case 144:
            h = cinfft16_144;
            break;
        case 288:
            h = cinfft16_288;
            break;
        case 96:
            h = cinfft16_96;
            break;
        case 240:
            h = cinfft16_240;
            break;
        case 176:
            h = cinfft16_176;
            break;
        case 352:
            h = cinfft16_352;
            break;
        case 576:
            h = cinfft16_576;
            break;
        case 640:
            h = cinfft16_640;
            break;
        case 384:
            h = cinfft16_384;
            break;
        default:
            h = NULL;
            ASSERT(!"ERROR : ApplyCIFFT_toCols invalid N");
    } /* switch (M) */

    // Apply IFFT to columns, transform size is M
    __Pragma("loop_count min=2 factor=2");
    for (j = 0; j < N / 2; j += 2)
    {
        ae_int16x4 x10, x32, y10, y32;

        px0 = (ae_int16x4 *)&x[j];
        py0 = (ae_int16x4 *)scrX0;
        py1 = (ae_int16x4 *)scrX1;
        /* Store two columns into temporary buffers */

        __Pragma("loop_count min=2 factor=2");
        for (i = 0; i < M; i += 2)
        {
            AE_L16X4_XP(x10, px0, N * sizeof(complex_fract16));
            AE_L16X4_XP(x32, px0, N * sizeof(complex_fract16));

            y32 = AE_SEL16_5410(x10, x32);
            y10 = AE_SEL16_7632(x10, x32);

            AE_S16X4_IP(y10, py0, 2 * sizeof(complex_fract16));
            AE_S16X4_IP(y32, py1, 2 * sizeof(complex_fract16));
        }
        /* Apply complex IFFT */
        Exp[j] = ifft_cplx16x16((int16_t*)scrY0, (int16_t*)scrX0, h, 2);
        Exp[(j + 1)] = ifft_cplx16x16((int16_t*)scrY1, (int16_t*)scrX1, h, 2);


        px0 = (ae_int16x4 *)scrY0;
        px1 = (ae_int16x4 *)scrY1;
        py0 = (ae_int16x4 *)&y[j];

        /* Store two columns into output buffer */
        __Pragma("loop_count min=2 factor=2");
        for (i = 0; i < M; i += 2)
        {
            AE_L16X4_XP(x10, px0, 2 * sizeof(complex_fract16));
            AE_L16X4_XP(x32, px1, 2 * sizeof(complex_fract16));

            y32 = AE_SEL16_5410(x10, x32);
            y10 = AE_SEL16_7632(x10, x32);
            AE_S16X4_XP(y10, py0, N * sizeof(complex_fract16));
            AE_S16X4_XP(y32, py0, N * sizeof(complex_fract16));
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

        Exp[j] = ifft_cplx16x16((int16_t*)scrY0, (int16_t*)scrX0, h, 2);

        __Pragma("loop_count min=2 factor=2");
        for (i = 0; i < M; i++)
        {
            y[j + i*N] = scrY0[i];
        }

        maxExp = XT_MAX(maxExp, Exp[j]);
    }

    /* Rescaling output */
    __Pragma("loop_count min=1"); 
    for (j = 0; j < (N / 2 + 1); j++)
    {
        ae_int32x2 tmp;
        int shift = maxExp - Exp[j];
        px = (ae_p16x2s*)(y + j - N);
        py = (ae_p16x2s*)(y + j - N);
        __Pragma("loop_count min=4 factor=4"); 
        for (i = 0; i < M; i++)
        {
            AE_L16X2M_XU(tmp, px, N*sizeof(complex_fract16)); 
            tmp = AE_SRAA32RS(tmp, shift);
            AE_S16X2M_XU(tmp, py, N*sizeof(complex_fract16));
        }
    }

    return maxExp;
} /* ApplyIFFT_toCols */

/*-----------------------------------------------------------
Apply RIFFT to rows, find common scale and rescale results
Input:
x[M*N]  spectrum
Output:
y[M*N]  real data
Temporary:
scr[N+M]
Returns common scale factor
-----------------------------------------------------------*/
int ApplyRIFFT_toRows(complex_fract16* scr, int16_t *y, complex_fract16 *x, int M, int N)
{
    int16_t *scrY = (int16_t*)scr;
    int *Exp = (int*)(scr + N);
    int i, j, maxExpRows = -100;
    ae_int16x4 * restrict px;
    ae_int16x4 * restrict py;

    fft_handle_t h;
    NASSERT_ALIGN(x, HIFI_SIMD_WIDTH);
    NASSERT_ALIGN(y, HIFI_SIMD_WIDTH);
    NASSERT_ALIGN(scrY, HIFI_SIMD_WIDTH);
    NASSERT_ALIGN(Exp, HIFI_SIMD_WIDTH);

    switch (N)
    {
    case 64:
        h = rifft16_64;
        break;
    case 128:
        h = rifft16_128;
        break;
    case 256:
        h = rifft16_256;
        break;
    case 512:
        h = rifft16_512;
        break;
    case 1024:
        h = rifft16_1024;
        break;
    case 2048:
        h = rifft16_2048;
        break;
    case 640:
        h = rinfft16_640;
        break;
    case 320:
        h = rinfft16_320;
        break;
    case 480:
        h = rinfft16_480;
        break;
    case 288:
        h = rinfft16_288;
        break;
    case 576:
        h = rinfft16_576;
        break;
    case 144:
        h = rinfft16_144;
        break;
    case 384:
        h = rinfft16_384;
        break;
    case 240:
        h = rinfft16_240;
        break;
    case 352:
        h = rinfft16_352;
        break;
    case 96:
        h = rinfft16_96;
        break;
    case 176:
        h = rinfft16_176;
        break;
    default:
        h = NULL;
        ASSERT(!"ERROR : ApplyRIFFT_toRows invalid N");
    } /* switch */

    py = (ae_int16x4*)y;
    // Apply RIFFT to rows, transform size is N
    __Pragma("loop_count min=1");
    for (i = 0; i < M; i++)
    {
        Exp[i] = ifft_real16x16((int16_t*)scrY, (int16_t*)&x[i*N], h, 2);
        maxExpRows = XT_MAX(maxExpRows, Exp[i]);

        px = (ae_int16x4*)scrY;
        __Pragma("loop_count min=2 factor=2");
        for (j = 0; j < (N >> 2); j++)
        {
            ae_int16x4 tmp;
            AE_L16X4_IP(tmp, px, sizeof(tmp));
            AE_S16X4_IP(tmp, py, sizeof(tmp));
        }
    }

    px = (ae_int16x4*)y;
    py = (ae_int16x4*)y;

    __Pragma("loop_count min=1");
    for (i = 0; i < M; i++)
    {
        int shift = maxExpRows - Exp[i]; 
        __Pragma("loop_count min=2 factor=2");
        for (j = 0; j < (N>>2); j++)
        {
            ae_int16x4 t;
            AE_L16X4_IP( t, px, sizeof(t) ); 
            t = AE_SRAA16RS(t, shift); 
            AE_S16X4_IP(t, py, sizeof(t));
        }
    }

    return maxExpRows;
} /* ApplyRIFFT_toRows */
