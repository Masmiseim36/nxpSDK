/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*!
 * @file lvhb.h
 *
 * Low voltage H-Bridge driver based on AML layer.
 * Supports boards based on MPC17510, MPC17511, MPC17529, MPC17531A, MPC17533,
 * MPC17C724 and MC34933.
 *
 * This module is common for all supported models.
 */

#ifndef __LVHB_H__
#define __LVHB_H__

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "stdbool.h"
#include "stdint.h"

#include "gpio_aml.h"
#include "tmr_aml/tmr_aml.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Maximal number of H-bridge interfaces in supported devices. */
#define LVHB_BRIDGE_COUNT_MAX       2U

/*! @brief Maximal PWM frequency for speed control of brushed motors. */
#define LVHB_BRUSH_PWM_FREQ_MAX     20000U

/*! @brief Speed control PWM frequency of brushed motors set in default configuration. */
#define LVHB_BRUSH_PWM_FREQ_DEF     10000U

/*! @brief Motor direction of brushed motors set in default configuration. */
#define LVHB_BRUSH_DIR_DEF          lvhbDirectionAHighBLow

/*! @brief Recirculation side of brushed motors set in default configuration. */
#define LVHB_BRUSH_REC_DEF          lvhbRecirculationLow

/*! @brief Initial timer PWM frequency in timer configuration for stepper motors. */
#define LVHB_STEPPER_INIT_PWM_FREQ  1000U

/*! @brief Maximal number of steps that can be executed. */
#define LVHB_STEPPER_MAX_STEPS      100000000U

/*! @brief Minimal timer input frequency for full-stepping. */
#define LVHB_FULLSTEP_IN_FREQ_MIN   131072UL

/*! @brief Maximal timer input frequency for full-stepping. */
#define LVHB_FULLSTEP_IN_FREQ_MAX   10000000UL

/*! @brief Minimal speed in full-step mode (full-steps per second). */
#define LVHB_FULLSTEP_SPEED_MIN     1U

/*! @brief Maximal speed in full-step mode (full-steps per second). */
#define LVHB_FULLSTEP_SPEED_MAX     5000U

/*! @brief Speed in full-step mode set in default configuration. */
#define LVHB_FULLSTEP_SPEED_DEF     100U

/*! @brief Maximal acceleration in full-step mode. */
#define LVHB_FULLSTEP_ACCEL_MAX     5000U

/*! @brief Acceleration in full-step mode set in default configuration. */
#define LVHB_FULLSTEP_ACCEL_DEF     0U

/*! @brief Number of full-steps per one electrical cycle. */
#define LVHB_FULLSTEP_CYCLE         4U

/*! @brief Number of full-steps to next timer restart interrupt in continual mode. */
#define LVHB_FULLSTEP_CNT_CONT      1U

/*! @brief Output value of channel is Low. */
#define LVHB_FULLSTEP_CH_INACTIVE   0x00U

/*! @brief Output value of channel IN1A is High. */
#define LVHB_FULLSTEP_CH_IN1A       0x01U

/*! @brief Output value of channel IN1B is High. */
#define LVHB_FULLSTEP_CH_IN1B       0x02U

/*! @brief Output value of channel IN2A is High. */
#define LVHB_FULLSTEP_CH_IN2A       0x04U

/*! @brief Output value of channel IN2B is High. */
#define LVHB_FULLSTEP_CH_IN2B       0x08U

/*! @brief Minimal timer input frequency for micro-stepping. */
#define LVHB_MICROSTEP_IN_FREQ_MIN  1200000UL

/*! @brief Maximal timer input frequency for micro-stepping. */
#define LVHB_MICROSTEP_IN_FREQ_MAX  10000000UL

/*! @brief Minimal PWM frequency for micro-stepping. */
#define LVHB_MICROSTEP_PWM_FREQ_MIN 10000U

/*! @brief Maximal PWM frequency for micro-stepping. */
#define LVHB_MICROSTEP_PWM_FREQ_MAX 20000U

/*! @brief PWM frequency for micro-stepping set in default configuration.. */
#define LVHB_MICROSTEP_PWM_FREQ_DEF 20000U

/*! @brief Minimal speed in micro-step mode (micro-steps per second). */
#define LVHB_MICROSTEP_SPEED_MIN    1U

/*! @brief Maximal speed in micro-step mode (micro-steps per second). */
#define LVHB_MICROSTEP_SPEED_MAX    5000U

/*! @brief Speed in micro-step mode set in default configuration. */
#define LVHB_MICROSTEP_SPEED_DEF    100U

/*! @brief Maximal acceleration in micro-step mode. */
#define LVHB_MICROSTEP_ACCEL_MAX    5000U

/*! @brief Acceleration in micro-step mode set in default configuration. */
#define LVHB_MICROSTEP_ACCEL_DEF    0U

/*! @brief Maximal number of micro-steps per full-step. */
#define LVHB_MICROSTEP_CNT_MAX      32

/*! @brief Micro-steps per full-step set in default configuration. */
#define LVHB_MICROSTEP_PER_FS_DEF   lvhbMicroStep2

/*! @brief Initial value of index for winding 1 that represents physical position of full-step no. 0. */
#define LVHB_WIND1_IDX_INIT         (LVHB_MICROSTEP_CNT_MAX / 2U)

/*! @brief Initial value of index for winding 2 that represents physical position of full-step no. 0. */
#define LVHB_WIND2_IDX_INIT         (LVHB_MICROSTEP_CNT_MAX + (LVHB_MICROSTEP_CNT_MAX / 2U))

/*! @brief Maximum PWM channels used by LVHB driver. */
#define LVHB_PWM_CH_MAX             4

/*!
 * @brief Returns maximal value of counter.
 *
 * @param drvConfig Pointer to the LVHB driver instance configuration.
 * @return Maximal value of counter + 1.
 */
#define LVHB_TIMER_VALUE_MAX(drvConfig) \
    (1U << (drvConfig->tmrLvhbConfig.counterWidth))

/*!
 * @brief Returns timer input frequency for full-stepping.
 *
 * @param drvConfig Pointer to the LVHB driver instance configuration.
 * @return Timer input frequency for full-stepping.
 */
