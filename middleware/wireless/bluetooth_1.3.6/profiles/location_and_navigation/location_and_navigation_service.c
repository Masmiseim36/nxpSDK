/*! *********************************************************************************
* \addtogroup Location and Navigation Service
* @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2016, Freescale Semiconductor, Inc.
* Copyright 2019 NXP
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
#include "ble_general.h"
#include "gatt_db_app_interface.h"
#include "gatt_server_interface.h"
#include "gap_interface.h"
#include "MemManager.h"

#include "location_and_navigation_interface.h"

/************************************************************************************
*************************************************************************************
* Private constants & macros
*************************************************************************************
************************************************************************************/

/***********************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
********************************************************************************** */

/***********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
********************************************************************************** */
/*! Location and Navigation Service - Subscribed Client */
static deviceId_t      mLns_ClientDeviceId = gInvalidDeviceId_c;
static bool_t          mNotificationIncomplete = FALSE;

/***********************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
********************************************************************************** */
static void Lns_SendNotification (uint16_t handle);
static bleResult_t Lns_SetLnFeatures (uint16_t handle, lnsFeatures_t features);
static bleResult_t Lns_UpdateLocAndSpeedCharacteristic (uint16_t handle, lnsLocAndSpeed_t *pLocAndSpeed);
static bleResult_t Lns_UpdateNavigationCharacteristic (uint16_t handle, lnsNavigation_t *pNavigation);
static bleResult_t Lns_UpdatePositionQualityCharacteristic (uint16_t handle, lnsPositionQuality_t *pPositionQuality);

/***********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */
bleResult_t Lns_Start (lnsConfig_t *pServiceConfig)
{
    bleResult_t result = gBleSuccess_c;

    result =  Lns_SetLnFeatures(pServiceConfig->serviceHandle, pServiceConfig->lnsFeatures);
    if (result == gBleSuccess_c)
    {
        result = Lns_RecordPositionQualityCharacteristic(pServiceConfig->serviceHandle, &(pServiceConfig->pUserData->lnsPositionQuality));
    }

    return result;
}

bleResult_t Lns_Stop (lnsConfig_t *pServiceConfig)
{
    return Lns_Unsubscribe(pServiceConfig);
}

bleResult_t Lns_Subscribe(deviceId_t clientdeviceId)
{
    mLns_ClientDeviceId = clientdeviceId;
    return gBleSuccess_c;
}

bleResult_t Lns_Unsubscribe(lnsConfig_t *pServiceConfig)
{
    mLns_ClientDeviceId = gInvalidDeviceId_c;

    return gBleSuccess_c;
}

bleResult_t Lns_RecordLocationAndSpeedCharacteristic(uint16_t serviceHandle, lnsLocAndSpeed_t *pLocAndSpeed)
{
    union
    {
        uint16_t*  uuidTemp;
        bleUuid_t* bleUuidTemp;
    }uuidVars;

    uint16_t    handle;
    bleResult_t result;
    uint16_t    uuid = gBleSig_LocationAndSpeed_d;

    uuidVars.uuidTemp = &uuid;
    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(serviceHandle,
        gBleUuidType16_c, uuidVars.bleUuidTemp, &handle);

    if (result == gBleSuccess_c)
    {
        /* Update characteristic value and send indication */
        if (gBleSuccess_c == Lns_UpdateLocAndSpeedCharacteristic(handle, pLocAndSpeed))
        {
            Lns_SendNotification(handle);
        }
    }

    return result;
}

bleResult_t Lns_RecordNavigationCharacteristic(uint16_t serviceHandle, lnsNavigation_t *pNavigation)
{
    union
    {
        uint16_t*  uuidTemp;
        bleUuid_t* bleUuidTemp;
    }uuidVars;

    uint16_t    handle;
    bleResult_t result;
    uint16_t    uuid = gBleSig_Navigation_d;

    uuidVars.uuidTemp = &uuid;
    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(serviceHandle,
        gBleUuidType16_c, uuidVars.bleUuidTemp, &handle);

    if (result == gBleSuccess_c)
    {
        /* Update characteristic value and send indication */
        if (gBleSuccess_c == Lns_UpdateNavigationCharacteristic(handle, pNavigation))
        {
            Lns_SendNotification(handle);
        }
    }

    return result;
}

