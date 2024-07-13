/*
 * Copyright 2020-2021,2023-2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */


#include <stdio.h>
#include <nxp_iot_agent_time.h>
#include <nxp_iot_agent_macros.h>

#if IOT_AGENT_TIME_MEASUREMENT_ENABLE

#if (!AX_EMBEDDED)
#include <time.h>
#endif

iot_agent_time_t iot_agent_time = {0};

#if defined(RW612_SERIES) || defined(__ZEPHYR__)
// in RW61x setup the functions executing measurement should be locally defined
#ifdef __ZEPHYR__
#include <zephyr/kernel.h>
#else
#include "task.h"
#endif

long getMeasurement(axTimeMeasurement_t *mPair)
{
    if (mPair->tEnd >= mPair->tStart)
        return mPair->tEnd - mPair->tStart;
    else
        return 0;
}

void initMeasurement(axTimeMeasurement_t *mPair)
{
#ifdef __ZEPHYR__
    mPair->tEnd = mPair->tStart = k_uptime_get();
#else
    mPair->tEnd = mPair->tStart = xTaskGetTickCount();
#endif
}

void concludeMeasurement(axTimeMeasurement_t *mPair)
{
#ifdef __ZEPHYR__
    mPair->tEnd = k_uptime_get();
#else
    mPair->tEnd = xTaskGetTickCount();
#endif
}
#endif

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
	struct tm detail_time = {0};
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

#endif //IOT_AGENT_TIME_MEASUREMENT_ENABLE
