/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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

/** 
* @file pedometer.h
* @brief The pedometer.h file contains the interface and structure definitions for
* pedometer application.
*/

#ifndef PEDOMETER_H_
#define PEDOMETER_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "KeynetikPedometer.h"
/******************************************************************************
 * Public Datatypes
 *****************************************************************************/

typedef KeynetikActivityLevel activitylevel_t; /* alias to a shorter name for readability */
typedef uint16_t debounce_count_t;
#define PEDO_ONEG_8G 4096      /* One G value for 8G mode.*/
#define PEDO_ONEG_4G 8192      /* One G value for 4G mode.*/
#define PEDO_ONEG_2G 16384     /* One G value for 2G mode.*/
#define PEDO_FREQHZ_DEFAULT 50 /* Default frequency*/
#define PEDO_STEP_THRESHOLD_DEFAULT 130
#define PEDO_SPEED_PERIOD_DEFAULT 5
#define PEDO_FILTER_TIME_DEFAULT 3
#define PEDO_FILTER_STEPS_DEFAULT 4
/*!
 * @brief This defines the acceleration input data for the pedometer.
 */
typedef struct
{
    int16_t accel[3];
} ped_accel_t;
/*!
 * @brief This defines the configuration structure of the pedometer.
 */
typedef struct
{
    uint16_t sleepminimum;                 /* Sleep vector magnitude minimum */
    uint16_t sleepmaximum;                 /* Sleep vector magnitude maximum */
    debounce_count_t sleepcount_threshold; /* Sleep debounce counter threshold */
    struct
    {
        uint8_t : 5;              /* Reserved */
        bool sleep_dbcntm : 1;    /* Sleep debounce counter mode. Set to 1 to clear the debounce counter when the test
                                     condition fails, otherwise decrement. */
        bool activity_dbcntm : 1; /* Activity debounce counter mode. Set to 1 to clear the debounce counter when the
                                     test condition fails, otherwise decrement. */
        bool config : 1;          /* Set to 1 to initialize the pedometer and configure it with the other configuration
                                     variables. */
    } bits;
    KeynetikConfig keynetik;                  /* Keynetik parameters (step length, height, weight, etc) */
    debounce_count_t activitycount_threshold; /* Activity debounce counter threshold */
    uint8_t stepcoalesce;                     /* Number of steps to coalesce for step interrupt */
    uint16_t oneG; /* One G value for the range at accelerometer is running. refer the data sheet of the accelerometer
                    and compute this value.
                    eg. 2G Mode, 1G is 4096.*/
    uint16_t frequency; /* The frequency at which accelerometer runs in HZ. best set frequencies are 25HZ or 50HZ. It
                           can run at higher too.*/
} pedometer_config_t;
/*!
 * @brief This defines the pedometer instance.
 */
typedef struct
{
    struct pedometer_status_tag
    {
        union
        {
            struct
            {
                activitylevel_t activity : 3; /* Activity level. */
                bool suspend : 1;             /* Pedometer autonomously suspended. */
                bool activitychange : 1;      /* Change in activity level. */
                bool stepchange : 1;          /* Change in step count. */
                bool suspendchange : 1;       /* Change in suspend state .*/
                bool mergedflags : 1;         /* Merged status flags. */
            } bits;
            uint8_t byte;
        } status;
        uint8_t version;    /* Pedometer version number. */
        uint16_t stepcount; /* Step count. */
        uint16_t distance;  /* Total distance in meters or activity state. */
        uint16_t speed;     /* Average speed in meters per hour. The value is updated on the completion of each step. */
        uint16_t calories;  /* Calorie count expended. */
        debounce_count_t sleepcount; /* Sleep debounce counter. */
    } status;
    pedometer_config_t config; /* Pedometer configuration.*/
    struct pedometer_private_tag
    {
        debounce_count_t activitycount; /* Activity debounce counter. */
        uint16_t stepchg_stepcount;     /* Step count at last stepchange interrupt (for step coalesing) */
    } private;
} pedometer_t;

/******************************************************************************
 * Public Function Declarations
 ******************************************************************************/
/*******************************************************************************
 * API
 ******************************************************************************/
/*! @brief       The interface function initialize the pedometer.
 *  @details     This function initialize the pedometer structure and return the handle.

 *  @param[in]   pPedometer      handle to the pedometer.
  *
 *  @return      void.
 *
 *  @Constraints None
 *
 *  @Reentrant   Yes
 */
void pedometer_init(pedometer_t *pPedometer);

/*! @brief       The interface function to configure the pedometer.
 *
 *  @param[in]   pedometer_t    handle to the pedometer.
 *  @param[in]   config         configuration value.
 *
 *  @return      void.
 *
 *  @Constraints None
 *
 *  @Reentrant   Yes
 */
void pedometer_configure(pedometer_t *pPedometer, const pedometer_config_t *pConfig);
/*! @brief       The interface function excutes the pedometer algorithm.
 *  @details     Call this function the rate at which accelerometer runs.

 *  @param[in]   pPedometer         handle to the pedometer.
 *  @param[in]   accel_data         acceleration data.
 *  @return      ::pedometer_run() returns the status .
 *
 *  @Constraints None
 *
 *  @Reentrant   Yes
 */
int32_t pedometer_run(pedometer_t *pPedometer, ped_accel_t *pData);

#endif // PEDOMETER_H_
