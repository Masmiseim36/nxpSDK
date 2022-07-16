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
void q2rot_32x32(int32_t   *r, const int32_t   *q, int L)
{
  ae_int32x2    *  pr = (ae_int32x2      *)r;
  const ae_int32x2    *  restrict pq = (const ae_int32x2      *)(q);
  int l;
  ae_int32x2 q0, q1, z0, z1, z2, z3, z4;
  ae_f64 A0, A1, A2;
  ae_f64 B0, B1, B2, B3, B4, B5, B6, B7, B8;
  ae_valign qa, ra;
  qa = AE_LA64_PP(pq);
  ra = AE_ZALIGN64();
  for (l = 0; l<(L); l++)
  {
    AE_LA32X2_IP(q0, qa, pq);
    AE_LA32X2_IP(q1, qa, pq);
    A0 = AE_MULF32R_HH(q0, q0); //q[0] * q[0]
    A1 = A0;
    AE_MULSF32R_LL(A1, q1, q1);//q[0] * q[0] - q[3] * q[3]
    A2 = AE_MULF32R_LL(q0, q0); //q[1] * q[1]
    AE_MULSF32R_HH(A2, q1, q1);//q[1] * q[1] - q[2] * q[2]
    B0 = AE_ADD64(A1, A2);//r[0]
    B4 = AE_SUB64(A1, A2);//r[4]
    AE_MULSF32R_LL(A0, q0, q0);
    AE_MULSF32R_HH(A0, q1, q1);
    AE_MULAF32R_LL(A0, q1, q1);
    B8 = A0; //r[8]

    A0 = AE_MULF32R_LH(q0, q1); // q[1] * q[2]
    B1 = A0; B3 = A0;
    AE_MULAF32R_HL(B1, q0, q1); //[r1]
    AE_MULSF32R_HL(B3, q0, q1); //[r3]

    A1 = AE_MULF32R_LL(q0, q1); // q[1] * q[3]
    B2 = A1; B6 = A1;
    AE_MULSF32R_HH(B2, q0, q1); //[r2]
    AE_MULAF32R_HH(B6, q0, q1); //[r6]

    A2 = AE_MULF32R_HL(q1, q1); //q[2] * q[3]
    B5 = A2; B7 = A2;
    AE_MULAF32R_HL(B5, q0, q0);
    AE_MULSF32R_HL(B7, q0, q0);

    
    B1 = AE_SLAI64(B1, 1);
    B2 = AE_SLAI64(B2, 1);
    B3 = AE_SLAI64(B3, 1);
    B5 = AE_SLAI64(B5, 1);
    B6 = AE_SLAI64(B6, 1);
    B7 = AE_SLAI64(B7, 1);

    z0 = AE_ROUND32X2F48SASYM(B0, B1);
    z1 = AE_ROUND32X2F48SASYM(B2, B3);
    z2 = AE_ROUND32X2F48SASYM(B4, B5);
    z3 = AE_ROUND32X2F48SASYM(B6, B7);
    z4 = AE_ROUND32X2F48SASYM(B8, B8);

    AE_SA32X2_IP(z0, ra, pr);
    AE_SA32X2_IP(z1, ra, pr);
    AE_SA32X2_IP(z2, ra, pr);
    AE_SA32X2_IP(z3, ra, pr);
    AE_SA64POS_FP(ra, pr);
    AE_S32_L_IP(z4, castxcc(ae_int32, pr), 4);
  }
} /* q2rot_32x32() */

