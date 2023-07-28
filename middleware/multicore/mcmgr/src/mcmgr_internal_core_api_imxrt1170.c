/*
 * Copyright 2019-2023 NXP
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

/* Boot Core Selection Fuse bit */
#define BT_CORE_SEL_BIT 0x1000u

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
    {.coreType = kMCMGR_CoreTypeCortexM7, .coreName = "Main"},
    {.coreType = kMCMGR_CoreTypeCortexM4, .coreName = "Secondary"}};

const mcmgr_system_info_t g_mcmgrSystem = {
    .coreCount = MCMGR_CORECOUNT, .memRegCount = MCMGR_MEMREGCOUNT, .cores = s_mcmgrCores};

mcmgr_status_t mcmgr_early_init_internal(mcmgr_core_t coreNum)
{
    uint32_t flags;
    __attribute__((unused)) uint32_t data;
/* MUA clk enable */
#if defined(FSL_FEATURE_MU_SIDE_A)
    MU_Init(MUA);
    /* Clear all RX registers and status flags */
    for (uint32_t idx = 0U; idx < MU_RR_COUNT; idx++)
    {
        data = MU_ReceiveMsgNonBlocking(MUA, idx);
    }
    flags = MU_GetStatusFlags(MUA);
    MU_ClearStatusFlags(MUA, flags);
    /* Do not perform MU reset to avoid issues when debugging both CM4 and CM7 */
#endif
/* MUB clk enable */
#if defined(FSL_FEATURE_MU_SIDE_B)
    MU_Init(MUB);
    for (uint32_t idx = 0U; idx < MU_RR_COUNT; idx++)
    {
        data = MU_ReceiveMsgNonBlocking(MUB, idx);
    }
    flags = MU_GetStatusFlags(MUB);
    MU_ClearStatusFlags(MUB, flags);
#endif

    /* Trigger core up event here, core is starting! */
    return MCMGR_TriggerEvent(kMCMGR_RemoteCoreUpEvent, 0);
}

mcmgr_status_t mcmgr_late_init_internal(mcmgr_core_t coreNum)
{
#if defined(FSL_FEATURE_MU_SIDE_A)
    MU_EnableInterrupts(MUA, (uint32_t)mcmgr_mu_channel_flag);

#if (defined(FSL_FEATURE_MU_HAS_RESET_ASSERT_INT) && FSL_FEATURE_MU_HAS_RESET_ASSERT_INT)
    MU_EnableInterrupts(MUA, (uint32_t)kMU_ResetAssertInterruptEnable);
#endif

    NVIC_SetPriority(MUA_IRQn, 2);

    NVIC_EnableIRQ(MUA_IRQn);

#elif defined(FSL_FEATURE_MU_SIDE_B)
    MU_EnableInterrupts(MUB, (uint32_t)mcmgr_mu_channel_flag);

#if (defined(FSL_FEATURE_MU_HAS_RESET_ASSERT_INT) && FSL_FEATURE_MU_HAS_RESET_ASSERT_INT)
    MU_EnableInterrupts(MUB, (uint32_t)kMU_ResetAssertInterruptEnable);
#endif

    NVIC_SetPriority(MUB_IRQn, 2);

    NVIC_EnableIRQ(MUB_IRQn);

#endif

    return kStatus_MCMGR_Success;
}

