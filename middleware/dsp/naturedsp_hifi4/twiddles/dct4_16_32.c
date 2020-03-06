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
    NatureDSP Signal Processing Library. FFT part
    DCT-IV twiddles
    Integrit, 2006-2018
*/

#include "NatureDSP_Signal_fft.h"
#include "common.h"
#include "dct4_twd.h"

static const complex_fract16 ALIGN(8) dct4_twd_32[]=
{
    {{32758,-804  }},{{32679,-2411 }},{{32522,-4011 }},{{32286,-5602 }},
    {{31972,-7180 }},{{31581,-8740 }},{{31114,-10279}},{{30572,-11793}},
    {{22595,-23732}},{{21403,-24812}},{{20160,-25833}},{{18868,-26791}},
    {{17531,-27684}},{{16151,-28511}},{{14733,-29269}},{{13279,-29957}}
};

static const complex_fract16 dct3_32[]=
{
    {{32767,0    }},{{32610,3212 }},{{32138,6393 }},{{31357,9512 }},
    {{30274,12540}},{{28899,15447}},{{27246,18205}},{{25330,20788}}
};

static const complex_fract16 rfft_16[]=
{
    {{30274,12540}},{{23170,23170}},{{12540,30274}}
};

static const complex_fract16 fft_8[]=
{
    {{ 32767, 0    }},
    {{ 32767, 0    }},
    {{ 32767, 0    }},
    {{ 23170,-23170}},
    {{ 0    ,-32768}},
    {{-23170,-23170}}
};

static const tdct4_twd_fr16 descr  = {32,dct4_twd_32 ,dct3_32 ,rfft_16 ,fft_8  };
const dct_handle_t dct4_16_32=(dct_handle_t)&descr;
const dct_handle_t mdct_16_32=(dct_handle_t)&descr;
