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
    Lattice Block Real IIR, 24x24-bit
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
#define MAGIC     0xd4bdbae2

/* Reserve memory for alignment. */
#define ALIGNED_SIZE( size, align ) \
      ( (size_t)(size) + (align) - 1 )

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
      (void*)( ( (uintptr_t)(addr) + ( (align) - 1 ) ) & ~( (align) - 1 ) )

#define sz_f24 sizeof(f24)

/* AE_SELP24_LL adapted for ae_f24x2 data type. */
#define select_f24x2_ll( s_out, s_in0, s_in1 ) \
  { \
    ae_int24x2 t0 = ( s_in0 ); \
    ae_int24x2 t1 = ( s_in1 ); \
    ae_int24x2 t2; \
    \
    t2 = AE_SELP24_LL( t0, t1 ); \
    \
    s_out = ( t2 ); \
  }

/* AE_SELP24_LH adapted for ae_f24x2 data type. */
#define select_f24x2_lh( s_out, s_in0, s_in1 ) \
  { \
    ae_int24x2 t0 = ( s_in0 ); \
    ae_int24x2 t1 = ( s_in1 ); \
    ae_int24x2 t2; \
    \
    t2 = AE_SELP24_LH( t0, t1 ); \
    \
    s_out = ( t2 ); \
  }

/* AE_SELP24_HL adapted for ae_f24x2 data type. */
#define select_f24x2_hl( s_out, s_in0, s_in1 ) \
  { \
    ae_int24x2 t0 = ( s_in0 ); \
    ae_int24x2 t1 = ( s_in1 ); \
    ae_int24x2 t2; \
    \
    t2 = AE_SELP24_HL( t0, t1 ); \
    \
    s_out = ( t2 ); \
  }

/* AE_SELP24_HH adapted for ae_f24x2 data type. */
#define select_f24x2_hh( s_out, s_in0, s_in1 ) \
  { \
    ae_int24x2 t0 = ( s_in0 ); \
    ae_int24x2 t1 = ( s_in1 ); \
    ae_int24x2 t2; \
    \
    t2 = AE_SELP24_HH( t0, t1 ); \
    \
    s_out = ( t2 ); \
  }

/* Lattice filter data processing function. */
typedef void (proc_fxn_t)( f24 * restrict r,     // r[N]     [out   ] Q(23+8)
                     const f24 * restrict x,     // x[N]     [in    ] Q(23+8)
                           f24 *          dline, // dline[M] [in/out] Q(1.22+8)
                     const f24 *          coef,  // coef[M]  [in    ] Q(23+8)
                     f24 scale, int N, int M );  // scale    [in    ] Q(23+8)

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
typedef struct tag_latr24x24_t
{
  uint32_t     magic;     // Instance pointer validation number
  int          M;         // Lattice filter order
  proc_fxn_t * procFxn;   // Custom data processing function
  f24          scale;     // Input signal prescaling factor, Q(23+8)
  const f24 *  coef;      // M reflection coefficients, Q(23+8)
  f24 *        delayLine; // M delay elements, Q(1.22+8)

} latr24x24_t, *latr24x24_ptr_t;

// Determine the memory area size for a filter instance.
size_t latr24x24_alloc(int M )
{
  int _M;

  ASSERT( M > 0 );

  // Complement M to the next even number.
  _M = M + ( -M & 1 );

  return ( ALIGNED_SIZE( sizeof( latr24x24_t ), 4 )
           + // M delay elements
           ALIGNED_SIZE( _M*sz_f24, 8 )
           + // M reflection coefficients
           ALIGNED_SIZE( _M*sz_f24, 8 ) );

} // latr24x24_alloc()

// Given a memory area of sufficient size, initialize a filter instance and 
// return a handle to it.
latr24x24_handle_t latr24x24_init( void *         objmem, 
                                   int            M,
                             const f24 * restrict k,
                                   f24            scale )
{
  latr24x24_ptr_t latr;

  proc_fxn_t * procFxn;

  f24  * delayLine;
  f24  * coef;
  void * ptr;

  int _M, m;

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
  // Partition the memory block.
  //

  // Complement M to the next even number.
  _M = M + ( -M & 1 );

  ptr       = objmem;
  latr      = (latr24x24_ptr_t)(ALIGNED_ADDR( ptr, 4 ));
  ptr       = latr + 1;
  delayLine = (f24*)(ALIGNED_ADDR( ptr, 8 ));
  ptr       = delayLine + _M;
  coef      = (f24*)(ALIGNED_ADDR( ptr, 8 ));
  ptr       = coef + _M;

  ASSERT( (int8_t*)ptr - (int8_t*)objmem <= (int)latr24x24_alloc( M ) );

  //
  // Copy reflection coefficients, zero the delay line.
  //

  for ( m=0; m<M; m++ )
  {
    coef[m] = k[M-1-m];

    delayLine[m] = 0;
  }

  for ( ; m<_M; m++ )
  {
    coef[m] = delayLine[m] = 0;
  }

  //
  // Initialize the filter instance.
  //

  latr->magic     = MAGIC;
  latr->M         = M;
  latr->procFxn   = procFxn;
  latr->scale     = scale;
  latr->coef      = coef;
  latr->delayLine = delayLine;

  return (latr);

} // latr24x24_init()

// Process data. The filter instance is identified with a handle returned by
// the initializing function.
void latr24x24_process( latr24x24_handle_t _latr, 
                        f24 * restrict     r,
                  const f24 *              x, int N )
{
  latr24x24_ptr_t latr = (latr24x24_ptr_t)_latr;

  ASSERT( latr && latr->magic == MAGIC && r && x );

  ASSERT( latr->procFxn );

  ( *latr->procFxn )( r, x,
                      latr->delayLine,
                      latr->coef,
                      latr->scale,
                      N,
                      latr->M );

} // latr24x24_process()

/* Order 1 lattice filter data processing function. */
static void latr1_proc( f24 * restrict r,     // r[N]     [out   ] Q(23+8)
                  const f24 * restrict x,     // x[N]     [in    ] Q(23+8)
                        f24 *          dline, // dline[M] [in/out] Q(1.22+8)
                  const f24 *          coef,  // coef[M]  [in    ] Q(23+8)
                  f24 scale, int N, int M )   // scale    [in    ] Q(23+8)
{
  const ae_f24   * restrict X;
        ae_f24   * restrict R;
  const ae_f24x2 *          C;
        ae_f24x2 *          D;

  ae_f24x2   cf_k0z;
  ae_f24x2   dl_d0z, dl_d0d0;
  ae_f24x2   s0;
  ae_int24x2 t0, t_zg;
  ae_f64     q0, q1;
  ae_int64   u0;

  int n;

  ASSERT( r && x && dline && coef );

  ASSERT( M == 1 );

  ASSERT( IS_ALIGN( dline ) && IS_ALIGN( coef ) );

  //
  // Setup pointers.
  //

  X = (const ae_f24   *)x;
  R = (      ae_f24   *)r;
  C = (const ae_f24x2 *)coef;
  D = (      ae_f24x2 *)dline;

  //
  // Load the prescaling factor and set zero registers.
  //

  // Q23 <- Q(23+8) - 8
  t_zg = AE_MOVPA24X2( 0, (unsigned)( scale >> 8 ) );

  //
  // Pre-load the reflection coefficient and delay line element.
  //

  // Load reflection coefficient.
  // Q23 <- Q(23+8) - 8
  cf_k0z  = AE_L32X2F24_I( C, +0 );

  // Load state element d0.
  // Q1.22 <- Q(1.22+8) - 8
  dl_d0z  = AE_L32X2F24_I( D, +0 );

  // 0 d0
  select_f24x2_hh( dl_d0d0, dl_d0z, dl_d0z );

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
    // Q23 <- Q(23+8) - 8
    AE_L32F24_IP( s0, X, +4 );

    t0 = ( s0 );

    // Prescaling; q0 = g*x[n]
    // Q17.46 <- Q23*Q23
    u0 = AE_MULZAAD24_HH_LL( t_zg, t0 );

    q0 = ( u0 );

    // q0 -= k0*d0
    // Q17.46 <- Q17.46 + [ Q23*Q1.22 + 1 ]
    AE_MULSSFD24_HH_LL( q0, cf_k0z, dl_d0d0 );

    // d0 = q0
    // Q1.22 <- Q17.46 - 24 w/ rounding and saturation
    dl_d0d0 = AE_ROUNDSP24Q48ASYM( q0 );

    // Q16.47 <- Q17.46 + 1
    q1 = AE_F64_SLAIS( q0, 1 );

    // r[n] = q0
    // Q(23+8) <- [ Q16.47 - 24 w/ rounding and saturation ] + 8
    AE_S24RA64S_IP( q1, R, +4 );
  }

  //
  // Store updated delay line element.
  //

  // d0 0
  select_f24x2_ll( dl_d0z, dl_d0d0, cf_k0z );

  // Q(1.22+8) <- Q1.22 + 8
  AE_S32X2F24_I( dl_d0z , D, +0 );

} // latr1_proc()

