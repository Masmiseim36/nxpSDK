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
    tables for log10f(x) approximation
*/

#include "NatureDSP_types.h"
#include "log10f_tbl.h"
#include "common.h"

#if LOG10F_ALG==0
/* 
   polynomial coefficients for log10(x)/(1-x) 
   derived by MATLAB code:
   x=(sqrt(0.5):pow2(1,-16):sqrt(2));
   z=1-x;
   y=log10(x)./z;
   p=polyfit(z,y,LOGF10_TBL_ORDER-1);

*/
const union ufloat32uint32 ALIGN(8) log10f_tbl[LOGF10_TBL_ORDER]=
#if LOGF10_TBL_ORDER==8
{
    {0xbd3204bf},
    {0xbd8e9c2c},
    {0xbd990ebd},
    {0xbdb12e8b},
    {0xbdde2240},
    {0xbe143e8c},
    {0xbe5e5c29},
    {0xbede5bd8}
};
#elif LOGF10_TBL_ORDER==9
{
    {0xbd199153},/*-3.7492107494e-002*/
    {0xbd7c887e},/*-6.1653605702e-002*/
    {0xbd849a5b},/*-6.4747533825e-002*/
    {0xbd937f39},/*-7.2019999908e-002*/
    {0xbdb18ed5},/*-8.6698210732e-002*/
    {0xbdde5eef},/*-1.0857951025e-001*/
    {0xbe143dfd},/*-1.4476771442e-001*/
    {0xbe5e5bd7},/*-2.1714721664e-001*/
    {0xbede5bd8} /*-4.3429447406e-001*/
};
#elif LOGF10_TBL_ORDER==10
{
    {0xbd0621bf},/*-3.2747027244e-002*/
    {0xbd62c97c},/*-5.5367933294e-002*/
    {0xbd6aca9f},/*-5.7322140672e-002*/
    {0xbd7ca532},/*-6.1680979899e-002*/
    {0xbd93cb10},/*-7.2164655217e-002*/
    {0xbdb1e5b6},/*-8.6863922938e-002*/
    {0xbdde5ebd},/*-1.0857913376e-001*/
    {0xbe143d3c},/*-1.4476483747e-001*/
    {0xbe5e5bd6},/*-2.1714720390e-001*/
    {0xbede5bd9} /*-4.3429448180e-001*/
};
#else
#error wrong LOGF10_TBL_ORDER
#endif

const union ufloat32uint32 log10_2={0x3e9a209b};
const union ufloat32uint32 _4log10_2minus1={0x3e5104d4};    /* 4*log10(2)-1 */
#elif LOG10F_ALG==1
/*
    polynomial coefficients for lg(x)/(1-x) 
    derived by MATLAB code:

    x=(sqrt(0.5):pow2(1,-16):sqrt(2));
    z=1-x;
    y=log10(x)./z;
    p=polyfit(z,y,9);
    p(end)=p(end)+1;
    fprintf(1,'%d ',round(pow2(p,31)));
*/
const int32_t ALIGN_PDX_4M log10f_tbl_q31[]= 
{   -70323706,
   -118901731,
   -123098360,
   -132458896,
   -154972417,
   -186538854,
   -233171914,
   -310880121,
   -466320070,
   1214843350 
};

const int32_t log10_2_q32=1292913986L;  /* log10(2),Q32 */
#else
#error wrong LOG10F_ALG
#endif
