
/*! *********************************************************************************
 * Copyright 2025 NXP
 *
 * \file
 *
 * This is the source file for the security module used by the connectivity stacks. The Security
 *    Module SecLib provides an abstraction from the Hardware to the upper layer.
 *    In this file, a wrapper to PSA API component is implemented.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */

#include <stdint.h>
#include "EmbeddedTypes.h"
#include "fwk_config.h"
#include "SecLib.h"
#include "psa/crypto.h"
#include "psa/crypto_types.h"
#include "psa/crypto_values.h"
#include "psa/crypto_extra.h"
#include "fsl_component_mem_manager.h"
#include "p256-m.h"
#include "FunctionLib.h"
#include "fwk_platform_crypto.h"
#include "SecLib_ecp256.h"
#include "CryptoLibSW.h"

/*! *********************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
********************************************************************************** */
#define KEY_ID_BLE0 0x426c6530

#define RAISE_ERROR(st, expected)                                \
    if ((st) != (expected))                                      \
    {                                                            \
        if (PRINTF("\tassertion failed at %s:%d - "              \
                   "actual:-%d expected:-%d\r\n",                \
                   __FILE__, __LINE__, -(st), -(expected)) > -1) \
        {                                                        \
            break;                                               \
        }                                                        \
    }

/*! *********************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
********************************************************************************** */
#define ECP256_COORDINATE_BITLEN 256u
#define ECP256_COORDINATE_LEN    (ECP256_COORDINATE_BITLEN >> 3)
#define ECP256_COORDINATE_WLEN   ((ECP256_COORDINATE_LEN) / 4U)

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
typedef struct psa_ecp256_context_t
{
    big_int256_t PrivateKey[ECP256_COORDINATE_WLEN];        /*!< The private key : RNG output */
    big_int256_t OwnPublicKey[2U * ECP256_COORDINATE_WLEN]; /*! Own Public computed from PrivateKey */
    uint32_t     keyId;
    psa_key_id_t OwnKey;                                    /*! Own Key object reference */
} psa_ecp256_context_t;

static psa_ecp256_context_t  psa_g_ECP_KeyPair;
static psa_ecp256_context_t *psa_pECPKeyPair = ((void *)0);

/*! *********************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
********************************************************************************** */

static bool ECP256_LePointValid(const ecp256Point_t *P)
{
#if gSecLibUseDspExtension_d
    ecp256Point_t tmp;
    ECP256_PointCopy_and_change_endianness(tmp.raw, P->raw);
    return ECP256_PointValid(&tmp);
#else
    extern bool_t EcP256_IsPointOnCurve(const uint32_t *X, const uint32_t *Y);
    return EcP256_IsPointOnCurve((const uint32_t *)&P->components_32bit.x[0],
                                 (const uint32_t *)&P->components_32bit.y[0]);
#endif
}

/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
 * \brief  This function performs initialization of the cryptographic HW acceleration.
 *
 ********************************************************************************** */
void SecLib_Init(void)
{
    psa_status_t status;
    do
    {
        /* initialize psa crypto*/
        status = psa_crypto_init();
        RAISE_ERROR(status, PSA_SUCCESS)
    } while (false);
}

void SecLib_ReInit(void)
{
    /* Initialize cryptographic hardware.*/
    (void)PLATFORM_ResetCrypto();
}

/*! *********************************************************************************
 * \brief  This function will allow reinitizialize the cryptographic HW acceleration
 * next time we need it, typically after lowpower mode.
 *
 ********************************************************************************** */
void SecLib_DeInit(void)
{
    /* Deinitialize cryptographic hardware.*/
    (void)PLATFORM_TerminateCrypto();
}

/* to see if implementation needed */
void SecLib_SetExternalMultiplicationCb(secLibCallback_t pfCallback)
{
    return;
}

/*! *********************************************************************************
 * \brief  This function performs all SHA256 steps on multiple bytes: initialize,
 *         update, finish, and update context data.
 *         The final hash value is stored at the provided output location.
 *
 * \param [in]       pData       Pointer to the input data
 * \param [in]       numBytes    Number of bytes to hash
 * \param [in,out]   pOutput     Pointer to the output location
 *
 ********************************************************************************** */
void SHA256_Hash(const uint8_t *pData, uint32_t numBytes, uint8_t *pOutput)
{
    const psa_algorithm_t alg        = PSA_ALG_SHA_256;
    size_t                hashLength = 0U; /* Initialize hash length to 0 */
    psa_status_t          status;

    do
    {
        /* SHA 256 computation */
        status = psa_hash_compute(alg, pData, numBytes, pOutput, SHA256_HASH_SIZE, &hashLength);
        RAISE_ERROR(status, PSA_SUCCESS);
    } while (false);
}

