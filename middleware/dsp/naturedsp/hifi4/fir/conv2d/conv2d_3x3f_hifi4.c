/* ------------------------------------------------------------------------ */
/* Copyright (c) 2018 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
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
/*          Copyright (C) 2015-2018 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */

/*
  NatureDSP Signal Processing Library. FIR part
  2D Convolution  
  IntegrIT, 2006-2018
*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fir.h"
#include "common.h"
#include "common_fpu.h"


/*-------------------------------------------------------------------------
  2D convolution
  Functions compute the two-dimensional convolution of input matrix x[M][N]
  and y[P][Q] and store the result in matrix z[M+P-1][N+Q-1]
  Additional parameter rsh allows to control fixed point representation of 
  output data.

  Precision: 
  8x8      8-bit coefficients, 8-bit data, 8-bit output, Q7
  8x16     8-bit coefficients Q7, 16-bit data, 16-bit output, Q15
  16x16    16-bit coefficients, 16-bit data, 16-bit output, Q15
  f        floating point data

  Input:
  x[M][N]   input data Q15, Q7, floating point
  y[P][Q]   input data Q15, Q7, floating point
  M         number of rows in the matrix x
  N         number of columns in the matrix x
  P         number of rows in the matrix y
  Q         number of columns in the matrix y
  rsh       additional right shift (for fixed point API only)

  Output:
  z	[M+P-1][N+Q-1] output data, Q(7-rsh), Q(15-rsh), floating point

  Temporary:
  pScr     scratch data. Should have size at least as requested by 
           corresponding scratch allocation function

  Restrictions:
  x,y,z        should not overlap
  x,y,z,pScr   aligned on a 8-bytes boundary
  P, Q	       >0 and multiplies of 8
-------------------------------------------------------------------------*/

#if  (HAVE_FPU==0 && HAVE_VFPU==0)
size_t conv2d_3x3f_getScratchSize (int P, int Q) 
{ 
    (void)P,(void)Q;
    return 0;
}
DISCARD_FUN(void,conv2d_3x3f,(void* pScr, float32_t *z, const float32_t * x, const float32_t * y, int P, int Q))
#elif HAVE_VFPU

/* macro defines load I xtfloat with replicate to xtfloatx2 */
#define MACRO_LSI_REPL(v, p, off)\
{                                               \
    ae_int32x2 v_i;                             \
    v_i = AE_L32_I((const ae_int32*)(p), off);  \
    v = XT_AE_MOVXTFLOATX2_FROMINT32X2(v_i);    \
}

/* macro defines load XP xtfloat with replicate to xtfloatx2 */
#define MACRO_LSIP_REPL(v, p, off) \
{                                                       \
    ae_int32x2 v_i;                                     \
    AE_L32_IP(v_i, castxcc(const ae_int32, p), off);    \
    v = XT_AE_MOVXTFLOATX2_FROMINT32X2(v_i);            \
}

/* macro defines load XP xtfloat with replicate to xtfloatx2 */
#define MACRO_LSXP_REPL(v, p, off) \
{                                                       \
    ae_int32x2 v_i;                                     \
    AE_L32_XP(v_i, castxcc(const ae_int32, p), (off));  \
    v = XT_AE_MOVXTFLOATX2_FROMINT32X2(v_i);            \
}

/* macro defines load XC xtfloat with replicate to xtfloatx2 */
#define MACRO_LSXC_REPL(v, p, off)                      \
{                                                       \
    ae_int32x2 v_i;                                     \
    AE_L32_XC(v_i, castxcc(const ae_int32, p), off);    \
    v = XT_AE_MOVXTFLOATX2_FROMINT32X2(v_i);            \
}

/* macro defines load XC1 xtfloatx2 */
#define MACRO_LSX2XC1(v, p, off)\
{                                                           \
    ae_int32x2 v_i;                                         \
    AE_L32X2_XC1(v_i, castxcc(const ae_int32x2, p), off);   \
    v = XT_AE_MOVXTFLOATX2_FROMINT32X2(v_i);                \
}

#define MACRO_ADD_HL_LH_XC2(v) XT_MADDMUX_S(v, (xtfloatx2)(1.0f), v, 5)

