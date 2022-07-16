/* ------------------------------------------------------------------------ */
/* Copyright (c) 2019 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
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
/*          Copyright (C) 2015-2019 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
#include "NatureDSP_types.h"
#include "NatureDSP_Signal_img.h"
#include "common.h"
#include "img_common.h"
#include "imgresizer_nearest_common.h"

/*    image resizer, nearest method, horizontal direction */

static void getIdx(int16_t* ix, int M, int N)
#if 1
{
    ae_int16x4* pIx=(ae_int16x4*)ix;
    ae_int32x2 x0,x1,inc,y0,y1,e,z;
    int32_t x;
    int n,s,s2N1M;
    NASSERT_ALIGN(ix,ALIGNMENT);
    x=N; s=NSA(x); x<<=s;
    s2N1M=NSA((2*N-1)*M);
    s=(31-s)+s2N1M;
    // compute 1/N
    /* first approximation */
    z=AE_SUB32(0xBAEC0000,x);
    /* 4 iterations to achieve 1 LSB for reciprocal */
    e=0x40000000; AE_MULSFP32X2RAS(e,x,z); AE_MULAFP32X2RAS(z,z,AE_ADD32(e,e));
    e=0x40000000; AE_MULSFP32X2RAS(e,x,z); AE_MULAFP32X2RAS(z,z,AE_ADD32(e,e));
    e=0x40000000; AE_MULSFP32X2RAS(e,x,z); AE_MULAFP32X2RAS(z,z,AE_ADD32(e,e));
    e=0x40000000; AE_MULSFP32X2RAS(e,x,z); AE_MULAFP32X2RAS(z,z,AE_ADD32(e,e));
    x0=AE_MOVDA32X2(1,3);
    x1=AE_MOVDA32X2(5,7);
    x0=AE_SLAA32S(AE_MULP32X2(x0,M),s2N1M);
    x1=AE_SLAA32S(AE_MULP32X2(x1,M),s2N1M);
    inc=AE_SLAA32S(M<<3,s2N1M);
    for (n=0; n<((N+3)&~3); n+=4)
    {
        ae_int16x4 res;
        y0=AE_SRAA32S(AE_MULFP32X2RAS(x0,z),s);
        y1=AE_SRAA32S(AE_MULFP32X2RAS(x1,z),s);
        y0=AE_MIN32(M-1,AE_MAX32(0,y0));
        y1=AE_MIN32(M-1,AE_MAX32(0,y1));
        res=AE_SAT16X4(y0,y1);
        AE_S16X4_IP(res,pIx,sizeof(ae_int16x4));

        x0=AE_ADD32(x0,inc);
        x1=AE_ADD32(x1,inc);
    }
}
#else
{
    for (n=0; n<N; n++)
    {
        float32_t u;
        u=((n+0.5f)*M)/N;
        x=L_mpy_ll(((2*n+1)*M)<<s2N1M,z);
        m=(int)floorf(u);
        m=MIN(M-1,MAX(0,m));
        ix[n]=m;
    }
}
#endif

/* returns size of coefficients */
static size_t getCoefSz(const imgsize_t* in,const imgsize_t* out)
{
    int win=in->width,wout=out->width;
    NASSERT(in->height == out->height);
    (void)in,(void)out;
    (void)win,(void)wout;
    imgsize_validate(in,2,0);
    imgsize_validate(out,2,0);
    return sizeof(int16_t)*((wout+3)&~3);   // multiple of 4
}
/* returns coefficients */
static void getCoef(void* coef, const imgsize_t* in,const imgsize_t* out)
{
    int win=in->width,wout=out->width;
    NASSERT(in->height == out->height);
    (void)in,(void)out,(void)coef;
    imgsize_validate(in,2,0);
    imgsize_validate(out,2,0);
    getIdx((int16_t*)coef,win,wout);
}

static size_t getScratchSize(const imgsize_t* in,const imgsize_t* out)
{
    (void)in,(void)out;
    imgsize_validate(in,2,0);
    imgsize_validate(out,2,0);
    /* 4 aligned rows */
    return (((in->width*sizeof(int16_t) + ALIGNMENT-1) & ~(ALIGNMENT-1))+15) * 4;
}

static void process_gs16(void *pScr, void* pCoef, const void* imgIn, void*imgOut, const imgsize_t* in, const imgsize_t* out);
static void process_fast_gu8(void *pScr, void* pCoef, const void* imgIn, void*imgOut, const imgsize_t* in, const imgsize_t* out);
static void process_gu8(void *pScr, void* pCoef, const void* imgIn, void*imgOut, const imgsize_t* in, const imgsize_t* out);

