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
   Matrix transpose
 * Reference code
 */

/* Cross-platform data type definitions. */
#include "NatureDSP_types.h"
/* Library API */
#include "NatureDSP_Signal_matop.h"
/* Common helper macros. */
#include "common.h"

/*-------------------------------------------------------------------------
  Matrix Transpose
  These functions transpose matrices.

  Precision: 
  32x32 32-bit input, 32-bit output
  16x16 16-bit input, 16-bit output
  8x8   8-bit inputs, 8-bit output
  f     floating point

  Input:
  x[M][N] input matrix,Q31,Q15,Q7 or floating point
  M       number of rows in matrix x
  N       number of columns in matrix x
  Output:
  y[N][M] output vector,Q31,Q15,Q7 or floating point

  Restriction:
  For regular routines (mtx_transpose_32x32, mtx_transpose_16x16, 
  mtx_transpose_8x8, mtx_transposef):
  x,y should not overlap

  For faster routines (mtx_transpose 32x32_fast, mtx_transpose 16x16_fast, 
  mtx_transpose_8x8_fast, mtx_transposef_fast)
  x,y   should not overlap
  x,y   aligned on 8-byte boundary
  N and M are multiples of 4
-------------------------------------------------------------------------*/
void mtx_transpose8x8_fast   (int8_t   *  y, const int8_t *     x, int M, int N)
{
    ae_f16x4 x0,x1,x2,x3;
    const int8_t* restrict pX0;
    const int8_t* restrict pX1;
    const int8_t* restrict pX2;
    const int8_t* restrict pX3;
          ae_int32* restrict pY;
    int m, n;
    xtbool4 b=AE_MOVAB4(5);
    NASSERT( x );
    NASSERT( y );
    NASSERT(x != y);
    NASSERT(N%4==0 && M%4==0);
    NASSERT_ALIGN(x,XCHAL_DATA_WIDTH);
    NASSERT_ALIGN(y,XCHAL_DATA_WIDTH);
    if (N<=0 || M<=0) return;
    pX0=x+0*N;
    pX1=x+1*N;
    pX2=x+2*N;
    pX3=x+3*N;
    for ( m=0; m<M; m+=4 )  
    {    
        pY=(ae_int32*)(y+m);
        for ( n=0; n<N; n+=4 )    
        {      
            /* first, load 4x4 submatrix with 8-bit zero padding in between bytes,
                next, combine adjucent vectors via OR
                at the final, interleave them by select and movt instructions 
            */
            AE_L8X4F_IP(x0,pX0,4);
            AE_L8X4F_IP(x1,pX1,4);
            AE_L8X4F_IP(x2,pX2,4);
            AE_L8X4F_IP(x3,pX3,4);

            x0=AE_OR16(x1,AE_MOVINT16X4_FROMINT64(AE_SRLI64(AE_MOVINT64_FROMINT16X4(x0),8)));
            x2=AE_OR16(x3,AE_MOVINT16X4_FROMINT64(AE_SRLI64(AE_MOVINT64_FROMINT16X4(x2),8)));
            x1=x2; AE_MOVT16X4(x1,AE_SEL16_2301(x0,x0),b);
            x3=x0; AE_MOVF16X4(x3,AE_SEL16_2301(x2,x2),b);

            AE_S32_L_X(AE_MOVINT32X2_FROMINT16X4(AE_SEL16_7632(x3,x3)) ,pY,1*M);
            AE_S32_L_X(AE_MOVINT32X2_FROMINT16X4(x1)                   ,pY,2*M);
            AE_S32_L_X(AE_MOVINT32X2_FROMINT16X4(x3)                   ,pY,3*M);
            AE_S32_L_XP(AE_MOVINT32X2_FROMINT16X4(AE_SEL16_7632(x1,x1)),pY,4*M);
        }  
        pX0+=3*N;
        pX1+=3*N;
        pX2+=3*N;
        pX3+=3*N;
    }
}
