/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017, 2019-2020 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#include "TMR_Adapter.h"
#include "fsl_device_registers.h"
#include "fsl_os_abstraction.h"
#include "fsl_common.h"
#include "board.h"

#if defined(FSL_FEATURE_SOC_FTM_COUNT) && FSL_FEATURE_SOC_FTM_COUNT
  #include "fsl_ftm.h"
#elif defined(FSL_FEATURE_RTC_HAS_FRC) && FSL_FEATURE_RTC_HAS_FRC
  #include "fsl_rtc.h"
  #include "clock_config.h"
#elif defined(FSL_FEATURE_SOC_CTIMER_COUNT) && FSL_FEATURE_SOC_CTIMER_COUNT
  #include "fsl_ctimer.h"
#else
   #include "fsl_tpm.h"
#endif


/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
#if defined(FSL_FEATURE_SOC_FTM_COUNT) && FSL_FEATURE_SOC_FTM_COUNT
static const IRQn_Type mFtmIrqId[] = FTM_IRQS;
static FTM_Type * mFtmBase[] = FTM_BASE_PTRS;
static const ftm_config_t mFtmConfig = {
    .prescale = kFTM_Prescale_Divide_128,
    .bdmMode = kFTM_BdmMode_0,
    .pwmSyncMode = kFTM_SoftwareTrigger,
    .reloadPoints = 0,
    .faultMode = kFTM_Fault_Disable,
    .faultFilterValue = 0,
    .deadTimePrescale = kFTM_Deadtime_Prescale_1,
    .deadTimeValue = 0,
    .extTriggers = 0,
    .chnlInitState = 0,
    .chnlPolarity = 0,
    .useGlobalTimeBase = 0
};

#elif defined(FSL_FEATURE_RTC_HAS_FRC) && FSL_FEATURE_RTC_HAS_FRC
static const IRQn_Type  mRtcFrIrqId = RTC_FR_IRQn;
static       RTC_Type   *mRtcBase[] = RTC_BASE_PTRS;

#elif defined(FSL_FEATURE_SOC_CTIMER_COUNT) && FSL_FEATURE_SOC_CTIMER_COUNT
static const IRQn_Type          mCtimerIrqId[]  = CTIMER_IRQS;
static CTIMER_Type              *mCtimerBase[]  = CTIMER_BASE_PTRS;
static const ctimer_config_t    mCtimerConfig[FSL_FEATURE_SOC_CTIMER_COUNT]
                                                = {{.mode =  kCTIMER_TimerMode,
                                                    .input = kCTIMER_Capture_0,
                                                    .prescale = 0},
                                                   {.mode =  kCTIMER_TimerMode,
                                                    .input = kCTIMER_Capture_0,
                                                    .prescale = 0},
                                                   {.mode =  kCTIMER_TimerMode,
                                                    .input = kCTIMER_Capture_0,
                                                    .prescale = 0},
                                                   {.mode =  kCTIMER_TimerMode,
                                                    .input = kCTIMER_Capture_0,
                                                    .prescale = 0}
};
static ctimer_match_config_t mCtimerMatchConfig = {.enableCounterReset = true,
                                                   .enableCounterStop = true,
                                                   .matchValue = 0xff,
                                                   .outControl = kCTIMER_Output_NoAction,
                                                   .outPinInitState = false,
                                                   .enableInterrupt = true};

/*! @brief List of Timer Match channel interrupts,
 * this can be accessed using the channel number as index*/
static const ctimer_interrupt_enable_t ctimer_match_ch_interrupts[] = {
    kCTIMER_Match0InterruptEnable,
    kCTIMER_Match1InterruptEnable,
    kCTIMER_Match2InterruptEnable,
    kCTIMER_Match3InterruptEnable,
};

#else
static const IRQn_Type mTpmIrqId[] = TPM_IRQS;
static TPM_Type * mTpmBase[] = TPM_BASE_PTRS;
static const tpm_config_t mTpmConfig = {
    .prescale = kTPM_Prescale_Divide_128,
    .useGlobalTimeBase = FALSE,
    .enableDoze = FALSE,
    .enableDebugMode = FALSE,
    .enableReloadOnTrigger = FALSE,
    .enableStopOnOverflow = FALSE,
    .enableStartOnTrigger = FALSE,
    .triggerSelect = kTPM_Trigger_Select_0
};
#endif


