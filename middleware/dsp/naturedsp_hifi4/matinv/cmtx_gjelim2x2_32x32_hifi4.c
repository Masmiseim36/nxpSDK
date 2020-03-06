/* ------------------------------------------------------------------------ */
/* Copyright (c) 2018 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
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
/*          Copyright (C) 2015-2018 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
/*
 * complex Matrix Gauss-Jordan Elimination for linear equation problem, 32-bit 
   fixed point API
 * Optimized code for HiFi4
 */

/* Cross-platform data type definitions. */
#include "NatureDSP_types.h"
/* Common helper macros. */
#include "common.h"
/* Matrix functions */
#include "NatureDSP_Signal_matinv.h"

/*-------------------------------------------------------------------------
  These functions implement Gauss elimination elimination process with full 
  pivoting to findsolution of linear equations A*y=x
  
  Fixed point version takes representation of input matrix/vector and forms 
  representation of output vector with proper scaling.

  Precision: 
  32x32 32-bit input, 32-bit output

  Input:
  A[N*N]      input matrix, representation is defined by parameter qA
  x[N]        input rigth side of equation, representation is defined by 
              parameter qX
  qA          input matrix A representation (for fixed point API only)
  qX          input vector x representation (for fixed point API only)
  Output:
  y[N]        output vector
  Temporary:
  pScr        scratch memory. Size in bytes is defined by corresponding 
              scratch allocation function 
  N is 2,3,4,6,8,10

  Returned value: fixed point functions return fixed-point representation 
                  of resulted vector
  Restrictions:
  none
-------------------------------------------------------------------------*/
int  cmtx_gjelim2x2_32x32  (void* pScr, complex_fract32 *y, const complex_fract32* A,const complex_fract32 * x, int qA, int qX)
{
    const int N=2;
    xtbool2 bnegim=AE_LT32(AE_MOVDA32X2(0,-1),AE_ZERO32());
    int k,n;
    ae_int32x2 *B; // [N][N]
    ae_int32x2 *C; // [N][N]
    ae_int32x2 *T; // [N]
    ae_int32x2 * restrict pB; // [N][N]
    ae_int32x2 * restrict pC; // [N][N]
    ae_int32x2 * restrict pT; // [N][N]
    const ae_int32x2 * restrict pX;
          ae_int32x2 * restrict pY;

    int qB,qC; // fixed point representations
    // allocate on scratch
    B=(ae_int32x2 *)pScr;
    C=B+N*N;
    T=C+N;
    // setup circular pointers for B and T
    WUR_AE_CBEGIN0((uintptr_t)B);    WUR_AE_CEND0((uintptr_t)(B+N*N));
    WUR_AE_CBEGIN1((uintptr_t)T);    WUR_AE_CEND1((uintptr_t)(T+N));
    // copy input
    pB=B;
    pX=(const ae_int32x2*)A;
    for (k=0; k<N*N; k++) 
    {
        ae_int32x2 b;
        AE_L32X2_IP(b,pX,sizeof(ae_int32x2));
        AE_S32X2_IP(b,pB,sizeof(ae_int32x2));
    }
    pC=C;
    pX=(const ae_int32x2*)x;
    for (k=0; k<N; k++) 
    {
        ae_int32x2 b;
        AE_L32X2_IP(b,pX,sizeof(ae_int32x2));
        AE_S32X2_IP(b,pC,sizeof(ae_int32x2));
    }

    qB=31;
    qC=qX+(31-qA); // representation of inverted matrix

    for (k=0; k<N; k++)
    {
        int i,imax;
        int e,expB,expC;
        // find matrix normalization scale
        pB=(ae_int32x2 *)B;
        pC=(ae_int32x2 *)C;
        ae_int32x2 bmax2,cmax2;
        bmax2=cmax2=AE_ZERO32();
        for(n=0; n<N*N; n++) 
        {
            ae_int32x2 b;
            AE_L32X2_IP(b,pB,sizeof(ae_int32x2));
            bmax2=AE_MAXABS32S(bmax2,b);
        }
        for(n=0; n<N; n++) 
        {
            ae_int32x2 c;
            AE_L32X2_IP(c,pC,sizeof(ae_int32x2));
            cmax2=AE_MAXABS32S(cmax2,c);
        }
        bmax2=AE_MAX32(bmax2,AE_SEL32_LH(bmax2,bmax2));
        cmax2=AE_MAX32(cmax2,AE_SEL32_LH(cmax2,cmax2));
        expB=AE_NSAZ32_L(bmax2);
        expC=AE_NSAZ32_L(cmax2);
        // pivoting
        {
            ae_int64 bmax64;
            imax=k; bmax64=AE_ZERO64();
            pB=&B[k*N+k];
            for (n=k; n<N; n++)
            {
                ae_int32x2 bb;
                ae_int64 b;
                xtbool bbig;
                AE_L32X2_XP(bb,pB,sizeof(ae_int32x2)*N);
                b=AE_MULZAAD32_HH_LL(bb,bb);
                bbig=AE_LE64(bmax64,b);
                AE_MOVT64(bmax64,b,bbig);
                XT_MOVT(imax, n, bbig);
            }
        }
        int off=(int)((imax-k)*sizeof(ae_int32x2)*N);
        pB=&B[k*N];
        pC=&C[k];        
        for (n=0; n<N; n++) 
        {
            ae_int32x2 bk,bi;
            bk=AE_L32X2_I(pB,0);
            bi=AE_L32X2_X(pB,off);
            AE_S32X2_X (bk,pB,off);
            AE_S32X2_IP(bi,pB,sizeof(ae_int32x2));
        }
        off=(int)((imax-k)*sizeof(ae_int32x2));
        {
            ae_int32x2 ck,ci;
            ck=AE_L32X2_I(pC,0);
            ci=AE_L32X2_X(pC,off);
            AE_S32X2_X (ck,pC,off);
            AE_S32X2_IP(ci,pC,sizeof(ae_int32x2));
        }
        // find normalization factor
        {
            ae_int32x2 Tk,Tn;
            int e_den,e_num,e_rden;
            ae_int64 d2,n2;
            ae_int32x2 rden;
            pT=T;
            pB=(B+k);
            for (n=0; n<N; n++)
            {
                ae_int32x2 b;
                AE_L32X2_XP(b,pB,sizeof(ae_int32x2)*N);
                AE_S32X2_IP(b,pT,sizeof(ae_int32x2));
            }
            pT=T;
            Tk=AE_L32X2_X(pT,k*sizeof(ae_int32x2));
            AE_S32X2_X(AE_SLAA32S(AE_MOVDA32X2(1,0),qB),pT,k*sizeof(ae_int32x2));
            d2=AE_MULZAAD32_HH_LL(Tk,Tk);
            n2=AE_ZERO64();
            for (n=0; n<N; n++) 
            {
                ae_int64 t;
                AE_L32X2_IP(Tn,pT,sizeof(ae_int32x2));
                t=AE_MULZAAD32_HH_LL(Tn,Tn);
                n2=AE_MAX64(n2,t);
            }
            n2=AE_ADD64S(AE_SRAI64(n2,1),AE_SRAI64(d2,1));
            e_num=AE_NSA64(n2);
            e_num=(e_num>>1)-1;
            e_den=AE_NSA64(d2);
            e_den=(e_den>>1);
            d2=AE_SLAA64(d2,(e_den<<1));
            rden=AE_TRUNCI32X2F64S(d2,d2,0);
            e_rden=AE_NSAZ32_L(rden);
            rden=AE_SLAA32S(rden,e_rden);
            {   // reciprocal: assume rden is positive!
                ae_int32x2 X,Y,E;
                X=rden;
                Y=AE_SUB32(0xBAEC0000,X);
                E=0x40000000; AE_MULSFP32X2RAS(E,X,Y); AE_MULAFP32X2RAS(Y,Y,AE_SLLI32(E,1));
                E=0x40000000; AE_MULSFP32X2RAS(E,X,Y); AE_MULAFP32X2RAS(Y,Y,AE_SLLI32(E,1));
                E=0x40000000; AE_MULSFP32X2RAS(E,X,Y); AE_MULAFP32X2RAS(Y,Y,AE_SLLI32(E,1));
                E=0x40000000; AE_MULSFP32X2RAS(E,X,Y); AE_MULAFP32X2RAS(Y,Y,AE_SLLI32(E,1));
                rden=Y;
            }
            Tk=AE_MULFP32X2RAS(Tk,rden);
            AE_MOVT32X2(Tk,AE_NEG32S(Tk),bnegim);
            Tk=AE_SLAA32S(Tk,e_den+e_rden-1);
            pT=T;
            for (n=0; n<N; n++)
            {
                Tn=AE_L32X2_I(pT,0);
                Tn=AE_SLAA32S(Tn,e_num);
                Tn=AE_MULFC32RAS(Tn,Tk);
                AE_S32X2_IP(Tn,pT,sizeof(ae_int32x2));
            }
           e=e_den-e_num+1;
        }
        // scale B and C (2-way shifts possible!)
        qB=qB+expB-e;
        qC=qC+expC-e;
        // Gauss-Jordan elimination: might be made using curcular addressing on B/C
        {
            ae_int32x2 Cin;
            ae_int32x2 Ti,Ckn;
            pB=(ae_int32x2 *)B;
            pC=(ae_int32x2 *)C;
            for (n=0; n<N*N; n++)
            {
                pB[n]=AE_SLAA32S(pB[n],expB);
            }
            for (n=0; n<N; n++)
                pC[n]=AE_SLAA32S(pC[n],expC);
            pC=(ae_int32x2 *)C;
            Ckn=pC[k];
            pB=(B+k*N);
            pT=(T+k);
            AE_ADDCIRC32X2_XC (pB,sizeof(ae_int32x2)*N);
            AE_ADDCIRC32X2_XC1(pT,sizeof(ae_int32x2));
            for (i=0; i<N-1; i++)
            {
                Ti=AE_L32X2_I(pT,0);
                Ti=AE_NEG32S(Ti);
                Cin=AE_L32X2_X (pT,(int)((uintptr_t)C-(uintptr_t)T));
                Cin=AE_SLAA32S(Cin,-e);
                AE_MULAFC32RAS(Cin,Ckn,Ti);
                AE_S32X2_X (Cin,pT,(int)((uintptr_t)C-(uintptr_t)T));
                AE_ADDCIRC32X2_XC1(pT,sizeof(ae_int32x2));
                for (n=0; n<N; n++)
                {
                    ae_int32x2 Bkn,Bin;
                    Bkn=B[k*N+n];
                    Bin=AE_L32X2_I(pB,0);
                    Bin=AE_SLAA32S(Bin,-e);
                    AE_MULAFC32RAS(Bin,Bkn,Ti);
                    AE_S32X2_XC (Bin,pB,sizeof(ae_int32x2));
                }
            }
            Ti=AE_L32X2_I(pT,0);
            Cin=AE_MULFC32RAS(Ckn,Ti);
            AE_S32X2_X (Cin,pT,(int)((uintptr_t)C-(uintptr_t)T));
            AE_ADDCIRC32X2_XC1(pT,sizeof(ae_int32x2));
            for (n=0; n<N; n++)
            {
                ae_int32x2 Bin;
                Bin=AE_L32X2_I(pB,0);
                Bin=AE_MULFC32RAS(Bin,Ti);
                AE_S32X2_XC (Bin,pB,sizeof(ae_int32x2));
            }
            pB=(B+k);
            for (i=0; i<N; i++)  AE_S32X2_XP(AE_ZERO32(),pB,N*sizeof(ae_int32x2));
        }
    }
    // copy back to the output
    pC=C;
    pY=(ae_int32x2 *)y;
    for (k=0; k<N; k++) 
    {
        ae_int32x2 c;
        AE_L32X2_IP(c,pC,sizeof(ae_int32x2));
        AE_S32X2_IP(c,pY,sizeof(ae_int32x2));
    }
    return qC;
}
// scratch allocation
size_t cmtx_gjelim2x2_32x32_getScratchSize   () { return  (2*2+2*2)*sizeof(complex_fract32);  }
