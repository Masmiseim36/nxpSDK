/*! *********************************************************************************
* \addtogroup FSCI_BLE
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
* All rights reserved.
*
* \file
*
* This is a source file for FSCI BLE management.
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/

#include "fsci_ble_gatt_db_app.h"

#if gFsciBleHost_d
    #include "host_ble.h"
#endif /* gFsciBleHost_d */


#if gFsciIncluded_c && gFsciBleGattDbAppLayerEnabled_d

/************************************************************************************
*************************************************************************************
* Private constants & macros
*************************************************************************************
************************************************************************************/

#if gFsciBleHost_d
    /* Macro used for saving the out parameters pointers of the GATT Database
    (application) functions */
    #define fsciBleGattDbAppSaveOutParams(pFirstParam, pSecondParam)    \
            fsciBleGattDbAppOutParams.pParam1 = (uint8_t*)pFirstParam;  \
            fsciBleGattDbAppOutParams.pParam2 = (uint8_t*)pSecondParam

    /* Macro used for restoring the out parameters pointers of the GATT Database
    (application) functions */
    #define fsciBleGattDbAppRestoreOutParams()    \
            &fsciBleGattDbAppOutParams;

    /* Macro used for cleaning the out parameters pointers of the GATT Database
    (application) functions to NULL */
    #define fsciBleGattDbAppCleanOutParams()            \
            fsciBleGattDbAppOutParams.pParam1 = NULL;   \
            fsciBleGattDbAppOutParams.pParam2 = NULL
#endif /* gFsciBleHost_d */


#define fsciBleGattDbAppCallApiFunction(apiFunction)        bleResult_t result = (apiFunction); \
                                                            fsciBleGattDbAppStatusMonitor(result)
#define fsciBleGattDbAppMonitorOutParams(functionId, ...)   if(gBleSuccess_c == result)                  \
                                                            {                                            \
                                                                FsciEvtMonitor(functionId, __VA_ARGS__); \
                                                            }

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

#if gFsciBleHost_d
    /* Structure used for keeping the out parameters pointers of the GATT Database
    (application) functions */
    typedef struct fsciBleGattDbAppOutParams_tag
    {
        uint8_t*    pParam1;
        uint8_t*    pParam2;
    }fsciBleGattDbAppOutParams_t;

    /* Structure used for keeping the out parameters pointers of the
    GattDb_ReadAttribute function */
    typedef struct fsciBleGattDbAppReadAttributeOutParams_tag
    {
        uint8_t*    pValue;
        uint16_t*   pValueLength;
    }fsciBleGattDbAppReadAttributeOutParams_t;

    /* Structure used for keeping the out parameter pointer of the
    GattDb_FindServiceHandle function */
    typedef struct fsciBleGattDbAppFindServiceHandleOutParams_tag
    {
        uint16_t*   pOutServiceHandle;
    }fsciBleGattDbAppFindServiceHandleOutParams_t;

    /* Structure used for keeping the out parameter pointer of the
    GattDb_FindCharValueHandleInService function */
    typedef struct fsciBleGattDbAppFindCharValueHandleInServiceOutParams_tag
    {
        uint16_t*   pCharValueHandle;
    }fsciBleGattDbAppFindCharValueHandleInServiceOutParams_t;

    /* Structure used for keeping the out parameter pointer of the
    GattDb_FindCccdHandleForCharValueHandle function */
    typedef struct fsciBleGattDbAppFindCccdHandleForCharValueHandleOutParams_tag
    {
        uint16_t*   pCccdHandle;
    }fsciBleGattDbAppFindCccdHandleForCharValueHandleOutParams_t;

    /* Structure used for keeping the out parameter pointer of the
    GattDb_FindDescriptorHandleForCharValueHandle function */
    typedef struct fsciBleGattDbAppFindDescriptorHandleForCharValueHandleOutParams_tag
    {
        uint16_t*   pDescriptorHandle;
    }fsciBleGattDbAppFindDescriptorHandleForCharValueHandleOutParams_t;

    /* Structure used for keeping the out parameters pointers of the
    GattDbDynamic_AddPrimaryServiceDeclaration function */
    typedef struct fsciBleGattDbAppAddPrimaryServiceDeclarationOutParams_tag
    {
        uint16_t* pOutHandle;
    }fsciBleGattDbAppAddPrimaryServiceDeclarationOutParams_t;

    /* Structure used for keeping the out parameters pointers of the
    GattDbDynamic_AddSecondaryServiceDeclaration function */
    typedef struct fsciBleGattDbAppAddSecondaryServiceDeclarationOutParams_tag
    {
        uint16_t* pOutHandle;
    }fsciBleGattDbAppAddSecondaryServiceDeclarationOutParams_t;

    /* Structure used for keeping the out parameters pointers of the
    GattDbDynamic_AddIncludeDeclaration function */
    typedef struct fsciBleGattDbAppAddIncludeDeclarationOutParams_tag
    {
        uint16_t* pOutHandle;
    }fsciBleGattDbAppAddIncludeDeclarationOutParams_t;

    /* Structure used for keeping the out parameters pointers of the
    GattDbDynamic_AddCharacteristicDeclarationAndValue function */
    typedef struct fsciBleGattDbAppAddCharacteristicDeclarationAndValueOutParams_tag
    {
        uint16_t* pOutHandle;
    }fsciBleGattDbAppAddCharacteristicDeclarationAndValueOutParams_t;

    /* Structure used for keeping the out parameters pointers of the
    GattDbDynamic_AddCharDescriptor function */
    typedef struct fsciBleGattDbAppAddCharacteristicDescriptorOutParams_tag
    {
        uint16_t* pOutHandle;
    }fsciBleGattDbAppAddCharacteristicDescriptorOutParams_t;

    /* Structure used for keeping the out parameters pointers of the
    GattDbDynamic_AddCccd function */
    typedef struct fsciBleGattDbAppAddCccdOutParams_tag
    {
        uint16_t* pOutHandle;
    }fsciBleGattDbAppAddCccdOutParams_t;

    /* Structure used for keeping the out parameters pointers of the
    GattDbDynamic_AddCharDeclWithUniqueValue function */
    typedef struct fsciBleGattDbAppAddCharacteristicDeclarationWithUniqueValueOutParams_tag
    {
        uint16_t* pOutHandle;
    }fsciBleGattDbAppAddCharacteristicDeclarationWithUniqueValueOutParams_t;

    /* Structure used for keeping the out parameters pointers of the
    GattDbDynamic_AddCharacteristicDescriptorWithUniqueValue function */
    typedef struct fsciBleGattDbAppAddCharDescriptorWithUniqueValueOutParams_tag
    {
        uint16_t* pOutHandle;
    }fsciBleGattDbAppAddCharDescriptorWithUniqueValueOutParams_t;

