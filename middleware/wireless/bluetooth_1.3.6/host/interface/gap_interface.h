/*! *********************************************************************************
 * \defgroup GAP Generic Access Profile
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef GAP_INTERFACE_H
#define GAP_INTERFACE_H

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "ble_general.h"
#include "gap_types.h"
#include "gatt_types.h"
#include "SecLib.h"

/************************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief  Macro used to combine two security mode-levels.
*
* \param[in] modeLevelA,modeLevelB The two security mode-levels.
*
* \return  The resulting security mode-level.
*
* \remarks This macro is useful when two different security requirements
* must be satisfied at the same time, such as a device master security requirement
* and a service-specific security requirement.
*
********************************************************************************** */
#define Gap_AddSecurityModesAndLevels(modeLevelA, modeLevelB) \
   ((isSameMode((modeLevelA), (modeLevelB))) ? \
    (addSameSecurityModes((modeLevelA), (modeLevelB))) : \
    (addDifferentSecurityModes((modeLevelA), (modeLevelB))))

/*! *********************************************************************************
* \brief  Macro used to cancel a connection initiated by Gap_Connect(...).
*
* \return  gBleSuccess_c or error.
*
* \remarks This macro can only be used for a connection that has not yet been established, such as
* the "gConnEvtConnected_c" has not been received. For example, call this when a connection
* request has timed out. Application should listen for gCreateConnectionCanceled_c generic event.
********************************************************************************** */
#define Gap_CancelInitiatingConnection()\
    Gap_Disconnect(gCancelOngoingInitiatingConnection_d)

/*! *********************************************************************************
* \brief  Macro used to read the radio transmitter power when advertising.
*
* \return  gBleSuccess_c or error.
*
* \remarks The result is contained in the gAdvTxPowerLevelRead_c generic event.
********************************************************************************** */
#define Gap_ReadAdvertisingTxPowerLevel()\
    Gap_ReadRadioPowerLevel(gTxPowerLevelForAdvertising_c, 0U)

/*! *********************************************************************************
* \brief  Macro used to read the RSSI of a radio connection.
*
* \param[in] deviceId Device ID identifying the radio connection.
*
* \return  gBleSuccess_c or error.
*
* \remarks The result is contained in the gConnEvtRssiRead_c connection event. The RSSI
* value is a signed byte, and the unit is dBm. If the RSSI cannot be read, the gConnEvtPowerReadFailure_c
* connection event is generated.
********************************************************************************** */
#define Gap_ReadRssi(deviceId)\
    Gap_ReadRadioPowerLevel(gRssi_c, (deviceId))

/*! *********************************************************************************
* \brief  Macro used to read the radio transmitting power level of a radio connection.
*
* \param[in] deviceId Device ID identifying the radio connection.
*
* \return  gBleSuccess_c or error.
*
* \remarks The result is contained in the gConnEvtTxPowerLevelRead_c connection event.
* If the TX Power cannot be read, the gConnEvtPowerReadFailure_c
* connection event is generated.
********************************************************************************** */
#define Gap_ReadTxPowerLevelInConnection(deviceId)\
    Gap_ReadRadioPowerLevel(gTxPowerCurrentLevelInConnection_c, (deviceId))

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*! *********************************************************************************
* \brief  Registers the device security requirements.
* This function includes a master security for all services and, optionally, additional stronger security
* settings for services as required by the profile and/or application.
*
* \param[in] pSecurity A pointer to the application-allocated gapDeviceSecurityRequirements_t structure.
*
* \return  gBleSuccess_c or error.
*
* \remarks pSecurity or any other contained security structure pointers that are NULL are ignored,
* i.e., defaulted to No Security (Security Mode 1 Level 1, No Authorization, Minimum encryption key size).
* This function executes synchronously.
*
* \remarks GATT Server-only API function.
*
********************************************************************************** */
bleResult_t Gap_RegisterDeviceSecurityRequirements
(
    const gapDeviceSecurityRequirements_t* pSecurity
);

/*! *********************************************************************************
* \brief  Sets up the Advertising Parameters.
*
* \param[in] pAdvertisingParameters   Pointer to gapAdvertisingParameters_t structure.
*
* \return  gBleSuccess_c or error.
*
* \remarks GAP Peripheral-only API function.
*
********************************************************************************** */
bleResult_t Gap_SetAdvertisingParameters
(
    const gapAdvertisingParameters_t*   pAdvertisingParameters
);

/*! *********************************************************************************
* \brief  Sets up the Advertising and Scan Response Data.
*
* \param[in] pAdvertisingData   Pointer to gapAdvertisingData_t structure or NULL.
* \param[in] pScanResponseData  Pointer to gapScanResponseData_t structure or NULL.
*
* \return  gBleSuccess_c or error.
*
* \remarks Any of the parameters may be NULL, in which case they are ignored.
* Therefore, this function can be used to set any of the parameters individually or both at once.
* The standard advertising packet payload is 37 bytes. Some of the payload may be occupied by
* the Advertisser Address which takes up 6 bytes and for some advertising PDU types also by
* the Initiator Address which takes another 6 bytes. This leaves 25-31 bytes to the application
* to include advertising structures (Length [1Byte], AD Type [1 Byte], AD Data[Length-1 Bytes])
*
* \remarks GAP Peripheral-only API function.
*
********************************************************************************** */
bleResult_t Gap_SetAdvertisingData
(
    const gapAdvertisingData_t*         pAdvertisingData,
    const gapScanResponseData_t*        pScanResponseData
);

/*! *********************************************************************************
* \brief  Commands the controller to start advertising.
*
* \param[in] advertisingCallback   Callback used by the application to receive advertising events.
*                                  Can be NULL.
* \param[in] connectionCallback    Callback used by the application to receive connection events.
*                                  Can be NULL.
*
* \return  gBleSuccess_c or error.
*
* \remarks The advertisingCallback confirms or denies whether the advertising has started.
* The connectionCallback is only used if a connection gets established during advertising.
*
* \remarks GAP Peripheral-only API function.
*
********************************************************************************** */
bleResult_t Gap_StartAdvertising
(
    gapAdvertisingCallback_t    advertisingCallback,
    gapConnectionCallback_t     connectionCallback
);

