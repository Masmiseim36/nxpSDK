
/**
 *  \file lc3_common_internal.h
 *
 *  \brief  This file defines common internal definations
 */

/*
 *  Copyright (C) 2021. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_LC3_COMMON_INTERNAL_
#define _H_LC3_COMMON_INTERNAL_

/* --------------------------------------------- Header File Inclusion */
#include "LC3_api.h"
#include "intrinsics.h"

/* --------------------------------------------- Global Definitions */
#define MAX_32 (INT32)0x7FFFFFFFL
#define MIN_32 (INT32)0x80000000L

#define MAX_16 (INT16)0x7FFF
#define MIN_16 (INT16)0x8000

/* TODO check 4 or 16 byte */
#if defined(__GNUC__)
#define DATA_ALIGN __attribute__((aligned(16)))
#elif defined(__CC_ARM)
#define DATA_ALIGN __align(16)
#elif defined(_MSC_VER)
#define DATA_ALIGN __declspec(align(16))
#else
#define DATA_ALIGN
#endif

#define PROPERTY_OPTIMIZE_NONE

/* TODO disable for if any max(), min() intrinsics */
#ifdef min
#undef min
#endif /* min */
#define min(a,b) (((a) < (b)) ? (a) : (b))   /*avoid error in mac*/

#ifdef max
#undef max
#endif /* max */
#define max(a,b) (((a) > (b)) ? (a) : (b))   /*avoid error in mac*/

/* #define LC3_BUFFER_SET(ptr, offset) (void *)((UINT8*)(ptr) + (offset)) */
#define LC3_BUFFER_SET(ptr, offset)  ((void *)( (UINT8*)(ptr) + ((((offset)+3)>>2)<<2)))

#define LC3_ASSERT(test)  ((void)0)

static __inline void lc3_memcpy(void *dst, const void *src, size_t n)
{
    memcpy(dst, src, n);
}

static __inline void lc3_memmove(void *dst, const void *src, size_t n)
{
    memmove(dst, src, n);
}

static __inline void lc3_memset(void *dst, INT32 val, size_t n)
{
    memset(dst, val, n);
}


/* Depends on intrinsics */

/* Note : Replace these if abs(), max(),min() intrinsics available */
#define L_abs(x)             ((x)<0 ? -(x) : (x))
#define L_max(x, y)          ((x) >= (y) ? (x) : (y))
#define L_min(x, y)          ((x) <= (y) ? (x) : (y))


/* gets exact 32 bit top from 47bit result of smull [32x16] */
static __inline  INT32 __SMULL_32_16(INT32 x, INT32 y)
{
    INT32  mh;
    INT32  ml;

    asm volatile ("SMULL %0 ,%1, %2, %3"
    : "=r" (ml), "=r" (mh)
    : "r" (x), "r" (y));

    mh = ( (mh)<<17) | (((UINT32)ml)>>15 );

    return mh;
}

/* gets exact 32 bit top from 63bit result of smull [32x32] */
static __inline  INT32 __SMULL_32_32(INT32 x, INT32 y)
{
    INT32  mh;
    INT32  ml;

    asm volatile ("SMULL %0 ,%1, %2, %3"
    : "=r" (ml), "=r" (mh)
    : "r" (x), "r" (y));

    mh = __QDADD(((UINT32)ml>>31), mh);

    return mh;
}

static __inline  INT32 __MLA(INT32 x, INT32 y, INT32 a)
{
    INT32  sum;

    asm volatile ("MLA %0, %1, %2, %3"
    : "=r" (sum)
    : "r" (x), "r" (y), "r" (a));

    return sum;
}

static __inline  INT32 __SMULWB_h(INT32 x, INT32 y)
{
    INT32  mh;

    asm volatile ("SMULWB %0 ,%1, %2 "
    : "=r" (mh)
    : "r" (x), "r" (y));

    return mh;
}

static __inline  INT32 __SMLAWB_h(INT32 x, INT32 y, INT32 z)
{
    INT32  mh;

    asm volatile ("SMLAWB %0 ,%1, %2, %3 "
    : "=r" (mh)
    : "r" (x), "r" (y), "r" (z));

    return mh;
}

static __inline INT16 norm_s(INT16 var1)
{
    return __CLZ(var1 ^ ((INT32)var1 << 17)) & 15;
}

static __inline  INT16 norm_l(INT32 L_var1)
{
    return __CLZ(L_var1 ^ (L_var1 << 1)) & 31;
}

