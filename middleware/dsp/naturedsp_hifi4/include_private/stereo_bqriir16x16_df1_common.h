/* ------------------------------------------------------------------------ */
/* Copyright (c) 2018 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
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
/*          Copyright (C) 2015-2018 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
#ifndef STEREO_BQRIIR16X16_DF1_COMMON_H__
#define STEREO_BQRIIR16X16_DF1_COMMON_H__

/* Portable data types. */
#include "NatureDSP_types.h"
/* Common utility and macros declarations. */
#include "common.h"


/* Instance pointer validation number. */
#define STEREO_BQRIIR16X16_DF1_MAGIC 0xc830d46a

/* Filter instance structure. */
typedef struct tag_stereo_bqriir16x16_df1_t
{
	uint32_t magic;    // Instance pointer validation number
	bqriir16x16_df1_handle_t bqriir_left;
	bqriir16x16_df1_handle_t bqriir_right;
	void * bqriir_left_mem;
	void * bqriir_right_mem;
} stereo_bqriir16x16_df1_t, *stereo_bqriir16x16_df1_ptr_t;

#endif /* STEREO_BQRIIR16X16_DF1_COMMON_H__ */