/*! *********************************************************************************
* \brief  Commands the controller to stop advertising.
*
* \return  gBleSuccess_c or error.
*
* \remarks GAP Peripheral-only API function.
*
********************************************************************************** */
bleResult_t Gap_StopAdvertising(void);

/*! *********************************************************************************
* \brief  Authorizes a peer for an attribute in the database.
*
* \param[in] deviceId   The peer being authorized.
* \param[in] handle     The attribute handle.
* \param[in] access     The type of access granted (gAccessRead_c or gAccessWrite_c).
*
* \return  gBleSuccess_c or error.
*
* \remarks This function executes synchronously.
*
* \remarks GATT Server-only API function.
*
********************************************************************************** */
bleResult_t Gap_Authorize
(
    deviceId_t          deviceId,
    uint16_t            handle,
    gattDbAccessType_t  access
);

/*! *********************************************************************************
* \brief  Save the CCCD value for a specific Client and CCCD handle.
*
* \param[in] deviceId    The peer GATT Client.
* \param[in] handle      The handle of the CCCD as defined in the GATT Database.
* \param[in] cccd        The bit mask representing the CCCD value to be saved.
*
* \return  gBleSuccess_c or error.
*
* \remarks The GATT Server layer saves the CCCD value automatically when it is written by the Client.
* This API should only be used to save the CCCD in other situations, e.g., when for some reason
* the application decides to disable notifications/indications for a specific Client.
*
* \remarks This function executes synchronously.
*
* \remarks GATT Server-only API function.
*
********************************************************************************** */
bleResult_t Gap_SaveCccd
(
    deviceId_t      deviceId,
    uint16_t        handle,
    gattCccdFlags_t cccd
);

/*! *********************************************************************************
* \brief  Retrieves the notification status for a given Client and a given CCCD handle.
*
* \param[in]  deviceId      The peer GATT Client.
* \param[in]  handle        The handle of the CCCD.
* \param[out] pOutIsActive  The address to store the status into.
*
* \return  gBleSuccess_c or error.
*
* \remarks This function executes synchronously.
*
* \remarks GATT Server-only API function.
*
********************************************************************************** */
bleResult_t Gap_CheckNotificationStatus
(
    deviceId_t  deviceId,
    uint16_t    handle,
    bool_t*     pOutIsActive
);

/*! *********************************************************************************
* \brief  Retrieves the indication status for a given Client and a given CCCD handle.
*
* \param[in]  deviceId      The peer GATT Client.
* \param[in]  handle        The handle of the CCCD.
* \param[out] pOutIsActive  The address to store the status into.
*
* \return  gBleSuccess_c or error.
*
* \remarks This function executes synchronously.
*
* \remarks GATT Server-only API function.
*
********************************************************************************** */
bleResult_t Gap_CheckIndicationStatus
(
    deviceId_t  deviceId,
    uint16_t    handle,
    bool_t*     pOutIsActive
);

/*! *********************************************************************************
* \brief  Retrieves a list of the identity information of bonded devices, if any.
*
* \param[out] aOutIdentityAddresses  Array of identities to be filled.
* \param[in]  maxDevices             Maximum number of identities to be obtained.
* \param[out] pOutActualCount        The actual number of identities written.
*
* \return  gBleSuccess_c or error.
*
* \remarks This API may be useful when creating a white list or a resolving list.
*
* \remarks This function executes synchronously.
*
********************************************************************************** */
bleResult_t Gap_GetBondedDevicesIdentityInformation
(
    gapIdentityInformation_t*   aOutIdentityAddresses,
    uint8_t                     maxDevices,
    uint8_t*                    pOutActualCount
);

/*! *********************************************************************************
* \brief  Initiates pairing with a peer device.
*
* \param[in] deviceId            The peer to pair with.
* \param[in] pPairingParameters  Pairing parameters as required by the SMP.
*
* \return  gBleSuccess_c or error.
*
* \remarks GAP Central-only API function.
*
********************************************************************************** */
bleResult_t Gap_Pair
(
    deviceId_t                        deviceId,
    const gapPairingParameters_t*     pPairingParameters
);

/*! *********************************************************************************
* \brief  Informs the peer Master about the local security requirements.
*
* \param[in] deviceId               The GAP peer to pair with.
* \param[in] pPairingParameters     Pairing parameters as required by the SMP.
*
* \return  gBleSuccess_c or error.
*
* \remarks The procedure has the same parameters as the pairing request, but, because it is initiated
* by the Slave, it has no pairing effect. It only informs the Master about the requirements.
*
* \remarks GAP Peripheral-only API function.
*
********************************************************************************** */
bleResult_t Gap_SendSlaveSecurityRequest
(
    deviceId_t                      deviceId,
    const gapPairingParameters_t    *pPairingParameters
);

/*! *********************************************************************************
* \brief  Encrypts the link with a bonded peer.
*
* \param[in] deviceId   Device ID of the peer.
*
* \return  gBleSuccess_c or error.
*
* \remarks GAP Central-only API function.
*
********************************************************************************** */
bleResult_t Gap_EncryptLink
(
    deviceId_t                  deviceId
);

/*! *********************************************************************************
* \brief  Accepts the pairing request from a peer.
*
* \param[in] deviceId            The peer requesting authentication.
* \param[in] pPairingParameters  Pairing parameters as required by the SMP.
*
* \return  gBleSuccess_c or error.
*
* \remarks This should be called in response to a gPairingRequest_c event.
*
* \remarks GAP Peripheral-only API function.
*
********************************************************************************** */
bleResult_t Gap_AcceptPairingRequest
(
    deviceId_t                        deviceId,
    const gapPairingParameters_t*     pPairingParameters
);

/*! *********************************************************************************
* \brief  Rejects the peer's authentication request.
*
* \param[in] deviceId The GAP peer who requested authentication.
*
* \param[in] reason Reason why the current device rejects the authentication.
*
* \return  gBleSuccess_c or error.
*
********************************************************************************** */
bleResult_t Gap_RejectPairing
(
    deviceId_t                      deviceId,
    gapAuthenticationRejectReason_t reason
);

