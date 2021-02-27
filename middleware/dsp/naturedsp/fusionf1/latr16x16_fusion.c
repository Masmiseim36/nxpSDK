/* ------------------------------------------------------------------------ */
/* Copyright (c) 2016 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (ÿCadence    */
/* Librariesÿ) are the copyrighted works of Cadence Design Systems Inc.	    */
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
  NatureDSP Signal Processing Library. IIR part
    Lattice Block Real IIR, 16x16-bit
    Optimized code for Fusion
*/

/*-------------------------------------------------------------------------
  Lattice Block Real IIR
  Computes a real cascaded lattice autoregressive IIR filter using reflection 
  coefficients stored in vector k. The real data input are stored in vector x.
  The filter output result is stored in vector r.  Input scaling is done before 
  the first cascade for normalization and overflow protection..

  Precision: 
  16x16  16-bit data, 16-bit coefficients
  24x24  24-bit data, 24-bit coefficients
  32x16  32-bit data, 16-bit coefficients
  32x32  32-bit data, 32-bit coefficients
  f      single precision floating point

  Input:
  N      length of input sample block
  M      number of reflection coefficients
  scale  input scale factor g, Q31, Q15 or floating point
  k[M]   reflection coefficients, Q31, Q15 or floating point
  x[N]   input samples, Q31, Q15 or floating point
  Output:
  r[N]   output data, Q31, Q15 or floating point

  Restriction:
  x,r,k should not overlap

  PERFORMANCE NOTE:
  for optimum performance follow rules:
  M - from the range 1...8
-------------------------------------------------------------------------*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common utility and macros declarations. */
#include "common.h"

#define sz_i16 sizeof(int16_t)

/* AE_SEL32_LL adapted for ae_f32x2 data type. */
#define select_f32x2_ll( s_out, s_in0, s_in1 ) \
  { \
    ae_int32x2 t0 = ( s_in0 ); \
    ae_int32x2 t1 = ( s_in1 ); \
    ae_int32x2 t2; \
    \
    t2 = AE_SEL32_LL( t0, t1 ); \
    \
    s_out = ( t2 ); \
  }

/* AE_SEL32_LH adapted for ae_f32x2 data type. */
#define select_f32x2_lh( s_out, s_in0, s_in1 ) \
  { \
    ae_int32x2 t0 = ( s_in0 ); \
    ae_int32x2 t1 = ( s_in1 ); \
    ae_int32x2 t2; \
    \
    t2 = AE_SEL32_LH( t0, t1 ); \
    \
    s_out = ( t2 ); \
  }

/* Instance pointer validation number. */
#define MAGIC     0x52efa966

/* Reserve memory for alignment. */
#define ALIGNED_SIZE( size, align ) \
      ( (size_t)(size) + (align) - 1 )

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
      (void*)( ( (uintptr_t)(addr) + ( (align) - 1 ) ) & ~( (align) - 1 ) )


/* Lattice filter data processing function. */
typedef void (proc_fxn_t)( int16_t * r,      // r[N]     [out   ] Q15
                     const int16_t * x,      // x[N]     [in    ] Q15
                           int16_t * delLine,// dline[M] [in/out] Q14
                     const int16_t * coef,   // coef[M]  [in    ] Q15
                           int16_t scale,    // scale    [in    ] Q15
                           int N, int M );




/* Filter instance structure. */
typedef struct tag_latr16x16_t
{
  uint32_t        magic;     // Instance pointer validation number
  int             M;         // Lattice filter order
  proc_fxn_t *    procFxn;   // Custom data processing function
  int16_t         scale;     // Input signal prescaling factor, Q15
  const int16_t * coef;      // M reflection coefficients, Q15
  int16_t *       delayLine; // M delay elements, Q14

} latr16x16_t, *latr16x16_ptr_t;
/* Custom data processing functions for particular lattice orders. */
static proc_fxn_t latr1_proc;
static proc_fxn_t latr2_proc;
static proc_fxn_t latr3_proc;
static proc_fxn_t latr4_proc;
static proc_fxn_t latr5_proc;
static proc_fxn_t latr6_proc;
static proc_fxn_t latr7_proc;
static proc_fxn_t latr8_proc;
static proc_fxn_t latrX_proc;
/* Table of processing functions */
static proc_fxn_t * const proc_fxn_tbl[9] =
{&latr1_proc, &latr2_proc, &latr3_proc, &latr4_proc, 
&latr5_proc, &latr6_proc, &latr7_proc, &latr8_proc, &latrX_proc};

// Determine the memory area size for a filter instance.
size_t latr16x16_alloc( int M )
{
  ASSERT( M > 0 );

  return ( ALIGNED_SIZE( sizeof( latr16x16_t ), 4 )
           + // M delay elements
           ALIGNED_SIZE( M*sz_i16, 4*sz_i16 )
           + // M reflection coefficients
           ALIGNED_SIZE( M*sz_i16, 4*sz_i16 ) );

} // latr16x16_alloc()

// Given a memory area of sufficient size, initialize a filter instance and 
// return a handle to it.
latr16x16_handle_t latr16x16_init( void *             objmem, 
                                   int                M,
                                   const int16_t * restrict k,
                                   int16_t            scale )
{
  latr16x16_ptr_t latr;
  int16_t *       delayLine;
  int16_t *       coef;
  void *          ptr;

  int m;

  ASSERT( objmem && M > 0 && k );

  //
  // Partition the memory block
  //

  ptr = objmem;

  latr      = (latr16x16_ptr_t)ALIGNED_ADDR( ptr, 4 );
  ptr       = latr + 1;
  delayLine = (int16_t*)ALIGNED_ADDR( ptr, 4*sz_i16 );
  ptr       = delayLine + M;
  coef      = (int16_t*)ALIGNED_ADDR( ptr, 4*sz_i16 );
  ptr       = coef + M;

  ASSERT( (int8_t*)ptr - (int8_t*)objmem <= (int)latr16x16_alloc( M ) );

  //
  // Copy reflection coefficients, zero the delay line.
  //

  for ( m=0; m<M; m++ )
  {
    coef[m] = k[m];

    delayLine[m] = 0;
  }

  //
  // Initialize the filter instance.
  //

  latr->magic     = MAGIC;
  latr->M         = M;
  latr->scale     = scale;
  latr->coef      = coef;
  latr->delayLine = delayLine;

  //
  // Set the correct processing function.
  //

  M = M>8 ? 8 : M-1;
  latr->procFxn = proc_fxn_tbl[M];

  return (latr);

} // latr16x16_init()

// Process data. The filter instance is identified with a handle returned by
// the initializing function.
void latr16x16_process( latr16x16_handle_t _latr, 
                        int16_t * restrict     r,
                  const int16_t * restrict     x, int N )
{
  latr16x16_ptr_t latr = (latr16x16_ptr_t)_latr;

        int16_t * delLine;
  const int16_t * coef;
  int16_t scale;
  int M;

  ASSERT( latr && latr->magic == MAGIC && r && x );

  delLine = latr->delayLine;
  coef    = latr->coef;
  scale   = latr->scale;
  M       = latr->M;

  if (N <= 0) return;
  latr->procFxn(r, x, delLine, coef, scale, N, M);

} // latr16x16_process()

/* Order 1 lattice filter data processing function. */
void latr1_proc( int16_t * r,       const int16_t * x,
                 int16_t * delLine, const int16_t * coef,
                 int16_t scale, int N, int M )
{
  const ae_int16x4 * restrict pin;
        ae_int16x4 * restrict pout;
  ae_int16x4 xin, rout;
  ae_int16x4 dl, sc, cf;
  ae_f32x2 acc, rout32;

  int n;

  ASSERT(M == 1);
  // Set the input and output pointers
  pin  = (const ae_int16x4 *)x;
  pout = (      ae_int16x4 *)r;
  // Load the scale and reflection coefficients, the delay element
  sc = AE_MOVDA16X2(scale, scale);
  // sc : Q14 <- Q15 - 1 w/ rounding
  sc = AE_SRAI16R(sc, 1);
  dl = *delLine;
  cf = *coef;
  
  // Load the first input sample
  AE_L16_IP(xin, castxcc(const ae_int16, pin), sz_i16);
  // Scale the input sample.
  // Q30 <- ( Q15*Q14 + 1 )
  acc = AE_MULF16SS_00(xin, sc);
  // Q30 <- Q30 - ( Q14*Q15 + 1 )
  AE_MULSF16SS_20(acc, dl, cf );
  // Update the delay element
  // Q14 <- Q30 - 16 w/ rounding and saturation
  dl = AE_ROUND16X4F32SASYM(acc, acc);

  for ( n=0; n<N-1; n++ )
  {
    // Prepare the output sample for storing in Q15
    rout32 = AE_F32X2_SLAIS(acc, 1);
    // Load next input sample
    AE_L16_IP(xin, castxcc(const ae_int16, pin), sz_i16);
    // Scale the input sample.
    // Q30 <- ( Q15*Q14 + 1 )
    acc = AE_MULF16SS_00(xin, sc);
    // Compute the output sample
    // Q30 <- Q30 - ( Q14*Q15 + 1 )
    AE_MULSF16SS_20(acc, dl, cf );
    // Format the delay element and the previous output sample.
    // dl   : Q14 <- Q30 - 16 w/ rounding and saturation
    // rout : Q15 <- Q31 - 16 w/ rounding and saturation
    rout = AE_ROUND16X4F32SASYM(acc, rout32);
    // Store the previous output sample
    AE_S16_0_IP(rout, castxcc(ae_int16, pout), sz_i16);
    // Update the delay element
    dl = rout;
  }
  // Format the previous output sample.
  // Q15 <- Q30 + 1 - 16 w/ rounding and saturation
  rout32 = AE_F32X2_SLAIS(acc, 1);
  rout = AE_ROUND16X4F32SASYM(rout32, rout32);
  // Store the last output sample
  AE_S16_0_IP(rout, castxcc(ae_int16, pout), sz_i16);

  // Save the updated delay element
  *delLine = AE_MOVINT16X4_FROMINT32X2(AE_INTSWAP(AE_MOVINT32X2_FROMINT16X4(dl)));

} // latr1_proc()



