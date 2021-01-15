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
#ifndef BQRIIR32X16_DF1_COMMON_H__
#define BQRIIR32X16_DF1_COMMON_H__

/*
    NatureDSP Signal Processing Library. IIR part
    Biquad Real Block IIR, 32x16-bit, Direct Form I
    internal structures
    IntegrIT, 2006-2018
*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Common utility and macros declarations. */
#include "common.h"

/* Instance pointer validation number. */
#define MAGIC     0x914c4a88

/* Filter instance structure. */
typedef struct tag_bqriir32x16_df1_t
{
  uint32_t        magic; // Instance pointer validation number
  int             M;     // Number of sections
  int16_t         gain;  // Total gain shift amount for the last biquad
  const int16_t * coef;  // Num/den coefs (Q14) and gain (Q15) for each biquad
  int32_t *       state; // 4 state elements per section, Q31

} bqriir32x16_df1_t, *bqriir32x16_df1_ptr_t;

#endif
