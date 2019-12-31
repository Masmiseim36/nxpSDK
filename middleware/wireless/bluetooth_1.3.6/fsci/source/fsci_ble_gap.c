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
#include "EmbeddedTypes.h"
#include "fsci_ble_gap.h"
#if gFsciBleHost_d
    #include "host_ble.h"
    #include "FsciCommands.h"
#endif /* gFsciBleHost_d */

#if gFsciIncluded_c && gFsciBleGapLayerEnabled_d

#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE == 1U)
#if STACK_THREAD
#include "thread_ble_callbacks.h"
#else
#include "bluetooth_ble_callbacks.h"
#endif
#endif

/************************************************************************************
*************************************************************************************
* Private constants & macros
*************************************************************************************
************************************************************************************/

#if gFsciBleHost_d
    /* Macro used for saving the out parameters pointers of the GAP functions */
    #define fsciBleGapSaveOutParams(pFirstParam, pSecondParam)      \
            fsciBleGapOutParams.pParam1 = (uint8_t*)pFirstParam;    \
            fsciBleGapOutParams.pParam2 = (uint8_t*)pSecondParam

    /* Macro used for restoring the out parameters pointers of the GAP functions */
    #define fsciBleGapRestoreOutParams()    \
            &fsciBleGapOutParams

    /* Macro used for setting the out parameters pointers of the GAP
    functions to NULL */
    #define fsciBleGapCleanOutParams()              \
            fsciBleGapOutParams.pParam1 = NULL;     \
            fsciBleGapOutParams.pParam2 = NULL;
#endif /* gFsciBleHost_d */

#define fsciBleGapCallApiFunction(apiFunction)          bleResult_t result = (apiFunction); \
                                                        fsciBleGapStatusMonitor(result)
#define fsciBleGapMonitorOutParams(functionId, ...)     if(gBleSuccess_c == result)                  \
                                                        {                                            \
                                                            FsciEvtMonitor(functionId, __VA_ARGS__); \
                                                        }

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

/*! Structure that keeps callbacks registered by application or FSCI in GAP. */
typedef struct fsciBleGapCallbacks_tag
{
    hciHostToControllerInterface_t  hostToControllerInterface;  /* LE Controller uplink interface function
                                                                pointer. */
    gapGenericCallback_t            genericCallback;            /* Callback used to propagate GAP controller
                                                                events to the application. */
    gapAdvertisingCallback_t        advertisingCallback;        /* Callback used by the application to
                                                                receive advertising events. */
    gapScanningCallback_t           scanningCallback;           /* Callback used by the application to
                                                                receive scanning events. */
    gapConnectionCallback_t         connectionCallback;         /* Callback used by the application to
                                                                receive connection events. */
}fsciBleGapCallbacks_t;

#if gFsciBleHost_d
    /* Structure used for keeping the out parameters pointers of the GAP
     functions */
    typedef struct fsciBleGapOutParams_tag
    {
        uint8_t*    pParam1;
        uint8_t*    pParam2;
    }fsciBleGapOutParams_t;

    /* Structure used for keeping the out parameters pointers of the
    Gap_CheckNotificationStatus function */
    typedef struct fsciBleGapCheckNotificationStatusOutParams_tag
    {
        bool_t* pIsActive;
    }fsciBleGapCheckNotificationStatusOutParams_t;

    /* Structure used for keeping the out parameters pointers of the
    Gap_CheckIndicationStatus function */
    typedef struct fsciBleGapCheckIndicationStatusOutParams_tag
    {
        bool_t* pIsActive;
    }fsciBleGapCheckIndicationStatusOutParams_t;

    /* Structure used for keeping the out parameters pointers of the
    Gap_GetBondedDevicesIdentityInformation function */
    typedef struct fsciBleGapGetBondedDevicesIdentityInformationOutParams_tag
    {
        gapIdentityInformation_t* pIdentityAddresses;
        uint8_t*                  pActualCount;
    }fsciBleGapGetBondedDevicesIdentityInformationParams_t;

    /* Structure used for keeping the out parameters pointers of the
    Gap_LoadEncryptionInformation function */
    typedef struct fsciBleGapLoadEncryptionInformationOutParams_tag
    {
        uint8_t*    pLtk;
        uint8_t*    pLtkSize;
    }fsciBleGapLoadEncryptionInformationOutParams_t;

    /* Structure used for keeping the out parameters pointers of the
    Gap_LoadCustomPeerInformation function */
    typedef struct fsciBleGapLoadCustomPeerInformationOutParams_tag
    {
        uint8_t*   pInfo;
    }fsciBleGapLoadCustomPeerInformationOutParams_t;

    /* Structure used for keeping the out parameters pointers of the
    Gap_CheckIfBonded function */
    typedef struct fsciBleGapCheckIfBondedOutParams_tag
    {
        bool_t* pIsBonded;
    }fsciBleGapCheckIfBondedOutParams_t;

    /* Structure used for keeping the out parameters pointers of the
    Gap_GetBondedDevicesCount function */
    typedef struct fsciBleGapGetBondedDevicesCountOutParams_tag
    {
        uint8_t*    pBondedDevicesCount;
    }fsciBleGapGetBondedDevicesCountOutParams_t;

    /* Structure used for keeping the out parameters pointers of the
    Gap_GetBondedDeviceName function */
    typedef struct fsciBleGapGetBondedDeviceNameOutParams_tag
    {
        uchar_t*    pName;
    }fsciBleGapGetBondedDeviceNameOutParams_t;
#endif /* gFsciBleHost_d */

/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/

static void fsciBleGapGenericCallback(gapGenericEvent_t* pGenericEvent);
static bleResult_t fsciBleHciHostToControllerInterface(hciPacketType_t packetType, void* pPacket, uint16_t packetSize);

/************************************************************************************
*************************************************************************************
* Public functions prototypes
*************************************************************************************
************************************************************************************/
void fsciBleGapAdvertisingCallback(gapAdvertisingEvent_t* pAdvertisingEvent);
void fsciBleGapScanningCallback(gapScanningEvent_t* pScanningEvent);
void fsciBleGapConnectionCallback(deviceId_t deviceId, gapConnectionEvent_t* pConnectionEvent);

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

/* GAP callbacks structure initialized with FSCI empty static functions */
static fsciBleGapCallbacks_t fsciBleGapCallbacks =
{
    fsciBleHciHostToControllerInterface,
    fsciBleGapGenericCallback,
    fsciBleGapAdvertisingCallback,
    fsciBleGapScanningCallback,
    fsciBleGapConnectionCallback
};

#if gFsciBleBBox_d || gFsciBleTest_d
    static gapAutoConnectParams_t* pAutoConnectParams = NULL;
#endif /* gFsciBleBBox_d || gFsciBleTest_d */

#if gFsciBleTest_d
    /* Indicates if FSCI for GAP is enabled or not */
    static bool_t bFsciBleGapEnabled            = FALSE;

    /* Indicates if the command was initiated by FSCI (to be not monitored) */
    static bool_t bFsciBleGapCmdInitiatedByFsci = FALSE;
#endif /* gFsciBleTest_d */

#if gFsciBleHost_d
    /* Keeps out parameters pointers for Host - BBox functionality */
    static fsciBleGapOutParams_t fsciBleGapOutParams = {NULL, NULL};
#endif /* gFsciBleHost_d */

#if gFsciBleTest_d
    static bool_t                   mDefaultGapPairingActive = FALSE;
    static gapPairingParameters_t   mDefaultGapPairingParameters;
#endif

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

void fsciBleSetHciHostToControllerInterface(hciHostToControllerInterface_t hostToControllerInterface)
{
    /* Set HCI host to controller interface to an application desired function, only
    if not NULL. Otherwise set it to the FSCI empty static function */
    fsciBleGapCallbacks.hostToControllerInterface = (NULL != hostToControllerInterface) ?
                                                    hostToControllerInterface :
                                                    fsciBleHciHostToControllerInterface;
}


void fsciBleSetGapGenericCallback(gapGenericCallback_t genericCallback)
{
    /* Set GAP controller callback to an application desired function, only
    if not NULL. Otherwise set it to the FSCI empty static function */
    fsciBleGapCallbacks.genericCallback  = (NULL != genericCallback) ?
                                           genericCallback :
                                           fsciBleGapGenericCallback;
}


void fsciBleSetGapAdvertisingCallback(gapAdvertisingCallback_t advertisingCallback)
{
    /* Set GAP advertising callback to an application desired function, only
    if not NULL. Otherwise set it to the FSCI empty static function */
    fsciBleGapCallbacks.advertisingCallback = (NULL != advertisingCallback) ?
                                              advertisingCallback :
                                              fsciBleGapAdvertisingCallback;
}


void fsciBleSetGapConnectionCallback(gapConnectionCallback_t connectionCallback)
{
    /* Set GAP connection callback to an application desired function, only
    if not NULL. Otherwise set it to the FSCI empty static function */
    fsciBleGapCallbacks.connectionCallback  = (NULL != connectionCallback) ?
                                              connectionCallback :
                                              fsciBleGapConnectionCallback;
}


void fsciBleSetGapScanningCallback(gapScanningCallback_t scanningCallback)
{
    /* Set GAP scanning callback to an application desired function, only
    if not NULL. Otherwise set it to the FSCI empty static function */
    fsciBleGapCallbacks.scanningCallback  = (NULL != scanningCallback) ?
                                            scanningCallback :
                                            fsciBleGapScanningCallback;
}

void fsciBleGapHandler(void* pData, void* param, uint32_t fsciInterfaceId)
{
    clientPacket_t* pClientPacket   = (clientPacket_t*)pData;
    uint8_t*        pBuffer         = &pClientPacket->structured.payload[0];

#if gFsciBleTest_d
    /* Mark this command as initiated by FSCI */
    bFsciBleGapCmdInitiatedByFsci = TRUE;

    /* Verify if the command is Mode Select */
    if(gBleGapModeSelectOpCode_c == pClientPacket->structured.header.opCode)
    {
        /* Get the new FSCI for GAP status (enabled or disabled) */
        fsciBleGetBoolValueFromBuffer(bFsciBleGapEnabled, pBuffer);
        /* Return status */
        fsciBleGapStatusMonitor(gBleSuccess_c);
    }
    else
    {
        /* Verify if FSCI is enabled for this layer */
        if(TRUE == bFsciBleGapEnabled)
        {
#endif /* gFsciBleTest_d */
            /* Select the GAP function to be called (using the FSCI opcode) */
            switch(pClientPacket->structured.header.opCode)
            {
#if gFsciBleBBox_d || gFsciBleTest_d
                case (uint8_t)gBleGapCmdBleHostInitializeOpCode_c:
                    {
#if !defined(MULTICORE_APPLICATION_CORE) || (!MULTICORE_APPLICATION_CORE)
                        fsciBleGapCallApiFunction(Ble_HostInitialize(fsciBleGapCallbacks.genericCallback, fsciBleGapCallbacks.hostToControllerInterface));
#endif
                    }
                    break;

                case (uint8_t)gBleGapCmdRegisterDeviceSecurityRequirementsOpCode_c:
                    {
                        bool_t                              bDeviceSecurityRequirementsIncluded;
                        gapDeviceSecurityRequirements_t*    pDeviceSecurityRequirements = NULL;
                        bleResult_t                         status                      = gBleSuccess_c;

                        /* Get from buffer the boolean value which indicates if device security
                        requirements are included in the request */
                        fsciBleGetBoolValueFromBuffer(bDeviceSecurityRequirementsIncluded, pBuffer);

                        /* Verify if device security requirements are included in the request or not */
                        if(TRUE == bDeviceSecurityRequirementsIncluded)
                        {
                            /* Allocate buffer for the device security requirements */
                            pDeviceSecurityRequirements = fsciBleGapAllocDeviceSecurityRequirementsForBuffer(pBuffer);

                            if(NULL == pDeviceSecurityRequirements)
                            {
                                /* No memory */
                                status = gBleOverflow_c;
                            }
                            else
                            {
                                /* Get the device security requirements from buffer */
                                fsciBleGapGetDeviceSecurityRequirementsFromBuffer(pDeviceSecurityRequirements, &pBuffer);
                            }
                        }

                        if(gBleSuccess_c == status)
                        {
                            fsciBleGapCallApiFunction(Gap_RegisterDeviceSecurityRequirements(pDeviceSecurityRequirements));

                            /* Free the buffer allocated for device security requirements */
                            (void)fsciBleGapFreeDeviceSecurityRequirements(pDeviceSecurityRequirements);
                        }
                        else
                        {
                            /* The GAP command can not be executed */
                            fsciBleError(gFsciOutOfMessages_c, fsciInterfaceId);
                        }
                    }
                    break;

                case (uint8_t)gBleGapCmdSetAdvertisingParametersOpCode_c:
                    {
                        gapAdvertisingParameters_t advertisingParameters;

                        /* Get advertising parameters from buffer */
                        fsciBleGapGetAdvertisingParametersFromBuffer(&advertisingParameters, &pBuffer);

                        fsciBleGapCallApiFunction(Gap_SetAdvertisingParameters(&advertisingParameters));
                    }
                    break;

#if defined(gBLE50_d) && (gBLE50_d == 1U)
                case gBleGapCmdSetExtAdvertisingParametersOpCode_c:
                    {
                        gapExtAdvertisingParameters_t advertisingParameters;

                        /* Get advertising parameters from buffer */
                        fsciBleGapGetExtAdvertisingParametersFromBuffer(&advertisingParameters, &pBuffer);

                        fsciBleGapCallApiFunction(Gap_SetExtAdvertisingParameters(&advertisingParameters));
                    }
                    break;

                case gBleGapCmdSetExtAdvertisingDataOpCode_c:
                    {
                        uint8_t                 handle;
                        bool_t                  bAdvertisingDataIncluded;
                        gapAdvertisingData_t*   pAdvertisingData    = NULL;
                        bool_t                  bScanResponseDataIncluded;
                        gapScanResponseData_t*  pScanResponseData   = NULL;
                        bleResult_t             status              = gBleSuccess_c;

                        fsciBleGetUint8ValueFromBuffer(handle, pBuffer);

                        /* Get from buffer the boolean value which indicates if advertising
                        data is included in the request */
                        fsciBleGetBoolValueFromBuffer(bAdvertisingDataIncluded, pBuffer);

                        /* Verify if advertising data is included in the request or not */
                        if(TRUE == bAdvertisingDataIncluded)
                        {
                            /* Allocate buffer for the advertising data */
                            pAdvertisingData = fsciBleGapAllocAdvertisingDataForBuffer(pBuffer);

                            if(NULL == pAdvertisingData)
                            {
                                /* No memory */
                                status = gBleOverflow_c;
                            }
                            else
                            {
                                /* Get the advertising data from buffer */
                                fsciBleGapGetAdvertisingDataFromBuffer(pAdvertisingData, &pBuffer);
                            }
                        }

                        if(gBleSuccess_c == status)
                        {
                            /* Get from buffer the boolean value which indicates if scan response
                            data is included in the request */
                            fsciBleGetBoolValueFromBuffer(bScanResponseDataIncluded, pBuffer);

                            /* Verify if scan response data is included in the request or not */
                            if(TRUE == bScanResponseDataIncluded)
                            {
                                /* Allocate buffer for the scan response data */
                                pScanResponseData = fsciBleGapAllocScanResponseDataForBuffer(pBuffer);

                                if(NULL == pScanResponseData)
                                {
                                    /* No memory */
                                    status = gBleOverflow_c;
                                    /* Free the buffer allocated for advertising data */
                                    fsciBleGapFreeAdvertisingData(pAdvertisingData);
                                }
                                else
                                {
                                    /* Get the scan response data from buffer */
                                    fsciBleGapGetScanResponseDataFromBuffer(pScanResponseData, &pBuffer);
                                }
                            }
                        }

                        if(gBleSuccess_c == status)
                        {
                            fsciBleGapCallApiFunction(Gap_SetExtAdvertisingData(handle, pAdvertisingData, pScanResponseData));

                            /* Free the buffers allocated for advertising and scan response data */
                            fsciBleGapFreeAdvertisingData(pAdvertisingData);
                            fsciBleGapFreeScanResponseData(pScanResponseData);
                        }
                        else
                        {
                            /* The GAP command can not be executed */
                            fsciBleError(gFsciOutOfMessages_c, fsciInterfaceId);
                        }
                    }
                    break;

                case gBleGapCmdSetPeriodicAdvDataOpCode_c:
                    {
                        uint8_t                 handle;
                        bleResult_t             status = gBleSuccess_c;
                        gapAdvertisingData_t*   pAdvertisingData;

                        fsciBleGetUint8ValueFromBuffer(handle, pBuffer);

                        /* Allocate buffer for the advertising data */
                        pAdvertisingData = fsciBleGapAllocAdvertisingDataForBuffer(pBuffer);

                        if(NULL == pAdvertisingData)
                        {
                            /* No memory */
                            status = gBleOverflow_c;
                        }
                        else
                        {
                            /* Get the advertising data from buffer */
                            fsciBleGapGetAdvertisingDataFromBuffer(pAdvertisingData, &pBuffer);
                        }

                        if (gBleSuccess_c == status)
                        {
                            fsciBleGapCallApiFunction(Gap_SetPeriodicAdvertisingData(handle, pAdvertisingData));

                            /* Free the buffers allocated for advertising and scan response data */
                            fsciBleGapFreeAdvertisingData(pAdvertisingData);
                        }
                        else
                        {
                            /* The GAP command can not be executed */
                            fsciBleError(gFsciOutOfMessages_c, fsciInterfaceId);
                        }
                    }
                    break;

#endif

                case (uint8_t)gBleGapCmdSetAdvertisingDataOpCode_c:
                    {
                        bool_t                  bAdvertisingDataIncluded;
                        gapAdvertisingData_t*   pAdvertisingData    = NULL;
                        bool_t                  bScanResponseDataIncluded;
                        gapScanResponseData_t*  pScanResponseData   = NULL;
                        bleResult_t             status              = gBleSuccess_c;

                        /* Get from buffer the boolean value which indicates if advertising
                        data is included in the request */
                        fsciBleGetBoolValueFromBuffer(bAdvertisingDataIncluded, pBuffer);

                        /* Verify if advertising data is included in the request or not */
                        if(TRUE == bAdvertisingDataIncluded)
                        {
                            /* Allocate buffer for the advertising data */
                            pAdvertisingData = fsciBleGapAllocAdvertisingDataForBuffer(pBuffer);

                            if(NULL == pAdvertisingData)
                            {
                                /* No memory */
                                status = gBleOverflow_c;
                            }
                            else
                            {
                                /* Get the advertising data from buffer */
                                fsciBleGapGetAdvertisingDataFromBuffer(pAdvertisingData, &pBuffer);
                            }
                        }

                        if(gBleSuccess_c == status)
                        {
                            /* Get from buffer the boolean value which indicates if scan response
                            data is included in the request */
                            fsciBleGetBoolValueFromBuffer(bScanResponseDataIncluded, pBuffer);

                            /* Verify if scan response data is included in the request or not */
                            if(TRUE == bScanResponseDataIncluded)
                            {
                                /* Allocate buffer for the scan response data */
                                pScanResponseData = fsciBleGapAllocScanResponseDataForBuffer(pBuffer);

                                if(NULL == pScanResponseData)
                                {
                                    /* No memory */
                                    status = gBleOverflow_c;
                                    /* Free the buffer allocated for advertising data */
                                    (void)fsciBleGapFreeAdvertisingData(pAdvertisingData);
                                }
                                else
                                {
                                    /* Get the scan response data from buffer */
                                    fsciBleGapGetScanResponseDataFromBuffer(pScanResponseData, &pBuffer);
                                }
                            }
                        }

                        if(gBleSuccess_c == status)
                        {
                            fsciBleGapCallApiFunction(Gap_SetAdvertisingData(pAdvertisingData, pScanResponseData));

                            /* Free the buffers allocated for advertising and scan response data */
                            (void)fsciBleGapFreeAdvertisingData(pAdvertisingData);
                            (void)fsciBleGapFreeScanResponseData(pScanResponseData);
                        }
                        else
                        {
                            /* The GAP command can not be executed */
                            fsciBleError(gFsciOutOfMessages_c, fsciInterfaceId);
                        }
                    }
                    break;

                case (uint8_t)gBleGapCmdStartAdvertisingOpCode_c:
                    {
                        fsciBleGapCallApiFunction(Gap_StartAdvertising(fsciBleGapCallbacks.advertisingCallback, fsciBleGapCallbacks.connectionCallback));
                    }
                    break;
#if defined(gBLE50_d) && (gBLE50_d == 1U)
                case gBleGapCmdStartExtAdvertisingOpCode_c:
                    {
                        uint8_t  handle;
                        uint16_t duration;
                        uint8_t maxExtAdvEvents;

                        /* Get start parameters from buffer */
                        fsciBleGetUint8ValueFromBuffer(handle, pBuffer);
                        fsciBleGetUint16ValueFromBuffer(duration, pBuffer);
                        fsciBleGetUint8ValueFromBuffer(maxExtAdvEvents, pBuffer);
                        fsciBleGapCallApiFunction(Gap_StartExtAdvertising(fsciBleGapCallbacks.advertisingCallback,
                                                                          fsciBleGapCallbacks.connectionCallback,
                                                                          handle,
                                                                          duration,
                                                                          maxExtAdvEvents));
                    }
                    break;

                case gBleGapCmdUpdatePeriodicAdvListOpCode_c:
                    {
                        gapPeriodicAdvListOperation_t operation;
                        bleAddressType_t addrType;
                        bleDeviceAddress_t addr;
                        uint8_t SID;

                        fsciBleGetEnumValueFromBuffer(operation, pBuffer, gapPeriodicAdvListOperation_t);
                        fsciBleGetEnumValueFromBuffer(addrType, pBuffer, bleAddressType_t);
                        fsciBleGetAddressFromBuffer(addr, pBuffer);
                        fsciBleGetUint8ValueFromBuffer(SID, pBuffer);
                        fsciBleGapCallApiFunction(Gap_UpdatePeriodicAdvList(operation, addrType, addr, SID));

                    }
                    break;

                case gBleGapCmdPeriodicAdvTerminateSyncOpCode_c:
                    {
                        uint16_t  syncHandle;

                        /* Get start parameters from buffer */
                        fsciBleGetUint16ValueFromBuffer(syncHandle, pBuffer);
                        fsciBleGapCallApiFunction(Gap_PeriodicAdvTerminateSync(syncHandle));
                    }
                    break;
#endif

                case (uint8_t)gBleGapCmdStopAdvertisingOpCode_c:
                    {
                        fsciBleGapCallApiFunction(Gap_StopAdvertising());
                    }
                    break;

#if defined(gBLE50_d) && (gBLE50_d == 1U)
                case gBleGapCmdStopExtAdvertisingOpCode_c:
                    {
                        uint8_t handle;

                        /* Get stop handle from buffer */
                        fsciBleGetUint8ValueFromBuffer(handle, pBuffer);
                        fsciBleGapCallApiFunction(Gap_StopExtAdvertising(handle));
                    }
                    break;

                case gBleGapCmdRemoveAdvSetOpCode_c:
                    {
                        uint8_t handle;
                        fsciBleGetUint8ValueFromBuffer(handle, pBuffer);
                        fsciBleGapCallApiFunction(Gap_RemoveAdvSet(handle));
                    }
                    break;

                case gBleGapCmdStartPeriodicAdvOpCode_c:
                    {
                        uint8_t handle;

                        /* Get handle from buffer */
                        fsciBleGetUint8ValueFromBuffer(handle, pBuffer);
                        fsciBleGapCallApiFunction(Gap_StartPeriodicAdvertising(handle));
                    }
                    break;

                case gBleGapCmdStopPeriodicAdvOpCode_c:
                    {
                        uint8_t handle;

                        /* Get handle from buffer */
                        fsciBleGetUint8ValueFromBuffer(handle, pBuffer);
                        fsciBleGapCallApiFunction(Gap_StopPeriodicAdvertising(handle));
                    }
                    break;

                case gBleGapCmdPeriodicAdvCreateSyncOpCode_c:
                    {
                        gapPeriodicAdvSyncReq_t pReq;
                        fsciBleGapGetPeriodicAdvSyncReqFromBuffer(&pReq, &pBuffer);
                        fsciBleGapCallApiFunction(Gap_PeriodicAdvCreateSync(&pReq, fsciBleGapCallbacks.scanningCallback));
                    }
                    break;
#endif

                case (uint8_t)gBleGapCmdAuthorizeOpCode_c:
                    {
                        deviceId_t          deviceId;
                        uint16_t            handle;
                        gattDbAccessType_t  access;

                        /* Get command parameters from buffer */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);
                        fsciBleGetUint16ValueFromBuffer(handle, pBuffer);
                        fsciBleGetEnumValueFromBuffer(access, pBuffer, gattDbAccessType_t);

                        fsciBleGapCallApiFunction(Gap_Authorize(deviceId, handle, access));
                    }
                    break;

                case (uint8_t)gBleGapCmdSaveCccdOpCode_c:
                    {
                        deviceId_t      deviceId;
                        uint16_t        handle;
                        gattCccdFlags_t cccd;

                        /* Get command parameters from buffer */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);
                        fsciBleGetUint16ValueFromBuffer(handle, pBuffer);
                        fsciBleGetEnumValueFromBuffer(cccd, pBuffer, gattCccdFlags_t);

                        fsciBleGapCallApiFunction(Gap_SaveCccd(deviceId, handle, cccd));
                    }
                    break;

                case (uint8_t)gBleGapCmdCheckNotificationStatusOpCode_c:
                    {
                        deviceId_t  deviceId;
                        uint16_t    handle;
                        bool_t      bOutIsActive;

                        /* Get command parameters from buffer */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);
                        fsciBleGetUint16ValueFromBuffer(handle, pBuffer);

                        fsciBleGapCallApiFunction(Gap_CheckNotificationStatus(deviceId, handle, &bOutIsActive));
                        fsciBleGapMonitorOutParams(CheckNotificationStatus, &bOutIsActive);
                    }
                    break;

                case (uint8_t)gBleGapCmdCheckIndicationStatusOpCode_c:
                    {
                        deviceId_t  deviceId;
                        uint16_t    handle;
                        bool_t      bOutIsActive;

                        /* Get command parameters from buffer */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);
                        fsciBleGetUint16ValueFromBuffer(handle, pBuffer);

                        fsciBleGapCallApiFunction(Gap_CheckIndicationStatus(deviceId, handle, &bOutIsActive));
                        fsciBleGapMonitorOutParams(CheckIndicationStatus, &bOutIsActive);
                    }
                    break;

                case (uint8_t)gBleGapCmdPairOpCode_c:
                    {
                        deviceId_t              deviceId;
                        gapPairingParameters_t  pairingParameters;

                        /* Get command parameters from buffer */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);
                        fsciBleGapGetPairingParametersFromBuffer(&pairingParameters, &pBuffer);

                        fsciBleGapCallApiFunction(Gap_Pair(deviceId, &pairingParameters));
                    }
                    break;

                case (uint8_t)gBleGapCmdSendSlaveSecurityRequestOpCode_c:
                    {
                        deviceId_t              deviceId;

                        /* Get command parameters from buffer */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);

