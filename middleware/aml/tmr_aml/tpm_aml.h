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
 * @file tpm_aml.h
 *
 * TPM peripheral driver intended for MCUXpresso SDK.
 */

#ifndef SOURCE_TPM_AML_H_
#define SOURCE_TPM_AML_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "../common_aml.h"
#include "tmr_aml.h"

#if (TMR_AML_TYPE == TMR_AML_TPM)

#if (SDK_VERSION == SDK_KSDK)

#include "fsl_tpm.h"

#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @addtogroup enum_group
 * @{
 */
/*! @brief Timer interrupt flags. */
typedef enum
{
    tmrIntChnl0 = (1U << 0),        /*!< Channel 0 interrupt. */
    tmrIntChnl1 = (1U << 1),        /*!< Channel 1 interrupt. */
    tmrIntChnl2 = (1U << 2),        /*!< Channel 2 interrupt. */
    tmrIntChnl3 = (1U << 3),        /*!< Channel 3 interrupt. */
    tmrIntChnl4 = (1U << 4),        /*!< Channel 4 interrupt. */
    tmrIntChnl5 = (1U << 5),        /*!< Channel 5 interrupt. */
    tmrIntChnl6 = (1U << 6),        /*!< Channel 6 interrupt. */
    tmrIntChnl7 = (1U << 7),        /*!< Channel 7 interrupt. */
    tmrIntTimeOverflow = (1U << 8)  /*!< Timer overflow interrupt. */
} tmr_aml_int_flags_t;

/*! @brief Timer status flags. */
typedef enum
{
    tmrStsChnl0 = (1U << 0),        /*!< Channel 0 flag. */
    tmrStsChnl1 = (1U << 1),        /*!< Channel 1 flag. */
    tmrStsChnl2 = (1U << 2),        /*!< Channel 2 flag. */
    tmrStsChnl3 = (1U << 3),        /*!< Channel 3 flag. */
    tmrStsChnl4 = (1U << 4),        /*!< Channel 4 flag. */
    tmrStsChnl5 = (1U << 5),        /*!< Channel 5 flag. */
    tmrStsChnl6 = (1U << 6),        /*!< Channel 6 flag. */
    tmrStsChnl7 = (1U << 7),        /*!< Channel 7 flag. */
    tmrStsTimeOverflow = (1U << 8)  /*!< Time overflow flag. */
} tmr_aml_status_flags_t;
/*! @} */

/*!
 * @addtogroup struct_group
 * @{
 */
/* Definition of KSDK specific structures. */
#if (SDK_VERSION == SDK_KSDK)

/*! @brief PWM mode configuration for TPM device for KSDK. */
typedef struct
{
    tpm_chnl_pwm_signal_param_t *chnlParam; /*!< Configuration of PWM channel's. */
    uint8_t numOfChnls;             /*!< Number of channels to configure. */
    tpm_pwm_mode_t mode;            /*!< The counter operation mode. */
} tmr_sdk_pwm_t;

/*! @brief Configuration of a channel for output compare mode. It contains
 * input parameters of SetupOutputCompare function of TPM device for KSDK. */
typedef struct
{
    tpm_chnl_t chnlNumber;          /*!< The channel number. */
    tpm_output_compare_mode_t mode; /*!< The output action. */
    uint32_t compareValue;          /*!< Value to be programmed in the CnV
                                         register. */
} tmr_sdk_outcmp_ch_t;

/*! @brief Output compare mode configuration of TPM channels for KSDK. */
typedef struct
{
    uint8_t numOfChnls;             /*!< Number of channels to configure. */
    tmr_sdk_outcmp_ch_t *chConfig;  /*!< Array of PWM channels parameters. */
} tmr_sdk_outcmp_t;

/*! @brief Configuration of a channel for input capture mode. It contains
 * input parameters of SetupInputCapture function of TPM device for KSDK. */
typedef struct
{
    tpm_chnl_t chnlNumber;          /*!< The channel number. */
    tpm_input_capture_edge_t mode;  /*!< Selection of capture edge. */
} tmr_sdk_incpt_ch_t;

/*! @brief Input capture mode configuration of TPM channels for KSDK. */
typedef struct
{
    uint8_t numOfChnls;             /*!< Number of channels to configure. */
    tmr_sdk_incpt_ch_t *chConfig;   /*!< Array of PWM channels parameters. */
} tmr_sdk_incpt_t;

#endif  /* END of KSDK specific structures definition. */

/*!
 * @brief Initial configuration of the TPM device for KSDK.
 *
 * This structure contains all structures needed for TPM configuration for
 * each SDK.
 */
typedef struct
{
#if (SDK_VERSION == SDK_KSDK)

    tpm_config_t tmrConf;          /*!< TPM configuration passed into TPM_Init
                                         function. */
    uint32_t pwmFreq_Hz;            /*!< The counter frequency in Hz. */
    uint32_t srcClck_Hz;            /*!< The counter clock in Hz. */
    tmr_sdk_pwm_t tmrPwmConf;      /*!< TPM configuration including frequency
                                         and channels configuration for PWM mode. */
    tmr_sdk_outcmp_t tmrCmpConf;   /*!< TPM configuration of channels for output
                                         compare mode. */
    tmr_sdk_incpt_t tmrCptConf;    /*!< TPM configuration of channels for input
                                         capture mode. */
    tpm_clock_source_t clkSrc;      /*!< Timer clock source selection. */

#endif

} tmr_sdk_config_t;

/*! @brief Device data structure needed by AML driver in KSDK. */
typedef struct
{
#if (SDK_VERSION == SDK_KSDK)

    tpm_clock_source_t clkSrc;      /*!< Clock source selection. */

#endif
} tmr_data_t;

/*! @name Timer configuration (SDK specific)
 *
 * General timer configuration and PWM mode configuration are needed by AML driver.
 * Types are defined according to used SDK (KSDK or S32 SDK) and peripheral type
 * (TPM or FTM).
 */
#if (SDK_VERSION == SDK_KSDK)

/*! @brief SDK specific timer general configuration. */
typedef tpm_config_t tmr_config_t;
/*! @brief SDK specific PWM mode configuration of a channel. */
typedef tpm_chnl_pwm_signal_param_t tmr_sdk_pwm_ch_config_t;

#else

/* S32 SDK does not contain any driver for TPM peripheral. */
typedef void tmr_config_t;

#endif

/*! @} */

#endif /* END of TPM timer check. */

#endif /* SOURCE_TPM_AML_H_ */
/*******************************************************************************
 * EOF
 ******************************************************************************/