static __inline  INT16 norm_ul(INT32 L_var1)
{
    return __CLZ(L_var1) & 31;
}


/* A list of functions that need saturation can be find below marked with an _sat */


static __inline INT32 L_shl_sat(INT32 L_var1, INT16 var2)
{
    INT32 L_var_out = 0;

    if (var2 <= 0)
    {
        if (var2 < -32)
        {
            var2 = -32;
        }
        var2 = -var2;
        if (var2 >= 31)
        {
            L_var_out = (L_var1 < 0L) ? -1 : 0;
        }
        else
        {
            if (L_var1 < 0)
            {
                L_var_out = ~((~L_var1) >> var2);
            }
            else
            {
                L_var_out = L_var1 >> var2;
            }
        }
    }
    else
    {
        if (var2 > norm_l(L_var1))  /* L_var1==0 ?? ==> L_var_out = 0 */
        {
            if (L_var1 > 0)         /* +ve */
            {
                L_var_out = (MAX_32);
            }
            else if (L_var1 < 0)    /* -ve */
            {
                L_var_out = (MIN_32);
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }
        }
        else
        {
            L_var_out = L_var1 << var2;
        }
    }
    return L_var_out;
}

static __inline INT32 L_shr_sat(INT32 L_var1, INT16 var2)
{
    INT32 L_var_out =0;

    if (var2 < 0)
    {
        if (var2 < -32)
        {    var2 = -32;  }

        var2 = -var2;


        if (var2 > norm_l(L_var1))  /* L_var1==0 ?? ==> L_var_out = 0 */
        {
            if (L_var1 > 0)         /* +ve */
            {
                L_var_out = (MAX_32);
            }
            else if (L_var1 < 0)    /* -ve */
            {
                L_var_out = (MIN_32);
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }
        }
        else
        {
            L_var_out = L_var1 << var2;
        }
    }
    else
    {
        if (var2 >= 31)
        {
            L_var_out = (L_var1 < 0L) ? -1 : 0;
        }
        else
        {
            if (L_var1 < 0)
            {
                L_var_out = ~((~L_var1) >> var2);
            }
            else
            {
                L_var_out = L_var1 >> var2;
            }
        }
    }
    return L_var_out;
}

static __inline INT16 div_s(INT16 var1, INT16 var2)
{
    INT32 quot;
    LC3_ASSERT(var2 > 0);
    LC3_ASSERT(var1 >= 0);
    LC3_ASSERT(var1 <= var2);
    quot = 0x8000 * var1;

    quot /= var2;
    if (quot > 32767)
    {
        return 32767;
    }
    else
    {
        return (INT16)quot;
    }
}

#define div_l_unroll_1x( var_out,L_num,L_den)         \
        var_out = (var_out << 1);                     \
        L_num = (L_num << 1);                         \
        if (L_num >= L_den)                           \
        {   L_num = __QSUB(L_num,L_den);              \
            var_out = __QADD16(var_out,1);            \
        }                                             \

static __inline  INT16 div_l(INT32  L_num, INT16 den)
{
    INT32   var_out = (INT16)0;
    INT32   L_den;

    if (den == (INT16)0)
    {
        abort();
    }

    if ((L_num < (INT32)0) || (den < (INT16)0))
    {
        abort();
    }

    L_den = (den << 16);

    if (L_num >= L_den)
    {
        return MAX_16;
    }
    else
    {
        L_num = (L_num >> 1);
        L_den = (L_den >> 1);

        div_l_unroll_1x(var_out,L_num,L_den);
        div_l_unroll_1x(var_out,L_num,L_den);
        div_l_unroll_1x(var_out,L_num,L_den);
        div_l_unroll_1x(var_out,L_num,L_den);
        div_l_unroll_1x(var_out,L_num,L_den);
        div_l_unroll_1x(var_out,L_num,L_den);
        div_l_unroll_1x(var_out,L_num,L_den);
        div_l_unroll_1x(var_out,L_num,L_den);
        div_l_unroll_1x(var_out,L_num,L_den);
        div_l_unroll_1x(var_out,L_num,L_den);
        div_l_unroll_1x(var_out,L_num,L_den);
        div_l_unroll_1x(var_out,L_num,L_den);
        div_l_unroll_1x(var_out,L_num,L_den);
        div_l_unroll_1x(var_out,L_num,L_den);
        div_l_unroll_1x(var_out,L_num,L_den);

        return (INT16)(var_out);
    }
}

#endif /* _H_LC3_COMMON_INTERNAL_ */
