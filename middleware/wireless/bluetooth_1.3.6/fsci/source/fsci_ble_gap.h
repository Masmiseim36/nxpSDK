/*! *********************************************************************************
 * \defgroup FSCI_BLE
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
* All rights reserved.
*
* \file
*
* This is a header file for FSCI BLE management.
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef FSCI_BLE_GAP_H
#define FSCI_BLE_GAP_H

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/

#include "fsci_ble_gap_types.h"

/************************************************************************************
*************************************************************************************
* Public constants & macros
*************************************************************************************
************************************************************************************/

/*! Macro which indicates if FSCI for GAP is enabled or not */
#ifndef gFsciBleGapLayerEnabled_d
    #define gFsciBleGapLayerEnabled_d           0
#endif /* gFsciBleGapLayerEnabled_d */

/*! FSCI operation group for GAP */
#define gFsciBleGapOpcodeGroup_c                0x48


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
#if gFsciIncluded_c && gFsciBleGapLayerEnabled_d

    #if gFsciBleHost_d || gFsciBleTest_d
        #define FsciCmdMonitor(function, ...)       fsciBleGap##function##CmdMonitor(__VA_ARGS__)
    #else
        #define FsciCmdMonitor(function, ...)
    #endif

    #if gFsciBleBBox_d || gFsciBleTest_d
        #define FsciStatusMonitor(function, ...)    fsciBleGapStatusMonitor(__VA_ARGS__)
        #define FsciEvtMonitor(function, ...)       fsciBleGap##function##EvtMonitor(__VA_ARGS__)
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
* \brief  Allocates a FSCI packet for GAP.
*
* \param[in]    opCode      FSCI GAP operation code
* \param[in]    dataSize    Size of the payload
*
* \return The allocated FSCI packet
*
********************************************************************************** */
#define fsciBleGapAllocFsciPacket(opCode, dataSize)         \
        fsciBleAllocFsciPacket(gFsciBleGapOpcodeGroup_c,    \
                               (opCode),                      \
                               (dataSize))

/*! *********************************************************************************
* \brief  Gap_StopAdvertising command monitoring macro.
*
********************************************************************************** */
#define fsciBleGapStopAdvertisingCmdMonitor()   \
        fsciBleGapNoParamCmdMonitor(gBleGapCmdStopAdvertisingOpCode_c)

/*! *********************************************************************************
* \brief  Gap_StopScanning command monitoring macro.
*
********************************************************************************** */
#define fsciBleGapStopScanningCmdMonitor()  \
        fsciBleGapNoParamCmdMonitor(gBleGapCmdStopScanningOpCode_c)

/*! *********************************************************************************
* \brief  Gap_ReadWhiteListSize command monitoring macro.
*
********************************************************************************** */
#define fsciBleGapReadWhiteListSizeCmdMonitor() \
        fsciBleGapNoParamCmdMonitor(gBleGapCmdReadWhiteListSizeOpCode_c)

/*! *********************************************************************************
* \brief  Gap_ClearWhiteList command monitoring macro.
*
********************************************************************************** */
#define fsciBleGapClearWhiteListCmdMonitor()    \
        fsciBleGapNoParamCmdMonitor(gBleGapCmdClearWhiteListOpCode_c)

/*! *********************************************************************************
* \brief  Gap_RemoveAllBonds command monitoring macro.
*
********************************************************************************** */
#define fsciBleGapRemoveAllBondsCmdMonitor()    \
        fsciBleGapNoParamCmdMonitor(gBleGapCmdRemoveAllBondsOpCode_c)

/*! *********************************************************************************
* \brief  Gap_EncryptLink command monitoring macro.
*
* \param[in]    deviceId    Device ID of the peer.
*
********************************************************************************** */
#define fsciBleGapEncryptLinkCmdMonitor(deviceId)                           \
        fsciBleGapDeviceIdParamCmdMonitor(gBleGapCmdEncryptLinkOpCode_c,    \
                                          (deviceId))

/*! *********************************************************************************
* \brief  Gap_RemoveBond command monitoring macro.
*
* \param[in]    nvmIndex    Index of the device in NVM bonding area.
*
********************************************************************************** */
#define fsciBleGapRemoveBondCmdMonitor(nvmIndex)                        \
        fsciBleGapUint8ParamCmdMonitor(gBleGapCmdRemoveBondOpCode_c,    \
                                       (nvmIndex))

/*! *********************************************************************************
* \brief  Gap_RejectPasskeyRequest command monitoring macro.
*
* \param[in]    deviceId    The GAP peer that requested a passkey entry.
*
********************************************************************************** */
#define fsciBleGapRejectPasskeyRequestCmdMonitor(deviceId)                          \
        fsciBleGapDeviceIdParamCmdMonitor(gBleGapCmdRejectPasskeyRequestOpCode_c,   \
                                          (deviceId))

/*! *********************************************************************************
* \brief  Gap_RejectKeyExchangeRequest command monitoring macro.
*
* \param[in]    deviceId    The GAP peer who requested the Key Exchange procedure.
*
********************************************************************************** */
#define fsciBleGapRejectKeyExchangeRequestCmdMonitor(deviceId)                          \
        fsciBleGapDeviceIdParamCmdMonitor(gBleGapCmdRejectKeyExchangeRequestOpCode_c,   \
                                          (deviceId))

/*! *********************************************************************************
* \brief  Gap_DenyLongTermKey command monitoring macro.
*
* \param[in]    deviceId    The GAP peer who requested encryption.
*
********************************************************************************** */
#define fsciBleGapDenyLongTermKeyCmdMonitor(deviceId)                           \
        fsciBleGapDeviceIdParamCmdMonitor(gBleGapCmdDenyLongTermKeyOpCode_c,    \
                                          (deviceId))

/*! *********************************************************************************
* \brief  Gap_Disconnect command monitoring macro.
*
* \param[in]    deviceId    The connected peer to disconnect from.
*
********************************************************************************** */
#define fsciBleGapDisconnectCmdMonitor(deviceId)                        \
        fsciBleGapDeviceIdParamCmdMonitor(gBleGapCmdDisconnectOpCode_c, \
                                          (deviceId))

/*! *********************************************************************************
* \brief  Gap_CheckNotificationStatus command monitoring macro.
*
* \param[in]    deviceId        The peer GATT Client.
* \param[in]    handle          The handle of the CCCD.
* \param[in]    pOutIsActive    The address to store the status into.
*
********************************************************************************** */
#define fsciBleGapCheckNotificationStatusCmdMonitor(deviceId,                                           \
                                                    handle,                                             \
                                                    pOutIsActive)                                       \
        fsciBleGapCheckNotificationsAndIndicationsCmdMonitor(gBleGapCmdCheckNotificationStatusOpCode_c, \
                                                             (deviceId),                                  \
                                                             (handle),                                    \
                                                             (pOutIsActive))

/*! *********************************************************************************
* \brief  Gap_CheckIndicationStatus command monitoring macro.
*
* \param[in]    deviceId        The peer GATT Client.
* \param[in]    handle          The handle of the CCCD.
* \param[in]    pOutIsActive    The address to store the status into.
*
********************************************************************************** */
#define fsciBleGapCheckIndicationStatusCmdMonitor(deviceId,                                             \
                                                  handle,                                               \
                                                  pOutIsActive)                                         \
        fsciBleGapCheckNotificationsAndIndicationsCmdMonitor(gBleGapCmdCheckIndicationStatusOpCode_c,   \
                                                             (deviceId),                                  \
                                                             (handle),                                    \
                                                             (pOutIsActive))

/*! *********************************************************************************
* \brief  Gap_AddDeviceToWhiteList command monitoring macro.
*
* \param[in]    address         The address of the white-listed device.
* \param[in]    addressType     The device address type (public or random).
*
********************************************************************************** */
#define fsciBleGapAddDeviceToWhiteListCmdMonitor(addressType,                       \
                                                 address)                           \
        fsciBleGapAddressParamsCmdMonitor(gBleGapCmdAddDeviceToWhiteListOpCode_c,   \
                                          (addressType),                              \
                                          (address))

/*! *********************************************************************************
* \brief  Gap_RemoveDeviceFromWhiteList command monitoring macro.
*
* \param[in]    address         The address of the white-listed device.
* \param[in]    addressType     The device address type (public or random).
*
********************************************************************************** */
#define fsciBleGapRemoveDeviceFromWhiteListCmdMonitor(addressType,                      \
                                                      address)                          \
        fsciBleGapAddressParamsCmdMonitor(gBleGapCmdRemoveDeviceFromWhiteListOpCode_c,  \
                                          (addressType),                                  \
                                          (address))

/*! *********************************************************************************
* \brief  Gap_ReadPublicDeviceAddress command monitoring macro.
*
********************************************************************************** */
#define fsciBleGapReadPublicDeviceAddressCmdMonitor()   \
        fsciBleGapNoParamCmdMonitor(gBleGapCmdReadPublicDeviceAddressOpCode_c)

/*! *********************************************************************************
* \brief  Gap_LeScRegeneratePublicKey command monitoring macro.
*
********************************************************************************** */
#define fsciBleGapLeScRegeneratePublicKeyCmdMonitor()   \
        fsciBleGapNoParamCmdMonitor(gBleGapCmdLeScRegeneratePublicKeyOpCode_c)

