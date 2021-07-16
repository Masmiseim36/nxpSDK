/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

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


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static clock_output1_selection_t APP_GetClockOutput1Selection(void);
static clock_output2_selection_t APP_GetClockOutput2Selection(void);
/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    clock_output1_selection_t clko1Selection;
    clock_output2_selection_t clko2Selection;
    char chDivider;
    uint8_t divider;

    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    CLOCK_EnableClock(kCLOCK_Usdhc1);
    CLOCK_EnableClock(kCLOCK_Usdhc2);
    CLOCK_EnableClock(kCLOCK_Sai1);
    CLOCK_EnableClock(kCLOCK_Sai2);
    CLOCK_EnableClock(kCLOCK_FlexSpi);
    CLOCK_EnableClock(kCLOCK_Spdif);
    CLOCK_EnableClock(kCLOCK_Can2S);
    CLOCK_EnableClock(kCLOCK_Enet);
    CLOCK_EnableClock(kCLOCK_Lpspi1);
    CLOCK_EnableClock(kCLOCK_Trace);
    CLOCK_EnableClock(kCLOCK_Pit);
    CLOCK_EnableClock(kCLOCK_Lpi2c1);

    /* Set Spdif clock source. */
    CLOCK_SetMux(kCLOCK_SpdifMux, 0);

    PRINTF("\r\nClock Output Driver Example.\r\n");

    while (1)
    {
        clko1Selection = APP_GetClockOutput1Selection();
        PRINTF("\r\nPlease set the clock divider, range from 1 to 8.\r\n");
        chDivider = GETCHAR();
        PRINTF("%c\r\n", chDivider);
        divider = chDivider - '0';
        CLOCK_SetClockOutput1(clko1Selection, (clock_output_divider_t)(divider - 1U));
        PRINTF("\r\nTheoretically, the output frequency of CLKO1 is %ld Hz.\r\n", CLOCK_GetClockOutCLKO1Freq());
        clko2Selection = APP_GetClockOutput2Selection();
        PRINTF("\r\nPlease set the clock divider, range from 1 to 8.\r\n");
        chDivider = GETCHAR();
        PRINTF("%c\r\n", chDivider);
        divider = chDivider - '0';
        CLOCK_SetClockOutput2(clko2Selection, (clock_output_divider_t)(divider - 1U));
        PRINTF("\r\nTheoretically, the output frequency of CLKO2 is %ld Hz.\r\n", CLOCK_GetClockOutClkO2Freq());
        PRINTF("\r\nPlease press any key to continue.\r\n");
        GETCHAR();
    }
}

static clock_output1_selection_t APP_GetClockOutput1Selection(void)
{
    char *output1ClockName[]                          = APP_OUTPUT1_CLOCK_NAME_ARRAY;
    clock_output1_selection_t output1ClockSelection[] = APP_OUTPUT1_CLOCK_NAME_ENUM_ARRAY;
    uint8_t i                                         = 0U;
    char ch;
    uint8_t outputClockIndex;
    clock_output1_selection_t output1Selection = kCLOCK_DisableClockOutput1;

    PRINTF("Please Choose one clock to be outputted on CLKO1.\r\n");

    for (i = 0U; i < ARRAY_SIZE(output1ClockName); i++)
    {
        PRINTF("\t%c -- %s.\r\n", ('A' + i), output1ClockName[i]);
    }

    ch = GETCHAR();
    PRINTF("%c\r\n", ch);

    if (ch >= 'a')
    {
        ch -= 'a' - 'A';
    }
    outputClockIndex = (clock_output1_selection_t)(ch - 'A');

    if (outputClockIndex < ARRAY_SIZE(output1ClockSelection))
    {
        output1Selection = output1ClockSelection[outputClockIndex];
    }

    return output1Selection;
}

static clock_output2_selection_t APP_GetClockOutput2Selection(void)
{
    char *output2ClockName[]                          = APP_OUTPUT2_CLOCK_NAME_ARRAY;
    clock_output2_selection_t output2ClockSelection[] = APP_OUTPUT2_CLOCK_NAME_ENUM_ARRAY;
    uint8_t i                                         = 0U;
    char ch;
    uint8_t outputClockIndex;
    clock_output2_selection_t output2Selection = kCLOCK_DisableClockOutput2;

    PRINTF("Please Choose one clock to be outputted on CLKO2.\r\n");

    for (i = 0U; i < ARRAY_SIZE(output2ClockName); i++)
    {
        PRINTF("\t%c -- %s.\r\n", 'A' + i, output2ClockName[i]);
    }

    ch = GETCHAR();
    PRINTF("%c\r\n", ch);

    if (ch >= 'a')
    {
        ch -= 'a' - 'A';
    }

    outputClockIndex = (clock_output2_selection_t)(ch - 'A');

    if (outputClockIndex < ARRAY_SIZE(output2ClockSelection))
    {
        output2Selection = output2ClockSelection[outputClockIndex];
    }

    return output2Selection;
}
