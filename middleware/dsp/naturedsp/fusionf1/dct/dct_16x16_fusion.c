/* ------------------------------------------------------------------------ */
/* Copyright (c) 2016 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (ÿCadence    */
/* Librariesÿ) are the copyrighted works of Cadence Design Systems Inc.	    */
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

/*
    MATLAB:
    N = 32;
    w=exp(1j*pi/2*(0:N/2-1)/N);
    w = [conj(w(2:N/4)), w(N/4+1), w(end:-1:N/4+1)];
    tmp = round(w * 2^15);
    tmp = [real(tmp); imag(tmp)];
    tmp = reshape(tmp, N, 1);
    fprintf('    (int16_t)0x%04X,\n', typecast(int16(tmp), 'uint16'));
*/
static const int16_t ALIGN(8) dct32_w_tab[32] =
{
    (int16_t)0x7FD9,
    (int16_t)0xF9B8,
    (int16_t)0x7F62,
    (int16_t)0xF374,
    (int16_t)0x7E9D,
    (int16_t)0xED38,
    (int16_t)0x7D8A,
    (int16_t)0xE707,
    (int16_t)0x7C2A,
    (int16_t)0xE0E6,
    (int16_t)0x7A7D,
    (int16_t)0xDAD8,
    (int16_t)0x7885,
    (int16_t)0xD4E1,
    (int16_t)0x7642,
    (int16_t)0x30FC,
    (int16_t)0x5ED7,
    (int16_t)0x55F6,
    (int16_t)0x62F2,
    (int16_t)0x5134,
    (int16_t)0x66D0,
    (int16_t)0x4C40,
    (int16_t)0x6A6E,
    (int16_t)0x471D,
    (int16_t)0x6DCA,
    (int16_t)0x41CE,
    (int16_t)0x70E3,
    (int16_t)0x3C57,
    (int16_t)0x73B6,
    (int16_t)0x36BA,
    (int16_t)0x7642,
    (int16_t)0x30FC,
};
/*
MATLAB:
    N = 32;
    cosi = -exp(2 * 1j * pi*(0:N / 4 - 1) / (N));
    cosi = [1 / sqrt(2)*(1 + 1j)*[1, 1], cosi(2:end)];
    tmp = round(cosi * 2 ^ 15);
    tmp = [real(tmp); imag(tmp)];
    tmp = reshape(tmp, numel(tmp), 1);
    fprintf('=============================\n');
    fprintf('    (int16_t)0x%04X,\n', typecast(int16(tmp), 'uint16'));
*/
static ALIGN(8) const int16_t dct32_cosi_tab[] = {
    (int16_t)0x5A82,
    (int16_t)0x5A82,
    (int16_t)0x5A82,
    (int16_t)0x5A82,
    (int16_t)0x8276,
    (int16_t)0xE707,
    (int16_t)0x89BE,
    (int16_t)0xCF04,
    (int16_t)0x9592,
    (int16_t)0xB8E3,
    (int16_t)0xA57E,
    (int16_t)0xA57E,
    (int16_t)0xB8E3,
    (int16_t)0x9592,
    (int16_t)0xCF04,
    (int16_t)0x89BE,
    (int16_t)0xE707,
    (int16_t)0x8276
}; 

#if 0 //__MNSD__
inline_ int16_t cmulre(int16_t xre, int16_t xim, int16_t yre, int16_t yim)
{
    int32_t acc = L_mpy_ss(xre, yre); 
    acc = L_mas_ss(acc, xim, yim);
    return  S_round_l(acc);      
    //return S_round_l(L_sub_ll(L_mpy_ss(xre, yre), L_mpy_ss(xim, yim)));
}

inline_ int16_t cmulim(int16_t xre, int16_t xim, int16_t yre, int16_t yim)
{
    int32_t acc = L_mpy_ss(xre, yim);
    acc = L_mac_ss(acc, xim, yre);
    return S_round_l(acc);
}
#endif 

