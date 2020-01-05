/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <fsl_sss_sscp_apis.h>

#if SSS_HAVE_A71CH || SSS_HAVE_SE050_EAR

#include <fsl_sscp_a71ch.h>

#include <HLSEAPI.h>
#include <a71ch_api.h>
#include <assert.h>
#include <fsl_sscp.h>
#include <fsl_sscp_commands.h>
#include <fsl_sss_keyid_map.h>
#include <stdio.h>
#include <string.h>

/* ************************************************************************** */
/* Includes                                                                   */
/* ************************************************************************** */

/* ************************************************************************** */
/* Local Defines                                                              */
/* ************************************************************************** */

/* ************************************************************************** */
/* Structures and Typedefs                                                    */
/* ************************************************************************** */

/* ************************************************************************** */
/* Global Variables                                                           */
/* ************************************************************************** */

keyStoreTable_t gkeystore_shadow;

/* ************************************************************************** */
/* Static function declarations                                               */
/* ************************************************************************** */

static HLSE_RET_CODE a71ch_GenerateKey(keyStoreTable_t *keystore_shadow, uint8_t keyType, uint32_t extKeyID);
static HLSE_RET_CODE a71ch_EraseKey(keyStoreTable_t *keystore_shadow, uint8_t keyType, uint32_t extKeyID);
static HLSE_RET_CODE a71ch_FreezeKey(keyStoreTable_t *keystore_shadow, uint8_t keyType, uint32_t extKeyID);

static sscp_status_t swToSSCPResult(uint16_t checkSW);

static HLSE_RET_CODE a71ch_AllocateKeyStore(sss_a71ch_key_store_t *keyStore, uint32_t keyStoreID);

static HLSE_RET_CODE a71ch_loadKeyStore(sss_a71ch_key_store_t *keyStore);
static HLSE_RET_CODE a71ch_saveKeyStore(sss_a71ch_key_store_t *keyStore);

static HLSE_RET_CODE a71ch_AllocateKeyObject(
    sss_a71ch_key_store_t *keyStore, uint32_t extKeyID, sss_key_type_t keyType, size_t keyByteLenMax, uint32_t options);

static HLSE_RET_CODE a71ch_KeyObjectGetHandle(sss_a71ch_key_store_t *keyStore, uint32_t extKeyID, uint32_t *keyType);

static HLSE_RET_CODE a71ch_GenerateKey(keyStoreTable_t *keystore_shadow, uint8_t keyType, uint32_t extKeyID);

static HLSE_RET_CODE a71ch_getKey(
    keyStoreTable_t *keystore_shadow, sss_key_type_t key_type, uint32_t extId, uint8_t *key, uint8_t *keyLen);

static HLSE_RET_CODE a71ch_setKey(
    keyStoreTable_t *keystore_shadow, sss_key_type_t key_type, uint32_t extId, uint8_t *key, uint8_t keyLen);

static HLSE_RET_CODE a71ch_DhDeriveKey(sss_a71ch_key_store_t *keyStore,
                                       uint32_t keyID,
                                       uint8_t *pubKeyBuf,
                                       size_t pubKeyBufLen,
                                       uint8_t *pDerivekey,
                                       size_t *pDerivekeykeylen);

static HLSE_RET_CODE a71ch_SymDeriveKey(sss_a71ch_key_store_t *keyStore,
                                        uint32_t extKeyID,
                                        const uint8_t *saltData,
                                        size_t saltLen,
                                        const uint8_t *info,
                                        size_t infoLen,
                                        uint8_t *pDerivekey,
                                        size_t derivekeykeylen);

static HLSE_RET_CODE a71_eccSign(keyStoreTable_t *keystore_shadow,
                                 uint8_t extKeyID,
                                 uint8_t *pHash,
                                 uint16_t hashLen,
                                 uint8_t *pSignature,
                                 uint16_t *pSignatureLen);

static HLSE_RET_CODE a71_eccVerify(keyStoreTable_t *keystore_shadow,
                                   uint8_t extKeyID,
                                   uint8_t *pHash,
                                   uint16_t hashLen,
                                   uint8_t *pSignature,
                                   uint16_t SignatureLen);

/* ************************************************************************** */
/* Public Functions                                                           */
/* ************************************************************************** */

