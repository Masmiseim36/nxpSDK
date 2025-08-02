/*
 * Copyright 2023-2025 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */
#ifndef _IOT_AGENT_CLAIMCODE_IMPORT_H_
#define _IOT_AGENT_CLAIMCODE_IMPORT_H_

#include <nxp_iot_agent.h>
#include <nxp_iot_agent_config.h>

#if defined(NXP_IOT_AGENT_HAVE_PSA_IMPL_TFM) && (NXP_IOT_AGENT_HAVE_PSA_IMPL_TFM == 1)

#ifdef __cplusplus
extern "C" {
#endif

/*! @defgroup edgelock2go_agent_claimcode_import Import claimcode into TF-M internal trusted storage.
*
* @ingroup edgelock2go_agent_claimcode_import
*
* @brief Provides functionality to import claimcode as a binary object into TF-M internal trusted storage.
*
*/

/*!
* @addtogroup edgelock2go_agent_claimcode_import
* @{
*/

/**
 * @brief Import claimcode into TF_M internal trusted storage.
 * @return Success if import of claimcode is successful.
 */
iot_agent_status_t iot_agent_claimcode_import(void);

#ifdef __cplusplus
} // extern "C"
#endif

/*!
*@}
*/ /* end of edgelock2go_agent_claimcode_import */

#endif //#if defined(NXP_IOT_AGENT_HAVE_PSA_IMPL_TFM) && (NXP_IOT_AGENT_HAVE_PSA_IMPL_TFM == 1)

#endif // #ifndef _IOT_AGENT_CLAIMCODE_IMPORT_H_
