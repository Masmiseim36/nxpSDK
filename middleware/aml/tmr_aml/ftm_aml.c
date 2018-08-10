/*
 * Copyright (c) 2016 - 2018, NXP Semiconductors, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 * @file ftm_aml.c
 *
 * FTM driver intended for S32 SDK and MCUXpresso SDK.
 * This driver creates abstraction layer for FTM peripheral.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "tmr_aml.h"
#if (SDK_VERSION == SDK_KSDK)

#include "fsl_common.h"

#elif (SDK_VERSION == SDK_S32)

#endif
#include "ftm_aml.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Note: this source file is intended for FTM peripheral only. */
#if (TMR_AML_TYPE == TMR_AML_FTM)

#if (SDK_VERSION == SDK_KSDK)

/*! @brief Number of timer devices present on the MCU. */
#define TMR_AML_DEV_CNT             FSL_FEATURE_SOC_FTM_COUNT

#elif(SDK_VERSION == SDK_S32)

/*! @brief Number of timer devices present on the MCU. */
#define TMR_AML_DEV_CNT             FTM_INSTANCE_COUNT

#endif

/*! @brief Max. value of the counter. */
#define TMR_CNT_MAX                 65535U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Updates SDK specific configuration of a timer channel according
 * to AML configuration (SDK independent). This function copies PWM mode
 * settings only.
 *
 * @param amlChConf AML configuration of a channel for PWM mode.
 * @param sdkChConf SDK specific configuration of a channel for PWM mode.
 *
 * @return          Error code (staERR_OK when no error occurred).
 */
static status_t TMR_AML_UpdatePWMChnlConf(
        const tmr_aml_pwm_ch_t *amlChConf,
        tmr_sdk_pwm_ch_config_t *sdkChConf);

/*!
 * @brief Updates SDK specific configuration of a timer channel according
 * to AML configuration (SDK independent). This function copies Output
 * compare mode settings only.
 *
 * @param amlChConf AML configuration of a channel for PWM mode.
 * @param sdkChConf SDK specific configuration of a channel for PWM mode.
 *
 * @return          Error code (staERR_OK when no error occurred).
 */
static status_t TMR_AML_UpdateOutCmpChnlConf(
        const tmr_aml_outcmp_ch_t *amlChConf,
        tmr_sdk_outcmp_ch_t *sdkChConf);

/*!
 * @brief Updates SDK specific configuration of a timer channel according
 * to AML configuration (SDK independent). This function copies Output
 * compare mode settings only.
 *
 * @param amlChConf AML configuration of a channel for PWM mode.
 * @param sdkChConf SDK specific configuration of a channel for PWM mode.
 *
 * @return          Error code (staERR_OK when no error occurred).
 */
static status_t TMR_AML_UpdatePWMConf(const tmr_aml_config_t *amlConf,
        tmr_sdk_pwm_t *sdkPwmConf);

/*!
 * @brief Updates SDK specific configuration of Output compare mode according
 * to AML configuration (SDK independent).
 *
 * @param amlChConf AML configuration of a channel for PWM mode.
 * @param sdkChConf SDK specific configuration of a channel for PWM mode.
 *
 * @return          Error code (staERR_OK when no error occurred).
 */
static status_t TMR_AML_UpdateOutCmpConf(const tmr_aml_config_t *amlConf,
        ftm_output_cmp_param_t *sdkCmpConf);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief Device data structure. */
static tmr_data_t g_tmrData[TMR_AML_DEV_CNT];

#if (SDK_VERSION == SDK_KSDK)

/*! @brief Pointers to FTM bases for each instance (needed in KSDK only). */
static FTM_Type *const g_ftmBase[] = FTM_BASE_PTRS;  // Base address pointers.

#else
/*S32 SDK FlexTimer driver PWM Fault store variable. */
static ftm_pwm_fault_param_t g_faultPwm;
ftm_state_t g_ftmState[FTM_INSTANCE_COUNT];              /*!< State structure. */

#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : TMR_AML_UpdatePWMChnlConf
 * Description   : Updates SDK specific configuration of a timer channel
 * according to AML configuration (SDK independent). This function copies
 * PWM mode settings only.
 *
 *END**************************************************************************/
