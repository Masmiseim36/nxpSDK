/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "general_utils.h"

/*!
 * @file    general_utils.c
 * @brief   This file provides general helper and data conversion functions.
 */

int32_t smallest_int(int32_t a, int32_t b)
{
    return ((a >= b) ? (b) : (a));
}

/* See .h file for function documentation */
uint32_t _transform_16_to_32(
    /* [in] 61bit sample's buffer*/
    void *src,

    /* [in] 16bit sample's buffer length */
    uint32_t src_len,

    /* [in] 32bit destination's buffer*/
    void *dst,

    /* [in] 32bit destination's buffer length */
    uint32_t dst_len)
{
    uint16_t *src_temp = (uint16_t *)src;
    uint32_t *dst_temp = (uint32_t *)dst;
    uint32_t num = 0, i = 0;

    if (src_len % 2)
    {
        /*length should align with 4 bytes*/
        src_len = (src_len * 2) / 2;
    }

    if (src_len < (dst_len >> 1))
        num = (src_len >> 1);
    else
        num = (dst_len >> 2);

    for (i = 0; i < num; i++)
    {
        *dst_temp = (uint32_t)(*src_temp << 16);
        dst_temp++;
        src_temp++;
    }

    return (num * 4);
}

/* See .h file for function documentation */
uint32_t _transform_24_to_32(
    /* [in] 24bit sample's buffer*/
    void *src,

    /* [in] 24bit sample's buffer length */
    uint32_t src_len,

    /* [in] 32bit sample's buffer*/
    void *dst,

    /* [in] 32bit sample's buffer length */
    uint32_t dst_len)
{
    uint8_t *src_temp   = (uint8_t *)src;
    uint32_t *dst_temp  = (uint32_t *)dst;
    uint32_t sample_num = 0;
    uint32_t i = 0, j = 0;

    if (src_len % 3)
    {
        /*length should align with 3 bytes*/
        src_len = (src_len * 3) / 3;
    }

    if (dst_len <= ((src_len / 3) * 4))
        sample_num = dst_len / 4;
    else
        sample_num = src_len / 3;

    j = 0;
    for (i = 0; i < sample_num; i++)
    {
        dst_temp[i] = (src_temp[j + 2] << 24) | (src_temp[j + 1] << 16) | (src_temp[j] << 8);
        j += 3;
    }

    return (sample_num * 4);
}

/* See .h file for function documentation */
uint32_t _transform_32_to_24(
    /* [in] 32bit sample's buffer*/
    void *src,

    /* [in] 32bit sample's buffer length */
    uint32_t src_len,

    /* [in] 24bit sample's buffer*/
    void *dst,

    /* [in] 24bit sample's buffer length */
    uint32_t dst_len)
{
    uint8_t *src_temp  = (uint8_t *)src, *fill_pos;
    uint32_t *dst_temp = (uint32_t *)dst;
    uint32_t b_len     = ((dst_len / 3) * 4);
    uint32_t i = 0, j = 0, k = 0;
    register uint32_t temp_value = 0;

    if (src_len % 4)
    {
        /*length should align with 4 bytes*/
        src_len = (src_len / 4) * 4;
    }

    if (src_len < b_len)
    {
        b_len = src_len;
    }

    for (i = 0; i < b_len; i++)
    {
        if (1 != ((i + 1) % 4))
        {
            temp_value |= (src_temp[i] << (8 * j));
            j++;
        }

        if (4 == j)
        {
            dst_temp[k] = temp_value;
            j           = 0;
            k++;
            temp_value = 0;
        }
    }

    if (j)
    {
        fill_pos = (uint8_t *)(uintptr_t)(dst_temp[k]);
        src_temp = (uint8_t *)src + b_len - j + 1;

        for (i = 0; i < j; i++)
            fill_pos[i] = src_temp[i];
    }

    return ((k * 4) + j);
}

/* See .h file for function documentation */
uint32_t _transform_32_to_16(
    /* [in] 32bit sample's buffer*/
    void *src,

    /* [in] 32bit sample's buffer length */
    uint32_t src_len,

    /* [in] 16bit sample's buffer*/
    void *dst,

    /* [in] 16bit sample's buffer length */
    uint32_t dst_len)
{
    uint32_t *src_temp = (uint32_t *)src;
    uint16_t *dst_temp = (uint16_t *)dst;
    uint32_t num = 0, i = 0;

    if (src_len % 4)
    {
        /*length should align with 4 bytes*/
        src_len = (src_len / 4) * 4;
    }

    if (dst_len < (src_len >> 1))
        num = (dst_len >> 1);
    else
        num = (src_len >> 2);

    for (i = 0; i < num; i++)
    {
        *dst_temp = (uint16_t)(*src_temp >> 16);
        dst_temp++;
        src_temp++;
    }

    return (num * 2);
}

