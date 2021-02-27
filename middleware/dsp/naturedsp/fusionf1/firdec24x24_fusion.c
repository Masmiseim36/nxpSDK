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
    Decimating block real FIR filter, 24x24-bit
    C code optimized for Fusion
*/

/*-------------------------------------------------------------------------
  Decimating Block Real FIR Filter
  Computes a real FIR filter (direct-form) with decimation using IR stored 
  in vector h. The real data input is stored in vector x. The filter output 
  result is stored in vector r. The filter calculates N output samples using
  M coefficients and requires last D*N+M-1 samples on the delay line.
  NOTE:
  - To avoid aliasing IR should be synthesized in such a way to be narrower 
    than input sample rate divided to 2D.
  - user application is not responsible for management of delay lines

  Precision: 
  24x24     24-bit data, 24-bit coefficients, 24-bit outputs
  32x16     32-bit data, 16-bit coefficients, 32-bit outputs
  f         floating point

  Input:
  h[M]          filter coefficients; h[0] is to be multiplied with the newest 
                sample, Q31, Q15, floating point
  D             decimation factor 
  N             length of output sample block
  M             length of filter
  x[D*N]        input samples , Q15, Q31 or floating point
  Output:
  y[N]          output samples, Q15, Q31 or floating point

  Restriction:
  x,h,r should not overlap
  x, h - aligned on an 8-bytes boundary
  N - multiple of 8
  D should exceed 1

  PERFORMANCE NOTE:
  for optimum performance follow rules:
  D - 2, 3 or 4
-------------------------------------------------------------------------*/
/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common utility and macros declarations. */
#include "common.h"

/* Instance pointer validation number. */
#define MAGIC     0x3953e987

/* Decimator instance structure. */
typedef struct 
{
  int32_t magic;
  int               M; /* Filter length                   */
  int               D; /* Interpolation/decimation factor */
  int          delLen; /* delay length                    */
  const int32_t * h; /* Filter coefficients               */
        int32_t * d; /* Delay line of length M            */
        int32_t * p; /* Pointer into the delay line       */
} firdec24x24_t;

/* Calculate the memory block size for a decimator with given attributes. */
size_t firdec24x24_alloc( int D, int M )
{
  int L;
  NASSERT( D > 1 && M > 0 );
  M=(M+1)&~1;
  L =(M+8*D + 3) & ~3;
  return sizeof(firdec24x24_t)+(M+L)*sizeof(int32_t)+7;

} // firdecf_alloc()

/* Initialize the decimator structure. The delay line is zeroed. */
firdecf_handle_t firdec24x24_init( void * objmem, int D, 
                               int M, const f24 * restrict h )
{
    firdec24x24_t* firdec;
    void *           ptr;
    int M1=(M+1)&~1;
    int L =(M1+8*D) & ~3;
    int32_t * restrict pd;
    int32_t * restrict ph;
    int m;
    NASSERT( objmem && D > 1 && M > 0 && h );
    NASSERT_ALIGN( h,8 );
    NASSERT(h);
    /* partition the memblock */
    ptr     = objmem;
    pd = (int32_t*)((((uintptr_t)ptr)+7)&~7);
    ph = pd+L;
    firdec=(firdec24x24_t*)(ph+M1);
    firdec->magic=MAGIC;
    firdec->M   = M1;
    firdec->D   = D;
    firdec->delLen=L;
    firdec->h = ph;
    firdec->d = firdec->p = pd;
    /* copy coefficients and clean up delay line */
    for (m=0; m<M; m++) ph[m]=h[m];
    for (;m<M1; m++) ph[m]=0;
    for (m=0; m<L; m++) pd[m]=0;
    return firdec;
} // firdec24x24_init()


