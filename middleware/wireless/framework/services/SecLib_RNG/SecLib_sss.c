/*! *********************************************************************************
 * Copyright 2022-2023 NXP
 * All rights reserved.
 *
 * \file
 *
 * This is the source file for the security module used by the connectivity stacks. The Security
 *    Module SecLib provides an abstraction from the Hardware to the upper layer.
 *    In this file, a wrapper to SSS component is implemented.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */

#include "fsl_device_registers.h"
#include "fsl_os_abstraction.h"
#include "fsl_component_mem_manager.h"
#include "fsl_sss_sscp.h"
#include "sss_crypto.h"

#include "FunctionLib.h"
#include "fwk_platform.h"
#include "fwk_platform_crypto.h"
#include "SecLib.h"
#include "RNG_Interface.h"
#include "fsl_debug_console.h"

#include "SecLib_ecp256.h"
#include "CryptoLibSW.h"

#include "fwk_config.h"

/*! *********************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
********************************************************************************** */
#if (defined(USE_RTOS) && (USE_RTOS > 0))
/* On ELE Sentinel, the mutex is mandatory */
#define gSecLibUseMutex_c TRUE
#else
#define gSecLibUseMutex_c FALSE
#endif

extern osa_status_t SecLibMutexCreate(void);
extern osa_status_t SecLibMutexLock(void);
extern osa_status_t SecLibMutexUnlock(void);

#define SECLIB_MUTEX_LOCK()   (void)SecLibMutexLock()
#define SECLIB_MUTEX_UNLOCK() (void)SecLibMutexUnlock()

#define KEY_ID_BLE0                        0x426c6530
#define ELE_S200_KEY_STORE_USER_ID_GENERIC 0

#ifndef gSecLibAllowLtkFromBlob_c
#define gSecLibAllowLtkFromBlob_c 0
#endif

/* Set all crypto operations needed  */
#define mSecLibKeyPropCryptoAlgoAll_c                                                                                 \
    SSS_KEYPROP_OPERATION_AES | SSS_KEYPROP_OPERATION_MAC | SSS_KEYPROP_OPERATION_AEAD | SSS_KEYPROP_OPERATION_ASYM | \
        SSS_KEYPROP_OPERATION_KDF

/*
 * __DSP_PRESENT is defined in the device specific file, however avoid use of __DSP_PRESENT to avoid
 * a dependency with SDK.
 * It is likely to be present on all Core M33, Core M7 and Core M4 devices.
 * Nonetheless RW61x was designed without ARM DSP extension, in which case avoid defining
 * gSecLibUseDspExtension_d.
 */

#ifndef gSecLibUseDspExtension_d
#define gSecLibUseDspExtension_d 0
#endif

/*! *********************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
********************************************************************************** */

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
#if gSecLibUseMutex_c
/*! Mutex used to protect the AES Context when an RTOS is used. */
static OSA_MUTEX_HANDLE_DEFINE(mSecLibMutexId);
#endif /* gSecLibUseMutex_c */

static sss_ecp256_context_t  g_ECP_KeyPair;
static sss_ecp256_context_t *pECPKeyPair = ((void *)0);

static size_t  ecdhKeyPairBlobSize = (3 * ECP256_COORDINATE_LEN) + BLOB_DATA_OVERLAY_BYTE_LEN;
static uint8_t ecdhKeyPairBlob[(3 * ECP256_COORDINATE_LEN) + BLOB_DATA_OVERLAY_BYTE_LEN];

static bool_t IsSecLibEcdhContextInit = false;

/*! *********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */

/*! Callback used to offload Security steps onto application message queue. When it is not set the
 * multiplication is done using SecLib means */
extern secLibCallback_t pfSecLibMultCallback;

secLibCallback_t pfSecLibMultCallback = ((void *)0);

/*! *********************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
********************************************************************************** */

static sss_status_t ELKE_BLE_SM_F5_DeriveKeysSecure(sss_sscp_object_t *pPubDhKey,
                                                    const uint8_t     *pDerivationDataMacKey,
                                                    const uint8_t     *pDerivationDataLTK,
                                                    uint8_t           *pMacKey,
                                                    uint8_t           *pLTKBlob);

/*! *********************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
********************************************************************************** */

#if gSecLibUseDspExtension_d
static bool ECP256_LePointValid(const ecp256Point_t *P)
{
    ecp256Point_t tmp;
    ECP256_PointCopy_and_change_endianness(tmp.raw, P->raw);
    return ECP256_PointValid(&tmp);
}
#else

extern bool_t EcP256_IsPointOnCurve(const uint32_t *X, const uint32_t *Y);

static bool ECP256_LePointValid(const ecp256Point_t *P)
{
    return EcP256_IsPointOnCurve((const uint32_t *)&P->components_32bit.x[0],
                                 (const uint32_t *)&P->components_32bit.y[0]);
}
#endif

/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */
osa_status_t SecLibMutexCreate(void)
{
    osa_status_t st = KOSA_StatusSuccess;
#if gSecLibUseMutex_c
    static bool seclib_mutex_created = false;

    if (!seclib_mutex_created)
    {
        /*! Initialize the SecLib Mutex here. If not already done by RNG module */
        st = OSA_MutexCreate((osa_mutex_handle_t)mSecLibMutexId);

        if (KOSA_StatusSuccess != st)
        {
            assert(0);
        }
        else
        {
            seclib_mutex_created = true;
        }
    }
#endif
    return st;
}
osa_status_t SecLibMutexLock(void)
{
#if gSecLibUseMutex_c
    return OSA_MutexLock((osa_mutex_handle_t)mSecLibMutexId, osaWaitForever_c);
#else
    return KOSA_StatusSuccess;
#endif
}

osa_status_t SecLibMutexUnlock(void)
{
#if gSecLibUseMutex_c
    return OSA_MutexUnlock((osa_mutex_handle_t)mSecLibMutexId);
#else
    return KOSA_StatusSuccess;
#endif
}

/*! *********************************************************************************
 * \brief  This function performs initialization of the cryptographic HW acceleration.
 *
 ********************************************************************************** */

void SecLib_Init(void)
{
    /* Initialize cryptographic hardware.*/
    (void)PLATFORM_InitCrypto();

    (void)SecLibMutexCreate();
}

/*! *********************************************************************************
 * \brief  This function performs initialization of the cryptografic HW acceleration.
 *
 ********************************************************************************** */
void SecLib_ReInit(void)
{
    IsSecLibEcdhContextInit = false;
    (void)PLATFORM_ResetCrypto();
}

/*! *********************************************************************************
 * \brief  This function will allow reinitizialize the cryptographic HW acceleration
 * next time we need it, typically after lowpower mode.
 *
 ********************************************************************************** */
