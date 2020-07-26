/*
 * Copyright 2018 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "ais_alerts.h"
#include "semphr.h"
#include "task.h"

#include "audio_samples.h"
#include "sln_amplifier.h"
#include "sln_flash.h"
#include "sln_flash_mgmt.h"
#include "ux_attention_system.h"

typedef enum _alert_slot_state
{
    ALERT_SLOT_STATE_EMPTY,
    ALERT_SLOT_STATE_NULL_PTR,
    ALERT_SLOT_STATE_INVALID,
    ALERT_SLOT_STATE_DELETED,
    ALERT_SLOT_STATE_INUSE,
} alert_slot_state_t;

// TODO: Make this "spark joy"
extern EventGroupHandle_t s_offlineAudioEventGroup;

static TaskHandle_t s_thisHandle;
static TaskHandle_t s_uxHandle;
static uint32_t s_uxNotifyBit;
static uint32_t s_uxTimer;
static uint32_t s_uxAlarm;
static uint32_t s_uxReminder;
static SemaphoreHandle_t s_aisAlertLock;
volatile static uint32_t s_aisAlertCount                                                                      = 0U;
__attribute__((section(".ocram_non_cacheable_data"))) static ais_alert_t s_aisAlerts[AIS_APP_MAX_ALERT_COUNT] = {0};

// Must be higher priority than AIS_Task
#define AIS_ALERT_TASK_PRIORITY (configTIMER_TASK_PRIORITY - 1)

static int32_t ais_alert_get_flash_slot_state(ais_alert_t *alertSlot)
{
    int32_t ret = ALERT_SLOT_STATE_NULL_PTR;

    if (NULL != alertSlot)
    {
        bool isTokenOnes  = false;
        bool isTokenZeros = false;

        bool isTimeOnes  = (UINT64_MAX == alertSlot->scheduledTime) ? true : false;
        bool isTimeZeros = (0 == alertSlot->scheduledTime) ? true : false;

        bool isDurOnes  = (UINT32_MAX == alertSlot->durationMs) ? true : false;
        bool isDurZeros = (0 == alertSlot->durationMs) ? true : false;

        bool isTypeOnes  = (AIS_ALERT_TYPE_INVALID < alertSlot->type) ? true : false;
        bool isTypeZeros = (0 == alertSlot->type) ? true : false;

        bool isValid = alertSlot->valid;
        bool isIdle  = alertSlot->idle;

        volatile uint32_t tokenByteCount = 0;
        uint32_t tokenMaxByteCount       = 0xFF * AIS_MAX_ALERT_TOKEN_LEN_BYTES;

        for (uint32_t idx = 0; idx < AIS_MAX_ALERT_TOKEN_LEN_BYTES; idx++)
        {
            tokenByteCount += alertSlot->token[idx];
        }

        if (tokenMaxByteCount == tokenByteCount)
        {
            isTokenOnes = true;
        }
        else if (0 == tokenByteCount)
        {
            isTokenZeros = true;
        }

        if (isTokenOnes && isTimeOnes && isDurOnes && isTypeOnes && isValid && isIdle)
        {
            ret = ALERT_SLOT_STATE_EMPTY;
        }
        else if (isTokenZeros && isTimeZeros && isDurZeros && isTypeZeros && !isValid && !isIdle)
        {
            ret = ALERT_SLOT_STATE_DELETED;
        }
        else if (!isTokenZeros && !isTokenOnes && !isTimeZeros && !isTimeOnes && !isDurZeros && !isDurOnes &&
                 !isTypeOnes)
        {
            ret = ALERT_SLOT_STATE_INUSE;
        }
        else
        {
            ret = ALERT_SLOT_STATE_INVALID;
        }
    }

    return ret;
}

int32_t ais_alert_store_alerts_to_nvm(void)
{
    int32_t ret = -1;

    if (NULL != s_aisAlertLock)
    {
        if (pdTRUE == xSemaphoreTake(s_aisAlertLock, portMAX_DELAY))
        {
            ret               = -2;
            uint32_t fileSize = sizeof(s_aisAlerts);

            // Allocate ram space for current file
            ais_alert_t *flashFile = (ais_alert_t *)pvPortMalloc(fileSize);

            if (NULL == flashFile)
            {
                ret = -3;
                goto exit;
            }

            // Read current file into temporary file buffer
            ret = SLN_FLASH_MGMT_Read(AIS_ALERT_FILE_NAME, (uint8_t *)flashFile, &fileSize);

            if (SLN_FLASH_MGMT_ENOENTRY2 == ret)
            {
                // No entry so we should clear flashFile contents
                memset((uint8_t *)flashFile, 0xFF, fileSize);

                // Copy in RAM copy of alerts
                memcpy(flashFile, s_aisAlerts, sizeof(ais_alert_t));

                // Write a new file entry
                ret = SLN_FLASH_MGMT_Save(AIS_ALERT_FILE_NAME, (uint8_t *)flashFile, fileSize);
            }
            /* Due to the CRC implementation, CRC will always fail */
            else if ((SLN_FLASH_MGMT_OK == ret) || (SLN_FLASH_MGMT_EENCRYPT2 == ret))
            {
                // Iterate through file and add new valid entries
                volatile uint32_t nvmInvalidCount = 0;
                volatile uint32_t ramInvalidCount = 0;
                for (uint32_t idx = 0; idx < AIS_APP_MAX_ALERT_COUNT; idx++)
                {
                    volatile bool copyNewSlot       = false;
                    alert_slot_state_t nvmSlotState = ais_alert_get_flash_slot_state(&flashFile[idx]);
                    alert_slot_state_t ramSlotState = ais_alert_get_flash_slot_state(&s_aisAlerts[idx]);

                    // If RAM and Flash contents do not match we must take action
                    if (memcmp(&(flashFile[idx]), &s_aisAlerts[idx], sizeof(ais_alert_t)))
                    {
                        // Tentatively set copy flag to true
                        copyNewSlot = true;

                        if ((ALERT_SLOT_STATE_DELETED == nvmSlotState) || (ALERT_SLOT_STATE_INVALID == nvmSlotState))
                        {
                            // There is garbage in here or it zeros (can't update)
                            nvmInvalidCount |= (1U << idx);
                        }

                        if ((ALERT_SLOT_STATE_DELETED == ramSlotState) || (ALERT_SLOT_STATE_INVALID == ramSlotState))
                        {
                            // There is garbage in here or it zeros (can't update)
                            ramInvalidCount |= (1U << idx);

                            // Reset copy flag back to false; should not copy deleted or invalid data
                            copyNewSlot = false;
                        }
                    }

                    if (copyNewSlot)
                    {
                        memcpy(&(flashFile[idx]), &s_aisAlerts[idx], sizeof(ais_alert_t));
                    }
                }

                // Make sure invalid counts match in ram and nvm
                if (nvmInvalidCount == ramInvalidCount)
                {
                    // Over write flash file with new valid entries
                    ret = SLN_FLASH_MGMT_Update(AIS_ALERT_FILE_NAME, (uint8_t *)flashFile, &fileSize);
                }
                else
                {
                    // Write a new file entry
                    ret = SLN_FLASH_MGMT_Save(AIS_ALERT_FILE_NAME, (uint8_t *)flashFile, fileSize);
                }
            }
            else
            {
                ret = -4;
                goto exit;
            }

            if (0 != ret)
            {
                ret = -5;
                goto exit;
            }

            // Update RAM copy; re-sync the ram copy with Flash copy
            ret = SLN_FLASH_MGMT_Read(AIS_ALERT_FILE_NAME, (uint8_t *)s_aisAlerts, &fileSize);

            /* Due to the CRC implementation, CRC will always fail */
            if ((0 != ret) && (SLN_FLASH_MGMT_EENCRYPT2 == ret))
            {
                ret = -6;
                goto exit;
            }

        exit:
            if (NULL != flashFile)
            {
                memset(flashFile, 0x00, fileSize);
            }
            vPortFree(flashFile);
            flashFile = NULL;
            xSemaphoreGive(s_aisAlertLock);
        }
    }

    return ret;
}

