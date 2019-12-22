/*
* Copyright 2018,2019 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <fsl_sss_se05x_apis.h>
#include <nxLog_sss.h>

#if SSS_HAVE_SE05X
#include <fsl_sss_policy.h>
#include <fsl_sss_se05x_policy.h>
#include <fsl_sss_se05x_scp03.h>
#include <fsl_sss_util_asn1_der.h>
#include <fsl_sss_util_rsa_sign_utils.h>
#include <se05x_const.h>
#include <se05x_ecc_curves.h>
#include <sm_api.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nxEnsure.h"
#include "nxScp03_Apis.h"
#include "se05x_APDU.h"
#include "se05x_tlv.h"
#include "smCom.h"

#if (__GNUC__ && !AX_EMBEDDED)
#define LOCK_TXN(lock)                                           \
    LOG_D("Trying to Acquire Lock thread: %ld", pthread_self()); \
    pthread_mutex_lock(&lock);                                   \
    LOG_D("LOCK Acquired by thread: %ld", pthread_self());

#define UNLOCK_TXN(lock)                                             \
    LOG_D("Trying to Released Lock by thread: %ld", pthread_self()); \
    pthread_mutex_unlock(&lock);                                     \
    LOG_D("LOCK Released by thread: %ld", pthread_self());
#endif

static SE05x_ECSignatureAlgo_t se05x_get_ec_sign_hash_mode(
    sss_algorithm_t algorithm);

#if SSSFTR_SE05X_AuthSession
static smStatus_t se05x_CreateVerifyUserIDSession(pSe05xSession_t se05xSession,
    const uint32_t auth_id,
    SE05x_AuthCtx_ID_t *pin,
    pSe05xPolicy_t policy);
#endif

#if SSS_HAVE_SCP_SCP03_SSS
#if SSSFTR_SE05X_AuthFastSCP
static smStatus_t se05x_CreateFastScpSession(pSe05xSession_t se05xSession,
    const uint32_t auth_id,
    SE05x_AuthCtx_FastScp_t *pFScpCtx);
#endif

#if SSSFTR_SE05X_AuthSession
static smStatus_t se05x_CreateVerifyAppletSCP03Session(
    pSe05xSession_t se05xSession,
    const uint32_t auth_id,
    NXSCP03_AuthCtx_t *pAppletSCPCtx);
#endif
#endif

static smStatus_t sss_se05x_channel_txnRaw(const tlvHeader_t *hdr,
    uint8_t *cmdBuf,
    size_t cmdBufLen,
    uint8_t *rsp,
    size_t *rspLen,
    uint8_t hasle);
#if 0
static SE05x_RSASignatureAlgo_t se05x_get_rsa_sign_mode(
    sss_algorithm_t algorithm);
#endif

static SE05x_RSAEncryptionAlgo_t se05x_get_rsa_encrypt_mode(
    sss_algorithm_t algorithm);
static SE05x_RSASignatureAlgo_t se05x_get_rsa_sign_hash_mode(
    sss_algorithm_t algorithm);
static SE05x_CipherMode_t se05x_get_cipher_mode(sss_algorithm_t algorithm);
static SE05x_MACAlgo_t se05x_get_mac_algo(sss_algorithm_t algorithm);
static SE05x_DigestMode_t se05x_get_sha_algo(sss_algorithm_t algorithm);
#if SSSFTR_SE05X_KEY_SET
static uint8_t CheckIfKeyIdExists(uint32_t keyId, pSe05xSession_t session_ctx);
#endif
static smStatus_t sss_se05x_channel_txn(
    struct _sss_se05x_tunnel_context *pChannelCtx,
    SE_AuthType_t currAuth,
    const tlvHeader_t *hdr,
    uint8_t *cmdBuf,
    size_t cmdBufLen,
    uint8_t *rsp,
    size_t *rspLen,
    uint8_t hasle);

static smStatus_t sss_se05x_TXn(struct Se05xSession *pSession,
    const tlvHeader_t *hdr,
    uint8_t *cmdBuf,
    size_t cmdBufLen,
    uint8_t *rsp,
    size_t *rspLen,
    uint8_t hasle);

#if SSSFTR_SE05X_AuthFastSCP || SSSFTR_SE05X_AuthSession
static sss_status_t sss_session_auth_open(sss_se05x_session_t *session,
    sss_type_t subsystem,
    uint32_t auth_id,
    sss_connection_type_t connection_type,
    void *connectionData);
#endif

#if SSSFTR_SE05X_RSA && SSSFTR_SE05X_KEY_SET
static sss_status_t sss_se05x_key_store_set_rsa_key(
    sss_se05x_key_store_t *keyStore,
    sss_se05x_object_t *keyObject,
    const uint8_t *key,
    size_t keyLen,
    size_t keyBitLen,
    void *policy_buff,
    size_t policy_buff_len);
#endif

/* ************************************************************************** */
/* Defines                                                                    */
/* ************************************************************************** */

/* ************************************************************************** */
/* Functions : sss_se05x_session                                              */
/* ************************************************************************** */

sss_status_t sss_se05x_session_create(sss_se05x_session_t *session,
    sss_type_t subsystem,
    uint32_t application_id,
    sss_connection_type_t connetion_type,
    void *connectionData)
{
    sss_status_t retval = kStatus_SSS_Success;
    /* Nothing special to be handled */
    return retval;
}

sss_status_t sss_se05x_session_open(sss_se05x_session_t *session,
    sss_type_t subsystem,
    uint32_t application_id,
    sss_connection_type_t connection_type,
    void *connectionData)
{
    sss_status_t retval = kStatus_SSS_InvalidArgument;
    SE05x_Connect_Ctx_t *pAuthCtx = NULL;
    SmCommState_t CommState = {0};
    smStatus_t status = SM_NOT_OK;
    U16 lReturn;
    pSe05xSession_t se05xSession;
    ENSURE_OR_GO_EXIT(session);
    se05xSession = &session->s_ctx;

    memset(session, 0, sizeof(*session));

    ENSURE_OR_GO_EXIT(connectionData);
    pAuthCtx = (SE05x_Connect_Ctx_t *)connectionData;

    if (pAuthCtx->connType != kType_SE_Conn_Type_Channel) {
        uint8_t atr[100];
        uint16_t atrLen = ARRAY_SIZE(atr);
        CommState.connType = pAuthCtx->connType;
        if (1 == pAuthCtx->skip_select_applet) {
            CommState.skip_select_applet = 1;
        }
#if defined(SMCOM_JRCP_V1) || defined(SMCOM_JRCP_V2) || defined(RJCT_VCOM) || \
    defined(SMCOM_PCSC)
        lReturn = SM_RjctConnect(pAuthCtx->portName, &CommState, atr, &atrLen);
        if (lReturn != SW_OK) {
            LOG_E("SM_RjctConnect Failed. Status %04X", lReturn);
            goto exit;
        }
        LOG_AU8_I(atr, atrLen);
#else
        /* AX_EMBEDDED Or Native */
        lReturn = SM_Connect(&CommState, atr, &atrLen);
        if (lReturn != SW_OK) {
            LOG_E("SM_Connect Failed. Status %04X", lReturn);
            retval = kStatus_SSS_Fail;
            return retval;
        }
        LOG_AU8_I(atr, atrLen);
#endif
        if (1 == pAuthCtx->skip_select_applet) {
            status = lReturn;
            /* Not selecting the applet, so we don't know whether it's old or new */
        }
        else {
#define HEX_EXPECTED_APPLET_VERSION            \
    (0 | (APPLET_SE050_VER_MAJOR) << (8 * 3) | \
        (APPLET_SE050_VER_MINOR) << (8 * 2) |  \
        (APPLET_SE050_VER_DEV) << (8 * 1))

            if ((0xFFFFFF00 & CommState.appletVersion) ==
                HEX_EXPECTED_APPLET_VERSION) {
                /* Fine */
            }
            else if ((0xFFFFFF00 & CommState.appletVersion) <
                     HEX_EXPECTED_APPLET_VERSION) {
                LOG_E("Mistmatch Applet version.");
                LOG_E("Compiled for 0x%X. Got older 0x%X (last 8 bts not used)",
                    HEX_EXPECTED_APPLET_VERSION,
                    CommState.appletVersion);
                LOG_E("Aborting!!!");
                SM_Close(0);
                return kStatus_SSS_Fail;
            }
            else {
                LOG_W("Mistmatch Applet version.");
                LOG_W("Compiled for 0x%X. Got newer 0x%X (last 8 bts not used)",
                    HEX_EXPECTED_APPLET_VERSION,
                    CommState.appletVersion);
            }
        }
    }

    se05xSession->fp_TXn = &sss_se05x_TXn;
    se05xSession->fp_RawTXn = &sss_se05x_channel_txn;

    /* Auth type is None */
    if (1 == pAuthCtx->skip_select_applet) {
        /* Not selecting the applet */
    }
    else {
        if ((pAuthCtx->auth.authType == kSSS_AuthType_None) &&
            (connection_type == kSSS_ConnectionType_Plain)) {
            LOG_W("Communication channel is Plain.");
            LOG_W("!!!Not recommended for production use.!!!");
            se05xSession->fp_Transform = &se05x_Transform;
            se05xSession->fp_DeCrypt = &se05x_DeCrypt;
            se05xSession->authType = kSSS_AuthType_None;
            status = SM_OK;
        }
    }

#if SSS_HAVE_SCP_SCP03_SSS
    /* Auth type is Platform SCP03 */
    if ((pAuthCtx->auth.authType == kSSS_AuthType_SCP03) &&
        (connection_type == kSSS_ConnectionType_Encrypted)) {
        se05xSession->fp_Transform = &se05x_Transform;
        se05xSession->fp_DeCrypt = &se05x_DeCrypt;
        retval = nxScp03_AuthenticateChannel(
            se05xSession, &pAuthCtx->auth.ctx.scp03);
        if (retval == kStatus_SSS_Success) {
            pAuthCtx->auth.ctx.scp03.pDyn_ctx->authType =
                (uint8_t)kSSS_AuthType_SCP03;
            /*Auth type to Platform SCP03 again as channel authentication will modify it
            to auth type None*/
            se05xSession->authType = kSSS_AuthType_SCP03;
            se05xSession->pdynScp03Ctx = pAuthCtx->auth.ctx.scp03.pDyn_ctx;
            status = SM_OK;
            se05xSession->fp_Transform = &se05x_Transform_scp;
        }
        else {
            LOG_E("Could not set SCP03 Secure Channel");
        }
    }
#endif

#if SSSFTR_SE05X_AuthFastSCP || SSSFTR_SE05X_AuthSession
    if (pAuthCtx->connType == kType_SE_Conn_Type_Channel) {
        se05xSession->pChannelCtx =
            (struct _sss_se05x_tunnel_context *)pAuthCtx->tunnelCtx;
    }

    if ((application_id != 0) &&
        ((connection_type == kSSS_ConnectionType_Password) ||
            (connection_type == kSSS_ConnectionType_Encrypted))) {
        retval = sss_session_auth_open(session,
            subsystem,
            application_id,
            connection_type,
            connectionData);
        if (retval == kStatus_SSS_Success) {
            status = SM_OK;
        }
    }
#endif

    if (status == SM_OK) {
        session->subsystem = subsystem;
        retval = kStatus_SSS_Success;
    }
    else {
        memset(session, 0x00, sizeof(*session));
        retval = kStatus_SSS_Fail;
    }
exit:
    return retval;
}

#if SSSFTR_SE05X_AuthFastSCP || SSSFTR_SE05X_AuthSession
static sss_status_t sss_session_auth_open(sss_se05x_session_t *session,
    sss_type_t subsystem,
    uint32_t auth_id,
    sss_connection_type_t connect_type,
    void *connectionData)
{
    sss_status_t retval = kStatus_SSS_Fail;
    memset(session, 0, sizeof(*session));
    SE05x_Connect_Ctx_t *pAuthCtx;
    smStatus_t status = SM_NOT_OK;
    size_t valid_policy_buff_len = 0;
    Se05xPolicy_t se05x_policy;
    uint8_t *ppolicySet;
    retval = kStatus_SSS_Fail;
    pSe05xSession_t se05xSession = &session->s_ctx;
    uint8_t session_policies_buff[MAX_POLICY_BUFFER_SIZE];

    ENSURE_OR_GO_EXIT(connectionData != NULL);
    pAuthCtx = (SE05x_Connect_Ctx_t *)connectionData;

    if ((pAuthCtx->auth.authType == kSSS_AuthType_ID) &&
        (connect_type != kSSS_ConnectionType_Password)) {
        return retval;
    }
    if (((pAuthCtx->auth.authType == kSSS_AuthType_AppletSCP03) ||
            (pAuthCtx->auth.authType == kSSS_AuthType_FastSCP)) &&
        (connect_type != kSSS_ConnectionType_Encrypted)) {
        return retval;
    }

    se05xSession->fp_TXn = &sss_se05x_TXn;
    se05xSession->fp_RawTXn = &sss_se05x_channel_txn;
    if (pAuthCtx->connType == kType_SE_Conn_Type_Channel) {
        se05xSession->pChannelCtx =
            (struct _sss_se05x_tunnel_context *)pAuthCtx->tunnelCtx;
    }

    /*Session Policy check*/
    if (pAuthCtx->session_policy) {
        if (kStatus_SSS_Success !=
            sss_se05x_create_session_policy_buffer(pAuthCtx->session_policy,
                &session_policies_buff[0],
                &valid_policy_buff_len)) {
            goto exit;
        }
        ppolicySet = session_policies_buff;
    }
    else {
        ppolicySet = NULL;
    }
    se05x_policy.value = (uint8_t *)ppolicySet;
    se05x_policy.value_len = valid_policy_buff_len;

    /* Auth type is Platform UserID */

    if (pAuthCtx->auth.authType == kSSS_AuthType_ID)
#if SSSFTR_SE05X_AuthSession
    {
        LOG_W("Communication channel is with UserID (But Plain).");
        LOG_W("!!!Not recommended for production use.!!!");
        se05xSession->fp_Transform = &se05x_Transform;
        se05xSession->fp_DeCrypt = &se05x_DeCrypt;
        status = se05x_CreateVerifyUserIDSession(
            se05xSession, auth_id, &pAuthCtx->auth.ctx.idobj, &se05x_policy);
        if (status != SM_OK) {
            se05xSession->hasSession = 1;
            se05xSession->authType = kSSS_AuthType_ID;
        }
    }
#else
        LOG_W("User Id Support compiled out");
    status = SM_NOT_OK;
#endif

#if SSS_HAVE_SCP_SCP03_SSS
    /* Auth type is FastSCP */
    if ((pAuthCtx->auth.authType == kSSS_AuthType_FastSCP) && (auth_id != 0)) {
#if SSSFTR_SE05X_AuthFastSCP
        se05xSession->fp_Transform = &se05x_Transform;
        se05xSession->fp_DeCrypt = &se05x_DeCrypt;
        status = se05x_CreateFastScpSession(
            se05xSession, auth_id, &pAuthCtx->auth.ctx.fastscp);
        if (status == SM_OK) {
            se05xSession->fp_Transform = &se05x_Transform_scp;
            if (se05x_policy.value_len > 0) {
                status = SM_NOT_OK;
                status =
                    Se05x_API_ExchangeSessionData(se05xSession, &se05x_policy);
            }
        }
#else
        LOG_W("FastSCP Support compiled out");
        status = SM_NOT_OK;
#endif
    }
    /* Auth type is Applet SCP03 */
    if ((pAuthCtx->auth.authType == kSSS_AuthType_AppletSCP03) &&
        (auth_id != 0)) {
#if SSSFTR_SE05X_AuthSession
        se05xSession->fp_Transform = &se05x_Transform;
        se05xSession->fp_DeCrypt = &se05x_DeCrypt;
        status = se05x_CreateVerifyAppletSCP03Session(
            se05xSession, auth_id, &pAuthCtx->auth.ctx.scp03);
        if (status == SM_OK) {
            se05xSession->fp_Transform = &se05x_Transform_scp;
            if (se05x_policy.value_len > 0) {
                status = SM_NOT_OK;
                status =
                    Se05x_API_ExchangeSessionData(se05xSession, &se05x_policy);
            }
        }
#else
        LOG_W("AppletSCP Support compiled out");
        status = SM_NOT_OK;
#endif
    }

#endif

    if (status == SM_OK) {
        session->subsystem = subsystem;
        retval = kStatus_SSS_Success;
    }
    else {
        memset(session, 0x00, sizeof(*session));
        retval = kStatus_SSS_Fail;
    }

exit:
    return retval;
}

#endif

sss_status_t sss_se05x_refresh_session(
    sss_se05x_session_t *session, void *connectionData)
{
    sss_status_t retval = kStatus_SSS_Fail;
    pSe05xSession_t se05xSession = &session->s_ctx;
    sss_policy_session_u *session_policy =
        (sss_policy_session_u *)connectionData;
    smStatus_t status = SM_NOT_OK;
    size_t valid_policy_buff_len = 0;
    Se05xPolicy_t se05x_policy = {0};
    uint8_t *ppolicySet;
    uint8_t session_policies_buff[MAX_POLICY_BUFFER_SIZE];

    if (session_policy) {
        if (kStatus_SSS_Success !=
            sss_se05x_create_session_policy_buffer(session_policy,
                &session_policies_buff[0],
                &valid_policy_buff_len)) {
            goto exit;
        }
        ppolicySet = session_policies_buff;
        se05x_policy.value = (uint8_t *)ppolicySet;
        se05x_policy.value_len = valid_policy_buff_len;
    }
    else {
        ppolicySet = NULL;
        se05x_policy.value = NULL;
        se05x_policy.value_len = 0;
    }

    status = Se05x_API_RefreshSession(se05xSession, &se05x_policy);
    if (status == SM_OK) {
        retval = kStatus_SSS_Success;
    }
exit:
    return retval;
}

sss_status_t sss_se05x_session_prop_get_u32(
    sss_se05x_session_t *session, uint32_t property, uint32_t *pValue)
{
    sss_status_t retval = kStatus_SSS_Success;
    sss_session_prop_u32_t prop = property;

    if (pValue == NULL) {
        retval = kStatus_SSS_Fail;
        goto cleanup;
    }

    switch (prop) {
    case kSSS_SessionProp_VerMaj:
        *pValue = PLUGANDTRUST_HOSTLIB_VER_MAJOR;
        break;
    case kSSS_SessionProp_VerMin:
        *pValue = PLUGANDTRUST_HOSTLIB_VER_MINOR;
        break;
    case kSSS_SessionProp_VerDev:
        *pValue = PLUGANDTRUST_HOSTLIB_VER_DEV;
        break;
    case kSSS_SessionProp_UIDLen:
        *pValue = 18;
        break;
    default:
        *pValue = 0;
        retval = kStatus_SSS_Fail;
    }

cleanup:
    return retval;
}

sss_status_t sss_se05x_session_prop_get_au8(sss_se05x_session_t *session,
    uint32_t property,
    uint8_t *pValue,
    size_t *pValueLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    sss_session_prop_au8_t prop = property;
    sss_s05x_sesion_prop_au8_t se050xprop = property;
    smStatus_t sm_status = SM_NOT_OK;

    if (pValue == NULL || pValueLen == NULL) {
        goto cleanup;
    }

    switch (prop) {
    case kSSS_SessionProp_UID:
        if (*pValueLen >= 18) {
            sm_status = Se05x_API_ReadObject(&session->s_ctx,
                kSE05x_AppletResID_UNIQUE_ID,
                0,
                18,
                pValue,
                pValueLen);
        }
        else {
            LOG_D("Buffer too short");
        }
        break;
    default:;
    }

    if (sm_status == SM_OK)
        goto cleanup;

    switch (se050xprop) {
    case kSSS_SE05x_SessionProp_CertUID:
        if (*pValueLen >= 10) {
            uint8_t uid18[SE050_MODULE_UNIQUE_ID_LEN];
            size_t uid18Len = sizeof(uid18);

            sm_status = Se05x_API_ReadObject(&session->s_ctx,
                kSE05x_AppletResID_UNIQUE_ID,
                0,
                (uint16_t)uid18Len,
                uid18,
                &uid18Len);
            if (sm_status == SM_OK) {
                int idx = 0;
#define A71CH_UID_IC_TYPE_OFFSET 2
#define A71CH_UID_IC_FABRICATION_DATA_OFFSET 8
#define A71CH_UID_IC_SERIAL_NR_OFFSET 10
#define A71CH_UID_IC_BATCH_ID_OFFSET 13
                pValue[idx++] = uid18[A71CH_UID_IC_TYPE_OFFSET];
                pValue[idx++] = uid18[A71CH_UID_IC_TYPE_OFFSET + 1];
                pValue[idx++] = uid18[A71CH_UID_IC_FABRICATION_DATA_OFFSET];
                pValue[idx++] = uid18[A71CH_UID_IC_FABRICATION_DATA_OFFSET + 1];
                pValue[idx++] = uid18[A71CH_UID_IC_SERIAL_NR_OFFSET];
                pValue[idx++] = uid18[A71CH_UID_IC_SERIAL_NR_OFFSET + 1];
                pValue[idx++] = uid18[A71CH_UID_IC_SERIAL_NR_OFFSET + 2];
                pValue[idx++] = uid18[A71CH_UID_IC_BATCH_ID_OFFSET];
                pValue[idx++] = uid18[A71CH_UID_IC_BATCH_ID_OFFSET + 1];
                pValue[idx++] = uid18[A71CH_UID_IC_BATCH_ID_OFFSET + 2];
                *pValueLen = 10;
            }
        }
        break;
    }

cleanup:
    if (sm_status == SM_OK)
        retval = kStatus_SSS_Success;
    return retval;
}

void sss_se05x_session_close(sss_se05x_session_t *session)
{
    if (session->s_ctx.value[0] || session->s_ctx.value[1] ||
        session->s_ctx.value[2] || session->s_ctx.value[3] ||
        session->s_ctx.value[4] || session->s_ctx.value[5] ||
        session->s_ctx.value[6] || session->s_ctx.value[7]) {
        Se05x_API_CloseSession(&session->s_ctx);
    }
    if (session->s_ctx.pChannelCtx == NULL) {
        SM_Close(0);
    }
    memset(session, 0, sizeof(*session));
}

void sss_se05x_session_delete(sss_se05x_session_t *session)
{
    ;
}

/* End: se05x_session */

/* ************************************************************************** */
/* Functions : sss_se05x_keyobj                                               */
/* ************************************************************************** */

sss_status_t sss_se05x_key_object_init(
    sss_se05x_object_t *keyObject, sss_se05x_key_store_t *keyStore)
{
    sss_status_t retval = kStatus_SSS_Success;
    memset(keyObject, 0, sizeof(*keyObject));
    keyObject->keyStore = keyStore;

    return retval;
}