/* not in-place image resize */
static void process(void *pScr, void* pCoef, const void* imgIn, void*imgOut, const imgsize_t* in, const imgsize_t* out, int fast)
{
    int16_t * restrict t = (int16_t *)pScr;
    const int16_t * restrict ix = (const int16_t *)pCoef;
    const ae_int16x4 * restrict pIn;
          ae_int16x4 * restrict pOut;
    const ae_int16x4 * restrict pIdx;
    ae_int16x4 x0, x1, x2, x3;
    ae_int16x4 t0, t1, t2, t3;
    int m, n,
        win = in->width,
        wout = out->width,
        hout = out->height,
        ostride = out->stride,
        istride = in->stride;
    if (fast == 2)
    {
        return process_fast_gu8(pScr, pCoef, imgIn, imgOut, in, out);
    }
    if (fast == 3)
    {
        return process_gu8(pScr, pCoef, imgIn, imgOut, in, out);
    }
    if (fast == 1)
    {
        //if (win * 2 == wout) return;
        return process_gs16(pScr, pCoef, imgIn, imgOut, in, out);
    }
    
    (void)win;
    imgsize_validate(in, 2, 1);
    imgsize_validate(out, 2, 1);
    NASSERT_ALIGN(imgIn, ALIGNMENT);
    NASSERT_ALIGN(imgOut, ALIGNMENT);
    NASSERT_ALIGN(pScr, ALIGNMENT);
    NASSERT(in->height == out->height);// && in->stride == out->stride);

    if (win == wout * 2)
    {
        /* 2x decimation */
        for (n = 0; n < hout; n++)
        {
            pIn = (const ae_int16x4 *)((int16_t *)imgIn + n*istride);
            pOut = (ae_int16x4 *)((int16_t *)imgOut + n*ostride);

            for (m = 0; m < (win >> 3); m++)
            {
                AE_L16X4_IP(x0, pIn, sizeof(ae_int16x4));
                AE_L16X4_IP(x1, pIn, sizeof(ae_int16x4));
                t0 = AE_SEL16_7531(x0, x1);
                AE_S16X4_IP(t0, pOut, sizeof(ae_int16x4));
            }
            if (win & 4)
            {
                AE_L16X4_IP(x0, pIn, sizeof(ae_int16x4));
                t0 = AE_SEL16_7531(x0, x0);
                t0 = AE_SHORTSWAP(t0);
                AE_S32_L_IP(AE_MOVINT32X2_FROMINT16X4(t0), castxcc(ae_int32, pOut), 2 * sizeof(int16_t));
            }
            if (win & 2)
            {
                AE_L16X4_IP(x0, pIn, sizeof(ae_int16x4));
                x0 = AE_SHORTSWAP(x0);
                AE_S16_0_IP(x0, castxcc(ae_int16, pOut), sizeof(int16_t));
            }
        }
        return;
    }
    else if (win * 2 == wout)
    {
        /* 2x interpolation */
        for (n = 0; n < hout; n++)
        {
            pIn  = (const ae_int16x4 *)((int16_t *)imgIn + n*istride + win-1);
            pOut = (      ae_int16x4 *)((int16_t *)imgOut + n*ostride + wout-2);

            for (m = 0; m < (win&3); m++)
            {
                AE_L16_IP(x0, castxcc(ae_int16,pIn), -(int)sizeof(int16_t));
                AE_S32_L_IP(AE_MOVINT32X2_FROMINT16X4(x0), castxcc(ae_int32,pOut), -2*(int)sizeof(int16_t));
            }
            pIn  = (const ae_int16x4 *)((int16_t *)pIn  -3);
            pOut = (      ae_int16x4 *)((int16_t *)pOut - 2);
            for (m = 0; m < (win>>2); m++)
            {
                AE_L16X4_XP(x0, pIn, -(int)sizeof(ae_int16x4));
                t0 = AE_SEL16_7362(x0, x0);
                t1 = AE_SEL16_5140(x0, x0);
                AE_S16X4_XP(t1, pOut, -(int)sizeof(ae_int16x4));
                AE_S16X4_XP(t0, pOut, -(int)sizeof(ae_int16x4));
            }
        }
        return;
    }

    /* Process the image by 4 rows per iteration */
    for (n = 0; n < (hout & ~3); n+=4)
    {
        /* Interleave input samples from 4 rows and save them to the scratch */
        pIn  = (const ae_int16x4 *)((int16_t *)imgIn + n*istride);
        pOut = (      ae_int16x4 *)(t);
        __Pragma("loop_count min=1");
        for (m = 0; m < win; m+=4)
        {
            AE_L16X4_XP(x0, pIn, istride*sizeof(int16_t));
            AE_L16X4_XP(x1, pIn, istride*sizeof(int16_t));
            AE_L16X4_XP(x2, pIn, istride*sizeof(int16_t));
            AE_L16X4_XP(x3, pIn, -3*istride*sizeof(int16_t) + sizeof(ae_int16x4));
            t0 = AE_SEL16_7531(x0, x1);
            t1 = AE_SEL16_6420(x0, x1);
            t2 = AE_SEL16_7531(x2, x3);
            t3 = AE_SEL16_6420(x2, x3);
            x0 = AE_SEL16_7531(t0, t2);
            x2 = AE_SEL16_6420(t0, t2);
            x1 = AE_SEL16_7531(t1, t3);
            x3 = AE_SEL16_6420(t1, t3);
            AE_S16X4_IP(x0, pOut, sizeof(ae_int16x4));
            AE_S16X4_IP(x1, pOut, sizeof(ae_int16x4));
            AE_S16X4_IP(x2, pOut, sizeof(ae_int16x4));
            AE_S16X4_IP(x3, pOut, sizeof(ae_int16x4));
        }

        pIn  = (const ae_int16x4 *)(t);
        pOut = (      ae_int16x4 *)((int16_t *)imgOut + n*ostride);
        pIdx = (const ae_int16x4 *)(ix);
        for (m = 0; m < (wout >> 2); m++)
        {
            ae_int16x4 idx, ofs;
            /* Load 4x4 elems for the 4 columns */
            AE_L16X4_IP(idx, castxcc(ae_int16x4, pIdx), sizeof(ae_int16x4));
            ofs = AE_SLAI16S(idx, 3); /* offset = idx*sizeof(ae_int16x4) */
            x0 = AE_L16X4_X(pIn, AE_MOVAD16_3(ofs));
            x1 = AE_L16X4_X(pIn, AE_MOVAD16_2(ofs));
            x2 = AE_L16X4_X(pIn, AE_MOVAD16_1(ofs));
            x3 = AE_L16X4_X(pIn, AE_MOVAD16_0(ofs));

            t0 = AE_SEL16_7531(x0, x1);
            t1 = AE_SEL16_6420(x0, x1);
            t2 = AE_SEL16_7531(x2, x3);
            t3 = AE_SEL16_6420(x2, x3);
            x0 = AE_SEL16_7531(t0, t2);
            x2 = AE_SEL16_6420(t0, t2);
            x1 = AE_SEL16_7531(t1, t3);
            x3 = AE_SEL16_6420(t1, t3);
            AE_S16X4_XP(x0, pOut, ostride*sizeof(int16_t));
            AE_S16X4_XP(x1, pOut, ostride*sizeof(int16_t));
            AE_S16X4_XP(x2, pOut, ostride*sizeof(int16_t));
            AE_S16X4_XP(x3, pOut, -3 * ostride*sizeof(int16_t)+sizeof(ae_int16x4));
        }
        if (wout & 3)
        {
            ae_int16x4 idx, ofs;
            int16_t* pout = (int16_t*)pOut;
            /* Load 4x4 elems for the 4 columns */
            AE_L16X4_IP(idx, castxcc(ae_int16x4, pIdx), sizeof(ae_int16x4));
            ofs = AE_SLAI16S(idx, 3); /* offset = idx*sizeof(ae_int16x4) */
            x0 = AE_L16X4_X(pIn, AE_MOVAD16_3(ofs));
            x1 = AE_L16X4_X(pIn, AE_MOVAD16_2(ofs));
            x2 = AE_L16X4_X(pIn, AE_MOVAD16_1(ofs));
            x3 = AE_L16X4_X(pIn, AE_MOVAD16_0(ofs));

            t0 = AE_SEL16_7531(x0, x1);
            t1 = AE_SEL16_6420(x0, x1);
            t2 = AE_SEL16_7531(x2, x3);
            t3 = AE_SEL16_6420(x2, x3);
            x0 = AE_SEL16_7531(t0, t2);
            x2 = AE_SEL16_6420(t0, t2);
            x1 = AE_SEL16_7531(t1, t3);
            x3 = AE_SEL16_6420(t1, t3);
            pout[0 * ostride + 0] = AE_MOVAD16_3(x0);
            pout[1 * ostride + 0] = AE_MOVAD16_3(x1);
            pout[2 * ostride + 0] = AE_MOVAD16_3(x2);
            pout[3 * ostride + 0] = AE_MOVAD16_3(x3);
            if (wout & 2)
            {
                pout[0 * ostride + 1] = AE_MOVAD16_2(x0);
                pout[1 * ostride + 1] = AE_MOVAD16_2(x1);
                pout[2 * ostride + 1] = AE_MOVAD16_2(x2);
                pout[3 * ostride + 1] = AE_MOVAD16_2(x3);
            }
            if ((wout & 3) == 3)
            {
                pout[0 * ostride + 2] = AE_MOVAD16_1(x0);
                pout[1 * ostride + 2] = AE_MOVAD16_1(x1);
                pout[2 * ostride + 2] = AE_MOVAD16_1(x2);
                pout[3 * ostride + 2] = AE_MOVAD16_1(x3);
            }
        }
    }
    /* Process last 0...3 rows */
    for (n = (hout & ~3); n < hout; n++)
    {
        pIn  = (const ae_int16x4 *)((int16_t *)imgIn + n*istride);
        pOut = (      ae_int16x4 *)(t);
        __Pragma("loop_count min=1");
        for (m = 0; m < win; m+=4)
        {
            AE_L16X4_IP(x0, castxcc(ae_int16x4,pIn), sizeof(ae_int16x4));
            AE_S16X4_IP(x0, pOut, sizeof(ae_int16x4));
        }

        pIn  = (const ae_int16x4 *)(t);
        pOut = (      ae_int16x4 *)((int16_t *)imgOut + n*ostride);
        pIdx = (const ae_int16x4 *)(ix);
        for (m = 0; m < (wout >> 2); m++)
        {
            ae_int16x4 idx, ofs;
            /* Load 1x4 elems for the 4 columns */
            AE_L16X4_IP(idx, castxcc(ae_int16x4, pIdx), sizeof(ae_int16x4));
            ofs = AE_SLAI16S(idx, 1); /* offset = idx*sizeof(int16_t) */
            x0 = AE_L16_X((const ae_int16 *)pIn, AE_MOVAD16_3(ofs));
            x1 = AE_L16_X((const ae_int16 *)pIn, AE_MOVAD16_2(ofs));
            x2 = AE_L16_X((const ae_int16 *)pIn, AE_MOVAD16_1(ofs));
            x3 = AE_L16_X((const ae_int16 *)pIn, AE_MOVAD16_0(ofs));

            x0 = AE_SEL16_5140(x0, x1);
            x2 = AE_SEL16_5140(x2, x3);
            x0 = AE_SEL16_5410(x0, x2);
            AE_S16X4_IP(x0, castxcc(ae_int16x4, pOut), sizeof(ae_int16x4));
        }
        if (wout & 3)
        {
            ae_int16x4 idx, ofs;
            int16_t* pout = (int16_t*)pOut;
            /* Load 1x4 elems for the 4 columns */
            AE_L16X4_IP(idx, castxcc(ae_int16x4, pIdx), sizeof(ae_int16x4));
            ofs = AE_SLAI16S(idx, 1); /* offset = idx*sizeof(int16_t) */
            x0 = AE_L16_X((const ae_int16 *)pIn, AE_MOVAD16_3(ofs));
            x1 = AE_L16_X((const ae_int16 *)pIn, AE_MOVAD16_2(ofs));
            x2 = AE_L16_X((const ae_int16 *)pIn, AE_MOVAD16_1(ofs));
            x3 = AE_L16_X((const ae_int16 *)pIn, AE_MOVAD16_0(ofs));

            x0 = AE_SEL16_5140(x0, x1);
            x2 = AE_SEL16_5140(x2, x3);
            x0 = AE_SEL16_5410(x0, x2);
            pout[0 * ostride + 0] = AE_MOVAD16_3(x0);
            if (wout & 2)
            {
                pout[0 * ostride + 1] = AE_MOVAD16_2(x0);
            }
            if ((wout & 3) == 3)
            {
                pout[0 * ostride + 2] = AE_MOVAD16_1(x0);
            }
        }
    }
}