void SecLib_DeInit(void)
{
    (void)PLATFORM_TerminateCrypto();
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
    return;
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
    status_t      ret;
    aes_context_t ctx;

    SECLIB_MUTEX_LOCK();

    ret = SSS_aes_operation(&ctx, pInput, 16, NULL, pKey, AES_128_KEY_BITS, pOutput, true, kAlgorithm_SSS_AES_ECB);
    if (ret != kStatus_Success)
    {
        assert(0);
    }

    SECLIB_MUTEX_UNLOCK();
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
    status_t      ret;
    aes_context_t ctx;

    SECLIB_MUTEX_LOCK();

    ret = SSS_aes_operation(&ctx, pInput, 16, NULL, pKey, AES_128_KEY_BITS, pOutput, false, kAlgorithm_SSS_AES_ECB);
    if (ret != kStatus_Success)
    {
        assert(0);
    }

    SECLIB_MUTEX_UNLOCK();
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
    status_t      ret;
    aes_context_t ctx;

    /* If the input length is not a multiple of AES 128 block size return */
    if (!((inputLen == 0U) || ((inputLen % AES_128_BLOCK_SIZE) != 0U)))
    {
        SECLIB_MUTEX_LOCK();
        ret = SSS_aes_operation(&ctx, pInput, inputLen, NULL, pKey, AES_128_KEY_BITS, pOutput, true,
                                kAlgorithm_SSS_AES_ECB);
        if (ret != kStatus_Success)
        {
            assert(0);
        }
        SECLIB_MUTEX_UNLOCK();
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
    status_t      ret;
    aes_context_t ctx;

    /* If the input length is not a multiple of AES 128 block size return */
    if (!((inputLen == 0U) || ((inputLen % AES_128_BLOCK_SIZE) != 0U)))
    {
        SECLIB_MUTEX_LOCK();

        ret = SSS_aes_operation(&ctx, pInput, inputLen, pInitVector, pKey, AES_128_KEY_BITS, pOutput, true,
                                kAlgorithm_SSS_AES_CBC);
        if (ret != kStatus_Success)
        {
            assert(0);
        }

        SECLIB_MUTEX_UNLOCK();
    }
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
    uint32_t      newLen = 0;
    status_t      ret;
    aes_context_t ctx;

    /* compute new length */
    newLen = inputLen + (AES_128_BLOCK_SIZE - (inputLen & (AES_128_BLOCK_SIZE - 1U)));
    /* pad the input buffer with 1 bit of 1 and trail of 0's from inputLen to newLen */
    for (uint32_t idx = 0U; idx < ((newLen - inputLen) - 1U); idx++)
    {
        pInput[newLen - 1U - idx] = 0x00U;
    }
    pInput[inputLen] = 0x80U;

    SECLIB_MUTEX_LOCK();

    ret = SSS_aes_operation(&ctx, pInput, newLen, pInitVector, pKey, AES_128_KEY_BITS, pOutput, true,
                            kAlgorithm_SSS_AES_CBC);
    if (ret != kStatus_Success)
    {
        assert(0);
    }

    SECLIB_MUTEX_UNLOCK();

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
    uint32_t      newLen = inputLen;
    status_t      ret;
    aes_context_t ctx;

    SECLIB_MUTEX_LOCK();

    ret = SSS_aes_operation(&ctx, pInput, inputLen, pInitVector, pKey, AES_128_KEY_BITS, pOutput, false,
                            kAlgorithm_SSS_AES_CBC);
    if (ret != kStatus_Success)
    {
        assert(0);
    }
    SECLIB_MUTEX_UNLOCK();

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
    int           ret;
    uint32_t      ctrOffset                     = 0U;
    uint8_t       streamBlk[AES_128_BLOCK_SIZE] = {0U};
    aes_context_t ctx;

    SECLIB_MUTEX_LOCK();

    /* The lenght of the input does not need to be a multiple of AES 128 block size */
    ret = SSS_aes128_CTR_operation(&ctx, pInput, inputLen, pCounter, pKey, pOutput, true, streamBlk,
                                   (size_t *)&ctrOffset);
    if (ret != kStatus_Success)
    {
        assert(0);
    }
    SECLIB_MUTEX_UNLOCK();
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
void AES_128_OFB(
    const uint8_t *pInput, const uint32_t inputLen, uint8_t *pInitVector, const uint8_t *pKey, uint8_t *pOutput)
{
    /* If the input length is not a multiple of AES 128 block size return */
    if (!((inputLen == 0U) || ((inputLen % AES_128_BLOCK_SIZE) != 0U)))
    {
        int           ret;
        aes_context_t ctx;

        SECLIB_MUTEX_LOCK();
        ret = SSS_aes_operation(&ctx, pInput, inputLen, pInitVector, pKey, AES_128_KEY_BITS, pOutput, true,
                                kAlgorithm_SSS_AES_OFB);
        if (ret != kStatus_Success)
        {
            assert(0);
        }
        SECLIB_MUTEX_UNLOCK();
    }
}
#endif

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
    status_t           result;
    cmac_aes_context_t cmac_ctx;

    SECLIB_MUTEX_LOCK();

    result = SSS_aes_cmac(&cmac_ctx, pKey, AES_128_KEY_BITS, pInput, inputLen, pOutput);
    if (result != kStatus_Success)
    {
        assert(0);
    }

    SECLIB_MUTEX_UNLOCK();
}

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
    uint8_t reversedBlock[AES_128_BLOCK_SIZE] = {0};
#if defined(ELE_FEATURE_MAC_MULTIPART) && (ELE_FEATURE_MAC_MULTIPART != 0)
    /* ELE supports partial operations with CMAC update only on KW47 not on KW45 */
    status_t           result;
    cmac_aes_context_t cmac_ctx; /* structure is 64 bytes */

    do
    {
        /* Initialize context */
        result = SSS_aes_cmac_starts(&cmac_ctx, pKey, AES_128_KEY_BYTE_LEN);
        if (result != kStatus_Success)
        {
            break;
        }

        /* Initialize CMAC multipart operation */
        result = SSS_aes_cmac_init(&cmac_ctx);
        if (result != kStatus_Success)
        {
            break;
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
            /* Copy the input block to the reversed CMAC input buffer */
            FLib_MemCpyReverseOrder(reversedBlock, pInput, currentCmacInputBlkLen);

            SECLIB_MUTEX_LOCK();

            result = SSS_aes_cmac_update(&cmac_ctx, (const unsigned char *)&reversedBlock, currentCmacInputBlkLen);
            SECLIB_MUTEX_UNLOCK();

            if (result != kStatus_Success)
            {
                break;
            }

        } while (inputLen != 0U);

        if (result != kStatus_Success)
        {
            break;
        }
        SECLIB_MUTEX_LOCK();

        result = SSS_aes_cmac_finish(&cmac_ctx, pOutput);

        SECLIB_MUTEX_UNLOCK();

    } while (false);

    if (result != kStatus_Success)
    {
        assert(0);
    }
    /*CALL THIS AT THE END*/
    SSS_aes_cmac_free(&cmac_ctx);

#else  /* ELE_FEATURE_MAC_MULTIPART */
    /* Workaround to compensate for issue with CMAC partial update not working */
    /* Requires the allocation of a buffer the size of the input : normally called with 32 byte input */
    uint8_t *reversedMsg = NULL;
    if (inputLen <= AES_128_BLOCK_SIZE)
    {
        reversedMsg = &reversedBlock[0];
    }
    else
    {
        reversedMsg = (uint8_t *)MEM_BufferAlloc(((inputLen + 15U) >> 4) << 4);
        /* Some MEM_BufferAlloc implementations return a NULL pointer for a 0 length allocation */
    }

    if (reversedMsg != NULL)
    {
        uint8_t *p   = &reversedMsg[0];
        size_t   cnt = inputLen;
        pInput += cnt;
        do
        {
            uint32_t currentCmacInputBlkLen = 0;
            if (cnt < AES_128_BLOCK_SIZE)
            {
                /* If this is the first and single block it is legal for it to have an input length of 0
                 * in which case nothing will be copied in the reversed CMAC input buffer. */
                currentCmacInputBlkLen = cnt;
            }
            else
            {
                currentCmacInputBlkLen = AES_128_BLOCK_SIZE;
            }
            pInput -= currentCmacInputBlkLen;
            cnt -= currentCmacInputBlkLen;
            /* Copy the input block to the reversed CMAC input buffer */
            FLib_MemCpyReverseOrder(p, pInput, currentCmacInputBlkLen);

            p += currentCmacInputBlkLen;
        } while (cnt != 0U);

        AES_128_CMAC(reversedMsg, inputLen, pKey, pOutput);
        if (inputLen > AES_128_BLOCK_SIZE)
        {
            /* reversedMsg was allocated */
            (void)MEM_BufferFree(reversedMsg);
        }
    }

#endif /* ELE_FEATURE_MAC_MULTIPART */
}

/*! *********************************************************************************
 * \brief  This function performs AES 128 CMAC Pseudo-Random Function (AES-CMAC-PRF-128),
 *         according to rfc4615, on a message block.
 * \details The AES-CMAC-PRF-128 algorithm behaves similar to teh AES CMAC 128 algorithm
 *          but removes 128 bit key size restriction.
 * \param[in]  pInput Pointer to the location of the input message.
 * \param[in]  inputLen Length of the input message in bytes.
 * \param[in]  pVarKey Pointer to the location of the variable length key.
 * \param[in]  varKeyLen Length of the input key in bytes
 * \param[out]  pOutput Pointer to the location to store the 16-byte pseudo random variable.
 *
 ********************************************************************************** */
void AES_CMAC_PRF_128(
    const uint8_t *pInput, uint32_t inputLen, const uint8_t *pVarKey, uint32_t varKeyLen, uint8_t *pOutput)
{
    int result;
    SECLIB_MUTEX_LOCK();

    cmac_aes_context_t cmac_ctx;
    result = SSS_aes_cmac_prf_128(&cmac_ctx, pVarKey, varKeyLen, pInput, inputLen, pOutput);
    if (result != kStatus_Success)
    {
        assert(0);
    }
    SECLIB_MUTEX_UNLOCK();
}

#if gSecLibAesEaxEnable_d
/*! *********************************************************************************
 * \brief  This function performs AES-128-EAX encryption on a message block.
 * \param[in]  pInput Pointer to the location of the input message.
 * \param[in]  inputLen Length of the input message in bytes.
 * \param[in]  pNonce Pointer to the location of the nonce.
 * \param[in]  nonceLen Nonce length in bytes.
 * \param[in]  pHeader Pointer to the location of header.
 * \param[in]  headerLen Header length in bytes.
 * \param[in]  pKey Pointer to the location of the 128-bit key.
 * \param[out]  pOutput Pointer to the location to store the 16-byte authentication code.
 * \param[out]  pTag Pointer to the location to store the 128-bit tag.
 *
 ********************************************************************************** */
