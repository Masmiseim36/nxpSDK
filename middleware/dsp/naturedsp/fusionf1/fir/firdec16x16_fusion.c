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
    Decimating block real FIR filter, 16x16-bit
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

#include "firdec16x16_common.h"

/* Instance pointer validation number. */
#define MAGIC     0x589FE2D6

/* Reserve memory for alignment. */
#define ALIGNED_SIZE( size, align ) \
    ((size_t)(size)+(align)-1)

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
    (void*)(((uintptr_t)(addr)+((align)-1)) & ~((align)-1))

#define sz_i16   sizeof(int16_t)


#if !(XCHAL_HAVE_FUSION_16BIT_BASEBAND)
/* code for no Quad MAC option */

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
} firdec16x16_t;

/* Calculate the memory block size for a decimator with given attributes. */
size_t firdec16x16_alloc( int D, int M )
{
  NASSERT( D > 1 && M > 0 );
  M=(M+3)&~3;
  return sizeof(firdec16x16_t)+(M+M+4*D)*sizeof(int32_t)+7;

} // firdec16x16_alloc()


/* Initialize the decimator structure. The delay line is zeroed. */
firdecf_handle_t firdec16x16_init( void * objmem, int D, 
                               int M, const int16_t * restrict h )
{
    firdec16x16_t* firdec;
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
    firdec=(firdec16x16_t*)(ph+M1);
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
} // firdec16x16_init()

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
static int32_t* firdec16x16_D2(
    const int16_t* h, int M, 
    int32_t* d, int32_t* p, int L,
    int16_t * restrict y, const int16_t * x, int N, int D )
{
    const ae_int16x4 *restrict pX;
          ae_int16x4 *restrict pY;
    const ae_int32x2* pD;
    const int32_t* pH;
    ae_int32x2 * pWr;
    ae_valign aD,ay;
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

    pX=(const ae_int16x4 *)x;
    WUR_AE_CBEGIN0( (uintptr_t)( d    ) );
    WUR_AE_CEND0  ( (uintptr_t)( d + L) );
    pY=(ae_int16x4*)y;
    ay=AE_ZALIGN64();
    pWr=(ae_int32x2*)p;
    for (n=0; n<N; n+=4)
    {
        ae_int16x4 XX;
        ae_int32x2 XN0,XN1;
        ae_int32x2 t; 
        ae_f64 q0,q1,q2,q3;
        q0=q1=q2=q3=AE_ZERO();
        pD=pWr;
        AE_L32_XC(t, castxcc(ae_int32, pD), 6 * sizeof(int32_t));
        pH=(const int32_t*)h;

        AE_L16X4_IP(XX,pX,sizeof(ae_int16x4));
        XN0=AE_CVT32X2F16_32(XX);
        XN1=AE_CVT32X2F16_10(XX);
        AE_S32X2_XC(XN0,pWr,2*sizeof(int32_t));
        AE_S32X2_XC(XN1,pWr,2*sizeof(int32_t));
        AE_L16X4_IP(XX,pX,sizeof(ae_int16x4));
        XN0=AE_CVT32X2F16_32(XX);
        XN1=AE_CVT32X2F16_10(XX);
        AE_S32X2_XC(XN0,pWr,2*sizeof(int32_t));
        AE_S32X2_XC(XN1,pWr,2*sizeof(int32_t));
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
        XN0 = AE_TRUNCA32X2F64S(q0,q1,  16);
        XN1 = AE_TRUNCA32X2F64S(q2,q3,  16);
        XX = AE_ROUND16X4F32SASYM(XN0, XN1);
        AE_SA16X4_IP(XX,ay,pY);
    }
    AE_SA64POS_FP(ay,pY);
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
static int32_t* firdec16x16_D4(
    const int16_t* h, int M, 
    int32_t* d, int32_t* p, int L,
    int16_t * restrict y, const int16_t * x, int N, int D )
{
    const ae_int16x4 *restrict pX;
          ae_int16   *restrict pY;
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

    pX=(const ae_int16x4 *)x;
    WUR_AE_CBEGIN0( (uintptr_t)( d    ) );
    WUR_AE_CEND0  ( (uintptr_t)( d + L) );
    pY=(ae_int16*)y;
    pWr=(ae_int32x2*)p;
    for (n=0; n<N; n+=2)
    {
        ae_int16x4 XX;
        ae_int32x2 XN0,XN1;
        ae_int32x2 t;
        ae_f64 q0,q1;
        q0=q1=AE_ZERO();
        pD=(ae_int32x2*)pWr;
        AE_L32_XC(t, castxcc(ae_int32, pD), 4 * sizeof(int32_t));
        pH=(const ae_f16x4*)h;
        AE_L16X4_IP(XX,pX,sizeof(ae_int16x4));
        XN0=AE_CVT32X2F16_32(XX);
        XN1=AE_CVT32X2F16_10(XX);
        AE_S32X2_XC(XN0,pWr,2*sizeof(int32_t));
        AE_S32X2_XC(XN1,pWr,2*sizeof(int32_t));
        AE_L16X4_IP(XX,pX,sizeof(ae_int16x4));
        XN0=AE_CVT32X2F16_32(XX);
        XN1=AE_CVT32X2F16_10(XX);
        AE_S32X2_XC(XN0,pWr,2*sizeof(int32_t));
        AE_S32X2_XC(XN1,pWr,2*sizeof(int32_t));
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
        XN0 = AE_TRUNCA32X2F64S(q0,q0,  16);
        XN1 = AE_TRUNCA32X2F64S(q1,q1,  16);
        XX = AE_ROUND16X4F32SASYM(XN0, XN0);
        AE_S16_0_IP(XX,pY,sizeof(int16_t));
        XX = AE_ROUND16X4F32SASYM(XN1, XN1);
        AE_S16_0_IP(XX,pY,sizeof(int16_t));
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
static int32_t* firdec16x16_Dx(
    const int16_t* h, int M, 
    int32_t* d, int32_t* p, int L,
    int16_t * restrict y, const int16_t * x, int N, int D )
{ 
    const ae_int16x4 *restrict pX;
          ae_int16x4 *restrict pY;
    const ae_int32x2* pD0;
    const ae_int32x2* pD1;
    const ae_int32x2* pD2;
    const ae_int32x2* pD3;
    const ae_int16x4* pH;
    ae_int32x2 * pWr;
    ae_int32x2 X01,X23,X45,X67;
    ae_int16x4 H01;
    ae_valign ay;
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

    pX=(const ae_int16x4 *)x;
    WUR_AE_CBEGIN0( (uintptr_t)( d    ) );
    WUR_AE_CEND0  ( (uintptr_t)( d + L) );
    pY=(ae_int16x4 *)y;
    ay=AE_ZALIGN64();
    pWr=(ae_int32x2*)p;
    for (n=0; n<N; n+=4)
    {
        ae_int16x4 XX;
        ae_int32x2 XN0,XN1;
        ae_f64 q0,q1,q2,q3;
        ae_int32x2 t;
        ae_valign ad0,ad1,ad2,ad3;
        pH=(const ae_int16x4*)h;
        q0=q1=q2=q3=AE_ZERO();
        pD0=(ae_int32x2*)(pWr);
        for(j=0; j<D; j++) 
        {
            AE_L16X4_IP(XX,pX,sizeof(ae_int16x4));
            XN0=AE_CVT32X2F16_32(XX);
            XN1=AE_CVT32X2F16_10(XX);
            AE_S32X2_XC(XN0,pWr,2*sizeof(int32_t));
            AE_S32X2_XC(XN1,pWr,2*sizeof(int32_t));
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
        XN0 = AE_TRUNCA32X2F64S(q0,q1,  16);
        XN1 = AE_TRUNCA32X2F64S(q2,q3,  16);
        XX = AE_ROUND16X4F32SASYM(XN0, XN1);
        AE_SA16X4_IP(XX,ay,pY);
    }
    AE_SA64POS_FP(ay,pY);
    return (int32_t*)pWr;
}

typedef int32_t* (*tfun)(
    const int16_t* h, int M, 
    int32_t* d, int32_t* p, int L,
    int16_t * y, const int16_t * x, int N, int D );

/* process block of samples */
void firdec16x16_process( firdec16x16_handle_t _firdec, 
                          int16_t * restrict       y, 
                    const int16_t *                x, int N )
{
    static const tfun fxns[]={firdec16x16_D2,0,firdec16x16_D4};
    tfun fun;
    firdec16x16_t *state;
    int M,D,L;

    NASSERT(_firdec);
    NASSERT_ALIGN(x,8);
    NASSERT(N%8==0);
    NASSERT(x);
    NASSERT(y);
    if(N<=0) return;
    state = (firdec16x16_t*)_firdec;
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
    fun =  (D==2 || D==4) ? fxns[D-2] : &firdec16x16_Dx;
    state->p=fun(state->h,M,state->d,state->p,L,y,x,N,D);

} // firdec32x16_process()

#else
/* code for Quad MAC option */

/* Decimator instance structure. */
typedef struct
{
    uint32_t        magic;     // Instance pointer validation number
    int             D;         // Decimation factor
    int             M;         // length of filter
    const int16_t * coef;      // Filter coefficients
    proc_fxn_t    * procFxn;   // Filter data processing function
    int16_t       * delayLine; // Delay line
    int             delayLen;  // Delay line length, in samples
    int             wrIx;      // Index of the oldest sample
} firdec16x16_t, *firdec16x16_ptr_t;

/* Calculate the memory block size for a decimator with given attributes. */
size_t firdec16x16_alloc(int D, int M)
{
    int bM;
    int delayLen, coefNum;
    NASSERT(D > 1 && M > 0);

    bM = M + (-M & 3);
    coefNum = bM + 4;
    delayLen = bM + 4 * D;

    return (ALIGNED_SIZE(sizeof(firdec16x16_t), 4)
        + // Delay line
        ALIGNED_SIZE((delayLen)*sz_i16, 8)
        + // Coefficients
        ALIGNED_SIZE(coefNum*sz_i16, 8));
} // firdec16x16_alloc()

/* Initialize the decimator structure. The delay line is zeroed. */
firdec16x16_handle_t firdec16x16_init(void * objmem, int D,
    int M, const int16_t * restrict h)
{
    firdec16x16_ptr_t firdec;
    void *            ptr;
    int16_t *         delLine;
    int               delLen;
    int               coefNum;
    int16_t *         coef;
    int16_t *         coefB;
    proc_fxn_t *      procFxn;

    int bM;
    int m;

    NASSERT(objmem && D > 1 && M > 0 && h);

    NASSERT(IS_ALIGN(h));

    //
    // Select the processing function, delay line length and coefficients
    // block layout.
    //

    bM = M + (-M & 3);

    coefNum = bM + 4;
    delLen = bM + 4 * D;
    procFxn = ( D == 2 ? &fir16x16_D2_proc :
                D == 3 ? &fir16x16_D3_proc :
                D == 4 ? &fir16x16_D4_proc :
                         &fir16x16_DX_proc);
    //
    // Partition the memory block.
    //

    ptr     = objmem;
    firdec  = (firdec16x16_ptr_t)(ALIGNED_ADDR(ptr, 4));
    ptr     = firdec + 1;
    delLine = (int16_t*)ALIGNED_ADDR(ptr, 8);
    ptr     = delLine + delLen;
    coef    = (int16_t*)ALIGNED_ADDR(ptr, sz_i16);
    ptr     = coef + coefNum;

    NASSERT((int8_t*)ptr - (int8_t*)objmem <= (int)firdec16x16_alloc(D, M));

    //
    // Copy the filter coefficients in reverted order and zero the delay line.
    //

    coefB = coef;

    // Begin by a few zeros to make the number of coefficients a multiple of 4.
    for (m = M; m<bM; m++)
    {
        *coefB++ = 0;
    }

    // Pad the coefficients with 4 zeros (one at the beginning, three following 
    // the end) to avoid a 1-sample delay of filter response.
    coefB[0] = coefB[M + 1] = coefB[M + 2] = coefB[M + 3] = 0;

    // Copy coefficients in reverted order.
    for (m = 1; m <= M; m++)
    {
        coefB[m] = h[M - m];
    }

    //
    // Zero the delay line.
    //

    for (m = 0; m<delLen; m++)
    {
        delLine[m] = 0;
    }

    firdec->magic       = MAGIC;
    firdec->D           = D;
    firdec->M           = bM;
    firdec->procFxn     = procFxn;
    firdec->coef        = coef;
    firdec->delayLine   = delLine;
    firdec->delayLen    = delLen;
    firdec->wrIx        = 0;

    return (firdec);
} // firdec16x16_init()

/* process block of samples */
void firdec16x16_process(firdec16x16_handle_t     _firdec,
                         int16_t * restrict       y,
                   const int16_t *                x, int N)
{
    firdec16x16_ptr_t firdec = (firdec16x16_ptr_t)_firdec;

    if (N <= 0) return;
    NASSERT(N % 8 == 0);

    //
    // Call filter's data processing function. It will store the block of input 
    // samples to the delay line, and compute the filter response. Returns the
    // updated next position pointer into the delay line buffer.
    //

    NASSERT(firdec->procFxn);

    firdec->wrIx = (*firdec->procFxn)(
        y,
        firdec->delayLine,
        firdec->delayLen,
        x,
        firdec->coef,
        firdec->wrIx,
        firdec->D,
        N,
        firdec->M);
} // firdec16x16_process()
#endif

