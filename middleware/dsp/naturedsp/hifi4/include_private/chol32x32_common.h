/* ------------------------------------------------------------------------ */
/* Copyright (c) 2020 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
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
/*          Copyright (C) 2015-2020 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
#ifndef CHOL32X32_COMMON_H__
#define CHOL32X32_COMMON_H__

// 64-bit NSA
inline_ int nsa64(int64_t x)
#if 0
{
  int z=0;
  if ( x==0 || x==-1)  return 63;
  while ( (int64_t)(x^(x<<1))>0 ) //MSB != MSB-1
  {
    x<<=1;
    z++;
  }
  return z;
}
#else
{
    int64_t ALIGN(8) tmp=x;
    ae_int64 t=tmp;
    t=AE_NSA64(t);
    tmp=t;
    return tmp;
}
#endif


/* two way shift of 64-bit number with saturation */
inline_ int64_t LL_shl_ll(int64_t x, int sh)
#if 0
{
    int64_t a;
    if (sh<-63) sh=-63;
    if (sh>63) sh=63;
    if (sh<0) return x>>(-sh);
    a = (int64_t)(x >> (63 - sh));
    if ((a == 0) || (a == -1))
      x <<= sh;
    else
      x = (a > 0) ? MAX_INT64 : MIN_INT64;
    return x;
}
#else
{
    int64_t ALIGN(8) tmp=x;
    ae_int64 t=tmp;
    t=AE_SLAA64S(t,sh);
    tmp=t;
    return tmp;
}
#endif

/* fractional multiply of Q63 and Q31 numbers */
inline_ int64_t mulQ63Q31(int64_t x, int32_t y)
#if 0
{
    int32_t xh,yh;
    uint32_t xl;
    int64_t B0,B1;
    int72_t A0,A1;
    A0.hi=0;
    A0.lo=0;
    A1.hi=0;
    A1.lo=0;
    xh = (int32_t)(x>>32);
    yh = (int32_t)(y);
    xl = (uint32_t)(x&0xFFFFFFFF);
    A0=add72x64(A0,(int64_t)xh*(int64_t)yh);
    A1=add72x64(A1,(int64_t)xl*(int64_t)yh);
    B1=sra72(A1,31);
    A0=add72(A0,A0);
    A0=add72x64(A0,B1);
    B0=sat72(A0);
    return B0;
}
#else
{
    int64_t ALIGN(8) tmp=x;
    ae_int32x2 Y=y;
    ae_int64 X=tmp;
    ae_int64 B1;
    ae_int64 A1; ae_ep ep1;
    AE_MUL32USEP_LH(ep1,A1,AE_MOVINT32X2_FROMINT64(X),Y);
    B1=AE_SRAI72(ep1,A1,31);
    AE_MULAF32S_HH(B1,AE_MOVINT32X2_FROMINT64(X),Y);
    tmp=B1;
    return tmp;
}
#endif

inline_ int32_t mulf31ras(int32_t x, int32_t y)
#if 0
{
  return ((((int64_t)x*y) + (1 << 30)) >> 31);
}
#else
{
    return AE_MOVAD32_L(AE_MULFP32X2RAS(x,y));
}
#endif


