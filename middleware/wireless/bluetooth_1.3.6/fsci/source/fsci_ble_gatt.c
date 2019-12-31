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

#include "fsci_ble_gatt.h"

#if gFsciBleHost_d
    #include "host_ble.h"
#endif /* gFsciBleHost_d */


#if gFsciIncluded_c && gFsciBleGattLayerEnabled_d

/************************************************************************************
*************************************************************************************
* Private constants & macros
*************************************************************************************
************************************************************************************/

#if gFsciBleHost_d
    /* Macro used for saving the out parameters pointers of the GATT functions */
    #define fsciBleGattSaveOutParams(pOutParam) \
            fsciBleGattOutParams.pParam = (uint8_t*)pOutParam

    /* Macro used for restoring the out parameters pointers of the GATT functions */
    #define fsciBleGattRestoreOutParams()   \
            &fsciBleGattOutParams

    /* Macro used for setting the out parameters pointers of the GATT
    functions to NULL */
    #define fsciBleGattCleanOutParams() \
            fsciBleGattOutParams.pParam = NULL
#endif /* gFsciBleHost_d */


#if gFsciBleBBox_d
    #define bFsciBleGattClientAllocatedInfo             TRUE
#endif /* gFsciBleBBox_d */

#define fsciBleGattCallApiFunction(apiFunction)         bleResult_t result = (apiFunction); \
                                                        fsciBleGattStatusMonitor(result)
#define fsciBleGattMonitorOutParams(functionId, ...)    if(gBleSuccess_c == result)                  \
                                                        {                                            \
                                                            FsciEvtMonitor(functionId, __VA_ARGS__); \
                                                        }

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

/*! Structure that keeps callbacks registered by application or FSCI in GATT. */
typedef struct fsciBleGattCallbacks_tag
{
    gattClientProcedureCallback_t       clientProcedureCallback;    /* GATT Client procedure callback. */
    gattClientNotificationCallback_t    clientNotificationCallback; /* GATT Client notification callback. */
    gattClientIndicationCallback_t      clientIndicationCallback;   /* GATT Client indication callback. */
    gattServerCallback_t                serverCallback;             /* GATT Server callback. */
}fsciBleGattCallbacks_t;

#if gFsciBleHost_d
    /* Structure used for keeping the out parameters pointers of the GATT
     functions */
    typedef struct fsciBleGattOutParams_tag
    {
        uint8_t*    pParam;
    }fsciBleGattOutParams_t;

    /* Structure used for keeping the out parameters pointers of the
    Gatt_GetMtu function */
    typedef struct fsciBleGattGetMtuOutParams_tag
    {
        uint16_t* pMtu;
    }fsciBleGattGetMtuOutParams_t;
#endif /* gFsciBleHost_d */

/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/

void fsciBleGattClientProcedureCallback(deviceId_t deviceId, gattProcedureType_t procedureType, gattProcedureResult_t procedureResult, bleResult_t error);
void fsciBleGattClientNotificationCallback(deviceId_t deviceId, uint16_t characteristicValueHandle, uint8_t* aValue, uint16_t valueLength);
void fsciBleGattClientIndicationCallback(deviceId_t deviceId, uint16_t characteristicValueHandle, uint8_t* aValue, uint16_t valueLength);

void fsciBleGattServerCallback(deviceId_t deviceId, gattServerEvent_t* pServerEvent);

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

/* GATT callbacks structure initialized with FSCI empty static functions */
static fsciBleGattCallbacks_t fsciBleGattCallbacks =
{
    fsciBleGattClientProcedureCallback,
    fsciBleGattClientNotificationCallback,
    fsciBleGattClientIndicationCallback,
    fsciBleGattServerCallback
};

/* Keeps the number of characteristics to be read by GattClient_ReadMultipleCharacteristicValues
function */
static uint8_t fsciBleGattClientNbOfCharacteristics = 0;

#if gFsciBleTest_d
    /* Indicates if FSCI for GATT is enabled or not */
    static bool_t bFsciBleGattEnabled               = FALSE;

    /* Indicates if the command was initiated by FSCI (to be not monitored) */
    static bool_t bFsciBleGattCmdInitiatedByFsci    = FALSE;

    /* Indicates if the client information was allocated by FSCI or just saved */
    static bool_t bFsciBleGattClientAllocatedInfo   = FALSE;
#endif /* gFsciBleTest_d */

#if gFsciBleHost_d
    /* Keeps out parameters pointers for Host - BBox functionality */
    static fsciBleGattOutParams_t fsciBleGattOutParams = {NULL};
#endif /* gFsciBleHost_d */

#if gFsciBleBBox_d || gFsciBleTest_d
    /* Flag that indicates if the last request was a GATT Client request with
    out parameters (this procedures are asynchronous) */
    static bool_t bGattClientRequestWithOutParameters = FALSE;
#endif /* gFsciBleBBox_d || gFsciBleTest_d */

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

void fsciBleSetGattClientProcedureCallback(gattClientProcedureCallback_t clientProcedureCallback)
{
    /* Set GATT Client procedure callback to an application desired function, only
    if not NULL. Otherwise set it to the FSCI empty static function */
    fsciBleGattCallbacks.clientProcedureCallback  = (NULL != clientProcedureCallback) ?
                                                    clientProcedureCallback :
                                                    fsciBleGattClientProcedureCallback;
}


void fsciBleSetGattClientNotificationCallback(gattClientNotificationCallback_t clientNotificationCallback)
{
    /* Set GATT Client notification callback to an application desired function, only
    if not NULL. Otherwise set it to the FSCI empty static function */
    fsciBleGattCallbacks.clientNotificationCallback  = (NULL != clientNotificationCallback) ?
                                                       clientNotificationCallback :
                                                       fsciBleGattClientNotificationCallback;
}


void fsciBleSetGattClientIndicationCallback(gattClientIndicationCallback_t clientIndicationCallback)
{
    /* Set GATT Client indication callback to an application desired function, only
    if not NULL. Otherwise set it to the FSCI empty static function */
    fsciBleGattCallbacks.clientIndicationCallback  = (NULL != clientIndicationCallback) ?
                                                     clientIndicationCallback :
                                                     fsciBleGattClientIndicationCallback;
}


void fsciBleSetGattServerCallback(gattServerCallback_t serverCallback)
{
    /* Set GATT Server callback to an application desired function, only
    if not NULL. Otherwise set it to the FSCI empty static function */
    fsciBleGattCallbacks.serverCallback  = (NULL != serverCallback) ?
                                                    serverCallback :
                                                    fsciBleGattServerCallback;
}


