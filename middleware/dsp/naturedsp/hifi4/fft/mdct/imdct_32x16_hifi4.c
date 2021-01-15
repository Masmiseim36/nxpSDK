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
    IMDCT 32x16 with scaling option 3
    C code optimized for HiFi4
    Integrit, 2006-2017
*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fft.h"
/* Common utility and macros declarations. */
#include "common.h"
/* Twiddle factor tables for DCTs. */
#include "dct4_twd.h"

/*-------------------------------------------------------------------------
  Modified Discrete Cosine Transform.
  These functions apply Modified DCT to input (convert 2N real data to N 
  spectral components) and make inverse conversion forming 2N numbers from 
  N inputs. Normally, combination of MDCT and DCT is invertible if applied 
  to subsequent data blocks with overlapping.
  Scaling:
      +-----------------------+--------------------------------------+
      |      Function         |           Scaling options            |
      +-----------------------+--------------------------------------+
      |       mdct_32x16      |  3 - fixed scaling before each stage |
      |       mdct_32x32      |  3 - fixed scaling before each stage |
      |      imdct_32x16      |  3 - fixed scaling before each stage |
      |      imdct_32x32      |  3 - fixed scaling before each stage |
      +-----------------------+--------------------------------------+
  NOTES:
     1. MDCT/IMDCT runs in-place algorithm so INPUT DATA WILL APPEAR DAMAGED 
     after the call.
     2. N - MDCT size (depends on selected MDCT handle)

  Precision: 
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
int imdct_32x16( int32_t * y, int32_t * x, dct_handle_t h, int scalingOpt)
{  
    const tdct4_twd_fr16 *ptwd=(const tdct4_twd_fr16 *)h;
    const ae_int32x2 * restrict pyrd_pos0;
    const ae_int32x2 * restrict pyrd_pos1;
    const ae_int64   * restrict pyrd_neg0;
    const ae_int64   * restrict pyrd_neg1;
          ae_int32x2 * restrict pywr_pos0;
          ae_int32x2 * restrict pywr_pos1;
          ae_int64   * restrict pywr_neg0;
          ae_int64   * restrict pywr_neg1;
    ae_int32x2 t0, t1, t2, t3, r2, r3;
    ae_int64   T0, T1, T2, T3;
    int N,n,scl;
    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);
    NASSERT(scalingOpt == 3);
    N=ptwd->N;

    scl=dct4_32x16(y, x, h, scalingOpt);

    pyrd_pos0 = (const ae_int32x2 *)(y);
    pyrd_pos1 = (const ae_int32x2 *)(y+N/2);
    pyrd_neg0 = (const ae_int64   *)(y+N/2-2);
    pyrd_neg1 = (const ae_int64   *)(y+N-2);
    pywr_pos0 = (      ae_int32x2 *)(y+N);
    pywr_pos1 = (      ae_int32x2 *)(y);
    pywr_neg0 = (      ae_int64   *)(y+N/2-2);
    pywr_neg1 = (      ae_int64   *)(y+N-2);

    /* Transform 0...N*2 samples */
    __Pragma("loop_count min=4");
    for(n=0;n<(N>>3);n++)
    {
        AE_L32X2_IP(t0, pyrd_pos0, 2*sizeof(int32_t));
        AE_L64_IP  (T1, pyrd_neg0, -2*(int)sizeof(int32_t));
        AE_L32X2_IP(t2, pyrd_pos1, 2*sizeof(int32_t));
        AE_L64_IP  (T3, pyrd_neg1, -2*(int)sizeof(int32_t));
        t1 = AE_MOVINT32X2_FROMINT64(T1);
        t3 = AE_MOVINT32X2_FROMINT64(T3);
        t0 = AE_SRAI32R(t0, 1);
        t1 = AE_SRAI32R(t1, 1);
        t2 = AE_SRAI32R(t2, 1);
        t3 = AE_SRAI32R(t3, 1);
        t0 = AE_NEG32S(t0);
        t1 = AE_NEG32S(t1);
        r2 = AE_NEG32S(t2);
        r3 = AE_NEG32S(t3);
        T0 = AE_MOVINT64_FROMINT32X2(t0);
        T1 = AE_MOVINT64_FROMINT32X2(t1);
        T2 = AE_MOVINT64_FROMINT32X2(r2);
        T3 = AE_MOVINT64_FROMINT32X2(t3);
        __Pragma("no_reorder");
        AE_S32X2_X (t0, pywr_pos0, N/2*sizeof(int32_t));
        AE_S32X2_IP(t1, pywr_pos0, 2*sizeof(int32_t));
        AE_S32X2_X (r3, pywr_pos1, N/2*sizeof(int32_t));
        AE_S32X2_IP(t2, pywr_pos1, 2*sizeof(int32_t));
        AE_S64_X   (T0, pywr_neg0, N*sizeof(int32_t));
        AE_S64_X   (T1, pywr_neg1, N*sizeof(int32_t));
        AE_S64_IP  (T3, pywr_neg0, -2*(int)sizeof(int32_t));
        AE_S64_IP  (T2, pywr_neg1, -2*(int)sizeof(int32_t));
    }

    return scl+1;

}/* imdct_32x16() */
