/*! *********************************************************************************
 * \addtogroup GATT_DB
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2020 NXP
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
#include "board.h"

#include "gatt_database_dynamic.h"

/************************************************************************************
*************************************************************************************
* Private constants & macros
*************************************************************************************
************************************************************************************/

#define mGattDbDynamic_GattServiceChangedInitValue                    {0x00, 0x00, 0x00, 0x00}

#define mGattDbDynamic_GapServiceDeviceNameInitValue                  "Kinetis BLE"
#define mGattDbDynamic_GapServiceAppearanceInitValue                  {UuidArray((uint8_t)gUnknown_c)}
#define mGattDbDynamic_GapServicePpcpInitValue                        {0x0A, 0x00, 0x10, 0x00, 0x64, 0x00, 0xE2, 0x04}

#define mGattDbDynamic_HRServiceHrMeasurementInitValue                {0x00, 0xB4}
#define mGattDbDynamic_HRServiceBodySensorLocationInitValue           {0x01}
#define mGattDbDynamic_HRServiceHrControlPointInitValue               {0x00}

#define mGattDbDynamic_BServiceLevelInitValue                         {0x5A}
#define mGattDbDynamic_BServiceCharPresFormatDescriptorInitValue      {0x04, 0x00, 0xAD, 0x27, 0x01, 0x00, 0x00}

#define mGattDbDynamic_DIServiceManufNameInitValue                    MANUFACTURER_NAME
#define mGattDbDynamic_DIServiceModelNbInitValue                      "Kinetis BLE"
#define mGattDbDynamic_DIServiceSerialNoInitValue                     "BLESN01"
#define mGattDbDynamic_DIServiceHwRevInitValue                        BOARD_NAME
#define mGattDbDynamic_DIServiceFwRevInitValue                        "1.1.1"
#define mGattDbDynamic_DIServiceSwRevInitValue                        "1.1.4"
#define mGattDbDynamic_DIServiceSysIdInitValue                        {0x00, 0x00, 0x00, 0xFE, 0xFF, 0x9F, 0x04, 0x00}
#define mGattDbDynamic_DIServiceIeeeRcdlInitValue                     {0x00, 0x00, 0x00, 0x00}

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

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/


bleResult_t GattDbDynamic_AddServiceInDatabase(serviceInfo_t* pServiceInfo)
{
    union
    {
        uint16_t*    uuidTemp;
        bleUuid_t*   bleUuidTemp;
    }uuidVars;

    characteristicInfo_t* pCharacteristicInfo = NULL;
    descriptorInfo_t*     pDescriptorInfo = NULL;
    uint32_t              i = 0;
    uint32_t              j = 0;
    bleResult_t           result = gBleSuccess_c;

    /* Add service */
    uuidVars.uuidTemp = &pServiceInfo->uuid16;
    result = GattDbDynamic_AddPrimaryServiceDeclaration(0, gBleUuidType16_c,
                                                        uuidVars.bleUuidTemp,
                                                        &pServiceInfo->handle);

    if( gBleSuccess_c == result )
    {
        for( i = 0; i < pServiceInfo->nbOfCharacteristics; i++ )
        {
            /* Add characteristic */
            pCharacteristicInfo = &pServiceInfo->pCharacteristicInfo[i];
            uuidVars.uuidTemp = &pCharacteristicInfo->uuid16;
            result = GattDbDynamic_AddCharacteristicDeclarationAndValue(gBleUuidType16_c,
                                                                        uuidVars.bleUuidTemp,
                                                                        pCharacteristicInfo->properties,
                                                                        pCharacteristicInfo->maxValueLength,
                                                                        pCharacteristicInfo->valueLength,
                                                                        pCharacteristicInfo->pValue,
                                                                        pCharacteristicInfo->accessPermissions,
                                                                        &pCharacteristicInfo->handle);

            if( gBleSuccess_c != result )
            {
                break;
            }
            else
            {
                for( j = 0; j < pCharacteristicInfo->nbOfDescriptors; j++ )
                {
                    /* Add descriptor */
                    pDescriptorInfo = &pCharacteristicInfo->pDescriptorInfo[j];
                    uuidVars.uuidTemp = &pDescriptorInfo->uuid16;
                    result = GattDbDynamic_AddCharDescriptor(gBleUuidType16_c,
                                                                       uuidVars.bleUuidTemp,
                                                                       pDescriptorInfo->valueLength,
                                                                       pDescriptorInfo->pValue,
                                                                       pDescriptorInfo->accessPermissions,
                                                                       &pDescriptorInfo->handle);

                    if( gBleSuccess_c != result )
                    {
                        break;
                    }
                }
            }

            if( pCharacteristicInfo->bAddCccd && (gBleSuccess_c == result) )
            {
                /* Add Cccd */
                result = GattDbDynamic_AddCccd(&pCharacteristicInfo->cccdHandle);
            }

            if( gBleSuccess_c != result )
            {
                break;
            }
        }
    }

    return result;
}