/* 
    Decimator on 2:

    Input:
    h[M]    filter impluse response
    p       circular pointer in the delay line
    x[N*D]  input samples
    Input/output:
    d[L]  filter delay line
    Output:
    y[N]    output samples

    Returns: updated pointer to the delay line (p)

    Restrictions:
    N>0, N multiple of 8
    M>0, M multiple of 2
    D==2
    d,p,x aligned on 8 byte boundary
*/
static f24* firdec24x24_D2(
    const f24* h, int M, 
    f24* d, f24* p, int L,
    f24 * restrict y, const f24 * x, int N, int D )
{
    const ae_f24x2 *restrict pX;
          ae_f24x2 *restrict pY;
    const ae_f24x2* pD;
    const ae_f24x2* pH;
    ae_f24x2 * pWr;
    ae_valign ay,aD;
    ae_f24x2 X01,X23,X45,X67,H01;
    int n,m;
    NASSERT(h);
    NASSERT(d);
    NASSERT(p);
    NASSERT(x);
    NASSERT(y);
    NASSERT_ALIGN(x,8);
    NASSERT_ALIGN(d,8);
    NASSERT_ALIGN(p,8);
    NASSERT(N>0 && N%8==0);
    NASSERT(M>0 && M%2==0);
    NASSERT(D==2);

    pX=(const ae_f24x2 *)x;
    WUR_AE_CBEGIN0( (uintptr_t)( d    ) );
    WUR_AE_CEND0  ( (uintptr_t)( d + L) );
    ay=AE_ZALIGN64();
    pY=(ae_f24x2*)y;
    pWr=(ae_f24x2*)p;
    for (n=0; n<N; n+=4)
    {
        ae_f24x2 t; 
        ae_f64 q0,q1,q2,q3;
        q0=q1=q2=q3=AE_ZERO();
        pD=pWr;
        AE_L32F24_XC(t, castxcc(ae_f24, pD), 6 * sizeof(int32_t));
        pH=(const ae_f24x2*)h;

        AE_L32X2F24_IP(t,pX,2*sizeof(int32_t));  AE_S32X2F24_XC(t,pWr,2*sizeof(int32_t));
        AE_L32X2F24_IP(t,pX,2*sizeof(int32_t));  AE_S32X2F24_XC(t,pWr,2*sizeof(int32_t));
        AE_L32X2F24_IP(t,pX,2*sizeof(int32_t));  AE_S32X2F24_XC(t,pWr,2*sizeof(int32_t));
        AE_L32X2F24_IP(t,pX,2*sizeof(int32_t));  AE_S32X2F24_XC(t,pWr,2*sizeof(int32_t));
        AE_LA32X2F24NEG_PC(aD,pD);
        AE_LA32X2F24_RIC(X67,aD,pD);
        AE_LA32X2F24_RIC(X45,aD,pD);
        AE_LA32X2F24_RIC(X23,aD,pD);
        AE_LA32X2F24_RIC(X01,aD,pD);
        for (m=0; m<M; m+=2)
        {
            AE_L32X2F24_IP(H01,pH,2*sizeof(int32_t));
            AE_MULAAFD24_HH_LL(q0,X01,H01);
            AE_MULAAFD24_HH_LL(q1,X23,H01);
            AE_MULAAFD24_HH_LL(q2,X45,H01);
            AE_MULAAFD24_HH_LL(q3,X67,H01);
            X67=X45;
            X45=X23;
            X23=X01;
            AE_LA32X2F24_RIC(X01,aD,pD);
        }
        // Store 4 filter outputs.
        AE_SA32X2F24_IP( AE_ROUND24X2F48SASYM( q0, q1 ), ay, pY );
        AE_SA32X2F24_IP( AE_ROUND24X2F48SASYM( q2, q3 ), ay, pY );
    }
    AE_SA64POS_FP(ay,pY);
    return (int32_t*)pWr;
}

