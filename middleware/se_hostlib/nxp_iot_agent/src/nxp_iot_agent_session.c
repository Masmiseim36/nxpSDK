/*
 * Copyright 2018, 2019, 2020, 2021 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#include <nxp_iot_agent_session.h>

#if defined(FRDM_KW41Z) || defined(FRDM_K64F) || defined(IMX_RT) || \
    defined(LPC_55x)
#define HAVE_KSDK
#endif

#ifdef HAVE_KSDK
#include "ex_sss_main_inc_ksdk.h"
#endif

#include <fsl_sss_api.h>

#include <nxp_iot_agent.h>
#include <nxp_iot_agent_service.h>
#include <nxp_iot_agent_macros.h>

iot_agent_status_t iot_agent_session_connect(ex_sss_boot_ctx_t *pCtx)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	sss_status_t sss_status;
	const char *portName;

    sss_status = ex_sss_boot_connectstring(0, NULL, &portName);
	SSS_SUCCESS_OR_EXIT_MSG("ex_sss_boot_connectstring returned with 0x%04x", sss_status);

    sss_status = ex_sss_boot_open(pCtx, portName);
	SSS_SUCCESS_OR_EXIT_MSG("ex_sss_boot_open returned with 0x%04x", sss_status);

	sss_status = ex_sss_key_store_and_object_init(pCtx);
	SSS_SUCCESS_OR_EXIT_MSG("ex_sss_key_store_and_object_init returned with 0x%04x", sss_status);

exit:
	return agent_status;
}

void iot_agent_session_disconnect(ex_sss_boot_ctx_t *pCtx) {
    ex_sss_session_close(pCtx);
}


#if AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1
void iot_agent_session_bm(void)
{
#ifdef HAVE_KSDK
	ex_sss_main_ksdk_bm();
#endif
}

void iot_agent_session_boot_rtos_task(void)
{
#ifdef HAVE_KSDK
    ex_sss_main_ksdk_boot_rtos_task();
#endif
}

void iot_agent_session_led_success(void)
{
#ifdef FRDM_K64F
    ex_sss_main_ksdk_success();
#endif
}

void iot_agent_session_led_failure(void)
{
#ifdef FRDM_K64F
    ex_sss_main_ksdk_failure();
#endif
}

void iot_agent_session_led_start(void)
{
#ifdef FRDM_K64F
    LED_BLUE_ON();
    LED_RED_OFF();
    LED_GREEN_OFF();
#endif
}
#endif

iot_agent_status_t iot_agent_session_init(int argc, const char *argv[], ex_sss_boot_ctx_t *pCtx)
{
    sss_status_t sss_status;
    const char *portName;
    iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;

    memset(pCtx, 0, sizeof(ex_sss_boot_ctx_t));

    sss_status = ex_sss_boot_connectstring(argc, argv, &portName);
    SSS_SUCCESS_OR_EXIT_MSG("ex_sss_boot_connectstring failed returned with 0x%04x", sss_status);

    sss_status = ex_sss_boot_open(pCtx, portName);
    SSS_SUCCESS_OR_EXIT_MSG("ex_sss_boot_open failed with 0x%04x", sss_status);

    if ((sss_type_t)kType_SSS_SubSystem_NONE == ((pCtx)->session.subsystem)) {
        /* Nothing to do. Device is not opened
         * This is needed for the case when we open a generic communication
         * channel, without being specific to SE05X
         */
    }
    else {
        sss_status = ex_sss_key_store_and_object_init(pCtx);
        SSS_SUCCESS_OR_EXIT_MSG("ex_sss_key_store_and_object_init failed with 0x%04x", sss_status);
    }

    ex_sss_boot_open_host_session(pCtx);
    SSS_SUCCESS_OR_EXIT_MSG("ex_sss_boot_open_host_session failed with 0x%04x", sss_status);
    agent_status = IOT_AGENT_SUCCESS;

exit:
    return agent_status;
}