bleResult_t GattDbDynamic_AddGattService(gattServiceHandles_t* pOutServiceHandles)
{
    union
    {
        uint8_t*                serviceInfoTemp;
        characteristicInfo_t*   characteristicInfoTemp;
    }infoVars;

    uint8_t             gattServiceChangedValue[]   = mGattDbDynamic_GattServiceChangedInitValue;
    serviceInfo_t*      pServiceInfo                = (serviceInfo_t*)MEM_BufferAlloc(sizeof(serviceInfo_t) +
                                                                                     1U * sizeof(characteristicInfo_t) +
                                                                                     0U * sizeof(descriptorInfo_t) +
                                                                                     sizeof(gattServiceChangedValue));
    bleResult_t         result = gBleSuccess_c;

    if( NULL != pServiceInfo )
    {
        pServiceInfo->uuid16                = gBleSig_GenericAttributeProfile_d;
        pServiceInfo->nbOfCharacteristics   = 1;
        infoVars.serviceInfoTemp = ((uint8_t*)pServiceInfo + sizeof(serviceInfo_t));
        pServiceInfo->pCharacteristicInfo   = infoVars.characteristicInfoTemp;
        pServiceInfo->pCharacteristicInfo[0].uuid16             = gBleSig_GattServiceChanged_d;
        pServiceInfo->pCharacteristicInfo[0].properties         = (gattCharacteristicPropertiesBitFields_t)((uint8_t)gGattCharPropRead_c | (uint8_t)gGattCharPropNotify_c);
        pServiceInfo->pCharacteristicInfo[0].maxValueLength     = 0;
        pServiceInfo->pCharacteristicInfo[0].valueLength        = (uint16_t)sizeof(gattServiceChangedValue);
        pServiceInfo->pCharacteristicInfo[0].pValue             = (uint8_t*)pServiceInfo->pCharacteristicInfo + sizeof(characteristicInfo_t);
        FLib_MemCpy(pServiceInfo->pCharacteristicInfo[0].pValue, &gattServiceChangedValue, sizeof(gattServiceChangedValue));
        pServiceInfo->pCharacteristicInfo[0].accessPermissions  = gPermissionNone_c;
        pServiceInfo->pCharacteristicInfo[0].bAddCccd           = TRUE;
        pServiceInfo->pCharacteristicInfo[0].nbOfDescriptors    = 0;
        pServiceInfo->pCharacteristicInfo[0].pDescriptorInfo    = NULL;

        result = GattDbDynamic_AddServiceInDatabase(pServiceInfo);

        if( (gBleSuccess_c == result) && (NULL != pOutServiceHandles) )
        {
            /* Fill service handles */
            pOutServiceHandles->serviceHandle = pServiceInfo->handle;
            pOutServiceHandles->charServiceChangedHandle = pServiceInfo->pCharacteristicInfo->handle;
            pOutServiceHandles->charServiceChangedCccdHandle = pServiceInfo->pCharacteristicInfo->cccdHandle;
        }

        /* Free allocated service */
        (void)MEM_BufferFree(pServiceInfo);
    }
    else
    {
        result = gBleOutOfMemory_c;
    }

    return result;
}