/* Order 2 lattice filter data processing function. */
static void latr2_proc( f24 * restrict r,     // r[N]     [out   ] Q(23+8)
                  const f24 * restrict x,     // x[N]     [in    ] Q(23+8)
                        f24 *          dline, // dline[M] [in/out] Q(1.22+8)
                  const f24 *          coef,  // coef[M]  [in    ] Q(23+8)
                  f24 scale, int N, int M )   // scale    [in    ] Q(23+8)
{
  const ae_f24   * restrict X;
        ae_f24   * restrict R;
  const ae_f24x2 *          C;
        ae_f24x2 *          D;

  ae_f24x2   cf_k1k0, cf_zk1, cf_zk2;
  ae_f24x2   dl_d1d0;
  ae_f24x2   s0, s_zz;
  ae_int24x2 t0, t_zg;
  ae_f32x2   r0;
  ae_f64     q0;
  ae_int64   u0;

  int n;

  ASSERT( r && x && dline && coef );

  ASSERT( M == 2 );

  ASSERT( IS_ALIGN( dline ) && IS_ALIGN( coef ) );

  //
  // Setup pointers.
  //

  X = (const ae_f24   *)x;
  R = (      ae_f24   *)r;
  C = (const ae_f24x2 *)coef;
  D = (      ae_f24x2 *)dline;

  //
  // Load the prescaling factor and set zero registers.
  //

  // t_zg = 0 g
  // Q23 <- Q(23+8) - 8
  t_zg = AE_MOVPA24X2( 0, (unsigned)( scale >> 8 ) );

  s_zz = 0;

  //
  // Pre-load all reflection coefficients and delay line elements.
  //

  // Load reflection coefficients.
  // Q23 <- Q(23+8) - 8
  cf_k1k0 = AE_L32X2F24_I( C, +0 );

  // 0 k1
  select_f24x2_hh( cf_zk1, s_zz, cf_k1k0 );

  // 0 k0*k1
  // Q8.23 <- Q23*Q23 - 23 w/ rounding
  r0 = AE_MULFP24X2RA( cf_k1k0, cf_zk1 );


  // Q23 <- sat24( Q8.23 )
  cf_zk2 = AE_ADD24S( r0, s_zz );

  // 0 k2=k0*k1

  // Load state elements d1,d0.
  // Q1.22 <- Q(1.22+8) - 8
  dl_d1d0 = AE_L32X2F24_I( D, +0 );

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
  // is faster and provides the same performance in terms of the round-off
  // error.
  //

  for ( n=0; n<N; n++ )
  {
    // Q23 <- Q(23+8) - 8
    AE_L32F24_IP( s0, X, +4 );

    t0 = ( s0 );

    // Prescaling; q0 = g*x[n]
    // Q17.46 <- Q23*Q23
    u0 = AE_MULZAAD24_HH_LL( t_zg, t0 );

    q0 = ( u0 );

    // q0 -= k1*d1 + k0*d0
    // Q17.46 <- Q17.46 + [ Q23*Q1.22 + 1 ] + [ Q23*Q1.22 + 1 ]
    AE_MULSSFD24_HH_LL( q0, cf_k1k0, dl_d1d0 );

    // q0 -= k0*k1*d0
    // Q17.46 <- Q17.46 + [ Q23*Q1.22 + 1 ]
    AE_MULSSFD24_HH_LL( q0, cf_zk2,  dl_d1d0 );

    // d1 = d0; d0 = q0
    // Q1.22 <- Q17.46 - 24 w/ rounding and saturation
    AE_PKSR24( dl_d1d0, q0, 0 );

    // Q16.47 <- Q17.46 + 1
    q0 = AE_F64_SLAIS( q0, 1 );

    // r[n] = p0
    // Q(23+8) <- [ Q16.47 - 24 w/ rounding and saturation ] + 8
    AE_S24RA64S_IP( q0, R, +4 );
  }

  //
  // Store updated delay line elements.
  //

  // Q(1.22+8) <- Q1.22 + 8
  AE_S32X2F24_I( dl_d1d0, D, +0 );

} // latr2_proc()

/* Order 3 lattice filter data processing function. */
static void latr3_proc( f24 * restrict r,     // r[N]     [out   ] Q(23+8)
                  const f24 * restrict x,     // x[N]     [in    ] Q(23+8)
                        f24 *          dline, // dline[M] [in/out] Q(1.22+8)
                  const f24 *          coef,  // coef[M]  [in    ] Q(23+8)
                  f24 scale, int N, int M )   // scale    [in    ] Q(23+8)
{
  const ae_f24   * restrict X;
        ae_f24   * restrict R;
  const ae_f24x2 *          C;
        ae_f24x2 *          D;

  ae_f24x2   cf_k2k1, cf_k1k2, cf_k1k0, cf_k0z;
  ae_f24x2   dl_d2d1, dl_d0z, dl_d1d0;
  ae_f32x2   r0;
  ae_f24x2   s0, s1;
  ae_int24x2 t0, t_zg;
  ae_f64     q0, q1, q2;
  ae_int64   u0, u1;

  int n;

  ASSERT( r && x && dline && coef );

  ASSERT( M == 3 );

  ASSERT( IS_ALIGN( dline ) && IS_ALIGN( coef ) );

  //
  // Setup pointers.
  //

  X = (const ae_f24   *)x;
  R = (      ae_f24   *)r;
  C = (const ae_f24x2 *)coef;
  D = (      ae_f24x2 *)dline;

  //
  // Load the prescaling factor.
  //

  // Q23 <- Q(23+8) - 8
  t_zg = AE_MOVPA24X2( 0, (unsigned)( scale >> 8 ) );

  //
  // Pre-load all reflection coefficients and delay line elements.
  //

  // Load reflection coefficients.
  // Q23 <- Q(23+8) - 8
  cf_k2k1 = AE_L32X2F24_I( C, +0 );
  cf_k0z  = AE_L32X2F24_I( C, +8 );

  // Make skew coefficient pairs.
  select_f24x2_lh( cf_k1k0, cf_k2k1, cf_k0z  ); // k1 k0
  select_f24x2_lh( cf_k1k2, cf_k2k1, cf_k2k1 ); // k1 k2

  // Load state elements d2,d1,d0.
  // Q1.22 <- Q(1.22+8) - 8
  dl_d2d1 = AE_L32X2F24_I( D, +0 );
  dl_d0z  = AE_L32X2F24_I( D, +8 );

  //
  // Pass the input samples block through the AR lattice. n-th response sample
  // and lattice state update are defined as follows:
  //
  //   p1 = g*x[n] - k2*d2 - k1*d1
  //   p0 = p1 - k0*d0;
  //
  //   d2 = d1 + k1*p1; d1 = d0 + k0*p0; d0 = p0;
  //
  //   r[n] = p0;
  //
  // The inner loop is fully unrolled.
  //

  for ( n=0; n<N; n++ )
  {
    // Q23 <- Q(23+8) - 8
    AE_L32F24_IP( s0, X, +4 );

    t0 = ( s0 );

    // Prescaling; q0 = g*x[n]; q1 = g*x[n]
    // Q17.46 <- Q23*Q23
    u0 = AE_MULZAAD24_HH_LL( t0, t_zg);
    u1 = u0;

    q0 = ( u0 );
    q1 = ( u1 );

    // q1 -= k0*d0
    // Q17.46 <- Q17.46 + [ Q23*Q1.22 + 1 ]
    AE_MULSSFD24_HH_LL( q1, cf_k0z, dl_d0z );

    // q0 -= k2*d2 + k1*d1;
    // q1 -= k2*d2 + k1*d1;
    // Q17.46 <- Q17.46 + [ Q23*Q1.22 + 1 ] + [ Q23*Q1.22 + 1 ]
    AE_MULSSFD24_HH_LL( q0, dl_d2d1, cf_k2k1 );
    AE_MULSSFD24_HH_LL( q1, dl_d2d1, cf_k2k1 );

    // p1 = q0; p0 = q1;
    // Q1.22 <- Q17.46 - 24 w/ rounding and saturation
    s1 = AE_ROUND24X2F48SASYM( q0, q1 );

    // k1*p1 k0*p0
    // Q9.22 <- Q23*Q1.22 - 23 w/ rounding
    r0 = AE_MULFP24X2RA( cf_k1k0, s1 );

    // d1 d0
    select_f24x2_lh( dl_d1d0, dl_d2d1, dl_d0z );


    // d2 = d1 + k1*p1; d1 = d0 + k0*p0
    // Q1.22 = sat24( Q9.22 + Q1.22 )
    dl_d2d1 = AE_ADD24S( r0, dl_d1d0 );


    // d0 = p0; 0
    select_f24x2_ll( dl_d0z, s1, dl_d0z );

    // p0 = q1
    // Q16.47 <- Q17.46 + 1
    q2 = AE_F64_SLAIS( q1, 1 );

    // r[n] = p0
    // Q(23+8) <- [ Q16.47 - 24 w/ rounding and saturation ] + 8
    AE_S24RA64S_IP( q2, R, +4 );
  }

  //
  // Store updated delay line elements.
  //

  // Q(1.22+8) <- Q1.22 + 8
  AE_S32X2F24_I( dl_d2d1, D, +0 );
  AE_S32X2F24_I( dl_d0z , D, +8 );

} // latr3_proc()

