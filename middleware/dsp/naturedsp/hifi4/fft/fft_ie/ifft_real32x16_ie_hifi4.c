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
    C code optimized for HiFi4
    Integrit, 2006-2017
*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fft.h"
/* Common utility and macros declarations. */
#include "common.h"

/*
    in-place inverse split part of FFT:
    x[N+2]  input (N+2 samples)/output(N samples)
    N       size of FFT
    Scaling factor of isplitPart_x2 is 3.
*/
static void isplitPart_x2(int32_t *x, int N, int shift, const int16_t *tw, int step)
{
  int i;

  const int step_back = -8;

        ae_int32x2 * restrict p_x0;
        ae_int32x2 * restrict p_x1;
  const ae_int32x2 * restrict p_x0rd;
  const ae_int32x2 * restrict p_x1rd;
  const ae_int32   * restrict p_twd;

  ae_int32x2  vA0, vA1, vB0, vB1, vC0, vC1, vR;
  ae_int16x4  vT;

  NASSERT_ALIGN8(x);

  step *= sizeof(int16_t)*2;
  p_twd = (const ae_int32 *)((uintptr_t)tw + step);
  p_x0rd = p_x0 = (ae_int32x2 *)x;
  p_x1rd = p_x1 = (ae_int32x2 *)(x + N);

  vR = AE_MOVI(1);
  WUR_AE_SAR(1);

  // first point
  AE_L32X2_IP(vA0, p_x0rd, 8);
  AE_L32X2_XP(vA1, p_x1rd, step_back);

  vA0 = AE_SRAA32RS(vA0, shift-1);
  vA1 = AE_SRAA32RS(vA1, shift-1);

  vB0 = AE_ADD32S(vA0, vA1);
  vB1 = AE_SUB32S(vA0, vA1);
  vB0 = AE_SEL32_HH(vB0, vB1);
  vB1 = AE_MOVI(0);
  vB0 = AE_SRAI32R(vB0, 1);
  AE_S32X2_IP(vB0, p_x0, 8);
  AE_S32X2_XP(vB1, p_x1, step_back);

  __Pragma("loop_count min=2");
  for(i = 1; i < (N>>2); i++)
  {
    // load twiddle
    AE_L32_XP(vB1, p_twd, step);
    vT = AE_MOVINT16X4_FROMINT32X2(vB1);

    // load next data
    AE_L32X2_IP(vA0, p_x0rd, 8);
    AE_L32X2_XP(vA1, p_x1rd, step_back);

    vA0 = AE_SRAA32RS(vA0, shift-1);
    vA1 = AE_SRAA32RS(vA1, shift-1);

    vB0 = AE_SUBADD32S(vA0, vA1);
    vB0 = AE_SEL32_LH(vB0, vB0);
    vB0 = AE_MULFC32X16RAS_H(vB0, vT);
    vB0 = AE_ADDSUB32S(vR, vB0);
    
    vB1 = AE_ADDSUB32S(AE_ADD32S(vA0, vR), vA1);

    AE_ADDANDSUBRNG32(vC1, vC0, vB1, vB0);
    vC1 = AE_ADDSUB32S(0, vC1);

    AE_S32X2_IP(vC0, p_x0, 8);
    AE_S32X2_XP(vC1, p_x1, step_back);

  }

  // middle sample
  AE_L32X2_IP(vA0, p_x0rd, 8);
  vA0 = AE_SRAA32RS(vA0, shift-1);
  vB0 = AE_NEG32S(vA0);
  vC0 = AE_SEL32_HL(vA0, vB0);
  AE_S32X2_I(vC0, p_x0, 0);
} /* isplitPart_x2() */

