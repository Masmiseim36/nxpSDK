/*
 * Copyright 2018,2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <fsl_sss_sscp.h>

#if SSS_HAVE_A71CH || SSS_HAVE_SE050_EAR_CH

#include <HLSEAPI.h>
#include <a71ch_api.h>
#include <fsl_sscp.h>
#include <fsl_sscp_a71ch.h>
#include <fsl_sscp_commands.h>
#include <fsl_sss_keyid_map.h>
#include <stdio.h>
#include <string.h>

#include "ax_sss_scp.h"
#include "sm_apdu.h"
#include "stdlib.h"
//#define NX_LOG_ENABLE_SSCP_DEBUG 1
#include <nxLog_sscp.h>
#include <nxScp03_Types.h>

/* ************************************************************************** */
/* Includes                                                                   */
/* ************************************************************************** */

/* ************************************************************************** */
/* Local Defines                                                              */
/* ************************************************************************** */
#define DER_ECC_NISTP256_HEADER 26
#define ADD_DER_ECC_NISTP256_HEADER(x) (x) + DER_ECC_NISTP256_HEADER
#define REMOVE_DER_ECC_NISTP256_HEADER(x) (x) - DER_ECC_NISTP256_HEADER
#define CONVERT_BYTE(x) (x) / 8
#define CONVERT_BIT(x) (x) * 8

/* ************************************************************************** */
/* Structures and Typedefs                                                    */
/* ************************************************************************** */

/* ************************************************************************** */
/* Global Variables                                                           */
/* ************************************************************************** */

keyStoreTable_t gkeystore_shadow_ch;
keyIdAndTypeIndexLookup_t gLookupEntires_ch[KS_N_ENTIRES];

typedef struct
{
    /** Fixed - Unique 32bit magic number.
    *
    * In case some one over-writes we can know. */
    uint32_t magic;
    /** Fixed - constant based on version number */
    uint16_t version;

    uint16_t maxEntries;
    /** Dynamic entries */
    keyIdAndTypeIndexLookup_t entries[KS_N_ENTIRES];
} keyStoreTableEEPROM_t;
/* ************************************************************************** */
/* Static function declarations                                               */
/* ************************************************************************** */

static HLSE_RET_CODE a71ch_GenerateKey(keyStoreTable_t *keystore_shadow,
    uint8_t keyType,
    uint8_t cipherType,
    uint32_t extKeyID);
static HLSE_RET_CODE a71ch_EraseKey(keyStoreTable_t *keystore_shadow,
    uint8_t keyType,
    uint8_t cipherType,
    uint32_t extKeyID);
static HLSE_RET_CODE a71ch_FreezeKey(keyStoreTable_t *keystore_shadow,
    uint8_t keyType,
    uint8_t cipherType,
    uint32_t extKeyID);

static sss_status_t swToSSSResult(uint16_t checkSW);

static HLSE_RET_CODE a71ch_AllocateKeyStore(
    sss_a71ch_key_store_t *keyStore, uint32_t keyStoreID);

static HLSE_RET_CODE a71ch_loadKeyStore(sss_a71ch_key_store_t *keyStore);
static HLSE_RET_CODE a71ch_saveKeyStore(sss_a71ch_key_store_t *keyStore);

static HLSE_RET_CODE a71ch_AllocateKeyObject(sss_a71ch_key_store_t *keyStore,
    uint32_t extKeyID,
    sss_key_part_t keyPart,
    sss_cipher_type_t cipherType,
    size_t keyByteLenMax,
    uint32_t options);

static HLSE_RET_CODE a71ch_KeyObjectGetHandle(sss_a71ch_key_store_t *keyStore,
    uint32_t extKeyID,
    uint32_t *keyType,
    uint32_t *cipherType);

static HLSE_RET_CODE a71ch_GenerateKey(keyStoreTable_t *keystore_shadow,
    uint8_t keyType,
    uint8_t cipherType,
    uint32_t extKeyID);

static HLSE_RET_CODE a71ch_getKey(keyStoreTable_t *keystore_shadow,
    sss_key_part_t key_part,
    sss_cipher_type_t cipher_type,
    uint32_t extId,
    uint8_t *key,
    uint32_t *keyByteLen,
    uint32_t *keyBitLen);

static HLSE_RET_CODE a71ch_setKey(keyStoreTable_t *keystore_shadow,
    sss_key_part_t key_part,
    sss_cipher_type_t cipher_type,
    uint32_t extId,
    uint8_t *key,
    size_t keyLen);

static HLSE_RET_CODE a71ch_DhDeriveKey(sss_a71ch_key_store_t *keyStore,
    uint32_t extKeyID,
    uint8_t *pubKeyBuf,
    size_t pubKeyBufBitLen,
    uint8_t *pDerivekey,
    size_t *pDerivekeykeyBitlen);

static HLSE_RET_CODE a71ch_SymDeriveKey(sss_a71ch_key_store_t *keyStore,
    uint32_t extKeyID,
    const uint8_t *saltData,
    size_t saltLen,
    const uint8_t *info,
    size_t infoLen,
    uint8_t *pDerivekey,
    size_t derivekeykeylen);

static HLSE_RET_CODE a71ch_CalHmacSha256(sss_a71ch_key_store_t *keyStore,
    uint32_t extKeyID,
    const uint8_t *info,
    size_t infoLen,
    uint8_t *hmac,
    size_t hmaclen);

static HLSE_RET_CODE a71_eccSign(keyStoreTable_t *keystore_shadow,
    uint32_t extKeyID,
    uint8_t *pHash,
    uint16_t hashLen,
    uint8_t *pSignature,
    uint16_t *pSignatureLen);

