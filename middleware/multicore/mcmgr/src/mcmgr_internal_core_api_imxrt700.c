/*
 * Copyright 2023-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mcmgr.h"
#include "mcmgr_internal_core_api.h"
#include "fsl_device_registers.h"
#include "fsl_mu.h"
#include "board.h"

/* At start make decision for what core mcmgr is build for at the compile time */
#if (defined(MIMXRT735S_cm33_core0_SERIES) || defined(MIMXRT758S_cm33_core0_SERIES) || \
     defined(MIMXRT798S_cm33_core0_SERIES))
#define MCMGR_BUILD_FOR_CORE_0
#elif (defined(MIMXRT735S_cm33_core1_SERIES) || defined(MIMXRT758S_cm33_core1_SERIES) || \
       defined(MIMXRT798S_cm33_core1_SERIES))
#define MCMGR_BUILD_FOR_CORE_1
#elif (defined(MIMXRT735S_hifi4_SERIES) || defined(MIMXRT758S_hifi4_SERIES) || defined(MIMXRT798S_hifi4_SERIES))
#define MCMGR_BUILD_FOR_CORE_2
#elif (defined(MIMXRT735S_hifi1_SERIES) || defined(MIMXRT758S_hifi1_SERIES) || defined(MIMXRT798S_hifi1_SERIES))
#define MCMGR_BUILD_FOR_CORE_3
#else
#error "Building for not supported platform!"
#endif

#if (defined(MCMGR_BUILD_FOR_CORE_0) || defined(MCMGR_BUILD_FOR_CORE_1))
#include "dsp_support.h"
#endif

#if (defined(MCMGR_BUILD_FOR_CORE_2) || defined(MCMGR_BUILD_FOR_CORE_3))
#include <xtensa/xos.h>
#include "fsl_inputmux.h"

/* For DSP we need to register custom IRQ Handlers
 * These handlers are defined in `mcmgr_mu_internal.c`.
 */
extern void MU4_B_IRQHandler(void *arg);
extern void MU2_A_IRQHandler(void *arg);
extern void MU3_B_IRQHandler(void *arg);
extern void MU0_B_IRQHandler(void *arg);
#endif

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
    {.coreType = kMCMGR_CoreTypeDSPHifi4, .coreName = "DSP Hifi4"},
    {.coreType = kMCMGR_CoreTypeDSPHifi1, .coreName = "DSP Hifi1"}};

const mcmgr_system_info_t g_mcmgrSystem = {
    .coreCount = MCMGR_CORECOUNT, .memRegCount = MCMGR_MEMREGCOUNT, .cores = s_mcmgrCores};

static void init_mu(MU_Type *base)
{
    MU_Init(base);

    /* Clear all RX registers and status flags */
    for (uint32_t idx = 0U; idx < MU_RR_COUNT; idx++)
    {
        (void)MU_ReceiveMsgNonBlocking(base, idx);
    }
    uint32_t flags = MU_GetStatusFlags(base);
    MU_ClearStatusFlags(base, flags);
}

