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
/*
  NatureDSP Signal Processing Library. IIR part
    Lattice Block Real IIR, floating point
  IntegrIT, 2006-2015
*/
#ifndef LATRF_COMMON_H
#define LATRF_COMMON_H

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common utility and macros declarations. */
#include "common.h"

struct tag_latrf_t;
typedef void (latrf_processFxns)(struct tag_latrf_t *handle, float32_t * restrict     r,const float32_t*         x, int N );

/* Filter instance structure. */
typedef struct tag_latrf_t
{
  uint32_t           magic;     /* Instance pointer validation number */
  int                M;         /* Lattice filter order               */
  float32_t          scale;     /* Input signal prescaling factor     */
  const float32_t*   coef;      /* M reflection coefficients          */
  float32_t*         delayLine; /* M delay elements                   */
  latrf_processFxns*  fxns;      /* processing function                */

} latrf_t, *latrf_ptr_t;

void latrf_process1(struct tag_latrf_t *latr, float32_t * restrict r,const float32_t*  x, int N );
void latrf_process2(struct tag_latrf_t *latr, float32_t * restrict r,const float32_t*  x, int N );
void latrf_process3(struct tag_latrf_t *latr, float32_t * restrict r,const float32_t*  x, int N );
void latrf_process4(struct tag_latrf_t *latr, float32_t * restrict r,const float32_t*  x, int N );
void latrf_process5(struct tag_latrf_t *latr, float32_t * restrict r,const float32_t*  x, int N );
void latrf_process6(struct tag_latrf_t *latr, float32_t * restrict r,const float32_t*  x, int N );
void latrf_process7(struct tag_latrf_t *latr, float32_t * restrict r,const float32_t*  x, int N );
void latrf_process8(struct tag_latrf_t *latr, float32_t * restrict r,const float32_t*  x, int N );
void latrf_processX(struct tag_latrf_t *latr, float32_t * restrict r,const float32_t*  x, int N );

#endif
