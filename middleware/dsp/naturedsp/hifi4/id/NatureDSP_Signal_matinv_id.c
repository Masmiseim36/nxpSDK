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

ANNOTATE_FUN(cmatcholdecomp4x4f,             "Cholesky decomposition (single matrix complex floating-point data)");
ANNOTATE_FUN(cmatcholdecomp6x6f,             "Cholesky decomposition (single matrix complex floating-point data)");
ANNOTATE_FUN(cmatcholdecomp8x8f,             "Cholesky decomposition (single matrix complex floating-point data)");
ANNOTATE_FUN(cmatcholdecomp10x10f,           "Cholesky decomposition (single matrix complex floating-point data)");
ANNOTATE_FUN(matcholdecomp4x4f,              "Cholesky decomposition (single matrix real floating-point data)");
ANNOTATE_FUN(matcholdecomp6x6f,              "Cholesky decomposition (single matrix real floating-point data)");
ANNOTATE_FUN(matcholdecomp8x8f,              "Cholesky decomposition (single matrix real floating-point data)");
ANNOTATE_FUN(matcholdecomp10x10f,            "Cholesky decomposition (single matrix real floating-point data)");
ANNOTATE_FUN(cmatcholfwdsubst4x4f,          "Cholesky forward substitution (single matrix complex floating-point data)");
ANNOTATE_FUN(cmatcholfwdsubst6x6f,          "Cholesky forward substitution (single matrix complex floating-point data)");
ANNOTATE_FUN(cmatcholfwdsubst8x8f,          "Cholesky forward substitution (single matrix complex floating-point data)");
ANNOTATE_FUN(cmatcholfwdsubst10x10f,        "Cholesky forward substitution (single matrix complex floating-point data)");
ANNOTATE_FUN(matcholfwdsubst4x4f,           "Cholesky forward substitution (single matrix real floating-point data)");
ANNOTATE_FUN(matcholfwdsubst6x6f,           "Cholesky forward substitution (single matrix real floating-point data)");
ANNOTATE_FUN(matcholfwdsubst8x8f,           "Cholesky forward substitution (single matrix real floating-point data)");
ANNOTATE_FUN(matcholfwdsubst10x10f,         "Cholesky forward substitution (single matrix real floating-point data)");
ANNOTATE_FUN(cmatcholbkwsubst4x4f,          "Cholesky back substitution (single matrix complex floating-point data)");
ANNOTATE_FUN(cmatcholbkwsubst6x6f,          "Cholesky back substitution (single matrix complex floating-point data)");
ANNOTATE_FUN(cmatcholbkwsubst8x8f,          "Cholesky back substitution (single matrix complex floating-point data)");
ANNOTATE_FUN(cmatcholbkwsubst10x10f,        "Cholesky back substitution (single matrix complex floating-point data)");
ANNOTATE_FUN(matcholbkwsubst4x4f,           "Cholesky back substitution (single matrix real floating-point data)");
ANNOTATE_FUN(matcholbkwsubst6x6f,           "Cholesky back substitution (single matrix real floating-point data)");
ANNOTATE_FUN(matcholbkwsubst8x8f,           "Cholesky back substitution (single matrix real floating-point data)");
ANNOTATE_FUN(matcholbkwsubst10x10f,         "Cholesky back substitution (single matrix real floating-point data)");
ANNOTATE_FUN(cmatcholmmsesolver4x4f,         "Cholesky MMSE (single matrix complex floating-point data)");
ANNOTATE_FUN(cmatcholmmsesolver6x6f,         "Cholesky MMSE (single matrix complex floating-point data)");
ANNOTATE_FUN(cmatcholmmsesolver8x8f,         "Cholesky MMSE (single matrix complex floating-point data)");
ANNOTATE_FUN(cmatcholmmsesolver10x10f,       "Cholesky MMSE (single matrix complex floating-point data)");
ANNOTATE_FUN(matcholmmsesolver4x4f,          "Cholesky MMSE (single matrix real floating-point data)");
ANNOTATE_FUN(matcholmmsesolver6x6f,          "Cholesky MMSE (single matrix real floating-point data)");
ANNOTATE_FUN(matcholmmsesolver8x8f,          "Cholesky MMSE (single matrix real floating-point data)");
ANNOTATE_FUN(matcholmmsesolver10x10f,        "Cholesky MMSE (single matrix real floating-point data)");
ANNOTATE_FUN(cmatcholpreprocess4x4f,   "Cholesky Preprocessing (single matrix complex floating-point data)");
ANNOTATE_FUN(cmatcholpreprocess6x6f,   "Cholesky Preprocessing (single matrix complex floating-point data)");
ANNOTATE_FUN(cmatcholpreprocess8x8f,   "Cholesky Preprocessing (single matrix complex floating-point data)");
ANNOTATE_FUN(cmatcholpreprocess10x10f, "Cholesky Preprocessing (single matrix complex floating-point data)");
ANNOTATE_FUN(matcholpreprocess4x4f,    "Cholesky Preprocessing (single matrix real floating-point data)");
ANNOTATE_FUN(matcholpreprocess6x6f,    "Cholesky Preprocessing (single matrix real floating-point data)");
ANNOTATE_FUN(matcholpreprocess8x8f,    "Cholesky Preprocessing (single matrix real floating-point data)");
ANNOTATE_FUN(matcholpreprocess10x10f,  "Cholesky Preprocessing (single matrix real floating-point data)");
ANNOTATE_FUN(cmatcholpseudoinv4x4f,         "Matrix (Pseudo) Inversion (single matrix complex floating-point data)");
ANNOTATE_FUN(cmatcholpseudoinv6x6f,         "Matrix (Pseudo) Inversion (single matrix complex floating-point data)");
ANNOTATE_FUN(cmatcholpseudoinv8x8f,         "Matrix (Pseudo) Inversion (single matrix complex floating-point data)");
ANNOTATE_FUN(cmatcholpseudoinv10x10f,       "Matrix (Pseudo) Inversion (single matrix complex floating-point data)");
ANNOTATE_FUN(matcholpseudoinv4x4f,          "Matrix (Pseudo) Inversion (single matrix real floating-point data)");
ANNOTATE_FUN(matcholpseudoinv6x6f,          "Matrix (Pseudo) Inversion (single matrix real floating-point data)");
ANNOTATE_FUN(matcholpseudoinv8x8f,          "Matrix (Pseudo) Inversion (single matrix real floating-point data)");
ANNOTATE_FUN(matcholpseudoinv10x10f,        "Matrix (Pseudo) Inversion (single matrix real floating-point data)");