sss_status_t sscp_a71ch_init(sscp_a71ch_context_t *context, sss_a71ch_key_store_t *keyStore)
{
    /* assign A71ch implementation of ::sscp_invoke_command() */
    context->keyStore = keyStore;
    context->invoke = &sscp_a71ch_invoke_command;
    keyStore->session->sscp_context = (sscp_context_t *)context;
    return kStatus_SSS_Success;
}

void sscp_a71ch_free(sscp_a71ch_context_t *context)
{
    if (context != NULL)
    {
        memset(context, 0, sizeof(*context));
    }
}

sscp_status_t sscp_a71ch_invoke_command(sscp_context_t *a71chContext,
                                        uint32_t commandID,
                                        sscp_operation_t *op,
                                        uint32_t *returnOrigin)
{
    uint16_t resSW = SMCOM_SND_FAILED;
    sscp_status_t retSSCP;
    uint8_t keyId;

    sscp_a71ch_context_t *context = (sscp_a71ch_context_t *)a71chContext;

    switch (commandID)
    {
        case TEE_CMD_GENERATE_KEY:
            resSW = a71ch_GenerateKey(context->keyStore->keystore_shadow, op->params[0].value.a, op->params[0].value.b);
            break;

        case TEE_CMD_ERASE_KEY:
            resSW = a71ch_EraseKey(context->keyStore->keystore_shadow, op->params[0].value.a, op->params[0].value.b);
            if (resSW = SMCOM_OK)
                resSW = a71ch_saveKeyStore(context->keyStore);
            break;

        case TEE_CMD_GET_KEY:
            resSW = a71ch_getKey(context->keyStore->keystore_shadow, op->params[0].value.a, op->params[0].value.b,
                                 op->params[1].memref.buffer, (uint8_t *)&op->params[1].memref.size);
            break;

        case TEE_CMD_FREEZE_KEY:
            resSW = a71ch_FreezeKey(context->keyStore->keystore_shadow, op->params[0].value.a, op->params[0].value.b);
            break;

        case TEE_CMD_ALLOCATE_KEYSTORE:
            resSW = a71ch_AllocateKeyStore(context->keyStore, op->params[0].value.a);
            break;

        case TEE_CMD_LOAD_KEYSTORE:
            resSW = a71ch_loadKeyStore(context->keyStore);
            break;

        case TEE_CMD_SAVE_KEYSTORE:
            resSW = a71ch_saveKeyStore(context->keyStore);
            break;
        case TEE_CMD_SET_KEY:
            resSW = a71ch_setKey(context->keyStore->keystore_shadow, op->params[0].value.a, op->params[0].value.b,
                                 op->params[1].memref.buffer, (uint8_t)op->params[1].memref.size);
            break;

        case TEE_KEYOBJ_CMD_ALLOCATE_HANDLE:
            resSW = a71ch_AllocateKeyObject(context->keyStore, op->params[0].value.a, op->params[0].value.b,
                                            op->params[1].value.a, op->params[1].value.b);
            break;

        case TEE_KEYOBJ_CMD_GET_HANDLE:
            resSW = a71ch_KeyObjectGetHandle(context->keyStore, op->params[0].value.a, &(op->params[1].value.a));
            break;

        case TEE_KEYOBJ_CMD_SET_USER:
            break;

        case TEE_KEYOBJ_CMD_SET_PURPOSE:
            break;

        case TEE_KEYOBJ_CMD_SET_ACCESS:
            break;

        case TEE_KEYOBJ_CMD_GET_USER:
            break;

        case TEE_KEYOBJ_CMD_GET_PURPOSE:
            break;

        case TEE_KEYOBJ_CMD_GET_ACCESS:
            break;

        case TEE_ASYMMETRIC_CTX_INIT:
            op->params[2].value.a = (0xFFFFFFFFu ^ op->params[1].value.a);
            resSW = SMCOM_OK;
            break;
        case TEE_ASYMMETRIC_CMD_ENCRYPT:
            break;
        case TEE_ASYMMETRIC_CMD_DECRYPT:
            break;
        case TEE_ASYMMETRIC_CMD_SIGN_DIGEST:
            keyId = (0xFFFFFFFFu ^ op->params[0].value.a);
            resSW = a71_eccSign(context->keyStore->keystore_shadow, keyId, op->params[1].memref.buffer,
                                (uint16_t)op->params[1].memref.size, op->params[2].memref.buffer,
                                (uint16_t *)&op->params[2].memref.size);
            break;
        case TEE_ASYMMETRIC_CMD_VERIFY_DIGEST:
            keyId = (0xFFFFFFFFu ^ op->params[0].value.a);
            resSW = a71_eccVerify(context->keyStore->keystore_shadow, keyId, op->params[1].memref.buffer,
                                  (uint16_t)op->params[1].memref.size, op->params[2].memref.buffer,
                                  (uint16_t)op->params[2].memref.size);
            break;

        case TEE_SYMM_CIPHER_CTX_INIT:
            op->params[2].value.a = (0xFFFFFFFFu ^ op->params[1].value.a);
            resSW = SMCOM_OK;
            break;
        case TEE_SYMM_CIPHER_ONE_GO:
            break;
        case TEE_SYMM_CIPHER_INIT:
            break;
        case TEE_SYMM_CIPHER_UPDATE:
            break;
        case TEE_SYMM_CIPHER_FINISH:
            break;
        case TEE_SYMM_CIPHER_CRYPT_CTR:
            break;

        case TEE_DERIVE_CTX_INIT:
            op->params[2].value.a = (0xFFFFFFFF ^ op->params[1].value.a);
            resSW = SMCOM_OK;
            break;

        case TEE_ASYMM_DH_DERIVE_KEY:
            resSW =
                a71ch_DhDeriveKey(context->keyStore, (0xFFFFFFFF ^ (op->params[0].value.a)), /* = context->sessionId*/
                                  op->params[1].memref.buffer,                               /* key*/
                                  op->params[1].memref.size,                                 /*keylen*/
                                  op->params[2].memref.buffer,                               /* derivekey*/
                                  &(op->params[2].memref.size));                             /* derivekeykeylen;*/
            break;

        case TEE_DERIVE_KEY:
            resSW = a71ch_SymDeriveKey(context->keyStore,
                                       (0xFFFFFFFF ^ (op->params[0].value.a)), /* extKeyID = ~context->sessionId*/
                                       op->params[1].memref.buffer,            /*  (void *)saltData*/
                                       op->params[1].memref.size,              /*  saltLen    */
                                       op->params[2].memref.buffer,            /*  info  */
                                       op->params[2].memref.size,              /*   infoLen    */
                                       op->params[2].memref.buffer,            /* derivekey*/
                                       op->params[2].memref.size);             /* derivekeykeylen;*/
            break;

        default:
            printf("Not a SSCP command");
    }
    retSSCP = swToSSCPResult(resSW);
    return (retSSCP);
}

