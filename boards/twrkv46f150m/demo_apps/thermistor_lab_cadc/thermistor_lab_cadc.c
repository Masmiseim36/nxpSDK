/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "board.h"
#include "fsl_cadc.h"
#include "fsl_debug_console.h"
#include "fsl_sim.h"

/* arm DSP lib */
#include "arm_math.h"

#include "pin_mux.h"
/******************************************************************************
 * Definitions
 *****************************************************************************/

#define DEMO_CADC_BASEADDR ADC

/*! @brief ADC channels definition for thermistors using differential input */
#define RT1_ADC_CHL     (0x01U)
#define RT2_ADC_CHL     (0x00U)
#define RT3_ADC_CHL     (0x01U)
#define RT4_ADC_CHL     (0x02U)

/*! @brief PWM frequency */
#define PWM_FREQ_HZ          16000



/*! @brief Low pass filter */
#define DIFF_TS     400
#define BUFF_SIZE   20                              /* filter buf size */

/*! @brief S/W timer */
#define SWTMR_TOUT  ((400*PWM_FREQ_HZ)/1000)        /*!< 400 ms period */
#define _100MS      ((100*PWM_FREQ_HZ)/1000)        /*!< 100 ms period */

/*! @briefRT thresholds */
#define TH_TRY       10
#define RT1ONTH     -5
#define RT1OFFTH    3
#define RT2ONTH     -3
#define RT2OFFTH    2
#define RT3ONTH     -TH_TRY
#define RT3OFFTH    TH_TRY-3
#define RT4ONTH     -5
#define RT4OFFTH    2

/*! @brief FIR algorithm related */
#define Q15                     32768
#define FRAC16(a)               (a*Q15)

#define TEST_LENGTH_SAMPLES     1
#define BLOCK_SIZE              1
#define NUM_TAPS                32

/*! @brief MAX no of ADC samples */
#define MAX_NO_SAMPLES          16

#define ALL_LED_ON()      LED_GREEN1_ON();LED_GREEN2_ON();LED_GREEN3_ON();LED_YELLOW1_ON();LED_YELLOW2_ON();LED_YELLOW3_ON();LED_GREEN4_ON();LED_YELLOW4_ON();\
                          LED_ORANGE_ON()
#define ALL_LED_OFF()     LED_GREEN1_OFF();LED_GREEN2_OFF();LED_GREEN3_OFF();LED_YELLOW1_OFF();LED_YELLOW2_OFF();LED_YELLOW3_OFF();LED_GREEN4_OFF();LED_YELLOW4_OFF();\
                          LED_ORANGE_OFF();
