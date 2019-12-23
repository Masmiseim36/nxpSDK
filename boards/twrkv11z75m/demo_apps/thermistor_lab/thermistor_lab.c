/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_adc16.h"
#include "fsl_pdb.h"
#include "fsl_ftm.h"
/* arm DSP lib */
#include "arm_math.h"

#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* The Flextimer instance/channel used for board */
#define DEMO_FTM_BASE FTM0
#define FTM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_CoreSysClk)

#define DEMO_ADC_BASE0 ADC0
#define DEMO_ADC_BASE1 ADC1
#define DEMO_ADC_IRQ0 ADC0_IRQn
#define DEMO_ADC_IRQ1 ADC1_IRQn
#define DEMO_ADC_IRQHandler0 ADC0_IRQHandler
#define DEMO_ADC_IRQHandler1 ADC1_IRQHandler
#define DEMO_PDB_BASE PDB0

/*! @brief MAX no of ADC samples */
#define MAX_NO_SAMPLES 16U
#define ALL_LED_ON()  \
    LED_GREEN1_ON();  \
    LED_YELLOW1_ON(); \
    LED_GREEN2_ON();  \
    LED_YELLOW2_ON(); \
    LED_GREEN3_ON();  \
    LED_YELLOW3_ON(); \
    LED_ORANGE_ON();  \
    LED_RED_ON();
#define ALL_LED_OFF()  \
    LED_GREEN1_OFF();  \
    LED_YELLOW1_OFF(); \
    LED_GREEN2_OFF();  \
    LED_YELLOW2_OFF(); \
    LED_GREEN3_OFF();  \
    LED_YELLOW3_OFF(); \
    LED_ORANGE_OFF();  \
    LED_RED_OFF();
#define ALL_LED_INIT()   \
    LED_GREEN1_INIT(1);  \
    LED_YELLOW1_INIT(1); \
    LED_GREEN2_INIT(1);  \
    LED_YELLOW2_INIT(1); \
    LED_GREEN3_INIT(1);  \
    LED_YELLOW3_INIT(1); \
    LED_ORANGE_INIT(1);  \
    LED_RED_INIT(1);

/*! @brief ADC channels definition for thermistors using differential input */
#define RT1_ADC_CHL (0x01U)
#define RT2_ADC_CHL (0x00U)
#define RT3_ADC_CHL (0x01U)
#define RT4_ADC_CHL (0x02U)

/*! @brief PWM frequency */
#define PWM_FREQ_HZ 8000U

/*! @brief Low pass filter */
#define DIFF_TS 400U
#define BUFF_SIZE 20U /*!< filter buf size */

/*! @brief S/W timer */
#define SWTMR_TOUT ((400 * PWM_FREQ_HZ) / 1000) /*!< 400 ms period */
#define _100MS ((100 * PWM_FREQ_HZ) / 1000)     /*!< 100 ms period */

/*! @briefRT thresholds */
#define RT1ONTH -8
#define RT1OFFTH 8
#define RT2ONTH -25
#define RT2OFFTH 20
#define RT3ONTH -10
#define RT3OFFTH 8
#define RT4ONTH -25
#define RT4OFFTH 10

/*! @brief FIR algorithm related */
#define Q15 32768U
#define FRAC16(a) (a * Q15)

#define TEST_LENGTH_SAMPLES 1U
#define BLOCK_SIZE 1U
#define NUM_TAPS 31U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void DEMO_ADC_Init(void);
static void PDB_ADC_trigger_Init(void);
static void DEMO_FTM_Init(void);
void RT_LEDs_On(void);
/*******************************************************************************
 * Variable
 ******************************************************************************/
typedef union
{
    struct
    {
        uint16_t rt1on : 1;    /*!< RT1ON */
        uint16_t rt2on : 1;    /*!< RT2ON */
        uint16_t rt3on : 1;    /*!< RT3ON */
        uint16_t rt4on : 1;    /*!< RT4ON */
        uint16_t idleloop : 1; /*!< idle loop flag */
        uint16_t Rsvd : 11;    /*!< RESERVED */
    } Bits;
    uint16_t W16;
} RT_ControlType; /*!< RT Control bits */

