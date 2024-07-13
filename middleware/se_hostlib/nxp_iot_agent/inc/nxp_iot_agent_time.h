/*
 * Copyright 2020-2021,2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef _NXP_IOT_AGENT_TIME_H_
#define _NXP_IOT_AGENT_TIME_H_

#include <nxp_iot_agent_common.h>

#if IOT_AGENT_TIME_MEASUREMENT_ENABLE

#include <nxp_iot_agent.h>

#if defined(RW612_SERIES) || defined(__ZEPHYR__)
/**
 * Utility structure enabling delta time measurement.
 */
typedef struct {
    long tStart; //!< To contain start of time measurement
    long tEnd;   //!< To contain end of time measurement
} axTimeMeasurement_t;

long getMeasurement(axTimeMeasurement_t *mPair);
void initMeasurement(axTimeMeasurement_t *mPair);
void concludeMeasurement(axTimeMeasurement_t *mPair);
#else
#include "tst_sm_time.h"
#endif

typedef struct iot_agent_time_t
{
	long init_time;        /*Initialization of iot_agent with session*/
	long prepare_tls_time; /*Time for reading device credentials for TLS connection*/
	long network_connect_time;  /*Time for successful network connection to EL2GO*/
#if NXP_IOT_AGENT_REQUEST_CRL_FROM_EDGELOCK_2GO
	long crl_time;         /* Time for CRL verification*/
#endif
    long apdu_time;        /*Apdu exchange time with SE after connecting to EL2GO */
    long process_provision_time;        /*Time for processing provisions from EL2GO after successful TLS connection*/
    long total_time;       /*Total time taken for a session with EL2GO*/
} iot_agent_time_t;

extern iot_agent_time_t iot_agent_time;

iot_agent_status_t iot_agent_log_performance_timing(void);

#endif //IOT_AGENT_TIME_MEASUREMENT_ENABLE

#endif // #ifndef _NXP_IOT_AGENT_TIME_H_
