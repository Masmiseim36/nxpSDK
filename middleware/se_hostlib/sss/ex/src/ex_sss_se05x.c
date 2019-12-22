/*
 * Copyright 2018,2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* ************************************************************************** */
/* Includes                                                                   */
/* ************************************************************************** */

#include <ex_sss_boot.h>
#include <fsl_sss_se05x_types.h>
#include <nxLog_App.h>
#include <se05x_APDU.h>
#include <sm_const.h>
#include <stdio.h>

#include "ex_sss_boot_int.h"
#if AX_EMBEDDED
#include <app_boot.h>
#endif

#include "ex_sss_auth.h"

/* ************************************************************************** */
/* Local Defines                                                              */
/* ************************************************************************** */
/* clang-format off */
#if SSS_HAVE_SE05X
const uint8_t se050Authkey[] = EX_SSS_AUTH_SE05X_UserID_VALUE;
#endif
/* clang-format on */
/* ************************************************************************** */
/* Structures and Typedefs                                                    */
/* ************************************************************************** */

/* ************************************************************************** */
/* Global Variables                                                           */
/* ************************************************************************** */
#if SSS_HAVE_SE05X

#if defined(EXFL_SE050_AUTH_UserID)
#define SSS_EX_SE05x_AUTH_MECH kSSS_AuthType_ID
#define SSS_EX_SE05x_AUTH_ID kEX_SSS_ObjID_UserID_Auth
#define SSS_EX_CONNECTION_TYPE kSSS_ConnectionType_Password
#endif

#if defined(EXFL_SE050_AUTH_PlatfSCP03)
#define SSS_EX_SE05x_AUTH_MECH kSSS_AuthType_SCP03
#define SSS_EX_CONNECTION_TYPE kSSS_ConnectionType_Encrypted
#endif

#if defined(EXFL_SE050_AUTH_UserID_PlatfSCP03)
#define SSS_EX_SE05x_AUTH_MECH kSSS_AuthType_SCP03
#define SSS_EX_SE05x_TUNN_AUTH_MECH kSSS_AuthType_ID
#define SSS_EX_SE05x_AUTH_ID kEX_SSS_ObjID_UserID_Auth
#define SSS_EX_CONNECTION_TYPE kSSS_ConnectionType_Encrypted
#endif

#if defined(EXFL_SE050_AUTH_AppletSCP03_PlatfSCP03)
#define SSS_EX_SE05x_AUTH_MECH kSSS_AuthType_SCP03
#define SSS_EX_SE05x_TUNN_AUTH_MECH kSSS_AuthType_AppletSCP03
#define SSS_EX_SE05x_AUTH_ID kEX_SSS_ObjID_APPLETSCP03_Auth
#define SSS_EX_CONNECTION_TYPE kSSS_ConnectionType_Encrypted
#endif

#if defined(EXFL_SE050_AUTH_FastSCP_PlatfSCP03)
#define SSS_EX_SE05x_AUTH_MECH kSSS_AuthType_SCP03
#define SSS_EX_SE05x_TUNN_AUTH_MECH kSSS_AuthType_FastSCP
#define SSS_EX_SE05x_AUTH_ID kEX_SSS_objID_FASTSCP_Auth
#define SSS_EX_CONNECTION_TYPE kSSS_ConnectionType_Encrypted
#endif

#if defined(EXFL_SE050_AUTH_AppletSCP03)
#define SSS_EX_SE05x_AUTH_MECH kSSS_AuthType_AppletSCP03
#define SSS_EX_SE05x_AUTH_ID kEX_SSS_ObjID_APPLETSCP03_Auth
#define SSS_EX_CONNECTION_TYPE kSSS_ConnectionType_Encrypted
#endif

#if defined(EXFL_SE050_AUTH_FastSCP)
#define SSS_EX_SE05x_AUTH_MECH kSSS_AuthType_FastSCP
#define SSS_EX_SE05x_AUTH_ID kEX_SSS_objID_FASTSCP_Auth
#define SSS_EX_CONNECTION_TYPE kSSS_ConnectionType_Encrypted
#endif