#if XCHAL_HAVE_FUSION_16BIT_BASEBAND
/* Quad MAC supported */
inline_ void MULCx2_(ae_int16x4 *x, const ae_int16x4 *t)
{
    ae_f16x4 f;
    f = AE_MULFC16RAS_L(AE_MOVF16X4_FROMINT16X4(*x), AE_MOVF16X4_FROMINT16X4(*t));
    AE_MULFC16RAS_H(f, AE_MOVF16X4_FROMINT16X4(*x), AE_MOVF16X4_FROMINT16X4(*t));
    *x = AE_MOVINT16X4_FROMF16X4(f);
}
#else
/* No Quad MAC */
inline_ void MULCx2_(ae_int16x4 *x, const ae_int16x4 *t)
{
    ae_f32x2 hi,lo,zhi,zlo;
    ae_f16x4 f;

    hi=AE_CVT32X2F16_32(AE_MOVF16X4_FROMINT16X4(*x));
    lo=AE_CVT32X2F16_10(AE_MOVF16X4_FROMINT16X4(*x));
    zhi=AE_MULFCI32X16RAS_H(hi, AE_MOVF16X4_FROMINT16X4(*t));
    AE_MULFCR32X16RAS_H(zhi,hi, AE_MOVF16X4_FROMINT16X4(*t));
    zlo=AE_MULFCI32X16RAS_L(lo, AE_MOVF16X4_FROMINT16X4(*t));
    AE_MULFCR32X16RAS_L(zlo,lo, AE_MOVF16X4_FROMINT16X4(*t));
    f=AE_ROUND16X4F32SASYM(zhi,zlo);
    *x = AE_MOVINT16X4_FROMF16X4(f);
}
#endif

