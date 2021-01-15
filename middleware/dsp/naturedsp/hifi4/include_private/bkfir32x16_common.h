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
#ifndef BKFIR32X16_COMMON_H__
#define BKFIR32X16_COMMON_H__

/* Portable data types. */
#include "NatureDSP_types.h"
#include "common.h"

#define BKFIR32X16_MAGIC     0xbe4eaf30

struct tag_bkfir32x16_t;
typedef void (*fnbkfir32x16_process)( struct tag_bkfir32x16_t* _bkfir, int32_t *  y,const int32_t *  x, int N);

/* Filter instance structure. */
typedef struct tag_bkfir32x16_t
{
  uint32_t        magic;     // Instance pointer validation number
  int             M;         // Number of filter coefficients
  const int16_t * coef;      // M filter coefficients, aligned
  int32_t *       delayLine; // Delay line for samples, aligned
  int             delayLen;  // Delay line length, in samples
  int             wrIx;      // Index of the oldest sample
  fnbkfir32x16_process fnprocess;
} 
bkfir32x16_t, *bkfir32x16_ptr_t;

void bkfir32x16_process4( struct tag_bkfir32x16_t* _bkfir, int32_t * restrict  y,const int32_t * restrict  x, int N);
void bkfir32x16_processSmall( struct tag_bkfir32x16_t* _bkfir, int32_t * restrict  y,const int32_t * restrict  x, int N);
void bkfir32x16_processBig( struct tag_bkfir32x16_t* _bkfir, int32_t * restrict  y,const int32_t * restrict  x, int N);
#endif
