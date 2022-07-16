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
#include "logq23_tbl.h"
#include "common.h"

/*
 polynomial coefficients for ln(x)/(1-x) 
 derived by MATLAB code:
 x=(sqrt(0.5):pow2(1,-16):sqrt(2));
 z=1-x;
 y=log(x)./z;
 p=polyfit(z,y,4);
 p(end)=[];
 fprintf(1,'%d ',round(pow2(p,23)));
*/
const int32_t logq23_tbl[]=
{-1892539,-2943799,-4208767,
     5931642, /* sqrt(2), q22 */
     378194 , /* log2(e), Q18 */
     5814540, /* log(2), Q23  */
     28462,   /* log10(e), Q16*/
     2525223  /* log10(2), Q23*/
};
