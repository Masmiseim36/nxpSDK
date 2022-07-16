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
#include "imgrotate_common.h"

static const int32_t coef_cosine[]={-685797,34038740,-662336450,2147483607};
static const int32_t coef_sine[]  ={-77133,5347095,-173399414,1686629696};
static const int32_t coef_tan[]   ={191166,2656537,43354474,843314543};

/* cos(pi*x): valid for -1/4...1/4
   Input:
   x - Q31
   Output:
   result in Q31
   Accuracy ~ 6e-8
*/
#define my_cosine(y,x)                                 \
{                                                      \
    ae_int32x2 xx,x2,t,yy;                             \
    xx=x;                                              \
    xx=AE_SLAI32S(xx,2);                               \
    x2=AE_MULFP32X2RAS(xx,xx);                         \
    yy=coef_cosine[0];                                 \
    t=coef_cosine[1]; AE_MULAFP32X2RAS(t,x2,yy); yy=t; \
    t=coef_cosine[2]; AE_MULAFP32X2RAS(t,x2,yy); yy=t; \
    t=coef_cosine[3]; AE_MULAFP32X2RAS(t,x2,yy); yy=t; \
    y=yy;                                              \
}

/* sin(pi*x): valid for -1/4...1/4
   Input:
   x - Q31
   Output:
   result in Q31
   Accuracy ~ 4e-9
*/
#define my_sine(y,x)                                \
{                                                   \
    ae_int32x2 t,xx,x2,yy;                          \
    xx=x;                                           \
    xx=AE_SLAI32S(xx,2);                            \
    x2=AE_MULFP32X2RAS(xx,xx);                      \
    yy=coef_sine[0];                                \
    t=coef_sine[1]; AE_MULAFP32X2RAS(t,x2,yy); yy=t;\
    t=coef_sine[2]; AE_MULAFP32X2RAS(t,x2,yy); yy=t;\
    t=coef_sine[3]; AE_MULAFP32X2RAS(t,x2,yy); yy=t;\
    y=AE_MULFP32X2RAS(yy,xx);                       \
}

/* tan(pi*x): valid for -1/8...1/8
   Input:
   x - Q31
   Output:
   result in Q31
   Accuracy ~ 1e-7
*/
#define my_tan(y,x)                                 \
{                                                   \
    ae_int32x2 t,xx,x2,yy;                          \
    xx=x;                                           \
    xx=AE_SLAI32S(xx,3);                            \
    x2=AE_MULFP32X2RAS(xx,xx);                      \
    yy=coef_tan[0];                                 \
    t=coef_tan[1]; AE_MULAFP32X2RAS(t,x2,yy); yy=t; \
    t=coef_tan[2]; AE_MULAFP32X2RAS(t,x2,yy); yy=t; \
    t=coef_tan[3]; AE_MULAFP32X2RAS(t,x2,yy); yy=t; \
    y=AE_MULFP32X2RAS(yy,xx);                       \
}