int32_t ais_alert_delete(uint32_t index)
{
    int32_t ret = -1;

    if (AIS_APP_MAX_ALERT_COUNT > index)
    {
        if (NULL != s_aisAlertLock)
        {
            if (pdTRUE == xSemaphoreTake(s_aisAlertLock, portMAX_DELAY))
            {
                uint32_t fileSize = sizeof(s_aisAlerts);

                // Allocate ram space for current file
                ais_alert_t *flashFile = (ais_alert_t *)pvPortMalloc(fileSize);

                if (NULL == flashFile)
                {
                    ret = -2;
                    goto exit;
                }

                // Read current file into temporary file buffer
                ret = SLN_FLASH_MGMT_Read(AIS_ALERT_FILE_NAME, (uint8_t *)flashFile, &fileSize);

                /* Due to the CRC implementation, CRC will always fail */
                if ((0 != ret) && (SLN_FLASH_MGMT_EENCRYPT2 == ret))
                {
                    ret = -3;
                    goto exit;
                }

                // Clear all bits in Flash to invalidate the entry in this file space
                memset(&flashFile[index], 0x00, sizeof(ais_alert_t));

                // Over write flash file
                ret = SLN_FLASH_MGMT_Update(AIS_ALERT_FILE_NAME, (uint8_t *)flashFile, &fileSize);

                if (0 != ret)
                {
                    ret = -4;
                    goto exit;
                }

                // Update RAM copy
                ret = SLN_FLASH_MGMT_Read(AIS_ALERT_FILE_NAME, (uint8_t *)s_aisAlerts, &fileSize);

                /* Due to the CRC implementation, CRC will always fail */
                if ((0 != ret) && (SLN_FLASH_MGMT_EENCRYPT2 == ret))
                {
                    ret = -5;
                    goto exit;
                }

                // Update count
                if (s_aisAlertCount != 0)
                {
                    s_aisAlertCount--;
                }

            exit:
                if (NULL != flashFile)
                {
                    memset(flashFile, 0x00, fileSize);
                }
                vPortFree(flashFile);
                flashFile = NULL;
                xSemaphoreGive(s_aisAlertLock);
            }
        }
    }

    return ret;
}