void fsciBleGattHandler(void* pData, void* pParam, uint32_t fsciInterfaceId)
{
    clientPacket_t* pClientPacket   = (clientPacket_t*)pData;
    uint8_t*        pBuffer         = &pClientPacket->structured.payload[0];

#if gFsciBleTest_d
    /* Mark this command as initiated by FSCI */
    bFsciBleGattCmdInitiatedByFsci = TRUE;

    /* Verify if the command is Mode Select */
    if(gBleGattModeSelectOpCode_c == pClientPacket->structured.header.opCode)
    {
        fsciBleGetBoolValueFromBuffer(bFsciBleGattEnabled, pBuffer);
        /* Return status */
        fsciBleGattStatusMonitor(gBleSuccess_c);
    }
    else
    {
        /* Verify if FSCI is enabled for this layer */
        if(TRUE == bFsciBleGattEnabled)
        {
#endif /* gFsciBleTest_d */
           /* Select the GATT, GATT Client or GATT Server function to be called (using the FSCI opcode) */
            switch(pClientPacket->structured.header.opCode)
            {
#if gFsciBleBBox_d || gFsciBleTest_d
               /* GATT */
                case (uint8_t)gBleGattCmdInitOpCode_c:
                    {
                        fsciBleGattCallApiFunction(Gatt_Init());
                    }
                    break;

                case (uint8_t)gBleGattCmdGetMtuOpCode_c:
                    {
                        deviceId_t  deviceId;
                        uint16_t    mtu;

                        /* Get deviceId from the received packet */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);

                        fsciBleGattCallApiFunction(Gatt_GetMtu(deviceId, &mtu));
                        fsciBleGattMonitorOutParams(GetMtu, &mtu);

                    }
                    break;

                /* GATT Client */
                case (uint8_t)gBleGattCmdClientInitOpCode_c:
                    {
                        fsciBleGattCallApiFunction(GattClient_Init());
                    }
                    break;

                case (uint8_t)gBleGattCmdClientResetProceduresOpCode_c:
                    {
                        fsciBleGattCallApiFunction(GattClient_ResetProcedure());
                    }
                    break;

                case (uint8_t)gBleGattCmdClientRegisterProcedureCallbackOpCode_c:
                    {
                        fsciBleGattCallApiFunction(GattClient_RegisterProcedureCallback(fsciBleGattCallbacks.clientProcedureCallback));
                    }
                    break;

                case (uint8_t)gBleGattCmdClientRegisterNotificationCallbackOpCode_c:
                    {
                        fsciBleGattCallApiFunction(GattClient_RegisterNotificationCallback(fsciBleGattCallbacks.clientNotificationCallback));
                    }
                    break;

                case (uint8_t)gBleGattCmdClientRegisterIndicationCallbackOpCode_c:
                    {
                        fsciBleGattCallApiFunction(GattClient_RegisterIndicationCallback(fsciBleGattCallbacks.clientIndicationCallback));
                    }
                    break;

                case (uint8_t)gBleGattCmdClientExchangeMtuOpCode_c:
                    {
                        deviceId_t  deviceId;

                        /* Get deviceId parameter from the received packet */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);

                        fsciBleGattCallApiFunction(GattClient_ExchangeMtu(deviceId));
                    }
                    break;

                case (uint8_t)gBleGattCmdClientDiscoverAllPrimaryServicesOpCode_c:
                    {
                        deviceId_t      deviceId;
                        uint8_t         maxServiceCount;
                        gattService_t*  pOutServices;
                        uint8_t*        pOutServiceCount;

                        /* Get function parameters from the received packet */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);
                        fsciBleGetUint8ValueFromBuffer(maxServiceCount, pBuffer);

                        /* Allocate buffer for the services array - consider that maxServiceCount
                         is bigger than 0 */
                        pOutServices        = fsciBleGattClientAllocOutOrIoServices(maxServiceCount);
                        /* Allocate buffer for the services array count */
                        pOutServiceCount    = (uint8_t*)fsciBleGattClientAllocOutOrIoArraySize();

                        if((NULL != pOutServices) &&
                           (NULL != pOutServiceCount))
                        {
                            /* Set flag to indicate that the last GATT request was a Client
                            request with out parameters */
                            bGattClientRequestWithOutParameters = TRUE;

                            fsciBleGattCallApiFunction(GattClient_DiscoverAllPrimaryServices(deviceId, pOutServices,
                                                                                             maxServiceCount,
                                                                                             pOutServiceCount));
                        }
                    }
                    break;

                case (uint8_t)gBleGattCmdClientDiscoverPrimaryServicesByUuidOpCode_c:
                    {
                        deviceId_t      deviceId;
                        bleUuidType_t   uuidType;
                        bleUuid_t       uuid;
                        uint8_t         maxServiceCount;
                        gattService_t*  pOutServices;
                        uint8_t*        pOutServiceCount;

                        /* Get function parameters from the received packet */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);
                        fsciBleGetEnumValueFromBuffer(uuidType, pBuffer, bleUuidType_t);
                        fsciBleGetUuidFromBuffer(&uuid, &pBuffer, uuidType);
                        fsciBleGetUint8ValueFromBuffer(maxServiceCount, pBuffer);

                        /* Allocate buffer for the services array - consider that maxServiceCount
                        is bigger than 0 */
                        pOutServices        = fsciBleGattClientAllocOutOrIoServices(maxServiceCount);
                        /* Allocate buffer for the services array count */
                        pOutServiceCount    = (uint8_t*)fsciBleGattClientAllocOutOrIoArraySize();

                        if((NULL != pOutServices) ||
                           (NULL != pOutServiceCount))
                        {
                            /* Set flag to indicate that the last GATT request was a Client
                            request with out parameters */
                            bGattClientRequestWithOutParameters = TRUE;

                            fsciBleGattCallApiFunction(GattClient_DiscoverPrimaryServicesByUuid(deviceId, uuidType, &uuid,
                                                                                                pOutServices,
                                                                                                maxServiceCount,
                                                                                                pOutServiceCount));
                        }
                    }
                    break;

                case (uint8_t)gBleGattCmdClientFindIncludedServicesOpCode_c:
                    {
                        deviceId_t      deviceId;
                        gattService_t*  pIoService;

                        /* Get deviceId from the received packet */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);

                        /* Allocate buffer for the service */
                        pIoService = fsciBleGattClientAllocOutOrIoServices(1);

                        if(NULL != pIoService)
                        {
                            uint8_t         maxIncludedServiceCount;
                            gattService_t*  pOutIncludedServices;

                            /* Get more parameters from the received packet */
                            fsciBleGattClientGetServiceFromBuffer(pIoService, &pBuffer);
                            fsciBleGetUint8ValueFromBuffer(maxIncludedServiceCount, pBuffer);

                            /* Allocate buffer for the included services - consider that maxIncludedServiceCount
                            is bigger than 0 */
                            pOutIncludedServices = fsciBleGattClientAllocOutOrIoIncludedServices(maxIncludedServiceCount);

                            if(NULL != pOutIncludedServices)
                            {
                                pIoService->aIncludedServices = pOutIncludedServices;

                                /* Set flag to indicate that the last GATT request was a Client
                                request with out parameters */
                                bGattClientRequestWithOutParameters = TRUE;

                                fsciBleGattCallApiFunction(GattClient_FindIncludedServices(deviceId, pIoService,
                                                                                           maxIncludedServiceCount));
                            }
                        }
                    }
                    break;

                case (uint8_t)gBleGattCmdClientDiscoverAllCharacteristicsOfServiceOpCode_c:
                    {
                        deviceId_t      deviceId;
                        gattService_t*  pIoService;

                        /* Get deviceId from the received packet */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);

                        /* Allocate buffer for the service */
                        pIoService = fsciBleGattClientAllocOutOrIoServices(1);

                        if(NULL != pIoService)
                        {
                            uint8_t                 maxCharacteristicCount;
                            gattCharacteristic_t*   pOutCharacteristics;

                            /* Get more parameters from the received packet */
                            fsciBleGattClientGetServiceFromBuffer(pIoService, &pBuffer);
                            fsciBleGetUint8ValueFromBuffer(maxCharacteristicCount, pBuffer);

                            /* Allocate buffer for the characteristics - consider that maxCharacteristicCount
                            is bigger than 0 */
                            pOutCharacteristics = fsciBleGattClientAllocOutOrIoCharacteristics(maxCharacteristicCount);

                            if(NULL != pOutCharacteristics)
                            {
                                pIoService->aCharacteristics = pOutCharacteristics;

                                /* Set flag to indicate that the last GATT request was a Client
                                request with out parameters */
                                bGattClientRequestWithOutParameters = TRUE;

                                fsciBleGattCallApiFunction(GattClient_DiscoverAllCharacteristicsOfService(deviceId, pIoService,
                                                                                                          maxCharacteristicCount));
                            }
                        }
                    }
                    break;

                case (uint8_t)gBleGattCmdClientDiscoverCharacteristicOfServiceByUuidOpCode_c:
                    {
                        deviceId_t              deviceId;
                        bleUuidType_t           uuidType;
                        bleUuid_t               uuid;
                        gattService_t           service = {0};
                        uint8_t                 maxCharacteristicCount;
                        gattCharacteristic_t*   pOutCharacteristics;
                        uint8_t*                pOutCharacteristicCount;


                        /* Get function parameters from the received packet */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);
                        fsciBleGetEnumValueFromBuffer(uuidType, pBuffer, bleUuidType_t);
                        fsciBleGetUuidFromBuffer(&uuid, &pBuffer, uuidType);
                        fsciBleGattClientGetServiceFromBuffer(&service, &pBuffer);
                        fsciBleGetUint8ValueFromBuffer(maxCharacteristicCount, pBuffer);

                        /* Allocate buffer for the characteristics - consider that maxCharacteristicCount
                        is bigger than 0 */
                        pOutCharacteristics     = fsciBleGattClientAllocOutOrIoCharacteristics(maxCharacteristicCount);
                        /* Allocate buffer for the characteristics array count */
                        pOutCharacteristicCount = (uint8_t*)fsciBleGattClientAllocOutOrIoArraySize();

                        if((NULL != pOutCharacteristics) &&
                           (NULL != pOutCharacteristicCount))
                        {
                            /* Set flag to indicate that the last GATT request was a Client
                            request with out parameters */
                            bGattClientRequestWithOutParameters = TRUE;

                            fsciBleGattCallApiFunction(GattClient_DiscoverCharacteristicOfServiceByUuid(deviceId, uuidType, &uuid,
                                                                                                        &service, pOutCharacteristics,
                                                                                                        maxCharacteristicCount,
                                                                                                        pOutCharacteristicCount));
                        }
                    }
                    break;

                case (uint8_t)gBleGattCmdClientDiscoverAllCharacteristicDescriptorsOpCode_c:
                    {
                        deviceId_t              deviceId;
                        gattCharacteristic_t*   pIoCharacteristic;

                        /* Get deviceId from the received packet */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);

                        /* Allocate buffer for the characteristic */
                        pIoCharacteristic = fsciBleGattClientAllocOutOrIoCharacteristics(1);

                        if(NULL != pIoCharacteristic)
                        {
                            uint16_t            endingHandle;
                            uint8_t             maxDescriptorCount;
                            gattAttribute_t*    pOutDescriptors;

                            /* Get more parameters from the received packet */
                            fsciBleGattClientGetCharFromBuffer(pIoCharacteristic, &pBuffer);
                            fsciBleGetUint16ValueFromBuffer(endingHandle, pBuffer);
                            fsciBleGetUint8ValueFromBuffer(maxDescriptorCount, pBuffer);

                            /* Allocate buffer for the descriptors - consider that maxDescriptorCount
                            is bigger than 0 */
                            pOutDescriptors = fsciBleGattClientAllocOutOrIoAttributes(maxDescriptorCount);

                            if(NULL != pOutDescriptors)
                            {
                                pIoCharacteristic->aDescriptors = pOutDescriptors;

                                /* Set flag to indicate that the last GATT request was a Client
                                request with out parameters */
                                bGattClientRequestWithOutParameters = TRUE;

                                fsciBleGattCallApiFunction(GattClient_DiscoverAllCharacteristicDescriptors(deviceId, pIoCharacteristic,
                                                                                                           endingHandle,
                                                                                                           maxDescriptorCount));
                            }
                        }
                    }
                    break;

                case (uint8_t)gBleGattCmdClientReadCharacteristicValueOpCode_c:
                    {
                        deviceId_t              deviceId;
                        gattCharacteristic_t*   pIoCharacteristic;

                        /* Get deviceId from the received packet */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);

                        /* Allocate buffer for the characteristic */
                        pIoCharacteristic = fsciBleGattClientAllocOutOrIoCharacteristics(1);

                        if(NULL != pIoCharacteristic)
                        {
                            uint16_t    maxValueLength;
                            uint8_t*    pOutValue;

                            /* Get more parameters from the received packet */
                            fsciBleGattClientGetCharFromBuffer(pIoCharacteristic, &pBuffer);
                            fsciBleGetUint16ValueFromBuffer(maxValueLength, pBuffer);

                            /* Allocate buffer for the descriptors - consider that maxValueLength
                            is bigger than 0 */
                            pOutValue = fsciBleGattClientAllocOutOrIoValue(maxValueLength);

                            if(NULL != pOutValue)
                            {
                                pIoCharacteristic->value.paValue = pOutValue;

                                /* Set flag to indicate that the last GATT request was a Client
                                request with out parameters */
                                bGattClientRequestWithOutParameters = TRUE;

                                fsciBleGattCallApiFunction(GattClient_ReadCharacteristicValue(deviceId,
                                                                                              pIoCharacteristic,
                                                                                              maxValueLength));
                            }
                        }
                    }
                    break;

                case (uint8_t)gBleGattCmdClientReadUsingCharacteristicUuidOpCode_c:
                    {
                        deviceId_t          deviceId;
                        bleUuidType_t       uuidType;
                        bleUuid_t           uuid;
                        bool_t              bHandleRangeIncluded;
                        gattHandleRange_t   handleRange;
                        gattHandleRange_t*  pHandleRange = NULL;
                        uint16_t            maxValueLength;
                        uint8_t*            pOutValue;
                        uint16_t*           pOutValueLength;

                        /* Get function parameters from the received packet */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);
                        fsciBleGetEnumValueFromBuffer(uuidType, pBuffer, bleUuidType_t);
                        fsciBleGetUuidFromBuffer(&uuid, &pBuffer, uuidType);
                        fsciBleGetBoolValueFromBuffer(bHandleRangeIncluded, pBuffer);

                        if(TRUE == bHandleRangeIncluded)
                        {
                            fsciBleGattGetHandleRangeFromBuffer(&handleRange, &pBuffer);
                            pHandleRange = &handleRange;
                        }

                        fsciBleGetUint16ValueFromBuffer(maxValueLength, pBuffer);

                        /* Allocate buffer for the value - consider that maxValueLength
                        is bigger than 0 */
                        pOutValue       = fsciBleGattClientAllocOutOrIoValue(maxValueLength);
                        /* Allocate buffer for the value length */
                        pOutValueLength = fsciBleGattClientAllocOutOrIoArraySize();

                        if((NULL != pOutValue) &&
                           (NULL != pOutValueLength))
                        {
                            /* Set flag to indicate that the last GATT request was a Client
                            request with out parameters */
                            bGattClientRequestWithOutParameters = TRUE;

                            fsciBleGattCallApiFunction(GattClient_ReadUsingCharacteristicUuid(deviceId, uuidType, &uuid,
                                                                                              pHandleRange, pOutValue,
                                                                                              maxValueLength, pOutValueLength));
                        }
                    }
                    break;

                case (uint8_t)gBleGattCmdClientReadMultipleCharacteristicValuesOpCode_c:
                    {
                        deviceId_t              deviceId;
                        gattCharacteristic_t*   pIoCharacteristics;

                        /* Get function parameters from the received packet */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);
                        fsciBleGetUint8ValueFromBuffer(fsciBleGattClientNbOfCharacteristics, pBuffer);

                        /* Allocate buffer for the characteristics - considering that fsciBleGattClientNbOfCharacteristics
                        is bigger than 0 */
                        pIoCharacteristics = fsciBleGattClientAllocOutOrIoCharacteristics(fsciBleGattClientNbOfCharacteristics);

                        if(NULL != pIoCharacteristics)
                        {
                            uint16_t    maxValueLength = 0;
                            uint8_t*    pOutValue;
                            uint8_t     iCount;

                            /* Get all characteristics from the received packet */
                            for(iCount = 0; iCount < fsciBleGattClientNbOfCharacteristics; iCount++)
                            {
                                fsciBleGattClientGetCharFromBuffer(&pIoCharacteristics[iCount], &pBuffer);

                                /* Compute the total length of all the characteristics values */
                                maxValueLength += pIoCharacteristics[iCount].value.maxValueLength;
                            }

                            /* Allocate a single buffer for all the characteristics values - considering
                            that maxValueLength is bigger than 0 */
                            pOutValue = fsciBleGattClientAllocOutOrIoValue(maxValueLength);

                            if(NULL != pOutValue)
                            {
                                /* Set paValue field for all the characteristics to the corresponding position in the
                                value buffer */
                                for(iCount = 0; iCount < fsciBleGattClientNbOfCharacteristics; iCount++)
                                {
                                    pIoCharacteristics[iCount].value.paValue = pOutValue;

                                    pOutValue += pIoCharacteristics[iCount].value.maxValueLength;
                                }

                                /* Set flag to indicate that the last GATT request was a Client
                                request with out parameters */
                                bGattClientRequestWithOutParameters = TRUE;

                                fsciBleGattCallApiFunction(GattClient_ReadMultipleCharacteristicValues(deviceId,
                                                                                                       fsciBleGattClientNbOfCharacteristics,
                                                                                                       pIoCharacteristics));
                            }
                        }
                    }
                    break;

                case (uint8_t)gBleGattCmdClientWriteCharacteristicValueOpCode_c:
                    {
                        deviceId_t              deviceId;
                        gattCharacteristic_t    characteristic;
                        uint16_t                valueLength;
                        uint8_t*                pValue;

                        characteristic.aDescriptors = NULL;

                        /* Get function parameters from the received packet */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);
                        fsciBleGattClientGetCharFromBuffer(&characteristic, &pBuffer);
                        fsciBleGetUint16ValueFromBuffer(valueLength, pBuffer);

                        /* Allocate buffer for the value */
                        pValue = MEM_BufferAlloc(valueLength);

                        if((0U == valueLength) ||
                           (NULL != pValue))
                        {
                            bool_t      bWithoutResponse;
                            bool_t      bSignedWrite;
                            bool_t      bDoReliableLongCharWrites;
                            uint8_t     csrk[gcSmpCsrkSize_c];

                            /* Get more parameters from the received packet */
                            fsciBleGetArrayFromBuffer(pValue, pBuffer, valueLength);
                            fsciBleGetBoolValueFromBuffer(bWithoutResponse, pBuffer);
                            fsciBleGetBoolValueFromBuffer(bSignedWrite, pBuffer);
                            fsciBleGetBoolValueFromBuffer(bDoReliableLongCharWrites, pBuffer);
                            fsciBleGetCsrkFromBuffer(csrk, pBuffer);

                            fsciBleGattCallApiFunction(GattClient_WriteCharacteristicValue(deviceId, &characteristic,
                                                                                           valueLength, pValue,
                                                                                           bWithoutResponse, bSignedWrite,
                                                                                           bDoReliableLongCharWrites,
                                                                                           csrk));

                            (void)MEM_BufferFree(pValue);
                        }
                        else
                        {
                            fsciBleError(gFsciOutOfMessages_c, fsciInterfaceId);
                        }
                    }
                    break;

                case (uint8_t)gBleGattCmdClientReadCharacteristicDescriptorsOpCode_c:
                    {
                        deviceId_t          deviceId;
                        gattAttribute_t*    pIoDescriptor;

                        /* Get deviceId from the received packet */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);

                        /* Allocate buffer for the descriptor */
                        pIoDescriptor = fsciBleGattClientAllocOutOrIoAttributes(1);

                        if(NULL != pIoDescriptor)
                        {
                            uint16_t    maxValueLength;
                            uint8_t*    pOutValue;

                            /* Get more parameters from the received packet */
                            fsciBleGattClientGetAttributeFromBuffer(pIoDescriptor, &pBuffer);
                            fsciBleGetUint16ValueFromBuffer(maxValueLength, pBuffer);

                            /* Allocate buffer for the value - considering that maxValueLength
                            is bigger than 0 */
                            pOutValue = fsciBleGattClientAllocOutOrIoValue(maxValueLength);

                            if(NULL != pOutValue)
                            {
                                pIoDescriptor->paValue = pOutValue;

                                /* Set flag to indicate that the last GATT request was a Client
                                request with out parameters */
                                bGattClientRequestWithOutParameters = TRUE;

                                fsciBleGattCallApiFunction(GattClient_ReadCharacteristicDescriptor(deviceId,
                                                                                                   pIoDescriptor,
                                                                                                   maxValueLength));
                            }
                        }
                     }
                    break;

                case (uint8_t)gBleGattCmdClientWriteCharacteristicDescriptorsOpCode_c:
                    {
                        deviceId_t      deviceId;
                        gattAttribute_t descriptor;
                        uint16_t        valueLength;
                        uint8_t*        pValue;

                        descriptor.paValue = NULL;

                        /* Get function parameters from the received packet */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);
                        fsciBleGattClientGetAttributeFromBuffer(&descriptor, &pBuffer);
                        fsciBleGetUint16ValueFromBuffer(valueLength, pBuffer);

                        /* Allocate buffer for the value */
                        pValue = MEM_BufferAlloc(valueLength);

                        if((0U == valueLength) ||
                           (NULL != pValue))
                        {
                            /* Get value from the received packet */
                            fsciBleGetArrayFromBuffer(pValue, pBuffer, valueLength);

                            fsciBleGattCallApiFunction(GattClient_WriteCharacteristicDescriptor(deviceId, &descriptor,
                                                                                                valueLength, pValue));

                            (void)MEM_BufferFree(pValue);
                        }
                        else
                        {
                            fsciBleError(gFsciOutOfMessages_c, fsciInterfaceId);
                        }
                    }
                    break;

                /* GATT Server */
                case (uint8_t)gBleGattCmdServerInitOpCode_c:
                    {
                        fsciBleGattCallApiFunction(GattServer_Init());
                    }
                    break;

                case (uint8_t)gBleGattCmdServerRegisterCallbackOpCode_c:
                    {
                        fsciBleGattCallApiFunction(GattServer_RegisterCallback(fsciBleGattCallbacks.serverCallback));
                    }
                    break;

                case (uint8_t)gBleGattCmdServerRegisterHandlesForWriteNotificationsOpCode_c:
                case (uint8_t)gBleGattCmdServerRegisterHandlesForReadNotificationsOpCode_c:
                case (uint8_t)gBleGattCmdServerUnregisterHandlesForWriteNotificationsOpCode_c:
                case (uint8_t)gBleGattCmdServerUnregisterHandlesForReadNotificationsOpCode_c:
                    {
                        uint8_t     handleCount;
                        uint16_t*   pAttributeHandles;

                        /* Read handleCount parameter from buffer */
                        fsciBleGetUint8ValueFromBuffer(handleCount, pBuffer);

                        /* Allocate buffer for pAttributeHandles */
                        pAttributeHandles = (uint16_t*)MEM_BufferAlloc((uint32_t)handleCount * sizeof(uint16_t));

                        if(NULL == pAttributeHandles)
                        {
                            fsciBleError(gFsciOutOfMessages_c, fsciInterfaceId);
                        }
                        else
                        {
                            /* Get pAttributeHandles parameter from the packet */
                            fsciBleGetArrayFromBuffer(pAttributeHandles, pBuffer, (uint32_t)handleCount * sizeof(uint16_t));

                            switch(pClientPacket->structured.header.opCode)
                            {
                                case (uint8_t)gBleGattCmdServerRegisterHandlesForWriteNotificationsOpCode_c:
                                    {
                                        fsciBleGattCallApiFunction(GattServer_RegisterHandlesForWriteNotifications(handleCount, pAttributeHandles));
                                    }
                                    break;
                                case (uint8_t)gBleGattCmdServerRegisterHandlesForReadNotificationsOpCode_c:
                                    {
                                        fsciBleGattCallApiFunction(GattServer_RegisterHandlesForReadNotifications(handleCount, pAttributeHandles));
                                    }
                                    break;
                                case (uint8_t)gBleGattCmdServerUnregisterHandlesForWriteNotificationsOpCode_c:
                                    {
                                        fsciBleGattCallApiFunction(GattServer_UnregisterHandlesForWriteNotifications(handleCount, pAttributeHandles));
                                    }
                                    break;
                                case (uint8_t)gBleGattCmdServerUnregisterHandlesForReadNotificationsOpCode_c:
                                    {
                                        fsciBleGattCallApiFunction(GattServer_UnregisterHandlesForReadNotifications(handleCount, pAttributeHandles));
                                    }
                                    break;
                                default:
                                    ; /* For MISRA compliance */
                                    break;
                            }

                            /* Free the buffer used for pAttributeHandles */
                            (void)MEM_BufferFree(pAttributeHandles);
                        }
                    }
                    break;

                case (uint8_t)gBleGattCmdServerSendAttributeWrittenStatusOpCode_c:
                case (uint8_t)gBleGattCmdServerSendAttributeReadStatusOpCode_c:
                    {
                        deviceId_t  deviceId;
                        uint16_t    attributeHandle;
                        uint8_t     status;

                        /* Get function parameters from the received packet */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);
                        fsciBleGetUint16ValueFromBuffer(attributeHandle, pBuffer);
                        fsciBleGetUint8ValueFromBuffer(status, pBuffer);

                        if((uint8_t)gBleGattCmdServerSendAttributeWrittenStatusOpCode_c == pClientPacket->structured.header.opCode)
                        {
                            fsciBleGattCallApiFunction(GattServer_SendAttributeWrittenStatus(deviceId, attributeHandle, status));
                        }
                        else
                        {
                            fsciBleGattCallApiFunction(GattServer_SendAttributeReadStatus(deviceId, attributeHandle, status));
                        }
                    }
                    break;

                case (uint8_t)gBleGattCmdServerSendNotificationOpCode_c:
                    {
                        deviceId_t  deviceId;
                        uint16_t    handle;

                        /* Get deviceId and handle parameters from the received packet */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);
                        fsciBleGetUint16ValueFromBuffer(handle, pBuffer);

                        fsciBleGattCallApiFunction(GattServer_SendNotification(deviceId, handle));
                    }
                    break;

                case (uint8_t)gBleGattCmdServerSendIndicationOpCode_c:
                    {
                        deviceId_t  deviceId;
                        uint16_t    handle;

                        /* Get deviceId and handle parameters from the received packet */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);
                        fsciBleGetUint16ValueFromBuffer(handle, pBuffer);

                        fsciBleGattCallApiFunction(GattServer_SendIndication(deviceId, handle));
                    }
                    break;

                case (uint8_t)gBleGattCmdServerSendInstantValueNotificationOpCode_c:
                case (uint8_t)gBleGattCmdServerSendInstantValueIndicationOpCode_c:
                    {
                        deviceId_t  deviceId;
                        uint16_t    handle;
                        uint16_t    valueLength;
                        uint8_t*    pValue;

                        /* Get deviceId, handle and valueLength parameters from the received packet */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);
                        fsciBleGetUint16ValueFromBuffer(handle, pBuffer);
                        fsciBleGetUint16ValueFromBuffer(valueLength, pBuffer);

                        /* Allocate buffer for pValue */
                        pValue = MEM_BufferAlloc(valueLength);

                        if((0U < valueLength) &&
                           (NULL == pValue))
                        {
                            fsciBleError(gFsciOutOfMessages_c, fsciInterfaceId);
                        }
                        else
                        {
                            /* Get pValue parameter from the packet */
                            fsciBleGetArrayFromBuffer(pValue, pBuffer, valueLength);

                            if(gBleGattCmdServerSendInstantValueNotificationOpCode_c == (fsciBleGattOpCode_t)pClientPacket->structured.header.opCode)
                            {
                                fsciBleGattCallApiFunction(GattServer_SendInstantValueNotification(deviceId, handle, valueLength, pValue));
                            }
                            else
                            {
                                fsciBleGattCallApiFunction(GattServer_SendInstantValueIndication(deviceId, handle, valueLength, pValue));
                            }

                            /* Free the buffer used for pValue */
                            (void)MEM_BufferFree(pValue);
                        }
                    }
                    break;

                    case (uint8_t)gBleGattCmdServerRegisterUniqueHandlesForNotificationsOpCode_c:
                    {
                        bool_t bWrite;
                        bool_t bRead;

                        fsciBleGetBoolValueFromBuffer(bWrite, pBuffer);
                        fsciBleGetBoolValueFromBuffer(bRead, pBuffer);

                        fsciBleGattCallApiFunction(GattServer_RegisterUniqueHandlesForNotifications(bWrite, bRead));
                    }
                    break;
