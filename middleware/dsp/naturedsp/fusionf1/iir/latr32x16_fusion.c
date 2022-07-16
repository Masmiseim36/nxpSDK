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
    Lattice Block Real IIR, 32x16-bit
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

/* Instance pointer validation number. */
#define MAGIC     0x1c6951b2

/* Reserve memory for alignment. */
#define ALIGNED_SIZE( size, align ) \
      ( (size_t)(size) + (align) - 1 )

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
      (void*)( ( (uintptr_t)(addr) + ( (align) - 1 ) ) & ~( (align) - 1 ) )

#define sz_i16 sizeof(int16_t)
#define sz_i32 sizeof(int32_t)

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

/* AE_SEL32_HH adapted for ae_f32x2 data type. */
#define select_f32x2_hh( s_out, s_in0, s_in1 ) \
  { \
    ae_int32x2 t0 = ( s_in0 ); \
    ae_int32x2 t1 = ( s_in1 ); \
    ae_int32x2 t2; \
    \
    t2 = AE_SEL32_HH( t0, t1 ); \
    \
    s_out = ( t2 ); \
  }

/* Lattice filter data processing function. */
typedef void (proc_fxn_t)( int32_t * restrict r,     // r[N]     [out   ] Q31
                     const int32_t * restrict x,     // x[N]     [in    ] Q31
                           int32_t *          dline, // dline[M] [in/out] Q30
                     const int16_t *          coef,  // coef[M]  [in    ] Q15
                     int16_t scale, int N, int M );  // scale    [in    ] Q15

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

/* Filter instance structure. */
typedef struct tag_latr32x16_t
{
  uint32_t        magic;     // Instance pointer validation number
  int             M;         // Lattice filter order
  proc_fxn_t *    procFxn;   // Custom data processing function
  int16_t         scale;     // Input signal prescaling factor, Q15
  const int16_t * coef;      // M reflection coefficients, Q15
  int32_t *       delayLine; // M delay elements, Q30

} latr32x16_t, *latr32x16_ptr_t;

// Determine the memory area size for a filter instance.
size_t latr32x16_alloc( int M )
{
  int M2, M4;

  ASSERT( M > 0 );

  // Complement M to the next multiples of 2 and 4.
  M2 = M + ( -M & 1 );
  M4 = M + ( -M & 3 );

  return ( ALIGNED_SIZE( sizeof( latr32x16_t ), 4 )
           + // M delay elements
           ALIGNED_SIZE( M2*sz_i32, 8 )
           + // M reflection coefficients
           ALIGNED_SIZE( M4*sz_i16, 8 ) );

} // latr32x16_alloc()

// Given a memory area of sufficient size, initialize a filter instance and 
// return a handle to it.
latr32x16_handle_t latr32x16_init( void *             objmem, 
                                   int                M,
                             const int16_t * restrict k,
                                   int16_t            scale )
{
  latr32x16_ptr_t latr;

  proc_fxn_t * procFxn;
  int32_t *    delayLine;
  int16_t *    coef16;
  int32_t *    coef32;
  void *       ptr;

  int M2, M4;
  int m;

  ASSERT( objmem && M > 0 && k );

  //
  // Select the data processing function.
  //

  procFxn = ( M == 1 ? &latr1_proc :
              M == 2 ? &latr2_proc :
              M == 3 ? &latr3_proc :
              M == 4 ? &latr4_proc :
              M == 5 ? &latr5_proc :
              M == 6 ? &latr6_proc :
              M == 7 ? &latr7_proc : 
              M == 8 ? &latr8_proc : &latrX_proc );

  //
  // Partition the memory block
  //

  // Complement M to the next multiples of 2 and 4.
  M2 = M + ( -M & 1 );
  M4 = M + ( -M & 3 );

  ptr       = objmem;
  latr      = (latr32x16_ptr_t)(ALIGNED_ADDR( ptr, 4 ));
  ptr       = latr + 1;
  delayLine = (int32_t*)(ALIGNED_ADDR( ptr, 8 ));
  ptr       = delayLine + M2;
  coef16    = (int16_t*)(ALIGNED_ADDR( ptr, 8 ));
  ptr       = coef16 + M4;

  ASSERT( (int8_t*)ptr - (int8_t*)objmem <= (int)latr32x16_alloc( M ) );

  //
  // Copy reflection coefficients.
  //

  if ( procFxn == latrX_proc )
  {
    //
    // For a lattice filter based on the generic processing function, pack
    // the coefficients into 32-bit words such that the higher numbered
    // coefficient in each pair occupies 16 MSBs. Pairs are stored in reverted
    // order. The processing function uses 32-bit memory loads to read
    // coefficients, and the code is invariant over the endianness selection.
    //

    coef32 = (int32_t*)coef16;

    for ( m=0; m<(M>>1); m++ )
    {
      coef32[m] = ( ( int32_t)k[M-1-2*m-0] << 16 ) |
                  ( (uint16_t)k[M-1-2*m-1] <<  0 );
    }

    if ( M & 1 )
    {
      coef32[m++] = ( (int32_t)k[0] << 16 );
    }

    for ( ; m<(M4>>1); m++ )
    {
      coef32[m] = 0;
    }
  }
  else
  {
    //
    // For a lattice filter posessing custom data processing function, just copy
    // the coefficients in reverted order, and append zeros to make the whole
    // number of coefficients a multiple of 4.
    //

    for ( m=0; m<M; m++ )
    {
      coef16[m] = k[M-1-m];
    }

    for ( ; m<M4; m++ )
    {
      coef16[m] = 0;
    }
  }

  //
  // Zero the delay line.
  //

  for ( m=0; m<M2; m++ )
  {
    delayLine[m] = 0;
  }

  //
  // Initialize the filter instance.
  //

  latr->magic     = MAGIC;
  latr->M         = M;
  latr->procFxn   = procFxn;
  latr->scale     = scale;
  latr->coef      = coef16;
  latr->delayLine = delayLine;

  return (latr);

} // latr32x16_init()

// Process data. The filter instance is identified with a handle returned by
// the initializing function.
void latr32x16_process( latr32x16_handle_t _latr, 
                        f24 * restrict     r,
                  const f24 *              x, int N )
{
  latr32x16_ptr_t latr = (latr32x16_ptr_t)_latr;

  ASSERT( latr && latr->magic == MAGIC && r && x );

  ASSERT( latr->procFxn );

  ( *latr->procFxn )( r, x,
                      latr->delayLine,
                      latr->coef,
                      latr->scale,
                      N,
                      latr->M );

} // latr32x16_process()