/* not in-place image resize */
static void process_gs16(void *pScr, void* pCoef, const void* imgIn, void*imgOut, const imgsize_t* in, const imgsize_t* out)
{
    int16_t * restrict t = (int16_t *)pScr;
    const int16_t * restrict ix = (const int16_t *)pCoef;
    const ae_int16x4 * restrict pIn;
    const ae_int16x4 * restrict pIn0;
    const ae_int16x4 * restrict pIn1;
    const ae_int16x4 * restrict pIn2;
    const ae_int16x4 * restrict pIn3;
    ae_int16x4 * restrict pOut;
    ae_int16x4 * restrict pOut0;
    ae_int16x4 * restrict pOut1;
    ae_int16x4 * restrict pOut2;
    ae_int16x4 * restrict pOut3;
    const ae_int16x4 * restrict pIdx;
    ae_valign al_in, al_out;
    ae_valign al_in0, al_in1, al_in2, al_in3;
    ae_valign al_out0, al_out1, al_out2, al_out3;
    ae_int16x4 x0, x1, x2, x3;
    ae_int16x4 t0, t1, t2, t3;
    int m, n,
        win = in->width,
        wout = out->width,
        hout = out->height,
        ostride = out->stride,
        istride = in->stride;

    (void)win;
    imgsize_validate(in, 2, 0);
    imgsize_validate(out, 2, 0);
    NASSERT_ALIGN(imgIn, 2);
    NASSERT_ALIGN(imgOut, 2);
    NASSERT_ALIGN(pScr, ALIGNMENT);
    NASSERT(in->height == out->height);

    if (win == wout * 2)
    {
        /* 2x decimation */
        for (n = 0; n < hout; n++)
        {
            pIn = (const ae_int16x4 *)((int16_t *)imgIn + n*istride);
            pOut = (ae_int16x4 *)((int16_t *)imgOut + n*ostride);
            al_in = AE_LA64_PP(pIn);
            al_out = AE_ZALIGN64();
            for (m = 0; m < (win >> 3); m++)
            {
                AE_LA16X4_IP(x0, al_in, pIn);
                AE_LA16X4_IP(x1, al_in, pIn);
                t0 = AE_SEL16_7531(x0, x1);
                AE_SA16X4_IP(t0, al_out, pOut);
            }
            AE_SA64POS_FP(al_out, pOut);
            if (win & 4)
            {
                AE_LA16X4_IP(x0, al_in, pIn);
                ((int16_t*)pOut)[0] = AE_MOVAD16_3(x0);
                ((int16_t*)pOut)[1] = AE_MOVAD16_1(x0);
                pOut = (ae_int16x4 *)((int16_t *)pOut + 2);
            }
            if (win & 2)
            {
                AE_LA16X4_IP(x0, al_in, pIn);
                x0 = AE_SHORTSWAP(x0);
                AE_S16_0_IP(x0, castxcc(ae_int16, pOut), sizeof(int16_t));
            }
        }
        return;
    }
    else if (win * 2 == wout)
    {
        /* 2x interpolation */
        for (n = 0; n < hout; n++)
        {
            pIn = (const ae_int16x4 *)((int16_t *)imgIn + n*istride + win - 1);
            pOut = (ae_int16x4 *)((int16_t *)imgOut + n*ostride + wout - 2);
            int16_t* out = (int16_t *)pOut;
            for (m = 0; m < (win & 3); m++)
            {
                AE_L16_XP(x0, castxcc(ae_int16, pIn), -(int)sizeof(int16_t));
                out[-2 * m + 0] = AE_MOVAD16_0(x0);
                out[-2 * m + 1] = AE_MOVAD16_0(x0);
            }
            pIn0 = (const ae_int16x4*)((int16_t *)imgIn + n*istride + win - 1 - (win & 3));
            pOut0 = (ae_int16x4 *)((int16_t *)imgOut + n*ostride + wout - 1 - 2*(win & 3)); 
            al_in = AE_LA64_PP(pIn0);
            al_out = AE_ZALIGN64();
            for (m = 0; m < (win >> 2); m++)
            {
                AE_LA16X4_RIP(x0, al_in, pIn0);
                t0 = AE_SEL16_7362(x0, x0);
                t1 = AE_SEL16_5140(x0, x0);
                AE_SA16X4_RIP(t0, al_out, pOut0);
                AE_SA16X4_RIP(t1, al_out, pOut0);
            }
            AE_SA64NEG_FP(al_out, pOut0);
        }
        return;
    }

    /* Process the image by 4 rows per iteration */
    for (n = 0; n < (hout & ~3); n += 4)
    {
        /* Interleave input samples from 4 rows and save them to the scratch */
        pIn = (const ae_int16x4 *)((int16_t *)imgIn + n*istride);
        pIn0 = (const ae_int16x4 *)((int16_t *)imgIn + (n+0)*istride);
        pIn1 = (const ae_int16x4 *)((int16_t *)imgIn + (n+1)*istride);
        pIn2 = (const ae_int16x4 *)((int16_t *)imgIn + (n+2)*istride);
        pIn3 = (const ae_int16x4 *)((int16_t *)imgIn + (n+3)*istride);
        al_in0 = AE_LA64_PP(pIn0);
        al_in1 = AE_LA64_PP(pIn1);
        al_in2 = AE_LA64_PP(pIn2);
        al_in3 = AE_LA64_PP(pIn3);
        pOut = (ae_int16x4 *)(t);
        __Pragma("loop_count min=1");
        for (m = 0; m < win; m += 4)
        {
            AE_LA16X4_IP(x0, al_in0, pIn0);
            AE_LA16X4_IP(x1, al_in1, pIn1);
            AE_LA16X4_IP(x2, al_in2, pIn2);
            AE_LA16X4_IP(x3, al_in3, pIn3);
            t0 = AE_SEL16_7531(x0, x1);
            t1 = AE_SEL16_6420(x0, x1);
            t2 = AE_SEL16_7531(x2, x3);
            t3 = AE_SEL16_6420(x2, x3);
            x0 = AE_SEL16_7531(t0, t2);
            x2 = AE_SEL16_6420(t0, t2);
            x1 = AE_SEL16_7531(t1, t3);
            x3 = AE_SEL16_6420(t1, t3);
            AE_S16X4_IP(x0, pOut, sizeof(ae_int16x4));
            AE_S16X4_IP(x1, pOut, sizeof(ae_int16x4));
            AE_S16X4_IP(x2, pOut, sizeof(ae_int16x4));
            AE_S16X4_IP(x3, pOut, sizeof(ae_int16x4));
        }

        pIn = (const ae_int16x4 *)(t);
        pOut = (ae_int16x4 *)((int16_t *)imgOut + n*ostride);
        pOut0 = (ae_int16x4 *)((int16_t *)imgOut + (n+0)*ostride);
        pOut1 = (ae_int16x4 *)((int16_t *)imgOut + (n+1)*ostride);
        pOut2 = (ae_int16x4 *)((int16_t *)imgOut + (n+2)*ostride);
        pOut3 = (ae_int16x4 *)((int16_t *)imgOut + (n+3)*ostride);
        al_out0 = AE_ZALIGN64();
        al_out1 = AE_ZALIGN64();
        al_out2 = AE_ZALIGN64();
        al_out3 = AE_ZALIGN64();
        pIdx = (const ae_int16x4 *)(ix);
        for (m = 0; m < (wout >> 2); m++)
        {
            ae_int16x4 idx, ofs;
            /* Load 4x4 elems for the 4 columns */
            AE_L16X4_IP(idx, castxcc(ae_int16x4, pIdx), sizeof(ae_int16x4));
            ofs = AE_SLAI16S(idx, 3); /* offset = idx*sizeof(ae_int16x4) */
            x0 = AE_L16X4_X(pIn, AE_MOVAD16_3(ofs));
            x1 = AE_L16X4_X(pIn, AE_MOVAD16_2(ofs));
            x2 = AE_L16X4_X(pIn, AE_MOVAD16_1(ofs));
            x3 = AE_L16X4_X(pIn, AE_MOVAD16_0(ofs));

            t0 = AE_SEL16_7531(x0, x1);
            t1 = AE_SEL16_6420(x0, x1);
            t2 = AE_SEL16_7531(x2, x3);
            t3 = AE_SEL16_6420(x2, x3);
            x0 = AE_SEL16_7531(t0, t2);
            x2 = AE_SEL16_6420(t0, t2);
            x1 = AE_SEL16_7531(t1, t3);
            x3 = AE_SEL16_6420(t1, t3);
            AE_SA16X4_IP(x0, al_out0, pOut0);
            AE_SA16X4_IP(x1, al_out1, pOut1);
            AE_SA16X4_IP(x2, al_out2, pOut2);
            AE_SA16X4_IP(x3, al_out3, pOut3);
        }
        AE_SA64POS_FP(al_out0, pOut0);
        AE_SA64POS_FP(al_out1, pOut1);
        AE_SA64POS_FP(al_out2, pOut2);
        AE_SA64POS_FP(al_out3, pOut3);
        pOut = pOut0;
        if (wout & 3)
        {
            ae_int16x4 idx, ofs;
            int16_t* pout = (int16_t*)pOut;
            /* Load 4x4 elems for the 4 columns */
            AE_L16X4_IP(idx, castxcc(ae_int16x4, pIdx), sizeof(ae_int16x4));
            ofs = AE_SLAI16S(idx, 3); /* offset = idx*sizeof(ae_int16x4) */
            x0 = AE_L16X4_X(pIn, AE_MOVAD16_3(ofs));
            x1 = AE_L16X4_X(pIn, AE_MOVAD16_2(ofs));
            x2 = AE_L16X4_X(pIn, AE_MOVAD16_1(ofs));
            x3 = AE_L16X4_X(pIn, AE_MOVAD16_0(ofs));

            t0 = AE_SEL16_7531(x0, x1);
            t1 = AE_SEL16_6420(x0, x1);
            t2 = AE_SEL16_7531(x2, x3);
            t3 = AE_SEL16_6420(x2, x3);
            x0 = AE_SEL16_7531(t0, t2);
            x2 = AE_SEL16_6420(t0, t2);
            x1 = AE_SEL16_7531(t1, t3);
            x3 = AE_SEL16_6420(t1, t3);
            pout[0 * ostride + 0] = AE_MOVAD16_3(x0);
            pout[1 * ostride + 0] = AE_MOVAD16_3(x1);
            pout[2 * ostride + 0] = AE_MOVAD16_3(x2);
            pout[3 * ostride + 0] = AE_MOVAD16_3(x3);
            if (wout & 2)
            {
                pout[0 * ostride + 1] = AE_MOVAD16_2(x0);
                pout[1 * ostride + 1] = AE_MOVAD16_2(x1);
                pout[2 * ostride + 1] = AE_MOVAD16_2(x2);
                pout[3 * ostride + 1] = AE_MOVAD16_2(x3);
            }
            if ((wout & 3) == 3)
            {
                pout[0 * ostride + 2] = AE_MOVAD16_1(x0);
                pout[1 * ostride + 2] = AE_MOVAD16_1(x1);
                pout[2 * ostride + 2] = AE_MOVAD16_1(x2);
                pout[3 * ostride + 2] = AE_MOVAD16_1(x3);
            }
        }
    }
    /* Process last 0...3 rows */
    for (n = (hout & ~3); n < hout; n++)
    {
        pIn = (const ae_int16x4 *)((int16_t *)imgIn + n*istride);
        al_in = AE_LA64_PP(pIn);
        pOut = (ae_int16x4 *)(t);
        __Pragma("loop_count min=1");
        for (m = 0; m < win; m += 4)
        {
            AE_LA16X4_IP(x0, al_in, pIn);
            AE_S16X4_IP(x0, pOut, sizeof(ae_int16x4));
        }

        pIn = (const ae_int16x4 *)(t);
        pOut = (ae_int16x4 *)((int16_t *)imgOut + n*ostride);
        al_out = AE_ZALIGN64();
        pIdx = (const ae_int16x4 *)(ix);
        for (m = 0; m < (wout >> 2); m++)
        {
            ae_int16x4 idx, ofs;
            /* Load 1x4 elems for the 4 columns */
            AE_L16X4_IP(idx, castxcc(ae_int16x4, pIdx), sizeof(ae_int16x4));
            ofs = AE_SLAI16S(idx, 1); /* offset = idx*sizeof(int16_t) */
            x0 = AE_L16_X((const ae_int16 *)pIn, AE_MOVAD16_3(ofs));
            x1 = AE_L16_X((const ae_int16 *)pIn, AE_MOVAD16_2(ofs));
            x2 = AE_L16_X((const ae_int16 *)pIn, AE_MOVAD16_1(ofs));
            x3 = AE_L16_X((const ae_int16 *)pIn, AE_MOVAD16_0(ofs));

            x0 = AE_SEL16_5140(x0, x1);
            x2 = AE_SEL16_5140(x2, x3);
            x0 = AE_SEL16_5410(x0, x2);
            AE_SA16X4_IP(x0, al_out, pOut);
        }
        AE_SA64POS_FP(al_out, pOut);
        if (wout & 3)
        {
            ae_int16x4 idx, ofs;
            int16_t* pout = (int16_t*)pOut;
            /* Load 1x4 elems for the 4 columns */
            AE_L16X4_IP(idx, castxcc(ae_int16x4, pIdx), sizeof(ae_int16x4));
            ofs = AE_SLAI16S(idx, 1); /* offset = idx*sizeof(int16_t) */
            x0 = AE_L16_X((const ae_int16 *)pIn, AE_MOVAD16_3(ofs));
            x1 = AE_L16_X((const ae_int16 *)pIn, AE_MOVAD16_2(ofs));
            x2 = AE_L16_X((const ae_int16 *)pIn, AE_MOVAD16_1(ofs));
            x3 = AE_L16_X((const ae_int16 *)pIn, AE_MOVAD16_0(ofs));

            x0 = AE_SEL16_5140(x0, x1);
            x2 = AE_SEL16_5140(x2, x3);
            x0 = AE_SEL16_5410(x0, x2);
            pout[0 * ostride + 0] = AE_MOVAD16_3(x0);
            if (wout & 2)
            {
                pout[0 * ostride + 1] = AE_MOVAD16_2(x0);
            }
            if ((wout & 3) == 3)
            {
                pout[0 * ostride + 2] = AE_MOVAD16_1(x0);
            }
        }
    }
}


