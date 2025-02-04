/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017, 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"

#if defined(FSL_FEATURE_SOC_PIT_COUNT) && FSL_FEATURE_SOC_PIT_COUNT
#include "fsl_pit.h"
#endif

#if defined(FSL_FEATURE_SOC_XBARA_COUNT) && FSL_FEATURE_SOC_XBARA_COUNT
#include "fsl_xbara.h"
#endif

#if defined(FSL_FEATURE_SOC_XBARB_COUNT) && FSL_FEATURE_SOC_XBARB_COUNT
#include "fsl_xbarb.h"
#endif

#if defined(FSL_FEATURE_SOC_LPIT_COUNT) && FSL_FEATURE_SOC_LPIT_COUNT
#include "fsl_lpit.h"
#endif

#if defined(FSL_FEATURE_SOC_XBAR_DSC_COUNT) && FSL_FEATURE_SOC_XBAR_DSC_COUNT
#include "fsl_xbar.h"
#endif

#include "fsl_aoi.h"
#include "board.h"
#include "app.h"

#if defined(FSL_FEATURE_SOC_ACMP_COUNT) && FSL_FEATURE_SOC_ACMP_COUNT
#include "fsl_acmp.h"
#elif defined(FSL_FEATURE_SOC_CMP_COUNT) && FSL_FEATURE_SOC_CMP_COUNT
#include "fsl_cmp.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Initialize the CMP.
 *
 */
#if defined(DEMO_CMP_BASEADDR)
static void CMP_Configuration(void);
#endif

/*!
 * @brief Initialize the PIT timer.
 *
 */
static void PIT_Configuration(void);

/*!
 * @brief Initialize the XBAR module.
 *
 */
static void XBAR_Configuration(void);

/*!
 * @brief Initialize the AOI.
 *
 */
