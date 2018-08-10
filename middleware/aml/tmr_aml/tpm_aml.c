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
 * @file tpm_aml.c
 *
 * TPM peripheral driver intended for MCUXpresso SDK.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "tmr_aml.h"

#if (SDK_VERSION == SDK_KSDK)

#include "fsl_common.h"

#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Note: this source file is intended for TPM peripheral only. */
#if (TMR_AML_TYPE == TMR_AML_TPM)

/*! @brief Max. value of the counter. */
#define TMR_CNT_MAX                 65535U

#if (SDK_VERSION == SDK_KSDK)

/*! @brief Number of timer devices present on the MCU. */
#define TMR_AML_DEV_CNT             FSL_FEATURE_SOC_TPM_COUNT

#elif (SDK_VERSION == SDK_S32)

/*! @brief Number of timer devices present on the MCU. */
#define TMR_AML_DEV_CNT             0U  /* S32 does not include any TPM driver. */

#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#if (SDK_VERSION == SDK_KSDK)

/**
 * @brief Updates SDK specific configuration of a timer channel according
 * to AML configuration (SDK independent). This function copies PWM mode
 * settings only.
 *
 * @param amlChConf AML channel configuration (SDK independent).
 * @param sdkChConf SDK specific configuration of timer channels.
 *
 * @return Error code (staERR_OK when no error occurred).
 */
static status_t TMR_AML_UpdatePWMChnlConf(
        const tmr_aml_pwm_ch_t *amlChConf,
        tpm_chnl_pwm_signal_param_t *sdkChConf);

/**
 * @brief Updates SDK specific configuration of a timer channel according
 * to AML configuration (SDK independent). This function copies Output
 * compare mode settings only.
 *
 * @param amlChConf AML channel configuration (SDK independent).
 * @param sdkChConf SDK specific configuration of timer channels.
 *
 * @return Error code (staERR_OK when no error occurred).
 */
static status_t TMR_AML_UpdateOutCmpChnlConf(
        const tmr_aml_outcmp_ch_t *amlChConf,
        tmr_sdk_outcmp_ch_t *sdkChConf);

/**
 * @brief Updates SDK specific configuration of all timer channels
 * configured for PWM mode.
 *
 * @param amlConfig     AML timer configuration (SDK independent).
 * @param sdkPwmConf    SDK specific configuration of PWM mode.
 *
 * @return Error code (staERR_OK when no error occurred).
 */
static status_t TMR_AML_UpdatePWMConf(const tmr_aml_config_t *amlConfig,
        tmr_sdk_pwm_t *sdkPwmConf);

/**
 * @brief Updates SDK specific configuration of Output compare mode according
 * to AML configuration (SDK independent).
 *
 * @param amlConfig     AML timer configuration (SDK independent).
 * @param sdkCmpConf    SDK specific configuration of output compare mode.
 *
 * @return Error code (staERR_OK when no error occurred).
 */
static status_t TMR_AML_UpdateOutCmpConf(const tmr_aml_config_t *amlConfig,
        tmr_sdk_outcmp_t *sdkCmpConf);

#endif  /* END of KSDK function. */

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if (SDK_VERSION == SDK_KSDK)

/*! @brief Device data structure. */
static tmr_data_t g_tmrData[TMR_AML_DEV_CNT];

/*! @brief Pointers to TPM bases for each instance. */
static TPM_Type *const g_tpmBase[] = TPM_BASE_PTRS;

#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
#if (SDK_VERSION == SDK_KSDK)

/*FUNCTION**********************************************************************
 *
 * Function Name : TMR_AML_UpdatePWMChnlConf
 * Description   : Updates SDK specific configuration of a timer channel
 * according to AML configuration (SDK independent). This function copies PWM
 * mode settings only.
 *
 *END**************************************************************************/
