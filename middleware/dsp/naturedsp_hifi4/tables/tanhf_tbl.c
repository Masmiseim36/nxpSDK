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
    tables for tanhf() approximation
*/

#include "NatureDSP_types.h"
#include "tanhf_tbl.h"

/* polynomial approximation of tanh(x) in range [-log(3)/2...-log(3)/2]
    only odd coefficients are non zero
    s=pow2(2,-16);
    x=[s:s:log(3)/2+0.008]; x=[-x(end:-1:1) x];
    y=tanh(x); z=tanh(x)./x;
    p=polyfit(x,z,8);
    p=p(1:2:end); p(end)=[];
*/
const union ufloat32uint32 ALIGN(8) polytanhf_tbl[]=
{
    {0x3c86a7d1UL},/* 1.6437442973e-002*/
    {0xbd57b3abUL},/*-5.2661579102e-002*/
    {0x3e086615UL},/* 1.3320191205e-001*/
    {0xbeaaaa0fUL} /*-3.3332869411e-001*/
};

const union ufloat32uint32 halfln3={0x3f0c9f54UL} ; /* log(3)/2 - tanh(log(3)/2)==0.5 */
