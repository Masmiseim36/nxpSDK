/*
* Copyright 2019 NXP
*
* This software is owned or controlled by NXP and may only be used
* strictly in accordance with the applicable license terms.  By expressly
* accepting such terms or by downloading, installing, activating and/or
* otherwise using the software, you are agreeing that you have read, and
* that you agree to comply with and are bound by, such license terms.  If
* you do not agree to be bound by the applicable license terms, then you
* may not retain, install, activate or otherwise use the software.
*/

/** @file
*
* ex_sss_se050x_auth.c:  *The purpose and scope of this file*
*
* Project:  sss-doc-upstream
*
* $Date: Mar 10, 2019 $
* $Author: ing05193 $
* $Revision$
*/

/* *****************************************************************************************************************
* Includes
* ***************************************************************************************************************** */
#include <string.h>

#include "ex_sss_auth.h"
#include "ex_sss_boot_int.h"
#include "nxLog_App.h"
#include "nxScp03_Types.h"

/* *****************************************************************************************************************
* Internal Definitions
* ***************************************************************************************************************** */

/* *****************************************************************************************************************
* Type Definitions
* ***************************************************************************************************************** */

/* *****************************************************************************************************************
* Global and Static Variables
* Total Size: NNNbytes
* ***************************************************************************************************************** */

/* *****************************************************************************************************************
* Private Functions Prototypes
* ***************************************************************************************************************** */

#if SSSFTR_SE05X_AuthSession
static sss_status_t ex_sss_se05x_prepare_host_userid(
    sss_object_t *pObj, sss_key_store_t *pKs);

/* Prepare host for multiple user userId based sessions */
/* Each User can store different userIds for authentication in HOST*/
/* Each User should have a unique Id*/

static sss_status_t ex_sss_se05x_prepare_host_userid_Id(
    sss_object_t *pObj, sss_key_store_t *pKs, uint32_t Id);
#endif

#if SSS_HAVE_SCP_SCP03_SSS
static sss_status_t ex_sss_se05x_prepare_host_platformscp(
    NXSCP03_AuthCtx_t *pCtx,
    ex_SE05x_authCtx_t *pauthctx,
    sss_key_store_t *pKs);

#if SSSFTR_SE05X_AuthFastSCP
static sss_status_t ex_sss_se05x_prepare_host_fastscp(
    SE05x_AuthCtx_FastScp_t *pCtx,
    ex_SE05x_authCtx_t *pauthctx,
    sss_key_store_t *pKs);

static sss_status_t Alloc_FastScpkey_toSE05xAuthctx(sss_object_t *keyObject,
    sss_key_store_t *pKs,
    uint32_t keyId,
    sss_key_part_t keypart);

#endif // SSSFTR_SE05X_AuthFastSCP
#if SSSFTR_SE05X_AuthSession
static sss_status_t ex_sss_se05x_prepare_host_AppletScp03Keys(
    NXSCP03_AuthCtx_t *pAuthCtx,
    ex_SE05x_authCtx_t *pauthctx,
    sss_key_store_t *host_k);
#endif
static sss_status_t Alloc_Scp03key_toSE05xAuthctx(
    sss_object_t *keyObject, sss_key_store_t *pKs, uint32_t keyId);

#if SSSFTR_SE05X_AuthSession
static sss_status_t Alloc_AppletScp03key_toSE05xAuthctx(
    sss_object_t *keyObject, uint32_t keyId, sss_key_store_t *host_ks);
#endif // SSSFTR_SE05X_AuthSession

#endif

/* *****************************************************************************************************************
* Public Functions
* ***************************************************************************************************************** */

