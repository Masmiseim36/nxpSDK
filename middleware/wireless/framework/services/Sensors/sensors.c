/*
 * Copyright 2021-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/************************************************************************************
 * Include
 ************************************************************************************/

#include "sensors.h"
#include "fwk_workq.h"

#include "fwk_platform.h"
#include "fwk_platform_sensors.h"

#include "fsl_device_registers.h"
#include "fsl_os_abstraction.h"
#include "fsl_common.h"
#include "fsl_component_timer_manager.h"
#include "fwk_platform_ics.h"

/************************************************************************************
*************************************************************************************
* Private type definitions and macros
*************************************************************************************
************************************************************************************/
#if USE_RTOS && 1 // Currently only tested with Mutex enabled in Sensors
#define gSensorsUseMutex_c TRUE
#else
#define gSensorsUseMutex_c FALSE
#endif /*USE_RTOS*/

#if gSensorsUseMutex_c
#define ADC_MUTEX_LOCK()   (void)OSA_MutexLock((osa_mutex_handle_t)mADCMutexId, osaWaitForever_c)
#define ADC_MUTEX_UNLOCK() (void)OSA_MutexUnlock((osa_mutex_handle_t)mADCMutexId)
#else
#define ADC_MUTEX_LOCK()
#define ADC_MUTEX_UNLOCK()
#endif /* gSensorsUseMutex_c */

#if gSensorsUseMutex_c
static OSA_MUTEX_HANDLE_DEFINE(mADCMutexId);
#endif /* gSensorsUseMutex_c */

#define VALUE_NOT_AVAILABLE_8  0xFFu
#define VALUE_NOT_AVAILABLE_32 0xFFFFFFFFu

#ifndef gSensorsLpConstraint_c
#define gSensorsLpConstraint_c 0 /* Set lowpower constraint to WFI - See PWR_LowpowerMode_t from PWR.h */
#endif

#ifndef gSensorsAdcCalibrationDurationInMs_c
#define gSensorsAdcCalibrationDurationInMs_c 0U
#endif

typedef enum _sensors_measurement_s
{
    MEASUREMENT_IDLE,
    TMP_MEASUREMENT_ONGOING,
    BAT_MEASUREMENT_ONGOING,
} sensors_measurement_s;

/*! *********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
*********************************************************************************** */

static uint8_t                        LastBatteryLevel   = VALUE_NOT_AVAILABLE_8;
static int32_t                        LastTemperature    = (int32_t)VALUE_NOT_AVAILABLE_32;
static volatile sensors_measurement_s measurement_status = MEASUREMENT_IDLE;

/*!
 * @brief pointer to Callback function structure used to allow / disallow lowpower during Sensors activity
 */
static const Sensors_LowpowerCriticalCBs_t *pfSensorsLowpowerCriticalCallbacks = NULL;

static volatile uint32_t CurrentTemperatureMeasIntervalMs = VALUE_NOT_AVAILABLE_32;

static TIMER_MANAGER_HANDLE_DEFINE(mTempSensorTimer);

static void Sensors_PeriodicTempWorkHandler(fwk_work_t *work);

static fwk_work_t periodic_temp_trig_work = {
    .handler = Sensors_PeriodicTempWorkHandler,
};

/*! *********************************************************************************
*************************************************************************************
* Private function
*************************************************************************************
*********************************************************************************** */

static int32_t Sensors_SetLpConstraint(int32_t power_mode)
{
    int32_t status = -1;
    if ((pfSensorsLowpowerCriticalCallbacks != NULL) &&
        (pfSensorsLowpowerCriticalCallbacks->SensorsEnterLowpowerCriticalFunc != NULL))
    {
        /* Register Low Power constraints for sensors */
        status = pfSensorsLowpowerCriticalCallbacks->SensorsEnterLowpowerCriticalFunc(power_mode);
    }
    return status;
}
static int32_t Sensors_ReleaseLpConstraint(int32_t power_mode)
{
    int32_t status = -1;

    if ((pfSensorsLowpowerCriticalCallbacks != NULL) &&
        (pfSensorsLowpowerCriticalCallbacks->SensorsExitLowpowerCriticalFunc != NULL))
    {
        /* Unregister Low Power constraints for sensors */
        status = pfSensorsLowpowerCriticalCallbacks->SensorsExitLowpowerCriticalFunc(power_mode);
    }
    return status;
}

