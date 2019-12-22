/**
 * @file axHostCrypto.h
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright 2016 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 *
 * @par Description
 * Host Crypto wrapper API for the A7-series security IC's
 *
 * @par HISTORY
 *
 */

#ifndef _AX_HOSTCRYPTO_H_
#define _AX_HOSTCRYPTO_H_

#include "sm_types.h"
#ifdef OPENSSL
#include <openssl/cmac.h>
#include <openssl/aes.h>
#endif

#ifdef MBEDTLS
#include <mbedtls/cmac.h>
#include <mbedtls/aes.h>
#endif

#ifdef TGT_A70CI
#include "a70ciHostCrypto.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Move this in cryptolibrary specific header files
#ifdef OPENSSL
typedef CMAC_CTX axHcCmacCtx_t;
#define HOST_ENCRYPT    (AES_ENCRYPT)  //!< Request encrypt operation
#define HOST_DECRYPT    (AES_DECRYPT)  //!< Request decrypt operation
#endif //OPENSSL

#ifdef MBEDTLS
typedef mbedtls_cipher_context_t axHcCmacCtx_t;
#define HOST_ENCRYPT    (1)  //!< Request encrypt operation
#define HOST_DECRYPT    (0)  //!< Request decrypt operation
#endif // MBEDTLS

#ifdef NO_SECURE_CHANNEL_SUPPORT
typedef struct {
    int dummy;
} axHcCmacCtx_t;
#endif // NO_SECURE_CHANNEL_SUPPORT

/* The error code defines follow the OpenSSL conventions */
#define HOST_CRYPTO_NOT_SUPPORTED -1  //!< Crypto engine does not support implementation
#define HOST_CRYPTO_ERROR          0  //!< Failure
#define HOST_CRYPTO_OK             1  //!< Success

/**
 * Calculate in one shot the SHA-1 hash value of the provided input \p msg.
 * SHA-1 (Secure Hash Algorithm) is a cryptographic hash function with a 160 bit output.
 * @param[in] msg     Input data whose hash value must be calculated
 * @param[in] msgLen  Length in byte of \p msg
 * @param[in,out] pHash IN: Buffer provided by caller (at least 20 byte); OUT: Calculated hash value
 *
 * @retval HOST_CRYPTO_OK
 * @retval HOST_CRYPTO_ERROR
 */
S32 HOST_SHA1_Get(const U8 *msg, U32 msgLen, U8 *pHash);

/**
 * Calculate in one shot the SHA256 hash value of the provided input \p msg.
 * SHA256 (Secure Hash Algorithm) is a cryptographic hash function with a 256 bit output.
 * @param[in] msg     Input data whose hash value must be calculated
 * @param[in] msgLen  Length in byte of \p msg
 * @param[in,out] pHash IN: Buffer provided by caller (at least 32 byte); OUT: Calculated hash value
 *
 * @retval HOST_CRYPTO_OK
 * @retval HOST_CRYPTO_ERROR
 */
S32 HOST_SHA256_Get(const U8 *msg, U32 msgLen, U8 *pHash);

#ifdef TGT_A70CU
#define SHA256_STATE_SIZE   32
#define PARTIAL_HASH_DEFAULT_NO_STATE
S32 HOST_SHA256_GetPartialHash(const U8 *msg, U32 msgLen, U8 *pHashState, U32 *pNumProcessedMsgBytes);
#endif

/**
 * Calculate in one shot the AES-128 based CMAC of \p mMsg.
 * @param[in] pKey           128 bit AES key
 * @param[in] keySizeInBytes Currently only a keysize of 16 is supported
 * @param[in] pMsg           input data to me mac'd
 * @param[in] msgLen         length of input data in bytes
 * @param[in,out] pMac       IN: Buffer of at least 16 byte; OUT: Calculated CMAC value (16 byte long)
 *
 * @retval ::ERR_MEMORY
 * @retval ::HOST_CRYPTO_OK
 * @retval ::HOST_CRYPTO_ERROR
 */
S32 HOST_CMAC_Get(const U8 *pKey, U8 keySizeInBytes, const U8 *pMsg, U32 msgLen, U8 *pMac);