/* Order 4 lattice filter data processing function. */
static void latr4_proc( f24 * restrict r,     // r[N]     [out   ] Q(23+8)
                  const f24 * restrict x,     // x[N]     [in    ] Q(23+8)
                        f24 *          dline, // dline[M] [in/out] Q(1.22+8)
                  const f24 *          coef,  // coef[M]  [in    ] Q(23+8)
                  f24 scale, int N, int M )   // scale    [in    ] Q(23+8)
{
  const ae_f24   * restrict X;
        ae_f24   * restrict R;
  const ae_f24x2 *          C;
        ae_f24x2 *          D;

  ae_f24x2   cf_k3k2, cf_k1k0;
  ae_f24x2   cf_k2k1, cf_k0z, cf_k1z;
  ae_f24x2   dl_d3d2, dl_d1d0, dl_d2d1;
  ae_f24x2   s0, s1, s2, s_zz;
  ae_int24x2 t0, t_zg;
  ae_f32x2   r0;
  ae_f64     q0, q1;
  ae_int64   u0, u1;

  int n;

  ASSERT( r && x && dline && coef );

  ASSERT( M == 4 );

  ASSERT( IS_ALIGN( dline ) && IS_ALIGN( coef ) );

  //
  // Setup pointers.
  //

  X = (const ae_f24   *)x;
  R = (      ae_f24   *)r;
  C = (const ae_f24x2 *)coef;
  D = (      ae_f24x2 *)dline;

  //
  // Load the prescaling factor and set zero registers.
  //

  // t_zg = 0 g
  // Q23 <- Q(23+8) - 8
  t_zg = AE_MOVPA24X2( 0, (unsigned)( scale >> 8 ) );

  s_zz = 0;

  //
  // Pre-load all reflection coefficients and delay line elements.
  //

  // Load reflection coefficients.
  // Q23 <- Q(23+8) - 8
  cf_k3k2 = AE_L32X2F24_I( C, +0 );
  cf_k1k0 = AE_L32X2F24_I( C, +8 );

  // Make skew coefficients pairs.
  select_f24x2_lh( cf_k2k1, cf_k3k2, cf_k1k0 ); // k2 k1
  select_f24x2_lh( cf_k0z , cf_k1k0, s_zz    ); // k0  0
  select_f24x2_hh( cf_k1z , cf_k1k0, s_zz    ); // k1  0

  // Load state elements d3..d0.
  // Q1.22 <- Q(1.22+8) - 8
  dl_d3d2 = AE_L32X2F24_I( D, +0 );
  dl_d1d0 = AE_L32X2F24_I( D, +8 );

  //
  // Pass the input samples block through the AR lattice. n-th response sample
  // and lattice state update are defined as follows:
  //
  //   p2 = g*x[n] - k3*d3 - k2*d2
  //   p1 = p2 - k1*d1; p0 = p1 - k0*d0;
  //
  //   d3 = d2 + k2*p2; d2 = d1 + k1*p1; d1 = d0 + k0*p0;
  //   d0 = p0;
  //
  //   r[n] = p0;
  //
  // The inner loop is fully unrolled.
  //

  for ( n=0; n<N; n++ )
  {
    // Q23 <- Q(23+8) - 8
    AE_L32F24_IP( s0, X, +4 );

    t0 = ( s0 );

    // Prescaling, q0 = g*x[n]; q1 = g*x[n];
    // Q17.46 <- Q23*Q23
    u0 = AE_MULZAAD24_HH_LL( t0, t_zg );
    u1 = AE_MULZAAD24_HL_LH( t0, t_zg );

    q0 = ( u0 );
    q1 = ( u1 );

    //--------------------------------------------------------------------------
    // Update delay elements d3, d2

    // q0 -= k3*d3 + k2*d2; q1 -= k3*d3 + k2*d2
    // Q17.46 <- Q17.46 + [ Q23*Q1.22 + 1 ] + [ Q23*Q1.22 + 1 ]
    AE_MULSSFD24_HH_LL( q0, dl_d3d2, cf_k3k2 );
    AE_MULSSFD24_HH_LL( q1, dl_d3d2, cf_k3k2 );

    // q1 -= k1*d1
    // Q17.46 <- Q17.46 + [ Q23*Q1.22 + 1 ]
    AE_MULSSFD24_HH_LL( q1, dl_d1d0, cf_k1z );

    // p2 = q0; p1 = q1;
    // Q1.22 <- Q17.46 - 24 w/ rounding and saturation
    s1 = AE_ROUND24X2F48SASYM( q0, q1 );

    // k2*p2; k1*p1
    // Q9.22 <- Q23*Q1.22 - 23 w/ rounding
    r0 = AE_MULFP24X2RA( s1, cf_k2k1 );

    // d2 d1
    select_f24x2_lh( dl_d2d1, dl_d3d2, dl_d1d0 );

    // d3 = d2 + k2*p2; d2 = d1 + k1*p1;
    // Q1.22 <- sat24( Q1.22 + Q9.22 )
    dl_d3d2 = AE_ADD24S( dl_d2d1, r0);


    //--------------------------------------------------------------------------
    // Update delay elements d1, d0

    // q1 -= k0*d0
    // Q17.46 <- Q17.46 + [ Q23*Q1.22 + 1 ]
    AE_MULSSFD24_HL_LH( q1, dl_d1d0, cf_k0z );

    // p0 = q1; p0 = q1
    // Q1.22 <- Q17.46 - 24 w/ rounding and saturation
    s1 = AE_ROUNDSP24Q48ASYM( q1 );

    // k0*p0; 0*p0;
    // Q9.22 <- Q23*Q1.22 - 23 w/ rounding
    r0 = AE_MULFP24X2RA( s1, cf_k0z );

    // d0 p0
    select_f24x2_lh( dl_d1d0, dl_d1d0, s1 );


    // d1 = d0 + k0*p0; d0 = p0 + 0*p0;
    // Q1.22 <- sat24( Q1.22 + Q9.22 )
    dl_d1d0 = AE_ADD24S( dl_d1d0, r0);

    //--------------------------------------------------------------------------
    // Format and save the response sample r[n].

    // r[n-1]; r[n] = q1
    // Q23 <- Q17.46 + 1 - 24 w/ rounding and saturation.
    AE_PKSR24( s2, q1, 1 );

    // Q(23+8) <- Q23 + 8
    AE_S32F24_L_IP( s2, R, +4 );
  }

  //
  // Store updated delay line elements.
  //

  // Q(1.22+8) <- Q1.22 + 8
  AE_S32X2F24_I( dl_d3d2, D, +0 );
  AE_S32X2F24_I( dl_d1d0, D, +8 );

} // latr4_proc()