/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
void StackTimer_Init(void (*cb)(void))
{
    IRQn_Type irqId;
#if defined(FSL_FEATURE_SOC_FTM_COUNT) && FSL_FEATURE_SOC_FTM_COUNT
    FTM_Type *ftmBaseAddr = (FTM_Type*)mFtmBase[gStackTimerInstance_c];

    FTM_Init(ftmBaseAddr, &mFtmConfig);
    FTM_StopTimer(ftmBaseAddr);
    ftmBaseAddr->MOD = 0xFFFF;
    /* Configure channel to Software compare; output pin not used */
    FTM_SetupOutputCompare(ftmBaseAddr, (ftm_chnl_t)gStackTimerChannel_c, kFTM_NoOutputSignal, 0x01);

    /* Install ISR */
    irqId = mFtmIrqId[gStackTimerInstance_c];
    FTM_EnableInterrupts(ftmBaseAddr, kFTM_TimeOverflowInterruptEnable |  (1 << gStackTimerChannel_c));
#elif defined(FSL_FEATURE_RTC_HAS_FRC) && FSL_FEATURE_RTC_HAS_FRC
    RTC_Init(RTC);
    /* Enable RTC free running interrupt */
    RTC_EnableInterrupts(RTC, kRTC_FreeRunningInterruptEnable);
    irqId = mRtcFrIrqId;
#elif defined(FSL_FEATURE_SOC_CTIMER_COUNT) && FSL_FEATURE_SOC_CTIMER_COUNT
    CTIMER_Type *ctimerBaseAddr = mCtimerBase[gStackTimerInstance_c];

    CTIMER_Init(ctimerBaseAddr, &mCtimerConfig[gStackTimerInstance_c]);
    CTIMER_StopTimer(ctimerBaseAddr);

    /* Configure channel to Software compare; output pin not used */
    CTIMER_SetupMatch(ctimerBaseAddr, (ctimer_match_t)gStackTimerChannel_c, &mCtimerMatchConfig);

    /* Install ISR */
    irqId = mCtimerIrqId[gStackTimerInstance_c];
    CTIMER_EnableInterrupts(ctimerBaseAddr, ctimer_match_ch_interrupts[gStackTimerChannel_c]);
#else
    TPM_Type *tpmBaseAddr = (TPM_Type*)mTpmBase[gStackTimerInstance_c];

    TPM_Init(tpmBaseAddr, &mTpmConfig);
    TPM_StopTimer(tpmBaseAddr);

    /* Set the timer to be in free-running mode */
    tpmBaseAddr->MOD = 0xFFFF;
    /* Configure channel to Software compare; output pin not used */
    TPM_SetupOutputCompare(tpmBaseAddr, (tpm_chnl_t)gStackTimerChannel_c, kTPM_NoOutputSignal, 0x01);
    
    /* Install ISR */
    irqId = mTpmIrqId[gStackTimerInstance_c];
    TPM_EnableInterrupts(tpmBaseAddr, (uint32_t)kTPM_TimeOverflowInterruptEnable | (1U << gStackTimerChannel_c));
#endif
    /* Overwrite old ISR, else leave the default one. */
    if (cb != NULL)
    {
        OSA_InstallIntHandler((uint32_t)irqId, cb);
    }
    /* set interrupt priority */
    NVIC_SetPriority(irqId, (uint32_t)gStackTimer_IsrPrio_c >> (8 - __NVIC_PRIO_BITS));
    NVIC_ClearPendingIRQ(irqId);
    NVIC_EnableIRQ(irqId);
}

/*************************************************************************************/
void StackTimer_Enable(void)
{
#if defined(FSL_FEATURE_SOC_FTM_COUNT) && FSL_FEATURE_SOC_FTM_COUNT
    FTM_StartTimer(mFtmBase[gStackTimerInstance_c], kFTM_SystemClock);
#elif defined(FSL_FEATURE_RTC_HAS_FRC) && FSL_FEATURE_RTC_HAS_FRC
    RTC_FreeRunningEnable(mRtcBase[0], true);
#elif defined(FSL_FEATURE_SOC_CTIMER_COUNT) && FSL_FEATURE_SOC_CTIMER_COUNT
    CTIMER_StartTimer(mCtimerBase[gStackTimerInstance_c]);
#else
    TPM_StartTimer(mTpmBase[gStackTimerInstance_c], kTPM_SystemClock);
#endif
}