/* Order 1 lattice filter data processing function. */
static void latr1_proc( int32_t * restrict r,     // r[N]     [out   ] Q31
                  const int32_t * restrict x,     // x[N]     [in    ] Q31
                        int32_t *          dline, // dline[M] [in/out] Q30
                  const int16_t *          coef,  // coef[M]  [in    ] Q15
                  int16_t scale, int N, int M )   // scale    [in    ] Q15
{
        ae_int32 * restrict R;
  const ae_int32 * restrict X;
        ae_f32x2 *          D;
  const ae_f16x4 *          C;

  ae_f32x2   dl_0z, dl_x0;
  ae_f16x4   cf_0zzz;
  ae_int16x4 s_g;
  ae_f32x2   p0;
  ae_int32x2 t0, t1;
  ae_f64     q0;
  ae_int64   u0;

  int n;

  ASSERT( r && x && dline && coef );

  ASSERT( IS_ALIGN( dline ) && IS_ALIGN( coef ) );

  ASSERT( M == 1 );

  //
  // Setup pointers.
  //

  R = (      ae_int32 *)r;
  X = (const ae_int32 *)x;
  D = (      ae_f32x2 *)dline;
  C = (const ae_f16x4 *)coef;

  //
  // Load the prescaling factor.
  //

  // Q15
  s_g = AE_MOVDA16X2( 0, (unsigned)scale );

  //
  // Pre-load the reflection coefficient and delay line element.
  //

  // Q15
  cf_0zzz = ae_f16x4_loadi( C, +0 );

  // Load delay line element d0.
  // Q1.30
  dl_0z = ae_f32x2_loadi( D, +0 );

  // <don't care>; d0
  select_f32x2_hh( dl_x0, dl_0z, dl_0z );

  //
  // Pass the input samples block through the AR lattice. n-th response sample
  // and lattice state update are defined as follows:
  //
  //   q0 = g*x[n] - k0*d0;
  //   d0 = q0;
  //   r[n] = q0;
  //

  for ( n=0; n<N; n++ )
  {
    // Q31
    AE_L32_IP( t0, X, +4 );

    // Prescaling, q0 = g*x[n]
    // Q17.46 <- Q15*Q31
    u0 = AE_MUL32X16_L0( t0, s_g );

    q0 = ( u0 );

    // q0 -= k0*d0
    // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ]
    AE_MULSF32X16_L3( q0, dl_x0, cf_0zzz );

    // <don't care>; d0 = q0
    // Q1.30 <- Q17.46 - 16 w/ rounding and saturation
    AE_PKSR32( dl_x0, q0, 0 );

    // r[n] = q0
    // Q31 <- Q17.46 + 1 - 16 w/ rounding and saturation
    AE_PKSR32( p0, q0, 1 );

    t1 = ( p0 );

    // Q31
    AE_S32_L_IP( t1, R, +4 );
  }

  //
  // Store the updated delay line element.
  //

  // d0 0
  select_f32x2_ll( dl_0z, dl_x0, dl_0z );

  // Q1.30
  ae_f32x2_storei( dl_0z, D, +0 );

} // latr1_proc()

/* Order 2 lattice filter data processing function. */
static void latr2_proc( int32_t * restrict r,     // r[N]     [out   ] Q31
                  const int32_t * restrict x,     // x[N]     [in    ] Q31
                        int32_t *          dline, // dline[M] [in/out] Q30
                  const int16_t *          coef,  // coef[M]  [in    ] Q15
                  int16_t scale, int N, int M )   // scale    [in    ] Q15
{
        ae_int32   * restrict R;
  const ae_int32   * restrict X;
        ae_f32x2   *          D;
  const ae_int16x4 *          C;

  ae_f32x2   dl_10;
  ae_f16x4   cf_10zz;
  ae_f32x2   cf_22;
  ae_int16x4 s0, s_g;
  ae_f32x2   p0;
  ae_int32x2 t0, t1;
  ae_f64     q0;
  ae_int64   u0;

  int n;

  ASSERT( r && x && dline && coef );

  ASSERT( IS_ALIGN( dline ) && IS_ALIGN( coef ) );

  ASSERT( M == 2 );

  //
  // Setup pointers.
  //

  R = (      ae_int32   *)r;
  X = (const ae_int32   *)x;
  D = (      ae_f32x2   *)dline;
  C = (const ae_int16x4 *)coef;

  //
  // Load the prescaling factor.
  //

  // Q15
  s_g = AE_MOVDA16( (unsigned)scale );

  //
  // Pre-load the reflection coefficients and delay line elements.
  //

  // Q15
  s0 = AE_L16X4_I( C, +0 ); // k1 k0 0 0

  cf_10zz = ( s0 );

  // k1*k0; k1*k0
  // Q31 <- Q15*Q15 + 1
  cf_22 = AE_MULF16SS_32( cf_10zz, cf_10zz );

  // Load delay line elements d1,d0.
  // Q1.30
  dl_10 = ae_f32x2_loadi( D, +0 );

  //
  // Pass the input samples block through the AR lattice. n-th response sample
  // and lattice state update are defined as follows:
  //   q0 = g*x[n] - k1*d1 - k0*d0
  //   d1 = d0 + k0*q0; d0 = q0;
  //   r[n] = q0;
  //
  // In order to shorten the dependency path, we reorder the calculations:
  //   q0 = g*x[n] - k1*d1 - k0*(1+k1)*d0
  //   d1 = d0; d0 = q0;
  //   r[n] = q0;
  //
  // This modified algorithm is not bit exact with the original one, but it
  // is faster and provides at least the same performance in terms of the 
  // round-off error.
  //

  for ( n=0; n<N; n++ )
  {
    //--------------------------------------------------------------------------
    // Load the next input sample and prescale it.

    // Q31
    AE_L32_IP( t0, X, +4 );

    // q0 = g*x[n]
    // Q17.46 <- Q15*Q31
    u0 = AE_MUL32X16_L0( t0, s_g );

    q0 = ( u0 );

    //--------------------------------------------------------------------------
    // Update delay elements d1, d0.

    // q0 -= k1*d1 + k0*d0 
    // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ] + [ Q15*Q1.30 + 1 ]
    AE_MULSSFD32X16_H3_L2( q0, dl_10, cf_10zz );

    // q0 -= k1*k0*d0
    // Q16.46 <- Q16.46 + [ Q31*Q1.30 - 15 ] w/ symmetric rounding
    AE_MULSF32R_LL( q0, dl_10, cf_22 );

    // d1 = d0; d0 = q0
    // Q1.30 <- Q17.46 - 16 w/ rounding and saturation
    AE_PKSR32( dl_10, q0, 0 );

    //--------------------------------------------------------------------------
    // Format and store response sample.

    // r[n] = q0
    // Q31 <- Q17.46 + 1 - 16 w/ rounding and saturation
    AE_PKSR32( p0, q0, 1 );

    t1 = ( p0 );

    // Q31
    AE_S32_L_IP( t1, R, +4 );
  }

  //
  // Store updated delay line elements.
  //

  // Q1.30
  ae_f32x2_storei( dl_10, D, +0 );

} // latr2_proc()

