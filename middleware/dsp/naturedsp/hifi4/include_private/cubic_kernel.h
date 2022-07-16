/* ------------------------------------------------------------------------ */
/* Copyright (c) 2020 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
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
/*          Copyright (C) 2015-2020 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
#ifndef CUBIC_KERNEL_H__
#define CUBIC_KERNEL_H__
#include "NatureDSP_types.h"
#include "common.h"

/*--------------------------------------------------
 compute cubic kernel: input in Q14, output in Q15

See Keys, "Cubic Convolution Interpolation for Digital Image
Processing," IEEE Transactions on Acoustics, Speech, and Signal
Processing, Vol. ASSP-29, No. 6, December 1981, p. 1155.


 Input:
 w[N] input in Q14
 Output:
 w[N] output Q15
 
 array is aligned, N is a multiple of 4
-------------------------------------------------*/
void cubic_kernel(int16_t *w, int N);

#endif
