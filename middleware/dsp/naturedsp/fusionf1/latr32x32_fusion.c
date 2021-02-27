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
  NatureDSP Signal Processing Library. IIR part
    Lattice Block Real IIR, 32x32-bit
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

#define sz_i32 sizeof(int32_t)

/* Puts the high 32-bit element to the low part of a register *
 * and extends the sign to high 32 bits                       */
#define AE_SEXT_64_32X2_H(inout)                    \
        {                                           \
            ae_f64 tmp = AE_MOVF64_FROMF32X2(inout);\
            tmp = AE_SRAI64(tmp, 32);               \
            inout = AE_MOVF32X2_FROMF64(tmp);       \
        }

/* Instance pointer validation number. */
#define MAGIC     0x52efa965

/* Reserve memory for alignment. */
#define ALIGNED_SIZE( size, align ) \
      ( (size_t)(size) + (align) - 1 )

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
      (void*)( ( (uintptr_t)(addr) + ( (align) - 1 ) ) & ~( (align) - 1 ) )

/* Lattice filter data processing function. */
typedef void (proc_fxn_t)( int32_t * r,      // r[N]     [out   ] Q15
                     const int32_t * x,      // x[N]     [in    ] Q15
                           int32_t * delLine,// dline[M] [in/out] Q14
                     const int32_t * coef,   // coef[M]  [in    ] Q15
                           int32_t scale,    // scale    [in    ] Q15
                           int N, int M );

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

/* Filter instance structure. */
typedef struct tag_latr32x32_t
{
  uint32_t        magic;     // Instance pointer validation number
  int             M;         // Lattice filter order
  proc_fxn_t    * procFxn;   // Custom data processing function
  int32_t         scale;     // Input signal prescaling factor, Q31
  const int32_t * coef;      // M reflection coefficients, Q31
  int32_t       * delayLine; // M delay elements, Q30

} latr32x32_t, *latr32x32_ptr_t;

// Determine the memory area size for a filter instance.
size_t latr32x32_alloc( int M )
{
  ASSERT( M > 0 );

  return ( ALIGNED_SIZE( sizeof( latr32x32_t ), 4 )
           + // M delay elements
           ALIGNED_SIZE( M*sz_i32, 2*sz_i32 )
           + // M reflection coefficients
           ALIGNED_SIZE( M*sz_i32, 2*sz_i32 ) );

} // latr32x32_alloc()

// Given a memory area of sufficient size, initialize a filter instance and 
// return a handle to it.
latr32x32_handle_t latr32x32_init( void *             objmem, 
                                   int                M,
                                   const int32_t * restrict k,
                                   int32_t            scale )
{
  latr32x32_ptr_t latr;
  int32_t *       delayLine;
  int32_t *       coef;
  void *          ptr;

  int m;

  ASSERT( objmem && M > 0 && k );

  //
  // Partition the memory block
  //

  ptr = objmem;

  latr      = (latr32x32_ptr_t)ALIGNED_ADDR( ptr, 4 );
  ptr       = latr + 1;
  delayLine = (int32_t*)ALIGNED_ADDR( ptr, 2*sz_i32 );
  ptr       = delayLine + M;
  coef      = (int32_t*)ALIGNED_ADDR( ptr, 2*sz_i32 );
  ptr       = coef + M;

  ASSERT( (int8_t*)ptr - (int8_t*)objmem <= (int)latr32x32_alloc( M ) );

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

} // latr32x32_init()

// Process data. The filter instance is identified with a handle returned by
// the initializing function.
void latr32x32_process( latr32x32_handle_t _latr, 
                        int32_t * restrict     r,
                  const int32_t *              x, int N )
{
  latr32x32_ptr_t latr = (latr32x32_ptr_t)_latr;

        int32_t * delLine;
  const int32_t * coef;
  int32_t scale;
  int M;

  ASSERT( latr && latr->magic == MAGIC && r && x );

  delLine = latr->delayLine;
  coef    = latr->coef;
  scale   = latr->scale;
  M       = latr->M;

  if (N <= 0) return;
  latr->procFxn(r, x, delLine, coef, scale, N, M);

} // latr32x32_process()



/* Order 1 lattice filter data processing function. */
void latr1_proc( int32_t * r,       const int32_t * x,
                 int32_t * delLine, const int32_t * coef,
                 int32_t scale, int N, int M )
{
  const ae_f32x2 * restrict in;
        ae_f32x2 * restrict out;
  ae_f64  ACC, Rout;
  ae_int32x2 X;
  ae_f32x2 xin, rout;
  ae_f32x2 dl, cf, sc;
  int n;

  ASSERT(M == 1);
  // Set the input and output pointers
  in  = (const ae_f32x2 *)x;
  out = (      ae_f32x2 *)r;
  // Load the scale and reflection coefficients, the delay element
  dl = AE_L32_I((const ae_int32 *)delLine, 0);
  cf = AE_L32_I((const ae_int32 *)coef   , 0);
  sc = AE_MOVDA32X2(scale, scale);
  // sc : Q30 <- Q31 - 1 w/ rounding
  sc = AE_SRAI32R(sc, 1);

  __Pragma("loop_count min=1")
  for ( n=0; n<N; n++ )
  {
    // Load the input sample
    AE_L32_IP(X, castxcc(const ae_int32, in), sz_i32);
    xin = X;
    // Scale the input sample.
    // Q17.46 <- ( Q31*Q30 + 1 ) - 16 /w rounding
    ACC = AE_MULF32R_LL(xin, sc);
    // Q17.46 <- Q17.46 - [( Q30*Q31 + 1 ) - 16]
    AE_MULSF32R_LL(ACC, dl, cf);
    // Update the delay element with the resulting sample
    // Q30 <- Q17.46 - 16 w/ rounding and saturation
    dl = AE_ROUND32X2F48SASYM(ACC, ACC);
    // Make and store the output sample.
    // Q31 <- Q17.46 - 15 w/ rounding and saturation
    Rout = AE_PKSR32_0(ACC, 1);
    rout = AE_MOVF32X2_FROMF64(Rout);
    AE_S32_L_IP(rout, castxcc( ae_int32, out), sz_i32);
  }
  // Save the delay line
  AE_S32_L_I(dl, (ae_int32 *)delLine, 0);

} // latr1_proc()



