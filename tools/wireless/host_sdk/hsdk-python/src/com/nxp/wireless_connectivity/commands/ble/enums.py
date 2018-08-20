'''
* The Clear BSD License
* Copyright 2014-2015 Freescale Semiconductor, Inc.
* Copyright 2016-2018 NXP
* All rights reserved.
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
'''

from com.nxp.wireless_connectivity.hsdk.config import BLE_PTS_TEST

class GenericEnum(object):

    @classmethod
    def getEnumString(cls, value):
        if BLE_PTS_TEST:
            return value

        for k, v in cls.__dict__.items():
            if v == value:
                return k
        return value


class L2CAPCancelConnectionRequestRefuseReason(GenericEnum):

    gSuccessful_c = 0x0000
    gLePsmNotSupported_c = 0x0002
    gNoResourcesAvailable_c = 0x0004
    gInsufficientAuthentication_c = 0x0005
    gInsufficientAuthorization_c = 0x0006
    gInsufficientEncryptionKeySize_c = 0x0007
    gInsufficientEncryption_c = 0x0008
    gInvalidSourceCid_c = 0x0009
    gSourceCidAlreadyAllocated_c = 0x000A
    gCommandRejected_c = 0x0100
    gResponseTimeout_c = 0xFFFE


class SMInitiatorStartPairingRequestSmPairingParams_IoCapabilities(GenericEnum):

    gDisplayOnly_c = 0x00
    gDisplayYesNo_c = 0x01
    gKeyboardOnly_c = 0x02
    gNoInputNoOutput_c = 0x03
    gKeyboardDisplay_c = 0x04


class SMInitiatorStartPairingRequestSmPairingParams_OobDataFlag(GenericEnum):

    gAuthDataNotPresent_c = 0x00
    gAuthDataPresent_c = 0x01


class SmPairingParams_AuthRequestBondingFlags(GenericEnum):

    gNoBonding_c = 0x00
    gBonding_c = 0x01
    gReserved10_c = 0x02
    gReserved11_c = 0x03


class SmPairingParams_AuthRequestMitm(GenericEnum):

    gReqOff_c = 0x00
    gReqOn_c = 0x01


class SmPairingParams_AuthRequestsc(GenericEnum):

    gReqOff_c = 0x00
    gReqOn_c = 0x01


class SmPairingParams_AuthRequestkeypress(GenericEnum):

    gReqOff_c = 0x00
    gReqOn_c = 0x01


class SmSecurityReqParams_AuthRequestBondingFlags(GenericEnum):

    gNoBonding_c = 0x00
    gBonding_c = 0x01
    gReserved10_c = 0x02
    gReserved11_c = 0x03


class SmSecurityReqParams_AuthRequestMitm(GenericEnum):

    gReqOff_c = 0x00
    gReqOn_c = 0x01


class SmSecurityReqParams_AuthRequestsc(GenericEnum):

    gReqOff_c = 0x00
    gReqOn_c = 0x01


class SmSecurityReqParams_AuthRequestkeypress(GenericEnum):

    gReqOff_c = 0x00
    gReqOn_c = 0x01


class SMTerminatePairingRequestSmTerminatePairingParams_Reason(GenericEnum):

    gReserved_c = 0x00
    gPasskeyEntryFailed_c = 0x01
    # gOobNotAvailable_c
    gOobNotAvailable_c = 0x02
    gAuthenticationRequirements_c = 0x03
    gConfirmValueFailed_c = 0x04
    gPairingNotSupported_c = 0x05
    gEncryptionKeySize_c = 0x06
    gCommandNotSupported_c = 0x07
    gUnspecifiedReason_c = 0x08
    gRepeatedAttempts_c = 0x09
    gInvalidParameters_c = 0x0A


class SMPairingRequestReplyRequestSmPairingParams_IoCapabilities(GenericEnum):

    gDisplayOnly_c = 0x00
    gDisplayYesNo_c = 0x01
    gKeyboardOnly_c = 0x02
    gNoInputNoOutput_c = 0x03
    gKeyboardDisplay_c = 0x04


class SMPairingRequestReplyRequestSmPairingParams_OobDataFlag(GenericEnum):

    gAuthDataNotPresent_c = 0x00
    gAuthDataPresent_c = 0x01


class SMPasskeyRequestReplyRequestSmPasskeyReqReplyParams_KeyType(GenericEnum):

    # Denotes a Passkey: 0 to 999.999 decimal
    Passkey = 0x00
    # Denotes an OOB obtained key: 16 bytes long
    Oob = 0x01


class PairingKeysetRequestReplyParams_KeyDistPayloadBDAddrType(GenericEnum):

    gPublic_c = 0x00
    gRandom_c = 0x01


class SMTbCreateRandomDeviceAddressRequestRequestTbCreateRandomDeviceAddrReqParams_AddrType(GenericEnum):

    RandAddrTypeNone = 0x00
    StaticAddr = 0x01
    NonResolvablePrivateAddr = 0x02
    ResolvablePrivateAddr = 0x03


class SMSetOobMitmProtectionRequestoobMitmProt(GenericEnum):

    smOobMitmProtectionOff_c = 0x00
    smOobMitmProtectionOn_c = 0x01


class SMSendKeypressNotificationRequestSmKeypressNotificationParams_keypressNotifType(GenericEnum):

    gSmpKeypressNotificationPasskeyEntryStarted_c = 0x00
    gSmpKeypressNotificationPasskeyDigitEntered_c = 0x01
    # gSmpKeypressNotificationPasskeyDigitErased_c
    gSmpKeypressNotificationPasskeyDigitErased_c = 0x02
    gSmpKeypressNotificationPasskeyCleared_c = 0x03
    gSmpKeypressNotificationPasskeyEntryCompleted_c = 0x04


class ATTRegisterOpcodeCallbackRequestOpcode(GenericEnum):

    gErrorResponse_c = 0x01
    gExchangeMtuRequest_c = 0x02
    gExchangeMtuResponse_c = 0x03
    gFindInformationRequest_c = 0x04
    gFindInformationResponse_c = 0x05
    gFindByTypeValueRequest_c = 0x06
    gFindByTypeValueResponse_c = 0x07
    gReadByTypeRequest_c = 0x08
    gReadByTypeResponse_c = 0x09
    gReadRequest_c = 0x0A
    gReadResponse_c = 0x0B
    gReadBlobRequest_c = 0x0C
    gReadBlobResponse_c = 0x0D
    gReadMultipleRequest_c = 0x0E
    gReadMultipleResponse_c = 0x0F
    gReadByGroupTypeRequest_c = 0x10
    gReadByGroupTypeResponse_c = 0x11
    gWriteRequest_c = 0x12
    gWriteResponse_c = 0x13
    gWriteCommand_c = 0x52
    gSignedWriteCommand_c = 0xD2
    gPrepareWriteRequest_c = 0x16
    gPrepareWriteResponse_c = 0x17
    gExecuteWriteRequest_c = 0x18
    gExecuteWriteResponse_c = 0x19
    gHandleValueNotification_c = 0x1B
    gHandleValueIndication_c = 0x1D
    gHandleValueConfirmation_c = 0x1E
    gLastOpcode_c = 0xE0


class ATTServerSendErrorResponseRequestParams_RequestOpcodeInError(GenericEnum):

    gErrorResponse_c = 0x01
    gExchangeMtuRequest_c = 0x02
    gExchangeMtuResponse_c = 0x03
    gFindInformationRequest_c = 0x04
    gFindInformationResponse_c = 0x05
    gFindByTypeValueRequest_c = 0x06
    gFindByTypeValueResponse_c = 0x07
    gReadByTypeRequest_c = 0x08
    gReadByTypeResponse_c = 0x09
    gReadRequest_c = 0x0A
    gReadResponse_c = 0x0B
    gReadBlobRequest_c = 0x0C
    gReadBlobResponse_c = 0x0D
    gReadMultipleRequest_c = 0x0E
    gReadMultipleResponse_c = 0x0F
    gReadByGroupTypeRequest_c = 0x10
    gReadByGroupTypeResponse_c = 0x11
    gWriteRequest_c = 0x12
    gWriteResponse_c = 0x13
    gWriteCommand_c = 0x52
    gSignedWriteCommand_c = 0xD2
    gPrepareWriteRequest_c = 0x16
    gPrepareWriteResponse_c = 0x17
    gExecuteWriteRequest_c = 0x18
    gExecuteWriteResponse_c = 0x19
    gHandleValueNotification_c = 0x1B
    gHandleValueIndication_c = 0x1D
    gHandleValueConfirmation_c = 0x1E
    #  gAttLastOpcode_c
    gAttLastOpcode_c = 0xE0


class ATTServerSendErrorResponseRequestParams_ErrorCode(GenericEnum):

    gInvalidHandle_c = 0x01
    gReadNotPermitted_c = 0x02
    gWriteNotPermitted_c = 0x03
    gInvalidPdu_c = 0x04
    gInsufficientAuthentication_c = 0x05
    gRequestNotSupported_c = 0x06
    gInvalidOffset_c = 0x07
    gInsufficientAuthorization_c = 0x08
    gPrepareQueueFull_c = 0x09
    gAttributeNotFound_c = 0x0A
    gAttributeNotLong_c = 0x0B
    gInsufficientEncryptionKeySize_c = 0x0C
    gInvalidAttributeValueLength_c = 0x0D
    gUnlikelyError_c = 0x0E
    gInsufficientEncryption_c = 0x0F
    gUnsupportedGroupType_c = 0x10
    gInsufficientResources_c = 0x11


class ATTServerSendFindInformationResponseRequestParams_Format(GenericEnum):

    # Uuid 16 bits format
    Uuid16BitFormat = 0x01
    # Uuid 128 bits format
    Uuid128BitFormat = 0x02


class ATTClientSendReadByTypeRequestRequestParams_Format(GenericEnum):

    # Uuid 16 bits format
    Uuid16BitFormat = 0x01
    # Uuid 128 bits format
    Uuid128BitFormat = 0x02


