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
    NatureDSP Signal Processing Library. FIR part
    Real data linear auto-correlation, 32x32, no requirements on vectors
    length and alignment.
*/

/*-------------------------------------------------------------------------
  Linear Autocorrelation 
  Functions estimate the linear auto-correlation of vector x. Returns 
  autocorrelation of length N.

  Precision: 
  16x16   16-bit data, 16-bit outputs
  32x32   32-bit data, 32-bit outputs
  f       floating point

  Input:
  s[]       scratch area of
            FIR_LACORRA16X16_SCRATCH_SIZE( N )
            FIR_LACORRA32X32_SCRATCH_SIZE( N )
            FIR_LACORRAF_SCRATCH_SIZE( N )    
            bytes
  x[N]      input data Q31, Q15 or floating point
  N         length of x
  Output:
  r[N]      output data, Q31, Q15 or floating point

  Restrictions:
  x,r,s should not overlap
  N >0
  s   - aligned on an 8-bytes boundary
-------------------------------------------------------------------------*/
/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"

/* Common utility and macros declarations. */
#include "common.h"

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
    (void*)(((uintptr_t)(addr)+((align)-1)) & ~((align)-1))

void fir_lacorra32x32(void     * restrict s,
                      int32_t  * restrict r,
                const int32_t  * restrict x, int N){

    const ae_f32x2 *          X;
    const ae_f32x2 *          S;
    const ae_f32x2 *          Y;
          ae_f32x2 * restrict R;

    void * s_ptr;
    int32_t  * x_buf;
    ae_f32x2 * restrict D;
    ae_valign S_va, D_va;

    int n, m;

    ae_f64     q0, q1, q2, q3;
    ae_int32x2 t0, t1, t2;
    ae_f32x2   x0, x1, x2;
    ae_f32x2   y0;

    ae_valign  ar;

    NASSERT(s);
    NASSERT(r);
    NASSERT(x);
    NASSERT_ALIGN(s, 8);
    NASSERT(N >= 0);

    //----------------------------------------------------------------------------
    // Partition the scratch memory area.
    s_ptr = s;
    x_buf = (int32_t*)ALIGNED_ADDR(s_ptr, 8);
    s_ptr = x_buf + N + 4;

    ASSERT((int8_t *)s_ptr - (int8_t *)s <=
        (int)FIR_LACORRA32X32_SCRATCH_SIZE(N));
    //----------------------------------------------------------------------------
    S = (const ae_f32x2 *)x;
    D = (ae_f32x2 *)(x_buf);
    S_va = AE_LA64_PP(S);
    D_va = AE_ZALIGN64();
    for (m = 0; m < ((N + 1) >> 1); m++)
    {
        AE_LA32X2_IP(t0, S_va, S); x0 = (t0);
        AE_S32X2_IP(x0, D, +8);
    }

    D = (ae_f32x2 *)(x_buf+N);
    AE_S32_L_I( AE_ZERO32(),(ae_int32*) D, 0*sizeof(int32_t) );
    AE_S32_L_I( AE_ZERO32(),(ae_int32*) D, 1*sizeof(int32_t) );
    AE_S32_L_I( AE_ZERO32(),(ae_int32*) D, 2*sizeof(int32_t) );
    

    /*
    * Compute r[0] and r[1] entries.
    */
    {
        int n_iter = XT_MIN(N, 2);
        int m_iter = ((N + 1) >> 1) - 1;

        ar = AE_ZALIGN64();

        X = (const ae_f32x2 *)x_buf;
        R = (ae_f32x2 *)(r);

        if (n_iter)
        {
            AE_L32X2_IP(t0, X, 8); x0 = (t0);
            AE_L32X2_IP(t1, X, 8); x1 = (t1);
            S = X;
            AE_L32X2_IP(t2, S, 8); x2 = (t2);

            Y = (const ae_f32x2 *)x_buf;

            AE_L32X2_IP(t0, Y, 8); y0 = (t0);

            q0 = AE_MULF32R_HH(x0, y0);
            AE_MULAF32R_LL(q0, x0, y0);

            q1 = AE_MULF32R_LH(x0, y0);
            AE_MULAF32R_LH(q1, y0, x1);

            //__Pragma("loop_count min=1")
            for (m = 0; m < m_iter; m++){

                x0 = x1; x1 = x2;
                AE_L32X2_IP(t2, S, 8); x2 = (t2);

                AE_L32X2_IP(t0, Y, 8);
                y0 = (t0);

                AE_MULAF32R_HH(q0, x0, y0);
                AE_MULAF32R_LL(q0, x0, y0);

                AE_MULAF32R_LH(q1, x0, y0);
                AE_MULAF32R_LH(q1, y0, x1);
            }

            x0 = AE_ROUND32X2F48SASYM(q0, q1);
            AE_S32_L_IP(AE_INTSWAP(x0), castxcc(ae_int32, R), 4);
            if (n_iter>1) AE_S32_L_IP(x0, castxcc(ae_int32, R), 4);
        }
    }


    /*
    * Compute r[2]....r[N-1] entries.
    */
    {
        int n_iter = XT_MAX(0, N - 2);
        int m_iter = (N >> 1) - 2;

        X = (const ae_f32x2 *)(x_buf + 2);
        R = (ae_f32x2 *)(r + 2);

        ar = AE_ZALIGN64();

        for (n = 0; n < (n_iter&~3); n += 4, m_iter -= 2){
            AE_L32X2_IP(t0, X, 8); x0 = (t0);
            AE_L32X2_IP(t1, X, 8); x1 = (t1);
            S = X;
            AE_L32X2_IP(t2, S, 8); x2 = (t2);

            Y = (const ae_f32x2 *)(x_buf);
            AE_L32X2_IP(t0, Y, 8); y0 = (t0);

            q0 = AE_MULF32R_HH(x0, y0);
            AE_MULAF32R_LL(q0, x0, y0);

            q1 = AE_MULF32R_LH(x0, y0);
            AE_MULAF32R_LH(q1, y0, x1);

            q2 = AE_MULF32R_HH(x1, y0);
            AE_MULAF32R_LL(q2, x1, y0);

            q3 = AE_MULF32R_LH(x1, y0);
            AE_MULAF32R_LH(q3, y0, x2);

            for (m = 0; m < m_iter; m++){
                x0 = x1; x1 = x2;
                AE_L32X2_IP(t2, S, 8); x2 = (t2);

                AE_L32X2_IP(t0, Y, 8);  y0 = (t0);

                AE_MULAF32R_HH(q0, x0, y0);
                AE_MULAF32R_LL(q0, x0, y0);

                AE_MULAF32R_LH(q1, x0, y0);
                AE_MULAF32R_LH(q1, y0, x1);

                AE_MULAF32R_HH(q2, x1, y0);
                AE_MULAF32R_LL(q2, x1, y0);

                AE_MULAF32R_LH(q3, x1, y0);
                AE_MULAF32R_LH(q3, y0, x2);
            }

            AE_L32X2_IP(t0, Y, 8); y0 = (t0);

            AE_MULAF32R_HH(q0, x1, y0);
            AE_MULAF32R_LL(q0, x1, y0);

            AE_MULAF32R_LH(q1, x1, y0);

            x0 = AE_ROUND32X2F48SASYM(q0, q1); AE_SA32X2_IP(x0, ar, R);
            x0 = AE_ROUND32X2F48SASYM(q2, q3); AE_SA32X2_IP(x0, ar, R);
        }
        AE_SA64POS_FP(ar, R);

        n_iter &= 3;
        if (n_iter){
            AE_L32X2_IP(t0, X, 8); x0 = (t0);
            AE_L32X2_IP(t1, X, 8); x1 = (t1);

            Y = (const ae_f32x2 *)(x_buf);
            AE_L32X2_IP(t0, Y, 8); y0 = (t0);

            q1 = AE_MULF32R_LH(x0, y0);
            AE_MULAF32R_LH(q1, y0, x1);

            q2 = AE_MULF32R_HH(x1, y0);
            AE_MULAF32R_LL(q2, x1, y0);

            q0 = AE_MULF32R_HH(x0, y0);
            AE_MULAF32R_LL(q0, x0, y0);
            AE_L32X2_IP(t0, Y, 8); y0 = (t0);
            AE_MULAF32R_HH(q0, x1, y0);

            x0 = AE_ROUND32X2F48SASYM(q0, q1);
            AE_S32_L_IP(AE_INTSWAP(x0), castxcc(ae_int32, R), 4);
            if (n_iter>1) AE_S32_L_IP(x0, castxcc(ae_int32, R), 4);
            x0 = AE_ROUND32X2F48SASYM(q2, q2);
            if (n_iter>2) AE_S32_L_IP(AE_INTSWAP(x0), castxcc(ae_int32, R), 4);
        }
    }
}
