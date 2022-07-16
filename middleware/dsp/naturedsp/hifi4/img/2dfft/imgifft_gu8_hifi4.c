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
#include "NatureDSP_types.h"
#include "NatureDSP_Signal_img.h"
#include "common.h"
#include "img_common.h"
#include "imgfft_common.h"
#include "fft_x16_common.h"

/*-------------------------------------------------------------------------
  Inverse image 2D-FFT makes special kind of Fourier transform over image data. 
  It supports a number of resolutions. For processing of arbitrary sizes, user 
  might pad original data to the closest available dimension.

  Important notes:
  1.  IFFT ignores zero-th tap of transform. Instead, it uses dedicated 
      parameter to set mean intensity of the output image
  2.  IFFT accepts input in form of 16-bit complex data and form output in 
      desired format 
  3.  IFFT makes in-place transform so input spectrum will be damaged after 
      the call
  4.  Supported dimensions: 64, 96, 128, 144, 176, 240, 256, 288, 320, 352, 
      384, 480, 512, 576, 640

  Image formats:
  gu8    8-bit unsigned grayscale data
  gs8    8-bit signed grayscale data
  gs16   16-bit grayscale data

  Input:
  y[2*w*h]    input spectrum. w and h - width and height of output image
  mean        desired mean value (average intensity) over the output image, Q15.16
  fftscale    scale factor returned by FFT
  sz[1]       output image size
  Output:
  img         output image

  Restrictions:
  img,y        should be aligned on a 8-bytes boundary
  image        dimensions should be selected from the list of supported 
               dimensions
  image stride should be a multiple of 8 for 8-bit images and 4 for 16-bit 
               images
-------------------------------------------------------------------------*/
void imgifft_gu8 (void *pScr, 
               void* img, complex_fract16* y,
               int32_t mean, const imgsize_t* sz,
               int fftscale)
{
    complex_fract16 *scr = (complex_fract16*)pScr; 
    int M = sz->height;
    int N = sz->width;
    int16_t f;
    int log2MN;
    int stride = sz->stride;
    // Q15.16 -> Q15.0
    int16_t dcBias = (int16_t)AE_MOVAD32_L(AE_SRAA32RS(mean, 16));
    int i, expRows, expCols, j;
    int16_t *tmpXre = (int16_t*)y;
    uint8_t* z = (uint8_t*)img;
    ae_int16x4 * restrict px = (ae_int16x4*)tmpXre;
    ae_p16x2s  * restrict py = (ae_p16x2s  *)(z - 4);

    NASSERT(pScr);
    NASSERT(y);
    NASSERT(img);
    NASSERT_ALIGN(pScr,HIFI_SIMD_WIDTH);
    NASSERT_ALIGN(y   ,HIFI_SIMD_WIDTH);
    NASSERT_ALIGN(img ,HIFI_SIMD_WIDTH);
    imgsize_validate(sz,1,1);
    NASSERT(sz->width== 64 || sz->width== 96 || sz->width==128 || sz->width==144 || 
            sz->width==176 || sz->width==240 || sz->width==256 || sz->width==288 || 
            sz->width==320 || sz->width==352 || sz->width==384 || sz->width==480 || 
            sz->width==512 || sz->width==576 || sz->width==640);
    NASSERT(sz->height== 64 || sz->height== 96 || sz->height==128 || sz->height==144 || 
            sz->height==176 || sz->height==240 || sz->height==256 || sz->height==288 || 
            sz->height==320 || sz->height==352 || sz->height==384 || sz->height==480 || 
            sz->height==512 || sz->height==576 || sz->height==640);
    /*
        1. copy column by column, remove 0th taps and make column IFFTs
        2. find common scale for all ffts
        3. rescale results 
        4. copy row by row and make row IFFTs
        5. find common scale for all ffts
        6. rescale results, add desired mean value, saturate and transform to 
           desired format
    */
    {
        int32_t q; 
       // int32_t e = XT_NSA(M*N);
        int32_t e = NSA(M*N);

        log2MN = 30 - e;
        q = (1L << 30) / ((M*N << e) >> 15); 
        f = (int16_t)(AE_MOVAD32_L( AE_SLAA32S(q, 16)) >> 16); 
    }

    y[0].s.im = y[0].s.re = 0;

    expCols = ApplyIFFT_toCols(scr, y, y, M, N);
    expRows = ApplyRIFFT_toRows(scr, tmpXre, y, M, N);

    int lshift = expCols + expRows + fftscale - log2MN; 
    ae_int16x4 tmp, y0;


    ASSERT(N % 8 == 0); 

    __Pragma("loop_count min=1");
    for (j = 0; j < M; j++)
    {
#if 0
        // Reference code
        for (i = 0; i < N; i++)
        {
            int16_t tmp = S_add_ss(S_shl_s( S_mpy_ss(tmpXre[k], f), lshift), dcBias );
            // Sature negative values to zero
            tmp = S_add_ss(tmp, MIN_INT16) ^ MIN_INT16;

            z[j*stride + i] = (uint8_t)(tmp >> 7);
            k++; 
        }
#else
        __Pragma("loop_count min=2 factor=2"); 
        for (i = 0; i < (N >> 2); i++)
        {
            AE_L16X4_IP(tmp, px, sizeof(tmp));
            tmp = AE_SLAA16S( AE_MULFP16X4RAS(tmp, f), lshift);
            tmp = AE_ADD16S(tmp, dcBias); 
            /* Sature negative values to zero */
            tmp = AE_XOR16(MIN_INT16, AE_ADD16S(tmp, MIN_INT16)); 
            tmp = AE_SRAA16RS(tmp, 7); 
            /* Pack and store 4 bytes */
            y0  = AE_MOVINT16X4_FROMINT32X2(AE_SLAI32(AE_MOVINT32X2_FROMINT16X4(tmp), 16));
            tmp = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(tmp),  8));

            y0 = AE_OR16(y0, tmp);
            AE_S16X2M_IU(AE_MOVINT32X2_FROMINT16X4(y0), py, sizeof(int32_t));
        }
        py = (ae_p16x2s  *)( stride - N + (uintptr_t)py ); 
#endif
    }
}

/* Request the scratch size:*/ 
size_t imgifft_gu8_getScratchSize   (const imgsize_t* sz)
{
    int M = sz->height;
    int N = sz->width;
    imgsize_validate(sz,1,1);
    return sizeof(complex_fract16)* XT_MAX(N + M, 4 * M + N / 2 + 1);
}