#endif /* gFsciBleBBox_d || gFsciBleTest_d */

#if gFsciBleHost_d
                case gBleGattStatusOpCode_c:
                    {
                        bleResult_t status;

                        /* Get status from buffer */
                        fsciBleGetEnumValueFromBuffer(status, pBuffer, bleResult_t);

                        if(gBleSuccess_c != status)
                        {
                            /* Clean out parameters pointers kept in FSCI (only for synchronous function) */
                            fsciBleGattCleanOutParams();

                            /* Erase the information kept (for asynchronous functions) */
                            fsciBleGattClientEraseInfo(FALSE);
                        }
                    }
                    break;

                case gBleGattEvtGetMtuOpCode_c:
                    {
                        fsciBleGattGetMtuOutParams_t* pOutParams = (fsciBleGattGetMtuOutParams_t*)fsciBleGattRestoreOutParams();

                        if(NULL != pOutParams->pMtu)
                        {
                            /* Get out parameter of the Gatt_GetMtu function from buffer */
                            fsciBleGetUint16ValueFromBuffer(*pOutParams->pMtu, pBuffer);

                            /* Clean out parameters pointers kept in FSCI */
                            fsciBleGattCleanOutParams();

                            /* Signal out parameters ready */
                            Ble_OutParamsReady();
                        }
                    }
                    break;

                case gBleGattEvtClientProcedureExchangeMtuOpCode_c:
                case gBleGattEvtClientProcedureDiscoverAllPrimaryServicesOpCode_c:
                case gBleGattEvtClientProcedureDiscoverPrimaryServicesByUuidOpCode_c:
                case gBleGattEvtClientProcedureFindIncludedServicesOpCode_c:
                case gBleGattEvtClientProcedureDiscoverAllCharacteristicsOpCode_c:
                case gBleGattEvtClientProcedureDiscoverCharacteristicByUuidOpCode_c:
                case gBleGattEvtClientProcedureDiscoverAllCharacteristicDescriptorsOpCode_c:
                case gBleGattEvtClientProcedureReadCharacteristicValueOpCode_c:
                case gBleGattEvtClientProcedureReadUsingCharacteristicUuidOpCode_c:
                case gBleGattEvtClientProcedureReadMultipleCharacteristicValuesOpCode_c:
                case gBleGattEvtClientProcedureWriteCharacteristicValueOpCode_c:
                case gBleGattEvtClientProcedureReadCharacteristicDescriptorOpCode_c:
                case gBleGattEvtClientProcedureWriteCharacteristicDescriptorOpCode_c:
                    {
                        deviceId_t                  deviceId;
                        gattProcedureType_t         procedureType = gGattProcExchangeMtu_c;
                        gattProcedureResult_t       procedureResult;
                        bleResult_t                 error;
                        /* Procedure completed - get the kept information pointers */
                        gattService_t*              pServices           = NULL;
                        gattCharacteristic_t*       pCharacteristics    = NULL;
                        gattAttribute_t*            pDescriptors        = NULL;
                        uint8_t*                    pValue              = NULL;
                        uint16_t*                   pArraySize          = NULL;
                        uint32_t                    iCount;

                        /* Get client procedure callback parameters (from buffer) */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);
                        fsciBleGetEnumValueFromBuffer(procedureResult, pBuffer, gattProcedureResult_t);
                        fsciBleGetEnumValueFromBuffer(error, pBuffer, bleResult_t);

                        pServices           = fsciBleGattClientGetServicesInfo(deviceId, FALSE);
                        pCharacteristics    = fsciBleGattClientGetCharInfo(deviceId, FALSE);
                        pDescriptors        = fsciBleGattClientGetDescriptorsInfo(deviceId, FALSE);
                        pValue              = fsciBleGattClientGetValueInfo(deviceId, FALSE);
                        pArraySize          = fsciBleGattClientGetArraySizeInfo(deviceId, FALSE);

                        /* Get procedure type (from opCode) and variable length data) */
                        switch(pClientPacket->structured.header.opCode)
                        {
                            case gBleGattEvtClientProcedureExchangeMtuOpCode_c:
                                {
                                    procedureType = gGattProcExchangeMtu_c;
                                }
                                break;

                            case gBleGattEvtClientProcedureDiscoverAllPrimaryServicesOpCode_c:
                                {
                                    procedureType = gGattProcDiscoverAllPrimaryServices_c;

                                    if(gGattProcSuccess_c == procedureResult)
                                    {
                                        /* Get services array size parameter from buffer */
                                        fsciBleGetUint8ValueFromBuffer(*(uint8_t*)pArraySize, pBuffer);
                                        /* Get services array parameter from buffer */
                                        for(iCount = 0; iCount < *(uint8_t*)pArraySize; iCount++)
                                        {
                                            fsciBleGattClientGetServiceFromBuffer(&pServices[iCount], &pBuffer);
                                        }
                                    }
                                }
                                break;

                            case gBleGattEvtClientProcedureDiscoverPrimaryServicesByUuidOpCode_c:
                                {
                                    procedureType = gGattProcDiscoverPrimaryServicesByUuid_c;

                                    if(gGattProcSuccess_c == procedureResult)
                                    {
                                        /* Get services array size parameter from buffer */
                                        fsciBleGetUint8ValueFromBuffer(*(uint8_t*)pArraySize, pBuffer);
                                        /* Get services array parameter from buffer */
                                        for(iCount = 0; iCount < *(uint8_t*)pArraySize; iCount++)
                                        {
                                            fsciBleGattClientGetServiceFromBuffer(&pServices[iCount], &pBuffer);
                                        }
                                    }
                                }
                                break;

                            case gBleGattEvtClientProcedureFindIncludedServicesOpCode_c:
                                {
                                    procedureType = gGattProcFindIncludedServices_c;

                                    if(gGattProcSuccess_c == procedureResult)
                                    {
                                        /* Get service parameter from buffer */
                                        fsciBleGattClientGetServiceFromBuffer(pServices, &pBuffer);
                                    }
                                }
                                break;

                            case gBleGattEvtClientProcedureDiscoverAllCharacteristicsOpCode_c:
                                {
                                    procedureType = gGattProcDiscoverAllCharacteristics_c;

                                    if(gGattProcSuccess_c == procedureResult)
                                    {
                                        /* Get service parameter from buffer */
                                        fsciBleGattClientGetServiceFromBuffer(pServices, &pBuffer);
                                    }
                                }
                                break;

                            case gBleGattEvtClientProcedureDiscoverCharacteristicByUuidOpCode_c:
                                {
                                    procedureType = gGattProcDiscoverCharacteristicByUuid_c;

                                    if(gGattProcSuccess_c == procedureResult)
                                    {
                                        /* Get characteristics array size parameter from buffer */
                                        fsciBleGetUint8ValueFromBuffer(*(uint8_t*)pArraySize, pBuffer);
                                        /* Get characteristics array parameter from buffer */
                                        for(iCount = 0; iCount < *(uint8_t*)pArraySize; iCount++)
                                        {
                                            fsciBleGattClientGetCharFromBuffer(&pCharacteristics[iCount], &pBuffer);
                                        }
                                    }
                                }
                                break;

                            case gBleGattEvtClientProcedureDiscoverAllCharacteristicDescriptorsOpCode_c:
                                {
                                    procedureType = gGattProcDiscoverAllCharacteristicDescriptors_c;

                                    if(gGattProcSuccess_c == procedureResult)
                                    {
                                        /* Get characteristic parameter from buffer */
                                        fsciBleGattClientGetCharFromBuffer(pCharacteristics, &pBuffer);
                                    }
                                }
                                break;

                            case gBleGattEvtClientProcedureReadCharacteristicValueOpCode_c:
                                {
                                    procedureType = gGattProcReadCharacteristicValue_c;

                                    if(gGattProcSuccess_c == procedureResult)
                                    {
                                        /* Get characteristic parameter from buffer */
                                        fsciBleGattClientGetCharFromBuffer(pCharacteristics, &pBuffer);
                                    }
                                }
                                break;

                            case gBleGattEvtClientProcedureReadUsingCharacteristicUuidOpCode_c:
                                {
                                    procedureType = gGattProcReadUsingCharacteristicUuid_c;

                                    if(gGattProcSuccess_c == procedureResult)
                                    {
                                        /* Get value size parameter from buffer */
                                        fsciBleGetUint16ValueFromBuffer(*pArraySize, pBuffer);
                                        /* Get value parameter from buffer */
                                        fsciBleGetArrayFromBuffer(pValue, pBuffer, *pArraySize);
                                    }
                                }
                                break;

                            case gBleGattEvtClientProcedureReadMultipleCharacteristicValuesOpCode_c:
                                {
                                    uint8_t nbOfCharacteristics;
                                    procedureType = gGattProcReadMultipleCharacteristicValues_c;

                                    if(gGattProcSuccess_c == procedureResult)
                                    {
                                        /* Get characteristics array size parameter from buffer */
                                        fsciBleGetUint8ValueFromBuffer(nbOfCharacteristics, pBuffer);
                                        /* Get characteristics array parameter from buffer */
                                        for(iCount = 0; iCount < nbOfCharacteristics; iCount++)
                                        {
                                            fsciBleGattClientGetCharFromBuffer(&pCharacteristics[iCount], &pBuffer);
                                        }
                                    }
                                }
                                break;

                            case gBleGattEvtClientProcedureWriteCharacteristicValueOpCode_c:
                                {
                                    procedureType = gGattProcWriteCharacteristicValue_c;
                                }
                                break;

                            case gBleGattEvtClientProcedureReadCharacteristicDescriptorOpCode_c:
                                {
                                    procedureType = gGattProcReadCharacteristicDescriptor_c;

                                    if(gGattProcSuccess_c == procedureResult)
                                    {
                                        /* Get descriptor parameter from buffer */
                                        fsciBleGattClientGetAttributeFromBuffer(pDescriptors, &pBuffer);
                                    }
                                }
                                break;

                            case gBleGattEvtClientProcedureWriteCharacteristicDescriptorOpCode_c:
                                {
                                    procedureType = gGattProcWriteCharacteristicDescriptor_c;
                                }
                                break;

                            default:
                                {
                                    ; /* For MISRA compliance */
                                }
                                break;
                        }

                        /* Call client procedure callback */
                        fsciBleGattCallbacks.clientProcedureCallback(deviceId, procedureType, procedureResult, error);

                        /* Erase the information kept (allocated or just saved) */
                        fsciBleGattClientEraseInfo(FALSE);
                    }
                    break;

                case gBleGattEvtClientNotificationOpCode_c:
                case gBleGattEvtClientIndicationOpCode_c:
                    {
                        deviceId_t  deviceId;
                        uint16_t    characteristicValueHandle;
                        uint8_t*    pValue;
                        uint16_t    valueLength;

                        /* Get event parameters from buffer */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);
                        fsciBleGetUint16ValueFromBuffer(characteristicValueHandle, pBuffer);
                        fsciBleGetUint16ValueFromBuffer(valueLength, pBuffer);

                        pValue = MEM_BufferAlloc(valueLength);

                        if(NULL == pValue)
                        {
                            /* No memory */
                            fsciBleError(gFsciOutOfMessages_c, fsciInterfaceId);
                        }
                        else
                        {
                            fsciBleGetArrayFromBuffer(pValue, pBuffer, valueLength);

                            if(gBleGattEvtClientNotificationOpCode_c == pClientPacket->structured.header.opCode)
                            {
                                /* Call client notification callback */
                                fsciBleGattCallbacks.clientNotificationCallback(deviceId, characteristicValueHandle, pValue, valueLength);
                            }
                            else
                            {
                                /* Call client indication callback */
                                fsciBleGattCallbacks.clientIndicationCallback(deviceId, characteristicValueHandle, pValue, valueLength);
                            }

                            (void)MEM_BufferFree(pValue);

                            Ble_OutParamsReady();
                        }
                    }
                    break;

                case gBleGattEvtServerMtuChangedOpCode_c:
                case gBleGattEvtServerHandleValueConfirmationOpCode_c:
                case gBleGattEvtServerAttributeWrittenOpCode_c:
                case gBleGattEvtServerCharacteristicCccdWrittenOpCode_c:
                case gBleGattEvtServerAttributeWrittenWithoutResponseOpCode_c:
                case gBleGattEvtServerErrorOpCode_c:
                case gBleGattEvtServerLongCharacteristicWrittenOpCode_c:
                case gBleGattEvtServerAttributeReadOpCode_c:
                    {
                        deviceId_t              deviceId;
                        gattServerEvent_t*      pServerEvent;
                        gattServerEventType_t   eventType = gEvtMtuChanged_c;

                        switch(pClientPacket->structured.header.opCode)
                        {
                            case gBleGattEvtServerMtuChangedOpCode_c:
                                {
                                    eventType = gEvtMtuChanged_c;
                                }
                                break;

                            case gBleGattEvtServerHandleValueConfirmationOpCode_c:
                                {
                                    eventType = gEvtHandleValueConfirmation_c;
                                }
                                break;

                            case gBleGattEvtServerAttributeWrittenOpCode_c:
                                {
                                    eventType = gEvtAttributeWritten_c;
                                }
                                break;

                            case gBleGattEvtServerCharacteristicCccdWrittenOpCode_c:
                                {
                                    eventType = gEvtCharacteristicCccdWritten_c;
                                }
                                break;

                            case gBleGattEvtServerAttributeWrittenWithoutResponseOpCode_c:
                                {
                                    eventType = gEvtAttributeWrittenWithoutResponse_c;
                                }
                                break;

                            case gBleGattEvtServerErrorOpCode_c:
                                {
                                    eventType = gEvtError_c;
                                }
                                break;

                            case gBleGattEvtServerLongCharacteristicWrittenOpCode_c:
                                {
                                    eventType = gEvtLongCharacteristicWritten_c;
                                }
                                break;

                            case gBleGattEvtServerAttributeReadOpCode_c:
                                {
                                    eventType = gEvtAttributeRead_c;
                                }
                                break;

                            default:
                                ; /* For MISRA compliance */
                                break;
                        }

                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);

                        pServerEvent = fsciBleGattServerAllocServerEventForBuffer(eventType, pBuffer);

                        if(NULL == pServerEvent)
                        {
                            /* No memory */
                            fsciBleError(gFsciOutOfMessages_c, fsciInterfaceId);
                        }
                        else
                        {
                            /* Get event parameters from buffer */
                            fsciBleGattServGetServEventFromBuffer(pServerEvent, &pBuffer);

                            /* Call server callback */
                            fsciBleGattCallbacks.serverCallback(deviceId, pServerEvent);

                            /* Free allocated server event */
                            (void)MEM_BufferFree(pServerEvent);
                        }
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
    bFsciBleGattCmdInitiatedByFsci = FALSE;
