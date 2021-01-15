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

static const complex_fract16 ALIGN(8) dct4_twd_64[]=
{
    {{32766,-402  }},{{32746,-1206 }},{{32706,-2009 }},{{32647,-2811 }},
    {{32568,-3612 }},{{32470,-4410 }},{{32352,-5205 }},{{32214,-5998 }},
    {{32058,-6787 }},{{31881,-7571 }},{{31686,-8351 }},{{31471,-9127 }},
    {{31238,-9896 }},{{30986,-10660}},{{30715,-11417}},{{30425,-12167}},
    {{22884,-23453}},{{22302,-24008}},{{21706,-24548}},{{21097,-25073}},
    {{20475,-25583}},{{19841,-26078}},{{19195,-26557}},{{18538,-27020}},
    {{17869,-27467}},{{17190,-27897}},{{16500,-28311}},{{15800,-28707}},
    {{15091,-29086}},{{14373,-29448}},{{13646,-29792}},{{12910,-30118}}
};
static const complex_fract16 dct3_64[]=
{
    {{32767,0    }},{{32729,1608 }},{{32610,3212 }},{{32413,4808 }},
    {{32138,6393 }},{{31786,7962 }},{{31357,9512 }},{{30853,11039}},
    {{30274,12540}},{{29622,14010}},{{28899,15447}},{{28106,16846}},
    {{27246,18205}},{{26320,19520}},{{25330,20788}},{{24279,22006}}
};

static const complex_fract16 rfft_32[]=
{
    {{32138,6393 }},
    {{30274,12540}},
    {{27246,18205}},
    {{23170,23170}},
    {{18205,27246}},
    {{12540,30274}},
    {{6393 ,32138}}
};

static const complex_fract16 fft_16[]=
{
    {{ 32767, 0    }},
    {{ 32767, 0    }},
    {{ 32767, 0    }},
    {{ 30274,-12540}},
    {{ 23170,-23170}},
    {{ 12540,-30274}},
    {{ 23170,-23170}},
    {{ 0    ,-32768}},
    {{-23170,-23170}},
    {{ 12540,-30274}},
    {{-23170,-23170}},
    {{-30274, 12540}}
};

static const tdct4_twd_fr16 descr  = {64,dct4_twd_64 ,dct3_64 ,rfft_32 ,fft_16 };
const dct_handle_t dct4_16_64=(dct_handle_t)&descr;
const dct_handle_t mdct_16_64=(dct_handle_t)&descr;
