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
 * @file tmr_aml.h
 *
 * TPM and FTM driver intended for S32 SDK and MCUXpresso SDK. This driver creates
 * abstraction layer for TPM and FTM peripherals.
 */

#ifndef SOURCE_TMR_AML_H_
#define SOURCE_TMR_AML_H_

 /*******************************************************************************
 * Includes
 ******************************************************************************/
#include "../common_aml.h"

#if (SDK_VERSION == SDK_KSDK)

#include "fsl_common.h"

#else

#include "device_registers.h"
#endif

/*! @brief Timer types supported by AML timer driver (tmr_aml.h). */
#define TMR_AML_TPM 0U              /*! TPM timer. */
#define TMR_AML_FTM 1U              /*! FTM timer. */

/*!
 * @brief Selection of the timer type that is present on used MCU (TPM or FTM
 * peripheral).
 *
 * This option configures the AML timer driver for desired timer type.
 * Use macros defined above (TMR_AML_TPM or TMR_AML_FTM).
 */
#if (SDK_VERSION == SDK_KSDK)        /* KSDK. */
    #if (FSL_FEATURE_SOC_FTM_COUNT > 0)

#define TMR_AML_TYPE TMR_AML_FTM    /*!< MCU utilizes FTM timers. */

    #elif (FSL_FEATURE_SOC_TPM_COUNT > 0)

#define TMR_AML_TYPE TMR_AML_TPM    /*!< MCU utilizes TPM timers. */

    #endif

#elif (SDK_VERSION == SDK_S32)      /* S32 SDK. */
    #if (FTM_INSTANCE_COUNT > 0)

#define TMR_AML_TYPE TMR_AML_FTM    /*!< MCU utilizes FTM timers. */

    #elif (TPM_INSTANCE_COUNT > 0)

#define TMR_AML_TYPE TMR_AML_TPM    /*!< MCU utilizes TPM timers. */

    #endif
#endif

#if (TMR_AML_TYPE == TMR_AML_TPM)

#include "tpm_aml.h"

#elif (TMR_AML_TYPE == TMR_AML_FTM)

#include "ftm_aml.h"

