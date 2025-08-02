/*
 * Copyright 2024-2025 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef INC_FREERTOS_H /* Header guard of FreeRTOS */
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#endif /* INC_FREERTOS_H */
#include "task.h"
#include "iot_agent_osal.h"
#include "iot_agent_osal_freertos.h"
#include "iot_agent_network.h"
#include "nxp_iot_agent_common.h"
#include "nxp_iot_agent_log.h"
#include "nxp_iot_agent_status.h"
#include "nxp_iot_agent_macros.h"
#include "nxp_iot_agent_time.h"

#if defined(NXP_IOT_AGENT_HAVE_SSS) && (NXP_IOT_AGENT_HAVE_SSS == 1)
#include <nxLog_App.h>
#endif

#if defined(FRDM_KW41Z) || defined(FRDM_K64F) || defined(IMX_RT) || \
    defined(LPC_55x)
#define HAVE_KSDK
#endif

#ifdef HAVE_KSDK
#include "ex_sss_main_inc_ksdk.h"
#endif

// The TFM implementation comes with the NXP SDK which includes the app.h
#if (defined(NXP_IOT_AGENT_HAVE_PSA_IMPL_TFM) && (NXP_IOT_AGENT_HAVE_PSA_IMPL_TFM == 1)) && !defined(__ZEPHYR__)
#include "app.h"
#endif

#define EX_SSS_BOOT_RTOS_STACK_SIZE (1024*8)

#if defined(NXP_IOT_AGENT_HAVE_PSA_IMPL_TFM) && (NXP_IOT_AGENT_HAVE_PSA_IMPL_TFM == 1)
#ifdef NXP_IOT_AGENT_ENABLE_LITE
extern void config_mbedtls_threading_alt(void);
#else
#include "os_wrapper/common.h"
extern uint32_t tfm_ns_interface_init(void);
#endif
#endif

// the arguments passed to the FreeRTOS as pointer to it; it should not be allocated on the stack
typedef struct agent_start_task_ags
{
	agent_start_task_t agent_start_task;
	int c;
	const char **v;
} agent_start_task_args_t;

static agent_start_task_args_t agent_start_args;

void iot_agent_freertos_bm(void)
{
#if defined(NXP_IOT_AGENT_HAVE_SSS) && (NXP_IOT_AGENT_HAVE_SSS == 1)
	ex_sss_main_ksdk_bm();
#else
	BOARD_InitHardware();
#endif
}

void iot_agent_freertos_boot_rtos_task(void)
{
#if defined(NXP_IOT_AGENT_HAVE_SSS) && (NXP_IOT_AGENT_HAVE_SSS == 1)
    ex_sss_main_ksdk_boot_rtos_task();
#endif
}

void iot_agent_freertos_led_success(void)
{
#ifdef FRDM_K64F
    ex_sss_main_ksdk_success();
#endif
}

void iot_agent_freertos_led_failure(void)
{
#ifdef FRDM_K64F
    ex_sss_main_ksdk_failure();
#endif
}

void iot_agent_freertos_led_start(void)
{
#ifdef FRDM_K64F
    LED_BLUE_ON();
    LED_RED_OFF();
    LED_GREEN_OFF();
#endif
}

static void agent_start_task_in_loop(void *args) {

	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;

#if defined(IOT_AGENT_TIME_MEASUREMENT_ENABLE) && (IOT_AGENT_TIME_MEASUREMENT_ENABLE == 1)
	iot_agent_time_context_t iot_agent_demo_boot_time = { 0 };
	iot_agent_time_init_measurement(&iot_agent_demo_boot_time);
#endif

	iot_agent_freertos_boot_rtos_task();

#if defined(NXP_IOT_AGENT_HAVE_PSA_IMPL_TFM) && (NXP_IOT_AGENT_HAVE_PSA_IMPL_TFM == 1)
#ifdef NXP_IOT_AGENT_ENABLE_LITE
	config_mbedtls_threading_alt();
#else
	uint32_t ret = tfm_ns_interface_init();
	ASSERT_OR_EXIT_MSG(ret == OS_WRAPPER_SUCCESS, "TF-M interface initialization failed");	
#endif
#endif

	agent_status = network_init();
	AGENT_SUCCESS_OR_EXIT_MSG("Network initialization failed");

	const TickType_t xDelay = 2U * 1000U / portTICK_PERIOD_MS;

#if defined(IOT_AGENT_TIME_MEASUREMENT_ENABLE) && (IOT_AGENT_TIME_MEASUREMENT_ENABLE == 1)
	iot_agent_time_conclude_measurement(&iot_agent_demo_boot_time);
	IOT_AGENT_INFO("Performance timing: DEVICE_INIT_TIME : %lums", iot_agent_time_get_measurement(&iot_agent_demo_boot_time));
	 iot_agent_time_free_measurement_ctx(&iot_agent_demo_boot_time);
#endif

	for (;;)
	{
		iot_agent_freertos_led_start();

		agent_start_task_args_t* a = args;

		agent_status = a->agent_start_task(a->c, a->v);

		if (agent_status == IOT_AGENT_SUCCESS)
		{
			iot_agent_freertos_led_success();
		}
		else
		{
			iot_agent_freertos_led_failure();
		}

		vTaskDelay(xDelay);

	}

exit:
	return;
}

iot_agent_status_t iot_agent_osal_start_task(agent_start_task_t agent_start_task, int argc, const char* argv[])
{
	iot_agent_freertos_bm();

	agent_start_args.agent_start_task = agent_start_task;
	agent_start_args.c = argc;
	agent_start_args.v = argv;

	// in case of FreeRTOS the agent execution function will be started inside a task which will run it in loop
	if (xTaskCreate(&agent_start_task_in_loop,
		"agent_start_session_task",
		EX_SSS_BOOT_RTOS_STACK_SIZE,
		(void *)&agent_start_args,
		(tskIDLE_PRIORITY),
		NULL) != pdPASS) {
		IOT_AGENT_INFO("Task creation failed!.\r\n");
		while (true);
	}

	/* Run RTOS */
	vTaskStartScheduler();

	return IOT_AGENT_FAILURE;
}