/*! *********************************************************************************
* \brief  Gap_LeScGetLocalOobData command monitoring macro.
*
********************************************************************************** */
#define fsciBleGapLeScGetLocalOobDataCmdMonitor()   \
        fsciBleGapNoParamCmdMonitor(gBleGapCmdLeScGetLocalOobDataOpCode_c)

/*! *********************************************************************************
* \brief  Gap_ReadChannelMap command monitoring macro.
*
* \param[in]    deviceId    The connected peer to read the channel map.
*
********************************************************************************** */
#define fsciBleGapReadChannelMapCmdMonitor(deviceId)                \
        fsciBleGapDeviceIdParamCmdMonitor(gBleGapCmdReadChannelMapOpCode_c,   \
                                          (deviceId))

/*! *********************************************************************************
* \brief  Gap_RemoveAdvSet command monitoring macro.
*
* \param[in]    handle                 Set ID to remove or 0xFF to remove all
*
********************************************************************************** */
#define fsciBleGapRemoveAdvSetCmdMonitor(handle)                                    \
        fsciBleGapUint8ParamCmdMonitor(gBleGapCmdRemoveAdvSetOpCode_c,              \
                                       (handle))

/*! *********************************************************************************
* \brief  Gap_CheckNotificationStatus out parameter monitoring macro.
*
* \param[in]    pOutIsActive    Pointer to the notification status.
*
********************************************************************************** */
#define fsciBleGapCheckNotificationStatusEvtMonitor(pOutIsActive)                   \
        fsciBleGapBoolParamEvtMonitor(gBleGapEvtCheckNotificationStatusOpCode_c,    \
                                      *(pOutIsActive))

/*! *********************************************************************************
* \brief  Gap_CheckIndicationStatus out parameter monitoring macro.
*
* \param[in]    pOutIsActive    Pointer to the indication status.
*
********************************************************************************** */
#define fsciBleGapCheckIndicationStatusEvtMonitor(pOutIsActive)                 \
        fsciBleGapBoolParamEvtMonitor(gBleGapEvtCheckIndicationStatusOpCode_c,  \
                                      *(pOutIsActive))

/*! *********************************************************************************
* \brief  Gap_CheckIfBonded out parameter monitoring macro.
*
* \param[in]    pOutIsActive    Pointer to the bonded flag.
*
********************************************************************************** */
#define fsciBleGapCheckIfBondedEvtMonitor(pOutIsBonded)                 \
        fsciBleGapBoolParamEvtMonitor(gBleGapEvtCheckIfBondedOpCode_c,  \
                                      *(pOutIsBonded))

/*! *********************************************************************************
* \brief  Gap_LoadEncryptionInformation out parameters monitoring macro.
*
* \param[in]    aOutLtk         Array filled with the LTK.
* \param[in]    pOutLtkSize     The LTK size.
*
********************************************************************************** */
#define fsciBleGapLoadEncryptionInformationEvtMonitor(aOutLtk,                              \
                                                      pOutLtkSize)                          \
        fsciBleGapArrayAndSizeParamEvtMonitor(gBleGapEvtLoadEncryptionInformationOpCode_c,  \
                                              (aOutLtk),                                    \
                                              *(pOutLtkSize))

/*! *********************************************************************************
* \brief  Gap_GetBondedDeviceName out parameter monitoring macro.
*
* \param[in]    aOutName    Destination array that keeps the device name.
*
********************************************************************************** */
#define fsciBleGapGetBondedDeviceNameEvtMonitor(aOutName)                               \
        fsciBleGapArrayAndSizeParamEvtMonitor(gBleGapEvtGetBondedDeviceNameOpCode_c,    \
                                              (uint8_t*)(aOutName),                     \
                                              (uint8_t)strlen((char const*)(aOutName)))

/*! *********************************************************************************
* \brief  Gap_GetBondedDevicesCount out parameter monitoring macro.
*
* \param[in]    pOutBondedDevicesCount  Pointer to the number of bonded devices.
*
********************************************************************************** */
#define fsciBleGapGetBondedDevicesCountEvtMonitor(pOutBondedDevicesCount)               \
        fsciBleGapArrayAndSizeParamEvtMonitor(gBleGapEvtGetBondedDevicesCountOpCode_c,  \
                                              NULL,                                     \
                                              *(pOutBondedDevicesCount))

#if defined(gBLE50_d) && (gBLE50_d == 1U)
/*! *********************************************************************************
* \brief  Gap_StopExtAdvertising command monitoring function.
*
* \param[in]    handle                   The ID of the advertising set
*
********************************************************************************** */
#define fsciBleGapStopExtAdvertisingCmdMonitor(handle)                              \
        fsciBleGapUint8ParamCmdMonitor(gBleGapCmdStopExtAdvertisingOpCode_c,        \
                                       (handle))

/*! *********************************************************************************
* \brief  Gap_StartPeriodicAdvertising command monitoring function.
*
* \param[in]    handle                   The ID of the advertising set
*
********************************************************************************** */
#define fsciBleGapStartPeriodicAdvertisingCmdMonitor(handle)                        \
        fsciBleGapUint8ParamCmdMonitor(gBleGapCmdStartPeriodicAdvOpCode_c,          \
                                       (handle))

/*! *********************************************************************************
* \brief  Gap_StopPeriodicAdvertising command monitoring function.
*
* \param[in]    handle                   The ID of the advertising set
*
********************************************************************************** */
#define fsciBleGapStopPeriodicAdvertisingCmdMonitor(handle)                         \
        fsciBleGapUint8ParamCmdMonitor(gBleGapCmdStopPeriodicAdvOpCode_c,           \
                                       (handle))

/*! *********************************************************************************
* \brief  Gap_PeriodicAdvTerminateSync command monitoring function.
*
* \param[in]    SID                   The ID of the advertising set
*
********************************************************************************** */
#define fsciBleGapPeriodicAdvTerminateSyncCmdMonitor(SID)                            \
        fsciBleGapUint8ParamCmdMonitor(gBleGapCmdPeriodicAdvTerminateSyncOpCode_c,   \
                                       (SID))