secResultType_t AES_128_EAX_Encrypt(const uint8_t *pInput,
                                    uint32_t       inputLen,
                                    const uint8_t *pNonce,
                                    uint32_t       nonceLen,
                                    const uint8_t *pHeader,
                                    uint8_t        headerLen,
                                    const uint8_t *pKey,
                                    uint8_t       *pOutput,
                                    uint8_t       *pTag)
{
    uint8_t        *buf;
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
 * \param[in]  inputLen Length of the input message in bytes.
 * \param[in]  pNonce Pointer to the location of the nonce.
 * \param[in]  nonceLen Nonce length in bytes.
 * \param[in]  pHeader Pointer to the location of header.
 * \param[in]  headerLen Header length in bytes.
 * \param[in]  pKey Pointer to the location of the 128-bit key.
 * \param[out]  pOutput Pointer to the location to store the 16-byte authentication code.
 * \param[out]  pTag Pointer to the location to store the 128-bit tag.
 *
 ********************************************************************************** */
secResultType_t AES_128_EAX_Decrypt(const uint8_t *pInput,
                                    uint32_t       inputLen,
                                    const uint8_t *pNonce,
                                    uint32_t       nonceLen,
                                    const uint8_t *pHeader,
                                    uint8_t        headerLen,
                                    const uint8_t *pKey,
                                    uint8_t       *pOutput,
                                    uint8_t       *pTag)
{
    uint8_t        *buf;
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
#endif

/*! *********************************************************************************
 * \brief  This function performs AES-128-CCM on a message block.
 *
 * \param[in]  pInput       Pointer to the location of the input message (plaintext or ciphertext).
 * \param[in]  inputLen     Length of the input plaintext in bytes when encrypting.
 *                          Length of the input ciphertext without the MAC length when decrypting.
 * \param[in]  pAuthData    Pointer to the additional authentication data.
 * \param[in]  authDataLen  Length of additional authentication data.
 * \param[in]  pNonce       Pointer to the Nonce.
 * \param[in]  nonceSize    The size of the nonce (7-13).
 * \param[in]  pKey         Pointer to the location of the 128-bit key.
 * \param[out]  pOutput     Pointer to the location to store the plaintext data when decrypting.
 *                          Pointer to the location to store the ciphertext data when encrypting.
 * \param[out]  pCbcMac     Pointer to the location to store the Message Authentication Code (MAC) when encrypting.
 *                          Pointer to the location where the received MAC can be found when decrypting.
 * \param[out]  macSize     The size of the MAC.
 * \param[out]  flags       Select encrypt/decrypt operations (gSecLib_CCM_Encrypt_c, gSecLib_CCM_Decrypt_c)
 *
 * \return 0 if encryption/decryption was successful; otherwise, error code for failed encryption/decryption
 *
 * \remarks At decryption, MIC fail is also signaled by returning a non-zero value.
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
    uint8_t           st = (uint8_t)gSecError_c;
    sss_ccm_context_t ccm_ctx;

    FLib_MemSet(&ccm_ctx, 0, sizeof(sss_ccm_context_t));

    do
    {
        int32_t status;

        status = SSS_ccm_setkey(&ccm_ctx, pKey, 128);
        if (status != kStatus_Success)
        {
            break;
        }

        SECLIB_MUTEX_LOCK();

        if ((flags & gSecLib_CCM_Decrypt_c) != 0U)
        {
            status = SSS_ccm_auth_decrypt(&ccm_ctx, inputLen, pNonce, nonceSize, pAuthData, authDataLen, pInput,
                                          pOutput, pCbcMac, macSize);
        }
        else
        {
            status = SSS_ccm_encrypt_and_tag(&ccm_ctx, inputLen, pNonce, nonceSize, pAuthData, authDataLen, pInput,
                                             pOutput, pCbcMac, macSize);
        }

        SECLIB_MUTEX_UNLOCK();

        SSS_ccm_free(&ccm_ctx);
        if (status != kStatus_Success)
        {
            break;
        }

        st = (uint8_t)gSecSuccess_c;
    } while (false);
    return st;
}

/*! *********************************************************************************
 * \brief  This function allocates a memory buffer for a SHA256 context structure
 *
 * \return    Address of the SHA256 context buffer
 *            Deallocate using SHA256_FreeCtx()
 *
 ********************************************************************************** */
void *SHA256_AllocCtx(void)
{
    void *p_ctx = MEM_BufferAlloc(sizeof(sss_sha256_context_t));

    return p_ctx;
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
    SSS_sha256_clone(pDestCtx, pSourceCtx);
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
    status_t              result;
    sss_sha256_context_t *pSha256Ctx = (sss_sha256_context_t *)pContext;

    SECLIB_MUTEX_LOCK();

    SSS_sha256_init(pSha256Ctx);
    result = SSS_sha256_starts_ret(pSha256Ctx, false);
    if (result != kStatus_Success)
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
    status_t              result;
    sss_sha256_context_t *pSha256Ctx = (sss_sha256_context_t *)pContext;

    SECLIB_MUTEX_LOCK();

    result = SSS_sha256_update_ret(pSha256Ctx, pData, numBytes);
    if (result != kStatus_Success)
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
    status_t              result;
    sss_sha256_context_t *pSha256Ctx = (sss_sha256_context_t *)pContext;

    SECLIB_MUTEX_LOCK();

    result = SSS_sha256_finish_ret(pSha256Ctx, pOutput);
    if (result != kStatus_Success)
    {
        assert(0);
    }

    SECLIB_MUTEX_UNLOCK();

    SSS_sha256_free(pSha256Ctx);
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
    status_t result;
    SECLIB_MUTEX_LOCK();

    result = SSS_sha256_ret(pData, numBytes, pOutput, false);
    if (result != kStatus_Success)
    {
        assert(0);
    }
    (void)result;

    SECLIB_MUTEX_UNLOCK();
}

/* HMAC_SHA256 is used by the Gen FSK AKE controller */

/*! *********************************************************************************
 * \brief  This function allocates a memory buffer for a HMAC SHA256 context structure
 *
 * \return    Address of the HMAC SHA256 context buffer
 *            Deallocate using HMAC_SHA256_FreeCtx()
 *
 ********************************************************************************** */
void *HMAC_SHA256_AllocCtx(void)
{
    void *mdhmac_sha256Ctx = MEM_BufferAlloc(sizeof(sss_hmac_sha256_context_t));

    return mdhmac_sha256Ctx;
}

/*! *********************************************************************************
 * \brief  This function deallocates the memory buffer for the HMAC SHA256 context structure
 *
 * \param [in]    pContext    Address of the HMAC SHA256 context buffer
 *
 ********************************************************************************** */
void HMAC_SHA256_FreeCtx(void *pContext)
{
    FLib_MemSet(pContext, 0, sizeof(sss_hmac_sha256_context_t));
    (void)MEM_BufferFree(pContext);
}

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
    SECLIB_MUTEX_LOCK();

    if (SSS_md_hmac_sha256_starts((sss_hmac_sha256_context_t *)pContext, pKey, keyLen) != kStatus_Success)
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
    SECLIB_MUTEX_LOCK();

    if (SSS_md_hmac_sha256_update((sss_hmac_sha256_context_t *)pContext, pData, numBytes) != kStatus_Success)
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
    SECLIB_MUTEX_LOCK();
    if (SSS_md_hmac_sha256_finish((sss_hmac_sha256_context_t *)pContext, pOutput) != kStatus_Success)
    {
        assert(0);
    }
    SECLIB_MUTEX_UNLOCK();
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
    status_t result;

    SECLIB_MUTEX_LOCK();
    sss_hmac_sha256_context_t hmac_ctx;

    result = SSS_md_hmac_sha256(&hmac_ctx, pKey, keyLen, pData, numBytes, pOutput);
    if (result != kStatus_Success)
    {
        assert(0);
    }
    (void)result;
    SECLIB_MUTEX_UNLOCK();
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

    uint8_t *wrk_buf = NULL;

    SECLIB_MUTEX_LOCK();
    do
    {
        size_t wrk_buf_sz;
        if (pECPKeyPair != NULL)
        {
            /* Once the key oject gets destroyed context is not ready anymore */
            IsSecLibEcdhContextInit = false;
            /* need to release previous allocated key */
            (void)SSS_KEY_OBJ_FREE(&pECPKeyPair->OwnKey);
            FLib_MemSet(pECPKeyPair, 0, sizeof(sss_ecp256_context_t));
            pECPKeyPair = NULL;
        }

        wrk_buf_sz = sizeof(ecdhPublicKey_t); /* 2 * ECP256_COORDINATE_LEN */
        wrk_buf    = MEM_BufferAlloc(wrk_buf_sz);
        if (wrk_buf == NULL)
        {
            RAISE_ERROR(ret, gSecAllocError_c);
        }
        g_ECP_KeyPair.keyId = KEY_ID_BLE0;

        if (sss_ecdh_make_public_ecp256_key(&g_ECP_KeyPair, wrk_buf, wrk_buf_sz) != kStatus_Success)
        {
            RAISE_ERROR(ret, gSecError_c);
        }
        pECPKeyPair = &g_ECP_KeyPair;

        if (sss_sscp_key_store_export_key(&g_keyStore, &pECPKeyPair->OwnKey, ecdhKeyPairBlob, &ecdhKeyPairBlobSize,
                                          kSSS_blobType_ELKE_blob) != kStatus_SSS_Success)
        {
            RAISE_ERROR(ret, gSecError_c);
        }

        IsSecLibEcdhContextInit = true;

        /* pubKey returned by SSS in Big-Endian format: return it as Low Endian */
        ECP256_PointCopy_and_change_endianness(pOutPublicKey->raw, &wrk_buf[0]);
        /* From S200 A1 on, the private key is not passed in plain text so the pOutPrivateKey remains uninitialized */
        FLib_MemCpy(pOutPrivateKey->raw_8bit, &pECPKeyPair->PrivateKey, sizeof(ecdhPrivateKey_t));

    } while (false);
    SECLIB_MUTEX_UNLOCK();

    (void)MEM_BufferFree(wrk_buf);

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
    secResultType_t ret     = gSecSuccess_c;
    uint8_t        *wrk_buf = NULL;
    SECLIB_MUTEX_LOCK();
    sss_ecdh_context_t ecdh_ctx = {0};

    ecdh_ctx.keepSharedSecret = keepBlobDhKey;
    do
    {
        ecdhPoint_t EcdhPubKey = {0U};
        size_t      wrk_buf_sz;

        if (pOutDhKey == NULL)
        {
            ret = gSecError_c;
            break;
        }
        if (pECPKeyPair == NULL)
        {
            ret = gSecError_c;
            break;
        }
        if (!ECP256_LePointValid(pInPeerPublicKey))
        {
            ret = gSecInvalidPublicKey_c;
            break;
        }

        if (IsSecLibEcdhContextInit == false)
        {
            if ((sss_ecdh_init_key(pECPKeyPair)) != kStatus_Success)
            {
                break;
            }

            if (kStatus_SSS_Success != sss_sscp_key_store_import_key(&g_keyStore, &pECPKeyPair->OwnKey, ecdhKeyPairBlob,
                                                                     ecdhKeyPairBlobSize, 3U * ECP256_COORDINATE_BITLEN,
                                                                     kSSS_blobType_ELKE_blob))
            {
                break;
            }
            IsSecLibEcdhContextInit = true;
        }
        wrk_buf_sz = 3u * ECP256_COORDINATE_LEN;
        wrk_buf    = MEM_BufferAlloc(wrk_buf_sz);
        if (wrk_buf == NULL)
        {
            RAISE_ERROR(ret, gSecAllocError_c);
        }
        ecdh_ctx.ecdh_key_pair = pECPKeyPair;

        uint8_t *pubkey = &EcdhPubKey.raw[0];
        ECP256_PointCopy_and_change_endianness(pubkey, (const uint8_t *)&pInPeerPublicKey->raw[0]);

        FLib_MemCpy(&ecdh_ctx.Qp, &EcdhPubKey, sizeof(ecdhPoint_t));

        if (sss_ecdh_calc_secret(&ecdh_ctx, wrk_buf, wrk_buf_sz) != kStatus_Success)
        {
            RAISE_ERROR(ret, gSecError_c);
        }
        ECP256_PointCopy_and_change_endianness(pOutDhKey->raw, wrk_buf);

        (void)MEM_BufferFree(wrk_buf);
    } while (false);

    SECLIB_MUTEX_UNLOCK();

    if (ret == gSecSuccess_c)
    {
        /* Keep DHKey object for later use */
        if (ecdh_ctx.keepSharedSecret == true)
        {
            FLib_MemCpy(pOutDhKey->raw, (void *)&ecdh_ctx.sharedSecret, sizeof(sss_sscp_object_t));
        }
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
 * \brief Free any data allocated in the input structure.
 *
 * \param[in]  pDhKeyData Pointer to the structure holding information about the
 *                        multiplication
 *
 * \return gSecSuccess_c or error
 *
 ************************************************************************************/
void ECDH_P256_FreeDhKeyDataSecure(computeDhKeyParam_t *pDhKeyData)
{
    /* turn into void* first to avoid MISRA 11.3 */
    void *pKeyData = &pDhKeyData->outPoint;
    (void)sss_sscp_key_object_free((sss_sscp_object_t *)pKeyData, kSSS_keyObjFree_KeysStoreNoDefragment);
}

/************************************************************************************
 * \brief Function used to create the mac key and LTK using Bluetooth F5 algorithm.
 *        Kigher security version all keys remain on security bus.
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
    secResultType_t result     = gSecError_c;
    const uint8_t   f5KeyId[4] = {0x62, 0x74, 0x6c, 0x65}; /*!< Big Endian, "btle" */
    uint8_t         f5CmacBuffer[1 + 4 + 16 + 16 + 7 + 7 +
                         2]; /* Counter[1] || keyId[4] || N1[16] || N2[16] || A1[7] || A2[7] || Length[2] = 53 */

    uint8_t f5CmacBuffer_Counter1[1 + 4 + 16 + 16 + 7 + 7 + 2]; /* Counter[1] || keyId[4] || N1[16] || N2[16] ||
                                                                       A1[7] || A2[7] || Length[2] = 53 */
    sss_status_t status = kStatus_SSS_Fail;
    union
    {
        sss_sscp_object_t *pSSSObject;
        const uint8_t     *pW;
    } tempF5;

    /*! Check for NULL output pointers and return with proper status if this is the case. */
    do
    {
        if ((NULL == pMacKey) || (NULL == pLtk) || (NULL == pN1) || (NULL == pN2) || (NULL == pA1) || (NULL == pA2))
        {
#if defined(gSmDebugEnabled_d) && (gSmDebugEnabled_d == 1U)
            SmDebug_Log(gSmDebugFileSmCrypto_c, __LINE__, smDebugLogTypeError_c, 0);
#endif /* gSmDebugEnabled_d */
            RAISE_ERROR(result, gSecError_c);
        }
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

        /*! Build the least significant part of the f5 input data to compute the LTK.
         *  It is identical to the most significant part with the exception of the counter. */
        FLib_MemCpy(f5CmacBuffer_Counter1, f5CmacBuffer, 53u);
        f5CmacBuffer_Counter1[0] = 1; /* Counter = 1 */

        tempF5.pW = pW;

        status = ELKE_BLE_SM_F5_DeriveKeysSecure(tempF5.pSSSObject, f5CmacBuffer, f5CmacBuffer_Counter1, pMacKey, pLtk);
        if (status != kStatus_SSS_Success)
        {
            RAISE_ERROR(result, gSecError_c);
        }
        result = gSecSuccess_c;
    } while (false);

    return result;
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
#if defined(gSmDebugEnabled_d) && (gSmDebugEnabled_d == 1U)
            SmDebug_Log(gSmDebugFileSmCrypto_c, __LINE__, smDebugLogTypeError_c, 0);
#endif /* gSmDebugEnabled_d */
            RAISE_ERROR(result, gSecError_c);
        }

        /*! Compute the f5 function key T using the predefined salt as key for AES-128-CAMC */
        AES_128_CMAC_LsbFirstInput((const uint8_t *)pW, 32, (const uint8_t *)f5Salt, f5T);

        /*! Build the most significant part of the f5 input data to compute the MacKey */
        f5CmacBuffer[0] = 0; /* Counter = 0 */
        FLib_MemCpy(&f5CmacBuffer[1], (const uint8_t *)f5KeyId, 4u);
        FLib_MemCpyReverseOrder(&f5CmacBuffer[5], (const uint8_t *)pN1, 16u);
        FLib_MemCpyReverseOrder(&f5CmacBuffer[21], (const uint8_t *)pN2, 16u);
        f5CmacBuffer[37] = 0x01U & a1at;
        FLib_MemCpyReverseOrder(&f5CmacBuffer[38], (const uint8_t *)pA1, 6u);
        f5CmacBuffer[44] = 0x01U & a2at;
        FLib_MemCpyReverseOrder(&f5CmacBuffer[45], (const uint8_t *)pA2, 6u);
        f5CmacBuffer[51] = 0x01; /* Length msB big endian = 0x01, Length = 256 */
        f5CmacBuffer[52] = 0x00; /* Length lsB big endian = 0x00, Length = 256 */

        /*! Compute the MacKey into the temporary buffer. */
        AES_128_CMAC(f5CmacBuffer, sizeof(f5CmacBuffer), f5T, tempOut);

        /*! Copy the MacKey to the output location
         *  in reverse order. The CMAC result is generated MSB first. */
        FLib_MemCpyReverseOrder(pMacKey, (const uint8_t *)tempOut, 16u);

        /*! Build the least significant part of the f5 input data to compute the MacKey.
         *  It is identical to the most significant part with the exception of the counter. */
        f5CmacBuffer[0] = 1u; /* Counter = 1 */

        /*! Compute the LTK into the temporary buffer. */
        AES_128_CMAC(f5CmacBuffer, sizeof(f5CmacBuffer), f5T, tempOut);

        /*! Copy the LTK to the output location
         *  in reverse order. The CMAC result is generated MSB first. */
        FLib_MemCpyReverseOrder(pLtk, (const uint8_t *)tempOut, 16u);

        result = gSecSuccess_c;

    } while (false);

    return result;
}