/* Order 5 lattice filter data processing function. */
static void latr5_proc( f24 * restrict r,     // r[N]     [out   ] Q(23+8)
                  const f24 * restrict x,     // x[N]     [in    ] Q(23+8)
                        f24 *          dline, // dline[M] [in/out] Q(1.22+8)
                  const f24 *          coef,  // coef[M]  [in    ] Q(23+8)
                  f24 scale, int N, int M )   // scale    [in    ] Q(23+8)
{
  const ae_f24   * restrict X;
        ae_f24   * restrict R;
  const ae_f24x2 *          C;
        ae_f24x2 *          D;

  ae_f24x2   cf_k4k3, cf_k2k1, cf_k0z;
  ae_f24x2   cf_k3k2, cf_k2z, cf_k1k0;
  ae_f24x2   dl_d4d3, dl_d2d1, dl_d0z;
  ae_f24x2   dl_d3d2, dl_d1d0;
  ae_f32x2   r0;
  ae_f24x2   s0, s1;
  ae_int24x2 t0, t_zg;
  ae_f64     q0, q1, q2;
  ae_int64   u0;

  int n;

  ASSERT( r && x && dline && coef );

  ASSERT( M == 5 );

  ASSERT( IS_ALIGN( dline ) && IS_ALIGN( coef ) );

  //
  // Setup pointers.
  //

  X = (const ae_f24   *)x;
  R = (      ae_f24   *)r;
  C = (const ae_f24x2 *)coef;
  D = (      ae_f24x2 *)dline;

  //
  // Load the prescaling factor.
  //

  // Q23 <- Q(23+8) - 8
  t_zg = AE_MOVPA24X2( 0, (unsigned)( scale >> 8 ) );

  //
  // Pre-load all reflection coefficients and delay line elements.
  //

  // Load reflection coefficients.
  // Q23 <- Q(23+8) - 8
  cf_k4k3 = AE_L32X2F24_I( C,  +0 );
  cf_k2k1 = AE_L32X2F24_I( C, + 8 );
  cf_k0z  = AE_L32X2F24_I( C, +16 );

  // Make skew coefficient pairs.
  select_f24x2_lh( cf_k3k2, cf_k4k3, cf_k2k1 ); // k3 k2
  select_f24x2_hl( cf_k2z , cf_k2k1, cf_k0z  ); // k2  0
  select_f24x2_lh( cf_k1k0, cf_k2k1, cf_k0z  ); // k1 k0

  // Load state elements d4..d0.
  // Q1.22 <- Q(1.22+8) - 8
  dl_d4d3 = AE_L32X2F24_I( D,  +0 );
  dl_d2d1 = AE_L32X2F24_I( D,  +8 );
  dl_d0z  = AE_L32X2F24_I( D, +16 );

  //
  // Pass the input samples block through the AR lattice. n-th response sample
  // and lattice state update are defined as follows:
  //
  //   p3 = g*x[n] - k4*d4 - k3*d3
  //   p2 = p3 - k2*d2; p1 = p2 - k1*d1; p0 = p1 - k0*d0;
  //
  //   d4 = d3 + k3*p3; d3 = d2 + k2*p2; d2 = d1 + k1*p1;
  //   d1 = d0 + k0*p0; d0 = p0;
  //
  //   r[n] = p0;
  //
  // The inner loop is fully unrolled.
  //

  for ( n=0; n<N; n++ )
  {
    // Q23 <- Q(23+8) - 8
    AE_L32F24_IP( s0, X, +4 );

    t0 = ( s0 );

    // Prescaling, q0 = g*x[n]
    // Q17.46 <- Q23*Q23
    u0 = AE_MULZAAD24_HH_LL( t0, t_zg );

    q0 = ( u0 );

    //--------------------------------------------------------------------------
    // Update delay elements d4, d3.

    // q0 -= k4*d4 + k3*d3;
    // Q17.46 <- Q17.46 + [ Q23*Q1.22 + 1 ] + [ Q23*Q1.22 + 1 ]
    AE_MULSSFD24_HH_LL( q0, dl_d4d3, cf_k4k3 );

    q1 = q0;

    // q1 -= k2*d2
    // Q17.46 <- Q17.46 + [ Q23*Q1.22 + 1 ]
    AE_MULSSFD24_HH_LL( q1, dl_d2d1, cf_k2z );

    // p3 = q0; p2 = q1
    // Q1.22 <- Q17.46 - 24 w/ rounding and saturation.
    s1 = AE_ROUND24X2F48SASYM( q0, q1 );

    // k3*p3; k2*p2
    // Q9.22 <- Q23*Q1.22 - 23 w/ rounding
    r0 = AE_MULFP24X2RA( cf_k3k2, s1 );

    // d3 d2
    select_f24x2_lh( dl_d3d2, dl_d4d3, dl_d2d1 );

    // d4 = d3 + k3*p3; d3 = d2 + k2*p2;
    // Q1.22 <- sat24( Q1.22 + Q9.22 )
    dl_d4d3 = AE_ADD24S( dl_d3d2, r0);


    //--------------------------------------------------------------------------
    // Update delay elements d2, d1.

    // d1 d0
    select_f24x2_lh( dl_d1d0, dl_d2d1, dl_d0z );

    // q0 -= k2*d2 + k1*d1; q1 -= k1*d1 + k0*d0
    // Q17.46 <- Q17.46 + [ Q23*Q1.22 + 1 ] + [ Q23*Q1.22 + 1 ]
    AE_MULSSFD24_HH_LL( q0, dl_d2d1, cf_k2k1 );
    AE_MULSSFD24_HH_LL( q1, dl_d1d0, cf_k1k0 );

    // p1 = q0; p0 = q1
    // Q1.22 <- Q17.46 - 24 w/ rounding and saturation.
    s1 = AE_ROUND24X2F48SASYM( q0, q1 );

    // k1*p1; k0*p0
    // Q9.22 <- Q23*Q1.22 - 23 w/ rounding
    r0 = AE_MULFP24X2RA( s1, cf_k1k0 );


    // d2 = d1 + k1*p1; d1 = d0 + k0*p0;
    // Q1.22 <- sat24( Q1.22 + Q9.22 )
    dl_d2d1 = AE_ADD24S( dl_d1d0, r0);


    //--------------------------------------------------------------------------
    // Update delay element d0; format and save the response sample r[n].

    // d0 = p0; 0
    select_f24x2_ll( dl_d0z, s1, dl_d0z );

    // r[n] = q1
    // Q16.47 <- Q17.46 + 1
    q2 = AE_F64_SLAIS( q1, 1 );

    // Q(23+8) <- [ Q16.47 - 24 w/ rounding and saturation ] + 8
    AE_S24RA64S_IP( q2, R, +4 );
  }

  //
  // Store updated delay line elements.
  //

  // Q(1.22+8) <- Q1.22 + 8
  AE_S32X2F24_I( dl_d4d3, D,  +0 );
  AE_S32X2F24_I( dl_d2d1, D,  +8 );
  AE_S32X2F24_I( dl_d0z , D, +16 );

} // latr5_proc()

