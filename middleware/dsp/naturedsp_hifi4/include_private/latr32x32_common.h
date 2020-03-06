/* ------------------------------------------------------------------------ */
/* Copyright (c) 2018 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
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
/*          Copyright (C) 2015-2018 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */

/*
  NatureDSP Signal Processing Library. IIR part
    Lattice Block Real IIR, 32x32-bit
    internal data structures
  IntegrIT, 2006-2018
*/
#ifndef LATR32X32_COMMON_H__
#define LATR32X32_COMMON_H__
#include "NatureDSP_types.h"
#include "common.h"
/* Lattice filter data processing function. */
typedef void (proc_fxn_t)( int32_t * restrict r,     // r[N]     [out   ] Q31
                     const int32_t * restrict x,     // x[N]     [in    ] Q31
                           int32_t *          dline, // dline[M] [in/out] Q30
                     const int32_t *          coef,  // coef[M]  [in    ] Q31
                     int32_t scale, int N, int M );  // scale    [in    ] Q31

proc_fxn_t latr32x32_1_proc;
proc_fxn_t latr32x32_2_proc;
proc_fxn_t latr32x32_3_proc;
proc_fxn_t latr32x32_4_proc;
proc_fxn_t latr32x32_5_proc;
proc_fxn_t latr32x32_6_proc;
proc_fxn_t latr32x32_7_proc;
proc_fxn_t latr32x32_8_proc;
proc_fxn_t latr32x32_X_proc;

#endif
