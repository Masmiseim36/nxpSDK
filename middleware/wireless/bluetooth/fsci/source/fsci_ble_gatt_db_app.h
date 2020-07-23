/*! *********************************************************************************
 * \defgroup FSCI_BLE
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This is a header file for FSCI BLE management.
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef FSCI_BLE_GATT_DB_APP_H
#define FSCI_BLE_GATT_DB_APP_H

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/

#include "fsci_ble_gatt_db_app_types.h"

/************************************************************************************
*************************************************************************************
* Public constants & macros
*************************************************************************************
************************************************************************************/

/*! Macro which indicates if FSCI for GATT Database (application) (application) is enabled or not */
#ifndef gFsciBleGattDbAppLayerEnabled_d
    #define gFsciBleGattDbAppLayerEnabled_d         0
#endif /* gFsciBleGattDbAppLayerEnabled_d */

/*! FSCI operation group for GATT Database (application) */
#define gFsciBleGattDbAppOpcodeGroup_c              0x46


#if defined(FsciCmdMonitor)
    //#warning "FsciCmdMonitor macro is already defined"
    #undef FsciCmdMonitor
#endif
    
#if defined(FsciStatusMonitor)
    //#warning "FsciStatusMonitor macro is already defined"
    #undef FsciStatusMonitor
#endif

#if defined(FsciEvtMonitor)
    //#warning "FsciEvtMonitor macro is already defined"
    #undef FsciEvtMonitor
#endif

/*! Macros used for monitoring commands, statuses and events */
#if gFsciIncluded_c && gFsciBleGattDbAppLayerEnabled_d

    #if gFsciBleHost_d || gFsciBleTest_d
        #define FsciCmdMonitor(function, ...)       fsciBleGattDbApp##function##CmdMonitor(__VA_ARGS__)
    #else
        #define FsciCmdMonitor(function, ...)
    #endif
        
    #if gFsciBleBBox_d || gFsciBleTest_d
        #define FsciStatusMonitor(function, ...)    fsciBleGattDbAppStatusMonitor(__VA_ARGS__)
        #define FsciEvtMonitor(function, ...)       fsciBleGattDbApp##function##EvtMonitor(__VA_ARGS__)
    #else
        #define FsciStatusMonitor(function, ...)
        #define FsciEvtMonitor(function, ...)   
    #endif

#else

    #define FsciCmdMonitor(function, ...)
    #define FsciStatusMonitor(function, ...)
    #define FsciEvtMonitor(function, ...)

#endif


/*! *********************************************************************************
* \brief  Allocates a FSCI packet for GATT Database (application).
*
* \param[in]    opCode      FSCI GATT Database (application) operation code
* \param[in]    dataSize    Size of the payload
*
* \return The allocated FSCI packet
*
********************************************************************************** */
#define fsciBleGattDbAppAllocFsciPacket(opCode,                 \
                                        dataSize)               \
        fsciBleAllocFsciPacket(gFsciBleGattDbAppOpcodeGroup_c,  \
                               (opCode),                          \
                               (dataSize))

/*! *********************************************************************************
* \brief  GattDb_FindServiceHandle command monitoring macro.
*
* \param[in]  startHandle              The handle to start the search. Should be 0x0001 on the first call.
* \param[in]  serviceUuidType          Service UUID type.
* \param[in]  pServiceUuid             Service UUID.
* \param[out] pOutServiceHandle        Pointer to the service declaration handle to be written.
*
********************************************************************************** */
#define fsciBleGattDbAppFindServiceHandleCmdMonitor(startHandle,            \
                                                    serviceUuidType,        \
                                                    pServiceUuid,           \
                                                    pOutServiceHandle)      \
        fsciBleGattDbAppFindServiceCharValueOrDescriptorHandleCmdMonitor(   \
                        gBleGattDbAppCmdFindServiceHandleOpCode_c,          \
                        (startHandle),                                        \
                        (serviceUuidType),                                    \
                        (pServiceUuid),                                       \
                        (pOutServiceHandle))