/* ! @brief Instances of FIR internal state */
static arm_fir_instance_q15 S1, S2, S3, S4;
/*! @brief Declare FIR State buffer of size (numTaps + u32BlockSize - 1) */
static int16_t i16FirStateQ15_1[BLOCK_SIZE + NUM_TAPS - 1];
static int16_t i16FirStateQ15_2[BLOCK_SIZE + NUM_TAPS - 1];
static int16_t i16FirStateQ15_3[BLOCK_SIZE + NUM_TAPS - 1];
static int16_t i16FirStateQ15_4[BLOCK_SIZE + NUM_TAPS - 1];
/*! @brief Instances of FIR internal state */
static arm_fir_instance_q15 S1, S2, S3, S4;

uint32_t u32BlockSize = BLOCK_SIZE;
uint32_t u32NumBlocks = TEST_LENGTH_SAMPLES / BLOCK_SIZE;
volatile uint16_t swDiffCNTR, swTimerCNTR;
volatile uint16_t timeout;
volatile RT_ControlType rton;
int16_t rt1, rt2, rt3, rt4;
int16_t rt1_filt, rt2_filt, rt3_filt, rt4_filt;
int16_t delta_rt1, delta_rt2;
int16_t delta_rt3, delta_rt4;
volatile uint16_t rt_filter_on;
volatile uint16_t rt_filter_off;
volatile uint16_t i_delay = 1, i_sample = 0;

volatile int16_t rt1_filt_buff[BUFF_SIZE], rt2_filt_buff[BUFF_SIZE];
volatile int16_t rt3_filt_buff[BUFF_SIZE], rt4_filt_buff[BUFF_SIZE];
/*! @brief For ADC samples */
uint16_t u16Result0A[MAX_NO_SAMPLES] = {0}, u16Result0B[MAX_NO_SAMPLES] = {0};
uint16_t u16Result1A[MAX_NO_SAMPLES] = {0}, u16Result1B[MAX_NO_SAMPLES] = {0};
volatile uint16_t u8CycleTimes = 0, gu8RdCounter = 0;

/* use const to output FIR filter coefficients */
/* to the data section to be placed in data memory. */
const int16_t i16FirCoefs[] = {
    0xfffb, /* {coeff 0} */
    0x0008, /* {coeff 1} */
    0x001f, /* {coeff 2} */
    0x0051, /* {coeff 3} */
    0x00a5, /* {coeff 4} */
    0x0126, /* {coeff 5} */
    0x01d9, /* {coeff 6} */
    0x02c2, /* {coeff 7} */
    0x03db, /* {coeff 8} */
    0x0519, /* {coeff 9} */
    0x0667, /* {coeff 10} */
    0x07ae, /* {coeff 11} */
    0x08d3, /* {coeff 12} */
    0x09bb, /* {coeff 13} */
    0x0a50, /* {coeff 14} */
    0x0a83, /* {coeff 15} */
    0x0a50, /* {coeff 16} */
    0x09bb, /* {coeff 17} */
    0x08d3, /* {coeff 18} */
    0x07ae, /* {coeff 19} */
    0x0667, /* {coeff 20} */
    0x0519, /* {coeff 21} */
    0x03db, /* {coeff 22} */
    0x02c2, /* {coeff 23} */
    0x01d9, /* {coeff 24} */
    0x0126, /* {coeff 25} */
    0x00a5, /* {coeff 26} */
    0x0051, /* {coeff 27} */
    0x001f, /* {coeff 28} */
    0x0008, /* {coeff 29} */
    0xfffb  /* {coeff 30} */
};
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Initialize the ADCx for HW trigger.
 *
 */
