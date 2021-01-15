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
/*          Copyright (C) 2015-2017 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
/*
   NatureDSP Signal Processing Library. FFT part
    Inverse Modified Discrete Cosine Transform 
    C code optimized for HiFi4
   Integrit, 2006-2017
*/
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fft.h"
#include "common.h"

/*-------------------------------------------------------------------------
  Modified Discrete Cosine Transform.
  These functions apply Modified DCT to input (convert 2N real data to N 
  spectral components) and make inverse conversion forming 2N numbers from 
  N inputs. Normally, combination of MDCT and DCT is invertible if applied 
  to subsequent data blocks with overlapping.
    Scaling  : 
      +-----------------------+--------------------------------------+
      |      Function         |           Scaling options            |
      +-----------------------+--------------------------------------+
      |       mdct_24x24      |  3 - fixed scaling before each stage |
      |       mdct_32x16      |  3 - fixed scaling before each stage |
      |       mdct_32x32      |  3 - fixed scaling before each stage |
      |      imdct_24x24      |  3 - fixed scaling before each stage |
      |      imdct_32x16      |  3 - fixed scaling before each stage |
      |      imdct_32x32      |  3 - fixed scaling before each stage |
      +-----------------------+--------------------------------------+
  NOTES:
     1. MDCT/IMDCT runs in-place algorithm so INPUT DATA WILL APPEAR DAMAGED 
     after the call.
     2. N - MDCT size (depends on selected MDCT handle)

  Precision: 
  24x24  24-bit input/outputs, 24-bit twiddles
  32x16  32-bit input/outputs, 16-bit twiddles
  32x32  32-bit input/outputs, 32-bit twiddles

  -------------------------------------------------------------------------
  For MDCT:
  Input:
  x[2*N]      input signal
  h           MDCT handle
  scalingOpt  scaling option (see table above)
  Output:
  y[N]        output of transform 
  -------------------------------------------------------------------------
  For IMDCT:
  Input:
  x[N]        input signal
  h           IMDCT handle
  scalingOpt  scaling option (see table above)
  Output:
  y[2*N]      output of transform
  -------------------------------------------------------------------------
  Returned value:
              total number of right shifts occurred during scaling 
              procedure 
  Restriction:
  x,y         should not overlap
  x,y         aligned on 8-bytes boundary
-------------------------------------------------------------------------*/
DISCARD_FUN( int, imdct_24x24, (f24* y, f24* x, dct_handle_t h, int scalingOpt) )
