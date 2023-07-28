/*! *********************************************************************************
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2022 NXP
 * All rights reserved.
 *
 * \file
 *
 * This is the source file for the security module used by the connectivity stacks. The Security
 *    Module SecLib provides an abstraction from the Hardware to the upper layer.
 *    In this file, a wrapper to mbdedTLS component is implemented
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ********************************************************************************** */
/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */

#include "fwk_platform.h"
#include "fsl_component_mem_manager.h"
#include "FunctionLib.h"

#include "SecLib.h"
#include "fsl_device_registers.h"
#include "fsl_os_abstraction.h"
#include "RNG_Interface.h"

/* mbedTLS headers */
#include "mbedtls/aes.h"
#include "mbedtls/cipher.h"
#include "mbedtls/cmac.h"
#include "mbedtls/ccm.h"

#include "mbedtls/sha1.h"

#include "mbedtls/sha256.h"
#include "mbedtls/md.h"
#include "mbedtls/ecdh.h"
#include "sss_crypto.h"

/*! *********************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
********************************************************************************** */

#if USE_RTOS && 1 // Currently only tested with Mutex enabled in SecLib()
#define gSecLibUseMutex_c TRUE
#else
#define gSecLibUseMutex_c FALSE
#endif

#if gSecLibUseMutex_c
#define SECLIB_MUTEX_LOCK()   OSA_MutexLock((osa_mutex_handle_t)mSecLibMutexId, osaWaitForever_c)
#define SECLIB_MUTEX_UNLOCK() OSA_MutexUnlock((osa_mutex_handle_t)mSecLibMutexId)
#else
#define SECLIB_MUTEX_LOCK()
#define SECLIB_MUTEX_UNLOCK()
#endif /* gSecLibUseMutex_c */

/*! *********************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
********************************************************************************** */
#if gSecLibUseMutex_c
/*! Mutex used to protect the AES Context when an RTOS is used. */
OSA_MUTEX_HANDLE_DEFINE(mSecLibMutexId);
#endif /* gSecLibUseMutex_c */

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
/*!
 * @brief pointer to Callback function used to offload Security steps onto application message queue
 */
static secLibCallback_t pfSecLibMultCallback = NULL;

/*! *********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
 * \brief  This function performs initialization of the cryptografic HW acceleration.
 *
 ********************************************************************************** */
void SecLib_Init(void)
{
    /* Initialize cryptographic hardware.*/
    PLATFORM_InitCrypto();

#if gSecLibUseMutex_c
    /*! Initialize the SecLib Mutex here. */
    if (KOSA_StatusSuccess != OSA_MutexCreate((osa_mutex_handle_t)mSecLibMutexId))
    {
        assert(0);
        return;
    }
#endif
}

/*! *********************************************************************************
 * \brief  This function performs initialization of the cryptografic HW acceleration.
 *
 ********************************************************************************** */
void SecLib_ReInit(void)
{
    /* Initialize cryptographic hardware.*/
    (void)CRYPTO_ReinitHardware();
}

/*! *********************************************************************************
 * \brief  This function will allow reinitizialize the cryptographic HW acceleration
 * next time we need it, typically after lowpower mode.
 *
 ********************************************************************************** */
void SecLib_DeInit(void)
{
    CRYPTO_DeinitHardware();
}

/*! *********************************************************************************
 * \brief  This function performs initialization of the callback used to offload
 * elliptic curve multiplication.
 *
 * \param[in]  pfCallback Pointer to the function used to handle multiplication.
 *
 ********************************************************************************** */
void SecLib_SetExternalMultiplicationCb(secLibCallback_t pfCallback)
{
    pfSecLibMultCallback = pfCallback;
}

/*! *********************************************************************************
 * \brief  This function performs initialization of the callback used to offload
 * elliptic curve multiplication.
 *
 * \param[in]  pfCallback Pointer to the function used to handle multiplication.
 *
 ********************************************************************************** */
void SecLib_SetLowpowerCriticalCb(const Seclib_LowpowerCriticalCBs_t *pfCallback)
{
    (void)pfCallback;
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
    int                 result;
    mbedtls_aes_context aesCtx;

    mbedtls_aes_init(&aesCtx);
    result = mbedtls_aes_setkey_enc(&aesCtx, pKey, AES_128_KEY_BITS);
    if (result != 0)
    {
        assert(0);
    }

    SECLIB_MUTEX_LOCK();

    result = mbedtls_aes_crypt_ecb(&aesCtx, MBEDTLS_AES_ENCRYPT, pInput, pOutput);
    if (result != 0)
    {
        assert(0);
    }

    SECLIB_MUTEX_UNLOCK();

    mbedtls_aes_free(&aesCtx);
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
    int                 result;
    mbedtls_aes_context aesCtx;

    mbedtls_aes_init(&aesCtx);
    result = mbedtls_aes_setkey_dec(&aesCtx, pKey, AES_128_KEY_BITS);
    if (result != 0)
    {
        assert(0);
    }

    SECLIB_MUTEX_LOCK();

    result = mbedtls_aes_crypt_ecb(&aesCtx, MBEDTLS_AES_DECRYPT, pInput, pOutput);
    if (result != 0)
    {
        assert(0);
    }

    SECLIB_MUTEX_UNLOCK();

    mbedtls_aes_free(&aesCtx);
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
    /* If the input length is not a multiple of AES 128 block size return */
    if ((inputLen == 0U) || ((inputLen % AES_128_BLOCK_SIZE) != 0U))
    {
        return;
    }

    /* Process all data blocks*/
    while (inputLen != 0U)
    {
        AES_128_Encrypt(pInput, pKey, pOutput);
        pInput += AES_128_BLOCK_SIZE;
        pOutput += AES_128_BLOCK_SIZE;
        inputLen -= AES_128_BLOCK_SIZE;
    }
}

/*! *********************************************************************************
 * \brief  This function performs AES-128-CBC encryption on a message block.
 *         This function only accepts input lengths which are multiple
 *         of 16 bytes (AES 128 block size).
 *
 * \param[in]  pInput Pointer to the location of the input message.
 *
 * \param[in]  inputLen Input message length in bytes.
 *
 * \param[in]  pInitVector Pointer to the location of the 128-bit initialization vector.
 *
 * \param[in]  pKey Pointer to the location of the 128-bit key.
 *
 * \param[out]  pOutput Pointer to the location to store the ciphered output.
 *
 ********************************************************************************** */
void AES_128_CBC_Encrypt(
    const uint8_t *pInput, uint32_t inputLen, uint8_t *pInitVector, const uint8_t *pKey, uint8_t *pOutput)
{
    int                 result;
    mbedtls_aes_context aesCtx;

    /* If the input length is not a multiple of AES 128 block size return */
    if ((inputLen == 0U) || ((inputLen % AES_128_BLOCK_SIZE) != 0U))
    {
        return;
    }

    mbedtls_aes_init(&aesCtx);
    result = mbedtls_aes_setkey_enc(&aesCtx, pKey, AES_128_KEY_BITS);
    if (result != 0)
    {
        assert(0);
    }

    SECLIB_MUTEX_LOCK();

    result = mbedtls_aes_crypt_cbc(&aesCtx, MBEDTLS_AES_ENCRYPT, inputLen, pInitVector, pInput, pOutput);
    if (result != 0)
    {
        assert(0);
    }

    SECLIB_MUTEX_UNLOCK();

    mbedtls_aes_free(&aesCtx);
}

/*! *********************************************************************************
 * \brief  This function performs AES-128-CBC encryption on a message block after
 *         padding it with 1 bit of 1 and 0 bits trail.
 *
 * \param[in]  pInput Pointer to the location of the input message.
 *
 * \param[in]  inputLen Input message length in bytes.
 *
 *             IMPORTANT: User must make sure that input and output
 *             buffers have at least inputLen + 16 bytes size
 *
 * \param[in]  pInitVector Pointer to the location of the 128-bit initialization vector.
 *
 * \param[in]  pKey Pointer to the location of the 128-bit key.
 *
 * \param[out]  pOutput Pointer to the location to store the ciphered output.
 *
 * Return value: size of output buffer (after padding)
 *
 ********************************************************************************** */
