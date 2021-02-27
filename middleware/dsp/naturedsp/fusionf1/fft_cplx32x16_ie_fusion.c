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
	NatureDSP Signal Processing Library. FFT part
    C code optimized for Fusion
*/
/*===========================================================================
Fast Fourier Transforms:
fft_cplx             FFT on Complex Data
fft_real             FFT on Real Data
ifft_cplx            IFFT on Complex Data
ifft_real            Inverse FFT Forming Real Data
fft_cplx<prec>_ie    FFT on Complex Data with optimized memory usage
fft_real<prec>_ie    FFT on Real Data with optimized memory usage
ifft_cplx<prec>_ie   IFFT on Complex Data with optimized memory usage
ifft_real<prec>_ie   Inverse FFT Forming Real Data with optimized memory usage
dct                  Discrete Cosine Transform

There are limited combinations of precision and scaling options available:
----------------+---------------------------------------------------------------
FFT/IFFT        | Scaling options                        | Restrictions on the
                |                                        | input dynamic range
----------------+---------------------------------------------------------------
cplx24x24       | 0 ÿ no scaling                         | input signal < 2^23/(2*N),
                |                                        | N-fft-size
real24x24       | 1 ÿ 24-bit scaling                     |        none
                | 2 ÿ 32-bit scaling on the first stage  |        none
                | and 24-bit scaling later               |        none
                | 3 ÿ fixed scaling before each stage    |        none
------------------------------------------------------------------------------------
cplx32x16       | 3 ÿ fixed scaling before each stage    |        none
cplx16x16       | 3 ÿ fixed scaling before each stage    |        none
cplx32x16_ie    | 3 ÿ fixed scaling before each stage    |        none
cplx24x24_ie    | 3 ÿ fixed scaling before each stage    |        none
real32x16       | 3 ÿ fixed scaling before each stage    |        none
real16x16       | 3 ÿ fixed scaling before each stage    |        none
real32x16_ie    | 3 ÿ fixed scaling before each stage    |        none
real24x24_ie    | 3 ÿ fixed scaling before each stage    |        none
real32x16_ie_24p| 3 ÿ fixed scaling before each stage    |        none
----------------+---------------------------------------------------------------
real24x24_ie_24p| 1 ÿ 24-bit scaling                     |        none
----------------+---------------------------------------------------------------
DCT:            |
----------------+---------------------------------------------------------------
24x24	          | 0 ÿ no scaling                         |        none
32x16	          | 0 ÿ no scaling                         |        none
----------------+---------------------------------------------------------------

===========================================================================*/
#include "NatureDSP_Signal.h"
#include "common.h"
#if 0


#define SCALING_BFL4  2 // Right shift for a inner stages radix 4
// Complex conjugation
#define CONJ_F16X4(x)\
{\
    ae_f16x4 tmp;\
    ae_int16x4 x0, xj; \
    tmp = AE_NEG16S(x);\
    xj = AE_MOVINT16X4_FROMF16X4(tmp);\
    x0 = AE_MOVINT16X4_FROMF16X4(x);\
    AE_MOVT16X4(x0, xj, b4);\
    x = AE_MOVF16X4_FROMINT16X4(x0);\
}

#define LF32X2_X(result, ptr, offset) \
{\
    ae_int32x2 t; \
    t = AE_L32X2_X(ptr, offset); \
    result = AE_MOVF32X2_FROMINT32X2(t); \
}
    
#define SF32X2_X(d, ptr, offset) \
{\
    ae_int32x2 t = AE_MOVINT32X2_FROMF32X2(d); \
    AE_S32X2_X(t, ptr, offset); \
}

#define SF32X2_XC(d, ptr, offset) \
{\
    ae_int32x2 t = AE_MOVINT32X2_FROMF32X2(d); \
    AE_S32X2_XC(t, ptr, offset); \
}

#define LF32X2_XC(result, ptr, inc) \
{\
    ae_int32x2 t; \
    AE_L32X2_XC(t, ptr, inc); \
    result = AE_MOVF32X2_FROMINT32X2(t); \
}


#define LF32X2_XP(result, ptr, inc) \
{\
    ae_int32x2 t; \
    AE_L32X2_XP(t, ptr, inc); \
    result = AE_MOVF32X2_FROMINT32X2(t); \
}


#define LF32_X(result, ptr, offset) \
{\
    ae_int32x2 t; \
    t = AE_L32_X(ptr, offset); \
    result = AE_MOVF32X2_FROMINT32X2(t); \
}

#define SWAP_F32X2(inout)\
{\
    ae_int32x2 tmp = AE_MOVINT32X2_FROMF32X2(inout); \
    tmp  = AE_SEL32_LH(tmp, tmp); \
    inout = AE_MOVF32X2_FROMINT32X2(tmp); \
}