/* 
    Decimator on 3:

    Input:
    h[M]    filter impluse response
    p       circular pointer in the delay line
    x[N*D]  input samples
    Input/output:
    d[L]  filter delay line
    Output:
    y[N]    output samples

    Returns: updated pointer to the delay line (p)

    Restrictions:
    N>0, N multiple of 8
    M>0, M multiple of 2
    D==3
    d,p,x aligned on 8 byte boundary
*/
static f24* firdec24x24_D3(
    const f24* h, int M, 
    f24* d, f24* p, int L,
    f24 * restrict y, const f24 * x, int N, int D )
{
    const ae_f24x2 *restrict pX;
          ae_f24   *restrict pY;
    const ae_f24x2* pD0;
    const ae_f24x2* pD1;
    const ae_f24x2* pD2;
    const ae_f24x2* pD3;
    const ae_f24x2* pD4;
    const ae_f24x2* pD5;
    const ae_f24x2* pD6;
    const ae_f24x2* pD7;
    const ae_f24x2* pH;
    ae_f24x2 * pWr;
    ae_valign ay;
    ae_f24x2 X01,X23,X45,X67,H01;
    int m,n,j;
    NASSERT(h);
    NASSERT(d);
    NASSERT(p);
    NASSERT(x);
    NASSERT(y);
    NASSERT_ALIGN(x,8);
    NASSERT_ALIGN(d,8);
    NASSERT_ALIGN(p,8);
    NASSERT(N>0 && N%8==0);
    NASSERT(M>0 && M%2==0);
    NASSERT(D==3);

    pX=(const ae_f24x2 *)x;
    WUR_AE_CBEGIN0( (uintptr_t)( d    ) );
    WUR_AE_CEND0  ( (uintptr_t)( d + L) );
    ay=AE_ZALIGN64();
    pY=(ae_f24 *)y;
    pWr=(ae_f24x2*)p;
    for (n=0; n<N; n+=8)
    {
        ae_f64 q0,q1,q2,q3,q4,q5,q6,q7;
        ae_f24x2 t;
        ae_valign ad0,ad2,ad4,ad6;
        pH=(const ae_f24x2*)h;
        q0=q1=q2=q3=q4=q5=q6=q7=AE_ZERO();
        pD0=(ae_f24x2*)(pWr);
        for(j=0; j<4*D; j++) 
        {
             AE_L32X2F24_IP(t,pX,2*sizeof(int32_t));  
             AE_S32X2F24_XC(t,pWr,2*sizeof(int32_t));
        }
        pD1 = pD0; AE_L32F24_XC(t, castxcc(ae_f24, pD1), D*sizeof(int32_t));
        AE_LA32X2F24NEG_PC(ad0,pD1);
        pD3=pD1; AE_L32F24_XC(t,castxcc(ae_f24, pD3),2*D*sizeof(int32_t)); 
        pD5=pD3; AE_L32F24_XC(t,castxcc(ae_f24, pD5),2*D*sizeof(int32_t)); 
        pD7=pD5; AE_L32F24_XC(t,castxcc(ae_f24, pD7),2*D*sizeof(int32_t)); 
        pD2=pD0; AE_L32F24_XC(t,castxcc(ae_f24, pD2),2*D*sizeof(int32_t)); 
        pD4=pD2; AE_L32F24_XC(t,castxcc(ae_f24, pD4),2*D*sizeof(int32_t)); 
        pD6=pD4; AE_L32F24_XC(t,castxcc(ae_f24, pD6),2*D*sizeof(int32_t)); 

        AE_LA32X2F24NEG_PC(ad0,pD0);
        AE_LA32X2F24NEG_PC(ad2,pD2);
        AE_LA32X2F24NEG_PC(ad4,pD4);
        AE_LA32X2F24NEG_PC(ad6,pD6);
        for (m=0; m<M; m+=2)
        {
            AE_L32X2F24_IP(H01,pH,2*sizeof(int32_t));
            AE_LA32X2F24_RIC(X01,ad0,pD0); 
            AE_L32X2F24_RIC (X23,pD1);
            AE_LA32X2F24_RIC(X45,ad2,pD2); 
            AE_L32X2F24_RIC(X67,pD3);
            AE_MULAAFD24_HH_LL(q0,X01,H01);
            AE_MULAAFD24_HH_LL(q1,X23,H01);
            AE_MULAAFD24_HH_LL(q2,X45,H01);
            AE_MULAAFD24_HH_LL(q3,X67,H01);
            AE_LA32X2F24_RIC(X01,ad4,pD4); 
            AE_L32X2F24_RIC(X23,pD5);
            AE_LA32X2F24_RIC(X45,ad6,pD6); 
            AE_L32X2F24_RIC(X67,pD7);
            AE_MULAAFD24_HH_LL(q4,X01,H01);
            AE_MULAAFD24_HH_LL(q5,X23,H01);
            AE_MULAAFD24_HH_LL(q6,X45,H01);
            AE_MULAAFD24_HH_LL(q7,X67,H01);
        }
        AE_S24RA64S_IP(q0, pY, sizeof(int32_t));
        AE_S24RA64S_IP(q1, pY, sizeof(int32_t));
        AE_S24RA64S_IP(q2, pY, sizeof(int32_t));
        AE_S24RA64S_IP(q3, pY, sizeof(int32_t));
        AE_S24RA64S_IP(q4, pY, sizeof(int32_t));
        AE_S24RA64S_IP(q5, pY, sizeof(int32_t));
        AE_S24RA64S_IP(q6, pY, sizeof(int32_t));
        AE_S24RA64S_IP(q7, pY, sizeof(int32_t));
    }
    return (int32_t*)pWr;
}

