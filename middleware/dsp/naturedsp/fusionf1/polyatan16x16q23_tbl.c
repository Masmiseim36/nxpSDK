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
    tables for atan(x) approximation, q23
*/
#include "NatureDSP_types.h"
#include "common.h"
#include "polyatan16x16q23_tbl.h"

/* Matlab code for coefficients 
   x=(-1.1:pow2(1,-15):1.1);
   y=atan(x)/pi;
   p=polyfit(x,y,9);
   plot(x,y-polyval(p,x)); grid on
   p=p(1:2:end);
*/
const int32_t polyatan16x16q23[]=
   {-89499,367732,-847739,2667062,
    /* additional constants */
    12275712, /* coefficient for 1-st approximation of reciprocal */
    0x400000, /* 0.5Q23 */
    0x800000  /* 1.0Q23 */
   };

