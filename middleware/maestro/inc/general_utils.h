/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef GENERAL_UTILS_H
#define GENERAL_UTILS_H

#include <stdint.h>

/*!
 * @file    general_utils.h
 * @brief   This file provides general helper functions.
 */

/*!
 * @ingroup utils
 * @brief   smallest_int
 * @details Function to determine the smallest of two integers.
 * @param a    first integer
 * @param b    second integer
 * @returns value of the smallest integer
 */
int32_t smallest_int(int32_t a, int32_t b);

/*!
 * @ingroup utils
 * @brief Extend 16 bit per sample to 32bit per sample
 * @param src [in] 16bit sample's buffer
 * @param src_len [in] Number of bytes in the input
 * @param dst [OUT] Output buffer where the 32-bit data is going to be written
 * @param dst_len [in] Maximum num of bytes that should be written on the output
 * @return uint32_t Number of bytes that were written in the destination buffer
 * @details This function uses src_len as the number byes and the
 * number of bytes in dst_len to determine how many
 * bytes will be copied in the output buffer dst. The return value will be the
 * number in bytes that was copied to the out buffer
 *
 */
uint32_t _transform_16_to_32(void *src, uint32_t src_len, void *dst, uint32_t dst_len);

/*!
 * @ingroup utils
 * @brief Extend 24 bit per sample to 32bit per sample
 * @param src [in] 24 bit sample's buffer
 * @param src_len [in] Number of bytes in the input
 * @param dst [OUT] Output buffer where the 32-bit data is going to be written
 * @param dst_len [in] Maximum num of bytes that should be written on the output
 * @return uint32_t Number of bytes that were written in the destination buffer
 * @details This function uses src_len as the number byes and the
 * number of bytes in dst_len to determine how many
 * bytes will be copied in the output buffer dst. The return value will be the
 * number in bytes that was copied to the out buffer
 */
uint32_t _transform_24_to_32(void *src, uint32_t src_len, void *dst, uint32_t dst_len);

/*!
 * @ingroup utils
 * @brief Compress 32 bit per sample to 24 bit per sample
 * @param src [in] 32 bit sample's buffer
 * @param src_len [in] Number of bytes in the input
 * @param dst [OUT] Output buffer where the 24-bit data is going to be written
 * @param dst_len [in] Maximum num of bytes that should be written on the output
 * @return uint32_t Number of bytes that were written in the destination buffer
 * @details This function uses src_len as the number byes and the
 * number of bytes in dst_len to determine how many
 * bytes will be copied in the output buffer dst. The return value will be the
 * number in bytes that was copied to the out buffer
 */
uint32_t _transform_32_to_24(void *src, uint32_t src_len, void *dst, uint32_t dst_len);

/*!
 * @ingroup utils
 * @brief Compress 32 bit per sample to 16 bit per sample
 * @param src [in] 32 bit sample's buffer
 * @param src_len [in] Number of bytes in the input
 * @param dst [OUT] Output buffer where the 16-bit data is going to be written
 * @param dst_len [in] Maximum num of bytes that should be written on the output
 * @return uint32_t Number of bytes that were written in the destination buffer
 * @details This function uses src_len as the number byes and the
 * number of bytes in dst_len to determine how many
 * bytes will be copied in the output buffer dst. The return value will be the
 * number in bytes that was copied to the out buffer
 */
uint32_t _transform_32_to_16(void *src, uint32_t src_len, void *dst, uint32_t dst_len);

/*!
 * @ingroup utils
 * @brief Equivalent to a memcpy used with src and dest size validation to
 * determine the amount of bytes to copy
 * @param src [in] 32 bit sample's buffer
 * @param src_len [in] Number of bytes in the input
 * @param dst [OUT] Output buffer where the 32-bit data is going to be written
 * @param dst_len [in] Maximum num of bytes that should be written on the output
 * @return uint32_t Number of bytes that were written in the destination buffer
 */
uint32_t data_copy_32bits(void *src, uint32_t src_len, void *dst, uint32_t dst_len);

/*!
 * @ingroup utils
 * @brief Used to interleave samples from two input buffers on an output buffer
 * @param src [in] 16 bit sample's buffer
 * @param src_len [in] Number of bytes in the input.
 * @param dst [OUT] Output buffer where the 32-bit data is going to be written
 * @param dst_len [in] Maximum num of bytes that should be written on the output
 * @return uint32_t Number of bytes that were written in the destination buffer
 */
uint32_t _interleave_16_to_32(void *src, uint32_t src_len, void *dst, uint32_t dst_len);

/*!
 * @ingroup utils
 * @brief Used to interleave samples from two input buffers on an output buffer
 * @param src [in] 24 bit sample's buffer
 * @param src_len [in] Number of bytes in the input.
 * @param dst [OUT] Output buffer where the 32-bit data is going to be written
 * @param dst_len [in] Maximum num of bytes that should be written on the output
 * @return uint32_t Number of bytes that were written in the destination buffer
 */
uint32_t _interleave_24_to_32(void *src, uint32_t src_len, void *dst, uint32_t dst_len);

/*!
 * @ingroup utils
 * @brief Used to interleave samples from two input buffers on an output buffer
 * @param src [in] 32 bit sample's buffer
 * @param src_len [in] Number of bytes in the input. Applies to both input buffers
 * @param dst [OUT] Output buffer where the 32-bit data is going to be written
 * @param dst_len [in] Maximum num of bytes that should be written on the output
 * @return uint32_t Number of bytes that were written in the destination buffer
 */
uint32_t _interleave_32bits(void *src, uint32_t src_len, void *dst, uint32_t dst_len);

#endif
