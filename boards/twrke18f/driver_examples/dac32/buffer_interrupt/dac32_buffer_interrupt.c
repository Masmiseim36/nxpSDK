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
#define DEMO_DAC32_USED_BUFFER_SIZE 16U

#define DEMO_DAC32_REPEAT_COUNT_MAX 10U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_Dac32BufferWatermarkInterruptFlag;
volatile bool g_Dac32BufferReadPointerTopPositionInterruptFlag;
volatile bool g_Dac32BufferReadPointerBottomPositionInterruptFlag;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief IRQ function for DAC32 buffer interrupt
 */
void DEMO_DAC32_IRQ_HANDLER_FUNC(void)
{
    uint32_t flags = DAC32_GetBufferStatusFlags(DEMO_DAC32_BASEADDR);

    if (kDAC32_BufferWatermarkFlag == (kDAC32_BufferWatermarkFlag & flags))
    {
        g_Dac32BufferWatermarkInterruptFlag = true;
    }
    if (kDAC32_BufferReadPointerTopPositionFlag == (kDAC32_BufferReadPointerTopPositionFlag & flags))
    {
        g_Dac32BufferReadPointerTopPositionInterruptFlag = true;
    }
    if (kDAC32_BufferReadPointerBottomPositionFlag == (kDAC32_BufferReadPointerBottomPositionFlag & flags))
    {
        g_Dac32BufferReadPointerBottomPositionInterruptFlag = true;
    }
    DAC32_ClearBufferStatusFlags(DEMO_DAC32_BASEADDR, flags); /* Clear flags. */
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

/*!
 * @brief Main function
 */
int main(void)
{
    dac32_config_t dac32ConfigStruct;
    dac32_buffer_config_t dac32BufferConfigStruct;
    uint32_t index;
    uint16_t dacValue;
    uint32_t mask;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    EnableIRQ(DEMO_DAC32_IRQ_ID);

    PRINTF("\r\nDAC32 buffer interrupt Example.\r\n");

    /* Configure the DAC32. */
    DAC32_GetDefaultConfig(&dac32ConfigStruct);
    DAC32_Init(DEMO_DAC32_BASEADDR, &dac32ConfigStruct);
    DAC32_Enable(DEMO_DAC32_BASEADDR, true);

    /* Configure the DAC buffer. */
    DAC32_GetDefaultBufferConfig(&dac32BufferConfigStruct);
    DAC32_SetBufferConfig(DEMO_DAC32_BASEADDR, &dac32BufferConfigStruct);
    DAC32_SetBufferReadPointer(DEMO_DAC32_BASEADDR, 0U); /* Make sure the read pointer to the start. */
    dacValue = 0U;
    for (index = 0U; index < DEMO_DAC32_USED_BUFFER_SIZE; index++)
    {
        DAC32_SetBufferValue(DEMO_DAC32_BASEADDR, index, dacValue);
        dacValue += (0xFFFU / DEMO_DAC32_USED_BUFFER_SIZE);
    }
    /* Clear flags. */
    g_Dac32BufferWatermarkInterruptFlag                 = false;
    g_Dac32BufferReadPointerTopPositionInterruptFlag    = false;
    g_Dac32BufferReadPointerBottomPositionInterruptFlag = false;

    /* Enable interrupts. */
    mask = kDAC32_BufferWatermarkInterruptEnable | kDAC32_BufferReadPointerTopInterruptEnable |
           kDAC32_BufferReadPointerBottomInterruptEnable;
    DAC32_EnableBuffer(DEMO_DAC32_BASEADDR, true);
    DAC32_EnableBufferInterrupts(DEMO_DAC32_BASEADDR, mask);
    DAC32_EnableBufferOutput(DEMO_DAC32_BASEADDR, true);

    PRINTF("\r\nDAC32 Buffer Information\r\n");
    PRINTF("\t  Buffer index max  : %d\r\n", dac32BufferConfigStruct.upperLimit);
    PRINTF("\t  Buffer watermark  : %d\r\n",
           dac32BufferConfigStruct.upperLimit - (uint32_t)(dac32BufferConfigStruct.watermark) - 1U);
    PRINTF("Press any key in terminal to trigger the buffer ...\r\n");
    index = 0U;
    while (1)
    {
        if (0U == index)
        {
            PRINTF("\r\n");
        }
        PRINTF("Buffer Index %2d: ", index);
        if (g_Dac32BufferWatermarkInterruptFlag)
        {
            PRINTF("WatermarkEvent \t");
            g_Dac32BufferWatermarkInterruptFlag = false;
        }
        if (g_Dac32BufferReadPointerTopPositionInterruptFlag)
        {
            PRINTF("ReadPointerTopPositionEvent \t");
            g_Dac32BufferReadPointerTopPositionInterruptFlag = false;
        }
        if (g_Dac32BufferReadPointerBottomPositionInterruptFlag)
        {
            PRINTF("ReadPointerBottomPositionEvent \t");
            g_Dac32BufferReadPointerBottomPositionInterruptFlag = false;
        }
        PRINTF("\r\n");

        /* Trigger the buffer and move the pointer. */
        GETCHAR();
        DAC32_DoSoftwareTriggerBuffer(DEMO_DAC32_BASEADDR);
        index++;
        if (index >= DEMO_DAC32_USED_BUFFER_SIZE)
        {
            index = 0U;
        }
    }
}
