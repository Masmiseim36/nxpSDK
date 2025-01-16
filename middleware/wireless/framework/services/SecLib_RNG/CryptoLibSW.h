/*! *********************************************************************************
 * Copyright 2019, 2022 NXP
 * All rights reserved.
 *
 * \file
 *
 * This is the header file for the security module.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ********************************************************************************** */

#ifndef _CRYPTOLIBSW_H_
#define _CRYPTOLIBSW_H_

/************************************************************************************
*************************************************************************************
* Includes
*************************************************************************************
************************************************************************************/
#include <stdbool.h>

#include "EmbeddedTypes.h"
#include "SecLib_ecp256.h"

/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/* SHA1 SW implemetation */

void sw_sha1_initialize_output(uint32_t *sha1_state);
void sw_sha1_hash_n(uint8_t *msg_data, int32_t num_blks, uint32_t *sha1_state);

/* SHA256 SW implementation */

void sw_sha256_initialize_output(uint32_t *sha256_state);
void sw_sha256_hash_n(const uint8_t *msg_data, int32_t num_blks, uint32_t *sha256_state);

/* AES128 */
void    CCM_CreateA0(uint8_t *pData, const uint8_t *nonce, uint8_t size);
void    sw_Aes128(const uint8_t *pData, const uint8_t *pKey, uint8_t enc, uint8_t *pReturnData);
uint8_t sw_AES128_CCM(const uint8_t *pInput,
                      uint16_t       inputLen,
                      const uint8_t *pAuthData,
                      uint16_t       authDataLen,
                      const uint8_t *pNonce,
                      uint8_t        nonceSize,
                      const uint8_t *pKey,
                      uint8_t       *pOutput,
                      uint8_t       *pCbcMac,
                      uint8_t        macSize,
                      uint32_t       flags);

/* EC_P_256 */
extern const uint32_t gEcP256_MultiplicationBufferSize_c;

/* SW RNG */
uint32_t SecLib_set_rng_seed(uint32_t seed);
uint32_t SecLib_get_random(void);

/* SW ECDH */
secEcdhStatus_t Ecdh_GenerateNewKeys(ecdhPublicKey_t  *pOutPublicKey,
                                     ecdhPrivateKey_t *pOutPrivateKey,
                                     void             *pMultiplicationBuffer);

secEcdhStatus_t Ecdh_GenerateNewKeysSeg(computeDhKeyParam_t *pDhKeyData);

secEcdhStatus_t Ecdh_ComputeDhKeySeg(computeDhKeyParam_t *pDhKeyData);

void Ecdh_ComputeJacobiChunk(uint8_t index, uint8_t stepSize, computeDhKeyParam_t *pData);

void Ecdh_JacobiCompleteMult(computeDhKeyParam_t *pData);

/*! *********************************************************************************
 * \brief Compute ECDH Diffie Hellman shared secret
 *
 *  Internal API calling legacy unaccelerated lib (no DSP extension)
 *
 * \param[in]  pPrivateKey   Own ECP256R1 private key (scalar) to be multiplied with
 *                           peer's public key, results from previous key pair generation.
 *
 * \param[in] pPeerPublicKey  pointer on ECP256R1 public key received from peer.
 *
 * \param[out] pOutDhKey  pointer on buffer to receive DH shared secret.
 *
 * \param[in] pMultiplicationBuffer  pointer on work buffer for multiplication buffer.
 *
 * \return  gSecEcdhSuccess_c if success.
 * Note:    Does not retry if private key is not suitable
 *
 ********************************************************************************** */
secEcdhStatus_t Ecdh_ComputeDhKey(const ecdhPrivateKey_t *pPrivateKey,
                                  const ecdhPublicKey_t  *pPeerPublicKey,
                                  ecdhDhKey_t            *pOutDhKey,
                                  void                   *pMultiplicationBuffer);