/* Order 3 lattice filter data processing function. */
static void latr3_proc( int32_t * restrict r,     // r[N]     [out   ] Q31
                  const int32_t * restrict x,     // x[N]     [in    ] Q31
                        int32_t *          dline, // dline[M] [in/out] Q30
                  const int16_t *          coef,  // coef[M]  [in    ] Q15
                  int16_t scale, int N, int M )   // scale    [in    ] Q15
{
        ae_int32   * restrict R;
  const ae_int32   * restrict X;
        ae_f32x2   *          D;
  const ae_int16x4 *          C;

  ae_f32x2   dl_21, dl_0z, dl_x0;
  ae_f16x4   cf_210z, cf_10z2;
  ae_int16x4 s0, s1, s_g;
  ae_f32x2   p0;
  ae_int32x2 t0, t1, t_rnd;
  ae_f64     q0, q1, q2, q_rnd;
  ae_int64   u0, u1, u2;

  int n;

  ASSERT( r && x && dline && coef );

  ASSERT( IS_ALIGN( dline ) && IS_ALIGN( coef ) );

  ASSERT( M == 3 );

  //
  // Setup pointers.
  //

  R = (      ae_int32   *)r;
  X = (const ae_int32   *)x;
  D = (      ae_f32x2   *)dline;
  C = (const ae_int16x4 *)coef;

  //
  // Load the prescaling factor.
  //

  // Q15
  s_g = AE_MOVDA16X2( 1, (unsigned)scale );

  // Rounding term for Q17.46 -> Q1.30 conversion.
  t_rnd = AE_MOVDA32X2( 0, (1<<15) );

  q_rnd = AE_MOVF64_FROMINT32X2( t_rnd );

  //
  // Pre-load the reflection coefficients and delay line elements.
  //

  // Q15
  s0 = AE_L16X4_I( C, +0 ); // k2 k1 k0 0

  // Make a skew coefficient quadruple.
  s1 = AE_SEL16_6543( s0, s0 ); // k1 k0 0 k2

  cf_210z = ( s0 );
  cf_10z2 = ( s1 );

  // Load delay line elements d2..d0.
  // Q1.30
  dl_21 = ae_f32x2_loadi( D, +0 );
  dl_0z = ae_f32x2_loadi( D, +8 );

  // <don't care>; d0
  select_f32x2_hh( dl_x0, dl_0z, dl_0z );

  //
  // Pass the input samples block through the AR lattice. n-th response sample
  // and lattice state update are defined as follows:
  //
  //   p1 = g*x[n] - k2*d2 - k1*d1
  //   p0 = p1 - k0*d0;
  //
  //   d2 = d1 + k1*p1;
  //   d1 = d0 + k0*p0;
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

    // Q31
    AE_L32_IP( t0, X, +4 );

    // <rounding term> x[n]
    t0 = AE_SEL32_LL( t_rnd, t0 );

    // q0 = <rounding term> + g*x[n]
    // Q17.46 <- Q15*Q31
    u0 = AE_MULZAAD32X16_H1_L0( t0, s_g );

    q0 = ( u0 );
    q1 = ( u0 );

    //--------------------------------------------------------------------------
    // Update delay elements d2, d1.

    // q1 -= k0*d0
    // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ]
    AE_MULSF32X16_L1( q1, dl_x0, cf_210z );

    // q0 -= k2*d2 + k1*d1
    // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ] + [ Q15*Q1.30 + 1 ]
    AE_MULSSFD32X16_H3_L2( q0, dl_21, cf_210z );
    AE_MULSSFD32X16_H3_L2( q1, dl_21, cf_210z );

    u1 = ( q0 );
    u2 = ( q1 );

    // p1 = q0; p0 = q1
    // Q1.30 <- Q17.46 + 16 - 32 w/ truncation.
    // Actually we get rounded values due to the rounding term.
    t1 = AE_TRUNCA32X2F64S( u1, u2, 16 );

    p0 = ( t1 );

    // d1 d0
    select_f32x2_ll( dl_21, dl_21, dl_x0 );

    // d2 = d1 + k1*p1; d1 = d0 + k0*p0
    // Q1.30 <- Q1.30 + [ Q15*Q1.30 - 15 ] w/ rounding and saturation
    AE_MULAFP32X16X2RAS_H( dl_21, p0, cf_10z2 );

    //--------------------------------------------------------------------------
    // Update delay element d0, format and store response sample.

    // <don't care>; d0 = p0
    dl_x0 = p0;

    // r[n] = q1 - <rounding term>
    q2 = AE_F64_SUBS( q1, q_rnd );

    // Q16.47 <- Q17.46 + 1
    q2 = AE_F64_SLAIS( q2, 1 );

    // Q31 <- Q16.47 - 16 w/ rounding and saturation
    AE_S32RA64S_IP( q2, R, +4 );
  }

  //
  // Store updated delay line elements.
  //

  // d0 0
  select_f32x2_ll( dl_0z, dl_x0, dl_0z );

  // Q1.30
  ae_f32x2_storei( dl_21, D, +0 );
  ae_f32x2_storei( dl_0z, D, +8 );

} // latr3_proc()

