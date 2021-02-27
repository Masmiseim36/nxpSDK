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
/*
    NatureDSP Signal Processing Library. FFT part
    C code optimized for Fusion
    DCT 32x16 with scaling option 3
*/

#include "NatureDSP_Signal.h"
#include "baseop.h"
#include "common.h"

/*
    Reference Matlab code for Radix4 DCT32

    y(1:N/2)=x(1:2:N);
    y(N:-1:N/2+1)=x(2:2:N);
    y=y(1:2:N)+i*y(2:2:N);
    Y=fft(y);
    cosi=exp(2*i*pi*(0:N/4-1)/(N));
    w=exp(-i*pi/2*(0:N-1)/N);
    % DCT split algorithm
    Y0=Y(1);
    T0=real(Y0)+imag(Y0);
    T1=real(Y0)-imag(Y0);
    z(1      )= real(T0)*sqrt(2)/2;
    z(N/2+1  )= real(T1)*sqrt(2)/2;
    for k=2:N/4
        Y0=Y(k);
        Y1=Y(N/2+2-k);
        COSI=cosi(k);
        W1=w(k);
        W2=w(N/2+2-k);
        S=Y0+Y1;
        D=Y0-Y1;
        T0=i*real(D)+imag(S);
        T1=i*imag(D)+real(S);
        T1=T1/2;
        Y0=  (imag(T0)*imag(COSI)-real(T0)*real(COSI)) + ...
           i*(real(T0)*imag(COSI)+imag(T0)*real(COSI));
        Y0=Y0/2;
        T0=T1-Y0;
        T1=T1+Y0;
        z(k      )= real(T0)*real(W1)-imag(T0)*imag(W1);
        z(N+2-k  )=-real(T0)*imag(W1)-imag(T0)*real(W1);
        z(N/2+2-k)= real(T1)*real(W2)+imag(T1)*imag(W2);
        z(N/2+k  )=-real(T1)*imag(W2)+imag(T1)*real(W2);
    end
    W1=w(N/4+1);
    T0=Y(N/4+1);
    z(N/4+1  )= real(T0)*real(W1)+imag(T0)*imag(W1);
    z(N+1-N/4)=-real(T0)*imag(W1)+imag(T0)*real(W1);
    z=z*sqrt(2/N);

*/



/*
    MATLAB code:
    N = 32; % transform size
    P = 32; % precision bits
    L=2;    % number of digits per row
    f = 1;  % output file id
    if(P==16)
        format = '(int16_t)0x%04X,   ';
    else
        format = '(int32_t)0x%08X,   ';
    end

    cosi=exp(1j*2*pi/N*(0:N/4-1));

    w=exp(1j*pi/2*(0:N/2-1)/N);
    w = [conj(w(1:N/4)),  conj(w(end:-1:N/4+1))];

    tmpw = zeros(size(w));
    tmpw(1) = real(w(1)) + 1j*1/sqrt(2);
    tmpw(2:2:N/2-1) = (w(2:N/4));
    tmpw(3:2:N/2-1) = conj(w(N/4+1:end-1)); %(w(end:-1:N/4+2));
    tmpw(end) = conj(w(end));
    w = tmpw;

    tmp = reshape( [real(w); imag(w)], N, 1).';
    tmp = round(tmp*2^(P-1));

    fprintf(f, '\nALIGN(8) static const int32_t _w32[%d ]= {\n', N);

    for i=1:L: length(tmp)
        fprintf(f, '\n    ');
        if(P==32)
            t = typecast(int32(tmp(i:i+L-1)), 'uint32');
        else
            t = typecast(int16(tmp(i:i+L-1)), 'uint16');
        end
        fprintf(f, format, t);
    end
    fprintf(f, '};\n    ');
    % imag part is first !!!
    tmp = reshape( [imag(cosi); real(cosi)], N/2, 1).';
    tmp = round(tmp*2^(P-1));


    fprintf(f, '\nALIGN(8) static const int32_t _cosi32[%d ]= { \n', N/2);

    for i=1:L: length(tmp)
        fprintf(f, '\n    ');
        if(P==32)
            t = typecast(int32(tmp(i:i+L-1)), 'uint32');
        else
            t = typecast(int16(tmp(i:i+L-1)), 'uint16');
        end
        fprintf(f, format, t);
    end
    fprintf(f, '};\n    ');
*/

