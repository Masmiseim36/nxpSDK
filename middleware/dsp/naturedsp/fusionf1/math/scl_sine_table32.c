/* ------------------------------------------------------------------------ */
/* Copyright (c) 2016 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (“Cadence    */
/* Libraries”) are the copyrighted works of Cadence Design Systems Inc.	    */
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
/*          Copyright (C) 2015-2016 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
#include "scl_sine_table32.h"
#include "common.h"


/* -------------------------------------------------
	sine table: contains packed cos/sin 
	
	MATLAB CODE:
	phase=(0:512-1)/512*(2*pi);
	tbl=S_round_l(sin(phase)*32768.);
   	tbl = (max(min(tbl,32767),-32768));
	tbl=bitand((tbl+65536),65535);
	for k=1:16:length(tbl)
		fprintf(1,'0x%04x,0x%04x,0x%04x,0x%04x,0x%04x,0x%04x,0x%04x,0x%04x,0x%04x,0x%04x,0x%04x,0x%04x,0x%04x,0x%04x,0x%04x,0x%04x,\n',tbl(k:k+16));
	end


    ord=9;
    tbl=round(sin(2*pi*(0:2^ord-1)/(2^ord))*2^31);
    tbl=min(2^31-1,max(-2^31,tbl));
    for n=1:8:length(tbl)
        for k=0:7
            x=tbl(n+k);
            if (x<0) x=x+2^32; end
            fprintf(1,'0x%08x,',x);
        end
        fprintf(1,'\n');
    end

------------------------------------------------ */ 
#ifdef COMPILER_MSVC
const int32_t _declspec(align(8)) sine_table32[] =
#else
const int32_t                     sine_table32[] __attribute__((aligned(8))) =
#endif
{
(int32_t)0x00000000,(int32_t)0x01921d20,(int32_t)0x03242abf,(int32_t)0x04b6195d,(int32_t)0x0647d97c,(int32_t)0x07d95b9e,(int32_t)0x096a9049,(int32_t)0x0afb6805,
(int32_t)0x0c8bd35e,(int32_t)0x0e1bc2e4,(int32_t)0x0fab272b,(int32_t)0x1139f0cf,(int32_t)0x12c8106f,(int32_t)0x145576b1,(int32_t)0x15e21445,(int32_t)0x176dd9de,
(int32_t)0x18f8b83c,(int32_t)0x1a82a026,(int32_t)0x1c0b826a,(int32_t)0x1d934fe5,(int32_t)0x1f19f97b,(int32_t)0x209f701c,(int32_t)0x2223a4c5,(int32_t)0x23a6887f,
(int32_t)0x25280c5e,(int32_t)0x26a82186,(int32_t)0x2826b928,(int32_t)0x29a3c485,(int32_t)0x2b1f34eb,(int32_t)0x2c98fbba,(int32_t)0x2e110a62,(int32_t)0x2f875262,
(int32_t)0x30fbc54d,(int32_t)0x326e54c7,(int32_t)0x33def287,(int32_t)0x354d9057,(int32_t)0x36ba2015,(int32_t)0x382493b0,(int32_t)0x398cdd32,(int32_t)0x3af2eeb7,
(int32_t)0x3c56ba70,(int32_t)0x3db832a6,(int32_t)0x3f1749b8,(int32_t)0x4073f21d,(int32_t)0x41ce1e65,(int32_t)0x4325c135,(int32_t)0x447acd50,(int32_t)0x45cd358f,
(int32_t)0x471cece7,(int32_t)0x4869e665,(int32_t)0x49b41533,(int32_t)0x4afb6c98,(int32_t)0x4c3fdff4,(int32_t)0x4d8162c4,(int32_t)0x4ebfe8a5,(int32_t)0x4ffb654d,
(int32_t)0x5133cc94,(int32_t)0x5269126e,(int32_t)0x539b2af0,(int32_t)0x54ca0a4b,(int32_t)0x55f5a4d2,(int32_t)0x571deefa,(int32_t)0x5842dd54,(int32_t)0x59646498,
(int32_t)0x5a82799a,(int32_t)0x5b9d1154,(int32_t)0x5cb420e0,(int32_t)0x5dc79d7c,(int32_t)0x5ed77c8a,(int32_t)0x5fe3b38d,(int32_t)0x60ec3830,(int32_t)0x61f1003f,
(int32_t)0x62f201ac,(int32_t)0x63ef3290,(int32_t)0x64e88926,(int32_t)0x65ddfbd3,(int32_t)0x66cf8120,(int32_t)0x67bd0fbd,(int32_t)0x68a69e81,(int32_t)0x698c246c,
(int32_t)0x6a6d98a4,(int32_t)0x6b4af279,(int32_t)0x6c242960,(int32_t)0x6cf934fc,(int32_t)0x6dca0d14,(int32_t)0x6e96a99d,(int32_t)0x6f5f02b2,(int32_t)0x7023109a,
(int32_t)0x70e2cbc6,(int32_t)0x719e2cd2,(int32_t)0x72552c85,(int32_t)0x7307c3d0,(int32_t)0x73b5ebd1,(int32_t)0x745f9dd1,(int32_t)0x7504d345,(int32_t)0x75a585cf,
(int32_t)0x7641af3d,(int32_t)0x76d94989,(int32_t)0x776c4edb,(int32_t)0x77fab989,(int32_t)0x78848414,(int32_t)0x7909a92d,(int32_t)0x798a23b1,(int32_t)0x7a05eead,
(int32_t)0x7a7d055b,(int32_t)0x7aef6323,(int32_t)0x7b5d039e,(int32_t)0x7bc5e290,(int32_t)0x7c29fbee,(int32_t)0x7c894bde,(int32_t)0x7ce3ceb2,(int32_t)0x7d3980ec,
(int32_t)0x7d8a5f40,(int32_t)0x7dd6668f,(int32_t)0x7e1d93ea,(int32_t)0x7e5fe493,(int32_t)0x7e9d55fc,(int32_t)0x7ed5e5c6,(int32_t)0x7f0991c4,(int32_t)0x7f3857f6,
(int32_t)0x7f62368f,(int32_t)0x7f872bf3,(int32_t)0x7fa736b4,(int32_t)0x7fc25596,(int32_t)0x7fd8878e,(int32_t)0x7fe9cbc0,(int32_t)0x7ff62182,(int32_t)0x7ffd885a,
(int32_t)0x7fffffff,(int32_t)0x7ffd885a,(int32_t)0x7ff62182,(int32_t)0x7fe9cbc0,(int32_t)0x7fd8878e,(int32_t)0x7fc25596,(int32_t)0x7fa736b4,(int32_t)0x7f872bf3,
(int32_t)0x7f62368f,(int32_t)0x7f3857f6,(int32_t)0x7f0991c4,(int32_t)0x7ed5e5c6,(int32_t)0x7e9d55fc,(int32_t)0x7e5fe493,(int32_t)0x7e1d93ea,(int32_t)0x7dd6668f,
(int32_t)0x7d8a5f40,(int32_t)0x7d3980ec,(int32_t)0x7ce3ceb2,(int32_t)0x7c894bde,(int32_t)0x7c29fbee,(int32_t)0x7bc5e290,(int32_t)0x7b5d039e,(int32_t)0x7aef6323,
(int32_t)0x7a7d055b,(int32_t)0x7a05eead,(int32_t)0x798a23b1,(int32_t)0x7909a92d,(int32_t)0x78848414,(int32_t)0x77fab989,(int32_t)0x776c4edb,(int32_t)0x76d94989,
(int32_t)0x7641af3d,(int32_t)0x75a585cf,(int32_t)0x7504d345,(int32_t)0x745f9dd1,(int32_t)0x73b5ebd1,(int32_t)0x7307c3d0,(int32_t)0x72552c85,(int32_t)0x719e2cd2,
(int32_t)0x70e2cbc6,(int32_t)0x7023109a,(int32_t)0x6f5f02b2,(int32_t)0x6e96a99d,(int32_t)0x6dca0d14,(int32_t)0x6cf934fc,(int32_t)0x6c242960,(int32_t)0x6b4af279,
(int32_t)0x6a6d98a4,(int32_t)0x698c246c,(int32_t)0x68a69e81,(int32_t)0x67bd0fbd,(int32_t)0x66cf8120,(int32_t)0x65ddfbd3,(int32_t)0x64e88926,(int32_t)0x63ef3290,
(int32_t)0x62f201ac,(int32_t)0x61f1003f,(int32_t)0x60ec3830,(int32_t)0x5fe3b38d,(int32_t)0x5ed77c8a,(int32_t)0x5dc79d7c,(int32_t)0x5cb420e0,(int32_t)0x5b9d1154,
(int32_t)0x5a82799a,(int32_t)0x59646498,(int32_t)0x5842dd54,(int32_t)0x571deefa,(int32_t)0x55f5a4d2,(int32_t)0x54ca0a4b,(int32_t)0x539b2af0,(int32_t)0x5269126e,
(int32_t)0x5133cc94,(int32_t)0x4ffb654d,(int32_t)0x4ebfe8a5,(int32_t)0x4d8162c4,(int32_t)0x4c3fdff4,(int32_t)0x4afb6c98,(int32_t)0x49b41533,(int32_t)0x4869e665,
(int32_t)0x471cece7,(int32_t)0x45cd358f,(int32_t)0x447acd50,(int32_t)0x4325c135,(int32_t)0x41ce1e65,(int32_t)0x4073f21d,(int32_t)0x3f1749b8,(int32_t)0x3db832a6,
(int32_t)0x3c56ba70,(int32_t)0x3af2eeb7,(int32_t)0x398cdd32,(int32_t)0x382493b0,(int32_t)0x36ba2015,(int32_t)0x354d9057,(int32_t)0x33def287,(int32_t)0x326e54c7,
(int32_t)0x30fbc54d,(int32_t)0x2f875262,(int32_t)0x2e110a62,(int32_t)0x2c98fbba,(int32_t)0x2b1f34eb,(int32_t)0x29a3c485,(int32_t)0x2826b928,(int32_t)0x26a82186,
(int32_t)0x25280c5e,(int32_t)0x23a6887f,(int32_t)0x2223a4c5,(int32_t)0x209f701c,(int32_t)0x1f19f97b,(int32_t)0x1d934fe5,(int32_t)0x1c0b826a,(int32_t)0x1a82a026,
(int32_t)0x18f8b83c,(int32_t)0x176dd9de,(int32_t)0x15e21445,(int32_t)0x145576b1,(int32_t)0x12c8106f,(int32_t)0x1139f0cf,(int32_t)0x0fab272b,(int32_t)0x0e1bc2e4,
(int32_t)0x0c8bd35e,(int32_t)0x0afb6805,(int32_t)0x096a9049,(int32_t)0x07d95b9e,(int32_t)0x0647d97c,(int32_t)0x04b6195d,(int32_t)0x03242abf,(int32_t)0x01921d20,
(int32_t)0x00000000,(int32_t)0xfe6de2e0,(int32_t)0xfcdbd541,(int32_t)0xfb49e6a3,(int32_t)0xf9b82684,(int32_t)0xf826a462,(int32_t)0xf6956fb7,(int32_t)0xf50497fb,
(int32_t)0xf3742ca2,(int32_t)0xf1e43d1c,(int32_t)0xf054d8d5,(int32_t)0xeec60f31,(int32_t)0xed37ef91,(int32_t)0xebaa894f,(int32_t)0xea1debbb,(int32_t)0xe8922622,
(int32_t)0xe70747c4,(int32_t)0xe57d5fda,(int32_t)0xe3f47d96,(int32_t)0xe26cb01b,(int32_t)0xe0e60685,(int32_t)0xdf608fe4,(int32_t)0xdddc5b3b,(int32_t)0xdc597781,
(int32_t)0xdad7f3a2,(int32_t)0xd957de7a,(int32_t)0xd7d946d8,(int32_t)0xd65c3b7b,(int32_t)0xd4e0cb15,(int32_t)0xd3670446,(int32_t)0xd1eef59e,(int32_t)0xd078ad9e,
(int32_t)0xcf043ab3,(int32_t)0xcd91ab39,(int32_t)0xcc210d79,(int32_t)0xcab26fa9,(int32_t)0xc945dfec,(int32_t)0xc7db6c50,(int32_t)0xc67322ce,(int32_t)0xc50d1149,
(int32_t)0xc3a94590,(int32_t)0xc247cd5a,(int32_t)0xc0e8b648,(int32_t)0xbf8c0de3,(int32_t)0xbe31e19b,(int32_t)0xbcda3ecb,(int32_t)0xbb8532b0,(int32_t)0xba32ca71,
(int32_t)0xb8e31319,(int32_t)0xb796199b,(int32_t)0xb64beacd,(int32_t)0xb5049368,(int32_t)0xb3c0200c,(int32_t)0xb27e9d3c,(int32_t)0xb140175b,(int32_t)0xb0049ab3,
(int32_t)0xaecc336c,(int32_t)0xad96ed92,(int32_t)0xac64d510,(int32_t)0xab35f5b5,(int32_t)0xaa0a5b2e,(int32_t)0xa8e21106,(int32_t)0xa7bd22ac,(int32_t)0xa69b9b68,
(int32_t)0xa57d8666,(int32_t)0xa462eeac,(int32_t)0xa34bdf20,(int32_t)0xa2386284,(int32_t)0xa1288376,(int32_t)0xa01c4c73,(int32_t)0x9f13c7d0,(int32_t)0x9e0effc1,
(int32_t)0x9d0dfe54,(int32_t)0x9c10cd70,(int32_t)0x9b1776da,(int32_t)0x9a22042d,(int32_t)0x99307ee0,(int32_t)0x9842f043,(int32_t)0x9759617f,(int32_t)0x9673db94,
(int32_t)0x9592675c,(int32_t)0x94b50d87,(int32_t)0x93dbd6a0,(int32_t)0x9306cb04,(int32_t)0x9235f2ec,(int32_t)0x91695663,(int32_t)0x90a0fd4e,(int32_t)0x8fdcef66,
(int32_t)0x8f1d343a,(int32_t)0x8e61d32e,(int32_t)0x8daad37b,(int32_t)0x8cf83c30,(int32_t)0x8c4a142f,(int32_t)0x8ba0622f,(int32_t)0x8afb2cbb,(int32_t)0x8a5a7a31,
(int32_t)0x89be50c3,(int32_t)0x8926b677,(int32_t)0x8893b125,(int32_t)0x88054677,(int32_t)0x877b7bec,(int32_t)0x86f656d3,(int32_t)0x8675dc4f,(int32_t)0x85fa1153,
(int32_t)0x8582faa5,(int32_t)0x85109cdd,(int32_t)0x84a2fc62,(int32_t)0x843a1d70,(int32_t)0x83d60412,(int32_t)0x8376b422,(int32_t)0x831c314e,(int32_t)0x82c67f14,
(int32_t)0x8275a0c0,(int32_t)0x82299971,(int32_t)0x81e26c16,(int32_t)0x81a01b6d,(int32_t)0x8162aa04,(int32_t)0x812a1a3a,(int32_t)0x80f66e3c,(int32_t)0x80c7a80a,
(int32_t)0x809dc971,(int32_t)0x8078d40d,(int32_t)0x8058c94c,(int32_t)0x803daa6a,(int32_t)0x80277872,(int32_t)0x80163440,(int32_t)0x8009de7e,(int32_t)0x800277a6,
(int32_t)0x80000000,(int32_t)0x800277a6,(int32_t)0x8009de7e,(int32_t)0x80163440,(int32_t)0x80277872,(int32_t)0x803daa6a,(int32_t)0x8058c94c,(int32_t)0x8078d40d,
(int32_t)0x809dc971,(int32_t)0x80c7a80a,(int32_t)0x80f66e3c,(int32_t)0x812a1a3a,(int32_t)0x8162aa04,(int32_t)0x81a01b6d,(int32_t)0x81e26c16,(int32_t)0x82299971,
(int32_t)0x8275a0c0,(int32_t)0x82c67f14,(int32_t)0x831c314e,(int32_t)0x8376b422,(int32_t)0x83d60412,(int32_t)0x843a1d70,(int32_t)0x84a2fc62,(int32_t)0x85109cdd,
(int32_t)0x8582faa5,(int32_t)0x85fa1153,(int32_t)0x8675dc4f,(int32_t)0x86f656d3,(int32_t)0x877b7bec,(int32_t)0x88054677,(int32_t)0x8893b125,(int32_t)0x8926b677,
(int32_t)0x89be50c3,(int32_t)0x8a5a7a31,(int32_t)0x8afb2cbb,(int32_t)0x8ba0622f,(int32_t)0x8c4a142f,(int32_t)0x8cf83c30,(int32_t)0x8daad37b,(int32_t)0x8e61d32e,
(int32_t)0x8f1d343a,(int32_t)0x8fdcef66,(int32_t)0x90a0fd4e,(int32_t)0x91695663,(int32_t)0x9235f2ec,(int32_t)0x9306cb04,(int32_t)0x93dbd6a0,(int32_t)0x94b50d87,
(int32_t)0x9592675c,(int32_t)0x9673db94,(int32_t)0x9759617f,(int32_t)0x9842f043,(int32_t)0x99307ee0,(int32_t)0x9a22042d,(int32_t)0x9b1776da,(int32_t)0x9c10cd70,
(int32_t)0x9d0dfe54,(int32_t)0x9e0effc1,(int32_t)0x9f13c7d0,(int32_t)0xa01c4c73,(int32_t)0xa1288376,(int32_t)0xa2386284,(int32_t)0xa34bdf20,(int32_t)0xa462eeac,
(int32_t)0xa57d8666,(int32_t)0xa69b9b68,(int32_t)0xa7bd22ac,(int32_t)0xa8e21106,(int32_t)0xaa0a5b2e,(int32_t)0xab35f5b5,(int32_t)0xac64d510,(int32_t)0xad96ed92,
(int32_t)0xaecc336c,(int32_t)0xb0049ab3,(int32_t)0xb140175b,(int32_t)0xb27e9d3c,(int32_t)0xb3c0200c,(int32_t)0xb5049368,(int32_t)0xb64beacd,(int32_t)0xb796199b,
(int32_t)0xb8e31319,(int32_t)0xba32ca71,(int32_t)0xbb8532b0,(int32_t)0xbcda3ecb,(int32_t)0xbe31e19b,(int32_t)0xbf8c0de3,(int32_t)0xc0e8b648,(int32_t)0xc247cd5a,
(int32_t)0xc3a94590,(int32_t)0xc50d1149,(int32_t)0xc67322ce,(int32_t)0xc7db6c50,(int32_t)0xc945dfec,(int32_t)0xcab26fa9,(int32_t)0xcc210d79,(int32_t)0xcd91ab39,
(int32_t)0xcf043ab3,(int32_t)0xd078ad9e,(int32_t)0xd1eef59e,(int32_t)0xd3670446,(int32_t)0xd4e0cb15,(int32_t)0xd65c3b7b,(int32_t)0xd7d946d8,(int32_t)0xd957de7a,
(int32_t)0xdad7f3a2,(int32_t)0xdc597781,(int32_t)0xdddc5b3b,(int32_t)0xdf608fe4,(int32_t)0xe0e60685,(int32_t)0xe26cb01b,(int32_t)0xe3f47d96,(int32_t)0xe57d5fda,
(int32_t)0xe70747c4,(int32_t)0xe8922622,(int32_t)0xea1debbb,(int32_t)0xebaa894f,(int32_t)0xed37ef91,(int32_t)0xeec60f31,(int32_t)0xf054d8d5,(int32_t)0xf1e43d1c,
(int32_t)0xf3742ca2,(int32_t)0xf50497fb,(int32_t)0xf6956fb7,(int32_t)0xf826a462,(int32_t)0xf9b82684,(int32_t)0xfb49e6a3,(int32_t)0xfcdbd541,(int32_t)0xfe6de2e0,
(int32_t)0x00000000,(int32_t)0x01921d20		
};

#if SIN24X24_ALG==1
/*
    sin(pi/2*x) polynomial coefficients
    generated by Matlab:
    x=(-1:pow2(1,-15):1);
    y=sin(pi/2*x)-x;
    p=polyfit(x,y,7);
    p=int32(round(pow2(p(1:2:end),31)));
*/
const int32_t sine_coef24[]={-9347575,170649474,-1387072597,1225767301};
#endif
