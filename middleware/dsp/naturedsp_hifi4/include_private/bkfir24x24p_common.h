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
/*          Copyright (C) 2015-2016 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
#ifndef BKFIR24X24P_COMMON_H__
#define BKFIR24X24P_COMMON_H__

/* Portable data types. */
#include "NatureDSP_types.h"
#include "common.h"

#define BKFIR24X24P_MAGIC     0x8e629e4c

/*
 * Helper data types and utilities for handling packed 24-bit data.
 */

typedef struct tag_f24p
{
  uint8_t b0; // Bits 7..0
  uint8_t b1; // Bits 15..8
  int8_t  b2; // Bits 23..16

} f24p;

#define sz_f24p   sizeof(f24p)

/* Filter instance structure. */
typedef struct tag_bkfir24x24p_t
{
  uint32_t     magic;     // Instance pointer validation number
  int          M;         // Number of filter coefficients
  const f24p * coef;      // M filter coefficients, reverted
  f24p *       delayLine; // Delay line for samples
  int          delayLen;  // Delay line length, in samples
  uintptr_t    wrIx;      // Index of the oldest sample (in bytes)

} bkfir24x24p_t, *bkfir24x24p_ptr_t;

#endif