class ATTClientSendReadByGroupTypeRequestRequestParams_Format(GenericEnum):

    # Uuid 16 bits format
    Uuid16BitFormat = 0x01
    # Uuid 128 bits format
    Uuid128BitFormat = 0x02


class ATTClientSendExecuteWriteRequestRequestParams_Flags(GenericEnum):

    gCancel_c = 0x00
    gImmediately_c = 0x01


class GATTClientDiscoverPrimaryServicesByUuidRequestUuidType(GenericEnum):

    Uuid16Bits = 0x01
    Uuid128Bits = 0x02
    Uuid32Bits = 0x03


class GATTClientFindIncludedServicesRequestService_UuidType(GenericEnum):

    Uuid16Bits = 0x01
    Uuid128Bits = 0x02
    Uuid32Bits = 0x03


class Service_CharacteristicsProperties(GenericEnum):

    # No Properties selected
    gNone_c = 0x00
    # Characteristic can be broadcast
    gBroadcast_c = 0x01
    # Characteristic can be read
    gRead_c = 0x02
    # Characteristic can be written without response
    gWriteWithoutRsp_c = 0x04
    # Characteristic can be written with response
    gWrite_c = 0x08
    # Characteristic can be notified
    gNotify_c = 0x10
    # Characteristic can be indicated
    gIndicate_c = 0x20
    # Characteristic can be written with signed data
    gAuthSignedWrites_c = 0x40
    # Extended Characteristic properties
    gExtendedProperties_c = 0x80


class ValueUuidType(GenericEnum):

    Uuid16Bits = 0x01
    Uuid128Bits = 0x02
    Uuid32Bits = 0x03


class DescriptorsUuidType(GenericEnum):

    Uuid16Bits = 0x01
    Uuid128Bits = 0x02
    Uuid32Bits = 0x03


class Service_IncludedServicesUuidType(GenericEnum):

    Uuid16Bits = 0x01
    Uuid128Bits = 0x02
    Uuid32Bits = 0x03


class GATTClientDiscoverAllCharacteristicsOfServiceRequestService_UuidType(GenericEnum):

    Uuid16Bits = 0x01
    Uuid128Bits = 0x02
    Uuid32Bits = 0x03


class GATTClientDiscoverCharacteristicOfServiceByUuidRequestUuidType(GenericEnum):

    Uuid16Bits = 0x01
    Uuid128Bits = 0x02
    Uuid32Bits = 0x03


class GATTClientDiscoverCharacteristicOfServiceByUuidRequestService_UuidType(GenericEnum):

    Uuid16Bits = 0x01
    Uuid128Bits = 0x02
    Uuid32Bits = 0x03


class GATTClientDiscoverAllCharacteristicDescriptorsRequestCharacteristic_Properties(GenericEnum):

    # No Properties selected
    gNone_c = 0x00
    # Characteristic can be broadcast
    gBroadcast_c = 0x01
    # Characteristic can be read
    gRead_c = 0x02
    # Characteristic can be written without response
    gWriteWithoutRsp_c = 0x04
    # Characteristic can be written with response
    gWrite_c = 0x08
    # Characteristic can be notified
    gNotify_c = 0x10
    # Characteristic can be indicated
    gIndicate_c = 0x20
    # Characteristic can be written with signed data
    gAuthSignedWrites_c = 0x40
    # Extended Characteristic properties
    gExtendedProperties_c = 0x80


class Characteristic_ValueUuidType(GenericEnum):

    Uuid16Bits = 0x01
    Uuid128Bits = 0x02
    Uuid32Bits = 0x03


class Characteristic_DescriptorsUuidType(GenericEnum):

    Uuid16Bits = 0x01
    Uuid128Bits = 0x02
    Uuid32Bits = 0x03


class GATTClientReadCharacteristicValueRequestCharacteristic_Properties(GenericEnum):

    # No Properties selected
    gNone_c = 0x00
    # Characteristic can be broadcast
    gBroadcast_c = 0x01
    # Characteristic can be read
    gRead_c = 0x02
    # Characteristic can be written without response
    gWriteWithoutRsp_c = 0x04
    # Characteristic can be written with response
    gWrite_c = 0x08
    # Characteristic can be notified
    gNotify_c = 0x10
    # Characteristic can be indicated
    gIndicate_c = 0x20
    # Characteristic can be written with signed data
    gAuthSignedWrites_c = 0x40
    # Extended Characteristic properties
    gExtendedProperties_c = 0x80


class GATTClientReadUsingCharacteristicUuidRequestUuidType(GenericEnum):

    Uuid16Bits = 0x01
    Uuid128Bits = 0x02
    Uuid32Bits = 0x03


class CharacteristicsProperties(GenericEnum):

    # No Properties selected
    gNone_c = 0x00
    # Characteristic can be broadcast
    gBroadcast_c = 0x01
    # Characteristic can be read
    gRead_c = 0x02
    # Characteristic can be written without response
    gWriteWithoutRsp_c = 0x04
    # Characteristic can be written with response
    gWrite_c = 0x08
    # Characteristic can be notified
    gNotify_c = 0x10
    # Characteristic can be indicated
    gIndicate_c = 0x20
    # Characteristic can be written with signed data
    gAuthSignedWrites_c = 0x40
    # Extended Characteristic properties
    gExtendedProperties_c = 0x80


class GATTClientWriteCharacteristicValueRequestCharacteristic_Properties(GenericEnum):

    # No Properties selected
    gNone_c = 0x00
    # Characteristic can be broadcast
    gBroadcast_c = 0x01
    # Characteristic can be read
    gRead_c = 0x02
    # Characteristic can be written without response
    gWriteWithoutRsp_c = 0x04
    # Characteristic can be written with response
    gWrite_c = 0x08
    # Characteristic can be notified
    gNotify_c = 0x10
    # Characteristic can be indicated
    gIndicate_c = 0x20
    # Characteristic can be written with signed data
    gAuthSignedWrites_c = 0x40
    # Extended Characteristic properties
    gExtendedProperties_c = 0x80


class GATTClientReadCharacteristicDescriptorRequestDescriptor_UuidType(GenericEnum):

    Uuid16Bits = 0x01
    Uuid128Bits = 0x02
    Uuid32Bits = 0x03


class GATTClientWriteCharacteristicDescriptorRequestDescriptor_UuidType(GenericEnum):

    Uuid16Bits = 0x01
    Uuid128Bits = 0x02
    Uuid32Bits = 0x03


class GATTDBFindServiceHandleRequestUuidType(GenericEnum):

    Uuid16Bits = 0x01
    Uuid128Bits = 0x02
    Uuid32Bits = 0x03


class GATTDBFindCharValueHandleInServiceRequestUuidType(GenericEnum):

    Uuid16Bits = 0x01
    Uuid128Bits = 0x02
    Uuid32Bits = 0x03


class GATTDBFindDescriptorHandleForCharValueHandleRequestUuidType(GenericEnum):

    Uuid16Bits = 0x01
    Uuid128Bits = 0x02
    Uuid32Bits = 0x03


class GATTDBDynamicAddPrimaryServiceDeclarationRequestUuidType(GenericEnum):

    Uuid16Bits = 0x01
    Uuid128Bits = 0x02
    Uuid32Bits = 0x03


class GATTDBDynamicAddSecondaryServiceDeclarationRequestUuidType(GenericEnum):

    Uuid16Bits = 0x01
    Uuid128Bits = 0x02
    Uuid32Bits = 0x03


class GATTDBDynamicAddIncludeDeclarationRequestUuidType(GenericEnum):

    Uuid16Bits = 0x01
    Uuid128Bits = 0x02
    Uuid32Bits = 0x03


class GATTDBDynamicAddCharacteristicDeclarationAndValueRequestUuidType(GenericEnum):

    Uuid16Bits = 0x01
    Uuid128Bits = 0x02
    Uuid32Bits = 0x03


class GATTDBDynamicAddCharacteristicDeclarationAndValueRequestCharacteristicProperties(GenericEnum):

    # No Properties selected
    gNone_c = 0x00
    # Characteristic can be broadcast
    gBroadcast_c = 0x01
    # Characteristic can be read
    gRead_c = 0x02
    # Characteristic can be written without response
    gWriteWithoutRsp_c = 0x04
    # Characteristic can be written with response
    gWrite_c = 0x08
    # Characteristic can be notified
    gNotify_c = 0x10
    # Characteristic can be indicated
    gIndicate_c = 0x20
    # Characteristic can be written with signed data
    gAuthSignedWrites_c = 0x40
    # Extended Characteristic properties
    gExtendedProperties_c = 0x80


class GATTDBDynamicAddCharacteristicDeclarationAndValueRequestValueAccessPermissions(GenericEnum):

    # No permissions selected
    gPermissionNone_c = 0x00
    # Attribute can be read
    gPermissionFlagReadable_c = 0x01
    # Attribute may be read only if link is encrypted
    gPermissionFlagReadWithEncryption_c = 0x02
    # Attribute may be read only by authenticated peers
    gPermissionFlagReadWithAuthentication_c = 0x04
    # Attribute may be read only by authorized peers
    gPermissionFlagReadWithAuthorization_c = 0x08
    # Attribute can be written
    gPermissionFlagWritable_c = 0x10
    # Attribute may be written only if link is encrypted
    gPermissionFlagWriteWithEncryption_c = 0x20
    # Attribute may be written only by authenticated peers
    gPermissionFlagWriteWithAuthentication_c = 0x40
    # Attribute may be written only by authorized peers
    gPermissionFlagWriteWithAuthorization_c = 0x80


class GATTDBDynamicAddCharacteristicDescriptorRequestUuidType(GenericEnum):

    Uuid16Bits = 0x01
    Uuid128Bits = 0x02
    Uuid32Bits = 0x03


