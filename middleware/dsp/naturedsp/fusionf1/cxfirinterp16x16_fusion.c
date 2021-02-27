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
  NatureDSP Signal Processing Library. FIR part
    Interpolating block real FIR filter, 16x16-bit
*/

/*-------------------------------------------------------------------------
  Interpolating Block Real/Complex FIR Filter
  Computes a real FIR filter (direct-form) with interpolation using IR stored 
  in vector h. The real data input is stored in vector x. The filter output 
  result is stored in vector y. The filter calculates N*D output samples 
  using M*D coefficients and requires last N+M*D-1 samples on the delay line.
  NOTE:
  user application is not responsible for management of delay lines

  Precision: 
  16x16     16-bit real data, 16-bit coefficients, 16-bit real outputs 
  16x16     16-bit complex data, 16-bit coefficients, 16-bit complex outputs
  24x24     24-bit real data, 24-bit coefficients, 24-bit real outputs 
  32x16     32-bit real data, 16-bit coefficients, 32-bit real outputs 
  32x32     32-bit real data, 32-bit coefficients, 32-bit real outputs 
  f         floating point

  Input:
  h[M*D]        filter coefficients; h[0] is to be multiplied with the 
                newest sample,Q31, Q15, floating point
  D             interpolation ratio
  N             length of input sample block
  M             length of subfilter. Total length of filter is M*D
  x[N]          input samples,Q15, Q31 or floating point
  Output:
  y[N*D]        output samples, Q15, Q31 or floating point

  Restrictions:
  x,h,y should not overlap
  x,h - aligned on an 8-bytes boundary
  N   - multiple of 8
  M   - multiple of 4
  D should be >1

  PERFORMANCE NOTE:
  for optimum performance follow rules:
  D   - 2, 3 or 4

-------------------------------------------------------------------------*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common utility and macros declarations. */
#include "common.h"
#include "baseop.h"

#include "cxfirinterp16x16_common.h"

/* Instance pointer validation number. */
#define MAGIC     0x5FE2D896

/* Reserve memory for alignment. */
#define ALIGNED_SIZE( size, align ) \
    ((size_t)(size)+(align)-1)

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
    (void*)(((uintptr_t)(addr)+((align)-1)) & ~((align)-1))

#define sz_i16    sizeof(int16_t)


#if !(XCHAL_HAVE_FUSION_16BIT_BASEBAND)
/* code with no Quad MAC option */

/* Interpolator instance structure. */
typedef struct tag_cxfirinterp16x16_t
{
  int32_t magic;
  int               M; /* Filter length                   */
  int               D; /* Interpolation/decimation factor */
  const int16_t   * h; /* Filter coefficients (M*D)       */
        int32_t   * d[2]; /* Delay lines of length (M)      */
        int32_t   * p[2]; /* Pointer into the delay lines     */
} cxfirinterp16x16_t;

/* Calculate the memory block size for an interpolator with given 
 * attributes. */
size_t cxfirinterp16x16_alloc( int D, int M )
{
  NASSERT( D > 1 && M > 0 );
  NASSERT( M%4==0);
  return M*D*sizeof(int16_t) + 2*M*sizeof(int32_t) + sizeof(cxfirinterp16x16_t) + 7;
} // cxfirinterp16x16_alloc()

/* Initialize the interpolator structure. The delay line is zeroed. */
cxfirinterp16x16_handle_t cxfirinterp16x16_init( void * objmem, 
                                             int D, int M, 
                                             const int16_t * restrict h )
{
    cxfirinterp16x16_t* firinterp;
    void *           ptr;
    int32_t * restrict pd;
    int16_t * restrict ph;
    int m,d;
    NASSERT( D > 1 && M > 0 );
    NASSERT( M%4==0);
    NASSERT(h);
    NASSERT(objmem);
    NASSERT_ALIGN(h,8);
    /* partition the memblock */
    ptr     = objmem;
    ph = (int16_t*)((((uintptr_t)ptr)+7)&~7);
    pd = (int32_t*)(ph+M*D);
    firinterp=(cxfirinterp16x16_t*)(pd+2*M);
    firinterp->magic=MAGIC;
    firinterp->M   = M;
    firinterp->D   = D;
    firinterp->h = ph;
    firinterp->d[0] = firinterp->p[0] = pd;
    firinterp->d[1] = firinterp->p[1] = pd+M;
    /* copy/rearrange coefficients and clean up delay line */
    for (m=0; m<M; m++) 
    for (d=0; d<D; d++) 
        ph[d*M+m]=h[m*D+d];
    for (m=0; m<2*M; m++) pd[m]=0;
    return firinterp;
} // cxfirinterp16x16_init()


