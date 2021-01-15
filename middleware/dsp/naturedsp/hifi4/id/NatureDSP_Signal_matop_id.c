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
 * Matrix Operations
 * Annotations
*/

#include "NatureDSP_types.h"
#include "NatureDSP_Signal_matop.h"
#include "common.h"

ANNOTATE_FUN(mtx_mpy24x24,      "Matrix Multiply (24-bit data)");
ANNOTATE_FUN(mtx_mpy32x32,      "Matrix Multiply (32-bit data)");
ANNOTATE_FUN(mtx_mpy16x16,      "Matrix Multiply (16-bit data)");
ANNOTATE_FUN(mtx_mpyf,          "Matrix Multiply (floating point data)");
ANNOTATE_FUN(mtx_mpy24x24_fast, "Fast Matrix Multiply (24-bit data)");
ANNOTATE_FUN(mtx_mpy32x32_fast, "Fast Matrix Multiply (32-bit data)");
ANNOTATE_FUN(mtx_mpy16x16_fast, "Fast Matrix Multiply (16-bit data)");
ANNOTATE_FUN(mtx_mpyf_fast,     "Fast Matrix Multiply (floating point data)");
ANNOTATE_FUN(mtx_mpyt32x32,      "Matrix Multiply Transpose (32-bit data)");
ANNOTATE_FUN(mtx_mpyt16x16,      "Matrix Multiply Transpose (16-bit data)");
ANNOTATE_FUN(mtx_mpytf,          "Matrix Multiply Transpose (floating point data)");
ANNOTATE_FUN(mtx_mpyt32x32_fast, "Fast Matrix Multiply Transpose (32-bit data)");
ANNOTATE_FUN(mtx_mpyt16x16_fast, "Fast Matrix Multiply Transpose (16-bit data)");
ANNOTATE_FUN(mtx_mpytf_fast,     "Fast Matrix Multiply Transpose (floating point data)");
ANNOTATE_FUN(mtx_vecmpy32x32,       "Matrix by Vector Multiply (32-bit data)");
ANNOTATE_FUN(mtx_vecmpy24x24,       "Matrix by Vector Multiply (24-bit data)");
ANNOTATE_FUN(mtx_vecmpy16x16,       "Matrix by Vector Multiply (16-bit data)");
ANNOTATE_FUN(mtx_vecmpy32x32_fast,  "Fast Matrix by Vector Multiply (32-bit data)");
ANNOTATE_FUN(mtx_vecmpy24x24_fast,  "Fast Matrix by Vector Multiply (24-bit data)");
ANNOTATE_FUN(mtx_vecmpy16x16_fast,  "Fast Matrix by Vector Multiply (16-bit data)");
ANNOTATE_FUN(mtx_vecmpyf,           "Matrix by Vector Multiply (floating point data)");
ANNOTATE_FUN(mtx_vecmpyf_fast,      "Fast Matrix by Vector Multiply (floating point data)");

ANNOTATE_FUN(mtx_transpose16x16, "Matrix transpose (16-bit data)");
ANNOTATE_FUN(mtx_transpose16x16_fast, "Fast Matrix transpose (16-bit data)");
ANNOTATE_FUN(mtx_transpose32x32, "Matrix transpose (32-bit data)");
ANNOTATE_FUN(mtx_transpose32x32_fast, "Fast Matrix transpose (32-bit data)");
ANNOTATE_FUN(mtx_transpose8x8, "Matrix transpose (8-bit data)");
ANNOTATE_FUN(mtx_transpose8x8_fast, "Fast Matrix transpose (8-bit data)");
ANNOTATE_FUN(mtx_transposef, "Matrix transpose (floating point data)");
ANNOTATE_FUN(mtx_transposef_fast, "Fast Matrix transpose (floating point data)");
ANNOTATE_FUN(mtx_vecmpy8x8,       "Matrix by Vector Multiply (8-bit data)");
ANNOTATE_FUN(mtx_vecmpy8x8_fast,       "Fast Matrix by Vector Multiply (8-bit data)");
ANNOTATE_FUN(mtx_mpy8x8,      "Matrix Multiply (8-bit data)");
ANNOTATE_FUN(mtx_mpy8x8_fast, "Fast Matrix Multiply (8-bit data)");
ANNOTATE_FUN(mtx_mpyt8x8,      "Matrix Multiply Transpose (8-bit data)");
ANNOTATE_FUN(mtx_mpyt8x8_fast, "Fast Matrix Multiply Transpose (8-bit data)");
ANNOTATE_FUN(mtx_vecmpy8x16,       "Matrix by Vector Multiply (8/16-bit data)");
ANNOTATE_FUN(mtx_vecmpy8x16_fast,       "Fast Matrix by Vector Multiply (8/16-bit data)");
ANNOTATE_FUN(mtx_mpy8x16,      "Matrix Multiply (8/16-bit data)");
ANNOTATE_FUN(mtx_mpy8x16_fast, "Fast Matrix Multiply (8/16-bit data)");
ANNOTATE_FUN(mtx_mpyt8x16,      "Matrix Multiply Transpose (8/16-bit data)");
ANNOTATE_FUN(mtx_mpyt8x16_fast, "Fast Matrix Multiply Transpose (8/16-bit data)");
