/*
 *  RFC 1521 base64 encoding/decoding
 *
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/* ************************************************************************** */
/* Includes                                                                   */
/* ************************************************************************** */

#include "sss_pkcs11_pal.h"

/* clang-format off */

static const unsigned char base64_dec_map[128] =
{
    127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
    127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
    127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
    127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
    127, 127, 127,  62, 127, 127, 127,  63,  52,  53,
     54,  55,  56,  57,  58,  59,  60,  61, 127, 127,
    127,  64, 127, 127, 127,   0,   1,   2,   3,   4,
      5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
     15,  16,  17,  18,  19,  20,  21,  22,  23,  24,
     25, 127, 127, 127, 127, 127, 127,  26,  27,  28,
     29,  30,  31,  32,  33,  34,  35,  36,  37,  38,
     39,  40,  41,  42,  43,  44,  45,  46,  47,  48,
     49,  50,  51, 127, 127, 127, 127, 127
};

/* clang-format on */

/* ************************************************************************** */
/* Public Functions                                                           */
/* ************************************************************************** */

/*
 * Constant flow conditional assignment to unsigned char
 */
static void base64_cond_assign_uchar(unsigned char *dest, const unsigned char *const src, unsigned char condition)
{
    /* MSVC has a warning about unary minus on unsigned integer types,
     * but this is well-defined and precisely what we want to do here. */
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4146)
#endif

    /* Generate bitmask from condition, mask will either be 0xFF or 0 */
    unsigned char mask = (condition | -condition);
    mask >>= 7;
    mask = -mask;

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

    *dest = ((*src) & mask) | ((*dest) & ~mask);
}

/*
 * Constant flow conditional assignment to uint_32
 */
static void base64_cond_assign_uint32(uint32_t *dest, const uint32_t src, uint32_t condition)
{
    /* MSVC has a warning about unary minus on unsigned integer types,
     * but this is well-defined and precisely what we want to do here. */
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4146)
#endif

    /* Generate bitmask from condition, mask will either be 0xFFFFFFFF or 0 */
    uint32_t mask = (condition | -condition);
    mask >>= 31;
    mask = -mask;

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

    *dest = (src & mask) | ((*dest) & ~mask);
}

/*
 * Constant flow check for equality
 */
static unsigned char base64_eq(size_t in_a, size_t in_b)
{
    size_t difference = in_a ^ in_b;

    /* MSVC has a warning about unary minus on unsigned integer types,
     * but this is well-defined and precisely what we want to do here. */
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4146)
#endif

    difference |= -difference;

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

    /* cope with the varying size of size_t per platform */
    difference >>= (sizeof(difference) * 8 - 1);

    return (unsigned char)(1 ^ difference);
}

/*
 * Constant flow lookup into table.
 */
static unsigned char base64_table_lookup(
    const unsigned char *const table, const size_t table_size, const size_t table_index)
{
    size_t i;
    unsigned char result = 0;

    for (i = 0; i < table_size; ++i) {
        base64_cond_assign_uchar(&result, &table[i], base64_eq(i, table_index));
    }

    return result;
}

/**
 * @brief Decode a base64-formatted buffer
 *
 * @param dst      destination buffer (can be NULL for checking size)
 * @param dlen     size of the destination buffer
 * @param olen     number of bytes written
 * @param src      source buffer
 * @param slen     amount of data to be decoded
 *
 * @returns        0 if successful, 1 or -1 if the input data is
 *                 not correct. *olen is always updated to reflect the amount
 *                 of data that has (or would have) been written.
 *
 * @note           Call this function with *dst = NULL or dlen = 0 to obtain
 *                 the required buffer size in *olen
 */

int base64_decode(unsigned char *dst, size_t dlen, size_t *olen, const unsigned char *src, size_t slen)
{
    size_t i, n;
    uint32_t j, x;
    unsigned char *p;
    unsigned char dec_map_lookup;

    /* First pass: check for validity and get output length */
    for (i = n = j = 0; i < slen; i++) {
        /* Skip spaces before checking for EOL */
        x = 0;
        while (i < slen && src[i] == ' ') {
            ++i;
            ++x;
        }

        /* Spaces at end of buffer are OK */
        if (i == slen)
            break;

        if ((slen - i) >= 2 && src[i] == '\r' && src[i + 1] == '\n')
            continue;

        if (src[i] == '\n')
            continue;

        /* Space inside a line is an error */
        if (x != 0)
            return (ERR_BASE64_INVALID_CHARACTER);

        if (src[i] == '=' && ++j > 2)
            return (ERR_BASE64_INVALID_CHARACTER);

        dec_map_lookup = base64_table_lookup(base64_dec_map, sizeof(base64_dec_map), src[i]);

        if (src[i] > 127 || dec_map_lookup == 127)
            return (ERR_BASE64_INVALID_CHARACTER);

        if (dec_map_lookup < 64 && j != 0)
            return (ERR_BASE64_INVALID_CHARACTER);

        n++;
    }

    if (n == 0) {
        *olen = 0;
        return (0);
    }

    /* The following expression is to calculate the following formula without
     * risk of integer overflow in n:
     *     n = ( ( n * 6 ) + 7 ) >> 3;
     */
    n = (6 * (n >> 3)) + ((6 * (n & 0x7) + 7) >> 3);
    n -= j;

    if (dst == NULL || dlen < n) {
        *olen = n;
        return (ERR_BASE64_BUFFER_TOO_SMALL);
    }

    for (j = 3, n = x = 0, p = dst; i > 0; i--, src++) {
        if (*src == '\r' || *src == '\n' || *src == ' ')
            continue;

        dec_map_lookup = base64_table_lookup(base64_dec_map, sizeof(base64_dec_map), *src);

        base64_cond_assign_uint32(&j, j - 1, base64_eq(dec_map_lookup, 64));
        x = (x << 6) | (dec_map_lookup & 0x3F);

        if (++n == 4) {
            n = 0;
            if (j > 0)
                *p++ = (unsigned char)(x >> 16);
            if (j > 1)
                *p++ = (unsigned char)(x >> 8);
            if (j > 2)
                *p++ = (unsigned char)(x);
        }
    }

    *olen = p - dst;

    return (0);
}