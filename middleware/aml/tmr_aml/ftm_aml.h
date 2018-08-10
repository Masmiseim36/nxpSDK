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
 * @file: ftm_aml.h
 *
 * FTM driver file. It is intended for S32 SDK and MCUXpresso SDK.
 * API of the driver is the same as API of TPM AML driver. You can include
 * appropriate header file (TPM or FTM) and use a timer without any dependency
 * on the timer type.
 */

#ifndef SOURCE_FTM_AML_H_
#define SOURCE_FTM_AML_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "../common_aml.h"
#include "tmr_aml.h"

#if (TMR_AML_TYPE == TMR_AML_FTM)
#if (SDK_VERSION == SDK_KSDK)

#include "fsl_ftm.h"

#elif (SDK_VERSION == SDK_S32)

#include "stddef.h"
#include "ftm_ic_driver.h"
#include "ftm_mc_driver.h"
#include "ftm_qd_driver.h"
#include "ftm_oc_driver.h"
#include "ftm_pwm_driver.h"

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
    tmrIntFault = (1U << 8),        /*!< Fault interrupt. */
    tmrIntTimeOverflow = (1U << 9), /*!< Timer overflow interrupt. */
    tmrIntChnlTrigger = (1U << 10), /*!< Channel trigger interrupt. */
    tmrIntReload = (1U << 11)       /*!< Reload Flag; Available only on certain SoC's */
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
    tmrStsFault = (1U << 8),        /*!< Fault Flag */
    tmrStsTimeOverflow = (1U << 9), /*!< Time overflow flag. */
    tmrStsChnlTrigger = (1U << 10), /*!< Channel trigger Flag */
    tmrStsReload = (1U << 11)       /*!< Reload Flag; Available only on certain SoC's */
} tmr_aml_status_flags_t;
/*! @} */

#if (SDK_VERSION == SDK_KSDK)        /* Definition of KSDK specific structures. */

/*!
 * @addtogroup struct_group
 * @{
 */
/*! @brief PWM mode configuration for FTM device in KSDK. */
typedef struct
{
    ftm_chnl_pwm_signal_param_t *chnlParam; /*!< Configuration of PWM channel's. */
    uint8_t numOfChnls;         /*!< Number of channels to configure. */
    ftm_pwm_mode_t mode;        /*!< The counter frequency in Hz. */
    uint32_t pwmFreq_Hz;        /*!< The counter frequency in Hz. */
    uint32_t srcClck_Hz;        /*!< The counter clock in Hz. */
} tmr_sdk_pwm_t;

/*! @brief Configuration of a channel for output compare mode. It contains
 * input parameters of SetupOutputCompare function of FTM device in KSDK. */
typedef struct
{
    ftm_chnl_t chnlNumber;          /*!< The channel number. */
    ftm_output_compare_mode_t mode; /*!< The output action. */
    uint32_t compareValue;          /*!< Value to be programmed in the CnV
                                         register. */
} tmr_sdk_outcmp_ch_t;

/*! @brief Output compare mode configuration of FTM channels in KSDK. */
typedef struct
{
    uint8_t numOfChnls;             /*!< Number of channels to configure. */
    tmr_sdk_outcmp_ch_t *chConfig;  /*!< Array of PWM channels parameters. */
} ftm_output_cmp_param_t;

/*! @brief Configuration of a channel for input capture mode. It contains
 * input parameters of SetupInputCapture function of FTM device in KSDK. */
typedef struct
{
    ftm_chnl_t chnlNumber;          /*!< The channel number. */
    ftm_input_capture_edge_t mode;  /*!< Selection of capture edge. */
    uint32_t filterValue;           /*!< Filter value, specify 0 to disable filter. */
} tmr_sdk_incpt_ch_t;

/*! @brief Input capture mode configuration of FTM channels in KSDK. */
typedef struct
{
    uint8_t numOfChnls;             /*!< Number of channels to configure. */
    tmr_sdk_incpt_ch_t *chConfig;   /*!< Array of PWM channels parameters. */
} tmr_sdk_incpt_t;

/* END of KSDK specific structures definition. */
#elif(SDK_VERSION == SDK_S32)

/* END of S32 SDK specific structures definition. */
#endif

/*! @brief Initial configuration of the FTM device for KSDK. */
typedef struct {

#if (SDK_VERSION == SDK_KSDK)

    ftm_config_t tmrConf;           /*!< FTM configuration passed into FTM_Init
                                         function. */
    tmr_sdk_pwm_t tmrPwmConf;       /*!< FTM configuration including frequency
                                         and channels configuration for PWM mode. */
    ftm_output_cmp_param_t tmrCmpConf;/*!< FTM configuration of channels for output
                                         compare mode. */
    tmr_sdk_incpt_t tmrCptConf;     /*!< FTM configuration of channels for input
                                         capture mode. */
    ftm_clock_source_t clkSrc;      /*!< Timer clock source selection. */

#elif (SDK_VERSION == SDK_S32)

    ftm_user_config_t tmrConf;     /*!< FTM configuration passed into FTM_Init
                                         function. */
    ftm_pwm_param_t tmrPwmConf;    /*!< FTM configuration including frequency
                                         and channels configuration for PWM mode. */
    ftm_output_cmp_param_t tmrCmpConf; /*!< FTM configuration of channels for
                                         output compare mode. */
    ftm_input_param_t tmrCptConf;  /*!< FTM configuration of channels for input
                                         capture mode. */

#endif

} tmr_sdk_config_t;

/*! @brief Device data structure needed by AML driver in KSDK. */
typedef struct
{
#if (SDK_VERSION == SDK_KSDK)

    ftm_clock_source_t clkSrc;      /*!< Clock source selection. */
#elif(SDK_VERSION == SDK_S32)

    ftm_clock_source_t clkSrc;      /*!< Clock source selection. */
#endif
} tmr_data_t;

/*! @name Timer configuration (SDK specific)
 *
 * General timer configuration and PWM mode configuration are needed by AML driver.
 * Types are defined according to used SDK (KSDK or S32) and peripheral type
 * (TPM or FTM).
 */
#if (SDK_VERSION == SDK_KSDK)

/*! @brief SDK specific timer general configuration. */
typedef ftm_config_t tmr_config_t;
/*! @brief SDK specific PWM mode configuration of a channel. */
typedef ftm_chnl_pwm_signal_param_t tmr_sdk_pwm_ch_config_t;

#else

/*! @brief SDK specific timer general configuration. */
typedef ftm_user_config_t tmr_config_t;
/*! @brief SDK specific PWM mode configuration of a channel. */
typedef ftm_independent_ch_param_t tmr_sdk_pwm_ch_config_t;
/*! @brief SDK specific PWM mode configuration. */
typedef ftm_pwm_param_t tmr_sdk_pwm_t;
/*! @brief SDK specific Output compare mode configuration of a channel. */
typedef ftm_output_cmp_ch_param_t tmr_sdk_outcmp_ch_t;
/*! @brief SDK specific Output compare mode configuration. */
typedef ftm_input_ch_param_t tmr_sdk_incpt_ch_t;

#endif

/*! @} */

#endif /* End of timer type check (TMR_AML_TYPE == TMR_AML_FTM). */
#endif /* SOURCE_FTM_AML_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
