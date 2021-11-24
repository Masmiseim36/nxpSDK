/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief low power mode manager implementation.
 */

#include "fwk_sln_platform.h"

#include "fwk_log.h"
#include "fwk_message.h"
#include "fwk_sln_task.h"

#include "fwk_lpm_manager.h"

#include "hal_lpm_dev.h"

typedef struct _lpm_manager
{
    lpm_dev_t *dev;                  /* registered lpm devices */
    hal_lpm_manager_status_t enable; /* enable/disable sleep */
    hal_lpm_mode_t currentMode;      /* current sleep mode */
    fwk_message_t lpmNotifyMsg;      /* pre-enter sleep msg notify other manager */
    List_t lpmRequestList;           /* manage request list */
} lpm_manager_t;

typedef struct
{
    void *dev;
    char requestName[LPM_REQUEST_NAME_MAX_LENGTH];
    int8_t usageCount;
    ListItem_t requestListItem;
} lpm_request_item_t;

lpm_manager_t s_LpmManager;

static int _FWK_LpmManager_Lock()
{
    if (s_LpmManager.dev == NULL || s_LpmManager.dev->ops->lock == NULL)
    {
        LOGE("Lpm lock func is null");
        return kStatus_HAL_LpmLockNull;
    }

    return s_LpmManager.dev->ops->lock(s_LpmManager.dev);
}

static int _FWK_LpmManager_Unlock()
{
    if (s_LpmManager.dev == NULL || s_LpmManager.dev->ops->unlock == NULL)
    {
        LOGE("Lpm lock func is null");
        return kStatus_HAL_LpmLockNull;
    }

    return s_LpmManager.dev->ops->unlock(s_LpmManager.dev);
}

static int _FWK_LpmManager_PreEnterSleepTimerCallback(lpm_dev_t *dev)
{
    int ret = kStatus_HAL_LpmSuccess;

    lpm_dev_t *pDev = s_LpmManager.dev;
    if (pDev != NULL && pDev->ops->enterSleep != NULL)
    {
        ret = pDev->ops->enterSleep(pDev, s_LpmManager.currentMode);
    }
    else
    {
        ret = kStatus_HAL_LpmError;
    }

    return ret;
}

static int _FWK_LpmManager_PreEnterSleep(lpm_dev_t *dev)
{
    int ret = kStatus_HAL_LpmSuccess;

    lpm_dev_t *pDev     = s_LpmManager.dev;
    fwk_message_t *pMsg = (fwk_message_t *)&s_LpmManager.lpmNotifyMsg;

    if (pMsg)
    {
        pMsg->freeAfterConsumed = 0;
        pMsg->id                = kFWKMessageID_LpmPreEnterSleep;

        for (int i = kFWKTaskID_Camera; i < kFWKTaskID_APPStart; i++)
        {
            /* send the lpm enter sleep to dispatcher */
            char *taskName = NULL;
            uint32_t taskPriority;

            FWK_Task_GetInfo(i, &taskName, &taskPriority);

            if (taskName != NULL)
            {
                FWK_Message_Put(i, &pMsg);
            }
        }
    }

    ret = pDev->ops->openPreEnterTimer(pDev);
    if (ret)
    {
        LOGE("Start lpm dev %d pre-enter sleep timer error %d", pDev->id, ret);
    }

    return ret;
}

static int FWK_LpmManager_TimerCallback(lpm_dev_t *dev)
{
    int ret = kStatus_HAL_LpmSuccess;

    List_t *pRequestList             = &(s_LpmManager.lpmRequestList);
    lpm_request_item_t *pRequestItem = NULL;
    lpm_dev_t *pDev                  = s_LpmManager.dev;

    if (s_LpmManager.enable == kLPMManagerStatus_SleepEnable)
    {
        ListItem_t *pxListItem, *pxNext;
        ListItem_t const *pxListEnd;
        pxListItem = listGET_HEAD_ENTRY(pRequestList);
        pxListEnd  = listGET_END_MARKER(pRequestList);
        while (pxListItem != pxListEnd)
        {
            pRequestItem = (lpm_request_item_t *)listGET_LIST_ITEM_OWNER(pxListItem);
            if ((pRequestItem != NULL) && (pRequestItem->dev != NULL) && (pRequestItem->requestName != NULL))
            {
                if (pRequestItem->usageCount > 0)
                {
                    break;
                }
            }

            pxNext     = listGET_NEXT(pxListItem);
            pxListItem = pxNext;
        }

        if (pxListItem == pxListEnd && (pRequestList->uxNumberOfItems > 0))
        {
            if (pDev != NULL && pDev->ops->enterSleep != NULL)
            {
                pDev->ops->stopTimer(pDev);
                _FWK_LpmManager_PreEnterSleep(pDev);
            }
        }
    }

    return ret;
}

