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
  NatureDSP Signal Processing Library. IIR part
    Lattice Block Real IIR, floating point
    C code optimized for HiFi4

    M=2
  IntegrIT, 2006-2018
*/
#include "latrf_common.h"
#include "common_fpu.h"

#if (HAVE_VFPU || HAVE_FPU)
void latrf_process2(struct tag_latrf_t *latr, 
                    float32_t * restrict     r,
                    const float32_t*         x, int N )
{
    float32_t d0,d1,c0,c1;
          float32_t * restrict delLine;
    const float32_t * restrict coef;

    float32_t t0;
    float32_t scale;

    int n;

    NASSERT(latr->M==2);
    delLine = latr->delayLine;
    coef    = latr->coef;
    scale   = latr->scale;
    d0=delLine[0];
    d1=delLine[1];
    c0=coef[0];
    c1=coef[1];

    for ( n=0; n<N; n++ )
    {
        t0 = x[n]*scale;
        t0 -= d0 * c0;
        t0 -= d1 * c1;
        d1 = d0 + t0 * c0;
        d0 = t0;
        r[n] = t0;
    }
    delLine[0]=d0;
    delLine[1]=d1;
} // latrf2_process()
#endif