/* Put a chunk of input signal into the delay line and compute the filter
 * response. */
void cxfirinterp16x16_process( cxfirinterp16x16_handle_t _cxfirinterp, 
                             complex_fract16 * restrict      y,
                       const complex_fract16 * restrict      x, int N )
{
    const ae_int16x4* restrict pX;
    const ae_int32x2* restrict pX0;
    const ae_int32x2* restrict pX1;
    const int32_t* restrict pH;
    ae_int32x2* pWr;
    cxfirinterp16x16_t *state;
    int reim,n, m, M, D, j;
    const int16_t* h;
    const int16_t *y0;
    const int16_t *x0;
    ae_int16* pY;
    int g_exp;
    int q_frac_int ;
    ae_p24x2s g_frac;
    ae_int32x2 X01,X23,X12,X34,t;
    ae_f16x4 H01,H23;
    ae_valign ax,aX0,aX1;
    ae_f64 q0,q1,q2,q3;
    ae_int16x4 XX;

    NASSERT(_cxfirinterp);
    state=(cxfirinterp16x16_t *)_cxfirinterp;
    NASSERT(x);
    NASSERT(y);
    NASSERT(state);
    NASSERT(state->magic==MAGIC);
    NASSERT_ALIGN(state->d,8);
    M = state->M;
    D = state->D;
    if(N<=0) return;
    NASSERT(N>0);
    NASSERT(M>0);
    NASSERT(D>1);
    NASSERT(M%2==0);
    h = state->h;
    /* compute scale factor represented as mantissa and exponent */
    g_exp  = 31 - AE_NSAZ32_L( D );
    q_frac_int =  D << ( 15 + 8 - g_exp );
    g_frac = *(ae_p24s *) &q_frac_int;
    WUR_AE_SAR( g_exp );
    y0=(int16_t*)y;
    x0=(int16_t*)x;
    for (reim=0; reim<2; reim++)
    {
        y=(complex_fract16*)(y0+reim);
        pWr=(ae_int32x2*)state->p[reim];
        pX =(const ae_int16x4*)(x0+reim);
        /* setup circular buffer boundaries */
        WUR_AE_CBEGIN0( (uintptr_t)( state->d[reim]     ) );
        WUR_AE_CEND0  ( (uintptr_t)( state->d[reim] + M ) );
        /* process by 4 samples */
        for (n = 0; n<N; n += 4)
        {
            pH =(int32_t*)h;
            pY =(ae_int16*)y;
            for (j = 0; j<D; j++)
            {
                /* filtering loop */
                pX0=(const ae_int32x2*)(pWr);
                pX1=(const ae_int32x2*)(((int32_t*)pWr)+1);
                AE_LA32X2NEG_PC(aX0,pX0);
                AE_LA32X2NEG_PC(aX1,pX1);
                AE_LA32X2_RIC(X12,aX1,pX1);
                AE_LA32X2_RIC(t  ,aX0,pX0);
                {   /* take 2 vectors and extract even elements */
                    const ae_int16x4 *pXX=pX;
                    ae_int16x4 XX0,XX1;
                    ax=AE_LA64_PP(pXX);
                    AE_LA16X4_IP(XX0,ax,pXX);
                    AE_LA16X4_IP(XX1,ax,pXX);
                    XX=AE_SEL16_7531(XX0,XX1);
                }
                X12=AE_CVT32X2F16_32(XX);
                X34=AE_CVT32X2F16_10(XX);
                X12=(AE_INTSWAP(X12));
                X34=(AE_INTSWAP(X34));
                X01=AE_SEL32_LL(X12,t);
                X23=AE_SEL32_LH(X34,X12);

                AE_L32_IP(t,castxcc(ae_int32,pH),sizeof(int32_t)); H01=AE_MOVF16X4_FROMINT32X2(t);
                q0=q1=AE_ZERO();
                q2=AE_MULZAAFD32X16_H2_L3(X23,H01);
                q3=AE_MULZAAFD32X16_H2_L3(X34,H01);
                __Pragma("loop_count min=1")
                for (m=0; m<M-2; m+=2)
                {
                  AE_L32_IP(t, castxcc(ae_int32, pH), sizeof(int32_t)); H23 = AE_MOVF16X4_FROMINT32X2(t);
                    AE_MULAAFD32X16_H2_L3(q0,X01,H01);
                    AE_MULAAFD32X16_H2_L3(q1,X12,H01);
                    AE_MULAAFD32X16_H2_L3(q2,X01,H23);
                    AE_MULAAFD32X16_H2_L3(q3,X12,H23);
                    H01=H23;
                    AE_LA32X2_RIC(X01,aX0,pX0);
                    AE_LA32X2_RIC(X12,aX1,pX1);
                }
                AE_MULAAFD32X16_H2_L3(q0,X01,H01);
                AE_MULAAFD32X16_H2_L3(q1,X12,H01);
                /* scale qx by D and save */
                q0 = AE_SLAS64S(AE_MULF48Q32SP16S_L( AE_ROUNDSQ32F48ASYM(q0), g_frac ));
                q1 = AE_SLAS64S(AE_MULF48Q32SP16S_L( AE_ROUNDSQ32F48ASYM(q1), g_frac ));
                q2 = AE_SLAS64S(AE_MULF48Q32SP16S_L( AE_ROUNDSQ32F48ASYM(q2), g_frac ));
                q3 = AE_SLAS64S(AE_MULF48Q32SP16S_L( AE_ROUNDSQ32F48ASYM(q3), g_frac ));
                {
                    ae_int32x2 t0,t1,t2,t3;
                    t0=AE_TRUNCA32X2F64S(q0,q0, 16);
                    t1=AE_TRUNCA32X2F64S(q1,q1, 16);
                    t2=AE_TRUNCA32X2F64S(q2,q2, 16);
                    t3=AE_TRUNCA32X2F64S(q3,q3, 16);
                    XX = AE_ROUND16X4F32SASYM(t0,t0); AE_S16_0_XP(XX,pY,2*sizeof(int16_t)*D      );
                    XX = AE_ROUND16X4F32SASYM(t1,t1); AE_S16_0_XP(XX,pY,2*sizeof(int16_t)*D      );
                    XX = AE_ROUND16X4F32SASYM(t2,t2); AE_S16_0_XP(XX,pY,2*sizeof(int16_t)*D      );
                    XX = AE_ROUND16X4F32SASYM(t3,t3); AE_S16_0_XP(XX,pY,  sizeof(int16_t)*(2-6*D));
                }
            }
            /* update delay line */
            {   /* take 2 vectors and extract even elements */
                ae_int16x4 XX0,XX1;
                ax=AE_LA64_PP(pX);
                AE_LA16X4_IP(XX0,ax,pX);
                AE_LA16X4_IP(XX1,ax,pX);
                XX=AE_SEL16_7531(XX0,XX1);
            }
            X01=AE_CVT32X2F16_32(XX);
            X23=AE_CVT32X2F16_10(XX);
            AE_S32X2_XC(X01,pWr,sizeof(int32_t)*2);
            AE_S32X2_XC(X23,pWr,sizeof(int32_t)*2);
            state->p[reim] =(int32_t*)pWr;
            y+=4*D;
        }
        state->p[reim] =(int32_t*)pWr;
    }
} // cxfirinterp16x16_process()
#else
/* code with Quad MAC option */