#endif
/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/*! FSCI operation codes for GAP */
typedef enum
{
    gBleGapModeSelectOpCode_c                   = 0x00,                     /*! GAP Mode Select operation code */

    gBleGapCmdFirstOpCode_c                     = 0x01,
    gBleGapCmdBleHostInitializeOpCode_c         = gBleGapCmdFirstOpCode_c,  /*! Ble_HostInitialize command operation code */
    gBleGapCmdRegisterDeviceSecurityRequirementsOpCode_c,                   /*! Gap_RegisterDeviceSecurityRequirements command operation code */
    gBleGapCmdSetAdvertisingParametersOpCode_c,                             /*! Gap_SetAdvertisingParameters command operation code */
    gBleGapCmdSetAdvertisingDataOpCode_c,                                   /*! Gap_SetAdvertisingData command operation code */
    gBleGapCmdStartAdvertisingOpCode_c,                                     /*! Gap_StartAdvertising command operation code */
    gBleGapCmdStopAdvertisingOpCode_c,                                      /*! Gap_StopAdvertising command operation code */
    gBleGapCmdAuthorizeOpCode_c,                                            /*! Gap_Authorize command operation code */
    gBleGapCmdSaveCccdOpCode_c,                                             /*! Gap_SaveCccd command operation code */
    gBleGapCmdCheckNotificationStatusOpCode_c,                              /*! Gap_CheckNotificationStatus command operation code */
    gBleGapCmdCheckIndicationStatusOpCode_c,                                /*! Gap_CheckIndicationStatus command operation code */
    gFsciBleGapReserved1_c,                                                 /*! Removed: Gap_GetBondedStaticAddresses command operation code */
    gBleGapCmdPairOpCode_c,                                                 /*! Gap_Pair command operation code */
    gBleGapCmdSendSlaveSecurityRequestOpCode_c,                             /*! Gap_SendSlaveSecurityRequest command operation code */
    gBleGapCmdEncryptLinkOpCode_c,                                          /*! Gap_EncryptLink command operation code */
    gBleGapCmdAcceptPairingRequestOpCode_c,                                 /*! Gap_AcceptPairingRequest command operation code */
    gBleGapCmdRejectPairingOpCode_c,                                        /*! Gap_RejectAuthentication command operation code */
    gBleGapCmdEnterPasskeyOpCode_c,                                         /*! Gap_EnterPasskey command operation code */
    gBleGapCmdProvideOobOpCode_c,                                           /*! Gap_ProvideOob command operation code */
    gBleGapCmdRejectPasskeyRequestOpCode_c,                                 /*! Gap_RejectPasskeyRequest command operation code */
    gBleGapCmdSendSmpKeysOpCode_c,                                          /*! Gap_SendSmpKeys command operation code */
    gBleGapCmdRejectKeyExchangeRequestOpCode_c,                             /*! Gap_RejectKeyExchangeRequest command operation code */
    gBleGapCmdProvideLongTermKeyOpCode_c,                                   /*! Gap_ProvideLongTermKey command operation code */
    gBleGapCmdDenyLongTermKeyOpCode_c,                                      /*! Gap_DenyLongTermKey command operation code */
    gBleGapCmdLoadEncryptionInformationOpCode_c,                            /*! Gap_LoadEncryptionInformation command operation code */
    gBleGapCmdSetLocalPasskeyOpCode_c,                                      /*! Gap_SetLocalPasskey command operation code */
    gBleGapCmdStartScanningOpCode_c,                                        /*! Gap_StartScanning command operation code */
    gBleGapCmdStopScanningOpCode_c,                                         /*! Gap_StopScanning command operation code */
    gBleGapCmdConnectOpCode_c,                                              /*! Gap_Connect command operation code */
    gBleGapCmdDisconnectOpCode_c,                                           /*! Gap_Disconnect command operation code */
    gBleGapCmdSaveCustomPeerInformationOpCode_c,                            /*! Gap_SaveCustomPeerInformation command operation code */
    gBleGapCmdLoadCustomPeerInformationOpCode_c,                            /*! Gap_LoadCustomPeerInformation command operation code */
    gBleGapCmdCheckIfBondedOpCode_c,                                        /*! Gap_CheckIfBonded command operation code */
    gBleGapCmdReadWhiteListSizeOpCode_c,                                    /*! Gap_ReadWhiteListSize command operation code */
    gBleGapCmdClearWhiteListOpCode_c,                                       /*! Gap_ClearWhiteList command operation code */
    gBleGapCmdAddDeviceToWhiteListOpCode_c,                                 /*! Gap_AddDeviceToWhiteList command operation code */
    gBleGapCmdRemoveDeviceFromWhiteListOpCode_c,                            /*! Gap_RemoveDeviceFromWhiteList command operation code */
    gBleGapCmdReadPublicDeviceAddressOpCode_c,                              /*! Gap_ReadPublicDeviceAddress command operation code */
    gBleGapCmdCreateRandomDeviceAddressOpCode_c,                            /*! Gap_CreateRandomDeviceAddress command operation code */
    gBleGapCmdSaveDeviceNameOpCode_c,                                       /*! Gap_SaveDeviceName command operation code */
    gBleGapCmdGetBondedDevicesCountOpCode_c,                                /*! Gap_GetBondedDevicesCount command operation code */
    gBleGapCmdGetBondedDeviceNameOpCode_c,                                  /*! Gap_GetBondedDeviceName command operation code */
    gBleGapCmdRemoveBondOpCode_c,                                           /*! Gap_RemoveBond command operation code */
    gBleGapCmdRemoveAllBondsOpCode_c,                                       /*! Gap_RemoveAllBonds command operation code */
    gBleGapCmdReadRadioPowerLevelOpCode_c,                                  /*! Gap_ReadRadioPowerLevel command operation code */
    gBleGapCmdVerifyPrivateResolvableAddressOpCode_c,                       /*! Gap_VerifyPrivateResolvableAddress command operation code */
    gBleGapCmdSetRandomAddressOpCode_c,                                     /*! Gap_SetRandomAddress command operation code */
    gBleGapCmdSetScanModeOpCode_c,                                          /*! Gap_SetScanMode command operation code */
    gBleGapCmdSetDefaultPairingParameters_c,                                /*! Gap_SetDefaultPairingParameters command operation code */
    gBleGapCmdUpdateConnectionParametersOpCode_c,                           /*! Gap_UpdateConnectionParameters command operation code */
    gBleGapCmdEnableUpdateConnectionParametersOpCode_c,                     /*! Gap_EnableUpdateConnectionParameters command operation code */
    gBleGapCmdUpdateLeDataLengthOpCode_c,                                   /*! Gap_UpdateLeDataLength command operation code */
    gReserved1_c,
    gBleGapCmdEnableHostPrivacyOpCode_c,                                    /*! Gap_EnableHostPrivacy command operation code */
    gBleGapCmdEnableControllerPrivacyOpCode_c,                              /*! Gap_EnableControllerPrivacy command operation code */
    gBleGapCmdLeScRegeneratePublicKeyOpCode_c,                              /*! Gap_LeScRegeneratePublicKey command operation code */
    gBleGapCmdLeScValidateNumericValueOpCode_c,                             /*! Gap_LeScValidateNumericValue command operation code */
    gBleGapCmdLeScGetLocalOobDataOpCode_c,                                  /*! Gap_LeScGetLocalOobData command operation code */
    gBleGapCmdLeScSetPeerOobDataOpCode_c,                                   /*! Gap_LeScSetPeerOobData command operation code */
    gBleGapCmdLeScSendKeypressNotificationPrivacyOpCode_c,                  /*! Gap_LeScSendKeypressNotification command operation code */
    gBleGapCmdGetBondedDevicesIdentityInformationOpCode_c,                  /*! Gap_GetBondedDevicesIdentityInformation command operation code */
    gBleGapCmdSetTxPowerLevelOpCode_c,                                      /*! Gap_SetTxPowerLevel command operation code */
    gBleGapCmdLeReadPhyOpCode_c,                                            /*! Gap_LeReadPhy command operation code */
    gBleGapCmdLeSetPhyOpCode_c,                                             /*! Gap_LeSetPhy command operation code */
    gBleGapCmdControllerNotificationOpCode_c,                               /*! Gap_ControllerEnhancedNotification command operation code */
    gBleGapCmdLoadKeysOpCode_c,                                             /*! Gap_LoadKeys command operation code */
    gBleGapCmdSaveKeysOpCode_c,                                             /*! Gap_SaveKeys command operation code */
    gBleGapCmdSetChannelMapOpCode_c,                                        /*! Gap_SetChannelMap command operation code */
    gBleGapCmdReadChannelMapOpCode_c,                                       /*! Gap_ReadChannelMap command operation code */

    gBleGapCmdSetPrivacyModeOpCode_c,                                       /*! Gap_SetPrivacyMode command operation code */

    /* BLE 5.0 */
    gBleGapCmd50FirstOpCode_c                   = 0x50,
    gBleGapCmdSetExtAdvertisingParametersOpCode_c = gBleGapCmd50FirstOpCode_c, /*! Gap_SetExtAdvertisingParameters command operation code */
    gBleGapCmdStartExtAdvertisingOpCode_c,                                  /*! Gap_StartExtAdvertising command operation code */
    gBleGapCmdRemoveAdvSetOpCode_c,                                         /*! Gap_RemoveAdvSet command operation code */
    gBleGapCmdStopExtAdvertisingOpCode_c,                                   /*! Gap_StopExtAdvertising command operation code */
    gBleGapCmdUpdatePeriodicAdvListOpCode_c,                                /*! Gap_UpdatePeriodicAdvList command operation code */
    gBleGapCmdSetPeriodicAdvParametersOpCode_c,                             /*! Gap_SetPeriodicAdvParameters command operation code */
    gBleGapCmdStartPeriodicAdvOpCode_c,                                     /*! Gap_StartPeriodicAdvertising command operation code */
    gBleGapCmdStopPeriodicAdvOpCode_c,                                      /*! Gap_StopPeriodicAdvertising command operation code */
    gBleGapCmdSetExtAdvertisingDataOpCode_c,                                /*! Gap_SetExtAdvertisingData command operation code */
    gBleGapCmdSetPeriodicAdvDataOpCode_c,                                   /*! Gap_SetPeriodicAdvData command operation code */
    gBleGapCmdPeriodicAdvCreateSyncOpCode_c,                                /*! Gap_PeriodicAdvCreateSync command operation code */
    gBleGapCmdPeriodicAdvTerminateSyncOpCode_c,                             /*! Gap_PeriodicAdvTerminateSync command operation code */

    gBleGapStatusOpCode_c                       = 0x80,                     /*! GAP status operation code */

    gBleGapEvtFirstOpCode_c                     = 0x81,
    gBleGapEvtCheckNotificationStatusOpCode_c   = gBleGapEvtFirstOpCode_c,  /*! Gap_CheckNotificationStatus command out parameters event operation code */
    gBleGapEvtCheckIndicationStatusOpCode_c,                                /*! Gap_CheckIndicationStatus command out parameters event operation code */
    gBleGapEvtLoadKeysOpCode_c,                                             /*! Gap_LoadKeys command out parameters event operation code */
    gBleGapEvtLoadEncryptionInformationOpCode_c,                            /*! Gap_LoadEncryptionInformation command out parameters event operation code */
    gBleGapEvtLoadCustomPeerInformationOpCode_c,                            /*! Gap_LoadCustomPeerInformation command out parameters event operation code */
    gBleGapEvtCheckIfBondedOpCode_c,                                        /*! Gap_CheckIfBonded command out parameters event operation code */
    gBleGapEvtGetBondedDevicesCountOpCode_c,                                /*! Gap_GetBondedDevicesCount command out parameters event operation code */
    gBleGapEvtGetBondedDeviceNameOpCode_c,                                  /*! Gap_GetBondedDeviceName command out parameters event operation code */

    gBleGapEvtGenericEventInitializationCompleteOpCode_c,                   /*! gapGenericCallback (type = gInitializationComplete_c) event operation code */
    gBleGapEvtGenericEventInternalErrorOpCode_c,                            /*! gapGenericCallback (type = gInternalError_c) event operation code */
    gBleGapEvtGenericEventAdvertisingSetupFailedOpCode_c,                   /*! gapGenericCallback (type = gAdvertisingSetupFailed_c) event operation code */
    gBleGapEvtGenericEventAdvertisingParametersSetupCompleteOpCode_c,       /*! gapGenericCallback (type = gAdvertisingParametersSetupComplete_c) event operation code */
    gBleGapEvtGenericEventAdvertisingDataSetupCompleteOpCode_c,             /*! gapGenericCallback (type = gAdvertisingDataSetupComplete_c) event operation code */
    gBleGapEvtGenericEventWhiteListSizeReadOpCode_c,                        /*! gapGenericCallback (type = gWhiteListSizeRead_c) event operation code */
    gBleGapEvtGenericEventDeviceAddedToWhiteListOpCode_c,                   /*! gapGenericCallback (type = gDeviceAddedToWhiteList_c) event operation code */
    gBleGapEvtGenericEventDeviceRemovedFromWhiteListOpCode_c,               /*! gapGenericCallback (type = gDeviceRemovedFromWhiteList_c) event operation code */
    gBleGapEvtGenericEventWhiteListClearedOpCode_c,                         /*! gapGenericCallback (type = gWhiteListCleared_c) event operation code */
    gBleGapEvtGenericEventRandomAddressReadyOpCode_c,                       /*! gapGenericCallback (type = gRandomAddressReady_c) event operation code */
    gBleGapEvtGenericEventCreateConnectionCanceledOpCode_c,                 /*! gapGenericCallback (type = gCreateConnectionCanceled_c) event operation code */
    gBleGapEvtGenericEventPublicAddressReadOpCode_c,                        /*! gapGenericCallback (type = gPublicAddressRead_c) event operation code */
    gBleGapEvtGenericEventAdvTxPowerLevelReadOpCode_c,                      /*! gapGenericCallback (type = gAdvTxPowerLevelRead_c) event operation code */
    gBleGapEvtGenericEventPrivateResolvableAddressVerifiedOpCode_c,         /*! gapGenericCallback (type = gPrivateResolvableAddressVerified_c) event operation code */
    gBleGapEvtGenericEventRandomAddressSetOpCode_c,                         /*! gapGenericCallback (type = gRandomAddressSet_c) event operation code */

    gBleGapEvtAdvertisingEventAdvertisingStateChangedOpCode_c,              /*! gapAdvertisingCallback (type = gAdvertisingStateChanged_c) event operation code */
    gBleGapEvtAdvertisingEventAdvertisingCommandFailedOpCode_c,             /*! gapAdvertisingCallback (type = gAdvertisingCommandFailed_c) event operation code */

    gBleGapEvtScanningEventScanStateChangedOpCode_c,                        /*! gapScanningCallback (type = gScanStateChanged_c) event operation code */
    gBleGapEvtScanningEventScanCommandFailedOpCode_c,                       /*! gapScanningCallback (type = gScanCommandFailed_c) event operation code */
    gBleGapEvtScanningEventDeviceScannedOpCode_c,                           /*! gapScanningCallback (type = gDeviceScanned_c) event operation code */

    gBleGapEvtConnectionEventConnectedOpCode_c,                             /*! gapConnectionCallback (type = gConnected_c) event operation code */
    gBleGapEvtConnectionEventPairingRequestOpCode_c,                        /*! gapConnectionCallback (type = gPairingRequest_c) event operation code */
    gBleGapEvtConnectionEventSlaveSecurityRequestOpCode_c,                  /*! gapConnectionCallback (type = gSlaveSecurityRequest_c) event operation code */
    gBleGapEvtConnectionEventPairingResponseOpCode_c,                       /*! gapConnectionCallback (type = gPairingResponse_c) event operation code */
    gBleGapEvtConnectionEventAuthenticationRejectedOpCode_c,                /*! gapConnectionCallback (type = gAuthenticationRejected_c) event operation code */
    gBleGapEvtConnectionEventPasskeyRequestOpCode_c,                        /*! gapConnectionCallback (type = gPasskeyRequest_c) event operation code */
    gBleGapEvtConnectionEventOobRequestOpCode_c,                            /*! gapConnectionCallback (type = gOobRequest_c) event operation code */
    gBleGapEvtConnectionEventPasskeyDisplayOpCode_c,                        /*! gapConnectionCallback (type = gPasskeyDisplay_c) event operation code */
    gBleGapEvtConnectionEventKeyExchangeRequestOpCode_c,                    /*! gapConnectionCallback (type = gKeyExchangeRequest_c) event operation code */
    gBleGapEvtConnectionEventKeysReceivedOpCode_c,                          /*! gapConnectionCallback (type = gKeysReceived_c) event operation code */
    gBleGapEvtConnectionEventLongTermKeyRequestOpCode_c,                    /*! gapConnectionCallback (type = gLongTermKeyRequest_c) event operation code */
    gBleGapEvtConnectionEventEncryptionChangedOpCode_c,                     /*! gapConnectionCallback (type = gEncryptionChanged_c) event operation code */
    gBleGapEvtConnectionEventPairingCompleteOpCode_c,                       /*! gapConnectionCallback (type = gPairingComplete_c) event operation code */
    gBleGapEvtConnectionEventDisconnectedOpCode_c,                          /*! gapConnectionCallback (type = gDisconnected_c) event operation code */
    gBleGapEvtConnectionEventRssiReadOpCode_c,                              /*! gapConnectionCallback (type = gRssiRead_c) event operation code */
    gBleGapEvtConnectionEventTxPowerLevelReadOpCode_c,                      /*! gapConnectionCallback (type = gTxPowerLevelRead_c) event operation code */
    gBleGapEvtConnectionEventPowerReadFailureOpCode_c,                      /*! gapConnectionCallback (type = gPowerReadFailureOpCode_c) event operation code */
    gBleGapEvtConnectionEventParameterUpdateRequestOpCode_c,                /*! gapConnectionCallback (type = gConnEvtParameterUpdateRequest_c) event operation code */
    gBleGapEvtConnectionEventParameterUpdateCompleteOpCode_c,               /*! gapConnectionCallback (type = gConnEvtParameterUpdateComplete_c) event operation code */
    gBleGapEvtConnectionEventLeDataLengthChangedOpCode_c,                   /*! gapConnectionCallback (type = gConnEvtLeDataLengthChanged_c) event operation code */
    gBleGapEvtConnectionEventLeScOobDataRequestOpCode_c,                    /*! gapConnectionCallback (type = gConnEvtLeScOobDataRequest_c) event operation code */
    gBleGapEvtConnectionEventLeScDisplayNumericValueOpCode_c,               /*! gapConnectionCallback (type = gConnEvtLeScDisplayNumericValue_c) event operation code */
    gBleGapEvtConnectionEventLeScKeypressNotificationOpCode_c,              /*! gapConnectionCallback (type = gConnEvtLeScKeypressNotification_c) event operation code */

    gReserved2_c,
    gBleGapEvtGenericEventLeScPublicKeyRegeneratedOpCode_c,                 /*! gapGenericCallback (type = gLeScPublicKeyRegenerated_c) event operation code */
    gBleGapEvtGenericEventLeScLocalOobDataOpCode_c,                         /*! gapGenericCallback (type = gLeScLocalOobData_c) event operation code */
    gBleGapEvtGenericEventHostPrivacyStateChangedOpCode_c,                  /*! gapGenericCallback (type = gHostPrivacyStateChanged_c) event operation code */
    gBleGapEvtGenericEventControllerPrivacyStateChangedOpCode_c,            /*! gapGenericCallback (type = gControllerPrivacyStateChanged_c) event operation code */
    gBleGapEvtGenericEventTxPowerLevelSetCompleteOpCode_c,                  /*! gapGenericCallback (type = gTxPowerLevelSetComplete_c) event operation code */
    gBleGapEvtGenericEventLePhyEventOpCode_c,                               /*! gapGenericCallback (type = gLePhyEvent_c) event operation code */

    gBleGapEvtGetBondedDevicesIdentityInformationOpCode_c,                  /*! Gap_GetBondedDevicesIdentityInformation command out parameters event operation code */
    gBleGapEvtGenericEventControllerNotificationOpCode_c,                   /*! gapGenericCallback (type = gControllerNotificationEvent_c) event operation code */
    gBleGapEvtGenericEventBondCreatedOpCode_c,                              /*! gapGenericCallback (type = gBondCreatedEvent_c) event operation code */
    gBleGapEvtGenericEventChannelMapSetOpCode_c,                            /*! gapGenericCallback (type = gChannelMapSet_c) event operation code */

    gBleGapEvtConnectionEventChannelMapReadOpCode_c,                        /*! gapConnectionCallback (type = gConnEvtChannelMapRead_c) event operation code */
    gBleGapEvtConnectionEventChannelMapReadFailureOpCode_c,                 /*! gapConnectionCallback (type = gConnEvtChannelMapReadFailure_c) event operation code */

    /* BLE 5.0: Advertising extensions */
    gBleGapEvtGenericEventExtAdvertisingParametersSetupCompleteOpCode_c,    /*! gapGenericCallback (type = gExtAdvertisingParametersSetupComplete_c) event operation code */
    gBleGapEvtGenericEventExtAdvertisingDataSetupCompleteOpCode_c,          /*! gapGenericCallback (type = gExtAdvertisingDataSetupComplete_c) event operation code */
    gBleGapEvtGenericEventPeriodicAdvParamSetupCompleteOpCode_c,            /*! gapGenericCallback (type = gPeriodicAdvParamSetupComplete_c) event operation code */
    gBleGapEvtGenericEventPeriodicAdvDataSetupCompleteOpCode_c,             /*! gapGenericCallback (type = gPeriodicAdvDataSetupComplete_c) event operation code */
    gBleGapEvtGenericEventPeriodicAdvListUpdateCompleteOpCode_c,            /*! gapGenericCallback (type = gPeriodicAdvListUpdateComplete_c) event operation code */

    gBleGapEvtAdvertisingEventExtAdvertisingStateChangedOpCode_c,           /*! gapAdvertisingCallback (type = gExtAdvertisingStateChanged_c) event operation code */
    gBleGapEvtAdvertisingEventAdvertisingSetTerminatedOpCode_c,             /*! gapAdvertisingCallback (type = gAdvertisingSetTerminated_c) event operation code */
    gBleGapEvtAdvertisingEventExtAdvertisingSetRemovedOpCode_c,             /*! gapAdvertisingCallback (type = gExtAdvertisingSetRemoveComplete_c) event operation code */
    gBleGapEvtAdvertisingEventExtScanReqReceivedOpCode_c,                   /*! gapAdvertisingCallback (type = gExtScanNotification_c) event operation code */
    gBleGapEvtAdvertisingEventPeriodicAdvertisingStateChangedOpCode_c,      /*! gapAdvertisingCallback (type = gPeriodicAdvertisingStateChanged_c) event operation code */

    gBleGapEvtScanningEventExtDeviceScannedOpCode_c,                        /*! gapScanningCallback (type = gExtDeviceScanned_c) event operation code */
    gBleGapEvtScanningEventPeriodicAdvSyncEstablishedOpCode_c,              /*! gapScanningCallback (type = gPeriodicAdvSyncEstablished_c) event operation code */
    gBleGapEvtScanningEventPeriodicAdvSyncTerminatedOpCode_c,               /*! gapScanningCallback (type = gPeriodicAdvSyncTerminated_c) event operation code */
    gBleGapEvtScanningEventPeriodicAdvSyncLostOpCode_c,                     /*! gapScanningCallback (type = gPeriodicAdvSyncLost_c) event operation code */
    gBleGapEvtScanningEventPeriodicDeviceScannedOpCode_c,                   /*! gapScanningCallback (type = gPeriodicDeviceScanned_c) event operation code */
    gBleGapEvtGenericEventPeriodicAdvCreateSyncCancelledOpCode_c,           /*! gapGenericCallback (type = gPeriodicAdvCreateSyncCancelled_c) event operation code */
    gBleGapEvtConnectionEventChannelSelectionAlgorithm2OpCode_c,            /*! gapConnectionCallback (type = gConnEvtChanSelectionAlgorithm2_c) event operation code */
    gBleGapEvtGenericEventTxEntryAvailableOpCode_c,                         /*! gapGenericCallback (type = gTxEntryAvailable_c) event operation code */
}fsciBleGapOpCode_t;

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
* \brief  Calls the GAP function associated with the operation code received over UART.
*         The GAP function parameters are extracted from the received FSCI payload.
*
* \param[in]    pData               Packet (containing FSCI header and FSCI
*                                   payload) received over UART
* \param[in]    param               Pointer given when this function is registered in
*                                   FSCI
* \param[in]    fsciInterfaceId     FSCI interface on which the packet was received
*
********************************************************************************** */
void fsciBleGapHandler
(
    void* pData,
    void* param,
    uint32_t fsciInterfaceId
);