static HLSE_RET_CODE a71_eccVerify(keyStoreTable_t *keystore_shadow,
    uint32_t extKeyID,
    uint8_t *pHash,
    uint16_t hashLen,
    uint8_t *pSignature,
    uint16_t SignatureLen);

/* ************************************************************************** */
/* Public Functions                                                           */
/* ************************************************************************** */

sss_status_t sscp_a71ch_openSession(
    void *connectionData, sss_sscp_session_t *openSession)

{
    uint32_t retval = kStatus_SSS_Fail;
    if (connectionData == NULL) {
        return kStatus_SSS_InvalidArgument;
    }
    SE_Connect_Ctx_t *pA71Auth = (SE_Connect_Ctx_t *)connectionData;
    memset(openSession, 0, sizeof(*openSession));
    SmCommState_t CommState = {0};
    CommState.connType = pA71Auth->connType;
    uint8_t atr[100];
    uint16_t atrLen = ARRAY_SIZE(atr);
    U16 lReturn;

#if defined(SMCOM_JRCP_V1) || defined(SMCOM_JRCP_V2) || defined(RJCT_VCOM) || \
    defined(SMCOM_PCSC)
    lReturn = SM_RjctConnect(pA71Auth->portName, &CommState, atr, &atrLen);
    {
        if (lReturn == SW_OK) {
            openSession->fp_closeConnection = &sscp_a71ch_closeConnect;
            retval = kStatus_SSS_Success;
        }
        else if (lReturn == ERR_COM_ALREADY_OPEN)
            retval = kStatus_SSS_Success;
        else if (lReturn == ERR_API_ERROR)
            retval = kStatus_SSS_InvalidArgument;
    }
#else
    /* AX_EMBEDDED Or Native */
    lReturn = SM_Connect(&CommState, atr, &atrLen);
    if (lReturn != SW_OK) {
        LOG_E("SM_Connect Failed. Status %04X", lReturn);
        return retval;
    }
    else {
        retval = kStatus_SSS_Success;
    }
#endif
    if (retval == kStatus_SSS_Success) {
#if SSS_HAVE_SCP_SCP03_SSS
        if ((retval == kStatus_SSS_Success) &&
            (pA71Auth->auth.authType == kSSS_AuthType_SCP03)) {
            retval = kStatus_SSS_Fail;

            LOG_I("Invoking SM_ChannelAuthenticate()");
            lReturn =
                SM_ChannelAuthenticate(&pA71Auth->auth.ctx.a71chAuthKeys, NULL);
            if (lReturn == SW_OK) {
                retval = kStatus_SSS_Success;
            }
            else {
                LOG_E("SM_ChannelAuthenticate() failed.");
            }
        }
#endif
    }

    return (sss_status_t)retval;
}

void sscp_a71chkey_store_context_free(sss_a71ch_key_store_t *context)
{
    memset(&gLookupEntires_ch, 0, sizeof(gLookupEntires_ch));
    memset(&gkeystore_shadow_ch, 0, sizeof(gkeystore_shadow_ch));
}
void sscp_a71ch_closeSession(sss_sscp_session_t *closeSession)
{
    if (closeSession->sessionId != 0) {
#if SSS_HAVE_SE050_EAR
#endif
    }
    if (closeSession->fp_closeConnection != NULL) {
        closeSession->fp_closeConnection();
        closeSession->fp_closeConnection = NULL;
    }
}
void sscp_a71ch_closeConnect(void)
{
    HLSE_CloseConnection(HLSE_CLOSE_CONNECTION_NO_RESET);
}

sss_status_t sscp_a71ch_init(
    sscp_a71ch_context_t *context, sss_a71ch_key_store_t *keyStore)
{
    /* assign A71ch implementation of ::sscp_invoke_command() */
    context->keyStore = keyStore;
    context->invoke = &sscp_a71ch_invoke_command;
    keyStore->session->sscp_context = (sscp_context_t *)context;
    return kStatus_SSS_Success;
}

void sscp_a71ch_free(sscp_a71ch_context_t *context)
{
    if (context != NULL) {
        memset(context, 0, sizeof(*context));
    }
}

void getA7CHKeyStore(sss_a71ch_key_store_t **ks, sscp_context_reference_t *ref)
{
    switch (ref->type) {
    case kSSCP_ParamContextType_SSS_Symmetric: {
        sss_sscp_symmetric_t *ctx = (sss_sscp_symmetric_t *)ref->ptr;
        *ks = (sss_a71ch_key_store_t *)ctx->keyObject->keyStore;
    } break;
    case kSSCP_ParamContextType_SSS_Aead:
        break;
    case kSSCP_ParamContextType_SSS_Digest:
        break;
    case kSSCP_ParamContextType_SSS_Mac:
        break;
    case kSSCP_ParamContextType_SSS_Tunnel:
        break;
    case kSSCP_ParamContextType_SSS_DeriveKey: {
        sss_derive_key_t *ctx = (sss_derive_key_t *)ref->ptr;
        *ks = (sss_a71ch_key_store_t *)ctx->keyObject->keyStore;
        break;
    }
    case kSSCP_ParamContextType_SSS_Asymmetric: {
        sss_asymmetric_t *ctx = (sss_asymmetric_t *)ref->ptr;
        *ks = (sss_a71ch_key_store_t *)ctx->keyObject->keyStore;
        break;
    }
    case kSSCP_ParamContextType_SSS_Object: {
        sss_object_t *pobj = (sss_object_t *)ref->ptr;
        *ks = (sss_a71ch_key_store_t *)pobj->keyStore;
        break;
    }
    case kSSCP_ParamContextType_SSS_KeyStore: {
        *ks = (sss_a71ch_key_store_t *)ref->ptr;
        break;
    }
    case kSSCP_ParamContextType_SSS_RandomGen:
        *ks = (sss_a71ch_key_store_t *)ref->ptr;
        break;
    }
}