/* 
    Decimator on 4:

    Input:
    h[M]    filter impluse response
    p       circular pointer in the delay line
    x[N*D]  input samples
    Input/output:
    d[L]  filter delay line
    Output:
    y[N]    output samples

    Returns: updated pointer to the delay line (p)

    Restrictions:
    N>0, N multiple of 8
    M>0, M multiple of 2
    D==4
    d,p,x aligned on 8 byte boundary
*/
static f24* firdec24x24_D4(
    const f24* h, int M, 
    f24* d, f24* p, int L,
    f24 * restrict y, const f24 * x, int N, int D )
{
    const ae_f24x2 *restrict pX;
          ae_f24x2 *restrict pY;
    const ae_f24x2* pD;
    const ae_f24x2* pH;
    ae_f24x2 * pWr;
    ae_valign ay,aD;
    ae_f24x2 X01,X23,X89,Xab,H01,H23;
    int m,n;
    NASSERT(h);
    NASSERT(d);
    NASSERT(p);
    NASSERT(x);
    NASSERT(y);
    NASSERT_ALIGN(x,8);
    NASSERT_ALIGN(d,8);
    NASSERT_ALIGN(p,8);
    NASSERT(N>0 && N%8==0);
    NASSERT(M>0 && M%2==0);
    NASSERT(D==4);

    pX=(const ae_f24x2 *)x;
    WUR_AE_CBEGIN0( (uintptr_t)( d    ) );
    WUR_AE_CEND0  ( (uintptr_t)( d + L) );
    ay=AE_ZALIGN64();
    pY=(ae_f24x2*)y;
    pWr=(ae_f24x2*)p;
    for (n=0; n<N; n+=2)
    {
        ae_f24x2 t;
        ae_f64 q0,q1;
        q0=q1=AE_ZERO();
        pD=(ae_f24x2*)pWr;
        AE_L32F24_XC(t, castxcc(ae_f24, pD), 4 * sizeof(int32_t));
        pH=(const ae_f24x2*)h;
        AE_L32X2F24_IP(t,pX,2*sizeof(int32_t));  AE_S32X2F24_XC(t,pWr,2*sizeof(int32_t));
        AE_L32X2F24_IP(t,pX,2*sizeof(int32_t));  AE_S32X2F24_XC(t,pWr,2*sizeof(int32_t));
        AE_L32X2F24_IP(t,pX,2*sizeof(int32_t));  AE_S32X2F24_XC(t,pWr,2*sizeof(int32_t));
        AE_L32X2F24_IP(t,pX,2*sizeof(int32_t));  AE_S32X2F24_XC(t,pWr,2*sizeof(int32_t));
        AE_LA32X2F24NEG_PC(aD,pD);
        AE_LA32X2F24_RIC(X23,aD,pD); AE_LA32X2F24_RIC(Xab,aD,pD);
        AE_LA32X2F24_RIC(X01,aD,pD); AE_LA32X2F24_RIC(X89,aD,pD);
        for (m=0; m<(M & ~3); m+=4)
        {
            AE_L32X2F24_IP(H01,pH,2*sizeof(int32_t));
            AE_L32X2F24_IP(H23,pH,2*sizeof(int32_t));
            AE_MULAAFD24_HH_LL(q0,X01,H01);
            AE_MULAAFD24_HH_LL(q1,X23,H01);
            AE_MULAAFD24_HH_LL(q0,X89,H23);
            AE_MULAAFD24_HH_LL(q1,Xab,H23);
            X23=X01; Xab=X89; 
            AE_LA32X2F24_RIC(X01,aD,pD); AE_LA32X2F24_RIC(X89,aD,pD);
        }
        if (M&2)
        {
            AE_L32X2F24_IP(H01,pH,2*sizeof(int32_t));
            AE_MULAAFD24_HH_LL(q0,X01,H01);
            AE_MULAAFD24_HH_LL(q1,X23,H01);
        }
        // Store 2 filter outputs.
        AE_SA32X2F24_IP( AE_ROUND24X2F48SASYM( q0, q1 ), ay, pY );
    }
    AE_SA64POS_FP(ay,pY);
    return (int32_t*)pWr;
}