/*! *********************************************************************************
 * \brief  This function calculates XOR of individual byte pairs in two uint8_t arrays.
 *         pDst[i] := pDst[i] ^ pSrc[i] for i=0 to n-1
 *
 * \param[in]  pDst First byte array operand for XOR and destination byte array
 *
 * \param[in]  pSrc Second byte array operand for XOR
 *
 * \param[in]  n  Length of the byte array which will be XORed
 *
 ********************************************************************************** */
void SecLib_XorN(uint8_t *pDst, const uint8_t *pSrc, uint8_t n)
{
    while (n != 0U)
    {
        *pDst = *pDst ^ *pSrc;
        pDst  = pDst + 1;
        pSrc  = pSrc + 1;
        n--;
    }
}

/*! *********************************************************************************
 * \brief  This function performs AES-128 encryption on a 16-byte block.
 *
 * \param[in]  pInput Pointer to the location of the 16-byte plain text block.
 *
 * \param[in]  pKey Pointer to the location of the 128-bit key.
 *
 * \param[out]  pOutput Pointer to the location to store the 16-byte ciphered output.
 *
 * \pre All Input/Output pointers must refer to a memory address aligned to 4 bytes!
 *
 ********************************************************************************** */
void AES_128_Encrypt(const uint8_t *pInput, const uint8_t *pKey, uint8_t *pOutput)
{
    size_t                key_bits = AES_128_KEY_BYTE_LEN;
    const psa_algorithm_t alg      = PSA_ALG_ECB_NO_PADDING;
    psa_status_t          status;
    psa_key_attributes_t  attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_id_t          key        = 0U;
    size_t                output_len = 0U;

    /* key initialisation before import */
    psa_set_key_type(&attributes, PSA_KEY_TYPE_AES);
    psa_set_key_algorithm(&attributes, alg);
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_ENCRYPT);

    do
    {
        status = psa_import_key(&attributes, pKey, key_bits, &key); /* import the key in psa and get its id */
        RAISE_ERROR(status, PSA_SUCCESS);

        /* execute encryption with ECB NO PADDING */
        status = psa_cipher_encrypt(key, alg, pInput, AES_BLOCK_SIZE, pOutput, AES_BLOCK_SIZE, &output_len);
        RAISE_ERROR(status, PSA_SUCCESS);

        status = psa_destroy_key(key);
        RAISE_ERROR(status, PSA_SUCCESS);
    } while (false);
}

/*! *********************************************************************************
 * \brief  This function performs AES-128-ECB encryption on a message block.
 *         This function only accepts input lengths which are multiple
 *         of 16 bytes (AES 128 block size).
 *
 * \param[in]  pInput Pointer to the location of the input message.
 *
 * \param[in]  inputLen Input message length in bytes.
 *
 * \param[in]  pKey Pointer to the location of the 128-bit key.
 *
 * \param[out]  pOutput Pointer to the location to store the ciphered output.
 *
 * \pre All Input/Output pointers must refer to a memory address aligned to 4 bytes!
 *
 ********************************************************************************** */
void AES_128_ECB_Encrypt(const uint8_t *pInput, uint32_t inputLen, const uint8_t *pKey, uint8_t *pOutput)
{
    size_t                key_bits = AES_128_KEY_BYTE_LEN;
    const psa_algorithm_t alg      = PSA_ALG_ECB_NO_PADDING;
    psa_status_t          status;
    psa_key_attributes_t  attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_id_t          key        = 0U;
    size_t                output_len = 0U;

    /* key initialisation before import */
    psa_set_key_type(&attributes, PSA_KEY_TYPE_AES);
    psa_set_key_algorithm(&attributes, alg);
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_ENCRYPT);

    do
    {
        /* import pKey and get the address of the imported key */
        status = psa_import_key(&attributes, pKey, key_bits, &key);
        RAISE_ERROR(status, PSA_SUCCESS);

        /* execute the encryption with our key*/
        status = psa_cipher_encrypt(key, alg, pInput, inputLen, pOutput, inputLen, &output_len);
        RAISE_ERROR(status, PSA_SUCCESS);

        status = psa_destroy_key(key); /* destroy key after use */
        RAISE_ERROR(status, PSA_SUCCESS);
    } while (false);
}

/*! *********************************************************************************
 * \brief  This function performs AES-128 decryption on a 16-byte block.
 *
 * \param[in]  pInput Pointer to the location of the 16-byte plain text block.
 *
 * \param[in]  pKey Pointer to the location of the 128-bit key.
 *
 * \param[out]  pOutput Pointer to the location to store the 16-byte ciphered output.
 *
 * \pre All Input/Output pointers must refer to a memory address aligned to 4 bytes!
 *
 ********************************************************************************** */