void ais_alert_task(void *pvParameters)
{
    int32_t status            = 0;
    uint32_t taskNotification = 0;

    while (1)
    {
        xTaskNotifyWait(0, ULONG_MAX, &taskNotification, portMAX_DELAY);

        if (kNewAlertSet & taskNotification)
        {
            status = ais_alert_store_alerts_to_nvm();

            if (0 != status)
            {
                configPRINTF(("Error saving alerts: %d!\r\n", status));
            }
            else
            {
                configPRINTF(("Successfully updated the alerts!\r\n"));
            }

            taskNotification &= ~kNewAlertSet;
        }

        if (kAlertDelete & taskNotification)
        {
            for (uint32_t idx = 0; idx < AIS_APP_MAX_ALERT_COUNT; idx++)
            {
                alert_slot_state_t ramSlotState = ais_alert_get_flash_slot_state(&s_aisAlerts[idx]);

                if (ALERT_SLOT_STATE_INUSE == ramSlotState)
                {
                    if (false == s_aisAlerts[idx].valid)
                    {
                        status = ais_alert_delete(idx);

                        if (0 != status)
                        {
                            configPRINTF(("Error deleting alerts: %d!\r\n", status));
                        }
                        else
                        {
                            configPRINTF(("Successfully deleted the alert %d !\r\n", idx));
                        }
                    }
                }
            }

            taskNotification &= ~kAlertDelete;
        }
    }
}