static void AOI_Configuration(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_xbaraInterrupt = false;
/*******************************************************************************
 * Code
 ******************************************************************************/

#if defined(DEMO_CMP_BASEADDR)
static void CMP_Configuration(void)
{
#if defined(FSL_FEATURE_SOC_ACMP_COUNT) && FSL_FEATURE_SOC_ACMP_COUNT
    acmp_config_t acmpConfig;
    acmp_channel_config_t channelConfigStruct;
    acmp_discrete_mode_config_t acmpDiscreteconfig;
    acmp_dac_config_t dacConfigStruct;

    ACMP_GetDefaultConfig(&acmpConfig);
    acmpConfig.enablePinOut = true;
    ACMP_Init(DEMO_CMP_BASEADDR, &acmpConfig);

    /* Configure positive inputs are coming from 3v domain. */
    ACMP_GetDefaultDiscreteModeConfig(&acmpDiscreteconfig);
    acmpDiscreteconfig.enablePositiveChannelDiscreteMode = true;
    ACMP_SetDiscreteModeConfig(DEMO_CMP_BASEADDR, &acmpDiscreteconfig);

    /* Configure channel. Select the negative port input from DAC and positive port input from plus mux input. */
    channelConfigStruct.minusMuxInput = DEMO_CMP_MINUS_CHANNEL;
    channelConfigStruct.plusMuxInput  = DEMO_CMP_PLUS_CHANNEL;
    ACMP_SetChannelConfig(DEMO_CMP_BASEADDR, &channelConfigStruct);

    /* Configure DAC. */
#if defined(DEMO_CMP_USE_ALT_VREF) && DEMO_CMP_USE_ALT_VREF
    dacConfigStruct.referenceVoltageSource = kACMP_VrefSourceVin2;
#else
    dacConfigStruct.referenceVoltageSource = kACMP_VrefSourceVin1;
#endif                                /* DEMO_ACMP_USE_ALT_VREF */
    dacConfigStruct.DACValue = 0x7FU; /* Half of referene voltage. */
#if defined(FSL_FEATURE_ACMP_HAS_C1_DACOE_BIT) && (FSL_FEATURE_ACMP_HAS_C1_DACOE_BIT == 1U)
    dacConfigStruct.enableOutput = false;
#endif
    dacConfigStruct.workMode = kACMP_DACWorkLowSpeedMode;
    ACMP_SetDACConfig(DEMO_CMP_BASEADDR, &dacConfigStruct);

    ACMP_Enable(DEMO_CMP_BASEADDR, true);
#elif defined(FSL_FEATURE_SOC_CMP_COUNT) && FSL_FEATURE_SOC_CMP_COUNT
    cmp_config_t cmpConfig;
    cmp_dac_config_t cmpdacConfig;

    cmpdacConfig.referenceVoltageSource = kCMP_VrefSourceVin2;
    cmpdacConfig.DACValue               = 32U; /* Set DAC output value */

    CMP_GetDefaultConfig(&cmpConfig);
    CMP_Init(DEMO_CMP_BASEADDR, &cmpConfig);
    /* Set input plus is CMP_channel1, input minus is CMP_DAC out */
    CMP_SetInputChannels(DEMO_CMP_BASEADDR, DEMO_CMP_MINUS_CHANNEL, DEMO_CMP_PLUS_CHANNEL);
    CMP_SetDACConfig(DEMO_CMP_BASEADDR, &cmpdacConfig);
#endif
}
#endif

#if defined(FSL_FEATURE_SOC_PIT_COUNT) && FSL_FEATURE_SOC_PIT_COUNT
static void PIT_Configuration(void)
{
    pit_config_t pitConfig;

    PIT_GetDefaultConfig(&pitConfig);
    pitConfig.enableRunInDebug = false;
    PIT_Init(DEMO_PIT_BASEADDR, &pitConfig);

    /* Set period is 500ms */
    PIT_SetTimerPeriod(DEMO_PIT_BASEADDR, DEMO_PIT_CHANNEL, USEC_TO_COUNT(500000u, BUS_CLK_FREQ));
    PIT_StartTimer(DEMO_PIT_BASEADDR, DEMO_PIT_CHANNEL);
}

#elif defined(FSL_FEATURE_SOC_LPIT_COUNT) && FSL_FEATURE_SOC_LPIT_COUNT
static void PIT_Configuration(void)
{
    lpit_config_t lpitConfig;
    lpit_chnl_params_t lpitChannelConfig;

    LPIT_GetDefaultConfig(&lpitConfig);
    lpitConfig.enableRunInDebug = false;
    LPIT_Init(DEMO_LPIT_BASE, &lpitConfig);

    lpitChannelConfig.chainChannel          = false;
    lpitChannelConfig.enableReloadOnTrigger = false;
    lpitChannelConfig.enableStartOnTrigger  = false;
    lpitChannelConfig.enableStopOnTimeout   = false;
    lpitChannelConfig.timerMode             = kLPIT_PeriodicCounter;

    /* Set default values for the trigger source */
    lpitChannelConfig.triggerSelect = kLPIT_Trigger_TimerChn0;
    lpitChannelConfig.triggerSource = kLPIT_TriggerSource_External;

    LPIT_SetupChannel(DEMO_LPIT_BASE, DEMO_LPIT_CHANNEL, &lpitChannelConfig);

    /* Set timer period to 0.5 second */
    LPIT_SetTimerPeriod(DEMO_LPIT_BASE, DEMO_LPIT_CHANNEL, USEC_TO_COUNT(500000U, LPIT_SOURCECLOCK));

    LPIT_StartTimer(DEMO_LPIT_BASE, DEMO_LPIT_CHANNEL);
}
#endif

#if defined(FSL_FEATURE_SOC_XBARA_COUNT) && FSL_FEATURE_SOC_XBARA_COUNT && defined(FSL_FEATURE_SOC_XBARB_COUNT) && FSL_FEATURE_SOC_XBARB_COUNT

static void XBAR_Configuration(void)
{
    xbara_control_config_t xbaraConfig;

    /* Init XBAR module. */
    XBARA_Init(DEMO_XBARA_BASEADDR);
    XBARB_Init(DEMO_XBARB_BASEADDR);

    /* Configure the XBAR signal connections */
#if defined(DEMO_CMP_BASEADDR)
    XBARB_SetSignalsConnection(DEMO_XBARB_BASEADDR, DEMO_XBARB_INPUT_CMP_SIGNAL, DEMO_XBARB_OUTPUT_AOI_SIGNAL_1);
#endif
    XBARB_SetSignalsConnection(DEMO_XBARB_BASEADDR, DEMO_XBARB_INPUT_PIT_SIGNAL, DEMO_XBARB_OUTPUT_AOI_SIGNAL_2);
    XBARA_SetSignalsConnection(DEMO_XBARA_BASEADDR, DEMO_XBARA_INPUT_AOI_SIGNAL, DEMO_XBARA_OUTPUT_SIGNAL);

    /* Configure the XBARA interrupt */
    xbaraConfig.activeEdge  = kXBARA_EdgeRising;
    xbaraConfig.requestType = kXBARA_RequestInterruptEnalbe;
    XBARA_SetOutputSignalConfig(DEMO_XBARA_BASEADDR, DEMO_XBARA_OUTPUT_SIGNAL, &xbaraConfig);

    /* Enable at the NVIC. */
    EnableIRQ(DEMO_XBARA_IRQ_ID);
}

void DEMO_XBARA_IRQ_HANDLER_FUNC(void)
{
    /* Clear interrupt flag */
    XBARA_ClearStatusFlags(DEMO_XBARA_BASEADDR, kXBARA_EdgeDetectionOut0);
    g_xbaraInterrupt = true;
    SDK_ISR_EXIT_BARRIER;
}

#elif defined(FSL_FEATURE_SOC_XBAR_DSC_COUNT) && FSL_FEATURE_SOC_XBAR_DSC_COUNT

static void XBAR_Configuration(void)
{
    xbar_control_config_t xbaraConfig;

    /* Init XBAR module. */
    XBAR_Init(kXBAR_DSC1);
    XBAR_Init(kXBAR_DSC2);

    /* Configure the XBAR signal connections */
#if defined(DEMO_CMP_BASEADDR)
    XBAR_SetSignalsConnection(DEMO_XBAR_INPUT_CMP_SIGNAL, DEMO_XBAR_OUTPUT_AOI_SIGNAL_1);
#endif
    XBAR_SetSignalsConnection(DEMO_XBAR_INPUT_PIT_SIGNAL, DEMO_XBAR_OUTPUT_AOI_SIGNAL_2);
    XBAR_SetSignalsConnection(DEMO_XBAR_INPUT_AOI_SIGNAL, DEMO_XBAR_OUTPUT_SIGNAL);

    /* Configure the XBAR interrupt */
    xbaraConfig.activeEdge      = kXBAR_EdgeRising;
    xbaraConfig.requestType     = kXBAR_RequestInterruptEnable;
    XBAR_SetOutputSignalConfig(DEMO_XBAR_OUTPUT_SIGNAL, &xbaraConfig);
    EnableIRQ(DEMO_XBAR_IRQ_ID);
}

void DEMO_XBAR_IRQ_HANDLER_FUNC(void)
{
    bool status;

    XBAR_GetOutputStatusFlag(DEMO_XBAR_OUTPUT_SIGNAL, &status);
    if (status)
    {
        XBAR_ClearOutputStatusFlag(DEMO_XBAR_OUTPUT_SIGNAL);
    }
    g_xbaraInterrupt = true;

    SDK_ISR_EXIT_BARRIER;
}

#endif

static void AOI_Configuration(void)
{
    aoi_event_config_t aoiEventLogicStruct;

    /* Configure the AOI event */
#if defined(DEMO_CMP_BASEADDR)
    aoiEventLogicStruct.PT0AC = kAOI_InputSignal;    /* CMP0 output*/
#else
    aoiEventLogicStruct.PT0AC = kAOI_LogicOne;    /* force input to become 1 */
#endif
    aoiEventLogicStruct.PT0BC = kAOI_InvInputSignal; /* PIT0 output*/
    aoiEventLogicStruct.PT0CC = kAOI_LogicOne;
    aoiEventLogicStruct.PT0DC = kAOI_LogicOne;

    aoiEventLogicStruct.PT1AC = kAOI_LogicZero;
    aoiEventLogicStruct.PT1BC = kAOI_LogicOne;
    aoiEventLogicStruct.PT1CC = kAOI_LogicOne;
    aoiEventLogicStruct.PT1DC = kAOI_LogicOne;

    aoiEventLogicStruct.PT2AC = kAOI_LogicZero;
    aoiEventLogicStruct.PT2BC = kAOI_LogicOne;
    aoiEventLogicStruct.PT2CC = kAOI_LogicOne;
    aoiEventLogicStruct.PT2DC = kAOI_LogicOne;

    aoiEventLogicStruct.PT3AC = kAOI_LogicZero;
    aoiEventLogicStruct.PT3BC = kAOI_LogicOne;
    aoiEventLogicStruct.PT3CC = kAOI_LogicOne;
    aoiEventLogicStruct.PT3DC = kAOI_LogicOne;

    /* Init AOI module. */
    AOI_Init(DEMO_AOI_BASEADDR);
    AOI_SetEventLogicConfig(DEMO_AOI_BASEADDR, kAOI_Event0, &aoiEventLogicStruct);
}

int main(void)
{
    /* Init board hardware */
    BOARD_InitHardware();
    /* Init PIT timer */
    PIT_Configuration();
    /* Init CMP */
#if defined(DEMO_CMP_BASEADDR)
    CMP_Configuration();
#endif
    /* Init XBAR */
    XBAR_Configuration();
    /* Init AOI */
    AOI_Configuration();

    PRINTF("XBAR and AOI Demo: Start...\r\n");

    while (1)
    {
        if (g_xbaraInterrupt)
        {
            g_xbaraInterrupt = false;
            PRINTF("XBAR interrupt occurred\r\n\r\n");
        }
    }
}
