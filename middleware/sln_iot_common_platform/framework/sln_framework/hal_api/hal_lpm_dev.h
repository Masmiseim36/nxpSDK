/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief hal low power mode device declaration. The low power mode device implements all functionality required to send
 * the active platform into a low power mode. This includes sleep timers and the ability to immediately go into the
 * specified low power/sleep mode. Generally only one low power mode device should be required, and this device's low
 * power capabilities will be specific to the platform the app is running on.
 */

#ifndef _HAL_LPM_DEV_H_
#define _HAL_LPM_DEV_H_

#include "fwk_common.h"

typedef struct _lpm_dev lpm_dev_t;

/*! @brief Error code for low power mode hal device */
typedef enum _hal_lpm_status
{
    kStatus_HAL_LpmSuccess        = 0,
    kStatus_HAL_LpmError          = MAKE_FRAMEWORK_STATUS(kStatusFrameworkGroups_Lpm, 1),
    kStatus_HAL_LpmRegisterFail   = MAKE_FRAMEWORK_STATUS(kStatusFrameworkGroups_Lpm, 2),
    kStatus_HAL_LpmUnregisterFail = MAKE_FRAMEWORK_STATUS(kStatusFrameworkGroups_Lpm, 3),
    kStatus_HAL_LpmLockNull       = MAKE_FRAMEWORK_STATUS(kStatusFrameworkGroups_Lpm, 4),
    kStatus_HAL_LpmLockError      = MAKE_FRAMEWORK_STATUS(kStatusFrameworkGroups_Lpm, 5),
    kStatus_HAL_LpmRuntimeGetFail = MAKE_FRAMEWORK_STATUS(kStatusFrameworkGroups_Lpm, 6),
    kStatus_HAL_LpmRuntimePutFail = MAKE_FRAMEWORK_STATUS(kStatusFrameworkGroups_Lpm, 7),
    kStatus_HAL_LpmRuntimeSetFail = MAKE_FRAMEWORK_STATUS(kStatusFrameworkGroups_Lpm, 8),
    kStatus_HAL_LpmTimerNull      = MAKE_FRAMEWORK_STATUS(kStatusFrameworkGroups_Lpm, 9),
    kStatus_HAL_LpmTimerFail      = MAKE_FRAMEWORK_STATUS(kStatusFrameworkGroups_Lpm, 10),
} hal_lpm_status_t;

/*! @brief Low power modes supported */
typedef enum _hal_lpm_mode
{
    kLPMMode_SNVS,
    kLPMMode_Invalid,
} hal_lpm_mode_t;

/*! @brief Status of the low power mode */
typedef enum _hal_lpm_manager_status
{
    kLPMManagerStatus_SleepDisable,
    kLPMManagerStatus_SleepEnable,
} hal_lpm_manager_status_t;

/*! @brief Callback function to timeout check requester list busy status. */
typedef int (*lpm_manager_timer_callback_t)(lpm_dev_t *dev);

/*! @brief Operation that needs to be implementat by a lpm device */
typedef struct _lpm_dev_operator
{
    hal_lpm_status_t (*init)(lpm_dev_t *dev,
                             lpm_manager_timer_callback_t callback,
                             lpm_manager_timer_callback_t preEnterSleepTimer);
    hal_lpm_status_t (*deinit)(const lpm_dev_t *dev);
    hal_lpm_status_t (*openTimer)(const lpm_dev_t *dev);
    hal_lpm_status_t (*stopTimer)(const lpm_dev_t *dev);
    hal_lpm_status_t (*openPreEnterTimer)(const lpm_dev_t *dev);
    hal_lpm_status_t (*stopPreEnterTimer)(const lpm_dev_t *dev);
    hal_lpm_status_t (*enterSleep)(const lpm_dev_t *dev, hal_lpm_mode_t mode);
    hal_lpm_status_t (*lock)(const lpm_dev_t *dev);
    hal_lpm_status_t (*unlock)(const lpm_dev_t *dev);
} lpm_dev_operator_t;

/*! @brief Attributes of a lpm device */
struct _lpm_dev
{
    /* unique id */
    int id;
    /* operations */
    const lpm_dev_operator_t *ops;
    /* timer */
    TimerHandle_t timer;
    /* pre-enter sleep timer */
    TimerHandle_t preEnterSleepTimer;
    /* lock */
    SemaphoreHandle_t lock;
    /* callback */
    lpm_manager_timer_callback_t callback;
    /* preEnterSleepCallback */
    lpm_manager_timer_callback_t preEnterSleepCallback;
};

typedef struct _hal_lpm_request
{
    void *dev;                              /* request dev handle */
    char name[LPM_REQUEST_NAME_MAX_LENGTH]; /* request name */
} hal_lpm_request_t;

#endif /* _HAL_LPM_DEV_H_ */