static void Sensors_TemperatureReqCb(uint32_t temperature_meas_interval_ms)
{
    if (temperature_meas_interval_ms == 0U)
    {
        /* 0U means that the NBU requests a non periodic oneshot measurement */
        CurrentTemperatureMeasIntervalMs = VALUE_NOT_AVAILABLE_32;
    }
    else
    {
        CurrentTemperatureMeasIntervalMs = temperature_meas_interval_ms;
    }
    /* trigger one shot measurement
     * SENSORS_RefreshTemperatureValue function will be called once the measurement is ready
     * and will schedule the next measurement to be done after interval time
     */
    if (WORKQ_Submit(&periodic_temp_trig_work) < 0)
    {
        assert(0);
    }
}

static void Sensors_TempMeasTimerCallback(void *pParam)
{
    (void)pParam;
    /* Submit temperature measurement job on timer expiration */
    if (WORKQ_Submit(&periodic_temp_trig_work) < 0)
    {
        assert(0);
    }
}

static void Sensors_TemperatureReadyCb(int32_t temperature_value)
{
    (void)SENSORS_RefreshTemperatureValue();
}

static void Sensors_PeriodicTempWorkHandler(fwk_work_t *work)
{
    if (measurement_status == MEASUREMENT_IDLE)
    {
        SENSORS_TriggerTemperatureMeasurement();
    }
    else
    {
        /* For recovery, retry to trigger after 1ms time */
        (void)TM_Start((timer_handle_t)mTempSensorTimer, kTimerModeSingleShot, 1U);
    }
}

static timer_status_t Sensors_InitTimer(void)
{
    timer_status_t st;
    do
    {
        /* Open timer */
        st = TM_Open((timer_handle_t)mTempSensorTimer);
        if (kStatus_TimerSuccess != st)
        {
            break;
        }
        /* Register temperature measurement callback */
        st = TM_InstallCallback((timer_handle_t)mTempSensorTimer, Sensors_TempMeasTimerCallback, NULL);
        if (kStatus_TimerSuccess != st)
        {
            break;
        }
    } while (false);
    return st;
}

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*!
 * @brief Init Sensors driver.
 *
 */
void SENSORS_Init(void)
{
    PLATFORM_InitAdc();

    if (PLATFORM_InitTimerManager() < 0)
    {
        assert(0);
    }

#if gSensorsUseMutex_c
    /*! Initialize the ADC Mutex here. */
    if (KOSA_StatusSuccess != OSA_MutexCreate((osa_mutex_handle_t)mADCMutexId))
    {
        assert(0);
    }
#endif

    if (kStatus_TimerSuccess != Sensors_InitTimer())
    {
        assert(0);
    }

    /* The workqueue is used to post and execute the temperature
     * trigger function upon periodic timer timeout events.
     */
    if (WORKQ_InitSysWorkQ() < 0)
    {
        assert(0);
    }

    PLATFORM_RegisterNbuTemperatureRequestEventCb(&Sensors_TemperatureReqCb);
    PLATFORM_RegisterTemperatureReadyEventCb(&Sensors_TemperatureReadyCb);
}

/*!
 * @brief Deinit ADC driver.
 *
 */
void SENSORS_Deinit(void)
{
    ADC_MUTEX_LOCK();
    PLATFORM_DeinitAdc();

#if gSensorsUseMutex_c
    /*! Destroy the ADC Mutex here. */
    if (KOSA_StatusSuccess != OSA_MutexDestroy((osa_mutex_handle_t)mADCMutexId))
    {
        assert(0);
    }
#endif

    (void)TM_Close((timer_handle_t)mTempSensorTimer);

    PLATFORM_RegisterNbuTemperatureRequestEventCb(NULL);
    PLATFORM_RegisterTemperatureReadyEventCb(NULL);
    ADC_MUTEX_UNLOCK();
}

/*!
 * \brief  This function performs initialization of the callbacks structure used to
 *       disable lowpower when ADC is active
 *
 * \param[in]  pfCallback Pointer to the function structure used to allow/disable lowpower .
 *
 */
void Sensors_SetLowpowerCriticalCb(const Sensors_LowpowerCriticalCBs_t *pfCallback)
{
    /* Register Low Power callbacks */
    pfSensorsLowpowerCriticalCallbacks = pfCallback;
}

/*!
 * @brief Trig the ADC on the temperature.
 *
 *
 */
