/*
 * Copyright 2023-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mcmgr.h"
#include "mcmgr_internal_core_api.h"
#include "fsl_device_registers.h"
#include "fsl_mu.h"
#include "board.h"

/* Count of cores in the system */
#define MCMGR_CORECOUNT 4

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
    {.state = kMCMGR_ResetCoreState, .startupData = 0},
    {.state = kMCMGR_ResetCoreState, .startupData = 0},
    {.state = kMCMGR_ResetCoreState, .startupData = 0},
    {.state = kMCMGR_ResetCoreState, .startupData = 0}};

/* Initialize structure with informations of all cores */
static const mcmgr_core_info_t s_mcmgrCores[MCMGR_CORECOUNT] = {
    {.coreType = kMCMGR_CoreTypeCortexM33, .coreName = "CPU0"},
    {.coreType = kMCMGR_CoreTypeCortexM33, .coreName = "CPU1"},
    {.coreType = kMCMGR_CoreTypeDSPHifi1, .coreName = "DSP Hifi1"},
    {.coreType = kMCMGR_CoreTypeDSPHifi4, .coreName = "DSP Hifi4"}};

const mcmgr_system_info_t g_mcmgrSystem = {
    .coreCount = MCMGR_CORECOUNT, .memRegCount = MCMGR_MEMREGCOUNT, .cores = s_mcmgrCores};

mcmgr_status_t mcmgr_early_init_internal(mcmgr_core_t coreNum)
{
    /* This function is intended to be called as close to the reset entry as possible,
       (within the startup sequence in SystemInitHook) to allow CoreUp event triggering.
       Avoid using uninitialized data here. */

    uint32_t flags;
    mcmgr_status_t ret = kStatus_MCMGR_Success;
    __attribute__((unused)) uint32_t data;
#if (defined(MIMXRT735S_cm33_core0_SERIES) || defined(MIMXRT758S_cm33_core0_SERIES) || \
     defined(MIMXRT798S_cm33_core0_SERIES))
    if (coreNum == kMCMGR_Core0)
    {
        /* CPU0 to CPU1 communication case */
        MU_Init(MU1_MUA);
        /* Clear all RX registers and status flags */
        for (uint32_t idx = 0U; idx < MU_RR_COUNT; idx++)
        {
            data = MU_ReceiveMsgNonBlocking(MU1_MUA, idx);
        }
        flags = MU_GetStatusFlags(MU1_MUA);
        MU_ClearStatusFlags(MU1_MUA, flags);

        /* Trigger core up event here, core is starting! */
        ret = MCMGR_TriggerEvent(kMCMGR_RemoteCoreUpEvent, 0);
    }
    else if (coreNum == kMCMGR_Core2)
    {
        /* CPU0 to HiFi4 communication case */
        MU_Init(MU4_MUA);
        /* Clear all RX registers and status flags */
        for (uint32_t idx = 0U; idx < MU_RR_COUNT; idx++)
        {
            data = MU_ReceiveMsgNonBlocking(MU4_MUA, idx);
        }
        flags = MU_GetStatusFlags(MU4_MUA);
        MU_ClearStatusFlags(MU4_MUA, flags);

        // TODO route event to hifi4
        /* Trigger core up event here, core is starting! */
        ret = MCMGR_TriggerEvent(kMCMGR_RemoteCoreUpEvent, 0);
    }
    else
    {
        ret = kStatus_MCMGR_Error;
    }
