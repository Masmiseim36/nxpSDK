/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief vision algorithm manager implementation.
 */

#include "fwk_sln_platform.h"

#include "fwk_log.h"
#include "fwk_message.h"
#include "fwk_sln_task.h"
#include "fwk_perf.h"

#include "fwk_vision_algo_manager.h"

typedef struct
{
    fwk_task_data_t commonData;
    /* registered vision algorithm devices */
    vision_algo_dev_t *devs[MAXIMUM_VISION_ALGO_DEV];
    /* vision algorithm request frame message */
    fwk_message_t VAlgoReqMsgs[MAXIMUM_VISION_ALGO_DEV * kVAlgoFrameID_Count];
    int frameReady[MAXIMUM_VISION_ALGO_DEV * kVAlgoFrameID_Count];

} vision_algo_task_data_t;

typedef struct
{
    fwk_task_t task;
    vision_algo_task_data_t algoData;
} vision_algo_task_t;

/*
 * vision algorithm manager task
 */
static vision_algo_task_t s_VisionAlgoTask;
#if FWK_SUPPORT_STATIC_ALLOCATION
FWKDATA static StackType_t s_VisionAlgoTaskStack[VISION_ALGO_MANAGER_TASK_STACK];
FWKDATA static StaticTask_t s_VisionAlgoTaskTCB;
static void *s_VisionAlgoTaskTCBReference = (void *)&s_VisionAlgoTaskTCB;
#else
static void *s_VisionAlgoTaskStack        = NULL;
static void *s_VisionAlgoTaskTCBReference = NULL;
#endif

/*
 * vision algorithm dev callback
 */
static int _FWK_VisionAlgoManager_DeviceCallback(
    int devId, valgo_event_t event, void *param, unsigned int size, uint8_t fromISR)
{
    fwk_task_id_t taskID;
    fwk_message_id_t msgID;
    switch (event)
    {
        case kVAlgoEvent_VisionResultUpdate:
        {
            taskID = kFWKTaskID_Output;
            msgID  = kFWKMessageID_VAlgoResultUpdate;
        }
        break;
        case kVAlgoEvent_VisionLedPwmControl:
        {
            taskID = kFWKTaskID_Output;
            msgID  = kFWKMessageID_InputNotify;
        }
        break;
        case kVAlgoEvent_VisionCamExpControl:
        {
            taskID = kFWKTaskID_Camera;
            msgID  = kFWKMessageID_InputNotify;
        }
        break;
        default:
            break;
    }

    switch (event)
    {
        case kVAlgoEvent_VisionResultUpdate:
        case kVAlgoEvent_VisionLedPwmControl:
        case kVAlgoEvent_VisionCamExpControl:
        {
            fwk_message_t *pMsg = (fwk_message_t *)SLN_MALLOC(sizeof(fwk_message_t));
            if (pMsg)
            {
                pMsg->freeAfterConsumed = 1;
                pMsg->id                = msgID;

                if (size == 0)
                {
                    pMsg->raw.data              = param;
                    pMsg->raw.freeAfterConsumed = 0;
                }
                else
                {
                    pMsg->raw.data = SLN_MALLOC(size);

                    if (pMsg->raw.data != NULL)
                    {
                        memcpy(pMsg->raw.data, param, size);
                    }

                    pMsg->raw.freeAfterConsumed = 1;
                }

                pMsg->raw.devId = devId;

                /* send the vision algorithm msg to dispatcher */
                if (fromISR)
                {
                    FWK_Message_PutFromIsr(taskID, &pMsg);
                }
                else
                {
                    FWK_Message_Put(taskID, &pMsg);
                }
            }
            else
                LOGE("Can't allocate memory for algorithm Message");
        }
        break;

        default:
            break;
    }

    return 0;
}

