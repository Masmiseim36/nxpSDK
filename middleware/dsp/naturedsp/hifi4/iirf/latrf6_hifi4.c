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

    M=6
  IntegrIT, 2006-2018
*/
#include "latrf_common.h"
#include "common_fpu.h"

#if (HAVE_VFPU || HAVE_FPU)
void latrf_process6(struct tag_latrf_t *latr, 
                    float32_t  *  restrict    r,
                    const float32_t*         x, int N )
{
        float32_t * restrict  delLine;
    const float32_t * restrict  coef;

    float32_t t0;
    float32_t scale;

    int n;

    NASSERT(latr->M==6);
    delLine = latr->delayLine;
    coef    = latr->coef;
    scale   = latr->scale;
    __Pragma("loop_count min=1");
    for (n=0; n<N; n++)
    {
        t0 = x[n]*scale;
        t0 -= delLine[6-1]*coef[6-1];
        t0 -=         delLine[4]       * coef[4];
        delLine[4+1]= delLine[4] +  t0 * coef[4];
        t0 -=         delLine[3]       * coef[3];
        delLine[3+1]= delLine[3] +  t0 * coef[3];
        t0 -=         delLine[2]       * coef[2];
        delLine[2+1]= delLine[2] +  t0 * coef[2];
        t0 -=         delLine[1]       * coef[1];
        delLine[1+1]= delLine[1] +  t0 * coef[1];
        t0 -=         delLine[0]       * coef[0];
        delLine[0+1]= delLine[0] +  t0 * coef[0];
        delLine[0] = t0;
        r[n] = t0;
    }
} // latrf6_process()
#endif
