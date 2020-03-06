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
#ifndef BQRIIR32X32_DF2_COMMON_H__
#define BQRIIR32X32_DF2_COMMON_H__

/*
    NatureDSP Signal Processing Library. IIR part
    Biquad Real Block IIR, 32x32-bit, Direct Form II
    Internal structures
    IntegrIT, 2006-2018
*/
/* Portable data types. */
#include "NatureDSP_types.h"
/* Common utility and macros declarations. */
#include "common.h"

/* Instance pointer validation number. */
#define MAGIC     0xb6766c94
#define USE_INTEGER_32X32   1 // if 1, code uses integer 32x32 multiplies instead of legacy Q31xQ31->Q47 multiplies

/* Filter instance structure. */
typedef struct tag_bqriir32x32_df2_t
{
  uint32_t        magic;     // Instance pointer validation number
  int             M;         // Number of sections
  int16_t         gain;      // Gain shift amount applied after the last section
  const int32_t*  coef_gsos; // gain+Num/den coefs for each of M biquads, Q30
  int32_t *       state;     // 2 state elements per section, Q31

} bqriir32x32_df2_t, *bqriir32x32_df2_ptr_t;

#endif