static status_t TMR_AML_UpdatePWMChnlConf(
        const tmr_aml_pwm_ch_t *amlChConf,
        tpm_chnl_pwm_signal_param_t *sdkChConf)
{
    status_t status = kStatus_Success;

    /* Preconditions. */
    AML_ASSERT(amlChConf != NULL);
    AML_ASSERT(sdkChConf != NULL);

    /* Update the configuration. */
    sdkChConf->chnlNumber = (tpm_chnl_t)amlChConf->channel;
    sdkChConf->dutyCyclePercent = amlChConf->dutyPercent;
    switch (amlChConf->pwmAction)
    {
        case tmrPwmActNoSignal:
            sdkChConf->level = kTPM_NoPwmSignal;
            break;
        case tmrPwmActLowTrue:
            sdkChConf->level = kTPM_LowTrue;
            break;
        case tmrPwmActHighTrue:
            sdkChConf->level = kTPM_HighTrue;
            break;
        default:
            AML_ASSERT("Invalid value of amlChConf->action");
            status = kStatus_InvalidArgument;
    }

    return status;
}

#endif  /* END of KSDK function. */

#if (SDK_VERSION == SDK_KSDK)

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

    /* Update the configuration. */
    sdkChConf->chnlNumber = (tpm_chnl_t)amlChConf->channel;
    sdkChConf->compareValue = amlChConf->compareVal;
    switch (amlChConf->cmpAction)
    {
        case tmrOutActNoSignal:
            sdkChConf->mode = kTPM_NoOutputSignal;
            break;
        case tmrOutActToggleOnMatch:
            sdkChConf->mode = kTPM_ToggleOnMatch;
            break;
        case tmrOutActClearOnMatch:
            sdkChConf->mode = kTPM_ClearOnMatch;
            break;
        case tmrOutActSetOnMatch:
            sdkChConf->mode = kTPM_SetOnMatch;
            break;
        case tmrOutActHighPulseOutput:
            sdkChConf->mode = kTPM_HighPulseOutput;
            break;
        case tmrOutActLowPulseOutput:
            sdkChConf->mode = kTPM_LowPulseOutput;
            break;
        default:
            AML_ASSERT("Invalid value of amlChConf->action");
            status = kStatus_InvalidArgument;
    }

    return status;
}

#endif  /* END of KSDK function. */

#if (SDK_VERSION == SDK_KSDK)

/*FUNCTION**********************************************************************
 *
 * Function Name : TMR_AML_UpdatePWMConf
 * Description   : Updates SDK specific configuration of all timer channels
 * configured for PWM mode.
 *
 *END**************************************************************************/
static status_t TMR_AML_UpdatePWMConf(const tmr_aml_config_t *amlConfig,
        tmr_sdk_pwm_t *sdkPwmConf)
{
    tmr_aml_pwm_ch_t   *chPwmAml;           /* AML channel configuration for PWM
                                               mode (SDK independent). */
    tpm_chnl_pwm_signal_param_t *chPwmSdk;  /* SDK specific Channel configuration
                                               for PWM mode. */
    uint8_t             i;
    status_t            status = kStatus_Success;

    AML_ASSERT(amlConfig != NULL);
    AML_ASSERT(sdkPwmConf != NULL);
    /* Size of the array for PWM channels must be the same in AML structure
     * and SDK specific structure. */
    AML_ASSERT(sdkPwmConf->numOfChnls == amlConfig->chPwmCnt);

    /* Default is edge aligned mode. */
    sdkPwmConf->mode = kTPM_EdgeAlignedPwm;

    chPwmAml = amlConfig->chPwm;
    chPwmSdk = sdkPwmConf->chnlParam;
    for (i = 0U; i < amlConfig->chPwmCnt; i++)
    {
        status |= TMR_AML_UpdatePWMChnlConf(chPwmAml, chPwmSdk);
        chPwmSdk++;
        chPwmAml++;
    }

    return status;
}

#endif  /* END of KSDK function. */

#if (SDK_VERSION == SDK_KSDK)

/*FUNCTION**********************************************************************
 *
 * Function Name : TMR_AML_UpdateOutCmpConf
 * Description   : Updates SDK specific configuration of Output compare mode
 * according to AML configuration (SDK independent).
 *
 *END**************************************************************************/
