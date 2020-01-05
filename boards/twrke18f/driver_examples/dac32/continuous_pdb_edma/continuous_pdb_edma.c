/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_dac32.h"
#include "fsl_edma.h"
#include "fsl_pdb.h"
#include "fsl_dmamux.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_trgmux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_PDB_BASEADDR PDB0
#define DEMO_PDB_MODULUS_VALUE 0xFFFFU
#define DEMO_PDB_DELAY_VALUE 0U
#define DEMO_PDB_DAC_CHANNEL kPDB_DACTriggerChannel0
#define DEMO_PDB_DAC_INTERVAL_VALUE 0xFFFFU
#define DEMO_DAC32_BASEADDR DAC0
#define DEMO_DMAMUX_BASEADDR DMAMUX
#define DEMO_DMA_CHANNEL 0U
#define DEMO_DMA_DAC_SOURCE 56U
#define DEMO_DMA_BASEADDR DMA0
#define DAC_DATA_REG_ADDR 0x4003f000U
#define DEMO_DMA_IRQ_ID DMA0_IRQn
#define DEMO_DAC_USED_BUFFER_SIZE 32U
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Initialize the EDMA.
 */
static void EDMA_Configuration(void);
/*!
 * @brief Initialize the DMAMUX.
 */
static void DMAMUX_Configuration(void);
/*!
 * @brief Initialize the PDB.
 */
static void PDB_Configuration(void);
/*!
 * @brief Initialize the DAC32.
 */
static void DAC32_Configuration(void);
/*!
 * @brief Callback function for EDMA.
 */
static void Edma_Callback(edma_handle_t *handle, void *userData, bool transferDone, uint32_t tcds);
/*******************************************************************************
 * Variables
 ******************************************************************************/
edma_handle_t g_EDMA_Handle;                             /* Edma handler. */
edma_transfer_config_t g_transferConfig;                 /* Edma transfer config. */
volatile uint32_t g_index                          = 0U; /* Index of the g_dacDataArray array. */
uint16_t g_dacDataArray[DEMO_DAC_USED_BUFFER_SIZE] = {
    0U,    401U,  799U,  1188U, 1567U, 1930U, 2275U, 2598U, 2895U, 3165U, 3405U, 3611U, 3783U, 3918U, 4016U, 4075U,
    4095U, 4075U, 4016U, 3918U, 3783U, 3611U, 3405U, 3165U, 2895U, 2598U, 2275U, 1930U, 1567U, 1188U, 799U,  401U};
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    /* Initialize hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Configure the TRGMUX to make DAC can capture the trigger from PDB.
     * kTRGMUX_SourcePdb0Dac is trigger for the DAC interval trigger.
     */
    TRGMUX_SetTriggerSource(TRGMUX1, kTRGMUX_Ctrl0, kTRGMUX_TriggerInput0, kTRGMUX_SourcePdb0Dac);
    TRGMUX_SetTriggerSource(TRGMUX0, kTRGMUX_Dac0, kTRGMUX_TriggerInput0, kTRGMUX_SourceTrgmux1Output0);

    PRINTF("DAC32 CONTINUOUS PDB EDMA DEMO\r\n");

    /* Initialize DMAMUX. */
    DMAMUX_Configuration();
    /* Initialize EDMA. */
    EDMA_Configuration();
    /* Initialize the HW trigger source. */
    PDB_Configuration();
    /* Initialize DAC32. */
    DAC32_Configuration();

    /* Generate continuous trigger signal to DAC. */
    PDB_DoSoftwareTrigger(DEMO_PDB_BASEADDR);

    PRINTF("Please probe the DAC output with a oscilloscope.\r\n");
    while (1)
    {
    }
}

static void EDMA_Configuration(void)
{
    edma_config_t userConfig;

    EDMA_GetDefaultConfig(&userConfig);
    EDMA_Init(DEMO_DMA_BASEADDR, &userConfig);
    EDMA_CreateHandle(&g_EDMA_Handle, DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL);
    EDMA_SetCallback(&g_EDMA_Handle, Edma_Callback, NULL);
    EDMA_PrepareTransfer(&g_transferConfig, (void *)(g_dacDataArray + g_index), sizeof(uint16_t),
                         (void *)DAC_DATA_REG_ADDR, sizeof(uint16_t), 16 * sizeof(uint16_t), 16 * sizeof(uint16_t),
                         kEDMA_MemoryToMemory);
    EDMA_SubmitTransfer(&g_EDMA_Handle, &g_transferConfig);
    /* Enable interrupt when transfer is done. */
    EDMA_EnableChannelInterrupts(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL, kEDMA_MajorInterruptEnable);
#if defined(FSL_FEATURE_EDMA_ASYNCHRO_REQUEST_CHANNEL_COUNT) && FSL_FEATURE_EDMA_ASYNCHRO_REQUEST_CHANNEL_COUNT
    /* Enable async DMA request. */
    EDMA_EnableAsyncRequest(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL, true);
#endif /* FSL_FEATURE_EDMA_ASYNCHRO_REQUEST_CHANNEL_COUNT */
    /* Enable transfer. */
    EDMA_StartTransfer(&g_EDMA_Handle);
}

