/*
 * Copyright 2020-2021,2024-2025 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef _NXP_IOT_AGENT_TIME_H_
#define _NXP_IOT_AGENT_TIME_H_

#include <nxp_iot_agent_status.h>
#include <nxp_iot_agent_config.h>

typedef struct iot_agent_time_context_t
{
	void* ctx;
}iot_agent_time_context_t;

typedef struct iot_agent_time_t
{
	long init_time;        /*Initialization of iot_agent with session*/
	long prepare_tls_time; /*Time for reading device credentials for TLS connection*/
	long network_connect_time;  /*Time for successful network connection to EL2GO*/
#if defined(NXP_IOT_AGENT_REQUEST_CRL_FROM_EDGELOCK_2GO) && (NXP_IOT_AGENT_REQUEST_CRL_FROM_EDGELOCK_2GO == 1)
	long crl_time;         /* Time for CRL verification*/
#endif
    long apdu_time;        /*Apdu exchange time with SE after connecting to EL2GO */
    long process_provision_time;        /*Time for processing provisions from EL2GO after successful TLS connection*/
    long total_time;       /*Total time taken for a session with EL2GO*/
} iot_agent_time_t;

extern iot_agent_time_t iot_agent_time;

/*! @brief Initializes the time measurement.
 *
 * @param time_context: Context for the iot_agent.
 */
void iot_agent_time_init_measurement(iot_agent_time_context_t* time_context);

/*! @brief Initializes the time measurement.
 *
 * @param time_context: Context for the iot_agent.
 */
void iot_agent_time_conclude_measurement(iot_agent_time_context_t* time_context);

/*! @brief Gets the time measurement.
 *
 * @param time_context: Context for the iot_agent.
 * @retval The measured time.
 */
long iot_agent_time_get_measurement(iot_agent_time_context_t* time_context);

/*! @brief Frees the measurement context.
 *
 * @param time_context: Context for the iot_agent.
 */
void iot_agent_time_free_measurement_ctx(iot_agent_time_context_t* time_context);

/*! @brief Prints the time measurement.
 *
 * @param time: structure including the time to be printed 
 * @retval IOT_AGENT_SUCCESS if pass
 */
iot_agent_status_t iot_agent_log_performance_timing(void);

#endif // #ifndef _NXP_IOT_AGENT_TIME_H_
