/* ------------------------------------------------------------------------ */
/* Copyright (c) 2018 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs ("Cadence    */
/* Libraries") are the copyrighted works of Cadence Design Systems Inc.	    */
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
/*          Copyright (C) 2015-2018 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
/*
 * NatureDSP_Signal Library API
 * Mel-frequency cepstrum coefficients extraction
 * Function annotations
 */

/* Portable data types. */
#include "NatureDSP_types.h"
/* NatureDSP_Signal Library MFCC features extraction API. */
#include "NatureDSP_Signal_audio.h"
/* Common utility declarations. */
#include "common.h"

ANNOTATE_FUN(logmel32x32_process, "Compute log mel filterbank energies (32-bit fixed-point input/output data)                     ");
ANNOTATE_FUN(logmelf_process    , "Compute log mel filterbank energies (single precision floating-point input/output data)        ");
ANNOTATE_FUN(mfcc32x32_process  , "Compute Mel-Frequency Cepstrum Coefficients (32-bit fixed-point input/output data)             ");
ANNOTATE_FUN(mfccf_process      , "Compute Mel-Frequency Cepstrum Coefficients (single precision floating-point input/output data)");