/* Order 2 lattice filter data processing function. */
void latr2_proc( int32_t * r,       const int32_t * x,
                 int32_t * delLine, const int32_t * coef,
                 int32_t scale, int N, int M )
{
  const ae_f32x2   * restrict in;
        ae_f32x2   * restrict out;
        ae_int32   * restrict pdl;
  const ae_int32x2 * restrict pcoef;
  ae_f64  ACC, D1, D0;
  ae_int32x2 X;
  ae_f32x2 xin, r0, rout;
  ae_f32x2 dl1, dl0, cf01, sc;
  int n;

  ASSERT(M == 2);
  // Set the input and output pointers
  in    = (const ae_f32x2   *)x;
  out   = (      ae_f32x2   *)r;
  pdl   = (      ae_int32   *)delLine;
  pcoef = (const ae_int32x2 *)coef;
  // Load the scale and reflection coefficients, the delay elements
  dl0  = AE_L32_I(pdl  , 0);
  dl1  = AE_L32_I(pdl  , sz_i32);
  cf01 = AE_L32X2_I(pcoef, 0);
  sc = AE_MOVDA32X2(scale, scale);
  // D0 <- dl0
  // Q17.46 <- Q1.30
  D0 = AE_MOVF64_FROMF32X2(dl0);
  D0 = AE_SLAI64(D0, 32);
  D0 = AE_SRAI64(D0, 16);

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
    AE_L32_IP(X, castxcc(const ae_int32, in), sz_i32);
    xin = X;

    // Scale the input sample.
    // Q17.46 <- ( Q31*Q31 + 1 ) - 17
    ACC = AE_MULF32R_LL(xin, sc);
    ACC = AE_SRAI64(ACC, 1);
    // Compute the output sample
    // Q17.46 <- Q17.46 - ( Q30*Q31 + 1 )
    AE_MULSF32R_LH(ACC, dl0, cf01);
    AE_MULSF32R_LL(ACC, dl1, cf01);
    // Q30 <- Q17.46 - 16 w/ rounding and saturation
    r0 = AE_ROUND32X2F48SASYM(ACC, ACC);
    // Update delay elements
    // dl1 = dl0 + r[n]*cf0
    // dl0 = r[n]
    D1 = D0;
    AE_MULAF32R_LH(D1, r0, cf01);
    // Q30 <- Q17.46 - 16 w/ rounding and saturation
    D1 = AE_PKSR32_0(D1, 0);
    dl1 = AE_MOVF32X2_FROMF64(D1);
    D0 = ACC;
    dl0 = r0;

    // Make and store the output sample.
    // Q31 <- Q17.46 - 15 w/ rounding and saturation
    ACC = AE_PKSR32_0(ACC, 1);
    rout = AE_MOVF32X2_FROMF64(ACC);
    AE_S32_L_IP(rout, castxcc(ae_int32, out), sz_i32);
  }
  // Save the updated delay line elements
  AE_S32_L_I(dl0, pdl, 0);
  AE_S32_L_I(dl1, pdl, sz_i32);

} // latr2_proc()



/* Order 3 lattice filter data processing function. */
void latr3_proc( int32_t * r,       const int32_t * x,
                 int32_t * delLine, const int32_t * coef,
                 int32_t scale, int N, int M )
{
  const ae_f32x2   * restrict in;
        ae_f32x2   * restrict out;
        ae_int32   * restrict pdl;
  const ae_int32x2 * restrict pcoef;
  ae_f64  ACC, R1, R0, D2, D1;
  ae_int32x2 X;
  ae_f32x2 xin, r1, r0, rout;
  ae_f32x2 dl2, dl1, dl0,
           cf01, cf2sc;
  ae_int16x4 tmp0, tmp1, zero;
  int32_t cf2;
  int n;

  ASSERT(M == 3);

  zero = AE_ZERO16();
  // Set the input and output pointers
  in    = (const ae_f32x2   *)x;
  out   = (      ae_f32x2   *)r;
  pdl   = (      ae_int32   *)delLine;
  pcoef = (const ae_int32x2 *)coef;
  // Load the scale and reflection coefficients, the delay elements
  dl0 = AE_L32_I(pdl, 0*sz_i32);
  dl1 = AE_L32_I(pdl, 1*sz_i32);
  dl2 = AE_L32_I(pdl, 2*sz_i32);
  cf01 = AE_L32X2_I(pcoef, 0*sz_i32);
  cf2 = ((const int32_t *)pcoef)[2];
  cf2sc = AE_MOVDA32X2(cf2, scale);
  AE_SEXT_64_32X2_H(dl0);
  AE_SEXT_64_32X2_H(dl1);
  
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
    AE_L32_IP(X, castxcc(const ae_int32, in), sz_i32);
    xin = X;
    // Scale the input sample.
    // Q17.46 <- ( Q31*Q31 + 1 ) - 17
    ACC = AE_MULF32R_LL(xin, cf2sc);
    ACC = AE_SRAI64(ACC, 1);
    // Compute output samples of each section
    // Q17.46 <- Q17.46 - ( Q30*Q31 + 1 )
    AE_MULSF32R_LH(ACC, dl2, cf2sc);
    AE_MULSF32R_LL(ACC, dl1, cf01);
    R1 = ACC;
    AE_MULSF32R_LH(ACC, dl0, cf01);
    R0 = ACC;
    // Q30 <- Q17.46 - 16 w/ rounding and saturation
    r1 = AE_MOVF32X2_FROMF64(AE_PKSR32_0(R1, 0));
    r0 = AE_MOVF32X2_FROMF64(AE_PKSR32_0(R0, 0));
    
    // Update delay elements
    // dl2 = dl1 + cf1*r1;
    // dl1 = dl0 + cf0*r0;
    // dl0 = r0;
    // D2 <- dl1; D1 <- dl0
    // Q17.46 <- Q30
    tmp1 = AE_MOVINT16X4_FROMF32X2(dl1);
    tmp0 = AE_MOVINT16X4_FROMF32X2(dl0);
    tmp1 = AE_SEL16_6543(tmp1, zero);
    tmp0 = AE_SEL16_6543(tmp0, zero);
    D2 = AE_MOVF64_FROMINT16X4(tmp1);
    D1 = AE_MOVF64_FROMINT16X4(tmp0);
    // Q17.46 <- Q17.46 + [(Q30*Q31 + 1) - 16] /w rounding
    AE_MULAF32R_LL(D2, r1, cf01);
    AE_MULAF32R_LH(D1, r0, cf01);
    // Q30 <- Q17.46 - 16 w/ rounding and saturation
    dl2 = AE_MOVF32X2_FROMF64(AE_PKSR32_0(D2, 0));
    dl1 = AE_MOVF32X2_FROMF64(AE_PKSR32_0(D1, 0));
    dl0 = r0;

    // Make and store the output sample.
    // Q31 <- Q17.46 - 15 w/ rounding and saturation
    ACC = AE_PKSR32_0(ACC, 1);
    rout = AE_MOVF32X2_FROMF64(ACC);
    AE_S32_L_IP(rout, castxcc(ae_int32, out), sz_i32);
  }
  // Save the updated delay elements
  AE_S32_L_I(dl0, pdl, 0*sz_i32);
  AE_S32_L_I(dl1, pdl, 1*sz_i32);
  AE_S32_L_I(dl2, pdl, 2*sz_i32);

} // latr3_proc()