uint32_t AES_128_CBC_Encrypt_And_Pad(
    uint8_t *pInput, uint32_t inputLen, uint8_t *pInitVector, const uint8_t *pKey, uint8_t *pOutput)
{
    int                 result;
    uint32_t            newLen = 0;
    uint32_t            idx;
    mbedtls_aes_context aesCtx;

    /* compute new length */
    newLen = inputLen + (AES_128_BLOCK_SIZE - (inputLen & (AES_128_BLOCK_SIZE - 1U)));
    /* pad the input buffer with 1 bit of 1 and trail of 0's from inputLen to newLen */
    for (idx = 0U; idx < ((newLen - inputLen) - 1U); idx++)
    {
        pInput[newLen - 1U - idx] = 0x00U;
    }
    pInput[inputLen] = 0x80U;

    mbedtls_aes_init(&aesCtx);
    result = mbedtls_aes_setkey_enc(&aesCtx, pKey, AES_128_KEY_BITS);
    if (result != 0)
    {
        assert(0);
    }

    SECLIB_MUTEX_LOCK();

    result = mbedtls_aes_crypt_cbc(&aesCtx, MBEDTLS_AES_ENCRYPT, inputLen, pInitVector, pInput, pOutput);
    if (result != 0)
    {
        assert(0);
    }

    SECLIB_MUTEX_UNLOCK();

    mbedtls_aes_free(&aesCtx);

    return newLen;
}

/*! *********************************************************************************
 * \brief  This function performs AES-128-CBC decryption on a message block.
 *
 * \param[in]  pInput Pointer to the location of the input message.
 *
 * \param[in]  inputLen Input message length in bytes.
 *
 * \param[in]  pInitVector Pointer to the location of the 128-bit initialization vector.
 *
 * \param[in]  pKey Pointer to the location of the 128-bit key.
 *
 * \param[out]  pOutput Pointer to the location to store the ciphered output.
 *
 * Return value: size of output buffer (after depadding)
 *
 ********************************************************************************** */
uint32_t AES_128_CBC_Decrypt_And_Depad(
    const uint8_t *pInput, uint32_t inputLen, uint8_t *pInitVector, const uint8_t *pKey, uint8_t *pOutput)
{
    int                 result;
    uint32_t            newLen = inputLen;
    mbedtls_aes_context aesCtx;

    mbedtls_aes_init(&aesCtx);
    result = mbedtls_aes_setkey_dec(&aesCtx, pKey, AES_128_KEY_BITS);
    if (result != 0)
    {
        assert(0);
    }

    SECLIB_MUTEX_LOCK();

    result = mbedtls_aes_crypt_cbc(&aesCtx, MBEDTLS_AES_DECRYPT, inputLen, pInitVector, pInput, pOutput);
    if (result != 0)
    {
        assert(0);
    }

    SECLIB_MUTEX_UNLOCK();

    mbedtls_aes_free(&aesCtx);

    while ((pOutput[--newLen] != 0x80U) && (newLen != 0U))
    {
    }
    return newLen;
}

/*! *********************************************************************************
 * \brief  This function performs AES-128-CTR encryption on a message block.
 *         This function only accepts input lengths which are multiple
 *         of 16 bytes (AES 128 block size).
 *
 * \param[in]  pInput Pointer to the location of the input message.
 *
 * \param[in]  inputLen Input message length in bytes.
 *
 * \param[in]  pCounter Pointer to the location of the 128-bit counter.
 *
 * \param[in]  pKey Pointer to the location of the 128-bit key.
 *
 * \param[out]  pOutput Pointer to the location to store the ciphered output.
 *
 ********************************************************************************** */
void AES_128_CTR(const uint8_t *pInput, uint32_t inputLen, uint8_t *pCounter, const uint8_t *pKey, uint8_t *pOutput)
{
    int                 result;
    mbedtls_aes_context aesCtx;
    uint32_t            ctrOffset                     = 0U;
    uint8_t             streamBlk[AES_128_BLOCK_SIZE] = {0U};

    /* If the input length is not a multiple of AES 128 block size return */
    if ((inputLen == 0U) || ((inputLen % AES_128_BLOCK_SIZE) != 0U))
    {
        return;
    }

    mbedtls_aes_init(&aesCtx);
    result = mbedtls_aes_setkey_enc(&aesCtx, pKey, AES_128_KEY_BITS);
    if (result != 0)
    {
        assert(0);
    }

    SECLIB_MUTEX_LOCK();

    result = mbedtls_aes_crypt_ctr(&aesCtx, inputLen, (size_t *)&ctrOffset, pCounter, (unsigned char *)&streamBlk,
                                   pInput, pOutput);
    if (result != 0)
    {
        assert(0);
    }

    SECLIB_MUTEX_UNLOCK();

    mbedtls_aes_free(&aesCtx);
}

#if gSecLibAesOfbEnable_d

/*! *********************************************************************************
 * \brief  This function performs AES-128-OFB encryption on a message block.
 *         This function only accepts input lengths which are multiple
 *         of 16 bytes (AES 128 block size).
 *
 * \param[in]  pInput Pointer to the location of the input message.
 *
 * \param[in]  inputLen Input message length in bytes.
 *
 * \param[in]  pInitVector Pointer to the location of the 128-bit initialization vector.
 *
 * \param[in]  pKey Pointer to the location of the 128-bit key.
 *
 * \param[out]  pOutput Pointer to the location to store the ciphered output.
 *
 ********************************************************************************** */
void AES_128_OFB(uint8_t *pInput, uint32_t inputLen, uint8_t *pInitVector, uint8_t *pKey, uint8_t *pOutput)
{
    uint8_t tempBuffIn[AES_128_BLOCK_SIZE]  = {0U};
    uint8_t tempBuffOut[AES_128_BLOCK_SIZE] = {0U};

    if (pInitVector != NULL)
    {
        FLib_MemCpy(tempBuffIn, pInitVector, AES_128_BLOCK_SIZE);
    }

    /* If the input length is not a multiple of AES 128 block size return */
    if ((inputLen == 0U) || ((inputLen % AES_128_BLOCK_SIZE) != 0U))
    {
        return;
    }

    /* Process all data blocks*/
    while (inputLen != 0U)
    {
        AES_128_Encrypt(tempBuffIn, pKey, tempBuffOut);
        FLib_MemCpy(tempBuffIn, tempBuffOut, AES_128_BLOCK_SIZE);
        SecLib_XorN(tempBuffOut, pInput, AES_128_BLOCK_SIZE);
        FLib_MemCpy(pOutput, tempBuffOut, AES_128_BLOCK_SIZE);
        pInput += AES_128_BLOCK_SIZE;
        pOutput += AES_128_BLOCK_SIZE;
        inputLen -= AES_128_BLOCK_SIZE;
    }
}
#endif /* gSecLibAesOfbEnable_d */

/*! *********************************************************************************
 * \brief  This function performs AES-128-CMAC on a message block.
 *
 * \param[in]  pInput Pointer to the location of the input message.
 *
 * \param[in]  inputLen Length of the input message in bytes. The input data must be provided MSB first.
 *
 * \param[in]  pKey Pointer to the location of the 128-bit key. The key must be provided MSB first.
 *
 * \param[out]  pOutput Pointer to the location to store the 16-byte authentication code.
 *              The code will be generated MSB first.
 *
 * \remarks This is public open source code! Terms of use must be checked before use!
 *
 ********************************************************************************** */
void AES_128_CMAC(const uint8_t *pInput, const uint32_t inputLen, const uint8_t *pKey, uint8_t *pOutput)
{
    int                          result;
    const mbedtls_cipher_info_t *pCmacCipherInfo;

    pCmacCipherInfo = mbedtls_cipher_info_from_type(MBEDTLS_CIPHER_AES_128_ECB);

    SECLIB_MUTEX_LOCK();

    result = mbedtls_cipher_cmac(pCmacCipherInfo, pKey, AES_128_KEY_BITS, pInput, inputLen, pOutput);
    if (result != 0)
    {
        assert(0);
    }
    SECLIB_MUTEX_UNLOCK();
}

/*! *********************************************************************************
 * \brief  This function performs AES-128-CMAC on a message block accepting input data
 *         which is in LSB first format and computing the authentication code starting from the end of the data.
 *
 * \param[in]  pInput Pointer to the location of the input message.
 *             The input data must be provided LSB first.
 *
 * \param[in]  inputLen Length of the input message in bytes.
 *
 * \param[in]  pKey Pointer to the location of the 128-bit key.
 *             The key must be provided MSB first.
 *
 * \param[out]  pOutput Pointer to the location to store the 16-byte authentication code.
 *              The code will be generated MSB first.
 *
 ********************************************************************************** */