bleResult_t Lns_RecordPositionQualityCharacteristic(uint16_t serviceHandle, lnsPositionQuality_t *pPositionQuality)
{
    union
    {
        uint16_t*  uuidTemp;
        bleUuid_t* bleUuidTemp;
    }uuidVars;

    uint16_t    handle;
    bleResult_t result;
    uint16_t    uuid = gBleSig_PositionQuality_d;

    uuidVars.uuidTemp = &uuid;
    /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(serviceHandle,
        gBleUuidType16_c, uuidVars.bleUuidTemp, &handle);

    if (result == gBleSuccess_c)
    {
        /* Update characteristic value */
        (void)Lns_UpdatePositionQualityCharacteristic(handle, pPositionQuality);
    }

    return result;
}

/***********************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
********************************************************************************** */
static bleResult_t Lns_SetLnFeatures (uint16_t handle, lnsFeatures_t features)
{
    union
    {
        uint16_t*  uuidTemp;
        bleUuid_t* bleUuidTemp;
    }uuidVars;

    uint16_t  hValueLnFeature;
    uint16_t uuid = gBleSig_LnFeature_d;
    bleResult_t result;

    uuidVars.uuidTemp = &uuid;
      /* Get handle of characteristic */
    result = GattDb_FindCharValueHandleInService(handle,
                gBleUuidType16_c, uuidVars.bleUuidTemp, &hValueLnFeature);

    if (result == gBleSuccess_c)
    {
        result = GattDb_WriteAttribute(hValueLnFeature, sizeof(lnsFeatures_t), (uint8_t*)&features);
    }

    return result;
}

static bleResult_t Lns_UpdateLocAndSpeedCharacteristic (uint16_t handle, lnsLocAndSpeed_t *pLocAndSpeed)
{
    uint8_t             charValue[20];
    uint8_t             index = 0x00;
    bleResult_t         result;
    ctsDayDateTime_t    utcTime;
    uint16_t            flags = 0x0000;

    if(!mNotificationIncomplete)
    {
        flags = pLocAndSpeed->lnsLocAndSpeedFlags & 0xFF9FU;

        FLib_MemCpy(&charValue[index], &flags, sizeof(uint16_t));
        index += sizeof(uint16_t);

        if((pLocAndSpeed->lnsLocAndSpeedFlags & gLns_InstantaneousSpeedPresent_c) != 0U)
        {
            FLib_MemCpy(&charValue[index], &pLocAndSpeed->lnsInstantaneousSpeed, sizeof(uint16_t));
            index += sizeof(uint16_t);
        }

        if((pLocAndSpeed->lnsLocAndSpeedFlags & gLns_TotalDistancePresent_c) != 0U)
        {
            FLib_MemCpy(&charValue[index], &pLocAndSpeed->lnsTotalDistance, 3);
            index += 3U;
        }

        if((pLocAndSpeed->lnsLocAndSpeedFlags & gLns_LocationPresent_c) != 0U)
        {
            FLib_MemCpy(&charValue[index], &pLocAndSpeed->lnsLatitude, sizeof(int32_t));
            index += sizeof(int32_t);
            FLib_MemCpy(&charValue[index], &pLocAndSpeed->lnsLongitude, sizeof(int32_t));
            index += sizeof(int32_t);
        }

        if((pLocAndSpeed->lnsLocAndSpeedFlags & gLns_ElevationPresent_c) != 0U)
        {
            FLib_MemCpy(&charValue[index], &pLocAndSpeed->lnsElevation, 3);
            index += 3U;
        }

        if((pLocAndSpeed->lnsLocAndSpeedFlags & gLns_HeadingPresent_c) != 0U)
        {
            FLib_MemCpy(&charValue[index], &pLocAndSpeed->lnsHeading, sizeof(uint16_t));
            index += sizeof(uint16_t);
        }
        mNotificationIncomplete = TRUE;
    }
    else
    {
        flags = (uint16_t)(pLocAndSpeed->lnsLocAndSpeedFlags & 0x0060U);

        FLib_MemCpy(&charValue[index], &flags, sizeof(uint16_t));
        index += sizeof(uint16_t);

        if((pLocAndSpeed->lnsLocAndSpeedFlags & gLns_RollingTimePresent_c) != 0U)
        {
            FLib_MemCpy(&charValue[index], &pLocAndSpeed->lnsRollingTime, sizeof(uint8_t));
            index += sizeof(uint8_t);
        }

        if(0U != (pLocAndSpeed->lnsLocAndSpeedFlags & gLns_UtcTimePresent_c))
        {
            utcTime = Cts_EpochToDayDateTime(pLocAndSpeed->lnsUtcTime);
            FLib_MemCpy(&charValue[index], &utcTime.dateTime, sizeof(ctsDateTime_t));
            index += sizeof(ctsDateTime_t);
        }
        mNotificationIncomplete = FALSE;
    }

    result = GattDb_WriteAttribute(handle, index, &charValue[0]);
    return result;
}

