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
    tables for 2^x approximation
*/
#include "NatureDSP_types.h"
#include "pow2f_tbl.h"
#include "common.h"

/*
        polynomial coefficients for 2.^x, x=[-1...0)
        x=(-1:pow2(1,-16):0); y=2.^x; p=polyfit(x,y,6);
*/
const union ufloat32uint32 ALIGN(8) polypow2f_tbl[]= 
{
        {0x38e55635},/*1.0935628112e-004f */
        {0x3aa72691},/*1.2752582519e-003f */
        {0x3c1cf169},/*9.5790408401e-003f */
        {0x3d6349a7},/*5.5490162065e-002f */
        {0x3e75fd4a},/*2.4022402810e-001f */
        {0x3f317215},/*6.9314699859e-001f */
        {0x3f800000} /*9.9999999677e-001f */
};
/*
polynomial coefficients for log2(x):
x=(sqrt(0.5):pow2(1,-16):sqrt(2));
z=1-x;
y=log(x)./z;
p=polyfit(z,y,11);
p(end)=p(end)+1;
num2hex(single(-p))
*/
const union ufloat32uint32 ALIGN(8) log2f_coef[] =
{
  { 0x3d726a49 },
  { 0x3dd91c88 },
  { 0x3ddde76c },
  { 0x3de21e63 },
  { 0x3dfe600b },
  { 0x3e124679 },
  { 0x3e2ab2f1 },
  { 0x3e4ccd1b },
  { 0x3e7fffde },
  { 0x3eaaaaaa },
  { 0x3f000000 },
  { 0x3f800000 },
  /* log2(e) */
  { 0x3fb8aa3b }, /* 1.4426950216      */
  { 0x32a57060 }  /* 1.9259629891e-008 */
};

/* polynomial coefficients for 2^x, x=-0.5...0.5 */
const union ufloat32uint32 ALIGN(8) pow2f_coef[] =
{
  { 0x39222a65 },
  { 0x3aaf931c },
  { 0x3c1d94fc },
  { 0x3d63578a },
  { 0x3e75fdf0 },
  { 0x3f317218 },
  { 0x3f800000 }

 //{ 0x3aaf931b },
 //{ 0x3c1e7220 },
 //{ 0x3d63578a },
 //{ 0x3e75fcc9 },
 //{ 0x3f317218 },
 //{ 0x3f800000 }

};