static void DEMO_ADC_Init(void)
{
    adc16_channel_config_t adcChnConfig;
    adc16_config_t adcUserConfig;

    CLOCK_SetOutDiv(0U, 2U, 3U);
    SIM->CLKDIV1 |= SIM_CLKDIV1_OUTDIV5EN(1U);

    EnableIRQ(DEMO_ADC_IRQ0);
    EnableIRQ(DEMO_ADC_IRQ1);
    /* ADC module configuration and initialization */
    ADC16_GetDefaultConfig(&adcUserConfig);

    adcUserConfig.enableContinuousConversion = false;
    adcUserConfig.enableLowPower             = false;
    adcUserConfig.enableAsynchronousClock    = false;
    adcUserConfig.referenceVoltageSource     = kADC16_ReferenceVoltageSourceVref;
    adcUserConfig.clockSource                = kADC16_ClockSourceAlt2;
    adcUserConfig.clockDivider               = kADC16_ClockDivider1;
    adcUserConfig.resolution                 = kADC16_Resolution16Bit;
    adcUserConfig.longSampleMode             = kADC16_LongSampleCycle16;

    ADC16_Init(DEMO_ADC_BASE0, &adcUserConfig);
    ADC16_Init(DEMO_ADC_BASE1, &adcUserConfig);
    ADC16_EnableHardwareTrigger(DEMO_ADC_BASE0, true);
    ADC16_EnableHardwareTrigger(DEMO_ADC_BASE1, true);

    /* RT1 Chl Config */
    adcChnConfig.channelNumber = RT1_ADC_CHL;
#if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
    adcChnConfig.enableDifferentialConversion = true;
#endif
    adcChnConfig.enableInterruptOnConversionCompleted = true;
    ADC16_SetChannelConfig(DEMO_ADC_BASE0, 0U, &adcChnConfig);
#if defined(FSL_FEATURE_ADC16_HAS_MUX_SELECT) && FSL_FEATURE_ADC16_HAS_MUX_SELECT
    ADC16_SetChannelMuxMode(DEMO_ADC_BASE0, kADC16_ChannelMuxA);
#endif
    /* RT2 Chl Config */
    adcChnConfig.channelNumber = RT2_ADC_CHL;
#if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
    adcChnConfig.enableDifferentialConversion = true;
#endif
    adcChnConfig.enableInterruptOnConversionCompleted = true;
    ADC16_SetChannelConfig(DEMO_ADC_BASE0, 1U, &adcChnConfig);

    /* RT3 Chl Config */
    adcChnConfig.channelNumber = RT3_ADC_CHL;
#if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
    adcChnConfig.enableDifferentialConversion = true;
#endif
    adcChnConfig.enableInterruptOnConversionCompleted = true;
    ADC16_SetChannelConfig(DEMO_ADC_BASE1, 0U, &adcChnConfig);
#if defined(FSL_FEATURE_ADC16_HAS_MUX_SELECT) && FSL_FEATURE_ADC16_HAS_MUX_SELECT
    ADC16_SetChannelMuxMode(DEMO_ADC_BASE1, kADC16_ChannelMuxA);
#endif
    /* RT4 Chl Config */
    adcChnConfig.channelNumber = RT4_ADC_CHL;
#if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
    adcChnConfig.enableDifferentialConversion = true;
#endif
    adcChnConfig.enableInterruptOnConversionCompleted = true;
    ADC16_SetChannelConfig(DEMO_ADC_BASE1, 1U, &adcChnConfig);
}

/*!
 * @brief Initialize the PDB for ADC hardware trigger.
 *
 */
static void PDB_ADC_trigger_Init(void)
{
    /* Allocate memory for PDB driver */
    pdb_config_t MyPdbUserConfigStruct;
    pdb_adc_pretrigger_config_t MyPdbAdcTriggerConfigStruct;

    PDB_GetDefaultConfig(&MyPdbUserConfigStruct);
    MyPdbUserConfigStruct.triggerInputSource = kPDB_TriggerInput8;
    PDB_Init(PDB0, &MyPdbUserConfigStruct);
    PDB_SetModulusValue(DEMO_PDB_BASE, 1500U);
    PDB_SetCounterDelayValue(DEMO_PDB_BASE, 1500U);

    /* Initialize the ADC trigger in PDB.*/
    MyPdbAdcTriggerConfigStruct.enableBackToBackOperationMask = true;
    MyPdbAdcTriggerConfigStruct.enableOutputMask              = true;
    MyPdbAdcTriggerConfigStruct.enablePreTriggerMask          = true;

    PDB_SetADCPreTriggerDelayValue(DEMO_PDB_BASE, kPDB_ADCTriggerChannel0, kPDB_ADCPreTrigger0, 200U);

    PDB_SetADCPreTriggerConfig(DEMO_PDB_BASE, kPDB_ADCTriggerChannel0, &MyPdbAdcTriggerConfigStruct);

    MyPdbAdcTriggerConfigStruct.enableBackToBackOperationMask = true;
    MyPdbAdcTriggerConfigStruct.enableOutputMask              = true;
    MyPdbAdcTriggerConfigStruct.enablePreTriggerMask          = true;

    PDB_SetADCPreTriggerDelayValue(DEMO_PDB_BASE, kPDB_ADCTriggerChannel0, kPDB_ADCPreTrigger1, 1000U);

    PDB_SetADCPreTriggerConfig(DEMO_PDB_BASE, kPDB_ADCTriggerChannel1, &MyPdbAdcTriggerConfigStruct);

    /* Initialize the ADC trigger in PDB.*/
    MyPdbAdcTriggerConfigStruct.enableBackToBackOperationMask = 1U;
    MyPdbAdcTriggerConfigStruct.enableOutputMask              = 3U;
    MyPdbAdcTriggerConfigStruct.enablePreTriggerMask          = 3U;

    PDB_SetADCPreTriggerDelayValue(DEMO_PDB_BASE, kPDB_ADCTriggerChannel1, kPDB_ADCPreTrigger0, 200U);

    PDB_SetADCPreTriggerConfig(DEMO_PDB_BASE, kPDB_ADCTriggerChannel0, &MyPdbAdcTriggerConfigStruct);

    MyPdbAdcTriggerConfigStruct.enableBackToBackOperationMask = 1U;
    MyPdbAdcTriggerConfigStruct.enableOutputMask              = 3U;
    MyPdbAdcTriggerConfigStruct.enablePreTriggerMask          = 3U;

    PDB_SetADCPreTriggerDelayValue(DEMO_PDB_BASE, kPDB_ADCTriggerChannel1, kPDB_ADCPreTrigger1, 1000U);

    PDB_SetADCPreTriggerConfig(DEMO_PDB_BASE, kPDB_ADCTriggerChannel1, &MyPdbAdcTriggerConfigStruct);
}

