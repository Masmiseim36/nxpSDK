/*! *********************************************************************************
 * \addtogroup GATT_DB
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

#ifndef _GATT_DATABASE_DYNAMIC_H_
#define _GATT_DATABASE_DYNAMIC_H_

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/

#include "EmbeddedTypes.h"

/*************************************************************************************
**************************************************************************************
* Public macros
**************************************************************************************
*************************************************************************************/

/*************************************************************************************
**************************************************************************************
* Public types
**************************************************************************************
*************************************************************************************/

/* Service, Characteristics and Descriptors used to add a service */

typedef struct descriptorInfo_tag
{
    uint16_t                                uuid16;
    uint16_t                                handle;
    uint16_t                                valueLength;
    uint8_t*                                pValue;
    gattAttributePermissionsBitFields_t     accessPermissions;
}descriptorInfo_t;

typedef struct characteristicInfo_tag
{
    uint16_t                                uuid16;
    uint16_t                                handle;
    gattCharacteristicPropertiesBitFields_t properties;
    uint16_t                                maxValueLength;
    uint16_t                                valueLength;
    uint8_t*                                pValue;
    gattAttributePermissionsBitFields_t     accessPermissions;
    bool_t                                  bAddCccd;
    uint16_t                                cccdHandle;
    uint8_t                                 nbOfDescriptors;
    descriptorInfo_t*                       pDescriptorInfo;
}characteristicInfo_t;

typedef struct serviceInfo_tag
{
    uint16_t                                uuid16;
    uint16_t                                handle;
    uint8_t                                 nbOfCharacteristics;
    characteristicInfo_t*                   pCharacteristicInfo;
}serviceInfo_t;

/* Output handles returned by the service add functions obtained from Ble Host */

/* GATT service */
typedef struct gattServiceHandles_tag
{
    uint16_t serviceHandle;
    uint16_t charServiceChangedHandle;
    uint16_t charServiceChangedCccdHandle;
}gattServiceHandles_t;

/* GAP service */
typedef struct gapServiceHandles_tag
{
    uint16_t serviceHandle;
    uint16_t charDeviceNameHandle;
    uint16_t charAppearanceHandle;
    uint16_t charPpcpHandle;
}gapServiceHandles_t;

/* IPSS service */
typedef struct ipssServiceHandles_tag
{
    uint16_t serviceHandle;
}ipssServiceHandles_t;

/* Heart rate service */
typedef struct heartRateServiceHandles_tag
{
    uint16_t serviceHandle;
    uint16_t charHrMeasurementHandle;
    uint16_t charHrMeasurementCccdHandle;
    uint16_t charBodySensorLocationHandle;
    uint16_t charHrCtrlPointHandle;
}heartRateServiceHandles_t;

/* Battery service */
typedef struct batteryServiceHandles_tag
{
    uint16_t serviceHandle;
    uint16_t charBatteryLevelHandle;
    uint16_t charBatteryLevelCccdHandle;
}batteryServiceHandles_t;

/* Device info service */
typedef struct deviceInfoServiceHandles_tag
{
    uint16_t serviceHandle;
    uint16_t charManufNameHandle;
    uint16_t charModelNoHandle;
    uint16_t charSerialNoHandle;
    uint16_t charHwRevHandle;
    uint16_t charFwRevHandle;
    uint16_t charSwRevHandle;  
    uint16_t charSystemIdHandle;
    uint16_t charRcdlHandle;     
}deviceInfoServiceHandles_t;

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

bleResult_t GattDbDynamic_AddServiceInDatabase(serviceInfo_t* pServiceInfo);
  
bleResult_t GattDbDynamic_AddGattService(gattServiceHandles_t* pOutServiceHandles);
bleResult_t GattDbDynamic_AddGapService(gapServiceHandles_t* pOutServiceHandles);
bleResult_t GattDbDynamic_AddIpssService(ipssServiceHandles_t* pOutServiceHandles);
bleResult_t GattDbDynamic_AddHeartRateService(heartRateServiceHandles_t* pOutServiceHandles);
bleResult_t GattDbDynamic_AddBatteryService(batteryServiceHandles_t* pOutServiceHandles);
bleResult_t GattDbDynamic_AddDeviceInformationService(deviceInfoServiceHandles_t* pOutServiceHandles);

serviceInfo_t* GattDbDynamic_GetIpssService();
serviceInfo_t* GattDbDynamic_GetHeartRateService();
serviceInfo_t* GattDbDynamic_GetBatteryService();
serviceInfo_t* GattDbDynamic_GetDeviceInformationService();

#ifdef __cplusplus
}
#endif 

#endif /* _GATT_DATABASE_DYNAMIC_H_ */

/*! *********************************************************************************
* @}
********************************************************************************** */
