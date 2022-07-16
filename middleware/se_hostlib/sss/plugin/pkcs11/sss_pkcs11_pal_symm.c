/*
 * Copyright 2021 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

/* ************************************************************************** */
/* Includes                                                                   */
/* ************************************************************************** */

#include "sss_pkcs11_pal.h"

/* ************************************************************************** */
/* Public Functions                                                           */
/* ************************************************************************** */

/** @brief Symmetric Encryption.
 * This function generates a random IV (initialization vector), nonce
 * or initial counter value for the encryption operation as appropriate
 * for the chosen algorithm, key type and key size.
 *
 * @param pxSessionObj - Pointer to mechParameter for initializing initialization vector.
 * @param algorithm - Algorithm to be applied, e.g. kAlgorithm_SSS_AES_CBC.
 * @param pData - Buffer containing the input data.
 * @param ulDataLen - Length of the buffer pData.
 * @param pEncryptedData - Buffer containing the encrypted data.
 * @param pulEncryptedDataLen - Length of the buffer pEncryptedData.

 * @returns Status of the operation
 * @retval #CKR_OK The operation has completed successfully.
 * @retval #CKR_FUNCTION_FAILED The requested function could not be performed.
 * @retval #CKR_DEVICE_ERROR If some problem has occured with the token or slot.
 * @retval #CKR_BUFFER_TOO_SMALL The output of function is too large to fit in supplied buffer.
 */

CK_RV SymmetricEncrypt(P11SessionPtr_t pxSessionObj,
    sss_algorithm_t algorithm,
    CK_BYTE_PTR pData,
    CK_ULONG ulDataLen,
    CK_BYTE_PTR pEncryptedData,
    CK_ULONG_PTR pulEncryptedDataLen)
{
    CK_RV xResult       = CKR_OK;
    sss_status_t status = kStatus_SSS_Fail;
    uint8_t data[2048]  = {0};
    sss_symmetric_t symmCtx;
    sss_object_t symmObject;

    if (ulDataLen > sizeof(data)) {
        LOG_E("Buffer overflow");
        return CKR_HOST_MEMORY;
    }
    memcpy(&data[0], pData, ulDataLen);
    uint8_t iv[AES_BLOCK_SIZE] = {0};
    size_t ivLen               = sizeof(iv);
    uint8_t encData[2048]      = {0};
    size_t encDataLen          = sizeof(encData);
    size_t tempOutBufLen       = encDataLen;
    uint8_t *pOut              = &encData[0];
    if (algorithm == kAlgorithm_SSS_AES_CBC || algorithm == kAlgorithm_SSS_AES_CTR) {
        if (pxSessionObj->mechParameterLen != 0) {
            memcpy(iv, pxSessionObj->mechParameter, ivLen);
        }
    }

    LOCK_MUTEX_FOR_RTOS
    {
        status = sss_key_object_init(&symmObject, &pex_sss_demo_boot_ctx->ks);
        if (status != kStatus_SSS_Success) {
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_DEVICE_ERROR)
            // return CKR_DEVICE_ERROR;
        }

        status = sss_key_object_get_handle(&symmObject, pxSessionObj->xOperationKeyHandle);
        if (status != kStatus_SSS_Success) {
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_DEVICE_ERROR)
            // return CKR_DEVICE_ERROR;
        }

        status = sss_symmetric_context_init(
            &symmCtx, &pex_sss_demo_boot_ctx->session, &symmObject, algorithm, kMode_SSS_Encrypt);
        if (status != kStatus_SSS_Success) {
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_DEVICE_ERROR)
            // return CKR_DEVICE_ERROR;
        }

        /*Do Encryption*/
        status = sss_cipher_init(&symmCtx, iv, ivLen);
        if (status != kStatus_SSS_Success) {
            sss_symmetric_context_free(&symmCtx);
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_DEVICE_ERROR)
        }

        status = sss_cipher_update(&symmCtx, (const uint8_t *)pData, (size_t)ulDataLen, pOut, &tempOutBufLen);
        if (status != kStatus_SSS_Success) {
            sss_symmetric_context_free(&symmCtx);
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_DEVICE_ERROR)
            // return CKR_DEVICE_ERROR;
        }

        pOut          = pOut + tempOutBufLen;
        encDataLen    = tempOutBufLen;
        tempOutBufLen = sizeof(encData) - tempOutBufLen;

        status = sss_cipher_finish(&symmCtx, NULL, 0, pOut, &tempOutBufLen);
        // LOG_AU8_W(encData,encDataLen);
        if (status != kStatus_SSS_Success) {
            sss_symmetric_context_free(&symmCtx);
            LOG_E("sss_cipher_one_go failed");
            xResult = CKR_FUNCTION_FAILED;
        }
        encDataLen = encDataLen + tempOutBufLen;
        if (xResult == CKR_OK) {
            if (pEncryptedData) {
                if (*pulEncryptedDataLen < encDataLen) {
                    xResult = CKR_BUFFER_TOO_SMALL;
                }
                else {
                    memcpy(pEncryptedData, &encData[0], encDataLen);
                    pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
                }
            }
            *pulEncryptedDataLen = encDataLen;
        }
        LOG_AU8_W(encData, encDataLen);
        sss_symmetric_context_free(&symmCtx);

        UNLOCK_MUTEX_FOR_RTOS
    }

    return xResult;
}

