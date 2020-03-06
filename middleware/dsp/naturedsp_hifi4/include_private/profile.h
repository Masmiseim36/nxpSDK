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
    Integrated profiler API
    IntegrIT, 2006-2018
*/

#ifndef __PROFILE_H
#define __PROFILE_H

#include <common.h>

/*
 * Before including this header, define the PROFILE_ENABLE preprocessor symbol as follows:
 * A) #define PROFILE_ENABLE or #define PROFILE_ENABLE <non zero> enables the profiler;
 * B) #define PROFILE_ENABLE 0 or PROFILE_ENABLE not defined at all disables the profiler.
 */

#if defined(COMPILER_MSVC)
#pragma warning(push)
#pragma warning(disable:4003)
#endif

#if defined(PROFILE_ENABLE)
#define TONUM(arg)  (1##arg)
#define TONUMX(arg) TONUM(arg)
#if TONUMX(PROFILE_ENABLE)==1 
#define _PROFILE_ENABLE 1
#else
#define _PROFILE_ENABLE (PROFILE_ENABLE)!=0
#endif
#undef TONUM
#undef TONUMX
#else
#define _PROFILE_ENABLE 0
#endif

#if defined(COMPILER_MSVC)
#pragma warning(pop)
#endif

#if _PROFILE_ENABLE
#include <stdio.h>

#if defined(COMPILER_XTENSA)
#include <xtensa/config/core.h>
#if XCHAL_HAVE_CCOUNT
#ifndef NO_XT_RSR_CCOUNT
#include <xtensa/tie/xt_timer.h>
#endif
#ifdef XT_RSR_CCOUNT
#define GETCLOCK() XT_RSR_CCOUNT()
#else
static unsigned long inline GETCLOCK(void)
{
    unsigned long r;
    __asm__ volatile ("rsr.ccount %0" : "=r" (r));
    return r;
}
#endif
#endif
#endif /* defined(COMPILER_XTENSA) */

#ifndef GETCLOCK
#include <time.h>
#define GETCLOCK() clock()
#endif

#define PROFILE_CREATE(item)  \
          unsigned long profile_##item##_trialCount = 0; \
          unsigned long profile_##item##_clkCount = 0; \
          unsigned long profile_##item##_clkStart = 0;
#define PROFILE_EXTERN(item)  \
          extern unsigned long profile_##item##_trialCount; \
          extern unsigned long profile_##item##_clkCount; \
          extern unsigned long profile_##item##_clkStart;
#define PROFILE_RESET(item) { \
          profile_##item##_trialCount = 0; profile_##item##_clkCount = 0; }
#define PROFILE_START(item) { \
          profile_##item##_clkStart = GETCLOCK(); }
#define PROFILE_STOP(item) { \
          unsigned long clk = GETCLOCK(); \
          profile_##item##_clkCount += clk - profile_##item##_clkStart; \
          profile_##item##_trialCount++; }
#define PROFILE_REPORT(item) { \
          (void)profile_##item##_clkStart; /* Avoid "defined but not used" warning. */ \
          printf("\nPROFILE_REPORT \t%-42s \tTrials: %5lu \tOverall Cycles: %7lu \tAverage Cycles per Trial: %7.0f \t", \
                 #item, profile_##item##_trialCount, profile_##item##_clkCount, \
                 profile_##item##_trialCount>0 ? (double)profile_##item##_clkCount/profile_##item##_trialCount : 0 ); \
          profile_##item##_trialCount = 0; profile_##item##_clkCount = 0; }

#else /* _PROFILE_ENABLE */

#define PROFILE_CREATE(item)
#define PROFILE_EXTERN(item)
#define PROFILE_RESET(item)
#define PROFILE_START(item)
#define PROFILE_STOP(item)
#define PROFILE_REPORT(item)

#endif

#endif /* __PROFILE_H */