ANNOTATE_FUN(cmatcholdecomp4x4_32x32,             "Cholesky decomposition (single matrix complex 32-bit data)");
ANNOTATE_FUN(cmatcholdecomp6x6_32x32,             "Cholesky decomposition (single matrix complex 32-bit data)");
ANNOTATE_FUN(cmatcholdecomp8x8_32x32,             "Cholesky decomposition (single matrix complex 32-bit data)");
ANNOTATE_FUN(cmatcholdecomp10x10_32x32,           "Cholesky decomposition (single matrix complex 32-bit data)");
ANNOTATE_FUN(matcholdecomp4x4_32x32,              "Cholesky decomposition (single matrix real 32-bit data)");
ANNOTATE_FUN(matcholdecomp6x6_32x32,              "Cholesky decomposition (single matrix real 32-bit data)");
ANNOTATE_FUN(matcholdecomp8x8_32x32,              "Cholesky decomposition (single matrix real 32-bit data)");
ANNOTATE_FUN(matcholdecomp10x10_32x32,            "Cholesky decomposition (single matrix real 32-bit data)");
ANNOTATE_FUN(cmatcholfwdsubst4x4_32x32,          "Cholesky forward substitution (single matrix complex 32-bit data)");
ANNOTATE_FUN(cmatcholfwdsubst6x6_32x32,          "Cholesky forward substitution (single matrix complex 32-bit data)");
ANNOTATE_FUN(cmatcholfwdsubst8x8_32x32,          "Cholesky forward substitution (single matrix complex 32-bit data)");
ANNOTATE_FUN(cmatcholfwdsubst10x10_32x32,        "Cholesky forward substitution (single matrix complex 32-bit data)");
ANNOTATE_FUN(matcholfwdsubst4x4_32x32,           "Cholesky forward substitution (single matrix real 32-bit data)");
ANNOTATE_FUN(matcholfwdsubst6x6_32x32,           "Cholesky forward substitution (single matrix real 32-bit data)");
ANNOTATE_FUN(matcholfwdsubst8x8_32x32,           "Cholesky forward substitution (single matrix real 32-bit data)");
ANNOTATE_FUN(matcholfwdsubst10x10_32x32,         "Cholesky forward substitution (single matrix real 32-bit data)");
ANNOTATE_FUN(cmatcholbkwsubst4x4_32x32,          "Cholesky back substitution (single matrix complex 32-bit data)");
ANNOTATE_FUN(cmatcholbkwsubst6x6_32x32,          "Cholesky back substitution (single matrix complex 32-bit data)");
ANNOTATE_FUN(cmatcholbkwsubst8x8_32x32,          "Cholesky back substitution (single matrix complex 32-bit data)");
ANNOTATE_FUN(cmatcholbkwsubst10x10_32x32,        "Cholesky back substitution (single matrix complex 32-bit data)");
ANNOTATE_FUN(matcholbkwsubst4x4_32x32,           "Cholesky back substitution (single matrix real 32-bit data)");
ANNOTATE_FUN(matcholbkwsubst6x6_32x32,           "Cholesky back substitution (single matrix real 32-bit data)");
ANNOTATE_FUN(matcholbkwsubst8x8_32x32,           "Cholesky back substitution (single matrix real 32-bit data)");
ANNOTATE_FUN(matcholbkwsubst10x10_32x32,         "Cholesky back substitution (single matrix real 32-bit data)");
ANNOTATE_FUN(cmatcholmmsesolver4x4_32x32,         "Cholesky MMSE (single matrix complex 32-bit data)");
ANNOTATE_FUN(cmatcholmmsesolver6x6_32x32,         "Cholesky MMSE (single matrix complex 32-bit data)");
ANNOTATE_FUN(cmatcholmmsesolver8x8_32x32,         "Cholesky MMSE (single matrix complex 32-bit data)");
ANNOTATE_FUN(cmatcholmmsesolver10x10_32x32,       "Cholesky MMSE (single matrix complex 32-bit data)");
ANNOTATE_FUN(matcholmmsesolver4x4_32x32,          "Cholesky MMSE (single matrix real 32-bit data)");
ANNOTATE_FUN(matcholmmsesolver6x6_32x32,          "Cholesky MMSE (single matrix real 32-bit data)");
ANNOTATE_FUN(matcholmmsesolver8x8_32x32,          "Cholesky MMSE (single matrix real 32-bit data)");
ANNOTATE_FUN(matcholmmsesolver10x10_32x32,        "Cholesky MMSE (single matrix real 32-bit data)");
ANNOTATE_FUN(cmatcholpreprocess4x4_32x32,   "Cholesky Preprocessing (single matrix complex 32-bit data)");
ANNOTATE_FUN(cmatcholpreprocess6x6_32x32,   "Cholesky Preprocessing (single matrix complex 32-bit data)");
ANNOTATE_FUN(cmatcholpreprocess8x8_32x32,   "Cholesky Preprocessing (single matrix complex 32-bit data)");
ANNOTATE_FUN(cmatcholpreprocess10x10_32x32, "Cholesky Preprocessing (single matrix complex 32-bit data)");
ANNOTATE_FUN(matcholpreprocess4x4_32x32,    "Cholesky Preprocessing (single matrix real 32-bit data)");
ANNOTATE_FUN(matcholpreprocess6x6_32x32,    "Cholesky Preprocessing (single matrix real 32-bit data)");
ANNOTATE_FUN(matcholpreprocess8x8_32x32,    "Cholesky Preprocessing (single matrix real 32-bit data)");
ANNOTATE_FUN(matcholpreprocess10x10_32x32,  "Cholesky Preprocessing (single matrix real 32-bit data)");
ANNOTATE_FUN(cmatcholpseudoinv4x4_32x32,         "Matrix (Pseudo) Inversion (single matrix complex 32-bit data)");
ANNOTATE_FUN(cmatcholpseudoinv6x6_32x32,         "Matrix (Pseudo) Inversion (single matrix complex 32-bit data)");
ANNOTATE_FUN(cmatcholpseudoinv8x8_32x32,         "Matrix (Pseudo) Inversion (single matrix complex 32-bit data)");
ANNOTATE_FUN(cmatcholpseudoinv10x10_32x32,       "Matrix (Pseudo) Inversion (single matrix complex 32-bit data)");
ANNOTATE_FUN(matcholpseudoinv4x4_32x32,          "Matrix (Pseudo) Inversion (single matrix real 32-bit data)");
ANNOTATE_FUN(matcholpseudoinv6x6_32x32,          "Matrix (Pseudo) Inversion (single matrix real 32-bit data)");
ANNOTATE_FUN(matcholpseudoinv8x8_32x32,          "Matrix (Pseudo) Inversion (single matrix real 32-bit data)");
ANNOTATE_FUN(matcholpseudoinv10x10_32x32,        "Matrix (Pseudo) Inversion (single matrix real 32-bit data)");