sss_status_t sss_se05x_key_object_allocate_handle(sss_se05x_object_t *keyObject,
    uint32_t keyId,
    sss_key_part_t keyPart,
    sss_cipher_type_t cipherType,
    size_t keyByteLenMax,
    uint32_t options)
{
    sss_status_t retval = kStatus_SSS_Success;
    smStatus_t status;
    SE05x_Result_t exists = kSE05x_Result_NA;
    keyObject->objectType = keyPart;
    keyObject->cipherType = cipherType;
    keyObject->keyId = keyId;
    if (options == kKeyObject_Mode_Persistent)
        keyObject->isPersistant = 1;

    status = Se05x_API_CheckObjectExists(
        &keyObject->keyStore->session->s_ctx, keyId, &exists);
    if (status == SM_OK) {
        if (exists == kSE05x_Result_SUCCESS) {
            LOG_W("Warning. Object id 0x%X exists", keyId);
        }
    }
    else {
        LOG_E("Couldn't check if object id 0x%X exists", keyId);
        return kStatus_SSS_Fail;
    }

    return retval;
}

//static sss_status_t sss_se05x_key_object_get_handle_binary(
//    sss_se05x_object_t *keyObject) {
//    sss_status_t retval = kStatus_SSS_Success;
//    keyObject->objectType = kSSS_KeyPart_Default;
//    keyObject->cipherType = kSSS_CipherType_Binary;
//    return retval;
//}
sss_status_t sss_se05x_key_object_get_handle(
    sss_se05x_object_t *keyObject, uint32_t keyId)
{
    sss_status_t retval = kStatus_SSS_Fail;
    SE05x_SecObjTyp_t retObjectType;
    uint8_t retTransientType;
    SE05x_ECCurve_t retCurveId;
    const SE05x_AttestationType_t attestationType = kSE05x_AttestationType_None;

    smStatus_t apiRetval =
        Se05x_API_ReadType(&keyObject->keyStore->session->s_ctx,
            keyId,
            &retObjectType,
            &retTransientType,
            attestationType);
    if (apiRetval == SM_OK) {
        keyObject->isPersistant = retTransientType;
        if (retObjectType >= kSE05x_SecObjTyp_EC_KEY_PAIR &&
            retObjectType <= kSE05x_SecObjTyp_EC_PUB_KEY) {
            apiRetval = Se05x_API_EC_CurveGetId(
                &keyObject->keyStore->session->s_ctx, keyId, &retCurveId);
            if (apiRetval == SM_OK) {
                keyObject->curve_id = retCurveId;
                if ((retCurveId >= kSE05x_ECCurve_NIST_P192) &&
                    (retCurveId <= kSE05x_ECCurve_NIST_P521)) {
                    keyObject->cipherType = kSSS_CipherType_EC_NIST_P;
                }
                else if ((retCurveId >= kSE05x_ECCurve_Brainpool160) &&
                         (retCurveId <= kSE05x_ECCurve_Brainpool512)) {
                    keyObject->cipherType = kSSS_CipherType_EC_BRAINPOOL;
                }
                else if ((retCurveId >= kSE05x_ECCurve_Secp160k1) &&
                         (retCurveId <= kSE05x_ECCurve_Secp256k1)) {
                    keyObject->cipherType = kSSS_CipherType_EC_NIST_K;
                }
                else if (retCurveId ==
                         kSE05x_ECCurve_RESERVED_ID_ECC_ED_25519) {
                    keyObject->cipherType = kSSS_CipherType_EC_TWISTED_ED;
                }
                else if (retCurveId ==
                         kSE05x_ECCurve_RESERVED_ID_ECC_MONT_DH_25519) {
                    keyObject->cipherType = kSSS_CipherType_EC_MONTGOMERY;
                }
                else if (retCurveId == kSE05x_ECCurve_TPM_ECC_BN_P256) {
                    keyObject->cipherType = kSSS_CipherType_EC_BARRETO_NAEHRIG;
                }
                else {
                    return retval;
                }
            }
            else {
                LOG_E("error in Se05x_API_GetECCurveId");
                return retval;
            }
        }
        else if (retObjectType >= kSE05x_SecObjTyp_RSA_KEY_PAIR &&
                 retObjectType <= kSE05x_SecObjTyp_RSA_PUB_KEY) {
            keyObject->cipherType = kSSS_CipherType_RSA;
        }
        else if (retObjectType == kSE05x_SecObjTyp_AES_KEY) {
            keyObject->cipherType = kSSS_CipherType_AES;
        }
        else if (retObjectType == kSE05x_SecObjTyp_DES_KEY) {
            keyObject->cipherType = kSSS_CipherType_DES;
        }
        else if (retObjectType == kSE05x_SecObjTyp_BINARY_FILE) {
            //keyObject->cipherType = kSSS_CipherType_Binary;
            keyObject->cipherType = kSSS_CipherType_Binary;
        }
        else if (retObjectType == kSE05x_SecObjTyp_UserID) {
            keyObject->cipherType = kSSS_CipherType_UserID;
        }
        else if (retObjectType == kSE05x_SecObjTyp_COUNTER) {
            keyObject->cipherType = kSSS_CipherType_Count;
        }
        else if (retObjectType == kSE05x_SecObjTyp_PCR) {
            keyObject->cipherType = kSSS_CipherType_PCR;
        }

        switch (retObjectType) {
        case kSE05x_SecObjTyp_EC_KEY_PAIR:
        case kSE05x_SecObjTyp_RSA_KEY_PAIR:
        case kSE05x_SecObjTyp_RSA_KEY_PAIR_CRT:
            keyObject->objectType = kSSS_KeyPart_Pair;
            break;
        case kSE05x_SecObjTyp_EC_PUB_KEY:
        case kSE05x_SecObjTyp_RSA_PUB_KEY:
            keyObject->objectType = kSSS_KeyPart_Public;
            break;
        case kSE05x_SecObjTyp_BINARY_FILE:
        case kSE05x_SecObjTyp_PCR:
            keyObject->objectType = kSSS_KeyPart_Default;
            break;
        default:
            keyObject->objectType = kSSS_KeyPart_NONE;
            break;
        }
    }
    else {
        LOG_E("error in Se05x_API_ReadType");
        return retval;
    }

    keyObject->keyId = keyId;
    retval = kStatus_SSS_Success;
    return retval;
}

sss_status_t sss_se05x_key_object_set_user(
    sss_se05x_object_t *keyObject, uint32_t user, uint32_t options)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* Purpose / Policy is set during creation time and hence can not
     * enforced in SE050 later on */
    LOG_W("Not supported in SE05X");
    return retval;
}

sss_status_t sss_se05x_key_object_set_purpose(
    sss_se05x_object_t *keyObject, sss_mode_t purpose, uint32_t options)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* Purpose / Policy is set during creation time and hence can not
     * enforced in SE050 later on */
    LOG_W("Not supported in SE05X");
    return retval;
}

sss_status_t sss_se05x_key_object_set_access(
    sss_se05x_object_t *keyObject, uint32_t access, uint32_t options)
{
    sss_status_t retval = kStatus_SSS_Fail;
    LOG_W("Not supported in SE05X");
    return retval;
}

sss_status_t sss_se05x_key_object_set_eccgfp_group(
    sss_se05x_object_t *keyObject, sss_eccgfp_group_t *group)
{
    sss_status_t retval = kStatus_SSS_Fail;
    LOG_W("Not supported in SE05X");
    return retval;
}

sss_status_t sss_se05x_key_object_get_user(
    sss_se05x_object_t *keyObject, uint32_t *user)
{
    sss_status_t retval = kStatus_SSS_Fail;
    LOG_W("Not supported in SE05X");
    return retval;
}

sss_status_t sss_se05x_key_object_get_purpose(
    sss_se05x_object_t *keyObject, sss_mode_t *purpose)
{
    sss_status_t retval = kStatus_SSS_Fail;
    LOG_W("Not supported in SE05X");
    return retval;
}

sss_status_t sss_se05x_key_object_get_access(
    sss_se05x_object_t *keyObject, uint32_t *access)
{
    sss_status_t retval = kStatus_SSS_Fail;
    LOG_W("Not supported in SE05X");
    return retval;
}

void sss_se05x_key_object_free(sss_se05x_object_t *keyObject)
{
    memset(keyObject, 0, sizeof(*keyObject));
}

/* End: se05x_keyobj */

/* ************************************************************************** */
/* Functions : sss_se05x_keyderive                                            */
/* ************************************************************************** */

sss_status_t sss_se05x_derive_key_context_init(sss_se05x_derive_key_t *context,
    sss_se05x_session_t *session,
    sss_se05x_object_t *keyObject,
    sss_algorithm_t algorithm,
    sss_mode_t mode)
{
    sss_status_t retval = kStatus_SSS_Success;

    context->session = session;
    context->keyObject = keyObject;
    context->algorithm = algorithm;
    context->mode = mode;

    return retval;
}

sss_status_t sss_se05x_derive_key_go(sss_se05x_derive_key_t *context,
    const uint8_t *saltData,
    size_t saltLen,
    const uint8_t *info,
    size_t infoLen,
    sss_se05x_object_t *derivedKeyObject,
    uint16_t deriveDataLen,
    uint8_t *hkdfOutput,
    size_t *hkdfOutputLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;

    SE05x_DigestMode_t digestMode;
    digestMode = se05x_get_sha_algo(context->algorithm);

    status = Se05x_API_HKDF(&context->session->s_ctx,
        context->keyObject->keyId,
        digestMode,
        saltData,
        saltLen,
        info,
        infoLen,
        deriveDataLen,
        hkdfOutput,
        hkdfOutputLen);

    ENSURE_OR_GO_EXIT(status == SM_OK);

    retval = kStatus_SSS_Success;
exit:

    return retval;
}

sss_status_t sss_se05x_derive_key_dh(sss_se05x_derive_key_t *context,
    sss_se05x_object_t *otherPartyKeyObject,
    sss_se05x_object_t *derivedKeyObject)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;
    uint8_t pubkey[256] = {0};
    size_t pubkeylen = sizeof(pubkey);
    uint8_t sharedsecret[256];
    size_t sharedsecretLen = sizeof(sharedsecret);
    size_t pbKeyBitLen = 0;
    uint8_t *pPublicKey = NULL;
    size_t publicKeyLen = 0;
    uint16_t publicKeyIndex = 0;

    sss_object_t *sss_other_keyObject = (sss_object_t *)otherPartyKeyObject;
    sss_object_t *sss_derived_keyObject = (sss_object_t *)derivedKeyObject;

    retval =
        sss_key_store_get_key((sss_key_store_t *)sss_other_keyObject->keyStore,
            sss_other_keyObject,
            pubkey,
            &pubkeylen,
            &pbKeyBitLen);
    ENSURE_OR_GO_EXIT(retval == kStatus_SSS_Success);

    switch (otherPartyKeyObject->curve_id) {
    case kSE05x_ECCurve_TPM_ECC_BN_P256:
        /* TODO: Implement asn parser */
        publicKeyLen = pubkeylen;
        publicKeyIndex = 0;
        break;
    default: {
        retval = sss_util_pkcs8_asn1_get_ec_public_key_index(
            (const uint8_t *)pubkey, pubkeylen, &publicKeyIndex, &publicKeyLen);
        if (retval != kStatus_SSS_Success) {
            LOG_W("error in sss_util_pkcs8_asn1_get_ec_public_key_index");
            goto exit;
        }
    }
    }

    pPublicKey = &pubkey[publicKeyIndex];

    status = Se05x_API_ECGenSharedSecret(&context->session->s_ctx,
        context->keyObject->keyId,
        pPublicKey,
        publicKeyLen,
        sharedsecret,
        &sharedsecretLen);
    if (status != SM_OK) {
        retval = kStatus_SSS_Fail;
        goto exit;
    }

    retval =
        sss_key_store_set_key((sss_key_store_t *)derivedKeyObject->keyStore,
            sss_derived_keyObject,
            sharedsecret,
            sharedsecretLen,
            sharedsecretLen * 8,
            NULL,
            0);
    ENSURE_OR_GO_EXIT(retval == kStatus_SSS_Success);

    retval = kStatus_SSS_Success;
exit:
    return retval;
}

void sss_se05x_derive_key_context_free(sss_se05x_derive_key_t *context)
{
    ;
}

/* End: se05x_keyderive */

/* ************************************************************************** */
/* Functions : sss_se05x_keystore                                             */
/* ************************************************************************** */

sss_status_t sss_se05x_key_store_context_init(
    sss_se05x_key_store_t *keyStore, sss_se05x_session_t *session)
{
    sss_status_t retval = kStatus_SSS_Success;
    memset(keyStore, 0, sizeof(*keyStore));
    keyStore->session = session;
    return retval;
}

sss_status_t sss_se05x_key_store_allocate(
    sss_se05x_key_store_t *keyStore, uint32_t keyStoreId)
{
    return kStatus_SSS_Success;
}

sss_status_t sss_se05x_key_store_save(sss_se05x_key_store_t *keyStore)
{
    return kStatus_SSS_Success;
}

sss_status_t sss_se05x_key_store_load(sss_se05x_key_store_t *keyStore)
{
    return kStatus_SSS_Success;
}

#if SSSFTR_SE05X_RSA && SSSFTR_SE05X_KEY_SET
static sss_status_t sss_se05x_key_store_set_rsa_key(
    sss_se05x_key_store_t *keyStore,
    sss_se05x_object_t *keyObject,
    const uint8_t *key,
    size_t keyLen,
    size_t keyBitLen,
    void *policy_buff,
    size_t policy_buff_len)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;
    //int ret;
    uint32_t key_type = keyObject->objectType;
    Se05xPolicy_t se05x_policy;
    //SE05x_KeyPart_t key_part;
    uint8_t *rsaN = NULL, *rsaE = NULL, *rsaD = NULL;
    uint8_t *rsaP = NULL, *rsaQ = NULL, *rsaDP = NULL, *rsaDQ = NULL,
            *rsaQINV = NULL;
    size_t rsaNlen, rsaElen, rsaDlen;
    size_t rsaPlen, rsaQlen, rsaDPlen, rsaDQlen, rsaQINVlen;

    se05x_policy.value = (uint8_t *)policy_buff;
    se05x_policy.value_len = policy_buff_len;
    SE05x_TransientType_t transient_type;
    SE05x_RSAKeyFormat_t rsa_format;
    uint8_t IdExists = 0;
    size_t keyBitLength = 0;

    (keyObject->isPersistant) ?
        (transient_type = kSE05x_TransientType_Persistent) :
        (transient_type = kSE05x_TransientType_Transient);

    if (keyObject->cipherType == kSSS_CipherType_RSA)
        rsa_format = kSE05x_RSAKeyFormat_RAW;
    else if (keyObject->cipherType == kSSS_CipherType_RSA_CRT)
        rsa_format = kSE05x_RSAKeyFormat_CRT;
    else {
        retval = kStatus_SSS_Fail;
        goto exit;
    }

#if 0
    if (key_type == kSSS_KeyPart_Public)
        key_part = SE05x_KeyPart_Public;
    else if (key_type == kSSS_KeyPart_Private)
        key_part = kSE05x_KeyPart_Private;
    else if (key_type == kSSS_KeyPart_Pair)
        key_part = kSE05x_KeyPart_Pair;
    else {
        retval = kStatus_SSS_Fail;
        goto exit;
    }

    /* Set the kye parameters */
    status = Se05x_API_WriteRSAKey(&keyStore->session->s_ctx,
        &se05x_policy,
        keyObject->keyId,
        (U16)keyBitLen,
        SE05X_RSA_NO_p,
        SE05X_RSA_NO_q,
        SE05X_RSA_NO_dp,
        SE05X_RSA_NO_dq,
        SE05X_RSA_NO_qInv,
        SE05X_RSA_NO_pubExp,
        SE05X_RSA_NO_priv,
        SE05X_RSA_NO_pubMod,
        transient_type,
        key_part,
        rsa_format);

    if (status != SM_OK) {
        retval = kStatus_SSS_Fail;
        goto exit;
    }