/*! *********************************************************************************
* \brief  Enters the passkey requested by the peer during the pairing process.
*
* \param[in] deviceId      The GAP peer that requested a passkey entry.
* \param[in] passkey       The peer's secret passkey.
*
* \return  gBleSuccess_c or error.
*
********************************************************************************** */
bleResult_t Gap_EnterPasskey
(
    deviceId_t  deviceId,
    uint32_t    passkey
);

/*! *********************************************************************************
* \brief  Provides the Out-Of-Band data for the SMP Pairing process.
*
* \param[in] deviceId   The pairing device.
* \param[in] aOob       Pointer to OOB data (array of gcSmpOobSize_d size).
*
* \return  gBleSuccess_c or error.
*
********************************************************************************** */
bleResult_t Gap_ProvideOob
(
    deviceId_t      deviceId,
    const uint8_t*  aOob
);

/*! *********************************************************************************
* \brief  Rejects the passkey request from a peer.
*
* \param[in] deviceId The GAP peer that requested a passkey entry.
*
* \return  gBleSuccess_c or error.
*
* \remarks GAP Central-only API function.
*
********************************************************************************** */
bleResult_t Gap_RejectPasskeyRequest
(
    deviceId_t deviceId
);

/*! *********************************************************************************
* \brief  Sends the SMP keys during the SMP Key Exchange procedure.
*
* \param[in] deviceId   The GAP peer who initiated the procedure.
* \param[in] pKeys      The SMP keys of the local device.
*
* \return  gBleSuccess_c or error.
*
********************************************************************************** */
bleResult_t Gap_SendSmpKeys
(
    deviceId_t      deviceId,
    const gapSmpKeys_t*   pKeys
);

/*! *********************************************************************************
* \brief  Rejects the Key Exchange procedure with a paired peer.
*
* \param[in] deviceId  The GAP peer who requested the Key Exchange procedure.
*
* \return  gBleSuccess_c or error.
*
********************************************************************************** */
bleResult_t Gap_RejectKeyExchangeRequest
(
    deviceId_t deviceId
);

/*! *********************************************************************************
* \brief  Regenerates the private/public key pair used for LE Secure Connections pairing.
*
* \return  gBleSuccess_c or error.
*
* \remarks The application should listen for the gLeScPublicKeyRegenerated_c generic event.
*
********************************************************************************** */
bleResult_t Gap_LeScRegeneratePublicKey(void);

/*! *********************************************************************************
* \brief  Validates the numeric value during the Numeric Comparison LE Secure Connections pairing.
*
* \param deviceId  Device ID of the peer.
*
* \param valid  TRUE if user has indicated that numeric values are matched, FALSE otherwise.
*
* \return  gBleSuccess_c or error.
*
********************************************************************************** */
bleResult_t Gap_LeScValidateNumericValue
(
    deviceId_t  deviceId,
    bool_t      valid
);

/*! *********************************************************************************
* \brief  Retrieves local OOB data used for LE Secure Connections pairing.
*
* \return  gBleSuccess_c or error.
*
* \remarks The application should listen for the gLeScLocalOobData_c generic event.
*
********************************************************************************** */
bleResult_t Gap_LeScGetLocalOobData(void);

/*! *********************************************************************************
* \brief  Sets peer OOB data used for LE Secure Connections pairing.
*
* \param deviceId       Device ID of the peer.
*
* \param pPeerOobData   OOB data received from the peer.
*
* \return  gBleSuccess_c or error.
*
* \remarks This function should be called in response to the gConnEvtLeScOobDataRequest_c event.
*
********************************************************************************** */
bleResult_t Gap_LeScSetPeerOobData
(
    deviceId_t                    deviceId,
    const gapLeScOobData_t*       pPeerOobData
);

/*! *********************************************************************************
* \brief  Sends a Keypress Notification to the peer.
*
* \param deviceId               Device ID of the peer.
* \param keypressNotification   Value of the Keypress Notification.
*
* \return  gBleSuccess_c or error.
*
* \remarks This function shall only be called during the passkey entry process and
           only if both peers support Keypress Notifications.
*
********************************************************************************** */
bleResult_t Gap_LeScSendKeypressNotification
(
    deviceId_t                  deviceId,
    gapKeypressNotification_t   keypressNotification
);

/*! *********************************************************************************
* \brief  Provides the Long Term Key (LTK) to the controller for encryption setup.
*
* \param[in] deviceId   The GAP peer who requested encryption.
* \param[in] aLtk       The Long Term Key.
* \param[in] ltkSize    The Long Term Key size.
*
* \return  gBleSuccess_c or error.
*
* \remarks The application should provide the same LTK used during bonding with the respective peer.
*
* \remarks GAP Peripheral-only API function.
*
********************************************************************************** */
bleResult_t Gap_ProvideLongTermKey
(
    deviceId_t        deviceId,
    const uint8_t*    aLtk,
    uint8_t           ltkSize
);

/*! *********************************************************************************
* \brief  Rejects an LTK request originating from the controller.
*
* \param[in] deviceId The GAP peer who requested encryption.
*
* \return  gBleSuccess_c or error.
*
* \remarks GAP Peripheral-only API function.
*
********************************************************************************** */
bleResult_t Gap_DenyLongTermKey
(
    deviceId_t deviceId
);

/*! *********************************************************************************
* \brief  Loads the encryption key for a bonded device.
*
* \param[in]  deviceId          Device ID of the peer.
* \param[out] aOutLtk           Array of size gcMaxLtkSize_d to be filled with the LTK.
* \param[out] pOutLtkSize       The LTK size.
*
* \return  gBleSuccess_c or error.
*
* \remarks This function executes synchronously.
*
********************************************************************************** */
bleResult_t Gap_LoadEncryptionInformation
(
    deviceId_t   deviceId,
    uint8_t*     aOutLtk,
    uint8_t*     pOutLtkSize
);

/*! *********************************************************************************
* \brief  Sets the SMP passkey for this device.
*
* \param[in] passkey    The SMP passkey.
*
* \return  gBleSuccess_c or error.
*
* \remarks This is the PIN that the peer's user must enter during pairing.
*
* \remarks This function executes synchronously.
*
* \remarks GAP Peripheral-only API function.
*
********************************************************************************** */
bleResult_t Gap_SetLocalPasskey
(
    uint32_t passkey
);