/* Order 4 lattice filter data processing function. */
void latr4_proc( int32_t * r,       const int32_t * x,
                 int32_t * delLine, const int32_t * coef,
                 int32_t scale, int N, int M )
{
  const ae_f32x2   * restrict in;
        ae_f32x2   * restrict out;
        ae_int32   * restrict pdl;
  const ae_int32x2 * restrict pcoef;
  ae_f64  ACC, R2, R1, R0,
           D3, D2, D1;
  ae_int32x2 X;
  ae_f32x2 xin, r2, r1, r0, rout;
  ae_f32x2 dl3, dl2, dl1, dl0,
           sc, cf01, cf23;
  ae_int16x4 tmp0, tmp1, tmp2, zero;
  int n;

  ASSERT(M == 4);
  zero = AE_ZERO16();
  // Set the input and output pointers
  in    = (const ae_f32x2   *)x;
  out   = (      ae_f32x2   *)r;
  pdl   = (      ae_int32   *)delLine;
  pcoef = (const ae_int32x2 *)coef;
  // Load the scale and reflection coefficients, the delay elements
  dl0 = AE_L32_I(pdl, 0*sz_i32);
  dl1 = AE_L32_I(pdl, 1*sz_i32);
  dl2 = AE_L32_I(pdl, 2*sz_i32);
  dl3 = AE_L32_I(pdl, 3*sz_i32);
  cf01 = AE_L32X2_I(pcoef, 0*sz_i32);
  cf23 = AE_L32X2_I(pcoef, 2*sz_i32);
  sc = AE_MOVDA32X2(scale, scale);
  AE_SEXT_64_32X2_H(dl0);
  AE_SEXT_64_32X2_H(dl1);
  AE_SEXT_64_32X2_H(dl2);
  
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
    AE_L32_IP(X, castxcc(const ae_int32, in), sz_i32);
    xin = X;
    // Scale the input sample.
    // Q17.46 <- ( Q31*Q31 + 1 ) - 17
    ACC = AE_MULF32R_LL(xin, sc);
    ACC = AE_SRAI64(ACC, 1);
    // Compute output samples of each section
    // Q17.46 <- Q17.46 - [( Q30*Q31 + 1 ) - 16]
    AE_MULSF32R_LL(ACC, dl3, cf23);
    AE_MULSF32R_LH(ACC, dl2, cf23);
    R2 = ACC;
    AE_MULSF32R_LL(ACC, dl1, cf01);
    R1 = ACC;
    AE_MULSF32R_LH(ACC, dl0, cf01);
    R0 = ACC;
    // Q30 <- Q17.46 - 16 w/ rounding and saturation
    r2 = AE_MOVF32X2_FROMF64(AE_PKSR32_0(R2, 0));
    r1 = AE_MOVF32X2_FROMF64(AE_PKSR32_0(R1, 0));
    r0 = AE_MOVF32X2_FROMF64(AE_PKSR32_0(R0, 0));
    
    // Update delay elements
    // dl3 = dl2 + cf2*r2;
    // dl2 = dl1 + cf1*r1;
    // dl1 = dl0 + cf0*r0;
    // dl0 = r0;
    // D3 <- dl2; D2 <- dl1; D1 <- dl0
    // Q17.46 <- Q30
    tmp2 = AE_MOVINT16X4_FROMF32X2(dl2);
    tmp1 = AE_MOVINT16X4_FROMF32X2(dl1);
    tmp0 = AE_MOVINT16X4_FROMF32X2(dl0);
    tmp2 = AE_SEL16_6543(tmp2, zero);
    tmp1 = AE_SEL16_6543(tmp1, zero);
    tmp0 = AE_SEL16_6543(tmp0, zero);
    D3 = AE_MOVF64_FROMINT16X4(tmp2);
    D2 = AE_MOVF64_FROMINT16X4(tmp1);
    D1 = AE_MOVF64_FROMINT16X4(tmp0);
    // Q17.46 <- Q17.46 + [(Q30*Q31 + 1) - 16] /w rounding
    AE_MULAF32R_LH(D3, r2, cf23);
    AE_MULAF32R_LL(D2, r1, cf01);
    AE_MULAF32R_LH(D1, r0, cf01);
    // Q30 <- Q17.46 - 16 w/ rounding and saturation
    dl3 = AE_MOVF32X2_FROMF64(AE_PKSR32_0(D3, 0));
    dl2 = AE_MOVF32X2_FROMF64(AE_PKSR32_0(D2, 0));
    dl1 = AE_MOVF32X2_FROMF64(AE_PKSR32_0(D1, 0));
    dl0 = r0;
    
    // Make and store the output sample.
    // Q31 <- Q17.46 - 15 w/ rounding and saturation
    ACC = AE_PKSR32_0(ACC, 1);
    rout = AE_MOVF32X2_FROMF64(ACC);
    AE_S32_L_IP(rout, castxcc( ae_int32, out), sz_i32);
  }
  // Save the updated delay elements
  AE_S32_L_I(dl0, pdl, 0*sz_i32);
  AE_S32_L_I(dl1, pdl, 1*sz_i32);
  AE_S32_L_I(dl2, pdl, 2*sz_i32);
  AE_S32_L_I(dl3, pdl, 3*sz_i32);

} // latr4_proc()