#endif

    if (key_type == kSSS_KeyPart_Public) {
        retval = sss_util_asn1_rsa_parse_public(
            key, keyLen, &rsaN, &rsaNlen, &rsaE, &rsaElen);
        ENSURE_OR_GO_EXIT(retval == kStatus_SSS_Success);

        IdExists =
            CheckIfKeyIdExists(keyObject->keyId, &keyStore->session->s_ctx);
        keyBitLength = (IdExists == 1) ? 0 : keyBitLen;

        /* Set the Public Exponent */
        status = Se05x_API_WriteRSAKey(&keyStore->session->s_ctx,
            &se05x_policy,
            keyObject->keyId,
            (U16)keyBitLength,
            SE05X_RSA_NO_p,
            SE05X_RSA_NO_q,
            SE05X_RSA_NO_dp,
            SE05X_RSA_NO_dq,
            SE05X_RSA_NO_qInv,
            rsaE,
            rsaElen,
            SE05X_RSA_NO_priv,
            SE05X_RSA_NO_pubMod,
            transient_type,
            kSE05x_KeyPart_Public,
            rsa_format);
        if (status != SM_OK) {
            retval = kStatus_SSS_Fail;
            goto exit;
        }

        /* Set the Modulus */
        status = Se05x_API_WriteRSAKey(&keyStore->session->s_ctx,
            NULL,
            keyObject->keyId,
            0,
            SE05X_RSA_NO_p,
            SE05X_RSA_NO_q,
            SE05X_RSA_NO_dp,
            SE05X_RSA_NO_dq,
            SE05X_RSA_NO_qInv,
            SE05X_RSA_NO_pubExp,
            SE05X_RSA_NO_priv,
            rsaN,
            rsaNlen,
            transient_type,
            kSE05x_KeyPart_NA,
            rsa_format);

        if (status != SM_OK) {
            retval = kStatus_SSS_Fail;
            goto exit;
        }
    }
    else if (key_type == kSSS_KeyPart_Private) {
        if (keyObject->cipherType == kSSS_CipherType_RSA) {
            retval = sss_util_asn1_rsa_parse_private(key,
                keyLen,
                keyObject->cipherType,
                &rsaN,
                &rsaNlen,
                NULL,
                NULL,
                &rsaD,
                &rsaDlen,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL);
            if (retval != kStatus_SSS_Success) {
                retval = kStatus_SSS_Fail;
                goto exit;
            }
            if ((rsaN == NULL) || (rsaD == NULL)) {
                retval = kStatus_SSS_Fail;
                goto exit;
            }

            IdExists =
                CheckIfKeyIdExists(keyObject->keyId, &keyStore->session->s_ctx);
            keyBitLength = (IdExists == 1) ? 0 : keyBitLen;

            // Set D(Private exponent) component
            status = Se05x_API_WriteRSAKey(&keyStore->session->s_ctx,
                &se05x_policy,
                keyObject->keyId,
                (U16)keyBitLength,
                SE05X_RSA_NO_p,
                SE05X_RSA_NO_q,
                SE05X_RSA_NO_dp,
                SE05X_RSA_NO_dq,
                SE05X_RSA_NO_qInv,
                SE05X_RSA_NO_pubExp,
                rsaD,
                rsaDlen,
                SE05X_RSA_NO_pubMod,
                transient_type,
                kSE05x_KeyPart_Pair,
                rsa_format);

            if (status != SM_OK) {
                retval = kStatus_SSS_Fail;
                goto exit;
            }

            // Set N(Modulus) component
            status = Se05x_API_WriteRSAKey(&keyStore->session->s_ctx,
                NULL,
                keyObject->keyId,
                0,
                SE05X_RSA_NO_p,
                SE05X_RSA_NO_q,
                SE05X_RSA_NO_dp,
                SE05X_RSA_NO_dq,
                SE05X_RSA_NO_qInv,
                SE05X_RSA_NO_pubExp,
                SE05X_RSA_NO_priv,
                rsaN,
                rsaNlen,
                transient_type,
                kSE05x_KeyPart_NA,
                rsa_format);

            if (status != SM_OK) {
                retval = kStatus_SSS_Fail;
                goto exit;
            }
        }
        else if (keyObject->cipherType == kSSS_CipherType_RSA_CRT) {
            retval = sss_util_asn1_rsa_parse_private(key,
                keyLen,
                keyObject->cipherType,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                &rsaP,
                &rsaPlen,
                &rsaQ,
                &rsaQlen,
                &rsaDP,
                &rsaDPlen,
                &rsaDQ,
                &rsaDQlen,
                &rsaQINV,
                &rsaQINVlen);
            if (retval != kStatus_SSS_Success) {
                retval = kStatus_SSS_Fail;
                goto exit;
            }
            if ((rsaP == NULL) || (rsaQ == NULL) || (rsaDP == NULL) ||
                (rsaDQ == NULL) || (rsaQINV == NULL)) {
                retval = kStatus_SSS_Fail;
                goto exit;
            }

            IdExists =
                CheckIfKeyIdExists(keyObject->keyId, &keyStore->session->s_ctx);
            keyBitLength = (IdExists == 1) ? 0 : keyBitLen;

            // Set P component
            status = Se05x_API_WriteRSAKey(&keyStore->session->s_ctx,
                &se05x_policy,
                keyObject->keyId,
                (U16)keyBitLength,
                rsaP,
                rsaPlen,
                SE05X_RSA_NO_q,
                SE05X_RSA_NO_dp,
                SE05X_RSA_NO_dq,
                SE05X_RSA_NO_qInv,
                SE05X_RSA_NO_pubExp,
                SE05X_RSA_NO_priv,
                SE05X_RSA_NO_pubMod,
                transient_type,
                kSE05x_KeyPart_Private,
                rsa_format);

            if (status != SM_OK) {
                retval = kStatus_SSS_Fail;
                goto exit;
            }

            // Set Q component
            status = Se05x_API_WriteRSAKey(&keyStore->session->s_ctx,
                NULL,
                keyObject->keyId,
                0,
                SE05X_RSA_NO_p,
                rsaQ,
                rsaQlen,
                SE05X_RSA_NO_dp,
                SE05X_RSA_NO_dq,
                SE05X_RSA_NO_qInv,
                SE05X_RSA_NO_pubExp,
                SE05X_RSA_NO_priv,
                SE05X_RSA_NO_pubMod,
                transient_type,
                kSE05x_KeyPart_NA,
                rsa_format);

            if (status != SM_OK) {
                retval = kStatus_SSS_Fail;
                goto exit;
            }

            // Set DP component
            status = Se05x_API_WriteRSAKey(&keyStore->session->s_ctx,
                NULL,
                keyObject->keyId,
                0,
                SE05X_RSA_NO_p,
                SE05X_RSA_NO_q,
                rsaDP,
                rsaDPlen,
                SE05X_RSA_NO_dq,
                SE05X_RSA_NO_qInv,
                SE05X_RSA_NO_pubExp,
                SE05X_RSA_NO_priv,
                SE05X_RSA_NO_pubMod,
                transient_type,
                kSE05x_KeyPart_NA,
                rsa_format);

            if (status != SM_OK) {
                retval = kStatus_SSS_Fail;
                goto exit;
            }

            // Set DQ component
            status = Se05x_API_WriteRSAKey(&keyStore->session->s_ctx,
                NULL,
                keyObject->keyId,
                0,
                SE05X_RSA_NO_p,
                SE05X_RSA_NO_q,
                SE05X_RSA_NO_dp,
                rsaDQ,
                rsaDQlen,
                SE05X_RSA_NO_qInv,
                SE05X_RSA_NO_pubExp,
                SE05X_RSA_NO_priv,
                SE05X_RSA_NO_pubMod,
                transient_type,
                kSE05x_KeyPart_NA,
                rsa_format);

            if (status != SM_OK) {
                retval = kStatus_SSS_Fail;
                goto exit;
            }

            // Set INV_Q component
            status = Se05x_API_WriteRSAKey(&keyStore->session->s_ctx,
                NULL,
                keyObject->keyId,
                0,
                SE05X_RSA_NO_p,
                SE05X_RSA_NO_q,
                SE05X_RSA_NO_dp,
                SE05X_RSA_NO_dq,
                rsaQINV,
                rsaQINVlen,
                SE05X_RSA_NO_pubExp,
                SE05X_RSA_NO_priv,
                SE05X_RSA_NO_pubMod,
                transient_type,
                kSE05x_KeyPart_NA,
                rsa_format);

            if (status != SM_OK) {
                retval = kStatus_SSS_Fail;
                goto exit;
            }
        }
    }
    else if (key_type == kSSS_KeyPart_Pair) {
        if (keyObject->cipherType == kSSS_CipherType_RSA) {
            retval = sss_util_asn1_rsa_parse_private(key,
                keyLen,
                keyObject->cipherType,
                &rsaN,
                &rsaNlen,
                &rsaE,
                &rsaElen,
                &rsaD,
                &rsaDlen,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL);

            ENSURE_OR_GO_EXIT(retval == kStatus_SSS_Success);
            ENSURE_OR_EXIT_WITH_STATUS_ON_ERROR(
                !((rsaD == NULL) || (rsaE == NULL) || (rsaN == NULL)),
                retval,
                kStatus_SSS_Fail);

            IdExists =
                CheckIfKeyIdExists(keyObject->keyId, &keyStore->session->s_ctx);
            keyBitLength = (IdExists == 1) ? 0 : keyBitLen;

            // Set E(Public exponent) component
            status = Se05x_API_WriteRSAKey(&keyStore->session->s_ctx,
                &se05x_policy,
                keyObject->keyId,
                (U16)keyBitLength,
                SE05X_RSA_NO_p,
                SE05X_RSA_NO_q,
                SE05X_RSA_NO_dp,
                SE05X_RSA_NO_dq,
                SE05X_RSA_NO_qInv,
                rsaE,
                rsaElen,
                SE05X_RSA_NO_priv,
                SE05X_RSA_NO_pubMod,
                transient_type,
                kSE05x_KeyPart_Pair,
                rsa_format);

            if (status != SM_OK) {
                retval = kStatus_SSS_Fail;
                goto exit;
            }

            // Set D(Private exponent) component
            status = Se05x_API_WriteRSAKey(&keyStore->session->s_ctx,
                NULL,
                keyObject->keyId,
                0,
                SE05X_RSA_NO_p,
                SE05X_RSA_NO_q,
                SE05X_RSA_NO_dp,
                SE05X_RSA_NO_dq,
                SE05X_RSA_NO_qInv,
                SE05X_RSA_NO_pubExp,
                rsaD,
                rsaDlen,
                SE05X_RSA_NO_pubMod,
                transient_type,
                kSE05x_KeyPart_NA,
                rsa_format);

            if (status != SM_OK) {
                retval = kStatus_SSS_Fail;
                goto exit;
            }

            // Set N(Modulus) component
            status = Se05x_API_WriteRSAKey(&keyStore->session->s_ctx,
                NULL,
                keyObject->keyId,
                0,
                SE05X_RSA_NO_p,
                SE05X_RSA_NO_q,
                SE05X_RSA_NO_dp,
                SE05X_RSA_NO_dq,
                SE05X_RSA_NO_qInv,
                SE05X_RSA_NO_pubExp,
                SE05X_RSA_NO_priv,
                rsaN,
                rsaNlen,
                transient_type,
                kSE05x_KeyPart_NA,
                rsa_format);

            if (status != SM_OK) {
                retval = kStatus_SSS_Fail;
                goto exit;
            }
        }
        else if (keyObject->cipherType == kSSS_CipherType_RSA_CRT) {
            retval = sss_util_asn1_rsa_parse_private(key,
                keyLen,
                keyObject->cipherType,
                &rsaN,
                &rsaNlen,
                &rsaE,
                &rsaElen,
                NULL,
                NULL,
                &rsaP,
                &rsaPlen,
                &rsaQ,
                &rsaQlen,
                &rsaDP,
                &rsaDPlen,
                &rsaDQ,
                &rsaDQlen,
                &rsaQINV,
                &rsaQINVlen);
            ENSURE_OR_GO_EXIT(retval == kStatus_SSS_Success);

            if ((rsaP == NULL) || (rsaQ == NULL) || (rsaDP == NULL) ||
                (rsaDQ == NULL) || (rsaQINV == NULL) || (rsaE == NULL) ||
                (rsaN == NULL)) {
                retval = kStatus_SSS_Fail;
                goto exit;
            }

            IdExists =
                CheckIfKeyIdExists(keyObject->keyId, &keyStore->session->s_ctx);
            keyBitLength = (IdExists == 1) ? 0 : keyBitLen;

            // Set P component
            status = Se05x_API_WriteRSAKey(&keyStore->session->s_ctx,
                &se05x_policy,
                keyObject->keyId,
                (U16)keyBitLength,
                rsaP,
                rsaPlen,
                SE05X_RSA_NO_q,
                SE05X_RSA_NO_dp,
                SE05X_RSA_NO_dq,
                SE05X_RSA_NO_qInv,
                SE05X_RSA_NO_pubExp,
                SE05X_RSA_NO_priv,
                SE05X_RSA_NO_pubMod,
                transient_type,
                kSE05x_KeyPart_Pair,
                rsa_format);

            if (status != SM_OK) {
                retval = kStatus_SSS_Fail;
                goto exit;
            }

            // Set Q component
            status = Se05x_API_WriteRSAKey(&keyStore->session->s_ctx,
                NULL,
                keyObject->keyId,
                0,
                SE05X_RSA_NO_p,
                rsaQ,
                rsaQlen,
                SE05X_RSA_NO_dp,
                SE05X_RSA_NO_dq,
                SE05X_RSA_NO_qInv,
                SE05X_RSA_NO_pubExp,
                SE05X_RSA_NO_priv,
                SE05X_RSA_NO_pubMod,
                transient_type,
                kSE05x_KeyPart_NA,
                rsa_format);

            if (status != SM_OK) {
                retval = kStatus_SSS_Fail;
                goto exit;
            }

            // Set DP component
            status = Se05x_API_WriteRSAKey(&keyStore->session->s_ctx,
                NULL,
                keyObject->keyId,
                0,
                SE05X_RSA_NO_p,
                SE05X_RSA_NO_q,
                rsaDP,
                rsaDPlen,
                SE05X_RSA_NO_dq,
                SE05X_RSA_NO_qInv,
                SE05X_RSA_NO_pubExp,
                SE05X_RSA_NO_priv,
                SE05X_RSA_NO_pubMod,
                transient_type,
                kSE05x_KeyPart_NA,
                rsa_format);

            if (status != SM_OK) {
                retval = kStatus_SSS_Fail;
                goto exit;
            }

            // Set DQ component
            status = Se05x_API_WriteRSAKey(&keyStore->session->s_ctx,
                NULL,
                keyObject->keyId,
                0,
                SE05X_RSA_NO_p,
                SE05X_RSA_NO_q,
                SE05X_RSA_NO_dp,
                rsaDQ,
                rsaDQlen,
                SE05X_RSA_NO_qInv,
                SE05X_RSA_NO_pubExp,
                SE05X_RSA_NO_priv,
                SE05X_RSA_NO_pubMod,
                transient_type,
                kSE05x_KeyPart_NA,
                rsa_format);

            if (status != SM_OK) {
                retval = kStatus_SSS_Fail;
                goto exit;
            }

            // Set INV_Q component
            status = Se05x_API_WriteRSAKey(&keyStore->session->s_ctx,
                NULL,
                keyObject->keyId,
                0,
                SE05X_RSA_NO_p,
                SE05X_RSA_NO_q,
                SE05X_RSA_NO_dp,
                SE05X_RSA_NO_dq,
                rsaQINV,
                rsaQINVlen,
                SE05X_RSA_NO_pubExp,
                SE05X_RSA_NO_priv,
                SE05X_RSA_NO_pubMod,
                transient_type,
                kSE05x_KeyPart_NA,
                rsa_format);

            if (status != SM_OK) {
                retval = kStatus_SSS_Fail;
                goto exit;
            }

            // Set E (Public exponent) component
            status = Se05x_API_WriteRSAKey(&keyStore->session->s_ctx,
                NULL,
                keyObject->keyId,
                0,
                SE05X_RSA_NO_p,
                SE05X_RSA_NO_q,
                SE05X_RSA_NO_dp,
                SE05X_RSA_NO_dq,
                SE05X_RSA_NO_qInv,
                rsaE,
                rsaElen,
                SE05X_RSA_NO_priv,
                SE05X_RSA_NO_pubMod,
                transient_type,
                kSE05x_KeyPart_NA,
                rsa_format);

            if (status != SM_OK) {
                retval = kStatus_SSS_Fail;
                goto exit;
            }

            // Set N (Modulus) component
            status = Se05x_API_WriteRSAKey(&keyStore->session->s_ctx,
                NULL,
                keyObject->keyId,
                0,
                SE05X_RSA_NO_p,
                SE05X_RSA_NO_q,
                SE05X_RSA_NO_dp,
                SE05X_RSA_NO_dq,
                SE05X_RSA_NO_qInv,
                SE05X_RSA_NO_pubExp,
                SE05X_RSA_NO_priv,
                rsaN,
                rsaNlen,
                transient_type,
                kSE05x_KeyPart_NA,
                rsa_format);

            if (status != SM_OK) {
                retval = kStatus_SSS_Fail;
                goto exit;
            }
        }
    }
exit:
    if (rsaN != NULL)
        free(rsaN);
    if (rsaE != NULL)
        free(rsaE);
    if (rsaD != NULL)
        free(rsaD);
    if (rsaP != NULL)
        free(rsaP);
    if (rsaQ != NULL)
        free(rsaQ);
    if (rsaDP != NULL)
        free(rsaDP);
    if (rsaDQ != NULL)
        free(rsaDQ);
    if (rsaQINV != NULL)
        free(rsaQINV);

    return retval;
}
#endif // SSSFTR_SE05X_RSA && SSSFTR_SE05X_KEY_SET

#if SSSFTR_SE05X_ECC && SSSFTR_SE05X_KEY_SET
static smStatus_t CreateCurveIfNeeded(
    Se05xSession_t *pSession, uint32_t curve_id)
{
    smStatus_t status = SM_NOT_OK;
    //uint32_t existing_curve_id = 0;
    uint8_t curveList[kSE05x_ECCurve_Total_Weierstrass_Curves] = {
        0,
    };
    size_t curveListLen = sizeof(curveList);
    //int i = 0;

    if (curve_id == kSE05x_ECCurve_RESERVED_ID_ECC_ED_25519 ||
        curve_id == kSE05x_ECCurve_RESERVED_ID_ECC_MONT_DH_25519) {
        return SM_OK;
        /* ECC_ED_25519 and ECC_MONT_DH_25519 are always present */
    }

    status = Se05x_API_ReadECCurveList(pSession, curveList, &curveListLen);
    if (status == SM_OK) {
        if (curveList[curve_id - 1] == kSE05x_SetIndicator_SET) {
            return SM_OK;
        }
    }
    else {
        return SM_NOT_OK;
    }

    switch (curve_id) {
    case kSE05x_ECCurve_NIST_P192:
        status = Se05x_API_CreateCurve_prime192v1(pSession, NULL, curve_id);
        break;
    case kSE05x_ECCurve_NIST_P224:
        status = Se05x_API_CreateCurve_secp224r1(pSession, NULL, curve_id);
        break;
    case kSE05x_ECCurve_NIST_P256:
        status = Se05x_API_CreateCurve_prime256v1(pSession, NULL, curve_id);
        break;
    case kSE05x_ECCurve_NIST_P384:
        status = Se05x_API_CreateCurve_secp384r1(pSession, NULL, curve_id);
        break;
    case kSE05x_ECCurve_NIST_P521:
        status = Se05x_API_CreateCurve_secp521r1(pSession, NULL, curve_id);
        break;
    case kSE05x_ECCurve_Brainpool160:
        status =
            Se05x_API_CreateCurve_brainpoolP160r1(pSession, NULL, curve_id);
        break;
    case kSE05x_ECCurve_Brainpool192:
        status =
            Se05x_API_CreateCurve_brainpoolP192r1(pSession, NULL, curve_id);
        break;
    case kSE05x_ECCurve_Brainpool224:
        status =
            Se05x_API_CreateCurve_brainpoolP224r1(pSession, NULL, curve_id);
        break;
    case kSE05x_ECCurve_Brainpool256:
        status =
            Se05x_API_CreateCurve_brainpoolP256r1(pSession, NULL, curve_id);
        break;
    case kSE05x_ECCurve_Brainpool320:
        status =
            Se05x_API_CreateCurve_brainpoolP320r1(pSession, NULL, curve_id);
        break;
    case kSE05x_ECCurve_Brainpool384:
        status =
            Se05x_API_CreateCurve_brainpoolP384r1(pSession, NULL, curve_id);
        break;
    case kSE05x_ECCurve_Brainpool512:
        status =
            Se05x_API_CreateCurve_brainpoolP512r1(pSession, NULL, curve_id);
        break;
    case kSE05x_ECCurve_Secp160k1:
        status = Se05x_API_CreateCurve_secp160k1(pSession, NULL, curve_id);
        break;
    case kSE05x_ECCurve_Secp192k1:
        status = Se05x_API_CreateCurve_secp192k1(pSession, NULL, curve_id);
        break;
    case kSE05x_ECCurve_Secp224k1:
        status = Se05x_API_CreateCurve_secp224k1(pSession, NULL, curve_id);
        break;
    case kSE05x_ECCurve_Secp256k1:
        status = Se05x_API_CreateCurve_secp256k1(pSession, NULL, curve_id);
        break;
    case kSE05x_ECCurve_TPM_ECC_BN_P256:
        status = Se05x_API_CreateCurve_tpm_bm_p256(pSession, NULL, curve_id);
        break;
    default:
        break;
    }

    ENSURE_OR_GO_EXIT(status != SM_NOT_OK);
    if (status == SM_ERR_CONDITIONS_OF_USE_NOT_SATISFIED) {
        LOG_W(
            "Allowing SM_ERR_CONDITIONS_OF_USE_NOT_SATISFIED for CreateCurve");
    }
exit:
    return status;
}
#endif // SSSFTR_SE05X_ECC && SSSFTR_SE05X_KEY_SET

#if SSSFTR_SE05X_KEY_SET
static uint8_t CheckIfKeyIdExists(uint32_t keyId, pSe05xSession_t session_ctx)
{
    smStatus_t retStatus = SM_NOT_OK;
    SE05x_Result_t IdExists = 0;

    retStatus = Se05x_API_CheckObjectExists(session_ctx, keyId, &IdExists);
    if (retStatus == SM_OK) {
        if (IdExists == kSE05x_Result_SUCCESS) {
            LOG_W("Key Id 0x%X exists", keyId);
            return 1;
        }
        else {
            return 0;
        }
    }
    else {
        LOG_E("Error in Se05x_API_CheckObjectExists");
        return 0;
    }
}
#endif

#if SSSFTR_SE05X_ECC && SSSFTR_SE05X_KEY_SET
static sss_status_t sss_se05x_key_store_set_ecc_key(
    sss_se05x_key_store_t *keyStore,
    sss_se05x_object_t *keyObject,
    const uint8_t *key,
    size_t keyLen,
    size_t keyBitLen,
    void *policy_buff,
    size_t policy_buff_len)
{
    sss_status_t retval = kStatus_SSS_Fail;
    sss_status_t asn_retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;
    Se05xPolicy_t se05x_policy;
    SE05x_TransientType_t transient_type;
    SE05x_ECCurve_t curveId = keyObject->curve_id;
    SE05x_KeyPart_t key_part = kSE05x_KeyPart_NA;
    SE05x_Result_t exists = kSE05x_Result_NA;
    SE05x_ECCurve_t retCurveId = keyObject->curve_id;

    (keyObject->isPersistant) ?
        (transient_type = kSE05x_TransientType_Persistent) :
        (transient_type = kSE05x_TransientType_Transient);

    se05x_policy.value = (uint8_t *)policy_buff;
    se05x_policy.value_len = policy_buff_len;

    if (keyObject->curve_id == 0) {
        keyObject->curve_id =
            se05x_sssKeyTypeLenToCurveId(keyObject->cipherType, keyBitLen);
    }

    if (keyObject->curve_id == 0) {
        goto exit;
    }

    status = CreateCurveIfNeeded(
        &keyObject->keyStore->session->s_ctx, keyObject->curve_id);

    if (status == SM_NOT_OK) {
        goto exit;
    }
    else if (status == SM_ERR_CONDITIONS_OF_USE_NOT_SATISFIED) {
        LOG_W(
            "Allowing SM_ERR_CONDITIONS_OF_USE_NOT_SATISFIED for CreateCurve");
    }
    status = Se05x_API_CheckObjectExists(
        &keyStore->session->s_ctx, keyObject->keyId, &exists);
    ENSURE_OR_GO_EXIT(status == SM_OK);

    if (exists == kSE05x_Result_SUCCESS) {
        /* Check if object is of same curve id */
        status = Se05x_API_EC_CurveGetId(&keyObject->keyStore->session->s_ctx,
            keyObject->keyId,
            &retCurveId);
        ENSURE_OR_GO_EXIT(status == SM_OK);

        if (retCurveId == keyObject->curve_id) {
            curveId = kSE05x_ECCurve_NA;
        }
        else {
            LOG_W("Cannot overwrite object with different curve id");
            goto exit;
        }

        if (se05x_policy.value_len != 0) {
            LOG_W("Policy + Existing Key is not a valid combination");
        }
    }
    else {
        curveId = keyObject->curve_id;
    }

    if (keyObject->objectType == kSSS_KeyPart_Pair) {
        const uint8_t *pPrivateKey = NULL;
        const uint8_t *pPublicKey = NULL;
        size_t privateKeyLen = 0;
        size_t publicKeyLen = 0;
        uint16_t privateKeyIndex = 0;
        uint16_t publicKeyIndex = 0;
        if (exists == kSE05x_Result_FAILURE)
            key_part = kSE05x_KeyPart_Pair;

        switch (keyObject->curve_id) {
        case kSE05x_ECCurve_TPM_ECC_BN_P256:
        case kSE05x_ECCurve_RESERVED_ID_ECC_ED_25519:
        case kSE05x_ECCurve_RESERVED_ID_ECC_MONT_DH_25519: {
            /* No header included in ED, MONT and BN curve keys */
            privateKeyIndex = 0;
            publicKeyIndex = 32;
            privateKeyLen = 32;
            publicKeyLen = 32;
        } break;
        default: {
            asn_retval = sss_util_pkcs8_asn1_get_ec_pair_key_index(key,
                keyLen,
                &publicKeyIndex,
                &publicKeyLen,
                &privateKeyIndex,
                &privateKeyLen);
            if (asn_retval != kStatus_SSS_Success) {
                LOG_W("error in sss_util_pkcs8_asn1_get_ec_pair_key_index");
                goto exit;
            }
        }
        }

        pPrivateKey = &key[privateKeyIndex];
        pPublicKey = &key[publicKeyIndex];

        status = Se05x_API_WriteECKey(&keyStore->session->s_ctx,
            &se05x_policy,
            SE05x_MaxAttemps_UNLIMITED,
            keyObject->keyId,
            curveId,
            pPrivateKey,
            privateKeyLen,
            pPublicKey,
            publicKeyLen,
            transient_type,
            key_part);
        ENSURE_OR_GO_EXIT(status == SM_OK);
    }
    else if (keyObject->objectType == kSSS_KeyPart_Public) {
        const uint8_t *pPublicKey = NULL;
        size_t publicKeyLen = 0;
        uint16_t publicKeyIndex = 0;
        if (exists == kSE05x_Result_FAILURE)
            key_part = kSE05x_KeyPart_Public;

        switch (keyObject->curve_id) {
        case kSE05x_ECCurve_TPM_ECC_BN_P256:
        case kSE05x_ECCurve_RESERVED_ID_ECC_ED_25519:
        case kSE05x_ECCurve_RESERVED_ID_ECC_MONT_DH_25519:
            publicKeyLen = keyLen;
            break;
        default: {
            asn_retval = sss_util_pkcs8_asn1_get_ec_public_key_index(
                key, keyLen, &publicKeyIndex, &publicKeyLen);
            if (asn_retval != kStatus_SSS_Success) {
                LOG_W("error in sss_util_pkcs8_asn1_get_ec_public_key_index");
                goto exit;
            }
        }
        }

        pPublicKey = &key[publicKeyIndex];

        status = Se05x_API_WriteECKey(&keyStore->session->s_ctx,
            &se05x_policy,
            SE05x_MaxAttemps_NA,
            keyObject->keyId,
            curveId,
            NULL,
            0,
            pPublicKey,
            publicKeyLen,
            transient_type,
            key_part);

        ENSURE_OR_GO_EXIT(status == SM_OK);
    }
    else if (keyObject->objectType == kSSS_KeyPart_Private) {
        const uint8_t *pPrivKey = NULL;
        size_t privKeyLen = 0;
        if (exists == kSE05x_Result_FAILURE)
            key_part = kSE05x_KeyPart_Private;

        pPrivKey = &key[0];
        privKeyLen = (uint16_t)keyLen;

        status = Se05x_API_WriteECKey(&keyStore->session->s_ctx,
            &se05x_policy,
            SE05x_MaxAttemps_NA,
            keyObject->keyId,
            curveId,
            pPrivKey,
            privKeyLen,
            NULL,
            0,
            transient_type,
            key_part);
        ENSURE_OR_GO_EXIT(status == SM_OK);
    }
    else {
        goto exit;
    }

    retval = kStatus_SSS_Success;
exit:
    return retval;
}
#endif // SSSFTR_SE05X_ECC && SSSFTR_SE05X_KEY_SET

#if SSSFTR_SE05X_AES && SSSFTR_SE05X_KEY_SET
static sss_status_t sss_se05x_key_store_set_aes_key(
    sss_se05x_key_store_t *keyStore,
    sss_se05x_object_t *keyObject,
    const uint8_t *key,
    size_t keyLen,
    size_t keyBitLen,
    void *policy_buff,
    size_t policy_buff_len)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;
    Se05xPolicy_t se05x_policy;
    SE05x_TransientType_t transient_type;
    SE05x_SymmKeyType_t type = 0;
    SE05x_KeyID_t kekID = SE05x_KeyID_KEK_NONE;

    (keyObject->isPersistant) ?
        (transient_type = kSE05x_TransientType_Persistent) :
        (transient_type = kSE05x_TransientType_Transient);

    se05x_policy.value = (uint8_t *)policy_buff;
    se05x_policy.value_len = policy_buff_len;

    if (keyBitLen % 8 == 0) {
        if (keyObject->cipherType == kSSS_CipherType_AES) {
            type = kSE05x_SymmKeyType_AES;
        }
        else if (keyObject->cipherType == kSSS_CipherType_CMAC) {
            type = kSE05x_SymmKeyType_CMAC;
        }
        else if (keyObject->cipherType == kSSS_CipherType_HMAC) {
            type = kSE05x_SymmKeyType_HMAC;
        }

        if (keyStore->kekKey != NULL) {
            kekID = keyStore->kekKey->keyId;
        }
        status = Se05x_API_WriteSymmKey(&keyStore->session->s_ctx,
            &se05x_policy,
            SE05x_MaxAttemps_NA,
            keyObject->keyId,
            kekID,
            key,
            keyLen,
            transient_type,
            type);
        ENSURE_OR_GO_EXIT(status == SM_OK);
    }
    else {
        goto exit;
    }
    retval = kStatus_SSS_Success;
