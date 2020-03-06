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
  NatureDSP Signal Processing Library. Vector mathematics
    Vector operations
    code optimized for HiFi4 core
  IntegrIT, 2006-2018
*/

#include "NatureDSP_Signal_math.h"
#include "NatureDSP_types.h"
#include "common.h"
#include "vec_pow_32x32_table.h"

void scl_pow_32x32(int32_t *  m, int16_t *e, int32_t   x, int32_t  y);
/*-------------------------------------------------------------------------
  Power function
  This routine calculates power function for 32-bit fixed-point numbers. 
  The  base is represented in Q31, the exponent is represented in Q6.25. 
  Results are represented as normalized fixed point  number with separate
  mantissa in Q31 and exponent.
  NOTE: function returns 0 for negative or zero base

  Precision:
  32x32  32-bit inputs, 32-bit outputs. Accuracy: 2 ULP

  Input:
  x[N]  input data,Q0.31
  y[N]  input data,Q6.25 
  N     length of vectors
  Output:
  m[N]  mantissa of output, Q31 
  e[N]  exponent of output  

  Restriction:
  x,y,m should not overlap
-------------------------------------------------------------------------*/
void vec_pow_32x32(int32_t *  m, int16_t *e, 
                   const int32_t *  x, const int32_t*  y, int N)
{
    #define SCR_SZ ((MAX_ALLOCA_SZ/(sizeof(int64_t)))&~3)
    int64_t ALIGN(8) scratch[SCR_SZ];
    const ae_int32x2* restrict pScrRd;
          ae_int32x2* restrict pScrWr;
    const ae_int32x2* restrict pX;
    const ae_int32x2* restrict pY;
          ae_int32x2* restrict pM;
          ae_int16*   restrict pE;
    const ae_int32*   restrict pPoly;
    ae_valign aX,aM,aY;

    int n;
    NASSERT(x);
    NASSERT(y);
    NASSERT(e);
    NASSERT(m);
    if (N<=0) return;
    pM=(ae_int32x2*)m;
    pE=(ae_int16*)e;
    aM=AE_ZALIGN64();
    pY=(const ae_int32x2*)y;
    aY=AE_LA64_PP(pY);
    while (N>1)
    {
        int M;
        M=XT_MIN(N,SCR_SZ/4);
        M&=~1;
        /* first compute log2 of input with extended precision in Q6.57 */
        /* normalization phase */
        __Pragma("no_reorder")
        pX    =(const ae_int32x2*)x;
        pScrWr=(      ae_int32x2*)scratch;
        aX=AE_LA64_PP(pX);
        for (n=0; n<(M>>1); n++)
        {
            ae_int32x2 z,ex;
            xtbool2 bsmall;

            /* normalization */
            AE_LA32X2_IP(z,aX,pX);
            ex=AE_MOVDA32X2(AE_NSAZ32_L(AE_SEL32_LH(z,z)),AE_NSAZ32_L(z));
            AE_MOVSARD7(ex);
            z=AE_SLAS32(z);
            bsmall=AE_LT32(z,1518500250);
            AE_MOVT32X2(z,AE_SLLI32(z,1),bsmall);
            AE_MOVT32X2(ex,AE_ADD32(ex,1),bsmall);
            z=AE_SUB32(0x80000000,z);
            AE_S32X2_IP(z ,pScrWr,  sizeof(ae_int64));
            AE_S32X2_IP(ex,pScrWr,3*sizeof(ae_int64));
        }
        /* compute polynomial */
        __Pragma("no_reorder")
        pScrRd=(const ae_int32x2*)scratch;
        pScrWr=(      ae_int32x2*)(scratch+2);
        pPoly =((      ae_int32*)vec_pow_32x32_polylogQ63)+1;
        for (n=0; n<(M>>1); n++)
        {
            ae_int32x2 t,y,z;
            ae_int64 v0,v1;
            AE_L32X2_IP(z ,pScrRd, 4*sizeof(ae_int64));
           
            /* a portion of polynomial is computed by 32x32 multiplies, but remainder via 64x32 multiplies */
            AE_L32_IP(y,pPoly,1*sizeof(int64_t)); 
            AE_L32_IP(t,pPoly,1*sizeof(int64_t)); AE_MULAFP32X2RAS(t,y,z); y=t;
            AE_L32_IP(t,pPoly,1*sizeof(int64_t)); AE_MULAFP32X2RAS(t,y,z); y=t;
            AE_L32_IP(t,pPoly,1*sizeof(int64_t)); AE_MULAFP32X2RAS(t,y,z); y=t;
            AE_L32_IP(t,pPoly,1*sizeof(int64_t)); AE_MULAFP32X2RAS(t,y,z); y=t;
            AE_L32_IP(t,pPoly,1*sizeof(int64_t)); AE_MULAFP32X2RAS(t,y,z); y=t;
            AE_L32_IP(t,pPoly,1*sizeof(int64_t)); AE_MULAFP32X2RAS(t,y,z); y=t;
            AE_L32_IP(t,pPoly,1*sizeof(int64_t)); AE_MULAFP32X2RAS(t,y,z); y=t;
            AE_L32_IP(t,pPoly,1*sizeof(int32_t)); AE_MULAFP32X2RAS(t,y,z); y=t;
            AE_L64_XP(v0,castxcc(ae_int64,pPoly),-(int)(17*sizeof(int32_t))); 
            v1=v0;  AE_MULAF32S_HH(v0,y,z); AE_MULAF32S_LL(v1,y,z);
            AE_S64_IP(v0,castxcc(ae_int64,pScrWr),  sizeof(ae_int64));
            AE_S64_IP(v1,castxcc(ae_int64,pScrWr),3*sizeof(ae_int64));
        }
        __Pragma("no_reorder")
        pScrRd=(const ae_int32x2*)scratch;
        pScrWr=(      ae_int32x2*)(scratch+2);
        pPoly =((const ae_int32*)(vec_pow_32x32_polylogQ63+10));
        for (n=0; n<(M>>1); n++)
        {
            ae_int32x2 t,z,ex;
            ae_int64 u,w,v0,v1;
            ae_ep acc_ep;
            AE_L32X2_IP(z ,pScrRd,  sizeof(ae_int64));
            AE_L32X2_IP(ex,pScrRd,  sizeof(ae_int64));
            AE_L64_IP(v0,castxcc(ae_int64,pScrRd),  sizeof(ae_int64));
            AE_L64_IP(v1,castxcc(ae_int64,pScrRd),  sizeof(ae_int64));

            AE_L64_IP(w,castxcc(ae_int64,pPoly),sizeof(int64_t)); 
            t=AE_MOVINT32X2_FROMINT64(v0); AE_MUL32USEP_LH(acc_ep,u,t,z); u=AE_SRAI64(u,31); AE_MULAF32S_HH(u,t,z); v0=AE_ADD64(u,w);
            t=AE_MOVINT32X2_FROMINT64(v1); AE_MUL32USEP_LL(acc_ep,u,t,z); u=AE_SRAI64(u,31); AE_MULAF32S_HL(u,t,z); v1=AE_ADD64(u,w);
            AE_L64_IP(w,castxcc(ae_int64,pPoly),sizeof(int64_t)); 
            t=AE_MOVINT32X2_FROMINT64(v0); AE_MUL32USEP_LH(acc_ep,u,t,z); u=AE_SRAI64(u,31); AE_MULAF32S_HH(u,t,z); v0=AE_ADD64(u,w);
            t=AE_MOVINT32X2_FROMINT64(v1); AE_MUL32USEP_LL(acc_ep,u,t,z); u=AE_SRAI64(u,31); AE_MULAF32S_HL(u,t,z); v1=AE_ADD64(u,w);
            AE_L64_IP(w,castxcc(ae_int64,pPoly),sizeof(int64_t)); 
            t=AE_MOVINT32X2_FROMINT64(v0); AE_MUL32USEP_LH(acc_ep,u,t,z); u=AE_SRAI64(u,31); AE_MULAF32S_HH(u,t,z); v0=AE_ADD64(u,w);
            t=AE_MOVINT32X2_FROMINT64(v1); AE_MUL32USEP_LL(acc_ep,u,t,z); u=AE_SRAI64(u,31); AE_MULAF32S_HL(u,t,z); v1=AE_ADD64(u,w);
            AE_L64_IP(w,castxcc(ae_int64,pPoly),sizeof(int64_t)); 
            t=AE_MOVINT32X2_FROMINT64(v0); AE_MUL32USEP_LH(acc_ep,u,t,z); u=AE_SRAI64(u,31); AE_MULAF32S_HH(u,t,z); v0=AE_ADD64(u,w);
            t=AE_MOVINT32X2_FROMINT64(v1); AE_MUL32USEP_LL(acc_ep,u,t,z); u=AE_SRAI64(u,31); AE_MULAF32S_HL(u,t,z); v1=AE_ADD64(u,w);

            t=AE_MOVINT32X2_FROMINT64(v0); AE_MUL32USEP_LH(acc_ep,u,t,z); u=AE_SRAI64(u,31); AE_MULAF32S_HH(u,t,z); v0=u;
            t=AE_MOVINT32X2_FROMINT64(v0); AE_MUL32USEP_LH(acc_ep,u,t,z); u=AE_SRAI64(u,31); AE_MULAF32S_HH(u,t,z); v0=u;
            t=AE_MOVINT32X2_FROMINT64(v1); AE_MUL32USEP_LL(acc_ep,u,t,z); u=AE_SRAI64(u,31); AE_MULAF32S_HL(u,t,z); v1=u;
            t=AE_MOVINT32X2_FROMINT64(v1); AE_MUL32USEP_LL(acc_ep,u,t,z); u=AE_SRAI64(u,31); AE_MULAF32S_HL(u,t,z); v1=u;
            // final scaling by log2(e)
            u=AE_MOVINT64_FROMINT32X2(AE_SEL32_HH(z,AE_ZERO32())); v0=AE_SUB64(v0,u);
            u=AE_MOVINT64_FROMINT32X2(AE_SEL32_LL(z,AE_ZERO32())); v1=AE_SUB64(v1,u);
            AE_L64_XP(w,castxcc(ae_int64,pPoly),-(int)(4*sizeof(int64_t))); 
            t=AE_MOVINT32X2_FROMINT64(v0); AE_MULZAAD32USEP_HL_LH(acc_ep,v0,t,AE_MOVINT32X2_FROMINT64(w)); v0=AE_SRAI72(acc_ep,v0,31);  AE_MULAF32S_HH(v0,t,AE_MOVINT32X2_FROMINT64(w));
            t=AE_MOVINT32X2_FROMINT64(v1); AE_MULZAAD32USEP_HL_LH(acc_ep,v1,t,AE_MOVINT32X2_FROMINT64(w)); v1=AE_SRAI72(acc_ep,v1,31);  AE_MULAF32S_HH(v1,t,AE_MOVINT32X2_FROMINT64(w));
            ex=AE_NEG32(ex);
            v0=AE_SRAI64(v0,5); t=AE_SLAI32(AE_SEL32_HH(ex,AE_ZERO32()),25); v0=AE_ADD64(AE_MOVINT64_FROMINT32X2(t),v0);
            v1=AE_SRAI64(v1,5); t=AE_SLAI32(AE_SEL32_LL(ex,AE_ZERO32()),25); v1=AE_ADD64(AE_MOVINT64_FROMINT32X2(t),v1);
            /* here we have f[0] in range -0.5...0.5 and exponent in range -31...0 */
            AE_S64_IP(v0,castxcc(ae_int64,pScrWr),  sizeof(ae_int64));
            AE_S64_IP(v1,castxcc(ae_int64,pScrWr),3*sizeof(ae_int64));
        }
        /* scale, extract exponential part */
        __Pragma("no_reorder")
        pX=(const ae_int32x2*)x;
        pScrWr=(      ae_int32x2*)scratch;
        pScrRd=(const ae_int32x2*)(scratch+2);
        aX=AE_LA64_PP(pX);
        for (n=0; n<M; n+=2)
        {
            ae_int64 r0,r1;
            ae_int32x2 s0,s1,x,exp,y;
            xtbool2 zerox;
            ae_ep acc_ep ;

            AE_LA32X2_IP(x,aX,pX);
            zerox=AE_LE32(x,AE_ZERO32());
            /* multiply log2 to form Q51 result */
            AE_LA32X2_IP(y,aY,pY);
            AE_L64_IP(r0,castxcc(ae_int64,pScrRd),  sizeof(ae_int32x2)); s0=AE_MOVINT32X2_FROMINT64(r0);
            AE_L64_IP(r1,castxcc(ae_int64,pScrRd),3*sizeof(ae_int32x2)); s1=AE_MOVINT32X2_FROMINT64(r1);

            AE_MUL32USEP_LH(acc_ep,r0, s0, y);
            AE_MUL32USEP_LL(acc_ep,r1, s1, y);
            r0=AE_SRAI64(r0,31);
            r1=AE_SRAI64(r1,31);
            AE_MULAF32S_HH(r0,s0,y);
            AE_MULAF32S_HL(r1,s1,y);

            /* exponent is found via ceil, mantissa is computed from fractional part */
            r0=AE_NEG64(r0);
            r1=AE_NEG64(r1);
            exp=AE_TRUNCA32X2F64S(r0,r1,-19);
            exp=AE_NEG32(exp);
            r0=AE_AND64(r0,0x7ffffffffffffULL);
            r1=AE_AND64(r1,0x7ffffffffffffULL);
            r0=AE_NEG64(r0);
            r1=AE_NEG64(r1);
            AE_S32X2_IP(AE_TRUNCA32X2F64S(r0,r1,12),pScrWr,4*sizeof(ae_int32x2));
            AE_MOVT32X2(exp,AE_ZERO32(),zerox);
            AE_S16_0_IP(AE_SEL16_5432(AE_MOVINT16X4_FROMINT32X2(exp),AE_MOVINT16X4_FROMINT32X2(exp)),pE,sizeof(int16_t));
            AE_S16_0_IP(AE_MOVINT16X4_FROMINT32X2(exp),pE,sizeof(int16_t));
        }
        /* compute mantissa as 2^x from fractional part */
        __Pragma("no_reorder")
        pX=(const ae_int32x2*)x;
        pScrRd=(const ae_int32x2*)scratch;
        aX=AE_LA64_PP(pX);
        pPoly=(const ae_int32*)vec_pow_32x32_polypow2;
        for (n=0; n<(M>>1); n++)
        {
            ae_int32x2 x,y,t;
            xtbool2 zerox;
            AE_LA32X2_IP(x,aX,pX);
            zerox=AE_LE32(x,AE_ZERO32());
            AE_L32X2_IP(x,pScrRd,4*sizeof(ae_int32x2));
            x=AE_ADD32(x,(1<<30));
            y=AE_L32_I(pPoly,0*sizeof(int32_t)); 
            t=AE_L32_I(pPoly,1*sizeof(int32_t));  AE_MULAFP32X2RAS(t,y,x); y=t;
            t=AE_L32_I(pPoly,2*sizeof(int32_t));  AE_MULAFP32X2RAS(t,y,x); y=t;
            t=AE_L32_I(pPoly,3*sizeof(int32_t));  AE_MULAFP32X2RAS(t,y,x); y=t;
            t=AE_L32_I(pPoly,4*sizeof(int32_t));  AE_MULAFP32X2RAS(t,y,x); y=t;
            t=AE_L32_I(pPoly,5*sizeof(int32_t));  AE_MULAFP32X2RAS(t,y,x); y=t;
            t=AE_L32_I(pPoly,6*sizeof(int32_t));  AE_MULAFP32X2RAS(t,y,x); y=t;
            t=AE_L32_I(pPoly,7*sizeof(int32_t));  y=AE_MULFP32X2RAS(y,x); y=AE_SRAI32(y,1); y=AE_ADD32S(y,t);
            t=AE_L32_X(pPoly,8*sizeof(int32_t));  AE_MULAFP32X2RAS(t,y,x); y=t;
            AE_MOVT32X2(y,AE_ZERO32(),zerox);
            AE_SA32X2_IP(y,aM,pM);
        }
        x+=M;  N-=M;
    }
    AE_SA64POS_FP(aM,pM);
    if (N&1)
    {
        scl_pow_32x32((int32_t*)pM,(int16_t*)pE,x[0],((const int32_t*)pY)[0]); 
    }

}