int32_t AIS_Alerts_Init(void)
{
    int32_t ret = -1;

    // Set default state to all ones to match erased flash state
    memset(&s_aisAlerts[0], 0xFF, AIS_APP_MAX_ALERT_COUNT * sizeof(ais_alert_t));

    // Check for existing file
    uint32_t fileSize = sizeof(s_aisAlerts);

    // Allocate ram space for current file
    ais_alert_t *flashFile = (ais_alert_t *)pvPortMalloc(fileSize);

    /* Memset to 0 just in case the memory block was previously used. Best practice */
    memset(flashFile, 0, fileSize);

    if (NULL == flashFile)
    {
        ret = -2;
        goto exit;
    }

    // Read current file into temporary file buffer
    ret = SLN_FLASH_MGMT_Read(AIS_ALERT_FILE_NAME, (uint8_t *)flashFile, &fileSize);

    /* Due to the CRC implementation, CRC will always fail */
    if ((0 != ret) && (SLN_FLASH_MGMT_ENOENTRY2 != ret) && (SLN_FLASH_MGMT_EENCRYPT2 != ret))
    {
        ret = -3;
        goto exit;
    }

    /* If this is the first time, then don't check for alerts */
    if (SLN_FLASH_MGMT_ENOENTRY2 != ret)
    {
        volatile bool isFileDeleted = (safe_strlen((char *)flashFile, fileSize)) ?
                                          false :
                                          true; // Protecting from optimization for debug purposes

        if (!isFileDeleted)
        {
            // Update Count
            for (uint32_t idx = 0; idx < AIS_APP_MAX_ALERT_COUNT; idx++)
            {
                volatile size_t tokenLen = safe_strlen(flashFile[idx].token, AIS_MAX_ALERT_TOKEN_LEN_BYTES);
                volatile bool isEmpty    = (AIS_MAX_ALERT_TOKEN_LEN_BYTES == tokenLen) || (0 == tokenLen);

                if (!isEmpty && s_aisAlerts[idx].valid)
                {
                    s_aisAlertCount++;
                    configPRINTF(("Found alert index %d in flash !\r\n", idx));
                }
            }

            // Copy contents into static copy
            memcpy(s_aisAlerts, flashFile, fileSize);
        }
    }

    // Reset ret back to zero
    ret = 0;

    // Create a lock with priority inheritance
    s_aisAlertLock = xSemaphoreCreateMutex();

    if (NULL == s_aisAlertLock)
    {
        ret = -4;
        goto exit;
    }

    if (xTaskCreate(ais_alert_task, "ais_alerts", 512U, NULL, AIS_ALERT_TASK_PRIORITY, &s_thisHandle) != pdPASS)
    {
        ret = -5;
        goto exit;
    }

exit:
    if (NULL != flashFile)
    {
        memset(flashFile, 0x00, fileSize);
    }
    vPortFree(flashFile);
    flashFile = NULL;

    return ret;
}

TaskHandle_t AIS_Alerts_GetThisHandle(void)
{
    return s_thisHandle;
}

int32_t AIS_Alerts_SetUxHandle(TaskHandle_t *handle)
{
    int32_t ret = -1;

    if ((NULL != handle) && (NULL != *handle))
    {
        s_uxHandle = *handle;
        ret        = 0;
    }

    return ret;
}

uint32_t AIS_Alerts_GetUxNotifyBit()
{
    return s_uxNotifyBit;
}

int32_t AIS_Alerts_SetUxTimerBit(EventBits_t timerBit)
{
    s_uxTimer = timerBit;
    return 0;
}

int32_t AIS_Alerts_SetUxAlarmBit(EventBits_t alarmBit)
{
    s_uxAlarm = alarmBit;
    return 0;
}

int32_t AIS_Alerts_SetUxReminderBit(EventBits_t reminderBit)
{
    s_uxReminder = reminderBit;
    return 0;
}