/* Order 4 lattice filter data processing function. */
static void latr4_proc( int32_t * restrict r,     // r[N]     [out   ] Q31
                  const int32_t * restrict x,     // x[N]     [in    ] Q31
                        int32_t *          dline, // dline[M] [in/out] Q30
                  const int16_t *          coef,  // coef[M]  [in    ] Q15
                  int16_t scale, int N, int M )   // scale    [in    ] Q15
{
        ae_int32   * restrict R;
  const ae_int32   * restrict X;
        ae_f32x2   *          D;
  const ae_int16x4 *          C;

  ae_f32x2   dl_32, dl_10;
  ae_f32x2   dl_21;
  ae_f16x4   cf_3210;
  ae_f16x4   cf_2103;
  ae_int16x4 s0, s1, s_g;
  ae_f32x2   p0, p1;
  ae_int32x2 t0, t1;
  ae_f64     q0, q1;
  ae_int64   u0;

  int n;

  ASSERT( r && x && dline && coef );

  ASSERT( IS_ALIGN( dline ) && IS_ALIGN( coef ) );

  ASSERT( M == 4 );

  //
  // Setup pointers.
  //

  R = (      ae_int32   *)r;
  X = (const ae_int32   *)x;
  D = (      ae_f32x2   *)dline;
  C = (const ae_int16x4 *)coef;

  //
  // Load the prescaling factor.
  //

  // Q15
  s_g = AE_MOVDA16( (unsigned)scale );

  //
  // Pre-load the reflection coefficients and delay line elements.
  //

  // Q15
  s0 = AE_L16X4_I( C, +0 ); // k3 k2 k1 k0

  // Make a skew coefficient quadruple.
  s1 = AE_SEL16_6543( s0, s0 ); // k2 k1 k0 k3

  cf_3210 = ( s0 );
  cf_2103 = ( s1 );

  // Load delay line elements d3..d0.
  // Q1.30
  dl_32 = ae_f32x2_loadi( D, +0 );
  dl_10 = ae_f32x2_loadi( D, +8 );

  //
  // Pass the input samples block through the AR lattice. n-th response sample
  // and lattice state update are defined as follows:
  //
  //   p2 = g*x[n] - k3*d3 - k2*d2
  //   p1 = p2 - k1*d1;
  //   p0 = p1 - k0*d0;
  //
  //   d3 = d2 + k2*p2;
  //   d2 = d1 + k1*p1;
  //   d1 = d0 + k0*p0;
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

    // Q31
    AE_L32_IP( t0, X, +4 );

    // q0 = g*x[n]
    // Q17.46 <- Q15*Q31
    u0 = AE_MUL32X16_L0( t0, s_g );

    q0 = ( u0 );

    //--------------------------------------------------------------------------
    // Update delay elements d3, d2.

    // q0 -= k3*d3 + k2*d2
    // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ] + [ Q15*Q1.30 + 1 ]
    AE_MULSSFD32X16_H3_L2( q0, dl_32, cf_3210 );

    q1 = q0;

    // q1 -= k1*d1
    // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ]
    AE_MULSF32X16_H1( q1, dl_10, cf_3210 );

    // p2 = q0; p1 = q1
    // Q1.30 <- Q17.46 - 16 w/ rounding and saturation
    AE_PKSR32( p0, q0, 0 );
    AE_PKSR32( p0, q1, 0 );

    // d2 d1
    select_f32x2_lh( dl_21, dl_32, dl_10 );

    // d3 = d2 + k2*p2; d2 = d1 + k1*p1
    // Q1.30 <- Q1.30 + [ Q15*Q1.30 - 15 ] w/ rounding and saturation
    AE_MULAFP32X16X2RAS_H( dl_21, p0, cf_2103 );

    dl_32 = dl_21;

    //--------------------------------------------------------------------------
    // Update delay elements d1, d0.

    // q0 -= k1*d1 + k0*d0 
    // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ] + [ Q15*Q1.30 + 1 ]
    AE_MULSSFD32X16_H1_L0( q0, dl_10, cf_3210 );

    // p0 = q0
    // Q1.30 <- Q17.46 - 16 w/ rounding and saturation
    AE_PKSR32( p0, q0, 0 );

    // d1 = d0 + k0*p0
    // Q1.30 <- Q1.30 + [ Q15*Q1.30 - 15 ] w/ rounding and saturation
    AE_MULAFP32X16X2RAS_L( dl_10, p0, cf_3210 );

    // d1; d0 = p0
    select_f32x2_ll( dl_10, dl_10, p0 );

    //--------------------------------------------------------------------------
    // Format and store response sample.

    // r[n] = q0
    // Q31 <- Q17.46 + 1 - 16 w/ rounding and saturation
    AE_PKSR32( p1, q0, 1 );

    t1 = ( p1 );

    // Q31
    AE_S32_L_IP( t1, R, +4 );
  }

  //
  // Store updated delay line elements.
  //

  // Q1.30
  ae_f32x2_storei( dl_32, D, +0 );
  ae_f32x2_storei( dl_10, D, +8 );

} // latr4_proc()