/************************************************************************************
 * \brief Function used to derive the Bluetooth SKD used in LL encryption.
 *        Only to be called for applications activating encrypted key blobs.
 *
 * \param  [in] pInSKD   pointer to the received SKD (16-byte array)
 * \param  [in] pLtkBlob pointer to the blob (40-byte array)
 * \param  [in] bOpenKey  if TRUE sends derived key to NBU
 * \param  [out] pOutSKD pointer to the resulted SKD (16-byte array)
 *
 * \retval gSecSuccess_c operation succeeded
 * \retval gSecError_c operation failed
 ************************************************************************************/
secResultType_t SecLib_DeriveBluetoothSKDSecure(const uint8_t *pInSKD,
                                                const uint8_t *pLtkBlob,
                                                bool_t         bOpenKey,
                                                uint8_t       *pOutSKD)
{
    secResultType_t       result     = gSecError_c;
    size_t                eskByteLen = 16U;
    size_t                keyBitLen  = 128U;
    sss_sscp_object_t     keyObjLTK;
    sss_sscp_object_t     keyObjSK;
    sss_sscp_derive_key_t ctxDeriveKey;
    uint8_t               aInSKD[16];
    bool                  bLTKObjectInitialized        = false;
    bool                  bSKObjectInitialized         = false;
    bool                  bDeriveKeyContextInitialized = false;

    SECLIB_MUTEX_LOCK();

    FLib_MemCpyReverseOrder(aInSKD, pInSKD, 16);
    do
    {
        if ((CRYPTO_InitHardware()) != kStatus_Success)
        {
            break;
        }

        /* allocate LTK key object */
        if (sss_sscp_key_object_init(&keyObjLTK, &g_keyStore) != kStatus_SSS_Success)
        {
            break;
        }
        bLTKObjectInitialized = true;

        if (sss_sscp_key_object_allocate_handle(
                &keyObjLTK, ELE_S200_KEY_STORE_USER_ID_GENERIC, kSSS_KeyPart_Default, kSSS_CipherType_SYMMETRIC, 16u,
                kSSS_KeyProp_NoPlainRead | kSSS_KeyProp_NoPlainWrite | kSSS_KeyProp_CryptoAlgo_KDF) !=
            kStatus_SSS_Success)
        {
            break;
        }

        /* Allocate SK object */
        if (sss_sscp_key_object_init(&keyObjSK, &g_keyStore) != kStatus_SSS_Success)
        {
            break;
        }
        bSKObjectInitialized = true;

        if (sss_sscp_key_object_allocate_handle(
                &keyObjSK, ELE_S200_KEY_STORE_USER_ID_GENERIC, kSSS_KeyPart_Default, kSSS_CipherType_SYMMETRIC, 16u,
                kSSS_KeyProp_NoPlainRead | kSSS_KeyProp_NoPlainWrite) != kStatus_SSS_Success)
        {
            break;
        }

        /* import LTK blob into S200 */
        if (sss_sscp_key_store_import_key(&g_keyStore, &keyObjLTK, pLtkBlob, 40, keyBitLen, kSSS_blobType_ELKE_blob) !=
            kStatus_SSS_Success)
        {
            break;
        }

        /* compute encrypted sessionKey from LTK */
        if (sss_sscp_derive_key_context_init(&ctxDeriveKey, &g_sssSession, &keyObjLTK, kAlgorithm_SSS_AES_ECB,
                                             kMode_SSS_SymmetricKDF) != kStatus_SSS_Success)
        {
            break;
        }
        bDeriveKeyContextInitialized = true;

        if (sss_sscp_derive_key(&ctxDeriveKey, aInSKD, eskByteLen, &keyObjSK, 0U) != kStatus_SSS_Success)
        {
            break;
        }

        if (bOpenKey == TRUE)
        {
            sss_status_t ret;
            /* Generate random NBU_DKEY_SK and send it to NBU */
            PLATFORM_RemoteActiveReq();
            ret = sss_sscp_key_store_open_internal_key(&g_keyStore, kSSS_internalKey_NBU_DKEY_SK);
            PLATFORM_RemoteActiveRel();
            if (kStatus_SSS_Success != ret)
            {
                break;
            }
        }

        if (sss_sscp_key_store_export_key(&g_keyStore, &keyObjSK, pOutSKD, &eskByteLen, kSSS_blobType_NBU_ESK_blob) !=
            kStatus_SSS_Success)
        {
            break;
        }

        result = gSecSuccess_c;
    } while (false);

    /* de-allocate derive context and keys in S200 */
    if (bDeriveKeyContextInitialized == true)
    {
        (void)sss_sscp_derive_key_context_free(&ctxDeriveKey);
    }
    if (bLTKObjectInitialized == true)
    {
        (void)sss_sscp_key_object_free(&keyObjLTK, kSSS_keyObjFree_KeysStoreNoDefragment);
    }
    if (bSKObjectInitialized == true)
    {
        (void)sss_sscp_key_object_free(&keyObjSK, kSSS_keyObjFree_KeysStoreNoDefragment);
    }

    SECLIB_MUTEX_UNLOCK();
    return result;
}

