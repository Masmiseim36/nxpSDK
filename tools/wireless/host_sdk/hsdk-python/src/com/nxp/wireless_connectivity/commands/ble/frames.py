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
from com.nxp.wireless_connectivity.hsdk.utils import list_to_int, to_bytes


from com.nxp.wireless_connectivity.commands.ble.enums import ValueUuidType as UuidType


class FSCICPUResetRequest(object):

    pass


class HCIModeSelectRequest(object):

    def __init__(self, Enable=bytearray(1)):
        '''
        @param Enable: Enable/disable HCI
        '''
        self.Enable = Enable


class HCICommandRequest(object):

    def __init__(self, CommandLength=bytearray(2), Command=[]):
        '''
        @param CommandLength: The length of the HCI command
        @param Command: The HCI command
        '''
        self.CommandLength = CommandLength
        self.Command = Command


class HCIDataRequest(object):

    def __init__(self, DataLength=bytearray(2), Data=[]):
        '''
        @param DataLength: The length of the HCI data
        @param Data: The HCI data
        '''
        self.DataLength = DataLength
        self.Data = Data


class HCISynchronousDataRequest(object):

    def __init__(self, SynchronousDataLength=bytearray(2), SynchronousData=[]):
        '''
        @param SynchronousDataLength: The length of the HCI synchronous data
        @param SynchronousData: The HCI synchronous data
        '''
        self.SynchronousDataLength = SynchronousDataLength
        self.SynchronousData = SynchronousData


class L2CAPModeSelectRequest(object):

    def __init__(self, Enable=bytearray(1)):
        '''
        @param Enable: Enable/disable L2CAP
        '''
        self.Enable = Enable


class L2CAPInitRequest(object):

    pass


class L2CAPConfigRequest(object):

    def __init__(self, ConfigStruct_LeFeatures=bytearray(4), ConfigStruct_HCILeBufferSize=bytearray(4), ConfigStruct_MaxPduPayloadSize=bytearray(2)):
        '''
        @param ConfigStruct_LeFeatures: The list of the supported LE features
        @param ConfigStruct_HCILeBufferSize: The size of the HCILe buffer
        @param ConfigStruct_MaxPduPayloadSize: The maximum size of payload data
        '''
        self.ConfigStruct_LeFeatures = ConfigStruct_LeFeatures
        self.ConfigStruct_HCILeBufferSize = ConfigStruct_HCILeBufferSize
        self.ConfigStruct_MaxPduPayloadSize = ConfigStruct_MaxPduPayloadSize