int FWK_LpmManager_Init()
{
    s_LpmManager.dev         = NULL;
    s_LpmManager.enable      = kLPMManagerStatus_SleepDisable;
    s_LpmManager.currentMode = kLPMMode_Invalid;

    vListInitialise(&s_LpmManager.lpmRequestList);

    return kStatus_HAL_LpmSuccess;
}

int FWK_LpmManager_Deinit()
{
    return kStatus_HAL_LpmSuccess;
}

int FWK_LpmManager_Start()
{
    LOGD("[LpmManager]:Starting...");

    int ret = kStatus_HAL_LpmSuccess;

    lpm_dev_t *pDev = s_LpmManager.dev;

    if (pDev == NULL)
    {
        ret = kStatus_HAL_LpmError;
    }

    if (pDev != NULL && pDev->ops->init != NULL)
    {
        LOGD("[LpmManager]:INIT lpm dev[%d]", pDev->id);
        ret =
            s_LpmManager.dev->ops->init(pDev, FWK_LpmManager_TimerCallback, _FWK_LpmManager_PreEnterSleepTimerCallback);

        if (ret)
        {
            LOGE("INIT lpm dev [%d] error: %d", pDev->id, ret);
        }
    }

    if (pDev != NULL && pDev->ops->openTimer != NULL)
    {
        LOGD("START lpm dev [%d]", pDev->id);
        ret = s_LpmManager.dev->ops->openTimer(pDev);

        if (ret)
        {
            LOGE("START lpm dev [%d] error: %d", pDev->id, ret);
        }
    }

    LOGD("[LpmManager]:started %d", ret);
    return ret;
}

int FWK_LpmManager_DeviceRegister(lpm_dev_t *dev)
{
    int ret = kStatus_HAL_LpmSuccess;

    ret = FWK_LpmManager_Init();

    s_LpmManager.dev = dev;

    ret = FWK_LpmManager_Start();

    return ret;
}

int FWK_LpmManager_RegisterRequestHandler(hal_lpm_request_t *req)
{
    int ret = kStatus_HAL_LpmSuccess;

    if (!listLIST_IS_INITIALISED(&s_LpmManager.lpmRequestList))
    {
        LOGE("Lpm list not initialised %s", req->name);
        return kStatus_HAL_LpmError;
    }

    lpm_request_item_t *pRequestItem = SLN_MALLOC(sizeof(lpm_request_item_t));
    if (pRequestItem == NULL)
    {
        LOGE("Cannot allocate memory for lpm request (%s)!", req->name);
        return kStatus_HAL_LpmRegisterFail;
    }

    pRequestItem->dev        = req->dev;
    pRequestItem->usageCount = 0;
    strcpy(pRequestItem->requestName, req->name);
    vListInitialiseItem(&(pRequestItem->requestListItem));

    listSET_LIST_ITEM_OWNER(&(pRequestItem->requestListItem), pRequestItem);

    List_t *pRequestList = &s_LpmManager.lpmRequestList;
    vListInsertEnd(pRequestList, &(pRequestItem->requestListItem));

    return ret;
}

