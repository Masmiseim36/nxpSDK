/* ------------------------------------------------------------------------ */
/* Copyright (c) 2016 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (�Cadence    */
/* Libraries�) are the copyrighted works of Cadence Design Systems Inc.	    */
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



#ifdef COMPILER_MSVC
#include <malloc.h>
#else
#include <alloca.h>
#endif

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
void mtx_mpy16x16_fast (  
                     int16_t** restrict z,
               const int16_t*  restrict x,
               const int16_t** restrict y,
               int M, int N, int P, int lsh )
#if !(XCHAL_HAVE_FUSION_16BIT_BASEBAND)
{
    /* code with no Quad MAC option */
        ae_int32   *  restrict pz = (      ae_int32      *)z;
  const ae_int16   *  restrict px0;
  const ae_int32   *  restrict py = (const ae_int32      *)y;
  const ae_int16   *  restrict px1;
  const ae_int16   *  restrict px2;
  const ae_int16   *  restrict px3;

  int         m, n, offa, offb, offc, offd;
  ae_int32x2  vpw, vqw;

  const ae_int16x4  * restrict T1;
  const ae_int16x4  * restrict T2;
  ae_p16x2s   * restrict Y1;
  ae_p16x2s   * restrict Y2;
  ae_p16x2s   * restrict Y3;
  ae_p16x2s   * restrict Y4;
  ae_valign y_align, z_align;
  NASSERT((N&1)==0);
  NASSERT(M==8);
  NASSERT(P==2);
  pz = (      ae_int32      *)z;
  z_align = AE_LA64_PP(pz);
  px0 = (const ae_int16 *)x - 3*N - 2;
  for(m=0; m<2; m++)
  {
    ae_int64    C0=0, C1=0, C2=0, C3=0, C4=0, C5=0, C6=0, C7=0;
    ae_int32x2  vah, vbh, vch, vdh;

    C0 = AE_XOR64(C0,C0);
    // C0 = AE_L64_I(0,0);  
    C1 = AE_XOR64(C1,C1);
    // C1 = AE_L64_I(0,0);  
    C2 = AE_XOR64(C2,C2);
    // C2 = AE_L64_I(0,0);  
    C3 = AE_XOR64(C3,C3);
    // C3 = AE_L64_I(0,0);  
    C4 = AE_XOR64(C4,C4);
    // C4 = AE_L64_I(0,0);  
    C5 = AE_XOR64(C5,C5);
    // C5 = AE_L64_I(0,0);  
    C6 = AE_XOR64(C6,C6);
    // C6 = AE_L64_I(0,0);  
    C7 = AE_XOR64(C7,C7);
    // C7 = AE_L64_I(0,0);  
    px0 = (const ae_int16 *)px0 + 3*N;
    px1 = (const ae_int16 *)px0 +   N;
    px2 = (const ae_int16 *)px1 +   N;
    px3 = (const ae_int16 *)px2 +   N;
    py  = (const ae_int32 *)y;
    y_align = AE_LA64_PP(py);
    for(n=0; n<N; n+=2)
    {
      ae_int32x2 x0,y1,y2,x1,x2,x3;
      ae_int32x2 tx,ty;
      ae_f32x2 xx,yy;
      ae_f16x4 i0;
      ae_int16x4 ii;

      AE_L16X2M_IU(x0,castxcc(ae_p16x2s,px0), 4);
      AE_L16X2M_IU(x1,castxcc(ae_p16x2s,px1), 4);
      AE_L16X2M_IU(x2,castxcc(ae_p16x2s,px2), 4);
      AE_L16X2M_IU(x3,castxcc(ae_p16x2s,px3), 4);
      AE_LA32X2_IP(vqw, y_align, castxcc(ae_int32x2,py));
      offa = AE_MOVAD32_H(vqw);
      offb = AE_MOVAD32_L(vqw);
      T1 = (ae_int16x4 *)(offa);
      T2 = (ae_int16x4 *)(offb);
      NASSERT_ALIGN4(T1);
      NASSERT_ALIGN4(T2);
      AE_L16X2M_IU(y1, castxcc(ae_p16x2s,T1),0);
      AE_L16X2M_IU(y2, castxcc(ae_p16x2s,T2),0);
      tx = AE_SEL32_HH(y1,y2);
      ty = AE_SEL32_LL(y1,y2);
      tx = AE_SLAI32(tx,8);
      ty = AE_SLAI32(ty,8);
      xx = (tx);
      yy = (ty);
      i0 = AE_TRUNC16X4F32(xx,yy);
      ii = (i0);
      AE_MULAAD32X16_H3_L2(C0,x0,ii);
      AE_MULAAD32X16_H1_L0(C1,x0,ii); 
      AE_MULAAD32X16_H3_L2(C2,x1,ii);
      AE_MULAAD32X16_H1_L0(C3,x1,ii);
      AE_MULAAD32X16_H3_L2(C4,x2,ii);
      AE_MULAAD32X16_H1_L0(C5,x2,ii);
      AE_MULAAD32X16_H3_L2(C6,x3,ii);
      AE_MULAAD32X16_H1_L0(C7,x3,ii);
    }
    vah = AE_TRUNCA32X2F64S(C0,C1,25);
    vbh = AE_TRUNCA32X2F64S(C2,C3,25);
    vch = AE_TRUNCA32X2F64S(C4,C5,25);
    vdh = AE_TRUNCA32X2F64S(C6,C7,25);
    vah = AE_SLAA32S(vah,lsh);
    vbh = AE_SLAA32S(vbh,lsh);
    vch = AE_SLAA32S(vch,lsh);
    vdh = AE_SLAA32S(vdh,lsh);
    vah = AE_SRAI32(vah,8);
    vbh = AE_SRAI32(vbh,8);
    vch = AE_SRAI32(vch,8);
    vdh = AE_SRAI32(vdh,8);
    AE_LA32X2_IP(vpw, z_align, castxcc(ae_int32x2,pz)); 
    AE_LA32X2_IP(vqw, z_align, castxcc(ae_int32x2,pz)); 
    offa = AE_MOVAD32_H(vpw);
    offb = AE_MOVAD32_L(vpw);
    offc = AE_MOVAD32_H(vqw);
    offd = AE_MOVAD32_L(vqw);
    Y1=(ae_p16x2s *)(offa);
    Y2=(ae_p16x2s *)(offb);
    Y3=(ae_p16x2s *)(offc);
    Y4=(ae_p16x2s *)(offd);
    NASSERT_ALIGN4(Y1);
    NASSERT_ALIGN4(Y2);
    NASSERT_ALIGN4(Y3);
    NASSERT_ALIGN4(Y4);
    AE_S16X2M_IU(vah,Y1,0);
    AE_S16X2M_IU(vbh,Y2,0);
    AE_S16X2M_IU(vch,Y3,0);
    AE_S16X2M_IU(vdh,Y4,0);
   }
}
#else
{
    /* code with Quad MAC option */
    int m,n;
    const ae_int16x4  * restrict py;
    const ae_int32x2  * restrict pz;
    const ae_int16x4  * restrict px;
    const ae_int16    * restrict T0;
    const ae_int16    * restrict T1;
    ae_int16x4 t0,t1;
    ae_valign az;
    int16_t * restrict scr;


    NASSERT(N%4==0);
    NASSERT(M==8);
    NASSERT(P==2);

    if(N<=0)
    {
        for (m=0; m<8; m++) z[m][0]=z[m][1]=0; 
        return;
    }

    scr=(int16_t *)alloca(N*2*sizeof(int16_t));
    NASSERT_ALIGN8(scr);
    for(n=0; n<N; n++)
    {
        scr[n  ]=y[n][0];
        scr[n+N]=y[n][1];
    }
    __Pragma("no_reorder")
    pz=(const ae_int32x2  *)z;
    az = AE_LA64_PP(pz);
    for (m=0; m<8; m+=2)
    {
        ae_int32x2 a0,a1,addr;
        ae_int64 B0,B1,B2,B3;
        py=(const ae_int16x4  *)scr;
        px=(const ae_int16x4  *)x;

        B0=B1=B2=B3=AE_ZERO64();
        for (n=0; n<N; n+=4)
        {
            ae_int16x4 y0,y1,x0,x1;
            x1=AE_L16X4_X(px,N*sizeof(int16_t));
            y1=AE_L16X4_X(py,N*sizeof(int16_t));
            AE_L16X4_IP(x0,px,sizeof(x0));
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
        T0 = (ae_int16*) AE_MOVAD32_H(addr);
        T1 = (ae_int16*) AE_MOVAD32_L(addr);
        AE_S16_0_I(AE_SHORTSWAP(t0),castxcc(ae_int16,T0),0);
        AE_S16_0_I(             t0 ,castxcc(ae_int16,T0),2);
        AE_S16_0_I(AE_SHORTSWAP(t1),castxcc(ae_int16,T1),0);
        AE_S16_0_I(             t1 ,castxcc(ae_int16,T1),2);
        x+=N*2;
    }
} /* mtx_mpy16x16_fast() */
#endif