static int _FWK_VisionAlgoManager_TaskInit(fwk_task_data_t *pTaskData)
{
    if (pTaskData == NULL)
        return -1;

    int error                              = 0;
    hal_valgo_status_t hal_ret             = kStatus_HAL_ValgoError;
    vision_algo_task_data_t *pAlgoTaskData = (vision_algo_task_data_t *)pTaskData;

    /* init the camera dev */
    for (int i = 0; i < MAXIMUM_VISION_ALGO_DEV; i++)
    {
        vision_algo_dev_t *pDev = pAlgoTaskData->devs[i];
        if (pDev != NULL && pDev->ops->init != NULL)
        {
            LOGD("INIT vision algo dev[%d]", i);
            hal_ret = pDev->ops->init(pDev, _FWK_VisionAlgoManager_DeviceCallback, NULL);

            if (hal_ret)
            {
                LOGE("INIT vision algo dev [%d] error: %d", i, hal_ret);
                error = -1;
                continue;
            }

            /* send the frame requests to camera manager */
            for (int frame_index = 0; frame_index < kVAlgoFrameID_Count; frame_index++)
            {
                if (pDev->data.frames[frame_index].is_supported)
                {
                    LOGD("SEND vision algo dev[%d] frame[%d] request", i, frame_index);
                    fwk_message_t *pMsg;
                    pMsg                  = &pAlgoTaskData->VAlgoReqMsgs[i * kVAlgoFrameID_Count + frame_index];
                    pMsg->id              = kFWKMessageID_VAlgoRequestFrame;
                    pMsg->frame.devId     = pDev->id * kVAlgoFrameID_Count + frame_index;
                    pMsg->frame.height    = pDev->data.frames[frame_index].height;
                    pMsg->frame.width     = pDev->data.frames[frame_index].width;
                    pMsg->frame.pitch     = pDev->data.frames[frame_index].pitch;
                    pMsg->frame.left      = 0;
                    pMsg->frame.top       = 0;
                    pMsg->frame.right     = pMsg->frame.width - 1;
                    pMsg->frame.bottom    = pMsg->frame.height - 1;
                    pMsg->frame.rotate    = pDev->data.frames[frame_index].rotate;
                    pMsg->frame.flip      = pDev->data.frames[frame_index].flip;
                    pMsg->frame.swapByte  = pDev->data.frames[frame_index].swapByte;
                    pMsg->frame.format    = pDev->data.frames[frame_index].format;
                    pMsg->frame.srcFormat = pDev->data.frames[frame_index].srcFormat;
                    pMsg->frame.data      = pDev->data.frames[frame_index].data;
                    FWK_Message_Put(kFWKTaskID_Camera, &pMsg);
                }
            }
        }
    }

    return error;
}