bleResult_t GattDbDynamic_AddGapService(gapServiceHandles_t* pOutServiceHandles)
{
    union
    {
        uint8_t*                serviceInfoTemp;
        characteristicInfo_t*   characteristicInfoTemp;
    }infoVars;

    uint8_t             gapDeviceNameValue[]    = mGattDbDynamic_GapServiceDeviceNameInitValue;
    uint8_t             gapAppearanceValue[]    = mGattDbDynamic_GapServiceAppearanceInitValue;
    uint8_t             gapPpcpValue[]          = mGattDbDynamic_GapServicePpcpInitValue;
    serviceInfo_t*      pServiceInfo            = (serviceInfo_t*)MEM_BufferAlloc(sizeof(serviceInfo_t) +
                                                                                 3U * sizeof(characteristicInfo_t) +
                                                                                 0U * sizeof(descriptorInfo_t) +
                                                                                 sizeof(gapDeviceNameValue) +
                                                                                 sizeof(gapAppearanceValue) +
                                                                                 sizeof(gapPpcpValue));
    bleResult_t         result = gBleSuccess_c;

    if( NULL != pServiceInfo )
    {
        pServiceInfo->uuid16                = gBleSig_GenericAccessProfile_d;
        pServiceInfo->nbOfCharacteristics   = 3;
        infoVars.serviceInfoTemp = ((uint8_t*)pServiceInfo + sizeof(serviceInfo_t));
        pServiceInfo->pCharacteristicInfo   = infoVars.characteristicInfoTemp;
        pServiceInfo->pCharacteristicInfo[0].uuid16             = gBleSig_GapDeviceName_d;
        pServiceInfo->pCharacteristicInfo[0].properties         = (uint8_t)gGattCharPropRead_c;
        pServiceInfo->pCharacteristicInfo[0].maxValueLength     = 20;
        pServiceInfo->pCharacteristicInfo[0].valueLength        = (uint16_t)sizeof(gapDeviceNameValue);
        pServiceInfo->pCharacteristicInfo[0].pValue             = (uint8_t*)pServiceInfo->pCharacteristicInfo + 3U * sizeof(characteristicInfo_t);
        FLib_MemCpy(pServiceInfo->pCharacteristicInfo[0].pValue, &gapDeviceNameValue, sizeof(gapDeviceNameValue));
        pServiceInfo->pCharacteristicInfo[0].accessPermissions  = gPermissionFlagReadable_c;
        pServiceInfo->pCharacteristicInfo[0].bAddCccd           = FALSE;
        pServiceInfo->pCharacteristicInfo[0].nbOfDescriptors    = 0;
        pServiceInfo->pCharacteristicInfo[0].pDescriptorInfo    = NULL;

        pServiceInfo->pCharacteristicInfo[1].uuid16             = gBleSig_GapAppearance_d;
        pServiceInfo->pCharacteristicInfo[1].properties         = (uint8_t)gGattCharPropRead_c;
        pServiceInfo->pCharacteristicInfo[1].maxValueLength     = 0;
        pServiceInfo->pCharacteristicInfo[1].valueLength        = (uint16_t)sizeof(gapAppearanceValue);
        pServiceInfo->pCharacteristicInfo[1].pValue             = pServiceInfo->pCharacteristicInfo[0].pValue + sizeof(gapDeviceNameValue);
        FLib_MemCpy(pServiceInfo->pCharacteristicInfo[1].pValue, &gapAppearanceValue, sizeof(gapAppearanceValue));
        pServiceInfo->pCharacteristicInfo[1].accessPermissions  = gPermissionFlagReadable_c;
        pServiceInfo->pCharacteristicInfo[1].bAddCccd           = FALSE;
        pServiceInfo->pCharacteristicInfo[1].nbOfDescriptors    = 0;
        pServiceInfo->pCharacteristicInfo[1].pDescriptorInfo    = NULL;

        pServiceInfo->pCharacteristicInfo[2].uuid16             = gBleSig_GapPpcp_d;
        pServiceInfo->pCharacteristicInfo[2].properties         = (uint8_t)gGattCharPropRead_c;
        pServiceInfo->pCharacteristicInfo[2].maxValueLength     = 0;
        pServiceInfo->pCharacteristicInfo[2].valueLength        = (uint16_t)sizeof(gapPpcpValue);
        pServiceInfo->pCharacteristicInfo[2].pValue             = pServiceInfo->pCharacteristicInfo[1].pValue + sizeof(gapAppearanceValue);
        FLib_MemCpy(pServiceInfo->pCharacteristicInfo[2].pValue, &gapPpcpValue, sizeof(gapPpcpValue));
        pServiceInfo->pCharacteristicInfo[2].accessPermissions  = gPermissionFlagReadable_c;
        pServiceInfo->pCharacteristicInfo[2].bAddCccd           = FALSE;
        pServiceInfo->pCharacteristicInfo[2].nbOfDescriptors    = 0;
        pServiceInfo->pCharacteristicInfo[2].pDescriptorInfo    = NULL;

        result = GattDbDynamic_AddServiceInDatabase(pServiceInfo);

        if( (gBleSuccess_c == result) && (NULL != pOutServiceHandles) )
        {
            /* Fill service handles */
            pOutServiceHandles->serviceHandle = pServiceInfo->handle;
            pOutServiceHandles->charDeviceNameHandle = pServiceInfo->pCharacteristicInfo[0].handle;
            pOutServiceHandles->charAppearanceHandle = pServiceInfo->pCharacteristicInfo[1].handle;
            pOutServiceHandles->charPpcpHandle = pServiceInfo->pCharacteristicInfo[2].handle;
        }

        /* Free allocated service */
        (void)MEM_BufferFree(pServiceInfo);
    }
    else
    {
        result = gBleOutOfMemory_c;
    }

    return result;
}

bleResult_t GattDbDynamic_AddIpssService(ipssServiceHandles_t* pOutServiceHandles)
{
    serviceInfo_t* pServiceInfo = GattDbDynamic_GetIpssService();
    bleResult_t    result = gBleSuccess_c;

    if( NULL != pServiceInfo )
    {
        result = GattDbDynamic_AddServiceInDatabase(pServiceInfo);

        if( (gBleSuccess_c == result) && (NULL != pOutServiceHandles) )
        {
            /* Fill service handles */
            pOutServiceHandles->serviceHandle = pServiceInfo->handle;
        }

        /* Free allocated service */
        (void)MEM_BufferFree(pServiceInfo);
    }
    else
    {
        result = gBleOutOfMemory_c;
    }

    return result;
}

