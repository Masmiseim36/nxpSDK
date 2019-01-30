/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_common.h"
#include "fsl_cadc.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"
#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_CADC_BASEADDR ADC
#define DEMO_CADC_USER_CHANNEL 4U
#define DEMO_DMAMUX_BASEADDR DMAMUX0
#define DEMO_DMA_CHANNEL 0U
#define DEMO_DMA_ADC_SOURCE 40U
#define DEMO_DMA_BASEADDR DMA0
#define CADC_RESULT_REG_ADDR 0x4005c01cU

#define DEMO_CADC_SAMPLE_COUNT 16U /* The cadc sample count. */

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
* @brief Initialize the CADC.
*/
static void CADC_Configuration(void);

/*!
 * @brief Process ADC values.
 */
static void ProcessSampleData(void);

/*!
 * @brief Callback function for EDMA.
 */
static void Edma_Callback(edma_handle_t *handle, void *userData, bool transferDone, uint32_t tcds);

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_Transfer_Done = false; /* DMA transfer completion flag. */
uint16_t g_cadcSampleDataArray[DEMO_CADC_SAMPLE_COUNT];
uint32_t g_avgADCValue = 0U; /* Average ADC value .*/
edma_handle_t g_EDMA_Handle; /* Edma handler. */
edma_transfer_config_t g_transferConfig;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    /* Initialize board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("CADC continuous edma conversion.\r\n");

    EDMA_Configuration();   /* Initialize EDMA. */
    DMAMUX_Configuration(); /* Initialize DMAMUX. */
    CADC_Configuration();   /* Initialize CADC. */

    /* Trigger the converter.
     * Trigger converter A would execute the loop sequential conversion when in
     * "kCADC_DualConverterWorkAsLoopSequential" work mode. */
    CADC_DoSoftwareTriggerConverter(DEMO_CADC_BASEADDR, kCADC_ConverterA);

    PRINTF("Press any key to get user channel's ADC value ...\r\n");

    while (1)
    {
        GETCHAR();
        g_Transfer_Done = false;
        while (!g_Transfer_Done)
        {
        }
        ProcessSampleData();
        PRINTF("ADC value: %d\r\n", g_avgADCValue >> ADC_RSLT_RSLT_SHIFT);
    }
}

static void DMAMUX_Configuration(void)
{
    /* Configure DMAMUX */
    DMAMUX_Init(DEMO_DMAMUX_BASEADDR);
    DMAMUX_SetSource(DEMO_DMAMUX_BASEADDR, DEMO_DMA_CHANNEL, DEMO_DMA_ADC_SOURCE); /* Map ADC source to channel 0 */
    DMAMUX_EnableChannel(DEMO_DMAMUX_BASEADDR, DEMO_DMA_CHANNEL);
}