/* 8 bit fast process */
static void process_fast_gu8(void *pScr, void* pCoef, const void* imgIn, void*imgOut, const imgsize_t* in, const imgsize_t* out)
{
    const uint8_t * restrict pin;
    uint8_t * restrict pout;
    int16_t * restrict t = (int16_t *)pScr;
    const int16_t * restrict ix = (const int16_t *)pCoef;
    const ae_int16x4 * restrict pIn;
    const signed char * restrict pIn0;
    const signed char * restrict pIn1;
    const signed char * restrict pIn2;
    const signed char * restrict pIn3;
    ae_int16x4 * restrict pOut;
    const ae_int16x4 * restrict pIdx;
    ae_int16x4 x0, x1, x2, x3;
    ae_int16x4 x0_, x1_, x2_, x3_;
    ae_int16x4 t0, t1, t2, t3;
    int m, n,
        win = in->width,
        wout = out->width,
        hout = out->height,
        ostride = out->stride,
        istride = in->stride;

    (void)win;
    imgsize_validate(in, 1, 1);
    imgsize_validate(out, 1, 1);
    NASSERT_ALIGN(imgIn, ALIGNMENT);
    NASSERT_ALIGN(imgOut, ALIGNMENT);
    NASSERT_ALIGN(pScr, ALIGNMENT);
    NASSERT(in->height == out->height);

    if (win == wout * 2)
    {
        /* 2x decimation */
        for (n = 0; n < hout; n++)
        {
            pIn = (const ae_int16x4 *)((uint8_t *)imgIn + n*istride);
            pOut = (ae_int16x4 *)((uint8_t *)imgOut + n*ostride);
            for (m = 0; m < (wout >> 3); m++)
            {
                AE_L16X4_IP(x0, pIn, sizeof(ae_int16x4));
                AE_L16X4_IP(x1, pIn, sizeof(ae_int16x4));
                x0 = AE_AND16(x0, 0x00ff);
                x1 = AE_AND16(x1, 0x00ff);
                t0 = AE_SEL16_7531(x0, x1);
                t1 = AE_SEL16_6420(x0, x1);
                t1 = AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(t1), 8));
                t0 = AE_OR16(t0, t1);
                AE_S16X4_IP(t0, pOut, sizeof(ae_int16x4));
            }
            if (wout & 7)
            {
                pin = (uint8_t *)pIn;
                pout = (uint8_t *)pOut;
                for (m = 0; m<(wout & 7); m++)
                {
                    pout[m] = pin[2*m];
                }
            }
        }
        return;
    }
    else if (win * 2 == wout)
    {
        for (n = 0; n < hout; n++)
        {
            pIn = (const ae_int16x4 *)((uint8_t *)imgIn + n*istride);
            pOut = (ae_int16x4 *)((uint8_t *)imgOut + n*ostride);
            for (m = 0; m < (win >> 3); m++)
            {
                AE_L16X4_IP(x0, pIn, sizeof(ae_int16x4));
                t0 = AE_AND16(x0, 0x00ff);
                t1 = AE_AND16(x0, (int16_t)(0xff00));
                t0 = AE_OR16(t0, AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(t0), 8)));
                t1 = AE_OR16(t1, AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(t1), 8)));
                x0 = AE_SEL16_7362(t0, t1);
                x1 = AE_SEL16_5140(t0, t1);
                AE_S16X4_IP(x0, pOut, sizeof(ae_int16x4));
                AE_S16X4_IP(x1, pOut, sizeof(ae_int16x4));
            }
            if (win & 7)
            {
                pin = (uint8_t *)pIn;
                pout = (uint8_t *)pOut;
                for (m = 0; m < (win & 7); m++)
                {
                    pout[2 * m + 0] = pin[m];
                    pout[2 * m + 1] = pin[m];
                }
            }
        }
        return;
    }

    /* Process the image by 4 rows per iteration */
    for (n = 0; n < (hout & ~3); n += 4)
    {
        /* Interleave input samples from 4 rows and save them to the scratch */
        pIn0 = (const signed char *)((uint8_t *)imgIn + (n + 0)*istride);
        pIn1 = (const signed char *)((uint8_t *)imgIn + (n + 1)*istride);
        pIn2 = (const signed char *)((uint8_t *)imgIn + (n + 2)*istride);
        pIn3 = (const signed char *)((uint8_t *)imgIn + (n + 3)*istride);
        pOut = (ae_int16x4 *)(t);
        __Pragma("loop_count min=1");
        for (m = 0; m < win; m += 4)
        {
            AE_L8X4F_IP(x0, pIn0, 4*sizeof(uint8_t));
            AE_L8X4F_IP(x1, pIn1, 4*sizeof(uint8_t));
            AE_L8X4F_IP(x2, pIn2, 4*sizeof(uint8_t));
            AE_L8X4F_IP(x3, pIn3, 4*sizeof(uint8_t));
            t0 = AE_SEL16_7531(x0, x1);
            t1 = AE_SEL16_6420(x0, x1);
            t2 = AE_SEL16_7531(x2, x3);
            t3 = AE_SEL16_6420(x2, x3);
            x0 = AE_SEL16_7531(t0, t2);
            x2 = AE_SEL16_6420(t0, t2);
            x1 = AE_SEL16_7531(t1, t3);
            x3 = AE_SEL16_6420(t1, t3);
            AE_S16X4_IP(x0, pOut, sizeof(ae_int16x4));
            AE_S16X4_IP(x1, pOut, sizeof(ae_int16x4));
            AE_S16X4_IP(x2, pOut, sizeof(ae_int16x4));
            AE_S16X4_IP(x3, pOut, sizeof(ae_int16x4));
        }

        pIn = (const ae_int16x4 *)(t);
        pOut = (ae_int16x4 *)((uint8_t *)imgOut + n*ostride);
        pIdx = (const ae_int16x4 *)(ix);
        for (m = 0; m < (wout >> 3); m++)
        {
            ae_int16x4 idx, ofs;
            /* Load 4x8 elems for the 8 columns */
            AE_L16X4_IP(idx, castxcc(ae_int16x4, pIdx), sizeof(ae_int16x4));
            ofs = AE_SLAI16S(idx, 3); /* offset = idx*sizeof(ae_int16x4) */
            x0 = AE_L16X4_X(pIn, AE_MOVAD16_3(ofs));
            x0_ = AE_L16X4_X(pIn, AE_MOVAD16_2(ofs));
            x1 = AE_L16X4_X(pIn, AE_MOVAD16_1(ofs));
            x1_ = AE_L16X4_X(pIn, AE_MOVAD16_0(ofs));

            AE_L16X4_IP(idx, castxcc(ae_int16x4, pIdx), sizeof(ae_int16x4));
            ofs = AE_SLAI16S(idx, 3); /* offset = idx*sizeof(ae_int16x4) */
            x2 = AE_L16X4_X(pIn, AE_MOVAD16_3(ofs));
            x2_ = AE_L16X4_X(pIn, AE_MOVAD16_2(ofs));
            x3 = AE_L16X4_X(pIn, AE_MOVAD16_1(ofs));
            x3_ = AE_L16X4_X(pIn, AE_MOVAD16_0(ofs));

            t0 = AE_SEL16_7531(x0, x1);
            t1 = AE_SEL16_6420(x0, x1);
            t2 = AE_SEL16_7531(x2, x3);
            t3 = AE_SEL16_6420(x2, x3);
            x0 = AE_SEL16_7531(t0, t2);
            x2 = AE_SEL16_6420(t0, t2);
            x1 = AE_SEL16_7531(t1, t3);
            x3 = AE_SEL16_6420(t1, t3);

            t0 = AE_SEL16_7531(x0_, x1_);
            t1 = AE_SEL16_6420(x0_, x1_);
            t2 = AE_SEL16_7531(x2_, x3_);
            t3 = AE_SEL16_6420(x2_, x3_);
            x0_ = AE_SEL16_7531(t0, t2);
            x2_ = AE_SEL16_6420(t0, t2);
            x1_ = AE_SEL16_7531(t1, t3);
            x3_ = AE_SEL16_6420(t1, t3);

            x0 = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(x0), 8));
            x1 = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(x1), 8));
            x2 = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(x2), 8));
            x3 = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(x3), 8));
            x0 = AE_OR16(x0, x0_);
            x1 = AE_OR16(x1, x1_);
            x2 = AE_OR16(x2, x2_);
            x3 = AE_OR16(x3, x3_);

            AE_S16X4_XP(x0, pOut, ostride*sizeof(uint8_t));
            AE_S16X4_XP(x1, pOut, ostride*sizeof(uint8_t));
            AE_S16X4_XP(x2, pOut, ostride*sizeof(uint8_t));
            AE_S16X4_XP(x3, pOut, -3 * ostride*sizeof(uint8_t)+sizeof(ae_int16x4));
        }
        if (wout & 7)
        {
#if 0
            ae_int16x4 idx, ofs;
            int16_t* pout = (int16_t*)pOut;
            /* Load 4x4 elems for the 4 columns */
            AE_L16X4_IP(idx, castxcc(ae_int16x4, pIdx), sizeof(ae_int16x4));
            ofs = AE_SLAI16S(idx, 3); /* offset = idx*sizeof(ae_int16x4) */
            x0 = AE_L16X4_X(pIn, AE_MOVAD16_3(ofs));
            x1 = AE_L16X4_X(pIn, AE_MOVAD16_2(ofs));
            x2 = AE_L16X4_X(pIn, AE_MOVAD16_1(ofs));
            x3 = AE_L16X4_X(pIn, AE_MOVAD16_0(ofs));

            t0 = AE_SEL16_7531(x0, x1);
            t1 = AE_SEL16_6420(x0, x1);
            t2 = AE_SEL16_7531(x2, x3);
            t3 = AE_SEL16_6420(x2, x3);
            x0 = AE_SEL16_7531(t0, t2);
            x2 = AE_SEL16_6420(t0, t2);
            x1 = AE_SEL16_7531(t1, t3);
            x3 = AE_SEL16_6420(t1, t3);
            pout[0 * ostride + 0] = AE_MOVAD16_3(x0);
            pout[1 * ostride + 0] = AE_MOVAD16_3(x1);
            pout[2 * ostride + 0] = AE_MOVAD16_3(x2);
            pout[3 * ostride + 0] = AE_MOVAD16_3(x3);
            if (wout & 2)
            {
                pout[0 * ostride + 1] = AE_MOVAD16_2(x0);
                pout[1 * ostride + 1] = AE_MOVAD16_2(x1);
                pout[2 * ostride + 1] = AE_MOVAD16_2(x2);
                pout[3 * ostride + 1] = AE_MOVAD16_2(x3);
            }
            if ((wout & 3) == 3)
            {
                pout[0 * ostride + 2] = AE_MOVAD16_1(x0);
                pout[1 * ostride + 2] = AE_MOVAD16_1(x1);
                pout[2 * ostride + 2] = AE_MOVAD16_1(x2);
                pout[3 * ostride + 2] = AE_MOVAD16_1(x3);
            }
#else
            int i;
            for (i = 0; i < 4; i++)
            {
                pin = (uint8_t *)imgIn + (n+i)*istride;
                pout = ((uint8_t *)imgOut) + (n+i)*ostride;
                for (m = (wout&(~7)); m < wout; m++)
                {
                    pout[m] = pin[ix[m]];
                }
            }
#endif
        }

    }
