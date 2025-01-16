/*
 * Copyright 2021 NXP
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
#define APP_CLOCK_OUT_SELECT_ARRAY                                                                                  \
    {                                                                                                               \
        kSFRO_to_CLKOUT, kXTALIN_CLK_to_CLKOUT, kLPOSC_to_CLKOUT, kFFRO_to_CLKOUT, kMAIN_CLK_to_CLKOUT,             \
            kDSP_MAIN_to_CLKOUT, kMAIN_PLL_to_CLKOUT, kAUX0_PLL_to_CLKOUT, kDSP_PLL_to_CLKOUT, kAUX1_PLL_to_CLKOUT, \
            kAUDIO_PLL_to_CLKOUT,                                                                                   \
    }
#define APP_CLOCK_OUT_NAME_ARRAY                                                                                    \
    {                                                                                                               \
        "SFRO Clock", "External Clock", "Low Power Oscillator Clock", "FFRO Clock", "Main Clock", "DSP Main Clock", \
            "Main System PLL", "SYSPLL0 AUX0 PLL", "DSP PLL", "SYSPLL0 AUX1 PLL", "AUDIO PLL",                      \
    }

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
