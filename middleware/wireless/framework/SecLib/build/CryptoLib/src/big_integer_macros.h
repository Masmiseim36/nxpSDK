/*! *********************************************************************************
 * \defgroup ECDH
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
 * \file
 *
 * Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * NXP Confidential Proprietary
 *
 * No part of this document must be reproduced in any form - including copied,
 * transcribed, printed or by any electronic means - without specific written
 * permission from NXP.
 ********************************************************************************** */

#ifndef _BIG_INTEGER_MACROS_H
#define _BIG_INTEGER_MACROS_H

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
#define Bi_Add(x, y, result, length, retval)                                     \
    {                                                                            \
        uint64_t d_in_macro = 0;                                                 \
        int      v_in_macro = 0;                                                 \
        for (v_in_macro = 0; v_in_macro < (length); v_in_macro++)                \
        {                                                                        \
            d_in_macro += (uint64_t)(x)[v_in_macro] + (uint64_t)(y)[v_in_macro]; \
            (result)[v_in_macro] = d_in_macro;                                   \
            d_in_macro           = d_in_macro >> 32;                             \
        }                                                                        \
        (retval) = (uint32_t)d_in_macro;                                         \
    }

#define Bi_Sub(x, y, result, length, retval)                                                           \
    {                                                                                                  \
        uint64_t d_in_macro = 0;                                                                       \
        int      v_in_macro;                                                                           \
        for (v_in_macro = 0; v_in_macro < (length); v_in_macro++)                                      \
        {                                                                                              \
            d_in_macro           = (uint64_t)(x)[v_in_macro] - (uint64_t)(y)[v_in_macro] - d_in_macro; \
            (result)[v_in_macro] = d_in_macro & 0xFFFFFFFFu;                                           \
            d_in_macro           = d_in_macro >> 32;                                                   \
            d_in_macro &= 0x1;                                                                         \
        }                                                                                              \
        (retval) = (uint32_t)d_in_macro;                                                               \
    }

#define Bi_IsZero(A, retval)                               \
    {                                                      \
        uint8_t n_in_macro, r_in_macro = 0;                \
        for (n_in_macro = 0; n_in_macro < 8; n_in_macro++) \
        {                                                  \
            if ((A)[n_in_macro] == 0)                      \
                r_in_macro++;                              \
        }                                                  \
        (retval) = r_in_macro == 8;                        \
    }

#define Bi_IsOne(A, retval)                                \
    {                                                      \
        uint8_t n_in_macro;                                \
        for (n_in_macro = 1; n_in_macro < 8; n_in_macro++) \
            if ((A)[n_in_macro] != 0)                      \
                break;                                     \
        if ((n_in_macro == 8) && ((A)[0] == 1))            \
            (retval) = 1;                                  \
        else                                               \
            (retval) = 0;                                  \
    }

#define Bi_RightShift(A)                                              \
    {                                                                 \
        uint32_t n_in_macro, i_in_macro, nOld_in_macro = 0;           \
        for (i_in_macro = 8; i_in_macro--;)                           \
        {                                                             \
            n_in_macro    = A[i_in_macro] & 0x1;                      \
            A[i_in_macro] = A[i_in_macro] >> 1 | nOld_in_macro << 31; \
            nOld_in_macro = n_in_macro;                               \
        }                                                             \
    }

#define Bi_Copy(from, to, length)                               \
    {                                                           \
        int i_in_macro;                                         \
        for (i_in_macro = 0; i_in_macro < length; ++i_in_macro) \
        {                                                       \
            to[i_in_macro] = from[i_in_macro];                  \
        }                                                       \
    }

#define Bi_IsSame(A, B, length, retval)                         \
    {                                                           \
        int i_in_macro;                                         \
        (retval) = 1;                                           \
        for (i_in_macro = 0; i_in_macro < length; i_in_macro++) \
        {                                                       \
            if (A[i_in_macro] != B[i_in_macro])                 \
            {                                                   \
                (retval) = 0;                                   \
                break;                                          \
            }                                                   \
        }                                                       \
    }

#define Bi_SetZero(A, length)                                   \
    {                                                           \
        uint8_t i_in_macro;                                     \
        for (i_in_macro = 0; i_in_macro < length; ++i_in_macro) \
        {                                                       \
            A[i_in_macro] = 0;                                  \
        }                                                       \
    }

#define Bi_RightShiftBy(in, in_size, out, out_size, shift)                                          \
    {                                                                                               \
        int i_in_macro;                                                                             \
        for (i_in_macro = 0; i_in_macro < (in_size - shift) && i_in_macro < out_size; i_in_macro++) \
            out[i_in_macro] = in[i_in_macro + shift];                                               \
        for (/* reuse i_in_macro */; i_in_macro < out_size; i_in_macro++)                           \
            out[i_in_macro] = 0;                                                                    \
    }

#define Bi_Compare(A, B, length, retval)                             \
    {                                                                \
        int i_in_macro;                                              \
        (retval) = 0;                                                \
        for (i_in_macro = length - 1; i_in_macro >= 0; --i_in_macro) \
        {                                                            \
            if (A[i_in_macro] > B[i_in_macro])                       \
            {                                                        \
                (retval) = 1;                                        \
                break;                                               \
            }                                                        \
            if (A[i_in_macro] < B[i_in_macro])                       \
            {                                                        \
                (retval) = -1;                                       \
                break;                                               \
            }                                                        \
        }                                                            \
    }

#endif /* _BIG_INTEGER_MACROS_H */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