/* Order 2 lattice filter data processing function. */
void latr2_proc( int16_t * r,       const int16_t * x,
                 int16_t * delLine, const int16_t * coef,
                 int16_t scale, int N, int M )
{
  const ae_int16x4 * restrict pin;
        ae_int16x4 * restrict pout;
  ae_int16x4 xin, rout;
  ae_int16x4 dl0, dl1, cf0, cf1, cf10, sc;
  ae_f32x2 acc0, acc1, rout32;

  int n;

  ASSERT(M == 2);
  // Set the input and output pointers
  pin  = (const ae_int16x4 *)x;
  pout = (      ae_int16x4 *)r;
  // Load the scale and reflection coefficients, the delay elements
  sc = AE_MOVDA16X2(scale, scale);
  // sc : Q14 <- Q15 - 1 w/ rounding
  sc = AE_SRAI16R(sc, 1);
  dl0 = delLine[0];
  dl1 = delLine[1];
  cf0 = coef[0];
  cf1 = coef[1];
  cf10 = AE_SEL16_5432(cf1, cf0);// cf1 cf1 cf0 cf0
  // Update accumulator for dl1 for the next iteration
  // Q30 <- Q14 + 16
  acc1 = AE_CVT32X2F16_10(dl0);
  
  //
  // Pass the input samples block through the AR lattice. n-th response sample
  // and lattice state update are defined as follows:
  //
  //   r0 = sc*x[n] - cf1*dl1 - cf0*dl0
  //
  //   dl1 = dl0 + cf0*r0;
  //   dl0 = r0;
  //
  //   r[n] = r0;
  //
  // The inner loop is fully unrolled.
  //

  __Pragma("loop_count min=1")
  for ( n=0; n<N; n++ )
  {
    // Load the input sample
    AE_L16_IP(xin, castxcc(const ae_int16, pin), sz_i16);
    // Scale the input sample.
    // Q30 <- ( Q15*Q14 + 1 )
    acc0 = AE_MULF16SS_00(xin, sc);
    // Compute the output sample
    // Q30 <- Q30 - ( Q14*Q15 + 1 )
    AE_MULSF16SS_00(acc0, dl0, cf10);
    AE_MULSF16SS_22(acc0, dl1, cf10);
    // Update delay elements
    // dl1 = dl0 + r[n]*cf0
    // dl0 = r[n] : Q14 <- Q30 - 16 w/ rounding and saturation
    dl0 = AE_ROUND16X4F32SASYM(acc0, acc0);
    AE_MULAF16SS_00(acc1, dl0, cf10);
    // Format the previous output sample and delay element.
    // dl1: Q14 <- Q30 - 16 w/ rounding and saturation
    // r  : Q15 <- Q31 + 1 - 16 w/ rounding and saturation
    rout32 = AE_F32X2_SLAIS(acc0, 1);
    dl1 = AE_ROUND16X4F32SASYM(acc1, rout32);
    rout = dl1;
    // Store the output sample
    AE_S16_0_IP(rout, castxcc(ae_int16, pout), sz_i16);
    // Update accumulator for dl1 for the next iteration
    acc1 = acc0;
  }

  // Save the updated delay elements
  dl1 = AE_MOVINT16X4_FROMINT32X2(AE_INTSWAP(AE_MOVINT32X2_FROMINT16X4(dl1)));
  delLine[0] = dl0;
  delLine[1] = dl1;

} // latr2_proc()



/* Order 3 lattice filter data processing function. */
void latr3_proc( int16_t * r,       const int16_t * x,
                 int16_t * delLine, const int16_t * coef,
                 int16_t scale, int N, int M )
{
  const ae_f16x4 * restrict pin;
        ae_f16x4 * restrict pout;
  ae_int16x4 X, zero;
  ae_f16x4 xin, r1, r0, rout;
  ae_f16x4 dl0, dl1, dl2, cf0, cf1, cf2, sc, cf2sc;
  ae_f32x2 acc, rout32, rnd0, rnd1;

  int n;

  ASSERT(M == 3);

  zero = AE_ZERO16();
  rnd0 = AE_MOVI(1);
  // rounding term for Q1.30 -> Q15 conversion
  rnd1 = AE_SLAI32(rnd0, 14);
  // rounding term for Q1.30 -> Q1.14 conversion
  rnd0 = AE_SLAI32(rnd0, 15);
  // Set the input and output pointers
  pin  = (const ae_f16x4 *)x;
  pout = (      ae_f16x4 *)r;
  // Load the scale and reflection coefficients, the delay elements
  sc = AE_MOVDA16X2(scale, scale);
  dl0 = (ae_int16x4)delLine[0];
  dl1 = (ae_int16x4)delLine[1];
  dl2 = (ae_int16x4)delLine[2];
  cf0 = (ae_int16x4)coef[0];
  cf1 = (ae_int16x4)coef[1];
  cf2 = (ae_int16x4)coef[2];
  // Put coefficients to the necessary order in registers:
  // cf2sc : cf2 cf2 sc  sc
  // cf0   :  z   z  cf0 0
  // cf1   :  z   z  cf1 0
  cf2sc = AE_SEL16_5432(cf2, sc);
  cf0 = AE_SEL16_6543(cf0, zero);
  cf1 = AE_SEL16_6543(cf1, zero);
  
  //
  // Pass the input samples block through the AR lattice. n-th response sample
  // and lattice state update are defined as follows:
  //
  //   r1 = sc*x[n] - cf2*dl2 - cf1*dl1
  //   r0 = r1 - cf0*dl0;
  //
  //   dl2 = dl1 + cf1*r1;
  //   dl1 = dl0 + cf0*r0;
  //   dl0 = r0;
  //
  //   r[n] = r0;
  //
  // The inner loop is fully unrolled.
  //

  __Pragma("loop_count min=1")
  for ( n=0; n<N; n++ )
  {
    // Load the input sample
    AE_L16_IP(X, castxcc(const ae_int16, pin), sz_i16);
    xin = X;
    // Scale the input sample.
    // Q30 <- ( Q15*Q15 + 1 ) - 1 w/ rounding
    acc = AE_MULF16SS_00(cf2sc, xin);
    acc = AE_SRAI32R(acc, 1);
    // Add rounding term to the accumulator
    acc = AE_ADD32S(acc, rnd0);
    // Compute output samples of each section
    // Q30 <- Q30 - ( Q14*Q15 + 1 )
    AE_MULSF16SS_21(acc, cf2sc, dl2);
    AE_MULSF16SS_11(acc, cf1  , dl1);
    r1 = AE_MOVINT16X4_FROMINT32X2(acc);
    AE_MULSF16SS_11(acc, cf0  , dl0);
    r0 = AE_MOVINT16X4_FROMINT32X2(acc);
    // Update delay elements
    // Use high halves of 32-bit values r1, r0 in multiplications;
    // these values are correctly rounded due to adding the rounding term.
    // dl2 = dl1 + cf1*r1;
    // dl1 = dl0 + cf0*r0;
    // dl0 = r0;
    // Q14 <- Q14 + (Q14*Q15 + 1 - 16) w/ rounding and saturation
#if (XCHAL_HAVE_FUSION_16BIT_BASEBAND)
    dl2 = dl1;
    AE_MULAFC16RAS_L(dl2, r1, cf1);
    dl1 = dl0;
    AE_MULAFC16RAS_L(dl1, r0, cf0);
    dl0 = r0;
#else
    {
      ae_f32x2 t;
      t = AE_MOVF32X2_FROMF16X4(dl1);
      AE_MULAF16SS_11(t, r1, cf1);
      dl2 = AE_MOVF16X4_FROMF32X2(t);
      t = AE_MOVF32X2_FROMF16X4(dl0);
      AE_MULAF16SS_11(t, r0, cf0);
      dl1 = AE_MOVF16X4_FROMF32X2(t);
      dl0 = r0;
    }
#endif
    // Correct rounding term for saving the output sample in Q15
    acc = AE_SUB32S(acc, rnd1);
    // rout: Q15 <- Q30 + 1 - 16 w/ saturation
    rout32 = AE_F32X2_SLAIS(acc, 1);
    rout = AE_MOVINT16X4_FROMINT32X2(AE_SRAI32(rout32,16));
    // Store the output sample
    AE_S16_0_IP(rout, castxcc(ae_int16, pout), sz_i16);
  }

  // Save the updated delay elements
  dl0 = AE_SEL16_4321(dl0, dl0);
  dl1 = AE_SEL16_4321(dl1, dl1);
  dl2 = AE_SEL16_4321(dl2, dl2);
  delLine[0] = (ae_int16x4)dl0;
  delLine[1] = (ae_int16x4)dl1;
  delLine[2] = (ae_int16x4)dl2;

} // latr3_proc()