/* Order 5 lattice filter data processing function. */
void latr5_proc( int32_t * r,       const int32_t * x,
                 int32_t * delLine, const int32_t * coef,
                 int32_t scale, int N, int M )
{
  const ae_f32x2   * restrict in;
        ae_f32x2   * restrict out;
        ae_int32   * restrict pdl;
  const ae_int32x2 * restrict pcoef;
  ae_f64  ACC, R3, R2, R1, R0,
               D4, D3, D2, D1;
  ae_int32x2 X;
  ae_f32x2 xin, r3, r21, r0, rout;
  ae_f32x2 dl4, dl3, dl2, dl1, dl0,
           cf01, cf23, cf4sc;
  int32_t cf4;
  int n;

  ASSERT(M == 5);
  
  // Set the input and output pointers
  in    = (const ae_f32x2   *)x;
  out   = (      ae_f32x2   *)r;
  pdl   = (      ae_int32   *)delLine;
  pcoef = (const ae_int32x2 *)coef;
  // Load the scale and reflection coefficients, the delay elements
  dl0 = AE_L32_I(pdl, 0*sz_i32);
  dl1 = AE_L32_I(pdl, 1*sz_i32);
  dl2 = AE_L32_I(pdl, 2*sz_i32);
  dl3 = AE_L32_I(pdl, 3*sz_i32);
  dl4 = AE_L32_I(pdl, 4*sz_i32);
  AE_SEXT_64_32X2_H(dl0);
  AE_SEXT_64_32X2_H(dl1);
  AE_SEXT_64_32X2_H(dl2);
  AE_SEXT_64_32X2_H(dl3);
  cf01 = AE_L32X2_I(pcoef, 0*sz_i32);
  cf23 = AE_L32X2_I(pcoef, 2*sz_i32);
  cf4 = ((const int32_t *)pcoef)[4];
  cf4sc = AE_MOVDA32X2(cf4, scale);
  
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
    AE_L32_IP(X, castxcc(const ae_int32, in), sz_i32);
    xin = X;
    // Scale the input sample.
    // Q17.46 <- ( Q31*Q31 + 1 ) - 17
    ACC = AE_MULF32R_LL(xin, cf4sc);
    ACC = AE_SRAI64(ACC, 1);
    // Compute output samples of each section
    // Q17.46 <- Q17.46 - [( Q30*Q31 + 1 ) - 16]
    AE_MULSF32R_LH(ACC, dl4, cf4sc);
    AE_MULSF32R_LL(ACC, dl3, cf23);
    R3 = ACC;
    AE_MULSF32R_LH(ACC, dl2, cf23);
    R2 = ACC;
    AE_MULSF32R_LL(ACC, dl1, cf01);
    R1 = ACC;
    AE_MULSF32R_LH(ACC, dl0, cf01);
    R0 = ACC;
    // Q30 <- Q17.46 - 16 w/ rounding and saturation
    r3  = AE_ROUND32X2F48SASYM(R3, R3);
    r21 = AE_ROUND32X2F48SASYM(R2, R1);
    r0  = AE_MOVF32X2_FROMF64(AE_PKSR32_0(R0, 0));
    
    // Update delay elements
    // dl4 = dl3 + cf3*r3;
    // dl3 = dl2 + cf2*r2;
    // dl2 = dl1 + cf1*r1;
    // dl1 = dl0 + cf0*r0;
    // dl0 = r0;
    // D4 <- dl3; D3 <- dl2; D2 <- dl1; D1 <- dl0
    // Q17.46 <- Q30
    D4 = AE_SLAI64S(AE_MOVF64_FROMF32X2(dl3), 16);
    D3 = AE_SLAI64S(AE_MOVF64_FROMF32X2(dl2), 16);
    D2 = AE_SLAI64S(AE_MOVF64_FROMF32X2(dl1), 16);
    D1 = AE_SLAI64S(AE_MOVF64_FROMF32X2(dl0), 16);
    // Q17.46 <- Q17.46 + [(Q30*Q31 + 1) - 16] /w rounding
    AE_MULAF32R_LL(D4, r3 , cf23);
    AE_MULAF32R_HH(D3, r21, cf23);
    AE_MULAF32R_LL(D2, r21, cf01);
    AE_MULAF32R_LH(D1, r0 , cf01);
    // Q30 <- Q17.46 - 16 w/ rounding and saturation
    dl4 = AE_MOVF32X2_FROMF64(AE_PKSR32_0(D4, 0));
    dl3 = AE_MOVF32X2_FROMF64(AE_PKSR32_0(D3, 0));
    dl2 = AE_MOVF32X2_FROMF64(AE_PKSR32_0(D2, 0));
    dl1 = AE_MOVF32X2_FROMF64(AE_PKSR32_0(D1, 0));
    dl0 = r0;
    
    // Make and store the output sample.
    // Q31 <- Q17.46 - 15 w/ rounding and saturation
    ACC = AE_PKSR32_0(ACC, 1);
    rout = AE_MOVF32X2_FROMF64(ACC);
    AE_S32_L_IP(rout, castxcc(ae_int32, out), sz_i32);
  }
  // Save the updated delay elements
  AE_S32_L_I(dl0, pdl, 0*sz_i32);
  AE_S32_L_I(dl1, pdl, 1*sz_i32);
  AE_S32_L_I(dl2, pdl, 2*sz_i32);
  AE_S32_L_I(dl3, pdl, 3*sz_i32);
  AE_S32_L_I(dl4, pdl, 4*sz_i32);

} // latr5_proc()