/*! *********************************************************************************
* \brief  GattDb_FindCharValueHandleInService command monitoring macro.
*
* \param[in]    serviceHandle           The handle of the Service declaration.
* \param[in]    characteristicUuidType  CharacteristicUUID type.
* \param[in]    pCharacteristicUuid     CharacteristicUUID.
* \param[out]   pOutCharValueHandle     Pointer to the characteristic value handle 
*                                       to be written.
*
********************************************************************************** */
#define fsciBleGattDbAppFindCharValueHandleInServiceCmdMonitor(serviceHandle,           \
                                                               characteristicUuidType,  \
                                                               pCharacteristicUuid,     \
                                                               pOutCharValueHandle)     \
        fsciBleGattDbAppFindServiceCharValueOrDescriptorHandleCmdMonitor(               \
                        gBleGattDbAppCmdFindCharValueHandleInServiceOpCode_c,           \
                        (serviceHandle),                                                  \
                        (characteristicUuidType),                                         \
                        (pCharacteristicUuid),                                            \
                        (pOutCharValueHandle))

/*! *********************************************************************************
* \brief  GattDb_FindDescriptorHandleForCharValueHandle command monitoring macro. 
*
* \param[in]    charValueHandle         The handle of the Service declaration.
* \param[in]    descriptorUuidType      Descriptor's UUID type.
* \param[in]    pDescriptorUuid         Descriptor's UUID.
* \param[out]   pOutCharValueHandle     Pointer to the Descriptor handle to be written.
*
********************************************************************************** */
#define fsciBleGattDbAppFindDescriptorHandleForCharValueHandleCmdMonitor(charValueHandle,       \
                                                                         descriptorUuidType,    \
                                                                         pDescriptorUuid,       \
                                                                         pOutHandle)            \
        fsciBleGattDbAppFindServiceCharValueOrDescriptorHandleCmdMonitor(                       \
                        gBleGattDbAppCmdFindDescriptorHandleForCharValueHandleOpCode_c,         \
                        (charValueHandle),                                                        \
                        (descriptorUuidType),                                                     \
                        (pDescriptorUuid),                                                        \
                        (pOutHandle))

/*! *********************************************************************************
* \brief  GattDbDynamic_Init command monitoring macro.
*
********************************************************************************** */
#define fsciBleGattDbAppInitDatabaseCmdMonitor() \
        fsciBleGattDbAppNoParamCmdMonitor(gBleGattDbAppCmdInitDatabaseOpCode_c)

/*! *********************************************************************************
* \brief  GattDbDynamic_ReleaseDatabase command monitoring macro.
*
********************************************************************************** */
#define fsciBleGattDbAppReleaseDatabaseCmdMonitor() \
        fsciBleGattDbAppNoParamCmdMonitor(gBleGattDbAppCmdReleaseDatabaseOpCode_c)

/*! *********************************************************************************
* \brief  GattDbDynamic_AddPrimaryServiceDeclaration command monitoring macro.
*
* \param[in]    desiredHandle
* \param[in]    serviceUuidType
* \param[in]    pServiceUuid
* \param[out]   pOutHandle          
*
********************************************************************************** */
#define fsciBleGattDbAppAddPrimaryServiceDeclarationCmdMonitor(desiredHandle,                                   \
                                                               serviceUuidType,                                 \
                                                               pServiceUuid,                                    \
                                                               pOutHandle)                                      \
        fsciBleGattDbAppUuidTypeAndUuidParamCmdMonitor(gBleGattDbAppCmdAddPrimaryServiceDeclarationOpCode_c,    \
                                                    (desiredHandle),                                            \
                                                    (serviceUuidType),                                          \
                                                    (pServiceUuid),                                               \
                                                    (pOutHandle))
                                                       
/*! *********************************************************************************
* \brief  GattDbDynamic_AddSecondaryServiceDeclaration command monitoring macro.
*
* \param[in]    serviceUuidType
* \param[in]    pServiceUuid
* \param[out]   pOutHandle          
*
********************************************************************************** */
#define fsciBleGattDbAppAddSecondaryServiceDeclarationCmdMonitor(desiredHandle,                                 \
                                                                 serviceUuidType,                               \
                                                                 pServiceUuid,                                  \
                                                                 pOutHandle)                                    \
        fsciBleGattDbAppUuidTypeAndUuidParamCmdMonitor(gBleGattDbAppCmdAddSecondaryServiceDeclarationOpCode_c,  \
                                                    (desiredHandle),                                            \
                                                    (serviceUuidType),                                          \
                                                    (pServiceUuid),                                             \
                                                    (pOutHandle))