bleResult_t GattDbDynamic_AddHeartRateService(heartRateServiceHandles_t* pOutServiceHandles)
{
    serviceInfo_t*      pServiceInfo = GattDbDynamic_GetHeartRateService();
    bleResult_t         result = gBleSuccess_c;

    if( NULL != pServiceInfo )
    {
        result = GattDbDynamic_AddServiceInDatabase(pServiceInfo);

        if( (gBleSuccess_c == result) && (NULL != pOutServiceHandles) )
        {
            /* Fill service handles */
            pOutServiceHandles->serviceHandle = pServiceInfo->handle;
            pOutServiceHandles->charHrMeasurementHandle = pServiceInfo->pCharacteristicInfo[0].handle;
            pOutServiceHandles->charHrMeasurementCccdHandle = pServiceInfo->pCharacteristicInfo[0].cccdHandle;
            pOutServiceHandles->charBodySensorLocationHandle = pServiceInfo->pCharacteristicInfo[1].handle;
            pOutServiceHandles->charHrCtrlPointHandle = pServiceInfo->pCharacteristicInfo[2].handle;
        }

        /* Free allocated service */
        (void)MEM_BufferFree(pServiceInfo);
    }
    else
    {
        result = gBleOutOfMemory_c;
    }

    return result;
}

bleResult_t GattDbDynamic_AddBatteryService(batteryServiceHandles_t* pOutServiceHandles)
{
    serviceInfo_t*      pServiceInfo = GattDbDynamic_GetBatteryService();
    bleResult_t         result = gBleSuccess_c;

    if( NULL != pServiceInfo )
    {
        result = GattDbDynamic_AddServiceInDatabase(pServiceInfo);

        if( (gBleSuccess_c == result) && (NULL != pOutServiceHandles) )
        {
            /* Fill service handles */
            pOutServiceHandles->serviceHandle = pServiceInfo->handle;
            pOutServiceHandles->charBatteryLevelHandle = pServiceInfo->pCharacteristicInfo[0].handle;
            pOutServiceHandles->charBatteryLevelCccdHandle = pServiceInfo->pCharacteristicInfo[0].cccdHandle;
        }

        /* Free allocated service */
        (void)MEM_BufferFree(pServiceInfo);
    }
    else
    {
        result = gBleOutOfMemory_c;
    }

    return result;
}

bleResult_t GattDbDynamic_AddDeviceInformationService(deviceInfoServiceHandles_t* pOutServiceHandles)
{
    serviceInfo_t*      pServiceInfo = GattDbDynamic_GetDeviceInformationService();
    bleResult_t         result = gBleSuccess_c;

    if( NULL != pServiceInfo )
    {
        result = GattDbDynamic_AddServiceInDatabase(pServiceInfo);

        if( (gBleSuccess_c == result) && (NULL != pOutServiceHandles) )
        {
            /* Fill service handles */
            pOutServiceHandles->serviceHandle = pServiceInfo->handle;
            pOutServiceHandles->charManufNameHandle = pServiceInfo->pCharacteristicInfo[0].handle;
            pOutServiceHandles->charModelNoHandle = pServiceInfo->pCharacteristicInfo[1].handle;
            pOutServiceHandles->charSerialNoHandle = pServiceInfo->pCharacteristicInfo[2].handle;
            pOutServiceHandles->charHwRevHandle = pServiceInfo->pCharacteristicInfo[3].handle;
            pOutServiceHandles->charFwRevHandle = pServiceInfo->pCharacteristicInfo[4].handle;
            pOutServiceHandles->charSwRevHandle = pServiceInfo->pCharacteristicInfo[5].handle;
            pOutServiceHandles->charSystemIdHandle = pServiceInfo->pCharacteristicInfo[6].handle;
            pOutServiceHandles->charRcdlHandle = pServiceInfo->pCharacteristicInfo[7].handle;
        }

        /* Free allocated service */
        (void)MEM_BufferFree(pServiceInfo);
    }
    else
    {
        result = gBleOutOfMemory_c;
    }

    return result;
}

serviceInfo_t* GattDbDynamic_GetIpssService(void)
{
    serviceInfo_t* pServiceInfo = (serviceInfo_t*)MEM_BufferAlloc(sizeof(serviceInfo_t) +
                                                                  0U * sizeof(characteristicInfo_t) +
                                                                  0U * sizeof(descriptorInfo_t));

    if( NULL != pServiceInfo )
    {
        pServiceInfo->uuid16                = gBleSig_IpsService_d;
        pServiceInfo->nbOfCharacteristics   = 0;
        pServiceInfo->pCharacteristicInfo   = NULL;
    }

    return pServiceInfo;
}

