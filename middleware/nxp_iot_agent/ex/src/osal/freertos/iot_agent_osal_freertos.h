/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _IOT_AGENT_OSAL_FREERTOS_H_
#define _IOT_AGENT_OSAL_FREERTOS_H_

/*! @brief Initialize the board. */
void iot_agent_freertos_bm(void);

/*! @brief Boots freertos task. */
void iot_agent_freertos_boot_rtos_task(void);

/*! @brief LED success. */
void iot_agent_freertos_led_success(void);

/*! @brief LED failure. */
void iot_agent_freertos_led_failure(void);

/*! @brief LED start. */
void iot_agent_freertos_led_start(void);

#endif // #ifndef _IOT_AGENT_OSAL_FREERTOS_H_