void conv2d_3x3f(void* pScr, float32_t *z, const float32_t * x, const float32_t * y, int P, int Q)
{
#   define M 3
#   define N 3
	int i, j;

    const xtfloatx2 * restrict pX;
    const xtfloatx2 * restrict pY0;
    const xtfloatx2 * restrict pY1;
    const xtfloatx2 * restrict pY2;
    xtfloatx2 * restrict pW;
    xtfloatx2 * restrict pWwr;
    xtfloatx2 * restrict pZ;
    xtfloatx2 * restrict pz0;
    xtfloatx2 * restrict pz1;
    ae_valign aX;
    float32_t* pwr = (float32_t*)pScr;

    xtfloatx2 tmp;
    xtfloatx2 z01, z23;
    xtfloatx2 w00, w11, w22;
    xtfloatx2 y01, y23, y45, y12, y34;

    NASSERT(x);
    NASSERT(y);
    NASSERT(z);
    NASSERT(pScr);
    NASSERT_ALIGN(x, HIFI_SIMD_WIDTH);
    NASSERT_ALIGN(y, HIFI_SIMD_WIDTH);
    NASSERT_ALIGN(z, HIFI_SIMD_WIDTH);
    NASSERT_ALIGN(pScr, HIFI_SIMD_WIDTH);
    NASSERT(P >= 0 && P % 8 == 0);
    NASSERT(Q >= 0 && Q % 8 == 0);

    if (P <= 0 || Q <= 0) return;

    pX = (xtfloatx2 *)x;
    pW = (xtfloatx2 *)pScr + (M*N - 1);
    for (i = 0; i < ((M*N) >> 1); i++)
    {
        XT_LSX2IP(w00, pX, sizeof(xtfloatx2));
        tmp = XT_SEL32_HH_SX2(w00, w00);
        XT_SSX2XP(tmp, pW, -1 * ((int)sizeof(xtfloatx2)));
        tmp = XT_SEL32_LL_SX2(w00, w00);
        XT_SSX2XP(tmp, pW, -1 * ((int)sizeof(xtfloatx2)));
    }
    pX = (xtfloatx2 *)((float32_t*)pX - 1);
    aX = XT_LASX2PP(pX);
    XT_LASX2IP(w00, aX, pX);
    tmp = XT_SEL32_LL_SX2(w00, w00);
    XT_SSX2XP(tmp, pW, -1 * ((int)sizeof(xtfloatx2)));

    __Pragma("no_reorder")

    /*
    * Processing of convolution
    */
    for (i = 0; i < M - 1; i++)
    {
        /* Process first N-1 elements */
        if (i == 0)
        {
            tmp = XT_MOV_SX2(0.0f);
            pY0 = (const xtfloatx2 *)(y + (0)*Q);
            pWwr = (xtfloatx2*)(pwr + 2*6);
            w11 = XT_LSX2I(pWwr, 1 * sizeof(xtfloatx2));
            w22 = XT_LSX2I(pWwr, 2 * sizeof(xtfloatx2));
            y01 = XT_LSX2I(pY0, 0);
            z01 = XT_MUL_SX2(w22, y01);
            y01 = XT_SEL32_LH_SX2(tmp, y01);
            XT_MADD_SX2(z01, w11, y01);

            pZ = (xtfloatx2 *)(z + i*(N + Q - 1));
            XT_SSX2IP(z01, pZ, sizeof(xtfloatx2));
        }
        if (i == 1)
        {
            tmp = XT_MOV_SX2(0.0f);
            pY0 = (const xtfloatx2 *)(y + (0)*Q);
            pWwr = (xtfloatx2*)(pwr + 6);
            w11 = XT_LSX2I(pWwr, 1 * sizeof(xtfloatx2));
            w22 = XT_LSX2I(pWwr, 2 * sizeof(xtfloatx2));
            y01 = XT_LSX2I(pY0, 0);
            z01 = XT_MUL_SX2(w22, y01);
            y01 = XT_SEL32_LH_SX2(tmp, y01);
            XT_MADD_SX2(z01, w11, y01);

            pY1 = (const xtfloatx2 *)(y + (1)*Q);
            pWwr = (xtfloatx2*)(pwr + 2*6);
            w11 = XT_LSX2I(pWwr, 1 * sizeof(xtfloatx2));
            w22 = XT_LSX2I(pWwr, 2 * sizeof(xtfloatx2));
            y01 = XT_LSX2I(pY1, 0);
            XT_MADD_SX2(z01, w22, y01);
            y01 = XT_SEL32_LH_SX2(tmp, y01);
            XT_MADD_SX2(z01, w11, y01);;

            pZ = (xtfloatx2 *)(z + i*(N + Q - 1));
            XT_SSX2IP(z01, pZ, sizeof(xtfloatx2));
        }
        
        /* Main section */
        if (i == 0)
        {
            pY0 = (const xtfloatx2 *)(y + (0)*Q);
            pWwr = (xtfloatx2*)(pwr + 2*6);
            pz0 = (xtfloatx2*)(pwr + 3 * 6);
            XT_LSX2IP(w00, pWwr, sizeof(xtfloatx2));
            XT_LSX2IP(w11, pWwr, sizeof(xtfloatx2));
            XT_LSX2IP(w22, pWwr, sizeof(xtfloatx2));

            for (j = N - 1; j < Q; j += 4)
            {
                XT_LSX2IP(y01, pY0, sizeof(xtfloatx2));
                XT_LSX2IP(y23, pY0, sizeof(xtfloatx2));
                y45 = XT_LSX2I(pY0, 0);
                y12 = XT_SEL32_LH_SX2(y01, y23);
                y34 = XT_SEL32_LH_SX2(y23, y45);
                z01 = XT_MUL_SX2(w00, y01);
                z23 = XT_MUL_SX2(w00, y23);
                XT_MADD_SX2(z01, w11, y12);
                XT_MADD_SX2(z01, w22, y23);
                XT_MADD_SX2(z23, w11, y34);
                XT_MADD_SX2(z23, w22, y45);
                XT_SSX2IP(z01, pZ, sizeof(xtfloatx2));
                XT_SSX2IP(z23, pZ, sizeof(xtfloatx2));
            }
        }
        if (i == 1)
        {
            pY0 = (const xtfloatx2 *)(y + (0)*Q);
            pWwr = (xtfloatx2*)(pwr + 6);
            pz0 = (xtfloatx2*)(pwr + 3 * 6);
            XT_LSX2IP(w00, pWwr, sizeof(xtfloatx2));
            XT_LSX2IP(w11, pWwr, sizeof(xtfloatx2));
            XT_LSX2IP(w22, pWwr, sizeof(xtfloatx2));

            for (j = N - 1; j < Q; j += 4)
            {
                XT_LSX2IP(y01, pY0, sizeof(xtfloatx2));
                XT_LSX2IP(y23, pY0, sizeof(xtfloatx2));
                y45 = XT_LSX2I(pY0, 0);
                y12 = XT_SEL32_LH_SX2(y01, y23);
                y34 = XT_SEL32_LH_SX2(y23, y45);
                z01 = XT_MUL_SX2(w00, y01);
                z23 = XT_MUL_SX2(w00, y23);
                XT_MADD_SX2(z01, w11, y12);
                XT_MADD_SX2(z01, w22, y23);
                XT_MADD_SX2(z23, w11, y34);
                XT_MADD_SX2(z23, w22, y45);
                XT_SSX2IP(z01, pz0, sizeof(xtfloatx2));
                XT_SSX2IP(z23, pz0, sizeof(xtfloatx2));
            }
            __Pragma("no_reorder")
            pz0 = (xtfloatx2*)(pwr + 3 * 6);
            pY1 = (const xtfloatx2 *)(y + (1)*Q);
            XT_LSX2IP(w00, pWwr, sizeof(xtfloatx2));
            XT_LSX2IP(w11, pWwr, sizeof(xtfloatx2));
            XT_LSX2IP(w22, pWwr, sizeof(xtfloatx2));
            for (j = N - 1; j < Q; j += 4)
            {
                XT_LSX2IP(z01, pz0, sizeof(xtfloatx2));
                XT_LSX2IP(z23, pz0, sizeof(xtfloatx2));
                XT_LSX2IP(y01, pY1, sizeof(xtfloatx2));
                XT_LSX2IP(y23, pY1, sizeof(xtfloatx2));
                y45 = XT_LSX2I(pY1, 0);
                y12 = XT_SEL32_LH_SX2(y01, y23);
                y34 = XT_SEL32_LH_SX2(y23, y45);
                XT_MADD_SX2(z01, w00, y01);
                XT_MADD_SX2(z23, w00, y23);
                XT_MADD_SX2(z01, w11, y12);
                XT_MADD_SX2(z01, w22, y23);
                XT_MADD_SX2(z23, w11, y34);
                XT_MADD_SX2(z23, w22, y45);
                XT_SSX2IP(z01, pZ, sizeof(xtfloatx2));
                XT_SSX2IP(z23, pZ, sizeof(xtfloatx2));
            }
        }         

        /* Process last N-1 elements */
        if (i == 0)
        {
            tmp = XT_MOV_SX2(0.0f);
            pY0 = (const xtfloatx2 *)(y + (0)*Q + (Q - 2));
            pWwr = (xtfloatx2*)(pwr + 2*6);
            w00 = XT_LSX2I(pWwr, 0 * sizeof(xtfloatx2));
            w11 = XT_LSX2I(pWwr, 1 * sizeof(xtfloatx2));
            y01 = XT_LSX2I(pY0, 0);
            z01 = XT_MUL_SX2(w00, y01);
            y01 = XT_SEL32_LL_SX2(y01, tmp);
            XT_MADD_SX2(z01, w11, y01);
            pZ = (xtfloatx2 *)(z + i*(N + Q - 1) + Q);
            XT_SSX2IP(z01, pZ, sizeof(xtfloatx2));
        }
        if (i == 1)
        {
            tmp = XT_MOV_SX2(0.0f);
            pY0 = (const xtfloatx2 *)(y + (0)*Q + (Q - 2));
            pWwr = (xtfloatx2*)(pwr + 6);
            w00 = XT_LSX2I(pWwr, 0 * sizeof(xtfloatx2));
            w11 = XT_LSX2I(pWwr, 1 * sizeof(xtfloatx2));
            y01 = XT_LSX2I(pY0, 0);
            z01 = XT_MUL_SX2(w00, y01);
            y01 = XT_SEL32_LL_SX2(y01, tmp);
            XT_MADD_SX2(z01, w11, y01);

            pY1 = (const xtfloatx2 *)(y + (1)*Q + (Q - 2));
            pWwr = (xtfloatx2*)(pwr + 2*6);
            w00 = XT_LSX2I(pWwr, 0 * sizeof(xtfloatx2));
            w11 = XT_LSX2I(pWwr, 1 * sizeof(xtfloatx2));
            y01 = XT_LSX2I(pY1, 0);
            XT_MADD_SX2(z01, w00, y01);
            y01 = XT_SEL32_LL_SX2(y01, tmp);
            XT_MADD_SX2(z01, w11, y01);

            pZ = (xtfloatx2 *)(z + i*(N + Q - 1) + Q);
            XT_SSX2IP(z01, pZ, sizeof(xtfloatx2));
        }     
    }

	for (i = M-1; i < P; i++)
	{	
        /* Process first N-1 elements */
        tmp = XT_MOV_SX2(0.0f);
        pY0 = (const xtfloatx2 *)(y + (i - (M-1))*Q);     
        pWwr = (xtfloatx2*)(pwr);   
        w11 = XT_LSX2I(pWwr, 1*sizeof(xtfloatx2));
        w22 = XT_LSX2I(pWwr, 2*sizeof(xtfloatx2));
        y01 = XT_LSX2I(pY0, 0);
        z01 = XT_MUL_SX2(w22, y01);
        y01 = XT_SEL32_LH_SX2(tmp, y01);
        XT_MADD_SX2(z01, w11, y01);

        pY1 = (const xtfloatx2 *)(y + (i - (M - 1) + 1)*Q);
        pWwr = (xtfloatx2*)(pwr + 6);
        w11 = XT_LSX2I(pWwr, 1 * sizeof(xtfloatx2));
        w22 = XT_LSX2I(pWwr, 2 * sizeof(xtfloatx2));
        y01 = XT_LSX2I(pY1, 0);
        XT_MADD_SX2(z01, w22, y01);
        y01 = XT_SEL32_LH_SX2(tmp, y01);
        XT_MADD_SX2(z01, w11, y01);

        pY2 = (const xtfloatx2 *)(y + (i - (M - 1) + 2)*Q);
        pWwr = (xtfloatx2*)(pwr + 12);
        w11 = XT_LSX2I(pWwr, 1 * sizeof(xtfloatx2));
        w22 = XT_LSX2I(pWwr, 2 * sizeof(xtfloatx2));
        y01 = XT_LSX2I(pY2, 0);
        XT_MADD_SX2(z01, w22, y01);
        y01 = XT_SEL32_LH_SX2(tmp, y01);
        XT_MADD_SX2(z01, w11, y01);

        pZ = (xtfloatx2 *)(z + i*(N + Q - 1));
        XT_SSX2IP(z01, pZ, sizeof(xtfloatx2));
      
        /* Main section */
        pY0 = (const xtfloatx2 *)(y + (i - (M-1))*Q);     
        pWwr = (xtfloatx2*)(pwr);
        pz0 = (xtfloatx2*)(pwr + 3 * 6);
      
        XT_LSX2IP(w00, pWwr, sizeof(xtfloatx2));
        XT_LSX2IP(w11, pWwr, sizeof(xtfloatx2));
        XT_LSX2IP(w22, pWwr, sizeof(xtfloatx2));
              
        for (j = N - 1; j < Q; j += 4)
        {
            XT_LSX2IP(y01, pY0, sizeof(xtfloatx2));
            XT_LSX2IP(y23, pY0, sizeof(xtfloatx2));
            y45 = XT_LSX2I(pY0, 0);
            y12 = XT_SEL32_LH_SX2(y01, y23);
            y34 = XT_SEL32_LH_SX2(y23, y45);
            z01 = XT_MUL_SX2(w00, y01);
            z23 = XT_MUL_SX2(w00, y23);
            XT_MADD_SX2(z01, w22, y23);
            XT_MADD_SX2(z01, w11, y12);
            XT_MADD_SX2(z23, w22, y45);
            XT_MADD_SX2(z23, w11, y34);           
            XT_SSX2IP(z01, pz0, sizeof(xtfloatx2));
            XT_SSX2IP(z23, pz0, sizeof(xtfloatx2));
        }

        __Pragma("no_reorder")

        pz0 = (xtfloatx2*)(pwr + 3 * 6);
        pz1 = (xtfloatx2*)(pwr + 3 * 6 + Q);
        pY1 = (const xtfloatx2 *)(y + (i - (M - 1) + 1)*Q);
        XT_LSX2IP(w00, pWwr, sizeof(xtfloatx2));
        XT_LSX2IP(w11, pWwr, sizeof(xtfloatx2));
        XT_LSX2IP(w22, pWwr, sizeof(xtfloatx2));
        for (j = N - 1; j < Q; j += 4)
        {
            XT_LSX2IP(z01, pz0, sizeof(xtfloatx2));
            XT_LSX2IP(z23, pz0, sizeof(xtfloatx2));
            XT_LSX2IP(y01, pY1, sizeof(xtfloatx2));
            XT_LSX2IP(y23, pY1, sizeof(xtfloatx2));
            y45 = XT_LSX2I(pY1, 0);
            y12 = XT_SEL32_LH_SX2(y01, y23);
            y34 = XT_SEL32_LH_SX2(y23, y45);
            XT_MADD_SX2(z01, w00, y01);
            XT_MADD_SX2(z23, w00, y23);
            XT_MADD_SX2(z01, w11, y12);
            XT_MADD_SX2(z01, w22, y23);
            XT_MADD_SX2(z23, w11, y34);
            XT_MADD_SX2(z23, w22, y45);
            XT_SSX2IP(z01, pz1, sizeof(xtfloatx2));
            XT_SSX2IP(z23, pz1, sizeof(xtfloatx2));
        }
        __Pragma("no_reorder")
 
        pz1 = (xtfloatx2*)(pwr + 3 * 6 + Q);
        pZ = (xtfloatx2 *)(z + i*(N + Q - 1) + (N-1));
        pY2 = (const xtfloatx2 *)(y + (i - (M - 1) + 2)*Q);
        XT_LSX2IP(w00, pWwr, sizeof(xtfloatx2));
        XT_LSX2IP(w11, pWwr, sizeof(xtfloatx2));
        XT_LSX2IP(w22, pWwr, sizeof(xtfloatx2));
        for (j = N - 1; j < Q; j += 4)
        {
            XT_LSX2IP(z01, pz1, sizeof(xtfloatx2));
            XT_LSX2IP(z23, pz1, sizeof(xtfloatx2));
            XT_LSX2IP(y01, pY2, sizeof(xtfloatx2));
            XT_LSX2IP(y23, pY2, sizeof(xtfloatx2));
            y45 = XT_LSX2I(pY2, 0);
            y12 = XT_SEL32_LH_SX2(y01, y23);
            y34 = XT_SEL32_LH_SX2(y23, y45);
            XT_MADD_SX2(z01, w00, y01);
            XT_MADD_SX2(z23, w00, y23);
            XT_MADD_SX2(z01, w11, y12);
            XT_MADD_SX2(z01, w22, y23);
            XT_MADD_SX2(z23, w11, y34);
            XT_MADD_SX2(z23, w22, y45);
            XT_SSX2IP(z01, pZ, sizeof(xtfloatx2));
            XT_SSX2IP(z23, pZ, sizeof(xtfloatx2));
        } 
        /* Process last N-1 elements */
        tmp = XT_MOV_SX2(0.0f);
        pY0 = (const xtfloatx2 *)(y + (i - (M - 1))*Q + (Q-2));
        pWwr = (xtfloatx2*)(pwr);
        w00 = XT_LSX2I(pWwr, 0 * sizeof(xtfloatx2));
        w11 = XT_LSX2I(pWwr, 1 * sizeof(xtfloatx2));
        y01 = XT_LSX2I(pY0, 0);
        z01 = XT_MUL_SX2(w00, y01);
        y01 = XT_SEL32_LL_SX2(y01, tmp);
        XT_MADD_SX2(z01, w11, y01);

        pY1 = (const xtfloatx2 *)(y + (i - (M - 1) + 1)*Q + (Q - 2));
        pWwr = (xtfloatx2*)(pwr + 6);
        w00 = XT_LSX2I(pWwr, 0 * sizeof(xtfloatx2));
        w11 = XT_LSX2I(pWwr, 1 * sizeof(xtfloatx2));
        y01 = XT_LSX2I(pY1, 0);
        XT_MADD_SX2(z01, w00, y01);
        y01 = XT_SEL32_LL_SX2(y01, tmp);
        XT_MADD_SX2(z01, w11, y01);

        pY2 = (const xtfloatx2 *)(y + (i - (M - 1) + 2)*Q + (Q - 2));
        pWwr = (xtfloatx2*)(pwr + 12);
        w00 = XT_LSX2I(pWwr, 0 * sizeof(xtfloatx2));
        w11 = XT_LSX2I(pWwr, 1 * sizeof(xtfloatx2));
        y01 = XT_LSX2I(pY2, 0);
        XT_MADD_SX2(z01, w00, y01);
        y01 = XT_SEL32_LL_SX2(y01, tmp);
        XT_MADD_SX2(z01, w11, y01);

        pZ = (xtfloatx2 *)(z + i*(N + Q - 1) + Q);
        XT_SSX2IP(z01, pZ, sizeof(xtfloatx2));
	}

    for (i = P; i < P + M - 1; i++)
    {
        /* Process first N-1 elements */
        if (i == P+1)
        {
            tmp = XT_MOV_SX2(0.0f);
            pY0 = (const xtfloatx2 *)(y + (i-2)*Q);
            pWwr = (xtfloatx2*)(pwr);
            w11 = XT_LSX2I(pWwr, 1 * sizeof(xtfloatx2));
            w22 = XT_LSX2I(pWwr, 2 * sizeof(xtfloatx2));
            y01 = XT_LSX2I(pY0, 0);
            z01 = XT_MUL_SX2(w22, y01);
            y01 = XT_SEL32_LH_SX2(tmp, y01);
            XT_MADD_SX2(z01, w11, y01);

            pZ = (xtfloatx2 *)(z + i*(N + Q - 1));
            XT_SSX2IP(z01, pZ, sizeof(xtfloatx2));
        }
        if (i == P)
        {
            tmp = XT_MOV_SX2(0.0f);
            pY0 = (const xtfloatx2 *)(y + (i-2)*Q);
            pWwr = (xtfloatx2*)(pwr);
            w11 = XT_LSX2I(pWwr, 1 * sizeof(xtfloatx2));
            w22 = XT_LSX2I(pWwr, 2 * sizeof(xtfloatx2));
            y01 = XT_LSX2I(pY0, 0);
            z01 = XT_MUL_SX2(w22, y01);
            y01 = XT_SEL32_LH_SX2(tmp, y01);
            XT_MADD_SX2(z01, w11, y01);

            pY1 = (const xtfloatx2 *)(y + (i-1)*Q);
            pWwr = (xtfloatx2*)(pwr + 6);
            w11 = XT_LSX2I(pWwr, 1 * sizeof(xtfloatx2));
            w22 = XT_LSX2I(pWwr, 2 * sizeof(xtfloatx2));
            y01 = XT_LSX2I(pY1, 0);
            XT_MADD_SX2(z01, w22, y01);
            y01 = XT_SEL32_LH_SX2(tmp, y01);
            XT_MADD_SX2(z01, w11, y01);;

            pZ = (xtfloatx2 *)(z + i*(N + Q - 1));
            XT_SSX2IP(z01, pZ, sizeof(xtfloatx2));
        }

        /* Main section */
        if (i == P+1)
        {
            pY0 = (const xtfloatx2 *)(y + (i-2)*Q);
            pWwr = (xtfloatx2*)(pwr);
            pz0 = (xtfloatx2*)(pwr + 3 * 6);
            XT_LSX2IP(w00, pWwr, sizeof(xtfloatx2));
            XT_LSX2IP(w11, pWwr, sizeof(xtfloatx2));
            XT_LSX2IP(w22, pWwr, sizeof(xtfloatx2));

            for (j = N - 1; j < Q; j += 4)
            {
                XT_LSX2IP(y01, pY0, sizeof(xtfloatx2));
                XT_LSX2IP(y23, pY0, sizeof(xtfloatx2));
                y45 = XT_LSX2I(pY0, 0);
                y12 = XT_SEL32_LH_SX2(y01, y23);
                y34 = XT_SEL32_LH_SX2(y23, y45);
                z01 = XT_MUL_SX2(w00, y01);
                z23 = XT_MUL_SX2(w00, y23);
                XT_MADD_SX2(z01, w11, y12);
                XT_MADD_SX2(z01, w22, y23);
                XT_MADD_SX2(z23, w11, y34);
                XT_MADD_SX2(z23, w22, y45);
                XT_SSX2IP(z01, pZ, sizeof(xtfloatx2));
                XT_SSX2IP(z23, pZ, sizeof(xtfloatx2));
            }
        }
        if (i == P)
        {
            pY0 = (const xtfloatx2 *)(y + (i-2)*Q);
            pWwr = (xtfloatx2*)(pwr);
            pz0 = (xtfloatx2*)(pwr + 3 * 6);
            XT_LSX2IP(w00, pWwr, sizeof(xtfloatx2));
            XT_LSX2IP(w11, pWwr, sizeof(xtfloatx2));
            XT_LSX2IP(w22, pWwr, sizeof(xtfloatx2));

            for (j = N - 1; j < Q; j += 4)
            {
                XT_LSX2IP(y01, pY0, sizeof(xtfloatx2));
                XT_LSX2IP(y23, pY0, sizeof(xtfloatx2));
                y45 = XT_LSX2I(pY0, 0);
                y12 = XT_SEL32_LH_SX2(y01, y23);
                y34 = XT_SEL32_LH_SX2(y23, y45);
                z01 = XT_MUL_SX2(w00, y01);
                z23 = XT_MUL_SX2(w00, y23);
                XT_MADD_SX2(z01, w11, y12);
                XT_MADD_SX2(z01, w22, y23);
                XT_MADD_SX2(z23, w11, y34);
                XT_MADD_SX2(z23, w22, y45);
                XT_SSX2IP(z01, pz0, sizeof(xtfloatx2));
                XT_SSX2IP(z23, pz0, sizeof(xtfloatx2));
            }
            __Pragma("no_reorder")
            pz0 = (xtfloatx2*)(pwr + 3 * 6);
            pY1 = (const xtfloatx2 *)(y + (i-1)*Q);
            XT_LSX2IP(w00, pWwr, sizeof(xtfloatx2));
            XT_LSX2IP(w11, pWwr, sizeof(xtfloatx2));
            XT_LSX2IP(w22, pWwr, sizeof(xtfloatx2));
            for (j = N - 1; j < Q; j += 4)
            {
                XT_LSX2IP(z01, pz0, sizeof(xtfloatx2));
                XT_LSX2IP(z23, pz0, sizeof(xtfloatx2));
                XT_LSX2IP(y01, pY1, sizeof(xtfloatx2));
                XT_LSX2IP(y23, pY1, sizeof(xtfloatx2));
                y45 = XT_LSX2I(pY1, 0);
                y12 = XT_SEL32_LH_SX2(y01, y23);
                y34 = XT_SEL32_LH_SX2(y23, y45);
                XT_MADD_SX2(z01, w00, y01);
                XT_MADD_SX2(z23, w00, y23);
                XT_MADD_SX2(z01, w11, y12);
                XT_MADD_SX2(z01, w22, y23);
                XT_MADD_SX2(z23, w11, y34);
                XT_MADD_SX2(z23, w22, y45);
                XT_SSX2IP(z01, pZ, sizeof(xtfloatx2));
                XT_SSX2IP(z23, pZ, sizeof(xtfloatx2));
            }
        }

        /* Process last N-1 elements */
        if (i == P+1)
        {
            tmp = XT_MOV_SX2(0.0f);
            pY0 = (const xtfloatx2 *)(y + (i-2)*Q + (Q - 2));
            pWwr = (xtfloatx2*)(pwr);
            w00 = XT_LSX2I(pWwr, 0 * sizeof(xtfloatx2));
            w11 = XT_LSX2I(pWwr, 1 * sizeof(xtfloatx2));
            y01 = XT_LSX2I(pY0, 0);
            z01 = XT_MUL_SX2(w00, y01);
            y01 = XT_SEL32_LL_SX2(y01, tmp);
            XT_MADD_SX2(z01, w11, y01);
            pZ = (xtfloatx2 *)(z + i*(N + Q - 1) + Q);
            XT_SSX2IP(z01, pZ, sizeof(xtfloatx2));
        }
        if (i == P)
        {
            tmp = XT_MOV_SX2(0.0f);
            pY0 = (const xtfloatx2 *)(y + (i-2)*Q + (Q - 2));
            pWwr = (xtfloatx2*)(pwr);
            w00 = XT_LSX2I(pWwr, 0 * sizeof(xtfloatx2));
            w11 = XT_LSX2I(pWwr, 1 * sizeof(xtfloatx2));
            y01 = XT_LSX2I(pY0, 0);
            z01 = XT_MUL_SX2(w00, y01);
            y01 = XT_SEL32_LL_SX2(y01, tmp);
            XT_MADD_SX2(z01, w11, y01);

            pY1 = (const xtfloatx2 *)(y + (i-1)*Q + (Q - 2));
            pWwr = (xtfloatx2*)(pwr + 6);
            w00 = XT_LSX2I(pWwr, 0 * sizeof(xtfloatx2));
            w11 = XT_LSX2I(pWwr, 1 * sizeof(xtfloatx2));
            y01 = XT_LSX2I(pY1, 0);
            XT_MADD_SX2(z01, w00, y01);
            y01 = XT_SEL32_LL_SX2(y01, tmp);
            XT_MADD_SX2(z01, w11, y01);

            pZ = (xtfloatx2 *)(z + i*(N + Q - 1) + Q);
            XT_SSX2IP(z01, pZ, sizeof(xtfloatx2));
        }
    }

#   undef M
#   undef N
}

