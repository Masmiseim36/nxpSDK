/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _IOT_AGENT_OSAL_H_
#define _IOT_AGENT_OSAL_H_

#include "nxp_iot_agent_status.h"

typedef iot_agent_status_t(*agent_start_task_t)(int argc, const char* argv[]);

/*! @brief Creates and runs the agent function as task in the actual operating system.
 *
 * @param[in] agent_start_task Function to start
 * @param[in] argc Number of arguments
 * @param[in] argv Arguments
 * @retval IOT_AGENT_SUCCESS Upon success
 */
iot_agent_status_t iot_agent_osal_start_task(agent_start_task_t agent_start_task, int argc, const char* argv[]);

#endif // #ifndef _IOT_AGENT_OSAL_H_