/*!
 * @brief Initialize the FTM to generate trigger source
 *
 */
static void DEMO_FTM_Init(void)
{
    ftm_config_t ftmInfo;
    ftm_chnl_pwm_signal_param_t ftmParam;

    FTM_GetDefaultConfig(&ftmInfo);
    ftmInfo.deadTimePrescale = kFTM_Deadtime_Prescale_4;
    ftmInfo.deadTimeValue    = 19U;
    FTM_Init(DEMO_FTM_BASE, &ftmInfo);

    /* FTM config */
    ftmParam.chnlNumber            = kFTM_Chnl_1;
    ftmParam.firstEdgeDelayPercent = 0U;
    ftmParam.dutyCyclePercent      = 50U;
    ftmParam.level                 = kFTM_LowTrue;
    ftmParam.enableDeadtime        = false;

    /* Configure FTM0 channel ouput period is 16KHz complementary waveform (channel n and n+1) */
    FTM_SetupPwm(DEMO_FTM_BASE, &ftmParam, 1U, kFTM_EdgeAlignedPwm, PWM_FREQ_HZ, FTM_SOURCE_CLOCK);

    FTM_SetSoftwareTrigger(DEMO_FTM_BASE, true);

    FTM_StartTimer(DEMO_FTM_BASE, kFTM_SystemClock);

    /* Issue a software trigger to update registers */
    FTM_SetSoftwareTrigger(DEMO_FTM_BASE, true);
    DEMO_FTM_BASE->EXTTRIG = 0x20;
}

/*!
 * @brief check 4 thermistors to see if fingers are close to them or left.
 *
 * This function is called by ADC interrupts.
 */