size_t conv2d_3x3f_getScratchSize(int P, int Q)
{
    (void)P, (void)Q;
    return 3*6*sizeof(xtfloat) + 2*Q*sizeof(xtfloat);
} // MxN=3x3

#else 

void conv2d_3x3f      (void* pScr, float32_t *z, const float32_t * x, const float32_t * y, int P, int Q)
{
    NASSERT_ALIGN(x,HIFI_SIMD_WIDTH);
    NASSERT_ALIGN(y,HIFI_SIMD_WIDTH);
    NASSERT_ALIGN(z,HIFI_SIMD_WIDTH);
    NASSERT_ALIGN(pScr,HIFI_SIMD_WIDTH);
    NASSERT(P>=0 && P%8==0);
    NASSERT(Q>=0 && Q%8==0);
    //conv2df(pScr, z, x, y, 5, 5, P, Q);
    int i, j, m, n, n0, n1, m0, m1;
    float32_t * pwr = (float32_t*)pScr;
    xtfloat * restrict pWwr = (xtfloat*)pwr;
    const xtfloat * restrict pY;
    xtfloat * restrict pX;
    xtfloat * restrict pW;
    xtfloat * restrict pZ;
    xtfloat w0, w1, w2, y0, y1, y2, y3, y4, y5;
    if (P <= 0 || Q <= 0) return;

#define M 3
#define N 3
    pX = (xtfloat*)x;
    pW = (xtfloat*)pScr + M*N - 1;
    for (i = 0; i < M*N; i++)
    {
        XT_LSIP(w0, pX, sizeof(xtfloat));
        XT_SSXP(w0, pW, -1 * ((int)sizeof(xtfloat)));
    }

    /* first M-1 rows */
    for (i = 0; i < M - 1; i++)
    {
        for (j = 0; j < N - 1; j++)
        {
            float32_t s;
            m0 = XT_MAX(i - P + 1, 0);
            m1 = XT_MIN(i + 1, M);
            n0 = XT_MAX(j - Q + 1, 0);
            n1 = XT_MIN(j + 1, N);
            s = 0;
            for (n = n0; n < n1; n++)
            for (m = m0; m < m1; m++)
            {
                s += x[m*N + n] * y[(i - m)*Q + (j - n)];
            }
            z[i*(N + Q - 1) + j] = s;
        }
        pZ = (xtfloat *)(z + i*(N + Q - 1) + (N - 1));
        for (j = 0; j < (Q - (N - 1)) >> 2; j++)
        {
            xtfloat s0 = XT_MOV_S(0.0f);
            xtfloat s1 = XT_MOV_S(0.0f);
            xtfloat s2 = XT_MOV_S(0.0f);
            xtfloat s3 = XT_MOV_S(0.0f);
            pWwr = (xtfloat*)(pwr + ((M - 1) - i)*N);
            for (m = 0; m < i+1; m++)
            {
                pY = (const xtfloat *)(y + (m)*Q + (4 * j));
                XT_LSIP(w0, pWwr, sizeof(xtfloat));
                XT_LSIP(w1, pWwr, sizeof(xtfloat));
                XT_LSIP(w2, pWwr, sizeof(xtfloat));
                y0 = XT_LSI(pY, 0 * sizeof(xtfloat));
                y1 = XT_LSI(pY, 1 * sizeof(xtfloat));
                y2 = XT_LSI(pY, 2 * sizeof(xtfloat));
                y3 = XT_LSI(pY, 3 * sizeof(xtfloat));
                y4 = XT_LSI(pY, 4 * sizeof(xtfloat));
                y5 = XT_LSI(pY, 5 * sizeof(xtfloat));
                XT_MADD_S(s0, w0, y0);
                XT_MADD_S(s0, w1, y1);
                XT_MADD_S(s0, w2, y2);

                XT_MADD_S(s1, w0, y1);
                XT_MADD_S(s1, w1, y2);
                XT_MADD_S(s1, w2, y3);

                XT_MADD_S(s2, w0, y2);
                XT_MADD_S(s2, w1, y3);
                XT_MADD_S(s2, w2, y4);

                XT_MADD_S(s3, w0, y3);
                XT_MADD_S(s3, w1, y4);
                XT_MADD_S(s3, w2, y5);
            }
            XT_SSIP(s0, pZ, sizeof(xtfloat));
            XT_SSIP(s1, pZ, sizeof(xtfloat));
            XT_SSIP(s2, pZ, sizeof(xtfloat));
            XT_SSIP(s3, pZ, sizeof(xtfloat));
        }
        if ((Q - (N - 1)) & 2)
        {
            xtfloat s0 = XT_MOV_S(0.0f);
            xtfloat s1 = XT_MOV_S(0.0f);
            pWwr = (xtfloat*)(pwr + ((M - 1) - i)*N);
            for (m = 0; m < i+1; m++)
            {
                pY = (const xtfloat *)(y + (m)*Q + ((Q - (N - 1)) - (N - 1)));
                XT_LSIP(w0, pWwr, sizeof(xtfloat));
                XT_LSIP(w1, pWwr, sizeof(xtfloat));
                XT_LSIP(w2, pWwr, sizeof(xtfloat));
                y0 = XT_LSI(pY, 0 * sizeof(xtfloat));
                y1 = XT_LSI(pY, 1 * sizeof(xtfloat));
                y2 = XT_LSI(pY, 2 * sizeof(xtfloat));
                y3 = XT_LSI(pY, 3 * sizeof(xtfloat));
                XT_MADD_S(s0, w0, y0);
                XT_MADD_S(s0, w1, y1);
                XT_MADD_S(s0, w2, y2);

                XT_MADD_S(s1, w0, y1);
                XT_MADD_S(s1, w1, y2);
                XT_MADD_S(s1, w2, y3);
            }
            XT_SSIP(s0, pZ, sizeof(xtfloat));
            XT_SSIP(s1, pZ, sizeof(xtfloat));
        }

        for (j = Q; j < N + Q - 1; j++)
        {
            float32_t s;
            m0 = XT_MAX(i - P + 1, 0);
            m1 = XT_MIN(i + 1, M);
            n0 = XT_MAX(j - Q + 1, 0);
            n1 = XT_MIN(j + 1, N);
            s = 0;
            for (n = n0; n < n1; n++)
            for (m = m0; m < m1; m++)
            {
                s += x[m*N + n] * y[(i - m)*Q + (j - n)];
            }
            z[i*(N + Q - 1) + j] = s;
        }

    }

    for (i = M - 1; i < P; i++)
    {
        pZ = (xtfloat *)(z + i*(N + Q - 1));
        for (j = 0; j < N - 1; j++)
        {
            m0 = i - (M - 1);
            n0 = (N - 1) - j;
            xtfloat s = XT_MOV_S(0.0f);
            for (m = 0; m < M; m++)
            {
                pY = (const xtfloat *)(y + (m0 + m)*Q);
                pWwr = (xtfloat *)(pwr + m*N + n0);
                for (n = 0; n < j + 1; n++)
                {
                    XT_LSIP(w0, pWwr, sizeof(xtfloat));
                    XT_LSIP(y0, pY, sizeof(xtfloat));
                    XT_MADD_S(s, w0, y0);
                }
            }
            XT_SSIP(s, pZ, sizeof(xtfloat));
        }
        /* Main section */
        for (j = 0; j < (Q-(N-1))>>2; j++)
        {
            xtfloat s0 = XT_MOV_S(0.0f);
            xtfloat s1 = XT_MOV_S(0.0f);
            xtfloat s2 = XT_MOV_S(0.0f);
            xtfloat s3 = XT_MOV_S(0.0f);
            pWwr = (xtfloat*)pwr;
            m0 = i - (M - 1);
            for (m = 0; m < M; m++)
            {
                pY = (const xtfloat *)(y + (m0 + m)*Q + (4*j));
                XT_LSIP(w0, pWwr, sizeof(xtfloat));
                XT_LSIP(w1, pWwr, sizeof(xtfloat));
                XT_LSIP(w2, pWwr, sizeof(xtfloat));
                y0 = XT_LSI(pY, 0*sizeof(xtfloat));
                y1 = XT_LSI(pY, 1*sizeof(xtfloat));
                y2 = XT_LSI(pY, 2*sizeof(xtfloat));
                y3 = XT_LSI(pY, 3*sizeof(xtfloat));
                y4 = XT_LSI(pY, 4*sizeof(xtfloat));
                y5 = XT_LSI(pY, 5*sizeof(xtfloat));
                XT_MADD_S(s0, w0, y0);
                XT_MADD_S(s0, w1, y1);
                XT_MADD_S(s0, w2, y2);

                XT_MADD_S(s1, w0, y1);
                XT_MADD_S(s1, w1, y2);
                XT_MADD_S(s1, w2, y3);

                XT_MADD_S(s2, w0, y2);
                XT_MADD_S(s2, w1, y3);
                XT_MADD_S(s2, w2, y4);

                XT_MADD_S(s3, w0, y3);
                XT_MADD_S(s3, w1, y4);
                XT_MADD_S(s3, w2, y5);
            }
            XT_SSIP(s0, pZ, sizeof(xtfloat));
            XT_SSIP(s1, pZ, sizeof(xtfloat));
            XT_SSIP(s2, pZ, sizeof(xtfloat));
            XT_SSIP(s3, pZ, sizeof(xtfloat));
        }
        if ((Q - (N-1))&2)
        {
            xtfloat s0 = XT_MOV_S(0.0f);
            xtfloat s1 = XT_MOV_S(0.0f);
            pWwr = (xtfloat*)pwr;
            m0 = i - (M - 1);
            for (m = 0; m < M; m++)
            {
                pY = (const xtfloat *)(y + (m0 + m)*Q + ((Q-(N-1)) - (N - 1)));
                XT_LSIP(w0, pWwr, sizeof(xtfloat));
                XT_LSIP(w1, pWwr, sizeof(xtfloat));
                XT_LSIP(w2, pWwr, sizeof(xtfloat));
                y0 = XT_LSI(pY, 0 * sizeof(xtfloat));
                y1 = XT_LSI(pY, 1 * sizeof(xtfloat));
                y2 = XT_LSI(pY, 2 * sizeof(xtfloat));
                y3 = XT_LSI(pY, 3 * sizeof(xtfloat));
                XT_MADD_S(s0, w0, y0);
                XT_MADD_S(s0, w1, y1);
                XT_MADD_S(s0, w2, y2);

                XT_MADD_S(s1, w0, y1);
                XT_MADD_S(s1, w1, y2);
                XT_MADD_S(s1, w2, y3);
            }
            XT_SSIP(s0, pZ, sizeof(xtfloat));
            XT_SSIP(s1, pZ, sizeof(xtfloat));
        }

        for (j = Q; j < N + Q - 1; j++)
        {
            xtfloat s = XT_MOV_S(0.0f);
            n1 = N + Q - j - 1;
            m0 = i - (M - 1);
            n0 = j - (N - 1);
            for (m = 0; m < M; m++)
            {
                pWwr = (xtfloat *)(pwr + m*N);
                pY = (const xtfloat *)(y + (m0 + m)*Q + n0);
                for (n = 0; n < n1; n++)
                {
                    XT_LSIP(w0, pWwr, sizeof(xtfloat));
                    XT_LSIP(y0, pY, sizeof(xtfloat));
                    XT_MADD_S(s, w0, y0);
                }
            }
            XT_SSIP(s, pZ, sizeof(xtfloat));
        }
    }

    /* Last M-1 rows */
    for (i = P; i < M + P - 1; i++)
    {
        for (j = 0; j < N - 1; j++)
        {
            float32_t s;
            m0 = XT_MAX(i - P + 1, 0);
            m1 = XT_MIN(i + 1, M);
            n0 = XT_MAX(j - Q + 1, 0);
            n1 = XT_MIN(j + 1, N);
            s = 0;
            for (n = n0; n < n1; n++)
            for (m = m0; m < m1; m++)
            {
                s += x[m*N + n] * y[(i - m)*Q + (j - n)];
            }
            z[i*(N + Q - 1) + j] = s;
        }

        pZ = (xtfloat *)(z + i*(N + Q - 1) + (N - 1));

        for (j = 0; j < (Q - (N - 1)) >> 2; j++)
        {
            xtfloat s0 = XT_MOV_S(0.0f);
            xtfloat s1 = XT_MOV_S(0.0f);
            xtfloat s2 = XT_MOV_S(0.0f);
            xtfloat s3 = XT_MOV_S(0.0f);
            pWwr = (xtfloat*)pwr;
            m0 = i - (M - 1);
            m1 = (M + P - i - 1);
            for (m = 0; m < m1; m++)
            {
                pY = (const xtfloat *)(y + (m0 + m)*Q + (4 * j));
                XT_LSIP(w0, pWwr, sizeof(xtfloat));
                XT_LSIP(w1, pWwr, sizeof(xtfloat));
                XT_LSIP(w2, pWwr, sizeof(xtfloat));
                y0 = XT_LSI(pY, 0 * sizeof(xtfloat));
                y1 = XT_LSI(pY, 1 * sizeof(xtfloat));
                y2 = XT_LSI(pY, 2 * sizeof(xtfloat));
                y3 = XT_LSI(pY, 3 * sizeof(xtfloat));
                y4 = XT_LSI(pY, 4 * sizeof(xtfloat));
                y5 = XT_LSI(pY, 5 * sizeof(xtfloat));
                XT_MADD_S(s0, w0, y0);
                XT_MADD_S(s0, w1, y1);
                XT_MADD_S(s0, w2, y2);

                XT_MADD_S(s1, w0, y1);
                XT_MADD_S(s1, w1, y2);
                XT_MADD_S(s1, w2, y3);

                XT_MADD_S(s2, w0, y2);
                XT_MADD_S(s2, w1, y3);
                XT_MADD_S(s2, w2, y4);

                XT_MADD_S(s3, w0, y3);
                XT_MADD_S(s3, w1, y4);
                XT_MADD_S(s3, w2, y5);
            }
            XT_SSIP(s0, pZ, sizeof(xtfloat));
            XT_SSIP(s1, pZ, sizeof(xtfloat));
            XT_SSIP(s2, pZ, sizeof(xtfloat));
            XT_SSIP(s3, pZ, sizeof(xtfloat));
        }
        if ((Q - (N - 1)) & 2)
        {
            xtfloat s0 = XT_MOV_S(0.0f);
            xtfloat s1 = XT_MOV_S(0.0f);
            pWwr = (xtfloat*)pwr;
            m0 = i - (M - 1);
            m1 = (M + P - i - 1);
            for (m = 0; m < m1; m++)
            {
                pY = (const xtfloat *)(y + (m0 + m)*Q + ((Q - (N - 1)) - (N - 1)));
                XT_LSIP(w0, pWwr, sizeof(xtfloat));
                XT_LSIP(w1, pWwr, sizeof(xtfloat));
                XT_LSIP(w2, pWwr, sizeof(xtfloat));
                y0 = XT_LSI(pY, 0 * sizeof(xtfloat));
                y1 = XT_LSI(pY, 1 * sizeof(xtfloat));
                y2 = XT_LSI(pY, 2 * sizeof(xtfloat));
                y3 = XT_LSI(pY, 3 * sizeof(xtfloat));
                XT_MADD_S(s0, w0, y0);
                XT_MADD_S(s0, w1, y1);
                XT_MADD_S(s0, w2, y2);

                XT_MADD_S(s1, w0, y1);
                XT_MADD_S(s1, w1, y2);
                XT_MADD_S(s1, w2, y3);
            }
            XT_SSIP(s0, pZ, sizeof(xtfloat));
            XT_SSIP(s1, pZ, sizeof(xtfloat));
        }

        for (j = Q; j < N + Q - 1; j++)
        {
            float32_t s;
            m0 = XT_MAX(i - P + 1, 0);
            m1 = XT_MIN(i + 1, M);
            n0 = XT_MAX(j - Q + 1, 0);
            n1 = XT_MIN(j + 1, N);
            s = 0;
            for (n = n0; n < n1; n++)
            for (m = m0; m < m1; m++)
            {
                s += x[m*N + n] * y[(i - m)*Q + (j - n)];
            }
            z[i*(N + Q - 1) + j] = s;
        }
    }
#undef M
#undef N
}

size_t conv2d_3x3f_getScratchSize(int P, int Q)
{ 
    return 3 * 3 * sizeof(xtfloat);
} // MxN=3x3

#endif
