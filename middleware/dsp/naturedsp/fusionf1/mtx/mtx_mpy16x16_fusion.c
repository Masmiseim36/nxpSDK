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
  These functions compute the expression z = 2^lsh * x * y for the matrices 
  x and y. The columnar dimension of x must match the row dimension of y. 
  The resulting matrix has the same number of rows as x and the same number 
  of columns as y.

  NOTES:
  In the fixed-point routines, rows of matrices z and y may be stored in 
  non consecutive manner. Matrix x will have all the elements in contiguous 
  memory locations.

  Functions require scratch memory for storing intermediate data. This 
  scratch memory area should be aligned on 8 byte boundary and its size 
  is calculated by macros SCRATCH_MTX_MPY32X32(M,N,P), 
  SCRATCH_MTX_MPY24X24(M,N,P), SCRATCH_MTX_MPY16X16(M,N,P)

  Two versions of functions available: regular version (mtx_mpy32x32, 
  mtx_mpy24x24, mtx_mpy16x16, mtx_mpyf) with arbitrary arguments and 
  faster version (mtx_mpy32x32_fast, mtx_mpy24x24_fast, 
  mtx_mpy16x16_fast, mtx_mpyf_fast) that apply some restrictions.

  Precision: 
  32x32 32-bit inputs, 32-bit output
  24x24 24-bit inputs, 24-bit output
  16x16 16-bit inputs, 16-bit output
  f     floating point

  Input:
  x[M*N]      input matrix,Q31, Q15 or floating point
  y[N][P]     input matrix y. For fixed point routines, these are N 
              vectors of size P,Q31 or Q15. For floating point, this 
              is just a matrix of size NxP.
  M           number of rows in matrix x and z
  N           number of columns in matrix x and number of rows in matrix y
  P           number of columns in matrices y and z
  lsh         additional left shift
  Output:
  z[M][P]     output matrix z. For fixed point routines, these are M 
              vectors of size P Q31 or Q15. For floating point, this 
              is single matrix of size MxP
  Scratch:
  pScr        Scratch memory area with size in bytes defined by macros 
              SCRATCH_MTX_MPY32X32, SCRATCH_MTX_MPY24X24, 
              SCRATCH_MTX_MPY16X16


  Restrictions:
  For regular routines (mtx_mpy32x32,mtx_mpy24x24, mtx_mpy16x16, mtx_mpyf):
  x,y,z should not overlap

  For faster routines (mtx_mpy32x32_fast, mtx_mpy24x24_fast, 
  mtx_mpy16x16_fast, mtx_mpyf_fast):
  x,y,z should not overlap
  x - aligned on 8-byte boundary
  all rows which addresses are written to y[] - aligned on 8-byte boundary
  N is a multiple of 4,M=8,P=2  

-------------------------------------------------------------------------*/
void mtx_mpy16x16 (  void* pScr,
                     int16_t** restrict z,
               const int16_t*  restrict x,
               const int16_t** restrict y,
               int M, int N, int P, int lsh )