void RT_Check(int16_t i16Rt1, int16_t i16Rt2, int16_t i16Rt3, int16_t i16Rt4)
{
    rt1 = i16Rt1;
    rt2 = i16Rt2;
    rt3 = i16Rt3;
    rt4 = i16Rt4;

    /* Execute FIR filter on ADC samples */
    arm_fir_q15(&S1, &i16Rt1, &rt1_filt, 1);
    arm_fir_q15(&S2, &i16Rt2, &rt2_filt, 1);
    arm_fir_q15(&S3, &i16Rt3, &rt3_filt, 1);
    arm_fir_q15(&S4, &i16Rt4, &rt4_filt, 1);

    /* Reset SWSamplerCNT */
    if (swDiffCNTR >= DIFF_TS)
    {
        /* Evaluate delta */
        delta_rt1 = rt1_filt - rt1_filt_buff[i_delay];
        delta_rt2 = rt2_filt - rt2_filt_buff[i_delay];
        delta_rt3 = rt3_filt - rt3_filt_buff[i_delay];
        delta_rt4 = rt4_filt - rt4_filt_buff[i_delay];

        /* Store samples in the buffer */
        rt1_filt_buff[i_sample] = rt1_filt;
        rt2_filt_buff[i_sample] = rt2_filt;
        rt3_filt_buff[i_sample] = rt3_filt;
        rt4_filt_buff[i_sample] = rt4_filt;
        if (++i_sample >= BUFF_SIZE)
        {
            i_sample = 0;
        }
        if (++i_delay >= BUFF_SIZE)
        {
            i_delay = 0;
        }
        /* Detect rt ON/OFF */
        /* Three consequent samples has to be higher than threshold limit */
        /* rt1 ON filter */
        if (delta_rt1 < RT1ONTH)
        {
            rt_filter_on += 0x1;
            if ((rt_filter_on & 0xf) >= 0x3)
            {
                rt_filter_on &= ~0xf;
                rton.Bits.rt1on = 1;
            }
        }
        else
        {
            rt_filter_on &= ~0xf;
        }
        /* rt1 OFF filter */
        if (delta_rt1 > RT1OFFTH)
        {
            rt_filter_off += 0x1;
            if ((rt_filter_off & 0xf) >= 0x3)
            {
                rt_filter_off &= ~0xf;
                rton.W16 &= ~0xf;
            }
        }
        else
        {
            rt_filter_off &= ~0xf;
        }

        /* rt2 ON filter */
        if (delta_rt2 < RT2ONTH)
        {
            rt_filter_on += 0x10;
            if ((rt_filter_on & 0xf0) >= 0x30)
            {
                rt_filter_on &= ~0xf0;
                rton.Bits.rt2on = 1;
            }
        }
        else
        {
            rt_filter_on &= ~0xf0;
        }
        /* rt2 OFF filter */
        if (delta_rt2 > RT2OFFTH)
        {
            rt_filter_off += 0x10;
            if ((rt_filter_off & 0xf0) >= 0x30)
            {
                rt_filter_off &= ~0xf0;
                rton.W16 &= ~0xf;
            }
        }
        else
        {
            rt_filter_off &= ~0xf0;
        }

        /* rt3 ON filter */
        if (delta_rt3 < RT3ONTH)
        {
            rt_filter_on += 0x100;
            if ((rt_filter_on & 0xf00) >= 0x300)
            {
                rt_filter_on &= ~0xf00;
                rton.Bits.rt3on = 1;
            }
        }
        else
        {
            rt_filter_on &= ~0xf00;
        }
        /* rt3 OFF filter */
        if (delta_rt3 > RT3OFFTH)
        {
            rt_filter_off += 0x100;
            if ((rt_filter_off & 0xf00) >= 0x300)
            {
                rt_filter_off &= ~0xf00;
                rton.W16 &= ~0xf;
            }
        }
        else
        {
            rt_filter_off &= ~0xf00;
        }

        /* rt4 ON filter */
        if (delta_rt4 < RT4ONTH)
        {
            rt_filter_on += 0x1000;
            if ((rt_filter_on & 0xf000) >= 0x3000)
            {
                rt_filter_on &= ~0xf000;
                rton.Bits.rt4on = 1;
            }
        }
        else
        {
            rt_filter_on &= ~0xf000;
        }
        /* rt4 OFF filter */
        if (delta_rt4 > RT4OFFTH)
        {
            rt_filter_off += 0x1000;
            if ((rt_filter_off & 0xf000) >= 0x3000)
            {
                rt_filter_off &= ~0xf000;
                rton.W16 &= ~0xf;
            }
        }
        else
        {
            rt_filter_off &= ~0xf000;
        }

        /* Reset counter */
        swDiffCNTR = 0;
    }
    else
    {
        swDiffCNTR++;
    }

    /* Reset SWTimerCNTR every 400ms */
    /* Software timer is used to control LED flashing intervals */
    if (swTimerCNTR >= SWTMR_TOUT)
    {
        swTimerCNTR = 0;
    }
    else
    {
        swTimerCNTR++;
    }

    if (swTimerCNTR == timeout)
    {
        rton.Bits.idleloop = 0;
        /* Time out event every 100 ms */
        timeout += _100MS;
        if (timeout >= SWTMR_TOUT)
            timeout = 0;
    }
}