void AES_128_Decrypt(const uint8_t *pInput, const uint8_t *pKey, uint8_t *pOutput)
{
    size_t                key_bits = AES_128_KEY_BYTE_LEN;
    const psa_algorithm_t alg      = PSA_ALG_ECB_NO_PADDING;
    psa_status_t          status;
    psa_key_attributes_t  attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_id_t          key        = 0U;
    size_t                output_len = 0U;

    /* key initialisation before import */
    psa_set_key_type(&attributes, PSA_KEY_TYPE_AES);
    psa_set_key_algorithm(&attributes, alg);
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_DECRYPT);

    do
    {
        status = psa_import_key(&attributes, pKey, key_bits, &key);
        RAISE_ERROR(status, PSA_SUCCESS);

        /* decrypt using ECB NO PADDING */
        status = psa_cipher_decrypt(key, alg, pInput, AES_BLOCK_SIZE, pOutput, AES_128_BLOCK_SIZE, &output_len);
        RAISE_ERROR(status, PSA_SUCCESS);

        /* destroy key after use */
        status = psa_destroy_key(key);
        RAISE_ERROR(status, PSA_SUCCESS);
    } while (false);
}

/*! *********************************************************************************
 * \brief  This function performs AES-128 decryption on a 16-byte block.
 *
 * \param[in]  pInput Pointer to the location of the 16-byte plain text block.
 *
 * \param[in]  inputLen Input message length in bytes.
 *
 * \param[in]  pKey Pointer to the location of the 128-bit key.
 *
 * \param[out]  pOutput Pointer to the location to store the 16-byte ciphered output.
 *
 * \pre All Input/Output pointers must refer to a memory address aligned to 4 bytes!
 *
 ********************************************************************************** */
#ifdef FSL_FEATURE_SOC_AES_HW
void AES_128_ECB_Decrypt(const uint8_t *pInput, uint32_t inputLen, const uint8_t *pKey, uint8_t *pOutput)
{
    size_t                key_bits = AES_128_KEY_BYTE_LEN;
    const psa_algorithm_t alg      = PSA_ALG_ECB_NO_PADDING;
    psa_status_t          status;
    psa_key_attributes_t  attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_id_t          key        = 0U;
    size_t                output_len = 0U;

    psa_set_key_type(&attributes, PSA_KEY_TYPE_AES);
    psa_set_key_algorithm(&attributes, alg);
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_DECRYPT);

    do
    {
        status = psa_import_key(&attributes, pKey, key_bits, &key);
        RAISE_ERROR(status, PSA_SUCCESS);

        status = psa_cipher_decrypt(key, alg, pInput, inputLen, pOutput, inputLen, &output_len);
        RAISE_ERROR(status, PSA_SUCCESS);

        status = psa_destroy_key(key);
        RAISE_ERROR(status, PSA_SUCCESS);
    } while (false);
}
#endif

/*! *********************************************************************************
 * \brief  This function performs AES-128-CMAC on a message block accepting input data
 *         which is in LSB first format and computing the authentication code
 *         starting from the end of the data.
 *
 * \param[in]  pInput Pointer to the location of the input message.
 *
 * \param[in]  inputLen Length of the input message in bytes.
 *             The input data must be provided LSB first.
 *
 * \param[in]  pKey Pointer to the location of the 128-bit key.
 *              The key must be provided MSB first.
 *
 * \param[out]  pOutput Pointer to the location to store the 16-byte authentication code.
 *              The code will be generated MSB first.
 *
 ********************************************************************************** */
