/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_pit.h"
#include "fsl_cadc.h"
#include "fsl_dac.h"
#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_PIT_BASEADDR PIT
#define PIT_SOURCE_CLOCK CLOCK_GetFreq(BUS_CLK)
#define PIT_CHANNEL kPIT_Chnl_0

#define DEMO_CADC_BASEADDR ADC
#define DEMO_ADC_IRQ_ID ADCA_IRQn
#define DEMO_ADC_IRQ_HANDLER ADCA_IRQHandler
#define DEMO_CADC_CHANNEL 4U

#define DEMO_DAC_BASEADDR DAC0

#define SYSTICK_CLK SYS_CLK

#define INPUT_SIGNAL_FREQ 10
#define NR_SAMPLES 100
#define CHART_ROWS 30U  /*!< chart row for sampled data */
#define CHART_COLS 100U /*!< chart column for sampled data */

#define ADC_12BIT_MAXVALUE (0x1000U)
#define RATIO (ADC_12BIT_MAXVALUE / CHART_ROWS)
#define CORE_CLK_FREQ CLOCK_GetFreq(SYSTICK_CLK)
typedef struct sparse_node
{
    struct sparse_node *next; /* Next node */
    uint8_t value;            /* The sample index */

} sparse_node_t, *sparse_node_ptr;
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* Reset data linker list */
static void SparseReset(void);
/* Insert value to linker list */
static void SparseInsert(uint32_t index, uint8_t value);
/* Configuration and the trigger source of PIT */
static void InitTriggerSource(void);
/* Initialize the ADCx for HW trigger */
static void Init_ADC(void);
/* Configuration DAC - Use DAC fifo to generate sinewave on DACx_OUT */
static void DAC_genWave(void);
/* Config Systick timer, Use HW timer of systick to do SW trigger of DAC */
static void Systick_Configuration(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile bool gAdcDone = false;
static sparse_node_ptr gChartHead[CHART_ROWS]; /* Sparse matrix head */
static sparse_node_t gChartNodes[NR_SAMPLES];  /* Sparse matrix nodes */
static uint32_t gFreeNode        = 0;          /* Free node slot index for gChartNodes[] */
static volatile uint8_t gCurChan = 0;

/* DAC buffer output data to create sine wave */
static uint16_t dacBuf[] = {0U,    39U,   156U,  345U,  600U,  910U,  1264U, 1648U,
                            2048U, 2448U, 2832U, 3186U, 3496U, 3751U, 3940U, 4056U};

/*******************************************************************************
 * Code
 ******************************************************************************/
static void SparseReset(void)
{
    memset(gChartHead, 0, sizeof(gChartHead));
    memset(gChartNodes, 0, sizeof(gChartNodes));
    gFreeNode = 0;
}

static void SparseInsert(uint32_t index, uint8_t value)
{
    assert(gFreeNode < NR_SAMPLES);

    sparse_node_ptr p = gChartHead[index];

    if (!p)
    {
        gChartHead[index]        = &gChartNodes[gFreeNode++];
        gChartHead[index]->value = value;
    }
    else
    {
        while (p->next != NULL)
        {
            p = p->next;
        }
        p->next        = &gChartNodes[gFreeNode++];
        p->next->value = value;
    }
}

void ADCA_IRQHandler(void)
{
    if (kCADC_ConverterAEndOfScanFlag & CADC_GetStatusFlags(DEMO_CADC_BASEADDR))
    {
        CADC_ClearStatusFlags(DEMO_CADC_BASEADDR, kCADC_ConverterAEndOfScanFlag);
    }
    gAdcDone = true;
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

static void InitTriggerSource(void)
{
    uint64_t frepUs = 0U;
    pit_config_t pitConfig;

    PIT_GetDefaultConfig(&pitConfig);

    /* Initialize the PIT */
    PIT_Init(DEMO_PIT_BASEADDR, &pitConfig);

    frepUs = 1000000U / (INPUT_SIGNAL_FREQ * NR_SAMPLES) * 2;
    frepUs = ((frepUs * PIT_SOURCE_CLOCK) / 1000000U);

    PIT_SetTimerPeriod(DEMO_PIT_BASEADDR, PIT_CHANNEL, frepUs - 1);
    PIT_StartTimer(DEMO_PIT_BASEADDR, PIT_CHANNEL);
    SIM->SOPT7 |= (SIM_SOPT7_ADCAALTTRGEN(0x2U) | SIM_SOPT7_ADCATRGSEL(0x4U));
}

static void Init_ADC(void)
{
    cadc_config_t cadcConfigStruct;
    cadc_converter_config_t cadcConverterConfigStruct;
    cadc_sample_config_t cadcSampleConfigStruct;

    /* Configure the CADC */
    cadcConfigStruct.dualConverterScanMode  = kCADC_DualConverterWorkAsTriggeredSequential;
    cadcConfigStruct.powerUpDelay           = 0x2AU;
    cadcConfigStruct.enableSimultaneousMode = false;
    CADC_Init(DEMO_CADC_BASEADDR, &cadcConfigStruct);
    CADC_EnableInterrupts(DEMO_CADC_BASEADDR, kCADC_ZeroCrossingInterruptEnable | kCADC_LowLimitInterruptEnable |
                                                  kCADC_HighLimitInterruptEnable);

    /* Trigger channel */
    CADC_EnableConverterDMA(DEMO_CADC_BASEADDR, kCADC_ConverterA, false);
    CADC_EnableConverter(DEMO_CADC_BASEADDR, kCADC_ConverterA, true);
    CADC_EnableConverterSyncInput(DEMO_CADC_BASEADDR, kCADC_ConverterA, true);
    CADC_EnableInterrupts(DEMO_CADC_BASEADDR, kCADC_ConverterAEndOfScanInterruptEnable);

    CADC_GetDefaultConverterConfig(&cadcConverterConfigStruct);
    cadcConverterConfigStruct.clockDivisor = 0xAU;
    cadcConverterConfigStruct.speedMode    = kCADC_SpeedMode3;
    CADC_SetConverterConfig(DEMO_CADC_BASEADDR, kCADC_ConverterA, &cadcConverterConfigStruct);
    CADC_EnableConverterPower(DEMO_CADC_BASEADDR, kCADC_ConverterA, true);
    CADC_EnableConverterPower(DEMO_CADC_BASEADDR, kCADC_ConverterB, false);

    /* Configure slot in conversion sequence. */
    /* Common setting. */
    cadcSampleConfigStruct.channelNumber          = DEMO_CADC_CHANNEL;
    cadcSampleConfigStruct.channelGain            = kCADC_ChannelGainx1;
    cadcSampleConfigStruct.enableDifferentialPair = 0U;
    cadcSampleConfigStruct.zeroCrossingMode       = kCADC_ZeroCorssingDisabled;
    cadcSampleConfigStruct.lowLimitValue          = 0U;
    cadcSampleConfigStruct.highLimitValue         = 0xFFFFU;
    cadcSampleConfigStruct.offsetValue            = 0U;
    cadcSampleConfigStruct.enableWaitSync         = false;
    CADC_SetSampleConfig(DEMO_CADC_BASEADDR, 0U, &cadcSampleConfigStruct);
    CADC_EnableSample(DEMO_CADC_BASEADDR, 0U, false);
    DEMO_CADC_BASEADDR->SDIS |= (1U << 1U);
    CADC_SetSampleConfig(DEMO_CADC_BASEADDR, 1U, &cadcSampleConfigStruct);
    EnableIRQ(ADCA_IRQn);
}

static void Systick_Configuration(void)
{
    uint8_t buffLen       = 0;
    uint32_t period       = 0;
    uint64_t clockSystick = 0;

    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;

    buffLen       = sizeof(dacBuf) / sizeof(dacBuf[0]);
    period        = 1000000U / (INPUT_SIGNAL_FREQ * (2 * buffLen - 2));
    clockSystick  = CORE_CLK_FREQ;
    clockSystick  = (clockSystick * period) / 1000000U;
    SysTick->LOAD = clockSystick - 1;

    /* A write of any value to current value register clears the field to 0, and also clears the SYST_CSR COUNTFLAG bit
     * to 0. */
    SysTick->VAL = 0U;

    /* Run timer and enable interrupt */
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk;
}

static void DAC_genWave(void)
{
    uint8_t buffLen = 0U;
    uint8_t index   = 0U;
    dac_config_t dacConfigStruct;
    dac_buffer_config_t dacBufferConfigStruct;

    buffLen = sizeof(dacBuf) / sizeof(dacBuf[0]);

    DAC_GetDefaultConfig(&dacConfigStruct);
    DAC_Init(DEMO_DAC_BASEADDR, &dacConfigStruct);
    DAC_Enable(DEMO_DAC_BASEADDR, true);
    DAC_ClearBufferStatusFlags(DEMO_DAC_BASEADDR, DAC_SR_DACBFRPTF_MASK);

    /* Configure the DAC buffer. */
    DAC_GetDefaultBufferConfig(&dacBufferConfigStruct);

    dacBufferConfigStruct.watermark = kDAC_BufferWatermark2Word;
    dacBufferConfigStruct.workMode  = kDAC_BufferWorkAsSwingMode;
    DAC_SetBufferConfig(DEMO_DAC_BASEADDR, &dacBufferConfigStruct);
    DAC_EnableBuffer(DEMO_DAC_BASEADDR, true);

    for (index = 0U; index < buffLen; index++)
    {
        DAC_SetBufferValue(DEMO_DAC_BASEADDR, index, dacBuf[index]);
    }
}

void SysTick_Handler(void)
{
    DAC0->C0 |= DAC_C0_DACSWTRG_MASK;
}

int main(void)
{
    uint8_t cnt     = 0U;
    int32_t row     = 0U;
    uint16_t result = 0U;
    uint32_t last   = 0U;

    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("\r\ncadc_pit_trigger demo running...\r\n\r\n");

    DAC_genWave();

    Init_ADC();

    Systick_Configuration();

    InitTriggerSource();

    SparseReset();

    for (cnt = 0; cnt < NR_SAMPLES; cnt++)
    {
        while (gAdcDone != true)
        {
        }

        result   = CADC_GetSampleResultValue(DEMO_CADC_BASEADDR, (uint16_t)gCurChan);
        result   = result >> 3U;
        gAdcDone = false;

        /* Insert the sample data into the sparse matrix */
        row = (int32_t)(result / RATIO);

        if (row >= CHART_ROWS)
        {
            row = CHART_ROWS - 1;
        }
        /* Fill one samples into sparse matrix */
        SparseInsert(row, cnt);
    }

    /* Print the chart */
    for (row = CHART_ROWS - 1; row >= 0; row--)
    {
        sparse_node_ptr p = gChartHead[row];
        last              = 0;

        while (p)
        {
            for (; last < p->value; last++)
            {
                PRINTF(" ");
            }
            PRINTF("*");
            p = p->next;
            last++;
        }
        PRINTF("\r\n");
    }

    CADC_Deinit(DEMO_CADC_BASEADDR);
    PIT_StopTimer(DEMO_PIT_BASEADDR, PIT_CHANNEL);
    PIT_Deinit(DEMO_PIT_BASEADDR);

    while (1)
    {
    }
}