/*! *********************************************************************************
* \brief  Sets internal scan filters and actions.
*
* \param[in] scanMode           The scan mode to be activated. Default is gDefaultScan_c.
* \param[in] pAutoConnectParams Pointer to the gapAutoConnectParams_t structures if scanMode
*                               is set to gAutoConnect_c. The memory used must be persistent
*                               and should not change during the next scan periods or until
*                               another Gap_SetScanMode is called.
* \param[in] connCallback       Auto-Connect callback. Must be set if scanMode is set to gAutoConnect_c.
*
* \return  gBleSuccess_c or error.
*
* \remarks This function can be called before Gap_StartScanning. If this function is
* never called, then the default value of gDefaultScan_c is considered and all scanned
* devices are reported to the application without any additional filtering or action.
*
* \remarks This function executes synchronously.
*
* \remarks GAP Central-only API function.
*
********************************************************************************** */
bleResult_t Gap_SetScanMode
(
    gapScanMode_t           scanMode,
    gapAutoConnectParams_t* pAutoConnectParams,
    gapConnectionCallback_t connCallback
);

/*! *********************************************************************************
* \brief  Optionally sets the scanning parameters and begins scanning.
*
* \param[in] pScanningParameters The scanning parameters; may be NULL.
* \param[in] scanningCallback The scanning callback.
* \param[in] enableFilterDuplicates Enable or disable duplicate advertising
*            report filtering
* \param[in] duration  Scan duration. Used only for BLE5.0, otherwise ignored.
* \param[in] period    Time interval from when the Controller started its last Scan_Duration
*                      until it begins the subsequent Scan_Duration.
                       Used only for BLE5.0, otherwise ignored.
* \return  gBleSuccess_c or error.
*
* \remarks Use this API to both set the scanning parameters and start scanning.
* If pScanningParameters is NULL, scanning is started with the existing settings.
*
* \remarks GAP Central-only API function.
*
********************************************************************************** */
bleResult_t Gap_StartScanning
(
    const gapScanningParameters_t*    pScanningParameters,
    gapScanningCallback_t       scanningCallback,
    gapFilterDuplicates_t       enableFilterDuplicates,
    uint16_t                    duration,
    uint16_t                    period

);

/*! *********************************************************************************
* \brief  Commands the controller to stop scanning.
*
* \return  gBleSuccess_c or error.
*
* \remarks GAP Central-only API function.
*
********************************************************************************** */
bleResult_t Gap_StopScanning(void);

/*! *********************************************************************************
* \brief  Connects to a scanned device.
*
* \param[in] pParameters  Create Connection command parameters.
* \param[in] connCallback Callback used to receive connection events.
*
* \return  gBleSuccess_c or error.
*
* \remarks GAP Central-only API function.
*
********************************************************************************** */
bleResult_t Gap_Connect
(
    const gapConnectionRequestParameters_t*   pParameters,
    gapConnectionCallback_t                   connCallback
);

/*! *********************************************************************************
* \brief  Initiates disconnection from a connected peer device.
*
* \param[in] deviceId The connected peer to disconnect from.
*
* \return  gBleSuccess_c or error.
*
********************************************************************************** */
bleResult_t Gap_Disconnect
(
    deviceId_t deviceId
);

/*! *********************************************************************************
* \brief  Saves custom peer information in raw data format.
*
* \param[in] deviceId          Device ID of the GAP peer.
* \param[in] aInfo             Pointer to the beginning of the data.
* \param[in] offset            Offset from the beginning of the reserved memory area.
* \param[in] infoSize          Data size (maximum equal to gcReservedFlashSizeForCustomInformation_d).
*
* \return  gBleSuccess_c or error.
*
* \remarks This function can be called by the application to save custom information about the
* peer device, e.g., Service Discovery data (to avoid doing it again on reconnection).
*
* \remarks This function executes synchronously.
*
********************************************************************************** */
bleResult_t Gap_SaveCustomPeerInformation
(
    deviceId_t        deviceId,
    const uint8_t*    aInfo,
    uint16_t          offset,
    uint16_t          infoSize
);

/*! *********************************************************************************
* \brief  Loads the custom peer information in raw data format.
*
* \param[in] deviceId           Device ID of the GAP peer.
* \param[out] aOutInfo          Pointer to the beginning of the allocated memory.
* \param[in] offset             Offset from the beginning of the reserved memory area.
* \param[in] infoSize           Data size (maximum equal to gcReservedFlashSizeForCustomInformation_d).
*
* \return  gBleSuccess_c or error.
*
* \remarks This function can be called by the application to load custom information about the
* peer device, e.g., Service Discovery data (to avoid doing it again on reconnection).
*
* \remarks This function executes synchronously.
*
********************************************************************************** */
bleResult_t Gap_LoadCustomPeerInformation
(
    deviceId_t  deviceId,
    uint8_t*    aOutInfo,
    uint16_t    offset,
    uint16_t    infoSize
);

/*! *********************************************************************************
* \brief  Returns whether or not a connected peer device is bonded.
*
* \param[in] deviceId           Device ID of the GAP peer.
* \param[out] pOutIsBonded      Boolean to be filled with the bonded flag.
*
* \return  gBleSuccess_c or error.
*
* \remarks This function executes synchronously.
*
********************************************************************************** */
bleResult_t Gap_CheckIfBonded
(
    deviceId_t  deviceId,
    bool_t*     pOutIsBonded
);

/*! *********************************************************************************
* \brief  Retrieves the size of the White List.
*
* \return  gBleSuccess_c or error.
*
* \remarks Response is received in the gWhiteListSizeReady_c generic event.
*
********************************************************************************** */
bleResult_t Gap_ReadWhiteListSize(void);

/*! *********************************************************************************
* \brief  Removes all addresses from the White List, if any.
*
* \return  gBleSuccess_c or error.
*
* \remarks Confirmation is received in the gWhiteListCleared_c generic event.
*
********************************************************************************** */
bleResult_t Gap_ClearWhiteList(void);

/*! *********************************************************************************
* \brief  Adds a device address to the White List.
*
* \param[in] address The address of the white-listed device.
* \param[in] addressType The device address type (public or random).
*
* \return  gBleSuccess_c or error.
*
********************************************************************************** */
bleResult_t Gap_AddDeviceToWhiteList
(
    bleAddressType_t          addressType,
    const bleDeviceAddress_t  address
);

