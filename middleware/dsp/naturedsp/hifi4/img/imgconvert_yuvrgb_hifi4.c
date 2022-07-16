/* ------------------------------------------------------------------------ */
/* Copyright (c) 2019 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
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
/*          Copyright (C) 2015-2019 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
#include "NatureDSP_types.h"
#include "NatureDSP_Signal_img.h"
#include "common.h"
#include "img_common.h"

/*-------------------------------------------------------------------------
  RGB<->YUV conversion 
  Functions convert YUV (BT.601) planar images to RGB planar 

  R=Y+c9*(V-bias)
  G=Y+c10*(U-bias)+c11*(V-bias)
  B=Y+c12*(U-bias)
  where bias is 128 for 8-bit data and 16384 for 16-bit data

  Image formats:
  8-bit unsigned RGB/YUV
  16-bit signed RGB/YUV

  Input:
  inImgY,
  inImgU,
  inImgV  planes with Y,U,V components
  c[13]   transform coefficients, Q29 format
  sz      image size
  Output:
  outImgR
  outImgG
  outImgB planes with R,G,B components

  Restrictions:
  see general restrictions applied for all images for fast/generic 
  functions
-------------------------------------------------------------------------*/
void imgconvert_yuvrgb( void * restrict outImgR, 
                        void * restrict outImgG, 
                        void * restrict outImgB, 
                        const void * restrict inImgY, 
                        const void * restrict inImgU, 
                        const void * restrict inImgV, 
                        const int32_t * restrict c,
                        const imgsize_t* sz)
{
    int i, j, w = (int)sz->width, h = (int)sz->height, stride = sz->stride;
    ae_int16x4 y, u, v, r, g, b;
    ae_int32x2 dy0, dy1, dv0, dv1, du0, du1, tmp;
    const uint8_t * restrict inY = (const uint8_t *)inImgY;
    const uint8_t * restrict inU = (const uint8_t *)inImgU;
    const uint8_t * restrict inV = (const uint8_t *)inImgV;
    uint8_t * restrict outR = (uint8_t *)outImgR;
    uint8_t * restrict outG = (uint8_t *)outImgG;
    uint8_t * restrict outB = (uint8_t *)outImgB;
    ae_int16x4 c9, c10, c11, c12;
    NASSERT(outImgR);
    NASSERT(outImgG);
    NASSERT(outImgB);
    NASSERT(inImgY);
    NASSERT(inImgU);
    NASSERT(inImgV);
    NASSERT_ALIGN(outImgR, 1);
    NASSERT_ALIGN(outImgG, 1);
    NASSERT_ALIGN(outImgB, 1);
    NASSERT_ALIGN(inImgY, 1);
    NASSERT_ALIGN(inImgU, 1);
    NASSERT_ALIGN(inImgV, 1);
    imgsize_validate(sz, 1, 0);
    for (i = 0; i < h; i++)
    {
        tmp = AE_L32_X((const ae_int32 *)c, 9 * sizeof(int32_t));
        c9 = AE_ROUND16X4F32SASYM(tmp, tmp);
        tmp = AE_L32_X((const ae_int32 *)c, 10 * sizeof(int32_t));
        c10 = AE_ROUND16X4F32SASYM(tmp, tmp);
        tmp = AE_L32_X((const ae_int32 *)c, 11 * sizeof(int32_t));
        c11 = AE_ROUND16X4F32SASYM(tmp, tmp);
        tmp = AE_L32_X((const ae_int32 *)c, 12 * sizeof(int32_t));
        c12 = AE_ROUND16X4F32SASYM(tmp, tmp);
        ae_int16x4 cr, cg, cb;
        cr = 8192;
        cg = 8192;
        cb = 8192;
        AE_MOVT16X4(cr, c9, AE_MOVAB4(2));
        AE_MOVT16X4(cr, 0, AE_MOVAB4(4));
        AE_MOVT16X4(cr, 0, AE_MOVAB4(1));
        AE_MOVT16X4(cg, c11, AE_MOVAB4(2));
        AE_MOVT16X4(cg, c10, AE_MOVAB4(4));
        AE_MOVT16X4(cg, 0, AE_MOVAB4(1));
        AE_MOVT16X4(cb, 0, AE_MOVAB4(2));
        AE_MOVT16X4(cb, c12, AE_MOVAB4(4));
        AE_MOVT16X4(cb, 0, AE_MOVAB4(1));
        ae_int16x4 s;
        s = 0;
        //AE_MOVT16X4(s, 16384, AE_MOVAB4(6));
        AE_MOVT16X4(s, 128, AE_MOVAB4(6));

        for (j = 0; j < (w >> 2); j++)
        {
            ae_int16x4 u1, u2, u3;
            ae_int16x4 v1, v2, v3;
            ae_int64 t0, t1, t2, t3;
            ae_int32x2 r01, g01, b01, r23, g23, b23;
            ae_int16x4 yuv0, yuv1, yuv2, yuv3;
            
            yuv0 = AE_MOVDA16(inY[i*stride + 4 * j + 0]);
            yuv1 = AE_MOVDA16(inY[i*stride + 4 * j + 1]);
            yuv2 = AE_MOVDA16(inY[i*stride + 4 * j + 2]);
            yuv3 = AE_MOVDA16(inY[i*stride + 4 * j + 3]);

            u = AE_MOVDA16(inU[i*stride + 4 * j + 0]);
            u1 = AE_MOVDA16(inU[i*stride + 4 * j + 1]);
            u2 = AE_MOVDA16(inU[i*stride + 4 * j + 2]);
            u3 = AE_MOVDA16(inU[i*stride + 4 * j + 3]);

            v = AE_MOVDA16(inV[i*stride + 4 * j + 0]);
            v1 = AE_MOVDA16(inV[i*stride + 4 * j + 1]);
            v2 = AE_MOVDA16(inV[i*stride + 4 * j + 2]);
            v3 = AE_MOVDA16(inV[i*stride + 4 * j + 3]);

            AE_MOVT16X4(yuv0, u, AE_MOVAB4(4));
            AE_MOVT16X4(yuv0, v, AE_MOVAB4(2));

            AE_MOVT16X4(yuv1, u1, AE_MOVAB4(4));
            AE_MOVT16X4(yuv1, v1, AE_MOVAB4(2));

            AE_MOVT16X4(yuv2, u2, AE_MOVAB4(4));
            AE_MOVT16X4(yuv2, v2, AE_MOVAB4(2));

            AE_MOVT16X4(yuv3, u3, AE_MOVAB4(4));
            AE_MOVT16X4(yuv3, v3, AE_MOVAB4(2));
           
            yuv0 = AE_SUB16(yuv0, s);
            yuv1 = AE_SUB16(yuv1, s);
            yuv2 = AE_SUB16(yuv2, s);
            yuv3 = AE_SUB16(yuv3, s);
            
            t0 = AE_MULZAAAAQ16(yuv0, cr);
            t1 = AE_MULZAAAAQ16(yuv1, cr);
            r01 = AE_TRUNCA32X2F64S(t0, t1, 26);
            t2 = AE_MULZAAAAQ16(yuv2, cr);
            t3 = AE_MULZAAAAQ16(yuv3, cr);
            r23 = AE_TRUNCA32X2F64S(t2, t3, 26);
            r = AE_SAT16X4(r01, r23);

            t0 = AE_MULZAAAAQ16(yuv0, cg);
            t1 = AE_MULZAAAAQ16(yuv1, cg);
            g01 = AE_TRUNCA32X2F64S(t0, t1, 26);
            t2 = AE_MULZAAAAQ16(yuv2, cg);
            t3 = AE_MULZAAAAQ16(yuv3, cg);
            g23 = AE_TRUNCA32X2F64S(t2, t3, 26);
            g = AE_SAT16X4(g01, g23);

            t0 = AE_MULZAAAAQ16(yuv0, cb);
            t1 = AE_MULZAAAAQ16(yuv1, cb);
            b01 = AE_TRUNCA32X2F64S(t0, t1, 26);
            t2 = AE_MULZAAAAQ16(yuv2, cb);
            t3 = AE_MULZAAAAQ16(yuv3, cb);
            b23 = AE_TRUNCA32X2F64S(t2, t3, 26);
            b = AE_SAT16X4(b01, b23);

            AE_MOVT16X4(r, 0, AE_LT16(r, 0));
            AE_MOVT16X4(g, 0, AE_LT16(g, 0));
            AE_MOVT16X4(b, 0, AE_LT16(b, 0));

            r = AE_SRAI16(r, 7);
            g = AE_SRAI16(g, 7);
            b = AE_SRAI16(b, 7);
            outR[i*stride + 4 * j + 0] = (uint8_t)AE_MOVAD16_3(r);
            outG[i*stride + 4 * j + 0] = (uint8_t)AE_MOVAD16_3(g);
            outB[i*stride + 4 * j + 0] = (uint8_t)AE_MOVAD16_3(b);
            outR[i*stride + 4 * j + 1] = (uint8_t)AE_MOVAD16_2(r);
            outG[i*stride + 4 * j + 1] = (uint8_t)AE_MOVAD16_2(g);
            outB[i*stride + 4 * j + 1] = (uint8_t)AE_MOVAD16_2(b);
            outR[i*stride + 4 * j + 2] = (uint8_t)AE_MOVAD16_1(r);
            outG[i*stride + 4 * j + 2] = (uint8_t)AE_MOVAD16_1(g);
            outB[i*stride + 4 * j + 2] = (uint8_t)AE_MOVAD16_1(b);
            outR[i*stride + 4 * j + 3] = (uint8_t)AE_MOVAD16_0(r);
            outG[i*stride + 4 * j + 3] = (uint8_t)AE_MOVAD16_0(g);
            outB[i*stride + 4 * j + 3] = (uint8_t)AE_MOVAD16_0(b);
        }
        tmp = AE_L32_X((const ae_int32 *)c, 9 * sizeof(int32_t));
        c9 = AE_ROUND16X4F32SASYM(tmp, tmp);
        tmp = AE_L32_X((const ae_int32 *)c, 10 * sizeof(int32_t));
        c10 = AE_ROUND16X4F32SASYM(tmp, tmp);
        tmp = AE_L32_X((const ae_int32 *)c, 11 * sizeof(int32_t));
        c11 = AE_ROUND16X4F32SASYM(tmp, tmp);
        tmp = AE_L32_X((const ae_int32 *)c, 12 * sizeof(int32_t));
        c12 = AE_ROUND16X4F32SASYM(tmp, tmp);
        for (j = w&(~3); j < w; j++)
        {
            y = AE_MOVDA16(inY[i*stride + j]);
            u = AE_MOVDA16(inU[i*stride + j]);
            v = AE_MOVDA16(inV[i*stride + j]);
            y = AE_SLAI16S(y, 7);
            u = AE_SLAI16S(u, 7);
            v = AE_SLAI16S(v, 7);
            v = AE_SUB16(v, 16384);
            u = AE_SUB16(u, 16384);
            AE_MUL16X4(dy0, dy1, 8192, y);
            AE_MUL16X4(dv0, dv1, c9, v);
            dv0 = AE_ADD32(dv0, dy0);
            dv0 = AE_ADD32(dv0, (1 << 19));
            dv1 = AE_ADD32(dv1, dy1);
            dv1 = AE_ADD32(dv1, (1 << 19));
            r = AE_SAT16X4(AE_SRAI32(dv0, 13), AE_SRAI32(dv1, 13));
            AE_MUL16X4(du0, du1, c12, u);
            du0 = AE_ADD32(du0, dy0);
            du0 = AE_ADD32(du0, (1 << 19));
            du1 = AE_ADD32(du1, dy1);
            du1 = AE_ADD32(du1, (1 << 19));
            b = AE_SAT16X4(AE_SRAI32(du0, 13), AE_SRAI32(du1, 13));
            AE_MUL16X4(du0, du1, c10, u);
            AE_MUL16X4(dv0, dv1, c11, v);
            du0 = AE_ADD32(du0, dy0);
            dv0 = AE_ADD32(dv0, (1 << 19));
            dv0 = AE_ADD32(dv0, du0);
            du1 = AE_ADD32(du1, dy1);
            dv1 = AE_ADD32(dv1, (1 << 19));
            dv1 = AE_ADD32(dv1, du1);
            g = AE_SAT16X4(AE_SRAI32(dv0, 13), AE_SRAI32(dv1, 13));

            AE_MOVT16X4(r, 0, AE_LT16(r, 0));
            AE_MOVT16X4(g, 0, AE_LT16(g, 0));
            AE_MOVT16X4(b, 0, AE_LT16(b, 0));
            r = AE_SRAI16(r, 7);
            g = AE_SRAI16(g, 7);
            b = AE_SRAI16(b, 7);
            outR[i*stride + j] = (uint8_t)AE_MOVAD16_0(r);
            outG[i*stride + j] = (uint8_t)AE_MOVAD16_0(g);
            outB[i*stride + j] = (uint8_t)AE_MOVAD16_0(b);
        }
    }
}