int FWK_LpmManager_UnregisterRequestHandler(hal_lpm_request_t *req)
{
    int ret = kStatus_HAL_LpmSuccess;

    if (!listLIST_IS_INITIALISED(&s_LpmManager.lpmRequestList))
    {
        LOGE("Lpm list not initialised %s", req->name);
        return kStatus_HAL_LpmError;
    }

    ListItem_t *pxListItem, *pxNext;
    ListItem_t const *pxListEnd;
    lpm_request_item_t *pRequestItem = NULL;
    List_t *pRequestList             = &s_LpmManager.lpmRequestList;
    pxListItem                       = listGET_HEAD_ENTRY(pRequestList);
    pxListEnd                        = listGET_END_MARKER(pRequestList);

    while (pxListItem != pxListEnd)
    {
        pRequestItem = (lpm_request_item_t *)listGET_LIST_ITEM_OWNER(pxListItem);

        if ((pRequestItem != NULL) && (pRequestItem->dev == req->dev))
        {
            break;
        }

        pxNext     = listGET_NEXT(pxListItem);
        pxListItem = pxNext;
    }

    if ((pRequestItem == NULL) || (pxListItem == pxListEnd))
    {
        LOGE("Failed to unregister handler for lpm request [%d]", req->dev);
        return kStatus_HAL_LpmUnregisterFail;
    }

    pRequestItem->dev        = NULL;
    pRequestItem->usageCount = 0;
    memset(pRequestItem->requestName, 0x00, sizeof(pRequestItem->requestName));

    uxListRemove(&(pRequestItem->requestListItem));

    SLN_FREE(pRequestItem);

    return ret;
}

int FWK_LpmManager_RuntimeGet(hal_lpm_request_t *req)
{
    int ret = kStatus_HAL_LpmSuccess;

    if (!listLIST_IS_INITIALISED(&s_LpmManager.lpmRequestList))
    {
        LOGE("Lpm list not initialised %s", req->name);
        return kStatus_HAL_LpmError;
    }

    ret = _FWK_LpmManager_Lock();
    if (ret != kStatus_HAL_LpmSuccess)
    {
        LOGE("Lpm runtime get lock error");
        return ret;
    }

    ListItem_t *pxListItem, *pxNext;
    ListItem_t const *pxListEnd;
    lpm_request_item_t *pRequestItem = NULL;
    List_t *pRequestList             = &s_LpmManager.lpmRequestList;
    pxListItem                       = listGET_HEAD_ENTRY(pRequestList);
    pxListEnd                        = listGET_END_MARKER(pRequestList);

    while (pxListItem != pxListEnd)
    {
        pRequestItem = (lpm_request_item_t *)listGET_LIST_ITEM_OWNER(pxListItem);

        if ((pRequestItem != NULL) && (pRequestItem->dev == req->dev))
        {
            pRequestItem->usageCount++;
            break;
        }

        pxNext     = listGET_NEXT(pxListItem);
        pxListItem = pxNext;
    }

    if ((pRequestItem == NULL) || (pxListItem == pxListEnd))
    {
        LOGE("Failed to \"get\" lpm runtime dev: %d", req->dev);
        ret = kStatus_HAL_LpmRuntimeGetFail;
    }

    _FWK_LpmManager_Unlock();

    return ret;
}

int FWK_LpmManager_RuntimePut(hal_lpm_request_t *req)
{
    int ret = kStatus_HAL_LpmSuccess;

    if (!listLIST_IS_INITIALISED(&s_LpmManager.lpmRequestList))
    {
        LOGE("Lpm list not initialised %s", req->name);
        return kStatus_HAL_LpmError;
    }

    ret = _FWK_LpmManager_Lock();
    if (ret != kStatus_HAL_LpmSuccess)
    {
        LOGE("Lpm runtime put lock error");
        return ret;
    }

    ListItem_t *pxListItem, *pxNext;
    ListItem_t const *pxListEnd;
    lpm_request_item_t *pRequestItem = NULL;
    List_t *pRequestList             = &s_LpmManager.lpmRequestList;
    pxListItem                       = listGET_HEAD_ENTRY(pRequestList);
    pxListEnd                        = listGET_END_MARKER(pRequestList);

    while (pxListItem != pxListEnd)
    {
        pRequestItem = (lpm_request_item_t *)listGET_LIST_ITEM_OWNER(pxListItem);

        if ((pRequestItem != NULL) && (pRequestItem->dev == req->dev))
        {
            pRequestItem->usageCount--;
            break;
        }

        pxNext     = listGET_NEXT(pxListItem);
        pxListItem = pxNext;
    }

    if ((pRequestItem == NULL) || (pxListItem == pxListEnd))
    {
        LOGE("Failed to \"put\" lpm runtime dev: %d", req->dev);
        ret = kStatus_HAL_LpmRuntimePutFail;
    }

    _FWK_LpmManager_Unlock();

    return ret;
}