/* Order 4 lattice filter data processing function. */
void latr4_proc( int16_t * r,       const int16_t * x,
                 int16_t * delLine, const int16_t * coef,
                 int16_t scale, int N, int M )
{
  const ae_f16x4 * restrict pin;
        ae_f16x4 * restrict pout;
  ae_int16x4 X, zero;
  ae_f16x4 xin, r2, r1, r0, rout;
  ae_f16x4 dl0, dl1, dl2, dl3, cf0, cf1, cf2, cf3, sc, sccf32;
  ae_f32x2 acc, rout32, rnd0, rnd1;

  int n;

  ASSERT(M == 4);

  zero = AE_ZERO16();
  rnd0 = AE_MOVI(1);
  // rounding term for Q1.30 -> Q15 conversion
  rnd1 = AE_SLAI32(rnd0, 14);
  // rounding term for Q1.30 -> Q1.14 conversion
  rnd0 = AE_SLAI32(rnd0, 15);
  // Set the input and output pointers
  pin  = (const ae_f16x4 *)x;
  pout = (      ae_f16x4 *)r;
  // Load the scale and reflection coefficients, the delay elements
  sc = AE_MOVDA16X2(scale, scale);
  dl0 = (ae_int16x4)delLine[0];
  dl1 = (ae_int16x4)delLine[1];
  dl2 = (ae_int16x4)delLine[2];
  dl3 = (ae_int16x4)delLine[3];
  cf0 = (ae_int16x4)coef[0];
  cf1 = (ae_int16x4)coef[1];
  cf2 = (ae_int16x4)coef[2];
  cf3 = (ae_int16x4)coef[3];
  // Put coefficients to the necessary order in registers:
  // sccf32 : sc  cf3 cf2 0
  //   cf1  :  z   z  cf1 0
  //   cf0  :  z   z  cf0 0
  sccf32 = AE_SEL16_4321(cf2, zero);
  sccf32 = AE_SEL16_4321(cf3, sccf32);
  sccf32 = AE_SEL16_4321(sc , sccf32);
  cf1 = AE_SEL16_6543(cf1, zero);
  cf0 = AE_SEL16_6543(cf0, zero);
  
  //
  // Pass the input samples block through the AR lattice. n-th response sample
  // and lattice state update are defined as follows:
  //
  //   r2 = sc*x[n] - cf3*dl3 - cf2*dl2
  //   r1 = r2 - cf1*dl1;
  //   r0 = r1 - cf0*dl0;
  //
  //   dl3 = dl2 + cf2*r2;
  //   dl2 = dl1 + cf1*r1;
  //   dl1 = dl0 + cf0*r0;
  //   dl0 = r0;
  //
  //   r[n] = r0;
  //
  // The inner loop is fully unrolled.
  //

  __Pragma("loop_count min=1")
  for ( n=0; n<N; n++ )
  {
    // Load the input sample
    AE_L16_IP(X, castxcc(const ae_int16, pin), sz_i16);
    xin = X;
    // Scale the input sample.
    // Q30 <- ( Q15*Q15 + 1 ) - 1 w/ rounding
    acc = AE_MULF16SS_30(sccf32, xin);
    acc = AE_SRAI32R(acc, 1);
    // Add rounding term to the accumulator
    acc = AE_ADD32S(acc, rnd0);
    // Compute output samples of each section
    // Q30 <- Q30 - ( Q14*Q15 + 1 )
    AE_MULSF16SS_21(acc, sccf32, dl3);
    AE_MULSF16SS_11(acc, sccf32, dl2);
    r2 = AE_MOVINT16X4_FROMINT32X2(acc);
    AE_MULSF16SS_11(acc, cf1   , dl1);
    r1 = AE_MOVINT16X4_FROMINT32X2(acc);
    AE_MULSF16SS_11(acc, cf0   , dl0);
    r0 = AE_MOVINT16X4_FROMINT32X2(acc);
    // Update delay elements
    // Use high halves of 32-bit values r2, r1, r0 in multiplications;
    // these values are correctly rounded due to adding the rounding term.
    // dl3 = dl2 + cf2*r2;
    // dl2 = dl1 + cf1*r1;
    // dl1 = dl0 + cf0*r0;
    // dl0 = r0;
    // Q14 <- Q14 + (Q14*Q15 + 1 - 16) w/ rounding and saturation
#if (XCHAL_HAVE_FUSION_16BIT_BASEBAND)
    dl3 = dl2;
    AE_MULAFC16RAS_L(dl3, r2, sccf32);
    dl2 = dl1;
    AE_MULAFC16RAS_L(dl2, r1, cf1);
    dl1 = dl0;
    AE_MULAFC16RAS_L(dl1, r0, cf0);
    dl0 = r0;
#else
    {
      ae_f32x2 t;
      t = AE_MOVF32X2_FROMF16X4(dl2);
      AE_MULAF16SS_11(t, r2, sccf32);
      dl3 = AE_MOVF16X4_FROMF32X2(t);
      t = AE_MOVF32X2_FROMF16X4(dl1);
      AE_MULAF16SS_11(t, r1, cf1);
      dl2 = AE_MOVF16X4_FROMF32X2(t);
      t = AE_MOVF32X2_FROMF16X4(dl0);
      AE_MULAF16SS_11(t, r0, cf0);
      dl1 = AE_MOVF16X4_FROMF32X2(t);
      dl0 = r0;
    }
#endif
    // Correct rounding term for saving the output sample in Q15
    acc = AE_SUB32S(acc, rnd1);
    // rout: Q15 <- Q30 + 1 - 16 w/ saturation
    rout32 = AE_F32X2_SLAIS(acc, 1);
    rout = AE_MOVINT16X4_FROMINT32X2(AE_SRAI32(rout32,16));
    // Store the output sample
    AE_S16_0_IP(rout, castxcc(ae_int16, pout), sz_i16);
  }

  // Save the updated delay elements
  dl0 = AE_SEL16_4321(dl0, dl0);
  dl1 = AE_SEL16_4321(dl1, dl1);
  dl2 = AE_SEL16_4321(dl2, dl2);
  dl3 = AE_SEL16_4321(dl3, dl3);
  delLine[0] = (ae_int16x4)dl0;
  delLine[1] = (ae_int16x4)dl1;
  delLine[2] = (ae_int16x4)dl2;
  delLine[3] = (ae_int16x4)dl3;
} // latr4_proc()



/* Order 5 lattice filter data processing function. */
void latr5_proc( int16_t * r,       const int16_t * x,
                 int16_t * delLine, const int16_t * coef,
                 int16_t scale, int N, int M )
{
  const ae_f16x4 * restrict pin;
        ae_f16x4 * restrict pout;
  ae_int16x4 X, zero;
  ae_f16x4 xin, r3, r2, r1, r0, rout;
  ae_f16x4 dl0, dl1, dl2, dl3, dl4, 
           cf0, cf1, cf2, cf3, cf4,
           sc , sccf43;
  ae_f32x2 acc, rout32, rnd0, rnd1;

  int n;

  ASSERT(M == 5);
  
  zero = AE_ZERO16();
  rnd0 = AE_MOVI(1);
  // rounding term for Q1.30 -> Q15 conversion
  rnd1 = AE_SLAI32(rnd0, 14);
  // rounding term for Q31 -> Q1.14 conversion
  rnd0 = AE_SLAI32(rnd0, 16);
  // Set the input and output pointers
  pin  = (const ae_f16x4 *)x;
  pout = (      ae_f16x4 *)r;
  // Load the scale and reflection coefficients, the delay elements
  sc = AE_MOVDA16X2(scale, scale);
  dl0 = (ae_int16x4)delLine[0];
  dl1 = (ae_int16x4)delLine[1];
  dl2 = (ae_int16x4)delLine[2];
  dl3 = (ae_int16x4)delLine[3];
  dl4 = (ae_int16x4)delLine[4];
  cf0 = (ae_int16x4)coef[0];
  cf1 = (ae_int16x4)coef[1];
  cf2 = (ae_int16x4)coef[2];
  cf3 = (ae_int16x4)coef[3];
  cf4 = (ae_int16x4)coef[4];
  // Put coefficients to the necessary order in registers:
  // sccf43 : sc  cf4 cf3 0
  //   cf2  :  z   z  cf2 0
  //   cf1  :  z   z  cf1 0
  //   cf0  :  z   z  cf0 0
  sccf43 = AE_SEL16_4321(cf3, zero);
  sccf43 = AE_SEL16_4321(cf4, sccf43);
  sccf43 = AE_SEL16_4321(sc , sccf43);// sc cf4 cf3 0
  cf2 = AE_SEL16_6543(cf2, zero);// z z cf2 0
  cf1 = AE_SEL16_6543(cf1, zero);// z z cf1 0
  cf0 = AE_SEL16_6543(cf0, zero);// z z cf0 0
  
  //
  // Pass the input samples block through the AR lattice. n-th response sample
  // and lattice state update are defined as follows:
  //
  //   r3 = sc*x[n] - cf4*dl4 - cf3*dl3
  //   r2 = r3 - cf2*dl2;
  //   r1 = r2 - cf1*dl1;
  //   r0 = r1 - cf0*dl0;
  //
  //   dl4 = dl3 + cf3*r3;
  //   dl3 = dl2 + cf2*r2;
  //   dl2 = dl1 + cf1*r1;
  //   dl1 = dl0 + cf0*r0;
  //   dl0 = r0;
  //
  //   r[n] = r0;
  //
  // The inner loop is fully unrolled.
  //

  __Pragma("loop_count min=1")
  for ( n=0; n<N; n++ )
  {
    // Load the input sample
    AE_L16_IP(X, castxcc(const ae_int16, pin), sz_i16);
    xin = X;
    // Add rounding term to the accumulator
    acc = rnd0;
    // Scale the input sample.
    // Q30 <- [Q31 + ( Q15*Q15 + 1 )] - 1 w/ rounding
    AE_MULAF16SS_30(acc, sccf43, xin);
    acc = AE_SRAI32R(acc, 1);
    // Compute output samples of each section
    // Q30 <- Q30 - ( Q14*Q15 + 1 )
    AE_MULSF16SS_21(acc, sccf43, dl4);
    AE_MULSF16SS_11(acc, sccf43, dl3);
    r3 = AE_MOVINT16X4_FROMINT32X2(acc);
    AE_MULSF16SS_11(acc, cf2   , dl2);
    r2 = AE_MOVINT16X4_FROMINT32X2(acc);
    AE_MULSF16SS_11(acc, cf1   , dl1);
    r1 = AE_MOVINT16X4_FROMINT32X2(acc);
    AE_MULSF16SS_11(acc, cf0   , dl0);
    r0 = AE_MOVINT16X4_FROMINT32X2(acc);
    // Update delay elements
    // Use high halves of 32-bit values r3, r2, r1, r0 in multiplications;
    // these values are correctly rounded due to adding the rounding term.
    // dl4 = dl3 + cf3*r3;
    // dl3 = dl2 + cf2*r2;
    // dl2 = dl1 + cf1*r1;
    // dl1 = dl0 + cf0*r0;
    // dl0 = r0;
    // Q14 <- Q14 + (Q14*Q15 + 1 - 16) w/ rounding and saturation
#if (XCHAL_HAVE_FUSION_16BIT_BASEBAND)
    dl4 = dl3;
    AE_MULAFC16RAS_L(dl4, r3, sccf43);
    dl3 = dl2;
    AE_MULAFC16RAS_L(dl3, r2, cf2);
    dl2 = dl1;
    AE_MULAFC16RAS_L(dl2, r1, cf1);
    dl1 = dl0;
    AE_MULAFC16RAS_L(dl1, r0, cf0);
    dl0 = r0;
#else
    {
      ae_f32x2 t;
      t = AE_MOVF32X2_FROMF16X4(dl3);
      AE_MULAF16SS_11(t, r3, sccf43);
      dl4 = AE_MOVF16X4_FROMF32X2(t);
      t = AE_MOVF32X2_FROMF16X4(dl2);
      AE_MULAF16SS_11(t, r2, cf2);
      dl3 = AE_MOVF16X4_FROMF32X2(t);
      t = AE_MOVF32X2_FROMF16X4(dl1);
      AE_MULAF16SS_11(t, r1, cf1);
      dl2 = AE_MOVF16X4_FROMF32X2(t);
      t = AE_MOVF32X2_FROMF16X4(dl0);
      AE_MULAF16SS_11(t, r0, cf0);
      dl1 = AE_MOVF16X4_FROMF32X2(t);
      dl0 = r0;
    }
#endif
    // Correct rounding term for saving the output sample in Q15
    acc = AE_SUB32S(acc, rnd1);
    // rout: Q15 <- Q30 + 1 - 16 w/ saturation
    rout32 = AE_F32X2_SLAIS(acc, 1);
    rout = AE_MOVINT16X4_FROMINT32X2(AE_SRAI32(rout32,16));
    // Store the output sample
    AE_S16_0_IP(rout, castxcc(ae_int16, pout), sz_i16);
  }

  // Save the updated delay elements
  dl0 = AE_SEL16_4321(dl0, dl0);
  dl1 = AE_SEL16_4321(dl1, dl1);
  dl2 = AE_SEL16_4321(dl2, dl2);
  dl3 = AE_SEL16_4321(dl3, dl3);
  dl4 = AE_SEL16_4321(dl4, dl4);
  delLine[0] = (ae_int16x4)dl0;
  delLine[1] = (ae_int16x4)dl1;
  delLine[2] = (ae_int16x4)dl2;
  delLine[3] = (ae_int16x4)dl3;
  delLine[4] = (ae_int16x4)dl4;
} // latr5_proc()



