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
/*
  NatureDSP Signal Processing Library. IIR part
    Lattice Block Real IIR, floating point
*/

#include "latrf_common.h"


#if (XCHAL_HAVE_FUSION_FP)

void latrf_process3(struct tag_latrf_t *latr, 
                    float32_t * restrict     r,
                    const float32_t*         x, int N )
{
          float32_t * restrict delLine;
    const float32_t * restrict coef;

    xtfloat d0,d1,d2,c0,c1,c2;
    xtfloat t0,t1;
    xtfloat scale;

    int n;

    NASSERT(latr->M==3);
    delLine = latr->delayLine;
    coef    = latr->coef;
    scale   = latr->scale;
    d2=delLine[2];
    d1=delLine[1];
    d0=delLine[0];
    c0=coef[0];
    c1=coef[1];
    c2=coef[2];

    for ( n=0; n<N; n++ )
    {
        t0 = t1 = XT_MUL_S(x[n], scale);
        XT_MSUB_S(t1, d1, c1);
        XT_MSUB_S(t1, d2, c2);
        
        XT_MSUB_S(t0, d0, c0);
        XT_MSUB_S(t0, d1, c1);
        XT_MSUB_S(t0, d2, c2);

        d2 = d1;
        d1 = d0;
        d0 = t0;
        XT_MADD_S(d2, t1, c1);
        XT_MADD_S(d1, t0, c0);
        r[n] = t0;
    }
    delLine[2]=d2;
    delLine[1]=d1;
    delLine[0]=d0;

} // latrf3_process()

#endif