static void EDMA_Configuration(void)
{
    edma_config_t userConfig;

    EDMA_GetDefaultConfig(&userConfig);
    EDMA_Init(DEMO_DMA_BASEADDR, &userConfig);
    EDMA_CreateHandle(&g_EDMA_Handle, DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL);
    EDMA_SetCallback(&g_EDMA_Handle, Edma_Callback, NULL);
    EDMA_PrepareTransfer(&g_transferConfig, (void *)CADC_RESULT_REG_ADDR, sizeof(uint16_t),
                         (void *)g_cadcSampleDataArray, sizeof(uint16_t), sizeof(uint16_t),
                         sizeof(g_cadcSampleDataArray), kEDMA_PeripheralToMemory);
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

static void CADC_Configuration(void)
{
    cadc_config_t cadcConfigStruct;
    cadc_converter_config_t cadcConverterConfigStruct;
    cadc_sample_config_t cadcSampleConfigStruct;
    uint16_t sampleMask;

    /*
     * cadcConfigStruct.dualConverterScanMode = kCADC_DualConverterWorkAsTriggeredParallel;
     * cadcConfigStruct.enableSimultaneousMode = true;
     * cadcConfigStruct.DMATriggerSoruce = kCADC_DMATriggerSourceAsEndOfScan;
     * cadcConfigStruct.idleWorkMode = kCADC_IdleKeepNormal;
     * cadcConfigStruct.powerUpDelay = 26U;
     */
    CADC_GetDefaultConfig(&cadcConfigStruct);
    cadcConfigStruct.dualConverterScanMode = kCADC_DualConverterWorkAsLoopSequential;
    cadcConfigStruct.DMATriggerSoruce = kCADC_DMATriggerSourceAsSampleReady;
    CADC_Init(DEMO_CADC_BASEADDR, &cadcConfigStruct);

    /* Configure converterA. */
    CADC_GetDefaultConverterConfig(&cadcConverterConfigStruct);
    cadcConverterConfigStruct.clockDivisor = 10U;
    CADC_SetConverterConfig(DEMO_CADC_BASEADDR, kCADC_ConverterA, &cadcConverterConfigStruct);
    /* Enable the power for converterA. */
    CADC_EnableConverterPower(DEMO_CADC_BASEADDR, kCADC_ConverterA, true);
    while (kCADC_ConverterAPowerDownFlag == (kCADC_ConverterAPowerDownFlag & CADC_GetStatusFlags(ADC)))
    {
    }
    /* Open the gate for converterA. */
    CADC_EnableConverter(DEMO_CADC_BASEADDR, kCADC_ConverterA, true);

    /* Configure the samples. */
    cadcSampleConfigStruct.channelGain = kCADC_ChannelGainx1;
    cadcSampleConfigStruct.zeroCrossingMode = kCADC_ZeroCorssingDisabled;
    cadcSampleConfigStruct.highLimitValue = 0xFFFFU;
    cadcSampleConfigStruct.lowLimitValue = 0x0U;
    cadcSampleConfigStruct.offsetValue = 0x0U;
    cadcSampleConfigStruct.enableWaitSync = false;

    /* For converter A. */
    cadcSampleConfigStruct.channelNumber = DEMO_CADC_USER_CHANNEL;
    cadcSampleConfigStruct.enableDifferentialPair = false;
    CADC_SetSampleConfig(DEMO_CADC_BASEADDR, 0U, &cadcSampleConfigStruct);

    /* Enable the sample slot. */
    sampleMask = CADC_SAMPLE_MASK(0U); /* For converter A. */
    CADC_EnableSample(DEMO_CADC_BASEADDR, sampleMask, true);
    CADC_EnableSample(DEMO_CADC_BASEADDR, (uint16_t)(~sampleMask), false); /* Disable other sample slot. */

    /* Enable the DMA for converterA. */
    CADC_EnableConverterDMA(DEMO_CADC_BASEADDR, kCADC_ConverterA, true);
}

static void ProcessSampleData(void)
{
    uint32_t i = 0U;

    g_avgADCValue = 0;
    /* Get average adc value. */
    for (i = 0; i < DEMO_CADC_SAMPLE_COUNT; i++)
    {
        g_avgADCValue += g_cadcSampleDataArray[i];
    }
    g_avgADCValue = g_avgADCValue / DEMO_CADC_SAMPLE_COUNT;

    /* Reset old value. */
    for (i = 0; i < DEMO_CADC_SAMPLE_COUNT; i++)
    {
        g_cadcSampleDataArray[i] = 0U;
    }
}

static void Edma_Callback(edma_handle_t *handle, void *userData, bool transferDone, uint32_t tcds)
{
    /* Clear Edma interrupt flag. */
    EDMA_ClearChannelStatusFlags(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL, kEDMA_InterruptFlag);
    /* Setup transfer */
    EDMA_PrepareTransfer(&g_transferConfig, (void *)CADC_RESULT_REG_ADDR, sizeof(uint16_t),
                         (void *)g_cadcSampleDataArray, sizeof(uint16_t), sizeof(uint16_t),
                         sizeof(g_cadcSampleDataArray), kEDMA_PeripheralToMemory);
    EDMA_SetTransferConfig(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL, &g_transferConfig, NULL);
    /* Enable transfer. */
    EDMA_StartTransfer(&g_EDMA_Handle);
    g_Transfer_Done = true;
}