/* Order 6 lattice filter data processing function. */
void latr6_proc( int16_t * r,       const int16_t * x,
                 int16_t * delLine, const int16_t * coef,
                 int16_t scale, int N, int M )
{
#if (XCHAL_HAVE_FUSION_16BIT_BASEBAND)
  const ae_f16x4 * restrict pin;
        ae_f16x4 * restrict pout;
  ae_int16x4 X, zero;
  ae_f16x4 xin, r43, r21, r0out;
  ae_f16x4 dl0, dl1, dl2, dl3, dl4, dl5,
           dl54, dl32, dl10,
           cf0, cf1, cf2, cf3, cf4, cf5,
           sccf5, cf43, cf21,
           sc;
  ae_f32x2 ACC, R4, R3, R2, R1, R0, Rout;

  int n;

  ASSERT(M == 6);

  zero = AE_ZERO16();
  // Set the input and output pointers
  pin  = (const ae_f16x4 *)x;
  pout = (      ae_f16x4 *)r;
  // Load the scale and reflection coefficients, the delay elements
  sc = AE_MOVDA16X2(scale, scale);
  dl0 = (ae_int16x4)delLine[0];
  dl1 = (ae_int16x4)delLine[1];
  dl2 = (ae_int16x4)delLine[2];
  dl3 = (ae_int16x4)delLine[3];
  dl4 = (ae_int16x4)delLine[4];
  dl5 = (ae_int16x4)delLine[5];
  cf0 = (ae_int16x4)coef[0];
  cf1 = (ae_int16x4)coef[1];
  cf2 = (ae_int16x4)coef[2];
  cf3 = (ae_int16x4)coef[3];
  cf4 = (ae_int16x4)coef[4];
  cf5 = (ae_int16x4)coef[5];
  // Put coefficients to the necessary order in registers:
  // dl54 : z  dl5 z  dl4
  // dl32 : z  dl3 z  dl2
  // dl10 : z  dl1 z  dl0
  // sccf5 : sc  sc  cf5 cf5
  // cf43  : cf4  0  cf3  0
  // cf21  : cf2  0  cf1  0
  // cf0   :  z   z  cf0  0
  dl54 = AE_SEL16_5432(dl5, dl4);
  dl32 = AE_SEL16_5432(dl3, dl2);
  dl10 = AE_SEL16_5432(dl1, dl0);
  sccf5 = AE_SEL16_5432(sc, cf5);
  cf43 = AE_SEL16_4321(cf4, cf3);
  cf43 = AE_SEL16_7362(cf43, zero);
  cf21 = AE_SEL16_4321(cf2, cf1);
  cf21 = AE_SEL16_7362(cf21, zero);
  cf0  = AE_SEL16_4321(cf0, zero);
  
  //
  // Pass the input samples block through the AR lattice. n-th response sample
  // and lattice state update are defined as follows:
  //
  //   r4 = sc*x[n] - cf5*dl5 - cf4*dl4
  //   r3 = r4 - cf3*dl3;
  //   r2 = r3 - cf2*dl2;
  //   r1 = r2 - cf1*dl1;
  //   r0 = r1 - cf0*dl0;
  //
  //   dl5 = dl4 + cf4*r4;
  //   dl4 = dl3 + cf3*r3;
  //   dl3 = dl2 + cf2*r2;
  //   dl2 = dl1 + cf1*r1;
  //   dl1 = dl0 + cf0*r0;
  //   dl0 = r0;
  //
  //   r[n] = r0;
  //
  // The inner loop is fully unrolled.
  //

  __Pragma("loop_count min=1")
  for ( n=0; n<N; n++ )
  {
    // Load the input sample
    AE_L16_IP(X, castxcc(const ae_int16, pin), sz_i16);
    xin = X;
    // Scale the input sample.
    // Q30 <- ( Q15*Q15 + 1 ) - 1 w/ rounding
    ACC = AE_MULF16SS_30(sccf5, xin);
    ACC = AE_SRAI32R(ACC, 1);
    // Compute output samples of each section
    // Q30 <- Q30 - ( Q14*Q15 + 1 )
    AE_MULSF16SS_21(ACC, dl54, sccf5);
    AE_MULSF16SS_30(ACC, cf43, dl54);
    R4 = ACC;
    AE_MULSF16SS_21(ACC, dl32, cf43);
    R3 = ACC;
    AE_MULSF16SS_30(ACC, cf21, dl32);
    R2 = ACC;
    AE_MULSF16SS_21(ACC, dl10, cf21);
    R1 = ACC;
    AE_MULSF16SS_30(ACC,  cf0, dl10);
    R0 = ACC;
    Rout = AE_F32X2_SLAIS(ACC, 1);
    // Get output samples of each section
    // Q14 <- Q30 - 16 w/ rounding and saturation
    r43 = AE_ROUND16X4F32SASYM(R4, R3);
    r21 = AE_ROUND16X4F32SASYM(R2, R1);
    r0out = AE_ROUND16X4F32SASYM(R0, Rout);
    // Update delay elements
    // dl5 = dl4 + cf4*r4;
    // dl4 = dl3 + cf3*r3;
    // dl3 = dl2 + cf2*r2;
    // dl2 = dl1 + cf1*r1;
    // dl1 = dl0 + cf0*r0;
    // dl0 = r0;
    // Q14 <- Q14 + (Q14*Q15 + 1 - 16) w/ rounding and saturation
    dl54 = AE_SEL16_5432(dl54, dl32);
    dl32 = AE_SEL16_5432(dl32, dl10);
    dl10 = AE_SEL16_5432(dl10, r0out);
    AE_MULAFC16RAS_L(dl54, r43, cf43);
    AE_MULAFC16RAS_H(dl54, r43, cf43);
    AE_MULAFC16RAS_L(dl32, r21, cf21);
    AE_MULAFC16RAS_H(dl32, r21, cf21);
    AE_MULAFC16RAS_H(dl10, r0out, cf0);
    // Store the output sample
    AE_S16_0_IP(r0out, castxcc(ae_int16, pout), sz_i16);
  }

  // Save the updated delay elements
  dl0 = dl10;
  dl1 = AE_SEL16_5432(dl10, dl10);
  dl2 = dl32;
  dl3 = AE_SEL16_5432(dl32, dl32);
  dl4 = dl54;
  dl5 = AE_SEL16_5432(dl54, dl54);
  delLine[0] = (ae_int16x4)dl0;
  delLine[1] = (ae_int16x4)dl1;
  delLine[2] = (ae_int16x4)dl2;
  delLine[3] = (ae_int16x4)dl3;
  delLine[4] = (ae_int16x4)dl4;
  delLine[5] = (ae_int16x4)dl5;
#else
        ae_int16   * restrict R;
  const ae_int16   * restrict X;
        ae_int16x4 *          D;

  ae_int16x4 dl_5432, dl_10zz,
             cf0, cf1, cf2, cf3, cf4, cf5;
  ae_f32x2   dl_54, dl_32, dl_10;
  ae_f32x2   dl_43, dl_21;
  ae_f16x4   cf_5432, cf_10zz;
  ae_f16x4   cf_4321;
  ae_int16x4 s0, s1, s2;
  ae_int16x4 t0, t1;
  ae_f32x2   p0, p1, s_g;
  ae_f64     q0, q1;
  ae_int64   u0;

  int n;

  ASSERT( r && x && delLine && coef );

  ASSERT( IS_ALIGN( delLine ) && IS_ALIGN( coef ) );

  ASSERT( M == 6 );

  //
  // Setup pointers.
  //

  R = (      ae_int16   *)r;
  X = (const ae_int16   *)x;
  D = (      ae_int16x4 *)delLine;

  //
  // Load the prescaling factor.
  //

  // Q31 <- Q15 + 16
  s_g = AE_MOVDA32( ((int32_t)scale)<<16 );

  //
  // Pre-load the reflection coefficients and delay line elements.
  //
  

  // Q15
  cf0 = (ae_int16x4)coef[0];
  cf1 = (ae_int16x4)coef[1];
  cf2 = (ae_int16x4)coef[2];
  cf3 = (ae_int16x4)coef[3];
  cf4 = (ae_int16x4)coef[4];
  cf5 = (ae_int16x4)coef[5];
  t0 = AE_SEL16_4321( cf5, cf4 );
  t1 = AE_SEL16_4321( cf3, cf2 );
  s0 = AE_SEL16_7632( t0, t1 ); // k5 k4 k3 k2
  t0 = AE_SEL16_4321( cf1, cf0 );
  s1 = AE_SEL16_7632( t0, t0 ); // k1 k0  0  0

  // Make a skew coefficient quadruple.
  s2 = AE_SEL16_6543( s0, s1 ); // k4 k3 k2 k1

  cf_5432 = ( s0 );
  cf_10zz = ( s1 );
  cf_4321 = ( s2 );

  // Load delay line elements d5..d0.
  // Q1.30 <- Q1.14 + 16
  dl_5432 = AE_L16X4_I( D, +0 ); // d5 d4 d3 d2
  dl_10zz = AE_L16X4_I( D, +8 ); // d1 d0  0  0
  dl_54 = AE_CVT32X2F16_32(dl_5432);
  dl_32 = AE_CVT32X2F16_10(dl_5432);
  dl_10 = AE_CVT32X2F16_32(dl_10zz);

  //
  // Pass the input samples block through the AR lattice. n-th response sample
  // and lattice state update are defined as follows:
  //
  //   p4 = g*x[n] - k5*d5 - k4*d4
  //   p3 = p4 - k3*d3; p2 = p3 - k2*d2;
  //   p1 = p2 - k1*d1; p0 = p1 - k0*d0;
  //
  //   d5 = d4 + k4*p4; d4 = d3 + k3*p3; d3 = d2 + k2*p2;
  //   d2 = d1 + k1*p1; d1 = d0 + k0*p0; d0 = p0;
  //
  //   r[n] = p0;
  //
  // The inner loop is fully unrolled.
  //

  for ( n=0; n<N; n++ )
  {
    //--------------------------------------------------------------------------
    // Load the next input sample and prescale it.

    // Q15
    AE_L16_IP( t0, X, +2 );

    // q0 = g*x[n]
    // Q17.46 <- Q31*Q15
    u0 = AE_MUL32X16_L0( s_g, t0 );

    q0 = ( u0 );

    //--------------------------------------------------------------------------
    // Update delay elements d5, d4.

    // q0 -= k5*d5 + k4*d4
    // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ] + [ Q15*Q1.30 + 1 ]
    AE_MULSSFD32X16_H3_L2( q0, dl_54, cf_5432 );

    q1 = q0;

    // q1 -= k3*d3
    // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ]
    AE_MULSF32X16_H1( q1, dl_32, cf_5432 );

    // p4 = q0; p3 = q1
    // Q1.30 <- Q17.46 - 16 w/ rounding and saturation
    AE_PKSR32( p0, q0, 0 );
    AE_PKSR32( p0, q1, 0 );

    // d4 d3
    select_f32x2_lh( dl_43, dl_54, dl_32 );

    // d5 = d4 + k4*p4; d4 = d3 + k3*p3
    // Q1.30 <- Q1.30 + [ Q15*Q1.30 - 15 ] w/ rounding and saturation
    AE_MULAFP32X16X2RAS_H( dl_43, p0, cf_4321 );

    dl_54 = dl_43;

    //--------------------------------------------------------------------------
    // Update delay elements d3, d2.

    // d2 d1
    select_f32x2_lh( dl_21, dl_32, dl_10 );

    // q0 -= k3*d3 + k2*d2; q1 -= k2*d2 + k1*d1
    // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ] + [ Q15*Q1.30 + 1 ]
    AE_MULSSFD32X16_H1_L0( q0, dl_32, cf_5432 );
    AE_MULSSFD32X16_H1_L0( q1, dl_21, cf_4321 );

    // p2 = q0; p1 = q1
    // Q1.30 <- Q17.46 - 16 w/ rounding and saturation
    AE_PKSR32( p0, q0, 0 );
    AE_PKSR32( p0, q1, 0 );

    // d3 = d2 + k2*p2; d2 = d1 + k1*p1
    // Q1.30 <- Q1.30 + [ Q15*Q1.30 - 15 ] w/ rounding and saturation
    AE_MULAFP32X16X2RAS_L( dl_21, p0, cf_4321 );

    dl_32 = dl_21;

    //--------------------------------------------------------------------------
    // Update delay elements d1, d0.

    // q0 -= k1*d1 + k0*d0 
    // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ] + [ Q15*Q1.30 + 1 ]
    AE_MULSSFD32X16_H3_L2( q0, dl_10, cf_10zz );

    // p0 = q0
    // Q1.30 <- Q17.46 - 16 w/ rounding and saturation
    AE_PKSR32( p0, q0, 0 );

    // d1 = d0 + k0*p0
    // Q1.30 <- Q1.30 + [ Q15*Q1.30 - 15 ] w/ rounding and saturation
    AE_MULAFP32X16X2RAS_H( dl_10, p0, cf_10zz );

    // d1; d0 = p0
    select_f32x2_ll( dl_10, dl_10, p0 );

    //--------------------------------------------------------------------------
    // Format and store response sample.

    // r[n] = q0
    // Q31 <- Q17.46 + 1 - 16 w/ rounding and saturation
    p1 = AE_MOVF32X2_FROMF64(AE_PKSR32_0( q0, 1 ));

    // Q15 <- Q31 - 16 w/ asym. rounding and saturation
    t1 = AE_ROUND16X4F32SASYM( p1, p1 );
    AE_S16_0_IP( t1, R, +2 );
  }

  //
  // Store updated delay line elements.
  //

  // Q1.14 <- Q1.30 - 16 w/ asym. rounding and saturation
  dl_5432 = AE_ROUND16X4F32SASYM(dl_54, dl_32);
  dl_10zz = AE_ROUND16X4F32SASYM(dl_10, dl_10);
  AE_S16X4_I(dl_5432, D, 0);
  t0 = AE_SEL16_4321(dl_10zz, dl_10zz);
  AE_S16_0_I( t0,      (ae_int16 *)D, +8 );
  AE_S16_0_I( dl_10zz, (ae_int16 *)D, +10 );
#endif
} // latr6_proc()