void AES_128_CMAC_LsbFirstInput(const uint8_t *pInput, uint32_t inputLen, const uint8_t *pKey, uint8_t *pOutput)
{
    int                          result;
    const mbedtls_cipher_info_t *pCmacCipherInfo;
    mbedtls_cipher_context_t     cmacCipherCtx;
    uint8_t                      reversedBlock[AES_128_BLOCK_SIZE] = {0};

    /* This function does not perform dynamic allocation, it just returns
     * the address of a static global structure. */
    pCmacCipherInfo = mbedtls_cipher_info_from_type(MBEDTLS_CIPHER_AES_128_ECB);

    mbedtls_cipher_init(&cmacCipherCtx);
    result = mbedtls_cipher_setup(&cmacCipherCtx, pCmacCipherInfo);
    if (result != 0)
    {
        assert(0);
    }
    result = mbedtls_cipher_cmac_starts(&cmacCipherCtx, pKey, AES_128_KEY_BITS);
    if (result != 0)
    {
        assert(0);
    }

    /* Walk the input buffer from the end to the start and reverse the blocks
     * before calling the CMAC update function. */
    pInput += inputLen;
    do
    {
        uint32_t currentCmacInputBlkLen = 0;
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
        /* Copy the input block to the reversed CAMC input buffer */
        FLib_MemCpyReverseOrder(reversedBlock, pInput, currentCmacInputBlkLen);

        SECLIB_MUTEX_LOCK();

        result =
            mbedtls_cipher_cmac_update(&cmacCipherCtx, (const unsigned char *)&reversedBlock, currentCmacInputBlkLen);
        if (result != 0)
        {
            assert(0);
        }

        SECLIB_MUTEX_UNLOCK();
    } while (inputLen != 0U);

    SECLIB_MUTEX_LOCK();

    result = mbedtls_cipher_cmac_finish(&cmacCipherCtx, pOutput);
    if (result != 0)
    {
        assert(0);
    }

    SECLIB_MUTEX_UNLOCK();

    /*CALL THIS AT THE END*/
    mbedtls_cipher_free(&cmacCipherCtx);
}

/*! *********************************************************************************
 * \brief  This function performs AES 128 CMAC Pseudo-Random Function (AES-CMAC-PRF-128),
 *         according to rfc4615, on a message block.
 *
 * \details The AES-CMAC-PRF-128 algorithm behaves similar to teh AES CMAC 128 algorithm
 *          but removes 128 bit key size restriction.
 *
 * \param[in]  pInput Pointer to the location of the input message.
 *
 * \param[in]  inputLen Length of the input message in bytes.
 *
 * \param[in]  pVarKey Pointer to the location of the variable length key.
 *
 * \param[in]  varKeyLen Length of the input key in bytes
 *
 * \param[out]  pOutput Pointer to the location to store the 16-byte pseudo random variable.
 *
 ********************************************************************************** */
void AES_CMAC_PRF_128(uint8_t *pInput, uint32_t inputLen, uint8_t *pVarKey, uint32_t varKeyLen, uint8_t *pOutput)
{
    int result;
    SECLIB_MUTEX_LOCK();

    result = mbedtls_aes_cmac_prf_128(pVarKey, varKeyLen, pInput, inputLen, pOutput);
    if (result != 0)
    {
        assert(0);
    }

    SECLIB_MUTEX_UNLOCK();
}

/*! *********************************************************************************
 * \brief  This function performs AES-128-EAX encryption on a message block.
 *
 * \param[in]  pInput Pointer to the location of the input message.
 *
 * \param[in]  inputLen Length of the input message in bytes.
 *
 * \param[in]  pNonce Pointer to the location of the nonce.
 *
 * \param[in]  nonceLen Nonce length in bytes.
 *
 * \param[in]  pHeader Pointer to the location of header.
 *
 * \param[in]  headerLen Header length in bytes.
 *
 * \param[in]  pKey Pointer to the location of the 128-bit key.
 *
 * \param[out]  pOutput Pointer to the location to store the 16-byte authentication code.
 *
 * \param[out]  pTag Pointer to the location to store the 128-bit tag.
 *
 ********************************************************************************** */
secResultType_t AES_128_EAX_Encrypt(uint8_t *pInput,
                                    uint32_t inputLen,
                                    uint8_t *pNonce,
                                    uint32_t nonceLen,
                                    uint8_t *pHeader,
                                    uint8_t  headerLen,
                                    uint8_t *pKey,
                                    uint8_t *pOutput,
                                    uint8_t *pTag)
{
    uint8_t *       buf;
    uint32_t        buf_len;
    uint8_t         nonce_mac[AES_128_BLOCK_SIZE] = {0};
    uint8_t         hdr_mac[AES_128_BLOCK_SIZE]   = {0};
    uint8_t         data_mac[AES_128_BLOCK_SIZE]  = {0};
    uint8_t         tempBuff[AES_128_BLOCK_SIZE]  = {0};
    secResultType_t status                        = gSecSuccess_c;
    uint32_t        i;

    if (nonceLen > inputLen)
    {
        buf_len = nonceLen;
    }
    else
    {
        buf_len = inputLen;
    }

    if (headerLen > buf_len)
    {
        buf_len = headerLen;
    }

    buf_len += 16U;

    buf = MEM_BufferAlloc(buf_len);

    if (buf == NULL)
    {
        status = gSecAllocError_c;
    }
    else
    {
        FLib_MemSet(buf, 0, 15);

        buf[15] = 0U;
        FLib_MemCpy((buf + 16), pNonce, nonceLen);
        AES_128_CMAC(buf, 16U + nonceLen, pKey, nonce_mac);

        buf[15] = 1U;
        FLib_MemCpy((buf + 16), pHeader, headerLen);
        AES_128_CMAC(buf, 16U + (uint32_t)headerLen, pKey, hdr_mac);

        /* keep the original value of nonce_mac, because AES_128_CTR will increment it */
        FLib_MemCpy(tempBuff, nonce_mac, nonceLen);

        AES_128_CTR(pInput, inputLen, tempBuff, pKey, pOutput);

        buf[15] = 2U;
        FLib_MemCpy((buf + 16), pOutput, inputLen);
        AES_128_CMAC(buf, 16U + inputLen, pKey, data_mac);

        for (i = 0U; i < AES_128_BLOCK_SIZE; i++)
        {
            pTag[i] = nonce_mac[i] ^ data_mac[i] ^ hdr_mac[i];
        }

        (void)MEM_BufferFree(buf);
    }

    return status;
}

/*! *********************************************************************************
 * \brief  This function performs AES-128-EAX decryption on a message block.
 *
 * \param[in]  pInput Pointer to the location of the input message.
 *
 * \param[in]  inputLen Length of the input message in bytes.
 *
 * \param[in]  pNonce Pointer to the location of the nonce.
 *
 * \param[in]  nonceLen Nonce length in bytes.
 *
 * \param[in]  pHeader Pointer to the location of header.
 *
 * \param[in]  headerLen Header length in bytes.
 *
 * \param[in]  pKey Pointer to the location of the 128-bit key.
 *
 * \param[out]  pOutput Pointer to the location to store the 16-byte authentication code.
 *
 * \param[out]  pTag Pointer to the location to store the 128-bit tag.
 *
 ********************************************************************************** */