exit:
    return retval;
}
#endif // SSSFTR_SE05X_AES && SSSFTR_SE05X_KEY_SET

#if SSSFTR_SE05X_KEY_SET
static sss_status_t sss_se05x_key_store_set_des_key(
    sss_se05x_key_store_t *keyStore,
    sss_se05x_object_t *keyObject,
    const uint8_t *key,
    size_t keyLen,
    size_t keyBitLen,
    void *policy_buff,
    size_t policy_buff_len)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;
    Se05xPolicy_t se05x_policy;
    SE05x_TransientType_t transient_type;
    SE05x_KeyID_t kekID = SE05x_KeyID_KEK_NONE;

    (keyObject->isPersistant) ?
        (transient_type = kSE05x_TransientType_Persistent) :
        (transient_type = kSE05x_TransientType_Transient);

    se05x_policy.value = (uint8_t *)policy_buff;
    se05x_policy.value_len = policy_buff_len;

    if (keyStore->kekKey != NULL) {
        kekID = keyStore->kekKey->keyId;
    }

    status = Se05x_API_WriteSymmKey(&keyStore->session->s_ctx,
        &se05x_policy,
        SE05x_MaxAttemps_NA,
        keyObject->keyId,
        kekID,
        key,
        keyLen,
        transient_type,
        kSE05x_SymmKeyType_DES);
    ENSURE_OR_GO_EXIT(status == SM_OK);

    retval = kStatus_SSS_Success;
exit:
    return retval;
}
#endif // SSSFTR_SE05X_KEY_SET

#if 0
static sss_status_t sss_se05x_key_store_set_deswrapped_key(
    sss_se05x_key_store_t *keyStore,
    sss_se05x_object_t *keyObject,
    const uint8_t *key,
    size_t keyLen,
    size_t keyBitLen,
    void *policy_buff,
    size_t policy_buff_len)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;
    Se05xPolicy_t se05x_policy;

    se05x_policy.value = (uint8_t *)policy_buff;
    se05x_policy.value_len = policy_buff_len;

    if (keyObject->isPersistant) {
        status = Se05x_API_DES_SetNewWrapped_P(&keyStore->session->s_ctx,
            &se05x_policy,
            keyObject->keyId,
            keyObject->kekId,
            (U16)keyBitLen,
            key,
            keyLen);
    }
    else {
        status = Se05x_API_DES_SetNewWrapped_T(&keyStore->session->s_ctx,
            &se05x_policy,
            keyObject->keyId,
            keyObject->kekId,
            (U16)keyBitLen,
            key,
            keyLen);
    }
    ENSURE_OR_GO_EXIT(status == SM_OK);

    retval = kStatus_SSS_Success;
exit:
    return retval;
}

#endif

#if SSSFTR_SE05X_KEY_SET
static sss_status_t sss_se05x_key_store_set_cert(
    sss_se05x_key_store_t *keyStore,
    sss_se05x_object_t *keyObject,
    const uint8_t *key,
    size_t keyLen,
    size_t keyBitLen,
    void *policy_buff,
    size_t policy_buff_len)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;
    Se05xPolicy_t se05x_policy;
    uint16_t data_rem;
    uint16_t offset = 0;
    uint16_t fileSize = 0;
    uint8_t IdExists = 0;

    ENSURE_OR_GO_EXIT(keyLen < 0xFFFFu);

    IdExists = CheckIfKeyIdExists(keyObject->keyId, &keyStore->session->s_ctx);
    fileSize = (IdExists == 1) ? 0 : (uint16_t)keyLen;
    data_rem = (uint16_t)keyLen;

    se05x_policy.value = (uint8_t *)policy_buff;
    se05x_policy.value_len = policy_buff_len;

    while (data_rem > 0) {
        uint16_t chunk =
            (data_rem > BINARY_WRITE_MAX_LEN) ? BINARY_WRITE_MAX_LEN : data_rem;
        data_rem = data_rem - chunk;

        status = Se05x_API_WriteBinary(&keyStore->session->s_ctx,
            &se05x_policy,
            keyObject->keyId,
            offset,
            (uint16_t)fileSize,
            (key + offset),
            chunk);
        ENSURE_OR_GO_EXIT(status == SM_OK);

        fileSize = 0;
        offset = offset + chunk;
    }

    retval = kStatus_SSS_Success;
exit:
    return retval;
}
#endif // SSSFTR_SE05X_KEY_SET

#if 0
static sss_status_t sss_se05x_key_store_set_pcr(
    sss_se05x_key_store_t *keyStore,
    sss_se05x_object_t *keyObject,
    const uint8_t *key,
    size_t keyLen,
    void *policy_buff,
    size_t policy_buff_len)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;
    Se05xPolicy_t se05x_policy;

    se05x_policy.value = (uint8_t *)policy_buff;
    se05x_policy.value_len = policy_buff_len;

    if (keyObject->cipherType == kSSS_CipherType_PCR) {
        status = Se05x_API_WritePCR(&keyStore->session->s_ctx,
            &se05x_policy,
            keyObject->keyId,
            key,
            keyLen,
            NULL,
            0);
    }
    else if (keyObject->cipherType == kSSS_CipherType_Update_PCR) {
        status = Se05x_API_WritePCR(&keyStore->session->s_ctx,
            &se05x_policy,
            keyObject->keyId,
            NULL,
            0,
            key,
            keyLen
        );
    }
    else if (keyObject->cipherType == kSSS_CipherType_Reset_PCR) {
        status = Se05x_API_WritePCR(&keyStore->session->s_ctx,
            &se05x_policy,
            keyObject->keyId,
            NULL,
            0,
            NULL,
            0);
    }
    else
    {
        goto exit;
    }

    ENSURE_OR_GO_EXIT(status == SM_OK);

    retval = kStatus_SSS_Success;
exit:
    return retval;
}
#endif

sss_status_t sss_se05x_key_store_set_key(sss_se05x_key_store_t *keyStore,
    sss_se05x_object_t *keyObject,
    const uint8_t *key,
    size_t keyLen,
    size_t keyBitLen,
    void *options,
    size_t optionsLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
#if SSSFTR_SE05X_KEY_SET
    uint32_t cipher_type = keyObject->cipherType;
    sss_policy_t *policies = (sss_policy_t *)options;
    uint8_t *ppolicySet;
    size_t valid_policy_buff_len = 0;
    uint8_t policies_buff[MAX_POLICY_BUFFER_SIZE];

    if (policies) {
        if (kStatus_SSS_Success !=
            sss_se05x_create_object_policy_buffer(
                policies, &policies_buff[0], &valid_policy_buff_len)) {
            goto exit;
        }
        ppolicySet = policies_buff;
    }
    else {
        ppolicySet = NULL;
    }

    switch (cipher_type) {
#if SSSFTR_SE05X_RSA
    case kSSS_CipherType_RSA:
    case kSSS_CipherType_RSA_CRT:
        if (kStatus_SSS_Success != sss_se05x_key_store_set_rsa_key(keyStore,
                                       keyObject,
                                       key,
                                       keyLen,
                                       keyBitLen,
                                       ppolicySet,
                                       valid_policy_buff_len)) {
            goto exit;
        }
        break;
#endif
#if SSSFTR_SE05X_ECC
    case kSSS_CipherType_EC_NIST_P:
    case kSSS_CipherType_EC_NIST_K:
    case kSSS_CipherType_EC_BRAINPOOL:
    case kSSS_CipherType_EC_MONTGOMERY:
    case kSSS_CipherType_EC_TWISTED_ED:
    case kSSS_CipherType_EC_BARRETO_NAEHRIG:
        if (kStatus_SSS_Success != sss_se05x_key_store_set_ecc_key(keyStore,
                                       keyObject,
                                       key,
                                       keyLen,
                                       keyBitLen,
                                       ppolicySet,
                                       valid_policy_buff_len)) {
            goto exit;
        }
        break;
#endif // SSSFTR_SE05X_ECC
    case kSSS_CipherType_AES:
        if ((keyLen != 16 && keyLen != 24 && keyLen != 32 && keyLen != 40)) {
            goto exit;
        }
        /* fall through */
    case kSSS_CipherType_CMAC:
    case kSSS_CipherType_HMAC:
        if (kStatus_SSS_Success != sss_se05x_key_store_set_aes_key(keyStore,
                                       keyObject,
                                       key,
                                       keyLen,
                                       keyBitLen,
                                       ppolicySet,
                                       valid_policy_buff_len)) {
            goto exit;
        }
        break;
    case kSSS_CipherType_DES:
        if (kStatus_SSS_Success != sss_se05x_key_store_set_des_key(keyStore,
                                       keyObject,
                                       key,
                                       keyLen,
                                       keyBitLen,
                                       ppolicySet,
                                       valid_policy_buff_len)) {
            goto exit;
        }
        break;
    case kSSS_CipherType_Binary:
        if (kStatus_SSS_Success != sss_se05x_key_store_set_cert(keyStore,
                                       keyObject,
                                       key,
                                       keyLen,
                                       keyBitLen,
                                       ppolicySet,
                                       valid_policy_buff_len)) {
            goto exit;
        }
        break;
    default:
        goto exit;
    }
    retval = kStatus_SSS_Success;
exit:
#endif /* SSSFTR_SE05X_KEY_SET */
    return retval;
}

sss_status_t sss_se05x_key_store_generate_key(sss_se05x_key_store_t *keyStore,
    sss_se05x_object_t *keyObject,
    size_t keyBitLen,
    void *options)
{
    sss_status_t retval = kStatus_SSS_Fail;
#if SSSFTR_SE05X_KEY_SET
    smStatus_t status = SM_NOT_OK;
    sss_policy_t *policies = (sss_policy_t *)options;
    uint8_t *ppolicySet;
    size_t valid_policy_buff_len = 0;
    Se05xPolicy_t se05x_policy;
    SE05x_TransientType_t transient_type;
    uint8_t IdExists = 0;
    uint8_t policies_buff[MAX_POLICY_BUFFER_SIZE];

    if (policies) {
        if (kStatus_SSS_Success !=
            sss_se05x_create_object_policy_buffer(
                policies, &policies_buff[0], &valid_policy_buff_len)) {
            goto exit;
        }
        ppolicySet = policies_buff;
    }
    else {
        ppolicySet = NULL;
    }
    se05x_policy.value = (uint8_t *)ppolicySet;
    se05x_policy.value_len = valid_policy_buff_len;

    (keyObject->isPersistant) ?
        (transient_type = kSE05x_TransientType_Persistent) :
        (transient_type = kSE05x_TransientType_Transient);

    ENSURE_OR_GO_EXIT(keyObject->objectType == kSSS_KeyPart_Pair);

#if SSSFTR_SE05X_ECC
    if (keyObject->cipherType == kSSS_CipherType_EC_NIST_P ||
        keyObject->cipherType == kSSS_CipherType_EC_NIST_K ||
        keyObject->cipherType == kSSS_CipherType_EC_BRAINPOOL ||
        keyObject->cipherType == kSSS_CipherType_EC_MONTGOMERY ||
        keyObject->cipherType == kSSS_CipherType_EC_BARRETO_NAEHRIG ||
        keyObject->cipherType == kSSS_CipherType_EC_TWISTED_ED) {
        SE05x_ECCurve_t curve_id;
        if (keyObject->curve_id == 0) {
            keyObject->curve_id =
                se05x_sssKeyTypeLenToCurveId(keyObject->cipherType, keyBitLen);
        }

        if (keyObject->curve_id == 0) {
            goto exit;
        }

        status = CreateCurveIfNeeded(
            &keyObject->keyStore->session->s_ctx, keyObject->curve_id);

        IdExists =
            CheckIfKeyIdExists(keyObject->keyId, &keyStore->session->s_ctx);
        curve_id = (IdExists == 1) ? 0 : keyObject->curve_id;

        status = Se05x_API_WriteECKey(&keyStore->session->s_ctx,
            &se05x_policy,
            SE05x_MaxAttemps_NA,
            keyObject->keyId,
            curve_id,
            NULL,
            0,
            NULL,
            0,
            transient_type,
            kSE05x_KeyPart_Pair);
        ENSURE_OR_GO_EXIT(status == SM_OK);
    }
    else
#endif // < SSSFTR_SE05X_ECC
        if (keyObject->cipherType == kSSS_CipherType_RSA ||
            keyObject->cipherType == kSSS_CipherType_RSA_CRT) {
        /* Hard Coded Public exponent to be 65537 */
        //uint8_t pubexp[] = {0x01, 0x00, 0x01};
        SE05x_KeyPart_t key_part = kSE05x_KeyPart_Pair;
        SE05x_RSAKeyFormat_t rsa_format;
        size_t keyBitLength = 0;
        if (keyObject->cipherType == kSSS_CipherType_RSA)
            rsa_format = kSE05x_RSAKeyFormat_RAW;
        else if (keyObject->cipherType == kSSS_CipherType_RSA_CRT)
            rsa_format = kSE05x_RSAKeyFormat_CRT;
        else {
            retval = kStatus_SSS_Fail;
            goto exit;
        }

        IdExists =
            CheckIfKeyIdExists(keyObject->keyId, &keyStore->session->s_ctx);
        keyBitLength = (IdExists == 1) ? 0 : keyBitLen;

        status = Se05x_API_WriteRSAKey(&keyStore->session->s_ctx,
            &se05x_policy,
            keyObject->keyId,
            (uint16_t)keyBitLength,
            SE05X_RSA_NO_p,
            SE05X_RSA_NO_q,
            SE05X_RSA_NO_dp,
            SE05X_RSA_NO_dq,
            SE05X_RSA_NO_qInv,
            SE05X_RSA_NO_pubExp,
            SE05X_RSA_NO_priv,
            SE05X_RSA_NO_pubMod,
            transient_type,
            key_part,
            rsa_format);

        ENSURE_OR_GO_EXIT(status == SM_OK);
    }
    else {
        goto exit;
    }

    retval = kStatus_SSS_Success;
exit:
#endif // SSSFTR_SE05X_KEY_SET
    return retval;
}

#define ADD_DER_ECC_NISTP192_HEADER(x) ((x) + der_ecc_nistp192_header_len)
#define REMOVE_DER_ECC_NISTP192_HEADER(x) ((x)-der_ecc_nistp192_header_len)

#define ADD_DER_ECC_NISTP224_HEADER(x) ((x) + der_ecc_nistp224_header_len)
#define REMOVE_DER_ECC_NISTP224_HEADER(x) ((x)-der_ecc_nistp224_header_len)

#define ADD_DER_ECC_NISTP256_HEADER(x) ((x) + der_ecc_nistp256_header_len)
#define REMOVE_DER_ECC_NISTP256_HEADER(x) ((x)-der_ecc_nistp256_header_len)

#define ADD_DER_ECC_NISTP384_HEADER(x) ((x) + der_ecc_nistp384_header_len)
#define REMOVE_DER_ECC_NISTP384_HEADER(x) ((x)-der_ecc_nistp384_header_len)

#define ADD_DER_ECC_NISTP521_HEADER(x) ((x) + der_ecc_nistp521_header_len)
#define REMOVE_DER_ECC_NISTP521_HEADER(x) ((x)-der_ecc_nistp521_header_len)

#define ADD_DER_ECC_160K_HEADER(x) ((x) + der_ecc_160k_header_len)
#define REMOVE_DER_ECC_160K_HEADER(x) ((x)-der_ecc_160k_header_len)

#define ADD_DER_ECC_192K_HEADER(x) ((x) + der_ecc_192k_header_len)
#define REMOVE_DER_ECC_192K_HEADER(x) ((x)-der_ecc_192k_header_len)

#define ADD_DER_ECC_224K_HEADER(x) ((x) + der_ecc_224k_header_len)
#define REMOVE_DER_ECC_224K_HEADER(x) ((x)-der_ecc_224k_header_len)

#define ADD_DER_ECC_256K_HEADER(x) ((x) + der_ecc_256k_header_len)
#define REMOVE_DER_ECC_256K_HEADER(x) ((x)-der_ecc_256k_header_len)

#define ADD_DER_ECC_BP160_HEADER(x) ((x) + der_ecc_bp160_header_len)
#define REMOVE_DER_ECC_BP160_HEADER(x) ((x)-der_ecc_bp160_header_len)

#define ADD_DER_ECC_BP192_HEADER(x) ((x) + der_ecc_bp192_header_len)
#define REMOVE_DER_ECC_BP192_HEADER(x) ((x)-der_ecc_bp192_header_len)

#define ADD_DER_ECC_BP224_HEADER(x) ((x) + der_ecc_bp224_header_len)
#define REMOVE_DER_ECC_BP224_HEADER(x) ((x)-der_ecc_bp224_header_len)

#define ADD_DER_ECC_BP320_HEADER(x) ((x) + der_ecc_bp320_header_len)
#define REMOVE_DER_ECC_BP320_HEADER(x) ((x)-der_ecc_bp320_header_len)

#define ADD_DER_ECC_BP384_HEADER(x) ((x) + der_ecc_bp384_header_len)
#define REMOVE_DER_ECC_BP384_HEADER(x) ((x)-der_ecc_bp384_header_len)

#define ADD_DER_ECC_BP256_HEADER(x) ((x) + der_ecc_bp256_header_len)
#define REMOVE_DER_ECC_BP256_HEADER(x) ((x)-der_ecc_bp256_header_len)

#define ADD_DER_ECC_BP512_HEADER(x) ((x) + der_ecc_bp512_header_len)
#define REMOVE_DER_ECC_BP512_HEADER(x) ((x)-der_ecc_bp512_header_len)

#define CONVERT_BYTE(x) ((x) / 8)
#define CONVERT_BIT(x) ((x)*8)

void add_ecc_header(
    uint8_t *key, uint8_t **key_buf, size_t *key_buflen, uint32_t curve_id)
{
#if SSSFTR_SE05X_KEY_SET
    if (curve_id == kSE05x_ECCurve_NIST_P192) {
        memcpy(key, gecc_der_header_nist192, der_ecc_nistp192_header_len);
        *key_buf = ADD_DER_ECC_NISTP192_HEADER(key);
        *key_buflen = (uint16_t)ADD_DER_ECC_NISTP192_HEADER(*key_buflen);
    }
    else if (curve_id == kSE05x_ECCurve_NIST_P224) {
        memcpy(key, gecc_der_header_nist224, der_ecc_nistp224_header_len);
        *key_buf = ADD_DER_ECC_NISTP224_HEADER(key);
        *key_buflen = (uint16_t)ADD_DER_ECC_NISTP224_HEADER(*key_buflen);
    }
    else if (curve_id == kSE05x_ECCurve_NIST_P256) {
        memcpy(key, gecc_der_header_nist256, der_ecc_nistp256_header_len);
        *key_buf = ADD_DER_ECC_NISTP256_HEADER(key);
        *key_buflen = (uint16_t)ADD_DER_ECC_NISTP256_HEADER(*key_buflen);
    }
    else if (curve_id == kSE05x_ECCurve_NIST_P384) {
        memcpy(key, gecc_der_header_nist384, der_ecc_nistp384_header_len);
        *key_buf = ADD_DER_ECC_NISTP384_HEADER(key);
        *key_buflen = (uint16_t)ADD_DER_ECC_NISTP384_HEADER(*key_buflen);
    }
    else if (curve_id == kSE05x_ECCurve_NIST_P521) {
        memcpy(key, gecc_der_header_nist521, der_ecc_nistp521_header_len);
        *key_buf = ADD_DER_ECC_NISTP521_HEADER(key);
        *key_buflen = (uint16_t)ADD_DER_ECC_NISTP521_HEADER(*key_buflen);
    }
    else if (curve_id == kSE05x_ECCurve_Brainpool160) {
        memcpy(key, gecc_der_header_bp160, der_ecc_bp160_header_len);
        *key_buf = ADD_DER_ECC_BP160_HEADER(key);
        *key_buflen = (uint16_t)ADD_DER_ECC_BP160_HEADER(*key_buflen);
    }
    else if (curve_id == kSE05x_ECCurve_Brainpool192) {
        memcpy(key, gecc_der_header_bp192, der_ecc_bp192_header_len);
        *key_buf = ADD_DER_ECC_BP192_HEADER(key);
        *key_buflen = (uint16_t)ADD_DER_ECC_BP192_HEADER(*key_buflen);
    }
    else if (curve_id == kSE05x_ECCurve_Brainpool224) {
        memcpy(key, gecc_der_header_bp224, der_ecc_bp224_header_len);
        *key_buf = ADD_DER_ECC_BP224_HEADER(key);
        *key_buflen = (uint16_t)ADD_DER_ECC_BP224_HEADER(*key_buflen);
    }
    else if (curve_id == kSE05x_ECCurve_Brainpool320) {
        memcpy(key, gecc_der_header_bp320, der_ecc_bp320_header_len);
        *key_buf = ADD_DER_ECC_BP320_HEADER(key);
        *key_buflen = (uint16_t)ADD_DER_ECC_BP320_HEADER(*key_buflen);
    }
    else if (curve_id == kSE05x_ECCurve_Brainpool384) {
        memcpy(key, gecc_der_header_bp384, der_ecc_bp384_header_len);
        *key_buf = ADD_DER_ECC_BP384_HEADER(key);
        *key_buflen = (uint16_t)ADD_DER_ECC_BP384_HEADER(*key_buflen);
    }
    else if (curve_id == kSE05x_ECCurve_Brainpool256) {
        memcpy(key, gecc_der_header_bp256, der_ecc_bp256_header_len);
        *key_buf = ADD_DER_ECC_BP256_HEADER(key);
        *key_buflen = (uint16_t)ADD_DER_ECC_BP256_HEADER(*key_buflen);
    }
    else if (curve_id == kSE05x_ECCurve_Brainpool512) {
        memcpy(key, gecc_der_header_bp512, der_ecc_bp512_header_len);
        *key_buf = ADD_DER_ECC_BP512_HEADER(key);
        *key_buflen = (uint16_t)ADD_DER_ECC_BP512_HEADER(*key_buflen);
    }
    else if (curve_id == kSE05x_ECCurve_Secp256k1) {
        memcpy(key, gecc_der_header_256k, der_ecc_256k_header_len);
        *key_buf = ADD_DER_ECC_256K_HEADER(key);
        *key_buflen = (uint16_t)ADD_DER_ECC_256K_HEADER(*key_buflen);
    }
    else if (curve_id == kSE05x_ECCurve_Secp160k1) {
        memcpy(key, gecc_der_header_160k, der_ecc_160k_header_len);
        *key_buf = ADD_DER_ECC_160K_HEADER(key);
        *key_buflen = (uint16_t)ADD_DER_ECC_160K_HEADER(*key_buflen);
    }
    else if (curve_id == kSE05x_ECCurve_Secp192k1) {
        memcpy(key, gecc_der_header_192k, der_ecc_192k_header_len);
        *key_buf = ADD_DER_ECC_192K_HEADER(key);
        *key_buflen = (uint16_t)ADD_DER_ECC_192K_HEADER(*key_buflen);
    }
    else if (curve_id == kSE05x_ECCurve_Secp224k1) {
        memcpy(key, gecc_der_header_224k, der_ecc_224k_header_len);
        *key_buf = ADD_DER_ECC_224K_HEADER(key);
        *key_buflen = (uint16_t)ADD_DER_ECC_224K_HEADER(*key_buflen);
    }
    else {
        LOG_W("Returned is not in DER Format");
        *key_buf = key;
        *key_buflen = 0;
    }
#endif
}

