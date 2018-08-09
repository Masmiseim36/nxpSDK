/*! *********************************************************************************
 * \addtogroup Temperature Custom Profile
 * @{
 ********************************************************************************** */
/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* \file
*
* This file is the interface file for the Temperature Service
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

#ifndef _TEMPERATURE_INTERFACE_H_
#define _TEMPERATURE_INTERFACE_H_

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public constants & macros
*************************************************************************************
************************************************************************************/

/*! Temperature Service - Invalid Value */
#define gTms_InvalidTemperatureValue_c     0x1000

/*! Temperature Service - Maximum Value ( -273.15 C)*/
#define gTms_MinimumTemperatureValue_c     0x954D

/*! Temperature Service - Minimum Value ( -273.15 C)*/
#define gTms_MaximumTemperatureValue_c     0x8FFF

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/*! Temperature Service - Configuration */
typedef struct tmsConfig_tag
{
    uint16_t    serviceHandle;
    int16_t     initialTemperature;        
} tmsConfig_t;

/*! Temperature Client - Configuration */
typedef struct tmcConfig_tag
{
    uint16_t    hService;
    uint16_t    hTemperature;
    uint16_t    hTempCccd; 
    uint16_t    hTempDesc; 
    gattDbCharPresFormat_t  tempFormat;
} tmcConfig_t;

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*!**********************************************************************************
* \brief        Starts Temperature Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Tms_Start(tmsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Stops Temperature Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Tms_Stop(tmsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Subscribes a GATT client to the Temperature service
*
* \param[in]    pClient  Client Id in Device DB.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Tms_Subscribe(deviceId_t clientDeviceId);

/*!**********************************************************************************
* \brief        Unsubscribes a GATT client from the Temperature service
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Tms_Unsubscribe();

/*!**********************************************************************************
* \brief        Records Temperature measurement on a specified service handle.
*
* \param[in]    serviceHandle   Service handle.
* \param[in]    value           Temperature Value.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Tms_RecordTemperatureMeasurement (uint16_t serviceHandle, int16_t temperature);

#ifdef __cplusplus
}
#endif

#endif /* _TEMPERATURE_INTERFACE_H_ */

/*! **********************************************************************************
 * @}
 ************************************************************************************/