void AES_128_CMAC_LsbFirstInput(const uint8_t *pInput, uint32_t inputLen, const uint8_t *pKey, uint8_t *pOutput)
{
    size_t                key_bits = AES_128_KEY_BYTE_LEN;
    const psa_algorithm_t alg      = PSA_ALG_CMAC; /* Set algorithm to cmac */
    psa_status_t          status;
    psa_key_attributes_t  attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_id_t          key        = 0U;
    size_t                output_len = 0U;

    /* key initialisation before import */
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_SIGN_MESSAGE);
    psa_set_key_algorithm(&attributes, alg);
    psa_set_key_type(&attributes, PSA_KEY_TYPE_AES);

    do
    {
        status = psa_import_key(&attributes, pKey, key_bits, &key);
        RAISE_ERROR(status, PSA_SUCCESS);

        psa_mac_operation_t operation = PSA_MAC_OPERATION_INIT; /* init cmac operation */

        status = psa_mac_sign_setup(&operation, key, alg);      /* setup function */
        RAISE_ERROR(status, PSA_SUCCESS);

        /* Walk the input buffer from the end to the start and reverse the blocks
         * before calling the CMAC update function. */
        uint8_t reversedBlock[AES_128_BLOCK_SIZE] = {0U};
        pInput += inputLen;
        do
        {
            uint32_t currentCmacInputBlkLen = 0U;
            if (inputLen < AES_128_BLOCK_SIZE)
            {
                /* If this is the first and single block it is legal for it to have an input length of 0
                 * in which case nothing will be copied in the reversed CMAC input buffer. */
                currentCmacInputBlkLen = inputLen;
            }
            else
            {
                currentCmacInputBlkLen = AES_128_BLOCK_SIZE;
            }
            pInput -= currentCmacInputBlkLen;
            inputLen -= currentCmacInputBlkLen;
            /* Copy the input block to the reversed CMAC input buffer */
            FLib_MemCpyReverseOrder(reversedBlock, pInput, currentCmacInputBlkLen);

            /* compute mac operation on a block */
            status = psa_mac_update(&operation, reversedBlock, currentCmacInputBlkLen);
            RAISE_ERROR(status, PSA_SUCCESS);

        } while (inputLen != 0U);

        size_t macLen =
            16U; /* value of PSA_MAC_LENGTH(PSA_KEY_TYPE_AES, key_bits, alg) but macro has multiple misras; */
        status = psa_mac_sign_finish(&operation, pOutput, macLen, &output_len);
        RAISE_ERROR(status, PSA_SUCCESS);

        status = psa_destroy_key(key); /* destroy key after use */
        RAISE_ERROR(status, PSA_SUCCESS);
    } while (false);
}

/*! *********************************************************************************
 * \brief  This function performs AES-128-CMAC on a message block.
 *
 * \param[in]  pInput Pointer to the location of the input message.
 *
 * \param[in]  inputLen Length of the input message in bytes. The input data must be provided MSB first.
 *
 * \param[in]  pKey Pointer to the location of the 128-bit key. The key must be provided MSB first.
 *
 * \param[out]  pOutput Pointer to the location to store the 16-byte authentication code. The code will be generated MSB
 *first.
 *
 * \remarks This is public open source code! Terms of use must be checked before use!
 *
 ********************************************************************************** */
void AES_128_CMAC(const uint8_t *pInput, const uint32_t inputLen, const uint8_t *pKey, uint8_t *pOutput)
{
    size_t                key_bits = AES_128_KEY_BYTE_LEN;
    const psa_algorithm_t alg      = PSA_ALG_CMAC; /* set algorithm to cmac */
    psa_status_t          status;
    psa_key_attributes_t  attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_id_t          key        = 0U;
    size_t                output_len = 0U;

    /* key initialisation before import */
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_SIGN_MESSAGE);
    psa_set_key_algorithm(&attributes, alg);
    psa_set_key_type(&attributes, PSA_KEY_TYPE_AES);

    do
    {
        status = psa_import_key(&attributes, pKey, key_bits, &key);
        RAISE_ERROR(status, PSA_SUCCESS);

        /* compute mac operation on pInput */
        status = psa_mac_compute(key, alg, pInput, inputLen, pOutput, AES_BLOCK_SIZE, &output_len);
        RAISE_ERROR(status, PSA_SUCCESS);

        status = psa_destroy_key(key); /* destroy key after use */
        RAISE_ERROR(status, PSA_SUCCESS);
    } while (false);
}

/*! *********************************************************************************
 * \brief  This function performs AES-128-CCM on a message block.
 *
 * \param[in]  pInput       Pointer to the location of the input message (plaintext or ciphertext).
 *
 * \param[in]  inputLen     Length of the input plaintext in bytes when encrypting.
 *                          Length of the input ciphertext without the MAC length when decrypting.
 *
 * \param[in]  pAuthData    Pointer to the additional authentication data.
 *
 * \param[in]  authDataLen  Length of additional authentication data.
 *
 * \param[in]  pNonce       Pointer to the Nonce.
 *
 * \param[in]  nonceSize    The size of the nonce (7-13).
 *
 * \param[in]  pKey         Pointer to the location of the 128-bit key.
 *
 * \param[out]  pOutput     Pointer to the location to store the plaintext data when decrypting.
 *                          Pointer to the location to store the ciphertext data when encrypting.
 *
 * \param[out]  pCbcMac     Pointer to the location to store the Message Authentication Code (MAC) when encrypting.
 *                          Pointer to the location where the received MAC can be found when decrypting.
 *
 * \param[in]  macSize     The size of the MAC.
 *
 * \param[in]  flags       Select encrypt/decrypt operations (gSecLib_CCM_Encrypt_c, gSecLib_CCM_Decrypt_c)
 *
 * \return 0 if encryption/decryption was successful; otherwise, error code for failed encryption/decryption
 *
 * \remarks At decryption, MIC fail is also signalled by returning a non-zero value.
 *
 ********************************************************************************** */
