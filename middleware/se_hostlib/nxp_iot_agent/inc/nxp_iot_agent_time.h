/*
 * Copyright 2020, 2021 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef _NXP_IOT_AGENT_TIME_H_
#define _NXP_IOT_AGENT_TIME_H_
#include <nxp_iot_agent_config.h>

#if IOT_AGENT_TIME_MEASUREMENT_ENABLE
#include <nxp_iot_agent.h>
#include "tst_sm_time.h"

typedef struct iot_agent_time_t
{
#if AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1
	long boot_time;        /*Device initialization time*/
#endif
	long init_time;        /*Initialization of iot_agent with session*/
#if IOT_AGENT_CLAIMCODE_INJECT_ENABLE
    long claimcode_inject_time; /*Time for injectiong claimcode into secure element*/
#endif
	long prepare_tls_time; /*Time for reading device credentials for TLS connection*/
	long network_connect;  /*Time for successful network connection to EL2GO*/
#if NXP_IOT_AGENT_REQUEST_CRL_FROM_EDGELOCK_2GO
	long crl_time;         /* Time for CRL verification*/
#endif
    long apdu_time;        /*Apdu exchange time with SE after connecting to EL2GO */
    long process_provision_time;        /*Time for processing provisions from EL2GO after successful TLS connection*/
    long total_time;       /*Total time taken for a session with EL2GO*/
} iot_agent_time_t;

extern iot_agent_time_t iot_agent_time;

iot_agent_status_t iot_agent_log_performance_timing();

#endif //IOT_AGENT_TIME_MEASUREMENT_ENABLE

#endif // #ifndef _NXP_IOT_AGENT_TIME_H_