sss_status_t ex_sss_se05x_prepare_host(sss_session_t *host_session,
    sss_key_store_t *host_ks,
    SE_Connect_Ctx_t *se05x_open_ctx,
    ex_SE05x_authCtx_t *se05x_auth_ctx,
    SE_AuthType_t auth_type)
{
    sss_status_t status = kStatus_SSS_Fail;

    if (host_session->subsystem == kType_SSS_SubSystem_NONE) {
        sss_type_t hostsubsystem = kType_SSS_SubSystem_NONE;

#if SSS_HAVE_MBEDTLS
        hostsubsystem = kType_SSS_mbedTLS;
#endif

#if SSS_HAVE_OPENSSL
        hostsubsystem = kType_SSS_OpenSSL;
#endif
        status = sss_host_session_open(
            host_session, hostsubsystem, 0, kSSS_ConnectionType_Plain, NULL);
        if (kStatus_SSS_Success != status) {
            LOG_E("Failed to open Host Session");
            goto cleanup;
        }
        status = sss_host_key_store_context_init(host_ks, host_session);
        if (kStatus_SSS_Success != status) {
            LOG_E("Host: sss_key_store_context_init failed");
            goto cleanup;
        }
        status = sss_host_key_store_allocate(host_ks, __LINE__);
        if (kStatus_SSS_Success != status) {
            LOG_E("Host: sss_key_store_allocate failed");
            goto cleanup;
        }
    }
    switch (auth_type) {
    case kSSS_AuthType_ID:
        se05x_open_ctx->auth.ctx.idobj.pObj = &se05x_auth_ctx->id.ex_id;
#if SSSFTR_SE05X_AuthSession
        status = ex_sss_se05x_prepare_host_userid(
            se05x_open_ctx->auth.ctx.idobj.pObj, host_ks);
#endif
        break;
#if SSS_HAVE_SCP_SCP03_SSS
    case kSSS_AuthType_SCP03:
        status = ex_sss_se05x_prepare_host_platformscp(
            &se05x_open_ctx->auth.ctx.scp03, se05x_auth_ctx, host_ks);
        break;
#if SSSFTR_SE05X_AuthFastSCP
    case kSSS_AuthType_FastSCP:
        status = ex_sss_se05x_prepare_host_fastscp(
            &se05x_open_ctx->auth.ctx.fastscp, se05x_auth_ctx, host_ks);
        break;
#endif
    case kSSS_AuthType_AppletSCP03:
#if SSSFTR_SE05X_AuthSession
        status = ex_sss_se05x_prepare_host_AppletScp03Keys(
            &se05x_open_ctx->auth.ctx.scp03, se05x_auth_ctx, host_ks);
#endif
        break;
#endif
    case kSSS_AuthType_None:
        /* Nothing to do */
        status = kStatus_SSS_Success;
        break;
    default:
        status = kStatus_SSS_Fail;
        LOG_E("Not handled");
    }

    if (kStatus_SSS_Success != status) {
        LOG_E(
            "Host: ex_sss_se05x_prepare_host_<type=(SE_AuthType_t)%d> "
            "failed",
            auth_type);
        goto cleanup;
    }
    se05x_open_ctx->auth.authType = auth_type;

cleanup:
    return status;
}