void get_ecc_raw_data(
    uint8_t *key, uint8_t **key_buf, size_t *key_buflen, uint32_t curve_id)
{
    if (curve_id == kSE05x_ECCurve_NIST_P192) {
        *key_buf = ADD_DER_ECC_NISTP192_HEADER(key);
        *key_buflen = (uint16_t)REMOVE_DER_ECC_NISTP192_HEADER(*key_buflen);
    }
    else if (curve_id == kSE05x_ECCurve_NIST_P224) {
        *key_buf = ADD_DER_ECC_NISTP224_HEADER(key);
        *key_buflen = (uint16_t)REMOVE_DER_ECC_NISTP224_HEADER(*key_buflen);
    }
    else if (curve_id == kSE05x_ECCurve_NIST_P256) {
        *key_buf = ADD_DER_ECC_NISTP256_HEADER(key);
        *key_buflen = (uint16_t)REMOVE_DER_ECC_NISTP256_HEADER(*key_buflen);
    }
    else if (curve_id == kSE05x_ECCurve_NIST_P384) {
        *key_buf = ADD_DER_ECC_NISTP384_HEADER(key);
        *key_buflen = (uint16_t)REMOVE_DER_ECC_NISTP384_HEADER(*key_buflen);
    }
    else if (curve_id == kSE05x_ECCurve_NIST_P521) {
        *key_buf = ADD_DER_ECC_NISTP521_HEADER(key);
        *key_buflen = (uint16_t)REMOVE_DER_ECC_NISTP521_HEADER(*key_buflen);
    }
    else if (curve_id == kSE05x_ECCurve_Brainpool160) {
        *key_buf = ADD_DER_ECC_BP160_HEADER(key);
        *key_buflen = (uint16_t)REMOVE_DER_ECC_BP160_HEADER(*key_buflen);
    }
    else if (curve_id == kSE05x_ECCurve_Brainpool192) {
        *key_buf = ADD_DER_ECC_BP192_HEADER(key);
        *key_buflen = (uint16_t)REMOVE_DER_ECC_BP192_HEADER(*key_buflen);
    }
    else if (curve_id == kSE05x_ECCurve_Brainpool224) {
        *key_buf = REMOVE_DER_ECC_BP224_HEADER(key);
        *key_buflen = (uint16_t)REMOVE_DER_ECC_BP224_HEADER(*key_buflen);
    }
    else if (curve_id == kSE05x_ECCurve_Brainpool320) {
        *key_buf = ADD_DER_ECC_BP320_HEADER(key);
        *key_buflen = (uint16_t)REMOVE_DER_ECC_BP320_HEADER(*key_buflen);
    }
    else if (curve_id == kSE05x_ECCurve_Brainpool384) {
        *key_buf = ADD_DER_ECC_BP384_HEADER(key);
        *key_buflen = (uint16_t)REMOVE_DER_ECC_BP384_HEADER(*key_buflen);
    }
    else if (curve_id == kSE05x_ECCurve_Brainpool256) {
        *key_buf = ADD_DER_ECC_BP256_HEADER(key);
        *key_buflen = (uint16_t)REMOVE_DER_ECC_BP256_HEADER(*key_buflen);
    }
    else if (curve_id == kSE05x_ECCurve_Brainpool512) {
        *key_buf = ADD_DER_ECC_BP512_HEADER(key);
        *key_buflen = (uint16_t)REMOVE_DER_ECC_BP512_HEADER(*key_buflen);
    }
    else if (curve_id == kSE05x_ECCurve_Secp256k1) {
        *key_buf = ADD_DER_ECC_256K_HEADER(key);
        *key_buflen = (uint16_t)REMOVE_DER_ECC_256K_HEADER(*key_buflen);
    }
    else if (curve_id == kSE05x_ECCurve_Secp160k1) {
        *key_buf = ADD_DER_ECC_160K_HEADER(key);
        *key_buflen = (uint16_t)REMOVE_DER_ECC_160K_HEADER(*key_buflen);
    }
    else if (curve_id == kSE05x_ECCurve_Secp192k1) {
        *key_buf = ADD_DER_ECC_192K_HEADER(key);
        *key_buflen = (uint16_t)REMOVE_DER_ECC_192K_HEADER(*key_buflen);
    }
    else if (curve_id == kSE05x_ECCurve_Secp224k1) {
        *key_buf = ADD_DER_ECC_224K_HEADER(key);
        *key_buflen = (uint16_t)REMOVE_DER_ECC_224K_HEADER(*key_buflen);
    }
    else {
        LOG_W("Returned is not in DER Format");
        *key_buf = key;
        *key_buflen = 0;
    }
}

sss_status_t sss_se05x_key_store_get_key(sss_se05x_key_store_t *keyStore,
    sss_se05x_object_t *keyObject,
    uint8_t *key,
    size_t *keylen,
    size_t *pKeyBitLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    sss_cipher_type_t cipher_type = keyObject->cipherType;
    smStatus_t status = SM_NOT_OK;
    uint16_t size;

    switch (cipher_type) {
    case kSSS_CipherType_EC_NIST_P:
    case kSSS_CipherType_EC_NIST_K:
    case kSSS_CipherType_EC_BRAINPOOL:
    case kSSS_CipherType_EC_BARRETO_NAEHRIG:
    case kSSS_CipherType_EC_MONTGOMERY:
    case kSSS_CipherType_EC_TWISTED_ED: {
        uint8_t *key_buf = NULL;
        size_t key_buflen = 0;

        /* Return the Key length including the ECC DER Header */
        add_ecc_header(key, &key_buf, &key_buflen, keyObject->curve_id);

        status = Se05x_API_ReadObject(
            &keyStore->session->s_ctx, keyObject->keyId, 0, 0, key_buf, keylen);
        ENSURE_OR_GO_EXIT(status == SM_OK);

        /* Return the Key length with header length */
        *keylen += key_buflen;
        break;
    }
    case kSSS_CipherType_RSA:
    case kSSS_CipherType_RSA_CRT: {
        uint8_t modulus[1024];
        uint8_t exponent[4];
        size_t modLen = sizeof(modulus);
        size_t expLen = sizeof(exponent);

        status = Se05x_API_ReadRSA(&keyStore->session->s_ctx,
            keyObject->keyId,
            0,
            0,
            kSE05x_RSAPubKeyComp_MOD,
            modulus,
            &modLen);
        ENSURE_OR_GO_EXIT(status == SM_OK);

        status = Se05x_API_ReadRSA(&keyStore->session->s_ctx,
            keyObject->keyId,
            0,
            0,
            kSE05x_RSAPubKeyComp_PUB_EXP,
            exponent,
            &expLen);
        ENSURE_OR_GO_EXIT(status == SM_OK);

        if (sss_util_asn1_rsa_get_public(
                key, keylen, modulus, modLen, exponent, expLen) !=
            kStatus_SSS_Success) {
            goto exit;
        }
    } break;
    case kSSS_CipherType_AES:
        status = Se05x_API_ReadObject(
            &keyStore->session->s_ctx, keyObject->keyId, 0, 0, key, keylen);
        ENSURE_OR_GO_EXIT(status == SM_OK);
        break;
    case kSSS_CipherType_Binary: {
        uint16_t rem_data = 0;
        uint16_t offset = 0;
        size_t max_buffer = 0;
        status = Se05x_API_ReadSize(
            &keyStore->session->s_ctx, keyObject->keyId, &size);
        ENSURE_OR_GO_EXIT(status == SM_OK);
        if (*keylen < size) {
            LOG_E("Insufficient buffer ");
            goto exit;
        }

        rem_data = size;
        *keylen = size;
        while (rem_data > 0) {
            uint16_t chunk = (rem_data > BINARY_WRITE_MAX_LEN) ?
                                 BINARY_WRITE_MAX_LEN :
                                 rem_data;
            rem_data = rem_data - chunk;
            max_buffer = chunk;
            status = Se05x_API_ReadObject(&keyStore->session->s_ctx,
                keyObject->keyId,
                offset,
                chunk,
                (key + offset),
                &max_buffer);
            ENSURE_OR_GO_EXIT(status == SM_OK);
            offset = offset + chunk;
        }

    } break;
    case kSSS_CipherType_DES:
        status = Se05x_API_ReadObject(
            &keyStore->session->s_ctx, keyObject->keyId, 0, 0, key, keylen);
        ENSURE_OR_GO_EXIT(status == SM_OK);
        break;
    case kSSS_CipherType_PCR:
        status = Se05x_API_ReadObject(
            &keyStore->session->s_ctx, keyObject->keyId, 0, 0, key, keylen);
        ENSURE_OR_GO_EXIT(status == SM_OK);
        break;
    case kSSS_CipherType_Count:
        status = Se05x_API_ReadObject(
            &keyStore->session->s_ctx, keyObject->keyId, 0, 0, key, keylen);
        ENSURE_OR_GO_EXIT(status == SM_OK);
        break;
    default:
        goto exit;
    }

    retval = kStatus_SSS_Success;
exit:
    return retval;
}

sss_status_t sss_se05x_key_store_get_key_attst(sss_se05x_key_store_t *keyStore,
    sss_se05x_object_t *keyObject,
    uint8_t *key,
    size_t *keylen,
    size_t *pKeyBitLen,
    sss_se05x_object_t *keyObject_attst,
    sss_algorithm_t algorithm_attst,
    uint8_t *random_attst,
    size_t randomLen_attst,
    sss_se05x_attst_data_t *attst_data)
{
    sss_status_t retval = kStatus_SSS_Fail;
    sss_cipher_type_t cipher_type = keyObject->cipherType;
    smStatus_t status = SM_NOT_OK;
    uint16_t size;

    uint32_t attestID;
    SE05x_AttestationAlgo_t attestAlgo;

    attestID = keyObject_attst->keyId;

    switch (keyObject_attst->cipherType) {
    case kSSS_CipherType_EC_NIST_P:
    case kSSS_CipherType_EC_NIST_K:
    case kSSS_CipherType_EC_BRAINPOOL: {
        SE05x_ECSignatureAlgo_t ecSignAlgo =
            se05x_get_ec_sign_hash_mode(algorithm_attst);
        attestAlgo = (SE05x_AttestationAlgo_t)ecSignAlgo;
    } break;
    case kSSS_CipherType_RSA:
    case kSSS_CipherType_RSA_CRT: {
        SE05x_RSASignatureAlgo_t rsaSigningAlgo =
            se05x_get_rsa_sign_hash_mode(algorithm_attst);
        attestAlgo = (SE05x_AttestationAlgo_t)rsaSigningAlgo;
    } break;
    default:
        goto exit;
    }

    switch (cipher_type) {
    case kSSS_CipherType_EC_NIST_P:
    case kSSS_CipherType_EC_NIST_K:
    case kSSS_CipherType_EC_BRAINPOOL:
    case kSSS_CipherType_EC_BARRETO_NAEHRIG:
    case kSSS_CipherType_EC_MONTGOMERY:
    case kSSS_CipherType_EC_TWISTED_ED: {
        uint8_t *key_buf = NULL;
        size_t key_buflen = 0;

        /* Return the Key length including the ECC DER Header */
        add_ecc_header(key, &key_buf, &key_buflen, keyObject->curve_id);

        attst_data->data[0].timeStampLen = sizeof(SE05x_TimeStamp_t);
        status = Se05x_API_ReadObject_W_Attst(&keyStore->session->s_ctx,
            keyObject->keyId,
            0,
            0,
            attestID,
            attestAlgo,
            random_attst,
            randomLen_attst,
            key_buf,
            keylen,
            attst_data->data[0].attribute,
            &(attst_data->data[0].attributeLen),
            &(attst_data->data[0].timeStamp),
            attst_data->data[0].outrandom,
            &(attst_data->data[0].outrandomLen),
            attst_data->data[0].chipId,
            &(attst_data->data[0].chipIdLen),
            attst_data->data[0].signature,
            &(attst_data->data[0].signatureLen));
        ENSURE_OR_GO_EXIT(status == SM_OK);

        attst_data->valid_number = 1;
        /* Return the Key length with header length */
        *keylen += key_buflen;

        break;
    }
    case kSSS_CipherType_RSA:
    case kSSS_CipherType_RSA_CRT: {
        uint8_t modulus[1024];
        uint8_t exponent[4];
        size_t modLen = sizeof(modulus);
        size_t expLen = sizeof(exponent);

        attst_data->data[0].timeStampLen = sizeof(SE05x_TimeStamp_t);
        status = Se05x_API_ReadRSA_W_Attst(&keyStore->session->s_ctx,
            keyObject->keyId,
            0,
            0,
            kSE05x_RSAPubKeyComp_MOD,
            attestID,
            attestAlgo,
            random_attst,
            randomLen_attst,
            modulus,
            &modLen,
            attst_data->data[0].attribute,
            &(attst_data->data[0].attributeLen),
            &(attst_data->data[0].timeStamp),
            attst_data->data[0].outrandom,
            &(attst_data->data[0].outrandomLen),
            attst_data->data[0].chipId,
            &(attst_data->data[0].chipIdLen),
            attst_data->data[0].signature,
            &(attst_data->data[0].signatureLen));
        ENSURE_OR_GO_EXIT(status == SM_OK);

        attst_data->data[1].timeStampLen = sizeof(SE05x_TimeStamp_t);
        status = Se05x_API_ReadRSA_W_Attst(&keyStore->session->s_ctx,
            keyObject->keyId,
            0,
            0,
            kSE05x_RSAPubKeyComp_PUB_EXP,
            attestID,
            attestAlgo,
            random_attst,
            randomLen_attst,
            exponent,
            &expLen,
            attst_data->data[1].attribute,
            &(attst_data->data[1].attributeLen),
            &(attst_data->data[1].timeStamp),
            attst_data->data[1].outrandom,
            &(attst_data->data[1].outrandomLen),
            attst_data->data[1].chipId,
            &(attst_data->data[1].chipIdLen),
            attst_data->data[1].signature,
            &(attst_data->data[1].signatureLen));

        attst_data->valid_number = 2;

        ENSURE_OR_GO_EXIT(status == SM_OK);

        if (sss_util_asn1_rsa_get_public(
                key, keylen, modulus, modLen, exponent, expLen) !=
            kStatus_SSS_Success) {
            goto exit;
        }
    } break;
    case kSSS_CipherType_AES:
        attst_data->data[0].timeStampLen = sizeof(SE05x_TimeStamp_t);
        status = Se05x_API_ReadObject_W_Attst(&keyStore->session->s_ctx,
            keyObject->keyId,
            0,
            0,
            attestID,
            attestAlgo,
            random_attst,
            randomLen_attst,
            key,
            keylen,
            attst_data->data[0].attribute,
            &(attst_data->data[0].attributeLen),
            &(attst_data->data[0].timeStamp),
            attst_data->data[0].outrandom,
            &(attst_data->data[0].outrandomLen),
            attst_data->data[0].chipId,
            &(attst_data->data[0].chipIdLen),
            attst_data->data[0].signature,
            &(attst_data->data[0].signatureLen));

        attst_data->valid_number = 1;

        ENSURE_OR_GO_EXIT(status == SM_OK);
        break;
    case kSSS_CipherType_Binary: {
        uint16_t rem_data = 0;
        uint16_t offset = 0;
        size_t max_buffer = 0;
        status = Se05x_API_ReadSize(
            &keyStore->session->s_ctx, keyObject->keyId, &size);
        ENSURE_OR_GO_EXIT(status == SM_OK);

        if (*keylen < size) {
            LOG_E("Insufficient buffer ");
            goto exit;
        }

        rem_data = size;
        *keylen = size;
        while (rem_data > 0) {
            uint16_t chunk = (rem_data > BINARY_WRITE_MAX_LEN) ?
                                 BINARY_WRITE_MAX_LEN :
                                 rem_data;
            rem_data = rem_data - chunk;
            max_buffer = chunk;

            attst_data->data[0].timeStampLen = sizeof(SE05x_TimeStamp_t);
            status = Se05x_API_ReadObject_W_Attst(&keyStore->session->s_ctx,
                keyObject->keyId,
                offset,
                chunk,
                attestID,
                attestAlgo,
                random_attst,
                randomLen_attst,
                (key + offset),
                &max_buffer,
                attst_data->data[0].attribute,
                &(attst_data->data[0].attributeLen),
                &(attst_data->data[0].timeStamp),
                attst_data->data[0].outrandom,
                &(attst_data->data[0].outrandomLen),
                attst_data->data[0].chipId,
                &(attst_data->data[0].chipIdLen),
                attst_data->data[0].signature,
                &(attst_data->data[0].signatureLen));

            attst_data->valid_number = 1;

            ENSURE_OR_GO_EXIT(status == SM_OK);

            offset = offset + chunk;
        }
    } break;
    case kSSS_CipherType_DES:
        attst_data->data[0].timeStampLen = sizeof(SE05x_TimeStamp_t);
        status = Se05x_API_ReadObject_W_Attst(&keyStore->session->s_ctx,
            keyObject->keyId,
            0,
            0,
            attestID,
            attestAlgo,
            random_attst,
            randomLen_attst,
            key,
            keylen,
            attst_data->data[0].attribute,
            &(attst_data->data[0].attributeLen),
            &(attst_data->data[0].timeStamp),
            attst_data->data[0].outrandom,
            &(attst_data->data[0].outrandomLen),
            attst_data->data[0].chipId,
            &(attst_data->data[0].chipIdLen),
            attst_data->data[0].signature,
            &(attst_data->data[0].signatureLen));

        attst_data->valid_number = 1;

        ENSURE_OR_GO_EXIT(status == SM_OK);
        break;

    case kSSS_CipherType_PCR:
        attst_data->data[0].timeStampLen = sizeof(SE05x_TimeStamp_t);
        status = Se05x_API_ReadObject_W_Attst(&keyStore->session->s_ctx,
            keyObject->keyId,
            0,
            0,
            attestID,
            attestAlgo,
            random_attst,
            randomLen_attst,
            key,
            keylen,
            attst_data->data[0].attribute,
            &(attst_data->data[0].attributeLen),
            &(attst_data->data[0].timeStamp),
            attst_data->data[0].outrandom,
            &(attst_data->data[0].outrandomLen),
            attst_data->data[0].chipId,
            &(attst_data->data[0].chipIdLen),
            attst_data->data[0].signature,
            &(attst_data->data[0].signatureLen));

        attst_data->valid_number = 1;

        ENSURE_OR_GO_EXIT(status == SM_OK);
        break;

    case kSSS_CipherType_Count:
        attst_data->data[0].timeStampLen = sizeof(SE05x_TimeStamp_t);
        status = Se05x_API_ReadObject_W_Attst(&keyStore->session->s_ctx,
            keyObject->keyId,
            0,
            0,
            attestID,
            attestAlgo,
            random_attst,
            randomLen_attst,
            key,
            keylen,
            attst_data->data[0].attribute,
            &(attst_data->data[0].attributeLen),
            &(attst_data->data[0].timeStamp),
            attst_data->data[0].outrandom,
            &(attst_data->data[0].outrandomLen),
            attst_data->data[0].chipId,
            &(attst_data->data[0].chipIdLen),
            attst_data->data[0].signature,
            &(attst_data->data[0].signatureLen));

        attst_data->valid_number = 1;

        ENSURE_OR_GO_EXIT(status == SM_OK);
        break;

    case kSSS_CipherType_HMAC: {
        attst_data->data[0].timeStampLen = sizeof(SE05x_TimeStamp_t);
        status = Se05x_API_ReadObject_W_Attst(&keyStore->session->s_ctx,
            keyObject->keyId,
            0,
            0,
            attestID,
            attestAlgo,
            random_attst,
            randomLen_attst,
            key,
            keylen,
            attst_data->data[0].attribute,
            &(attst_data->data[0].attributeLen),
            &(attst_data->data[0].timeStamp),
            attst_data->data[0].outrandom,
            &(attst_data->data[0].outrandomLen),
            attst_data->data[0].chipId,
            &(attst_data->data[0].chipIdLen),
            attst_data->data[0].signature,
            &(attst_data->data[0].signatureLen));

        attst_data->valid_number = 1;

        ENSURE_OR_GO_EXIT(status == SM_OK);
        break;
    }
    default:
        goto exit;
    }

    retval = kStatus_SSS_Success;
exit:
    return retval;
}

#if 0
/* To be reviewed: Purnank */
sss_status_t sss_se05x_key_store_get_key_fromoffset(sss_se05x_key_store_t *keyStore,
    sss_se05x_object_t *keyObject,
    uint8_t *key,
    size_t *keylen,
    size_t *pKeyBitLen,
    uint16_t offset)
{
    sss_status_t retval = kStatus_SSS_Fail;
    sss_key_type_t key_type = keyObject->objectType;
    smStatus_t status = SM_NOT_OK;

    switch (key_type) {
    case kSSS_KeyType_Certificate:
        status =
            Se05x_API_FIL_BinaryReadFromOffset(&keyStore->session->s_ctx,
                keyObject->keyId,
                (uint16_t)*keylen,
                offset,
                key,
                keylen);
        if (status != SM_OK)
            goto exit;
        break;
    default:
        goto exit;
    }

    retval = kStatus_SSS_Success;
exit:
    return retval;
}
#endif
sss_status_t sss_se05x_key_store_open_key(
    sss_se05x_key_store_t *keyStore, sss_se05x_object_t *keyObject)
{
    sss_status_t retval = kStatus_SSS_Fail;

    if (NULL == keyObject) {
        keyStore->kekKey = NULL;
        retval = kStatus_SSS_Success;
    }
    else if (keyObject->keyStore == keyStore) {
        keyStore->kekKey = keyObject;
        retval = kStatus_SSS_Success;
    }
    else {
        LOG_W("KeyObject must be of same KeyStore.");
    }

    return retval;
}

sss_status_t sss_se05x_key_store_freeze_key(
    sss_se05x_key_store_t *keyStore, sss_se05x_object_t *keyObject)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* Purpose / Policy is set during creation time and hence can not
     * enforced in SE050 later on */
    return retval;
}

sss_status_t sss_se05x_key_store_erase_key(
    sss_se05x_key_store_t *keyStore, sss_se05x_object_t *keyObject)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status;

    status = Se05x_API_DeleteSecureObject(
        &keyStore->session->s_ctx, keyObject->keyId);
    if (SM_OK == status) {
        LOG_D("Erased Key id %X", keyObject->keyId);
        retval = kStatus_SSS_Success;
    }
    else {
        LOG_W("Could not delete Key id %X", keyObject->keyId);
    }

    return retval;
}

sss_status_t sss_se05x_key_store_prop_get_u32(
    sss_se05x_key_store_t *session, uint32_t property, uint32_t *pValue)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* Key store based properties would be added later in future.
     * Hence returning failure as of now */
    return retval;
}