/* Order 6 lattice filter data processing function. */
static void latr6_proc( f24 * restrict r,     // r[N]     [out   ] Q(23+8)
                  const f24 * restrict x,     // x[N]     [in    ] Q(23+8)
                        f24 *          dline, // dline[M] [in/out] Q(1.22+8)
                  const f24 *          coef,  // coef[M]  [in    ] Q(23+8)
                  f24 scale, int N, int M )   // scale    [in    ] Q(23+8)
{
  const ae_f24   * restrict X;
        ae_f24   * restrict R;
  const ae_f24x2 *          C;
        ae_f24x2 *          D;

  ae_f24x2   cf_k5k4, cf_k3k2, cf_k1k0;
  ae_f24x2   cf_k4k3, cf_k3z, cf_k2k1, cf_k0z;
  ae_f24x2   dl_d5d4, dl_d3d2, dl_d1d0;
  ae_f24x2   dl_d4d3, dl_d2d1;
  ae_f24x2   s0, s1, s_zz;
  ae_int24x2 t0, t_zg;
  ae_f32x2   r0;
  ae_f64     q0, q1, q2;
  ae_int64   u0/*, u1*/;

  int n;

  ASSERT( r && x && dline && coef );

  ASSERT( M == 6 );

  ASSERT( IS_ALIGN( dline ) && IS_ALIGN( coef ) );

  //
  // Setup pointers.
  //

  X = (const ae_f24   *)x;
  R = (      ae_f24   *)r;
  C = (const ae_f24x2 *)coef;
  D = (      ae_f24x2 *)dline;

  //
  // Load the prescaling factor and set zero registers.
  //

  // t_zg = 0 g
  // Q23 <- Q(23+8) - 8
  t_zg = AE_MOVPA24X2( 0, (unsigned)( scale >> 8 ) );

  s_zz = 0;

  //
  // Pre-load all reflection coefficients and delay line elements.
  //

  // Load reflection coefficients.
  // Q23 <- Q(23+8) - 8
  cf_k5k4 = AE_L32X2F24_I( C,  +0 );
  cf_k3k2 = AE_L32X2F24_I( C,  +8 );
  cf_k1k0 = AE_L32X2F24_I( C, +16 );

  // Make skew coefficients pairs.
  select_f24x2_lh( cf_k4k3, cf_k5k4, cf_k3k2 ); // k4 k3
  select_f24x2_hh( cf_k3z , cf_k3k2, s_zz    ); // k3  0
  select_f24x2_lh( cf_k2k1, cf_k3k2, cf_k1k0 ); // k2 k1
  select_f24x2_lh( cf_k0z , cf_k1k0, s_zz    ); // k0  0

  // Load state elements d5..d0.
  // Q1.22 <- Q(1.22+8) - 8
  dl_d5d4 = AE_L32X2F24_I( D,  +0 );
  dl_d3d2 = AE_L32X2F24_I( D,  +8 );
  dl_d1d0 = AE_L32X2F24_I( D, +16 );

  //
  // Pass the input samples block through the AR lattice. n-th response sample
  // and lattice state update are defined as follows:
  //
  //   p4 = g*x[n] - k5*d5 - k4*d4
  //   p3 = p4 - k3*d3; p2 = p3 - k2*d2; p1 = p2 - k1*d1; p0 = p1 - k0*d0;
  //
  //   d5 = d4 + k4*p4; d4 = d3 + k3*p3; d3 = d2 + k2*p2; d2 = d1 + k1*p1;
  //   d1 = d0 + k0*p0; d0 = p0;
  //
  //   r[n] = p0;
  //
  // The inner loop is fully unrolled.
  //

  for ( n=0; n<N; n++ )
  {
    // Q23 <- Q(23+8) - 8
    AE_L32F24_IP( s0, X, +4 );

    t0 = ( s0 );

    // Prescaling, q0 = g*x[n]
    // Q17.46 <- Q23*Q23
    u0 = AE_MULZAAD24_HH_LL( t0, t_zg );

    q0 = ( u0 );

    //--------------------------------------------------------------------------
    // Update delay elements d5, d4.

    // q0 -= k5*d5 + k4*d4
    // Q17.46 <- Q17.46 + [ Q23*Q1.22 + 1 ] + [ Q23*Q1.22 + 1 ]
    AE_MULSSFD24_HH_LL( q0, dl_d5d4, cf_k5k4 );

    q1 = q0;

    // q1 -= k3*d3
    // Q17.46 <- Q17.46 + [ Q23*Q1.22 + 1 ]
    AE_MULSSFD24_HH_LL( q1, dl_d3d2, cf_k3z );

    // p4 = q0; p3 = q1
    // Q1.22 <- Q17.46 - 24 w/ rounding and saturation
    s1 = AE_ROUND24X2F48SASYM( q0, q1 );

    // k4*p4; k3*p3
    // Q9.22 <- Q23*Q1.22 - 23 w/ rounding
    r0 = AE_MULFP24X2RA( s1, cf_k4k3 );

    // d4 d3
    select_f24x2_lh( dl_d4d3, dl_d5d4, dl_d3d2 );


    // d5 = d4 + k4*p4; d4 = d3 + k3*p3
    // Q1.22 <- sat24( Q1.22 + Q9.22 )
    dl_d5d4 = AE_ADD24S( dl_d4d3, r0 );


    //--------------------------------------------------------------------------
    // Update delay elements d3, d2

    // d2 d1
    select_f24x2_lh( dl_d2d1, dl_d3d2, dl_d1d0 );

    // q0 -= k3*d3 + k2*d2; q1 -= k2*d2 + k1*d1
    // Q17.46 <- Q17.46 + [ Q23*Q1.22 + 1 ] + [ Q23*Q1.22 + 1 ]
    AE_MULSSFD24_HH_LL( q0, dl_d3d2, cf_k3k2 );
    AE_MULSSFD24_HH_LL( q1, dl_d2d1, cf_k2k1 );

    // p2 = q0; p1 = q1
    // Q1.22 <- Q17.46 - 24 w/ rounding and saturation
    s1 = AE_ROUND24X2F48SASYM( q0, q1 );

    // k2*p2; k1*p1
    // Q9.22 <- Q23*Q1.22 - 23 w/ rounding
    r0 = AE_MULFP24X2RA( s1, cf_k2k1 );

    // d3 = d2 + k2*p2; d2 = d1 + k1*p1;
    // Q1.22 <- sat24( Q1.22 + Q9.22 )
    dl_d3d2 = AE_ADD24S( dl_d2d1, r0 );

    //--------------------------------------------------------------------------
    // Update delay elements d1, d0

    // q1 -= k0*d0
    // Q17.46 <- Q17.46 + [ Q23*Q1.22 + 1 ]
    AE_MULSSFD24_HL_LH( q1, dl_d1d0, cf_k0z );

    // p0 = q1; p0 = q1
    // Q1.22 <- Q17.46 - 24 w/ rounding and saturation
    s1 = AE_ROUNDSP24Q48ASYM( q1 );

    // k0*p0; 0*p0;
    // Q9.22 <- Q23*Q1.22 - 23 w/ rounding
    r0 = AE_MULFP24X2RA( s1, cf_k0z );

    // d0 p0
    select_f24x2_lh( dl_d1d0, dl_d1d0, s1 );

    // d1 = d0 + k0*p0; d0 = p0 + 0*p0
    // Q1.22 <- sat24( Q1.22 + Q9.22 )
    dl_d1d0 = AE_ADD24S( dl_d1d0, r0 );


    //--------------------------------------------------------------------------
    // Format and save the response sample r[n].

    // r[n] = q1
    // Q16.47 <- Q17.46 + 1
    q2 = AE_F64_SLAIS( q1, 1 );

    // Q(23+8) <- [ Q16.47 - 24 w/ rounding and saturation ] + 8
    AE_S24RA64S_IP( q2, R, +4 );
  }

  //
  // Store updated delay line elements.
  //

  // Q(1.22+8) <- Q1.22 + 8
  AE_S32X2F24_I( dl_d5d4, D,  +0 );
  AE_S32X2F24_I( dl_d3d2, D,  +8 );
  AE_S32X2F24_I( dl_d1d0, D, +16 );

} // latr6_proc()