secResultType_t AES_128_EAX_Decrypt(uint8_t *pInput,
                                    uint32_t inputLen,
                                    uint8_t *pNonce,
                                    uint32_t nonceLen,
                                    uint8_t *pHeader,
                                    uint8_t  headerLen,
                                    uint8_t *pKey,
                                    uint8_t *pOutput,
                                    uint8_t *pTag)
{
    uint8_t *       buf;
    uint32_t        buf_len;
    uint8_t         nonce_mac[AES_128_BLOCK_SIZE] = {0};
    uint8_t         hdr_mac[AES_128_BLOCK_SIZE]   = {0};
    uint8_t         data_mac[AES_128_BLOCK_SIZE]  = {0};
    secResultType_t status                        = gSecSuccess_c;
    uint32_t        i;

    if (nonceLen > inputLen)
    {
        buf_len = nonceLen;
    }
    else
    {
        buf_len = inputLen;
    }

    if (headerLen > buf_len)
    {
        buf_len = headerLen;
    }

    buf_len += 16U;

    buf = MEM_BufferAlloc(buf_len);

    if (buf == NULL)
    {
        status = gSecAllocError_c;
    }
    else
    {
        FLib_MemSet(buf, 0, 15);

        buf[15] = 0U;
        FLib_MemCpy((buf + 16), pNonce, nonceLen);
        AES_128_CMAC(buf, 16U + nonceLen, pKey, nonce_mac);

        buf[15] = 1U;
        FLib_MemCpy((buf + 16), pHeader, headerLen);
        AES_128_CMAC(buf, 16U + (uint32_t)headerLen, pKey, hdr_mac);

        buf[15] = 2U;
        FLib_MemCpy((buf + 16), pInput, inputLen);
        AES_128_CMAC(buf, 16U + inputLen, pKey, data_mac);

        (void)MEM_BufferFree(buf);

        for (i = 0U; i < AES_128_BLOCK_SIZE; i++)
        {
            if (pTag[i] != (nonce_mac[i] ^ data_mac[i] ^ hdr_mac[i]))
            {
                status = gSecError_c;
                break;
            }
        }

        if (gSecSuccess_c == status)
        {
            AES_128_CTR(pInput, inputLen, nonce_mac, pKey, pOutput);
        }
    }

    return status;
}

/*! *********************************************************************************
 * \brief  This function performs AES-128-CCM on a message block.
 *
 * \param[in]  pInput       Pointer to the location of the input message (plaintext or cyphertext).
 *
 * \param[in]  inputLen     Length of the input plaintext in bytes when encrypting.
 *                          Length of the input cypertext without the MAC length when decrypting.
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
 *                          Pointer to the location to store the cyphertext data when encrypting.
 *
 * \param[out]  pCbcMac     Pointer to the location to store the Message Authentication Code (MAC) when encrypting.
 *                          Pointer to the location where the received MAC can be found when decrypting.
 *
 * \param[out]  macSize     The size of the MAC.
 *
 * \param[out]  flags       Select encrypt/decrypt operations (gSecLib_CCM_Encrypt_c, gSecLib_CCM_Decrypt_c)
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
                    uint8_t *      pOutput,
                    uint8_t *      pCbcMac,
                    uint8_t        macSize,
                    uint32_t       flags)
{
    int32_t             status;
    mbedtls_ccm_context ccmCtx;
    mbedtls_ccm_init(&ccmCtx);
    status = mbedtls_ccm_setkey(&ccmCtx, MBEDTLS_CIPHER_ID_AES, pKey, AES_128_KEY_BITS);
    assert(status == 0);

    SECLIB_MUTEX_LOCK();

    if ((flags & gSecLib_CCM_Decrypt_c) != 0U)
    {
        status = mbedtls_ccm_auth_decrypt(&ccmCtx, inputLen, pNonce, nonceSize, pAuthData, authDataLen, pInput, pOutput,
                                          pCbcMac, macSize);
    }
    else
    {
        status = mbedtls_ccm_encrypt_and_tag(&ccmCtx, inputLen, pNonce, nonceSize, pAuthData, authDataLen, pInput,
                                             pOutput, pCbcMac, macSize);
    }

    SECLIB_MUTEX_UNLOCK();

    mbedtls_ccm_free(&ccmCtx);

    if (status == 0)
    {
        return (uint8_t)gSecSuccess_c;
    }
    else
    {
        return (uint8_t)gSecError_c;
    }
}

/*! *********************************************************************************
 * \brief  This function calculates XOR of individual byte pairs in two uint8_t arrays.
 *         pDst[i] := pDst[i] ^ pSrc[i] for i=0 to n-1
 *
 * \param[in, out]  pDst First byte array operand for XOR and destination byte array
 *
 * \param[in]  pSrc Second byte array operand for XOR
 *
 * \param[in]  n  Length of the byte arrays which will be XORed
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
*************************************************************************************
* Private functions
*************************************************************************************
********************************************************************************** */

#if defined(MBEDTLS_SHA1_C)
/*! *********************************************************************************
 * \brief  This function allocates a memory buffer for a SHA1 context structure
 *
 * \return    Address of the SHA1 context buffer
 *            Deallocate using SHA1_FreeCtx()
 *
 ********************************************************************************** */
void *SHA1_AllocCtx(void)
{
    void *sha1Ctx = MEM_BufferAlloc(sizeof(mbedtls_sha1_context));

    return sha1Ctx;
}

/*! *********************************************************************************
 * \brief  This function deallocates the memory buffer for the SHA1 context structure
 *
 * \param [in]    pContext    Address of the SHA1 context buffer
 *
 ********************************************************************************** */
void SHA1_FreeCtx(void *pContext)
{
    (void)MEM_BufferFree(pContext);
}

/*! *********************************************************************************
 * \brief  This function clones a SHA1 context.
 *         Make sure the size of the allocated destination context buffer is appropriate.
 *
 * \param [in]    pDestCtx    Address of the destination SHA1 context
 * \param [in]    pSourceCtx  Address of the source SHA1 context
 *
 ********************************************************************************** */
void SHA1_CloneCtx(void *pDestCtx, void *pSourceCtx)
{
    mbedtls_sha1_clone(pDestCtx, pSourceCtx);
}

/*! *********************************************************************************
 * \brief  This function initializes the SHA1 context data
 *
 * \param [in]    pContext    Pointer to the SHA1 context data
 *                            Allocated using SHA1_AllocCtx()
 *
 ********************************************************************************** */
void SHA1_Init(void *pContext)
{
    mbedtls_sha1_context *pSha1Ctx = (mbedtls_sha1_context *)pContext;

    SECLIB_MUTEX_LOCK();

    mbedtls_sha1_init(pSha1Ctx);
    mbedtls_sha1_starts(pSha1Ctx);

    SECLIB_MUTEX_UNLOCK();
}

/*! *********************************************************************************
 * \brief  This function performs SHA1 on multiple bytes and updates the context data
 *
 * \param [in]    pContext    Pointer to the SHA1 context data
 *                            Allocated using SHA1_AllocCtx()
 * \param [in]    pData       Pointer to the input data
 * \param [in]    numBytes    Number of bytes to hash
 *
 ********************************************************************************** */
void SHA1_HashUpdate(void *pContext, uint8_t *pData, uint32_t numBytes)
{
    mbedtls_sha1_context *pSha1Ctx = (mbedtls_sha1_context *)pContext;

    SECLIB_MUTEX_LOCK();

    mbedtls_sha1_update(pSha1Ctx, pData, numBytes);

    SECLIB_MUTEX_UNLOCK();
}

/*! *********************************************************************************
 * \brief  This function performs SHA1 on the last bytes of data and updates the context data.
 *         The final hash value is stored at the provided output location.
 *
 * \param [in]       pContext    Pointer to the SHA1 context data
 *                               Allocated using SHA1_AllocCtx()
 * \param [in,out]   pOutput     Pointer to the output location
 *
 ********************************************************************************** */
void SHA1_HashFinish(void *pContext, uint8_t *pOutput)
{
    mbedtls_sha1_context *pSha1Ctx = (mbedtls_sha1_context *)pContext;

    SECLIB_MUTEX_LOCK();

    mbedtls_sha1_finish(pSha1Ctx, pOutput);

    SECLIB_MUTEX_UNLOCK();

    mbedtls_sha1_free(pSha1Ctx);
}

/*! *********************************************************************************
 * \brief  This function performs all SHA1 steps on multiple bytes: initialize,
 *         update, finish, and update context data.
 *         The final hash value is stored at the provided output location.
 *
 * \param [in]       pData       Pointer to the input data
 * \param [in]       numBytes    Number of bytes to hash
 * \param [in,out]   pOutput     Pointer to the output location
 *
 ********************************************************************************** */
void SHA1_Hash(uint8_t *pData, uint32_t numBytes, uint8_t *pOutput)
{
    SECLIB_MUTEX_LOCK();

    mbedtls_sha1(pData, numBytes, pOutput);

    SECLIB_MUTEX_UNLOCK();
}
#endif /* defined(MBEDTLS_SHA1_C) */

/*! *********************************************************************************
 * \brief  This function allocates a memory buffer for a SHA256 context structure
 *
 * \return    Address of the SHA256 context buffer
 *            Deallocate using SHA256_FreeCtx()
 *
 ********************************************************************************** */
