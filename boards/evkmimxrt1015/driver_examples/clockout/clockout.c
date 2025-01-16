/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

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

    BOARD_InitHardware();

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