sscp_status_t sscp_a71ch_invoke_command(sscp_context_t *a71chContext,
    uint32_t commandID,
    sscp_operation_t *op,
    uint32_t *returnOrigin)
{
    uint16_t resSW = SMCOM_SND_FAILED;
    sscp_status_t retSSCP = kStatus_SSCP_Success;
    uint32_t keyId;
    sss_a71ch_key_store_t *a71ch_keystore = NULL;
    sss_derive_key_t *derctx;
    sscp_a71ch_context_t *context = (sscp_a71ch_context_t *)a71chContext;
    if (kSSCP_ParamType_ContextReference ==
        SSCP_OP_GET_PARAM(0, op->paramTypes)) {
        getA7CHKeyStore(&a71ch_keystore, &op->params[0].context);
    }

#if SSS_HAVE_SE050_EAR
    if (a71ch_keystore->session->sessionId != 0) {
        set_SessionId_Tlv(a71ch_keystore->session->sessionId);
    }
#endif

    switch (commandID) {
    case kSSCP_CMD_GENERATE_KEY:
        resSW = a71ch_GenerateKey(a71ch_keystore->keystore_shadow,
            op->params[1].value.a,
            op->params[2].value.b,
            op->params[1].value.b);
        break;

    case kSSCP_CMD_ERASE_KEY:
        resSW = a71ch_EraseKey(a71ch_keystore->keystore_shadow,
            op->params[1].value.a,
            op->params[2].value.a,
            op->params[1].value.b);
        if (resSW == SMCOM_OK) {
            resSW = a71ch_saveKeyStore(a71ch_keystore);
        }
        break;

    case kSSCP_CMD_GET_KEY:
        resSW = a71ch_getKey(a71ch_keystore->keystore_shadow,
            op->params[1].value.a,
            op->params[3].value.b,
            op->params[1].value.b,
            op->params[2].memref.buffer,
            &op->params[3].value.a,
            &op->params[3].value.b);
        break;

    case kSSCP_CMD_FREEZE_KEY:
        resSW = a71ch_FreezeKey(a71ch_keystore->keystore_shadow,
            op->params[1].value.a,
            op->params[2].value.a,
            op->params[1].value.b);
        break;

    case kSSCP_CMD_ALLOCATE_KEYSTORE:
        resSW = a71ch_AllocateKeyStore(a71ch_keystore, op->params[1].value.a);
        break;

    case kSSCP_CMD_LOAD_KEYSTORE:
        if (a71ch_keystore->shadow_handle == 0) {
            /* It's OK. Allocate + Load here. */
            resSW = a71ch_AllocateKeyStore(a71ch_keystore, __LINE__);
        }
        else {
            resSW = a71ch_loadKeyStore(a71ch_keystore);
        }
        break;

    case kSSCP_CMD_SAVE_KEYSTORE:
        resSW = a71ch_saveKeyStore(a71ch_keystore);
        break;
    case kSSCP_CMD_SET_KEY:
        resSW = a71ch_setKey(a71ch_keystore->keystore_shadow,
            op->params[1].value.a,
            op->params[3].value.b,
            op->params[1].value.b,
            op->params[2].memref.buffer,
            op->params[2].memref.size);
        break;

    case kSSCP_KEYOBJ_CMD_ALLOCATE_HANDLE:
        resSW = a71ch_AllocateKeyObject(a71ch_keystore,
            op->params[1].value.a,
            op->params[1].value.b,
            op->params[3].value.a,
            op->params[2].value.a,
            op->params[2].value.b);
        break;

    case kSSCP_KEYOBJ_CMD_GET_HANDLE:
        resSW = a71ch_KeyObjectGetHandle(a71ch_keystore,
            op->params[1].value.a,
            &(op->params[2].value.a),
            &(op->params[2].value.b));
        break;

    case kSSCP_ASYMMETRIC_CTX_INIT:
        op->params[3].value.a = (0xFFFFFFFFu ^ op->params[2].value.a);
        resSW = SMCOM_OK;
        break;
    case kSSCP_ASYMMETRIC_CMD_ENCRYPT:
        break;
    case kSSCP_ASYMMETRIC_CMD_DECRYPT:
        break;
    case kSSCP_ASYMMETRIC_CMD_SIGN_DIGEST:
        keyId = (0xFFFFFFFFu ^ op->params[1].value.a);
        resSW = a71_eccSign(a71ch_keystore->keystore_shadow,
            keyId,
            op->params[2].memref.buffer,
            (uint16_t)op->params[2].memref.size,
            op->params[3].memref.buffer,
            (uint16_t *)&op->params[3].memref.size);
        break;
    case kSSCP_ASYMMETRIC_CMD_VERIFY_DIGEST:
        keyId = (0xFFFFFFFFu ^ op->params[1].value.a);
        resSW = a71_eccVerify(context->keyStore->keystore_shadow,
            keyId,
            op->params[2].memref.buffer,
            (uint16_t)op->params[2].memref.size,
            op->params[3].memref.buffer,
            (uint16_t)op->params[3].memref.size);
        break;

    case kSSCP_SYMM_CIPHER_CTX_INIT:
        op->params[2].value.a = (0xFFFFFFFFu ^ op->params[1].value.a);
        resSW = SMCOM_OK;
        break;

    case kSSCP_DERIVE_CTX_INIT:
        op->params[3].value.a = (0xFFFFFFFFu ^ op->params[2].value.a);
        resSW = SMCOM_OK;
        break;

    case kSSCP_ASYMM_DH_DERIVE_KEY: {
        resSW = a71ch_DhDeriveKey(a71ch_keystore,
            (0xFFFFFFFFu ^ (op->params[1].value.a)), /* = context->sessionId*/
            op->params[2].memref.buffer,             /* key*/
            op->params[2].memref.size,               /*keylen*/
            op->params[3].memref.buffer,             /* derivekey*/
            &(op->params[4].memref.size));           /* derivekeykeylen;*/
    } break;

    case kSSCP_DERIVE_KEY:
        derctx = (sss_derive_key_t *)op->params[0].context.ptr;
        if (derctx != NULL) {
            if ((derctx->algorithm == kAlgorithm_SSS_HMAC_SHA256) &&
                (derctx->mode == kMode_SSS_Mac)) {
                resSW = a71ch_CalHmacSha256(a71ch_keystore,
                    (0xFFFFFFFFu ^
                        (op->params[1]
                                .value.a)), /* extKeyID = ~context->sessionId*/
                    op->params[3].memref.buffer, /*  info  */
                    op->params[3].memref.size,   /*   infoLen    */
                    op->params[4].memref.buffer, /* hmac*/
                    op->params[4].memref.size);  /* hmaclen;*/
            }
            else if ((derctx->algorithm == kAlgorithm_SSS_HMAC_SHA256) &&
                     (derctx->mode == kMode_SSS_ComputeSharedSecret)) {
                resSW = a71ch_SymDeriveKey(a71ch_keystore,
                    (0xFFFFFFFFu ^
                        (op->params[1]
                                .value.a)), /* extKeyID = ~context->sessionId*/
                    op->params[2].memref.buffer, /*  (void *)saltData*/
                    op->params[2].memref.size,   /*  saltLen    */
                    op->params[3].memref.buffer, /*  info  */
                    op->params[3].memref.size,   /*   infoLen    */
                    op->params[4].memref.buffer, /* derivekey*/
                    op->params[4].memref.size);  /* derivekeykeylen;*/
            }
        }
        break;
    case kSSCP_GEN_RANDOM_NUM: {
        resSW = A71_GetRandom(
            op->params[2].memref.buffer, (uint8_t)op->params[2].memref.size);
    } break;
    default:
        retSSCP = kStatus_SSCP_Fail;
        LOG_E("Not a SSCP command");
    }
    *returnOrigin = (uint32_t)swToSSSResult(resSW);
    return (retSSCP);
}