/************************************************************************************
 * \brief Converts a plaintext symmetric key into a blob of blobType. Reverses key beforehand.
 *
 * \param[in]  pKey      Pointer to the key.
 *
 * \param[out] pBlob     Pointer to the blob (shall be allocated, 40 or 16, depending on blobType)
 *
 * \param[in]  blobType  Blob type. 0x01 - ELKE die unique blob, 0x02 - EDGE to EDGE blob,
 *                       0x03 - NBU ESK blob, 0x04 - NBU EIRK blob.
 *
 * \return gSecSuccess_c or error
 *
 ************************************************************************************/
secResultType_t SecLib_ObfuscateKeySecure(const uint8_t *pKey, uint8_t *pBlob, const uint8_t blobType)
{
    secResultType_t   result = gSecError_c;
    size_t            blobByteLen;
    sss_sscp_object_t keyObj;
    bool_t            keyInit = false;
    uint8_t           tempKey[16];

    blobByteLen = (blobType == 1U) ? 40U : 16U;

    FLib_MemCpyReverseOrder(tempKey, pKey, 16U);

    SECLIB_MUTEX_LOCK();
    do
    {
        if ((CRYPTO_InitHardware()) != kStatus_Success)
        {
            break;
        }

        if (sss_sscp_key_object_init(&keyObj, &g_keyStore) != kStatus_SSS_Success)
        {
            break;
        }
        keyInit = true;

        if (sss_sscp_key_object_allocate_handle(&keyObj, ELE_S200_KEY_STORE_USER_ID_GENERIC, kSSS_KeyPart_Default,
                                                kSSS_CipherType_NONE, 16u,
                                                mSecLibKeyPropCryptoAlgoAll_c) != kStatus_SSS_Success)
        {
            break;
        }

        if (sss_sscp_key_store_set_key(&g_keyStore, &keyObj, (const uint8_t *)tempKey, 16u, 128u,
                                       kSSS_KeyPart_Default) != kStatus_SSS_Success)
        {
            break;
        }

        if (sss_sscp_key_store_export_key(&g_keyStore, &keyObj, pBlob, &blobByteLen, blobType) != kStatus_SSS_Success)
        {
            break;
        }

        result = gSecSuccess_c;
    } while (false);

    if (keyInit == true)
    {
        (void)sss_sscp_key_object_free(&keyObj, kSSS_keyObjFree_KeysStoreNoDefragment);
    }
    SECLIB_MUTEX_UNLOCK();
    return result;
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
secResultType_t SecLib_DeobfuscateKeySecure(const uint8_t *pBlob, uint8_t *pKey)
{
    secResultType_t   result    = gSecError_c;
    size_t            keyBitLen = 128U, keyByteLen = 16U;
    sss_sscp_object_t keyObj;
    bool_t            keyInit = false;
    uint8_t           tempKey[16];
    SECLIB_MUTEX_LOCK();
    do
    {
        if ((CRYPTO_InitHardware()) != kStatus_Success)
        {
            break;
        }

        if (sss_sscp_key_object_init(&keyObj, &g_keyStore) != kStatus_SSS_Success)
        {
            break;
        }
        keyInit = true;

        if (sss_sscp_key_object_allocate_handle(&keyObj, ELE_S200_KEY_STORE_USER_ID_GENERIC, kSSS_KeyPart_Default,
                                                kSSS_CipherType_NONE, 16u,
                                                mSecLibKeyPropCryptoAlgoAll_c) != kStatus_SSS_Success)
        {
            break;
        }

        if (sss_sscp_key_store_import_key(&g_keyStore, &keyObj, pBlob, 40U, 8U * 40U, kSSS_blobType_ELKE_blob) !=
            kStatus_SSS_Success)
        {
            break;
        }

        if (sss_sscp_key_store_get_key(&g_keyStore, &keyObj, tempKey, &keyByteLen, &keyBitLen, kSSS_KeyPart_Default) !=
            kStatus_SSS_Success)
        {
            break;
        }

        result = gSecSuccess_c;
        FLib_MemCpyReverseOrder(pKey, tempKey, 16U);
    } while (false);

    if (keyInit == true)
    {
        (void)sss_sscp_key_object_free(&keyObj, kSSS_keyObjFree_KeysStoreNoDefragment);
    }
    SECLIB_MUTEX_UNLOCK();
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
    secResultType_t result                       = gSecError_c;
    uint8_t         tempAddrPart[AES_BLOCK_SIZE] = {0u};
    uint8_t         tempOutHash[AES_BLOCK_SIZE]  = {0u};
    uint8_t         tempKey[AES_128_KEY_BYTE_LEN];
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
        for (uint8_t i = 0; i < 3u; i++)
        {
            tempAddrPart[AES_BLOCK_SIZE - 1u - i] = pR[i];
        }
        /* Regular operation with plaintext key */
        /*! Reverse the Key and place it in a temporary location. */
        FLib_MemCpyReverseOrder(tempKey, (const uint8_t *)pKey, AES_128_KEY_BYTE_LEN);

        /*! Compute the hash. */
        AES_128_Encrypt(tempAddrPart, tempKey, tempOutHash);

        /*! Copy the relevant bytes to the output. */
        for (uint8_t i = 0; i < 3u; i++)
        {
            pHash[i] = tempOutHash[AES_BLOCK_SIZE - 1u - i];
        }
        result = gSecSuccess_c;

    } while (false);

    return result;
}

