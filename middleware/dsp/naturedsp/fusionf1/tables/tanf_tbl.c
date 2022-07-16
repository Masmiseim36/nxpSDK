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
    tables for tanf/cotanf approximation
*/

#include "NatureDSP_types.h"
#include "tanf_tbl.h"

const float32_t tanf_maxval=9099.f; /* domain ranges - zero outside */

/*
    tangent polynomial in range -pi/4...pi/4
    Matlab code:
    POLYTAN_LEN=7;
    s=pow2(1,-16); x=(s:s:pi/4); x=[-x(end:-1:1) x];
    y=tan(x)./x;
    p=polyfit(x,y,POLYTAN_LEN*2);
    p=p(1:2:end); p(end)=[];
*/
const union ufloat32uint32 ALIGN(8) polytanf_tbl[POLYTAN_LEN +1]=
#if POLYTAN_LEN==7
    {
        {0x3b8beb97}, /* 4.2700276868e-003*/
        {0x39996b33}, /* 2.9262304874e-004*/
        {0x3c2f1d4e}, /* 1.0688139527e-002*/
        {0x3caec172}, /* 2.1332475657e-002*/
        {0x3d5d65cf}, /* 5.4052168993e-002*/
        {0x3e0886df}, /* 1.3332699106e-001*/
        {0x3eaaaab1}  /* 3.3333351374e-001*/
    };
#else 
#error unsupported
#endif

#if   TANF_ALG==0
#elif TANF_ALG==1
/*
    inverse cosine polynomial in range -pi/4...pi/4
    Matlab code:
    s=pow2(1,-16); x=(s:s:pi/4); x=[-x(end:-1:1) x];
    y=1./cos(x).^2;
    p=polyfit(x,y,8);
    p=p(1:2:end); p(end)=[];
*/
const union ufloat32uint32 ALIGN_PDX_4M polyinvcosf_tbl[]=
{
    {0x3ee2cb7b}, /* 4.4295868930e-001*/
    {0x3e6cc20e}, /* 2.3120900533e-001*/
    {0x3f330566}, /* 6.9930114506e-001*/
    {0x3f7f6033}, /* 9.9756164722e-001*/
    {0x3f8000ed}  /* 1.0000282621e+000*/
};
#elif TANF_ALG==2
/*
    Same as above, with reduced polynomial order.
*/
const union ufloat32uint32 ALIGN_PDX_4M polyinvcosf_tbl[]=
{
    {0x3fbf1f49}, /* 1.493142222583e+000 */
    {0x3f775a75}  /* 9.662240338840e-001 */
};
#else
#error wrong TANF_ALG
#endif
