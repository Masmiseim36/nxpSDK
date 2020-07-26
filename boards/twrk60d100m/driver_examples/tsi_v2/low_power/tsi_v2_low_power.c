/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
 * o Neither the name of the copyright holder nor the names of its
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

/*******************************************************************************
 *  Includes
 ******************************************************************************/

#include "board.h"
#include "fsl_tsi_v2.h"
#include "fsl_llwu.h"
#include "fsl_pmc.h"
#include "fsl_rcm.h"
#include "fsl_smc.h"
#include "fsl_debug_console.h"

#include "clock_config.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Available PAD names on board */
#define PAD_TSI_ELECTRODE_1_NAME "D7"
#define PAD_TSI_ELECTRODE_2_NAME "D8"

/* LLWU module wakeup source index for TSI module */
#define LLWU_TSI_IDX 4U

#define CORE_CLK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/* Delay n ms */
void DelayMs(uint32_t n)
{
    uint32_t temp;
    temp = n * ((CORE_CLK_FREQ) / 1000U);

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
    uint8_t usr_input = 0U;
#if (defined(FSL_FEATURE_MCG_HAS_PLL) && FSL_FEATURE_MCG_HAS_PLL)
    bool tsi_pee_work_mode_flag = 0;
#endif /* FSL_FEATURE_MCG_HAS_PLL */
    tsi_config_t tsiConfig_lowPower = {0};

#if (defined(FSL_FEATURE_SOC_OSC_COUNT) && FSL_FEATURE_SOC_OSC_COUNT)
    oscer_config_t osc_tsiConfig = {0};
    osc_tsiConfig.enableMode = kOSC_ErClkEnable | kOSC_ErClkEnableInStop;
#if (defined(FSL_FEATURE_OSC_HAS_EXT_REF_CLOCK_DIVIDER) && FSL_FEATURE_OSC_HAS_EXT_REF_CLOCK_DIVIDER)
    osc_tsiConfig.erclkDiv = 0;
#endif /* FSL_FEATURE_OSC_HAS_EXT_REF_CLOCK_DIVIDER */
#endif /* FSL_FEATURE_SOC_OSC_COUNT */

#if (defined(FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE)
    smc_power_mode_vlls_config_t vlls_config;
    vlls_config.subMode = kSMC_StopSub1;
#if (defined(FSL_FEATURE_SMC_HAS_PORPO) && FSL_FEATURE_SMC_HAS_PORPO)
    vlls_config.enablePorDetectInVlls0 = true;
#endif /* FSL_FEATURE_SMC_HAS_PORPO */
#if (defined(FSL_FEATURE_SMC_HAS_RAM2_POWER_OPTION) && FSL_FEATURE_SMC_HAS_RAM2_POWER_OPTION)
    vlls_config.enableRam2InVlls2 = true;
#endif /* FSL_FEATURE_SMC_HAS_RAM2_POWER_OPTION */
#if (defined(FSL_FEATURE_SMC_HAS_LPOPO) && FSL_FEATURE_SMC_HAS_LPOPO)
    vlls_config.enableLpoClock = true;
#endif /* FSL_FEATURE_SMC_HAS_LPOPO */
#endif /* FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE */

    /* Initialize standard SDK demo application pins */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
#if (defined(FSL_FEATURE_MCG_HAS_PLL) && FSL_FEATURE_MCG_HAS_PLL)
    if (kMCG_ModePEE == CLOCK_GetMode())
    {
        tsi_pee_work_mode_flag = 1;
    }
#endif                          /* FSL_FEATURE_MCG_HAS_PLL */
#if (defined(FSL_FEATURE_SIM_OPT_HAS_OSC32K_SELECTION) && FSL_FEATURE_SIM_OPT_HAS_OSC32K_SELECTION)
    CLOCK_SetEr32kClock(0x03U); /* LPO 1kHz for TSI */
#endif  /* FSL_FEATURE_SIM_OPT_HAS_OSC32K_SELECTION */
#if (defined(FSL_FEATURE_SOC_OSC_COUNT) && FSL_FEATURE_SOC_OSC_COUNT)
    OSC_SetExtRefClkConfig(OSC, &osc_tsiConfig);
#endif  /* FSL_FEATURE_SOC_OSC_COUNT */
    TSI_GetLowPowerModeDefaultConfig(&tsiConfig_lowPower);

    CLOCK_EnableClock(kCLOCK_Tsi0);
    TSI_EnableModule(TSI0, true); /* Enable module */
    EnableIRQ(TSI0_IRQn);
    TSI_EnableInterrupts(TSI0, kTSI_GlobalInterruptEnable);
    TSI_EnableInterrupts(TSI0, kTSI_OutOfRangeInterruptEnable);

    /* Setup LLWU for wakeup. */
    if (RCM_GetPreviousResetSources(RCM) & 0x01U)
    {
        PMC_ClearPeriphIOIsolationFlag(PMC);
        PRINTF("\r\nWake up from VLLSx modes...\r\n");
        PRINTF("Wakeup channel Counter is:%d\r\n", TSI_GetWakeUpChannelCounter(TSI0));
    }

    PRINTF("\r\nTSI_V2 Low_Power_Modes Example Start!\r\n");
    /************ LOW POWER FUNCTIONS **********/
    TSI_EnableModule(TSI0, false);  /* Disable module before hardware configuration */
    TSI_EnableLowPower(TSI0, true); /* Enable low power */
    TSI_Init(TSI0, &tsiConfig_lowPower);
    TSI_EnableChannel(TSI0, BOARD_TSI_ELECTRODE_1, true);
    TSI_SetLowPowerChannel(TSI0, BOARD_TSI_ELECTRODE_1); /* Using BOARD_TSI_ELECTRODE_1 as wakeup electrode */
    TSI_EnablePeriodicalScan(TSI0, true);
    TSI_EnableInterrupts(TSI0, kTSI_GlobalInterruptEnable);
    TSI_EnableInterrupts(TSI0, kTSI_OutOfRangeInterruptEnable);
    TSI_ClearStatusFlags(TSI0, kTSI_OutOfRangeFlag);
    TSI_EnableModule(TSI0, true); /* Enable module, the hardware trigger begins */

    SMC_SetPowerModeProtection(SMC, SMC_PMPROT_AVLLS_MASK | SMC_PMPROT_ALLS_MASK | SMC_PMPROT_AVLP_MASK);
    SIM->SCGC4 |= 1U << SIM_SCGC4_LLWU_SHIFT; /* Enable LLWU module clock */

    PRINTF("\r\nLet's move to the low power modes world!!!\r\n");
    PRINTF("\r\nPress specific key to enter stop mode.\r\n");
    PRINTF("After enter stop modes, touch pad %s to wakeup.\r\n", PAD_TSI_ELECTRODE_1_NAME);
    while (1)
    {
        PRINTF("\r\n Please input the mode you want to enter! \r\n");
#if (defined(FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE)
        PRINTF("0 ------>  LLS  MODE \r\n");
#endif /* FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE */
#if (defined(FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE)
        PRINTF("1 ------>  VLLS1 MODE\r\n");
        PRINTF("2 ------>  VLLS2 MODE\r\n");
        PRINTF("3 ------>  VLLS3 MODE\r\n");
#endif  /* FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE */

        usr_input = GETCHAR();
        switch (usr_input)
        {
#if (defined(FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE)
            case '0':
                PRINTF("About to enter into LLS MODE! \r\n");
                PRINTF("Wakeup will lead to a interrupt flow, pay attention to it!!!\r\n");
                DelayMs(1U);
                EnableIRQ(LLWU_IRQn);
                LLWU_EnableInternalModuleInterruptWakup(LLWU, LLWU_TSI_IDX, true);
                TSI_ClearStatusFlags(TSI0, kTSI_OutOfRangeFlag);
                SMC_SetPowerModeLls(SMC);

#if (defined(FSL_FEATURE_MCG_HAS_PLL) && FSL_FEATURE_MCG_HAS_PLL)
                if (1 == tsi_pee_work_mode_flag)
                {
                    CLOCK_SetPeeMode();
                }
#endif /* FSL_FEATURE_MCG_HAS_PLL */
                PRINTF("Great!!! You touched it!!!\r\n");
                break;
#endif /* FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE */

#if (defined(FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE)
            case '1':
                PRINTF("About to enter into VLLS1 MODE! \r\n");
                PRINTF("Wakeup will lead to a reset flow, pay attention to it!!!\r\n");
                EnableIRQ(LLWU_IRQn);
                LLWU_EnableInternalModuleInterruptWakup(LLWU, LLWU_TSI_IDX, true);
                TSI_ClearStatusFlags(TSI0, kTSI_OutOfRangeFlag);
                SMC_SetPowerModeVlls(SMC, &vlls_config);

                PRINTF("\r\nShould never get here !!!!!!\r\n");
                break;

            case '2':
                PRINTF("About to enter into VLLS2 MODE! \r\n");
                PRINTF("Wakeup will lead to a reset flow, pay attention to it!!!\r\n");
                EnableIRQ(LLWU_IRQn);
                LLWU_EnableInternalModuleInterruptWakup(LLWU, LLWU_TSI_IDX, true);
                TSI_ClearStatusFlags(TSI0, kTSI_OutOfRangeFlag);
                vlls_config.subMode = kSMC_StopSub2;
                SMC_SetPowerModeVlls(SMC, &vlls_config);

                PRINTF("\r\nShould never get here !!!!!!\r\n");
                break;

            case '3':
                PRINTF("About to enter into VLLS3 MODE! \r\n");
                PRINTF("Wakeup will lead to a reset flow, pay attention to it!!!\r\n");
                EnableIRQ(LLWU_IRQn);
                LLWU_EnableInternalModuleInterruptWakup(LLWU, LLWU_TSI_IDX, true);
                TSI_ClearStatusFlags(TSI0, kTSI_OutOfRangeFlag);
                vlls_config.subMode = kSMC_StopSub3;
                SMC_SetPowerModeVlls(SMC, &vlls_config);

                PRINTF("\r\nShould never get here !!!!!!\r\n");
                break;
#endif    /* FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE */

            default:
                PRINTF("Please input a valid character, 0~3!!!\r\n");
                break;
        } /* End of switch(usr_input) */
    }     /* End of while(1) */
} /* End of main() */
