/*
* The Clear BSD License
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
* 
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* 
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
* 
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
\fn     void* App_GetTempDataString(void)
\brief  Return post data.

\param  [in]    none

\return         return data to be send through post
***************************************************************************************************/
void* App_GetTempDataString
(
    void
)
{
#if USE_TEMPERATURE_SENSOR
    /* Compute temperature */
    int32_t temperature = (int32_t)APP_GetCurrentTempValue()*100;
    uint8_t* pIndex = NULL;
    uint8_t sTemp[] = "Temp:"; 
    uint8_t * sendTemperatureData = MEM_BufferAlloc(TEMP_BUFF_SIZE);
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