/* ************************************************************************** */
/* Private Functions                                                          */
/* ************************************************************************** */

static HLSE_RET_CODE a71ch_SymDeriveKey(sss_a71ch_key_store_t *keyStore,
                                        uint32_t extKeyID,
                                        const uint8_t *saltData,
                                        size_t saltLen,
                                        const uint8_t *info,
                                        size_t infoLen,
                                        uint8_t *pDerivekey,
                                        size_t derivekeykeylen)
{
    HLSE_RET_CODE hlseret;
    uint16_t intId = 0;
    uint8_t nBlock = 1;

    sss_status_t status = ks_common_extId_to_int_index(keyStore->keystore_shadow, extKeyID, &intId);
    if (status != kStatus_SSS_Success)
    {
        printf("ERROR: Could not find Ext ID 0x%08X in keystore_shadow", extKeyID);
        hlseret = HLSE_ERR_API_ERROR;
    }
    else
    {
        hlseret = A71_HkdfSymKey((U8)intId, nBlock, saltData, (uint16_t)saltLen, info, (uint16_t)infoLen, pDerivekey,
                                 (uint16_t)derivekeykeylen);
    }

    return hlseret;
}

static HLSE_RET_CODE a71ch_DhDeriveKey(sss_a71ch_key_store_t *keyStore,
                                       uint32_t extKeyID,
                                       uint8_t *pubKeyBuf,
                                       size_t pubKeyBufLen,
                                       uint8_t *pDerivekey,
                                       size_t *pDerivekeykeylen)
{
    HLSE_RET_CODE hlseret;
    uint16_t intId = 0;
    uint16_t derivedKeylen;

    sss_status_t status = ks_common_extId_to_int_index(keyStore->keystore_shadow, extKeyID, &intId);
    if (status != kStatus_SSS_Success)
    {
        printf("ERROR: Could not find Ext ID 0x%08X in keystore_shadow", extKeyID);
        hlseret = HLSE_ERR_API_ERROR;
    }
    else
    {
        derivedKeylen = (uint16_t)(*pDerivekeykeylen);
        hlseret = A71_EcdhGetSharedSecret((U8)intId, pubKeyBuf, (U16)pubKeyBufLen, pDerivekey, &derivedKeylen);
        // Doing this as pointer types are different
        *pDerivekeykeylen = derivedKeylen;
    }

    return hlseret;
}

