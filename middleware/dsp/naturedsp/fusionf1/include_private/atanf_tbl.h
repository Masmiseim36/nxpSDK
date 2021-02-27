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
    tables for atanf(x) approximation
*/
#ifndef ATANFTBL_H__
#define ATANFTBL_H__

#include "NatureDSP_types.h"
#include "common.h"

#define ATANF_ALG 0     /* 0 - 2 ULP code, 1 - 1 ULP code */

externC const union ufloat32uint32 atanftbl1[8]; 
externC const union ufloat32uint32 atanftbl2[8]; 

#if ATANF_ALG==0
#elif ATANF_ALG==1
externC const union ufloat32uint32 atanftbl1a[8]; 
externC const union ufloat32uint32 atanftbl2a[8]; 
#else
#error wrong ATANF_ALG
#endif

#endif /* ATANFTBL_H__ */
