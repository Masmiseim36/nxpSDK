/*
 * The Clear BSD License
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
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
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
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

#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_XTAL0_CLK_HZ 32768U

#define LED_INIT() LED_RED_INIT(LOGIC_LED_OFF)
#define LED_TOGGLE() LED_RED_TOGGLE()
#define MAX_STEP_NUM 13

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_InitOsc0(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

void BOARD_InitOsc0(void)
{
    const osc_config_t oscConfig = {.freq = BOARD_XTAL0_CLK_HZ,
                                    .capLoad = 0U,
                                    .workMode = kOSC_ModeOscLowPower,
                                    .oscerConfig = {
                                        .enableMode = kOSC_ErClkEnable,
#if (defined(FSL_FEATURE_OSC_HAS_EXT_REF_CLOCK_DIVIDER) && FSL_FEATURE_OSC_HAS_EXT_REF_CLOCK_DIVIDER)
                                        .erclkDiv = 0U,
#endif
                                    }};

    CLOCK_InitOsc0(&oscConfig);

    /* Passing the XTAL0 frequency to clock driver. */
    CLOCK_SetXtal0Freq(BOARD_XTAL0_CLK_HZ);
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t volatile i = 0U;
    uint8_t curStep = 1U;

    /* MCG_Lite configuration for HIRC mode. */
    mcglite_config_t mcgliteConfigHirc;
    mcgliteConfigHirc.outSrc = kMCGLITE_ClkSrcHirc;
    mcgliteConfigHirc.irclkEnableMode = kMCGLITE_IrclkEnable;
    mcgliteConfigHirc.ircs = kMCGLITE_Lirc2M;
    mcgliteConfigHirc.fcrdiv = kMCGLITE_LircDivBy1;
    mcgliteConfigHirc.lircDiv2 = kMCGLITE_LircDivBy2;
    mcgliteConfigHirc.hircEnableInNotHircMode = false;

    /* MCG_Lite configuration for LIRC2M mode. */
    mcglite_config_t mcgliteConfigLirc2M;
    mcgliteConfigLirc2M.outSrc = kMCGLITE_ClkSrcLirc;
    mcgliteConfigLirc2M.irclkEnableMode = 0U;
    mcgliteConfigLirc2M.ircs = kMCGLITE_Lirc2M;
    mcgliteConfigLirc2M.fcrdiv = kMCGLITE_LircDivBy1;
    mcgliteConfigLirc2M.lircDiv2 = kMCGLITE_LircDivBy2;
    mcgliteConfigLirc2M.hircEnableInNotHircMode = true;

    /* MCG_Lite configuration for EXT mode. */
    mcglite_config_t mcgliteConfigExt;
    mcgliteConfigExt.outSrc = kMCGLITE_ClkSrcExt;
    mcgliteConfigExt.irclkEnableMode = 0U;
    mcgliteConfigExt.ircs = kMCGLITE_Lirc8M;
    mcgliteConfigExt.fcrdiv = kMCGLITE_LircDivBy2;
    mcgliteConfigExt.lircDiv2 = kMCGLITE_LircDivBy4;
    mcgliteConfigExt.hircEnableInNotHircMode = false;

    /* MCG_Lite configuration for LIRC8M mode. */
    mcglite_config_t mcgliteConfigLirc8M;
    mcgliteConfigLirc8M.outSrc = kMCGLITE_ClkSrcLirc;
    mcgliteConfigLirc8M.irclkEnableMode = 0U;
    mcgliteConfigLirc8M.ircs = kMCGLITE_Lirc8M;
    mcgliteConfigLirc8M.fcrdiv = kMCGLITE_LircDivBy2;
    mcgliteConfigLirc8M.lircDiv2 = kMCGLITE_LircDivBy4;
    mcgliteConfigLirc8M.hircEnableInNotHircMode = false;

    /* Init hardware */
    BOARD_InitPins();
    BOARD_InitOsc0();
    CLOCK_SetXtal0Freq(BOARD_XTAL0_CLK_HZ);
    /* Init using Led in Demo app */
    LED_INIT();

    while (curStep < MAX_STEP_NUM)
    {
        switch (curStep)
        {
            case 1U:
            case 4U:
            case 8U:
                /* Set to HIRC mode */
                CLOCK_SetMcgliteConfig(&mcgliteConfigHirc);
                break;
            case 2U:
            case 7U:
            case 11U:
                /* Set LIRC2M mode */
                CLOCK_SetMcgliteConfig(&mcgliteConfigLirc2M);
                break;
            case 3U:
            case 6U:
            case 9U:
                /* Set EXT mode */
                CLOCK_SetMcgliteConfig(&mcgliteConfigExt);
                break;
            case 5U:
            case 10U:
            case 12U:
                /* Set LIRC8M mode */
                CLOCK_SetMcgliteConfig(&mcgliteConfigLirc8M);
                break;
            default:
                /* Should never reach here */
                break;
        }

        curStep++;

        i = 0x400;
        while (i--)
        {
        }
    }

    /* LED blinks. */
    while (1)
    {
        i = 0x40000;
        while (i--)
        {
        }
        LED_TOGGLE();
    }
}
