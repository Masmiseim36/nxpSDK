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
#ifndef TANFTBL_H__
#define TANFTBL_H__

#include "NatureDSP_types.h"
#include "common.h"

#define TANF_ALG 0 /* 0 - 2 ULP, 1 - 1 ULP code, 2 - slightly simplified 1 ULP code */

externC const float32_t tanf_maxval; /* domain ranges - zero outside */
/*
    tangent polynomial in range -pi/4...pi/4
    Matlab code:
    POLYTAN_LEN=7;
    s=pow2(1,-16); x=(s:s:pi/4); x=[-x(end:-1:1) x];
    y=tan(x)./x;
    p=polyfit(x,y,POLYTAN_LEN*2);
    p=p(1:2:end); p(end)=[];
*/
#define POLYTAN_LEN 7
externC const union ufloat32uint32 polytanf_tbl[];

#if   TANF_ALG==0
#elif (TANF_ALG==1) || (TANF_ALG==2)
externC const union ufloat32uint32 polyinvcosf_tbl[];
#else
#error wrong TANF_ALG
#endif

#endif /* TANFTBL_H__ */