/**
 * Initialize AES-128 based CMAC primitive
 * @param[out] ctx Pointer to context (double indirection)
 * @param[in] pKey           128 bit AES key
 * @param[in] keySizeInBytes Currently only 16 is supported
 *
 * @retval ::ERR_MEMORY
 * @retval ::HOST_CRYPTO_OK
 * @retval ::HOST_CRYPTO_ERROR
 */
S32 HOST_CMAC_Init(axHcCmacCtx_t **ctx, const U8 *pKey, U8 keySizeInBytes);

/**
 * Update/Append to data that must be mac'd
 * @param[in] ctx Pointer to context
 * @param[in] pMsg
 * @param[in] msgLen
 *
 * @retval ::HOST_CRYPTO_OK
 * @retval ::HOST_CRYPTO_ERROR
 */
S32 HOST_CMAC_Update(axHcCmacCtx_t *ctx, const U8 *pMsg, U32 msgLen);

/**
 * Retrieve the mac value and clean up/free the context
 * @param[in]  ctx Pointer to context
 * @param[out] pMac
 *
 * @retval ::HOST_CRYPTO_OK
 * @retval ::HOST_CRYPTO_ERROR
 */
S32 HOST_CMAC_Finish(axHcCmacCtx_t *ctx, U8 *pMac);

/**
 * Decrypt the provided ciphertext (assumed to be a block of 16 byte) using AES in ECB mode
 * @param[in,out] plainText
 * @param[in] cipherText
 * @param[in] key
 * @param[in] keyLen
 *
 * @retval ::HOST_CRYPTO_OK
 * @retval ::HOST_CRYPTO_ERROR
 */
S32 HOST_AES_ECB_DECRYPT(U8 *plainText, const U8 *cipherText, const U8 *key, U32 keyLen);

/**
 * Encrypt the provided plaintext (assumed to be a block of 16 byte) using AES in ECB mode
 * @param[in] plainText
 * @param[in,out] cipherText
 * @param[in] key
 * @param[in] keyLen
 *
 * @retval ::HOST_CRYPTO_OK
 * @retval ::HOST_CRYPTO_ERROR
 */
S32 HOST_AES_ECB_ENCRYPT(const U8 *plainText, U8 *cipherText, const U8 *key, U32 keyLen);

/**
 * Encrypt/Decrypt the provided plaintext \p pIn using AES-128 in CBC mode
 * @param[in] pKey
 * @param[in] keyLen  Currently only a keysize of 16 byte is supported
 * @param[in] pIv Initialization vector
 * @param[in] dir Either ::HOST_ENCRYPT or ::HOST_DECRYPT
 * @param[in] pIn input data
 * @param[in] inLen
 * @param[in,out] pOut
 *
 * @retval ::ERR_API_ERROR
 * @retval ::HOST_CRYPTO_OK
 * @retval ::HOST_CRYPTO_ERROR
 */
S32 HOST_AES_CBC_Process(const U8 *pKey, U32 keyLen, const U8 *pIv, U8 dir, const U8 *pIn, U32 inLen, U8 *pOut);

/**
 * Fill up the provided buffer \p pRandom with \p inLen byte of random data
 * @param[in]     inLen    number (in byte) of random data requested
 * @param[in,out] pRandom  IN: buffer of at least inLen size; OUT: retrieved random data
 *
 * @retval ::HOST_CRYPTO_NOT_SUPPORTED   Random not supported by Crypto Engine
 * @retval ::HOST_CRYPTO_OK
 * @retval ::HOST_CRYPTO_ERROR
 */
S32 HOST_GetRandom(U32 inLen, U8 * pRandom);

/**
* Encrypt/Decrypt the provided plaintext \p pIn using AES-128 in ecb mode
* @param[in] pKey
* @param[in] keyLen  Currently only a keysize of 16 byte is supported
* @param[in] pIv Initialization vector
* @param[in] dir Either ::HOST_ENCRYPT or ::HOST_DECRYPT
* @param[in] pIn input data
* @param[in] inLen
* @param[in,out] pOut
*
* @retval ::ERR_API_ERROR
* @retval ::HOST_CRYPTO_OK
* @retval ::HOST_CRYPTO_ERROR
*/
S32 HOST_AES_ECB_Process(const U8 *pKey, U32 keyLen, const U8 *pIv, U8 dir, const U8 *pIn, U32 inLen, U8 *pOut);