/* Order 6 lattice filter data processing function. */
void latr6_proc( int32_t * r,       const int32_t * x,
                 int32_t * delLine, const int32_t * coef,
                 int32_t scale, int N, int M )
{
  const ae_f32x2 * restrict in;
        ae_f32x2 * restrict out;
        ae_f32x2 * restrict pdl;
  const ae_f32x2  * restrict pcoef;
  ae_f64  ACC, R4, R3, R2, R1, R0, Rout,
           D5, D4, D3, D2, D1;
  ae_int32x2 X;
  ae_f32x2 xin, r4, r32, r10, rout;
  ae_f32x2 dl54, dl32, dl10,
           cf01, cf23, cf45, sc;
  int n;

  ASSERT(M == 6);
  
  // Set the input and output pointers
  in    = (const ae_f32x2 *)x;
  out   = (      ae_f32x2 *)r;
  pdl   = (      ae_f32x2 *)delLine;
  pcoef = (const ae_f32x2 *)coef;
  // Load the scale and reflection coefficients, the delay elements
  dl54 = ae_f32x2_loadi(pdl, 0*sz_i32);
  dl32 = ae_f32x2_loadi(pdl, 2*sz_i32);
  dl10 = ae_f32x2_loadi(pdl, 4*sz_i32);
  cf01 = ae_f32x2_loadi(pcoef, 0*sz_i32);
  cf23 = ae_f32x2_loadi(pcoef, 2*sz_i32);
  cf45 = ae_f32x2_loadi(pcoef, 4*sz_i32);
  sc = AE_MOVDA32X2(scale, scale);
  
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
    AE_L32_IP(X, castxcc(const ae_int32, in), sz_i32);
    xin = X;
    // Scale the input sample.
    // Q17.46 <- ( Q31*Q31 + 1 ) - 17
    ACC = AE_MULF32R_LL(xin, sc);
    ACC = AE_SRAI64(ACC, 1);
    // Compute output samples of each section
    // Q17.46 <- Q17.46 - [( Q30*Q31 + 1 ) - 16]
    AE_MULSF32R_HL(ACC, dl54, cf45);
    AE_MULSF32R_LH(ACC, dl54, cf45);
    R4 = ACC;
    AE_MULSF32R_HL(ACC, dl32, cf23);
    R3 = ACC;
    AE_MULSF32R_LH(ACC, dl32, cf23);
    R2 = ACC;
    AE_MULSF32R_HL(ACC, dl10, cf01);
    R1 = ACC;
    AE_MULSF32R_LH(ACC, dl10, cf01);
    R0 = ACC;
    // Q30 <- Q17.46 - 16 w/ rounding and saturation
    r4  = AE_MOVF32X2_FROMF64(AE_PKSR32_0(R4, 0));
    r32 = AE_ROUND32X2F48SASYM(R3, R2);
    r10 = AE_ROUND32X2F48SASYM(R1, R0);
    
    // Make and store the output sample.
    // Q31 <- Q17.46 - 15 w/ rounding and saturation
    Rout = AE_PKSR32_0(ACC, 1);
    rout = AE_MOVF32X2_FROMF64(Rout);
    AE_S32_L_IP(rout, castxcc(ae_int32, out), sz_i32);
    
    // Update delay elements
    // dl5 = dl4 + cf4*r4;
    // dl4 = dl3 + cf3*r3;
    // dl3 = dl2 + cf2*r2;
    // dl2 = dl1 + cf1*r1;
    // dl1 = dl0 + cf0*r0;
    // dl0 = r0;
    // D5 <- dl4; D4 <- dl3; D3 <- dl2; D2 <- dl1; D1 <- dl0
    // Q17.46 <- Q30
    D5 = AE_SLAI64(AE_MOVF64_FROMF32X2(dl54), 32);
    D5 = AE_SRAI64(D5, 16);
    D4 = AE_SRAI64(AE_MOVF64_FROMF32X2(dl32), 32);
    D4 = AE_SLAI64(D4, 16);
    D3 = AE_SLAI64(AE_MOVF64_FROMF32X2(dl32), 32);
    D3 = AE_SRAI64(D3, 16);
    D2 = AE_SRAI64(AE_MOVF64_FROMF32X2(dl10), 32);
    D2 = AE_SLAI64(D2, 16);
    D1 = AE_SLAI64(AE_MOVF64_FROMF32X2(dl10), 32);
    D1 = AE_SRAI64(D1, 16);
    // Q17.46 <- Q17.46 + [(Q30*Q31 + 1) - 16] /w rounding
    AE_MULAF32R_LH(D5, r4 , cf45);
    AE_MULAF32R_HL(D4, r32, cf23);
    AE_MULAF32R_LH(D3, r32, cf23);
    AE_MULAF32R_HL(D2, r10, cf01);
    AE_MULAF32R_LH(D1, r10, cf01);
    // Q30 <- Q17.46 - 16 w/ rounding and saturation
    dl54 = AE_ROUND32X2F48SASYM(D5, D4);
    dl32 = AE_ROUND32X2F48SASYM(D3, D2);
    dl10 = AE_ROUND32X2F48SASYM(D1, R0);
  }
  // Save the updated delay elements
  ae_f32x2_storei(dl54, pdl, 0*sz_i32);
  ae_f32x2_storei(dl32, pdl, 2*sz_i32);
  ae_f32x2_storei(dl10, pdl, 4*sz_i32);

} // latr6_proc()