uint8_t AES_128_CCM(const uint8_t *pInput,
                    uint16_t       inputLen,
                    const uint8_t *pAuthData,
                    uint16_t       authDataLen,
                    const uint8_t *pNonce,
                    uint8_t        nonceSize,
                    const uint8_t *pKey,
                    uint8_t       *pOutput,
                    uint8_t       *pCbcMac,
                    uint8_t        macSize,
                    uint32_t       flags)
{
    size_t               key_bits   = AES_128_KEY_BYTE_LEN;
    psa_algorithm_t      alg        = PSA_ALG_CCM;
    uint8_t             *buff       = MEM_BufferAlloc((uint32_t)(inputLen + (uint32_t)macSize));
    psa_status_t         status     = PSA_ERROR_GENERIC_ERROR;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_id_t         key        = 0U;
    size_t               output_len = 0U;

    if (buff != NULL && pInput != NULL && pAuthData != NULL && pNonce != NULL && pOutput != NULL && pCbcMac != NULL)
    {
        /* set key usage depending on flags */
        if ((flags & gSecLib_CCM_Decrypt_c) != 0U)
        {
            psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_DECRYPT);
        }
        else
        {
            psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_ENCRYPT);
        }

        /* set the mac size of the algorithm to macSize, without this default is 32 */
        alg = PSA_ALG_AEAD_WITH_SHORTENED_TAG(alg, (uint32_t)macSize);

        psa_set_key_algorithm(&attributes, alg);
        psa_set_key_type(&attributes, PSA_KEY_TYPE_AES);

        do
        {
            status = psa_import_key(&attributes, pKey, key_bits, &key);
            RAISE_ERROR(status, PSA_SUCCESS);

            if ((flags & gSecLib_CCM_Decrypt_c) != 0U)
            {
                /* combine pInput and pCbcMac in a buffer to comply with PSA prototype psa_aead_decrypt */
                FLib_MemCpy(buff, pInput, inputLen);
                FLib_MemCpy(buff + inputLen, pCbcMac, macSize);
                status = psa_aead_decrypt(key, alg, pNonce, nonceSize, pAuthData, authDataLen, buff,
                                          (uint32_t)(inputLen + (uint32_t)macSize), pOutput,
                                          PSA_AEAD_DECRYPT_OUTPUT_MAX_SIZE(inputLen), &output_len);
            }
            else
            {
                status = psa_aead_encrypt(key, alg, pNonce, nonceSize, pAuthData, authDataLen, pInput, inputLen, buff,
                                          (uint32_t)(inputLen + (uint32_t)macSize), &output_len);
                /* split output buffer in pOutput and pCbcMac */
                FLib_MemCpy(pOutput, buff, inputLen);
                FLib_MemCpy(pCbcMac, buff + inputLen, macSize);
            }
            RAISE_ERROR(status, PSA_SUCCESS);

            status = psa_destroy_key(key); /* destroy key after use */
            RAISE_ERROR(status, PSA_SUCCESS);

            (void)MEM_BufferFree(buff);
        } while (false);
    }
    return (status == PSA_SUCCESS) ? (uint8_t)gSecSuccess_c : (uint8_t)gSecError_c;
}

/************************************************************************************
 * \brief Checks whether a public key is valid (point is on the curve).
 *
 * \return TRUE if valid, FALSE if not
 *
 ************************************************************************************/
bool_t ECP256_IsKeyValid(const ecp256Point_t *pKey)
{
    bool_t ret = false;

    if (ECP256_LePointValid(pKey))
    {
        ret = true;
    }

    return ret;
}

/************************************************************************************
 * \brief Computes the Diffie-Hellman Key for an ECDH P256 key pair.
 *
 * \return gSecSuccess_c or error
 *
 ************************************************************************************/
secResultType_t ECDH_P256_ComputeDhKeySeg(computeDhKeyParam_t *pDhKeyData)
{
    return ECDH_P256_ComputeDhKey(&pDhKeyData->privateKey, &pDhKeyData->peerPublicKey, &pDhKeyData->outPoint,
                                  pDhKeyData->keepInternalBlob);
}

/************************************************************************************
 * \brief Computes the Diffie-Hellman Key for an ECDH P256 key pair.
 *
 * \return gSecSuccess_c or error
 *
 ************************************************************************************/