#elif (defined(MIMXRT735S_cm33_core1_SERIES) || defined(MIMXRT758S_cm33_core1_SERIES) || \
       defined(MIMXRT798S_cm33_core1_SERIES))
    if (coreNum == kMCMGR_Core3)
    {
        /* CPU1 to HiFi1 communication case */
        MU_Init(MU3_MUA);
        /* Clear all RX registers and status flags */
        for (uint32_t idx = 0U; idx < MU_RR_COUNT; idx++)
        {
            data = MU_ReceiveMsgNonBlocking(MU3_MUA, idx);
        }
        flags = MU_GetStatusFlags(MU3_MUA);
        MU_ClearStatusFlags(MU3_MUA, flags);

        // TODO route event to hifi1
        /* Trigger core up event here, core is starting! */
        ret = MCMGR_TriggerEvent(kMCMGR_RemoteCoreUpEvent, 0);
    }
    else if (coreNum == kMCMGR_Core1)
    {
        /* CPU1 to CPU0 communication case */
        MU_Init(MU1_MUB);
        /* Clear all RX registers and status flags */
        for (uint32_t idx = 0U; idx < MU_RR_COUNT; idx++)
        {
            data = MU_ReceiveMsgNonBlocking(MU1_MUB, idx);
        }
        flags = MU_GetStatusFlags(MU1_MUB);
        MU_ClearStatusFlags(MU1_MUB, flags);

        /* Trigger core up event here, core is starting! */
        ret = MCMGR_TriggerEvent(kMCMGR_RemoteCoreUpEvent, 0);
    }
    else
    {
        ret = kStatus_MCMGR_Error;
    }
#endif
    return ret;
}

mcmgr_status_t mcmgr_late_init_internal(mcmgr_core_t coreNum)
{
#if (defined(MIMXRT735S_cm33_core0_SERIES) || defined(MIMXRT758S_cm33_core0_SERIES) || \
     defined(MIMXRT798S_cm33_core0_SERIES))
    if (coreNum == kMCMGR_Core0)
    {
        /* CPU0 to CPU1 communication case */
        MU_EnableInterrupts(MU1_MUA, (uint32_t)mcmgr_mu_channel_flag);

#if (defined(FSL_FEATURE_MU_HAS_RESET_ASSERT_INT) && FSL_FEATURE_MU_HAS_RESET_ASSERT_INT)
        MU_EnableInterrupts(MU1_MUA, (uint32_t)kMU_ResetAssertInterruptEnable);
#endif

        NVIC_SetPriority(MU1_A_IRQn, 2);

        NVIC_EnableIRQ(MU1_A_IRQn);
    }
    else if (coreNum == kMCMGR_Core2)
    {
        /* CPU0 to HiFi4 communication case */
        MU_EnableInterrupts(MU4_MUA, (uint32_t)mcmgr_mu_channel_flag);

#if (defined(FSL_FEATURE_MU_HAS_RESET_ASSERT_INT) && FSL_FEATURE_MU_HAS_RESET_ASSERT_INT)
        MU_EnableInterrupts(MU4_MUA, (uint32_t)kMU_ResetAssertInterruptEnable);
#endif

        NVIC_SetPriority(MU4_A_IRQn, 2);

        NVIC_EnableIRQ(MU4_A_IRQn);
    }
    else
    {
        return kStatus_MCMGR_Error;
    }
#elif (defined(MIMXRT735S_cm33_core1_SERIES) || defined(MIMXRT758S_cm33_core1_SERIES) || \
       defined(MIMXRT798S_cm33_core1_SERIES))
    if (coreNum == kMCMGR_Core3)
    {
        /* CPU1 to HiFi1 communication case */
        MU_EnableInterrupts(MU3_MUA, (uint32_t)mcmgr_mu_channel_flag);

#if (defined(FSL_FEATURE_MU_HAS_RESET_ASSERT_INT) && FSL_FEATURE_MU_HAS_RESET_ASSERT_INT)
        MU_EnableInterrupts(MU3_MUA, (uint32_t)kMU_ResetAssertInterruptEnable);
#endif

        NVIC_SetPriority(MU3_A_IRQn, 2);

        NVIC_EnableIRQ(MU3_A_IRQn);
    }
    else if (coreNum == kMCMGR_Core1)
    {
        /* CPU1 to CPU0 communication case */
        MU_EnableInterrupts(MU1_MUB, (uint32_t)mcmgr_mu_channel_flag);

#if (defined(FSL_FEATURE_MU_HAS_RESET_ASSERT_INT) && FSL_FEATURE_MU_HAS_RESET_ASSERT_INT)
        MU_EnableInterrupts(MU1_MUB, (uint32_t)kMU_ResetAssertInterruptEnable);
#endif

        NVIC_SetPriority(MU1_B_IRQn, 2);

        NVIC_EnableIRQ(MU1_B_IRQn);
    }
    else
    {
        return kStatus_MCMGR_Error;
    }