int32_t AIS_Alerts_UpdateState(uint64_t epoch)
{
    int32_t ret = -1;

    if (NULL != s_aisAlertLock)
    {
        if (pdTRUE == xSemaphoreTake(s_aisAlertLock, portMAX_DELAY))
        {
            bool isDeletePending = false;
            ret                  = 0;

            // Find the open slot and copy alert data
            for (uint32_t idx = 0; idx < AIS_APP_MAX_ALERT_COUNT; idx++)
            {
                volatile size_t tokenLen = safe_strlen(s_aisAlerts[idx].token, AIS_MAX_ALERT_TOKEN_LEN_BYTES);
                volatile bool isEmpty    = (AIS_MAX_ALERT_TOKEN_LEN_BYTES == tokenLen) || (0 == tokenLen);
                if (!isEmpty)
                {
                    if (epoch > AIS_Alerts_GetScheduledTime(idx))
                    {
                        // Update static copy
                        s_aisAlerts[idx].valid = 0;
                        isDeletePending        = true;
                    }
                }
            }

            if (isDeletePending)
            {
                ret = 1;
            }

            xSemaphoreGive(s_aisAlertLock);
        }
    }

    return ret;
}

int32_t AIS_Alerts_SaveAlert(ais_alert_t *alert)
{
    int32_t ret            = -1;
    int32_t duplicateIndex = -1;
    if (NULL != alert)
    {
        ret = -2;

        if (NULL != s_aisAlertLock)
        {
            if (pdTRUE == xSemaphoreTake(s_aisAlertLock, portMAX_DELAY))
            {
                volatile size_t incomingTokenLen = safe_strlen(alert->token, AIS_MAX_ALERT_TOKEN_LEN_BYTES);

                duplicateIndex = AIS_Alerts_GetDuplicate(alert->token, incomingTokenLen);

                /* If -1 is returned, it means a duplicate was not found, else, it returns an array element */
                if (duplicateIndex == -1)
                {
                    ret = -3;

                    if (AIS_APP_MAX_ALERT_COUNT >= s_aisAlertCount)
                    {
                        // Find the open slot and copy alert data
                        for (uint32_t idx = 0; idx < AIS_APP_MAX_ALERT_COUNT; idx++)
                        {
                            alert_slot_state_t ramSlotState = ais_alert_get_flash_slot_state(&s_aisAlerts[idx]);

                            if (ALERT_SLOT_STATE_INUSE != ramSlotState)
                            {
                                // Save Alert to RAM
                                memcpy(&(s_aisAlerts[idx]), alert, sizeof(ais_alert_t));

                                // Increment Total Alert Count
                                s_aisAlertCount++;

                                ret = 0;

                                break;
                            }
                        }
                    }
                }
                else
                {
                    /* TODO: Need to update the alert here */
                    ret = 0;
                }

                xSemaphoreGive(s_aisAlertLock);
            }
        }
    }

    return ret;
}

int32_t AIS_Alerts_MarkForDelete(const char *token, size_t len)
{
    int32_t ret = -1;

    if (NULL != s_aisAlertLock)
    {
        if (pdTRUE == xSemaphoreTake(s_aisAlertLock, portMAX_DELAY))
        {
            uint32_t idx = 0;
            ret          = -2;

            // Find a matching token
            for (idx = 0; idx < AIS_APP_MAX_ALERT_COUNT; idx++)
            {
                if (0 == memcmp(token, s_aisAlerts[idx].token, len))
                {
                    s_aisAlerts[idx].valid = false;
                    configPRINTF(("Marking for delete, alert token: %s\r\n", s_aisAlerts[idx].token));
                    ret = 0;
                    break;
                }
            }

            // Update flash file
            if (0 == ret)
            {
                uint32_t fileSize = sizeof(s_aisAlerts);

                // Allocate ram space for current file
                ais_alert_t *flashFile = (ais_alert_t *)pvPortMalloc(fileSize);

                if (NULL == flashFile)
                {
                    ret = -3;
                    goto exit;
                }

                // Read current file into temporary file buffer
                ret = SLN_FLASH_MGMT_Read(AIS_ALERT_FILE_NAME, (uint8_t *)flashFile, &fileSize);

                /* Due to the CRC implementation, CRC will always fail */
                if ((0 != ret) && (ret != SLN_FLASH_MGMT_EENCRYPT2))
                {
                    ret = -4;
                    goto exit;
                }

                // Clear all bits in Flash to invalidate the entry in this file space
                flashFile[idx].valid = 0;

                // Over write flash file
                ret = SLN_FLASH_MGMT_Update(AIS_ALERT_FILE_NAME, (uint8_t *)flashFile, &fileSize);

                if (0 != ret)
                {
                    ret = -5;
                    goto exit;
                }

            exit:
                if (NULL != flashFile)
                {
                    memset(flashFile, 0x00, fileSize);
                }
                vPortFree(flashFile);
                flashFile = NULL;
            }
            else
            {
                /* We didn't find an entry so return success */
                ret = 0;
            }

            xSemaphoreGive(s_aisAlertLock);
        }
    }

    return ret;
}

