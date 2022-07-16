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

/*    image resizer, nearest, vertical direction */

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
    int m,n;
    for (n=0; n<N; n++)
    {
        float32_t u;
        u=((n+0.5f)*M)/N;
        m=(int)floorf(u);
        m=MIN(M-1,MAX(0,m));
        ix[n]=m;
    }
}
#endif

/* returns size of coefficients */
static size_t getCoefSz(const imgsize_t* in,const imgsize_t* out)
{
    int hin=in->height,hout=out->height;
    (void)hin,(void)hout;
    (void)in,(void)out;
    imgsize_validate(in,2,0);
    imgsize_validate(out,2,0);
    NASSERT(in->width == out->width);
    return sizeof(int16_t)*hout;
}
/* returns coefficients */
static void getCoef(void* coef, const imgsize_t* in,const imgsize_t* out)
{
    int hin=in->height,hout=out->height;
    (void)in,(void)out;
    imgsize_validate(in,2,0);
    imgsize_validate(out,2,0);
    NASSERT(in->width == out->width);
    getIdx((int16_t*)coef,hin,hout);
}

static size_t getScratchSize(const imgsize_t* in,const imgsize_t* out)
{
    (void)in,(void)out;
    imgsize_validate(in,2,0);
    imgsize_validate(out,2,0);
    return 0;
}
static void process_gs16(void *pScr, void* pCoef, const void* imgIn, void*imgOut, const imgsize_t* in, const imgsize_t* out);
static void process_fast_gu8(void *pScr, void* pCoef, const void* imgIn, void*imgOut, const imgsize_t* in, const imgsize_t* out);
static void process_gu8(void *pScr, void* pCoef, const void* imgIn, void*imgOut, const imgsize_t* in, const imgsize_t* out);

/* not in-place image resize */
static void process(void *pScr, void* pCoef, const void* imgIn, void*imgOut, const imgsize_t* in, const imgsize_t* out, int fast)
{
    const ae_int16x4 * restrict pIn;
          ae_int16x4 * restrict pOut;
    const int16_t * restrict ix = (const int16_t*)pCoef;
    ae_int16x4 x0;
    int m, n,
        wout = out->width,
        hin = in->height,
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
        return process_gs16(pScr, pCoef, imgIn, imgOut, in, out);
    }

    (void)hin;
    imgsize_validate(in, 2, 1);
    imgsize_validate(out, 2, 1);
    NASSERT_ALIGN(imgIn, ALIGNMENT);
    NASSERT_ALIGN(imgOut, ALIGNMENT);
    NASSERT_ALIGN(pScr, ALIGNMENT);
    NASSERT(in->width == wout);// && in->stride == out->stride);

    ostride = ostride * sizeof(int16_t);
    istride = istride * sizeof(int16_t);
    if (hin < hout)
    {
        /* Upsampling */
        /* Go from the last pixels to the first in the column */
        for (n = hout - 1; n >= 0; n--)
        {                    
            pIn = (const ae_int16x4 *)((uintptr_t)imgIn + (ix[n] * istride));
            pOut = (ae_int16x4 *)((uintptr_t)imgOut + n*ostride);
            for (m = 0; m < (wout >> 2); m++)
            {
                AE_L16X4_IP(x0, pIn, 4*sizeof(int16_t));
                AE_S16X4_IP(x0, pOut, 4 * sizeof(int16_t));
            }
            if (wout & 3)
            {
                int16_t* pout = (int16_t*)pOut;
                AE_L16X4_IP(x0, pIn, 4 * sizeof(int16_t));
                pout[0] = AE_MOVAD16_3(x0);
                if (wout & 2)
                {
                    pout[1] = AE_MOVAD16_2(x0);
                }
                if ((wout & 3) == 3)
                {
                    pout[2] = AE_MOVAD16_1(x0);
                }
            }
        }
    }
    else
    {
        /* Downsampling */
        /* Go from the first pixels to the last in the column */
        for (n = 0; n < hout; n++)
        {
            pIn = (const ae_int16x4 *)((uintptr_t)imgIn + ix[n] * istride);
            pOut = (ae_int16x4 *)((uintptr_t)imgOut + n*ostride);
            for (m = 0; m < (wout >> 2); m++)
            {
                AE_L16X4_IP(x0, pIn, 4 * sizeof(int16_t));
                AE_S16X4_IP(x0, pOut, 4 * sizeof(int16_t));
            }
            if (wout & 3)
            {
                int16_t* pout = (int16_t*)pOut;
                AE_L16X4_IP(x0, pIn, 4 * sizeof(int16_t));
                pout[0] = AE_MOVAD16_3(x0);
                if (wout & 2)
                {
                    pout[1] = AE_MOVAD16_2(x0);
                }
                if ((wout & 3) == 3)
                {
                    pout[2] = AE_MOVAD16_1(x0);
                }
            }
        }
    }
}