#define SEL_F32X2(y, x, h)\
{\
    ae_int32x2 tmp = AE_MOVINT32X2_FROMF32X2(x); \
    tmp  = AE_SEL32_##h(tmp, tmp); \
    y = AE_MOVF32X2_FROMINT32X2(tmp); \
}


inline_ ae_f32x2 subadd_f32x2(const ae_f32x2 *x, const ae_f32x2 *y)
{
    ae_int32x2 t0 = AE_MOVINT32X2_FROMF32X2(*x), 
               t1 = AE_MOVINT32X2_FROMF32X2(*y); 

    t0 = AE_SUBADD32S(t0, t1); 
    return AE_MOVF32X2_FROMINT32X2(t0); 
}

inline_ ae_f32x2 addsub_f32x2(const ae_f32x2 *x, const ae_f32x2 *y)
{
    ae_int32x2 t0 = AE_MOVINT32X2_FROMF32X2(*x), 
               t1 = AE_MOVINT32X2_FROMF32X2(*y); 

    t0 = AE_ADDSUB32S(t0, t1); 
    return AE_MOVF32X2_FROMINT32X2(t0); 
}

#define SUBADD_F32X2(X, Y) subadd_f32x2(&(X), &(Y)) 
#define ADDSUB_F32X2(X, Y) addsub_f32x2(&(X), &(Y)) 


#define LF32X2_I(data, ptr, offset)\
{   \
    ae_int32x2 t0; \
    t0 = AE_L32X2_I(ptr, offset); \
    data = AE_MOVF32X2_FROMINT32X2(t0); \
}



#define LF32X2_I(data, ptr, offset)\
{   \
    ae_int32x2 t0; \
    t0 = AE_L32X2_I(ptr, offset); \
    data = AE_MOVF32X2_FROMINT32X2(t0); \
}



#define FFT_BUTTERFLY_R4(idx, _s_)       \
    {                               \
      LF32X2_XC(vA0, p_x0, 8);   \
      LF32X2_XC(vA1, p_x0, 8);    \
      LF32X2_XC(vA2, p_x0, 8);   \
      LF32X2_XC(vA3, p_x0, 8);   \
       vA0 =  AE_SRAA32RS(vA0, _s_); \
      vA1 =  AE_SRAA32RS(vA1, _s_); \
      vA2 =  AE_SRAA32RS(vA2, _s_); \
      vA3 =  AE_SRAA32RS(vA3, _s_); \
      vB0 = AE_ADD32S(vA0, vA2);    \
      vB2 = AE_SUB32S(vA0, vA2);    \
      vB1 = AE_ADD32S(vA1, vA3);    \
      vB3 = AE_SUB32S(vA1, vA3);    \
      SWAP_F32X2(vB3);  \
      vA0 = AE_ADD32S(vB0, vB1);    \
      vA2 = AE_SUB32S(vB0, vB1);    \
      vA1 = ADDSUB_F32X2(vB2, vB3); \
      vA3 = SUBADD_F32X2(vB2, vB3); \
      SF32X2_X(vA0, p_y0, idx);   \
      SF32X2_X(vA1, p_y1, idx);   \
      SF32X2_X(vA2, p_y2, idx);   \
      SF32X2_X(vA3, p_y3, idx);   \
    }
// Last stage Radix 2 always no scaling
#define FFT_BUTTERFLY_R2(idx, _s_)       \
    {                               \
      LF32X2_XC(vA0, p_x0, 8);   \
      LF32X2_XC(vA1, p_x0, 8);   \
      LF32X2_XC(vA2, p_x0, 8);   \
      LF32X2_XC(vA3, p_x0, 8);    \
     vA0 =  AE_SRAA32RS(vA0, _s_);\
     vA1 =  AE_SRAA32RS(vA1, _s_);\
     vA2 =  AE_SRAA32RS(vA2, _s_);\
     vA3 =  AE_SRAA32RS(vA3, _s_);\
      vB0 = AE_ADD32S(vA0, vA1);    \
      vB2 = AE_SUB32S(vA0, vA1);    \
      vB1 = AE_ADD32S(vA2, vA3);    \
      vB3 = AE_SUB32S(vA2, vA3);    \
      SF32X2_X(vB0, p_y0, idx);   \
      SF32X2_X(vB1, p_y1, idx);   \
      SF32X2_X(vB2, p_y2, idx);   \
      SF32X2_X(vB3, p_y3, idx);   \
    }











