/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=================================================================================================
\file       App_LightSensor.c
\brief      ZLO Demo LTO Sensor -Implementation
==================================================================================================*/


/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include "app_zcl_cfg.h"
#include "dbg.h"
#include <string.h>
#include "LightingBoard.h"

#include "app_common.h"
#include "App_LightTemperatureOccupancySensor.h"
#include "app_main.h"
#include "FunctionLib.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
tsZLO_LightSensorDevice sSensor;

const uint8 u8MyEndpoint = LIGHTTEMPERATUREOCCUPANCYSENSOR_SENSOR_ENDPOINT;

#ifdef DEBUG_LIGHT_TEMP_OCC_SENSOR
    #define TRACE_LIGHT_TEMP_OCC_SENSOR TRUE
#else
    #define TRACE_LIGHT_TEMP_OCC_SENSOR FALSE
#endif

/****************************************************************************/
/* define the default reports */
tsReports asDefaultReports[ZCL_NUMBER_OF_REPORTS] = \
{\
    {MEASUREMENT_AND_SENSING_CLUSTER_ID_ILLUMINANCE_MEASUREMENT,{0, E_ZCL_UINT16,E_CLD_ILLMEAS_ATTR_ID_MEASURED_VALUE,ZLO_MIN_REPORT_INTERVAL,ZLO_MAX_REPORT_INTERVAL,0,{LIGHT_SENSOR_MINIMUM_REPORTABLE_CHANGE}}},\
    {MEASUREMENT_AND_SENSING_CLUSTER_ID_OCCUPANCY_SENSING,{0, E_ZCL_BMAP8, E_CLD_OS_ATTR_ID_OCCUPANCY, ZLO_MIN_REPORT_INTERVAL, ZLO_MAX_REPORT_INTERVAL, 0, {0}}},\
    {MEASUREMENT_AND_SENSING_CLUSTER_ID_TEMPERATURE_MEASUREMENT,{0, E_ZCL_INT16,E_CLD_TEMPMEAS_ATTR_ID_MEASURED_VALUE,ZLO_MIN_REPORT_INTERVAL,ZLO_MAX_REPORT_INTERVAL,0,{TEMPERATURE_SENSOR_MINIMUM_REPORTABLE_CHANGE}}},\
};

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME: eApp_ZCL_RegisterEndpoint
 *
 * DESCRIPTION:
 * Register ZLO endpoints
 *
 * PARAMETER
 * Type                        Name                  Descirption
 * tfpZCL_ZCLCallBackFunction  fptr                  Pointer to ZCL Callback function
 *
 * RETURNS:
 * teZCL_Status
 *
 ****************************************************************************/
teZCL_Status eApp_ZCL_RegisterEndpoint(tfpZCL_ZCLCallBackFunction fptr)
{
    teZCL_Status eZCL_Status;

    // Register the Light Sensor on its own endpoint
    eZCL_Status = eZLO_RegisterLightSensorEndPoint(LIGHTTEMPERATUREOCCUPANCYSENSOR_SENSOR_ENDPOINT,
                                                  fptr,
                                                  &sSensor);

    // Check if the light sensor was successfully registered
    if (eZCL_Status != E_ZCL_SUCCESS)
    {
        DBG_vPrintf(TRACE_LIGHT_TEMP_OCC_SENSOR, "Unable to register light sensor : %x", eZCL_Status);
    }

    return eZCL_Status;
}

/****************************************************************************
 *
 * NAME: vAPP_ZCL_DeviceSpecific_Init
 *
 * DESCRIPTION:
 * ZCL Device Specific initialization
 *
 * PARAMETER: void
 *
 * RETURNS: void
 *
 ****************************************************************************/
