/* ------------------------------------------------------------------------ */
/* Copyright (c) 2016 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (�Cadence    */
/* Libraries�) are the copyrighted works of Cadence Design Systems Inc.	    */
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
  NatureDSP Signal Processing Library. IIR part
    Lattice Block Real IIR, floating point
*/
#include "latrf_common.h"

#if XCHAL_HAVE_FUSION_FP
void latrf_processX(struct tag_latrf_t *latr, float32_t * restrict r,const float32_t*  x, int N  )
#if 0
{
        float32_t * delLine;
  const float32_t * coef;

  float32_t t0;
  float32_t scale;

  int M;
  int n, m;

  delLine = latr->delayLine;
  coef    = latr->coef;
  scale   = latr->scale;
  M       = latr->M;

  for ( n=0; n<N; n++ )
  {
    // Scale the input sample.
    t0 = x[n]*scale;
    t0 -= delLine[M-1]*coef[M-1];
    for ( m=M-2; m>=0; m-- )
    {
      t0 -= ( delLine[m] * coef[m] );
      // Update the (m+1)-th delay line element.
      delLine[m+1] = delLine[m] + ( t0 * coef[m] );
    }
    // Update the first delay line element with the resulting sample
    delLine[0] = t0;
    // Make the output sample.
    r[n] = t0;
  }
} // latrf_process()
#elif 0
{
        float32_t * restrict  delLine;
  const float32_t * restrict  coef;

  float32_t t0,t1;
  float32_t scale;

  int M;
  int n, m;

  delLine = latr->delayLine;
  coef    = latr->coef;
  scale   = latr->scale;
  M       = latr->M;

  for ( n=0; n<(N&~1); n+=2 )
  {
    // Scale the input sample.
    t0 = x[n+0]*scale;
    t1 = x[n+1]*scale;

    t0 -= delLine[M-1]*coef[M-1];
    t0 -= delLine[M-2]*coef[M-2];
    delLine[M-1] = delLine[M-2] + t0 * coef[M-2];
    t1 -= delLine[M-1]*coef[M-1];
    for ( m=M-3; m>=0; m-- )
    {
      t0 -= delLine[m] * coef[m];
      delLine[m+1] = delLine[m] + t0 * coef[m];
      t1 -= delLine[m+1] * coef[m+1];
      delLine[m+2] = delLine[m+1] + t1 * coef[m+1];
    }
    delLine[0] = t0;
    t1 -= delLine[0] * coef[0];
    delLine[1] = delLine[0] + t1 * coef[0];

    delLine[0] = t1;
    r[n+0] = t0;
    r[n+1] = t1;
  }
  if (N&1)
  {
    // Scale the input sample.
    t0 = x[n]*scale;
    t0 -= delLine[M-1]*coef[M-1];
    for ( m=M-2; m>=0; m-- )
    {
      t0 -= ( delLine[m] * coef[m] );
      // Update the (m+1)-th delay line element.
      delLine[m+1] = delLine[m] + ( t0 * coef[m] );
    }
    // Update the first delay line element with the resulting sample
    delLine[0] = t0;
    // Make the output sample.
    r[n] = t0;
  }

} // latrf_process()
#elif 1
{
        float32_t *restrict  delLine;
  const float32_t * restrict  coef;


  float32_t *restrict   rd_delLine;
  float32_t *restrict   wr_delLine;

  float32_t *   rd_delLine1;
  float32_t *   wr_delLine1;

  const float32_t * restrict  rd_coef;
  const float32_t * restrict  rd_coef1;

  float32_t t0,t1,t2,t3;
  
  float32_t c0,c1,c2,c3;
  float32_t d0,d1,d2,d3,d4;
  float32_t coff0,coff1,coff2;

  float32_t scale;

  int M;
  int n, m;

  delLine = latr->delayLine;
  coef    = latr->coef;
  scale   = latr->scale;
  M       = latr->M;

  coff0=coef[0];
  coff1=coef[1];
  coff2=coef[2];

  for ( n=0; n<(N&~3); n+=4 )
  {
	    // Scale the input sample.
	    t0 = x[n+0]*scale;
	    t1 = x[n+1]*scale;
	    t2 = x[n+2]*scale;
	    t3 = x[n+3]*scale;

	    rd_coef1=(float32_t *)&coef[M];
	    rd_delLine1= (float32_t *)&delLine[M];
	    wr_delLine1= (float32_t *)&delLine[M];

	    c0=XT_LSI(rd_coef1,-4);
	    c1=XT_LSI(rd_coef1,-8);
	    c2=XT_LSI(rd_coef1,-12);
	    c3=XT_LSI(rd_coef1,-16);

	    d0=XT_LSI(rd_delLine1,-4);
	    d1=XT_LSI(rd_delLine1,-8);
	    d2=XT_LSI(rd_delLine1,-12);
	    d3=XT_LSI(rd_delLine1,-16);

	    XT_MSUB_S(t0,d0,c0);
	    XT_MSUB_S(t0,d1,c1);
	    XT_MADD_S(d1,t0,c1);
	    d0=d1;
	    XT_SSI(d0,wr_delLine1,-4);
	    XT_MSUB_S(t0,d2,c2);
	    XT_MADD_S(d2,t0,c2);
	    d1=d2;
	    XT_SSI(d1,wr_delLine1,-8);
	    XT_MSUB_S(t0,d3,c3);
	    XT_MADD_S(d3,t0,c3);
	    d2=d3;
	    XT_SSI(d2,wr_delLine1,-12);


	    d0=XT_LSI(rd_delLine1,-4);
	    d1=XT_LSI(rd_delLine1,-8);
	    d2=XT_LSI(rd_delLine1,-12);

	    XT_MSUB_S(t1,d0,c0);
	    XT_MSUB_S(t1,d1,c1);
	    XT_MADD_S(d1,t1,c1);
	    d0=d1;
	    XT_SSI(d0,wr_delLine1,-4);
	    XT_MSUB_S(t1,d2,c2);
	    XT_MADD_S(d2,t1,c2);
	    d1=d2;
	    XT_SSI(d1,wr_delLine1,-8);

      XT_MSUB_S(t2,d0,c0);
      XT_MSUB_S(t2,d1,c1);
      XT_MADD_S(d1,t2,c1);
      XT_SSI(d1,wr_delLine1,-4);


	    d0=XT_LSI(rd_delLine1,-4);
	    XT_MSUB_S(t3,d0,c0);

	    rd_delLine=(float32_t *)&delLine[M-5];
	    wr_delLine=(float32_t *)&delLine[M-4];
	    rd_coef=(float32_t *)&coef[M-5];

	    XT_LSIP(c0,rd_coef,sizeof(float32_t));
	    XT_LSIP(c1,rd_coef,sizeof(float32_t));
	    XT_LSIP(c2,rd_coef,sizeof(float32_t));
	    XT_LSIP(c3,rd_coef,-16);


    for ( m=M-5; m>=0; m-- )
    {

      XT_LSIP(d0,rd_delLine,-4);
      XT_MSUB_S(t0,d0,c0);
      XT_MADD_S(d0,t0,c0);   
      d1=d0;
      XT_SSIP(d1,wr_delLine,sizeof(float32_t));
      XT_MSUB_S(t1,d1,c1);
      XT_MADD_S(d1,t1,c1);
      d2=d1;
      XT_SSIP(d2,wr_delLine,sizeof(float32_t));
      XT_MSUB_S(t2,d2,c2);
      XT_MADD_S(d2,t2,c2);
      d3=d2;
      XT_SSIP(d3,wr_delLine,sizeof(float32_t));
      XT_MSUB_S(t3,d3,c3);
      XT_MADD_S(d3,t3,c3);
      d4=d3;
      XT_SSIP(d4,wr_delLine,-16);
      c3=c2;
      c2=c1;
      c1=c0;
      XT_LSIP(c0,rd_coef,-4);
    }


    XT_SSX(t0,r,n*4);
    XT_MSUB_S(t1,t0,coff0);
    XT_SSX(t1,r,(n*4+4));
    XT_MADD_S(t0,t1,coff0);
    XT_MSUB_S(t2,t0,coff1);
    XT_MADD_S(t0,t2,coff1);
    XT_MSUB_S(t2,t1,coff0);
    XT_SSX(t2,r,(n*4+8));
    XT_MADD_S(t1,t2,coff0);
    XT_MSUB_S(t3,t0,coff2);
    XT_MADD_S(t0,t3,coff2);
    XT_SSX(t0,delLine,12);
    XT_MSUB_S(t3,t1,coff1);
    XT_MADD_S(t1,t3,coff1);
    XT_SSX(t1,delLine,8);
    XT_MSUB_S(t3,t2,coff0);
    XT_SSX(t3,r,(n*4+12));
    XT_MADD_S(t2,t3,coff0);
    XT_SSI(t2,delLine,4);
    XT_SSI(t3,delLine,0);

  }
  if (N&3)
  {
      for ( ; n<N; n++ )
      {
        // Scale the input sample.
        t0 = x[n]*scale;
        t0 -= delLine[M-1]*coef[M-1];
        for ( m=M-2; m>=0; m-- )
        {
          t0 -= ( delLine[m] * coef[m] );
          // Update the (m+1)-th delay line element.
          delLine[m+1] = delLine[m] + ( t0 * coef[m] );
        }
        // Update the first delay line element with the resulting sample
        delLine[0] = t0;
        // Make the output sample.
        r[n] = t0;
      }
  }
} // latrf_process()
#endif
#endif /* HAVE_VFPU */