/* Order 5 lattice filter data processing function. */
static void latr5_proc( int32_t * restrict r,     // r[N]     [out   ] Q31
                  const int32_t * restrict x,     // x[N]     [in    ] Q31
                        int32_t *          dline, // dline[M] [in/out] Q30
                  const int16_t *          coef,  // coef[M]  [in    ] Q15
                  int16_t scale, int N, int M )   // scale    [in    ] Q15
{
        ae_int32   * restrict R;
  const ae_int32   * restrict X;
        ae_f32x2   *          D;
  const ae_int16x4 *          C;

  ae_f32x2   dl_43, dl_21, dl_0z;
  ae_f16x4   cf_4321, cf_0zzz;
  ae_f16x4   cf_3210;
  ae_int16x4 s0, s1, s2, s_g;
  ae_f32x2   p0, p1, p2;
  ae_int32x2 t0, t1;
  ae_f64     q0, q1;
  ae_int64   u0;

  int n;

  ASSERT( r && x && dline && coef );

  ASSERT( IS_ALIGN( dline ) && IS_ALIGN( coef ) );

  ASSERT( M == 5 );

  //
  // Setup pointers.
  //

  R = (      ae_int32   *)r;
  X = (const ae_int32   *)x;
  D = (      ae_f32x2   *)dline;
  C = (const ae_int16x4 *)coef;

  //
  // Load the prescaling factor.
  //

  // Q15
  s_g = AE_MOVDA16( (unsigned)scale );

  //
  // Pre-load the reflection coefficients and delay line elements.
  //

  // Q15
  s0 = AE_L16X4_I( C, +0 ); // k4 k3 k2 k1
  s1 = AE_L16X4_I( C, +8 ); // k0  0  0  0

  // Make a skew coefficient quadruple.
  s2 = AE_SEL16_6543( s0, s1 ); // k3 k2 k1 k0

  cf_4321 = ( s0 );
  cf_0zzz = ( s1 );
  cf_3210 = ( s2 );

  // Load delay line elements d4..d0.
  // Q1.30
  dl_43 = ae_f32x2_loadi( D,  +0 );
  dl_21 = ae_f32x2_loadi( D,  +8 );
  dl_0z = ae_f32x2_loadi( D, +16 );

  //
  // Pass the input samples block through the AR lattice. n-th response sample
  // and lattice state update are defined as follows:
  //
  //   p3 = g*x[n] - k4*d4 - k3*d3
  //   p2 = p3 - k2*d2; p1 = p2 - k1*d1; p0 = p1 - k0*d0;
  //
  //   d4 = d3 + k3*p3; d3 = d2 + k2*p2;
  //   d2 = d1 + k1*p1; d1 = d0 + k0*p0;
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

    // Q31
    AE_L32_IP( t0, X, +4 );

    // q0 = g*x[n]
    // Q17.46 <- Q15*Q31
    u0 = AE_MUL32X16_L0( t0, s_g );

    q0 = ( u0 );

    //--------------------------------------------------------------------------
    // Update delay elements d4, d3.

    // q0 -= k4*d4 + k3*d3
    // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ] + [ Q15*Q1.30 + 1 ]
    AE_MULSSFD32X16_H3_L2( q0, dl_43, cf_4321 );

    q1 = q0;

    // q1 -= k2*d2
    // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ]
    AE_MULSF32X16_H1( q1, dl_21, cf_4321 );

    // p3 = q0; p2 = q1
    // Q1.30 <- Q17.46 - 16 w/ rounding and saturation
    AE_PKSR32( p0, q0, 0 );
    AE_PKSR32( p0, q1, 0 );

    // d3 d2
    select_f32x2_lh( dl_43, dl_43, dl_21 );

    // d4 = d3 + k3*p3; d3 = d2 + k2*p2
    // Q1.30 <- Q1.30 + [ Q15*Q1.30 - 15 ] w/ rounding and saturation
    AE_MULAFP32X16X2RAS_H( dl_43, p0, cf_3210 );

    //--------------------------------------------------------------------------
    // Update delay elements d2, d1.

    // q0 -= k2*d2 + k1*d1
    // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ] + [ Q15*Q1.30 + 1 ]
    AE_MULSSFD32X16_H1_L0( q0, dl_21, cf_4321 );

    // d1 d0
    select_f32x2_lh( dl_21, dl_21, dl_0z );

    // q1 -= k1*d1 + k0*d0
    // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ] + [ Q15*Q1.30 + 1 ]
    AE_MULSSFD32X16_H1_L0( q1, dl_21, cf_3210 );

    // p1 = q0; p0 = q1
    // Q1.30 <- Q17.46 - 16 w/ rounding and saturation
    AE_PKSR32( p1, q0, 0 );
    AE_PKSR32( p1, q1, 0 );

    // d2 = d1 + k1*p1; d1 = d0 + k0*p0
    // Q1.30 <- Q1.30 + [ Q15*Q1.30 - 15 ] w/ rounding and saturation
    AE_MULAFP32X16X2RAS_L( dl_21, p1, cf_3210 );

    //--------------------------------------------------------------------------
    // Update delay element d0, format and store response sample.

    // d0 = p0; 0
    select_f32x2_ll( dl_0z, p1, dl_0z );

    // r[n] = q1
    // Q31 <- Q17.46 + 1 - 16 w/ rounding and saturation
    AE_PKSR32( p2, q1, 1 );

    t1 = ( p2 );

    // Q31
    AE_S32_L_IP( t1, R, +4 );
  }

  //
  // Store updated delay line elements.
  //

  // Q1.30
  ae_f32x2_storei( dl_43, D,  +0 );
  ae_f32x2_storei( dl_21, D,  +8 );
  ae_f32x2_storei( dl_0z, D, +16 );

} // latr5_proc()

/* Order 6 lattice filter data processing function. */
static void latr6_proc( int32_t * restrict r,     // r[N]     [out   ] Q31
                  const int32_t * restrict x,     // x[N]     [in    ] Q31
                        int32_t *          dline, // dline[M] [in/out] Q30
                  const int16_t *          coef,  // coef[M]  [in    ] Q15
                  int16_t scale, int N, int M )   // scale    [in    ] Q15
{
        ae_int32   * restrict R;
  const ae_int32   * restrict X;
        ae_f32x2   *          D;
  const ae_int16x4 *          C;

  ae_f32x2   dl_54, dl_32, dl_10;
  ae_f32x2   dl_43, dl_21;
  ae_f16x4   cf_5432, cf_10zz;
  ae_f16x4   cf_4321;
  ae_int16x4 s0, s1, s2, s_g;
  ae_f32x2   p0, p1;
  ae_int32x2 t0, t1;
  ae_f64     q0, q1;
  ae_int64   u0;

  int n;

  ASSERT( r && x && dline && coef );

  ASSERT( IS_ALIGN( dline ) && IS_ALIGN( coef ) );

  ASSERT( M == 6 );

  //
  // Setup pointers.
  //

  R = (      ae_int32   *)r;
  X = (const ae_int32   *)x;
  D = (      ae_f32x2   *)dline;
  C = (const ae_int16x4 *)coef;

  //
  // Load the prescaling factor.
  //

  // Q15
  s_g = AE_MOVDA16( (unsigned)scale );

  //
  // Pre-load the reflection coefficients and delay line elements.
  //

  // Q15
  s0 = AE_L16X4_I( C, +0 ); // k5 k4 k3 k2
  s1 = AE_L16X4_I( C, +8 ); // k1 k0  0  0

  // Make a skew coefficient quadruple.
  s2 = AE_SEL16_6543( s0, s1 ); // k4 k3 k2 k1

  cf_5432 = ( s0 );
  cf_10zz = ( s1 );
  cf_4321 = ( s2 );

  // Load delay line elements d5..d0.
  // Q1.30
  dl_54 = ae_f32x2_loadi( D,  +0 );
  dl_32 = ae_f32x2_loadi( D,  +8 );
  dl_10 = ae_f32x2_loadi( D, +16 );

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

    // Q31
    AE_L32_IP( t0, X, +4 );

    // q0 = g*x[n]
    // Q17.46 <- Q15*Q31
    u0 = AE_MUL32X16_L0( t0, s_g );

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
    AE_PKSR32( p1, q0, 1 );

    t1 = ( p1 );

    // Q31
    AE_S32_L_IP( t1, R, +4 );
  }

  //
  // Store updated delay line elements.
  //

  // Q1.30
  ae_f32x2_storei( dl_54, D,  +0 );
  ae_f32x2_storei( dl_32, D,  +8 );
  ae_f32x2_storei( dl_10, D, +16 );

} // latr6_proc()