secResultType_t ECDH_P256_ComputeDhKey(const ecdhPrivateKey_t *pInPrivateKey,
                                       const ecdhPublicKey_t  *pInPeerPublicKey,
                                       ecdhDhKey_t            *pOutDhKey,
                                       const bool_t            keepBlobDhKey)
{
    secResultType_t ret = gSecSuccess_c;
    uint8_t         bufPriv[sizeof(ecdhPrivateKey_t)];
    uint8_t         bufPub[sizeof(ecdhPublicKey_t)];
    uint8_t         bufSecret[sizeof(ecdhDhKey_t)];
    do
    {
        if (pOutDhKey == NULL)
        {
            ret = gSecError_c;
            break;
        }

        if (!ECP256_LePointValid(pInPeerPublicKey))
        {
            ret = gSecInvalidPublicKey_c;
            break;
        }

        /*little-indian to big-indian*/
        ECP256_PointCopy_and_change_endianness(bufPub, pInPeerPublicKey->raw);
        ECP256_coordinate_copy(bufPriv, psa_pECPKeyPair->PrivateKey->raw_8bit);

        psa_status_t status = p256_ecdh_shared_secret(bufSecret, (uint8_t const *)bufPriv, (uint8_t const *)bufPub);
        RAISE_ERROR(status, PSA_SUCCESS);

        /*big-indian to little-indian*/
        ECP256_PointCopy_and_change_endianness(pOutDhKey->raw, bufSecret);

    } while (false);
    return ret;
}

/************************************************************************************
 * \brief Generates a new ECDH P256 Private/Public key pair
 *
 * \return gSecSuccess_c or error
 *
 ************************************************************************************/
secResultType_t ECDH_P256_GenerateKeysSeg(computeDhKeyParam_t *pDhKeyData)
{
    return ECDH_P256_GenerateKeys(&pDhKeyData->outPoint, &pDhKeyData->privateKey);
}

/************************************************************************************
 * \brief Generates a new ECDH P256 Private/Public key pair
 *
 * \return gSecSuccess_c or error
 *
 ************************************************************************************/
secResultType_t ECDH_P256_GenerateKeys(ecdhPublicKey_t *pOutPublicKey, ecdhPrivateKey_t *pOutPrivateKey)
{
    secResultType_t ret = gSecSuccess_c;
    psa_status_t    st  = PSA_SUCCESS;
    do
    {
        if (psa_pECPKeyPair != NULL)
        {
            /* Once the key oject gets destroyed context is not ready anymore */
            st = psa_destroy_key(psa_pECPKeyPair->OwnKey);
            RAISE_ERROR(st, PSA_SUCCESS)
            FLib_MemSet(psa_pECPKeyPair, 0U, sizeof(psa_ecp256_context_t));
            psa_pECPKeyPair = NULL;
        }

        /* psa_g_ECP_KeyPair.keyId = KEY_ID_BLE0; */

        psa_pECPKeyPair = &psa_g_ECP_KeyPair;

        st = p256_gen_keypair(psa_pECPKeyPair->PrivateKey->raw_8bit, psa_pECPKeyPair->OwnPublicKey->raw_8bit);
        RAISE_ERROR(st, PSA_SUCCESS)

        /*big-indian to little-indian*/
        ECP256_PointCopy_and_change_endianness(pOutPublicKey->raw, psa_pECPKeyPair->OwnPublicKey->raw_8bit);
    } while (false);
    return ret;
}

/************************************************************************************
 * \brief Function used to create the mac key and LTK using Bluetooth F5 algorithm.
 *        Less secure version not using secure bus.
 *
 * \param  [out] pMacKey 128 bit MacKey output location (pointer)
 * \param  [out] pLtk    128 bit LTK output location (pointer)
 * \param  [in] pW       256 bit W (pointer) (DHKey)
 * \param  [in] pN1      128 bit N1 (pointer) (Na)
 * \param  [in] pN2      128 bit N2 (pointer) (Nb)
 * \param  [in] a1at     8 bit A1 address type, 0 = Public, 1 = Random
 * \param  [in] pA1      48 bit A1 (pointer) (A)
 * \param  [in] a2at     8 bit A2 address type, 0 = Public, 1 = Random
 * \param  [in] pA2      48 bit A2 (pointer) (B)
 *
 * \retval gSecSuccess_c operation succeeded
 * \retval gSecError_c operation failed
 *
 ************************************************************************************/
