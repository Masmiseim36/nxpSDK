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
 * Real Matrix Inversion 3x3, 32-bit fixed point API
 * Optimized code for HiFi4
 */

/* Cross-platform data type definitions. */
#include "NatureDSP_types.h"
/* Common helper macros. */
#include "common.h"
/* Matrix functions */
#include "NatureDSP_Signal_matinv.h"

/*-------------------------------------------------------------------------
  These functions implement in-place matrix inversion by Gauss elimination 
  with full pivoting
  NOTE: user may detect "invalid" or "divide-by-zero" exception in the CPU 
  flags which MAY indicate that inversion results are not accurate. Also 
  it's responsibility of the user to provide valid input matrix for 
  inversion.
  Fixed point version takes representation of input matrix and forms 
  representation of output matrix with proper scaling.

  Precision: 
  f     floating point
  32x32 32-bit input, 32-bit output

  Input:
  x[N*N]      input matrix
  qX          input matrix representation (for fixed point API only)
  Output:
  x[N*N]      result
  Temporary:
  pScr        scratch memory. Size in bytes is defined by corresponding 
              scratch allocation function 
  N is 2,3,4,6,8,10

  Returned value: floating functions return none, fixed point functions 
                  return fixed-point representation of inverted matrix
  Restrictions:
  none
-------------------------------------------------------------------------*/
int  mtx_inv3x3_32x32  (void* pScr, int32_t *x, int qX) 
{
    const int N=3;
    const int N0=4;
    int k,n;
    int32_t *B; // [N][N0]
    int32_t *C; // [N][N0]
    int32_t *T; // [N0]
    ae_int32x2 * restrict pB; // [N][N0]
    ae_int32x2 * restrict pC; // [N][N0]
    ae_int32x2 * restrict pT; // [N0]
    ae_int32x2 * restrict pX; // [N]
    ae_int32x2 * restrict pBwr; // [N][N0]
    ae_int32x2 * restrict pCwr; // [N][N0]

    int qB,qC; // fixed point representations
    // allocate on scratch
    B=(int32_t *)pScr;
    C=B+N*N0;
    T=C+N*N0;
    // setup circular pointers for B and T
    WUR_AE_CBEGIN0((uintptr_t)B);    WUR_AE_CEND0((uintptr_t)(B+N*N0));
    WUR_AE_CBEGIN1((uintptr_t)T);    WUR_AE_CEND1((uintptr_t)(T+N));

    // copy input
    {
        ae_valign ax;
        pB=(ae_int32x2*)B;
        pC=(ae_int32x2*)C;
        pX=(ae_int32x2*)x;
        for (k=0; k<N; k++) 
        {
            ae_int32x2 b;
            ax=AE_LA64_PP(pX);
            AE_LA32X2_IP(b,ax,pX);
            AE_S32X2_IP(b,pB,sizeof(ae_int32x2));
            AE_L32_IP(b,castxcc(ae_int32,pX),sizeof(int32_t));
            AE_S32X2_IP(b,pB,sizeof(ae_int32x2));
            AE_S32X2_IP(AE_ZERO32(),pC,sizeof(ae_int32x2));
            AE_S32X2_IP(AE_ZERO32(),pC,sizeof(ae_int32x2));
        }
        pC=(ae_int32x2*)C;
        AE_S32_L_XP(0x7fffffff,castxcc(ae_int32,pC),(N0+1)*sizeof(int32_t));
        AE_S32_L_XP(0x7fffffff,castxcc(ae_int32,pC),(N0+1)*sizeof(int32_t));
        AE_S32_L_XP(0x7fffffff,castxcc(ae_int32,pC),(N0+1)*sizeof(int32_t));
    }
    qB=31;
    qC=31+(31-qX); // representation of inverted matrix

    for (k=0; k<N; k++)
    {
        int i,imax;
        int e,expB,expC;
        // find matrix normalization scale
        ae_int32x2 bmax2,cmax2;
        bmax2=0;cmax2=0;
        pB=(ae_int32x2*)B;
        pC=(ae_int32x2*)C;
        for(n=0; n<(N*N0>>1); n++) 
        {
            bmax2=AE_MAXABS32S(bmax2,pB[n]);
            cmax2=AE_MAXABS32S(cmax2,pC[n]);
        }
        bmax2=AE_MAX32(bmax2,AE_SEL32_LH(bmax2,bmax2));
        cmax2=AE_MAX32(cmax2,AE_SEL32_LH(cmax2,cmax2));
        expB=AE_NSAZ32_L(bmax2);
        expC=AE_NSAZ32_L(cmax2);
        // pivoting
        {
            int off;
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
            pC=(ae_int32x2*)&C[k*N0];        
            for (n=0; n<(N0/2); n++) 
            {
                ae_int32x2 bk,bi,ck,ci;
                bk=AE_L32X2_I(pB,0);
                bi=AE_L32X2_X(pB,off);
                ck=AE_L32X2_I(pC,0);
                ci=AE_L32X2_X(pC,off);
                AE_S32X2_X (bk,pB,off);
                AE_S32X2_IP(bi,pB,sizeof(ae_int32x2));
                AE_S32X2_X (ck,pC,off);
                AE_S32X2_IP(ci,pC,sizeof(ae_int32x2));
            }
        }
        // find normalization factor
        {
            ae_int32x2 rden,tk,maxden,maxnum;
            int e_den,e_num;
            pT=(ae_int32x2*)T;
            pB=(ae_int32x2*)&B[k];
            for (n=0; n<N; n++) 
            {
                ae_int32x2 b;
                AE_L32_XP(b,castxcc(ae_int32,pB),N0*sizeof(int32_t));
                AE_S32_L_IP(b,castxcc(ae_int32,pT),sizeof(int32_t));
            }
            AE_S32_L_IP(AE_ZERO32(),castxcc(ae_int32,pT),sizeof(int32_t));
            pT=(ae_int32x2*)T;
            tk=AE_L32_X((const ae_int32*)pT,k*sizeof(int32_t));
            AE_S32_L_X(AE_SLAA32S(1,qB),(ae_int32*)pT,k*sizeof(int32_t));
            maxnum=AE_ZERO32();
            for (n=0; n<N0/2; n++) maxnum=AE_MAXABS32S(maxnum,pT[n]);
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
            for (n=0; n<N0/2; n++)
            {
                pT[n]=AE_MULFP32X2RAS( AE_SLAA32S(pT[n],e_num),rden);
            }
        }
        // scale B and C (2-way shifts possible!)
        qB=qB+expB-e;
        qC=qC+expC-e;
        // Gauss-Jordan elimination: might be made using curcular addressing on B/C
#if 1
        {
            ae_int32x2 Ti;
            ae_int32x2 esh_coef;
            ae_int32x2 Bk0,Bk1;
            ae_int32x2 Ck0,Ck1;
            ae_int32x2 expB_coef,expC_coef;
            pB=(ae_int32x2 *)B;
            pC=(ae_int32x2 *)C;
            expB_coef=AE_SLAA32S(1,expB);
            expC_coef=AE_SLAA32S(1,expC);
            for (n=0; n<((N*N0)>>1); n++)
            {
                pB[n]=AE_MULP32X2(pB[n],expB_coef);
                pC[n]=AE_MULP32X2(pC[n],expC_coef);
            }
            pB=(ae_int32x2*)(B+k*N0);
            pC=(ae_int32x2*)(((uintptr_t)pB)+(int)((uintptr_t)C-(uintptr_t)B));
            Bk1=AE_L32X2_I(pB,1*sizeof(ae_int32x2));
            AE_L32X2_XC(Bk0,pB,2*sizeof(ae_int32x2));
            Ck0=AE_L32X2_I(pC,0);
            Ck1=AE_L32X2_I(pC,1*sizeof(ae_int32x2));
            pT=(ae_int32x2*)(T+k);
            AE_ADDCIRC32X2_XC1(pT,sizeof(int32_t));
            pBwr=pB;
            NASSERT(e>0);
            esh_coef=AE_SLAA32S(0x40000000,-e+1);
            for (i=0; i<N-1; i++)
            {
                ae_int32x2 Bin0,Bin1;
                ae_int32x2 Cin0,Cin1;
                AE_L32_XC1(Ti,castxcc(ae_int32,pT),sizeof(int32_t));
                pC=(ae_int32x2*)(((uintptr_t)pB)+(int)((uintptr_t)C-(uintptr_t)B));
                pCwr=pC;
                Bin1=AE_L32X2_I(pB,1*sizeof(ae_int32x2));
                AE_L32X2_XC(Bin0,pB,2*sizeof(ae_int32x2));
                Cin0=AE_L32X2_I(pC,0);
                Cin1=AE_L32X2_I(pC,1*sizeof(ae_int32x2));

                Bin0=AE_MULFP32X2RAS(Bin0,esh_coef);
                Cin0=AE_MULFP32X2RAS(Cin0,esh_coef);
                Bin1=AE_MULFP32X2RAS(Bin1,esh_coef);
                Cin1=AE_MULFP32X2RAS(Cin1,esh_coef);
                AE_MULSFP32X2RAS(Bin0,Bk0,Ti);
                AE_MULSFP32X2RAS(Cin0,Ck0,Ti);
                AE_MULSFP32X2RAS(Bin1,Bk1,Ti);
                AE_MULSFP32X2RAS(Cin1,Ck1,Ti);

                AE_S32X2_I (Cin0,pCwr,0*sizeof(ae_int32x2));
                AE_S32X2_I (Cin1,pCwr,1*sizeof(ae_int32x2));

                AE_S32X2_I  (Bin1,pBwr,1*sizeof(ae_int32x2));
                AE_S32X2_XC (Bin0,pBwr,2*sizeof(ae_int32x2));
            }
            AE_L32_XC1(Ti,castxcc(ae_int32,pT),sizeof(int32_t));
            pCwr=(ae_int32x2*)(((uintptr_t)pBwr)+(int)((uintptr_t)C-(uintptr_t)B));
            Bk0=AE_MULFP32X2RAS(Bk0,Ti);
            Ck0=AE_MULFP32X2RAS(Ck0,Ti);
            Bk1=AE_MULFP32X2RAS(Bk1,Ti);
            Ck1=AE_MULFP32X2RAS(Ck1,Ti);
            AE_S32X2_I (Ck0,pCwr,0*sizeof(ae_int32x2));
            AE_S32X2_I (Ck1,pCwr,1*sizeof(ae_int32x2));

            AE_S32X2_I  (Bk1,pBwr,1*sizeof(ae_int32x2));
            AE_S32X2_XC (Bk0,pBwr,2*sizeof(ae_int32x2));
            __Pragma("no_reorder");
        }
        pB=(ae_int32x2*)(B+k);
        for (i=0; i<N; i++)  AE_S32_L_XP(AE_ZERO32(),castxcc(ae_int32,pB),N0*sizeof(int32_t));
#else
        ae_int32x2 Ti;
        pB=(ae_int32x2 *)B;
        pC=(ae_int32x2 *)C;
        {
            ae_int32x2 expB_coef,expC_coef;
            expB_coef=AE_SLAA32S(1,expB);
            expC_coef=AE_SLAA32S(1,expC);
            for (n=0; n<((N*N0)>>1); n++)
            {
                pB[n]=AE_MULP32X2(pB[n],expB_coef);
                pC[n]=AE_MULP32X2(pC[n],expC_coef);
            }
        }
        pB=(ae_int32x2*)(B+k*N0);
        pT=(ae_int32x2*)(T+k);
        AE_ADDCIRC32X2_XC (pB,sizeof(int32_t)*N0);
        AE_ADDCIRC32X2_XC1(pT,sizeof(int32_t));
        NASSERT(e>0);
        for (i=0; i<N-1; i++)
        {
            AE_L32_XC1(Ti,castxcc(ae_int32,pT),sizeof(int32_t));
            for (n=0; n<N0/2; n++)
            {
                ae_int32x2 Bkn,Ckn,Bin,Cin;
                Bkn=((ae_int32x2*)&B[k*N0])[n];
                Ckn=((ae_int32x2*)&C[k*N0])[n];
                Bin=AE_L32X2_I(pB,0);
                Cin=AE_L32X2_X(pB,(int)((uintptr_t)C-(uintptr_t)B));
                Bin=AE_SLAA32S(Bin,-e);
                Cin=AE_SLAA32S(Cin,-e);
                AE_MULSFP32X2RAS(Bin,Bkn,Ti);
                AE_MULSFP32X2RAS(Cin,Ckn,Ti);
                AE_S32X2_X  (Cin,pB,(int)((uintptr_t)C-(uintptr_t)B));
                AE_S32X2_XC (Bin,pB,sizeof(ae_int32x2));
            }
        }
        AE_L32_XC1(Ti,castxcc(ae_int32,pT),sizeof(int32_t));
        for (n=0; n<N0/2; n++)
        {
            ae_int32x2 Bin,Cin;
            Bin=AE_L32X2_I(pB,0);
            Cin=AE_L32X2_X(pB,(int)((uintptr_t)C-(uintptr_t)B));
            Bin=AE_MULFP32X2RAS(Bin,Ti);
            Cin=AE_MULFP32X2RAS(Cin,Ti);
            AE_S32X2_X  (Cin,pB,(int)((uintptr_t)C-(uintptr_t)B));
            AE_S32X2_XC (Bin,pB,sizeof(ae_int32x2));
        }
        pB=(ae_int32x2*)(B+k);
        for (i=0; i<N; i++)  AE_S32_L_XP(AE_ZERO32(),castxcc(ae_int32,pB),N0*sizeof(int32_t));
#endif
    }
    // copy back to the output
    {
        ae_valign aX;
        ae_int32x2 c0,c1,c2,c3,c4,c5;
        pC=(ae_int32x2*)C;
        c0=AE_L32X2_I(pC,0*sizeof(ae_int32x2));
        c1=AE_L32X2_I(pC,1*sizeof(ae_int32x2));
        c2=AE_L32X2_I(pC,2*sizeof(ae_int32x2));
        c3=AE_L32X2_I(pC,3*sizeof(ae_int32x2));
        c4=AE_L32X2_I(pC,4*sizeof(ae_int32x2));
        c5=AE_L32X2_I(pC,5*sizeof(ae_int32x2));
        aX=AE_ZALIGN64();
        pX=(ae_int32x2*)x;
        AE_SA32X2_IP(c0,aX,pX);
        AE_SA32X2_IP(AE_SEL32_HH(c1,c2),aX,pX);
        AE_SA32X2_IP(AE_SEL32_LH(c2,c3),aX,pX);
        AE_SA32X2_IP(c4,aX,pX);
        AE_SA64POS_FP(aX,pX);
        AE_S32_L_I(AE_SEL32_HH(c5,c5),(ae_int32*)pX,0);
    }

    return qC;
}

size_t mtx_inv3x3_32x32_getScratchSize   () 
{
    return (2*3*4+4)*sizeof(int32_t);
}