static bleResult_t Lns_UpdateNavigationCharacteristic (uint16_t handle, lnsNavigation_t *pNavigation)
{
    uint8_t             charValue[20];
    uint8_t             index = 0x00;
    bleResult_t         result;
    ctsDayDateTime_t    eta;

    FLib_MemCpy(&charValue[index], &pNavigation->lnsNavigationFlags, sizeof(uint16_t));
    index += sizeof(uint16_t);

    FLib_MemCpy(&charValue[index], &pNavigation->lnsBearing, sizeof(uint16_t));
    index += sizeof(uint16_t);

    FLib_MemCpy(&charValue[index], &pNavigation->lnsHeading, sizeof(uint16_t));
    index += sizeof(uint16_t);

    if(0U != (pNavigation->lnsNavigationFlags & gLns_RemainingDistancePresent_c))
    {
        FLib_MemCpy(&charValue[index], &pNavigation->lnsRemainingDistance, 3);
        index += 3U;
    }

    if(0U != (pNavigation->lnsNavigationFlags & gLns_RemainingVerticalDistancePresent_c))
    {
        FLib_MemCpy(&charValue[index], &pNavigation->lnsRemainingVerticalDistance, 3U);
        index += 3U;
    }

    if(0U != (pNavigation->lnsNavigationFlags & gLns_EstimatedTimeOfArrivalPresent_c))
    {
        eta = Cts_EpochToDayDateTime(pNavigation->lnsETA);
        FLib_MemCpy(&charValue[index], &eta.dateTime, sizeof(ctsDateTime_t));
        index += sizeof(ctsDateTime_t);
    }

    result = GattDb_WriteAttribute(handle, index, &charValue[0]);
    return result;
}

static bleResult_t Lns_UpdatePositionQualityCharacteristic (uint16_t handle, lnsPositionQuality_t *pPositionQuality)
{
    uint8_t             charValue[16];
    uint8_t             index = 0x00;
    bleResult_t         result;

    FLib_MemCpy(&charValue[index], &pPositionQuality->lnsPositionQualityFlags, sizeof(uint16_t));
    index += sizeof(uint16_t);

    if((pPositionQuality->lnsPositionQualityFlags & gLns_NoOfBeaconsInSolutionPresent_c) != 0U)
    {
        FLib_MemCpy(&charValue[index], &pPositionQuality->lnsNumberOfBeaconsInSolution, sizeof(uint8_t));
        index += sizeof(uint8_t);
    }

    if((pPositionQuality->lnsPositionQualityFlags & gLns_NoOfBeaconsInViewPresent_c) != 0U)
    {
        FLib_MemCpy(&charValue[index], &pPositionQuality->lnsNumberOfBeaconsInView, sizeof(uint8_t));
        index += sizeof(uint8_t);
    }

    if((pPositionQuality->lnsPositionQualityFlags & gLns_TimeToFirstFixPresent_c) != 0U)
    {
        FLib_MemCpy(&charValue[index], &pPositionQuality->lnsTimeToFirstFix, sizeof(uint16_t));
        index += sizeof(uint16_t);
    }

    if((pPositionQuality->lnsPositionQualityFlags & gLns_EHPEPresent_c) != 0U)
    {
        FLib_MemCpy(&charValue[index], &pPositionQuality->lnsEHPE, sizeof(uint32_t));
        index += sizeof(uint32_t);
    }

    if((pPositionQuality->lnsPositionQualityFlags & gLns_EVPEPresent_c) != 0U)
    {
        FLib_MemCpy(&charValue[index], &pPositionQuality->lnsEVPE, sizeof(uint32_t));
        index += sizeof(uint32_t);
    }

    if((pPositionQuality->lnsPositionQualityFlags & gLns_HDOPPresent_c) != 0U)
    {
        FLib_MemCpy(&charValue[index], &pPositionQuality->lnsHDOP, sizeof(uint8_t));
        index += sizeof(uint8_t);
    }

    if((pPositionQuality->lnsPositionQualityFlags & gLns_VDOPPresent_c) != 0U)
    {
        FLib_MemCpy(&charValue[index], &pPositionQuality->lnsVDOP, sizeof(uint8_t));
        index += sizeof(uint8_t);
    }

    result = GattDb_WriteAttribute(handle, index, &charValue[0]);
    return result;
}

