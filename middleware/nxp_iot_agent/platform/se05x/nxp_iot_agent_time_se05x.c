/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#include <nxp_iot_agent_time.h>
#include <nxp_iot_agent_config.h>
#include <nxp_iot_agent_macros.h>
#include "tst_sm_time.h"

#if (!AX_EMBEDDED)
#include <time.h>
#endif

iot_agent_time_t iot_agent_time = { 0 };

void iot_agent_time_init_measurement(iot_agent_time_context_t* time_context)
{
	axTimeMeasurement_t* time_meas_ctx = malloc(sizeof(axTimeMeasurement_t));
	initMeasurement(time_meas_ctx);
	time_context->ctx = (axTimeMeasurement_t*)time_meas_ctx;
}

void iot_agent_time_conclude_measurement(iot_agent_time_context_t* time_context)
{
	concludeMeasurement((axTimeMeasurement_t*)time_context->ctx);
}

long iot_agent_time_get_measurement(iot_agent_time_context_t* time_context)
{
	return getMeasurement((axTimeMeasurement_t*)time_context->ctx);
}

void iot_agent_time_free_measurement_ctx(iot_agent_time_context_t* time_context)
{
	free((axTimeMeasurement_t*)time_context->ctx);
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

	//For hosts with filesystem create a log file
#if (!AX_EMBEDDED)
	time_t systime;
	struct tm detail_time = { 0 };
	FILE* fd = fopen("performance_log.txt", "ab+");
	ASSERT_OR_EXIT_MSG(fd != NULL, "Error opening file");

	ASSERT_OR_EXIT_MSG(time(&systime) >= 0, "Error in getting the time");
#if defined(_WIN32) || defined(_WIN64)
	ASSERT_OR_EXIT_MSG(localtime_s(&detail_time, &systime) == 0, "Error in localtime function execution");
#else
	ASSERT_OR_EXIT_MSG(localtime_r(&systime, &detail_time) != NULL, "Error in localtime function execution");
#endif
	ASSERT_OR_EXIT_MSG(fprintf(fd, "Performance timing: ENTIRE_SESSION_TIME : %ldms\n", iot_agent_time.total_time) >= 0, "Error in fprintf execution");
	ASSERT_OR_EXIT_MSG(fprintf(fd, "Performance timing: AGENT_INIT_TIME : %ldms\n", iot_agent_time.init_time) >= 0, "Error in fprintf execution");
	ASSERT_OR_EXIT_MSG(fprintf(fd, "Performance timing: TLS_PREP_TIME : %ldms\n", iot_agent_time.prepare_tls_time) >= 0, "Error in fprintf execution");
	ASSERT_OR_EXIT_MSG(fprintf(fd, "Performance timing: NETWORK_CONNECT_TIME : %ldms\n", iot_agent_time.network_connect_time) >= 0, "Error in fprintf execution");
	ASSERT_OR_EXIT_MSG(fprintf(fd, "Performance timing: PROCESS_PROVISION_TIME : %ldms\n", iot_agent_time.process_provision_time) >= 0, "Error in fprintf execution");
#if NXP_IOT_AGENT_REQUEST_CRL_FROM_EDGELOCK_2GO
	ASSERT_OR_EXIT_MSG(fprintf(fd, "\tCRL_TIME : [%ldms] and ", iot_agent_time.crl_time) >= 0, "Error in fprintf execution");
#endif
	ASSERT_OR_EXIT_MSG(fprintf(fd, "COMMAND_TXRX_TIME : [%ldms] included in PROCESS_PROVISION_TIME\n\n", iot_agent_time.apdu_time) >= 0, "Error in fprintf execution");
exit:
	if (fd != NULL) {
		ASSERT_OR_EXIT_MSG(fclose(fd) == 0, "Error in closing the file");
	}
#endif
	return agent_status;
}