/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mcdrv_lpcxpresso55s69.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Configuration structure for 3-phase PWM mc driver */
mcdrv_sctimer_pwm_t g_sM1Pwm3ph;

/* Structure for current and voltage measurement*/
mcdrv_adc_t g_sM1AdcSensor;

/* Structure containing clock frequencies and modulos */
clock_setup_t g_sClockSetup;

static ctimer_match_config_t matchConfig0;

uint32_t ui32MainClockFreq;

sctimer_config_t sctimerInfo;
sctimer_pwm_signal_param_t pwmParam;
uint32_t sctimerClock;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
* @brief   void MCDRV_Init_M1(void)
*           - Motor control driver main initialization
*           - Calls initialization functions of peripherals required for motor
*             control functionality
*
* @param   void
*
* @return  none
*/
void MCDRV_Init_M1(void)
{
    /* Init application clock dependent variables */
    InitClock();

    /* Init ADC */
    M1_MCDRV_ADC_PERIPH_INIT();

    /* Init CTMR2 (slow loop counter) */
    M1_MCDRV_TMR_SLOWLOOP_INIT();

    /* Init CMP */
    M1_MCDRV_CMP_INIT();

    /* 6-channel PWM peripheral init for M1 */
    M1_MCDRV_PWM_PERIPH_INIT();

}

/*!
* @brief      Core, bus, flash clock setup
*
* @param      void
*
* @return     none
*/
void InitClock(void)
{

    /* Parameters for motor M1 */
    g_sClockSetup.ui16M1PwmFreq = M1_PWM_FREQ; /* 10 kHz */

    g_sClockSetup.ui16M1SpeedLoopFreq = M1_SPEED_LOOP_FREQ; /* 1kHz */
}