/*************************************************************************************/
void StackTimer_Disable(void)
{
#if defined(FSL_FEATURE_SOC_FTM_COUNT) && FSL_FEATURE_SOC_FTM_COUNT
    FTM_StopTimer(mFtmBase[gStackTimerInstance_c]);
#elif defined(FSL_FEATURE_RTC_HAS_FRC) && FSL_FEATURE_RTC_HAS_FRC
    RTC_FreeRunningEnable(mRtcBase[0], false);
#elif defined(FSL_FEATURE_SOC_CTIMER_COUNT) && FSL_FEATURE_SOC_CTIMER_COUNT
    CTIMER_StopTimer(mCtimerBase[gStackTimerInstance_c]);
#else
    TPM_StopTimer(mTpmBase[gStackTimerInstance_c]);
#endif
}

/*************************************************************************************/
IRQn_Type StackTimer_GetIrqId(void)
{
    IRQn_Type irqId;
#if defined(FSL_FEATURE_SOC_FTM_COUNT) && FSL_FEATURE_SOC_FTM_COUNT
    irqId = mFtmIrqId[gStackTimerInstance_c];
#elif defined(FSL_FEATURE_RTC_HAS_FRC) && FSL_FEATURE_RTC_HAS_FRC
    irqId = mRtcFrIrqId;
#elif defined(FSL_FEATURE_SOC_CTIMER_COUNT) && FSL_FEATURE_SOC_CTIMER_COUNT
    irqId = mCtimerIrqId[gStackTimerInstance_c];
#else
    irqId = mTpmIrqId[gStackTimerInstance_c];
#endif
    return irqId;
}

/*************************************************************************************/
uint32_t StackTimer_GetInputFrequency(void)
{
    uint32_t prescaller = 0;
    uint32_t refClk     = 0;
    uint32_t result     = 0;
#if defined(FSL_FEATURE_SOC_FTM_COUNT) && FSL_FEATURE_SOC_FTM_COUNT
    refClk = BOARD_GetFtmClock(gStackTimerInstance_c);
    prescaller = mFtmConfig.prescale;
    result = refClk / (1 << prescaller);
#elif defined(FSL_FEATURE_RTC_HAS_FRC) && FSL_FEATURE_RTC_HAS_FRC
    
    (void)prescaller; /* unused variables  */
    (void)refClk;     /* suppress warnings */
  #if (defined(BOARD_XTAL1_CLK_HZ) && (BOARD_XTAL1_CLK_HZ == CLK_XTAL_32KHZ))
    result = CLOCK_GetFreq(kCLOCK_32KClk);  //32,768Khz crystal is used
  #else
    if( RTC->CTRL | RTC_CTRL_CAL_EN_MASK) // is calibration enabled ?
    { 
        /* result = 32000 +- ( (32768-32000)*(calculated_ppm / ppm_for_32_000))
         *        = 32000 +- ( 768 * calculated_ppm / 0x6000 )
         *        = 32000 +- (3 * calculated_ppm / 0x60)
         */
        if (RTC->CAL & RTC_CAL_DIR_MASK) //backward calibration
        {
            result = 32000U - ((3 * (RTC->CAL & RTC_CAL_PPM_MASK)) / 0x60);
        }
        else
        {
            result = 32000U + ((3 * (RTC->CAL & RTC_CAL_PPM_MASK)) / 0x60);
        }
    }
    else
    {
        result = CLOCK_GetFreq(kCLOCK_32KClk);  //32,000Khz internal RCO is used
    }
  #endif
#elif defined(FSL_FEATURE_SOC_CTIMER_COUNT) && FSL_FEATURE_SOC_CTIMER_COUNT
    refClk = BOARD_GetCtimerClock(mCtimerBase[gStackTimerInstance_c]);
    prescaller = mCtimerConfig[gStackTimerInstance_c].prescale;
    result = refClk / (prescaller + 1);
#else
    refClk = BOARD_GetTpmClock(gStackTimerInstance_c);
    prescaller = (uint32_t)mTpmConfig.prescale;
    result = refClk / (uint32_t)(1UL << prescaller);
#endif
    return result;
}

