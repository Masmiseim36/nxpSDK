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
 * Fitting and Interpolation Routines
 * Annotations
 */
#include "NatureDSP_types.h"
#include "NatureDSP_Signal_fit.h"
#include "common.h"

ANNOTATE_FUN(vec_poly4_24x24, "Polynomial approximation (24-bit data)");
ANNOTATE_FUN(vec_poly8_24x24, "Polynomial approximation (24-bit data)");
ANNOTATE_FUN(vec_poly4_32x32, "Polynomial approximation (32-bit data)");
ANNOTATE_FUN(vec_poly8_32x32, "Polynomial approximation (32-bit data)");
ANNOTATE_FUN(vec_poly4f,      "Polynomial approximation (floating point data)");
ANNOTATE_FUN(vec_poly8f,      "Polynomial approximation (floating point data)");