static HLSE_RET_CODE a71ch_GenerateKey(keyStoreTable_t *keystore_shadow, uint8_t keyType, uint32_t extKeyID)
{
    HLSE_RET_CODE hlseret;
    if (keyType == kSSS_KeyType_ECC_Pair)
    {
        uint16_t intId = 0;
        sss_status_t status = ks_common_extId_to_int_index(keystore_shadow, extKeyID, &intId);
        if (status == kStatus_SSS_Success)
        {
            hlseret = A71_GenerateEccKeyPair((U8)intId);
        }
        else
        {
            printf("ERROR: Could not find Ext ID 0x%08X in keystore_shadow", extKeyID);
            hlseret = HLSE_ERR_API_ERROR;
        }
    }
    return hlseret;
}

static HLSE_RET_CODE a71ch_EraseKey(keyStoreTable_t *keystore_shadow, uint8_t keyType, uint32_t extKeyID)
{
    HLSE_RET_CODE hlseret = HLSE_ERR_API_ERROR;
    uint16_t intId = 0;
    sss_status_t status = ks_common_extId_to_int_index(keystore_shadow, extKeyID, &intId);
    if (status != kStatus_SSS_Success)
    {
        printf("ERROR: Could not find Ext ID 0x%08X in keystore_shadow", extKeyID);
        hlseret = HLSE_ERR_API_ERROR;
    }
    else
    {
        if (keyType == kSSS_KeyType_ECC_Pair)
        {
            hlseret = A71_EraseEccKeyPair((U8)intId);
        }
        else if (keyType == kSSS_KeyType_ECC_Public)
        {
            hlseret = A71_EraseEccPublicKey((U8)intId);
        }
    }
    if (hlseret == HLSE_SW_OK)
    {
        status = ks_common_remove_fat(keystore_shadow, extKeyID);
        if (status != kStatus_SSS_Success)
        {
            hlseret = HLSE_ERR_API_ERROR;
        }
    }
    return hlseret;
}

static HLSE_RET_CODE a71ch_FreezeKey(keyStoreTable_t *keystore_shadow, uint8_t keyType, uint32_t extKeyID)
{
    HLSE_RET_CODE hlseret;
    uint16_t intId = 0;
    sss_status_t status = ks_common_extId_to_int_index(keystore_shadow, extKeyID, &intId);
    if (status != kStatus_SSS_Success)
    {
        printf("ERROR: Could not find Ext ID 0x%08X in keystore_shadow", extKeyID);
        hlseret = HLSE_ERR_API_ERROR;
    }
    else
    {
        if (keyType == kSSS_KeyType_ECC_Pair)
        {
            hlseret = A71_FreezeEccKeyPair((U8)intId);
        }
        if (keyType == kSSS_KeyType_ECC_Public)
        {
            hlseret = A71_FreezeEccPublicKey((U8)intId);
        }
    }

    return hlseret;
}

static sscp_status_t swToSSCPResult(uint16_t checkSW)
{
    switch (checkSW)
    {
        case SMCOM_OK:
            return kStatus_SSCP_Success;
        default:
            return kStatus_SSCP_Fail;
    }
}