/* Order 7 lattice filter data processing function. */
static void latr7_proc( int32_t * restrict r,     // r[N]     [out   ] Q31
                  const int32_t * restrict x,     // x[N]     [in    ] Q31
                        int32_t *          dline, // dline[M] [in/out] Q30
                  const int16_t *          coef,  // coef[M]  [in    ] Q15
                  int16_t scale, int N, int M )   // scale    [in    ] Q15
{
        ae_int32   * restrict R;
  const ae_int32   * restrict X;
        ae_f32x2   *          D;
  const ae_int16x4 *          C;

  ae_f32x2   dl_65, dl_43, dl_21, dl_0z;
  ae_f32x2   dl_54, dl_32, dl_10;
  ae_f16x4   cf_6543, cf_210z;
  ae_f16x4   cf_5432, cf_10z6;
  ae_int16x4 s0, s1, s2, s3, s_g;
  ae_f32x2   p0, p1;
  ae_int32x2 t0, t1;
  ae_f64     q0, q1;
  ae_int64   u0;

  int n;

  ASSERT( r && x && dline && coef );

  ASSERT( IS_ALIGN( dline ) && IS_ALIGN( coef ) );

  ASSERT( M == 7 );

  //
  // Setup pointers.
  //

  R = (      ae_int32   *)r;
  X = (const ae_int32   *)x;
  D = (      ae_f32x2   *)dline;
  C = (const ae_int16x4 *)coef;

  //
  // Load the prescaling factor.
  //

  // Q15
  s_g = AE_MOVDA16( (unsigned)scale );

  //
  // Pre-load the reflection coefficients and delay line elements.
  //

  // Q15
  s0 = AE_L16X4_I( C, +0 ); // k6 k5 k4 k3
  s1 = AE_L16X4_I( C, +8 ); // k2 k1 k0  0

  // Make skew coefficient quadruples.
  s2 = AE_SEL16_6543( s0, s1 ); // k5 k4 k3 k2
  s3 = AE_SEL16_6543( s1, s0 ); // k1 k0  0 k6

  cf_6543 = ( s0 );
  cf_210z = ( s1 );
  cf_5432 = ( s2 );
  cf_10z6 = ( s3 );

  // Load delay line elements d6..d0.
  // Q1.30
  dl_65 = ae_f32x2_loadi( D,  +0 );
  dl_43 = ae_f32x2_loadi( D,  +8 );
  dl_21 = ae_f32x2_loadi( D, +16 );
  dl_0z = ae_f32x2_loadi( D, +24 );

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

    // Q31
    AE_L32_IP( t0, X, +4 );

    // q0 = g*x[n]
    // Q17.46 <- Q15*Q31
    u0 = AE_MUL32X16_L0( t0, s_g );

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
    AE_PKSR32( p1, q1, 1 );

    t1 = ( p1 );

    // Q31
    AE_S32_L_IP( t1, R, +4 );
  }

  //
  // Store updated delay line elements.
  //

  // Q1.30
  ae_f32x2_storei( dl_65, D,  +0 );
  ae_f32x2_storei( dl_43, D,  +8 );
  ae_f32x2_storei( dl_21, D, +16 );
  ae_f32x2_storei( dl_0z, D, +24 );

} // latr7_proc()

/* Order 8 lattice filter data processing function. */
static void latr8_proc( int32_t * restrict r,     // r[N]     [out   ] Q31
                  const int32_t * restrict x,     // x[N]     [in    ] Q31
                        int32_t *          dline, // dline[M] [in/out] Q1.30
                  const int16_t *          coef,  // coef[M]  [in    ] Q15
                  int16_t scale, int N, int M )   // scale    [in    ] Q15
{
        ae_int32   * restrict R;
  const ae_int32   * restrict X;
        ae_f32x2   *          D;
  const ae_int16x4 *          C;

  ae_f32x2   dl_76, dl_54, dl_32, dl_10;
  ae_f32x2   dl_65, dl_43, dl_21;
  ae_f16x4   cf_7654, cf_3210;
  ae_f16x4   cf_6543, cf_2107;
  ae_int16x4 s0, s1, s2, s3, s_g;
  ae_f32x2   p0, p1;
  ae_int32x2 t0, t1;
  ae_f64     q0, q1;
  ae_int64   u0;

  int n;

  ASSERT( r && x && dline && coef );

  ASSERT( IS_ALIGN( dline ) && IS_ALIGN( coef ) );

  ASSERT( M == 8 );

  //
  // Setup pointers.
  //

  R = (      ae_int32   *)r;
  X = (const ae_int32   *)x;
  D = (      ae_f32x2   *)dline;
  C = (const ae_int16x4 *)coef;

  //
  // Load the prescaling factor.
  //

  // Q15
  s_g = AE_MOVDA16( (unsigned)scale );

  //
  // Pre-load the reflection coefficients and delay line elements.
  //

  // Q15
  s0 = AE_L16X4_I( C, +0 ); // k7 k6 k5 k4
  s1 = AE_L16X4_I( C, +8 ); // k3 k2 k1 k0

  // Make skew coefficient quadruples.
  s2 = AE_SEL16_6543( s0, s1 ); // k6 k5 k4 k3
  s3 = AE_SEL16_6543( s1, s0 ); // k2 k1 k0 k7

  cf_7654 = ( s0 );
  cf_3210 = ( s1 );
  cf_6543 = ( s2 );
  cf_2107 = ( s3 );

  // Load delay line elements d7..d0.
  // Q1.30
  dl_76 = ae_f32x2_loadi( D,  +0 );
  dl_54 = ae_f32x2_loadi( D,  +8 );
  dl_32 = ae_f32x2_loadi( D, +16 );
  dl_10 = ae_f32x2_loadi( D, +24 );

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

    // Q31
    AE_L32_IP( t0, X, +4 );

    // q0 = g*x[n]
    // Q17.46 <- Q15*Q31
    u0 = AE_MUL32X16_L0( t0, s_g );

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
    AE_PKSR32( p1, q1, 1 );

    t1 = ( p1 );

    // Q31
    AE_S32_L_IP( t1, R, +4 );
  }

  //
  // Store updated delay line elements.
  //

  // Q1.30
  ae_f32x2_storei( dl_76, D,  +0 );
  ae_f32x2_storei( dl_54, D,  +8 );
  ae_f32x2_storei( dl_32, D, +16 );
  ae_f32x2_storei( dl_10, D, +24 );

} // latr8_proc()

