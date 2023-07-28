/*! *********************************************************************************
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * \file
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ********************************************************************************** */

#ifndef __SECLIB_ECP256_H__
#define __SECLIB_ECP256_H__

#include "SecLib.h"

/*! *********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */

/*! How many steps to use for the EC multiplication procedure */
#ifndef gSecLibEcStepsAtATime
#define gSecLibEcStepsAtATime 16U
#endif

/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */

typedef PACKED_UNION big_int256_tag
{
    uint8_t  raw_8bit[32];
    uint32_t raw_32bit[8];
}
big_int256_t;

typedef big_int256_t       ec_p256_coordinate;
typedef ec_p256_coordinate ecdhPrivateKey_t;

typedef union ecp256Point_t
{
    uint8_t  raw[64];
    uint32_t raw_32bit[16];
    PACKED_STRUCT
    {
        uint8_t x[32];
        uint8_t y[32];
    }
    components_8bit;
    PACKED_STRUCT
    {
        uint32_t x[8];
        uint32_t y[8];
    }
    components_32bit;
    PACKED_STRUCT
    {
        ec_p256_coordinate X;
        ec_p256_coordinate Y;
    }
    coord;
} ecp256Point_t;

typedef ecp256Point_t ecdhPublicKey_t;
typedef ecp256Point_t ecdhDhKey_t;
typedef ecp256Point_t ecdhPoint_t;

typedef void (*pfDhKeyCallback_t)(void *pParam);

typedef struct computeDhKeyParams_tag
{
    ecdhPrivateKey_t  privateKey;    /*!< Secret */
    ecdhPublicKey_t   peerPublicKey; /*!< Peer public key */
    ecdhPoint_t       outPoint;      /*!< The resulting point */
    void *            pWorkBuffer;   /*!< Pointer to the buffer used for computation */
    uint8_t           procStep;      /*!< The step used for segmented computation */
    uint8_t           result;        /*!< The asynchronous computation result */
    pfDhKeyCallback_t pFCallback;    /*!< The function to be called when asynchronous computation completes */
    void *pMsg; /*!< Pointer to a pre-allocated message to be injected in the SM state machine when asynchronous
                   computation completes */
    bool_t   keepInternalBlob; /*!< Keep internal object foir later usage? */
    uint32_t aUserData[1];     /*!< Hold upper layer private data */
} computeDhKeyParam_t;

typedef void (*secLibCallback_t)(computeDhKeyParam_t *pData);

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
 * \brief  This function performs initialization of the callback used to offload
 * elliptic curve multiplication.
 *
 * \param[in]  pfCallback Pointer to the function used to handle multiplication.
 *
 ********************************************************************************** */
void SecLib_SetExternalMultiplicationCb(secLibCallback_t pfCallback);

/************************************************************************************
 * \brief Generates a new ECDH P256 Private/Public key pair
 *
 * \return gSecSuccess_c or error
 *
 ************************************************************************************/
secResultType_t ECDH_P256_GenerateKeys(ecdhPublicKey_t *pOutPublicKey, ecdhPrivateKey_t *pOutPrivateKey);

/************************************************************************************
 * \brief Generates a new ECDH P256 Private/Public key pair. This function starts the
 *        ECDH generate procedure. The pDhKeyData must be allocated and kept
 *        allocated for the time of the computation procedure.
 *        When the result is gSecResultPending_c the memory should be kept until the
 *        last step.
 *        In any other result messages the data shall be cleared after this call.
 *
 * \param[in]  pDhKeyData Pointer to the structure holding information about the
 *                        multiplication
 *
 * \return gSecSuccess_c, gSecResultPending_c or error
 *
 ************************************************************************************/
secResultType_t ECDH_P256_GenerateKeysSeg(computeDhKeyParam_t *pDhKeyData);

/************************************************************************************
 * \brief Computes the Diffie-Hellman Key for an ECDH P256 key pair.
 *
 * \param[in]  pInPrivateKey    Pointer to the structure holding the private key
 * \param[in]  pInPeerPublicKey Pointer to the structure holding the public key
 * \param[out] pOutDhKey        Pointer to the structure holding the DH key computed
 * \param[in]  keepBlobDhKey    Specify if the shared key object is stored after this call. This
 *                              parameter is used only if gSecLibUseSecureSubSystem_d is enabled.
 *
 * \return gSecSuccess_c or error
 *
 ************************************************************************************/
secResultType_t ECDH_P256_ComputeDhKey(const ecdhPrivateKey_t *pInPrivateKey,
                                       const ecdhPublicKey_t * pInPeerPublicKey,
                                       ecdhDhKey_t *           pOutDhKey,
                                       const bool_t            keepBlobDhKey);

/************************************************************************************
 * \brief Computes the Diffie-Hellman Key for an ECDH P256 key pair. This function
 *        starts the ECDH key pair generate procedure. The pDhKeyData must be
 *        allocated and kept allocated for the time of the computation procedure.
 *        When the result is gSecResultPending_c the memory should be kept until the
 *        last step, when it can be safely freed.
 *        In any other result messages the data shall be cleared after this call.
 *
 * \param[in]  pDhKeyData Pointer to the structure holding information about the
 *                        multiplication
 *
 * \return gSecSuccess_c or error
 *
 ************************************************************************************/
secResultType_t ECDH_P256_ComputeDhKeySeg(computeDhKeyParam_t *pDhKeyData);

/************************************************************************************
 * \brief Free any data allocated in the input structure.
 *
 * \param[in]  pDhKeyData Pointer to the structure holding information about the
 *                        multiplication
 *
 * \return gSecSuccess_c or error
 *
 ************************************************************************************/
void ECDH_P256_FreeDhKeyData(computeDhKeyParam_t *pDhKeyData);

/************************************************************************************
 * \brief Computes the Edgelock to Edgelock key for an ECDH P256 key pair.
 *
 * \param[in]   pInPeerPublicKey     pointer to the public key.
 * \param[out]  pOutE2EKey           pointer where the E2E key object is stored
 *
 * \return gSecSuccess_c or error
 *
 ************************************************************************************/
secResultType_t ECDH_P256_ComputeA2BKey(const ecdhPublicKey_t *pInPeerPublicKey, ecdhDhKey_t *pOutE2EKey);

/************************************************************************************
 * \brief Free E2E key object
 *
 * \param[in]  pE2EKeyData   Pointer to the E2E key data to be freed.
 *
 * \return gSecSuccess_c or error
 *
 ************************************************************************************/
secResultType_t ECDH_P256_FreeE2EKeyData(ecdhDhKey_t *pE2EKeyData);

#endif /* __SECLIB_ECP256_H__ */