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
void imgfastconvert_yuvrgb( 
                        void * restrict outImgR, 
                        void * restrict outImgG, 
                        void * restrict outImgB, 
                        const void * restrict inImgY, 
                        const void * restrict inImgU, 
                        const void * restrict inImgV, 
                        const int32_t * restrict c,
                        const imgsize_t* sz)
{
    ae_int16x4 c9, c10, c11, c12;
    int i, j, w = (int)sz->width, h = (int)sz->height, stride = sz->stride;
    ae_int16x4 y, u, v, r0, g0, b0, r1, g1, b1, t0, t1;
    ae_int32x2 dy0, dy1, dv0, dv1, du0, du1, tmp;
    const signed char * restrict pInY;
    const uint8_t * restrict inY = (const uint8_t *)inImgY;
    const signed char * restrict pInU;
    const uint8_t * restrict inU = (const uint8_t *)inImgU;
    const signed char * restrict pInV;
    const uint8_t * restrict inV = (const uint8_t *)inImgV;
    ae_int16x4 * restrict pOutR;
    uint8_t * restrict outR = (uint8_t *)outImgR;
    ae_int16x4 * restrict pOutG;
    uint8_t * restrict outG = (uint8_t *)outImgG;
    ae_int16x4 * restrict pOutB;
    uint8_t * restrict outB = (uint8_t *)outImgB;
    NASSERT(outImgR);
    NASSERT(outImgG);
    NASSERT(outImgB);
    NASSERT(inImgY);
    NASSERT(inImgU);
    NASSERT(inImgV);
    NASSERT_ALIGN(outImgR, ALIGNMENT);
    NASSERT_ALIGN(outImgG, ALIGNMENT);
    NASSERT_ALIGN(outImgB, ALIGNMENT);
    NASSERT_ALIGN(inImgY, ALIGNMENT);
    NASSERT_ALIGN(inImgU, ALIGNMENT);
    NASSERT_ALIGN(inImgV, ALIGNMENT);
    imgsize_validate(sz, 1, 1);
    tmp = AE_L32_X((const ae_int32 *)c, 9 * sizeof(int32_t));
    c9 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 10 * sizeof(int32_t));
    c10 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 11 * sizeof(int32_t));
    c11 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 12 * sizeof(int32_t));
    c12 = AE_ROUND16X4F32SASYM(tmp, tmp);
    for (i = 0; i < h; i++)
    {
        pInY = (const signed char*)(inY + i*stride);
        pInU = (const signed char*)(inU + i*stride);
        pInV = (const signed char*)(inV + i*stride);
        pOutR = (ae_int16x4 *)(outR + i*stride);
        pOutG = (ae_int16x4 *)(outG + i*stride);
        pOutB = (ae_int16x4 *)(outB + i*stride);
        for (j = 0; j < (w>>3); j++)
        {
           
            AE_L8X4F_IP(y, pInY, 4 * sizeof(uint8_t));
            AE_L8X4F_IP(u, pInU, 4 * sizeof(uint8_t));
            AE_L8X4F_IP(v, pInV, 4 * sizeof(uint8_t));
            y = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(y), 1));
            u = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(u), 1));
            v = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(v), 1));
            v = AE_SUB16(v, 16384);
            u = AE_SUB16(u, 16384);

            ae_int32x2 dr0, dr1, dg0, dg1, db0, db1;
            AE_MUL16X4(dr0, dr1, 8192, y);
            AE_MULA16X4(dr0, dr1, c9, v);
            dr0 = AE_SLLI32S(dr0, 3);
            dr1 = AE_SLLI32S(dr1, 3);
            r0 = AE_ROUND16X4F32SASYM(dr0, dr1);

            AE_MUL16X4(db0, db1, 8192, y);
            AE_MULA16X4(db0, db1, c12, u);
            db0 = AE_SLLI32S(db0, 3);
            db1 = AE_SLLI32S(db1, 3);
            b0 = AE_ROUND16X4F32SASYM(db0, db1);

            AE_MUL16X4(dg0, dg1, 8192, y);
            AE_MULA16X4(dg0, dg1, c10, u);
            AE_MULA16X4(dg0, dg1, c11, v);
            dg0 = AE_SLLI32S(dg0, 3);
            dg1 = AE_SLLI32S(dg1, 3);
            g0 = AE_ROUND16X4F32SASYM(dg0, dg1);


            AE_MOVT16X4(r0, 0, AE_LT16(r0, 0));
            AE_MOVT16X4(g0, 0, AE_LT16(g0, 0));
            AE_MOVT16X4(b0, 0, AE_LT16(b0, 0));
            r0 = AE_SRAI16(r0, 7);
            g0 = AE_SRAI16(g0, 7);
            b0 = AE_SRAI16(b0, 7);

            /*Second half*/
            AE_L8X4F_IP(y, pInY, 4 * sizeof(uint8_t));
            AE_L8X4F_IP(u, pInU, 4 * sizeof(uint8_t));
            AE_L8X4F_IP(v, pInV, 4 * sizeof(uint8_t));
            y = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(y), 1));
            u = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(u), 1));
            v = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(v), 1));
            v = AE_SUB16(v, 16384);
            u = AE_SUB16(u, 16384);

            AE_MUL16X4(dr0, dr1, 8192, y);
            AE_MULA16X4(dr0, dr1, c9, v);
            dr0 = AE_SLLI32S(dr0, 3);
            dr1 = AE_SLLI32S(dr1, 3);
            r1 = AE_ROUND16X4F32SASYM(dr0, dr1);

            AE_MUL16X4(db0, db1, 8192, y);
            AE_MULA16X4(db0, db1, c12, u);
            db0 = AE_SLLI32S(db0, 3);
            db1 = AE_SLLI32S(db1, 3);
            b1 = AE_ROUND16X4F32SASYM(db0, db1);

            AE_MUL16X4(dg0, dg1, 8192, y);
            AE_MULA16X4(dg0, dg1, c10, u);
            AE_MULA16X4(dg0, dg1, c11, v);
            dg0 = AE_SLLI32S(dg0, 3);
            dg1 = AE_SLLI32S(dg1, 3);
            g1 = AE_ROUND16X4F32SASYM(dg0, dg1);

            AE_MOVT16X4(r1, 0, AE_LT16(r1, 0));
            AE_MOVT16X4(g1, 0, AE_LT16(g1, 0));
            AE_MOVT16X4(b1, 0, AE_LT16(b1, 0));
            r1 = AE_SRAI16(r1, 7);
            g1 = AE_SRAI16(g1, 7);
            b1 = AE_SRAI16(b1, 7);

            t0 = AE_SEL16_7531(r0, r1);
            t1 = AE_SEL16_6420(r0, r1);
            t1 = AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(t1), 8));
            t0 = AE_OR16(t0, t1);
            AE_S16X4_IP(t0, pOutR, sizeof(ae_int16x4));

            t0 = AE_SEL16_7531(g0, g1);
            t1 = AE_SEL16_6420(g0, g1);
            t1 = AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(t1), 8));
            t0 = AE_OR16(t0, t1);
            AE_S16X4_IP(t0, pOutG, sizeof(ae_int16x4));

            t0 = AE_SEL16_7531(b0, b1);
            t1 = AE_SEL16_6420(b0, b1);
            t1 = AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(t1), 8));
            t0 = AE_OR16(t0, t1);
            AE_S16X4_IP(t0, pOutB, sizeof(ae_int16x4));

            
        }
    }
    tmp = AE_L32_X((const ae_int32 *)c, 9 * sizeof(int32_t));
    c9 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 10 * sizeof(int32_t));
    c10 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 11 * sizeof(int32_t));
    c11 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 12 * sizeof(int32_t));
    c12 = AE_ROUND16X4F32SASYM(tmp, tmp);

    for (i = 0; i < h; i++)
    {
        for (j = w&(~7); j < w; j++)
        {
            ae_int16x4 r, g, b;
            y = AE_MOVDA16(((const uint8_t*)inImgY)[i*stride + j]);
            u = AE_MOVDA16(((const uint8_t*)inImgU)[i*stride + j]);
            v = AE_MOVDA16(((const uint8_t*)inImgV)[i*stride + j]);
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
            ((uint8_t*)outImgR)[i*stride + j] = (uint8_t)AE_MOVAD16_0(r);
            ((uint8_t*)outImgG)[i*stride + j] = (uint8_t)AE_MOVAD16_0(g);
            ((uint8_t*)outImgB)[i*stride + j] = (uint8_t)AE_MOVAD16_0(b);
        }
    }
}