#if gFsciBleTest_d
                        /* Just in test mode, use the previously set default pairing parameters */
                        if (mDefaultGapPairingActive)
                        {
                            fsciBleGapCallApiFunction(Gap_SendSlaveSecurityRequest(deviceId, &mDefaultGapPairingParameters));
                        }
                        else
                        {
                            fsciBleGapStatusMonitor(gBleInvalidState_c);
                        }
#else
                        gapPairingParameters_t  pairingParameters;
                        fsciBleGapGetPairingParametersFromBuffer(&pairingParameters, &pBuffer);
                        fsciBleGapCallApiFunction(Gap_SendSlaveSecurityRequest(deviceId, &pairingParameters));
#endif
                    }
                    break;

                case (uint8_t)gBleGapCmdEncryptLinkOpCode_c:
                    {
                        deviceId_t deviceId;

                        /* Get deviceId parameter from buffer */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);

                        fsciBleGapCallApiFunction(Gap_EncryptLink(deviceId));
                    }
                    break;

                case (uint8_t)gBleGapCmdAcceptPairingRequestOpCode_c:
                    {
                        deviceId_t              deviceId;
                        gapPairingParameters_t  pairingParameters;

                        /* Get command parameters from buffer */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);
                        fsciBleGapGetPairingParametersFromBuffer(&pairingParameters, &pBuffer);

                        fsciBleGapCallApiFunction(Gap_AcceptPairingRequest(deviceId, &pairingParameters));
                    }
                    break;

                case (uint8_t)gBleGapCmdRejectPairingOpCode_c:
                    {
                        deviceId_t                      deviceId;
                        gapAuthenticationRejectReason_t reason;

                        /* Get command parameters from buffer */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);
                        fsciBleGetEnumValueFromBuffer(reason, pBuffer, gapAuthenticationRejectReason_t);

                        fsciBleGapCallApiFunction(Gap_RejectPairing(deviceId, reason));
                    }
                    break;

                case (uint8_t)gBleGapCmdEnterPasskeyOpCode_c:
                    {
                        deviceId_t  deviceId;
                        uint32_t    passkey;

                        /* Get command parameters from buffer */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);
                        fsciBleGetUint32ValueFromBuffer(passkey, pBuffer);

                        fsciBleGapCallApiFunction(Gap_EnterPasskey(deviceId, passkey));
                    }
                    break;

                case (uint8_t)gBleGapCmdProvideOobOpCode_c:
                    {
                        deviceId_t  deviceId;
                        uint8_t     oob[gcSmpOobSize_c];

                        /* Get command parameters from buffer */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);
                        fsciBleGetOobFromBuffer(oob, pBuffer);

                        fsciBleGapCallApiFunction(Gap_ProvideOob(deviceId, oob));
                    }
                    break;

                case (uint8_t)gBleGapCmdRejectPasskeyRequestOpCode_c:
                    {
                        deviceId_t deviceId;

                        /* Get deviceId from buffer */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);

                        fsciBleGapCallApiFunction(Gap_RejectPasskeyRequest(deviceId));
                    }
                    break;

                case (uint8_t)gBleGapCmdSendSmpKeysOpCode_c:
                    {
                        deviceId_t      deviceId;
                        gapSmpKeys_t*   pSmpKeys;

                        /* Get command parameters from buffer */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);

                        /* Allocate buffer for SMP keys */
                        pSmpKeys = fsciBleGapAllocSmpKeysForBuffer(pBuffer);

                        if(NULL == pSmpKeys)
                        {
                            /* No memory => the GAP command can not be executed */
                            fsciBleError(gFsciOutOfMessages_c, fsciInterfaceId);
                        }
                        else
                        {
                            /* Get SMP keys from buffer */
                            fsciBleGapGetSmpKeysFromBuffer(pSmpKeys, &pBuffer);

                            fsciBleGapCallApiFunction(Gap_SendSmpKeys(deviceId, pSmpKeys));

                            /* Free the buffer allocated for SMP keys */
                            (void)fsciBleGapFreeSmpKeys(pSmpKeys);
                        }
                    }
                    break;

                case (uint8_t)gBleGapCmdRejectKeyExchangeRequestOpCode_c:
                    {
                        deviceId_t deviceId;

                        /* Get deviceId from buffer */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);

                        fsciBleGapCallApiFunction(Gap_RejectKeyExchangeRequest(deviceId));
                    }
                    break;

                case (uint8_t)gBleGapCmdProvideLongTermKeyOpCode_c:
                    {
                        deviceId_t  deviceId;
                        uint8_t     ltkSize;
                        uint8_t*    pLtk;

                        /* Get command parameters from buffer */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);
                        fsciBleGetUint8ValueFromBuffer(ltkSize, pBuffer);

                        /* Allocate buffer for LTK (consider that ltkSize is
                        bigger than 0) */
                        pLtk = MEM_BufferAlloc(ltkSize);

                        if(NULL == pLtk)
                        {
                            /* No memory => The GAP command can not be executed */
                            fsciBleError(gFsciOutOfMessages_c, fsciInterfaceId);
                        }
                        else
                        {
                            /* Get LTK from buffer */
                            fsciBleGetArrayFromBuffer(pLtk, pBuffer, ltkSize);

                            fsciBleGapCallApiFunction(Gap_ProvideLongTermKey(deviceId, pLtk, ltkSize));

                            /* Free the buffer allocated for LTK */
                            (void)MEM_BufferFree(pLtk);
                        }
                    }
                    break;

                case (uint8_t)gBleGapCmdDenyLongTermKeyOpCode_c:
                    {
                        deviceId_t deviceId;

                        /* Get deviceId from buffer */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);

                        fsciBleGapCallApiFunction(Gap_DenyLongTermKey(deviceId));
                    }
                    break;

                case (uint8_t)gBleGapCmdLoadEncryptionInformationOpCode_c:
                    {
                        deviceId_t  deviceId;
                        uint8_t*    pOutLtk;
                        uint8_t     outLtkSize;

                        /* Get deviceId from buffer */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);

                        /* Allocate buffer for LTK (maximum LTK size) */
                        pOutLtk = MEM_BufferAlloc(gcSmpMaxLtkSize_c);

                        if(NULL == pOutLtk)
                        {
                            /* No memory => the GAP command can not be executed */
                            fsciBleError(gFsciOutOfMessages_c, fsciInterfaceId);
                        }
                        else
                        {
                            fsciBleGapCallApiFunction(Gap_LoadEncryptionInformation(deviceId, pOutLtk, &outLtkSize));
                            fsciBleGapMonitorOutParams(LoadEncryptionInformation, pOutLtk, &outLtkSize);

                            /* Free the buffer allocated for LTK */
                            (void)MEM_BufferFree(pOutLtk);
                        }
                    }
                    break;

                case (uint8_t)gBleGapCmdSetLocalPasskeyOpCode_c:
                    {
                        uint32_t passkey;

                        /* Get passkey from buffer */
                        fsciBleGetUint32ValueFromBuffer(passkey, pBuffer);

                        fsciBleGapCallApiFunction(Gap_SetLocalPasskey(passkey));
                    }
                    break;

                case (uint8_t)gBleGapCmdStartScanningOpCode_c:
                    {
                        bool_t                      bScanningParametersIncluded;
                        gapFilterDuplicates_t       enableFilterDuplicates = gGapDuplicateFilteringEnable_c;
                        uint16_t                    duration = 0, period = 0;
                        gapScanningParameters_t     scanningParameters;
                        gapScanningParameters_t*    pScanningParameters = NULL;

                        /* Get from buffer the boolean value which indicates if scanning
                        parameters are included in the request */
                        fsciBleGetBoolValueFromBuffer(bScanningParametersIncluded, pBuffer);

                        /* Verify if scanning parameters are included in the request or not */
                        if(TRUE == bScanningParametersIncluded)
                        {
                            /* Get the scanning parameters from buffer */
                            fsciBleGapGetScanningParametersFromBuffer(&scanningParameters, &pBuffer);
                            pScanningParameters = &scanningParameters;
                        }

                        /* Get from buffer the enum value which enables or disables
                           duplicate advertising reports filtering */
                        fsciBleGetEnumValueFromBuffer(enableFilterDuplicates, pBuffer, gapFilterDuplicates_t);

                        /* Get the scanning PHYs */
                        fsciBleGetEnumValueFromBuffer(scanningParameters.scanningPHYs, pBuffer, gapLePhyFlags_t);

                        /* Get from buffer  the scan duration and period */
                        fsciBleGetUint16ValueFromBuffer(duration, pBuffer);
                        fsciBleGetUint16ValueFromBuffer(period, pBuffer);

                        fsciBleGapCallApiFunction(Gap_StartScanning(pScanningParameters,fsciBleGapCallbacks.scanningCallback, enableFilterDuplicates,
                                                                    duration, period));
                    }
                    break;

                case (uint8_t)gBleGapCmdStopScanningOpCode_c:
                    {
                        fsciBleGapCallApiFunction(Gap_StopScanning());
                    }
                    break;

                case (uint8_t)gBleGapCmdConnectOpCode_c:
                    {
                        gapConnectionRequestParameters_t connectionRequestParameters;

                        /* Get connection request parameters from buffer */
                        fsciBleGapGetConnectionRequestParametersFromBuffer(&connectionRequestParameters, &pBuffer);

                        fsciBleGapCallApiFunction(Gap_Connect(&connectionRequestParameters, fsciBleGapCallbacks.connectionCallback));
                    }
                    break;

                case (uint8_t)gBleGapCmdDisconnectOpCode_c:
                    {
                        deviceId_t deviceId;

                        /* Get deviceId from buffer */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);

                        fsciBleGapCallApiFunction(Gap_Disconnect(deviceId));
                    }
                    break;

                case (uint8_t)gBleGapCmdSaveCustomPeerInformationOpCode_c:
                    {
                        deviceId_t  deviceId;
                        uint16_t    offset;
                        uint16_t    infoSize;
                        uint8_t*    pInfo;

                        /* Get command parameters from buffer */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);
                        fsciBleGetUint16ValueFromBuffer(offset, pBuffer);
                        fsciBleGetUint16ValueFromBuffer(infoSize, pBuffer);

                        /* Alloc buffer for info (consider that infoSize is
                        bigger than 0) */
                        pInfo = MEM_BufferAlloc(infoSize);

                        if(NULL == pInfo)
                        {
                            /* No memory => the GAP command can not be executed */
                            fsciBleError(gFsciOutOfMessages_c, fsciInterfaceId);
                        }
                        else
                        {
                            /* Get info from buffer */
                            fsciBleGetArrayFromBuffer(pInfo, pBuffer, infoSize);

                            fsciBleGapCallApiFunction(Gap_SaveCustomPeerInformation(deviceId, pInfo, offset, infoSize));

                            /* Free the buffer allocated for info */
                            (void)MEM_BufferFree(pInfo);
                        }
                    }
                    break;

                case (uint8_t)gBleGapCmdLoadCustomPeerInformationOpCode_c:
                    {
                        deviceId_t  deviceId;
                        uint16_t    offset;
                        uint16_t    infoSize;
                        uint8_t*    pOutInfo;

                        /* Get command parameters from buffer */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);
                        fsciBleGetUint16ValueFromBuffer(offset, pBuffer);
                        fsciBleGetUint16ValueFromBuffer(infoSize, pBuffer);

                        /* Alloc buffer for info (consider that infoSize is
                        bigger than 0) */
                        pOutInfo = MEM_BufferAlloc(infoSize);

                        if(NULL == pOutInfo)
                        {
                            /* No memory => the GAP command can not be executed */
                            fsciBleError(gFsciOutOfMessages_c, fsciInterfaceId);
                        }
                        else
                        {
                            fsciBleGapCallApiFunction(Gap_LoadCustomPeerInformation(deviceId, pOutInfo, offset, infoSize));
                            fsciBleGapMonitorOutParams(LoadCustomPeerInfo, pOutInfo, infoSize);

                            /* Free the buffer allocated for info */
                            (void)MEM_BufferFree(pOutInfo);
                        }
                    }
                    break;

                case (uint8_t)gBleGapCmdCheckIfBondedOpCode_c:
                    {
                        deviceId_t  deviceId;
                        bool_t      bOutIsBonded;

                        /* Get deviceId from buffer */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);

                        fsciBleGapCallApiFunction(Gap_CheckIfBonded(deviceId, &bOutIsBonded));
                        fsciBleGapMonitorOutParams(CheckIfBonded, &bOutIsBonded);
                    }
                    break;

                case (uint8_t)gBleGapCmdReadWhiteListSizeOpCode_c:
                    {
                        fsciBleGapCallApiFunction(Gap_ReadWhiteListSize());
                    }
                    break;

                case (uint8_t)gBleGapCmdClearWhiteListOpCode_c:
                    {
                        fsciBleGapCallApiFunction(Gap_ClearWhiteList());
                    }
                    break;

                case (uint8_t)gBleGapCmdAddDeviceToWhiteListOpCode_c:
                    {
                        bleAddressType_t    addressType;
                        bleDeviceAddress_t  address;

                        /* Get command parameters from buffer */
                        fsciBleGetEnumValueFromBuffer(addressType, pBuffer, bleAddressType_t);
                        fsciBleGetAddressFromBuffer(address, pBuffer);

                        fsciBleGapCallApiFunction(Gap_AddDeviceToWhiteList(addressType, address));
                    }
                    break;

                case (uint8_t)gBleGapCmdRemoveDeviceFromWhiteListOpCode_c:
                    {
                        bleAddressType_t    addressType;
                        bleDeviceAddress_t  address;

                        /* Get command parameters from buffer */
                        fsciBleGetEnumValueFromBuffer(addressType, pBuffer, bleAddressType_t);
                        fsciBleGetAddressFromBuffer(address, pBuffer);

                        fsciBleGapCallApiFunction(Gap_RemoveDeviceFromWhiteList(addressType, address));
                    }
                    break;

                case (uint8_t)gBleGapCmdReadPublicDeviceAddressOpCode_c:
                    {
                        fsciBleGapCallApiFunction(Gap_ReadPublicDeviceAddress());
                    }
                    break;

                case (uint8_t)gBleGapCmdCreateRandomDeviceAddressOpCode_c:
                    {
                        bool_t      bIrkIncluded;
                        uint8_t     irk[gcSmpIrkSize_c];
                        uint8_t*    pIrk        = NULL;
                        bool_t      bRandomPartIncluded;
                        uint8_t     randomPart[3];
                        uint8_t*    pRandomPart = NULL;

                        /* Get from buffer the boolean value which indicates if IRK
                        is included in the request */
                        fsciBleGetBoolValueFromBuffer(bIrkIncluded, pBuffer);

                        /* Verify if IRK is included in the request or not */
                        if(TRUE == bIrkIncluded)
                        {
                            /* Get IRK from buffer */
                            fsciBleGetIrkFromBuffer(irk, pBuffer);
                            pIrk = irk;

                            /* Get from buffer the boolean value which indicates if random
                            part is included in the request */
                            fsciBleGetBoolValueFromBuffer(bRandomPartIncluded, pBuffer);

                            /* Verify if random part is included in the request or not */
                            if(TRUE == bRandomPartIncluded)
                            {
                                /* Get random part (3 bytes) from buffer */
                                fsciBleGetArrayFromBuffer(randomPart, pBuffer, 3);
                                pRandomPart = randomPart;
                            }
                        }

                        fsciBleGapCallApiFunction(Gap_CreateRandomDeviceAddress(pIrk, pRandomPart));
                    }
                    break;

                case (uint8_t)gBleGapCmdSaveDeviceNameOpCode_c:
                    {
                        deviceId_t  deviceId;
                        uint8_t     nameSize;
                        uchar_t*    pName;

                        /* Get command parameters from buffer */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);
                        fsciBleGetUint8ValueFromBuffer(nameSize, pBuffer);

                        /* Alloc buffer for name (consider that nameSize
                        is bigger than 0) */
                        pName = MEM_BufferAlloc(nameSize);

                        if(NULL == pName)
                        {
                            /* No memory => the GAP command can not be executed */
                            fsciBleError(gFsciOutOfMessages_c, fsciInterfaceId);
                        }
                        else
                        {
                            /* Get name from buffer */
                            fsciBleGetArrayFromBuffer(pName, pBuffer, nameSize);

                            fsciBleGapCallApiFunction(Gap_SaveDeviceName(deviceId, pName, nameSize));

                            /* Free buffer allocated for name */
                            (void)MEM_BufferFree(pName);
                        }
                    }
                    break;

                case (uint8_t)gBleGapCmdGetBondedDevicesCountOpCode_c:
                    {
                        uint8_t outBondedDevicesCount;

                        fsciBleGapCallApiFunction(Gap_GetBondedDevicesCount(&outBondedDevicesCount));
                        fsciBleGapMonitorOutParams(GetBondedDevicesCount, &outBondedDevicesCount);
                    }
                    break;

                case (uint8_t)gBleGapCmdGetBondedDeviceNameOpCode_c:
                    {
                        uint8_t     nvmIndex;
                        uint8_t     maxNameSize;
                        uchar_t*    pOutName;

                        /* Get command parameters from buffer */
                        fsciBleGetUint8ValueFromBuffer(nvmIndex, pBuffer);
                        fsciBleGetUint8ValueFromBuffer(maxNameSize, pBuffer);

                        /* Alloc buffer for name (consider that nameSize
                        is bigger than 0) */
                        pOutName = MEM_BufferAlloc(maxNameSize);

                        if(NULL == pOutName)
                        {
                            /* No memory => the GAP command can not be executed */
                            fsciBleError(gFsciOutOfMessages_c, fsciInterfaceId);
                        }
                        else
                        {
                            fsciBleGapCallApiFunction(Gap_GetBondedDeviceName(nvmIndex, pOutName, maxNameSize));
                            fsciBleGapMonitorOutParams(GetBondedDeviceName, pOutName);

                            /* Free buffer allocated for name */
                            (void)MEM_BufferFree(pOutName);
                        }
                    }
                    break;

                case (uint8_t)gBleGapCmdRemoveBondOpCode_c:
                    {
                        uint8_t nvmIndex;

                        /* Get nvmIndex from buffer */
                        fsciBleGetUint8ValueFromBuffer(nvmIndex, pBuffer);

                        fsciBleGapCallApiFunction(Gap_RemoveBond(nvmIndex));
                    }
                    break;

                case (uint8_t)gBleGapCmdRemoveAllBondsOpCode_c:
                    {
                        fsciBleGapCallApiFunction(Gap_RemoveAllBonds());
                    }
                    break;

                case (uint8_t)gBleGapCmdReadRadioPowerLevelOpCode_c:
                    {
                        gapRadioPowerLevelReadType_t    txReadType;
                        deviceId_t                      deviceId;

                        /* Get command parameters from buffer */
                        fsciBleGetEnumValueFromBuffer(txReadType, pBuffer, gapRadioPowerLevelReadType_t);
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);

                        fsciBleGapCallApiFunction(Gap_ReadRadioPowerLevel(txReadType, deviceId));
                    }
                    break;

                case (uint8_t)gBleGapCmdVerifyPrivateResolvableAddressOpCode_c:
                    {
                        uint8_t             nvmIndex;
                        bleDeviceAddress_t  address;

                        /* Get command parameters from buffer */
                        fsciBleGetUint8ValueFromBuffer(nvmIndex, pBuffer);
                        fsciBleGetAddressFromBuffer(address, pBuffer);

                        fsciBleGapCallApiFunction(Gap_VerifyPrivateResolvableAddress(nvmIndex, address));
                    }
                    break;

                case (uint8_t)gBleGapCmdSetRandomAddressOpCode_c:
                    {
                        bleDeviceAddress_t address;

                        /* Get address from buffer */
                        fsciBleGetAddressFromBuffer(address, pBuffer);

                        fsciBleGapCallApiFunction(Gap_SetRandomAddress(address));
                    }
                    break;

                case (uint8_t)gBleGapCmdSetScanModeOpCode_c:
                    {
                        gapScanMode_t scanMode;

                        /* Get scanMode parameter from buffer */
                        fsciBleGetEnumValueFromBuffer(scanMode, pBuffer, gapScanMode_t);

                        /* Free the oldest pAutoConnectParams */
                        (void)fsciBleGapFreeAutoConnectParams(pAutoConnectParams);
                        pAutoConnectParams = NULL;

                        if(gAutoConnect_c == scanMode)
                        {
                            /* Alloc buffer for pAutoConnectParams */
                            pAutoConnectParams = fsciBleGapAllocAutoConnectParamsForBuffer(pBuffer);

                            if(NULL == pAutoConnectParams)
                            {
                                /* No memory => the GAP command can not be executed */
                                fsciBleError(gFsciOutOfMessages_c, fsciInterfaceId);
                            }
                            else
                            {
                                /* Get pAutoConnectParams from buffer */
                                fsciBleGapGetAutoConnectParamsFromBuffer(pAutoConnectParams, &pBuffer);

                                fsciBleGapCallApiFunction(Gap_SetScanMode(scanMode, pAutoConnectParams, fsciBleGapConnectionCallback));
                            }
                        }
                        else
                        {
                            fsciBleGapCallApiFunction(Gap_SetScanMode(scanMode, NULL, NULL));
                        }
                    }
                    break;

                case (uint8_t)gBleGapCmdSetDefaultPairingParameters_c:
                    {
                        bool_t                  bPairingParametersIncluded;
                        gapPairingParameters_t  pairingParameters;
                        gapPairingParameters_t* pPairingParameters = NULL;

                        /* Get bPairingParametersIncluded from buffer */
                        fsciBleGetBoolValueFromBuffer(bPairingParametersIncluded, pBuffer);

                        if(TRUE == bPairingParametersIncluded)
                        {
                            /* Get pPairingParameters from buffer */
                            fsciBleGapGetPairingParametersFromBuffer(&pairingParameters, &pBuffer);
                            pPairingParameters = &pairingParameters;
                        }

#if gFsciBleTest_d
                        /* Just in test mode, save pairing parameters to be used on Gap_SendSlaveSecurityRequest */
                        mDefaultGapPairingActive = bPairingParametersIncluded;

                        if (mDefaultGapPairingActive)
                        {
                            mDefaultGapPairingParameters = *pPairingParameters;
                        }
#endif

                        fsciBleGapCallApiFunction(Gap_SetDefaultPairingParameters(pPairingParameters));

                    }
                    break;

                case (uint8_t)gBleGapCmdUpdateConnectionParametersOpCode_c:
                    {
                        deviceId_t  deviceId;
                        uint16_t    intervalMin;
                        uint16_t    intervalMax;
                        uint16_t    slaveLatency;
                        uint16_t    timeoutMultiplier;
                        uint16_t    minCeLength;
                        uint16_t    maxCeLength;

                        /* Get command parameters from the received packet */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);
                        fsciBleGetUint16ValueFromBuffer(intervalMin, pBuffer);
                        fsciBleGetUint16ValueFromBuffer(intervalMax, pBuffer);
                        fsciBleGetUint16ValueFromBuffer(slaveLatency, pBuffer);
                        fsciBleGetUint16ValueFromBuffer(timeoutMultiplier, pBuffer);
                        fsciBleGetUint16ValueFromBuffer(minCeLength, pBuffer);
                        fsciBleGetUint16ValueFromBuffer(maxCeLength, pBuffer);

                        fsciBleGapCallApiFunction(Gap_UpdateConnectionParameters(deviceId, intervalMin, intervalMax, slaveLatency, timeoutMultiplier, minCeLength, maxCeLength));
                    }
                    break;

                case (uint8_t)gBleGapCmdEnableUpdateConnectionParametersOpCode_c:
                    {
                        deviceId_t  deviceId;
                        bool_t      bEnable;

                        /* Get command parameters from the received packet */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);
                        fsciBleGetBoolValueFromBuffer(bEnable, pBuffer);

                        fsciBleGapCallApiFunction(Gap_EnableUpdateConnectionParameters(deviceId, bEnable));
                    }
                    break;

                case (uint8_t)gBleGapCmdUpdateLeDataLengthOpCode_c:
                    {
                        uint16_t    txOctets;
                        uint16_t    txTime;
                        deviceId_t  deviceId;

                        /* Get command parameters from buffer */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);
                        fsciBleGetUint16ValueFromBuffer(txOctets, pBuffer);
                        fsciBleGetUint16ValueFromBuffer(txTime, pBuffer);

                        fsciBleGapCallApiFunction(Gap_UpdateLeDataLength(deviceId, txOctets, txTime));
                    }
                    break;

                case (uint8_t)gBleGapCmdEnableHostPrivacyOpCode_c:
                    {
                        uint8_t* pIrk = NULL;
                        uint8_t  irk[gcSmpIrkSize_c];
                        bool_t   enable = FALSE;

                        /* Get command parameters from buffer */
                        fsciBleGetBoolValueFromBuffer(enable, pBuffer);

                        /* Verify if IRK is included in the request or not */
                        if( TRUE == enable )
                        {
                            /* Get IRK from buffer */
                            fsciBleGetIrkFromBuffer(irk, pBuffer);
                            pIrk = irk;
                        }

                        fsciBleGapCallApiFunction(Gap_EnableHostPrivacy(enable, pIrk));
                    }
                    break;

                case (uint8_t)gBleGapCmdEnableControllerPrivacyOpCode_c:
                    {
                        gapIdentityInformation_t* pPeerIdentities = NULL;
                        gapIdentityInformation_t* pBufferPeerIdentities = NULL;
                        uint32_t                  i = 0;
                        uint8_t                   ownIrk[gcSmpIrkSize_c];
                        bool_t                    enable = FALSE;
                        uint8_t                   peerIdCount = 0;

                        /* Get command parameters from buffer */
                        fsciBleGetBoolValueFromBuffer(enable, pBuffer);

                        /* Verify if parameters are included in the request or not */
                        if( TRUE == enable )
                        {
                            /* Get IRK from buffer */
                            fsciBleGetIrkFromBuffer(ownIrk, pBuffer);

                            fsciBleGetUint8ValueFromBuffer(peerIdCount, pBuffer);

                            if( peerIdCount != 0U)
                            {
                                /* Allocate memory for gapIdentityInformation_t */
                                pPeerIdentities = (gapIdentityInformation_t*)MEM_BufferAlloc((uint32_t)peerIdCount * sizeof(gapIdentityInformation_t));

                                if( NULL == pPeerIdentities )
                                {
                                    fsciBleError(gFsciOutOfMessages_c, fsciInterfaceId);
                                }
                                else
                                {
                                    pBufferPeerIdentities = pPeerIdentities;

                                    for( i=0; i<peerIdCount; i++ )
                                    {
                                        fsciBleGapGetIdentityInformationFromBuffer(pBufferPeerIdentities, &pBuffer);
                                        pBufferPeerIdentities++;
                                    }

                                    fsciBleGapCallApiFunction(Gap_EnableControllerPrivacy(enable, ownIrk, peerIdCount, pPeerIdentities));

                                    /* Free gapIdentityInformation_t allocated memory */
                                    (void)MEM_BufferFree(pPeerIdentities);
                                }
                            }
                            else
                            {
                                fsciBleGapCallApiFunction(Gap_EnableControllerPrivacy(TRUE, NULL, 0, NULL));
                            }
                        }
                        else
                        {
                            fsciBleGapCallApiFunction(Gap_EnableControllerPrivacy(FALSE, NULL, 0, NULL));
                        }
                    }
                    break;

                case (uint8_t)gBleGapCmdSetPrivacyModeOpCode_c:
                    {
                        uint8_t             nvmIndex;
                        blePrivacyMode_t    privacyMode;

                        /* Get command parameters from buffer */
                        fsciBleGetUint8ValueFromBuffer(nvmIndex, pBuffer);
                        fsciBleGetEnumValueFromBuffer(privacyMode, pBuffer, blePrivacyMode_t);
                        fsciBleGapCallApiFunction(Gap_SetPrivacyMode(nvmIndex, privacyMode));
                    }
                    break;

                case (uint8_t)gBleGapCmdLeScRegeneratePublicKeyOpCode_c:
                    {
                        fsciBleGapCallApiFunction(Gap_LeScRegeneratePublicKey());
                    }
                    break;

                case (uint8_t)gBleGapCmdLeScValidateNumericValueOpCode_c:
                    {
                        deviceId_t deviceId;
                        bool_t     valid;

                        /* Get command parameters from buffer */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);
                        fsciBleGetBoolValueFromBuffer(valid, pBuffer);

                        fsciBleGapCallApiFunction(Gap_LeScValidateNumericValue(deviceId, valid));
                    }
                    break;

                case (uint8_t)gBleGapCmdLeScGetLocalOobDataOpCode_c:
                    {
                        fsciBleGapCallApiFunction(Gap_LeScGetLocalOobData());
                    }
                    break;

                case (uint8_t)gBleGapCmdLeScSetPeerOobDataOpCode_c:
                    {
                        gapLeScOobData_t leScOObData;
                        deviceId_t       deviceId;

                        /* Get command parameters from buffer */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);
                        fsciBleGetArrayFromBuffer(leScOObData.randomValue, pBuffer, gSmpLeScRandomValueSize_c);
                        fsciBleGetArrayFromBuffer(leScOObData.confirmValue, pBuffer, gSmpLeScRandomConfirmValueSize_c);

                        fsciBleGapCallApiFunction(Gap_LeScSetPeerOobData(deviceId, &leScOObData));
                    }
                    break;

                case (uint8_t)gBleGapCmdLeScSendKeypressNotificationPrivacyOpCode_c:
                    {
                        deviceId_t                deviceId;
                        gapKeypressNotification_t keypressNotification;

                        /* Get command parameters from buffer */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);
                        fsciBleGetEnumValueFromBuffer(keypressNotification, pBuffer, gapKeypressNotification_t);

                        fsciBleGapCallApiFunction(Gap_LeScSendKeypressNotification(deviceId, keypressNotification));
                    }
                    break;

                case (uint8_t)gBleGapCmdGetBondedDevicesIdentityInformationOpCode_c:
                    {
                        gapIdentityInformation_t* pOutIdentityAddresses;
                        uint8_t                   maxDevices;
                        uint8_t                   outActualCount;

                        /* Get command parameters from buffer */
                        fsciBleGetUint8ValueFromBuffer(maxDevices, pBuffer);

                        /* Allocate buffer for the device addresses array (consider that
                        maxDevices is bigger than 0) */
                        pOutIdentityAddresses = (gapIdentityInformation_t*)MEM_BufferAlloc((uint32_t)maxDevices * sizeof(gapIdentityInformation_t));

                        if(NULL == pOutIdentityAddresses)
                        {
                            /* No memory => the GAP command can not be executed */
                            fsciBleError(gFsciOutOfMessages_c, fsciInterfaceId);
                        }
                        else
                        {
                           fsciBleGapCallApiFunction(Gap_GetBondedDevicesIdentityInformation(pOutIdentityAddresses, maxDevices, &outActualCount));
                           fsciBleGapMonitorOutParams(GetBondedDevIdentityInfo, pOutIdentityAddresses, &outActualCount);

                           /* Free the buffer allocated for the device addresses array */
                           (void)MEM_BufferFree(pOutIdentityAddresses);
                        }
                    }
                    break;

                case (uint8_t)gBleGapCmdSetTxPowerLevelOpCode_c:
                    {
                        uint8_t                       powerLevel;
                        bleTransmitPowerChannelType_t channelType;

                        fsciBleGetUint8ValueFromBuffer(powerLevel, pBuffer);
                        fsciBleGetEnumValueFromBuffer(channelType, pBuffer, bleTransmitPowerChannelType_t);

                        fsciBleGapCallApiFunction(Gap_SetTxPowerLevel(powerLevel, channelType));
                    }
                    break;

                case (uint8_t)gBleGapCmdLeReadPhyOpCode_c:
                    {
                        deviceId_t deviceId;

                        /* Get command parameters from buffer */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);

                        fsciBleGapCallApiFunction(Gap_LeReadPhy(deviceId));
                    }
                    break;

                case (uint8_t)gBleGapCmdLeSetPhyOpCode_c:
                    {
                        bool_t      defaultMode;
                        deviceId_t  deviceId;
                        uint8_t     allPhys;
                        uint8_t     txPhys;
                        uint8_t     rxPhys;
                        uint16_t    phyOptions;

                        /* Get command parameters from buffer */
                        fsciBleGetBoolValueFromBuffer(defaultMode, pBuffer);
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);
                        fsciBleGetUint8ValueFromBuffer(allPhys, pBuffer);
                        fsciBleGetUint8ValueFromBuffer(txPhys, pBuffer);
                        fsciBleGetUint8ValueFromBuffer(rxPhys, pBuffer);
                        fsciBleGetUint16ValueFromBuffer(phyOptions, pBuffer);

                        fsciBleGapCallApiFunction(Gap_LeSetPhy(defaultMode, deviceId, allPhys, txPhys, rxPhys, phyOptions));
                    }
                    break;

                case (uint8_t)gBleGapCmdControllerNotificationOpCode_c:
                    {
                        uint16_t   eventType;
                        deviceId_t deviceId;

                        /* Get command parameters from buffer */
                        fsciBleGetUint16ValueFromBuffer(eventType, pBuffer);
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);

                        fsciBleGapCallApiFunction(Gap_ControllerEnhancedNotification(eventType, deviceId));
                    }
                    break;

                case (uint8_t)gBleGapCmdLoadKeysOpCode_c:
                    {
                        uint8_t       nvmIndex;
                        gapSmpKeys_t *pKeys = NULL;
                        gapSmpKeyFlags_t keyFlags;
                        bool_t        leSc;
                        bool_t        auth;

                        /* Get command parameters from buffer */
                        fsciBleGetUint8ValueFromBuffer(nvmIndex, pBuffer);

                        /* Allocate buffer for SMP keys */
                        pKeys = fsciBleGapAllocSmpKeys();

                        if (NULL == pKeys)
                        {
                            /* No memory => the GAP command can not be executed */
                            fsciBleError(gFsciOutOfMessages_c, fsciBleInterfaceId);
                        }
                        else
                        {
                            fsciBleGapCallApiFunction(Gap_LoadKeys(nvmIndex, pKeys, &keyFlags, &leSc, &auth));

                            /* Set keys to NULL if not used. */
                            if ((keyFlags & gLtk_c) == 0U)  { pKeys->aLtk = NULL; }
                            if ((keyFlags & gIrk_c) == 0U)  { pKeys->aIrk = NULL; }
                            if ((keyFlags & gCsrk_c) == 0U) { pKeys->aCsrk = NULL; }

                            fsciBleGapMonitorOutParams(LoadKeys, pKeys, &keyFlags, &leSc, &auth);

                            /* Free the buffer allocated for SMP keys */
                            (void)fsciBleGapFreeSmpKeys(pKeys);
                        }
                    }
                    break;

                case (uint8_t)gBleGapCmdSaveKeysOpCode_c:
                    {
                        uint8_t       nvmIndex;
                        gapSmpKeys_t *pKeys;
                        bool_t        leSc;
                        bool_t        auth;

                        /* Get command parameters from buffer */
                        fsciBleGetUint8ValueFromBuffer(nvmIndex, pBuffer);

                        /* Allocate buffer for SMP keys */
                        pKeys = fsciBleGapAllocSmpKeysForBuffer(pBuffer);

                        if (NULL == pKeys)
                        {
                            /* No memory => the GAP command can not be executed */
                            fsciBleError(gFsciOutOfMessages_c, fsciBleInterfaceId);
                        }
                        else
                        {
                            /* Get SMP keys from buffer */
                            fsciBleGapGetSmpKeysFromBuffer(pKeys, &pBuffer);

                            fsciBleGetBoolValueFromBuffer(leSc, pBuffer);
                            fsciBleGetBoolValueFromBuffer(auth, pBuffer);

                            fsciBleGapCallApiFunction(Gap_SaveKeys(nvmIndex, pKeys, leSc, auth));

                            /* Free the buffer allocated for SMP keys */
                            (void)fsciBleGapFreeSmpKeys(pKeys);
                        }
                    }
                    break;

                case (uint8_t)gBleGapCmdSetChannelMapOpCode_c:
                    {
                        bleChannelMap_t channelMap;

                        /* Get command parameters from buffer */
                        fsciBleGetArrayFromBuffer(channelMap, pBuffer, gcBleChannelMapSize_c);

                        fsciBleGapCallApiFunction(Gap_SetChannelMap(channelMap));
                    }
                    break;

                case (uint8_t)gBleGapCmdReadChannelMapOpCode_c:
                    {
                        deviceId_t deviceId;

                        /* Get command parameters from buffer */
                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);

                        fsciBleGapCallApiFunction(Gap_ReadChannelMap(deviceId));
                    }
                    break;

