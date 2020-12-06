/*
 * Copyright 2019-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef _SLN_ENCRYPT_H_
#define _SLN_ENCRYPT_H_

#include <stdint.h>
#include "fsl_common.h"
#include "fsl_dcp.h"

#define AES_BLOCK_SIZE 16

#define ALIGN16 __attribute__((aligned(16)))

typedef enum _sln_encrypt_status
{
    SLN_ENCRYPT_STATUS_OK           = 0,
    SLN_ENCRYPT_NULL_CTX            = -1,
    SLN_ENCRYPT_NULL_HANDLE         = -2,
    SLN_ENCRYPT_NULL_PARAM          = -3,
    SLN_ENCRYPT_INVALID_KEY         = -4,
    SLN_ENCRYPT_KEYSLOT_BUSY        = -5,
    SLN_ENCRYPT_DCP_ENCRYPT_ERROR_1 = -6,
    SLN_ENCRYPT_DCP_ENCRYPT_ERROR_2 = -7,
    SLN_ENCRYPT_DCP_DECRYPT_ERROR_1 = -8,
    SLN_ENCRYPT_DCP_DECRYPT_ERROR_2 = -9,
    SLN_ENCRYPT_DCP_CRC_ERROR       = -10,
    SLN_ENCRYPT_WRONG_IN_BUFSIZE    = -11,
    SLN_ENCRYPT_WRONG_OUT_BUFSIZE   = -12,
} sln_encrypt_status_t;

typedef struct _sln_encrypt_ctx
{
    ALIGN16 uint8_t key[AES_BLOCK_SIZE];
    ALIGN16 uint8_t iv[AES_BLOCK_SIZE];

    uint16_t keySize;

    dcp_handle_t handle;
} sln_encrypt_ctx_t;

/*!
 * @brief Return encrypted length from a given plaintext length.
 *
 * @param plainLen Plaintext length of data.
 *
 * @returns Encrypted data length in bytes.
 *
 */
static inline uint32_t SLN_Encrypt_Get_Crypt_Length(uint32_t plainLen)
{
    return (uint32_t)((plainLen / AES_BLOCK_SIZE) * AES_BLOCK_SIZE +
                      ((plainLen % AES_BLOCK_SIZE) ? AES_BLOCK_SIZE : 0));
}

/*!
 * @brief Initialize a DCP cryptography slot.
 *
 * @param *ctx	Pointer to a pre-allocated context
 *
 * @returns True in case the intialization was successful or False otherwise
 *
 */
int32_t SLN_Encrypt_Init_Slot(sln_encrypt_ctx_t *ctx);

/*!
 * @brief Deinitialize a DCP cryptography slot.
 *
 * @param *ctx  Pointer to a pre-allocated context
 *
 * @returns True in case the deintialization was successful or False otherwise
 *
 */
int32_t SLN_Encrypt_Deinit_Slot(sln_encrypt_ctx_t *ctx);

/*!
 * @brief Encrypts a plain message, with PKCS#7 padding
 *
 * @param ctx         Pointer to an encryption session context
 * @param in          Pointer to the plain (unencrypted) buffer
 * @param inSize      Size of the input buffer
 * @param out         Pointer to the cypher (encrypted) buffer.
 * @param outSize    The size of the output buffer. MUST be a multiple of 16-bytes.
 *
 * @returns 0 in case of success or a negative value in case of error
 */
int32_t SLN_Encrypt_AES_CBC_PKCS7(
    sln_encrypt_ctx_t *ctx, const uint8_t *in, size_t inSize, uint8_t *out, size_t outSize);

/*!
 * @brief Decrypts an encrypted message
 *
 * @param ctx         Pointer to an encryption session context
 * @param in          Pointer to the cypher (encrypted) buffer.
 * @param inSize      The size of the input buffer. MUST be a multiple of 16-bytes.
 * @param out         Pointer to the plain (unencrypted) buffer
 * @param outSize    Ouput pointer updated with plaintext length (decoded from PKCS#7 padding)
 *
 * @returns 0 in case of success or a negative value in case of error
 */
int32_t SLN_Decrypt_AES_CBC_PKCS7(
    sln_encrypt_ctx_t *ctx, const uint8_t *in, size_t inSize, uint8_t *out, size_t *outSize);

/*!
 * @brief Performs 32-bit CRC on input data
 *
 * @param ctx         Pointer to an encryption session context
 * @param in          Pointer to the input buffer
 * @param inSize      The size of the input buffer
 * @param out         Pointer to the output buffer (32-bit CRC)
 * @param outSize     Pointer to the size of the output buffer (Should always output 4 bytes)
 *
 * @returns 0 in case of success or a negative value in case of error
 */
int32_t SLN_Crc(sln_encrypt_ctx_t *ctx, const uint8_t *in, size_t inSize, uint32_t *out, size_t *outSize);

#endif /* _SLN_ENCRYPT_H_ */
