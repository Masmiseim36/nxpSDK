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
    Decimating block real FIR filter, 32x16-bit
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
#define MAGIC     0xed1722f0

/* Decimator instance structure. */
typedef struct 
{
  int32_t magic;
  int               M; /* Filter length                   */
  int               D; /* Interpolation/decimation factor */
  int          delLen; /* delay length                    */
  const int16_t * h; /* Filter coefficients               */
        int32_t * d; /* Delay line of length M            */
        int32_t * p; /* Pointer into the delay line       */
} firdec32x16_t;

/* Calculate the memory block size for a decimator with given attributes. */
size_t firdec32x16_alloc( int D, int M )
{
  NASSERT( D > 1 && M > 0 );
  M=(M+3)&~3;
  return sizeof(firdec32x16_t)+(M+M+4*D)*sizeof(int32_t)+7;

} // firdec32x16_alloc()


/* Initialize the decimator structure. The delay line is zeroed. */
firdecf_handle_t firdec32x16_init( void * objmem, int D, 
                               int M, const int16_t * restrict h )
{
    firdec32x16_t* firdec;
    void *           ptr;
    int M1=(M+3)&~3;
    int L=M1+4*D;
    int32_t * restrict pd;
    int16_t * restrict ph;
    int m;
    NASSERT( objmem && D > 1 && M > 0 && h );
    NASSERT_ALIGN( h,8 );
    NASSERT(h);
    /* partition the memblock */
    ptr     = objmem;
    pd = (int32_t*)((((uintptr_t)ptr)+7)&~7);
    ph = (int16_t*)(pd+L);
    firdec=(firdec32x16_t*)(ph+M1);
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
} // firdec32x26_init()

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
    M>0, M multiple of 4
    D==2
    d,p,x aligned on 8 byte boundary
