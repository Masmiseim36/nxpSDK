/*
 * Copyright 2021-2022, 2024-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SENSORS_H_
#define _SENSORS_H_

#include "EmbeddedTypes.h"

/*!
 * @addtogroup Sensors
 *
 * \details The sensor module provides API to communicate with the ADC in order to obtain the
 *      temperature value and the battery level.
 *
 * Calling a "SENSORS_Get" function will give the last measured value that was copied
 *      to RAM. If you want to refresh this value you must first call a "SENSORS_Trigger"
 *      function that configures the ADC to perform temperature or battery measurement,
 *      then a call to "SENSORS_Refresh" function will read the returned value from the
 *      ADC and save it in RAM. You can run code between these two functions because the
 *      ADC won't give you the value until it's fully ready.
 *
 * The temperature is given in tenths of degrees Celsius and the battery in percentage.
 *
 * Here is an exemple of how to use the temperature API :
 * <pre>
 *      int32_t temperature1, temperature2;
 *      SENSORS_TriggerTemperatureMeasurement();
 *      ...
 *      temperature1 = (SENSORS_RefreshTemperatureValue())/10;
 *      temperature2 = (SENSORS_GetTemperature())/10;
 * </pre>
 * In our case temperature1 is equal to temperature2, both are in degree celsius
 * @{
 */

/*! *********************************************************************************
 *************************************************************************************
 * Public type definitions
 *************************************************************************************
 ********************************************************************************** */
/*!
 * \brief  Type definition for Function prototype for Lowpower callback to be registered to Sensors module
 */
typedef int32_t (*sensorsLowpowerCriticalFunc_t)(int32_t power_mode);

/*!
 * \brief  Type definition for structure for lowpower functions callback structure to be registered to Sensors module
 *     to disallow, allow lowpower during SecLib activity
 */
typedef struct
{
    sensorsLowpowerCriticalFunc_t
        SensorsEnterLowpowerCriticalFunc; /*!< function callback pointer to disable lowpower  */
    sensorsLowpowerCriticalFunc_t
        SensorsExitLowpowerCriticalFunc;  /*!< function callback pointer to reenable lowpower */
} Sensors_LowpowerCriticalCBs_t;

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

/*!
 * \brief Init sensor module.
 *
 */
void SENSORS_Init(void);

/*!
 * \brief Deinit sensor Module.
 * \note  The calling task should referesh the triggered measurement before deinitializing
 *        the sensor module.
 */
void SENSORS_Deinit(void);

/*!
 * \brief  This function performs initialization of the callbacks structure used to
 *       disable lowpower when ADC is active
 *
 * \param[in]  pfCallback Pointer to the function structure used to allow/disable lowpower .
 *
 */
void Sensors_SetLowpowerCriticalCb(const Sensors_LowpowerCriticalCBs_t *pfCallback);

/*!
 * \brief Trigger the ADC on the temperature.
 * \note  Triggering twice or triggering another type of measurement during an ongoing
 *        one is unsupported and the new request will be ignored.
 */
void SENSORS_TriggerTemperatureMeasurement(void);

/*!
 * \brief Trigger the ADC on the temperature. This an unsafe API, and MUST only be called
          when the scheduler is stopped or when interrupts are masked.
 * WARNING: Using this outside atomic sections is undefined behavior.
 * \note  Usually used when exiting low power to trigger the measurement as early
 *        as possible when interrupts are still masked, to optimize the active time
 */
void SENSORS_TriggerTemperatureMeasurementUnsafe(void);

/*!
 * \brief Refresh temperature value in RAM.
 *
 * \retval int32_t Temperature value in tenths of degree celsius.
 */
int32_t SENSORS_RefreshTemperatureValue(void);

/*!
 * \brief Get temperature value from RAM
 *
 * \retval int32_t Temperature value in tenth of degree celsius. VALUE_NOT_AVAILABLE_32 in case no measurement was
 *         done previously.
 */
int32_t SENSORS_GetTemperature(void);

/*!
 * \brief Trigger the ADC on the battery.
 * \note  Triggering twice or triggering another type of measurement during an ongoing
 *        one is unsupported and the new request will be ignored.
 */
void SENSORS_TriggerBatteryMeasurement(void);

/*!
 * \brief Refresh battery level in RAM.
 *
 * \details 3V or more = 100%
 *        1.8V or less = 0%
 *
 * \retval uint8_t Battery level
 */
uint8_t SENSORS_RefreshBatteryLevel(void);

/*!
 * \brief Get battery level from RAM.
 *
 * \details 3V or more = 100%
 *        1.8V or less = 0%
 *
 * \retval uint8_t Battery level
 */
uint8_t SENSORS_GetBatteryLevel(void);

/*! @}*/
#endif /* _SENSORS_H_ */