/*! *********************************************************************************
 * \brief  This function implements the SMP ah cryptographic toolbox function which calculates the
 *         hash part of a Resolvable Private Address.
 *         The key is kept in a secure blob. It is never exposed on non secure bus.
 *
 * \param[out]  pHash  Pointer where the 24 bit hash value will be written.
 *                     24 bit hash field of a Resolvable Private Address (output)
 *
 * \param[in]  pKey  Pointer to the 128 bit key.
 *
 * \param[in]  pR   Pointer to the 24 bit random value (Prand). 24 bit random part of a Resolvable private Address.
 *                  The most significant bits of this field must be 0b01 for Resolvable Private Addresses.
 *
 * \retval  gSecSuccess_c  All operations were successful.
 * \retval  gSecError_c The call failed.
 *
 ********************************************************************************** */
secResultType_t SecLib_VerifyBluetoothAhSecure(uint8_t *pHash, const uint8_t *pKey, const uint8_t *pR)
{
    secResultType_t result = gSecError_c;

    sss_sscp_object_t    keyObj;
    sss_sscp_symmetric_t context;
    bool_t               keyInit          = false;
    bool_t               contextInit      = false;
    uint8_t              tempAddrPart[16] = {0};
    uint8_t              tempFullHash[16] = {0};
    SECLIB_MUTEX_LOCK();
    do
    {
        /*! Check for NULL output pointers and return with proper status if this is the case. */
        if ((NULL == pHash) || (NULL == pR) || (NULL == pKey))
        {
            break;
        }
        if ((CRYPTO_InitHardware()) != kStatus_Success)
        {
            break;
        }

        if (sss_sscp_key_object_init(&keyObj, &g_keyStore) != kStatus_SSS_Success)
        {
            break;
        }
        keyInit = true;

        if (sss_sscp_key_object_allocate_handle(&keyObj, 0, kSSS_KeyPart_Default, kSSS_CipherType_NONE, 16u,
                                                SSS_KEYPROP_OPERATION_AES) != kStatus_SSS_Success)
        {
            break;
        }

        if (sss_sscp_key_store_import_key(&g_keyStore, &keyObj, pKey, 40U, 8U * 40U, kSSS_blobType_ELKE_blob) !=
            kStatus_SSS_Success)
        {
            break;
        }

        if (sss_sscp_symmetric_context_init(&context, g_keyStore.session, &keyObj, kAlgorithm_SSS_AES_ECB,
                                            kMode_SSS_Encrypt) != kStatus_SSS_Success)
        {
            break;
        }
        contextInit = true;

        /* Initialize the r' value in the temporary location. 3 bytes of ramdom value.
         *  Initialize it reversed for AES.
         */
        for (int i = 0; i < 3; i++)
        {
            tempAddrPart[15 - i] = pR[i];
        }

        if (sss_sscp_cipher_one_go(&context, NULL, 0, tempAddrPart, tempFullHash, 16U) != kStatus_SSS_Success)
        {
            break;
        }

        pHash[0] = tempFullHash[15];
        pHash[1] = tempFullHash[14];
        pHash[2] = tempFullHash[13];

        result = gSecSuccess_c;

    } while (false);

    if (keyInit == true)
    {
        (void)sss_sscp_key_object_free(&keyObj, kSSS_keyObjFree_KeysStoreNoDefragment);
    }
    if (contextInit == true)
    {
        (void)sss_sscp_symmetric_context_free(&context);
    }
    SECLIB_MUTEX_UNLOCK();
    return result;
}

#ifdef DBG_SECLIB
void dump_octet_string(const char *str, const unsigned char *data, size_t len)
{
    for (int i = 0; i < len; i++)
    {
        if (i % 16 == 0)
            PRINTF("\r\n%s[%d]:\t", str, i);
        if (data[i] < 0x10)
            PRINTF("0%X", data[i]);
        else
            PRINTF("%02X", data[i]);
    }
}
#endif

/************************************************************************************
 * \private
 * \brief Function used to create the mac key and LTK using Bluetooth F5 algorithm
 *        Only ever called in the secure (Key blobs) variant of the F5,
 *
 * \param  [in] pDhKey                   pointer to DH Key object
 * \param  [in] pDerivationDataMacKey    derivation data for MacKey
 * \param  [in] pDerivationDataLTK       derivation data for LTK
 * \param  [out] pMacKey                 pointer to mac key
 * \param  [out] pLTKBlob                pointer to LTK blob
 *
 * \return sss_status_t
 ************************************************************************************/
static sss_status_t ELKE_BLE_SM_F5_DeriveKeysSecure(sss_sscp_object_t *pPubDhKey,
                                                    const uint8_t     *pDerivationDataMacKey,
                                                    const uint8_t     *pDerivationDataLTK,
                                                    uint8_t           *pMacKey,
                                                    uint8_t           *pLTKBlob)
{
    sss_sscp_object_t     keyObj__MacKey;
    sss_sscp_object_t     keyObj__LTK;
    sss_sscp_derive_key_t ctx_deriveKey;

    bool bInitialized_MacKey = false;
    bool bInitialized_LTK    = false;

    sss_status_t result = kStatus_SSS_Fail;

    SECLIB_MUTEX_LOCK();
    do
    {
        if ((CRYPTO_InitHardware()) != kStatus_Success)
        {
            break;
        }

        if (sss_sscp_key_object_init(&keyObj__MacKey, &g_keyStore) != kStatus_SSS_Success)
        {
            break;
        }
        bInitialized_MacKey = true;

        if (sss_sscp_key_object_allocate_handle(
                &keyObj__MacKey, ELE_S200_KEY_STORE_USER_ID_GENERIC, kSSS_KeyPart_Default, kSSS_CipherType_SYMMETRIC,
                16u, kSSS_KeyProp_NoPlainWrite | kSSS_KeyProp_CryptoAlgo_MAC) != kStatus_SSS_Success)
        {
            break;
        }

        if (sss_sscp_key_object_init(&keyObj__LTK, &g_keyStore) != kStatus_SSS_Success)
        {
            break;
        }
        bInitialized_LTK = true;

        if (sss_sscp_key_object_allocate_handle(
                &keyObj__LTK, ELE_S200_KEY_STORE_USER_ID_GENERIC, kSSS_KeyPart_Default, kSSS_CipherType_SYMMETRIC, 16u,
#if !gSecLibAllowLtkFromBlob_c
                kSSS_KeyProp_NoPlainRead |
#endif
                    kSSS_KeyProp_NoPlainWrite | kSSS_KeyProp_CryptoAlgo_KDF) != kStatus_SSS_Success)
        {
            break;
        }

        /* derive keys for f5() */
        if (sss_sscp_derive_key_context_init(&ctx_deriveKey, &g_sssSession, pPubDhKey, kAlgorithm_SSS_BLE_F5,
                                             kMode_SSS_SymmetricKDF) != kStatus_SSS_Success)
        {
            break;
        }

        do
        {
            if (sss_sscp_derive_key(&ctx_deriveKey, pDerivationDataMacKey, 53u, &keyObj__MacKey, 0U) !=
                kStatus_SSS_Success)
            {
                break;
            }
            if (sss_sscp_derive_key(&ctx_deriveKey, pDerivationDataLTK, 53u, &keyObj__LTK, 0U) != kStatus_SSS_Success)
            {
                break;
            }
        } while (false);

        if (sss_sscp_derive_key_context_free(&ctx_deriveKey) != kStatus_SSS_Success)
        {
            break;
        }

        /* export keys from S200 */

        /* export MacKey */
        size_t  bufLen    = 16u;
        size_t  keyBitLen = 128u;
        uint8_t macData[16];
        if (sss_sscp_key_store_get_key(&g_keyStore, &keyObj__MacKey, macData, &bufLen, &keyBitLen,
                                       kSSS_KeyPart_Default) != kStatus_SSS_Success)
        {
            break;
        }
        FLib_MemCpyReverseOrder(pMacKey, macData, 16);

        /* export LTK blob */
        size_t ltkBlobByteLen = 40u;
        keyBitLen             = 128u;

        if (sss_sscp_key_store_export_key(&g_keyStore, &keyObj__LTK, pLTKBlob, &ltkBlobByteLen,
                                          kSSS_blobType_ELKE_blob) != kStatus_SSS_Success)
        {
            break;
        }

        result = kStatus_SSS_Success;
    } while (false);

    /* delete keys and contexts from S200 */
    if (bInitialized_MacKey)
    {
        (void)sss_sscp_key_object_free(&keyObj__MacKey, kSSS_keyObjFree_KeysStoreNoDefragment);
    }

    if (bInitialized_LTK)
    {
        (void)sss_sscp_key_object_free(&keyObj__LTK, kSSS_keyObjFree_KeysStoreNoDefragment);
    }

    /* DHkey object from pDhKeyData->outPoint can be deleted here */
    (void)sss_sscp_key_object_free(pPubDhKey, kSSS_keyObjFree_KeysStoreNoDefragment);

    SECLIB_MUTEX_UNLOCK();

    return result;
}