void *SHA256_AllocCtx(void)
{
    void *sha256Ctx = MEM_BufferAlloc(sizeof(mbedtls_sha256_context));

    return sha256Ctx;
}

/*! *********************************************************************************
 * \brief  This function deallocates the memory buffer for the SHA256 context structure
 *
 * \param [in]    pContext    Address of the SHA256 context buffer
 *
 ********************************************************************************** */
void SHA256_FreeCtx(void *pContext)
{
    (void)MEM_BufferFree(pContext);
}
/*! *********************************************************************************
 * \brief  This function clones SHA256 context.
 *         Make sure the size of the allocated destination context buffer is appropriate.
 *
 * \param [in]    pDestCtx    Address of the destination SHA256 context
 * \param [in]    pSourceCtx  Address of the source SHA256 context
 *
 ********************************************************************************** */
void SHA256_CloneCtx(void *pDestCtx, void *pSourceCtx)
{
    mbedtls_sha256_clone(pDestCtx, pSourceCtx);
}

/*! *********************************************************************************
 * \brief  This function initializes the SHA256 context data
 *
 * \param [in]    pContext    Pointer to the SHA256 context data
 *                            Allocated using SHA256_AllocCtx()
 *
 ********************************************************************************** */
void SHA256_Init(void *pContext)
{
    int                     result;
    mbedtls_sha256_context *pSha256Ctx = (mbedtls_sha256_context *)pContext;

    SECLIB_MUTEX_LOCK();

    mbedtls_sha256_init(pSha256Ctx);
    result = mbedtls_sha256_starts_ret(pSha256Ctx, 0);
    if (result != 0)
    {
        assert(0);
    }

    SECLIB_MUTEX_UNLOCK();
}

/*! *********************************************************************************
 * \brief  This function performs SHA256 on multiple bytes and updates the context data
 *
 * \param [in]    pContext    Pointer to the SHA256 context data
 *                            Allocated using SHA256_AllocCtx()
 * \param [in]    pData       Pointer to the input data
 * \param [in]    numBytes    Number of bytes to hash
 *
 ********************************************************************************** */
void SHA256_HashUpdate(void *pContext, const uint8_t *pData, uint32_t numBytes)
{
    int                     result;
    mbedtls_sha256_context *pSha256Ctx = (mbedtls_sha256_context *)pContext;

    SECLIB_MUTEX_LOCK();

    result = mbedtls_sha256_update_ret(pSha256Ctx, pData, numBytes);
    if (result != 0)
    {
        assert(0);
    }

    SECLIB_MUTEX_UNLOCK();
}

/*! *********************************************************************************
 * \brief  This function performs SHA256 on the last bytes of data and updates the context data.
 *         The final hash value is stored at the provided output location.
 *
 * \param [in]       pContext    Pointer to the SHA256 context data
 *                               Allocated using SHA256_AllocCtx()
 * \param [in,out]   pOutput     Pointer to the output location
 *
 ********************************************************************************** */
void SHA256_HashFinish(void *pContext, uint8_t *pOutput)
{
    int                     result;
    mbedtls_sha256_context *pSha256Ctx = (mbedtls_sha256_context *)pContext;

    SECLIB_MUTEX_LOCK();

    result = mbedtls_sha256_finish_ret(pSha256Ctx, pOutput);
    if (result != 0)
    {
        assert(0);
    }

    SECLIB_MUTEX_UNLOCK();

    mbedtls_sha256_free(pSha256Ctx);
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
    int result;
    SECLIB_MUTEX_LOCK();

    result = mbedtls_sha256_ret(pData, numBytes, pOutput, 0);
    if (result != 0)
    {
        assert(0);
    }
    (void)result;

    SECLIB_MUTEX_UNLOCK();
}

/*! *********************************************************************************
 * \brief  This function allocates a memory buffer for a HMAC SHA256 context structure
 *
 * \return    Address of the HMAC SHA256 context buffer
 *            Deallocate using HMAC_SHA256_FreeCtx()
 *
 ********************************************************************************** */
void *HMAC_SHA256_AllocCtx(void)
{
    void *mdHmacSha256Ctx = MEM_BufferAlloc(sizeof(mbedtls_md_context_t));

    return mdHmacSha256Ctx;
}

/*! *********************************************************************************
 * \brief  This function deallocates the memory buffer for the HMAC SHA256 context structure
 *
 * \param [in]    pContext    Address of the HMAC SHA256 context buffer
 *
 ********************************************************************************** */
void HMAC_SHA256_FreeCtx(void *pContext)
{
    (void)MEM_BufferFree(pContext);
}

/* HMAC_SHA256 is used by the Gen FSK AKE controller */
/*! *********************************************************************************
 * \brief  This function performs the initialization of the HMAC SHA256 context data
 *
 * \param [in]    pContext    Pointer to the HMAC SHA256 context data
 *                            Allocated using HMAC_SHA256_AllocCtx()
 * \param [in]    pKey        Pointer to the HMAC key
 * \param [in]    keyLen      Length of the HMAC key in bytes
 *
 ********************************************************************************** */
void HMAC_SHA256_Init(void *pContext, const uint8_t *pKey, uint32_t keyLen)
{
    int                      result;
    mbedtls_md_context_t *   pMdHmacSha256Ctx = (mbedtls_md_context_t *)pContext;
    const mbedtls_md_info_t *pMdInfo;

    pMdInfo = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);

    mbedtls_md_init(pMdHmacSha256Ctx);
    result = mbedtls_md_setup(pMdHmacSha256Ctx, pMdInfo, 1);
    if (result != 0)
    {
        assert(0);
    }

    SECLIB_MUTEX_LOCK();

    result = mbedtls_md_hmac_starts(pMdHmacSha256Ctx, pKey, keyLen);
    if (result != 0)
    {
        assert(0);
    }

    SECLIB_MUTEX_UNLOCK();
}

/*! *********************************************************************************
 * \brief  This function performs HMAC update with the input data.
 *
 * \param [in]    pContext    Pointer to the HMAC SHA256 context data
 *                            Allocated using HMAC_SHA256_AllocCtx()
 * \param [in]    pData       Pointer to the input data
 * \param [in]    numBytes    Number of bytes to hash
 *
 ********************************************************************************** */
void HMAC_SHA256_Update(void *pContext, const uint8_t *pData, uint32_t numBytes)
{
    int                   result;
    mbedtls_md_context_t *pMdHmacSha256Ctx = (mbedtls_md_context_t *)pContext;

    SECLIB_MUTEX_LOCK();

    result = mbedtls_md_hmac_update(pMdHmacSha256Ctx, pData, numBytes);
    if (result != 0)
    {
        assert(0);
    }

    SECLIB_MUTEX_UNLOCK();
}

/*! *********************************************************************************
 * \brief  This function finalizes the HMAC SHA256 computation and clears the context data.
 *         The final hash value is stored at the provided output location.
 *
 * \param [in]       pContext    Pointer to the HMAC SHA256 context data
 *                               Allocated using HMAC_SHA256_AllocCtx()
 * \param [in,out]   pOutput     Pointer to the output location
 *
 ********************************************************************************** */
void HMAC_SHA256_Finish(void *pContext, uint8_t *pOutput)
{
    int                   result;
    mbedtls_md_context_t *pMdHmacSha256Ctx = (mbedtls_md_context_t *)pContext;

    SECLIB_MUTEX_LOCK();

    result = mbedtls_md_hmac_finish(pMdHmacSha256Ctx, pOutput);
    if (result != 0)
    {
        assert(0);
    }

    SECLIB_MUTEX_UNLOCK();

    mbedtls_md_free(pMdHmacSha256Ctx);
}

/*! *********************************************************************************
 * \brief  This function performs all HMAC SHA256 steps on multiple bytes: initialize,
 *         update, finish, and update context data.
 *         The final HMAC value is stored at the provided output location.
 *
 * \param [in]       pKey        Pointer to the HMAC key
 * \param [in]       keyLen      Length of the HMAC key in bytes
 * \param [in]       pData       Pointer to the input data
 * \param [in]       numBytes    Number of bytes to perform HMAC on
 * \param [in,out]   pOutput     Pointer to the output location
 *
 ********************************************************************************** */