/* not in-place image resize, specialised for non aligned 16 bit images */
static void process_gs16(void *pScr, void* pCoef, const void* imgIn, void*imgOut, const imgsize_t* in, const imgsize_t* out)
{
    const ae_int16x4 * restrict pIn;
    ae_int16x4 * restrict pOut;
    const int16_t * restrict ix = (const int16_t*)pCoef;
    ae_int16x4 x0;
    ae_valign al_in, al_out;
    int m, n,
        wout = out->width,
        hin = in->height,
        hout = out->height,
        ostride = out->stride,
        istride = in->stride;

    (void)hin;
    imgsize_validate(in, 2, 0);
    imgsize_validate(out, 2, 0);
    NASSERT_ALIGN(imgIn, 2);
    NASSERT_ALIGN(imgOut, 2);
    NASSERT_ALIGN(pScr, ALIGNMENT);
    NASSERT(in->width == wout);// && in->stride == out->stride);

    ostride = ostride * sizeof(int16_t);
    istride = istride * sizeof(int16_t);

    if (hin < hout)
    {
        /* Upsampling */
        /* Go from the last pixels to the first in the column */
        for (n = hout - 1; n >= 0; n--)
        {                    
            pIn = (const ae_int16x4 *)((uintptr_t)imgIn + (ix[n] * istride));
            al_in = AE_LA64_PP(pIn);
            pOut = (ae_int16x4 *)((uintptr_t)imgOut + n*ostride);
            al_out = AE_ZALIGN64();
            for (m = 0; m < (wout >> 2); m++)
            {
                AE_LA16X4_IP(x0, al_in, pIn);
                AE_SA16X4_IP(x0, al_out, pOut);
            }
            AE_SA64POS_FP(al_out, pOut);
            if (wout & 3)
            {
                int16_t* pout = (int16_t*)pOut;
                AE_LA16X4_IP(x0, al_in, pIn);
                pout[0] = AE_MOVAD16_3(x0);
                if (wout & 2)
                {
                    pout[1] = AE_MOVAD16_2(x0);
                }
                if ((wout & 3) == 3)
                {
                    pout[2] = AE_MOVAD16_1(x0);
                }
            }
        }
    }
    else
    {
        /* Downsampling */
        /* Go from the first pixels to the last in the column */
        for (n = 0; n < hout; n++)
        {
            pIn = (const ae_int16x4 *)((uintptr_t)imgIn + ix[n] * istride);
            al_in = AE_LA64_PP(pIn);
            pOut = (ae_int16x4 *)((uintptr_t)imgOut + n*ostride);
            al_out = AE_ZALIGN64();
            for (m = 0; m < (wout >> 2); m++)
            {
                AE_LA16X4_IP(x0, al_in, pIn);
                AE_SA16X4_IP(x0, al_out, pOut);
            }
            AE_SA64POS_FP(al_out, pOut);
            if (wout & 3)
            {
                int16_t* pout = (int16_t*)pOut;
                AE_LA16X4_IP(x0, al_in, pIn);
                pout[0] = AE_MOVAD16_3(x0);
                if (wout & 2)
                {
                    pout[1] = AE_MOVAD16_2(x0);
                }
                if ((wout & 3) == 3)
                {
                    pout[2] = AE_MOVAD16_1(x0);
                }
            }
        }
    }
}

