/* ------------------------------------------------------------------------ */
/* Copyright (c) 2018 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs ('Cadence    */
/* Libraries') are the copyrighted works of Cadence Design Systems Inc.     */
/* Cadence IP is licensed for use with Cadence processor cores only and     */
/* must not be used for any other processors and platforms. Your use of the */
/* Cadence Libraries is subject to the terms of the license agreement you   */
/* have entered into with Cadence Design Systems, or a sublicense granted   */
/* to you by a direct Cadence licensee.                                     */
/* ------------------------------------------------------------------------ */
/*  IntegrIT, Ltd.   www.integrIT.com, info@integrIT.com                    */
/*                                                                          */
/* NatureDSP_Signal Library                                               */
/*                                                                          */
/* This library contains copyrighted materials, trade secrets and other     */
/* proprietary information of IntegrIT, Ltd. This software is licensed for  */
/* use with Cadence processor cores only and must not be used for any other */
/* processors and platforms. The license to use these sources was given to  */
/* Cadence, Inc. under Terms and Condition of a Software License Agreement  */
/* between Cadence, Inc. and IntegrIT, Ltd.                                 */
/* ------------------------------------------------------------------------ */
/*          Copyright (C) 2009-2018 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
/*
 * NatureDSP_Signal Library API
 * Matrix Decomposition and Inversion Functions
 * Annotations
 */
#include "NatureDSP_types.h"
#include "NatureDSP_Signal_matinv.h"
#include "common.h"


ANNOTATE_FUN(mtx_inv2x2f,   "Gauss-Jordan matrix inversion (floating point data)");
ANNOTATE_FUN(mtx_inv3x3f,   "Gauss-Jordan matrix inversion (floating point data)");
ANNOTATE_FUN(mtx_inv4x4f,   "Gauss-Jordan matrix inversion (floating point data)");
ANNOTATE_FUN(mtx_inv6x6f,   "Gauss-Jordan matrix inversion (floating point data)");
ANNOTATE_FUN(mtx_inv8x8f,   "Gauss-Jordan matrix inversion (floating point data)");
ANNOTATE_FUN(mtx_inv10x10f, "Gauss-Jordan matrix inversion (floating point data)");

ANNOTATE_FUN(mtx_inv2x2_32x32,   "Gauss-Jordan matrix inversion (32-bit fixed-point data)");
ANNOTATE_FUN(mtx_inv3x3_32x32,   "Gauss-Jordan matrix inversion (32-bit fixed-point data)");
ANNOTATE_FUN(mtx_inv4x4_32x32,   "Gauss-Jordan matrix inversion (32-bit fixed-point data)");
ANNOTATE_FUN(mtx_inv6x6_32x32,   "Gauss-Jordan matrix inversion (32-bit fixed-point data)");
ANNOTATE_FUN(mtx_inv8x8_32x32,   "Gauss-Jordan matrix inversion (32-bit fixed-point data)");
ANNOTATE_FUN(mtx_inv10x10_32x32, "Gauss-Jordan matrix inversion (32-bit fixed-point data)");
ANNOTATE_FUN(cmtx_inv2x2_32x32,   "Gauss-Jordan matrix inversion (complex 32-bit fixed-point data)");
ANNOTATE_FUN(cmtx_inv3x3_32x32,   "Gauss-Jordan matrix inversion (complex 32-bit fixed-point data)");
ANNOTATE_FUN(cmtx_inv4x4_32x32,   "Gauss-Jordan matrix inversion (complex 32-bit fixed-point data)");
ANNOTATE_FUN(cmtx_inv6x6_32x32,   "Gauss-Jordan matrix inversion (complex 32-bit fixed-point data)");
ANNOTATE_FUN(cmtx_inv8x8_32x32,   "Gauss-Jordan matrix inversion (complex 32-bit fixed-point data)");
ANNOTATE_FUN(cmtx_inv10x10_32x32, "Gauss-Jordan matrix inversion (complex 32-bit fixed-point data)");

ANNOTATE_FUN(mtx_gjelim2x2_32x32,   "Gauss-Jordan linear equations solver (32-bit fixed-point data)");
ANNOTATE_FUN(mtx_gjelim3x3_32x32,   "Gauss-Jordan linear equations solver (32-bit fixed-point data)");
ANNOTATE_FUN(mtx_gjelim4x4_32x32,   "Gauss-Jordan linear equations solver (32-bit fixed-point data)");
ANNOTATE_FUN(mtx_gjelim6x6_32x32,   "Gauss-Jordan linear equations solver (32-bit fixed-point data)");
ANNOTATE_FUN(mtx_gjelim8x8_32x32,   "Gauss-Jordan linear equations solver (32-bit fixed-point data)");
ANNOTATE_FUN(mtx_gjelim10x10_32x32, "Gauss-Jordan linear equations solver (32-bit fixed-point data)");
ANNOTATE_FUN(cmtx_gjelim2x2_32x32,  "Gauss-Jordan linear equations solver (complex 32-bit fixed-point data)");
ANNOTATE_FUN(cmtx_gjelim3x3_32x32,  "Gauss-Jordan linear equations solver (complex 32-bit fixed-point data)");
ANNOTATE_FUN(cmtx_gjelim4x4_32x32,  "Gauss-Jordan linear equations solver (complex 32-bit fixed-point data)");
ANNOTATE_FUN(cmtx_gjelim6x6_32x32,  "Gauss-Jordan linear equations solver (complex 32-bit fixed-point data)");
ANNOTATE_FUN(cmtx_gjelim8x8_32x32,  "Gauss-Jordan linear equations solver (complex 32-bit fixed-point data)");
ANNOTATE_FUN(cmtx_gjelim10x10_32x32,"Gauss-Jordan linear equations solver (complex 32-bit fixed-point data)");