/* Use this host crypto set up multiple sessions */
sss_status_t ex_sss_se05x_prepare_host_keys(sss_session_t *pHostSession,
    sss_key_store_t *pHostKs,
    SE_Connect_Ctx_t *pConnectCtx,
    ex_SE05x_authCtx_t *se05x_auth_ctx,
    uint32_t Id)
{
    sss_status_t status = kStatus_SSS_Fail;
    sss_type_t hostsubsystem = kType_SSS_SubSystem_NONE;

#if SSS_HAVE_MBEDTLS
    hostsubsystem = kType_SSS_mbedTLS;
#endif

#if SSS_HAVE_OPENSSL
    hostsubsystem = kType_SSS_OpenSSL;
#endif
    status = sss_host_session_open(
        pHostSession, hostsubsystem, 0, kSSS_ConnectionType_Plain, NULL);
    if (kStatus_SSS_Success != status) {
        LOG_E("Failed to open Host Session");
        goto cleanup;
    }
    status = sss_host_key_store_context_init(pHostKs, pHostSession);
    if (kStatus_SSS_Success != status) {
        LOG_E("Host: sss_key_store_context_init failed");
        goto cleanup;
    }
    status = sss_host_key_store_allocate(pHostKs, __LINE__);
    if (kStatus_SSS_Success != status) {
        LOG_E("Host: sss_key_store_allocate failed");
        goto cleanup;
    }

    switch (pConnectCtx->auth.authType) {
    case kSSS_AuthType_ID:
        pConnectCtx->auth.ctx.idobj.pObj = &se05x_auth_ctx->id.ex_id;
#if SSSFTR_SE05X_AuthSession
        status = ex_sss_se05x_prepare_host_userid_Id(
            pConnectCtx->auth.ctx.idobj.pObj, pHostKs, Id);
#endif
        break;
#if SSS_HAVE_SCP_SCP03_SSS
    case kSSS_AuthType_SCP03:
        status = ex_sss_se05x_prepare_host_platformscp(
            &pConnectCtx->auth.ctx.scp03, se05x_auth_ctx, pHostKs);
        break;
#if SSSFTR_SE05X_AuthFastSCP
    case kSSS_AuthType_FastSCP:
        status = ex_sss_se05x_prepare_host_fastscp(
            &pConnectCtx->auth.ctx.fastscp, se05x_auth_ctx, pHostKs);
        break;
#endif
    case kSSS_AuthType_AppletSCP03:
#if SSSFTR_SE05X_AuthSession
        status = ex_sss_se05x_prepare_host_AppletScp03Keys(
            &pConnectCtx->auth.ctx.scp03, se05x_auth_ctx, pHostKs);
#endif
        break;
#endif
    case kSSS_AuthType_None:
        /* Nothing to do */
        status = kStatus_SSS_Success;
        break;
    default:
        status = kStatus_SSS_Fail;
        LOG_E("Not handled");
    }

    if (kStatus_SSS_Success != status) {
        goto cleanup;
    }

cleanup:
    return status;
}
/* *****************************************************************************************************************
* Private Functions
* ***************************************************************************************************************** */
#if SSSFTR_SE05X_AuthSession
static sss_status_t ex_sss_se05x_prepare_host_userid(
    sss_object_t *pObj, sss_key_store_t *pKs)
{
    sss_status_t status = kStatus_SSS_Fail;
    uint32_t keyId = __LINE__;
    uint8_t data[32] = {
        0,
    };
    size_t dataLen = sizeof(data);
    size_t keyBitLen = sizeof(data) * 8;

    const uint8_t se050Authkey[] = EX_SSS_AUTH_SE05X_UserID_VALUE;

    if (pObj->keyId != keyId) {
        status = sss_host_key_object_init(pObj, pKs);
        if (status != kStatus_SSS_Success) {
            LOG_E("UserID: Key Object Init failed");
            goto cleanup;
        }

        status = sss_host_key_object_allocate_handle(pObj,
            keyId,
            kSSS_KeyPart_Default,
            kSSS_CipherType_UserID,
            sizeof(se050Authkey),
            kKeyObject_Mode_Persistent);

        if (status != kStatus_SSS_Success) {
            LOG_E("UserID: Allocate failed");
            goto cleanup;
        }

        status = sss_host_key_store_set_key(pObj->keyStore,
            pObj,
            se050Authkey,
            sizeof(se050Authkey),
            sizeof(se050Authkey) * 8,
            NULL,
            0);

        if (status != kStatus_SSS_Success) {
            LOG_E("UserID: Set value failed");
        }
    }
    else {
        status = sss_host_key_store_get_key(
            pObj->keyStore, pObj, data, &dataLen, &keyBitLen);
        if (status == kStatus_SSS_Success) {
            if (memcmp(data, se050Authkey, sizeof(se050Authkey)) != 0) {
                status = kStatus_SSS_Fail;
                LOG_E("UserID: Key Value is different");
                goto cleanup;
            }
        }
        else {
            LOG_E("UserID: Get value failed");
        }
    }

cleanup:
    return status;
}

/* Prepare host for multiple user userId based sessions */
static sss_status_t ex_sss_se05x_prepare_host_userid_Id(
    sss_object_t *pObj, sss_key_store_t *pKs, uint32_t Id)
{
    sss_status_t status = kStatus_SSS_Fail;
    status = sss_host_key_object_init(pObj, pKs);
    const uint8_t se050Authkey[] = EX_SSS_AUTH_SE05X_UserID_VALUE;

    status = sss_host_key_object_allocate_handle(pObj,
        __LINE__ + Id,
        kSSS_KeyPart_Default,
        kSSS_CipherType_UserID,
        sizeof(se050Authkey),
        kKeyObject_Mode_Persistent);
    if (status != kStatus_SSS_Success) {
        LOG_E("UserID: Allocate failed");
        goto cleanup;
    }

    status = sss_host_key_store_set_key(pObj->keyStore,
        pObj,
        se050Authkey,
        sizeof(se050Authkey),
        sizeof(se050Authkey) * 8,
        NULL,
        0);
    if (status != kStatus_SSS_Success) {
        LOG_E("UserID: Set value failed");
        goto cleanup;
    }

cleanup:
    return status;
}
#endif

