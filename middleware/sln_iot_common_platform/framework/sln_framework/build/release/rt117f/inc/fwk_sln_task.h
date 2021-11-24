/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief SLN task declaration.
 */

#ifndef __FWK_SLN_TASK_H__
#define __FWK_SLN_TASK_H__

#include "fwk_common.h"
#include "fwk_message.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct
{
    QueueHandle_t queueHandle;
} fwk_task_data_t;

typedef struct
{
    fwk_task_data_t *data;
    fwk_task_id_t taskId;
    int delayMs;
    StackType_t *taskStack;
    StaticTask_t *taskBuffer;
    void (*msgHandle)(fwk_message_t *, fwk_task_data_t *);
    int (*taskInit)(fwk_task_data_t *);
} fwk_task_t;

void FWK_Task_Start(fwk_task_t *pTask, const char *taskName, int taskStackSize, int taskPriority);
int FWK_Task_GetInfo(fwk_task_id_t taskId, char **name, uint32_t *priority);
int FWK_Task_GetCount(uint8_t *count);
bool FWK_Task_IsRegistered(fwk_task_id_t taskId);

#if defined(__cplusplus)
}
#endif

#endif /*__FWK_SLN_TASK_H__*/
