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
   Split converter with additional scaling = 1
*/
static void splitPart16x16_ie_shift1(int16_t * x, int N, const complex_fract16 *tw, int twstep)
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
    ae_int16x4 t0, t1, t2;

    ae_int16x4 tw_;
    ae_int16x4 scl;

    const int shift = 1;
    scl = AE_MOVDA16( 1 << ( 15 - (shift+1) ) );

    X0 = (const ae_int16x4*)( (uintptr_t)x +         0*2 );
    X1 = (const ae_int16x4*)( (uintptr_t)x + (2*N/2+1)*2 );
    Y0 = (      ae_int16x4*)( (uintptr_t)x +         0*2 );
    Y1 = (      ae_int16x4*)( (uintptr_t)x + (2*N/2+1)*2 );
    SPC = (const ae_int16x4*)tw;

    t0 = AE_L16_I( (ae_int16*)X0, 0 );
    t1 = AE_L16_I( (ae_int16*)X0, 2 );
    
    AE_S16_0_I( t0, (ae_int16*)Y1, -2 );
    AE_S16_0_I( t1, (ae_int16*)Y1,  0 );

    __Pragma( "no_reorder" );

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
      AE_MOVT16X4(tw_, AE_NEG16S(tw_), 0xa);

      // a00, a01
      AE_L16X4_IP( t0, X0, +2*4 );
      // 1j*conj(a10), 1j*conj(a11)
      AE_LA16X4_RIP( t1, X1_va, X1 );

      // Real and imaginary components are swapped, that is equivalent to
      // conjugation followed by multiplication by 1j:
      // a11, a10
      t1 = AE_SHORTSWAP( t1 );

      AE_MULF16X4SS( a00, a01, t0, scl );
      AE_MULF16X4SS( a11, a10, t1, scl );

      // b0x <- (a0x + conj(a1x))/2
      b00 = AE_ADDSUB32S( a00, a10 );
      b01 = AE_ADDSUB32S( a01, a11 );
      // b1x <- (a0x - conj(a1x))/2
      b10 = AE_SUBADD32S( a00, a10 );
      b11 = AE_SUBADD32S( a01, a11 );

      // b1x*twx
      b10 = AE_MULFC32X16RAS_H( b10, tw_ );
      b11 = AE_MULFC32X16RAS_L( b11, tw_ );

      // a0x <- b0x + b1x
      // a1x <- b0x - b1x
      AE_ADDANDSUB32S( a10, a00, b00, b10 );
      AE_ADDANDSUB32S( a11, a01, b01, b11 );

      // 1j*(b0x-b1x)
      a10 = AE_MUL32JS( a10 );
      a11 = AE_MUL32JS( a11 );

      // b00 + b10, b01 + b11
      t0 = AE_ROUND16X4F32SASYM( a00, a01 );
      // conj(b00 - b10), conj(b01 - b11)
      t1 = AE_ROUND16X4F32SASYM( a10, a11 );

      AE_S16X4_IP( t0, Y0, +2*4 );
      // conj(b01 - b11), conj(b00 - b10)
      AE_SA16X4_RIP( t1, Y1_va, Y1 );
    }

    AE_SA64NEG_FP( Y1_va, Y1 );

    __Pragma( "no_reorder" );

    //
    // y(N/4+1) = conj(y(N/4+1));
    //

    t0 = AE_L16X4_I( X0, 0 );

    t0 = AE_SRAA16RS( t0, shift );

    t1 = AE_SEL16_6543( t0, t0 ); // re
    t2 = AE_SEL16_7632( t0, t0 ); // im

    t2 = AE_NEG16S( t2 );

    AE_S16_0_I( t1, (ae_int16*)Y0, 0 );
    AE_S16_0_I( t2, (ae_int16*)Y0, 2 );
  }
} //splitPart16x16_ie_shift1

/*-------------------------------------------------------------------------
  FFT on Real Data with Optimized Memory Usage
  These functions make FFT on real data forming half of spectrum with
  optimized memory usage.
  Scaling: 
      +-----------------------+--------------------------------------+
      |      Function         |           Scaling options            |
      +-----------------------+--------------------------------------+
      |  fft_real16x16_ie     |  2 - 16-bit dynamic scaling          |
      |  fft_real32x16_ie     |  2 - 32-bit dynamic scaling          |
      |                       |  3 - fixed scaling before each stage |
      |  fft_real32x32_ie     |  2 - 32-bit dynamic scaling          |
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
  x                     input signal: 
  --------------+----------+-----------------+----------+
  Function      |   Size   |  Allocated Size |  type    |
  --------------+----------+-----------------+-----------
  16x16_ie      |     N    |      N          |  int16_t |
  32x16_ie      |     N    |      N          |  int32_t |
  32x32_ie      |     N    |      N          |  int32_t |
  --------------+----------+-----------------+----------+

  twd[N*twdstep*3/4]    twiddle factor table of a complex-valued 
                        FFT of size N*twdstep
  N                     FFT size
  twdstep               twiddle step
  scalingOpt            scaling option (see table above), not applicable 
                        to the floating point function

  Output:
  y                     output spectrum. Real and imaginary data 
                        are interleaved and real data goes first:
  --------------+----------+-----------------+---------------+
  Function      |   Size   |  Allocated Size |  type         |
  --------------+----------+-----------------+----------------
  16x16_ie      |   N/2+1  |      N/2+1      |complex_fract16|
  32x16_ie      |   N/2+1  |      N/2+1      |complex_fract32|
  32x32_ie      |   N/2+1  |      N/2+1      |complex_fract32|
  f_ie          |   N/2+1  |      N/2+1      |complex_float  |
  --------------+----------+-----------------+---------------+

  Returned value: total number of right shifts occurred during scaling
  procedure

  Restrictions:
  x,y     should not overlap
  x,y     aligned on 8-bytes boundary
-------------------------------------------------------------------------*/
int fft_real16x16_ie(complex_fract16* y, int16_t  * x, const complex_fract16* twd, int twdstep, int N, int scalingOpt)
{
    int s;
    NASSERT_ALIGN8(x); 
    NASSERT_ALIGN8(y);
    NASSERT_ALIGN8(twd);
    NASSERT(x != (int16_t*)y);
    NASSERT(scalingOpt==2); 
    NASSERT(N==256 || N==512 || N==1024);

    s = fft_cplx16x16_ie(y, (complex_fract16*)x, twd, twdstep * 2, N / 2, scalingOpt);

    splitPart16x16_ie_shift1((int16_t*)y, N, twd, twdstep);
    s += 1;

    return s;
} /* fft_real16x16_ie() */