/**
* Encrypt/Decrypt the provided plaintext \p pIn using Triple-DES keu in cbc mode
* @param[in] pKey
* @param[in] keyLen  Currently only a keysize of 16 byte is supported
* @param[in] pIv Initialization vector
* @param[in] dir Either ::HOST_ENCRYPT or ::HOST_DECRYPT
* @param[in] pIn input data
* @param[in] inLen
* @param[in,out] pOut
*
* @retval ::ERR_API_ERROR
* @retval ::HOST_CRYPTO_OK
* @retval ::HOST_CRYPTO_ERROR
*/
S32 HOST_3DES_CBC_Process(const U8 *pKey, U32 keyLen, const U8 *pIv, U8 dir, const U8 *pIn, U32 inLen, U8 *pOut);
/**
* Encrypt/Decrypt the provided plaintext \p pIn using Triple-DES keu in ecb mode
* @param[in] pKey
* @param[in] keyLen  Currently only a keysize of 16 byte is supported
* @param[in] pIv Initialization vector
* @param[in] dir Either ::HOST_ENCRYPT or ::HOST_DECRYPT
* @param[in] pIn input data
* @param[in] inLen
* @param[in,out] pOut
*
* @retval ::ERR_API_ERROR
* @retval ::HOST_CRYPTO_OK
* @retval ::HOST_CRYPTO_ERROR
*/
S32 HOST_3DES_ECB_Process(const U8 *pKey, U32 keyLen, const U8 *pIv,
                          U8 dir, const U8 *pIn, U32 inLen, U8 *pOut);

/**
* Calculate in one shot the DES based CMAC of \p mMsg.
* @param[in] pKey           the DES key
* @param[in] keySizeInBytes the key size
* @param[in] pMsg           input data to me mac'd
* @param[in] msgLen         length of input data in bytes
* @param[in,out] pMac       IN: Buffer of at least 8 byte; OUT: Calculated CMAC value (8 byte long)
*
* @retval ::ERR_MEMORY
* @retval ::HOST_CRYPTO_OK
* @retval ::HOST_CRYPTO_ERROR
*/
S32 HOST_CMAC_Get_Des(const U8 *pKey, U8 keySizeInBytes, const U8 *pMsg, U32 msgLen, U8* pMac);

/**
* Initialize DES based CMAC primitive
* @param[out] ctx Pointer to context (double indirection)
* @param[in] pKey           the key
* @param[in] keySizeInBytes the key size
*
* @retval ::ERR_MEMORY
* @retval ::HOST_CRYPTO_OK
* @retval ::HOST_CRYPTO_ERROR
*/
S32 HOST_CMAC_Init_Des(axHcCmacCtx_t **ctx, const U8 *pKey,  U8 keySizeInBytes);

/**
* Wrap data with an AES wrapping key according to RFC 3394
* @param[in] wrapKey           the AES wrapping key
* @param[in] wrapKeyLen        the key size
* @param[in,out] out           buffer to contain the wrapped data
* @param[in,out] outLen        the wrapped data length
* @param[in] in                input data to me wrapped
* @param[in] inLen             the length of input data in bytes
*
* @retval ::ERR_MEMORY
* @retval ::HOST_CRYPTO_OK
* @retval ::HOST_CRYPTO_ERROR
*/
S32 HOST_AesWrapKeyRFC3394(const U8 *wrapKey, U16 wrapKeyLen, U8 *out, U16 *outLen, const U8 *in, U16 inLen);

/**
* Wrap data with an AES wrapping key according to RFC 3394, with a give IV
* @param[in] wrapKey           the AES wrapping key
* @param[in] wrapKeyLen        the key size
* @param[in] iv                the Initialization Vector
* @param[in,out] out           buffer to contain the wrapped data
* @param[in,out] outLen        the wrapped data length
* @param[in] in                input data to me wrapped
* @param[in] inLen             the length of input data in bytes
*
* @retval ::ERR_MEMORY
* @retval ::HOST_CRYPTO_OK
* @retval ::HOST_CRYPTO_ERROR
*/
S32 HOST_AesWrapKeyRFC3394WithIV(const U8 *wrapKey, U16 wrapKeyLen, U8* iv, U8 *out, U16 *outLen, const U8 *in, U16 inLen);

#ifdef __cplusplus
}
#endif
#endif