#if defined(gBLE50_d) && (gBLE50_d == 1U)
                case gBleGapCmdSetPeriodicAdvParametersOpCode_c:
                    {
                        gapPeriodicAdvParameters_t advertisingParameters;

                        /* Get advertising parameters from buffer */
                        fsciBleGapGetPeriodicAdvParametersFromBuffer(&advertisingParameters, &pBuffer);

                        fsciBleGapCallApiFunction(Gap_SetPeriodicAdvParameters(&advertisingParameters));
                    }
                    break;
#endif

#endif /* gFsciBleBBox_d || gFsciBleTest_d */

#if gFsciBleHost_d
                case gBleGapStatusOpCode_c:
                    {
                        bleResult_t status;

                        /* Get status from buffer */
                        fsciBleGetEnumValueFromBuffer(status, pBuffer, bleResult_t);

                        if(gBleSuccess_c != status)
                        {
                            /* Clean out parameters pointers kept in FSCI */
                            fsciBleGapCleanOutParams();
                        }
                    }
                    break;

                case gBleGapEvtCheckNotificationStatusOpCode_c:
                    {
                        fsciBleGapCheckNotificationStatusOutParams_t* pOutParams = (fsciBleGapCheckNotificationStatusOutParams_t*)fsciBleGapRestoreOutParams();

                        if(NULL != pOutParams->pIsActive)
                        {
                            /* Get out parameter of the Gap_CheckNotificationStatus function from buffer */
                            fsciBleGetBoolValueFromBuffer(*pOutParams->pIsActive, pBuffer);

                            /* Clean out parameters pointers kept in FSCI */
                            fsciBleGapCleanOutParams();

                            /* Signal out parameters ready */
                            Ble_OutParamsReady();
                        }
                    }
                    break;

                case gBleGapEvtCheckIndicationStatusOpCode_c:
                    {
                        fsciBleGapCheckIndicationStatusOutParams_t* pOutParams = (fsciBleGapCheckIndicationStatusOutParams_t*)fsciBleGapRestoreOutParams();

                        if(NULL != pOutParams->pIsActive)
                        {
                            /* Get out parameter of the Gap_CheckIndicationStatus function from buffer */
                            fsciBleGetBoolValueFromBuffer(*pOutParams->pIsActive, pBuffer);

                            /* Clean out parameters pointers kept in FSCI */
                            fsciBleGapCleanOutParams();

                            /* Signal out parameters ready */
                            Ble_OutParamsReady();
                        }
                    }
                    break;

                case gBleGapEvtGetBondedDevicesIdentityInformationOpCode_c:
                    {
                        fsciBleGapGetBondedDevicesIdentityInformationParams_t*  pOutParams;
                        uint32_t                                                iCount;

                        pOutParams = (fsciBleGapGetBondedDevicesIdentityInformationParams_t*)fsciBleGapRestoreOutParams();

                        if(NULL != pOutParams->pActualCount)
                        {
                            /* Get number of device addresses from buffer */
                            fsciBleGetUint8ValueFromBuffer(*pOutParams->pActualCount, pBuffer);

                            if(NULL != pOutParams->pIdentityAddresses)
                            {
                                /* Get device addresses from buffer */
                                for(iCount = 0; iCount < *pOutParams->pActualCount; iCount++)
                                {
                                    fsciBleGapGetIdentityInformationFromBuffer(&pOutParams->pIdentityAddresses[iCount], &pBuffer);
                                }
                            }

                            /* Clean out parameters pointers kept in FSCI */
                            fsciBleGapCleanOutParams();

                            /* Signal out parameters ready */
                            Ble_OutParamsReady();
                        }
                    }
                    break;

                case gBleGapEvtLoadEncryptionInformationOpCode_c:
                    {
                        fsciBleGapLoadEncryptionInformationOutParams_t* pOutParams = (fsciBleGapLoadEncryptionInformationOutParams_t*)fsciBleGapRestoreOutParams();

                        /* Get out parameters of the Gap_LoadEncryptionInformation function from buffer */

                        if(NULL != pOutParams->pLtkSize)
                        {
                            fsciBleGetUint8ValueFromBuffer(*pOutParams->pLtkSize, pBuffer);
                        }

                        if(NULL != pOutParams->pLtk)
                        {
                            fsciBleGetArrayFromBuffer(pOutParams->pLtk, pBuffer, *pOutParams->pLtkSize);
                        }

                        if((NULL != pOutParams->pLtkSize) ||
                           (NULL != pOutParams->pLtk))
                        {
                            /* Clean out parameters pointers kept in FSCI */
                            fsciBleGapCleanOutParams();

                            /* Signal out parameters ready */
                            Ble_OutParamsReady();
                        }
                    }
                    break;

                case gBleGapEvtLoadCustomPeerInformationOpCode_c:
                    {
                        fsciBleGapLoadCustomPeerInformationOutParams_t* pOutParams = (fsciBleGapLoadCustomPeerInformationOutParams_t*)fsciBleGapRestoreOutParams();
                        uint16_t                                        infoSize;

                        if(NULL != pOutParams->pInfo)
                        {
                            /* Get info size from buffer */
                            fsciBleGetUint16ValueFromBuffer(infoSize, pBuffer);

                            /* Get info value from buffer */
                            fsciBleGetArrayFromBuffer(pOutParams->pInfo, pBuffer, infoSize);

                            /* Clean out parameters pointers kept in FSCI */
                            fsciBleGapCleanOutParams();

                            /* Signal out parameters ready */
                            Ble_OutParamsReady();
                        }
                    }
                    break;

                case gBleGapEvtCheckIfBondedOpCode_c:
                    {
                        fsciBleGapCheckIfBondedOutParams_t* pOutParams = (fsciBleGapCheckIfBondedOutParams_t*)fsciBleGapRestoreOutParams();

                        if(NULL != pOutParams->pIsBonded)
                        {
                            /* Get out parameter of the Gap_CheckIfBonded function from buffer */
                            fsciBleGetBoolValueFromBuffer(*pOutParams->pIsBonded, pBuffer);

                            /* Clean out parameters pointers kept in FSCI */
                            fsciBleGapCleanOutParams();

                            /* Signal out parameters ready */
                            Ble_OutParamsReady();
                        }
                    }
                    break;

                case gBleGapEvtGetBondedDevicesCountOpCode_c:
                    {
                        fsciBleGapGetBondedDevicesCountOutParams_t* pOutParams = (fsciBleGapGetBondedDevicesCountOutParams_t*)fsciBleGapRestoreOutParams();

                        if(NULL != pOutParams->pBondedDevicesCount)
                        {
                            /* Get out parameter of the Gap_GetBondedDevicesCount function from buffer */
                            fsciBleGetUint8ValueFromBuffer(*pOutParams->pBondedDevicesCount, pBuffer);

                            /* Clean out parameters pointers kept in FSCI */
                            fsciBleGapCleanOutParams();

                            /* Signal out parameters ready */
                            Ble_OutParamsReady();
                        }
                    }
                    break;

                case gBleGapEvtGetBondedDeviceNameOpCode_c:
                    {
                        fsciBleGapGetBondedDeviceNameOutParams_t*   pOutParams = (fsciBleGapGetBondedDeviceNameOutParams_t*)fsciBleGapRestoreOutParams();
                        uint16_t                                    nameSize;

                        if(NULL != pOutParams->pName)
                        {
                            /* Get out parameters of the Gap_GetBondedDeviceName function from buffer */
                            fsciBleGetUint16ValueFromBuffer(nameSize, pBuffer);
                            fsciBleGetArrayFromBuffer(pOutParams->pName, pBuffer, nameSize);

                            /* Clean out parameters pointers kept in FSCI */
                            fsciBleGapCleanOutParams();

                            /* Signal out parameters ready */
                            Ble_OutParamsReady();
                        }
                    }
                    break;

                case gBleGapEvtGenericEventInitializationCompleteOpCode_c:
                case gBleGapEvtGenericEventInternalErrorOpCode_c:
                case gBleGapEvtGenericEventAdvertisingSetupFailedOpCode_c:
                case gBleGapEvtGenericEventAdvertisingParametersSetupCompleteOpCode_c:
                case gBleGapEvtGenericEventAdvertisingDataSetupCompleteOpCode_c:
                case gBleGapEvtGenericEventWhiteListSizeReadOpCode_c:
                case gBleGapEvtGenericEventDeviceAddedToWhiteListOpCode_c:
                case gBleGapEvtGenericEventDeviceRemovedFromWhiteListOpCode_c:
                case gBleGapEvtGenericEventWhiteListClearedOpCode_c:
                case gBleGapEvtGenericEventRandomAddressReadyOpCode_c:
                case gBleGapEvtGenericEventCreateConnectionCanceledOpCode_c:
                case gBleGapEvtGenericEventPublicAddressReadOpCode_c:
                case gBleGapEvtGenericEventAdvTxPowerLevelReadOpCode_c:
                case gBleGapEvtGenericEventPrivateResolvableAddressVerifiedOpCode_c:
                case gBleGapEvtGenericEventRandomAddressSetOpCode_c:
                case gBleGapEvtGenericEventLeScPublicKeyRegeneratedOpCode_c:
                case gBleGapEvtGenericEventLeScLocalOobDataOpCode_c:
                case gBleGapEvtGenericEventHostPrivacyStateChangedOpCode_c:
                case gBleGapEvtGenericEventControllerPrivacyStateChangedOpCode_c:
                case gBleGapEvtGenericEventTxPowerLevelSetCompleteOpCode_c:
                case gBleGapEvtGenericEventLePhyEventOpCode_c:
                case gBleGapEvtGenericEventControllerNotificationOpCode_c:
                case gBleGapEvtGenericEventBondCreatedOpCode_c:
                case gBleGapEvtGenericEventChannelMapSetOpCode_c:
