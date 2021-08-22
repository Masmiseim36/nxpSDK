/************************************************************************************************
*
* Copyright 2013-2016, Freescale Semiconductor, Inc.
* Copyright 2016-2021 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*
****************************************************************************//*!
*
* @file      fft.h
*
* @author    B19186
* 
* @version   4.0.0.2
* 
* @date      July-24-2015
* 
* @brief     FFT library header based on the 'In place' radix-2/radix-4   
*            algorithms with supporting both the interpolation and fractional  
*            computing. For more information see AN4255 and AN4847.
*******************************************************************************/
#ifndef __FFT_H__
#define __FFT_H__

#define INTERP_BUFF 128                   ///< max. interpolation buffer length                 

/*******************************************************************************
 * public function prototypes and macro definitions                            *
 ******************************************************************************/
/***************************************************************************//*!
*
* @brief  Logarithm base 2 computing
*
* @param  n   Input number 
*
* @return log n (base 2)
*
*******************************************************************************/
extern unsigned long Log2n(unsigned long n);

/***************************************************************************//*!
*
* @brief   FFT DIT radix-2 Cooley-Tukey algorithm
*
* @param   n      Number of input samples (must be power-of-two)
* @param   stage  Number of FFT stages = log2(n)
* @param   x_re   Pointer to the input time domain buffer and frequency output 
                  buffer (real part) 
*          x_im   Pointer to the frequency output buffer (imaginary part)
*
* @remarks Data inside the input time domain buffer referenced by x_re pointer   
*          will be rewrited by the FFT real part values, while imaginary parts 
*          of the FFT result are saved to buffer referenced by x_im pointer. 
*          The first buffer position matches with zero harmonic, etc.
*
*******************************************************************************/
extern void FFTradix2(unsigned long n, unsigned long stage, frac24 *x_re, frac24 *x_im);

/***************************************************************************//*!
*
* @brief   FFT DIF radix-4 Cooley-Tukey algorithm
*
* @param   n      Number of input samples (must be power-of-four)
* @param   stage2 Number of FFT stages multiplied by two = 2log2(n)
* @param   x_re   Pointer to the input time domain buffer and frequency output 
                  buffer (real part) 
*          x_im   Pointer to the frequency output buffer (imaginary part)
*
* @remarks Data inside the input time domain buffer referenced by x_re pointer   
*          will be rewrited by the FFT real part values, while imaginary parts 
*          of the FFT result are saved to buffer referenced by x_im pointer. 
*          The first buffer position matches with zero harmonic, etc.
*
*******************************************************************************/
extern void FFTradix4(unsigned long n, unsigned long stage2, frac24 *x_re, frac24 *x_im);

/***************************************************************************//*!
*
* @brief    Interpolation
*
* @param    order      Interpolation order (1, 2 or 3)
*           buff       Pointer to the input/output buffer (Q0.23 data format )
*           log2_fft   log2(required FFT samples), e.g. 6 for FFT samples=64
*           inp_n      Number of input samples (> or < than FFT samples) 
*
* @remarks  Performs input data interpolation according to the selected scheme:
*           linear, quadratic, or cubic.
*           Required FFT samples must be power-of-two. 
*           Required FFT samples must be <= INTERP_BUFF when inp_n < FFT samples.
*           Supports oversampling and undersampling.
*
*******************************************************************************/
#define Interpolation(order,buff,log2_fft,inp_n) Interpolation##order##(buff,log2_fft,inp_n)

extern void InterpolationORD1(frac24 *buff, unsigned long log2_fft, unsigned long inp_n);
extern void InterpolationORD2(frac24 *buff, unsigned long log2_fft, unsigned long inp_n);
extern void InterpolationORD3(frac24 *buff, unsigned long log2_fft, unsigned long inp_n);
#endif /* __FFT_H__ */
