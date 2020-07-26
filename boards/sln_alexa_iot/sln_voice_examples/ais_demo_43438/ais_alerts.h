/*
 * Copyright 2018 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/** @file ais_alerts.h
 *  @brief This file handles the alerts
 *
 *  This file handles the saving, deleting and triggering of the alerts.
 */

#ifndef _AIS_ALERTS_H_
#define _AIS_ALERTS_H_

#include <limits.h>
#include <stdint.h>
#include "aisv2.h"
#include "flexspi_hyper_flash_ops.h"

#define AIS_ALERT_FILE_NAME "alerts.dat"

#define AIS_APP_MAX_ALERT_COUNT (6U)

#define AIS_ABS_MAX_ALERT_COUNT (32U)

#define OFFLINE_AUDIO_TIMER (1U << 0U)
#define OFFLINE_AUDIO_ALARM (1U << 1U)
#define OFFLINE_AUDIO_RMNDR (1U << 2U)
#define OFFLINE_AUDIO_ABORT (1U << 3U)
#define OFFLINE_AUDIO_EVENT_MASK (OFFLINE_AUDIO_TIMER | OFFLINE_AUDIO_ALARM | OFFLINE_AUDIO_RMNDR | OFFLINE_AUDIO_ABORT)

#if AIS_APP_MAX_ALERT_COUNT > AIS_ABS_MAX_ALERT_COUNT
#error "Absolute maximum AIS alert count reached for this implementation!"
#endif

/* TODO: Need to change this back to 1 when supported*/
#if (defined(AIS_SPEC_REV_325) && (AIS_SPEC_REV_325 == 0))
#define AIS_MAX_ALERT_TOKEN_LEN_BYTES (9U) // 8 character token with null terminator
#else
#define AIS_MAX_ALERT_TOKEN_LEN_BYTES (120U)
#endif

typedef enum _ais_alert_events
{
    kNewAlertSet        = (1U << 16U),
    kAlertDelete        = (1U << 17U),
    kNewAlertFail       = (1U << 18U),
    kFailDelete         = (1U << 19U),
    kAlertOnlineTrigger = (1U << 20U),
} ais_alert_events_t;

typedef struct _ais_alert
{
    char token[AIS_MAX_ALERT_TOKEN_LEN_BYTES];
    uint64_t scheduledTime; /* Time for alert to trigger; epoch seconds from Jan 1 1900 */
    uint32_t durationMs;
    ais_alert_type_t type;
    bool idle : 1;
    bool valid : 1;
} ais_alert_t;

typedef char alertTokenList_t[AIS_APP_MAX_ALERT_COUNT][AIS_MAX_ALERT_TOKEN_LEN_BYTES];

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initialize AIS Alerts module; create lock and task to handle events; copy any existing file into ram
 *
 * @returns Status of initialization
 */
int32_t AIS_Alerts_Init(void);

/*!
 * @brief Get task handle for this module
 *
 * @returns Task handle for this module
 */
TaskHandle_t AIS_Alerts_GetThisHandle(void);

/*!
 * @brief Set the UX system task handle and notification bit
 *
 * @param *handle Reference to UX Task handle
 *
 * @returns Status of operation
 */
int32_t AIS_Alerts_SetUxHandle(TaskHandle_t *handle);

/*!
 * @brief Returns the Ux Notification status bit
 *
 * @returns Type of Notify bit
 */
uint32_t AIS_Alerts_GetUxNotifyBit();

/*!
 * @brief Set the UX system timer notification bit
 *
 * @param notifyBit Notify bit to use with UX task handle
 *
 * @returns Status of operation
 */
int32_t AIS_Alerts_SetUxTimerBit(EventBits_t timerBit);

/*!
 * @brief Set the UX system alarm notification bit
 *
 * @param notifyBit Notify bit to use with UX task handle
 *
 * @returns Status of operation
 */