void RT_LEDs_On(void)
{
    if (rton.Bits.rt1on)
    {
        ALL_LED_OFF();
        LED_GREEN1_ON();
        LED_YELLOW1_ON();
    }
    if (rton.Bits.rt2on)
    {
        ALL_LED_OFF();
        LED_GREEN2_ON();
        LED_YELLOW2_ON();
    }
    if (rton.Bits.rt3on)
    {
        ALL_LED_OFF();
        LED_GREEN3_ON();
        LED_YELLOW3_ON();
    }
    if (rton.Bits.rt4on)
    {
        ALL_LED_OFF();
        LED_ORANGE_ON();
        LED_RED_ON();
    }
}

/*!
 * @brief override the DEMO_ADC_IRQHandler0
 *
 */
void DEMO_ADC_IRQHandler0(void)
{
    if (ADC16_GetChannelStatusFlags(DEMO_ADC_BASE0, 0U))
    {
        u16Result0A[u8CycleTimes] = ADC16_GetChannelConversionValue(DEMO_ADC_BASE0, 0U);
    }
    if (ADC16_GetChannelStatusFlags(DEMO_ADC_BASE0, 1U))
    {
        u16Result0B[u8CycleTimes] = ADC16_GetChannelConversionValue(DEMO_ADC_BASE0, 1U);
    }
    __DSB();
}

/*!
 * @brief override the DEMO_ADC_IRQHandler1
 *
 */
void DEMO_ADC_IRQHandler1(void)
{
    if (ADC16_GetChannelStatusFlags(DEMO_ADC_BASE1, 0U))
    {
        u16Result1A[u8CycleTimes] = ADC16_GetChannelConversionValue(DEMO_ADC_BASE1, 0U);
    }
    if (ADC16_GetChannelStatusFlags(DEMO_ADC_BASE1, 1U))
    {
        u16Result1B[u8CycleTimes] = ADC16_GetChannelConversionValue(DEMO_ADC_BASE1, 1U);
        RT_Check(u16Result0A[u8CycleTimes], u16Result1A[u8CycleTimes], u16Result0B[u8CycleTimes],
                 u16Result1B[u8CycleTimes]);
        u8CycleTimes++;
        if (u8CycleTimes >= MAX_NO_SAMPLES)
        {
            u8CycleTimes = 0;
        }
    }
    __DSB();
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint16_t timeout100MS;
    arm_status err;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("Thermistor lab demo!\r\n");

    ALL_LED_INIT();

    /* Call FIR init function to initialize the instance structure. */
    err = arm_fir_init_q15(&S1, NUM_TAPS, (q15_t *)&i16FirCoefs[0], &i16FirStateQ15_1[0], u32BlockSize);
    if (err != ARM_MATH_SUCCESS)
    {
        return (-1);
    }
    err = arm_fir_init_q15(&S2, NUM_TAPS, (q15_t *)&i16FirCoefs[0], &i16FirStateQ15_2[0], u32BlockSize);
    if (err != ARM_MATH_SUCCESS)
    {
        return (-1);
    }

    err = arm_fir_init_q15(&S3, NUM_TAPS, (q15_t *)&i16FirCoefs[0], &i16FirStateQ15_3[0], u32BlockSize);
    if (err != ARM_MATH_SUCCESS)
    {
        return (-1);
    }

    err = arm_fir_init_q15(&S4, NUM_TAPS, (q15_t *)&i16FirCoefs[0], &i16FirStateQ15_4[0], u32BlockSize);
    if (err != ARM_MATH_SUCCESS)
    {
        return (-1);
    }

    /* Initialize ADC module */
    DEMO_ADC_Init();
    /* Initialize PDB module */
    PDB_ADC_trigger_Init();
    /* Initialize FTM module */
    DEMO_FTM_Init();
    /* wait until rtx_filt_buffers are initialized */
    while (swTimerCNTR < ((350 * PWM_FREQ_HZ) / 1000))
        ;
    while (swTimerCNTR < ((350 * PWM_FREQ_HZ) / 1000))
        ;
    PRINTF("begin test...\r\n");

    rton.W16 &= ~0xF;
    while (1)
    {
        timeout100MS = timeout / (_100MS);
        switch (timeout100MS)
        {
            case 0:
                RT_LEDs_On();
                break;
            case 1:
                ALL_LED_OFF();
                break;
            case 2:
                RT_LEDs_On();
                break;
            case 3:
                ALL_LED_OFF();
                break;
            default:
                break;
        }
    }
}
