/*
 * Copyright (c) 2018-2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mcmgr.h"
#include "mcmgr_internal_core_api.h"
#include "fsl_device_registers.h"
#include "fsl_mailbox.h"

/* Count of cores in the system */
#define MCMGR_CORECOUNT 2

/* Count of memory regions in the system */
#define MCMGR_MEMREGCOUNT 2

volatile mcmgr_core_context_t s_mcmgrCoresContext[MCMGR_CORECOUNT] = {
    {.state = kMCMGR_ResetCoreState, .startupData = 0}, {.state = kMCMGR_ResetCoreState, .startupData = 0}};

/* Initialize structure with informations of all cores */
static const mcmgr_core_info_t s_mcmgrCores[MCMGR_CORECOUNT] = {
    {.coreType = kMCMGR_CoreTypeCortexM33, .coreName = "Main"},
    {.coreType = kMCMGR_CoreTypeCortexM33, .coreName = "Secondary"}};

const mcmgr_system_info_t g_mcmgrSystem = {
    .coreCount = MCMGR_CORECOUNT, .memRegCount = MCMGR_MEMREGCOUNT, .cores = s_mcmgrCores};

mcmgr_status_t mcmgr_early_init_internal(mcmgr_core_t coreNum)
{
    if ((uint32_t)coreNum < g_mcmgrSystem.coreCount)
    {
        MAILBOX_Init(MAILBOX);

        /* Trigger core up event here, core is starting! */
        return MCMGR_TriggerEvent(kMCMGR_RemoteCoreUpEvent, 0);
    }
    return kStatus_MCMGR_Error;
}

mcmgr_status_t mcmgr_late_init_internal(mcmgr_core_t coreNum)
{
#if defined(FSL_FEATURE_MAILBOX_SIDE_A)
    NVIC_SetPriority(MAILBOX_IRQn, 5);
#else
    NVIC_SetPriority(MAILBOX_IRQn, 2);
#endif

    NVIC_EnableIRQ(MAILBOX_IRQn);

    return kStatus_MCMGR_Success;
}

mcmgr_status_t mcmgr_start_core_internal(mcmgr_core_t coreNum, void *bootAddress)
{
    if (coreNum != kMCMGR_Core1)
    {
        return kStatus_MCMGR_Error;
    }

    SYSCON->CPUCFG |= SYSCON_CPUCFG_CPU1ENABLE_MASK;

    /* Boot source for Core 1 from RAM */
    SYSCON->CPBOOT = SYSCON_CPBOOT_CPBOOT(bootAddress);

    uint32_t temp = SYSCON->CPUCTRL;
    temp |= 0xc0c48000U;
    SYSCON->CPUCTRL = temp | SYSCON_CPUCTRL_CPU1RSTEN_MASK | SYSCON_CPUCTRL_CPU1CLKEN_MASK;
    SYSCON->CPUCTRL = (temp | SYSCON_CPUCTRL_CPU1CLKEN_MASK) & (~SYSCON_CPUCTRL_CPU1RSTEN_MASK);

    return kStatus_MCMGR_Success;
}

mcmgr_status_t mcmgr_get_startup_data_internal(mcmgr_core_t coreNum, uint32_t *startupData)
{
    if (coreNum != kMCMGR_Core1)
    {
        return kStatus_MCMGR_Error;
    }
    if (startupData == ((void *)0))
    {
        return kStatus_MCMGR_Error;
    }

    if (s_mcmgrCoresContext[coreNum].state >= kMCMGR_RunningCoreState)
    {
        *startupData = s_mcmgrCoresContext[coreNum].startupData;
        return kStatus_MCMGR_Success;
    }
    else
    {
        return kStatus_MCMGR_NotReady;
    }
}