#if 0
    /* Process last 0...3 rows */
    for (n = (hout & ~3); n < hout; n++)
    {
        pIn = (const ae_int16x4 *)((uint8_t *)imgIn + n*istride);
        pOut = (ae_int16x4 *)(t);
        __Pragma("loop_count min=1");
        for (m = 0; m < win; m += 8)
        {
            AE_L16X4_IP(x0, castxcc(ae_int16x4, pIn), sizeof(ae_int16x4));
            AE_S16X4_IP(x0, pOut, sizeof(ae_int16x4));
        }

        pIn = (const ae_int16x4 *)(t);
        pOut = (ae_int16x4 *)((uint8_t *)imgOut + n*ostride);
        pIdx = (const ae_int16x4 *)(ix);
        for (m = 0; m < (wout >> 3); m++)
        {
            ae_int16x4 idx, ofs;
            /* Load 1x4 elems for the 4 columns */
            AE_L16X4_IP(idx, castxcc(ae_int16x4, pIdx), sizeof(ae_int16x4));
            ofs = AE_SLAI16S(idx, 1); /* offset = idx*sizeof(int16_t) */
            x0 = AE_L16_X((const ae_int16 *)pIn, AE_MOVAD16_3(ofs));
            x1 = AE_L16_X((const ae_int16 *)pIn, AE_MOVAD16_2(ofs));
            x2 = AE_L16_X((const ae_int16 *)pIn, AE_MOVAD16_1(ofs));
            x3 = AE_L16_X((const ae_int16 *)pIn, AE_MOVAD16_0(ofs));

            x0 = AE_SEL16_5140(x0, x1);
            x2 = AE_SEL16_5140(x2, x3);
            x0 = AE_SEL16_5410(x0, x2);
            AE_S16X4_IP(x0, castxcc(ae_int16x4, pOut), sizeof(ae_int16x4));
        }
        if (wout & 7)
        {
#if 0
            ae_int16x4 idx, ofs;
            int16_t* pout = (int16_t*)pOut;
            /* Load 1x4 elems for the 4 columns */
            AE_L16X4_IP(idx, castxcc(ae_int16x4, pIdx), sizeof(ae_int16x4));
            ofs = AE_SLAI16S(idx, 1); /* offset = idx*sizeof(int16_t) */
            x0 = AE_L16_X((const ae_int16 *)pIn, AE_MOVAD16_3(ofs));
            x1 = AE_L16_X((const ae_int16 *)pIn, AE_MOVAD16_2(ofs));
            x2 = AE_L16_X((const ae_int16 *)pIn, AE_MOVAD16_1(ofs));
            x3 = AE_L16_X((const ae_int16 *)pIn, AE_MOVAD16_0(ofs));

            x0 = AE_SEL16_5140(x0, x1);
            x2 = AE_SEL16_5140(x2, x3);
            x0 = AE_SEL16_5410(x0, x2);
            pout[0 * ostride + 0] = AE_MOVAD16_3(x0);
            if (wout & 2)
            {
                pout[0 * ostride + 1] = AE_MOVAD16_2(x0);
            }
            if ((wout & 3) == 3)
            {
                pout[0 * ostride + 2] = AE_MOVAD16_1(x0);
            }
#else
            pin = (uint8_t *)pIn;
            pout = (uint8_t *)pOut;
            for (m = (wout&(~7)); m<wout; m++)
            {
                pout[m] = pin[ix[m]];
            }
#endif
        }
    }