serviceInfo_t* GattDbDynamic_GetHeartRateService(void)
{
    union
    {
        uint8_t*                serviceInfoTemp;
        characteristicInfo_t*   characteristicInfoTemp;
    }infoVars;

    uint8_t             hrHrMeasurementValue[]      = mGattDbDynamic_HRServiceHrMeasurementInitValue;
    uint8_t             hrBodySensorLocationValue[] = mGattDbDynamic_HRServiceBodySensorLocationInitValue;
    uint8_t             hrHrControlPointValue[]     = mGattDbDynamic_HRServiceHrControlPointInitValue;
    serviceInfo_t*      pServiceInfo                = (serviceInfo_t*)MEM_BufferAlloc(sizeof(serviceInfo_t) +
                                                                                    3U * sizeof(characteristicInfo_t) +
                                                                                    0U * sizeof(descriptorInfo_t) +
                                                                                    sizeof(hrHrMeasurementValue) +
                                                                                    sizeof(hrBodySensorLocationValue) +
                                                                                    sizeof(hrHrControlPointValue));

    if( NULL != pServiceInfo )
    {
        pServiceInfo->uuid16                = gBleSig_HeartRateService_d;
        pServiceInfo->nbOfCharacteristics   = 3;
        infoVars.serviceInfoTemp = (uint8_t*)pServiceInfo + sizeof(serviceInfo_t);
        pServiceInfo->pCharacteristicInfo   = infoVars.characteristicInfoTemp;

        pServiceInfo->pCharacteristicInfo[0].uuid16             = gBleSig_HrMeasurement_d;
        pServiceInfo->pCharacteristicInfo[0].properties         = gGattCharPropNotify_c;
        pServiceInfo->pCharacteristicInfo[0].maxValueLength     = 22;
        pServiceInfo->pCharacteristicInfo[0].valueLength        = (uint16_t)sizeof(hrHrMeasurementValue);
        pServiceInfo->pCharacteristicInfo[0].pValue             = (uint8_t*)pServiceInfo->pCharacteristicInfo + 3U * sizeof(characteristicInfo_t);
        FLib_MemCpy(pServiceInfo->pCharacteristicInfo[0].pValue, &hrHrMeasurementValue, sizeof(hrHrMeasurementValue));
        pServiceInfo->pCharacteristicInfo[0].accessPermissions  = gPermissionNone_c;
        pServiceInfo->pCharacteristicInfo[0].bAddCccd           = TRUE;
        pServiceInfo->pCharacteristicInfo[0].nbOfDescriptors    = 0;
        pServiceInfo->pCharacteristicInfo[0].pDescriptorInfo    = NULL;

        pServiceInfo->pCharacteristicInfo[1].uuid16             = gBleSig_BodySensorLocation_d;
        pServiceInfo->pCharacteristicInfo[1].properties         = (uint8_t)gGattCharPropRead_c;
        pServiceInfo->pCharacteristicInfo[1].maxValueLength     = 0;
        pServiceInfo->pCharacteristicInfo[1].valueLength        = (uint16_t)sizeof(hrBodySensorLocationValue);
        pServiceInfo->pCharacteristicInfo[1].pValue             = pServiceInfo->pCharacteristicInfo[0].pValue + sizeof(hrHrMeasurementValue);
        FLib_MemCpy(pServiceInfo->pCharacteristicInfo[1].pValue, &hrBodySensorLocationValue, sizeof(hrBodySensorLocationValue));
        pServiceInfo->pCharacteristicInfo[1].accessPermissions  = gPermissionFlagReadable_c;
        pServiceInfo->pCharacteristicInfo[1].bAddCccd           = FALSE;
        pServiceInfo->pCharacteristicInfo[1].nbOfDescriptors    = 0;
        pServiceInfo->pCharacteristicInfo[1].pDescriptorInfo    = NULL;

        pServiceInfo->pCharacteristicInfo[2].uuid16             = gBleSig_HrControlPoint_d;
        pServiceInfo->pCharacteristicInfo[2].properties         = (uint8_t)gGattCharPropWrite_c;
        pServiceInfo->pCharacteristicInfo[2].maxValueLength     = 0;
        pServiceInfo->pCharacteristicInfo[2].valueLength        = (uint16_t)sizeof(hrHrControlPointValue);
        pServiceInfo->pCharacteristicInfo[2].pValue             = pServiceInfo->pCharacteristicInfo[1].pValue + sizeof(hrBodySensorLocationValue);
        FLib_MemCpy(pServiceInfo->pCharacteristicInfo[2].pValue, &hrHrControlPointValue, sizeof(hrHrControlPointValue));
        pServiceInfo->pCharacteristicInfo[2].accessPermissions  = gPermissionFlagWritable_c;
        pServiceInfo->pCharacteristicInfo[2].bAddCccd           = FALSE;
        pServiceInfo->pCharacteristicInfo[2].nbOfDescriptors    = 0;
        pServiceInfo->pCharacteristicInfo[2].pDescriptorInfo    = NULL;
    }

    return pServiceInfo;
}

