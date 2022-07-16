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
  Functions convert RGB planar images to YUV planar. 

  Y=c0*R+c1*G+c2*B
  U=c3*R+c4*G+c5*B + bias
  V=c6*R+c7*G+c8*B + bias
  where bias is 128 for 8-bit data and 16384 for 16-bit data

  Image formats:
  8-bit unsigned RGB/YUV
  16-bit signed RGB/YUV

  Input:
  inImgR,
  inImgG,
  inImgB  planes with R,G,B components
  c[13]   transform coefficients, Q29 format
  sz      image size
  Output:
  outImgY
  outImgU
  outImgV planes with Y,U,V components

  Restrictions:
  see general restrictions applied for all images for fast/generic 
  functions
-------------------------------------------------------------------------*/
void imgconvert_rgbyuv( void * restrict outImgY, 
                        void * restrict outImgU, 
                        void * restrict outImgV, 
                        const void * restrict inImgR, 
                        const void * restrict inImgG, 
                        const void * restrict inImgB, 
                        const int32_t * restrict c,
                        const imgsize_t* sz)
{
    ae_int16x4 r, g, b, y, u, v;
    ae_int32x2 dr0, dr1, dg0, dg1, db0, db1, tmp;
    const uint8_t * restrict inR = (const uint8_t *)inImgR;
    const uint8_t * restrict inG = (const uint8_t *)inImgG;
    const uint8_t * restrict inB = (const uint8_t *)inImgB;
    uint8_t * restrict outY = (uint8_t *)outImgY;
    uint8_t * restrict outU = (uint8_t *)outImgU;
    uint8_t * restrict outV = (uint8_t *)outImgV;
    int i, j, w = (int)sz->width, h = (int)sz->height, stride = sz->stride;
    ae_int16x4 c0, c1, c2, c3, c4, c5, c6, c7, c8;
    NASSERT(outImgY);
    NASSERT(outImgU);
    NASSERT(outImgV);
    NASSERT(inImgR);
    NASSERT(inImgG);
    NASSERT(inImgB);
    NASSERT_ALIGN(outImgY, 1);
    NASSERT_ALIGN(outImgU, 1);
    NASSERT_ALIGN(outImgV, 1);
    NASSERT_ALIGN(inImgR, 1);
    NASSERT_ALIGN(inImgG, 1);
    NASSERT_ALIGN(inImgB, 1);
    imgsize_validate(sz, 1, 0);
    tmp = AE_L32_X((const ae_int32 *)c, 0 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c0 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 1 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c1 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 2 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c2 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 3 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c3 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 4 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c4 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 5 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c5 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 6 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c6 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 7 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c7 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 8 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c8 = AE_ROUND16X4F32SASYM(tmp, tmp);
    ae_int16x4 cy, cu, cv;
    cy = c0;
    cu = c3;
    cv = c6;
    AE_MOVT16X4(cy, c1, AE_MOVAB4(4));
    AE_MOVT16X4(cy, c2, AE_MOVAB4(2));
    AE_MOVT16X4(cy, 0, AE_MOVAB4(1));
    AE_MOVT16X4(cu, c4, AE_MOVAB4(4));
    AE_MOVT16X4(cu, c5, AE_MOVAB4(2));
    AE_MOVT16X4(cu, 0, AE_MOVAB4(1));
    AE_MOVT16X4(cv, c7, AE_MOVAB4(4));
    AE_MOVT16X4(cv, c8, AE_MOVAB4(2));
    AE_MOVT16X4(cv, 0, AE_MOVAB4(1));

    for (i = 0; i < h; i++)
    {
        for (j = 0; j < (w >> 2); j++)
        {
            ae_int16x4 b1, b2, b3;
            ae_int16x4 g1, g2, g3;
            ae_int64 t0, t1, t2, t3;
            ae_int32x2 y01, u01, v01, y23, u23, v23;
            ae_int16x4 rgb0, rgb1, rgb2, rgb3;
            rgb0 = AE_MOVDA16(inR[i*stride + 4 * j + 0]);
            rgb1 = AE_MOVDA16(inR[i*stride + 4 * j + 1]);
            rgb2 = AE_MOVDA16(inR[i*stride + 4 * j + 2]);
            rgb3 = AE_MOVDA16(inR[i*stride + 4 * j + 3]);

            g = AE_MOVDA16(inG[i*stride + 4 * j + 0]);
            g1 = AE_MOVDA16(inG[i*stride + 4 * j + 1]);
            g2 = AE_MOVDA16(inG[i*stride + 4 * j + 2]);
            g3 = AE_MOVDA16(inG[i*stride + 4 * j + 3]);

            b = AE_MOVDA16(inB[i*stride + 4 * j + 0]);
            b1 = AE_MOVDA16(inB[i*stride + 4 * j + 1]);
            b2 = AE_MOVDA16(inB[i*stride + 4 * j + 2]);
            b3 = AE_MOVDA16(inB[i*stride + 4 * j + 3]);

            AE_MOVT16X4(rgb0, g, AE_MOVAB4(4));
            AE_MOVT16X4(rgb0, b, AE_MOVAB4(2));

            AE_MOVT16X4(rgb1, g1, AE_MOVAB4(4));
            AE_MOVT16X4(rgb1, b1, AE_MOVAB4(2));

            AE_MOVT16X4(rgb2, g2, AE_MOVAB4(4));
            AE_MOVT16X4(rgb2, b2, AE_MOVAB4(2));

            AE_MOVT16X4(rgb3, g3, AE_MOVAB4(4));
            AE_MOVT16X4(rgb3, b3, AE_MOVAB4(2));

            t0 = AE_MULZAAAAQ16(rgb0, cy);
            t1 = AE_MULZAAAAQ16(rgb1, cy);
            y01 = AE_TRUNCA32X2F64S(t0, t1, 24);
            t2 = AE_MULZAAAAQ16(rgb2, cy);
            t3 = AE_MULZAAAAQ16(rgb3, cy);
            y23 = AE_TRUNCA32X2F64S(t2, t3, 24);
            y = AE_SAT16X4(y01, y23);
            y = AE_SRAI16(y, 7);
            outY[i*stride + 4 * j + 0] = (uint8_t)AE_MOVAD16_3(y);
            outY[i*stride + 4 * j + 1] = (uint8_t)AE_MOVAD16_2(y);
            outY[i*stride + 4 * j + 2] = (uint8_t)AE_MOVAD16_1(y);
            outY[i*stride + 4 * j + 3] = (uint8_t)AE_MOVAD16_0(y);

            t0 = AE_MULZAAAAQ16(rgb0, cu);
            t1 = AE_MULZAAAAQ16(rgb1, cu);
            u01 = AE_TRUNCA32X2F64S(t0, t1, 24);
            t2 = AE_MULZAAAAQ16(rgb2, cu);
            t3 = AE_MULZAAAAQ16(rgb3, cu);
            u23 = AE_TRUNCA32X2F64S(t2, t3, 24);

            u = AE_SAT16X4(u01, u23);
            u = AE_ADD16S(u, (ae_int16x4)16384);
            AE_MOVT16X4(u, 0, AE_LT16(u, 0));
            u = AE_SRAI16(u, 7);

            outU[i*stride + 4 * j + 0] = (uint8_t)AE_MOVAD16_3(u);
            outU[i*stride + 4 * j + 1] = (uint8_t)AE_MOVAD16_2(u);
            outU[i*stride + 4 * j + 2] = (uint8_t)AE_MOVAD16_1(u);
            outU[i*stride + 4 * j + 3] = (uint8_t)AE_MOVAD16_0(u);

            t0 = AE_MULZAAAAQ16(rgb0, cv);
            t1 = AE_MULZAAAAQ16(rgb1, cv);
            v01 = AE_TRUNCA32X2F64S(t0, t1, 24);
            t2 = AE_MULZAAAAQ16(rgb2, cv);
            t3 = AE_MULZAAAAQ16(rgb3, cv);
            v23 = AE_TRUNCA32X2F64S(t2, t3, 24);

            v = AE_SAT16X4(v01, v23);            
            v = AE_ADD16S(v, (ae_int16x4)16384);
            AE_MOVT16X4(v, 0, AE_LT16(v, 0));                                   
            v = AE_SRAI16(v, 7);
                       
            outV[i*stride + 4 * j + 0] = (uint8_t)AE_MOVAD16_3(v);                        
            outV[i*stride + 4 * j + 1] = (uint8_t)AE_MOVAD16_2(v);                        
            outV[i*stride + 4 * j + 2] = (uint8_t)AE_MOVAD16_1(v);                        
            outV[i*stride + 4 * j + 3] = (uint8_t)AE_MOVAD16_0(v);
        }
    }

    tmp = AE_L32_X((const ae_int32 *)c, 0 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c0 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 1 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c1 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 2 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c2 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 3 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c3 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 4 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c4 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 5 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c5 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 6 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c6 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 7 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c7 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 8 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c8 = AE_ROUND16X4F32SASYM(tmp, tmp);
    for (i = 0; i < h; i++)
    {
        for (j = w&(~3); j<w; j++)
        {
            r = AE_MOVDA16(inR[i*stride + j]);
            g = AE_MOVDA16(inG[i*stride + j]);
            b = AE_MOVDA16(inB[i*stride + j]);
            r = AE_SLAI16S(r, 7);
            g = AE_SLAI16S(g, 7);
            b = AE_SLAI16S(b, 7);

            AE_MUL16X4(dr0, dr1, r, c0);
            AE_MUL16X4(dg0, dg1, g, c1);
            AE_MUL16X4(db0, db1, b, c2);
            dr0 = AE_ADD32(dr0, dg0);
            db0 = AE_ADD32(db0, 1 << 21);
            dr0 = AE_ADD32(dr0, db0);
            dr1 = AE_ADD32(dr1, dg1);
            db1 = AE_ADD32(db1, 1 << 21);
            dr1 = AE_ADD32(dr1, db1);
            y = AE_SAT16X4(AE_SRAI32(dr0, 15), AE_SRAI32(dr1, 15));

            AE_MUL16X4(dr0, dr1, r, c3);
            AE_MUL16X4(dg0, dg1, g, c4);
            AE_MUL16X4(db0, db1, b, c5);
            dr0 = AE_ADD32(dr0, dg0);
            db0 = AE_ADD32(db0, 1 << 21);
            dr0 = AE_ADD32(dr0, db0);
            dr1 = AE_ADD32(dr1, dg1);
            db1 = AE_ADD32(db1, 1 << 21);
            dr1 = AE_ADD32(dr1, db1);
            u = AE_SAT16X4(AE_SRAI32(dr0, 15), AE_SRAI32(dr1, 15));

            AE_MUL16X4(dr0, dr1, r, c6);
            AE_MUL16X4(dg0, dg1, g, c7);
            AE_MUL16X4(db0, db1, b, c8);
            dr0 = AE_ADD32(dr0, dg0);
            db0 = AE_ADD32(db0, 1 << 21);
            dr0 = AE_ADD32(dr0, db0);
            dr1 = AE_ADD32(dr1, dg1);
            db1 = AE_ADD32(db1, 1 << 21);
            dr1 = AE_ADD32(dr1, db1);
            v = AE_SAT16X4(AE_SRAI32(dr0, 15), AE_SRAI32(dr1, 15));

            u = AE_ADD16S(u, (ae_int16x4)16384);
            v = AE_ADD16S(v, (ae_int16x4)16384);
            AE_MOVT16X4(v, 0, AE_LT16(v, 0));
            AE_MOVT16X4(u, 0, AE_LT16(u, 0));
            y = AE_SRAI16(y, 7);
            u = AE_SRAI16(u, 7);
            v = AE_SRAI16(v, 7);
            outY[i*stride + j] = (uint8_t)AE_MOVAD16_0(y);
            outU[i*stride + j] = (uint8_t)AE_MOVAD16_0(u);
            outV[i*stride + j] = (uint8_t)AE_MOVAD16_0(v);
        }
    }
}
