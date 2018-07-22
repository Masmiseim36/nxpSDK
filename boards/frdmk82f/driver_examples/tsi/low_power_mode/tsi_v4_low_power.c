/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "board.h"
#include "fsl_tsi_v4.h"
#include "fsl_llwu.h"
#include "fsl_pmc.h"
#include "fsl_rcm.h"
#include "fsl_smc.h"
#include "fsl_debug_console.h"
#include "fsl_lptmr.h"

#include "clock_config.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Available PAD names on board */
#define PAD_TSI_ELECTRODE_1_NAME "D9"
#define PAD_TSI_ELECTRODE_2_NAME "D8"

/* TSI leds of electrode 1/2 */
#define LED1_INIT() LED_RED_INIT(LOGIC_LED_OFF)
#define LED1_ON() LED_RED_ON()
#define LED1_OFF() LED_RED_OFF()

#define LED2_INIT() LED_GREEN_INIT(LOGIC_LED_OFF)
#define LED2_ON() LED_GREEN_ON()
#define LED2_OFF() LED_GREEN_OFF()

/* LLWU module wakeup source index for TSI module */
#define LLWU_TSI_IDX 4U

/* Get source clock for LPTMR driver */
#define LPTMR_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_LpoClk)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
tsi_calibration_data_t buffer;

/*******************************************************************************
 * Code
 ******************************************************************************/
/* Delay n ms */
void DelayMs(uint32_t n)
{
    uint32_t temp;
    temp = n * ((CLOCK_GetFreq(kCLOCK_CoreSysClk)) / 1000U);

    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk; /* use core clock */
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;  /* disable interrupt */

    do
    {
        if (temp > SysTick_LOAD_RELOAD_Msk)
        {
            SysTick->LOAD = SysTick_LOAD_RELOAD_Msk;
            temp -= SysTick_LOAD_RELOAD_Msk;
        }
        else
        {
            SysTick->LOAD = temp;
            temp = 0;
        }
        SysTick->VAL = 0U;
        SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
        /* wait for timeout */
        while (0U == (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk))
        {
        }
        SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    } while (temp != 0);
}

void TSI0_IRQHandler(void)
{
    /* Clear flags */
    TSI_ClearStatusFlags(TSI0, kTSI_EndOfScanFlag);
    TSI_ClearStatusFlags(TSI0, kTSI_OutOfRangeFlag);
}