/* See .h file for function documentation */
uint32_t data_copy_32bits(void *src, uint32_t src_len, void *dst, uint32_t dst_len)
{
    uint32_t *src_temp = (uint32_t *)src;
    uint32_t *dst_temp = (uint32_t *)dst;
    uint32_t length;
    uint32_t index = 0;

    if (src_len % 4)
    {
        /*length should align with 4 bytes*/
        src_len = (src_len / 4) * 4;
    }

    if (dst_len < src_len)
    {
        length = dst_len >> 2;
    }
    else
    {
        length = src_len >> 2;
    }

    while (length--)
    {
        dst_temp[index] = (uint32_t)src_temp[index];
        index++;
    }
    return (index << 2);
}

/* See .h file for function documentation */
uint32_t _interleave_16_to_32(
    /* [in] 16bit first sample's buffer*/
    void *src,

    /* [in] 16 bit sample's buffer length */
    uint32_t src_len,

    /* [in] 32bit destination's buffer*/
    void *dst,

    /* [in] 32bit sample's buffer length */
    uint32_t dst_len)
{
    uint16_t *src_temp = (uint16_t *)src;
    uint32_t *dst_temp = (uint32_t *)dst;
    uint32_t num = 0, i = 0;

    if (src_len % 2)
    {
        /*length should align with 4 bytes*/
        src_len = (src_len * 2) / 2;
    }

    /* for every 2 bytes in the source, 8-bytes in the destination will be
     * generated (4 to 1 ratio). Compare source vs Destination / 4*/
    if (src_len > (dst_len >> 2))
    {
        num = (dst_len >> 3);
    }
    else
    {
        num = src_len >> 1;
    }

    for (i = 0; i < num; i++)
    {
        *dst_temp = (uint32_t)(*src_temp << 16);
        dst_temp++;
        *dst_temp = (uint32_t)(*src_temp << 16);
        src_temp++;
        dst_temp++;
    }
    return (num * 8);
}

/* See .h file for function documentation */
uint32_t _interleave_24_to_32(
    /* [in] 24 bit first sample's buffer*/
    void *src,

    /* [in] 24bit sample's buffer length */
    uint32_t src_len,

    /* [in] 32bit sample's buffer*/
    void *dst,

    /* [in] 32bit sample's buffer length */
    uint32_t dst_len)
{
    uint8_t *src_temp   = (uint8_t *)src;
    uint32_t *dst_temp  = (uint32_t *)dst;
    uint32_t sample_num = 0;
    uint32_t i = 0, j = 0;

    if (src_len % 3)
    {
        /*length should align with 3 bytes*/
        src_len = (src_len * 3) / 3;
    }

    /* for every three bytes in the source, 8 are generated in the output */
    if (dst_len <= ((src_len / 3) * 8))
    {
        sample_num = dst_len / 8;
    }
    else
    {
        sample_num = src_len / 3;
    }

    j = 0;
    for (i = 0; i < sample_num; i++)
    {
        *dst_temp++ = (src_temp[j + 2] << 24) | (src_temp[j + 1] << 16) | (src_temp[j] << 8);
        *dst_temp++ = (src_temp[j + 2] << 24) | (src_temp[j + 1] << 16) | (src_temp[j] << 8);
        j += 3;
    }

    return (sample_num * 8);
}

uint32_t _interleave_32bits(
    /* [in] 32 bit first sample's buffer*/
    void *src,

    /* [in] Sources buffers length */
    uint32_t src_len,

    /* [in] 32bit destination buffer*/
    void *dst,

    /* [in] Destination buffer length */
    uint32_t dst_len)
{
    uint32_t *src_temp = (uint32_t *)src;
    uint32_t *dst_temp = (uint32_t *)dst;
    uint32_t length;
    uint32_t dst_index = 0;
    uint32_t src_index = 0;

    if (src_len % 4)
    {
        /*length should align with 4 bytes*/
        src_len = (src_len / 4) * 4;
    }

    if (dst_len < src_len)
    {
        length = dst_len >> 2;
    }
    else
    {
        length = src_len >> 2;
    }

    for (dst_index = 0; dst_index < length; dst_index++)
    {
        dst_temp[dst_index] = (uint32_t)src_temp[src_index];
        dst_index++;
        dst_temp[dst_index] = (uint32_t)src_temp[src_index];
        src_index++;
    }
    return (dst_index << 2);
}

/* EOF */