static void dct_split16x16(int16_t * restrict y, int16_t * restrict z)
{
    const int N = 32;

#if 0
    {
    // Reference code
    
    int k;

    const int16_t *pw1 = dct32_w_tab;
    const int16_t *pw2 = dct32_w_tab + N / 2;
    const int16_t *pcosi = dct32_cosi_tab;
    int16_t t0_re, t0_im, t1_re, t1_im;
    int16_t tw0r, tw0i, tw1r, tw1i, tw2r, tw2i;

    z[0] = S_add_ss(y[0], y[1]);
    z[N / 2] = S_mpy_ss(S_sub_ss(y[0], y[1]), pcosi[0]);
    pcosi += 4;

    for (k = 1; k < N / 4; k++)
    {
        int16_t y0_re, y0_im, y1_re, y1_im;
        int16_t s_re, s_im, d_re, d_im;

        tw1r = *pw1++;
        tw1i = *pw1++;
        tw2r = *pw2++;
        tw2i = *pw2++;

        y0_re = y[2 * k];
        y0_im = y[2 * k + 1];
        y1_re = y[N - 2 * k];
        y1_im = y[N - 2 * k + 1];

        s_re = S_add_ss(y0_re, y1_re); // S = Y0 + Y1;
        s_im = S_add_ss(y0_im, y1_im); // D = Y0 - Y1;
        d_re = S_sub_ss(y0_re, y1_re); // T0 = i*real(D) + imag(S);
        d_im = S_sub_ss(y0_im, y1_im); // T1 = i*imag(D) + real(S);
        // T1 = T1 / 2;
        t0_re = s_im;
        t0_im = d_re;
        t1_re = s_re >> 1;
        t1_im = d_im >> 1;
        // COSI = cosi(k);
        // Y0 = (imag(T0)*imag(COSI) - real(T0)*real(COSI)) + ...
        //     i*(real(T0)*imag(COSI) + imag(T0)*real(COSI));
        tw0r = *pcosi++;
        tw0i = *pcosi++;
        y0_re = cmulre(t0_re, t0_im, tw0r, tw0i);
        y0_im = S_neg_s(cmulim(t0_re, t0_im, tw0r, tw0i));
        y0_re >>= 1;  // Y0 = Y0 / 2;
        y0_im >>= 1;

        t0_re = S_sub_ss(t1_re, y0_re);
        t0_im = S_sub_ss(t1_im, y0_im);

        t1_re = S_add_ss(t1_re, y0_re);
        t1_im = S_add_ss(t1_im, y0_im);

        z[k] = cmulre(t0_re, t0_im, tw1r, tw1i);
        z[N - k] = S_neg_s(cmulim(t0_re, t0_im, tw1r, tw1i));
        z[N / 2 - k] = cmulre(t1_re, t1_im, tw2r, tw2i);
        z[N / 2 + k] = cmulim(t1_re, t1_im, tw2r, tw2i);

    }

    t0_re = y[2 * N / 4 + 0];
    t0_im = y[2 * N / 4 + 1];
    tw1r = *pw1++;
    tw1i = *pw1++;
    z[N / 4] = cmulre(t0_re, t0_im, tw1r, tw1i);
    z[3 * N / 4] = cmulim(t0_re, t0_im, tw1r, tw1i);
}
#else
    ae_int16  * restrict pzk = (ae_int16*)(z + 1);
    ae_int32  * restrict pzN_k_1 = (ae_int32 *)&z[N - (1 + 1)];
    ae_int16  * restrict pzN2k = (ae_int16*)&z[N / 2 + 1]; 
    ae_int32  * restrict pzN2_k_1 = (ae_int32 *)&z[N / 2 - (1 + 1)];

    ae_int16x4 * restrict py0 = (ae_int16x4*)(&y[2]);
    ae_int16x4 * restrict py1 = (ae_int16x4*)(&y[N - 4]);
    ae_int16x4 * restrict ptw0 = (ae_int16x4*)dct32_cosi_tab; 
    ae_int16x4 * restrict ptw1 = (ae_int16x4*)dct32_w_tab;
    ae_int16x4 * restrict ptw2 = (ae_int16x4*)(dct32_w_tab + N / 2);

    {
        int k;  
        ae_int16x4 y0, y1, tw0, z0, z1, tw1, t0;
        ae_valign v0 = AE_LA64_PP(py0); 
        ae_f32x2 f32;

        NASSERT_ALIGN8(py1); 
        xtbool4 mov_even = (int)0x5;


        // z[0] = S_add_ss(y[0], y[1]);
        // z[N / 2] = S_mpy_ss(S_sub_ss(y[0], y[1]), pcosi[0]);
        y0 = AE_L16_I((ae_int16*)y, 0);
        y1 = AE_L16_I((ae_int16*)y, 2);
        AE_L16X4_IP(tw0, ptw0, sizeof(*ptw0));
        z0 = AE_ADD16S(y0, y1); 
        z1 = AE_SUB16S(y0, y1); 
        f32 = AE_MULF16SS_00(AE_MOVF16X4_FROMINT16X4(z1), AE_MOVF16X4_FROMINT16X4(tw0));
        z1 = AE_MOVINT16X4_FROMF16X4(AE_ROUND16X4F32SASYM(f32, f32)); 
        AE_S16_0_I(z0, ( ae_int16*)z, 0);
        AE_S16_0_X(z1, ( ae_int16*)z, N/2*sizeof(int16_t));

        /*20 cycles per pipeline stage in steady state with unroll=1*/
        for (k = 1; k < N / 4-1 ; k += 2)
        {
            ae_int16x4  s, d, t1, tw1, tw2; 
 
            AE_L16X4_IP(tw1, ptw1, sizeof(*ptw1) );
            AE_L16X4_IP(tw2, ptw2, sizeof(*ptw2) );

            AE_LA16X4_IP(y0, v0, py0);
            AE_L16X4_XP(y1, py1, - (int)sizeof(*py1));

            y1 = AE_SEL16_5432(y1, y1); 
            s = AE_ADD16S(y0, y1); 
            d = AE_SUB16S(y0, y1);


            t0 = d;
            AE_MOVT16X4(t0, s, mov_even);
            /* Swap re <-> im */
            t0 = AE_MOVINT16X4_FROMINT32X2(AE_INTSWAP(AE_MOVINT32X2_FROMINT16X4(AE_SHORTSWAP(t0))));

            t1 = s;
            AE_MOVT16X4(t1, d, mov_even);
            t1 = AE_SRAI16(t1, 1); 

            // COSI = cosi(k);
            // Y0 = (imag(T0)*imag(COSI) - real(T0)*real(COSI)) + ...
            //     i*(real(T0)*imag(COSI) + imag(T0)*real(COSI));

            AE_L16X4_IP(tw0, ptw0, sizeof(*ptw0)); // tw0 = AE_L16X4_I((ae_int16x4*)pcosi, 0);
                                                    // pcosi += 4;
            y0 = t0; 
            MULCx2_(&y0, &tw0);
            AE_MOVT16X4(y0, AE_NEG16S(y0), mov_even); 
            y0 = AE_SRAI16(y0, 1); 
        
            t0 = AE_SUB16S(t1, y0);
            t1 = AE_ADD16S(t1, y0); 

            z0 = t0; 
            z1 = t1; 
            MULCx2_(&z0, &tw1);
            MULCx2_(&z1, &tw2);

            AE_MOVT16X4(z0, AE_NEG16S(z0), mov_even);
                  
            t0 = AE_SHORTSWAP(z0); 
            //  z[k] = AE_MOVAD16_3(z0);
            AE_S16_0_IP(t0, pzk, sizeof(int16_t)); 
            t0 = AE_MOVINT16X4_FROMINT32X2(AE_INTSWAP(AE_MOVINT32X2_FROMINT16X4((t0))));
            //  z[(k + 1)] = AE_MOVAD16_1(z0); 
            AE_S16_0_IP(t0, pzk, sizeof(int16_t));             

           // z[N - k] =          AE_MOVAD16_2(z0);            
           // z[N - (k + 1)] =    AE_MOVAD16_0(z0);
            t0 = AE_SEL16_7520(z0, z0); 
            AE_S32_L_IP(AE_MOVINT32X2_FROMINT16X4(t0), pzN_k_1, -(int)sizeof(int32_t));

            
            t1 = AE_SEL16_7520(z1, z1);
            t1 = AE_MOVINT16X4_FROMINT32X2(AE_INTSWAP(AE_MOVINT32X2_FROMINT16X4((t1))));

            // z[N / 2 - k] = AE_MOVAD16_3(z1);
            // z[N / 2 - (k + 1)] = AE_MOVAD16_1(z1);
            AE_S32_L_IP(AE_MOVINT32X2_FROMINT16X4(t1), pzN2_k_1, -(int)sizeof(int32_t));

            
            t0 = AE_MOVINT16X4_FROMINT32X2(AE_INTSWAP(AE_MOVINT32X2_FROMINT16X4((z1))));
            // z[N / 2 + k] = AE_MOVAD16_2(z1);
            AE_S16_0_IP(t0, pzN2k, sizeof(int16_t));
            // z[N / 2 + (k + 1)] = AE_MOVAD16_0(z1);
            AE_S16_0_IP(z1, pzN2k, sizeof(int16_t));

        }  
        {
            ae_int16x4 y0, y1, s, d, t0, t1, tw0, tw2, z0, z1;

            AE_L16X4_IP(tw1, ptw1, sizeof(*ptw1));
            AE_L16X4_IP(tw2, ptw2, sizeof(*ptw2));

            AE_LA16X4_IP(y0, v0, py0);
            AE_L16X4_XP(y1, py1, -(int)sizeof(*py1));

            y1 = AE_SEL16_5432(y1, y1);
            s = AE_ADD16S(y0, y1);
            d = AE_SUB16S(y0, y1);

            t0 = d;
            AE_MOVT16X4(t0, s, mov_even);
            /* Swap re <-> im */
            t0 = AE_MOVINT16X4_FROMINT32X2(AE_INTSWAP(AE_MOVINT32X2_FROMINT16X4(AE_SHORTSWAP(t0))));

            t1 = s;
            AE_MOVT16X4(t1, d, mov_even);
            t1 = AE_SRAI16(t1, 1);

            // COSI = cosi(k);
            // Y0 = (imag(T0)*imag(COSI) - real(T0)*real(COSI)) + ...
            //     i*(real(T0)*imag(COSI) + imag(T0)*real(COSI));
            tw0 = AE_L16X4_I(ptw0, 0);

            y0 = t0;
            MULCx2_(&y0, &tw0);
            AE_MOVT16X4(y0, AE_NEG16S(y0), mov_even);
            y0 = AE_SRAI16(y0, 1);
        

            t0 = AE_SUB16S(t1, y0);
            t1 = AE_ADD16S(t1, y0);

            z0 = t0;
            z1 = t1;
            MULCx2_(&z0, &tw1);
            MULCx2_(&z1, &tw2);

            AE_MOVT16X4(z0, AE_NEG16S(z0), mov_even);
            z[k] = AE_MOVAD16_3(z0);
            z[N - k] = AE_MOVAD16_2(z0);

            z[N / 2 - k] = AE_MOVAD16_3(z1);
            z[N / 2 + k] = AE_MOVAD16_2(z1);
        }

        {   
            /*
            int16_t t0_re = y[2 * N / 4 + 0];
            int16_t t0_im = y[2 * N / 4 + 1];
            int16_t tw1r = *pw1++;
            int16_t tw1i = *pw1++;

            z[N / 4] = cmulre(t0_re, t0_im, tw1r, tw1i);
            z[3 * N / 4] = cmulim(t0_re, t0_im, tw1r, tw1i);
            */
            t0 = AE_L16X4_X( (ae_int16x4*)y, 2 * N / 4 * sizeof(int16_t) ); 
            t0 = AE_MOVINT16X4_FROMINT32X2(AE_INTSWAP(AE_MOVINT32X2_FROMINT16X4(t0)));
#if XCHAL_HAVE_FUSION_16BIT_BASEBAND
            t0 = AE_MOVINT16X4_FROMF16X4(AE_MULFC16RAS_L(AE_MOVF16X4_FROMINT16X4(t0), AE_MOVF16X4_FROMINT16X4(tw1) ) );
#else
            MULCx2_(&t0,&tw1);
#endif
            z[N / 4] =      AE_MOVAD16_1(t0);
            z[3 * N / 4] =  AE_MOVAD16_0(t0); 
        }

    }
    
#endif
}

