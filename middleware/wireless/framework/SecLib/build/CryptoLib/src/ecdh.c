/*! *********************************************************************************
 * \addtogroup ECDH
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
 * \file
 *
 * Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2018, 2022 NXP
 *
 * NXP Confidential Proprietary
 *
 * No part of this document must be reproduced in any form - including copied,
 * transcribed, printed or by any electronic means - without specific written
 * permission from NXP.
 ********************************************************************************** */

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "RNG_Interface.h"
#include "FunctionLib.h"

#include "big_integer.h"
#include "ec_p_256.h"
#include "CryptoLibSW.h"
/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/
#define mTesting_d 0

/*

Testing for different private keys:

k = 1
x = 6B17D1F2E12C4247F8BCE6E563A440F277037D812DEB33A0F4A13945D898C296
y = 4FE342E2FE1A7F9B8EE7EB4A7C0F9E162BCE33576B315ECECBB6406837BF51F5

k = 115792089210356248762697446949407573529996955224135760342422259061068512044368
= (hexa, little endian) 0xFC632550, 0xF3B9CAC2, 0xA7179E84, 0xBCE6FAAD, 0xFFFFFFFF,
    0xFFFFFFFF, 0x00000000, 0xFFFFFFFF

x = 6B17D1F2E12C4247F8BCE6E563A440F277037D812DEB33A0F4A13945D898C296
y = B01CBD1C01E58065711814B583F061E9D431CCA994CEA1313449BF97C840AE0A



*/

/************************************************************************************
*************************************************************************************
* Public memory definitions
*************************************************************************************
************************************************************************************/
const uint32_t gEcP256_MultiplicationBufferSize_c =
    (sizeof(multiplicationCtx_t) + 11 * KEY_UINT32_ARRAY_LENGTH * sizeof(uint32_t));

/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/
static ecdhStatus_t ecdh_generateValidPrivateKey(ecdhPrivateKey_t *pOutPrivateKey);

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
ecdhStatus_t Ecdh_GenerateNewKeys(ecdhPublicKey_t * pOutPublicKey,
                                  ecdhPrivateKey_t *pOutPrivateKey,
                                  void *            pMultiplicationBuffer)
{
    /* Generate private key */
    ecdhStatus_t status = gEcdhBadParameters_c;

    if (NULL != pMultiplicationBuffer)
    {
        status = ecdh_generateValidPrivateKey(pOutPrivateKey);
        if (gEcdhSuccess_c == status)
        {
            /* Compute public key */
            EcP256_PointMult(P256_g_point_x, P256_g_point_y, (uint32_t const *)pOutPrivateKey->raw_32bit,
                             (uint32_t *)pOutPublicKey->components_32bit.x,
                             (uint32_t *)pOutPublicKey->components_32bit.y, pMultiplicationBuffer);
        }
    }
    return status;
}

ecdhStatus_t Ecdh_GenerateNewKeysSeg(computeDhKeyParam_t *pDhKeyData)
{
    /* Generate private key */
    ecdhStatus_t status = ecdh_generateValidPrivateKey((ecdhPrivateKey_t *)&pDhKeyData->privateKey);
    if (status == gEcdhSuccess_c)
    {
        /* Populate public key from CryptoLib */
        FLib_MemCpy(pDhKeyData->peerPublicKey.components_32bit.x, P256_g_point_x,
                    sizeof(pDhKeyData->peerPublicKey.components_32bit.x));
        FLib_MemCpy(pDhKeyData->peerPublicKey.components_32bit.y, P256_g_point_y,
                    sizeof(pDhKeyData->peerPublicKey.components_32bit.y));

        /* Compute public key */
        EcP256_PointMultSeg(pDhKeyData);
    }

    return status;
}