static status_t TMR_AML_UpdateOutCmpConf(const tmr_aml_config_t *amlConfig,
        tmr_sdk_outcmp_t *sdkCmpConf)
{
    tmr_aml_outcmp_ch_t *chCmpAml;  /* Channel configuration for output
                                       compare mode. */
    tmr_sdk_outcmp_ch_t *chCmpSdk;  /* Channel configuration for output
                                       compare mode */
    uint8_t             i;
    status_t            status = kStatus_Success;

    AML_ASSERT(amlConfig != NULL);
    AML_ASSERT(sdkCmpConf != NULL);
    /* Size of the array for PWM channels must be the same in AML structure
     * and SDK specific structure. */
    AML_ASSERT(sdkCmpConf->numOfChnls == amlConfig->chOutCmpCnt);

    chCmpAml = amlConfig->chOutCmp;
    chCmpSdk = sdkCmpConf->chConfig;
    for (i = 0U; i < amlConfig->chOutCmpCnt; i++)
    {
        status |= TMR_AML_UpdateOutCmpChnlConf(chCmpAml, chCmpSdk);
        chCmpSdk++;
        chCmpAml++;
    }

    return status;
}

#endif  /* END of KSDK function. */

/*FUNCTION**********************************************************************
 *
 * Function Name : TMR_AML_Init
 * Description   : Configures the peripheral for basic operation.
 *
 *END**************************************************************************/