*/
static int32_t* firdec32x16_D2(
    const int16_t* h, int M, 
    int32_t* d, int32_t* p, int L,
    int32_t * restrict y, const int32_t * x, int N, int D )
{
    const ae_int32x2 *restrict pX;
          ae_f32x2   *restrict pY;
    const ae_int32x2* pD;
    const int32_t* pH;
    ae_int32x2 * pWr;
    ae_valign aD;
    ae_int32x2 X01,X23,X45,X67;
    ae_f16x4 H01;
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
    NASSERT(M>0 && M%4==0);
    NASSERT(D==2);

    pX=(const ae_int32x2 *)x;
    WUR_AE_CBEGIN0( (uintptr_t)( d    ) );
    WUR_AE_CEND0  ( (uintptr_t)( d + L) );
    pY=(ae_f32x2*)y;
    pWr=(ae_int32x2*)p;
    for (n=0; n<N; n+=4)
    {
        ae_int32x2 t; 
        ae_f64 q0,q1,q2,q3;
        q0=q1=q2=q3=AE_ZERO();
        pD=pWr;
        AE_L32_XC(t, castxcc(ae_int32, pD), 6 * sizeof(int32_t));
        pH=(const int32_t*)h;

        AE_L32X2_IP(t,pX,2*sizeof(int32_t));  AE_S32X2_XC(t,pWr,2*sizeof(int32_t));
        AE_L32X2_IP(t,pX,2*sizeof(int32_t));  AE_S32X2_XC(t,pWr,2*sizeof(int32_t));
        AE_L32X2_IP(t,pX,2*sizeof(int32_t));  AE_S32X2_XC(t,pWr,2*sizeof(int32_t));
        AE_L32X2_IP(t,pX,2*sizeof(int32_t));  AE_S32X2_XC(t,pWr,2*sizeof(int32_t));
        AE_LA32X2NEG_PC(aD,pD);
        AE_LA32X2_RIC(X67,aD,pD);
        AE_LA32X2_RIC(X45,aD,pD);
        AE_LA32X2_RIC(X23,aD,pD);
        AE_LA32X2_RIC(X01,aD,pD);
        for (m=0; m<M; m+=2)
        {
            AE_L32_IP(t,castxcc(ae_int32, pH),sizeof(int32_t)); H01=AE_MOVF16X4_FROMINT32X2(t);
            AE_MULAAFD32X16_H2_L3(q0,X01,H01);
            AE_MULAAFD32X16_H2_L3(q1,X23,H01);
            AE_MULAAFD32X16_H2_L3(q2,X45,H01);
            AE_MULAAFD32X16_H2_L3(q3,X67,H01);
            X67=X45;
            X45=X23;
            X23=X01;
            AE_LA32X2_RIC(X01,aD,pD);
        }
        // Store 4 filter outputs.
        AE_S32RA64S_IP(q0,castxcc(ae_f32, pY),4);
        AE_S32RA64S_IP(q1,castxcc(ae_f32, pY),4);
        AE_S32RA64S_IP(q2,castxcc(ae_f32, pY),4);
        AE_S32RA64S_IP(q3,castxcc(ae_f32, pY),4);
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
    M>0, M multiple of 4
    D==4
    d,p,x aligned on 8 byte boundary
*/
static int32_t* firdec32x16_D4(
    const int16_t* h, int M, 
    int32_t* d, int32_t* p, int L,
    int32_t * restrict y, const int32_t * x, int N, int D )
{
    const ae_int32x2 *restrict pX;
          ae_f32x2 *restrict pY;
    const ae_int32x2* pD;
    const ae_f16x4* pH;
    ae_int32x2 * pWr;
    ae_valign aD;
    ae_int32x2 X01,X23,X89,Xab;
    ae_int16x4 H0123;
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
    NASSERT(M>0 && M%4==0);
    NASSERT(D==4);

    pX=(const ae_int32x2 *)x;
    WUR_AE_CBEGIN0( (uintptr_t)( d    ) );
    WUR_AE_CEND0  ( (uintptr_t)( d + L) );
    pY=(ae_f32x2*)y;
    pWr=(ae_int32x2*)p;
    for (n=0; n<N; n+=2)
    {
        ae_int32x2 t;
        ae_f64 q0,q1;
        q0=q1=AE_ZERO();
        pD=(ae_int32x2*)pWr;
        AE_L32_XC(t, castxcc(ae_int32, pD), 4 * sizeof(int32_t));
        pH=(const ae_f16x4*)h;
        AE_L32X2_IP(t,pX,2*sizeof(int32_t));  AE_S32X2_XC(t,pWr,2*sizeof(int32_t));
        AE_L32X2_IP(t,pX,2*sizeof(int32_t));  AE_S32X2_XC(t,pWr,2*sizeof(int32_t));
        AE_L32X2_IP(t,pX,2*sizeof(int32_t));  AE_S32X2_XC(t,pWr,2*sizeof(int32_t));
        AE_L32X2_IP(t,pX,2*sizeof(int32_t));  AE_S32X2_XC(t,pWr,2*sizeof(int32_t));
        AE_LA32X2NEG_PC(aD,pD);
        AE_LA32X2_RIC(X23,aD,pD); AE_LA32X2_RIC(Xab,aD,pD);
        AE_LA32X2_RIC(X01,aD,pD); AE_LA32X2_RIC(X89,aD,pD);
        for (m=0; m<M; m+=4)
        {
            AE_L16X4_IP(H0123,pH,4*sizeof(int16_t));
            AE_MULAAFD32X16_H3_L2(q0,X01,H0123);
            AE_MULAAFD32X16_H3_L2(q1,X23,H0123);
            AE_MULAAFD32X16_H1_L0(q0,X89,H0123);
            AE_MULAAFD32X16_H1_L0(q1,Xab,H0123);
            X23=X01; Xab=X89; 
            AE_LA32X2_RIC(X01,aD,pD); AE_LA32X2_RIC(X89,aD,pD);
        }
        // Store 2 filter outputs.
        AE_S32RA64S_IP(q0,castxcc(ae_f32, pY),4);
        AE_S32RA64S_IP(q1,castxcc(ae_f32, pY),4);
    }
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
static int32_t* firdec32x16_Dx(
    const int16_t* h, int M, 
    int32_t* d, int32_t* p, int L,
    int32_t * restrict y, const int32_t * x, int N, int D )
{ 
    const ae_int32x2 *restrict pX;
          ae_f32   *restrict pY;
    const ae_int32x2* pD0;
    const ae_int32x2* pD1;
    const ae_int32x2* pD2;
    const ae_int32x2* pD3;
    const ae_int16x4* pH;
    ae_int32x2 * pWr;
    ae_int32x2 X01,X23,X45,X67;
    ae_int16x4 H01;
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

    pX=(const ae_int32x2 *)x;
    WUR_AE_CBEGIN0( (uintptr_t)( d    ) );
    WUR_AE_CEND0  ( (uintptr_t)( d + L) );
    pY=(ae_f32 *)y;
    pWr=(ae_int32x2*)p;
    for (n=0; n<N; n+=4)
    {
        ae_f64 q0,q1,q2,q3;
        ae_int32x2 t;
        ae_valign ad0,ad1,ad2,ad3;
        pH=(const ae_int16x4*)h;
        q0=q1=q2=q3=AE_ZERO();
        pD0=(ae_int32x2*)(pWr);
        for(j=0; j<2*D; j++) 
        {
             AE_L32X2_IP(t,pX,2*sizeof(int32_t));  
             AE_S32X2_XC(t,pWr,2*sizeof(int32_t));
        }
        pD1=pD0; AE_L32_XC(t,castxcc(ae_int32, pD1),D*sizeof(int32_t)); 
        pD2=pD1; AE_L32_XC(t,castxcc(ae_int32, pD2),D*sizeof(int32_t)); 
        pD3=pD2; AE_L32_XC(t,castxcc(ae_int32, pD3),D*sizeof(int32_t)); 

        AE_LA32X2NEG_PC(ad0,pD0);
        AE_LA32X2NEG_PC(ad1,pD1);
        AE_LA32X2NEG_PC(ad2,pD2);
        AE_LA32X2NEG_PC(ad3,pD3);
        for (m=0; m<M; m+=4)
        {
            AE_LA32X2_RIC(X01,ad0,pD0); 
            AE_LA32X2_RIC(X23,ad1,pD1);
            AE_LA32X2_RIC(X45,ad2,pD2); 
            AE_LA32X2_RIC(X67,ad3,pD3);
            AE_L16X4_IP(H01,pH,2*sizeof(int32_t)); 
            AE_MULAAFD32X16_H3_L2(q0,X01,H01);
            AE_MULAAFD32X16_H3_L2(q1,X23,H01);
            AE_MULAAFD32X16_H3_L2(q2,X45,H01);
            AE_MULAAFD32X16_H3_L2(q3,X67,H01);
            AE_LA32X2_RIC(X01,ad0,pD0); 
            AE_LA32X2_RIC(X23,ad1,pD1);
            AE_LA32X2_RIC(X45,ad2,pD2); 
            AE_LA32X2_RIC(X67,ad3,pD3);
            AE_MULAAFD32X16_H1_L0(q0,X01,H01);
            AE_MULAAFD32X16_H1_L0(q1,X23,H01);
            AE_MULAAFD32X16_H1_L0(q2,X45,H01);
            AE_MULAAFD32X16_H1_L0(q3,X67,H01);
        }
        AE_S32RA64S_IP(q0, pY, sizeof(int32_t));
        AE_S32RA64S_IP(q1, pY, sizeof(int32_t));
        AE_S32RA64S_IP(q2, pY, sizeof(int32_t));
        AE_S32RA64S_IP(q3, pY, sizeof(int32_t));
    }
    return (int32_t*)pWr;
}

typedef int32_t* (*tfun)(
    const int16_t* h, int M, 
    int32_t* d, int32_t* p, int L,
    int32_t * y, const int32_t * x, int N, int D );

/* process block of samples */
void firdec32x16_process( firdecf_handle_t _firdec, 
                          int32_t * restrict       y, 
                    const int32_t *                x, int N )
{
    static const tfun fxns[]={firdec32x16_D2,0,firdec32x16_D4};
    tfun fun;
    firdec32x16_t *state;
    int M,D,L;

    NASSERT(_firdec);
    NASSERT_ALIGN(x,8);
    NASSERT(N%8==0);
    NASSERT(x);
    NASSERT(y);
    if(N<=0) return;
    state = (firdec32x16_t*)_firdec;
    NASSERT(state->h);
    NASSERT(state->d);
    NASSERT(state->p);
    NASSERT_ALIGN(state->d,8);
    NASSERT_ALIGN(state->p,8);
    NASSERT(state->magic==MAGIC);
    M=state->M;
    D=state->D;
    NASSERT(N>0);
    NASSERT(M>0);
    NASSERT(D>1);
    NASSERT(M%2==0);
    L=state->delLen;
    fun =  (D==2 || D==4) ? fxns[D-2] : &firdec32x16_Dx;
    state->p=fun(state->h,M,state->d,state->p,L,y,x,N,D);

} // firdec32x16_process()