int32_t AIS_Alerts_SetUxAlarmBit(EventBits_t alarmBit);

/*!
 * @brief Set the UX system reminder notification bit
 *
 * @param notifyBit Notify bit to use with UX task handle
 *
 * @returns Status of operation
 */
int32_t AIS_Alerts_SetUxReminderBit(EventBits_t reminderBit);

/*!
 * @brief Update the state of all alerts with given epoch
 *
 * @param epoch Seconds since Jan 1, 1900
 *
 * @returns Status of update
 */
int32_t AIS_Alerts_UpdateState(uint64_t epoch);

/*!
 * @brief Save an alert from AIS; saves to the working ram array of alerts
 *
 * @param *alert Reference to original alert from AIS
 *
 * @returns Status of save operation
 */
int32_t AIS_Alerts_SaveAlert(ais_alert_t *alert);

/*!
 * @brief Mark an alert for deletion
 *
 * @param token String for AIS alert token
 *
 * @returns Status of delete marking
 */
int32_t AIS_Alerts_MarkForDelete(const char *token, size_t len);

/*!
 * @brief Mark an alert for deletion; offline by index
 *
 * @param index Numeric index in array of deleted alert
 *
 * @returns Status of delete marking
 */
int32_t AIS_Alerts_MarkForDeleteOffline(uint32_t index);

/*!
 * @brief Trigger an alert through UX system
 *
 * @param index Numeric index in array of triggered alert
 * @param isOffline Boolean to indicate if we are triggering offline or not
 *
 * @returns Status of trigger
 */
int32_t AIS_Alerts_Trigger(uint32_t index, bool isOffline);

/*!
 * @brief Mark alert as triggered
 *
 * @param index Numeric index in array of triggered alert
 *
 * @returns Status of triggered marking
 */
int32_t AIS_Alerts_MarkAsTriggered(uint32_t index);

/*!
 * @brief Mark alert as triggered offline; triggered state saved to NVM
 *
 * @param index Numeric index in array of triggered alert
 *
 * @returns Status of triggered marking
 */
int32_t AIS_Alerts_MarkAsTriggeredOffline(uint32_t index);

/*!
 * @brief Get alert's scheduled time to trigger
 *
 * @param index Numeric index in array of alerts
 *
 * @returns 64-bit Epoch time from 1900
 */
uint64_t AIS_Alerts_GetScheduledTime(uint32_t index);

/*!
 * @brief Get all pending alert indices
 *
 * @param indices Reference to array of alert indices
 */
void AIS_Alerts_GetPendingAlertIndices(uint32_t *indices);

/*!
 * @brief Get current idle state of alert
 *
 * @param index Numeric index in array of alerts
 *
 * @returns bool Idle state True for idle, False for triggered
 */
bool AIS_Alerts_GetIdleState(uint32_t index);

/*!
 * @brief Get list of alerts that need to be deleted in service; typically called on connection to service
 *
 * @param deleteList Reference to list of tokens to delete
 * @param deleteCount Reference to number of items that need to be deleted
 *
 * @returns Status of list population
 */
int32_t AIS_Alerts_GetDeletedList(alertTokenList_t *deleteList, uint32_t *deleteCount);

/*!
 * @brief Get list of alerts in NVM
 *
 * @param alertList Reference to list of tokens stored
 * @param alertCount Reference to number of items stored
 *
 * @returns Status of list population
 */
int32_t AIS_Alerts_GetAlertsList(alertTokenList_t *alertList, uint32_t *alertCount);

/*!
 * @brief Gets the index of a duplicate if it exists; typically called when receiving a set alert
 *
 * @param token the string of the token id defined by amazon
 * @param len the size of the token id
 *
 * @returns Status index of duplicate or -1 if not found
 */
int32_t AIS_Alerts_GetDuplicate(const char *token, size_t len);

#if defined(__cplusplus)
}
#endif

#endif /* _AIS_ALERTS_H_ */