/* ************************************************************************** */
/* Private Functions                                                          */
/* ************************************************************************** */
static HLSE_RET_CODE a71ch_CalHmacSha256(sss_a71ch_key_store_t *keyStore,
    uint32_t extKeyID,
    const uint8_t *info,
    size_t infoLen,
    uint8_t *pDerivekey,
    size_t derivekeykeylen)
{
    HLSE_RET_CODE hlseret;
    uint16_t intId = 0;
    uint8_t nBlock = 1;
    U16 Hmaclen = 64;

    sss_status_t status = ks_common_extId_to_int_index(
        keyStore->keystore_shadow, extKeyID, &intId);
    if (status != kStatus_SSS_Success) {
        LOG_E("Could not find Ext ID 0x%X in keytore_shadow", extKeyID);
        hlseret = HLSE_ERR_API_ERROR;
    }
    else {
        hlseret = A71_GetHmacSha256(
            (U8)intId, nBlock, info, (U16)infoLen, pDerivekey, &Hmaclen);
    }
    return hlseret;
}

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

    sss_status_t status = ks_common_extId_to_int_index(
        keyStore->keystore_shadow, extKeyID, &intId);
    if (status != kStatus_SSS_Success) {
        LOG_E("Could not find Ext ID 0x%X in keytore_shadow", extKeyID);
        hlseret = HLSE_ERR_API_ERROR;
    }
    else {
        hlseret = A71_HkdfSymKey((U8)intId,
            nBlock,
            saltData,
            (uint16_t)saltLen,
            info,
            (uint16_t)infoLen,
            pDerivekey,
            (uint16_t)derivekeykeylen);
    }

    return hlseret;
}

static HLSE_RET_CODE a71ch_DhDeriveKey(sss_a71ch_key_store_t *keyStore,
    uint32_t extKeyID,
    uint8_t *pubKeyBuf,
    size_t pubKeyBufBitLen,
    uint8_t *pDerivekey,
    size_t *pDerivekeykeyBitlen)
{
    HLSE_RET_CODE hlseret;
    uint16_t intId = 0;
    uint16_t derivekeyBytelen;
    uint16_t pubKeyBufByteLen;
    sss_status_t status = ks_common_extId_to_int_index(
        keyStore->keystore_shadow, extKeyID, &intId);
    if (status != kStatus_SSS_Success) {
        LOG_E("Could not find Ext ID 0x%X in keytore_shadow", extKeyID);
        hlseret = HLSE_ERR_API_ERROR;
    }
    else {
        pubKeyBufByteLen = (uint16_t)CONVERT_BYTE(pubKeyBufBitLen);
        pubKeyBufByteLen =
            (uint16_t)REMOVE_DER_ECC_NISTP256_HEADER(pubKeyBufByteLen);
        derivekeyBytelen = (uint16_t)CONVERT_BYTE(*pDerivekeykeyBitlen);

        pubKeyBuf = ADD_DER_ECC_NISTP256_HEADER(pubKeyBuf);
        hlseret = A71_EcdhGetSharedSecret((U8)intId,
            pubKeyBuf,
            pubKeyBufByteLen,
            pDerivekey,
            &derivekeyBytelen);
        *pDerivekeykeyBitlen = CONVERT_BIT(derivekeyBytelen);
    }

    return hlseret;
}

