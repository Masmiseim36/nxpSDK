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
    tables for lognf(x) approximation
*/

#include "NatureDSP_types.h"
#include "lognf_tbl.h"
#include "common.h"
/*
  polynomial coefficients for ln(x) / (1 - x)
  derived by MATLAB code :
  x = (sqrt(0.5) : pow2(1, -16) : sqrt(2));
  z = 1 - x;
  y = log(x). / z;
  p = polyfit(z, y, 8);

  last coefficient is omitted because it is equal to one
*/
const union ufloat32uint32 ALIGN(8) lognf_tbl[]=
{
  { 0xbdb0cd23 },/*-8.6328767986e-002*/
  { 0xbe115eaa },/*-1.4196267345e-001*/
  { 0xbe18aa2f },/*-1.4908670611e-001*/
  { 0xbe29cfe9 },/*-1.6583217816e-001*/
  { 0xbe4c6bcf },/*-1.9963000763e-001*/
  { 0xbe8001c7 },/*-2.5001356171e-001*/
  { 0xbeaaab8a },/*-3.3333998118e-001*/
  { 0xbefffffe } /*-4.9999994403e-001*/
};

const union ufloat32uint32 ln2 = { 0x3f317218 }; /* ln(2) */
