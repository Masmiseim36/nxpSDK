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
#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(void, q2rotf, (float32_t *r, const float32_t *q, int L))
#else
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
void q2rotf(float32_t *r, const float32_t *q, int L)
{
        xtfloat    *  pr = (xtfloat      *)r;
        xtfloat    *  pr_ = (xtfloat      *)(r+1);
  const xtfloatx2    *  restrict pq = (const xtfloatx2      *)(q);
  int l;
  xtfloatx2 q0, q1;
  xtfloat A0, A1, A2, q02, q12;
  xtfloat B0, B1, B2, B3, B4, B5, B6, B7, B8;
  ae_valign qa;
  qa = AE_LA64_PP(pq);
  XT_LASX2IP(q0, qa, pq);
  XT_LASX2IP(q1, qa, pq);
  A0 = XT_MUL_LHH_S(q0, q0); //q[0] * q[0]
  A1 = A0;
  XT_MSUB_LLL_S(A1, q1, q1);//q[0] * q[0] - q[3] * q[3]
  A2 = XT_MUL_LLL_S(q0, q0); //q[1] * q[1]
  XT_MSUB_LHH_S(A2, q1, q1);//q[1] * q[1] - q[2] * q[2]
  B0 = XT_ADD_S(A1, A2);//r[0]
  B4 = XT_SUB_S(A1, A2);//r[4]
  XT_MSUB_LLL_S(A0, q0, q0);
  XT_MSUB_LHH_S(A0, q1, q1);
  XT_MADD_LLL_S(A0, q1, q1);
  B8 = A0; //r[8]
  for (l = 0; l<(L); l++)
  {
    XT_SSIP(B0, pr, 4 * 4);
    XT_SSIP(B4, pr, 4 * 4);
    XT_SSIP(B8, pr, 4);

    q02 = XT_ADD_LHH_S(q0, q0);
    q12 = XT_ADD_LLL_S(q0, q0);

    A0 = XT_MUL_S(q12,XT_HIGH_S(q1)); //2 * q[1] * q[2]
    B1 = A0; B3 = A0;
    XT_MADD_S(B1,q02, q1); //[r1]
    XT_MSUB_S(B3,q02, q1); //[r3]
    
    A1 = XT_MUL_S(q12,q1); //2 * q[1] * q[3]
    B2 = A1; B6 = A1;
    XT_MSUB_S(B2, q02, XT_HIGH_S(q1)); //[r2]
    XT_MADD_S(B6, q02, XT_HIGH_S(q1)); //[r6]
    
    A2 = XT_MUL_LHL_S(q1, q1); //q[2] * q[3]
    B5 = A2; B7 = A2;
    XT_MADD_LHL_S(B5, q0, q0);
    XT_MSUB_LHL_S(B7, q0, q0);
    B5 = XT_ADD_S(B5, B5);
    B7 = XT_ADD_S(B7, B7);

    
    XT_SSIP(B1, pr_, 4);
    XT_SSIP(B2, pr_, 4);
    XT_SSIP(B3, pr_, 2*4);
    XT_SSIP(B5, pr_, 4);
    XT_SSIP(B6, pr_, 4);
    XT_SSIP(B7, pr_, 3*4);

    XT_LASX2IP(q0, qa, pq);
    XT_LASX2IP(q1, qa, pq);
    A0 = XT_MUL_LHH_S(q0, q0); //q[0] * q[0]
    A1 = A0;
    XT_MSUB_LLL_S(A1, q1, q1);//q[0] * q[0] - q[3] * q[3]
    A2 = XT_MUL_LLL_S(q0, q0); //q[1] * q[1]
    XT_MSUB_LHH_S(A2, q1, q1);//q[1] * q[1] - q[2] * q[2]
    B0 = XT_ADD_S(A1, A2);//r[0]
    B4 = XT_SUB_S(A1, A2);//r[4]
    XT_MSUB_LLL_S(A0, q0, q0);
    XT_MSUB_LHH_S(A0, q1, q1);
    XT_MADD_LLL_S(A0, q1, q1);
    B8 = A0; //r[8]
  }
} /* q2rotf() */
#endif