void HMAC_SHA256(const uint8_t *pKey, uint32_t keyLen, const uint8_t *pData, uint32_t numBytes, uint8_t *pOutput)
{
    int                      result;
    const mbedtls_md_info_t *pMdInfo;

    pMdInfo = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);

    SECLIB_MUTEX_LOCK();

    result = mbedtls_md_hmac(pMdInfo, pKey, keyLen, pData, numBytes, pOutput);
    if (result != 0)
    {
        assert(0);
    }

    SECLIB_MUTEX_UNLOCK();
}

static void ecp_coordinate_copy(ec_p256_coordinate *dest, const uint8_t *src)
{
    FLib_MemCpy(dest->raw_8bit, src, ECP256_COORDINATE_LEN);
}

static void ecp_coordinate_copy_and_change_endiannes(ec_p256_coordinate *dest, const uint8_t *src)
{
    FLib_MemCpyReverseOrder(dest->raw_8bit, src, ECP256_COORDINATE_LEN);
}

static void ecp_p256_copy(ecdhPoint_t *XY, const uint8_t *src)
{
    ecp_coordinate_copy(&XY->coord.X, &src[0]);
    ecp_coordinate_copy(&XY->coord.Y, &src[ECP256_COORDINATE_LEN]);
}

static void ecp_p256_copy_and_change_endiannes(ecdhPoint_t *XY, const uint8_t *src)
{
    ecp_coordinate_copy_and_change_endiannes(&XY->coord.X, &src[0]);
    ecp_coordinate_copy_and_change_endiannes(&XY->coord.Y, &src[ECP256_COORDINATE_LEN]);
}

static void ecp_coordinate_change_endianness(ec_p256_coordinate *coord)
{
    FLib_ReverseByteOrderInPlace(coord->raw_8bit, ECP256_COORDINATE_LEN);
}

#define LEGACY_ECDH

mbedtls_ecdh_context gEcdhCtx;
/************************************************************************************
 * \brief Generates a new ECDH P256 Private/Public key pair
 *
 * \return gSecSuccess_c or error
 *
 ************************************************************************************/
secResultType_t ECDH_P256_GenerateKeys(ecdhPublicKey_t *pOutPublicKey, ecdhPrivateKey_t *pOutPrivateKey)
{
#if mRevertEcdhKeys_d
    ecdhPublicKey_t  reversedPublicKey;
    ecdhPrivateKey_t reversedPrivateKey;
#endif /* mRevertEcdhKeys_d */
    fpRngPrng_t pfPrng;
    void *      pPrngCtx;

    mbedtls_ecp_group p256EcpGrp;
    mbedtls_ecp_group_init(&p256EcpGrp);

#ifdef LEGACY_ECDH
    mbedtls_mpi       p256EcpPrivateKey;
    mbedtls_ecp_point p256EcpPublicKey;
    mbedtls_mpi_init(&p256EcpPrivateKey);
    mbedtls_ecp_point_init(&p256EcpPublicKey);
#endif

    const mbedtls_ecp_curve_info *pP256CurveInfo = NULL;

    int result = 0;
    do
    {
        pfPrng = RNG_GetPrngFunc();
        if (NULL == pfPrng)
        {
            result = -1;
            break;
        }

        pPrngCtx = RNG_GetPrngContext();
        if (NULL == pPrngCtx)
        {
            result = -1;
            break;
        }

        pP256CurveInfo = mbedtls_ecp_curve_info_from_grp_id(MBEDTLS_ECP_DP_SECP256R1);
        result         = mbedtls_ecp_group_load(&p256EcpGrp, pP256CurveInfo->grp_id);
        if (result != 0)
        {
            assert(0);
        }

        SECLIB_MUTEX_LOCK();
#ifdef LEGACY_ECDH

        result = mbedtls_ecdh_gen_public(&p256EcpGrp, &p256EcpPrivateKey, &p256EcpPublicKey, pfPrng, pPrngCtx);
#if !mRevertEcdhKeys_d
        result =
            mbedtls_mpi_write_binary(&p256EcpPrivateKey, pOutPrivateKey->raw_8bit, sizeof(pOutPrivateKey->raw_8bit));
        if (result != 0)
        {
            assert(0);
            break;
        }

        result = mbedtls_mpi_write_binary(&p256EcpPublicKey.X, pOutPublicKey->components_8bit.x,
                                          sizeof(pOutPublicKey->components_8bit.x));
        if (result != 0)
        {
            assert(0);
            break;
        }

        result = mbedtls_mpi_write_binary(&p256EcpPublicKey.Y, pOutPublicKey->components_8bit.y,
                                          sizeof(pOutPublicKey->components_8bit.y));
        if (result != 0)
        {
            assert(0);
            break;
        }
#else  /* mRevertEcdhKeys_d */
        result = mbedtls_mpi_write_binary(&p256EcpPrivateKey, reversedPrivateKey.raw_8bit,
                                          sizeof(reversedPrivateKey.raw_8bit));
        if (result != 0)
        {
            assert(0);
            break;
        }
        FLib_MemCpyReverseOrder(pOutPrivateKey->raw_8bit, reversedPrivateKey.raw_8bit,
                                sizeof(pOutPrivateKey->raw_8bit));
        result = mbedtls_mpi_write_binary(&p256EcpPublicKey.X, reversedPublicKey.components_8bit.x,
                                          sizeof(reversedPublicKey.components_8bit.x));
        if (result != 0)
        {
            assert(0);
            break;
        }
        FLib_MemCpyReverseOrder(pOutPublicKey->components_8bit.x, reversedPublicKey.components_8bit.x,
                                sizeof(pOutPublicKey->components_8bit.x));
        result = mbedtls_mpi_write_binary(&p256EcpPublicKey.Y, reversedPublicKey.components_8bit.y,
                                          sizeof(reversedPublicKey.components_8bit.y));
        if (result != 0)
        {
            assert(0);
            break;
        }
        FLib_MemCpyReverseOrder(pOutPublicKey->components_8bit.y, reversedPublicKey.components_8bit.y,
                                sizeof(pOutPublicKey->components_8bit.y));
#endif /* mRevertEcdhKeys_d */

#else /* LEGACY_ECDH */

        size_t olen = 0;
        if ((result = mbedtls_ecp_group_load(&gEcdhCtx.grp, MBEDTLS_ECP_DP_SECP256R1)) != 0)
        {
            break;
        }

        uint8_t buf[100];
        result = mbedtls_ecdh_make_public(&gEcdhCtx, &olen, buf, sizeof(buf), pfPrng, pPrngCtx);

#if !mRevertEcdhKeys_d
        ecp_p256_copy((ecdhPoint_t *)pOutPublicKey, buf);
#else
        ecp_p256_copy_and_change_endiannes((ecdhPoint_t *)pOutPublicKey, buf);
#endif

#endif
        SECLIB_MUTEX_UNLOCK();

        if (result != 0)
        {
            break;
        }
        /* Write the generated Secret Key and Public Key to the designated output locations.
         * The sizes copied should correspond to the sizes of the keys for the P256 curve. */

    } while (0);

#ifdef LEGACY_ECDH
    mbedtls_ecp_group_free(&p256EcpGrp);
    mbedtls_mpi_free(&p256EcpPrivateKey);
    mbedtls_ecp_point_free(&p256EcpPublicKey);
#endif

    if (result != 0)
    {
        return gSecError_c;
    }
    else
    {
        return gSecSuccess_c;
    }
}

/************************************************************************************
 * \brief Generates a new ECDH P256 Private/Public key pair
 *
 * \return gSecSuccess_c or error
 *
 ************************************************************************************/
secResultType_t ECDH_P256_GenerateKeysSeg(computeDhKeyParam_t *pDhKeyData)
{
    secResultType_t result;

    if (pfSecLibMultCallback == NULL)
    {
        result = ECDH_P256_GenerateKeys(&pDhKeyData->outPoint, &pDhKeyData->privateKey);
    }
    else
    {
        /* Segmented EC point multiply not implemented for K3 */
        result = gSecError_c;
    }

    return result;
}

/************************************************************************************
 * \brief Computes the Diffie-Hellman Key for an ECDH P256 key pair.
 *
 * \return gSecSuccess_c or error
 *
 ************************************************************************************/
