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
    tables for sin(pi/2*x) approximation
*/
#ifndef SINFTBL_H__
#define SINFTBL_H__

#include "NatureDSP_types.h"
#include "common.h"

#define SINNCOSF_ALG 0 /* 0 - 2 ULP, 1 - 1 ULP */

externC const union ufloat32uint32 sinf_maxval; /* domain ranges - zero outside */

/* pi/4 represented as a sum of exacly represented numbers.
    derived from hex value of pi: 3.243F6A8885A308D313198A2E037073
*/
externC const union ufloat64uint64 ALIGN(8) pi4fc[];

/* 
   polynomial coefficients for sin(x)/x, [-pi/4...pi/4]
   derived by MATLAB code:
   s=pow2(1,-16); x=(s:s:pi/4); x=[-x(end:-1:1) x];
   y=sin(x)./x;
   p=polyfit(x,y,6); p=p(1:2:end); p(end)=[];
*/
externC const union ufloat32uint32 ALIGN(8) polysinf_tbl[];

/* 
   polynomial coefficients for cos(x), [-pi/4...pi/4]
   derived by MATLAB code:
   s=pow2(1,-16); x=(s:s:pi/4); x=[-x(end:-1:1) x];
   y=cos(x);
   p=polyfit(x,y,6); p=p(1:2:end); p(end)=[];
*/
externC const union ufloat32uint32 ALIGN(8) polycosf_tbl[];

#endif /* SINFTBL_H__ */