/*!
* @brief   void InitADC(void)
*           - Initialization of the ADC peripheral
*           - 3-phase center-aligned PWM
*
* @param   void
*
* @return  none
*/
void InitADC(void)
{
  /* ***************************************************************************************************************************
   * SW/HW trigger -> TCTRL - set which CMD is called -> CMDLx - choose ADC channel               -> ADC conversion            *
   *                        - set which FIFO is used           - choose single or dual conversion                              *
   *                        - set HW trigger enabled             (executed simultaneously)                                     *
   *                                                     CMDHx - chose next ADC channel                                        *
   *                                                           - set wait trigger                                              *
   *                                                                                                                           *
   * If more ADC conversion are executed in a row - set watermark interrupt on number of conversion FCTRL[x] -> FWMARK.        *
   * Then will be interrupt generated after end of all ADC conversion. FIFO1Watermark is a programmable threshold setting.     *
   * When the number of datawords stored in the ADC Result FIFO1 is greater than the value in this field, the ready flag would *
   * be asserted to indicate stored data has reached the programmable threshold.                                               *
   *****************************************************************************************************************************/

    lpadc_config_t mLpadcConfigStruct;
    lpadc_conv_trigger_config_t mLpadcTriggerConfigStruct;
    lpadc_conv_command_config_t mLpadcCommandConfigStruct;

    /* Set clock source and divider for ADC0 before calibration */
	// Clocked from fro_hf
    CLOCK_SetClkDiv(kCLOCK_DivAdcAsyncClk, 1U, true); //96MHz/2 = 48MHz
    CLOCK_AttachClk(kFRO_HF_to_ADC_CLK);

	/*
	// Clocked from main_clk
    CLOCK_SetClkDiv(kCLOCK_DivAdcAsyncClk, 5U, true); //150MHz/6 = 25MHz
    CLOCK_AttachClk(kMAIN_CLK_to_ADC_CLK);
	*/

    /* Disable LDOGPADC power down */
    POWER_DisablePD(kPDRUNCFG_PD_LDOGPADC);

    LPADC_GetDefaultConfig(&mLpadcConfigStruct);

    mLpadcConfigStruct.enableAnalogPreliminary = true;
    mLpadcConfigStruct.referenceVoltageSource = kLPADC_ReferenceVoltageAlt2;
    mLpadcConfigStruct.conversionAverageMode = kLPADC_ConversionAverage1;
    mLpadcConfigStruct.FIFO0Watermark = 2U;
    mLpadcConfigStruct.powerLevelMode = kLPADC_PowerLevelAlt4;
    mLpadcConfigStruct.powerUpDelay = 0U;

    /* Init ADC0 */
    LPADC_Init(ADC0, &mLpadcConfigStruct);

    /* Request gain calibration. */
    LPADC_DoAutoCalibration(ADC0);

    /* Request offset calibration */
    LPADC_DoOffsetCalibration(ADC0);

    /* ************************************************************************
     * FIFO0                            FIFO1                                 *
     * I_A      - ADC_00    P19-4       I_B     - ADC_08 P19-2                *
     * I_C      - ADC_02    P18-11                                            *
     * UDCBus   - ADC_04    P17-19                                            *
     **************************************************************************/

    /* Set conversion CMD1 configuration. */
    LPADC_GetDefaultConvCommandConfig(&mLpadcCommandConfigStruct);
    mLpadcCommandConfigStruct.channelNumber = 0U; /* Set ADC channel - 0 */ //SET ADC CHANNEL 0 and CHANNEL 8
    mLpadcCommandConfigStruct.chainedNextCommandNumber = 2U; /* Next execuited CMD will be CMD2 */
    mLpadcCommandConfigStruct.sampleChannelMode = kLPADC_SampleChannelDualSingleEndBothSide; /* two simultaneous conversions Channel A and Channel B */
    mLpadcCommandConfigStruct.hardwareAverageMode = kLPADC_HardwareAverageCount1;
    mLpadcCommandConfigStruct.sampleTimeMode = kLPADC_SampleTimeADCK3;
    mLpadcCommandConfigStruct.enableWaitTrigger = FALSE;
    LPADC_SetConvCommandConfig(ADC0, 1U, &mLpadcCommandConfigStruct);  /* Configure the CMD 1 */ //CONFIGURE CMD 1 (CMDL1, CMDH1)

    /* Set conversion CMD2 configuration. */
    LPADC_GetDefaultConvCommandConfig(&mLpadcCommandConfigStruct);
    mLpadcCommandConfigStruct.channelNumber = 2U; /* Set ADC channel - 2 */ //SET ADC CHANNEL 2
    mLpadcCommandConfigStruct.chainedNextCommandNumber = 3U; /* Next execuited CMD will be CMD3 */
    mLpadcCommandConfigStruct.sampleChannelMode = kLPADC_SampleChannelSingleEndSideA;
    mLpadcCommandConfigStruct.hardwareAverageMode = kLPADC_HardwareAverageCount1;
    mLpadcCommandConfigStruct.sampleTimeMode = kLPADC_SampleTimeADCK3;
    mLpadcCommandConfigStruct.enableWaitTrigger = FALSE;
    LPADC_SetConvCommandConfig(ADC0, 2U, &mLpadcCommandConfigStruct);  /* Configure the CMD 2 */ //CONFIGURE CMD 2 (CMDL2, CMDH2)

    /* Set conversion CMD3 configuration. */
    LPADC_GetDefaultConvCommandConfig(&mLpadcCommandConfigStruct);
    mLpadcCommandConfigStruct.channelNumber = 4U; /* Set ADC channel - 4 */ //SET ADC CHANNEL 4
    mLpadcCommandConfigStruct.chainedNextCommandNumber = 0U; /* This was last execuited command */
    mLpadcCommandConfigStruct.sampleChannelMode = kLPADC_SampleChannelSingleEndSideA;
    mLpadcCommandConfigStruct.hardwareAverageMode = kLPADC_HardwareAverageCount1;
    mLpadcCommandConfigStruct.sampleTimeMode = kLPADC_SampleTimeADCK3;
    mLpadcCommandConfigStruct.enableWaitTrigger = FALSE;
    LPADC_SetConvCommandConfig(ADC0, 3U, &mLpadcCommandConfigStruct);  /* Configure the CMD 3 */ //CONFIGURE CMD 3 (CMDL3, CMDH3)

    /* Set trigger configuration. */
    LPADC_GetDefaultConvTriggerConfig(&mLpadcTriggerConfigStruct);
    mLpadcTriggerConfigStruct.targetCommandId = 1U; /* CMD1 is executed. */ //SET CMD CALLED FROM TRIGGER
    mLpadcTriggerConfigStruct.enableHardwareTrigger = 1U; //HW trigger ON
    mLpadcTriggerConfigStruct.channelAFIFOSelect = 0U; //Channels A store to FIFO0
    mLpadcTriggerConfigStruct.channelBFIFOSelect = 1U; //Channels B store to FIFO1
    mLpadcTriggerConfigStruct.delayPower = 0U;
    mLpadcTriggerConfigStruct.priority = 0U; //Trigger priority highest

    LPADC_SetConvTriggerConfig(ADC0, 4U, &mLpadcTriggerConfigStruct); /* Configurate the trigger0. */ //CONFIGURE TRIGGER 4 - can be HW triggered from sct0_outputs[9]

    /* Offset filter window */
    g_sM1AdcSensor.ui16OffsetFiltWindow = ADC_OFFSET_WINDOW;


    /* Enable the watermark interrupt. */
    LPADC_EnableInterrupts(ADC0, kLPADC_FIFO0WatermarkInterruptEnable);
    EnableIRQ(ADC0_IRQn);
    NVIC_SetPriority(ADC0_IRQn, 1U);

}

