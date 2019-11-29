/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=================================================================================================
\file       app_temp_sensor.c
\brief      This is a public source file for the application temperature sensor.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/

/* General Includes */
#include "EmbeddedTypes.h"
#include "network_utils.h"
#include "stdlib.h"

/* Drivers */
#include "board.h"

#include "app_temp_sensor.h"
#include "FunctionLib.h"
#include "MemManager.h"

/*==================================================================================================
Private macros
==================================================================================================*/
#define TEMP_BUFF_SIZE     (20U)
/*==================================================================================================
Private global variables declarations
==================================================================================================*/

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/* None */

/*==================================================================================================
Private prototypes
==================================================================================================*/
void calibrateParams(void);

/*==================================================================================================
Public functions
==================================================================================================*/
/*!*************************************************************************************************
\fn       void APP_InitADC(uint32_t instance)
\brief    ADC module initialization
\return   void
***************************************************************************************************/
#if USE_TEMPERATURE_SENSOR
void APP_InitADC(uint32_t instance)
{
    BOARD_InitAdc();
}

/*!*************************************************************************************************
\fn       int32_t APP_GetCurrentTempValue(void))
\brief    Gets current temperature of chip.
\return   int32_t temperature
***************************************************************************************************/
int32_t APP_GetCurrentTempValue(void)
{  
    return BOARD_GetTemperature();
}
#endif
/*!*************************************************************************************************
\fn     void *App_GetTempDataString(void)
\brief  Return post data.

\param  [in]    none

\return         return data to be send through post
***************************************************************************************************/
void *App_GetTempDataString
(
    void
)
{
#if USE_TEMPERATURE_SENSOR
    /* Compute temperature */
    int32_t temperature = (int32_t)APP_GetCurrentTempValue()*100;
    uint8_t *pIndex = NULL;
    uint8_t sTemp[] = "Temp:"; 
    uint8_t *sendTemperatureData = MEM_BufferAlloc(TEMP_BUFF_SIZE);

    if(NULL == sendTemperatureData)
    {
      return sendTemperatureData;
    }

    /* Clear data and reset buffers */
    FLib_MemSet(sendTemperatureData, 0, TEMP_BUFF_SIZE);

    /* Compute output */
    pIndex = sendTemperatureData;
    FLib_MemCpy(pIndex, sTemp, SizeOfString(sTemp));
    pIndex += SizeOfString(sTemp);
    NWKU_PrintDec((uint8_t)(temperature/100), pIndex, 2, TRUE);
    pIndex += 2; /* keep only the first 2 digits */
    *pIndex = '.';
    pIndex++;
    NWKU_PrintDec((uint8_t)(abs(temperature)%100), pIndex, 2, TRUE);
    return sendTemperatureData;
#else
    return NULL;
#endif    
}

/*==================================================================================================
Private functions
==================================================================================================*/

/*===============================================================================================
Private debug functions
==================================================================================================*/

