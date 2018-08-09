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

from com.nxp.wireless_connectivity.commands.ble.enums import *  # @UnusedWildImport
import com.nxp.wireless_connectivity.commands.ble.frames as Frames
from com.nxp.wireless_connectivity.commands.ble.operations import *  # @UnusedWildImport


def FSCICPUReset(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Firmware,
    timeout=1
):
    request = Frames.FSCICPUResetRequest()
    return FSCICPUResetOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def HCIModeSelect(
    device,
    Enable=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.HCIModeSelectRequest(Enable)
    return HCIModeSelectOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def HCICommand(
    device,
    CommandLength=bytearray(2),
    # Array length depends on CommandLength. Mask: 0xFFFF. Shift: 0x0000.
    Command=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.HCICommandRequest(CommandLength, Command)
    return HCICommandOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def HCIData(
    device,
    DataLength=bytearray(2),
    # Array length depends on DataLength. Mask: 0xFFFF. Shift: 0x0000.
    Data=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.HCIDataRequest(DataLength, Data)
    return HCIDataOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def HCISynchronousData(
    device,
    SynchronousDataLength=bytearray(2),
    # Array length depends on SynchronousDataLength. Mask: 0xFFFF. Shift: 0x0000.
    SynchronousData=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.HCISynchronousDataRequest(SynchronousDataLength, SynchronousData)
    return HCISynchronousDataOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def L2CAPModeSelect(
    device,
    Enable=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.L2CAPModeSelectRequest(Enable)
    return L2CAPModeSelectOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def L2CAPInit(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.L2CAPInitRequest()
    return L2CAPInitOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def L2CAPConfig(
    device,
    ConfigStruct_LeFeatures=bytearray(4),
    ConfigStruct_HCILeBufferSize=bytearray(4),
    ConfigStruct_MaxPduPayloadSize=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.L2CAPConfigRequest(ConfigStruct_LeFeatures, ConfigStruct_HCILeBufferSize, ConfigStruct_MaxPduPayloadSize)
    return L2CAPConfigOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def L2CAPSendAttData(
    device,
    DeviceId=bytearray(1),
    PacketLength=bytearray(2),
    # Array length depends on PacketLength. Mask: 0xFFFF. Shift: 0x0000.
    Packet=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.L2CAPSendAttDataRequest(DeviceId, PacketLength, Packet)
    return L2CAPSendAttDataOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def L2CAPSendSmpData(
    device,
    DeviceId=bytearray(1),
    PacketLength=bytearray(2),
    # Array length depends on PacketLength. Mask: 0xFFFF. Shift: 0x0000.
    Packet=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.L2CAPSendSmpDataRequest(DeviceId, PacketLength, Packet)
    return L2CAPSendSmpDataOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def L2CAPRegisterAttCallback(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.L2CAPRegisterAttCallbackRequest()
    return L2CAPRegisterAttCallbackOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def L2CAPRegisterSmpCallback(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.L2CAPRegisterSmpCallbackRequest()
    return L2CAPRegisterSmpCallbackOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def L2CAPRegisterLePsm(
    device,
    LePsm=bytearray(2),
    LePsmMtu=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.L2CAPRegisterLePsmRequest(LePsm, LePsmMtu)
    return L2CAPRegisterLePsmOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def L2CAPDeregisterLePsm(
    device,
    LePsm=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.L2CAPDeregisterLePsmRequest(LePsm)
    return L2CAPDeregisterLePsmOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def L2CAPConnectLePsm(
    device,
    LePsm=bytearray(2),
    DeviceId=bytearray(1),
    InitialCredits=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.L2CAPConnectLePsmRequest(LePsm, DeviceId, InitialCredits)
    return L2CAPConnectLePsmOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def L2CAPDisconnectLeCbChannel(
    device,
    DeviceId=bytearray(1),
    ChannelId=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.L2CAPDisconnectLeCbChannelRequest(DeviceId, ChannelId)
    return L2CAPDisconnectLeCbChannelOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def L2CAPCancelConnection(
    device,
    LePsm=bytearray(2),
    DeviceId=bytearray(1),
    RefuseReason=L2CAPCancelConnectionRequestRefuseReason.gSuccessful_c,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.L2CAPCancelConnectionRequest(LePsm, DeviceId, RefuseReason)
    return L2CAPCancelConnectionOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def L2CAPSendLeCbData(
    device,
    DeviceId=bytearray(1),
    ChannelId=bytearray(2),
    PacketLength=bytearray(2),
    # Array length depends on PacketLength. Mask: 0xFFFF. Shift: 0x0000.
    Packet=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.L2CAPSendLeCbDataRequest(DeviceId, ChannelId, PacketLength, Packet)
    return L2CAPSendLeCbDataOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def L2CAPSendLeCredit(
    device,
    DeviceId=bytearray(1),
    ChannelId=bytearray(2),
    Credits=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.L2CAPSendLeCreditRequest(DeviceId, ChannelId, Credits)
    return L2CAPSendLeCreditOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def SMModeSelect(
    device,
    Enable=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.SMModeSelectRequest(Enable)
    return SMModeSelectOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def SMInit(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.SMInitRequest()
    return SMInitOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def SMRegisterUserCallback(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.SMRegisterUserCallbackRequest()
    return SMRegisterUserCallbackOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def SMInitiatorStartPairing(
    device,
    DestinationDeviceId=bytearray(1),
    SmPairingParams_IoCapabilities=SMInitiatorStartPairingRequestSmPairingParams_IoCapabilities.gDisplayOnly_c,
    SmPairingParams_OobDataFlag=SMInitiatorStartPairingRequestSmPairingParams_OobDataFlag.gAuthDataNotPresent_c,
    SmPairingParams_AuthRequest=SMInitiatorStartPairingRequest.SmPairingParams_AuthRequest(),
    SmPairingParams_MaxEncKeySize=bytearray(1),
    SmPairingParams_InitatorKeyDistribution=SMInitiatorStartPairingRequest.SmPairingParams_InitatorKeyDistribution(),
    SmPairingParams_ResponderKeyDistribution=SMInitiatorStartPairingRequest.SmPairingParams_ResponderKeyDistribution(),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.SMInitiatorStartPairingRequest(DestinationDeviceId, SmPairingParams_IoCapabilities, SmPairingParams_OobDataFlag, SmPairingParams_AuthRequest, SmPairingParams_MaxEncKeySize, SmPairingParams_InitatorKeyDistribution, SmPairingParams_ResponderKeyDistribution)
    return SMInitiatorStartPairingOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def SMSendSecurity(
    device,
    DestinationDeviceId=bytearray(1),
    SmSecurityReqParams_AuthRequest=SMSendSecurityRequestRequest.SmSecurityReqParams_AuthRequest(),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.SMSendSecurityRequestRequest(DestinationDeviceId, SmSecurityReqParams_AuthRequest)
    return SMSendSecurityRequestOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def SMTerminatePairing(
    device,
    DestinationDeviceId=bytearray(1),
    SmTerminatePairingParams_Reason=SMTerminatePairingRequestSmTerminatePairingParams_Reason.gReserved_c,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.SMTerminatePairingRequest(DestinationDeviceId, SmTerminatePairingParams_Reason)
    return SMTerminatePairingOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def SMPairingReply(
    device,
    DestinationDeviceId=bytearray(1),
    SmPairingParams_IoCapabilities=SMPairingRequestReplyRequestSmPairingParams_IoCapabilities.gDisplayOnly_c,
    SmPairingParams_OobDataFlag=SMPairingRequestReplyRequestSmPairingParams_OobDataFlag.gAuthDataNotPresent_c,
    SmPairingParams_AuthRequest=SMPairingRequestReplyRequest.SmPairingParams_AuthRequest(),
    SmPairingParams_MaxEncKeySize=bytearray(1),
    SmPairingParams_InitatorKeyDistribution=SMPairingRequestReplyRequest.SmPairingParams_InitatorKeyDistribution(),
    SmPairingParams_ResponderKeyDistribution=SMPairingRequestReplyRequest.SmPairingParams_ResponderKeyDistribution(),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.SMPairingRequestReplyRequest(DestinationDeviceId, SmPairingParams_IoCapabilities, SmPairingParams_OobDataFlag, SmPairingParams_AuthRequest, SmPairingParams_MaxEncKeySize, SmPairingParams_InitatorKeyDistribution, SmPairingParams_ResponderKeyDistribution)
    return SMPairingRequestReplyOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def SMPasskeyReply(
    device,
    DestinationDeviceId=bytearray(1),
    SmPasskeyReqReplyParams_KeyType=SMPasskeyRequestReplyRequestSmPasskeyReqReplyParams_KeyType.Passkey,
    # Array length depends on KeyType. Mask: 0xFF. Shift: 0x00.
    SmPasskeyReqReplyParams_Key=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.SMPasskeyRequestReplyRequest(DestinationDeviceId, SmPasskeyReqReplyParams_KeyType, SmPasskeyReqReplyParams_Key)
    return SMPasskeyRequestReplyOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def SMPairingKeysetReply(
    device,
    DestinationDeviceId=bytearray(1),
    PairingKeysetRequestReplyParams_KeyDistPayload=SMPairingKeysetRequestReplyRequest.PairingKeysetRequestReplyParams_KeyDistPayload(),
    PairingKeysetRequestReplyParams_SentKeys=SMPairingKeysetRequestReplyRequest.PairingKeysetRequestReplyParams_SentKeys(),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.SMPairingKeysetRequestReplyRequest(DestinationDeviceId, PairingKeysetRequestReplyParams_KeyDistPayload, PairingKeysetRequestReplyParams_SentKeys)
    return SMPairingKeysetRequestReplyOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def SMLlStartEncryption(
    device,
    DestinationDeviceId=bytearray(1),
    LlStartEncryptionParams_RandomNumber=bytearray(8),
    LlStartEncryptionParams_EncryptedDiversifier=bytearray(2),
    # Unit length: 16 bytes
    LlStartEncryptionParams_LongTermKey=bytearray(16),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.SMLlStartEncryptionRequest(DestinationDeviceId, LlStartEncryptionParams_RandomNumber, LlStartEncryptionParams_EncryptedDiversifier, LlStartEncryptionParams_LongTermKey)
    return SMLlStartEncryptionOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def SMLlLtkReply(
    device,
    DestinationDeviceId=bytearray(1),
    # Unit length: 16 bytes
    LlLtkRequestReplyParams_LongTermKey=bytearray(16),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.SMLlLtkRequestReplyRequest(DestinationDeviceId, LlLtkRequestReplyParams_LongTermKey)
    return SMLlLtkRequestOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def SMLlLtkNegativeReply(
    device,
    DestinationDeviceId=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.SMLlLtkRequestNegativeReplyRequest(DestinationDeviceId)
    return SMLlLtkRequestNegativeReplyOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def SMLlEncrypt(
    device,
    # Unit length: 16 bytes
    LlEncryptReqParams_Key=bytearray(16),
    # Unit length: 16 bytes
    LlEncryptReqParams_PlainTextData=bytearray(16),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.SMLlEncryptRequestRequest(LlEncryptReqParams_Key, LlEncryptReqParams_PlainTextData)
    return SMLlEncryptResOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def SMLlRand(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.SMLlRandRequestRequest()
    return SMLlRandResOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def SMTbCreateRandomDeviceAddress(
    device,
    TbCreateRandomDeviceAddrReqParams_AddrType=SMTbCreateRandomDeviceAddressRequestRequestTbCreateRandomDeviceAddrReqParams_AddrType.RandAddrTypeNone,
    # Array length depends on AddrType. Mask: 0xFF. Shift: 0x00.
    TbCreateRandomDeviceAddrReqParams_Data=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.SMTbCreateRandomDeviceAddressRequestRequest(TbCreateRandomDeviceAddrReqParams_AddrType, TbCreateRandomDeviceAddrReqParams_Data)
    return SMTbCreateRandomDeviceAddrResOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def SMTbCheckResolvablePrivateAddress(
    device,
    # Unit length: 16 bytes
    TbCheckResolvablePrivateAddrReqParams_IRK=bytearray(16),
    # Unit length: 6 bytes
    TbCheckResolvablePrivateAddrReqParams_Address=bytearray(6),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.SMTbCheckResolvablePrivateAddressRequestRequest(TbCheckResolvablePrivateAddrReqParams_IRK, TbCheckResolvablePrivateAddrReqParams_Address)
    return SMTbCheckResolvablePrivateAddressRequestOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def SMTbSignData(
    device,
    # Unit length: 16 bytes
    TbSignDataReqParams_CSRK=bytearray(16),
    TbSignDataReqParams_DataLength=bytearray(2),
    # Array length depends on DataLength. Mask: 0xFFFF. Shift: 0x0000.
    TbSignDataReqParams_Data=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.SMTbSignDataRequestRequest(TbSignDataReqParams_CSRK, TbSignDataReqParams_DataLength, TbSignDataReqParams_Data)
    return SMTbSignDataRequestOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def SMTbVerifyDataSignature(
    device,
    # Unit length: 16 bytes
    TbVerifyDataSignatureReqParams_CSRK=bytearray(16),
    TbVerifyDataSignatureReqParams_DataLength=bytearray(2),
    # Array length depends on DataLength. Mask: 0xFFFF. Shift: 0x0000.
    TbVerifyDataSignatureReqParams_Data=[],
    TbVerifyDataSignatureReqParams_Signature=bytearray(8),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.SMTbVerifyDataSignatureRequestRequest(TbVerifyDataSignatureReqParams_CSRK, TbVerifyDataSignatureReqParams_DataLength, TbVerifyDataSignatureReqParams_Data, TbVerifyDataSignatureReqParams_Signature)
    return SMTbVerifyDataSignatureRequestOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def SMDeviceConnectNotification(
    device,
    DestinationDeviceId=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.SMDeviceConnectNotificationRequest(DestinationDeviceId)
    return SMDeviceConnectNotificationOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def SMDeviceDisconnectNotification(
    device,
    DestinationDeviceId=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.SMDeviceDisconnectNotificationRequest(DestinationDeviceId)
    return SMDeviceDisconnectNotificationOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def SMSetDefaultPasskey(
    device,
    SetDefaultPasskeyParams_DefaultPasskey=bytearray(4),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.SMSetDefaultPasskeyRequest(SetDefaultPasskeyParams_DefaultPasskey)
    return SMSetDefaultPasskeyOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def SMSetOobMitmProtection(
    device,
    oobMitmProt=SMSetOobMitmProtectionRequestoobMitmProt.smOobMitmProtectionOff_c,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.SMSetOobMitmProtectionRequest(oobMitmProt)
    return SMSetOobMitmProtectionOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def SMSendKeypressNotification(
    device,
    DestinationDeviceId=bytearray(1),
    SmKeypressNotificationParams_keypressNotifType=SMSendKeypressNotificationRequestSmKeypressNotificationParams_keypressNotifType.gSmpKeypressNotificationPasskeyEntryStarted_c,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.SMSendKeypressNotificationRequest(DestinationDeviceId, SmKeypressNotificationParams_keypressNotifType)
    return SMSendKeypressNotificationOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def SMNcDisplayConfirm(
    device,
    DestinationDeviceId=bytearray(1),
    ncValueConfirm=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.SMNcDisplayConfirmRequest(DestinationDeviceId, ncValueConfirm)
    return SMNcDisplayConfirmOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def SMLeScOobDataReply(
    device,
    DestinationDeviceId=bytearray(1),
    LeScOobDataParams_=bytearray(),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.SMLeScOobDataRequestReplyRequest(DestinationDeviceId, LeScOobDataParams_)
    return SMLeScOobDataRequestReplyOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def SMLocalLeScOobDataReq(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.SMLocalLeScOobDataRequestReqRequest()
    return SMLocalLeScOobDataOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def SMGenerateNewEcdhPkSkPair(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.SMGenerateNewEcdhPkSkPairRequest()
    return SMGenerateNewEcdhPkSkPairOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTModeSelect(
    device,
    Enable=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTModeSelectRequest(Enable)
    return ATTModeSelectOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTInit(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTInitRequest()
    return ATTInitOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTNotifyConnection(
    device,
    DeviceId=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTNotifyConnectionRequest(DeviceId)
    return ATTNotifyConnectionOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTNotifyDisconnection(
    device,
    DeviceId=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTNotifyDisconnectionRequest(DeviceId)
    return ATTNotifyDisconnectionOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTSetMtu(
    device,
    DeviceId=bytearray(1),
    Mtu=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTSetMtuRequest(DeviceId, Mtu)
    return ATTSetMtuOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTGetMtu(
    device,
    DeviceId=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTGetMtuRequest(DeviceId)
    return ATTGetMtuOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTRegisterOpcodeCallback(
    device,
    Opcode=ATTRegisterOpcodeCallbackRequestOpcode.gErrorResponse_c,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTRegisterOpcodeCallbackRequest(Opcode)
    return ATTRegisterOpcodeCallbackOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTRegisterUnsupportedOpcodeCallback(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTRegisterUnsupportedOpcodeCallbackRequest()
    return ATTRegisterUnsupportedOpcodeCallbackOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTRegisterTimeoutCallback(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTRegisterTimeoutCallbackRequest()
    return ATTRegisterTimeoutCallbackOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTServerSendErrorResponse(
    device,
    DeviceId=bytearray(1),
    Params_RequestOpcodeInError=ATTServerSendErrorResponseRequestParams_RequestOpcodeInError.gErrorResponse_c,
    Params_AttributeHandleInError=bytearray(2),
    Params_ErrorCode=ATTServerSendErrorResponseRequestParams_ErrorCode.gInvalidHandle_c,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTServerSendErrorResponseRequest(DeviceId, Params_RequestOpcodeInError, Params_AttributeHandleInError, Params_ErrorCode)
    return ATTServerSendErrorResponseOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTClientSendExchangeMtu(
    device,
    DeviceId=bytearray(1),
    Params_ClientRxMtu=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTClientSendExchangeMtuRequestRequest(DeviceId, Params_ClientRxMtu)
    return ATTClientSendExchangeMtuRequestOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTServerSendExchangeMtuResponse(
    device,
    DeviceId=bytearray(1),
    Params_ServerRxMtu=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTServerSendExchangeMtuResponseRequest(DeviceId, Params_ServerRxMtu)
    return ATTServerSendExchangeMtuResponseOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTClientSendFindInformation(
    device,
    DeviceId=bytearray(1),
    Params_StartingHandle=bytearray(2),
    Params_EndingHandle=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTClientSendFindInformationRequestRequest(DeviceId, Params_StartingHandle, Params_EndingHandle)
    return ATTClientSendFindInformationRequestOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTServerSendFindInformationResponse(
    device,
    DeviceId=bytearray(1),
    Params_Format=ATTServerSendFindInformationResponseRequestParams_Format(),
    # Array length depends on Format. Mask: 0xFF. Shift: 0x00.
    Params_InformationData=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTServerSendFindInformationResponseRequest(DeviceId, Params_Format, Params_InformationData)
    return ATTServerSendFindInformationResponseOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTClientSendFindByTypeValue(
    device,
    DeviceId=bytearray(1),
    Params_StartingHandle=bytearray(2),
    Params_EndingHandle=bytearray(2),
    Params_AttributeType=bytearray(2),
    Params_AttributeLength=bytearray(2),
    # Array length depends on AttributeLength. Mask: 0xFF. Shift: 0x00.
    Params_AttributeValue=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTClientSendFindByTypeValueRequestRequest(DeviceId, Params_StartingHandle, Params_EndingHandle, Params_AttributeType, Params_AttributeLength, Params_AttributeValue)
    return ATTClientSendFindByTypeValueRequestOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTServerSendFindByTypeValueResponse(
    device,
    DeviceId=bytearray(1),
    Params_GroupCount=bytearray(2),
    # Array length depends on GroupCount. Mask: 0xFFFF. Shift: 0x0000.
    Params_HandleGroup=ATTServerSendFindByTypeValueResponseRequest.Params_HandleGroup(),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTServerSendFindByTypeValueResponseRequest(DeviceId, Params_GroupCount, Params_HandleGroup)
    return ATTServerSendFindByTypeValueResponseOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTClientSendReadByType(
    device,
    DeviceId=bytearray(1),
    Params_StartingHandle=bytearray(2),
    Params_EndingHandle=bytearray(2),
    Params_Format=ATTClientSendReadByTypeRequestRequestParams_Format(),
    # Array length depends on Format. Mask: 0xFF. Shift: 0x00.
    Params_AttributeType=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTClientSendReadByTypeRequestRequest(DeviceId, Params_StartingHandle, Params_EndingHandle, Params_Format, Params_AttributeType)
    return ATTClientSendReadByTypeRequestOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTServerSendReadByTypeResponse(
    device,
    DeviceId=bytearray(1),
    Params_Length=bytearray(1),
    Params_AttributeDataListLength=bytearray(2),
    # Array length depends on AttributeDataListLength. Mask: 0xFFFF. Shift: 0x0000.
    Params_AttributeDataList=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTServerSendReadByTypeResponseRequest(DeviceId, Params_Length, Params_AttributeDataListLength, Params_AttributeDataList)
    return ATTServerSendReadByTypeResponseOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTClientSendRead(
    device,
    DeviceId=bytearray(1),
    Params_AttributeHandle=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTClientSendReadRequestRequest(DeviceId, Params_AttributeHandle)
    return ATTClientSendReadRequestOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTServerSendReadResponse(
    device,
    DeviceId=bytearray(1),
    Params_AttributeLength=bytearray(2),
    # Array length depends on AttributeLength. Mask: 0xFFFF. Shift: 0x0000.
    Params_AttributeValue=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTServerSendReadResponseRequest(DeviceId, Params_AttributeLength, Params_AttributeValue)
    return ATTServerSendReadResponseOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTClientSendReadBlob(
    device,
    DeviceId=bytearray(1),
    Params_AttributeHandle=bytearray(2),
    Params_ValueOffset=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTClientSendReadBlobRequestRequest(DeviceId, Params_AttributeHandle, Params_ValueOffset)
    return ATTClientSendReadBlobRequestOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTServerSendReadBlobResponse(
    device,
    DeviceId=bytearray(1),
    Params_AttributeLength=bytearray(2),
    # Array length depends on AttributeLength. Mask: 0xFFFF. Shift: 0x0000.
    Params_AttributeValue=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTServerSendReadBlobResponseRequest(DeviceId, Params_AttributeLength, Params_AttributeValue)
    return ATTServerSendReadBlobResponseOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTClientSendReadMultiple(
    device,
    DeviceId=bytearray(1),
    Params_HandleCount=bytearray(2),
    # Array length depends on HandleCount. Mask: 0xFFFF. Shift: 0x0000.
    Params_ListOfHandles=ATTClientSendReadMultipleRequestRequest.Params_ListOfHandles(),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTClientSendReadMultipleRequestRequest(DeviceId, Params_HandleCount, Params_ListOfHandles)
    return ATTClientSendReadMultipleRequestOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTServerSendReadMultipleResponse(
    device,
    DeviceId=bytearray(1),
    Params_ListLength=bytearray(2),
    # Array length depends on ListLength. Mask: 0xFFFF. Shift: 0x0000.
    Params_ListOfValues=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTServerSendReadMultipleResponseRequest(DeviceId, Params_ListLength, Params_ListOfValues)
    return ATTServerSendReadMultipleResponseOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTClientSendReadByGroupType(
    device,
    DeviceId=bytearray(1),
    Params_StartingHandle=bytearray(2),
    Params_EndingHandle=bytearray(2),
    Params_Format=ATTClientSendReadByGroupTypeRequestRequestParams_Format(),
    # Array length depends on Format. Mask: 0xFF. Shift: 0x00.
    Params_AttributeType=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTClientSendReadByGroupTypeRequestRequest(DeviceId, Params_StartingHandle, Params_EndingHandle, Params_Format, Params_AttributeType)
    return ATTClientSendReadByGroupTypeRequestOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTServerSendReadByGroupTypeResponse(
    device,
    DeviceId=bytearray(1),
    Params_Length=bytearray(1),
    Params_AttributeDataListLength=bytearray(2),
    # Array length depends on AttributeDataListLength. Mask: 0xFFFF. Shift: 0x0000.
    Params_AttributeDataList=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTServerSendReadByGroupTypeResponseRequest(DeviceId, Params_Length, Params_AttributeDataListLength, Params_AttributeDataList)
    return ATTServerSendReadByGroupTypeResponseOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTClientSendWrite(
    device,
    DeviceId=bytearray(1),
    Params_AttributeHandle=bytearray(2),
    Params_AttributeLength=bytearray(2),
    # Array length depends on AttributeLength. Mask: 0xFFFF. Shift: 0x0000.
    Params_AttributeValue=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTClientSendWriteRequestRequest(DeviceId, Params_AttributeHandle, Params_AttributeLength, Params_AttributeValue)
    return ATTClientSendWriteRequestOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTServerSendWriteResponse(
    device,
    DeviceId=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTServerSendWriteResponseRequest(DeviceId)
    return ATTServerSendWriteResponseOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTClientSendWriteCommand(
    device,
    DeviceId=bytearray(1),
    Params_AttributeHandle=bytearray(2),
    Params_AttributeLength=bytearray(2),
    # Array length depends on AttributeLength. Mask: 0xFFFF. Shift: 0x0000.
    Params_AttributeValue=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTClientSendWriteCommandRequest(DeviceId, Params_AttributeHandle, Params_AttributeLength, Params_AttributeValue)
    return ATTClientSendWriteCommandOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTClientSendSignedWriteCommand(
    device,
    DeviceId=bytearray(1),
    Params_AttributeHandle=bytearray(2),
    Params_AttributeLength=bytearray(2),
    # Array length depends on AttributeLength. Mask: 0xFFFF. Shift: 0x0000.
    Params_AttributeValue=[],
    # Unit length: 12 bytes
    Params_AuthenticationSignature=bytearray(12),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTClientSendSignedWriteCommandRequest(DeviceId, Params_AttributeHandle, Params_AttributeLength, Params_AttributeValue, Params_AuthenticationSignature)
    return ATTClientSendSignedWriteCommandOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTClientSendPrepareWrite(
    device,
    DeviceId=bytearray(1),
    Params_AttributeHandle=bytearray(2),
    Params_ValueOffset=bytearray(2),
    Params_AttributeLength=bytearray(2),
    # Array length depends on AttributeLength. Mask: 0xFFFF. Shift: 0x0000.
    Params_AttributeValue=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTClientSendPrepareWriteRequestRequest(DeviceId, Params_AttributeHandle, Params_ValueOffset, Params_AttributeLength, Params_AttributeValue)
    return ATTClientSendPrepareWriteRequestOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTServerSendPrepareWriteResponse(
    device,
    DeviceId=bytearray(1),
    Params_AttributeHandle=bytearray(2),
    Params_ValueOffset=bytearray(2),
    Params_AttributeLength=bytearray(2),
    # Array length depends on AttributeLength. Mask: 0xFFFF. Shift: 0x0000.
    Params_AttributeValue=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTServerSendPrepareWriteResponseRequest(DeviceId, Params_AttributeHandle, Params_ValueOffset, Params_AttributeLength, Params_AttributeValue)
    return ATTServerSendPrepareWriteResponseOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTClientSendExecuteWrite(
    device,
    DeviceId=bytearray(1),
    Params_Flags=ATTClientSendExecuteWriteRequestRequestParams_Flags.gCancel_c,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTClientSendExecuteWriteRequestRequest(DeviceId, Params_Flags)
    return ATTClientSendExecuteWriteRequestOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTServerSendExecuteWriteResponse(
    device,
    DeviceId=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTServerSendExecuteWriteResponseRequest(DeviceId)
    return ATTServerSendExecuteWriteResponseOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTServerSendHandleValueNotification(
    device,
    DeviceId=bytearray(1),
    Params_AttributeHandle=bytearray(2),
    Params_AttributeLength=bytearray(2),
    # Array length depends on AttributeLength. Mask: 0xFFFF. Shift: 0x0000.
    Params_AttributeValue=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTServerSendHandleValueNotificationRequest(DeviceId, Params_AttributeHandle, Params_AttributeLength, Params_AttributeValue)
    return ATTServerSendHandleValueNotificationOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTServerSendHandleValueIndication(
    device,
    DeviceId=bytearray(1),
    Params_AttributeHandle=bytearray(2),
    Params_AttributeLength=bytearray(2),
    # Array length depends on AttributeLength. Mask: 0xFFFF. Shift: 0x0000.
    Params_AttributeValue=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTServerSendHandleValueIndicationRequest(DeviceId, Params_AttributeHandle, Params_AttributeLength, Params_AttributeValue)
    return ATTServerSendHandleValueIndicationOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ATTClientSendHandleValueConfirmation(
    device,
    DeviceId=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.ATTClientSendHandleValueConfirmationRequest(DeviceId)
    return ATTClientSendHandleValueConfirmationOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTModeSelect(
    device,
    Enable=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTModeSelectRequest(Enable)
    return GATTModeSelectOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTInit(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTInitRequest()
    return GATTInitOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTGetMtu(
    device,
    DeviceId=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTGetMtuRequest(DeviceId)
    return GATTGetMtuOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTClientInit(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTClientInitRequest()
    return GATTClientInitOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTClientResetProcedure(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTClientResetProcedureRequest()
    return GATTClientResetProcedureOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTClientRegisterProcedureCallback(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTClientRegisterProcedureCallbackRequest()
    return GATTClientRegisterProcedureCallbackOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTClientRegisterNotificationCallback(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTClientRegisterNotificationCallbackRequest()
    return GATTClientRegisterNotificationCallbackOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTClientRegisterIndicationCallback(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTClientRegisterIndicationCallbackRequest()
    return GATTClientRegisterIndicationCallbackOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTClientExchangeMtu(
    device,
    DeviceId=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTClientExchangeMtuRequest(DeviceId)
    return GATTClientExchangeMtuOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTClientDiscoverAllPrimaryServices(
    device,
    DeviceId=bytearray(1),
    MaxNbOfServices=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTClientDiscoverAllPrimaryServicesRequest(DeviceId, MaxNbOfServices)
    return GATTClientDiscoverAllPrimaryServicesOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTClientDiscoverPrimaryServicesByUuid(
    device,
    DeviceId=bytearray(1),
    UuidType=GATTClientDiscoverPrimaryServicesByUuidRequestUuidType.Uuid16Bits,
    # Array length depends on UuidType. Mask: 0xFF. Shift: 0x00.
    Uuid=[],
    MaxNbOfServices=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTClientDiscoverPrimaryServicesByUuidRequest(DeviceId, UuidType, Uuid, MaxNbOfServices)
    return GATTClientDiscoverPrimaryServicesByUuidOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTClientFindIncludedServices(
    device,
    DeviceId=bytearray(1),
    Service_StartHandle=bytearray(2),
    Service_EndHandle=bytearray(2),
    Service_UuidType=GATTClientFindIncludedServicesRequestService_UuidType.Uuid16Bits,
    # Array length depends on UuidType. Mask: 0xFF. Shift: 0x00.
    Service_Uuid=[],
    Service_NbOfCharacteristics=bytearray(1),
    # Array length depends on NbOfCharacteristics. Mask: 0xFF. Shift: 0x00.
    Service_Characteristics=GATTClientFindIncludedServicesRequest.Service_Characteristics(),
    Service_NbOfIncludedServices=bytearray(1),
    # Array length depends on NbOfIncludedServices. Mask: 0xFF. Shift: 0x00.
    Service_IncludedServices=GATTClientFindIncludedServicesRequest.Service_IncludedServices(),
    MaxNbOfIncludedServices=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTClientFindIncludedServicesRequest(DeviceId, Service_StartHandle, Service_EndHandle, Service_UuidType, Service_Uuid, Service_NbOfCharacteristics, Service_Characteristics, Service_NbOfIncludedServices, Service_IncludedServices, MaxNbOfIncludedServices)
    return GATTClientFindIncludedServicesOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTClientDiscoverAllCharacteristicsOfService(
    device,
    DeviceId=bytearray(1),
    Service_StartHandle=bytearray(2),
    Service_EndHandle=bytearray(2),
    Service_UuidType=GATTClientDiscoverAllCharacteristicsOfServiceRequestService_UuidType.Uuid16Bits,
    # Array length depends on UuidType. Mask: 0xFF. Shift: 0x00.
    Service_Uuid=[],
    Service_NbOfCharacteristics=bytearray(1),
    # Array length depends on NbOfCharacteristics. Mask: 0xFF. Shift: 0x00.
    Service_Characteristics=GATTClientDiscoverAllCharacteristicsOfServiceRequest.Service_Characteristics(),
    Service_NbOfIncludedServices=bytearray(1),
    # Array length depends on NbOfIncludedServices. Mask: 0xFF. Shift: 0x00.
    Service_IncludedServices=GATTClientDiscoverAllCharacteristicsOfServiceRequest.Service_IncludedServices(),
    MaxNbOfCharacteristics=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTClientDiscoverAllCharacteristicsOfServiceRequest(DeviceId, Service_StartHandle, Service_EndHandle, Service_UuidType, Service_Uuid, Service_NbOfCharacteristics, Service_Characteristics, Service_NbOfIncludedServices, Service_IncludedServices, MaxNbOfCharacteristics)
    return GATTClientDiscoverAllCharacteristicsOfServiceOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTClientDiscoverCharacteristicOfServiceByUuid(
    device,
    DeviceId=bytearray(1),
    UuidType=GATTClientDiscoverCharacteristicOfServiceByUuidRequestUuidType.Uuid16Bits,
    # Array length depends on UuidType. Mask: 0xFF. Shift: 0x00.
    Uuid=[],
    Service_StartHandle=bytearray(2),
    Service_EndHandle=bytearray(2),
    Service_UuidType=GATTClientDiscoverCharacteristicOfServiceByUuidRequestService_UuidType.Uuid16Bits,
    # Array length depends on UuidType. Mask: 0xFF. Shift: 0x00.
    Service_Uuid=[],
    Service_NbOfCharacteristics=bytearray(1),
    # Array length depends on NbOfCharacteristics. Mask: 0xFF. Shift: 0x00.
    Service_Characteristics=GATTClientDiscoverCharacteristicOfServiceByUuidRequest.Service_Characteristics(),
    Service_NbOfIncludedServices=bytearray(1),
    # Array length depends on NbOfIncludedServices. Mask: 0xFF. Shift: 0x00.
    Service_IncludedServices=GATTClientDiscoverCharacteristicOfServiceByUuidRequest.Service_IncludedServices(),
    MaxNbOfCharacteristics=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTClientDiscoverCharacteristicOfServiceByUuidRequest(DeviceId, UuidType, Uuid, Service_StartHandle, Service_EndHandle, Service_UuidType, Service_Uuid, Service_NbOfCharacteristics, Service_Characteristics, Service_NbOfIncludedServices, Service_IncludedServices, MaxNbOfCharacteristics)
    return GATTClientDiscoverCharacteristicOfServiceByUuidOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTClientDiscoverAllCharacteristicDescriptors(
    device,
    DeviceId=bytearray(1),
    Characteristic_Properties=GATTClientDiscoverAllCharacteristicDescriptorsRequestCharacteristic_Properties.gNone_c,
    Characteristic_Value=GATTClientDiscoverAllCharacteristicDescriptorsRequest.Characteristic_Value(),
    Characteristic_NbOfDescriptors=bytearray(1),
    # Array length depends on NbOfDescriptors. Mask: 0xFF. Shift: 0x00.
    Characteristic_Descriptors=GATTClientDiscoverAllCharacteristicDescriptorsRequest.Characteristic_Descriptors(),
    EndingHandle=bytearray(2),
    MaxNbOfDescriptors=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTClientDiscoverAllCharacteristicDescriptorsRequest(DeviceId, Characteristic_Properties, Characteristic_Value, Characteristic_NbOfDescriptors, Characteristic_Descriptors, EndingHandle, MaxNbOfDescriptors)
    return GATTClientDiscoverAllCharacteristicDescriptorsOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTClientReadCharacteristicValue(
    device,
    DeviceId=bytearray(1),
    Characteristic_Properties=GATTClientReadCharacteristicValueRequestCharacteristic_Properties.gNone_c,
    Characteristic_Value=GATTClientReadCharacteristicValueRequest.Characteristic_Value(),
    Characteristic_NbOfDescriptors=bytearray(1),
    # Array length depends on NbOfDescriptors. Mask: 0xFF. Shift: 0x00.
    Characteristic_Descriptors=GATTClientReadCharacteristicValueRequest.Characteristic_Descriptors(),
    MaxReadBytes=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTClientReadCharacteristicValueRequest(DeviceId, Characteristic_Properties, Characteristic_Value, Characteristic_NbOfDescriptors, Characteristic_Descriptors, MaxReadBytes)
    return GATTClientReadCharacteristicValueOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTClientReadUsingCharacteristicUuid(
    device,
    DeviceId=bytearray(1),
    UuidType=GATTClientReadUsingCharacteristicUuidRequestUuidType.Uuid16Bits,
    # Array length depends on UuidType. Mask: 0xFF. Shift: 0x00.
    Uuid=[],
    HandleRangeIncluded=bytearray(1),
    # Array length depends on HandleRangeIncluded. Mask: 0xFF. Shift: 0x00.
    HandleRange=GATTClientReadUsingCharacteristicUuidRequest.HandleRange(),
    MaxReadBytes=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTClientReadUsingCharacteristicUuidRequest(DeviceId, UuidType, Uuid, HandleRangeIncluded, HandleRange, MaxReadBytes)
    return GATTClientReadUsingCharacteristicUuidOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTClientReadMultipleCharacteristicValues(
    device,
    DeviceId=bytearray(1),
    NbOfCharacteristics=bytearray(1),
    # Array length depends on NbOfCharacteristics. Mask: 0xFF. Shift: 0x00.
    Characteristics=GATTClientReadMultipleCharacteristicValuesRequest.Characteristics(),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTClientReadMultipleCharacteristicValuesRequest(DeviceId, NbOfCharacteristics, Characteristics)
    return GATTClientReadMultipleCharacteristicValuesOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTClientWriteCharacteristicValue(
    device,
    DeviceId=bytearray(1),
    Characteristic_Properties=GATTClientWriteCharacteristicValueRequestCharacteristic_Properties.gNone_c,
    Characteristic_Value=GATTClientWriteCharacteristicValueRequest.Characteristic_Value(),
    Characteristic_NbOfDescriptors=bytearray(1),
    # Array length depends on NbOfDescriptors. Mask: 0xFF. Shift: 0x00.
    Characteristic_Descriptors=GATTClientWriteCharacteristicValueRequest.Characteristic_Descriptors(),
    ValueLength=bytearray(2),
    # Array length depends on ValueLength. Mask: 0xFFFF. Shift: 0x0000.
    Value=[],
    WithoutResponse=bytearray(1),
    SignedWrite=bytearray(1),
    ReliableLongCharWrites=bytearray(1),
    # Unit length: 16 bytes
    Csrk=bytearray(16),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTClientWriteCharacteristicValueRequest(DeviceId, Characteristic_Properties, Characteristic_Value, Characteristic_NbOfDescriptors, Characteristic_Descriptors, ValueLength, Value, WithoutResponse, SignedWrite, ReliableLongCharWrites, Csrk)
    return GATTClientWriteCharacteristicValueOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTClientReadCharacteristicDescriptor(
    device,
    DeviceId=bytearray(1),
    Descriptor_Handle=bytearray(2),
    Descriptor_UuidType=GATTClientReadCharacteristicDescriptorRequestDescriptor_UuidType.Uuid16Bits,
    # Array length depends on UuidType. Mask: 0xFF. Shift: 0x00.
    Descriptor_Uuid=[],
    Descriptor_ValueLength=bytearray(2),
    Descriptor_MaxValueLength=bytearray(2),
    # Array length depends on ValueLength. Mask: 0xFFFF. Shift: 0x0000.
    Descriptor_Value=[],
    MaxReadBytes=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTClientReadCharacteristicDescriptorRequest(DeviceId, Descriptor_Handle, Descriptor_UuidType, Descriptor_Uuid, Descriptor_ValueLength, Descriptor_MaxValueLength, Descriptor_Value, MaxReadBytes)
    return GATTClientReadCharacteristicDescriptorOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTClientWriteCharacteristicDescriptor(
    device,
    DeviceId=bytearray(1),
    Descriptor_Handle=bytearray(2),
    Descriptor_UuidType=GATTClientWriteCharacteristicDescriptorRequestDescriptor_UuidType.Uuid16Bits,
    # Array length depends on UuidType. Mask: 0xFF. Shift: 0x00.
    Descriptor_Uuid=[],
    Descriptor_ValueLength=bytearray(2),
    Descriptor_MaxValueLength=bytearray(2),
    # Array length depends on ValueLength. Mask: 0xFFFF. Shift: 0x0000.
    Descriptor_Value=[],
    ValueLength=bytearray(2),
    # Array length depends on ValueLength. Mask: 0xFFFF. Shift: 0x0000.
    Value=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTClientWriteCharacteristicDescriptorRequest(DeviceId, Descriptor_Handle, Descriptor_UuidType, Descriptor_Uuid, Descriptor_ValueLength, Descriptor_MaxValueLength, Descriptor_Value, ValueLength, Value)
    return GATTClientWriteCharacteristicDescriptorOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTServerInit(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTServerInitRequest()
    return GATTServerInitOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTServerRegisterCallback(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTServerRegisterCallbackRequest()
    return GATTServerRegisterCallbackOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTServerRegisterHandlesForWriteNotifications(
    device,
    HandleCount=bytearray(1),
    # Array length depends on HandleCount. Mask: 0xFF. Shift: 0x00.
    AttributeHandles=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTServerRegisterHandlesForWriteNotificationsRequest(HandleCount, AttributeHandles)
    return GATTServerRegisterHandlesForWriteNotificationsOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTServerSendAttributeWrittenStatus(
    device,
    DeviceId=bytearray(1),
    AttributeHandle=bytearray(2),
    Status=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTServerSendAttributeWrittenStatusRequest(DeviceId, AttributeHandle, Status)
    return GATTServerSendAttributeWrittenStatusOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTServerSendNotification(
    device,
    DeviceId=bytearray(1),
    Handle=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTServerSendNotificationRequest(DeviceId, Handle)
    return GATTServerSendNotificationOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTServerSendIndication(
    device,
    DeviceId=bytearray(1),
    Handle=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTServerSendIndicationRequest(DeviceId, Handle)
    return GATTServerSendIndicationOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTServerSendInstantValueNotification(
    device,
    DeviceId=bytearray(1),
    Handle=bytearray(2),
    ValueLength=bytearray(2),
    # Array length depends on ValueLength. Mask: 0xFFFF. Shift: 0x0000.
    Value=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTServerSendInstantValueNotificationRequest(DeviceId, Handle, ValueLength, Value)
    return GATTServerSendInstantValueNotificationOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTServerSendInstantValueIndication(
    device,
    DeviceId=bytearray(1),
    Handle=bytearray(2),
    ValueLength=bytearray(2),
    # Array length depends on ValueLength. Mask: 0xFFFF. Shift: 0x0000.
    Value=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTServerSendInstantValueIndicationRequest(DeviceId, Handle, ValueLength, Value)
    return GATTServerSendInstantValueIndicationOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTServerRegisterHandlesForReadNotifications(
    device,
    HandleCount=bytearray(1),
    # Array length depends on HandleCount. Mask: 0xFF. Shift: 0x00.
    AttributeHandles=GATTServerRegisterHandlesForReadNotificationsRequest.AttributeHandles(),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTServerRegisterHandlesForReadNotificationsRequest(HandleCount, AttributeHandles)
    return GATTServerRegisterHandlesForReadNotificationsOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTServerSendAttributeReadStatus(
    device,
    DeviceId=bytearray(1),
    AttributeHandle=bytearray(2),
    Status=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTServerSendAttributeReadStatusRequest(DeviceId, AttributeHandle, Status)
    return GATTServerSendAttributeReadStatusOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTDBModeSelect(
    device,
    Enable=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTDBModeSelectRequest(Enable)
    return GATTDBModeSelectOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTDBInit(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTDBInitRequest()
    return GATTDBInitOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTDBWriteAttribute(
    device,
    Handle=bytearray(2),
    ValueLength=bytearray(2),
    # Array length depends on ValueLength. Mask: 0xFF. Shift: 0x00.
    Value=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTDBWriteAttributeRequest(Handle, ValueLength, Value)
    return GATTDBWriteAttributeOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTDBReadAttribute(
    device,
    Handle=bytearray(2),
    MaxBytes=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTDBReadAttributeRequest(Handle, MaxBytes)
    return GATTDBReadAttributeOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTDBFindServiceHandle(
    device,
    StartHandle=bytearray(2),
    UuidType=GATTDBFindServiceHandleRequestUuidType.Uuid16Bits,
    # Array length depends on UuidType. Mask: 0xFF. Shift: 0x00.
    Uuid=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTDBFindServiceHandleRequest(StartHandle, UuidType, Uuid)
    return GATTDBFindServiceHandleOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTDBFindCharValueHandleInService(
    device,
    ServiceHandle=bytearray(2),
    UuidType=GATTDBFindCharValueHandleInServiceRequestUuidType.Uuid16Bits,
    # Array length depends on UuidType. Mask: 0xFF. Shift: 0x00.
    Uuid=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTDBFindCharValueHandleInServiceRequest(ServiceHandle, UuidType, Uuid)
    return GATTDBFindCharValueHandleInServiceOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTDBFindCccdHandleForCharValueHandle(
    device,
    CharValueHandle=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTDBFindCccdHandleForCharValueHandleRequest(CharValueHandle)
    return GATTDBFindCccdHandleForCharValueHandleOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTDBFindDescriptorHandleForCharValueHandle(
    device,
    CharValueHandle=bytearray(2),
    UuidType=GATTDBFindDescriptorHandleForCharValueHandleRequestUuidType.Uuid16Bits,
    # Array length depends on UuidType. Mask: 0xFF. Shift: 0x00.
    Uuid=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTDBFindDescriptorHandleForCharValueHandleRequest(CharValueHandle, UuidType, Uuid)
    return GATTDBFindDescriptorHandleForCharValueHandleOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTDBDynamicInit(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTDBDynamicInitRequest()
    return GATTDBDynamicInitOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTDBDynamicReleaseDatabase(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTDBDynamicReleaseDatabaseRequest()
    return GATTDBDynamicReleaseDatabaseOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTDBDynamicAddPrimaryServiceDeclaration(
    device,
    UuidType=GATTDBDynamicAddPrimaryServiceDeclarationRequestUuidType.Uuid16Bits,
    # Array length depends on UuidType. Mask: 0xFF. Shift: 0x00.
    Uuid=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTDBDynamicAddPrimaryServiceDeclarationRequest(UuidType, Uuid)
    return GATTDBDynamicAddPrimaryServiceDeclarationOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTDBDynamicAddSecondaryServiceDeclaration(
    device,
    UuidType=GATTDBDynamicAddSecondaryServiceDeclarationRequestUuidType.Uuid16Bits,
    # Array length depends on UuidType. Mask: 0xFF. Shift: 0x00.
    Uuid=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTDBDynamicAddSecondaryServiceDeclarationRequest(UuidType, Uuid)
    return GATTDBDynamicAddSecondaryServiceDeclarationOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTDBDynamicAddIncludeDeclaration(
    device,
    IncludedServiceHandle=bytearray(2),
    EndGroupHandle=bytearray(2),
    UuidType=GATTDBDynamicAddIncludeDeclarationRequestUuidType.Uuid16Bits,
    # Array length depends on UuidType. Mask: 0xFF. Shift: 0x00.
    Uuid=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTDBDynamicAddIncludeDeclarationRequest(IncludedServiceHandle, EndGroupHandle, UuidType, Uuid)
    return GATTDBDynamicAddIncludeDeclarationOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTDBDynamicAddCharacteristicDeclarationAndValue(
    device,
    UuidType=GATTDBDynamicAddCharacteristicDeclarationAndValueRequestUuidType.Uuid16Bits,
    # Array length depends on UuidType. Mask: 0xFF. Shift: 0x00.
    Uuid=[],
    CharacteristicProperties=GATTDBDynamicAddCharacteristicDeclarationAndValueRequestCharacteristicProperties.gNone_c,
    MaxValueLength=bytearray(2),
    InitialValueLength=bytearray(2),
    # Array length depends on InitialValueLength. Mask: 0xFFFF. Shift: 0x0000.
    InitialValue=[],
    ValueAccessPermissions=GATTDBDynamicAddCharacteristicDeclarationAndValueRequestValueAccessPermissions.gPermissionNone_c,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTDBDynamicAddCharacteristicDeclarationAndValueRequest(UuidType, Uuid, CharacteristicProperties, MaxValueLength, InitialValueLength, InitialValue, ValueAccessPermissions)
    return GATTDBDynamicAddCharacteristicDeclarationAndValueOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTDBDynamicAddCharacteristicDescriptor(
    device,
    UuidType=GATTDBDynamicAddCharacteristicDescriptorRequestUuidType.Uuid16Bits,
    # Array length depends on UuidType. Mask: 0xFF. Shift: 0x00.
    Uuid=[],
    DescriptorValueLength=bytearray(2),
    # Array length depends on DescriptorValueLength. Mask: 0xFFFF. Shift: 0x0000.
    Value=[],
    DescriptorAccessPermissions=GATTDBDynamicAddCharacteristicDescriptorRequestDescriptorAccessPermissions.gPermissionNone_c,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTDBDynamicAddCharacteristicDescriptorRequest(UuidType, Uuid, DescriptorValueLength, Value, DescriptorAccessPermissions)
    return GATTDBDynamicAddCharacteristicDescriptorOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTDBDynamicAddCccd(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTDBDynamicAddCccdRequest()
    return GATTDBDynamicAddCccdOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTDBDynamicAddCharacteristicDeclarationWithUniqueValue(
    device,
    UuidType=GATTDBDynamicAddCharacteristicDeclarationWithUniqueValueRequestUuidType.Uuid16Bits,
    # Array length depends on UuidType. Mask: 0xFF. Shift: 0x00.
    Uuid=[],
    CharacteristicProperties=GATTDBDynamicAddCharacteristicDeclarationWithUniqueValueRequestCharacteristicProperties.gNone_c,
    ValueAccessPermissions=GATTDBDynamicAddCharacteristicDeclarationWithUniqueValueRequestValueAccessPermissions.gPermissionNone_c,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTDBDynamicAddCharacteristicDeclarationWithUniqueValueRequest(UuidType, Uuid, CharacteristicProperties, ValueAccessPermissions)
    return GATTDBDynamicAddCharacteristicDeclarationWithUniqueValueOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTDBDynamicRemoveService(
    device,
    ServiceHandle=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTDBDynamicRemoveServiceRequest(ServiceHandle)
    return GATTDBDynamicRemoveServiceOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTDBDynamicRemoveCharacteristic(
    device,
    CharacteristicHandle=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTDBDynamicRemoveCharacteristicRequest(CharacteristicHandle)
    return GATTDBDynamicRemoveCharacteristicOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTDBAttModeSelect(
    device,
    Enable=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTDBAttModeSelectRequest(Enable)
    return GATTDBAttModeSelectOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTDBAttFindInformation(
    device,
    DeviceId=bytearray(1),
    Params_StartingHandle=bytearray(2),
    Params_EndingHandle=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTDBAttFindInformationRequest(DeviceId, Params_StartingHandle, Params_EndingHandle)
    return GATTDBAttFindInformationOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTDBAttFindByTypeValue(
    device,
    DeviceId=bytearray(1),
    Params_StartingHandle=bytearray(2),
    Params_EndingHandle=bytearray(2),
    Params_AttributeType=bytearray(2),
    Params_AttributeLength=bytearray(2),
    # Array length depends on AttributeLength. Mask: 0xFF. Shift: 0x00.
    Params_AttributeValue=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTDBAttFindByTypeValueRequest(DeviceId, Params_StartingHandle, Params_EndingHandle, Params_AttributeType, Params_AttributeLength, Params_AttributeValue)
    return GATTDBAttFindByTypeValueOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTDBAttReadByType(
    device,
    DeviceId=bytearray(1),
    Params_StartingHandle=bytearray(2),
    Params_EndingHandle=bytearray(2),
    Params_Format=GATTDBAttReadByTypeRequestParams_Format(),
    # Array length depends on Format. Mask: 0xFF. Shift: 0x00.
    Params_AttributeType=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTDBAttReadByTypeRequest(DeviceId, Params_StartingHandle, Params_EndingHandle, Params_Format, Params_AttributeType)
    return GATTDBAttReadByTypeOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTDBAttRead(
    device,
    DeviceId=bytearray(1),
    Params_AttributeHandle=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTDBAttReadRequest(DeviceId, Params_AttributeHandle)
    return GATTDBAttReadOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTDBAttReadBlob(
    device,
    DeviceId=bytearray(1),
    Params_AttributeHandle=bytearray(2),
    Params_ValueOffset=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTDBAttReadBlobRequest(DeviceId, Params_AttributeHandle, Params_ValueOffset)
    return GATTDBAttReadBlobOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTDBAttReadMultiple(
    device,
    DeviceId=bytearray(1),
    Params_HandleCount=bytearray(2),
    # Array length depends on HandleCount. Mask: 0xFFFF. Shift: 0x0000.
    Params_ListOfHandles=GATTDBAttReadMultipleRequest.Params_ListOfHandles(),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTDBAttReadMultipleRequest(DeviceId, Params_HandleCount, Params_ListOfHandles)
    return GATTDBAttReadMultipleOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTDBAttReadByGroupType(
    device,
    DeviceId=bytearray(1),
    Params_StartingHandle=bytearray(2),
    Params_EndingHandle=bytearray(2),
    Params_Format=GATTDBAttReadByGroupTypeRequestParams_Format(),
    # Array length depends on Format. Mask: 0xFF. Shift: 0x00.
    Params_AttributeType=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTDBAttReadByGroupTypeRequest(DeviceId, Params_StartingHandle, Params_EndingHandle, Params_Format, Params_AttributeType)
    return GATTDBAttReadByGroupTypeOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTDBAttWrite(
    device,
    DeviceId=bytearray(1),
    Params_AttributeHandle=bytearray(2),
    Params_AttributeLength=bytearray(2),
    # Array length depends on AttributeLength. Mask: 0xFFFF. Shift: 0x0000.
    Params_AttributeValue=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTDBAttWriteRequest(DeviceId, Params_AttributeHandle, Params_AttributeLength, Params_AttributeValue)
    return GATTDBAttWriteOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTDBAttWriteCommand(
    device,
    DeviceId=bytearray(1),
    Params_AttributeHandle=bytearray(2),
    Params_AttributeLength=bytearray(2),
    # Array length depends on AttributeLength. Mask: 0xFFFF. Shift: 0x0000.
    Params_AttributeValue=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTDBAttWriteCommandRequest(DeviceId, Params_AttributeHandle, Params_AttributeLength, Params_AttributeValue)
    return GATTDBAttWriteCommandOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTDBAttSignedWriteCommand(
    device,
    DeviceId=bytearray(1),
    Params_AttributeHandle=bytearray(2),
    Params_AttributeLength=bytearray(2),
    # Array length depends on AttributeLength. Mask: 0xFFFF. Shift: 0x0000.
    Params_AttributeValue=[],
    # Unit length: 12 bytes
    Params_AuthenticationSignature=bytearray(12),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTDBAttSignedWriteCommandRequest(DeviceId, Params_AttributeHandle, Params_AttributeLength, Params_AttributeValue, Params_AuthenticationSignature)
    return GATTDBAttSignedWriteCommandOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTDBAttPrepareWrite(
    device,
    DeviceId=bytearray(1),
    Params_AttributeHandle=bytearray(2),
    Params_ValueOffset=bytearray(2),
    Params_AttributeLength=bytearray(2),
    # Array length depends on AttributeLength. Mask: 0xFFFF. Shift: 0x0000.
    Params_AttributeValue=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTDBAttPrepareWriteRequest(DeviceId, Params_AttributeHandle, Params_ValueOffset, Params_AttributeLength, Params_AttributeValue)
    return GATTDBAttPrepareWriteOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTDBAttExecuteWrite(
    device,
    DeviceId=bytearray(1),
    Params_Flags=GATTDBAttExecuteWriteRequestParams_Flags.gCancel_c,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTDBAttExecuteWriteRequest(DeviceId, Params_Flags)
    return GATTDBAttExecuteWriteOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTDBAttExecuteWriteFromQueue(
    device,
    DeviceId=bytearray(1),
    Params_AttributeHandle=bytearray(2),
    Params_ValueOffset=bytearray(2),
    Params_AttributeLength=bytearray(2),
    # Array length depends on AttributeLength. Mask: 0xFFFF. Shift: 0x0000.
    Params_AttributeValue=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTDBAttExecuteWriteFromQueueRequest(DeviceId, Params_AttributeHandle, Params_ValueOffset, Params_AttributeLength, Params_AttributeValue)
    return GATTDBAttExecuteWriteFromQueueOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GATTDBAttPrepareNotificationIndication(
    device,
    DeviceId=bytearray(1),
    Params_AttributeHandle=bytearray(2),
    Params_AttributeLength=bytearray(2),
    # Array length depends on AttributeLength. Mask: 0xFFFF. Shift: 0x0000.
    Params_AttributeValue=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GATTDBAttPrepareNotificationIndicationRequest(DeviceId, Params_AttributeHandle, Params_AttributeLength, Params_AttributeValue)
    return GATTDBAttPrepareNotificationIndicationOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPModeSelect(
    device,
    Enable=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPModeSelectRequest(Enable)
    return GAPModeSelectOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def BLEHostInitialize(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.BLEHostInitializeRequest()
    return BLEHostInitializeOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPRegisterDeviceSecurityRequirements(
    device,
    SecurityRequirementsIncluded=bytearray(1),
    # Array length depends on SecurityRequirementsIncluded. Mask: 0xFF. Shift: 0x00.
    SecurityRequirements=GAPRegisterDeviceSecurityRequirementsRequest.SecurityRequirements(),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPRegisterDeviceSecurityRequirementsRequest(SecurityRequirementsIncluded, SecurityRequirements)
    return GAPRegisterDeviceSecurityRequirementsOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPSetAdvertisingParameters(
    device,
    AdvertisingParameters_MinInterval=bytearray(2),
    AdvertisingParameters_MaxInterval=bytearray(2),
    AdvertisingParameters_AdvertisingType=GAPSetAdvertisingParametersRequestAdvertisingParameters_AdvertisingType.gConnectableUndirected_c,
    AdvertisingParameters_OwnAddressType=GAPSetAdvertisingParametersRequestAdvertisingParameters_OwnAddressType.gPublic_c,
    AdvertisingParameters_PeerAddressType=GAPSetAdvertisingParametersRequestAdvertisingParameters_PeerAddressType.gPublic_c,
    # Unit length: 6 bytes
    AdvertisingParameters_PeerAddress=bytearray(6),
    AdvertisingParameters_ChannelMap=GAPSetAdvertisingParametersRequestAdvertisingParameters_ChannelMap.gChannel37_c,
    AdvertisingParameters_FilterPolicy=GAPSetAdvertisingParametersRequestAdvertisingParameters_FilterPolicy.gProcessAll_c,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPSetAdvertisingParametersRequest(AdvertisingParameters_MinInterval, AdvertisingParameters_MaxInterval, AdvertisingParameters_AdvertisingType, AdvertisingParameters_OwnAddressType, AdvertisingParameters_PeerAddressType, AdvertisingParameters_PeerAddress, AdvertisingParameters_ChannelMap, AdvertisingParameters_FilterPolicy)
    return GAPSetAdvertisingParametersOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPSetAdvertisingData(
    device,
    AdvertisingDataIncluded=bytearray(1),
    # Array length depends on AdvertisingDataIncluded. Mask: 0xFF. Shift: 0x00.
    AdvertisingData=GAPSetAdvertisingDataRequest.AdvertisingData(),
    ScanResponseDataIncluded=bytearray(1),
    # Array length depends on ScanResponseDataIncluded. Mask: 0xFF. Shift: 0x00.
    ScanResponseData=GAPSetAdvertisingDataRequest.ScanResponseData(),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPSetAdvertisingDataRequest(AdvertisingDataIncluded, AdvertisingData, ScanResponseDataIncluded, ScanResponseData)
    return GAPSetAdvertisingDataOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPStartAdvertising(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPStartAdvertisingRequest()
    return GAPStartAdvertisingOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPStopAdvertising(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPStopAdvertisingRequest()
    return GAPStopAdvertisingOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPAuthorize(
    device,
    DeviceId=bytearray(1),
    Handle=bytearray(2),
    Access=GAPAuthorizeRequestAccess.gRead_c,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPAuthorizeRequest(DeviceId, Handle, Access)
    return GAPAuthorizeOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPSaveCccd(
    device,
    DeviceId=bytearray(1),
    Handle=bytearray(2),
    Cccd=GAPSaveCccdRequestCccd.gEmpty_c,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPSaveCccdRequest(DeviceId, Handle, Cccd)
    return GAPSaveCccdOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPCheckNotificationStatus(
    device,
    DeviceId=bytearray(1),
    Handle=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPCheckNotificationStatusRequest(DeviceId, Handle)
    return GAPCheckNotificationStatusOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPCheckIndicationStatus(
    device,
    DeviceId=bytearray(1),
    Handle=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPCheckIndicationStatusRequest(DeviceId, Handle)
    return GAPCheckIndicationStatusOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPGetBondedStaticAddresses(
    device,
    MaxNbOfDevices=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPGetBondedStaticAddressesRequest(MaxNbOfDevices)
    return GAPGetBondedStaticAddressesOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPPair(
    device,
    DeviceId=bytearray(1),
    PairingParameters_WithBonding=bytearray(1),
    PairingParameters_SecurityModeAndLevel=GAPPairRequestPairingParameters_SecurityModeAndLevel.gMode1Level1_c,
    PairingParameters_MaxEncryptionKeySize=bytearray(1),
    PairingParameters_LocalIoCapabilities=GAPPairRequestPairingParameters_LocalIoCapabilities.gIoDisplayOnly_c,
    PairingParameters_OobAvailable=bytearray(1),
    PairingParameters_CentralKeys=GAPPairRequestPairingParameters_CentralKeys.gNoKeys_c,
    PairingParameters_PeripheralKeys=GAPPairRequestPairingParameters_PeripheralKeys.gNoKeys_c,
    PairingParameters_LeSecureConnectionSupported=bytearray(1),
    PairingParameters_UseKeypressNotifications=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPPairRequest(DeviceId, PairingParameters_WithBonding, PairingParameters_SecurityModeAndLevel, PairingParameters_MaxEncryptionKeySize, PairingParameters_LocalIoCapabilities, PairingParameters_OobAvailable, PairingParameters_CentralKeys, PairingParameters_PeripheralKeys, PairingParameters_LeSecureConnectionSupported, PairingParameters_UseKeypressNotifications)
    return GAPPairOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPSendSlaveSecurity(
    device,
    DeviceId=bytearray(1),
    BondAfterPairing=bytearray(1),
    SecurityModeLevel=GAPSendSlaveSecurityRequestRequestSecurityModeLevel.gMode1Level1_c,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPSendSlaveSecurityRequestRequest(DeviceId, BondAfterPairing, SecurityModeLevel)
    return GAPSendSlaveSecurityRequestOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPEncryptLink(
    device,
    DeviceId=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPEncryptLinkRequest(DeviceId)
    return GAPEncryptLinkOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPAcceptPairing(
    device,
    DeviceId=bytearray(1),
    PairingParameters_WithBonding=bytearray(1),
    PairingParameters_SecurityModeAndLevel=GAPAcceptPairingRequestRequestPairingParameters_SecurityModeAndLevel.gMode1Level1_c,
    PairingParameters_MaxEncryptionKeySize=bytearray(1),
    PairingParameters_LocalIoCapabilities=GAPAcceptPairingRequestRequestPairingParameters_LocalIoCapabilities.gIoDisplayOnly_c,
    PairingParameters_OobAvailable=bytearray(1),
    PairingParameters_CentralKeys=GAPAcceptPairingRequestRequestPairingParameters_CentralKeys.gNoKeys_c,
    PairingParameters_PeripheralKeys=GAPAcceptPairingRequestRequestPairingParameters_PeripheralKeys.gNoKeys_c,
    PairingParameters_LeSecureConnectionSupported=bytearray(1),
    PairingParameters_UseKeypressNotifications=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPAcceptPairingRequestRequest(DeviceId, PairingParameters_WithBonding, PairingParameters_SecurityModeAndLevel, PairingParameters_MaxEncryptionKeySize, PairingParameters_LocalIoCapabilities, PairingParameters_OobAvailable, PairingParameters_CentralKeys, PairingParameters_PeripheralKeys, PairingParameters_LeSecureConnectionSupported, PairingParameters_UseKeypressNotifications)
    return GAPAcceptPairingRequestOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPRejectPairing(
    device,
    DeviceId=bytearray(1),
    Reason=GAPRejectPairingRequestReason.gOobNotAvailable_c,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPRejectPairingRequest(DeviceId, Reason)
    return GAPRejectPairingOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPEnterPasskey(
    device,
    DeviceId=bytearray(1),
    Passkey=bytearray(4),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPEnterPasskeyRequest(DeviceId, Passkey)
    return GAPEnterPasskeyOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPProvideOob(
    device,
    DeviceId=bytearray(1),
    # Unit length: 16 bytes
    Oob=bytearray(16),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPProvideOobRequest(DeviceId, Oob)
    return GAPProvideOobOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPRejectPasskey(
    device,
    DeviceId=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPRejectPasskeyRequestRequest(DeviceId)
    return GAPRejectPasskeyRequestOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPSendSmpKeys(
    device,
    DeviceId=bytearray(1),
    Keys_LtkIncluded=bytearray(1),
    # Array length depends on LtkIncluded. Mask: 0xFF. Shift: 0x00.
    Keys_LtkInfo=GAPSendSmpKeysRequest.Keys_LtkInfo(),
    Keys_IrkIncluded=bytearray(1),
    # Unit length: 16 bytes
        # Array length depends on IrkIncluded. Mask: 0xFF. Shift: 0x00.
    Keys_Irk=[],
    Keys_CsrkIncluded=bytearray(1),
    # Unit length: 16 bytes
        # Array length depends on CsrkIncluded. Mask: 0xFF. Shift: 0x00.
    Keys_Csrk=[],
    # Array length depends on LtkIncluded. Mask: 0xFF. Shift: 0x00.
    Keys_RandEdivInfo=GAPSendSmpKeysRequest.Keys_RandEdivInfo(),
    # Array length depends on IrkIncluded. Mask: 0xFF. Shift: 0x00.
    Keys_AddressIncluded=[],
    # Array length depends on AddressIncluded. Mask: 0xFF. Shift: 0x00.
    Keys_AddressInfo=GAPSendSmpKeysRequest.Keys_AddressInfo(),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPSendSmpKeysRequest(DeviceId, Keys_LtkIncluded, Keys_LtkInfo, Keys_IrkIncluded, Keys_Irk, Keys_CsrkIncluded, Keys_Csrk, Keys_RandEdivInfo, Keys_AddressIncluded, Keys_AddressInfo)
    return GAPSendSmpKeysOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPRejectKeyExchange(
    device,
    DeviceId=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPRejectKeyExchangeRequestRequest(DeviceId)
    return GAPRejectKeyExchangeRequestOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPProvideLongTermKey(
    device,
    DeviceId=bytearray(1),
    LtkSize=bytearray(1),
    # Array length depends on LtkSize. Mask: 0xFF. Shift: 0x00.
    Ltk=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPProvideLongTermKeyRequest(DeviceId, LtkSize, Ltk)
    return GAPProvideLongTermKeyOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPDenyLongTermKey(
    device,
    DeviceId=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPDenyLongTermKeyRequest(DeviceId)
    return GAPDenyLongTermKeyOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPLoadEncryptionInformation(
    device,
    DeviceId=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPLoadEncryptionInformationRequest(DeviceId)
    return GAPLoadEncryptionInformationOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPSetLocalPasskey(
    device,
    Passkey=bytearray(4),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPSetLocalPasskeyRequest(Passkey)
    return GAPSetLocalPasskeyOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPStartScanning(
    device,
    ScanningParametersIncluded=bytearray(1),
    # Array length depends on ScanningParametersIncluded. Mask: 0xFF. Shift: 0x00.
    ScanningParameters=GAPStartScanningRequest.ScanningParameters(),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPStartScanningRequest(ScanningParametersIncluded, ScanningParameters)
    return GAPStartScanningOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPStopScanning(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPStopScanningRequest()
    return GAPStopScanningOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPConnect(
    device,
    CreateConnectionParameters_ScanInterval=bytearray(2),
    CreateConnectionParameters_ScanWindow=bytearray(2),
    CreateConnectionParameters_FilterPolicy=GAPConnectRequestCreateConnectionParameters_FilterPolicy.gUseDeviceAddress_c,
    CreateConnectionParameters_OwnAddressType=GAPConnectRequestCreateConnectionParameters_OwnAddressType.gPublic_c,
    CreateConnectionParameters_PeerAddressType=GAPConnectRequestCreateConnectionParameters_PeerAddressType.gPublic_c,
    # Unit length: 6 bytes
    CreateConnectionParameters_PeerAddress=bytearray(6),
    CreateConnectionParameters_ConnIntervalMin=bytearray(2),
    CreateConnectionParameters_ConnIntervalMax=bytearray(2),
    CreateConnectionParameters_ConnLatency=bytearray(2),
    CreateConnectionParameters_SupervisionTimeout=bytearray(2),
    CreateConnectionParameters_ConnEventLengthMin=bytearray(2),
    CreateConnectionParameters_ConnEventLengthMax=bytearray(2),
    CreateConnectionParameters_usePeerIdentityAddress=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPConnectRequest(CreateConnectionParameters_ScanInterval, CreateConnectionParameters_ScanWindow, CreateConnectionParameters_FilterPolicy, CreateConnectionParameters_OwnAddressType, CreateConnectionParameters_PeerAddressType, CreateConnectionParameters_PeerAddress, CreateConnectionParameters_ConnIntervalMin, CreateConnectionParameters_ConnIntervalMax, CreateConnectionParameters_ConnLatency, CreateConnectionParameters_SupervisionTimeout, CreateConnectionParameters_ConnEventLengthMin, CreateConnectionParameters_ConnEventLengthMax, CreateConnectionParameters_usePeerIdentityAddress)
    return GAPConnectOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPDisconnect(
    device,
    DeviceId=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPDisconnectRequest(DeviceId)
    return GAPDisconnectOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPSaveCustomPeerInformation(
    device,
    DeviceId=bytearray(1),
    Offset=bytearray(2),
    InfoSize=bytearray(2),
    # Array length depends on InfoSize. Mask: 0xFFFF. Shift: 0x0000.
    Info=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPSaveCustomPeerInformationRequest(DeviceId, Offset, InfoSize, Info)
    return GAPSaveCustomPeerInformationOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPLoadCustomPeerInformation(
    device,
    DeviceId=bytearray(1),
    Offset=bytearray(2),
    InfoSize=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPLoadCustomPeerInformationRequest(DeviceId, Offset, InfoSize)
    return GAPLoadCustomPeerInformationOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPCheckIfBonded(
    device,
    DeviceId=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPCheckIfBondedRequest(DeviceId)
    return GAPCheckIfBondedOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPReadWhiteListSize(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPReadWhiteListSizeRequest()
    return GAPReadWhiteListSizeOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPClearWhiteList(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPClearWhiteListRequest()
    return GAPClearWhiteListOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPAddDeviceToWhiteList(
    device,
    AddressType=GAPAddDeviceToWhiteListRequestAddressType.gPublic_c,
    # Unit length: 6 bytes
    Address=bytearray(6),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPAddDeviceToWhiteListRequest(AddressType, Address)
    return GAPAddDeviceToWhiteListOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPRemoveDeviceFromWhiteList(
    device,
    AddressType=GAPRemoveDeviceFromWhiteListRequestAddressType.gPublic_c,
    # Unit length: 6 bytes
    Address=bytearray(6),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPRemoveDeviceFromWhiteListRequest(AddressType, Address)
    return GAPRemoveDeviceFromWhiteListOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPReadPublicDeviceAddress(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPReadPublicDeviceAddressRequest()
    return GAPReadPublicDeviceAddressOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPCreateRandomDeviceAddress(
    device,
    IrkIncluded=bytearray(1),
    # Unit length: 16 bytes
        # Array length depends on IrkIncluded. Mask: 0xFF. Shift: 0x00.
    Irk=[],
    # Array length depends on IrkIncluded. Mask: 0xFF. Shift: 0x00.
    RandomPartIncluded=[],
    # Unit length: 3 bytes
        # Array length depends on RandomPartIncluded. Mask: 0xFF. Shift: 0x00.
    RandomPart=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPCreateRandomDeviceAddressRequest(IrkIncluded, Irk, RandomPartIncluded, RandomPart)
    return GAPCreateRandomDeviceAddressOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPSaveDeviceName(
    device,
    DeviceId=bytearray(1),
    NameSize=bytearray(1),
    # Array length depends on NameSize. Mask: 0xFF. Shift: 0x00.
    Name=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPSaveDeviceNameRequest(DeviceId, NameSize, Name)
    return GAPSaveDeviceNameOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPGetBondedDevicesCount(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPGetBondedDevicesCountRequest()
    return GAPGetBondedDevicesCountOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPGetBondedDeviceName(
    device,
    NvmIndex=bytearray(1),
    MaxNameSize=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPGetBondedDeviceNameRequest(NvmIndex, MaxNameSize)
    return GAPGetBondedDeviceNameOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPRemoveBond(
    device,
    NvmIndex=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPRemoveBondRequest(NvmIndex)
    return GAPRemoveBondOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPRemoveAllBonds(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPRemoveAllBondsRequest()
    return GAPRemoveAllBondsOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPReadRadioPowerLevel(
    device,
    TxReadType=GAPReadRadioPowerLevelRequestTxReadType.gTxPowerCurrentLevelInConnection_c,
    DeviceId=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPReadRadioPowerLevelRequest(TxReadType, DeviceId)
    return GAPReadRadioPowerLevelOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPVerifyPrivateResolvableAddress(
    device,
    NvmIndex=bytearray(1),
    # Unit length: 6 bytes
    Address=bytearray(6),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPVerifyPrivateResolvableAddressRequest(NvmIndex, Address)
    return GAPVerifyPrivateResolvableAddressOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPSetRandomAddress(
    device,
    # Unit length: 6 bytes
    Address=bytearray(6),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPSetRandomAddressRequest(Address)
    return GAPSetRandomAddressOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPSetScanMode(
    device,
    ScanMode=GAPSetScanModeRequestScanMode.gDefaultScan_c,
    AutoConnectParams_NbOfAddresses=bytearray(1),
    AutoConnectParams_WriteInWhiteList=bytearray(1),
    # Array length depends on NbOfAddresses. Mask: 0xFF. Shift: 0x00.
    AutoConnectParams_AutoConnectData=GAPSetScanModeRequest.AutoConnectParams_AutoConnectData(),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPSetScanModeRequest(ScanMode, AutoConnectParams_NbOfAddresses, AutoConnectParams_WriteInWhiteList, AutoConnectParams_AutoConnectData)
    return GAPSetScanModeOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPSetDefaultPairingParameters(
    device,
    PairingParametersIncluded=bytearray(1),
    # Array length depends on PairingParametersIncluded. Mask: 0xFF. Shift: 0x00.
    PairingParameters=GAPSetDefaultPairingParametersRequest.PairingParameters(),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPSetDefaultPairingParametersRequest(PairingParametersIncluded, PairingParameters)
    return GAPSetDefaultPairingParametersOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPUpdateConnectionParameters(
    device,
    DeviceId=bytearray(1),
    IntervalMin=bytearray(2),
    IntervalMax=bytearray(2),
    SlaveLatency=bytearray(2),
    TimeoutMultiplier=bytearray(2),
    MinCeLength=bytearray(2),
    MaxCeLength=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPUpdateConnectionParametersRequest(DeviceId, IntervalMin, IntervalMax, SlaveLatency, TimeoutMultiplier, MinCeLength, MaxCeLength)
    return GAPUpdateConnectionParametersOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPEnableUpdateConnectionParameters(
    device,
    DeviceId=bytearray(1),
    Enable=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPEnableUpdateConnectionParametersRequest(DeviceId, Enable)
    return GAPEnableUpdateConnectionParametersOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GapUpdateLeDataLength(
    device,
    DeviceId=bytearray(1),
    TxOctets=bytearray(2),
    TxTime=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GapUpdateLeDataLengthRequest(DeviceId, TxOctets, TxTime)
    return GapUpdateLeDataLengthOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPControllerReset(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPControllerResetRequest()
    return GAPControllerResetOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPEnableHostPrivacy(
    device,
    Enable=bytearray(1),
    # Unit length: 16 bytes
        # Array length depends on Enable. Mask: 0xFF. Shift: 0x00.
    Irk=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPEnableHostPrivacyRequest(Enable, Irk)
    return GAPEnableHostPrivacyOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPEnableControllerPrivacy(
    device,
    Enable=bytearray(1),
    # Unit length: 16 bytes
        # Array length depends on Enable. Mask: 0xFF. Shift: 0x00.
    OwnIrk=[],
    # Array length depends on Enable. Mask: 0xFF. Shift: 0x00.
    PeerIdCount=[],
    # Array length depends on PeerIdCount. Mask: 0xFF. Shift: 0x00.
    PeerIdentities=GAPEnableControllerPrivacyRequest.PeerIdentities(),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPEnableControllerPrivacyRequest(Enable, OwnIrk, PeerIdCount, PeerIdentities)
    return GAPEnableControllerPrivacyOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPLeScRegeneratePublicKey(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPLeScRegeneratePublicKeyRequest()
    return GAPLeScRegeneratePublicKeyOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPLeScValidateNumericValue(
    device,
    DeviceId=bytearray(1),
    Valid=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPLeScValidateNumericValueRequest(DeviceId, Valid)
    return GAPLeScValidateNumericValueOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPLeScGetLocalOobData(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPLeScGetLocalOobDataRequest()
    return GAPLeScGetLocalOobDataOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPLeScSetPeerOobData(
    device,
    DeviceId=bytearray(1),
    # Unit length: 16 bytes
    RandomValue=bytearray(16),
    # Unit length: 16 bytes
    ConfirmValue=bytearray(16),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPLeScSetPeerOobDataRequest(DeviceId, RandomValue, ConfirmValue)
    return GAPLeScSetPeerOobDataOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPLeScSendKeypressNotificationPrivacy(
    device,
    DeviceId=bytearray(1),
    GapLeScKeypressNotificationParams_keypressNotifType=GAPLeScSendKeypressNotificationPrivacyRequestGapLeScKeypressNotificationParams_keypressNotifType.gKnPasskeyEntryStarted_c,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPLeScSendKeypressNotificationPrivacyRequest(DeviceId, GapLeScKeypressNotificationParams_keypressNotifType)
    return GAPLeScSendKeypressNotificationPrivacyOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def GAPGetBondedDevicesIdentityInformation(
    device,
    maxDevices=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.GAPGetBondedDevicesIdentityInformationRequest(maxDevices)
    return GAPGetBondedDevicesIdentityInformationOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def FSCIAllowDeviceToSleep(
    device,
    SignalHostWhenWakeUp=FSCIAllowDeviceToSleepRequestSignalHostWhenWakeUp.FALSE,
    DeepSleepDuration=bytearray(4),
    DeepSleepMode=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.FSCIAllowDeviceToSleepRequest(SignalHostWhenWakeUp, DeepSleepDuration, DeepSleepMode)
    return FSCIAllowDeviceToSleepOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def FSCIGetWakeupReason(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.BLE,
    timeout=1
):
    request = Frames.FSCIGetWakeupReasonRequest()
    return FSCIGetWakeupReasonOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)
