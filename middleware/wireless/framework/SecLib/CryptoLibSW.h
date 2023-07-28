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
#include "EmbeddedTypes.h"
#include "SecLib_ecp256.h"

/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */

typedef enum ecdhStatus_tag
{
    gEcdhSuccess_c,
    gEcdhBadParameters_c,
    gEcdhOutOfMemory_c,
    gEcdhRngError_c,
    gEcdhInvalidState_c,
    gEcdhInvalidPublicKey_c
} ecdhStatus_t;

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
                      uint8_t *      pOutput,
                      uint8_t *      pCbcMac,
                      uint8_t        macSize,
                      uint32_t       flags);

/* EC_P_256 */
extern const uint32_t gEcP256_MultiplicationBufferSize_c;

bool_t EcP256_IsPointOnCurve(const uint32_t *X, const uint32_t *Y);

/* SW RNG */
uint32_t SecLib_set_rng_seed(uint32_t seed);
uint32_t SecLib_get_random(void);

/* SW ECDH */
ecdhStatus_t Ecdh_GenerateNewKeys(ecdhPublicKey_t * pOutPublicKey,
                                  ecdhPrivateKey_t *pOutPrivateKey,
                                  void *            pMultiplicationBuffer);

ecdhStatus_t Ecdh_GenerateNewKeysSeg(computeDhKeyParam_t *pDhKeyData);

ecdhStatus_t Ecdh_ComputeDhKey(ecdhPrivateKey_t *pPrivateKey,
                               ecdhPublicKey_t * pPeerPublicKey,
                               ecdhDhKey_t *     pOutDhKey,
                               void *            pMultiplicationBuffer);

ecdhStatus_t Ecdh_ComputeDhKeySeg(computeDhKeyParam_t *pDhKeyData);

void Ecdh_ComputeJacobiChunk(uint8_t index, uint8_t stepSize, computeDhKeyParam_t *pData);

void Ecdh_JacobiCompleteMult(computeDhKeyParam_t *pData);

#ifdef __cplusplus
}
#endif

#endif /* _CRYPTOLIBSW_H_ */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