void SENSORS_TriggerTemperatureMeasurement(void)
{
    if (measurement_status == MEASUREMENT_IDLE)
    {
        /* Lock the mutex now to protect from concurrent access */
        ADC_MUTEX_LOCK();
        SENSORS_TriggerTemperatureMeasurementUnsafe();
        ADC_MUTEX_UNLOCK();
    }
}

/*!
 * @brief Trig the ADC on the temperature.
 *
 *
 */
void SENSORS_TriggerTemperatureMeasurementUnsafe(void)
{
    if (measurement_status == MEASUREMENT_IDLE)
    {
        measurement_status = TMP_MEASUREMENT_ONGOING;
        (void)Sensors_SetLpConstraint(gSensorsLpConstraint_c);
        if (false == PLATFORM_IsAdcInitialized())
        {
            /* Initialize ADC if not done yet */
            PLATFORM_InitAdc();
            OSA_TimeDelay(gSensorsAdcCalibrationDurationInMs_c);
        }
        PLATFORM_StartTemperatureMonitor();
    }
}

/*!
 * @brief Refresh temperature value in RAM.
 *
 * @retval int32_t Temperature value
 */
int32_t SENSORS_RefreshTemperatureValue(void)
{
    int32_t temperature;

    /* Lock the mutex now to protect from concurrent access */
    ADC_MUTEX_LOCK();

    if (measurement_status == TMP_MEASUREMENT_ONGOING)
    {
        PLATFORM_GetTemperatureValue(&temperature);
        (void)Sensors_ReleaseLpConstraint(gSensorsLpConstraint_c);
        LastTemperature = temperature;

        /* Temperature is ready, Stop timer and restart it if a periodic interval is requested */
        if (CurrentTemperatureMeasIntervalMs != VALUE_NOT_AVAILABLE_32)
        {
            /* Restart a oneshot timer with the requested interval.
             * Timer's timeout callback will trigger the measurement.
             * Timer is restarted each time the temperature is ready to make the measurement periodic.
             */
            (void)TM_Start((timer_handle_t)mTempSensorTimer, kTimerModeSingleShot, CurrentTemperatureMeasIntervalMs);
        }
        else
        {
            (void)TM_Stop((timer_handle_t)mTempSensorTimer);
        }
        measurement_status = MEASUREMENT_IDLE;
    }
    else
    {
        temperature = LastTemperature;
    }

    /* Unlock the mutex */
    ADC_MUTEX_UNLOCK();
    return temperature;
}

/*!
 * @brief Get temperature value from RAM.
 *
 * @retval int32_t Temperature value
 */
int32_t SENSORS_GetTemperature(void)
{
    return LastTemperature;
}

/*!
 * @brief Trig the ADC on the battery.
 *
 *
 */
void SENSORS_TriggerBatteryMeasurement(void)
{
    if (measurement_status == MEASUREMENT_IDLE)
    {
        /* Lock the mutex now to protect from concurrent access */
        ADC_MUTEX_LOCK();
        measurement_status = BAT_MEASUREMENT_ONGOING;
        (void)Sensors_SetLpConstraint(gSensorsLpConstraint_c);
        if (false == PLATFORM_IsAdcInitialized())
        {
            PLATFORM_InitAdc();
            OSA_TimeDelay(gSensorsAdcCalibrationDurationInMs_c);
        }
        PLATFORM_StartBatteryMonitor();
        ADC_MUTEX_UNLOCK();
    }
}

/*!
 * @brief Refresh battery level in RAM.
 *
 * @retval uint8_t Battery level
 */
uint8_t SENSORS_RefreshBatteryLevel(void)
{
    uint8_t BatteryLevel;

    ADC_MUTEX_LOCK();
    if (measurement_status == BAT_MEASUREMENT_ONGOING)
    {
        PLATFORM_GetBatteryLevel(&BatteryLevel);
        (void)Sensors_ReleaseLpConstraint(gSensorsLpConstraint_c);
        LastBatteryLevel   = BatteryLevel;
        measurement_status = MEASUREMENT_IDLE;
    }
    else
    {
        BatteryLevel = LastBatteryLevel;
    }
    ADC_MUTEX_UNLOCK();

    return BatteryLevel;
}

/*!
 * @brief Get battery level from RAM.
 *
 * @retval int32_t Temperature value
 */
uint8_t SENSORS_GetBatteryLevel(void)
{
    assert(LastBatteryLevel != VALUE_NOT_AVAILABLE_8);
    return LastBatteryLevel;
}