static HLSE_RET_CODE a71ch_GenerateKey(keyStoreTable_t *keystore_shadow,
    uint8_t keyType,
    uint8_t cipherType,
    uint32_t extKeyID)
{
    HLSE_RET_CODE hlseret = HLSE_ERR_API_ERROR;
    if (keyType == kSSS_KeyPart_Pair &&
        cipherType == kSSS_CipherType_EC_NIST_P) {
        uint16_t intId = 0;
        sss_status_t status =
            ks_common_extId_to_int_index(keystore_shadow, extKeyID, &intId);
        if (status == kStatus_SSS_Success) {
            hlseret = A71_GenerateEccKeyPair((U8)intId);
        }
        else {
            LOG_E("Could not find Ext ID 0x%X in keytore_shadow", extKeyID);
            hlseret = HLSE_ERR_API_ERROR;
        }
    }
    return hlseret;
}

static HLSE_RET_CODE a71ch_EraseKey(keyStoreTable_t *keystore_shadow,
    uint8_t keyType,
    uint8_t cipherType,
    uint32_t extKeyID)
{
    HLSE_RET_CODE hlseret = HLSE_ERR_API_ERROR;
    uint16_t intId = 0;

    sss_status_t status =
        ks_common_extId_to_int_index(keystore_shadow, extKeyID, &intId);
    if (status != kStatus_SSS_Success) {
        LOG_E("Could not find Ext ID 0x%X in keytore_shadow", extKeyID);
        hlseret = HLSE_ERR_API_ERROR;
    }
    else {
        if (keyType == kSSS_KeyPart_Pair &&
            cipherType == kSSS_CipherType_EC_NIST_P) {
            hlseret = A71_EraseEccKeyPair((U8)intId);
        }
        else if (keyType == kSSS_KeyPart_Public &&
                 cipherType == kSSS_CipherType_EC_NIST_P) {
            hlseret = A71_EraseEccPublicKey((U8)intId);
        }
        else if (keyType == kSSS_KeyPart_Default &&
                 cipherType == kSSS_CipherType_Binary) {
            HLSE_OBJECT_HANDLE Handles[5];
            U16 HandlesNum = sizeof(Handles) / sizeof(HLSE_OBJECT_HANDLE);
            U16 HandlesNum_copy;
            U8 i = 0;

            hlseret =
                HLSE_EnumerateObjects(HLSE_CERTIFICATE, Handles, &HandlesNum);

            if (hlseret == HLSE_SW_OK) {
                HandlesNum_copy = HandlesNum;
                while (HandlesNum_copy) {
                    if (HLSE_GET_OBJECT_INDEX(Handles[i]) == intId) {
                        break;
                    }
                    i++;
                    HandlesNum_copy--;
                }
                hlseret = HLSE_EraseObject(Handles[i]);
            }
        }
    }
    if (hlseret == HLSE_SW_OK) {
        status = ks_common_remove_fat(keystore_shadow, extKeyID);
        if (status != kStatus_SSS_Success) {
            hlseret = HLSE_ERR_API_ERROR;
        }
    }
    return hlseret;
}

static HLSE_RET_CODE a71ch_FreezeKey(keyStoreTable_t *keystore_shadow,
    uint8_t keyType,
    uint8_t cipherType,
    uint32_t extKeyID)
{
    HLSE_RET_CODE hlseret = HLSE_ERR_API_ERROR;
    uint16_t intId = 0;
    sss_status_t status =
        ks_common_extId_to_int_index(keystore_shadow, extKeyID, &intId);
    if (status != kStatus_SSS_Success) {
        LOG_E("Could not find Ext ID 0x%X in keytore_shadow", extKeyID);
        hlseret = HLSE_ERR_API_ERROR;
    }
    else {
        if (keyType == kSSS_KeyPart_Pair &&
            cipherType == kSSS_CipherType_EC_NIST_P) {
            hlseret = A71_FreezeEccKeyPair((U8)intId);
        }
        if (keyType == kSSS_KeyPart_Public &&
            cipherType == kSSS_CipherType_EC_NIST_P) {
            hlseret = A71_FreezeEccPublicKey((U8)intId);
        }
    }

    return hlseret;
}

static sss_status_t swToSSSResult(uint16_t checkSW)
{
    switch (checkSW) {
    case SMCOM_OK:
        return kStatus_SSS_Success;
    default:
        return kStatus_SSS_Fail;
    }
}

static HLSE_RET_CODE a71ch_AllocateKeyStore(
    sss_a71ch_key_store_t *keyStore, uint32_t keyStoreID)
{
    HLSE_RET_CODE hlseret = HLSE_ERR_API_ERROR;
    HLSE_OBJECT_HANDLE Handles[5];
    U16 HandlesNum = sizeof(Handles) / sizeof(HLSE_OBJECT_HANDLE);
    U16 HandlesNum_copy;
    U8 i = 0;
    HLSE_OBJECT_INDEX index = 0;

    /* Search for our data table @ index */
    if (keyStore->shadow_handle == 0) {
        hlseret = HLSE_EnumerateObjects(HLSE_DATA, Handles, &HandlesNum);
        if (hlseret == HLSE_SW_OK) {
            HandlesNum_copy = HandlesNum;
            while (HandlesNum_copy) {
                if (HLSE_GET_OBJECT_INDEX(Handles[i]) == index) {
                    keyStore->shadow_handle = Handles[i];
                    ks_common_init_fat(&gkeystore_shadow_ch,
                        gLookupEntires_ch,
                        ARRAY_SIZE(gLookupEntires_ch));
                    break;
                }
                i++;
                HandlesNum_copy--;
            }
        }
    }

    if (gkeystore_shadow_ch.magic == 0) {
        ks_common_init_fat(&gkeystore_shadow_ch,
            gLookupEntires_ch,
            ARRAY_SIZE(gLookupEntires_ch));
    }
    keyStore->keystore_shadow = &gkeystore_shadow_ch;

    /* If it was never there, create it @ index */
    if (keyStore->shadow_handle == 0) {
        /* Could not find it yet*/
        HLSE_OBJECT_TYPE objType = HLSE_DATA;
        U16 templateSize = 3;
        HLSE_ATTRIBUTE attr[3];
        keyStoreTableEEPROM_t eeKeyStore;

        eeKeyStore.magic = gkeystore_shadow_ch.magic;
        eeKeyStore.version = gkeystore_shadow_ch.version;
        eeKeyStore.maxEntries = gkeystore_shadow_ch.maxEntries;
        memcpy(eeKeyStore.entries,
            gkeystore_shadow_ch.entries,
            sizeof(eeKeyStore.entries));

        attr[0].type = HLSE_ATTR_OBJECT_TYPE;
        attr[0].value = &objType;
        attr[0].valueLen = sizeof(objType);
        attr[1].type = HLSE_ATTR_OBJECT_INDEX;
        attr[1].value = &index;
        attr[1].valueLen = sizeof(index);
        attr[2].type = HLSE_ATTR_OBJECT_VALUE;
        attr[2].value = &eeKeyStore;
        attr[2].valueLen = sizeof(eeKeyStore);

        hlseret =
            HLSE_CreateObject(attr, templateSize, &keyStore->shadow_handle);
    }

    /* Either we created it. Or it was already existing, read it. */
    if (keyStore->shadow_handle != 0) {
        hlseret = a71ch_loadKeyStore(keyStore);
    }
    return hlseret;
}

