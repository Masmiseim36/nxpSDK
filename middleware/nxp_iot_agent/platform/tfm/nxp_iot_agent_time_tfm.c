/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#include <nxp_iot_agent_time.h>
#include <nxp_iot_agent_config.h>
#include <nxp_iot_agent_macros.h>

// in RW61x setup the functions executing measurement should be locally defined
#ifdef __ZEPHYR__
#include <zephyr/kernel.h>
#else
#include "task.h"
#endif

iot_agent_time_t iot_agent_time = { 0 };

/**
 * Utility structure enabling delta time measurement.
 */
typedef struct {
	long tStart; //!< To contain start of time measurement
	long tEnd;   //!< To contain end of time measurement
} time_meas_t;

void iot_agent_time_init_measurement(iot_agent_time_context_t* time_context)
{
	time_meas_t* time_meas_ctx = malloc(sizeof(time_meas_t));
	if (time_meas_ctx == NULL)
	{
		IOT_AGENT_WARN("Issue in initializing measurement context");
		return;
	}
#ifdef __ZEPHYR__
	time_meas_ctx->tEnd = time_meas_ctx->tStart = k_uptime_get();
#else
	uint32_t time = xTaskGetTickCount();
	if (time > INT32_MAX)
	{
		IOT_AGENT_WARN("Possible issue in time measurement due to overflow");
		time_meas_ctx->tEnd = time_meas_ctx->tStart = 0;
	}
	else
	{
		time_meas_ctx->tEnd = time_meas_ctx->tStart = time;
	}
	
#endif
	time_context->ctx = (time_meas_t*)time_meas_ctx;
}

void iot_agent_time_conclude_measurement(iot_agent_time_context_t* time_context)
{
#ifdef __ZEPHYR__
	((time_meas_t*)time_context->ctx)->tEnd = k_uptime_get();
#else
	uint32_t time = xTaskGetTickCount();
	if (time > INT32_MAX)
	{
	  	IOT_AGENT_WARN("Possible issue in time measurement due to overflow");
		((time_meas_t*)time_context->ctx)->tEnd = 0;
	}
	else
	{
		((time_meas_t*)time_context->ctx)->tEnd = time;
	}
#endif
}

long iot_agent_time_get_measurement(iot_agent_time_context_t* time_context)
{
	if (((time_meas_t*)time_context->ctx)->tEnd >= ((time_meas_t*)time_context->ctx)->tStart)
  {
		return ((time_meas_t*)time_context->ctx)->tEnd - ((time_meas_t*)time_context->ctx)->tStart;
  }
	else
  {
		return 0;
  }
}

void iot_agent_time_free_measurement_ctx(iot_agent_time_context_t* time_context)
{
	free((time_meas_t*)time_context->ctx);
}


iot_agent_status_t iot_agent_log_performance_timing(void)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	IOT_AGENT_INFO("Performance timing: ENTIRE_SESSION_TIME : %ldms", iot_agent_time.total_time);
	IOT_AGENT_INFO("\tPerformance timing: AGENT_INIT_TIME : %ldms", iot_agent_time.init_time);
	IOT_AGENT_INFO("\tPerformance timing: TLS_PREP_TIME : %ldms", iot_agent_time.prepare_tls_time);
	IOT_AGENT_INFO("\tPerformance timing: NETWORK_CONNECT_TIME : %ldms", iot_agent_time.network_connect_time);
	IOT_AGENT_INFO("\tPerformance timing: PROCESS_PROVISION_TIME : %ldms", iot_agent_time.process_provision_time);
#if NXP_IOT_AGENT_REQUEST_CRL_FROM_EDGELOCK_2GO
	printf("\tCRL_TIME : [%ldms] and ", iot_agent_time.crl_time);
#endif
	printf("COMMAND_TXRX_TIME : [%ldms] included in PROCESS_PROVISION_TIME\n", iot_agent_time.apdu_time);

	return agent_status;
}