/*-------------------------------------------------------------------------
  Inverse FFT on Real Data with Optimized Memory Usage
  These functions make inverse FFT on real data from half of spectrum with
  optimized memory usage.
  Scaling: 
      +-----------------------+--------------------------------------+
      |      Function         |           Scaling options            |
      +-----------------------+--------------------------------------+
      | ifft_real16x16_ie     |  2 - 16-bit dynamic scaling          |
      | ifft_real32x16_ie     |  2 - 32-bit dynamic scaling          |
      |                       |  3 - fixed scaling before each stage |
      | ifft_real32x32_ie     |  2 - 32-bit dynamic scaling          |
      |                       |  3 - fixed scaling before each stage |
      +-----------------------+--------------------------------------+
  NOTES:
  1. Bit-reversing reordering is done here.
  2. INPUT DATA MAY APPEAR DAMAGED after the call.
  3. FFT functions may use input and output buffers for temporal storage
     of intermediate 32-bit data, so FFT functions with 24-bit packed
     I/O (Nx3-byte data) require that the buffers are large enough to 
     keep Nx4-byte data.
  4. FFT of size N may be supplied with constant data (twiddle factors) 
     of a larger-sized FFT = N*twdstep.

  Precision:
  16x16_ie      16-bit input/outputs, 16-bit data, 16-bit twiddles
  32x16_ie      32-bit input/outputs, 32-bit data, 16-bit twiddles
  32x32_ie      32-bit input/outputs, 32-bit data, 32-bit twiddles
  f_ie          floating point

  Input:
  x                     input spectrum (positive side). Real and imaginary
                        data are interleaved and real data goes first:
  --------------+----------+-----------------+----------------
  Function      |   Size   |  Allocated Size |       type    |
  --------------+----------+-----------------+----------------
  16x16_ie      |   N/2+1  |      N/2+1      |complex_fract16|
  32x16_ie      |   N/2+1  |      N/2+1      |complex_fract32|
  32x32_ie      |   N/2+1  |      N/2+1      |complex_fract32|
  f_ie          |   N/2+1  |      N/2+1      | complex_float |
  --------------+----------+-----------------+----------------

  twd[2*N*twdstep*3/4]  twiddle factor table of a complex-valued FFT
                        of size N*twdstep
  N                     FFT size
  twdstep               twiddle step
  scalingOpt            scaling option (see table above), not applicable 
                        to the floating point function
  Output:
  y                     output spectrum. Real and imaginary data are 
                        interleaved and real data goes first:
  --------------+----------+-----------------+-----------
  Function      |   Size   |  Allocated Size |  type    |
  --------------+----------+-----------------+-----------
  16x16_ie      |     N    |      N          |  int16_t |
  32x16_ie      |     N    |      N          |  int32_t |
  32x32_ie      |     N    |      N          |  int32_t |
  f_ie          |      N   |      N          | float32_t|
  --------------+----------+-----------------+-----------

  Returned value: total number of right shifts occurred during scaling
  procedure

  Restrictions:
  x,y   should not overlap
  x,y   aligned on 8-bytes boundary
-------------------------------------------------------------------------*/
int ifft_real32x16_ie(  int32_t* y,complex_fract32* x, const complex_fract16* twd, int twdstep, int N, int scalingOpt)
{
    const ae_int32x2 * restrict p_x0;
    int n, bexp;
    int scale = 3; //Scaling factor of isplitPart_x2

    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);
    NASSERT(scalingOpt==3 || scalingOpt==2);
    NASSERT(N>=32 && (N&(N-1))==0);

    bexp = 0;
    if (scalingOpt == 2)
    {
        int32_t i;
        ae_int32x2 acc, tmp;
        p_x0 = (ae_int32x2 *)x;
        acc = AE_ZERO32();
        __Pragma("loop_count min=4, factor=4");
        for (n = 0; n < ((N+2)>>1); n++)   
        {
            AE_L32X2_IP(tmp, p_x0, sizeof(ae_int32x2));
            tmp = AE_ABS32S(tmp); 
            acc = AE_OR32(acc, tmp);
        }
        acc = AE_OR32(acc, AE_SEL32_LH(acc, acc));
        i = AE_MOVAD32_L(acc);
        bexp = NSA(i);
        XT_MOVEQZ(bexp, 0, i);
    }
    scale = scale - bexp;
    isplitPart_x2((int32_t*)x, N, scale, (const int16_t*)twd, twdstep);
    scale +=  ifft_cplx32x16_ie((complex_fract32*)y, x, twd, twdstep*2, N/2, scalingOpt);

    return scale;
} /* ifft_real32x16_ie() */
