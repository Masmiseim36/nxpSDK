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
#include "scl_atan_table16.h"
//#include "common.h"
#ifdef COMPILER_MSVC
const int32_t _declspec(align(8)) atan_table16[] =
#else
const int32_t                     atan_table16[34] __attribute__((aligned(8))) =
#endif
{
0         ,0x0145C000,0x028b0000,0x03cf0000,0x05110000,0x0650C000,0x078D4000,0x08C64000,
0x09FB4000,0x0B2BC000,0x0C574000,0x0D7D8000,0x0E9E0000,0x0FB90000,0x10CD8000,0x11DC0000,
0x12E40000,0x13E58000,0x14E08000,0x15D4C000,0x16C28000,0x17A98000,0x188A0000,0x19644000,
0x1A380000,0x1B058000,0x1BCCC000,0x1C8E4000,0x1D49C000,0x1DFF8000,0x1EAFC000,0x1F5A8000,
0x1FFFC000,0x1FFFC000
};