mcmgr_status_t mcmgr_early_init_internal(mcmgr_core_t coreNum)
{
    /* This function is intended to be called as close to the reset entry as possible,
       (within the startup sequence in SystemInitHook) to allow CoreUp event triggering.
       Avoid using uninitialized data here. */

    mcmgr_status_t ret = kStatus_MCMGR_Success;

#if (defined(MCMGR_BUILD_FOR_CORE_0))
    if (coreNum == kMCMGR_Core0)
    {
        /* CPU0 to CPU1 communication case */
        init_mu(MU1_MUA);

        /* Trigger core up event here, core is starting! */
        ret = MCMGR_TriggerEvent(kMCMGR_Core1, kMCMGR_RemoteCoreUpEvent, 0);

        /* CPU0 to HiFi4 communication case */
        init_mu(MU4_MUA);

        /* Trigger core up event here, core is starting! */
        ret = MCMGR_TriggerEvent(kMCMGR_Core2, kMCMGR_RemoteCoreUpEvent, 0);

        /* CPU0 to HiFi1 communication case */
        init_mu(MU0_MUA);

        /* Trigger core up event here, core is starting! */
        ret = MCMGR_TriggerEvent(kMCMGR_Core3, kMCMGR_RemoteCoreUpEvent, 0);
    }
    else
    {
        ret = kStatus_MCMGR_Error;
    }
#elif (defined(MCMGR_BUILD_FOR_CORE_1))
    if (coreNum == kMCMGR_Core1)
    {
        /* CPU1 to HiFi1 communication case */
        init_mu(MU3_MUA);

        /* Trigger core up event here, core is starting! */
        ret = MCMGR_TriggerEvent(kMCMGR_Core3, kMCMGR_RemoteCoreUpEvent, 0);

        /* CPU1 to CPU0 communication case */
        init_mu(MU1_MUB);

        /* Trigger core up event here, core is starting! */
        ret = MCMGR_TriggerEvent(kMCMGR_Core0, kMCMGR_RemoteCoreUpEvent, 0);

        /* CPU1 to Hifi4 communication case */
        init_mu(MU2_MUB);

        /* Trigger core up event here, core is starting! */
        ret = MCMGR_TriggerEvent(kMCMGR_Core2, kMCMGR_RemoteCoreUpEvent, 0);
    }
    else
    {
        ret = kStatus_MCMGR_Error;
    }
#elif (defined(MCMGR_BUILD_FOR_CORE_2))
    if (coreNum == kMCMGR_Core2)
    {
        /* HiFi4 to CPU0 communication case */
        init_mu(MU4_MUB);

        /* Trigger core up event here, core is starting! */
        ret = MCMGR_TriggerEvent(kMCMGR_Core0, kMCMGR_RemoteCoreUpEvent, 0);

        /* HiFi4 to CPU1 communication case */
        init_mu(MU2_MUA);

        /* Trigger core up event here, core is starting! */
        ret = MCMGR_TriggerEvent(kMCMGR_Core0, kMCMGR_RemoteCoreUpEvent, 0);
    }
    else
    {
        ret = kStatus_MCMGR_Error;
    }
#elif (defined(MCMGR_BUILD_FOR_CORE_3))
    if (coreNum == kMCMGR_Core3)
    {
        /* HiFi1 to CPU0 communication case */
        init_mu(MU0_MUB);

        /* Trigger core up event here, core is starting! */
        ret = MCMGR_TriggerEvent(kMCMGR_Core0, kMCMGR_RemoteCoreUpEvent, 0);

        /* HiFi1 to CPU1 communication case */
        init_mu(MU3_MUB);

        /* Trigger core up event here, core is starting! */
        ret = MCMGR_TriggerEvent(kMCMGR_Core1, kMCMGR_RemoteCoreUpEvent, 0);
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
#if (defined(MCMGR_BUILD_FOR_CORE_0))
    if (coreNum == kMCMGR_Core0)
    {
        /* CPU0 to CPU1 communication case */
        MU_EnableInterrupts(MU1_MUA, (uint32_t)mcmgr_mu_channel_flag);

#if (defined(FSL_FEATURE_MU_HAS_RESET_ASSERT_INT) && FSL_FEATURE_MU_HAS_RESET_ASSERT_INT)
        MU_EnableInterrupts(MU1_MUA, (uint32_t)kMU_ResetAssertInterruptEnable);
#endif

        NVIC_SetPriority(MU1_A_IRQn, 2);

        NVIC_EnableIRQ(MU1_A_IRQn);

        /* CPU0 to HiFi4 communication case */
        MU_EnableInterrupts(MU4_MUA, (uint32_t)mcmgr_mu_channel_flag);

#if (defined(FSL_FEATURE_MU_HAS_RESET_ASSERT_INT) && FSL_FEATURE_MU_HAS_RESET_ASSERT_INT)
        MU_EnableInterrupts(MU4_MUA, (uint32_t)kMU_ResetAssertInterruptEnable);
#endif

        NVIC_SetPriority(MU4_A_IRQn, 2);

        NVIC_EnableIRQ(MU4_A_IRQn);

        /* CPU0 to HiFi1 communication case */
        MU_EnableInterrupts(MU0_MUA, (uint32_t)mcmgr_mu_channel_flag);

#if (defined(FSL_FEATURE_MU_HAS_RESET_ASSERT_INT) && FSL_FEATURE_MU_HAS_RESET_ASSERT_INT)
        MU_EnableInterrupts(MU0_MUA, (uint32_t)kMU_ResetAssertInterruptEnable);
#endif

        NVIC_SetPriority(MU0_A_IRQn, 2);

        NVIC_EnableIRQ(MU0_A_IRQn);
    }
    else
    {
        return kStatus_MCMGR_Error;
    }
#elif (defined(MCMGR_BUILD_FOR_CORE_1))
    if (coreNum == kMCMGR_Core1)
    {
        /* CPU1 to HiFi1 communication case */
        MU_EnableInterrupts(MU3_MUA, (uint32_t)mcmgr_mu_channel_flag);

#if (defined(FSL_FEATURE_MU_HAS_RESET_ASSERT_INT) && FSL_FEATURE_MU_HAS_RESET_ASSERT_INT)
        MU_EnableInterrupts(MU3_MUA, (uint32_t)kMU_ResetAssertInterruptEnable);
#endif

        NVIC_SetPriority(MU3_A_IRQn, 2);

        NVIC_EnableIRQ(MU3_A_IRQn);

        /* CPU1 to CPU0 communication case */
        MU_EnableInterrupts(MU1_MUB, (uint32_t)mcmgr_mu_channel_flag);

#if (defined(FSL_FEATURE_MU_HAS_RESET_ASSERT_INT) && FSL_FEATURE_MU_HAS_RESET_ASSERT_INT)
        MU_EnableInterrupts(MU1_MUB, (uint32_t)kMU_ResetAssertInterruptEnable);
#endif

        NVIC_SetPriority(MU1_B_IRQn, 2);

        NVIC_EnableIRQ(MU1_B_IRQn);

        /* CPU1 to Hifi4 communication case */
        MU_EnableInterrupts(MU2_MUB, (uint32_t)mcmgr_mu_channel_flag);

#if (defined(FSL_FEATURE_MU_HAS_RESET_ASSERT_INT) && FSL_FEATURE_MU_HAS_RESET_ASSERT_INT)
        MU_EnableInterrupts(MU2_MUB, (uint32_t)kMU_ResetAssertInterruptEnable);
#endif

        NVIC_SetPriority(MU2_B_IRQn, 2);

        NVIC_EnableIRQ(MU2_B_IRQn);
    }
    else
    {
        return kStatus_MCMGR_Error;
    }
#elif (defined(MCMGR_BUILD_FOR_CORE_2))
    if (coreNum == kMCMGR_Core2)
    {
        /* HiFi4 to CPU0 communication case */
        INPUTMUX_Init(INPUTMUX0);
        INPUTMUX_AttachSignal(INPUTMUX0, 1U, kINPUTMUX_Mu4BToDspInterrupt);

        /* DSP interrupt only can be enable after XOS is started. */
        xos_register_interrupt_handler(DSP_INT0_SEL1_IRQn, MU4_B_IRQHandler, ((void *)0));
        xos_interrupt_enable(DSP_INT0_SEL1_IRQn);

        MU_EnableInterrupts(MU4_MUB, (uint32_t)mcmgr_mu_channel_flag);

#if (defined(FSL_FEATURE_MU_HAS_RESET_ASSERT_INT) && FSL_FEATURE_MU_HAS_RESET_ASSERT_INT)
        MU_EnableInterrupts(MU4_MUB, (uint32_t)kMU_ResetAssertInterruptEnable);
#endif
        /* HiFi4 to CPU1 communication case */
        INPUTMUX_Init(INPUTMUX0);
        INPUTMUX_AttachSignal(INPUTMUX0, 2U, kINPUTMUX_Mu2AToDspInterrupt);

        /* DSP interrupt only can be enable after XOS is started. */
        xos_register_interrupt_handler(DSP_INT0_SEL2_IRQn, MU2_A_IRQHandler, ((void *)0));
        xos_interrupt_enable(DSP_INT0_SEL2_IRQn);

        MU_EnableInterrupts(MU2_MUA, (uint32_t)mcmgr_mu_channel_flag);

#if (defined(FSL_FEATURE_MU_HAS_RESET_ASSERT_INT) && FSL_FEATURE_MU_HAS_RESET_ASSERT_INT)
        MU_EnableInterrupts(MU2_MUA, (uint32_t)kMU_ResetAssertInterruptEnable);
#endif
    }
    else
    {
        return kStatus_MCMGR_Error;
    }
#elif (defined(MCMGR_BUILD_FOR_CORE_3))
    if (coreNum == kMCMGR_Core3)
    {
        /* HiFi1 to CPU0 communication case */
        INPUTMUX_Init(INPUTMUX1);
        INPUTMUX_AttachSignal(INPUTMUX1, 1U, kINPUTMUX_Mu0BToDspInterrupt);

        /* DSP interrupt only can be enable after XOS is started. */
        xos_register_interrupt_handler(DSP_INT0_SEL2_IRQn, MU0_B_IRQHandler, ((void *)0));
        xos_interrupt_enable(DSP_INT0_SEL2_IRQn);

        MU_EnableInterrupts(MU0_MUB, (uint32_t)mcmgr_mu_channel_flag);

#if (defined(FSL_FEATURE_MU_HAS_RESET_ASSERT_INT) && FSL_FEATURE_MU_HAS_RESET_ASSERT_INT)
        MU_EnableInterrupts(MU0_MUB, (uint32_t)kMU_ResetAssertInterruptEnable);
#endif

        /* HiFi1 to CPU1 communication case */
        INPUTMUX_Init(INPUTMUX1);
        INPUTMUX_AttachSignal(INPUTMUX1, 1U, kINPUTMUX_Mu3BToDspInterrupt);

        /* DSP interrupt only can be enable after XOS is started. */
        xos_register_interrupt_handler(DSP_INT0_SEL1_IRQn, MU3_B_IRQHandler, ((void *)0));
        xos_interrupt_enable(DSP_INT0_SEL1_IRQn);

        MU_EnableInterrupts(MU3_MUB, (uint32_t)mcmgr_mu_channel_flag);

#if (defined(FSL_FEATURE_MU_HAS_RESET_ASSERT_INT) && FSL_FEATURE_MU_HAS_RESET_ASSERT_INT)
        MU_EnableInterrupts(MU3_MUB, (uint32_t)kMU_ResetAssertInterruptEnable);
#endif
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
    if ((uint32_t)coreNum >= g_mcmgrSystem.coreCount)
    {
        return kStatus_MCMGR_Error;
    }
#if (defined(MCMGR_BUILD_FOR_CORE_0))
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
    else if (coreNum == kMCMGR_Core2)
    {
        /* CPU0 to HiFi4 communication case */
        BOARD_DSP_Init();
    }
    else
    {
        return kStatus_MCMGR_Error;
    }
#elif (defined(MCMGR_BUILD_FOR_CORE_1))
    if (coreNum == kMCMGR_Core3)
    {
        /* CPU1 to HiFi1 communication case */
        BOARD_DSP_Init();
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
    if ((uint32_t)coreNum >= g_mcmgrSystem.coreCount)
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
#if (defined(MCMGR_BUILD_FOR_CORE_0))
    return kMCMGR_Core0;
#elif (defined(MCMGR_BUILD_FOR_CORE_1))
    return kMCMGR_Core1;
#elif (defined(MCMGR_BUILD_FOR_CORE_2))
    return kMCMGR_Core2;
#elif (defined(MCMGR_BUILD_FOR_CORE_3))
    return kMCMGR_Core3;
#endif
}

mcmgr_status_t mcmgr_trigger_event_internal(mcmgr_core_t coreNum, uint32_t remoteData, bool forcedWrite)
{
    MU_Type *mu = NULL;

    mcmgr_core_t currentCore = MCMGR_GetCurrentCore();
    if (currentCore == coreNum)
    {
        return kStatus_MCMGR_Error;
    }

    switch (coreNum) {
    case kMCMGR_Core0:
#if (defined(MCMGR_BUILD_FOR_CORE_0))
        /* Not supported, core cannot trigger itself */
        /* Keeping this empty intentionally as reference */
        mu = NULL;
#elif (defined(MCMGR_BUILD_FOR_CORE_1))
        mu = MU1_MUB;
#elif (defined(MCMGR_BUILD_FOR_CORE_2))
        mu = MU4_MUB;
#elif (defined(MCMGR_BUILD_FOR_CORE_3))
        mu = MU0_MUB;
#endif
        break;

    case kMCMGR_Core1:
#if (defined(MCMGR_BUILD_FOR_CORE_0))
        mu = MU1_MUA;
#elif (defined(MCMGR_BUILD_FOR_CORE_1))
        /* Not supported, core cannot trigger itself */
        /* Keeping this empty intentionally as reference */
        mu = NULL;
#elif (defined(MCMGR_BUILD_FOR_CORE_2))
        mu = MU2_MUA;
#elif (defined(MCMGR_BUILD_FOR_CORE_3))
        mu = MU3_MUB;
#endif
        break;

    case kMCMGR_Core2:
#if (defined(MCMGR_BUILD_FOR_CORE_0))
        mu = MU4_MUA;
#elif (defined(MCMGR_BUILD_FOR_CORE_1))
        mu = MU2_MUB;
#elif (defined(MCMGR_BUILD_FOR_CORE_2))
        /* Not supported, core cannot trigger itself */
        /* Keeping this empty intentionally as reference */
        mu = NULL;
#elif (defined(MCMGR_BUILD_FOR_CORE_3))
        /* Not supported, core cannot trigger hifi4 */
        mu = NULL;
#endif
        break;

    case kMCMGR_Core3:
#if (defined(MCMGR_BUILD_FOR_CORE_0))
        mu = MU0_MUA;
#elif (defined(MCMGR_BUILD_FOR_CORE_1))
        mu = MU3_MUA;
#elif (defined(MCMGR_BUILD_FOR_CORE_2))
        /* Not supported, core cannot trigger hifi1 */
        mu = NULL;
#elif (defined(MCMGR_BUILD_FOR_CORE_3))
        /* Not supported, core cannot trigger itself */
        /* Keeping this empty intentionally as reference */
        mu = NULL;
#endif
        break;

    default:
        return kStatus_MCMGR_Error;
        break;
    }

    if (NULL == mu)
    {
        return kStatus_MCMGR_Error;
    }

    /* When forcedWrite is false, execute the blocking call, i.e. wait until previously
       sent data is processed. Otherwise, run the non-blocking version of the MU send function. */
    if (false == forcedWrite)
    {
        MU_SendMsg(mu, MCMGR_MU_CHANNEL, remoteData);
    }
    else
    {
        MU_SendMsgNonBlocking(mu, MCMGR_MU_CHANNEL, remoteData);
    }

    return kStatus_MCMGR_Success;
}

/*!
 * @brief ISR handler
 *
 * This function is called when data from MU is received
 */
void mcmgr_mu_channel_handler(MU_Type *base, mcmgr_core_t coreNum)
{
    uint32_t data;
    uint16_t eventType;
    uint16_t eventData;

    /* Non-blocking version of the receive function needs to be called here to avoid
       deadlock in ISR. The RX register must contain the payload now because the RX flag/event
       has been identified before reaching this point (mcmgr_mu_channel_handler function). */
    data = MU_ReceiveMsgNonBlocking(base, MCMGR_MU_CHANNEL);

    /* To be MISRA compliant, return value needs to be checked even it could not never be 0 */
    if (0U == data)
    {
        return;
    }

    eventType = (uint16_t)(data >> 16u);
    eventData = (uint16_t)(data & 0x0000FFFFu);

    /* Handle invalid event type */
    if (((mcmgr_event_type_t)eventType < kMCMGR_RemoteCoreUpEvent) &&
        ((mcmgr_event_type_t)eventType >= kMCMGR_EventTableLength))
    {
        return;
    }

    if (MCMGR_eventTable[(mcmgr_event_type_t)eventType].callback != ((void *)0))
    {
        MCMGR_eventTable[(mcmgr_event_type_t)eventType].callback(
            coreNum,
            eventData,
            MCMGR_eventTable[(mcmgr_event_type_t)eventType].callbackData);
    }
}

#if defined(MCMGR_HANDLE_EXCEPTIONS) && (MCMGR_HANDLE_EXCEPTIONS == 1)
/* This overrides the weak DefaultISR implementation from startup file */
void DefaultISR(void)
{
    mcmgr_core_t target_core;
    /* Select what core to trigger in case of exception */
#if (defined(MCMGR_BUILD_FOR_CORE_0))
    target_core = kMCMGR_Core1;
#elif (defined(MCMGR_BUILD_FOR_CORE_1))
    target_core = kMCMGR_Core0;
#elif (defined(MCMGR_BUILD_FOR_CORE_2))
    target_core = kMCMGR_Core0;
#elif (defined(MCMGR_BUILD_FOR_CORE_3))
    target_core = kMCMGR_Core0;
#endif

    uint32_t exceptionNumber = __get_IPSR();
    (void)MCMGR_TriggerEvent(target_core, kMCMGR_RemoteExceptionEvent, (uint16_t)exceptionNumber);
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