#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @addtogroup enum_group
 * @{
 */
/*! @brief Timer channel operation modes, which defines output action
 * in edge/center aligned mode. */
typedef enum
{
    tmrPwmActNoSignal = 0U,         /*!< No PWM output on pin */
    tmrPwmActLowTrue,               /*!< Low true pulses (set output on match,
                                         clear output on reload). */
    tmrPwmActHighTrue,              /*!< High true pulses (clear output on match,
                                         set output on reload). */
} tmr_aml_pwm_act_t;

/*! @brief Timer channel operation modes, which defines output action
 * in output compare mode. */
typedef enum
{
    tmrOutActNoSignal = 0U,         /*!< No channel output when counter reaches CnV  */
    tmrOutActToggleOnMatch,         /*!< Toggle output */
    tmrOutActClearOnMatch,          /*!< Clear output */
    tmrOutActSetOnMatch,            /*!< Set output */
    tmrOutActHighPulseOutput,       /*!< Pulse output high */
    tmrOutActLowPulseOutput         /*!< Pulse output low */
} tmr_aml_outcmp_act_t;

/*! @brief Timer prescale value selection for the clock source. */
typedef enum
{
    tmrPrescDiv_1 = 0U,             /*!< Divide by 1 */
    tmrPrescDiv_2,                  /*!< Divide by 2 */
    tmrPrescDiv_4,                  /*!< Divide by 4 */
    tmrPrescDiv_8,                  /*!< Divide by 8 */
    tmrPrescDiv_16,                 /*!< Divide by 16 */
    tmrPrescDiv_32,                 /*!< Divide by 32 */
    tmrPrescDiv_64,                 /*!< Divide by 64 */
    tmrPrescDiv_128                 /*!< Divide by 128 */
} tmr_clk_prescale_t;
/*! @} */

/*!
 * @addtogroup struct_group
 * @{
 */
/*! @brief AML timer channel parameters for PWM mode. */
typedef struct
{
    uint8_t channel;                /*!< Channel number. */
    uint32_t dutyPercent;           /*!< Duty cycle percent. */
    tmr_aml_pwm_act_t pwmAction;    /*!< Channel action on compare and reload. */
} tmr_aml_pwm_ch_t;

/*! @brief AML timer channel parameters for output compare. */
typedef struct
{
    uint8_t channel;                /*!< Channel number. */
    uint32_t compareVal;            /*!< Channel compare value. */
    tmr_aml_outcmp_act_t cmpAction; /*!< Channel action on compare and reload. */
} tmr_aml_outcmp_ch_t;

/*!
 * @brief AML timer parameters available for superior drivers (pointers).
 *
 * These parameters are used in superior drivers (user selectable or needed by driver).
 * Values from this structure are written to SDK specific configuration structures
 * (original values are overwritten by these values) only if the pointer is not NULL.
 */
typedef struct
{
    uint32_t freqHz;                /*!< The counter frequency in Hz */
    uint32_t srcClck_Hz;            /*!< The counter clock in Hz. */
    tmr_clk_prescale_t prescale;    /*!< Timer prescale value. */
    bool gtbEnabled;                /*!< Global time base function setting. */
    uint8_t chPwmCnt;               /*!< Number of channels for PWM mode to
                                         configure. */
    tmr_aml_pwm_ch_t *chPwm;       /*!< Array of channels parameters for PWM
                                         mode. */
    uint8_t chOutCmpCnt;            /*!< Number of channels for output
                                         compare mode to configure. */
    tmr_aml_outcmp_ch_t *chOutCmp; /*!< Array of channels parameters for
                                         output compare mode. */
} tmr_aml_config_t;
/*! @} */

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @addtogroup globals_group
 * @{
 */

/*!
 * @addtogroup function_group
 * @{
 */
/*!
 * @brief Configures the peripheral for basic operation.
 *
 * General configuration of TPM device including prescaler, GTB, trigger
 * settings, period, center aligned setting etc. It configures timer channels
 * (mode, output action and offset).
 *
 * @param instance  Timer peripheral instance number.
 * @param sdkConfig SDK specific timer configuration.
 *
 * @return          Error code (staERR_OK when no error occurred).
 */
status_t TMR_AML_Init(aml_instance_t instance,
        const tmr_sdk_config_t *sdkConfig);

/*!
 * @brief Disables the timer clock.
 *
 * @param instance Timer peripheral instance number.
 */
void TMR_AML_Deinit(aml_instance_t instance);

/*!
 * @brief Fills SDK specific configuration structure according to independent
 * AML configuration. This function configures S32 SDK or KSDK configuration
 * structure according SDK_VERSION form independent tmrAmlConfig
 *
 * @param tmrAmlConfig  AML configuration structure, which does not depend
 * on SDK version. It is compatible with S32 SDK and KSDK. Configuration will
 * be used for configure SDK which is currently used. It depends on SDK_VERSION
 * which is define in AML/common_aml.h file.
 * @param trmSdkConfig SDK specific configuration, this structure will configure
 * according tmrAmlConfig. 
 * @param tmr_sdk_pwm_ch_config_t(*)[] pointer to array of tmr_sdk_pwm_ch_config_t
 * for each PWM channel. It is used for generating independent PWM signal on output. 
 * Use NULL pointer if PWM is not needed.
 * @param tmr_sdk_outcmp_ch_t(*)[] pointer to array of tmr_sdk_outcmp_ch_t
 * for each output compare register. It is used for output compare mode.
 * Use NULL pointer if output compare mode is not needed.
 * @param chnlIncptParam(*)[] pointer to array of chnlIncptParam
 * for each input capture register. It is used for input capture mode.
 * Use NULL pointer if output compare mode is not needed.
 * @return              Error code (staERR_OK when no error occurred).
 */
status_t TMR_AML_FillSdkConfig(const tmr_aml_config_t *amlConfig,
        tmr_sdk_config_t *sdkConfig, tmr_sdk_pwm_ch_config_t **chnlPwmParam,
        tmr_sdk_outcmp_ch_t **chnlCmpParam, tmr_sdk_incpt_ch_t **chnlIncptParam);

/*!
 * @brief This function fills the TPM configuration structure with the default
 * settings.
 *
 * Note that the function calls GetDefaultConfig of used peripheral (TPM or
 * FTM) for KSDK.
 *
 * @param config Pointer to user's TMR configuration structure.
 */
void TMR_AML_GetDefaultConfig(tmr_config_t *config);

/*!
 * @brief Enables the selected timer interrupts.
 *
 * @param instance  Timer peripheral instance number.
 * @param mask      The interrupts to disable. This is a logical OR of members of the
 * enumeration tmr_aml_int_flags_t.
 */
void TMR_AML_EnableInterrupts(aml_instance_t instance, uint16_t mask);

/*!
 * @brief Disables the selected timer interrupts.
 *
 * @param instance  Timer peripheral instance number.
 * @param mask      The interrupts to disable. This is a logical OR of members of the
 * enumeration tmr_aml_int_flags_t.
 */
void TMR_AML_DisableInterrupts(aml_instance_t instance, uint16_t mask);

/*!
 * @brief Gets the enabled timer interrupts.
 *
 * @param instance  Timer peripheral instance number.
 *
 * @return The enabled interrupts. This is the logical OR of members of the
 * enumeration tmr_aml_int_flags_t.
 */
uint16_t TMR_AML_GetEnabledInterrupts(aml_instance_t instance);

/*!
 * @brief Gets the timer status flags.
 *
 * @param instance Timer peripheral instance number.
 *
 * @return The status flags. This is the logical OR of members of the
 * enumeration tmr_aml_status_flags_t.
 */
uint16_t TMR_AML_GetStatusFlags(aml_instance_t instance);

/*!
 * @brief Clears the TPM status flags.
 *
 * @param instance  Timer peripheral instance number.
 * @param mask      The status flags to clear. This is a logical OR of members of the
 * enumeration tmr_aml_status_flags_t.
 */
void TMR_AML_ClearStatusFlags(aml_instance_t instance, uint16_t mask);

/*!
 * @brief Starts the counter.
 *
 * It sets the clock source defined in initialization (see tmr_sdk_config_t
 * structure).
 *
 * @param instance Timer peripheral instance number.
 */
void TMR_AML_StartTimer(aml_instance_t instance);

/*!
 * @brief Stops the counter.
 *
 * @param instance Timer peripheral instance number.
 */
void TMR_AML_StopTimer(aml_instance_t instance);

/*!
 * @brief Returns the content of counter register.
 *
 * @param instance Timer peripheral instance number.
 *
 * @return Counter value (number of counter ticks).
 */
uint32_t TMR_AML_ReadTimer(aml_instance_t instance);

/**
 * @brief Resets counter (counter is set to zero).
 *
 * @param instance Timer peripheral instance number.
 */
void TMR_AML_ResetTimer(aml_instance_t instance);

/*!
 * @brief Sets timer period (in timer ticks).
 *
 * @param instance Timer peripheral instance number.
 * @param ticks Timer period (in timer ticks).
 */
void TMR_AML_SetPeriodTicks(aml_instance_t instance, uint32_t ticks);

/*!
 * @brief Returns timer period (in timer ticks).
 *
 * @param instance Timer peripheral instance number.
 *
 * @return Period value (number of counter ticks).
 */
uint32_t TMR_AML_GetPeriodTicks(aml_instance_t instance);

/*!
 * @brief Sets timer channel offset (in timer ticks).
 *
 * @param instance Timer peripheral instance number.
 * @param channel Index of the timer channel (starts with 0).
 * @param ticks Channel offset (in timer ticks).
 */
void TMR_AML_SetOffsetTicks(aml_instance_t instance, uint8_t channel, uint32_t ticks);

/*!
 * @brief Sets timer channel offset (in timer ticks).
 *
 * @param instance Timer peripheral instance number.
 * @param channel Index of the timer channel (starts with 0).
 *
 * @return Channel offset (in timer ticks).
 */
uint32_t TMR_AML_GetOffsetTicks(aml_instance_t instance, uint8_t channel);

/**
 * @brief Gets instance for timer module base pointer.
 *
 * @param tmrBase Base pointer to I2C peripheral module
 * @return instance number
 */
uint32_t TMR_AML_GetInstance(const void *tmrBase);

/*! @} */

#endif /* SOURCE_TMR_AML_H_ */
/*******************************************************************************
 * EOF
 ******************************************************************************/