/*--------------------------------------------------------------------------
  function compute coefficients according to the input image size and rotation angle
  bpp - bytes per pixel
  Input:
  params    image 
  bpp       bytes per pixel
  Output:
  coef      coefficients
--------------------------------------------------------------------------*/
void imgrotate_getCoef(tImgRotateCoef* coef, const imgrotate_params_t* params, int bpp, int fast)
{
    ae_int32x2 fiQ31,csQ31,snQ31,tnQ31,t;
    ae_int32x2 dhQ16,dwQ16,ddhQ16,w3sQ16,h3sQ16,cw3sQ16,ch3sQ16, houtQ0,woutQ0;
    ae_int16x4 w,h;
    int align=bpp==1 ? ALIGNMENT: ALIGNMENT/2;  
    int q;
    int16_t a;
    imgsize_t sz;
    coef->tmpSz.height=coef->tmpSz.width=coef->tmpSz.stride=0;
    coef->alpha= coef->beta=0;
    coef->left=coef->up=0;
    imgrotate_params_validate(params,bpp,fast);
    a=params->angleQ15;
    q=((a+0x2000)>>14)&3;
    a=a-(q<<14);
    sz.height =(q&1) ? params->in.width : params->in.height;
    sz.width  =(q&1) ? params->in.height : params->in.width;
    sz.stride=fast ? (sz.width+align-1)&~(align-1) : sz.width;
    if (a==0) 
    {
        coef->outSz=sz;
        return;   // no shear operation required
    }

    h=AE_MOVDA16(sz.height );
    w=AE_MOVDA16(sz.width  );

    fiQ31=AE_CVT32X2F16_32(a);
    my_cosine(csQ31,fiQ31);
    my_sine  (snQ31,fiQ31);
    my_tan   (tnQ31,AE_SRAI32(fiQ31,1));

    t=AE_MULFP32X2RAS (AE_CVT32X2F16_32(AE_SUB16(w,1)),AE_ABS32S(snQ31));
    AE_MULAFP32X2RAS(t,AE_CVT32X2F16_32(AE_SUB16(h,1)),AE_ABS32S(csQ31));
    t=AE_SRAI32(t,2);
    t = AE_NEG32(AE_SRAI32(AE_NEG32(t),16));
    houtQ0 = AE_ADD32((t<<2),1);
    t=AE_MULFP32X2RAS (AE_CVT32X2F16_32(AE_SUB16(h,1)),AE_ABS32S(snQ31));
    AE_MULAFP32X2RAS(t,AE_CVT32X2F16_32(AE_SUB16(w,1)),AE_ABS32S(csQ31));
    t=AE_SRAI32(t,2);
    t = AE_NEG32(AE_SRAI32(AE_NEG32(t),16));
    woutQ0 = AE_ADD32(AE_SLAI32(t,2),1);

    dhQ16 = AE_MULFP32X2RAS(AE_ABS32S(tnQ31), AE_SUB32(AE_CVT32X2F16_32(w),(1<<15)));
    dwQ16 = AE_MULFP32X2RAS(AE_ABS32S(snQ31), AE_SUB32(AE_ADD32(AE_CVT32X2F16_32(h),dhQ16),(1<<15)));
    ddhQ16= AE_MULFP32X2RAS(AE_ABS32S(tnQ31), AE_SUB32(AE_ADD32(AE_CVT32X2F16_32(w),dwQ16),(1<<15)));
    h3sQ16 = AE_ADD32(AE_ADD32(AE_CVT32X2F16_32(h),dhQ16),ddhQ16);
    w3sQ16 = AE_ADD32(AE_CVT32X2F16_32(w),dwQ16);
    cw3sQ16 = AE_SRAI32(AE_SUB32(w3sQ16,(1<<16)),1);
    ch3sQ16 = AE_SRAI32(AE_SUB32(h3sQ16,(1<<16)),1);

    // round results
    coef->alpha=AE_ROUND16X4F32SASYM(AE_NEG32(tnQ31),AE_NEG32(tnQ31));
    coef->beta =AE_ROUND16X4F32SASYM( snQ31,snQ31);
    coef->outSz.width =(int16_t)AE_MOVAD32_H(woutQ0);
    coef->outSz.height=(int16_t)AE_MOVAD32_H(houtQ0);
    coef->outSz.stride=fast ? (coef->outSz.width+align-1)&~(align-1) : coef->outSz.width;
    t=AE_SUB32(cw3sQ16,AE_SLAI32(woutQ0,15));
    coef->left =AE_MOVAD16_0(AE_ROUND16X4F32SASYM(t,t))+ALIGNMENT/2-1;
    t=AE_SUB32(ch3sQ16,AE_SLAI32(houtQ0,15));
    coef->up   =AE_MOVAD16_0(AE_ROUND16X4F32SASYM(t,t))+PADDING-1;
    /* some additional pixels from each side + ALIGNMENT/2 pixels from the left/righ sides */
    coef->tmpSz.width =AE_ADD32(AE_MAX32(AE_SRAI32(w3sQ16,16),woutQ0),ALIGNMENT);   
    coef->tmpSz.height=AE_ADD32(AE_MAX32(AE_SRAI32(h3sQ16,16),houtQ0),2*PADDING);
    coef->tmpSz.stride=(coef->tmpSz.width+ALIGNMENT/2-1)&~(ALIGNMENT/2-1);
}
