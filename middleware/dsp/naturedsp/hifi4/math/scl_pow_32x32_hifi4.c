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
    scalar variant of vec_pow_32x32
    code optimized for HiFi4 core
  IntegrIT, 2006-2018
*/

#include "NatureDSP_Signal_math.h"
#include "NatureDSP_types.h"
#include "common.h"
#include "vec_pow_32x32_table.h"

/* scalar variant of power function */
void scl_pow_32x32(int32_t *  m, int16_t *e, 
                          int32_t   x, int32_t  y)
{
    const ae_int32* restrict pPoly;
    xtbool2 zerox;
    ae_int64 log2x_q57;
    ae_int32x2 r;
    zerox=AE_LE32(AE_MOVDA32(x),AE_ZERO32());

    /*
        compute log2(x) with extended precision:
        returns e - integral part
                f - fract part, Q63
        Accuracy ~ 4e-12 (better than 37 bits)
        Representation: Q6.57
    */
    {
        ae_int32x2 t,z,y,ex;
        ae_int64 u,w,v0;
        ae_ep acc_ep;
        xtbool2 bsmall;
        pPoly =(( ae_int32*)vec_pow_32x32_polylogQ63)+1;

        /* normalization */
        z=AE_MOVDA32(x);
        ex=AE_MOVDA32X2(AE_NSAZ32_L(AE_SEL32_LH(z,z)),AE_NSAZ32_L(z));
        AE_MOVSARD7(ex);
        z=AE_SLAS32(z);
        bsmall=AE_LT32(z,1518500250);
        AE_MOVT32X2(z,AE_SLLI32(z,1),bsmall);
        AE_MOVT32X2(ex,AE_ADD32(ex,1),bsmall);
        z=AE_SUB32(0x80000000,z);

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
        AE_L64_XP(v0,castxcc(ae_int64,pPoly),sizeof(int64_t)); 
        AE_MULAF32S_HH(v0,y,z); 
        AE_L64_IP(w,castxcc(ae_int64,pPoly),sizeof(int64_t)); 
        t=AE_MOVINT32X2_FROMINT64(v0); AE_MUL32USEP_LH(acc_ep,u,t,z); u=AE_SRAI64(u,31); AE_MULAF32S_HH(u,t,z); v0=AE_ADD64(u,w);
        AE_L64_IP(w,castxcc(ae_int64,pPoly),sizeof(int64_t)); 
        t=AE_MOVINT32X2_FROMINT64(v0); AE_MUL32USEP_LH(acc_ep,u,t,z); u=AE_SRAI64(u,31); AE_MULAF32S_HH(u,t,z); v0=AE_ADD64(u,w);
        AE_L64_IP(w,castxcc(ae_int64,pPoly),sizeof(int64_t)); 
        t=AE_MOVINT32X2_FROMINT64(v0); AE_MUL32USEP_LH(acc_ep,u,t,z); u=AE_SRAI64(u,31); AE_MULAF32S_HH(u,t,z); v0=AE_ADD64(u,w);
        AE_L64_IP(w,castxcc(ae_int64,pPoly),sizeof(int64_t)); 
        t=AE_MOVINT32X2_FROMINT64(v0); AE_MUL32USEP_LH(acc_ep,u,t,z); u=AE_SRAI64(u,31); AE_MULAF32S_HH(u,t,z); v0=AE_ADD64(u,w);
        t=AE_MOVINT32X2_FROMINT64(v0); AE_MUL32USEP_LH(acc_ep,u,t,z); u=AE_SRAI64(u,31); AE_MULAF32S_HH(u,t,z); v0=u;
        t=AE_MOVINT32X2_FROMINT64(v0); AE_MUL32USEP_LH(acc_ep,u,t,z); u=AE_SRAI64(u,31); AE_MULAF32S_HH(u,t,z); v0=u;
        // final scaling by log2(e)
        u=AE_MOVINT64_FROMINT32X2(AE_SEL32_HH(z,AE_ZERO32())); v0=AE_SUB64(v0,u);
        AE_L64_XP(w,castxcc(ae_int64,pPoly),-(int)(4*sizeof(int64_t))); 
        t=AE_MOVINT32X2_FROMINT64(v0); AE_MULZAAD32USEP_HL_LH(acc_ep,v0,t,AE_MOVINT32X2_FROMINT64(w)); v0=AE_SRAI72(acc_ep,v0,31);  AE_MULAF32S_HH(v0,t,AE_MOVINT32X2_FROMINT64(w));
        ex=AE_NEG32(ex);
        v0=AE_SRAI64(v0,5); t=AE_SLAI32(AE_SEL32_HH(ex,AE_ZERO32()),25); v0=AE_ADD64(AE_MOVINT64_FROMINT32X2(t),v0);
        log2x_q57=v0;
    }
    /* scale and take exponential part */
    {
        ae_int64 r0;
        ae_int32x2 s0,exp,yy;
        ae_ep acc_ep ;

        /* multiply log2 to form Q51 result */
        yy=AE_MOVDA32(y);
        s0=AE_MOVINT32X2_FROMINT64(log2x_q57);

        AE_MUL32USEP_LH(acc_ep,r0, s0, yy);
        r0=AE_SRAI64(r0,31);
        AE_MULAF32S_HH(r0,s0,yy);

        /* exponent is found via ceil, mantissa is computed from fractional part */
        r0=AE_NEG64(r0);
        exp=AE_TRUNCA32X2F64S(r0,r0,-19);
        exp=AE_NEG32(exp);
        r0=AE_AND64(r0,0x7ffffffffffffULL);
        r0=AE_NEG64(r0);
        r=AE_TRUNCA32X2F64S(r0,r0,12);
        AE_MOVT32X2(exp,AE_ZERO32(),zerox);
        e[0]=AE_MOVAD32_H(exp);
    }
    /* compute 2^x, x=[-1...0), input/output Q31 */
    {
        ae_int32x2 x,y,t;
        pPoly=(const ae_int32*)vec_pow_32x32_polypow2;
        x=r;
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
        AE_S32_L_I(y,(ae_int32*)m,0);
    }
}