/*! *********************************************************************************
* \brief    GattDbDynamic_RemoveService command monitoring macro.
*
* \param[in]  serviceHandle    Attribute handle of the Service declaration.
*
********************************************************************************** */
#define fsciBleGattDbAppRemoveServiceCmdMonitor(serviceHandle)                          \
        fsciBleGattDbAppUint16ParamCmdMonitor(gBleGattDbAppCmdRemoveServiceOpCode_c,    \
                                              (serviceHandle))

/*! *********************************************************************************
* \brief    GattDbDynamic_RemoveCharacteristic command monitoring macro.
*
* \param[in]  characteristicHandle    Attribute handle of the Characteristic declaration.
*
********************************************************************************** */
#define fsciBleGattDbAppRemoveCharacteristicCmdMonitor(characteristicHandle)                \
        fsciBleGattDbAppUint16ParamCmdMonitor(gBleGattDbAppCmdRemoveCharacteristicOpCode_c, \
                                              (characteristicHandle))

/*! *********************************************************************************
* \brief  GattDb_FindServiceHandle command out parameters monitoring 
*         function.
*
* \param[in]    pOutCharValueHandle     Pointer to the service declaration handle to be written.
*
********************************************************************************** */
#define fsciBleGattDbAppFindServiceHandleEvtMonitor(pOutServiceHandle)                      \
        fsciBleGattDbAppUint16ParamEvtMonitor(gBleGattDbAppEvtFindServiceHandleOpCode_c,    \
                                              (pOutServiceHandle))

/*! *********************************************************************************
* \brief  GattDb_FindCharValueHandleInService command out parameters monitoring 
*         function.
*
* \param[in]    pOutCharValueHandle     Pointer to the characteristic value handle to be written.
*
********************************************************************************** */
#define fsciBleGattDbAppFindCharValueHandleInServiceEvtMonitor(pOutCharValueHandle)                 \
        fsciBleGattDbAppUint16ParamEvtMonitor(gBleGattDbAppEvtFindCharValueHandleInServiceOpCode_c, \
                                              (pOutCharValueHandle))

/*! *********************************************************************************
* \brief  GattDb_FindCccdHandleForCharValueHandle command out parameters monitoring 
*         function.
*
* \param[in]    pOutCccdHandle      Pointer to the CCCD handle to be written.
*
********************************************************************************** */
#define fsciBleGattDbAppFindCccdHandleForCharValueHandleEvtMonitor(pOutCccdHandle)                      \
        fsciBleGattDbAppUint16ParamEvtMonitor(gBleGattDbAppEvtFindCccdHandleForCharValueHandleOpCode_c, \
                                              (pOutCccdHandle))

/*! *********************************************************************************
* \brief  GattDb_FindDescriptorHandleForCharValueHandle command out parameters monitoring 
*         function.
*
* \param[in]    pOutDescriptorHandle    Pointer to the Descriptor handle to be written.
*
********************************************************************************** */
#define fsciBleGattDbAppFindDescriptorHandleForCharValueHandleEvtMonitor(pOutDescriptorHandle)                  \
        fsciBleGattDbAppUint16ParamEvtMonitor(gBleGattDbAppEvtFindDescriptorHandleForCharValueHandleOpCode_c,   \
                                              (pOutDescriptorHandle))

/*! *********************************************************************************
* \brief  GattDbDynamic_AddPrimaryServiceDeclaration command out parameters monitoring 
*         function.
*
* \param[in]    pOutHandle      Resulting attribute handle of the Service declaration. 
*                               Ignored if NULL.
*
********************************************************************************** */
#define fsciBleGattDbAppAddPrimaryServiceDeclarationEvtMonitor(pOutHandle)                          \
        fsciBleGattDbAppUint16ParamEvtMonitor(gBleGattDbAppEvtAddPrimaryServiceDeclarationOpCode_c, \
                                              (pOutHandle))

