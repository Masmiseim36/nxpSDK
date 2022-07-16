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
#include "NatureDSP_types.h"
#include "NatureDSP_Signal_img.h"
#include "common.h"
#include "img_common.h"
#include "divide_q31.h"

/*-------------------------------------------------------------------------
  Image normalization
  Function normalize the intensity of pixels to the given range

  Image formats:
  gu8    8-bit unsigned grayscale data
  gs8    8-bit signed grayscale data
  gs16   16-bit grayscale data

  Input:
  inImg   input image
  sz      image size
  minInt  min intensity on output (for linear normalization)
  maxInt  max intensity on output (for non-linear normalization)
  tbl[64] tabulated values (for non-linear normalization)
  Input:
  outImg   input image

  Restrictions:
  see general restrictions applied for all images for fast/generic 
  functions
-------------------------------------------------------------------------*/
void imgfastnorm_gs16( void * restrict outImg, const void * restrict inImg, const imgsize_t* sz, int minInt, int maxInt)
{
    ae_int32x2 rQ15;
    ae_int16x4 minIntv,maxIntv;
    ae_int16x4 pmaxv,pminv;
    const ae_int16x4 * restrict pIn;
          ae_int16x4 * restrict pOut;
    const int16_t * restrict in =(const int16_t *)inImg;
          int16_t * restrict out=(      int16_t *)outImg;
    int i,j;
    int h=(int)sz->height;
    int w=(int)sz->width;
    int istride=sz->stride;
    NASSERT(inImg!=NULL);
    NASSERT(outImg!=NULL);
    NASSERT_ALIGN(inImg,ALIGNMENT);
    NASSERT_ALIGN(outImg,ALIGNMENT);
    imgsize_validate(sz,2,1);
    if (h<=0 || w<=0) return;
    minIntv=XT_MAX(minInt,0);
    maxIntv=XT_MIN(maxInt,32767);
    // find min/max values
    {
        int k,K;
        xtbool4 bmask;
        ae_int16x4 p,p0,p1;
        pmaxv=0; pminv=32767;
        K=(w-1)>>2;
        k=w-(K<<2); //1...4
        bmask=AE_MOVAB4((0xf0>>k)&0xf); // 0x1000 for 1, 0x1100 for 2, 0x1110 for 3, 0x1111 for 4
        for (i=0; i<h; i++) 
        {
            pIn=(const ae_int16x4*)(in+i*istride);
            for (j=0; j<K;  j++)
            { 
                AE_L16X4_IP(p,pIn,4*sizeof(int16_t));
                AE_MOVT16X4(pmaxv,p,AE_LT16(pmaxv,p));
                AE_MOVT16X4(pminv,p,AE_LT16(p,pminv));
            }
            AE_L16X4_IP(p,pIn,4*sizeof(int16_t));
            p0=p;p1=p;
            AE_MOVF16X4(p0,    0,bmask);
            AE_MOVF16X4(p1,32767,bmask);
            AE_MOVT16X4(pmaxv,p0,AE_LT16(pmaxv,p0));
            AE_MOVT16X4(pminv,p1,AE_LT16(p1,pminv));
        }
        p=AE_SEL16_5432(pmaxv,pmaxv); AE_MOVT16X4(pmaxv,p,AE_LT16(pmaxv,p));
        p=AE_SEL16_5432(pminv,pminv); AE_MOVT16X4(pminv,p,AE_LT16(p,pminv));
        p=AE_SEL16_4321(pmaxv,pmaxv); AE_MOVT16X4(pmaxv,p,AE_LT16(pmaxv,p));
        p=AE_SEL16_4321(pminv,pminv); AE_MOVT16X4(pminv,p,AE_LT16(p,pminv));
    }
    {
        ae_int16x4 p;
        xtbool4 eq=AE_EQ16(pmaxv,pminv);
        // force output to minInt
        AE_MOVT16X4(maxIntv,minIntv,eq);
        AE_MOVT16X4(pmaxv,1,eq);
        AE_MOVT16X4(pminv,0,eq);
        p=AE_SUB16S(pmaxv,pminv);
        // get normalization coefficient in Q15
        rQ15=divide_q31(1,((int32_t) AE_MOVAD16_0(p))<<1);
    }
    /* normalization loop over all aligned columns */
    for (j=0; j<(w&~3); j+=4)
    {
        pIn =(const ae_int16x4*)&in[j];
        pOut=(      ae_int16x4*)&out[j];
        __Pragma("loop_count min=1")
        for (i=0; i<h; i++) 
        {
            ae_int16x4 p;
            ae_int16x4 x,cQ15;
            ae_int32x2 ch,cl;
            AE_L16X4_XP(p,pIn,sizeof(int16_t)*istride);
            p=AE_SUB16S(p,pminv);
            ch=AE_MULFP32X16X2RAS_H(rQ15,p);
            cl=AE_MULFP32X16X2RAS_L(rQ15,p);
            cQ15=AE_SAT16X4(ch,cl);
            cQ15=AE_MULFP16X4RAS(AE_SUB16S(maxIntv,minIntv),cQ15);
            x=AE_ADD16S(minIntv,cQ15);
            AE_MOVT16X4(x,0,AE_LT16(x,0));
            AE_S16X4_XP(x,pOut,sizeof(int16_t)*istride);
        }
    }
    /* remaining loop for (1...3)xh pixels */
    if (w&3)
    {
        int off1,off2;
        off1=(w&3)>1 ? 1*sizeof(int16_t):0;
        off2=(w&3)>2 ? 2*sizeof(int16_t):0;
        pIn =(const ae_int16x4*)&in[j];
        pOut=(      ae_int16x4*)&out[j];
        __Pragma("loop_count min=1")
        for (i=0; i<h; i++) 
        {
            ae_int16x4 p,x0,x1,x2;
            ae_int16x4 x,cQ15;
            ae_int32x2 ch,cl;
            AE_L16X4_XP(p,pIn,sizeof(int16_t)*istride);
            p=AE_SUB16S(p,pminv);
            ch=AE_MULFP32X16X2RAS_H(rQ15,p);
            cl=AE_MULFP32X16X2RAS_L(rQ15,p);
            cQ15=AE_SAT16X4(ch,cl);
            cQ15=AE_MULFP16X4RAS(AE_SUB16S(maxIntv,minIntv),cQ15);
            x=AE_ADD16S(minIntv,cQ15);
            AE_MOVT16X4(x,0,AE_LT16(x,0));
            x2=AE_SEL16_4321(x,x);
            x1=AE_SEL16_5432(x,x);
            x0=AE_SEL16_6543(x,x);
            AE_S16_0_X (x2,(ae_int16*)pOut,off2);
            AE_S16_0_X (x1,(ae_int16*)pOut,off1);
            AE_S16_0_XP(x0,castxcc(ae_int16,pOut),sizeof(int16_t)*istride);
        }
    }
}
