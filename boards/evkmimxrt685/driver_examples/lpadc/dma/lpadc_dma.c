/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_dma.h"
#include "fsl_inputmux.h"
#include "fsl_lpadc.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_LPADC_BASE ADC0
#define DEMO_LPADC_USER_CHANNEL 0U
#define DEMO_LPADC_USER_CMDID 1U /* The available command number are 1-15 */
#define DEMO_LPADC_RESFIFO_REG_ADDR (uint32_t)(&(ADC0->RESFIFO))

#define DEMO_DMA_BASE DMA0
#define DEMO_DMA_ADC_CHANNEL 0U
#define DEMO_DMA_TRANSFER_TYPE kDMA_MemoryToMemory
#define DEMO_DMA_HARDWARE_TRIGGER true
#define DEMO_DMA_ADC_CONNECTION kINPUTMUX_AdcToDma0

#define DEMO_INPUTMUX_BASE INPUTMUX
/* Resfifo data shift in 12-bit mode. */
#define DEMO_LPADC_RESFIFO_SHIFT 3U
#define DMA_DESCRIPTOR_NUM 2U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void ADC_Configuration(void);
static void DMA_Configuration(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
lpadc_conv_command_config_t g_LpadcCommandConfigStruct; /* Structure to configure conversion command. */
uint32_t g_AdcConvResult[1];                 /* Keep the ADC conversion resulut moved from ADC data register by DMA. */
dma_handle_t g_DmaHandleStruct;              /* Handler structure for using DMA. */
volatile bool g_DmaTransferDoneFlag = false; /* Flag of DMA transfer done trigger by ADC conversion. */
/* DMA descripter table used for ping-pong mode. */
SDK_ALIGN(uint32_t s_dma_table[DMA_DESCRIPTOR_NUM * sizeof(dma_descriptor_t)], FSL_FEATURE_DMA_DESCRIPTOR_ALIGN_SIZE);
const uint32_t g_XferConfig =
    DMA_CHANNEL_XFER(true,                          /* Reload link descriptor after current exhaust, */
                     true,                          /* Clear trigger status. */
                     true,                          /* Enable interruptA. */
                     false,                         /* Not enable interruptB. */
                     sizeof(uint32_t),              /* Dma transfer width. */
                     kDMA_AddressInterleave0xWidth, /* Dma source address no interleave  */
                     kDMA_AddressInterleave0xWidth, /* Dma destination address no interleave  */
                     sizeof(uint32_t)               /* Dma transfer byte. */
    );
const uint32_t g_Lpadc12bitFullRange = 4096U;

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Software ISR for DMA transfer done. */
void DEMO_DMA_Callback(dma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
    if (true == transferDone)
    {
        g_DmaTransferDoneFlag = true;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Initialize board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    SYSCTL0->PDRUNCFG0_CLR = SYSCTL0_PDRUNCFG0_ADC_PD_MASK;
    SYSCTL0->PDRUNCFG0_CLR = SYSCTL0_PDRUNCFG0_ADC_LP_MASK;
    RESET_PeripheralReset(kADC0_RST_SHIFT_RSTn);
    CLOCK_AttachClk(kSFRO_to_ADC_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivAdcClk, 1);
    PRINTF("LPADC DMA Example\r\n");

    /* Configure peripherals. */
    DMA_Configuration();
    ADC_Configuration();

    PRINTF("ADC Full Range: %d\r\n", g_Lpadc12bitFullRange);
#if defined(FSL_FEATURE_LPADC_HAS_CMDL_CSCALE) && FSL_FEATURE_LPADC_HAS_CMDL_CSCALE
    if (kLPADC_SampleFullScale == g_LpadcCommandConfigStruct.sampleScaleMode)
    {
        PRINTF("Full channel scale (Factor of 1).\r\n");
    }
    else if (kLPADC_SamplePartScale == g_LpadcCommandConfigStruct.sampleScaleMode)
    {
        PRINTF("Divided input voltage signal. (Factor of 30/64).\r\n");
    }
#endif

    PRINTF("Please press any key to trigger the conversion.\r\n");
    while (1)
    {
        /* Get the input from terminal and trigger the converter by software. */
        GETCHAR();

        g_DmaTransferDoneFlag = false;
        LPADC_DoSoftwareTrigger(DEMO_LPADC_BASE, 1U); /* Trigger the ADC and start the conversion. */
#if !(defined(DEMO_DMA_HARDWARE_TRIGGER) && DEMO_DMA_HARDWARE_TRIGGER)
        SDK_DelayAtLeastUs(100U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY); /* Wait ADC transfer done. */
        DMA_StartTransfer(&g_DmaHandleStruct); /* Enable the DMA every time for each transfer. */
#endif                                         /* DEMO_DMA_HARDWARE_TRIGGER */
        /* Wait for the converter & transfer to be done. */
        while (false == g_DmaTransferDoneFlag)
        {
        }
        PRINTF("Adc conversion word : 0x%X\r\n", g_AdcConvResult[0]);
        PRINTF("ADC conversion value: %d\r\n",
               ((uint16_t)(g_AdcConvResult[0] & ADC_RESFIFO_D_MASK) >> DEMO_LPADC_RESFIFO_SHIFT));
    }
}

static void ADC_Configuration(void)
{
    lpadc_config_t lpadcConfigStruct;
    lpadc_conv_trigger_config_t lpadcTriggerConfigStruct;

    /* Configure ADC. */
    LPADC_GetDefaultConfig(&lpadcConfigStruct);
    lpadcConfigStruct.enableAnalogPreliminary = true;
#if defined(DEMO_LPADC_VREF_SOURCE)
    lpadcConfigStruct.referenceVoltageSource = DEMO_LPADC_VREF_SOURCE;
#endif /* DEMO_LPADC_VREF_SOURCE */
#if defined(FSL_FEATURE_LPADC_HAS_CTRL_CAL_AVGS) && FSL_FEATURE_LPADC_HAS_CTRL_CAL_AVGS
    lpadcConfigStruct.conversionAverageMode = kLPADC_ConversionAverage128;
#endif /* FSL_FEATURE_LPADC_HAS_CTRL_CAL_AVGS */
    LPADC_Init(DEMO_LPADC_BASE, &lpadcConfigStruct);

#if defined(FSL_FEATURE_LPADC_HAS_CTRL_CALOFS) && FSL_FEATURE_LPADC_HAS_CTRL_CALOFS
#if defined(FSL_FEATURE_LPADC_HAS_OFSTRIM) && FSL_FEATURE_LPADC_HAS_OFSTRIM
    /* Request offset calibration. */
    if (true == DEMO_LPADC_DO_OFFSET_CALIBRATION)
    {
        LPADC_DoOffsetCalibration(DEMO_LPADC_BASE);
    }
    else
    {
        LPADC_SetOffsetValue(DEMO_LPADC_BASE, DEMO_LPADC_OFFSET_VALUE_A, DEMO_LPADC_OFFSET_VALUE_B);
    }
#endif /* FSL_FEATURE_LPADC_HAS_OFSTRIM */
    /* Request gain calibration. */
    LPADC_DoAutoCalibration(DEMO_LPADC_BASE);
#endif /* FSL_FEATURE_LPADC_HAS_CTRL_CALOFS */

    /* Set conversion CMD configuration. */
    LPADC_GetDefaultConvCommandConfig(&g_LpadcCommandConfigStruct);
    g_LpadcCommandConfigStruct.channelNumber = DEMO_LPADC_USER_CHANNEL;
    LPADC_SetConvCommandConfig(DEMO_LPADC_BASE, DEMO_LPADC_USER_CMDID, &g_LpadcCommandConfigStruct);

    /* Set trigger configuration. */
    LPADC_GetDefaultConvTriggerConfig(&lpadcTriggerConfigStruct);
    lpadcTriggerConfigStruct.targetCommandId       = DEMO_LPADC_USER_CMDID;
    lpadcTriggerConfigStruct.enableHardwareTrigger = false;
    LPADC_SetConvTriggerConfig(DEMO_LPADC_BASE, 0U, &lpadcTriggerConfigStruct); /* Configurate the trigger0. */

    /* DMA request enabled. */
#if (defined(FSL_FEATURE_LPADC_FIFO_COUNT) && (FSL_FEATURE_LPADC_FIFO_COUNT == 2U))
    LPADC_EnableFIFO0WatermarkDMA(DEMO_LPADC_BASE, true);
#else
    LPADC_EnableFIFOWatermarkDMA(DEMO_LPADC_BASE, true);
#endif /* FSL_FEATURE_LPADC_FIFO_COUNT */
}

static void DMA_Configuration(void)
{
    dma_channel_config_t dmaChannelConfigStruct;

#if defined(DEMO_DMA_HARDWARE_TRIGGER) && DEMO_DMA_HARDWARE_TRIGGER
    dma_channel_trigger_t dmaChannelTriggerStruct;

    /* Configure INPUTMUX. */
    INPUTMUX_Init(DEMO_INPUTMUX_BASE);
    INPUTMUX_AttachSignal(DEMO_INPUTMUX_BASE, DEMO_DMA_ADC_CHANNEL, DEMO_DMA_ADC_CONNECTION);
#endif /* DEMO_DMA_HARDWARE_TRIGGER */

    /* Configure DMA. */
    DMA_Init(DEMO_DMA_BASE);
    DMA_EnableChannel(DEMO_DMA_BASE, DEMO_DMA_ADC_CHANNEL);
    DMA_CreateHandle(&g_DmaHandleStruct, DEMO_DMA_BASE, DEMO_DMA_ADC_CHANNEL);
    DMA_SetCallback(&g_DmaHandleStruct, DEMO_DMA_Callback, NULL);

#if defined(DEMO_DMA_HARDWARE_TRIGGER) && DEMO_DMA_HARDWARE_TRIGGER
    dmaChannelTriggerStruct.burst = kDMA_EdgeBurstTransfer1;
    dmaChannelTriggerStruct.type  = kDMA_RisingEdgeTrigger;
    dmaChannelTriggerStruct.wrap  = kDMA_NoWrap;
#endif /* DEMO_DMA_HARDWARE_TRIGGER */

    /* Prepare and submit the transfer. */
    DMA_PrepareChannelTransfer(&dmaChannelConfigStruct,             /* DMA channel transfer configuration structure. */
                               (void *)DEMO_LPADC_RESFIFO_REG_ADDR, /* DMA transfer source address. */
                               (void *)g_AdcConvResult,             /* DMA transfer destination address. */
                               g_XferConfig,                        /* Xfer configuration */
                               DEMO_DMA_TRANSFER_TYPE,              /* DMA transfer type. */
#if defined(DEMO_DMA_HARDWARE_TRIGGER) && DEMO_DMA_HARDWARE_TRIGGER
                               &dmaChannelTriggerStruct, /* DMA channel trigger configurations. */
#else
                               NULL, /* DMA channel trigger configurations. */
#endif                                                               /* DEMO_DMA_HARDWARE_TRIGGER */
                               (dma_descriptor_t *)&(s_dma_table[0]) /* Address of next descriptor. */
    );
    DMA_SubmitChannelTransfer(&g_DmaHandleStruct, &dmaChannelConfigStruct);

    /* Set two DMA descripters to use ping-pong mode.  */
    DMA_SetupDescriptor((dma_descriptor_t *)&(s_dma_table[0]), g_XferConfig, (void *)DEMO_LPADC_RESFIFO_REG_ADDR,
                        (void *)g_AdcConvResult, (dma_descriptor_t *)&(s_dma_table[4]));
    DMA_SetupDescriptor((dma_descriptor_t *)&(s_dma_table[4]), g_XferConfig, (void *)DEMO_LPADC_RESFIFO_REG_ADDR,
                        (void *)g_AdcConvResult, (dma_descriptor_t *)&(s_dma_table[0]));
}