/* Order 7 lattice filter data processing function. */
void latr7_proc( int32_t * r,       const int32_t * x,
                 int32_t * delLine, const int32_t * coef,
                 int32_t scale, int N, int M )
{
  const ae_f32x2 * restrict in;
        ae_f32x2 * restrict out;
        ae_f32x2 * restrict pdl;
  const ae_f32x2 * restrict pcoef;
  ae_f64  ACC, R5, R4, R3, R2, R1, R0, Rout,
           D6, D5, D4, D3, D2, D1;
  ae_int32x2 X;
  ae_f32x2 xin, r54, r32, r10, rout;
  ae_f32x2 dl65, dl43, dl21, dl0,
           cf01, cf23, cf45, cf6sc;
  int32_t  cf6;
  int n;

  ASSERT(M == 7);
  
  // Set the input and output pointers
  in    = (const ae_f32x2 *)x;
  out   = (      ae_f32x2 *)r;
  pdl   = (      ae_f32x2 *)delLine;
  pcoef = (const ae_f32x2 *)coef;
  // Load the scale and reflection coefficients, the delay elements
  dl65 = ae_f32x2_loadi(pdl, 0*sz_i32);
  dl43 = ae_f32x2_loadi(pdl, 2*sz_i32);
  dl21 = ae_f32x2_loadi(pdl, 4*sz_i32);
  dl0  = AE_L32_I((ae_int32 *)pdl, 6*sz_i32);
  cf01 = ae_f32x2_loadi(pcoef, 0*sz_i32);
  cf23 = ae_f32x2_loadi(pcoef, 2*sz_i32);
  cf45 = ae_f32x2_loadi(pcoef, 4*sz_i32);
  cf6 = ((int32_t *)pcoef)[6];
  cf6sc = AE_MOVDA32X2(cf6, scale);
  
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
    AE_L32_IP(X, castxcc(const ae_int32, in), sz_i32);
    xin = X;
    // Scale the input sample.
    // Q17.46 <- ( Q31*Q31 + 1 ) - 17
    ACC = AE_MULF32R_LL(xin, cf6sc);
    ACC = AE_SRAI64(ACC, 1);
    // Compute output samples of each section
    // Q17.46 <- Q17.46 - [( Q30*Q31 + 1 ) - 16]
    AE_MULSF32R_HH(ACC, dl65, cf6sc);
    AE_MULSF32R_LL(ACC, dl65, cf45);
    R5 = ACC;
    AE_MULSF32R_HH(ACC, dl43, cf45);
    R4 = ACC;
    AE_MULSF32R_LL(ACC, dl43, cf23);
    R3 = ACC;
    AE_MULSF32R_HH(ACC, dl21, cf23);
    R2 = ACC;
    AE_MULSF32R_LL(ACC, dl21, cf01);
    R1 = ACC;
    AE_MULSF32R_LH(ACC, dl0, cf01);
    R0 = ACC;
    // Q30 <- Q17.46 - 16 w/ rounding and saturation
    r54 = AE_ROUND32X2F48SASYM(R5, R4);
    r32 = AE_ROUND32X2F48SASYM(R3, R2);
    r10 = AE_ROUND32X2F48SASYM(R1, R0);
    
    // Make and store the output sample.
    // Q31 <- Q17.46 - 15 w/ rounding and saturation
    Rout = AE_PKSR32_0(ACC, 1);
    rout = AE_MOVF32X2_FROMF64(Rout);
    AE_S32_L_IP(rout, castxcc(ae_int32, out), sz_i32);
    
    // Update delay elements
    // dl6 = dl5 + cf5*r5;
    // dl5 = dl4 + cf4*r4;
    // dl4 = dl3 + cf3*r3;
    // dl3 = dl2 + cf2*r2;
    // dl2 = dl1 + cf1*r1;
    // dl1 = dl0 + cf0*r0;
    // dl0 = r0;
    // D6 <- dl5; D5 <- dl4; D4 <- dl3; D3 <- dl2; D2 <- dl1; D1 <- dl0
    // Q17.46 <- Q30
    D6 = AE_SLAI64(AE_MOVF64_FROMF32X2(dl65), 32);
    D6 = AE_SRAI64(D6, 16);
    D5 = AE_SRAI64(AE_MOVF64_FROMF32X2(dl43), 32);
    D5 = AE_SLAI64(D5, 16);
    D4 = AE_SLAI64(AE_MOVF64_FROMF32X2(dl43), 32);
    D4 = AE_SRAI64(D4, 16);
    D3 = AE_SRAI64(AE_MOVF64_FROMF32X2(dl21), 32);
    D3 = AE_SLAI64(D3, 16);
    D2 = AE_SLAI64(AE_MOVF64_FROMF32X2(dl21), 32);
    D2 = AE_SRAI64(D2, 16);
    D1 = AE_SLAI64(AE_MOVF64_FROMF32X2(dl0), 32);
    D1 = AE_SRAI64(D1, 16);
    // Q17.46 <- Q17.46 + [(Q30*Q31 + 1) - 16] /w rounding
    AE_MULAF32R_HL(D6, r54, cf45);
    AE_MULAF32R_LH(D5, r54, cf45);
    AE_MULAF32R_HL(D4, r32, cf23);
    AE_MULAF32R_LH(D3, r32, cf23);
    AE_MULAF32R_HL(D2, r10, cf01);
    AE_MULAF32R_LH(D1, r10, cf01);
    // Q30 <- Q17.46 - 16 w/ rounding and saturation
    dl65 = AE_ROUND32X2F48SASYM(D6, D5);
    dl43 = AE_ROUND32X2F48SASYM(D4, D3);
    dl21 = AE_ROUND32X2F48SASYM(D2, D1);
    dl0 = r10;
  }
  // Save the updated delay elements
  ae_f32x2_storei(dl65, pdl, 0*sz_i32);
  ae_f32x2_storei(dl43, pdl, 2*sz_i32);
  ae_f32x2_storei(dl21, pdl, 4*sz_i32);
  AE_S32_L_I(dl0, (ae_int32 *)pdl, 6*sz_i32);

} // latr7_proc()