secResultType_t SecLib_GenerateBluetoothF5Keys(uint8_t       *pMacKey,
                                               uint8_t       *pLtk,
                                               const uint8_t *pW,
                                               const uint8_t *pN1,
                                               const uint8_t *pN2,
                                               const uint8_t  a1at,
                                               const uint8_t *pA1,
                                               const uint8_t  a2at,
                                               const uint8_t *pA2)
{
    secResultType_t result     = gSecError_c;
    const uint8_t   f5KeyId[4] = {0x62, 0x74, 0x6c, 0x65}; /*!< Big Endian, "btle" */
    uint8_t         f5CmacBuffer[1 + 4 + 16 + 16 + 7 + 7 + 2];
    /* Counter[1] || keyId[4] || N1[16] || N2[16] || A1[7] || A2[7] || Length[2] = 53 */

    uint8_t       f5T[16]    = {0};
    const uint8_t f5Salt[16] = {0x6C, 0x88, 0x83, 0x91, 0xAA, 0xF5, 0xA5, 0x38,
                                0x60, 0x37, 0x0B, 0xDB, 0x5A, 0x60, 0x83, 0xBE}; /*!< Big endian */

    do
    {
        uint8_t tempOut[16] = {0u};

        /*! Check for NULL output pointers and return with proper status if this is the case. */
        if ((NULL == pMacKey) || (NULL == pLtk) || (NULL == pN1) || (NULL == pN2) || (NULL == pA1) || (NULL == pA2))
        {
            break;
        }

        /*! Compute the f5 function key T using the predefined salt as key for AES-128-CAMC */
        AES_128_CMAC_LsbFirstInput((const uint8_t *)pW, 32, (const uint8_t *)f5Salt, f5T);

        /*! Build the most significant part of the f5 input data to compute the MacKey */
        f5CmacBuffer[0] = 0; /* Counter = 0 */
        FLib_MemCpy(&f5CmacBuffer[1], (const uint8_t *)f5KeyId, 4);
        FLib_MemCpyReverseOrder(&f5CmacBuffer[5], (const uint8_t *)pN1, 16);
        FLib_MemCpyReverseOrder(&f5CmacBuffer[21], (const uint8_t *)pN2, 16);
        f5CmacBuffer[37] = 0x01U & a1at;
        FLib_MemCpyReverseOrder(&f5CmacBuffer[38], (const uint8_t *)pA1, 6);
        f5CmacBuffer[44] = 0x01U & a2at;
        FLib_MemCpyReverseOrder(&f5CmacBuffer[45], (const uint8_t *)pA2, 6);
        f5CmacBuffer[51] = 0x01; /* Length msB big endian = 0x01, Length = 256 */
        f5CmacBuffer[52] = 0x00; /* Length lsB big endian = 0x00, Length = 256 */

        /*! Compute the MacKey into the temporary buffer. */
        AES_128_CMAC(f5CmacBuffer, sizeof(f5CmacBuffer), f5T, tempOut);

        /*! Copy the MacKey to the output location
         *  in reverse order. The CMAC result is generated MSB first. */
        FLib_MemCpyReverseOrder(pMacKey, (const uint8_t *)tempOut, 16);

        /*! Build the least significant part of the f5 input data to compute the MacKey.
         *  It is identical to the most significant part with the exception of the counter. */
        f5CmacBuffer[0] = 1; /* Counter = 1 */

        /*! Compute the LTK into the temporary buffer. */
        AES_128_CMAC(f5CmacBuffer, sizeof(f5CmacBuffer), f5T, tempOut);

        /*! Copy the LTK to the output location
         *  in reverse order. The CMAC result is generated MSB first. */
        FLib_MemCpyReverseOrder(pLtk, (const uint8_t *)tempOut, 16);

        result = gSecSuccess_c;

    } while (false);

    return result;
}

/*! *********************************************************************************
 * \brief  This function implements the SMP ah cryptographic toolbox function which
           calculates the hash part of a Resolvable Private Address.
 *         The key is kept in plaintext.
 *
 * \param[out]  pHash  Pointer where the 24 bit hash of a Resolvable Private Address value
 *                     will be written.
 *
 * \param[in]  pKey  Pointer to the 128 bit key.
 *
 * \param[in]  pR   Pointer to the 24 bit random value (Prand) of a Resolvable private Address.
 *                  The most significant bits of this field must be 0b01 for Resolvable Private
 *                  Addresses.
 *
 * \retval  gSecSuccess_c  All operations were successful.
 * \retval  gSecError_c The call failed.
 *
 ********************************************************************************** */