/* Order 7 lattice filter data processing function. */
static void latr7_proc( f24 * restrict r,     // r[N]     [out   ] Q(23+8)
                  const f24 * restrict x,     // x[N]     [in    ] Q(23+8)
                        f24 *          dline, // dline[M] [in/out] Q(1.22+8)
                  const f24 *          coef,  // coef[M]  [in    ] Q(23+8)
                  f24 scale, int N, int M )   // scale    [in    ] Q(23+8)
{
  const ae_f24   * restrict X;
        ae_f24   * restrict R;
  const ae_f24x2 *          C;
        ae_f24x2 *          D;

  ae_f24x2   cf_k6k5, cf_k4k3, cf_k2k1, cf_k0z;
  ae_f24x2   cf_k5k4, cf_k3k2, cf_k1k0;
  ae_f24x2   dl_d6d5, dl_d4d3, dl_d2d1, dl_d0z;
  ae_f24x2   dl_d5d4, dl_d3d2, dl_d1d0;
  ae_f32x2   r0;
  ae_f24x2   s0, s1, s2, s_d6z;
  ae_int24x2 t0, t_zg;
  ae_f64     q0, q1, q2;
  ae_int64   u0;

  int n;

  ASSERT( r && x && dline && coef );

  ASSERT( M == 7 );

  ASSERT( IS_ALIGN( dline ) && IS_ALIGN( coef ) );

  //
  // Setup pointers.
  //

  X = (const ae_f24   *)x;
  R = (      ae_f24   *)r;
  C = (const ae_f24x2 *)coef;
  D = (      ae_f24x2 *)dline;

  //
  // Load the prescaling factor and set zero registers.
  //

  // Q23 <- Q(23+8) - 8
  t_zg = AE_MOVPA24X2( 0, (unsigned)( scale >> 8 ) );

  s_d6z = 0;

  //
  // Pre-load reflection coefficients and some of delay line elements.
  //

  // Load reflection coefficients.
  // Q23 <- Q(23+8) - 8
  cf_k6k5 = AE_L32X2F24_I( C,  +0 );
  cf_k4k3 = AE_L32X2F24_I( C,  +8 );
  cf_k2k1 = AE_L32X2F24_I( C, +16 );
  cf_k0z  = AE_L32X2F24_I( C, +24 );

  // Load d6, d5. Other delay line elements are loaded/stored on each loop 
  // iteration in order to reduce pressure on the registers file.
  // Q1.22 <- Q(1.22+8) - 8
  dl_d6d5 = AE_L32X2F24_I( D, +0 );

  //
  // Pass the input samples block through the AR lattice. n-th response sample
  // and lattice state update are defined as follows:
  //
  //   p5 = g*x[n] - k6*d6 - k5*d5
  //   p4 = p5 - k4*d4; p3 = p4 - k3*d3; p2 = p3 - k2*d2; 
  //   p1 = p2 - k1*d1; p0 = p1 - k0*d0;
  //
  //   d6 = d5 + k5*p5; d5 = d4 + k4*p4; d4 = d3 + k3*p3; 
  //   d3 = d2 + k2*p2; d2 = d1 + k1*p1;
  //   d1 = d0 + k0*p0; d0 = p0;
  //
  //   r[n] = p0;
  //
  // The inner loop is fully unrolled.
  //

  for ( n=0; n<N; n++ )
  {
    // Q23 <- Q(23+8) - 8
    AE_L32F24_IP( s0, X, +4 );

    t0 = ( s0 );

    // Prescaling, q0 = g*x[n]; q1 = g*x[n]
    // Q17.46 <- Q23*Q23
    u0 = AE_MULZAAD24_HH_LL( t0, t_zg );

    q0 = ( u0 );
    q1 = ( u0 );

    // d6 0
    select_f24x2_hl( s_d6z, dl_d6d5, s_d6z );

    // q1 -= k6*d6
    // Q17.46 <- Q17.46 + [ Q23*Q1.22 + 1 ]
    AE_MULSSFD24_HH_LL( q1, cf_k6k5, s_d6z );

    //--------------------------------------------------------------------------
    // Update delay elements d6, d5.

    // Load d4, d3.
    // Q1.22 <- Q(1.22+8) - 8
    dl_d4d3 = AE_L32X2F24_I( D, +8 );

    // d5 d4
    select_f24x2_lh( dl_d5d4, dl_d6d5, dl_d4d3 );
    // k5 k4
    select_f24x2_lh( cf_k5k4, cf_k6k5, cf_k4k3 );

    // q0 -= k6*d6 + k5*d5; q1 -= k5*d5 + k4*d4;
    // Q17.46 <- Q17.46 + [ Q23*Q1.22 + 1 ] + [ Q23*Q1.22 + 1 ]
    AE_MULSSFD24_HH_LL( q0, dl_d6d5, cf_k6k5 );
    AE_MULSSFD24_HH_LL( q1, dl_d5d4, cf_k5k4 );

    // p5 = q0; p4 = q1
    // Q1.22 <- Q17.46 - 24 w/ rounding and saturation.
    s1 = AE_ROUND24X2F48SASYM( q0, q1 );

    // k5*p5; k4*p4
    // Q9.22 <- Q23*Q1.22 - 23 w/ rounding
    r0 = AE_MULFP24X2RA( cf_k5k4, s1 );

    // d6 = d5 + k5*p5; d5 = d4 + k4*p4
    // Q1.22 <- sat24( Q1.22 + Q9.22 )
    dl_d6d5 = AE_ADD24S( dl_d5d4, r0 );


    //--------------------------------------------------------------------------
    // Update delay elements d4, d3.

    // Load d2, d1.
    // Q1.22 <- Q(1.22+8) - 8
    dl_d2d1 = AE_L32X2F24_I( D, +16 );

    // d3 d2
    select_f24x2_lh( dl_d3d2, dl_d4d3, dl_d2d1 );
    // k3 k2
    select_f24x2_lh( cf_k3k2, cf_k4k3, cf_k2k1 );

    // q0 -= k4*d4 + k3*d3; q1 -= k3*d3 + k2*d2
    // Q17.46 <- Q17.46 + [ Q23*Q1.22 + 1 ] + [ Q23*Q1.22 + 1 ]
    AE_MULSSFD24_HH_LL( q0, dl_d4d3, cf_k4k3 );
    AE_MULSSFD24_HH_LL( q1, dl_d3d2, cf_k3k2 );

    // p3 = q0; p2 = q0
    // Q1.22 <- Q17.46 - 24 w/ rounding and saturation.
    s1 = AE_ROUND24X2F48SASYM( q0, q1 );

    // k3*p3; k2*p2
    // Q9.22 <- Q23*Q1.22 - 23 w/ rounding
    r0 = AE_MULFP24X2RA( cf_k3k2, s1 );

    // d4 = d3 + k3*p3; d3 = d2 + k2*p2;
    // Q1.22 <- sat24( Q1.22 + Q9.22 )
    dl_d4d3 = AE_ADD24S( dl_d3d2, r0 );

    // Store new d4, d3.
    // Q(1.22+8) <- Q1.22 + 8
    AE_S32X2F24_I( dl_d4d3, D, +8 );

    //--------------------------------------------------------------------------
    // Update delay elements d2, d1.

    // Load d0, zero.
    // Q1.22 <- Q(1.22+8) - 8
    dl_d0z = AE_L32X2F24_I( D, +24 );

    // d1 d0
    select_f24x2_lh( dl_d1d0, dl_d2d1, dl_d0z );
    // k1 k0
    select_f24x2_lh( cf_k1k0, cf_k2k1, cf_k0z );

    // q0 -= k2*d2 + k1*d1; q1 = k1*d1 + k0*d0
    // Q17.46 <- Q17.46 + [ Q23*Q1.22 + 1 ] + [ Q23*Q1.22 + 1 ]
    AE_MULSSFD24_HH_LL( q0, dl_d2d1, cf_k2k1 );
    AE_MULSSFD24_HH_LL( q1, dl_d1d0, cf_k1k0 );

    // p1 = q0; p0 = q1
    // Q1.22 <- Q17.46 - 24 w/ rounding and saturation.
    s1 = AE_ROUND24X2F48SASYM( q0, q1 );

    // k1*p1; k0*p0
    // Q9.22 <- Q23*Q1.22 - 23 w/ rounding
    r0 = AE_MULFP24X2RA( s1, cf_k1k0 );

    // d2 = d1 + k1*p1; d1 = d0 + k0*p0;
    // Q1.22 <- sat24( Q1.22 + Q9.22 )
    dl_d2d1 = AE_ADD24S( dl_d1d0, r0 );


    // Store new d2, d1.
    // Q(1.22+8) <- Q1.22 + 8
    AE_S32X2F24_I( dl_d2d1, D, +16 );

    //--------------------------------------------------------------------------
    // Update delay element d0; format and save the response sample r[n].

    // d0 = p0; 0
    select_f24x2_ll( dl_d0z, s1, dl_d0z );

    // Q(1.22+8) <- Q1.22 + 8
    AE_S32X2F24_I( dl_d0z, D, +24 );

    // r[n] = q1
    // Q16.47 <- Q17.46 + 1
    q2 = AE_F64_SLAIS( q1, 1 );

    // Q23 <- Q16.47 - 24 w/ rounding and saturation
    s2 = AE_ROUNDSP24Q48ASYM( q2 );

    // Q(23+8) <- Q23 + 8
    AE_S32F24_L_IP( s2, R, +4 );
  }

  // Store new d6, d5.
  // Q(1.22+8) <- Q1.22 + 8
  AE_S32X2F24_I( dl_d6d5, D, +0 );

} // latr7_proc()