#if gBLE50_d
                case gBleGapEvtGenericEventExtAdvertisingDataSetupCompleteOpCode_c:
                case gBleGapEvtGenericEventPeriodicAdvCreateSyncCancelledOpCode_c:
#endif
                    {
                        gapGenericEvent_t genericEvent;

                        switch(pClientPacket->structured.header.opCode)
                        {
                            case gBleGapEvtGenericEventInitializationCompleteOpCode_c:
                                {
                                    genericEvent.eventType = gInitializationComplete_c;
                                }
                                break;

                            case gBleGapEvtGenericEventInternalErrorOpCode_c:
                                {
                                    genericEvent.eventType = gInternalError_c;
                                }
                                break;

                            case gBleGapEvtGenericEventAdvertisingSetupFailedOpCode_c:
                                {
                                    genericEvent.eventType = gAdvertisingSetupFailed_c;
                                }
                                break;

                            case gBleGapEvtGenericEventAdvertisingParametersSetupCompleteOpCode_c:
                                {
                                    genericEvent.eventType = gAdvertisingParametersSetupComplete_c;
                                }
                                break;

                            case gBleGapEvtGenericEventAdvertisingDataSetupCompleteOpCode_c:
                                {
                                    genericEvent.eventType = gAdvertisingDataSetupComplete_c;
                                }
                                break;

                            case gBleGapEvtGenericEventWhiteListSizeReadOpCode_c:
                                {
                                    genericEvent.eventType = gWhiteListSizeRead_c;
                                }
                                break;

                            case gBleGapEvtGenericEventDeviceAddedToWhiteListOpCode_c:
                                {
                                    genericEvent.eventType = gDeviceAddedToWhiteList_c;
                                }
                                break;

                            case gBleGapEvtGenericEventDeviceRemovedFromWhiteListOpCode_c:
                                {
                                    genericEvent.eventType = gDeviceRemovedFromWhiteList_c;
                                }
                                break;

                            case gBleGapEvtGenericEventWhiteListClearedOpCode_c:
                                {
                                    genericEvent.eventType = gWhiteListCleared_c;
                                }
                                break;

                            case gBleGapEvtGenericEventRandomAddressReadyOpCode_c:
                                {
                                    genericEvent.eventType = gRandomAddressReady_c;
                                }
                                break;

                            case gBleGapEvtGenericEventCreateConnectionCanceledOpCode_c:
                                {
                                    genericEvent.eventType = gCreateConnectionCanceled_c;
                                }
                                break;

                            case gBleGapEvtGenericEventPublicAddressReadOpCode_c:
                                {
                                    genericEvent.eventType = gPublicAddressRead_c;
                                }
                                break;

                            case gBleGapEvtGenericEventAdvTxPowerLevelReadOpCode_c:
                                {
                                    genericEvent.eventType = gAdvTxPowerLevelRead_c;
                                }
                                break;

                            case gBleGapEvtGenericEventPrivateResolvableAddressVerifiedOpCode_c:
                                {
                                    genericEvent.eventType = gPrivateResolvableAddressVerified_c;
                                }
                                break;

                            case gBleGapEvtGenericEventRandomAddressSetOpCode_c:
                                {
                                    genericEvent.eventType = gRandomAddressSet_c;
                                }
                                break;

                            case gBleGapEvtGenericEventLeScPublicKeyRegeneratedOpCode_c:
                                {
                                    genericEvent.eventType = gLeScPublicKeyRegenerated_c;
                                }
                                break;

                            case gBleGapEvtGenericEventLeScLocalOobDataOpCode_c:
                                {
                                    genericEvent.eventType = gLeScLocalOobData_c;
                                }
                                break;

                            case gBleGapEvtGenericEventHostPrivacyStateChangedOpCode_c:
                                {
                                    genericEvent.eventType = gHostPrivacyStateChanged_c;
                                }
                                break;

                            case gBleGapEvtGenericEventControllerPrivacyStateChangedOpCode_c:
                                {
                                    genericEvent.eventType = gControllerPrivacyStateChanged_c;
                                }
                                break;

                            case gBleGapEvtGenericEventTxPowerLevelSetCompleteOpCode_c:
                                {
                                    genericEvent.eventType = gTxPowerLevelSetComplete_c;
                                }
                                break;

                            case gBleGapEvtGenericEventLePhyEventOpCode_c:
                                {
                                    genericEvent.eventType = gLePhyEvent_c;
                                }
                                break;

                            case gBleGapEvtGenericEventControllerNotificationOpCode_c:
                                {
                                    genericEvent.eventType = gControllerNotificationEvent_c;
                                }
                                break;

                            case gBleGapEvtGenericEventBondCreatedOpCode_c:
                                {
                                    genericEvent.eventType = gBondCreatedEvent_c;
                                }
                                break;

                            case gBleGapEvtGenericEventChannelMapSetOpCode_c:
                                {
                                    genericEvent.eventType = gChannelMapSet_c;
                                }
                                break;

#if gBLE50_d
                            case gBleGapEvtGenericEventExtAdvertisingDataSetupCompleteOpCode_c:
                                {
                                    genericEvent.eventType = gExtAdvertisingDataSetupComplete_c;
                                }
                                break;

                            case gBleGapEvtAdvertisingEventExtAdvertisingSetRemovedOpCode_c:
                                {
                                    genericEvent.eventType = gExtAdvertisingSetRemoveComplete_c;
                                }
                                break;

                            case gBleGapEvtGenericEventPeriodicAdvCreateSyncCancelledOpCode_c:
                                {
                                    genericEvent.eventType = gPeriodicAdvCreateSyncCancelled_c;
                                }
                                break;
#endif
                            case gBleGapEvtGenericEventTxEntryAvailableOpCode_c:
                                {
                                    genericEvent.eventType = gTxEntryAvailable_c;
                                }
                                break;

                            default:
                                {
                                    ; /* For MISRA compliance */
                                }
                                break;
                        }

                        fsciBleGapGetGenericEventFromBuffer(&genericEvent, &pBuffer);

                        fsciBleGapCallbacks.genericCallback(&genericEvent);
                    }
                    break;

                case gBleGapEvtAdvertisingEventAdvertisingStateChangedOpCode_c:
                case gBleGapEvtAdvertisingEventAdvertisingCommandFailedOpCode_c:
#if gBLE50_d
                case gBleGapEvtAdvertisingEventAdvertisingSetTerminatedOpCode_c:
#endif
                    {
                        gapAdvertisingEvent_t advertisingEvent;

                        switch(pClientPacket->structured.header.opCode)
                        {
                            case gBleGapEvtAdvertisingEventAdvertisingStateChangedOpCode_c:
                                {
                                    advertisingEvent.eventType = gAdvertisingStateChanged_c;
                                }
                                break;

                            case gBleGapEvtAdvertisingEventAdvertisingCommandFailedOpCode_c:
                                {
                                    advertisingEvent.eventType = gAdvertisingCommandFailed_c;
                                }
                                break;
#if gBLE50_d
                            case gBleGapEvtAdvertisingEventAdvertisingSetTerminatedOpCode_c:
                                {
                                    advertisingEvent.eventType = gAdvertisingSetTerminated_c;
                                }
                                break;
#endif
                            default:
                                {
                                    ; /* For MISRA compliance */
                                }
                                break;
                        }

                        fsciBleGapGetAdvertisingEventFromBuffer(&advertisingEvent, &pBuffer);

                        fsciBleGapCallbacks.advertisingCallback(&advertisingEvent);
                    }
                    break;

                case gBleGapEvtScanningEventScanStateChangedOpCode_c:
                case gBleGapEvtScanningEventScanCommandFailedOpCode_c:
                case gBleGapEvtScanningEventDeviceScannedOpCode_c:
#if gBLE50_d
                case gBleGapEvtScanningEventExtDeviceScannedOpCode_c:
                case gBleGapEvtScanningEventPeriodicDeviceScannedOpCode_c:
#endif
                    {
                        gapScanningEvent_t*     pScanningEvent;
                        gapScanningEventType_t  eventType = gScanStateChanged_c;

                        switch(pClientPacket->structured.header.opCode)
                        {
                            case gBleGapEvtScanningEventScanStateChangedOpCode_c:
                                {
                                    eventType = gScanStateChanged_c;
                                }
                                break;

                            case gBleGapEvtScanningEventScanCommandFailedOpCode_c:
                                {
                                    eventType = gScanCommandFailed_c;
                                }
                                break;

                            case gBleGapEvtScanningEventDeviceScannedOpCode_c:
                                {
                                    eventType = gDeviceScanned_c;
                                }
                                break;

#if gBLE50_d
                            case gBleGapEvtScanningEventExtDeviceScannedOpCode_c:
                                {
                                    eventType = gExtDeviceScanned_c;
                                }
                                break;

                            case gBleGapEvtScanningEventPeriodicDeviceScannedOpCode_c:
                                {
                                    eventType = gPeriodicDeviceScanned_c;
                                }
                                break;
#endif

                            default:
                                {
                                    ; /* For MISRA compliance */
                                }
                                break;
                        }

                        pScanningEvent = fsciBleGapAllocScanningEventForBuffer(eventType, pBuffer);

                        if(NULL == pScanningEvent)
                        {
                            /* No memory */
                            fsciBleError(gFsciOutOfMessages_c, fsciInterfaceId);
                        }
                        else
                        {
                            fsciBleGapGetScanningEventFromBuffer(pScanningEvent, &pBuffer);

                            fsciBleGapCallbacks.scanningCallback(pScanningEvent);

                            fsciBleGapFreeScanningEvent(pScanningEvent);
                        }
                    }
                    break;

                case gBleGapEvtConnectionEventConnectedOpCode_c:
                case gBleGapEvtConnectionEventPairingRequestOpCode_c:
                case gBleGapEvtConnectionEventSlaveSecurityRequestOpCode_c:
                case gBleGapEvtConnectionEventPairingResponseOpCode_c:
                case gBleGapEvtConnectionEventAuthenticationRejectedOpCode_c:
                case gBleGapEvtConnectionEventPasskeyRequestOpCode_c:
                case gBleGapEvtConnectionEventOobRequestOpCode_c:
                case gBleGapEvtConnectionEventPasskeyDisplayOpCode_c:
                case gBleGapEvtConnectionEventKeyExchangeRequestOpCode_c:
                case gBleGapEvtConnectionEventKeysReceivedOpCode_c:
                case gBleGapEvtConnectionEventLongTermKeyRequestOpCode_c:
                case gBleGapEvtConnectionEventEncryptionChangedOpCode_c:
                case gBleGapEvtConnectionEventPairingCompleteOpCode_c:
                case gBleGapEvtConnectionEventDisconnectedOpCode_c:
                case gBleGapEvtConnectionEventRssiReadOpCode_c:
                case gBleGapEvtConnectionEventTxPowerLevelReadOpCode_c:
                case gBleGapEvtConnectionEventPowerReadFailureOpCode_c:
                case gBleGapEvtConnectionEventParameterUpdateRequestOpCode_c:
                case gBleGapEvtConnectionEventParameterUpdateCompleteOpCode_c:
                case gBleGapEvtConnectionEventLeDataLengthChangedOpCode_c:
                case gBleGapEvtConnectionEventLeScOobDataRequestOpCode_c:
                case gBleGapEvtConnectionEventLeScDisplayNumericValueOpCode_c:
                case gBleGapEvtConnectionEventLeScKeypressNotificationOpCode_c:
                case gBleGapEvtConnectionEventChannelMapReadOpCode_c:
                case gBleGapEvtConnectionEventChannelMapReadFailureOpCode_c:
                    {
                        gapConnectionEvent_t*       gConnectionEvent;
                        gapConnectionEventType_t    eventType = gConnEvtConnected_c;
                        deviceId_t deviceId;

                        fsciBleGetDeviceIdFromBuffer(&deviceId, &pBuffer);
                        switch(pClientPacket->structured.header.opCode)
                        {
                            case gBleGapEvtConnectionEventConnectedOpCode_c:
                                {
                                    eventType = gConnEvtConnected_c;
                                }
                                break;

                            case gBleGapEvtConnectionEventPairingRequestOpCode_c:
                                {
                                    eventType = gConnEvtPairingRequest_c;
                                }
                                break;

                            case gBleGapEvtConnectionEventSlaveSecurityRequestOpCode_c:
                                {
                                    eventType = gConnEvtSlaveSecurityRequest_c;
                                }
                                break;

                            case gBleGapEvtConnectionEventPairingResponseOpCode_c:
                                {
                                    eventType = gConnEvtPairingResponse_c;
                                }
                                break;

                            case gBleGapEvtConnectionEventAuthenticationRejectedOpCode_c:
                                {
                                    eventType = gConnEvtAuthenticationRejected_c;
                                }
                                break;

                            case gBleGapEvtConnectionEventPasskeyRequestOpCode_c:
                                {
                                    eventType = gConnEvtPasskeyRequest_c;
                                }
                                break;

                            case gBleGapEvtConnectionEventOobRequestOpCode_c:
                                {
                                    eventType = gConnEvtOobRequest_c;
                                }
                                break;

                            case gBleGapEvtConnectionEventPasskeyDisplayOpCode_c:
                                {
                                    eventType = gConnEvtPasskeyDisplay_c;
                                }
                                break;

                            case gBleGapEvtConnectionEventKeyExchangeRequestOpCode_c:
                                {
                                    eventType = gConnEvtKeyExchangeRequest_c;
                                }
                                break;

                            case gBleGapEvtConnectionEventKeysReceivedOpCode_c:
                                {
                                    eventType = gConnEvtKeysReceived_c;
                                }
                                break;

                            case gBleGapEvtConnectionEventLongTermKeyRequestOpCode_c:
                                {
                                    eventType = gConnEvtLongTermKeyRequest_c;
                                }
                                break;

                            case gBleGapEvtConnectionEventEncryptionChangedOpCode_c:
                                {
                                    eventType = gConnEvtEncryptionChanged_c;
                                }
                                break;

                            case gBleGapEvtConnectionEventPairingCompleteOpCode_c:
                                {
                                    eventType = gConnEvtPairingComplete_c;
                                }
                                break;

                            case gBleGapEvtConnectionEventDisconnectedOpCode_c:
                                {
                                    eventType = gConnEvtDisconnected_c;
                                }
                                break;

                            case gBleGapEvtConnectionEventRssiReadOpCode_c:
                                {
                                    eventType = gConnEvtRssiRead_c;
                                }
                                break;

                            case gBleGapEvtConnectionEventTxPowerLevelReadOpCode_c:
                                {
                                    eventType = gConnEvtTxPowerLevelRead_c;
                                }
                                break;

                            case gBleGapEvtConnectionEventPowerReadFailureOpCode_c:
                                {
                                    eventType = gConnEvtPowerReadFailure_c;
                                }
                                break;

                            case gBleGapEvtConnectionEventParameterUpdateRequestOpCode_c:
                                {
                                    eventType = gConnEvtParameterUpdateRequest_c;
                                }
                                break;

                            case gBleGapEvtConnectionEventParameterUpdateCompleteOpCode_c:
                                {
                                    eventType = gConnEvtParameterUpdateComplete_c;
                                }
                                break;

                            case gBleGapEvtConnectionEventLeDataLengthChangedOpCode_c:
                                {
                                    eventType = gConnEvtLeDataLengthChanged_c;
                                }
                                break;

                            case gBleGapEvtConnectionEventLeScOobDataRequestOpCode_c:
                                {
                                    eventType = gConnEvtLeScOobDataRequest_c;
                                }
                                break;

                            case gBleGapEvtConnectionEventLeScDisplayNumericValueOpCode_c:
                                {
                                    eventType = gConnEvtLeScDisplayNumericValue_c;
                                }
                                break;

                            case gBleGapEvtConnectionEventLeScKeypressNotificationOpCode_c:
                                {
                                    eventType = gConnEvtLeScKeypressNotification_c;
                                }
                                break;

                            case gBleGapEvtConnectionEventChannelMapReadOpCode_c:
                                {
                                    eventType = gConnEvtChannelMapRead_c;
                                }
                                break;

                            case gBleGapEvtConnectionEventChannelMapReadFailureOpCode_c:
                                {
                                    eventType = gConnEvtChannelMapReadFailure_c;
                                }
                                break;

                            default:
                                {
                                    ; /* For MISRA compliance */
                                }
                                break;
                        }

                        gConnectionEvent = fsciBleGapAllocConnectionEventForBuffer(eventType, pBuffer);

                        if(NULL == gConnectionEvent)
                        {
                            /* No memory */
                            fsciBleError(gFsciOutOfMessages_c, fsciInterfaceId);
                        }
                        else
                        {
                            fsciBleGapGetConnectionEventFromBuffer(gConnectionEvent, &pBuffer);

                            fsciBleGapCallbacks.connectionCallback(deviceId, gConnectionEvent);

                            fsciBleGapFreeConnectionEvent(gConnectionEvent);
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
    bFsciBleGapCmdInitiatedByFsci = FALSE;
#endif /* gFsciBleTest_d */

    (void)MEM_BufferFree(pData);
}


#if gFsciBleBBox_d || gFsciBleTest_d

void fsciBleGapStatusMonitor(bleResult_t result)
{
#if gFsciBleTest_d
    /* If GAP is disabled the status must be not monitored */
    if(FALSE == bFsciBleGapEnabled)
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Send status over UART */
    fsciBleStatusMonitor(gFsciBleGapOpcodeGroup_c, (uint8_t)gBleGapStatusOpCode_c, result);
}

#endif /* gFsciBleBBox_d || gFsciBleTest_d */


#if gFsciBleHost_d || gFsciBleTest_d

void fsciBleGapNoParamCmdMonitor(fsciBleGapOpCode_t opCode)
{
#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

     /* Call the generic FSCI BLE monitor for commands or events that have no parameters */
    fsciBleNoParamCmdOrEvtMonitor(gFsciBleGapOpcodeGroup_c, opCode);
}


void fsciBleGapUint8ParamCmdMonitor(fsciBleGapOpCode_t opCode, uint8_t param)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(opCode, sizeof(uint8_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromUint8Value(param, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapDeviceIdParamCmdMonitor(fsciBleGapOpCode_t opCode, deviceId_t deviceId)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(opCode,
                                              fsciBleGetDeviceIdBufferSize(&deviceId));

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


void fsciBleGapCheckNotificationsAndIndicationsCmdMonitor(fsciBleGapOpCode_t opCode, deviceId_t deviceId, uint16_t handle, bool_t* pOutIsActive)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(opCode,
                                              fsciBleGetDeviceIdBufferSize(&deviceId) +
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

#if gFsciBleHost_d
    /* Save out parameters pointers */
    fsciBleGapSaveOutParams(pOutIsActive, NULL);
#endif /* gFsciBleHost_d */
}


void fsciBleGapAddressParamsCmdMonitor(fsciBleGapOpCode_t opCode, bleAddressType_t addressType, const bleDeviceAddress_t address)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(opCode,
                                              sizeof(bleAddressType_t) +
                                              gcBleDeviceAddressSize_c);

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromEnumValue(addressType, pBuffer, bleAddressType_t);
    fsciBleGetBufferFromAddress(address, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapHostInitializeCmdMonitor(gapGenericCallback_t genericCallback, hciHostToControllerInterface_t  hostToControllerInterface)
{
#if gFsciBleHost_d
    /* Save generic callback */
    fsciBleGapCallbacks.genericCallback = genericCallback;

    /* Send FSCI CPU reset command to BlackBox */
    fsciBleNoParamCmdOrEvtMonitor(gFSCI_ReqOpcodeGroup_c, mFsciMsgResetCPUReq_c);
#else
    fsciBleGapNoParamCmdMonitor(gBleGapCmdBleHostInitializeOpCode_c);
#endif /* gFsciBleHost_d */
}


void fsciBleGapRegisterDeviceSecurityRequirementsCmdMonitor(const gapDeviceSecurityRequirements_t* pSecurity)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;
    bool_t                      bDeviceSecurityRequirementsIncluded = ((NULL != pSecurity) ? TRUE : FALSE);

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdRegisterDeviceSecurityRequirementsOpCode_c,
                                              sizeof(bool_t) +
                                              ((TRUE == bDeviceSecurityRequirementsIncluded) ?
                                               fsciBleGapGetDeviceSecurityRequirementsBufferSize(pSecurity) :
                                               0));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromBoolValue(bDeviceSecurityRequirementsIncluded, pBuffer);
    if(TRUE == bDeviceSecurityRequirementsIncluded)
    {
        fsciBleGapGetBufferFromDeviceSecurityRequirements(pSecurity, &pBuffer);
    }

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapSetAdvertisingParametersCmdMonitor(const gapAdvertisingParameters_t* pAdvertisingParameters)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdSetAdvertisingParametersOpCode_c,
                                              fsciBleGapGetAdvertisingParametersBufferSize(pAdvertisingParameters));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGapGetBuffFromAdvParameters(pAdvertisingParameters, &pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


#if gBLE50_d
void fsciBleGapSetExtAdvertisingParametersCmdMonitor(gapExtAdvertisingParameters_t* pAdvertisingParameters)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdSetExtAdvertisingParametersOpCode_c,
                                              fsciBleGapGetExtAdvertisingParametersBufferSize(pAdvertisingParameters));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGapGetBufferFromExtAdvertisingParameters(pAdvertisingParameters, &pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}
#endif /* gBLE50_d */


void fsciBleGapSetAdvertisingDataCmdMonitor(const gapAdvertisingData_t* pAdvertisingData, const gapScanResponseData_t* pScanResponseData)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;
    bool_t                      bAdvertisingDataIncluded = FALSE;
    bool_t                      bScanResponseDataIncluded = FALSE;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    if( NULL != pAdvertisingData )
    {
        if( pAdvertisingData->cNumAdStructures )
        {
            bAdvertisingDataIncluded = TRUE;
        }
    }

    if( NULL != pScanResponseData )
    {
        if( pScanResponseData->cNumAdStructures )
        {
            bScanResponseDataIncluded = TRUE;
        }
    }

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdSetAdvertisingDataOpCode_c,
                                              sizeof(bool_t) +
                                              ((TRUE == bAdvertisingDataIncluded) ?
                                               fsciBleGapGetAdvertisingDataBufferSize(pAdvertisingData) :
                                               0) +
                                              sizeof(bool_t) +
                                              ((TRUE == bScanResponseDataIncluded) ?
                                              fsciBleGapGetScanResponseDataBufferSize(pScanResponseData) :
                                              0));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromBoolValue(bAdvertisingDataIncluded, pBuffer);
    if(TRUE == bAdvertisingDataIncluded)
    {
        fsciBleGapGetBufferFromAdvertisingData(pAdvertisingData, &pBuffer);
    }

    fsciBleGetBufferFromBoolValue(bScanResponseDataIncluded, pBuffer);
    if(TRUE == bScanResponseDataIncluded)
    {
        fsciBleGapGetBufferFromScanResponseData(pScanResponseData, &pBuffer);
    }

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}

#if gBLE50_d

void fsciBleGapSetExtAdvertisingDataCmdMonitor(uint8_t handle, gapAdvertisingData_t* pAdvertisingData, gapScanResponseData_t* pScanResponseData)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;
    bool_t                      bAdvertisingDataIncluded = FALSE;
    bool_t                      bScanResponseDataIncluded = FALSE;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    if( NULL != pAdvertisingData )
    {
        if( pAdvertisingData->cNumAdStructures )
        {
            bAdvertisingDataIncluded = TRUE;
        }
    }

    if( NULL != pScanResponseData )
    {
        if( pScanResponseData->cNumAdStructures )
        {
            bScanResponseDataIncluded = TRUE;
        }
    }

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdSetAdvertisingDataOpCode_c,
                                              sizeof(uint8_t) + sizeof(bool_t) +
                                              ((TRUE == bAdvertisingDataIncluded) ?
                                               fsciBleGapGetAdvertisingDataBufferSize(pAdvertisingData) :
                                               0) +
                                              sizeof(bool_t) +
                                              ((TRUE == bScanResponseDataIncluded) ?
                                              fsciBleGapGetScanResponseDataBufferSize(pScanResponseData) :
                                              0));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromUint8Value(handle, pBuffer);
    fsciBleGetBufferFromBoolValue(bAdvertisingDataIncluded, pBuffer);
    if(TRUE == bAdvertisingDataIncluded)
    {
        fsciBleGapGetBufferFromAdvertisingData(pAdvertisingData, &pBuffer);
    }

    fsciBleGetBufferFromBoolValue(bScanResponseDataIncluded, pBuffer);
    if(TRUE == bScanResponseDataIncluded)
    {
        fsciBleGapGetBufferFromScanResponseData(pScanResponseData, &pBuffer);
    }

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}
#endif