class L2CAPSendAttDataRequest(object):

    def __init__(self, DeviceId=bytearray(1), PacketLength=bytearray(2), Packet=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param PacketLength: Length of the ATT data packet to be sent
        @param Packet: The ATT data packet to be transmitted
        '''
        self.DeviceId = DeviceId
        self.PacketLength = PacketLength
        self.Packet = Packet


class L2CAPSendSmpDataRequest(object):

    def __init__(self, DeviceId=bytearray(1), PacketLength=bytearray(2), Packet=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param PacketLength: Length of the SM data packet to be sent
        @param Packet: The SM data packet to be transmitted
        '''
        self.DeviceId = DeviceId
        self.PacketLength = PacketLength
        self.Packet = Packet


class L2CAPRegisterAttCallbackRequest(object):

    pass


class L2CAPRegisterSmpCallbackRequest(object):

    pass


class L2CAPRegisterLePsmRequest(object):

    def __init__(self, LePsm=bytearray(2), LePsmMtu=bytearray(2)):
        '''
        @param LePsm: Bluetooth SIG or Vendor LE_PSM
        @param LePsmMtu: MTU value
        '''
        self.LePsm = LePsm
        self.LePsmMtu = LePsmMtu


class L2CAPDeregisterLePsmRequest(object):

    def __init__(self, LePsm=bytearray(2)):
        '''
        @param LePsm: Bluetooth SIG or Vendor LE_PSM
        '''
        self.LePsm = LePsm


class L2CAPConnectLePsmRequest(object):

    def __init__(self, LePsm=bytearray(2), DeviceId=bytearray(1), InitialCredits=bytearray(2)):
        '''
        @param LePsm: Bluetooth SIG or Vendor LE_PSM
        @param DeviceId: The DeviceId for which the command is intended
        @param InitialCredits: Initial credits
        '''
        self.LePsm = LePsm
        self.DeviceId = DeviceId
        self.InitialCredits = InitialCredits


class L2CAPDisconnectLeCbChannelRequest(object):

    def __init__(self, DeviceId=bytearray(1), ChannelId=bytearray(2)):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param ChannelId: The L2CAP Channel Id assigned on the initiator
        '''
        self.DeviceId = DeviceId
        self.ChannelId = ChannelId


class L2CAPCancelConnectionRequest(object):

    def __init__(self, LePsm=bytearray(2), DeviceId=bytearray(1), RefuseReason=L2CAPCancelConnectionRequestRefuseReason.gSuccessful_c):
        '''
        @param LePsm: Bluetooth SIG or Vendor LE_PSM
        @param DeviceId: The DeviceId for which the command is intended
        @param RefuseReason: Refuse reason
        '''
        self.LePsm = LePsm
        self.DeviceId = DeviceId
        self.RefuseReason = RefuseReason


class L2CAPSendLeCbDataRequest(object):

    def __init__(self, DeviceId=bytearray(1), ChannelId=bytearray(2), PacketLength=bytearray(2), Packet=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param ChannelId: The L2CAP Channel Id assigned on the initiator
        @param PacketLength: Length of the data packet to be sent
        @param Packet: The data packet to be transmitted
        '''
        self.DeviceId = DeviceId
        self.ChannelId = ChannelId
        self.PacketLength = PacketLength
        self.Packet = Packet


class L2CAPSendLeCreditRequest(object):

    def __init__(self, DeviceId=bytearray(1), ChannelId=bytearray(2), Credits=bytearray(2)):
        '''
        @param DeviceId: The DeviceID to which credits are given
        @param ChannelId: The L2CAP Channel Id assigned on the initiator
        @param Credits: Number of credits to be given
        '''
        self.DeviceId = DeviceId
        self.ChannelId = ChannelId
        self.Credits = Credits


class SMModeSelectRequest(object):

    def __init__(self, Enable=bytearray(1)):
        '''
        @param Enable: Enable/disable SM
        '''
        self.Enable = Enable


class SMInitRequest(object):

    pass


class SMRegisterUserCallbackRequest(object):

    pass


class SMInitiatorStartPairingRequest(object):

    class SmPairingParams_AuthRequest(object):

        def __init__(self, BondingFlags=SmPairingParams_AuthRequestBondingFlags.gNoBonding_c, Mitm=SmPairingParams_AuthRequestMitm.gReqOff_c, sc=SmPairingParams_AuthRequestsc.gReqOff_c, keypress=SmPairingParams_AuthRequestkeypress.gReqOff_c):
            self.BondingFlags = BondingFlags
            self.Mitm = Mitm
            self.sc = sc
            self.keypress = keypress

    class SmPairingParams_InitatorKeyDistribution(object):

        def __init__(self, EncKey=bytearray(1), IdKey=bytearray(1), Sign=bytearray(1), LinkKey=bytearray(1), Reserved=bytearray(1)):
            self.EncKey = EncKey
            self.IdKey = IdKey
            self.Sign = Sign
            self.LinkKey = LinkKey
            self.Reserved = Reserved

    class SmPairingParams_ResponderKeyDistribution(object):

        def __init__(self, EncKey=bytearray(1), IdKey=bytearray(1), Sign=bytearray(1), LinkKey=bytearray(1), Reserved=bytearray(1)):
            self.EncKey = EncKey
            self.IdKey = IdKey
            self.Sign = Sign
            self.LinkKey = LinkKey
            self.Reserved = Reserved

    def __init__(self, DestinationDeviceId=bytearray(1), SmPairingParams_IoCapabilities=SMInitiatorStartPairingRequestSmPairingParams_IoCapabilities.gDisplayOnly_c, SmPairingParams_OobDataFlag=SMInitiatorStartPairingRequestSmPairingParams_OobDataFlag.gAuthDataNotPresent_c, SmPairingParams_AuthRequest=SmPairingParams_AuthRequest(), SmPairingParams_MaxEncKeySize=bytearray(1), SmPairingParams_InitatorKeyDistribution=SmPairingParams_InitatorKeyDistribution(), SmPairingParams_ResponderKeyDistribution=SmPairingParams_ResponderKeyDistribution()):
        '''
        @param DestinationDeviceId: DeviceId of the destination device
        @param SmPairingParams_IoCapabilities: I/O capabilities
        @param SmPairingParams_OobDataFlag: OOB data flag
        @param SmPairingParams_AuthRequest: SM authentication request parameters
        @param SmPairingParams_MaxEncKeySize: Maximum encryption key size
        @param SmPairingParams_InitatorKeyDistribution: Initiator key distribution
        @param SmPairingParams_ResponderKeyDistribution: Responder key distribution
        '''
        self.DestinationDeviceId = DestinationDeviceId
        self.SmPairingParams_IoCapabilities = SmPairingParams_IoCapabilities
        self.SmPairingParams_OobDataFlag = SmPairingParams_OobDataFlag
        self.SmPairingParams_AuthRequest = SmPairingParams_AuthRequest
        self.SmPairingParams_MaxEncKeySize = SmPairingParams_MaxEncKeySize
        self.SmPairingParams_InitatorKeyDistribution = SmPairingParams_InitatorKeyDistribution
        self.SmPairingParams_ResponderKeyDistribution = SmPairingParams_ResponderKeyDistribution


class SMSendSecurityRequestRequest(object):

    class SmSecurityReqParams_AuthRequest(object):

        def __init__(self, BondingFlags=SmSecurityReqParams_AuthRequestBondingFlags.gNoBonding_c, Mitm=SmSecurityReqParams_AuthRequestMitm.gReqOff_c, sc=SmSecurityReqParams_AuthRequestsc.gReqOff_c, keypress=SmSecurityReqParams_AuthRequestkeypress.gReqOff_c):
            self.BondingFlags = BondingFlags
            self.Mitm = Mitm
            self.sc = sc
            self.keypress = keypress

    def __init__(self, DestinationDeviceId=bytearray(1), SmSecurityReqParams_AuthRequest=SmSecurityReqParams_AuthRequest()):
        '''
        @param DestinationDeviceId: DeviceId of the destination device
        @param SmSecurityReqParams_AuthRequest: SM authentication request parameters
        '''
        self.DestinationDeviceId = DestinationDeviceId
        self.SmSecurityReqParams_AuthRequest = SmSecurityReqParams_AuthRequest


class SMTerminatePairingRequest(object):

    def __init__(self, DestinationDeviceId=bytearray(1), SmTerminatePairingParams_Reason=SMTerminatePairingRequestSmTerminatePairingParams_Reason.gReserved_c):
        '''
        @param DestinationDeviceId: DeviceId of the destination device
        @param SmTerminatePairingParams_Reason: Reason
        '''
        self.DestinationDeviceId = DestinationDeviceId
        self.SmTerminatePairingParams_Reason = SmTerminatePairingParams_Reason


class SMPairingRequestReplyRequest(object):

    class SmPairingParams_AuthRequest(object):

        def __init__(self, BondingFlags=SmPairingParams_AuthRequestBondingFlags.gNoBonding_c, Mitm=SmPairingParams_AuthRequestMitm.gReqOff_c, sc=SmPairingParams_AuthRequestsc.gReqOff_c, keypress=SmPairingParams_AuthRequestkeypress.gReqOff_c):
            self.BondingFlags = BondingFlags
            self.Mitm = Mitm
            self.sc = sc
            self.keypress = keypress

    class SmPairingParams_InitatorKeyDistribution(object):

        def __init__(self, EncKey=bytearray(1), IdKey=bytearray(1), Sign=bytearray(1), LinkKey=bytearray(1), Reserved=bytearray(1)):
            self.EncKey = EncKey
            self.IdKey = IdKey
            self.Sign = Sign
            self.LinkKey = LinkKey
            self.Reserved = Reserved

    class SmPairingParams_ResponderKeyDistribution(object):

        def __init__(self, EncKey=bytearray(1), IdKey=bytearray(1), Sign=bytearray(1), LinkKey=bytearray(1), Reserved=bytearray(1)):
            self.EncKey = EncKey
            self.IdKey = IdKey
            self.Sign = Sign
            self.LinkKey = LinkKey
            self.Reserved = Reserved

    def __init__(self, DestinationDeviceId=bytearray(1), SmPairingParams_IoCapabilities=SMPairingRequestReplyRequestSmPairingParams_IoCapabilities.gDisplayOnly_c, SmPairingParams_OobDataFlag=SMPairingRequestReplyRequestSmPairingParams_OobDataFlag.gAuthDataNotPresent_c, SmPairingParams_AuthRequest=SmPairingParams_AuthRequest(), SmPairingParams_MaxEncKeySize=bytearray(1), SmPairingParams_InitatorKeyDistribution=SmPairingParams_InitatorKeyDistribution(), SmPairingParams_ResponderKeyDistribution=SmPairingParams_ResponderKeyDistribution()):
        '''
        @param DestinationDeviceId: DeviceId of the destination device
        @param SmPairingParams_IoCapabilities: I/O capabilities
        @param SmPairingParams_OobDataFlag: OOB data flag
        @param SmPairingParams_AuthRequest: SM authentication request parameters
        @param SmPairingParams_MaxEncKeySize: Maximum encryption key size
        @param SmPairingParams_InitatorKeyDistribution: Initiator key distribution
        @param SmPairingParams_ResponderKeyDistribution: Responder key distribution
        '''
        self.DestinationDeviceId = DestinationDeviceId
        self.SmPairingParams_IoCapabilities = SmPairingParams_IoCapabilities
        self.SmPairingParams_OobDataFlag = SmPairingParams_OobDataFlag
        self.SmPairingParams_AuthRequest = SmPairingParams_AuthRequest
        self.SmPairingParams_MaxEncKeySize = SmPairingParams_MaxEncKeySize
        self.SmPairingParams_InitatorKeyDistribution = SmPairingParams_InitatorKeyDistribution
        self.SmPairingParams_ResponderKeyDistribution = SmPairingParams_ResponderKeyDistribution


class SMPasskeyRequestReplyRequest(object):

    def __init__(self, DestinationDeviceId=bytearray(1), SmPasskeyReqReplyParams_KeyType=SMPasskeyRequestReplyRequestSmPasskeyReqReplyParams_KeyType.Passkey, SmPasskeyReqReplyParams_Key=[]):
        '''
        @param DestinationDeviceId: DeviceId of the destination device
        @param SmPasskeyReqReplyParams_KeyType: Decimal passkey from 0 to 999.999 or 16 byte OOB
        @param SmPasskeyReqReplyParams_Key: The key as specifed by the key type
        '''
        self.DestinationDeviceId = DestinationDeviceId
        self.SmPasskeyReqReplyParams_KeyType = SmPasskeyReqReplyParams_KeyType
        self.SmPasskeyReqReplyParams_Key = SmPasskeyReqReplyParams_Key


class SMPairingKeysetRequestReplyRequest(object):

    class PairingKeysetRequestReplyParams_KeyDistPayload(object):

        def __init__(self, Ltk=bytearray(16), Irk=bytearray(16), Csrk=bytearray(16), Ediv=bytearray(2), Rand=bytearray(8), BDAddr=bytearray(6), BDAddrType=PairingKeysetRequestReplyParams_KeyDistPayloadBDAddrType.gPublic_c):
            # Unit length: 16 bytes
            self.Ltk = Ltk
            # Unit length: 16 bytes
            self.Irk = Irk
            # Unit length: 16 bytes
            self.Csrk = Csrk
            self.Ediv = Ediv
            self.Rand = Rand
            # Unit length: 6 bytes
            self.BDAddr = BDAddr
            self.BDAddrType = BDAddrType

    class PairingKeysetRequestReplyParams_SentKeys(object):

        def __init__(self, EncKey=bytearray(1), IdKey=bytearray(1), Sign=bytearray(1), LinkKey=bytearray(1), Reserved=bytearray(1)):
            self.EncKey = EncKey
            self.IdKey = IdKey
            self.Sign = Sign
            self.LinkKey = LinkKey
            self.Reserved = Reserved

    def __init__(self, DestinationDeviceId=bytearray(1), PairingKeysetRequestReplyParams_KeyDistPayload=PairingKeysetRequestReplyParams_KeyDistPayload(), PairingKeysetRequestReplyParams_SentKeys=PairingKeysetRequestReplyParams_SentKeys()):
        '''
        @param DestinationDeviceId: DeviceId of the destination device
        @param PairingKeysetRequestReplyParams_KeyDistPayload: Keys and additional data to be exchanged during the SMP Pairing key distribution phase
        @param PairingKeysetRequestReplyParams_SentKeys: Keys to be sent to the peer device during the SMP Pairing key distribution phase
        '''
        self.DestinationDeviceId = DestinationDeviceId
        self.PairingKeysetRequestReplyParams_KeyDistPayload = PairingKeysetRequestReplyParams_KeyDistPayload
        self.PairingKeysetRequestReplyParams_SentKeys = PairingKeysetRequestReplyParams_SentKeys


class SMLlStartEncryptionRequest(object):

    def __init__(self, DestinationDeviceId=bytearray(1), LlStartEncryptionParams_RandomNumber=bytearray(8), LlStartEncryptionParams_EncryptedDiversifier=bytearray(2), LlStartEncryptionParams_LongTermKey=bytearray(16)):
        '''
        @param DestinationDeviceId: DeviceId of the destination device
        @param LlStartEncryptionParams_RandomNumber: Random number
        @param LlStartEncryptionParams_EncryptedDiversifier: Encrypted Diversifier
        @param LlStartEncryptionParams_LongTermKey: Long Term Key
        '''
        self.DestinationDeviceId = DestinationDeviceId
        self.LlStartEncryptionParams_RandomNumber = LlStartEncryptionParams_RandomNumber
        self.LlStartEncryptionParams_EncryptedDiversifier = LlStartEncryptionParams_EncryptedDiversifier
        self.LlStartEncryptionParams_LongTermKey = LlStartEncryptionParams_LongTermKey


class SMLlLtkRequestReplyRequest(object):

    def __init__(self, DestinationDeviceId=bytearray(1), LlLtkRequestReplyParams_LongTermKey=bytearray(16)):
        '''
        @param DestinationDeviceId: DeviceId of the destination device
        @param LlLtkRequestReplyParams_LongTermKey: Long Term Key
        '''
        self.DestinationDeviceId = DestinationDeviceId
        self.LlLtkRequestReplyParams_LongTermKey = LlLtkRequestReplyParams_LongTermKey


class SMLlLtkRequestNegativeReplyRequest(object):

    def __init__(self, DestinationDeviceId=bytearray(1)):
        '''
        @param DestinationDeviceId: DeviceId of the destination device
        '''
        self.DestinationDeviceId = DestinationDeviceId


class SMLlEncryptRequestRequest(object):

    def __init__(self, LlEncryptReqParams_Key=bytearray(16), LlEncryptReqParams_PlainTextData=bytearray(16)):
        '''
        @param LlEncryptReqParams_Key: Key
        @param LlEncryptReqParams_PlainTextData: Data
        '''
        self.LlEncryptReqParams_Key = LlEncryptReqParams_Key
        self.LlEncryptReqParams_PlainTextData = LlEncryptReqParams_PlainTextData


class SMLlRandRequestRequest(object):

    pass


class SMTbCreateRandomDeviceAddressRequestRequest(object):

    class ResolvablePrivateAddr(object):

        def __init__(self, IRK=bytearray(16), RandIncluded=bytearray(1), Rand=[]):
            # Unit length: 16 bytes
            self.IRK = IRK
            self.RandIncluded = RandIncluded
            # Unit length: 3 bytes
            # Array length depends on RandIncluded.
            self.Rand = Rand

    def __init__(self, TbCreateRandomDeviceAddrReqParams_AddrType=SMTbCreateRandomDeviceAddressRequestRequestTbCreateRandomDeviceAddrReqParams_AddrType.RandAddrTypeNone, TbCreateRandomDeviceAddrReqParams_Data=[]):
        '''
        @param TbCreateRandomDeviceAddrReqParams_AddrType: Type of BLE Random Address to be created
        @param TbCreateRandomDeviceAddrReqParams_Data: Data, depending on the random address type
        '''
        self.TbCreateRandomDeviceAddrReqParams_AddrType = TbCreateRandomDeviceAddrReqParams_AddrType
        self.TbCreateRandomDeviceAddrReqParams_Data = TbCreateRandomDeviceAddrReqParams_Data


class SMTbCheckResolvablePrivateAddressRequestRequest(object):

    def __init__(self, TbCheckResolvablePrivateAddrReqParams_IRK=bytearray(16), TbCheckResolvablePrivateAddrReqParams_Address=bytearray(6)):
        '''
        @param TbCheckResolvablePrivateAddrReqParams_IRK: IRK used to check the Resolvable Private Address
        @param TbCheckResolvablePrivateAddrReqParams_Address: Address to be checked
        '''
        self.TbCheckResolvablePrivateAddrReqParams_IRK = TbCheckResolvablePrivateAddrReqParams_IRK
        self.TbCheckResolvablePrivateAddrReqParams_Address = TbCheckResolvablePrivateAddrReqParams_Address


class SMTbSignDataRequestRequest(object):

    def __init__(self, TbSignDataReqParams_CSRK=bytearray(16), TbSignDataReqParams_DataLength=bytearray(2), TbSignDataReqParams_Data=[]):
        '''
        @param TbSignDataReqParams_CSRK: CSRK used to sign the data block
        @param TbSignDataReqParams_DataLength: Length of the data to be signed. Actual data + 4 byte sign counter
        @param TbSignDataReqParams_Data: Data to be checked
        '''
        self.TbSignDataReqParams_CSRK = TbSignDataReqParams_CSRK
        self.TbSignDataReqParams_DataLength = TbSignDataReqParams_DataLength
        self.TbSignDataReqParams_Data = TbSignDataReqParams_Data


class SMTbVerifyDataSignatureRequestRequest(object):

    def __init__(self, TbVerifyDataSignatureReqParams_CSRK=bytearray(16), TbVerifyDataSignatureReqParams_DataLength=bytearray(2), TbVerifyDataSignatureReqParams_Data=[], TbVerifyDataSignatureReqParams_Signature=bytearray(8)):
        '''
        @param TbVerifyDataSignatureReqParams_CSRK: CSRK used to verify the signature of the data block
        @param TbVerifyDataSignatureReqParams_DataLength: Length of the data to be verified. Actual data + 4 byte sign counter
        @param TbVerifyDataSignatureReqParams_Data: Data to be verified
        @param TbVerifyDataSignatureReqParams_Signature: Signature to be verified
        '''
        self.TbVerifyDataSignatureReqParams_CSRK = TbVerifyDataSignatureReqParams_CSRK
        self.TbVerifyDataSignatureReqParams_DataLength = TbVerifyDataSignatureReqParams_DataLength
        self.TbVerifyDataSignatureReqParams_Data = TbVerifyDataSignatureReqParams_Data
        self.TbVerifyDataSignatureReqParams_Signature = TbVerifyDataSignatureReqParams_Signature


class SMDeviceConnectNotificationRequest(object):

    def __init__(self, DestinationDeviceId=bytearray(1)):
        '''
        @param DestinationDeviceId: DeviceId of the destination device
        '''
        self.DestinationDeviceId = DestinationDeviceId


class SMDeviceDisconnectNotificationRequest(object):

    def __init__(self, DestinationDeviceId=bytearray(1)):
        '''
        @param DestinationDeviceId: DeviceId of the destination device
        '''
        self.DestinationDeviceId = DestinationDeviceId


class SMSetDefaultPasskeyRequest(object):

    def __init__(self, SetDefaultPasskeyParams_DefaultPasskey=bytearray(4)):
        '''
        @param SetDefaultPasskeyParams_DefaultPasskey: The SM default passkey (0 to 999.999 decimal)
        '''
        self.SetDefaultPasskeyParams_DefaultPasskey = SetDefaultPasskeyParams_DefaultPasskey


class SMSetOobMitmProtectionRequest(object):

    def __init__(self, oobMitmProt=SMSetOobMitmProtectionRequestoobMitmProt.smOobMitmProtectionOff_c):
        '''
        @param oobMitmProt: Sets the OOB pairing method MITM protection to ON or OFF
        '''
        self.oobMitmProt = oobMitmProt


class SMSendKeypressNotificationRequest(object):

    def __init__(self, DestinationDeviceId=bytearray(1), SmKeypressNotificationParams_keypressNotifType=SMSendKeypressNotificationRequestSmKeypressNotificationParams_keypressNotifType.gSmpKeypressNotificationPasskeyEntryStarted_c):
        '''
        @param DestinationDeviceId: DeviceId of the destination device
        @param SmKeypressNotificationParams_keypressNotifType: keypressNotifType
        '''
        self.DestinationDeviceId = DestinationDeviceId
        self.SmKeypressNotificationParams_keypressNotifType = SmKeypressNotificationParams_keypressNotifType


class SMNcDisplayConfirmRequest(object):

    def __init__(self, DestinationDeviceId=bytearray(1), ncValueConfirm=bytearray(1)):
        '''
        @param DestinationDeviceId: DeviceId of the destination device
        @param ncValueConfirm: True if the value is confirmed by the user (YES) and false otherwise (NO)
        '''
        self.DestinationDeviceId = DestinationDeviceId
        self.ncValueConfirm = ncValueConfirm


class SMLeScOobDataRequestReplyRequest(object):

    def __init__(self, DestinationDeviceId=bytearray(1), LeScOobDataParams_=bytearray()):
        '''
        @param DestinationDeviceId: DeviceId of the destination device
        @param LeScOobDataParams_: LeScOobDataParams_
        '''
        self.DestinationDeviceId = DestinationDeviceId
        self.LeScOobDataParams_ = LeScOobDataParams_


class SMLocalLeScOobDataRequestReqRequest(object):

    pass


class SMGenerateNewEcdhPkSkPairRequest(object):

    pass


class ATTModeSelectRequest(object):

    def __init__(self, Enable=bytearray(1)):
        '''
        @param Enable: Enable/disable ATT
        '''
        self.Enable = Enable


class ATTInitRequest(object):

    pass


class ATTNotifyConnectionRequest(object):

    def __init__(self, DeviceId=bytearray(1)):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        '''
        self.DeviceId = DeviceId


class ATTNotifyDisconnectionRequest(object):

    def __init__(self, DeviceId=bytearray(1)):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        '''
        self.DeviceId = DeviceId


class ATTSetMtuRequest(object):

    def __init__(self, DeviceId=bytearray(1), Mtu=bytearray(2)):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Mtu: The MTU to be set
        '''
        self.DeviceId = DeviceId
        self.Mtu = Mtu


class ATTGetMtuRequest(object):

    def __init__(self, DeviceId=bytearray(1)):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        '''
        self.DeviceId = DeviceId


class ATTRegisterOpcodeCallbackRequest(object):

    def __init__(self, Opcode=ATTRegisterOpcodeCallbackRequestOpcode.gErrorResponse_c):
        '''
        @param Opcode: The ATT opcode
        '''
        self.Opcode = Opcode


class ATTRegisterUnsupportedOpcodeCallbackRequest(object):

    pass


class ATTRegisterTimeoutCallbackRequest(object):

    pass


class ATTServerSendErrorResponseRequest(object):

    def __init__(self, DeviceId=bytearray(1), Params_RequestOpcodeInError=ATTServerSendErrorResponseRequestParams_RequestOpcodeInError.gErrorResponse_c, Params_AttributeHandleInError=bytearray(2), Params_ErrorCode=ATTServerSendErrorResponseRequestParams_ErrorCode.gInvalidHandle_c):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_RequestOpcodeInError: Request opcode in error
        @param Params_AttributeHandleInError: Attribute handle in error
        @param Params_ErrorCode: ATT error code
        '''
        self.DeviceId = DeviceId
        self.Params_RequestOpcodeInError = Params_RequestOpcodeInError
        self.Params_AttributeHandleInError = Params_AttributeHandleInError
        self.Params_ErrorCode = Params_ErrorCode


class ATTClientSendExchangeMtuRequestRequest(object):

    def __init__(self, DeviceId=bytearray(1), Params_ClientRxMtu=bytearray(2)):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_ClientRxMtu: Client received MTU
        '''
        self.DeviceId = DeviceId
        self.Params_ClientRxMtu = Params_ClientRxMtu


class ATTServerSendExchangeMtuResponseRequest(object):

    def __init__(self, DeviceId=bytearray(1), Params_ServerRxMtu=bytearray(2)):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_ServerRxMtu: Server received MTU
        '''
        self.DeviceId = DeviceId
        self.Params_ServerRxMtu = Params_ServerRxMtu


class ATTClientSendFindInformationRequestRequest(object):

    def __init__(self, DeviceId=bytearray(1), Params_StartingHandle=bytearray(2), Params_EndingHandle=bytearray(2)):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_StartingHandle: Starting handle
        @param Params_EndingHandle: Ending handle
        '''
        self.DeviceId = DeviceId
        self.Params_StartingHandle = Params_StartingHandle
        self.Params_EndingHandle = Params_EndingHandle


class ATTServerSendFindInformationResponseRequest(object):

    class Uuid16BitFormat(object):

        class HandleUuid16(object):

            def __init__(self, Handle=bytearray(2), Uuid16=bytearray(2)):
                self.Handle = Handle
                self.Uuid16 = Uuid16

        def __init__(self, HandleUuid16PairCount=bytearray(1), HandleUuid16=[]):
            self.HandleUuid16PairCount = HandleUuid16PairCount
            # Array length depends on HandleUuid16PairCount.
            self.HandleUuid16 = HandleUuid16

    class Uuid128BitFormat(object):

        class HandleUuid128(object):

            def __init__(self, Handle=bytearray(2), Uuid128=bytearray(16)):
                self.Handle = Handle
                # Unit length: 16 bytes
                self.Uuid128 = Uuid128

        def __init__(self, HandleUuid128PairCount=bytearray(1), HandleUuid128=[]):
            self.HandleUuid128PairCount = HandleUuid128PairCount
            # Array length depends on HandleUuid128PairCount.
            self.HandleUuid128 = HandleUuid128

    def __init__(self, DeviceId=bytearray(1), Params_Format=ATTServerSendFindInformationResponseRequestParams_Format(), Params_InformationData=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_Format: ATT Uuid format
        @param Params_InformationData: Information data
        '''
        self.DeviceId = DeviceId
        self.Params_Format = Params_Format
        self.Params_InformationData = Params_InformationData


class ATTClientSendFindByTypeValueRequestRequest(object):

    def __init__(self, DeviceId=bytearray(1), Params_StartingHandle=bytearray(2), Params_EndingHandle=bytearray(2), Params_AttributeType=bytearray(2), Params_AttributeLength=bytearray(2), Params_AttributeValue=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_StartingHandle: Starting handle
        @param Params_EndingHandle: Ending handle
        @param Params_AttributeType: Attribute type
        @param Params_AttributeLength: Attribute length
        @param Params_AttributeValue: Attribute value
        '''
        self.DeviceId = DeviceId
        self.Params_StartingHandle = Params_StartingHandle
        self.Params_EndingHandle = Params_EndingHandle
        self.Params_AttributeType = Params_AttributeType
        self.Params_AttributeLength = Params_AttributeLength
        self.Params_AttributeValue = Params_AttributeValue


class ATTServerSendFindByTypeValueResponseRequest(object):

    class Params_HandleGroup(object):

        def __init__(self, StartingHandle=bytearray(2), EndingHandle=bytearray(2)):
            self.StartingHandle = StartingHandle
            self.EndingHandle = EndingHandle

        def pickle(self):
            result = bytearray()
            result += to_bytes(self.StartingHandle, 2)
            result += to_bytes(self.EndingHandle, 2)
            return result

    def __init__(self, DeviceId=bytearray(1), Params_GroupCount=bytearray(2), Params_HandleGroup=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_GroupCount: Number of groups
        @param Params_HandleGroup: ATT find information request parameters
        '''
        self.DeviceId = DeviceId
        self.Params_GroupCount = Params_GroupCount
        self.Params_HandleGroup = Params_HandleGroup

    def pickle(self):
        result = bytearray()
        result += to_bytes(self.DeviceId, 1)
        result += to_bytes(self.Params_GroupCount, 2)
        for i in range(list_to_int(self.Params_GroupCount, False)):
            result += self.Params_HandleGroup[i].pickle()
        return result


class ATTClientSendReadByTypeRequestRequest(object):

    def __init__(self, DeviceId=bytearray(1), Params_StartingHandle=bytearray(2), Params_EndingHandle=bytearray(2), Params_Format=ATTClientSendReadByTypeRequestRequestParams_Format(), Params_AttributeType=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_StartingHandle: Starting handle
        @param Params_EndingHandle: Ending handle
        @param Params_Format: ATT Uuid format
        @param Params_AttributeType: Attribute type
        '''
        self.DeviceId = DeviceId
        self.Params_StartingHandle = Params_StartingHandle
        self.Params_EndingHandle = Params_EndingHandle
        self.Params_Format = Params_Format
        self.Params_AttributeType = Params_AttributeType


class ATTServerSendReadByTypeResponseRequest(object):

    def __init__(self, DeviceId=bytearray(1), Params_Length=bytearray(1), Params_AttributeDataListLength=bytearray(2), Params_AttributeDataList=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_Length: Fixed length of each handle+value pair in the AttributeDataList
        @param Params_AttributeDataListLength: Attribute data list length
        @param Params_AttributeDataList: Attribute data list
        '''
        self.DeviceId = DeviceId
        self.Params_Length = Params_Length
        self.Params_AttributeDataListLength = Params_AttributeDataListLength
        self.Params_AttributeDataList = Params_AttributeDataList


class ATTClientSendReadRequestRequest(object):

    def __init__(self, DeviceId=bytearray(1), Params_AttributeHandle=bytearray(2)):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_AttributeHandle: Attribute handle
        '''
        self.DeviceId = DeviceId
        self.Params_AttributeHandle = Params_AttributeHandle


class ATTServerSendReadResponseRequest(object):

    def __init__(self, DeviceId=bytearray(1), Params_AttributeLength=bytearray(2), Params_AttributeValue=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_AttributeLength: Attribute length
        @param Params_AttributeValue: Attribute value
        '''
        self.DeviceId = DeviceId
        self.Params_AttributeLength = Params_AttributeLength
        self.Params_AttributeValue = Params_AttributeValue


class ATTClientSendReadBlobRequestRequest(object):

    def __init__(self, DeviceId=bytearray(1), Params_AttributeHandle=bytearray(2), Params_ValueOffset=bytearray(2)):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_AttributeHandle: Attribute handle
        @param Params_ValueOffset: Value offset
        '''
        self.DeviceId = DeviceId
        self.Params_AttributeHandle = Params_AttributeHandle
        self.Params_ValueOffset = Params_ValueOffset


class ATTServerSendReadBlobResponseRequest(object):

    def __init__(self, DeviceId=bytearray(1), Params_AttributeLength=bytearray(2), Params_AttributeValue=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_AttributeLength: Attribute length
        @param Params_AttributeValue: Attribute value
        '''
        self.DeviceId = DeviceId
        self.Params_AttributeLength = Params_AttributeLength
        self.Params_AttributeValue = Params_AttributeValue


class ATTClientSendReadMultipleRequestRequest(object):

    class Params_ListOfHandles(object):

        def __init__(self, Handle=bytearray(2)):
            self.Handle = Handle

        def pickle(self):
            result = bytearray()
            result += to_bytes(self.Handle, 2)
            return result

    def __init__(self, DeviceId=bytearray(1), Params_HandleCount=bytearray(2), Params_ListOfHandles=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_HandleCount: Number of handles
        @param Params_ListOfHandles: List of handles
        '''
        self.DeviceId = DeviceId
        self.Params_HandleCount = Params_HandleCount
        self.Params_ListOfHandles = Params_ListOfHandles

    def pickle(self):
        result = bytearray()
        result += to_bytes(self.DeviceId, 1)
        result += to_bytes(self.Params_HandleCount, 2)
        for i in range(list_to_int(self.Params_HandleCount, False)):
            result += self.Params_ListOfHandles[i].pickle()
        return result


class ATTServerSendReadMultipleResponseRequest(object):

    def __init__(self, DeviceId=bytearray(1), Params_ListLength=bytearray(2), Params_ListOfValues=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_ListLength: List length
        @param Params_ListOfValues: List of values
        '''
        self.DeviceId = DeviceId
        self.Params_ListLength = Params_ListLength
        self.Params_ListOfValues = Params_ListOfValues


class ATTClientSendReadByGroupTypeRequestRequest(object):

    def __init__(self, DeviceId=bytearray(1), Params_StartingHandle=bytearray(2), Params_EndingHandle=bytearray(2), Params_Format=ATTClientSendReadByGroupTypeRequestRequestParams_Format(), Params_AttributeType=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_StartingHandle: Starting handle
        @param Params_EndingHandle: Ending handle
        @param Params_Format: ATT Uuid format
        @param Params_AttributeType: Attribute type
        '''
        self.DeviceId = DeviceId
        self.Params_StartingHandle = Params_StartingHandle
        self.Params_EndingHandle = Params_EndingHandle
        self.Params_Format = Params_Format
        self.Params_AttributeType = Params_AttributeType


class ATTServerSendReadByGroupTypeResponseRequest(object):

    def __init__(self, DeviceId=bytearray(1), Params_Length=bytearray(1), Params_AttributeDataListLength=bytearray(2), Params_AttributeDataList=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_Length: Fixed length of each handle+value pair in the AttributeDataList
        @param Params_AttributeDataListLength: Attribute data list length
        @param Params_AttributeDataList: Attribute data list
        '''
        self.DeviceId = DeviceId
        self.Params_Length = Params_Length
        self.Params_AttributeDataListLength = Params_AttributeDataListLength
        self.Params_AttributeDataList = Params_AttributeDataList


class ATTClientSendWriteRequestRequest(object):

    def __init__(self, DeviceId=bytearray(1), Params_AttributeHandle=bytearray(2), Params_AttributeLength=bytearray(2), Params_AttributeValue=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_AttributeHandle: Attribute handle
        @param Params_AttributeLength: Attribute length
        @param Params_AttributeValue: Attribute value
        '''
        self.DeviceId = DeviceId
        self.Params_AttributeHandle = Params_AttributeHandle
        self.Params_AttributeLength = Params_AttributeLength
        self.Params_AttributeValue = Params_AttributeValue


class ATTServerSendWriteResponseRequest(object):

    def __init__(self, DeviceId=bytearray(1)):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        '''
        self.DeviceId = DeviceId


class ATTClientSendWriteCommandRequest(object):

    def __init__(self, DeviceId=bytearray(1), Params_AttributeHandle=bytearray(2), Params_AttributeLength=bytearray(2), Params_AttributeValue=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_AttributeHandle: Attribute handle
        @param Params_AttributeLength: Attribute length
        @param Params_AttributeValue: Attribute value
        '''
        self.DeviceId = DeviceId
        self.Params_AttributeHandle = Params_AttributeHandle
        self.Params_AttributeLength = Params_AttributeLength
        self.Params_AttributeValue = Params_AttributeValue


class ATTClientSendSignedWriteCommandRequest(object):

    def __init__(self, DeviceId=bytearray(1), Params_AttributeHandle=bytearray(2), Params_AttributeLength=bytearray(2), Params_AttributeValue=[], Params_AuthenticationSignature=bytearray(12)):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_AttributeHandle: Attribute handle
        @param Params_AttributeLength: Attribute length
        @param Params_AttributeValue: Attribute value
        @param Params_AuthenticationSignature: Authentication signature
        '''
        self.DeviceId = DeviceId
        self.Params_AttributeHandle = Params_AttributeHandle
        self.Params_AttributeLength = Params_AttributeLength
        self.Params_AttributeValue = Params_AttributeValue
        self.Params_AuthenticationSignature = Params_AuthenticationSignature


class ATTClientSendPrepareWriteRequestRequest(object):

    def __init__(self, DeviceId=bytearray(1), Params_AttributeHandle=bytearray(2), Params_ValueOffset=bytearray(2), Params_AttributeLength=bytearray(2), Params_AttributeValue=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_AttributeHandle: Attribute handle
        @param Params_ValueOffset: Value offset
        @param Params_AttributeLength: Attribute length
        @param Params_AttributeValue: Attribute value
        '''
        self.DeviceId = DeviceId
        self.Params_AttributeHandle = Params_AttributeHandle
        self.Params_ValueOffset = Params_ValueOffset
        self.Params_AttributeLength = Params_AttributeLength
        self.Params_AttributeValue = Params_AttributeValue


class ATTServerSendPrepareWriteResponseRequest(object):

    def __init__(self, DeviceId=bytearray(1), Params_AttributeHandle=bytearray(2), Params_ValueOffset=bytearray(2), Params_AttributeLength=bytearray(2), Params_AttributeValue=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_AttributeHandle: Attribute handle
        @param Params_ValueOffset: Value offset
        @param Params_AttributeLength: Attribute length
        @param Params_AttributeValue: Attribute value
        '''
        self.DeviceId = DeviceId
        self.Params_AttributeHandle = Params_AttributeHandle
        self.Params_ValueOffset = Params_ValueOffset
        self.Params_AttributeLength = Params_AttributeLength
        self.Params_AttributeValue = Params_AttributeValue


class ATTClientSendExecuteWriteRequestRequest(object):

    def __init__(self, DeviceId=bytearray(1), Params_Flags=ATTClientSendExecuteWriteRequestRequestParams_Flags.gCancel_c):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_Flags: Flags
        '''
        self.DeviceId = DeviceId
        self.Params_Flags = Params_Flags


class ATTServerSendExecuteWriteResponseRequest(object):

    def __init__(self, DeviceId=bytearray(1)):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        '''
        self.DeviceId = DeviceId


class ATTServerSendHandleValueNotificationRequest(object):

    def __init__(self, DeviceId=bytearray(1), Params_AttributeHandle=bytearray(2), Params_AttributeLength=bytearray(2), Params_AttributeValue=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_AttributeHandle: Attribute handle
        @param Params_AttributeLength: Attribute length
        @param Params_AttributeValue: Attribute value
        '''
        self.DeviceId = DeviceId
        self.Params_AttributeHandle = Params_AttributeHandle
        self.Params_AttributeLength = Params_AttributeLength
        self.Params_AttributeValue = Params_AttributeValue


class ATTServerSendHandleValueIndicationRequest(object):

    def __init__(self, DeviceId=bytearray(1), Params_AttributeHandle=bytearray(2), Params_AttributeLength=bytearray(2), Params_AttributeValue=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_AttributeHandle: Attribute handle
        @param Params_AttributeLength: Attribute length
        @param Params_AttributeValue: Attribute value
        '''
        self.DeviceId = DeviceId
        self.Params_AttributeHandle = Params_AttributeHandle
        self.Params_AttributeLength = Params_AttributeLength
        self.Params_AttributeValue = Params_AttributeValue


class ATTClientSendHandleValueConfirmationRequest(object):

    def __init__(self, DeviceId=bytearray(1)):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        '''
        self.DeviceId = DeviceId


class GATTModeSelectRequest(object):

    def __init__(self, Enable=bytearray(1)):
        '''
        @param Enable: Enable/disable GATT
        '''
        self.Enable = Enable


class GATTInitRequest(object):

    pass


class GATTGetMtuRequest(object):

    def __init__(self, DeviceId=bytearray(1)):
        '''
        @param DeviceId: Device ID of the connected peer
        '''
        self.DeviceId = DeviceId


class GATTClientInitRequest(object):

    pass


class GATTClientResetProcedureRequest(object):

    pass


class GATTClientRegisterProcedureCallbackRequest(object):

    pass


class GATTClientRegisterNotificationCallbackRequest(object):

    pass


class GATTClientRegisterIndicationCallbackRequest(object):

    pass


class GATTClientExchangeMtuRequest(object):

    def __init__(self, DeviceId=bytearray(1)):
        '''
        @param DeviceId: Device ID of the connected peer
        '''
        self.DeviceId = DeviceId


class GATTClientDiscoverAllPrimaryServicesRequest(object):

    def __init__(self, DeviceId=bytearray(1), MaxNbOfServices=bytearray(1)):
        '''
        @param DeviceId: Device ID of the connected peer
        @param MaxNbOfServices: Maximum number of services to be filled
        '''
        self.DeviceId = DeviceId
        self.MaxNbOfServices = MaxNbOfServices


class GATTClientDiscoverPrimaryServicesByUuidRequest(object):

    def __init__(self, DeviceId=bytearray(1), UuidType=GATTClientDiscoverPrimaryServicesByUuidRequestUuidType.Uuid16Bits, Uuid=[], MaxNbOfServices=bytearray(1)):
        '''
        @param DeviceId: Device ID of the connected peer
        @param UuidType: UUID type
        @param Uuid: UUID value
        @param MaxNbOfServices: Maximum number of services to be filled
        '''
        self.DeviceId = DeviceId
        self.UuidType = UuidType
        self.Uuid = Uuid
        self.MaxNbOfServices = MaxNbOfServices


class GATTClientFindIncludedServicesRequest(object):

    class Service_Characteristics(object):

        class Value(object):

            def __init__(self, Handle=bytearray(2), UuidType=ValueUuidType.Uuid16Bits, Uuid=[], ValueLength=bytearray(2), MaxValueLength=bytearray(2), Value=[]):
                self.Handle = Handle
                self.UuidType = UuidType
                # Array length depends on UuidType.
                self.Uuid = Uuid
                self.ValueLength = ValueLength
                self.MaxValueLength = MaxValueLength
                # Array length depends on ValueLength.
                self.Value = Value

            def pickle(self):
                result = bytearray()
                result += to_bytes(self.Handle, 2)
                result += to_bytes(self.UuidType, 1)
                if self.UuidType == UuidType.Uuid16Bits:
                    selected_len = 2
                if self.UuidType == UuidType.Uuid128Bits:
                    selected_len = 16
                if self.UuidType == UuidType.Uuid32Bits:
                    selected_len = 4
                result += to_bytes(self.Uuid, selected_len)
                result += to_bytes(self.ValueLength, 2)
                result += to_bytes(self.MaxValueLength, 2)
                result += to_bytes(self.Value, self.ValueLength)
                return result

        class Descriptors(object):

            def __init__(self, Handle=bytearray(2), UuidType=DescriptorsUuidType.Uuid16Bits, Uuid=[], ValueLength=bytearray(2), MaxValueLength=bytearray(2), Value=[]):
                self.Handle = Handle
                self.UuidType = UuidType
                # Array length depends on UuidType.
                self.Uuid = Uuid
                self.ValueLength = ValueLength
                self.MaxValueLength = MaxValueLength
                # Array length depends on ValueLength.
                self.Value = Value

            def pickle(self):
                result = bytearray()
                result += to_bytes(self.Handle, 2)
                result += to_bytes(self.UuidType, 1)
                if self.UuidType == UuidType.Uuid16Bits:
                    selected_len = 2
                if self.UuidType == UuidType.Uuid128Bits:
                    selected_len = 16
                if self.UuidType == UuidType.Uuid32Bits:
                    selected_len = 4
                result += to_bytes(self.Uuid, selected_len)
                result += to_bytes(self.ValueLength, 2)
                result += to_bytes(self.MaxValueLength, 2)
                result += to_bytes(self.Value, self.ValueLength)
                return result

        def __init__(self, Properties=Service_CharacteristicsProperties.gNone_c, Value=Value(), NbOfDescriptors=bytearray(1), Descriptors=[]):
            self.Properties = Properties
            self.Value = Value
            self.NbOfDescriptors = NbOfDescriptors
            # Array length depends on NbOfDescriptors.
            self.Descriptors = Descriptors

        def pickle(self):
            result = bytearray()
            result += to_bytes(self.Properties, 1)
            result += self.Value.pickle()
            result += to_bytes(self.NbOfDescriptors, 1)
            for i in range(list_to_int(self.NbOfDescriptors, False)):
                result += self.Descriptors[i].pickle()
            return result

    class Service_IncludedServices(object):

        def __init__(self, StartHandle=bytearray(2), EndHandle=bytearray(2), UuidType=Service_IncludedServicesUuidType.Uuid16Bits, Uuid=[], NbOfCharacteristics=bytearray(1), NbOfIncludedServices=bytearray(1)):
            self.StartHandle = StartHandle
            self.EndHandle = EndHandle
            self.UuidType = UuidType
            # Array length depends on UuidType.
            self.Uuid = Uuid
            self.NbOfCharacteristics = NbOfCharacteristics
            self.NbOfIncludedServices = NbOfIncludedServices

        def pickle(self):
            result = bytearray()
            result += to_bytes(self.StartHandle, 2)
            result += to_bytes(self.EndHandle, 2)
            result += to_bytes(self.UuidType, 1)
            if self.UuidType == UuidType.Uuid16Bits:
                selected_len = 2
            if self.UuidType == UuidType.Uuid128Bits:
                selected_len = 16
            if self.UuidType == UuidType.Uuid32Bits:
                selected_len = 4
            result += to_bytes(self.Uuid, selected_len)
            result += to_bytes(self.NbOfCharacteristics, 1)
            result += to_bytes(self.NbOfIncludedServices, 1)
            return result

    def __init__(self, DeviceId=bytearray(1), Service_StartHandle=bytearray(2), Service_EndHandle=bytearray(2), Service_UuidType=GATTClientFindIncludedServicesRequestService_UuidType.Uuid16Bits, Service_Uuid=[], Service_NbOfCharacteristics=bytearray(1), Service_Characteristics=[], Service_NbOfIncludedServices=bytearray(1), Service_IncludedServices=[], MaxNbOfIncludedServices=bytearray(1)):
        '''
        @param DeviceId: Device ID of the connected peer
        @param Service_StartHandle: The handle of the Service Declaration attribute
        @param Service_EndHandle: The last handle belonging to this Service (followed by another Service declaration of the end of the database)
        @param Service_UuidType: UUID type
        @param Service_Uuid: UUID value
        @param Service_NbOfCharacteristics: Size of the Characteristic array
        @param Service_Characteristics: Characteristic array
        @param Service_NbOfIncludedServices: Size of the Included Services array
        @param Service_IncludedServices: Included Services array
        @param MaxNbOfIncludedServices: Maximum number of included services to be filled
        '''
        self.DeviceId = DeviceId
        self.Service_StartHandle = Service_StartHandle
        self.Service_EndHandle = Service_EndHandle
        self.Service_UuidType = Service_UuidType
        self.Service_Uuid = Service_Uuid
        self.Service_NbOfCharacteristics = Service_NbOfCharacteristics
        self.Service_Characteristics = Service_Characteristics
        self.Service_NbOfIncludedServices = Service_NbOfIncludedServices
        self.Service_IncludedServices = Service_IncludedServices
        self.MaxNbOfIncludedServices = MaxNbOfIncludedServices

    def pickle(self):
        result = bytearray()
        result += to_bytes(self.DeviceId, 1)
        result += to_bytes(self.Service_StartHandle, 2)
        result += to_bytes(self.Service_EndHandle, 2)
        result += to_bytes(self.Service_UuidType, 1)
        if self.Service_UuidType == UuidType.Uuid16Bits:
            selected_len = 2
        if self.Service_UuidType == UuidType.Uuid128Bits:
            selected_len = 16
        if self.Service_UuidType == UuidType.Uuid32Bits:
            selected_len = 4
        result += to_bytes(self.Service_Uuid, selected_len)
        result += to_bytes(self.Service_NbOfCharacteristics, 1)
        for i in range(list_to_int(self.Service_NbOfCharacteristics, False)):
            result += self.Service_Characteristics[i].pickle()
        result += to_bytes(self.Service_NbOfIncludedServices, 1)
        for i in range(list_to_int(self.Service_NbOfIncludedServices, False)):
            result += self.Service_IncludedServices[i].pickle()
        result += to_bytes(self.MaxNbOfIncludedServices, 1)
        return result


class GATTClientDiscoverAllCharacteristicsOfServiceRequest(object):

    class Service_Characteristics(object):

        class Value(object):

            def __init__(self, Handle=bytearray(2), UuidType=ValueUuidType.Uuid16Bits, Uuid=[], ValueLength=bytearray(2), MaxValueLength=bytearray(2), Value=[]):
                self.Handle = Handle
                self.UuidType = UuidType
                # Array length depends on UuidType.
                self.Uuid = Uuid
                self.ValueLength = ValueLength
                self.MaxValueLength = MaxValueLength
                # Array length depends on ValueLength.
                self.Value = Value

            def pickle(self):
                result = bytearray()
                result += to_bytes(self.Handle, 2)
                result += to_bytes(self.UuidType, 1)
                if self.UuidType == UuidType.Uuid16Bits:
                    selected_len = 2
                if self.UuidType == UuidType.Uuid128Bits:
                    selected_len = 16
                if self.UuidType == UuidType.Uuid32Bits:
                    selected_len = 4
                result += to_bytes(self.Uuid, selected_len)
                result += to_bytes(self.ValueLength, 2)
                result += to_bytes(self.MaxValueLength, 2)
                result += to_bytes(self.Value, self.ValueLength)
                return result

        class Descriptors(object):

            def __init__(self, Handle=bytearray(2), UuidType=DescriptorsUuidType.Uuid16Bits, Uuid=[], ValueLength=bytearray(2), MaxValueLength=bytearray(2), Value=[]):
                self.Handle = Handle
                self.UuidType = UuidType
                # Array length depends on UuidType.
                self.Uuid = Uuid
                self.ValueLength = ValueLength
                self.MaxValueLength = MaxValueLength
                # Array length depends on ValueLength.
                self.Value = Value

            def pickle(self):
                result = bytearray()
                result += to_bytes(self.Handle, 2)
                result += to_bytes(self.UuidType, 1)
                if self.UuidType == UuidType.Uuid16Bits:
                    selected_len = 2
                if self.UuidType == UuidType.Uuid128Bits:
                    selected_len = 16
                if self.UuidType == UuidType.Uuid32Bits:
                    selected_len = 4
                result += to_bytes(self.Uuid, selected_len)
                result += to_bytes(self.ValueLength, 2)
                result += to_bytes(self.MaxValueLength, 2)
                result += to_bytes(self.Value, self.ValueLength)
                return result

        def __init__(self, Properties=Service_CharacteristicsProperties.gNone_c, Value=Value(), NbOfDescriptors=bytearray(1), Descriptors=[]):
            self.Properties = Properties
            self.Value = Value
            self.NbOfDescriptors = NbOfDescriptors
            # Array length depends on NbOfDescriptors.
            self.Descriptors = Descriptors

        def pickle(self):
            result = bytearray()
            result += to_bytes(self.Properties, 1)
            result += self.Value.pickle()
            result += to_bytes(self.NbOfDescriptors, 1)
            for i in range(list_to_int(self.NbOfDescriptors, False)):
                result += self.Descriptors[i].pickle()
            return result

    class Service_IncludedServices(object):

        def __init__(self, StartHandle=bytearray(2), EndHandle=bytearray(2), UuidType=Service_IncludedServicesUuidType.Uuid16Bits, Uuid=[], NbOfCharacteristics=bytearray(1), NbOfIncludedServices=bytearray(1)):
            self.StartHandle = StartHandle
            self.EndHandle = EndHandle
            self.UuidType = UuidType
            # Array length depends on UuidType.
            self.Uuid = Uuid
            self.NbOfCharacteristics = NbOfCharacteristics
            self.NbOfIncludedServices = NbOfIncludedServices

        def pickle(self):
            result = bytearray()
            result += to_bytes(self.StartHandle, 2)
            result += to_bytes(self.EndHandle, 2)
            result += to_bytes(self.UuidType, 1)
            if self.UuidType == UuidType.Uuid16Bits:
                selected_len = 2
            if self.UuidType == UuidType.Uuid128Bits:
                selected_len = 16
            if self.UuidType == UuidType.Uuid32Bits:
                selected_len = 4
            result += to_bytes(self.Uuid, selected_len)
            result += to_bytes(self.NbOfCharacteristics, 1)
            result += to_bytes(self.NbOfIncludedServices, 1)
            return result

    def __init__(self, DeviceId=bytearray(1), Service_StartHandle=bytearray(2), Service_EndHandle=bytearray(2), Service_UuidType=GATTClientDiscoverAllCharacteristicsOfServiceRequestService_UuidType.Uuid16Bits, Service_Uuid=[], Service_NbOfCharacteristics=bytearray(1), Service_Characteristics=[], Service_NbOfIncludedServices=bytearray(1), Service_IncludedServices=[], MaxNbOfCharacteristics=bytearray(1)):
        '''
        @param DeviceId: Device ID of the connected peer
        @param Service_StartHandle: The handle of the Service Declaration attribute
        @param Service_EndHandle: The last handle belonging to this Service (followed by another Service declaration of the end of the database)
        @param Service_UuidType: UUID type
        @param Service_Uuid: UUID value
        @param Service_NbOfCharacteristics: Size of the Characteristic array
        @param Service_Characteristics: Characteristic array
        @param Service_NbOfIncludedServices: Size of the Included Services array
        @param Service_IncludedServices: Included Services array
        @param MaxNbOfCharacteristics: Maximum number of characteristics to be filled
        '''
        self.DeviceId = DeviceId
        self.Service_StartHandle = Service_StartHandle
        self.Service_EndHandle = Service_EndHandle
        self.Service_UuidType = Service_UuidType
        self.Service_Uuid = Service_Uuid
        self.Service_NbOfCharacteristics = Service_NbOfCharacteristics
        self.Service_Characteristics = Service_Characteristics
        self.Service_NbOfIncludedServices = Service_NbOfIncludedServices
        self.Service_IncludedServices = Service_IncludedServices
        self.MaxNbOfCharacteristics = MaxNbOfCharacteristics

    def pickle(self):
        result = bytearray()
        result += to_bytes(self.DeviceId, 1)
        result += to_bytes(self.Service_StartHandle, 2)
        result += to_bytes(self.Service_EndHandle, 2)
        result += to_bytes(self.Service_UuidType, 1)
        if self.Service_UuidType == UuidType.Uuid16Bits:
            selected_len = 2
        if self.Service_UuidType == UuidType.Uuid128Bits:
            selected_len = 16
        if self.Service_UuidType == UuidType.Uuid32Bits:
            selected_len = 4
        result += to_bytes(self.Service_Uuid, selected_len)
        result += to_bytes(self.Service_NbOfCharacteristics, 1)
        for i in range(list_to_int(self.Service_NbOfCharacteristics, False)):
            result += self.Service_Characteristics[i].pickle()
        result += to_bytes(self.Service_NbOfIncludedServices, 1)
        for i in range(list_to_int(self.Service_NbOfIncludedServices, False)):
            result += self.Service_IncludedServices[i].pickle()
        result += to_bytes(self.MaxNbOfCharacteristics, 1)
        return result


class GATTClientDiscoverCharacteristicOfServiceByUuidRequest(object):

    class Service_Characteristics(object):

        class Value(object):

            def __init__(self, Handle=bytearray(2), UuidType=ValueUuidType.Uuid16Bits, Uuid=[], ValueLength=bytearray(2), MaxValueLength=bytearray(2), Value=[]):
                self.Handle = Handle
                self.UuidType = UuidType
                # Array length depends on UuidType.
                self.Uuid = Uuid
                self.ValueLength = ValueLength
                self.MaxValueLength = MaxValueLength
                # Array length depends on ValueLength.
                self.Value = Value

            def pickle(self):
                result = bytearray()
                result += to_bytes(self.Handle, 2)
                result += to_bytes(self.UuidType, 1)
                if self.UuidType == UuidType.Uuid16Bits:
                    selected_len = 2
                if self.UuidType == UuidType.Uuid128Bits:
                    selected_len = 16
                if self.UuidType == UuidType.Uuid32Bits:
                    selected_len = 4
                result += to_bytes(self.Uuid, selected_len)
                result += to_bytes(self.ValueLength, 2)
                result += to_bytes(self.MaxValueLength, 2)
                result += to_bytes(self.Value, self.ValueLength)
                return result

        class Descriptors(object):

            def __init__(self, Handle=bytearray(2), UuidType=DescriptorsUuidType.Uuid16Bits, Uuid=[], ValueLength=bytearray(2), MaxValueLength=bytearray(2), Value=[]):
                self.Handle = Handle
                self.UuidType = UuidType
                # Array length depends on UuidType.
                self.Uuid = Uuid
                self.ValueLength = ValueLength
                self.MaxValueLength = MaxValueLength
                # Array length depends on ValueLength.
                self.Value = Value

            def pickle(self):
                result = bytearray()
                result += to_bytes(self.Handle, 2)
                result += to_bytes(self.UuidType, 1)
                if self.UuidType == UuidType.Uuid16Bits:
                    selected_len = 2
                if self.UuidType == UuidType.Uuid128Bits:
                    selected_len = 16
                if self.UuidType == UuidType.Uuid32Bits:
                    selected_len = 4
                result += to_bytes(self.Uuid, selected_len)
                result += to_bytes(self.ValueLength, 2)
                result += to_bytes(self.MaxValueLength, 2)
                result += to_bytes(self.Value, self.ValueLength)
                return result

        def __init__(self, Properties=Service_CharacteristicsProperties.gNone_c, Value=Value(), NbOfDescriptors=bytearray(1), Descriptors=[]):
            self.Properties = Properties
            self.Value = Value
            self.NbOfDescriptors = NbOfDescriptors
            # Array length depends on NbOfDescriptors.
            self.Descriptors = Descriptors

        def pickle(self):
            result = bytearray()
            result += to_bytes(self.Properties, 1)
            result += self.Value.pickle()
            result += to_bytes(self.NbOfDescriptors, 1)
            for i in range(list_to_int(self.NbOfDescriptors, False)):
                result += self.Descriptors[i].pickle()
            return result

    class Service_IncludedServices(object):

        def __init__(self, StartHandle=bytearray(2), EndHandle=bytearray(2), UuidType=Service_IncludedServicesUuidType.Uuid16Bits, Uuid=[], NbOfCharacteristics=bytearray(1), NbOfIncludedServices=bytearray(1)):
            self.StartHandle = StartHandle
            self.EndHandle = EndHandle
            self.UuidType = UuidType
            # Array length depends on UuidType.
            self.Uuid = Uuid
            self.NbOfCharacteristics = NbOfCharacteristics
            self.NbOfIncludedServices = NbOfIncludedServices

        def pickle(self):
            result = bytearray()
            result += to_bytes(self.StartHandle, 2)
            result += to_bytes(self.EndHandle, 2)
            result += to_bytes(self.UuidType, 1)
            if self.UuidType == UuidType.Uuid16Bits:
                selected_len = 2
            if self.UuidType == UuidType.Uuid128Bits:
                selected_len = 16
            if self.UuidType == UuidType.Uuid32Bits:
                selected_len = 4
            result += to_bytes(self.Uuid, selected_len)
            result += to_bytes(self.NbOfCharacteristics, 1)
            result += to_bytes(self.NbOfIncludedServices, 1)
            return result

    def __init__(self, DeviceId=bytearray(1), UuidType=GATTClientDiscoverCharacteristicOfServiceByUuidRequestUuidType.Uuid16Bits, Uuid=[], Service_StartHandle=bytearray(2), Service_EndHandle=bytearray(2), Service_UuidType=GATTClientDiscoverCharacteristicOfServiceByUuidRequestService_UuidType.Uuid16Bits, Service_Uuid=[], Service_NbOfCharacteristics=bytearray(1), Service_Characteristics=[], Service_NbOfIncludedServices=bytearray(1), Service_IncludedServices=[], MaxNbOfCharacteristics=bytearray(1)):
        '''
        @param DeviceId: Device ID of the connected peer
        @param UuidType: UUID type
        @param Uuid: UUID value
        @param Service_StartHandle: The handle of the Service Declaration attribute
        @param Service_EndHandle: The last handle belonging to this Service (followed by another Service declaration of the end of the database)
        @param Service_UuidType: UUID type
        @param Service_Uuid: UUID value
        @param Service_NbOfCharacteristics: Size of the Characteristic array
        @param Service_Characteristics: Characteristic array
        @param Service_NbOfIncludedServices: Size of the Included Services array
        @param Service_IncludedServices: Included Services array
        @param MaxNbOfCharacteristics: Maximum number of characteristics to be filled
        '''
        self.DeviceId = DeviceId
        self.UuidType = UuidType
        self.Uuid = Uuid
        self.Service_StartHandle = Service_StartHandle
        self.Service_EndHandle = Service_EndHandle
        self.Service_UuidType = Service_UuidType
        self.Service_Uuid = Service_Uuid
        self.Service_NbOfCharacteristics = Service_NbOfCharacteristics
        self.Service_Characteristics = Service_Characteristics
        self.Service_NbOfIncludedServices = Service_NbOfIncludedServices
        self.Service_IncludedServices = Service_IncludedServices
        self.MaxNbOfCharacteristics = MaxNbOfCharacteristics

    def pickle(self):
        result = bytearray()
        result += to_bytes(self.DeviceId, 1)
        result += to_bytes(self.UuidType, 1)
        if self.UuidType == UuidType.Uuid16Bits:
            selected_len = 2
        if self.UuidType == UuidType.Uuid128Bits:
            selected_len = 16
        if self.UuidType == UuidType.Uuid32Bits:
            selected_len = 4
        result += to_bytes(self.Uuid, selected_len)
        result += to_bytes(self.Service_StartHandle, 2)
        result += to_bytes(self.Service_EndHandle, 2)
        result += to_bytes(self.Service_UuidType, 1)
        if self.Service_UuidType == UuidType.Uuid16Bits:
            selected_len = 2
        if self.Service_UuidType == UuidType.Uuid128Bits:
            selected_len = 16
        if self.Service_UuidType == UuidType.Uuid32Bits:
            selected_len = 4
        result += to_bytes(self.Service_Uuid, selected_len)
        result += to_bytes(self.Service_NbOfCharacteristics, 1)
        for i in range(list_to_int(self.Service_NbOfCharacteristics, False)):
            result += self.Service_Characteristics[i].pickle()
        result += to_bytes(self.Service_NbOfIncludedServices, 1)
        for i in range(list_to_int(self.Service_NbOfIncludedServices, False)):
            result += self.Service_IncludedServices[i].pickle()
        result += to_bytes(self.MaxNbOfCharacteristics, 1)
        return result


class GATTClientDiscoverAllCharacteristicDescriptorsRequest(object):

    class Characteristic_Value(object):

        def __init__(self, Handle=bytearray(2), UuidType=Characteristic_ValueUuidType.Uuid16Bits, Uuid=[], ValueLength=bytearray(2), MaxValueLength=bytearray(2), Value=[]):
            self.Handle = Handle
            self.UuidType = UuidType
            # Array length depends on UuidType.
            self.Uuid = Uuid
            self.ValueLength = ValueLength
            self.MaxValueLength = MaxValueLength
            # Array length depends on ValueLength.
            self.Value = Value

        def pickle(self):
            result = bytearray()
            result += to_bytes(self.Handle, 2)
            result += to_bytes(self.UuidType, 1)
            if self.UuidType == UuidType.Uuid16Bits:
                selected_len = 2
            if self.UuidType == UuidType.Uuid128Bits:
                selected_len = 16
            if self.UuidType == UuidType.Uuid32Bits:
                selected_len = 4
            result += to_bytes(self.Uuid, selected_len)
            result += to_bytes(self.ValueLength, 2)
            result += to_bytes(self.MaxValueLength, 2)
            result += to_bytes(self.Value, self.ValueLength)
            return result

    class Characteristic_Descriptors(object):

        def __init__(self, Handle=bytearray(2), UuidType=Characteristic_DescriptorsUuidType.Uuid16Bits, Uuid=[], ValueLength=bytearray(2), MaxValueLength=bytearray(2), Value=[]):
            self.Handle = Handle
            self.UuidType = UuidType
            # Array length depends on UuidType.
            self.Uuid = Uuid
            self.ValueLength = ValueLength
            self.MaxValueLength = MaxValueLength
            # Array length depends on ValueLength.
            self.Value = Value

        def pickle(self):
            result = bytearray()
            result += to_bytes(self.Handle, 2)
            result += to_bytes(self.UuidType, 1)
            if self.UuidType == UuidType.Uuid16Bits:
                selected_len = 2
            if self.UuidType == UuidType.Uuid128Bits:
                selected_len = 16
            if self.UuidType == UuidType.Uuid32Bits:
                selected_len = 4
            result += to_bytes(self.Uuid, selected_len)
            result += to_bytes(self.ValueLength, 2)
            result += to_bytes(self.MaxValueLength, 2)
            result += to_bytes(self.Value, self.ValueLength)
            return result

    def __init__(self, DeviceId=bytearray(1), Characteristic_Properties=GATTClientDiscoverAllCharacteristicDescriptorsRequestCharacteristic_Properties.gNone_c, Characteristic_Value=Characteristic_Value(), Characteristic_NbOfDescriptors=bytearray(1), Characteristic_Descriptors=[], EndingHandle=bytearray(2), MaxNbOfDescriptors=bytearray(1)):
        '''
        @param DeviceId: Device ID of the connected peer
        @param Characteristic_Properties: Characteristic Properties as defined by GATT
        @param Characteristic_Value: Characteristic Value attribute
        @param Characteristic_NbOfDescriptors: Size of the Characteristic Descriptors array
        @param Characteristic_Descriptors: Characteristic Descriptors array
        @param EndingHandle: The last handle of the Characteristic
        @param MaxNbOfDescriptors: Maximum number of descriptors to be filled
        '''
        self.DeviceId = DeviceId
        self.Characteristic_Properties = Characteristic_Properties
        self.Characteristic_Value = Characteristic_Value
        self.Characteristic_NbOfDescriptors = Characteristic_NbOfDescriptors
        self.Characteristic_Descriptors = Characteristic_Descriptors
        self.EndingHandle = EndingHandle
        self.MaxNbOfDescriptors = MaxNbOfDescriptors

    def pickle(self):
        result = bytearray()
        result += to_bytes(self.DeviceId, 1)
        result += to_bytes(self.Characteristic_Properties, 1)
        result += self.Characteristic_Value.pickle()
        result += to_bytes(self.Characteristic_NbOfDescriptors, 1)
        for i in range(list_to_int(self.Characteristic_NbOfDescriptors, False)):
            result += self.Characteristic_Descriptors[i].pickle()
        result += to_bytes(self.EndingHandle, 2)
        result += to_bytes(self.MaxNbOfDescriptors, 1)
        return result


class GATTClientReadCharacteristicValueRequest(object):

    class Characteristic_Value(object):

        def __init__(self, Handle=bytearray(2), UuidType=Characteristic_ValueUuidType.Uuid16Bits, Uuid=[], ValueLength=bytearray(2), MaxValueLength=bytearray(2), Value=[]):
            self.Handle = Handle
            self.UuidType = UuidType
            # Array length depends on UuidType.
            self.Uuid = Uuid
            self.ValueLength = ValueLength
            self.MaxValueLength = MaxValueLength
            # Array length depends on ValueLength.
            self.Value = Value

        def pickle(self):
            result = bytearray()
            result += to_bytes(self.Handle, 2)
            result += to_bytes(self.UuidType, 1)
            if self.UuidType == UuidType.Uuid16Bits:
                selected_len = 2
            if self.UuidType == UuidType.Uuid128Bits:
                selected_len = 16
            if self.UuidType == UuidType.Uuid32Bits:
                selected_len = 4
            result += to_bytes(self.Uuid, selected_len)
            result += to_bytes(self.ValueLength, 2)
            result += to_bytes(self.MaxValueLength, 2)
            result += to_bytes(self.Value, self.ValueLength)
            return result

    class Characteristic_Descriptors(object):

        def __init__(self, Handle=bytearray(2), UuidType=Characteristic_DescriptorsUuidType.Uuid16Bits, Uuid=[], ValueLength=bytearray(2), MaxValueLength=bytearray(2), Value=[]):
            self.Handle = Handle
            self.UuidType = UuidType
            # Array length depends on UuidType.
            self.Uuid = Uuid
            self.ValueLength = ValueLength
            self.MaxValueLength = MaxValueLength
            # Array length depends on ValueLength.
            self.Value = Value

        def pickle(self):
            result = bytearray()
            result += to_bytes(self.Handle, 2)
            result += to_bytes(self.UuidType, 1)
            if self.UuidType == UuidType.Uuid16Bits:
                selected_len = 2
            if self.UuidType == UuidType.Uuid128Bits:
                selected_len = 16
            if self.UuidType == UuidType.Uuid32Bits:
                selected_len = 4
            result += to_bytes(self.Uuid, selected_len)
            result += to_bytes(self.ValueLength, 2)
            result += to_bytes(self.MaxValueLength, 2)
            result += to_bytes(self.Value, self.ValueLength)
            return result

    def __init__(self, DeviceId=bytearray(1), Characteristic_Properties=GATTClientReadCharacteristicValueRequestCharacteristic_Properties.gNone_c, Characteristic_Value=Characteristic_Value(), Characteristic_NbOfDescriptors=bytearray(1), Characteristic_Descriptors=[], MaxReadBytes=bytearray(2)):
        '''
        @param DeviceId: Device ID of the connected peer
        @param Characteristic_Properties: Characteristic Properties as defined by GATT
        @param Characteristic_Value: Characteristic Value attribute
        @param Characteristic_NbOfDescriptors: Size of the Characteristic Descriptors array
        @param Characteristic_Descriptors: Characteristic Descriptors array
        @param MaxReadBytes: Maximum number of bytes to be read
        '''
        self.DeviceId = DeviceId
        self.Characteristic_Properties = Characteristic_Properties
        self.Characteristic_Value = Characteristic_Value
        self.Characteristic_NbOfDescriptors = Characteristic_NbOfDescriptors
        self.Characteristic_Descriptors = Characteristic_Descriptors
        self.MaxReadBytes = MaxReadBytes

    def pickle(self):
        result = bytearray()
        result += to_bytes(self.DeviceId, 1)
        result += to_bytes(self.Characteristic_Properties, 1)
        result += self.Characteristic_Value.pickle()
        result += to_bytes(self.Characteristic_NbOfDescriptors, 1)
        for i in range(list_to_int(self.Characteristic_NbOfDescriptors, False)):
            result += self.Characteristic_Descriptors[i].pickle()
        result += to_bytes(self.MaxReadBytes, 2)
        return result


class GATTClientReadUsingCharacteristicUuidRequest(object):

    class HandleRange(object):

        def __init__(self, StartHandle=bytearray(2), EndHandle=bytearray(2)):
            self.StartHandle = StartHandle
            self.EndHandle = EndHandle

        def pickle(self):
            result = bytearray()
            result += to_bytes(self.StartHandle, 2)
            result += to_bytes(self.EndHandle, 2)
            return result

    def __init__(self, DeviceId=bytearray(1), UuidType=GATTClientReadUsingCharacteristicUuidRequestUuidType.Uuid16Bits, Uuid=[], HandleRangeIncluded=bytearray(1), HandleRange=HandleRange(), MaxReadBytes=bytearray(2)):
        '''
        @param DeviceId: Device ID of the connected peer
        @param UuidType: UUID type
        @param Uuid: UUID value
        @param HandleRangeIncluded: Boolean value which indicates if handle range is included or not
        @param HandleRange: Handle range for the search
        @param MaxReadBytes: Maximum number of bytes to be read
        '''
        self.DeviceId = DeviceId
        self.UuidType = UuidType
        self.Uuid = Uuid
        self.HandleRangeIncluded = HandleRangeIncluded
        self.HandleRange = HandleRange
        self.MaxReadBytes = MaxReadBytes

    def pickle(self):
        result = bytearray()
        result += to_bytes(self.DeviceId, 1)
        result += to_bytes(self.UuidType, 1)
        if self.UuidType == UuidType.Uuid16Bits:
            selected_len = 2
        if self.UuidType == UuidType.Uuid128Bits:
            selected_len = 16
        if self.UuidType == UuidType.Uuid32Bits:
            selected_len = 4
        result += to_bytes(self.Uuid, selected_len)
        result += to_bytes(self.HandleRangeIncluded, 1)
        if self.HandleRangeIncluded:
            result += self.HandleRange.pickle()
        result += to_bytes(self.MaxReadBytes, 2)
        return result


class GATTClientReadMultipleCharacteristicValuesRequest(object):

    class Characteristics(object):

        class Value(object):

            def __init__(self, Handle=bytearray(2), UuidType=ValueUuidType.Uuid16Bits, Uuid=[], ValueLength=bytearray(2), MaxValueLength=bytearray(2), Value=[]):
                self.Handle = Handle
                self.UuidType = UuidType
                # Array length depends on UuidType.
                self.Uuid = Uuid
                self.ValueLength = ValueLength
                self.MaxValueLength = MaxValueLength
                # Array length depends on ValueLength.
                self.Value = Value

            def pickle(self):
                result = bytearray()
                result += to_bytes(self.Handle, 2)
                result += to_bytes(self.UuidType, 1)
                if self.UuidType == UuidType.Uuid16Bits:
                    selected_len = 2
                if self.UuidType == UuidType.Uuid128Bits:
                    selected_len = 16
                if self.UuidType == UuidType.Uuid32Bits:
                    selected_len = 4
                result += to_bytes(self.Uuid, selected_len)
                result += to_bytes(self.ValueLength, 2)
                result += to_bytes(self.MaxValueLength, 2)
                result += to_bytes(self.Value, self.ValueLength)
                return result

        class Descriptors(object):

            def __init__(self, Handle=bytearray(2), UuidType=DescriptorsUuidType.Uuid16Bits, Uuid=[], ValueLength=bytearray(2), MaxValueLength=bytearray(2), Value=[]):
                self.Handle = Handle
                self.UuidType = UuidType
                # Array length depends on UuidType.
                self.Uuid = Uuid
                self.ValueLength = ValueLength
                self.MaxValueLength = MaxValueLength
                # Array length depends on ValueLength.
                self.Value = Value

            def pickle(self):
                result = bytearray()
                result += to_bytes(self.Handle, 2)
                result += to_bytes(self.UuidType, 1)
                if self.UuidType == UuidType.Uuid16Bits:
                    selected_len = 2
                if self.UuidType == UuidType.Uuid128Bits:
                    selected_len = 16
                if self.UuidType == UuidType.Uuid32Bits:
                    selected_len = 4
                result += to_bytes(self.Uuid, selected_len)
                result += to_bytes(self.ValueLength, 2)
                result += to_bytes(self.MaxValueLength, 2)
                result += to_bytes(self.Value, self.ValueLength)
                return result

        def __init__(self, Properties=CharacteristicsProperties.gNone_c, Value=Value(), NbOfDescriptors=bytearray(1), Descriptors=[]):
            self.Properties = Properties
            self.Value = Value
            self.NbOfDescriptors = NbOfDescriptors
            # Array length depends on NbOfDescriptors.
            self.Descriptors = Descriptors

        def pickle(self):
            result = bytearray()
            result += to_bytes(self.Properties, 1)
            result += self.Value.pickle()
            result += to_bytes(self.NbOfDescriptors, 1)
            for i in range(list_to_int(self.NbOfDescriptors, False)):
                result += self.Descriptors[i].pickle()
            return result

    def __init__(self, DeviceId=bytearray(1), NbOfCharacteristics=bytearray(1), Characteristics=[]):
        '''
        @param DeviceId: Device ID of the connected peer
        @param NbOfCharacteristics: Number of characteristics in the array
        @param Characteristics: Array of the characteristics whose values are to be read
        '''
        self.DeviceId = DeviceId
        self.NbOfCharacteristics = NbOfCharacteristics
        self.Characteristics = Characteristics

    def pickle(self):
        result = bytearray()
        result += to_bytes(self.DeviceId, 1)
        result += to_bytes(self.NbOfCharacteristics, 1)
        for i in range(list_to_int(self.NbOfCharacteristics, False)):
            result += self.Characteristics[i].pickle()
        return result


class GATTClientWriteCharacteristicValueRequest(object):

    class Characteristic_Value(object):

        def __init__(self, Handle=bytearray(2), UuidType=Characteristic_ValueUuidType.Uuid16Bits, Uuid=[], ValueLength=bytearray(2), MaxValueLength=bytearray(2), Value=[]):
            self.Handle = Handle
            self.UuidType = UuidType
            # Array length depends on UuidType.
            self.Uuid = Uuid
            self.ValueLength = ValueLength
            self.MaxValueLength = MaxValueLength
            # Array length depends on ValueLength.
            self.Value = Value

        def pickle(self):
            result = bytearray()
            result += to_bytes(self.Handle, 2)
            result += to_bytes(self.UuidType, 1)
            if self.UuidType == UuidType.Uuid16Bits:
                selected_len = 2
            if self.UuidType == UuidType.Uuid128Bits:
                selected_len = 16
            if self.UuidType == UuidType.Uuid32Bits:
                selected_len = 4
            result += to_bytes(self.Uuid, selected_len)
            result += to_bytes(self.ValueLength, 2)
            result += to_bytes(self.MaxValueLength, 2)
            result += to_bytes(self.Value, self.ValueLength)
            return result

    class Characteristic_Descriptors(object):

        def __init__(self, Handle=bytearray(2), UuidType=Characteristic_DescriptorsUuidType.Uuid16Bits, Uuid=[], ValueLength=bytearray(2), MaxValueLength=bytearray(2), Value=[]):
            self.Handle = Handle
            self.UuidType = UuidType
            # Array length depends on UuidType.
            self.Uuid = Uuid
            self.ValueLength = ValueLength
            self.MaxValueLength = MaxValueLength
            # Array length depends on ValueLength.
            self.Value = Value

        def pickle(self):
            result = bytearray()
            result += to_bytes(self.Handle, 2)
            result += to_bytes(self.UuidType, 1)
            if self.UuidType == UuidType.Uuid16Bits:
                selected_len = 2
            if self.UuidType == UuidType.Uuid128Bits:
                selected_len = 16
            if self.UuidType == UuidType.Uuid32Bits:
                selected_len = 4
            result += to_bytes(self.Uuid, selected_len)
            result += to_bytes(self.ValueLength, 2)
            result += to_bytes(self.MaxValueLength, 2)
            result += to_bytes(self.Value, self.ValueLength)
            return result

    def __init__(self, DeviceId=bytearray(1), Characteristic_Properties=GATTClientWriteCharacteristicValueRequestCharacteristic_Properties.gNone_c, Characteristic_Value=Characteristic_Value(), Characteristic_NbOfDescriptors=bytearray(1), Characteristic_Descriptors=[], ValueLength=bytearray(2), Value=[], WithoutResponse=bytearray(1), SignedWrite=bytearray(1), ReliableLongCharWrites=bytearray(1), Csrk=bytearray(16)):
        '''
        @param DeviceId: Device ID of the connected peer
        @param Characteristic_Properties: Characteristic Properties as defined by GATT
        @param Characteristic_Value: Characteristic Value attribute
        @param Characteristic_NbOfDescriptors: Size of the Characteristic Descriptors array
        @param Characteristic_Descriptors: Characteristic Descriptors array
        @param ValueLength: Number of bytes to be written
        @param Value: Array of bytes to be written
        @param WithoutResponse: Indicates if a Write Command will be used
        @param SignedWrite: Indicates if a Signed Write will be performed
        @param ReliableLongCharWrites: Indicates Reliable Long Writes
        @param Csrk: The CSRK if signedWrite is TRUE, ignored otherwise
        '''
        self.DeviceId = DeviceId
        self.Characteristic_Properties = Characteristic_Properties
        self.Characteristic_Value = Characteristic_Value
        self.Characteristic_NbOfDescriptors = Characteristic_NbOfDescriptors
        self.Characteristic_Descriptors = Characteristic_Descriptors
        self.ValueLength = ValueLength
        self.Value = Value
        self.WithoutResponse = WithoutResponse
        self.SignedWrite = SignedWrite
        self.ReliableLongCharWrites = ReliableLongCharWrites
        self.Csrk = Csrk

    def pickle(self):
        result = bytearray()
        result += to_bytes(self.DeviceId, 1)
        result += to_bytes(self.Characteristic_Properties, 1)
        result += self.Characteristic_Value.pickle()
        result += to_bytes(self.Characteristic_NbOfDescriptors, 1)
        for i in range(list_to_int(self.Characteristic_NbOfDescriptors, False)):
            result += self.Characteristic_Descriptors[i].pickle()
        result += to_bytes(self.ValueLength, 2)
        for i in range(list_to_int(self.ValueLength, False)):
            result += to_bytes(self.Value[i], 1)
        result += to_bytes(self.WithoutResponse, 1)
        result += to_bytes(self.SignedWrite, 1)
        result += to_bytes(self.ReliableLongCharWrites, 1)
        result += to_bytes(self.Csrk, 16)
        return result


class GATTClientReadCharacteristicDescriptorRequest(object):

    def __init__(self, DeviceId=bytearray(1), Descriptor_Handle=bytearray(2), Descriptor_UuidType=GATTClientReadCharacteristicDescriptorRequestDescriptor_UuidType.Uuid16Bits, Descriptor_Uuid=[], Descriptor_ValueLength=bytearray(2), Descriptor_MaxValueLength=bytearray(2), Descriptor_Value=[], MaxReadBytes=bytearray(2)):
        '''
        @param DeviceId: Device ID of the connected peer
        @param Descriptor_Handle: Attribute handle
        @param Descriptor_UuidType: UUID type
        @param Descriptor_Uuid: UUID value
        @param Descriptor_ValueLength: Length of the attribute value array
        @param Descriptor_MaxValueLength: Maximum length of the attribute value array; if this is set to 0, then the attribute's length is fixed and cannot be changed
        @param Descriptor_Value: Attribute value array
        @param MaxReadBytes: Maximum number of bytes to be read
        '''
        self.DeviceId = DeviceId
        self.Descriptor_Handle = Descriptor_Handle
        self.Descriptor_UuidType = Descriptor_UuidType
        self.Descriptor_Uuid = Descriptor_Uuid
        self.Descriptor_ValueLength = Descriptor_ValueLength
        self.Descriptor_MaxValueLength = Descriptor_MaxValueLength
        self.Descriptor_Value = Descriptor_Value
        self.MaxReadBytes = MaxReadBytes


class GATTClientWriteCharacteristicDescriptorRequest(object):

    def __init__(self, DeviceId=bytearray(1), Descriptor_Handle=bytearray(2), Descriptor_UuidType=GATTClientWriteCharacteristicDescriptorRequestDescriptor_UuidType.Uuid16Bits, Descriptor_Uuid=[], Descriptor_ValueLength=bytearray(2), Descriptor_MaxValueLength=bytearray(2), Descriptor_Value=[], ValueLength=bytearray(2), Value=[]):
        '''
        @param DeviceId: Device ID of the connected peer
        @param Descriptor_Handle: Attribute handle
        @param Descriptor_UuidType: UUID type
        @param Descriptor_Uuid: UUID value
        @param Descriptor_ValueLength: Length of the attribute value array
        @param Descriptor_MaxValueLength: Maximum length of the attribute value array; if this is set to 0, then the attribute's length is fixed and cannot be changed
        @param Descriptor_Value: Attribute value array
        @param ValueLength: Number of bytes to be written
        @param Value: Array of bytes to be written
        '''
        self.DeviceId = DeviceId
        self.Descriptor_Handle = Descriptor_Handle
        self.Descriptor_UuidType = Descriptor_UuidType
        self.Descriptor_Uuid = Descriptor_Uuid
        self.Descriptor_ValueLength = Descriptor_ValueLength
        self.Descriptor_MaxValueLength = Descriptor_MaxValueLength
        self.Descriptor_Value = Descriptor_Value
        self.ValueLength = ValueLength
        self.Value = Value


class GATTServerInitRequest(object):

    pass


class GATTServerRegisterCallbackRequest(object):

    pass


class GATTServerRegisterHandlesForWriteNotificationsRequest(object):

    def __init__(self, HandleCount=bytearray(1), AttributeHandles=[]):
        '''
        @param HandleCount: Number of handles in array
        @param AttributeHandles: Statically allocated array of handles
        '''
        self.HandleCount = HandleCount
        self.AttributeHandles = AttributeHandles


class GATTServerSendAttributeWrittenStatusRequest(object):

    def __init__(self, DeviceId=bytearray(1), AttributeHandle=bytearray(2), Status=bytearray(1)):
        '''
        @param DeviceId: The device ID of the connected peer
        @param AttributeHandle: The attribute handle that was written
        @param Status: The status of the write operation
        '''
        self.DeviceId = DeviceId
        self.AttributeHandle = AttributeHandle
        self.Status = Status


class GATTServerSendNotificationRequest(object):

    def __init__(self, DeviceId=bytearray(1), Handle=bytearray(2)):
        '''
        @param DeviceId: The device ID of the connected peer
        @param Handle: Handle of the Value of the Characteristic to be notified
        '''
        self.DeviceId = DeviceId
        self.Handle = Handle


class GATTServerSendIndicationRequest(object):

    def __init__(self, DeviceId=bytearray(1), Handle=bytearray(2)):
        '''
        @param DeviceId: The device ID of the connected peer
        @param Handle: Handle of the Value of the Characteristic to be indicated
        '''
        self.DeviceId = DeviceId
        self.Handle = Handle


class GATTServerSendInstantValueNotificationRequest(object):

    def __init__(self, DeviceId=bytearray(1), Handle=bytearray(2), ValueLength=bytearray(2), Value=[]):
        '''
        @param DeviceId: The device ID of the connected peer
        @param Handle: Handle of the Value of the Characteristic to be notified
        @param ValueLength: Length of data to be notified
        @param Value: Data to be notified
        '''
        self.DeviceId = DeviceId
        self.Handle = Handle
        self.ValueLength = ValueLength
        self.Value = Value


class GATTServerSendInstantValueIndicationRequest(object):

    def __init__(self, DeviceId=bytearray(1), Handle=bytearray(2), ValueLength=bytearray(2), Value=[]):
        '''
        @param DeviceId: The device ID of the connected peer
        @param Handle: Handle of the Value of the Characteristic to be indicated
        @param ValueLength: Length of data to be indicated
        @param Value: Data to be indicated
        '''
        self.DeviceId = DeviceId
        self.Handle = Handle
        self.ValueLength = ValueLength
        self.Value = Value


class GATTServerRegisterHandlesForReadNotificationsRequest(object):

    class AttributeHandles(object):

        def __init__(self, Handle=bytearray(2)):
            self.Handle = Handle

        def pickle(self):
            result = bytearray()
            result += to_bytes(self.Handle, 2)
            return result

    def __init__(self, HandleCount=bytearray(1), AttributeHandles=[]):
        '''
        @param HandleCount: Number of handles in array
        @param AttributeHandles: Statically allocated array of handles
        '''
        self.HandleCount = HandleCount
        self.AttributeHandles = AttributeHandles

    def pickle(self):
        result = bytearray()
        result += to_bytes(self.HandleCount, 1)
        for i in range(list_to_int(self.HandleCount, False)):
            result += self.AttributeHandles[i].pickle()
        return result


class GATTServerSendAttributeReadStatusRequest(object):

    def __init__(self, DeviceId=bytearray(1), AttributeHandle=bytearray(2), Status=bytearray(1)):
        '''
        @param DeviceId: The device ID of the connected peer
        @param AttributeHandle: The attribute handle that was read
        @param Status: The status of the read operation
        '''
        self.DeviceId = DeviceId
        self.AttributeHandle = AttributeHandle
        self.Status = Status


class GATTDBModeSelectRequest(object):

    def __init__(self, Enable=bytearray(1)):
        '''
        @param Enable: Enable/disable GATT Database (application)
        '''
        self.Enable = Enable


class GATTDBInitRequest(object):

    pass


class GATTDBWriteAttributeRequest(object):

    def __init__(self, Handle=bytearray(2), ValueLength=bytearray(2), Value=[]):
        '''
        @param Handle: The handle of the attribute to be written
        @param ValueLength: The number of bytes to be written
        @param Value: The value to be written
        '''
        self.Handle = Handle
        self.ValueLength = ValueLength
        self.Value = Value


class GATTDBReadAttributeRequest(object):

    def __init__(self, Handle=bytearray(2), MaxBytes=bytearray(2)):
        '''
        @param Handle: The handle of the attribute to be read
        @param MaxBytes: The maximum number of bytes to be read
        '''
        self.Handle = Handle
        self.MaxBytes = MaxBytes


class GATTDBFindServiceHandleRequest(object):

    def __init__(self, StartHandle=bytearray(2), UuidType=GATTDBFindServiceHandleRequestUuidType.Uuid16Bits, Uuid=[]):
        '''
        @param StartHandle: The handle to start the search. Should be 0x0001 on the first call
        @param UuidType: UUID type
        @param Uuid: UUID value
        '''
        self.StartHandle = StartHandle
        self.UuidType = UuidType
        self.Uuid = Uuid


class GATTDBFindCharValueHandleInServiceRequest(object):

    def __init__(self, ServiceHandle=bytearray(2), UuidType=GATTDBFindCharValueHandleInServiceRequestUuidType.Uuid16Bits, Uuid=[]):
        '''
        @param ServiceHandle: The handle of the Service declaration
        @param UuidType: UUID type
        @param Uuid: UUID value
        '''
        self.ServiceHandle = ServiceHandle
        self.UuidType = UuidType
        self.Uuid = Uuid


class GATTDBFindCccdHandleForCharValueHandleRequest(object):

    def __init__(self, CharValueHandle=bytearray(2)):
        '''
        @param CharValueHandle: The handle of the characteristic value
        '''
        self.CharValueHandle = CharValueHandle


class GATTDBFindDescriptorHandleForCharValueHandleRequest(object):

    def __init__(self, CharValueHandle=bytearray(2), UuidType=GATTDBFindDescriptorHandleForCharValueHandleRequestUuidType.Uuid16Bits, Uuid=[]):
        '''
        @param CharValueHandle: The handle of the characteristic value
        @param UuidType: UUID type
        @param Uuid: UUID value
        '''
        self.CharValueHandle = CharValueHandle
        self.UuidType = UuidType
        self.Uuid = Uuid


class GATTDBDynamicInitRequest(object):

    pass


class GATTDBDynamicReleaseDatabaseRequest(object):

    pass


class GATTDBDynamicAddPrimaryServiceDeclarationRequest(object):

    def __init__(self, UuidType=GATTDBDynamicAddPrimaryServiceDeclarationRequestUuidType.Uuid16Bits, Uuid=[]):
        '''
        @param UuidType: UUID type
        @param Uuid: UUID value
        '''
        self.UuidType = UuidType
        self.Uuid = Uuid


class GATTDBDynamicAddSecondaryServiceDeclarationRequest(object):

    def __init__(self, UuidType=GATTDBDynamicAddSecondaryServiceDeclarationRequestUuidType.Uuid16Bits, Uuid=[]):
        '''
        @param UuidType: UUID type
        @param Uuid: UUID value
        '''
        self.UuidType = UuidType
        self.Uuid = Uuid


class GATTDBDynamicAddIncludeDeclarationRequest(object):

    def __init__(self, IncludedServiceHandle=bytearray(2), EndGroupHandle=bytearray(2), UuidType=GATTDBDynamicAddIncludeDeclarationRequestUuidType.Uuid16Bits, Uuid=[]):
        '''
        @param IncludedServiceHandle: Handle of the included Service declaration
        @param EndGroupHandle: Last handle of the included Service
        @param UuidType: UUID type
        @param Uuid: UUID value
        '''
        self.IncludedServiceHandle = IncludedServiceHandle
        self.EndGroupHandle = EndGroupHandle
        self.UuidType = UuidType
        self.Uuid = Uuid


class GATTDBDynamicAddCharacteristicDeclarationAndValueRequest(object):

    def __init__(self, UuidType=GATTDBDynamicAddCharacteristicDeclarationAndValueRequestUuidType.Uuid16Bits, Uuid=[], CharacteristicProperties=GATTDBDynamicAddCharacteristicDeclarationAndValueRequestCharacteristicProperties.gNone_c, MaxValueLength=bytearray(2), InitialValueLength=bytearray(2), InitialValue=[], ValueAccessPermissions=GATTDBDynamicAddCharacteristicDeclarationAndValueRequestValueAccessPermissions.gPermissionNone_c):
        '''
        @param UuidType: UUID type
        @param Uuid: UUID value
        @param CharacteristicProperties: Characteristic properties
        @param MaxValueLength: If the Characteristic Value length is variable, this is the maximum length; for fixed lengths this must be set to 0
        @param InitialValueLength: Value length at initialization; remains fixed if maxValueLength is set to 0, otherwise cannot be greater than maxValueLength
        @param InitialValue: Contains the initial value of the Characteristic
        @param ValueAccessPermissions: Access permissions for the value attribute
        '''
        self.UuidType = UuidType
        self.Uuid = Uuid
        self.CharacteristicProperties = CharacteristicProperties
        self.MaxValueLength = MaxValueLength
        self.InitialValueLength = InitialValueLength
        self.InitialValue = InitialValue
        self.ValueAccessPermissions = ValueAccessPermissions


class GATTDBDynamicAddCharacteristicDescriptorRequest(object):

    def __init__(self, UuidType=GATTDBDynamicAddCharacteristicDescriptorRequestUuidType.Uuid16Bits, Uuid=[], DescriptorValueLength=bytearray(2), Value=[], DescriptorAccessPermissions=GATTDBDynamicAddCharacteristicDescriptorRequestDescriptorAccessPermissions.gPermissionNone_c):
        '''
        @param UuidType: UUID type
        @param Uuid: UUID value
        @param DescriptorValueLength: Fixed length of the descriptor attribute value
        @param Value: Initial value of the descriptor attribute
        @param DescriptorAccessPermissions: Access permissions for the descriptor attribute
        '''
        self.UuidType = UuidType
        self.Uuid = Uuid
        self.DescriptorValueLength = DescriptorValueLength
        self.Value = Value
        self.DescriptorAccessPermissions = DescriptorAccessPermissions


class GATTDBDynamicAddCccdRequest(object):

    pass


class GATTDBDynamicAddCharacteristicDeclarationWithUniqueValueRequest(object):

    def __init__(self, UuidType=GATTDBDynamicAddCharacteristicDeclarationWithUniqueValueRequestUuidType.Uuid16Bits, Uuid=[], CharacteristicProperties=GATTDBDynamicAddCharacteristicDeclarationWithUniqueValueRequestCharacteristicProperties.gNone_c, ValueAccessPermissions=GATTDBDynamicAddCharacteristicDeclarationWithUniqueValueRequestValueAccessPermissions.gPermissionNone_c):
        '''
        @param UuidType: UUID type
        @param Uuid: UUID value
        @param CharacteristicProperties: Characteristic properties
        @param ValueAccessPermissions: Access permissions for the value attribute
        '''
        self.UuidType = UuidType
        self.Uuid = Uuid
        self.CharacteristicProperties = CharacteristicProperties
        self.ValueAccessPermissions = ValueAccessPermissions


class GATTDBDynamicRemoveServiceRequest(object):

    def __init__(self, ServiceHandle=bytearray(2)):
        '''
        @param ServiceHandle: Attribute handle of the Service declaration
        '''
        self.ServiceHandle = ServiceHandle


class GATTDBDynamicRemoveCharacteristicRequest(object):

    def __init__(self, CharacteristicHandle=bytearray(2)):
        '''
        @param CharacteristicHandle: Attribute handle of the Characteristic declaration
        '''
        self.CharacteristicHandle = CharacteristicHandle


class GATTDBAttModeSelectRequest(object):

    def __init__(self, Enable=bytearray(1)):
        '''
        @param Enable: Enable/disable GATT Database (ATT)
        '''
        self.Enable = Enable


class GATTDBAttFindInformationRequest(object):

    def __init__(self, DeviceId=bytearray(1), Params_StartingHandle=bytearray(2), Params_EndingHandle=bytearray(2)):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_StartingHandle: Starting handle
        @param Params_EndingHandle: Ending handle
        '''
        self.DeviceId = DeviceId
        self.Params_StartingHandle = Params_StartingHandle
        self.Params_EndingHandle = Params_EndingHandle


class GATTDBAttFindByTypeValueRequest(object):

    def __init__(self, DeviceId=bytearray(1), Params_StartingHandle=bytearray(2), Params_EndingHandle=bytearray(2), Params_AttributeType=bytearray(2), Params_AttributeLength=bytearray(2), Params_AttributeValue=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_StartingHandle: Starting handle
        @param Params_EndingHandle: Ending handle
        @param Params_AttributeType: Attribute type
        @param Params_AttributeLength: Attribute length
        @param Params_AttributeValue: Attribute value
        '''
        self.DeviceId = DeviceId
        self.Params_StartingHandle = Params_StartingHandle
        self.Params_EndingHandle = Params_EndingHandle
        self.Params_AttributeType = Params_AttributeType
        self.Params_AttributeLength = Params_AttributeLength
        self.Params_AttributeValue = Params_AttributeValue


class GATTDBAttReadByTypeRequest(object):

    def __init__(self, DeviceId=bytearray(1), Params_StartingHandle=bytearray(2), Params_EndingHandle=bytearray(2), Params_Format=GATTDBAttReadByTypeRequestParams_Format(), Params_AttributeType=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_StartingHandle: Starting handle
        @param Params_EndingHandle: Ending handle
        @param Params_Format: ATT Uuid format
        @param Params_AttributeType: Attribute type
        '''
        self.DeviceId = DeviceId
        self.Params_StartingHandle = Params_StartingHandle
        self.Params_EndingHandle = Params_EndingHandle
        self.Params_Format = Params_Format
        self.Params_AttributeType = Params_AttributeType


class GATTDBAttReadRequest(object):

    def __init__(self, DeviceId=bytearray(1), Params_AttributeHandle=bytearray(2)):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_AttributeHandle: Attribute handle
        '''
        self.DeviceId = DeviceId
        self.Params_AttributeHandle = Params_AttributeHandle


class GATTDBAttReadBlobRequest(object):

    def __init__(self, DeviceId=bytearray(1), Params_AttributeHandle=bytearray(2), Params_ValueOffset=bytearray(2)):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_AttributeHandle: Attribute handle
        @param Params_ValueOffset: Value offset
        '''
        self.DeviceId = DeviceId
        self.Params_AttributeHandle = Params_AttributeHandle
        self.Params_ValueOffset = Params_ValueOffset


class GATTDBAttReadMultipleRequest(object):

    class Params_ListOfHandles(object):

        def __init__(self, Handle=bytearray(2)):
            self.Handle = Handle

        def pickle(self):
            result = bytearray()
            result += to_bytes(self.Handle, 2)
            return result

    def __init__(self, DeviceId=bytearray(1), Params_HandleCount=bytearray(2), Params_ListOfHandles=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_HandleCount: Number of handles
        @param Params_ListOfHandles: List of handles
        '''
        self.DeviceId = DeviceId
        self.Params_HandleCount = Params_HandleCount
        self.Params_ListOfHandles = Params_ListOfHandles

    def pickle(self):
        result = bytearray()
        result += to_bytes(self.DeviceId, 1)
        result += to_bytes(self.Params_HandleCount, 2)
        for i in range(list_to_int(self.Params_HandleCount, False)):
            result += self.Params_ListOfHandles[i].pickle()
        return result


class GATTDBAttReadByGroupTypeRequest(object):

    def __init__(self, DeviceId=bytearray(1), Params_StartingHandle=bytearray(2), Params_EndingHandle=bytearray(2), Params_Format=GATTDBAttReadByGroupTypeRequestParams_Format(), Params_AttributeType=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_StartingHandle: Starting handle
        @param Params_EndingHandle: Ending handle
        @param Params_Format: ATT Uuid format
        @param Params_AttributeType: Attribute type
        '''
        self.DeviceId = DeviceId
        self.Params_StartingHandle = Params_StartingHandle
        self.Params_EndingHandle = Params_EndingHandle
        self.Params_Format = Params_Format
        self.Params_AttributeType = Params_AttributeType


class GATTDBAttWriteRequest(object):

    def __init__(self, DeviceId=bytearray(1), Params_AttributeHandle=bytearray(2), Params_AttributeLength=bytearray(2), Params_AttributeValue=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_AttributeHandle: Attribute handle
        @param Params_AttributeLength: Attribute length
        @param Params_AttributeValue: Attribute value
        '''
        self.DeviceId = DeviceId
        self.Params_AttributeHandle = Params_AttributeHandle
        self.Params_AttributeLength = Params_AttributeLength
        self.Params_AttributeValue = Params_AttributeValue


class GATTDBAttWriteCommandRequest(object):

    def __init__(self, DeviceId=bytearray(1), Params_AttributeHandle=bytearray(2), Params_AttributeLength=bytearray(2), Params_AttributeValue=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_AttributeHandle: Attribute handle
        @param Params_AttributeLength: Attribute length
        @param Params_AttributeValue: Attribute value
        '''
        self.DeviceId = DeviceId
        self.Params_AttributeHandle = Params_AttributeHandle
        self.Params_AttributeLength = Params_AttributeLength
        self.Params_AttributeValue = Params_AttributeValue


class GATTDBAttSignedWriteCommandRequest(object):

    def __init__(self, DeviceId=bytearray(1), Params_AttributeHandle=bytearray(2), Params_AttributeLength=bytearray(2), Params_AttributeValue=[], Params_AuthenticationSignature=bytearray(12)):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_AttributeHandle: Attribute handle
        @param Params_AttributeLength: Attribute length
        @param Params_AttributeValue: Attribute value
        @param Params_AuthenticationSignature: Authentication signature
        '''
        self.DeviceId = DeviceId
        self.Params_AttributeHandle = Params_AttributeHandle
        self.Params_AttributeLength = Params_AttributeLength
        self.Params_AttributeValue = Params_AttributeValue
        self.Params_AuthenticationSignature = Params_AuthenticationSignature


class GATTDBAttPrepareWriteRequest(object):

    def __init__(self, DeviceId=bytearray(1), Params_AttributeHandle=bytearray(2), Params_ValueOffset=bytearray(2), Params_AttributeLength=bytearray(2), Params_AttributeValue=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_AttributeHandle: Attribute handle
        @param Params_ValueOffset: Value offset
        @param Params_AttributeLength: Attribute length
        @param Params_AttributeValue: Attribute value
        '''
        self.DeviceId = DeviceId
        self.Params_AttributeHandle = Params_AttributeHandle
        self.Params_ValueOffset = Params_ValueOffset
        self.Params_AttributeLength = Params_AttributeLength
        self.Params_AttributeValue = Params_AttributeValue


class GATTDBAttExecuteWriteRequest(object):

    def __init__(self, DeviceId=bytearray(1), Params_Flags=GATTDBAttExecuteWriteRequestParams_Flags.gCancel_c):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_Flags: Flags
        '''
        self.DeviceId = DeviceId
        self.Params_Flags = Params_Flags


class GATTDBAttExecuteWriteFromQueueRequest(object):

    def __init__(self, DeviceId=bytearray(1), Params_AttributeHandle=bytearray(2), Params_ValueOffset=bytearray(2), Params_AttributeLength=bytearray(2), Params_AttributeValue=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_AttributeHandle: Attribute handle
        @param Params_ValueOffset: Value offset
        @param Params_AttributeLength: Attribute length
        @param Params_AttributeValue: Attribute value
        '''
        self.DeviceId = DeviceId
        self.Params_AttributeHandle = Params_AttributeHandle
        self.Params_ValueOffset = Params_ValueOffset
        self.Params_AttributeLength = Params_AttributeLength
        self.Params_AttributeValue = Params_AttributeValue


class GATTDBAttPrepareNotificationIndicationRequest(object):

    def __init__(self, DeviceId=bytearray(1), Params_AttributeHandle=bytearray(2), Params_AttributeLength=bytearray(2), Params_AttributeValue=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_AttributeHandle: Attribute handle
        @param Params_AttributeLength: Attribute length
        @param Params_AttributeValue: Attribute value
        '''
        self.DeviceId = DeviceId
        self.Params_AttributeHandle = Params_AttributeHandle
        self.Params_AttributeLength = Params_AttributeLength
        self.Params_AttributeValue = Params_AttributeValue


class GAPModeSelectRequest(object):

    def __init__(self, Enable=bytearray(1)):
        '''
        @param Enable: Enable/disable GAP
        '''
        self.Enable = Enable


class BLEHostInitializeRequest(object):

    pass


class GAPRegisterDeviceSecurityRequirementsRequest(object):

    class SecurityRequirements(object):

        class MasterSecurityRequirements(object):

            def __init__(self, SecurityModeLevel=MasterSecurityRequirementsSecurityModeLevel.gMode1Level1_c, Authorization=bytearray(1), MinimumEncryptionKeySize=bytearray(2)):
                self.SecurityModeLevel = SecurityModeLevel
                self.Authorization = Authorization
                self.MinimumEncryptionKeySize = MinimumEncryptionKeySize

            def pickle(self):
                result = bytearray()
                result += to_bytes(self.SecurityModeLevel, 1)
                result += to_bytes(self.Authorization, 1)
                result += to_bytes(self.MinimumEncryptionKeySize, 2)
                return result

        class GapServiceSecurityRequirements(object):

            class Requirements(object):

                def __init__(self, SecurityModeLevel=RequirementsSecurityModeLevel.gMode1Level1_c, Authorization=bytearray(1), MinimumEncryptionKeySize=bytearray(2)):
                    self.SecurityModeLevel = SecurityModeLevel
                    self.Authorization = Authorization
                    self.MinimumEncryptionKeySize = MinimumEncryptionKeySize

                def pickle(self):
                    result = bytearray()
                    result += to_bytes(self.SecurityModeLevel, 1)
                    result += to_bytes(self.Authorization, 1)
                    result += to_bytes(self.MinimumEncryptionKeySize, 2)
                    return result

            def __init__(self, ServiceHandle=bytearray(2), Requirements=Requirements()):
                self.ServiceHandle = ServiceHandle
                self.Requirements = Requirements

            def pickle(self):
                result = bytearray()
                result += to_bytes(self.ServiceHandle, 2)
                result += self.Requirements.pickle()
                return result

        def __init__(self, MasterSecurityRequirements=MasterSecurityRequirements(), NbOfServices=bytearray(1), GapServiceSecurityRequirements=[]):
            self.MasterSecurityRequirements = MasterSecurityRequirements
            self.NbOfServices = NbOfServices
            # Array length depends on NbOfServices.
            self.GapServiceSecurityRequirements = GapServiceSecurityRequirements

        def pickle(self):
            result = bytearray()
            result += self.MasterSecurityRequirements.pickle()
            result += to_bytes(self.NbOfServices, 1)
            for i in range(list_to_int(self.NbOfServices, False)):
                result += self.GapServiceSecurityRequirements[i].pickle()
            return result

    def __init__(self, SecurityRequirementsIncluded=bytearray(1), SecurityRequirements=SecurityRequirements()):
        '''
        @param SecurityRequirementsIncluded: Boolean value which indicates if security requirements are included or not
        @param SecurityRequirements: Device security requirements
        '''
        self.SecurityRequirementsIncluded = SecurityRequirementsIncluded
        self.SecurityRequirements = SecurityRequirements

    def pickle(self):
        result = bytearray()
        result += to_bytes(self.SecurityRequirementsIncluded, 1)
        if self.SecurityRequirementsIncluded:
            result += self.SecurityRequirements.pickle()
        return result


class GAPSetAdvertisingParametersRequest(object):

    def __init__(self, AdvertisingParameters_MinInterval=bytearray(2), AdvertisingParameters_MaxInterval=bytearray(2), AdvertisingParameters_AdvertisingType=GAPSetAdvertisingParametersRequestAdvertisingParameters_AdvertisingType.gConnectableUndirected_c, AdvertisingParameters_OwnAddressType=GAPSetAdvertisingParametersRequestAdvertisingParameters_OwnAddressType.gPublic_c, AdvertisingParameters_PeerAddressType=GAPSetAdvertisingParametersRequestAdvertisingParameters_PeerAddressType.gPublic_c, AdvertisingParameters_PeerAddress=bytearray(6), AdvertisingParameters_ChannelMap=GAPSetAdvertisingParametersRequestAdvertisingParameters_ChannelMap.gChannel37_c, AdvertisingParameters_FilterPolicy=GAPSetAdvertisingParametersRequestAdvertisingParameters_FilterPolicy.gProcessAll_c):
        '''
        @param AdvertisingParameters_MinInterval: Minimum desired advertising interval - default 1.28s
        @param AdvertisingParameters_MaxInterval: Maximum desired advertising interval - default 1.28s
        @param AdvertisingParameters_AdvertisingType: Advertising type - default connectable undirected
        @param AdvertisingParameters_OwnAddressType: Indicates whether the advertising address will be the public address or the random address
        @param AdvertisingParameters_PeerAddressType: Address type of the peer; only used in directed advertising and Enhanced Privacy
        @param AdvertisingParameters_PeerAddress: Address of the peer
        @param AdvertisingParameters_ChannelMap: Bit mask indicating which of the three advertising channels will be used - default all three
        @param AdvertisingParameters_FilterPolicy: Indicates whether the connect and scan requests are filtered using the White List - default does not use White List
        '''
        self.AdvertisingParameters_MinInterval = AdvertisingParameters_MinInterval
        self.AdvertisingParameters_MaxInterval = AdvertisingParameters_MaxInterval
        self.AdvertisingParameters_AdvertisingType = AdvertisingParameters_AdvertisingType
        self.AdvertisingParameters_OwnAddressType = AdvertisingParameters_OwnAddressType
        self.AdvertisingParameters_PeerAddressType = AdvertisingParameters_PeerAddressType
        self.AdvertisingParameters_PeerAddress = AdvertisingParameters_PeerAddress
        self.AdvertisingParameters_ChannelMap = AdvertisingParameters_ChannelMap
        self.AdvertisingParameters_FilterPolicy = AdvertisingParameters_FilterPolicy


class GAPSetAdvertisingDataRequest(object):

    class AdvertisingData(object):

        class AdStructures(object):

            def __init__(self, Length=bytearray(1), Type=AdStructuresType(), Data=[]):
                self.Length = Length
                self.Type = Type
                # Array length depends on Length.
                self.Data = Data

            def pickle(self):
                result = bytearray()
                result += to_bytes(self.Length, 1)
                result += to_bytes(self.Type, 1)
                result += to_bytes(self.Data, self.Length)
                return result

        def __init__(self, NbOfAdStructures=bytearray(1), AdStructures=[]):
            self.NbOfAdStructures = NbOfAdStructures
            # Array length depends on NbOfAdStructures.
            self.AdStructures = AdStructures

        def pickle(self):
            result = bytearray()
            result += to_bytes(self.NbOfAdStructures, 1)
            for i in range(list_to_int(self.NbOfAdStructures, False)):
                result += self.AdStructures[i].pickle()
            return result

    class ScanResponseData(object):

        class AdStructures(object):

            def __init__(self, Length=bytearray(1), Type=AdStructuresType(), Data=[]):
                self.Length = Length
                self.Type = Type
                # Array length depends on Length.
                self.Data = Data

            def pickle(self):
                result = bytearray()
                result += to_bytes(self.Length, 1)
                result += to_bytes(self.Type, 1)
                result += to_bytes(self.Data, self.Length)
                return result

        def __init__(self, NbOfAdStructures=bytearray(1), AdStructures=[]):
            self.NbOfAdStructures = NbOfAdStructures
            # Array length depends on NbOfAdStructures.
            self.AdStructures = AdStructures

        def pickle(self):
            result = bytearray()
            result += to_bytes(self.NbOfAdStructures, 1)
            for i in range(list_to_int(self.NbOfAdStructures, False)):
                result += self.AdStructures[i].pickle()
            return result

    def __init__(self, AdvertisingDataIncluded=bytearray(1), AdvertisingData=AdvertisingData(), ScanResponseDataIncluded=bytearray(1), ScanResponseData=ScanResponseData()):
        '''
        @param AdvertisingDataIncluded: Boolean value which indicates if advertising data is included or not
        @param AdvertisingData: Advertising data
        @param ScanResponseDataIncluded: Boolean value which indicates if scan response data is included or not
        @param ScanResponseData: Scan response data
        '''
        self.AdvertisingDataIncluded = AdvertisingDataIncluded
        self.AdvertisingData = AdvertisingData
        self.ScanResponseDataIncluded = ScanResponseDataIncluded
        self.ScanResponseData = ScanResponseData

    def pickle(self):
        result = bytearray()
        result += to_bytes(self.AdvertisingDataIncluded, 1)
        if self.AdvertisingDataIncluded:
            result += self.AdvertisingData.pickle()
        result += to_bytes(self.ScanResponseDataIncluded, 1)
        if self.ScanResponseDataIncluded:
            result += self.ScanResponseData.pickle()
        return result


class GAPStartAdvertisingRequest(object):

    pass


class GAPStopAdvertisingRequest(object):

    pass


class GAPAuthorizeRequest(object):

    def __init__(self, DeviceId=bytearray(1), Handle=bytearray(2), Access=GAPAuthorizeRequestAccess.gRead_c):
        '''
        @param DeviceId: The peer being authorized
        @param Handle: The attribute handle
        @param Access: GATT Database access type (the type of access granted - read or write)
        '''
        self.DeviceId = DeviceId
        self.Handle = Handle
        self.Access = Access


class GAPSaveCccdRequest(object):

    def __init__(self, DeviceId=bytearray(1), Handle=bytearray(2), Cccd=GAPSaveCccdRequestCccd.gEmpty_c):
        '''
        @param DeviceId: The peer GATT client
        @param Handle: The handle of the CCCD as defined in the GATT Database
        @param Cccd: The bit mask representing the CCCD value to be saved
        '''
        self.DeviceId = DeviceId
        self.Handle = Handle
        self.Cccd = Cccd


class GAPCheckNotificationStatusRequest(object):

    def __init__(self, DeviceId=bytearray(1), Handle=bytearray(2)):
        '''
        @param DeviceId: The peer GATT client
        @param Handle: The handle of the CCCD
        '''
        self.DeviceId = DeviceId
        self.Handle = Handle


class GAPCheckIndicationStatusRequest(object):

    def __init__(self, DeviceId=bytearray(1), Handle=bytearray(2)):
        '''
        @param DeviceId: The peer GATT client
        @param Handle: The handle of the CCCD
        '''
        self.DeviceId = DeviceId
        self.Handle = Handle


class GAPGetBondedStaticAddressesRequest(object):

    def __init__(self, MaxNbOfDevices=bytearray(1)):
        '''
        @param MaxNbOfDevices: Maximum number of addresses to be obtained
        '''
        self.MaxNbOfDevices = MaxNbOfDevices


class GAPPairRequest(object):

    def __init__(self, DeviceId=bytearray(1), PairingParameters_WithBonding=bytearray(1), PairingParameters_SecurityModeAndLevel=GAPPairRequestPairingParameters_SecurityModeAndLevel.gMode1Level1_c, PairingParameters_MaxEncryptionKeySize=bytearray(1), PairingParameters_LocalIoCapabilities=GAPPairRequestPairingParameters_LocalIoCapabilities.gIoDisplayOnly_c, PairingParameters_OobAvailable=bytearray(1), PairingParameters_CentralKeys=GAPPairRequestPairingParameters_CentralKeys.gNoKeys_c, PairingParameters_PeripheralKeys=GAPPairRequestPairingParameters_PeripheralKeys.gNoKeys_c, PairingParameters_LeSecureConnectionSupported=bytearray(1), PairingParameters_UseKeypressNotifications=bytearray(1)):
        '''
        @param DeviceId: The peer to pair with
        @param PairingParameters_WithBonding: TRUE if this device is able to and wants to bond after pairing, FALSE otherwise
        @param PairingParameters_SecurityModeAndLevel: The desired security mode-level
        @param PairingParameters_MaxEncryptionKeySize: Maximum LTK size supported by the device
        @param PairingParameters_LocalIoCapabilities: I/O capabilities used to determine the pairing method
        @param PairingParameters_OobAvailable: TRUE if this device has Out-of-Band data that can be used for authenticated pairing, FALSE otherwise
        @param PairingParameters_CentralKeys: SMP keys to be distributed by the Central
        @param PairingParameters_PeripheralKeys: SMP keys to be distributed by the Peripheral
        @param PairingParameters_LeSecureConnectionSupported: indicates if device supports LE Secure Connections pairing
        @param PairingParameters_UseKeypressNotifications: Indicates if device supports Keypress Notification PDUs during Passkey Entry pairing
        '''
        self.DeviceId = DeviceId
        self.PairingParameters_WithBonding = PairingParameters_WithBonding
        self.PairingParameters_SecurityModeAndLevel = PairingParameters_SecurityModeAndLevel
        self.PairingParameters_MaxEncryptionKeySize = PairingParameters_MaxEncryptionKeySize
        self.PairingParameters_LocalIoCapabilities = PairingParameters_LocalIoCapabilities
        self.PairingParameters_OobAvailable = PairingParameters_OobAvailable
        self.PairingParameters_CentralKeys = PairingParameters_CentralKeys
        self.PairingParameters_PeripheralKeys = PairingParameters_PeripheralKeys
        self.PairingParameters_LeSecureConnectionSupported = PairingParameters_LeSecureConnectionSupported
        self.PairingParameters_UseKeypressNotifications = PairingParameters_UseKeypressNotifications


class GAPSendSlaveSecurityRequestRequest(object):

    def __init__(self, DeviceId=bytearray(1), BondAfterPairing=bytearray(1), SecurityModeLevel=GAPSendSlaveSecurityRequestRequestSecurityModeLevel.gMode1Level1_c):
        '''
        @param DeviceId: The GAP peer to pair with
        @param BondAfterPairing: Specifies if bonding is supported
        @param SecurityModeLevel: The level of security requested
        '''
        self.DeviceId = DeviceId
        self.BondAfterPairing = BondAfterPairing
        self.SecurityModeLevel = SecurityModeLevel


class GAPEncryptLinkRequest(object):

    def __init__(self, DeviceId=bytearray(1)):
        '''
        @param DeviceId: Device ID of the peer
        '''
        self.DeviceId = DeviceId


class GAPAcceptPairingRequestRequest(object):

    def __init__(self, DeviceId=bytearray(1), PairingParameters_WithBonding=bytearray(1), PairingParameters_SecurityModeAndLevel=GAPAcceptPairingRequestRequestPairingParameters_SecurityModeAndLevel.gMode1Level1_c, PairingParameters_MaxEncryptionKeySize=bytearray(1), PairingParameters_LocalIoCapabilities=GAPAcceptPairingRequestRequestPairingParameters_LocalIoCapabilities.gIoDisplayOnly_c, PairingParameters_OobAvailable=bytearray(1), PairingParameters_CentralKeys=GAPAcceptPairingRequestRequestPairingParameters_CentralKeys.gNoKeys_c, PairingParameters_PeripheralKeys=GAPAcceptPairingRequestRequestPairingParameters_PeripheralKeys.gNoKeys_c, PairingParameters_LeSecureConnectionSupported=bytearray(1), PairingParameters_UseKeypressNotifications=bytearray(1)):
        '''
        @param DeviceId: The peer requesting authentication
        @param PairingParameters_WithBonding: TRUE if this device is able to and wants to bond after pairing, FALSE otherwise
        @param PairingParameters_SecurityModeAndLevel: The desired security mode-level
        @param PairingParameters_MaxEncryptionKeySize: Maximum LTK size supported by the device
        @param PairingParameters_LocalIoCapabilities: I/O capabilities used to determine the pairing method
        @param PairingParameters_OobAvailable: TRUE if this device has Out-of-Band data that can be used for authenticated pairing, FALSE otherwise
        @param PairingParameters_CentralKeys: SMP keys to be distributed by the Central
        @param PairingParameters_PeripheralKeys: SMP keys to be distributed by the Peripheral
        @param PairingParameters_LeSecureConnectionSupported: indicates if device supports LE Secure Connections pairing
        @param PairingParameters_UseKeypressNotifications: Indicates if device supports Keypress Notification PDUs during Passkey Entry pairing
        '''
        self.DeviceId = DeviceId
        self.PairingParameters_WithBonding = PairingParameters_WithBonding
        self.PairingParameters_SecurityModeAndLevel = PairingParameters_SecurityModeAndLevel
        self.PairingParameters_MaxEncryptionKeySize = PairingParameters_MaxEncryptionKeySize
        self.PairingParameters_LocalIoCapabilities = PairingParameters_LocalIoCapabilities
        self.PairingParameters_OobAvailable = PairingParameters_OobAvailable
        self.PairingParameters_CentralKeys = PairingParameters_CentralKeys
        self.PairingParameters_PeripheralKeys = PairingParameters_PeripheralKeys
        self.PairingParameters_LeSecureConnectionSupported = PairingParameters_LeSecureConnectionSupported
        self.PairingParameters_UseKeypressNotifications = PairingParameters_UseKeypressNotifications


class GAPRejectPairingRequest(object):

    def __init__(self, DeviceId=bytearray(1), Reason=GAPRejectPairingRequestReason.gOobNotAvailable_c):
        '''
        @param DeviceId: The GAP peer who requested authentication
        @param Reason: Pairing reject reason
        '''
        self.DeviceId = DeviceId
        self.Reason = Reason


class GAPEnterPasskeyRequest(object):

    def __init__(self, DeviceId=bytearray(1), Passkey=bytearray(4)):
        '''
        @param DeviceId: The GAP peer that requested a passkey entry
        @param Passkey: The peer's secret passkey
        '''
        self.DeviceId = DeviceId
        self.Passkey = Passkey


class GAPProvideOobRequest(object):

    def __init__(self, DeviceId=bytearray(1), Oob=bytearray(16)):
        '''
        @param DeviceId: The pairing device
        @param Oob: OOB data
        '''
        self.DeviceId = DeviceId
        self.Oob = Oob


class GAPRejectPasskeyRequestRequest(object):

    def __init__(self, DeviceId=bytearray(1)):
        '''
        @param DeviceId: The GAP peer that requested a passkey entry
        '''
        self.DeviceId = DeviceId


class GAPSendSmpKeysRequest(object):

    class Keys_LtkInfo(object):

        def __init__(self, LtkSize=bytearray(1), Ltk=[]):
            self.LtkSize = LtkSize
            # Array length depends on LtkSize.
            self.Ltk = Ltk

        def pickle(self):
            result = bytearray()
            result += to_bytes(self.LtkSize, 1)
            result += to_bytes(self.Ltk, self.LtkSize)
            return result

    class Keys_RandEdivInfo(object):

        def __init__(self, RandSize=bytearray(1), Rand=[], Ediv=bytearray(2)):
            self.RandSize = RandSize
            # Array length depends on RandSize.
            self.Rand = Rand
            self.Ediv = Ediv

        def pickle(self):
            result = bytearray()
            result += to_bytes(self.RandSize, 1)
            result += to_bytes(self.Rand, self.RandSize)
            result += to_bytes(self.Ediv, 2)
            return result

    class Keys_AddressInfo(object):

        def __init__(self, DeviceAddressType=Keys_AddressInfoDeviceAddressType.gPublic_c, DeviceAddress=bytearray(6)):
            self.DeviceAddressType = DeviceAddressType
            # Unit length: 6 bytes
            self.DeviceAddress = DeviceAddress

        def pickle(self):
            result = bytearray()
            result += to_bytes(self.DeviceAddressType, 1)
            result += to_bytes(self.DeviceAddress, 6)
            return result

    def __init__(self, DeviceId=bytearray(1), Keys_LtkIncluded=bytearray(1), Keys_LtkInfo=Keys_LtkInfo(), Keys_IrkIncluded=bytearray(1), Keys_Irk=[], Keys_CsrkIncluded=bytearray(1), Keys_Csrk=[], Keys_RandEdivInfo=Keys_RandEdivInfo(), Keys_AddressIncluded=[], Keys_AddressInfo=Keys_AddressInfo()):
        '''
        @param DeviceId: The GAP peer who initiated the procedure
        @param Keys_LtkIncluded: Boolean value which indicates if LTK size and LTK are included or not
        @param Keys_LtkInfo: LTK information (size and value)
        @param Keys_IrkIncluded: Boolean value which indicates if IRK is included or not
        @param Keys_Irk: Identity Resolving Key
        @param Keys_CsrkIncluded: Boolean value which indicates if CSRK is included or not
        @param Keys_Csrk: Connection Signature Resolving Key
        @param Keys_RandEdivInfo: Rand information (size and value) and Ediv
        @param Keys_AddressIncluded: Boolean value which indicates if Address is included or not
        @param Keys_AddressInfo: Address information (type and value
        '''
        self.DeviceId = DeviceId
        self.Keys_LtkIncluded = Keys_LtkIncluded
        self.Keys_LtkInfo = Keys_LtkInfo
        self.Keys_IrkIncluded = Keys_IrkIncluded
        self.Keys_Irk = Keys_Irk
        self.Keys_CsrkIncluded = Keys_CsrkIncluded
        self.Keys_Csrk = Keys_Csrk
        self.Keys_RandEdivInfo = Keys_RandEdivInfo
        self.Keys_AddressIncluded = Keys_AddressIncluded
        self.Keys_AddressInfo = Keys_AddressInfo

    def pickle(self):
        result = bytearray()
        result += to_bytes(self.DeviceId, 1)
        result += to_bytes(self.Keys_LtkIncluded, 1)
        if self.Keys_LtkIncluded:
            result += self.Keys_LtkInfo.pickle()
        result += to_bytes(self.Keys_IrkIncluded, 1)
        if self.Keys_IrkIncluded:
            result += to_bytes(self.Keys_Irk, 16)
        result += to_bytes(self.Keys_CsrkIncluded, 1)
        if self.Keys_CsrkIncluded:
            result += to_bytes(self.Keys_Csrk, 16)
        if self.Keys_LtkIncluded:
            result += self.Keys_RandEdivInfo.pickle()
        if self.Keys_IrkIncluded:
            result += to_bytes(self.Keys_AddressIncluded, 1)
        if self.Keys_AddressIncluded:
            result += self.Keys_AddressInfo.pickle()
        return result


class GAPRejectKeyExchangeRequestRequest(object):

    def __init__(self, DeviceId=bytearray(1)):
        '''
        @param DeviceId: The GAP peer who requested the Key Exchange procedure
        '''
        self.DeviceId = DeviceId


class GAPProvideLongTermKeyRequest(object):

    def __init__(self, DeviceId=bytearray(1), LtkSize=bytearray(1), Ltk=[]):
        '''
        @param DeviceId: The GAP peer who requested encryption
        @param LtkSize: The Long Term Key size
        @param Ltk: The Long Term Key
        '''
        self.DeviceId = DeviceId
        self.LtkSize = LtkSize
        self.Ltk = Ltk


class GAPDenyLongTermKeyRequest(object):

    def __init__(self, DeviceId=bytearray(1)):
        '''
        @param DeviceId: The GAP peer who requested encryption
        '''
        self.DeviceId = DeviceId


class GAPLoadEncryptionInformationRequest(object):

    def __init__(self, DeviceId=bytearray(1)):
        '''
        @param DeviceId: Device ID of the peer
        '''
        self.DeviceId = DeviceId


class GAPSetLocalPasskeyRequest(object):

    def __init__(self, Passkey=bytearray(4)):
        '''
        @param Passkey: The SMP passkey
        '''
        self.Passkey = Passkey


class GAPStartScanningRequest(object):

    class ScanningParameters(object):

        def __init__(self, Type=ScanningParametersType.gPassive_c, Interval=bytearray(2), Window=bytearray(2), OwnAddressType=ScanningParametersOwnAddressType.gPublic_c, FilterPolicy=ScanningParametersFilterPolicy.gScanAll_c):
            self.Type = Type
            self.Interval = Interval
            self.Window = Window
            self.OwnAddressType = OwnAddressType
            self.FilterPolicy = FilterPolicy

        def pickle(self):
            result = bytearray()
            result += to_bytes(self.Type, 1)
            result += to_bytes(self.Interval, 2)
            result += to_bytes(self.Window, 2)
            result += to_bytes(self.OwnAddressType, 1)
            result += to_bytes(self.FilterPolicy, 1)
            return result

    def __init__(self, ScanningParametersIncluded=bytearray(1), ScanningParameters=ScanningParameters()):
        '''
        @param ScanningParametersIncluded: Boolean value which indicates if scanning parameters are included or not
        @param ScanningParameters: The scanning parameters
        '''
        self.ScanningParametersIncluded = ScanningParametersIncluded
        self.ScanningParameters = ScanningParameters

    def pickle(self):
        result = bytearray()
        result += to_bytes(self.ScanningParametersIncluded, 1)
        if self.ScanningParametersIncluded:
            result += self.ScanningParameters.pickle()
        return result


class GAPStopScanningRequest(object):

    pass


class GAPConnectRequest(object):

    def __init__(self, CreateConnectionParameters_ScanInterval=bytearray(2), CreateConnectionParameters_ScanWindow=bytearray(2), CreateConnectionParameters_FilterPolicy=GAPConnectRequestCreateConnectionParameters_FilterPolicy.gUseDeviceAddress_c, CreateConnectionParameters_OwnAddressType=GAPConnectRequestCreateConnectionParameters_OwnAddressType.gPublic_c, CreateConnectionParameters_PeerAddressType=GAPConnectRequestCreateConnectionParameters_PeerAddressType.gPublic_c, CreateConnectionParameters_PeerAddress=bytearray(6), CreateConnectionParameters_ConnIntervalMin=bytearray(2), CreateConnectionParameters_ConnIntervalMax=bytearray(2), CreateConnectionParameters_ConnLatency=bytearray(2), CreateConnectionParameters_SupervisionTimeout=bytearray(2), CreateConnectionParameters_ConnEventLengthMin=bytearray(2), CreateConnectionParameters_ConnEventLengthMax=bytearray(2), CreateConnectionParameters_usePeerIdentityAddress=bytearray(1)):
        '''
        @param CreateConnectionParameters_ScanInterval: Scanning interval - default 10ms
        @param CreateConnectionParameters_ScanWindow: Scanning window - default 10ms
        @param CreateConnectionParameters_FilterPolicy: Indicates whether the connection request is issued for a specific device or for all the devices in the White List - default specific device
        @param CreateConnectionParameters_OwnAddressType: Indicates whether the address used in connection requests will be the public address or the random address - default public address
        @param CreateConnectionParameters_PeerAddressType: When connecting to a specific device, this indicates that device's address type - default public address
        @param CreateConnectionParameters_PeerAddress: When connecting to a specific device, this indicates that device's address
        @param CreateConnectionParameters_ConnIntervalMin: The minimum desired connection interval - default 100ms
        @param CreateConnectionParameters_ConnIntervalMax: The maximum desired connection interval - default 200ms
        @param CreateConnectionParameters_ConnLatency: The desired connection latency (the maximum number of consecutive connection events the Slave is allowed to ignore) - default 0
        @param CreateConnectionParameters_SupervisionTimeout: The maximum time interval between consecutive over-the-air packets; if this timer expires, the connection is dropped - default 10s
        @param CreateConnectionParameters_ConnEventLengthMin: The minimum desired connection event length - default 0ms
        @param CreateConnectionParameters_ConnEventLengthMax: The maximum desired connection event length - default maximum possible, ~41 s
        @param CreateConnectionParameters_usePeerIdentityAddress: TRUE if the address defined in the peerAddressType and peerAddress is an identity address
        '''
        self.CreateConnectionParameters_ScanInterval = CreateConnectionParameters_ScanInterval
        self.CreateConnectionParameters_ScanWindow = CreateConnectionParameters_ScanWindow
        self.CreateConnectionParameters_FilterPolicy = CreateConnectionParameters_FilterPolicy
        self.CreateConnectionParameters_OwnAddressType = CreateConnectionParameters_OwnAddressType
        self.CreateConnectionParameters_PeerAddressType = CreateConnectionParameters_PeerAddressType
        self.CreateConnectionParameters_PeerAddress = CreateConnectionParameters_PeerAddress
        self.CreateConnectionParameters_ConnIntervalMin = CreateConnectionParameters_ConnIntervalMin
        self.CreateConnectionParameters_ConnIntervalMax = CreateConnectionParameters_ConnIntervalMax
        self.CreateConnectionParameters_ConnLatency = CreateConnectionParameters_ConnLatency
        self.CreateConnectionParameters_SupervisionTimeout = CreateConnectionParameters_SupervisionTimeout
        self.CreateConnectionParameters_ConnEventLengthMin = CreateConnectionParameters_ConnEventLengthMin
        self.CreateConnectionParameters_ConnEventLengthMax = CreateConnectionParameters_ConnEventLengthMax
        self.CreateConnectionParameters_usePeerIdentityAddress = CreateConnectionParameters_usePeerIdentityAddress


class GAPDisconnectRequest(object):

    def __init__(self, DeviceId=bytearray(1)):
        '''
        @param DeviceId: The connected peer to disconnect from
        '''
        self.DeviceId = DeviceId


class GAPSaveCustomPeerInformationRequest(object):

    def __init__(self, DeviceId=bytearray(1), Offset=bytearray(2), InfoSize=bytearray(2), Info=[]):
        '''
        @param DeviceId: Device ID of the GAP peer
        @param Offset: Data offset (offset from the beginning)
        @param InfoSize: Data size
        @param Info: Data
        '''
        self.DeviceId = DeviceId
        self.Offset = Offset
        self.InfoSize = InfoSize
        self.Info = Info


class GAPLoadCustomPeerInformationRequest(object):

    def __init__(self, DeviceId=bytearray(1), Offset=bytearray(2), InfoSize=bytearray(2)):
        '''
        @param DeviceId: Device ID of the GAP peer
        @param Offset: Data offset (offset from the beginning)
        @param InfoSize: Data size
        '''
        self.DeviceId = DeviceId
        self.Offset = Offset
        self.InfoSize = InfoSize


class GAPCheckIfBondedRequest(object):

    def __init__(self, DeviceId=bytearray(1)):
        '''
        @param DeviceId: Device ID of the GAP peer
        '''
        self.DeviceId = DeviceId


class GAPReadWhiteListSizeRequest(object):

    pass


class GAPClearWhiteListRequest(object):

    pass


class GAPAddDeviceToWhiteListRequest(object):

    def __init__(self, AddressType=GAPAddDeviceToWhiteListRequestAddressType.gPublic_c, Address=bytearray(6)):
        '''
        @param AddressType: The address type (public or random)
        @param Address: The address of the white-listed device
        '''
        self.AddressType = AddressType
        self.Address = Address


class GAPRemoveDeviceFromWhiteListRequest(object):

    def __init__(self, AddressType=GAPRemoveDeviceFromWhiteListRequestAddressType.gPublic_c, Address=bytearray(6)):
        '''
        @param AddressType: The address type (public or random)
        @param Address: The address of the white-listed device
        '''
        self.AddressType = AddressType
        self.Address = Address


class GAPReadPublicDeviceAddressRequest(object):

    pass


class GAPCreateRandomDeviceAddressRequest(object):

    def __init__(self, IrkIncluded=bytearray(1), Irk=[], RandomPartIncluded=[], RandomPart=[]):
        '''
        @param IrkIncluded: Boolean value which indicates if IRK is included or not
        @param Irk: The Identity Resolving Key to be used for a private resolvable address
        @param RandomPartIncluded: Boolean value which indicates if random part is included or not
        @param RandomPart: Array containing the random part of a Private Resolvable Address
        '''
        self.IrkIncluded = IrkIncluded
        self.Irk = Irk
        self.RandomPartIncluded = RandomPartIncluded
        self.RandomPart = RandomPart


class GAPSaveDeviceNameRequest(object):

    def __init__(self, DeviceId=bytearray(1), NameSize=bytearray(1), Name=[]):
        '''
        @param DeviceId: Device ID for the active peer which name is saved
        @param NameSize: Number of characters to be saved
        @param Name: Array of characters holding the name
        '''
        self.DeviceId = DeviceId
        self.NameSize = NameSize
        self.Name = Name


class GAPGetBondedDevicesCountRequest(object):

    pass


class GAPGetBondedDeviceNameRequest(object):

    def __init__(self, NvmIndex=bytearray(1), MaxNameSize=bytearray(1)):
        '''
        @param NvmIndex: Index of the device in NVM bonding area
        @param MaxNameSize: Maximum number of characters to be copied, including the terminating NULL character
        '''
        self.NvmIndex = NvmIndex
        self.MaxNameSize = MaxNameSize


class GAPRemoveBondRequest(object):

    def __init__(self, NvmIndex=bytearray(1)):
        '''
        @param NvmIndex: Index of the device in NVM bonding area
        '''
        self.NvmIndex = NvmIndex


class GAPRemoveAllBondsRequest(object):

    pass


class GAPReadRadioPowerLevelRequest(object):

    def __init__(self, TxReadType=GAPReadRadioPowerLevelRequestTxReadType.gTxPowerCurrentLevelInConnection_c, DeviceId=bytearray(1)):
        '''
        @param TxReadType: Transmission read type
        @param DeviceId: The DeviceId for which the command is intended
        '''
        self.TxReadType = TxReadType
        self.DeviceId = DeviceId


class GAPVerifyPrivateResolvableAddressRequest(object):

    def __init__(self, NvmIndex=bytearray(1), Address=bytearray(6)):
        '''
        @param NvmIndex: Index of the device in NVM bonding area whose IRK must be checked
        @param Address: The Private Resolvable Address to be verified
        '''
        self.NvmIndex = NvmIndex
        self.Address = Address


class GAPSetRandomAddressRequest(object):

    def __init__(self, Address=bytearray(6)):
        '''
        @param Address: The Private Resolvable, Private Non-Resolvable, or Static Random Address
        '''
        self.Address = Address


class GAPSetScanModeRequest(object):

    class AutoConnectParams_AutoConnectData(object):

        def __init__(self, ScanInterval=bytearray(2), ScanWindow=bytearray(2), FilterPolicy=AutoConnectParams_AutoConnectDataFilterPolicy.gUseDeviceAddress_c, OwnAddressType=AutoConnectParams_AutoConnectDataOwnAddressType.gPublic_c, PeerAddressType=AutoConnectParams_AutoConnectDataPeerAddressType.gPublic_c, PeerAddress=bytearray(6), ConnIntervalMin=bytearray(2), ConnIntervalMax=bytearray(2), ConnLatency=bytearray(2), SupervisionTimeout=bytearray(2), ConnEventLengthMin=bytearray(2), ConnEventLengthMax=bytearray(2), usePeerIdentityAddress=bytearray(1)):
            self.ScanInterval = ScanInterval
            self.ScanWindow = ScanWindow
            self.FilterPolicy = FilterPolicy
            self.OwnAddressType = OwnAddressType
            self.PeerAddressType = PeerAddressType
            # Unit length: 6 bytes
            self.PeerAddress = PeerAddress
            self.ConnIntervalMin = ConnIntervalMin
            self.ConnIntervalMax = ConnIntervalMax
            self.ConnLatency = ConnLatency
            self.SupervisionTimeout = SupervisionTimeout
            self.ConnEventLengthMin = ConnEventLengthMin
            self.ConnEventLengthMax = ConnEventLengthMax
            self.usePeerIdentityAddress = usePeerIdentityAddress

        def pickle(self):
            result = bytearray()
            result += to_bytes(self.ScanInterval, 2)
            result += to_bytes(self.ScanWindow, 2)
            result += to_bytes(self.FilterPolicy, 1)
            result += to_bytes(self.OwnAddressType, 1)
            result += to_bytes(self.PeerAddressType, 1)
            result += to_bytes(self.PeerAddress, 6)
            result += to_bytes(self.ConnIntervalMin, 2)
            result += to_bytes(self.ConnIntervalMax, 2)
            result += to_bytes(self.ConnLatency, 2)
            result += to_bytes(self.SupervisionTimeout, 2)
            result += to_bytes(self.ConnEventLengthMin, 2)
            result += to_bytes(self.ConnEventLengthMax, 2)
            result += to_bytes(self.usePeerIdentityAddress, 1)
            return result

    def __init__(self, ScanMode=GAPSetScanModeRequestScanMode.gDefaultScan_c, AutoConnectParams_NbOfAddresses=bytearray(1), AutoConnectParams_WriteInWhiteList=bytearray(1), AutoConnectParams_AutoConnectData=[]):
        '''
        @param ScanMode: The scan mode to be activated.
        @param AutoConnectParams_NbOfAddresses: Number of device addresses to automatically connect to
        @param AutoConnectParams_WriteInWhiteList: If set to TRUE, the device addresses will be written in the White List before scanning is enabled
        @param AutoConnectParams_AutoConnectData: The connection request parameters
        '''
        self.ScanMode = ScanMode
        self.AutoConnectParams_NbOfAddresses = AutoConnectParams_NbOfAddresses
        self.AutoConnectParams_WriteInWhiteList = AutoConnectParams_WriteInWhiteList
        self.AutoConnectParams_AutoConnectData = AutoConnectParams_AutoConnectData

    def pickle(self):
        result = bytearray()
        result += to_bytes(self.ScanMode, 1)
        result += to_bytes(self.AutoConnectParams_NbOfAddresses, 1)
        result += to_bytes(self.AutoConnectParams_WriteInWhiteList, 1)
        for i in range(list_to_int(self.AutoConnectParams_NbOfAddresses, False)):
            result += self.AutoConnectParams_AutoConnectData[i].pickle()
        return result


class GAPSetDefaultPairingParametersRequest(object):

    class PairingParameters(object):

        def __init__(self, WithBonding=bytearray(1), SecurityModeAndLevel=PairingParametersSecurityModeAndLevel.gMode1Level1_c, MaxEncryptionKeySize=bytearray(1), LocalIoCapabilities=PairingParametersLocalIoCapabilities.gIoDisplayOnly_c, OobAvailable=bytearray(1), CentralKeys=PairingParametersCentralKeys.gNoKeys_c, PeripheralKeys=PairingParametersPeripheralKeys.gNoKeys_c, LeSecureConnectionSupported=bytearray(1), UseKeypressNotifications=bytearray(1)):
            self.WithBonding = WithBonding
            self.SecurityModeAndLevel = SecurityModeAndLevel
            self.MaxEncryptionKeySize = MaxEncryptionKeySize
            self.LocalIoCapabilities = LocalIoCapabilities
            self.OobAvailable = OobAvailable
            self.CentralKeys = CentralKeys
            self.PeripheralKeys = PeripheralKeys
            self.LeSecureConnectionSupported = LeSecureConnectionSupported
            self.UseKeypressNotifications = UseKeypressNotifications

        def pickle(self):
            result = bytearray()
            result += to_bytes(self.WithBonding, 1)
            result += to_bytes(self.SecurityModeAndLevel, 1)
            result += to_bytes(self.MaxEncryptionKeySize, 1)
            result += to_bytes(self.LocalIoCapabilities, 1)
            result += to_bytes(self.OobAvailable, 1)
            result += to_bytes(self.CentralKeys, 1)
            result += to_bytes(self.PeripheralKeys, 1)
            result += to_bytes(self.LeSecureConnectionSupported, 1)
            result += to_bytes(self.UseKeypressNotifications, 1)
            return result

    def __init__(self, PairingParametersIncluded=bytearray(1), PairingParameters=PairingParameters()):
        '''
        @param PairingParametersIncluded: Boolean value which indicates if pairing parameters are included or not
        @param PairingParameters: Pairing parameters as required by the SMP
        '''
        self.PairingParametersIncluded = PairingParametersIncluded
        self.PairingParameters = PairingParameters

    def pickle(self):
        result = bytearray()
        result += to_bytes(self.PairingParametersIncluded, 1)
        if self.PairingParametersIncluded:
            result += self.PairingParameters.pickle()
        return result


class GAPUpdateConnectionParametersRequest(object):

    def __init__(self, DeviceId=bytearray(1), IntervalMin=bytearray(2), IntervalMax=bytearray(2), SlaveLatency=bytearray(2), TimeoutMultiplier=bytearray(2), MinCeLength=bytearray(2), MaxCeLength=bytearray(2)):
        '''
        @param DeviceId: The DeviceID for which the command is intended
        @param IntervalMin: The minimum value for the connection event interval
        @param IntervalMax: The maximum value for the connection event interval
        @param SlaveLatency: The slave latency parameter
        @param TimeoutMultiplier: The connection timeout parameter
        @param MinCeLength: The minimum connection event length
        @param MaxCeLength: The maximum connection event length
        '''
        self.DeviceId = DeviceId
        self.IntervalMin = IntervalMin
        self.IntervalMax = IntervalMax
        self.SlaveLatency = SlaveLatency
        self.TimeoutMultiplier = TimeoutMultiplier
        self.MinCeLength = MinCeLength
        self.MaxCeLength = MaxCeLength


class GAPEnableUpdateConnectionParametersRequest(object):

    def __init__(self, DeviceId=bytearray(1), Enable=bytearray(1)):
        '''
        @param DeviceId: The DeviceID for which the command is intended
        @param Enable: Allow/dissalow the parameters update
        '''
        self.DeviceId = DeviceId
        self.Enable = Enable


class GapUpdateLeDataLengthRequest(object):

    def __init__(self, DeviceId=bytearray(1), TxOctets=bytearray(2), TxTime=bytearray(2)):
        '''
        @param DeviceId: The DeviceID for which the command is intended
        @param TxOctets: Maximum transmission number of payload octets
        @param TxTime: Maximum transmission time
        '''
        self.DeviceId = DeviceId
        self.TxOctets = TxOctets
        self.TxTime = TxTime


class GAPControllerResetRequest(object):

    pass


class GAPEnableHostPrivacyRequest(object):

    def __init__(self, Enable=bytearray(1), Irk=[]):
        '''
        @param Enable: Enables or disables Host Privacy
        @param Irk: Local IRK to be used for Resolvable Private Address generation
        '''
        self.Enable = Enable
        self.Irk = Irk


class GAPEnableControllerPrivacyRequest(object):

    class PeerIdentities(object):

        def __init__(self, IdentityAddressType=PeerIdentitiesIdentityAddressType.gPublic_c, IdentityAddress=bytearray(6), Irk=bytearray(16)):
            self.IdentityAddressType = IdentityAddressType
            # Unit length: 6 bytes
            self.IdentityAddress = IdentityAddress
            # Unit length: 16 bytes
            self.Irk = Irk

        def pickle(self):
            result = bytearray()
            result += to_bytes(self.IdentityAddressType, 1)
            result += to_bytes(self.IdentityAddress, 6)
            result += to_bytes(self.Irk, 16)
            return result

    def __init__(self, Enable=bytearray(1), OwnIrk=[], PeerIdCount=[], PeerIdentities=[]):
        '''
        @param Enable: Enables or disables Controller Privacy
        @param OwnIrk: Local IRK
        @param PeerIdCount: Size of PeerIdentities array
        @param PeerIdentities: Array of peer identity addresses and IRKs
        '''
        self.Enable = Enable
        self.OwnIrk = OwnIrk
        self.PeerIdCount = PeerIdCount
        self.PeerIdentities = PeerIdentities

    def pickle(self):
        result = bytearray()
        result += to_bytes(self.Enable, 1)
        if self.Enable:
            result += to_bytes(self.OwnIrk, 16)
        if self.Enable:
            result += to_bytes(self.PeerIdCount, 1)
        for i in range(list_to_int(self.PeerIdCount, False)):
            result += self.PeerIdentities[i].pickle()
        return result


class GAPLeScRegeneratePublicKeyRequest(object):

    pass


class GAPLeScValidateNumericValueRequest(object):

    def __init__(self, DeviceId=bytearray(1), Valid=bytearray(1)):
        '''
        @param DeviceId: The DeviceID for which the command is intended
        @param Valid: TRUE if user has indicated that numeric values are matched, FALSE otherwise
        '''
        self.DeviceId = DeviceId
        self.Valid = Valid


class GAPLeScGetLocalOobDataRequest(object):

    pass


class GAPLeScSetPeerOobDataRequest(object):

    def __init__(self, DeviceId=bytearray(1), RandomValue=bytearray(16), ConfirmValue=bytearray(16)):
        '''
        @param DeviceId: The DeviceID for which the command is intended
        @param RandomValue: LE SC OOB r (Random value)
        @param ConfirmValue: LE SC OOB Cr (Random Confirm value)
        '''
        self.DeviceId = DeviceId
        self.RandomValue = RandomValue
        self.ConfirmValue = ConfirmValue


class GAPLeScSendKeypressNotificationPrivacyRequest(object):

    def __init__(self, DeviceId=bytearray(1), GapLeScKeypressNotificationParams_keypressNotifType=GAPLeScSendKeypressNotificationPrivacyRequestGapLeScKeypressNotificationParams_keypressNotifType.gKnPasskeyEntryStarted_c):
        '''
        @param DeviceId: The DeviceID for which the command is intended
        @param GapLeScKeypressNotificationParams_keypressNotifType: keypressNotifType
        '''
        self.DeviceId = DeviceId
        self.GapLeScKeypressNotificationParams_keypressNotifType = GapLeScKeypressNotificationParams_keypressNotifType


class GAPGetBondedDevicesIdentityInformationRequest(object):

    def __init__(self, maxDevices=bytearray(1)):
        '''
        @param maxDevices: Maximum number of addresses to be obtained
        '''
        self.maxDevices = maxDevices


class FSCIAllowDeviceToSleepRequest(object):

    def __init__(self, SignalHostWhenWakeUp=FSCIAllowDeviceToSleepRequestSignalHostWhenWakeUp.FALSE, DeepSleepDuration=bytearray(4), DeepSleepMode=bytearray(1)):
        '''
        @param SignalHostWhenWakeUp: Signal Host When Wake Up
        @param DeepSleepDuration: Deep sleep duration in units of 16 us. Default value is 5 s
        @param DeepSleepMode: Deep sleep mode selection.
        '''
        self.SignalHostWhenWakeUp = SignalHostWhenWakeUp
        self.DeepSleepDuration = DeepSleepDuration
        self.DeepSleepMode = DeepSleepMode


class FSCIGetWakeupReasonRequest(object):

    pass


class FSCIErrorIndication(object):

    def __init__(self, Status=FSCIErrorIndicationStatus.gFsciSuccess_c):
        '''
        @param Status: Occurred error
        '''
        self.Status = Status


class FSCIAllowDeviceToSleepConfirm(object):

    def __init__(self, Status=FSCIAllowDeviceToSleepConfirmStatus.gSuccess):
        '''
        @param Status: Status
        '''
        self.Status = Status


class FSCIWakeUpIndication(object):

    pass


class FSCIGetWakeupReasonResponse(object):

    def __init__(self, WakeUpReason=bytearray()):
        '''
        @param WakeUpReason: Wake Up Reason
        '''
        self.WakeUpReason = WakeUpReason


class HCIConfirm(object):

    def __init__(self, Status=HCIConfirmStatus.gBleSuccess_c):
        '''
        @param Status: The status of the HCI request
        '''
        self.Status = Status


class HCIEventIndication(object):

    def __init__(self, EventLength=bytearray(2), Event=[]):
        '''
        @param EventLength: The length of the HCI event
        @param Event: The HCI event
        '''
        self.EventLength = EventLength
        self.Event = Event


class HCIDataIndication(object):

    def __init__(self, DataLength=bytearray(2), Data=[]):
        '''
        @param DataLength: The length of the HCI data
        @param Data: The HCI data
        '''
        self.DataLength = DataLength
        self.Data = Data


class HCISynchronousDataIndication(object):

    def __init__(self, SynchronousDataLength=bytearray(2), SynchronousData=[]):
        '''
        @param SynchronousDataLength: The length of the HCI synchronous data
        @param SynchronousData: HCI received synchronous data
        '''
        self.SynchronousDataLength = SynchronousDataLength
        self.SynchronousData = SynchronousData


class L2CAPConfirm(object):

    def __init__(self, Status=L2CAPConfirmStatus.gBleSuccess_c):
        '''
        @param Status: The status of the L2CAP request
        '''
        self.Status = Status


class L2CAPAttDataIndication(object):

    def __init__(self, DeviceId=bytearray(1), PacketLength=bytearray(2), Packet=[]):
        '''
        @param DeviceId: The DeviceId from which ATT data packet is received
        @param PacketLength: Length of the received ATT data packet
        @param Packet: The received ATT data packet
        '''
        self.DeviceId = DeviceId
        self.PacketLength = PacketLength
        self.Packet = Packet


class L2CAPSmpDataIndication(object):

    def __init__(self, DeviceId=bytearray(1), PacketLength=bytearray(2), Packet=[]):
        '''
        @param DeviceId: The DeviceId from which SM data packet is received
        @param PacketLength: Length of the received SM data packet
        @param Packet: The received SM data packet
        '''
        self.DeviceId = DeviceId
        self.PacketLength = PacketLength
        self.Packet = Packet


class L2CAPLePsmConnectionRequestIndication(object):

    class LeCbConnectionRequest(object):

        def __init__(self, DeviceId=bytearray(1), LePsm=bytearray(2), PeerMtu=bytearray(2), PeerMps=bytearray(2), InitialCredits=bytearray(2)):
            self.DeviceId = DeviceId
            self.LePsm = LePsm
            self.PeerMtu = PeerMtu
            self.PeerMps = PeerMps
            self.InitialCredits = InitialCredits

    def __init__(self, InformationIncluded=bytearray(1), LeCbConnectionRequest=LeCbConnectionRequest()):
        '''
        @param InformationIncluded: Information is included or not
        @param LeCbConnectionRequest: Connection request event data
        '''
        self.InformationIncluded = InformationIncluded
        self.LeCbConnectionRequest = LeCbConnectionRequest


class L2CAPLePsmConnectionCompleteIndication(object):

    class LeCbConnectionComplete(object):

        def __init__(self, DeviceId=bytearray(1), ChannelId=bytearray(2), PeerMtu=bytearray(2), PeerMps=bytearray(2), InitialCredits=bytearray(2), Result=LeCbConnectionCompleteResult.gSuccessful_c):
            self.DeviceId = DeviceId
            self.ChannelId = ChannelId
            self.PeerMtu = PeerMtu
            self.PeerMps = PeerMps
            self.InitialCredits = InitialCredits
            self.Result = Result

    def __init__(self, InformationIncluded=bytearray(1), LeCbConnectionComplete=LeCbConnectionComplete()):
        '''
        @param InformationIncluded: Information is included or not
        @param LeCbConnectionComplete: Connection complete event data
        '''
        self.InformationIncluded = InformationIncluded
        self.LeCbConnectionComplete = LeCbConnectionComplete


class L2CAPLePsmDisconnectNotificationIndication(object):

    class LeCbDisconnection(object):

        def __init__(self, DeviceId=bytearray(1), ChannelId=bytearray(2)):
            self.DeviceId = DeviceId
            self.ChannelId = ChannelId

    def __init__(self, InformationIncluded=bytearray(1), LeCbDisconnection=LeCbDisconnection()):
        '''
        @param InformationIncluded: Information is included or not
        @param LeCbDisconnection: Disconnection event data
        '''
        self.InformationIncluded = InformationIncluded
        self.LeCbDisconnection = LeCbDisconnection


class L2CAPNoPeerCreditsIndication(object):

    class LeCbNoPeerCredits(object):

        def __init__(self, DeviceId=bytearray(1), ChannelId=bytearray(2)):
            self.DeviceId = DeviceId
            self.ChannelId = ChannelId

    def __init__(self, InformationIncluded=bytearray(1), LeCbNoPeerCredits=LeCbNoPeerCredits()):
        '''
        @param InformationIncluded: Information is included or not
        @param LeCbNoPeerCredits: No peer credits event data
        '''
        self.InformationIncluded = InformationIncluded
        self.LeCbNoPeerCredits = LeCbNoPeerCredits


class L2CAPLocalCreditsNotificationIndication(object):

    class LeCbLocalCreditsNotification(object):

        def __init__(self, DeviceId=bytearray(1), ChannelId=bytearray(2), LocalCredits=bytearray(2)):
            self.DeviceId = DeviceId
            self.ChannelId = ChannelId
            self.LocalCredits = LocalCredits

    def __init__(self, InformationIncluded=bytearray(1), LeCbLocalCreditsNotification=LeCbLocalCreditsNotification()):
        '''
        @param InformationIncluded: Information is included or not
        @param LeCbLocalCreditsNotification: Local credits notification event data
        '''
        self.InformationIncluded = InformationIncluded
        self.LeCbLocalCreditsNotification = LeCbLocalCreditsNotification


class L2CAPLeCbDataIndication(object):

    def __init__(self, DeviceId=bytearray(1), SrcCid=bytearray(2), PacketLength=bytearray(2), Packet=[]):
        '''
        @param DeviceId: The DeviceId from which data packet is received
        @param SrcCid: Source Channel Id
        @param PacketLength: Length of the received data packet
        @param Packet: The received data packet
        '''
        self.DeviceId = DeviceId
        self.SrcCid = SrcCid
        self.PacketLength = PacketLength
        self.Packet = Packet


class SMConfirm(object):

    def __init__(self, Status=SMConfirmStatus.gBleSuccess_c):
        '''
        @param Status: The status of the SM request
        '''
        self.Status = Status


class SMStatusIndication(object):

    def __init__(self, SourceDeviceId=bytearray(1), SmStatusParams_Status=SMStatusIndicationSmStatusParams_Status.gBleSuccess_c):
        '''
        @param SourceDeviceId: Device Id corresponding to the event
        @param SmStatusParams_Status: A status returned by the SM regarding it's functionality
        '''
        self.SourceDeviceId = SourceDeviceId
        self.SmStatusParams_Status = SmStatusParams_Status


class SMRemoteSecurityRequestIndication(object):

    class SmSecurityReqParams_AuthRequest(object):

        def __init__(self, BondingFlags=SmSecurityReqParams_AuthRequestBondingFlags.gNoBonding_c, Mitm=SmSecurityReqParams_AuthRequestMitm.gReqOff_c, sc=SmSecurityReqParams_AuthRequestsc.gReqOff_c, keypress=SmSecurityReqParams_AuthRequestkeypress.gReqOff_c):
            self.BondingFlags = BondingFlags
            self.Mitm = Mitm
            self.sc = sc
            self.keypress = keypress

    def __init__(self, SourceDeviceId=bytearray(1), SmSecurityReqParams_AuthRequest=SmSecurityReqParams_AuthRequest()):
        '''
        @param SourceDeviceId: Device Id corresponding to the event
        @param SmSecurityReqParams_AuthRequest: SM authentication request parameters
        '''
        self.SourceDeviceId = SourceDeviceId
        self.SmSecurityReqParams_AuthRequest = SmSecurityReqParams_AuthRequest


class SMRemotePairingRequestIndication(object):

    class SmPairingParams_AuthRequest(object):

        def __init__(self, BondingFlags=SmPairingParams_AuthRequestBondingFlags.gNoBonding_c, Mitm=SmPairingParams_AuthRequestMitm.gReqOff_c, sc=SmPairingParams_AuthRequestsc.gReqOff_c, keypress=SmPairingParams_AuthRequestkeypress.gReqOff_c):
            self.BondingFlags = BondingFlags
            self.Mitm = Mitm
            self.sc = sc
            self.keypress = keypress

    class SmPairingParams_InitatorKeyDistribution(object):

        def __init__(self, EncKey=bytearray(1), IdKey=bytearray(1), Sign=bytearray(1), LinkKey=bytearray(1), Reserved=bytearray(1)):
            self.EncKey = EncKey
            self.IdKey = IdKey
            self.Sign = Sign
            self.LinkKey = LinkKey
            self.Reserved = Reserved

    class SmPairingParams_ResponderKeyDistribution(object):

        def __init__(self, EncKey=bytearray(1), IdKey=bytearray(1), Sign=bytearray(1), LinkKey=bytearray(1), Reserved=bytearray(1)):
            self.EncKey = EncKey
            self.IdKey = IdKey
            self.Sign = Sign
            self.LinkKey = LinkKey
            self.Reserved = Reserved

    def __init__(self, SourceDeviceId=bytearray(1), SmPairingParams_IoCapabilities=SMRemotePairingRequestIndicationSmPairingParams_IoCapabilities.gDisplayOnly_c, SmPairingParams_OobDataFlag=SMRemotePairingRequestIndicationSmPairingParams_OobDataFlag.gAuthDataNotPresent_c, SmPairingParams_AuthRequest=SmPairingParams_AuthRequest(), SmPairingParams_MaxEncKeySize=bytearray(1), SmPairingParams_InitatorKeyDistribution=SmPairingParams_InitatorKeyDistribution(), SmPairingParams_ResponderKeyDistribution=SmPairingParams_ResponderKeyDistribution()):
        '''
        @param SourceDeviceId: Device Id corresponding to the event
        @param SmPairingParams_IoCapabilities: I/O capabilities
        @param SmPairingParams_OobDataFlag: OOB data flag
        @param SmPairingParams_AuthRequest: SM authentication request parameters
        @param SmPairingParams_MaxEncKeySize: Maximum encryption key size
        @param SmPairingParams_InitatorKeyDistribution: Initiator key distribution
        @param SmPairingParams_ResponderKeyDistribution: Responder key distribution
        '''
        self.SourceDeviceId = SourceDeviceId
        self.SmPairingParams_IoCapabilities = SmPairingParams_IoCapabilities
        self.SmPairingParams_OobDataFlag = SmPairingParams_OobDataFlag
        self.SmPairingParams_AuthRequest = SmPairingParams_AuthRequest
        self.SmPairingParams_MaxEncKeySize = SmPairingParams_MaxEncKeySize
        self.SmPairingParams_InitatorKeyDistribution = SmPairingParams_InitatorKeyDistribution
        self.SmPairingParams_ResponderKeyDistribution = SmPairingParams_ResponderKeyDistribution


class SMRemotePairingResponseIndication(object):

    class SmPairingParams_AuthRequest(object):

        def __init__(self, BondingFlags=SmPairingParams_AuthRequestBondingFlags.gNoBonding_c, Mitm=SmPairingParams_AuthRequestMitm.gReqOff_c, sc=SmPairingParams_AuthRequestsc.gReqOff_c, keypress=SmPairingParams_AuthRequestkeypress.gReqOff_c):
            self.BondingFlags = BondingFlags
            self.Mitm = Mitm
            self.sc = sc
            self.keypress = keypress

    class SmPairingParams_InitatorKeyDistribution(object):

        def __init__(self, EncKey=bytearray(1), IdKey=bytearray(1), Sign=bytearray(1), LinkKey=bytearray(1), Reserved=bytearray(1)):
            self.EncKey = EncKey
            self.IdKey = IdKey
            self.Sign = Sign
            self.LinkKey = LinkKey
            self.Reserved = Reserved

    class SmPairingParams_ResponderKeyDistribution(object):

        def __init__(self, EncKey=bytearray(1), IdKey=bytearray(1), Sign=bytearray(1), LinkKey=bytearray(1), Reserved=bytearray(1)):
            self.EncKey = EncKey
            self.IdKey = IdKey
            self.Sign = Sign
            self.LinkKey = LinkKey
            self.Reserved = Reserved

    def __init__(self, SourceDeviceId=bytearray(1), SmPairingParams_IoCapabilities=SMRemotePairingResponseIndicationSmPairingParams_IoCapabilities.gDisplayOnly_c, SmPairingParams_OobDataFlag=SMRemotePairingResponseIndicationSmPairingParams_OobDataFlag.gAuthDataNotPresent_c, SmPairingParams_AuthRequest=SmPairingParams_AuthRequest(), SmPairingParams_MaxEncKeySize=bytearray(1), SmPairingParams_InitatorKeyDistribution=SmPairingParams_InitatorKeyDistribution(), SmPairingParams_ResponderKeyDistribution=SmPairingParams_ResponderKeyDistribution()):
        '''
        @param SourceDeviceId: Device Id corresponding to the event
        @param SmPairingParams_IoCapabilities: I/O capabilities
        @param SmPairingParams_OobDataFlag: OOB data flag
        @param SmPairingParams_AuthRequest: SM authentication request parameters
        @param SmPairingParams_MaxEncKeySize: Maximum encryption key size
        @param SmPairingParams_InitatorKeyDistribution: Initiator key distribution
        @param SmPairingParams_ResponderKeyDistribution: Responder key distribution
        '''
        self.SourceDeviceId = SourceDeviceId
        self.SmPairingParams_IoCapabilities = SmPairingParams_IoCapabilities
        self.SmPairingParams_OobDataFlag = SmPairingParams_OobDataFlag
        self.SmPairingParams_AuthRequest = SmPairingParams_AuthRequest
        self.SmPairingParams_MaxEncKeySize = SmPairingParams_MaxEncKeySize
        self.SmPairingParams_InitatorKeyDistribution = SmPairingParams_InitatorKeyDistribution
        self.SmPairingParams_ResponderKeyDistribution = SmPairingParams_ResponderKeyDistribution


class SMPasskeyDisplayRequestIndication(object):

    def __init__(self, SourceDeviceId=bytearray(1), PasskeyDisplayReqParams_Passkey=bytearray(4)):
        '''
        @param SourceDeviceId: Device Id corresponding to the event
        @param PasskeyDisplayReqParams_Passkey: A number between 0 and 999.999 decimal
        '''
        self.SourceDeviceId = SourceDeviceId
        self.PasskeyDisplayReqParams_Passkey = PasskeyDisplayReqParams_Passkey


class SMPasskeyRequestIndication(object):

    def __init__(self, SourceDeviceId=bytearray(1), PasskeyReqParams_PasskeyType=SMPasskeyRequestIndicationPasskeyReqParams_PasskeyType.Passkey):
        '''
        @param SourceDeviceId: Device Id corresponding to the event
        @param PasskeyReqParams_PasskeyType: Decimal passkey from 0 to 999.999 or 16 byte OOB
        '''
        self.SourceDeviceId = SourceDeviceId
        self.PasskeyReqParams_PasskeyType = PasskeyReqParams_PasskeyType


class SMPairingKeysetRequestIndication(object):

    class PairingKeysetReqParams_RequestedKeys(object):

        def __init__(self, EncKey=bytearray(1), IdKey=bytearray(1), Sign=bytearray(1), LinkKey=bytearray(1), Reserved=bytearray(1)):
            self.EncKey = EncKey
            self.IdKey = IdKey
            self.Sign = Sign
            self.LinkKey = LinkKey
            self.Reserved = Reserved

    def __init__(self, SourceDeviceId=bytearray(1), PairingKeysetReqParams_RequestedKeys=PairingKeysetReqParams_RequestedKeys(), PairingKeysetReqParams_EncKeySize=bytearray(1)):
        '''
        @param SourceDeviceId: Device Id corresponding to the event
        @param PairingKeysetReqParams_RequestedKeys: Keys requested by the peer device to be exchanged during the SMP Pairing key distribution phase
        @param PairingKeysetReqParams_EncKeySize: Encryption key size negotiated by the SMP
        '''
        self.SourceDeviceId = SourceDeviceId
        self.PairingKeysetReqParams_RequestedKeys = PairingKeysetReqParams_RequestedKeys
        self.PairingKeysetReqParams_EncKeySize = PairingKeysetReqParams_EncKeySize


class SMPairingKeysetReceivedIndication(object):

    class PairingKeysetReceivedParams_KeyDistPayload(object):

        def __init__(self, Ltk=bytearray(16), Irk=bytearray(16), Csrk=bytearray(16), Ediv=bytearray(2), Rand=bytearray(8), BDAddr=bytearray(6), BDAddrType=PairingKeysetReceivedParams_KeyDistPayloadBDAddrType.gPublic_c):
            # Unit length: 16 bytes
            self.Ltk = Ltk
            # Unit length: 16 bytes
            self.Irk = Irk
            # Unit length: 16 bytes
            self.Csrk = Csrk
            self.Ediv = Ediv
            self.Rand = Rand
            # Unit length: 6 bytes
            self.BDAddr = BDAddr
            self.BDAddrType = BDAddrType

    class PairingKeysetReceivedParams_ReceivedKeys(object):

        def __init__(self, EncKey=bytearray(1), IdKey=bytearray(1), Sign=bytearray(1), LinkKey=bytearray(1), Reserved=bytearray(1)):
            self.EncKey = EncKey
            self.IdKey = IdKey
            self.Sign = Sign
            self.LinkKey = LinkKey
            self.Reserved = Reserved

    def __init__(self, SourceDeviceId=bytearray(1), PairingKeysetReceivedParams_KeyDistPayload=PairingKeysetReceivedParams_KeyDistPayload(), PairingKeysetReceivedParams_ReceivedKeys=PairingKeysetReceivedParams_ReceivedKeys(), PairingKeysetReceivedParams_EncKeySize=bytearray(1)):
        '''
        @param SourceDeviceId: Device Id corresponding to the event
        @param PairingKeysetReceivedParams_KeyDistPayload: Keys and additional data exchanged during the SMP Pairing key distribution phase
        @param PairingKeysetReceivedParams_ReceivedKeys: Keys received from the peer device during the SMP Pairing key distribution phase
        @param PairingKeysetReceivedParams_EncKeySize: Encryption key size negotiated by the SMP
        '''
        self.SourceDeviceId = SourceDeviceId
        self.PairingKeysetReceivedParams_KeyDistPayload = PairingKeysetReceivedParams_KeyDistPayload
        self.PairingKeysetReceivedParams_ReceivedKeys = PairingKeysetReceivedParams_ReceivedKeys
        self.PairingKeysetReceivedParams_EncKeySize = PairingKeysetReceivedParams_EncKeySize


class SMPairingCompleteIndication(object):

    def __init__(self, SourceDeviceId=bytearray(1), PairingCompleteParams_EncKeySize=bytearray(1), PairingCompleteParams_bMitmProtection=bytearray(1), PairingCompleteParams_bBonding=bytearray(1), PairingCompleteParams_LeSc=bytearray(1), PairingCompleteParams_LeScLtk=bytearray(16)):
        '''
        @param SourceDeviceId: Device Id corresponding to the event
        @param PairingCompleteParams_EncKeySize: Encryption key size negotiated by the SMP
        @param PairingCompleteParams_bMitmProtection: The pairing procedure was completed with MITM protection enabled
        @param PairingCompleteParams_bBonding: Both devices involved in the pairing procedure support bonding
        @param PairingCompleteParams_LeSc: The pairing procedure was performed using a LE Secure Connections pairing method
        @param PairingCompleteParams_LeScLtk: This is the LTK generated during a LE Secure Connections pairing procedure
        '''
        self.SourceDeviceId = SourceDeviceId
        self.PairingCompleteParams_EncKeySize = PairingCompleteParams_EncKeySize
        self.PairingCompleteParams_bMitmProtection = PairingCompleteParams_bMitmProtection
        self.PairingCompleteParams_bBonding = PairingCompleteParams_bBonding
        self.PairingCompleteParams_LeSc = PairingCompleteParams_LeSc
        self.PairingCompleteParams_LeScLtk = PairingCompleteParams_LeScLtk


class SMPairingFailedIndication(object):

    def __init__(self, SourceDeviceId=bytearray(1), SmPairingFailedParams_Reason=SMPairingFailedIndicationSmPairingFailedParams_Reason.gReserved_c):
        '''
        @param SourceDeviceId: Device Id corresponding to the event
        @param SmPairingFailedParams_Reason: Reason
        '''
        self.SourceDeviceId = SourceDeviceId
        self.SmPairingFailedParams_Reason = SmPairingFailedParams_Reason


class SMReceivedPairingFailedIndication(object):

    def __init__(self, SourceDeviceId=bytearray(1), SmPairingFailedParams_Reason=SMReceivedPairingFailedIndicationSmPairingFailedParams_Reason.gReserved_c):
        '''
        @param SourceDeviceId: Device Id corresponding to the event
        @param SmPairingFailedParams_Reason: Reason
        '''
        self.SourceDeviceId = SourceDeviceId
        self.SmPairingFailedParams_Reason = SmPairingFailedParams_Reason


class SMLlLtkRequestIndication(object):

    def __init__(self, SourceDeviceId=bytearray(1), LlLtkRequestParams_RandomNumber=bytearray(8), LlLtkRequestParams_EncryptedDiversifier=bytearray(2)):
        '''
        @param SourceDeviceId: Device Id corresponding to the event
        @param LlLtkRequestParams_RandomNumber: Random number
        @param LlLtkRequestParams_EncryptedDiversifier: Encrypted Diversifier
        '''
        self.SourceDeviceId = SourceDeviceId
        self.LlLtkRequestParams_RandomNumber = LlLtkRequestParams_RandomNumber
        self.LlLtkRequestParams_EncryptedDiversifier = LlLtkRequestParams_EncryptedDiversifier


class SMLlEncryptionStatusIndication(object):

    def __init__(self, SourceDeviceId=bytearray(1), LlEncryptionStatusParams_LlStatus=SMLlEncryptionStatusIndicationLlEncryptionStatusParams_LlStatus.gBleSuccess_c, LlEncryptionStatusParams_EncryptionState=SMLlEncryptionStatusIndicationLlEncryptionStatusParams_EncryptionState.gLinkEncryptionNoChange_c, LlEncryptionStatusParams_EncKeyType=SMLlEncryptionStatusIndicationLlEncryptionStatusParams_EncKeyType.gLinkEncKeyTypeNone_c):
        '''
        @param SourceDeviceId: Device Id corresponding to the event
        @param LlEncryptionStatusParams_LlStatus: Status returned by the Link Layer during the link encryption procedure
        @param LlEncryptionStatusParams_EncryptionState: State of the encryption of the link
        @param LlEncryptionStatusParams_EncKeyType: Type of key used to encrypt the link
        '''
        self.SourceDeviceId = SourceDeviceId
        self.LlEncryptionStatusParams_LlStatus = LlEncryptionStatusParams_LlStatus
        self.LlEncryptionStatusParams_EncryptionState = LlEncryptionStatusParams_EncryptionState
        self.LlEncryptionStatusParams_EncKeyType = LlEncryptionStatusParams_EncKeyType


class SMLlEncryptResIndication(object):

    def __init__(self, SourceDeviceId=bytearray(1), LlEncryptResParams_Status=SMLlEncryptResIndicationLlEncryptResParams_Status.gBleSuccess_c, LlEncryptResParams_EncryptedData=bytearray(16)):
        '''
        @param SourceDeviceId: Device Id corresponding to the event
        @param LlEncryptResParams_Status: The status of the request
        @param LlEncryptResParams_EncryptedData: Encrypted data
        '''
        self.SourceDeviceId = SourceDeviceId
        self.LlEncryptResParams_Status = LlEncryptResParams_Status
        self.LlEncryptResParams_EncryptedData = LlEncryptResParams_EncryptedData


class SMLlRandResIndication(object):

    def __init__(self, SourceDeviceId=bytearray(1), LlRandResParams_Status=SMLlRandResIndicationLlRandResParams_Status.gBleSuccess_c, LlRandResParams_RandomNumber=bytearray(8)):
        '''
        @param SourceDeviceId: Device Id corresponding to the event
        @param LlRandResParams_Status: The status of the request
        @param LlRandResParams_RandomNumber: Random number
        '''
        self.SourceDeviceId = SourceDeviceId
        self.LlRandResParams_Status = LlRandResParams_Status
        self.LlRandResParams_RandomNumber = LlRandResParams_RandomNumber


class SMTbCreateRandomDeviceAddrResIndication(object):

    def __init__(self, SourceDeviceId=bytearray(1), TbCreateRandomDeviceAddrResParams_Status=SMTbCreateRandomDeviceAddrResIndicationTbCreateRandomDeviceAddrResParams_Status.gBleSuccess_c, TbCreateRandomDeviceAddrResParams_AddrType=SMTbCreateRandomDeviceAddrResIndicationTbCreateRandomDeviceAddrResParams_AddrType.RandAddrTypeNone, TbCreateRandomDeviceAddrResParams_Addr=bytearray(6)):
        '''
        @param SourceDeviceId: Device Id corresponding to the event
        @param TbCreateRandomDeviceAddrResParams_Status: Status of the operation
        @param TbCreateRandomDeviceAddrResParams_AddrType: Type of BLE Random Address returned
        @param TbCreateRandomDeviceAddrResParams_Addr: Returned random address
        '''
        self.SourceDeviceId = SourceDeviceId
        self.TbCreateRandomDeviceAddrResParams_Status = TbCreateRandomDeviceAddrResParams_Status
        self.TbCreateRandomDeviceAddrResParams_AddrType = TbCreateRandomDeviceAddrResParams_AddrType
        self.TbCreateRandomDeviceAddrResParams_Addr = TbCreateRandomDeviceAddrResParams_Addr


class SMTbCheckResolvalePrivateAddrResIndication(object):

    def __init__(self, SourceDeviceId=bytearray(1), TbCheckResolvablePrivateAddrResParams_Status=SMTbCheckResolvalePrivateAddrResIndicationTbCheckResolvablePrivateAddrResParams_Status.gBleSuccess_c):
        '''
        @param SourceDeviceId: Device Id corresponding to the event
        @param TbCheckResolvablePrivateAddrResParams_Status: Status of the verification
        '''
        self.SourceDeviceId = SourceDeviceId
        self.TbCheckResolvablePrivateAddrResParams_Status = TbCheckResolvablePrivateAddrResParams_Status


class SMTbSignDataResIndication(object):

    def __init__(self, SourceDeviceId=bytearray(1), TbSignDataResParams_Status=SMTbSignDataResIndicationTbSignDataResParams_Status.gBleSuccess_c, TbSignDataResParams_Signature=bytearray(8)):
        '''
        @param SourceDeviceId: Device Id corresponding to the event
        @param TbSignDataResParams_Status: Status of the signing operation
        @param TbSignDataResParams_Signature: Signature for the provided data (64 bit CMAC)
        '''
        self.SourceDeviceId = SourceDeviceId
        self.TbSignDataResParams_Status = TbSignDataResParams_Status
        self.TbSignDataResParams_Signature = TbSignDataResParams_Signature


class SMTbVerifyDataSignatureResIndication(object):

    def __init__(self, SourceDeviceId=bytearray(1), TbVerifyDataSignatureResParams_Status=SMTbVerifyDataSignatureResIndicationTbVerifyDataSignatureResParams_Status.gBleSuccess_c):
        '''
        @param SourceDeviceId: Device Id corresponding to the event
        @param TbVerifyDataSignatureResParams_Status: Status of the signature verification operation
        '''
        self.SourceDeviceId = SourceDeviceId
        self.TbVerifyDataSignatureResParams_Status = TbVerifyDataSignatureResParams_Status


class SMRemoteKeypressNotificationIndication(object):

    def __init__(self, SourceDeviceId=bytearray(1), SmKeypressNotificationParams_keypressNotifType=SMRemoteKeypressNotificationIndicationSmKeypressNotificationParams_keypressNotifType.gSmpKeypressNotificationPasskeyEntryStarted_c):
        '''
        @param SourceDeviceId: Device Id corresponding to the event
        @param SmKeypressNotificationParams_keypressNotifType: keypressNotifType
        '''
        self.SourceDeviceId = SourceDeviceId
        self.SmKeypressNotificationParams_keypressNotifType = SmKeypressNotificationParams_keypressNotifType


class SMNcDisplayRequestIndication(object):

    def __init__(self, SourceDeviceId=bytearray(1), ncValue=bytearray(4)):
        '''
        @param SourceDeviceId: Device Id corresponding to the event
        @param ncValue: A number between 0 and 999999 decimal which must be confirmed by the user on both pairing devices
        '''
        self.SourceDeviceId = SourceDeviceId
        self.ncValue = ncValue


class SMLeScOobDataRequestIndication(object):

    def __init__(self, SourceDeviceId=bytearray(1)):
        '''
        @param SourceDeviceId: Device Id corresponding to the event
        '''
        self.SourceDeviceId = SourceDeviceId


class SMLocalLeScOobDataIndication(object):

    def __init__(self, SourceDeviceId=bytearray(1), LeScOobDataParams_=bytearray()):
        '''
        @param SourceDeviceId: Device Id corresponding to the event
        @param LeScOobDataParams_: LeScOobDataParams_
        '''
        self.SourceDeviceId = SourceDeviceId
        self.LeScOobDataParams_ = LeScOobDataParams_


class SMGenerateNewEcdhPkSkPairResIndication(object):

    def __init__(self, SourceDeviceId=bytearray(1), Status=SMGenerateNewEcdhPkSkPairResIndicationStatus.gBleSuccess_c):
        '''
        @param SourceDeviceId: Device Id corresponding to the event
        @param Status: Status of the signing operation
        '''
        self.SourceDeviceId = SourceDeviceId
        self.Status = Status


class ATTConfirm(object):

    def __init__(self, Status=ATTConfirmStatus.gBleSuccess_c):
        '''
        @param Status: The status of the ATT request
        '''
        self.Status = Status


class ATTGetMtuIndication(object):

    def __init__(self, DeviceId=bytearray(1), Mtu=bytearray(2)):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Mtu: MTU value
        '''
        self.DeviceId = DeviceId
        self.Mtu = Mtu


class ATTClientIncomingServerErrorResponseIndication(object):

    def __init__(self, DeviceId=bytearray(1), Params_RequestOpcodeInError=ATTClientIncomingServerErrorResponseIndicationParams_RequestOpcodeInError.gErrorResponse_c, Params_AttributeHandleInError=bytearray(2), Params_ErrorCode=ATTClientIncomingServerErrorResponseIndicationParams_ErrorCode.gInvalidHandle_c):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_RequestOpcodeInError: Request opcode in error
        @param Params_AttributeHandleInError: Attribute handle in error
        @param Params_ErrorCode: ATT error code
        '''
        self.DeviceId = DeviceId
        self.Params_RequestOpcodeInError = Params_RequestOpcodeInError
        self.Params_AttributeHandleInError = Params_AttributeHandleInError
        self.Params_ErrorCode = Params_ErrorCode


class ATTServerIncomingClientExchangeMtuRequestIndication(object):

    def __init__(self, DeviceId=bytearray(1), Params_ClientRxMtu=bytearray(2)):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_ClientRxMtu: Client received MTU
        '''
        self.DeviceId = DeviceId
        self.Params_ClientRxMtu = Params_ClientRxMtu


class ATTClientIncomingServerExchangeMtuResponseIndication(object):

    def __init__(self, DeviceId=bytearray(1), Params_ServerRxMtu=bytearray(2)):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_ServerRxMtu: Server received MTU
        '''
        self.DeviceId = DeviceId
        self.Params_ServerRxMtu = Params_ServerRxMtu


class ATTServerIncomingClientFindInformationRequestIndication(object):

    def __init__(self, DeviceId=bytearray(1), Params_StartingHandle=bytearray(2), Params_EndingHandle=bytearray(2)):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_StartingHandle: Starting handle
        @param Params_EndingHandle: Ending handle
        '''
        self.DeviceId = DeviceId
        self.Params_StartingHandle = Params_StartingHandle
        self.Params_EndingHandle = Params_EndingHandle


class ATTClientIncomingServerFindInformationResponseIndication(object):

    class Uuid16BitFormat(object):

        class HandleUuid16(object):

            def __init__(self, Handle=bytearray(2), Uuid16=bytearray(2)):
                self.Handle = Handle
                self.Uuid16 = Uuid16

        def __init__(self, HandleUuid16PairCount=bytearray(1), HandleUuid16=[]):
            self.HandleUuid16PairCount = HandleUuid16PairCount
            # Array length depends on HandleUuid16PairCount.
            self.HandleUuid16 = HandleUuid16

    class Uuid128BitFormat(object):

        class HandleUuid128(object):

            def __init__(self, Handle=bytearray(2), Uuid128=bytearray(16)):
                self.Handle = Handle
                # Unit length: 16 bytes
                self.Uuid128 = Uuid128

        def __init__(self, HandleUuid128PairCount=bytearray(1), HandleUuid128=[]):
            self.HandleUuid128PairCount = HandleUuid128PairCount
            # Array length depends on HandleUuid128PairCount.
            self.HandleUuid128 = HandleUuid128

    def __init__(self, DeviceId=bytearray(1), Params_Format=ATTClientIncomingServerFindInformationResponseIndicationParams_Format(), Params_InformationData=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_Format: ATT Uuid format
        @param Params_InformationData: Information data
        '''
        self.DeviceId = DeviceId
        self.Params_Format = Params_Format
        self.Params_InformationData = Params_InformationData


class ATTServerIncomingClientFindByTypeValueRequestIndication(object):

    def __init__(self, DeviceId=bytearray(1), Params_StartingHandle=bytearray(2), Params_EndingHandle=bytearray(2), Params_AttributeType=bytearray(2), Params_AttributeLength=bytearray(2), Params_AttributeValue=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_StartingHandle: Starting handle
        @param Params_EndingHandle: Ending handle
        @param Params_AttributeType: Attribute type
        @param Params_AttributeLength: Attribute length
        @param Params_AttributeValue: Attribute value
        '''
        self.DeviceId = DeviceId
        self.Params_StartingHandle = Params_StartingHandle
        self.Params_EndingHandle = Params_EndingHandle
        self.Params_AttributeType = Params_AttributeType
        self.Params_AttributeLength = Params_AttributeLength
        self.Params_AttributeValue = Params_AttributeValue


class ATTClientIncomingServerFindByTypeValueResponseIndication(object):

    class Params_HandleGroup(object):

        def __init__(self, StartingHandle=bytearray(2), EndingHandle=bytearray(2)):
            self.StartingHandle = StartingHandle
            self.EndingHandle = EndingHandle

    def __init__(self, DeviceId=bytearray(1), Params_GroupCount=bytearray(2), Params_HandleGroup=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_GroupCount: Number of groups
        @param Params_HandleGroup: ATT find information request parameters
        '''
        self.DeviceId = DeviceId
        self.Params_GroupCount = Params_GroupCount
        self.Params_HandleGroup = Params_HandleGroup


class ATTServerIncomingClientReadByTypeRequestIndication(object):

    def __init__(self, DeviceId=bytearray(1), Params_StartingHandle=bytearray(2), Params_EndingHandle=bytearray(2), Params_Format=ATTServerIncomingClientReadByTypeRequestIndicationParams_Format(), Params_AttributeType=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_StartingHandle: Starting handle
        @param Params_EndingHandle: Ending handle
        @param Params_Format: ATT Uuid format
        @param Params_AttributeType: Attribute type
        '''
        self.DeviceId = DeviceId
        self.Params_StartingHandle = Params_StartingHandle
        self.Params_EndingHandle = Params_EndingHandle
        self.Params_Format = Params_Format
        self.Params_AttributeType = Params_AttributeType


class ATTClientIncomingServerReadByTypeResponseIndication(object):

    def __init__(self, DeviceId=bytearray(1), Params_Length=bytearray(1), Params_AttributeDataListLength=bytearray(2), Params_AttributeDataList=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_Length: Fixed length of each handle+value pair in the AttributeDataList
        @param Params_AttributeDataListLength: Attribute data list length
        @param Params_AttributeDataList: Attribute data list
        '''
        self.DeviceId = DeviceId
        self.Params_Length = Params_Length
        self.Params_AttributeDataListLength = Params_AttributeDataListLength
        self.Params_AttributeDataList = Params_AttributeDataList


class ATTServerIncomingClientReadRequestIndication(object):

    def __init__(self, DeviceId=bytearray(1), Params_AttributeHandle=bytearray(2)):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_AttributeHandle: Attribute handle
        '''
        self.DeviceId = DeviceId
        self.Params_AttributeHandle = Params_AttributeHandle


class ATTClientIncomingServerReadResponseIndication(object):

    def __init__(self, DeviceId=bytearray(1), Params_AttributeLength=bytearray(2), Params_AttributeValue=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_AttributeLength: Attribute length
        @param Params_AttributeValue: Attribute value
        '''
        self.DeviceId = DeviceId
        self.Params_AttributeLength = Params_AttributeLength
        self.Params_AttributeValue = Params_AttributeValue


class ATTServerIncomingClientReadBlobRequestIndication(object):

    def __init__(self, DeviceId=bytearray(1), Params_AttributeHandle=bytearray(2), Params_ValueOffset=bytearray(2)):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_AttributeHandle: Attribute handle
        @param Params_ValueOffset: Value offset
        '''
        self.DeviceId = DeviceId
        self.Params_AttributeHandle = Params_AttributeHandle
        self.Params_ValueOffset = Params_ValueOffset


class ATTClientIncomingServerReadBlobResponseIndication(object):

    def __init__(self, DeviceId=bytearray(1), Params_AttributeLength=bytearray(2), Params_AttributeValue=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_AttributeLength: Attribute length
        @param Params_AttributeValue: Attribute value
        '''
        self.DeviceId = DeviceId
        self.Params_AttributeLength = Params_AttributeLength
        self.Params_AttributeValue = Params_AttributeValue


class ATTServerIncomingClientReadMultipleRequestIndication(object):

    class Params_ListOfHandles(object):

        def __init__(self, Handle=bytearray(2)):
            self.Handle = Handle

    def __init__(self, DeviceId=bytearray(1), Params_HandleCount=bytearray(2), Params_ListOfHandles=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_HandleCount: Number of handles
        @param Params_ListOfHandles: List of handles
        '''
        self.DeviceId = DeviceId
        self.Params_HandleCount = Params_HandleCount
        self.Params_ListOfHandles = Params_ListOfHandles


class ATTClientIncomingServerReadMultipleResponseIndication(object):

    def __init__(self, DeviceId=bytearray(1), Params_ListLength=bytearray(2), Params_ListOfValues=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_ListLength: List length
        @param Params_ListOfValues: List of values
        '''
        self.DeviceId = DeviceId
        self.Params_ListLength = Params_ListLength
        self.Params_ListOfValues = Params_ListOfValues


class ATTServerIncomingClientReadByGroupTypeRequestIndication(object):

    def __init__(self, DeviceId=bytearray(1), Params_StartingHandle=bytearray(2), Params_EndingHandle=bytearray(2), Params_Format=ATTServerIncomingClientReadByGroupTypeRequestIndicationParams_Format(), Params_AttributeType=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_StartingHandle: Starting handle
        @param Params_EndingHandle: Ending handle
        @param Params_Format: ATT Uuid format
        @param Params_AttributeType: Attribute type
        '''
        self.DeviceId = DeviceId
        self.Params_StartingHandle = Params_StartingHandle
        self.Params_EndingHandle = Params_EndingHandle
        self.Params_Format = Params_Format
        self.Params_AttributeType = Params_AttributeType


class ATTClientIncomingServerReadByGroupTypeResponseIndication(object):

    def __init__(self, DeviceId=bytearray(1), Params_Length=bytearray(1), Params_AttributeDataListLength=bytearray(2), Params_AttributeDataList=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_Length: Fixed length of each handle+value pair in the AttributeDataList
        @param Params_AttributeDataListLength: Attribute data list length
        @param Params_AttributeDataList: Attribute data list
        '''
        self.DeviceId = DeviceId
        self.Params_Length = Params_Length
        self.Params_AttributeDataListLength = Params_AttributeDataListLength
        self.Params_AttributeDataList = Params_AttributeDataList


class ATTServerIncomingClientWriteRequestIndication(object):

    def __init__(self, DeviceId=bytearray(1), Params_AttributeHandle=bytearray(2), Params_AttributeLength=bytearray(2), Params_AttributeValue=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_AttributeHandle: Attribute handle
        @param Params_AttributeLength: Attribute length
        @param Params_AttributeValue: Attribute value
        '''
        self.DeviceId = DeviceId
        self.Params_AttributeHandle = Params_AttributeHandle
        self.Params_AttributeLength = Params_AttributeLength
        self.Params_AttributeValue = Params_AttributeValue


class ATTClientIncomingServerWriteResponseIndication(object):

    def __init__(self, DeviceId=bytearray(1)):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        '''
        self.DeviceId = DeviceId


class ATTServerIncomingClientWriteCommandIndication(object):

    def __init__(self, DeviceId=bytearray(1), Params_AttributeHandle=bytearray(2), Params_AttributeLength=bytearray(2), Params_AttributeValue=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_AttributeHandle: Attribute handle
        @param Params_AttributeLength: Attribute length
        @param Params_AttributeValue: Attribute value
        '''
        self.DeviceId = DeviceId
        self.Params_AttributeHandle = Params_AttributeHandle
        self.Params_AttributeLength = Params_AttributeLength
        self.Params_AttributeValue = Params_AttributeValue


class ATTServerIncomingClientSignedWriteCommandIndication(object):

    def __init__(self, DeviceId=bytearray(1), Params_AttributeHandle=bytearray(2), Params_AttributeLength=bytearray(2), Params_AttributeValue=[], Params_AuthenticationSignature=bytearray(12)):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_AttributeHandle: Attribute handle
        @param Params_AttributeLength: Attribute length
        @param Params_AttributeValue: Attribute value
        @param Params_AuthenticationSignature: Authentication signature
        '''
        self.DeviceId = DeviceId
        self.Params_AttributeHandle = Params_AttributeHandle
        self.Params_AttributeLength = Params_AttributeLength
        self.Params_AttributeValue = Params_AttributeValue
        self.Params_AuthenticationSignature = Params_AuthenticationSignature


class ATTServerIncomingClientPrepareWriteRequestIndication(object):

    def __init__(self, DeviceId=bytearray(1), Params_AttributeHandle=bytearray(2), Params_ValueOffset=bytearray(2), Params_AttributeLength=bytearray(2), Params_AttributeValue=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_AttributeHandle: Attribute handle
        @param Params_ValueOffset: Value offset
        @param Params_AttributeLength: Attribute length
        @param Params_AttributeValue: Attribute value
        '''
        self.DeviceId = DeviceId
        self.Params_AttributeHandle = Params_AttributeHandle
        self.Params_ValueOffset = Params_ValueOffset
        self.Params_AttributeLength = Params_AttributeLength
        self.Params_AttributeValue = Params_AttributeValue


class ATTClientIncomingServerPrepareWriteResponseIndication(object):

    def __init__(self, DeviceId=bytearray(1), Params_AttributeHandle=bytearray(2), Params_ValueOffset=bytearray(2), Params_AttributeLength=bytearray(2), Params_AttributeValue=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_AttributeHandle: Attribute handle
        @param Params_ValueOffset: Value offset
        @param Params_AttributeLength: Attribute length
        @param Params_AttributeValue: Attribute value
        '''
        self.DeviceId = DeviceId
        self.Params_AttributeHandle = Params_AttributeHandle
        self.Params_ValueOffset = Params_ValueOffset
        self.Params_AttributeLength = Params_AttributeLength
        self.Params_AttributeValue = Params_AttributeValue


class ATTServerIncomingClientExecuteWriteRequestIndication(object):

    def __init__(self, DeviceId=bytearray(1), Params_Flags=ATTServerIncomingClientExecuteWriteRequestIndicationParams_Flags.gCancel_c):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_Flags: Flags
        '''
        self.DeviceId = DeviceId
        self.Params_Flags = Params_Flags


class ATTClientIncomingServerExecuteWriteResponseIndication(object):

    def __init__(self, DeviceId=bytearray(1)):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        '''
        self.DeviceId = DeviceId


class ATTClientIncomingServerHandleValueNotificationIndication(object):

    def __init__(self, DeviceId=bytearray(1), Params_AttributeHandle=bytearray(2), Params_AttributeLength=bytearray(2), Params_AttributeValue=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_AttributeHandle: Attribute handle
        @param Params_AttributeLength: Attribute length
        @param Params_AttributeValue: Attribute value
        '''
        self.DeviceId = DeviceId
        self.Params_AttributeHandle = Params_AttributeHandle
        self.Params_AttributeLength = Params_AttributeLength
        self.Params_AttributeValue = Params_AttributeValue


class ATTClientIncomingServerHandleValueIndicationIndication(object):

    def __init__(self, DeviceId=bytearray(1), Params_AttributeHandle=bytearray(2), Params_AttributeLength=bytearray(2), Params_AttributeValue=[]):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Params_AttributeHandle: Attribute handle
        @param Params_AttributeLength: Attribute length
        @param Params_AttributeValue: Attribute value
        '''
        self.DeviceId = DeviceId
        self.Params_AttributeHandle = Params_AttributeHandle
        self.Params_AttributeLength = Params_AttributeLength
        self.Params_AttributeValue = Params_AttributeValue


class ATTServerIncomingClientHandleValueConfirmationIndication(object):

    def __init__(self, DeviceId=bytearray(1)):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        '''
        self.DeviceId = DeviceId


class ATTUnsupportedOpcodeIndication(object):

    def __init__(self, DeviceId=bytearray(1), Opcode=bytearray(1)):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        @param Opcode: Unsupported opcode
        '''
        self.DeviceId = DeviceId
        self.Opcode = Opcode


class ATTTimeoutIndication(object):

    def __init__(self, DeviceId=bytearray(1)):
        '''
        @param DeviceId: The DeviceId for which the command is intended
        '''
        self.DeviceId = DeviceId


class GATTConfirm(object):

    def __init__(self, Status=GATTConfirmStatus.gBleSuccess_c):
        '''
        @param Status: The status of the GATT request
        '''
        self.Status = Status


class GATTGetMtuIndication(object):

    def __init__(self, Mtu=bytearray(2)):
        '''
        @param Mtu: MTU value
        '''
        self.Mtu = Mtu


class GATTClientProcedureExchangeMtuIndication(object):

    def __init__(self, DeviceId=bytearray(1), ProcedureResult=GATTClientProcedureExchangeMtuIndicationProcedureResult.gGattProcSuccess_c, Error=GATTClientProcedureExchangeMtuIndicationError.gBleSuccess_c):
        '''
        @param DeviceId: Device ID identifying the active connection
        @param ProcedureResult: The result of the completed procedure
        @param Error: If procedureResult is not gSuccess_c, this contains the error that terminated the procedure
        '''
        self.DeviceId = DeviceId
        self.ProcedureResult = ProcedureResult
        self.Error = Error


class GATTClientProcedureDiscoverAllPrimaryServicesIndication(object):

    class DiscoveredServices(object):

        class Characteristics(object):

            class Value(object):

                def __init__(self, Handle=bytearray(2), UuidType=ValueUuidType.Uuid16Bits, Uuid=[], ValueLength=bytearray(2), MaxValueLength=bytearray(2), Value=[]):
                    self.Handle = Handle
                    self.UuidType = UuidType
                    # Array length depends on UuidType.
                    self.Uuid = Uuid
                    self.ValueLength = ValueLength
                    self.MaxValueLength = MaxValueLength
                    # Array length depends on ValueLength.
                    self.Value = Value

            class Descriptors(object):

                def __init__(self, Handle=bytearray(2), UuidType=DescriptorsUuidType.Uuid16Bits, Uuid=[], ValueLength=bytearray(2), MaxValueLength=bytearray(2), Value=[]):
                    self.Handle = Handle
                    self.UuidType = UuidType
                    # Array length depends on UuidType.
                    self.Uuid = Uuid
                    self.ValueLength = ValueLength
                    self.MaxValueLength = MaxValueLength
                    # Array length depends on ValueLength.
                    self.Value = Value

            def __init__(self, Properties=CharacteristicsProperties.gNone_c, Value=Value(), NbOfDescriptors=bytearray(1), Descriptors=[]):
                self.Properties = Properties
                self.Value = Value
                self.NbOfDescriptors = NbOfDescriptors
                # Array length depends on NbOfDescriptors.
                self.Descriptors = Descriptors

        class IncludedServices(object):

            def __init__(self, StartHandle=bytearray(2), EndHandle=bytearray(2), UuidType=IncludedServicesUuidType.Uuid16Bits, Uuid=[], NbOfCharacteristics=bytearray(1), NbOfIncludedServices=bytearray(1)):
                self.StartHandle = StartHandle
                self.EndHandle = EndHandle
                self.UuidType = UuidType
                # Array length depends on UuidType.
                self.Uuid = Uuid
                self.NbOfCharacteristics = NbOfCharacteristics
                self.NbOfIncludedServices = NbOfIncludedServices

        def __init__(self, StartHandle=bytearray(2), EndHandle=bytearray(2), UuidType=DiscoveredServicesUuidType.Uuid16Bits, Uuid=[], NbOfCharacteristics=bytearray(1), Characteristics=[], NbOfIncludedServices=bytearray(1), IncludedServices=[]):
            self.StartHandle = StartHandle
            self.EndHandle = EndHandle
            self.UuidType = UuidType
            # Array length depends on UuidType.
            self.Uuid = Uuid
            self.NbOfCharacteristics = NbOfCharacteristics
            # Array length depends on NbOfCharacteristics.
            self.Characteristics = Characteristics
            self.NbOfIncludedServices = NbOfIncludedServices
            # Array length depends on NbOfIncludedServices.
            self.IncludedServices = IncludedServices

    def __init__(self, DeviceId=bytearray(1), ProcedureResult=GATTClientProcedureDiscoverAllPrimaryServicesIndicationProcedureResult.gGattProcSuccess_c, Error=GATTClientProcedureDiscoverAllPrimaryServicesIndicationError.gBleSuccess_c, NbOfDiscoveredServices=bytearray(1), DiscoveredServices=[]):
        '''
        @param DeviceId: Device ID identifying the active connection
        @param ProcedureResult: The result of the completed procedure
        @param Error: If procedureResult is not gSuccess_c, this contains the error that terminated the procedure
        @param NbOfDiscoveredServices: Number of discovered services
        @param DiscoveredServices: Discovered services
        '''
        self.DeviceId = DeviceId
        self.ProcedureResult = ProcedureResult
        self.Error = Error
        self.NbOfDiscoveredServices = NbOfDiscoveredServices
        self.DiscoveredServices = DiscoveredServices


class GATTClientProcedureDiscoverPrimaryServicesByUuidIndication(object):

    class DiscoveredServices(object):

        class Characteristics(object):

            class Value(object):

                def __init__(self, Handle=bytearray(2), UuidType=ValueUuidType.Uuid16Bits, Uuid=[], ValueLength=bytearray(2), MaxValueLength=bytearray(2), Value=[]):
                    self.Handle = Handle
                    self.UuidType = UuidType
                    # Array length depends on UuidType.
                    self.Uuid = Uuid
                    self.ValueLength = ValueLength
                    self.MaxValueLength = MaxValueLength
                    # Array length depends on ValueLength.
                    self.Value = Value

            class Descriptors(object):

                def __init__(self, Handle=bytearray(2), UuidType=DescriptorsUuidType.Uuid16Bits, Uuid=[], ValueLength=bytearray(2), MaxValueLength=bytearray(2), Value=[]):
                    self.Handle = Handle
                    self.UuidType = UuidType
                    # Array length depends on UuidType.
                    self.Uuid = Uuid
                    self.ValueLength = ValueLength
                    self.MaxValueLength = MaxValueLength
                    # Array length depends on ValueLength.
                    self.Value = Value

            def __init__(self, Properties=CharacteristicsProperties.gNone_c, Value=Value(), NbOfDescriptors=bytearray(1), Descriptors=[]):
                self.Properties = Properties
                self.Value = Value
                self.NbOfDescriptors = NbOfDescriptors
                # Array length depends on NbOfDescriptors.
                self.Descriptors = Descriptors

        class IncludedServices(object):

            def __init__(self, StartHandle=bytearray(2), EndHandle=bytearray(2), UuidType=IncludedServicesUuidType.Uuid16Bits, Uuid=[], NbOfCharacteristics=bytearray(1), NbOfIncludedServices=bytearray(1)):
                self.StartHandle = StartHandle
                self.EndHandle = EndHandle
                self.UuidType = UuidType
                # Array length depends on UuidType.
                self.Uuid = Uuid
                self.NbOfCharacteristics = NbOfCharacteristics
                self.NbOfIncludedServices = NbOfIncludedServices

        def __init__(self, StartHandle=bytearray(2), EndHandle=bytearray(2), UuidType=DiscoveredServicesUuidType.Uuid16Bits, Uuid=[], NbOfCharacteristics=bytearray(1), Characteristics=[], NbOfIncludedServices=bytearray(1), IncludedServices=[]):
            self.StartHandle = StartHandle
            self.EndHandle = EndHandle
            self.UuidType = UuidType
            # Array length depends on UuidType.
            self.Uuid = Uuid
            self.NbOfCharacteristics = NbOfCharacteristics
            # Array length depends on NbOfCharacteristics.
            self.Characteristics = Characteristics
            self.NbOfIncludedServices = NbOfIncludedServices
            # Array length depends on NbOfIncludedServices.
            self.IncludedServices = IncludedServices

    def __init__(self, DeviceId=bytearray(1), ProcedureResult=GATTClientProcedureDiscoverPrimaryServicesByUuidIndicationProcedureResult.gGattProcSuccess_c, Error=GATTClientProcedureDiscoverPrimaryServicesByUuidIndicationError.gBleSuccess_c, NbOfDiscoveredServices=bytearray(1), DiscoveredServices=[]):
        '''
        @param DeviceId: Device ID identifying the active connection
        @param ProcedureResult: The result of the completed procedure
        @param Error: If procedureResult is not gSuccess_c, this contains the error that terminated the procedure
        @param NbOfDiscoveredServices: Number of discovered services
        @param DiscoveredServices: Discovered services
        '''
        self.DeviceId = DeviceId
        self.ProcedureResult = ProcedureResult
        self.Error = Error
        self.NbOfDiscoveredServices = NbOfDiscoveredServices
        self.DiscoveredServices = DiscoveredServices


class GATTClientProcedureFindIncludedServicesIndication(object):

    class Service_Characteristics(object):

        class Value(object):

            def __init__(self, Handle=bytearray(2), UuidType=ValueUuidType.Uuid16Bits, Uuid=[], ValueLength=bytearray(2), MaxValueLength=bytearray(2), Value=[]):
                self.Handle = Handle
                self.UuidType = UuidType
                # Array length depends on UuidType.
                self.Uuid = Uuid
                self.ValueLength = ValueLength
                self.MaxValueLength = MaxValueLength
                # Array length depends on ValueLength.
                self.Value = Value

        class Descriptors(object):

            def __init__(self, Handle=bytearray(2), UuidType=DescriptorsUuidType.Uuid16Bits, Uuid=[], ValueLength=bytearray(2), MaxValueLength=bytearray(2), Value=[]):
                self.Handle = Handle
                self.UuidType = UuidType
                # Array length depends on UuidType.
                self.Uuid = Uuid
                self.ValueLength = ValueLength
                self.MaxValueLength = MaxValueLength
                # Array length depends on ValueLength.
                self.Value = Value

        def __init__(self, Properties=Service_CharacteristicsProperties.gNone_c, Value=Value(), NbOfDescriptors=bytearray(1), Descriptors=[]):
            self.Properties = Properties
            self.Value = Value
            self.NbOfDescriptors = NbOfDescriptors
            # Array length depends on NbOfDescriptors.
            self.Descriptors = Descriptors

    class Service_IncludedServices(object):

        def __init__(self, StartHandle=bytearray(2), EndHandle=bytearray(2), UuidType=Service_IncludedServicesUuidType.Uuid16Bits, Uuid=[], NbOfCharacteristics=bytearray(1), NbOfIncludedServices=bytearray(1)):
            self.StartHandle = StartHandle
            self.EndHandle = EndHandle
            self.UuidType = UuidType
            # Array length depends on UuidType.
            self.Uuid = Uuid
            self.NbOfCharacteristics = NbOfCharacteristics
            self.NbOfIncludedServices = NbOfIncludedServices

    def __init__(self, DeviceId=bytearray(1), ProcedureResult=GATTClientProcedureFindIncludedServicesIndicationProcedureResult.gGattProcSuccess_c, Error=GATTClientProcedureFindIncludedServicesIndicationError.gBleSuccess_c, Service_StartHandle=bytearray(2), Service_EndHandle=bytearray(2), Service_UuidType=GATTClientProcedureFindIncludedServicesIndicationService_UuidType.Uuid16Bits, Service_Uuid=[], Service_NbOfCharacteristics=bytearray(1), Service_Characteristics=[], Service_NbOfIncludedServices=bytearray(1), Service_IncludedServices=[]):
        '''
        @param DeviceId: Device ID identifying the active connection
        @param ProcedureResult: The result of the completed procedure
        @param Error: If procedureResult is not gSuccess_c, this contains the error that terminated the procedure
        @param Service_StartHandle: The handle of the Service Declaration attribute
        @param Service_EndHandle: The last handle belonging to this Service (followed by another Service declaration of the end of the database)
        @param Service_UuidType: UUID type
        @param Service_Uuid: UUID value
        @param Service_NbOfCharacteristics: Size of the Characteristic array
        @param Service_Characteristics: Characteristic array
        @param Service_NbOfIncludedServices: Size of the Included Services array
        @param Service_IncludedServices: Included Services array
        '''
        self.DeviceId = DeviceId
        self.ProcedureResult = ProcedureResult
        self.Error = Error
        self.Service_StartHandle = Service_StartHandle
        self.Service_EndHandle = Service_EndHandle
        self.Service_UuidType = Service_UuidType
        self.Service_Uuid = Service_Uuid
        self.Service_NbOfCharacteristics = Service_NbOfCharacteristics
        self.Service_Characteristics = Service_Characteristics
        self.Service_NbOfIncludedServices = Service_NbOfIncludedServices
        self.Service_IncludedServices = Service_IncludedServices


class GATTClientProcedureDiscoverAllCharacteristicsIndication(object):

    class Service_Characteristics(object):

        class Value(object):

            def __init__(self, Handle=bytearray(2), UuidType=ValueUuidType.Uuid16Bits, Uuid=[], ValueLength=bytearray(2), MaxValueLength=bytearray(2), Value=[]):
                self.Handle = Handle
                self.UuidType = UuidType
                # Array length depends on UuidType.
                self.Uuid = Uuid
                self.ValueLength = ValueLength
                self.MaxValueLength = MaxValueLength
                # Array length depends on ValueLength.
                self.Value = Value

        class Descriptors(object):

            def __init__(self, Handle=bytearray(2), UuidType=DescriptorsUuidType.Uuid16Bits, Uuid=[], ValueLength=bytearray(2), MaxValueLength=bytearray(2), Value=[]):
                self.Handle = Handle
                self.UuidType = UuidType
                # Array length depends on UuidType.
                self.Uuid = Uuid
                self.ValueLength = ValueLength
                self.MaxValueLength = MaxValueLength
                # Array length depends on ValueLength.
                self.Value = Value

        def __init__(self, Properties=Service_CharacteristicsProperties.gNone_c, Value=Value(), NbOfDescriptors=bytearray(1), Descriptors=[]):
            self.Properties = Properties
            self.Value = Value
            self.NbOfDescriptors = NbOfDescriptors
            # Array length depends on NbOfDescriptors.
            self.Descriptors = Descriptors

    class Service_IncludedServices(object):

        def __init__(self, StartHandle=bytearray(2), EndHandle=bytearray(2), UuidType=Service_IncludedServicesUuidType.Uuid16Bits, Uuid=[], NbOfCharacteristics=bytearray(1), NbOfIncludedServices=bytearray(1)):
            self.StartHandle = StartHandle
            self.EndHandle = EndHandle
            self.UuidType = UuidType
            # Array length depends on UuidType.
            self.Uuid = Uuid
            self.NbOfCharacteristics = NbOfCharacteristics
            self.NbOfIncludedServices = NbOfIncludedServices

    def __init__(self, DeviceId=bytearray(1), ProcedureResult=GATTClientProcedureDiscoverAllCharacteristicsIndicationProcedureResult.gGattProcSuccess_c, Error=GATTClientProcedureDiscoverAllCharacteristicsIndicationError.gBleSuccess_c, Service_StartHandle=bytearray(2), Service_EndHandle=bytearray(2), Service_UuidType=GATTClientProcedureDiscoverAllCharacteristicsIndicationService_UuidType.Uuid16Bits, Service_Uuid=[], Service_NbOfCharacteristics=bytearray(1), Service_Characteristics=[], Service_NbOfIncludedServices=bytearray(1), Service_IncludedServices=[]):
        '''
        @param DeviceId: Device ID identifying the active connection
        @param ProcedureResult: The result of the completed procedure
        @param Error: If procedureResult is not gSuccess_c, this contains the error that terminated the procedure
        @param Service_StartHandle: The handle of the Service Declaration attribute
        @param Service_EndHandle: The last handle belonging to this Service (followed by another Service declaration of the end of the database)
        @param Service_UuidType: UUID type
        @param Service_Uuid: UUID value
        @param Service_NbOfCharacteristics: Size of the Characteristic array
        @param Service_Characteristics: Characteristic array
        @param Service_NbOfIncludedServices: Size of the Included Services array
        @param Service_IncludedServices: Included Services array
        '''
        self.DeviceId = DeviceId
        self.ProcedureResult = ProcedureResult
        self.Error = Error
        self.Service_StartHandle = Service_StartHandle
        self.Service_EndHandle = Service_EndHandle
        self.Service_UuidType = Service_UuidType
        self.Service_Uuid = Service_Uuid
        self.Service_NbOfCharacteristics = Service_NbOfCharacteristics
        self.Service_Characteristics = Service_Characteristics
        self.Service_NbOfIncludedServices = Service_NbOfIncludedServices
        self.Service_IncludedServices = Service_IncludedServices


class GATTClientProcedureDiscoverCharacteristicByUuidIndication(object):

    class Characteristics(object):

        class Value(object):

            def __init__(self, Handle=bytearray(2), UuidType=ValueUuidType.Uuid16Bits, Uuid=[], ValueLength=bytearray(2), MaxValueLength=bytearray(2), Value=[]):
                self.Handle = Handle
                self.UuidType = UuidType
                # Array length depends on UuidType.
                self.Uuid = Uuid
                self.ValueLength = ValueLength
                self.MaxValueLength = MaxValueLength
                # Array length depends on ValueLength.
                self.Value = Value

        class Descriptors(object):

            def __init__(self, Handle=bytearray(2), UuidType=DescriptorsUuidType.Uuid16Bits, Uuid=[], ValueLength=bytearray(2), MaxValueLength=bytearray(2), Value=[]):
                self.Handle = Handle
                self.UuidType = UuidType
                # Array length depends on UuidType.
                self.Uuid = Uuid
                self.ValueLength = ValueLength
                self.MaxValueLength = MaxValueLength
                # Array length depends on ValueLength.
                self.Value = Value

        def __init__(self, Properties=CharacteristicsProperties.gNone_c, Value=Value(), NbOfDescriptors=bytearray(1), Descriptors=[]):
            self.Properties = Properties
            self.Value = Value
            self.NbOfDescriptors = NbOfDescriptors
            # Array length depends on NbOfDescriptors.
            self.Descriptors = Descriptors

    def __init__(self, DeviceId=bytearray(1), ProcedureResult=GATTClientProcedureDiscoverCharacteristicByUuidIndicationProcedureResult.gGattProcSuccess_c, Error=GATTClientProcedureDiscoverCharacteristicByUuidIndicationError.gBleSuccess_c, NbOfCharacteristics=bytearray(1), Characteristics=[]):
        '''
        @param DeviceId: Device ID identifying the active connection
        @param ProcedureResult: The result of the completed procedure
        @param Error: If procedureResult is not gSuccess_c, this contains the error that terminated the procedure
        @param NbOfCharacteristics: Number of discovered characteristics
        @param Characteristics: Characteristics that were filled
        '''
        self.DeviceId = DeviceId
        self.ProcedureResult = ProcedureResult
        self.Error = Error
        self.NbOfCharacteristics = NbOfCharacteristics
        self.Characteristics = Characteristics


class GATTClientProcedureDiscoverAllCharacteristicDescriptorsIndication(object):

    class Characteristic_Value(object):

        def __init__(self, Handle=bytearray(2), UuidType=Characteristic_ValueUuidType.Uuid16Bits, Uuid=[], ValueLength=bytearray(2), MaxValueLength=bytearray(2), Value=[]):
            self.Handle = Handle
            self.UuidType = UuidType
            # Array length depends on UuidType.
            self.Uuid = Uuid
            self.ValueLength = ValueLength
            self.MaxValueLength = MaxValueLength
            # Array length depends on ValueLength.
            self.Value = Value

    class Characteristic_Descriptors(object):

        def __init__(self, Handle=bytearray(2), UuidType=Characteristic_DescriptorsUuidType.Uuid16Bits, Uuid=[], ValueLength=bytearray(2), MaxValueLength=bytearray(2), Value=[]):
            self.Handle = Handle
            self.UuidType = UuidType
            # Array length depends on UuidType.
            self.Uuid = Uuid
            self.ValueLength = ValueLength
            self.MaxValueLength = MaxValueLength
            # Array length depends on ValueLength.
            self.Value = Value

    def __init__(self, DeviceId=bytearray(1), ProcedureResult=GATTClientProcedureDiscoverAllCharacteristicDescriptorsIndicationProcedureResult.gGattProcSuccess_c, Error=GATTClientProcedureDiscoverAllCharacteristicDescriptorsIndicationError.gBleSuccess_c, Characteristic_Properties=GATTClientProcedureDiscoverAllCharacteristicDescriptorsIndicationCharacteristic_Properties.gNone_c, Characteristic_Value=Characteristic_Value(), Characteristic_NbOfDescriptors=bytearray(1), Characteristic_Descriptors=[]):
        '''
        @param DeviceId: Device ID identifying the active connection
        @param ProcedureResult: The result of the completed procedure
        @param Error: If procedureResult is not gSuccess_c, this contains the error that terminated the procedure
        @param Characteristic_Properties: Characteristic Properties as defined by GATT
        @param Characteristic_Value: Characteristic Value attribute
        @param Characteristic_NbOfDescriptors: Size of the Characteristic Descriptors array
        @param Characteristic_Descriptors: Characteristic Descriptors array
        '''
        self.DeviceId = DeviceId
        self.ProcedureResult = ProcedureResult
        self.Error = Error
        self.Characteristic_Properties = Characteristic_Properties
        self.Characteristic_Value = Characteristic_Value
        self.Characteristic_NbOfDescriptors = Characteristic_NbOfDescriptors
        self.Characteristic_Descriptors = Characteristic_Descriptors


class GATTClientProcedureReadCharacteristicValueIndication(object):

    class Characteristic_Value(object):

        def __init__(self, Handle=bytearray(2), UuidType=Characteristic_ValueUuidType.Uuid16Bits, Uuid=[], ValueLength=bytearray(2), MaxValueLength=bytearray(2), Value=[]):
            self.Handle = Handle
            self.UuidType = UuidType
            # Array length depends on UuidType.
            self.Uuid = Uuid
            self.ValueLength = ValueLength
            self.MaxValueLength = MaxValueLength
            # Array length depends on ValueLength.
            self.Value = Value

    class Characteristic_Descriptors(object):

        def __init__(self, Handle=bytearray(2), UuidType=Characteristic_DescriptorsUuidType.Uuid16Bits, Uuid=[], ValueLength=bytearray(2), MaxValueLength=bytearray(2), Value=[]):
            self.Handle = Handle
            self.UuidType = UuidType
            # Array length depends on UuidType.
            self.Uuid = Uuid
            self.ValueLength = ValueLength
            self.MaxValueLength = MaxValueLength
            # Array length depends on ValueLength.
            self.Value = Value

    def __init__(self, DeviceId=bytearray(1), ProcedureResult=GATTClientProcedureReadCharacteristicValueIndicationProcedureResult.gGattProcSuccess_c, Error=GATTClientProcedureReadCharacteristicValueIndicationError.gBleSuccess_c, Characteristic_Properties=GATTClientProcedureReadCharacteristicValueIndicationCharacteristic_Properties.gNone_c, Characteristic_Value=Characteristic_Value(), Characteristic_NbOfDescriptors=bytearray(1), Characteristic_Descriptors=[]):
        '''
        @param DeviceId: Device ID identifying the active connection
        @param ProcedureResult: The result of the completed procedure
        @param Error: If procedureResult is not gSuccess_c, this contains the error that terminated the procedure
        @param Characteristic_Properties: Characteristic Properties as defined by GATT
        @param Characteristic_Value: Characteristic Value attribute
        @param Characteristic_NbOfDescriptors: Size of the Characteristic Descriptors array
        @param Characteristic_Descriptors: Characteristic Descriptors array
        '''
        self.DeviceId = DeviceId
        self.ProcedureResult = ProcedureResult
        self.Error = Error
        self.Characteristic_Properties = Characteristic_Properties
        self.Characteristic_Value = Characteristic_Value
        self.Characteristic_NbOfDescriptors = Characteristic_NbOfDescriptors
        self.Characteristic_Descriptors = Characteristic_Descriptors


class GATTClientProcedureReadUsingCharacteristicUuidIndication(object):

    def __init__(self, DeviceId=bytearray(1), ProcedureResult=GATTClientProcedureReadUsingCharacteristicUuidIndicationProcedureResult.gGattProcSuccess_c, Error=GATTClientProcedureReadUsingCharacteristicUuidIndicationError.gBleSuccess_c, NbOfReadBytes=bytearray(2), ReadBytes=[]):
        '''
        @param DeviceId: Device ID identifying the active connection
        @param ProcedureResult: The result of the completed procedure
        @param Error: If procedureResult is not gSuccess_c, this contains the error that terminated the procedure
        @param NbOfReadBytes: The number of bytes that were read
        @param ReadBytes: The bytes that were read
        '''
        self.DeviceId = DeviceId
        self.ProcedureResult = ProcedureResult
        self.Error = Error
        self.NbOfReadBytes = NbOfReadBytes
        self.ReadBytes = ReadBytes


class GATTClientProcedureReadMultipleCharacteristicValuesIndication(object):

    class Characteristics(object):

        class Value(object):

            def __init__(self, Handle=bytearray(2), UuidType=ValueUuidType.Uuid16Bits, Uuid=[], ValueLength=bytearray(2), MaxValueLength=bytearray(2), Value=[]):
                self.Handle = Handle
                self.UuidType = UuidType
                # Array length depends on UuidType.
                self.Uuid = Uuid
                self.ValueLength = ValueLength
                self.MaxValueLength = MaxValueLength
                # Array length depends on ValueLength.
                self.Value = Value

        class Descriptors(object):

            def __init__(self, Handle=bytearray(2), UuidType=DescriptorsUuidType.Uuid16Bits, Uuid=[], ValueLength=bytearray(2), MaxValueLength=bytearray(2), Value=[]):
                self.Handle = Handle
                self.UuidType = UuidType
                # Array length depends on UuidType.
                self.Uuid = Uuid
                self.ValueLength = ValueLength
                self.MaxValueLength = MaxValueLength
                # Array length depends on ValueLength.
                self.Value = Value

        def __init__(self, Properties=CharacteristicsProperties.gNone_c, Value=Value(), NbOfDescriptors=bytearray(1), Descriptors=[]):
            self.Properties = Properties
            self.Value = Value
            self.NbOfDescriptors = NbOfDescriptors
            # Array length depends on NbOfDescriptors.
            self.Descriptors = Descriptors

    def __init__(self, DeviceId=bytearray(1), ProcedureResult=GATTClientProcedureReadMultipleCharacteristicValuesIndicationProcedureResult.gGattProcSuccess_c, Error=GATTClientProcedureReadMultipleCharacteristicValuesIndicationError.gBleSuccess_c, NbOfCharacteristics=bytearray(1), Characteristics=[]):
        '''
        @param DeviceId: Device ID identifying the active connection
        @param ProcedureResult: The result of the completed procedure
        @param Error: If procedureResult is not gSuccess_c, this contains the error that terminated the procedure
        @param NbOfCharacteristics: Number of characteristics whose values were read
        @param Characteristics: Characteristics whose values were read
        '''
        self.DeviceId = DeviceId
        self.ProcedureResult = ProcedureResult
        self.Error = Error
        self.NbOfCharacteristics = NbOfCharacteristics
        self.Characteristics = Characteristics


class GATTClientProcedureWriteCharacteristicValueIndication(object):

    def __init__(self, DeviceId=bytearray(1), ProcedureResult=GATTClientProcedureWriteCharacteristicValueIndicationProcedureResult.gGattProcSuccess_c, Error=GATTClientProcedureWriteCharacteristicValueIndicationError.gBleSuccess_c):
        '''
        @param DeviceId: Device ID identifying the active connection
        @param ProcedureResult: The result of the completed procedure
        @param Error: If procedureResult is not gSuccess_c, this contains the error that terminated the procedure
        '''
        self.DeviceId = DeviceId
        self.ProcedureResult = ProcedureResult
        self.Error = Error


class GATTClientProcedureReadCharacteristicDescriptorIndication(object):

    def __init__(self, DeviceId=bytearray(1), ProcedureResult=GATTClientProcedureReadCharacteristicDescriptorIndicationProcedureResult.gGattProcSuccess_c, Error=GATTClientProcedureReadCharacteristicDescriptorIndicationError.gBleSuccess_c, Descriptor_Handle=bytearray(2), Descriptor_UuidType=GATTClientProcedureReadCharacteristicDescriptorIndicationDescriptor_UuidType.Uuid16Bits, Descriptor_Uuid=[], Descriptor_ValueLength=bytearray(2), Descriptor_MaxValueLength=bytearray(2), Descriptor_Value=[]):
        '''
        @param DeviceId: Device ID identifying the active connection
        @param ProcedureResult: The result of the completed procedure
        @param Error: If procedureResult is not gSuccess_c, this contains the error that terminated the procedure
        @param Descriptor_Handle: Attribute handle
        @param Descriptor_UuidType: UUID type
        @param Descriptor_Uuid: UUID value
        @param Descriptor_ValueLength: Length of the attribute value array
        @param Descriptor_MaxValueLength: Maximum length of the attribute value array; if this is set to 0, then the attribute's length is fixed and cannot be changed
        @param Descriptor_Value: Attribute value array
        '''
        self.DeviceId = DeviceId
        self.ProcedureResult = ProcedureResult
        self.Error = Error
        self.Descriptor_Handle = Descriptor_Handle
        self.Descriptor_UuidType = Descriptor_UuidType
        self.Descriptor_Uuid = Descriptor_Uuid
        self.Descriptor_ValueLength = Descriptor_ValueLength
        self.Descriptor_MaxValueLength = Descriptor_MaxValueLength
        self.Descriptor_Value = Descriptor_Value


class GATTClientProcedureWriteCharacteristicDescriptorIndication(object):

    def __init__(self, DeviceId=bytearray(1), ProcedureResult=GATTClientProcedureWriteCharacteristicDescriptorIndicationProcedureResult.gGattProcSuccess_c, Error=GATTClientProcedureWriteCharacteristicDescriptorIndicationError.gBleSuccess_c):
        '''
        @param DeviceId: Device ID identifying the active connection
        @param ProcedureResult: The result of the completed procedure
        @param Error: If procedureResult is not gSuccess_c, this contains the error that terminated the procedure
        '''
        self.DeviceId = DeviceId
        self.ProcedureResult = ProcedureResult
        self.Error = Error


class GATTClientNotificationIndication(object):

    def __init__(self, DeviceId=bytearray(1), CharacteristicValueHandle=bytearray(2), ValueLength=bytearray(2), Value=[]):
        '''
        @param DeviceId: Device ID identifying the active connection
        @param CharacteristicValueHandle: Handle of the Characteristic Value attribute to be notified
        @param ValueLength: Value array size
        @param Value: The Characteristic value array
        '''
        self.DeviceId = DeviceId
        self.CharacteristicValueHandle = CharacteristicValueHandle
        self.ValueLength = ValueLength
        self.Value = Value


class GATTClientIndicationIndication(object):

    def __init__(self, DeviceId=bytearray(1), CharacteristicValueHandle=bytearray(2), ValueLength=bytearray(2), Value=[]):
        '''
        @param DeviceId: Device ID identifying the active connection
        @param CharacteristicValueHandle: Handle of the Characteristic Value attribute to be indicated
        @param ValueLength: Value array size
        @param Value: The Characteristic value array
        '''
        self.DeviceId = DeviceId
        self.CharacteristicValueHandle = CharacteristicValueHandle
        self.ValueLength = ValueLength
        self.Value = Value


class GATTServerMtuChangedIndication(object):

    def __init__(self, DeviceId=bytearray(1), MtuChangedEvent_NewMtu=bytearray(2)):
        '''
        @param DeviceId: Device ID identifying the active connection
        @param MtuChangedEvent_NewMtu: Value of the agreed ATT_MTU for this connection
        '''
        self.DeviceId = DeviceId
        self.MtuChangedEvent_NewMtu = MtuChangedEvent_NewMtu


class GATTServerHandleValueConfirmationIndication(object):

    def __init__(self, DeviceId=bytearray(1)):
        '''
        @param DeviceId: Device ID identifying the active connection
        '''
        self.DeviceId = DeviceId


class GATTServerAttributeWrittenIndication(object):

    def __init__(self, DeviceId=bytearray(1), AttributeWrittenEvent_Handle=bytearray(2), AttributeWrittenEvent_ValueLength=bytearray(2), AttributeWrittenEvent_Value=[]):
        '''
        @param DeviceId: Device ID identifying the active connection
        @param AttributeWrittenEvent_Handle: Handle of the attribute
        @param AttributeWrittenEvent_ValueLength: Length of the attribute value array
        @param AttributeWrittenEvent_Value: Attribute value array attempted to be written
        '''
        self.DeviceId = DeviceId
        self.AttributeWrittenEvent_Handle = AttributeWrittenEvent_Handle
        self.AttributeWrittenEvent_ValueLength = AttributeWrittenEvent_ValueLength
        self.AttributeWrittenEvent_Value = AttributeWrittenEvent_Value


class GATTServerCharacteristicCccdWrittenIndication(object):

    def __init__(self, DeviceId=bytearray(1), CharCccdWrittenEvent_Handle=bytearray(2), CharCccdWrittenEvent_NewCccd=GATTServerCharacteristicCccdWrittenIndicationCharCccdWrittenEvent_NewCccd.gCccdEmpty_c):
        '''
        @param DeviceId: Device ID identifying the active connection
        @param CharCccdWrittenEvent_Handle: Handle of the CCCD attribute
        @param CharCccdWrittenEvent_NewCccd: New value of the CCCD
        '''
        self.DeviceId = DeviceId
        self.CharCccdWrittenEvent_Handle = CharCccdWrittenEvent_Handle
        self.CharCccdWrittenEvent_NewCccd = CharCccdWrittenEvent_NewCccd


class GATTServerAttributeWrittenWithoutResponseIndication(object):

    def __init__(self, DeviceId=bytearray(1), AttributeWrittenEvent_Handle=bytearray(2), AttributeWrittenEvent_ValueLength=bytearray(2), AttributeWrittenEvent_Value=[]):
        '''
        @param DeviceId: Device ID identifying the active connection
        @param AttributeWrittenEvent_Handle: Handle of the attribute
        @param AttributeWrittenEvent_ValueLength: Length of the attribute value array
        @param AttributeWrittenEvent_Value: Attribute value array attempted to be written
        '''
        self.DeviceId = DeviceId
        self.AttributeWrittenEvent_Handle = AttributeWrittenEvent_Handle
        self.AttributeWrittenEvent_ValueLength = AttributeWrittenEvent_ValueLength
        self.AttributeWrittenEvent_Value = AttributeWrittenEvent_Value


class GATTServerErrorIndication(object):

    def __init__(self, DeviceId=bytearray(1), ProcedureError_ProcedureType=GATTServerErrorIndicationProcedureError_ProcedureType.gSendAttributeWrittenStatus_c, ProcedureError_Error=GATTServerErrorIndicationProcedureError_Error.gBleSuccess_c):
        '''
        @param DeviceId: Device ID identifying the active connection
        @param ProcedureError_ProcedureType: Procedure that generated error
        @param ProcedureError_Error: Error generated
        '''
        self.DeviceId = DeviceId
        self.ProcedureError_ProcedureType = ProcedureError_ProcedureType
        self.ProcedureError_Error = ProcedureError_Error


class GATTServerLongCharacteristicWrittenIndication(object):

    def __init__(self, DeviceId=bytearray(1), LongCharacteristicWrittenEvent_Handle=bytearray(2), LongCharacteristicWrittenEvent_ValueLength=bytearray(2), LongCharacteristicWrittenEvent_Value=[]):
        '''
        @param DeviceId: Device ID identifying the active connection
        @param LongCharacteristicWrittenEvent_Handle: Handle of the characteristic value
        @param LongCharacteristicWrittenEvent_ValueLength: Length of the characteristic value
        @param LongCharacteristicWrittenEvent_Value: Characteristic value array
        '''
        self.DeviceId = DeviceId
        self.LongCharacteristicWrittenEvent_Handle = LongCharacteristicWrittenEvent_Handle
        self.LongCharacteristicWrittenEvent_ValueLength = LongCharacteristicWrittenEvent_ValueLength
        self.LongCharacteristicWrittenEvent_Value = LongCharacteristicWrittenEvent_Value


class GATTServerAttributeReadIndication(object):

    def __init__(self, DeviceId=bytearray(1), AttributeReadEvent_Handle=bytearray(2)):
        '''
        @param DeviceId: Device ID identifying the active connection
        @param AttributeReadEvent_Handle: Handle of the attribute
        '''
        self.DeviceId = DeviceId
        self.AttributeReadEvent_Handle = AttributeReadEvent_Handle


class GATTDBConfirm(object):

    def __init__(self, Status=GATTDBConfirmStatus.gBleSuccess_c):
        '''
        @param Status: The status of the GATT Database request
        '''
        self.Status = Status


class GATTDBReadAttributeIndication(object):

    def __init__(self, ValueLength=bytearray(2), Value=[]):
        '''
        @param ValueLength: Attribute value length
        @param Value: Attribute value
        '''
        self.ValueLength = ValueLength
        self.Value = Value


class GATTDBFindServiceHandleIndication(object):

    def __init__(self, ServiceHandle=bytearray(2)):
        '''
        @param ServiceHandle: Service handle
        '''
        self.ServiceHandle = ServiceHandle


class GATTDBFindCharValueHandleInServiceIndication(object):

    def __init__(self, CharValueHandle=bytearray(2)):
        '''
        @param CharValueHandle: Characteristic value handle
        '''
        self.CharValueHandle = CharValueHandle


class GATTDBFindCccdHandleForCharValueHandleIndication(object):

    def __init__(self, CccdHandle=bytearray(2)):
        '''
        @param CccdHandle: CCCD handle
        '''
        self.CccdHandle = CccdHandle


class GATTDBFindDescriptorHandleForCharValueHandleIndication(object):

    def __init__(self, DescriptorHandle=bytearray(2)):
        '''
        @param DescriptorHandle: Descriptor handle
        '''
        self.DescriptorHandle = DescriptorHandle


class GATTDBDynamicAddPrimaryServiceDeclarationIndication(object):

    def __init__(self, ServiceHandle=bytearray(2)):
        '''
        @param ServiceHandle: Handle of the Service declaration
        '''
        self.ServiceHandle = ServiceHandle


class GATTDBDynamicAddSecondaryServiceDeclarationIndication(object):

    def __init__(self, ServiceHandle=bytearray(2)):
        '''
        @param ServiceHandle: Handle of the Service declaration
        '''
        self.ServiceHandle = ServiceHandle


class GATTDBDynamicAddIncludeDeclarationIndication(object):

    def __init__(self, IncludeHandle=bytearray(2)):
        '''
        @param IncludeHandle: Handle of the Include declaration
        '''
        self.IncludeHandle = IncludeHandle


class GATTDBDynamicAddCharacteristicDeclarationAndValueIndication(object):

    def __init__(self, CharacteristicHandle=bytearray(2)):
        '''
        @param CharacteristicHandle: Handle of the Characteristic declaration
        '''
        self.CharacteristicHandle = CharacteristicHandle


class GATTDBDynamicAddCharacteristicDescriptorIndication(object):

    def __init__(self, DescriptorHandle=bytearray(2)):
        '''
        @param DescriptorHandle: Handle of the Descriptor declaration
        '''
        self.DescriptorHandle = DescriptorHandle


class GATTDBDynamicAddCccdIndication(object):

    def __init__(self, CCCDHandle=bytearray(2)):
        '''
        @param CCCDHandle: Handle of the CCCD declaration
        '''
        self.CCCDHandle = CCCDHandle


class GATTDBDynamicAddCharacteristicDeclarationWithUniqueValueIndication(object):

    def __init__(self, CharacteristicHandle=bytearray(2)):
        '''
        @param CharacteristicHandle: Handle of the Characteristic declaration
        '''
        self.CharacteristicHandle = CharacteristicHandle


class GATTDBAttConfirm(object):

    def __init__(self, Status=GATTDBAttConfirmStatus.gBleSuccess_c):
        '''
        @param Status: The status of the GATT Database request
        '''
        self.Status = Status


class GATTDBAttFindInformationIndication(object):

    class Uuid16BitFormat(object):

        class HandleUuid16(object):

            def __init__(self, Handle=bytearray(2), Uuid16=bytearray(2)):
                self.Handle = Handle
                self.Uuid16 = Uuid16

        def __init__(self, HandleUuid16PairCount=bytearray(1), HandleUuid16=[]):
            self.HandleUuid16PairCount = HandleUuid16PairCount
            # Array length depends on HandleUuid16PairCount.
            self.HandleUuid16 = HandleUuid16

    class Uuid128BitFormat(object):

        class HandleUuid128(object):

            def __init__(self, Handle=bytearray(2), Uuid128=bytearray(16)):
                self.Handle = Handle
                # Unit length: 16 bytes
                self.Uuid128 = Uuid128

        def __init__(self, HandleUuid128PairCount=bytearray(1), HandleUuid128=[]):
            self.HandleUuid128PairCount = HandleUuid128PairCount
            # Array length depends on HandleUuid128PairCount.
            self.HandleUuid128 = HandleUuid128

    def __init__(self, Params_Format=GATTDBAttFindInformationIndicationParams_Format(), Params_InformationData=[], ErrorAttributeHandle=bytearray(2)):
        '''
        @param Params_Format: ATT Uuid format
        @param Params_InformationData: Information data
        @param ErrorAttributeHandle: The attribute handle where an error occurred
        '''
        self.Params_Format = Params_Format
        self.Params_InformationData = Params_InformationData
        self.ErrorAttributeHandle = ErrorAttributeHandle


class GATTDBAttFindByTypeValueIndication(object):

    class Params_HandleGroup(object):

        def __init__(self, StartingHandle=bytearray(2), EndingHandle=bytearray(2)):
            self.StartingHandle = StartingHandle
            self.EndingHandle = EndingHandle

    def __init__(self, Params_GroupCount=bytearray(2), Params_HandleGroup=[], ErrorAttributeHandle=bytearray(2)):
        '''
        @param Params_GroupCount: Number of groups
        @param Params_HandleGroup: ATT find information request parameters
        @param ErrorAttributeHandle: The attribute handle where an error occurred
        '''
        self.Params_GroupCount = Params_GroupCount
        self.Params_HandleGroup = Params_HandleGroup
        self.ErrorAttributeHandle = ErrorAttributeHandle


class GATTDBAttReadByTypeIndication(object):

    def __init__(self, Params_Length=bytearray(1), Params_AttributeDataListLength=bytearray(2), Params_AttributeDataList=[], ErrorAttributeHandle=bytearray(2)):
        '''
        @param Params_Length: Fixed length of each handle+value pair in the AttributeDataList
        @param Params_AttributeDataListLength: Attribute data list length
        @param Params_AttributeDataList: Attribute data list
        @param ErrorAttributeHandle: The attribute handle where an error occurred
        '''
        self.Params_Length = Params_Length
        self.Params_AttributeDataListLength = Params_AttributeDataListLength
        self.Params_AttributeDataList = Params_AttributeDataList
        self.ErrorAttributeHandle = ErrorAttributeHandle


class GATTDBAttReadIndication(object):

    def __init__(self, Params_AttributeLength=bytearray(2), Params_AttributeValue=[], ErrorAttributeHandle=bytearray(2)):
        '''
        @param Params_AttributeLength: Attribute length
        @param Params_AttributeValue: Attribute value
        @param ErrorAttributeHandle: The attribute handle where an error occurred
        '''
        self.Params_AttributeLength = Params_AttributeLength
        self.Params_AttributeValue = Params_AttributeValue
        self.ErrorAttributeHandle = ErrorAttributeHandle


class GATTDBAttReadBlobIndication(object):

    def __init__(self, ErrorAttributeHandle=bytearray(2)):
        '''
        @param ErrorAttributeHandle: The attribute handle where an error occurred
        '''
        self.ErrorAttributeHandle = ErrorAttributeHandle


class GATTDBAttReadMultipleIndication(object):

    def __init__(self, Params_ListLength=bytearray(2), Params_ListOfValues=[], ErrorAttributeHandle=bytearray(2)):
        '''
        @param Params_ListLength: List length
        @param Params_ListOfValues: List of values
        @param ErrorAttributeHandle: The attribute handle where an error occurred
        '''
        self.Params_ListLength = Params_ListLength
        self.Params_ListOfValues = Params_ListOfValues
        self.ErrorAttributeHandle = ErrorAttributeHandle


class GATTDBAttReadByGroupTypeIndication(object):

    def __init__(self, Params_Length=bytearray(1), Params_AttributeDataListLength=bytearray(2), Params_AttributeDataList=[], ErrorAttributeHandle=bytearray(2)):
        '''
        @param Params_Length: Fixed length of each handle+value pair in the AttributeDataList
        @param Params_AttributeDataListLength: Attribute data list length
        @param Params_AttributeDataList: Attribute data list
        @param ErrorAttributeHandle: The attribute handle where an error occurred
        '''
        self.Params_Length = Params_Length
        self.Params_AttributeDataListLength = Params_AttributeDataListLength
        self.Params_AttributeDataList = Params_AttributeDataList
        self.ErrorAttributeHandle = ErrorAttributeHandle


class GATTDBAttWriteIndication(object):

    def __init__(self, ErrorAttributeHandle=bytearray(2)):
        '''
        @param ErrorAttributeHandle: The attribute handle where an error occurred
        '''
        self.ErrorAttributeHandle = ErrorAttributeHandle


class GATTDBAttPrepareWriteIndication(object):

    def __init__(self, Params_AttributeHandle=bytearray(2), Params_ValueOffset=bytearray(2), Params_AttributeLength=bytearray(2), Params_AttributeValue=[], ErrorAttributeHandle=bytearray(2)):
        '''
        @param Params_AttributeHandle: Attribute handle
        @param Params_ValueOffset: Value offset
        @param Params_AttributeLength: Attribute length
        @param Params_AttributeValue: Attribute value
        @param ErrorAttributeHandle: The attribute handle where an error occurred
        '''
        self.Params_AttributeHandle = Params_AttributeHandle
        self.Params_ValueOffset = Params_ValueOffset
        self.Params_AttributeLength = Params_AttributeLength
        self.Params_AttributeValue = Params_AttributeValue
        self.ErrorAttributeHandle = ErrorAttributeHandle


class GATTDBAttExecuteWriteIndication(object):

    def __init__(self, ErrorAttributeHandle=bytearray(2)):
        '''
        @param ErrorAttributeHandle: The attribute handle where an error occurred
        '''
        self.ErrorAttributeHandle = ErrorAttributeHandle


class GATTDBAttExecuteWriteFromQueueIndication(object):

    def __init__(self, ErrorAttributeHandle=bytearray(2)):
        '''
        @param ErrorAttributeHandle: The attribute handle where an error occurred
        '''
        self.ErrorAttributeHandle = ErrorAttributeHandle


class GATTDBAttPrepareNotificationIndicationIndication(object):

    def __init__(self, ErrorAttributeHandle=bytearray(2)):
        '''
        @param ErrorAttributeHandle: The attribute handle where an error occurred
        '''
        self.ErrorAttributeHandle = ErrorAttributeHandle


class GAPConfirm(object):

    def __init__(self, Status=GAPConfirmStatus.gBleSuccess_c):
        '''
        @param Status: The status of the GAP request
        '''
        self.Status = Status


class GAPCheckNotificationStatusIndication(object):

    def __init__(self, IsActive=bytearray(1)):
        '''
        @param IsActive: Notification status
        '''
        self.IsActive = IsActive


class GAPCheckIndicationStatusIndication(object):

    def __init__(self, IsActive=bytearray(1)):
        '''
        @param IsActive: Indication status
        '''
        self.IsActive = IsActive


class GAPGetBondedStaticAddressesIndication(object):

    def __init__(self, NbOfDeviceAddresses=bytearray(1), DeviceAddresses=[]):
        '''
        @param NbOfDeviceAddresses: The number of device addresses
        @param DeviceAddresses: Device addresses list
        '''
        self.NbOfDeviceAddresses = NbOfDeviceAddresses
        self.DeviceAddresses = DeviceAddresses


class GAPLoadEncryptionInformationIndication(object):

    def __init__(self, LtkSize=bytearray(1), Ltk=[]):
        '''
        @param LtkSize: LTK size
        @param Ltk: LTK value
        '''
        self.LtkSize = LtkSize
        self.Ltk = Ltk


class GAPLoadCustomPeerInformationIndication(object):

    def __init__(self, InfoSize=bytearray(2), Info=[]):
        '''
        @param InfoSize: Data size
        @param Info: Data value
        '''
        self.InfoSize = InfoSize
        self.Info = Info


class GAPCheckIfBondedIndication(object):

    def __init__(self, IsBonded=bytearray(1)):
        '''
        @param IsBonded: Bonded flag
        '''
        self.IsBonded = IsBonded


class GAPGetBondedDevicesCountIndication(object):

    def __init__(self, NbOfBondedDevices=bytearray(1)):
        '''
        @param NbOfBondedDevices: Number of bonded devices
        '''
        self.NbOfBondedDevices = NbOfBondedDevices


class GAPGetBondedDeviceNameIndication(object):

    def __init__(self, NameSize=bytearray(1), Name=[]):
        '''
        @param NameSize: The name size
        @param Name: The name value
        '''
        self.NameSize = NameSize
        self.Name = Name


class GAPGenericEventInitializationCompleteIndication(object):

    pass


class GAPGenericEventInternalErrorIndication(object):

    def __init__(self, InternalError_ErrorCode=GAPGenericEventInternalErrorIndicationInternalError_ErrorCode.gBleSuccess_c, InternalError_ErrorSource=GAPGenericEventInternalErrorIndicationInternalError_ErrorSource.gHciCommandStatus_c, InternalError_HciCommandOpcode=bytearray(2)):
        '''
        @param InternalError_ErrorCode: Error code
        @param InternalError_ErrorSource: Error source
        @param InternalError_HciCommandOpcode: HCI command operation code
        '''
        self.InternalError_ErrorCode = InternalError_ErrorCode
        self.InternalError_ErrorSource = InternalError_ErrorSource
        self.InternalError_HciCommandOpcode = InternalError_HciCommandOpcode


class GAPGenericEventAdvertisingSetupFailedIndication(object):

    def __init__(self, SetupFailReason=GAPGenericEventAdvertisingSetupFailedIndicationSetupFailReason.gBleSuccess_c):
        '''
        @param SetupFailReason: Setup fail reason
        '''
        self.SetupFailReason = SetupFailReason


class GAPGenericEventAdvertisingParametersSetupCompleteIndication(object):

    pass


class GAPGenericEventAdvertisingDataSetupCompleteIndication(object):

    pass


class GAPGenericEventWhiteListSizeReadIndication(object):

    def __init__(self, WhiteListSize=bytearray(1)):
        '''
        @param WhiteListSize: White list size
        '''
        self.WhiteListSize = WhiteListSize


class GAPGenericEventDeviceAddedToWhiteListIndication(object):

    pass


class GAPGenericEventDeviceRemovedFromWhiteListIndication(object):

    pass


class GAPGenericEventWhiteListClearedIndication(object):

    pass


class GAPGenericEventRandomAddressReadyIndication(object):

    def __init__(self, Address=bytearray(6)):
        '''
        @param Address: Random address
        '''
        self.Address = Address


class GAPGenericEventCreateConnectionCanceledIndication(object):

    pass


class GAPGenericEventPublicAddressReadIndication(object):

    def __init__(self, Address=bytearray(6)):
        '''
        @param Address: Public address
        '''
        self.Address = Address


class GAPGenericEventAdvTxPowerLevelReadIndication(object):

    def __init__(self, AdvTxPowerLevel=bytearray(1)):
        '''
        @param AdvTxPowerLevel: Advertising transmission power level (in dBm)
        '''
        self.AdvTxPowerLevel = AdvTxPowerLevel


class GAPGenericEventPrivateResolvableAddressVerifiedIndication(object):

    def __init__(self, Verified=bytearray(1)):
        '''
        @param Verified: Private Resolvable Address verified
        '''
        self.Verified = Verified


class GAPGenericEventRandomAddressSetIndication(object):

    pass


class GAPAdvertisingEventStateChangedIndication(object):

    pass


class GAPAdvertisingEventCommandFailedIndication(object):

    def __init__(self, FailReason=GAPAdvertisingEventCommandFailedIndicationFailReason.gBleSuccess_c):
        '''
        @param FailReason: The reason of failure
        '''
        self.FailReason = FailReason


class GAPScanningEventStateChangedIndication(object):

    pass


class GAPScanningEventCommandFailedIndication(object):

    def __init__(self, FailReason=GAPScanningEventCommandFailedIndicationFailReason.gBleSuccess_c):
        '''
        @param FailReason: The reason of failure
        '''
        self.FailReason = FailReason


class GAPScanningEventDeviceScannedIndication(object):

    def __init__(self, ScannedDevice_AddressType=GAPScanningEventDeviceScannedIndicationScannedDevice_AddressType.gPublic_c, ScannedDevice_Address=bytearray(6), ScannedDevice_Rssi=bytearray(1), ScannedDevice_DataLength=bytearray(1), ScannedDevice_Data=[], ScannedDevice_AdvEventType=GAPScanningEventDeviceScannedIndicationScannedDevice_AdvEventType.gBleAdvRepAdvInd_c, ScannedDevice_DirectRpaUsed=bytearray(1), ScannedDevice_DirectRpa=[], ScannedDevice_advertisingAddressResolved=bytearray(1)):
        '''
        @param ScannedDevice_AddressType: Device's advertising address type
        @param ScannedDevice_Address: Device's advertising address
        @param ScannedDevice_Rssi: RSSI on the advertising channel; may be compared to the TX power contained in the AD Structure of type gAdTxPowerLevel_c to estimate distance from the advertiser
        @param ScannedDevice_DataLength: Length of the advertising or scan response data
        @param ScannedDevice_Data: Advertising or scan response data
        @param ScannedDevice_AdvEventType: Advertising report type, indicating what type of event generated this data (advertising, scan response)
        @param ScannedDevice_DirectRpaUsed: TRUE if directed advertising with Resolvable Private Address as Direct Address was detected while Enhanced Privacy is enabled
        @param ScannedDevice_DirectRpa: Resolvable Private Address set as Direct Address for directed advertising
        @param ScannedDevice_advertisingAddressResolved: TRUE if the address contained in the addressType and aAddress fields is the identity address of a resolved RPA from the Advertising Address field
        '''
        self.ScannedDevice_AddressType = ScannedDevice_AddressType
        self.ScannedDevice_Address = ScannedDevice_Address
        self.ScannedDevice_Rssi = ScannedDevice_Rssi
        self.ScannedDevice_DataLength = ScannedDevice_DataLength
        self.ScannedDevice_Data = ScannedDevice_Data
        self.ScannedDevice_AdvEventType = ScannedDevice_AdvEventType
        self.ScannedDevice_DirectRpaUsed = ScannedDevice_DirectRpaUsed
        self.ScannedDevice_DirectRpa = ScannedDevice_DirectRpa
        self.ScannedDevice_advertisingAddressResolved = ScannedDevice_advertisingAddressResolved


class GAPConnectionEventConnectedIndication(object):

    def __init__(self, DeviceId=bytearray(1), ConnectionParameters_ConnInterval=bytearray(2), ConnectionParameters_ConnLatency=bytearray(2), ConnectionParameters_SupervisionTimeout=bytearray(2), ConnectionParameters_MasterClockAccuracy=GAPConnectionEventConnectedIndicationConnectionParameters_MasterClockAccuracy.gMasterClkAcc500ppm_c, PeerAddressType=GAPConnectionEventConnectedIndicationPeerAddressType.gPublic_c, PeerAddress=bytearray(6), peerRpaResolved=bytearray(1), peerRpa=[], localRpaUsed=bytearray(1), localRpa=[]):
        '''
        @param DeviceId: Device ID identifying the connection
        @param ConnectionParameters_ConnInterval: Interval between connection events
        @param ConnectionParameters_ConnLatency: Number of consecutive connection events the Slave may ignore
        @param ConnectionParameters_SupervisionTimeout: The maximum time interval between consecutive over-the-air packets; if this timer expires, the connection is dropped
        @param ConnectionParameters_MasterClockAccuracy: Accuracy of master's clock, allowing for frame detection optimizations
        @param PeerAddressType: Connected device's address type
        @param PeerAddress: Connected device's address
        @param peerRpaResolved: TRUE if the address defined by peerAddressType and peerAddress is the identity address of the peer, and the peer used an RPA that was resolved by the Controller and is contained in the peerRpa field
        @param peerRpa: Peer Resolvable Private Address if Controller Privacy is active and peerRpaResolved is TRUE
        @param localRpaUsed: TRUE if the Controller has used an RPA contained in the localRpa field
        @param localRpa: Local Resolvable Private Address if Controller Privacy is active and localRpaUsed is TRUE
        '''
        self.DeviceId = DeviceId
        self.ConnectionParameters_ConnInterval = ConnectionParameters_ConnInterval
        self.ConnectionParameters_ConnLatency = ConnectionParameters_ConnLatency
        self.ConnectionParameters_SupervisionTimeout = ConnectionParameters_SupervisionTimeout
        self.ConnectionParameters_MasterClockAccuracy = ConnectionParameters_MasterClockAccuracy
        self.PeerAddressType = PeerAddressType
        self.PeerAddress = PeerAddress
        self.peerRpaResolved = peerRpaResolved
        self.peerRpa = peerRpa
        self.localRpaUsed = localRpaUsed
        self.localRpa = localRpa


class GAPConnectionEventPairingRequestIndication(object):

    def __init__(self, DeviceId=bytearray(1), PairingParameters_WithBonding=bytearray(1), PairingParameters_SecurityModeAndLevel=GAPConnectionEventPairingRequestIndicationPairingParameters_SecurityModeAndLevel.gMode1Level1_c, PairingParameters_MaxEncryptionKeySize=bytearray(1), PairingParameters_LocalIoCapabilities=GAPConnectionEventPairingRequestIndicationPairingParameters_LocalIoCapabilities.gIoDisplayOnly_c, PairingParameters_OobAvailable=bytearray(1), PairingParameters_CentralKeys=GAPConnectionEventPairingRequestIndicationPairingParameters_CentralKeys.gNoKeys_c, PairingParameters_PeripheralKeys=GAPConnectionEventPairingRequestIndicationPairingParameters_PeripheralKeys.gNoKeys_c, PairingParameters_LeSecureConnectionSupported=bytearray(1), PairingParameters_UseKeypressNotifications=bytearray(1)):
        '''
        @param DeviceId: Device ID identifying the connection
        @param PairingParameters_WithBonding: TRUE if this device is able to and wants to bond after pairing, FALSE otherwise
        @param PairingParameters_SecurityModeAndLevel: The desired security mode-level
        @param PairingParameters_MaxEncryptionKeySize: Maximum LTK size supported by the device
        @param PairingParameters_LocalIoCapabilities: I/O capabilities used to determine the pairing method
        @param PairingParameters_OobAvailable: TRUE if this device has Out-of-Band data that can be used for authenticated pairing, FALSE otherwise
        @param PairingParameters_CentralKeys: SMP keys to be distributed by the Central
        @param PairingParameters_PeripheralKeys: SMP keys to be distributed by the Peripheral
        @param PairingParameters_LeSecureConnectionSupported: indicates if device supports LE Secure Connections pairing
        @param PairingParameters_UseKeypressNotifications: Indicates if device supports Keypress Notification PDUs during Passkey Entry pairing
        '''
        self.DeviceId = DeviceId
        self.PairingParameters_WithBonding = PairingParameters_WithBonding
        self.PairingParameters_SecurityModeAndLevel = PairingParameters_SecurityModeAndLevel
        self.PairingParameters_MaxEncryptionKeySize = PairingParameters_MaxEncryptionKeySize
        self.PairingParameters_LocalIoCapabilities = PairingParameters_LocalIoCapabilities
        self.PairingParameters_OobAvailable = PairingParameters_OobAvailable
        self.PairingParameters_CentralKeys = PairingParameters_CentralKeys
        self.PairingParameters_PeripheralKeys = PairingParameters_PeripheralKeys
        self.PairingParameters_LeSecureConnectionSupported = PairingParameters_LeSecureConnectionSupported
        self.PairingParameters_UseKeypressNotifications = PairingParameters_UseKeypressNotifications


class GAPConnectionEventSlaveSecurityRequestIndication(object):

    def __init__(self, DeviceId=bytearray(1), SlaveSecurityRequestParameters_BondAfterPairing=bytearray(1), SlaveSecurityRequestParameters_bAuthenticationRequired=bytearray(1)):
        '''
        @param DeviceId: Device ID identifying the connection
        @param SlaveSecurityRequestParameters_BondAfterPairing: TRUE if the Slave supports bonding, FALSE otherwise
        @param SlaveSecurityRequestParameters_bAuthenticationRequired: TRUE if the slave requires authentication for MITM protection, FALSE otherwise
        '''
        self.DeviceId = DeviceId
        self.SlaveSecurityRequestParameters_BondAfterPairing = SlaveSecurityRequestParameters_BondAfterPairing
        self.SlaveSecurityRequestParameters_bAuthenticationRequired = SlaveSecurityRequestParameters_bAuthenticationRequired


class GAPConnectionEventPairingResponseIndication(object):

    def __init__(self, DeviceId=bytearray(1), PairingParameters_WithBonding=bytearray(1), PairingParameters_SecurityModeAndLevel=GAPConnectionEventPairingResponseIndicationPairingParameters_SecurityModeAndLevel.gMode1Level1_c, PairingParameters_MaxEncryptionKeySize=bytearray(1), PairingParameters_LocalIoCapabilities=GAPConnectionEventPairingResponseIndicationPairingParameters_LocalIoCapabilities.gIoDisplayOnly_c, PairingParameters_OobAvailable=bytearray(1), PairingParameters_CentralKeys=GAPConnectionEventPairingResponseIndicationPairingParameters_CentralKeys.gNoKeys_c, PairingParameters_PeripheralKeys=GAPConnectionEventPairingResponseIndicationPairingParameters_PeripheralKeys.gNoKeys_c, PairingParameters_LeSecureConnectionSupported=bytearray(1), PairingParameters_UseKeypressNotifications=bytearray(1)):
        '''
        @param DeviceId: Device ID identifying the connection
        @param PairingParameters_WithBonding: TRUE if this device is able to and wants to bond after pairing, FALSE otherwise
        @param PairingParameters_SecurityModeAndLevel: The desired security mode-level
        @param PairingParameters_MaxEncryptionKeySize: Maximum LTK size supported by the device
        @param PairingParameters_LocalIoCapabilities: I/O capabilities used to determine the pairing method
        @param PairingParameters_OobAvailable: TRUE if this device has Out-of-Band data that can be used for authenticated pairing, FALSE otherwise
        @param PairingParameters_CentralKeys: SMP keys to be distributed by the Central
        @param PairingParameters_PeripheralKeys: SMP keys to be distributed by the Peripheral
        @param PairingParameters_LeSecureConnectionSupported: indicates if device supports LE Secure Connections pairing
        @param PairingParameters_UseKeypressNotifications: Indicates if device supports Keypress Notification PDUs during Passkey Entry pairing
        '''
        self.DeviceId = DeviceId
        self.PairingParameters_WithBonding = PairingParameters_WithBonding
        self.PairingParameters_SecurityModeAndLevel = PairingParameters_SecurityModeAndLevel
        self.PairingParameters_MaxEncryptionKeySize = PairingParameters_MaxEncryptionKeySize
        self.PairingParameters_LocalIoCapabilities = PairingParameters_LocalIoCapabilities
        self.PairingParameters_OobAvailable = PairingParameters_OobAvailable
        self.PairingParameters_CentralKeys = PairingParameters_CentralKeys
        self.PairingParameters_PeripheralKeys = PairingParameters_PeripheralKeys
        self.PairingParameters_LeSecureConnectionSupported = PairingParameters_LeSecureConnectionSupported
        self.PairingParameters_UseKeypressNotifications = PairingParameters_UseKeypressNotifications


class GAPConnectionEventAuthenticationRejectedIndication(object):

    def __init__(self, DeviceId=bytearray(1), RejectReason=GAPConnectionEventAuthenticationRejectedIndicationRejectReason.gOobNotAvailable_c):
        '''
        @param DeviceId: Device ID identifying the connection
        @param RejectReason: The reason of the rejection
        '''
        self.DeviceId = DeviceId
        self.RejectReason = RejectReason


class GAPConnectionEventPasskeyRequestIndication(object):

    def __init__(self, DeviceId=bytearray(1)):
        '''
        @param DeviceId: Device ID identifying the connection
        '''
        self.DeviceId = DeviceId


class GAPConnectionEventOobRequestIndication(object):

    def __init__(self, DeviceId=bytearray(1)):
        '''
        @param DeviceId: Device ID identifying the connection
        '''
        self.DeviceId = DeviceId


class GAPConnectionEventPasskeyDisplayIndication(object):

    def __init__(self, DeviceId=bytearray(1), PasskeyForDisplay=bytearray(4)):
        '''
        @param DeviceId: Device ID identifying the connection
        @param PasskeyForDisplay: Passkey to be displayed by this Slave
        '''
        self.DeviceId = DeviceId
        self.PasskeyForDisplay = PasskeyForDisplay


class GAPConnectionEventKeyExchangeRequestIndication(object):

    def __init__(self, DeviceId=bytearray(1), RequestedKeys=GAPConnectionEventKeyExchangeRequestIndicationRequestedKeys.gNoKeys_c, RequestedLtkSize=bytearray(1)):
        '''
        @param DeviceId: Device ID identifying the connection
        @param RequestedKeys: Mask identifying the keys being requested
        @param RequestedLtkSize: Requested size of the encryption key
        '''
        self.DeviceId = DeviceId
        self.RequestedKeys = RequestedKeys
        self.RequestedLtkSize = RequestedLtkSize


class GAPConnectionEventKeysReceivedIndication(object):

    def __init__(self, DeviceId=bytearray(1)):
        '''
        @param DeviceId: Device ID identifying the connection
        '''
        self.DeviceId = DeviceId


class GAPConnectionEventLongTermKeyRequestIndication(object):

    def __init__(self, DeviceId=bytearray(1), Ediv=bytearray(2), RandSize=bytearray(1), Rand=[]):
        '''
        @param DeviceId: Device ID identifying the connection
        @param Ediv: The Encryption Diversifier, as defined by the SMP
        @param RandSize: Random number size
        @param Rand: The Random number, as defined by the SMP
        '''
        self.DeviceId = DeviceId
        self.Ediv = Ediv
        self.RandSize = RandSize
        self.Rand = Rand


class GAPConnectionEventEncryptionChangedIndication(object):

    def __init__(self, DeviceId=bytearray(1), NewEncryptionState=bytearray(1)):
        '''
        @param DeviceId: Device ID identifying the connection
        @param NewEncryptionState: TRUE if link has been encrypted, FALSE if encryption was paused or removed
        '''
        self.DeviceId = DeviceId
        self.NewEncryptionState = NewEncryptionState


class GAPConnectionEventPairingCompleteIndication(object):

    class PairingSuccessful(object):

        def __init__(self, WithBonding=bytearray(1)):
            self.WithBonding = WithBonding

    class PairingFailed(object):

        def __init__(self, FailReason=PairingFailedFailReason.gBleSuccess_c):
            self.FailReason = FailReason

    def __init__(self, DeviceId=bytearray(1), PairingStatus=GAPConnectionEventPairingCompleteIndicationPairingStatus.PairingSuccessful, PairingData=[]):
        '''
        @param DeviceId: Device ID identifying the connection
        @param PairingStatus: Pairing status
        @param PairingData: Information of pairing completion
        '''
        self.DeviceId = DeviceId
        self.PairingStatus = PairingStatus
        self.PairingData = PairingData


class GAPConnectionEventDisconnectedIndication(object):

    def __init__(self, DeviceId=bytearray(1), Reason=GAPConnectionEventDisconnectedIndicationReason.gBleSuccess_c):
        '''
        @param DeviceId: Device ID identifying the connection
        @param Reason: Reason for disconnection
        '''
        self.DeviceId = DeviceId
        self.Reason = Reason


class GAPConnectionEventRssiReadIndication(object):

    def __init__(self, DeviceId=bytearray(1), Rssi=bytearray(1)):
        '''
        @param DeviceId: Device ID identifying the connection
        @param Rssi: Value of the RSSI
        '''
        self.DeviceId = DeviceId
        self.Rssi = Rssi


class GAPConnectionEventTxPowerLevelReadIndication(object):

    def __init__(self, DeviceId=bytearray(1), TxPowerLevel=bytearray(1)):
        '''
        @param DeviceId: Device ID identifying the connection
        @param TxPowerLevel: Value of the TX power
        '''
        self.DeviceId = DeviceId
        self.TxPowerLevel = TxPowerLevel


class GAPConnectionEventPowerReadFailureIndication(object):

    def __init__(self, DeviceId=bytearray(1), FailReason=GAPConnectionEventPowerReadFailureIndicationFailReason.gBleSuccess_c):
        '''
        @param DeviceId: Device ID identifying the connection
        @param FailReason: Reason for power reading failure
        '''
        self.DeviceId = DeviceId
        self.FailReason = FailReason


class GAPConnectionEventParameterUpdateRequestIndication(object):

    def __init__(self, DeviceId=bytearray(1), ConnectionParameterUpdateRequest_IntervalMin=bytearray(2), ConnectionParameterUpdateRequest_IntervalMax=bytearray(2), ConnectionParameterUpdateRequest_SlaveLatency=bytearray(2), ConnectionParameterUpdateRequest_TimeoutMultiplier=bytearray(2)):
        '''
        @param DeviceId: Device ID identifying the connection
        @param ConnectionParameterUpdateRequest_IntervalMin: Minimum interval
        @param ConnectionParameterUpdateRequest_IntervalMax: Maximum interval
        @param ConnectionParameterUpdateRequest_SlaveLatency: Slave latency
        @param ConnectionParameterUpdateRequest_TimeoutMultiplier: Timeout multiplier
        '''
        self.DeviceId = DeviceId
        self.ConnectionParameterUpdateRequest_IntervalMin = ConnectionParameterUpdateRequest_IntervalMin
        self.ConnectionParameterUpdateRequest_IntervalMax = ConnectionParameterUpdateRequest_IntervalMax
        self.ConnectionParameterUpdateRequest_SlaveLatency = ConnectionParameterUpdateRequest_SlaveLatency
        self.ConnectionParameterUpdateRequest_TimeoutMultiplier = ConnectionParameterUpdateRequest_TimeoutMultiplier


class GAPConnectionEventParameterUpdateCompleteIndication(object):

    def __init__(self, DeviceId=bytearray(1), ConnectionParameterUpdateComplete_Status=GAPConnectionEventParameterUpdateCompleteIndicationConnectionParameterUpdateComplete_Status.gBleSuccess_c, ConnectionParameterUpdateComplete_ConnInterval=bytearray(2), ConnectionParameterUpdateComplete_ConnLatency=bytearray(2), ConnectionParameterUpdateComplete_SupervisionTimeout=bytearray(2)):
        '''
        @param DeviceId: Device ID identifying the connection
        @param ConnectionParameterUpdateComplete_Status: The status of the request
        @param ConnectionParameterUpdateComplete_ConnInterval: Connection interval
        @param ConnectionParameterUpdateComplete_ConnLatency: Connection latency
        @param ConnectionParameterUpdateComplete_SupervisionTimeout: Supervision timeout
        '''
        self.DeviceId = DeviceId
        self.ConnectionParameterUpdateComplete_Status = ConnectionParameterUpdateComplete_Status
        self.ConnectionParameterUpdateComplete_ConnInterval = ConnectionParameterUpdateComplete_ConnInterval
        self.ConnectionParameterUpdateComplete_ConnLatency = ConnectionParameterUpdateComplete_ConnLatency
        self.ConnectionParameterUpdateComplete_SupervisionTimeout = ConnectionParameterUpdateComplete_SupervisionTimeout


class GAPConnectionEventLeDataLengthChangedIndication(object):

    def __init__(self, DeviceId=bytearray(1), LeDataLengthChanged_MaxTxOctets=bytearray(2), LeDataLengthChanged_MaxTxTime=bytearray(2), LeDataLengthChanged_MaxRxOctets=bytearray(2), LeDataLengthChanged_MaxRxTime=bytearray(2)):
        '''
        @param DeviceId: Device ID identifying the connection
        @param LeDataLengthChanged_MaxTxOctets: The maximum number of payload octets in a Link Layer Data Channel PDU to transmit on this connection
        @param LeDataLengthChanged_MaxTxTime: The maximum time that the local Controller will take to send a Link Layer Data Channel PDU on this connection
        @param LeDataLengthChanged_MaxRxOctets: The maximum number of payload octets in a Link Layer Data Channel PDU to receive on this connection
        @param LeDataLengthChanged_MaxRxTime: The maximum time that the local Controller will take to receive a Link Layer Data Channel PDU on this connection
        '''
        self.DeviceId = DeviceId
        self.LeDataLengthChanged_MaxTxOctets = LeDataLengthChanged_MaxTxOctets
        self.LeDataLengthChanged_MaxTxTime = LeDataLengthChanged_MaxTxTime
        self.LeDataLengthChanged_MaxRxOctets = LeDataLengthChanged_MaxRxOctets
        self.LeDataLengthChanged_MaxRxTime = LeDataLengthChanged_MaxRxTime


class GAPConnectionEventLeScOobDataRequestIndication(object):

    def __init__(self, DeviceId=bytearray(1)):
        '''
        @param DeviceId: Device ID identifying the connection
        '''
        self.DeviceId = DeviceId


class GAPConnectionEventLeScDisplayNumericValueIndication(object):

    def __init__(self, DeviceId=bytearray(1), ncValue=bytearray(4)):
        '''
        @param DeviceId: Device ID identifying the connection
        @param ncValue: A number between 0 and 999999 decimal which must be confirmed by the user on both pairing devices
        '''
        self.DeviceId = DeviceId
        self.ncValue = ncValue


class GAPConnectionEventLeScKeypressNotificationIndication(object):

    def __init__(self, DeviceId=bytearray(1), GapLeScKeypressNotificationParams_keypressNotifType=GAPConnectionEventLeScKeypressNotificationIndicationGapLeScKeypressNotificationParams_keypressNotifType.gKnPasskeyEntryStarted_c):
        '''
        @param DeviceId: Device ID identifying the connection
        @param GapLeScKeypressNotificationParams_keypressNotifType: keypressNotifType
        '''
        self.DeviceId = DeviceId
        self.GapLeScKeypressNotificationParams_keypressNotifType = GapLeScKeypressNotificationParams_keypressNotifType


class GAPGenericEventControllerResetCompleteIndication(object):

    pass


class GAPLeScPublicKeyRegeneratedIndication(object):

    pass


class GAPGenericEventLeScLocalOobDataIndication(object):

    def __init__(self, RandomValue=bytearray(16), ConfirmValue=bytearray(16)):
        '''
        @param RandomValue: LE SC OOB r (Random value)
        @param ConfirmValue: LE SC OOB Cr (Random Confirm value)
        '''
        self.RandomValue = RandomValue
        self.ConfirmValue = ConfirmValue


class GAPGenericEventControllerPrivacyStateChangedIndication(object):

    def __init__(self, NewControllerPrivacyState=bytearray(1)):
        '''
        @param NewControllerPrivacyState: Indicates the new state of the controller privacy: TRUE if enabled, FALSE if disabled
        '''
        self.NewControllerPrivacyState = NewControllerPrivacyState


class GAPGetBondedDevicesIdentityInformationIndication(object):

    class IdentityAddresses(object):

        def __init__(self, IdentityAddressType=IdentityAddressesIdentityAddressType.gPublic_c, IdentityAddress=bytearray(6), Irk=bytearray(16)):
            self.IdentityAddressType = IdentityAddressType
            # Unit length: 6 bytes
            self.IdentityAddress = IdentityAddress
            # Unit length: 16 bytes
            self.Irk = Irk

    def __init__(self, NbOfDeviceIdentityAddresses=bytearray(1), IdentityAddresses=[]):
        '''
        @param NbOfDeviceIdentityAddresses: The number of device identity addresses
        @param IdentityAddresses: Identity addresses list
        '''
        self.NbOfDeviceIdentityAddresses = NbOfDeviceIdentityAddresses
        self.IdentityAddresses = IdentityAddresses
