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

#define APP_OUTPUT1_CLOCK_NAME_ARRAY                                                                      \
    {                                                                                                     \
        "USB1 PLL Clock(Divided by 2)", "SYS PLL Clock(Divided by 2)", "VIDEO PLL Clock(Divided by 2)",   \
            "Semc Clock Root", "Lcdif Pix Clock Root", "AHB Clock Root", "IPG Clock Root", "Perclk Root", \
            "Ckil Sync Clock Root", "PLL4 Main Clock",                                                    \
    }

#define APP_OUTPUT1_CLOCK_NAME_ENUM_ARRAY                                                            \
    {                                                                                                \
        kCLOCK_OutputPllUsb1, kCLOCK_OutputPllSys, kCLOCK_OutputPllVideo, kCLOCK_OutputSemcClk,      \
            kCLOCK_OutputLcdifPixClk, kCLOCK_OutputAhbClk, kCLOCK_OutputIpgClk, kCLOCK_OutputPerClk, \
            kCLOCK_OutputCkilSyncClk, kCLOCK_OutputPll4MainClk,                                      \
    }

#define APP_OUTPUT2_CLOCK_NAME_ARRAY                                                                         \
    {                                                                                                        \
        "Usdhc1 Clock Root", "LPI2C Clock Root", "CSI Clock Root", "OSC Clock", "Usdhc2 Clock Root",         \
            "SAI1 Clock Root", "SAI2 Clock Root", "SAI3 Clock Root(Shared with ADC1 and ADC2 alt_clk root)", \
            "Can Clock Root(FlexCAN, shared with CANFD)", "Flexspi Clock Root", "UART Clock Root",           \
            "Spdif0 Clock Root",                                                                             \
    }
#define APP_OUTPUT2_CLOCK_NAME_ENUM_ARRAY                                                               \
    {                                                                                                   \
        kCLOCK_OutputUsdhc1Clk, kCLOCK_OutputLpi2cClk, kCLOCK_OutputCsiClk, kCLOCK_OutputOscClk,        \
            kCLOCK_OutputUsdhc2Clk, kCLOCK_OutputSai1Clk, kCLOCK_OutputSai2Clk, kCLOCK_OutputSai3Clk,   \
            kCLOCK_OutputCanClk, kCLOCK_OutputFlexspiClk, kCLOCK_OutputUartClk, kCLOCK_OutputSpdif0Clk, \
    }
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