static HLSE_RET_CODE a71ch_loadKeyStore(sss_a71ch_key_store_t *keyStore)
{
    HLSE_RET_CODE hlseret;
    HLSE_ATTRIBUTE attr;
    keyStoreTableEEPROM_t eeKeyStore = {0};

    if (keyStore->shadow_handle == 0) {
        /* With shadow handle == 0, no point of reading. */
        hlseret = HLSE_ERR_MEMORY;
        goto cleanup;
    }

    attr.type = HLSE_ATTR_OBJECT_VALUE;
    attr.value = &eeKeyStore;
    attr.valueLen = sizeof(eeKeyStore);
    /* Read from Key Store and load it here in gkeystore_shadow_ch */
    hlseret = HLSE_GetObjectAttribute(keyStore->shadow_handle, &attr);

    if (hlseret != HLSE_SW_OK)
        goto cleanup;
    if (eeKeyStore.magic == gkeystore_shadow_ch.magic &&
        eeKeyStore.maxEntries == gkeystore_shadow_ch.maxEntries) {
        keyStore->keystore_shadow = &gkeystore_shadow_ch;
        if (eeKeyStore.version > gkeystore_shadow_ch.version) {
            LOG_E("Keystore version mismatch");
            hlseret = HLSE_ERR_NOT_SUPPORTED;
            goto cleanup;
        }
        memcpy(gkeystore_shadow_ch.entries,
            eeKeyStore.entries,
            sizeof(eeKeyStore.entries));
        keyStore->keystore_shadow->version = eeKeyStore.version;
    }
    else {
        LOG_E(" Mismatch in eeKeyStore == gkeystore_shadow_ch");
        /* This is the case when there is mismatch between KeyStore and Middleware */
        hlseret = HLSE_ERR_MEMORY;
    }

cleanup:
    return hlseret;
}
static HLSE_RET_CODE a71ch_saveKeyStore(sss_a71ch_key_store_t *keyStore)
{
    HLSE_RET_CODE hlseret;
    if (keyStore->shadow_handle == 0) {
        hlseret = HLSE_ERR_API_ERROR;
    }
    else {
        keyStoreTableEEPROM_t eeKeyStore;
        if (KEYSTORE_VERSION > gkeystore_shadow_ch.version) {
            gkeystore_shadow_ch.version = KEYSTORE_VERSION;
        }

        eeKeyStore.magic = gkeystore_shadow_ch.magic;
        eeKeyStore.version = gkeystore_shadow_ch.version;
        eeKeyStore.maxEntries = gkeystore_shadow_ch.maxEntries;
        memcpy(eeKeyStore.entries,
            gkeystore_shadow_ch.entries,
            sizeof(eeKeyStore.entries));

        HLSE_ATTRIBUTE attr;
        attr.type = HLSE_ATTR_OBJECT_VALUE;
        attr.value = &eeKeyStore;
        attr.valueLen = sizeof(eeKeyStore);
        /* write gkeystore_shadow_ch */
        hlseret = HLSE_SetObjectAttribute(keyStore->shadow_handle, &attr);
    }
    return hlseret;
}

static HLSE_RET_CODE a71ch_KeyObjectGetHandle(sss_a71ch_key_store_t *keyStore,
    uint32_t extKeyID,
    uint32_t *keyType,
    uint32_t *cipherType)
{
    HLSE_RET_CODE hlseret = HLSE_ERR_GENERAL_ERROR;
    sss_status_t ks_status = kStatus_SSS_Success;
    *keyType = kSSS_KeyPart_NONE;
    ks_status = ks_common_get_keyType_from_keyid(
        keyStore->keystore_shadow, extKeyID, keyType, cipherType);
    if (ks_status == kStatus_SSS_Success)
        hlseret = HLSE_SW_OK;
    return hlseret;
}