/** @brief Symmetric Decryption.
 * This function generates a random IV (initialization vector), nonce
 * or initial counter value for the decryption operation as appropriate
 * for the chosen algorithm, key type and key size.
 *
 * @param pxSessionObj - Pointer to mechParameter for initializing initialization vector.
 * @param algorithm - Algorithm to be applied, e.g. kAlgorithm_SSS_AES_CBC.
 * @param pEncryptedData - Buffer containing the encrypted data.
 * @param ulEncryptedDataLen - Length of the buffer pEncryptedData.
 * @param pData - Buffer containing the decrypted data.
 * @param pulDecryptedDataLen - Length of the buffer pData.

 * @returns Status of the operation
 * @retval #CKR_OK The operation has completed successfully.
 * @retval #CKR_FUNCTION_FAILED The requested function could not be performed.
 * @retval #CKR_DEVICE_ERROR If some problem has occured with the token or slot.
 * @retval #CKR_BUFFER_TOO_SMALL The output of function is too large to fit in supplied buffer.
 */

CK_RV SymmetricDecrypt(P11SessionPtr_t pxSessionObj,
    sss_algorithm_t algorithm,
    CK_BYTE_PTR pEncryptedData,
    CK_ULONG ulEncryptedDataLen,
    CK_BYTE_PTR pData,
    CK_ULONG_PTR pulDecryptedDataLen)
{
    CK_RV xResult       = CKR_OK;
    sss_status_t status = kStatus_SSS_Fail;
    sss_symmetric_t symmCtx;
    sss_object_t symmObject;

    uint8_t iv[AES_BLOCK_SIZE] = {0};
    size_t ivLen               = sizeof(iv);
    uint8_t encData[256]       = {0};
    size_t encDataLen          = sizeof(encData);
    size_t tempOutBufLen       = encDataLen;
    uint8_t *pOut              = &encData[0];
    if (algorithm == kAlgorithm_SSS_AES_CBC || algorithm == kAlgorithm_SSS_AES_CTR) {
        if (pxSessionObj->mechParameterLen != 0) {
            memcpy(iv, pxSessionObj->mechParameter, ivLen);
        }
    }

    LOCK_MUTEX_FOR_RTOS
    {
        status = sss_key_object_init(&symmObject, &pex_sss_demo_boot_ctx->ks);
        if (status != kStatus_SSS_Success) {
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_DEVICE_ERROR)
        }

        status = sss_key_object_get_handle(&symmObject, pxSessionObj->xOperationKeyHandle);
        if (status != kStatus_SSS_Success) {
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_DEVICE_ERROR)
        }

        status = sss_symmetric_context_init(
            &symmCtx, &pex_sss_demo_boot_ctx->session, &symmObject, algorithm, kMode_SSS_Decrypt);
        if (status != kStatus_SSS_Success) {
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_DEVICE_ERROR)
            // return CKR_DEVICE_ERROR;
        }

        /*Do Decryption*/
        status = sss_cipher_init(&symmCtx, iv, ivLen);
        if (status != kStatus_SSS_Success) {
            sss_symmetric_context_free(&symmCtx);
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_DEVICE_ERROR)
        }

        status = sss_cipher_update(
            &symmCtx, (const uint8_t *)pEncryptedData, (size_t)ulEncryptedDataLen, pOut, &tempOutBufLen);
        if (status != kStatus_SSS_Success) {
            sss_symmetric_context_free(&symmCtx);
            pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
            UNLOCK_MUTEX_FOR_RTOS_RET(CKR_DEVICE_ERROR)
            // return CKR_DEVICE_ERROR;
        }

        pOut          = pOut + tempOutBufLen;
        encDataLen    = tempOutBufLen;
        tempOutBufLen = sizeof(encData) - tempOutBufLen;

        status = sss_cipher_finish(&symmCtx, NULL, 0, pOut, &tempOutBufLen);

        if (status != kStatus_SSS_Success) {
            sss_symmetric_context_free(&symmCtx);
            LOG_E("sss_cipher_one_go failed");
            xResult = CKR_FUNCTION_FAILED;
        }
        encDataLen = encDataLen + tempOutBufLen;
        if (xResult == CKR_OK) {
            size_t i = 0;
            while ((encData[encDataLen - 1 - i] == 0) && i < encDataLen) {
                i++;
            }
            encDataLen = encDataLen - i;
            if (pData) {
                if (*pulDecryptedDataLen < encDataLen) {
                    xResult = CKR_BUFFER_TOO_SMALL;
                }
                else {
                    if (encDataLen > 0) {
                        memcpy(pData, &encData[0], encDataLen);
                    }
                    else {
                        LOG_E("memcpy failed");
                    }
                    pxSessionObj->xOperationInProgress = pkcs11NO_OPERATION;
                }
            }
            *pulDecryptedDataLen = encDataLen;
        }

        sss_symmetric_context_free(&symmCtx);

        UNLOCK_MUTEX_FOR_RTOS
    }

    return xResult;
}