int dct_16x16(int16_t   * y, int16_t   * x, int N, int scalingOpt)
{
    int i ; 
    ae_int32x2 *restrict px = (ae_int32x2 *)x;
    ae_int16x4 *restrict py0 = (ae_int16x4 *)y;
    ae_int16x4 *restrict py1 = (ae_int16x4 *)(y+32-4); 
   

    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);
    NASSERT(scalingOpt == 3);
    NASSERT(N == 32);

    for (i = 0; i < N / 8; i++)
    {
        ae_int32x2 a32, b32;
        ae_int16x4 a, b, A, B;
        /*
            y[i] = x[i * 2]; 
            y[N - 1 - i] = x[i * 2 + 1];
        */

        AE_L32X2_IP(a32, px, sizeof(*px));
        AE_L32X2_IP(b32, px, sizeof(*px));
      
        a = AE_MOVINT16X4_FROMINT32X2(a32);
        b = AE_MOVINT16X4_FROMINT32X2(b32);

        A = (AE_SEL16_6420(a, b)); 
        B = (AE_SEL16_6420(AE_SHORTSWAP(b), AE_SHORTSWAP(a)));
        AE_S16X4_IP(A, py0, sizeof(*py0) );
        AE_S16X4_XP(B, py1, -8);
    }

    fft_cplx16x16(x, y, cfft16x16_16,  3); 
    dct_split16x16(x, y);


    return 5;
}