ecdhStatus_t Ecdh_ComputeDhKey(ecdhPrivateKey_t *pPrivateKey,
                               ecdhPublicKey_t * pPeerPublicKey,
                               ecdhDhKey_t *     pOutDhKey,
                               void *            pMultiplicationBuffer)
{
    ecdhStatus_t status = gEcdhSuccess_c;

    if ((NULL == pMultiplicationBuffer) || (NULL == pPrivateKey) || (NULL == pPeerPublicKey) || (NULL == pOutDhKey) ||
        (Bi_IsZero((uint32_t const *)pPrivateKey->raw_32bit) != 0))
    {
        status = gEcdhBadParameters_c;
    }
    else if (!EcP256_IsPointOnCurve((const uint32_t *)pPeerPublicKey->components_32bit.x,
                                    (const uint32_t *)pPeerPublicKey->components_32bit.y))
    {
        status = gEcdhInvalidPublicKey_c;
    }
    else
    {
        EcP256_PointMult((uint32_t *)pPeerPublicKey->components_32bit.x, (uint32_t *)pPeerPublicKey->components_32bit.y,
                         (uint32_t const *)pPrivateKey->raw_32bit, (uint32_t *)pOutDhKey->components_32bit.x,
                         (uint32_t *)pOutDhKey->components_32bit.y, pMultiplicationBuffer);
    }

    return status;
}

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
ecdhStatus_t Ecdh_ComputeDhKeySeg(computeDhKeyParam_t *pDhKeyData)
{
    ecdhStatus_t status = gEcdhSuccess_c;

    /* Check if the work buffer is allocated and the private key is valid */
    if ((NULL == pDhKeyData->pWorkBuffer) || (Bi_IsZero((uint32_t const *)pDhKeyData->privateKey.raw_32bit) != 0))
    {
        status = gEcdhBadParameters_c;
    }
    /* Check if the public key is valid */
    else if (!EcP256_IsPointOnCurve((const uint32_t *)pDhKeyData->peerPublicKey.components_32bit.x,
                                    (const uint32_t *)pDhKeyData->peerPublicKey.components_32bit.y))
    {
        status = gEcdhInvalidPublicKey_c;
    }
    else
    {
        /* Start point multiplication */
        EcP256_PointMultSeg(pDhKeyData);
    }

    return status;
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/
static ecdhStatus_t ecdh_generateValidPrivateKey(ecdhPrivateKey_t *pOutPrivateKey)
{
    ecdhStatus_t status = gEcdhSuccess_c;
#if mTesting_d
    /* k = 1 */
    // pOutPrivateKey->raw_32bit[0] = 0x00000001;
    /* k = 115792089210356248762697446949407573529996955224135760342422259061068512044368 */
    pOutPrivateKey->raw_32bit[0] = 0xFC632550u;
    pOutPrivateKey->raw_32bit[1] = 0xF3B9CAC2u;
    pOutPrivateKey->raw_32bit[2] = 0xA7179E84u;
    pOutPrivateKey->raw_32bit[3] = 0xBCE6FAADu;
    pOutPrivateKey->raw_32bit[4] = 0xFFFFFFFFu;
    pOutPrivateKey->raw_32bit[5] = 0xFFFFFFFFu;
    pOutPrivateKey->raw_32bit[6] = 0x00000000u;
    pOutPrivateKey->raw_32bit[7] = 0xFFFFFFFFu;
#else
    if (pOutPrivateKey != NULL)
    {
        /* TODO: PRNG must be initialized outside this module */
        if (-1 == RNG_GetPseudoRandomNo((uint8_t *)pOutPrivateKey->raw_32bit, 16, NULL))
        {
            status = gEcdhRngError_c;
        }
        else
        {
            if (-1 == RNG_GetPseudoRandomNo((uint8_t *)pOutPrivateKey->raw_32bit + 16, 16, NULL))
            {
                status = gEcdhRngError_c;
            }
            else
            {
                /* Compute c mod (n-1) */
                while (Bi_Compare((uint32_t const *)pOutPrivateKey->raw_32bit, P256_orderMinusOne, 8) >= 0)
                {
                    (void)Bi_Sub((uint32_t const *)pOutPrivateKey->raw_32bit, P256_orderMinusOne,
                                 (uint32_t *)pOutPrivateKey->raw_32bit, 8);
                }
                /* d = c mod (n-1) + 1 */
                (void)Bi_Add((uint32_t const *)pOutPrivateKey->raw_32bit, P256_one,
                             (uint32_t *)pOutPrivateKey->raw_32bit, 8);
            }
        }
    }
    else
    {
        status = gEcdhBadParameters_c;
    }
#endif

    return status;
}

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