void LLWU_IRQHandler(void)
{
    if (LLWU_GetInternalWakeupModuleFlag(LLWU, LLWU_TSI_IDX))
    {
        /* Clear flags */
        TSI_ClearStatusFlags(TSI0, kTSI_EndOfScanFlag);
        TSI_ClearStatusFlags(TSI0, kTSI_OutOfRangeFlag);
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    volatile uint32_t i = 0;
    uint8_t usrInput = 0;
#if (defined(FSL_FEATURE_MCG_HAS_PLL) && FSL_FEATURE_MCG_HAS_PLL)
    bool tsi_pee_work_mode_flag = 0;
#endif /* FSL_FEATURE_MCG_HAS_PLL */
    tsi_config_t tsiConfig_normal = {0};
    tsi_config_t tsiConfig_lowPower = {0};
    lptmr_config_t lptmrConfig;
    memset((void *)&lptmrConfig, 0, sizeof(lptmrConfig));
    oscer_config_t osc_tsiConfig = {0};

    osc_tsiConfig.enableMode = kOSC_ErClkEnable | kOSC_ErClkEnableInStop;
#if (defined(FSL_FEATURE_OSC_HAS_EXT_REF_CLOCK_DIVIDER) && FSL_FEATURE_OSC_HAS_EXT_REF_CLOCK_DIVIDER)
    osc_tsiConfig.erclkDiv = 0;
#endif

#if (defined(FSL_FEATURE_SMC_HAS_LLS_SUBMODE) && FSL_FEATURE_SMC_HAS_LLS_SUBMODE) || \
    (defined(FSL_FEATURE_SMC_HAS_LPOPO) && FSL_FEATURE_SMC_HAS_LPOPO)
    smc_power_mode_lls_config_t lls_config;
    lls_config.subMode = kSMC_StopSub3;
#if (defined(FSL_FEATURE_SMC_HAS_LPOPO) && FSL_FEATURE_SMC_HAS_LPOPO)
    lls_config.enableLpoClock = true;
#endif

#endif /* (FSL_FEATURE_SMC_HAS_LLS_SUBMODE || FSL_FEATURE_SMC_HAS_LPOPO) */
    smc_power_mode_vlls_config_t vlls_config;
    vlls_config.subMode = kSMC_StopSub1;
    vlls_config.enablePorDetectInVlls0 = true;
#if (defined(FSL_FEATURE_SMC_HAS_RAM2_POWER_OPTION) && FSL_FEATURE_SMC_HAS_RAM2_POWER_OPTION)
    vlls_config.enableRam2InVlls2 = true;
#endif
#if (defined(FSL_FEATURE_SMC_HAS_LPOPO) && FSL_FEATURE_SMC_HAS_LPOPO)
    vlls_config.enableLpoClock = true;
#endif

    /* Initialize standard SDK demo application pins */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
#if (defined(FSL_FEATURE_MCG_HAS_PLL) && FSL_FEATURE_MCG_HAS_PLL)
    if (kMCG_ModePEE == CLOCK_GetMode())
    {
        tsi_pee_work_mode_flag = 1;
    }
#endif                         /* FSL_FEATURE_MCG_HAS_PLL */
    CLOCK_SetEr32kClock(0x3U); /* Use LPO as source clock */
    OSC_SetExtRefClkConfig(OSC0, &osc_tsiConfig);

    /* Check wake up from VLLSx mode */
    if (RCM_GetPreviousResetSources(RCM) & 0x01U)
    {
        PMC_ClearPeriphIOIsolationFlag(PMC);
        PRINTF("\r\nWake up from VLLSx modes...\r\n");
    }

    CLOCK_EnableClock(kCLOCK_Tsi0); /* Enable TSI clock after wakeup reset */
    TSI_EnableModule(TSI0, true);   /* Enable module */
    NVIC_EnableIRQ(LLWU_IRQn);
    NVIC_EnableIRQ(TSI0_IRQn);
    TSI_EnableInterrupts(TSI0, kTSI_GlobalInterruptEnable);
    TSI_EnableInterrupts(TSI0, kTSI_OutOfRangeInterruptEnable);

    /* TSI default hardware configuration for normal mode */
    TSI_GetNormalModeDefaultConfig(&tsiConfig_normal);
    TSI_Init(TSI0, &tsiConfig_normal);
    TSI_EnableHardwareTriggerScan(TSI0, false); /* Enable software trigger scan */

    PRINTF("\r\nTSI_V4 Low_Power_Modes Example Start!\r\n");
    /************ CALIBRATION PROCESS ************/
    PRINTF("Please don't touch the touchpad.\r\n");
    PRINTF("Press anykey to start TSI low power example. \r\n");
    GETCHAR();
    PRINTF("Start calibartion....\r\n");
    memset((void *)&buffer, 0, sizeof(buffer));
    TSI_Calibrate(TSI0, &buffer);
    /* Print calibrated counter values */
    for (i = 0U; i < FSL_FEATURE_TSI_CHANNEL_COUNT; i++)
    {
        PRINTF("Calibrated counters for channel %d is: %d \r\n", i, buffer.calibratedData[i]);
    }
    /************ LOW POWER FUNCTIONS **********/
    /*
     * lptmrConfig.timerMode = kLPTMR_TimerModeTimeCounter;
     * lptmrConfig.pinSelect = kLPTMR_PinSelectInput_0;
     * lptmrConfig.pinPolarity = kLPTMR_PinPolarityActiveHigh;
     * lptmrConfig.enableFreeRunning = false;
     * lptmrConfig.bypassPrescaler = true;
     * lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_1;
     * lptmrConfig.value = kLPTMR_Prescale_Glitch_0;
     */
    LPTMR_GetDefaultConfig(&lptmrConfig);                                     /* Configure LPTMR */
    lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_1;               /* Use LPO as clock source. */
    LPTMR_Init(LPTMR0, &lptmrConfig);                                         /* Initialize the LPTMR */
    LPTMR_SetTimerPeriod(LPTMR0, USEC_TO_COUNT(500000U, LPTMR_SOURCE_CLOCK)); /* Set timer period */
    /* TSI hardware configuration for low power mode */
    TSI_GetLowPowerModeDefaultConfig(&tsiConfig_lowPower);
    TSI_Init(TSI0, &tsiConfig_lowPower);
    TSI_EnableModule(TSI0, false);                             /* Disable module */
    TSI_EnableLowPower(TSI0, true);                            /* Enable low power */
    TSI_SetMeasuredChannelNumber(TSI0, BOARD_TSI_ELECTRODE_1); /* Using BOARD_TSI_ELECTRODE_1 as wakeup electrode */
    TSI_SetLowThreshold(TSI0, buffer.calibratedData[BOARD_TSI_ELECTRODE_1] / 3);
    TSI_SetHighThreshold(TSI0, buffer.calibratedData[BOARD_TSI_ELECTRODE_1] + 100U);
    TSI_EnableHardwareTriggerScan(TSI0, true);
    TSI_EnableInterrupts(TSI0, kTSI_GlobalInterruptEnable);
    TSI_EnableInterrupts(TSI0, kTSI_OutOfRangeInterruptEnable);
    TSI_ClearStatusFlags(TSI0, kTSI_OutOfRangeFlag);
    TSI_EnableModule(TSI0, true);

    SMC_SetPowerModeProtection(SMC, SMC_PMPROT_AVLLS_MASK | SMC_PMPROT_ALLS_MASK | SMC_PMPROT_AVLP_MASK);

    PRINTF("\r\nLet's move to the low power modes world!!!\r\n");
    PRINTF("\r\nPress specific key to enter stop mode.\r\n");
    PRINTF("After enter stop modes, touch PAD %s to wakeup.\r\n", PAD_TSI_ELECTRODE_1_NAME);
    LPTMR_StartTimer(LPTMR0); /* Start LPTMR triggering */

    while (1)
    {
        PRINTF("\r\nPlease input the mode you want to enter!");
        PRINTF(
            "\r\n0 ------>  STOP MODE \
                \r\n1 ------>  VLPS MODE \
                \r\n2 ------>  LLS  MODE \
                \r\n3 ------>  VLLS1 MODE \
                \r\n4 ------>  VLLS2 MODE \
                \r\n5 ------>  VLLS3 MODE\r\n");

        usrInput = GETCHAR();
        switch (usrInput)
        {
            case '0':
                PRINTF("About to enter into STOP MODE! \r\n");
                DelayMs(1U);
                SMC_SetPowerModeStop(SMC, kSMC_PartialStop);

#if (defined(FSL_FEATURE_MCG_HAS_PLL) && FSL_FEATURE_MCG_HAS_PLL)
                if (1 == tsi_pee_work_mode_flag)
                {
                    CLOCK_SetPeeMode();
                }
#endif /* FSL_FEATURE_MCG_HAS_PLL */
                PRINTF("Great!!! You touched it!!!\r\n");
                break;
            case '1':
                PRINTF("About to enter into VLPS MODE! \r\n");
                DelayMs(1U);
                SMC_SetPowerModeVlps(SMC);

#if (defined(FSL_FEATURE_MCG_HAS_PLL) && FSL_FEATURE_MCG_HAS_PLL)
                if (1 == tsi_pee_work_mode_flag)
                {
                    CLOCK_SetPeeMode();
                }
#endif /* FSL_FEATURE_MCG_HAS_PLL */
                PRINTF("Great!!! You touched it!!!\r\n");
                break;
#if (defined(FSL_FEATURE_SMC_HAS_LLS_SUBMODE) && FSL_FEATURE_SMC_HAS_LLS_SUBMODE) || \
    (defined(FSL_FEATURE_SMC_HAS_LPOPO) && FSL_FEATURE_SMC_HAS_LPOPO)
            case '2':
                PRINTF("About to enter into LLS MODE! \r\n");
                PRINTF("Wakeup will lead to a interrupt flow, pay attention to it!!!\r\n");
                DelayMs(1U);
                LLWU_EnableInternalModuleInterruptWakup(LLWU, LLWU_TSI_IDX, true);
                SMC_SetPowerModeLls(SMC, &lls_config);

#if (defined(FSL_FEATURE_MCG_HAS_PLL) && FSL_FEATURE_MCG_HAS_PLL)
                if (1 == tsi_pee_work_mode_flag)
                {
                    CLOCK_SetPeeMode();
                }
#endif /* FSL_FEATURE_MCG_HAS_PLL */
                PRINTF("Great!!! You touched it!!!\r\n");
                break;
#endif
            case '3':
                PRINTF("About to enter into VLLS1 MODE! \r\n");
                PRINTF("Wakeup will lead to a reset flow, pay attention to it!!!\r\n");
                LLWU_EnableInternalModuleInterruptWakup(LLWU, LLWU_TSI_IDX, true);
                SMC_SetPowerModeVlls(SMC, &vlls_config);

                PRINTF("\r\nShould never get here !!!!!!\r\n");
                break;

            case '4':
                PRINTF("About to enter into VLLS2 MODE! \r\n");
                PRINTF("Wakeup will lead to a reset flow, pay attention to it!!!\r\n");
                LLWU_EnableInternalModuleInterruptWakup(LLWU, LLWU_TSI_IDX, true);
                vlls_config.subMode = kSMC_StopSub2;
                SMC_SetPowerModeVlls(SMC, &vlls_config);

                PRINTF("\r\nShould never get here !!!!!!\r\n");
                break;

            case '5':
                PRINTF("About to enter into VLLS3 MODE! \r\n");
                PRINTF("Wakeup will lead to a reset flow, pay attention to it!!!\r\n");
                LLWU_EnableInternalModuleInterruptWakup(LLWU, LLWU_TSI_IDX, true);
                vlls_config.subMode = kSMC_StopSub3;
                SMC_SetPowerModeVlls(SMC, &vlls_config);

                PRINTF("\r\nShould never get here !!!!!!\r\n");
                break;

            default:
                PRINTF("Please input a valid character, 0~5!!!\r\n");
                break;
        } /* End of switch(usr_input) */
    }     /* End of while(1) */
} /* End of main() */