ALIGN(8) static const int32_t _w32[32] = {
    (int32_t)0x7FFFFFFF, (int32_t)0x5A82799A,
    (int32_t)0x7FD8878E, (int32_t)0xF9B82684,
    (int32_t)0x5ED77C8A, (int32_t)0x55F5A4D2,
    (int32_t)0x7F62368F, (int32_t)0xF3742CA2,
    (int32_t)0x62F201AC, (int32_t)0x5133CC94,
    (int32_t)0x7E9D55FC, (int32_t)0xED37EF91,
    (int32_t)0x66CF8120, (int32_t)0x4C3FDFF4,
    (int32_t)0x7D8A5F40, (int32_t)0xE70747C4,
    (int32_t)0x6A6D98A4, (int32_t)0x471CECE7,
    (int32_t)0x7C29FBEE, (int32_t)0xE0E60685,
    (int32_t)0x6DCA0D14, (int32_t)0x41CE1E65,
    (int32_t)0x7A7D055B, (int32_t)0xDAD7F3A2,
    (int32_t)0x70E2CBC6, (int32_t)0x3C56BA70,
    (int32_t)0x78848414, (int32_t)0xD4E0CB15,
    (int32_t)0x73B5EBD1, (int32_t)0x36BA2014,
    (int32_t)0x7641AF3D, (int32_t)0x30FBC54D, 
};

ALIGN(8) static const int32_t _cosi32[16] = {
    (int32_t)0x00000000, (int32_t)0x7FFFFFFF,
    (int32_t)0x18F8B83C, (int32_t)0x7D8A5F40,
    (int32_t)0x30FBC54D, (int32_t)0x7641AF3D,
    (int32_t)0x471CECE7, (int32_t)0x6A6D98A4,
    (int32_t)0x5A82799A, (int32_t)0x5A82799A,
    (int32_t)0x6A6D98A4, (int32_t)0x471CECE7,
    (int32_t)0x7641AF3D, (int32_t)0x30FBC54D,
    (int32_t)0x7D8A5F40, (int32_t)0x18F8B83C
};
inline_ void _cmult32x32(ae_int32x2 *result, ae_int32x2 *x, ae_int32x2 *y)
{


        ae_f64 re, im;
        ae_f32x2 r;

        re = AE_MULF32S_HH(*x, *y);
        im = AE_MULF32S_LH(*x, *y);
        AE_MULSF32S_LL(re, *x, *y);
        AE_MULAF32S_LH(im, *y, *x);

        r = AE_ROUND32X2F64SASYM(re, im);
        *result = r;

}
/*
	in-place split part of DCT:
	y[2*N]	input
	z[2*N]	output
	N		size of FFT
*/
//#define TESTVA 1