sss_status_t sss_se05x_key_store_prop_get_au8(sss_se05x_key_store_t *session,
    uint32_t property,
    uint8_t *pValue,
    size_t *pValueLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    /* Key store based properties would be added later in future.
     * Hence returning failure as of now */
    return retval;
}

void sss_se05x_key_store_context_free(sss_se05x_key_store_t *keyStore)
{
    memset(keyStore, 0, sizeof(*keyStore));
}

#if SSSFTR_SE05X_KEY_SET && SSSFTR_SE05X_ECC
sss_status_t sss_se05x_key_store_create_curve(
    Se05xSession_t *pSession, uint32_t curve_id)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;

    status = CreateCurveIfNeeded(pSession, curve_id);

    if (SM_OK == status)
        retval = kStatus_SSS_Success;

    return retval;
}
#endif // SSSFTR_SE05X_KEY_SET

sss_status_t sss_se05x_key_store_check_curve(
    Se05xSession_t *pSession, uint32_t curve_id)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;
    uint8_t curveList[32] = {
        0,
    };
    size_t curveListLen = 32;

    if (curve_id == kSE05x_ECCurve_RESERVED_ID_ECC_ED_25519 ||
        curve_id == kSE05x_ECCurve_RESERVED_ID_ECC_MONT_DH_25519) {
        return kStatus_SSS_Success;
        /* ECC_ED_25519 and ECC_MONT_DH_25519 are always present */
    }

    status = Se05x_API_ReadECCurveList(pSession, curveList, &curveListLen);
    if (status == SM_OK) {
        if (curveList[curve_id - 1] == kSE05x_SetIndicator_SET)
            retval = kStatus_SSS_Success;
    }

    return retval;
}

sss_status_t sss_se05x_key_store_delete_curve(
    Se05xSession_t *pSession, uint32_t curve_id)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;

    status = Se05x_API_DeleteECCurve(pSession, curve_id);

    if (SM_OK == status)
        retval = kStatus_SSS_Success;
    return retval;
}

sss_status_t sss_se05x_key_store_export_key(sss_se05x_key_store_t *keyStore,
    sss_se05x_object_t *keyObject,
    uint8_t *key,
    size_t *keylen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    sss_cipher_type_t cipher_type = keyObject->cipherType;
    smStatus_t status = SM_NOT_OK;

    switch (cipher_type) {
    case kSSS_CipherType_EC_NIST_P:
    case kSSS_CipherType_EC_NIST_K:
    case kSSS_CipherType_EC_BRAINPOOL:
    case kSSS_CipherType_EC_BARRETO_NAEHRIG:
    case kSSS_CipherType_EC_MONTGOMERY:
    case kSSS_CipherType_EC_TWISTED_ED:
    case kSSS_CipherType_AES:
    case kSSS_CipherType_DES: {
        status = Se05x_API_ExportObject(&keyStore->session->s_ctx,
            keyObject->keyId,
            kSE05x_RSAKeyComponent_NA,
            key,
            keylen);
        if (status != SM_OK) {
            goto exit;
        }

        break;
    }

    default:
        goto exit;
    }

    retval = kStatus_SSS_Success;
exit:
    return retval;
}

sss_status_t sss_se05x_key_store_import_key(sss_se05x_key_store_t *keyStore,
    sss_se05x_object_t *keyObject,
    uint8_t *key,
    size_t keylen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    sss_cipher_type_t cipher_type = keyObject->cipherType;
    smStatus_t status = SM_NOT_OK;

    switch (cipher_type) {
    case kSSS_CipherType_EC_NIST_P:
    case kSSS_CipherType_EC_NIST_K:
    case kSSS_CipherType_EC_BRAINPOOL:
    case kSSS_CipherType_EC_BARRETO_NAEHRIG:
    case kSSS_CipherType_EC_MONTGOMERY:
    case kSSS_CipherType_EC_TWISTED_ED:
    case kSSS_CipherType_AES:
    case kSSS_CipherType_DES: {
        status = Se05x_API_ImportObject(&keyStore->session->s_ctx,
            keyObject->keyId,
            kSE05x_RSAKeyComponent_NA,
            key,
            keylen);
        if (status != SM_OK) {
            goto exit;
        }

        break;
    }

    default:
        goto exit;
    }

    retval = kStatus_SSS_Success;
exit:
    return retval;
}

/* End: se05x_keystore */

/* ************************************************************************** */
/* Functions : sss_se05x_asym                                                 */
/* ************************************************************************** */

sss_status_t sss_se05x_asymmetric_context_init(sss_se05x_asymmetric_t *context,
    sss_se05x_session_t *session,
    sss_se05x_object_t *keyObject,
    sss_algorithm_t algorithm,
    sss_mode_t mode)
{
    sss_status_t retval = kStatus_SSS_Success;

    context->session = session;
    context->keyObject = keyObject;
    context->algorithm = algorithm;
    context->mode = mode;

    return retval;
}

sss_status_t sss_se05x_asymmetric_encrypt(sss_se05x_asymmetric_t *context,
    const uint8_t *srcData,
    size_t srcLen,
    uint8_t *destData,
    size_t *destLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;
    SE05x_RSAEncryptionAlgo_t rsaEncryptionAlgo =
        se05x_get_rsa_encrypt_mode(context->algorithm);
    status = Se05x_API_RSAEncrypt(&context->session->s_ctx,
        context->keyObject->keyId,
        rsaEncryptionAlgo,
        srcData,
        srcLen,
        destData,
        destLen);
    if (status == SM_OK)
        retval = kStatus_SSS_Success;
    return retval;
}

sss_status_t sss_se05x_asymmetric_decrypt(sss_se05x_asymmetric_t *context,
    const uint8_t *srcData,
    size_t srcLen,
    uint8_t *destData,
    size_t *destLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;

    SE05x_RSAEncryptionAlgo_t rsaEncryptionAlgo =
        se05x_get_rsa_encrypt_mode(context->algorithm);
    status = Se05x_API_RSADecrypt(&context->session->s_ctx,
        context->keyObject->keyId,
        rsaEncryptionAlgo,
        srcData,
        srcLen,
        destData,
        destLen);
    if (status == SM_OK)
        retval = kStatus_SSS_Success;
    return retval;
}

sss_status_t sss_se05x_asymmetric_sign_digest(sss_se05x_asymmetric_t *context,
    uint8_t *digest,
    size_t digestLen,
    uint8_t *signature,
    size_t *signatureLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;
#if SSS_HAVE_MBEDTLS
    int ret;
#endif

    switch (context->keyObject->cipherType) {
    case kSSS_CipherType_EC_NIST_P:
    case kSSS_CipherType_EC_NIST_K:
    case kSSS_CipherType_EC_BRAINPOOL: {
        SE05x_ECSignatureAlgo_t ecSignAlgo =
            se05x_get_ec_sign_hash_mode(context->algorithm);
        status = Se05x_API_ECDSASign(&context->session->s_ctx,
            context->keyObject->keyId,
            ecSignAlgo,
            digest,
            digestLen,
            signature,
            signatureLen);
    } break;
    case kSSS_CipherType_EC_BARRETO_NAEHRIG: {
        /* clang-format off */
        uint8_t random[32] = {
            0x7A, 0xCB, 0x93, 0x3D, 0xBE, 0x70, 0x39, 0x9B, 0xF6,
            0xC9, 0x2D, 0xA3, 0x3A, 0xF0, 0x1D, 0x4F, 0xB7, 0x70,
            0xE9, 0x8C, 0x03, 0x25, 0xF4, 0x1D, 0x3E, 0xBA, 0xF8,
            0x98, 0x6D, 0xA7, 0x12, 0xCA
        };
        /* clang-format on */
        uint8_t raw_signature[64];
        size_t raw_signatureLen = sizeof(raw_signature);
        SE05x_ECDAASignatureAlgo_t ecSignAlgo = kSE05x_ECDAASignatureAlgo_ECDAA;
        sss_status_t asn_retval = kStatus_SSS_Fail;

        status = Se05x_API_ECDAASign(&context->session->s_ctx,
            context->keyObject->keyId,
            ecSignAlgo,
            digest,
            digestLen,
            random,
            sizeof(random),
            raw_signature,
            &raw_signatureLen);
        if (status != SM_OK) {
            LOG_E("SE050 ECDAA Sign failed");
            return kStatus_SSS_Fail;
        }

        asn_retval = sss_util_asn1_ecdaa_get_signature(
            signature, signatureLen, raw_signature, raw_signatureLen);
        if (asn_retval != kStatus_SSS_Success) {
            LOG_E("SE050 ECDAA Sign failed");
            return kStatus_SSS_Fail;
        }
    } break;
    case kSSS_CipherType_EC_TWISTED_ED: {
        SE05x_EDSignatureAlgo_t ecSignAlgo =
            kSE05x_EDSignatureAlgo_ED25519PH_SHA_512;
        status = Se05x_API_EdDSASign(&context->session->s_ctx,
            context->keyObject->keyId,
            ecSignAlgo,
            digest,
            digestLen,
            signature,
            signatureLen);
    } break;
    case kSSS_CipherType_RSA:
    case kSSS_CipherType_RSA_CRT: {
        if ((context->algorithm <=
                kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA512) &&
            (context->algorithm >= kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA1)) {
            /* Perform EMSA encoding on input data and and RSA decrypt on emsa data --> RSA sign without hash */
            /* clang-format off */
            uint8_t emsa_data[512] = {0,}; /* MAX - SHA512*/
            size_t emsa_len = sizeof(emsa_data);
            /* clang-format on */

            if (0 !=
                emsa_encode(context, digest, digestLen, emsa_data, &emsa_len)) {
                return kStatus_SSS_Fail;
            }
            status = Se05x_API_RSADecrypt(&context->session->s_ctx,
                context->keyObject->keyId,
                kSE05x_RSAEncryptionAlgo_NO_PAD,
                emsa_data,
                emsa_len,
                signature,
                signatureLen);
        }
        else if ((context->algorithm <=
                     kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA512) &&
                 (context->algorithm >=
                     kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA1)) {
            /* Perform PKCS1-v15 encoding on input data and and RSA decrypt on PKCS1-v15 data --> RSA sign without hash */
            /* clang-format off */
            uint8_t pkcs1v15_encode_data[512] = {0,}; /* MAX - SHA512*/
            size_t encode_data_len = sizeof(pkcs1v15_encode_data);
            /* clang-format on */

            if (0 != pkcs1_v15_encode(context,
                         digest,
                         digestLen,
                         pkcs1v15_encode_data,
                         &encode_data_len)) {
                return kStatus_SSS_Fail;
            }
            status = Se05x_API_RSADecrypt(&context->session->s_ctx,
                context->keyObject->keyId,
                kSE05x_RSAEncryptionAlgo_NO_PAD,
                pkcs1v15_encode_data,
                encode_data_len,
                signature,
                signatureLen);
        }
        else if (context->algorithm == kAlgorithm_SSS_RSASSA_NO_PADDING) {
            uint8_t padded_data[512] = {0};
            size_t padded_len = sizeof(padded_data);

            size_t parsedKeyByteLen = 0;
            uint16_t u16parsedKeyByteLen = 0;
            status = Se05x_API_ReadSize(&context->session->s_ctx,
                context->keyObject->keyId,
                &u16parsedKeyByteLen);
            parsedKeyByteLen = u16parsedKeyByteLen;
            if (status != SM_OK) {
                return kStatus_SSS_Fail;
            }

            if (digestLen <= parsedKeyByteLen && digestLen > 0) {
                memset(padded_data, 0x00, padded_len);
                memcpy(&padded_data[parsedKeyByteLen - digestLen],
                    &digest[0],
                    digestLen);
                padded_len = parsedKeyByteLen;
            }
            else {
                return kStatus_SSS_Fail;
            }
            status = Se05x_API_RSADecrypt(&context->session->s_ctx,
                context->keyObject->keyId,
                kSE05x_RSAEncryptionAlgo_NO_PAD,
                padded_data,
                padded_len,
                signature,
                signatureLen);
        }
        else {
            LOG_E("Selected padding is not supported for RSA Sign in SE050");
            return kStatus_SSS_Fail;
        }
    } break;
    default:
        break;
    }

    if (status == SM_OK) {
        retval = kStatus_SSS_Success;
#if SSS_HAVE_MBEDTLS
        if (context->keyObject->cipherType >= kSSS_CipherType_EC_NIST_P &&
            context->keyObject->cipherType <
                kSSS_CipherType_EC_BARRETO_NAEHRIG) {
            /* Workaround for ECDSA signiture to omit prefix zeros if asn1
               signiutre tag (integer) length in R and S component is 20 */

            if (context->keyObject->cipherType ==
                kSSS_CipherType_EC_TWISTED_ED) {
                /* Signature not in ASN format */
                return retval;
            }

            size_t length = 0, bufIndex = 0;
            ret = asn_1_parse_tlv(signature, &length, &bufIndex);
            if (ret != 0) {
                retval = kStatus_SSS_Fail;
                return retval;
            }
            if (signature[bufIndex] == 0x02) /* Check for tag interger */
            {
                LOG_AU8_D(signature, *signatureLen);

                int count = 0;
                uint16_t i = 0;
                /* For R and S component */
                for (i = 0; i < 2; i++) {
                    count = 0;
                    ret = asn_1_parse_tlv(signature, &length, &bufIndex);
                    if (ret != 0) {
                        retval = kStatus_SSS_Fail;
                        return retval;
                    }
                    if (length == 0x20) {
                        size_t j = bufIndex;
                        for (;; j++) {
                            if (signature[j] == 0 && signature[j + 1] > 0x7F) {
                                count++;
                            }
                            else {
                                break;
                            }
                        }
                    }
                    if (count) {
                        uint16_t k = 0;
                        signature[bufIndex - 1] -=
                            count; /* Update the tag length */
                        signature[1] -=
                            count; /* Update the Sequence tag length */

                        for (k = 0; k < (*signatureLen - bufIndex - count);
                             k++) {
                            signature[bufIndex + k] =
                                signature[bufIndex + count + k];
                        }

                        *signatureLen -= count;
                    }
                    bufIndex += length - count;
                }
            }
        }
#endif
    }

    return retval;
}

sss_status_t sss_se05x_asymmetric_sign(sss_se05x_asymmetric_t *context,
    uint8_t *srcData,
    size_t srcLen,
    uint8_t *destData,
    size_t *destLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;

    switch (context->keyObject->cipherType) {
    case kSSS_CipherType_RSA:
    case kSSS_CipherType_RSA_CRT: {
        SE05x_RSASignatureAlgo_t rsaSigningAlgo =
            se05x_get_rsa_sign_hash_mode(context->algorithm);
        status = Se05x_API_RSASign(&context->session->s_ctx,
            context->keyObject->keyId,
            rsaSigningAlgo,
            srcData,
            srcLen,
            destData,
            destLen);
    } break;
    default:
        break;
    }

    if (status == SM_OK) {
        retval = kStatus_SSS_Success;
    }

    return retval;
}

sss_status_t sss_se05x_asymmetric_verify_digest(sss_se05x_asymmetric_t *context,
    uint8_t *digest,
    size_t digestLen,
    uint8_t *signature,
    size_t signatureLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;
    SE05x_Result_t result = kSE05x_Result_FAILURE;
    switch (context->keyObject->cipherType) {
    case kSSS_CipherType_EC_NIST_P:
    case kSSS_CipherType_EC_NIST_K:
    case kSSS_CipherType_EC_BRAINPOOL: {
        SE05x_ECSignatureAlgo_t ecSignAlgo =
            se05x_get_ec_sign_hash_mode(context->algorithm);
        status = Se05x_API_ECDSAVerify(&context->session->s_ctx,
            context->keyObject->keyId,
            ecSignAlgo,
            digest,
            digestLen,
            signature,
            signatureLen,
            &result);
    } break;
    case kSSS_CipherType_EC_BARRETO_NAEHRIG: {
        LOG_W("Verify not supported for BN Curve");
    } break;
    case kSSS_CipherType_EC_TWISTED_ED: {
        SE05x_EDSignatureAlgo_t ecSignAlgo =
            kSE05x_EDSignatureAlgo_ED25519PH_SHA_512;
        status = Se05x_API_EdDSAVerify(&context->session->s_ctx,
            context->keyObject->keyId,
            ecSignAlgo,
            digest,
            digestLen,
            signature,
            signatureLen,
            &result);
    } break;
    case kSSS_CipherType_RSA:
    case kSSS_CipherType_RSA_CRT: {
        if ((context->algorithm <=
                kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA512) &&
            (context->algorithm >= kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA1)) {
            /* clang-format off */
            uint8_t dec_data[512] = { 0, }; /* MAX - SHA512*/
            size_t dec_len = sizeof(dec_data);
            /* clang-format on */

            status = Se05x_API_RSAEncrypt(&context->session->s_ctx,
                context->keyObject->keyId,
                kSE05x_RSAEncryptionAlgo_NO_PAD,
                signature,
                signatureLen,
                dec_data,
                &dec_len);

            if (0 == emsa_decode_and_compare(
                         context, dec_data, dec_len, digest, digestLen)) {
                result = kSE05x_Result_SUCCESS;
            }
        }
        else if ((context->algorithm <=
                     kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA512) &&
                 (context->algorithm >=
                     kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA1)) {
            /* clang-format off */
            uint8_t dec_data[512] = { 0, }; /* MAX - SHA512*/
            size_t dec_len = sizeof(dec_data);
            uint8_t pkcs1v15_encode_data[512] = { 0, }; /* MAX - SHA512*/
            size_t encode_data_len = sizeof(pkcs1v15_encode_data);
            /* clang-format on */

            status = Se05x_API_RSAEncrypt(&context->session->s_ctx,
                context->keyObject->keyId,
                kSE05x_RSAEncryptionAlgo_NO_PAD,
                signature,
                signatureLen,
                dec_data,
                &dec_len);

            if (0 != pkcs1_v15_encode(context,
                         digest,
                         digestLen,
                         pkcs1v15_encode_data,
                         &encode_data_len)) {
                return kStatus_SSS_Fail;
            }

            if (memcmp(dec_data, pkcs1v15_encode_data, encode_data_len) == 0) {
                result = kSE05x_Result_SUCCESS;
            }
        }
        else if (context->algorithm == kAlgorithm_SSS_RSASSA_NO_PADDING) {
            uint8_t dec_data[512] = {
                0,
            }; /*MAX - RSA4096*/
            size_t dec_len = sizeof(dec_data);

            status = Se05x_API_RSAEncrypt(&context->session->s_ctx,
                context->keyObject->keyId,
                kSE05x_RSAEncryptionAlgo_NO_PAD,
                signature,
                signatureLen,
                dec_data,
                &dec_len);

            uint8_t padded_data[512] = {0};
            size_t padded_len = sizeof(padded_data);

            size_t parsedKeyByteLen = 0;
            uint16_t u16parsedKeyByteLen = 0;
            status = Se05x_API_ReadSize(&context->session->s_ctx,
                context->keyObject->keyId,
                &u16parsedKeyByteLen);
            parsedKeyByteLen = u16parsedKeyByteLen;
            if (status != SM_OK) {
                return kStatus_SSS_Fail;
            }

            if (digestLen <= parsedKeyByteLen && digestLen > 0) {
                memset(padded_data, 0x00, padded_len);
                memcpy(&padded_data[parsedKeyByteLen - digestLen],
                    &digest[0],
                    digestLen);
                padded_len = parsedKeyByteLen;
            }

            else {
                return kStatus_SSS_Fail;
            }

            if (memcmp(&dec_data[0], &padded_data[0], padded_len) == 0) {
                result = kSE05x_Result_SUCCESS;
            }
        }
        else {
            LOG_E("Selected padding is not supported for RSA Sign in SE050");
            return kStatus_SSS_Fail;
        }

    } break;
    default:
        break;
    }

    if (status == SM_OK) {
        if (result == kSE05x_Result_SUCCESS) {
            retval = kStatus_SSS_Success;
        }
    }

    return retval;
}

sss_status_t sss_se05x_asymmetric_verify(sss_se05x_asymmetric_t *context,
    uint8_t *srcData,
    size_t srcLen,
    uint8_t *signature,
    size_t signatureLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;
    SE05x_Result_t result = kSE05x_Result_FAILURE;
    switch (context->keyObject->cipherType) {
    case kSSS_CipherType_RSA:
    case kSSS_CipherType_RSA_CRT: {
        SE05x_RSASignatureAlgo_t rsaSigningAlgo =
            se05x_get_rsa_sign_hash_mode(context->algorithm);
        status = Se05x_API_RSAVerify(&context->session->s_ctx,
            context->keyObject->keyId,
            rsaSigningAlgo,
            srcData,
            srcLen,
            signature,
            signatureLen,
            &result);
    } break;
    default:
        break;
    }

    if (status == SM_OK) {
        if (result == kSE05x_Result_SUCCESS) {
            retval = kStatus_SSS_Success;
        }
    }

    return retval;
}

void sss_se05x_asymmetric_context_free(sss_se05x_asymmetric_t *context)
{
    memset(context, 0, sizeof(*context));
}

/* End: se05x_asym */

/* ************************************************************************** */
/* Functions : sss_se05x_symm                                                 */
/* ************************************************************************** */

sss_status_t sss_se05x_symmetric_context_init(sss_se05x_symmetric_t *context,
    sss_se05x_session_t *session,
    sss_se05x_object_t *keyObject,
    sss_algorithm_t algorithm,
    sss_mode_t mode)
{
    sss_status_t retval = kStatus_SSS_Success;
#if SSSFTR_SE05X_CREATE_DELETE_CRYPTOOBJ
    SE05x_CryptoModeSubType_t subtype = {0};

    uint8_t list[1024] = {
        0,
    };

    smStatus_t status = SM_NOT_OK;
    size_t listlen = sizeof(list);
    size_t i;
    uint8_t create_crypto_obj = 1;

    context->session = session;
    context->keyObject = keyObject;
    context->algorithm = algorithm;
    context->mode = mode;
    context->cache_data_len = 0;

    switch (algorithm) {
    case kAlgorithm_SSS_AES_ECB:
        subtype.cipher = kSE05x_CipherMode_AES_ECB_NOPAD;
        context->cryptoObjectId = kSE05x_CryptoObject_AES_ECB_NOPAD;
        break;
    case kAlgorithm_SSS_AES_CBC:
        subtype.cipher = kSE05x_CipherMode_AES_CBC_NOPAD;
        context->cryptoObjectId = kSE05x_CryptoObject_AES_CBC_NOPAD;
        break;
    case kAlgorithm_SSS_AES_CTR:
        subtype.cipher = kSE05x_CipherMode_AES_CTR;
        context->cryptoObjectId = kSE05x_CryptoObject_AES_CTR;
        break;
    case kAlgorithm_SSS_DES_ECB:
        subtype.cipher = kSE05x_CipherMode_DES_ECB_NOPAD;
        context->cryptoObjectId = kSE05x_CryptoObject_DES_ECB_NOPAD;
        break;
    case kAlgorithm_SSS_DES_CBC:
        subtype.cipher = kSE05x_CipherMode_DES_ECB_NOPAD;
        context->cryptoObjectId = kSE05x_CryptoObject_DES_CBC_NOPAD;
        break;
    default:
        return kStatus_SSS_Fail;
    }

    status = Se05x_API_ReadCryptoObjectList(
        &context->session->s_ctx, list, &listlen);
    for (i = 0; i < listlen; i += 4) {
        uint16_t cryptoObjectId = list[i + 1] | (list[i + 0] << 8);
        if (cryptoObjectId == context->cryptoObjectId) {
            create_crypto_obj = 0;
        }
    }

    if (create_crypto_obj) {
        status = Se05x_API_CreateCryptoObject(&context->session->s_ctx,
            context->cryptoObjectId,
            kSE05x_CryptoContext_CIPHER,
            subtype);
        if (status != SM_OK) {
            return kStatus_SSS_Fail;
        }
    }
#endif
    return retval;
}

