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
void imgfastconvert_rgbyuv( void * restrict outImgY, 
                        void * restrict outImgU, 
                        void * restrict outImgV, 
                        const void * restrict inImgR, 
                        const void * restrict inImgG, 
                        const void * restrict inImgB, 
                        const int32_t * restrict c,
                        const imgsize_t* sz)
{
    const signed char * restrict pInR;
    const uint8_t * restrict inR = (const uint8_t *)inImgR;
    const signed char * restrict pInG;
    const uint8_t * restrict inG = (const uint8_t *)inImgG;
    const signed char * restrict pInB;
    const uint8_t * restrict inB = (const uint8_t *)inImgB;
    ae_int16x4 * restrict pOutY;
    uint8_t * restrict outY = (uint8_t *)outImgY;
    ae_int16x4 * restrict pOutU;
    uint8_t * restrict outU = (uint8_t *)outImgU;
    ae_int16x4 * restrict pOutV;
    uint8_t * restrict outV = (uint8_t *)outImgV;
    ae_int16x4 r, g, b, y0, u0, v0, y1, u1, v1, t0, t1;
    ae_int32x2 dr0, dr1, dg0, dg1, db0, db1, tmp;
    int i, j, w = (int)sz->width, h = (int)sz->height, stride = sz->stride;
    ae_int16x4 c0, c1, c2, c3, c4, c5, c6, c7, c8;        
    
    NASSERT(outImgY);
    NASSERT(outImgU);
    NASSERT(outImgV);
    NASSERT(inImgR);
    NASSERT(inImgG);
    NASSERT(inImgB);
    NASSERT_ALIGN(outImgY, ALIGNMENT);
    NASSERT_ALIGN(outImgU, ALIGNMENT);
    NASSERT_ALIGN(outImgV, ALIGNMENT);
    NASSERT_ALIGN(inImgR, ALIGNMENT);
    NASSERT_ALIGN(inImgG, ALIGNMENT);
    NASSERT_ALIGN(inImgB, ALIGNMENT);
    imgsize_validate(sz, 1, 1);
    /*Y*/
    tmp = AE_L32_X((const ae_int32 *)c, 0 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c0 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 1 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c1 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 2 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c2 = AE_ROUND16X4F32SASYM(tmp, tmp);    
    for (i = 0; i < h; i++)
    {
        pInR = (const signed char*)(inR + i*stride);
        pInG = (const signed char*)(inG + i*stride);
        pInB = (const signed char*)(inB + i*stride);
        pOutY = (ae_int16x4 *)(outY + i*stride);
        for (j = 0; j < (w >> 3); j++)
        {
            AE_L8X4F_IP(r, pInR, 4 * sizeof(uint8_t));
            AE_L8X4F_IP(g, pInG, 4 * sizeof(uint8_t));
            AE_L8X4F_IP(b, pInB, 4 * sizeof(uint8_t));
            r = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(r), 1));
            g = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(g), 1));
            b = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(b), 1));

            AE_MUL16X4(dr0, dr1, r, c0);
            AE_MULA16X4(dr0, dr1, g, c1);
            AE_MULA16X4(dr0, dr1, b, c2);
            dr0 = AE_ADD32(dr0, 1 << 21);
            dr1 = AE_ADD32(dr1, 1 << 21);
            y0 = AE_SAT16X4(AE_SRAI32(dr0, 15), AE_SRAI32(dr1, 15));
            y0 = AE_SRAI16(y0, 7);

            /*second 4*/

            AE_L8X4F_IP(r, pInR, 4 * sizeof(uint8_t));
            AE_L8X4F_IP(g, pInG, 4 * sizeof(uint8_t));
            AE_L8X4F_IP(b, pInB, 4 * sizeof(uint8_t));
            r = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(r), 1));
            g = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(g), 1));
            b = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(b), 1));

            AE_MUL16X4(dr0, dr1, r, c0);
            AE_MULA16X4(dr0, dr1, g, c1);
            AE_MULA16X4(dr0, dr1, b, c2);
            dr0 = AE_ADD32(dr0, 1 << 21);
            dr1 = AE_ADD32(dr1, 1 << 21);
            y1 = AE_SAT16X4(AE_SRAI32(dr0, 15), AE_SRAI32(dr1, 15));

            y1 = AE_SRAI16(y1, 7);
            /* save results */
            t0 = AE_SEL16_7531(y0, y1);
            t1 = AE_SEL16_6420(y0, y1);
            t1 = AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(t1), 8));
            t0 = AE_OR16(t0, t1);
            AE_S16X4_IP(t0, pOutY, sizeof(ae_int16x4));
        }
    }
    /*U*/    
    tmp = AE_L32_X((const ae_int32 *)c, 3 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c3 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 4 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c4 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 5 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c5 = AE_ROUND16X4F32SASYM(tmp, tmp);    
    for (i = 0; i < h; i++)
    {
        pInR = (const signed char*)(inR + i*stride);
        pInG = (const signed char*)(inG + i*stride);
        pInB = (const signed char*)(inB + i*stride);
        pOutU = (ae_int16x4 *)(outU + i*stride);
        for (j = 0; j < (w >> 3); j++)
        {
            AE_L8X4F_IP(r, pInR, 4 * sizeof(uint8_t));
            AE_L8X4F_IP(g, pInG, 4 * sizeof(uint8_t));
            AE_L8X4F_IP(b, pInB, 4 * sizeof(uint8_t));
            r = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(r), 1));
            g = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(g), 1));
            b = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(b), 1));

            AE_MUL16X4(dr0, dr1, r, c3);
            AE_MULA16X4(dr0, dr1, g, c4);
            AE_MULA16X4(dr0, dr1, b, c5);
            dr0 = AE_ADD32(dr0, 1 << 21);
            dr1 = AE_ADD32(dr1, 1 << 21);
            u0 = AE_SAT16X4(AE_SRAI32(dr0, 15), AE_SRAI32(dr1, 15));

            u0 = AE_ADD16S(u0, (ae_int16x4)16384);
            AE_MOVT16X4(u0, 0, AE_LT16(u0, 0));
            u0 = AE_SRAI16(u0, 7);

            /*second 4*/

            AE_L8X4F_IP(r, pInR, 4 * sizeof(uint8_t));
            AE_L8X4F_IP(g, pInG, 4 * sizeof(uint8_t));
            AE_L8X4F_IP(b, pInB, 4 * sizeof(uint8_t));
            r = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(r), 1));
            g = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(g), 1));
            b = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(b), 1));

            AE_MUL16X4(dr0, dr1, r, c3);
            AE_MULA16X4(dr0, dr1, g, c4);
            AE_MULA16X4(dr0, dr1, b, c5);
            dr0 = AE_ADD32(dr0, 1 << 21);
            dr1 = AE_ADD32(dr1, 1 << 21);
            u1 = AE_SAT16X4(AE_SRAI32(dr0, 15), AE_SRAI32(dr1, 15));

            u1 = AE_ADD16S(u1, (ae_int16x4)16384);
            AE_MOVT16X4(u1, 0, AE_LT16(u1, 0));
            u1 = AE_SRAI16(u1, 7);

            /* save results */
            t0 = AE_SEL16_7531(u0, u1);
            t1 = AE_SEL16_6420(u0, u1);
            t1 = AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(t1), 8));
            t0 = AE_OR16(t0, t1);
            AE_S16X4_IP(t0, pOutU, sizeof(ae_int16x4));
        }
    }
    /*V*/
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
        pInR = (const signed char*)(inR + i*stride);
        pInG = (const signed char*)(inG + i*stride);
        pInB = (const signed char*)(inB + i*stride);
        pOutV = (ae_int16x4 *)(outV + i*stride);
        for (j = 0; j < (w >> 3); j++)
        {
            AE_L8X4F_IP(r, pInR, 4 * sizeof(uint8_t));
            AE_L8X4F_IP(g, pInG, 4 * sizeof(uint8_t));
            AE_L8X4F_IP(b, pInB, 4 * sizeof(uint8_t));
            r = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(r), 1));
            g = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(g), 1));
            b = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(b), 1));

            AE_MUL16X4(dr0, dr1, r, c6);
            AE_MULA16X4(dr0, dr1, g, c7);
            AE_MULA16X4(dr0, dr1, b, c8);
            dr0 = AE_ADD32(dr0, 1 << 21);
            dr1 = AE_ADD32(dr1, 1 << 21);
            v0 = AE_SAT16X4(AE_SRAI32(dr0, 15), AE_SRAI32(dr1, 15));

            v0 = AE_ADD16S(v0, (ae_int16x4)16384);
            AE_MOVT16X4(v0, 0, AE_LT16(v0, 0));
            v0 = AE_SRAI16(v0, 7);

            /*second 4*/

            AE_L8X4F_IP(r, pInR, 4 * sizeof(uint8_t));
            AE_L8X4F_IP(g, pInG, 4 * sizeof(uint8_t));
            AE_L8X4F_IP(b, pInB, 4 * sizeof(uint8_t));
            r = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(r), 1));
            g = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(g), 1));
            b = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(b), 1));

            AE_MUL16X4(dr0, dr1, r, c6);
            AE_MULA16X4(dr0, dr1, g, c7);
            AE_MULA16X4(dr0, dr1, b, c8);
            dr0 = AE_ADD32(dr0, 1 << 21);
            dr1 = AE_ADD32(dr1, 1 << 21);
            v1 = AE_SAT16X4(AE_SRAI32(dr0, 15), AE_SRAI32(dr1, 15));

            v1 = AE_ADD16S(v1, (ae_int16x4)16384);
            AE_MOVT16X4(v1, 0, AE_LT16(v1, 0));
            v1 = AE_SRAI16(v1, 7);
            /* save results */
            t0 = AE_SEL16_7531(v0, v1);
            t1 = AE_SEL16_6420(v0, v1);
            t1 = AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(t1), 8));
            t0 = AE_OR16(t0, t1);
            AE_S16X4_IP(t0, pOutV, sizeof(ae_int16x4));
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
        for (j = w&(~7); j<w; j++)
        {
            ae_int16x4 y, u, v;
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