serviceInfo_t* GattDbDynamic_GetBatteryService(void)
{
    union
    {
        uint8_t*                serviceInfoTemp;
        descriptorInfo_t*       descriptorInfoTemp;
        characteristicInfo_t*   characteristicInfoTemp;
    }infoVars;
    uint8_t             bBatteryLevelValue[]                = mGattDbDynamic_BServiceLevelInitValue;
    uint8_t             bCharPresFormatDescriptorValue[]    = mGattDbDynamic_BServiceCharPresFormatDescriptorInitValue;
    serviceInfo_t*      pServiceInfo                        = (serviceInfo_t*)MEM_BufferAlloc(sizeof(serviceInfo_t) +
                                                                                             1U * sizeof(characteristicInfo_t) +
                                                                                             1U * sizeof(descriptorInfo_t) +
                                                                                             sizeof(bBatteryLevelValue) +
                                                                                             sizeof(bCharPresFormatDescriptorValue));

    if( NULL != pServiceInfo )
    {
        pServiceInfo->uuid16                = gBleSig_BatteryService_d;
        pServiceInfo->nbOfCharacteristics   = 1;
        infoVars.serviceInfoTemp = ((uint8_t*)pServiceInfo + sizeof(serviceInfo_t));
        pServiceInfo->pCharacteristicInfo   = infoVars.characteristicInfoTemp;

        pServiceInfo->pCharacteristicInfo[0].uuid16             = gBleSig_BatteryLevel_d;
        pServiceInfo->pCharacteristicInfo[0].properties         = (gattCharacteristicPropertiesBitFields_t)((uint8_t)gGattCharPropNotify_c | (uint8_t)gGattCharPropRead_c);
        pServiceInfo->pCharacteristicInfo[0].maxValueLength     = 0;
        pServiceInfo->pCharacteristicInfo[0].valueLength        = (uint16_t)sizeof(bBatteryLevelValue);
        pServiceInfo->pCharacteristicInfo[0].pValue             = (uint8_t*)pServiceInfo->pCharacteristicInfo + 1U * sizeof(characteristicInfo_t) + 1U * sizeof(descriptorInfo_t);
        FLib_MemCpy(pServiceInfo->pCharacteristicInfo[0].pValue, &bBatteryLevelValue, sizeof(bBatteryLevelValue));
        pServiceInfo->pCharacteristicInfo[0].accessPermissions  = gPermissionFlagReadable_c;
        pServiceInfo->pCharacteristicInfo[0].bAddCccd           = TRUE;
        pServiceInfo->pCharacteristicInfo[0].nbOfDescriptors    = 1;
        infoVars.serviceInfoTemp = ((uint8_t*)pServiceInfo->pCharacteristicInfo + 1U * sizeof(characteristicInfo_t));
        pServiceInfo->pCharacteristicInfo[0].pDescriptorInfo    = infoVars.descriptorInfoTemp;

        pServiceInfo->pCharacteristicInfo[0].pDescriptorInfo[0].uuid16              = gBleSig_CharPresFormatDescriptor_d;
        pServiceInfo->pCharacteristicInfo[0].pDescriptorInfo[0].valueLength         = (uint16_t)sizeof(bCharPresFormatDescriptorValue);
        pServiceInfo->pCharacteristicInfo[0].pDescriptorInfo[0].pValue              = (uint8_t*)pServiceInfo->pCharacteristicInfo[0].pValue + sizeof(bBatteryLevelValue);
        FLib_MemCpy(pServiceInfo->pCharacteristicInfo[0].pDescriptorInfo[0].pValue, &bCharPresFormatDescriptorValue, sizeof(bCharPresFormatDescriptorValue));
        pServiceInfo->pCharacteristicInfo[0].pDescriptorInfo[0].accessPermissions   = gPermissionFlagReadable_c;
    }

    return pServiceInfo;
}