/*! *********************************************************************************
* \brief  GattDbDynamic_AddSecondaryServiceDeclaration command out parameters monitoring 
*         function.
*
* \param[in]    pOutHandle      Resulting attribute handle of the Service declaration. 
*                               Ignored if NULL.
*
********************************************************************************** */
#define fsciBleGattDbAppAddSecondaryServiceDeclarationEvtMonitor(pOutHandle)                            \
        fsciBleGattDbAppUint16ParamEvtMonitor(gBleGattDbAppEvtAddSecondaryServiceDeclarationOpCode_c,   \
                                              (pOutHandle))

/*! *********************************************************************************
* \brief  GattDbDynamic_AddIncludeDeclaration command out parameters monitoring 
*         function.
*
* \param[in]    pOutHandle      Resulting attribute handle of the Include declaration. 
*                               Ignored if NULL.
*
********************************************************************************** */
#define fsciBleGattDbAppAddIncludeDeclarationEvtMonitor(pOutHandle)                             \
        fsciBleGattDbAppUint16ParamEvtMonitor(gBleGattDbAppEvtAddIncludeDeclarationOpCode_c,    \
                                              (pOutHandle))

/*! *********************************************************************************
* \brief  GattDbDynamic_AddCharacteristicDeclarationAndValue command out parameters monitoring 
*         function.
*
* \param[[in]    pOutHandle     Resulting attribute handle of the Characteristic declaration.
*                               Ignored if NULL.          
*
********************************************************************************** */
#define fsciBleGattDbAppAddCharacteristicDeclarationAndValueEvtMonitor(pOutHandle)                          \
        fsciBleGattDbAppUint16ParamEvtMonitor(gBleGattDbAppEvtAddCharacteristicDeclarationAndValueOpCode_c, \
                                              (pOutHandle))

/*! *********************************************************************************
* \brief  GattDbDynamic_AddCharDescriptor command out parameters monitoring 
*         function.
*
* \param[[in]    pOutHandle     Resulting attribute handle of the Descriptor declaration. 
*                               Ignored if NULL.
*
********************************************************************************** */
#define fsciBleGattDbAppAddCharacteristicDescriptorEvtMonitor(pOutHandle)                           \
        fsciBleGattDbAppUint16ParamEvtMonitor(gBleGattDbAppEvtAddCharacteristicDescriptorOpCode_c,  \
                                              (pOutHandle))

/*! *********************************************************************************
* \brief  GattDbDynamic_AddCccd command out parameters monitoring 
*         function.
*
* \param[out]   pOutHandle      Resulting attribute handle of the CCCD. Ignored if NULL.
*
********************************************************************************** */
#define fsciBleGattDbAppAddCccdEvtMonitor(pOutHandle)                           \
        fsciBleGattDbAppUint16ParamEvtMonitor(gBleGattDbAppEvtAddCccdOpCode_c,  \
                                              (pOutHandle))

/*! *********************************************************************************
* \brief  GattDbDynamic_AddCharDeclWithUniqueValue command out parameters monitoring 
*         function.
*
* \param[out]   pOutHandle      Resulting attribute handle of the Characteristic declaration. 
*                               Ignored if NULL.
*
********************************************************************************** */
#define fsciBleGattDbAppAddCharacteristicDeclarationWithUniqueValueEvtMonitor(pOutHandle)                           \
        fsciBleGattDbAppUint16ParamEvtMonitor(gBleGattDbAppEvtAddCharacteristicDeclarationWithUniqueValueOpCode_c,  \
                                              (pOutHandle))
          
