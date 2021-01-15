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
    In-place inverse split part of FFT:
    x[N+2]  input (N+2 samples)/output(N samples)
    N       size of FFT
    Internal scaling is 2-lshift
*/
static void isplitPart16x16_ie_lshift(complex_fract16 *x, int N, const complex_fract16 *tw, int twstep, int lshift)
{
  const ae_int16x4 *          X0;
  const ae_int16x4 *          X1;
        ae_int16x4 * restrict Y0;
        ae_int16x4 * restrict Y1;
  const ae_int16x4 *          SPC;

  ae_valign X1_va, Y1_va;
  int n, step;

  NASSERT_ALIGN8( x );
  NASSERT( !( N&(N-1) ) );

  step = twstep*2*(int)sizeof(int16_t);

  {
    ae_f32x2   a00, a01, a10, a11;
    ae_f32x2   b00, b01, b10, b11;
    ae_int16x4 t0, t1;

    ae_int16x4 tw_;
    ae_int16x4 scl;

    const int shift = 2;
    scl = AE_MOVDA16( 1<<lshift );
    WUR_AE_SAR( shift );

    X0 = (const ae_int16x4*)( (uintptr_t)x +         0*2 );
    X1 = (const ae_int16x4*)( (uintptr_t)x + (2*N/2+1)*2 );
    Y0 = (      ae_int16x4*)( (uintptr_t)x +         0*2 );
    Y1 = (      ae_int16x4*)( (uintptr_t)x + (2*N/2+1)*2 );
    SPC = (const ae_int16x4*)tw;

    X1_va = AE_LA64_PP( X1 );
    Y1_va = AE_ZALIGN64();

    __Pragma( "loop_count min=4, factor=4" );
    for ( n=0; n<N/(4*2); n++ )
    {
      ae_int32x2 vT32_0, vT32_1;
      // tw0, tw1
      AE_L32_XP( vT32_0, castxcc(ae_int32,SPC), step );
      AE_L32_XP( vT32_1, castxcc(ae_int32,SPC), step );
      vT32_0 = AE_SEL32_LL(vT32_1, vT32_0);
      tw_ = AE_MOVINT16X4_FROMINT32X2(vT32_0);
      tw_ = AE_SHORTSWAP(tw_);

      // a00, a01
      AE_L16X4_IP( t0, X0, +2*4 );
      // 1j*conj(a10), 1j*conj(a11)
      AE_LA16X4_RIP( t1, X1_va, X1 );

      AE_MUL16X4( a00, a01, t0, scl );
      AE_MUL16X4( a10, a11, t1, scl );
      a10 = AE_MUL32JS(a10);
      a11 = AE_MUL32JS(a11);

      // b0x <- a0x + conj(a1x)
      // b1x <- a0x - conj(a1x)
      AE_ADDANDSUBRNG32( b10, b00, a00, a10 );
      AE_ADDANDSUBRNG32( b11, b01, a01, a11 );

      // b1x*conj(twx)
      b10 = AE_MULFC32X16RAS_H( b10, tw_ );
      b11 = AE_MULFC32X16RAS_L( b11, tw_ );

      // a0x <- b0x + b1x
      // a1x <- b0x - b1x
      AE_ADDANDSUB32S( a00, a10, b00, b10 );
      AE_ADDANDSUB32S( a01, a11, b01, b11 );

      // 1j*(b0x-b1x)
      a10 = AE_MUL32JS( a10 );
      a11 = AE_MUL32JS( a11 );

      // b00 + b10, b01 + b11
      t0 = AE_SAT16X4( a00, a01 );
      // conj(b00 - b10), conj(b01 - b11)
      t1 = AE_SAT16X4( a10, a11 );

      AE_S16X4_IP( t0, Y0, +2*4 );
      // conj(b01 - b11), conj(b00 - b10)
      AE_SA16X4_RIP( t1, Y1_va, Y1 );
    }

    AE_SA64NEG_FP( Y1_va, Y1 );

    __Pragma( "no_reorder" );

    //
    // x(N/4+1) = 2*conj(x(N/4+1));
    //

    t0 = AE_L16X4_I( X0, 0 );

    t0 = AE_SRAA16S( t0, 1-lshift );
    t1 = AE_SEL16_6543(t0, t0);
    t0 = AE_NEG16S(t0);
    t0 = AE_SEL16_5432(t0, t0);
    AE_S16_0_I( t1, (ae_int16*)Y0, 0 );
    AE_S16_0_I( t0, (ae_int16*)Y0, 2 );
  }
} //isplitPart16x16_ie_lshift

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

int ifft_real16x16_ie(int16_t* y, complex_fract16* x, const complex_fract16* twd, int twdstep, int N, int scalingOpt)
{
    int scale, bexp; 
    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);
    NASSERT_ALIGN8(twd);
    NASSERT(x != (complex_fract16*)y);
    NASSERT(N == 256 || N == 512 || N == 1024);
    NASSERT(scalingOpt == 2);

    {
        int i;
        ae_int16x4 * px;
        ae_int16x4 acc, tmp;
        px = (ae_int16x4*)x;
        acc = AE_ZERO16();
        __Pragma("loop_count min=2");
        for (i = 0; i < (N >> 2); i++)   
        {
            AE_L16X4_IP(tmp, px, sizeof(*px));
            tmp = AE_ABS16S(tmp); 
            acc = AE_OR16(acc, tmp);
        }
        /* last 2 samples */
        {
            ae_int32x2 tmp32;
            tmp32 = AE_L32_I((const ae_int32 *)px, 0);
            tmp = AE_MOVINT16X4_FROMINT32X2(tmp32);
            tmp = AE_ABS16S(tmp); 
            acc = AE_OR16(acc, tmp);
        }
        acc = AE_OR16(acc, AE_SEL16_5432(acc, acc));
        acc = AE_OR16(acc, AE_SHORTSWAP(acc));

        i = AE_MOVAD16_0(acc);
        bexp = NSA(i) - 16;
        XT_MOVEQZ(bexp, 0, i);
    }
    scale = 2-bexp; 
    isplitPart16x16_ie_lshift(x, N, twd, twdstep, bexp);

    scale += ifft_cplx16x16_ie((complex_fract16*)y, x, twd, twdstep*2, N/2, scalingOpt);
    
    return scale;
} /* ifft_real16x16_ie() */
