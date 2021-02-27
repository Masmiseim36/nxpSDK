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
    tables for expf(x) approximation
*/
#ifndef EXPFTBL_H__
#define EXPFTBL_H__

#include "NatureDSP_types.h"
#include "common.h"

#define ANTILOG_ALG 1

/* 
   polynomial coefficients for 2^x in range 0...1

   derived by MATLAB code:
   order=6;
   x=(0:pow2(1,-16):1);
   y=2.^x;
   p=polyfit(x,y,6);
   p(order+1)=1;
   p(order)=p(order)-(sum(p)-2);
*/
#if ANTILOG_ALG ==0
externC const int32_t expftbl_Q30[8]; 
#endif
externC const union ufloat32uint32 expfminmax[2];  /* minimum and maximum arguments of expf() input */
externC const int32_t invln2_Q30; /* 1/ln(2), Q30 */
/* Matlab code for coefficients (24x24 multiplies)
order=2;
x=(0:pow2(1,-16):1);
y=2.^x;
p=polyfit(x,y,order);
p(order+1)=1;
p(order)=p(order)-(sum(p)-2);

orderq=6;
z=y-polyval(p,x);
q=polyfit(x,z,orderq);
plot(x,z-polyval(q,x)); grid on;

fprintf(1,'%d ',round(pow2(q,26)));
fprintf(1,'\n');
fprintf(1,'%d ',round(pow2(p,30)));
fprintf(1,'\n');
*/
#if ANTILOG_ALG ==1
externC const int32_t expftblpq[];
#endif
#endif /* EXPFTBL_H__ */