/* Order 7 lattice filter data processing function. */
void latr7_proc( int16_t * r,       const int16_t * x,
                 int16_t * delLine, const int16_t * coef,
                 int16_t scale, int N, int M )
{
#if (XCHAL_HAVE_FUSION_16BIT_BASEBAND)
  const ae_f16x4 * restrict pin;
        ae_f16x4 * restrict pout;
  ae_int16x4 X, zero;
  ae_f16x4 xin, r54, r32, r10, rout;
  ae_f16x4 dl0, dl1, dl2, dl3, dl4, dl5, dl6, dl65, dl43, dl21,
           cf0, cf1, cf2, cf3, cf4, cf5, cf6,
           sccf6, cf54, cf32, cf10,
           sc;
  ae_f32x2 acc0, R5, R4, R3, R2, R1, R0, Rout;

  int n;

  ASSERT(M == 7);

  zero = AE_ZERO16();
  // Set the input and output pointers
  pin  = (const ae_f16x4 *)x;
  pout = (      ae_f16x4 *)r;
  // Load the scale and reflection coefficients, the delay elements
  sc = AE_MOVDA16X2(scale, scale);
  dl0 = (ae_int16x4)delLine[0];
  dl1 = (ae_int16x4)delLine[1];
  dl2 = (ae_int16x4)delLine[2];
  dl3 = (ae_int16x4)delLine[3];
  dl4 = (ae_int16x4)delLine[4];
  dl5 = (ae_int16x4)delLine[5];
  dl6 = (ae_int16x4)delLine[6];
  cf0 = (ae_int16x4)coef[0];
  cf1 = (ae_int16x4)coef[1];
  cf2 = (ae_int16x4)coef[2];
  cf3 = (ae_int16x4)coef[3];
  cf4 = (ae_int16x4)coef[4];
  cf5 = (ae_int16x4)coef[5];
  cf6 = (ae_int16x4)coef[6];
  // Put coefficients to the necessary order in registers:
  // dl65 : z  dl6 z  dl5
  // dl43 : z  dl4 z  dl3
  // dl21 : z  dl2 z  dl1
  // sccf6 : sc  sc  cf6 cf6
  // cf54  : cf5  0  cf4  0
  // cf32  : cf3  0  cf2  0
  // cf10  : cf1  0  cf0  0
  dl65 = AE_SEL16_5432(dl6, dl5);
  dl43 = AE_SEL16_5432(dl4, dl3);
  dl21 = AE_SEL16_5432(dl2, dl1);
  sccf6 = AE_SEL16_5432(sc, cf6);
  cf54 = AE_SEL16_4321(cf5, cf4);
  cf54 = AE_SEL16_7362(cf54, zero);
  cf32 = AE_SEL16_4321(cf3, cf2);
  cf32 = AE_SEL16_7362(cf32, zero);
  cf10 = AE_SEL16_4321(cf1, cf0);
  cf10 = AE_SEL16_7362(cf10, zero);
  
  //
  // Pass the input samples block through the AR lattice. n-th response sample
  // and lattice state update are defined as follows:
  //
  //   r5 = sc*x[n] - cf6*dl6 - cf5*dl5
  //   r4 = r5 - cf4*dl4;
  //   r3 = r4 - cf3*dl3;
  //   r2 = r3 - cf2*dl2;
  //   r1 = r2 - cf1*dl1;
  //   r0 = r1 - cf0*dl0;
  //
  //   dl6 = dl5 + cf5*r5;
  //   dl5 = dl4 + cf4*r4;
  //   dl4 = dl3 + cf3*r3;
  //   dl3 = dl2 + cf2*r2;
  //   dl2 = dl1 + cf1*r1;
  //   dl1 = dl0 + cf0*r0;
  //   dl0 = r0;
  //
  //   r[n] = r0;
  //
  // The inner loop is fully unrolled.
  //

  __Pragma("loop_count min=1")
  for ( n=0; n<N; n++ )
  {
    // Load the input sample
    AE_L16_IP(X, castxcc(const ae_int16, pin), sz_i16);
    xin = X;
    // Scale the input sample.
    // Q30 <- ( Q15*Q15 + 1 ) - 1 w/ rounding
    acc0 = AE_MULF16SS_30(sccf6, xin);
    acc0 = AE_SRAI32R(acc0, 1);
    // Compute output samples of each section
    // Q30 <- Q30 - ( Q14*Q15 + 1 )
    AE_MULSF16SS_21(acc0, dl65, sccf6);
    AE_MULSF16SS_30(acc0, cf54, dl65);
    R5 = acc0;
    AE_MULSF16SS_21(acc0, dl43, cf54);
    R4 = acc0;
    AE_MULSF16SS_30(acc0, cf32, dl43);
    R3 = acc0;
    AE_MULSF16SS_21(acc0, dl21, cf32);
    R2 = acc0;
    AE_MULSF16SS_30(acc0, cf10, dl21);
    R1 = acc0;
    AE_MULSF16SS_10(acc0, cf10, dl0);
    R0 = acc0;
    // Get output samples of each section
    // Q14 <- Q30 - 16 w/ rounding and saturation
    r54 = AE_ROUND16X4F32SASYM(R5, R4);
    r32 = AE_ROUND16X4F32SASYM(R3, R2);
    r10 = AE_ROUND16X4F32SASYM(R1, R0);
    // Update delay elements
    // dl6 = dl5 + cf5*r5;
    // dl5 = dl4 + cf4*r4;
    // dl4 = dl3 + cf3*r3;
    // dl3 = dl2 + cf2*r2;
    // dl2 = dl1 + cf1*r1;
    // dl1 = dl0 + cf0*r0;
    // dl0 = r0;
    // Q14 <- Q14 + (Q14*Q15 + 1 - 16) w/ rounding and saturation
    dl65 = AE_SEL16_5432(dl65, dl43);
    dl43 = AE_SEL16_5432(dl43, dl21);
    dl21 = AE_SEL16_5410(dl21, dl0 );
    AE_MULAFC16RAS_L(dl65, r54, cf54);
    AE_MULAFC16RAS_H(dl65, r54, cf54);
    AE_MULAFC16RAS_L(dl43, r32, cf32);
    AE_MULAFC16RAS_H(dl43, r32, cf32);
    AE_MULAFC16RAS_L(dl21, r10, cf10);
    AE_MULAFC16RAS_H(dl21, r10, cf10);
    dl0 = r10;
    // Format and store the output sample
    // Q15 <- (Q30 + 1) - 16 w/ rounding and saturation
    Rout = AE_F32X2_SLAIS(acc0, 1);
    rout = AE_ROUND16X4F32SASYM(Rout, Rout);
    AE_S16_0_IP(rout, castxcc(ae_int16, pout), sz_i16);
  }

  // Save the updated delay elements
  dl1 = dl21;
  dl2 = AE_SEL16_5432(dl21, dl21);
  dl3 = dl43;
  dl4 = AE_SEL16_5432(dl43, dl43);
  dl5 = dl65;
  dl6 = AE_SEL16_5432(dl65, dl65);
  delLine[0] = (ae_int16x4)dl0;
  delLine[1] = (ae_int16x4)dl1;
  delLine[2] = (ae_int16x4)dl2;
  delLine[3] = (ae_int16x4)dl3;
  delLine[4] = (ae_int16x4)dl4;
  delLine[5] = (ae_int16x4)dl5;
  delLine[6] = (ae_int16x4)dl6;
#else
        ae_int16   * restrict R;
  const ae_int16   * restrict X;
        ae_int16x4 *          D;

  ae_int16x4 dl_6543, dl_210z,
             cf0, cf1, cf2, cf3, cf4, cf5, cf6;
  ae_f32x2   dl_65, dl_43, dl_21, dl_0z;
  ae_f32x2   dl_54, dl_32, dl_10;
  ae_f16x4   cf_6543, cf_210z;
  ae_f16x4   cf_5432, cf_10z6;
  ae_int16x4 s0, s1, s2, s3;
  ae_int16x4 t0, t1;
  ae_f32x2   p0, p1, s_g;
  ae_f64     q0, q1;
  ae_int64   u0;

  int n;

  ASSERT( r && x && delLine && coef );

  ASSERT( IS_ALIGN( delLine ) && IS_ALIGN( coef ) );

  ASSERT( M == 7 );

  //
  // Setup pointers.
  //

  R = (      ae_int16   *)r;
  X = (const ae_int16   *)x;
  D = (      ae_int16x4 *)delLine;

  //
  // Load the prescaling factor.
  //
  
  // Q31 <- Q15 + 16
  s_g = AE_MOVDA32( ((int32_t)scale)<<16 );

  //
  // Pre-load the reflection coefficients and delay line elements.
  //

  // Q15
  cf0 = (ae_int16x4)coef[0];
  cf1 = (ae_int16x4)coef[1];
  cf2 = (ae_int16x4)coef[2];
  cf3 = (ae_int16x4)coef[3];
  cf4 = (ae_int16x4)coef[4];
  cf5 = (ae_int16x4)coef[5];
  cf6 = (ae_int16x4)coef[6];
  t0 = AE_SEL16_4321( cf6, cf5 );
  t1 = AE_SEL16_4321( cf4, cf3 );
  s0 = AE_SEL16_7632( t0, t1 ); // k6 k5 k4 k3
  t0 = AE_SEL16_4321( cf2, cf1 );
  t1 = AE_SEL16_4321( cf0, cf0 );
  s1 = AE_SEL16_7632( t0, t1 ); // k2 k1 k0  0

  // Make skew coefficient quadruples.
  s2 = AE_SEL16_6543( s0, s1 ); // k5 k4 k3 k2
  s3 = AE_SEL16_6543( s1, s0 ); // k1 k0  0 k6

  cf_6543 = ( s0 );
  cf_210z = ( s1 );
  cf_5432 = ( s2 );
  cf_10z6 = ( s3 );

  // Load delay line elements d6..d0.
  // Q1.30 <- Q1.14 + 16
  dl_6543 = AE_L16X4_I( D, +0 ); // d6 d5 d4 d3
  dl_210z = AE_L16X4_I( D, +8 ); // d2 d1 d0  0
  dl_65 = AE_CVT32X2F16_32(dl_6543);
  dl_43 = AE_CVT32X2F16_10(dl_6543);
  dl_21 = AE_CVT32X2F16_32(dl_210z);
  dl_0z = AE_CVT32X2F16_10(dl_210z);

  //
  // Pass the input samples block through the AR lattice. n-th response sample
  // and lattice state update are defined as follows:
  //
  //   p5 = g*x[n] - k6*d6 - k5*d5
  //   p4 = p5 - k4*d4; p3 = p4 - k3*d3; p2 = p3 - k2*d2;
  //   p1 = p2 - k1*d1; p0 = p1 - k0*d0;
  //
  //   d6 = d5 + k5*p5; d5 = d4 + k4*p4; d4 = d3 + k3*p3;
  //   d3 = d2 + k2*p2; d2 = d1 + k1*p1; d1 = d0 + k0*p0;
  //   d0 = p0;
  //
  //   r[n] = p0;
  //
  // The inner loop is fully unrolled.
  //

  for ( n=0; n<N; n++ )
  {
    //--------------------------------------------------------------------------
    // Load the next input sample and prescale it.
      
    // Q15
    AE_L16_IP( t0, X, +2 );

    // q0 = g*x[n]
    // Q17.46 <- Q15*Q31
    u0 = AE_MUL32X16_L0( s_g, t0 );

    q0 = ( u0 );

    //--------------------------------------------------------------------------
    // Update delay elements d6, d5.

    // d5 d4
    select_f32x2_lh( dl_54, dl_65, dl_43 );

    // q0 -= k6*d6 + k5*d5
    // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ] + [ Q15*Q1.30 + 1 ]
    AE_MULSSFD32X16_H3_L2( q0, dl_65, cf_6543 );

    q1 = q0;

    // q1 -= k4*d4
    // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ]
    AE_MULSF32X16_L1( q1, dl_54, cf_6543 );

    // p5 = q0; p4 = q1
    // Q1.30 <- Q17.46 - 16 w/ rounding and saturation
    AE_PKSR32( p0, q0, 0 );
    AE_PKSR32( p0, q1, 0 );

    // d6 = d5 + k5*p5; d5 = d4 + k4*p4
    // Q1.30 <- Q1.30 + [ Q15*Q1.30 - 15 ] w/ rounding and saturation
    AE_MULAFP32X16X2RAS_H( dl_54, p0, cf_5432 );

    dl_65 = dl_54;

    //--------------------------------------------------------------------------
    // Update delay elements d4, d3.

    // d3 d2
    select_f32x2_lh( dl_32, dl_43, dl_21 );

    // q0 -= k4*d4 + k3*d3; q1 -= k3*d3 + k2*d2
    // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ] + [ Q15*Q1.30 + 1 ]
    AE_MULSSFD32X16_H1_L0( q0, dl_43, cf_6543 );
    AE_MULSSFD32X16_H1_L0( q1, dl_32, cf_5432 );

    // p3 = q0; p2 = q1
    // Q1.30 <- Q17.46 - 16 w/ rounding and saturation
    AE_PKSR32( p0, q0, 0 );
    AE_PKSR32( p0, q1, 0 );

    // d4 = d3 + k3*p3; d3 = d2 + k2*p2
    // Q1.30 <- Q1.30 + [ Q15*Q1.30 - 15 ] w/ rounding and saturation
    AE_MULAFP32X16X2RAS_L( dl_32, p0, cf_5432 );

    dl_43 = dl_32;

    //--------------------------------------------------------------------------
    // Update delay elements d2, d1.

    // d1 d0
    select_f32x2_lh( dl_10, dl_21, dl_0z );

    // q0 -= k2*d2 + k1*d1; q1 -= k1*d1 + k0*d0
    // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ] + [ Q15*Q1.30 + 1 ]
    AE_MULSSFD32X16_H3_L2( q0, dl_21, cf_210z );
    AE_MULSSFD32X16_H3_L2( q1, dl_10, cf_10z6 );

    // p1 = q0; p0 = q1
    // Q1.30 <- Q17.46 - 16 w/ rounding and saturation
    AE_PKSR32( p0, q0, 0 );
    AE_PKSR32( p0, q1, 0 );

    // d2 = d1 + k1*p1; d1 = d0 + k0*p0
    // Q1.30 <- Q1.30 + [ Q15*Q1.30 - 15 ] w/ rounding and saturation
    AE_MULAFP32X16X2RAS_H( dl_10, p0, cf_10z6 );

    dl_21 = dl_10;

    //--------------------------------------------------------------------------
    // Update delay element d0, format and store response sample.

    // d0 = p0; 0
    select_f32x2_ll( dl_0z, p0, dl_0z );

    // r[n] = q1
    // Q31 <- Q17.46 + 1 - 16 w/ rounding and saturation
    p1 = AE_MOVF32X2_FROMF64(AE_PKSR32_0( q1, 1 ));
    
    // Q15 <- Q31 - 16 w/ asym. rounding and saturation
    t1 = AE_ROUND16X4F32SASYM( p1, p1 );
    AE_S16_0_IP( t1, R, +2 );
  }

  //
  // Store updated delay line elements.
  //
  
  // Q1.14 <- Q1.30 - 16 w/ asym. rounding and saturation
  dl_6543 = AE_ROUND16X4F32SASYM( dl_65, dl_43 );
  dl_210z = AE_ROUND16X4F32SASYM( dl_21, dl_0z );
  AE_S16X4_I( dl_6543, D, 0 );
  t0 = AE_SEL16_6543(dl_210z, dl_210z);
  t1 = AE_SEL16_5432(dl_210z, dl_210z);
  AE_S16_0_I( t0, (ae_int16 *)D, +8  );
  AE_S16_0_I( t1, (ae_int16 *)D, +10 );
  t0 = AE_SEL16_4321(dl_210z, dl_210z);
  AE_S16_0_I( t0, (ae_int16 *)D, +12 );
#endif
} // latr7_proc()



