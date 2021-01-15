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
#ifndef STEREO_BKFIR32X32_COMMON_H__
#define STEREO_BKFIR32X32_COMMON_H__

/* Portable data types. */
#include "NatureDSP_types.h"
#include "common.h"

#define STEREO_BKFIR32X32_MAGIC    0xa9c373b8

/* Filter instance structure. */
typedef struct tag_stereo_bkfir32x32_t
{
  uint32_t        magic;          // Instance pointer validation number
  int             M;              // Number of filter coefficients
  const int32_t * coefLeft;       // M filter coefficients
  const int32_t * coefRight;      // M filter coefficients
  int32_t *       delayLineLeft;  // Delay line for samples
  int32_t *       delayLineRight; // Delay line for samples
  int             delayLen;       // Delay line length, in samples
  int             wrIx;           // Index of the oldest sample
  int32_t*        delayPosLeft;
  int32_t*        delayPosRight;
} stereo_bkfir32x32_t, *stereo_bkfir32x32_ptr_t;

#endif
