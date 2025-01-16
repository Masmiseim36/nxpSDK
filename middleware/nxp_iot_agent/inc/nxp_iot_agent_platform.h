/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

/** @file */

#ifndef _NXP_IOT_AGENT_PLATFORM_H_
#define _NXP_IOT_AGENT_PLATFORM_H_

#include <nxp_iot_agent_status.h>

/*!@defgroup edgelock2go_agent_platform EdgeLock 2GO agent platform specific functionality.
 * @ingroup edgelock2go_agent
 *
 * @brief Functions interacting with platform specific functionalities.
 */


/*!
 * @addtogroup edgelock2go_agent_platform
 * @{
 */

/**
 * @brief The platform context.
 */
typedef struct iot_agent_platform_context_t
{
	void* ctx;
} iot_agent_platform_context_t;

/** @brief Destroy the keystore.
 *
 * Depending on the type of the keystore this is triggering actions on the keystore's context
 * itself by calling the _destroy() function of the keystore interface.
 *
 * The \p keystore is not usable after a call to iot_agent_keystore_free.
 */
iot_agent_status_t iot_agent_platform_init(int argc, const char *argv[], iot_agent_platform_context_t* platform_context);

/*!
*@}
*/ /* end of edgelock2go_agent_keystore */

#endif // _NXP_IOT_AGENT_PLATFORM_H_