static void Lns_SendProcedureResponse (lnsConfig_t *pServiceConfig, gattServerAttributeWrittenEvent_t* pEvent)
{
    union
    {
        uint8_t*        aValueTemp;
        lnsProcedure_t* pProcedureTemp;
    }procedureVars;

    uint32_t            rspSize = 3U;
    lnsProcedure_t*     pResponse;
    procedureVars.aValueTemp = pEvent->aValue;
    lnsProcedure_t*     pProcedure = procedureVars.pProcedureTemp;
    uint8_t             procDataLength = (uint8_t)(pEvent->cValueLength - sizeof(lnsOpCode_t));
    uint16_t            handleCccd;
    uint16_t            handle;
    uint16_t            uuid = gBleSig_Navigation_d;
    bool_t              earlyReturn = FALSE;

    union
    {
        uint16_t*   uuidTemp;
        bleUuid_t*  bleUuidTemp;
    }uuidVars;

    /* Allocate buffer for biggest response */
    pResponse = MEM_BufferAlloc(rspSize + 4U);

    if (pResponse != NULL)
    {
        pResponse->opCode = gLns_ResponseCode_c;
        pResponse->procedureData.response.reqOpCode = pProcedure->opCode;

        switch (pProcedure->opCode)
        {
            case gLns_SetCumulativeValue_c:
            {
                if (procDataLength == 3U) /* size of INT24 */
                {
                    pServiceConfig->pUserData->lnsLocAndSpeed.lnsTotalDistance = pProcedure->procedureData.lnsTotalDistance;
                    pResponse->procedureData.response.rspValue = gErrCodeNoError_c;
                }
                else
                {
                    pResponse->procedureData.response.rspValue = gErrCodeLnsInvalidParameter_c;
                }
                break;
            }

            case gLns_MaskLocAndSpeedContent_c:
            {
                if (procDataLength == (sizeof(uint16_t)))
                {
                    pResponse->procedureData.response.rspValue = gErrCodeNoError_c;
                }
                else
                {
                    pResponse->procedureData.response.rspValue = gErrCodeLnsInvalidParameter_c;
                }
                break;
            }

            case gLns_NavigationControl_c:
            {
                if (procDataLength == (sizeof(uint8_t)))
                {

                    uuidVars.uuidTemp = &uuid;
                    /* Get handle of characteristic */
                    if(GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
                        gBleUuidType16_c, uuidVars.bleUuidTemp, &handle) == gBleSuccess_c)
                    {
                        /* Get handle of CCCD */
                        if (GattDb_FindCccdHandleForCharValueHandle(handle, &handleCccd) != gBleSuccess_c)
                        {
                            earlyReturn = TRUE;
                            break;
                        }
                    }
                    else
                    {
                        earlyReturn = TRUE;
                        break;
                    }

                    if(pProcedure->procedureData.lnsNavigationControl == gLnsStopNavigation_c)
                    {
                        (void)Gap_SaveCccd(mLns_ClientDeviceId, handleCccd, gCccdEmpty_c);
                    }
                    else if (pProcedure->procedureData.lnsNavigationControl == gLnsStartNavigationFirstWaypoint_c)
                    {
                        (void)Gap_SaveCccd(mLns_ClientDeviceId, handleCccd, (gCccdNotification_c));
                    }
                    else if (pProcedure->procedureData.lnsNavigationControl == gLnsPauseNavigation_c)
                    {
                        (void)Gap_SaveCccd(mLns_ClientDeviceId, handleCccd, (gCccdEmpty_c));
                    }
                    else if (pProcedure->procedureData.lnsNavigationControl == gLnsContinueNavigation_c)
                    {
                        (void)Gap_SaveCccd(mLns_ClientDeviceId, handleCccd, (gCccdNotification_c));
                    }
                    else if (pProcedure->procedureData.lnsNavigationControl == gLnsSkipWaypoint_c)
                    {

                    }
                    else if (pProcedure->procedureData.lnsNavigationControl == gLnsNearestWaypoint_c)
                    {

                    }
                    else
                    {
                        ; /* For MISRA compliance */
                    }
                    pResponse->procedureData.response.rspValue = gErrCodeNoError_c;
                }
                else
                {
                    pResponse->procedureData.response.rspValue = gErrCodeLnsInvalidParameter_c;
                }
                break;
            }

            case gLns_RequestNumberOfRoutes_c:
            {
                pResponse->procedureData.response.rspValue = gErrCodeNoError_c;
                pResponse->procedureData.response.rspData.lnsNumberOfRoutes = 0x0002;
                rspSize += sizeof(uint16_t);
                break;
            }

            case gLns_RequestNameOfRoutes_c:
            {
                pResponse->procedureData.response.rspValue = gErrCodeNoError_c;
                const char* name = "asdf";
                FLib_MemCpy(pResponse->procedureData.response.rspData.lnsNameOfRoute, name, 4);
                rspSize += sizeof(uint32_t);
                break;
            }

            case gLns_SelectRoute_c:
            {
                pResponse->procedureData.response.rspValue = gErrCodeNoError_c;
                pResponse->procedureData.response.rspData.lnsSelectedRoute = 0x0001;
                break;
            }

            case gLns_SetElevation_c:
            {
                if (procDataLength == 3U) /* size of INT24 */
                {
                    pServiceConfig->pUserData->lnsLocAndSpeed.lnsElevation = pProcedure->procedureData.lnsElevation;
                    pResponse->procedureData.response.rspValue = gErrCodeNoError_c;
                }
                else
                {
                    pResponse->procedureData.response.rspValue = gErrCodeLnsInvalidParameter_c;
                }
                break;
            }

            case gLns_SetFixRate_c:
            {
                if (procDataLength == sizeof(uint8_t))
                {
                    pResponse->procedureData.response.rspValue = gErrCodeNoError_c;
                }
                else
                {
                    pResponse->procedureData.response.rspValue = gErrCodeLnsInvalidParameter_c;
                }
                break;
            }

            default:
            {
                pResponse->procedureData.response.rspValue = gErrCodeLnsOpCodeNotSupported_c;
                break;
            }

        }

        if (!earlyReturn)
        {
            /* Write response in characteristic */
            (void)GattDb_WriteAttribute(pEvent->handle, (uint16_t)rspSize, (uint8_t*) pResponse);

            /* Indicate value to client */
            (void)GattServer_SendIndication(mLns_ClientDeviceId, pEvent->handle);
        }

        /* Free buffer */
        (void)MEM_BufferFree(pResponse);
    }
}

