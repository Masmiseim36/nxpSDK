/*! *********************************************************************************
 * \defgroup ECDH
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
 * \file
 *
 * Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * NXP Confidential Proprietary
 *
 * No part of this document must be reproduced in any form - including copied,
 * transcribed, printed or by any electronic means - without specific written
 * permission from NXP.
 ********************************************************************************** */

#if (!defined(mUseBigIntMacros_d) || (mUseBigIntMacros_d == 0))

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "big_integer.h"

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
uint32_t Bi_Add(const uint32_t *x, const uint32_t *y, uint32_t *result, uint8_t length)
{
    uint64_t d = 0; // carry
    uint8_t  v = 0;
    for (v = 0; v < length; v++)
    {
        d += (uint64_t)x[v] + (uint64_t)y[v];
        result[v] = (uint32_t)d;
        d         = d >> 32; // save carry
    }

    return (uint32_t)d;
}

uint32_t Bi_Sub(const uint32_t *x, const uint32_t *y, uint32_t *result, uint8_t length)
{
    uint64_t d = 0;
    uint8_t  v;

    for (v = 0; v < length; v++)
    {
        d         = (uint64_t)x[v] - (uint64_t)y[v] - d;
        result[v] = (uint32_t)(d & 0xFFFFFFFFULL);
        d         = d >> 32;
        d &= 0x1ULL;
    }
    return (uint32_t)d;
}

int8_t Bi_IsZero(const uint32_t *A)
{
    uint8_t n, r = 0;
    int8_t  ret = 0;

    for (n = 0; n < 8u; n++)
    {
        if (A[n] == 0u)
        {
            r++;
        }
    }

    if (r == 8u)
    {
        ret = 1;
    }
    return ret;
}

int8_t Bi_IsOne(const uint32_t *A)
{
    uint8_t n;
    int8_t  ret = 0;

    for (n = 1; n < 8u; n++)
    {
        if (A[n] != 0u)
        {
            break;
        }
    }

    if ((n == 8u) && (A[0] == 1u))
    {
        ret = 1;
    }

    return ret;
}

void Bi_RightShift(uint32_t *A)
{
    uint32_t n, i = 8u, nOld = 0;
    while (i-- > 0u)
    {
        n    = A[i] & 0x1u;
        A[i] = (A[i] >> 1) | (nOld << 31);
        nOld = n;
    }
}

void Bi_Copy(const uint32_t *from, uint32_t *to, uint8_t length)
{
    uint8_t i;
    for (i = 0; i < length; ++i)
    {
        to[i] = from[i];
    }
}

#ifdef _keepUnused
int8_t Bi_IsSame(const uint32_t *A, const uint32_t *B, uint8_t length)
{
    uint8_t i;
    int8_t  ret = 1;

    for (i = 0; i < length; i++)
    {
        if (A[i] != B[i])
        {
            ret = 0;
            break;
        }
    }
    return ret;
}
#endif

void Bi_SetZero(uint32_t *A, const int length)
{
    int i;
    for (i = 0; i < length; ++i)
    {
        A[i] = 0;
    }
}
#ifdef _keepUnused
void Bi_RightShiftBy(const uint32_t *in, uint8_t in_size, uint32_t *out, uint8_t out_size, uint8_t shift)
{
    uint32_t i;
    for (i = 0; i < ((uint32_t)in_size - (uint32_t)shift) && i < (uint32_t)out_size; i++)
    {
        out[i] = in[i + (uint32_t)shift];
    }
    for (/* reuse i */; i < out_size; i++)
    {
        out[i] = 0;
    }
}
#endif

int8_t Bi_Compare(const uint32_t *A, const uint32_t *B, uint8_t length)
{
    int    i;
    int8_t ret = 0;

    for (i = (int)length - 1; i >= 0; --i)
    {
        if (A[i] > B[i])
        {
            ret = 1;
            break;
        }
        if (A[i] < B[i])
        {
            ret = -1;
            break;
        }
    }
    return ret;
}

#endif /* !mUseBigIntMacros_d */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