/* Interpolator instance structure. */
typedef struct tag_cxfirinterp16x16_t
{
    uint32_t        magic;     // Instance pointer validation number
    int             D;         // Interpolation factor
    int             M;         // Number of filter coefficients
    proc_fxn_t    * procFxn;   // Filter data processing function
    const int16_t * coef;      // Filter coefficients
    int16_t *       delayLine; // Delay line for complex samples
    int             delayLen;  // Delay line length, in complex samples
    int             wrIx;      // Index of the oldest sample

} cxfirinterp16x16_t, *cxfirinterp16x16_ptr_t;

/* Calculate the memory block size for an interpolator with given
* attributes. */
size_t cxfirinterp16x16_alloc(int D, int M)
{
    int delayLen, coefNum;
    NASSERT(D > 1 && M > 0);

    NASSERT(!(M & 3));

    delayLen = (M + 8);
    coefNum = (M + 8)*D;

    return (ALIGNED_SIZE(sizeof(cxfirinterp16x16_t), 4)
        + // Delay line
        // Allocated memory for delay line has increased by 2 samples
        // to avoid memory bank conflicts and get the best performance
        ALIGNED_SIZE(2 * (delayLen + 2)*sz_i16, 8)
        + // Coefficients
        ALIGNED_SIZE(coefNum*sz_i16, 8));
} // cxfirinterp16x16_alloc();