static HLSE_RET_CODE a71ch_AllocateKeyStore(sss_a71ch_key_store_t *keyStore, uint32_t keyStoreID)
{
    HLSE_RET_CODE hlseret;
    HLSE_OBJECT_HANDLE Handles[5];
    U16 HandlesNum = sizeof(Handles) / sizeof(HLSE_OBJECT_HANDLE);
    U16 HandlesNum_copy;
    U8 i = 0;
    HLSE_OBJECT_INDEX index = 0;

    /* Search for our data table @ index */
    if (keyStore->shadow_handle == 0)
    {
        hlseret = HLSE_EnumerateObjects(HLSE_DATA, Handles, &HandlesNum);
        if (hlseret == HLSE_SW_OK)
        {
            HandlesNum_copy = HandlesNum;
            while (HandlesNum_copy)
            {
                if (HLSE_GET_OBJECT_INDEX(Handles[i]) == index)
                {
                    keyStore->shadow_handle = Handles[i];
                    break;
                }
                i++;
                HandlesNum_copy--;
            }
        }
    }

    /* If it was never there, create it @ index */
    if (keyStore->shadow_handle == 0)
    {
        /* Could not find it yet*/
        HLSE_OBJECT_TYPE objType = HLSE_DATA;
        U16 templateSize = 3;
        HLSE_ATTRIBUTE attr[3];
        ks_common_init_fat(&gkeystore_shadow);
        attr[0].type = HLSE_ATTR_OBJECT_TYPE;
        attr[0].value = &objType;
        attr[0].valueLen = sizeof(objType);
        attr[1].type = HLSE_ATTR_OBJECT_INDEX;
        attr[1].value = &index;
        attr[1].valueLen = sizeof(index);
        attr[2].type = HLSE_ATTR_OBJECT_VALUE;
        attr[2].value = &gkeystore_shadow;
        attr[2].valueLen = sizeof(gkeystore_shadow);

        hlseret = HLSE_CreateObject(attr, templateSize, &keyStore->shadow_handle);
    }

    /* Either we created it. Or it was already existing, read it. */
    if (keyStore->shadow_handle != 0)
    {
        hlseret = a71ch_loadKeyStore(keyStore);
    }
    return hlseret;
}

static HLSE_RET_CODE a71ch_loadKeyStore(sss_a71ch_key_store_t *keyStore)
{
    HLSE_RET_CODE hlseret;
    HLSE_ATTRIBUTE attr;
    attr.type = HLSE_ATTR_OBJECT_VALUE;
    attr.value = &gkeystore_shadow;
    attr.valueLen = sizeof(gkeystore_shadow);
    /* Read from Key Store and load it here in gkeystore_shadow */
    hlseret = HLSE_GetObjectAttribute(keyStore->shadow_handle, &attr);
    keyStore->keystore_shadow = &gkeystore_shadow;
    return hlseret;
}
static HLSE_RET_CODE a71ch_saveKeyStore(sss_a71ch_key_store_t *keyStore)
{
    HLSE_RET_CODE hlseret;
    if (keyStore->shadow_handle == 0)
    {
        hlseret = HLSE_ERR_API_ERROR;
    }
    else
    {
        HLSE_ATTRIBUTE attr;
        attr.type = HLSE_ATTR_OBJECT_VALUE;
        attr.value = keyStore->keystore_shadow;
        attr.valueLen = sizeof(*keyStore->keystore_shadow);
        /* write gkeystore_shadow */
        hlseret = HLSE_SetObjectAttribute(keyStore->shadow_handle, &attr);
    }
    return hlseret;
}