mcmgr_status_t mcmgr_stop_core_internal(mcmgr_core_t coreNum)
{
    if (coreNum != kMCMGR_Core1)
    {
        return kStatus_MCMGR_Error;
    }
    uint32_t temp = SYSCON->CPUCTRL;
    temp |= 0xc0c48000U;

    /* hold in reset and disable clock */
    SYSCON->CPUCTRL = (temp | SYSCON_CPUCTRL_CPU1RSTEN_MASK) & (~SYSCON_CPUCTRL_CPU1CLKEN_MASK);
    return kStatus_MCMGR_Success;
}

mcmgr_status_t mcmgr_get_core_property_internal(mcmgr_core_t coreNum,
                                                mcmgr_core_property_t property,
                                                void *value,
                                                uint32_t *length)
{
    return kStatus_MCMGR_NotImplemented;
}

mcmgr_core_t mcmgr_get_current_core_internal(void)
{
#if defined(FSL_FEATURE_MAILBOX_SIDE_A)
    return kMCMGR_Core0;
#else
    return kMCMGR_Core1;
#endif
}

mcmgr_status_t mcmgr_trigger_event_internal(uint32_t remoteData, bool forcedWrite)
{
#if defined(FSL_FEATURE_MAILBOX_SIDE_A)
    mailbox_cpu_id_t cpu_id = kMAILBOX_CM33_Core1;
#else
    mailbox_cpu_id_t cpu_id = kMAILBOX_CM33_Core0;
#endif
    /* When forcedWrite is false, wait until the Mailbox Interrupt request register is cleared,
       i.e. until previously sent data is processed. */
    if (false == forcedWrite)
    {
        while (0U != MAILBOX_GetValue(MAILBOX, cpu_id))
        {
        }
    }
    MAILBOX_SetValueBits(MAILBOX, cpu_id, remoteData);
    return kStatus_MCMGR_Success;
}

/*!
 * @brief ISR handler
 *
 * This function is called when data from Mailbox is received
 */
void MAILBOX_IRQHandler(void)
{
#if defined(FSL_FEATURE_MAILBOX_SIDE_A)
    mailbox_cpu_id_t cpu_id = kMAILBOX_CM33_Core0;
#else
    mailbox_cpu_id_t cpu_id = kMAILBOX_CM33_Core1;
#endif
    uint32_t data;
    uint16_t eventType;
    uint16_t eventData;

    data = MAILBOX_GetValue(MAILBOX, cpu_id);
    /* To be MISRA compliant, return value needs to be checked even it could not never be 0 */
    if (0U != data)
    {
        MAILBOX_ClearValueBits(MAILBOX, cpu_id, data);

        eventType = (uint16_t)(data >> 16u);
        eventData = (uint16_t)(data & 0x0000FFFFu);

        if (((mcmgr_event_type_t)eventType >= kMCMGR_RemoteCoreUpEvent) &&
            ((mcmgr_event_type_t)eventType < kMCMGR_EventTableLength))
        {
            if (MCMGR_eventTable[(mcmgr_event_type_t)eventType].callback != ((void *)0))
            {
                MCMGR_eventTable[(mcmgr_event_type_t)eventType].callback(
                    eventData, MCMGR_eventTable[(mcmgr_event_type_t)eventType].callbackData);
            }
        }
    }
}

#if defined(MCMGR_HANDLE_EXCEPTIONS) && (MCMGR_HANDLE_EXCEPTIONS == 1)
/* This overrides the weak DefaultISR implementation from startup file */
void DefaultISR(void)
{
    uint32_t exceptionNumber = __get_IPSR();
    (void)MCMGR_TriggerEvent(kMCMGR_RemoteExceptionEvent, (uint16_t)exceptionNumber);
    for (;;)
    {
    } /* stop here */
}

void HardFault_Handler(void)
{
    DefaultISR();
}

void NMI_Handler(void)
{
    DefaultISR();
}

void MemManage_Handler(void)
{
    DefaultISR();
}

void BusFault_Handler(void)
{
    DefaultISR();
}

void UsageFault_Handler(void)
{
    DefaultISR();
}

#endif /* MCMGR_HANDLE_EXCEPTIONS */