sss_status_t sss_se05x_cipher_one_go(sss_se05x_symmetric_t *context,
    uint8_t *iv,
    size_t ivLen,
    const uint8_t *srcData,
    uint8_t *destData,
    size_t dataLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;
    SE05x_CipherMode_t cipherMode = se05x_get_cipher_mode(context->algorithm);
    SE05x_Cipher_Oper_OneShot_t OperType =
        (context->mode == kMode_SSS_Encrypt) ?
            kSE05x_Cipher_Oper_OneShot_Encrypt :
            kSE05x_Cipher_Oper_OneShot_Decrypt;

    status = Se05x_API_CipherOneShot(&context->session->s_ctx,
        context->keyObject->keyId,
        cipherMode,
        srcData,
        dataLen,
        iv,
        ivLen,
        destData,
        &dataLen,
        OperType);

    ENSURE_OR_GO_EXIT(status == SM_OK);

    retval = kStatus_SSS_Success;
exit:
    return retval;
}

sss_status_t sss_se05x_cipher_init(
    sss_se05x_symmetric_t *context, uint8_t *iv, size_t ivLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;
    //size_t retdataLen = 0;
    SE05x_Cipher_Oper_t OperType = (context->mode == kMode_SSS_Encrypt) ?
                                       kSE05x_Cipher_Oper_Encrypt :
                                       kSE05x_Cipher_Oper_Decrypt;
    SE05x_CipherMode_t cipherMode = se05x_get_cipher_mode(context->algorithm);

    if (cipherMode == kSE05x_CipherMode_AES_ECB_NOPAD) {
        ivLen = 0;
    }

    status = Se05x_API_CipherInit(&context->session->s_ctx,
        context->keyObject->keyId,
        context->cryptoObjectId,
        iv,
        ivLen,
        OperType);
    ENSURE_OR_GO_EXIT(status == SM_OK);

    retval = kStatus_SSS_Success;
exit:
    return retval;
}

sss_status_t sss_se05x_cipher_update(sss_se05x_symmetric_t *context,
    const uint8_t *srcData,
    size_t srcLen,
    uint8_t *destData,
    size_t *destLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;
    uint8_t inputData[CIPHER_BLOCK_SIZE] = {
        0,
    };
    size_t inputData_len = 0;
    size_t src_offset = 0;
    size_t output_offset = 0;
    size_t outBuffSize = *destLen;
    size_t blockoutLen = 0;

    if ((context->cache_data_len + srcLen) < CIPHER_BLOCK_SIZE) {
        /* Insufficinet data to process . Cache the data */
        memcpy(
            (context->cache_data + context->cache_data_len), srcData, srcLen);
        context->cache_data_len = context->cache_data_len + srcLen;
        *destLen = 0;
        return kStatus_SSS_Success;
    }
    else {
        /* Concatenate the unprocessed and current input data*/
        memcpy(inputData, context->cache_data, context->cache_data_len);
        inputData_len = context->cache_data_len;
        memcpy((inputData + inputData_len),
            srcData,
            (CIPHER_BLOCK_SIZE - context->cache_data_len));
        inputData_len += (CIPHER_BLOCK_SIZE - context->cache_data_len);
        src_offset += (CIPHER_BLOCK_SIZE - context->cache_data_len);

        blockoutLen = outBuffSize;
        status = Se05x_API_CipherUpdate(&context->session->s_ctx,
            context->cryptoObjectId,
            inputData,
            inputData_len,
            (destData + output_offset),
            &blockoutLen);
        ENSURE_OR_GO_EXIT(status == SM_OK);
        outBuffSize -= blockoutLen;
        output_offset += blockoutLen;

        while (srcLen - src_offset >= CIPHER_BLOCK_SIZE) {
            memcpy(inputData, (srcData + src_offset), 16);
            src_offset += CIPHER_BLOCK_SIZE;

            blockoutLen = outBuffSize;
            status = Se05x_API_CipherUpdate(&context->session->s_ctx,
                context->cryptoObjectId,
                inputData,
                inputData_len,
                (destData + output_offset),
                &blockoutLen);
            ENSURE_OR_GO_EXIT(status == SM_OK);
            outBuffSize -= blockoutLen;
            output_offset += blockoutLen;
        }

        *destLen = output_offset;

        /* Copy unprocessed data to cache */
        memcpy(
            context->cache_data, (srcData + src_offset), (srcLen - src_offset));
        context->cache_data_len = (srcLen - src_offset);
    }

    retval = kStatus_SSS_Success;
exit:
    if (retval == kStatus_SSS_Fail) {
        *destLen = 0;
    }
    return retval;
}

sss_status_t sss_se05x_cipher_finish(sss_se05x_symmetric_t *context,
    const uint8_t *srcData,
    size_t srcLen,
    uint8_t *destData,
    size_t *destLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;
    uint8_t srcdata_updated[2 * CIPHER_BLOCK_SIZE] = {
        0,
    };
    size_t srcdata_updated_len = 0;

    if (srcLen > CIPHER_BLOCK_SIZE) {
        LOG_E("srcLen cannot be grater than 16 bytes. Call update function ");
        *destLen = 0;
        goto exit;
    }

    if (context->cache_data_len != 0) {
        memcpy(srcdata_updated, context->cache_data, context->cache_data_len);
        srcdata_updated_len = context->cache_data_len;
    }
    if (srcLen != 0) {
        memcpy((srcdata_updated + srcdata_updated_len), srcData, srcLen);
        srcdata_updated_len += srcLen;
    }

    if (context->algorithm == kAlgorithm_SSS_AES_ECB ||
        context->algorithm == kAlgorithm_SSS_AES_CBC) {
        if (srcdata_updated_len % CIPHER_BLOCK_SIZE != 0) {
            srcdata_updated_len =
                srcdata_updated_len +
                (CIPHER_BLOCK_SIZE - (srcdata_updated_len % 16));
        }
    }

    status = Se05x_API_CipherFinal(&context->session->s_ctx,
        context->cryptoObjectId,
        srcdata_updated,
        srcdata_updated_len,
        destData,
        destLen);
    ENSURE_OR_GO_EXIT(status == SM_OK);

    retval = kStatus_SSS_Success;
exit:
    return retval;
}

sss_status_t sss_se05x_cipher_crypt_ctr(sss_se05x_symmetric_t *context,
    const uint8_t *srcData,
    uint8_t *destData,
    size_t size,
    uint8_t *initialCounter,
    uint8_t *lastEncryptedCounter,
    size_t *szLeft)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;
    size_t outputDataLen = 128;
    SE05x_CipherMode_t cipherMode = se05x_get_cipher_mode(context->algorithm);
    SE05x_Cipher_Oper_OneShot_t OperType =
        (context->mode == kMode_SSS_Encrypt) ?
            kSE05x_Cipher_Oper_OneShot_Encrypt :
            kSE05x_Cipher_Oper_OneShot_Decrypt;

    status = Se05x_API_CipherOneShot(&context->session->s_ctx,
        context->keyObject->keyId,
        cipherMode,
        srcData,
        size,
        initialCounter,
        16,
        destData,
        &outputDataLen,
        OperType);

    ENSURE_OR_GO_EXIT(status == SM_OK);

    retval = kStatus_SSS_Success;
exit:
    return retval;
}

void sss_se05x_symmetric_context_free(sss_se05x_symmetric_t *context)
{
#if SSSFTR_SE05X_CREATE_DELETE_CRYPTOOBJ
    smStatus_t status;
    uint8_t list[1024] = {
        0,
    };
    uint8_t object_exists = 0;
    size_t listlen = sizeof(list);

    if (context->cryptoObjectId != 0) {
        status = Se05x_API_ReadCryptoObjectList(
            &context->session->s_ctx, list, &listlen);
        for (size_t i = 0; i < listlen; i += 4) {
            uint16_t cryptoObjectId = list[i + 1] | (list[i + 0] << 8);
            if (cryptoObjectId == context->cryptoObjectId) {
                object_exists = 1;
            }
        }

        if (object_exists) {
            status = Se05x_API_DeleteCryptoObject(
                &context->session->s_ctx, context->cryptoObjectId);
            if (status != SM_OK) {
                LOG_D("Could not delete crypto object 0x04X",
                    context->cryptoObjectId);
                return;
            }
        }
    }
#endif
    memset(context, 0, sizeof(*context));
}

/* End: se05x_symm */

/* ************************************************************************** */
/* Functions : sss_se05x_aead                                                 */
/* ************************************************************************** */

sss_status_t sss_se05x_aead_context_init(sss_se05x_aead_t *context,
    sss_se05x_session_t *session,
    sss_se05x_object_t *keyObject,
    sss_algorithm_t algorithm,
    sss_mode_t mode)
{
    return kStatus_SSS_Fail;
}

sss_status_t sss_se05x_aead_one_go(sss_se05x_aead_t *context,
    const uint8_t *srcData,
    uint8_t *destData,
    size_t size,
    uint8_t *nonce,
    size_t nonceLen,
    const uint8_t *aad,
    size_t aadLen,
    uint8_t *tag,
    size_t *tagLen)
{
    return kStatus_SSS_Fail;
}

sss_status_t sss_se05x_aead_init(sss_se05x_aead_t *context,
    uint8_t *nonce,
    size_t nonceLen,
    size_t tagLen,
    size_t aadLen,
    size_t payloadLen)
{
    return kStatus_SSS_Fail;
}

sss_status_t sss_se05x_aead_update_aad(
    sss_se05x_aead_t *context, const uint8_t *aadData, size_t aadDataLen)
{
    return kStatus_SSS_Fail;
}

sss_status_t sss_se05x_aead_update(sss_se05x_aead_t *context,
    const uint8_t *srcData,
    size_t srcLen,
    uint8_t destData,
    size_t *destLen)
{
    return kStatus_SSS_Fail;
}

sss_status_t sss_se05x_aead_finish(sss_se05x_aead_t *context,
    const uint8_t *srcData,
    size_t srcLen,
    uint8_t destData,
    size_t *destLen,
    uint8_t *tag,
    size_t *tagLen)
{
    return kStatus_SSS_Fail;
}

void sss_se05x_aead_context_free(sss_se05x_aead_t *context)
{
    ;
}

/* End: se05x_aead */

/* ************************************************************************** */
/* Functions : sss_se05x_mac                                                  */
/* ************************************************************************** */

sss_status_t sss_se05x_mac_context_init(sss_se05x_mac_t *context,
    sss_se05x_session_t *session,
    sss_se05x_object_t *keyObject,
    sss_algorithm_t algorithm,
    sss_mode_t mode)
{
    sss_status_t retval = kStatus_SSS_Success;
    context->session = session;
    context->keyObject = keyObject;
    context->algorithm = algorithm;
    context->mode = mode;
    return retval;
}

sss_status_t sss_se05x_mac_one_go(sss_se05x_mac_t *context,
    const uint8_t *message,
    size_t messageLen,
    uint8_t *mac,
    size_t *macLen)
{
    sss_status_t retval = kStatus_SSS_Fail;

    smStatus_t status = SM_NOT_OK;

    SE05x_MACAlgo_t macOperation = se05x_get_mac_algo(context->algorithm);

    status = Se05x_API_MACOneShot_G(&context->session->s_ctx,
        context->keyObject->keyId,
        macOperation,
        message,
        messageLen,
        mac,
        macLen);
    ENSURE_OR_GO_EXIT(status == SM_OK);

    retval = kStatus_SSS_Success;
exit:
    return retval;
}

sss_status_t sss_se05x_mac_validate_one_go(sss_se05x_mac_t *context,
    const uint8_t *message,
    size_t messageLen,
    uint8_t *mac,
    size_t macLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;
    SE05x_MACAlgo_t macOperation;
    SE05x_Result_t result = kSE05x_Result_FAILURE;
    size_t result_size = sizeof(result);

    if (context == NULL) {
        goto exit;
    }

    macOperation = se05x_get_mac_algo(context->algorithm);

    status = Se05x_API_MACOneShot_V(&context->session->s_ctx,
        context->keyObject->keyId,
        macOperation,
        message,
        messageLen,
        mac,
        macLen,
        (uint8_t *)&result,
        &result_size);

    if (status == SM_OK) {
        if (result == kSE05x_Result_SUCCESS) {
            retval = kStatus_SSS_Success;
        }
    }

exit:
    return retval;
}

sss_status_t sss_se05x_mac_init(sss_se05x_mac_t *context)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;
#if SSSFTR_SE05X_CREATE_DELETE_CRYPTOOBJ
    SE05x_CryptoModeSubType_t subtype = {0};

    uint8_t list[1024] = {
        0,
    };
    size_t listlen = sizeof(list);
    size_t i;
    uint8_t create_crypto_obj = 1;

    SE05x_CryptoContext_t cryptoContext;

    switch (context->algorithm) {
    case kAlgorithm_SSS_CMAC_AES:
        subtype.mac = kSE05x_MACAlgo_CMAC_128;
        cryptoContext = kSE05x_CryptoContext_SIGNATURE;
        context->cryptoObjectId = kSE05x_CryptoObject_CMAC_128;
        break;
    case kAlgorithm_SSS_HMAC_SHA1:
        subtype.mac = kSE05x_MACAlgo_HMAC_SHA1;
        cryptoContext = kSE05x_CryptoContext_SIGNATURE;
        context->cryptoObjectId = kSE05x_CryptoObject_HMAC_SHA1;
        break;
    case kAlgorithm_SSS_HMAC_SHA256:
        subtype.mac = kSE05x_MACAlgo_HMAC_SHA256;
        cryptoContext = kSE05x_CryptoContext_SIGNATURE;
        context->cryptoObjectId = kSE05x_CryptoObject_HMAC_SHA256;
        break;
    case kAlgorithm_SSS_HMAC_SHA384:
        subtype.mac = kSE05x_MACAlgo_HMAC_SHA384;
        cryptoContext = kSE05x_CryptoContext_SIGNATURE;
        context->cryptoObjectId = kSE05x_CryptoObject_HMAC_SHA384;
        break;
    case kAlgorithm_SSS_HMAC_SHA512:
        subtype.mac = kSE05x_MACAlgo_HMAC_SHA512;
        cryptoContext = kSE05x_CryptoContext_SIGNATURE;
        context->cryptoObjectId = kSE05x_CryptoObject_HMAC_SHA512;
        break;
    default:
        return kStatus_SSS_Fail;
    }

    status = Se05x_API_ReadCryptoObjectList(
        &context->session->s_ctx, list, &listlen);
    for (i = 0; i < listlen; i += 4) {
        uint16_t cryptoObjectId = list[i + 1] | (list[i + 0] << 8);
        if (cryptoObjectId == context->cryptoObjectId) {
            create_crypto_obj = 0;
        }
    }

    if (create_crypto_obj) {
        status = Se05x_API_CreateCryptoObject(&context->session->s_ctx,
            context->cryptoObjectId,
            cryptoContext,
            subtype);
        if (status != SM_OK) {
            LOG_W("CreateCryptoObject Failed");
            return kStatus_SSS_Fail;
        }
    }
#endif
    SE05x_Mac_Oper_t operType = kSE05x_Mac_Oper_Generate;

    status = Se05x_API_MACInit(&context->session->s_ctx,
        context->keyObject->keyId,
        context->cryptoObjectId,
        operType);
    ENSURE_OR_GO_EXIT(status == SM_OK);

    retval = kStatus_SSS_Success;
exit:
    return retval;
}

sss_status_t sss_se05x_mac_update(
    sss_se05x_mac_t *context, const uint8_t *message, size_t messageLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;

    //SE05x_MACAlgo_t macOperation = se05x_get_mac_algo(context->algorithm);

    status = Se05x_API_MACUpdate(
        &context->session->s_ctx, message, messageLen, context->cryptoObjectId);
    ENSURE_OR_GO_EXIT(status == SM_OK);

    retval = kStatus_SSS_Success;
exit:
    return retval;
}

sss_status_t sss_se05x_mac_finish(
    sss_se05x_mac_t *context, uint8_t *mac, size_t *macLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;

    //SE05x_MACAlgo_t macOperation = se05x_get_mac_algo(context->algorithm);

    status = Se05x_API_MACFinal(&context->session->s_ctx,
        NULL,
        0,
        context->cryptoObjectId,
        NULL,
        0,
        mac,
        macLen);
    ENSURE_OR_GO_EXIT(status == SM_OK);

    retval = kStatus_SSS_Success;
exit:
    return retval;
}

void sss_se05x_mac_context_free(sss_se05x_mac_t *context)
{
    if (context->cryptoObjectId != 0) {
        smStatus_t status = Se05x_API_DeleteCryptoObject(
            &context->session->s_ctx, context->cryptoObjectId);
        if (status != SM_OK) {
            LOG_D("Could not delete crypto object 0x04X",
                context->cryptoObjectId);
            return;
        }
    }
    memset(context, 0, sizeof(*context));
}

/* End: se05x_mac */

/* ************************************************************************** */
/* Functions : sss_se05x_md                                                   */
/* ************************************************************************** */

sss_status_t sss_se05x_digest_context_init(sss_se05x_digest_t *context,
    sss_se05x_session_t *session,
    sss_algorithm_t algorithm,
    sss_mode_t mode)
{
    sss_status_t retval = kStatus_SSS_Success;
#if SSSFTR_SE05X_CREATE_DELETE_CRYPTOOBJ
    SE05x_CryptoModeSubType_t subtype = {0};

    smStatus_t status = SM_NOT_OK;
    uint8_t list[1024] = {
        0,
    };
    size_t listlen = sizeof(list);
    size_t i;
    uint8_t create_crypto_obj = 1;

    context->session = session;
    context->algorithm = algorithm;
    context->mode = mode;

    switch (algorithm) {
    case kAlgorithm_SSS_SHA1:
        subtype.digest = kSE05x_DigestMode_SHA;
        context->cryptoObjectId = kSE05x_CryptoObject_DIGEST_SHA;
        break;
    case kAlgorithm_SSS_SHA224:
        subtype.digest = kSE05x_DigestMode_SHA224;
        context->cryptoObjectId = kSE05x_CryptoObject_DIGEST_SHA224;
        break;
    case kAlgorithm_SSS_SHA256:
        subtype.digest = kSE05x_DigestMode_SHA256;
        context->cryptoObjectId = kSE05x_CryptoObject_DIGEST_SHA256;
        break;
    case kAlgorithm_SSS_SHA384:
        subtype.digest = kSE05x_DigestMode_SHA384;
        context->cryptoObjectId = kSE05x_CryptoObject_DIGEST_SHA384;
        break;
    case kAlgorithm_SSS_SHA512:
        subtype.digest = kSE05x_DigestMode_SHA512;
        context->cryptoObjectId = kSE05x_CryptoObject_DIGEST_SHA512;
        break;
    default:
        return kStatus_SSS_Fail;
    }

    status = Se05x_API_ReadCryptoObjectList(
        &context->session->s_ctx, list, &listlen);
    for (i = 0; i < listlen; i += 4) {
        uint16_t cryptoObjectId = list[i + 1] | (list[i + 0] << 8);
        if (cryptoObjectId == context->cryptoObjectId) {
            create_crypto_obj = 0;
        }
    }

    if (create_crypto_obj) {
        status = Se05x_API_CreateCryptoObject(&context->session->s_ctx,
            context->cryptoObjectId,
            kSE05x_CryptoContext_DIGEST,
            subtype);
        if (status != SM_OK) {
            return kStatus_SSS_Fail;
        }
    }
#endif

    return retval;
}

sss_status_t sss_se05x_digest_one_go(sss_se05x_digest_t *context,
    const uint8_t *message,
    size_t messageLen,
    uint8_t *digest,
    size_t *digestLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;
    uint8_t sha_type = se05x_get_sha_algo(context->algorithm);

    status = Se05x_API_SHAOneShot(&context->session->s_ctx,
        sha_type,
        message,
        messageLen,
        digest,
        digestLen);
    if (status != SM_OK) {
        *digestLen = 0;
        goto exit;
    }

    retval = kStatus_SSS_Success;
exit:
    return retval;
}

sss_status_t sss_se05x_digest_init(sss_se05x_digest_t *context)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;

    status =
        Se05x_API_DigestInit(&context->session->s_ctx, context->cryptoObjectId);
    ENSURE_OR_GO_EXIT(status == SM_OK);

    retval = kStatus_SSS_Success;
exit:
    return retval;
}

sss_status_t sss_se05x_digest_update(
    sss_se05x_digest_t *context, const uint8_t *message, size_t messageLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;

    status = Se05x_API_DigestUpdate(
        &context->session->s_ctx, context->cryptoObjectId, message, messageLen);
    ENSURE_OR_GO_EXIT(status == SM_OK);

    retval = kStatus_SSS_Success;
exit:
    return retval;
}

sss_status_t sss_se05x_digest_finish(
    sss_se05x_digest_t *context, uint8_t *digest, size_t *digestLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;

    status = Se05x_API_DigestFinal(&context->session->s_ctx,
        context->cryptoObjectId,
        NULL,
        0,
        digest,
        digestLen);
    ENSURE_OR_GO_EXIT(status == SM_OK);

    retval = kStatus_SSS_Success;
exit:
    return retval;
}

void sss_se05x_digest_context_free(sss_se05x_digest_t *context)
{
    if (context->cryptoObjectId != 0) {
        smStatus_t status = Se05x_API_DeleteCryptoObject(
            &context->session->s_ctx, context->cryptoObjectId);
        if (status != SM_OK) {
            LOG_D("Could not delete crypto object 0x04X",
                context->cryptoObjectId);
            return;
        }
    }
    memset(context, 0, sizeof(*context));
}

/* End: se05x_md */

/* ************************************************************************** */
/* Functions : sss_se05x_rng                                                  */
/* ************************************************************************** */

sss_status_t sss_se05x_rng_context_init(
    sss_se05x_rng_context_t *context, sss_se05x_session_t *session)
{
    sss_status_t retval = kStatus_SSS_Success;
    context->session = session;
    return retval;
}

sss_status_t sss_se05x_rng_get_random(
    sss_se05x_rng_context_t *context, uint8_t *random_data, size_t dataLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;
    size_t chunk = 0;
    size_t offset = 0;

    while (dataLen > 0) {
        /* TODO - Replace 512 with max rsp buffer size based on with/without SCP */
        if (dataLen > 512) {
            chunk = 512;
        }
        else {
            chunk = dataLen;
        }

        status = Se05x_API_GetRandom(&context->session->s_ctx,
            (uint16_t)chunk,
            (random_data + offset),
            &chunk);
        ENSURE_OR_GO_EXIT(status == SM_OK);

        offset += chunk;
        dataLen -= chunk;
    }

    retval = kStatus_SSS_Success;
exit:
    return retval;
}