mcmgr_status_t mcmgr_start_core_internal(mcmgr_core_t coreNum, void *bootAddress)
{
    if (coreNum != kMCMGR_Core1)
    {
        return kStatus_MCMGR_Error;
    }
/* When kicking off the CM4 core from the CM7 core */
#if defined(FSL_FEATURE_MU_SIDE_A)
    IOMUXC_LPSR_GPR->GPR0 = IOMUXC_LPSR_GPR_GPR0_CM4_INIT_VTOR_LOW(((uint32_t)(char *)bootAddress) >> 3u);
    IOMUXC_LPSR_GPR->GPR1 = IOMUXC_LPSR_GPR_GPR1_CM4_INIT_VTOR_HIGH(((uint32_t)(char *)bootAddress) >> 16u);

    SRC->CTRL_M4CORE = SRC_CTRL_M4CORE_SW_RESET_MASK;
    SRC->SCR |= SRC_SCR_BT_RELEASE_M4_MASK;
#endif
/* When kicking off the CM7 core from the CM4 core */
#if defined(FSL_FEATURE_MU_SIDE_B)
    if (ANADIG_MISC->MISC_DIFPROG == 0x1170A0U)
    {
        /* Set hidden register at 0x400e404c address */
        *(uint32_t *)&IOMUXC_GPR->RESERVED_0[0] = (uint32_t)(char *)bootAddress >> 7u;
    }
    else
    {
        IOMUXC_LPSR_GPR->GPR26 &= ~IOMUXC_LPSR_GPR_GPR26_CM7_INIT_VTOR_MASK;
        IOMUXC_LPSR_GPR->GPR26 |= IOMUXC_LPSR_GPR_GPR26_CM7_INIT_VTOR(((uint32_t)(char *)bootAddress) >> 7u);
    }

    SRC->CTRL_M7CORE = SRC_CTRL_M7CORE_SW_RESET_MASK;
    SRC->SCR |= SRC_SCR_BT_RELEASE_M7_MASK;
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
    if (value == ((void *)0))
    {
        /* If value is null set only length */
        switch (property)
        {
            case kMCMGR_CoreStatus:
                *length = sizeof(mcmgr_core_status_t);
                break;

#if !(defined(FSL_FEATURE_MU_NO_PM) && FSL_FEATURE_MU_NO_PM)
            case kMCMGR_CorePowerMode:
                *length = sizeof(mu_power_mode_t);
                break;
#endif /* FSL_FEATURE_MU_NO_PM */

            case kMCMGR_CoreType:
                *length = sizeof(mcmgr_core_type_t);
                break;

            default:
                return kStatus_MCMGR_Error;
        }
    }
    else
    {
        switch (property)
        {
            case kMCMGR_CoreStatus:
                if (*length < sizeof(mcmgr_core_status_t))
                {
                    return kStatus_MCMGR_Error;
                }
                if (coreNum == kMCMGR_Core0)
                {
                    *((mcmgr_core_status_t *)value) = kMCMGR_NotInReset;
                }
                else if (coreNum == kMCMGR_Core1)
                {
/* Read BRS value from MU_SR, 0-processor is not in reset, 1-processor is in reset */
#if defined(FSL_FEATURE_MU_SIDE_A)
                    uint32_t reg = MUA->CR;
                    if (0UL != (reg & (1u << 7u)))
                    {
                        *((mcmgr_core_status_t *)value) = kMCMGR_InReset;
                    }
                    else
                    {
                        *((mcmgr_core_status_t *)value) = kMCMGR_NotInReset;
                    }
#endif
                }
                else
                {
                    /* should not happen */
                }
                break;

#if !(defined(FSL_FEATURE_MU_NO_PM) && FSL_FEATURE_MU_NO_PM)
            case kMCMGR_CorePowerMode:
                if (*length < sizeof(mu_power_mode_t))
                {
                    return kStatus_MCMGR_Error;
                }
/* Read BPM value from MU_SR - power mode */
#if defined(FSL_FEATURE_MU_SIDE_A)
                *((mu_power_mode_t *)value) = (mu_power_mode_t)MU_GetOtherCorePowerMode(MUA);
#endif
                break;
#endif /* FSL_FEATURE_MU_NO_PM */

            case kMCMGR_CoreType:
                if (*length < sizeof(mcmgr_core_type_t))
                {
                    return kStatus_MCMGR_Error;
                }
                *((mcmgr_core_type_t *)value) = g_mcmgrSystem.cores[(uint32_t)coreNum].coreType;
                break;

            default:
                return kStatus_MCMGR_Error;
        }
    }

    return kStatus_MCMGR_Success;
}

mcmgr_core_t mcmgr_get_current_core_internal(void)
{
#if defined(FSL_FEATURE_MU_SIDE_A)
    if (0U == (OCOTP->FUSEN[22].FUSE & BT_CORE_SEL_BIT))
    {
        /* CM4 is the boot core */
        return kMCMGR_Core0;
    }
    else
    {
        /* CM7 is the boot core */
        return kMCMGR_Core1;
    }
#else
    if (0U == (OCOTP->FUSEN[22].FUSE & BT_CORE_SEL_BIT))
    {
        /* CM4 is the boot core */
        return kMCMGR_Core1;
    }
    else
    {
        /* CM7 is the boot core */
        return kMCMGR_Core0;
    }
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
        MU_SendMsg(MUA, MCMGR_MU_CHANNEL, remoteData);
#elif defined(FSL_FEATURE_MU_SIDE_B)
        MU_SendMsg(MUB, MCMGR_MU_CHANNEL, remoteData);
#endif
    }
    else
    {
        /* This is a non-blocking call */
#if defined(FSL_FEATURE_MU_SIDE_A)
        MU_SendMsgNonBlocking(MUA, MCMGR_MU_CHANNEL, remoteData);
#elif defined(FSL_FEATURE_MU_SIDE_B)
        MU_SendMsgNonBlocking(MUB, MCMGR_MU_CHANNEL, remoteData);
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
    data = MU_ReceiveMsgNonBlocking(MUA, MCMGR_MU_CHANNEL);
#elif defined(FSL_FEATURE_MU_SIDE_B)
    data = MU_ReceiveMsgNonBlocking(MUB, MCMGR_MU_CHANNEL);
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

#if defined(__CM4_CMSIS_VERSION)
/* Cortex-M4 contains additional exception handlers */
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
#endif

#endif /* MCMGR_HANDLE_EXCEPTIONS */