static HLSE_RET_CODE a71ch_KeyObjectGetHandle(sss_a71ch_key_store_t *keyStore, uint32_t extKeyID, uint32_t *keyType)
{
    HLSE_RET_CODE hlseret = HLSE_ERR_GENERAL_ERROR;
    sss_status_t ks_status = kStatus_SSS_Success;
    *keyType = kSSS_KeyType_NONE;
    ks_status = ks_common_get_keyType_from_keyid(keyStore->keystore_shadow, extKeyID, keyType);
    if (ks_status == kStatus_SSS_Success)
        hlseret = HLSE_SW_OK;
    return hlseret;
}
static HLSE_RET_CODE a71ch_AllocateKeyObject(
    sss_a71ch_key_store_t *keyStore, uint32_t extKeyID, sss_key_type_t keyType, size_t keyByteLenMax, uint32_t options)
{
    HLSE_RET_CODE hlseret = HLSE_SW_OK;
    uint16_t intIndex = 0;
    bool found_new_slot = 0;
    sss_status_t ks_status;

    ks_status = ks_common_check_available_int_index(keyStore->keystore_shadow, (uint8_t)keyType, &intIndex);

    if (ks_status != kStatus_SSS_Success)
    {
        hlseret = HLSE_ERR_MEMORY;
    }

    if (hlseret == HLSE_SW_OK)
    {
        ks_status = ks_common_update_fat(keyStore->keystore_shadow, extKeyID, (uint8_t)keyType, intIndex);
    }
    if (ks_status != kStatus_SSS_Success)
    {
        hlseret = HLSE_ERR_MEMORY;
    }
    if (hlseret == HLSE_SW_OK)
    {
        switch (keyType)
        {
            case kSSS_KeyType_Certificate:
            {
                HLSE_OBJECT_INDEX index = intIndex;
                HLSE_OBJECT_TYPE objType = HLSE_CERTIFICATE;
                U16 templateSize = 3;
                HLSE_ATTRIBUTE attr[3];
                HLSE_OBJECT_HANDLE handle = 0;
                attr[0].type = HLSE_ATTR_OBJECT_TYPE;
                attr[0].value = &objType;
                attr[0].valueLen = sizeof(objType);
                attr[1].type = HLSE_ATTR_OBJECT_INDEX;
                attr[1].value = &index;
                attr[1].valueLen = sizeof(index);
                attr[2].type = HLSE_ATTR_OBJECT_VALUE;
                /* This is dummy value we write... Actually this source code */
                attr[2].value = &a71ch_AllocateKeyObject;
                attr[2].valueLen = (U16)keyByteLenMax;

                hlseret = HLSE_CreateObject(attr, templateSize, &handle);
                /* TODO: if object is already present and all attributes are same then mark it as success*/
                if (hlseret == HLSE_OBJ_ALREADY_EXISTS)
                    hlseret = HLSE_SW_OK;
                break;
            }
            case kSSS_KeyType_ECC_Pair:
            case kSSS_KeyType_ECC_Public:
            case kSSS_KeyType_AES:
                break;
            default:
                hlseret = HLSE_ERR_API_ERROR;
        }
    }
    if (hlseret == HLSE_SW_OK)
    {
        /* Persist to EEPROM */
        hlseret = a71ch_saveKeyStore(keyStore);
    }
    else
    {
        /* Reset the structure based on EEPROM */
        a71ch_loadKeyStore(keyStore);
    }
    return hlseret;
}

static HLSE_RET_CODE a71ch_getKey(
    keyStoreTable_t *keystore_shadow, sss_key_type_t key_type, uint32_t extId, uint8_t *key, uint8_t *keyLen)
{
    HLSE_RET_CODE resSW = HLSE_ERR_API_ERROR;
    uint16_t intId = 0;
    U8 i = 0;
    HLSE_OBJECT_HANDLE Handles[5];
    HLSE_OBJECT_HANDLE GetHandle = 0;
    U16 HandlesNum = sizeof(Handles) / sizeof(HLSE_OBJECT_HANDLE);
    U16 HandlesNum_copy;
    HLSE_ATTRIBUTE attr;
    /* clang-format off */
    const uint8_t der_header[] = { 0x30, 0x59, 0x30, 0x13,0x06, 0x07, 0x2A, 0x86,
                                   0x48, 0xCE, 0x3D, 0x02,0x01, 0x06, 0x08, 0x2A,
                                   0x86, 0x48, 0xCE, 0x3D,0x03, 0x01, 0x07, 0x03,
                                   0x42, 0x00
    };
    /* clang-format on */
    sss_status_t status = ks_common_extId_to_int_index(keystore_shadow, extId, &intId);
    if (status == kStatus_SSS_Success)
    {
        if (key_type == kSSS_KeyType_ECC_Pair)
        {
            memcpy(key, der_header, sizeof(der_header));
            resSW = A71_GetPublicKeyEccKeyPair((U8)intId, key + 26, (U16 *)keyLen);
            *keyLen = *keyLen + 26;
        }
        else if (key_type == kSSS_KeyType_ECC_Public)
        {
            memcpy(key, der_header, sizeof(der_header));
            resSW = A71_GetEccPublicKey((U8)intId, key + 26, (U16 *)keyLen);
            *keyLen = *keyLen + 26;
        }
        else if (key_type == kSSS_KeyType_Certificate)
        {
            resSW = HLSE_EnumerateObjects(HLSE_CERTIFICATE, Handles, &HandlesNum);
            if (resSW == HLSE_SW_OK)
            {
                HandlesNum_copy = HandlesNum;
                while (HandlesNum_copy)
                {
                    if (HLSE_GET_OBJECT_INDEX(Handles[i]) == intId)
                    {
                        GetHandle = Handles[i];
                        break;
                    }
                    i++;
                    HandlesNum_copy--;
                }
            }
            if (GetHandle != 0)
            {
                attr.type = HLSE_ATTR_OBJECT_VALUE;
                attr.value = key;
                attr.valueLen = (U16)keyLen;
                resSW = HLSE_GetObjectAttribute(GetHandle, &attr);
            }
        }
    }
    else
    {
        printf("ERROR: Could not find Ext ID 0x%08X in keystore_shadow", extId);
    }
    return resSW;
}

