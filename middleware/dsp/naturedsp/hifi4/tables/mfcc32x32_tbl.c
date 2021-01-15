/* ------------------------------------------------------------------------ */
/* Copyright (c) 2018 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs ("Cadence    */
/* Libraries") are the copyrighted works of Cadence Design Systems Inc.	    */
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
/*
  NatureDSP Signal Processing Library. Audio processing part
    Compute Mel-Frequency Cepstrum Coefficients (MFCC) from speech signal
    32-bit fixed-point variant
    Data tables
  IntegrIT, 2006-2018
*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Common utility and macros declarations. */
#include "common.h"
/* MFCC extractor internal declarations. */
#include "mfcc_internal.h"

/* Polynomial coeffs for 32-bit sin/cos evaluation.
   P=5;N=8;X=[];Y=[]; Z=[];
   for n=1:N
       x=(n-1)/N+(0:pow2(1,-16):1/N);
       z=sin(x*(pi/2));
       x0=(n-0.5)/N;
       p=polyfit(x-x0,z,P);
       y=polyval(p,x-x0);
       X=[X x];
       Y=[Y y];
       Z=[Z z];
       q=zeros(1,P+1);q(P+1)=1; q=q+30;
       fprintf(1,'%d ',round(pow2(p,q)));fprintf(1,'\n');
   end
 */

const fract32 ALIGN(8) mfcc32x32_polysin_tbl[6*8] = {
      85125697,  81854367,  75437424,  66121459,   54264461,   40322149,   24830263,    8384161, /* Q30 */
      26685776,  79031810, 128340695, 172717520,  210456906,  240108552,  260532962,  270945236, /* Q30 */
    -690258745,-663732489,-611699365,-536158978, -440014301, -326960122, -201341049,  -67984553, /* Q30 */
    -129840926,-384533065,-624447813,-840365383,-1023988182,-1168259689,-1267635632,-1318297048, /* Q30 */
    1678508130,1614004004,1487474609,1303782399, 1069986563,  795071743,  489602762,  165318621, /* Q30 */
     210490206, 623381598,1012316784,1362349204, 1660027308, 1893911494, 2055013723, 2137142927  /* Q31 */
};