#endif /* gFsciBleHost_d */

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

#if gFsciBleTest_d
    /* Indicates if FSCI for GATT Database (application) is enabled or not */
    static bool_t bFsciBleGattDbAppEnabled              = FALSE;

     /* Indicates if the command was initiated by FSCI (to be not monitored) */
    static bool_t bFsciBleGattDbAppCmdInitiatedByFsci   = FALSE;
#endif /* gFsciBleTest_d */

#if gFsciBleHost_d
    /* Keeps out parameters pointers for Host - BBox functionality */
    static fsciBleGattDbAppOutParams_t fsciBleGattDbAppOutParams = {NULL, NULL};
#endif /* gFsciBleHost_d */

/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

void fsciBleGattDbAppHandler(void* pData, void* pParam, uint32_t fsciInterfaceId)
{
    clientPacket_t* pClientPacket   = (clientPacket_t*)pData;
    uint8_t*        pBuffer         = &pClientPacket->structured.payload[0];

#if gFsciBleTest_d
    /* Mark this command as initiated by FSCI */
    bFsciBleGattDbAppCmdInitiatedByFsci = TRUE;

    /* Verify if the command is Mode Select */
    if(gBleGattDbAppModeSelectOpCode_c == pClientPacket->structured.header.opCode)
    {
        fsciBleGetBoolValueFromBuffer(bFsciBleGattDbAppEnabled, pBuffer);
        /* Return status */
        fsciBleGattDbAppStatusMonitor(gBleSuccess_c);
    }
    else
    {
        /* Verify if FSCI is enabled for this layer */
        if(TRUE == bFsciBleGattDbAppEnabled)
        {
#endif /* gFsciBleTest_d*/
           /* Select the GATT Database (application) function to be called (using the FSCI opcode) */
            switch(pClientPacket->structured.header.opCode)
            {
#if gFsciBleBBox_d || gFsciBleTest_d
                case (uint8_t)gBleGattDbAppCmdWriteAttributeOpCode_c:
                    {
                        uint16_t    handle;
                        uint16_t    valueLength;
                        uint8_t*    pValue;

                        /* Get command parameters from buffer */
                        fsciBleGetUint16ValueFromBuffer(handle, pBuffer);
                        fsciBleGetUint16ValueFromBuffer(valueLength, pBuffer);

                        /* Allocate buffer for the attribute value */
                        pValue = MEM_BufferAlloc(valueLength);

                        if((0U < valueLength) &&
                           (NULL == pValue))
                        {
                            /* No memory => The GATT Database (application) function can not be executed */
                            fsciBleError(gFsciOutOfMessages_c, fsciInterfaceId);
                        }
                        else
                        {
                            /* Get attribute value from buffer */
                            fsciBleGetArrayFromBuffer(pValue, pBuffer, valueLength);

                            fsciBleGattDbAppCallApiFunction(GattDb_WriteAttribute(handle, valueLength, pValue));

                            /* Free the buffer allocated for the attribute value */
                            (void)MEM_BufferFree(pValue);
                        }
                    }
                    break;

                case (uint8_t)gBleGattDbAppCmdReadAttributeOpCode_c:
                    {
                        uint16_t    handle;
                        uint16_t    maxBytes;
                        uint8_t*    pValue;
                        uint16_t    valueLength;

                        /* Get command parameters from buffer */
                        fsciBleGetUint16ValueFromBuffer(handle, pBuffer);
                        fsciBleGetUint16ValueFromBuffer(maxBytes, pBuffer);

                        /* Allocate buffer for the attribute value (consider that
                        maxBytes is bigger than 0) */
                        pValue = MEM_BufferAlloc(maxBytes);

                        if(NULL == pValue)
                        {
                            /* No memory => The GATT Database (application) function can not be executed */
                            fsciBleError(gFsciOutOfMessages_c, fsciInterfaceId);
                        }
                        else
                        {
                            fsciBleGattDbAppCallApiFunction(GattDb_ReadAttribute(handle, maxBytes, pValue, &valueLength));
                            fsciBleGattDbAppMonitorOutParams(ReadAttribute, pValue, &valueLength);

                            /* Free the buffer allocated for the attribute value */
                            (void)MEM_BufferFree(pValue);
                        }


                    }
                    break;

                case (uint8_t)gBleGattDbAppCmdFindServiceHandleOpCode_c:
                case (uint8_t)gBleGattDbAppCmdFindCharValueHandleInServiceOpCode_c:
                case (uint8_t)gBleGattDbAppCmdFindDescriptorHandleForCharValueHandleOpCode_c:
                    {
                        uint16_t        givenHandle;
                        bleUuidType_t   uuidType;
                        bleUuid_t       uuid;
                        uint16_t        requestedHandle;

                        /* Get command parameters from buffer */
                        fsciBleGetUint16ValueFromBuffer(givenHandle, pBuffer);
                        fsciBleGetEnumValueFromBuffer(uuidType, pBuffer, bleUuidType_t);
                        fsciBleGetUuidFromBuffer(&uuid, &pBuffer, uuidType);

                        if((uint8_t)gBleGattDbAppCmdFindServiceHandleOpCode_c == (uint8_t)pClientPacket->structured.header.opCode)
                        {
                            fsciBleGattDbAppCallApiFunction(GattDb_FindServiceHandle(givenHandle, uuidType, &uuid, &requestedHandle));
                            fsciBleGattDbAppMonitorOutParams(FindServiceHandle, &requestedHandle);
                        }
                        else if((uint8_t)gBleGattDbAppCmdFindCharValueHandleInServiceOpCode_c == (uint8_t)pClientPacket->structured.header.opCode)
                        {
                            fsciBleGattDbAppCallApiFunction(GattDb_FindCharValueHandleInService(givenHandle, uuidType, &uuid, &requestedHandle));
                            fsciBleGattDbAppMonitorOutParams(FindCharValueHandleInService, &requestedHandle);
                        }
                        else
                        {
                            fsciBleGattDbAppCallApiFunction(GattDb_FindDescriptorHandleForCharValueHandle(givenHandle, uuidType, &uuid, &requestedHandle));
                            fsciBleGattDbAppMonitorOutParams(FindDescriptorHandleForCharValueHandle, &requestedHandle);
                        }
                    }
                    break;

                case (uint8_t)gBleGattDbAppCmdFindCccdHandleForCharValueHandleOpCode_c:
                    {
                        uint16_t charValueHandle;
                        uint16_t cccdHandle;

                        /* Get command parameter from buffer */
                        fsciBleGetUint16ValueFromBuffer(charValueHandle, pBuffer);

                        fsciBleGattDbAppCallApiFunction(GattDb_FindCccdHandleForCharValueHandle(charValueHandle, &cccdHandle));
                        fsciBleGattDbAppMonitorOutParams(FindCccdHandleForCharValueHandle, &cccdHandle);
                    }
                    break;

            case (uint8_t)gBleGattDbAppCmdInitDatabaseOpCode_c:
                    {
                        fsciBleGattDbAppCallApiFunction(GattDbDynamic_Init());
                    }
                    break;

                case (uint8_t)gBleGattDbAppCmdReleaseDatabaseOpCode_c:
                    {
                        fsciBleGattDbAppCallApiFunction(GattDbDynamic_ReleaseDatabase());
                    }
                    break;

                case (uint8_t)gBleGattDbAppCmdAddPrimaryServiceDeclarationOpCode_c:
                    {
                        uint16_t        desiredHandle;
                        bleUuidType_t   serviceUuidType;
                        bleUuid_t       serviceUuid;
                        uint16_t        outHandle;

                        /* Get command parameters from buffer */
                        fsciBleGetUint16ValueFromBuffer(desiredHandle, pBuffer);
                        fsciBleGetEnumValueFromBuffer(serviceUuidType, pBuffer, bleUuidType_t);
                        fsciBleGetUuidFromBuffer(&serviceUuid, &pBuffer, serviceUuidType);

                        fsciBleGattDbAppCallApiFunction(GattDbDynamic_AddPrimaryServiceDeclaration(desiredHandle, serviceUuidType, &serviceUuid, &outHandle));
                        fsciBleGattDbAppMonitorOutParams(AddPrimaryServiceDeclaration, &outHandle);
                    }
                    break;

                case (uint8_t)gBleGattDbAppCmdAddSecondaryServiceDeclarationOpCode_c:
                    {
                        uint16_t        desiredHandle;
                        bleUuidType_t   serviceUuidType;
                        bleUuid_t       serviceUuid;
                        uint16_t        outHandle;

                        /* Get command parameters from buffer */
                        fsciBleGetUint16ValueFromBuffer(desiredHandle, pBuffer);
                        fsciBleGetEnumValueFromBuffer(serviceUuidType, pBuffer, bleUuidType_t);
                        fsciBleGetUuidFromBuffer(&serviceUuid, &pBuffer, serviceUuidType);

                        fsciBleGattDbAppCallApiFunction(GattDbDynamic_AddSecondaryServiceDeclaration(desiredHandle, serviceUuidType, &serviceUuid, &outHandle));
                        fsciBleGattDbAppMonitorOutParams(AddSecondaryServiceDeclaration, &outHandle);
                    }
                    break;

                case (uint8_t)gBleGattDbAppCmdAddIncludeDeclarationOpCode_c:
                    {
                        uint16_t        includedServiceHandle;
                        uint16_t        endGroupHandle;
                        bleUuidType_t   serviceUuidType;
                        bleUuid_t       serviceUuid;
                        uint16_t        outHandle;

                        /* Get command parameters from buffer */
                        fsciBleGetUint16ValueFromBuffer(includedServiceHandle, pBuffer);
                        fsciBleGetUint16ValueFromBuffer(endGroupHandle, pBuffer);
                        fsciBleGetEnumValueFromBuffer(serviceUuidType, pBuffer, bleUuidType_t);
                        fsciBleGetUuidFromBuffer(&serviceUuid, &pBuffer, serviceUuidType);

                        fsciBleGattDbAppCallApiFunction(GattDbDynamic_AddIncludeDeclaration(includedServiceHandle, endGroupHandle,
                                                                                            serviceUuidType, &serviceUuid, &outHandle));
                        fsciBleGattDbAppMonitorOutParams(AddIncludeDeclaration, &outHandle);

                    }
                    break;

                case (uint8_t)gBleGattDbAppCmdAddCharacteristicDeclarationAndValueOpCode_c:
                    {
                        bleUuidType_t                               characteristicUuidType;
                        bleUuid_t                                   characteristicUuid;
                        gattCharacteristicPropertiesBitFields_t     characteristicProperties;
                        uint16_t                                    maxValueLength;
                        uint16_t                                    initialValueLength;
                        uint8_t*                                    pInitialValue;
                        gattAttributePermissionsBitFields_t         valueAccessPermissions;
                        uint16_t                                    outHandle;

                        /* Get command parameters from buffer */
                        fsciBleGetEnumValueFromBuffer(characteristicUuidType, pBuffer, bleUuidType_t);
                        fsciBleGetUuidFromBuffer(&characteristicUuid, &pBuffer, characteristicUuidType);
                        fsciBleGetEnumValueFromBuffer(characteristicProperties, pBuffer, gattCharacteristicPropertiesBitFields_t);
                        fsciBleGetUint16ValueFromBuffer(maxValueLength, pBuffer);
                        fsciBleGetUint16ValueFromBuffer(initialValueLength, pBuffer);

                        /* Allocate memory buffer */
                        pInitialValue = MEM_BufferAlloc(initialValueLength);

                        if(NULL != pInitialValue)
                        {
                            /* Get pInitialValue and valueAccessPermissions from buffer */
                            fsciBleGetArrayFromBuffer(pInitialValue, pBuffer, initialValueLength);
                            fsciBleGetEnumValueFromBuffer(valueAccessPermissions, pBuffer, gattAttributePermissionsBitFields_t);

                            fsciBleGattDbAppCallApiFunction(GattDbDynamic_AddCharacteristicDeclarationAndValue(characteristicUuidType,
                                                                                                               &characteristicUuid,
                                                                                                               characteristicProperties,
                                                                                                               maxValueLength,
                                                                                                               initialValueLength,
                                                                                                               pInitialValue,
                                                                                                               valueAccessPermissions,
                                                                                                               &outHandle));
                            fsciBleGattDbAppMonitorOutParams(AddCharacteristicDeclarationAndValue, &outHandle);

                            (void)MEM_BufferFree(pInitialValue);
                        }
                        else
                        {
                            /* No memory - the Application GATT Database command can not be executed */
                            fsciBleError(gFsciOutOfMessages_c, fsciInterfaceId);
                        }
                    }
                    break;

                case (uint8_t)gBleGattDbAppCmdAddCharacteristicDescriptorOpCode_c:
                    {
                        bleUuidType_t                               descriptorUuidType;
                        bleUuid_t                                   descriptorUuid;
                        uint16_t                                    descriptorValueLength;
                        uint8_t*                                    pInitialValue;
                        gattAttributePermissionsBitFields_t         descriptorAccessPermissions;
                        uint16_t                                    outHandle;

                        /* Get command parameters from buffer */
                        fsciBleGetEnumValueFromBuffer(descriptorUuidType, pBuffer, bleUuidType_t);
                        fsciBleGetUuidFromBuffer(&descriptorUuid, &pBuffer, descriptorUuidType);
                        fsciBleGetUint16ValueFromBuffer(descriptorValueLength, pBuffer);

                        /* Allocate memory buffer */
                        pInitialValue = MEM_BufferAlloc(descriptorValueLength);

                        if(NULL != pInitialValue)
                        {
                            /* Get pInitialValue and descriptorAccessPermissions from buffer */
                            fsciBleGetArrayFromBuffer(pInitialValue, pBuffer, descriptorValueLength);
                            fsciBleGetEnumValueFromBuffer(descriptorAccessPermissions, pBuffer, gattAttributePermissionsBitFields_t);

                            fsciBleGattDbAppCallApiFunction(GattDbDynamic_AddCharDescriptor(descriptorUuidType,
                                                                                                      &descriptorUuid,
                                                                                                      descriptorValueLength,
                                                                                                      pInitialValue,
                                                                                                      descriptorAccessPermissions,
                                                                                                      &outHandle));
                            fsciBleGattDbAppMonitorOutParams(AddCharacteristicDescriptor, &outHandle);

                            (void)MEM_BufferFree(pInitialValue);
                        }
                        else
                        {
                            /* No memory - the Application GATT Database command can not be executed */
                            fsciBleError(gFsciOutOfMessages_c, fsciInterfaceId);
                        }
                    }
                    break;

                case (uint8_t)gBleGattDbAppCmdAddCccdOpCode_c:
                    {
                        uint16_t outHandle;

                        fsciBleGattDbAppCallApiFunction(GattDbDynamic_AddCccd(&outHandle));
                        fsciBleGattDbAppMonitorOutParams(AddCccd, &outHandle);
                    }
                    break;

                case (uint8_t)gBleGattDbAppCmdAddCharacteristicDeclarationWithUniqueValueOpCode_c:
                    {
                        bleUuidType_t                               characteristicUuidType;
                        bleUuid_t                                   characteristicUuid;
                        gattCharacteristicPropertiesBitFields_t     characteristicProperties;
                        gattAttributePermissionsBitFields_t         valueAccessPermissions;
                        uint16_t                                    outHandle;

                        /* Get command parameters from buffer */
                        fsciBleGetEnumValueFromBuffer(characteristicUuidType, pBuffer, bleUuidType_t);
                        fsciBleGetUuidFromBuffer(&characteristicUuid, &pBuffer, characteristicUuidType);
                        fsciBleGetEnumValueFromBuffer(characteristicProperties, pBuffer, gattCharacteristicPropertiesBitFields_t);
                        fsciBleGetEnumValueFromBuffer(valueAccessPermissions, pBuffer, gattAttributePermissionsBitFields_t);

                        fsciBleGattDbAppCallApiFunction(GattDbDynamic_AddCharDeclWithUniqueValue(characteristicUuidType,
                                                                                                                  &characteristicUuid,
                                                                                                                  characteristicProperties,
                                                                                                                  valueAccessPermissions,
                                                                                                                  &outHandle));
                        fsciBleGattDbAppMonitorOutParams(AddCharacteristicDeclarationWithUniqueValue, &outHandle);
                    }
                    break;

                case (uint8_t)gBleGattDbAppCmdRemoveServiceOpCode_c:
                    {
                        uint16_t serviceHandle;

                        /* Get command parameters from buffer */
                        fsciBleGetUint16ValueFromBuffer(serviceHandle, pBuffer);

                        fsciBleGattDbAppCallApiFunction(GattDbDynamic_RemoveService(serviceHandle));
                    }
                    break;

                case (uint8_t)gBleGattDbAppCmdRemoveCharacteristicOpCode_c:
                    {
                        uint16_t characteristicHandle;

                        /* Get command parameters from buffer */
                        fsciBleGetUint16ValueFromBuffer(characteristicHandle, pBuffer);

                        fsciBleGattDbAppCallApiFunction(GattDbDynamic_RemoveCharacteristic(characteristicHandle));
                    }
                    break;

                case (uint8_t)gBleGattDbAppCmdAddCharDescriptorWithUniqueValueOpCode_c:
                    {
                        bleUuidType_t                               descriptorUuidType;
                        bleUuid_t                                   descriptorUuid;
                        gattAttributePermissionsBitFields_t         descriptorAccessPermissions;
                        uint16_t                                    outHandle;

                        /* Get command parameters from buffer */
                        fsciBleGetEnumValueFromBuffer(descriptorUuidType, pBuffer, bleUuidType_t);
                        fsciBleGetUuidFromBuffer(&descriptorUuid, &pBuffer, descriptorUuidType);
                        fsciBleGetEnumValueFromBuffer(descriptorAccessPermissions, pBuffer, gattAttributePermissionsBitFields_t);

                        fsciBleGattDbAppCallApiFunction(GattDbDynamic_AddCharDescriptorWithUniqueValue(descriptorUuidType,
                                                                             &descriptorUuid,
                                                                             descriptorAccessPermissions,
                                                                             &outHandle));
                        fsciBleGattDbAppMonitorOutParams(AddCharDescriptorWithUniqueValue, &outHandle);
                    }
                    break;

#endif /* gFsciBleBBox_d || gFsciBleTest_d */

#if gFsciBleHost_d
                case gBleGattDbAppStatusOpCode_c:
                    {
                        bleResult_t status;

                        fsciBleGetEnumValueFromBuffer(status, pBuffer, bleResult_t);

                        if(gBleSuccess_c != status)
                        {
                            /* Clean out parameters pointers kept in FSCI */
                            fsciBleGattDbAppCleanOutParams();
                        }
                    }
                    break;

                case gBleGattDbAppEvtReadAttributeValueOpCode_c:
                    {
                        fsciBleGattDbAppReadAttributeOutParams_t* pOutParams = (fsciBleGattDbAppReadAttributeOutParams_t*)fsciBleGattDbAppRestoreOutParams();

                        /* Get out parameters of the GattDb_ReadAttribute function from buffer */
                        if(NULL != pOutParams->pValueLength)
                        {
                            fsciBleGetUint16ValueFromBuffer(*pOutParams->pValueLength, pBuffer);
                        }

                        if(NULL != pOutParams->pValue)
                        {
                            fsciBleGetArrayFromBuffer(pOutParams->pValue, pBuffer, *pOutParams->pValueLength);
                        }

                        if((NULL != pOutParams->pValueLength) ||
                           (NULL != pOutParams->pValue))
                        {
                            /* Clean out parameters pointers kept in FSCI */
                            fsciBleGattDbAppCleanOutParams();

                            /* Signal out parameters ready */
                            Ble_OutParamsReady();
                        }
                    }
                    break;

                case gBleGattDbAppEvtFindServiceHandleOpCode_c:
                    {
                        fsciBleGattDbAppFindServiceHandleOutParams_t* pOutParams = (fsciBleGattDbAppFindServiceHandleOutParams_t*)fsciBleGattDbAppRestoreOutParams();

                        if(NULL != pOutParams->pOutServiceHandle)
                        {
                            /* Get out parameters of the GattDb_FindServiceHandle function from buffer */
                            fsciBleGetUint16ValueFromBuffer(*pOutParams->pOutServiceHandle, pBuffer);

                            /* Clean out parameters pointers kept in FSCI */
                            fsciBleGattDbAppCleanOutParams();

                            /* Signal out parameter ready */
                            Ble_OutParamsReady();
                        }
                    }
                    break;

                case gBleGattDbAppEvtFindCharValueHandleInServiceOpCode_c:
                    {
                        fsciBleGattDbAppFindCharValueHandleInServiceOutParams_t* pOutParams = (fsciBleGattDbAppFindCharValueHandleInServiceOutParams_t*)fsciBleGattDbAppRestoreOutParams();

                        if(NULL != pOutParams->pCharValueHandle)
                        {
                            /* Get out parameters of the GattDb_FindCharValueHandleInService function from buffer */
                            fsciBleGetUint16ValueFromBuffer(*pOutParams->pCharValueHandle, pBuffer);

                            /* Clean out parameters pointers kept in FSCI */
                            fsciBleGattDbAppCleanOutParams();

                            /* Signal out parameter ready */
                            Ble_OutParamsReady();
                        }
                    }
                    break;

                case gBleGattDbAppEvtFindCccdHandleForCharValueHandleOpCode_c:
                    {
                        fsciBleGattDbAppFindCccdHandleForCharValueHandleOutParams_t* pOutParams = (fsciBleGattDbAppFindCccdHandleForCharValueHandleOutParams_t*)fsciBleGattDbAppRestoreOutParams();

                        if(NULL != pOutParams->pCccdHandle)
                        {
                            /* Get out parameters of the GattDb_FindCccdHandleForCharValueHandle function from buffer */
                            fsciBleGetUint16ValueFromBuffer(*pOutParams->pCccdHandle, pBuffer);

                            /* Clean out parameters pointers kept in FSCI */
                            fsciBleGattDbAppCleanOutParams();

                            /* Signal out parameter ready */
                            Ble_OutParamsReady();
                        }
                    }
                    break;

                case gBleGattDbAppEvtFindDescriptorHandleForCharValueHandleOpCode_c:
                    {
                        fsciBleGattDbAppFindDescriptorHandleForCharValueHandleOutParams_t* pOutParams = (fsciBleGattDbAppFindDescriptorHandleForCharValueHandleOutParams_t*)fsciBleGattDbAppRestoreOutParams();

                        if(NULL != pOutParams->pDescriptorHandle)
                        {
                            /* Get out parameters of the GattDb_FindDescriptorHandleForCharValueHandle function from buffer */
                            fsciBleGetUint16ValueFromBuffer(*pOutParams->pDescriptorHandle, pBuffer);

                            /* Clean out parameters pointers kept in FSCI */
                            fsciBleGattDbAppCleanOutParams();

                            /* Signal out parameter ready */
                            Ble_OutParamsReady();
                        }
                    }
                    break;

                case gBleGattDbAppEvtAddPrimaryServiceDeclarationOpCode_c:
                    {
                        fsciBleGattDbAppAddPrimaryServiceDeclarationOutParams_t* pOutParams = (fsciBleGattDbAppAddPrimaryServiceDeclarationOutParams_t*)fsciBleGattDbAppRestoreOutParams();

                        if(NULL != pOutParams->pOutHandle)
                        {
                            /* Get out parameters of the GattDbDynamic_AddPrimaryServiceDeclaration function from buffer */
                            fsciBleGetUint16ValueFromBuffer(*pOutParams->pOutHandle, pBuffer);

                            /* Clean out parameters pointers kept in FSCI */
                            fsciBleGattDbAppCleanOutParams();
                        }

                        /* Signal out parameter ready */
                        Ble_OutParamsReady();
                    }
                    break;

                case gBleGattDbAppEvtAddSecondaryServiceDeclarationOpCode_c:
                    {
                        fsciBleGattDbAppAddSecondaryServiceDeclarationOutParams_t* pOutParams = (fsciBleGattDbAppAddSecondaryServiceDeclarationOutParams_t*)fsciBleGattDbAppRestoreOutParams();

                        if(NULL != pOutParams->pOutHandle)
                        {
                            /* Get out parameters of the GattDbDynamic_AddSecondaryServiceDeclaration function from buffer */
                            fsciBleGetUint16ValueFromBuffer(*pOutParams->pOutHandle, pBuffer);

                            /* Clean out parameters pointers kept in FSCI */
                            fsciBleGattDbAppCleanOutParams();
                        }

                        /* Signal out parameter ready */
                        Ble_OutParamsReady();
                    }
                    break;

                case gBleGattDbAppEvtAddIncludeDeclarationOpCode_c:
                    {
                        fsciBleGattDbAppAddIncludeDeclarationOutParams_t* pOutParams = (fsciBleGattDbAppAddIncludeDeclarationOutParams_t*)fsciBleGattDbAppRestoreOutParams();

                        if(NULL != pOutParams->pOutHandle)
                        {
                            /* Get out parameters of the GattDbDynamic_AddIncludeDeclaration function from buffer */
                            fsciBleGetUint16ValueFromBuffer(*pOutParams->pOutHandle, pBuffer);

                            /* Clean out parameters pointers kept in FSCI */
                            fsciBleGattDbAppCleanOutParams();
                        }

                        /* Signal out parameter ready */
                        Ble_OutParamsReady();
                    }
                    break;

                case gBleGattDbAppEvtAddCharacteristicDeclarationAndValueOpCode_c:
                    {
                        fsciBleGattDbAppAddCharacteristicDeclarationAndValueOutParams_t* pOutParams = (fsciBleGattDbAppAddCharacteristicDeclarationAndValueOutParams_t*)fsciBleGattDbAppRestoreOutParams();

                        if(NULL != pOutParams->pOutHandle)
                        {
                            /* Get out parameters of the GattDbDynamic_AddCharacteristicDeclarationAndValue function from buffer */
                            fsciBleGetUint16ValueFromBuffer(*pOutParams->pOutHandle, pBuffer);

                            /* Clean out parameters pointers kept in FSCI */
                            fsciBleGattDbAppCleanOutParams();
                        }

                        /* Signal out parameter ready */
                        Ble_OutParamsReady();
                    }
                    break;

                case gBleGattDbAppEvtAddCharacteristicDescriptorOpCode_c:
                    {
                        fsciBleGattDbAppAddCharacteristicDescriptorOutParams_t* pOutParams = (fsciBleGattDbAppAddCharacteristicDescriptorOutParams_t*)fsciBleGattDbAppRestoreOutParams();

                        if(NULL != pOutParams->pOutHandle)
                        {
                            /* Get out parameters of the GattDbDynamic_AddCharDescriptor function from buffer */
                            fsciBleGetUint16ValueFromBuffer(*pOutParams->pOutHandle, pBuffer);

                            /* Clean out parameters pointers kept in FSCI */
                            fsciBleGattDbAppCleanOutParams();
                        }

                        /* Signal out parameter ready */
                        Ble_OutParamsReady();
                    }
                    break;

                case gBleGattDbAppEvtAddCccdOpCode_c:
                    {
                        fsciBleGattDbAppAddCccdOutParams_t* pOutParams = (fsciBleGattDbAppAddCccdOutParams_t*)fsciBleGattDbAppRestoreOutParams();

                        if(NULL != pOutParams->pOutHandle)
                        {
                            /* Get out parameters of the GattDbDynamic_AddCccd function from buffer */
                            fsciBleGetUint16ValueFromBuffer(*pOutParams->pOutHandle, pBuffer);

                            /* Clean out parameters pointers kept in FSCI */
                            fsciBleGattDbAppCleanOutParams();
                        }

                        /* Signal out parameter ready */
                        Ble_OutParamsReady();
                    }
                    break;

                case gBleGattDbAppEvtAddCharacteristicDeclarationWithUniqueValueOpCode_c:
                    {
                        fsciBleGattDbAppAddCharacteristicDeclarationWithUniqueValueOutParams_t* pOutParams = (fsciBleGattDbAppAddCharacteristicDeclarationWithUniqueValueOutParams_t*)fsciBleGattDbAppRestoreOutParams();

                        if(NULL != pOutParams->pOutHandle)
                        {
                            /* Get out parameters of the GattDbDynamic_AddCharDeclWithUniqueValue function from buffer */
                            fsciBleGetUint16ValueFromBuffer(*pOutParams->pOutHandle, pBuffer);

                            /* Clean out parameters pointers kept in FSCI */
                            fsciBleGattDbAppCleanOutParams();
                        }

                        /* Signal out parameter ready */
                        Ble_OutParamsReady();
                    }
                    break;

                case gBleGattDbAppEvtAddCharDescriptorWithUniqueValueOpCode_c:
                    {
                        fsciBleGattDbAppAddCharDescriptorValueOutParams_t* pOutParams = (fsciBleGattDbAppAddCharDescriptorWithUniqueValueOutParams_t*)fsciBleGattDbAppRestoreOutParams();

                        if(NULL != pOutParams->pOutHandle)
                        {
                            /* Get out parameters of the GattDbDynamic_AddCharacteristicDeclarationWithUniqueValue function from buffer */
                            fsciBleGetUint16ValueFromBuffer(*pOutParams->pOutHandle, pBuffer);

                            /* Clean out parameters pointers kept in FSCI */
                            fsciBleGattDbAppCleanOutParams();
						}

                        /* Signal out parameter ready */
                        Ble_OutParamsReady();
                    }
                    break;

#endif /* gFsciBleHost_d */

                default:
                    {
                        /* Unknown FSCI opcode */
                        fsciBleError(gFsciUnknownOpcode_c, fsciInterfaceId);
                    }
                    break;
            }
#if gFsciBleTest_d
        }
        else
        {
            /* FSCI SAP disabled */
            fsciBleError(gFsciSAPDisabled_c, fsciInterfaceId);
        }
    }

    /* Mark the next command as not initiated by FSCI */
    bFsciBleGattDbAppCmdInitiatedByFsci = FALSE;
#endif /* gFsciBleTest_d */

    (void)MEM_BufferFree(pData);
}