/*! *********************************************************************************
* \brief  GattDbDynamic_AddCharDescriptorWithUniqueValue command out parameters monitoring 
*         function.
*
* \param[out]   pOutHandle      Resulting attribute handle of the Characteristic declaration. 
*                               Ignored if NULL.
*
********************************************************************************** */
#define fsciBleGattDbAppAddCharDescriptorWithUniqueValueEvtMonitor(pOutHandle)                           \
        fsciBleGattDbAppUint16ParamEvtMonitor(gBleGattDbAppEvtAddCharDescriptorWithUniqueValueOpCode_c,  \
                                              (pOutHandle))

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/*! FSCI operation codes for GATT Database (application) */
typedef enum
{
    gBleGattDbAppModeSelectOpCode_c             = 0x00,                             /*! GATT Database (application) Mode Select operation code */

    gBleGattDbAppCmdFirstOpCode_c               = 0x01,
    gFsciBleGattDbReserved1_c                   = gBleGattDbAppCmdFirstOpCode_c,    /*! Removed: GattDb_Init command operation code */
    gBleGattDbAppCmdWriteAttributeOpCode_c,                                         /*! GattDb_WriteAttribute command operation code */
    gBleGattDbAppCmdReadAttributeOpCode_c,                                          /*! GattDb_ReadAttribute command operation code */
    gBleGattDbAppCmdFindServiceHandleOpCode_c,                                      /*! GattDb_FindServiceHandle command operation code */
    gBleGattDbAppCmdFindCharValueHandleInServiceOpCode_c,                           /*! GattDb_FindCharValueHandleInService command operation code */
    gBleGattDbAppCmdFindCccdHandleForCharValueHandleOpCode_c,                       /*! GattDb_FindCccdHandleForCharValueHandle command operation code */
    gBleGattDbAppCmdFindDescriptorHandleForCharValueHandleOpCode_c,                 /*! GattDb_FindDescriptorHandleForCharValueHandle command operation code */
    gBleGattDbAppCmdInitDatabaseOpCode_c,                                           /*! GattDbDynamic_Init command operation code */
    gBleGattDbAppCmdReleaseDatabaseOpCode_c,                                        /*! GattDbDynamic_ReleaseDatabase command operation code */
    gBleGattDbAppCmdAddPrimaryServiceDeclarationOpCode_c,                           /*! GattDbDynamic_AddPrimaryServiceDeclaration command operation code */
    gBleGattDbAppCmdAddSecondaryServiceDeclarationOpCode_c,                         /*! GattDbDynamic_AddSecondaryServiceDeclaration command operation code */
    gBleGattDbAppCmdAddIncludeDeclarationOpCode_c,                                  /*! GattDbDynamic_AddIncludeDeclaration command operation code */
    gBleGattDbAppCmdAddCharacteristicDeclarationAndValueOpCode_c,                   /*! GattDbDynamic_AddCharacteristicDeclarationAndValue command operation code */
    gBleGattDbAppCmdAddCharacteristicDescriptorOpCode_c,                            /*! GattDbDynamic_AddCharDescriptor command operation code */
    gBleGattDbAppCmdAddCccdOpCode_c,                                                /*! GattDbDynamic_AddCccd command operation code */
    gBleGattDbAppCmdAddCharacteristicDeclarationWithUniqueValueOpCode_c,            /*! GattDbDynamic_AddCharDeclWithUniqueValue command operation code */
    gBleGattDbAppCmdRemoveServiceOpCode_c,                                          /*! GattDbDynamic_RemoveService command operation code */
    gBleGattDbAppCmdRemoveCharacteristicOpCode_c,                                   /*! GattDbDynamic_RemoveCharacteristic command operation code */
    gBleGattDbAppCmdAddCharDescriptorWithUniqueValueOpCode_c,                       /*! GattDbDynamic_AddCharDescriptorWithUniqueValue command operation code */
    
    gBleGattDbAppStatusOpCode_c                 = 0x80,                             /*! GATT Database (application) status operation code */

    gBleGattDbAppEvtFirstOpCode_c               = 0x81,
    gBleGattDbAppEvtReadAttributeValueOpCode_c  = gBleGattDbAppEvtFirstOpCode_c,    /*! GattDb_ReadAttributeValue command out parameters event operation code */
    gBleGattDbAppEvtFindServiceHandleOpCode_c,                                      /*! GattDb_FindServiceHandle command out parameters event operation code */
    gBleGattDbAppEvtFindCharValueHandleInServiceOpCode_c,                           /*! GattDb_FindCharValueHandleInService command out parameters event operation code */
    gBleGattDbAppEvtFindCccdHandleForCharValueHandleOpCode_c,                       /*! GattDb_FindCccdHandleForCharValueHandle command out parameters event operation code */
    gBleGattDbAppEvtFindDescriptorHandleForCharValueHandleOpCode_c,                 /*! GattDb_FindDescriptorHandleForCharValueHandle command out parameters event operation code */
    gBleGattDbAppEvtAddPrimaryServiceDeclarationOpCode_c,                           /*! GattDbDynamic_AddPrimaryServiceDeclaration command out parameters event operation code */
    gBleGattDbAppEvtAddSecondaryServiceDeclarationOpCode_c,                         /*! GattDbDynamic_AddSecondaryServiceDeclaration command out parameters event operation code */
    gBleGattDbAppEvtAddIncludeDeclarationOpCode_c,                                  /*! GattDbDynamic_AddIncludeDeclaration command out parameters event operation code */
    gBleGattDbAppEvtAddCharacteristicDeclarationAndValueOpCode_c,                   /*! GattDbDynamic_AddCharacteristicDeclarationAndValue command out parameters event operation code */
    gBleGattDbAppEvtAddCharacteristicDescriptorOpCode_c,                            /*! GattDbDynamic_AddCharDescriptor command out parameters event operation code */
    gBleGattDbAppEvtAddCccdOpCode_c,                                                /*! GattDbDynamic_AddCccd command out parameters event operation code */
    gBleGattDbAppEvtAddCharacteristicDeclarationWithUniqueValueOpCode_c,            /*! GattDbDynamic_AddCharDeclWithUniqueValue command out parameters event operation code */      
    gBleGattDbAppEvtAddCharDescriptorWithUniqueValueOpCode_c,                       /*! GattDbDynamic_AddCharDescriptorWithUniqueValue command out parameters event operation code */
}fsciBleGattDbAppOpCode_t;

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