#define LVHB_FULLSTEP_IN_FREQ(drvConfig) \
    (drvConfig->tmrLvhbConfig.srcClck_Hz >> drvConfig->tmrLvhbConfig.prescale)

/*!
 * @brief Returns factored frequency for full-step ramp.
 *
 * @param drvConfig Pointer to the LVHB driver instance configuration.
 * @return Factored frequency for full-step ramp.
 */
#define LVHB_FULLSTEP_IN_FREQ_0676(drvConfig) \
    ((uint32_t)(LVHB_FULLSTEP_IN_FREQ(drvConfig) * 0.676))

/*!
 * @brief Returns number of timer ticks per timer period.
 *
 * @param drvConfig Pointer to the LVHB driver instance configuration.
 * @param stepsSec Number of steps per second.
 * @return Number of timer ticks.
 */
#define LVHB_GET_FULL_SPEED_TICKS(drvConfig, stepsSec) \
    (LVHB_FULLSTEP_IN_FREQ(drvConfig) / (stepsSec))

/*!
 * @brief Returns timer input frequency for micro-stepping.
 *
 * @param drvConfig Pointer to the LVHB driver instance configuration.
 * @return Timer input frequency for micro-stepping.
 */
#define LVHB_MICROSTEP_IN_FREQ(drvConfig) \
    (drvConfig->tmrLvhbConfig.srcClck_Hz >> drvConfig->tmrLvhbConfig.prescale)

/*!
 * @brief Returns factored frequency for micro-step ramp.
 *
 * @param drvConfig Pointer to the LVHB driver instance configuration.
 * @return Factored frequency for micro-step ramp.
 */
#define LVHB_MICROSTEP_IN_FREQ_0676(drvConfig) \
    ((uint32_t)(LVHB_MICROSTEP_IN_FREQ(drvConfig) * 0.676))

/*!
 * @brief Returns timer period in micro-step mode (ticks per period).
 *
 * @param drvConfig Pointer to the LVHB driver instance configuration.
 * @return Timer period in micro-step mode (ticks per period).
 */
#define LVHB_MICROSTEP_PERIOD_TICKS(drvConfig) \
    (LVHB_MICROSTEP_IN_FREQ(drvConfig) / drvConfig->deviceConfig.stepperConfig.microStepPwmFrequency)

/*!
 * @brief Returns number of micro-steps per second.
 *
 * @param drvConfig Pointer to the LVHB driver instance configuration.
 * @param speedOverflows Micro-step speed in number of timer overflows.
 * @return Number of micro-steps per second.
 */
#define LVHB_GET_MICRO_SPEED(drvConfig, speedOverflows) \
    (((uint32_t)LVHB_MICROSTEP_IN_FREQ(drvConfig) / LVHB_MICROSTEP_PERIOD_TICKS(drvConfig)) / (speedOverflows))

/*!
 * @brief Returns number of timer overflows according to desired speed.
 *
 * @param drvConfig Pointer to the LVHB driver instance configuration.
 * @param microStepsSec Micro-step speed in number of steps per second.
 * @return Number of overflows.
 */
#define LVHB_GET_MICRO_SPEED_OF(drvConfig, microStepsSec) \
    ((uint32_t)(drvConfig->deviceConfig.stepperConfig.microStepPwmFrequency / (microStepsSec)));

/*!
 * @brief Returns offset for specified micro-step.
 *
 * @param drvConfig Pointer to the LVHB driver instance configuration.
 * @param activeChannel Channel output value (one of LVHB_FULLSTEP_CH_INxx constants).
 * @param fullStepVal Active channels in current full-step. Value is logic OR of
 *        channels output values (LVHB_FULLSTEP_CH_INX constants).
 * @param tableIdx Index to the table with micro-step offsets.
 * @return Channel offset.
 */
#define LVHB_GET_MICRO_OFFSET(drvConfig, activeChannel, fullStepVal, tableIdx) \
    ((((activeChannel) & (fullStepVal)) != LVHB_FULLSTEP_CH_INACTIVE) ? drvConfig->deviceConfig.stepperData.microStepOffset[tableIdx] : 0U)

/*!
 * @brief Compute remainder after integer division.
 *
 * Remainder = dividend mod divisor = dividend - (divisor * (dividend / divisor)).
 *
 * @param dividend Expression dividend.
 * @param divisor Expression divisor.
 * @param fraction Equal to dividend div divisor.
 * @return Remainder after integer division.
 */
#define LVHB_COMPUTE_REMAINDER(dividend, divisor, fraction) \
    (((dividend) < (divisor)) ? (dividend) : (dividend) - ((divisor) * (fraction)))

/*!
 * @brief Checks if the device has two H-Bridges.
 *
 * @param device Device from lvhb_device_t enumeration.
 * @return True if supported device has two H-Bridges.
 */
#define LVHB_HAS_TWO_BRIDGES(device) \
    ((device != lvhbDeviceMPC17510) && (device != lvhbDeviceMPC17511))

/*!
 * @brief Checks if the device is MPC17510 or MPC17511.
 *
 * @param device Device from lvhb_device_t enumeration.
 * @return True if device is MPC17510 or MPC17511.
 */
#define LVHB_IS_MPC1751x(device) \
    ((device == lvhbDeviceMPC17510) || (device == lvhbDeviceMPC17511))

/*!
 * @brief Checks if the bridge can be selected in LVHB configuration.
 *
 * Bridge can be selected from lvhb_bridge_t enumeration only.
 * LvhbBridge1 can be selected everytime. LvhbBridge2 can be selected
 * if device with two H-bridges is chosen and stepper motor or
 * secondaryBridgeUsed variable is set in driver configuration.
 *
 * @param drvConfig Pointer to the LVHB driver instance configuration.
 * @param bridge Bridge from lvhb_bridge_t enumeration.
 * @return True if bridge can be selected.
 */
#define LVHB_CHECK_BRIDGE(drvConfig, bridge)                  \
    ((bridge <= lvhbBridge2) &&                               \
     ((bridge == lvhbBridge1) ||                              \
      (LVHB_HAS_TWO_BRIDGES(drvConfig->deviceConfig.device) && (LVHB_IS_STEPPER_SELECTED(drvConfig) || drvConfig->deviceConfig.secondaryBridgeUsed))))

