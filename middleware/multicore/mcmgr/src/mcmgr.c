/*
 * Copyright (c) 2014-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "mcmgr.h"
#include "mcmgr_internal_core_api.h"

mcmgr_event_t MCMGR_eventTable[kMCMGR_EventTableLength] = {0};

mcmgr_status_t MCMGR_RegisterEvent(mcmgr_event_type_t type, mcmgr_event_callback_t callback, void *callbackData)
{
    if (type >= kMCMGR_EventTableLength)
    {
        return kStatus_MCMGR_Error;
    }
    /* Make sure any old handler is inactive */
    MCMGR_eventTable[type].callback = NULL;
    /* Install callback data first */
    MCMGR_eventTable[type].callbackData = callbackData;
    /* Install the callback */
    MCMGR_eventTable[type].callback = callback;

    return kStatus_MCMGR_Success;
}

static mcmgr_status_t MCMGR_TriggerEventCommon(mcmgr_event_type_t type, uint16_t eventData, bool forcedWrite)
{
    uint32_t remoteData;
    if (type >= kMCMGR_EventTableLength)
    {
        return kStatus_MCMGR_Error;
    }

    mcmgr_core_t coreNum = MCMGR_GetCurrentCore();
    if (coreNum < g_mcmgrSystem.coreCount)
    {
        remoteData = (((uint32_t)type) << 16) | eventData;
        return mcmgr_trigger_event_internal(remoteData, forcedWrite);
    }
    return kStatus_MCMGR_Error;
}

mcmgr_status_t MCMGR_TriggerEvent(mcmgr_event_type_t type, uint16_t eventData)
{
    return MCMGR_TriggerEventCommon(type, eventData, false);
}

mcmgr_status_t MCMGR_TriggerEventForce(mcmgr_event_type_t type, uint16_t eventData)
{
    return MCMGR_TriggerEventCommon(type, eventData, true);
}

static void MCMGR_StartupDataEventHandler(uint16_t startupDataChunk, void *context)
{
    mcmgr_core_context_t *coreContext = (mcmgr_core_context_t *)context;

    switch (coreContext->state)
    {
        case kMCMGR_StartupGettingLowCoreState:
            coreContext->startupData = startupDataChunk; /* Receive the low part */
            coreContext->state = kMCMGR_StartupGettingHighCoreState;
            MCMGR_TriggerEvent(kMCMGR_FeedStartupDataEvent, kMCMGR_StartupGettingHighCoreState);
            break;

        case kMCMGR_StartupGettingHighCoreState:
            coreContext->startupData |= ((uint32_t)startupDataChunk) << 16;
            coreContext->state = kMCMGR_RunningCoreState;
            MCMGR_TriggerEvent(kMCMGR_FeedStartupDataEvent, kMCMGR_RunningCoreState);
            break;

        default:
            /* All the cases have been listed above, the default clause should not be reached. */
            break;
    }
}

static void MCMGR_FeedStartupDataEventHandler(uint16_t startupDataChunk, void *context)
{
    mcmgr_core_context_t *coreContext = (mcmgr_core_context_t *)context;

    switch ((mcmgr_core_state_t)startupDataChunk)
    {
        case kMCMGR_StartupGettingLowCoreState:
            MCMGR_TriggerEvent(kMCMGR_StartupDataEvent, (uint16_t)(coreContext->startupData & 0xFFFF));
            coreContext->state = (mcmgr_core_state_t)startupDataChunk;
            break;

        case kMCMGR_StartupGettingHighCoreState:
            MCMGR_TriggerEvent(kMCMGR_StartupDataEvent, (uint16_t)((coreContext->startupData) >> 16));
            coreContext->state = (mcmgr_core_state_t)startupDataChunk;
            break;

        case kMCMGR_RunningCoreState:
            coreContext->state = (mcmgr_core_state_t)startupDataChunk;
            break;

        default:
            /* All the cases have been listed above, the default clause should not be reached. */
            break;
    }
}

mcmgr_status_t MCMGR_EarlyInit(void)
{
    mcmgr_core_t coreNum = MCMGR_GetCurrentCore();
    if (coreNum < g_mcmgrSystem.coreCount)
    {
        return mcmgr_early_init_internal(coreNum);
    }
    return kStatus_MCMGR_Error;
}

mcmgr_status_t MCMGR_Init(void)
{
    mcmgr_core_t coreNum = MCMGR_GetCurrentCore();
    if (coreNum < g_mcmgrSystem.coreCount)
    {
        /* Register critical and generic event handlers */
        MCMGR_RegisterEvent(kMCMGR_StartupDataEvent, MCMGR_StartupDataEventHandler, (void *)&s_mcmgrCoresContext[coreNum]);
        MCMGR_RegisterEvent(kMCMGR_FeedStartupDataEvent, MCMGR_FeedStartupDataEventHandler,
                            (void *)&s_mcmgrCoresContext[!coreNum]);
        return mcmgr_late_init_internal(coreNum);
    }
    return kStatus_MCMGR_Error;
}

mcmgr_status_t MCMGR_StartCore(mcmgr_core_t coreNum, void *bootAddress, uint32_t startupData, mcmgr_start_mode_t mode)
{
    mcmgr_status_t ret;

    if (coreNum < g_mcmgrSystem.coreCount)
    {
        /* Pass the startupData - LSB first */
        s_mcmgrCoresContext[coreNum].startupData = startupData;
        /* the startup data is sent asynchronously */
        ret = mcmgr_start_core_internal(coreNum, bootAddress);

        if (ret == kStatus_MCMGR_Success)
        {
            if (mode == kMCMGR_Start_Synchronous)
            {
                /* Wait until the second core reads and confirms the startup data */
                while (s_mcmgrCoresContext[coreNum].state != kMCMGR_RunningCoreState)
                {
                }
            }
            return kStatus_MCMGR_Success;
        }
    }
    return kStatus_MCMGR_Error;
}

mcmgr_status_t MCMGR_GetStartupData(uint32_t *startupData)
{
    mcmgr_core_t coreNum = MCMGR_GetCurrentCore();
    if (coreNum < g_mcmgrSystem.coreCount)
    {
        if (s_mcmgrCoresContext[coreNum].state == kMCMGR_ResetCoreState)
        {
            s_mcmgrCoresContext[coreNum].state = kMCMGR_StartupGettingLowCoreState;
            MCMGR_TriggerEvent(kMCMGR_FeedStartupDataEvent, kMCMGR_StartupGettingLowCoreState);
        }
        return mcmgr_get_startup_data_internal(coreNum, startupData);
    }
    return kStatus_MCMGR_Error;
}

mcmgr_status_t MCMGR_StopCore(mcmgr_core_t coreNum)
{
    if (coreNum < g_mcmgrSystem.coreCount)
    {
        return mcmgr_stop_core_internal(coreNum);
    }
    return kStatus_MCMGR_Error;
}

int32_t MCMGR_GetVersion(void)
{
    return kMCMGR_Version;
}

mcmgr_status_t MCMGR_GetCoreProperty(mcmgr_core_t coreNum,
                                     mcmgr_core_property_t property,
                                     void *value,
                                     uint32_t *length)
{
    if (coreNum < g_mcmgrSystem.coreCount)
    {
        return mcmgr_get_core_property_internal(coreNum, property, value, length);
    }
    return kStatus_MCMGR_Error;
}

uint32_t MCMGR_GetCoreCount(void)
{
    return g_mcmgrSystem.coreCount;
}

mcmgr_core_t MCMGR_GetCurrentCore(void)
{
    return mcmgr_get_current_core_internal();
}