int32_t AIS_Alerts_MarkForDeleteOffline(uint32_t index)
{
    int32_t ret = -1;

    if (AIS_APP_MAX_ALERT_COUNT > index)
    {
        ret = -2;

        if (NULL != s_aisAlertLock)
        {
            if (pdTRUE == xSemaphoreTake(s_aisAlertLock, portMAX_DELAY))
            {
                // Set ram copy to triggered
                s_aisAlerts[index].valid = false;

                // Update flash file
                uint32_t fileSize = sizeof(s_aisAlerts);

                // Allocate ram space for current file
                ais_alert_t *flashFile = (ais_alert_t *)pvPortMalloc(fileSize);

                if (NULL == flashFile)
                {
                    ret = -3;
                    goto exit;
                }

                // Read current file into temporary file buffer
                ret = SLN_FLASH_MGMT_Read(AIS_ALERT_FILE_NAME, (uint8_t *)flashFile, &fileSize);

                /* Due to the CRC implementation, CRC will always fail */
                if ((0 != ret) && (ret != SLN_FLASH_MGMT_EENCRYPT2))
                {
                    ret = -4;
                    goto exit;
                }

                // Clear all bits in Flash to invalidate the entry in this file space
                flashFile[index].valid = 0;

                // Over write flash file
                ret = SLN_FLASH_MGMT_Update(AIS_ALERT_FILE_NAME, (uint8_t *)flashFile, &fileSize);

                if (0 != ret)
                {
                    ret = -5;
                    goto exit;
                }

            exit:
                if (NULL != flashFile)
                {
                    memset(flashFile, 0x00, fileSize);
                }
                vPortFree(flashFile);
                flashFile = NULL;

                xSemaphoreGive(s_aisAlertLock);
            }
        }
    }

    return ret;
}

int32_t AIS_Alerts_Trigger(uint32_t index, bool isOffline)
{
    int32_t ret = -1;

    if (AIS_APP_MAX_ALERT_COUNT > index)
    {
        ret = 0;
        if (!isOffline)
        {
            /* This is an online alert and we don't care which one it is */
            s_uxNotifyBit = s_uxAlarm;
            configPRINTF(("[Alert] Starting online timer!\r\n"));
            ret = 0;
        }
        else
        {
            switch (s_aisAlerts[index].type)
            {
                case AIS_ALERT_TYPE_TIMER:
                    s_uxNotifyBit = s_uxTimer;
                    if (isOffline)
                    {
                        // Play appropriate audio
                        if (NULL != s_offlineAudioEventGroup)
                        {
                            xEventGroupSetBits(s_offlineAudioEventGroup, OFFLINE_AUDIO_TIMER);
                            configPRINTF(("[Alert] Starting offline timer!\r\n"));
                        }
                    }
                    break;
                case AIS_ALERT_TYPE_ALARM:
                    s_uxNotifyBit = s_uxAlarm;
                    if (isOffline)
                    {
                        // Play appropriate audio
                        if (NULL != s_offlineAudioEventGroup)
                        {
                            xEventGroupSetBits(s_offlineAudioEventGroup, OFFLINE_AUDIO_ALARM);
                            configPRINTF(("[Alert] Starting offline alarm!\r\n"));
                        }
                    }
                    break;
                case AIS_ALERT_TYPE_REMINDER:
                    s_uxNotifyBit = s_uxReminder;
                    if (isOffline)
                    {
                        // Play appropriate audio
                        if (NULL != s_offlineAudioEventGroup)
                        {
                            xEventGroupSetBits(s_offlineAudioEventGroup, OFFLINE_AUDIO_RMNDR);
                            configPRINTF(("[Alert] Starting offline reminder!\r\n"));
                        }
                    }
                    break;
                case AIS_ALERT_TYPE_INVALID:
                default:
                    ret = -2;
                    break;
            }
        }
        ux_attention_set_state(s_uxNotifyBit);
    }

    return ret;
}