#if gFsciBleBBox_d || gFsciBleTest_d
/*! *********************************************************************************
* \brief  Creates the FSCI packet which contains the status of the last executed GAP
*         command and sends it over UART.
*
* \param[in]    result      Status of the last executed GAP command.
*
********************************************************************************** */
void fsciBleGapStatusMonitor
(
    bleResult_t result
);
#endif /* gFsciBleBBox_d || gFsciBleTest_d */

#if gFsciBleHost_d || gFsciBleTest_d
/*! *********************************************************************************
* \brief  Creates a GAP FSCI packet without payload and sends it over UART.
*
* \param[in]    opCode      GAP command operation code.
*
********************************************************************************** */
void fsciBleGapNoParamCmdMonitor
(
    fsciBleGapOpCode_t opCode
);

/*! *********************************************************************************
* \brief  Creates a GAP FSCI packet with one byte of payload and sends it over UART.
*
* \param[in]    opCode      GAP command operation code.
* \param[in]    param       One byte parameter.
*
********************************************************************************** */
void fsciBleGapUint8ParamCmdMonitor
(
    fsciBleGapOpCode_t  opCode,
    uint8_t             param
);

/*! *********************************************************************************
* \brief  Creates a GAP FSCI packet with deviceId as payload and sends it over UART.
*
* \param[in]    opCode      GAP command operation code.
* \param[in]    deviceId    Device identifier of the peer.
*
********************************************************************************** */
void fsciBleGapDeviceIdParamCmdMonitor
(
    fsciBleGapOpCode_t  opCode,
    deviceId_t          deviceId
);