/* Order 8 lattice filter data processing function. */
void latr8_proc( int32_t * r,       const int32_t * x,
                 int32_t * delLine, const int32_t * coef,
                 int32_t scale, int N, int M )
{
  const ae_f32x2 * restrict in;
        ae_f32x2 * restrict out;
        ae_f32x2 * restrict pdl;
  const ae_f32x2 * restrict pcoef;
  ae_f64  ACC, R6, R5, R4, R3, R2, R1, R0, Rout,
           D7, D6, D5, D4, D3, D2, D1;
  ae_int32x2 X;
  ae_f32x2 xin, r6, r54, r32, r10, rout;
  ae_f32x2 dl76, dl54, dl32, dl10,
           cf01, cf23, cf45, cf67, sc;
  int n;

  ASSERT(M == 8);
  
  // Set the input and output pointers
  in    = (const ae_f32x2 *)x;
  out   = (      ae_f32x2 *)r;
  pdl   = (      ae_f32x2 *)delLine;
  pcoef = (const ae_f32x2 *)coef;
  // Load the scale and reflection coefficients, the delay elements
  dl76 = ae_f32x2_loadi(pdl, 0*sz_i32);
  dl54 = ae_f32x2_loadi(pdl, 2*sz_i32);
  dl32 = ae_f32x2_loadi(pdl, 4*sz_i32);
  dl10 = ae_f32x2_loadi(pdl, 6*sz_i32);
  cf01 = ae_f32x2_loadi(pcoef, 0*sz_i32);
  cf23 = ae_f32x2_loadi(pcoef, 2*sz_i32);
  cf45 = ae_f32x2_loadi(pcoef, 4*sz_i32);
  cf67 = ae_f32x2_loadi(pcoef, 6*sz_i32);
  sc = AE_MOVDA32X2(scale, scale);
  
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
    AE_L32_IP(X, castxcc(const ae_int32, in), sz_i32);
    xin = X;
    // Scale the input sample.
    // Q17.46 <- ( Q31*Q31 + 1 ) - 17
    ACC = AE_MULF32R_LL(xin, sc);
    ACC = AE_SRAI64(ACC, 1);
    // Compute output samples of each section
    // Q17.46 <- Q17.46 - [( Q30*Q31 + 1 ) - 16]
    AE_MULSF32R_HL(ACC, dl76, cf67);
    AE_MULSF32R_LH(ACC, dl76, cf67);
    R6 = ACC;
    AE_MULSF32R_HL(ACC, dl54, cf45);
    R5 = ACC;
    AE_MULSF32R_LH(ACC, dl54, cf45);
    R4 = ACC;
    AE_MULSF32R_HL(ACC, dl32, cf23);
    R3 = ACC;
    AE_MULSF32R_LH(ACC, dl32, cf23);
    R2 = ACC;
    AE_MULSF32R_HL(ACC, dl10, cf01);
    R1 = ACC;
    AE_MULSF32R_LH(ACC, dl10, cf01);
    R0 = ACC;
    // Q30 <- Q17.46 - 16 w/ rounding and saturation
    r6  = AE_MOVF32X2_FROMF64(AE_PKSR32_0(R6, 0));
    r54 = AE_ROUND32X2F48SASYM(R5, R4);
    r32 = AE_ROUND32X2F48SASYM(R3, R2);
    r10 = AE_ROUND32X2F48SASYM(R1, R0);

    // Make and store the output sample.
    // Q31 <- Q17.46 - 15 w/ rounding and saturation
    Rout = AE_PKSR32_0(ACC, 1);
    rout = AE_MOVF32X2_FROMF64(Rout);
    AE_S32_L_IP(rout, castxcc(ae_int32, out), sz_i32);
    
    // Update delay elements
    // dl7 = dl6 + cf6*r6;
    // dl6 = dl5 + cf5*r5;
    // dl5 = dl4 + cf4*r4;
    // dl4 = dl3 + cf3*r3;
    // dl3 = dl2 + cf2*r2;
    // dl2 = dl1 + cf1*r1;
    // dl1 = dl0 + cf0*r0;
    // dl0 = r0;
    // D7 <- dl6; D6 <- dl5; D5 <- dl4; D4 <- dl3; D3 <- dl2; D2 <- dl1; D1 <- dl0
    // Q17.46 <- Q30
    D7 = AE_SLAI64(AE_MOVF64_FROMF32X2(dl76), 32);
    D7 = AE_SRAI64(D7, 16);
    D6 = AE_SRAI64(AE_MOVF64_FROMF32X2(dl54), 32);
    D6 = AE_SLAI64(D6, 16);
    D5 = AE_SLAI64(AE_MOVF64_FROMF32X2(dl54), 32);
    D5 = AE_SRAI64(D5, 16);
    D4 = AE_SRAI64(AE_MOVF64_FROMF32X2(dl32), 32);
    D4 = AE_SLAI64(D4, 16);
    D3 = AE_SLAI64(AE_MOVF64_FROMF32X2(dl32), 32);
    D3 = AE_SRAI64(D3, 16);
    D2 = AE_SRAI64(AE_MOVF64_FROMF32X2(dl10), 32);
    D2 = AE_SLAI64(D2, 16);
    D1 = AE_SLAI64(AE_MOVF64_FROMF32X2(dl10), 32);
    D1 = AE_SRAI64(D1, 16);
    // Q17.46 <- Q17.46 + [(Q30*Q31 + 1) - 16] /w rounding
    AE_MULAF32R_LH(D7, r6 , cf67);
    AE_MULAF32R_HL(D6, r54, cf45);
    AE_MULAF32R_LH(D5, r54, cf45);
    AE_MULAF32R_HL(D4, r32, cf23);
    AE_MULAF32R_LH(D3, r32, cf23);
    AE_MULAF32R_HL(D2, r10, cf01);
    AE_MULAF32R_LH(D1, r10, cf01);
    // Q30 <- Q17.46 - 16 w/ rounding and saturation
    dl76 = AE_ROUND32X2F48SASYM(D7, D6);
    dl54 = AE_ROUND32X2F48SASYM(D5, D4);
    dl32 = AE_ROUND32X2F48SASYM(D3, D2);
    dl10 = AE_ROUND32X2F48SASYM(D1, R0);
  }
  // Save the updated delay elements
  ae_f32x2_storei(dl76, pdl, 0*sz_i32);
  ae_f32x2_storei(dl54, pdl, 2*sz_i32);
  ae_f32x2_storei(dl32, pdl, 4*sz_i32);
  ae_f32x2_storei(dl10, pdl, 6*sz_i32);

} // latr8_proc()