#endif /* gFsciBleTest_d */

    (void)MEM_BufferFree(pData);
}


#if gFsciBleBBox_d || gFsciBleTest_d

void fsciBleGattStatusMonitor(bleResult_t result)
{
#if gFsciBleTest_d
    /* If GATT is disabled the status must be not monitored */
    if(FALSE == bFsciBleGattEnabled)
    {
        return;
    }
#endif /* gFsciBleTest_d */

    if(TRUE == bGattClientRequestWithOutParameters)
    {
        /* GATT client request with out parameters was initiated */
        if(gBleSuccess_c == result)
        {
#if gFsciBleTest_d
            bFsciBleGattClientAllocatedInfo = bFsciBleGattCmdInitiatedByFsci;
#endif /* gFsciBleTest_d */
            /* Keep the information because the procedure was accepted */
            fsciBleGattClientKeepInfo(bFsciBleGattClientAllocatedInfo);
        }
        else
        {
            /* Erase the information because the procedure was rejected */
#if gFsciBleTest_d
            fsciBleGattClientEraseTmpInfo(bFsciBleGattCmdInitiatedByFsci);
#else
            fsciBleGattClientEraseTmpInfo(bFsciBleGattClientAllocatedInfo);
#endif /* gFsciBleTest_d */
        }
    }

    /* Reset the flag that indicates if the last GATT request was a Client request
    with out parameters */
    bGattClientRequestWithOutParameters = FALSE;

    /* Send status over UART */
    fsciBleStatusMonitor(gFsciBleGattOpcodeGroup_c, (uint8_t)gBleGattStatusOpCode_c, result);
}

