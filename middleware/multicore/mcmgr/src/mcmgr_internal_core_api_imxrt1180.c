/*
 * Copyright 2022-2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mcmgr.h"
#include "mcmgr_internal_core_api.h"
#include "fsl_device_registers.h"
#include "fsl_mu.h"

/* Count of cores in the system */
#define MCMGR_CORECOUNT 2

/* Count of memory regions in the system */
#define MCMGR_MEMREGCOUNT 2

/* MCMGR MU channel index - used for passing startupData */
#define MCMGR_MU_CHANNEL 3

/* MU TR/RR $MCMGR_MU_CHANNEL is managed by MCMGR */
#define MU_RX_ISR_Handler(x)     MU_RX_ISR(x)
#define MU_RX_ISR(number)        MU_Rx##number##FullFlagISR
#define mcmgr_mu_channel_handler MU_RX_ISR_Handler(MCMGR_MU_CHANNEL)
#define MU_RX_ISR_FLAG_Mask(x)   MU_RX_ISR_FLAG(x)
#define MU_RX_ISR_FLAG(number)   kMU_Rx##number##FullInterruptEnable
#define mcmgr_mu_channel_flag    MU_RX_ISR_FLAG_Mask(MCMGR_MU_CHANNEL)

volatile mcmgr_core_context_t s_mcmgrCoresContext[MCMGR_CORECOUNT] = {
    {.state = kMCMGR_ResetCoreState, .startupData = 0}, {.state = kMCMGR_ResetCoreState, .startupData = 0}};

/* Initialize structure with informations of all cores */
static const mcmgr_core_info_t s_mcmgrCores[MCMGR_CORECOUNT] = {
    {.coreType = kMCMGR_CoreTypeCortexM33, .coreName = "Main"},
    {.coreType = kMCMGR_CoreTypeCortexM7, .coreName = "Secondary"}};

const mcmgr_system_info_t g_mcmgrSystem = {
    .coreCount = MCMGR_CORECOUNT, .memRegCount = MCMGR_MEMREGCOUNT, .cores = s_mcmgrCores};

mcmgr_status_t mcmgr_early_init_internal(mcmgr_core_t coreNum)
{
    /* This function is intended to be called as close to the reset entry as possible,
       (within the startup sequence in SystemInitHook) to allow CoreUp event triggering.
       Avoid using uninitialized data here. */

    uint32_t flags;
    __attribute__((unused)) uint32_t data;
/* MUA clk enable */
#if defined(FSL_FEATURE_MU_SIDE_A)
    MU_Init(MU1_MUA);
    /* Clear all RX registers and status flags */
    for (uint32_t idx = 0U; idx < MU_RR_COUNT; idx++)
    {
        data = MU_ReceiveMsgNonBlocking(MU1_MUA, idx);
    }
    flags = MU_GetStatusFlags(MU1_MUA);
    MU_ClearStatusFlags(MU1_MUA, flags);
    /* Do not perform MU reset to avoid issues when debugging both CM33 and CM7 */
#endif
/* MUB clk enable */
#if defined(FSL_FEATURE_MU_SIDE_B)
    MU_Init(MU1_MUB);
    for (uint32_t idx = 0U; idx < MU_RR_COUNT; idx++)
    {
        data = MU_ReceiveMsgNonBlocking(MU1_MUB, idx);
    }
    flags = MU_GetStatusFlags(MU1_MUB);
    MU_ClearStatusFlags(MU1_MUB, flags);
#endif

    /* Trigger core up event here, core is starting! */
    return MCMGR_TriggerEvent(kMCMGR_RemoteCoreUpEvent, 0);
}

mcmgr_status_t mcmgr_late_init_internal(mcmgr_core_t coreNum)
{
#if defined(FSL_FEATURE_MU_SIDE_A)
    MU_EnableInterrupts(MU1_MUA, (uint32_t)mcmgr_mu_channel_flag);

#if (defined(FSL_FEATURE_MU_HAS_RESET_ASSERT_INT) && FSL_FEATURE_MU_HAS_RESET_ASSERT_INT)
    MU_EnableInterrupts(MU1_MUA, (uint32_t)kMU_ResetAssertInterruptEnable);
#endif

    NVIC_SetPriority(MU1_IRQn, 2);

    NVIC_EnableIRQ(MU1_IRQn);

#elif defined(FSL_FEATURE_MU_SIDE_B)
    MU_EnableInterrupts(MU1_MUB, (uint32_t)mcmgr_mu_channel_flag);

#if (defined(FSL_FEATURE_MU_HAS_RESET_ASSERT_INT) && FSL_FEATURE_MU_HAS_RESET_ASSERT_INT)
    MU_EnableInterrupts(MUB, (uint32_t)kMU_ResetAssertInterruptEnable);
#endif

    NVIC_SetPriority(MU1_IRQn, 2);

    NVIC_EnableIRQ(MU1_IRQn);

#endif

    return kStatus_MCMGR_Success;
}

