/*
 * Copyright 2018-2022,2024 NXP
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

// The TFM implementation comes with the NXP SDK which includes the app.h
#if NXP_IOT_AGENT_HAVE_PSA_IMPL_TFM && !defined(__ZEPHYR__)
#include "app.h"
#endif

#include <nxp_iot_agent.h>
#include <nxp_iot_agent_service.h>
#include <nxp_iot_agent_macros.h>

#if NXP_IOT_AGENT_HAVE_SSS
#include <fsl_sss_api.h>
#include "ex_sss_ports.h"
iot_agent_status_t iot_agent_session_connect(ex_sss_boot_ctx_t *pCtx)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	sss_status_t sss_status;
	char *portName;

    sss_status = ex_sss_boot_connectstring(0, NULL, &portName);
	SSS_SUCCESS_OR_EXIT_MSG("ex_sss_boot_connectstring returned with 0x%04x", sss_status);

    sss_status = ex_sss_boot_open(pCtx, portName);
	SSS_SUCCESS_OR_EXIT_MSG("ex_sss_boot_open returned with 0x%04x", sss_status);

	sss_status = ex_sss_key_store_and_object_init(pCtx);
	SSS_SUCCESS_OR_EXIT_MSG("ex_sss_key_store_and_object_init returned with 0x%04x", sss_status);

exit:
#if defined(_MSC_VER)
    if (portName) {
        char* dummy_portName = NULL;
        size_t dummy_sz = 0;
        _dupenv_s(&dummy_portName, &dummy_sz, EX_SSS_BOOT_SSS_PORT);
        if (NULL != dummy_portName) {
            free(dummy_portName);
            free(portName);
        }
    }
#endif // _MSC_VER

	return agent_status;
}

void iot_agent_session_disconnect(ex_sss_boot_ctx_t *pCtx) {
    ex_sss_session_close(pCtx);
}
#endif //NXP_IOT_AGENT_HAVE_SSS

#if AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1
void iot_agent_session_bm(void)
{
	// Initialization of the specific board
#if NXP_IOT_AGENT_HAVE_SSS
  	// In case the SSS library exist, initialization is happening there
	ex_sss_main_ksdk_bm();
#elif !defined(__ZEPHYR__)
	BOARD_InitHardware();
	// otherwise use the SDK functions to initialize it
#endif //NXP_IOT_AGENT_HAVE_SSS
}

void iot_agent_session_boot_rtos_task(void)
{
#if NXP_IOT_AGENT_HAVE_SSS
    ex_sss_main_ksdk_boot_rtos_task();
#endif
}

void iot_agent_session_led_success(void)
{
#ifdef FRDM_K64F
    ex_sss_main_ksdk_success();
#endif //FRDM_K64F
}

void iot_agent_session_led_failure(void)
{
#ifdef FRDM_K64F
    ex_sss_main_ksdk_failure();
#endif //FRDM_K64F
}

void iot_agent_session_led_start(void)
{
#ifdef FRDM_K64F
    LED_BLUE_ON();
    LED_RED_OFF();
    LED_GREEN_OFF();
#endif //FRDM_K64F
}
#endif //AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1

#if NXP_IOT_AGENT_HAVE_SSS
iot_agent_status_t iot_agent_session_init(int argc, const char *argv[], ex_sss_boot_ctx_t *pCtx)
{
    sss_status_t sss_status;
    char *portName;
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
#if defined(_MSC_VER)
    if (portName) {
        char* dummy_portName = NULL;
        size_t dummy_sz = 0;
        _dupenv_s(&dummy_portName, &dummy_sz, EX_SSS_BOOT_SSS_PORT);
        if (NULL != dummy_portName) {
            free(dummy_portName);
            free(portName);
        }
    }
#endif // _MSC_VER
    return agent_status;
}

#endif //NXP_IOT_AGENT_HAVE_SSS