/*
 * Copyright 2020, 2025 NXP
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
static uint32_t APP_ConvertNumberStringToIntValue(uint32_t MAXValue)
{
    char ch = 0U;
    char pre_ch = 0U;
    uint32_t value = 0;
    do
        {
            ch = GETCHAR();
            if (ch >= '0' && ch <= '9')
            {
                value = value * 10U + ch - '0';
                pre_ch = ch;
            }
            else if (ch == '\r')
            {
                pre_ch = ch;
                continue;
            }
            else if (ch == '\n')
            {
                if (pre_ch == '\r')
                {
                   break;
                }
                assert(false);
            }
            else
            {
                PRINTF("%c\r\nWrong value!\r\n", ch);
                assert(false);
            }
        } while (value != 0U && value <= MAXValue);

    assert(value <= MAXValue);
    return value;
}

int main(void)
{
    clock_output1_selection_t clko1Selection;
    clock_output2_selection_t clko2Selection;
    uint32_t divider;

    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    PRINTF("\r\nClock Output Driver Example.\r\n");

    while (1)
    {
        clko1Selection = APP_GetClockOutput1Selection();
        PRINTF("\r\nPlease set the clock divider, range from 1 to %d.\r\n", EXAMPLE_CLKO1_MAX_DIVIDER_VALUE);
        divider = APP_ConvertNumberStringToIntValue(EXAMPLE_CLKO1_MAX_DIVIDER_VALUE);
        PRINTF("%d\r\n", divider);
#if defined(CLOCK_DIVIDER_NEED_ATCUAL_VALUE) || (CLOCK_DIVIDER_NEED_ATCUAL_VALUE == 1)
        CLOCK_SetClockOutput1(clko1Selection, (clock_output_divider_t)(divider));
#else
        CLOCK_SetClockOutput1(clko1Selection, (clock_output_divider_t)(divider - 1U));
#endif
        PRINTF("\r\nTheoretically, the output frequency of CLKO1 is %ld Hz.\r\n", CLOCK_GetClockOutCLKO1Freq());
        clko2Selection = APP_GetClockOutput2Selection();
        PRINTF("\r\nPlease set the clock divider, range from 1 to %d.\r\n", EXAMPLE_CLKO2_MAX_DIVIDER_VALUE);
        divider = APP_ConvertNumberStringToIntValue(EXAMPLE_CLKO2_MAX_DIVIDER_VALUE);
        PRINTF("%d\r\n", divider);
#if defined(CLOCK_DIVIDER_NEED_ATCUAL_VALUE) || (CLOCK_DIVIDER_NEED_ATCUAL_VALUE == 1)
        CLOCK_SetClockOutput2(clko2Selection, (clock_output_divider_t)(divider));
#else
        CLOCK_SetClockOutput2(clko2Selection, (clock_output_divider_t)(divider - 1U));
#endif
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
    uint8_t outputClockIndex;
#ifdef kCLOCK_DisableClockOutput1
    clock_output1_selection_t output1Selection = kCLOCK_DisableClockOutput1;
#else
    clock_output1_selection_t output1Selection = (clock_output1_selection_t)0;
#endif
    PRINTF("Please Choose one clock to be outputted on CLKO1.\r\n");

    for (i = 0U; i < ARRAY_SIZE(output1ClockName); i++)
    {
        PRINTF("\t%d -- %s.\r\n", (i + 1), output1ClockName[i]);
    }

    outputClockIndex = APP_ConvertNumberStringToIntValue(ARRAY_SIZE(output1ClockName));
    PRINTF("%d\r\n", outputClockIndex);

    outputClockIndex = (clock_output1_selection_t)(outputClockIndex - 1U);

    if (outputClockIndex < ARRAY_SIZE(output1ClockSelection))
    {
        output1Selection = output1ClockSelection[outputClockIndex];
    }
    else
    {
        PRINTF("\r\nWrong value!\r\n");
        assert(false);
    }

    return output1Selection;
}

static clock_output2_selection_t APP_GetClockOutput2Selection(void)
{
    char *output2ClockName[]                          = APP_OUTPUT2_CLOCK_NAME_ARRAY;
    clock_output2_selection_t output2ClockSelection[] = APP_OUTPUT2_CLOCK_NAME_ENUM_ARRAY;
    uint8_t i                                         = 0U;
    uint8_t outputClockIndex;
#ifdef kCLOCK_DisableClockOutput2
    clock_output2_selection_t output2Selection = kCLOCK_DisableClockOutput2;
#else
    clock_output2_selection_t output2Selection = (clock_output2_selection_t)0;
#endif

    PRINTF("Please Choose one clock to be outputted on CLKO2.\r\n");

    for (i = 0U; i < ARRAY_SIZE(output2ClockName); i++)
    {
        PRINTF("\t%d -- %s.\r\n", (i + 1), output2ClockName[i]);
    }

    outputClockIndex = APP_ConvertNumberStringToIntValue(ARRAY_SIZE(output2ClockName));
    PRINTF("%d\r\n", outputClockIndex);

    outputClockIndex = (clock_output2_selection_t)(outputClockIndex - 1U);

    if (outputClockIndex < ARRAY_SIZE(output2ClockSelection))
    {
        output2Selection = output2ClockSelection[outputClockIndex];
    }
    else
    {
        PRINTF("\r\nWrong value!\r\n");
        assert(false);
    }

    return output2Selection;
}
