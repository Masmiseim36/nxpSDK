/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#include <nxp_iot_agent_platform.h>
#include <nxp_iot_agent_session.h>
#include <nxp_iot_agent_macros.h>

static ex_sss_boot_ctx_t gex_sss_demo_boot_ctx;

iot_agent_status_t iot_agent_platform_init(int argc, const char *argv[], iot_agent_platform_context_t* platform_context)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;

	ASSERT_OR_EXIT_MSG(platform_context != NULL, "The platform context is null");

	agent_status = iot_agent_session_init(argc, argv, &gex_sss_demo_boot_ctx);
	AGENT_SUCCESS_OR_EXIT();

	platform_context->ctx = &gex_sss_demo_boot_ctx;

exit:
	return agent_status;
}