void fsciBleGapStartAdvertisingCmdMonitor(gapAdvertisingCallback_t advertisingCallback, gapConnectionCallback_t connectionCallback)
{
#if gFsciBleHost_d
    fsciBleGapCallbacks.advertisingCallback = advertisingCallback;
    fsciBleGapCallbacks.connectionCallback  = connectionCallback;
#endif /* gFsciBleHost_d */

    fsciBleGapNoParamCmdMonitor(gBleGapCmdStartAdvertisingOpCode_c);
}

#if gBLE50_d
void fsciBleGapStartExtAdvertisingCmdMonitor(gapAdvertisingCallback_t advertisingCallback, gapConnectionCallback_t connectionCallback, uint8_t handle, uint16_t duration, uint8_t maxExtAdvEvents)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

#if gFsciBleHost_d
    fsciBleGapCallbacks.advertisingCallback = advertisingCallback;
    fsciBleGapCallbacks.connectionCallback  = connectionCallback;
#endif /* gFsciBleHost_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdStartExtAdvertisingOpCode_c,
                                              sizeof(uint8_t) + sizeof(uint16_t) + sizeof(uint8_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromUint8Value(handle, pBuffer);
    fsciBleGetBufferFromUint16Value(duration, pBuffer);
    fsciBleGetBufferFromUint8Value(maxExtAdvEvents, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}
#endif

void fsciBleGapAuthorizeCmdMonitor(deviceId_t deviceId, uint16_t handle, gattDbAccessType_t access)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdAuthorizeOpCode_c, fsciBleGetDeviceIdBufferSize(&deviceId) +
                                                                           sizeof(uint16_t) +
                                                                           sizeof(gattDbAccessType_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
    fsciBleGetBufferFromUint16Value(handle, pBuffer);
    fsciBleGetBufferFromEnumValue(access, pBuffer, gattDbAccessType_t);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapSaveCccdCmdMonitor(deviceId_t deviceId, uint16_t handle, gattCccdFlags_t cccd)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdSaveCccdOpCode_c, fsciBleGetDeviceIdBufferSize(&deviceId) +
                                                                          sizeof(uint16_t) +
                                                                          sizeof(gattCccdFlags_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
    fsciBleGetBufferFromUint16Value(handle, pBuffer);
    fsciBleGetBufferFromEnumValue(cccd, pBuffer, gattCccdFlags_t);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}

void fsciBleGapGetBondedDevicesIdentityInformationCmdMonitor(const gapIdentityInformation_t* aOutIdentityAddresses, uint8_t maxDevices, const uint8_t* pOutActualCount)
{
    fsciBleGapUint8ParamCmdMonitor(gBleGapCmdGetBondedDevicesIdentityInformationOpCode_c, maxDevices);

#if gFsciBleHost_d
    /* Save out parameters pointers */
    fsciBleGapSaveOutParams(aOutIdentityAddresses, pOutActualCount);
#endif /* gFsciBleHost_d */
}

void fsciBleGapPairCmdMonitor(deviceId_t deviceId, const gapPairingParameters_t* pPairingParameters)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdPairOpCode_c, fsciBleGetDeviceIdBufferSize(&deviceId) +
                                                                      fsciBleGapGetPairingParametersBufferSize(pPairingParameters));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
    fsciBleGapGetBufferFromPairingParameters(pPairingParameters, &pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapSendSlaveSecurityRequestCmdMonitor(deviceId_t deviceId, const gapPairingParameters_t* pPairingParameters)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdSendSlaveSecurityRequestOpCode_c, fsciBleGetDeviceIdBufferSize(&deviceId) +
                                                                                          fsciBleGapGetPairingParametersBufferSize(pPairingParameters));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
    fsciBleGapGetBufferFromPairingParameters(pPairingParameters, &pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapAcceptPairingRequestCmdMonitor(deviceId_t deviceId, const gapPairingParameters_t* pPairingParameters)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdAcceptPairingRequestOpCode_c,
                                              fsciBleGetDeviceIdBufferSize(&deviceId) +
                                              fsciBleGapGetPairingParametersBufferSize(pPairingParameters));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
    fsciBleGapGetBufferFromPairingParameters(pPairingParameters, &pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapRejectPairingCmdMonitor(deviceId_t deviceId, gapAuthenticationRejectReason_t reason)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdRejectPairingOpCode_c,
                                              fsciBleGetDeviceIdBufferSize(&deviceId) +
                                              sizeof(gapAuthenticationRejectReason_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
    fsciBleGetBufferFromEnumValue(reason, pBuffer, gapAuthenticationRejectReason_t);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapEnterPasskeyCmdMonitor(deviceId_t deviceId, uint32_t passkey)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdEnterPasskeyOpCode_c,
                                              fsciBleGetDeviceIdBufferSize(&deviceId) +
                                              sizeof(uint32_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
    fsciBleGetBufferFromUint32Value(passkey, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapProvideOobCmdMonitor(deviceId_t deviceId, const uint8_t* aOob)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
   /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdProvideOobOpCode_c,
                                              fsciBleGetDeviceIdBufferSize(&deviceId) +
                                              gcSmpOobSize_c);

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
    fsciBleGetBufferFromOob(aOob, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapSendSmpKeysCmdMonitor(deviceId_t deviceId, const gapSmpKeys_t* pKeys)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdSendSmpKeysOpCode_c,
                                              fsciBleGetDeviceIdBufferSize(&deviceId) +
                                              fsciBleGapGetSmpKeysBufferSize(pKeys));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
    fsciBleGapGetBufferFromSmpKeys(pKeys, &pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapProvideLongTermKeyCmdMonitor(deviceId_t deviceId, const uint8_t* aLtk, uint8_t ltkSize)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
   /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdProvideLongTermKeyOpCode_c,
                                              fsciBleGetDeviceIdBufferSize(&deviceId) +
                                              sizeof(uint8_t) + ltkSize);

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
    fsciBleGetBufferFromUint8Value(ltkSize, pBuffer);
    fsciBleGetBufferFromArray(aLtk, pBuffer, ltkSize);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapLoadEncryptionInformationCmdMonitor(deviceId_t deviceId, const uint8_t* aOutLtk, const uint8_t* pOutLtkSize)
{
    /* Monitor deviceId parameter */
    fsciBleGapDeviceIdParamCmdMonitor(gBleGapCmdLoadEncryptionInformationOpCode_c, deviceId);

#if gFsciBleHost_d
    /* Save out parameters pointers */
    fsciBleGapSaveOutParams(aOutLtk, pOutLtkSize);
#endif /* gFsciBleHost_d */
}


void fsciBleGapSetLocalPasskeyCmdMonitor(uint32_t passkey)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdSetLocalPasskeyOpCode_c,
                                              sizeof(uint32_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromUint32Value(passkey, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapStartScanningCmdMonitor(const gapScanningParameters_t* pScanningParameters,
                                       gapScanningCallback_t scanningCallback,
                                       gapFilterDuplicates_t enableFilterDuplicates,
                                       uint16_t duration,
                                       uint16_t period
)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;
    bool_t                      bScanningParametersIncluded = ((NULL != pScanningParameters) ? TRUE : FALSE);

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

#if gFsciBleHost_d
    fsciBleGapCallbacks.scanningCallback = scanningCallback;
#endif /* gFsciBleHost_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdStartScanningOpCode_c,
                                              sizeof(bool_t) +
                                              ((TRUE == bScanningParametersIncluded) ?
                                               fsciBleGapGetScanningParametersBufferSize(pScanningParameters) :
                                               0) + sizeof(gapFilterDuplicates_t) + sizeof(uint16_t) + sizeof(uint16_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromBoolValue(bScanningParametersIncluded, pBuffer);
    if(TRUE == bScanningParametersIncluded)
    {
        fsciBleGapGetBuffFromScanParameters(pScanningParameters, &pBuffer);
    }
    /* Set the enable filter duplicates field */
    fsciBleGetBufferFromEnumValue(enableFilterDuplicates, pBuffer, gapFilterDuplicates_t);

    /* Set the scanning PHYs */
    fsciBleGetBufferFromEnumValue(pScanningParameters->scanningPHYs, pBuffer, gapLePhyFlags_t);

    /* Set scan duration and period */
    fsciBleGetBufferFromUint16Value(duration, pBuffer);
    fsciBleGetBufferFromUint16Value(period, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapConnectCmdMonitor(const gapConnectionRequestParameters_t* pParameters, gapConnectionCallback_t connCallback)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

#if gFsciBleHost_d
    fsciBleGapCallbacks.connectionCallback = connCallback;
#endif /* gFsciBleHost_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdConnectOpCode_c,
                                              fsciBleGapGetConnectionRequestParametersBufferSize(pParameters));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGapGetBuffFromConnReqParameters(pParameters, &pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapSaveCustomPeerInformationCmdMonitor(deviceId_t deviceId, const uint8_t* aInfo, uint16_t offset, uint16_t infoSize)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdSaveCustomPeerInformationOpCode_c,
                                              fsciBleGetDeviceIdBufferSize(&deviceId) +
                                              sizeof(uint16_t) + sizeof(uint16_t) + infoSize);

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
    fsciBleGetBufferFromUint16Value(offset, pBuffer);
    fsciBleGetBufferFromUint16Value(infoSize, pBuffer);
    fsciBleGetBufferFromArray(aInfo, pBuffer, infoSize);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapLoadCustomPeerInformationCmdMonitor(deviceId_t deviceId, const uint8_t* aOutInfo, uint16_t offset, uint16_t infoSize)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdLoadCustomPeerInformationOpCode_c,
                                              fsciBleGetDeviceIdBufferSize(&deviceId) +
                                              sizeof(uint16_t) + sizeof(uint16_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
    fsciBleGetBufferFromUint16Value(offset, pBuffer);
    fsciBleGetBufferFromUint16Value(infoSize, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);

#if gFsciBleHost_d
    /* Save out parameters pointers */
    fsciBleGapSaveOutParams(aOutInfo, NULL);
#endif /* gFsciBleHost_d */
}


void fsciBleGapCheckIfBondedCmdMonitor(deviceId_t deviceId, bool_t* pOutIsBonded)
{
    /* Monitor deviceId parameter */
    fsciBleGapDeviceIdParamCmdMonitor(gBleGapCmdCheckIfBondedOpCode_c, deviceId);

#if gFsciBleHost_d
    /* Save out parameters pointers */
    fsciBleGapSaveOutParams(pOutIsBonded, NULL);
#endif /* gFsciBleHost_d */
}


void fsciBleGapCreateRandomDeviceAddressCmdMonitor(const uint8_t* aIrk, const uint8_t* aRandomPart)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;
    bool_t                      bIrkIncluded        = ((NULL != aIrk) ? TRUE : FALSE);
    bool_t                      bRandomPartIncluded = ((NULL != aRandomPart) ? TRUE : FALSE);

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdCreateRandomDeviceAddressOpCode_c,
                                              sizeof(bool_t) +
                                              ((TRUE == bIrkIncluded) ?
                                               (gcSmpIrkSize_c +
                                                sizeof(bool_t) +
                                                ((TRUE == bRandomPartIncluded) ? 3 : 0))
                                               : 0));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromBoolValue(bIrkIncluded, pBuffer);
    if(TRUE == bIrkIncluded)
    {
        fsciBleGetBufferFromIrk(aIrk, pBuffer);
    }

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapSaveDeviceNameCmdMonitor(deviceId_t deviceId, const uchar_t* aName, uint8_t cNameSize)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdSaveDeviceNameOpCode_c,
                                              fsciBleGetDeviceIdBufferSize(&deviceId) +
                                              sizeof(uint8_t) + cNameSize);

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
    fsciBleGetBufferFromUint8Value(cNameSize, pBuffer);
    fsciBleGetBufferFromArray(aName, pBuffer, cNameSize);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapGetBondedDevicesCountCmdMonitor(const uint8_t* pOutBondedDevicesCount)
{
    fsciBleGapNoParamCmdMonitor(gBleGapCmdGetBondedDevicesCountOpCode_c);

#if gFsciBleHost_d
    /* Save out parameters pointers */
    fsciBleGapSaveOutParams(pOutBondedDevicesCount, NULL);
#endif /* gFsciBleHost_d */
}


void fsciBleGapGetBondedDeviceNameCmdMonitor(uint8_t nvmIndex, uchar_t* aOutName, uint8_t maxNameSize)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdGetBondedDeviceNameOpCode_c,
                                              sizeof(uint8_t) + sizeof(uint8_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromUint8Value(nvmIndex, pBuffer);
    fsciBleGetBufferFromUint8Value(maxNameSize, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);

#if gFsciBleHost_d
    /* Save out parameters pointers */
    fsciBleGapSaveOutParams(aOutName, NULL);
#endif /* gFsciBleHost_d */
}


void fsciBleGapReadRadioPowerLevelCmdMonitor(gapRadioPowerLevelReadType_t txReadType, deviceId_t deviceId)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdReadRadioPowerLevelOpCode_c,
                                              sizeof(gapRadioPowerLevelReadType_t) + fsciBleGetDeviceIdBufferSize(&deviceId));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromEnumValue(txReadType, pBuffer, gapRadioPowerLevelReadType_t);
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapVerifyPrivateResolvableAddressCmdMonitor(uint8_t nvmIndex, const bleDeviceAddress_t aAddress)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdVerifyPrivateResolvableAddressOpCode_c,
                                              sizeof(uint8_t) + gcBleDeviceAddressSize_c);

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromUint8Value(nvmIndex, pBuffer);
    fsciBleGetBufferFromAddress(aAddress, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapSetRandomAddressCmdMonitor(const bleDeviceAddress_t aAddress)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdSetRandomAddressOpCode_c,
                                              gcBleDeviceAddressSize_c);

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromAddress(aAddress, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapSetScanModeCmdMonitor(gapScanMode_t scanMode, gapAutoConnectParams_t* pAutoConnectParams)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdSetScanModeOpCode_c,
                                              sizeof(gapScanMode_t) +
                                              ((gAutoConnect_c == scanMode) ?
                                                fsciBleGapGetAutoConnectParamsBufferSize(pAutoConnectParams) : 0));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromEnumValue(scanMode, pBuffer, gapScanMode_t);
    fsciBleGapGetBufferFromAutoConnectParams(pAutoConnectParams, &pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapSetDefaultPairingParametersCmdMonitor(const gapPairingParameters_t* pPairingParameters)
{
    bool_t                      bPairingParametersIncluded = (NULL == pPairingParameters) ? FALSE : TRUE;
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdSetDefaultPairingParameters_c,
                                              sizeof(bool_t) +
                                              ((TRUE == bPairingParametersIncluded) ?
                                               fsciBleGapGetPairingParametersBufferSize(pPairingParameters) : 0));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromBoolValue(bPairingParametersIncluded, pBuffer);

    if(TRUE == bPairingParametersIncluded)
    {
        fsciBleGapGetBufferFromPairingParameters(pPairingParameters, &pBuffer);
    }

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapUpdateConnectionParametersCmdMonitor(deviceId_t deviceId, uint16_t intervalMin, uint16_t intervalMax, uint16_t slaveLatency, uint16_t timeoutMultiplier, uint16_t minCeLength, uint16_t maxCeLength)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If Gap is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdUpdateConnectionParametersOpCode_c,
                                                fsciBleGetDeviceIdBufferSize(&deviceId) +
                                                sizeof(uint16_t) + sizeof(uint16_t) +
                                                sizeof(uint16_t) + sizeof(uint16_t) +
                                                sizeof(uint16_t) + sizeof(uint16_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
    fsciBleGetBufferFromUint16Value(intervalMin, pBuffer);
    fsciBleGetBufferFromUint16Value(intervalMax, pBuffer);
    fsciBleGetBufferFromUint16Value(slaveLatency, pBuffer);
    fsciBleGetBufferFromUint16Value(timeoutMultiplier, pBuffer);
    fsciBleGetBufferFromUint16Value(minCeLength, pBuffer);
    fsciBleGetBufferFromUint16Value(maxCeLength, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapEnableUpdateConnectionParametersCmdMonitor(deviceId_t deviceId, bool_t enable)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If Gap is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdEnableUpdateConnectionParametersOpCode_c,
                                                fsciBleGetDeviceIdBufferSize(&deviceId) + sizeof(bool_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
    fsciBleGetBufferFromBoolValue(enable, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapUpdateLeDataLengthCmdMonitor(deviceId_t deviceId, uint16_t txOctets, uint16_t txTime)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdUpdateLeDataLengthOpCode_c,
                                              2 * sizeof(uint16_t) + fsciBleGetDeviceIdBufferSize(&deviceId));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
    fsciBleGetBufferFromUint16Value(txOctets, pBuffer);
    fsciBleGetBufferFromUint16Value(txTime, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapEnableHostPrivacyCmdMonitor(bool_t enable, const uint8_t* aIrk)
{
    clientPacketStructured_t* pClientPacket;
    uint8_t*                  pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdEnableHostPrivacyOpCode_c,
                                              sizeof(bool_t) +
                                              ((TRUE == enable) ? gcSmpIrkSize_c : 0));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromBoolValue(enable, pBuffer);
    if(TRUE == enable)
    {
        fsciBleGetBufferFromIrk(aIrk, pBuffer);
    }

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapEnableControllerPrivacyCmdMonitor(bool_t enable, const uint8_t* aOwnIrk, uint8_t peerIdCount, const gapIdentityInformation_t* aPeerIdentities)
{
    clientPacketStructured_t* pClientPacket;
    uint8_t*                  pBuffer;
    uint32_t                  i = 0;
    uint8_t                   size = sizeof(bool_t);

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    if( TRUE == enable )
    {
        size += gcSmpIrkSize_c + peerIdCount * (sizeof(bleAddressType_t) +
                                                gcBleDeviceAddressSize_c +
                                                gcSmpIrkSize_c +
                                                sizeof(blePrivacyMode_t));
    }

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdEnableControllerPrivacyOpCode_c, size);
    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromBoolValue(enable, pBuffer);
    if(TRUE == enable)
    {
        fsciBleGetBufferFromIrk(aOwnIrk, pBuffer);
        fsciBleGetBufferFromUint8Value(peerIdCount, pBuffer);

        for( i=0; i<peerIdCount; i++ )
        {
            fsciBleGapGetBufferFromIdentityInformation(&aPeerIdentities[i], &pBuffer);
        }
    }

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapSetPrivacyModeCmdMonitor(uint8_t nvmIndex, blePrivacyMode_t privacyMode)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdSetPrivacyModeOpCode_c,
                                              sizeof(uint8_t) + sizeof(blePrivacyMode_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromUint8Value(nvmIndex, pBuffer);
    fsciBleGetBufferFromEnumValue(privacyMode, pBuffer, blePrivacyMode_t);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapLeScValidateNumericValueCmdMonitor(deviceId_t deviceId, bool_t valid)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdLeScValidateNumericValueOpCode_c,
                                              sizeof(bool_t) + fsciBleGetDeviceIdBufferSize(&deviceId));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
    fsciBleGetBufferFromBoolValue(valid, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapLeScSetPeerOobDataCmdMonitor(deviceId_t deviceId, const gapLeScOobData_t* pPeerOobData)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdLeScSetPeerOobDataOpCode_c,
                                              gSmpLeScRandomValueSize_c +
                                              gSmpLeScRandomConfirmValueSize_c +
                                              fsciBleGetDeviceIdBufferSize(&deviceId));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
    fsciBleGetBufferFromArray(pPeerOobData->randomValue, pBuffer, gSmpLeScRandomValueSize_c);
    fsciBleGetBufferFromArray(pPeerOobData->confirmValue, pBuffer, gSmpLeScRandomConfirmValueSize_c);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapLeScSendKeypressNotificationCmdMonitor(deviceId_t deviceId, gapKeypressNotification_t keypressNotification)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdLeScSendKeypressNotificationPrivacyOpCode_c,
                                              sizeof(gapKeypressNotification_t) + fsciBleGetDeviceIdBufferSize(&deviceId));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
    fsciBleGetBufferFromEnumValue(keypressNotification, pBuffer, gapKeypressNotification_t);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapSetTxPowerLevelCmdMonitor(uint8_t powerLevel, bleTransmitPowerChannelType_t channelType)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdSetTxPowerLevelOpCode_c,
                                              sizeof(uint8_t) + sizeof(bleTransmitPowerChannelType_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromUint8Value(powerLevel, pBuffer);
    fsciBleGetBufferFromEnumValue(channelType, pBuffer, bleTransmitPowerChannelType_t);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}

void fsciBleGapLeReadPhyCmdMonitor(deviceId_t deviceId)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdLeReadPhyOpCode_c,
                                              fsciBleGetDeviceIdBufferSize(&deviceId));

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

void fsciBleGapLeSetPhyCmdMonitor(bool_t defaultMode, deviceId_t deviceId, uint8_t allPhys,
                                  uint8_t txPhys, uint8_t rxPhys, uint16_t phyOptions)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdLeSetPhyOpCode_c,
                                              sizeof(bool_t) + fsciBleGetDeviceIdBufferSize(&deviceId) +
                                              3 * sizeof(uint8_t) + sizeof(uint16_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromBoolValue(defaultMode, pBuffer);
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
    fsciBleGetBufferFromUint8Value(allPhys, pBuffer);
    fsciBleGetBufferFromUint8Value(txPhys, pBuffer);
    fsciBleGetBufferFromUint8Value(rxPhys, pBuffer);
    fsciBleGetBufferFromUint16Value(phyOptions, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}

void fsciBleGapControllerEnhancedNotificationCmdMonitor(uint16_t eventType, deviceId_t deviceId)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdControllerNotificationOpCode_c,
                                              sizeof(uint16_t) + fsciBleGetDeviceIdBufferSize(&deviceId));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromUint16Value(eventType, pBuffer);
    fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}

void fsciBleGapLoadKeysCmdMonitor
(
    uint8_t           nvmIndex,
    gapSmpKeys_t*     pOutKeys,
    gapSmpKeyFlags_t* pOutKeyFlags,
    bool_t*           pOutLeSc,
    bool_t*           pOutAuth
)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdLoadKeysOpCode_c,
                                              sizeof(uint8_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromUint8Value(nvmIndex, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapSaveKeysCmdMonitor(uint8_t nvmIndex, gapSmpKeys_t* pKeys, bool_t leSc, bool_t auth)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdSaveKeysOpCode_c,
                                              sizeof(uint8_t) + 2 * sizeof(bool_t) +
                                              fsciBleGapGetSmpKeysBufferSize(pKeys));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromUint8Value(nvmIndex, pBuffer);
    fsciBleGapGetBufferFromSmpKeys(pKeys, &pBuffer);
    fsciBleGetBufferFromBoolValue(leSc, pBuffer);
    fsciBleGetBufferFromBoolValue(auth, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}

void fsciBleGapSetChannelMapCmdMonitor
(
    bleChannelMap_t channelMap
)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdSetChannelMapOpCode_c,
                                              gcBleChannelMapSize_c);

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromArray(channelMap, pBuffer, gcBleChannelMapSize_c);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}

#if gBLE50_d
void fsciBleGapSetPeriodicAdvParametersCmdMonitor(gapPeriodicAdvParameters_t* pAdvertisingParameters)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdSetPeriodicAdvParametersOpCode_c,
                                              sizeof(uint8_t) + sizeof(bool_t) +  2 * sizeof(uint16_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGapGetBufferFromPerAdvParameters(pAdvertisingParameters, &pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}

//gcapraru: use this API with adv data NULL ? this may be used to trigger the DID update.
void fsciBleGapSetPeriodicAdvertisingDataCmdMonitor(uint8_t handle, gapAdvertisingData_t* pAdvertisingData)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdSetPeriodicAdvDataOpCode_c,
                                              sizeof(uint8_t) +
                                              fsciBleGapGetAdvertisingDataBufferSize(pAdvertisingData));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromUint8Value(handle, pBuffer);
    fsciBleGapGetBufferFromAdvertisingData(pAdvertisingData, &pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}
#endif /* gBLE50_d */

#endif /* gFsciBleHost_d || gFsciBleTest_d */


#if gFsciBleBBox_d || gFsciBleTest_d

void fsciBleGapBoolParamEvtMonitor(fsciBleGapOpCode_t opCode, bool_t bParam)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled the event must be not monitored */
    if(FALSE == bFsciBleGapEnabled)
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket((uint8_t)opCode, sizeof(bool_t));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set event parameters in the buffer */
    fsciBleGetBufferFromBoolValue(bParam, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapArrayAndSizeParamEvtMonitor(fsciBleGapOpCode_t opCode, const uint8_t* aArray, uint8_t arraySize)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled the event must be not monitored */
    if(FALSE == bFsciBleGapEnabled)
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket((uint8_t)opCode, (sizeof(uint8_t) +
                                                      ((NULL == aArray) ? 0U : (uint16_t)arraySize)));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set event parameters in the buffer */
    fsciBleGetBufferFromUint8Value(arraySize, pBuffer);

    if(NULL != aArray)
    {
        fsciBleGetBufferFromArray(aArray, pBuffer, arraySize);
    }

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}

void fsciBleGapGetBondedDevIdentityInfoEvtMonitor(const gapIdentityInformation_t* aOutIdentityAddresses, const uint8_t* pOutActualCount)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;
    uint32_t                    iCount;

 #if gFsciBleTest_d
   /* If GAP is disabled the event must be not monitored */
    if(FALSE == bFsciBleGapEnabled)
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket((uint8_t)gBleGapEvtGetBondedDevicesIdentityInformationOpCode_c,
                                              (uint16_t)sizeof(uint8_t) +
                                              (uint16_t)*pOutActualCount * (sizeof(bleAddressType_t) +
                                                                            gcBleDeviceAddressSize_c +
                                                                            gcSmpIrkSize_c +
                                                                            sizeof(blePrivacyMode_t)));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set event parameters in the buffer */
    fsciBleGetBufferFromUint8Value(*pOutActualCount, pBuffer);

    for(iCount = 0; iCount < *pOutActualCount; iCount++)
    {
        fsciBleGapGetBufferFromIdentityInformation(&aOutIdentityAddresses[iCount], &pBuffer);
    }

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapLoadCustomPeerInfoEvtMonitor(const uint8_t* aOutInfo, uint16_t infoSize)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled the event must be not monitored */
    if(FALSE == bFsciBleGapEnabled)
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket((uint8_t)gBleGapEvtLoadCustomPeerInformationOpCode_c,
                                              sizeof(uint16_t) + infoSize);

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set event parameters in the buffer */
    fsciBleGetBufferFromUint16Value(infoSize, pBuffer);
    fsciBleGetBufferFromArray(aOutInfo, pBuffer, infoSize);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapLoadKeysEvtMonitor
(
    gapSmpKeys_t     *pOutKeys,
    gapSmpKeyFlags_t *pOutKeyFlags,
    bool_t           *pOutLeSc,
    bool_t           *pOutAuth
)
{
    clientPacketStructured_t   *pClientPacket;
    uint8_t                    *pBuffer;

#if gFsciBleTest_d

    /* If GAP is disabled the event must be not monitored */
    if (FALSE == bFsciBleGapEnabled)
    {
        return;
    }

#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket((uint8_t)gBleGapEvtLoadKeysOpCode_c,
                                              fsciBleGapGetSmpKeysBufferSize(pOutKeys) +
                                              sizeof(gapSmpKeyFlags_t) + 2U * sizeof(bool_t));

    if (NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set event parameters in the buffer */
    fsciBleGapGetBufferFromSmpKeys(pOutKeys, &pBuffer);
    fsciBleGetBufferFromEnumValue(*pOutKeyFlags, pBuffer, gapSmpKeyFlags_t);
    fsciBleGetBufferFromBoolValue(*pOutLeSc, pBuffer);
    fsciBleGetBufferFromBoolValue(*pOutAuth, pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}


void fsciBleGapGenericEvtMonitor(gapGenericEvent_t* pGenericEvent)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;
    fsciBleGapOpCode_t          opCode;
    bool_t earlyReturn = FALSE;

#if gFsciBleTest_d
    /* If GAP is disabled the event must be not monitored */
    if ((FALSE == bFsciBleGapEnabled) &&
        (gInitializationComplete_c != pGenericEvent->eventType))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Get FSCI opCode */
    switch(pGenericEvent->eventType)
    {
        case gInitializationComplete_c:
            {
                opCode = gBleGapEvtGenericEventInitializationCompleteOpCode_c;
            }
            break;

        case gInternalError_c:
            {
                opCode = gBleGapEvtGenericEventInternalErrorOpCode_c;
            }
            break;

        case gAdvertisingSetupFailed_c:
            {
                opCode = gBleGapEvtGenericEventAdvertisingSetupFailedOpCode_c;
            }
            break;

        case gAdvertisingParametersSetupComplete_c:
            {
                opCode = gBleGapEvtGenericEventAdvertisingParametersSetupCompleteOpCode_c;
            }
            break;

        case gAdvertisingDataSetupComplete_c:
            {
                opCode = gBleGapEvtGenericEventAdvertisingDataSetupCompleteOpCode_c;
            }
            break;

        case gWhiteListSizeRead_c:
            {
                opCode = gBleGapEvtGenericEventWhiteListSizeReadOpCode_c;
            }
            break;

        case gDeviceAddedToWhiteList_c:
            {
                opCode = gBleGapEvtGenericEventDeviceAddedToWhiteListOpCode_c;
            }
            break;

        case gDeviceRemovedFromWhiteList_c:
            {
                opCode = gBleGapEvtGenericEventDeviceRemovedFromWhiteListOpCode_c;
            }
            break;

        case gWhiteListCleared_c:
            {
                opCode = gBleGapEvtGenericEventWhiteListClearedOpCode_c;
            }
            break;

        case gRandomAddressReady_c:
            {
                opCode = gBleGapEvtGenericEventRandomAddressReadyOpCode_c;
            }
            break;

        case gCreateConnectionCanceled_c:
            {
                opCode = gBleGapEvtGenericEventCreateConnectionCanceledOpCode_c;
            }
            break;

        case gPublicAddressRead_c:
            {
                opCode = gBleGapEvtGenericEventPublicAddressReadOpCode_c;
            }
            break;

        case gAdvTxPowerLevelRead_c:
            {
                opCode = gBleGapEvtGenericEventAdvTxPowerLevelReadOpCode_c;
            }
            break;

        case gPrivateResolvableAddressVerified_c:
            {
                opCode = gBleGapEvtGenericEventPrivateResolvableAddressVerifiedOpCode_c;
            }
            break;

        case gRandomAddressSet_c:
            {
                opCode = gBleGapEvtGenericEventRandomAddressSetOpCode_c;
            }
            break;

        case gLeScPublicKeyRegenerated_c:
            {
                opCode = gBleGapEvtGenericEventLeScPublicKeyRegeneratedOpCode_c;
            }
            break;

        case gLeScLocalOobData_c:
            {
                opCode = gBleGapEvtGenericEventLeScLocalOobDataOpCode_c;
            }
            break;

        case gHostPrivacyStateChanged_c:
            {
                opCode = gBleGapEvtGenericEventHostPrivacyStateChangedOpCode_c;
            }
            break;

        case gControllerPrivacyStateChanged_c:
            {
                opCode = gBleGapEvtGenericEventControllerPrivacyStateChangedOpCode_c;
            }
            break;

        case gTxPowerLevelSetComplete_c:
            {
                opCode = gBleGapEvtGenericEventTxPowerLevelSetCompleteOpCode_c;
            }
            break;

        case gLePhyEvent_c:
            {
                opCode = gBleGapEvtGenericEventLePhyEventOpCode_c;
            }
            break;

        case gControllerNotificationEvent_c:
           {
                opCode = gBleGapEvtGenericEventControllerNotificationOpCode_c;
           }
           break;

        case gBondCreatedEvent_c:
           {
                opCode = gBleGapEvtGenericEventBondCreatedOpCode_c;
           }
           break;

        case gChannelMapSet_c:
           {
                opCode = gBleGapEvtGenericEventChannelMapSetOpCode_c;
           }
           break;

#if defined(gBLE50_d) && (gBLE50_d == 1U)
        case gExtAdvertisingDataSetupComplete_c:
            {
                opCode = gBleGapEvtGenericEventExtAdvertisingDataSetupCompleteOpCode_c;
            }
            break;

        case gExtAdvertisingParametersSetupComplete_c:
            {
                opCode = gBleGapEvtGenericEventExtAdvertisingParametersSetupCompleteOpCode_c;
            }
            break;

        case gPeriodicAdvParamSetupComplete_c:
            {
                opCode = gBleGapEvtGenericEventPeriodicAdvParamSetupCompleteOpCode_c;
            }
            break;

        case gPeriodicAdvDataSetupComplete_c:
            {
                opCode = gBleGapEvtGenericEventPeriodicAdvDataSetupCompleteOpCode_c;
            }
            break;

        case gExtAdvertisingSetRemoveComplete_c:
            {
                opCode = gBleGapEvtAdvertisingEventExtAdvertisingSetRemovedOpCode_c;
            }
            break;

        case gPeriodicAdvListUpdateComplete_c:
            {
                opCode = gBleGapEvtGenericEventPeriodicAdvListUpdateCompleteOpCode_c;
            }
            break;

        case gPeriodicAdvCreateSyncCancelled_c:
            {
                opCode = gBleGapEvtGenericEventPeriodicAdvCreateSyncCancelledOpCode_c;
            }
            break;
#endif
        case gTxEntryAvailable_c:
            {
                opCode = gBleGapEvtGenericEventTxEntryAvailableOpCode_c;
            }
            break;

        default:
            {
                /* Unknown event type */
                fsciBleError(gFsciError_c, fsciBleInterfaceId);
                earlyReturn = TRUE;
                break;
            }
    }

    if(!earlyReturn)
    {
        /* Allocate the packet to be sent over UART */
        pClientPacket = fsciBleGapAllocFsciPacket((uint8_t)opCode,
                                                  fsciBleGapGetGenericEventBufferSize(pGenericEvent));

        if(NULL == pClientPacket)
        {
            return;
        }

        pBuffer = &pClientPacket->payload[0];

        /* Set event parameters in the buffer */
        fsciBleGapGetBufferFromGenericEvent(pGenericEvent, &pBuffer);

        /* Transmit the packet over UART */
        fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
    }
}


void fsciBleGapAdvertisingEvtMonitor(gapAdvertisingEvent_t* pAdvertisingEvent)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;
    fsciBleGapOpCode_t          opCode;
    bool_t earlyReturn = FALSE;

#if gFsciBleTest_d
   /* If GAP is disabled the event must be not monitored */
    if(FALSE == bFsciBleGapEnabled)
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Get FSCI opCode */
    switch(pAdvertisingEvent->eventType)
    {
        case gAdvertisingStateChanged_c:
            {
                opCode = gBleGapEvtAdvertisingEventAdvertisingStateChangedOpCode_c;
            }
            break;

        case gAdvertisingCommandFailed_c:
            {
                opCode = gBleGapEvtAdvertisingEventAdvertisingCommandFailedOpCode_c;
            }
            break;
#if defined(gBLE50_d) && (gBLE50_d == 1U)
        case gExtAdvertisingStateChanged_c:
            {
                opCode = gBleGapEvtAdvertisingEventExtAdvertisingStateChangedOpCode_c;
            }
            break;

        case gAdvertisingSetTerminated_c:
            {
                opCode = gBleGapEvtAdvertisingEventAdvertisingSetTerminatedOpCode_c;
            }
            break;

        case gExtScanNotification_c:
            {
                opCode = gBleGapEvtAdvertisingEventExtScanReqReceivedOpCode_c;
            }
            break;

        case gPeriodicAdvertisingStateChanged_c:
            {
                opCode = gBleGapEvtAdvertisingEventPeriodicAdvertisingStateChangedOpCode_c;
            }
            break;
#endif
        default:
            {
                /* Unknown event type */
                fsciBleError(gFsciError_c, fsciBleInterfaceId);
                earlyReturn = TRUE;
                break;
            }
    }

    if (!earlyReturn)
    {
        /* Allocate the packet to be sent over UART */
        pClientPacket = fsciBleGapAllocFsciPacket((uint8_t)opCode,
                                                  fsciBleGapGetAdvertisingEventBufferSize(pAdvertisingEvent));

        if(NULL == pClientPacket)
        {
            return;
        }

        pBuffer = &pClientPacket->payload[0];

        /* Set event parameters in the buffer */
        fsciBleGapGetBuffFromAdvEvent(pAdvertisingEvent, &pBuffer);

        /* Transmit the packet over UART */
        fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
    }
}


void fsciBleGapScanningEvtMonitor(gapScanningEvent_t* pScanningEvent)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;
    fsciBleGapOpCode_t          opCode;
    bool_t earlyReturn = FALSE;

#if gFsciBleTest_d
    /* If GAP is disabled the event must be not monitored */
    if(FALSE == bFsciBleGapEnabled)
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Get FSCI opCode */
    switch(pScanningEvent->eventType)
    {
        case gScanStateChanged_c:
            {
                opCode = gBleGapEvtScanningEventScanStateChangedOpCode_c;
            }
            break;

        case gScanCommandFailed_c:
            {
                opCode = gBleGapEvtScanningEventScanCommandFailedOpCode_c;
            }
            break;

        case gDeviceScanned_c:
            {
                opCode = gBleGapEvtScanningEventDeviceScannedOpCode_c;
            }
            break;

#if defined(gBLE50_d) && (gBLE50_d == 1U)
        case gExtDeviceScanned_c:
            {
                opCode = gBleGapEvtScanningEventExtDeviceScannedOpCode_c;
            }
            break;

        case gPeriodicDeviceScanned_c:
            {
                opCode = gBleGapEvtScanningEventPeriodicDeviceScannedOpCode_c;
            }
            break;

        case gPeriodicAdvSyncEstablished_c:
            {
                opCode = gBleGapEvtScanningEventPeriodicAdvSyncEstablishedOpCode_c;
            }
            break;

        case gPeriodicAdvSyncTerminated_c:
            {
                opCode = gBleGapEvtScanningEventPeriodicAdvSyncTerminatedOpCode_c;
            }
            break;

        case gPeriodicAdvSyncLost_c:
            {
                opCode = gBleGapEvtScanningEventPeriodicAdvSyncLostOpCode_c;
            }
            break;
#endif

        default:
            {
                /* Unknown event type */
                fsciBleError(gFsciError_c, fsciBleInterfaceId);
                earlyReturn = TRUE;
                break;
            }
    }

    if (!earlyReturn)
    {
        /* Allocate the packet to be sent over UART */
        pClientPacket = fsciBleGapAllocFsciPacket((uint8_t)opCode,
                                                  fsciBleGapGetScanningEventBufferSize(pScanningEvent));

        if(NULL == pClientPacket)
        {
            return;
        }

        pBuffer = &pClientPacket->payload[0];

        /* Set event parameters in the buffer */
        fsciBleGapGetBufferFromScanningEvent(pScanningEvent, &pBuffer);

        /* Transmit the packet over UART */
        fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
    }
}


void fsciBleGapConnectionEvtMonitor(deviceId_t deviceId, gapConnectionEvent_t* pConnectionEvent)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;
    fsciBleGapOpCode_t          opCode;
    bool_t earlyReturn = FALSE;

#if gFsciBleTest_d
    /* If GAP is disabled the event must be not monitored */
    if(FALSE == bFsciBleGapEnabled)
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Get FSCI opCode */
    switch(pConnectionEvent->eventType)
    {
        case gConnEvtConnected_c:
            {
                opCode = gBleGapEvtConnectionEventConnectedOpCode_c;
            }
            break;

        case gConnEvtPairingRequest_c:
            {
                opCode = gBleGapEvtConnectionEventPairingRequestOpCode_c;
            }
            break;

        case gConnEvtSlaveSecurityRequest_c:
            {
                opCode = gBleGapEvtConnectionEventSlaveSecurityRequestOpCode_c;
            }
            break;

        case gConnEvtPairingResponse_c:
            {
                opCode = gBleGapEvtConnectionEventPairingResponseOpCode_c;
            }
            break;

        case gConnEvtAuthenticationRejected_c:
            {
                opCode = gBleGapEvtConnectionEventAuthenticationRejectedOpCode_c;
            }
            break;

        case gConnEvtPasskeyRequest_c:
            {
                opCode = gBleGapEvtConnectionEventPasskeyRequestOpCode_c;
            }
            break;

        case gConnEvtOobRequest_c:
            {
                opCode = gBleGapEvtConnectionEventOobRequestOpCode_c;
            }
            break;

        case gConnEvtPasskeyDisplay_c:
            {
                opCode = gBleGapEvtConnectionEventPasskeyDisplayOpCode_c;
            }
            break;

        case gConnEvtKeyExchangeRequest_c:
            {
                opCode = gBleGapEvtConnectionEventKeyExchangeRequestOpCode_c;
            }
            break;

        case gConnEvtKeysReceived_c:
            {
                opCode = gBleGapEvtConnectionEventKeysReceivedOpCode_c;
            }
            break;

        case gConnEvtLongTermKeyRequest_c:
            {
                opCode = gBleGapEvtConnectionEventLongTermKeyRequestOpCode_c;
            }
            break;

        case gConnEvtEncryptionChanged_c:
            {
                opCode = gBleGapEvtConnectionEventEncryptionChangedOpCode_c;
            }
            break;

        case gConnEvtPairingComplete_c:
            {
                opCode = gBleGapEvtConnectionEventPairingCompleteOpCode_c;
            }
            break;

        case gConnEvtDisconnected_c:
            {
                opCode = gBleGapEvtConnectionEventDisconnectedOpCode_c;
            }
            break;

        case gConnEvtRssiRead_c:
            {
                opCode = gBleGapEvtConnectionEventRssiReadOpCode_c;
            }
            break;

         case gConnEvtTxPowerLevelRead_c:
            {
                opCode = gBleGapEvtConnectionEventTxPowerLevelReadOpCode_c;
            }
            break;

         case gConnEvtPowerReadFailure_c:
            {
                opCode = gBleGapEvtConnectionEventPowerReadFailureOpCode_c;
            }
            break;

        case gConnEvtParameterUpdateRequest_c:
            {
                opCode = gBleGapEvtConnectionEventParameterUpdateRequestOpCode_c;
            }
            break;

        case gConnEvtParameterUpdateComplete_c:
            {
                opCode = gBleGapEvtConnectionEventParameterUpdateCompleteOpCode_c;
            }
            break;

        case gConnEvtLeDataLengthChanged_c:
            {
                opCode = gBleGapEvtConnectionEventLeDataLengthChangedOpCode_c;
            }
            break;

        case gConnEvtLeScOobDataRequest_c:
            {
                opCode = gBleGapEvtConnectionEventLeScOobDataRequestOpCode_c;
            }
            break;

        case gConnEvtLeScDisplayNumericValue_c:
            {
                opCode = gBleGapEvtConnectionEventLeScDisplayNumericValueOpCode_c;
            }
            break;

        case gConnEvtLeScKeypressNotification_c:
            {
                opCode = gBleGapEvtConnectionEventLeScKeypressNotificationOpCode_c;
            }
            break;

        case gConnEvtChannelMapRead_c:
            {
                opCode = gBleGapEvtConnectionEventChannelMapReadOpCode_c;
            }
            break;

        case gConnEvtChannelMapReadFailure_c:
            {
                opCode = gBleGapEvtConnectionEventChannelMapReadFailureOpCode_c;
            }
            break;

#if defined(gBLE50_d) && (gBLE50_d == 1U)
        case gConnEvtChanSelectionAlgorithm2_c:
            {
                opCode = gBleGapEvtConnectionEventChannelSelectionAlgorithm2OpCode_c;
            }
            break;
#endif

        default:
            {
                /* Unknown event type */
                fsciBleError(gFsciError_c, fsciBleInterfaceId);
                earlyReturn= TRUE;
            }
            break;
    }

    if(!earlyReturn)
    {
        /* Allocate the packet to be sent over UART */
        pClientPacket = fsciBleGapAllocFsciPacket((uint8_t)opCode,
                                                  fsciBleGetDeviceIdBufferSize(&deviceId) +
                                                  fsciBleGapGetConnectionEventBufferSize(pConnectionEvent));

        if(NULL == pClientPacket)
        {
            return;
        }

        pBuffer = &pClientPacket->payload[0];

        /* Set event parameters in the buffer */
        fsciBleGetBufferFromDeviceId(&deviceId, &pBuffer);
        fsciBleGapGetBufferFromConnectionEvent(pConnectionEvent, &pBuffer);

        /* Transmit the packet over UART */
        fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
    }
}

#endif /* gFsciBleBBox_d || gFsciBleTest_d */

void fsciBleGapAdvertisingCallback(gapAdvertisingEvent_t* pAdvertisingEvent)
{
    fsciBleGapAdvertisingEvtMonitor(pAdvertisingEvent);
}


void fsciBleGapScanningCallback(gapScanningEvent_t* pScanningEvent)
{
    fsciBleGapScanningEvtMonitor(pScanningEvent);
}


void fsciBleGapConnectionCallback(deviceId_t deviceId, gapConnectionEvent_t* pConnectionEvent)
{
    fsciBleGapConnectionEvtMonitor(deviceId, pConnectionEvent);
}

#if defined(gBLE50_d) && (gBLE50_d == 1U)
void fsciBleGapUpdatePeriodicAdvListCmdMonitor(gapPeriodicAdvListOperation_t operation, bleAddressType_t addrType, uint8_t* pAddr, uint8_t SID)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
    /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
    if((FALSE == bFsciBleGapEnabled) ||
       (TRUE == bFsciBleGapCmdInitiatedByFsci))
    {
        return;
    }
#endif /* gFsciBleTest_d */

    /* Allocate the packet to be sent over UART */
    if (operation != gRemoveAllDevices_c)
    {
        pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdUpdatePeriodicAdvListOpCode_c,
                                                     sizeof(gapPeriodicAdvListOperation_t) + sizeof(bleAddressType_t) + gcBleDeviceAddressSize_c + sizeof(uint8_t));
    }
    else
    {
        pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdUpdatePeriodicAdvListOpCode_c, sizeof(gapPeriodicAdvListOperation_t));
    }

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGetBufferFromEnumValue(operation, pBuffer, gapPeriodicAdvListOperation_t);
    /* the clear operation does not require parameters */
    if (operation != gRemoveAllDevices_c)
    {
        fsciBleGetBufferFromEnumValue(addrType, pBuffer, bleAddressType_t);
        fsciBleGetBufferFromAddress(pAddr, pBuffer);
        fsciBleGetBufferFromUint8Value(SID, pBuffer);
    }

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}