status_t TMR_AML_Init(aml_instance_t instance,
        const tmr_sdk_config_t *sdkConfig)
{
#if (SDK_VERSION == SDK_KSDK)

    uint8_t i = 0U;
    status_t status = kStatus_Success;      /*!< Error code. */
    const tmr_sdk_pwm_t *pwmConf = NULL;    /* PWM mode configuration. */
    tmr_sdk_outcmp_ch_t *chOutComp = NULL;  /* Output compare mode configuration. */
    tmr_sdk_incpt_ch_t  *chInCpt = NULL;    /* Input capture mode configuration. */

    /* Preconditions. */
    AML_ASSERT(instance < TMR_AML_DEV_CNT);
    AML_ASSERT(sdkConfig != NULL);

    /* General configuration of the TPM device including prescaler, GTB and
     * trigger settings. */
    TPM_Init(g_tpmBase[instance], &(sdkConfig->tmrConf));

    /* Configure channels for output compare mode. */
    chOutComp = sdkConfig->tmrCmpConf.chConfig;
    for (i = 0U; i < sdkConfig->tmrCmpConf.numOfChnls; i++)
    {
        TPM_SetupOutputCompare(g_tpmBase[instance], chOutComp->chnlNumber,
                chOutComp->mode, chOutComp->compareValue);
        chOutComp++;
    }
    chOutComp = NULL;

    /* Configure channels for input capture mode. */
    chInCpt = sdkConfig->tmrCptConf.chConfig;
    for (i = 0U; i < sdkConfig->tmrCptConf.numOfChnls; i++)
    {
        TPM_SetupInputCapture(g_tpmBase[instance], chInCpt->chnlNumber,
                chInCpt->mode);
        chInCpt++;
    }
    chInCpt = NULL;

    /* Set counter period, center aligned setting and configure channels for
     * PWM mode. */
    pwmConf = &(sdkConfig->tmrPwmConf);

    if (0U == pwmConf->numOfChnls)
    {
        /* Fix an issue in the TMR_Init function
         * It is not possible to call TPM_SetupPwm without a channel in PWM mode.
         * There is an issue in the FTM_SetupPwm and TPM_SetupPwm functions,
         * they require non-null pwm channel configuration even if the number of
         * channels is zero. */

        uint32_t mod;
        uint32_t tpmClock = (sdkConfig->srcClck_Hz / (1U << (g_tpmBase[instance]->SC & TPM_SC_PS_MASK)));

        switch (pwmConf->mode)
        {
            case kTPM_EdgeAlignedPwm:
#if defined(FSL_FEATURE_TPM_HAS_COMBINE) && FSL_FEATURE_TPM_HAS_COMBINE
            case kTPM_CombinedPwm:
#endif
                g_tpmBase[instance]->SC &= ~TPM_SC_CPWMS_MASK;
                mod = (tpmClock / sdkConfig->pwmFreq_Hz) - 1;
                break;
            case kTPM_CenterAlignedPwm:
                g_tpmBase[instance]->SC |= TPM_SC_CPWMS_MASK;
                mod = tpmClock / (sdkConfig->pwmFreq_Hz * 2);
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
        g_tpmBase[instance]->MOD = mod;
    }
    else
    {
        status = TPM_SetupPwm(g_tpmBase[instance], pwmConf->chnlParam,
            pwmConf->numOfChnls, pwmConf->mode, sdkConfig->pwmFreq_Hz,
            sdkConfig->srcClck_Hz);
    }

    /* Store clock source selection, it is applied in the StartTimer function. */
    g_tmrData[instance].clkSrc = sdkConfig->clkSrc;

    return status;

#elif (SDK_VERSION == SDK_S32)

    /* Note: S32 SDK does not contain any driver for TPM peripheral. */
    AML_ASSERT("S32 SDK does not contain any driver for TPM peripheral.");
    return kStatus_Fail;

#endif
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

    TPM_Deinit(g_tpmBase[instance]);

#elif (SDK_VERSION == SDK_S32)

    /* Note: S32 SDK does not contain any driver for TPM peripheral. */
    AML_ASSERT("S32 SDK does not contain any driver for TPM peripheral.");

#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TMR_AML_FillSdkConfig
 * Description   : Fills SDK specific configuration structure according
 * to independent AML configuration.
 *
 *END**************************************************************************/
status_t TMR_AML_FillSdkConfig(const tmr_aml_config_t *amlConfig,
        tmr_sdk_config_t *sdkConfig, tmr_sdk_pwm_ch_config_t **chnlPwmParam,
                              tmr_sdk_outcmp_ch_t **chnlCmpParam, tmr_sdk_incpt_ch_t **chnlIncptParam)
{
#if (SDK_VERSION == SDK_KSDK)

    status_t status = kStatus_Success; /*!< Error code. */

    /* Preconditions. */
    AML_ASSERT(sdkConfig != NULL);
    AML_ASSERT(amlConfig != NULL);

    TPM_GetDefaultConfig(&(sdkConfig->tmrConf));

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
    sdkConfig->clkSrc = kTPM_SystemClock;
    sdkConfig->tmrConf.prescale = (tpm_clock_prescale_t) amlConfig->prescale;
    sdkConfig->tmrConf.useGlobalTimeBase = amlConfig->gtbEnabled;
    sdkConfig->pwmFreq_Hz = amlConfig->freqHz;
    sdkConfig->srcClck_Hz = amlConfig->srcClck_Hz;

    /* Settings of PWM mode. */
    /* Update configuration of channels (PWM mode). */
    status |= TMR_AML_UpdatePWMConf(amlConfig, &(sdkConfig->tmrPwmConf));

    /* Settings of Output compare mode. */
    if (amlConfig->chOutCmpCnt > 0U)
    {   /* Update configuration of channels (output compare mode). */
        status |= TMR_AML_UpdateOutCmpConf(amlConfig, &(sdkConfig->tmrCmpConf));
    }

    return status;

#elif (SDK_VERSION == SDK_S32)

    /* Note: S32 SDK does not contain any driver for TPM peripheral. */
    AML_ASSERT("S32 SDK does not contain any driver for TPM peripheral.");
    return kStatus_Fail;

#endif
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

    TPM_GetDefaultConfig(config);

#elif (SDK_VERSION == SDK_S32)

    /* Note: S32 SDK does not contain any driver for TPM peripheral. */
    AML_ASSERT("S32 SDK does not contain any driver for TPM peripheral.");

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

    TPM_EnableInterrupts(g_tpmBase[instance], (uint32_t)mask);

#elif (SDK_VERSION == SDK_S32)

    /* Note: S32 SDK does not contain any driver for TPM peripheral. */
    AML_ASSERT("S32 SDK does not contain any driver for TPM peripheral.");

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

    TPM_DisableInterrupts(g_tpmBase[instance], (uint32_t)mask);

#elif (SDK_VERSION == SDK_S32)

    /* Note: S32 SDK does not contain any driver for TPM peripheral. */
    AML_ASSERT("S32 SDK does not contain any driver for TPM peripheral.");

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

    return (uint16_t)TPM_GetEnabledInterrupts(g_tpmBase[instance]);

#elif (SDK_VERSION == SDK_S32)

    /* Note: S32 SDK does not contain any driver for TPM peripheral. */
    AML_ASSERT("S32 SDK does not contain any driver for TPM peripheral.");
    return 0;

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
#if (SDK_VERSION == SDK_KSDK)

    AML_ASSERT(instance < TMR_AML_DEV_CNT);

    return (uint16_t)TPM_GetStatusFlags(g_tpmBase[instance]);

#elif (SDK_VERSION == SDK_S32)

    /* Note: S32 SDK does not contain any driver for TPM peripheral. */
    AML_ASSERT("S32 SDK does not contain any driver for TPM peripheral.");
    return 0U;

#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TMR_AML_ClearStatusFlags
 * Description   : Clears the TPM status flags.
 *
 *END**************************************************************************/
void TMR_AML_ClearStatusFlags(aml_instance_t instance, uint16_t mask)
{
#if (SDK_VERSION == SDK_KSDK)

    AML_ASSERT(instance < TMR_AML_DEV_CNT);

    TPM_ClearStatusFlags(g_tpmBase[instance], (uint32_t)mask);

#elif (SDK_VERSION == SDK_S32)

    /* Note: S32 SDK does not contain any driver for TPM peripheral. */
    AML_ASSERT("S32 SDK does not contain any driver for TPM peripheral.");

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
#if (SDK_VERSION == SDK_KSDK)

    AML_ASSERT(instance < TMR_AML_DEV_CNT);

    TPM_StartTimer(g_tpmBase[instance], g_tmrData[instance].clkSrc);

#elif (SDK_VERSION == SDK_S32)

    /* Note: S32 SDK does not contain any driver for TPM peripheral. */
    AML_ASSERT("S32 SDK does not contain any driver for TPM peripheral.");

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
#if (SDK_VERSION == SDK_KSDK)

    AML_ASSERT(instance < TMR_AML_DEV_CNT);

    TPM_StopTimer(g_tpmBase[instance]);

#elif (SDK_VERSION == SDK_S32)

    /* Note: S32 SDK does not contain any driver for TPM peripheral. */
    AML_ASSERT("S32 SDK does not contain any driver for TPM peripheral.");

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
#if (SDK_VERSION == SDK_KSDK)

    AML_ASSERT(instance < TMR_AML_DEV_CNT);

    return TPM_CNT_COUNT(g_tpmBase[instance]->CNT);

#elif (SDK_VERSION == SDK_S32)

    /* Note: S32 SDK does not contain any driver for TPM peripheral. */
    AML_ASSERT("S32 SDK does not contain any driver for TPM peripheral.");
    return 0U;

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
#if (SDK_VERSION == SDK_KSDK)

    AML_ASSERT(instance < TMR_AML_DEV_CNT);

    g_tpmBase[instance]->CNT = TPM_CNT_COUNT(0U);

#elif (SDK_VERSION == SDK_S32)

    /* Note: S32 SDK does not contain any driver for TPM peripheral. */
    AML_ASSERT("S32 SDK does not contain any driver for TPM peripheral.");

#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TMR_AML_SetPeriodTicks
 * Description   : Sets timer period (in timer ticks).
 *
 *END**************************************************************************/
void TMR_AML_SetPeriodTicks(aml_instance_t instance, uint32_t ticks)
{
#if (SDK_VERSION == SDK_KSDK)

    AML_ASSERT(instance < TMR_AML_DEV_CNT);
    AML_ASSERT((ticks >= 1U) && ((ticks - 1U) <= TMR_CNT_MAX));

    g_tpmBase[instance]->MOD = TPM_MOD_MOD(ticks - 1U);

#elif (SDK_VERSION == SDK_S32)

    /* Note: S32 SDK does not contain any driver for TPM peripheral. */
    AML_ASSERT("S32 SDK does not contain any driver for TPM peripheral.");

#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TMR_AML_GetPeriodTicks
 * Description   : Returns timer period (in timer ticks).
 *
 *END**************************************************************************/
uint32_t TMR_AML_GetPeriodTicks(aml_instance_t instance)
{
#if (SDK_VERSION == SDK_KSDK)

    AML_ASSERT(instance < TMR_AML_DEV_CNT);

    return TPM_MOD_MOD(g_tpmBase[instance]->MOD) + 1U;

#elif (SDK_VERSION == SDK_S32)

    /* Note: S32 SDK does not contain any driver for TPM peripheral. */
    AML_ASSERT("S32 SDK does not contain any driver for TPM peripheral.");
    return 0U;

#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TMR_AML_SetOffsetTicks
 * Description   : Sets timer channel offset (in timer ticks).
 *
 *END**************************************************************************/
void TMR_AML_SetOffsetTicks(aml_instance_t instance, uint8_t channel, uint32_t ticks)
{
#if (SDK_VERSION == SDK_KSDK)

    AML_ASSERT(instance < TMR_AML_DEV_CNT);
    AML_ASSERT(channel < FSL_FEATURE_TPM_CHANNEL_COUNTn(g_tpmBase[instance]));
    AML_ASSERT(ticks <= TMR_CNT_MAX);

    g_tpmBase[instance]->CONTROLS[channel].CnV = TPM_CnV_VAL(ticks);

#elif (SDK_VERSION == SDK_S32)

    /* Note: S32 SDK does not contain any driver for TPM peripheral. */
    AML_ASSERT("S32 SDK does not contain any driver for TPM peripheral.");

#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TMR_AML_GetOffsetTicks
 * Description   : Sets timer channel offset (in timer ticks).
 *
 *END**************************************************************************/
uint32_t TMR_AML_GetOffsetTicks(aml_instance_t instance, uint8_t channel)
{
#if (SDK_VERSION == SDK_KSDK)

    AML_ASSERT(instance < TMR_AML_DEV_CNT);
    AML_ASSERT(channel < FSL_FEATURE_TPM_CHANNEL_COUNTn(g_tpmBase[instance]));

    return TPM_CnV_VAL(g_tpmBase[instance]->CONTROLS[channel].CnV);

#elif (SDK_VERSION == SDK_S32)

    /* Note: S32 SDK does not contain any driver for TPM peripheral. */
    AML_ASSERT("S32 SDK does not contain any driver for TPM peripheral.");
    return 0U;

#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TMR_AML_GetInstance
 * Description   : Gets instance for timer module base pointer.
 *
 *END**************************************************************************/
uint32_t TMR_AML_GetInstance(const void *tmrBase)
{
#if (SDK_VERSION == SDK_KSDK)

    const TPM_Type  *base = (const TPM_Type *)tmrBase;
    uint32_t        instance;

    AML_ASSERT(tmrBase != NULL);

    /* Find the instance index from base address mappings. */
    for (instance = 0U; instance < TMR_AML_DEV_CNT; instance++)
    {
        if (g_tpmBase[instance] == base)
        {
            break;
        }
    }

    AML_ASSERT(instance < TMR_AML_DEV_CNT);

    return instance;

#elif (SDK_VERSION == SDK_S32)

    /* Note: S32 SDK does not contain any driver for TPM peripheral. */
    AML_ASSERT("S32 SDK does not contain any driver for TPM peripheral.");
    return 0U;

#endif
}

#endif  /* END of TPM peripheral driver. */
/*******************************************************************************
 * EOF
 ******************************************************************************/
