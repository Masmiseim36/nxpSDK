/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_pwm.h"
#include "fsl_xbara.h"
#include "fsl_cadc.h"
#include "fsl_dac.h"
#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_PWM_BASEADDR PWMA

#define DEMO_CADC_BASEADDR ADC
#define DEMO_ADC_IRQ_ID ADCA_IRQn
#define DEMO_ADC_IRQ_HANDLER ADCA_IRQHandler
#define DEMO_CADC_CHANNEL 4U

#define DEMO_DAC_BASEADDR DAC0
#define DEMO_DAC_TRG_INTERVAL_FREQ_HZ 3000UL /* Interval freq for the change of DAC output */

#define SYSTICK_CLK CLOCK_GetFreq(SYS_CLK)

#define NR_SAMPLES 100
#define CHART_ROWS 30U  /*!< chart row for sampled data */
#define CHART_COLS 100U /*!< chart column for sampled data */

#define ADC_12BIT_MAXVALUE (0x1000U)
#define RATIO (ADC_12BIT_MAXVALUE / CHART_ROWS)
#define PWM_SRC_CLK_FREQ CLOCK_GetFreq(kCLOCK_FastPeriphClk)
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
/* Configuration and the trigger source of PWM */
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

/* ISR for ADC conversion seq. */
void DEMO_ADC_IRQ_HANDLER(void)
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
    uint16_t deadTimeVal;
    pwm_signal_param_t pwmSignal[1];
    uint32_t pwmSourceClockInHz = 0;
    uint32_t pwmFrequencyInHz   = 1000;

    /* Structure of initialize PWM */
    pwm_config_t pwmConfig;

    /*
     * pwmConfig.enableDebugMode = false;
     * pwmConfig.enableWait = false;
     * pwmConfig.reloadSelect = kPWM_LocalReload;
     * pwmConfig.faultFilterCount = 0;
     * pwmConfig.faultFilterPeriod = 0;
     * pwmConfig.clockSource = kPWM_BusClock;
     * pwmConfig.prescale = kPWM_Prescale_Divide_1;
     * pwmConfig.initializationControl = kPWM_Initialize_LocalSync;
     * pwmConfig.forceTrigger = kPWM_Force_Local;
     * pwmConfig.reloadFrequency = kPWM_LoadEveryOportunity;
     * pwmConfig.reloadLogic = kPWM_ReloadImmediate;
     * pwmConfig.pairOperation = kPWM_Independent;
     */
    pwmSourceClockInHz            = PWM_SRC_CLK_FREQ;
    deadTimeVal                   = ((uint64_t)pwmSourceClockInHz * 650) / 1000000000;
    pwmSignal[0].pwmChannel       = kPWM_PwmA;
    pwmSignal[0].level            = kPWM_HighTrue;
    pwmSignal[0].dutyCyclePercent = 50;
    pwmSignal[0].deadtimeValue    = deadTimeVal;
    PWM_GetDefaultConfig(&pwmConfig);

    /* Use full cycle reload */
    pwmConfig.reloadLogic = kPWM_ReloadPwmFullCycle;
    /* PWM A & PWM B form a complementary PWM pair */
    pwmConfig.pairOperation   = kPWM_ComplementaryPwmA;
    pwmConfig.enableDebugMode = false;
    pwmConfig.reloadFrequency = kPWM_LoadEvery10Oportunity;

    /* Initialize submodule 0 */
    PWM_Init(DEMO_PWM_BASEADDR, kPWM_Module_0, &pwmConfig);

    /*********** PWMA_SM0 - phase A, configuration, setup 2 channel as an example ************/
    PWM_SetupPwm(DEMO_PWM_BASEADDR, kPWM_Module_0, pwmSignal, 1, kPWM_SignedCenterAligned, pwmFrequencyInHz,
                 pwmSourceClockInHz);

    /* Set the load okay bit for all submodules to load registers from their buffer */
    PWM_SetPwmLdok(DEMO_PWM_BASEADDR, kPWM_Control_Module_0, true);

    PWM_OutputTriggerEnable(DEMO_PWM_BASEADDR, kPWM_Module_0, kPWM_ValueRegister_0, true);

    PWM_StartTimer(DEMO_PWM_BASEADDR, kPWM_Control_Module_0);

    /* Init xbara module. */
    XBARA_Init(XBARA);

    /* Configure the XBARA signal connections. */
    XBARA_SetSignalsConnection(XBARA, kXBARA_InputPwm0Trg0, kXBARA_OutputAdcaTrig);
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
    EnableIRQ(DEMO_ADC_IRQ_ID);
}

static void Systick_Configuration(void)
{
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
    SysTick->LOAD = SYSTICK_CLK / DEMO_DAC_TRG_INTERVAL_FREQ_HZ - 1;

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
    DAC_DoSoftwareTriggerBuffer(DEMO_DAC_BASEADDR);
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

    PRINTF("\r\ncadc_pwm_trigger demo running...\r\n\r\n");

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
        last              = 0U;

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
    PWM_Deinit(DEMO_PWM_BASEADDR, kPWM_Module_0);

    while (1)
    {
    }
}