/*!
 * @brief Checks if the device has GIN pin.
 *
 * @param device Device from lvhb_device_t enumeration.
 * @return True if device has GIN pin.
 */
#define LVHB_HAS_GIN_PIN(device) \
    ((device == lvhbDeviceMPC17510) || (device == lvhbDeviceMPC17511))

/*!
 * @brief Checks if the device has EN/OE/PSAVE pin.
 *
 * @param device Device from lvhb_device_t enumeration.
 * @return True if device has EN/OE/PSAVE pin.
 */
#define LVHB_HAS_EN_PIN(device) \
    (device != lvhbDeviceMC34933)

/*!
 * @brief Checks if all four INxA and INxB pins are set as GPIO pins in the driver configuration.
 *
 * @param drvConfig Pointer to the LVHB driver instance configuration.
 * @return True if all four INxA and INxB pins are set as GPIO pins.
 */
#define LVHB_HAS_GPIO_IN_PINS(drvConfig) \
    ((drvConfig->inputPins[lvhbBridge1] == lvhbPinsGpio) && (drvConfig->inputPins[lvhbBridge2] == lvhbPinsGpio))

/*!
 * @brief Checks if all four INxA and INxB pins are set as PWM outputs in the driver configuration.
 *
 * @param drvConfig Pointer to the LVHB driver instance configuration.
 * @return True if all four INxA and INxB pins are set as PWM outputs.
 */
#define LVHB_HAS_PWM_IN_PINS(drvConfig) \
    ((drvConfig->inputPins[lvhbBridge1] == lvhbPinsPwm) && (drvConfig->inputPins[lvhbBridge2] == lvhbPinsPwm))

/*!
 * @brief Checks if stepper motor is selected in the driver configuration.
 *
 * @param drvConfig Pointer to the LVHB driver instance configuration.
 * @return True if stepper motor is selected.
 */
#define LVHB_IS_STEPPER_SELECTED(drvConfig) \
    (drvConfig->deviceConfig.motorType == lvhbMotorStepper)

/*!
 * @brief Checks if brushed motor is selected in the driver configuration.
 *
 * @param drvConfig Pointer to the LVHB driver instance configuration.
 * @return True if brushed motor is selected.
 */
#define LVHB_IS_BRUSHED_MOTOR_SELECTED(drvConfig) \
    (drvConfig->deviceConfig.motorType == lvhbMotorBrushed)

/* Enum types definition. */
/*!
 * @addtogroup enum_group
 * @{
 */
/*! @brief Error codes. */
enum _lvhb_status
{
    kStatus_LVHB_TimerInit              = MAKE_STATUS(kStatusGroup_LVHB, 0), /*!< Wrong parameter in tmr_lvhb_config_t or wrong Timer initialization. */
    kStatus_LVHB_InvalidDriverConfig    = MAKE_STATUS(kStatusGroup_LVHB, 1), /*!< Invalid driver configuration. */
    kStatus_LVHB_InvalidArgument        = MAKE_STATUS(kStatusGroup_LVHB, 2), /*!< Invalid function argument. */
    kStatus_LVHB_TimerFrequency         = MAKE_STATUS(kStatusGroup_LVHB, 3), /*!< Frequency of timer can not be out of range for this application. */
    kStatus_LVHB_NotInContinualMovement = MAKE_STATUS(kStatusGroup_LVHB, 4), /*!< Motor is not running or in continual movement. */
    kStatus_LVHB_DeviceBusy             = MAKE_STATUS(kStatusGroup_LVHB, 5)  /*!< Motor is running. */
};

/*! @brief H-bridge device. */
typedef enum
{
    lvhbDeviceMPC17510 = 0x0U,          /*!< MPC17510. */
    lvhbDeviceMPC17511 = 0x1U,          /*!< MPC17511. */
    lvhbDeviceMPC17529 = 0x2U,          /*!< MPC17529. */
    lvhbDeviceMPC17531A = 0x3U,         /*!< MPC17531A. */
    lvhbDeviceMPC17533 = 0x4U,          /*!< MPC17533. */
    lvhbDeviceMPC17C724 = 0x5U,         /*!< MPC17C724. */
    lvhbDeviceMC34933 = 0x6U            /*!< MC34933. */
} lvhb_device_t;

/*! @brief Bridge of H-bridge. */
typedef enum
{
    lvhbBridge1 = 0x0U,                 /*!< Bridge 1 - IN1A, IN1B (IN1 and IN2 by MPC1751x) pins. */
    lvhbBridge2 = 0x1U                  /*!< Bridge 2 - IN2A, IN2B pins. */
} lvhb_bridge_t;

/*! @brief Motor type. */
typedef enum
{
    lvhbMotorStepper = 0x0U,            /*!< Stepper motor. */
    lvhbMotorBrushed = 0x1U             /*!< Brush motor. */
} lvhb_motor_type_t;

/*! @brief Bridge output direction. */
typedef enum
{
    lvhbDirectionALowBHigh = 0x0U,      /*!< INxA (IN1 by MPC1751x) is Low, INxB (IN2 by MPC1751x) is High. */
    lvhbDirectionAHighBLow = 0x1U       /*!< INxA (IN1 by MPC1751x) is High, INxB (IN2 by MPC1751x) is Low. */
} lvhb_output_direction_t;

/*! @brief Recirculation side. */
typedef enum
{
    lvhbRecirculationLow     = 0x0U,    /*!< Low-side recirculation. */
    lvhbRecirculationHighImp = 0x1U     /*!< High-impedance recirculation. */
} lvhb_recirculation_t;

/*! @brief Output state. */
typedef enum
{
    lvhbOutputOff = 0x0U,               /*!< Output turned off. */
    lvhbOutputOn  = 0x1U                /*!< Output turned on. */
} lvhb_output_state_t;