sss_status_t sss_se05x_rng_context_free(sss_se05x_rng_context_t *context)
{
    sss_status_t retval = kStatus_SSS_Success;
    memset(context, 0, sizeof(*context));
    return retval;
}
/* End: se05x_rng */

sss_status_t sss_se05x_tunnel_context_init(
    sss_se05x_tunnel_context_t *context, sss_se05x_session_t *session)
{
    context->se05x_session = session;
    sss_status_t retval = kStatus_SSS_Success;
#if (__GNUC__ && !AX_EMBEDDED)
    if (pthread_mutex_init(&context->channelLock, NULL) != 0) {
        LOG_E("\n mutex init has failed");
        return kStatus_SSS_Fail;
    }
    else {
        LOG_D("Mutex Init successfull");
    }
#endif
    return retval;
}

sss_status_t sss_se05x_tunnel(sss_se05x_tunnel_context_t *context,
    uint8_t *data,
    size_t dataLen,
    sss_se05x_object_t *keyObjects,
    uint32_t keyObjectCount,
    uint32_t tunnelType)
{
    sss_status_t retval = kStatus_SSS_Fail;
    return retval;
}

void sss_se05x_tunnel_context_free(sss_se05x_tunnel_context_t *context)
{
    memset(context, 0, sizeof(*context));
#if (__GNUC__ && !AX_EMBEDDED)
    pthread_mutex_destroy(&context->channelLock);
#endif
}

static smStatus_t sss_se05x_TXn(struct Se05xSession *pSession,
    const tlvHeader_t *hdr,
    uint8_t *cmdBuf,
    size_t cmdBufLen,
    uint8_t *rsp,
    size_t *rspLen,
    uint8_t hasle)
{
    smStatus_t ret = SM_NOT_OK;
    tlvHeader_t outHdr = {
        0,
    };
    uint8_t txBuf[1024] = {
        0,
    };
    size_t txBufLen = sizeof(txBuf);

    ret = pSession->fp_Transform(
        pSession, hdr, cmdBuf, cmdBufLen, &outHdr, txBuf, &txBufLen, hasle);
    ENSURE_OR_GO_EXIT(ret == SM_OK);
    ret = pSession->fp_RawTXn(pSession->pChannelCtx,
        pSession->authType,
        &outHdr,
        txBuf,
        txBufLen,
        rsp,
        rspLen,
        hasle);

    ret = pSession->fp_DeCrypt(pSession, cmdBufLen, rsp, rspLen, hasle);

    ENSURE_OR_GO_EXIT(ret == SM_OK);
exit:
    return ret;
}

static smStatus_t sss_se05x_channel_txnRaw(const tlvHeader_t *hdr,
    uint8_t *cmdBuf,
    size_t cmdBufLen,
    uint8_t *rsp,
    size_t *rspLen,
    uint8_t hasle)
{
    uint8_t txBuf[1024] = {0};
    size_t i = 0;
    memcpy(&txBuf[i], hdr, sizeof(*hdr));
    smStatus_t ret = SM_NOT_OK;
    i += sizeof(*hdr);
    if (cmdBufLen > 0) {
        // The Lc field must be extended in case the length does not fit
        // into a single byte (Note, while the standard would allow to
        // encode 0x100 as 0x00 in the Lc field, nobody who is sane in his mind
        // would actually do that).
        if ((cmdBufLen < 0xFF) && !hasle) {
            txBuf[i++] = (uint8_t)cmdBufLen;
        }
        else {
            txBuf[i++] = 0x00;
            txBuf[i++] = 0xFFu & (cmdBufLen >> 8);
            txBuf[i++] = 0xFFu & (cmdBufLen);
        }
        memcpy(&txBuf[i], cmdBuf, cmdBufLen);
        i += cmdBufLen;
    }
    if (hasle) {
        txBuf[i++] = 0x00;
        txBuf[i++] = 0x00;
    }
    uint32_t U32rspLen = (uint32_t)*rspLen;
    ret = (smStatus_t)smCom_TransceiveRaw(txBuf, (U16)i, rsp, &U32rspLen);
    *rspLen = U32rspLen;
    return ret;
}

static smStatus_t sss_se05x_channel_txn(
    struct _sss_se05x_tunnel_context *pChannelCtx,
    SE_AuthType_t currAuth,
    const tlvHeader_t *hdr,
    uint8_t *cmdBuf,
    size_t cmdBufLen,
    uint8_t *rsp,
    size_t *rspLen,
    uint8_t hasle)
{
    smStatus_t retStatus = SM_NOT_OK;

    if ((pChannelCtx != NULL)) {
#if SSSFTR_SE05X_AuthFastSCP || SSSFTR_SE05X_AuthSession
        struct Se05xSession *se05xCtx =
            (struct Se05xSession *)&pChannelCtx->se05x_session->s_ctx;
        if (se05xCtx->authType == kSSS_AuthType_SCP03) {
#if (__GNUC__ && !AX_EMBEDDED)
            LOCK_TXN(pChannelCtx->channelLock);
#endif
            retStatus = se05xCtx->fp_TXn(
                se05xCtx, hdr, cmdBuf, cmdBufLen, rsp, rspLen, hasle);

#if (__GNUC__ && !AX_EMBEDDED)
            UNLOCK_TXN(pChannelCtx->channelLock);
#endif
            ENSURE_OR_GO_EXIT(retStatus == SM_OK);
        }
        else if (se05xCtx->authType == kSSS_AuthType_None) {
#if (__GNUC__ && !AX_EMBEDDED)
            LOCK_TXN(pChannelCtx->channelLock);
#endif
            retStatus = se05xCtx->fp_TXn(
                se05xCtx, hdr, cmdBuf, cmdBufLen, rsp, rspLen, hasle);

#if (__GNUC__ && !AX_EMBEDDED)
            UNLOCK_TXN(pChannelCtx->channelLock);
#endif
            ENSURE_OR_GO_EXIT(retStatus == SM_OK);
        }
        else {
            LOG_E("Invalid auth type");
            goto exit;
        }
#endif
    }
    else {
        if (currAuth == kSSS_AuthType_SCP03) {
            uint32_t u32rspLen = (uint32_t)*rspLen;
            retStatus = (smStatus_t)smCom_TransceiveRaw(
                cmdBuf, (uint16_t)cmdBufLen, rsp, &u32rspLen);
            ENSURE_OR_GO_EXIT(retStatus == SM_OK);
            *rspLen = u32rspLen;
        }
        else {
            retStatus = sss_se05x_channel_txnRaw(
                hdr, cmdBuf, cmdBufLen, rsp, rspLen, hasle);
            ENSURE_OR_GO_EXIT(retStatus == SM_OK);
        }
    }

exit:
    return retStatus;
}

/* End: se05x_tunnel */

sss_status_t sss_se05x_tls_generate_random(
    sss_se05x_rng_context_t *context, uint8_t *random_data, size_t *dataLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;

    if ((context != NULL) && (random_data != NULL) && (dataLen != NULL)) {
        status = Se05x_API_TLSGenerateRandom(
            &context->session->s_ctx, random_data, dataLen);
        if (status != SM_OK) {
            goto exit;
        }
    }
    else {
        goto exit;
    }

    retval = kStatus_SSS_Success;
exit:
    return retval;
}

sss_status_t sss_se05x_tls_get_preMaster_secret(sss_se05x_key_store_t *keyStore,
    uint32_t keyPairId,
    uint32_t pskId,
    uint32_t hmacKeyId,
    const uint8_t *inputData,
    size_t inputDataLen)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;

    if ((keyStore == NULL) || (inputData == NULL))
        goto exit;

    status = Se05x_API_TLSCalculatePreMasterSecret(&keyStore->session->s_ctx,
        keyPairId,
        pskId,
        hmacKeyId,
        inputData,
        inputDataLen);
    if (status == SM_OK) {
        retval = kStatus_SSS_Success;
    }

exit:
    return retval;
}

sss_status_t sss_se05x_tls_perform_PRF(sss_se05x_mac_t *context,
    const uint8_t *label,
    size_t labelLen,
    const uint8_t *random,
    size_t randomLen,
    uint16_t reqLen,
    uint8_t *outputData,
    size_t *poutputDataLen,
    const SE05x_TLSPerformPRFType_t tlsprf)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;
    SE05x_DigestMode_t digestMode;

    if ((context == NULL) || (label == NULL) || (random == NULL))
        goto exit;

    if ((outputData == NULL) || (poutputDataLen == NULL))
        goto exit;

    digestMode = se05x_get_sha_algo(context->algorithm);

    status = Se05x_API_TLSPerformPRF(&context->session->s_ctx,
        context->keyObject->keyId,
        digestMode,
        label,
        labelLen,
        random,
        randomLen,
        reqLen,
        outputData,
        poutputDataLen,
        tlsprf);

    if (status != SM_OK)
        goto exit;

    retval = kStatus_SSS_Success;
exit:
    return retval;
}

sss_status_t sss_se05x_session_cancel_scp_request(sss_se05x_session_t *session)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;

    if (session == NULL)
        goto exit;

    status = Se05x_API_SetPlatformSCPRequest(
        &session->s_ctx, kSE05x_PlatformSCPRequest_NOT_REQUIRED);

    if (status == SM_OK) {
        retval = kStatus_SSS_Success;
    }

exit:
    return retval;
}

sss_status_t sss_se05x_set_feature(
    sss_se05x_session_t *session, SE05x_Applet_Feature_t feature)
{
    sss_status_t retval = kStatus_SSS_Fail;
    smStatus_t status = SM_NOT_OK;
    SE05x_Variant_t variant = 0;

    if (session == NULL)
        goto exit;

    if (feature.AppletConfig_ECDAA == 1)
        variant |= kSE05x_AppletConfig_ECDAA;
    else if (feature.AppletConfig_ECDSA_ECDH_ECDHE == 1)
        variant |= kSE05x_AppletConfig_ECDSA_ECDH_ECDHE;
    else if (feature.AppletConfig_EDDSA == 1)
        variant |= kSE05x_AppletConfig_EDDSA;
    else if (feature.AppletConfig_DH_MONT == 1)
        variant |= kSE05x_AppletConfig_DH_MONT;
    else if (feature.AppletConfig_HMAC == 1)
        variant |= kSE05x_AppletConfig_HMAC;
    else if (feature.AppletConfig_RSA_PLAIN == 1)
        variant |= kSE05x_AppletConfig_RSA_PLAIN;
    else if (feature.AppletConfig_RSA_CRT == 1)
        variant |= kSE05x_AppletConfig_RSA_CRT;
    else if (feature.AppletConfig_AES == 1)
        variant |= kSE05x_AppletConfig_AES;
    else if (feature.AppletConfig_DES == 1)
        variant |= kSE05x_AppletConfig_DES;
    else if (feature.AppletConfig_PBKDF == 1)
        variant |= kSE05x_AppletConfig_PBKDF;
    else if (feature.AppletConfig_TLS == 1)
        variant |= kSE05x_AppletConfig_TLS;
    else if (feature.AppletConfig_MIFARE == 1)
        variant |= kSE05x_AppletConfig_MIFARE;
    else if (feature.AppletConfig_I2CM == 1)
        variant |= kSE05x_AppletConfig_I2CM;
    else
        goto exit;

    status = Se05x_API_SetAppletFeatures(&session->s_ctx, variant);

    if (status == SM_OK) {
        retval = kStatus_SSS_Success;
    }

exit:
    return retval;
}

#if SSSFTR_SE05X_AuthSession
static smStatus_t se05x_CreateVerifyUserIDSession(pSe05xSession_t se05xSession,
    const uint32_t auth_id,
    SE05x_AuthCtx_ID_t *userId,
    pSe05xPolicy_t policy)
{
    sss_status_t retval = kStatus_SSS_Fail;
    SE05x_Result_t exists = kSE05x_Result_FAILURE;
    smStatus_t status = SM_NOT_OK;
    size_t sessionIdLen = 8;
    uint8_t keyVal[60];
    size_t keyValLen = sizeof(keyVal);
    size_t keyBitLen = sizeof(keyVal) * 8;

    status = Se05x_API_CheckObjectExists(se05xSession, auth_id, &exists);
    if (status == SM_OK && exists == kSE05x_Result_FAILURE) {
        status = SM_NOT_OK;
        LOG_E("UserID is not Provisioned!!!");
    }
    if (status == SM_OK) {
        status = Se05x_API_CreateSession(
            se05xSession, auth_id, se05xSession->value, &sessionIdLen);
    }
    if (status == SM_OK) {
        status = SM_NOT_OK;
        retval = sss_key_store_get_key(userId->pObj->keyStore,
            userId->pObj,
            keyVal,
            &keyValLen,
            &keyBitLen);
        if (retval == kStatus_SSS_Success) {
            se05xSession->hasSession = 1;
            status =
                Se05x_API_VerifySessionUserID(se05xSession, keyVal, keyValLen);
            if (status == SM_OK) {
                if (policy->value > 0) {
                    status = SM_NOT_OK;
                    status =
                        Se05x_API_ExchangeSessionData(se05xSession, policy);
                }
            }
        }
    }
    return status;
}
#endif

#if SSS_HAVE_SCP_SCP03_SSS
#if SSSFTR_SE05X_AuthSession
static smStatus_t se05x_CreateVerifyAppletSCP03Session(
    pSe05xSession_t se05xSession,
    const uint32_t auth_id,
    NXSCP03_AuthCtx_t *pAppletSCPCtx)
{
    SE05x_Result_t exists = kSE05x_Result_FAILURE;
    smStatus_t status = SM_NOT_OK;
    size_t sessionIdLen = 8;
    sss_status_t retval = kStatus_SSS_Fail;

    status = Se05x_API_CheckObjectExists(se05xSession, auth_id, &exists);
    if (status == SM_OK && exists == kSE05x_Result_FAILURE) {
        status = SM_NOT_OK;
        LOG_E("Applet key is not Provisioned!!!");
    }
    if (status == SM_OK) {
        status = Se05x_API_CreateSession(
            se05xSession, auth_id, se05xSession->value, &sessionIdLen);
        if (status != SM_OK) {
            se05xSession->hasSession = 0;
        }
        else {
            se05xSession->hasSession = 1;
            se05xSession->authType = kSSS_AuthType_AppletSCP03;
            retval = nxScp03_AuthenticateChannel(se05xSession, pAppletSCPCtx);
            if (retval == kStatus_SSS_Success) {
                pAppletSCPCtx->pDyn_ctx->authType = kSSS_AuthType_AppletSCP03;
                se05xSession->pdynScp03Ctx = pAppletSCPCtx->pDyn_ctx;
                status = SM_OK;
            }
            else {
                status = SM_NOT_OK;
            }
        }
    }
    return status;
}
#endif

#if SSSFTR_SE05X_AuthFastSCP
static smStatus_t se05x_CreateFastScpSession(pSe05xSession_t se05xSession,
    const uint32_t auth_id,
    SE05x_AuthCtx_FastScp_t *pFScpCtx)
{
    sss_status_t retval = kStatus_SSS_Fail;
    SE05x_Result_t exists = kSE05x_Result_FAILURE;
    smStatus_t status = SM_NOT_OK;
    size_t sessionIdLen = 8;

    status = Se05x_API_CheckObjectExists(se05xSession, auth_id, &exists);
    if (status == SM_OK && exists == kSE05x_Result_FAILURE) {
        status = SM_NOT_OK;
        LOG_E("SE ECDSA Public Key is not Provisioned!!!");
    }
    if (status == SM_OK) {
        status = Se05x_API_CreateSession(
            se05xSession, auth_id, se05xSession->value, &sessionIdLen);
        if (status != SM_OK) {
            se05xSession->hasSession = 0;
        }
        else {
            status = SM_NOT_OK;
            se05xSession->hasSession = 1;
            retval = nxFastScp_AuthenticateChannel(se05xSession, pFScpCtx);
            if (retval == kStatus_SSS_Success) {
                NXSCP03_DynCtx_t *pDyn_ctx = pFScpCtx->pDyn_ctx;

                pDyn_ctx->authType = se05xSession->authType =
                    kSSS_AuthType_FastSCP;
                se05xSession->pdynScp03Ctx = pFScpCtx->pDyn_ctx;
                status = SM_OK;
            }
        }
    }
    return status;
}
#endif /* SSSFTR_SE05X_AuthFastSCP */
#endif

static SE05x_ECSignatureAlgo_t se05x_get_ec_sign_hash_mode(
    sss_algorithm_t algorithm)
{
    uint8_t mode;
    switch (algorithm) {
    case kAlgorithm_SSS_SHA1:
    case kAlgorithm_SSS_ECDSA_SHA1:
        mode = kSE05x_ECSignatureAlgo_SHA;
        break;
    case kAlgorithm_SSS_SHA224:
    case kAlgorithm_SSS_ECDSA_SHA224:
        mode = kSE05x_ECSignatureAlgo_SHA_224;
        break;
    case kAlgorithm_SSS_SHA256:
    case kAlgorithm_SSS_ECDSA_SHA256:
        mode = kSE05x_ECSignatureAlgo_SHA_256;
        break;
    case kAlgorithm_SSS_SHA384:
    case kAlgorithm_SSS_ECDSA_SHA384:
        mode = kSE05x_ECSignatureAlgo_SHA_384;
        break;
    case kAlgorithm_SSS_SHA512:
    case kAlgorithm_SSS_ECDSA_SHA512:
        mode = kSE05x_ECSignatureAlgo_SHA_512;
        break;
    default:
        mode = kSE05x_ECSignatureAlgo_PLAIN;
        break;
    }
    return mode;
}

static SE05x_RSAEncryptionAlgo_t se05x_get_rsa_encrypt_mode(
    sss_algorithm_t algorithm)
{
    SE05x_RSAEncryptionAlgo_t mode;
    switch (algorithm) {
    case kAlgorithm_SSS_RSAES_PKCS1_OAEP_SHA1:
    case kAlgorithm_SSS_RSAES_PKCS1_OAEP_SHA224:
    case kAlgorithm_SSS_RSAES_PKCS1_OAEP_SHA256:
    case kAlgorithm_SSS_RSAES_PKCS1_OAEP_SHA384:
    case kAlgorithm_SSS_RSAES_PKCS1_OAEP_SHA512:
        mode = kSE05x_RSAEncryptionAlgo_PKCS1_OAEP;
        break;
    case kAlgorithm_SSS_RSAES_PKCS1_V1_5_SHA1:
    case kAlgorithm_SSS_RSAES_PKCS1_V1_5_SHA224:
    case kAlgorithm_SSS_RSAES_PKCS1_V1_5_SHA256:
    case kAlgorithm_SSS_RSAES_PKCS1_V1_5_SHA384:
    case kAlgorithm_SSS_RSAES_PKCS1_V1_5_SHA512:
        mode = kSE05x_RSAEncryptionAlgo_PKCS1;
        break;
    default:
        mode = kSE05x_RSAEncryptionAlgo_NO_PAD;
        break;
    }
    return mode;
}

static SE05x_RSASignatureAlgo_t se05x_get_rsa_sign_hash_mode(
    sss_algorithm_t algorithm)
{
    SE05x_RSASignatureAlgo_t mode;
    switch (algorithm) {
    case kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA224:
        mode = kSE05x_RSASignatureAlgo_SHA_224_PKCS1;
        break;
    case kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA256:
        mode = kSE05x_RSASignatureAlgo_SHA_256_PKCS1;
        break;
    case kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA384:
        mode = kSE05x_RSASignatureAlgo_SHA_384_PKCS1;
        break;
    case kAlgorithm_SSS_RSASSA_PKCS1_V1_5_SHA512:
        mode = kSE05x_RSASignatureAlgo_SHA_512_PKCS1;
        break;
    case kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA1:
        mode = kSE05x_RSASignatureAlgo_SHA1_PKCS1_PSS;
        break;
    case kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA224:
        mode = kSE05x_RSASignatureAlgo_SHA224_PKCS1_PSS;
        break;
    case kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA256:
        mode = kSE05x_RSASignatureAlgo_SHA256_PKCS1_PSS;
        break;
    case kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA384:
        mode = kSE05x_RSASignatureAlgo_SHA384_PKCS1_PSS;
        break;
    case kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA512:
        mode = kSE05x_RSASignatureAlgo_SHA512_PKCS1_PSS;
        break;
    default:
        mode = kSE05x_RSASignatureAlgo_NA;
    }
    return mode;
}

static SE05x_CipherMode_t se05x_get_cipher_mode(sss_algorithm_t algorithm)
{
    SE05x_CipherMode_t mode;
    switch (algorithm) {
    case kAlgorithm_SSS_AES_ECB:
        mode = kSE05x_CipherMode_AES_ECB_NOPAD;
        break;
    case kAlgorithm_SSS_DES_ECB:
        mode = kSE05x_CipherMode_DES_ECB_NOPAD;
        break;
    case kAlgorithm_SSS_AES_CBC:
        mode = kSE05x_CipherMode_AES_CBC_NOPAD;
        break;
    case kAlgorithm_SSS_DES_CBC:
        mode = kSE05x_CipherMode_DES_CBC_NOPAD;
        break;
    case kAlgorithm_SSS_AES_CTR:
        mode = kSE05x_CipherMode_AES_CTR;
        break;
    default:
        mode = 0;
    }
    return mode;
}

static SE05x_MACAlgo_t se05x_get_mac_algo(sss_algorithm_t algorithm)
{
    SE05x_MACAlgo_t mode;
    switch (algorithm) {
    case kAlgorithm_SSS_CMAC_AES:
        mode = kSE05x_MACAlgo_CMAC_128;
        break;
    case kAlgorithm_SSS_HMAC_SHA1:
        mode = kSE05x_MACAlgo_HMAC_SHA1;
        break;
    case kAlgorithm_SSS_HMAC_SHA256:
        mode = kSE05x_MACAlgo_HMAC_SHA256;
        break;
    case kAlgorithm_SSS_HMAC_SHA384:
        mode = kSE05x_MACAlgo_HMAC_SHA384;
        break;
    case kAlgorithm_SSS_HMAC_SHA512:
        mode = kSE05x_MACAlgo_HMAC_SHA512;
        break;
    default:
        mode = 0;
    }
    return mode;
}

static SE05x_DigestMode_t se05x_get_sha_algo(sss_algorithm_t algorithm)
{
    SE05x_DigestMode_t sha_type;

    switch (algorithm) {
    case kAlgorithm_SSS_SHA1:
    case kAlgorithm_SSS_HMAC_SHA1:
        sha_type = kSE05x_DigestMode_SHA;
        break;
    case kAlgorithm_SSS_SHA224:
        sha_type = kSE05x_DigestMode_SHA224;
        break;
    case kAlgorithm_SSS_SHA256:
    case kAlgorithm_SSS_HMAC_SHA256:
        sha_type = kSE05x_DigestMode_SHA256;
        break;
    case kAlgorithm_SSS_SHA384:
    case kAlgorithm_SSS_HMAC_SHA384:
        sha_type = kSE05x_DigestMode_SHA384;
        break;
    case kAlgorithm_SSS_SHA512:
    case kAlgorithm_SSS_HMAC_SHA512:
        sha_type = kSE05x_DigestMode_SHA512;
        break;
    default:
        sha_type = 0x00;
    }

    return sha_type;
}

#ifdef __cplusplus
}
#endif

#endif /* SSS_HAVE_SE05X */
