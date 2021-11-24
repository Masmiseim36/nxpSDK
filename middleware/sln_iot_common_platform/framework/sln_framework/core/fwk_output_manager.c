/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief output device manager framework implementation.
 */

#include "fwk_sln_platform.h"

#include "fwk_log.h"
#include "fwk_message.h"
#include "fwk_sln_task.h"

#include "fwk_graphics.h"
#include "fwk_output_manager.h"

typedef struct
{
    fwk_task_data_t commonData;
    output_dev_t *devs[MAXIMUM_OUTPUT_DEV]; /* registered output devices */
    List_t outEventReceiverList;            /* registered output event receiver */
    int uiReceiverCount;
} output_task_data_t;

typedef struct
{
    fwk_task_t task;
    output_task_data_t outputData;
} output_task_t;

typedef struct
{
    output_dev_t *pDev;

    const output_dev_event_handler_t *handler;

    ListItem_t receiverListItem;
} output_event_receiver_t;

static output_task_t s_OutputTask;
#if FWK_SUPPORT_STATIC_ALLOCATION
FWKDATA static StackType_t s_OutputTaskStack[OUTPUT_MANAGER_TASK_STACK];
FWKDATA static StaticTask_t s_OutputTaskTCB;
static void *s_OutputTaskTCBReference = (void *)&s_OutputTaskTCB;
#else
static void *s_OutputTaskStack        = NULL;
static void *s_OutputTaskTCBReference = NULL;
#endif

static int _FWK_OutputManager_task_init(fwk_task_data_t *pTaskData)
{
    if (pTaskData == NULL)
        return -1;

    int error                           = 0;
    output_task_data_t *pOutputTaskData = (output_task_data_t *)pTaskData;

    /* init the output dev */
    for (int i = 0; i < MAXIMUM_OUTPUT_DEV; i++)
    {
        output_dev_t *pDev = pOutputTaskData->devs[i];
        if (pDev != NULL && pDev->ops->init != NULL)
        {
            LOGD("INIT output dev \"%s\"", pDev->name);
            error = pDev->ops->init(pDev);

            if (error)
            {
                LOGE("INIT output dev \"%s\" error: %d", pDev->name, error);
            }
        }
    }

    for (int i = 0; i < MAXIMUM_OUTPUT_DEV; i++)
    {
        output_dev_t *pDev = pOutputTaskData->devs[i];
        if (pDev != NULL && pDev->ops->start != NULL)
        {
            LOGD("START output dev \"%s\"", pDev->name);
            error = pDev->ops->start(pDev);

            if (error)
            {
                LOGE("START output dev \"%s\" error: %d", pDev->name, error);
                return error;
            }
        }
    }

    return error;
}