/************************************************************************************
 * \brief Generates a symmetric key in ELKE blob or plain text form .
 *
 * \param[in]  keySize the size of the generated key.
 *
 * \param[in] blobOutput true - blob, false - plain text output.
 *
 * \param[out] pOut   the address of the buffer to store the key.
 *                    Storage for sss_sscp_object_t key reference
 *
 * \return gSecSuccess_c or error
 *
 ************************************************************************************/
secResultType_t SecLib_GenerateSymmetricKey(const uint32_t keySize, const bool_t blobOutput, void *pOut)
{
    secResultType_t   result     = gSecError_c;
    bool_t            keyObjFree = false;
    sss_sscp_object_t keyObj;
    SECLIB_MUTEX_LOCK();
    do
    {
        if ((CRYPTO_InitHardware()) != kStatus_Success)
        {
            break;
        }

        if (kStatus_SSS_Success != sss_sscp_key_object_init(&keyObj, &g_keyStore))
        {
            break;
        }
        keyObjFree = true;
        if (kStatus_SSS_Success != sss_sscp_key_object_allocate_handle(&keyObj, ELE_S200_KEY_STORE_USER_ID_GENERIC,
                                                                       kSSS_KeyPart_Default, kSSS_CipherType_SYMMETRIC,
                                                                       keySize, mSecLibKeyPropCryptoAlgoAll_c))
        {
            break;
        }
        if (kStatus_SSS_Success != sss_sscp_key_store_generate_key(&g_keyStore, &keyObj, (size_t)(keySize << 3U), NULL))
        {
            break;
        }
        if (blobOutput == true)
        {
            size_t blobByteLen = gSecLibElkeBlobSize_c;
            if (kStatus_SSS_Success !=
                sss_sscp_key_store_export_key(&g_keyStore, &keyObj, pOut, &blobByteLen, kSSS_blobType_ELKE_blob))
            {
                break;
            }
        }
        else
        {
            size_t keyByteLen = (size_t)keySize;
            size_t keyBitLen  = (size_t)(keySize << 3U);
            if (kStatus_SSS_Success !=
                sss_sscp_key_store_get_key(&g_keyStore, &keyObj, pOut, &keyByteLen, &keyBitLen, kSSS_KeyPart_Default))
            {
                break;
            }
        }
        result = gSecSuccess_c;

    } while (false);

    if (keyObjFree == true)
    {
        (void)sss_sscp_key_object_free(&keyObj, kSSS_keyObjFree_KeysStoreNoDefragment);
    }
    SECLIB_MUTEX_UNLOCK();
    return result;
}

/************************************************************************************
 * \brief Generates an EIRK blob from an ELKE blob or plain text symmetric key.
 *
 * \param[in]  pIRK pointer to the input IRK key.
 *
 * \param[in] blobInput true - pIRK points to an ELKE blob, false - pIRK points to a plain text key.
 *
 * \param[in] generateDKeyIRK true - DKeyIRK is slso generated and provided to NBU.
 *
 * \param[out] pOutEIRKblob   the address of the buffer to store the EIRK blob.
 *
 * \return gSecSuccess_c or error
 *
 ************************************************************************************/
secResultType_t SecLib_GenerateBluetoothEIRKBlobSecure(const void  *pIRK,
                                                       const bool_t blobInput,
                                                       const bool_t generateDKeyIRK,
                                                       uint8_t     *pOutEIRKblob)
{
    secResultType_t   result     = gSecError_c;
    bool_t            keyObjFree = false;
    sss_sscp_object_t keyObj;
#ifdef REVERSE_EIRK
    uint8_t tempKey[16];
#endif
    SECLIB_MUTEX_LOCK();
    do
    {
        if ((CRYPTO_InitHardware()) != kStatus_Success)
        {
            break;
        }

        if (kStatus_SSS_Success != sss_sscp_key_object_init(&keyObj, &g_keyStore))
        {
            break;
        }
        keyObjFree = true;
        if (kStatus_SSS_Success != sss_sscp_key_object_allocate_handle(&keyObj, ELE_S200_KEY_STORE_USER_ID_GENERIC,
                                                                       kSSS_KeyPart_Default, kSSS_CipherType_SYMMETRIC,
                                                                       16U, mSecLibKeyPropCryptoAlgoAll_c))
        {
            break;
        }
        if (blobInput == true)
        {
            if (kStatus_SSS_Success != sss_sscp_key_store_import_key(&g_keyStore, &keyObj, pIRK, gSecLibElkeBlobSize_c,
                                                                     128U, kSSS_blobType_ELKE_blob))
            {
                break;
            }
        }
        else
        {
            if (kStatus_SSS_Success !=
                sss_sscp_key_store_set_key(&g_keyStore, &keyObj, pIRK, 16U, 128U, kSSS_KeyPart_Default))
            {
                break;
            }
        }

        if (generateDKeyIRK == true)
        {
            sss_status_t ret;
            /* Generate random NBU_DKEY_IRK and send it to NBU */
            PLATFORM_RemoteActiveReq();
            ret = sss_sscp_key_store_open_internal_key(&g_keyStore, kSSS_internalKey_NBU_DKEY_IRK);
            PLATFORM_RemoteActiveRel();
            if (kStatus_SSS_Success != ret)
            {
                break;
            }
        }

        size_t eirkBlobByteLen = gSecLibEirkBlobSize_c;

#ifdef REVERSE_EIRK
        if (kStatus_SSS_Success !=
            sss_sscp_key_store_export_key(&g_keyStore, &keyObj, tempKey, &eirkBlobByteLen, kSSS_blobType_NBU_EIRK_blob))
#else
        if (kStatus_SSS_Success != sss_sscp_key_store_export_key(&g_keyStore, &keyObj, pOutEIRKblob, &eirkBlobByteLen,
                                                                 kSSS_blobType_NBU_EIRK_blob))
#endif
        {
            break;
        }
        result = gSecSuccess_c;
#ifdef REVERSE_EIRK
        FLib_MemCpyReverseOrder(pOutEIRKblob, tempKey, 16U);
#endif
    } while (false);
    if (keyObjFree == true)
    {
        (void)sss_sscp_key_object_free(&keyObj, kSSS_keyObjFree_KeysStoreNoDefragment);
    }
    SECLIB_MUTEX_UNLOCK();
    return result;
}

/************************************************************************************
 * \brief Computes the Edgelock to Edgelock key for an ECDH P256 key pair.
 *
 * \param[in]   pInPeerPublicKey     pointer to the public key.
 * \param[out]  pOutE2EKey           pointer where the E2E key object is stored
 *
 * \return gSecSuccess_c or error
 *
 ************************************************************************************/
secResultType_t ECDH_P256_ComputeA2BKeySecure(const ecdhPublicKey_t *pInPeerPublicKey, ecdhDhKey_t *pOutE2EKey)
{
    secResultType_t ret     = gSecError_c;
    uint8_t        *wrk_buf = NULL;
    SECLIB_MUTEX_LOCK();
    sss_ecdh_context_t ecdh_ctx = {0};
    do
    {
        if (pECPKeyPair == NULL)
        {
            ret = gSecError_c;
            break;
        }

        size_t wrk_buf_sz = 3u * ECP256_COORDINATE_LEN;
        wrk_buf           = MEM_BufferAlloc(wrk_buf_sz);
        if (wrk_buf == NULL)
        {
            RAISE_ERROR(ret, gSecAllocError_c);
        }
        ecdh_ctx.ecdh_key_pair = pECPKeyPair;
        ECP256_PointCopy_and_change_endianness((uint8_t *)&ecdh_ctx.Qp, (const uint8_t *)&pInPeerPublicKey->raw[0]);
        ecdh_ctx.keepSharedSecret = true;

        if (sss_ecdh_calc_EL2EL_key(&ecdh_ctx, wrk_buf, wrk_buf_sz) != kStatus_Success)
        {
            RAISE_ERROR(ret, gSecError_c);
        }
        FLib_MemCpy(pOutE2EKey->raw, (void *)&ecdh_ctx.sharedSecret, sizeof(sss_sscp_object_t));
        (void)MEM_BufferFree(wrk_buf);

        ret = gSecSuccess_c;

    } while (false);
    SECLIB_MUTEX_UNLOCK();
    return ret;
}