#if gFsciBleBBox_d || gFsciBleTest_d

void fsciBleGattDbAppStatusMonitor(bleResult_t result)
{
#if gFsciBleTest_d
    /* If GATT Database (application) is disabled the status must be not monitored */
    if(FALSE == bFsciBleGattDbAppEnabled)
    {
        return;
    }
#endif /* gFsciBleTest_d */

    fsciBleStatusMonitor(gFsciBleGattDbAppOpcodeGroup_c, (uint8_t)gBleGattDbAppStatusOpCode_c, result);
}

#endif /* gFsciBleBBox_d || gFsciBleTest_d */


#if gFsciBleHost_d || gFsciBleTest_d

void fsciBleGattDbAppNoParamCmdMonitor(fsciBleGattDbAppOpCode_t opCode)
{
#if gFsciBleTest_d
    /* If GATT Database (application) is disabled or if the command was initiated by FSCI it
    must be not monitored */
    if((FALSE == bFsciBleGattDbAppEnabled) ||
       (TRUE == bFsciBleGattDbAppCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Call the generic FSCI BLE monitor for commands or events that have no parameters */
    fsciBleNoParamCmdOrEvtMonitor(gFsciBleGattDbAppOpcodeGroup_c, opCode);
}


void fsciBleGattDbAppUint16ParamCmdMonitor(fsciBleGattDbAppOpCode_t opCode, uint16_t value)
{
#if gFsciBleTest_d
    /* If GATT Database (application) is disabled or if the command was initiated by FSCI it
    must be not monitored */
    if((FALSE == bFsciBleGattDbAppEnabled) ||
       (TRUE == bFsciBleGattDbAppCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Call the generic FSCI BLE monitor for commands or events that have an uint16_t parameters */
    fsciBleGattDbAppUint16ParamEvtMonitor(opCode, &value);
}


void fsciBleGattDbAppUuidTypeAndUuidParamCmdMonitor(fsciBleGattDbAppOpCode_t opCode, uint16_t desiredHandle, bleUuidType_t uuidType, const bleUuid_t* pUuid, uint16_t* pOutHandle)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If Application GATT Database is disabled or if the command was initiated by
    FSCI it must be not monitored */
    if((FALSE == bFsciBleGattDbAppEnabled) ||
       (TRUE == bFsciBleGattDbAppCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGattDbAppAllocFsciPacket(opCode,
                                                    sizeof(uint16_t) +
                                                    sizeof(bleUuidType_t) +
                                                    fsciBleGetUuidBufferSize(uuidType));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromUint16Value(desiredHandle, pBuffer);
    fsciBleGetBufferFromEnumValue(uuidType, pBuffer, bleUuidType_t);
    fsciBleGetBufferFromUuid(pUuid, &pBuffer, uuidType);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);

#if gFsciBleHost_d
    /* Save out parameters pointers */
    fsciBleGattDbAppSaveOutParams(pOutHandle, NULL);
#endif /* gFsciBleHost_d */
}


void fsciBleGattDbAppWriteAttributeCmdMonitor(uint16_t handle, uint16_t valueLength, const uint8_t* aValue)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GATT Database (application) is disabled or if the command was initiated by FSCI it
    must be not monitored */
    if((FALSE == bFsciBleGattDbAppEnabled) ||
       (TRUE == bFsciBleGattDbAppCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGattDbAppAllocFsciPacket(gBleGattDbAppCmdWriteAttributeOpCode_c, sizeof(uint16_t) + sizeof(uint16_t) + valueLength);

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromUint16Value(handle, pBuffer);
    fsciBleGetBufferFromUint16Value(valueLength, pBuffer);
    fsciBleGetBufferFromArray(aValue, pBuffer, valueLength);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGattDbAppReadAttributeCmdMonitor(uint16_t handle, uint16_t maxBytes, uint8_t* aOutValue, uint16_t*pOutValueLength)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GATT Database (application) is disabled or if the command was initiated by FSCI it
    must be not monitored */
    if((FALSE == bFsciBleGattDbAppEnabled) ||
       (TRUE == bFsciBleGattDbAppCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGattDbAppAllocFsciPacket(gBleGattDbAppCmdReadAttributeOpCode_c, sizeof(uint16_t) + sizeof(uint16_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromUint16Value(handle, pBuffer);
    fsciBleGetBufferFromUint16Value(maxBytes, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);

#if gFsciBleHost_d
    /* Save out parameters pointers */
    fsciBleGattDbAppSaveOutParams(aOutValue, pOutValueLength);
#endif /* gFsciBleHost_d */
}


void fsciBleGattDbAppFindCccdHandleForCharValueHandleCmdMonitor(uint16_t charValueHandle, uint16_t* pOutCccdHandle)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GATT Database (application) is disabled or if the command was initiated by FSCI it
    must be not monitored */
    if((FALSE == bFsciBleGattDbAppEnabled) ||
       (TRUE == bFsciBleGattDbAppCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGattDbAppAllocFsciPacket(gBleGattDbAppCmdFindCccdHandleForCharValueHandleOpCode_c, sizeof(uint16_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromUint16Value(charValueHandle, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);

#if gFsciBleHost_d
    /* Save out parameters pointers */
    fsciBleGattDbAppSaveOutParams(pOutCccdHandle, NULL);
#endif /* gFsciBleHost_d */
}


void fsciBleGattDbAppFindServiceCharValueOrDescriptorHandleCmdMonitor(fsciBleGattDbAppOpCode_t opCode, uint16_t handle, bleUuidType_t uuidType, const bleUuid_t* pUuid, uint16_t* pOutHandle)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GATT Database (application) is disabled or if the command was initiated by FSCI it
    must be not monitored */
    if((FALSE == bFsciBleGattDbAppEnabled) ||
       (TRUE == bFsciBleGattDbAppCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGattDbAppAllocFsciPacket(opCode,
                                                 sizeof(uint16_t) + sizeof(bleUuidType_t) +
                                                 fsciBleGetUuidBufferSize(uuidType));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromUint16Value(handle, pBuffer);
    fsciBleGetBufferFromEnumValue(uuidType, pBuffer, bleUuidType_t);
    fsciBleGetBufferFromUuid(pUuid, &pBuffer, uuidType);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);

#if gFsciBleHost_d
    /* Save out parameters pointers */
    fsciBleGattDbAppSaveOutParams(pOutHandle, NULL);
#endif /* gFsciBleHost_d */
}


void fsciBleGattDbAppAddIncludeDeclarationCmdMonitor(uint16_t includedServiceHandle, uint16_t endGroupHandle, bleUuidType_t serviceUuidType, const bleUuid_t* pServiceUuid, uint16_t* pOutHandle)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If Application GATT Database is disabled or if the command was initiated by
    FSCI it must be not monitored */
    if((FALSE == bFsciBleGattDbAppEnabled) ||
       (TRUE == bFsciBleGattDbAppCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGattDbAppAllocFsciPacket(gBleGattDbAppCmdAddIncludeDeclarationOpCode_c,
                                                    sizeof(uint16_t) + sizeof(uint16_t) +
                                                    sizeof(bleUuidType_t) +
                                                    fsciBleGetUuidBufferSize(serviceUuidType));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromUint16Value(includedServiceHandle, pBuffer);
    fsciBleGetBufferFromUint16Value(endGroupHandle, pBuffer);
    fsciBleGetBufferFromEnumValue(serviceUuidType, pBuffer, bleUuidType_t);
    fsciBleGetBufferFromUuid(pServiceUuid, &pBuffer, serviceUuidType);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);

#if gFsciBleHost_d
    /* Save out parameters pointers */
    fsciBleGattDbAppSaveOutParams(pOutHandle, NULL);
#endif /* gFsciBleHost_d */
}


void fsciBleGattDbAppAddCharacteristicDeclarationAndValueCmdMonitor(bleUuidType_t characteristicUuidType, const bleUuid_t* pCharacteristicUuid, gattCharacteristicPropertiesBitFields_t characteristicProperties, uint16_t maxValueLength, uint16_t initialValueLength, const uint8_t* aInitialValue, gattAttributePermissionsBitFields_t valueAccessPermissions, uint16_t* pOutHandle)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If Application GATT Database is disabled or if the command was initiated by
    FSCI it must be not monitored */
    if((FALSE == bFsciBleGattDbAppEnabled) ||
       (TRUE == bFsciBleGattDbAppCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGattDbAppAllocFsciPacket(gBleGattDbAppCmdAddCharacteristicDeclarationAndValueOpCode_c,
                                                    sizeof(bleUuidType_t) +
                                                    fsciBleGetUuidBufferSize(characteristicUuidType) +
                                                    sizeof(gattCharacteristicPropertiesBitFields_t) +
                                                    sizeof(uint16_t) + sizeof(uint16_t) + initialValueLength +
                                                    sizeof(gattAttributePermissionsBitFields_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromEnumValue(characteristicUuidType, pBuffer, bleUuidType_t);
    fsciBleGetBufferFromUuid(pCharacteristicUuid, &pBuffer, characteristicUuidType);
    fsciBleGetBufferFromEnumValue(characteristicProperties, pBuffer, gattCharacteristicPropertiesBitFields_t);
    fsciBleGetBufferFromUint16Value(maxValueLength, pBuffer);
    fsciBleGetBufferFromUint16Value(initialValueLength, pBuffer);
    fsciBleGetBufferFromArray(aInitialValue, pBuffer, initialValueLength);
    fsciBleGetBufferFromEnumValue(valueAccessPermissions, pBuffer, gattAttributePermissionsBitFields_t);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);

#if gFsciBleHost_d
    /* Save out parameters pointers */
    fsciBleGattDbAppSaveOutParams(pOutHandle, NULL);
#endif /* gFsciBleHost_d */
}


void fsciBleGattDbAppAddCharDeclWithUniqueValCmdMonitor(bleUuidType_t characteristicUuidType, const bleUuid_t* pCharacteristicUuid, gattCharacteristicPropertiesBitFields_t characteristicProperties, gattAttributePermissionsBitFields_t valueAccessPermissions, uint16_t* pOutHandle)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If Application GATT Database is disabled or if the command was initiated by
    FSCI it must be not monitored */
    if((FALSE == bFsciBleGattDbAppEnabled) ||
       (TRUE == bFsciBleGattDbAppCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGattDbAppAllocFsciPacket(gBleGattDbAppCmdAddCharacteristicDeclarationWithUniqueValueOpCode_c,
                                                    sizeof(bleUuidType_t) +
                                                    fsciBleGetUuidBufferSize(characteristicUuidType) +
                                                    sizeof(gattCharacteristicPropertiesBitFields_t) +
                                                    sizeof(gattAttributePermissionsBitFields_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromEnumValue(characteristicUuidType, pBuffer, bleUuidType_t);
    fsciBleGetBufferFromUuid(pCharacteristicUuid, &pBuffer, characteristicUuidType);
    fsciBleGetBufferFromEnumValue(characteristicProperties, pBuffer, gattCharacteristicPropertiesBitFields_t);
    fsciBleGetBufferFromEnumValue(valueAccessPermissions, pBuffer, gattAttributePermissionsBitFields_t);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);

#if gFsciBleHost_d
    /* Save out parameters pointers */
    fsciBleGattDbAppSaveOutParams(pOutHandle, NULL);
#endif /* gFsciBleHost_d */
}


void fsciBleGattDbAppAddCharDescrCmdMonitor( bleUuidType_t descriptorUuidType, const bleUuid_t* pDescriptorUuid, uint16_t descriptorValueLength, const uint8_t* aInitialValue, gattAttributePermissionsBitFields_t descriptorAccessPermissions, uint16_t* pOutHandle)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If Application GATT Database is disabled or if the command was initiated by
    FSCI it must be not monitored */
    if((FALSE == bFsciBleGattDbAppEnabled) ||
       (TRUE == bFsciBleGattDbAppCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGattDbAppAllocFsciPacket(gBleGattDbAppCmdAddCharacteristicDescriptorOpCode_c,
                                                    sizeof(bleUuidType_t) +
                                                    fsciBleGetUuidBufferSize(descriptorUuidType) +
                                                    sizeof(uint16_t) + descriptorValueLength +
                                                    sizeof(gattAttributePermissionsBitFields_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromEnumValue(descriptorUuidType, pBuffer, bleUuidType_t);
    fsciBleGetBufferFromUuid(pDescriptorUuid, &pBuffer, descriptorUuidType);
    fsciBleGetBufferFromUint16Value(descriptorValueLength, pBuffer);
    fsciBleGetBufferFromArray(aInitialValue, pBuffer, descriptorValueLength);
    fsciBleGetBufferFromEnumValue(descriptorAccessPermissions, pBuffer, gattAttributePermissionsBitFields_t);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);

#if gFsciBleHost_d
    /* Save out parameters pointers */
    fsciBleGattDbAppSaveOutParams(pOutHandle, NULL);
#endif /* gFsciBleHost_d */
}


void fsciBleGattDbAppAddCccdCmdMonitor(uint16_t* pOutHandle)
{
    fsciBleGattDbAppNoParamCmdMonitor(gBleGattDbAppCmdAddCccdOpCode_c);

#if gFsciBleHost_d
    /* Save out parameters pointers */
    fsciBleGattDbAppSaveOutParams(pOutHandle, NULL);
#endif /* gFsciBleHost_d */
}

void fsciBleGattDbAppAddCharDescriptorWithUniqueValueCmdMonitor( bleUuidType_t descriptorUuidType,
                                                                 bleUuid_t* pDescriptorUuid,
                                                                 gattAttributePermissionsBitFields_t descriptorAccessPermissions,
                                                                 uint16_t* pOutHandle)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If Application GATT Database is disabled or if the command was initiated by
    FSCI it must be not monitored */
    if((FALSE == bFsciBleGattDbAppEnabled) ||
       (TRUE == bFsciBleGattDbAppCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGattDbAppAllocFsciPacket(gBleGattDbAppCmdAddCharDescriptorWithUniqueValueOpCode_c,
                                                    sizeof(bleUuidType_t) +
                                                    fsciBleGetUuidBufferSize(descriptorUuidType) +
                                                    sizeof(gattAttributePermissionsBitFields_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromEnumValue(descriptorUuidType, pBuffer, bleUuidType_t);
    fsciBleGetBufferFromUuid(pDescriptorUuid, &pBuffer, descriptorUuidType);
    fsciBleGetBufferFromEnumValue(descriptorAccessPermissions, pBuffer, gattAttributePermissionsBitFields_t);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);

#if gFsciBleHost_d
    /* Save out parameters pointers */
    fsciBleGattDbAppSaveOutParams(pOutHandle, NULL);
#endif /* gFsciBleHost_d */
}

#endif /* gFsciBleHost_d || gFsciBleTest_d */


#if gFsciBleBBox_d || gFsciBleTest_d


void fsciBleGattDbAppReadAttributeEvtMonitor(uint8_t* aOutValue, uint16_t* pOutValueLength)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GATT Database (application) is disabled the event must be not monitored */
    if(FALSE == bFsciBleGattDbAppEnabled)
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGattDbAppAllocFsciPacket((uint8_t)gBleGattDbAppEvtReadAttributeValueOpCode_c,
                                                 sizeof(uint16_t) + *pOutValueLength);

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set event parameters in the buffer */
    fsciBleGetBufferFromUint16Value(*pOutValueLength, pBuffer);
    fsciBleGetBufferFromArray(aOutValue, pBuffer, *pOutValueLength);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGattDbAppUint16ParamEvtMonitor(fsciBleGattDbAppOpCode_t opCode, const uint16_t* pValue)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GATT Database (application) is disabled the event must be not monitored */
    if(FALSE == bFsciBleGattDbAppEnabled)
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGattDbAppAllocFsciPacket((uint8_t)opCode, sizeof(uint16_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set event parameters in the buffer */
    fsciBleGetBufferFromUint16Value(*pValue, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}

#endif /* gFsciBleBBox_d || gFsciBleTest_d */

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

#endif /* gFsciIncluded_c && gFsciBleGattDbAppLayerEnabled_d */


/*! *********************************************************************************
* @}
********************************************************************************** */