/*! *********************************************************************************
* \brief  Calls the GATT Database (application) function associated with the operation code received 
*         over UART. The GAP function parameters are extracted from the received 
*         FSCI payload.
*
* \param[in]    pData               Packet (containing FSCI header and FSCI 
*                                   payload) received over UART   
* \param[in]    pParam              Pointer given when this function is registered in
*                                   FSCI
* \param[in]    fsciInterfaceId     FSCI interface on which the packet was received  
*
********************************************************************************** */
void fsciBleGattDbAppHandler
(
    void*       pData, 
    void*       pParam, 
    uint32_t    fsciInterfaceId
);

/*! *********************************************************************************
* \brief  Creates the FSCI packet which contains the status of the last executed GAP 
*         command and sends it over UART.
*
* \param[in]    result      Status of the last executed GATT Database (application) command.
*
********************************************************************************** */
void fsciBleGattDbAppStatusMonitor
(
    bleResult_t result
);

#if gFsciBleHost_d || gFsciBleTest_d
/*! *********************************************************************************
* \brief  Creates a GATT Database (application) FSCI packet without payload and sends it over UART.
*
* \param[in]    opCode      GATT Database (application) command operation code.
*
********************************************************************************** */
void fsciBleGattDbAppNoParamCmdMonitor
(
    fsciBleGattDbAppOpCode_t opCode
);

/*! *********************************************************************************
* \brief  Creates a GATT Database FSCI packet with an uint16_t parameter as payload 
*         and sends it over UART.
*
* \param[in]    opCode      GATT Database (application) command operation code.
* \param[in]    value       A 16 bits value.
*
********************************************************************************** */
void fsciBleGattDbAppUint16ParamCmdMonitor
(
    fsciBleGattDbAppOpCode_t    opCode, 
    uint16_t                    value
);

/*! *********************************************************************************
* \brief  Creates a GATT Database FSCI packet with two parameters (uuidType and uuid)
*         as payload and sends it over UART.
*
* \param[in]    opCode          Application GATT Database command operation code.
* \param[in]    desiredHandle   Desired primary/secondary service handle.
* \param[in]    uuidType        UUID type.
* \param[in]    pUuid           Service UUID.
* \param[out]   pOutHandle      Allocated handle for this service.
*                          
********************************************************************************** */
void fsciBleGattDbAppUuidTypeAndUuidParamCmdMonitor
(
    fsciBleGattDbAppOpCode_t    opCode,
    uint16_t                    desiredHandle,
    bleUuidType_t               uuidType, 
    const bleUuid_t*            pUuid,
    uint16_t*                   pOutHandle
);

