/*
 * Copyright 2020-2022 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */
#ifndef _IOT_AGENT_CLAIMCODE_INJECT_H_
#define _IOT_AGENT_CLAIMCODE_INJECT_H_

#include <nxp_iot_agent.h>
#include <nxp_iot_agent_config.h>

#if NXP_IOT_AGENT_HAVE_SSS

#include <ex_sss_boot.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! @defgroup edgelock2go_agent_claimcode_inject Inject claimcode as a binary object into secure element
*
* @ingroup edgelock2go_agent_claimcode_inject
*
* @brief Provides functionality to inject claimcode as a binary object into secure element.
*
*/

/*!
* @addtogroup edgelock2go_agent_claimcode_inject
* @{
*/

#define CLAIMCODE_OBJ_ID    		0xF00000E0U
#define DEFAULT_USER_ID     		0x00000000U
/*FastSCP autentication keyID for demo devices*/
#define EDGELOCK2GO_USER_ID_DEMO 	0xF0000021U
/*FastSCP autentication keyID for production devices*/
#define EDGELOCK2GO_USER_ID      	0xF0000020U

/**
 * @brief Inject claimcode into secure element.
 * @param[in] pCtx: Pointer to boot context.
 * @param[in] claimcode: Pointer to string containing claimcode.
 * @param[in] claimcode_len: Length of the claimcode.
 * @return Success if injection of new claimcode is successful.
 */
iot_agent_status_t iot_agent_claimcode_inject(ex_sss_boot_ctx_t *pCtx, char *claimcode, size_t claimcode_len);

#ifdef __cplusplus
} // extern "C"
#endif

/*!
*@}
*/ /* end of edgelock2go_agent_claimcode_inject */

#endif // NXP_IOT_AGENT_HAVE_SSS

#endif // #ifndef _IOT_AGENT_CLAIMCODE_INJECT_H_