/*************************************************************************************/
uint32_t StackTimer_GetCounterValue(void)
{
    uint32_t counter_value = 0;
#if defined(FSL_FEATURE_SOC_FTM_COUNT) && FSL_FEATURE_SOC_FTM_COUNT
    counter_value = mFtmBase[gStackTimerInstance_c]->CNT;
#elif defined(FSL_FEATURE_RTC_HAS_FRC) && FSL_FEATURE_RTC_HAS_FRC
    counter_value = RTC_GetFreeRunningCount(mRtcBase[0]);
#elif defined(FSL_FEATURE_SOC_CTIMER_COUNT) && FSL_FEATURE_SOC_CTIMER_COUNT
    counter_value = mCtimerBase[gStackTimerInstance_c]->TC;
#else
    counter_value = mTpmBase[gStackTimerInstance_c]->CNT;
#endif
    return counter_value;
}

/*************************************************************************************/
void StackTimer_SetOffsetTicks(uint32_t offset)
{
#if defined(FSL_FEATURE_SOC_FTM_COUNT) && FSL_FEATURE_SOC_FTM_COUNT
    FTM_SetupOutputCompare(mFtmBase[gStackTimerInstance_c], (ftm_chnl_t)gStackTimerChannel_c, kFTM_NoOutputSignal, offset);
#elif defined(FSL_FEATURE_RTC_HAS_FRC) && FSL_FEATURE_RTC_HAS_FRC
    RTC_SetFreeRunningInterruptThreshold(mRtcBase[0], offset);
#elif defined(FSL_FEATURE_SOC_CTIMER_COUNT) && FSL_FEATURE_SOC_CTIMER_COUNT
    mCtimerMatchConfig.matchValue = offset;
    CTIMER_SetupMatch(mCtimerBase[gStackTimerInstance_c], (ctimer_match_t)gStackTimerInstance_c, &mCtimerMatchConfig);
#else
    TPM_SetupOutputCompare(mTpmBase[gStackTimerInstance_c], (tpm_chnl_t)gStackTimerChannel_c, kTPM_NoOutputSignal, offset);
#endif
}

/*************************************************************************************/
void StackTimer_ClearIntFlag(void)
{
    uint32_t flags = 0;
#if defined(FSL_FEATURE_SOC_FTM_COUNT) && FSL_FEATURE_SOC_FTM_COUNT
    flags = FTM_GetStatusFlags(mFtmBase[gStackTimerInstance_c]);
    FTM_ClearStatusFlags(mFtmBase[gStackTimerInstance_c], flags);
#elif defined(FSL_FEATURE_RTC_HAS_FRC) && FSL_FEATURE_RTC_HAS_FRC
    flags = RTC_STATUS_FREE_RUNNING_INT_MASK;
    RTC_ClearStatusFlags(mRtcBase[0], flags);
#elif defined(FSL_FEATURE_SOC_CTIMER_COUNT) && FSL_FEATURE_SOC_CTIMER_COUNT
    flags = CTIMER_GetStatusFlags(mCtimerBase[gStackTimerInstance_c]);
    CTIMER_ClearStatusFlags(mCtimerBase[gStackTimerInstance_c], flags);
#else
    flags = TPM_GetStatusFlags(mTpmBase[gStackTimerInstance_c]);
    TPM_ClearStatusFlags(mTpmBase[gStackTimerInstance_c], flags);
#endif
}

/*************************************************************************************/
/*                                       PWM                                         */
/*************************************************************************************/
void PWM_Init(uint8_t instance)
{
#if defined(FSL_FEATURE_SOC_FTM_COUNT) && FSL_FEATURE_SOC_FTM_COUNT
    ftm_config_t config;
    FTM_GetDefaultConfig(&config);
    FTM_Init(mFtmBase[instance], &config);
    /* Enable TPM compatibility. Free running counter and synchronization compatible with TPM */
    mFtmBase[instance]->MODE &= ~(FTM_MODE_FTMEN_MASK);
    FTM_StartTimer(mFtmBase[instance], kFTM_SystemClock);
#elif defined(FSL_FEATURE_RTC_HAS_FRC) && FSL_FEATURE_RTC_HAS_FRC
    // do nothing, RTC has no PWM
#elif defined(FSL_FEATURE_SOC_CTIMER_COUNT) && FSL_FEATURE_SOC_CTIMER_COUNT
    ctimer_config_t config;
    CTIMER_GetDefaultConfig(&config);
    CTIMER_Init(mCtimerBase[gStackTimerInstance_c], &config);
    CTIMER_StartTimer(mCtimerBase[gStackTimerInstance_c]);
#else
    tpm_config_t config;
    TPM_GetDefaultConfig(&config);
    TPM_Init(mTpmBase[instance], &config);
    TPM_StartTimer(mTpmBase[instance], kTPM_SystemClock);
#endif
}