/* 8 bit specific process for fast */
static void process_fast_gu8(void *pScr, void* pCoef, const void* imgIn, void*imgOut, const imgsize_t* in, const imgsize_t* out)
{
    const ae_int16x4 * restrict pIn;
    ae_int16x4 * restrict pOut;
    const int16_t * restrict ix = (const int16_t*)pCoef;
    ae_int16x4 x0;
    int m, n,
        wout = out->width,
        hin = in->height,
        hout = out->height,
        ostride = out->stride,
        istride = in->stride;


    (void)hin;
    imgsize_validate(in, 1, 1);
    imgsize_validate(out, 1, 1);
    NASSERT_ALIGN(imgIn, ALIGNMENT);
    NASSERT_ALIGN(imgOut, ALIGNMENT);
    NASSERT_ALIGN(pScr, ALIGNMENT);
    NASSERT(in->width == wout);

    if (hin < hout)
    {
        /* Upsampling */
        /* Go from the last pixels to the first in the column */
        for (n = hout - 1; n >= 0; n--)
        {
            pIn = (const ae_int16x4 *)((uintptr_t)imgIn + (ix[n] * istride));
            pOut = (ae_int16x4 *)((uintptr_t)imgOut + n*ostride);
            for (m = 0; m < (wout >> 3); m++)
            {
                AE_L16X4_IP(x0, pIn, 4 * sizeof(int16_t));
                AE_S16X4_IP(x0, pOut, 4 * sizeof(int16_t));
            }
            if (wout & 7)
            {
                uint8_t* pout = (uint8_t*)pOut;
                uint8_t * pin = (uint8_t*)pIn;
                for (m = 0; m < (wout & 7); m++)
                {
                    pout[m] = pin[m];
                }
            }
        }
    }
    else
    {
        /* Downsampling */
        /* Go from the first pixels to the last in the column */
        for (n = 0; n < hout; n++)
        {
            pIn = (const ae_int16x4 *)((uintptr_t)imgIn + ix[n] * istride);
            pOut = (ae_int16x4 *)((uintptr_t)imgOut + n*ostride);
            for (m = 0; m < (wout >> 3); m++)
            {
                AE_L16X4_IP(x0, pIn, 4 * sizeof(int16_t));
                AE_S16X4_IP(x0, pOut, 4 * sizeof(int16_t));
            }
            if (wout & 7)
            {
                uint8_t* pout = (uint8_t*)pOut;
                uint8_t * pin = (uint8_t*)pIn;
                for (m = 0; m < (wout & 7); m++)
                {
                    pout[m] = pin[m];
                }
            }
        }
    }
}