#if SSS_HAVE_SCP_SCP03_SSS
/* Function to Set Init and Allocate static Scp03Keys and Init Allocate dynamic keys */
static sss_status_t ex_sss_se05x_prepare_host_platformscp(
    NXSCP03_AuthCtx_t *pAuthCtx,
    ex_SE05x_authCtx_t *pEx_auth,
    sss_key_store_t *pKs)
{
    sss_status_t status = kStatus_SSS_Fail;
    uint8_t KEY_ENC[] = EX_SSS_AUTH_SE05X_KEY_ENC;
    uint8_t KEY_MAC[] = EX_SSS_AUTH_SE05X_KEY_MAC;
    uint8_t KEY_DEK[] = EX_SSS_AUTH_SE05X_KEY_DEK;
    pAuthCtx->pStatic_ctx = &pEx_auth->scp03.ex_static;
    pAuthCtx->pDyn_ctx = &pEx_auth->scp03.ex_dyn;
    NXSCP03_StaticCtx_t *pStatic_ctx = pAuthCtx->pStatic_ctx;
    NXSCP03_DynCtx_t *pDyn_ctx = pAuthCtx->pDyn_ctx;

    pStatic_ctx->keyVerNo = EX_SSS_AUTH_SE05X_KEY_VERSION_NO;

    /* Init Allocate ENC Static Key */
    status = Alloc_Scp03key_toSE05xAuthctx(
        &pStatic_ctx->Enc, pKs, MAKE_TEST_ID(__LINE__));
    if (status != kStatus_SSS_Success) {
        return status;
    }
    /* Set ENC Static Key */
    status = sss_host_key_store_set_key(
        pKs, &pStatic_ctx->Enc, KEY_ENC, 16, 16 * 8, NULL, 0);
    if (status != kStatus_SSS_Success) {
        return status;
    }

    /* Init Allocate MAC Static Key */
    status = Alloc_Scp03key_toSE05xAuthctx(
        &pStatic_ctx->Mac, pKs, MAKE_TEST_ID(__LINE__));
    if (status != kStatus_SSS_Success) {
        return status;
    }
    /* Set MAC Static Key */
    status = sss_host_key_store_set_key(
        pKs, &pStatic_ctx->Mac, KEY_MAC, 16, 16 * 8, NULL, 0);
    if (status != kStatus_SSS_Success) {
        return status;
    }

    /* Init Allocate DEK Static Key */
    status = Alloc_Scp03key_toSE05xAuthctx(
        &pStatic_ctx->Dek, pKs, MAKE_TEST_ID(__LINE__));
    if (status != kStatus_SSS_Success) {
        return status;
    }
    /* Set DEK Static Key */
    status = sss_host_key_store_set_key(
        pKs, &pStatic_ctx->Dek, KEY_DEK, 16, 16 * 8, NULL, 0);
    if (status != kStatus_SSS_Success) {
        return status;
    }

    /* Init Allocate ENC Session Key */
    status = Alloc_Scp03key_toSE05xAuthctx(
        &pDyn_ctx->Enc, pKs, MAKE_TEST_ID(__LINE__));
    if (status != kStatus_SSS_Success) {
        return status;
    }
    /* Init Allocate MAC Session Key */
    status = Alloc_Scp03key_toSE05xAuthctx(
        &pDyn_ctx->Mac, pKs, MAKE_TEST_ID(__LINE__));
    if (status != kStatus_SSS_Success) {
        return status;
    }
    /* Init Allocate DEK Session Key */
    status = Alloc_Scp03key_toSE05xAuthctx(
        &pDyn_ctx->Rmac, pKs, MAKE_TEST_ID(__LINE__));
    return status;
}
#endif