secResultType_t ECDH_P256_ComputeDhKey(const ecdhPrivateKey_t *pInPrivateKey,
                                       const ecdhPublicKey_t * pInPeerPublicKey,
                                       ecdhDhKey_t *           pOutDhKey,
                                       const bool_t            keepBlobDhKey)
{
    fpRngPrng_t       pfPrng;
    void *            pPrngCtx;
    mbedtls_ecp_group p256EcpGrp;
#if mRevertEcdhKeys_d
    ecdhPublicKey_t  reversedPeerPublicKey;
    ecdhPrivateKey_t reversedPrivateKey;
#endif /* mRevertEcdhKeys_d */

    mbedtls_mpi       p256EcpPrivateKey;
    mbedtls_ecp_point p256EcpPublicKey;
    mbedtls_mpi       sharedZ;
    /*! The Z coordinate of the P256 public key has the value positive 1.
     *  Only the X and Y coordinates should be stored and provided by the caller of this fucntion */
    const uint8_t publicZ = 1;

    const mbedtls_ecp_curve_info *pP256CurveInfo;

    int result = 0;

    (void)keepBlobDhKey;
    pfPrng = RNG_GetPrngFunc();
    if (NULL == pfPrng)
    {
        return gSecError_c;
    }

    pPrngCtx = RNG_GetPrngContext();
    if (NULL == pPrngCtx)
    {
        return gSecError_c;
    }

    mbedtls_ecp_group_init(&p256EcpGrp);
    mbedtls_mpi_init(&p256EcpPrivateKey);
    mbedtls_ecp_point_init(&p256EcpPublicKey);
    mbedtls_mpi_init(&sharedZ);

    pP256CurveInfo = mbedtls_ecp_curve_info_from_grp_id(MBEDTLS_ECP_DP_SECP256R1);
    result         = mbedtls_ecp_group_load(&p256EcpGrp, pP256CurveInfo->grp_id);
    if (result != 0)
    {
        assert(0);
    }

    /* Load the local Secret Key and remote Public Key to the appropriate local variables.
     * The sizes loaded should correspond to the sizes of the keys for the P256 curve. */
#if !mRevertEcdhKeys_d
#ifdef LEGACY_ECDH
    result = mbedtls_mpi_read_binary(&p256EcpPrivateKey, pInPrivateKey->raw_8bit, sizeof(pInPrivateKey->raw_8bit));
    if (result != 0)
    {
        assert(0);
    }
#endif
    result = mbedtls_mpi_read_binary(&p256EcpPublicKey.X, pInPeerPublicKey->components_8bit.x,
                                     sizeof(pInPeerPublicKey->components_8bit.x));
    if (result != 0)
    {
        assert(0);
    }
    result = mbedtls_mpi_read_binary(&p256EcpPublicKey.Y, pInPeerPublicKey->components_8bit.y,
                                     sizeof(pInPeerPublicKey->components_8bit.y));
    if (result != 0)
    {
        assert(0);
    }
#else /* mRevertEcdhKeys_d */
#ifdef LEGACY_ECDH

    FLib_MemCpyReverseOrder(reversedPrivateKey.raw_8bit, pInPrivateKey->raw_8bit, sizeof(reversedPrivateKey.raw_8bit));
    result =
        mbedtls_mpi_read_binary(&p256EcpPrivateKey, reversedPrivateKey.raw_8bit, sizeof(reversedPrivateKey.raw_8bit));
    if (result != 0)
    {
        assert(0);
    }
#endif
    FLib_MemCpyReverseOrder(reversedPeerPublicKey.components_8bit.x, pInPeerPublicKey->components_8bit.x,
                            sizeof(reversedPeerPublicKey.components_8bit.x));
    result = mbedtls_mpi_read_binary(&p256EcpPublicKey.X, reversedPeerPublicKey.components_8bit.x,
                                     sizeof(reversedPeerPublicKey.components_8bit.x));
    if (result != 0)
    {
        assert(0);
    }
    FLib_MemCpyReverseOrder(reversedPeerPublicKey.components_8bit.y, pInPeerPublicKey->components_8bit.y,
                            sizeof(reversedPeerPublicKey.components_8bit.y));
    result = mbedtls_mpi_read_binary(&p256EcpPublicKey.Y, reversedPeerPublicKey.components_8bit.y,
                                     sizeof(reversedPeerPublicKey.components_8bit.y));
    if (result != 0)
    {
        assert(0);
    }
#endif /* mRevertEcdhKeys_d */

    result = mbedtls_mpi_read_binary(&p256EcpPublicKey.Z, &publicZ, sizeof(publicZ));
    if (result != 0)
    {
        assert(0);
    }
#ifdef LEGACY_ECDH

    SECLIB_MUTEX_LOCK();

    result =
        mbedtls_ecdh_compute_shared(&p256EcpGrp, &sharedZ, &p256EcpPublicKey, &p256EcpPrivateKey, pfPrng, pPrngCtx);

    SECLIB_MUTEX_UNLOCK();
#else /* LEGACY_ECDH */
    uint8_t buf[100];
    size_t olen;

    SECLIB_MUTEX_LOCK();

    (void)mbedtls_ecp_copy(&gEcdhCtx.Qp, &p256EcpPublicKey);

    mbedtls_ecdh_calc_secret(&gEcdhCtx, &olen, buf, sizeof(buf), pfPrng, pPrngCtx);
    SECLIB_MUTEX_UNLOCK();

#endif
    /* Write the generated Shared Key to the designated output location.
     * The size copied should correspond to the size of the key for the P256 curve.
     * Only the X component is provided by the implementation thus it is the only one
     * written to the output location. */
    if (result == 0)
    {
#if !mRevertEcdhKeys_d
        result = mbedtls_mpi_write_binary(&sharedZ, pOutDhKey->components_8bit.x, sizeof(pOutDhKey->components_8bit.x));
        if (result != 0)
        {
            assert(0);
        }
#else  /* mRevertEcdhKeys_d */
        /* Use the reversed public key X component as a buffer for the reversed shared key. */
        result = mbedtls_mpi_write_binary(&sharedZ, reversedPeerPublicKey.components_8bit.x,
                                          sizeof(reversedPeerPublicKey.components_8bit.x));
        if (result != 0)
        {
            assert(0);
        }
        FLib_MemCpyReverseOrder(pOutDhKey->components_8bit.x, reversedPeerPublicKey.components_8bit.x,
                                sizeof(pOutDhKey->components_8bit.x));
#endif /* mRevertEcdhKeys_d */
    }

    mbedtls_ecp_group_free(&p256EcpGrp);
    mbedtls_mpi_free(&p256EcpPrivateKey);
    mbedtls_ecp_point_free(&p256EcpPublicKey);
    mbedtls_mpi_free(&sharedZ);

    if (result == MBEDTLS_ERR_ECP_INVALID_KEY)
    {
        return gSecInvalidPublicKey_c;
    }
    else if (result != 0)
    {
        return gSecError_c;
    }
    else
    {
        return gSecSuccess_c;
    }
}

/************************************************************************************
 * \brief Computes the Diffie-Hellman Key for an ECDH P256 key pair.
 *
 * \return gSecSuccess_c or error
 *
 ************************************************************************************/
secResultType_t ECDH_P256_ComputeDhKeySeg(computeDhKeyParam_t *pDhKeyData)
{
    secResultType_t result;

    if (pfSecLibMultCallback == NULL)
    {
        result =
            ECDH_P256_ComputeDhKey(&pDhKeyData->privateKey, &pDhKeyData->peerPublicKey, &pDhKeyData->outPoint, FALSE);
    }
    else
    {
        /* Segmented EC point multiply not working when the callback is not set */
        result = gSecError_c;
    }
    return result;
}

/************************************************************************************
 * \brief Free any data allocated in the input structure.
 *
 * \param[in]  pDhKeyData Pointer to the structure holding information about the
 *                        multiplication
 *
 * \return gSecSuccess_c or error
 *
 ************************************************************************************/
void ECDH_P256_FreeDhKeyData(computeDhKeyParam_t *pDhKeyData)
{
}

/************************************************************************************
 * \brief Function used to create the mac key and LTK using Bluetooth F5 algorithm
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
 * \param  [in] bSecure  use available extra security bus
 *
 * \retval gSecSuccess_c operation succeeded
 * \retval gSecError_c operation failed
 ************************************************************************************/