/* 
    Decimator on D:

    Input:
    h[M]    filter impluse response
    p       circular pointer in the delay line
    x[N*D]  input samples
    Input/output:
    d[L]  filter delay line
    Output:
    y[N]    output samples

    Returns: updated pointer to the delay line (p)

    Restrictions:
    N>0, N multiple of 8
    M>0, M multiple of 2
    D>1
    d,p,x aligned on 8 byte boundary
*/
static f24* firdec24x24_Dx(
    const f24* h, int M, 
    f24* d, f24* p, int L,
    f24 * restrict y, const f24 * x, int N, int D )
{
    const ae_f24x2 *restrict pX;
          ae_f24   *restrict pY;
    const ae_f24x2* pD0;
    const ae_f24x2* pD1;
    const ae_f24x2* pD2;
    const ae_f24x2* pD3;
    const ae_f24x2* pH;
    ae_f24x2 * pWr;
    ae_valign ay;
    ae_f24x2 X01,X23,X45,X67,H01;
    int m,n,j;
    NASSERT(h);
    NASSERT(d);
    NASSERT(p);
    NASSERT(x);
    NASSERT(y);
    NASSERT_ALIGN(x,8);
    NASSERT_ALIGN(d,8);
    NASSERT_ALIGN(p,8);
    NASSERT(N>0 && N%8==0);
    NASSERT(M>0 && M%2==0);
    NASSERT(D>1);

    pX=(const ae_f24x2 *)x;
    WUR_AE_CBEGIN0( (uintptr_t)( d    ) );
    WUR_AE_CEND0  ( (uintptr_t)( d + L) );
    ay=AE_ZALIGN64();
    pY=(ae_f24 *)y;
    pWr=(ae_f24x2*)p;
    for (n=0; n<N; n+=4)
    {
        ae_f64 q0,q1,q2,q3;
        ae_f24x2 t;
        ae_valign ad0,ad1,ad2,ad3;
        pH=(const ae_f24x2*)h;
        q0=q1=q2=q3=AE_ZERO();
        pD0=(ae_f24x2*)(pWr);
        for(j=0; j<2*D; j++) 
        {
             AE_L32X2F24_IP(t,pX,2*sizeof(int32_t));  
             AE_S32X2F24_XC(t,pWr,2*sizeof(int32_t));
        }
        pD1=pD0; AE_L32F24_XC(t,castxcc(ae_f24, pD1),D*sizeof(int32_t)); 
        pD2=pD1; AE_L32F24_XC(t,castxcc(ae_f24, pD2),D*sizeof(int32_t)); 
        pD3=pD2; AE_L32F24_XC(t,castxcc(ae_f24, pD3),D*sizeof(int32_t)); 

        AE_LA32X2F24NEG_PC(ad0,pD0);
        AE_LA32X2F24NEG_PC(ad1,pD1);
        AE_LA32X2F24NEG_PC(ad2,pD2);
        AE_LA32X2F24NEG_PC(ad3,pD3);
        for (m=0; m<M; m+=2)
        {
            AE_LA32X2F24_RIC(X01,ad0,pD0); 
            AE_LA32X2F24_RIC(X23,ad1,pD1);
            AE_LA32X2F24_RIC(X45,ad2,pD2); 
            AE_LA32X2F24_RIC(X67,ad3,pD3);
            AE_L32X2F24_IP(H01,pH,2*sizeof(int32_t));
            AE_MULAAFD24_HH_LL(q0,X01,H01);
            AE_MULAAFD24_HH_LL(q1,X23,H01);
            AE_MULAAFD24_HH_LL(q2,X45,H01);
            AE_MULAAFD24_HH_LL(q3,X67,H01);
        }
        AE_S24RA64S_IP(q0, pY, sizeof(int32_t));
        AE_S24RA64S_IP(q1, pY, sizeof(int32_t));
        AE_S24RA64S_IP(q2, pY, sizeof(int32_t));
        AE_S24RA64S_IP(q3, pY, sizeof(int32_t));
    }
    return (int32_t*)pWr;
}

typedef f24* (*tfun)(
    const f24* h, int M, 
    f24* d, f24* p, int L,
    f24 * y, const f24 * x, int N, int D );


/* process block of samples */
void firdec24x24_process( firdecf_handle_t _firdec, 
                          f24 * restrict       y, 
                    const f24 *                x, int N )
{
    static const tfun fxns[]={firdec24x24_D2,firdec24x24_D3,firdec24x24_D4};
    firdec24x24_t *state;
    int M,D,L;
    tfun fun;

    NASSERT(_firdec);
    NASSERT_ALIGN(x,8);
    NASSERT(N%8==0);
    NASSERT(x);
    NASSERT(y);
    if(N<=0) return;
    state = (firdec24x24_t*)_firdec;
    NASSERT(state->magic==MAGIC);
    NASSERT(state->h);
    NASSERT(state->d);
    NASSERT(state->p);
    NASSERT_ALIGN(state->d,8);
    NASSERT_ALIGN(state->p,8);
    M=state->M;
    D=state->D;
    NASSERT(N>0);
    NASSERT(M>0);
    NASSERT(D>1);
    NASSERT(M%2==0);
    L=state->delLen;
    fun = (D<=4) ? fxns[D-2] : &firdec24x24_Dx;
    state->p=fun(state->h,M,state->d,state->p,L,y,x,N,D);
} // firdec24x24_process()