#if SSS_HAVE_SCP_SCP03_SSS
static sss_status_t Alloc_Scp03key_toSE05xAuthctx(
    sss_object_t *keyObject, sss_key_store_t *pKs, uint32_t keyId)
{
    sss_status_t status = kStatus_SSS_Fail;
    status = sss_host_key_object_init(keyObject, pKs);
    if (status != kStatus_SSS_Success) {
        return status;
    }

    status = sss_host_key_object_allocate_handle(keyObject,
        keyId,
        kSSS_KeyPart_Default,
        kSSS_CipherType_AES,
        16,
        kKeyObject_Mode_Transient);
    return status;
}

#if SSSFTR_SE05X_AuthFastSCP
static sss_status_t Alloc_FastScpkey_toSE05xAuthctx(sss_object_t *keyObject,
    sss_key_store_t *pKs,
    uint32_t keyId,
    sss_key_part_t keypart)
{
    sss_status_t status = kStatus_SSS_Fail;
    status = sss_host_key_object_init(keyObject, pKs);
    if (status != kStatus_SSS_Success) {
        return status;
    }
    status = sss_host_key_object_allocate_handle(keyObject,
        keyId,
        keypart,
        kSSS_CipherType_EC_NIST_P,
        256,
        kKeyObject_Mode_Persistent);
    return status;
}

static sss_status_t ex_sss_se05x_prepare_host_fastscp(
    SE05x_AuthCtx_FastScp_t *pAuthCtx,
    ex_SE05x_authCtx_t *pEx_auth,
    sss_key_store_t *pKs)
{
    sss_status_t status = kStatus_SSS_Fail;
    uint8_t hostEcdsakey[] = EX_SSS_AUTH_SE05X_KEY_HOST_ECDSA_KEY;
    pAuthCtx->pStatic_ctx = &pEx_auth->fastscp.ex_static;
    pAuthCtx->pDyn_ctx = &pEx_auth->fastscp.ex_dyn;

    NXFastSCP03_StaticCtx_t *pStatic_ctx = pAuthCtx->pStatic_ctx;
    NXSCP03_DynCtx_t *pDyn_ctx = pAuthCtx->pDyn_ctx;

    /* Init allocate Host ECDSA Key pair */
    status = Alloc_FastScpkey_toSE05xAuthctx(&pStatic_ctx->HostEcdsaObj,
        pKs,
        MAKE_TEST_ID(__LINE__),
        kSSS_KeyPart_Pair);
    /* Set Host ECDSA Key pair */
    status = sss_host_key_store_set_key(pKs,
        &pStatic_ctx->HostEcdsaObj,
        hostEcdsakey,
        sizeof(hostEcdsakey),
        256,
        NULL,
        0);
    if (status == kStatus_SSS_Fail) {
        return status;
    }

    /* Init allocate Host ECKA Key pair */
    status = Alloc_FastScpkey_toSE05xAuthctx(&pStatic_ctx->HostEcKeypair,
        pKs,
        MAKE_TEST_ID(__LINE__),
        kSSS_KeyPart_Pair);
    /* Generate Host EC Key pair */
    status = sss_host_key_store_generate_key(
        pKs, &pStatic_ctx->HostEcKeypair, 256, NULL);
    if (status != kStatus_SSS_Success) {
        return status;
    }

    /* Init allocate SE ECKA Public Key */
    status = Alloc_FastScpkey_toSE05xAuthctx(&pStatic_ctx->SeEcPubKey,
        pKs,
        MAKE_TEST_ID(__LINE__),
        kSSS_KeyPart_Public);
    if (status != kStatus_SSS_Success) {
        return status;
    }

    /* Init Allocate Master Secret */
    status = Alloc_Scp03key_toSE05xAuthctx(
        &pStatic_ctx->masterSec, pKs, MAKE_TEST_ID(__LINE__));
    if (status != kStatus_SSS_Success) {
        return status;
    }

    /* Init Allocate ENC Session Key */
    status = Alloc_Scp03key_toSE05xAuthctx(
        &pDyn_ctx->Enc, pKs, MAKE_TEST_ID(__LINE__));
    if (status != kStatus_SSS_Success) {
        return status;
    }
    /* Init Allocate MAC Session Key */
    status = Alloc_Scp03key_toSE05xAuthctx(
        &pDyn_ctx->Mac, pKs, MAKE_TEST_ID(__LINE__));
    if (status != kStatus_SSS_Success) {
        return status;
    }
    /* Init Allocate DEK Session Key */
    status = Alloc_Scp03key_toSE05xAuthctx(
        &pDyn_ctx->Rmac, pKs, MAKE_TEST_ID(__LINE__));

    return status;
}
#endif // SSSFTR_SE05X_AuthFastSCP