#else
    /* Process last 0...3 rows */
    for (n = (hout & ~3); n<hout; n++)
    {
        pin = ((uint8_t *)imgIn) + n*istride;
        pout = ((uint8_t *)imgOut) + n*ostride;
        for (m = 0; m<wout; m++)
        {
            pout[m] = pin[ix[m]];
        }
    }
#endif 
}

/* 8 bit process */
static void process_gu8(void *pScr, void* pCoef, const void* imgIn, void*imgOut, const imgsize_t* in, const imgsize_t* out)
{
    const uint8_t * restrict pin;
    uint8_t * restrict pout;
    int16_t * restrict t = (int16_t *)pScr;
    const int16_t * restrict ix = (const int16_t *)pCoef;
    const ae_int16x4 * restrict pIn;
    const ae_int16x4 * restrict pIn0;
    const ae_int16x4 * restrict pIn1;
    const ae_int16x4 * restrict pIn2;
    const ae_int16x4 * restrict pIn3;
    ae_valign al_in, al_out;
    ae_valign al_in0, al_in1, al_in2, al_in3;
    ae_valign al_out0, al_out1, al_out2, al_out3;
    int shin, shout;
    int i;
    ae_int16x4 * restrict pOut;
    ae_int16x4 * restrict pOut0;
    ae_int16x4 * restrict pOut1;
    ae_int16x4 * restrict pOut2;
    ae_int16x4 * restrict pOut3;
    const ae_int16x4 * restrict pIdx;
    ae_int16x4 x0, x1, x2, x3;
    ae_int16x4 x0_, x1_, x2_, x3_;
    ae_int16x4 t0, t1, t2, t3;
    ae_int16x4 t0_, t1_, t2_, t3_;
    int m, n,
        win = in->width,
        wout = out->width,
        hout = out->height,
        ostride = out->stride,
        istride = in->stride;

    (void)win;
    imgsize_validate(in, 1, 0);
    imgsize_validate(out, 1, 0);
    NASSERT_ALIGN(pScr, ALIGNMENT);
    NASSERT(in->height == out->height);

    if (win == wout * 2)
    {
        /* 2x decimation */
        for (n = 0; n < hout; n++)
        {
            pIn = (const ae_int16x4 *)((uint8_t *)imgIn + n*istride);
            pOut = (ae_int16x4 *)((uint8_t *)imgOut + n*ostride);
            shout = (uintptr_t)pOut & 1;
            shin = (uintptr_t)pIn & 1;
            if (shout)
            {
                ((uint8_t*)pOut)[0] = ((uint8_t*)pIn)[0];
                pOut = (ae_int16x4 *)((uintptr_t)pOut + 1);
                pIn = (const ae_int16x4 *)((uintptr_t)pIn + 2);
            }
            if (shin == 0)
            {
                al_in = AE_LA64_PP(pIn);
                al_out = AE_ZALIGN64();
                for(m = 0; m < ((wout-shout) >> 3); m++)
                {
                    AE_LA16X4_IP(x0, al_in, pIn);
                    AE_LA16X4_IP(x1, al_in, pIn);
                    x0 = AE_AND16(x0, 0x00ff);
                    x1 = AE_AND16(x1, 0x00ff);
                    t0 = AE_SEL16_7531(x0, x1);
                    t1 = AE_SEL16_6420(x0, x1);
                    t1 = AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(t1), 8));
                    t0 = AE_OR16(t0, t1);
                    AE_SA16X4_IP(t0, al_out, pOut);
                }
                AE_SA64POS_FP(al_out, pOut);
            }
            else
            {
                /* unaligned input */
                t2 = AE_MOVDA16(((uint8_t*)pIn)[0]);
                pIn = (const ae_int16x4 *)((uintptr_t)pIn + 1);
                al_in = AE_LA64_PP(pIn);     
                al_out = AE_ZALIGN64();
                t2 = AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(t2), 8));
                for (m = 0; m < ((wout - shout) >> 3); m++)
                {
                    AE_LA16X4_IP(x0, al_in, pIn);
                    AE_LA16X4_IP(x1, al_in, pIn);
                    x0 = AE_AND16(x0, (int16_t)(0xff00));
                    x1 = AE_AND16(x1, (int16_t)(0xff00));
                    t1 = AE_SEL16_7531(x0, x1);
                    t0 = AE_SEL16_6420(x0, x1);
                    t0 = AE_SEL16_4321(t2, t0);
                    t0 = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(t0), 8));
                    t0 = AE_OR16(t0, t1);
                    t2 = x1;
                    AE_SA16X4_IP(t0, al_out, pOut);
                }
                AE_SA64POS_FP(al_out, pOut);
            }
            if ((wout-shout) & 7)
            {
                pin = (uint8_t *)pIn-shin;
                pout = (uint8_t *)pOut;
                for (m = 0; m<((wout-shout) & 7); m++)
                {
                    pout[m] = pin[2 * m];
                }
            }
        }
        return;
    }
    else if (win * 2 == wout)
    {
        /* 2x interpolation */
        for (n = 0; n < hout; n++)
        {
            pIn = (const ae_int16x4 *)((uint8_t *)imgIn + n*istride);
            pOut = (ae_int16x4 *)((uint8_t *)imgOut + n*ostride);
            shout = ((uintptr_t)pOut) & 1;
            shin = ((uintptr_t)pIn) & 1;
            if (shin)
            {
                ((uint8_t*)pOut)[0] = ((uint8_t*)pIn)[0];
                ((uint8_t*)pOut)[1] = ((uint8_t*)pIn)[0];
                pOut = (ae_int16x4 *)((uintptr_t)pOut + 2);
                pIn = (const ae_int16x4 *)((uintptr_t)pIn + 1);
            }
            if (shout == 0)
            {
                al_in = AE_LA64_PP(pIn);
                al_out = AE_ZALIGN64();
                for (m = 0; m < ((win-shin) >> 3); m++)
                {
                    AE_LA16X4_IP(x0, al_in, pIn);
                    t0 = AE_AND16(x0, 0x00ff);
                    t1 = AE_AND16(x0, (int16_t)(0xff00));
                    t0 = AE_OR16(t0, AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(t0), 8)));
                    t1 = AE_OR16(t1, AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(t1), 8)));
                    x0 = AE_SEL16_7362(t0, t1);
                    x1 = AE_SEL16_5140(t0, t1);
                    AE_SA16X4_IP(x0, al_out, pOut);
                    AE_SA16X4_IP(x1, al_out, pOut);
                }
                AE_SA64POS_FP(al_out, pOut);
                if ((win - shin) & 7)
                {
                    pin = (uint8_t *)pIn;
                    pout = (uint8_t *)pOut;
                    for (m = 0; m < ((win - shin) & 7); m++)
                    {
                        pout[2 * m + 0] = pin[m];
                        pout[2 * m + 1] = pin[m];
                    }
                }
            }
            else
            {
                pout = ((uint8_t*)pOut);
                pin = ((uint8_t*)pIn);
                pOut = (ae_int16x4 *)((uintptr_t)pOut + 1);
                al_in = AE_LA64_PP(pIn);
                al_out = AE_ZALIGN64();
                for (m = 0; m < ((win - shin-shout) >> 3); m++)
                {
                    AE_LA16X4_IP(x0, al_in, pIn);
                    t0 = AE_AND16(x0, 0x00ff);
                    t1 = AE_AND16(x0, (int16_t)(0xff00));
                    t1 = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(t1), 8));
                    t2 = AE_SEL16_7362(t0, t1);
                    t3 = AE_SEL16_5140(t0, t0);
                    t3 = AE_SEL16_6543(t2, t3);
                    t3 = AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(t3), 8));
                    x0 = AE_OR16(t2, t3);
                    AE_SA16X4_IP(x0, al_out, pOut);
                    t2 = AE_SEL16_5140(t0, t1);
                    t3 = AE_SEL16_6543(t2, t2);
                    t3 = AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(t3), 8));
                    x1 = AE_OR16(t2, t3);                   
                    AE_SA16X4_IP(x1, al_out, pOut);
                }
                AE_SA64POS_FP(al_out, pOut);
                for (m = 0; m < ((win - shin - shout) >> 3); m++)
                {
                    pout[(m)* 16] = pin[(m)* 8];
                }
                int right = (win - shin) - (((win - shin - shout)>>3)<<3);
                if (right)
                {
                    pin = (uint8_t *)pIn;
                    pout = (uint8_t *)pOut-1;
                    for (m = 0; m < right; m++)
                    {
                        pout[2 * m + 0] = pin[m];
                        pout[2 * m + 1] = pin[m];
                    }
                }
            }                       
        }
        return;
    }
    /* Process the image by 4 rows per iteration */
    for (n = 0; n < (hout & ~3); n += 4)
    {
        if ((istride & 1) == 0)
        {
            shin = ((uintptr_t)imgIn + n*istride) & 1;
            /* Interleave input samples from 4 rows and save them to the scratch */
            pIn0 = (const ae_int16x4 *)((uint8_t *)imgIn + (n + 0)*istride + shin);
            pIn1 = (const ae_int16x4 *)((uint8_t *)imgIn + (n + 1)*istride + shin);
            pIn2 = (const ae_int16x4 *)((uint8_t *)imgIn + (n + 2)*istride + shin);
            pIn3 = (const ae_int16x4 *)((uint8_t *)imgIn + (n + 3)*istride + shin);
            al_in0 = AE_LA64_PP(pIn0);
            al_in1 = AE_LA64_PP(pIn1);
            al_in2 = AE_LA64_PP(pIn2);
            al_in3 = AE_LA64_PP(pIn3);
            pOut = (ae_int16x4 *)t + shin;
            pOut0 = (ae_int16x4 *)t + 1 + shin;
            if (shin)
            {
                pin = (uint8_t*)imgIn + n*istride;
                t[0] = ((int16_t)pin[0 * istride]) << 8;
                t[1] = ((int16_t)pin[1 * istride]) << 8;
                t[2] = ((int16_t)pin[2 * istride]) << 8;
                t[3] = ((int16_t)pin[3 * istride]) << 8;
            }
            __Pragma("loop_count min=1");
            for (m = 0; m < (win - shin); m += 8)
            {
                AE_LA16X4_IP(x0, al_in0, pIn0);
                AE_LA16X4_IP(x1, al_in1, pIn1);
                AE_LA16X4_IP(x2, al_in2, pIn2);
                AE_LA16X4_IP(x3, al_in3, pIn3);
                x0_ = AE_AND16(x0, (int16_t)(0xff00));
                x1_ = AE_AND16(x1, (int16_t)(0xff00));
                x2_ = AE_AND16(x2, (int16_t)(0xff00));
                x3_ = AE_AND16(x3, (int16_t)(0xff00));
                x0 = AE_AND16(x0, 0x00ff);
                x1 = AE_AND16(x1, 0x00ff);
                x2 = AE_AND16(x2, 0x00ff);
                x3 = AE_AND16(x3, 0x00ff);
                x0 = AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(x0), 8));
                x1 = AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(x1), 8));
                x2 = AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(x2), 8));
                x3 = AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(x3), 8));
                t0 = AE_SEL16_7531(x0, x1);
                t1 = AE_SEL16_6420(x0, x1);
                t2 = AE_SEL16_7531(x2, x3);
                t3 = AE_SEL16_6420(x2, x3);
                x0 = AE_SEL16_7531(t0, t2);
                x2 = AE_SEL16_6420(t0, t2);
                x1 = AE_SEL16_7531(t1, t3);
                x3 = AE_SEL16_6420(t1, t3);
                AE_S16X4_IP(x0, pOut, 2 * sizeof(ae_int16x4));
                AE_S16X4_IP(x1, pOut, 2 * sizeof(ae_int16x4));
                AE_S16X4_IP(x2, pOut, 2 * sizeof(ae_int16x4));
                AE_S16X4_IP(x3, pOut, 2 * sizeof(ae_int16x4));
                t0_ = AE_SEL16_7531(x0_, x1_);
                t1_ = AE_SEL16_6420(x0_, x1_);
                t2_ = AE_SEL16_7531(x2_, x3_);
                t3_ = AE_SEL16_6420(x2_, x3_);
                x0_ = AE_SEL16_7531(t0_, t2_);
                x2_ = AE_SEL16_6420(t0_, t2_);
                x1_ = AE_SEL16_7531(t1_, t3_);
                x3_ = AE_SEL16_6420(t1_, t3_);
                AE_S16X4_IP(x0_, pOut0, 2 * sizeof(ae_int16x4));
                AE_S16X4_IP(x1_, pOut0, 2 * sizeof(ae_int16x4));
                AE_S16X4_IP(x2_, pOut0, 2 * sizeof(ae_int16x4));
                AE_S16X4_IP(x3_, pOut0, 2 * sizeof(ae_int16x4));
            }
        }
        else
        {
            pin = (uint8_t*)imgIn + n*istride;
            shin = ((uintptr_t)imgIn + n*istride) & 1;
            /* Interleave input samples from 4 rows and save them to the scratch */
            pIn0 = (const ae_int16x4 *)((uint8_t *)imgIn + (n + 0)*istride + shin);
            pIn1 = (const ae_int16x4 *)((uint8_t *)imgIn + (n + 1)*istride + shin+1);
            pIn2 = (const ae_int16x4 *)((uint8_t *)imgIn + (n + 2)*istride + shin);
            pIn3 = (const ae_int16x4 *)((uint8_t *)imgIn + (n + 3)*istride + shin+1);
            al_in0 = AE_LA64_PP(pIn0);
            al_in1 = AE_LA64_PP(pIn1);
            al_in2 = AE_LA64_PP(pIn2);
            al_in3 = AE_LA64_PP(pIn3);
            ae_int16x4 tmp1, tmp3, tmp;
            pOut = (ae_int16x4 *)t + shin;
            pOut0 = (ae_int16x4 *)t + 1 + shin;
            if (shin)
            {                
                t[0] = ((int16_t)pin[0 * istride]) << 8;
                t[1] = ((int16_t)pin[1 * istride]) << 8;
                t[2] = ((int16_t)pin[2 * istride]) << 8;
                t[3] = ((int16_t)pin[3 * istride]) << 8;
            }
            tmp1 = AE_MOVDA16((int16_t)(pin[istride + shin]) << 8);
            tmp3 = AE_MOVDA16((int16_t)(pin[3 * istride + shin]) << 8);
            __Pragma("loop_count min=1");
            for (m = 0; m < (win - shin); m += 8)
            {
                AE_LA16X4_IP(x0, al_in0, pIn0);
                AE_LA16X4_IP(x1, al_in1, pIn1);
                AE_LA16X4_IP(x2, al_in2, pIn2);
                AE_LA16X4_IP(x3, al_in3, pIn3);
                x0_ = AE_AND16(x0, (int16_t)(0xff00));
                x1_ = AE_AND16(x1, 0x00ff);
                x2_ = AE_AND16(x2, (int16_t)(0xff00));
                x3_ = AE_AND16(x3, 0x00ff);
                x1_ = AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(x1_), 8));
                x3_ = AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(x3_), 8));
                x0 = AE_AND16(x0, 0x00ff);
                x1 = AE_AND16(x1, (int16_t)(0xff00));
                x2 = AE_AND16(x2, 0x00ff);
                x3 = AE_AND16(x3, (int16_t)(0xff00));
                x0 = AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(x0), 8));
                x2 = AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(x2), 8));
                tmp = x1;
                x1 = AE_SEL16_4321(tmp1, x1);
                tmp1 = tmp;
                tmp = x3;
                x3 = AE_SEL16_4321(tmp3, x3);
                tmp3 = tmp;
                t0 = AE_SEL16_7531(x0, x1);
                t1 = AE_SEL16_6420(x0, x1);
                t2 = AE_SEL16_7531(x2, x3);
                t3 = AE_SEL16_6420(x2, x3);
                x0 = AE_SEL16_7531(t0, t2);
                x2 = AE_SEL16_6420(t0, t2);
                x1 = AE_SEL16_7531(t1, t3);
                x3 = AE_SEL16_6420(t1, t3);
                AE_S16X4_IP(x0, pOut, 2 * sizeof(ae_int16x4));
                AE_S16X4_IP(x1, pOut, 2 * sizeof(ae_int16x4));
                AE_S16X4_IP(x2, pOut, 2 * sizeof(ae_int16x4));
                AE_S16X4_IP(x3, pOut, 2 * sizeof(ae_int16x4));
                t0_ = AE_SEL16_7531(x0_, x1_);
                t1_ = AE_SEL16_6420(x0_, x1_);
                t2_ = AE_SEL16_7531(x2_, x3_);
                t3_ = AE_SEL16_6420(x2_, x3_);
                x0_ = AE_SEL16_7531(t0_, t2_);
                x2_ = AE_SEL16_6420(t0_, t2_);
                x1_ = AE_SEL16_7531(t1_, t3_);
                x3_ = AE_SEL16_6420(t1_, t3_);
                AE_S16X4_IP(x0_, pOut0, 2 * sizeof(ae_int16x4));
                AE_S16X4_IP(x1_, pOut0, 2 * sizeof(ae_int16x4));
                AE_S16X4_IP(x2_, pOut0, 2 * sizeof(ae_int16x4));
                AE_S16X4_IP(x3_, pOut0, 2 * sizeof(ae_int16x4));
            }
        }
        if ((ostride & 1) == 0)
        {
            pOut = (ae_int16x4 *)((uint8_t *)imgOut + n*ostride);
            shout = (uintptr_t)pOut & 1;
            pIdx = (const ae_int16x4 *)(ix + shout);
            al_in = AE_LA64_PP(pIdx);
            pOut0 = (ae_int16x4 *)((uint8_t *)imgOut + (n + 0)*ostride + shout);
            pOut1 = (ae_int16x4 *)((uint8_t *)imgOut + (n + 1)*ostride + shout);
            pOut2 = (ae_int16x4 *)((uint8_t *)imgOut + (n + 2)*ostride + shout);
            pOut3 = (ae_int16x4 *)((uint8_t *)imgOut + (n + 3)*ostride + shout);
            al_out0 = AE_ZALIGN64();
            al_out1 = AE_ZALIGN64();
            al_out2 = AE_ZALIGN64();
            al_out3 = AE_ZALIGN64();
            pIn = (const ae_int16x4 *)t;
            if (shout)
            {
                int i;
                for (i = 0; i < 4; i++)
                {
                    pin = ((uint8_t *)imgIn) + (n + i)*istride;
                    pout = ((uint8_t *)imgOut) + (n + i)*ostride;
                    pout[0] = pin[ix[0]];
                }

            }
            for (m = 0; m < ((wout - shout) >> 3); m++)
            {
                ae_int16x4 idx, ofs;
                /* Load 4x8 elems for the 8 columns */
                AE_LA16X4_IP(idx, al_in, pIdx);
                ofs = AE_SLAI16S(idx, 3); /* offset = idx*sizeof(ae_int16x4) */
                x0 = AE_L16X4_X(pIn, AE_MOVAD16_3(ofs));
                x0_ = AE_L16X4_X(pIn, AE_MOVAD16_2(ofs));
                x1 = AE_L16X4_X(pIn, AE_MOVAD16_1(ofs));
                x1_ = AE_L16X4_X(pIn, AE_MOVAD16_0(ofs));

                AE_LA16X4_IP(idx, al_in, pIdx);
                ofs = AE_SLAI16S(idx, 3); /* offset = idx*sizeof(ae_int16x4) */
                x2 = AE_L16X4_X(pIn, AE_MOVAD16_3(ofs));
                x2_ = AE_L16X4_X(pIn, AE_MOVAD16_2(ofs));
                x3 = AE_L16X4_X(pIn, AE_MOVAD16_1(ofs));
                x3_ = AE_L16X4_X(pIn, AE_MOVAD16_0(ofs));

                t0 = AE_SEL16_7531(x0, x1);
                t1 = AE_SEL16_6420(x0, x1);
                t2 = AE_SEL16_7531(x2, x3);
                t3 = AE_SEL16_6420(x2, x3);
                x0 = AE_SEL16_7531(t0, t2);
                x2 = AE_SEL16_6420(t0, t2);
                x1 = AE_SEL16_7531(t1, t3);
                x3 = AE_SEL16_6420(t1, t3);

                t0 = AE_SEL16_7531(x0_, x1_);
                t1 = AE_SEL16_6420(x0_, x1_);
                t2 = AE_SEL16_7531(x2_, x3_);
                t3 = AE_SEL16_6420(x2_, x3_);
                x0_ = AE_SEL16_7531(t0, t2);
                x2_ = AE_SEL16_6420(t0, t2);
                x1_ = AE_SEL16_7531(t1, t3);
                x3_ = AE_SEL16_6420(t1, t3);

                x0 = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(x0), 8));
                x1 = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(x1), 8));
                x2 = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(x2), 8));
                x3 = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(x3), 8));
                x0 = AE_OR16(x0, x0_);
                x1 = AE_OR16(x1, x1_);
                x2 = AE_OR16(x2, x2_);
                x3 = AE_OR16(x3, x3_);

                AE_SA16X4_IP(x0, al_out0, pOut0);
                AE_SA16X4_IP(x1, al_out1, pOut1);
                AE_SA16X4_IP(x2, al_out2, pOut2);
                AE_SA16X4_IP(x3, al_out3, pOut3);
            }
            AE_SA64POS_FP(al_out0, pOut0);
            AE_SA64POS_FP(al_out1, pOut1);
            AE_SA64POS_FP(al_out2, pOut2);
            AE_SA64POS_FP(al_out3, pOut3);
            pOut = pOut0;
            if ((wout - shout) & 7)
            {
                int i;
                for (i = 0; i < 4; i++)
                {
                    pin = (uint8_t *)imgIn + (n + i)*istride;
                    pout = ((uint8_t *)imgOut) + (n + i)*ostride;
                    for (m = shout + ((wout - shout)&(~7)); m < wout; m++)
                    {
                        pout[m] = pin[ix[m]];
                    }
                }
            }
        }
        else
        {
            pOut = (ae_int16x4 *)((uint8_t *)imgOut + n*ostride);
            shout = (uintptr_t)pOut & 1;
            pIdx = (const ae_int16x4 *)(ix + shout);
            al_in = AE_LA64_PP(pIdx);
            pOut0 = (ae_int16x4 *)((uint8_t *)imgOut + (n + 0)*ostride + shout);
            pOut1 = (ae_int16x4 *)((uint8_t *)imgOut + (n + 1)*ostride + shout + 1);
            pOut2 = (ae_int16x4 *)((uint8_t *)imgOut + (n + 2)*ostride + shout);
            pOut3 = (ae_int16x4 *)((uint8_t *)imgOut + (n + 3)*ostride + shout + 1);
            al_out0 = AE_ZALIGN64();
            al_out1 = AE_ZALIGN64();
            al_out2 = AE_ZALIGN64();
            al_out3 = AE_ZALIGN64();
            pIn = (const ae_int16x4 *)t;
            if (shout)
            {
                for (i = 0; i < 4; i++)
                {
                    pin = ((uint8_t *)imgIn) + (n + i)*istride;
                    pout = ((uint8_t *)imgOut) + (n + i)*ostride;
                    pout[0] = pin[ix[0]];
                }

            }
            for (m = 0; m < ((wout - shout-1) >> 3); m++)
            {
                ae_int16x4 idx, ofs;
                /* Load 4x8 elems for the 8 columns */
                AE_LA16X4_IP(idx, al_in, pIdx);
                ofs = AE_SLAI16S(idx, 3); /* offset = idx*sizeof(ae_int16x4) */
                x0 = AE_L16X4_X(pIn, AE_MOVAD16_3(ofs));
                x0_ = AE_L16X4_X(pIn, AE_MOVAD16_2(ofs));
                x1 = AE_L16X4_X(pIn, AE_MOVAD16_1(ofs));
                x1_ = AE_L16X4_X(pIn, AE_MOVAD16_0(ofs));

                AE_LA16X4_IP(idx, al_in, pIdx);
                ofs = AE_SLAI16S(idx, 3); /* offset = idx*sizeof(ae_int16x4) */
                x2 = AE_L16X4_X(pIn, AE_MOVAD16_3(ofs));
                x2_ = AE_L16X4_X(pIn, AE_MOVAD16_2(ofs));
                x3 = AE_L16X4_X(pIn, AE_MOVAD16_1(ofs));
                x3_ = AE_L16X4_X(pIn, AE_MOVAD16_0(ofs));

                t0 = AE_SEL16_7531(x0, x1);
                t1 = AE_SEL16_6420(x0, x1);
                t2 = AE_SEL16_7531(x2, x3);
                t3 = AE_SEL16_6420(x2, x3);
                x0 = AE_SEL16_7531(t0, t2);
                x2 = AE_SEL16_6420(t0, t2);
                x1 = AE_SEL16_7531(t1, t3);
                x3 = AE_SEL16_6420(t1, t3);

                t0 = AE_SEL16_7531(x0_, x1_);
                t1 = AE_SEL16_6420(x0_, x1_);
                t2 = AE_SEL16_7531(x2_, x3_);
                t3 = AE_SEL16_6420(x2_, x3_);
                x0_ = AE_SEL16_7531(t0, t2);
                x2_ = AE_SEL16_6420(t0, t2);
                x1_ = AE_SEL16_7531(t1, t3);
                x3_ = AE_SEL16_6420(t1, t3);

                x0 = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(x0), 8));
                x1_ = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(x1_), 8));
                x2 = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(x2), 8));
                x3_ = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(x3_), 8));
                x1 = AE_SEL16_6543(x1, (ae_int16x4)0);
                x3 = AE_SEL16_6543(x3, (ae_int16x4)0);
                x0 = AE_OR16(x0, x0_);
                x1 = AE_OR16(x1, x1_);
                x2 = AE_OR16(x2, x2_);
                x3 = AE_OR16(x3, x3_);

                AE_SA16X4_IP(x0, al_out0, pOut0);
                AE_SA16X4_IP(x1, al_out1, pOut1);
                AE_SA16X4_IP(x2, al_out2, pOut2);
                AE_SA16X4_IP(x3, al_out3, pOut3);
            }
            AE_SA64POS_FP(al_out0, pOut0);
            AE_SA64POS_FP(al_out1, pOut1);
            AE_SA64POS_FP(al_out2, pOut2);
            AE_SA64POS_FP(al_out3, pOut3);
            for (i = 0; i < 2; i++)
            {
                pin = ((uint8_t *)imgIn) + (n + 2 * i + 1)*istride;
                pout = ((uint8_t *)imgOut) + (n + 2 * i + 1)*ostride + shout;
                for (m = 0; m < ((wout - shout-1) >> 3); m++)
                {
                    pout[8 * m] = pin[ix[8 * m+shout]];
                }
            }
            int right =wout - (shout + (((wout - shout - 1) >> 3) << 3));
            if (right)
            {
                for (i = 0; i < 4; i++)
                {
                    pin = (uint8_t *)imgIn + (n + i)*istride;
                    pout = ((uint8_t *)imgOut) + (n + i)*ostride;
                    for (m = wout-right; m < wout; m++)
                    {
                        pout[m] = pin[ix[m]];
                    }
                }
            }                   
        }
    }
    /* Process last 0...3 rows */
    for (n = (hout & ~3); n<hout; n++)
    {
        pin = ((uint8_t *)imgIn) + n*istride;
        pout = ((uint8_t *)imgOut) + n*ostride;
        for (m = 0; m<wout; m++)
        {
            pout[m] = pin[ix[m]];
        }
    }
}

const imgresizer_api_t imgresizer_api_nh={NULL,getCoefSz,getCoef,getScratchSize,process};