#endif
    return kStatus_MCMGR_Success;
}

mcmgr_status_t mcmgr_start_core_internal(mcmgr_core_t coreNum, void *bootAddress)
{
    if (coreNum > kMCMGR_Core3)
    {
        return kStatus_MCMGR_Error;
    }
#if (defined(MIMXRT735S_cm33_core0_SERIES) || defined(MIMXRT758S_cm33_core0_SERIES) || \
     defined(MIMXRT798S_cm33_core0_SERIES))
    if (coreNum == kMCMGR_Core1)
    {
        /* CPU0 to CPU1 communication case */
        /*Glikey write enable, GLIKEY4*/
        GlikeyWriteEnable(GLIKEY4, 1U);

        /* Boot source for Core 1 from RAM. */
        SYSCON3->CPU1_NSVTOR = ((uint32_t)(char *)bootAddress >> 7U);
        SYSCON3->CPU1_SVTOR  = ((uint32_t)(char *)bootAddress >> 7U);

        GlikeyClearConfig(GLIKEY4);

        /* Enable cpu1 clock. */
        CLOCK_EnableClock(kCLOCK_Cpu1);

        /* Clear reset*/
        RESET_ClearPeripheralReset(kCPU1_RST_SHIFT_RSTn);

        /* Release cpu wait*/
        SYSCON3->CPU_STATUS &= ~SYSCON3_CPU_STATUS_CPU_WAIT_MASK;
    }
    else if (coreNum == kMCMGR_Core3)
    {
        /* CPU0 to HiFi4 communication case */
        SYSCON0->DSPSTALL = 0x0;
    }
    else
    {
        return kStatus_MCMGR_Error;
    }
#elif (defined(MIMXRT735S_cm33_core1_SERIES) || defined(MIMXRT758S_cm33_core1_SERIES) || \
       defined(MIMXRT798S_cm33_core1_SERIES))
    if (coreNum == kMCMGR_Core2)
    {
        /* CPU1 to HiFi1 communication case */
    }
    else if (coreNum == kMCMGR_Core0)
    {
        /* CPU1 to CPU0 communication case */
        /* Does it make sense? */
    }
    else
    {
        return kStatus_MCMGR_Error;
    }
#endif

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
#if (defined(MIMXRT735S_cm33_core0_SERIES) || defined(MIMXRT758S_cm33_core0_SERIES) || \
     defined(MIMXRT798S_cm33_core0_SERIES))
    return kMCMGR_Core0;
#elif (defined(MIMXRT735S_cm33_core1_SERIES) || defined(MIMXRT758S_cm33_core1_SERIES) || \
       defined(MIMXRT798S_cm33_core1_SERIES))
    return kMCMGR_Core1;
#elif (defined(MIMXRT735S_hifi4_SERIES) || defined(MIMXRT758S_hifi4_SERIES) || defined(MIMXRT798S_hifi4_SERIES))
    return kMCMGR_Core2;
#elif (defined(MIMXRT735S_hifi1_SERIES) || defined(MIMXRT758S_hifi1_SERIES) || defined(MIMXRT798S_hifi1_SERIES))
    return kMCMGR_Core3;
#endif
}