/* Order 8 lattice filter data processing function. */
void latr8_proc( int16_t * r,       const int16_t * x,
                 int16_t * delLine, const int16_t * coef,
                 int16_t scale, int N, int M )
{
#if (XCHAL_HAVE_FUSION_16BIT_BASEBAND)
  const ae_f16x4 * restrict pin;
        ae_f16x4 * restrict pout;
  ae_int16x4 X, zero;
  ae_f16x4 xin, r65, r43, r21, r0out;
  ae_f16x4 dl0, dl1, dl2, dl3, dl4, dl5, dl6, dl7,
           dl76, dl54, dl32, dl10,
           cf0, cf1, cf2, cf3, cf4, cf5, cf6, cf7,
           sccf7, cf65, cf43, cf21,
           sc;
  ae_f32x2 acc0, R6, R5, R4, R3, R2, R1, R0, Rout;

  int n;

  ASSERT(M == 8);

  zero = AE_ZERO16();
  // Set the input and output pointers
  pin  = (const ae_f16x4 *)x;
  pout = (      ae_f16x4 *)r;
  // Load the scale and reflection coefficients, the delay elements
  sc = AE_MOVDA16X2(scale, scale);
  dl0 = (ae_int16x4)delLine[0];
  dl1 = (ae_int16x4)delLine[1];
  dl2 = (ae_int16x4)delLine[2];
  dl3 = (ae_int16x4)delLine[3];
  dl4 = (ae_int16x4)delLine[4];
  dl5 = (ae_int16x4)delLine[5];
  dl6 = (ae_int16x4)delLine[6];
  dl7 = (ae_int16x4)delLine[7];
  cf0 = (ae_int16x4)coef[0];
  cf1 = (ae_int16x4)coef[1];
  cf2 = (ae_int16x4)coef[2];
  cf3 = (ae_int16x4)coef[3];
  cf4 = (ae_int16x4)coef[4];
  cf5 = (ae_int16x4)coef[5];
  cf6 = (ae_int16x4)coef[6];
  cf7 = (ae_int16x4)coef[7];
  // Put coefficients to the necessary order in registers:
  // dl76 : z  dl7 z  dl6
  // dl54 : z  dl5 z  dl4
  // dl32 : z  dl3 z  dl2
  // dl10 : z  dl1 z  dl0
  // sccf7 : sc  sc  cf7 cf7
  // cf65  : cf6  0  cf5  0
  // cf43  : cf4  0  cf3  0
  // cf21  : cf2  0  cf1  0
  // cf0   :  z   z  cf0  0
  dl76 = AE_SEL16_5432(dl7, dl6);
  dl54 = AE_SEL16_5432(dl5, dl4);
  dl32 = AE_SEL16_5432(dl3, dl2);
  dl10 = AE_SEL16_5432(dl1, dl0);
  sccf7 = AE_SEL16_5432(sc, cf7);
  cf65 = AE_SEL16_4321(cf6, cf5);
  cf65 = AE_SEL16_7362(cf65, zero);
  cf43 = AE_SEL16_4321(cf4, cf3);
  cf43 = AE_SEL16_7362(cf43, zero);
  cf21 = AE_SEL16_4321(cf2, cf1);
  cf21 = AE_SEL16_7362(cf21, zero);
  cf0  = AE_SEL16_4321(cf0, zero);
  
  //
  // Pass the input samples block through the AR lattice. n-th response sample
  // and lattice state update are defined as follows:
  //
  //   r6 = sc*x[n] - cf7*dl7 - cf6*dl6
  //   r5 = r6 - cf5*dl5;
  //   r4 = r5 - cf4*dl4;
  //   r3 = r4 - cf3*dl3;
  //   r2 = r3 - cf2*dl2;
  //   r1 = r2 - cf1*dl1;
  //   r0 = r1 - cf0*dl0;
  //
  //   dl7 = dl6 + cf6*r6;
  //   dl6 = dl5 + cf5*r5;
  //   dl5 = dl4 + cf4*r4;
  //   dl4 = dl3 + cf3*r3;
  //   dl3 = dl2 + cf2*r2;
  //   dl2 = dl1 + cf1*r1;
  //   dl1 = dl0 + cf0*r0;
  //   dl0 = r0;
  //
  //   r[n] = r0;
  //
  // The inner loop is fully unrolled.
  //

  __Pragma("loop_count min=1")
  for ( n=0; n<N; n++ )
  {
    // Load the input sample
    AE_L16_IP(X, castxcc(const ae_int16, pin), sz_i16);
    xin = X;
    // Scale the input sample.
    // Q30 <- ( Q15*Q15 + 1 ) - 1 w/ rounding
    acc0 = AE_MULF16SS_30(sccf7, xin);
    acc0 = AE_SRAI32R(acc0, 1);
    // Compute output samples of each section
    // Q30 <- Q30 - ( Q14*Q15 + 1 )
    AE_MULSF16SS_21(acc0, dl76, sccf7);
    AE_MULSF16SS_30(acc0, cf65, dl76);
    R6 = acc0;
    AE_MULSF16SS_21(acc0, dl54, cf65);
    R5 = acc0;
    AE_MULSF16SS_30(acc0, cf43, dl54);
    R4 = acc0;
    AE_MULSF16SS_21(acc0, dl32, cf43);
    R3 = acc0;
    AE_MULSF16SS_30(acc0, cf21, dl32);
    R2 = acc0;
    AE_MULSF16SS_21(acc0, dl10, cf21);
    R1 = acc0;
    AE_MULSF16SS_30(acc0,  cf0, dl10);
    R0 = acc0;
    // Get output samples of each section
    // Rout : Q15 <- (Q30 + 1) - 16 w/ rounding and saturation
    Rout = AE_F32X2_SLAIS(acc0, 1);
    // Q14 <- Q30 - 16 w/ rounding and saturation
    r65 = AE_ROUND16X4F32SASYM(R6, R5);
    r43 = AE_ROUND16X4F32SASYM(R4, R3);
    r21 = AE_ROUND16X4F32SASYM(R2, R1);
    r0out = AE_ROUND16X4F32SASYM(R0, Rout);
    // Update delay elements
    // dl7 = dl6 + cf6*r6;
    // dl6 = dl5 + cf5*r5;
    // dl5 = dl4 + cf4*r4;
    // dl4 = dl3 + cf3*r3;
    // dl3 = dl2 + cf2*r2;
    // dl2 = dl1 + cf1*r1;
    // dl1 = dl0 + cf0*r0;
    // dl0 = r0;
    // Q14 <- Q14 + (Q14*Q15 + 1 - 16) w/ rounding and saturation
    dl76 = AE_SEL16_5432(dl76, dl54);
    dl54 = AE_SEL16_5432(dl54, dl32);
    dl32 = AE_SEL16_5432(dl32, dl10);
    dl10 = AE_SEL16_5432(dl10, r0out);
    AE_MULAFC16RAS_L(dl76, r65, cf65);
    AE_MULAFC16RAS_H(dl76, r65, cf65);
    AE_MULAFC16RAS_L(dl54, r43, cf43);
    AE_MULAFC16RAS_H(dl54, r43, cf43);
    AE_MULAFC16RAS_L(dl32, r21, cf21);
    AE_MULAFC16RAS_H(dl32, r21, cf21);
    AE_MULAFC16RAS_H(dl10, r0out, cf0);
    // Store the output sample
    AE_S16_0_IP(r0out, castxcc(ae_int16, pout), sz_i16);
  }

  // Save the updated delay elements
  dl0 = dl10;
  dl1 = AE_SEL16_5432(dl10, dl10);
  dl2 = dl32;
  dl3 = AE_SEL16_5432(dl32, dl32);
  dl4 = dl54;
  dl5 = AE_SEL16_5432(dl54, dl54);
  dl6 = dl76;
  dl7 = AE_SEL16_5432(dl76, dl76);
  delLine[0] = (ae_int16x4)dl0;
  delLine[1] = (ae_int16x4)dl1;
  delLine[2] = (ae_int16x4)dl2;
  delLine[3] = (ae_int16x4)dl3;
  delLine[4] = (ae_int16x4)dl4;
  delLine[5] = (ae_int16x4)dl5;
  delLine[6] = (ae_int16x4)dl6;
  delLine[7] = (ae_int16x4)dl7;
#else
        ae_int16   * restrict R;
  const ae_int16   * restrict X;
        ae_int16x4 *          D;

  ae_int16x4 dl_7654, dl_3210,
             cf0, cf1, cf2, cf3, cf4, cf5, cf6, cf7;
  ae_f32x2   dl_76, dl_54, dl_32, dl_10;
  ae_f32x2   dl_65, dl_43, dl_21;
  ae_f16x4   cf_7654, cf_3210;
  ae_f16x4   cf_6543, cf_2107;
  ae_int16x4 s0, s1, s2, s3;
  ae_int16x4 t0, t1;
  ae_f32x2   p0, p1, s_g;
  ae_f64     q0, q1;
  ae_int64   u0;

  int n;

  ASSERT( r && x && delLine && coef );

  ASSERT( IS_ALIGN( delLine ) && IS_ALIGN( coef ) );

  ASSERT( M == 8 );

  //
  // Setup pointers.
  //

  R = (      ae_int16   *)r;
  X = (const ae_int16   *)x;
  D = (      ae_int16x4 *)delLine;

  //
  // Load the prescaling factor.
  //
  
  // Q31 <- Q15 + 16
  s_g = AE_MOVDA32( ((int32_t)scale)<<16 );

  //
  // Pre-load the reflection coefficients and delay line elements.
  //
  
  // Q15
  cf0 = (ae_int16x4)coef[0];
  cf1 = (ae_int16x4)coef[1];
  cf2 = (ae_int16x4)coef[2];
  cf3 = (ae_int16x4)coef[3];
  cf4 = (ae_int16x4)coef[4];
  cf5 = (ae_int16x4)coef[5];
  cf6 = (ae_int16x4)coef[6];
  cf7 = (ae_int16x4)coef[7];
  t0 = AE_SEL16_4321( cf7, cf6 );
  t1 = AE_SEL16_4321( cf5, cf4 );
  s0 = AE_SEL16_7632( t0, t1 ); // k7 k6 k5 k4
  t0 = AE_SEL16_4321( cf3, cf2 );
  t1 = AE_SEL16_4321( cf1, cf0 );
  s1 = AE_SEL16_7632( t0, t1 ); // k3 k2 k1 k0

  // Make skew coefficient quadruples.
  s2 = AE_SEL16_6543( s0, s1 ); // k6 k5 k4 k3
  s3 = AE_SEL16_6543( s1, s0 ); // k2 k1 k0 k7

  cf_7654 = ( s0 );
  cf_3210 = ( s1 );
  cf_6543 = ( s2 );
  cf_2107 = ( s3 );

  // Load delay line elements d7..d0.
  // Q1.30 <- Q1.14 + 16
  dl_7654 = AE_L16X4_I( D, +0 ); // d7 d6 d5 d4
  dl_3210 = AE_L16X4_I( D, +8 ); // d3 d2 d1 d0
  dl_76 = AE_CVT32X2F16_32(dl_7654);
  dl_54 = AE_CVT32X2F16_10(dl_7654);
  dl_32 = AE_CVT32X2F16_32(dl_3210);
  dl_10 = AE_CVT32X2F16_10(dl_3210);

  //
  // Pass the input samples block through the AR lattice. n-th response sample
  // and lattice state update are defined as follows:
  //
  //   p6 = g*x[n] - k7*d7 - k6*d6
  //   p5 = p6 - k5*d5; p4 = p5 - k4*d4; p3 = p4 - k3*d3;
  //   p2 = p3 - k2*d2; p1 = p2 - k1*d1; p0 = p1 - k0*d0;
  //
  //   d7 = d6 + k6*p6; d6 = d5 + k5*p5; d5 = d4 + k4*p4;
  //   d4 = d3 + k3*p3; d3 = d2 + k2*p2; d2 = d1 + k1*p1;
  //   d1 = d0 + k0*p0; d0 = p0;
  //
  //   r[n] = p0;
  //
  // The inner loop is fully unrolled.
  //

  for ( n=0; n<N; n++ )
  {
    //--------------------------------------------------------------------------
    // Load the next input sample and prescale it.
      
    // Q15
    AE_L16_IP( t0, X, +2 );

    // q0 = g*x[n]
    // Q17.46 <- Q15*Q31
    u0 = AE_MUL32X16_L0( s_g, t0 );

    q0 = ( u0 );

    //--------------------------------------------------------------------------
    // Update delay elements d7, d6.

    // q0 -= k7*d7 + k6*d6
    // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ] + [ Q15*Q1.30 + 1 ]
    AE_MULSSFD32X16_H3_L2( q0, dl_76, cf_7654 );

    q1 = q0;

    // q1 -= k5*d5
    // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ]
    AE_MULSF32X16_H1( q1, dl_54, cf_7654 );

    // p6 = q0; p5 = q1
    // Q1.30 <- Q17.46 - 16 w/ rounding and saturation
    AE_PKSR32( p0, q0, 0 );
    AE_PKSR32( p0, q1, 0 );

    // d6 d5
    select_f32x2_lh( dl_65, dl_76, dl_54 );

    // d7 = d6 + k6*p6; d6 = d5 + k5*p5
    // Q1.30 <- Q1.30 + [ Q15*Q1.30 - 15 ] w/ rounding and saturation
    AE_MULAFP32X16X2RAS_H( dl_65, p0, cf_6543 );

    dl_76 = dl_65;

    //--------------------------------------------------------------------------
    // Update delay elements d5, d4.

    // d4 d3
    select_f32x2_lh( dl_43, dl_54, dl_32 );

    // q0 -= k5*d5 + k4*d4; q1 -= k4*d4 + k3*d3
    // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ] + [ Q15*Q1.30 + 1 ]
    AE_MULSSFD32X16_H1_L0( q0, dl_54, cf_7654 );
    AE_MULSSFD32X16_H1_L0( q1, dl_43, cf_6543 );

    // p4 = q0; p3 = q1
    // Q1.30 <- Q17.46 - 16 w/ rounding and saturation
    AE_PKSR32( p0, q0, 0 );
    AE_PKSR32( p0, q1, 0 );

    // d5 = d4 + k4*p4; d4 = d3 + k3*p3
    // Q1.30 <- Q1.30 + [ Q15*Q1.30 - 15 ] w/ rounding and saturation
    AE_MULAFP32X16X2RAS_L( dl_43, p0, cf_6543 );

    dl_54 = dl_43;

    //--------------------------------------------------------------------------
    // Update delay elements d3, d2.

    // d2 d1
    select_f32x2_lh( dl_21, dl_32, dl_10 );

    // q0 -= k3*d3 + k2*d2; q1 -= k2*d2 + k1*d1
    // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ] + [ Q15*Q1.30 + 1 ]
    AE_MULSSFD32X16_H3_L2( q0, dl_32, cf_3210 );
    AE_MULSSFD32X16_H3_L2( q1, dl_21, cf_2107 );

    // p2 = q0; p1 = q1
    // Q1.30 <- Q17.46 - 16 w/ rounding and saturation
    AE_PKSR32( p0, q0, 0 );
    AE_PKSR32( p0, q1, 0 );

    // d3 = d2 + k2*p2; d2 = d1 + k1*p1
    // Q1.30 <- Q1.30 + [ Q15*Q1.30 - 15 ] w/ rounding and saturation
    AE_MULAFP32X16X2RAS_H( dl_21, p0, cf_2107 );

    dl_32 = dl_21;

    //--------------------------------------------------------------------------
    // Update delay elements d1, d0.

    // q1 -= k0*d0
    // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ] + [ Q15*Q1.30 + 1 ]
    AE_MULSF32X16_L0( q1, dl_10, cf_3210 );

    // p0 = q1
    // Q1.30 <- Q17.46 - 16 w/ rounding and saturation
    AE_PKSR32( p0, q1, 0 );

    // d1 + k1*p0 (junk); d1 = d0 + k0*p0
    // Q1.30 <- Q1.30 + [ Q15*Q1.30 - 15 ] w/ rounding and saturation
    AE_MULAFP32X16X2RAS_L( dl_10, p0, cf_3210 );

    // d1; d0 = p0
    select_f32x2_ll( dl_10, dl_10, p0 );

    //--------------------------------------------------------------------------
    // Format and store response sample.

    // r[n] = q1
    // Q31 <- Q17.46 + 1 - 16 w/ rounding and saturation
    p1 = AE_MOVF32X2_FROMF64(AE_PKSR32_0( q1, 1 ));
    
    // Q15 <- Q31 - 16 w/ asym. rounding and saturation
    t1 = AE_ROUND16X4F32SASYM( p1, p1 );
    AE_S16_0_IP( t1, R, +2 );
  }

  //
  // Store updated delay line elements.
  //
  
  // Q1.14 <- Q1.30 - 16 w/ asym. rounding and saturation
  dl_7654 = AE_ROUND16X4F32SASYM( dl_76, dl_54 );
  dl_3210 = AE_ROUND16X4F32SASYM( dl_32, dl_10 );
  AE_S16X4_I( dl_7654, D, +0 );
  AE_S16X4_I( dl_3210, D, +8 );
#endif
} // latr8_proc()