/*! *********************************************************************************
* \brief  Gap_CheckNotificationStatus and Gap_CheckIndicationsStatus commands
*         monitoring function.
*
* \param[in]    opCode          GAP command operation code.
* \param[in]    deviceId        The peer GATT Client.
* \param[in]    handle          The handle of the CCCD.
*
********************************************************************************** */
void fsciBleGapCheckNotificationsAndIndicationsCmdMonitor
(
    fsciBleGapOpCode_t  opCode,
    deviceId_t          deviceId,
    uint16_t            handle,
    bool_t*             pOutIsActive
);

/*! *********************************************************************************
* \brief  Gap_AddDeviceToWhiteList and Gap_RemoveDeviceFromWhiteList commands
*         monitoring function.
*
* \param[in]    opCode          GAP command operation code.
* \param[in]    address         The address of the white-listed device.
* \param[in]    addressType     The device address type (public or random).
*
********************************************************************************** */
void fsciBleGapAddressParamsCmdMonitor
(
    fsciBleGapOpCode_t  opCode,
    bleAddressType_t    addressType,
    const bleDeviceAddress_t  address
);

/*! *********************************************************************************
* \brief  Gap_SaveCustomPeerInformation and Gap_LoadCustomPeerInformation commands
*         monitoring function.
*
* \param[in]    opCode      GAP command operation code.
* \param[in]    deviceId    Device ID of the GAP peer.
* \param[in]    aInfo       Pointer to the beginning of the data.
* \param[in]    offset      Offset from the beginning of the reserved memory area.
* \param[in]    infoSize    Data size (maximum equal to gcReservedFlashSizeForCustomInformation_d).
*
********************************************************************************** */
void fsciBleGapSaveOrLoadCustomPeerInformationCmdMonitor
(
    fsciBleGapOpCode_t  opCode,
    deviceId_t          deviceId,
    const uint8_t*            aInfo,
    uint16_t            offset,
    uint16_t            infoSize
);

/*! *********************************************************************************
* \brief  Ble_HostInitialize command monitoring function.
*
* \param[in]    genericCallback             Callback used to propagate GAP controller
                                            events to the application.
* \param[in]    hostToControllerInterface   LE Controller uplink interface function pointer
*
********************************************************************************** */
void fsciBleGapHostInitializeCmdMonitor
(
    gapGenericCallback_t            genericCallback,
    hciHostToControllerInterface_t  hostToControllerInterface
);

/*! *********************************************************************************
* \brief  Gap_RegisterDeviceSecurityRequirements command monitoring function.
*
* \param[in]    pSecurity       Pointer to the application-allocated
*                               gapDeviceSecurityRequirements_t structure.
*
********************************************************************************** */
void fsciBleGapRegisterDeviceSecurityRequirementsCmdMonitor
(
    const gapDeviceSecurityRequirements_t* pSecurity
);

/*! *********************************************************************************
* \brief  Gap_SetAdvertisingParameters command monitoring function.
*
* \param[in]    pAdvertisingParameters      Pointer to gapAdvertisingParameters_t
*                                           structure.
*
********************************************************************************** */
void fsciBleGapSetAdvertisingParametersCmdMonitor
(
    const gapAdvertisingParameters_t* pAdvertisingParameters
);

/*! *********************************************************************************
* \brief  Gap_SetExtAdvertisingParameters command monitoring function.
*
* \param[in]    pAdvertisingParameters      Pointer to gapExtAdvertisingParameters_t
*                                           structure.
*
********************************************************************************** */
void fsciBleGapSetExtAdvertisingParametersCmdMonitor
(
    gapExtAdvertisingParameters_t* pAdvertisingParameters
);

/*! *********************************************************************************
* \brief  Gap_SetExtAdvertisingData command monitoring function.
*
* \param[in]    handle                      The advertising set identifier
* \param[in]    pStaticAdvertisingData      Pointer to gapAdvertisingData_t structure
*                                           or NULL.
* \param[in]    pStaticScanResponseData     Pointer to gapScanResponseData_t structure
*                                           or NULL.
*
********************************************************************************** */
void fsciBleGapSetExtAdvertisingDataCmdMonitor
(
    uint8_t handle,
    gapAdvertisingData_t*   pAdvertisingData,
    gapScanResponseData_t*  pScanResponseData
);

/*! *********************************************************************************
* \brief  Gap_SetAdvertisingData command monitoring function.
*
* \param[in]    pStaticAdvertisingData      Pointer to gapAdvertisingData_t structure
*                                           or NULL.
* \param[in]    pStaticScanResponseData     Pointer to gapScanResponseData_t structure
*                                           or NULL.
*
********************************************************************************** */
void fsciBleGapSetAdvertisingDataCmdMonitor
(
    const gapAdvertisingData_t*   pAdvertisingData,
    const gapScanResponseData_t*  pScanResponseData
);