static HLSE_RET_CODE a71ch_AllocateKeyObject(sss_a71ch_key_store_t *keyStore,
    uint32_t extKeyID,
    sss_key_part_t key_part,
    sss_cipher_type_t cipherType,
    size_t keyByteLenMax,
    uint32_t options)
{
    HLSE_RET_CODE hlseret = HLSE_SW_OK;
    uint16_t intIndex = 0;
    sss_status_t ks_status;

    ks_status = ks_common_check_available_int_index(
        keyStore->keystore_shadow, (uint8_t)key_part, cipherType, &intIndex);

    if (ks_status != kStatus_SSS_Success) {
        hlseret = HLSE_ERR_MEMORY;
    }

    if (hlseret == HLSE_SW_OK) {
        ks_status = ks_common_update_fat(keyStore->keystore_shadow,
            extKeyID,
            (uint8_t)key_part,
            (uint8_t)cipherType,
            (uint8_t)intIndex,
            0);
    }
    if (ks_status != kStatus_SSS_Success) {
        hlseret = HLSE_ERR_MEMORY;
    }
    if (hlseret == HLSE_SW_OK) {
        switch (cipherType) {
        case kSSS_CipherType_Binary: {
            HLSE_OBJECT_INDEX index = intIndex;
            HLSE_OBJECT_TYPE objType = HLSE_CERTIFICATE;
            U16 templateSize = 3;
            HLSE_ATTRIBUTE attr[3];
            HLSE_OBJECT_HANDLE handle = 0;
            void *pMem;
            pMem = malloc(keyByteLenMax);
            if (NULL != pMem) {
                attr[0].type = HLSE_ATTR_OBJECT_TYPE;
                attr[0].value = &objType;
                attr[0].valueLen = sizeof(objType);
                attr[1].type = HLSE_ATTR_OBJECT_INDEX;
                attr[1].value = &index;
                attr[1].valueLen = sizeof(index);
                attr[2].type = HLSE_ATTR_OBJECT_VALUE;
                /* This is dummy value we write... Actually this source code */
                attr[2].value = pMem;
                attr[2].valueLen = (U16)keyByteLenMax;

                memset(pMem, 0, keyByteLenMax);
                hlseret = HLSE_CreateObject(attr, templateSize, &handle);
                /* TODO: if object is already present and all attributes are same then mark it as success*/
                if (hlseret == HLSE_OBJ_ALREADY_EXISTS)
                    hlseret = HLSE_SW_OK;
            }
            if (NULL != pMem) {
                free(pMem);
            }
            break;
        }
        case kSSS_CipherType_EC_NIST_P:
        case kSSS_CipherType_AES:
        case kSSS_CipherType_UserID:
            break;
        default:
            hlseret = HLSE_ERR_API_ERROR;
        }
    }
    if (hlseret == HLSE_SW_OK) {
        /* Persist to EEPROM */
        hlseret = a71ch_saveKeyStore(keyStore);
    }
    else {
        /* Reset the structure based on EEPROM */
        a71ch_loadKeyStore(keyStore);
    }
    return hlseret;
}

static HLSE_RET_CODE a71ch_getKey(keyStoreTable_t *keystore_shadow,
    sss_key_part_t key_part,
    sss_cipher_type_t cipher_type,
    uint32_t extId,
    uint8_t *key,
    uint32_t *keyByteLen,
    uint32_t *keyBitLen)
{
    HLSE_RET_CODE resSW = HLSE_ERR_API_ERROR;
    uint16_t intId = 0;
    U8 i = 0;
    HLSE_OBJECT_HANDLE Handles[5];
    HLSE_OBJECT_HANDLE GetHandle = 0;
    U16 HandlesNum = sizeof(Handles) / sizeof(HLSE_OBJECT_HANDLE);
    U16 HandlesNum_copy;
    HLSE_ATTRIBUTE attr;
    U16 keyLen = *keyByteLen;
    /* clang-format off */
    const uint8_t der_header[] = { 0x30, 0x59, 0x30, 0x13,0x06, 0x07, 0x2A, 0x86,
                                   0x48, 0xCE, 0x3D, 0x02,0x01, 0x06, 0x08, 0x2A,
                                   0x86, 0x48, 0xCE, 0x3D,0x03, 0x01, 0x07, 0x03,
                                   0x42, 0x00
    };
    /* clang-format on */
    sss_status_t status =
        ks_common_extId_to_int_index(keystore_shadow, extId, &intId);
    if (status == kStatus_SSS_Success) {
        if (key_part == kSSS_KeyPart_Pair &&
            cipher_type == kSSS_CipherType_EC_NIST_P) {
            memcpy(key, der_header, DER_ECC_NISTP256_HEADER);
            resSW = A71_GetPublicKeyEccKeyPair(
                (U8)intId, ADD_DER_ECC_NISTP256_HEADER(key), &keyLen);
            /* Return the Key length including the ECC DER Header */
            keyLen = ADD_DER_ECC_NISTP256_HEADER(keyLen);
            /* Return the Key length in bits */
            *keyByteLen = keyLen;
            *keyBitLen = 256;
        }
        else if (key_part == kSSS_KeyPart_Public &&
                 cipher_type == kSSS_CipherType_EC_NIST_P) {
            memcpy(key, der_header, DER_ECC_NISTP256_HEADER);
            resSW = A71_GetEccPublicKey(
                (U8)intId, ADD_DER_ECC_NISTP256_HEADER(key), &keyLen);
            /* Return the Key length including the ECC DER Header */
            keyLen = ADD_DER_ECC_NISTP256_HEADER(keyLen);
            /* Return the Key length in bits */
            *keyByteLen = keyLen;
            *keyBitLen = 256;
        }
        else if (key_part == kSSS_KeyPart_Default &&
                 cipher_type == kSSS_CipherType_Binary) {
            resSW =
                HLSE_EnumerateObjects(HLSE_CERTIFICATE, Handles, &HandlesNum);
            if (resSW == HLSE_SW_OK) {
                HandlesNum_copy = HandlesNum;
                while (HandlesNum_copy) {
                    if (HLSE_GET_OBJECT_INDEX(Handles[i]) == intId) {
                        GetHandle = Handles[i];
                        break;
                    }
                    i++;
                    HandlesNum_copy--;
                }
            }
            if (GetHandle != 0) {
                attr.type = HLSE_ATTR_OBJECT_VALUE;
                attr.value = key;
                attr.valueLen = keyLen;
                resSW = HLSE_GetObjectAttribute(GetHandle, &attr);
                *keyByteLen = attr.valueLen;
                *keyBitLen = (attr.valueLen * 8);
            }
        }
    }
    else {
        LOG_E("Could not find Ext ID 0x%X in keystore_shadow", extId);
    }
    return resSW;
}

