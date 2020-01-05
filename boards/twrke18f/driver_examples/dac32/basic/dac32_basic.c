/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_dac32.h"

#include "fsl_common.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_DAC32_BASEADDR DAC0
#define DEMO_DAC32_IRQ_ID DAC0_IRQn
#define DEMO_DAC32_IRQ_HANDLER_FUNC DAC0_IRQHandler

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    uint8_t index;
    dac32_config_t dac32ConfigStruct;
    uint32_t dacValue;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("\r\nDAC32 basic Example.\r\n");

    /* Configure the DAC. */
    DAC32_GetDefaultConfig(&dac32ConfigStruct);
    DAC32_Init(DEMO_DAC32_BASEADDR, &dac32ConfigStruct);
    DAC32_Enable(DEMO_DAC32_BASEADDR, true);
    DAC32_SetBufferReadPointer(DEMO_DAC32_BASEADDR, 0U); /* Make sure the read pointer to the start. */
                                                         /*
                                                          * The buffer is not enabled, so the read pointer can not move automatically. However, the buffer's read pointer
                                                          * and itemss can be written manually by user.
                                                          */
    DAC32_EnableBufferOutput(DEMO_DAC32_BASEADDR, true);

    while (1)
    {
        index    = 0;
        dacValue = 0;
        PRINTF("\r\nPlease input a value (0 - 4095) to output with DAC: ");
        while (index != 0x0D)
        {
            index = GETCHAR();
            if ((index >= '0') && (index <= '9'))
            {
                PUTCHAR(index);
                dacValue = dacValue * 10 + (index - 0x30U);
            }
        }
        PRINTF("\r\nInput value is %d\r\n", dacValue);
        if (dacValue > 0xFFFU)
        {
            PRINTF("Your value is output of range.\r\n");
            continue;
        }
        DAC32_SetBufferValue(DEMO_DAC32_BASEADDR, 0U, dacValue);
        PRINTF("DAC out: %d\r\n", dacValue);
        /*
         * The value in the first item would be converted. User can measure the output voltage from DAC_OUTx pin.
         */
    }
}