/* Data processing function for a lattice filter of arbitrary order. */
void latrX_proc( int16_t * r,       const int16_t * x,
                 int16_t * delLine, const int16_t * coef,
                 int16_t scale, int N, int M )
{
  const ae_f16x4 * restrict Pin;
        ae_f16x4 * restrict Pout;
  const ae_f16x4 * restrict Pdl_ld;
        ae_f16x4 * restrict Pdl_st;
  const ae_f16x4 * restrict Pcoef;
  ae_int16x4 X, DL, CF;
  ae_f16x4 xin, rm, rout;
  ae_f16x4 dl0, dl1, cf0, cf1, sc;
  ae_f32x2 ACC, ACCD;

  int n, m;

  ASSERT(M > 8);
  
  sc = AE_MOVDA16X2(scale, scale);
  // Set the input and output pointers
  Pin  = (const ae_f16x4 *)x;
  Pout = (      ae_f16x4 *)r;
  
  __Pragma("loop_count min=1")
  for ( n=0; n<N; n++ )
  {
    // Set pointers to the delay elements and coefficients
    Pdl_st = (      ae_f16x4 *)(delLine+M-1);
    Pdl_ld = (const ae_f16x4 *)Pdl_st;
    Pcoef  = (const ae_f16x4 *)(coef+M-1);
    // Load (M-1)-th delay element and coefficient
    // dl[M-1] : Q14
    AE_L16_IP(DL, castxcc(const ae_int16, Pdl_ld), -(int)sz_i16);    dl1 = DL;
    // cf[M-1] : Q15
    AE_L16_IP(CF, castxcc(const ae_int16, Pcoef), -(int)sz_i16);    cf1 = CF;
    // Load the input sample x[n].
    AE_L16_IP(X, castxcc(const ae_int16, Pin), sz_i16);    xin = X;
    // Scale the input sample.
    // Q30 <- ( Q15*Q15 + 1 ) - 1 w/ rounding
    ACC = AE_MULF16SS_00(sc, xin);
    ACC = AE_SRAI32R(ACC, 1);
    // acc = acc - dl[M-1]*cf[M-1]
    // Q30 <- Q30 - ( Q14*Q15 + 1 )
    AE_MULSF16SS_00(ACC, dl1, cf1);

    __Pragma("loop_count min=2")
    for ( m=M-2; m>=0; m-- )
    {
      // Load m-th delay element and coefficient
      // dl[m] : Q14
      AE_L16_IP(DL, castxcc(const ae_int16, Pdl_ld), -(int)sz_i16);    dl0 = DL;
      // cf[m] : Q15
      AE_L16_IP(CF, castxcc(const ae_int16, Pcoef), -(int)sz_i16);    cf0 = CF;
      // acc = acc - dl[m]*cf[m]
      // Q30 <- Q30 - ( Q14*Q15 + 1 )
      AE_MULSF16SS_00(ACC, dl0, cf0);
      // Get the output sample of m-th section
      // Q14 <- Q30 - 16 w/ rounding and saturation
      rm = AE_ROUND16X4F32SASYM(ACC, ACC);
      // Compute dl[m+1] delay element
      // dl[m+1] = dl[m] + r[m]*cf[m]
      // Q30 <- Q30 + (Q14*Q15 + 1)
      ACCD = AE_CVT32X2F16_10(dl0);
      AE_MULAF16SS_00( ACCD, rm, cf0 );
      // Get the d[m+1] delay element
      // Q14 <- Q30 - 16 w/ rounding and saturation
      dl1 = AE_ROUND16X4F32SASYM(ACCD, ACCD);
      // Update the (m+1)-th delay line element.
      AE_S16_0_IP(dl1, castxcc(ae_int16, Pdl_st), -(int)sz_i16);
    }
    // Update the first delay line element with the resulting sample
    // Q14 <- Q30 - 16 w/ rounding and saturation
    dl1 = AE_ROUND16X4F32SASYM(ACC, ACC);
    AE_S16_0_IP(dl1, castxcc(ae_int16, Pdl_st), -(int)sz_i16);
    // Make the output sample.
    // Q15 <- Q30 + 1 - 16 w/ rounding and saturation
    ACC = AE_SLAI32S(ACC, 1);
    rout = AE_ROUND16X4F32SASYM(ACC, ACC);
    AE_S16_0_IP(rout, castxcc(ae_int16, Pout), sz_i16);
  }

} // latrX_proc()
