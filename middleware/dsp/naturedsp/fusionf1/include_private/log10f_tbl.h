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
#ifndef LOG10F_TBL_H__
#define LOG10F_TBL_H__
#include "NatureDSP_types.h"
#include "common.h"

#define LOG10F_ALG 0    /* 0 - 2 ULP code, 1 - 1 ULP code */

#if LOG10F_ALG==0
#define LOGF10_TBL_ORDER 9   /* 9 or 10 */
externC const union ufloat32uint32 ALIGN(8) log10f_tbl[LOGF10_TBL_ORDER];
externC const union ufloat32uint32 log10_2;
externC const union ufloat32uint32 _4log10_2minus1;    /* 4*log10(2)-1 */

#elif LOG10F_ALG==1
externC const int32_t ALIGN(8) log10f_tbl_q31[];
externC const int32_t log10_2_q32;                 /* log10(2),Q32 */
#else
#error wrong LOG10F_ALG
#endif

#endif /* LOG10F_TBL_H__ */