/* Order 8 lattice filter data processing function. */
static void latr8_proc( f24 * restrict r,     // r[N]     [out   ] Q(23+8)
                  const f24 * restrict x,     // x[N]     [in    ] Q(23+8)
                        f24 *          dline, // dline[M] [in/out] Q(1.22+8)
                  const f24 *          coef,  // coef[M]  [in    ] Q(23+8)
                  f24 scale, int N, int M )   // scale    [in    ] Q(23+8)
{
  const ae_f24   * restrict X;
        ae_f24   * restrict R;
  const ae_f24x2 *          C;
        ae_f24x2 *          D;

  ae_f24x2   cf_k7k6, cf_k5k4, cf_k3k2, cf_k1k0;
  ae_f24x2   cf_zk7,  cf_k6k5, cf_k4k3, cf_k2k1, cf_k0z;
  ae_f24x2   dl_d7d6, dl_d5d4, dl_d3d2, dl_d1d0;
  ae_f24x2   dl_d6d5, dl_d4d3, dl_d2d1;
  ae_f24x2   s0, s1, s2, s_zd7;
  ae_int24x2 t0, t_zg;
  ae_f32x2   r0;
  ae_f64     q0, q1, q2;
  ae_int64   u0;

  int n;

  ASSERT( r && x && dline && coef );

  ASSERT( M == 8 );

  ASSERT( IS_ALIGN( dline ) && IS_ALIGN( coef ) );

  //
  // Setup pointers.
  //

  X = (const ae_f24   *)x;
  R = (      ae_f24   *)r;
  C = (const ae_f24x2 *)coef;
  D = (      ae_f24x2 *)dline;

  //
  // Load the prescaling factor and set zero registers.
  //

  // 0 g
  // Q23 <- Q(23+8) - 8
  t_zg = AE_MOVPA24X2( 0, (unsigned)( scale >> 8 ) );

  s_zd7 = 0;

  //
  // Pre-load selected reflection coefficients and some of delay line elements.
  // Most of reflection coefficients and state elements are loaded/stored on
  // each iteration to reduce pressure on the registers file at the price of
  // redundant memory access operations.
  //

  // Load reflection coefficients k1, k0.
  // Q23 <- Q(23+8) - 8
  cf_k1k0 = AE_L32X2F24_I( C, +24 );

  // Load delay line lements d7..d4.
  // Q1.22 <- Q(1.22+8) - 8
  dl_d7d6 = AE_L32X2F24_I( D, +0 );
  dl_d5d4 = AE_L32X2F24_I( D, +8 );

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
    // Q23 <- Q(23+8) - 8
    cf_k7k6 = AE_L32X2F24_I( C,  +0 );
    cf_k5k4 = AE_L32X2F24_I( C,  +8 );
    cf_k3k2 = AE_L32X2F24_I( C, +16 );

    // Q23 <- Q(23+8) - 8
    AE_L32F24_IP( s0, X, +4 );

    t0 = ( s0 );

    // Prescaling, q0 = g*x[n]; q1 = g*x[n]
    // Q17.46 <- Q23*Q23
    u0 = AE_MULZAAD24_HH_LL( t0, t_zg );

    q0 = ( u0 );
    q1 = ( u0 );

    // 0 k7
    select_f24x2_hh( cf_zk7, s_zd7, cf_k7k6 );
    // 0 d7
    select_f24x2_hh( s_zd7, s_zd7, dl_d7d6 );

    // q1 -= k7*d7
    // Q17.46 <- Q17.46 + [ Q23*Q1.22 + 1 ]
    AE_MULSSFD24_HH_LL( q1, s_zd7, cf_zk7 );

    //--------------------------------------------------------------------------
    // Update delay elements d7, d6.

    // d6 d5
    select_f24x2_lh( dl_d6d5, dl_d7d6, dl_d5d4 );
    // k6 k5
    select_f24x2_lh( cf_k6k5, cf_k7k6, cf_k5k4 );

    // q0 -= k7*d7 + k6*d6; q1 -= k6*d6 + k5*d5
    // Q17.46 <- Q17.46 + [ Q23*Q1.22 + 1 ] + [ Q23*Q1.22 + 1 ]
    AE_MULSSFD24_HH_LL( q0, dl_d7d6, cf_k7k6 );
    AE_MULSSFD24_HH_LL( q1, dl_d6d5, cf_k6k5 );

    // p6 = q0; p5 = q1;
    // Q1.22 <- Q17.46 - 24 w/ rounding and saturation
    s1 = AE_ROUND24X2F48SASYM( q0, q1 );

    // k6*p6 k5*p5
    // Q9.22 <- Q23*Q1.22 - 23 w/ rounding
    r0 = AE_MULFP24X2RA( s1, cf_k6k5 );

    // d7 = d6 + k6*p6; d6 = d5 + k5*p5
    // Q1.22 <- sat24( Q1.22 + Q9.22 )
    dl_d7d6 = AE_ADD24S( dl_d6d5, r0 );


    //--------------------------------------------------------------------------
    // Update delay elements d5, d4.

    // Load d3, d2
    // Q1.22 <- Q(1.22+8) - 8
    dl_d3d2 = AE_L32X2F24_I( D, +16 );

    // d4 d3
    select_f24x2_lh( dl_d4d3, dl_d5d4, dl_d3d2 );
    // k4 k3
    select_f24x2_lh( cf_k4k3, cf_k5k4, cf_k3k2 );

    // q0 -= k5*d5 + k4*d4; q1 -= k4*d4 + k3*d3;
    // Q17.46 <- Q17.46 + [ Q23*Q1.22 + 1 ] + [ Q23*Q1.22 + 1 ]
    AE_MULSSFD24_HH_LL( q0, dl_d5d4, cf_k5k4 );
    AE_MULSSFD24_HH_LL( q1, dl_d4d3, cf_k4k3 );

    // p4 = q0; p3 = q1
    // Q1.22 <- Q17.46 - 24 w/ rounding and saturation
    s1 = AE_ROUND24X2F48SASYM( q0, q1 );

    // k4*p4; k3*p3
    // Q9.22 <- Q23*Q1.22 - 23 w/ rounding
    r0 = AE_MULFP24X2RA( s1, cf_k4k3 );

    // d5 = d4 + k4*p4; d4 = d3 + k3*p3
    // Q1.22 <- sat24( Q1.22 + Q9.22 )
    dl_d5d4 = AE_ADD24S( dl_d4d3, r0 );

    //--------------------------------------------------------------------------
    // Update delay elements d3, d2

    // Load d1, d0
    // Q1.22 <- Q(1.22+8) - 8
    dl_d1d0 = AE_L32X2F24_I( D, +24 );

    // d2 d1
    select_f24x2_lh( dl_d2d1, dl_d3d2, dl_d1d0 );
    // k2 k1
    select_f24x2_lh( cf_k2k1, cf_k3k2, cf_k1k0 );

    // q0 -= k3*d3 + k2*d2; q1 -= k2*d2 + k1*d1;
    // Q17.46 <- Q17.46 + [ Q23*Q1.22 + 1 ] + [ Q23*Q1.22 + 1 ]
    AE_MULSSFD24_HH_LL( q0, dl_d3d2, cf_k3k2 );
    AE_MULSSFD24_HH_LL( q1, dl_d2d1, cf_k2k1 );

    // p2 = q0; p1 = q1
    // Q1.22 <- Q17.46 - 24 w/ rounding and saturation
    s1 = AE_ROUND24X2F48SASYM( q0, q1 );

    // k2*p2; k1*p1
    // Q9.22 <- Q23*Q1.22 - 23 w/ rounding
    r0 = AE_MULFP24X2RA( s1, cf_k2k1 );

    // d3 = d2 + k2*p2; d2 = d1 + k1*p1;
    // Q1.22 <- sat24( Q1.22 + Q9.22 )
    dl_d3d2 = AE_ADD24S( dl_d2d1, r0 );

    // Store new d3, d2
    // Q(1.22+8) <- Q1.22 + 8
    AE_S32X2F24_I( dl_d3d2, D, +16 );

    //--------------------------------------------------------------------------
    // Update delay elements d1, d0

    // q0 -= k1*d1 + k0*d0
    // Q17.46 <- Q17.46 + [ Q23*Q1.22 + 1 ] + [ Q23*Q1.22 + 1 ]
    AE_MULSSFD24_HH_LL( q0, dl_d1d0, cf_k1k0 );

    // p0 = q0; p0 = q0
    // Q1.22 <- Q17.46 - 24 w/ rounding and saturation
    s1 = AE_ROUNDSP24Q48ASYM( q0 );

    // k0 0
    select_f24x2_lh( cf_k0z, cf_k1k0, s_zd7 );

    // k0*p0; 0*p0
    // Q9.22 <- Q23*Q1.22 - 23 w/ rounding
    r0 = AE_MULFP24X2RA( s1, cf_k0z );

    // d0 p0
    select_f24x2_lh( dl_d1d0, dl_d1d0, s1 );

    // d1 = d0 + k0*p0; d0 = p0 + 0*p0;
    // Q1.22 <- sat24( Q1.22 + Q9.22 )
    dl_d1d0 = AE_ADD24S( dl_d1d0, r0 );

    // Store new d1, d0
    // Q(1.22+8) <- Q1.22 + 8
    AE_S32X2F24_I( dl_d1d0, D, +24 );

    //--------------------------------------------------------------------------
    // Format and save the response sample r[n].

    // r[n] = q0
    // Q16.47 <- Q17.46 + 1
    q2 = AE_F64_SLAIS( q0, 1 );

    // Q23 <- Q16.47 - 24 w/ rounding and saturation
    s2 = AE_ROUNDSP24Q48ASYM( q2 );

    // Q(23+8) <- Q23 + 8
    AE_S32F24_L_IP( s2, R, +4 );
  }

  // Store new d7..d4.
  // Q(1.22+8) <- Q1.22 + 8
  AE_S32X2F24_I( dl_d7d6, D, +0 );
  AE_S32X2F24_I( dl_d5d4, D, +8 );

} // latr8_proc()

