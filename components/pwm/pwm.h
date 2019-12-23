/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PWM_H_
#define _PWM_H_

/*!
 * @addtogroup PWM_Adapter
 * @{
 */

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
***********************************************************************************/

/************************************************************************************
*************************************************************************************
* Public types
*************************************************************************************
************************************************************************************/
/*! @brief Hal pwm mode. */
typedef enum _hal_pwm_mode
{
    kHAL_EdgeAlignedPwm = 0U, /*!< Edge aligned PWM */
    kHAL_CenterAlignedPwm,    /*!< Center aligned PWM */
} hal_pwm_mode_t;

/*! @brief PWM output pulse level select: high-true, low-true or no output */
typedef enum _hal_pwm_level_select
{
    kHAL_PwmNoPwmSignal = 0U, /*!< No PWM output on pin */
    kHAL_PwmLowTrue,          /*!< Low true pulses */
    kHAL_PwmHighTrue          /*!< High true pulses */
} hal_pwm_level_select_t;

/*! @brief Hal pwm status. */
typedef enum _hal_pwm_status
{
    kStatus_HAL_PwmSuccess     = kStatus_Success,                      /*!< Success */
    kStatus_HAL_PwmFail        = MAKE_STATUS(kStatusGroup_HAL_PWM, 1), /*!< Failure*/
    kStatus_HAL_PwmNotSupport  = MAKE_STATUS(kStatusGroup_HAL_PWM, 2), /*!< Not support*/
    kStatus_HAL_PwmOutOfRanger = MAKE_STATUS(kStatusGroup_HAL_PWM, 3), /*!< Pwm is Out Of Ranger */
} hal_pwm_status_t;

/*! @brief hal pwm configuration structure for hal pwm setting. */
typedef struct _hal_pwm_setup_config
{
    hal_pwm_level_select_t level; /*!< PWM output pulse level select */
    hal_pwm_mode_t mode;          /*!< PWM mode select */
    uint32_t pwmFreq_Hz;          /*!< PWM frequency */
    uint8_t dutyCyclePercent;     /*!< PWM duty cycle percent */
} hal_pwm_setup_config_t;

#define HAL_PWM_HANDLE_SIZE (8U)

/*! @brief Hal pwm handle. */
typedef void *hal_pwm_handle_t;
/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* _cplusplus */

/*!
 * @brief Initializes the pwm adapter module for a pwm basic operation.
 *
 * @note This API should be called at the beginning of the application using the pwm adapter.
 *  @code
 *   uint32_t halPwmHandleBuffer[((HAL_PWM_HANDLE_SIZE + sizeof(uint32_t) - 1) / sizeof(uitn32_t))];
 *   hal_pwm_handle_t halPwmHandle = (hal_pwm_handle_t)&halPwmHandleBuffer[0];
 *   HAL_PwmInit(halPwmHandle, BOARD_PWM_INSTANCE, BOARD_PWM_SOURCE_CLOCK);
 *  @endcode
 *
 * @param  halPwmHandle Hal pwm adapter handle, the handle buffer with size #HAL_PWM_HANDLE_SIZE should be
 * allocated at upper level
 * The handle should be 4 byte aligned, because unaligned access does not support on some devices.
 * @param  instance The instance index of the hardware PWM. For example, if FTM is used as the PWM hardware,
 * 0 should be set to "instance" to use FTM0; 2 should be set to "instance" to use FTM2
 * detail information please refer to the SOC corresponding RM.
 * Invalid instance value will cause initialization failure.
 * @param  srcClock_Hz Frequency of source clock of the pwm module
 * @retval kStatus_HAL_PwmSuccess pwm initialization succeed
 *
 */
hal_pwm_status_t HAL_PwmInit(hal_pwm_handle_t halPwmHandle, uint8_t instance, uint32_t srcClock_Hz);

/*!
 * @brief DeInitilizate the pwm adapter module.
 *
 * @note This API should be called when not using the pwm adapter driver anymore.
 *
 * @param halPwmHandle     Hal pwm adapter handle
 */
void HAL_PwmDeinit(hal_pwm_handle_t halPwmHandle);

/*!
 * @brief setup pwm.
 *
 * @note This API should be called when setup the pwm.
 *
 * @param halPwmHandle         Hal pwm adapter handle
 * @param channel             Channel of pwm
 * @param setupConfig         A pointer to the HAL pwm setup configuration structure
 * @retval kStatus_HAL_PwmSuccess pwm setup succeed
 */
hal_pwm_status_t HAL_PwmSetupPwm(hal_pwm_handle_t halPwmHandle, uint8_t channel, hal_pwm_setup_config_t *setupConfig);

/*!
 * @brief Update duty cycle of pwm.
 *
 * @note This API should be called when need update duty cycle.
 *
 * @param halPwmHandle         Hal pwm adapter handle
 * @param channel             Channel of pwm
 * @param mode                 PWM mode select
 * @param dutyCyclePercent     PWM duty cycle percent
 * @retval kStatus_HAL_PwmSuccess pwm Update duty cycle succeed
 */
hal_pwm_status_t HAL_PwmUpdateDutycycle(hal_pwm_handle_t halPwmHandle,
                                        uint8_t channel,
                                        hal_pwm_mode_t mode,
                                        uint8_t dutyCyclePercent);

#if defined(__cplusplus)
}
#endif
/*! @}*/
#endif /* _PWM_H_ */