static void dct_split_32_32x16(int32_t * z, const int32_t * y)
{
  const ae_int32x2  * restrict p_y0,  * restrict p_y1;
//  const ae_int16x4  * restrict p_cos/*, * restrict p_w*/;
//  ae_int32x2        * restrict p_z0,  * restrict p_z1;

  const ae_int32x2        * restrict pw32       = (const ae_int32x2*)_w32;
  const ae_int32x2        * restrict pcosi32    = (const ae_int32x2*)_cosi32;
#if 1
  /* 42 cycles per pipeline stage in steady state with unroll = 1 */
  int32_t * restrict pa  = z + 31; 
  int32_t * restrict pa0 = z + 2; 
  int32_t * restrict pb1 = z + 0x12;
  int32_t * restrict pb0 = z + 0xf;
#else
  ae_int32  * restrict pa = (ae_int32  *)(z + 31);
  ae_int32  * restrict pa0 = (ae_int32  *)(z + 2);
  ae_int32  * restrict pb1 = (ae_int32  *)(z + 0x12);
  ae_int32  * restrict pb0 = (ae_int32  *)(z + 0xf);

#endif
//  ae_f32x2    vF0, vF1;
 // ae_f16x4    vC0, vW0, vW1;
//  ae_int16x4  vST;
  ae_int32x2  vA0, vA1, vB0, vB1, vR0;
  ae_int32x2 w, c; 

  const int step_back = -(int)sizeof(ae_int32x2);

  int i;

  p_y0  = (const ae_int32x2 *)(y);
  p_y1  = (const ae_int32x2 *)(y+30);
//  p_cos = (const ae_int16x4 *)ae_cosi;
 // p_w   = (const ae_int16x4 *)ae_w;
//  p_z0  = (      ae_int32x2 *)(z);
//  p_z1  = (      ae_int32x2 *)(z+30);


  /* load data and prepare pointers for pre-increment
     first and last samples */
  AE_L32X2_IP(vA0, p_y0, 8);
//  AE_L16X4_IP(vST, p_w, 8);
//  vW0 = (vST);
  vA1 = AE_SEL32_LH(vA0, vA0);
  vB0 = AE_ADDSUB32S(vA1, vA0);
  //vR0 = AE_MULFP32X16X2RAS_H(vB0, vST); 

  //////////////
  AE_L32X2_IP(w, pw32, sizeof(ae_int32x2)); 
  vR0 = AE_MULFP32X2RAS(vB0, w);
  
  AE_L32X2_IP(vA0, p_y0, 8);
  AE_L32X2_XP(vA1, p_y1, step_back);
//  AE_L16X4_IP(vST, p_cos, 8);
//  vC0 = (vST);
//  AE_L16X4_IP(vST, p_w, 8);
//  vW1 = (vST);

  vB0 = AE_SUBADD32S(vA0, vA1);
  vB1 = AE_ADDSUB32S(vA0, vA1);
//  vA0 = AE_MULFC32X16RAS_H(vB0, vC0);
  /////////////////////
  AE_L32X2_IP(c, pcosi32, 8); // adjust pointer
  AE_L32X2_IP(c, pcosi32, 8);
 // c = AE_INTSWAP(c); 
  _cmult32x32(&vA0, &vB0, &c); 

  vA0 = AE_SRAI32(vA0, 1);
  vA1 = AE_SRAI32(vB1, 1);

  vB0 = AE_SUB32S(vA1, vA0);
  vB1 = AE_ADD32S(vA1, vA0);

  /*
    z(k      )= real(T0)*real(W1)-imag(T0)*imag(W1);
    z(N+2-k  )=-real(T0)*imag(W1)-imag(T0)*real(W1);
    z(N/2+2-k)= real(T1)*real(W2)+imag(T1)*imag(W2);
    z(N/2+k  )=-real(T1)*imag(W2)+imag(T1)*real(W2);
  */
  //vA0 = AE_MULFC32X16RAS_L(vB0, vW0);
  //vA1 = AE_NEG32S(vA0);
  //vB1 = AE_MULFC32X16RAS_H(vB1, vW1);
  ///////////////
  AE_L32X2_IP(w, pw32, sizeof(ae_int32x2));
 // w = AE_INTSWAP(w); 
  _cmult32x32(&vA0, &vB0, &w);
  vA1 = AE_NEG32S(vA0);

  AE_L32X2_IP(w, pw32, sizeof(ae_int32x2));
  //w = AE_INTSWAP(w);
  _cmult32x32(&vB1, &vB1, &w); 

 
  vA0 = AE_SEL32_HH(vR0, vA0);
  vB0 = AE_SEL32_LL(vR0, vB1);
  vR0 = AE_SEL32_HL(vB1, vA1);

  AE_S32X2_I(vA0, (ae_int32x2*)z, 0);
  AE_S32X2_I(vB0, (ae_int32x2*)(z+16), 0);;
#ifdef TESTVA
  vR0._[0] = 0xF0; 
  vR0._[1] = 0xF1;
#endif
//  pa0[-2] = AE_MOVAD32_H(vA0); 
//  pa0[1]  = AE_MOVAD32_L(vA0);
#if 1
  *pb0-- = AE_MOVAD32_H(vR0); 
  *pa--  = AE_MOVAD32_L(vR0);
#else
  AE_S32_L_IP(vR0, pa, -4);
  AE_S32_L_IP(AE_INTSWAP(vR0), pb0, -4);
#endif
  /* 42 cycles per pipeline stage in steady state with unroll = 1 */
  for (i = 1; i < (32>>3); i++)
  {
      ae_int32x2 vA0_, vA1_,  vB1_; 
   //   ae_int32x2 w0, w1, w2, w3;
    /*
      Y0=Y(k);
      Y1=Y(N/2+2-k);
      COSI=cosi(k);
      W1=w(k);
      W2=w(N/2+2-k);
      S=Y0+Y1;
      D=Y0-Y1;
      T0=i*real(D)+imag(S);
      T1=i*imag(D)+real(S);
      T1=T1/2;
      Y0=  (imag(T0)*imag(COSI)-real(T0)*real(COSI)) + ...
            i*(real(T0)*imag(COSI)+imag(T0)*real(COSI));
      Y0=Y0/2;
      T0=T1-Y0;
      T1=T1+Y0;
    */
    AE_L32X2_IP(vA0, p_y0, 8);
    AE_L32X2_XP(vA1, p_y1, step_back);
//    AE_L16X4_IP(vST, p_w, 8);
//    vW0 = (vST);

    vB0 = AE_SUBADD32S(vA0, vA1);
    vB1 = AE_ADDSUB32S(vA0, vA1);
  //  vA0 = AE_MULFC32X16RAS_L(vB0, vC0);
    ////////////////
    AE_L32X2_IP(c, pcosi32, 8);
//    c = AE_INTSWAP(c);
    _cmult32x32(&vA0, &vB0, &c);

    vA0 = AE_SRAI32(vA0, 1);
    vA1 = AE_SRAI32(vB1, 1);

    vB0 = AE_SUB32S(vA1, vA0);
    vB1 = AE_ADD32S(vA1, vA0);
 
    /*
      z(k      )= real(T0)*real(W1)-imag(T0)*imag(W1);
      z(N+2-k  )=-real(T0)*imag(W1)-imag(T0)*real(W1);
      z(N/2+2-k)= real(T1)*real(W2)+imag(T1)*imag(W2);
      z(N/2+k  )=-real(T1)*imag(W2)+imag(T1)*real(W2);
    */
   //  vA0 = AE_MULFC32X16RAS_L(vB0, vW1);
    // vA1 = AE_NEG32S(vA0);
    // vB1 = AE_MULFC32X16RAS_H(vB1, vW0);
    //////////////////////////////////////////////////////
    AE_L32X2_IP(w, pw32, sizeof(ae_int32x2));
    _cmult32x32(&vA0, &vB0, &w);
    vA1 = AE_NEG32S(vA0);

   // printf("\n L(vW1): ref=%X, new = %X %X", AE_MOVAD32_L(AE_MOVINT32X2_FROMF16X4(vW1)), w._[0], w._[1]); 
    AE_L32X2_IP(w, pw32, sizeof(ae_int32x2));
    _cmult32x32(&vB1, &vB1, &w);
   // printf("\n H(vW0): ref=%X, new = %X %X", AE_MOVAD32_H(AE_MOVINT32X2_FROMF16X4(vW0)), w._[0], w._[1]);



   // _cmult32x32(&vB1, &vB1, &w);

   // AE_L32X2_IP(w, pw32, sizeof(ae_int32x2));
   // _cmult32x32(&vA0, &vB0, &w);
   // vA1 = AE_NEG32S(vA0);
#ifdef TESTVA
    vA0._[0] = 0xA00000 + i;
    vA0._[1] = 0xA01000 + i;

    vA1._[0] = 0xA100 + i;
    vA1._[1] = 0xA110 + i;
    vB1._[0] = 0xB100 + i;
    vB1._[1] = 0xB110 + i;
#endif
    vA0_ = vA0;
    vA1_ = vA1; 
    vB1_ = vB1; 

    //vA1 = AE_SEL32_LL(vA1, vR0);
    //vB0 = AE_SEL32_HH(vB1, vR0);
    //vR0 = AE_SEL32_HL(vA0, vB1);

//    AE_S32X2_I(vB0, p_z1, -8*(int)sizeof(*p_z0));
//    AE_S32X2_XP(vA1, p_z1, step_back);
#if 1
    *pa0++ = AE_MOVAD32_H(vA0_);
    *pa--  = AE_MOVAD32_L(vA1_); 
    *pb1++ = AE_MOVAD32_L(vB1_);
    *pb0-- = AE_MOVAD32_H(vB1_);
#else
    /*45 cycles per pipeline stage in steady state with unroll=1*/
    AE_S32_L_IP(vA1_, pa, -4); 
    AE_S32_L_IP(vB1_, pb1, 4);
    AE_S32_L_IP(AE_INTSWAP(vA0_), pa0, 4);
    AE_S32_L_IP(AE_INTSWAP(vB1_), pb0, -4);

#endif

    AE_L32X2_IP(vA0, p_y0, 8);
    AE_L32X2_XP(vA1, p_y1, step_back);
//    AE_L16X4_IP(vST, p_cos, 8);
//    vC0 = (vST);
//    AE_L16X4_IP(vST, p_w, 8);
//    vW1 = (vST);
    //////////////////////
  //  AE_L32X2_IP(w, pw32, sizeof(ae_int32x2));
  //  AE_L32X2_IP(w, pw32, sizeof(ae_int32x2));

    vB0 = AE_SUBADD32S(vA0, vA1);
    vB1 = AE_ADDSUB32S(vA0, vA1);
  //  vA0 = AE_MULFC32X16RAS_H(vB0, vC0);
    ////////////////
    AE_L32X2_IP(c, pcosi32, 8);
    //c = AE_INTSWAP(c);
    _cmult32x32(&vA0, &vB0, &c);

    vA0 = AE_SRAI32(vA0, 1);
    vA1 = AE_SRAI32(vB1, 1);

    vB0 = AE_SUB32S(vA1, vA0);
    vB1 = AE_ADD32S(vA1, vA0);

  //  vF0 = (vB0);
  //  vF1 = (vB1);

    /*
      z(k      )= real(T0)*real(W1)-imag(T0)*imag(W1);
      z(N+2-k  )=-real(T0)*imag(W1)-imag(T0)*real(W1);
      z(N/2+2-k)= real(T1)*real(W2)+imag(T1)*imag(W2);
      z(N/2+k  )=-real(T1)*imag(W2)+imag(T1)*real(W2);
    */
  //  vA0 = AE_MULFC32X16RAS_L(vB0, vW0);
   // vA1 = AE_NEG32S(vA0);
   // vB1 = AE_MULFC32X16RAS_H(vB1, vW1);
///////////////////////////
    AE_L32X2_IP(w, pw32, sizeof(ae_int32x2));
    _cmult32x32(&vA0, &vB0, &w);
    vA1 = AE_NEG32S(vA0);
    AE_L32X2_IP(w, pw32, sizeof(ae_int32x2));
    _cmult32x32(&vB1, &vB1, &w);
#ifdef TESTVA
    vA0._[0] = 0xA20000+i;
    vA0._[1] = 0xA21000+i;

    vA1._[0] = 0xA200 + i;
    vA1._[1] = 0xA210 + i;
    vB1._[0] = 0xB200 + i;
    vB1._[1] = 0xB210 + i;
#endif
    vA0_ = vA0;
    vA1_ = vA1; 
    vB1_ = vB1; 

   // vA0 = AE_SEL32_HH(vR0, vA0);
   // vB0 = AE_SEL32_LL(vR0, vB1);
   // vR0 = AE_SEL32_HL(vB1, vA1);

 //   AE_S32X2_IP(vA0, p_z0, sizeof(*p_z0));
 //   AE_S32X2_I(vB0, p_z0, 7*sizeof(*p_z0));
#if 1
    *pa0++ = AE_MOVAD32_H(vA0_);
    *pb1++ = AE_MOVAD32_L(vB1_);
    *pa-- = AE_MOVAD32_L(vA1_);
    *pb0-- = AE_MOVAD32_H(vB1_);
#else
    AE_S32_L_IP(vA1_, pa, -4);
    AE_S32_L_IP(vB1_, pb1, 4);
    AE_S32_L_IP(AE_INTSWAP(vA0_), pa0, 4);
    AE_S32_L_IP(AE_INTSWAP(vB1_), pb0, -4);
#endif
  }

  /*** middle sample ***/
  /*
    W1=w(N/4+1);
    T0=Y(N/4+1);
    z(N/4+1  )= real(T0)*real(W1)+imag(T0)*imag(W1);
    z(N+1-N/4)=-real(T0)*imag(W1)+imag(T0)*real(W1);
  */
  AE_L32X2_IP(vA0, p_y0, 8);

//  vB0 = AE_MULFC32X16RAS_L(vA0, vW1);
  AE_L32X2_IP(w, pw32, sizeof(ae_int32x2));
  _cmult32x32(&vB0, &vA0, &w);
#ifdef TESTVA
  vB0._[0] = 0xB300 + i;
  vB0._[1] = 0xB310 + i;
#endif
  *pa0 = AE_MOVAD32_H(vB0);
  *pa =  AE_MOVAD32_L(vB0);

 // vA0 = AE_SEL32_HH(vB0, vR0);
//  vA1 = AE_SEL32_LL(vB0, vR0);

//  AE_S32X2_I(vA0, p_z0, 0);
//  AE_S32X2_I(vA1, p_z1, 0);
#ifdef TESTVA
  printf("*"); 
#endif

}