/*! *********************************************************************************
* \brief  Gap_StartAdvertising command monitoring function.
*
* \param[in]    advertisingCallback     Callback used by the application to receive
                                        advertising events.
* \param[in]    connectionCallback      Callback used by the application to receive
                                        connection events.
*
********************************************************************************** */
void fsciBleGapStartAdvertisingCmdMonitor
(
    gapAdvertisingCallback_t    advertisingCallback,
    gapConnectionCallback_t     connectionCallback
);

#if gBLE50_d
/*! *********************************************************************************
* \brief  Gap_StartExtAdvertising command monitoring function.
*
* \param[in]    advertisingCallback     Callback used by the application to receive
                                        advertising events.
* \param[in]    connectionCallback      Callback used by the application to receive
                                        connection events.
* \param[in]    handle                  The ID of the advertising set
* \param[in]    duration                The duration of the advertising
* \param[in]    maxExtAdvEvents         The maximum number of advertising events
*
********************************************************************************** */
void fsciBleGapStartExtAdvertisingCmdMonitor
(
    gapAdvertisingCallback_t advertisingCallback,
    gapConnectionCallback_t connectionCallback,
    uint8_t handle,
    uint16_t duration,
    uint8_t maxExtAdvEvents
);

/*! *********************************************************************************
* \brief  Gap_UpdatePeriodicAdvList command monitoring function.
*
* \param[in]    operation     operation to be executed on the list
* \param[in]    addrType      the address type of the device
* \param[in]    pAddr         address of the device
* \param[in]    SID           id of the advertising set used to identify the periodic advertising
*
********************************************************************************** */
void fsciBleGapUpdatePeriodicAdvListCmdMonitor
(
    gapPeriodicAdvListOperation_t operation,
    bleAddressType_t addrType,
    uint8_t * pAddr,
    uint8_t SID
);

/*! *********************************************************************************
* \brief  Gap_SetPeriodicAdvertisingData command monitoring function.
*
* \param[in]    handle                      Identifier of the Periodic Advertising set
* \param[in]    pStaticAdvertisingData      Pointer to gapAdvertisingData_t structure
*
********************************************************************************** */
void fsciBleGapSetPeriodicAdvertisingDataCmdMonitor
(
    uint8_t handle,
    gapAdvertisingData_t*   pAdvertisingData
);

/*! *********************************************************************************
* \brief  Gap_PeriodicAdvCreateSync command monitoring function.
*
* \param[in]    pReq                    Pointer to gapPeriodicAdvSyncReq_t structure
*                                       or NULL
* \param[in]    scanningCallback        Callback used by the application to receive
                                        scanning events.
*
********************************************************************************** */
void fsciBleGapPeriodicAdvCreateSyncCmdMonitor
(
    gapPeriodicAdvSyncReq_t * pReq,
    gapScanningCallback_t     scanningCallback
);
#endif
/*! *********************************************************************************
* \brief  Gap_Authorize command monitoring function.
*
* \param[in]    deviceId    The peer being authorized.
* \param[in]    handle      The attribute handle.
* \param[in]    access      The type of access granted (gAccessRead_c or gAccessWrite_c).
*
********************************************************************************** */
void fsciBleGapAuthorizeCmdMonitor
(
    deviceId_t          deviceId,
    uint16_t            handle,
    gattDbAccessType_t  access
);

/*! *********************************************************************************
* \brief  Gap_SaveCccd command monitoring function.
*
* \param[in]    deviceId    The peer GATT Client.
* \param[in]    handle      The handle of the CCCD as defined in the GATT Database.
* \param[in]    cccd        The bit mask representing the CCCD value to be saved.
*
********************************************************************************** */
void fsciBleGapSaveCccdCmdMonitor
(
    deviceId_t      deviceId,
    uint16_t        handle,
    gattCccdFlags_t cccd
);

/*! *********************************************************************************
* \brief  Gap_GetBondedDevicesIdentityInformation command monitoring function.
*
* \param[in]    aOutIdentityAddresses     Array of addresses to be filled.
* \param[in]    maxDevices                Maximum number of addresses to be obtained.
* \param[in]    pOutActualCount           The actual number of addresses written.
*
********************************************************************************** */
void fsciBleGapGetBondedDevicesIdentityInformationCmdMonitor
(
    const gapIdentityInformation_t*   aOutIdentityAddresses,
    uint8_t                           maxDevices,
    const uint8_t*                    pOutActualCount
);

/*! *********************************************************************************
* \brief  Gap_Pair command monitoring function.
*
* \param[in]    deviceId            The peer to pair with.
* \param[in]    pPairingParameters  Pairing parameters as required by the SMP.
*
********************************************************************************** */
void fsciBleGapPairCmdMonitor
(
    deviceId_t                    deviceId,
    const gapPairingParameters_t  *pPairingParameters
);

/*! *********************************************************************************
* \brief  Gap_SendSlaveSecurityRequest command monitoring function.
*
* \param[in]    deviceId            The GAP peer to pair with.
* \param[in]    pPairingParameters  Pairing parameters as required by the SMP.
*
********************************************************************************** */
void fsciBleGapSendSlaveSecurityRequestCmdMonitor
(
    deviceId_t                    deviceId,
    const gapPairingParameters_t  *pPairingParameters
);

/*! *********************************************************************************
* \brief  Gap_AcceptPairingRequest command monitoring function.
*
* \param[in]    deviceId            The peer requesting authentication.
* \param[in]    pPairingParameters  Pairing parameters as required by the SMP.
*
********************************************************************************** */
void fsciBleGapAcceptPairingRequestCmdMonitor
(
    deviceId_t                    deviceId,
    const gapPairingParameters_t  *pPairingParameters
);

/*! *********************************************************************************
* \brief  Gap_RejectAuthentication command monitoring function.
*
* \param[in]    deviceId    The GAP peer who requested authentication.
* \param[in]    reason      The reason of rejection.
*
********************************************************************************** */
void fsciBleGapRejectPairingCmdMonitor
(
    deviceId_t                          deviceId,
    gapAuthenticationRejectReason_t     reason
);

/*! *********************************************************************************
* \brief  Gap_EnterPasskey command monitoring function.
*
* \param[in]    deviceId    The GAP peer that requested a passkey entry.
* \param[in]    passkey     The peer's secret passkey.
*
********************************************************************************** */
void fsciBleGapEnterPasskeyCmdMonitor
(
    deviceId_t  deviceId,
    uint32_t    passkey
);

/*! *********************************************************************************
* \brief  Gap_ProvideOob command monitoring function.
*
* \param[in]    deviceId    The pairing device.
* \param[in]    aOob        Pointer to OOB data (array of gcSmpOobSize_d size).
*
********************************************************************************** */
void fsciBleGapProvideOobCmdMonitor
(
    deviceId_t  deviceId,
    const uint8_t*    aOob
);

/*! *********************************************************************************
* \brief  Gap_SendSmpKeys command monitoring function.
*
* \param[in]    deviceId    The GAP peer who initiated the procedure.
* \param[in]    pKeys       The SMP keys of the local device.
*
********************************************************************************** */
void fsciBleGapSendSmpKeysCmdMonitor
(
    deviceId_t      deviceId,
    const gapSmpKeys_t*   pKeys
);

/*! *********************************************************************************
* \brief  Gap_ProvideLongTermKey command monitoring function.
*
* \param[in]    deviceId    The GAP peer who requested encryption.
* \param[in]    aLtk        The Long Term Key.
* \param[in]    ltkSize     The Long Term Key size.
*
********************************************************************************** */
void fsciBleGapProvideLongTermKeyCmdMonitor
(
    deviceId_t  deviceId,
    const uint8_t*    aLtk,
    uint8_t     ltkSize
);

/*! *********************************************************************************
* \brief  Gap_LoadEncryptionInformation command monitoring function.
*
* \param[in]    deviceId        Device ID of the peer.
* \param[in]    aOutLtk         Array of size gcMaxLtkSize_d to be filled with the LTK.
* \param[in]    pOutLtkSize     The LTK size.
*
********************************************************************************** */
void fsciBleGapLoadEncryptionInformationCmdMonitor
(
    deviceId_t  deviceId,
    const uint8_t*    aOutLtk,
    const uint8_t*    pOutLtkSize
);

/*! *********************************************************************************
* \brief  Gap_SetLocalPasskey command monitoring function.
*
* \param[in]    passkey     The SMP passkey.
*
********************************************************************************** */
void fsciBleGapSetLocalPasskeyCmdMonitor
(
    uint32_t passkey
);

/*! *********************************************************************************
* \brief  Gap_StartScanning command monitoring function.
*
* \param[in]    pScanningParameters     The scanning parameters, may be NULL.
* \param[in]    scanningCallback        The scanning callback.
* \param[in]    enableFilterDuplicates  Enable or disable duplicate advertising reports
*                                       filtering
*
********************************************************************************** */
void fsciBleGapStartScanningCmdMonitor
(
    const gapScanningParameters_t*    pScanningParameters,
    gapScanningCallback_t       scanningCallback,
    gapFilterDuplicates_t       enableFilterDuplicates,
    uint16_t                    duration,
    uint16_t                    period
);