#if defined(EXFL_SE050_AUTH_NONE)
#define SSS_EX_SE05x_AUTH_MECH kSSS_AuthType_None
#define SSS_EX_CONNECTION_TYPE kSSS_ConnectionType_Plain
#endif

#ifndef SSS_EX_SE05x_AUTH_MECH
#define SSS_EX_SE05x_AUTH_MECH kSSS_AuthType_None
#endif

#ifndef SSS_EX_CONNECTION_TYPE
#define SSS_EX_CONNECTION_TYPE kSSS_ConnectionType_Plain
#endif

#ifndef SSS_EX_SE05x_TUNN_AUTH_MECH
#define SSS_EX_SE05x_TUNN_AUTH_MECH kSSS_AuthType_None
#else
/* Only define if using Tunnel*/
sss_tunnel_t gTunnel_ctx;
ex_sss_platf_ctx_t gPlatfCtx;
#endif

/* ************************************************************************** */
/* Static function declarations                                               */
/* ************************************************************************** */

/* ************************************************************************** */
/* Public Functions                                                           */
/* ************************************************************************** */

/* ************************************************************************** */
/* Private Functions                                                          */
/* ************************************************************************** */

sss_status_t ex_sss_boot_se05x_open(
    ex_sss_boot_ctx_t *pCtx, const char *portName)
{
    sss_status_t status = kStatus_SSS_Fail;
    SE_Connect_Ctx_t *pConnectCtx = NULL;
    sss_session_t *pSession = NULL;
#if defined(EXFL_SE050_AUTH_UserID_PlatfSCP03) ||      \
    defined(EXFL_SE050_AUTH_AppletSCP03_PlatfSCP03) || \
    defined(EXFL_SE050_AUTH_FastSCP_PlatfSCP03)
    sss_connection_type_t connectType = kSSS_ConnectionType_Plain;
#endif

#if defined SSS_EX_SE05x_AUTH_ID
    const uint32_t auth_id = SSS_EX_SE05x_AUTH_ID;
#endif

#if defined(EXFL_SE050_AUTH_UserID_PlatfSCP03) ||      \
    defined(EXFL_SE050_AUTH_AppletSCP03_PlatfSCP03) || \
    defined(EXFL_SE050_AUTH_FastSCP_PlatfSCP03)
    ex_sss_platf_ctx_t *pPlatfCtx = &gPlatfCtx;

    pCtx->pTunnel_ctx = &gTunnel_ctx;
    pPlatfCtx->phost_session = &pCtx->host_session;
    pPlatfCtx->phost_ks = &pCtx->host_ks;
    pSession = &pPlatfCtx->platf_session;
    pConnectCtx = &pPlatfCtx->platf_open_ctx;
    pConnectCtx->auth.ctx.scp03.pStatic_ctx =
        &pPlatfCtx->ex_se05x_auth.scp03.ex_static;
    pConnectCtx->auth.ctx.scp03.pDyn_ctx =
        &pPlatfCtx->ex_se05x_auth.scp03.ex_dyn;

#else
    pSession = &pCtx->session;
    pConnectCtx = &pCtx->se05x_open_ctx;
#endif

#if defined(SMCOM_JRCP_V1)
    if (ex_sss_boot_isSocketPortName(portName)) {
        pConnectCtx->connType = kType_SE_Conn_Type_JRCP_V1;
        pConnectCtx->portName = portName;
    }
#endif

#if defined(SMCOM_JRCP_V2)
    if (ex_sss_boot_isSocketPortName(portName)) {
        pConnectCtx->connType = kType_SE_Conn_Type_JRCP_V2;
        pConnectCtx->portName = portName;
    }
#endif

#if defined(RJCT_VCOM)
    if (ex_sss_boot_isSerialPortName(portName)) {
        pConnectCtx->connType = kType_SE_Conn_Type_VCOM;
        pConnectCtx->portName = portName;
    }
#endif

#if defined(SCI2C)
#error "Not a valid  combination"
#endif

#if defined(T1oI2C)
    pConnectCtx->connType = kType_SE_Conn_Type_T1oI2C;
    pConnectCtx->portName = NULL;
#endif

#if defined(SMCOM_PCSC)
    pConnectCtx->connType = kType_SE_Conn_Type_PCSC;
    pConnectCtx->portName = NULL;
#endif

#if defined(SMCOM_PN7150)
    pConnectCtx->connType = kType_SE_Conn_Type_NFC;
    pConnectCtx->portName = NULL;
#endif

    status = ex_sss_se05x_prepare_host(&pCtx->host_session,
        &pCtx->host_ks,
        pConnectCtx,
        &pCtx->ex_se05x_auth,
        SSS_EX_SE05x_AUTH_MECH);
    if (kStatus_SSS_Success != status) {
        LOG_E("ex_sss_se05x_prepare_host failed");
        goto cleanup;
    }

    if (SSS_EX_SE05x_AUTH_MECH == kSSS_AuthType_SCP03 ||
        SSS_EX_SE05x_AUTH_MECH == kSSS_AuthType_None) {
        status = sss_session_open(pSession,
            kType_SSS_SE_SE05x,
            0,
            SSS_EX_CONNECTION_TYPE,
            pConnectCtx);
        if (kStatus_SSS_Success != status) {
            LOG_E("sss_session_open failed");
            goto cleanup;
        }
    }
#ifdef SSS_EX_SE05x_AUTH_ID
    else {
        status = sss_session_open(pSession,
            kType_SSS_SE_SE05x,
            auth_id,
            SSS_EX_CONNECTION_TYPE,
            pConnectCtx);
        if (kStatus_SSS_Success != status) {
            LOG_E("sss_session_open failed");
        }
    }
#else
    else {
        LOG_E("Invalid combination for boot selection");
        status = kStatus_SSS_Fail;
    }
#endif /* SSS_EX_SE05x_AUTH_ID */

#if defined(EXFL_SE050_AUTH_UserID_PlatfSCP03) ||      \
    defined(EXFL_SE050_AUTH_AppletSCP03_PlatfSCP03) || \
    defined(EXFL_SE050_AUTH_FastSCP_PlatfSCP03)
    SE05x_Connect_Ctx_t *pchannlCtxt = &pCtx->se05x_open_ctx;
    pchannlCtxt->auth.authType = SSS_EX_SE05x_TUNN_AUTH_MECH;

    status = ex_sss_se05x_prepare_host(&pCtx->host_session,
        &pCtx->host_ks,
        pchannlCtxt,
        &pPlatfCtx->ex_se05x_auth,
        SSS_EX_SE05x_TUNN_AUTH_MECH);
    if (kStatus_SSS_Success != status) {
        LOG_E("ex_sss_se05x_prepare_host failed");
        goto cleanup;
    }

    status = sss_tunnel_context_init(pCtx->pTunnel_ctx, pSession /* session */);
    if (kStatus_SSS_Success != status) {
        LOG_E("sss_tunnel_context_init failed");
        goto cleanup;
    }

    pchannlCtxt->connType = kType_SE_Conn_Type_Channel;
    pchannlCtxt->tunnelCtx = pCtx->pTunnel_ctx;
    if (pchannlCtxt->auth.authType == kSSS_AuthType_ID) {
        connectType = kSSS_ConnectionType_Password;
    }
    else {
        connectType = kSSS_ConnectionType_Encrypted;
    }
    status = sss_session_open(
        &pCtx->session, kType_SSS_SE_SE05x, auth_id, connectType, pchannlCtxt);
    if (kStatus_SSS_Success != status) {
        LOG_E("sss_session_open failed");
    }
#endif

cleanup:
    return status;
}

#endif /* SSS_HAVE_SE05X */