static void _FWK_VisionAlgoManager_MessageHandle(fwk_message_t *pMsg, fwk_task_data_t *pTaskData)
{
    if ((pMsg == NULL) || (pTaskData == NULL))
        return;

    vision_algo_task_data_t *pAlgoTaskData = (vision_algo_task_data_t *)pTaskData;

    switch (pMsg->id)
    {
        case kFWKMessageID_VAlgoResponseFrame:
        {
            /* received one VALGO response frame */
            int valgo_dev_id        = pMsg->frame.devId / kVAlgoFrameID_Count;
            vision_algo_dev_t *pDev = pAlgoTaskData->devs[valgo_dev_id];
            if ((pDev != NULL) && (pDev->ops->run != NULL))
            {
                int allFramesReady                           = 1;
                pAlgoTaskData->frameReady[pMsg->frame.devId] = 1;
                for (int frame_index = 0; frame_index < kVAlgoFrameID_Count; frame_index++)
                {
                    if ((pDev->data.frames[frame_index].is_supported) &&
                        (pAlgoTaskData->frameReady[valgo_dev_id * kVAlgoFrameID_Count + frame_index] == 0))
                    {
                        allFramesReady = 0;
                    }
                }
                if (allFramesReady == 0)
                    break;

                hal_valgo_status_t status;
                status = pDev->ops->run(pDev, NULL);

                for (int frame_index = 0; frame_index < kVAlgoFrameID_Count; frame_index++)
                {
                    if (pDev->data.frames[frame_index].is_supported)
                    {
                        pAlgoTaskData->frameReady[valgo_dev_id * kVAlgoFrameID_Count + frame_index] = 0;
                    }
                }

                if (status == kStatus_HAL_ValgoSuccess)
                {
                    /* request new frames from the camera */
                    for (int frame_index = 0; frame_index < kVAlgoFrameID_Count; frame_index++)
                    {
                        if (pDev->data.frames[frame_index].is_supported)
                        {
                            fwk_message_t *pVAlgoReqMsg;
                            pVAlgoReqMsg =
                                &pAlgoTaskData->VAlgoReqMsgs[valgo_dev_id * kVAlgoFrameID_Count + frame_index];
                            pVAlgoReqMsg->id          = kFWKMessageID_VAlgoRequestFrame;
                            pVAlgoReqMsg->frame.devId = valgo_dev_id * kVAlgoFrameID_Count + frame_index;

                            FWK_Message_Put(kFWKTaskID_Camera, &pVAlgoReqMsg);
                        }
                    }
                    /* calculate the fps */
                    fwk_fps(kFWKFPSType_VAlgo, valgo_dev_id);
                }
            }
        }
        break;
        case kFWKMessageID_LpmPreEnterSleep:
        {
            vision_algo_dev_t *pDev = pAlgoTaskData->devs[0];
            if ((pDev != NULL) && (pDev->ops->deinit != NULL))
            {
                LOGD("DEINIT valgo dev \"%s\"", pDev->name);
                hal_valgo_status_t status = pDev->ops->deinit(pDev);
                if (status != kStatus_HAL_ValgoSuccess)
                {
                    LOGE("DEINIT valgo dev %s failed with error: %d", pDev->name, status);
                }
            }
        }
        break;

        case kFWKMessageID_InputNotify:
        {
            vision_algo_dev_t *pDev = pAlgoTaskData->devs[0];
            if ((pDev != NULL) && (pDev->ops->inputNotify != NULL))
            {
                hal_valgo_status_t status = pDev->ops->inputNotify(pDev, pMsg->raw.data);

                if (status != kStatus_HAL_ValgoSuccess)
                {
                    LOGE("inputNotify valgo dev %s failed with error: %d", pDev->name, status);
                }

                if (pMsg->raw.freeAfterConsumed)
                {
                    SLN_FREE(pMsg->raw.data);
                }
            }
        }
        break;

        case kFWKMessageID_InputFrameworkGetComponents:
        {
            framework_request_t frameworkRequest = pMsg->frameworkRequest;
            fwk_task_component_t fwkTaskComponent;
            framework_response_t framework_response;
            fwkTaskComponent.managerId = kFWKTaskID_VisionAlgo;
            for (int i = 0; i < MAXIMUM_VISION_ALGO_DEV; i++)
            {
                vision_algo_dev_t *pDev = pAlgoTaskData->devs[i];
                if (pDev != NULL)
                {
                    fwkTaskComponent.deviceId           = pDev->id;
                    fwkTaskComponent.deviceName         = pDev->name;
                    fwkTaskComponent.configs            = NULL;
                    framework_response.fwkTaskComponent = fwkTaskComponent;
                    if (frameworkRequest.respond != NULL)
                    {
                        frameworkRequest.respond(kFrameworkEvents_GetManagerComponents, &framework_response, false);
                    }
                }
            }

            frameworkRequest.respond(kFrameworkEvents_GetManagerComponents, NULL, true);
        }
        break;

        default:
            break;
    }
}

int FWK_VisionAlgoManager_Init()
{
    memset(s_VisionAlgoTask.algoData.devs, 0, sizeof(s_VisionAlgoTask.algoData.devs));

    return 0;
}

int FWK_VisionAlgoManager_Start()
{
    /* create the vision algorithm manager task */
    LOGD("[vision_algo_manager]:Starting...");
    int error = 0;

    s_VisionAlgoTask.task.msgHandle  = _FWK_VisionAlgoManager_MessageHandle;
    s_VisionAlgoTask.task.taskInit   = _FWK_VisionAlgoManager_TaskInit;
    s_VisionAlgoTask.task.data       = (fwk_task_data_t *)&(s_VisionAlgoTask.algoData);
    s_VisionAlgoTask.task.taskId     = kFWKTaskID_VisionAlgo;
    s_VisionAlgoTask.task.delayMs    = 1;
    s_VisionAlgoTask.task.taskStack  = s_VisionAlgoTaskStack;
    s_VisionAlgoTask.task.taskBuffer = s_VisionAlgoTaskTCBReference;
    FWK_Task_Start((fwk_task_t *)&s_VisionAlgoTask.task, VISION_ALGO_MANAGER_TASK_NAME, VISION_ALGO_MANAGER_TASK_STACK,
                   VISION_ALGO_MANAGER_TASK_PRIORITY);

    LOGD("[vision_algo_manager]:Started");
    return error;
}

int FWK_VisionAlgoManager_Deinit()
{
    return 0;
}

int FWK_VisionAlgoManager_DeviceRegister(vision_algo_dev_t *dev)
{
    int error = -1;

    for (int i = 0; i < MAXIMUM_VISION_ALGO_DEV; i++)
    {
        if (s_VisionAlgoTask.algoData.devs[i] == NULL)
        {
            dev->id                           = i;
            s_VisionAlgoTask.algoData.devs[i] = dev;
            return 0;
        }
    }

    return error;
}
