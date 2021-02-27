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
*/
/*===========================================================================

    This is last stage of complex FFT. Used in 
    fft_cplx_24x24_ie, fft_cplx_32x16_ie.

===========================================================================*/
#include "NatureDSP_Signal.h"
#include "common.h"

int fft_stage_last_ie(    int32_t *x, 
                        int32_t *y, 
                        int N)
{
    int32_t i,i0,i1,ai;
    ae_int32x2 * restrict p_y0 = (ae_int32x2 *)(y);
    ae_int32x2 * restrict p_y1 = (p_y0 + (N >> 2));
    ae_int32x2 * restrict p_y2 = (p_y1 + (N >> 2));
    ae_int32x2 * restrict p_y3 = (p_y2 + (N >> 2));
    ae_int32x2 * restrict p_x0 = (ae_int32x2 *)(x);
    int shift; 

    ae_int32x2 tmp; 

    tmp = AE_MOVDA32X2(N, N); 
    i = AE_NSAZ32_L(tmp)+1;
    ai=((int32_t)0x1)<<i;
    i0=0;

    
    if ( (i&1)==0 ) 
    {
        shift = 1;    //Select scaling
        WUR_AE_SAR(shift); 
        //--------------------------------------------------------------------------
        // last stage is RADIX2 !!!
        //--------------------------------------------------------------------------
/*

#<swps>  18 cycles per pipeline stage in steady state with unroll=1
#<swps>   2 pipeline stages
#<swps>  34 real ops (excluding nop)
#<swps> 
#<swps>      min  18 cycles required by resources
#<swps>      min   2 cycles required by recurrences
#<swps>      min  18 cycles required by resources/recurrence
#<swps>      min   6 cycles required for critical path
#<swps>           32 cycles non-loop schedule length
*/
        i = N>>3; 
        do
        {
            ae_int32x2 vA1, vA2, vA3, vA0; 
            ae_int32x2 vB1, vB2, vB3, vB0; 
            i1 = AE_ADDBRBA32(i0, ai);

            // FFT_BUTTERFLY_R2(i0, shift);

            AE_L32X2_IP(vA0,  p_x0, 8);    
            AE_L32X2_IP(vA1,  p_x0, 8);    
            AE_L32X2_IP(vA2,  p_x0, 8);    
            AE_L32X2_IP(vA3,  p_x0, 8);     

#if 0
            vA0 =  AE_SRAA32RS(vA0, shift); 
            vA1 =  AE_SRAA32RS(vA1, shift); 
            vA2 =  AE_SRAA32RS(vA2, shift); 
            vA3 =  AE_SRAA32RS(vA3, shift); 
#else
            vA0 = AE_SRAS32(vA0); 
            vA1 = AE_SRAS32(vA1); 
            vA2 = AE_SRAS32(vA2); 
            vA3 = AE_SRAS32(vA3); 
#endif
            vB0 = AE_ADD32S(vA0, vA1);     
            vB2 = AE_SUB32S(vA0, vA1);     
            vB1 = AE_ADD32S(vA2, vA3);     
            vB3 = AE_SUB32S(vA2, vA3);     
            AE_S32X2_X(vB0, p_y0, i0);    
            AE_S32X2_X(vB1, p_y1, i0);    
            AE_S32X2_X(vB2, p_y2, i0);    
            AE_S32X2_X(vB3, p_y3, i0);    

            //FFT_BUTTERFLY_R2(i1, shift);

            AE_L32X2_IP(vA0,  p_x0, 8);    
            AE_L32X2_IP(vA1,  p_x0, 8);    
            AE_L32X2_IP(vA2,  p_x0, 8);    
            AE_L32X2_IP(vA3,  p_x0, 8);     
#if 0
            vA0 =  AE_SRAA32RS(vA0, shift); 
            vA1 =  AE_SRAA32RS(vA1, shift); 
            vA2 =  AE_SRAA32RS(vA2, shift); 
            vA3 =  AE_SRAA32RS(vA3, shift); 
#else
            vA0 = AE_SRAS32(vA0); 
            vA1 = AE_SRAS32(vA1); 
            vA2 = AE_SRAS32(vA2); 
            vA3 = AE_SRAS32(vA3); 
#endif

            vB0 = AE_ADD32S(vA0, vA1);     
            vB2 = AE_SUB32S(vA0, vA1);     
            vB1 = AE_ADD32S(vA2, vA3);     
            vB3 = AE_SUB32S(vA2, vA3);     

            AE_S32X2_X(vB0, p_y0, i1);    
            AE_S32X2_X(vB1, p_y1, i1);    
            AE_S32X2_X(vB2, p_y2, i1);    
            AE_S32X2_X(vB3, p_y3, i1);                        
          i0 = AE_ADDBRBA32(i1, ai);
        }while(--i); 
    } 
    else 
    {  

        //--------------------------------------------------------------------------
        // last stage is RADIX4 !!!
        //--------------------------------------------------------------------------
        shift = 2;    //Select scaling
        WUR_AE_SAR(shift); 
/*

#<swps>   9 cycles per pipeline stage in steady state with unroll=1
#<swps>   2 pipeline stages
#<swps>  22 real ops (excluding nop)
#<swps> 
#<swps>      min   9 cycles required by resources
#<swps>      min   2 cycles required by recurrences
#<swps>      min   9 cycles required by resources/recurrence
#<swps>      min   6 cycles required for critical path
#<swps>           18 cycles non-loop schedule length

*/  
        i = N>>2; 
        do//for (i = 0; i < (N>>4); i++) 
        {
            ae_int32x2 vA1, vA2, vA3, vA0; 
            ae_int32x2 vB1, vB2, vB3, vB0; 

            //     FFT_BUTTERFLY_R4(i0, shift);
            AE_L32X2_IP(vA0,  p_x0, 8);    
            AE_L32X2_IP(vA1,  p_x0, 8);    
            AE_L32X2_IP(vA2,  p_x0, 8);    
            AE_L32X2_IP(vA3,  p_x0, 8);     
#if 0
            vA0 =  AE_SRAA32RS(vA0, shift); 
            vA1 =  AE_SRAA32RS(vA1, shift); 
            vA2 =  AE_SRAA32RS(vA2, shift); 
            vA3 =  AE_SRAA32RS(vA3, shift); 
#else
            vA0 = AE_SRAS32(vA0); 
            vA1 = AE_SRAS32(vA1); 
            vA2 = AE_SRAS32(vA2); 
            vA3 = AE_SRAS32(vA3); 
#endif
            vB0 = AE_ADD32S(vA0, vA2);    
            vB2 = AE_SUB32S(vA0, vA2);    
            vB1 = AE_ADD32S(vA1, vA3);    
            vB3 = AE_SUB32S(vA1, vA3);    
            vB3 = AE_SEL32_LH(vB3, vB3);  
            vA0 = AE_ADD32S(vB0, vB1);    
            vA2 = AE_SUB32S(vB0, vB1);    
            vA1 = AE_ADDSUB32S(vB2, vB3); 
            vA3 = AE_SUBADD32S(vB2, vB3); 
            AE_S32X2_X(vA0, p_y0, i0);   
            AE_S32X2_X(vA1, p_y1, i0);   
            AE_S32X2_X(vA2, p_y2, i0);   
            AE_S32X2_X(vA3, p_y3, i0);   
            i0 = AE_ADDBRBA32(i0, ai);
        }while(--i); 
    }
    return shift;
} //fft_stage_last_ie