static void fft_stage_last(int32_t *x, 
                int32_t *y, 
                int N, int shift)
{
    int32_t i,i0,i1,/*i2,i3,*/ai;
    ae_int32x2 * restrict p_y0 = (ae_int32x2 *)(y);
    ae_int32x2 * restrict p_y1 = (p_y0 + (N >> 2));
    ae_int32x2 * restrict p_y2 = (p_y1 + (N >> 2));
    ae_int32x2 * restrict p_y3 = (p_y2 + (N >> 2));
    ae_int32x2 * restrict p_x0 = (ae_int32x2 *)(x);


    ae_f32x2 vA1, vA2, vA3, vA0; 
    ae_f32x2 vB1, vB2, vB3, vB0; 
    ae_int32x2 tmp; 

    tmp = AE_MOVDA32X2(N, N); 
    i = AE_NSAZ32_L(tmp)+1;
    ai=((int32_t)0x1)<<i;
    i0=0;

    WUR_AE_CBEGIN0((unsigned)(uintptr_t)p_x0); 
    WUR_AE_CEND0((unsigned)(uintptr_t)(p_x0 + N));   

    if ( (i&1)==0 ) 
    {
        //--------------------------------------------------------------------------
        // last stage is RADIX2 !!!
        //--------------------------------------------------------------------------
/*
#<loop> Loop body line 262, nesting depth: 1, estimated iterations: 100
#<swps> 
#<swps>  26 cycles per pipeline stage in steady state with unroll=1
#<swps>   1 pipeline stages
#<swps>  34 real ops (excluding nop)
#<swps> 
#<swps>      min  18 cycles required by resources
#<swps>      min   8 cycles required by recurrences
#<swps>      min  18 cycles required by resources/recurrence
#<swps>      min  11 cycles required for critical path
#<swps>           26 cycles non-loop schedule length
*/
        i = N>>3; 
        do
        {
          i1 = AE_ADDBRBA32(i0, ai);
          FFT_BUTTERFLY_R2(i0, shift);
          FFT_BUTTERFLY_R2(i1, shift);
          i0 = AE_ADDBRBA32(i1, ai);
        }while(--i); 
    } 
    else 
    {  
        //--------------------------------------------------------------------------
        // last stage is RADIX4 !!!
        //--------------------------------------------------------------------------
/*
#<loop> Loop body line 281, nesting depth: 1, estimated iterations: 100
#<swps> 
#<swps>  13 cycles per pipeline stage in steady state with unroll=1
#<swps>   2 pipeline stages
#<swps>  22 real ops (excluding nop)
#<swps> 
#<swps>      min   9 cycles required by resources
#<swps>      min   4 cycles required by recurrences
#<swps>      min   9 cycles required by resources/recurrence
#<swps>      min   9 cycles required for critical path
#<swps>           20 cycles non-loop schedule length
*/  
        i = N>>2; 
        do//for (i = 0; i < (N>>4); i++) 
        {
          FFT_BUTTERFLY_R4(i0, shift);
          i0 = AE_ADDBRBA32(i0, ai);
        }while(--i); 
    }
} //fft_stage_last
#endif
extern int fft_stage_last_ie( int32_t *x, 
                               int32_t *y, 
                               int N); 
/*-------------------------------------------------------------------------
  These functions make FFT on complex data with optimized memory usage.
  Scaling  : Fixed data scaling at each stage
  NOTES:
  1. Bit-reversing reordering is done here.
  2. FFT runs in-place algorithm so INPUT DATA WILL APPEAR DAMAGED after 
     the call
  3. Forward/inverse FFT of size N may be supplied with constant data
     (twiddle factors) of a larger-sized FFT = N*twdstep.

  Precision: 
  24x24_ie      24-bit input/outputs, 24-bit twiddles
  32x16_ie      32-bit input/outputs, 16-bit twiddles
  f             floating point
 
  Input:
  x[N]                  complex input signal. Real and imaginary data are interleaved 
                        and real data goes first
  twd[N*twdstep*3/4]    twiddle factor table of a complex-valued FFT of size N*twdstep
  N                     FFT size
  twdstep               twiddle step 
  scalingOpt            scaling option = 3 (fixed scaling)
  Output:
  y[N]                  output spectrum. Real and imaginary data are interleaved and 
                        real data goes first

  Returned value: total number of right shifts occurred during scaling 
                  procedure

  Restrictions:
  x,y - should not overlap
  x,y - aligned on 8-bytes boundary
  N   - 256, 512, 1024 for fixed-point routines, power of 2 and >=8 for 
        floating point

-------------------------------------------------------------------------*/