secResultType_t SecLib_VerifyBluetoothAh(uint8_t *pHash, const uint8_t *pKey, const uint8_t *pR)
{
    secResultType_t result           = gSecError_c;
    uint8_t         tempAddrPart[16] = {0u};
    uint8_t         tempOutHash[16]  = {0u};
    uint8_t         tempKey[16]      = {0u};
    do
    {
        /*! Check for NULL output pointers and return with proper status if this is the case. */
        if ((NULL == pHash) || (NULL == pKey) || (NULL == pR))
        {
            break;
        }
        /* Initialize the r' value in the temporary location. 3 bytes of ramdom value.
         *  Initialize it reversed for AES.
         */
        for (int i = 0; i < 3; i++)
        {
            tempAddrPart[15 - i] = pR[i];
        }
        /* Regular operation with plaintext key */
        /*! Reverse the Key and place it in a temporary location. */
        FLib_MemCpyReverseOrder(tempKey, (const uint8_t *)pKey, 16);

        /*! Compute the hash. */
        AES_128_Encrypt(tempAddrPart, tempKey, tempOutHash);

        /*! Copy the relevant bytes to the output. */
        pHash[0] = tempOutHash[15];
        pHash[1] = tempOutHash[14];
        pHash[2] = tempOutHash[13];

        result = gSecSuccess_c;

    } while (false);
    return result;
}

void ECDH_P256_FreeDhKeyDataSecure(computeDhKeyParam_t *pDhKeyData)
{
    NOT_USED(pDhKeyData);
}

secResultType_t SecLib_DeriveBluetoothSKDSecure(const uint8_t *pInSKD,
                                                const uint8_t *pLtkBlob,
                                                bool_t         bOpenKey,
                                                uint8_t       *pOutSKD)
{
    NOT_USED(pInSKD);
    NOT_USED(pLtkBlob);
    NOT_USED(bOpenKey);
    NOT_USED(pOutSKD);

    return gSecError_c;
}

secResultType_t SecLib_ObfuscateKeySecure(const uint8_t *pKey, uint8_t *pBlob, const uint8_t blobType)
{
    NOT_USED(pKey);
    NOT_USED(pBlob);
    NOT_USED(blobType);
    return gSecError_c;
}

secResultType_t SecLib_DeobfuscateKeySecure(const uint8_t *pBlob, uint8_t *pKey)
{
    NOT_USED(pBlob);
    NOT_USED(pKey);
    return gSecError_c;
}

secResultType_t ECDH_P256_ComputeA2BKeySecure(const ecdhPublicKey_t *pInPeerPublicKey, ecdhDhKey_t *pOutE2EKey)
{
    NOT_USED(pInPeerPublicKey);
    NOT_USED(pOutE2EKey);
    return gSecError_c;
}

secResultType_t ECDH_P256_FreeE2EKeyDataSecure(ecdhDhKey_t *pE2EKeyData)
{
    NOT_USED(pE2EKeyData);
    return gSecError_c;
}

secResultType_t SecLib_ExportA2BBlobSecure(const void *pKey, const secInputKeyType_t keyType, uint8_t *pOutKey)
{
    NOT_USED(pKey);
    NOT_USED(keyType);
    NOT_USED(pOutKey);
    return gSecError_c;
}

secResultType_t SecLib_ImportA2BBlobSecure(const uint8_t *pKey, const secInputKeyType_t keyType, uint8_t *pOutKey)
{
    NOT_USED(pKey);
    NOT_USED(keyType);
    NOT_USED(pOutKey);
    return gSecError_c;
}

secResultType_t SecLib_GenerateBluetoothF5KeysSecure(uint8_t       *pMacKey,
                                                     uint8_t       *pLtk,
                                                     const uint8_t *pW,
                                                     const uint8_t *pN1,
                                                     const uint8_t *pN2,
                                                     const uint8_t  a1at,
                                                     const uint8_t *pA1,
                                                     const uint8_t  a2at,
                                                     const uint8_t *pA2)
{
    NOT_USED(pMacKey);
    NOT_USED(pLtk);
    NOT_USED(pW);
    NOT_USED(pN1);
    NOT_USED(pN2);
    NOT_USED(a1at);
    NOT_USED(pA1);
    NOT_USED(a2at);
    NOT_USED(pA2);
    return gSecError_c;
}

secResultType_t SecLib_VerifyBluetoothAhSecure(uint8_t *pHash, const uint8_t *pKey, const uint8_t *pR)
{
    NOT_USED(pHash);
    NOT_USED(pKey);
    NOT_USED(pR);
    return gSecError_c;
}

secResultType_t SecLib_GenerateBluetoothEIRKBlobSecure(const void  *pIRK,
                                                       const bool_t blobInput,
                                                       const bool_t generateDKeyIRK,
                                                       uint8_t     *pOutEIRKblob)
{
    NOT_USED(pIRK);
    NOT_USED(blobInput);
    NOT_USED(generateDKeyIRK);
    NOT_USED(pOutEIRKblob);
    return gSecError_c;
}