inline_ void invSqrt_hifi(int32_t *res, int64_t x)
#if 0
{
    int  nsa,k;
    int32_t X, Y, E, Z;
    uint32_t _80000000 = 0x80000000;
    uint32_t U;
    x>>=1;
    nsa = nsa64(x);
    X = (int32_t)((x << (nsa&~1))>>32);

    Y = _80000000 - ((X) >> 1); //Q30
    for (k = 0; k<3; k++)
    {
        Z = mulf31ras(Y,Y);
        E = 536870912 - mulf31ras(X,Z);
        Z = mulf31ras(E,Y);
        Y = Y + 2 * Z;
    }
    U = (uint32_t)((((int64_t)Y*Y) + (1 << 29)) >> 30);/*Q30*/
    E = (int32_t)((((int64_t)X*(uint32_t)U)) >> 29);/*Q32*/
    Z = mulf31ras(E,Y);
    Z = L_shr_l((Z + 2), 2);
    Y = L_sub_ll(Y, Z);
    res[0] = Y;
    res[1] = ((nsa >> 1) -2);
}
#else
{
    int64_t ALIGN(8) tmp=x;
    ae_int64 xx=tmp;
    int  nsa;
    ae_int32x2 U, X, Y, E, Z;
    ae_int32x2 _80000000 = 0x80000000;
    xx=AE_SRAI64(xx,1);
    nsa = AE_NSA64(xx);
    X=(AE_TRUNCA32X2F64S(xx,xx,(nsa&~1)));
    Y = AE_SUB32(_80000000, AE_SRAI32(X,1)); //Q30
    Z = AE_MULFP32X2RAS(Y,Y); E = 536870912; AE_MULSFP32X2RAS(E,X,Z); Z = AE_MULFP32X2RAS(E,Y); AE_MULAP32X2(Y,Z,2);
    Z = AE_MULFP32X2RAS(Y,Y); E = 536870912; AE_MULSFP32X2RAS(E,X,Z); Z = AE_MULFP32X2RAS(E,Y); AE_MULAP32X2(Y,Z,2);
    Z = AE_MULFP32X2RAS(Y,Y); E = 536870912; AE_MULSFP32X2RAS(E,X,Z); Z = AE_MULFP32X2RAS(E,Y); AE_MULAP32X2(Y,Z,2);
    xx=(1<<29); AE_MULA32_LL(xx,Y,Y); 
    xx=AE_SLAI64(xx,2); U=AE_SEL32_HH(AE_MOVINT32X2_FROMINT64(xx),AE_MOVINT32X2_FROMINT64(xx));
    xx=AE_MUL32U_LL(U,X); 
    xx=AE_SLAI64(xx,3); 
    E=AE_SEL32_HH(AE_MOVINT32X2_FROMINT64(xx),AE_MOVINT32X2_FROMINT64(xx));
    Z = AE_MULFP32X2RAS(E,Y);
    Z = AE_SRAI32R(Z, 2);
    Y = AE_SUB32S(Y, Z);
    AE_S32_L_I(Y,(ae_int32*)res,0);
    XT_S32I(((nsa >> 1) -2), (int*)res, sizeof(int32_t));
}
#endif

/* scratch for pinv functions */
typedef struct 
{
    complex_fract32* R; // R matrix, N*(N+1)/2
    complex_fract32* D; // D vector, N
    complex_fract32* Rt; // transposed R matrix, N*(N-1)/2
    complex_fract32* y; // y, intermediate solution NxN
    void*     pEnd;
    size_t sz;
}
tcmatcholpseudoinv_32x32_Scratch;

inline_ void tcmatcholpseudoinv_32x32_getScratch (tcmatcholpseudoinv_32x32_Scratch* p, void* pScr, int N)
{
    uintptr_t a=(uintptr_t)pScr;
    NASSERT_ALIGN(pScr, HIFI_SIMD_WIDTH);
    p->R=(complex_fract32*)a; a=(a+((N*(N+1))>>1 )*sizeof(complex_fract32)+(HIFI_SIMD_WIDTH-1))&~(HIFI_SIMD_WIDTH-1);
    p->D=(complex_fract32*)a; a=(a+(N            )*sizeof(complex_fract32)+(HIFI_SIMD_WIDTH-1))&~(HIFI_SIMD_WIDTH-1);
    p->Rt=(complex_fract32*)a; a=(a+((N*(N-1))>>1)*sizeof(complex_fract32)+(HIFI_SIMD_WIDTH-1))&~(HIFI_SIMD_WIDTH-1);
    p->y=(complex_fract32*)a; a=(a+(N*N          )*sizeof(complex_fract32)+(HIFI_SIMD_WIDTH-1))&~(HIFI_SIMD_WIDTH-1);
    p->pEnd=(void*)a;
    p->sz=a-((uintptr_t)pScr);
}


