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

#ifndef _APP_TEMP_SENSOR_H
#define _APP_TEMP_SENSOR_H

/*!=================================================================================================
\file       app_temp_sensor.h
\brief      This is a header file for the temperature sensor utility for socket app client demo.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/

#include "EmbeddedTypes.h"


/*==================================================================================================
Public macros
==================================================================================================*/

#ifndef USE_TEMPERATURE_SENSOR
  #define USE_TEMPERATURE_SENSOR 0
#endif

#define ADC_0                           (0U)            /* ADC instance */

/*==================================================================================================
Public type definitions
==================================================================================================*/

/* None */

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/* None */

/*==================================================================================================
Public function prototypes
==================================================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================*/  

  
/* Temp Sensor functions:*/
/*!*************************************************************************************************
\fn     void APP_InitADC(uint32_t instance)
\brief  ADC module initialization

\param  [in]    adc instance

\return         none
***************************************************************************************************/  
void APP_InitADC(uint32_t instance);
/*!*************************************************************************************************
\fn     int32_t APP_GetCurrentTempValue(void)
\brief  Calculate the current temperature

\param  [in]    none

\return         int32 temperature value
***************************************************************************************************/
int32_t APP_GetCurrentTempValue(void);

/*!*************************************************************************************************
\fn     void* App_GetTempDataString(void)
\brief  Return post data.

\param  [in]    none

\return         return data to be send through post
***************************************************************************************************/
void* App_GetTempDataString(void);

#ifdef __cplusplus
}
#endif
/*================================================================================================*/


#endif /* _TEMP_SENSOR_H */