int32_t AIS_Alerts_MarkAsTriggered(uint32_t index)
{
    int32_t ret = -1;

    if (AIS_APP_MAX_ALERT_COUNT > index)
    {
        ret = -2;

        if (NULL != s_aisAlertLock)
        {
            if (pdTRUE == xSemaphoreTake(s_aisAlertLock, portMAX_DELAY))
            {
                ret                     = 0;
                s_aisAlerts[index].idle = false;
                xSemaphoreGive(s_aisAlertLock);
            }
        }
    }

    return ret;
}

int32_t AIS_Alerts_MarkAsTriggeredOffline(uint32_t index)
{
    int32_t ret = -1;

    if (AIS_APP_MAX_ALERT_COUNT > index)
    {
        ret = -2;

        if (NULL != s_aisAlertLock)
        {
            if (pdTRUE == xSemaphoreTake(s_aisAlertLock, portMAX_DELAY))
            {
                // Set ram copy to triggered
                s_aisAlerts[index].idle = false;

                // Update flash file
                uint32_t fileSize = sizeof(s_aisAlerts);

                // Allocate ram space for current file
                ais_alert_t *flashFile = (ais_alert_t *)pvPortMalloc(fileSize);

                if (NULL == flashFile)
                {
                    ret = -3;
                    goto exit;
                }

                // Read current file into temporary file buffer
                ret = SLN_FLASH_MGMT_Read(AIS_ALERT_FILE_NAME, (uint8_t *)flashFile, &fileSize);

                /* Due to the CRC implementation, CRC will always fail */
                if ((0 != ret) && (SLN_FLASH_MGMT_EENCRYPT2 == ret))
                {
                    ret = -4;
                    goto exit;
                }

                // Clear all bits in Flash to invalidate the entry in this file space
                flashFile[index].idle = 0;

                // Over write flash file
                ret = SLN_FLASH_MGMT_Update(AIS_ALERT_FILE_NAME, (uint8_t *)flashFile, &fileSize);

                if (0 != ret)
                {
                    ret = -5;
                    goto exit;
                }

            exit:
                if (NULL != flashFile)
                {
                    memset(flashFile, 0x00, fileSize);
                }
                vPortFree(flashFile);
                flashFile = NULL;

                xSemaphoreGive(s_aisAlertLock);
            }
        }
    }

    return ret;
}

uint64_t AIS_Alerts_GetScheduledTime(uint32_t index)
{
    uint64_t ret = 0;

    if (AIS_APP_MAX_ALERT_COUNT > index)
    {
        ret = s_aisAlerts[index].scheduledTime;
    }

    return ret;
}

void AIS_Alerts_GetPendingAlertIndices(uint32_t *indices)
{
    if (NULL != indices)
    {
        for (uint32_t idx = 0; idx < AIS_APP_MAX_ALERT_COUNT; idx++)
        {
            if (false == s_aisAlerts[idx].idle)
            {
                indices[idx] = 0xFFFFFFFFU;
            }
        }
    }
}

bool AIS_Alerts_GetIdleState(uint32_t index)
{
    bool ret = false;

    if (AIS_APP_MAX_ALERT_COUNT > index)
    {
        if (true == s_aisAlerts[index].valid)
        {
            ret = s_aisAlerts[index].idle;
        }
    }

    return ret;
}