/*************************************************************************************/
void PWM_SetChnCountVal(uint8_t instance, uint8_t channel, tmrTimerTicks_t val)
{
#if defined(FSL_FEATURE_SOC_FTM_COUNT) && FSL_FEATURE_SOC_FTM_COUNT
    mFtmBase[instance]->CONTROLS[channel].CnV = val;
#elif defined(FSL_FEATURE_RTC_HAS_FRC) && FSL_FEATURE_RTC_HAS_FRC
    // do nothing, RTC has no PWM
#elif defined(FSL_FEATURE_SOC_CTIMER_COUNT) && FSL_FEATURE_SOC_CTIMER_COUNT
    mCtimerBase[gStackTimerInstance_c]->MR[channel] = val;
#else
    do
    {
        mTpmBase[instance]->CONTROLS[channel].CnV = val;
    }while (val != mTpmBase[instance]->CONTROLS[channel].CnV);
#endif
}

/*************************************************************************************/
tmrTimerTicks_t PWM_GetChnCountVal(uint8_t instance, uint8_t channel)
{
    tmrTimerTicks_t value = 0;
#if defined(FSL_FEATURE_SOC_FTM_COUNT) && FSL_FEATURE_SOC_FTM_COUNT
    value = mFtmBase[instance]->CONTROLS[channel].CnV;
#elif defined(FSL_FEATURE_RTC_HAS_FRC) && FSL_FEATURE_RTC_HAS_FRC
    // do nothing, RTC has no PWM
#elif defined(FSL_FEATURE_SOC_CTIMER_COUNT) && FSL_FEATURE_SOC_CTIMER_COUNT
    value =  mCtimerBase[gStackTimerInstance_c]->MR[channel];
#else
    value = (tmrTimerTicks_t)mTpmBase[instance]->CONTROLS[channel].CnV;
#endif
    return value;
}

#if !defined(FSL_FEATURE_SOC_CTIMER_COUNT) || (FSL_FEATURE_SOC_CTIMER_COUNT == 0)
/* For QN9080 CTimer pwm output always starts low and rises on compare match */
/*************************************************************************************/
void PWM_StartEdgeAlignedLowTrue(uint8_t instance, tmr_adapter_pwm_param_t *param, uint8_t channel)
{
#if defined(FSL_FEATURE_SOC_FTM_COUNT) && FSL_FEATURE_SOC_FTM_COUNT
    ftm_chnl_pwm_signal_param_t pwmChannelConfig = {
        .chnlNumber = (ftm_chnl_t)channel,
        .level = kFTM_LowTrue,
        .dutyCyclePercent = param->initValue,
        .firstEdgeDelayPercent = 0
    };
    
    FTM_SetupPwm(mFtmBase[instance], &pwmChannelConfig, 1, kFTM_EdgeAlignedPwm, param->frequency, BOARD_GetFtmClock(instance));
#else
    tpm_chnl_pwm_signal_param_t pwmChannelConfig = {
        .chnlNumber = (tpm_chnl_t)channel,
        .level = (tpm_pwm_level_select_t)kTPM_LowTrue,
        .dutyCyclePercent = (uint8_t)param->initValue,
#if defined(FSL_FEATURE_TPM_HAS_COMBINE) && FSL_FEATURE_TPM_HAS_COMBINE
        .firstEdgeDelayPercent = 0
#endif
    };
    
    (void)TPM_SetupPwm(mTpmBase[instance], &pwmChannelConfig, 1, kTPM_EdgeAlignedPwm, param->frequency, BOARD_GetTpmClock(instance));
#endif
}
#endif