static void _FWK_OutputManager_MessageHandle(fwk_message_t *pMsg, fwk_task_data_t *pTaskData)
{
    ListItem_t *pxListItem, *pxNext;
    ListItem_t const *pxListEnd;

    if ((pMsg == NULL) || (pTaskData == NULL))
        return;

    static bool s_SleepModeIsOn         = false;
    output_task_data_t *pOutputTaskData = (output_task_data_t *)pTaskData;
    List_t *pReceiverList               = &(pOutputTaskData->outEventReceiverList);

    pxListItem                    = listGET_HEAD_ENTRY(pReceiverList);
    pxListEnd                     = listGET_END_MARKER(pReceiverList);
    output_event_receiver_t *pRec = NULL;

    switch (pMsg->id)
    {
        case kFWKMessageID_VAlgoASRResultUpdate:
        case kFWKMessageID_VAlgoResultUpdate:
        case kFWKMessageID_LpmPreEnterSleep:
        {
            if (s_SleepModeIsOn)
            {
                /* Don't forward any inference results */
                break;
            }

            while (pxListItem != pxListEnd)
            {
                pRec = (output_event_receiver_t *)listGET_LIST_ITEM_OWNER(pxListItem);

                if ((pRec != NULL) && (pRec->pDev != NULL) && (pRec->handler != NULL) &&
                    (pRec->handler->inferenceComplete != NULL))
                {
                    int error = 0;
                    if (pMsg->id == kFWKMessageID_VAlgoASRResultUpdate && pRec->pDev->attr.type != kOutputDevType_UI)
                    {
                        error = pRec->handler->inferenceComplete(pRec->pDev, kOutputAlgoSource_Voice, pMsg->raw.data);
                    }
                    else if (pMsg->id == kFWKMessageID_VAlgoResultUpdate)
                    {
                        error = pRec->handler->inferenceComplete(pRec->pDev, kOutputAlgoSource_Vision, pMsg->raw.data);
                        if ((error == kStatus_HAL_OutputSuccess) && (pRec->pDev->attr.type == kOutputDevType_UI))
                        {
                            /* only support one UI receiver currently */
                            fwk_message_t *pMsg     = (fwk_message_t *)SLN_MALLOC(sizeof(fwk_message_t));
                            pMsg->id                = kFWKMessageID_DispatcherRequestShowOverlay;
                            pMsg->freeAfterConsumed = 1;
                            pMsg->overlay.pSurface  = pRec->pDev->attr.pSurface;
                            FWK_Message_Put(kFWKTaskID_Camera, &pMsg);
                        }
                    }
                    else if (pMsg->id == kFWKMessageID_LpmPreEnterSleep)
                    {
                        error = pRec->handler->inferenceComplete(pRec->pDev, kOutputAlgoSource_LPM, pMsg->raw.data);
                        s_SleepModeIsOn = true;
                    }

                    if (error)
                    {
                        LOGE("Output device \"%s\":inference result event handler resulted in an error: %d",
                             pRec->pDev->name, error);
                    }
                }

                pxNext     = listGET_NEXT(pxListItem);
                pxListItem = pxNext;
            }
        }
        break;

        case kFWKMessageID_InputNotify:
        {
            while (pxListItem != pxListEnd)
            {
                pRec = (output_event_receiver_t *)listGET_LIST_ITEM_OWNER(pxListItem);
                if ((pRec != NULL) && (pRec->pDev != NULL) && (pRec->handler != NULL) &&
                    (pRec->handler->inputNotify != NULL))
                {
                    int error = pRec->handler->inputNotify(pRec->pDev, pMsg->raw.data);

                    if (error)
                    {
                        LOGE("Output device \"%s\":input notify event handler resulted in an error: %d",
                             pRec->pDev->name, error);
                    }
                }

                pxNext     = listGET_NEXT(pxListItem);
                pxListItem = pxNext;
            }
        }
        break;

        case kFWKMessageID_InputAudioReceived:
        {
            while (pxListItem != pxListEnd)
            {
                pRec = (output_event_receiver_t *)listGET_LIST_ITEM_OWNER(pxListItem);
                if ((pRec != NULL) && (pRec->pDev != NULL) && (pRec->handler != NULL) && (pRec->handler->dump != NULL))
                {
                    pRec->handler->dump(pRec->pDev, &pMsg->audio);
                }
                pxNext     = listGET_NEXT(pxListItem);
                pxListItem = pxNext;
            }
        }
        break;

        case kFWKMessageID_InputFrameworkGetComponents:
        {
            framework_request_t frameworkRequest = pMsg->frameworkRequest;
            fwk_task_component_t fwkTaskComponent;
            framework_response_t frameworkResponse;
            fwkTaskComponent.managerId = kFWKTaskID_Output;
            while (pxListItem != pxListEnd)
            {
                pRec               = (output_event_receiver_t *)listGET_LIST_ITEM_OWNER(pxListItem);
                output_dev_t *pDev = pRec->pDev;
                if (pDev != NULL)
                {
                    fwkTaskComponent.deviceId          = pDev->id;
                    fwkTaskComponent.deviceName        = pDev->name;
                    fwkTaskComponent.configs           = pDev->configs;
                    frameworkResponse.fwkTaskComponent = fwkTaskComponent;
                    if (frameworkRequest.respond != NULL)
                    {
                        frameworkRequest.respond(kFrameworkEvents_GetManagerComponents, &frameworkResponse, false);
                    }
                }
                pxNext     = listGET_NEXT(pxListItem);
                pxListItem = pxNext;
            }

            frameworkRequest.respond(kFrameworkEvents_GetManagerComponents, NULL, true);
        }
        break;

        case kFWKMessageID_InputFrameworkGetDeviceConfigs:
        {
            framework_request_t frameworkRequest = pMsg->frameworkRequest;
            fwk_task_component_t fwkTaskComponent;
            framework_response_t frameworkResponse;
            fwkTaskComponent.managerId = kFWKTaskID_Output;
            while (pxListItem != pxListEnd)
            {
                pRec               = (output_event_receiver_t *)listGET_LIST_ITEM_OWNER(pxListItem);
                output_dev_t *pDev = pRec->pDev;

                /* Found device with matching name and/or ID, so print configs associated with it */
                /* TODO: Extend to use manager id + dev_id as well as device name*/
                if (pDev != NULL && ((strcmp(pDev->name, frameworkRequest.deviceName) == 0)))
                {
                    fwkTaskComponent.deviceId          = pDev->id;
                    fwkTaskComponent.deviceName        = pDev->name;
                    fwkTaskComponent.configs           = pDev->configs;
                    frameworkResponse.fwkTaskComponent = fwkTaskComponent;

                    if (frameworkRequest.respond != NULL)
                    {
                        frameworkRequest.respond(kFrameworkEvents_GetDeviceConfigs, &frameworkResponse, false);
                    }
                    return;
                }
                pxNext     = listGET_NEXT(pxListItem);
                pxListItem = pxNext;
            }
            /* Signifies that a matching device was never found */
            frameworkRequest.respond(kFrameworkEvents_GetDeviceConfigs, NULL, true);
        }
        break;

        default:
            break;
    }

    if (pMsg->raw.freeAfterConsumed)
    {
        SLN_FREE(pMsg->raw.data);
    }
}