static HLSE_RET_CODE a71ch_setKey(keyStoreTable_t *keystore_shadow,
    sss_key_part_t key_part,
    sss_cipher_type_t cipher_type,
    uint32_t extId,
    uint8_t *key,
    size_t keyLen)
{
    HLSE_RET_CODE resSW = HLSE_ERR_API_ERROR;
    uint16_t intId = 0;
    U8 i = 0;
    HLSE_OBJECT_HANDLE Handles[5];
    HLSE_OBJECT_HANDLE GetHandle = 0;
    U16 HandlesNum = sizeof(Handles) / sizeof(HLSE_OBJECT_HANDLE);
    U16 HandlesNum_copy;
    HLSE_ATTRIBUTE attr;
    sss_status_t status =
        ks_common_extId_to_int_index(keystore_shadow, extId, &intId);
    if (status == kStatus_SSS_Success) {
        if (key_part == kSSS_KeyPart_Public &&
            cipher_type == kSSS_CipherType_EC_NIST_P && keyLen == (91)) {
            key = ADD_DER_ECC_NISTP256_HEADER(key);
            keyLen = REMOVE_DER_ECC_NISTP256_HEADER(keyLen);
            resSW = A71_SetEccPublicKey((U8)intId, key, (U16)keyLen);
        }
        //FIXIT: Check how this 65 length public key can be handled
        else if (key_part == kSSS_KeyPart_Public &&
                 cipher_type == kSSS_CipherType_EC_NIST_P && keyLen == (65)) {
            resSW = A71_SetEccPublicKey((U8)intId, key, (U16)keyLen);
        }
        else if (key_part == kSSS_KeyPart_Default &&
                 cipher_type == kSSS_CipherType_AES) {
            resSW = A71_SetSymKey((U8)intId, key, (U16)keyLen);
        }
        else if ((key_part == kSSS_KeyPart_Pair &&
                     cipher_type == kSSS_CipherType_EC_NIST_P) &&
                 keyLen == (138)) {
            uint8_t *pPrivateKey = &key[36];
            uint16_t privateKeyLen = 32;
            uint8_t *pPublicKey = &key[73];
            uint16_t publicKeyLen = 65;

            resSW = A71_SetEccKeyPair((U8)intId,
                pPublicKey,
                (U16)publicKeyLen,
                pPrivateKey,
                (U16)privateKeyLen);
        }
        else if (key_part == kSSS_KeyPart_Default &&
                 cipher_type == kSSS_CipherType_Binary) {
            resSW =
                HLSE_EnumerateObjects(HLSE_CERTIFICATE, Handles, &HandlesNum);
            if (resSW == HLSE_SW_OK) {
                HandlesNum_copy = HandlesNum;
                while (HandlesNum_copy) {
                    if (HLSE_GET_OBJECT_INDEX(Handles[i]) == intId) {
                        GetHandle = Handles[i];
                        break;
                    }
                    i++;
                    HandlesNum_copy--;
                }
            }
            if (GetHandle != 0) {
                attr.type = HLSE_ATTR_OBJECT_VALUE;
                attr.value = key;
                attr.valueLen = (U16)keyLen;
                resSW = HLSE_SetObjectAttribute(GetHandle, &attr);
            }
        }
    }
    else {
        LOG_E("Could not find Ext ID 0x%X in keystore_shadow", extId);
    }
    return resSW;
}

static HLSE_RET_CODE a71_eccSign(keyStoreTable_t *keystore_shadow,
    uint32_t extKeyID,
    uint8_t *pHash,
    uint16_t hashLen,
    uint8_t *pSignature,
    uint16_t *pSignatureLen)
{
    uint16_t intId = 0;
    HLSE_RET_CODE resSW = HLSE_ERR_API_ERROR;
    sss_status_t status =
        ks_common_extId_to_int_index(keystore_shadow, extKeyID, &intId);
    /* A71CH Max hashlen is AX_SHA256_LEN*/
    if (hashLen > AX_SHA256_LEN)
        hashLen = AX_SHA256_LEN;
    if (status == kStatus_SSS_Success) {
        resSW = A71_EccSign(
            (SST_Index_t)intId, pHash, hashLen, pSignature, pSignatureLen);
    }
    return resSW;
}

static HLSE_RET_CODE a71_eccVerify(keyStoreTable_t *keystore_shadow,
    uint32_t extKeyID,
    uint8_t *pHash,
    uint16_t hashLen,
    uint8_t *pSignature,
    uint16_t SignatureLen)
{
    uint16_t intId = 0;
    uint8_t result = 0;
    HLSE_RET_CODE resSW = HLSE_ERR_API_ERROR;
    sss_status_t status =
        ks_common_extId_to_int_index(keystore_shadow, extKeyID, &intId);
    if (status == kStatus_SSS_Success) {
        resSW = A71_EccVerify((SST_Index_t)intId,
            pHash,
            hashLen,
            pSignature,
            SignatureLen,
            &result);
    }
    if (resSW == SW_OK)
        if (result == 0)
            resSW = ERR_GENERAL_ERROR;

    return resSW;
}

#endif /* SSS_HAVE_A71CH */
