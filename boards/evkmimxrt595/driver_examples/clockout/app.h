/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define APP_CLOCK_OUT_SELECT_ARRAY                                                                                   \
    {                                                                                                                \
        kOSC_CLK_to_CLKOUT, kLPOSC_to_CLKOUT, kFRO_DIV2_to_CLKOUT, kMAIN_CLK_to_CLKOUT, kDSP_MAIN_to_CLKOUT,         \
            kMAIN_PLL_to_CLKOUT, kAUX0_PLL_to_CLKOUT, kDSP_PLL_to_CLKOUT, kAUX1_PLL_to_CLKOUT, kAUDIO_PLL_to_CLKOUT, \
            kOSC32K_to_CLKOUT,                                                                                       \
    }
#define APP_CLOCK_OUT_NAME_ARRAY                                                                                       \
    {                                                                                                                  \
        "OSC_CLK Clock", "Low Power Oscillator Clock", "FRO_DIV2 Clock", "Main Clock", "Dsp Main Clock",               \
            "Main System PLL", "SYSPLL0 AUX0_PLL_Clock", "DSP PLL Clock", "SYSPLL0 AUX1_PLL_Clock", "AUDIO PLL Clock", \
            "32 KHz RTC Clock",                                                                                        \
    }

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