/*! *********************************************************************************
 * \brief Compute ECDH Diffie Hellman shared secret
 *
 *  Internal API calling DSP extension based UltraFast lib
 *
 * \param[in]  pPrivateKey   Own ECP256R1 private key (scalar) to be multiplied with
 *                           peer's public key, results from previous key pair generation.
 *
 * \param[in] pPeerPublicKey  pointer on ECP256R1 public key received from peer.
 *
 * \param[out] pOutDhKey  pointer on buffer to receive DH shared secret.
 *
 * \param[in] pMultiplicationBuffer  pointer on work buffer for multiplication buffer.
 *
 * \return  gSecEcdhSuccess_c if success.
 * Note:    Does not retry if private key is not suitable
 *          Same as Ecdh_ComputeDhKey but uses DSP extension.
 *
 ********************************************************************************** */
secEcdhStatus_t Ecdh_ComputeDhKeyUltraFast(const ecdhPrivateKey_t *pPrivateKey,
                                           const ecdhPublicKey_t  *pPeerPublicKey,
                                           ecdhDhKey_t            *pOutDhKey,
                                           void                   *pMultiplicationBuffer);

/*! *********************************************************************************
 * \brief Duplicate ECP256 affine coordinate without endianess modification
 *
 *  Internal API
 *
 * \param[out] dest pointer on 32 byte storage in RAM location, preferably aligned
 * \param[in]  src pointer on point coordinate, eith X or Y, can be unaligned or read-only
 *
 ********************************************************************************** */
void ECP256_coordinate_copy(uint8_t dest[SEC_ECP256_COORDINATE_LEN], const uint8_t src[SEC_ECP256_COORDINATE_LEN]);

/*! *********************************************************************************
 * \brief Duplicate ECP256 affine coordinate swapping its endianess
 *
 *  Internal API *
 *
 * \param[out] dest pointer on 32 byte storage in RAM location, preferably aligned
 * \param[in]  src pointer on point coordinate, either X or Y
 *             must start with in[0] == 0x04
 *
 *  \remark   see ECP256_coordinate_copy
 *
 ********************************************************************************** */
void ECP256_coordinate_copy_and_change_endianness(uint8_t       dest_coordinate[SEC_ECP256_COORDINATE_LEN],
                                                  const uint8_t coordinate[SEC_ECP256_COORDINATE_LEN]);

/*! *********************************************************************************
 * \brief Inplace endianness change of a P256 coordinate
 *
 *  Internal API
 *
 * \param[in/out]  src pointer on point coordinate, either X or Y
 *             must be in RAM
 *
 *  \remark   see ECP256_coordinate_copy
 *
 ********************************************************************************** */
void ECP256_coordinate_change_endianness(uint8_t coordinate[SEC_ECP256_COORDINATE_LEN]);

/*! *********************************************************************************
 * \brief Duplicate ECP256 affine point  keeping its endianess
 *
 *  Internal API
 *
 * \param[out] dest_XY pointer on 64 byte storage in RAM location, preferably aligned
 * \param[in]  src pointer on point (might be unaligned and or in read-only)
 *
 *  \remark  ECP256_coordinate_copy applied to both coordinates
 *
 ********************************************************************************** */
void ECP256_PointCopy(uint8_t dest_XY[SEC_ECP256_POINT_LEN - 1], const uint8_t *src);

/*! *********************************************************************************
 * \brief Duplicate ECP256 affine point swapping its endianess
 *
 *  Internal API
 *
 * \param[out] dest_XY pointer on 64 byte storage in RAM location, preferably aligned
 * \param[in]  src pointer on point (might be unaligned and or in read-only)
 *
 *  \remark  ECP256_coordinate_copy_and_change_endianess applied to both coordinates
 *
 ********************************************************************************** */
void ECP256_PointCopy_and_change_endianness(uint8_t dest_XY[SEC_ECP256_POINT_LEN - 1], const uint8_t *src);

/*
 *  Internal API for Debug
 */
void ECP256_PointDisplay(const char *str, const uint8_t *point);

#ifdef __cplusplus
}
#endif

#endif /* _CRYPTOLIBSW_H_ */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
