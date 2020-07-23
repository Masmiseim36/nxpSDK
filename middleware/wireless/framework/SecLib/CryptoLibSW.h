/*! *********************************************************************************
* Copyright 2019 NXP
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

/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */

typedef enum ecdhStatus_tag {
    gEcdhSuccess_c,
    gEcdhBadParameters_c,
    gEcdhOutOfMemory_c,
    gEcdhRngError_c,
    gEcdhInvalidState_c,
    gEcdhInvalidPublicKey_c
} ecdhStatus_t;

typedef union ecdhPrivateKey_tag {
    uint8_t  raw_8bit[32];
    uint32_t raw_32bit[8];
} ecdhPrivateKey_t;

typedef PACKED_UNION ecdhPoint_tag {
    uint8_t raw[64];
    PACKED_STRUCT {
        uint8_t x[32];
        uint8_t y[32];
    } components_8bit;
    PACKED_STRUCT {
        uint32_t x[8];
        uint32_t y[8];
    } components_32bit;
} ecdhPoint_t;

typedef ecdhPoint_t ecdhPublicKey_t;

typedef ecdhPoint_t ecdhDhKey_t;

typedef struct computeDhKeyParams_tag{
    ecdhPrivateKey_t privateKey;    /*!< Secret */
    ecdhPublicKey_t peerPublicKey;  /*!< Peer public key */
    ecdhPoint_t outPoint;           /*!< The resulting point */
    void *pWorkBuffer;              /*!< Pointer to the buffer used for computation */
    uint8_t procStep;               /*!< The step used for segmented computation */
    uint32_t aUserData[1];          /*!< Hold upper layer private data */
} computeDhKeyParam_t;

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/* SHA1 SW implemetation */

void sw_sha1_initialize_output (uint32_t *sha1_state);
void sw_sha1_hash_n (uint8_t *msg_data, int32_t num_blks, uint32_t *sha1_state);

/* SHA256 SW implementation */

void sw_sha256_initialize_output (uint32_t *sha256_state);
void sw_sha256_hash_n (uint8_t *msg_data, int32_t num_blks, uint32_t *sha256_state);

/* AES128 */
void CCM_CreateA0(uint8_t* pData, const uint8_t* nonce, uint8_t size);
void sw_Aes128(const uint8_t *pData, const uint8_t *pKey, uint8_t enc, uint8_t *pReturnData);
uint8_t sw_AES128_CCM(uint8_t* pInput,
                      uint16_t inputLen,
                      uint8_t* pAuthData,
                      uint16_t authDataLen,
                      uint8_t* pNonce,
                      uint8_t  nonceSize,
                      uint8_t* pKey,
                      uint8_t* pOutput,
                      uint8_t* pCbcMac,
                      uint8_t  macSize,
                      uint32_t flags);

/* EC_P_256 */
extern const uint32_t gEcP256_MultiplicationBufferSize_c;

/* SW RNG */
uint32_t SecLib_set_rng_seed( uint32_t seed );
uint32_t SecLib_get_random(void);

/* SW ECDH */
ecdhStatus_t Ecdh_GenerateNewKeys(ecdhPublicKey_t*    pOutPublicKey,
                                  ecdhPrivateKey_t*   pOutPrivateKey,
                                  void*               pMultiplicationBuffer);

ecdhStatus_t Ecdh_GenerateNewKeysSeg(computeDhKeyParam_t* pDhKeyData);

ecdhStatus_t Ecdh_ComputeDhKey(ecdhPrivateKey_t*   pPrivateKey,
                               ecdhPublicKey_t*    pPeerPublicKey,
                               ecdhDhKey_t*        pOutDhKey,
                               void*               pMultiplicationBuffer);

ecdhStatus_t Ecdh_ComputeDhKeySeg(computeDhKeyParam_t* pDhKeyData);

void Ecdh_ComputeJacobiChunk(uint8_t                index,
                             uint8_t                stepSize,
                             computeDhKeyParam_t    *pData);

void Ecdh_JacobiCompleteMult(computeDhKeyParam_t *pData);

#ifdef __cplusplus
}
#endif

#endif /* _CRYPTOLIBSW_H_ */

/*! *********************************************************************************
* @}
********************************************************************************** */