static HLSE_RET_CODE a71ch_setKey(
    keyStoreTable_t *keystore_shadow, sss_key_type_t key_type, uint32_t extId, uint8_t *key, uint8_t keyLen)
{
    HLSE_RET_CODE resSW = HLSE_ERR_API_ERROR;
    uint16_t intId = 0;
    U8 i = 0;
    HLSE_OBJECT_HANDLE Handles[5];
    HLSE_OBJECT_HANDLE GetHandle = 0;
    U16 HandlesNum = sizeof(Handles) / sizeof(HLSE_OBJECT_HANDLE);
    U16 HandlesNum_copy;
    HLSE_ATTRIBUTE attr;
    sss_status_t status = ks_common_extId_to_int_index(keystore_shadow, extId, &intId);
    if (status == kStatus_SSS_Success)
    {
        if (key_type == kSSS_KeyType_ECC_Public && keyLen == (91))
        {
            key = key + 26;
            keyLen = keyLen - 26;
            resSW = A71_SetEccPublicKey((U8)intId, key, keyLen);
        }
        else if (key_type == kSSS_KeyType_AES)
        {
            resSW = A71_SetSymKey((U8)intId, key, keyLen);
        }
        else if ((key_type == kSSS_KeyType_ECC_Pair) && keyLen == (138))
        {
            uint8_t *pPrivateKey = &key[36];
            uint16_t privateKeyLen = 32;
            uint8_t *pPublicKey = &key[73];
            uint16_t publicKeyLen = 65;

            resSW = A71_SetEccKeyPair((U8)intId, pPublicKey, (U16)publicKeyLen, pPrivateKey, (U16)privateKeyLen);
        }
        else if (key_type == kSSS_KeyType_Certificate)
        {
            resSW = HLSE_EnumerateObjects(HLSE_CERTIFICATE, Handles, &HandlesNum);
            if (resSW == HLSE_SW_OK)
            {
                HandlesNum_copy = HandlesNum;
                while (HandlesNum_copy)
                {
                    if (HLSE_GET_OBJECT_INDEX(Handles[i]) == intId)
                    {
                        GetHandle = Handles[i];
                        break;
                    }
                    i++;
                    HandlesNum_copy--;
                }
            }
            if (GetHandle != 0)
            {
                attr.type = HLSE_ATTR_OBJECT_VALUE;
                attr.value = key;
                attr.valueLen = keyLen;
                resSW = HLSE_SetObjectAttribute(GetHandle, &attr);
            }
        }
    }
    else
    {
        printf("ERROR: Could not find Ext ID 0x%08X in keystore_shadow", extId);
    }
    return resSW;
}

static HLSE_RET_CODE a71_eccSign(keyStoreTable_t *keystore_shadow,
                                 uint8_t extKeyID,
                                 uint8_t *pHash,
                                 uint16_t hashLen,
                                 uint8_t *pSignature,
                                 uint16_t *pSignatureLen)
{
    uint16_t intId = 0;
    HLSE_RET_CODE resSW = HLSE_ERR_API_ERROR;
    sss_status_t status = ks_common_extId_to_int_index(keystore_shadow, extKeyID, &intId);
    if (status == kStatus_SSS_Success)
    {
        resSW = A71_EccSign((SST_Index_t)intId, pHash, hashLen, pSignature, pSignatureLen);
    }
    return resSW;
}

static HLSE_RET_CODE a71_eccVerify(keyStoreTable_t *keystore_shadow,
                                   uint8_t extKeyID,
                                   uint8_t *pHash,
                                   uint16_t hashLen,
                                   uint8_t *pSignature,
                                   uint16_t SignatureLen)
{
    uint16_t intId = 0;
    uint8_t result;
    HLSE_RET_CODE resSW = HLSE_ERR_API_ERROR;
    sss_status_t status = ks_common_extId_to_int_index(keystore_shadow, extKeyID, &intId);
    if (status == kStatus_SSS_Success)
    {
        resSW = A71_EccVerify((SST_Index_t)intId, pHash, hashLen, pSignature, SignatureLen, &result);
        printf("result - %d", result);
    }
    return resSW;
}

#endif /* SSS_HAVE_A71CH */
