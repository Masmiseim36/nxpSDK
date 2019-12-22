/*
 * Copyright 2018,2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_SSCP_A71CH_H_
#define _FSL_SSCP_A71CH_H_

#include "fsl_sscp.h"
#include "fsl_sss_sscp.h"

#if SSS_HAVE_A71XX || SSS_HAVE_SE050_EAR

#include <HLSEAPI.h>
#include <fsl_sss_keyid_map.h>
#include <nxScp03_Types.h>
#include <sm_api.h>

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * struct _sscp_a71ch_context - SSCP context struct for A71 CH implementation
 *
 * This data type is used to keep context of the SSCP link.
 * It is completely implementation specific.
 *
 * @param context Container for the implementation specific data.
 */
typedef struct _sss_a71ch_key_store
{
    sss_sscp_session_t *session;
    /*! Implementation specific part */

    keyStoreTable_t *keystore_shadow;
    HLSE_OBJECT_HANDLE shadow_handle;

} sss_a71ch_key_store_t;

typedef struct _sscp_a71ch_context
{
    fn_sscp_invoke_command_t invoke;
    /*! Implementation specific part */
    sss_a71ch_key_store_t *keyStore;
} sscp_a71ch_context_t;

/*! @brief Initializes the SSCP link
 *
 * This function initializes the SSCP for operation - e.g.underlaying hardware
 * is initialized and prepared for data exchange.
 *
 * @param context Context structure for the SSCP.
 * @param base The A71ch peripheral base address to be used for communication
 *
 * @returns Status of the operation
 * @retval kStatus_SSCP_Success SSCP init success
 * @retval kStatus_SSCP_Fail SSCP init failure
 */
sss_status_t sscp_a71ch_init(
    sscp_a71ch_context_t *context, sss_a71ch_key_store_t *keyStore);

/*! @brief Close the SSCP link
 *
 * This function closes the SSCP link - e.g.underlying hardware is disabled.
 *
 * @param context Context structure for the SSCP.
 */
void sscp_a71ch_free(sscp_a71ch_context_t *context);

/*! @brief Sends a command and associated parameters to security sub-system
 *
 *  The commandID and operation content is serialized and sent over to the
 * selected security sub-system. This is implementation specific function. The
 * function can invoke both blocking and non-blocking secure functions in the
 * selected security sub-system.
 *
 * @param context Initialized SSCP context
 * @param commandID Command - an id of a remote secure function to be invoked
 * @param op Description of function arguments as a sequence of buffers and
 * values
 * @param ret Return code of the remote secure function (application layer
 * return value)
 *
 * @returns Status of the operation
 * @retval kStatus_SSCP_Success A blocking command has completed or a
 * non-blocking command has been accepted.
 * @retval kStatus_SSCP_Fail Operation failure, for example hardware fail.
 * @retval kStatus_SSCP_InvalidArgument One of the arguments is invalid for the
 * function to execute.
 */
sscp_status_t sscp_a71ch_invoke_command(sscp_context_t *context,
    uint32_t commandID,
    sscp_operation_t *op,
    uint32_t *ret);

sss_status_t sscp_a71ch_openSession(
    void *connectionData, sss_sscp_session_t *openSession);

void sscp_a71chkey_store_context_free(sss_a71ch_key_store_t *context);

void sscp_a71ch_closeSession(sss_sscp_session_t *closeSession);
void sscp_a71ch_closeConnect(void);

#if defined(__cplusplus)
}
#endif

/*!
 *@}
 */ /* end of sscp_a71ch */

#endif /* SSS_HAVE_A71CH */
#endif /* _FSL_SSCP_A71CH_H_ */
