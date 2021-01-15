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
 * Real Matrix Gauss-Jordan Elimination for linear equation problem 3x3, 32-bit 
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
int  mtx_gjelim3x3_32x32  (void* pScr, int32_t *y, const int32_t* A,const int32_t * x, int qA, int qX) 
{
    const int N=3;
    int N0=(N+1)&~1;
    int k,n;
    int32_t *B; // [N][N]
    int32_t *C; // [N0]
    int32_t *T; // [N0]
    ae_int32x2 * restrict pB; // [N][N]
    ae_int32x2 * restrict pC; // [N]
    ae_int32x2 * restrict pT; // [N]
    const ae_int32x2 * restrict pX; // [N]
          ae_int32x2 * restrict pY; // [N]
    ae_int32x2 * restrict pBwr; // [N][N]

    int qB,qC; // fixed point representations
    // allocate on scratch
    B=(int32_t *)pScr;
    C=B+N*N0;
    T=C+N0;
    // setup circular pointers for B and T
    WUR_AE_CBEGIN0((uintptr_t)B);    WUR_AE_CEND0((uintptr_t)(B+N*N0));
    WUR_AE_CBEGIN1((uintptr_t)T);    WUR_AE_CEND1((uintptr_t)(T+N));

    // copy input
    {
        ae_valign ax;
        ae_int32x2  c0,c1;
        pB=(ae_int32x2*)B;
        pX=(ae_int32x2*)A;
        for (k=0; k<N; k++) 
        {
            ae_int32x2 b;
            ax=AE_LA64_PP(pX);
            AE_LA32X2_IP(b,ax,pX);
            AE_S32X2_IP(b,pB,sizeof(ae_int32x2));
            AE_L32_IP(b,castxcc(ae_int32,pX),sizeof(int32_t));
            AE_S32X2_IP(AE_SEL32_HL(b,AE_ZERO32()),pB,sizeof(ae_int32x2));
        }
        pX=(ae_int32x2*)x;
        pC=(ae_int32x2*)C;
        c1=AE_L32_I((ae_int32*)pX,2*sizeof(int32_t));
        ax=AE_LA64_PP(pX);
        AE_LA32X2_IP(c0,ax,pX);
        AE_S32X2_I(c0,pC,0);
        AE_S32X2_I(AE_SEL32_HL(c1,AE_ZERO32()),pC,1*sizeof(ae_int32x2));
    }
    qB=31;
    qC=qX+(31-qA); // representation of inverted matrix

    for (k=0; k<N; k++)
    {
        int i,imax;
        int e,expB,expC;
        // find matrix normalization scale
        ae_int32x2 bmax2,cmax2;
        bmax2=0;cmax2=0;
        pB=(ae_int32x2*)B;
        pC=(ae_int32x2*)C;
        for(n=0; n<(N*(N0>>1)); n++) 
        {
            bmax2=AE_MAXABS32S(bmax2,pB[n]);
        }
        cmax2=AE_MAXABS32S(pC[0],pC[1]);
        bmax2=AE_MAX32(bmax2,AE_SEL32_LH(bmax2,bmax2));
        cmax2=AE_MAX32(cmax2,AE_SEL32_LH(cmax2,cmax2));
        expB=AE_NSAZ32_L(bmax2);
        expC=AE_NSAZ32_L(cmax2);
        // pivoting
#if 1
        {
            int off;
            ae_int32x2 bk,bi,ck,ci;
            ae_int32x2 imax2,n2;
            ae_int32x2 bmax=AE_ZERO32();
            pB=(ae_int32x2*)(&B[k*N0+k]);
            imax2=n2=k;
            for (n=k; n<N; n++)
            {
                ae_int32x2 b;
                xtbool2 bbig;
                AE_L32_XP(b,castxcc(ae_int32,pB),N0*sizeof(int32_t));
                b=AE_ABS32S(b);
                bbig=AE_LE32(bmax,b);
                AE_MOVT32X2(imax2,n2,bbig);
                bmax=AE_MAX32(bmax,b);
                n2=AE_ADD32(n2,1);
            }
            imax=AE_MOVAD32_L(imax2);

            off=(int)((imax-k)*sizeof(int32_t)*N0);
            pB=(ae_int32x2*)&B[k*N0];
            bk=AE_L32X2_I(pB,0);
            bi=AE_L32X2_X(pB,off);
            AE_S32X2_X (bk,pB,off);
            AE_S32X2_IP(bi,pB,sizeof(ae_int32x2));
            bk=AE_L32X2_I(pB,0);
            bi=AE_L32X2_X(pB,off);
            AE_S32X2_X (bk,pB,off);
            AE_S32X2_IP(bi,pB,sizeof(ae_int32x2));
            pC=(ae_int32x2*)&C[k];        
            off=(int)((imax-k)*sizeof(int32_t));
            ck=AE_L32_I((const ae_int32*)pC,0);
            ci=AE_L32_X((const ae_int32*)pC,off);
            AE_S32_L_X (ck,(ae_int32*)pC,off);
            AE_S32_L_I (ci,(ae_int32*)pC,0);
        }
#else
        {
            int off;
            ae_int32x2 bk,bi,ck,ci;
            ae_int32x2 imax2,n2;
            ae_int32x2 bmax=AE_ZERO32();
            pB=(ae_int32x2*)(&B[k*N0+k]);
            imax2=n2=k;
            if ((N-k)&1)
            {
                ae_int32x2 b;
                xtbool2 bbig;
                AE_L32_XP(b,castxcc(ae_int32,pB),N0*sizeof(int32_t));
                b=AE_ABS32S(b);
                bbig=AE_LE32(bmax,b);
                AE_MOVT32X2(imax2,n2,bbig);
                bmax=AE_MAX32(bmax,b);
                n2=AE_ADD32(n2,1);
            }
            for (n=0; n<((N-k)>>1); n++)
            {
                ae_int32x2 b;
                xtbool2 bbig;

                AE_L32_XP(b,castxcc(ae_int32,pB),N0*sizeof(int32_t));
                b=AE_ABS32S(b);
                bbig=AE_LE32(bmax,b);
                AE_MOVT32X2(imax2,n2,bbig);
                bmax=AE_MAX32(bmax,b);
                n2=AE_ADD32(n2,1);

                AE_L32_XP(b,castxcc(ae_int32,pB),N0*sizeof(int32_t));
                b=AE_ABS32S(b);
                bbig=AE_LE32(bmax,b);
                AE_MOVT32X2(imax2,n2,bbig);
                bmax=AE_MAX32(bmax,b);
                n2=AE_ADD32(n2,1);
            }
            imax=AE_MOVAD32_L(imax2);

            off=(int)((imax-k)*sizeof(int32_t)*N0);
            pB=(ae_int32x2*)&B[k*N0];
            bk=AE_L32X2_I(pB,0);
            bi=AE_L32X2_X(pB,off);
            AE_S32X2_X (bk,pB,off);
            AE_S32X2_IP(bi,pB,sizeof(ae_int32x2));
            bk=AE_L32X2_I(pB,0);
            bi=AE_L32X2_X(pB,off);
            AE_S32X2_X (bk,pB,off);
            AE_S32X2_IP(bi,pB,sizeof(ae_int32x2));
            pC=(ae_int32x2*)&C[k];        
            off=(int)((imax-k)*sizeof(int32_t));
            ck=AE_L32_I((const ae_int32*)pC,0);
            ci=AE_L32_X((const ae_int32*)pC,off);
            AE_S32_L_X (ck,(ae_int32*)pC,off);
            AE_S32_L_I (ci,(ae_int32*)pC,0);
        }
#endif
        // find normalization factor
        {
            ae_int32x2 rden,tk,maxden,maxnum;
            ae_int32x2 b;
            int e_den,e_num;
            pT=(ae_int32x2*)T;
            pB=(ae_int32x2*)&B[k];
            AE_L32_XP(b,castxcc(ae_int32,pB),N0*sizeof(int32_t));
            AE_S32_L_I(b,(ae_int32*)pT,0*sizeof(int32_t));
            AE_L32_XP(b,castxcc(ae_int32,pB),N0*sizeof(int32_t));
            AE_S32_L_I(b,(ae_int32*)pT,1*sizeof(int32_t));
            AE_L32_XP(b,castxcc(ae_int32,pB),N0*sizeof(int32_t));
            AE_S32_L_I(b,(ae_int32*)pT,2*sizeof(int32_t));
            AE_S32_L_I(AE_ZERO32(),(ae_int32*)pT,3*sizeof(int32_t));
            tk=AE_L32_X((const ae_int32*)pT,k*sizeof(int32_t));
            AE_S32_L_X(AE_SLAA32S(1,qB),(ae_int32*)pT,k*sizeof(int32_t));
            maxnum=AE_MAXABS32S(pT[0],pT[1]);
            maxnum=AE_MAX32(maxnum,AE_SEL32_LH(maxnum,maxnum));
            maxden=AE_ABS32S(tk);
            maxnum=AE_ADD32S(AE_SRAI32(maxnum,1),AE_SRAI32(maxden,1));
            e_den=AE_NSAZ32_L(maxden); 
            e_num=AE_NSAZ32_L(maxnum)-1; 
            e=e_den-e_num+1;
            tk=AE_SLAA32S(tk,e_den);
            {   // reciprocal
                ae_int32x2 x,y,e;
                xtbool2 sx;
                x=tk;
                sx=AE_LT32(x,AE_ZERO32());
                x=AE_ABS32S(x);
                y=AE_SUB32(0xBAEC0000,x);
                e=0x40000000; AE_MULSFP32X2RAS(e,x,y);  AE_MULAFP32X2RAS(y,y,AE_SLAI32(e,1));
                e=0x40000000; AE_MULSFP32X2RAS(e,x,y);  AE_MULAFP32X2RAS(y,y,AE_SLAI32(e,1));
                e=0x40000000; AE_MULSFP32X2RAS(e,x,y);  AE_MULAFP32X2RAS(y,y,AE_SLAI32(e,1));
                e=0x40000000; AE_MULSFP32X2RAS(e,x,y);  AE_MULAFP32X2RAS(y,y,AE_SLAI32(e,1));
                AE_MOVT32X2(y,AE_NEG32(y),sx);
                rden=y;
            }
            pT[0]=AE_MULFP32X2RAS( AE_SLAA32S(pT[0],e_num),rden);
            pT[1]=AE_MULFP32X2RAS( AE_SLAA32S(pT[1],e_num),rden);
        }
        // scale B and C (2-way shifts possible!)
        qB=qB+expB-e;
        qC=qC+expC-e;
        // Gauss-Jordan elimination

        {
            ae_int32x2 esh_coef,expB_coef,expC_coef;
            ae_int32x2 Cin;
            ae_int32x2 Ti,Ckn;
            ae_int32x2 Bk0,Bk1;
            pB=(ae_int32x2 *)B;
            pC=(ae_int32x2 *)C;
            expB_coef=AE_SLAA32S(1,expB);
            expC_coef=AE_SLAA32S(1,expC);
            pB[0]=AE_MULP32X2(pB[0],expB_coef);
            pB[1]=AE_MULP32X2(pB[1],expB_coef);
            pB[2]=AE_MULP32X2(pB[2],expB_coef);
            pB[3]=AE_MULP32X2(pB[3],expB_coef);
            pB[4]=AE_MULP32X2(pB[4],expB_coef);
            pB[5]=AE_MULP32X2(pB[5],expB_coef);
            pC[0]=AE_MULP32X2(pC[0],expC_coef);
            pC[1]=AE_MULP32X2(pC[1],expC_coef);
            pC=(ae_int32x2 *)C;
            Ckn=AE_L32_X((const ae_int32*)pC,k*sizeof(int32_t));
             __Pragma("no_reorder");    // important! - C[] is addressed below circularly via pT
            pB=(ae_int32x2 *)(B+k*N0);
            pT=(ae_int32x2 *)(T+k);
            Bk1=AE_L32X2_I(pB,1*sizeof(ae_int32x2));
            AE_L32X2_XC(Bk0,pB,N0*sizeof(int32_t));
            AE_ADDCIRC32X2_XC1(pT,sizeof(int32_t));
            pBwr=pB;
            esh_coef=AE_SLAA32S(0x40000000,-e+1);
            for (i=0; i<N-1; i++)
            {
                ae_int32x2 Bin0,Bin1;
                Ti=AE_L32_I((const ae_int32*)pT,0);
                Ti=AE_NEG32S(Ti);
                Cin=AE_L32_X ((const ae_int32*)pT,(int)((uintptr_t)C-(uintptr_t)T));
                Cin=AE_SLAA32S(Cin,-e);
                AE_MULAFP32X2RAS(Cin,Ckn,Ti);
                AE_S32_L_X (Cin,(ae_int32*)pT,(int)((uintptr_t)C-(uintptr_t)T));
                AE_ADDCIRC32X2_XC1(pT,sizeof(int32_t));
                Bin1=AE_L32X2_I(pB,1*sizeof(ae_int32x2));
                AE_L32X2_XC(Bin0,pB,N0*sizeof(int32_t));
                Bin0=AE_MULFP32X2RAS(Bin0,esh_coef);
                Bin1=AE_MULFP32X2RAS(Bin1,esh_coef);
                AE_MULAFP32X2RAS(Bin0,Bk0,Ti);
                AE_MULAFP32X2RAS(Bin1,Bk1,Ti);
                AE_S32X2_I(Bin1,pBwr,1*sizeof(ae_int32x2));
                AE_S32X2_XC(Bin0,pBwr,N0*sizeof(int32_t));
            }
            Ti=AE_L32_I((const ae_int32*)pT,0);
            Ckn=AE_MULFP32X2RAS(Ckn,Ti);
            AE_S32_L_X (Ckn,(ae_int32*)pT,(int)((uintptr_t)C-(uintptr_t)T));
            Bk0=AE_MULFP32X2RAS(Bk0,Ti);
            Bk1=AE_MULFP32X2RAS(Bk1,Ti);
            AE_S32X2_I(Bk1,pBwr,1*sizeof(ae_int32x2));
            AE_S32X2_XC(Bk0,pBwr,N0*sizeof(int32_t));
            pBwr=(ae_int32x2*)(B+k);
            AE_S32_L_XP(AE_ZERO32(),castxcc(ae_int32,pBwr),N0*sizeof(int32_t));
            AE_S32_L_XP(AE_ZERO32(),castxcc(ae_int32,pBwr),N0*sizeof(int32_t));
            AE_S32_L_XP(AE_ZERO32(),castxcc(ae_int32,pBwr),N0*sizeof(int32_t));
            __Pragma("no_reorder");
        }
    }
    // copy back to the output
    {
        ae_valign aY=AE_ZALIGN64();
        ae_int32x2 c0,c1;
        pY=(ae_int32x2*)y;
        pC=(ae_int32x2*)C;
        c0=AE_L32X2_I(pC,0*sizeof(ae_int32x2));
        c1=AE_L32X2_I(pC,1*sizeof(ae_int32x2));
        AE_SA32X2_IP(c0,aY,pY);
        AE_SA64POS_FP(aY,pY);
        AE_S32_L_I(AE_SEL32_HH(c1,c1),(ae_int32*)pY,0);
    }
    return qC;
}

// scratch allocation
size_t mtx_gjelim3x3_32x32_getScratchSize  ()
{
    const int N=3;
    int N0= (N+1)&~1;
    return (N*N0+2*N0)*sizeof(int32_t);
}