int FWK_OutputManager_Init()
{
    for (int i = 0; i < MAXIMUM_OUTPUT_DEV; i++)
    {
        s_OutputTask.outputData.devs[i] = NULL;
    }
    List_t *pReceiverList                   = &s_OutputTask.outputData.outEventReceiverList;
    s_OutputTask.outputData.uiReceiverCount = 0;
    vListInitialise(pReceiverList);

    return 0;
}

int FWK_OutputManager_Start()
{
    LOGD("[OutputManager]:Starting...");
    int error = 0;

    s_OutputTask.task.msgHandle  = _FWK_OutputManager_MessageHandle;
    s_OutputTask.task.taskInit   = _FWK_OutputManager_task_init;
    s_OutputTask.task.data       = (fwk_task_data_t *)&(s_OutputTask.outputData);
    s_OutputTask.task.taskId     = kFWKTaskID_Output;
    s_OutputTask.task.delayMs    = 1;
    s_OutputTask.task.taskStack  = s_OutputTaskStack;
    s_OutputTask.task.taskBuffer = s_OutputTaskTCBReference;

    FWK_Task_Start((fwk_task_t *)&s_OutputTask.task, OUTPUT_MANAGER_TASK_NAME, OUTPUT_MANAGER_TASK_STACK,
                   OUTPUT_MANAGER_TASK_PRIORITY);

    LOGD("[OutputManager]:Started");

    return error;
}

int FWK_OutputManager_Deinit()
{
    return 0;
}

int FWK_OutputManager_DeviceRegister(output_dev_t *dev)
{
    int error = -1;

    for (int i = 0; i < MAXIMUM_OUTPUT_DEV; i++)
    {
        if (s_OutputTask.outputData.devs[i] == NULL)
        {
            dev->id                         = i;
            s_OutputTask.outputData.devs[i] = dev;
            return 0;
        }
    }

    return error;
}

int FWK_OutputManager_RegisterEventHandler(const output_dev_t *dev, const output_dev_event_handler_t *handler)
{
    int error = 0;

    if (dev->attr.type == kOutputDevType_UI)
    {
        if (s_OutputTask.outputData.uiReceiverCount == 1)
        {
            LOGE(
                "[ERROR] A UI event receiver device has already been registered. Currently only one device can be "
                "registered as a UI event receiver.\r\n");
            return -1;
        }

        s_OutputTask.outputData.uiReceiverCount++;
    }

    output_event_receiver_t *pRec = SLN_MALLOC(sizeof(output_event_receiver_t));
    if (pRec == NULL)
    {
        LOGE("Cannot allocate memory for outevent receiver device \"%s\"!", dev->name);
        return error;
    }
    pRec->pDev    = dev;
    pRec->handler = handler;
    vListInitialiseItem(&(pRec->receiverListItem));

    listSET_LIST_ITEM_OWNER(&(pRec->receiverListItem), pRec);

    List_t *pReceiverList = &s_OutputTask.outputData.outEventReceiverList;
    vListInsertEnd(pReceiverList, &(pRec->receiverListItem));

    return error;
}

int FWK_OutputManager_UnregisterEventHandler(const output_dev_t *dev)
{
    int error = 0;

    ListItem_t *pxListItem, *pxNext;
    ListItem_t const *pxListEnd;
    List_t *pReceiverList         = &s_OutputTask.outputData.outEventReceiverList;
    pxListItem                    = listGET_HEAD_ENTRY(pReceiverList);
    pxListEnd                     = listGET_END_MARKER(pReceiverList);
    output_event_receiver_t *pRec = NULL;

    while (pxListItem != pxListEnd)
    {
        pRec = (output_event_receiver_t *)listGET_LIST_ITEM_OWNER(pxListItem);

        if ((pRec != NULL) && (pRec->pDev != NULL) && (pRec->handler != NULL) && (pRec->pDev == dev))
        {
            break;
        }

        pxNext     = listGET_NEXT(pxListItem);
        pxListItem = pxNext;
    }
    if ((pRec == NULL) || (pxListItem == pxListEnd))
    {
        LOGE("Failed to unregister handler for dev \"%s\"", dev->name);
        return error;
    }

    uxListRemove(&(pRec->receiverListItem));

    pRec->pDev    = NULL;
    pRec->handler = NULL;

    SLN_FREE(pRec);

    if (dev->attr.type == kOutputDevType_UI)
    {
        s_OutputTask.outputData.uiReceiverCount--;
    }

    return error;
}