int fft_cplx32x16_ie( complex_fract32* y,complex_fract32* x, const complex_fract16* twd, int twdstep, int N, int scalingOpt)
{
    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);
    NASSERT(scalingOpt==3);
    NASSERT(N==128||N==256||N==512||N==1024);

    int shift = 0;
    int M = twdstep*N;
    int s;
    int tw_step = twdstep;
    int stride = N/4;     

    ae_int32x2 * restrict px;
    ae_int32x2 * restrict py;

    ae_int32   * restrict p16tw1; 
    ae_int32   * restrict p16tw2;
    ae_int32   * restrict p16tw3;
 
    ae_int32x2  vA0, vA1, vA2, vA3, 
                vB0, vB1, vB2, vB3, 
                vC0, vC1, vC2, vC3;
    int i; 
    int log2n = 0; 

    WUR_AE_CBEGIN0((unsigned)x); 
    WUR_AE_CEND0((unsigned)(uintptr_t)(x+(N-1)));     

    s = 3; 

    while( stride > 1 )
    {
        unsigned acc = 0; 
        const unsigned acc_inc  = (log2n==0)? 0: (0x80000000 >> (log2n-1)); 
          
    
        WUR_AE_SAR(s); 
        p16tw1 = (ae_int32*)  twd; 
        p16tw2 = (ae_int32*) (twd+1*M/4);
        p16tw3 = (ae_int32*) (twd+2*M/4);

        log2n += 2;   
        shift += s;
        s = 2; 

        px = (ae_int32x2*)x; 
        py = (ae_int32x2*)x; 
  
        i = N/4;
/*
#<loop> Loop body line 381, nesting depth: 2, estimated iterations: 100
#<swps> 
#<swps>  12 cycles per pipeline stage in steady state with unroll=1
#<swps>   3 pipeline stages
#<swps>  32 real ops (excluding nop)
#<swps> 
#<swps>      min  11 cycles required by resources
#<swps>      min   4 cycles required by recurrences
#<swps>      min  11 cycles required by resources/recurrence
#<swps>      min  12 cycles required for critical path
#<swps>           27 cycles non-loop schedule length

#<swps>    register file usage:
#<swps>      'a' total 10 out of 16 [2-3,8-15]
#<swps>      'b' total 1 out of 16 [0]
#<swps>      'aed' total 14 out of 16 [0-13]
*/
        do            
        {
            
            int offset_inc = 0; 
            acc+=acc_inc;
            XT_MOVEQZ(offset_inc, tw_step*4, acc); 

            ae_int32x2 t1, t2, t3; 
            ae_f16x4 t1_f16x4; 
            ae_f16x4 t2_f16x4; 
            ae_f16x4 t3_f16x4; 
            
            vA3 = AE_L32X2_X( px,  3*8*stride); 
            vA2 = AE_L32X2_X(  px,  2*8*stride); 
            vA1 = AE_L32X2_X(  px,  1*8*stride); 
            AE_L32X2_XC(vA0, px,  4*8*stride); 

            vA3 = AE_SRAS32(vA3); 
            vA2 = AE_SRAS32(vA2); 
            vA1 = AE_SRAS32(vA1); 
            vA0 = AE_SRAS32(vA0); 

            vB0 = AE_ADD32S(vA0, vA2);
            vB2 = AE_SUB32S(vA0, vA2);
            vB1 = AE_ADD32S(vA1, vA3);
            vB3 = AE_SUB32S(vA1, vA3);
            
            vB3 = AE_SEL32_LH(vB3, vB3);

            vC0 = AE_ADD32S(vB0, vB1);
            vC2 = AE_SUB32S(vB0, vB1);
            vC3 = AE_SUBADD32S(vB2, vB3);
            vC1 = AE_ADDSUB32S(vB2, vB3);

            AE_L32_XP( t1 ,p16tw1, offset_inc);
            AE_L32_XP( t2 ,p16tw2, offset_inc);
            AE_L32_XP( t3 ,p16tw3, offset_inc);

            t1_f16x4 = AE_MOVF16X4_FROMINT32X2(t1);            
            t2_f16x4 = AE_MOVF16X4_FROMINT32X2(t2); 
            t3_f16x4 = AE_MOVF16X4_FROMINT32X2(t3); 

            vC1 = AE_MULFC32X16RAS_L(vC1, t1_f16x4); 
            vC2 = AE_MULFC32X16RAS_L(vC2, t2_f16x4); 
            vC3 = AE_MULFC32X16RAS_L(vC3, t3_f16x4); 

            AE_S32X2_X (vC3, py,  3*8*stride); 
            AE_S32X2_X (vC2, py,  1*8*stride); 
            AE_S32X2_X (vC1, py,  2*8*stride); 
            AE_S32X2_XC(vC0, py,  4*8*stride);     


        }while(--i);

        stride>>=2;  
        tw_step<<=2;

    }
  
    shift += fft_stage_last_ie((int32_t*)x, (int32_t*)y, N); 

  return shift;
}
  