/*! *********************************************************************************
* \brief  Removes a device address from the White List.
*
* \param[in] address The address of the white-listed device.
* \param[in] addressType The device address type (public or random).
*
* \return  gBleSuccess_c or error.
*
********************************************************************************** */
bleResult_t Gap_RemoveDeviceFromWhiteList
(
    bleAddressType_t          addressType,
    const bleDeviceAddress_t  address
);

/*! *********************************************************************************
* \brief  Reads the device's public address from the controller.
*
* \return  gBleSuccess_c or error.
*
* \remarks The application should listen for the gPublicAddressRead_c generic event.
*
********************************************************************************** */
bleResult_t Gap_ReadPublicDeviceAddress(void);

/*! *********************************************************************************
* \brief  Requests the controller to create a random address.
*
* \param[in] aIrk           The Identity Resolving Key to be used for a private resolvable address
*                           or NULL for a private non-resolvable address (fully random).
* \param[in] aRandomPart    If aIrk is not NULL, this is a 3-byte array containing the Random Part
*                           of a Private Resolvable Address, in LSB to MSB order; the most significant
*                           two bits of the most significant byte (aRandomPart[3] & 0xC0) are ignored.
*                           This may be NULL, in which case the Random Part is randomly generated internally.
*
* \return  gBleSuccess_c or error.
*
* \remarks The application should listen for the gRandomAddressReady_c generic event.
*          Note that this does not set the random address in the Controller. To set the random address,
*          call Gap_SetRandomAddress() with the generated address contained in the event data.
*
********************************************************************************** */
bleResult_t Gap_CreateRandomDeviceAddress
(
    const uint8_t* aIrk,
    const uint8_t* aRandomPart
);

/*! *********************************************************************************
* \brief  Store the name of a bonded device.
*
* \param[in]  deviceId      Device ID for the active peer which name is saved.
* \param[in]  aName         Array of characters holding the name.
* \param[in]  cNameSize     Number of characters to be saved.
*
* \return  gBleSuccess_c or error.
* \remarks This function copies cNameSize characters from the aName array and
* adds the NULL character to terminate the string.
*
* \remarks This function executes synchronously.
*
********************************************************************************** */
bleResult_t Gap_SaveDeviceName
(
    deviceId_t        deviceId,
    const uchar_t*    aName,
    uint8_t           cNameSize
);

/*! *********************************************************************************
* \brief  Retrieves the number of bonded devices.
*
* \param[out] pOutBondedDevicesCount   Pointer to integer to be written.
*
* \return  gBleSuccess_c or error.
*
* \remarks This function executes synchronously.
*
********************************************************************************** */
bleResult_t Gap_GetBondedDevicesCount
(
    uint8_t*    pOutBondedDevicesCount
);

/*! *********************************************************************************
* \brief  Retrieves the name of a bonded device.
*
* \param[in]  nvmIndex      Index of the device in NVM bonding area.
* \param[out] aOutName      Destination array to copy the name into.
* \param[in]  maxNameSize   Maximum number of characters to be copied,
*                           including the terminating NULL character.
*
* \return  gBleSuccess_c or error.
*
* \remarks nvmIndex is an integer ranging from 0 to N-1, where N is the number of
* bonded devices and can be obtained by calling Gap_GetBondedDevicesCount(&N).
*
* \remarks This function executes synchronously.
*
********************************************************************************** */
bleResult_t Gap_GetBondedDeviceName
(
    uint8_t     nvmIndex,
    uchar_t*    aOutName,
    uint8_t     maxNameSize
);

/*! *********************************************************************************
* \brief  Removes the bond with a device.
*
* \param[in] nvmIndex           Index of the device in the NVM bonding area.
*
* \return  gBleSuccess_c or error.
*
* \remarks This API requires that there are no active connections at call time.
* nvmIndex is an integer ranging from 0 to N-1, where N is the number of
* bonded devices and can be obtained by calling Gap_GetBondedDevicesCount(&N).
*
* \remarks This function executes synchronously.
*
********************************************************************************** */
bleResult_t Gap_RemoveBond
(
    uint8_t nvmIndex
);

/*! *********************************************************************************
* \brief  Removes all bonds with other devices.
*
* \return  gBleSuccess_c or error.
*
* \remarks This API requires that there are no active connections at call time.
*
* \remarks This function executes synchronously.
*
********************************************************************************** */
bleResult_t Gap_RemoveAllBonds(void);

/*! *********************************************************************************
* \brief  Reads the power level of the controller's radio.
*
* \return  gBleSuccess_c or error.
*
* \remarks The response is contained in the gConnEvtTxPowerLevelRead_c connection event when
* reading connection TX power level, the gAdvTxPowerLevelRead_c generic event when reading
* the advertising TX power level, or the gConnEvtRssiRead_c connection event when reading the RSSI.
*
********************************************************************************** */
bleResult_t Gap_ReadRadioPowerLevel
(
    gapRadioPowerLevelReadType_t    txReadType,
    deviceId_t                      deviceId
);

/*! *********************************************************************************
* \brief  Sets the Tx power level on the controller's radio.
*
* \param[in]  powerLevel      Power level as specified in the controller interface.
* \param[in]  channelType     The advertising or connnection channel type.
*
* \return  gBleSuccess_c or error.
*
* \remarks The application should listen for the gTxPowerLevelSetComplete_c generic event.
* \remarks This function executes synchronously.
*
********************************************************************************** */
bleResult_t Gap_SetTxPowerLevel
(
    uint8_t                         powerLevel,
    bleTransmitPowerChannelType_t   channelType
);

/*! *********************************************************************************
* \brief  Verifies a Private Resolvable Address with a bonded device's IRK.
*
* \param[in]  nvmIndex      Index of the device in NVM bonding area whose IRK must be checked.
* \param[in]  aAddress      The Private Resolvable Address to be verified.
*
* \return  gBleSuccess_c or error.
*
* \remarks nvmIndex is an integer ranging from 0 to N-1, where N is the number of
* bonded devices and can be obtained by calling Gap_GetBondedDevicesCount(&N); the application
* should listen to the gPrivateResolvableAddressVerified_c event.
*
********************************************************************************** */
bleResult_t Gap_VerifyPrivateResolvableAddress
(
    uint8_t                   nvmIndex,
    const bleDeviceAddress_t  aAddress
);