/*! *********************************************************************************
* \brief  GattDb_WriteAttribute command monitoring function
*
* \param[in]    handle          The handle of the attribute to be written.
* \param[in]    aValue          The source buffer containing the value to be written.
* \param[in]    valueLength     The number of bytes to be written.
*
********************************************************************************** */
void fsciBleGattDbAppWriteAttributeCmdMonitor
(
    uint16_t       handle, 
    uint16_t       valueLength, 
    const uint8_t* aValue
);

/*! *********************************************************************************
* \brief  GattDb_ReadAttribute command monitoring function
*
* \param[in]    handle              The handle of the attribute to be read.
* \param[in]    maxBytes            The maximum number of bytes to be received.
* \param[out]   aOutValue           The pre-allocated buffer ready to receive the bytes.
* \param[out]   pOutValueLength     The actual number of bytes received.
*
********************************************************************************** */
void fsciBleGattDbAppReadAttributeCmdMonitor
(
    uint16_t    handle, 
    uint16_t    maxBytes, 
    uint8_t*    aOutValue, 
    uint16_t*   pOutValueLength
);

/*! *********************************************************************************
* \brief  GattDb_FindCccdHandleForCharValueHandle command monitoring function
*
* \param[in]    charValueHandle     The handle of the Service declaration.
* \param[out]   pOutCccdHandle      Pointer to the CCCD handle to be written.
*
********************************************************************************** */
void fsciBleGattDbAppFindCccdHandleForCharValueHandleCmdMonitor
(
    uint16_t    charValueHandle, 
    uint16_t*   pOutCccdHandle
);

/*! *********************************************************************************
* \brief  GattDb_FindServiceHandle, GattDb_FindCharValueHandleInService and 
*         GattDb_FindDescriptorHandleForCharValueHandle commands monitoring function
*
* \param[in]    opCode          GATT Database (application) command operation code.
* \param[in]    handle          The given handle.
* \param[in]    uuidType        Service, Characteristic or Descriptor UUID type.
* \param[in]    pUuid           Service, Characteristic or Descriptor UUID.
* \param[out]   pOutHandle      Pointer to the service, characteristic or descriptor 
*                               value handle to be written.
*
********************************************************************************** */
void fsciBleGattDbAppFindServiceCharValueOrDescriptorHandleCmdMonitor
(
    fsciBleGattDbAppOpCode_t    opCode, 
    uint16_t                    handle, 
    bleUuidType_t               uuidType, 
    const bleUuid_t*            pUuid, 
    uint16_t*                   pOutHandle
);

/*! *********************************************************************************
* \brief  GattDbDynamic_AddIncludeDeclaration command monitoring function.
*
* \param[in]    includedServiceHandle
* \param[in]    endGroupHandle
* \param[in]    serviceUuidType
* \param[in]    pServiceUuid
* \param[out]   pOutHandle
*
********************************************************************************** */
void fsciBleGattDbAppAddIncludeDeclarationCmdMonitor
(
    uint16_t         includedServiceHandle,
    uint16_t         endGroupHandle,
    bleUuidType_t    serviceUuidType,
    const bleUuid_t* pServiceUuid,
    uint16_t*        pOutHandle 
);

/*! *********************************************************************************
* \brief  GattDbDynamic_AddCharacteristicDeclarationAndValue command monitoring function.
*
* \param[in]    characteristicUuidType
* \param[in]    pCharacteristicUuid
* \param[in]    characteristicProperties
* \param[in]    maxValueLength
* \param[in]    initialValueLength
* \param[in]    aInitialValue
* \param[in]    valueAccessPermissions
* \param[out]   pOutHandle
*
********************************************************************************** */
void fsciBleGattDbAppAddCharacteristicDeclarationAndValueCmdMonitor
(
    bleUuidType_t                               characteristicUuidType,
    const bleUuid_t*                            pCharacteristicUuid,
    gattCharacteristicPropertiesBitFields_t     characteristicProperties,
    uint16_t                                    maxValueLength,
    uint16_t                                    initialValueLength,
    const uint8_t*                              aInitialValue,
    gattAttributePermissionsBitFields_t         valueAccessPermissions,
    uint16_t*                                   pOutHandle
);