secResultType_t SecLib_GenerateBluetoothF5Keys(uint8_t *      pMacKey,
                                               uint8_t *      pLtk,
                                               const uint8_t *pW,
                                               const uint8_t *pN1,
                                               const uint8_t *pN2,
                                               const uint8_t  a1at,
                                               const uint8_t *pA1,
                                               const uint8_t  a2at,
                                               const uint8_t *pA2,
                                               const bool_t   bSecure)
{
    secResultType_t result     = gSecError_c;
    const uint8_t   f5KeyId[4] = {0x62, 0x74, 0x6c, 0x65}; /*!< Big Endian, "btle" */
    uint8_t         f5CmacBuffer[1 + 4 + 16 + 16 + 7 + 7 +
                         2]; /* Counter[1] || keyId[4] || N1[16] || N2[16] || A1[7] || A2[7] || Length[2] = 53 */

    if (bSecure == FALSE)
    {
        uint8_t       f5T[16]    = {0};
        const uint8_t f5Salt[16] = {0x6C, 0x88, 0x83, 0x91, 0xAA, 0xF5, 0xA5, 0x38,
                                    0x60, 0x37, 0x0B, 0xDB, 0x5A, 0x60, 0x83, 0xBE}; /*!< Big endian */

        uint8_t tempOut[16];

        /*! Check for NULL output pointers and return with proper status if this is the case. */
        if ((NULL == pMacKey) || (NULL == pLtk))
        {
#if defined(gSmDebugEnabled_d) && (gSmDebugEnabled_d == 1U)
            SmDebug_Log(gSmDebugFileSmCrypto_c, __LINE__, smDebugLogTypeError_c, 0);
#endif /* gSmDebugEnabled_d */
        }
        else
        {
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
        }
    }

    return result;
}

/************************************************************************************
 * \brief Function used to derive the Bluetooth SKD used in LL encryption
 *
 * \param  [in] pInSKD   pointer to the received SKD (16-byte array)
 * \param  [in] pLtkBlob pointer to the blob (40-byte array)
 * \param  [out] pOutSKD pointer to the resulted SKD (16-byte array)
 *
 * \retval gSecSuccess_c operation succeeded
 * \retval gSecError_c operation failed
 ************************************************************************************/
secResultType_t SecLib_DeriveBluetoothSKD(const uint8_t *pInSKD,
                                          const uint8_t *pLtkBlob,
                                          bool_t         bOpenKey,
                                          uint8_t *      pOutSKD)
{
    return gSecError_c;
}

/************************************************************************************
 * \brief Converts a plaintext symmetric key into a blob of blobType. Reverses key beforehand.
 *
 * \param[in]  pKey      Pointer to the key.
 *
 * \param[out] pBlob     Pointer to the blob (shall be allocated, 40 or 16, depending on blobType)
 *
 * \param[in]  blobType  Blob type.
 *
 * \return gSecSuccess_c or error
 *
 ************************************************************************************/
secResultType_t SecLib_ObfuscateKey(const uint8_t *pKey, uint8_t *pBlob, const uint8_t blobType)
{
    return gSecError_c;
}

/************************************************************************************
 * \brief Converts a blob of a symmetric key into the plaintext. Reverses key afterwards.
 *
 * \param[in]  pBlob    Pointer to the blob.
 *
 * \param[out] pKey     Pointer to the key.
 *
 * \return gSecSuccess_c or error
 *
 ************************************************************************************/
secResultType_t SecLib_DeobfuscateKey(const uint8_t *pBlob, uint8_t *pKey)
{
    return gSecError_c;
}

/*! *********************************************************************************
 * \brief  This function implements the SMP ah cryptographic toolbox function which calculates the
 *         hash part of a Resolvable Private Address.
 *
 * \param[out]  pHash  Pointer where the 24 bit hash value will be written.
 *
 * \param[in]  pKey  Pointer to the 128 bit key.
 *
 * \param[in]  pR   Pointer to the 24 bit random value (Prand).
 *                  The most significant bits of this field must be 0b01 for Resolvable Private Addresses.
 *
 * \param[in]  bIsKeyObfuscated   Specify if pKey is in plaintext or obfuscated.
 *
 * \retval  gSecSuccess_c  All operations were successful.
 * \retval  gSecError_c The call failed.
 *
 ********************************************************************************** */
secResultType_t SecLib_VerifyBluetoothAh(
    uint8_t *      pHash, /*!< 24 bit hash field of a Resolvable Private Address (output) */
    const uint8_t *pKey,  /*!< 128 bit key (pointer) */
    const uint8_t *pR,    /*!< 24 bit random part of a Resolvable private Address */
    const bool_t   bIsKeyObfuscated)
{
    const uint8_t temp[16] = {
        0x00, 0x00, 0x00, 0x00, 0x00,  0x00,  0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, pR[2], pR[1], pR[0]}; /*!< Initialize the r' value in the temporary location.
                                                       *   Initialize it reversed for AES. */
    uint8_t         tempOutHash[16];
    uint8_t         tempKey[16];
    secResultType_t result = gSecSuccess_c;

    /*! Check for NULL output pointers and return with proper status if this is the case. */
    if (NULL == pHash)
    {
        result = gSecError_c;
    }
    else
    {
        /* Regular operation with plaintext key */
        if (bIsKeyObfuscated == false)
        {
            /*! Reverse the Key and place it in a temporary location. */
            FLib_MemCpyReverseOrder(tempKey, (const uint8_t *)pKey, 16);

            /*! Compute the hash. */
            AES_128_Encrypt(temp, tempKey, tempOutHash);

            /*! Copy the relevant bytes to the output. */
            pHash[0] = tempOutHash[15];
            pHash[1] = tempOutHash[14];
            pHash[2] = tempOutHash[13];
        }
        /* Operation with blob */
        else
        {
            result = gSecError_c;
        }
    }
    return result;
}

#if defined(ECDH_SELF_TEST) && (ECDH_SELF_TEST == 2)

#include "fsl_debug_console.h"
sss_ecdh_context_t ecdhClient;
sss_ecdh_context_t ecdhServer;
#define TRACE(...)       \
    {                    \
        if (verbose)     \
            PRINTF(...); \
    }
int seclib_self_test(bool verbose)
{
    int      ret     = -1;
    uint8_t *wrk_buf = NULL;

    do
    {
        FLib_MemSet(&ecdhClient, 0, sizeof(ecdhClient));
        FLib_MemSet(&ecdhServer, 0, sizeof(ecdhServer));

        if (EC_P256_GenerateKeys(&ecdhServer.OwnPublicKey, &ecdhServer.PrivateKey) != 0)
        {
            TRACE("Server: Error generating SW ECDH Key Pair\n");
            break;
        }
        size_t wrk_buf_sz = 3 * ECP256_COORDINATE_LEN;
        wrk_buf           = MEM_BufferAlloc(wrk_buf_sz);
        if (wrk_buf == NULL)
        {
            TRACE("Allocation failure\n");
            break;
        }
        FLib_MemSet(wrk_buf, 0, sizeof(wrk_buf_sz));
        ecdhPublicKey_t OwnPublicKey = {0);
        ecdhPrivateKey_t *OwnPrivateKey = {0};
        ECDH_P256_GenerateKeys(&OwnPublicKey, &OwnPrivateKey);

        /* At this stage ecdhServer key pair is stored in LE format but ecdhClient in BE
            We have no visibility on the ecdhClient private key.
          */
        ecp_p256_copy(&ecdhServer.Qp, (const uint8_t *)&ecdhClient.OwnPublicKey.raw[0]);
        ecp_p256_copy_and_change_endiannes(&ecdhClient.Qp, (const uint8_t *)&ecdhServer.OwnPublicKey.raw[0]);

        FLib_MemSet(wrk_buf, 0, sizeof(wrk_buf_sz));

        if (sss_ecdh_calc_secret(&ecdhClient, wrk_buf, 3 * ECP256_COORDINATE_LEN, &ecdhClient.z) != kStatus_SSS_Success)
        {
            TRACE("Client: Error computing DH secret\n");
            break;
        }

        if (EC_P256_ComputeDhKey(&ecdhServer.PrivateKey, &ecdhServer.Qp, &ecdhServer.z) != 0)
        {
            TRACE("Server: Error computing DH secret\n");
            break;
        }
        ecp_coordinate_change_endianness(&ecdhServer.z.coord.X);

        if (FLib_MemCmp(&ecdhClient.z.coord.X, &ecdhServer.z.coord.X, ECP256_COORDINATE_LEN) != TRUE)
        {
            TRACE("DH failure\n");
            break;
        }

        TRACE("passed\n");
        ret = 0;
    } while (0);
    return ret;
}
#endif /* MBEDTLS_SELF_TEST == 1 */