/*-------------------------------------------------------------------------
  Discrete Cosine Transform :
  These functions apply DCT (Type II) to input
  NOTES:
  1. DCT runs in-place algorithm so INPUT DATA WILL APPEAR DAMAGED after 
     the call.

  Precision: 
  32x32  32-bit input/outputs, 32-bit twiddles
  24x24  24-bit input/outputs, 24-bit twiddles
  32x16  32-bit input/outputs, 16-bit twiddles
  16x16  16-bit input/outputs, 16-bit twiddles
  f      floating point

  Input:
  x[N]          input signal
  N             DCT size
  scalingOpt    scaling option
  Output:
  y[N]          transform output
  return ed value:
  total number of right shifts occurred during scaling procedure (always 5 
  for fixed point functions and 0 for floating point function)
  Restriction:
  x,y should not overlap
  x,y - aligned on 8-bytes boundary
  N   - 32 for fixed point routines, 32 or 64 for floating point

-------------------------------------------------------------------------*/
int dct_32x32( 
              int32_t* y,
              int32_t* x,
              int N,
              int scalingOpt)
{
    int i; 

    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);
    NASSERT(scalingOpt==3);
    NASSERT(N==32);



    //memcpy(test_x, x, sizeof(int32_t)*N); 

    for (i = 0; i < N / 2; i++)
    {
        y[i] = x[i * 2];
        y[N - 1 - i] = x[i * 2 + 1];
    }

    fft_cplx32x32(x, y, cfft32_16, 3);

   // fft16_32x16(x, y);    /* fft of half-size with reordering */
    if (0)
    {
        int i; 
        for (i = 0; i < 32; i++)
        {
            x[i] = y[i]; 
        }
    }
    dct_split_32_32x16(y,x);
    return 5;
}
