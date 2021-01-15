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

#include "NatureDSP_Signal_matop.h"
#include "NatureDSP_types.h"
#include "common.h"

/*-------------------------------------------------------------------------
  Matrix by Vector Multiply
  These functions compute the expression z = 2^lsh * x * y for the matrices 
  x and vector y. 
  NOTE: lsh factor is not relevant for floating point routines.

  Two versions of functions available: regular version (mtx_vecmpy32x32,  
  mtx_vecmpy16x16, mtx_vecmpy8x8, mtx_vecmpy8x16, mtx_vecmpyf) with arbitrary 
  arguments and faster version (mtx_vecmpy32x32_fast, mtx_vecmpy16x16_fast, 
  mtx_vecmpy8x8_fast, mtx_vecmpy8x16_fast,  mtx_vecmpyf_fast) that apply 
  some restrictions

  Precision: 
  32x32 32-bit input, 32-bit output
  16x16 16-bit input, 16-bit output
  8x8   8-bit inputs, 8-bit output
  8x16  8/16-bit inputs, 16-bit output
  f     floating point

  Input:
  x[M*N] input matrix,Q31,Q15 or floating point
  y[N]   input vector,Q31,Q15 or floating point
  M      number of rows in matrix x
  N      number of columns in matrix x
  lsh    additional left shift(applied to the fixed-
         point functions only) 
  Output:
  z[M]   output vector,Q31,Q15 or floating point

  Restriction:
  For regular routines (mtx_vecmpy32x32, mtx_vecmpy16x16, mtx_vecmpy8x8,
  mtx_vecmpy8x16,  mtx_vecmpyf)
  x,y,z should not overlap

  For faster routines  (mtx_vecmpy32x32_fast, mtx_vecmpy16x16_fast, 
  mtx_vecmpy8x8_fast, mtx_vecmpy8x16_fast, mtx_vecmpyf_fast)
  x,y,z   should not overlap
  x,y     aligned on 8-byte boundary
  N, M    multiples of 4
  lsh     should be in range:
          -31...31 for mtx_vecmpy32x32, mtx_vecmpy32x32_fast
          -15...15 for mtx_vecmpy16x16, mtx_vecmpy16x16_fast, 
                   mtx_vecmpy8x8_fast, mtx_vecmpy8x16_fast   
-------------------------------------------------------------------------*/
void mtx_vecmpy16x16 (  int16_t* restrict z,
               const int16_t* restrict x,
               const int16_t* restrict y,
               int M, int N, int lsh)
{
    ae_valign ax0,ax1,ax2,ax3;
    const ae_int16x4 * restrict px0;
    const ae_int16x4 * restrict px1;
    const ae_int16x4 * restrict px2;
    const ae_int16x4 * restrict px3;
    const ae_int16*    restrict px4;
    const ae_int16   * restrict py;
    ae_int16x4 X0,X1,X2,X3,Y0,Y1,Y2,Y3;
    ae_int32x2 Z0,Z1;
    ae_int64 B0,B1,B2,B3;
    xtbool4 bmask;
    int m,n;
    if (N<=0 || M<=0)    /* exceptional situation */
    {
        for (m=0; m<M; m++) z[m]=0;
        return ;
    }
    py=(const ae_int16 *)y;
    px0=(const ae_int16x4 *)(x);
    px1=(const ae_int16x4 *)(x+N);
    px2=(const ae_int16x4 *)(x+2*N);
    px3=(const ae_int16x4 *)(x+3*N);
    if (M>=4)
    {

        if(N&4)
        {   // optimized code for low-bit ordering for N&8==4,5,6,7
            __Pragma("loop_count min=1")
            for(m=0; m<(M>>2); m++)
            {
                ae_valign ay;
                py=(const ae_int16 *)y;
                px0=(const ae_int16x4 *)(x);
                px1=(const ae_int16x4 *)(x+N  );
                px2=(const ae_int16x4 *)(x+2*N);
                px3=(const ae_int16x4 *)(x+3*N);
                ax1=AE_LA64_PP(px1);
                ax2=AE_LA64_PP(px2);
                ax3=AE_LA64_PP(px3);
                B0=B1=B2=B3=AE_ZERO64();

                ay=AE_LA64_PP(py);
                px4=(const ae_int16*)px0;
                __Pragma("ymemory(px4)")
                __Pragma("loop_count min=1")
                for(n=0; n<(N-3); n+=4)
                {
                    X2=AE_L16_I( px4,2);
                    X1=AE_L16_I( px4,4);
                    X0=AE_L16_I( px4,6);
                    AE_L16_IP(X3,px4,8);
                    X0=AE_SEL16_6420(X1,X0);
                    X2=AE_SEL16_6420(X3,X2);
                    X0=AE_SEL16_6420(X2,X0);
                    AE_LA16X4_IP(Y0,ay ,castxcc(ae_int16x4,py));
                    AE_LA16X4_IP(X1,ax1,px1);
                    AE_LA16X4_IP(X2,ax2,px2);
                    AE_LA16X4_IP(X3,ax3,px3);
                    AE_MULAAAAQ16(B0,Y0,X0);
                    AE_MULAAAAQ16(B1,Y0,X1);
                    AE_MULAAAAQ16(B2,Y0,X2);
                    AE_MULAAAAQ16(B3,Y0,X3);
                }
                bmask=AE_MOVBA4(0xF0>>(N&3));
                AE_LA16X4_IP(Y0,ay ,castxcc(ae_int16x4,py));
                AE_MOVF16X4(Y0,AE_ZERO16(),bmask);
                X2=AE_L16_I( px4,2);
                X1=AE_L16_I( px4,4);
                X0=AE_L16_I( px4,6);
                AE_L16_IP(X3,px4,8);
                X0=AE_SEL16_6420(X1,X0);
                X2=AE_SEL16_6420(X3,X2);
                X0=AE_SEL16_6420(X2,X0);
                AE_LA16X4_IP(X1,ax1,px1);
                AE_LA16X4_IP(X2,ax2,px2);
                AE_LA16X4_IP(X3,ax3,px3);
                AE_MULAAAAQ16(B0,Y0,X0);
                AE_MULAAAAQ16(B1,Y0,X1);
                AE_MULAAAAQ16(B2,Y0,X2);
                AE_MULAAAAQ16(B3,Y0,X3);
                Z0=AE_TRUNCA32X2F64S(B0,B1,lsh+33);
                Z1=AE_TRUNCA32X2F64S(B2,B3,lsh+33);
                Y0=AE_ROUND16X4F32SASYM(Z0,Z1);
                z[0]=AE_MOVAD16_3(Y0);
                z[1]=AE_MOVAD16_2(Y0);
                z[2]=AE_MOVAD16_1(Y0);
                z[3]=AE_MOVAD16_0(Y0);
                z+=4;
                x =(const int16_t  *)XT_ADDX2(4*N,(int)(uintptr_t)x);
                //x +=4*N;
            }

        }
        else
        {
            // optimized code for low-bit ordering for N&8==0,1,2,3
            __Pragma("loop_count min=1")
            for(m=0; m<(M>>2); m++)
            {
                py=(const ae_int16 *)y;
                px0=(const ae_int16x4 *)(x);
                px1=(const ae_int16x4 *)(x+N  );
                px2=(const ae_int16x4 *)(x+2*N);
                px3=(const ae_int16x4 *)(x+3*N);
                ax0=AE_LA64_PP(px0);
                ax1=AE_LA64_PP(px1);
                ax2=AE_LA64_PP(px2);
                ax3=AE_LA64_PP(px3);
                B0=B1=B2=B3=AE_ZERO64();
                __Pragma("ymemory(py)")
                for(n=0; n<(N-3); n+=4)
                {
                    Y2=AE_L16_I( py,2);
                    Y1=AE_L16_I( py,4);
                    Y0=AE_L16_I( py,6);
                    AE_L16_IP(Y3,py,8);
                    Y0=AE_SEL16_6420(Y1,Y0);
                    Y2=AE_SEL16_6420(Y3,Y2);
                    Y0=AE_SEL16_6420(Y2,Y0);
                    AE_LA16X4_IP(X0,ax0,px0);
                    AE_LA16X4_IP(X1,ax1,px1);
                    AE_LA16X4_IP(X2,ax2,px2);
                    AE_LA16X4_IP(X3,ax3,px3);
                    AE_MULAAAAQ16(B0,Y0,X0);
                    AE_MULAAAAQ16(B1,Y0,X1);
                    AE_MULAAAAQ16(B2,Y0,X2);
                    AE_MULAAAAQ16(B3,Y0,X3);
                }
                bmask=AE_MOVBA4(0xF0>>(N&3));
                Y2=AE_L16_I( py,2);
                Y1=AE_L16_I( py,4);
                Y0=AE_L16_I( py,6);
                AE_L16_IP(Y3,py,8);
                Y0=AE_SEL16_6420(Y1,Y0);
                Y2=AE_SEL16_6420(Y3,Y2);
                Y0=AE_SEL16_6420(Y2,Y0);
                AE_MOVF16X4(Y0,AE_ZERO16(),bmask);
                AE_LA16X4_IP(X0,ax0,px0);
                AE_LA16X4_IP(X1,ax1,px1);
                AE_LA16X4_IP(X2,ax2,px2);
                AE_LA16X4_IP(X3,ax3,px3);
                AE_MULAAAAQ16(B0,Y0,X0);
                AE_MULAAAAQ16(B1,Y0,X1);
                AE_MULAAAAQ16(B2,Y0,X2);
                AE_MULAAAAQ16(B3,Y0,X3);
                Z0=AE_TRUNCA32X2F64S(B0,B1,lsh+33);
                Z1=AE_TRUNCA32X2F64S(B2,B3,lsh+33);
                Y0=AE_ROUND16X4F32SASYM(Z0,Z1);
                z[0]=AE_MOVAD16_3(Y0);
                z[1]=AE_MOVAD16_2(Y0);
                z[2]=AE_MOVAD16_1(Y0);
                z[3]=AE_MOVAD16_0(Y0);
                z+=4;
                x =(const int16_t  *)XT_ADDX2(4*N,(int)(uintptr_t)x);
                //x +=4*N;
            }
        }

    }
    // tail: last up to 3 iteration by m
    M&=3;
    if(M)
    {
        py=(const ae_int16 *)y;
        px1=px2=px3=px0=(const ae_int16x4 *)(x);
        if(M>1) px1=(const ae_int16x4 *)(x+N);
        if(M>2) px2=(const ae_int16x4 *)(x+2*N);
        ax0=AE_LA64_PP(px0);
        ax1=AE_LA64_PP(px1);
        ax2=AE_LA64_PP(px2);
        B0=B1=B2=B3=AE_ZERO64();
        //__Pragma("loop_count min=1")
        for(n=0; n<(N-3); n+=4)
        {
            Y2=AE_L16_I( py,2);
            Y1=AE_L16_I( py,4);
            Y0=AE_L16_I( py,6);
            AE_L16_IP(Y3,py,8);
            Y0=AE_SEL16_6420(Y1,Y0);
            Y2=AE_SEL16_6420(Y3,Y2);
            Y0=AE_SEL16_6420(Y2,Y0);
            AE_LA16X4_IP(X0,ax0,px0);
            AE_LA16X4_IP(X1,ax1,px1);
            AE_LA16X4_IP(X2,ax2,px2);
            AE_MULAAAAQ16(B0,Y0,X0);
            AE_MULAAAAQ16(B1,Y0,X1);
            AE_MULAAAAQ16(B2,Y0,X2);
        }
        bmask=AE_MOVBA4(0xF0>>(N&3));
        Y2=AE_L16_I( py,2);
        Y1=AE_L16_I( py,4);
        Y0=AE_L16_I( py,6);
        AE_L16_IP(Y3,py,8);
        Y0=AE_SEL16_6420(Y1,Y0);
        Y2=AE_SEL16_6420(Y3,Y2);
        Y0=AE_SEL16_6420(Y2,Y0);
        AE_MOVF16X4(Y0,AE_ZERO16(),bmask);

        AE_LA16X4_IP(X0,ax0,px0);
        AE_LA16X4_IP(X1,ax1,px1);
        AE_LA16X4_IP(X2,ax2,px2);
        AE_MULAAAAQ16(B0,Y0,X0);
        AE_MULAAAAQ16(B1,Y0,X1);
        AE_MULAAAAQ16(B2,Y0,X2);

        Z0=AE_TRUNCA32X2F64S(B0,B1,lsh+33);
        Z1=AE_TRUNCA32X2F64S(B2,B3,lsh+33);
        Y0=AE_ROUND16X4F32SASYM(Z0,Z1);
        z[0]=AE_MOVAD16_3(Y0);
        if(M>1) z[1]=AE_MOVAD16_2(Y0);
        if(M>2) z[2]=AE_MOVAD16_1(Y0);
    }
}