int FWK_LpmManager_RuntimeSet(hal_lpm_request_t *req, int8_t count)
{
    int ret = kStatus_HAL_LpmSuccess;

    if (!listLIST_IS_INITIALISED(&s_LpmManager.lpmRequestList))
    {
        LOGE("Lpm list not initialised %s", req->name);
        return kStatus_HAL_LpmError;
    }

    ret = _FWK_LpmManager_Lock();
    if (ret != kStatus_HAL_LpmSuccess)
    {
        LOGE("Lpm runtime set lock error");
        return ret;
    }

    ListItem_t *pxListItem, *pxNext;
    ListItem_t const *pxListEnd;
    lpm_request_item_t *pRequestItem = NULL;
    List_t *pRequestList             = &s_LpmManager.lpmRequestList;
    pxListItem                       = listGET_HEAD_ENTRY(pRequestList);
    pxListEnd                        = listGET_END_MARKER(pRequestList);

    while (pxListItem != pxListEnd)
    {
        pRequestItem = (lpm_request_item_t *)listGET_LIST_ITEM_OWNER(pxListItem);

        if ((pRequestItem != NULL) && (pRequestItem->dev == req->dev))
        {
            pRequestItem->usageCount = count;
            break;
        }

        pxNext     = listGET_NEXT(pxListItem);
        pxListItem = pxNext;
    }

    if ((pRequestItem == NULL) || (pxListItem == pxListEnd))
    {
        LOGE("Failed to \"set\" lpm runtime dev: %d", req->dev);
        ret = kStatus_HAL_LpmRuntimeSetFail;
    }

    _FWK_LpmManager_Unlock();

    return ret;
}

int FWK_LpmManager_RequestStatus(unsigned int *totalUsageCount)
{
    int ret = kStatus_HAL_LpmSuccess;

    if (totalUsageCount == NULL)
    {
        LOGE("Invalid parameter");
        return kStatus_HAL_LpmError;
    }

    *totalUsageCount = 0;

    if (!listLIST_IS_INITIALISED(&s_LpmManager.lpmRequestList))
    {
        LOGE("Lpm list not initialised");
        return kStatus_HAL_LpmError;
    }

    ret = _FWK_LpmManager_Lock();
    if (ret != kStatus_HAL_LpmSuccess)
    {
        LOGE("Lpm runtime put lock error");
        return ret;
    }

    ListItem_t *pxListItem, *pxNext;
    ListItem_t const *pxListEnd;
    lpm_request_item_t *pRequestItem = NULL;
    List_t *pRequestList             = &s_LpmManager.lpmRequestList;
    pxListItem                       = listGET_HEAD_ENTRY(pRequestList);
    pxListEnd                        = listGET_END_MARKER(pRequestList);

    LOGD("[LpmManager]:Current request [%d]", pRequestList->uxNumberOfItems);
    while (pxListItem != pxListEnd)
    {
        pRequestItem = (lpm_request_item_t *)listGET_LIST_ITEM_OWNER(pxListItem);

        if ((pRequestItem != NULL) && (pRequestItem->dev != NULL))
        {
            LOGD("[LpmManager]: dev[%d], dev_name[%s], usage[%d]", pRequestItem->dev, pRequestItem->requestName,
                 pRequestItem->usageCount);
            *totalUsageCount += pRequestItem->usageCount;
        }

        pxNext     = listGET_NEXT(pxListItem);
        pxListItem = pxNext;
    }

    _FWK_LpmManager_Unlock();

    return ret;
}

int FWK_LpmManager_SetSleepMode(hal_lpm_mode_t sleepMode)
{
    int ret = kStatus_HAL_LpmSuccess;

    ret = _FWK_LpmManager_Lock();
    if (ret != kStatus_HAL_LpmSuccess)
    {
        LOGE("Lpm set sleep mode error[%d]", sleepMode);
        return kStatus_HAL_LpmError;
    }

    s_LpmManager.currentMode = sleepMode;

    _FWK_LpmManager_Unlock();

    return ret;
}

int FWK_LpmManager_EnableSleepMode(hal_lpm_manager_status_t enable)
{
    int ret = kStatus_HAL_LpmSuccess;

    ret = _FWK_LpmManager_Lock();
    if (ret != kStatus_HAL_LpmSuccess)
    {
        LOGE("Lpm enable sleep mode error[%d]", enable);
        return kStatus_HAL_LpmError;
    }

    s_LpmManager.enable = enable;

    _FWK_LpmManager_Unlock();

    return ret;
}