/************************************************************************************
 * \brief Free E2E key object
 *
 * \param[in]  pE2EKeyData   Pointer to the E2E key data to be freed.
 *
 * \return gSecSuccess_c or error
 *
 ************************************************************************************/
secResultType_t ECDH_P256_FreeE2EKeyDataSecure(ecdhDhKey_t *pE2EKeyData)
{
    secResultType_t result = gSecError_c;
    sscp_status_t   status = kStatus_SSCP_Fail;

    /* turn into void* first to avoid MISRA 11.3 */
    void *pKeyData = pE2EKeyData;

    status = sss_sscp_key_object_free((sss_sscp_object_t *)pKeyData, kSSS_keyObjFree_KeysStoreNoDefragment);

    if (kStatus_SSS_Success == status)
    {
        result = gSecSuccess_c;
    }
    return result;
}

/************************************************************************************
 * \brief Generates an E2E blob from an ELKE blob or plain text symmetric key.
 *
 * \param[in]  pKey      pointer to the input key.
 * \param[in]  keyType   input key type.
 * \param[out] pOutKey   pointer to where the output E2E blob will be copied.
 *
 * \return gSecSuccess_c or error
 *
 ************************************************************************************/
secResultType_t SecLib_ExportA2BBlobSecure(const void *pKey, const secInputKeyType_t keyType, uint8_t *pOutKey)
{
    secResultType_t   result     = gSecError_c;
    bool_t            keyObjFree = false;
    sss_sscp_object_t keyObj;

    SECLIB_MUTEX_LOCK();
    do
    {
        if ((CRYPTO_InitHardware()) != kStatus_Success)
        {
            break;
        }

        if (kStatus_SSS_Success != sss_sscp_key_object_init(&keyObj, &g_keyStore))
        {
            break;
        }
        keyObjFree = true;

        if (gSecPlainText_c == keyType)
        {
            uint8_t tempKey[16];

            FLib_MemCpyReverseOrder(tempKey, pKey, 16U);

            if (kStatus_SSS_Success != sss_sscp_key_object_allocate_handle(&keyObj, ELE_S200_KEY_STORE_USER_ID_GENERIC,
                                                                           kSSS_KeyPart_Default, kSSS_CipherType_NONE,
                                                                           16U, SSS_KEYPROP_OPERATION_AES))
            {
                break;
            }

            if (kStatus_SSS_Success !=
                sss_sscp_key_store_set_key(&g_keyStore, &keyObj, tempKey, 16U, 128U, kSSS_KeyPart_Default))
            {
                break;
            }
        }
        else if (gSecElkeBlob_c == keyType)
        {
            if (kStatus_SSS_Success !=
                sss_sscp_key_object_allocate_handle(&keyObj, ELE_S200_KEY_STORE_USER_ID_GENERIC, kSSS_KeyPart_Default,
                                                    kSSS_CipherType_SYMMETRIC, 16U, mSecLibKeyPropCryptoAlgoAll_c))
            {
                break;
            }

            if (kStatus_SSS_Success != sss_sscp_key_store_import_key(&g_keyStore, &keyObj, pKey, gSecLibElkeBlobSize_c,
                                                                     128U, kSSS_blobType_ELKE_blob))
            {
                break;
            }
        }
        else if (gSecLtkElkeBlob_c == keyType)
        {
            if (kStatus_SSS_Success !=
                sss_sscp_key_object_allocate_handle(
                    &keyObj, ELE_S200_KEY_STORE_USER_ID_GENERIC, kSSS_KeyPart_Default, kSSS_CipherType_SYMMETRIC, 16U,
                    kSSS_KeyProp_NoPlainRead | kSSS_KeyProp_NoPlainWrite | kSSS_KeyProp_CryptoAlgo_KDF))
            {
                break;
            }

            if (kStatus_SSS_Success != sss_sscp_key_store_import_key(&g_keyStore, &keyObj, pKey, gSecLibElkeBlobSize_c,
                                                                     128U, kSSS_blobType_ELKE_blob))
            {
                break;
            }
        }
        else
        {
            /* Invalid keyType. */
            break;
        }

        size_t e2eBlobByteLen = gSecLibElkeBlobSize_c;

        if (kStatus_SSS_Success !=
            sss_sscp_key_store_export_key(&g_keyStore, &keyObj, pOutKey, &e2eBlobByteLen, kSSS_blobType_E2E_blob))
        {
            break;
        }
        result = gSecSuccess_c;
    } while (false);
    SECLIB_MUTEX_UNLOCK();
    if (keyObjFree == true)
    {
        (void)sss_sscp_key_object_free(&keyObj, kSSS_keyObjFree_KeysStoreNoDefragment);
    }
    return result;
}

/************************************************************************************
 * \brief Generates a symmetric key in ELKE blob or plain text form from an E2E blob.
 *
 * \param[in]  pKey      pointer to the input E2E blob.
 * \param[in]  keyType   output key type.
 * \param[out] pOutKey   pointer to where the output key will be copied.
 *
 * \return gSecSuccess_c or error
 *
 ************************************************************************************/
secResultType_t SecLib_ImportA2BBlobSecure(const uint8_t *pKey, const secInputKeyType_t keyType, uint8_t *pOutKey)
{
    secResultType_t   result    = gSecError_c;
    size_t            keyBitLen = 128U, keyByteLen = 16U;
    sss_sscp_object_t keyObj;
    bool_t            keyInit = false;
    uint8_t           tempKey[16];

    SECLIB_MUTEX_LOCK();
    do
    {
        if ((CRYPTO_InitHardware()) != kStatus_Success)
        {
            break;
        }

        if (sss_sscp_key_object_init(&keyObj, &g_keyStore) != kStatus_SSS_Success)
        {
            break;
        }
        keyInit = true;

        if (gSecPlainText_c == keyType)
        {
            if (sss_sscp_key_object_allocate_handle(&keyObj, ELE_S200_KEY_STORE_USER_ID_GENERIC, kSSS_KeyPart_Default,
                                                    kSSS_CipherType_NONE, 16u,
                                                    SSS_KEYPROP_OPERATION_AES) != kStatus_SSS_Success)
            {
                break;
            }

            if (sss_sscp_key_store_import_key(&g_keyStore, &keyObj, pKey, 40U, 8U * 40U, kSSS_blobType_E2E_blob) !=
                kStatus_SSS_Success)
            {
                break;
            }

            if (sss_sscp_key_store_get_key(&g_keyStore, &keyObj, tempKey, &keyByteLen, &keyBitLen,
                                           kSSS_KeyPart_Default) != kStatus_SSS_Success)
            {
                break;
            }

            FLib_MemCpyReverseOrder(pOutKey, tempKey, 16U);
        }
        else if (gSecElkeBlob_c == keyType)
        {
            size_t e2eBlobByteLen = gSecLibElkeBlobSize_c;

            if (kStatus_SSS_Success !=
                sss_sscp_key_object_allocate_handle(&keyObj, ELE_S200_KEY_STORE_USER_ID_GENERIC, kSSS_KeyPart_Default,
                                                    kSSS_CipherType_SYMMETRIC, 16U, mSecLibKeyPropCryptoAlgoAll_c))
            {
                break;
            }

            if (sss_sscp_key_store_import_key(&g_keyStore, &keyObj, pKey, 40U, 8U * 40U, kSSS_blobType_E2E_blob) !=
                kStatus_SSS_Success)
            {
                break;
            }

            if (kStatus_SSS_Success !=
                sss_sscp_key_store_export_key(&g_keyStore, &keyObj, pOutKey, &e2eBlobByteLen, kSSS_blobType_ELKE_blob))
            {
                break;
            }
        }
        else if (gSecLtkElkeBlob_c == keyType)
        {
            size_t e2eBlobByteLen = gSecLibElkeBlobSize_c;

            if (sss_sscp_key_object_allocate_handle(
                    &keyObj, ELE_S200_KEY_STORE_USER_ID_GENERIC, kSSS_KeyPart_Default, kSSS_CipherType_SYMMETRIC, 16u,
                    kSSS_KeyProp_NoPlainRead | kSSS_KeyProp_NoPlainWrite | kSSS_KeyProp_CryptoAlgo_KDF) !=
                kStatus_SSS_Success)
            {
                break;
            }

            if (sss_sscp_key_store_import_key(&g_keyStore, &keyObj, pKey, 40U, 8U * 40U, kSSS_blobType_E2E_blob) !=
                kStatus_SSS_Success)
            {
                break;
            }

            if (kStatus_SSS_Success !=
                sss_sscp_key_store_export_key(&g_keyStore, &keyObj, pOutKey, &e2eBlobByteLen, kSSS_blobType_ELKE_blob))
            {
                break;
            }
        }
        else
        {
            /* Invalid keyType */
            break;
        }

        result = gSecSuccess_c;
    } while (false);
    SECLIB_MUTEX_UNLOCK();

    if (keyInit == true)
    {
        (void)sss_sscp_key_object_free(&keyObj, kSSS_keyObjFree_KeysStoreNoDefragment);
    }
    return result;
}
