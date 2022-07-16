/*
 * Copyright 2020, 2021 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */


#include <stdio.h>
#include <nxp_iot_agent_time.h>
#include <nxp_iot_agent_macros.h>

#if (!AX_EMBEDDED)
#include <time.h>
#endif

#if IOT_AGENT_TIME_MEASUREMENT_ENABLE
iot_agent_time_t iot_agent_time = {0};

iot_agent_status_t iot_agent_log_performance_timing()
{
    iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
    IOT_AGENT_INFO("Performance timing: ENTIRE_SESSION_TIME : %ldms", iot_agent_time.total_time);
    IOT_AGENT_INFO("\tPerformance timing: AGENT_INIT_TIME : %ldms", iot_agent_time.init_time);
#if IOT_AGENT_CLAIMCODE_INJECT_ENABLE
    IOT_AGENT_INFO("\tPerformance timing: CLAIMCODE_INJECT_TIME : %ldms", iot_agent_time.claimcode_inject_time);
#endif
    IOT_AGENT_INFO("\tPerformance timing: TLS_PREP_TIME : %ldms", iot_agent_time.prepare_tls_time);
    IOT_AGENT_INFO("\tPerformance timing: NETWORK_CONNECT_TIME : %ldms", iot_agent_time.network_connect);
    IOT_AGENT_INFO("\tPerformance timing: PROCESS_PROVISION_TIME : %ldms", iot_agent_time.process_provision_time);
#if NXP_IOT_AGENT_REQUEST_CRL_FROM_EDGELOCK_2GO
    printf("\tCRL_TIME : [%ldms] and ", iot_agent_time.crl_time);
#endif
    printf("APDU_TXRX_TIME : [%ldms] included in PROCESS_PROVISION_TIME\n", iot_agent_time.apdu_time);

    //For hosts with filesystem create a log file
#if (!AX_EMBEDDED)
    time_t systime;
    struct tm * detail_time;
    FILE* fd = fopen("performance_log.txt", "ab+");
    ASSERT_OR_EXIT_MSG(fd != NULL, "Error opening file");

    time(&systime);
    detail_time = localtime(&systime);
    if (detail_time != NULL)
    {
        fprintf(fd, "Log time: %s", asctime(detail_time));
    }

    fprintf(fd, "Performance timing: ENTIRE_SESSION_TIME : %ldms\n", iot_agent_time.total_time);
    fprintf(fd, "Performance timing: AGENT_INIT_TIME : %ldms\n", iot_agent_time.init_time);
#if IOT_AGENT_CLAIMCODE_INJECT_ENABLE
    fprintf(fd, "Performance timing: CLAIMCODE_INJECT_TIME : %ldms\n", iot_agent_time.claimcode_inject_time);
#endif
    fprintf(fd, "Performance timing: TLS_PREP_TIME : %ldms\n", iot_agent_time.prepare_tls_time);
    fprintf(fd, "Performance timing: NETWORK_CONNECT_TIME : %ldms\n", iot_agent_time.network_connect);
    fprintf(fd, "Performance timing: PROCESS_PROVISION_TIME : %ldms\n", iot_agent_time.process_provision_time);
#if NXP_IOT_AGENT_REQUEST_CRL_FROM_EDGELOCK_2GO
    fprintf(fd, "\tCRL_TIME : [%ldms] and ", iot_agent_time.crl_time);
#endif
    fprintf(fd, "APDU_TXRX_TIME : [%ldms] included in PROCESS_PROVISION_TIME\n\n", iot_agent_time.apdu_time);
    fclose(fd);
exit:
#endif
    return agent_status;
}

#endif //IOT_AGENT_TIME_MEASUREMENT_ENABLE