/*! @brief Bridge input pin types. */
typedef enum
{
    lvhbPinsGpio    = 0x0U,             /*!< Both INxA, INxB (IN1, IN2 by MPC1751x) GPIO - state control only. */
    lvhbPinsGpioPwm = 0x1U,             /*!< INxA (IN1 by MPC1751x) GPIO, INxB (IN2 by MPC1751x) PWM. */
    lvhbPinsPwmGpio = 0x2U,             /*!< INxA (IN1 by MPC1751x) PWM, INxB (IN2 by MPC1751x) GPIO. */
    lvhbPinsPwm     = 0x3U              /*!< Both INxA, INxB (IN1, IN2 by MPC1751x) PWM. */
} lvhb_input_pins_t;

/*! @brief Motor status. */
typedef enum
{
    lvhbStatusRunning = 0x0U,           /*!< Motor is running. */
    lvhbStatusStop = 0x1U               /*!< Motor is not running. */
} lvhb_motor_status_t;

/*! @brief Stepper motor mode. */
typedef enum
{
    lvhbStepperModeMicrostep = 0U,      /*!< Motor is in micro-stepping mode. */
    lvhbStepperModeFullstep = 1U,       /*!< Motor is in full-stepping mode. */
    lvhbStepperModeDisabled = 2U        /*!< Motor is disabled (no signal generated on IN pins). */
} lvhb_stepper_mode_t;

/*! @brief Stepper motor control states. */
typedef enum
{
    lvhbStepperStateSleepLoop = 0U,     /*!< Nothing to do. */
    lvhbStepperStateInitCompleted = 1U, /*!< First part of full-step or micro-step initialization is completed. */
    lvhbStepperStateSlowStart = 2U      /*!< Second part of full-step or micro-step initialization is completed. */
} lvhb_stepper_state_t;

/*! @brief Number of micro-steps per one full-step. */
typedef enum
{
    lvhbMicroStep2  = LVHB_MICROSTEP_CNT_MAX / 2U,  /*!< Half-step mode. 2 micro-steps per one full-step. */
    lvhbMicroStep4  = LVHB_MICROSTEP_CNT_MAX / 4U,  /*!< 4 micro-steps per one full-step. */
    lvhbMicroStep8  = LVHB_MICROSTEP_CNT_MAX / 8U,  /*!< 8 micro-steps per one full-step. */
    lvhbMicroStep16 = LVHB_MICROSTEP_CNT_MAX / 16U, /*!< 16 micro-steps per one full-step. */
    lvhbMicroStep32 = LVHB_MICROSTEP_CNT_MAX / 32U  /*!< 32 micro-steps per one full-step. */
} lvhb_micro_step_t;

/*! @brief States of finite state machine which are used for acceleration ramp. */
typedef enum
{
    lvhbRampOff = 0U,                   /*!< Motor is not running. */
    lvhbRampUpSpeed = 1U,               /*!< Acceleration phase. */
    lvhbRampDownSpeed = 2U,             /*!< Deceleration phase. */
    lvhbRampConstSpeed = 3U             /*!< Motor is running at constant speed. */
} lvhb_ramp_state_t;
/*! @} */

/* Configure struct types definition. */
/*!
 * @addtogroup struct_group
 * @{
 */
/*!
 * @brief Driver specific Timer configuration.
 */
typedef struct
{
    uint8_t counterWidth;                             /*!< Counter width in bits.  */
    uint32_t srcClck_Hz;                              /*!< Timer source clock in Hz. */
    tmr_clk_prescale_t prescale;                      /*!< Timer prescale value. */
    uint8_t inxaChannelNumber[LVHB_BRIDGE_COUNT_MAX]; /*!< INxA/IN1 channel number - used for PWM/PWM and PWM/GPIO pin control only. */
    uint8_t inxbChannelNumber[LVHB_BRIDGE_COUNT_MAX]; /*!< INxB/IN2 channel number - used for PWM/PWM and GPIO/PWM pin control only. */
} tmr_lvhb_config_t;

/*!
 * @brief Brushed motor configuration.
 */
typedef struct {
    lvhb_recirculation_t recirculation;       /*!< Recirculation side (high-impedance, low) of used bridges. */
    lvhb_output_direction_t outputDirection;  /*!< Direction of rotation (state of outputs) of used bridges. */
} lvhb_brush_config_t;

/*!
 * @brief Stepper motor configuration.
 */
typedef struct {
    /* Full-step configuration */
    uint32_t fullStepSpeed;             /*!< Motor speed in Full-step mode. Unit is number of full-steps per second. Admissible range is 1 to 5000. */
    uint32_t fullStepAcceleration;      /*!< Fluent acceleration to desired speed and decelaration to zero. Put 0 value to disable ramp. Unit is full-steps per second^2. */
    /* Micro-step configuration */
    uint32_t microStepSpeed;            /*!< Motor speed in Micro-step mode. Unit is number of micro-steps per second. Admissible range is 1 to 5000. */
    uint32_t microStepAcceleration;     /*!< Fluent acceleration to desired speed and decelaration to zero. Put 0 value to disable ramp. Unit is micro-steps per second^2. */
    lvhb_micro_step_t microStepPerStep; /*!< Number of micro-steps per one full-step. */
    uint32_t microStepPwmFrequency;     /*!< Desired timer frequency for micro-stepping. Minimum value is 10 kHz and maximum value is 20 kHz. */
} lvhb_stepper_config_t;

/*!
 * @brief Stepper motor control data structure.
 */
