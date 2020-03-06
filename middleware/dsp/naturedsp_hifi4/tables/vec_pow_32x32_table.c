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
  NatureDSP Signal Processing Library. Vector mathematics
    Vector operations
    tables
  IntegrIT, 2006-2018
*/

#include "NatureDSP_types.h"
#include "vec_pow_32x32_table.h"

#include "common.h"
/*
 polynomial coefficients for ln(x)/(1-x), Q63
 x=(sqrt(0.5):pow2(1,-16):sqrt(2));
 z=1-x;
 y=log(x)./z;
 p=polyfit(z,y,13);
*/
const int32_t vec_pow_32x32_polypow2[]=
{  4026,46486,467803,4049374,29210263,168566008,729568022,  // Q32
        2105088334/2, // Q31
        1518500250};  // plus 2^31, Q31 

/*
 Matlab code for computing the polynomial:
 x=(sqrt(0.5):pow2(1,-16):sqrt(2));
 z=1-x;
 y=log(x)./z;
 p=polyfit(z,y,14);
*/
const int64_t vec_pow_32x32_polylogQ63[]=
{
    -399027437979318848,  -779485700671626368,  -801942930280481664,  -766430233539746176, 
    -826891569616276736,  -921386303912791040, -1025502463045125120, -1153001634696212992, 
    -1317604130860931584,-1537226204247329792, -1844674701694722048, -2305843038734005248,
    -3074457343986102784,-4611686018332004352,
    6653256548922159104 // log2(e), Q62
};