#if !(XCHAL_HAVE_FUSION_16BIT_BASEBAND)
{
    /* code with no Quad MAC option */
        ae_int32   *  restrict pz   = (      ae_int32    *)z;
  const ae_int16   *  restrict px1  = (const ae_int16    *)x;
  const ae_int32   *  restrict py   = (const ae_int32    *)y;

  int m,n,p;
  int offa,offb;
  ae_int32x2  vpw,vqw;
  ae_int16x4  vxh1;
  ae_int16x4  * restrict Z1;
  ae_int16    * restrict T1;
  ae_int16    * restrict Y1;
  if (N<0)    /* exceptional situation */
  {
        for (m=0; m<M; m++) 
        for (p=0; p<P; p++)
                z[m][p]=0;
        return ;
  }

  for(p=0; p<P; p+=4)
  {
    pz = (      ae_int32   *)z;
    Z1 = (ae_int16x4 *)pScr;
    py = (const ae_int32    *)y;
    for(n=0; n<N; ++n)
    {
      ae_valign t_align;
      ae_int16x4 xx; 
      AE_L32_IP(vqw, py, 4);
      offa = AE_MOVAD32_H(vqw);
      T1=(ae_int16 *)(offa)+p;
      t_align = AE_LA64_PP(T1);
      AE_LA16X4_IP(xx,t_align,castxcc(ae_int16x4,T1));
      AE_S16X4_IP(xx,Z1,8);
   }
    px1 = (const ae_int16 *)x;
    for(m=0; m<M; m++)
    {
      ae_int16x4 A0,A1,A2,A3;
      ae_int16x4 vyh;
      ae_int64 B0,B1,B2,B3;
      ae_int32x2 vah,vbh,vch,vdh;
      B0 = AE_ZERO64();
      B1 = AE_ZERO64();
      B2 = AE_ZERO64();
      B3 = AE_ZERO64();
      Z1 = (ae_int16x4 *)pScr;
      for(n=0; n<(N>>1); n++)
      {
        ae_int16x4 vxh2, vyh2;
        ae_int16x4 y0,y1;
        ae_int32x2 x0,x1;
        AE_L16_IP(vxh1,px1, 2);
        AE_L16_IP(vxh2,px1, 2);
        AE_L16X4_IP(vyh, Z1, 8);
        AE_L16X4_IP(vyh2, Z1, 8);
        y1 = AE_SEL16_5410(vyh,vyh2);
        y1 = AE_SEL16_7520(y1,y1);
        y0 = AE_SEL16_7362(vyh,vyh2);

        x0 = AE_SEXT32X2D16_32(vxh1);
        x1 = AE_SEXT32X2D16_32(vxh2);
        x0 = AE_SEL32_HH(x0,x1);
        AE_MULAAD32X16_H3_L2(B0,x0,y0);
        AE_MULAAD32X16_H1_L0(B1,x0,y0);
        AE_MULAAD32X16_H3_L2(B2,x0,y1);
        AE_MULAAD32X16_H1_L0(B3,x0,y1);
      }
      if (N&1) 
      {
        ae_int32x2 x0;
        AE_L16_IP(vxh1,px1, 2);
        AE_L16X4_IP(vyh, Z1, 8);
        x0 = AE_SEXT32X2D16_32(vxh1);
        AE_MULA32X16_L3(B0,x0,vyh);
        AE_MULA32X16_L2(B1,x0,vyh);
        AE_MULA32X16_L1(B2,x0,vyh);
        AE_MULA32X16_L0(B3,x0,vyh);
      }
      vah = AE_TRUNCA32X2F64S(B0,B1,33);
      vch = AE_TRUNCA32X2F64S(B2,B3,33);
      vbh = AE_SLAA32S(vah,lsh);
      vdh = AE_SLAA32S(vch,lsh);
      vbh = AE_SRAI32(vbh,16);
      vdh = AE_SRAI32(vdh,16);

      vah = AE_SEL32_HH(vbh,vbh);
      vch = AE_SEL32_HH(vdh,vdh);
      A0 = AE_MOVINT16X4_FROMINT32X2(vah);
      A1 = AE_MOVINT16X4_FROMINT32X2(vbh);
      A2 = AE_MOVINT16X4_FROMINT32X2(vch);
      A3 = AE_MOVINT16X4_FROMINT32X2(vdh);
      AE_L32_IP(vpw, pz, 4); 
      offb = AE_MOVAD32_H(vpw);
      Y1 = (ae_int16 *)(offb+p*2);
      AE_S16_0_IP(A0,Y1,2);
      if (p+1<P) AE_S16_0_IP(A1,Y1,2);
      if (p+2<P) AE_S16_0_IP(A2,Y1,2);
      if (p+3<P) AE_S16_0_IP(A3,Y1,2);
    }
  }    
}
#else
{
    /* code with Quad MAC option */
    const ae_int16x4  * restrict py;
    const ae_int32x2  * restrict pz;
    const ae_int16x4  * restrict px0;
    const ae_int16x4  * restrict px1;
    const ae_int16    * restrict T0;
    const ae_int16    * restrict T1;
    ae_int32x2 a0,a1,addr;
    ae_int64 B0,B1,B2,B3;
    ae_int16x4 t0,t1;
    ae_valign az,ax0,ax1;
    int m,n,p,NN;
    int16_t * restrict scr=(int16_t *)pScr;
    if (N<0)    /* exceptional situation */
    {
        for (m=0; m<M; m++) 
        for (p=0; p<P; p++)
                z[m][p]=0;
        return ;
    }
    NASSERT_ALIGN(pScr,8);
    NN=(N+3)&~3;
    for(n=N; n<NN; n++) scr[n]=scr[n+NN]=0;
    for(p=0; p<(P&~1); p+=2)
    {
        /* copy 2 colums with zero padding */
        for(n=0; n<N; n++)
        {
            scr[n   ]=y[n][p+0];
            scr[n+NN]=y[n][p+1];
        }
        __Pragma("no_reorder")
        pz=(const ae_int32x2  *)z;
        az = AE_LA64_PP(pz);
        /* main processing: Nrows*2 columns */
        for (m=0; m<(M&~1); m+=2)
        {
            py =(const ae_int16x4  *)scr;
            px0=(const ae_int16x4  *)(x+m*N);
            px1=(const ae_int16x4  *)(x+m*N+N);
            ax0=AE_LA64_PP(px0);
            ax1=AE_LA64_PP(px1);

            B0=B1=B2=B3=AE_ZERO64();
            for (n=0; n<NN; n+=4)
            {
                ae_int16x4 y0,y1,x0,x1;
                AE_LA16X4_IP(x0,ax0,px0);
                AE_LA16X4_IP(x1,ax1,px1);
                y1=AE_L16X4_X(py,NN*sizeof(int16_t));
                AE_L16X4_IP(y0,py,sizeof(y0));
                AE_MULAAAAQ16(B0,x0,y0);
                AE_MULAAAAQ16(B1,x0,y1);
                AE_MULAAAAQ16(B2,x1,y0);
                AE_MULAAAAQ16(B3,x1,y1);
            }
            a0=AE_TRUNCA32X2F64S(B0,B1,lsh+33);
            a1=AE_TRUNCA32X2F64S(B2,B3,lsh+33);
            t0=AE_ROUND16X4F32SASYM(a0,a0);
            t1=AE_ROUND16X4F32SASYM(a1,a1);
            AE_LA32X2_IP(addr, az, pz); 
            addr=AE_ADD32(addr,p*sizeof(int16_t));
            T0 = (ae_int16*) AE_MOVAD32_H(addr);
            T1 = (ae_int16*) AE_MOVAD32_L(addr);
            AE_S16_0_I(AE_SHORTSWAP(t0),castxcc(ae_int16,T0),0);
            AE_S16_0_I(t0, castxcc(ae_int16, T0), 2);
            AE_S16_0_I(AE_SHORTSWAP(t1), castxcc(ae_int16, T1), 0);
            AE_S16_0_I(t1, castxcc(ae_int16, T1), 2);
        }
        if(M>0 && M&1)
        {
            py =(const ae_int16x4  *)scr;
            px0=(const ae_int16x4  *)(x+m*N);
            ax0=AE_LA64_PP(px0);

            B0=B1=AE_ZERO64();
            for (n=0; n<NN; n+=4)
            {
                ae_int16x4 y0,y1,x0;
                AE_LA16X4_IP(x0,ax0,px0);
                y1=AE_L16X4_X(py,NN*sizeof(int16_t));
                AE_L16X4_IP(y0,py,sizeof(y0));
                AE_MULAAAAQ16(B0,x0,y0);
                AE_MULAAAAQ16(B1,x0,y1);
            }
            a0=AE_TRUNCA32X2F64S(B0,B1,lsh+33);
            t0=AE_ROUND16X4F32SASYM(a0,a0);
            AE_LA32X2_IP(addr, az, pz); 
            addr=AE_ADD32(addr,p*sizeof(int16_t));
            T0 = (ae_int16*) AE_MOVAD32_H(addr);
            AE_S16_0_I(AE_SHORTSWAP(t0), castxcc(ae_int16, T0), 0);
            AE_S16_0_I(t0, castxcc(ae_int16, T0), 2);
        }
    }
    /* last column */
    if(P>0 && P&1)
    {
        /* copy 2 colums with zero padding */
        for(n=0; n<N; n++)
        {
            scr[n   ]=y[n][p+0];
        }
        __Pragma("no_reorder")
        pz=(const ae_int32x2  *)z;
        az = AE_LA64_PP(pz);
        for (m=0; m<(M&~1); m+=2)
        {
            py =(const ae_int16x4  *)scr;
            px0=(const ae_int16x4  *)(x+m*N);
            px1=(const ae_int16x4  *)(x+m*N+N);
            ax0=AE_LA64_PP(px0);
            ax1=AE_LA64_PP(px1);

            B0=B2=AE_ZERO64();
            for (n=0; n<NN; n+=4)
            {
                ae_int16x4 y0,x0,x1;
                AE_LA16X4_IP(x0,ax0,px0);
                AE_LA16X4_IP(x1,ax1,px1);
                AE_L16X4_IP(y0,py,sizeof(y0));
                AE_MULAAAAQ16(B0,x0,y0);
                AE_MULAAAAQ16(B2,x1,y0);
            }
            a0=AE_TRUNCA32X2F64S(B0,B0,lsh+33);
            a1=AE_TRUNCA32X2F64S(B2,B2,lsh+33);
            t0=AE_ROUND16X4F32SASYM(a0,a0);
            t1=AE_ROUND16X4F32SASYM(a1,a1);
            AE_LA32X2_IP(addr, az, pz); 
            addr=AE_ADD32(addr,p*sizeof(int16_t));
            T0 = (ae_int16*) AE_MOVAD32_H(addr);
            T1 = (ae_int16*) AE_MOVAD32_L(addr);
            AE_S16_0_I(t0, castxcc(ae_int16, T0), 0);
            AE_S16_0_I(t1, castxcc(ae_int16, T1), 0);
        }
        if(M>0 && M&1)
        {
            py =(const ae_int16x4  *)scr;
            px0=(const ae_int16x4  *)(x+m*N);
            ax0=AE_LA64_PP(px0);

            B0=AE_ZERO64();
            for (n=0; n<NN; n+=4)
            {
                ae_int16x4 y0,x0;
                AE_LA16X4_IP(x0,ax0,px0);
                AE_L16X4_IP(y0,py,sizeof(y0));
                AE_MULAAAAQ16(B0,x0,y0);
            }
            a0=AE_TRUNCA32X2F64S(B0,B0,lsh+33);
            t0=AE_ROUND16X4F32SASYM(a0,a0);
            AE_LA32X2_IP(addr, az, pz); 
            addr=AE_ADD32(addr,p*sizeof(int16_t));
            T0 = (ae_int16*) AE_MOVAD32_H(addr);
            AE_S16_0_I(t0, castxcc(ae_int16, T0), 0);
        }
    }
} /* mtx_mpy16x16() */
#endif
