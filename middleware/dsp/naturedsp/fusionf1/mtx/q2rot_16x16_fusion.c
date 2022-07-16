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

/* Cross-platform data type definitions. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common helper macros. */
#include "common.h"
/*-------------------------------------------------------------------------
  Quaternion to Rotation Matrix Conversion
  These functions convert sequence of unit quaternions to corresponding 
  rotation matrices

  Precision: 
  16x16  16-bit input, 16-bit output
  32x32  32-bit input, 32-bit output
  f      floating point 

  Input:
  q[L][4]    L quaternions
  L          number of matrices
  Output:
  r[L][3*3]  L rotation matrices

  Restrictions:
  q,r  should not overlap
-------------------------------------------------------------------------*/
#define sz_i16    (int)sizeof(int16_t)
void q2rot_16x16(int16_t   *r, const int16_t   *q, int L)
{
#if 0
  int l;
        ae_int16x4    *  pr = (ae_int16x4      *)r;
  const ae_int16x4    *  restrict pq = (const ae_int16x4      *)q;
  ae_int16x4 x0, z0, z1, z2, z3;
  ae_int32x2 t0, t1;
  ae_int32x2 y0, y1, y2, y3, r0, r1, r2, r3, r4;
  ae_int24x2 p0, p1, p2, p3;
  ae_valign qa, ra;
  ae_int64 A0, A1, A2, A3, A4, A5;
  ae_int64 B0, B1, B2;
  qa = AE_LA64_PP(pq);
  ra = AE_ZALIGN64();
  for (l = 0; l<(L); l++)
  {
    AE_LA16X4_IP(x0, qa, pq);
    t0 = AE_SEXT32X2D16_32(x0);
    t1 = AE_SEXT32X2D16_10(x0);
    p0 = AE_MOVINT24X2_FROMINT32X2(t0);
    p1 = AE_MOVINT24X2_FROMINT32X2(t1);

    p2 = AE_SEL24_HH(p0, p1); //x0;x2
    p3 = AE_SEL24_LL(p1, p0); //x1;x3

    B0 = AE_MULZAAD24_HH_LL(p0, p0); //x0*x0+x1*x1
    AE_MULSSD24_HH_LL(B0,p1, p1);//r[0]
    B1 = AE_MULZASD24_HH_LL(p0, p0); //x0*x0+x1*x1
    AE_MULASD24_HH_LL(B1, p1, p1);//r[4]
    B2 = AE_MULZASD24_HH_LL(p0, p0); //x0*x0-x1*x1
    AE_MULSAD24_HH_LL(B2, p1, p1);//r[8]
    A0 = AE_MULZAAD24_HL_LH(p0, p1); //r[1]
    A1 = AE_MULZSAD24_HH_LL(p0, p1); //r[2]
    A2 = AE_MULZAAD24_HH_LL(p0, p1); //r[6]
    
    A3 = AE_MULZSAD24_HH_LL(p3, p2); //r[3]
    A4 = AE_MULZAAD24_HL_LH(p3, p2); //r[5]
    A5 = AE_MULZASD24_HL_LH(p3, p2); //r[7]

    A0 = AE_SLAI64(A0, 1 + 17);
    B0 = AE_SLAI64(B0, 17);
    r0 = AE_ROUND32X2F48SASYM(B0, A0);
    r1 = AE_TRUNCA32X2F64S(A1, A3, 34); //r[2];r[3]
    B1 = AE_SLAI64(B1, 17);
    A4 = AE_SLAI64(A4, 1+17);
    r2 = AE_ROUND32X2F48SASYM(B1, A4);
    A2 = AE_SLAI64(A2, 1 + 17);
    A5 = AE_SLAI64(A5, 1 + 17);
    r3 = AE_ROUND32X2F48SASYM(A2, A5);
    r4 = AE_TRUNCA32X2F64S(B2, B2, 33); //r[8];r[8]

    
    z0 = AE_ROUND16X4F32SASYM(r0,r1);//0..3
    z1 = AE_ROUND16X4F32SASYM(r2,r3);//4..7
    z2 = AE_ROUND16X4F32SASYM(r4,r4);
    AE_SA16X4_IP(z0, ra, pr);
    AE_SA16X4_IP(z1, ra, pr);
    AE_SA64POS_FP(ra, pr);
    AE_S16_0_IP(z2,pr,2);

  }
#else
  int l;
        ae_int16x4    *  pr = (ae_int16x4      *)r;
        ae_int16x4    *  pr_ = (ae_int16x4      *)(r+9);
  const ae_int16x4    *  restrict pq = (const ae_int16x4      *)q;
  ae_int16x4 x0, x1, x2, x3, z0, z1, z2, z3,z4;
  ae_int16x4 v0, v1, v2, v3, v4;
  ae_int32x2 t0, t1, t2, t3;
  ae_int32x2 y0, y1, r0, r1, r2, r3, r4;
  ae_int24x2 p0, p1, p2, p3, p02, p12;
  ae_valign qa, ra, ra_;
  ae_int32x2 B0, B1, B2, B3, B4, B5, B6, B7, B8;
  qa = AE_LA64_PP(pq);
  ra = AE_ZALIGN64();
  ra_ = AE_ZALIGN64();
  if (L<=0) return;
  AE_LA16X4_IP(x0, qa, pq);
  AE_LA16X4_IP(x1, qa, pq);
  z0 = AE_SEL16_7362(x0, x1);
  t0 = AE_SEXT32X2D16_32(z0);
  t1 = AE_SEXT32X2D16_10(z0);
  y0 = AE_SEXT32X2D16_10(x0);
  y1 = AE_SEXT32X2D16_10(x1);
  t2 = AE_SEL32_HH(y0, y1);
  t3 = AE_SEL32_LL(y0, y1);
  p0 = AE_MOVINT24X2_FROMINT32X2(t0); //x0
  p1 = AE_MOVINT24X2_FROMINT32X2(t1); //x1
  p2 = AE_MOVINT24X2_FROMINT32X2(t2); //x2
  p3 = AE_MOVINT24X2_FROMINT32X2(t3); //x3
  r0 = AE_MULP24X2(p0, p0); // x0*x0
  for (l = 0; l<(L>>1); l++)
  {
    r1 = r0;
    AE_MULSP24X2(r1, p3, p3); // x0*x0 - x3*x3
    r2 = AE_MULP24X2(p1, p1); // x1*x1
    AE_MULSP24X2(r2, p2, p2); // x1*x1 - x2*x2
    B0 = AE_ADD32(r1, r2); //r[0]
    B1 = AE_SUB32(r1, r2); //r[4]
    AE_MULSP24X2(r0, p1, p1); // x0*x0 - x1*x1
    AE_MULSP24X2(r0, p2, p2);
    AE_MULAP24X2(r0, p3, p3);
    B2 = r0; //r[8]
    p02 = AE_ADD24S(p0, p0);
    p12 = AE_ADD24S(p1, p1);
    r0 = AE_MULP24X2(p12, p2); // 2*x1*x2
    B3 = r0; B4 = r0;
    AE_MULAP24X2(B3, p02, p3); //r[1]
    AE_MULSP24X2(B4, p02, p3); //r[3]

    r1 = AE_MULP24X2(p12, p3); // 2*x1*x3
    B5 = r1; B6 = r1;
    AE_MULSP24X2(B5, p02, p2); //r[2]
    AE_MULAP24X2(B6, p02, p2); //r[6]

    r2 = AE_MULP24X2(p2, p3); //x2*x3
    B7 = r2; B8 = r2;
    AE_MULAP24X2(B7, p0, p1); 
    AE_MULSP24X2(B8, p0, p1); 
    B7 = AE_ADD32(B7, B7);//r[5]
    B8 = AE_ADD32(B8, B8);//r[7]

    B0 = AE_SLAI32S(B0, 1);
    B1 = AE_SLAI32S(B1, 1);
    B2 = AE_SLAI32S(B2, 1);
    B3 = AE_SLAI32S(B3, 1);
    B4 = AE_SLAI32S(B4, 1);
    B5 = AE_SLAI32S(B5, 1);
    B6 = AE_SLAI32S(B6, 1);
    B7 = AE_SLAI32S(B7, 1);
    B8 = AE_SLAI32S(B8, 1);

    v0 = AE_ROUND16X4F32SASYM(B0, B3);
    v1 = AE_ROUND16X4F32SASYM(B5, B4);
    v2 = AE_ROUND16X4F32SASYM(B1, B7);
    v3 = AE_ROUND16X4F32SASYM(B6, B8);
    v4 = AE_ROUND16X4F32SASYM(B2, B2);

   
    z1 = AE_SEL16_4321(v1, v1);
    z3 = AE_SEL16_4321(v3, v3);
    z4 = AE_SHORTSWAP(v4);
  
    x0 = AE_SEL16_7520(v0, z1);
    x1 = AE_SEL16_7520(v2, z3);
    AE_SA16X4_IP(x0, ra, pr);
    AE_SA16X4_IP(x1, ra, pr);
    AE_SA64POS_FP(ra, pr);
    AE_S16_0_XP(z4, castxcc(ae_int16, pr), 10 * 2);
  
    x2 = AE_SEL16_6420(v0, v1);
    x3 = AE_SEL16_6420(v2, v3);
  
    AE_SA16X4_IP(x2, ra_, pr_);
    AE_SA16X4_IP(x3, ra_, pr_);
    AE_SA64POS_FP(ra_, pr_);
    AE_S16_0_XP(v4, castxcc(ae_int16, pr_), 10 * 2);

    AE_LA16X4_IP(x0, qa, pq);
    AE_LA16X4_IP(x1, qa, pq);

    z0 = AE_SEL16_7362(x0, x1);
    t0 = AE_SEXT32X2D16_32(z0);
    t1 = AE_SEXT32X2D16_10(z0);
    y0 = AE_SEXT32X2D16_10(x0);
    y1 = AE_SEXT32X2D16_10(x1);
    t2 = AE_SEL32_HH(y0, y1);
    t3 = AE_SEL32_LL(y0, y1);
    p0 = AE_MOVINT24X2_FROMINT32X2(t0); //x0
    p1 = AE_MOVINT24X2_FROMINT32X2(t1); //x1
    p2 = AE_MOVINT24X2_FROMINT32X2(t2); //x2
    p3 = AE_MOVINT24X2_FROMINT32X2(t3); //x3

    r0 = AE_MULP24X2(p0, p0); // x0*x0
  }
  if (L&1)
  {
    ae_int64 A0, A1, A2, A3, A4, A5;
    ae_int64 B0, B1, B2;

    t0 = AE_SEXT32X2D16_32(x0);
    t1 = AE_SEXT32X2D16_10(x0);
    p0 = AE_MOVINT24X2_FROMINT32X2(t0);
    p1 = AE_MOVINT24X2_FROMINT32X2(t1);

    p2 = AE_SEL24_HH(p0, p1); //x0;x2
    p3 = AE_SEL24_LL(p1, p0); //x1;x3

    B0 = AE_MULZAAD24_HH_LL(p0, p0); //x0*x0+x1*x1
    AE_MULSSD24_HH_LL(B0, p1, p1);//r[0]
    B1 = AE_MULZASD24_HH_LL(p0, p0); //x0*x0+x1*x1
    AE_MULASD24_HH_LL(B1, p1, p1);//r[4]
    B2 = AE_MULZASD24_HH_LL(p0, p0); //x0*x0-x1*x1
    AE_MULSAD24_HH_LL(B2, p1, p1);//r[8]



    A0 = AE_MULZAAD24_HL_LH(p0, p1); //r[1]
    A1 = AE_MULZSAD24_HH_LL(p0, p1); //r[2]
    A2 = AE_MULZAAD24_HH_LL(p0, p1); //r[6]

    A3 = AE_MULZSAD24_HH_LL(p3, p2); //r[3]
    A4 = AE_MULZAAD24_HL_LH(p3, p2); //r[5]
    A5 = AE_MULZASD24_HL_LH(p3, p2); //r[7]

    A0 = AE_SLAI64(A0, 1);
    A4 = AE_SLAI64(A4, 1);
    r0 = AE_TRUNCA32X2F64S(B0, A0, 33); //r[0];r[1]
    r1 = AE_TRUNCA32X2F64S(A1, A3, 34); //r[2];r[3]
    r2 = AE_TRUNCA32X2F64S(B1, A4, 33); //r[4];r[5]
    r3 = AE_TRUNCA32X2F64S(A2, A5, 34); //r[6];r[7]
    r4 = AE_TRUNCA32X2F64S(B2, B2, 33); //r[8];r[8]

    z0 = AE_ROUND16X4F32SASYM(r0, r1);//0..3
    z1 = AE_ROUND16X4F32SASYM(r2, r3);//4..7
    z2 = AE_ROUND16X4F32SASYM(r4, r4);
    AE_SA16X4_IP(z0, ra, pr);
    AE_SA16X4_IP(z1, ra, pr);
    AE_SA64POS_FP(ra, pr);
    AE_S16_0_IP(z2, castxcc(ae_int16, pr), 2);

  }
  #endif
} /* q2rot_16x16() */