mcmgr_status_t mcmgr_trigger_event_internal(uint32_t remoteData, bool forcedWrite)
{
    // TODO add coreNum into fc param and thus allow triggering events to individual cores

    /* When forcedWrite is false, execute the blocking call, i.e. wait until previously
       sent data is processed. Otherwise, run the non-blocking version of the MU send function. */
    if (false == forcedWrite)
    {
        /* This is a blocking call */
#if (defined(MIMXRT735S_cm33_core0_SERIES) || defined(MIMXRT758S_cm33_core0_SERIES) || \
     defined(MIMXRT798S_cm33_core0_SERIES))
        /* CPU0 to CPU1 communication case */
#if defined(FSL_FEATURE_MU_SIDE_A)
        MU_SendMsg(MU1_MUA, MCMGR_MU_CHANNEL, remoteData);
        /* CPU0 to HiFi4 communication case */
#elif defined(FSL_FEATURE_MU_SIDE_B)
        MU_SendMsg(MU4_MUA, MCMGR_MU_CHANNEL, remoteData);
#endif
#elif (defined(MIMXRT735S_cm33_core1_SERIES) || defined(MIMXRT758S_cm33_core1_SERIES) || \
       defined(MIMXRT798S_cm33_core1_SERIES))
        /* CPU1 to HiFi1 communication case */
#if !defined(__CORTEX_M)
        MU_SendMsg(MU3_MUA, MCMGR_MU_CHANNEL, remoteData);
        /* CPU1 to CPU0 communication case */
// #elif defined(__CORTEX_M)
#else
        MU_SendMsg(MU1_MUB, MCMGR_MU_CHANNEL, remoteData);
#endif
#endif
    }
    else
    {
        /* This is a non-blocking call */
#if (defined(MIMXRT735S_cm33_core0_SERIES) || defined(MIMXRT758S_cm33_core0_SERIES) || \
     defined(MIMXRT798S_cm33_core0_SERIES))
        /* CPU0 to CPU1 communication case */
#if defined(FSL_FEATURE_MU_SIDE_A)
        /* CPU0 to HiFi4 communication case */
        MU_SendMsgNonBlocking(MU1_MUA, MCMGR_MU_CHANNEL, remoteData);
#elif defined(FSL_FEATURE_MU_SIDE_B)
        MU_SendMsgNonBlocking(MU4_MUA, MCMGR_MU_CHANNEL, remoteData);
#endif
#elif (defined(MIMXRT735S_cm33_core1_SERIES) || defined(MIMXRT758S_cm33_core1_SERIES) || \
       defined(MIMXRT798S_cm33_core1_SERIES))
        /* CPU1 to HiFi1 communication case */
#if !defined(__CORTEX_M)
        MU_SendMsgNonBlocking(MU3_MUA, MCMGR_MU_CHANNEL, remoteData);
        /* CPU1 to CPU0 communication case */
// #elif defined(FSL_FEATURE_MU_SIDE_B)
#else
        MU_SendMsgNonBlocking(MU1_MUB, MCMGR_MU_CHANNEL, remoteData);
#endif
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
#if (defined(MIMXRT735S_cm33_core0_SERIES) || defined(MIMXRT758S_cm33_core0_SERIES) || \
     defined(MIMXRT798S_cm33_core0_SERIES))
    /* CPU0 to CPU1 communication case */
#if defined(FSL_FEATURE_MU_SIDE_A)
    data = MU_ReceiveMsgNonBlocking(MU1_MUA, MCMGR_MU_CHANNEL);
    /* CPU0 to HiFi4 communication case */
#elif defined(FSL_FEATURE_MU_SIDE_B)
    data = MU_ReceiveMsgNonBlocking(MU4_MUA, MCMGR_MU_CHANNEL);
#endif
#elif (defined(MIMXRT735S_cm33_core1_SERIES) || defined(MIMXRT758S_cm33_core1_SERIES) || \
       defined(MIMXRT798S_cm33_core1_SERIES))
    /* CPU1 to HiFi1 communication case */
// #if defined(FSL_FEATURE_MU_SIDE_A)
#if !defined(__CORTEX_M)
    data = MU_ReceiveMsgNonBlocking(MU3_MUA, MCMGR_MU_CHANNEL);
    /* CPU1 to CPU0 communication case */
// #elif defined(FSL_FEATURE_MU_SIDE_B)
#else
    data = MU_ReceiveMsgNonBlocking(MU1_MUB, MCMGR_MU_CHANNEL);
#endif
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