#if SSSFTR_SE05X_AuthSession
/* Function to Set Init and Allocate static Scp03Keys and Init Allocate dynamic keys */
static sss_status_t ex_sss_se05x_prepare_host_AppletScp03Keys(
    NXSCP03_AuthCtx_t *pAuthCtx,
    ex_SE05x_authCtx_t *pEx_auth,
    sss_key_store_t *host_k)
{
    sss_status_t status = kStatus_SSS_Fail;
    uint8_t APPLET_KEY[] = EX_SSS_AUTH_SE05X_APPLETSCP_VALUE;
    pAuthCtx->pStatic_ctx = &pEx_auth->scp03.ex_static;
    pAuthCtx->pDyn_ctx = &pEx_auth->scp03.ex_dyn;
    NXSCP03_StaticCtx_t *pStatic_ctx = pAuthCtx->pStatic_ctx;
    NXSCP03_DynCtx_t *pDyn_ctx = pAuthCtx->pDyn_ctx;

    /* Init Allocate ENC Static Key */
    status = Alloc_AppletScp03key_toSE05xAuthctx(
        &pStatic_ctx->Enc, MAKE_TEST_ID(__LINE__), host_k);
    if (status != kStatus_SSS_Success) {
        return status;
    }
    /* Set ENC Static Key */
    status = sss_host_key_store_set_key(
        host_k, &pStatic_ctx->Enc, APPLET_KEY, 16, 16 * 8, NULL, 0);
    if (status != kStatus_SSS_Success) {
        return status;
    }

    /* Init Allocate MAC Static Key */
    status = Alloc_AppletScp03key_toSE05xAuthctx(
        &pStatic_ctx->Mac, MAKE_TEST_ID(__LINE__), host_k);
    if (status != kStatus_SSS_Success) {
        return status;
    }
    /* Set MAC Static Key */
    status = sss_host_key_store_set_key(
        host_k, &pStatic_ctx->Mac, APPLET_KEY, 16, 16 * 8, NULL, 0);
    if (status != kStatus_SSS_Success) {
        return status;
    }

    /* Init Allocate DEK Static Key */
    status = Alloc_AppletScp03key_toSE05xAuthctx(
        &pStatic_ctx->Dek, MAKE_TEST_ID(__LINE__), host_k);
    if (status != kStatus_SSS_Success) {
        return status;
    }
    /* Set DEK Static Key */
    status = sss_host_key_store_set_key(
        host_k, &pStatic_ctx->Dek, APPLET_KEY, 16, 16 * 8, NULL, 0);
    if (status != kStatus_SSS_Success) {
        return status;
    }

    /* Init Allocate ENC Session Key */
    status = Alloc_AppletScp03key_toSE05xAuthctx(
        &pDyn_ctx->Enc, MAKE_TEST_ID(__LINE__), host_k);
    if (status != kStatus_SSS_Success) {
        return status;
    }
    /* Init Allocate MAC Session Key */
    status = Alloc_AppletScp03key_toSE05xAuthctx(
        &pDyn_ctx->Mac, MAKE_TEST_ID(__LINE__), host_k);
    if (status != kStatus_SSS_Success) {
        return status;
    }
    /* Init Allocate DEK Session Key */
    status = Alloc_AppletScp03key_toSE05xAuthctx(
        &pDyn_ctx->Rmac, MAKE_TEST_ID(__LINE__), host_k);
    return status;
}

static sss_status_t Alloc_AppletScp03key_toSE05xAuthctx(
    sss_object_t *keyObject, uint32_t keyId, sss_key_store_t *host_ks)
{
    sss_status_t status = kStatus_SSS_Fail;
    status = sss_host_key_object_init(keyObject, host_ks);
    if (status != kStatus_SSS_Success) {
        return status;
    }

    status = sss_host_key_object_allocate_handle(keyObject,
        keyId,
        kSSS_KeyPart_Default,
        kSSS_CipherType_AES,
        16,
        kKeyObject_Mode_Persistent);
    return status;
}
#endif
#endif