mcmgr_status_t mcmgr_start_core_internal(mcmgr_core_t coreNum, void *bootAddress)
{
    __attribute__((unused)) volatile uint32_t result1, result2;

    if (coreNum != kMCMGR_Core1)
    {
        return kStatus_MCMGR_Error;
    }

    /*
     * RT1180 Specific CM7 Kick Off operation
     */
    /* Trigger S401 */
    while ((MU_RT_S3MUA->TSR & MU_TSR_TE0_MASK) == 0)
        ; /*Wait TR empty*/
    MU_RT_S3MUA->TR[0] = 0x17d20106;
    while ((MU_RT_S3MUA->RSR & MU_RSR_RF0_MASK) == 0)
        ; /*Wait RR Full*/
    while ((MU_RT_S3MUA->RSR & MU_RSR_RF1_MASK) == 0)
        ; /*Wait RR Full*/

    /* Response from ELE must be always read */
    result1 = MU_RT_S3MUA->RR[0];
    result2 = MU_RT_S3MUA->RR[1];

    // PRINTF("Get Status %x %x\r\n", result1, result2); /*Should be 0xE1D20206, 0xD6*/

    /* Deassert Wait */
    BLK_CTRL_S_AONMIX->M7_CFG =
        (BLK_CTRL_S_AONMIX->M7_CFG & (~BLK_CTRL_S_AONMIX_M7_CFG_WAIT_MASK)) | BLK_CTRL_S_AONMIX_M7_CFG_WAIT(0);

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
    /* It is not allowed to stop the secondary core */
    return kStatus_MCMGR_NotImplemented;
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
#if defined(FSL_FEATURE_MU_SIDE_A)
    return kMCMGR_Core0;
#elif defined(FSL_FEATURE_MU_SIDE_B)
    return kMCMGR_Core1;
#endif
}

mcmgr_status_t mcmgr_trigger_event_internal(uint32_t remoteData, bool forcedWrite)
{
    /* When forcedWrite is false, execute the blocking call, i.e. wait until previously
       sent data is processed. Otherwise, run the non-blocking version of the MU send function. */
    if (false == forcedWrite)
    {
        /* This is a blocking call */
#if defined(FSL_FEATURE_MU_SIDE_A)
        MU_SendMsg(MU1_MUA, MCMGR_MU_CHANNEL, remoteData);
#elif defined(FSL_FEATURE_MU_SIDE_B)
        MU_SendMsg(MU1_MUB, MCMGR_MU_CHANNEL, remoteData);
#endif
    }
    else
    {
        /* This is a non-blocking call */
#if defined(FSL_FEATURE_MU_SIDE_A)
        MU_SendMsgNonBlocking(MU1_MUA, MCMGR_MU_CHANNEL, remoteData);
#elif defined(FSL_FEATURE_MU_SIDE_B)
        MU_SendMsgNonBlocking(MU1_MUB, MCMGR_MU_CHANNEL, remoteData);
#endif
    }
    return kStatus_MCMGR_Success;
}

/*!
 * @brief ISR handler
 *
 * This function is called when data from MU is received
 */
void mcmgr_mu_channel_handler(void)
{
    uint32_t data;
    uint16_t eventType;
    uint16_t eventData;

    /* Non-blocking version of the receive function needs to be called here to avoid
       deadlock in ISR. The RX register must contain the payload now because the RX flag/event
       has been identified before reaching this point (mcmgr_mu_channel_handler function). */
#if defined(FSL_FEATURE_MU_SIDE_A)
    data = MU_ReceiveMsgNonBlocking(MU1_MUA, MCMGR_MU_CHANNEL);
#elif defined(FSL_FEATURE_MU_SIDE_B)
    data = MU_ReceiveMsgNonBlocking(MU1_MUB, MCMGR_MU_CHANNEL);
#endif

    /* To be MISRA compliant, return value needs to be checked even it could not never be 0 */
    if (0U != data)
    {
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
