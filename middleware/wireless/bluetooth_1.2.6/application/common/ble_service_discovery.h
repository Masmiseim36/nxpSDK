/*! *********************************************************************************
 * \addtogroup BLE
 * @{
 ********************************************************************************** */
/*!
* The Clear BSD License
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
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

#ifndef _BLE_SERVICE_DISCOVERY_H_
#define _BLE_SERVICE_DISCOVERY_H_

/************************************************************************************
*************************************************************************************
* Includes
*************************************************************************************
************************************************************************************/
#include "ble_general.h"

/************************************************************************************
*************************************************************************************
* Public Macros
*************************************************************************************
************************************************************************************/
/*! Maximum Number of Services that can be stored during Service Discovery */
#ifndef gMaxServicesCount_d
#define gMaxServicesCount_d		6
#endif

/*! Maximum Number of Characteristics per Service that can be stored during Service Discovery */
#ifndef gMaxServiceCharCount_d
#define gMaxServiceCharCount_d         	10
#endif

/*! Maximum Number of Descriptors per Service that can be stored during Service Discovery */
#ifndef gMaxCharDescriptorsCount_d
#define gMaxCharDescriptorsCount_d      4
#endif

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/
typedef enum servDiscEventType_tag{
	gServiceDiscovered_c,				/*!< A serviced has been discovered. */
	gDiscoveryFinished_c				/*!< Service DIscovery is finished. */
}servDiscEventType_t;

/*! Service Discovery Event */
typedef struct servDiscEvent_tag {
    servDiscEventType_t  eventType;       /*!< Event type. */
    union {
    	gattService_t* 	pService;
        bool_t          success;
    } eventData;                            /*!< Event data, selected according to event type. */
} servDiscEvent_t;

typedef void (*servDiscCallback_t)
(
    deviceId_t  		deviceId,			/*!< Device ID identifying the active connection. */
	servDiscEvent_t* 	pEvent				/*!< Service Discovery Event. */
);

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief	 Installs an application callback for the Service Discovery module.
*
* \param[in] pServDiscCallback  	   Pointer to service discovery callback.
*
********************************************************************************** */
void BleServDisc_RegisterCallback(servDiscCallback_t pServDiscCallback);

/*! *********************************************************************************
* \brief	Starts the Service Discovery procedure with the peer device.
*
* \param[in] peerDeviceId  	   The GAP peer Id.
*
* \return  gBleSuccess_c or error.
*
********************************************************************************** */
bleResult_t BleServDisc_Start(deviceId_t peerDeviceId);

/*! *********************************************************************************
* \brief	Stops the Service Discovery procedure with the peer device.
*
* \param[in] peerDeviceId  	   The GAP peer Id.
*
* \return  gBleSuccess_c or error.
*
********************************************************************************** */
void BleServDisc_Stop(deviceId_t peerDeviceId);

/*! *********************************************************************************
* \brief	Starts the Service Discovery procedure for a specified service UUID,
* 			with the peer device.
*
* \param[in] peerDeviceId		The GAP peer Id.
* \param[in] uuidType           Service UUID type.
* \param[in] pUuid              Service UUID.
*
* \return  gBleSuccess_c or error.
*
********************************************************************************** */
bleResult_t BleServDisc_FindService(deviceId_t peerDeviceId, bleUuidType_t uuidType, bleUuid_t *pUuid);


/*! *********************************************************************************
* \brief        Signals the module a GATT client callback from host stack.
* 				Must be called by the application, which is responsible for
*
* \param[in]    peerDeviceId        GATT Server device ID.
* \param[in]    procedureType    	Procedure type.
* \param[in]    procedureResult    	Procedure result.
* \param[in]    error    			Callback result.
*
* \return  gBleSuccess_c or error.
*
********************************************************************************** */
void BleServDisc_SignalGattClientEvent(
    deviceId_t              peerDeviceId,
    gattProcedureType_t     procedureType,
    gattProcedureResult_t   procedureResult,
    bleResult_t             error
);

#endif /* _BLE_SERVICE_DISCOVERY_H_ */

/*! *********************************************************************************
* @}
********************************************************************************** */
