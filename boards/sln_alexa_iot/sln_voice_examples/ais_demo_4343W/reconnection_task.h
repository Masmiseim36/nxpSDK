/*
 * Copyright 2018 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef __RECONNECTION_TASK_H_
#define __RECONNECTION_TASK_H_

#include <stdint.h>
#include <stdbool.h>

/* FreeRTOS kernel includes */
#include "FreeRTOS.h"
#include "task.h"

typedef enum __reconnect_state
{
    kInitState     = (1U << 0U),
    kStartState    = (1U << 1U),
    kLinkLoss      = (1U << 2U),
    kLinkFix       = (1U << 3U),
    kMqttReconnect = (1U << 4U),
    kAisDisconnect = (1U << 5U),
    kAisReconnect  = (1U << 6U),
    kOtaDisconnect = (1U << 7U),
    kOtaReconnect  = (1U << 8U),
    kLinkUp        = (1U << 9U),
    kSysReboot     = (1U << 10U)
} reconnectState_t;

typedef enum __reconnect_event
{
    kReconnectInvalidSequence = (1U << 0U),
    kReconnectMessageTampered = (1U << 1U),
    kReconnectAPIDeprecated   = (1U << 2U),
    kReconnectEncryptionError = (1U << 3U),
    kReconnectAISDisconnect   = (1U << 4U),
    kReconnectNetworkLoss     = (1U << 5U),
    kReconnectMQTTDisconnect  = (1U << 6U)
} reconnectEvent_t;

/*!
 * @brief Sets the reconnection task event to trigger reconnect state machine
 *
 * @param event reconnectEvent_t The event that needs to be set for reconnect
 *
 * @returns If the event was sent for processing
 */
bool reconnection_task_set_event(reconnectEvent_t event);

/*!
 * @brief Get reconnection task current state
 *
 * @returns The reconnectState_t that the state machine is in
 */
reconnectState_t reconnection_task_get_state(void);

/*!
 * @brief Initializes the reconnection task and supporting timer tasks
 *
 * @param *handle Pointer to TashHandle_t that will be used by application
 */
int32_t reconnection_task_init(TaskHandle_t *handle);

#endif