class GATTDBDynamicAddCharacteristicDescriptorRequestDescriptorAccessPermissions(GenericEnum):

    # No permissions selected
    gPermissionNone_c = 0x00
    # Attribute can be read
    gPermissionFlagReadable_c = 0x01
    # Attribute may be read only if link is encrypted
    gPermissionFlagReadWithEncryption_c = 0x02
    # Attribute may be read only by authenticated peers
    gPermissionFlagReadWithAuthentication_c = 0x04
    # Attribute may be read only by authorized peers
    gPermissionFlagReadWithAuthorization_c = 0x08
    # Attribute can be written
    gPermissionFlagWritable_c = 0x10
    # Attribute may be written only if link is encrypted
    gPermissionFlagWriteWithEncryption_c = 0x20
    # Attribute may be written only by authenticated peers
    gPermissionFlagWriteWithAuthentication_c = 0x40
    # Attribute may be written only by authorized peers
    gPermissionFlagWriteWithAuthorization_c = 0x80


class GATTDBDynamicAddCharacteristicDeclarationWithUniqueValueRequestUuidType(GenericEnum):

    Uuid16Bits = 0x01
    Uuid128Bits = 0x02
    Uuid32Bits = 0x03


class GATTDBDynamicAddCharacteristicDeclarationWithUniqueValueRequestCharacteristicProperties(GenericEnum):

    # No Properties selected
    gNone_c = 0x00
    # Characteristic can be broadcast
    gBroadcast_c = 0x01
    # Characteristic can be read
    gRead_c = 0x02
    # Characteristic can be written without response
    gWriteWithoutRsp_c = 0x04
    # Characteristic can be written with response
    gWrite_c = 0x08
    # Characteristic can be notified
    gNotify_c = 0x10
    # Characteristic can be indicated
    gIndicate_c = 0x20
    # Characteristic can be written with signed data
    gAuthSignedWrites_c = 0x40
    # Extended Characteristic properties
    gExtendedProperties_c = 0x80


class GATTDBDynamicAddCharacteristicDeclarationWithUniqueValueRequestValueAccessPermissions(GenericEnum):

    # No permissions selected
    gPermissionNone_c = 0x00
    # Attribute can be read
    gPermissionFlagReadable_c = 0x01
    # Attribute may be read only if link is encrypted
    gPermissionFlagReadWithEncryption_c = 0x02
    # Attribute may be read only by authenticated peers
    gPermissionFlagReadWithAuthentication_c = 0x04
    # Attribute may be read only by authorized peers
    gPermissionFlagReadWithAuthorization_c = 0x08
    # Attribute can be written
    gPermissionFlagWritable_c = 0x10
    # Attribute may be written only if link is encrypted
    gPermissionFlagWriteWithEncryption_c = 0x20
    # Attribute may be written only by authenticated peers
    gPermissionFlagWriteWithAuthentication_c = 0x40
    # Attribute may be written only by authorized peers
    gPermissionFlagWriteWithAuthorization_c = 0x80


class GATTDBAttReadByTypeRequestParams_Format(GenericEnum):

    # Uuid 16 bits format
    Uuid16BitFormat = 0x01
    # Uuid 128 bits format
    Uuid128BitFormat = 0x02


class GATTDBAttReadByGroupTypeRequestParams_Format(GenericEnum):

    # Uuid 16 bits format
    Uuid16BitFormat = 0x01
    # Uuid 128 bits format
    Uuid128BitFormat = 0x02


class GATTDBAttExecuteWriteRequestParams_Flags(GenericEnum):

    gCancel_c = 0x00
    gImmediately_c = 0x01


class MasterSecurityRequirementsSecurityModeLevel(GenericEnum):

    # Mode 1 Level 1 - No Security
    gMode1Level1_c = 0x10
    # Mode 1 Level 2 - Encryption without authentication
    gMode1Level2_c = 0x11
    # Mode 1 Level 3 - Encryption with authentication
    gMode1Level3_c = 0x12
    # Mode 1 Level 4 - Encryption with LE Secure Connections pairing (BLE 4.2 only)
    gMode1Level4_c = 0x13
    # Mode 2 Level 1 - Data Signing without authentication
    gMode2Level1_c = 0x21
    # Mode 2 Level 2 - Data Signing with authentication
    gMode2Level2_c = 0x22


class RequirementsSecurityModeLevel(GenericEnum):

    # Mode 1 Level 1 - No Security
    gMode1Level1_c = 0x10
    # Mode 1 Level 2 - Encryption without authentication
    gMode1Level2_c = 0x11
    # Mode 1 Level 3 - Encryption with authentication
    gMode1Level3_c = 0x12
    # Mode 1 Level 4 - Encryption with LE Secure Connections pairing (BLE 4.2 only)
    gMode1Level4_c = 0x13
    # Mode 2 Level 1 - Data Signing without authentication
    gMode2Level1_c = 0x21
    # Mode 2 Level 2 - Data Signing with authentication
    gMode2Level2_c = 0x22


class GAPSetAdvertisingParametersRequestAdvertisingParameters_AdvertisingType(GenericEnum):

    # Answers to both connect and scan requests
    gConnectableUndirected_c = 0x00
    # Answers only to connect requests; smaller advertising interval for quicker connection
    gDirectedHighDutyCycle_c = 0x01
    # Answers only to scan requests
    gScannable_c = 0x02
    # Does not answer to connect nor scan requests
    gNonConnectable_c = 0x03
    # Answers only to connect requests; larger advertising interval
    gDirectedLowDutyCycle_c = 0x04


class GAPSetAdvertisingParametersRequestAdvertisingParameters_OwnAddressType(GenericEnum):

    gPublic_c = 0x00
    gRandom_c = 0x01


class GAPSetAdvertisingParametersRequestAdvertisingParameters_PeerAddressType(GenericEnum):

    gPublic_c = 0x00
    gRandom_c = 0x01


class GAPSetAdvertisingParametersRequestAdvertisingParameters_ChannelMap(GenericEnum):

    # Bit for channel 37
    gChannel37_c = 0x01
    # Bit for channel 38
    gChannel38_c = 0x02
    # Bit for channel 39
    gChannel39_c = 0x04


class GAPSetAdvertisingParametersRequestAdvertisingParameters_FilterPolicy(GenericEnum):

    # Accept all connect and scan requests
    gProcessAll_c = 0x00
    # Accept all connect requests, but scan requests only from devices in White List
    gProcessConnAllScanWL_c = 0x01
    # Accept all scan requests, but connect requests only from devices in White List
    gProcessScanAllConnWL_c = 0x02
    # Accept connect and scan requests only from devices in White List
    gProcessWhiteListOnly_c = 0x03


class AdStructuresType(GenericEnum):

    # Defined by the Bluetooth SIG
    gAdFlags_c = 0x01
    # Defined by the Bluetooth SIG
    gAdIncomplete16bitServiceList_c = 0x02
    # Defined by the Bluetooth SIG
    gAdComplete16bitServiceList_c = 0x03
    # Defined by the Bluetooth SIG
    gAdIncomplete32bitServiceList_c = 0x04
    # Defined by the Bluetooth SIG
    gAdComplete32bitServiceList_c = 0x05
    # Defined by the Bluetooth SIG
    gAdIncomplete128bitServiceList_c = 0x06
    # Defined by the Bluetooth SIG
    gAdComplete128bitServiceList_c = 0x07
    # Defined by the Bluetooth SIG
    gAdShortenedLocalName_c = 0x08
    # Defined by the Bluetooth SIG
    gAdCompleteLocalName_c = 0x09
    # Defined by the Bluetooth SIG
    gAdTxPowerLevel_c = 0x0A
    # Defined by the Bluetooth SIG
    gAdClassOfDevice_c = 0x0D
    # Defined by the Bluetooth SIG
    gAdSimplePairingHashC192_c = 0x0E
    # Defined by the Bluetooth SIG
    gAdSimplePairingRandomizerR192_c = 0x0F
    # Defined by the Bluetooth SIG
    gAdSecurityManagerTkValue_c = 0x10
    # Defined by the Bluetooth SIG
    gAdSecurityManagerOobFlags_c = 0x11
    # Defined by the Bluetooth SIG
    gAdSlaveConnectionIntervalRange_c = 0x12
    # Defined by the Bluetooth SIG
    gAdServiceSolicitationList16bit_c = 0x14
    # Defined by the Bluetooth SIG
    gAdServiceSolicitationList128bit_c = 0x15
    # Defined by the Bluetooth SIG
    gAdServiceData16bit_c = 0x16
    # Defined by the Bluetooth SIG
    gAdPublicTargetAddress_c = 0x17
    # Defined by the Bluetooth SIG
    gAdRandomTargetAddress_c = 0x18
    # Defined by the Bluetooth SIG
    gAdAppearance_c = 0x19
    # Defined by the Bluetooth SIG
    gAdAdvertisingInterval_c = 0x1A
    # Defined by the Bluetooth SIG
    gAdLeDeviceAddress_c = 0x1B
    # Defined by the Bluetooth SIG
    gAdLeRole_c = 0x1C
    # Defined by the Bluetooth SIG
    gAdSimplePairingHashC256_c = 0x1D
    # Defined by the Bluetooth SIG
    gAdSimplePairingRandomizerR256_c = 0x1E
    # Defined by the Bluetooth SIG
    gAdServiceSolicitationList32bit_c = 0x1F
    # Defined by the Bluetooth SIG
    gAdServiceData32bit_c = 0x20
    # Defined by the Bluetooth SIG
    gAdServiceData128bit_c = 0x21
    # Defined by the Bluetooth SIG
    gAd3dInformationData_c = 0x3D
    # Defined by the Bluetooth SIG
    gAdManufacturerSpecificData_c = 0xFF


class GAPAuthorizeRequestAccess(GenericEnum):

    # Attempting to read the attribute
    gRead_c = 0x00
    # Attempting to write the attribute
    gWrite_c = 0x01


class GAPSaveCccdRequestCccd(GenericEnum):

    # Nothing is enabled
    gEmpty_c = 0x00
    # Enables notifications
    gNotification_c = 0x01
    # Enabled indications
    gIndication_c = 0x02