/*! *********************************************************************************
* \brief  Sets a random address into the Controller.
*
* \param[in] aAddress  The Private Resolvable, Private Non-Resolvable, or Static Random Address.
*
* \return  gBleSuccess_c or error.
*
* \remarks The application should listen for the gRandomAddressSet_c generic event.
*
********************************************************************************** */
bleResult_t Gap_SetRandomAddress
(
    const bleDeviceAddress_t aAddress
);

/*! *********************************************************************************
* \brief  Sets the default pairing parameters to be used by automatic pairing procedures.
*
* \param[in] pPairingParameters  Pairing parameters as required by the SMP or NULL.
*
* \return  gBleSuccess_c or error.
*
* \remarks When these parameters are set, the Security Manager automatically responds
* to a Pairing Request or a Slave Security Request using these parameters. If NULL is provided,
* it returns to the default state where all security requests are sent to the application.
*
* \remarks This function executes synchronously.
*
********************************************************************************** */
bleResult_t Gap_SetDefaultPairingParameters
(
    const gapPairingParameters_t* pPairingParameters
);

/*! *********************************************************************************
* \brief        Request a set of new connection parameters
*
* \param[in]    deviceId            The DeviceID for which the command is intended
* \param[in]    intervalMin         The minimum value for the connection event interval
* \param[in]    intervalMax         The maximum value for the connection event interval
* \param[in]    slaveLatency        The slave latency parameter
* \param[in]    timeoutMultiplier   The connection timeout parameter
* \param[in]    minCeLength         The minimum value for the connection event length
* \param[in]    maxCeLength         The maximum value for the connection event length
*
* \return       gBleSuccess_c or error.
*
* \pre          A connection must be in place
*
********************************************************************************** */
bleResult_t Gap_UpdateConnectionParameters
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
* \brief        Update the connection parameters
*
* \param[in]    deviceId            The DeviceID for which the command is intended
* \param[in]    enable              Allow/disallow the parameters update
*
* \return       Result of the operation
*
* \pre          A connection must be in place
*
* \remarks      The LE master Host may accept the requested parameters or reject the request
*
********************************************************************************** */
bleResult_t Gap_EnableUpdateConnectionParameters
(
    deviceId_t  deviceId,
    bool_t      enable
);

/*! *********************************************************************************
* \brief        Update the Tx Data Length
*
* \param[in]    deviceId            The DeviceID for which the command is intended
* \param[in]    txOctets            Maximum transmission number of payload octets
* \param[in]    txTime              Maximum transmission time
*
* \return       Result of the operation
*
* \pre          A connection must be in place
*
* \remarks      The response is contained in the gConnEvtLeDataLengthUpdated_c connection event.
*
********************************************************************************** */
bleResult_t Gap_UpdateLeDataLength
(
    deviceId_t  deviceId,
    uint16_t    txOctets,
    uint16_t    txTime
);

/*! *********************************************************************************
* \brief  Enables or disables Host Privacy (automatic regeneration of a Private Address).
*
* \param enable TRUE to enable, FALSE to disable.
* \param aIrk   Local IRK to be used for Resolvable Private Address generation
*               or NULL for Non-Resolvable Private Address generation. Ignored if enable is FALSE.
*
* \return  gBleSuccess_c or error.
*
* \remarks The application should listen for the gHostPrivacyStateChanged_c generic event.
*
********************************************************************************** */
bleResult_t Gap_EnableHostPrivacy
(
    bool_t            enable,
    const uint8_t*    aIrk
);

/*! *********************************************************************************
* \brief  Enables or disables Controller Privacy (Enhanced Privacy feature).
*
* \param enable             TRUE to enable, FALSE to disable.
* \param aOwnIrk            Local IRK. Ignored if enable is FALSE, otherwise shall not be NULL.
* \param peerIdCount        Size of aPeerIdentities array. Shall not be zero or greater than
*                           gcGapControllerResolvingListSize_c. Ignored if enable is FALSE.
* \param aPeerIdentities    Array of peer identity addresses and IRKs. Ignored if enable is FALSE,
*                           otherwise shall not be NULL.
*
* \return  gBleSuccess_c or error.
*
* \remarks The application should listen for the gControllerPrivacyStateChanged_c generic event.
*
********************************************************************************** */
bleResult_t Gap_EnableControllerPrivacy
(
    bool_t                            enable,
    const uint8_t*                    aOwnIrk,
    uint8_t                           peerIdCount,
    const gapIdentityInformation_t*   aPeerIdentities
);

/*! *********************************************************************************
* \brief  Sets the privacy mode to an existing bond.
*
* \param[in] nvmIndex       Index of the device in the NVM bonding area.
* \param[in] privacyMode    Controller privacy mode: Network or Device
*
* \return  gBleSuccess_c or error.
*
* \remarks The change has no effect (other than the change in NVM) unless controller
*          privacy is enabled for the bonded identities.
*
********************************************************************************** */
bleResult_t Gap_SetPrivacyMode
(
    uint8_t             nvmIndex,
    blePrivacyMode_t    privacyMode
);

/*! *********************************************************************************
* \brief  Commands a Controller Test procedure.
*
* \param testCmd            Command type - "start TX test", "start RX test" or "end test".
* \param radioChannel       Radio channel index. Valid range: 0-39.
*                           Frequency will be F[MHz] = 2402 + 2 * index. Effective range: 2402-2480 MHz.
*                           Ignored if command is "end test".
* \param txDataLength       Size of packet payload for TX tests.
*                           Ignored if command is "start RX test" or "end test".
* \param txPayloadType      Type of packet payload for TX tests.
*                           Ignored if command is "start RX test" or "end test".
*
* \return  gBleSuccess_c or error.
*
* \remarks The application should listen for the gControllerTestEvent_c generic event.
* \remarks This API function is available only in the full-featured host library.
*
********************************************************************************** */
bleResult_t Gap_ControllerTest
(
    gapControllerTestCmd_t      testCmd,
    uint8_t                     radioChannel,
    uint8_t                     txDataLength,
    gapControllerTestTxType_t   txPayloadType
);