/*! *********************************************************************************
* \brief  Gap_Connect command monitoring function.
*
* \param[in]    pParameters     Create Connection command parameters.
* \param[in]    connCallback    Callback used to receive connection events.
*
********************************************************************************** */
void fsciBleGapConnectCmdMonitor
(
    const gapConnectionRequestParameters_t*   pParameters,
    gapConnectionCallback_t             connCallback
);

/*! *********************************************************************************
* \brief  Gap_SaveCustomPeerInformation command monitoring function.
*
* \param[in]    deviceId    Device ID of the GAP peer.
* \param[in]    aInfo       Pointer to the beginning of the data.
* \param[in]    offset      Offset from the beginning of the reserved memory area.
* \param[in]    infoSize    Data size (maximum equal to gcReservedFlashSizeForCustomInformation_d).
*
********************************************************************************** */
void fsciBleGapSaveCustomPeerInformationCmdMonitor
(
    deviceId_t  deviceId,
    const uint8_t*    aInfo,
    uint16_t    offset,
    uint16_t    infoSize
);

/*! *********************************************************************************
* \brief  Gap_LoadCustomPeerInformation command monitoring function.
*
* \param[in]    deviceId    Device ID of the GAP peer.
* \param[in]    aOutInfo    Pointer to the beginning of the allocated memory.
* \param[in]    offset      Offset from the beginning of the reserved memory area.
* \param[in]    infoSize    Data size (maximum equal to gcReservedFlashSizeForCustomInformation_d).
*
********************************************************************************** */
void fsciBleGapLoadCustomPeerInformationCmdMonitor
(
    deviceId_t  deviceId,
    const uint8_t*    aOutInfo,
    uint16_t    offset,
    uint16_t    infoSize
);

/*! *********************************************************************************
* \brief  Gap_CheckIfBonded command monitoring function.
*
* \param[in]    deviceId        Device ID of the GAP peer.
* \param[in]    pOutIsBonded    Boolean to be filled with the bonded flag.
*
********************************************************************************** */
void fsciBleGapCheckIfBondedCmdMonitor
(
    deviceId_t  deviceId,
    bool_t*     pOutIsBonded
);

/*! *********************************************************************************
* \brief  Gap_CreateRandomDeviceAddress command monitoring function.
*
* \param[in]    aIrk            The Identity Resolving Key to be used for a private
*                               resolvable address or NULL for a private non-resolvable
*                               address (fully random).
* \param[in]    aRandomPart     If aIrk is not NULL, this is a 3-byte array containing
*                               the Random Part of a Private Resolvable Address, in LSB
*                               to MSB order; the most significant two bits of the most
*                               significant byte (aRandomPart[3] & 0xC0) are ignored.
*                               This may be NULL, in which case the Random Part is
*                               randomly generated internally.
*
********************************************************************************** */
void fsciBleGapCreateRandomDeviceAddressCmdMonitor
(
    const uint8_t*    aIrk,
    const uint8_t*    aRandomPart
);

/*! *********************************************************************************
* \brief  Gap_SaveDeviceName command monitoring function.
*
* \param[in]    deviceId    Device ID for the active peer which name is saved.
* \param[in]    aName       Array of characters holding the name.
* \param[in]    cNameSize   Number of characters to be saved.
*
********************************************************************************** */
void fsciBleGapSaveDeviceNameCmdMonitor
(
    deviceId_t     deviceId,
    const uchar_t* aName,
    uint8_t        cNameSize
);

/*! *********************************************************************************
* \brief  Gap_GetBondedDevicesCount command monitoring function.
*
* \param[in]    pOutBondedDevicesCount  Pointer to integer to be written.
*
********************************************************************************** */
void fsciBleGapGetBondedDevicesCountCmdMonitor
(
    const uint8_t* pOutBondedDevicesCount
);

/*! *********************************************************************************
* \brief  Gap_GetBondedDeviceName command monitoring function.
*
* \param[in]    nvmIndex        Index of the device in NVM bonding area.
* \param[in]    aOutName        Destination array to copy the name into.
* \param[in]    maxNameSize     Maximum number of characters to be copied,
*
********************************************************************************** */
void fsciBleGapGetBondedDeviceNameCmdMonitor
(
    uint8_t     nvmIndex,
    uchar_t*    aOutName,
    uint8_t     maxNameSize
);

/*! *********************************************************************************
* \brief  Gap_ReadRadioPowerLevel command monitoring function.
*
* \param[in]    txReadType      Radio power level read type.
* \param[in]    deviceId        Device identifier.
*
********************************************************************************** */
void fsciBleGapReadRadioPowerLevelCmdMonitor
(
    gapRadioPowerLevelReadType_t    txReadType,
    deviceId_t                      deviceId
);

/*! *********************************************************************************
* \brief  Gap_UpdateLeDataLength command monitoring function.
*
* \param[in]    txReadType      Radio power level read type.
* \param[in]    deviceId        Device identifier.
*
********************************************************************************** */
void fsciBleGapUpdateLeDataLengthCmdMonitor
(
    deviceId_t  deviceId,
    uint16_t    txOctets,
    uint16_t    txTime
);

/*! *********************************************************************************
* \brief  Gap_EnableHostPrivacy command monitoring function.
*
* \param[in]    enable      TRUE to enable, FALSE to disable.
* \param[in]    aIrk        Local IRK to be used for Resolvable Private Address generation
*                           or NULL for Non-Resolvable Private Address generation. Ignored if enable is FALSE.
*
********************************************************************************** */
void fsciBleGapEnableHostPrivacyCmdMonitor
(
    bool_t         enable,
    const uint8_t* aIrk
);

/*! *********************************************************************************
* \brief  Gap_EnableHostPrivacy command monitoring function.
*
* \param[in] enable             TRUE to enable, FALSE to disable.
* \param[in] aOwnIrk            Local IRK. Ignored if enable is FALSE, otherwise shall not be NULL.
* \param[in] peerIdCount        Size of aPeerIdentities array. Ignored if enable is FALSE.
* \param[in] aPeerIdentities    Array of peer identity addresses and IRKs. Ignored if enable is FALSE,
*                               otherwise shall not be NULL.
*
********************************************************************************** */
void fsciBleGapEnableControllerPrivacyCmdMonitor
(
    bool_t                      enable,
    const uint8_t*              aOwnIrk,
    uint8_t                     peerIdCount,
    const gapIdentityInformation_t*   aPeerIdentities
);

/*! *********************************************************************************
* \brief  Gap_SetPrivacyMode command monitoring function.
*
* \param[in] nvmIndex           Index of the device in the NVM bonding area.
* \param[in] privacyMode        Controller privacy mode: Network or Device
*
********************************************************************************** */
void fsciBleGapSetPrivacyModeCmdMonitor
(
    uint8_t nvmIndex,
    blePrivacyMode_t privacyMode
);

/*! *********************************************************************************
* \brief  Gap_LeScValidateNumericValue command monitoring function.
*
* \param[in] deviceId           The DeviceID for which the command is intended.
* \param[in] valid              TRUE if user has indicated that numeric values are matched, FALSE otherwise.
*
********************************************************************************** */
void fsciBleGapLeScValidateNumericValueCmdMonitor
(
    deviceId_t  deviceId,
    bool_t      valid
);

/*! *********************************************************************************
* \brief  Gap_LeScSetPeerOobData command monitoring function.
*
* \param[in] deviceId           Device ID of the peer.
* \param[in] pPeerOobData       OOB data received from the peer.
*
********************************************************************************** */
void fsciBleGapLeScSetPeerOobDataCmdMonitor
(
    deviceId_t          deviceId,
    const gapLeScOobData_t*   pPeerOobData
);

/*! *********************************************************************************
* \brief  Gap_LeScSendKeypressNotification command monitoring function.
*
* \param[in] deviceId               Device ID of the peer.
* \param[in] keypressNotification   Value of the Keypress Notification.
*
********************************************************************************** */
void fsciBleGapLeScSendKeypressNotificationCmdMonitor
(
    deviceId_t                deviceId,
    gapKeypressNotification_t keypressNotification
);

/*! *********************************************************************************
* \brief  Gap_SetTxPowerLevel command monitoring function.
*
* \param[in] powerLevel     Power level to set.
* \param[in] channelType    Type of channel for which the power level is set,
*                           advertising channel or connection channel.
*
********************************************************************************** */
void fsciBleGapSetTxPowerLevelCmdMonitor
(
    uint8_t                         powerLevel,
    bleTransmitPowerChannelType_t   channelType
);

/*! *********************************************************************************
* \brief  Gap_LeReadPhy command monitoring function.
*
* \param deviceId  Device ID of the peer.
*
********************************************************************************** */
void fsciBleGapLeReadPhyCmdMonitor
(
    deviceId_t deviceId
);

/*! *********************************************************************************
* \brief  Gap_LeSetPhy command monitoring function.
*
* \param defaultMode   Use the provided values for all subsequent connections
* \param deviceId      Device ID of the peer
*                      Ignored if defaultMode is TRUE.
* \param allPhys       Host preferences on Tx and Rx Phy, as defined by gapLeAllPhyFlags_t
* \param txPhys        Host preferences on Tx Phy, as defined by gapLePhyFlags_t
* \param rxPhys        Host preferences on Rx Phy, as defined by gapLePhyFlags_t
* \param phyOptions    Host preferences on Coded Phy, as defined by gapLePhyOptionsFlags_t
*                      Ignored if defaultMode is TRUE.
*
********************************************************************************** */
void fsciBleGapLeSetPhyCmdMonitor
(
    bool_t defaultMode,
    deviceId_t deviceId,
    uint8_t allPhys,
    uint8_t txPhys,
    uint8_t rxPhys,
    uint16_t phyOptions
);

