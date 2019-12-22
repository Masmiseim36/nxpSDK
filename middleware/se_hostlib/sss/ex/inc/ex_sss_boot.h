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
 * ex_sss_boot.h:  *The purpose and scope of this file*
 *
 * Project:  SecureIoTMW-Debug@appboot-top-eclipse_x86
 *
 * $Date: Mar 10, 2019 $
 * $Author: ing05193 $
 * $Revision$
 */

#ifndef SSS_EX_INC_EX_SSS_BOOT_H_
#define SSS_EX_INC_EX_SSS_BOOT_H_

/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */

#ifdef __cplusplus
extern "C" {
#endif

#include "ex_sss.h"
#include "fsl_sss_api.h"

#if SSS_HAVE_SE05X
#include "fsl_sss_se05x_types.h"
#endif
#include "ex_sss_ports.h"
#include "nxScp03_Types.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */
#if SSS_HAVE_SE || SSS_HAVE_SE05X

typedef union ex_auth {
    struct
    {
        NXSCP03_StaticCtx_t ex_static; //!< .static keys data
        NXSCP03_DynCtx_t ex_dyn;       //!<  session keys data
    } scp03;
    struct
    {
        NXFastSCP03_StaticCtx_t ex_static; //!< .static keys data
        NXSCP03_DynCtx_t ex_dyn;           //!<  session keys data
    } fastscp;
    struct
    {
        sss_object_t ex_id;
    } id;
} ex_SE05x_authCtx_t;
#endif

typedef struct
{
    sss_session_t session;
    sss_key_store_t ks;

#if SSS_HAVE_SE || SSS_HAVE_SE05X
    sss_session_t host_session;
    sss_key_store_t host_ks;
    SE_Connect_Ctx_t se05x_open_ctx;
    sss_tunnel_t *pTunnel_ctx;
    ex_SE05x_authCtx_t ex_se05x_auth;
#endif
#if SSS_HAVE_SSCP
    sss_session_t host_session;
    sss_key_store_t host_ks;
    sscp_context_t sscp_ctx;
#endif

} ex_sss_boot_ctx_t;

#if SSS_HAVE_SE05X
typedef struct
{
    sss_session_t platf_session;
    SE05x_Connect_Ctx_t platf_open_ctx;
    sss_session_t *phost_session;
    sss_key_store_t *phost_ks;
    ex_SE05x_authCtx_t ex_se05x_auth;
} ex_sss_platf_ctx_t;
#endif

typedef struct
{
    sss_object_t pub_obj;
    sss_object_t obj;
    sss_object_t dev_cert;
    sss_object_t interCaCert;
    sss_key_store_t *pHost_ks;
} ex_sss_cloud_ctx_t;

/* *****************************************************************************************************************
 *   Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Function Prototypes
 * ***************************************************************************************************************** */

#if SSS_HAVE_SE05X

sss_status_t ex_sss_se05x_prepare_host(sss_session_t *host_session,
    sss_key_store_t *host_ks,
    SE05x_Connect_Ctx_t *se05x_open_ctx,
    ex_SE05x_authCtx_t *ex_se05x_authctx,
    SE_AuthType_t auth_type);

/* Prepare host for multiple user sessions */
sss_status_t ex_sss_se05x_prepare_host_keys(sss_session_t *pHostSession,
    sss_key_store_t *pHostKs,
    SE_Connect_Ctx_t *pConnectCtx,
    ex_SE05x_authCtx_t *se05x_auth_ctx,
    uint32_t offset);
#endif

#if SSS_HAVE_SE
sss_status_t ex_sss_se_prepare_host(sss_session_t *host_session,
    sss_key_store_t *host_ks,
    SE_Connect_Ctx_t *se05x_open_ctx,
    ex_SE05x_authCtx_t *ex_se05x_authctx,
    SE_AuthType_t auth_type);
#endif

/** The case where we connect to the cyrptogrpahic system directly.
 *
 * e.g. when running form an embedded sytem, without any choice of Port Numbers, etc.
 */
sss_status_t ex_sss_boot_direct(void);

/** The case where we connect to the cyrptogrpahic system in-directly.
 *
 * This function is a similar to @ref ex_sss_boot_direct.
 *
 * This function expects that the last argument in argv is the
 * expected/probable port name.
 *
 * e.g. when running form PC, where we are connected
 * to secure element via a COM Port/Socket Port.  In such cases,
 * taking the Port number from a Command Line Argument,
 * or Environment Variable would make sense and examples
 * would become more portable.
 *
 * @param argc count of parameters, as received by main
 * @param argv Array of argv, as received by main
 * @param[out] pPortName Possible port name
 * @return 0 if successful.
 */
sss_status_t ex_sss_boot_connectstring(
    int argc, const char *argv[], const char **pPortName);

/**
 * For the case where few activities have to be performed
 * after RTOS initialization, this API would be executed
 * as an RTOS Task.
 *
 * @return
 */
sss_status_t ex_sss_boot_rtos(void *);

/** Is this a serail port */
bool ex_sss_boot_isSerialPortName(const char *portName);

/** Is this a socket port */
bool ex_sss_boot_isSocketPortName(const char *portName);

/** Open an example session */
sss_status_t ex_sss_boot_open(ex_sss_boot_ctx_t *pCtx, const char *portName);

/** Open an example session */
sss_status_t ex_sss_boot_factory_reset(ex_sss_boot_ctx_t *pCtx);

/** Close an example session */
void ex_sss_session_close(ex_sss_boot_ctx_t *pCtx);

/** Entry Point for each example */
sss_status_t ex_sss_entry(ex_sss_boot_ctx_t *pCtx);

sss_status_t ex_sss_kestore_and_object_init(ex_sss_boot_ctx_t *pCtx);

int ex_sss_boot_rtos_init(void);

sss_status_t ex_sss_boot_open_host_session(ex_sss_boot_ctx_t *pCtx);

#if defined(__cplusplus)
}
#endif

#endif /* SSS_EX_INC_EX_SSS_BOOT_H_ */
