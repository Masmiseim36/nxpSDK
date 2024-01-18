/* 
 * Copyright 2018-2023 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 * 
 */

/** @file */

#ifndef _NXP_IOT_AGENT_SESSION_H_
#define _NXP_IOT_AGENT_SESSION_H_

#include <nxp_iot_agent_common.h>
#include <nxp_iot_agent_utils.h>

#if NXP_IOT_AGENT_HAVE_SSS

#include <fsl_sss_api.h>
#include <nxLog_App.h>
#include <ex_sss_boot.h>
#include <sm_types.h>

#endif

#ifdef __cplusplus
extern "C" {
#endif

/*! @defgroup edgelock2go_agent_session Session handling functions for the EdgeLock 2GO agent.
 * @ingroup edgelock2go_agent
 *
 * @brief Session handling functions for the EdgeLock 2GO agent. When interacting with the OpenSSL engine, the agent session needs to be closed before and opened after OpenSSL is active.
 *
 *
 * @addtogroup edgelock2go_agent_session
 * @{
 */

#if AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1

void iot_agent_session_bm(void);

void iot_agent_session_boot_rtos_task(void);

void iot_agent_session_led_success(void);

void iot_agent_session_led_failure(void);

void iot_agent_session_led_start(void);

#endif

#if NXP_IOT_AGENT_HAVE_SSS
/**
 * Create an open a session with secure element
 *
 * @param[in] argc  arguments from command-line if any
 * @param[in] argv  arguments from command-line if any
 * @param[in] pCtx  pointer to session context

 * @retval IOT_AGENT_SUCCESS upon success
 * @retval IOT_AGENT_FAILURE upon failure
 */
iot_agent_status_t iot_agent_session_init(int argc, const char *argv[], ex_sss_boot_ctx_t *pCtx);

/**
* Re-create an open a session with secure element
*
* @param[in] pCtx  pointer to session context

* @retval IOT_AGENT_SUCCESS upon success
* @retval IOT_AGENT_FAILURE upon failure
*/
iot_agent_status_t iot_agent_session_connect(ex_sss_boot_ctx_t * pSeBootCtx);

/**
* Disconnect and close session with secure element
*
* @param[in] pCtx  pointer to session context
*/
void iot_agent_session_disconnect(ex_sss_boot_ctx_t * pSeBootCtx);

#ifdef __cplusplus
} // extern "C"
#endif
  /*!
  *@}
  */ /* end of edgelock2go_agent_session */

#endif // NXP_IOT_AGENT_HAVE_SSS

#endif // #ifndef _NXP_IOT_AGENT_SESSION_H_