/* Data processing function for a lattice filter of arbitrary order. */
static void latrX_proc( int32_t * restrict r,     // r[N]     [out   ] Q31
                  const int32_t * restrict x,     // x[N]     [in    ] Q31
                        int32_t *          dline, // dline[M] [in/out] Q1.30
                  const int16_t *          coef,  // coef[M]  [in    ] Q15
                  int16_t scale, int N, int M )   // scale    [in    ] Q15
{
        ae_f32     * restrict R;
  const ae_int32   *          X;
  const ae_int32x2 *          D_rd;
        ae_int32x2 * restrict D_wr;
  const ae_int32   *          C;

  ae_f32x2   dl_32, dl_10, dl_21;
  ae_f16x4   cf_3232, cf_1010, cf_2101;
  ae_f64     q0, q1, q2;
  ae_int64   u0;
  ae_f32x2   p0, p1;
  ae_int32x2 t0, t1, t2;
  ae_int16x4 s0, s1, s_g;

  int n, m;

  ASSERT( r && x && dline && coef );

  ASSERT( IS_ALIGN( dline ) && IS_ALIGN( coef ) );

  ASSERT( M > 1 );

  //
  // Setup pointers.
  //

  R = (      ae_f32   *)r;
  X = (const ae_int32 *)x;

  //
  // Load the prescaling factor.
  //

  // Q15
  s_g = AE_MOVDA16( (unsigned)scale );

  //
  // Pass the input samples block through the AR lattice. n-th response sample
  // and lattice state update are defined as follows:
  //
  //   for ( n=0; n<N; n++ )
  //   {
  //     q = g*x[n] - k[M-1]*d[M-1];
  //
  //     for ( m=M-2; m>=0; m-- )
  //     {
  //       q -= k[m]*d[m];
  //       d[m+1] = d[m] + k[m]*q;
  //     }
  //   }
  //
  //   d[0] = r[n] = q;
  //
  // After unrolling the inner loop (x2):
  //
  //   A) Odd filter order
  //
  //   for ( n=0; n<N; n++ )
  //   {
  //     q0 = g*x[n];
  //     q1 = g*x[n] - k[M-1]*d[M-1];
  //
  //     for ( m=M-3; m>=0; m-=2 )
  //     {
  //       q0 -= k[m+2]*d[m+2] + k[m+1]*d[m+1];
  //       q1 -= k[m+1]*d[m+1] + k[m+0]*d[m+0];
  //
  //       d[m+2] = d[m+1] + k[m+1]*q0;
  //       d[m+1] = d[m+0] + k[m+0]*q1;
  //     }
  //
  //     d[0] = r[n] = q1;
  //   }
  //
  //
  //   B) Even filter order
  //
  //   for ( n=0; n<N; n++ )
  //   {
  //     q0 = g*x[n];
  //     q1 = g*x[n] - k[M-1]*d[M-1];
  //
  //     for ( m=M-3; m>=0; m-=2 )
  //     {
  //       q0 -= k[m+2]*d[m+2] + k[m+1]*d[m+1];
  //       q1 -= k[m+1]*d[m+1] + k[m+0]*d[m+0];
  //
  //       d[m+2] = d[m+1] + k[m+1]*q0;
  //       d[m+1] = d[m+0] + k[m+0]*q1;
  //     }
  //
  //     q0 -= k[1]*d[1] + k[0]*d[0];
  //
  //     d[0] = r[n] = q0;
  //   }
  //
  //
  // Mapping of variable suffixes onto coefficient/delay indices:
  //   cf_/dl_ suffix:  3   2   1   0
  //   k[]/d[] index : m+2 m+1 m+0 m-1
  //
  // Coefficients and delay line elements are stored in reverted order:
  //   dline[M] = { d[M-1], d[M-2], ..., d[0] };
  //   coef [M] = { k[M-1], k[M-2], ..., k[0] };
  //


  if ( M & 1 )
  {
    for ( n=0; n<N; n++ )
    {
      D_rd = (const ae_int32x2 *)dline;
      D_wr = (      ae_int32x2 *)dline;
      C    = (const ae_int32   *)coef;

      // Q31
      AE_L32_IP( t0, X, +4 );

      // q0 = g*x[n], q1 = g*x[n]
      // Q17.46 <- Q31*Q15
      u0 = AE_MUL32X16_L0( t0, s_g );

      q0 = ( u0 );
      q1 = ( u0 );

      cf_1010 = ae_f16x4_loadi( (const ae_f16x4*)C, 0 );

      // Load d[M-1], d[M-2]; Q1.30
      AE_L32X2_IP( t1, D_rd, +8 );
      // Load k[M-1], k[M-2]; Q15
      AE_L32_IP( t2, C, +4 );

      dl_32   = ( t1 );
      cf_3232 = AE_MOVF16X4_FROMINT32X2( t2 );

      // q1 -= k[M-1]*d[M-1]
      // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ]
      AE_MULSF32X16_H3( q1, dl_32, cf_3232 );

      for ( m=M-3; m>=0; m-=2 )
      {
        // Load d[m], d[m-1]; Q1.30
        AE_L32X2_IP( t0, D_rd, +8 );

        dl_10 = ( t0 );

        // d[m+1] d[m]
        select_f32x2_lh( dl_21, dl_32, dl_10 );

        // Load k[m], k[m-1]; Q15
        AE_L32_IP( t1, C, +4 );

        cf_1010 = AE_MOVF16X4_FROMINT32X2( t1 );

        s0 = ( cf_3232 );
        s1 = ( cf_1010 );

        // Make skew coefficients quadruple: k[m+1] k[m] k[m-1] k[m].
        s0 = AE_SEL16_4321( s0, s1 );

        cf_2101 = ( s0 );

        // q0 -= k[m+2]*d[m+2] + k[m+1]*d[m+1]
        // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ] + [ Q15*Q1.30 + 1 ]
        AE_MULSSFD32X16_H1_L0( q0, dl_32, cf_3232 );

        // q1 -= k[m+1]*d[m+1] + k[m]*d[m]
        // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ] + [ Q15*Q1.30 + 1 ]
        AE_MULSSFD32X16_H3_L2( q1, dl_21, cf_2101 );

        // q0 q1
        // Q1.30 <- Q17.46 - 16 w/ rounding and saturation
        AE_PKSR32( p0, q0, 0 );
        AE_PKSR32( p0, q1, 0 );

        // d[m+2] = d[m+1] + k[m+1]*q0; d[m+1] = d[m] + k[m]*q1
        // Q1.30 <- Q1.30 + [ Q15*Q1.30 - 15 ] w/ rounding and saturation
        AE_MULAFP32X16X2RAS_H( dl_21, p0, cf_2101 );

        t2 = ( dl_21 );

        // Save updated d[m+2], d[m+1]
        AE_S32X2_IP( t2, D_wr, +8 );

        dl_32   = dl_10;
        cf_3232 = cf_1010;
      }

      t0 = ( p0 );

      // d0 = q1; Q1.30
      AE_S32_L_I( t0, (ae_int32*)D_wr, +0 );

      // Q16.47 <- Q17.46 + 1
      q2 = AE_F64_SLAIS( q1, 1 );

      // r[n] = q1
      // Q31 <- Q16.47 - 16
      AE_S32RA64S_IP( q2, R, +4 );

    #ifdef COMPILER_XTENSA
      #pragma flush_memory
    #endif
    }
  }
  else
  {
    for ( n=0; n<N; n++ )
    {
      D_rd = (const ae_int32x2 *)dline;
      D_wr = (      ae_int32x2 *)dline;
      C    = (const ae_int32   *)coef;

      // Q31
      AE_L32_IP( t0, X, +4 );

      // q0 = g*x[n], q1 = g*x[n]
      // Q17.46 <- Q31*Q15
      u0 = AE_MUL32X16_L0( t0, s_g );

      q0 = ( u0 );
      q1 = ( u0 );

      cf_1010 = ae_f16x4_loadi( (const ae_f16x4*)C, 0 );

      // Load d[M-1], d[M-2]; Q1.30
      AE_L32X2_IP( t1, D_rd, +8 );
      // Load k[M-1], k[M-2]; Q15
      AE_L32_IP( t2, C, +4 );

      dl_32   = ( t1 );
      cf_3232 = AE_MOVF16X4_FROMINT32X2( t2 );

      // q1 -= k[M-1]*d[M-1]
      // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ]
      AE_MULSF32X16_H3( q1, dl_32, cf_3232 );

      for ( m=M-3; m>=0; m-=2 )
      {
        // Load d[m], d[m-1]; Q1.30
        AE_L32X2_IP( t0, D_rd, +8 );

        dl_10 = ( t0 );

        // d[m+1] d[m]
        select_f32x2_lh( dl_21, dl_32, dl_10 );

        // Load k[m], k[m-1]; Q15
        AE_L32_IP( t1, C, +4 );

        cf_1010 = AE_MOVF16X4_FROMINT32X2( t1 );

        s0 = ( cf_3232 );
        s1 = ( cf_1010 );

        // Make skew coefficients quadruple: k[m+1] k[m] k[m-1] k[m].
        s0 = AE_SEL16_4321( s0, s1 );

        cf_2101 = ( s0 );

        // q0 -= k[m+2]*d[m+2] + k[m+1]*d[m+1]
        // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ] + [ Q15*Q1.30 + 1 ]
        AE_MULSSFD32X16_H1_L0( q0, dl_32, cf_3232 );

        // q1 -= k[m+1]*d[m+1] + k[m]*d[m]
        // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ] + [ Q15*Q1.30 + 1 ]
        AE_MULSSFD32X16_H3_L2( q1, dl_21, cf_2101 );

        // Q1.30 <- Q17.46 - 16 w/ rounding and saturation
        AE_PKSR32( p0, q0, 0 );
        AE_PKSR32( p0, q1, 0 );

        // d[m+2] = d[m+1] + k[m+1]*q0; d[m+1] = d[m] + k[m]*q1
        // Q1.30 <- Q1.30 + [ Q15*Q1.30 - 15 ] w/ rounding and saturation
        AE_MULAFP32X16X2RAS_H( dl_21, p0, cf_2101 );

        t2 = ( dl_21 );

        // Save updated d[m+2], d[m+1]
        AE_S32X2_IP( t2, D_wr, +8 );

        dl_32   = dl_10;
        cf_3232 = cf_1010;
      }

      // Reload d[1], d[0]; Q1.30
      t0 = AE_L32X2_I( D_rd, -8 );
      // Reload k[1], k[0]; Q15
      t1 = AE_L32_I( C, -4 );

      dl_10   = ( t0 );
      cf_1010 = AE_MOVF16X4_FROMINT32X2( t1 );

      // q0 -= k[1]*d[1] + k[0]*d[0]
      // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ] + [ Q15*Q1.30 + 1 ]
      AE_MULSSFD32X16_H1_L0( q0, dl_10, cf_1010 );

      // <don't care> q0
      // Q1.30 <- Q17.46 - 16 w/ rounding and saturation
      AE_PKSR32( p1, q0, 0 );

      // <don't care>; d[1] = d[0] + k[0]*q0
      // Q1.30 <- Q1.30 + [ Q15*Q1.30 - 15 ] w/ rounding and saturation
      AE_MULAFP32X16X2RAS_H( dl_10, p1, cf_1010 );

      // d[1]; d[0] = q0
      select_f32x2_ll( dl_10, dl_10, p1 );

      t2 = ( dl_10 );

      // Save updated d[1], d[0]
      AE_S32X2_I( t2, D_wr, +0 );

      // Q16.47 <- Q17.46 + 1
      q2 = AE_F64_SLAIS( q0, 1 );

      // r[n] = q0
      // Q31 <- Q16.47 - 16
      AE_S32RA64S_IP( q2, R, +4 );

    #ifdef COMPILER_XTENSA
      #pragma flush_memory
    #endif
    }
  }

} // latrX_proc()
