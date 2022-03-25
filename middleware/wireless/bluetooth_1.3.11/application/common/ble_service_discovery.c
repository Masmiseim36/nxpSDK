/*! *********************************************************************************
 * \addtogroup BLE
 * @{
 ********************************************************************************* */
/*! *********************************************************************************
* Copyright 2016-2021 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "EmbeddedTypes.h"
#include "MemManager.h"
#include "ble_service_discovery.h"
#include "ApplMain.h"

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/
typedef struct servDiscInfo_tag
{
    /* Buffer used for Service Discovery */
    gattService_t   *mpServiceDiscoveryBuffer;

    /* Buffer used for Characteristic Discovery */
    gattCharacteristic_t *mpCharDiscoveryBuffer;

    /* Buffer used for Characteristic Descriptor Discovery */
    gattAttribute_t *mpCharDescriptorBuffer;

    /* Counters */
    uint8_t mCurrentServiceInDiscoveryIndex;
    uint8_t mCurrentCharInDiscoveryIndex;
    uint8_t mCurrentDescInDiscoveryIndex;
    uint8_t mcPrimaryServices;
    bool_t  mServDiscInProgress;

}servDiscInfo_t;

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/
static void BleServDisc_Reset(deviceId_t peerDeviceId);
static void BleServDisc_Finished(deviceId_t peerDeviceId, bool_t result);
STATIC void BleServDisc_NewService(deviceId_t peerDeviceId, gattService_t *pService);
/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
static servDiscCallback_t pfServDiscCallback = NULL;

STATIC servDiscInfo_t maServDiscInfo[gAppMaxConnections_c];

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/***********************************************************************************
* \brief     Installs an application callback for the Service Discovery module.
*
* \param[in] pServDiscCallback         Pointer to service discovery callback.
*
* \param[out] none
*
********************************************************************************** */
void BleServDisc_RegisterCallback(servDiscCallback_t pServDiscCallback)
{
    pfServDiscCallback = pServDiscCallback;
}

/*! *********************************************************************************
* \brief    Starts the Service Discovery procedure with the peer device.
*
* \param[in] peerDeviceId      The GAP peer Id.
*
* \return  gBleSuccess_c or error.
*
********************************************************************************** */
bleResult_t BleServDisc_Start(deviceId_t peerDeviceId)
{
    bleResult_t result = gBleSuccess_c;

    if (!maServDiscInfo[peerDeviceId].mServDiscInProgress)
    {
        /* Allocate memory for Service Discovery */
        maServDiscInfo[peerDeviceId].mpServiceDiscoveryBuffer = MEM_BufferAlloc(sizeof(gattService_t) * (uint32_t)gMaxServicesCount_d);
        maServDiscInfo[peerDeviceId].mpCharDiscoveryBuffer = MEM_BufferAlloc(sizeof(gattCharacteristic_t) * (uint32_t)gMaxServiceCharCount_d);
        maServDiscInfo[peerDeviceId].mpCharDescriptorBuffer = MEM_BufferAlloc(sizeof(gattAttribute_t) * (uint32_t)gMaxCharDescriptorsCount_d);

        if (maServDiscInfo[peerDeviceId].mpServiceDiscoveryBuffer != NULL &&
            maServDiscInfo[peerDeviceId].mpCharDiscoveryBuffer != NULL &&
            maServDiscInfo[peerDeviceId].mpCharDescriptorBuffer != NULL)
        {
            maServDiscInfo[peerDeviceId].mServDiscInProgress = TRUE;

            /* Start Service Discovery*/
            result = GattClient_DiscoverAllPrimaryServices(
                                        peerDeviceId,
                                        maServDiscInfo[peerDeviceId].mpServiceDiscoveryBuffer,
                                        gMaxServicesCount_d,
                                        &maServDiscInfo[peerDeviceId].mcPrimaryServices);
        }
        else
        {
            BleServDisc_Reset(peerDeviceId);
            result = gBleOutOfMemory_c;
        }
    }
    else
    {
        result = gBleInvalidState_c;
    }
    return result;
}