/*! *********************************************************************************
* \brief  Read the Tx and Rx Phy on the connection with a device
*
* \param deviceId  Device ID of the peer.
*
* \return  gBleSuccess_c or error.
*
* \remarks  The application should listen for the gLePhyEvent_c generic event.
*           This API is available only in the Bluetooth 5.0 Host Stack.
*
********************************************************************************** */
bleResult_t Gap_LeReadPhy
(
    deviceId_t  deviceId
);

/*! *********************************************************************************
* \brief  Set the Tx and Rx Phy preferences on the connection with a device or all subsequent connections
*
* \param defaultMode   Use the provided values for all subsequent connections
* \param deviceId      Device ID of the peer
*                      Ignored if defaultMode is TRUE.
* \param allPhys       Host preferences on Tx and Rx Phy, as defined by gapLeAllPhyFlags_t
* \param txPhys        Host preferences on Tx Phy, as defined by gapLePhyFlags_t, ignored for gLeTxPhyNoPreference_c
* \param rxPhys        Host preferences on Rx Phy, as defined by gapLePhyFlags_t, ignored for gLeRxPhyNoPreference_c
* \param phyOptions    Host preferences on Coded Phy, as defined by gapLePhyOptionsFlags_t
*                      Ignored if defaultMode is TRUE.
*
* \return  gBleSuccess_c or error.
*
* \remarks  The application should listen for the gLePhyEvent_c generic event.
*           This API is available only in the Bluetooth 5.0 Host Stack.
*
********************************************************************************** */
bleResult_t Gap_LeSetPhy
(
    bool_t      defaultMode,
    deviceId_t  deviceId,
    uint8_t     allPhys,
    uint8_t     txPhys,
    uint8_t     rxPhys,
    uint16_t    phyOptions
);

/*! *********************************************************************************
* \brief  Configure enhanced notifications on advertising, scannning and connection events
*         on the controller.
*
* \param[in]  eventType       Event type selection as specified by bleNotificationEventType_t.
* \param[in]  deviceId        Device ID of the peer, used only for connection events.
*
* \return  gBleSuccess_c or error.
*
* \remarks The application should listen for the gControllerNotificationEvent_c generic event.
* \remarks This function executes synchronously.
*
********************************************************************************** */
bleResult_t Gap_ControllerEnhancedNotification
(
    uint16_t    eventType,
    deviceId_t  deviceId
);

/*! *********************************************************************************
* \brief  Retrieves the keys from an existing bond with a device.
*
* \param[in]  nvmIndex           Index of the device in the NVM bonding area.
* \param[out] pOutKeys           Pointer to fill the keys distributed during pairing.
* \param[out] pOutKeyFlags       Pointer to indicate which keys were distributed during pairing.
* \param[out] pOutLeSc           Pointer to mark if LE Secure Connections was used during pairing.
* \param[out] pOutAuth           Pointer to mark if the device was authenticated for MITM during pairing.
*
* \return  gBleSuccess_c or error.
*
* \remarks This API requires that the aAddress in the pOutKeys shall not be NULL.
* \remarks The application will check pOutKeyFlags to see which information is valid in pOutKeys.
* \remarks This function executes synchronously.
*
********************************************************************************** */
bleResult_t Gap_LoadKeys
(
    uint8_t           nvmIndex,
    gapSmpKeys_t*     pOutKeys,
    gapSmpKeyFlags_t* pOutKeyFlags,
    bool_t*           pOutLeSc,
    bool_t*           pOutAuth
);

/*! *********************************************************************************
* \brief  Saves the keys to a new or existing bond based on OOB information.
*
* \param[in] nvmIndex        Index of the device in the NVM bonding area.
* \param[in] pKeys           Pointer to the keys distributed during pairing.
* \param[in] leSc            Indicates if LE Secure Connections was used during pairing.
* \param[in] auth            Indicates if the device was authenticated for MITM during pairing.
*
* \return  gBleSuccess_c or error.
*
* \remarks This API requires that the aAddress in the pKeys shall not be NULL.
* \remarks If any of the keys are passed as NULL, they will not be saved.
* \remarks The application listen for gBondCreatedEvent_c to confirm the bond was created.
*
********************************************************************************** */
bleResult_t Gap_SaveKeys
(
    uint8_t             nvmIndex,
    const gapSmpKeys_t  *pKeys,
    bool_t              leSc,
    bool_t              auth
);

/*! *********************************************************************************
* \brief  Set the channel map in the Controller and trigger a LL channel map update.
*
* \param[in]  channelMap      Array with the channels using 0 for bad channels and 1 for unknown.
*
* \return  gBleSuccess_c or error.
*
* \remarks The application should listen for the gChannelMapSet_c generic event.
* \remarks This function executes synchronously.
*
* \remarks GAP Central-only API function.
*
********************************************************************************** */
bleResult_t Gap_SetChannelMap
(
    const bleChannelMap_t channelMap
);

/*! *********************************************************************************
* \brief  Reads the channel map of a connection.
*
* \param[in]  deviceId        Device ID of the peer.
*
* \return  gBleSuccess_c or error.
*
* \remarks The application should listen for the gConnEvtChannelMapRead_c connection event.
* \remarks This function executes synchronously.
*
********************************************************************************** */
bleResult_t Gap_ReadChannelMap
(
    deviceId_t  deviceId
);

/*! *********************************************************************************
* \brief  Sets up the Extended Advertising Parameters.
*
* \param[in] pAdvertisingParameters   Pointer to gapExtAdvertisingParameters_t structure.
*
* \return  gBleSuccess_c or error.
*
* \remarks GAP Peripheral-only API function.
*
********************************************************************************** */
bleResult_t Gap_SetExtAdvertisingParameters
(
    gapExtAdvertisingParameters_t*   pAdvertisingParameters
);