serviceInfo_t* GattDbDynamic_GetDeviceInformationService(void)
{
    union
    {
        uint8_t*                serviceInfoTemp;
        characteristicInfo_t*   characteristicInfoTemp;
    }infoVars;

    uint8_t             diManufacturerNameValue[]   = mGattDbDynamic_DIServiceManufNameInitValue;
    uint8_t             diModelNumberValue[]        = mGattDbDynamic_DIServiceModelNbInitValue;
    uint8_t             diSerialNumberValue[]       = mGattDbDynamic_DIServiceSerialNoInitValue;
    uint8_t             diHardwareRevisionValue[]   = mGattDbDynamic_DIServiceHwRevInitValue;
    uint8_t             diFirmwareRevisionValue[]   = mGattDbDynamic_DIServiceFwRevInitValue;
    uint8_t             diSoftwareRevisionValue[]   = mGattDbDynamic_DIServiceSwRevInitValue;
    uint8_t             diSystemIdValue[]           = mGattDbDynamic_DIServiceSysIdInitValue;
    uint8_t             diIeeeRcdlValue[]           = mGattDbDynamic_DIServiceIeeeRcdlInitValue;
    serviceInfo_t*      pServiceInfo                = (serviceInfo_t*)MEM_BufferAlloc(sizeof(serviceInfo_t) +
                                                                                      8U * sizeof(characteristicInfo_t) +
                                                                                      0U * sizeof(descriptorInfo_t) +
                                                                                      sizeof(diManufacturerNameValue) +
                                                                                      sizeof(diModelNumberValue) +
                                                                                      sizeof(diSerialNumberValue) +
                                                                                      sizeof(diHardwareRevisionValue) +
                                                                                      sizeof(diFirmwareRevisionValue) +
                                                                                      sizeof(diSoftwareRevisionValue) +
                                                                                      sizeof(diSystemIdValue) +
                                                                                      sizeof(diIeeeRcdlValue));

    if( NULL != pServiceInfo )
    {
        pServiceInfo->uuid16                = gBleSig_DeviceInformationService_d;
        pServiceInfo->nbOfCharacteristics   = 8;
        infoVars.serviceInfoTemp = ((uint8_t*)pServiceInfo + sizeof(serviceInfo_t));
        pServiceInfo->pCharacteristicInfo   = infoVars.characteristicInfoTemp;
        pServiceInfo->pCharacteristicInfo[0].uuid16             = gBleSig_ManufacturerNameString_d;
        pServiceInfo->pCharacteristicInfo[0].properties         = (uint8_t)gGattCharPropRead_c;
        pServiceInfo->pCharacteristicInfo[0].maxValueLength     = 0;
        pServiceInfo->pCharacteristicInfo[0].valueLength        = (uint16_t)sizeof(diManufacturerNameValue);
        pServiceInfo->pCharacteristicInfo[0].pValue             = (uint8_t*)pServiceInfo->pCharacteristicInfo + 8U * sizeof(characteristicInfo_t);
        FLib_MemCpy(pServiceInfo->pCharacteristicInfo[0].pValue, &diManufacturerNameValue, sizeof(diManufacturerNameValue));
        pServiceInfo->pCharacteristicInfo[0].accessPermissions  = gPermissionFlagReadable_c;
        pServiceInfo->pCharacteristicInfo[0].bAddCccd           = FALSE;
        pServiceInfo->pCharacteristicInfo[0].nbOfDescriptors    = 0;
        pServiceInfo->pCharacteristicInfo[0].pDescriptorInfo    = NULL;

        pServiceInfo->pCharacteristicInfo[1].uuid16             = gBleSig_ModelNumberString_d;
        pServiceInfo->pCharacteristicInfo[1].properties         = (uint8_t)gGattCharPropRead_c;
        pServiceInfo->pCharacteristicInfo[1].maxValueLength     = 0;
        pServiceInfo->pCharacteristicInfo[1].valueLength        = (uint16_t)sizeof(diModelNumberValue);
        pServiceInfo->pCharacteristicInfo[1].pValue             = pServiceInfo->pCharacteristicInfo[0].pValue + sizeof(diManufacturerNameValue);
        FLib_MemCpy(pServiceInfo->pCharacteristicInfo[1].pValue, &diModelNumberValue, sizeof(diModelNumberValue));
        pServiceInfo->pCharacteristicInfo[1].accessPermissions  = gPermissionFlagReadable_c;
        pServiceInfo->pCharacteristicInfo[1].bAddCccd           = FALSE;
        pServiceInfo->pCharacteristicInfo[1].nbOfDescriptors    = 0;
        pServiceInfo->pCharacteristicInfo[1].pDescriptorInfo    = NULL;

        pServiceInfo->pCharacteristicInfo[2].uuid16             = gBleSig_SerialNumberString_d;
        pServiceInfo->pCharacteristicInfo[2].properties         = (uint8_t)gGattCharPropRead_c;
        pServiceInfo->pCharacteristicInfo[2].maxValueLength     = 0;
        pServiceInfo->pCharacteristicInfo[2].valueLength        = (uint16_t)sizeof(diSerialNumberValue);
        pServiceInfo->pCharacteristicInfo[2].pValue             = pServiceInfo->pCharacteristicInfo[1].pValue + sizeof(diModelNumberValue);
        FLib_MemCpy(pServiceInfo->pCharacteristicInfo[2].pValue, &diSerialNumberValue, sizeof(diSerialNumberValue));
        pServiceInfo->pCharacteristicInfo[2].accessPermissions  = gPermissionFlagReadable_c;
        pServiceInfo->pCharacteristicInfo[2].bAddCccd           = FALSE;
        pServiceInfo->pCharacteristicInfo[2].nbOfDescriptors    = 0;
        pServiceInfo->pCharacteristicInfo[2].pDescriptorInfo    = NULL;

        pServiceInfo->pCharacteristicInfo[3].uuid16             = gBleSig_HardwareRevisionString_d;
        pServiceInfo->pCharacteristicInfo[3].properties         = (uint8_t)gGattCharPropRead_c;
        pServiceInfo->pCharacteristicInfo[3].maxValueLength     = 0;
        pServiceInfo->pCharacteristicInfo[3].valueLength        = (uint16_t)sizeof(diHardwareRevisionValue);
        pServiceInfo->pCharacteristicInfo[3].pValue             = pServiceInfo->pCharacteristicInfo[2].pValue + sizeof(diSerialNumberValue);
        FLib_MemCpy(pServiceInfo->pCharacteristicInfo[3].pValue, &diHardwareRevisionValue, sizeof(diHardwareRevisionValue));
        pServiceInfo->pCharacteristicInfo[3].accessPermissions  = gPermissionFlagReadable_c;
        pServiceInfo->pCharacteristicInfo[3].bAddCccd           = FALSE;
        pServiceInfo->pCharacteristicInfo[3].nbOfDescriptors    = 0;
        pServiceInfo->pCharacteristicInfo[3].pDescriptorInfo    = NULL;

        pServiceInfo->pCharacteristicInfo[4].uuid16             = gBleSig_FirmwareRevisionString_d;
        pServiceInfo->pCharacteristicInfo[4].properties         = (uint8_t)gGattCharPropRead_c;
        pServiceInfo->pCharacteristicInfo[4].maxValueLength     = 0;
        pServiceInfo->pCharacteristicInfo[4].valueLength        = (uint16_t)sizeof(diFirmwareRevisionValue);
        pServiceInfo->pCharacteristicInfo[4].pValue             = pServiceInfo->pCharacteristicInfo[3].pValue + sizeof(diHardwareRevisionValue);
        FLib_MemCpy(pServiceInfo->pCharacteristicInfo[4].pValue, &diFirmwareRevisionValue, sizeof(diFirmwareRevisionValue));
        pServiceInfo->pCharacteristicInfo[4].accessPermissions  = gPermissionFlagReadable_c;
        pServiceInfo->pCharacteristicInfo[4].bAddCccd           = FALSE;
        pServiceInfo->pCharacteristicInfo[4].nbOfDescriptors    = 0;
        pServiceInfo->pCharacteristicInfo[4].pDescriptorInfo    = NULL;

        pServiceInfo->pCharacteristicInfo[5].uuid16             = gBleSig_SoftwareRevisionString_d;
        pServiceInfo->pCharacteristicInfo[5].properties         = (uint8_t)gGattCharPropRead_c;
        pServiceInfo->pCharacteristicInfo[5].maxValueLength     = 0;
        pServiceInfo->pCharacteristicInfo[5].valueLength        = (uint16_t)sizeof(diSoftwareRevisionValue);
        pServiceInfo->pCharacteristicInfo[5].pValue             = pServiceInfo->pCharacteristicInfo[4].pValue + sizeof(diFirmwareRevisionValue);
        FLib_MemCpy(pServiceInfo->pCharacteristicInfo[5].pValue, &diSoftwareRevisionValue, sizeof(diSoftwareRevisionValue));
        pServiceInfo->pCharacteristicInfo[5].accessPermissions  = gPermissionFlagReadable_c;
        pServiceInfo->pCharacteristicInfo[5].bAddCccd           = FALSE;
        pServiceInfo->pCharacteristicInfo[5].nbOfDescriptors    = 0;
        pServiceInfo->pCharacteristicInfo[5].pDescriptorInfo    = NULL;

        pServiceInfo->pCharacteristicInfo[6].uuid16             = gBleSig_SystemId_d;
        pServiceInfo->pCharacteristicInfo[6].properties         = (uint8_t)gGattCharPropRead_c;
        pServiceInfo->pCharacteristicInfo[6].maxValueLength     = 0;
        pServiceInfo->pCharacteristicInfo[6].valueLength        = (uint16_t)sizeof(diSystemIdValue);
        pServiceInfo->pCharacteristicInfo[6].pValue             = pServiceInfo->pCharacteristicInfo[5].pValue + sizeof(diSoftwareRevisionValue);
        FLib_MemCpy(pServiceInfo->pCharacteristicInfo[6].pValue, &diSystemIdValue, sizeof(diSystemIdValue));
        pServiceInfo->pCharacteristicInfo[6].accessPermissions  = gPermissionFlagReadable_c;
        pServiceInfo->pCharacteristicInfo[6].bAddCccd           = FALSE;
        pServiceInfo->pCharacteristicInfo[6].nbOfDescriptors    = 0;
        pServiceInfo->pCharacteristicInfo[6].pDescriptorInfo    = NULL;

        pServiceInfo->pCharacteristicInfo[7].uuid16             = gBleSig_IeeeRcdl_d;
        pServiceInfo->pCharacteristicInfo[7].properties         = (uint8_t)gGattCharPropRead_c;
        pServiceInfo->pCharacteristicInfo[7].maxValueLength     = 0;
        pServiceInfo->pCharacteristicInfo[7].valueLength        = (uint16_t)sizeof(diIeeeRcdlValue);
        pServiceInfo->pCharacteristicInfo[7].pValue             = pServiceInfo->pCharacteristicInfo[6].pValue + sizeof(diSystemIdValue);
        FLib_MemCpy(pServiceInfo->pCharacteristicInfo[7].pValue, &diIeeeRcdlValue, sizeof(diIeeeRcdlValue));
        pServiceInfo->pCharacteristicInfo[7].accessPermissions  = gPermissionFlagReadable_c;
        pServiceInfo->pCharacteristicInfo[7].bAddCccd           = FALSE;
        pServiceInfo->pCharacteristicInfo[7].nbOfDescriptors    = 0;
        pServiceInfo->pCharacteristicInfo[7].pDescriptorInfo    = NULL;
    }

    return pServiceInfo;
}

/******************************************************;******************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* @}
********************************************************************************** */