/* 8 bit specific process */
static void process_gu8(void *pScr, void* pCoef, const void* imgIn, void*imgOut, const imgsize_t* in, const imgsize_t* out)
{
    const ae_int16x4 * restrict pIn;
    ae_int16x4 * restrict pOut;
    const uint8_t* restrict pin;
    uint8_t * restrict pout;
    ae_valign al_in, al_out;
    const int16_t * restrict ix = (const int16_t*)pCoef;
    ae_int16x4 x0, t0, t1, t2, t;
    int shin, shout;
    int m, n,
        wout = out->width,
        hin = in->height,
        hout = out->height,
        ostride = out->stride,
        istride = in->stride;


    (void)hin;
    imgsize_validate(in, 1, 0);
    imgsize_validate(out, 1, 0);
    NASSERT_ALIGN(pScr, ALIGNMENT);
    NASSERT(in->width == wout);
    al_out = AE_ZALIGN64();
    if (hin < hout)
    {
        /* Upsampling */
        /* Go from the last pixels to the first in the column */
        for (n = hout - 1; n >= 0; n--)
        {
            pIn = (const ae_int16x4 *)((uintptr_t)imgIn + (ix[n] * istride));
            pOut = (ae_int16x4 *)((uintptr_t)imgOut + n*ostride);
            shout = (uintptr_t)pOut & 1;
            shin = (uintptr_t)pIn & 1;
            if (shout)
            {
                ((uint8_t*)pOut)[0] = ((uint8_t*)pIn)[0];
                pOut = (ae_int16x4 *)((uintptr_t)pOut+ 1);
                pIn = (const ae_int16x4 *)((uintptr_t)pIn + 1);
            }

            if (shin == shout)
            {
                al_in = AE_LA64_PP(pIn);
                for (m = 0; m < ((wout - shout) >> 3); m++)
                {
                    AE_LA16X4_IP(x0, al_in, pIn);
                    AE_SA16X4_IP(x0, al_out, pOut);
                }
                AE_SA64POS_FP(al_out, pOut);
            }
            else
            {
                /*
                 8 elements x 8bit unaligned load imitation.
                 Preload one 8 bit element manually, then it is possible to load 8 elements
                 with LA16X4_IP, using 7 of them to form result, and last one for new preload element.
                */
                t = AE_MOVDA16(((uint8_t*)pIn)[0]);
                pIn = (const ae_int16x4 *)((uintptr_t)pIn + 1);
                al_in = AE_LA64_PP(pIn);                
                for (m = 0; m < ((wout - shout) >> 3); m++)
                {
                    t2 = t;
                    AE_LA16X4_IP(x0, al_in, pIn);                   
                    t0 = AE_AND16(x0, 0x00ff);
                    t1 = AE_AND16(x0, (int16_t)(0xff00));
                    t1 = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(t1), 8));
                    t0 = AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(t0), 8));
                    t = t1;
                    t1 = AE_SEL16_4321(t2, t1);                    
                    x0 = AE_OR16(t0, t1);
                    AE_SA16X4_IP(x0, al_out, pOut);
                }
                AE_SA64POS_FP(al_out, pOut);
            }
            
            if ((wout-shout) & 7)
            {
                pout = (uint8_t*)pOut;
                pin = (uint8_t*)((uintptr_t)imgIn + (ix[n] * istride) + ((wout - shout) & (~7)) + shout);
                for (m = 0; m < ((wout-shout) & 7); m++)
                {
                    pout[m] = pin[m];
                }
            }
        }
    }
    else
    {
        for (n = hout - 1; n >= 0; n--)
        {
            pIn = (const ae_int16x4 *)((uintptr_t)imgIn + (ix[n] * istride));
            pOut = (ae_int16x4 *)((uintptr_t)imgOut + n*ostride);
            shout = (uintptr_t)pOut & 1;
            shin = (uintptr_t)pIn & 1;
            if (shout)
            {
                ((uint8_t*)pOut)[0] = ((uint8_t*)pIn)[0];
                pOut = (ae_int16x4 *)((uintptr_t)pOut+ 1);
                pIn = (const ae_int16x4 *)((uintptr_t)pIn + 1);
            }

            if (shin == shout)
            {
                al_in = AE_LA64_PP(pIn);
                for (m = 0; m < ((wout - shout) >> 3); m++)
                {
                    AE_LA16X4_IP(x0, al_in, pIn);
                    AE_SA16X4_IP(x0, al_out, pOut);
                }
                AE_SA64POS_FP(al_out, pOut);
            }
            else
            {
                pin = (uint8_t*)pIn;
                pout = (uint8_t*)pOut;
                pIn = (const ae_int16x4 *)((uintptr_t)pIn + 1);
                al_in = AE_LA64_PP(pIn);
                for (m = 0; m < ((wout - shout) >> 3); m++)
                {
                    AE_LA16X4_IP(x0, al_in, pIn);
                    t0 = AE_AND16(x0, 0x00ff);
                    t1 = AE_AND16(x0, (int16_t)(0xff00));
                    t1 = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(t1), 8));
                    t0 = AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(t0), 8));
                    t2 = AE_MOVDA16(pin[m * 8]);
                    t1 = AE_SEL16_4321(t2, t1);
                    x0 = AE_OR16(t0, t1);
                    AE_SA16X4_IP(x0, al_out, pOut);
                }
                AE_SA64POS_FP(al_out, pOut);
            }

            if ((wout - shout) & 7)
            {
                pout = (uint8_t*)pOut;
                pin = (uint8_t*)((uintptr_t)imgIn + (ix[n] * istride) + ((wout - shout) & (~7)) + shout);
                for (m = 0; m < ((wout - shout) & 7); m++)
                {
                    pout[m] = pin[m];
                }
            }
        }
    }
}

const imgresizer_api_t imgresizer_api_nv={NULL,getCoefSz,getCoef,getScratchSize,process};
