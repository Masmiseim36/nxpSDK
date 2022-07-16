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

/* Library API */
#include "NatureDSP_Signal_vector.h"
#include "NatureDSP_types.h"
#include "common_fpu.h"

#if (HAVE_VFPU==0 && HAVE_FPU==0)
DISCARD_FUN(void,vec_dot_batchf,(float32_t *z, const float32_t   * x,const cfloat32ptr_t * y, int N, int M))
#elif (HAVE_VFPU)
/*-------------------------------------------------------------------------
  Batch Computation of Vector Dot products
  These routines take a set of input vectors and compute their dot product 
  with specific reference data.
  Two versions of routines are available: 
  - regular versions (vec_dot_batch8x8, vec_dot_batch8x16, vec_dot_batch16x16, 
    vec_dot_batchf). They work with arbitratry arguments
  - fast versions (vec_dot_batch8x8_fast, vec_dot_batch8x16_fast, 
    vec_dot_batch16x16_fast, vec_dot_batchf_fast) apply some restrictions.  

  Precision: 
  8x8    8x8-bit data, 16-bit output (fractional multiply Q7xQ7->Q15)
  8x16   8x16-bit data, 16-bit output (fractional multiply Q7xQ15->Q15)
  16x16  16x16-bit data, 16-bit output (fractional multiply Q15xQ15->Q31)
  f      single precision floating point

  Input:
  x[N]     input (reference) data, Q7, Q15 or floating point
  y[M][N]  pointers to M input vectors, Q7, Q15 or floating point
  N        length of vectors
  M        number of vectors
  rsh      right shift for output (for fixed point API only!)
  Output:
  z[M]     dot products between references and M inputs, Q15, Q31 or 
           floating point

  Restrictions:
  Regular versions:
    none
  Faster versions:
    x,y[m] - aligned on 8-byte boundary
    N      - multiple of 8
    M        multiple of 4
-------------------------------------------------------------------------*/
void vec_dot_batchf    (float32_t *z, const float32_t   * x,const cfloat32ptr_t * y        , int N, int M)
{
    int m, n;
    xtfloatx2 vxf, vyf0, vacc0;
    xtfloatx2 vyf1, vacc1;
    xtfloatx2 vyf2, vacc2;
    xtfloatx2 vyf3, vacc3;
    xtfloat xf, yf0, zf0, acc0;
    xtfloat yf1, zf1, acc1;
    xtfloat yf2, zf2, acc2;
    xtfloat yf3, zf3, acc3;
    ae_valign aly0, aly1, aly2, aly3;
    int sh = 0;
    const xtfloatx2 * restrict px = (const xtfloatx2 *)x;
    const xtfloatx2 * restrict py0;
    const xtfloatx2 * restrict py1;
    const xtfloatx2 * restrict py2;
    const xtfloatx2 * restrict py3;
    xtfloat * restrict pz = (xtfloat *)z;
    NASSERT(z);
    NASSERT(x);
    NASSERT(y);
    if (M<=0) return;
    if (N==0) 
    {
        for (m=0; m<M; m++) z[m]=0.f;
        return;
    }
    for (m = 0; m < (M>>2); m++)
    {
        px = (const xtfloatx2 *)x;
        py0 = (const xtfloatx2*)(y[4 * m + 0]);
        py1 = (const xtfloatx2*)(y[4 * m + 1]);
        py2 = (const xtfloatx2*)(y[4 * m + 2]);
        py3 = (const xtfloatx2*)(y[4 * m + 3]);
        vacc0 = XT_MOV_SX2(0.f);
        vacc1 = XT_MOV_SX2(0.f);
        vacc2 = XT_MOV_SX2(0.f);
        vacc3 = XT_MOV_SX2(0.f);
        zf0 = XT_MOV_S(0.f);
        zf1 = XT_MOV_S(0.f);
        zf2 = XT_MOV_S(0.f);
        zf3 = XT_MOV_S(0.f);
        sh = 0;
        if ((((uintptr_t)(x)) & 7))
        {
            ae_int32x2 tmp;
            xf = XT_LSI((const xtfloat *)px, 0);
            yf0 = XT_LSI((const xtfloat *)py0, 0);
            yf1 = XT_LSI((const xtfloat *)py1, 0);
            yf2 = XT_LSI((const xtfloat *)py2, 0);
            yf3 = XT_LSI((const xtfloat *)py3, 0);
            AE_L32_IP(tmp, castxcc(ae_int32, px), sizeof(float32_t));
            AE_L32_IP(tmp, castxcc(ae_int32, py0), sizeof(float32_t));
            AE_L32_IP(tmp, castxcc(ae_int32, py1), sizeof(float32_t));
            AE_L32_IP(tmp, castxcc(ae_int32, py2), sizeof(float32_t));
            AE_L32_IP(tmp, castxcc(ae_int32, py3), sizeof(float32_t));
            zf0 = XT_MUL_S(xf, yf0);
            zf1 = XT_MUL_S(xf, yf1);
            zf2 = XT_MUL_S(xf, yf2);
            zf3 = XT_MUL_S(xf, yf3);
            sh=1;
        }
        aly0 = AE_LA64_PP(py0);
        aly1 = AE_LA64_PP(py1);
        aly2 = AE_LA64_PP(py2);
        aly3 = AE_LA64_PP(py3);
        for (n = 0; n < ((N-sh) >> 1); n++)
        {
            XT_LSX2IP(vxf, px, 2 * sizeof(float32_t));
            XT_LASX2IP(vyf0, aly0, py0);
            XT_LASX2IP(vyf1, aly1, py1);
            XT_LASX2IP(vyf2, aly2, py2);
            XT_LASX2IP(vyf3, aly3, py3);
            XT_MADD_SX2(vacc0, vxf, vyf0);
            XT_MADD_SX2(vacc1, vxf, vyf1);
            XT_MADD_SX2(vacc2, vxf, vyf2);
            XT_MADD_SX2(vacc3, vxf, vyf3);
        }
        if ((N-sh) & 1)
        {
            xf = XT_LSI((const xtfloat *)px, 0);
            yf0 = XT_LSI((const xtfloat *)py0, 0);
            yf1 = XT_LSI((const xtfloat *)py1, 0);
            yf2 = XT_LSI((const xtfloat *)py2, 0);
            yf3 = XT_LSI((const xtfloat *)py3, 0);
            XT_MADD_S(zf0, xf, yf0);
            XT_MADD_S(zf1, xf, yf1);
            XT_MADD_S(zf2, xf, yf2);
            XT_MADD_S(zf3, xf, yf3);
        }
        acc0 = XT_RADD_SX2(vacc0);
        acc1 = XT_RADD_SX2(vacc1);
        acc2 = XT_RADD_SX2(vacc2);
        acc3 = XT_RADD_SX2(vacc3);
        acc0 = XT_ADD_S(acc0, zf0);
        acc1 = XT_ADD_S(acc1, zf1);
        acc2 = XT_ADD_S(acc2, zf2);
        acc3 = XT_ADD_S(acc3, zf3);
        XT_SSIP(acc0, pz, sizeof(xtfloat));
        XT_SSIP(acc1, pz, sizeof(xtfloat));
        XT_SSIP(acc2, pz, sizeof(xtfloat));
        XT_SSIP(acc3, pz, sizeof(xtfloat));
    }
    if (M & 3)
    {
        for (m = (M&(~3)); m < M; m++)
        {
            px = (const xtfloatx2 *)x;
            py0 = (const xtfloatx2*)(y[m]);
            vacc0 = XT_MOV_SX2(0.f);
            zf0 = XT_MOV_S(0.f);
            sh = 0;
            if ((((uintptr_t)(x)) & 7))
            {
                ae_int32x2 tmp;
                xf = XT_LSI((const xtfloat *)px, 0);
                yf0 = XT_LSI((const xtfloat *)py0, 0);
                AE_L32_IP(tmp, castxcc(ae_int32, px), sizeof(float32_t));
                AE_L32_IP(tmp, castxcc(ae_int32, py0), sizeof(float32_t));
                zf0 = XT_MUL_S(xf, yf0);
                sh=1;
            }
            aly0 = AE_LA64_PP(py0);        
            for (n = 0; n < ((N-sh) >> 1); n++)
            {
                XT_LSX2IP(vxf, px, 2 * sizeof(float32_t));
                XT_LASX2IP(vyf0, aly0, py0);
                XT_MADD_SX2(vacc0, vxf, vyf0);
            }
            if ((N-sh) & 1)
            {
                xf = XT_LSI((const xtfloat *)px, 0);
                yf0 = XT_LSI((const xtfloat *)py0, 0);
                XT_MADD_S(zf0, xf, yf0);
            }
            acc0 = XT_RADD_SX2(vacc0);
            acc0 = XT_ADD_S(acc0, zf0);
            XT_SSIP(acc0, pz, sizeof(xtfloat));
        }
    }
}
#else //SFPU
void vec_dot_batchf    (float32_t *z, const float32_t   * x,const cfloat32ptr_t * y        , int N, int M)
{
    xtfloat acc0, acc1, acc2, acc3, x0, y0, y1, y2, y3;
    int m, n;
    NASSERT(z); 
    NASSERT(x); 
    NASSERT(y);
    const xtfloat * restrict px = (const xtfloat*)x;
    const xtfloat * restrict py0;
    const xtfloat * restrict py1;
    const xtfloat * restrict py2;
    const xtfloat * restrict py3;
    xtfloat * restrict pz = (xtfloat *)z;
    if (M <= 0) return;
    if (N == 0)
    {
        for (m = 0; m<M; m++) z[m] = 0.f;
        return;
    }
    for (m = 0; m<(M>>2); m++)
    {
        px = (const xtfloat *)x;
        py0 = (const xtfloat*)(y[4*m]);
        py1 = (const xtfloat*)(y[4*m + 1]);
        py2 = (const xtfloat*)(y[4*m + 2]);
        py3 = (const xtfloat*)(y[4*m + 3]);
        acc0 = acc1 = XT_CONST_S(0);
        acc2 = acc3 = XT_CONST_S(0);
        for (n = 0; n<N; n++)
        {
            XT_LSIP(x0, px, sizeof(xtfloat));
            XT_LSIP(y0, py0, sizeof(xtfloat));
            XT_LSIP(y1, py1, sizeof(xtfloat));
            XT_LSIP(y2, py2, sizeof(xtfloat));
            XT_LSIP(y3, py3, sizeof(xtfloat));
            XT_MADD_S(acc0, x0, y0);
            XT_MADD_S(acc1, x0, y1);           
            XT_MADD_S(acc2, x0, y2);
            XT_MADD_S(acc3, x0, y3);
        }
        XT_SSIP(acc0, pz, sizeof(xtfloat));
        XT_SSIP(acc1, pz, sizeof(xtfloat));
        XT_SSIP(acc2, pz, sizeof(xtfloat));
        XT_SSIP(acc3, pz, sizeof(xtfloat));
    }
    if (M & 3)
    {
        for (m = (M&(~3)); m<M; m++)
        {
            px = (const xtfloat *)x;
            py0 = (const xtfloat*)(y[m]);
            acc0 = XT_CONST_S(0);
            for (n = 0; n<N; n++)
            {
                XT_LSIP(x0, px, sizeof(xtfloat));
                XT_LSIP(y0, py0, sizeof(xtfloat));
                XT_MADD_S(acc0, x0, y0);
            }
            XT_SSIP(acc0, pz, sizeof(xtfloat));
        }
    }
}
#endif
