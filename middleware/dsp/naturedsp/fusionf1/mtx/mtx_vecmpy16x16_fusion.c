/* ------------------------------------------------------------------------ */
/* Copyright (c) 2016 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (“Cadence    */
/* Libraries”) are the copyrighted works of Cadence Design Systems Inc.	    */
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
/*          Copyright (C) 2015-2016 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
#include "NatureDSP_Signal.h"
#include "NatureDSP_types.h"
#include "common.h"

/*===========================================================================
  Matrix Operations:
  mtx_mpy              Matrix Multiply
  mtx_vecmpy           Matrix by Vector Multiply
===========================================================================*/
/*-------------------------------------------------------------------------
  Matrix by Vector Multiply
  These functions compute the expression z = 2^lsh * x * y for the matrices 
  x and vector y. 

  Two versions of functions available: regular version (mtx_vecmpy32x32, 
  mtx_vecmpy24x24, mtx_vecmpy16x16,mtx_vecmpyf) with arbitrary arguments 
  and faster version (mtx_vecmpy32x32_fast, mtx_vecmpy24x24_fast, 
  mtx_vecmpy16x16_fast, mtx_vecmpyf_fast) that apply some restrictions.

  Precision: 
  32x32 32-bit inputs, 32-bit output
  24x24 24-bit inputs, 24-bit output
  16x16 16-bit inputs, 16-bit output
  f     floating point

  Input:
  x[M*N] input matrix,Q31,Q15 or floating point
  y[N]   input vector,Q31,Q15 or floating point
  M      number of rows in matrix x
  N      number of columns in matrix x
  lsh    additional left shift
  Output:
  z[M]   output vector,Q31,Q15 or floating point

  Restriction:
  For regular routines (mtx_vecmpy24x24, mtx_vecmpy16x16, mtx_vecmpyf)
  x,y,z should not overlap

  For faster routines (mtx_vecmpy24x24_fast, mtx_vecmpy16x16_fast,
  mtx_vecmpyf_fast)
  x,y,z should not overlap
  x,y   aligned on 8-byte boundary
  N and M are multiples of 4
-------------------------------------------------------------------------*/
void mtx_vecmpy16x16 (  int16_t* restrict z,
               const int16_t* restrict x,
               const int16_t* restrict y,
               int M, int N, int lsh)