/*! *********************************************************************************
* \brief    Starts the Service Discovery procedure for a specified service UUID,
*           with the peer device.
*
* \param[in] peerDeviceId       The GAP peer Id.
* \param[in] uuidType           Service UUID type.
* \param[in] pUuid              Service UUID.
*
* \return  gBleSuccess_c or error.
*
********************************************************************************** */
bleResult_t BleServDisc_FindService(deviceId_t peerDeviceId, bleUuidType_t uuidType, bleUuid_t *pUuid)
{
    bleResult_t result = gBleSuccess_c;

    if (!maServDiscInfo[peerDeviceId].mServDiscInProgress)
    {
        /* Allocate memory for Service Discovery */
        maServDiscInfo[peerDeviceId].mpServiceDiscoveryBuffer = MEM_BufferAlloc(sizeof(gattService_t));
        maServDiscInfo[peerDeviceId].mpCharDiscoveryBuffer = MEM_BufferAlloc(sizeof(gattCharacteristic_t) * (uint32_t)gMaxServiceCharCount_d);
        maServDiscInfo[peerDeviceId].mpCharDescriptorBuffer = MEM_BufferAlloc(sizeof(gattAttribute_t) * (uint32_t)gMaxCharDescriptorsCount_d);

        if (maServDiscInfo[peerDeviceId].mpServiceDiscoveryBuffer != NULL &&
            maServDiscInfo[peerDeviceId].mpCharDiscoveryBuffer != NULL &&
            maServDiscInfo[peerDeviceId].mpCharDescriptorBuffer != NULL)
        {
            maServDiscInfo[peerDeviceId].mServDiscInProgress = TRUE;

            /* Start Service Discovery*/
            result = GattClient_DiscoverPrimaryServicesByUuid(
                                                    peerDeviceId,
                                                    uuidType,
                                                    pUuid,
                                                    maServDiscInfo[peerDeviceId].mpServiceDiscoveryBuffer,
                                                    1,
                                                    &maServDiscInfo[peerDeviceId].mcPrimaryServices);
        }
        else
        {
            BleServDisc_Reset(peerDeviceId);
            result = gBleOutOfMemory_c;
        }
    }
    else
    {
        result = gBleInvalidState_c;
    }
    return result;
}

/*! *********************************************************************************
* \brief    Stops the Service Discovery procedure with the peer device.
*
* \param[in] peerDeviceId      The GAP peer Id.
*
* \return  gBleSuccess_c or error.
*
********************************************************************************** */
void BleServDisc_Stop(deviceId_t peerDeviceId)
{
    if (maServDiscInfo[peerDeviceId].mServDiscInProgress)
    {
        maServDiscInfo[peerDeviceId].mServDiscInProgress = FALSE;
        BleServDisc_Reset(peerDeviceId);
    }
}