void Lns_ControlPointHandler (lnsConfig_t *pServiceConfig, gattServerAttributeWrittenEvent_t *pEvent)
{
    bool_t  fIndicationActive = FALSE;
    uint16_t  handleCccd;

    /* Get handle of CCCD */
    if (GattDb_FindCccdHandleForCharValueHandle(pEvent->handle, &handleCccd) == gBleSuccess_c)
    {
        /* Check if indications are properly configured */
        (void)Gap_CheckIndicationStatus(mLns_ClientDeviceId, handleCccd, &fIndicationActive);

        if(!fIndicationActive)
        {
            (void)GattServer_SendAttributeWrittenStatus(mLns_ClientDeviceId, pEvent->handle,
                                                     (uint8_t)gAttErrCodeCccdImproperlyConfigured_c);
        }
        else
        {
            /* Procedure received successfully */
            (void)GattServer_SendAttributeWrittenStatus(mLns_ClientDeviceId, pEvent->handle,
                                                             (uint8_t)gAttErrCodeNoError_c);

            Lns_SendProcedureResponse(pServiceConfig, pEvent);
        }
    }
}

static void Lns_SendNotification (uint16_t handle)
{
    uint16_t    hCccd;
    bool_t      isNotificationActive;

    /* Get handle of CCCD */
    if (GattDb_FindCccdHandleForCharValueHandle(handle, &hCccd) == gBleSuccess_c)
    {
        if (gBleSuccess_c == Gap_CheckNotificationStatus
            (mLns_ClientDeviceId, hCccd, &isNotificationActive) &&
            TRUE == isNotificationActive)
        {
            (void)GattServer_SendNotification(mLns_ClientDeviceId, handle);
        }
    }
}

/*! *********************************************************************************
* @}
********************************************************************************** */