void vAPP_ZCL_DeviceSpecific_Init(void)
{
    /* -- Initialise Illuminance Endpoint cluster attributes -- */
    // Initialise the strings in Basic
    FLib_MemCpy(sSensor.sBasicServerCluster.au8ManufacturerName, "NXP", CLD_BAS_MANUF_NAME_SIZE);
    FLib_MemCpy(sSensor.sBasicServerCluster.au8ModelIdentifier, "ZLO-LTOSensor", CLD_BAS_MODEL_ID_SIZE);
    FLib_MemCpy(sSensor.sBasicServerCluster.au8DateCode, "20160210", CLD_BAS_DATE_SIZE);
    FLib_MemCpy(sSensor.sBasicServerCluster.au8SWBuildID, "4000-0001", CLD_BAS_SW_BUILD_SIZE);
    FLib_MemCpy(sSensor.sBasicServerCluster.au8ProductURL, "www.nxp.com", CLD_BAS_URL_SIZE);
    FLib_MemCpy(sSensor.sBasicServerCluster.au8ProductCode, "1234", CLD_BAS_PCODE_SIZE);
    #ifdef CLD_BAS_ATTR_MANUFACTURER_VERSION_DETAILS
    FLib_MemCpy(sSensor.sBasicServerCluster.au8ManufacturerVersionDetails, "Zigbee_Version_3.0", CLD_BAS_MANUFACTURER_VERSION_SIZE);
    #endif
    #ifdef CLD_BAS_ATTR_SERIAL_NUMBER
    FLib_MemCpy(sSensor.sBasicServerCluster.au8SerialNumber, "1234", CLD_BAS_SERIAL_NUMBER_SIZE);
    #endif
    #ifdef CLD_BAS_ATTR_PRODUCT_LABEL
    FLib_MemCpy(sSensor.sBasicServerCluster.au8ProductLabel, "Kinetis_KW41Z", CLD_BAS_PRODUCT_LABEL_SIZE);
    #endif
    sSensor.sBasicServerCluster.eGenericDeviceType = E_CLD_BAS_GENERIC_DEVICE_TYPE_MOTION_OR_LIGHT_SENSOR;


    /* Initialise the attribute in illuminance Measurement */
    sSensor.sIlluminanceMeasurementServerCluster.u16MeasuredValue = 0;
    sSensor.sIlluminanceMeasurementServerCluster.eLightSensorType = E_CLD_ILLMEAS_LST_CMOS;
    sSensor.sIlluminanceMeasurementServerCluster.u16MinMeasuredValue = LIGHT_SENSOR_MINIMUM_MEASURED_VALUE;
    sSensor.sIlluminanceMeasurementServerCluster.u16MaxMeasuredValue = LIGHT_SENSOR_MAXIMUM_MEASURED_VALUE;

    /* Initialise the attribute in Occupancy Sensing */
    // Initialise the strings in Basic
    sSensor.sOccupancySensingServerCluster.eOccupancySensorType = E_CLD_OS_SENSORT_TYPE_PIR;
    sSensor.sOccupancySensingServerCluster.u8Occupancy = 0;

    #ifdef PIR_TYPE_PWM
        sSensor.sOccupancySensingServerCluster.u8PIRUnoccupiedToOccupiedThreshold = APP_OCCUPANCY_SENSOR_TRIGGER_THRESHOLD;
        sSensor.sOccupancySensingServerCluster.u16PIRUnoccupiedToOccupiedDelay = APP_OCCUPANCY_SENSOR_UNOCCUPIED_TO_OCCUPIED_DELAY;
    #endif
    sSensor.sOccupancySensingServerCluster.u16PIROccupiedToUnoccupiedDelay = APP_OCCUPANCY_SENSOR_OCCUPIED_TO_UNOCCUPIED_DELAY;

    /* Initialise the attribute in Temperature Measurement */
    sSensor.sTemperatureMeasurementServerCluster.i16MeasuredValue = 0;
    sSensor.sTemperatureMeasurementServerCluster.i16MinMeasuredValue = TEMPERATURE_SENSOR_MINIMUM_MEASURED_VALUE;
    sSensor.sTemperatureMeasurementServerCluster.i16MaxMeasuredValue = TEMPERATURE_SENSOR_MAXIMUM_MEASURED_VALUE;
}

/****************************************************************************
 *
 * NAME: vAPP_ZCL_DeviceSpecific_SetIdentifyTime
 *
 * DESCRIPTION:
 * ZCL Device Specific setting of identify time
 *
 * PARAMETER:
 * uint16 u16Time Identify time duration
 *
 * RETURNS: void
 *
 ****************************************************************************/
PUBLIC void vAPP_ZCL_DeviceSpecific_SetIdentifyTime(uint16 u16Time)
{
    sSensor.sIdentifyServerCluster.u16IdentifyTime=u16Time;
}
/****************************************************************************
 *
 * NAME: vAPP_ZCL_DeviceSpecific_UpdateIdentify
 *
 * DESCRIPTION:
 * ZCL Device Specific identify updates
 *
 * PARAMETER: void
 *
 * RETURNS: void
 *
 ****************************************************************************/