/*! *********************************************************************************
* \brief        Signals the module a GATT client callback from host stack.
*               Must be called by the application, which is responsible for
*
* \param[in]    peerDeviceId        GATT Server device ID.
* \param[in]    procedureType       Procedure type.
* \param[in]    procedureResult     Procedure result.
* \param[in]    error               Callback result.
*
* \return  none
*
********************************************************************************** */
void BleServDisc_SignalGattClientEvent(
    deviceId_t              peerDeviceId,
    gattProcedureType_t     procedureType,
    gattProcedureResult_t   procedureResult,
    bleResult_t             error
)
{
    servDiscInfo_t  *pInfo = &maServDiscInfo[peerDeviceId];
    if (pInfo->mServDiscInProgress)
    {
        if (procedureResult == gGattProcError_c)
        {
            BleServDisc_Finished(peerDeviceId, FALSE);
        }
        else
        {
            switch (procedureType)
            {
                case gGattProcDiscoverPrimaryServicesByUuid_c:      /* Fall through */
                case gGattProcDiscoverAllPrimaryServices_c:
                {
                    /* We found at least one service */
                    if (pInfo->mcPrimaryServices != 0U)
                    {
                        /* Start characteristic discovery with first service*/
                        pInfo->mCurrentServiceInDiscoveryIndex = 0;
                        pInfo->mCurrentCharInDiscoveryIndex = 0;
                        pInfo->mCurrentDescInDiscoveryIndex = 0;

                        pInfo->mpServiceDiscoveryBuffer->aCharacteristics = pInfo->mpCharDiscoveryBuffer;

                        /* Start Characteristic Discovery for current service */
                        (void)GattClient_DiscoverAllCharacteristicsOfService(
                                                    peerDeviceId,
                                                    pInfo->mpServiceDiscoveryBuffer,
                                                    gMaxServiceCharCount_d);
                    }
                    else
                    {
                        BleServDisc_Finished(peerDeviceId, TRUE);
                    }
                }
                break;

                case gGattProcDiscoverAllCharacteristicDescriptors_c:   /* Fall through */
                case gGattProcDiscoverAllCharacteristics_c:
                {
                    if (procedureType == gGattProcDiscoverAllCharacteristicDescriptors_c)
                    {
                        gattService_t        *pCurrentService = pInfo->mpServiceDiscoveryBuffer + pInfo->mCurrentServiceInDiscoveryIndex;
                        gattCharacteristic_t *pCurrentChar = pCurrentService->aCharacteristics + pInfo->mCurrentCharInDiscoveryIndex;

                        pInfo->mCurrentDescInDiscoveryIndex += pCurrentChar->cNumDescriptors;

                        /* Move on to the next characteristic */
                        pInfo->mCurrentCharInDiscoveryIndex++;
                    }

                    gattService_t        *pCurrentService = pInfo->mpServiceDiscoveryBuffer + pInfo->mCurrentServiceInDiscoveryIndex;

                    if (pInfo->mCurrentCharInDiscoveryIndex < pCurrentService->cNumCharacteristics)
                    {
                        gattCharacteristic_t *pCurrentChar = pCurrentService->aCharacteristics + pInfo->mCurrentCharInDiscoveryIndex;

                        /* Find next characteristic with descriptors*/
                        while (pInfo->mCurrentCharInDiscoveryIndex < pCurrentService->cNumCharacteristics - 1U)
                        {
                            /* Check if we have handles available between adjacent characteristics */
                            if (pCurrentChar->value.handle + 2U < (pCurrentChar + 1)->value.handle)
                            {
                                if (pInfo->mCurrentDescInDiscoveryIndex < (uint8_t)gMaxCharDescriptorsCount_d)
                                {
                                    pCurrentChar->aDescriptors = pInfo->mpCharDescriptorBuffer + pInfo->mCurrentDescInDiscoveryIndex;
                                    (void)GattClient_DiscoverAllCharacteristicDescriptors(peerDeviceId,
                                                            pCurrentChar,
                                                            (pCurrentChar + 1)->value.handle,
                                                            (uint8_t)(gMaxCharDescriptorsCount_d - pInfo->mCurrentDescInDiscoveryIndex));
                                    return;
                                }
                            }

                            pInfo->mCurrentCharInDiscoveryIndex++;
                            pCurrentChar = pCurrentService->aCharacteristics + pInfo->mCurrentCharInDiscoveryIndex;
                        }

                        /* Made it to the last characteristic. Check against service end handle*/
                        if (pCurrentChar->value.handle < pCurrentService->endHandle)
                        {
                            if (pInfo->mCurrentDescInDiscoveryIndex < gMaxCharDescriptorsCount_d)
                            {
                                pCurrentChar->aDescriptors = pInfo->mpCharDescriptorBuffer + pInfo->mCurrentDescInDiscoveryIndex;
                                (void)GattClient_DiscoverAllCharacteristicDescriptors(peerDeviceId,
                                                        pCurrentChar,
                                                        pCurrentService->endHandle,
                                                        (uint8_t)(gMaxCharDescriptorsCount_d - pInfo->mCurrentDescInDiscoveryIndex));
                                return;
                            }

                        }
                    }

                    /* Signal Discovery of Service */
                    BleServDisc_NewService(peerDeviceId, pCurrentService);

                    /* Move on to the next service */
                    pInfo->mCurrentServiceInDiscoveryIndex++;

                    /* Reset characteristic discovery */
                    pInfo->mCurrentCharInDiscoveryIndex = 0;
                    pInfo->mCurrentDescInDiscoveryIndex = 0;
                    FLib_MemSet(pInfo->mpCharDescriptorBuffer, 0, sizeof(gattAttribute_t) * (uint32_t)gMaxCharDescriptorsCount_d);
                    FLib_MemSet(pInfo->mpCharDiscoveryBuffer, 0, sizeof(gattCharacteristic_t) * (uint32_t)gMaxServiceCharCount_d);

                    if (pInfo->mCurrentServiceInDiscoveryIndex < pInfo->mcPrimaryServices)
                    {
                        /* Allocate memory for Char Discovery */
                        (pInfo->mpServiceDiscoveryBuffer + pInfo->mCurrentServiceInDiscoveryIndex)->aCharacteristics = pInfo->mpCharDiscoveryBuffer;

                         /* Start Characteristic Discovery for current service */
                        (void)GattClient_DiscoverAllCharacteristicsOfService(peerDeviceId,
                                pInfo->mpServiceDiscoveryBuffer + pInfo->mCurrentServiceInDiscoveryIndex,
                                                    gMaxServiceCharCount_d);
                    }
                    else
                    {
                        BleServDisc_Finished(peerDeviceId, TRUE);
                    }
                }
                break;

                default:
                {
                    ; /* No action required */
                }
                break;
            }
        }
    }
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief        Frees the memory that has been allocated for Service Discovery
*
* \param[in]    peerDeviceId        GATT Server device ID.

* \return  none
*
********************************************************************************** */
static void BleServDisc_Reset(deviceId_t peerDeviceId)
{
    if (maServDiscInfo[peerDeviceId].mpServiceDiscoveryBuffer != NULL)
    {
        (void)MEM_BufferFree(maServDiscInfo[peerDeviceId].mpServiceDiscoveryBuffer);
        maServDiscInfo[peerDeviceId].mpServiceDiscoveryBuffer = NULL;
    }

    if (maServDiscInfo[peerDeviceId].mpCharDiscoveryBuffer != NULL)
    {
        (void)MEM_BufferFree(maServDiscInfo[peerDeviceId].mpCharDiscoveryBuffer);
        maServDiscInfo[peerDeviceId].mpCharDiscoveryBuffer = NULL;
    }

    if (maServDiscInfo[peerDeviceId].mpCharDescriptorBuffer != NULL)
    {
        (void)MEM_BufferFree(maServDiscInfo[peerDeviceId].mpCharDescriptorBuffer);
        maServDiscInfo[peerDeviceId].mpCharDescriptorBuffer = NULL;
    }
}

/*! *********************************************************************************
* \brief    Stops the Service Discovery procedure with the peer device.
*
* \param[in] peerDeviceId      The GAP peer Id
*
* \param[in] result            The result (true or false)
*
* \return none
*
********************************************************************************** */
static void BleServDisc_Finished(deviceId_t peerDeviceId, bool_t result)
{
    servDiscEvent_t event;

    BleServDisc_Stop(peerDeviceId);

    event.eventType = gDiscoveryFinished_c;
    event.eventData.success = result;
    pfServDiscCallback(peerDeviceId, &event);
}

/*! *********************************************************************************
* \brief    Signals the discovery of a new service
*
* \param[in] peerDeviceId      The GAP peer Id.
* \param[in] pService          The service that was discovered
*
* \return none
*
********************************************************************************** */
STATIC void BleServDisc_NewService(deviceId_t peerDeviceId, gattService_t *pService)
{
    servDiscEvent_t event;

    event.eventType = gServiceDiscovered_c;
    event.eventData.pService = pService;
    pfServDiscCallback(peerDeviceId, &event);
}

/*! *********************************************************************************
* @}
********************************************************************************** */