#if !(XCHAL_HAVE_FUSION_16BIT_BASEBAND)
{
    /* code with no Quad MAC option */
  const ae_int16x4      *  restrict px = (const ae_int16x4     *)x;
  const ae_int16x4      *  restrict py = (const ae_int16x4     *)y;
        ae_int16        *  restrict pz = (      ae_int16       *)z;
  
  ae_valign x_align,y_align;
  int m,n;
  ae_int32x2  zz;
  if (N<0)    /* exceptional situation */
  {
        for (m=0; m<M; m++) z[m]=0;
        return ;
  }

  zz = AE_MOVI(0);
  #ifdef COMPILER_XTENSA
    #pragma concurrent
  #endif
  for(m=0; m<M; m++)
  {
    ae_int32x2    vah,vbh;
    ae_int64 B0;
    ae_int16x4 A0;
    
   x_align = AE_LA64_PP(px);
   py = (const ae_int16x4   *)y;
   y_align = AE_LA64_PP(py);
    B0 = AE_ZERO64();
    for(n=0; n<N-3; n+=4)
    {
      ae_int16x4 vxh,vyh;
      ae_int32x2 x1,x2;

      AE_LA16X4_IP(vxh, x_align, px); 
      AE_LA16X4_IP(vyh, y_align, py);
      x1 = AE_SEXT32X2D16_32(vxh);
      x2 = AE_SEXT32X2D16_10(vxh);
      AE_MULAAD32X16_H3_L2(B0,x1,vyh);
      AE_MULAAD32X16_H1_L0(B0,x2,vyh);
    }

    {
      ae_int16x4 t1,t2,vxh,vyh;
      ae_int32x2  veh,vfh;
      ae_int32x2 x1,x2;
        switch(N&3)
        {
        case 1:
          AE_L16_IP(vxh, castxcc(ae_int16,px), 2); 
          AE_L16_IP(vyh, castxcc(ae_int16,py), 2);
          x1 = AE_SEXT32X2D16_32(vxh);
          AE_MULA32X16_L0(B0,x1,vyh);
          break;
        case 2:
          AE_L16_IP(vxh, castxcc(ae_int16,px), 2); 
          AE_L16_IP(t1, castxcc(ae_int16,px), 2); 
          x1 = AE_SEXT32X2D16_32(vxh);
          x2 = AE_SEXT32X2D16_32(t1);
          AE_LA16X4_IP(vyh, y_align, py);
          AE_MULA32X16_L3(B0,x1,vyh);
          AE_MULA32X16_L2(B0,x2,vyh);
          break;
        case 3:
          AE_L16_IP(vxh,castxcc(ae_int16,px), 2); 
          AE_L16_IP(t1, castxcc(ae_int16,px), 2); 
          AE_L16_IP(t2, castxcc(ae_int16,px), 2); 
          veh = AE_SEXT32X2D16_32(vxh);
          vfh = AE_SEXT32X2D16_32(t1); 
          x2 = AE_SEXT32X2D16_32(t2); 
          x1 = AE_SEL32_HH(veh,vfh);
          AE_LA16X4_IP(vyh, y_align, py);
          AE_MULAAD32X16_H3_L2(B0,x1,vyh);
          AE_MULA32X16_L1(B0,x2,vyh);
          break;
        default: break;
        }
    }
    B0 = AE_SLAI64(B0,1);
    vah = AE_TRUNCA32F64S(B0,32);
    vbh = AE_SLAA32S(vah,lsh);
    vbh = AE_SRAI32(vbh,16);
    A0 = AE_MOVINT16X4_FROMINT32X2(vbh);

    AE_S16_0_IP(A0,pz,2);
  }
}
#else
{
    /* code with Quad MAC option */
    int m,n;
    xtbool4 mask;
    const ae_int16x4      *  restrict px0;
    const ae_int16x4      *  restrict px1;
    const ae_int16x4      *  restrict py;
          ae_int16        *  restrict pz = (      ae_int16       *)z;
    ae_int32x2 a0;
    ae_int16x4 t,x0,x1,y0;
    ae_int64 B0,B1;
    ae_valign ax0,ax1,ay;

    if (N<=0 || M<=0)    /* exceptional situation */
    {
        for (m=0; m<M; m++) z[m]=0;
        return ;
    }
    /* mask for last 1...3 values from y */
    mask=(1<<(4-(N&3)))-1;
    /* process by pair of rows */
    for(m=0; m<(M&~1); m+=2)
    {
        B0=B1=AE_ZERO64();
        px0=(const ae_int16x4 *)x;
        px1=(const ae_int16x4 *)(x+N);
        py =(const ae_int16x4 *)y;
        ax0=AE_LA64_PP(px0);
        ax1=AE_LA64_PP(px1);
        ay =AE_LA64_PP(py);
        for (n=0; n<(N&~3); n+=4)
        {
            AE_LA16X4_IP(x0,ax0,px0);
            AE_LA16X4_IP(x1,ax1,px1);
            AE_LA16X4_IP(y0,ay ,py );
            AE_MULAAAAQ16(B0,x0,y0);
            AE_MULAAAAQ16(B1,x1,y0);
        }
        if(N&3)
        {
            AE_LA16X4_IP(x0,ax0,px0);
            AE_LA16X4_IP(x1,ax1,px1);
            AE_LA16X4_IP(y0,ay ,py );
            AE_MOVT16X4(y0,AE_ZERO16(),mask);
            AE_MULAAAAQ16(B0,x0,y0);
            AE_MULAAAAQ16(B1,x1,y0);
        }
        a0=AE_TRUNCA32X2F64S(B0,B1,lsh+33);
        t=AE_ROUND16X4F32SASYM(a0,a0);
        AE_S16_0_IP(AE_SHORTSWAP(t),pz,2);
        AE_S16_0_IP(t,pz,2);
        x+=N*2; /* next 2 rows */
    }
    if(M&1)
    {
        B0=AE_ZERO64();
        px0=(const ae_int16x4 *)x;
        py =(const ae_int16x4 *)y;
        ax0=AE_LA64_PP(px0);
        ay =AE_LA64_PP(py);
        for (n=0; n<(N&~3); n+=4)
        {
            AE_LA16X4_IP(x0,ax0,px0);
            AE_LA16X4_IP(y0,ay ,py );
            AE_MULAAAAQ16(B0,x0,y0);
        }
        if(N&3)
        {
            AE_LA16X4_IP(x0,ax0,px0);
            AE_LA16X4_IP(y0,ay ,py );
            AE_MOVT16X4(y0,AE_ZERO16(),mask);
            AE_MULAAAAQ16(B0,x0,y0);
        }
        a0=AE_TRUNCA32X2F64S(B0,B0,lsh+33);
        t=AE_ROUND16X4F32SASYM(a0,a0);
        AE_S16_0_IP(t,pz,2);
    }
} /* mtx_vecmpy16x16() */
#endif
