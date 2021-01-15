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
 * Real Matrix Inversion 8x8, 32-bit fixed point API
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
int  mtx_inv8x8_32x32  (void* pScr, int32_t *x, int qX)
{
    const int N=8;
    int k,n;
    int32_t *B; // [N][N]
    int32_t *C; // [N][N]
    int32_t *T; // [N]
    ae_int32x2 * restrict pB; // [N][N]
    ae_int32x2 * restrict pC; // [N][N]
    ae_int32x2 * restrict pT; // [N]
    ae_int32x2 * restrict pX; // [N]
    ae_int32x2 * restrict pBwr; // [N][N]
    ae_int32x2 * restrict pCwr; // [N][N]

    int qB,qC; // fixed point representations
    // allocate on scratch
    B=(int32_t *)pScr;
    C=B+N*N;
    T=C+N*N;
    // setup circular pointers for B and T
    WUR_AE_CBEGIN0((uintptr_t)B);    WUR_AE_CEND0((uintptr_t)(B+N*N));
    WUR_AE_CBEGIN1((uintptr_t)T);    WUR_AE_CEND1((uintptr_t)(T+N));

    // copy input
    {
        ae_valign ax;
        pB=(ae_int32x2*)B;
        pX=(ae_int32x2*)x;
        pC=(ae_int32x2*)C;
        ax=AE_LA64_PP(pX);
        for (k=0; k<N*(N>>1); k++) 
        {
            ae_int32x2 b;
            AE_LA32X2_IP(b,ax,pX);
            AE_S32X2_IP(b,pB,sizeof(ae_int32x2));
            AE_S32X2_IP(AE_ZERO32(),pC,sizeof(ae_int32x2));
        }
        pC=(ae_int32x2*)C;
        for (k=0; k<N; k++) 
        {
            AE_S32_L_XP(0x7fffffff,castxcc(ae_int32,pC),(N+1)*sizeof(int32_t));
        }
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
        for(n=0; n<(N*N>>1); n++) 
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
            pB=(ae_int32x2*)(&B[k*N+k]);
            imax2=n2=k;
            for (n=k; n<N; n++)
            {
                ae_int32x2 b;
                xtbool2 bbig;
                AE_L32_XP(b,castxcc(ae_int32,pB),N*sizeof(int32_t));
                b=AE_ABS32S(b);
                bbig=AE_LE32(bmax,b);
                AE_MOVT32X2(imax2,n2,bbig);
                bmax=AE_MAX32(bmax,b);
                n2=AE_ADD32(n2,1);
            }
            imax=AE_MOVAD32_L(imax2);
            off=(int)((imax-k)*sizeof(int32_t)*N);
            pB=(ae_int32x2*)&B[k*N];
            pC=(ae_int32x2*)&C[k*N];        
            for (n=0; n<(N/2); n++) 
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
                AE_L32_XP(b,castxcc(ae_int32,pB),N*sizeof(int32_t));
                AE_S32_L_IP(b,castxcc(ae_int32,pT),sizeof(int32_t));
            }
            pT=(ae_int32x2*)T;
            tk=AE_L32_X((const ae_int32*)pT,k*sizeof(int32_t));
            AE_S32_L_X(AE_SLAA32S(1,qB),(ae_int32*)pT,k*sizeof(int32_t));
            maxnum=AE_ZERO32();
            for (n=0; n<N/2; n++) maxnum=AE_MAXABS32S(maxnum,pT[n]);
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
            for (n=0; n<N/2; n++)
            {
                pT[n]=AE_MULFP32X2RAS( AE_SLAA32S(pT[n],e_num),rden);
            }
        }
        // scale B and C (2-way shifts possible!)
        qB=qB+expB-e;
        qC=qC+expC-e;
        // Gauss-Jordan elimination: might be made using curcular addressing on B/C
        {
            ae_int32x2 Ti;
            ae_int32x2 esh_coef;
            ae_int32x2 Bk0,Bk1,Bk2,Bk3;
            ae_int32x2 Ck0,Ck1,Ck2,Ck3;
            ae_int32x2 expB_coef,expC_coef;
            pB=(ae_int32x2 *)B;
            pC=(ae_int32x2 *)C;
            expB_coef=AE_SLAA32S(1,expB);
            expC_coef=AE_SLAA32S(1,expC);
            for (n=0; n<((N*N)>>1); n++)
            {
                pB[n]=AE_MULP32X2(pB[n],expB_coef);
                pC[n]=AE_MULP32X2(pC[n],expC_coef);
            }
            pB=(ae_int32x2*)(B+k*N);
            pC=(ae_int32x2*)(((uintptr_t)pB)+(int)((uintptr_t)C-(uintptr_t)B));
            Bk1=AE_L32X2_I(pB,1*sizeof(ae_int32x2));
            Bk2=AE_L32X2_I(pB,2*sizeof(ae_int32x2));
            Bk3=AE_L32X2_I(pB,3*sizeof(ae_int32x2));
            AE_L32X2_XC(Bk0,pB,4*sizeof(ae_int32x2));
            Ck0=AE_L32X2_I(pC,0);
            Ck1=AE_L32X2_I(pC,1*sizeof(ae_int32x2));
            Ck2=AE_L32X2_I(pC,2*sizeof(ae_int32x2));
            Ck3=AE_L32X2_I(pC,3*sizeof(ae_int32x2));
            pT=(ae_int32x2*)(T+k);
            AE_ADDCIRC32X2_XC1(pT,sizeof(int32_t));
            pBwr=pB;
            NASSERT(e>0);
            esh_coef=AE_SLAA32S(0x40000000,-e+1);
            for (i=0; i<N-1; i++)
            {
                ae_int32x2 Bin0,Bin1,Bin2,Bin3;
                ae_int32x2 Cin0,Cin1,Cin2,Cin3;
                AE_L32_XC1(Ti,castxcc(ae_int32,pT),sizeof(int32_t));
                pC=(ae_int32x2*)(((uintptr_t)pB)+(int)((uintptr_t)C-(uintptr_t)B));
                pCwr=pC;
                Bin1=AE_L32X2_I(pB,1*sizeof(ae_int32x2));
                Bin2=AE_L32X2_I(pB,2*sizeof(ae_int32x2));
                Bin3=AE_L32X2_I(pB,3*sizeof(ae_int32x2));
                AE_L32X2_XC(Bin0,pB,4*sizeof(ae_int32x2));
                Cin0=AE_L32X2_I(pC,0);
                Cin1=AE_L32X2_I(pC,1*sizeof(ae_int32x2));
                Cin2=AE_L32X2_I(pC,2*sizeof(ae_int32x2));
                Cin3=AE_L32X2_I(pC,3*sizeof(ae_int32x2));

                Bin0=AE_MULFP32X2RAS(Bin0,esh_coef);
                Cin0=AE_MULFP32X2RAS(Cin0,esh_coef);
                Bin1=AE_MULFP32X2RAS(Bin1,esh_coef);
                Cin1=AE_MULFP32X2RAS(Cin1,esh_coef);
                Bin2=AE_MULFP32X2RAS(Bin2,esh_coef);
                Cin2=AE_MULFP32X2RAS(Cin2,esh_coef);
                Bin3=AE_MULFP32X2RAS(Bin3,esh_coef);
                Cin3=AE_MULFP32X2RAS(Cin3,esh_coef);
                AE_MULSFP32X2RAS(Bin0,Bk0,Ti);
                AE_MULSFP32X2RAS(Cin0,Ck0,Ti);
                AE_MULSFP32X2RAS(Bin1,Bk1,Ti);
                AE_MULSFP32X2RAS(Cin1,Ck1,Ti);
                AE_MULSFP32X2RAS(Bin2,Bk2,Ti);
                AE_MULSFP32X2RAS(Cin2,Ck2,Ti);
                AE_MULSFP32X2RAS(Bin3,Bk3,Ti);
                AE_MULSFP32X2RAS(Cin3,Ck3,Ti);

                AE_S32X2_I (Cin0,pCwr,0*sizeof(ae_int32x2));
                AE_S32X2_I (Cin1,pCwr,1*sizeof(ae_int32x2));
                AE_S32X2_I (Cin2,pCwr,2*sizeof(ae_int32x2));
                AE_S32X2_I (Cin3,pCwr,3*sizeof(ae_int32x2));

                AE_S32X2_I  (Bin1,pBwr,1*sizeof(ae_int32x2));
                AE_S32X2_I  (Bin2,pBwr,2*sizeof(ae_int32x2));
                AE_S32X2_I  (Bin3,pBwr,3*sizeof(ae_int32x2));
                AE_S32X2_XC (Bin0,pBwr,4*sizeof(ae_int32x2));
            }
            AE_L32_XC1(Ti,castxcc(ae_int32,pT),sizeof(int32_t));
            pCwr=(ae_int32x2*)(((uintptr_t)pBwr)+(int)((uintptr_t)C-(uintptr_t)B));
            Bk0=AE_MULFP32X2RAS(Bk0,Ti);
            Ck0=AE_MULFP32X2RAS(Ck0,Ti);
            Bk1=AE_MULFP32X2RAS(Bk1,Ti);
            Ck1=AE_MULFP32X2RAS(Ck1,Ti);
            Bk2=AE_MULFP32X2RAS(Bk2,Ti);
            Ck2=AE_MULFP32X2RAS(Ck2,Ti);
            Bk3=AE_MULFP32X2RAS(Bk3,Ti);
            Ck3=AE_MULFP32X2RAS(Ck3,Ti);
            AE_S32X2_I (Ck0,pCwr,0*sizeof(ae_int32x2));
            AE_S32X2_I (Ck1,pCwr,1*sizeof(ae_int32x2));
            AE_S32X2_I (Ck2,pCwr,2*sizeof(ae_int32x2));
            AE_S32X2_I (Ck3,pCwr,3*sizeof(ae_int32x2));

            AE_S32X2_I  (Bk1,pBwr,1*sizeof(ae_int32x2));
            AE_S32X2_I  (Bk2,pBwr,2*sizeof(ae_int32x2));
            AE_S32X2_I  (Bk3,pBwr,3*sizeof(ae_int32x2));
            AE_S32X2_XC (Bk0,pBwr,4*sizeof(ae_int32x2));
            __Pragma("no_reorder");
        }
        pB=(ae_int32x2*)(B+k);
        for (i=0; i<N; i++)  AE_S32_L_XP(AE_ZERO32(),castxcc(ae_int32,pB),N*sizeof(int32_t));
    }
    // copy back to the output
    {
        ae_valign ax;
        pX=(ae_int32x2*)x;
        pC=(ae_int32x2*)C;
        ax=AE_ZALIGN64();
        for (k=0; k<N*(N>>1); k++) 
        {
            ae_int32x2 b;
            AE_L32X2_IP(b,pC,sizeof(ae_int32x2));
            AE_SA32X2_IP(b,ax,pX);
        }
        AE_SA64POS_FP(ax,pX);
    }
    return qC;
}

size_t mtx_inv8x8_32x32_getScratchSize   () 
{ 
    const int N=8;
    return (2*N*N+N)*sizeof(int32_t);  
}
