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
	NatureDSP_Signal library. FFT part
    C code with generic optimization

    twiddle factors for 32x16/16x16 real fwd/rev FFT transforms
*/

#include "NatureDSP_Signal.h"
#include "fft_cplx_twiddles.h"
#include "common.h"

/*   
    Increment tables show the stride on input data between two succesive RADIX-4 steps. 
    These numbers are in f24 units so that stride is twiced because data are complex
    Additional last item in the increment tables does not make sense but it is written to 
    prevent the access outside the memory. It is formed in such a way to move the butterfly 
    pointer to the original value,i.e. to the beginning of the input array.

    For FFT16 :
    x[0],x[1],x[2],x[3]  
    strides  1,1,1
    inc16   {2,2,2,-6};

    For FFT32:
    x[0],x[1],x[2],x[3],x[4],x[5],x[6],x[7],x[0],x[1],x[8],x[9],x[16],x[17],x[24],x[25] 
    strides 1,1,1,1,1,1,1,-7,1,7,1,7,1,7,1
    inc32  {2,2,2,2,2,2,2,-14,2,14,2,14,2,14,2,-50};
    ....

    for less memory comsumption, increment tables additionnaly modifyed by removing of following entries:
    - first N/4 entries  
    - for N!=4^n last N/4 entries 
    - each 1,2 and 3-rd entries from each four 
*/
const short fft_inc256[]={2,2,2,98,2,2,2,98,2,2,2,98,2,2,2,-414,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,-486};