/* Data processing function for a lattice filter of arbitrary order. */
void latrX_proc( int32_t * r,       const int32_t * x,
                 int32_t * delLine, const int32_t * coef,
                 int32_t scale, int N, int M )
{
  const ae_f32x2 * restrict in;
        ae_f32x2 * restrict out;
  const ae_f32x2 * restrict pdl_ld;
        ae_f32x2 * restrict pdl_st;
  const ae_f32x2 * restrict pcoef;
  ae_f64  ACC, ACCD;
  ae_int32x2 X, DL, CF, zero;
  ae_f32x2 xin, rm, rout;
  ae_f32x2 dl1, dl0, cf1, cf0, sc;
  int n, m;

  ASSERT(M > 8);

  zero = AE_ZERO32();
  sc = AE_MOVDA32X2(scale, scale);
  // Set the input and output pointers
  in  = (const ae_f32x2 *)x;
  out = (      ae_f32x2 *)r;

  __Pragma("loop_count min=1")
  for ( n=0; n<N; n++ )
  {
    // Set pointers to the delay elements and coefficients
    pdl_st = (      ae_f32x2 *)(delLine + M-1);
    pdl_ld = (const ae_f32x2 *)pdl_st;
    pcoef  = (      ae_f32x2 *)(coef + M-1);
    // Load the input sample x[n].
    AE_L32_IP(X, castxcc(const ae_int32, in), sz_i32);
    xin = X;
    // Scale the input sample.
    // Q17.46 <- ( Q31*Q31 + 1 ) - 17
    ACC = AE_MULF32R_LL(xin, sc);
    ACC = AE_SRAI64(ACC, 1);
    
    // Load (M-1)-th delay element and coefficient
    // dl[M-1] : Q30
    AE_L32_IP(DL, castxcc(const ae_int32, pdl_ld), -(int)sz_i32);
    dl1 = DL;
    // cf[M-1] : Q31
    AE_L32_IP(CF, castxcc(const ae_int32, pcoef), -(int)sz_i32);
    cf1 = CF;
    // acc = acc - dl[M-1]*cf[M-1]
    // Q17.46 <- Q17.46 - [( Q30*Q31 + 1 ) - 16] /w rounding
    AE_MULSF32R_LL(ACC, dl1, cf1);
    
    __Pragma("loop_count min=2")
    for ( m=M-2; m>=0; m-- )
    {
      // Load m-th delay element and coefficient
      // dl[m] : Q30
      AE_L32_IP(DL, castxcc(const ae_int32, pdl_ld), -(int)sz_i32);
      dl0 = DL;
      // cf[m] : Q31
      AE_L32_IP(CF, castxcc(const ae_int32, pcoef), -(int)sz_i32);
      cf0 = CF;
      // acc = acc - dl[m]*cf[m]
      // Q17.46 <- Q17.46 - [( Q30*Q31 + 1 ) - 16] /w rounding
      AE_MULSF32R_LL(ACC, dl0, cf0);
      // Get the output sample of m-th section
      // Q30 <- Q17.46 - 16 w/ rounding and saturation
      rm = AE_ROUND32X2F48SASYM(ACC, ACC);
      // Compute dl[m+1] delay element
      // dl[m+1] = dl[m] + r[m]*cf[m]
      // ACCD <- dl0
      // Q17.46 <- Q30
      dl0 = AE_SEL32_LL(dl0, zero);
      ACCD = AE_SRAI64(AE_MOVF64_FROMF32X2(dl0), 16);
      // Q17.46 <- Q17.46 + [(Q30*Q31 + 1) - 16] /w rounding
      AE_MULAF32R_LL(ACCD, rm, cf0);
      
      // Get the d[m+1] delay element
      // Q30 <- Q17.46 - 16 w/ rounding and saturation
      dl1 = AE_MOVF32X2_FROMF64(AE_PKSR32_0(ACCD, 0));
      // Update the (m+1)-th delay line element.
      AE_S32_L_IP(dl1, castxcc( ae_int32, pdl_st), -(int)sz_i32);
    }

    // Update the first delay line element with the resulting sample
    // Q30
    AE_S32_L_IP(rm, castxcc(ae_int32, pdl_st), -(int)sz_i32);
    
    // Make and store the output sample.
    // Q31 <- Q17.46 - 15 w/ rounding and saturation
    ACC = AE_PKSR32_0(ACC, 1);
    rout = AE_MOVF32X2_FROMF64(ACC);
    AE_S32_L_IP(rout, castxcc(ae_int32, out), sz_i32);
  }

} // latrX_proc()