PUBLIC void vAPP_ZCL_DeviceSpecific_UpdateIdentify(void)
{
    if(sSensor.sIdentifyServerCluster.u16IdentifyTime%2)
    {
        bRGB_LED_SetGroupLevel(LED_LEVEL);
        bRGB_LED_On();
    }
    else
    {
        bRGB_LED_Off();
    }
}
/****************************************************************************
 *
 * NAME: vAPP_ZCL_DeviceSpecific_IdentifyOff
 *
 * DESCRIPTION:
 * ZCL Device Specific stop identify
 *
 * PARAMETER: void
 *
 * RETURNS: void
 *
 ****************************************************************************/
PUBLIC void vAPP_ZCL_DeviceSpecific_IdentifyOff(void)
{
    vAPP_ZCL_DeviceSpecific_SetIdentifyTime(0);
    bRGB_LED_Off();
}

/****************************************************************************
 *
 * NAME: APP_cbTimerLightTempSensorSample
 *
 * DESCRIPTION:
 * Callback for timer of Light and Temp sensor sampling
 *
 * PARAMETER: void
 *
 * RETURNS: void
 *
 ****************************************************************************/
PUBLIC void APP_cbTimerLightTempSensorSample(void *pvParam)
{
    if(APP_bPersistantPolling == TRUE)
    {
        vAPP_LightSensorSample();
        vAPP_TemperatureSensorSample();
        /* Start sample timer so that you keep on sampling if KEEPALIVE_TIME is too high*/
        ZTIMER_eStart(u8TimerLightTempSensorSample, ZTIMER_TIME_MSEC(1000 * LIGHT_SENSOR_SAMPLING_TIME_IN_SECONDS));
    }
}


/****************************************************************************
 *
 * NAME: vAPP_LightSensorSample
 *
 * DESCRIPTION:
 * Light sensor sampling
 *
 * PARAMETER: void
 *
 * RETURNS: void
 *
 ****************************************************************************/
PUBLIC void vAPP_LightSensorSample(void)
{
    static uint8_t idx = 0;
    uint16_t resultArray[] = {0x11, 0x12, 0x12, 0x13, 0x13, 0x13, 0x13};
    uint16 u16ALSResult;

    /* Simulated value */
    idx = (idx + 1) % (sizeof(resultArray)/sizeof(resultArray[0]));
    u16ALSResult = resultArray[idx];

    if(u16ALSResult > (LIGHT_SENSOR_MINIMUM_MEASURED_VALUE - 1))
    {
        sSensor.sIlluminanceMeasurementServerCluster.u16MeasuredValue = u16ALSResult;
    }
    else
    {
        sSensor.sIlluminanceMeasurementServerCluster.u16MeasuredValue = LIGHT_SENSOR_MINIMUM_MEASURED_VALUE;
    }
}

/****************************************************************************
 *
 * NAME: vAPP_TemperatureSensorSample
 *
 * DESCRIPTION:
 * Temperature sensor sampling
 *
 * PARAMETER: void
 *
 * RETURNS: void
 *
 ****************************************************************************/
PUBLIC void vAPP_TemperatureSensorSample(void)
{
    static uint8_t idx = 0;
    uint16_t resultArray[] = {0x01, 0x02, 0x02, 0x03, 0x03, 0x03, 0x03};
    int16 i16TempResult;

    /* Simulated value */
    idx = (idx + 1) % (sizeof(resultArray)/sizeof(resultArray[0]));
    i16TempResult = resultArray[idx];

    if(i16TempResult >= (TEMPERATURE_SENSOR_MINIMUM_MEASURED_VALUE))
    {
        sSensor.sTemperatureMeasurementServerCluster.i16MeasuredValue = i16TempResult;
    }
    else
    {
        sSensor.sTemperatureMeasurementServerCluster.i16MeasuredValue = TEMPERATURE_SENSOR_MINIMUM_MEASURED_VALUE;
    }
}

/****************************************************************************
 *
 * NAME: app_u8GetDeviceEndpoint
 *
 * DESCRIPTION:
 * Returns the application endpoint
 *
 * PARAMETER: void
 *
 * RETURNS: void
 *
 ****************************************************************************/
PUBLIC uint8 app_u8GetDeviceEndpoint( void)
{
    return LIGHTTEMPERATUREOCCUPANCYSENSOR_SENSOR_ENDPOINT;
}

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