void fsciBleGapPeriodicAdvCreateSyncCmdMonitor
(
    gapPeriodicAdvSyncReq_t * pReq,
    gapScanningCallback_t     scanningCallback
)
{
    clientPacketStructured_t*   pClientPacket;
    uint8_t*                    pBuffer;

#if gFsciBleTest_d
        /* If GAP is disabled or if the command was initiated by FSCI it must be not monitored */
        if((FALSE == bFsciBleGapEnabled) ||
           (TRUE == bFsciBleGapCmdInitiatedByFsci))
        {
            return;
        }
#endif /* gFsciBleTest_d */

#if gFsciBleHost_d
    fsciBleGapCallbacks.scanningCallback = scanningCallback;
#endif /* gFsciBleHost_d */

    /* Allocate the packet to be sent over UART */
    pClientPacket = fsciBleGapAllocFsciPacket(gBleGapCmdPeriodicAdvCreateSyncOpCode_c,
                                              fsciBleGapGetPeriodicAdvSyncReqParametersBufferSize(pReq));

    if(NULL == pClientPacket)
    {
        return;
    }

    pBuffer = &pClientPacket->payload[0];

    /* Set command parameters in the buffer */
    fsciBleGapGetBufferFromPerAdvSyncReq(pReq, &pBuffer);

    /* Transmit the packet over UART */
    fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceId);
}
#endif

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

static void fsciBleGapGenericCallback(gapGenericEvent_t* pGenericEvent)
{
    fsciBleGapGenericEvtMonitor(pGenericEvent);
}


static bleResult_t fsciBleHciHostToControllerInterface(hciPacketType_t packetType, void* pPacket, uint16_t packetSize)
{
    return gBleSuccess_c;
}

#endif /* gFsciIncluded_c && gFsciBleGapLayerEnabled_d */

/*! *********************************************************************************
* @}
********************************************************************************** */