#define ALL_LED_INIT()    LED_GREEN1_INIT(LOGIC_LED_OFF);LED_GREEN2_INIT(LOGIC_LED_OFF);LED_GREEN3_INIT(LOGIC_LED_OFF);LED_YELLOW1_INIT(LOGIC_LED_OFF);\
                          LED_YELLOW2_INIT(LOGIC_LED_OFF);LED_YELLOW3_INIT(LOGIC_LED_OFF);LED_GREEN4_INIT(LOGIC_LED_OFF);LED_YELLOW4_INIT(LOGIC_LED_OFF);\
                          LED_ORANGE_INIT(LOGIC_LED_OFF);


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void RT_Check(int16_t i16Rt1, int16_t i16Rt2, int16_t i16Rt3, int16_t i16Rt4);
void RT_LEDs_On(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

typedef union
{
    struct
    {
        uint16_t rt1on          : 1;    /* !< RT1ON */
        uint16_t rt2on          : 1;    /* !< RT2ON */
        uint16_t rt3on          : 1;    /* !< RT3ON */
        uint16_t rt4on          : 1;    /* !< RT4ON */
        uint16_t idleloop       : 1;    /* !< idle loop flag */
        uint16_t Rsvd           : 11;   /* !< RESERVED */
    } Bits;
    uint16_t W16;
} RT_ControlType;                       /* !< RT Control bits */

/*! @brief Global variables */
volatile uint16_t        swDiffCNTR, swTimerCNTR;
volatile uint16_t        timeout;

volatile RT_ControlType  rton;
volatile uint16_t        rt_filter_on;
volatile uint16_t        rt_filter_off;

volatile int16_t         rt1_filt_buff[BUFF_SIZE], rt2_filt_buff[BUFF_SIZE];
volatile int16_t         rt3_filt_buff[BUFF_SIZE], rt4_filt_buff[BUFF_SIZE];

volatile uint16_t        i_delay = 1, i_sample = 0;

int16_t rt1, rt2, rt3, rt4;
int16_t rt1_filt, rt2_filt, rt3_filt, rt4_filt;
int16_t delta_rt1, delta_rt2;
int16_t delta_rt3, delta_rt4;

/*! @brief Declare FIR State buffer of size (numTaps + u32BlockSize - 1) */
static int16_t i16FirStateQ15_1[BLOCK_SIZE + NUM_TAPS - 1];
static int16_t i16FirStateQ15_2[BLOCK_SIZE + NUM_TAPS - 1];
static int16_t i16FirStateQ15_3[BLOCK_SIZE + NUM_TAPS - 1];
static int16_t i16FirStateQ15_4[BLOCK_SIZE + NUM_TAPS - 1];

/*! @brief Instances of FIR internal state */
static arm_fir_instance_q15 S1,S2,S3,S4;

uint32_t u32BlockSize = BLOCK_SIZE;
uint32_t u32NumBlocks = TEST_LENGTH_SAMPLES/BLOCK_SIZE;

/*! @brief For ADC samples */
uint16_t u16Result0A[MAX_NO_SAMPLES]={0},u16Result0B[MAX_NO_SAMPLES]={0},u16Result1A[MAX_NO_SAMPLES]={0},u16Result1B[MAX_NO_SAMPLES]={0};
volatile uint16_t u8CycleTimes=0,gu8RdCounter =0;

/*
 * Use const to output FIR filter coefficients
 * to the data section to be placed in data memory.
 */
const int16_t i16FirCoefs[] = {  0xfffb, /* {coeff 0} */
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
/******************************************************************************
 * Code
 *****************************************************************************/

/*!
 * @brief Initialize the CADCx for HW trigger.
 *
 */
static void DEMO_CADC_Init(void)
{
    cadc_config_t cadcConfigStruct;
    cadc_converter_config_t cadcConverterConfigStruct;
    cadc_sample_config_t cadcSampleConfigStruct;

    /* Configure the CADC */
    CADC_GetDefaultConfig(&cadcConfigStruct);
    cadcConfigStruct.dualConverterScanMode = kCADC_DualConverterWorkAsTriggeredParallel;
    cadcConfigStruct.enableSimultaneousMode = true;
    cadcConfigStruct.powerUpDelay = 0x2A;
    CADC_Init(DEMO_CADC_BASEADDR, &cadcConfigStruct);
    CADC_EnableInterrupts(DEMO_CADC_BASEADDR, kCADC_ZeroCrossingInterruptEnable | kCADC_LowLimitInterruptEnable | kCADC_HighLimitInterruptEnable);
    EnableIRQ(ADC_ERR_IRQn);
    EnableIRQ(ADCA_IRQn);
    EnableIRQ(ADCB_IRQn);

    /* Configure ADC sample input channel. */
    /*Mux selection for ADCA - CH6*/
    SIM->ADCOPT |= SIM_ADCOPT_ADCACH6SEL(2U);
    /*Mux selection for ADCA - CH7*/
    SIM->ADCOPT |= SIM_ADCOPT_ADCACH7SEL(2U);
    /*Mux selection for ADCB - CH6*/
    SIM->ADCOPT |= SIM_ADCOPT_ADCBCH6SEL(0U);
    /*Mux selection for ADCB - CH7*/
    SIM->ADCOPT |= SIM_ADCOPT_ADCBCH7SEL(4U);

    /* Enable converter */
    CADC_EnableConverterDMA(DEMO_CADC_BASEADDR, kCADC_ConverterA, false);
    CADC_EnableConverterDMA(DEMO_CADC_BASEADDR, kCADC_ConverterB, false);
    CADC_EnableConverter(DEMO_CADC_BASEADDR, kCADC_ConverterA, true);
    CADC_EnableConverter(DEMO_CADC_BASEADDR, kCADC_ConverterB, true);
    CADC_EnableConverterSyncInput(DEMO_CADC_BASEADDR, kCADC_ConverterA, true);
    CADC_EnableConverterSyncInput(DEMO_CADC_BASEADDR, kCADC_ConverterB, true);
    CADC_EnableInterrupts(DEMO_CADC_BASEADDR, kCADC_ConverterAEndOfScanInterruptEnable);
    CADC_DisableInterrupts(DEMO_CADC_BASEADDR, kCADC_ConverterBEndOfScanInterruptEnable);

    /* Configure converter */
    CADC_GetDefaultConverterConfig(&cadcConverterConfigStruct);
    cadcConverterConfigStruct.clockDivisor = 0x3F;
    cadcConverterConfigStruct.speedMode = kCADC_SpeedMode3;
    cadcConverterConfigStruct.sampleWindowCount = 0U;
    cadcConverterConfigStruct.highReferenceVoltageSource = kCADC_ReferenceVoltageVrefPad;
    cadcConverterConfigStruct.lowReferenceVoltageSource = kCADC_ReferenceVoltageVrefPad;
    CADC_SetConverterConfig(DEMO_CADC_BASEADDR, kCADC_ConverterA|kCADC_ConverterB, &cadcConverterConfigStruct);
    CADC_EnableConverterPower(DEMO_CADC_BASEADDR, kCADC_ConverterA|kCADC_ConverterB, true);

    /* Configure sample */
    cadcSampleConfigStruct.zeroCrossingMode = kCADC_ZeroCorssingDisabled;
    cadcSampleConfigStruct.lowLimitValue = 0U;
    cadcSampleConfigStruct.highLimitValue = 0xFFF8U;
    cadcSampleConfigStruct.offsetValue = 0U;
    cadcSampleConfigStruct.enableDifferentialPair = true;
    cadcSampleConfigStruct.channelGain = kCADC_ChannelGainx1;
    cadcSampleConfigStruct.enableWaitSync = false;

    /* For each slot in conversion sequence. */
    /* Slot 0. */
    cadcSampleConfigStruct.channelNumber = 4U;
    CADC_SetSampleConfig(DEMO_CADC_BASEADDR, 0U, &cadcSampleConfigStruct);
    /* Slot 1. */
    cadcSampleConfigStruct.channelNumber = 6U;
    CADC_SetSampleConfig(DEMO_CADC_BASEADDR, 1U, &cadcSampleConfigStruct);
    /* Slot 2. */
    cadcSampleConfigStruct.channelNumber = 12U;
    CADC_SetSampleConfig(DEMO_CADC_BASEADDR, 8U, &cadcSampleConfigStruct);
    /* Slot 3. */
    cadcSampleConfigStruct.channelNumber = 14U;
    CADC_SetSampleConfig(DEMO_CADC_BASEADDR, 9U, &cadcSampleConfigStruct);
    /* End seq. */
    CADC_EnableSample(DEMO_CADC_BASEADDR, 4U, false);
    CADC_SetSampleConfig(DEMO_CADC_BASEADDR, 2U, &cadcSampleConfigStruct);

    /* Wait until power up */
    while ((CADC_GetStatusFlags(DEMO_CADC_BASEADDR) == (kCADC_ConverterAPowerDownFlag | kCADC_ConverterBPowerDownFlag)) )
    {}

    /*
    * Trigger the conversion sequence.
    * When in sequential simult mode, operate converter A will driver the
    * main conversion sequence.
    */
    CADC_DoSoftwareTriggerConverter(DEMO_CADC_BASEADDR, kCADC_ConverterA);
}

void ADCA_IRQHandler(void)
{
    if(CADC_GetStatusFlags(DEMO_CADC_BASEADDR))
    {
        u16Result0A[u8CycleTimes] = CADC_GetSampleResultValue(DEMO_CADC_BASEADDR, 0U)>>3U;
        u16Result0B[u8CycleTimes] = CADC_GetSampleResultValue(DEMO_CADC_BASEADDR, 1U)>>3U;
        u16Result1A[u8CycleTimes] = CADC_GetSampleResultValue(DEMO_CADC_BASEADDR, 8U)>>3U;
        u16Result1B[u8CycleTimes] = CADC_GetSampleResultValue(DEMO_CADC_BASEADDR, 9U)>>3U;
        RT_Check(u16Result0A[u8CycleTimes], u16Result1A[u8CycleTimes],
                 u16Result0B[u8CycleTimes], u16Result1B[u8CycleTimes]);
        u8CycleTimes++;
        CADC_ClearStatusFlags(DEMO_CADC_BASEADDR, kCADC_ConverterAEndOfScanFlag);
        if(u8CycleTimes >= MAX_NO_SAMPLES)
        {
            u8CycleTimes = 0;
        }
    }
    CADC_DoSoftwareTriggerConverter(DEMO_CADC_BASEADDR, kCADC_ConverterA);
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
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
    rt1_filt = i16Rt1;
    rt2_filt = i16Rt2;
    rt3_filt = i16Rt3;
    rt4_filt = i16Rt4;
    /* Reset SWSamplerCNT */
    if(swDiffCNTR >= DIFF_TS)
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
        if(++i_sample >= BUFF_SIZE)
        {
            i_sample = 0;
        }
        if(++i_delay >= BUFF_SIZE)
        {
            i_delay = 0;
        }
        /* Detect rt ON/OFF
         * Three consequent samples has to be higher than threshold limit
         * rt1 ON filter
         */
        if(delta_rt1 < RT1ONTH)
        {
            rt_filter_on += 0x1;
            if((rt_filter_on & 0xf) >= 0x3)
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
        if(delta_rt1 > RT1OFFTH)
        {
            rt_filter_off += 0x1;
            if((rt_filter_off & 0xf) >= 0x3)
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
        if(delta_rt2 < RT2ONTH)
        {
            rt_filter_on += 0x10;
            if((rt_filter_on & 0xf0) >= 0x30)
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
        if(delta_rt2 > RT2OFFTH)
        {
            rt_filter_off += 0x10;
            if((rt_filter_off & 0xf0) >= 0x30)
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
        if(delta_rt3 < RT3ONTH)
        {
            rt_filter_on += 0x100;
            if((rt_filter_on & 0xf00) >= 0x300)
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
        if(delta_rt3 > RT3OFFTH)
        {
            rt_filter_off += 0x100;
            if((rt_filter_off & 0xf00) >= 0x300)
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
        if(delta_rt4 < RT4ONTH)
        {
            rt_filter_on += 0x1000;
            if((rt_filter_on & 0xf000) >= 0x3000)
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
        if(delta_rt4 > RT4OFFTH)
        {
            rt_filter_off += 0x1000;
            if((rt_filter_off & 0xf000) >= 0x3000)
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

    /* Reset SWTimerCNTR every 400ms
       Software timer is used to control LED flashing intervals */
    if(swTimerCNTR >= SWTMR_TOUT)
    {
        swTimerCNTR = 0;
    }
    else
    {
        swTimerCNTR++;
    }

    if(swTimerCNTR == timeout)
    {
        rton.Bits.idleloop = 0;
        /* Time out event every 100 ms */
        timeout += _100MS;
        if(timeout >= SWTMR_TOUT) timeout = 0;
    }
}

void RT_LEDs_On(void)
{
    if(rton.Bits.rt1on)
    {
       ALL_LED_OFF();
       LED_GREEN1_ON();
       LED_YELLOW1_ON();
    }
    if(rton.Bits.rt2on)
    {
       ALL_LED_OFF();
       LED_GREEN2_ON();
       LED_YELLOW2_ON();
    }
    if(rton.Bits.rt3on)
    {
       ALL_LED_OFF();
       LED_GREEN3_ON();
       LED_YELLOW3_ON();
    }
    if(rton.Bits.rt4on)
    {
       ALL_LED_OFF();
       LED_GREEN4_ON();
       LED_YELLOW4_ON();
    }
}

/*!
 * @brief main demo function.
 */
int main(void)
{
    uint16_t timeout100MS;

    arm_status err;
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("Thermistor Lab CADC Demo!\r\n");

    ALL_LED_INIT();
    ALL_LED_OFF();

    /* Call FIR init function to initialize the instance structure. */
    err = arm_fir_init_q15(&S1, NUM_TAPS, (q15_t*)&i16FirCoefs[0], &i16FirStateQ15_1[0], u32BlockSize);
    if (err != ARM_MATH_SUCCESS)
    {
        return (-1);
    }
    err = arm_fir_init_q15(&S2, NUM_TAPS, (q15_t*)&i16FirCoefs[0], &i16FirStateQ15_2[0], u32BlockSize);
    if (err != ARM_MATH_SUCCESS)
    {
        return (-1);
    }

    err = arm_fir_init_q15(&S3, NUM_TAPS, (q15_t*)&i16FirCoefs[0], &i16FirStateQ15_3[0], u32BlockSize);
    if (err != ARM_MATH_SUCCESS)
    {
        return (-1);
    }

    err = arm_fir_init_q15(&S4, NUM_TAPS, (q15_t*)&i16FirCoefs[0], &i16FirStateQ15_4[0], u32BlockSize);
    if (err != ARM_MATH_SUCCESS)
    {
        return (-1);
    }

    /* Initialize CADC module */
    DEMO_CADC_Init();

    /* Wait until rtx_filt_buffers are initialized */
    while(swTimerCNTR <  ((350*PWM_FREQ_HZ)/1000));
    while(swTimerCNTR <  ((350*PWM_FREQ_HZ)/1000));
    PRINTF("Begin test...\r\n");

    rton.W16 &= ~0xF;
    while(1)
    {
        timeout100MS = timeout / (_100MS);

        switch(timeout100MS)
        {
            case 0:
            case 2:
                RT_LEDs_On();
                break;
            case 1:
            case 3:
                ALL_LED_OFF();
                break;
            default:
                break;
        }
    }
}