class GAPPairRequestPairingParameters_SecurityModeAndLevel(GenericEnum):

    # Mode 1 Level 1 - No Security
    gMode1Level1_c = 0x10
    # Mode 1 Level 2 - Encryption without authentication
    gMode1Level2_c = 0x11
    # Mode 1 Level 3 - Encryption with authentication
    gMode1Level3_c = 0x12
    # Mode 1 Level 4 - Encryption with LE Secure Connections pairing (BLE 4.2 only)
    gMode1Level4_c = 0x13
    # Mode 2 Level 1 - Data Signing without authentication
    gMode2Level1_c = 0x21
    # Mode 2 Level 2 - Data Signing with authentication
    gMode2Level2_c = 0x22


class GAPPairRequestPairingParameters_LocalIoCapabilities(GenericEnum):

    # May display a PIN, no input
    gIoDisplayOnly_c = 0x00
    # May display a PIN and has a binary input (e.g. YES and NO buttons)
    gIoDisplayYesNo_c = 0x01
    # Has keyboard input, no display
    gIoKeyboardOnly_c = 0x02
    # No input and no display
    gIoNone_c = 0x03
    # Has keyboard input and display
    gIoKeyboardDisplay_c = 0x04


class GAPPairRequestPairingParameters_CentralKeys(GenericEnum):

    # No key can be distributed
    gNoKeys_c = 0x00
    # Long Term Key
    gLtk_c = 0x01
    # Identity Resolving Key
    gIrk_c = 0x02
    # Connection Signature Resolving Key
    gCsrk_c = 0x04


class GAPPairRequestPairingParameters_PeripheralKeys(GenericEnum):

    # No key can be distributed
    gNoKeys_c = 0x00
    # Long Term Key
    gLtk_c = 0x01
    # Identity Resolving Key
    gIrk_c = 0x02
    # Connection Signature Resolving Key
    gCsrk_c = 0x04


class GAPSendSlaveSecurityRequestRequestSecurityModeLevel(GenericEnum):

    # Mode 1 Level 1 - No Security
    gMode1Level1_c = 0x10
    # Mode 1 Level 2 - Encryption without authentication
    gMode1Level2_c = 0x11
    # Mode 1 Level 3 - Encryption with authentication
    gMode1Level3_c = 0x12
    # Mode 1 Level 4 - Encryption with LE Secure Connections pairing (BLE 4.2 only)
    gMode1Level4_c = 0x13
    # Mode 2 Level 1 - Data Signing without authentication
    gMode2Level1_c = 0x21
    # Mode 2 Level 2 - Data Signing with authentication
    gMode2Level2_c = 0x22


class GAPAcceptPairingRequestRequestPairingParameters_SecurityModeAndLevel(GenericEnum):

    # Mode 1 Level 1 - No Security
    gMode1Level1_c = 0x10
    # Mode 1 Level 2 - Encryption without authentication
    gMode1Level2_c = 0x11
    # Mode 1 Level 3 - Encryption with authentication
    gMode1Level3_c = 0x12
    # Mode 1 Level 4 - Encryption with LE Secure Connections pairing (BLE 4.2 only)
    gMode1Level4_c = 0x13
    # Mode 2 Level 1 - Data Signing without authentication
    gMode2Level1_c = 0x21
    # Mode 2 Level 2 - Data Signing with authentication
    gMode2Level2_c = 0x22


class GAPAcceptPairingRequestRequestPairingParameters_LocalIoCapabilities(GenericEnum):

    # May display a PIN, no input
    gIoDisplayOnly_c = 0x00
    # May display a PIN and has a binary input (e.g. YES and NO buttons)
    gIoDisplayYesNo_c = 0x01
    # Has keyboard input, no display
    gIoKeyboardOnly_c = 0x02
    # No input and no display
    gIoNone_c = 0x03
    # Has keyboard input and display
    gIoKeyboardDisplay_c = 0x04


class GAPAcceptPairingRequestRequestPairingParameters_CentralKeys(GenericEnum):

    # No key can be distributed
    gNoKeys_c = 0x00
    # Long Term Key
    gLtk_c = 0x01
    # Identity Resolving Key
    gIrk_c = 0x02
    # Connection Signature Resolving Key
    gCsrk_c = 0x04


class GAPAcceptPairingRequestRequestPairingParameters_PeripheralKeys(GenericEnum):

    # No key can be distributed
    gNoKeys_c = 0x00
    # Long Term Key
    gLtk_c = 0x01
    # Identity Resolving Key
    gIrk_c = 0x02
    # Connection Signature Resolving Key
    gCsrk_c = 0x04


class GAPRejectPairingRequestReason(GenericEnum):

    # This device does not have the required OOB for authenticated pairing
    gOobNotAvailable_c = 0x02
    # The combination of I/O capabilities does not allow pairing with the desired level of security
    gIncompatibleIoCapabilities_c = 0x03
    # This device does not support pairing
    gPairingNotSupported_c = 0x05
    # The peer's encryption key size is too low for this device's required security level
    gLowEncryptionKeySize_c = 0x06
    # The host has rejected the pairing for an unknown reason
    gUnspecifiedReason_c = 0x08
    # This device is the target of repeated unsuccessful pairing attempts and
    # does not allow further pairing attempts at the moment
    gRepeatedAttempts_c = 0x09
    # Link could not be encrypted
    gLinkEncryptionFailed_c = 0xF0


class Keys_AddressInfoDeviceAddressType(GenericEnum):

    gPublic_c = 0x00
    gRandom_c = 0x01


class ScanningParametersType(GenericEnum):

    # Passive Scanning - advertising packets are immediately reported to the Host
    gPassive_c = 0x00
    # Active Scanning - the scanner sends scan requests to the advertiser and
    # reports to the Host after the scan response is received
    gActive_c = 0x01


class ScanningParametersOwnAddressType(GenericEnum):

    gPublic_c = 0x00
    gRandom_c = 0x01


class ScanningParametersFilterPolicy(GenericEnum):

    # Scans all advertising packets
    gScanAll_c = 0x00
    # Scans advertising packets using the White List
    gScanWithWhiteList_c = 0x01


class GAPConnectRequestCreateConnectionParameters_FilterPolicy(GenericEnum):

    # Initiates a connection with a specific device identified by its address
    gUseDeviceAddress_c = 0x00
    # Initiates connections with all the devices in the White List at the same time
    gUseWhiteList_c = 0x01


class GAPConnectRequestCreateConnectionParameters_OwnAddressType(GenericEnum):

    gPublic_c = 0x00
    gRandom_c = 0x01


class GAPConnectRequestCreateConnectionParameters_PeerAddressType(GenericEnum):

    gPublic_c = 0x00
    gRandom_c = 0x01


class GAPAddDeviceToWhiteListRequestAddressType(GenericEnum):

    gPublic_c = 0x00
    gRandom_c = 0x01


class GAPRemoveDeviceFromWhiteListRequestAddressType(GenericEnum):

    gPublic_c = 0x00
    gRandom_c = 0x01


class GAPReadRadioPowerLevelRequestTxReadType(GenericEnum):

    # Reading the instantaneous TX power level in a connection
    gTxPowerCurrentLevelInConnection_c = 0x00
    # Reading the maximum TX power level achieved during a connection
    gTxPowerMaximumLevelInConnection_c = 0x01
    # Reading the TX power on the advertising channels
    gTxPowerLevelForAdvertising_c = 0x02
    # Reading the Received Signal Strength Indication in a connection
    gRssi_c = 0x03


class GAPSetScanModeRequestScanMode(GenericEnum):

    # Reports all scanned devices to the application
    gDefaultScan_c = 0x00
    # Reports only devices in Limited Discoverable Mode, i.e. containing the
    # Flags AD with the LE Limited Discoverable Flag set
    gLimitedDiscovery_c = 0x01
    # Reports only devices in General Discoverable Mode, i.e. containing the
    # Flags AD with the LE General Discoverable Flag set
    gGeneralDiscovery_c = 0x02
    # Automatically connects with devices with known addresses and does not
    # report any scanned device to the application
    gAutoConnect_c = 0x03


class AutoConnectParams_AutoConnectDataFilterPolicy(GenericEnum):

    # Initiates a connection with a specific device identified by its address
    gUseDeviceAddress_c = 0x00
    # Initiates connections with all the devices in the White List at the same time
    gUseWhiteList_c = 0x01


class AutoConnectParams_AutoConnectDataOwnAddressType(GenericEnum):

    gPublic_c = 0x00
    gRandom_c = 0x01


class AutoConnectParams_AutoConnectDataPeerAddressType(GenericEnum):

    gPublic_c = 0x00
    gRandom_c = 0x01


class PairingParametersSecurityModeAndLevel(GenericEnum):

    # Mode 1 Level 1 - No Security
    gMode1Level1_c = 0x10
    # Mode 1 Level 2 - Encryption without authentication
    gMode1Level2_c = 0x11
    # Mode 1 Level 3 - Encryption with authentication
    gMode1Level3_c = 0x12
    # Mode 1 Level 4 - Encryption with LE Secure Connections pairing (BLE 4.2 only)
    gMode1Level4_c = 0x13
    # Mode 2 Level 1 - Data Signing without authentication
    gMode2Level1_c = 0x21
    # Mode 2 Level 2 - Data Signing with authentication
    gMode2Level2_c = 0x22


class PairingParametersLocalIoCapabilities(GenericEnum):

    # May display a PIN, no input
    gIoDisplayOnly_c = 0x00
    # May display a PIN and has a binary input (e.g. YES and NO buttons)
    gIoDisplayYesNo_c = 0x01
    # Has keyboard input, no display
    gIoKeyboardOnly_c = 0x02
    # No input and no display
    gIoNone_c = 0x03
    # Has keyboard input and display
    gIoKeyboardDisplay_c = 0x04