int32_t AIS_Alerts_GetDeletedList(alertTokenList_t *deleteList, uint32_t *deleteCount)
{
    if ((NULL != deleteList) && (NULL != deleteCount))
    {
        /* Get all alerts marked as ready for delete and pass back how many there are */
        *deleteCount = 0U;
        for (uint32_t idx = 0; idx < AIS_APP_MAX_ALERT_COUNT; idx++)
        {
            volatile bool isValid    = s_aisAlerts[idx].valid;
            volatile size_t tokenLen = safe_strlen(s_aisAlerts[idx].token, AIS_MAX_ALERT_TOKEN_LEN_BYTES);
            volatile bool isEmpty    = (AIS_MAX_ALERT_TOKEN_LEN_BYTES == tokenLen) || (0 == tokenLen);
            if (!isValid && !isEmpty)
            {
                memcpy((*deleteList)[*deleteCount], s_aisAlerts[idx].token, AIS_MAX_ALERT_TOKEN_LEN_BYTES);
                (*deleteCount)++;
            }
        }
    }
    else if ((NULL == deleteList) && (NULL != deleteCount))
    {
        /* Get a count of how many are ready to delete */
        *deleteCount = 0U;
        for (uint32_t idx = 0; idx < AIS_APP_MAX_ALERT_COUNT; idx++)
        {
            volatile bool isValid    = s_aisAlerts[idx].valid;
            volatile size_t tokenLen = safe_strlen(s_aisAlerts[idx].token, AIS_MAX_ALERT_TOKEN_LEN_BYTES);
            volatile bool isEmpty    = (AIS_MAX_ALERT_TOKEN_LEN_BYTES == tokenLen) || (0 == tokenLen);
            if (!isValid && !isEmpty)
            {
                (*deleteCount)++;
            }
        }
    }
    else if ((NULL != deleteList) && (NULL == deleteCount))
    {
        /* Add all alerts to delete list, regardless of state */
        uint32_t tmpDeleteCount = 0U;
        for (uint32_t idx = 0; idx < AIS_APP_MAX_ALERT_COUNT; idx++)
        {
            memcpy((*deleteList)[idx], s_aisAlerts[idx].token, AIS_MAX_ALERT_TOKEN_LEN_BYTES);
            tmpDeleteCount++;
        }
    }
    else
    {
        /* Can't do anything with two null pointers */
        return -11;
    }

    return 0;
}

int32_t AIS_Alerts_GetAlertsList(alertTokenList_t *alertList, uint32_t *alertCount)
{
    int32_t ret = -1;
    if ((NULL != alertList) && (NULL != alertCount))
    {
        ret         = 0;
        *alertCount = 0U;

        for (uint32_t idx = 0; idx < AIS_APP_MAX_ALERT_COUNT; idx++)
        {
            volatile size_t tokenLen = safe_strlen(s_aisAlerts[idx].token, AIS_MAX_ALERT_TOKEN_LEN_BYTES);
            volatile bool isEmpty    = (AIS_MAX_ALERT_TOKEN_LEN_BYTES == tokenLen) || (0 == tokenLen);
            if (!isEmpty && s_aisAlerts[idx].valid)
            {
                memcpy((*alertList)[*alertCount], s_aisAlerts[idx].token, AIS_MAX_ALERT_TOKEN_LEN_BYTES);
                (*alertCount)++;
            }
        }
    }
    else
    {
        /* Can't do anything with two null pointers */
        ret = -11;
    }
    return ret;
}

int32_t AIS_Alerts_GetDuplicate(const char *token, size_t len)
{
    int ret = -1;

    for (uint32_t idx = 0; idx < AIS_APP_MAX_ALERT_COUNT; idx++)
    {
        if (memcmp(token, s_aisAlerts[idx].token, len) == 0)
        {
            ret = idx;
            break;
        }
    }
    return ret;
}