/*! *********************************************************************************
* \brief  Sets up the Extended Advertising and Extended Scan Response Data.
*
* \param[in] handle             The ID of the advertising set
* \param[in] pAdvertisingData   Pointer to gapAdvertisingData_t structure or NULL.
* \param[in] pScanResponseData  Pointer to gapScanResponseData_t structure or NULL.
*
* \return  gBleSuccess_c or error.
*
* \remarks Any of the parameters may be NULL, in which case they are ignored.
* Therefore, this function can be used to set any of the parameters individually or both at once.
*
* \remarks GAP Peripheral-only API function.
*
********************************************************************************** */
bleResult_t Gap_SetExtAdvertisingData
(
    uint8_t handle,
    gapAdvertisingData_t*  pAdvertisingData,
    gapScanResponseData_t* pScanResponseData
);

/*! *********************************************************************************
* \brief  Commands the controller to start the extended advertising.
*
* \param[in] advertisingCallback   Callback used by the application to receive advertising events.
*                                  Can be NULL.
* \param[in] connectionCallback    Callback used by the application to receive connection events.
*                                  Can be NULL.
*
* \param[in] handle                The ID of the advertising set
* \param[in] duration              The duration of the advertising
* \param[in] maxExtAdvEvents       The maximum number of advertising events
*
* \return  gBleSuccess_c or error.
*
* \remarks The advertisingCallback confirms or denies whether the advertising has started.
* The connectionCallback is only used if a connection gets established during advertising.
*
* \remarks GAP Peripheral-only API function.
*
********************************************************************************** */
bleResult_t Gap_StartExtAdvertising
(
    gapAdvertisingCallback_t    advertisingCallback,
    gapConnectionCallback_t     connectionCallback,
    uint8_t                     handle,
    uint16_t                    duration,
    uint8_t                     maxExtAdvEvents
);

/*! *********************************************************************************
* \brief  Commands the controller to stop extended advertising for set ID.
*
* \param[in] handle                   The ID of the advertising set
*
* \return  gBleSuccess_c or error.
*
* \remarks GAP Peripheral-only API function.
*
********************************************************************************** */
bleResult_t Gap_StopExtAdvertising
(
    uint8_t handle
);

/*! *********************************************************************************
* \brief  Commands the controller to remove the specified advertising set
*         and all it's data.
*
* \param[in] handle   The ID of the advertising set
*
* \return  gBleSuccess_c or error.
*
* \remarks GAP Peripheral-only API function.
*
********************************************************************************** */
bleResult_t Gap_RemoveAdvSet
(
    uint8_t handle
);

/*! *********************************************************************************
* \brief  Sets up the Periodic Advertising Parameters.
*
* \param[in] pAdvertisingParameters   Pointer to gapPeriodicAdvParameters_t structure.
*
* \return  gBleSuccess_c or error.
*
* \remarks GAP Peripheral-only API function.
*
********************************************************************************** */
bleResult_t Gap_SetPeriodicAdvParameters
(
    gapPeriodicAdvParameters_t*   pAdvertisingParameters
);

/*! *********************************************************************************
* \brief  Sets up the Periodic Advertising Data.
*
* \param[in] handle             The ID of the periodic advertising set
* \param[in] pAdvertisingData   Pointer to gapAdvertisingData_t structure.
*
* \return  gBleSuccess_c or error.
*
* \remarks GAP Peripheral-only API function.
*
********************************************************************************** */
bleResult_t Gap_SetPeriodicAdvertisingData
(
    uint8_t handle,
    gapAdvertisingData_t*  pAdvertisingData
);

/*! *********************************************************************************
* \brief  Commands the controller to start periodic advertising for set ID.
*
* \param[in] handle                   The ID of the periodic advertising set
*
* \return  gBleSuccess_c or error.
*
* \remarks GAP Peripheral-only API function.
*
********************************************************************************** */
bleResult_t Gap_StartPeriodicAdvertising
(
    uint8_t handle
);

/*! *********************************************************************************
* \brief  Commands the controller to stop periodic advertising for set ID.
*
* \param[in] handle                   The ID of the periodic advertising set
*
* \return  gBleSuccess_c or error.
*
* \remarks GAP Peripheral-only API function.
*
********************************************************************************** */
bleResult_t Gap_StopPeriodicAdvertising
(
    uint8_t handle
);

/*! *********************************************************************************
* \brief  Manage the periodic advertising list.
*
* \param[in] operation  The list operation: add/remove a device, or clear all.
* \param[in] addrType   The address type of the periodic advertiser.
* \param[in] pAddr      Pointer to the advertiser's address.
* \param[in] SID        The ID of the advertising set.
*
* \return  gBleSuccess_c or error.
*
* \remarks GAP Central-only API function.
*
********************************************************************************** */
bleResult_t Gap_UpdatePeriodicAdvList
(
    gapPeriodicAdvListOperation_t operation,
    bleAddressType_t addrType,
    uint8_t * pAddr,
    uint8_t SID
);

/*! *********************************************************************************
* \brief  Start tracking periodic advertisings.
*
* \param[in] pReq              Pointer to the Sync Request parameters.
* \param[in] scanningCallback  Callback function.
*
* \return  gBleSuccess_c or error.
*
* \remarks GAP Central-only API function.
*
********************************************************************************** */
bleResult_t Gap_PeriodicAdvCreateSync
(
    gapPeriodicAdvSyncReq_t * pReq,
    gapScanningCallback_t     scanningCallback
);

/*! *********************************************************************************
* \brief  Stop tracking periodic advertisings.
*
* \param[in] syncHandle        Used to identify the periodic advertiser
*
* \return  gBleSuccess_c or error.
*
* \remarks GAP Central-only API function.
*
********************************************************************************** */
bleResult_t Gap_PeriodicAdvTerminateSync
(
    uint16_t syncHandle
);

/*! *********************************************************************************
* \brief Resume the pairing process. At this point the ecdh key must be computed.
*        This function should be called only for secured LE connections. In any other
*        cases the user should make his own code for handling the case when the ECDH
*        computation is completed.
*
* \param[in] pData Pointer to the data used to resume the host state machine. The
*                  data is allocated by the stack when it requested an ECDH
*                  multiplication. It is also freed by the stack at the end of the
*                  multiplication.
*
* \return  status of the procedure.
*
********************************************************************************** */
bleResult_t Gap_ResumeLeScStateMachine
(
    computeDhKeyParam_t *pData
);

#ifdef __cplusplus
}
#endif

#endif /* GAP_INTERFACE_H */

/*! *********************************************************************************
* @}
********************************************************************************** */