class PairingParametersCentralKeys(GenericEnum):

    # No key can be distributed
    gNoKeys_c = 0x00
    # Long Term Key
    gLtk_c = 0x01
    # Identity Resolving Key
    gIrk_c = 0x02
    # Connection Signature Resolving Key
    gCsrk_c = 0x04


class PairingParametersPeripheralKeys(GenericEnum):

    # No key can be distributed
    gNoKeys_c = 0x00
    # Long Term Key
    gLtk_c = 0x01
    # Identity Resolving Key
    gIrk_c = 0x02
    # Connection Signature Resolving Key
    gCsrk_c = 0x04


class PeerIdentitiesIdentityAddressType(GenericEnum):

    gPublic_c = 0x00
    gRandom_c = 0x01


class GAPLeScSendKeypressNotificationPrivacyRequestGapLeScKeypressNotificationParams_keypressNotifType(GenericEnum):

    gKnPasskeyEntryStarted_c = 0x00
    gKnPasskeyDigitStarted_c = 0x01
    # gKnPasskeyDigitErased_c
    gKnPasskeyDigitErased_c = 0x02
    gKnPasskeyCleared_c = 0x03
    gKnPasskeyEntryCompleted_c = 0x04


class GAPSetTxPowerLevelRequestchannelType(GenericEnum):

    Advertising = 0x00
    Connection = 0x01


class FSCIAllowDeviceToSleepRequestSignalHostWhenWakeUp(GenericEnum):

    FALSE = 0x00
    TRUE = 0x01


class FSCIErrorIndicationStatus(GenericEnum):

    gFsciSuccess_c = 0x00
    gFsciSAPDisabled_c = 0xF0
    gFsciAppMsgTooBig_c = 0xF3
    gFsciOutOfMessages_c = 0xF4
    gFsciUnknownOpcodeGroup_c = 0xF7
    gFsciOpcodeGroupIsDisabled_c = 0xF8
    gFsciUnknownOpcode_c = 0xFD
    gFsciTooBig_c = 0xFE
    gFsciError_c = 0xFF


class FSCIAllowDeviceToSleepConfirmStatus(GenericEnum):

    # Success
    gSuccess = 0x00
    # Error
    gError = 0xFF


class NVMSaveConfirmStatus(GenericEnum):

    # Data set save has been queued.
    SUCCESS = 0x00