typedef struct 
{
    int32_t*         R; // R matrix, N*(N+1)/2
    complex_fract32* D; // D vector, N
    int32_t*         Rt;// transposed R matrix, N*(N-1)/2
    int32_t*         y; // y, intermediate solution NxN
    void*     pEnd;
    size_t sz;
}
tmatcholpseudoinv_32x32_Scratch;

inline_ void tmatcholpseudoinv_32x32_getScratch (tmatcholpseudoinv_32x32_Scratch* p, void* pScr, int N)
{
    uintptr_t a=(uintptr_t)pScr;
    NASSERT_ALIGN(pScr,HIFI_SIMD_WIDTH);
    p->R=(int32_t*        )a; a=(a+((N*(N+1))>>1 )*sizeof(int32_t        )+(HIFI_SIMD_WIDTH-1))&~(HIFI_SIMD_WIDTH-1);
    p->D=(complex_fract32*)a; a=(a+(N            )*sizeof(complex_fract32)+(HIFI_SIMD_WIDTH-1))&~(HIFI_SIMD_WIDTH-1);
    p->Rt=(int32_t*        )a; a=(a+((N*(N+1))>>1)*sizeof(int32_t        )+(HIFI_SIMD_WIDTH-1))&~(HIFI_SIMD_WIDTH-1);
    p->y=(int32_t*        )a; a=(a+(N*N          )*sizeof(int32_t        )+(HIFI_SIMD_WIDTH-1))&~(HIFI_SIMD_WIDTH-1);
    p->pEnd=(void*)a;
    p->sz=a-((uintptr_t)pScr);
}

/* scratch for mmse functions */
typedef struct
{
    complex_fract32* R; // N*(N+1)/2
    complex_fract32* D; // N
    complex_fract32* y; // N
    void*     pEnd;
    size_t sz;
}
tcmatcholmmsesolver_32x32_Scratch;

typedef struct
{
    int32_t*         R; // N*(N+1)/2
    complex_fract32* D; // N
    int32_t*         y; // N
    void*     pEnd;
    size_t sz;
}
tmatcholmmsesolver_32x32_Scratch;

inline_ void tcmatcholmmsesolver_32x32_getScratch (tcmatcholmmsesolver_32x32_Scratch* p, void* pScr, int N)
{
    uintptr_t a=(uintptr_t)pScr;
    NASSERT_ALIGN(pScr,HIFI_SIMD_WIDTH);
    p->R=(complex_fract32*)a; a=(a+((N*(N+1))>>1 )*sizeof(complex_fract32)+(HIFI_SIMD_WIDTH-1))&~(HIFI_SIMD_WIDTH-1);
    p->D=(complex_fract32*)a; a=(a+(N            )*sizeof(complex_fract32)+(HIFI_SIMD_WIDTH-1))&~(HIFI_SIMD_WIDTH-1);
    p->y=(complex_fract32*)a; a=(a+(N*N          )*sizeof(complex_fract32)+(HIFI_SIMD_WIDTH-1))&~(HIFI_SIMD_WIDTH-1);
    p->pEnd=(void*)a;
    p->sz=a-((uintptr_t)pScr);
}
inline_ void tmatcholmmsesolver_32x32_getScratch (tmatcholmmsesolver_32x32_Scratch* p, void* pScr, int N)
{
    uintptr_t a=(uintptr_t)pScr;
    NASSERT_ALIGN(pScr,HIFI_SIMD_WIDTH);
    p->R=(int32_t        *)a; a=(a+((N*(N+1))>>1 )*sizeof(int32_t        )+(HIFI_SIMD_WIDTH-1))&~(HIFI_SIMD_WIDTH-1);
    p->D=(complex_fract32*)a; a=(a+(N            )*sizeof(complex_fract32)+(HIFI_SIMD_WIDTH-1))&~(HIFI_SIMD_WIDTH-1);
    p->y=(int32_t        *)a; a=(a+(N*N          )*sizeof(int32_t        )+(HIFI_SIMD_WIDTH-1))&~(HIFI_SIMD_WIDTH-1);
    p->pEnd=(void*)a;
    p->sz=a-((uintptr_t)pScr);
}

