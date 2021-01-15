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
	NatureDSP Signal Processing Library. FFT part
    Inverse FFT on Real Data with Optimized Memory Usage
    C code optimized for HiFi4
	Integrit, 2006-2018
*/

/* Cross-platform data type definitions. */
#include "NatureDSP_types.h"
#include "NatureDSP_Signal_fft.h"
/* Common helper macros. */
#include "common.h"

#include "common_fpu.h"

#if (HAVE_FPU==0 && HAVE_VFPU==0)
DISCARD_FUN(int,ifft_realf_ie,(float32_t* y, complex_float* x, const  complex_float* twd, int twdstep, int N))
#elif (HAVE_VFPU)
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
#define SZ_CF32 (sizeof(complex_float))
#define XT_LSX2RXP(reg, addr, offs)\
{\
  ae_int64 t;\
  AE_L64_XP(t, castxcc(ae_int64,addr), offs);\
  reg = XT_AE_MOVXTFLOATX2_FROMINT32X2(AE_MOVINT32X2_FROMINT64(t));\
}
int ifft_realf_ie        (float32_t* y, complex_float* x, const  complex_float* twd, int twdstep, int N)
{
  const xtfloatx2 *restrict p_twd;
  const xtfloatx2 *restrict p0_ld;
  const xtfloatx2 *restrict p1_ld;
        xtfloatx2 *restrict p0_st;
        xtfloatx2 *restrict p1_st;
  xtfloatx2 a0, a1, a2, a3;
  xtfloatx2 b0, b1, b2, b3;
  int n;

  NASSERT( x );
  NASSERT( y );
  NASSERT( twd );
  NASSERT( (void*)x != y );
  NASSERT_ALIGN( x, 8 );
  NASSERT_ALIGN( y, 8 );
  NASSERT_ALIGN( twd, 8 );
  NASSERT( twdstep >= 1 );
  NASSERT( N>=8 && 0 == (N&(N-1)) );

  /*----------------------------------------------------------------------------
   First stage: Apply the real-to-complex spectrum conversion
  
   MATLAB code:
    twd = exp(-2*pi*1j*(0:N/4-1)/N);
    a0 = x(1:N/4);
    a1 = wrev(x(N/4+2:N/2+1));
    b0 = a0+conj(a1);
    b1 = (a0-conj(a1))*1j.*conj(twd);
    a0 = b0+b1;
    a1 = conj(b0-b1);
    x = [a0,conj(x(N/4+1)),wrev(a1(2:N/4))]/2; % N/2 complex samples
  */
  /*----------------------------------------------------------------------------
   Next stage: Perform the half-sized complex-valued inverse FFT
  
   MATLAB code:
    y = ifft(x);                        % Multiplies signal by 2/N!
    y = reshape([real(y);imag(y)],1,N); % N real samples
  */
  if ( N == 8 )
  {
    xtfloatx2 tw;
    xtfloatx2 c025, c05;

    p_twd = (const xtfloatx2 *)(twd+3*twdstep);
    p0_ld = (const xtfloatx2 *)(x);
    p0_st = (      xtfloatx2 *)(y);

    c05 = (xtfloatx2)0.5f;
    c025 = c05*c05;

    b0 = XT_LSX2I(p0_ld, 0*SZ_CF32);
    b1 = XT_LSX2I(p0_ld, 4*SZ_CF32);
    b2 = XT_LSX2I(p0_ld, 1*SZ_CF32);
    b3 = XT_LSX2I(p0_ld, 3*SZ_CF32);

    b0 = XT_SEL32_HH_SX2(b0,  b0);
    b1 = XT_SEL32_HH_SX2(b1, -b1);
    b0 = b0 + b1;
    /* Divide spectrum by 2 so that overall scale factor of RIFFT
     * is 1/2*2/N == N. */
    a0 = b0*c05;
    
    b3 = XT_CONJC_S(b3);
    b0 = b2 + b3;
    b1 = b2 - b3;

    /* twd <- conj(twd)*j */
    XT_LSX2RXP(tw, p_twd, 3*twdstep*SZ_CF32);
    /* a0 <- b0 + b1*twd */
    /* a1 <- b0 - b1*twd */ 
    b1 = XT_MULC_S(b1, tw);
    b2 = b0 + b1;
    b3 = b0 - b1;
    /* a0 <- 0.5*a0       */
    /* a1 <- 0.5*conj(a1) */
    a1 = c05*b2;
    a2 = XT_LSX2I(p0_ld, 2*SZ_CF32);
    a2 = XT_CONJC_S(a2);
    a3 = XT_MULMUX_S(c05, b3, 1);
    

    /* Radix-4 complex butterfly */
    b0 = a0 + a2;
    b1 = a1 + a3;
    b2 = a0 - a2;
    b3 = a1 - a3;
    
    a0 = (b0 + b1)*c025;
    a2 = (b0 - b1)*c025;
    /* a1 <- b2-j*b3 */
    /* a3 <- b2+j*b3 */
    a1 = a3 = b2*c025;
    XT_MADDMUX_S(a1, c025, b3, 1);
    XT_MADDMUX_S(a3, c025, b3, 3);

    XT_SSX2I(a0, p0_st, 0*SZ_CF32);
    XT_SSX2I(a1, p0_st, 1*SZ_CF32);
    XT_SSX2I(a2, p0_st, 2*SZ_CF32);
    XT_SSX2I(a3, p0_st, 3*SZ_CF32);
  }
  else
  {
    xtfloatx2 tw, c05;
    
    p_twd = (const xtfloatx2 *)(twd+3*twdstep);
    p0_st = (xtfloatx2 *)(x);
    p1_st = p0_st+(N>>1)-1;
    p0_ld = p0_st;
    p1_ld = p1_st;
    c05 = (xtfloatx2)0.5f;

    XT_LSX2IP(a0, p0_ld, SZ_CF32);
    a1 = XT_LSX2I(p1_ld, SZ_CF32);
    a0 = XT_SEL32_HH_SX2(a0,  a0);
    a1 = XT_SEL32_HH_SX2(a1, -a1);
    a0 = a0 + a1;

    /* Divide spectrum by 2 so that overall scale factor of RIFFT
     * is 1/2*2/N == N. */
    a0 = a0*c05;
    XT_SSX2IP(a0, p0_st, SZ_CF32);
    
    __Pragma("loop_count min=2")
    for ( n=1; n<(N>>2); n++ )
    {
      XT_LSX2IP(a0, p0_ld,       SZ_CF32);
      XT_LSX2XP(a1, p1_ld, -(int)SZ_CF32);
      
      a1 = XT_CONJC_S(a1);
      b0 = a0 + a1;
      b1 = a0 - a1;

      /* twd <- conj(twd)*j */
      XT_LSX2RXP(tw, p_twd, 3*twdstep*SZ_CF32);
      /* a0 <- b0 + b1*twd */
      /* a1 <- b0 - b1*twd */ 
      b1 = XT_MULC_S(b1, tw);
      a0 = b0 + b1;
      a1 = b0 - b1;
      /* a0 <- 0.5*a0       */
      /* a1 <- 0.5*conj(a1) */
      a0 = c05*a0;
      a1 = XT_MULMUX_S(c05, a1, 1);
      
      XT_SSX2IP(a0, p0_st,       SZ_CF32);
      XT_SSX2XP(a1, p1_st, -(int)SZ_CF32);
    }
    
    a0 = XT_LSX2I(p0_ld, 0);
    a0 = XT_CONJC_S(a0);
    XT_SSX2I (a0, p0_st, 0);

    p0_ld = (const xtfloatx2 *)(x);
    p0_st = (      xtfloatx2 *)(y);
  
    /* Divides signal by N/2 */
    ifft_cplxf_ie((complex_float*)y, x, twd, twdstep<<1, N>>1);
  }
  return 0;
} /* ifft_realf_ie() */
#else
// for scalar FPU
#include "baseop.h"
#define SZ_CF32 (sizeof(complex_float))
int ifft_realf_ie        (float32_t* y, complex_float* x, const  complex_float* twd, int twdstep, int N)
{
    const xtfloat* restrict pTwd;
          xtfloat* restrict pX0;
          xtfloat* restrict pX1;
    xtfloat a0_re, a1_re, a2_re, a3_re;
    xtfloat b0_re, b1_re, b2_re, b3_re;
    xtfloat a0_im, a1_im, a2_im, a3_im;
    xtfloat b0_im, b1_im, b2_im, b3_im;
    xtfloat twd_re,twd_im;
    int n;

    NASSERT( x );
    NASSERT( y );
    NASSERT( twd );

    NASSERT( (void*)x != y );

    NASSERT_ALIGN( x, 8 );
    NASSERT_ALIGN( y, 8 );
    NASSERT_ALIGN( twd, 8 );

    NASSERT( twdstep >= 1 );
    NASSERT( N>=8 && 0 == (N&(N-1)) );

  /*----------------------------------------------------------------------------
   Apply the real-to-complex spectrum conversion
  
   MATLAB code:
    twd = exp(-2*pi*1j*(0:N/4-1)/N);
    a0 = x(1:N/4);
    a1 = wrev(x(N/4+2:N/2+1));
    b0 = a0+conj(a1);
    b1 = (a0-conj(a1))*1j.*conj(twd);
    a0 = b0+b1;
    a1 = conj(b0-b1);
    x = [a0,conj(x(N/4+1)),wrev(a1(2:N/4))]/2; % N/2 complex samples
  */

  {
    pX0=(xtfloat* )x;
    pX1=(xtfloat* )(x+N/2-1);
    pTwd=(const xtfloat*)(twd+3*twdstep);

    b0_re=XT_LSI(pX0,0*sizeof(xtfloat));
    b0_im=XT_LSI(pX1,2*sizeof(xtfloat));

    a0_re = XT_MUL_S(XT_ADD_S(b0_re , b0_im),0.5f);
    a0_im = XT_MUL_S(XT_SUB_S(b0_re , b0_im),0.5f);
    XT_SSIP(a0_re,pX0,sizeof(xtfloat));
    XT_SSIP(a0_im,pX0,sizeof(xtfloat));

    for ( n=1; n<(N>>2); n++ )
    {
        a0_re=XT_LSI(pX0,0*sizeof(xtfloat));
        a0_im=XT_LSI(pX0,1*sizeof(xtfloat));
        a1_re=XT_LSI(pX1,0*sizeof(xtfloat));
        a1_im=XT_LSI(pX1,1*sizeof(xtfloat));

        /* b0 <- a0+conj(a1); */
        b0_re =  XT_ADD_S(a0_re , a1_re);
        b0_im =  XT_SUB_S(a0_im , a1_im);
        b1_re = XT_NEG_S(XT_ADD_S(a0_im , a1_im));
        b1_im =  XT_SUB_S(a0_re , a1_re);
        twd_im=XT_LSI(pTwd,sizeof(xtfloat));
        XT_LSXP(twd_re,pTwd,3*twdstep*sizeof(complex_float));

        /* b2<- b1*conj(twd); */
        b2_re=XT_MUL_S(b1_re,twd_re); XT_MADD_S(b2_re,b1_im,twd_im);
        b2_im=XT_MUL_S(b1_im,twd_re); XT_MSUB_S(b2_im,b1_re,twd_im);

        a0_re = XT_ADD_S(b0_re , b2_re);
        a0_im = XT_ADD_S(b0_im , b2_im);
        a1_re = XT_SUB_S(b0_re , b2_re);
        a1_im = XT_SUB_S(b2_im , b0_im);

        XT_SSIP(XT_MUL_S(a0_re,XT_CONST_S(3)),pX0,1*sizeof(xtfloat));
        XT_SSIP(XT_MUL_S(a0_im,XT_CONST_S(3)),pX0,1*sizeof(xtfloat));
        XT_SSI (XT_MUL_S(a1_im,XT_CONST_S(3)),pX1,1*sizeof(xtfloat));
        XT_SSXP(XT_MUL_S(a1_re,XT_CONST_S(3)),pX1,-2*(int)sizeof(xtfloat));
    }
    a0_im=XT_LSI(pX0,1*sizeof(xtfloat));
    XT_SSI(XT_NEG_S(a0_im),pX0,1*sizeof(xtfloat));
  }

  /*----------------------------------------------------------------------------
   Perform the half-sized complex-valued inverse FFT
  
   MATLAB code:
    y = ifft(x);                        % Multiplies signal by 2/N!
    y = reshape([real(y);imag(y)],1,N); % N real samples
  */

  if ( N ==8 )
  {
        xtfloat c=XT_MUL_S(XT_CONST_S(3),XT_CONST_S(3));
        a0_re=XT_LSI((const xtfloat*)x,0*sizeof(xtfloat));
        a0_im=XT_LSI((const xtfloat*)x,1*sizeof(xtfloat));
        a1_re=XT_LSI((const xtfloat*)x,2*sizeof(xtfloat));
        a1_im=XT_LSI((const xtfloat*)x,3*sizeof(xtfloat));
        a2_re=XT_LSI((const xtfloat*)x,4*sizeof(xtfloat));
        a2_im=XT_LSI((const xtfloat*)x,5*sizeof(xtfloat));
        a3_re=XT_LSI((const xtfloat*)x,6*sizeof(xtfloat));
        a3_im=XT_LSI((const xtfloat*)x,7*sizeof(xtfloat));

        b0_re = XT_ADD_S(a0_re , a2_re); b0_im = XT_ADD_S(a0_im , a2_im);
        b1_re = XT_ADD_S(a1_re , a3_re); b1_im = XT_ADD_S(a1_im , a3_im);
        b2_re = XT_SUB_S(a0_re , a2_re); b2_im = XT_SUB_S(a0_im , a2_im);
        b3_re = XT_SUB_S(a1_re , a3_re); b3_im = XT_SUB_S(a1_im , a3_im);
        a0_re = XT_ADD_S(b0_re , b1_re); a0_im = XT_ADD_S(b0_im , b1_im);
        a1_re = XT_SUB_S(b2_re , b3_im); a1_im = XT_ADD_S(b2_im , b3_re);
        a2_re = XT_SUB_S(b0_re , b1_re); a2_im = XT_SUB_S(b0_im , b1_im);
        a3_re = XT_ADD_S(b2_re , b3_im); a3_im = XT_SUB_S(b2_im , b3_re);

        XT_SSI(XT_MUL_S(a0_re,c),(xtfloat*)y,0*sizeof(xtfloat));
        XT_SSI(XT_MUL_S(a0_im,c),(xtfloat*)y,1*sizeof(xtfloat));
        XT_SSI(XT_MUL_S(a1_re,c),(xtfloat*)y,2*sizeof(xtfloat));
        XT_SSI(XT_MUL_S(a1_im,c),(xtfloat*)y,3*sizeof(xtfloat));
        XT_SSI(XT_MUL_S(a2_re,c),(xtfloat*)y,4*sizeof(xtfloat));
        XT_SSI(XT_MUL_S(a2_im,c),(xtfloat*)y,5*sizeof(xtfloat));
        XT_SSI(XT_MUL_S(a3_re,c),(xtfloat*)y,6*sizeof(xtfloat));
        XT_SSI(XT_MUL_S(a3_im,c),(xtfloat*)y,7*sizeof(xtfloat));
  }
  else
  {
    /* Divides signal by N/2 */
    ifft_cplxf_ie((complex_float*)y, x,  twd, twdstep*2, N>>1 );
  }

  return 0;
}
#endif