typedef struct {
    bool forward;                       /*!< Current direction of motor movement. */
    bool continual;                     /*!< Continual movement. */
    uint32_t stepCntr;                  /*!< Counter of steps. */
    uint32_t steps;                     /*!< Number of steps executed in current movement. */
    int32_t stepPos;                    /*!< Actual position of motor in full-steps. Not used in algorithm, only for information. User have to check possible overflow and reset variable by provided function. */
    uint8_t fullStepIdx;                /*!< Index to the table with full-step sequence values (used in micro-step and full-step). */
    lvhb_stepper_mode_t stepperMode;    /*!< Stepper motor mode (Full-stepping, micro-stepping or disabled). */
    lvhb_motor_status_t motorStatus;    /*!< Current motor state. */
    lvhb_stepper_state_t controlState;  /*!< States for control generating steps. */
    uint32_t timerOverflows;            /*!< Number of timer overflows in current step. */
    /* Full-stepping data. */
    uint32_t fullSpeed;                 /*!< Speed in full-step mode in number of timer clock source ticks (equal to timer period). */
    uint8_t fullStepsPerPeriod;         /*!< Number of full-steps executed until next timer restart event (counting of full-steps). */
    uint32_t currentFullSpeed;          /*!< Speed used in current movement. */
    /* Micro-stepping data. */
    uint32_t microSpeed;                /*!< Speed of micro-stepping in number of timer overflows. */
    uint8_t microWind1Idx;              /*!< Index to the table with micro-step offsets. Intended for first winding. */
    uint8_t microWind2Idx;              /*!< Index to the table with micro-step offsets. Intended for second winding. */
    int8_t microPosInFull;              /*!< No. of taken micro-steps from full-step position (it is zero at physic full-step position). */
    uint16_t microStepOffset[LVHB_MICROSTEP_CNT_MAX + 1]; /*!< Table for tmrInstance offset values for 32 micro-steps per one full-step (in number of ticks). */
    /* Data for acceleration ramp. */
    lvhb_ramp_state_t rampState;        /*!< State of the acceleration ramp. */
    uint32_t maxSpeedDelay;             /*!< Timer delay when max speed is reached (Size of 1 full-step in seconds). */
    uint32_t currentDelay;              /*!< Current timer delay. */
    uint32_t currentDelayRest;          /*!< Rest of the division in current timer delay computation. */
    uint32_t decelStepCnt;              /*!< Number of steps for deceleration. */
} lvhb_stepper_data_t;

/*!
 * @brief Device configuration.
 *
 * This structure contains device configuration.
 */
typedef struct {
    lvhb_device_t device;                                   /*!< H-Bridge device. */
    lvhb_motor_type_t motorType;                            /*!< Motor type. */
    lvhb_brush_config_t brushConfig[LVHB_BRIDGE_COUNT_MAX]; /*!< Brush motor configuration. */
    uint32_t brushPwmFrequency;                             /*!< PWM frequency for speed control of brushed motors. Maximum value is 20 kHz. */
    lvhb_stepper_config_t stepperConfig;                    /*!< Stepper motor configuration. */
    lvhb_stepper_data_t stepperData;                        /*!< Stepper motor data. */
    bool secondaryBridgeUsed;                               /*!< Usage of second bridge in case of brush motor control. Not used by MPC17510 and MPC17511. */
    bool activeMode;                                        /*!< Mode of H-Bridge. Used by all H-Bridges except MC34933. */
    bool gateDriverOutputHigh;                              /*!< GIN pin value. Used by MPC17510 and MPC17511 only. */
} lvhb_device_data_t;

/*!
 * @brief Driver configuration.
 *
 * This structure contains all information needed for proper functionality of the driver,
 * such as used peripherals configurations, control pins configuration and device configuration.
 */
