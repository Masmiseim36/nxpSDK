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
 * ex_sss_boot.c:  *The purpose and scope of this file*
 *
 * Project:  SecureIoTMW-Debug@appboot-top-eclipse_x86
 *
 * $Date: Mar 10, 2019 $
 * $Author: ing05193 $
 * $Revision$
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */

#ifdef __cplusplus
extern "C" {
#endif

#include "ex_sss_boot.h"

#include <ex_sss.h>
#include <string.h>

#include "ex_sss_boot_int.h"
#include "nxLog_App.h"
#include "stdio.h"

#if defined(SSS_USE_FTR_FILE)
#include "fsl_sss_ftr.h"
#else
#include "fsl_sss_ftr_default.h"
#endif

#if SSS_HAVE_A71CH || SSS_HAVE_SE050_EAR_CH
#endif

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

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

sss_status_t ex_sss_boot_open(ex_sss_boot_ctx_t *pCtx, const char *portName)
{
    sss_status_t status = kStatus_SSS_Fail;

#if SSS_HAVE_A71CH || SSS_HAVE_SE050_EAR_CH
    status = ex_sss_boot_a71ch_open(pCtx, portName);
#elif SSS_HAVE_A71CL || SSS_HAVE_SE050_L
    status = ex_sss_boot_a71cl_open(pCtx, portName);
#elif SSS_HAVE_SE050M
    status = ex_sss_boot_se_open(pCtx, portName);
#elif SSS_HAVE_SE05X
    status = ex_sss_boot_se05x_open(pCtx, portName);
#elif SSS_HAVE_SE
    status = ex_sss_boot_se_open(pCtx, portName);
#elif SSS_HAVE_MBEDTLS
    status = ex_sss_boot_mbedtls_open(pCtx, portName);
#elif SSS_HAVE_OPENSSL
    status = ex_sss_boot_openssl_open(pCtx, portName);
#endif
    return status;
}

sss_status_t ex_sss_boot_factory_reset(ex_sss_boot_ctx_t *pCtx)
{
    sss_status_t status = kStatus_SSS_Fail;

#if SSS_HAVE_A71CH || SSS_HAVE_SE050_EAR_CH
    uint16_t ret;
    ret = HLSE_DbgReset();
    if (ret == HLSE_SW_OK)
        status = kStatus_SSS_Success;

#elif SSS_HAVE_A71CL || SSS_HAVE_SE050_L
    status = kStatus_SSS_Success;

#elif SSS_HAVE_SE05X
    smStatus_t st;
    sss_se05x_session_t *pSession = (sss_se05x_session_t *)&pCtx->session;
    st = Se05x_API_DeleteAll_Iterative(&pSession->s_ctx);
    if (st == SW_OK)
        status = kStatus_SSS_Success;

#elif SSS_HAVE_MBEDTLS
    status = kStatus_SSS_Success;
#elif SSS_HAVE_OPENSSL
    status = kStatus_SSS_Success;
#else
    LOG_E("Select atleast one security subsystem");
#endif
    return status;
}

sss_status_t ex_sss_kestore_and_object_init(ex_sss_boot_ctx_t *pCtx)
{
    sss_status_t status;
    status = sss_key_store_context_init(&pCtx->ks, &pCtx->session);
    if (status != kStatus_SSS_Success) {
        LOG_E(" sss_key_store_context_init Failed...");
        goto cleanup;
    }

    status = sss_key_store_allocate(&pCtx->ks, __LINE__);
    if (status != kStatus_SSS_Success) {
        LOG_E(" sss_key_store_allocate Failed...");
        goto cleanup;
    }

cleanup:
    return status;
}

void ex_sss_session_close(ex_sss_boot_ctx_t *pCtx)
{
#if SSS_HAVE_SE05X || SSS_HAVE_SSCP
    if (pCtx->session.subsystem != kType_SSS_SubSystem_NONE) {
        sss_session_close(&pCtx->session);
        sss_session_delete(&pCtx->session);
    }

    if (pCtx->host_ks.session != NULL) {
        sss_key_store_context_free(&pCtx->host_ks);
    }
    if (pCtx->host_session.subsystem != kType_SSS_SubSystem_NONE) {
        sss_session_close(&pCtx->host_session);
    }
#endif

    if (pCtx->ks.session != NULL) {
        sss_key_store_context_free(&pCtx->ks);
    }
}

sss_status_t ex_sss_boot_open_host_session(ex_sss_boot_ctx_t *pCtx)
{
    sss_status_t status = kStatus_SSS_Fail;

#if SSS_HAVE_SE05X || SSS_HAVE_SSCP
    if (pCtx->host_ks.session == NULL) {
        status = sss_session_open(&pCtx->host_session,
#if SSS_HAVE_MBEDTLS
            kType_SSS_mbedTLS,
#else
            kType_SSS_OpenSSL,
#endif
            0,
            kSSS_ConnectionType_Plain,
            NULL);
        if (kStatus_SSS_Success != status) {
            LOG_E("Failed to open mbedtls Session");
            return status;
        }

        status =
            sss_key_store_context_init(&pCtx->host_ks, &pCtx->host_session);
        if (kStatus_SSS_Success != status) {
            LOG_E("sss_key_store_context_init failed");
            return status;
        }
        status = sss_key_store_allocate(&pCtx->host_ks, __LINE__);
        if (kStatus_SSS_Success != status) {
            LOG_E("sss_key_store_allocate failed");
            return status;
        }
    }
#endif

    return status;
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

#ifdef __cplusplus
}
#endif