#endif /* gFsciBleBBox_d || gFsciBleTest_d */


#if gFsciBleHost_d || gFsciBleTest_d

void fsciBleGattNoParamCmdMonitor(fsciBleGattOpCode_t opCode)
{
#if gFsciBleTest_d
    /* If GATT is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGattEnabled) ||
       (TRUE == bFsciBleGattCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Call the generic FSCI BLE monitor for commands or events that have no parameters */
    fsciBleNoParamCmdOrEvtMonitor(gFsciBleGattOpcodeGroup_c, opCode);
}


void fsciBleGattMtuCmdMonitor(fsciBleGattOpCode_t opCode, deviceId_t deviceId)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GATT is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGattEnabled) ||
       (TRUE == bFsciBleGattCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGattAllocFsciPacket(opCode, fsciBleGetDeviceIdBufferSize(&deviceId));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGattGetMtuCmdMonitor(deviceId_t deviceId, uint16_t* pOutMtu)
{
    fsciBleGattMtuCmdMonitor(gBleGattCmdGetMtuOpCode_c, deviceId);

#if gFsciBleHost_d
    /* Save out parameters pointers */
    fsciBleGattSaveOutParams(pOutMtu);
#endif /* gFsciBleHost_d */
}


void fsciBleGattClientRegisterProcedureCallbackCmdMonitor(gattClientProcedureCallback_t callback)
{
#if gFsciBleHost_d
    fsciBleGattCallbacks.clientProcedureCallback = callback;
#endif /* gFsciBleHost_d */

    fsciBleGattNoParamCmdMonitor(gBleGattCmdClientRegisterProcedureCallbackOpCode_c);
}


void fsciBleGattClientRegisterNotificationCallbackCmdMonitor(gattClientNotificationCallback_t callback)
{
#if gFsciBleHost_d
    fsciBleGattCallbacks.clientNotificationCallback = callback;
#endif /* gFsciBleHost_d */

    fsciBleGattNoParamCmdMonitor(gBleGattCmdClientRegisterNotificationCallbackOpCode_c);
}


void fsciBleGattClientRegisterIndicationCallbackCmdMonitor(gattClientIndicationCallback_t callback)
{
#if gFsciBleHost_d
    fsciBleGattCallbacks.clientIndicationCallback = callback;
#endif /* gFsciBleHost_d */

    fsciBleGattNoParamCmdMonitor(gBleGattCmdClientRegisterIndicationCallbackOpCode_c);
}


void fsciBleGattClientDiscoverAllPrimaryServicesCmdMonitor(deviceId_t deviceId, gattService_t* pOutPrimaryServices, uint8_t maxServiceCount, uint8_t* pOutDiscoveredCount)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GATT is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGattEnabled) ||
       (TRUE == bFsciBleGattCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGattAllocFsciPacket(gBleGattCmdClientDiscoverAllPrimaryServicesOpCode_c,
                                               fsciBleGetDeviceIdBufferSize(&deviceId) + sizeof(uint8_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
    fsciBleGetBufferFromUint8Value(maxServiceCount, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);

    /* Save the information that must be monitored when the
    procedure ends */
    fsciBleGattClientSaveServicesInfo(deviceId, pOutPrimaryServices);
    fsciBleGattClientSaveArraySizeInfo(deviceId, (uint16_t*)pOutDiscoveredCount);

#if gFsciBleTest_d
    /* Set flag to indicate that the last GATT request was a Client
    request with out parameters */
    bGattClientRequestWithOutParameters = TRUE;
#endif /* gFsciBleTest_d */
}


void fsciBleGattClientDiscoverPrimaryServicesByUuidCmdMonitor(deviceId_t deviceId, bleUuidType_t uuidType, const bleUuid_t* pUuid, gattService_t* aOutPrimaryServices, uint8_t maxServiceCount, uint8_t* pOutDiscoveredCount)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GATT is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGattEnabled) ||
       (TRUE == bFsciBleGattCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGattAllocFsciPacket(gBleGattCmdClientDiscoverPrimaryServicesByUuidOpCode_c,
                                               fsciBleGetDeviceIdBufferSize(&deviceId) + sizeof(bleUuidType_t) +
                                               fsciBleGetUuidBufferSize(uuidType) + sizeof(uint8_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
    fsciBleGetBufferFromEnumValue(uuidType, pBuffer, bleUuidType_t);
    fsciBleGetBufferFromUuid(pUuid, &pBuffer, uuidType);
    fsciBleGetBufferFromUint8Value(maxServiceCount, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);

    /* Save the information that must be monitored when the
    procedure ends */
    fsciBleGattClientSaveServicesInfo(deviceId, aOutPrimaryServices);
    fsciBleGattClientSaveArraySizeInfo(deviceId, (uint16_t*)pOutDiscoveredCount);

#if gFsciBleTest_d
    /* Set flag to indicate that the last GATT request was a Client
    request with out parameters */
    bGattClientRequestWithOutParameters = TRUE;
#endif /* gFsciBleTest_d */
}


void fsciBleGattClientDiscoverCharacteristicOfServiceByUuidCmdMonitor(deviceId_t deviceId, bleUuidType_t uuidType, const bleUuid_t* pUuid, const gattService_t* pIoService, gattCharacteristic_t* aOutCharacteristics, uint8_t maxCharacteristicCount, uint8_t* pOutDiscoveredCount)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GATT is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGattEnabled) ||
       (TRUE == bFsciBleGattCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGattAllocFsciPacket(gBleGattCmdClientDiscoverCharacteristicOfServiceByUuidOpCode_c,
                                               fsciBleGetDeviceIdBufferSize(&deviceId) + sizeof(bleUuidType_t) +
                                               fsciBleGetUuidBufferSize(uuidType) +
                                               fsciBleGattClientGetServiceBufferSize(pIoService) +
                                               sizeof(uint8_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
    fsciBleGetBufferFromEnumValue(uuidType, pBuffer, bleUuidType_t);
    fsciBleGetBufferFromUuid(pUuid, &pBuffer, uuidType);
    fsciBleGattClientGetBufferFromService(pIoService, &pBuffer);
    fsciBleGetBufferFromUint8Value(maxCharacteristicCount, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);

    /* Save the information that must be monitored when the
    procedure ends */
    fsciBleGattClientSaveCharacteristicsInfo(deviceId, aOutCharacteristics);
    fsciBleGattClientSaveArraySizeInfo(deviceId, (uint16_t*)pOutDiscoveredCount);

#if gFsciBleTest_d
    /* Set flag to indicate that the last GATT request was a Client
    request with out parameters */
    bGattClientRequestWithOutParameters = TRUE;
#endif /* gFsciBleTest_d */
}


void fsciBleGattClientDiscoverAllCharacteristicDescriptorsCmdMonitor(deviceId_t deviceId, gattCharacteristic_t* pIoCharacteristic, uint16_t endingHandle, uint8_t maxDescriptorCount)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GATT is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGattEnabled) ||
       (TRUE == bFsciBleGattCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGattAllocFsciPacket(gBleGattCmdClientDiscoverAllCharacteristicDescriptorsOpCode_c,
                                               fsciBleGetDeviceIdBufferSize(&deviceId) +
                                               fsciBleGattClientGetCharacteristicBufferSize(pIoCharacteristic) +
                                               sizeof(uint16_t) + sizeof(uint8_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
    fsciBleGattClientGetBufferFromCharacteristic(pIoCharacteristic, &pBuffer);
    fsciBleGetBufferFromUint16Value(endingHandle, pBuffer);
    fsciBleGetBufferFromUint8Value(maxDescriptorCount, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);

    /* Save the information that must be monitored when the
    procedure ends */
    fsciBleGattClientSaveCharacteristicsInfo(deviceId, pIoCharacteristic);

#if gFsciBleTest_d
    /* Set flag to indicate that the last GATT request was a Client
    request with out parameters */
    bGattClientRequestWithOutParameters = TRUE;
#endif /* gFsciBleTest_d */
}


void fsciBleGattClientReadCharValueCmdMonitor(deviceId_t deviceId, gattCharacteristic_t* pIoCharacteristic, uint16_t maxReadBytes)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GATT is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGattEnabled) ||
       (TRUE == bFsciBleGattCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGattAllocFsciPacket(gBleGattCmdClientReadCharacteristicValueOpCode_c,
                                               fsciBleGetDeviceIdBufferSize(&deviceId) +
                                               fsciBleGattClientGetCharacteristicBufferSize(pIoCharacteristic) +
                                               sizeof(uint16_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
    fsciBleGattClientGetBufferFromCharacteristic(pIoCharacteristic, &pBuffer);
    fsciBleGetBufferFromUint16Value(maxReadBytes, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);

    /* Save the information that must be monitored when the
    procedure ends */
    fsciBleGattClientSaveCharacteristicsInfo(deviceId, pIoCharacteristic);

#if gFsciBleTest_d
    /* Set flag to indicate that the last GATT request was a Client
    request with out parameters */
    bGattClientRequestWithOutParameters = TRUE;
#endif /* gFsciBleTest_d */
}


void fsciBleGattClientReadUsingCharacteristicUuidCmdMonitor(deviceId_t deviceId, bleUuidType_t uuidType, const bleUuid_t* pUuid, const gattHandleRange_t* pHandleRange, uint8_t* aOutBuffer, uint16_t maxReadBytes, uint16_t* pOutActualReadBytes)
{
    bool_t                      bHandleRangeIncluded = (NULL == pHandleRange) ? FALSE : TRUE;
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GATT is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGattEnabled) ||
       (TRUE == bFsciBleGattCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGattAllocFsciPacket(gBleGattCmdClientReadUsingCharacteristicUuidOpCode_c,
                                               fsciBleGetDeviceIdBufferSize(&deviceId) + sizeof(bleUuidType_t) +
                                               fsciBleGetUuidBufferSize(uuidType) + sizeof(bool_t) +
                                               ((TRUE == bHandleRangeIncluded) ?
                                                fsciBleGattGetHandleRangeBufferSize(pHandleRange) : 0) +
                                               sizeof(uint16_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
    fsciBleGetBufferFromEnumValue(uuidType, pBuffer, bleUuidType_t);
    fsciBleGetBufferFromUuid(pUuid, &pBuffer, uuidType);
    fsciBleGetBufferFromBoolValue(bHandleRangeIncluded, pBuffer);

    if(TRUE == bHandleRangeIncluded)
    {
        fsciBleGattGetBufferFromHandleRange(pHandleRange, &pBuffer);
    }

    fsciBleGetBufferFromUint16Value(maxReadBytes, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);

    /* Save the information that must be monitored when the
    procedure ends */
    fsciBleGattClientSaveValueInfo(deviceId, aOutBuffer);
    fsciBleGattClientSaveArraySizeInfo(deviceId, pOutActualReadBytes);

#if gFsciBleTest_d
    /* Set flag to indicate that the last GATT request was a Client
    request with out parameters */
    bGattClientRequestWithOutParameters = TRUE;
#endif /* gFsciBleTest_d */
}


void fsciBleGattClientReadMultipleCharacteristicValuesCmdMonitor(deviceId_t deviceId, uint8_t cNumCharacteristics, gattCharacteristic_t* aIoCharacteristics)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;
    uint16_t                    dataSize = fsciBleGetDeviceIdBufferSize(&deviceId) + sizeof(uint8_t);
    uint32_t                    iCount;

#if gFsciBleTest_d
    /* If GATT is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGattEnabled) ||
       (TRUE == bFsciBleGattCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    for(iCount = 0; iCount < cNumCharacteristics; iCount++)
    {
        dataSize += fsciBleGattClientGetCharacteristicBufferSize(&aIoCharacteristics[iCount]);
    }

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGattAllocFsciPacket(gBleGattCmdClientReadMultipleCharacteristicValuesOpCode_c, dataSize);

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
    fsciBleGetBufferFromUint8Value(cNumCharacteristics, pBuffer);

    for(iCount = 0; iCount < cNumCharacteristics; iCount++)
    {
        fsciBleGattClientGetBufferFromCharacteristic(&aIoCharacteristics[iCount], &pBuffer);
    }

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);

    /* Save the information that must be monitored when the
    procedure ends */
    fsciBleGattClientSaveCharacteristicsInfo(deviceId, aIoCharacteristics);
    /* Keep the number of the characteristics (this number can be not
    saved using the general keeping mechanism because it is not
    a pointer, it is a value) */
    fsciBleGattClientNbOfCharacteristics = cNumCharacteristics;

#if gFsciBleTest_d
    /* Set flag to indicate that the last GATT request was a Client
    request with out parameters */
    bGattClientRequestWithOutParameters = TRUE;
#endif /* gFsciBleTest_d */
}


void fsciBleGattClientWriteCharValueCmdMonitor(deviceId_t deviceId, const gattCharacteristic_t* pCharacteristic, uint16_t valueLength, const uint8_t* aValue, bool_t withoutResponse, bool_t signedWrite, bool_t doReliableLongCharWrites, const uint8_t* aCsrk)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GATT is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGattEnabled) ||
       (TRUE == bFsciBleGattCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGattAllocFsciPacket(gBleGattCmdClientWriteCharacteristicValueOpCode_c,
                                               fsciBleGetDeviceIdBufferSize(&deviceId) +
                                               fsciBleGattClientGetCharacteristicBufferSize(pCharacteristic) +
                                               sizeof(uint16_t) + valueLength +
                                               sizeof(bool_t) + sizeof(bool_t) +
                                               sizeof(bool_t) + gcSmpCsrkSize_c);

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
    fsciBleGattClientGetBufferFromCharacteristic(pCharacteristic, &pBuffer);
    fsciBleGetBufferFromUint16Value(valueLength, pBuffer);
    fsciBleGetBufferFromArray(aValue, pBuffer, valueLength);
    fsciBleGetBufferFromBoolValue(withoutResponse, pBuffer);
    fsciBleGetBufferFromBoolValue(signedWrite, pBuffer);
    fsciBleGetBufferFromBoolValue(doReliableLongCharWrites, pBuffer);
    fsciBleGetBufferFromCsrk(aCsrk, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGattClientReadCharacteristicDescriptorCmdMonitor(deviceId_t deviceId, gattAttribute_t* pIoDescriptor, uint16_t maxReadBytes)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
   /* If GATT is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGattEnabled) ||
       (TRUE == bFsciBleGattCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGattAllocFsciPacket(gBleGattCmdClientReadCharacteristicDescriptorsOpCode_c,
                                               fsciBleGetDeviceIdBufferSize(&deviceId) +
                                               fsciBleGattClientGetAttributeBufferSize(pIoDescriptor) +
                                               sizeof(uint16_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
    fsciBleGattClientGetBufferFromAttribute(pIoDescriptor, &pBuffer);
    fsciBleGetBufferFromUint16Value(maxReadBytes, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);

    /* Save the information that must be monitored when the
    procedure ends */
    fsciBleGattClientSaveDescriptorsInfo(deviceId, pIoDescriptor);

#if gFsciBleTest_d
    /* Set flag to indicate that the last GATT request was a Client
    request with out parameters */
    bGattClientRequestWithOutParameters = TRUE;
#endif /* gFsciBleTest_d */
}


void fsciBleGattClientWriteCharacteristicDescriptorCmdMonitor(deviceId_t deviceId, const gattAttribute_t* pDescriptor, uint16_t valueLength, const uint8_t* aValue)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GATT is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGattEnabled) ||
       (TRUE == bFsciBleGattCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGattAllocFsciPacket(gBleGattCmdClientWriteCharacteristicDescriptorsOpCode_c,
                                               fsciBleGetDeviceIdBufferSize(&deviceId) +
                                               fsciBleGattClientGetAttributeBufferSize(pDescriptor) +
                                               sizeof(uint16_t) + valueLength);

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
    fsciBleGattClientGetBufferFromAttribute(pDescriptor, &pBuffer);
    fsciBleGetBufferFromUint16Value(valueLength, pBuffer);
    fsciBleGetBufferFromArray(aValue, pBuffer, valueLength);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGattClientFindIncludedServicesOrCharacteristicsCmdMonitor(fsciBleGattOpCode_t opCode, deviceId_t deviceId, gattService_t* pIoService, uint8_t maxCount)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

 #if gFsciBleTest_d
   /* If GATT is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGattEnabled) ||
       (TRUE == bFsciBleGattCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGattAllocFsciPacket(opCode,
                                               fsciBleGetDeviceIdBufferSize(&deviceId) +
                                               fsciBleGattClientGetServiceBufferSize(pIoService) +
                                               sizeof(uint8_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
    fsciBleGattClientGetBufferFromService(pIoService, &pBuffer);
    fsciBleGetBufferFromUint8Value(maxCount, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);

    /* Save the information that must be monitored when the
    procedure ends */
    fsciBleGattClientSaveServicesInfo(deviceId, pIoService);

#if gFsciBleTest_d
    /* Set flag to indicate that the last GATT request was a Client
    request with out parameters */
    bGattClientRequestWithOutParameters = TRUE;
#endif /* gFsciBleTest_d */
}


void fsciBleGattServerRegisterCallbackCmdMonitor(gattServerCallback_t callback)
{
#if gFsciBleHost_d
    fsciBleGattCallbacks.serverCallback = callback;
#endif /* gFsciBleHost_d */

    fsciBleGattNoParamCmdMonitor(gBleGattCmdServerRegisterCallbackOpCode_c);
}


void fsciBleGattServerRegisterHandlesForWriteOrReadNotificationsCmdMonitor(fsciBleGattOpCode_t opCode, uint8_t handleCount, const uint16_t* aAttributeHandles)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GATT is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGattEnabled) ||
       (TRUE == bFsciBleGattCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGattAllocFsciPacket(opCode,
                                               sizeof(uint8_t) + handleCount * sizeof(uint16_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromUint8Value(handleCount, pBuffer);
    fsciBleGetBufferFromArray(aAttributeHandles, pBuffer, handleCount * sizeof(uint16_t));

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGattServerSendAttributeWrittenOrReadStatusCmdMonitor(fsciBleGattOpCode_t opCode, deviceId_t deviceId, uint16_t attributeHandle, uint8_t status)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

 #if gFsciBleTest_d
   /* If GATT is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGattEnabled) ||
       (TRUE == bFsciBleGattCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGattAllocFsciPacket(opCode,
                                               fsciBleGetDeviceIdBufferSize(&deviceId) + sizeof(uint16_t) + sizeof(uint8_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
    fsciBleGetBufferFromUint16Value(attributeHandle, pBuffer);
    fsciBleGetBufferFromUint8Value(status, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGattServerSendNotificationOrIndicationCmdMonitor(fsciBleGattOpCode_t opCode, deviceId_t deviceId, uint16_t handle)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GATT is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGattEnabled) ||
       (TRUE == bFsciBleGattCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGattAllocFsciPacket(opCode, fsciBleGetDeviceIdBufferSize(&deviceId) +
                                                       sizeof(uint16_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
    fsciBleGetBufferFromUint16Value(handle, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGattServerSendInstantValueNotificationOrIndicationCmdMonitor(fsciBleGattOpCode_t opCode, deviceId_t deviceId, uint16_t handle, uint16_t valueLength, const uint8_t* pValue)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GATT is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGattEnabled) ||
       (TRUE == bFsciBleGattCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGattAllocFsciPacket(opCode, fsciBleGetDeviceIdBufferSize(&deviceId) +
                                                       sizeof(uint16_t) + sizeof(uint16_t) + valueLength);

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
    fsciBleGetBufferFromUint16Value(handle, pBuffer);
    fsciBleGetBufferFromUint16Value(valueLength, pBuffer);
    fsciBleGetBufferFromArray(pValue, pBuffer, valueLength);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}

void fsciBleGattServerRegisterUniqueHandlesForNotificationsCmdMonitor
(
    bool_t bWrite,
    bool_t bRead
)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GATT is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGattEnabled) ||
       (TRUE == bFsciBleGattCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGattAllocFsciPacket(gBleGattCmdServerRegisterUniqueHandlesForNotificationsOpCode_c,
                                               2 * sizeof(bool_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromBoolValue(bWrite, pBuffer);
    fsciBleGetBufferFromBoolValue(bRead, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}

#endif /* gFsciBleHost_d || gFsciBleTest_d */


void fsciBleGattGetMtuEvtMonitor(uint16_t* pOutMtu)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GATT is disabled the event must be not monitored */
    if(FALSE == bFsciBleGattEnabled)
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGattAllocFsciPacket((uint8_t)gBleGattEvtGetMtuOpCode_c, sizeof(uint16_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set event parameters in the buffer */
    fsciBleGetBufferFromUint16Value(*pOutMtu, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


#if gFsciBleBBox_d || gFsciBleTest_d

void fsciBleGattClientProcedureEvtMonitor(deviceId_t deviceId, gattProcedureType_t procedureType, gattProcedureResult_t procedureResult, bleResult_t error)
{
    /* Procedure completed - get the kept information */
    gattService_t*              pServices           = fsciBleGattClientGetServicesInfo(deviceId, bFsciBleGattClientAllocatedInfo);
    gattCharacteristic_t*       pCharacteristics    = fsciBleGattClientGetCharInfo(deviceId, bFsciBleGattClientAllocatedInfo);
    gattAttribute_t*            pDescriptors        = fsciBleGattClientGetDescriptorsInfo(deviceId, bFsciBleGattClientAllocatedInfo);
    uint8_t*                    pValue              = fsciBleGattClientGetValueInfo(deviceId, bFsciBleGattClientAllocatedInfo);
    uint16_t*                   pArraySize          = fsciBleGattClientGetArraySizeInfo(deviceId, bFsciBleGattClientAllocatedInfo);
    uint16_t                    dataSize            = fsciBleGetDeviceIdBufferSize(&deviceId) + sizeof(gattProcedureResult_t) +
                                                      sizeof(bleResult_t);
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;
    fsciBleGattOpCode_t         opCode;
    uint32_t                    iCount;
    bool_t                      earlyReturn = FALSE;

#if gFsciBleTest_d
    /* If GATT is disabled the event must be not monitored */
    if(FALSE != bFsciBleGattEnabled)
    {
#endif /* gFsciBleTest_d */
        /* Get the opCode of the event (depending on procedure type) */
        switch(procedureType)
        {
            case gGattProcExchangeMtu_c:
                {
                    opCode = gBleGattEvtClientProcedureExchangeMtuOpCode_c;
                }
                break;

            case gGattProcDiscoverAllPrimaryServices_c:
                {
                    opCode = gBleGattEvtClientProcedureDiscoverAllPrimaryServicesOpCode_c;
                }
                break;

            case gGattProcDiscoverPrimaryServicesByUuid_c:
                {
                    opCode = gBleGattEvtClientProcedureDiscoverPrimaryServicesByUuidOpCode_c;
                }
                break;

            case gGattProcFindIncludedServices_c:
                {
                    opCode = gBleGattEvtClientProcedureFindIncludedServicesOpCode_c;
                }
                break;

            case gGattProcDiscoverAllCharacteristics_c:
                {
                    opCode = gBleGattEvtClientProcedureDiscoverAllCharacteristicsOpCode_c;
                }
                break;

            case gGattProcDiscoverCharacteristicByUuid_c:
                {
                    opCode = gBleGattEvtClientProcedureDiscoverCharacteristicByUuidOpCode_c;
                }
                break;

            case gGattProcDiscoverAllCharacteristicDescriptors_c:
                {
                    opCode = gBleGattEvtClientProcedureDiscoverAllCharacteristicDescriptorsOpCode_c;
                }
                break;

            case gGattProcReadCharacteristicValue_c:
                {
                    opCode = gBleGattEvtClientProcedureReadCharacteristicValueOpCode_c;
                }
                break;

            case gGattProcReadUsingCharacteristicUuid_c:
                {
                    opCode = gBleGattEvtClientProcedureReadUsingCharacteristicUuidOpCode_c;
                }
                break;

            case gGattProcReadMultipleCharacteristicValues_c:
                {
                    opCode = gBleGattEvtClientProcedureReadMultipleCharacteristicValuesOpCode_c;
                }
                break;

            case gGattProcWriteCharacteristicValue_c:
                {
                    opCode = gBleGattEvtClientProcedureWriteCharacteristicValueOpCode_c;
                }
                break;

            case gGattProcReadCharacteristicDescriptor_c:
                {
                    opCode = gBleGattEvtClientProcedureReadCharacteristicDescriptorOpCode_c;
                }
                break;

            case gGattProcWriteCharacteristicDescriptor_c:
                {
                    opCode = gBleGattEvtClientProcedureWriteCharacteristicDescriptorOpCode_c;
                }
                break;

            default:
                {
                    /* Procedure not recognized */
                    fsciBleError(gFsciError_c, fsciBleInterfaceId);
                    fsciBleGattClientEraseInfo(bFsciBleGattClientAllocatedInfo);
                    earlyReturn = TRUE;
                    break;
                }
        }

        if (!earlyReturn)
        {
            /* When the procedure successfully completes, the information kept when
            the procedure was started, must be also monitored */
            if(gGattProcSuccess_c == procedureResult)
            {
                switch(procedureType)
                {
                    case gGattProcDiscoverAllPrimaryServices_c:
                    case gGattProcDiscoverPrimaryServicesByUuid_c:
                        {
                            /* Consider also the size of the kept services */
                            dataSize += sizeof(uint8_t);
                            for(iCount = 0; iCount < (uint8_t)*pArraySize; iCount++)
                            {
                                dataSize += fsciBleGattClientGetServiceBufferSize(&pServices[iCount]);
                            }
                        }
                        break;

                    case gGattProcFindIncludedServices_c:
                    case gGattProcDiscoverAllCharacteristics_c:
                        {
                            /* Consider also the size of the kept service */
                            dataSize += fsciBleGattClientGetServiceBufferSize(pServices);
                        }
                        break;

                    case gGattProcDiscoverCharacteristicByUuid_c:
                        {
                            /* Consider also the size of the kept characteristics */
                            dataSize += sizeof(uint8_t);
                            for(iCount = 0; iCount < (uint8_t)*pArraySize; iCount++)
                            {
                                dataSize += fsciBleGattClientGetCharacteristicBufferSize(&pCharacteristics[iCount]);
                            }
                        }
                        break;

                    case gGattProcDiscoverAllCharacteristicDescriptors_c:
                    case gGattProcReadCharacteristicValue_c:
                        {
                            /* Consider also the size of the kept characteristic */
                            dataSize += fsciBleGattClientGetCharacteristicBufferSize(pCharacteristics);
                        }
                        break;

                    case gGattProcReadUsingCharacteristicUuid_c:
                        {
                            /* Consider also the size of the kept value */
                            dataSize += sizeof(uint16_t) + *pArraySize;
                        }
                        break;

                     case gGattProcReadMultipleCharacteristicValues_c:
                        {
                            /* Consider also the size of the kept characteristics */
                            dataSize += sizeof(uint8_t);
                            for(iCount = 0; iCount < fsciBleGattClientNbOfCharacteristics; iCount++)
                            {
                                dataSize += fsciBleGattClientGetCharacteristicBufferSize(&pCharacteristics[iCount]);
                            }
                        }
                        break;


                    case gGattProcReadCharacteristicDescriptor_c:
                        {
                            /* Consider also the size of the kept descriptor */
                            dataSize += fsciBleGattClientGetAttributeBufferSize(pDescriptors);
                        }
                        break;

                    default:
                        ; /* For MISRA compliance */
                        break;
                }
            }

            /* Allocate the packet to be sent over UART */
            pClientPacket = fsciBleGattAllocFsciPacket((uint8_t)opCode, dataSize);

            if(NULL == pClientPacket)
            {
                return;
            }

            pBuffer = &pClientPacket->payload[0];

            /* Set event parameters in the buffer */
            fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
            fsciBleGetBufferFromEnumValue(procedureResult, pBuffer, gattProcedureResult_t);
            fsciBleGetBufferFromEnumValue(error, pBuffer, bleResult_t);

            /* When the procedure successfully completes, the information kept when
            the procedure was started, must be also monitored */
            if(gGattProcSuccess_c == procedureResult)
            {
                switch(procedureType)
                {
                    case gGattProcDiscoverAllPrimaryServices_c:
                    case gGattProcDiscoverPrimaryServicesByUuid_c:
                        {
                            /* Set services array size parameter in the buffer */
                            fsciBleGetBufferFromUint8Value((uint8_t)*pArraySize, pBuffer);
                            /* Set services array parameter in the buffer */
                            for(iCount = 0; iCount < (uint8_t)*pArraySize; iCount++)
                            {
                                fsciBleGattClientGetBufferFromService(&pServices[iCount], &pBuffer);
                            }
                        }
                        break;

                    case gGattProcFindIncludedServices_c:
                    case gGattProcDiscoverAllCharacteristics_c:
                        {
                            /* Set service parameter in the buffer */
                            fsciBleGattClientGetBufferFromService(pServices, &pBuffer);
                        }
                        break;

                    case gGattProcDiscoverCharacteristicByUuid_c:
                        {
                            /* Set characteristics array size parameter in the buffer */
                            fsciBleGetBufferFromUint8Value((uint8_t)*pArraySize, pBuffer);
                            /* Set characteristics array parameter in the buffer */
                            for(iCount = 0; iCount < (uint8_t)*pArraySize; iCount++)
                            {
                                fsciBleGattClientGetBufferFromCharacteristic(&pCharacteristics[iCount], &pBuffer);
                            }
                        }
                        break;

                    case gGattProcDiscoverAllCharacteristicDescriptors_c:
                    case gGattProcReadCharacteristicValue_c:
                        {
                            /* Set characteristic parameter in the buffer */
                            fsciBleGattClientGetBufferFromCharacteristic(pCharacteristics, &pBuffer);
                        }
                        break;

                    case gGattProcReadUsingCharacteristicUuid_c:
                        {
                            /* Set value size parameter in the buffer */
                            fsciBleGetBufferFromUint16Value(*pArraySize, pBuffer);
                            /* Set value parameter in the buffer */
                            fsciBleGetBufferFromArray(pValue, pBuffer, *pArraySize);
                        }
                        break;

                     case gGattProcReadMultipleCharacteristicValues_c:
                        {
                            /* Set characteristics array size parameter in the buffer */
                            fsciBleGetBufferFromUint8Value(fsciBleGattClientNbOfCharacteristics, pBuffer);
                            /* Set characteristics array parameter in the buffer */
                            for(iCount = 0; iCount < fsciBleGattClientNbOfCharacteristics; iCount++)
                            {
                                fsciBleGattClientGetBufferFromCharacteristic(&pCharacteristics[iCount], &pBuffer);
                            }
                        }
                        break;


                    case gGattProcReadCharacteristicDescriptor_c:
                        {
                            /* Set descriptor parameter in the buffer */
                            fsciBleGattClientGetBufferFromAttribute(pDescriptors, &pBuffer);
                        }
                        break;

                    default:
                        ; /* For MISRA compliance */
                        break;
                }
            }

            /* Transmit the packet over UART */
            fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
        }
#if gFsciBleTest_d
    }
#endif /* gFsciBleTest_d */

    if(!earlyReturn)
    {
        /* Erase the information kept (allocated or just saved) */
        fsciBleGattClientEraseInfo(bFsciBleGattClientAllocatedInfo);
    }
}


void fsciBleGattClientNotificationOrIndicationEvtMonitor(fsciBleGattOpCode_t opCode, deviceId_t deviceId, uint16_t characteristicValueHandle, uint8_t* aValue, uint16_t valueLength)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GATT is disabled the event must be not monitored */
    if(FALSE == bFsciBleGattEnabled)
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGattAllocFsciPacket((uint8_t)opCode, fsciBleGetDeviceIdBufferSize(&deviceId) +
                                                       sizeof(uint16_t) + sizeof(uint16_t) +
                                                       valueLength);

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set event parameters in the buffer */
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
    fsciBleGetBufferFromUint16Value(characteristicValueHandle, pBuffer);
    fsciBleGetBufferFromUint16Value(valueLength, pBuffer);
    fsciBleGetBufferFromArray(aValue, pBuffer, valueLength);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGattServerEvtMonitor(deviceId_t deviceId, gattServerEvent_t* pServerEvent)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;
    fsciBleGattOpCode_t         opCode;
    bool_t earlyReturn = FALSE;

#if gFsciBleTest_d
    /* If GATT is disabled the event must be not monitored */
    if(FALSE == bFsciBleGattEnabled)
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Get the FSCI opcode */
    switch(pServerEvent->eventType)
    {
        case gEvtMtuChanged_c:
            {
                opCode = gBleGattEvtServerMtuChangedOpCode_c;
            }
            break;

        case gEvtHandleValueConfirmation_c:
            {
                opCode = gBleGattEvtServerHandleValueConfirmationOpCode_c;
            }
          break;

        case gEvtAttributeWritten_c:
            {
                opCode = gBleGattEvtServerAttributeWrittenOpCode_c;
            }
            break;

        case gEvtCharacteristicCccdWritten_c:
            {
                opCode = gBleGattEvtServerCharacteristicCccdWrittenOpCode_c;
            }
            break;

        case gEvtAttributeWrittenWithoutResponse_c:
            {
                opCode = gBleGattEvtServerAttributeWrittenWithoutResponseOpCode_c;
            }
            break;

        case gEvtError_c:
            {
                opCode = gBleGattEvtServerErrorOpCode_c;
            }
            break;

        case gEvtLongCharacteristicWritten_c:
            {
                opCode = gBleGattEvtServerLongCharacteristicWrittenOpCode_c;
            }
            break;

        case gEvtAttributeRead_c:
            {
                opCode = gBleGattEvtServerAttributeReadOpCode_c;
            }
            break;

        default:
            {
                fsciBleError(gFsciError_c, fsciBleInterfaceId);
                earlyReturn = TRUE;
                break;
            }
    }

    if (!earlyReturn)
    {
        /* Allocate the packet to be sent over UART */
        pClientPacket = fsciBleGattAllocFsciPacket((uint8_t)opCode, fsciBleGetDeviceIdBufferSize(&deviceId) +
                                                           fsciBleGattServerGetServerEventBufferSize(pServerEvent));

        if(NULL == pClientPacket)
        {
            return;
        }

        pBuffer = &pClientPacket->payload[0];

        /* Set event parameters in the buffer */
        fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
        fsciBleGattServGetBuffrFromServEvent(pServerEvent, &pBuffer);

        /* Transmit the packet over UART */
        fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
    }
}

#endif /* gFsciBleBBox_d || gFsciBleTest_d */

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

void fsciBleGattClientProcedureCallback(deviceId_t deviceId, gattProcedureType_t procedureType, gattProcedureResult_t procedureResult, bleResult_t error)
{
    fsciBleGattClientProcedureEvtMonitor(deviceId, procedureType, procedureResult, error);
}

void fsciBleGattClientNotificationCallback(deviceId_t deviceId, uint16_t characteristicValueHandle, uint8_t* aValue, uint16_t valueLength)
{
    fsciBleGattClientNotificationEvtMonitor(deviceId, characteristicValueHandle, aValue, valueLength);
}

void fsciBleGattClientIndicationCallback(deviceId_t deviceId, uint16_t characteristicValueHandle, uint8_t* aValue, uint16_t valueLength)
{
    fsciBleGattClientIndicationEvtMonitor(deviceId, characteristicValueHandle, aValue, valueLength);
}

void fsciBleGattServerCallback(deviceId_t deviceId, gattServerEvent_t* pServerEvent)
{
    fsciBleGattServerEvtMonitor(deviceId, pServerEvent);
}

#endif /* gFsciIncluded_c && gFsciBleGattLayerEnabled_d */

/*! *********************************************************************************
* @}
********************************************************************************** */