/*!
* @brief   void InitCTMR2(void)
*           - Initialization of the CTMR2 peripheral
*           - Performs slow control loop counter
*
* @param   void
*
* @return  none
*/
void InitCTMR2(void)
{
    ctimer_config_t config;

    /* Use 96 MHz clock for some of the Ctimers */
    CLOCK_AttachClk(kFRO_HF_to_CTIMER2);
    /* Get defaut configuration */
    CTIMER_GetDefaultConfig(&config);
    /* Init timer */
    CTIMER_Init(CTIMER2, &config);

    /* Configuration */
    matchConfig0.enableCounterReset = true;
    matchConfig0.enableCounterStop = false;
    matchConfig0.matchValue = CTIMER_CLK_FREQ / g_sClockSetup.ui16M1SpeedLoopFreq;
    matchConfig0.outControl = kCTIMER_Output_NoAction;
    matchConfig0.outPinInitState = false;
    matchConfig0.enableInterrupt = true;

    CTIMER_SetupMatch(CTIMER2, kCTIMER_Match_1, &matchConfig0);
    CTIMER_StartTimer(CTIMER2);

    EnableIRQ(CTIMER2_IRQn);
    NVIC_SetPriority(CTIMER2_IRQn, 2U);
}

/*!
* @brief   void InitPWM_M1(void)
*           - Initialization of the eFlexPWMA peripheral for motor M1
*           - 3-phase center-aligned PWM
*
* @param   void
*
* @return  none
*/
void M1_InitPWM(void)
{
    /* Initialize MC driver */
    g_sM1Pwm3ph.pui32PwmBaseAddress = (SCT_Type *)SCT0;
    g_sM1Pwm3ph.updatedDutycycle = 50U;
    g_sM1Pwm3ph.eventNumberOutput0 = 0U;
    g_sM1Pwm3ph.eventNumberOutput1 = 0U;
    g_sM1Pwm3ph.eventNumberOutput2 = 0U;
    g_sM1Pwm3ph.eventNumberOutput3 = 0U;
    g_sM1Pwm3ph.eventNumberOutput4 = 0U;
    g_sM1Pwm3ph.eventNumberOutput5 = 0U;

    /* Set deadtime */
    g_sM1Pwm3ph.ui16DeadTimePWM = 37; // 1 puls is 13.33ns (1/(150MHz/2)), 37*13.33ns = 493.21ns

    g_sM1Pwm3ph.ui16FaultFixNum = M1_FAULT_NUM; /* PWMA fixed-value over-current fault number */
    g_sM1Pwm3ph.ui16FaultAdjNum = M1_FAULT_NUM; /* PWMA adjustable over-current fault number */

    /* Set clock source for SCTimer */
    CLOCK_SetClkDiv(kCLOCK_DivSctClk, 0U, true); // main_clk divided by 1
    CLOCK_AttachClk(kMAIN_CLK_to_SCT_CLK); /* Clocked from main_clk */

    sctimerClock = CLOCK_GetFreq(kCLOCK_CoreSysClk);

    SCTIMER_GetDefaultConfig(&sctimerInfo);

    /* Initialize SCTimer module */
    SCTIMER_Init(g_sM1Pwm3ph.pui32PwmBaseAddress, &sctimerInfo);

    //AT P18-5
    /* Configure PWM params with frequency 10kHZ from output */
    pwmParam.output = kSCTIMER_Out_0;  //kSCTIMER_Out_0
    pwmParam.level = kSCTIMER_LowTrue;
    pwmParam.dutyCyclePercent = g_sM1Pwm3ph.updatedDutycycle + 1U;
    SCTIMER_SetupPwm(g_sM1Pwm3ph.pui32PwmBaseAddress, &pwmParam, kSCTIMER_CenterAlignedPwm, g_sClockSetup.ui16M1PwmFreq, sctimerClock, &g_sM1Pwm3ph.eventNumberOutput0);

    //AB P18-20
    /* Configure PWM params with frequency 10kHZ from output */
    pwmParam.output = kSCTIMER_Out_1;  //kSCTIMER_Out_1
    pwmParam.level = kSCTIMER_HighTrue;
    pwmParam.dutyCyclePercent = g_sM1Pwm3ph.updatedDutycycle - 1U;
    SCTIMER_SetupPwm(g_sM1Pwm3ph.pui32PwmBaseAddress, &pwmParam, kSCTIMER_CenterAlignedPwm, g_sClockSetup.ui16M1PwmFreq, sctimerClock, &g_sM1Pwm3ph.eventNumberOutput1);

    //BT P18-6
    /* Configure PWM params with frequency 10kHZ from output */
    pwmParam.output = kSCTIMER_Out_2;  //kSCTIMER_Out_2
    pwmParam.level = kSCTIMER_LowTrue;
    pwmParam.dutyCyclePercent = g_sM1Pwm3ph.updatedDutycycle + 1U;
    SCTIMER_SetupPwm(g_sM1Pwm3ph.pui32PwmBaseAddress, &pwmParam, kSCTIMER_CenterAlignedPwm, g_sClockSetup.ui16M1PwmFreq, sctimerClock, &g_sM1Pwm3ph.eventNumberOutput2);

    //BB P18-3
    /* Configure PWM params with frequency 10kHZ from output */
    pwmParam.output = kSCTIMER_Out_3;  //kSCTIMER_Out_3
    pwmParam.level = kSCTIMER_HighTrue;
    pwmParam.dutyCyclePercent = g_sM1Pwm3ph.updatedDutycycle - 1U;
    SCTIMER_SetupPwm(g_sM1Pwm3ph.pui32PwmBaseAddress, &pwmParam, kSCTIMER_CenterAlignedPwm, g_sClockSetup.ui16M1PwmFreq, sctimerClock, &g_sM1Pwm3ph.eventNumberOutput3);

    //CT P18-19
    /* Configure PWM params with frequency 10kHZ from output */
    pwmParam.output = kSCTIMER_Out_4;  //kSCTIMER_Out_4
    pwmParam.level = kSCTIMER_LowTrue;
    pwmParam.dutyCyclePercent = g_sM1Pwm3ph.updatedDutycycle + 1U;
    SCTIMER_SetupPwm(g_sM1Pwm3ph.pui32PwmBaseAddress, &pwmParam, kSCTIMER_CenterAlignedPwm, g_sClockSetup.ui16M1PwmFreq, sctimerClock, &g_sM1Pwm3ph.eventNumberOutput4);

    //CB P17-13
    /* Configure PWM params with frequency 10kHZ from output */
    pwmParam.output = kSCTIMER_Out_5;  //kSCTIMER_Out_5
    pwmParam.level = kSCTIMER_HighTrue;
    pwmParam.dutyCyclePercent = g_sM1Pwm3ph.updatedDutycycle - 1U;
    SCTIMER_SetupPwm(g_sM1Pwm3ph.pui32PwmBaseAddress, &pwmParam, kSCTIMER_CenterAlignedPwm, g_sClockSetup.ui16M1PwmFreq, sctimerClock, &g_sM1Pwm3ph.eventNumberOutput5);

    //TRIGGER SETUP (sct0_output[9])
    pwmParam.output = kSCTIMER_Out_9;  //kSCTIMER_Out_9
    pwmParam.level = kSCTIMER_HighTrue;
    SCTIMER_SetupPwm(g_sM1Pwm3ph.pui32PwmBaseAddress, &pwmParam, kSCTIMER_CenterAlignedPwm, g_sClockSetup.ui16M1PwmFreq, sctimerClock, &g_sM1Pwm3ph.eventNumberOutput6);

    SCT0->MATCH[13] = SCT_MATCH_MATCHn_L(1);
    SCT0->MATCHREL[13] = SCT_MATCHREL_RELOADn_L(25); //Trigger duration

    /* PWM Faults */
    SCT0->EV[14].STATE = 0x01; //Enable event 14
    SCT0->EV[14].CTRL |= SCT_EV_CTRL_DIRECTION(0x0); //This event is triggered regardless of the count direction
    SCT0->EV[14].CTRL |= SCT_EV_CTRL_IOCOND(0x1);    //Input condition - high //0x1
    SCT0->EV[14].CTRL |= SCT_EV_CTRL_OUTSEL(0x0);    //Selects the inputs selected by IOSEL
    SCT0->EV[14].CTRL |= SCT_EV_CTRL_IOSEL(0x0);     //input 0 (mapped from CMP)
    SCT0->EV[14].CTRL |= SCT_EV_CTRL_COMBMODE(0x2);

    SCT0->EVEN = 0x4000; //IRQ initiated from CMP

    MCDRV_scTmrPwm3PhOutInit(&g_sM1Pwm3ph);//INITIALIZE PWM STRUCTURE
    MCDRV_scTmrPwm3PhOutDis(&g_sM1Pwm3ph); //DISABLE PWM OUTPUT

    EnableIRQ(SCT0_IRQn);
    NVIC_SetPriority(SCT0_IRQn, 0U);

}


/*!
@brief   void InitCMP(void)
          - Initialization of the comparator module for dc-bus over current
            detection

@param   void

@return  none
*/
void InitCMP(void)
{
    cmp_config_t config;
    cmp_vref_config_t configVref;

    /* Enable the clock */
    CLOCK_EnableClock(kCLOCK_Comp);

    /* Enable the power to the analog comparator */
    PMC->PDRUNCFG0 &= ~PMC_PDRUNCFG0_PDEN_COMP_MASK;

    /* Initialize COMP */
    config.enableHysteresis = false;
    config.enableLowPower = false;

    /* CMP initialization */
    CMP_Init(&config);

    /* Set the input channels for the comparator. PositiveChannel 1, NegativeChannel 0 */
    CMP_SetInputChannels(1U, 0U);

    configVref.vrefSource = KCMP_VREFSourceVDDA;
    configVref.vrefValue = 0x1F;

    /* Configures the VREFINPUT. */
    CMP_SetVREF(&configVref);

    /* Enable CMP interrupt. */
    CMP_EnableInterrupt(kCMP_LevelHigh);

    /* Clear interrupt */
    CMP_ClearInterrupt();

}