static void DMAMUX_Configuration(void)
{
    /* Configure DMAMUX. */
    DMAMUX_Init(DEMO_DMAMUX_BASEADDR);
    DMAMUX_SetSource(DEMO_DMAMUX_BASEADDR, DEMO_DMA_CHANNEL, DEMO_DMA_DAC_SOURCE); /* Map ADC source to channel 0 */
    DMAMUX_EnableChannel(DEMO_DMAMUX_BASEADDR, DEMO_DMA_CHANNEL);
}

/* Enable the trigger source of PDB. */
static void PDB_Configuration(void)
{
    pdb_config_t pdbConfigStruct;
    pdb_dac_trigger_config_t pdbDacTriggerConfigStruct;

    /*
     * pdbConfigStruct.loadValueMode = kPDB_LoadValueImmediately;
     * pdbConfigStruct.prescalerDivider = kPDB_PrescalerDivider1;
     * pdbConfigStruct.dividerMultiplicationFactor = kPDB_DividerMultiplicationFactor40;
     * pdbConfigStruct.triggerInputSource = kPDB_TriggerSoftware;
     * pdbConfigStruct.enableContinuousMode = true;
     */
    PDB_GetDefaultConfig(&pdbConfigStruct);
    pdbConfigStruct.dividerMultiplicationFactor = kPDB_DividerMultiplicationFactor1;
    pdbConfigStruct.enableContinuousMode        = true;
    PDB_Init(DEMO_PDB_BASEADDR, &pdbConfigStruct);
    PDB_EnableInterrupts(DEMO_PDB_BASEADDR, kPDB_DelayInterruptEnable);
    PDB_SetModulusValue(DEMO_PDB_BASEADDR, DEMO_PDB_MODULUS_VALUE);
    PDB_SetCounterDelayValue(DEMO_PDB_BASEADDR, DEMO_PDB_DELAY_VALUE);

    /* Set DAC trigger. */
    pdbDacTriggerConfigStruct.enableExternalTriggerInput = false;
    pdbDacTriggerConfigStruct.enableIntervalTrigger      = true;
    PDB_SetDACTriggerConfig(DEMO_PDB_BASEADDR, DEMO_PDB_DAC_CHANNEL, &pdbDacTriggerConfigStruct);
    PDB_SetDACTriggerIntervalValue(DEMO_PDB_BASEADDR, DEMO_PDB_DAC_CHANNEL, DEMO_PDB_DAC_INTERVAL_VALUE);

    /* Load PDB values. */
    PDB_DoLoadValues(DEMO_PDB_BASEADDR);
}

static void DAC32_Configuration(void)
{
    dac32_config_t dac32ConfigStruct;
    dac32_buffer_config_t dac32BufferConfigStruct;

    /* Configure the DAC32. */
    DAC32_GetDefaultConfig(&dac32ConfigStruct);
    DAC32_Init(DEMO_DAC32_BASEADDR, &dac32ConfigStruct);
    DAC32_Enable(DEMO_DAC32_BASEADDR, true);

    /* Configure the DAC buffer. */
    DAC32_GetDefaultBufferConfig(&dac32BufferConfigStruct);
    dac32BufferConfigStruct.triggerMode = kDAC32_BufferTriggerByHardwareMode;
    DAC32_SetBufferConfig(DEMO_DAC32_BASEADDR, &dac32BufferConfigStruct);
    DAC32_SetBufferReadPointer(DEMO_DAC32_BASEADDR, 0U); /* Make sure the read pointer to the start. */
    DAC32_EnableBuffer(DEMO_DAC32_BASEADDR, true);
    DAC32_EnableBufferOutput(DEMO_DAC32_BASEADDR, true);

    /* Enable DMA. */
    DAC32_EnableBufferInterrupts(DEMO_DAC32_BASEADDR, kDAC32_BufferReadPointerTopInterruptEnable);
    DAC32_EnableBufferDMA(DEMO_DAC32_BASEADDR, true);
}

static void Edma_Callback(edma_handle_t *handle, void *userData, bool transferDone, uint32_t tcds)
{
    /* Clear Edma interrupt flag. */
    EDMA_ClearChannelStatusFlags(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL, kEDMA_InterruptFlag);
    /* Setup transfer */
    g_index += 16U;
    if (g_index == DEMO_DAC_USED_BUFFER_SIZE)
    {
        g_index = 0U;
    }
    EDMA_PrepareTransfer(&g_transferConfig, (void *)(g_dacDataArray + g_index), sizeof(uint16_t),
                         (void *)DAC_DATA_REG_ADDR, sizeof(uint16_t), 16 * sizeof(uint16_t), 16 * sizeof(uint16_t),
                         kEDMA_MemoryToMemory);
    EDMA_SetTransferConfig(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL, &g_transferConfig, NULL);
    /* Enable transfer. */
    EDMA_StartTransfer(&g_EDMA_Handle);
}