typedef struct
{
    /* EN pin settings. */
    aml_instance_t enPinInstance;                           /*!< EN (MPC17510, MPC17511) / OE (MPC17529, MPC17533) / PSAVE (MPC17C724, MPC17531A) port instance. Not used by MC34933. */
    uint8_t enPinIndex;                                     /*!< EN (MPC17510, MPC17511) / OE (MPC17529, MPC17533) / PSAVE (MPC17C724, MPC17531A) pin index. Not used by MC34933. */
    /* GIN pin settings. */
    aml_instance_t ginPinInstance;                          /*!< GIN pin port instance. Used by MPC17510 and MPC17511 only. */
    uint8_t ginPinIndex;                                    /*!< GIN pin index. Used by MPC17510 and MPC17511 only.*/
    /* INx, INxx pins settings */
    lvhb_input_pins_t inputPins[LVHB_BRIDGE_COUNT_MAX];     /*!< GPIO, GPIO/PWM, PWM/GPIO, or PWM. */
    /* IN1/IN1A (first bridge) & IN2A (second bridge) GPIO pin settings. */
    aml_instance_t inxaPinInstance[LVHB_BRIDGE_COUNT_MAX];  /*!< IN1/IN1A (first bridge) & IN2A (second bridge) GPIO pin port instance. */
    uint8_t inxaPinIndex[LVHB_BRIDGE_COUNT_MAX];            /*!< IN1/IN1A (first bridge) & IN2A (second bridge) GPIO pin index. */
    /* IN2/IN1B (first bridge) & IN2B (second bridge) GPIO pin settings. */
    aml_instance_t inxbPinInstance[LVHB_BRIDGE_COUNT_MAX];  /*!< IN2/IN1B (first bridge) & IN2B (second bridge) GPIO pin port instance. */
    uint8_t inxbPinIndex[LVHB_BRIDGE_COUNT_MAX];            /*!< IN2/IN1B (first bridge) & IN2B (second bridge) GPIO pin index. */
    /* Timer settings */
    aml_instance_t tmrInstance;                             /*!< Timer instance - only if any PWM pin used. */
    tmr_lvhb_config_t tmrLvhbConfig;                        /*!< Device Timer configuration - only if any PWM pin used. */
    /* Device settings. */
    lvhb_device_data_t deviceConfig;                        /*!< Device configuration. */
} lvhb_drv_config_t;
/*! @} */

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @addtogroup function_group
 * @{
 */

/*!
 * @brief This function configures GPIO for usage with this driver.
 *
 * This function initializes all used GPIO pins. These pins are identified
 * according to the selected device and INx pin types in the driver configuration.
 *
 * @param drvConfig Pointer to driver instance configuration.
 *
 * @return status_t Error code.
 */
status_t LVHB_ConfigureGpio(lvhb_drv_config_t* const drvConfig);

/*!
 * @brief This function configures Timer for usage with this driver.
 *
 * This function initializes Timer device. There are two parameters.
 * lvhb_drv_config_t must be used always with appropriate driver settings
 * but tmr_sdk_config_t could be used for custom user SDK settings (use
 * NULL pointer for default settings). There is no need to call this
 * function if GPIO control of brushed motors is used.
 *
 * @param drvConfig Pointer to driver instance configuration.
 * @param tmrSdkConfig SDK specific configuration. Use NULL pointer for
 *        default settings.
 *
 * @return status_t Error code.
 */
status_t LVHB_ConfigureTimer(lvhb_drv_config_t* const drvConfig, tmr_sdk_config_t* const tmrSdkConfig);

/*!
 * @brief This function gets a default configuration of the driver for
 * specific device and motor type.
 *
 * Default device configuration will be stored to the defaultConfig pointer.
 *
 * @param drvConfig Pointer to variable where the configuration will be stored.
 * @param device LVHB device.
 * @param motorType Motor type to control by this driver.
 *
 * @return status_t Error code.
 */
status_t LVHB_GetDefaultConfig(lvhb_drv_config_t* const drvConfig, lvhb_device_t device, lvhb_motor_type_t motorType);

/*!
 * @brief This function initializes driver data and output pin values.
 *
 * Initialization of all used peripherals and stepper control variables
 * is performed. GIN and OE/EN/PSAVE pin values are set according to the
 * driver configuration.
 * INx signals are set to get LOW at OUTx pins of used H-Bridge.
 *
 * @param initConfig Pointer to initial driver configuration.
 *
 * @return status_t Error code.
 */
status_t LVHB_Init(lvhb_drv_config_t* const initConfig);

/*!
 * @brief This function deinitializes the driver.
 *
 * @param initConfig Pointer to initial driver configuration.
 *
 * @return status_t Error code.
 */
status_t LVHB_Deinit(lvhb_drv_config_t* const initConfig);

/*!
 * @brief This method sets H-Bridge device mode using enable pin.
 *
 * This function may be used by devices with EN, OE or PSAVE pin
 * only - ie. all supported devices except MC34933.
 *
 * @param drvConfig Pointer to driver instance configuration.
 * @param active H-Bridge mode. Put FALSE to set power
 *        save mode, TRUE for normal operational mode.
 *
 * @return status_t Error code.
 */
status_t LVHB_SetMode(lvhb_drv_config_t* const drvConfig, bool active);

/*!
 * @brief This function controls Gate Driver Input (GIN) pin. It is available
 * for MPC17510 and MPC17511 only.
 *
 * @param drvConfig Pointer to driver instance configuration.
 * @param outputHigh TRUE to set GOUT pin to High, FALSE for Low.
 *
 * @return status_t Error code.
 */
status_t LVHB_SetGateDriver(lvhb_drv_config_t* const drvConfig, bool outputHigh);

/*!
 * @brief This function spins the motor in desired direction at full speed.
 *
 * This function serves to output state control of brushed motors,
 * it must not be used for stepper motor control.
 *
 * @param drvConfig Pointer to driver instance configuration.
 * @param state State of outputs (turned on/off).
 * @param bridge H-Bridge of device the motor is connected to. Only
 *        lvhbBridge1 value is correct when single H-Bridge device is used.
 *
 * @return status_t Error code.
 */
status_t LVHB_RotateFull(lvhb_drv_config_t* const drvConfig, lvhb_output_state_t state, lvhb_bridge_t bridge);

/*!
 * @brief This function spins the motor in desired direction at PWM duty speed.
 *
 * This function serves to speed control of brushed motors. It may be
 * used only for brushed motor control with one non-GPIO pin at least.
 *
 * @param drvConfig Pointer to driver instance configuration.
 * @param pwmDuty - PWM duty, Possible values: 0 - 100,
 *        where 0 is minimum and 100 is maximum.
 * @param bridge H-Bridge of device the motor is connected to. Only
 *        lvhbBridge1 value is correct when single H-Bridge device is used.
 *
 * @return status_t - Error code.
 */
status_t LVHB_RotateProportional(lvhb_drv_config_t* const drvConfig, uint8_t pwmDuty, lvhb_bridge_t bridge);

/*!
 * @brief This function sets output of specified H-Bridge to tri-state
 * (high impedance) using input control pins.
 *
 * This function may be used in case of brushed motor control only.
 *
 * @param drvConfig Pointer to driver instance configuration.
 * @param bridge H-Bridge of device the motor is connected to. Only
 *        lvhbBridge1 value is correct when single H-Bridge device is used.
 *
 * @return status_t Error code.
 */
status_t LVHB_SetTriState(lvhb_drv_config_t* const drvConfig, lvhb_bridge_t bridge);

/*!
 * @brief This function sets direction of brush motor at specified
 * H-Bridge interface.
 *
 * In lvhbDirectionAHighBLow direction the first IN pin is set to high
 * (or PWM) and the second IN pin to low. In reverse direction the first
 * IN is set to low and the second IN to high (or PWM).
 * Change of the direction is applied when you start rotation (not when
 * the motor is running).
 * This function may be used in case of brushed motor control only.
 *
 * @param drvConfig Pointer to driver instance configuration.
 * @param direction Desired motor direction from lvhb_output_direction_t
 *        enumeration.
 * @param bridge H-Bridge of device the motor is connected to. Only
 *        lvhbBridge1 value is correct when single H-Bridge device is used.
 *
 * @return status_t Error code.
 */
status_t LVHB_SetDirection(lvhb_drv_config_t* const drvConfig, lvhb_output_direction_t direction, lvhb_bridge_t bridge);

/*!
 * @brief This function sets low/high-impedance-side recirculation of the H-Bridge.
 *
 * When Low-side recirculation (freewheeling) is held while a DC motor is
 * spinning, the back-EMF of the motor will cause a recirculating current
 * to flow that will rapidly brake the motor to a stop. This is known
 * as dynamic braking.
 *
 * @param drvConfig Pointer to driver instance configuration.
 * @param side Low or High-impedance-side recirculation.
 * @param bridge H-Bridge of device the motor is connected to. Only
 *        lvhbBridge1 value is correct when single H-Bridge device is used.
 *
 * @return status_t Error code.
 */
status_t LVHB_SetRecirculation(lvhb_drv_config_t* const drvConfig, lvhb_recirculation_t side, lvhb_bridge_t bridge);

/*!
 * @brief This function sets the speed of full-step mode.
 *
 * Unit is number of full-steps per second.
 * Note that it is not allowed to change speed while motor is running.
 *
 * This function may be used for the stepper motors control only.
 *
 * @param drvConfig Pointer to driver instance configuration.
 * @param stepsSec Motor speed in number of full-steps per second.
 *        Minimal and maximal speed is defined by LVHB_FULLSTEP_SPEED_MIN
 *        and LVHB_FULLSTEP_SPEED_MAX.
 *
 * @return status_t Error code.
 */
status_t LVHB_SetFullStepSpeed(lvhb_drv_config_t* const drvConfig, uint16_t stepsSec);

/*!
 * @brief This function sets the speed of micro-step mode.
 *
 * Unit is number of micro-steps per second. Size of micro-step depends
 * on current driver setting.
 * Note that it is not allowed to change speed while motor is running.
 *
 * This function may be used for the stepper motors control with
 * micro-stepping support only - ie. all IN pins are used as timer outputs.
 *
 * @param drvConfig Pointer to driver instance configuration.
 * @param microStepsSec Motor speed in number of micro-steps per second.
 *        Minimal and maximal speed is defined by LVHB_MICROSTEP_SPEED_MIN
 *        and LVHB_MICROSTEP_SPEED_MAX.
 *
 * @return status_t Error code.
 */
status_t LVHB_SetMicroStepSpeed(lvhb_drv_config_t* const drvConfig, uint16_t microStepsSec);

/*!
 * @brief This function sets the acceleration ramp of full-step mode.
 *
 * Unit is full-steps per second^2. Put 0 value to disable ramp.
 * Note that it is not allowed to change acceleration ramp while motor
 * is running.
 *
 * This function may be used for the stepper motors control only.
 *
 * @param drvConfig Pointer to driver instance configuration.
 * @param acceleration Motor acceleration in number of full-steps per
 *        second^2. Maximal acceleration is LVHB_FULLSTEP_ACCEL_MAX.
 *
 * @return status_t Error code.
 */
status_t LVHB_SetFullStepAcceleration(lvhb_drv_config_t* const drvConfig, uint32_t acceleration);

/*!
 * @brief This function sets the acceleration ramp of micro-step mode.
 *
 * Unit is micro-steps per second^2. Put 0 value to disable ramp.
 * Note that it is not allowed to change acceleration ramp while motor
 * is running.
 *
 * This function may be used for the stepper motors control with
 * micro-stepping support only - ie. all IN pins are used as timer outputs.
 *
 * @param drvConfig Pointer to driver instance configuration.
 * @param acceleration Motor acceleration in number of micro-steps per
 *        second^2. Maximal acceleration is LVHB_MICROSTEP_ACCEL_MAX.
 *
 * @return status_t Error code.
 */
status_t LVHB_SetMicroStepAcceleration(lvhb_drv_config_t* const drvConfig, uint32_t acceleration);

/*!
 * @brief This function moves motor by specified number of full-steps.
 *
 * When the rotor is not at physical micro-step position then the function
 * sets nearest micro-step without correction.
 * The input timer frequency must be between LVHB_FULLSTEP_IN_FREQ_MIN
 * and LVHB_FULLSTEP_IN_FREQ_MAX.
 * Note that number of steps returned by method LVHB_GetFullStepPosition
 * are updated before they are executed.
 * Note that the motor must not run when this function is called and you
 * must wait for completion of this action before you can run motor again
 * (use method LVHB_GetMotorStatus or event LVHB_OnActionComplete).
 *
 * This function may be used for the stepper motors control only.
 *
 * @param drvConfig Pointer to driver instance configuration.
 * @param forward Direction of movement.
 * @param steps Number of steps to move (equal or less than
 *        LVHB_STEPPER_MAX_STEPS).
 *
 * @return status_t Error code.
 */
status_t LVHB_MoveSteps(lvhb_drv_config_t* const drvConfig, bool forward, uint32_t steps);

/*!
 * @brief This function moves motor by specified number of micro-steps.
 *
 * When the rotor is not at physical micro-step position then the function
 * sets nearest micro-step without correction. For example the size is
 * initialized to 32 micro-steps per one full-step and the motor executed
 * three micro-steps. Then user changes micro-step size to 2 micro-steps
 * per one full-step and starts motor movement (previous three micro-steps
 * are not visible).
 * The input timer frequency must be between LVHB_MICROSTEP_IN_FREQ_MIN
 * and LVHB_MICROSTEP_IN_FREQ_MAX.
 * Note that the motor must not run when this function is called and you
 * must wait for completion of this action before you can run motor again
 * (use method LVHB_GetMotorStatus or event LVHB_OnActionComplete).
 *
 * This function may be used for the stepper motors control with
 * micro-stepping support only - ie. all IN pins are used as timer outputs.
 *
 * @param drvConfig Pointer to driver instance configuration.
 * @param forward Direction of movement.
 * @param microSteps Number of micro-steps to move (equal or less than
 *        LVHB_STEPPER_MAX_STEPS).
 *
 * @return status_t Error code.
 */
status_t LVHB_MoveMicroSteps(lvhb_drv_config_t* const drvConfig, bool forward, uint32_t microSteps);

/*!
 * @brief This function moves motor continually in full-step mode.
 *
 * The motor can be stopped by LVHB_StopContinualMovement function.
 * When the rotor is not at physical full-step position then the function
 * sets nearest full-step without correction.
 * The input timer frequency must be between LVHB_FULLSTEP_IN_FREQ_MIN
 * and LVHB_FULLSTEP_IN_FREQ_MAX.
 * Note that the motor must not run when this function is called.
 *
 * This function may be used only for the stepper motors control when the
 * acceleration ramp is disabled (full-step acceleration equals 0).
 *
 * @param drvConfig Pointer to driver instance configuration.
 * @param forward Direction of movement.
 *
 * @return status_t Error code.
 */
status_t LVHB_MoveContinual(lvhb_drv_config_t* const drvConfig, bool forward);

/*!
 * @brief This function moves motor continually in micro-step mode.
 *
 * The motor can be stopped by LVHB_StopContinualMovement function.
 * When the rotor is not at physical micro-step position then the function
 * sets nearest micro-step without correction. For example the size is
 * initialized to 32 micro-steps per one full-step and the motor executed
 * three micro-steps. Then user changes micro-step size to 2 micro-steps
 * per one full-step and starts motor movement (previous three micro-steps
 * are not visible).
 * The input timer frequency must be between LVHB_MICROSTEP_IN_FREQ_MIN
 * and LVHB_MICROSTEP_IN_FREQ_MAX.
 * Note that the motor must not run when this function is called.
 *
 * This function may be used only for the stepper motors control  with
 * micro-stepping support only (ie. all IN pins are used as timer outputs)
 * when the acceleration ramp is disabled (micro-step acceleration equals 0).
 *
 * @param drvConfig Pointer to driver instance configuration.
 * @param forward Direction of movement.
 *
 * @return status_t Error code.
 */
status_t LVHB_MoveMicroContinual(lvhb_drv_config_t* const drvConfig, bool forward);

/*!
 * @brief This function is intended to stop continual movement of stepper motor.
 *
 * This function does not stop motor immediately, motor can execute several
 * steps. In micro-step mode the motor does not have to stop at full-step
 * position (can stop anywhere).
 * Note that you must wait for completion of this action before you can
 * run motor again (use method LVHB_GetMotorStatus or event
 * LVHB_OnActionComplete).
 *
 * This function may be used for the stepper motors control only.
 *
 * @param drvConfig Pointer to driver instance configuration.
 *
 * @return status_t Error code.
 */
status_t LVHB_StopContinualMovement(lvhb_drv_config_t* const drvConfig);

/*!
 * @brief This function returns status of stepper motor control.
 *
 * Possible returned values are defined in lvhb_motor_status_t enumeration.
 *
 * This function may be used for the stepper motors control only.
 *
 * @param drvConfig Pointer to driver instance configuration.
 *
 * @return lvhb_motor_status_t Motor status.
 */
lvhb_motor_status_t LVHB_GetMotorStatus(lvhb_drv_config_t* const drvConfig);

/*!
 * @brief This function aligns rotor to the full-step position.
 *
 * The method executes 4 full-steps forward (one electrical revolution)
 * at minimum speed (see LVHB_FULLSTEP_SPEED_MIN constant). These steps
 * are not counted to the number of full-steps.
 * The input timer frequency must be between LVHB_FULLSTEP_IN_FREQ_MIN
 * and LVHB_FULLSTEP_IN_FREQ_MAX.
 * Note that the motor must not run when this function is called and you
 * must wait for completion of this action before you can run motor again
 * (use method LVHB_GetMotorStatus or event LVHB_OnActionComplete).
 *
 * This function may be used for the stepper motors control only.
 *
 * @param drvConfig Pointer to driver instance configuration.
 *
 * @return status_t Error code.
 */
status_t LVHB_AlignRotor(lvhb_drv_config_t* const drvConfig);

/*!
 * @brief This function changes the size of micro-step.
 *
 * Note that the motor must not run when this function is called.
 *
 * This function may be used for the stepper motors control with
 * micro-stepping support only - ie. all IN pins are used as timer outputs.
 *
 * @param drvConfig Pointer to driver instance configuration.
 * @param microStepsPerStep New size from lvhb_micro_step_t enumeration.
 *
 * @return status_t Error code.
 */
status_t LVHB_SetMicroStepSize(lvhb_drv_config_t* const drvConfig, lvhb_micro_step_t microStepsPerStep);

/*!
 * @brief This function returns the current full-step position.
 *
 * Position is set to zero by the LVHB driver initialization. It can be
 * reset by LVHB_ResetFullStepPosition function.
 *
 * This function may be used for the stepper motors control only.
 *
 * @param drvConfig Pointer to driver instance configuration.
 *
 * @return Current rotor position in number of full-steps from initial position.
 */
int32_t LVHB_GetFullStepPosition(lvhb_drv_config_t* const drvConfig);

/*!
 * @brief This function returns the current micro-step position.
 *
 * Size of micro-step depends on current "Micro-steps per Step" setting.
 * Position is set to zero by the LVHB driver initialization.
 *
 * This function may be used for the stepper motors control with
 * micro-stepping support only - ie. all IN pins are used as timer outputs.
 *
 * @param drvConfig Pointer to driver instance configuration.
 *
 * @return Current rotor position in number of micro-steps from initial position.
 */
int32_t LVHB_GetMicroStepPosition(lvhb_drv_config_t* const drvConfig);

/*!
 * @brief This function sets the counter of full-steps to zero.
 *
 * This function may be used for the stepper motors control only.
 *
 * @param drvConfig Pointer to driver instance configuration.
 *
 * @return status_t Error code.
 */
status_t LVHB_ResetFullStepPosition(lvhb_drv_config_t* const drvConfig);

/*!
 * @brief This function disables the stepper motor.
 *
 * This function disables timer interrupts and sets the INxA and INxB
 * pins output value to LOW. It can be used to stop the stepper motor.
 * Note that default behavior of the motor control is to hold position
 * when a movement is completed.
 *
 * This function may be used for the stepper motors control only.
 *
 * @param drvConfig Pointer to driver instance configuration.
 *
 * @return status_t Error code.
 */
status_t LVHB_DisableMotor(lvhb_drv_config_t* const drvConfig);

/*!
 * @brief Counter restart event handler.
 * This function must be called from counter restart interrupt handler.
 *
 * This function may be used for the stepper motors control only.
 *
 * @param drvConfig Pointer to driver instance configuration.
 */
void LVHB_OnCounterRestart(lvhb_drv_config_t* const drvConfig);

/*!
 * @brief Declaration of function that can be used by user for handling
 * the action complete event. This event occurs when "LVHB_MoveSteps",
 * "LVHB_MoveMicroSteps", "LVHB_StopContinualMovement" or
 * "LVHB_AlignRotor" action is done.
 *
 * @param drvConfig Pointer to driver instance configuration.
 */
void LVHB_OnActionComplete(lvhb_drv_config_t* const drvConfig);
/*! @} */

#endif /* __LVHB_H__ */
/*******************************************************************************
 * EOF
 ******************************************************************************/