static status_t TMR_AML_UpdatePWMChnlConf(
        const tmr_aml_pwm_ch_t *amlChConf,
        tmr_sdk_pwm_ch_config_t *sdkChConf)
{
    status_t status = kStatus_Success;

    /* Preconditions. */
    AML_ASSERT(amlChConf != NULL);
    AML_ASSERT(sdkChConf != NULL);

#if (SDK_VERSION == SDK_KSDK)

    /* Update the configuration. */
    sdkChConf->chnlNumber = (ftm_chnl_t)amlChConf->channel;
    sdkChConf->dutyCyclePercent = amlChConf->dutyPercent;
    sdkChConf->firstEdgeDelayPercent = 0U;
    switch (amlChConf->pwmAction)
    {
        case tmrPwmActNoSignal:
            sdkChConf->level = kFTM_NoPwmSignal;
            break;
        case tmrPwmActLowTrue:
            sdkChConf->level = kFTM_LowTrue;
            break;
        case tmrPwmActHighTrue:
            sdkChConf->level = kFTM_HighTrue;
            break;
        default:
            AML_ASSERT("Invalid value of PWM action");
            status = kStatus_Fail;
            break;
    }

#elif (SDK_VERSION == SDK_S32)

    /* Update the configuration. */
    sdkChConf->hwChannelId = amlChConf->channel;
    sdkChConf->uDutyCyclePercent = (amlChConf->dutyPercent * 0x8000U) / 100U;
    switch (amlChConf->pwmAction)
    {
        case tmrPwmActHighTrue:
            sdkChConf->polarity = FTM_POLARITY_HIGH;
            break;

        case tmrPwmActLowTrue:
            sdkChConf->polarity = FTM_POLARITY_LOW;
            break;

        default:
            status = kStatus_Fail;
            break;
    }

#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TMR_AML_UpdateOutCmpChnlConf
 * Description   : Updates SDK specific configuration of a timer channel
 * according to AML configuration (SDK independent). This function copies
 * Output compare mode settings only.
 *
 *END**************************************************************************/
static status_t TMR_AML_UpdateOutCmpChnlConf(
        const tmr_aml_outcmp_ch_t *amlChConf,
        tmr_sdk_outcmp_ch_t *sdkChConf)
{
    status_t status = kStatus_Success;

    /* Preconditions. */
    AML_ASSERT(amlChConf != NULL);
    AML_ASSERT(sdkChConf != NULL);

#if (SDK_VERSION == SDK_KSDK)

    /* Update the configuration. */
    sdkChConf->chnlNumber = (ftm_chnl_t)amlChConf->channel;
    sdkChConf->compareValue = amlChConf->compareVal;
    switch (amlChConf->cmpAction)
    {
        case tmrOutActNoSignal:
            sdkChConf->mode = kFTM_NoOutputSignal;
            break;
        case tmrOutActToggleOnMatch:
            sdkChConf->mode = kFTM_ToggleOnMatch;
            break;
        case tmrOutActClearOnMatch:
            sdkChConf->mode = kFTM_ClearOnMatch;
            break;
        case tmrOutActSetOnMatch:
            sdkChConf->mode = kFTM_SetOnMatch;
            break;
        default:
            AML_ASSERT("Invalid value of output compare action");
            status = kStatus_Fail;
            break;
    }

#elif (SDK_VERSION == SDK_S32)

    /* Update the configuration. */
    sdkChConf->hwChannelId = amlChConf->channel;
    sdkChConf->comparedValue = amlChConf->compareVal;
    switch (amlChConf->cmpAction)
    {
        case tmrOutActNoSignal:
            sdkChConf->chMode = FTM_DISABLE_OUTPUT;
            break;

        case tmrOutActToggleOnMatch:
            sdkChConf->chMode = FTM_TOGGLE_ON_MATCH;
            break;

        case tmrOutActClearOnMatch:
            sdkChConf->chMode = FTM_CLEAR_ON_MATCH;
            break;

        case tmrOutActSetOnMatch:
            sdkChConf->chMode = FTM_SET_ON_MATCH;
            break;

        default:
            status = kStatus_Fail;
            break;
    }

#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TMR_AML_UpdatePWMConf
 * Description   : Updates SDK specific configuration of all timer channels
 * configured for PWM mode.
 *
 *END**************************************************************************/
static status_t TMR_AML_UpdatePWMConf(const tmr_aml_config_t *amlConf,
        tmr_sdk_pwm_t *sdkPwmConf)
{
    tmr_aml_pwm_ch_t   *chPwmAml;           /* AML channel configuration for PWM
                                               mode (SDK independent). */
    tmr_sdk_pwm_ch_config_t *chPwmSdk;      /* SDK specific Channel configuration
                                               for PWM mode. */
    uint8_t             i;
    status_t            status = kStatus_Success;

    AML_ASSERT(amlConf != NULL);
    AML_ASSERT(sdkPwmConf != NULL);
    /* Size of the array for PWM channels must be the same in AML structure
     * and SDK specific structure. */

#if (SDK_VERSION == SDK_KSDK)

    /* Default is edge aligned mode. */
    sdkPwmConf->mode = kFTM_EdgeAlignedPwm;
    sdkPwmConf->pwmFreq_Hz = amlConf->freqHz;
    sdkPwmConf->srcClck_Hz = amlConf->srcClck_Hz;

    /* Store pointer to the first channel in SDK configuration. */
    chPwmSdk = sdkPwmConf->chnlParam;

#elif (SDK_VERSION == SDK_S32)

    /* Default is edge aligned mode. */
    sdkPwmConf->mode = FTM_MODE_EDGE_ALIGNED_PWM;
    sdkPwmConf->uFrequencyHZ = amlConf->freqHz;
    /* Note: timer source clock (amlConf->srcClock_Hz) is not needed by S32
     * FTM driver. */

    /* Store pointer to the first channel in SDK configuration. */
    chPwmSdk = (tmr_sdk_pwm_ch_config_t*)sdkPwmConf->pwmIndependentChannelConfig;

#endif

    chPwmAml = amlConf->chPwm;
    for (i = 0U; i < amlConf->chPwmCnt; i++)
    {   /* Update all SDK channels configuration according to AML configuration. */
        status |= TMR_AML_UpdatePWMChnlConf(chPwmAml, chPwmSdk);
        chPwmSdk++;
        chPwmAml++;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TMR_AML_UpdateOutCmpConf
 * Description   : Updates SDK specific configuration of all timer channels
 * configured for PWM mode. This function is common for KSDK and S32 SDK.
 *
 *END**************************************************************************/
static status_t TMR_AML_UpdateOutCmpConf(const tmr_aml_config_t *amlConf,
        ftm_output_cmp_param_t *sdkCmpConf)
{
    tmr_aml_outcmp_ch_t *chCmpAml;  /* Channel configuration for output
                                       compare mode. */
    tmr_sdk_outcmp_ch_t *chCmpSdk;  /* Channel configuration for output
                                       compare mode. */
    uint8_t             i;
    status_t            status = kStatus_Success;

    AML_ASSERT(amlConf != NULL);
    AML_ASSERT(sdkCmpConf != NULL);

#if (SDK_VERSION == SDK_KSDK)

    chCmpSdk = sdkCmpConf->chConfig;

#elif (SDK_VERSION == SDK_S32)

    chCmpSdk = (tmr_sdk_outcmp_ch_t*)sdkCmpConf->outputChannelConfig;

#endif

    chCmpAml = amlConf->chOutCmp;
    for (i = 0U; i < amlConf->chOutCmpCnt; i++)
    {
        status |= TMR_AML_UpdateOutCmpChnlConf(chCmpAml, chCmpSdk);
        chCmpSdk++;
        chCmpAml++;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TMR_AML_Init
 * Description   : Configures the peripheral for basic operation.
 *
 *END**************************************************************************/
status_t TMR_AML_Init(aml_instance_t instance, const tmr_sdk_config_t *sdkConfig)
{
#if (SDK_VERSION == SDK_KSDK )   /* SDK_KSDK. */

    uint8_t i = 0U;
    status_t status = kStatus_Success;      /*!< Error code. */
    const tmr_sdk_pwm_t *pwmConf = NULL;    /*!< PWM mode configuration. */
    tmr_sdk_outcmp_ch_t *chOutComp = NULL;  /*!< Output compare mode configuration. */
    tmr_sdk_incpt_ch_t *chInCpt = NULL;     /*!< Input capture mode configuration. */

    /* Preconditions. */
    AML_ASSERT(instance < TMR_AML_DEV_CNT);
    AML_ASSERT(sdkConfig != NULL);

    /* General configuration of the FTM device including prescaler, GTB and
     * trigger settings. */
    status = FTM_Init(g_ftmBase[instance], &(sdkConfig->tmrConf));
    if (status != kStatus_Success)
    {
        return kStatus_Fail;
    }

    /* Update the settings of PWM synchronization (enhanced PWM synchronization). */
    /* MOD, CnV CNT etc. updated on reload points (i.e. counter reload), SW
     * trigger synchronization expected. */
    g_ftmBase[instance]->SYNCONF &= ~((uint32_t)FTM_SYNCONF_SWRSTCNT(1U));
    /* Set reload point CNTMIN (counter reload point). */
    g_ftmBase[instance]->SYNC |= FTM_SYNC_CNTMIN(1U);

    /* Configure channels for output compare mode. */
    chOutComp = sdkConfig->tmrCmpConf.chConfig;
    for (i = 0; i < sdkConfig->tmrCmpConf.numOfChnls; i++)
    {
        FTM_SetupOutputCompare(g_ftmBase[instance],
                chOutComp->chnlNumber, chOutComp->mode,
                chOutComp->compareValue);
        chOutComp++;
    }
    chOutComp = NULL;

    /* Configure channels for input capture mode. */
    chInCpt = sdkConfig->tmrCptConf.chConfig;
    for (i = 0; i < sdkConfig->tmrCptConf.numOfChnls; i++)
    {
        FTM_SetupInputCapture(g_ftmBase[instance],
                chInCpt->chnlNumber, chInCpt->mode,
                chInCpt->filterValue);
        chInCpt++;
    }
    chInCpt = NULL;

    /* Set counter period, center aligned setting and configure channels for
     * PWM mode. */
    pwmConf = &(sdkConfig->tmrPwmConf);

    if (0U == pwmConf->numOfChnls)
    {
        /* Fix an issue in the TMR_Init function
         * It is not possible to call FTM_SetupPwm without a channel in PWM mode.
         * There is an issue in the FTM_SetupPwm and TPM_SetupPwm functions,
         * they require non-null pwm channel configuration even if the number of
         * channels is zero. */
        uint32_t mod;
        uint32_t ftmClock = (pwmConf->srcClck_Hz / (1U << (g_ftmBase[instance]->SC & FTM_SC_PS_MASK)));

        switch (pwmConf->mode)
        {
            case kFTM_EdgeAlignedPwm:
            case kFTM_CombinedPwm:
                g_ftmBase[instance]->SC &= ~FTM_SC_CPWMS_MASK;
                mod = (ftmClock / pwmConf->pwmFreq_Hz) - 1;
                break;
            case kFTM_CenterAlignedPwm:
                g_ftmBase[instance]->SC |= FTM_SC_CPWMS_MASK;
                mod = ftmClock / (pwmConf->pwmFreq_Hz * 2);
                break;
            default:
                return kStatus_Fail;
        }

        /* Return an error in case we overflow the registers, probably would require changing
         * clock source to get the desired frequency */
        if (mod > 65535U)
        {
            return kStatus_Fail;
        }
        /* Set the PWM period */
        g_ftmBase[instance]->MOD = mod;

    }
    else
    {
        status = FTM_SetupPwm(g_ftmBase[instance], pwmConf->chnlParam,
                pwmConf->numOfChnls, pwmConf->mode, pwmConf->pwmFreq_Hz,
                pwmConf->srcClck_Hz);
    }

    /* Store clock source selection, it is applied in the StartTimer function. */
    g_tmrData[instance].clkSrc = sdkConfig->clkSrc;

    return status;

#elif (SDK_VERSION == SDK_S32)  /* SDK_S32. */

    status_t status = STATUS_SUCCESS; /*!< Error code. */

    /* Preconditions. */
    AML_ASSERT(instance < TMR_AML_DEV_CNT);
    AML_ASSERT(sdkConfig != NULL);

    /* General configuration of the FTM device including prescaler, GTB and
     * trigger settings. */
    status = FTM_DRV_Init(instance, &(sdkConfig->tmrConf), &(g_ftmState[instance]));
    if (status != STATUS_SUCCESS)
    {
        return kStatus_Fail;
    }
    TMR_AML_StopTimer(instance);

    if (sdkConfig->tmrCmpConf.nNumOutputChannels > 0U)
    {   /* Set Output Compare mode for appropriate channels. */
        status = FTM_DRV_InitOutputCompare(instance, &(sdkConfig->tmrCmpConf));
        if (status != STATUS_SUCCESS)
        {
            return kStatus_Fail;
        }
        TMR_AML_StopTimer(instance);
    }

    if (sdkConfig->tmrCptConf.nNumChannels > 0U)
    {   /* Set Input Capture Mode. */
        status = FTM_DRV_InitInputCapture(instance, &(sdkConfig->tmrCptConf));
        if (status != STATUS_SUCCESS)
        {
            return kStatus_Fail;
        }
        TMR_AML_StopTimer(instance);
    }

    /* Set period and PWM mode if is used. */
    status = FTM_DRV_InitPwm(instance, &(sdkConfig->tmrPwmConf));
    if (status != STATUS_SUCCESS)
    {
        return kStatus_Fail;
    }
    TMR_AML_StopTimer(instance);

    /* Store clock source selection, it is applied in the StartTimer function. */
    g_tmrData[instance].clkSrc = sdkConfig->tmrConf.ftmClockSource;

    /* Settings of FTM PWM synchronization (enhanced PWM synchronization). */
    /* It is expected that MOD, CnV CNT etc. are updated on reload points
     * (i.e. counter reload), SW trigger synchronization.
     * Reload point should be CNTMIN (counter reload point), synchronization
     * occurs on reload points (not immediately). */
    g_ftmBase[instance]->SYNCONF |= FTM_SYNCONF_SWWRBUF(1)| FTM_SYNCONF_SWRSTCNT(1);

    return (status == STATUS_SUCCESS) ? kStatus_Success : kStatus_Fail;

#endif /* SDK_S32. */

}

/*FUNCTION**********************************************************************
 *
 * Function Name : TMR_AML_DeInit
 * Description   : Disables the timer clock.
 *
 *END**************************************************************************/
void TMR_AML_Deinit(aml_instance_t instance)
{

#if (SDK_VERSION == SDK_KSDK)

    AML_ASSERT(instance < TMR_AML_DEV_CNT);

    FTM_Deinit(g_ftmBase[instance]);

#elif (SDK_VERSION == SDK_S32)

    AML_ASSERT(instance < TMR_AML_DEV_CNT);
    /* Deinit. FTM timer instance. */
    FTM_DRV_Deinit(instance);

#endif
}

status_t TMR_AML_FillSdkConfig(const tmr_aml_config_t *amlConfig,
        tmr_sdk_config_t *sdkConfig, tmr_sdk_pwm_ch_config_t **chnlPwmParam,
        tmr_sdk_outcmp_ch_t **chnlCmpParam, tmr_sdk_incpt_ch_t **chnlIncptParam)
{
    status_t            status = kStatus_Success; /*!< Error code. */

    /* Preconditions. */
    AML_ASSERT(sdkConfig != NULL);
    AML_ASSERT(amlConfig != NULL);

#if (SDK_VERSION == SDK_KSDK)

    FTM_GetDefaultConfig(&(sdkConfig->tmrConf));

    sdkConfig->tmrPwmConf.numOfChnls = amlConfig->chPwmCnt;
    sdkConfig->tmrCmpConf.numOfChnls = amlConfig->chOutCmpCnt;
    sdkConfig->tmrCptConf.numOfChnls = amlConfig->chPwmCnt;

    /* S32 SDK needs double pointer. */
    sdkConfig->tmrPwmConf.chnlParam = NULL;
    sdkConfig->tmrCmpConf.chConfig = NULL;
    sdkConfig->tmrCptConf.chConfig = NULL;

    if(chnlPwmParam != NULL)
    {
        sdkConfig->tmrPwmConf.chnlParam = *chnlPwmParam;
    }
    else
    {
        AML_ASSERT(sdkConfig->tmrPwmConf.numOfChnls == 0);
    }

    if(chnlCmpParam != NULL)
    {
        sdkConfig->tmrCmpConf.chConfig = *chnlCmpParam;
    }
    else
    {
        AML_ASSERT(sdkConfig->tmrCmpConf.numOfChnls == 0);
    }

    if(chnlIncptParam != NULL)
    {
        sdkConfig->tmrCptConf.chConfig = *chnlIncptParam;
    }
    else
    {
        /*Notice: Input capture is not handled in current AML. */
        sdkConfig->tmrCptConf.numOfChnls = 0;
    }

    /* General timer settings. */
    /* Default is system clock. */
    sdkConfig->clkSrc = kFTM_SystemClock;
    sdkConfig->tmrConf.prescale = (ftm_clock_prescale_t) amlConfig->prescale;
    sdkConfig->tmrConf.useGlobalTimeBase = amlConfig->gtbEnabled;

#elif (SDK_VERSION == SDK_S32)

    TMR_AML_GetDefaultConfig((tmr_config_t*)&(sdkConfig->tmrConf));

    sdkConfig->tmrConf.ftmPrescaler = amlConfig->prescale;

    sdkConfig->tmrPwmConf.nNumIndependentPwmChannels = amlConfig->chPwmCnt;
    sdkConfig->tmrCmpConf.nNumOutputChannels = amlConfig->chOutCmpCnt;
    sdkConfig->tmrCptConf.nNumChannels = amlConfig->chPwmCnt;
    sdkConfig->tmrPwmConf.nNumCombinedPwmChannels = 0;
    sdkConfig->tmrConf.ftmMode = FTM_MODE_EDGE_ALIGNED_PWM;

    sdkConfig->tmrPwmConf.pwmIndependentChannelConfig = (ftm_independent_ch_param_t*)*chnlPwmParam;
    sdkConfig->tmrCmpConf.outputChannelConfig = (ftm_output_cmp_ch_param_t*)*chnlCmpParam;
    sdkConfig->tmrCptConf.inputChConfig = (ftm_input_ch_param_t*)*chnlIncptParam;
    //TODO
    if(chnlPwmParam == NULL)
    {
        sdkConfig->tmrPwmConf.nNumIndependentPwmChannels = 0;
        sdkConfig->tmrCmpConf.nNumOutputChannels = 0;
    }

    if(chnlCmpParam == NULL)
    {
        sdkConfig->tmrCmpConf.nNumOutputChannels = 0;
    }

    if(chnlIncptParam == NULL)
    {
        sdkConfig->tmrCptConf.nNumChannels = 0;
    }


    sdkConfig->tmrPwmConf.faultConfig = &g_faultPwm;

#endif

    /* Settings of PWM mode. */
    /* Update configuration of channels (PWM mode). */
    status |= TMR_AML_UpdatePWMConf(amlConfig, &(sdkConfig->tmrPwmConf));

    /* Settings of Output compare mode. */
    if (amlConfig->chOutCmpCnt > 0U)
    {   /* Update configuration of channels (output compare mode). */
        status |= TMR_AML_UpdateOutCmpConf(amlConfig, &(sdkConfig->tmrCmpConf));
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TMR_AML_GetDefaultConfig
 * Description   : This function fills the TPM configuration structure with
 * the default settings.
 *
 *END**************************************************************************/
void TMR_AML_GetDefaultConfig(tmr_config_t *config)
{
#if (SDK_VERSION == SDK_KSDK)

    AML_ASSERT(config != NULL);

    FTM_GetDefaultConfig(config);

#elif (SDK_VERSION == SDK_S32)


    config->ftmClockSource = FTM_CLOCK_SOURCE_SYSTEMCLK;
    config->ftmPrescaler = FTM_CLOCK_DIVID_BY_1;
    config->BDMMode = FTM_BDM_MODE_00;
    config->isTofIsrEnabled = false;
    config->ftmMode = FTM_MODE_NOT_INITIALIZED;

    /* PWM synchronization settings. */
    /* MOD, CnV CNT etc. updated on reload points (i.e. counter reload), SW
     * trigger synchronization expected.  */
    config->syncMethod.softwareSync = true;
    /* Set reload point CNTMIN (counter reload point). */
    config->syncMethod.minLoadingPoint = true;
    /* Synchronization occurs on reload points. */
    config->syncMethod.syncPoint = FTM_WAIT_LOADING_POINTS;
    config->syncMethod.autoClearTrigger = false;
    config->syncMethod.hardwareSync0 = false;
    config->syncMethod.hardwareSync1 = false;
    config->syncMethod.hardwareSync2 = false;
    config->syncMethod.initCounterSync = FTM_SYSTEM_CLOCK;
    config->syncMethod.inverterSync = FTM_SYSTEM_CLOCK;
    config->syncMethod.maskRegSync = FTM_SYSTEM_CLOCK;
    config->syncMethod.maxLoadingPoint = false;
    config->syncMethod.outRegSync = FTM_SYSTEM_CLOCK;

#endif

}

/*FUNCTION**********************************************************************
 *
 * Function Name : TMR_AML_EnableInterrupts
 * Description   : Enables the selected timer interrupts.
 *
 *END**************************************************************************/
void TMR_AML_EnableInterrupts(aml_instance_t instance, uint16_t mask)
{

#if (SDK_VERSION == SDK_KSDK)

    AML_ASSERT(instance < TMR_AML_DEV_CNT);

    FTM_EnableInterrupts(g_ftmBase[instance], mask);

#elif (SDK_VERSION == SDK_S32)

    uint32_t chnlInts = (mask & 0xFFU);
    uint8_t chnlNumber = 0U;

    AML_ASSERT(instance < TMR_AML_DEV_CNT);

    /* Enable timer interrupt. */
    if (mask & tmrIntTimeOverflow)
    {
        FTM_DRV_SetTimerOverflowInt(g_ftmBase[instance], true);
    }

    /* Enable fault interrupts. */
    if (mask & tmrIntFault)
    {
        FTM_DRV_SetFaultInt(g_ftmBase[instance], true);
    }

    /* Enable the channel interrupts. */
    while (chnlInts)
    {
        if (chnlInts & 0x01U)
        {
            FTM_DRV_EnableChnInt(g_ftmBase[instance], chnlNumber);
        }
        chnlNumber++;
        chnlInts = chnlInts >> 1U;
    }

#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TMR_AML_DisableInterrupts
 * Description   : Disables the selected timer interrupts.
 *
 *END**************************************************************************/
void TMR_AML_DisableInterrupts(aml_instance_t instance, uint16_t mask)
{

#if (SDK_VERSION == SDK_KSDK)

    AML_ASSERT(instance < TMR_AML_DEV_CNT);

    FTM_DisableInterrupts(g_ftmBase[instance], mask);

#elif (SDK_VERSION == SDK_S32)

    uint32_t chnlInts = (mask & 0xFFU);
    uint8_t chnlNumber = 0U;

    AML_ASSERT(instance < TMR_AML_DEV_CNT);

    /* Disable timer interrupt. */
    if (mask & tmrIntTimeOverflow)
    {
        FTM_DRV_SetTimerOverflowInt(g_ftmBase[instance], false);
    }

    /* Disable the fault interrupt. */
    if (mask & tmrIntFault)
    {
        FTM_DRV_SetFaultInt(g_ftmBase[instance], false);
    }

    /* Disable the channel`s interrupt */
    while (chnlInts)
    {
        if (chnlInts & 0x01U)
        {
            FTM_DRV_DisableChnInt(g_ftmBase[instance], chnlNumber);
        }
        chnlNumber++;
        chnlInts = chnlInts >> 1U;
    }

#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TMR_AML_GetEnabledInterrupts
 * Description   : Gets the enabled timer interrupts.
 *
 *END**************************************************************************/
uint16_t TMR_AML_GetEnabledInterrupts(aml_instance_t instance)
{
#if (SDK_VERSION == SDK_KSDK)

    AML_ASSERT(instance < TMR_AML_DEV_CNT);

    return (uint16_t)FTM_GetEnabledInterrupts(g_ftmBase[instance]);

#elif (SDK_VERSION == SDK_S32)
    uint16_t mask = 0;
    uint8_t chnlNumber;

    AML_ASSERT(instance < TMR_AML_DEV_CNT);

    /* Check if timer interrupt is enabled. */
    if (FTM_DRV_IsOverflowIntEnabled(g_ftmBase[instance]))
    {
        mask |= tmrIntTimeOverflow;
    }

    /* Check if fault are enabled. */
    if (FTM_DRV_IsFaultIntEnabled(g_ftmBase[instance]))
    {
        mask |= tmrIntFault;
    }

    /* Check if channel`s interrupt is enabled. */
    for (chnlNumber = 0U; chnlNumber < FTM_CONTROLS_COUNT; chnlNumber++)
    {
        if (FTM_DRV_IsChnIntEnabled(g_ftmBase[instance], chnlNumber))
        {
            mask |= 0x01U << chnlNumber;
        }
    }

    return mask;
#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TMR_AML_GetStatusFlags
 * Description   : Gets the timer status flags.
 *
 *END**************************************************************************/
uint16_t TMR_AML_GetStatusFlags(aml_instance_t instance)
{
    uint32_t status = 0U;

#if (SDK_VERSION == SDK_KSDK)

    AML_ASSERT(instance < TMR_AML_DEV_CNT);

    status = (uint16_t)FTM_GetStatusFlags(g_ftmBase[instance]);

#elif (SDK_VERSION == SDK_S32)

    AML_ASSERT(instance < TMR_AML_DEV_CNT);

    /* Check timer overflow flag. */
    if (FTM_DRV_HasTimerOverflowed(g_ftmBase[instance]))
    {
        status |= tmrStsTimeOverflow;
    }

    /* Check if fault interrupt. */
    if (g_ftmBase[instance]->MODE & FTM_MODE_FAULTIE_MASK)
    {
        status |= tmrStsFault;
    }

    /* Check channel flags. */
    status |= FTM_DRV_GetEventStatus(g_ftmBase[instance]);

#endif
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TMR_AML_ClearStatusFlags
 * Description   : Clears the FTM status flags.
 *
 *END**************************************************************************/
void TMR_AML_ClearStatusFlags(aml_instance_t instance, uint16_t mask)
{

#if (SDK_VERSION == SDK_KSDK)

    AML_ASSERT(instance < TMR_AML_DEV_CNT);

    FTM_ClearStatusFlags(g_ftmBase[instance], mask);

#elif (SDK_VERSION == SDK_S32)

    uint32_t chnlInts = (mask & 0xFFU);
    uint8_t chnlNumber = 0U;

    AML_ASSERT(instance < TMR_AML_DEV_CNT);

    /* Clear the timer overflow flag by writing a 0 to the bit while it is set */
    if (mask & tmrStsTimeOverflow)
    {
        FTM_DRV_ClearTimerOverflow(g_ftmBase[instance]);
    }

    /* Clear fault flag by writing a 0 to the bit while it is set */
    if (mask & tmrStsFault)
    {
        FTM_DRV_ClearFaultsIsr(g_ftmBase[instance]);
    }

    /* Enable the channel interrupts */
    while (chnlInts)
    {
        if (chnlInts & 0x01U)
        {
            FTM_DRV_ClearChnEventFlag(g_ftmBase[instance], chnlNumber);
        }
        chnlNumber++;
        chnlInts = chnlInts >> 1U;
    }

#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TMR_AML_StartTimer
 * Description   : Starts the counter.
 *
 *END**************************************************************************/
void TMR_AML_StartTimer(aml_instance_t instance)
{
    AML_ASSERT(instance < TMR_AML_DEV_CNT);

#if (SDK_VERSION == SDK_KSDK)

    FTM_StartTimer(g_ftmBase[instance], g_tmrData[instance].clkSrc);

#elif (SDK_VERSION == SDK_S32)

    FTM_DRV_SetClockSource(g_ftmBase[instance], g_tmrData[instance].clkSrc);
#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TMR_AML_StopTimer
 * Description   : Stops the counter.
 *
 *END**************************************************************************/
void TMR_AML_StopTimer(aml_instance_t instance)
{
    AML_ASSERT(instance < TMR_AML_DEV_CNT);

#if (SDK_VERSION == SDK_KSDK)

    FTM_StopTimer(g_ftmBase[instance]);

#elif (SDK_VERSION == SDK_S32)

    FTM_DRV_SetClockSource(g_ftmBase[instance], FTM_CLOCK_SOURCE_NONE);

#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TMR_AML_ReadTimer
 * Description   : Returns the content of counter register.
 *
 *END**************************************************************************/
uint32_t TMR_AML_ReadTimer(aml_instance_t instance)
{
    AML_ASSERT(instance < TMR_AML_DEV_CNT);

#if (SDK_VERSION == SDK_KSDK)

    return FTM_CNT_COUNT(g_ftmBase[instance]->CNT);

#elif (SDK_VERSION == SDK_S32)

    return FTM_DRV_GetCounter(g_ftmBase[instance]);
#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TMR_AML_ResetTimer
 * Description   : Resets counter (counter is set to zero).
 *
 *END**************************************************************************/
void TMR_AML_ResetTimer(aml_instance_t instance)
{
    AML_ASSERT(instance < TMR_AML_DEV_CNT);

    g_ftmBase[instance]->CNT = FTM_CNT_COUNT(0U);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TMR_AML_SetPeriodTicks
 * Description   : Sets timer period (in timer ticks).
 *
 *END**************************************************************************/
void TMR_AML_SetPeriodTicks(aml_instance_t instance, uint32_t ticks)
{
    AML_ASSERT(instance < TMR_AML_DEV_CNT);
    AML_ASSERT((ticks >= 1U) && ((ticks - 1U) <= TMR_CNT_MAX));

    g_ftmBase[instance]->MOD = FTM_MOD_MOD(ticks - 1U);
    /* SW trigger, which updates value of the MOD register. */
    g_ftmBase[instance]->SYNC |= FTM_SYNC_SWSYNC(1U);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TMR_AML_GetPeriodTicks
 * Description   : Returns timer period (in timer ticks).
 *
 *END**************************************************************************/
uint32_t TMR_AML_GetPeriodTicks(aml_instance_t instance)
{

    AML_ASSERT(instance < TMR_AML_DEV_CNT);

    return FTM_MOD_MOD(g_ftmBase[instance]->MOD) + 1U;

}

/*FUNCTION**********************************************************************
 *
 * Function Name : TMR_AML_SetOffsetTicks
 * Description   : Sets timer channel offset (in timer ticks).
 *
 *END**************************************************************************/
void TMR_AML_SetOffsetTicks(aml_instance_t instance, uint8_t channel, uint32_t ticks)
{
    AML_ASSERT(instance < TMR_AML_DEV_CNT);
#if (SDK_VERSION == SDK_KSDK)

    AML_ASSERT(channel < FSL_FEATURE_FTM_CHANNEL_COUNTn(g_ftmBase[instance]));
#elif (SDK_VERSION == SDK_S32)

    AML_ASSERT(channel < FEATURE_FTM_CHANNEL_COUNT);
#endif

    AML_ASSERT(ticks <= TMR_CNT_MAX);
    g_ftmBase[instance]->CONTROLS[channel].CnV = FTM_CnV_VAL(ticks);

    /* SW trigger, which updates value of the CnV register. */
    g_ftmBase[instance]->SYNC |= FTM_SYNC_SWSYNC(1U);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TMR_AML_GetOffsetTicks
 * Description   : Sets timer channel offset (in timer ticks).
 *
 *END**************************************************************************/
uint32_t TMR_AML_GetOffsetTicks(aml_instance_t instance, uint8_t channel)
{

    AML_ASSERT(instance < TMR_AML_DEV_CNT);
#if (SDK_VERSION == SDK_KSDK)

    AML_ASSERT(channel < FSL_FEATURE_FTM_CHANNEL_COUNTn(g_ftmBase[instance]));
#elif (SDK_VERSION == SDK_S32)

    AML_ASSERT(channel < FEATURE_FTM_CHANNEL_COUNT);
#endif

    return FTM_CnV_VAL(g_ftmBase[instance]->CONTROLS[channel].CnV);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TMR_AML_GetInstance
 * Description   : Gets instance for timer module base pointer.
 *
 *END**************************************************************************/
uint32_t TMR_AML_GetInstance(const void *tmrBase)
{
    const FTM_Type *base = (const FTM_Type *) tmrBase;
    uint32_t instance;

    AML_ASSERT(tmrBase != NULL);

    /* Find the instance index from base address mappings. */
    for (instance = 0U; instance < TMR_AML_DEV_CNT; instance++)
    {
        if (g_ftmBase[instance] == base)

        {
            break;
        }
    }

    AML_ASSERT(instance < TMR_AML_DEV_CNT);

    return instance;
}

#endif
