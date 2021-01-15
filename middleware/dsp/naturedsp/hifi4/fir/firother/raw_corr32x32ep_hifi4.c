/* ------------------------------------------------------------------------ */
/* Copyright (c) 2018 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs ("Cadence    */
/* Libraries") are the copyrighted works of Cadence Design Systems Inc.     */
/* Cadence IP is licensed for use with Cadence processor cores only and     */
/* must not be used for any other processors and platforms. Your use of the */
/* Cadence Libraries is subject to the terms of the license agreement you   */
/* have entered into with Cadence Design Systems, or a sublicense granted   */
/* to you by a direct Cadence licensee.                                     */
/* ------------------------------------------------------------------------ */



/* ------------------------------------------------------------------------ */
/*  IntegrIT, Ltd.   www.integrIT.com, info@integrIT.com                    */
/*                                                                          */
/*  NatureDSP Signal Library                                                */
/*                                                                          */
/* This library contains copyrighted materials, trade secrets and other     */
/* proprietary information of IntegrIT, Ltd. This software is licensed for  */
/* use with Tensilica HiFi4 core only and must not be used for any other    */
/* processors and platforms. The license to use these sources was given to  */
/* Cadence, Inc. under Terms and Condition of a Software License Agreement  */
/* between Cadence, Inc. and IntegrIT, Ltd. Any third party must not use    */
/* this code in any form unless such rights are given by Tensilica, Inc.    */
/* Compiling, linking, executing or using this library in any form you      */
/* unconditionally accept these terms.                                      */
/* ------------------------------------------------------------------------ */
/*          Copyright (C) 2014 IntegrIT, Limited.                           */
/*                          All Rights Reserved.                            */
/* ------------------------------------------------------------------------ */ 
/* Portable data types. */
#include "NatureDSP_types.h"
/* Common utility and macros declarations. */
#include "common.h"
#include "raw_corr32x32ep.h"

/*--------------------------------------------------------------------------------
    Helper for circular correlation/convolution with originally non-aligned inputs
    This function takes aligned inputs (allocated from the scratch) with circularly 
    duplicated inputs

    raw linear correlation
    input:
    x[N+M-1+4] - input padded with 4 zeroes. should be aligned on 8-byte boundary
    y[M+4]     - input padded with 4 zeroes. should be aligned on 8-byte boundary
    output:
    r[N]       - correlations
--------------------------------------------------------------------------------*/
void raw_corr32x32ep(int32_t* restrict r, const int32_t* restrict  x,const int32_t* restrict y, int N,int M)
{
    int n,m;
        ae_int32x2 * restrict R;
  const ae_int32x2 * restrict X;
  const ae_int32x2 * restrict Z;
  const ae_int32x2 * restrict Y;
  ae_int32x2 x01,x12,x23,x34,x45,x56,x67,y01,y23,r01,r23;
  ae_int64 q0,q1,q2,q3;
  ae_ep    e0,e1,e2,e3;
  int _0=0;
  ae_valign x_align, r_align;

  X=(const ae_int32x2 *)x;
  Y=(const ae_int32x2 *)y;
  R=(      ae_int32x2 *)r;
  NASSERT_ALIGN8(x);
  NASSERT_ALIGN8(y);
  r_align = AE_ZALIGN64();
  M=(M+3)&~3;
  for ( n=0; n<(N>>2); n++ )
  {
  //X = (const ae_int32x2 *)(x+n);
    AE_L32X2_IP( x01, X,8 );
    AE_L32X2_IP( x23, X,8 );
    Z=X;
    XT_MOVEQZ(_0,_0,_0);
    e0=e1=e2=e3=AE_MOVEA(_0);
    q0=q1=q2=q3=AE_ZERO64();
    __Pragma("loop_count min=1")
    for (m=0; m<(M>>2); m++ )
    {
        AE_L32X2_IP( x45, X, 8);
        AE_L32X2_IP( x67, X, 8);
        AE_L32X2_IP( y01, Y, 8);
        AE_L32X2_IP( y23, Y, 8);
        x12=AE_SEL32_LH(x01,x23);
        x34=AE_SEL32_LH(x23,x45);
        x56=AE_SEL32_LH(x45,x67);

        AE_MULAAD32EP_HH_LL(e0,q0,x01,y01);
        AE_MULAAD32EP_HH_LL(e0,q0,x23,y23);

        AE_MULAAD32EP_HH_LL(e1,q1,x12,y01);
        AE_MULAAD32EP_HH_LL(e1,q1,x34,y23);

        AE_MULAAD32EP_HH_LL(e2,q2,x23,y01);
        AE_MULAAD32EP_HH_LL(e2,q2,x45,y23);

        AE_MULAAD32EP_HH_LL(e3,q3,x34,y01);
        AE_MULAAD32EP_HH_LL(e3,q3,x56,y23);
        x01=x45;
        x23=x67;
    }
    X=Z;
    Y-=M/2;
    q0=AE_SRAI72(e0,q0,15);
    q1=AE_SRAI72(e1,q1,15);
    q2=AE_SRAI72(e2,q2,15);
    q3=AE_SRAI72(e3,q3,15);
    r01=AE_ROUND32X2F48SASYM(q0,q1);
    r23=AE_ROUND32X2F48SASYM(q2,q3);
    AE_SA32X2_IP(r01,r_align,R);
    AE_SA32X2_IP(r23,r_align,R);
  }
  AE_SA64POS_FP(r_align, R);
  // final loop: up to 3 iterations
  for (n=(N&~3); n<N; n++ )
  {
    //X = (const ae_int32x2 *)(x_buf+n);
    x_align = AE_LA64_PP(X);
    XT_MOVEQZ(_0,_0,_0);
    e0=AE_MOVEA(_0);
    q0=AE_ZERO64();
    __Pragma("loop_count min=1")
    for ( m=0; m<M; m+=4 )
    {
        AE_LA32X2_IP( x01, x_align ,X);
        AE_LA32X2_IP( x23, x_align ,X);
        AE_L32X2_IP( y01, Y, 8);
        AE_L32X2_IP( y23, Y, 8);
        AE_MULAAD32EP_HH_LL(e0,q0,x01,y01);
        AE_MULAAD32EP_HH_LL(e0,q0,x23,y23);
    }
    Y-=M/2;
    X-=M/2;
    AE_L32_IP(x01,castxcc(ae_int32,X),4);
    q0=AE_SRAI72(e0,q0,15);
    r01=AE_ROUND32X2F48SASYM(q0,q0);
    AE_S32_L_IP(r01,castxcc(ae_int32,R),4);
  }
}