/* Arbitrary order lattice filter data processing function. */
static void latrX_proc( f24 * restrict r,     // r[N]     [out   ] Q(23+8)
                  const f24 * restrict x,     // x[N]     [in    ] Q(23+8)
                        f24 *          dline, // dline[M] [in/out] Q(1.22+8)
                  const f24 *          coef,  // coef[M]  [in    ] Q(23+8)
                  f24 scale, int N, int M )   // scale    [in    ] Q(23+8)
{
        ae_f24 * restrict R;
  const ae_f24 *          X;
  const ae_f24 *          D_rd;
        ae_f24 * restrict D_wr;
  const ae_f24 *          C;

  ae_f24x2   s0, s1, s2, s3, s_zz;
  ae_int24x2 t0, t_zg;
  ae_f32x2   r0;
  ae_f64     q0, q1;
  ae_int64   u0;

  int n, m;

  ASSERT( r && x && dline && coef );

  ASSERT( IS_ALIGN( dline ) && IS_ALIGN( coef ) );

  //
  // Setup pointers.
  //

  R = (      ae_f24 *)r;
  X = (const ae_f24 *)x;

  //
  // Load the prescaling factor and zero registers.
  //

  // Q23 <- Q(23+8) - 8
  t_zg = AE_MOVPA24X2( 0, (unsigned)( scale >> 8 ) );

  s_zz = 0;

  //
  // Pass the input samples block through the AR lattice. n-th response sample
  // and lattice state update are defined as follows:
  //
  //   q = g*x[n] - k[M-1]*d[M-1];
  //
  //   for ( m=M-2; m>=0; m-- )
  //   {
  //     q -= k[m]*d[m];
  //     d[m+1] = d[m] + k[m]*q;
  //   }
  //
  //   d[0] = r[n] = q;
  //

  for ( n=0; n<N; n++ )
  {
    D_rd = (const ae_f24 *)dline;
    D_wr = (      ae_f24 *)dline;
    C    = (const ae_f24 *)coef;

    // Q23 <- Q(23+8) - 8
    AE_L32F24_IP( s0, X, +4 );

    t0 = ( s0 );

    // Prescaling; q = g*x[n]
    // Q17.46 <- Q23*Q23
    u0 = AE_MULZAAD24_HH_LL( t_zg, t0 );

    q0 = ( u0 );

    // Load d[M-1], k[M-1].
    // Q1.22 <- Q(1.22+8) - 8
    AE_L32F24_IP( s1, D_rd, +4 );
    // Q23 <- Q(23+8) - 8
    AE_L32F24_IP( s2, C, +4 );

    // 0 k[M-1]
    select_f24x2_ll( s2, s_zz, s2 );

    // q -= k[M-1]*d[M-1]
    // Q17.46 <- Q23*Q1.22 + 1
    AE_MULSSFD24_HH_LL( q0, s1, s2 );

    for ( m=M-2; m>=0; m-- )
    {
      // Load d[m], k[m]
      // Q1.22 <- Q(1.22+8) - 8
      AE_L32F24_IP( s0, D_rd, +4 );
      // Q23 <- Q(23+8) - 8
      AE_L32F24_IP( s1, C, +4 );

      // 0 k[m]
      select_f24x2_ll( s1, s_zz, s1 );

      // q -= k[m]*d[m]
      // Q17.46 <- Q17.46 + [ Q23*Q1.22 + 1 ]
      AE_MULSSFD24_HH_LL( q0, s1, s0 );

      // Q1.22 <- Q17.46 - 24 w/ rounding and saturation
      s2 = AE_ROUNDSP24Q48ASYM( q0 );

      // k[m]*q
      // Q9.22 <- Q23*Q1.22 - 23 w/ rounding
      r0 = AE_MULFP24X2RA( s1, s2 );


      // d[m] + k[m]*q
      // Q1.22 <- sat24( Q1.22 + Q9.22 )
      s3 = AE_ADD24S( s0, r0 );

      // d[m+1] = d[m] + k[m]*q
      // Q(1.22+8) <- Q1.22 + 8
      AE_S32F24_L_IP( s3, D_wr, +4 );
    }

    // Q1.22 <- Q17.46 - 24 w/ rounding and saturation
    s0 = AE_ROUNDSP24Q48ASYM( q0 );

    // d[0] = q
    // Q(1.22+8) <- Q1.22 + 8
    AE_S32F24_L_IP( s0, D_wr, +4 );

    // Q16.47 <- Q17.46 + 1
    q1 = AE_F64_SLAIS( q0, 1 );

    // r[n] = q
    // Q(23+8) <- [ Q16.47 - 24 w/ rounding and saturation ] + 8
    AE_S24RA64S_IP( q1, R, +4 );

  #ifdef COMPILER_XTENSA
    #pragma flush_memory
  #endif
  }

} // latrX_proc()
