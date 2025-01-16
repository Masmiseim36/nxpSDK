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

#define APP_OUTPUT1_CLOCK_NAME_ARRAY                                                                          \
    {                                                                                                         \
        "PLL3 SW Clock", "PLL2(divided by 2)", "ENET PLL(divided by 2)", "Semc Clock Root", "AHB Clock Root", \
            "IPG Clock Root", "Perclk Root", "PLL4 Main Clock",                                               \
    }

#define APP_OUTPUT1_CLOCK_NAME_ENUM_ARRAY                                                            \
    {                                                                                                \
        kCLOCK_OutputPllUsb1Sw, kCLOCK_OutputPllSys, kCLOCK_OutputPllENET500M, kCLOCK_OutputSemcClk, \
            kCLOCK_OutputAhbClk, kCLOCK_OutputIpgClk, kCLOCK_OutputPerClk, kCLOCK_OutputPll4MainClk, \
    }

#define APP_OUTPUT2_CLOCK_NAME_ARRAY                                                                       \
    {                                                                                                      \
        "USDHC1 Clock Root", "LPI2C Clock Root", "OSC Clock", "LPSPI Clock Root", "USDHC2 Clock Root",     \
            "SAI1 Clock Root", "SAI2 Clock Root", "SAI3 Clock Root", "Trace Clock Root", "Can Clock Root", \
            "Flexspi Clock Root", "UART Clock Root", "Spdif0 Clock Root",                                  \
    }

#define APP_OUTPUT2_CLOCK_NAME_ENUM_ARRAY                                                              \
    {                                                                                                  \
        kCLOCK_OutputUsdhc1Clk, kCLOCK_OutputLpi2cClk, kCLOCK_OutputOscClk, kCLOCK_OutputLpspiClk,     \
            kCLOCK_OutputUsdhc2Clk, kCLOCK_OutputSai1Clk, kCLOCK_OutputSai2Clk, kCLOCK_OutputSai3Clk,  \
            kCLOCK_OutputTraceClk, kCLOCK_OutputCanClk, kCLOCK_OutputFlexspiClk, kCLOCK_OutputUartClk, \
            kCLOCK_OutputSpdif0Clk,                                                                    \
    }

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