/*! *********************************************************************************
* \brief  GattDbDynamic_AddCharDeclWithUniqueValue command monitoring function.
*
* \param[in]    characteristicUuidType
* \param[in]    pCharacteristicUuid
* \param[in]    characteristicProperties
* \param[in]    valueAccessPermissions
* \param[out]   pOutHandle
*
********************************************************************************** */
void fsciBleGattDbAppAddCharDeclWithUniqueValCmdMonitor
(
    bleUuidType_t                               characteristicUuidType,
    const bleUuid_t*                            pCharacteristicUuid,
    gattCharacteristicPropertiesBitFields_t     characteristicProperties,
    gattAttributePermissionsBitFields_t         valueAccessPermissions,
    uint16_t*                                   pOutHandle 
);

/*! *********************************************************************************
* \brief  GattDbDynamic_AddCharDescriptor command monitoring function.
*
* \param[in]    descriptorUuidType
* \param[in]    pDescriptorUuid
* \param[in]    descriptorValueLength
* \param[in]    aInitialValue
* \param[in]    descriptorAccessPermissions
* \param[out]   pOutHandle
*
********************************************************************************** */
void fsciBleGattDbAppAddCharDescrCmdMonitor
(
    bleUuidType_t                               descriptorUuidType,
    const bleUuid_t*                            pDescriptorUuid,
    uint16_t                                    descriptorValueLength,
    const uint8_t*                              aInitialValue,
    gattAttributePermissionsBitFields_t         descriptorAccessPermissions,
    uint16_t*                                   pOutHandle 
);

/*! *********************************************************************************
* \brief  GattDbDynamic_AddCccd command monitoring function.
*
* \param[out]   pOutHandle          
*
********************************************************************************** */
void fsciBleGattDbAppAddCccdCmdMonitor
(
    uint16_t* pOutHandle
);

/*! *********************************************************************************
* \brief  GattDbDynamic_AddCharDescriptorWithUniqueValue command monitoring function.
*
* \param[in]    descriptorUuidType
* \param[in]    pDescriptorUuid
* \param[in]    descriptorAccessPermissions
* \param[out]   pOutHandle
*
********************************************************************************** */
void fsciBleGattDbAppAddCharDescriptorWithUniqueValueCmdMonitor
(
    bleUuidType_t                               descriptorUuidType,
    bleUuid_t*                                  pDescriptorUuid,
    gattAttributePermissionsBitFields_t         descriptorAccessPermissions,
    uint16_t*                                   pOutHandle 
);

#endif /* gFsciBleHost_d || gFsciBleTest_d */
/*! *********************************************************************************
* \brief  GattDb_ReadAttribute command out parameters monitoring function
*
* \param[in]    aOutValue           The pre-allocated buffer ready to receive the bytes.
* \param[in]    pOutValueLength     The actual number of bytes received.
* \param[in]    pOutHandle
*
********************************************************************************** */
void fsciBleGattDbAppReadAttributeEvtMonitor
(
    uint8_t*    aOutValue, 
    uint16_t*   pOutValueLength
);

/*! *********************************************************************************
* \brief  GattDb_FindCharValueHandleInService, GattDb_FindCccdHandleForCharValueHandle 
*         and GattDb_FindDescriptorHandleForCharValueHandle commands out parameter 
*         monitoring function
*
* \param[in]    opCode      GATT Database (application) command operation code.
* \param[in]    pValue      Pointer to a 16 bits value.
*
********************************************************************************** */
void fsciBleGattDbAppUint16ParamEvtMonitor
(
    fsciBleGattDbAppOpCode_t    opCode, 
    const uint16_t*             pValue
);

#ifdef __cplusplus
}
#endif 

#endif /* FSCI_BLE_GATT_DB_APP_H */

/*! *********************************************************************************
* @}
********************************************************************************** */
