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
    tables for asinf(x) approximation
*/

#include "NatureDSP_types.h"
#include "asinf_tbl.h"
#include "common.h"

#if ASINF_ALG==0 /* for 2 ULP variant */
/* 
   polynomial coefficients for asin(x)/x-1 in range 0...0.5 

   derived by MATLAB code:
   x=(0:pow2(1,-16):0.5); x(1)=[]; x=[-x(end:-1:1) x];
   z=asin(x)./x-1;
   p=polyfit(x,z,11);
   p=p(2:2:end);

*/
const union ufloat32uint32 ALIGN(8) asinftbl[5]=
{
    {0x3d292d32},/*4.130286700171000e-002f, */
    {0x3cc9c38e},/*2.462938009620908e-002f, */
    {0x3d39f099},/*4.539546841135936e-002f, */
    {0x3d9982fc},/*7.495686502997100e-002f, */
    {0x3e2aaae6} /*1.666675521712198e-001f  */
};
#else
#error wrong ASINF_ALG
#endif