class HCIConfirmStatus(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class L2CAPConfirmStatus(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class LeCbConnectionCompleteResult(GenericEnum):

    gSuccessful_c = 0x0000
    gLePsmNotSupported_c = 0x0002
    gNoResourcesAvailable_c = 0x0004
    gInsufficientAuthentication_c = 0x0005
    gInsufficientAuthorization_c = 0x0006
    gInsufficientEncryptionKeySize_c = 0x0007
    gInsufficientEncryption_c = 0x0008
    gInvalidSourceCid_c = 0x0009
    gSourceCidAlreadyAllocated_c = 0x000A
    gCommandRejected_c = 0x0100
    gResponseTimeout_c = 0xFFFE


class SMConfirmStatus(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class SMStatusIndicationSmStatusParams_Status(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class SMRemotePairingRequestIndicationSmPairingParams_IoCapabilities(GenericEnum):

    gDisplayOnly_c = 0x00
    gDisplayYesNo_c = 0x01
    gKeyboardOnly_c = 0x02
    gNoInputNoOutput_c = 0x03
    gKeyboardDisplay_c = 0x04


class SMRemotePairingRequestIndicationSmPairingParams_OobDataFlag(GenericEnum):

    gAuthDataNotPresent_c = 0x00
    gAuthDataPresent_c = 0x01


class SMRemotePairingResponseIndicationSmPairingParams_IoCapabilities(GenericEnum):

    gDisplayOnly_c = 0x00
    gDisplayYesNo_c = 0x01
    gKeyboardOnly_c = 0x02
    gNoInputNoOutput_c = 0x03
    gKeyboardDisplay_c = 0x04


class SMRemotePairingResponseIndicationSmPairingParams_OobDataFlag(GenericEnum):

    gAuthDataNotPresent_c = 0x00
    gAuthDataPresent_c = 0x01


class SMPasskeyRequestIndicationPasskeyReqParams_PasskeyType(GenericEnum):

    # Denotes a Passkey: 0 to 999.999 decimal
    Passkey = 0x00
    # Denotes an OOB obtained key: 16 bytes long
    Oob = 0x01


class PairingKeysetReceivedParams_KeyDistPayloadBDAddrType(GenericEnum):

    gPublic_c = 0x00
    gRandom_c = 0x01


class SMPairingFailedIndicationSmPairingFailedParams_Reason(GenericEnum):

    gReserved_c = 0x00
    gPasskeyEntryFailed_c = 0x01
    # gOobNotAvailable_c
    gOobNotAvailable_c = 0x02
    gAuthenticationRequirements_c = 0x03
    gConfirmValueFailed_c = 0x04
    gPairingNotSupported_c = 0x05
    gEncryptionKeySize_c = 0x06
    gCommandNotSupported_c = 0x07
    gUnspecifiedReason_c = 0x08
    gRepeatedAttempts_c = 0x09
    gInvalidParameters_c = 0x0A


class SMReceivedPairingFailedIndicationSmPairingFailedParams_Reason(GenericEnum):

    gReserved_c = 0x00
    gPasskeyEntryFailed_c = 0x01
    # gOobNotAvailable_c
    gOobNotAvailable_c = 0x02
    gAuthenticationRequirements_c = 0x03
    gConfirmValueFailed_c = 0x04
    gPairingNotSupported_c = 0x05
    gEncryptionKeySize_c = 0x06
    gCommandNotSupported_c = 0x07
    gUnspecifiedReason_c = 0x08
    gRepeatedAttempts_c = 0x09
    gInvalidParameters_c = 0x0A


class SMLlEncryptionStatusIndicationLlEncryptionStatusParams_LlStatus(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class SMLlEncryptionStatusIndicationLlEncryptionStatusParams_EncryptionState(GenericEnum):

    # No changes have been made to the link encryption either because it was
    # not necessary or the encryption was not successful
    gLinkEncryptionNoChange_c = 0x00
    # The link was not encrypted
    gLinkEncryptionNotEncrypted_c = 0x01
    # The link was encrypted
    gLinkEncryptionEncrypted_c = 0x02
    # The link was previously encrypted, but the key was changed
    gLinkEncryptionKeyChanged_c = 0x03


class SMLlEncryptionStatusIndicationLlEncryptionStatusParams_EncKeyType(GenericEnum):

    # Not applicable or unknown key type
    gLinkEncKeyTypeNone_c = 0x00
    # Short Term Key set up by the SM during the pairing procedure
    gLinkEncKeyTypeStk_c = 0x01
    # Long Term Key provided by the upper layer
    gLinkEncKeyTypeLtk_c = 0x02


class SMLlEncryptResIndicationLlEncryptResParams_Status(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class SMLlRandResIndicationLlRandResParams_Status(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class SMTbCreateRandomDeviceAddrResIndicationTbCreateRandomDeviceAddrResParams_Status(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class SMTbCreateRandomDeviceAddrResIndicationTbCreateRandomDeviceAddrResParams_AddrType(GenericEnum):

    RandAddrTypeNone = 0x00
    StaticAddr = 0x01
    NonResolvablePrivateAddr = 0x02
    ResolvablePrivateAddr = 0x03


class SMTbCheckResolvalePrivateAddrResIndicationTbCheckResolvablePrivateAddrResParams_Status(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class SMTbSignDataResIndicationTbSignDataResParams_Status(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class SMTbVerifyDataSignatureResIndicationTbVerifyDataSignatureResParams_Status(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class SMRemoteKeypressNotificationIndicationSmKeypressNotificationParams_keypressNotifType(GenericEnum):

    gSmpKeypressNotificationPasskeyEntryStarted_c = 0x00
    gSmpKeypressNotificationPasskeyDigitEntered_c = 0x01
    # gSmpKeypressNotificationPasskeyDigitErased_c
    gSmpKeypressNotificationPasskeyDigitErased_c = 0x02
    gSmpKeypressNotificationPasskeyCleared_c = 0x03
    gSmpKeypressNotificationPasskeyEntryCompleted_c = 0x04


class SMGenerateNewEcdhPkSkPairResIndicationStatus(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class ATTConfirmStatus(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class ATTClientIncomingServerErrorResponseIndicationParams_RequestOpcodeInError(GenericEnum):

    gErrorResponse_c = 0x01
    gExchangeMtuRequest_c = 0x02
    gExchangeMtuResponse_c = 0x03
    gFindInformationRequest_c = 0x04
    gFindInformationResponse_c = 0x05
    gFindByTypeValueRequest_c = 0x06
    gFindByTypeValueResponse_c = 0x07
    gReadByTypeRequest_c = 0x08
    gReadByTypeResponse_c = 0x09
    gReadRequest_c = 0x0A
    gReadResponse_c = 0x0B
    gReadBlobRequest_c = 0x0C
    gReadBlobResponse_c = 0x0D
    gReadMultipleRequest_c = 0x0E
    gReadMultipleResponse_c = 0x0F
    gReadByGroupTypeRequest_c = 0x10
    gReadByGroupTypeResponse_c = 0x11
    gWriteRequest_c = 0x12
    gWriteResponse_c = 0x13
    gWriteCommand_c = 0x52
    gSignedWriteCommand_c = 0xD2
    gPrepareWriteRequest_c = 0x16
    gPrepareWriteResponse_c = 0x17
    gExecuteWriteRequest_c = 0x18
    gExecuteWriteResponse_c = 0x19
    gHandleValueNotification_c = 0x1B
    gHandleValueIndication_c = 0x1D
    gHandleValueConfirmation_c = 0x1E
    #  gAttLastOpcode_c
    gAttLastOpcode_c = 0xE0


class ATTClientIncomingServerErrorResponseIndicationParams_ErrorCode(GenericEnum):

    gInvalidHandle_c = 0x01
    gReadNotPermitted_c = 0x02
    gWriteNotPermitted_c = 0x03
    gInvalidPdu_c = 0x04
    gInsufficientAuthentication_c = 0x05
    gRequestNotSupported_c = 0x06
    gInvalidOffset_c = 0x07
    gInsufficientAuthorization_c = 0x08
    gPrepareQueueFull_c = 0x09
    gAttributeNotFound_c = 0x0A
    gAttributeNotLong_c = 0x0B
    gInsufficientEncryptionKeySize_c = 0x0C
    gInvalidAttributeValueLength_c = 0x0D
    gUnlikelyError_c = 0x0E
    gInsufficientEncryption_c = 0x0F
    gUnsupportedGroupType_c = 0x10
    gInsufficientResources_c = 0x11


class ATTClientIncomingServerFindInformationResponseIndicationParams_Format(GenericEnum):

    # Uuid 16 bits format
    Uuid16BitFormat = 0x01
    # Uuid 128 bits format
    Uuid128BitFormat = 0x02


class ATTServerIncomingClientReadByTypeRequestIndicationParams_Format(GenericEnum):

    # Uuid 16 bits format
    Uuid16BitFormat = 0x01
    # Uuid 128 bits format
    Uuid128BitFormat = 0x02


class ATTServerIncomingClientReadByGroupTypeRequestIndicationParams_Format(GenericEnum):

    # Uuid 16 bits format
    Uuid16BitFormat = 0x01
    # Uuid 128 bits format
    Uuid128BitFormat = 0x02


class ATTServerIncomingClientExecuteWriteRequestIndicationParams_Flags(GenericEnum):

    gCancel_c = 0x00
    gImmediately_c = 0x01


class GATTConfirmStatus(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class GATTClientProcedureExchangeMtuIndicationProcedureResult(GenericEnum):

    # The procedure was completed successfully
    gGattProcSuccess_c = 0x00
    # The procedure was terminated due to an error
    gProcedureError_c = 0x01


class GATTClientProcedureExchangeMtuIndicationError(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class GATTClientProcedureDiscoverAllPrimaryServicesIndicationProcedureResult(GenericEnum):

    # The procedure was completed successfully
    gGattProcSuccess_c = 0x00
    # The procedure was terminated due to an error
    gProcedureError_c = 0x01


class GATTClientProcedureDiscoverAllPrimaryServicesIndicationError(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class DiscoveredServicesUuidType(GenericEnum):

    Uuid16Bits = 0x01
    Uuid128Bits = 0x02
    Uuid32Bits = 0x03


class IncludedServicesUuidType(GenericEnum):

    Uuid16Bits = 0x01
    Uuid128Bits = 0x02
    Uuid32Bits = 0x03


class GATTClientProcedureDiscoverPrimaryServicesByUuidIndicationProcedureResult(GenericEnum):

    # The procedure was completed successfully
    gGattProcSuccess_c = 0x00
    # The procedure was terminated due to an error
    gProcedureError_c = 0x01


class GATTClientProcedureDiscoverPrimaryServicesByUuidIndicationError(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class GATTClientProcedureFindIncludedServicesIndicationProcedureResult(GenericEnum):

    # The procedure was completed successfully
    gGattProcSuccess_c = 0x00
    # The procedure was terminated due to an error
    gProcedureError_c = 0x01


class GATTClientProcedureFindIncludedServicesIndicationError(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class GATTClientProcedureFindIncludedServicesIndicationService_UuidType(GenericEnum):

    Uuid16Bits = 0x01
    Uuid128Bits = 0x02
    Uuid32Bits = 0x03


class GATTClientProcedureDiscoverAllCharacteristicsIndicationProcedureResult(GenericEnum):

    # The procedure was completed successfully
    gGattProcSuccess_c = 0x00
    # The procedure was terminated due to an error
    gProcedureError_c = 0x01


class GATTClientProcedureDiscoverAllCharacteristicsIndicationError(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class GATTClientProcedureDiscoverAllCharacteristicsIndicationService_UuidType(GenericEnum):

    Uuid16Bits = 0x01
    Uuid128Bits = 0x02
    Uuid32Bits = 0x03


class GATTClientProcedureDiscoverCharacteristicByUuidIndicationProcedureResult(GenericEnum):

    # The procedure was completed successfully
    gGattProcSuccess_c = 0x00
    # The procedure was terminated due to an error
    gProcedureError_c = 0x01


class GATTClientProcedureDiscoverCharacteristicByUuidIndicationError(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class GATTClientProcedureDiscoverAllCharacteristicDescriptorsIndicationProcedureResult(GenericEnum):

    # The procedure was completed successfully
    gGattProcSuccess_c = 0x00
    # The procedure was terminated due to an error
    gProcedureError_c = 0x01


class GATTClientProcedureDiscoverAllCharacteristicDescriptorsIndicationError(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class GATTClientProcedureDiscoverAllCharacteristicDescriptorsIndicationCharacteristic_Properties(GenericEnum):

    # No Properties selected
    gNone_c = 0x00
    # Characteristic can be broadcast
    gBroadcast_c = 0x01
    # Characteristic can be read
    gRead_c = 0x02
    # Characteristic can be written without response
    gWriteWithoutRsp_c = 0x04
    # Characteristic can be written with response
    gWrite_c = 0x08
    # Characteristic can be notified
    gNotify_c = 0x10
    # Characteristic can be indicated
    gIndicate_c = 0x20
    # Characteristic can be written with signed data
    gAuthSignedWrites_c = 0x40
    # Extended Characteristic properties
    gExtendedProperties_c = 0x80


class GATTClientProcedureReadCharacteristicValueIndicationProcedureResult(GenericEnum):

    # The procedure was completed successfully
    gGattProcSuccess_c = 0x00
    # The procedure was terminated due to an error
    gProcedureError_c = 0x01


class GATTClientProcedureReadCharacteristicValueIndicationError(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class GATTClientProcedureReadCharacteristicValueIndicationCharacteristic_Properties(GenericEnum):

    # No Properties selected
    gNone_c = 0x00
    # Characteristic can be broadcast
    gBroadcast_c = 0x01
    # Characteristic can be read
    gRead_c = 0x02
    # Characteristic can be written without response
    gWriteWithoutRsp_c = 0x04
    # Characteristic can be written with response
    gWrite_c = 0x08
    # Characteristic can be notified
    gNotify_c = 0x10
    # Characteristic can be indicated
    gIndicate_c = 0x20
    # Characteristic can be written with signed data
    gAuthSignedWrites_c = 0x40
    # Extended Characteristic properties
    gExtendedProperties_c = 0x80


class GATTClientProcedureReadUsingCharacteristicUuidIndicationProcedureResult(GenericEnum):

    # The procedure was completed successfully
    gGattProcSuccess_c = 0x00
    # The procedure was terminated due to an error
    gProcedureError_c = 0x01


class GATTClientProcedureReadUsingCharacteristicUuidIndicationError(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class GATTClientProcedureReadMultipleCharacteristicValuesIndicationProcedureResult(GenericEnum):

    # The procedure was completed successfully
    gGattProcSuccess_c = 0x00
    # The procedure was terminated due to an error
    gProcedureError_c = 0x01


class GATTClientProcedureReadMultipleCharacteristicValuesIndicationError(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class GATTClientProcedureWriteCharacteristicValueIndicationProcedureResult(GenericEnum):

    # The procedure was completed successfully
    gGattProcSuccess_c = 0x00
    # The procedure was terminated due to an error
    gProcedureError_c = 0x01


class GATTClientProcedureWriteCharacteristicValueIndicationError(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class GATTClientProcedureReadCharacteristicDescriptorIndicationProcedureResult(GenericEnum):

    # The procedure was completed successfully
    gGattProcSuccess_c = 0x00
    # The procedure was terminated due to an error
    gProcedureError_c = 0x01


class GATTClientProcedureReadCharacteristicDescriptorIndicationError(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class GATTClientProcedureReadCharacteristicDescriptorIndicationDescriptor_UuidType(GenericEnum):

    Uuid16Bits = 0x01
    Uuid128Bits = 0x02
    Uuid32Bits = 0x03


class GATTClientProcedureWriteCharacteristicDescriptorIndicationProcedureResult(GenericEnum):

    # The procedure was completed successfully
    gGattProcSuccess_c = 0x00
    # The procedure was terminated due to an error
    gProcedureError_c = 0x01


class GATTClientProcedureWriteCharacteristicDescriptorIndicationError(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class GATTServerCharacteristicCccdWrittenIndicationCharCccdWrittenEvent_NewCccd(GenericEnum):

    gCccdEmpty_c = 0x00
    gCccdNotification_c = 0x01
    gCccdIndication_c = 0x02


class GATTServerErrorIndicationProcedureError_ProcedureType(GenericEnum):

    # Procedure initiated by GattServer_SendAttributeWrittenStatus
    gSendAttributeWrittenStatus_c = 0x00
    # Procedure initiated by GattServer_SendAttributeReadStatus
    gSendAttributeReadStatus_c = 0x01
    # Procedure initiated by GattServer_SendNotification
    gSendNotification_c = 0x02
    # Procedure initiated by GattServer_SendIndication
    gSendIndication_c = 0x03


class GATTServerErrorIndicationProcedureError_Error(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class GATTDBConfirmStatus(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class GATTDBAttConfirmStatus(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class GATTDBAttFindInformationIndicationParams_Format(GenericEnum):

    # Uuid 16 bits format
    Uuid16BitFormat = 0x01
    # Uuid 128 bits format
    Uuid128BitFormat = 0x02


class GAPConfirmStatus(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class GAPGenericEventInternalErrorIndicationInternalError_ErrorCode(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class GAPGenericEventInternalErrorIndicationInternalError_ErrorSource(GenericEnum):

    gHciCommandStatus_c = 0x00
    gCheckPrivateResolvableAddress_c = 0x01
    gVerifySignature_c = 0x02
    gAddNewConnection_c = 0x03
    gResetController_c = 0x04
    gSetEventMask_c = 0x05
    gReadLeBufferSize_c = 0x06
    gSetLeEventMask_c = 0x07
    gReadDeviceAddress_c = 0x08
    gReadLocalSupportedFeatures_c = 0x09
    gReadWhiteListSize_c = 0x0A
    gClearWhiteList_c = 0x0B
    gAddDeviceToWhiteList_c = 0x0C
    gRemoveDeviceFromWhiteList_c = 0x0D
    gCancelCreateConnection_c = 0x0E
    gReadRadioPower_c = 0x0F
    gSetRandomAddress_c = 0x10
    gCreateRandomAddress_c = 0x11
    gEncryptLink_c = 0x12
    gProvideLongTermKey_c = 0x13
    gDenyLongTermKey_c = 0x14
    gConnect_c = 0x15
    gDisconnect_c = 0x16
    gTerminatePairing_c = 0x17
    gSendSlaveSecurityRequest_c = 0x18
    gEnterPasskey_c = 0x19
    gProvideOob_c = 0x1A
    gSendSmpKeys_c = 0x1B
    gWriteSuggestedDefaultDataLength_c = 0x1C
    gReadSuggestedDefaultDataLength_c = 0x1D
    gUpdateLeDataLength_c = 0x1E
    gEnableControllerPrivacy_c = 0x1F
    gLeScSendKeypressNotification_c = 0x20
    gLeScSetPeerOobData_c = 0x21
    gLeScGetLocalOobData_c = 0x22
    gLeScValidateNumericValue_c = 0x23
    gLeScRegeneratePublicKey_c = 0x24
    gLeSetResolvablePrivateAddressTimeout_c = 0x25


class GAPGenericEventAdvertisingSetupFailedIndicationSetupFailReason(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class GAPAdvertisingEventCommandFailedIndicationFailReason(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class GAPScanningEventCommandFailedIndicationFailReason(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class GAPScanningEventDeviceScannedIndicationScannedDevice_AddressType(GenericEnum):

    gPublic_c = 0x00
    gRandom_c = 0x01


class GAPScanningEventDeviceScannedIndicationScannedDevice_AdvEventType(GenericEnum):

    # Connectable undirected advertising (ADV_IND)
    gBleAdvRepAdvInd_c = 0x00
    # Connectable directed advertising (ADV_DIRECT_IND)
    gBleAdvRepAdvDirectInd_c = 0x01
    # Scannable undirected advertising (ADV_SCAN_IND)
    gBleAdvRepAdvScanInd_c = 0x02
    # Non Connectable undirected advertising (ADV_NONCONN_IND)
    gBleAdvRepAdvNonconnInd_c = 0x03
    # Scan Response (SCAN_RSP)
    gBleAdvRepScanRsp_c = 0x04


class GAPConnectionEventConnectedIndicationConnectionParameters_MasterClockAccuracy(GenericEnum):

    gMasterClkAcc500ppm_c = 0x00
    gMasterClkAcc250ppm_c = 0x01
    gMasterClkAcc150ppm_c = 0x02
    gMasterClkAcc100ppm_c = 0x03
    gMasterClkAcc75ppm_c = 0x04
    gMasterClkAcc50ppm_c = 0x05
    gMasterClkAcc30ppm_c = 0x06
    gMasterClkAcc20ppm_c = 0x07


class GAPConnectionEventConnectedIndicationPeerAddressType(GenericEnum):

    gPublic_c = 0x00
    gRandom_c = 0x01


class GAPConnectionEventPairingRequestIndicationPairingParameters_SecurityModeAndLevel(GenericEnum):

    # Mode 1 Level 1 - No Security
    gMode1Level1_c = 0x10
    # Mode 1 Level 2 - Encryption without authentication
    gMode1Level2_c = 0x11
    # Mode 1 Level 3 - Encryption with authentication
    gMode1Level3_c = 0x12
    # Mode 1 Level 4 - Encryption with LE Secure Connections pairing (BLE 4.2 only)
    gMode1Level4_c = 0x13
    # Mode 2 Level 1 - Data Signing without authentication
    gMode2Level1_c = 0x21
    # Mode 2 Level 2 - Data Signing with authentication
    gMode2Level2_c = 0x22


class GAPConnectionEventPairingRequestIndicationPairingParameters_LocalIoCapabilities(GenericEnum):

    # May display a PIN, no input
    gIoDisplayOnly_c = 0x00
    # May display a PIN and has a binary input (e.g. YES and NO buttons)
    gIoDisplayYesNo_c = 0x01
    # Has keyboard input, no display
    gIoKeyboardOnly_c = 0x02
    # No input and no display
    gIoNone_c = 0x03
    # Has keyboard input and display
    gIoKeyboardDisplay_c = 0x04


class GAPConnectionEventPairingRequestIndicationPairingParameters_CentralKeys(GenericEnum):

    # No key can be distributed
    gNoKeys_c = 0x00
    # Long Term Key
    gLtk_c = 0x01
    # Identity Resolving Key
    gIrk_c = 0x02
    # Connection Signature Resolving Key
    gCsrk_c = 0x04


class GAPConnectionEventPairingRequestIndicationPairingParameters_PeripheralKeys(GenericEnum):

    # No key can be distributed
    gNoKeys_c = 0x00
    # Long Term Key
    gLtk_c = 0x01
    # Identity Resolving Key
    gIrk_c = 0x02
    # Connection Signature Resolving Key
    gCsrk_c = 0x04


class GAPConnectionEventPairingResponseIndicationPairingParameters_SecurityModeAndLevel(GenericEnum):

    # Mode 1 Level 1 - No Security
    gMode1Level1_c = 0x10
    # Mode 1 Level 2 - Encryption without authentication
    gMode1Level2_c = 0x11
    # Mode 1 Level 3 - Encryption with authentication
    gMode1Level3_c = 0x12
    # Mode 1 Level 4 - Encryption with LE Secure Connections pairing (BLE 4.2 only)
    gMode1Level4_c = 0x13
    # Mode 2 Level 1 - Data Signing without authentication
    gMode2Level1_c = 0x21
    # Mode 2 Level 2 - Data Signing with authentication
    gMode2Level2_c = 0x22


class GAPConnectionEventPairingResponseIndicationPairingParameters_LocalIoCapabilities(GenericEnum):

    # May display a PIN, no input
    gIoDisplayOnly_c = 0x00
    # May display a PIN and has a binary input (e.g. YES and NO buttons)
    gIoDisplayYesNo_c = 0x01
    # Has keyboard input, no display
    gIoKeyboardOnly_c = 0x02
    # No input and no display
    gIoNone_c = 0x03
    # Has keyboard input and display
    gIoKeyboardDisplay_c = 0x04


class GAPConnectionEventPairingResponseIndicationPairingParameters_CentralKeys(GenericEnum):

    # No key can be distributed
    gNoKeys_c = 0x00
    # Long Term Key
    gLtk_c = 0x01
    # Identity Resolving Key
    gIrk_c = 0x02
    # Connection Signature Resolving Key
    gCsrk_c = 0x04


class GAPConnectionEventPairingResponseIndicationPairingParameters_PeripheralKeys(GenericEnum):

    # No key can be distributed
    gNoKeys_c = 0x00
    # Long Term Key
    gLtk_c = 0x01
    # Identity Resolving Key
    gIrk_c = 0x02
    # Connection Signature Resolving Key
    gCsrk_c = 0x04


class GAPConnectionEventAuthenticationRejectedIndicationRejectReason(GenericEnum):

    # This device does not have the required OOB for authenticated pairing
    gOobNotAvailable_c = 0x02
    # The combination of I/O capabilities does not allow pairing with the desired level of security
    gIncompatibleIoCapabilities_c = 0x03
    # This device does not support pairing
    gPairingNotSupported_c = 0x05
    # The peer's encryption key size is too low for this device's required security level
    gLowEncryptionKeySize_c = 0x06
    # The host has rejected the pairing for an unknown reason
    gUnspecifiedReason_c = 0x08
    # This device is the target of repeated unsuccessful pairing attempts and
    # does not allow further pairing attempts at the moment
    gRepeatedAttempts_c = 0x09
    # Link could not be encrypted
    gLinkEncryptionFailed_c = 0xF0


class GAPConnectionEventKeyExchangeRequestIndicationRequestedKeys(GenericEnum):

    # No key can be distributed
    gNoKeys_c = 0x00
    # Long Term Key
    gLtk_c = 0x01
    # Identity Resolving Key
    gIrk_c = 0x02
    # Connection Signature Resolving Key
    gCsrk_c = 0x04


class GAPConnectionEventPairingCompleteIndicationPairingStatus(GenericEnum):

    # Pairing succeeded
    PairingSuccessful = 0x01
    # Pairing failed
    PairingFailed = 0x00


class PairingFailedFailReason(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class GAPConnectionEventDisconnectedIndicationReason(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class GAPConnectionEventPowerReadFailureIndicationFailReason(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class GAPConnectionEventParameterUpdateCompleteIndicationConnectionParameterUpdateComplete_Status(GenericEnum):

    gBleSuccess_c = 0x0000
    gBleInvalidParameter_c = 0x0001
    gBleOverflow_c = 0x0002
    gBleUnavailable_c = 0x0003
    gBleFeatureNotSupported_c = 0x0004
    gBleOutOfMemory_c = 0x0005
    gBleAlreadyInitialized_c = 0x0006
    gBleOsError_c = 0x0007
    gBleUnexpectedError_c = 0x0008
    gBleInvalidState_c = 0x0009
    gHciUnknownHciCommand_c = 0x0101
    gHciUnknownConnectionIdentifier_c = 0x0102
    gHciHardwareFailure_c = 0x0103
    gHciPageTimeout_c = 0x0104
    gHciAuthenticationFailure_c = 0x0105
    gHciPinOrKeyMissing_c = 0x0106
    gHciMemoryCapacityExceeded_c = 0x0107
    gHciConnectionTimeout_c = 0x0108
    gHciConnectionLimitExceeded_c = 0x0109
    gHciSynchronousConnectionLimitToADeviceExceeded_c = 0x010A
    gHciAclConnectionAlreadyExists_c = 0x010B
    gHciCommandDisallowed_c = 0x010C
    gHciConnectionRejectedDueToLimitedResources_c = 0x010D
    gHciConnectionRejectedDueToSecurityReasons_c = 0x010E
    gHciConnectionRejectedDueToUnacceptableBdAddr_c = 0x010F
    gHciConnectionAcceptTimeoutExceeded_c = 0x0110
    gHciUnsupportedFeatureOrParameterValue_c = 0x0111
    gHciInvalidHciCommandParameters_c = 0x0112
    gHciRemoteUserTerminatedConnection_c = 0x0113
    gHciRemoteDeviceTerminatedConnectionLowResources_c = 0x0114
    gHciRemoteDeviceTerminatedConnectionPowerOff_c = 0x0115
    gHciConnectionTerminatedByLocalHost_c = 0x0116
    gHciRepeatedAttempts_c = 0x0117
    gHciPairingNotAllowed_c = 0x0118
    gHciUnknownLpmPdu_c = 0x0119
    gHciUnsupportedRemoteFeature_c = 0x011A
    gHciScoOffsetRejected_c = 0x011B
    gHciScoIntervalRejected_c = 0x011C
    gHciScoAirModeRejected_c = 0x011D
    gHciInvalidLpmParameters_c = 0x011E
    gHciUnspecifiedError_c = 0x011F
    gHciUnsupportedLpmParameterValue_c = 0x0120
    gHciRoleChangeNotAllowed_c = 0x0121
    gHciLLResponseTimeout_c = 0x0122
    gHciLmpErrorTransactionCollision_c = 0x0123
    gHciLmpPduNotAllowed_c = 0x0124
    gHciEncryptionModeNotAcceptable_c = 0x0125
    gHciLinkKeyCannotBeChanged_c = 0x0126
    gHciRequestedQosNotSupported_c = 0x0127
    gHciInstantPassed_c = 0x0128
    gHciPairingWithUnitKeyNotSupported_c = 0x0129
    gHciDifferentTransactionCollision_c = 0x012A
    gHciReserved_0x2B_c = 0x012B
    gHciQosNotAcceptableParameter_c = 0x012C
    gHciQosRejected_c = 0x012D
    gHciChannelClassificationNotSupported_c = 0x012E
    gHciInsufficientSecurity_c = 0x012F
    gHciParameterOutOfMandatoryRange_c = 0x0130
    gHciReserved_0x31_c = 0x0131
    gHciRoleSwitchPending_c = 0x0132
    gHciReserved_0x33_c = 0x0133
    gHciReservedSlotViolation_c = 0x0134
    gHciRoleSwitchFailed_c = 0x0135
    gHciExtendedInquiryResponseTooLarge_c = 0x0136
    gHciSecureSimplePairingNotSupportedByHost_c = 0x0137
    gHciHostBusyPairing_c = 0x0138
    gHciConnectionRejectedDueToNoSuitableChannelFound_c = 0x0139
    gHciControllerBusy_c = 0x013A
    gHciUnacceptableConnectionParameters_c = 0x013B
    gHciDirectedAdvertisingTimeout_c = 0x013C
    gHciConnectionTerminatedDueToMicFailure_c = 0x013D
    gHciConnectionFailedToBeEstablished_c = 0x013E
    gHciMacConnectionFailed_c = 0x013F
    gHciCoarseClockAdjustmentRejected_c = 0x0140
    gHciAlreadyInit_c = 0x01A0
    gHciInvalidParameter_c = 0x01A1
    gHciCallbackNotInstalled_c = 0x01A2
    gHciCallbackAlreadyInstalled_c = 0x01A3
    gHciCommandNotSupported_c = 0x01A4
    gHciEventNotSupported_c = 0x01A5
    gHciTransportError_c = 0x01A6
    gL2caAlreadyInit_c = 0x03F0
    gL2caInsufficientResources_c = 0x03F1
    gL2caCallbackNotInstalled_c = 0x03F2
    gL2caCallbackAlreadyInstalled_c = 0x03F3
    gL2caLePsmInvalid_c = 0x03F4
    gL2caLePsmAlreadyRegistered_c = 0x03F5
    gL2caLePsmNotRegistered_c = 0x03F6
    gL2caLePsmInsufficientResources_c = 0x03F7
    gL2caChannelInvalid_c = 0x03F8
    gL2caChannelClosed_c = 0x03F9
    gL2caChannelAlreadyConnected_c = 0x03FA
    gL2caConnectionParametersRejected_c = 0x03FB
    gL2caChannelBusy_c = 0x03FC
    gL2caInvalidParameter_c = 0x03FE
    gL2caError_c = 0x03FF
    gSmNullCBFunction_c = 0x0401
    gSmCommandNotSupported_c = 0x0402
    gSmUnexpectedCommand_c = 0x0403
    gSmInvalidCommandCode_c = 0x0404
    gSmInvalidCommandLength_c = 0x0405
    gSmInvalidCommandParameter_c = 0x0406
    gSmInvalidDeviceId_c = 0x0407
    gSmInvalidInternalOperation_c = 0x0408
    gSmInvalidConnectionHandle_c = 0x0409
    gSmInproperKeyDistributionField_c = 0x040A
    gSmUnexpectedKeyType_c = 0x040B
    gSmUnexpectedPairingTerminationReason_c = 0x040C
    gSmUnexpectedKeyset_c = 0x040D
    gSmSmpTimeoutOccurred_c = 0x040E
    gSmUnknownSmpPacketType_c = 0x040F
    gSmInvalidSmpPacketLength_c = 0x0410
    gSmInvalidSmpPacketParameter_c = 0x0411
    gSmReceivedUnexpectedSmpPacket_c = 0x0412
    gSmReceivedSmpPacketFromUnknownDevice_c = 0x0413
    gSmReceivedUnexpectedHciEvent_c = 0x0414
    gSmReceivedHciEventFromUnknownDevice_c = 0x0415
    gSmInvalidHciEventParameter_c = 0x0416
    gSmLlConnectionEncryptionInProgress_c = 0x0417
    gSmLlConnectionEncryptionFailure_c = 0x0418
    gSmInsufficientResources_c = 0x0419
    gSmOobDataAddressMismatch_c = 0x041A
    gSmSmpPacketReceivedAfterTimeoutOccurred_c = 0x041B
    gSmReceivedTimerEventForUnknownDevice_c = 0x041C
    gSmUnattainableLocalDeviceSecRequirements_c = 0x041D
    gSmUnattainableLocalDeviceMinKeySize_c = 0x041E
    gSmUnattainableSlaveSecReqRequirements_c = 0x041F
    gSmPairingErrorPasskeyEntryFailed_c = 0x0431
    gSmPairingErrorConfirmValueFailed_c = 0x0432
    gSmPairingErrorCommandNotSupported_c = 0x0433
    gSmPairingErrorInvalidParameters_c = 0x0434
    gSmPairingErrorUnknownReason_c = 0x0435
    gSmTbResolvableAddressDoesNotMatchIrk_c = 0x0460
    gSmTbInvalidDataSignature_c = 0x0461
    gAttInvalidHandle_c = 0x0501
    gAttReadNotPermitted_c = 0x0502
    gAttWriteNotPermitted_c = 0x0503
    gAttInvalidPdu_c = 0x0504
    gAttInsufficientAuthentication_c = 0x0505
    gAttRequestNotSupported_c = 0x0506
    gAttInvalidOffset_c = 0x0507
    gAttInsufficientAuthorization_c = 0x0508
    gAttPrepareQueueFull_c = 0x0509
    gAttAttributeNotFound_c = 0x050A
    gAttAttributeNotLong_c = 0x050B
    gAttInsufficientEncryptionKeySize_c = 0x050C
    gAttInvalidAttributeValueLength_c = 0x050D
    gAttUnlikelyor_c = 0x050E
    gAttInsufficientEncryption_c = 0x050F
    gAttUnsupportedGroupType_c = 0x0510
    gAttInsufficientResources_c = 0x0511
    gGattAnotherProcedureInProgress_c = 0x0601
    gGattLongAttributePacketsCorrupted_c = 0x0602
    gGattMultipleAttributesOverflow_c = 0x0603
    gGattUnexpectedReadMultipleResponseLength_c = 0x0604
    gGattInvalidValueLength_c = 0x0605
    gGattServerTimeout_c = 0x0606
    gGattIndicationAlreadyInProgress_c = 0x0607
    gGattClientConfirmationTimeout_c = 0x0608
    gGapAdvDataTooLong_c = 0x0701
    gGapScanRspDataTooLong_c = 0x0702
    gGapDeviceNotBonded_c = 0x0703
    gDevDbCccdLimitReached_c = 0x0801
    gDevDbCccdNotFound_c = 0x0802
    gGattDbInvalidHandle_c = 0x0901
    gGattDbCharacteristicNotFound_c = 0x0902
    gGattDbCccdNotFound_c = 0x0903
    gGattDbServiceNotFound_c = 0x0904
    gGattDbDescriptorNotFound_c = 0x0905


class GAPConnectionEventLeScKeypressNotificationIndicationGapLeScKeypressNotificationParams_keypressNotifType(GenericEnum):

    gKnPasskeyEntryStarted_c = 0x00
    gKnPasskeyDigitStarted_c = 0x01
    # gKnPasskeyDigitErased_c
    gKnPasskeyDigitErased_c = 0x02
    gKnPasskeyCleared_c = 0x03
    gKnPasskeyEntryCompleted_c = 0x04


class IdentityAddressesIdentityAddressType(GenericEnum):

    gPublic_c = 0x00
    gRandom_c = 0x01