/* Initialize the interpolator structure. The delay line is zeroed. */
cxfirinterp16x16_handle_t cxfirinterp16x16_init( void * objmem,
                                                 int D, int M,
                                           const int16_t * restrict h)
{
    cxfirinterp16x16_ptr_t firinterp;
    void            *    ptr;
    int16_t         *    delLine;
    int                  delLen;
    int16_t         *    coef;
    int16_t         *    coefB;
    int d, m;
    proc_fxn_t      *    procFxn;
    NASSERT(objmem && D > 1 && M > 0 && h);

    NASSERT(!(M & 3) && IS_ALIGN(h));

    delLen = (M + 8);

    procFxn = ( D == 2 ? &cxfirinterp16x16_D2_proc :
                D == 3 ? &cxfirinterp16x16_D3_proc :
                D == 4 ? &cxfirinterp16x16_D4_proc :
                         &cxfirinterp16x16_DX_proc);

  
    //
    // Partition the memory block.
    //
    ptr         = objmem;
    firinterp   = (cxfirinterp16x16_ptr_t)ALIGNED_ADDR(ptr, 4);
    ptr         = firinterp + 1;
    delLine     = (int16_t*)ALIGNED_ADDR(ptr, 8);
    // Allocated memory for delay line has increased by 2 samples
    // to avoid memory bank conflicts and get the best performance
    ptr         = delLine + 2*(delLen + 2);
    coef        = (int16_t*)ALIGNED_ADDR(ptr, 8);
    ptr         = coef + M*D;

    NASSERT((int8_t*)ptr - (int8_t*)objmem <=
        (int)cxfirinterp16x16_alloc(D, M));


    //
    // Break the impulse response into D coefficients banks and copy them in
    // reverted order.
    //

    for (d = 0; d<D; d++)
    {
        coefB = coef    + d*(M + 4);

        // To avoid a 1-sample delay, insert a zero coefficient that will match the
        // oldest sample in the delay line. To keep the filter length a multiple of
        // 4, append 3 zeros after the last coefficient.
        coefB[0]     = coefB[M + 1] = coefB[M + 2] = coefB[M + 3] = 0;
        //coefB[M + 4] = coefB[M + 5] = coefB[M + 6] = coefB[M + 7] = 0;

        // Copy bank's coefficients in reverted order.
        for (m = 1; m<M + 1; m++)
        {
            coefB[m] = h[D*(M - m) + d];
        }
    }

    //
    // Zero the delay line.
    //

    for (m = 0; m<delLen; m++)
    {
        delLine[m ] = 0;
        delLine[delLen + m] = 0;
    }


    //
    // Initialize the interpolator instance.
    //

    firinterp->magic     = MAGIC;
    firinterp->D         = D;
    firinterp->M         = M;
    firinterp->coef      = coef;
    firinterp->procFxn   = procFxn;
    firinterp->delayLine = delLine;
    firinterp->delayLen  = delLen;
    firinterp->wrIx      = 0;

    return (firinterp);
} // cxfirinterp16x16_init()

/* Put a chunk of input signal into the delay line and compute the filter
* response. */
void cxfirinterp16x16_process( cxfirinterp16x16_handle_t _cxfirinterp,
                               complex_fract16 * restrict      y,
                         const complex_fract16 * restrict      x, int N)
{
    cxfirinterp16x16_ptr_t firinterp = (cxfirinterp16x16_ptr_t)_cxfirinterp;
    NASSERT(firinterp && firinterp->magic == MAGIC && y && x);
    if (N <= 0) return;

    NASSERT(N % 8 == 0);
    NASSERT(IS_ALIGN(x));
    //
    // Call filter's data processing function. It will store the block of input 
    // samples to the delay line, and compute the filter response. Returns the
    // updated next position pointer into the delay line buffer.
    //

    NASSERT(firinterp->procFxn);

    firinterp->wrIx = (*firinterp->procFxn)(
        (int16_t *)y,
        firinterp->delayLine,
        firinterp->delayLen,
        (const int16_t *)x,
        firinterp->coef,
        firinterp->wrIx,
        firinterp->D,
        N,
        firinterp->M);
} // cxfirinterp16x16_process()
#endif
