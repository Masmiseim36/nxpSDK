/*! *********************************************************************************
 * \addtogroup Link Loss Service
 * @{
 ********************************************************************************** */
/*!
* The Clear BSD License
* Copyright (c) 2014, Freescale Semiconductor, Inc.
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

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "ble_general.h"
#include "gatt_db_app_interface.h"
#include "gatt_server_interface.h"
#include "gap_interface.h"

#include "link_loss_interface.h"
/************************************************************************************
*************************************************************************************
* Private constants & macros
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
/*! Link Loss Service - Subscribed Client*/
static deviceId_t mLls_SubscribedClientId = gInvalidDeviceId_c;

/***********************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
bleResult_t Lls_Start(llsConfig_t *pServiceConfig)
{
    Lls_SetAlertLevel(pServiceConfig->serviceHandle, pServiceConfig->initialAlertLevel);

    mLls_SubscribedClientId = gInvalidDeviceId_c;

    return gBleSuccess_c;
}

bleResult_t Lls_Stop(llsConfig_t *pServiceConfig)
{
    return gBleSuccess_c;
}

bleResult_t Lls_Subscribe(deviceId_t clientdeviceId)
{
    if (!mLls_SubscribedClientId)
        mLls_SubscribedClientId = clientdeviceId;

    return gBleSuccess_c;
}

bleResult_t Lls_Unsubscribe(void)
{
    mLls_SubscribedClientId = gInvalidDeviceId_c;
    return gBleSuccess_c;
}

bleResult_t Lls_GetAlertLevel(uint16_t serviceHandle, llsAlertLevel_t *pOutAlertLevel)
{
    uint16_t  hAlertLevel;
    bleResult_t result;
    bleUuid_t uuid =  Uuid16(gBleSig_AlertLevel_d);
    uint16_t outLen = 0;

    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(serviceHandle,
        gBleUuidType16_c, &uuid, &hAlertLevel);

    if (result != gBleSuccess_c)
        return result;

    return GattDb_ReadAttribute(hAlertLevel, sizeof(llsAlertLevel_t), pOutAlertLevel, &outLen);
}

bleResult_t Lls_SetAlertLevel(uint16_t serviceHandle, llsAlertLevel_t alertLevel)
{
    uint16_t  hAlertLevel;
    bleResult_t result;
    bleUuid_t uuid =  Uuid16(gBleSig_AlertLevel_d);

    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(serviceHandle,
        gBleUuidType16_c, &uuid, &hAlertLevel);

    if (result != gBleSuccess_c)
        return result;

    /* Write attribute value*/
    return GattDb_WriteAttribute(hAlertLevel, sizeof(llsAlertLevel_t), &alertLevel);
}
/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/


/*! *********************************************************************************
* @}
********************************************************************************** */
