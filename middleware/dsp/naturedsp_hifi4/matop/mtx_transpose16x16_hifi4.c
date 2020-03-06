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
void mtx_transpose16x16 (int16_t*    y, const int16_t*     x, int M, int N)
{
    xtbool4 b0=AE_MOVBA4(0x3);
    xtbool4 b1=AE_MOVBA4(0x5);
    ae_int16x4 * restrict pY;
    ae_valign aY,aX;
    const ae_int16 * restrict pX;
    ae_int16x4 x0,x1,x2,x3;
    int m, n;
    NASSERT( x );
    NASSERT( y );
    NASSERT(x != y);
    if (M<=0 || N<=0) return;
    if (M>=4)
    {
        for ( n=0; n<N; n++ )    
        {    
            pY=(ae_int16x4 *)(y+n*M);
            pX=(ae_int16*)(x+n);
            aY=AE_ZALIGN64();
            __Pragma("loop_count min=1")
            for ( m=0; m<(M>>2); m++ )  
            {      
                AE_L16_XP(x0,pX,N*sizeof(int16_t));
                AE_L16_XP(x1,pX,N*sizeof(int16_t));
                AE_L16_XP(x2,pX,N*sizeof(int16_t));
                AE_L16_XP(x3,pX,N*sizeof(int16_t));
                AE_MOVT16X4(x0,x2,b0); /* 0 0 2 2 */
                AE_MOVT16X4(x1,x3,b0); /* 1 1 3 3 */
                AE_MOVT16X4(x0,x1,b1); /* 0 1 2 3 */
                AE_SA16X4_IP(x0,aY,pY);
            }  
            AE_SA64POS_FP(aY,pY);
        }
    }
    if (M&3)
    {
        if (N>=4)
        {
            for ( m=(M&~3); m<M; m++ )  
            {
                pX=(ae_int16*)(x+m*N);
                pY=(ae_int16x4*)(y+m);
                aX=AE_LA64_PP(pX);
                for ( n=0; n<(N&~3); n+=4)
                {
                    AE_LA16X4_IP(x0,aX,castxcc(ae_int16x4,pX));
                    AE_S16_0_XP(AE_SEL16_6543(x0,x0),castxcc(ae_int16,pY),M*sizeof(int16_t));
                    AE_S16_0_XP(AE_SEL16_7632(x0,x0),castxcc(ae_int16,pY),M*sizeof(int16_t));
                    AE_S16_0_XP(AE_SEL16_4321(x0,x0),castxcc(ae_int16,pY),M*sizeof(int16_t));
                    AE_S16_0_XP(x0,castxcc(ae_int16,pY),M*sizeof(int16_t));
                }
            }
        }
        /* very small remainder - 3x3 or smaller */
        x+=(N&~3)  +(M&~3)*N;
        y+=(N&~3)*M+(M&~3)  ;
        pX=(const ae_int16*)x;
        pY=(      ae_int16x4*)y;
        switch(N&3)
        {
        case 1: 
            for ( m=0; m<(M&3); m++ )  
            {
                AE_L16_XP(x0,pX,N*sizeof(int16_t));
                AE_S16_0_IP(x0,castxcc(ae_int16,pY),sizeof(int16_t));
            }
            break;
        case 2:
            for ( m=0; m<(M&3); m++ )  
            {
                x1=AE_L16_I (pX,  sizeof(int16_t));
                AE_L16_XP(x0,pX,N*sizeof(int16_t));
                AE_S16_0_X (x1,(ae_int16*)pY,M*sizeof(int16_t));
                AE_S16_0_IP(x0,castxcc(ae_int16,pY),  sizeof(int16_t));
            }
            break;
        case 3:
            for ( m=0; m<(M&3); m++ )  
            {
                x1=AE_L16_I (pX,  sizeof(int16_t));
                x2=AE_L16_I (pX,2*sizeof(int16_t));
                AE_L16_XP(x0,pX,N*sizeof(int16_t));
                AE_S16_0_X (x1,(ae_int16*)pY,  M*sizeof(int16_t));
                AE_S16_0_X (x2,(ae_int16*)pY,2*M*sizeof(int16_t));
                AE_S16_0_IP(x0,castxcc(ae_int16,pY),  sizeof(int16_t));
            }
            break;
        default : break;
        }
    }
}