/*--------------------------------------------------------
   update N-th diagonal element
   Input:
   Z[L][N+1][C]  convolutions in N-th column
   C             1 for real, 2 for complex
   Input/output:
   y             pointer to the begining of column in matrix 
                 R[L][SR] (N+1 elements is written)
   Output:
   D[L][SD]      reciprocals of main diagonal (pointer to 
                 the N-th element
--------------------------------------------------------*/
void cmatcholdiagUpd(int32_t* y, int32_t* D, const int64_t* Z, int N);
void matcholdiagUpd(int32_t* y, int32_t* D, const int64_t* Z, int N);


/*--------------------------------------------------------
   make forward recursion to update n new column elements
   Input:
   Z[L][N+1][C]  convolutions in N-th column, Q(2*qR-4)
   D[L][SD]      reciprocals of main diagonal
   C             1 for real, 2 for complex
   Output:
   y             pointer to the N-th column in R[L][SR] 
                 (only N elements filled), Q(qR)
--------------------------------------------------------*/
void cmatcholfwdsubstrec(int32_t* restrict y, const int32_t* restrict R, const int32_t* restrict D, const int64_t* Z, int N);

/*--------------------------------------------------------
   make forward recursion to update n new column elements
   Input:
   Z[L][N+1][C]  convolutions in N-th column, Q(2*qR-4)
   D[L][SD]      reciprocals of main diagonal
   stride        width of matrix Z
   C             1 for real, 2 for complex
   Output:
   y             pointer to the N-th column in R[L][SR]
				 (only N elements filled), Q(qR)
--------------------------------------------------------*/
void matcholfwdsubstrec(int32_t* restrict y, const int32_t* restrict R, const int32_t* restrict D, const int64_t* Z, int N, int stride);

/*-----------------------------------------------
backward recursion:
makes backward recursion from transposed matrix
yt[NxN]
Input:
yt[NxN]    - transposed right part
Rt         - transposed Cholesky decomposition
D[N]       - diagonal terms
Output:
x[NxN] 
-----------------------------------------------*/
void cmatcholbkwsubstnxn(      complex_fract32* restrict x ,
                   const complex_fract32* Rt,
                   const complex_fract32 * restrict D,
                   const complex_fract32 * restrict yt, 
                   int N, int qXYR);
void matcholbkwsubstnxn(     int32_t * restrict x ,
                     const int32_t * Rt,
                     const complex_fract32 * restrict D,
                     const int32_t * restrict yt, 
                     int N, int qXYR);

/* ---------------------------------------------------------------------
make forward recursion for R'\y=A'
Input:
R,D       - Cholesky decomposition
A[NxN]    - original input maatrix
Output:
yt[NxN]     transposed output
---------------------------------------------------------------------*/
void cmatcholfwdsubstnxn(   complex_fract32 * restrict yt,
                const complex_fract32 * restrict R, 
                const complex_fract32 * restrict D, 
                const complex_fract32 * restrict A, int N, int qYBRA);
void matcholfwdsubstnxn(  int32_t* yt,
                  const int32_t* R, 
                  const complex_fract32* D, 
                  const int32_t* A, int N, int qYBRA);

/* --------------------------------------------------------
   reversing R matrices for easier readings by rows (diagonal 
   elements are omitted):
   original R    transformed R
   0 1 3 6 a     d 8 c 4 7 b 1 3 6 a
     2 4 7 b
       5 8 c
         9 d
           e

   Input:
   R[N*(N+1)/2]    input matrix
   Output:
   Rt[N*(N-1)/2]   transposed matrix
--------------------------------------------------------*/
void cmatcholtransformR(complex_fract32 *Rt, const complex_fract32* R, int N);

#endif
