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
#define APP_OUTPUT1_CLOCK_NAME_ARRAY                                                                                  \
    {                                                                                                                 \
        "PLL3 SW Clock(Divided by 2)", "PLL2 Clock(Divided by 2)", "ENET PLL Clock(Divided by 2)", "Core Clock Root", \
            "IPG Clock Root", "Perclk Root", "PLL4 Main Clock",                                                       \
    }

#define APP_OUTPUT1_CLOCK_NAME_ENUM_ARRAY                                                                             \
    {                                                                                                                 \
        kCLOCK_OutputPllUsb1Sw, kCLOCK_OutputPllSys, kCLOCK_OutputPllENET, kCLOCK_OutputCoreClk, kCLOCK_OutputIpgClk, \
            kCLOCK_OutputPerClk, kCLOCK_OutputPll4MainClk,                                                            \
    }

#define APP_OUTPUT2_CLOCK_NAME_ARRAY                                                                                   \
    {                                                                                                                  \
        "LPI2C Clock Root", "OSC Clock", "LPSPI Clock Root", "SAI1 Clock Root", "SAI3 Clock Root", "Trace Clock Root", \
            "Flexspi Clock Root", "UART Clock Root", "Spdif0 Clock Root",                                              \
    }

#define APP_OUTPUT2_CLOCK_NAME_ENUM_ARRAY                                                                              \
    {                                                                                                                  \
        kCLOCK_OutputLpi2cClk, kCLOCK_OutputOscClk, kCLOCK_OutputLpspiClk, kCLOCK_OutputSai1Clk, kCLOCK_OutputSai3Clk, \
            kCLOCK_OutputTraceClk, kCLOCK_OutputFlexspiClk, kCLOCK_OutputUartClk, kCLOCK_OutputSpdif0Clk,              \
    }

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
