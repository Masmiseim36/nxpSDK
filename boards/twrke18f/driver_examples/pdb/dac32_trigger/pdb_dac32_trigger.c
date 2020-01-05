/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_pdb.h"
#include "fsl_dac32.h"
#include "fsl_trgmux.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_PDB_BASE PDB0
#define DEMO_PDB_IRQ_ID PDB0_IRQn
#define DEMO_PDB_IRQ_HANDLER PDB0_IRQHandler
#define DEMO_PDB_DELAY_VALUE 1000U
#define DEMO_PDB_MODULUS_VALUE 1000U
#define DEMO_PDB_DAC_INTERVAL_VALUE 800U
#define DEMO_PDB_DAC_CHANNEL kPDB_DACTriggerChannel0

#define DEMO_DAC32_BASE DAC0
#define DEMO_DAC32_IRQ_ID DAC0_IRQn
#define DEMO_DAC32_IRQ_HANDLER DAC0_IRQHandler
#define DEMO_DAC32_USED_BUFFER_SIZE (DAC_DAT_COUNT * 2U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Initialize the DAC.
 */
static void DEMO_InitPDB_DAC(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t g_PdbDelayInterruptCounter;
volatile uint32_t g_PdbDelayInterruptFlag;
volatile bool g_Dac32BufferWatermarkInterruptFlag;
volatile bool g_Dac32BufferReadPointerTopPositionInterruptFlag;
volatile bool g_Dac32BufferReadPointerBottomPositionInterruptFlag;

/*******************************************************************************
 * Code
 ******************************************************************************/
void static DEMO_InitPDB_DAC(void)
{
    dac32_config_t dac32ConfigStruct;
    dac32_buffer_config_t dac32BufferConfigStruct;
    uint8_t index = 0U;
    uint16_t dac32Value;
    uint32_t mask;

    /* Configure the DAC32. */
    DAC32_GetDefaultConfig(&dac32ConfigStruct);
    DAC32_Init(DEMO_DAC32_BASE, &dac32ConfigStruct);
    DAC32_Enable(DEMO_DAC32_BASE, true);

    /* Configure the DAC32 buffer. */
    DAC32_GetDefaultBufferConfig(&dac32BufferConfigStruct);
    dac32BufferConfigStruct.triggerMode = kDAC32_BufferTriggerByHardwareMode;
    DAC32_SetBufferConfig(DEMO_DAC32_BASE, &dac32BufferConfigStruct);
    DAC32_EnableBufferOutput(DEMO_DAC32_BASE, true);

    /* Make sure the read pointer to the start. */
    DAC32_SetBufferReadPointer(DEMO_DAC32_BASE, 0U);
    dac32Value = 0U;
    for (index = 0U; index < DEMO_DAC32_USED_BUFFER_SIZE; index++)
    {
        DAC32_SetBufferValue(DEMO_DAC32_BASE, index, dac32Value);
        dac32Value += (0xFFFU / DEMO_DAC32_USED_BUFFER_SIZE);
    }

    /* Clear flags. */
    g_Dac32BufferWatermarkInterruptFlag                 = false;
    g_Dac32BufferReadPointerTopPositionInterruptFlag    = false;
    g_Dac32BufferReadPointerBottomPositionInterruptFlag = false;

    /* Enable interrupts. */
    mask = kDAC32_BufferWatermarkInterruptEnable | kDAC32_BufferReadPointerTopInterruptEnable |
           kDAC32_BufferReadPointerBottomInterruptEnable;
    DAC32_EnableBuffer(DEMO_DAC32_BASE, true);
    DAC32_EnableBufferInterrupts(DEMO_DAC32_BASE, mask);

    PRINTF("\r\nDAC32 Buffer Information\r\n");
    PRINTF("\t  Buffer index max  : %d\r\n", dac32BufferConfigStruct.upperLimit);
    PRINTF("\t  Buffer watermark  : %d\r\n",
           dac32BufferConfigStruct.upperLimit - (uint8_t)(dac32BufferConfigStruct.watermark) - 1U);
}

/*!
 * @brief ISR for PDB interrupt function
 */
void DEMO_PDB_IRQ_HANDLER(void)
{
    PDB_ClearStatusFlags(DEMO_PDB_BASE, kPDB_DelayEventFlag);
    g_PdbDelayInterruptCounter++;
    g_PdbDelayInterruptFlag = true;
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

/*!
 * @brief ISR for DAC interrutp function
 */
void DEMO_DAC32_IRQ_HANDLER(void)
{
    uint32_t flags = DAC32_GetBufferStatusFlags(DEMO_DAC32_BASE);

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
    /* Clear flags. */
    DAC32_ClearBufferStatusFlags(DEMO_DAC32_BASE, flags);
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
    pdb_config_t pdbConfigStruct;
    pdb_dac_trigger_config_t pdbDacTriggerConfigStruct;
    uint8_t index = 0U;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Configure the TRGMUX to make DAC can capture the trigger from PDB.
     * kTRGMUX_SourcePdb0Dac is trigger for the DAC interval trigger.
     */
    TRGMUX_SetTriggerSource(TRGMUX1, kTRGMUX_Ctrl0, kTRGMUX_TriggerInput0, kTRGMUX_SourcePdb0Dac);
    TRGMUX_SetTriggerSource(TRGMUX0, kTRGMUX_Dac0, kTRGMUX_TriggerInput0, kTRGMUX_SourceTrgmux1Output0);
    EnableIRQ(DEMO_PDB_IRQ_ID);
    EnableIRQ(DEMO_DAC32_IRQ_ID);

    PRINTF("\r\nPDB DAC32 trigger Example.\r\n");

    /* Configure the PDB counter. */
    PDB_GetDefaultConfig(&pdbConfigStruct);
    PDB_Init(DEMO_PDB_BASE, &pdbConfigStruct);

    /* Configure the delay interrupt. */
    PDB_SetModulusValue(DEMO_PDB_BASE, DEMO_PDB_MODULUS_VALUE);
    PDB_SetCounterDelayValue(DEMO_PDB_BASE, DEMO_PDB_DELAY_VALUE);
    PDB_EnableInterrupts(DEMO_PDB_BASE, kPDB_DelayInterruptEnable);

    /* Configure the DAC32 trigger. */
    pdbDacTriggerConfigStruct.enableExternalTriggerInput = false;
    pdbDacTriggerConfigStruct.enableIntervalTrigger      = true;
    PDB_SetDACTriggerConfig(DEMO_PDB_BASE, DEMO_PDB_DAC_CHANNEL, &pdbDacTriggerConfigStruct);
    PDB_SetDACTriggerIntervalValue(DEMO_PDB_BASE, DEMO_PDB_DAC_CHANNEL, DEMO_PDB_DAC_INTERVAL_VALUE);

    PDB_DoLoadValues(DEMO_PDB_BASE);

    /* Configure the DAC. */
    DEMO_InitPDB_DAC();

    PRINTF("Type any key into terminal to trigger the DAC buffer through PDB ...\r\n");

    g_PdbDelayInterruptCounter = 0U;

    while (1)
    {
        if (0U == index)
        {
            PRINTF("\r\n");
        }
        PRINTF("DAC32 Buffer Index %2d: ", index);
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

        /* Trigger the PDB and then trigger the buffer, move the pointer. */
        GETCHAR();
        g_PdbDelayInterruptFlag = false;
        PDB_DoSoftwareTrigger(DEMO_PDB_BASE);
        while (!g_PdbDelayInterruptFlag)
        {
        }
        index++;
        if (index >= DEMO_DAC32_USED_BUFFER_SIZE)
        {
            index = 0U;
        }
    }
}