/*! *********************************************************************************
* \brief  Gap_ControllerEnhancedNotification command monitoring function.
*
* \param eventType     Event Type selection
* \param deviceId      Device ID of the peer
*
********************************************************************************** */
void fsciBleGapControllerEnhancedNotificationCmdMonitor
(
    uint16_t   eventType,
    deviceId_t deviceId
);

/*! *********************************************************************************
* \brief  Gap_LoadKeys command monitoring function.
*
* \param[in]     nvmIndex    Index to save the keys.
* \param[out]    pOutKeys    The bond keys.
* \param[out]    pOutKeyFlags Key flags.
* \param[out]    leSc        LE SC connections was used.
* \param[out]    auth        Peer was authenticated.
*
********************************************************************************** */
void fsciBleGapLoadKeysCmdMonitor
(
    uint8_t           nvmIndex,
    gapSmpKeys_t*     pOutKeys,
    gapSmpKeyFlags_t* pOutKeyFlags,
    bool_t*           pOutLeSc,
    bool_t*           pOutAuth
);

/*! *********************************************************************************
* \brief  Gap_SaveKeys command monitoring function.
*
* \param[in]    nvmIndex    Index to save the keys.
* \param[in]    pKeys       The bond keys.
* \param[in]    leSc        LE SC connections was used.
* \param[in]    auth        Peer was authenticated.
*
********************************************************************************** */
void fsciBleGapSaveKeysCmdMonitor
(
    uint8_t        nvmIndex,
    gapSmpKeys_t*  pKeys,
    bool_t         leSc,
    bool_t         auth
);

/*! *********************************************************************************
* \brief  Gap_SetChannelMap command monitoring function.
*
* \param channelMap    Channel map to be set in Controller
*
********************************************************************************** */
void fsciBleGapSetChannelMapCmdMonitor
(
    bleChannelMap_t channelMap
);

/*! *********************************************************************************
* \brief  Gap_VerifyPrivateResolvableAddress command monitoring function.
*
* \param[in]    nvmIndex    Index of the device in NVM bonding area whose IRK must
*                           be checked.
* \param[in]    aAddress    The Private Resolvable Address to be verified.
*
********************************************************************************** */
void fsciBleGapVerifyPrivateResolvableAddressCmdMonitor
(
    uint8_t             nvmIndex,
    const bleDeviceAddress_t  aAddress
);

/*! *********************************************************************************
* \brief  Gap_SetRandomAddress command monitoring function.
*
* \param[in]    aAddress    The Private Resolvable, Private Non-Resolvable or Static
*                           Random Address.
*
********************************************************************************** */
void fsciBleGapSetRandomAddressCmdMonitor
(
    const bleDeviceAddress_t aAddress
);

/*! *********************************************************************************
* \brief  Gap_SetScanMode command monitoring function.
*
* \param[in]    discoveryMode       The scan mode to be activated.
* \param[in]    pAutoConnectParams  Parameters for the Auto Connect Scan Mode.
*
********************************************************************************** */
void fsciBleGapSetScanModeCmdMonitor
(
    gapScanMode_t           scanMode,
    gapAutoConnectParams_t* pAutoConnectParams
);


/*! *********************************************************************************
* \brief  Gap_SetDefaultPairingParameters command monitoring function.
*
* \param[in]    pPairingParameters  Pairing parameters as required by the SMP or NULL.
*
********************************************************************************** */
void fsciBleGapSetDefaultPairingParametersCmdMonitor
(
    const gapPairingParameters_t* pPairingParameters
);

/*! *********************************************************************************
* \brief  Gap_UpdateConnectionParameters command monitoring function.
*
* \param[in]    deviceId            The DeviceID for which the command is intended.
* \param[in]    intervalMin         The minimum value for the connection event interval.
* \param[in]    intervalMax         The maximum value for the connection event interval.
* \param[in]    slaveLatency        The slave latency parameter.
* \param[in]    timeoutMultiplier   The connection timeout parameter.
* \param[in]    minCeLength         The minimum connection event length.
* \param[in]    maxCeLength         The maximum connection event length.
*
********************************************************************************** */
void fsciBleGapUpdateConnectionParametersCmdMonitor
(
    deviceId_t  deviceId,
    uint16_t    intervalMin,
    uint16_t    intervalMax,
    uint16_t    slaveLatency,
    uint16_t    timeoutMultiplier,
    uint16_t    minCeLength,
    uint16_t    maxCeLength
);

/*! *********************************************************************************
* \brief  Gap_EnableUpdateConnectionParameters command monitoring function.
*
* \param[in]    deviceId            The DeviceID for which the command is intended.
* \param[in]    enable              Allow/disallow the parameters update.
*
********************************************************************************** */
void fsciBleGapEnableUpdateConnectionParametersCmdMonitor
(
    deviceId_t  deviceId,
    bool_t      enable
);

/*! *********************************************************************************
* \brief  Gap_SetPeriodicAdvParameters command monitoring function.
*
* \param[in]    pAdvertisingParameters      Pointer to gapPeriodicAdvParameters_t
*                                           structure.
*
********************************************************************************** */
void fsciBleGapSetPeriodicAdvParametersCmdMonitor
(
    gapPeriodicAdvParameters_t* pAdvertisingParameters
);
#endif /* gFsciBleHost_d || gFsciBleTest_d */
/*! *********************************************************************************
* \brief  Creates a GAP FSCI packet with a boolean payload and sends it over UART.
*
* \param[in]    opCode      GAP event operation code.
* \param[in]    bParam      The boolean parameter.
*
********************************************************************************** */
void fsciBleGapBoolParamEvtMonitor
(
    fsciBleGapOpCode_t  opCode,
    bool_t              bParam
);

/*! *********************************************************************************
* \brief  Creates a GAP FSCI packet which contains an array as payload and sends
*         it over UART.
*
* \param[in]    opCode      GAP event operation code.
* \param[in]    aArray      The array.
* \param[in]    arraySize   The array size.
*
********************************************************************************** */
void fsciBleGapArrayAndSizeParamEvtMonitor
(
    fsciBleGapOpCode_t  opCode,
    const uint8_t*      aArray,
    uint8_t             arraySize
);

/*! *********************************************************************************
* \brief  Gap_GetBondedDevicesIdentityInformation command out parameters monitoring function.
*
* \param[in]    aOutIdentityAddresses   Array of addresses.
* \param[in]    pOutActualCount         The number of addresses.
*
********************************************************************************** */
void fsciBleGapGetBondedDevIdentityInfoEvtMonitor
(
    const gapIdentityInformation_t*   aOutIdentityAddresses,
    const uint8_t*                    pOutActualCount
);

/*! *********************************************************************************
* \brief  Gap_LoadCustomPeerInformation command out parameters monitoring function.
*
* \param[in]    aOutInfo    Pointer to the beginning of the allocated memory.
* \param[in]    infoSize    Data size (maximum equal to gcReservedFlashSizeForCustomInformation_d).
*
********************************************************************************** */
void fsciBleGapLoadCustomPeerInfoEvtMonitor
(
    const uint8_t*    aOutInfo,
    uint16_t    infoSize
);

/*! *********************************************************************************
* \brief  Gap_LoadKeys out parameter monitoring macro.
*
* \param[in]    pOutIsActive    Pointer to the keys.
* \param[in]    pOutKeyFlags    Pointer to the key flags.
* \param[in]    pOutLeSc        Pointer to the LE SC used.
* \param[in]    pOutAuth        Pointer to the authenticated used.
*
********************************************************************************** */
void fsciBleGapLoadKeysEvtMonitor
(
    gapSmpKeys_t*     pOutKeys,
    gapSmpKeyFlags_t* pOutKeyFlags,
    bool_t*           pOutLeSc,
    bool_t*           pOutAuth
);

/*! *********************************************************************************
* \brief  gapGenericCallback event monitoring function.
*
* \param[in]    pGenericEvent    Controller event information.
*
********************************************************************************** */
void fsciBleGapGenericEvtMonitor
(
    gapGenericEvent_t* pGenericEvent
);

/*! *********************************************************************************
* \brief  gapAdvertisingCallback event monitoring function.
*
* \param[in]    pAdvertisingEvent    Advertising event information.
*
********************************************************************************** */
void fsciBleGapAdvertisingEvtMonitor
(
    gapAdvertisingEvent_t* pAdvertisingEvent
);

/*! *********************************************************************************
* \brief  gapScanningCallback event monitoring function.
*
* \param[in]    pScanningEvent    Scanning event information.
*
********************************************************************************** */
void fsciBleGapScanningEvtMonitor
(
    gapScanningEvent_t* pScanningEvent
);

/*! *********************************************************************************
* \brief  gapConnectionCallback event monitoring function.
*
* \param[in]    deviceId            Device identifier.
* \param[in]    pConnectionEvent    Connection event information.
*
********************************************************************************** */
void fsciBleGapConnectionEvtMonitor
(
    deviceId_t              deviceId,
    gapConnectionEvent_t*   pConnectionEvent
);

#ifdef __cplusplus
}
#endif

#endif /* FSCI_BLE_ATT_H */

/*! *********************************************************************************
* @}
********************************************************************************** */
