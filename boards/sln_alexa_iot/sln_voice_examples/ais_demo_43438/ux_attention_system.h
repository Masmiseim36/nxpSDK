/*
 * Copyright 2018 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef __UX_ATTENTION_SYSTEM_H_
#define __UX_ATTENTION_SYSTEM_H_

#include "FreeRTOS.h"
#include "task.h"
#include "aisv2_app.h"

/**
 * @brief This are all the states of the ux
 */
typedef enum _ux_attention_states
{
    uxNull,
    uxIdle,
    uxListeningStart,
    uxListeningActive,
    uxListeningEnd,
    uxThinking,
    uxSpeaking,
    uxSpeakingEnd,
    uxMicOntoOff,
    uxMicOfftoOn,
    uxTimer,
    uxTimerShort,
    uxTimerEnd,
    uxAlarm,
    uxAlarmShort,
    uxAlarmEnd,
    uxReminder,
    uxReminderShort,
    uxReminderEnd,
    uxNotificationIncoming,
    uxNotificationQueued,
    uxNotificationCleared,
    uxDoNotDisturb,
    uxReconnecting,
    uxConnected,
    uxBootUp,
    uxApMode,
    uxWiFiSetup,
    uxAccessPointFound,
    uxNoAccessPoint,
    uxInvalidWiFiCred,
    uxDeviceChange,
    uxDiscovery,
    uxAuthentication,
    uxDisconnected,
    uxBleProvision,
    uxMediumBrightness,
    uxLowBrightness,
    uxHighBrightness,
    uxError
} ux_attention_states_t;

#define UX_INTERACTION_MASK(x) \
    ((uxListeningStart == x) || (uxListeningActive == x) || (uxThinking == x) || (uxSpeaking == x))

/**
 * @brief This function initializes and creates the ux attention task
 * @param handle    The ux attention state task handler
 */
void ux_attention_task_Init(TaskHandle_t *handle);

/**
 * @brief This function sets the UX to the specified state
 * @param uxState   The desired UX state
 *
 * @return          Returns 0 for success and 1 for fail
 */
int32_t ux_attention_set_state(ux_attention_states_t uxState);

/**
 * @brief This function resumes the UX state based on the specified AIS state
 * @param state     The desired AIS state
 */
void ux_attention_resume_state(ais_state_t state);

/**
 * @brief This function gets the current UX state
 *
 * @return The current UX state
 */
ux_attention_states_t ux_attention_get_state(void);

/**
 * @brief This function sets the LED to the fault state
 * We need a dedicated function because in case of a fault the UX task will no longer be running
 * and this functions will set the LED to the desired colors
 */
void ux_attention_sys_fault(void);

#endif
