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
 * IIR Filters
 * Annotations
 */
#include "NatureDSP_types.h"
#include "NatureDSP_Signal_iir.h"
#include "common.h"

ANNOTATE_FUN(bqriir16x16_df1, "Bi-quad Real Block IIR, DFI (16-bit data, 16-bit coefficients, 16-bit intermediate stage outputs)  ");
ANNOTATE_FUN(bqriir16x16_df2, "Bi-quad Real Block IIR, DFII (16-bit data, 16-bit coefficients, 16-bit intermediate stage outputs) ");
ANNOTATE_FUN(bqriir24x24_df1, "Bi-quad Real Block IIR, DFI (32-bit data, 24-bit coefficients, 32-bit intermediate stage outputs)  ");
ANNOTATE_FUN(bqriir24x24_df2, "Bi-quad Real Block IIR, DFII (32-bit data, 24-bit coefficients, 32-bit intermediate stage outputs) ");
ANNOTATE_FUN(bqriir32x16_df1, "Bi-quad Real Block IIR, DFI (32-bit data, 16-bit coefficients, 32-bit intermediate stage outputs)  ");
ANNOTATE_FUN(bqriir32x16_df2, "Bi-quad Real Block IIR, DFII (32-bit data, 16-bit coefficients, 32-bit intermediate stage outputs) ");
ANNOTATE_FUN(bqriir32x32_df1, "Bi-quad Real Block IIR, DFI (32-bit data, 32-bit coefficients, 32-bit intermediate stage outputs)  ");
ANNOTATE_FUN(bqriir32x32_df2, "Bi-quad Real Block IIR, DFII (32-bit data, 32-bit coefficients, 32-bit intermediate stage outputs) ");
ANNOTATE_FUN(bqriirf_df1, "Bi-quad Real Block IIR, DFI (floating point data)                                                  ");
ANNOTATE_FUN(bqriirf_df2, "Bi-quad Real Block IIR, DFII (floating point data)                                                 ");
ANNOTATE_FUN(bqriirf_df2t, "Bi-quad Real Block IIR, DFIIt (floating point data)                                                ");
ANNOTATE_FUN(bqciirf_df1, "Bi-quad Real Block IIR, DFI (complex floating point data)                                          ");
ANNOTATE_FUN(stereo_bqriir16x16_df1, "Bi-quad Stereo Block Stereo IIR, DFI (16-bit data, 16-bit coefficients, 16-bit intermediate stage outputs)  ");
ANNOTATE_FUN(stereo_bqriir32x16_df1, "Bi-quad Stereo Block Stereo IIR, DFI (32-bit data, 16-bit coefficients, 32-bit intermediate stage outputs)  ");
ANNOTATE_FUN(stereo_bqriir32x32_df1, "Bi-quad Stereo Block Stereo IIR, DFI (32-bit data, 32-bit coefficients, 32-bit intermediate stage outputs)  ");
ANNOTATE_FUN(stereo_bqriirf_df1, "Bi-quad Real Block Stereo IIR, DFI (floating point data)                                                  ");
ANNOTATE_FUN(latr16x16_process, "Lattice Block Real IIR (16-bit data, 16-bit coefficients)                                        ");
ANNOTATE_FUN(latr24x24_process, "Lattice Block Real IIR (24-bit data, 24-bit coefficients)                                        ");
ANNOTATE_FUN(latr32x16_process, "Lattice Block Real IIR (32-bit data, 16-bit coefficients)                                        ");
ANNOTATE_FUN(latr32x32_process, "Lattice Block Real IIR (32-bit data, 32-bit coefficients)                                        ");
ANNOTATE_FUN(latrf_process, "Lattice Block Real IIR (floating point data)                                                     ");
