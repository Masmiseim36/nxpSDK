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

from com.nxp.wireless_connectivity.commands.thread.enums import *  # @UnusedWildImport
from com.nxp.wireless_connectivity.hsdk.utils import list_to_int, to_bytes


class MAC_MacFilteringAddEntryRequest(object):

    def __init__(self, InstanceId=bytearray(1), ExtendedAddress=bytearray(8), ShortAddress=bytearray(2), LinkIndicator=bytearray(1), BlockNeighbor=bytearray(1)):
        '''
        @param InstanceId: Instance Id
        @param ExtendedAddress: Extended Address
        @param ShortAddress: Short Address
        @param LinkIndicator: The neighbor Quality Link Indicator: Good Link: 20 - 255 Medium Link: 11 - 20 Bad Link: 3 - 10.
        @param BlockNeighbor: Add this neighbor to blacklist
        '''
        self.InstanceId = InstanceId
        self.ExtendedAddress = ExtendedAddress
        self.ShortAddress = ShortAddress
        self.LinkIndicator = LinkIndicator
        self.BlockNeighbor = BlockNeighbor


class MAC_MacFilteringRemoveEntryRequest(object):

    def __init__(self, InstanceId=bytearray(1), ExtendedAddress=bytearray(8)):
        '''
        @param InstanceId: Instance Id
        @param ExtendedAddress: Extended Address
        '''
        self.InstanceId = InstanceId
        self.ExtendedAddress = ExtendedAddress


class MAC_MacFilteringEnableRequest(object):

    def __init__(self, InstanceId=bytearray(1), MacFiltering=MAC_MacFilteringEnableRequestMacFiltering.Disable):
        '''
        @param InstanceId: Instance Id
        @param MacFiltering: Enable or disable mac filtering
        '''
        self.InstanceId = InstanceId
        self.MacFiltering = MacFiltering


class MAC_MacFilteringGetTableRequest(object):

    def __init__(self, InstanceId=bytearray(1), StartIndex=bytearray(1), NoOfElements=bytearray(1)):
        '''
        @param InstanceId: Instance Id
        @param StartIndex: Start Index
        @param NoOfElements: No of elements to print
        '''
        self.InstanceId = InstanceId
        self.StartIndex = StartIndex
        self.NoOfElements = NoOfElements


class THR_SetDeviceConfigRequest(object):

    def __init__(self, ThrInstanceID=bytearray(1), OutOfBandPreconfigured=bytearray(1), outOfBandChannel=bytearray(1), PanId=bytearray(2), ScanChannels=bytearray(4), ExtendedPanId=[], NwkNameSize=bytearray(1), NwkName=[], MLPrefix=[], MLprefixSizeInBits=bytearray(1), MasterKey=bytearray(16)):
        '''
        @param ThrInstanceID: Thread instance ID
        @param OutOfBandPreconfigured: OutOfBandPreconfigured
        @param outOfBandChannel: outOfBandChannel
        @param PanId: PanId
        @param ScanChannels: ScanChannels
        @param ExtendedPanId: Extended PAN ID
        @param NwkNameSize: Network Name Size
        @param NwkName: Network Name
        @param MLPrefix: MLPrefix
        @param MLprefixSizeInBits: ML Prefix Size
        @param MasterKey: Master Key
        '''
        self.ThrInstanceID = ThrInstanceID
        self.OutOfBandPreconfigured = OutOfBandPreconfigured
        self.outOfBandChannel = outOfBandChannel
        self.PanId = PanId
        self.ScanChannels = ScanChannels
        self.ExtendedPanId = ExtendedPanId
        self.NwkNameSize = NwkNameSize
        self.NwkName = NwkName
        self.MLPrefix = MLPrefix
        self.MLprefixSizeInBits = MLprefixSizeInBits
        self.MasterKey = MasterKey


class THR_NwkScanRequest(object):

    def __init__(self, InstanceID=bytearray(1), ScanChannelMask=bytearray(4), ScanType=THR_NwkScanRequestScanType.EnergyDetect, ScanDuration=bytearray(1), maxThrNwkToDiscover=bytearray(2)):
        '''
        @param InstanceID: InstanceID
        @param ScanChannelMask: Channel mask
        @param ScanType: Scan type
        @param ScanDuration: Scan Duration. Exponential scale, as seen in the 802.15.4 specification (Range:1 - 14)
        @param maxThrNwkToDiscover: maximum thread network to be discovered
        '''
        self.InstanceID = InstanceID
        self.ScanChannelMask = ScanChannelMask
        self.ScanType = ScanType
        self.ScanDuration = ScanDuration
        self.maxThrNwkToDiscover = maxThrNwkToDiscover


class THR_CreateNwkRequest(object):

    def __init__(self, InstanceID=bytearray(1)):
        '''
        @param InstanceID: InstanceID
        '''
        self.InstanceID = InstanceID


class THR_JoinRequest(object):

    def __init__(self, InstanceID=bytearray(1), discoveryMethod=THR_JoinRequestdiscoveryMethod.gUseThreadDiscovery_c):
        '''
        @param InstanceID: thread instance id
        @param discoveryMethod: discovery method
        '''
        self.InstanceID = InstanceID
        self.discoveryMethod = discoveryMethod


class THR_FactoryResetRequest(object):

    pass


class THR_CpuResetRequest(object):

    def __init__(self, TimeoutMs=bytearray(4)):
        '''
        @param TimeoutMs: The time in milliseconds after the device will reset.
        '''
        self.TimeoutMs = TimeoutMs


class THR_DisconnectRequest(object):

    def __init__(self, InstanceID=bytearray(1)):
        '''
        @param InstanceID: Thread instance id
        '''
        self.InstanceID = InstanceID


class THR_AttachRequest(object):

    def __init__(self, InstanceID=bytearray(1)):
        '''
        @param InstanceID: InstanceID
        '''
        self.InstanceID = InstanceID


class THR_PromoteAsRouterRequest(object):

    def __init__(self, InstanceID=bytearray(1), Reason=THR_PromoteAsRouterRequestReason.TooFewRouters):
        '''
        @param InstanceID: InstanceID
        @param Reason: Promoting reason
        '''
        self.InstanceID = InstanceID
        self.Reason = Reason


class THR_MgmtDiagnosticGetRequest(object):

    def __init__(self, InstanceId=bytearray(1), DestIpAddr=[], NumberOfTlvIds=bytearray(1), TlvIds=[]):
        '''
        @param InstanceId: Thread instance Id
        @param DestIpAddr: Destination Ip Address
        @param NumberOfTlvIds: Number of TLV Ids
        @param TlvIds: TlvIds
        '''
        self.InstanceId = InstanceId
        self.DestIpAddr = DestIpAddr
        self.NumberOfTlvIds = NumberOfTlvIds
        self.TlvIds = TlvIds

    def pickle(self):
        result = bytearray()
        result += to_bytes(self.InstanceId, 1)
        result += to_bytes(self.DestIpAddr, 16)
        result += to_bytes(self.NumberOfTlvIds, 1)
        result += bytearray(self.TlvIds)
        return result


class THR_DiagTestGetRequest(object):

    class Data(object):

        def __init__(self, SequenceNumber=bytearray(1), Payload=[]):
            self.SequenceNumber = SequenceNumber
            # Array length depends on PayloadSize.
            self.Payload = Payload

    def __init__(self, InstanceId=bytearray(1), DestIpAddr=bytearray(16), PayloadSize=bytearray(1), TlvId=THR_DiagTestGetRequestTlvId.ColdFactoryReset, AttributeValue=Data()):
        '''
        @param InstanceId: Thread instance Id
        @param DestIpAddr: Destination Ip Address
        @param PayloadSize: Size of payload - enter manually
        @param TlvId: TlvId
        @param AttributeValue: Attribute Value
        '''
        self.InstanceId = InstanceId
        self.DestIpAddr = DestIpAddr
        self.PayloadSize = PayloadSize
        self.TlvId = TlvId
        self.AttributeValue = AttributeValue

    def pickle(self):
        result = bytearray()
        result += to_bytes(self.InstanceId, 1)
        result += to_bytes(self.DestIpAddr, 16)
        result += to_bytes(self.PayloadSize, 1)
        result += to_bytes(self.TlvId, 1)
        if self.TlvId == THR_DiagTestGetRequestTlvId.Data:
            result += to_bytes(self.AttributeValue.SequenceNumber, 1)
            result += to_bytes(self.AttributeValue.Payload, self.PayloadSize)

        return result


class THR_MgmtDiagnosticResetRequest(object):

    def __init__(self, InstanceId=bytearray(1), DestIpAddr=[], NumberOfTlvIds=bytearray(1), TlvIds=[]):
        '''
        @param InstanceId: Thread instance Id
        @param DestIpAddr: Destination Ip Address
        @param NumberOfTlvIds: Number of TLV Ids
        @param TlvIds: TlvIds
        '''
        self.InstanceId = InstanceId
        self.DestIpAddr = DestIpAddr
        self.NumberOfTlvIds = NumberOfTlvIds
        self.TlvIds = TlvIds

    def pickle(self):
        result = bytearray()
        result += to_bytes(self.InstanceId, 1)
        result += to_bytes(self.DestIpAddr, 16)
        result += to_bytes(self.NumberOfTlvIds, 1)
        result += bytearray(self.TlvIds)
        return result


class THR_MgmtReadMemoryRequest(object):

    def __init__(self, Address=bytearray(4), Length=bytearray(1)):
        '''
        Reads 'Length' bytes from the given memory address.

        @param Address: Memory address
        @param Length: Length
        '''
        self.Address = Address
        self.Length = Length


class THR_MgmtWriteMemoryRequest(object):

    def __init__(self, Address=bytearray(4), Length=bytearray(1), Value=[]):
        '''
        Writes 'Length' bytes to the given memory address.

        @param Address: Memory address
        @param Length: Length
        @param Value: Value
        '''
        self.Address = Address
        self.Length = Length
        self.Value = Value


class THR_NwkDiscoveryRequest(object):

    def __init__(self, InstanceID=bytearray(1)):
        '''
        @param InstanceID: thread instance id
        '''
        self.InstanceID = InstanceID


class THR_NwkDiscoveryStopRequest(object):

    def __init__(self, InstanceID=bytearray(1)):
        '''
        @param InstanceID: thread instance id
        '''
        self.InstanceID = InstanceID


class THR_SearchNwkWithAnounceRequest(object):

    def __init__(self, InstanceID=bytearray(1)):
        '''
        @param InstanceID: thread instance id
        '''
        self.InstanceID = InstanceID


class THR_ChildUpdateToParentRequest(object):

    def __init__(self, InstanceID=bytearray(1)):
        '''
        @param InstanceID: InstanceID
        '''
        self.InstanceID = InstanceID


class THR_SetManualSlaacIIDRequest(object):

    def __init__(self, IID=bytearray(8)):
        '''
        @param IID: IID
        '''
        self.IID = IID


class THR_SendProactiveAddrNotifRequest(object):

    def __init__(self, InstanceId=bytearray(1), DestinationIPAddress=bytearray(16)):
        '''
        @param InstanceId: Thread instance Id
        @param DestinationIPAddress: DestinationIPAddress
        '''
        self.InstanceId = InstanceId
        self.DestinationIPAddress = DestinationIPAddress


class THR_SetThresholdRequest(object):

    def __init__(self, InstanceId=bytearray(1), ThresholdType=THR_SetThresholdRequestThresholdType.RouterUpgradeThreshold, Value=bytearray(1)):
        '''
        @param InstanceId: Thread instance Id
        @param ThresholdType: Threshold type, e.g. router upgrade/downgrade
        @param Value: Value
        '''
        self.InstanceId = InstanceId
        self.ThresholdType = ThresholdType
        self.Value = Value


class THR_SetNwkIdTimeoutRequest(object):

    def __init__(self, TimeoutInSeconds=bytearray(4)):
        '''
        @param TimeoutInSeconds: TimeoutInSeconds
        '''
        self.TimeoutInSeconds = TimeoutInSeconds


class THR_GetRoutingTableRequest(object):

    def __init__(self, InstanceId=bytearray(1), StartIndex=bytearray(1), NoOfElements=bytearray(1)):
        '''
        @param InstanceId: Instance Id
        @param StartIndex: Start Index
        @param NoOfElements: No of elements to print
        '''
        self.InstanceId = InstanceId
        self.StartIndex = StartIndex
        self.NoOfElements = NoOfElements


class THR_GetNeighborTableRequest(object):

    def __init__(self, InstanceId=bytearray(1), StartIndex=bytearray(1), NoOfElements=bytearray(1)):
        '''
        @param InstanceId: Instance Id
        @param StartIndex: Start Index
        @param NoOfElements: No of elements to print
        '''
        self.InstanceId = InstanceId
        self.StartIndex = StartIndex
        self.NoOfElements = NoOfElements


class THR_GetNeighborInfoRequest(object):

    def __init__(self, InstanceId=bytearray(1), ShortAddress=bytearray(2)):
        '''
        @param InstanceId: Instance Id
        @param ShortAddress: Short Address of the neighbor
        '''
        self.InstanceId = InstanceId
        self.ShortAddress = ShortAddress


class THR_GetChildrenTableRequest(object):

    def __init__(self, InstanceId=bytearray(1), StartIndex=bytearray(1), NoOfElements=bytearray(1)):
        '''
        @param InstanceId: Instance Id
        @param StartIndex: Start Index
        @param NoOfElements: No of elements to print
        '''
        self.InstanceId = InstanceId
        self.StartIndex = StartIndex
        self.NoOfElements = NoOfElements


class THR_GetAttrRequest(object):

    def __init__(self, InstanceId=bytearray(1), AttributeId=THR_GetAttrRequestAttributeId.RandomExtendedAddr, Index=bytearray(1)):
        '''
        @param InstanceId: Instance Id
        @param AttributeId: Allowed values in THR_GetAttrRequestAttributeId enum class.
        @param Index: Index into a table
        '''
        self.InstanceId = InstanceId
        self.AttributeId = AttributeId
        self.Index = Index


class THR_SetAttrRequest(object):

    def __init__(self, InstanceId=bytearray(1), AttributeId=bytearray(1), Index=bytearray(1), AttrSize=bytearray(1), AttributeValue=[]):
        '''
        @param InstanceId: Instance Id
        @param AttributeId: Allowed values in THR_SetAttrRequestAttributeId enum class.
        @param Index: Index in table if required
        @param AttributeValue: Attribute Value
        '''
        self.InstanceId = InstanceId
        self.AttributeId = AttributeId
        self.Index = Index
        self.AttrSize = AttrSize
        self.AttributeValue = AttributeValue


class THR_LeaderRemoveRouterIdRequest(object):

    def __init__(self, InstanceId=bytearray(1), RouterShortAddr=bytearray(2)):
        '''
        @param InstanceId: InstanceId
        @param RouterShortAddr: Router short address.
        '''
        self.InstanceId = InstanceId
        self.RouterShortAddr = RouterShortAddr


class THR_GenerateAllKeysRequest(object):

    def __init__(self, InstanceId=bytearray(1), KeySequenceCounter=bytearray(4)):
        '''
        @param InstanceId: Thread instance Id
        @param KeySequenceCounter: Key sequence counter
        '''
        self.InstanceId = InstanceId
        self.KeySequenceCounter = KeySequenceCounter


class THR_SwitchKeyRequest(object):

    def __init__(self, InstanceId=bytearray(1), KeySequenceCounter=bytearray(4)):
        '''
        @param InstanceId: Thread instance Id
        @param KeySequenceCounter: Key sequence counter
        '''
        self.InstanceId = InstanceId
        self.KeySequenceCounter = KeySequenceCounter


class THR_BrPrefixAddEntryRequest(object):

    def __init__(self, InstanceId=bytearray(1), prefixLength=bytearray(1), PrefixValue=bytearray(16), PrefixFlagsReserved=bytearray(1), PrefixFlags=bytearray(1), prefixLifetime=bytearray(4), prefixAdvertised=bytearray(1), ExternalRouteFlags=ExternalRouteFlagsR_preference.MediumDefault, ExternalRouteLifetime=bytearray(4), ExternalRouteAdvertised=bytearray(1)):
        '''
        :param InstanceId:                  Instance ID of the Thread stack
        :param prefixLength:                Prefix length
        :param PrefixValue:                 IPv6 prefix
        :param PrefixFlagsReserved:         Shall remain 0
        :param PrefixFlags:                 1 byte formatted as follows:
                             Prf                   P                     S                 D                C                    R                  O
                    |      2 bits       |        1 bit        |        1 bit      |      1 bit     |      1 bit      |         1 bit        |      1 bit     |

            Prf: P_preference is a two-bit signed integer indicating Router preference as defined in RFC 4191. It is valid only if P_default is true.
            P: P_preferred is true if addresses autoconfigured using P_prefix are considered preferred address; if false, then such addresses are considered deprecated.
            S: P_slaac is true if network devices are allowed to autoconfigure addresses using P_prefix; otherwise, false.
            D: P_dhcp is true if P_border_router is a DHCPv6 Agent that manages address configuration for P_prefix; otherwise, false.
            C: P_configure is true if P_border_router is a DHCPv6 Agent that supplies other configuration data, such as the identity of DNS servers; otherwise, false.
            R: P_default is true if P_border_router offers a default route for messages whose IP source uses P_prefix; otherwise, false.
            O: P_on_mesh is true if P_prefix is considered to be on-mesh; if false then P_prefix is not considered to be on-mesh.

        :param prefixLifetime:              Lifetime of the prefix
        :param prefixAdvertised:            Whether or not to advertise the prefix. MUST be True for network data propagation.
        :param ExternalRouteFlags:          The preference of the added prefix. May take one of the values:
                                                    MediumDefault = 0x00
                                                    High = 0x01
                                                    # Reserved - MUST NOT be used
                                                    Reserved = 0x02
                                                    Low = 0x03
        :param ExternalRouteLifetime:       The lifetime of the external route
        :param ExternalRouteAdvertised:     MUST be True for external route propagation
        '''
        self.InstanceId = InstanceId
        self.prefixLength = prefixLength
        self.PrefixValue = PrefixValue
        self.PrefixFlagsReserved = PrefixFlagsReserved
        self.PrefixFlags = PrefixFlags
        self.prefixLifetime = prefixLifetime
        self.prefixAdvertised = prefixAdvertised
        self.ExternalRouteFlags = ExternalRouteFlags
        self.ExternalRouteLifetime = ExternalRouteLifetime
        self.ExternalRouteAdvertised = ExternalRouteAdvertised


class THR_BrPrefixGetTableRequest(object):

    def __init__(self, InstanceId=bytearray(1), StartIndex=bytearray(1), NoOfElements=bytearray(1)):
        '''
        @param InstanceId: Instance Id
        @param StartIndex: Start Index
        @param NoOfElements: No of elements to print
        '''
        self.InstanceId = InstanceId
        self.StartIndex = StartIndex
        self.NoOfElements = NoOfElements


class THR_BrPrefixRemoveEntryRequest(object):

    def __init__(self, InstanceId=bytearray(1), prefixLength=bytearray(1), PrefixValue=bytearray(16)):
        '''
        @param InstanceId: Thread instance Id
        @param prefixLength: Prefix Length in bits
        @param PrefixValue: Prefix value
        '''
        self.InstanceId = InstanceId
        self.prefixLength = prefixLength
        self.PrefixValue = PrefixValue


class THR_BrServiceRemoveEntryRequest(object):

    def __init__(self, InstanceId=bytearray(1), ServiceName='dnsserver', ServerAddress=bytearray(16)):
        '''
        @param InstanceId: Thread instance Id
        @param ServiceName: 'dnsserver' or 'otaserver'
        @param ServerAddress: ServerIPAddress
        '''
        self.InstanceId = InstanceId
        self.ServiceName = ServiceName
        self.ServerAddress = ServerAddress


class THR_BrServiceAddEntryRequest(object):

    def __init__(self, InstanceId=bytearray(1), ServiceName='dnsserver', ServerAddress=bytearray(16)):
        '''
        @param InstanceId: Thread instance Id
        @param ServiceName: 'dnsserver' or 'otaserver'
        @param ServerAddress: ServerIPAddress
        '''
        self.InstanceId = InstanceId
        self.ServiceName = ServiceName
        self.ServerAddress = ServerAddress


class THR_BrPrefixSyncRequest(object):

    def __init__(self, InstanceId=bytearray(1)):
        '''
        @param InstanceId: Thread instance Id
        '''
        self.InstanceId = InstanceId


class THR_BrPrefixRemoveAllRequest(object):

    def __init__(self, InstanceId=bytearray(1)):
        '''
        @param InstanceId: Thread instance Id
        '''
        self.InstanceId = InstanceId


class THR_GetThreadIpAddrRequest(object):

    class Global(object):

        def __init__(self, StartIndex=bytearray(1), NoOfElements=bytearray(1)):
            self.StartIndex = StartIndex
            self.NoOfElements = NoOfElements

    class Anycast(object):

        def __init__(self, StartIndex=bytearray(1), NoOfElements=bytearray(1)):
            self.StartIndex = StartIndex
            self.NoOfElements = NoOfElements

    def __init__(self, InstanceId=bytearray(1), AddressType=THR_GetThreadIpAddrRequestAddressType.Link_Local_64, Data=[]):
        '''
        @param InstanceId: Thread instance Id
        @param AddressType: Address type
        @param Data: Data
        '''
        self.InstanceId = InstanceId
        self.AddressType = AddressType
        self.Data = Data


class THR_GetParentRequest(object):

    def __init__(self, InstanceId=bytearray(1)):
        '''
        @param InstanceId: Thread instance Id
        '''
        self.InstanceId = InstanceId


class NWKU_EidToRlocMapRequest(object):

    pass


class THR_IdentifyRequest(object):

    pass


class SocketCreateRequest(object):

    def __init__(self, SocketDomain=SocketCreateRequestSocketDomain(), SocketType=SocketCreateRequestSocketType.Datagram, SocketProtocol=SocketCreateRequestSocketProtocol()):
        '''
        @param SocketDomain: Socket Domain
        @param SocketType: Socket Type
        @param SocketProtocol: Socket Protocol
        '''
        self.SocketDomain = SocketDomain
        self.SocketType = SocketType
        self.SocketProtocol = SocketProtocol


class SocketShutdownRequest(object):

    def __init__(self, SocketIndex=bytearray(1)):
        '''
        @param SocketIndex: The socket index.
        '''
        self.SocketIndex = SocketIndex


class SocketBindRequest(object):

    def __init__(self, SocketIndex=bytearray(1), LocalIpAddress=bytearray(16), LocalPort=bytearray(2), SocketDomain=SocketBindRequestSocketDomain()):
        '''
        @param SocketIndex: The socket index.
        @param LocalIpAddress: The local IP address.
        @param LocalPort: The local port.
        @param SocketDomain: Socket Domain
        '''
        self.SocketIndex = SocketIndex
        self.LocalIpAddress = LocalIpAddress
        self.LocalPort = LocalPort
        self.SocketDomain = SocketDomain


class SocketSendRequest(object):

    def __init__(self, SocketIndex=bytearray(1), Flags=SocketSendRequestFlags(), Size=bytearray(2), Data=[]):
        '''
        @param SocketIndex: The socket index.
        @param Flags: Flags
        @param Size: The number of payload bytes
        @param Data: Data
        '''
        self.SocketIndex = SocketIndex
        self.Flags = Flags
        self.Size = Size
        self.Data = Data


class SocketSendToRequest(object):

    def __init__(self, SocketIndex=bytearray(1), Flags=SocketSendToRequestFlags(), SecuredMACData=bytearray(1), Size=bytearray(2), RemotePort=bytearray(2), RemoteIpAddress=bytearray(16), Data=[]):
        '''
        @param SocketIndex: The socket index.
        @param Flags: Flags
        @param SecuredMACData: The socket usses MAC security for data sent.
        @param Size: The number of payload bytes
        @param RemotePort: The remote port.
        @param RemoteIpAddress: The remote IP address.
        @param Data: Data
        '''
        self.SocketIndex = SocketIndex
        self.Flags = Flags
        self.SecuredMACData = SecuredMACData
        self.Size = Size
        self.RemotePort = RemotePort
        self.RemoteIpAddress = RemoteIpAddress
        self.Data = Data


class SocketReceiveRequest(object):

    def __init__(self, SocketIndex=bytearray(1), DataSize=bytearray(2), Flags=SocketReceiveRequestFlags()):
        '''
        @param SocketIndex: The socket index.
        @param DataSize: Data Size
        @param Flags: Flags
        '''
        self.SocketIndex = SocketIndex
        self.DataSize = DataSize
        self.Flags = Flags


class SocketReceiveFromRequest(object):

    def __init__(self, SocketIndex=bytearray(1), DataSize=bytearray(2), Flags=SocketReceiveFromRequestFlags()):
        '''
        @param SocketIndex: The socket index.
        @param DataSize: Data Size
        @param Flags: Flags
        '''
        self.SocketIndex = SocketIndex
        self.DataSize = DataSize
        self.Flags = Flags


class SocketConnectRequest(object):

    def __init__(self, SocketIndex=bytearray(1), RemoteIpAddress=bytearray(16), RemotePort=bytearray(2), SocketDomain=SocketConnectRequestSocketDomain()):
        '''
        @param SocketIndex: The socket index.
        @param RemoteIpAddress: The remote IP address.
        @param RemotePort: The remote port.
        @param SocketDomain: Socket Domain
        '''
        self.SocketIndex = SocketIndex
        self.RemoteIpAddress = RemoteIpAddress
        self.RemotePort = RemotePort
        self.SocketDomain = SocketDomain


class SocketListenRequest(object):

    def __init__(self, SocketIndex=bytearray(1), Backlog=bytearray(1)):
        '''
        @param SocketIndex: The socket index.
        @param Backlog: Backlog
        '''
        self.SocketIndex = SocketIndex
        self.Backlog = Backlog


class SocketAcceptRequest(object):

    def __init__(self, SocketIndex=bytearray(1)):
        '''
        @param SocketIndex: The socket index.
        '''
        self.SocketIndex = SocketIndex


class SocketSetOptionRequest(object):

    def __init__(self, SocketIndex=bytearray(1), SocketLevel=SocketSetOptionRequestSocketLevel.SOL_SOCKET, SocketOption=SocketSetOptionRequestSocketOption.SO_TYPE, SocketOptionValue=bytearray(4)):
        '''
        @param SocketIndex: The socket index.
        @param SocketLevel: Socket Level
        @param SocketOption: Socket Option
        @param SocketOptionValue: Socket Option Value
        '''
        self.SocketIndex = SocketIndex
        self.SocketLevel = SocketLevel
        self.SocketOption = SocketOption
        self.SocketOptionValue = SocketOptionValue


class SocketGetOptionRequest(object):

    def __init__(self, SocketIndex=bytearray(1), SocketLevel=SocketGetOptionRequestSocketLevel.SOL_SOCKET, SocketOption=SocketGetOptionRequestSocketOption.SO_TYPE):
        '''
        @param SocketIndex: The socket index.
        @param SocketLevel: Socket Level
        @param SocketOption: Socket Option
        '''
        self.SocketIndex = SocketIndex
        self.SocketLevel = SocketLevel
        self.SocketOption = SocketOption


class MESHCOP_StartCommissionerRequest(object):

    def __init__(self, InstanceId=bytearray(1)):
        '''
        @param InstanceId: Thread instance Id
        '''
        self.InstanceId = InstanceId


class MESHCOP_StartNativeCommissionerRequest(object):

    def __init__(self, InstanceId=bytearray(1)):
        '''
        @param InstanceId: Thread instance Id
        '''
        self.InstanceId = InstanceId


class MESHCOP_StopCommissionerRequest(object):

    def __init__(self, InstanceId=bytearray(1)):
        '''
        @param InstanceId: Thread instance Id
        '''
        self.InstanceId = InstanceId


class MESHCOP_AddExpectedJoinerRequest(object):

    def __init__(self, InstanceId=bytearray(1), Selected=bytearray(1), EuiType=MESHCOP_AddExpectedJoinerRequestEuiType(), EUI=[], PSKdSize=bytearray(1), PSKd=[]):
        '''
        @param InstanceId: Thread instance Id
        @param Selected: Allow this device?
        @param EuiType: The type of the EUI
        @param EUI: EUI
        @param PSKdSize: The size of the PSKd
        @param PSKd: PSKd
        '''
        self.InstanceId = InstanceId
        self.Selected = Selected
        self.EuiType = EuiType
        self.EUI = EUI
        self.PSKdSize = PSKdSize
        self.PSKd = PSKd


class MESHCOP_GetExpectedJoinerRequest(object):

    def __init__(self, InstanceId=bytearray(1), EuiType=MESHCOP_GetExpectedJoinerRequestEuiType(), EUI=[]):
        '''
        @param InstanceId: Thread instance Id
        @param EuiType: The type of the EUI
        @param EUI: EUI
        '''
        self.InstanceId = InstanceId
        self.EuiType = EuiType
        self.EUI = EUI


class MESHCOP_RemoveExpectedJoinerRequest(object):

    def __init__(self, InstanceId=bytearray(1), EuiType=MESHCOP_RemoveExpectedJoinerRequestEuiType(), EUI=[]):
        '''
        @param InstanceId: Thread instance Id
        @param EuiType: The type of the EUI
        @param EUI: EUI
        '''
        self.InstanceId = InstanceId
        self.EuiType = EuiType
        self.EUI = EUI


class MESHCOP_RemoveAllExpectedJoinersRequest(object):

    def __init__(self, InstanceId=bytearray(1)):
        '''
        @param InstanceId: Thread instance Id
        '''
        self.InstanceId = InstanceId


class MESHCOP_SyncSteeringDataRequest(object):

    def __init__(self, InstanceId=bytearray(1), EuiMask=MESHCOP_SyncSteeringDataRequestEuiMask.AllZeroes):
        '''
        @param InstanceId: Thread instance Id
        @param EuiMask: The EUI mask
        '''
        self.InstanceId = InstanceId
        self.EuiMask = EuiMask


class MESHCOP_MgmtSetRequest(object):

    def __init__(
        self,
        InstanceId=bytearray(1),
        PSKcEnable=bytearray(1), PSKcSize=bytearray(1), PSKc=[],
        NetworkNameEnable=bytearray(1), NetworkNameSize=bytearray(1), NetworkName=[],
        PolicyEnable=bytearray(1), Policy=bytearray(1), KeyRotationInterval=bytearray(2),
        TimestampEnable=bytearray(1), Seconds=bytearray(6), Ticks=bytearray(2)
    ):
        '''
        @param InstanceId: Thread instance Id
        @param PSKcEnable: PSKcEnable
        @param PSKcSize: PSKc size
        @param PSKc: PSKc
        @param NetworkNameEnable: NetworkNameEnable
        @param NetworkNameSize: Network Name Size
        @param NetworkName: Network Name
        @param PolicyEnable: PolicyEnable
        @param Policy: 1 byte as follows - 0b000000NO, where N - Native, O - OutOfBand
        @param KeyRotationInterval: KeyRotationInterval
        @param TimestampEnable: TimestampEnable
        @param Seconds: Seconds
        @param Ticks: Ticks
        '''
        self.InstanceId = InstanceId
        self.PSKcEnable = PSKcEnable
        self.PSKcSize = PSKcSize
        self.PSKc = PSKc
        self.NetworkNameEnable = NetworkNameEnable
        self.NetworkNameSize = NetworkNameSize
        self.NetworkName = NetworkName
        self.PolicyEnable = PolicyEnable
        self.Policy = Policy
        self.KeyRotationInterval = KeyRotationInterval
        self.TimestampEnable = TimestampEnable
        # Unit length: 6 bytes. Included if TimestampEnable.
        self.Seconds = Seconds
        # Unit length: 2 bytes. Included if TimestampEnable.
        self.Ticks = Ticks


class MESHCOP_MgmtGetRequest(object):

    def __init__(self, InstanceId=bytearray(1), NumberOfTlvIds=bytearray(1), TlvIds=[]):
        '''
        @param InstanceId: Thread instance Id
        @param NumberOfTlvIds: Number of TLV Ids
        @param TlvIds: TlvIds
        '''
        self.InstanceId = InstanceId
        self.NumberOfTlvIds = NumberOfTlvIds
        self.TlvIds = TlvIds

    def pickle(self):
        result = bytearray()
        result += to_bytes(self.InstanceId, 1)
        result += to_bytes(self.NumberOfTlvIds, 1)
        result += bytearray(self.TlvIds)
        return result


class MESHCOP_SetCommissionerCredentialRequest(object):

    def __init__(self, InstanceId=bytearray(1), PSKcSize=bytearray(1), PSKc=[], XpanId=bytearray(8), NetworkNameSize=bytearray(1), NetworkName=[]):
        '''
        @param InstanceId: Thread instance Id
        @param PSKcSize: PSKc size
        @param PSKc: PSKc
        @param XpanId: Extended Pan Id
        @param NetworkNameSize: Network Name Size
        @param NetworkName: Network Name
        '''
        self.InstanceId = InstanceId
        self.PSKcSize = PSKcSize
        self.PSKc = PSKc
        self.XpanId = XpanId
        self.NetworkNameSize = NetworkNameSize
        self.NetworkName = NetworkName


class MESHCOP_MgmNwkFormRequest(object):

    def __init__(self, IPAddress=bytearray(16), NwkNameSize=bytearray(1), NwkName=[], NetworkMasterKey=bytearray(16), PSKcSize=bytearray(1), PSKc=[], Channel=bytearray(1)):
        '''
        @param IPAddress: The IPv6 address of the device added to network
        @param NwkNameSize: Network Name Size
        @param NwkName: Network Name
        @param NetworkMasterKey: Network Master Key
        @param PSKcSize: Length of PSKc
        @param PSKc: PSKc
        @param Channel: Channel
        '''
        self.IPAddress = IPAddress
        self.NwkNameSize = NwkNameSize
        self.NwkName = NwkName
        self.NetworkMasterKey = NetworkMasterKey
        self.PSKcSize = PSKcSize
        self.PSKc = PSKc
        self.Channel = Channel


class MESHCOP_MgmtCommissionerGetRequest(object):

    def __init__(self, InstanceId=bytearray(1), IPAddress=bytearray(16), NumberOfTlvIds=bytearray(1), TlvIds=[]):
        '''
        @param InstanceId: Thread instance Id
        @param IPAddress: The IPv6 address
        @param NumberOfTlvIds: Number of TLV Ids
        @param TlvIds: TlvIds
        '''
        self.InstanceId = InstanceId
        self.IPAddress = IPAddress
        self.NumberOfTlvIds = NumberOfTlvIds
        self.TlvIds = TlvIds

    def pickle(self):
        result = bytearray()
        result += to_bytes(self.InstanceId, 1)
        result += to_bytes(self.IPAddress, 16)
        result += to_bytes(self.NumberOfTlvIds, 1)
        result += bytearray(self.TlvIds)
        return result


class MESHCOP_MgmtActiveGetRequest(object):

    def __init__(self, InstanceId=bytearray(1), IPAddress=bytearray(16), NumberOfTlvIds=bytearray(1), TlvIds=[]):
        '''
        @param InstanceId: Thread instance Id
        @param IPAddress: The IPv6 address
        @param NumberOfTlvIds: Number of TLV Ids
        @param TlvIds: TlvIds
        '''
        self.InstanceId = InstanceId
        self.IPAddress = IPAddress
        self.NumberOfTlvIds = NumberOfTlvIds
        self.TlvIds = TlvIds

    def pickle(self):
        result = bytearray()
        result += to_bytes(self.InstanceId, 1)
        result += to_bytes(self.IPAddress, 16)
        result += to_bytes(self.NumberOfTlvIds, 1)
        result += bytearray(self.TlvIds)
        return result


class MESHCOP_MgmtPendingGetRequest(object):

    def __init__(self, InstanceId=bytearray(1), IPAddress=bytearray(16), NumberOfTlvIds=bytearray(1), TlvIds=[]):
        '''
        @param InstanceId: Thread instance Id
        @param IPAddress: The IPv6 address
        @param NumberOfTlvIds: Number of TLV Ids
        @param TlvIds: TlvIds
        '''
        self.InstanceId = InstanceId
        self.IPAddress = IPAddress
        self.NumberOfTlvIds = NumberOfTlvIds
        self.TlvIds = TlvIds

    def pickle(self):
        result = bytearray()
        result += to_bytes(self.InstanceId, 1)
        result += to_bytes(self.IPAddress, 16)
        result += to_bytes(self.NumberOfTlvIds, 1)
        result += bytearray(self.TlvIds)
        return result


class MESHCOP_MgmtCommissionerSetRequest(object):

    class MLPrefix(object):

        def __init__(self, PrefixData=bytearray(16), PrefixLength=bytearray(1)):
            self.PrefixData = PrefixData
            self.PrefixLength = PrefixLength

        def pickle(self):
            result = bytearray()
            result += to_bytes(self.PrefixData, 16)
            result += to_bytes(self.PrefixLength, 1)
            return result

    def __init__(self, InstanceId=bytearray(1), IPAddress=bytearray(16), SessionIdEnable=bytearray(1), SessionId=[], BorderRouterLocatorEnable=bytearray(1), BorderRouterLocator=[], NewSessionIdEnable=bytearray(1), NewSessionId=[], SteeringDataEnable=bytearray(1), SteeringDataSize=[], SteeringData=[], ChannelEnable=bytearray(1), Channel=[], ChannelMaskEnable=bytearray(1), ChannelPage=[], ChannelMaskLength=[], ChannelMask=[], XpanIdEnable=bytearray(1), XpanId=[], MLPrefixEnable=bytearray(1), MLPrefix=MLPrefix(), MasterKeyEnable=bytearray(1), MasterKey=[], NwkNameEnable=bytearray(1), NwkNameSize=[], NwkName=[], PanIdEnable=bytearray(1), PanId=[], PSKcEnable=bytearray(1), PskcSize=[], PSKc=[], PolicyEnable=bytearray(1), RotationInterval=[], Policy=[], ActiveTimestampEnable=bytearray(1), ActiveSeconds=[], ActiveTicks=[], PendingTimestampEnable=bytearray(1), PendingSeconds=[], PendingTicks=[], DelayTimerEnable=bytearray(1), Timeout=[], FutureTlvEnable=bytearray(1), FutureTlvSize=bytearray(1), FutureTlvValue=[]):
        '''
        @param InstanceId: Thread instance Id
        @param IPAddress: IPAddress of the destination
        @param SessionIdEnable: SessionIdEnable
        @param SessionId: SessionId
        @param BorderRouterLocatorEnable: BorderRouterLocatorEnable
        @param BorderRouterLocator: Border Router Locator
        @param NewSessionIdEnable: NewSessionIdEnable
        @param NewSessionId: Send new session ID
        @param SteeringDataEnable: SteeringDataEnable
        @param SteeringDataSize: The size of the Steering Data
        @param SteeringData: SteeringData
        @param ChannelEnable: ChannelEnable
        @param Channel: Channel
        @param ChannelMaskEnable: ChannelMaskEnable
        @param ChannelPage: ChannelPage
        @param ChannelMaskLength: ChannelMaskLength
        @param ChannelMask: ChannelMask
        @param XpanIdEnable: XpanIdEnable
        @param XpanId: XpanId
        @param MLPrefixEnable: MLPrefixEnable
        @param MLPrefix: ML Prefix
        @param MasterKeyEnable: MasterKeyEnable
        @param MasterKey: MasterKey
        @param NwkNameEnable: NwkNameEnable
        @param NwkNameSize: Network Name Size
        @param NwkName: Network Name
        @param PanIdEnable: PanIdEnable
        @param PanId: PanId
        @param PSKcEnable: PSKcEnable
        @param PskcSize: PskcSize
        @param PSKc: PSKc
        @param PolicyEnable: PolicyEnable
        @param RotationInterval: RotationInterval
        @param Policy: Policy
        @param ActiveTimestampEnable: Will be applied after Delay Timer expires
        @param ActiveSeconds: Active timestamp seconds
        @param ActiveTicks: Active timestamp ticks
        @param PendingTimestampEnable: Used to compare multiple Pending Set requests
        @param PendingSeconds: Pending timestamp Seconds
        @param PendingTicks: Pending timestamp ticks
        @param DelayTimerEnable: DelayTimerEnable
        @param Timeout: Timeout(ms)
        @param FutureTlvEnable: Future Tlv Enable - for certification purposes
        @param FutureTlvSize: Future Tlv Size - for certification purposes
        @param FutureTlvValue: Future TLV Structure - for certification purposes
        '''
        self.InstanceId = InstanceId
        self.IPAddress = IPAddress
        self.SessionIdEnable = SessionIdEnable
        self.SessionId = SessionId
        self.BorderRouterLocatorEnable = BorderRouterLocatorEnable
        self.BorderRouterLocator = BorderRouterLocator
        self.NewSessionIdEnable = NewSessionIdEnable
        self.NewSessionId = NewSessionId
        self.SteeringDataEnable = SteeringDataEnable
        self.SteeringDataSize = SteeringDataSize
        self.SteeringData = SteeringData
        self.ChannelEnable = ChannelEnable
        self.Channel = Channel
        self.ChannelMaskEnable = ChannelMaskEnable
        self.ChannelPage = ChannelPage
        self.ChannelMaskLength = ChannelMaskLength
        self.ChannelMask = ChannelMask
        self.XpanIdEnable = XpanIdEnable
        self.XpanId = XpanId
        self.MLPrefixEnable = MLPrefixEnable
        self.MLPrefix = MLPrefix
        self.MasterKeyEnable = MasterKeyEnable
        self.MasterKey = MasterKey
        self.NwkNameEnable = NwkNameEnable
        self.NwkNameSize = NwkNameSize
        self.NwkName = NwkName
        self.PanIdEnable = PanIdEnable
        self.PanId = PanId
        self.PSKcEnable = PSKcEnable
        self.PskcSize = PskcSize
        self.PSKc = PSKc
        self.PolicyEnable = PolicyEnable
        self.RotationInterval = RotationInterval
        self.Policy = Policy
        self.ActiveTimestampEnable = ActiveTimestampEnable
        self.ActiveSeconds = ActiveSeconds
        self.ActiveTicks = ActiveTicks
        self.PendingTimestampEnable = PendingTimestampEnable
        self.PendingSeconds = PendingSeconds
        self.PendingTicks = PendingTicks
        self.DelayTimerEnable = DelayTimerEnable
        self.Timeout = Timeout
        self.FutureTlvEnable = FutureTlvEnable
        self.FutureTlvSize = FutureTlvSize
        self.FutureTlvValue = FutureTlvValue

    def pickle(self):
        result = bytearray()
        result += to_bytes(self.InstanceId, 1)
        result += to_bytes(self.IPAddress, 16)
        result += to_bytes(self.SessionIdEnable, 1)
        if self.SessionIdEnable:
            result += to_bytes(self.SessionId, 2)
        result += to_bytes(self.BorderRouterLocatorEnable, 1)
        if self.BorderRouterLocatorEnable:
            result += to_bytes(self.BorderRouterLocator, 2)
        result += to_bytes(self.NewSessionIdEnable, 1)
        if self.NewSessionIdEnable:
            result += to_bytes(self.NewSessionId, 2)
        result += to_bytes(self.SteeringDataEnable, 1)
        if self.SteeringDataEnable:
            result += to_bytes(self.SteeringDataSize, 1)
        for i in range(list_to_int(self.SteeringDataSize, False)):
            result += to_bytes(self.SteeringData[i], 1)
        result += to_bytes(self.ChannelEnable, 1)
        if self.ChannelEnable:
            result += to_bytes(self.Channel, 1)
        result += to_bytes(self.ChannelMaskEnable, 1)
        if self.ChannelMaskEnable:
            result += to_bytes(self.ChannelPage, 1)
        if self.ChannelMaskEnable:
            result += to_bytes(self.ChannelMaskLength, 1)
        result += to_bytes(self.ChannelMask, self.ChannelMaskLength, reverse=False)
        result += to_bytes(self.XpanIdEnable, 1)
        if self.XpanIdEnable:
            result += to_bytes(self.XpanId, 8)
        result += to_bytes(self.MLPrefixEnable, 1)
        if self.MLPrefixEnable:
            result += self.MLPrefix.pickle()
        result += to_bytes(self.MasterKeyEnable, 1)
        if self.MasterKeyEnable:
            result += to_bytes(self.MasterKey, 16)
        result += to_bytes(self.NwkNameEnable, 1)
        if self.NwkNameEnable:
            result += to_bytes(self.NwkNameSize, 1)
        result += to_bytes(self.NwkName, self.NwkNameSize)
        result += to_bytes(self.PanIdEnable, 1)
        if self.PanIdEnable:
            result += to_bytes(self.PanId, 2)
        result += to_bytes(self.PSKcEnable, 1)
        if self.PSKcEnable:
            result += to_bytes(self.PskcSize, 1)
        result += to_bytes(self.PSKc, self.PskcSize)
        result += to_bytes(self.PolicyEnable, 1)
        if self.PolicyEnable:
            result += to_bytes(self.RotationInterval, 2)
        if self.PolicyEnable:
            result += to_bytes(self.Policy, 1)
        result += to_bytes(self.ActiveTimestampEnable, 1)
        if self.ActiveTimestampEnable:
            result += to_bytes(self.ActiveSeconds, 6)
        if self.ActiveTimestampEnable:
            result += to_bytes(self.ActiveTicks, 2)
        result += to_bytes(self.PendingTimestampEnable, 1)
        if self.PendingTimestampEnable:
            result += to_bytes(self.PendingSeconds, 6)
        if self.PendingTimestampEnable:
            result += to_bytes(self.PendingTicks, 2)
        result += to_bytes(self.DelayTimerEnable, 1)
        if self.DelayTimerEnable:
            result += to_bytes(self.Timeout, 4)
        result += to_bytes(self.FutureTlvEnable, 1)
        if self.FutureTlvEnable:
            result += to_bytes(self.FutureTlvSize, 1)
            result += to_bytes(self.FutureTlvValue, self.FutureTlvSize)
        return result


class MESHCOP_MgmtActiveSetRequest(object):

    class MLPrefix(object):

        def __init__(self, PrefixData=bytearray(16), PrefixLength=bytearray(1)):
            self.PrefixData = PrefixData
            self.PrefixLength = PrefixLength

        def pickle(self):
            result = bytearray()
            result += to_bytes(self.PrefixData, 16)
            result += to_bytes(self.PrefixLength, 1)
            return result

    def __init__(self, InstanceId=bytearray(1), IPAddress=bytearray(16), SessionIdEnable=bytearray(1), SessionId=[], BorderRouterLocatorEnable=bytearray(1), BorderRouterLocator=[], NewSesswionIdEnable=bytearray(1), NewSesswionId=[], SteeringDataEnable=bytearray(1), SteeringDataSize=[], SteeringData=[], ChannelEnable=bytearray(1), Channel=[], ChannelMaskEnable=bytearray(1), ChannelPage=[], ChannelMaskLength=[], ChannelMask=[], XpanIdEnable=bytearray(1), XpanId=[], MLPrefixEnable=bytearray(1), MLPrefix=MLPrefix(), MasterKeyEnable=bytearray(1), MasterKey=[], NwkNameEnable=bytearray(1), NwkNameSize=[], NwkName=[], PanIdEnable=bytearray(1), PanId=[], PSKcEnable=bytearray(1), PskcSize=[], PSKc=[], PolicyEnable=bytearray(1), RotationInterval=[], Policy=[], ActiveTimestampEnable=bytearray(1), ActiveSeconds=[], ActiveTicks=[], PendingTimestampEnable=bytearray(1), PendingSeconds=[], PendingTicks=[], DelayTimerEnable=bytearray(1), Timeout=[], FutureTlvEnable=bytearray(1), FutureTlvSize=bytearray(1), FutureTlvValue=[]):
        '''
        @param InstanceId: Thread instance Id
        @param IPAddress: IPAddress of the destination
        @param SessionIdEnable: SessionIdEnable
        @param SessionId: SessionId
        @param BorderRouterLocatorEnable: BorderRouterLocatorEnable
        @param BorderRouterLocator: Border Router Locator
        @param NewSesswionIdEnable: NewSesswionIdEnable
        @param NewSesswionId: Send new session ID
        @param SteeringDataEnable: SteeringDataEnable
        @param SteeringDataSize: The size of the Steering Data
        @param SteeringData: SteeringData
        @param ChannelEnable: ChannelEnable
        @param Channel: Channel
        @param ChannelMaskEnable: ChannelMaskEnable
        @param ChannelPage: ChannelPage
        @param ChannelMaskLength: ChannelMaskLength
        @param ChannelMask: ChannelMask
        @param XpanIdEnable: XpanIdEnable
        @param XpanId: XpanId
        @param MLPrefixEnable: MLPrefixEnable
        @param MLPrefix: ML Prefix
        @param MasterKeyEnable: MasterKeyEnable
        @param MasterKey: MasterKey
        @param NwkNameEnable: NwkNameEnable
        @param NwkNameSize: Network Name Size
        @param NwkName: Network Name
        @param PanIdEnable: PanIdEnable
        @param PanId: PanId
        @param PSKcEnable: PSKcEnable
        @param PskcSize: PskcSize
        @param PSKc: PSKc
        @param PolicyEnable: PolicyEnable
        @param RotationInterval: RotationInterval
        @param Policy: Policy
        @param ActiveTimestampEnable: Will be applied after Delay Timer expires
        @param ActiveSeconds: Active timestamp seconds
        @param ActiveTicks: Active timestamp ticks
        @param PendingTimestampEnable: Used to compare multiple Pending Set requests
        @param PendingSeconds: Pending timestamp Seconds
        @param PendingTicks: Pending timestamp ticks
        @param DelayTimerEnable: DelayTimerEnable
        @param Timeout: Timeout(ms)
        @param FutureTlvEnable: Future Tlv Enable - for certification purposes
        @param FutureTlvSize: Future Tlv Size - for certification purposes
        @param FutureTlvValue: Future TLV Structure - for certification purposes
        '''
        self.InstanceId = InstanceId
        self.IPAddress = IPAddress
        self.SessionIdEnable = SessionIdEnable
        self.SessionId = SessionId
        self.BorderRouterLocatorEnable = BorderRouterLocatorEnable
        self.BorderRouterLocator = BorderRouterLocator
        self.NewSesswionIdEnable = NewSesswionIdEnable
        self.NewSesswionId = NewSesswionId
        self.SteeringDataEnable = SteeringDataEnable
        self.SteeringDataSize = SteeringDataSize
        self.SteeringData = SteeringData
        self.ChannelEnable = ChannelEnable
        self.Channel = Channel
        self.ChannelMaskEnable = ChannelMaskEnable
        self.ChannelPage = ChannelPage
        self.ChannelMaskLength = ChannelMaskLength
        self.ChannelMask = ChannelMask
        self.XpanIdEnable = XpanIdEnable
        self.XpanId = XpanId
        self.MLPrefixEnable = MLPrefixEnable
        self.MLPrefix = MLPrefix
        self.MasterKeyEnable = MasterKeyEnable
        self.MasterKey = MasterKey
        self.NwkNameEnable = NwkNameEnable
        self.NwkNameSize = NwkNameSize
        self.NwkName = NwkName
        self.PanIdEnable = PanIdEnable
        self.PanId = PanId
        self.PSKcEnable = PSKcEnable
        self.PskcSize = PskcSize
        self.PSKc = PSKc
        self.PolicyEnable = PolicyEnable
        self.RotationInterval = RotationInterval
        self.Policy = Policy
        self.ActiveTimestampEnable = ActiveTimestampEnable
        self.ActiveSeconds = ActiveSeconds
        self.ActiveTicks = ActiveTicks
        self.PendingTimestampEnable = PendingTimestampEnable
        self.PendingSeconds = PendingSeconds
        self.PendingTicks = PendingTicks
        self.DelayTimerEnable = DelayTimerEnable
        self.Timeout = Timeout
        self.FutureTlvEnable = FutureTlvEnable
        self.FutureTlvSize = FutureTlvSize
        self.FutureTlvValue = FutureTlvValue

    def pickle(self):
        result = bytearray()
        result += to_bytes(self.InstanceId, 1)
        result += to_bytes(self.IPAddress, 16)
        result += to_bytes(self.SessionIdEnable, 1)
        if self.SessionIdEnable:
            result += to_bytes(self.SessionId, 2)
        result += to_bytes(self.BorderRouterLocatorEnable, 1)
        if self.BorderRouterLocatorEnable:
            result += to_bytes(self.BorderRouterLocator, 2)
        result += to_bytes(self.NewSesswionIdEnable, 1)
        if self.NewSesswionIdEnable:
            result += to_bytes(self.NewSesswionId, 2)
        result += to_bytes(self.SteeringDataEnable, 1)
        if self.SteeringDataEnable:
            result += to_bytes(self.SteeringDataSize, 1)
        result += to_bytes(self.SteeringData, self.SteeringDataSize)
        result += to_bytes(self.ChannelEnable, 1)
        if self.ChannelEnable:
            result += to_bytes(self.Channel, 1)
        result += to_bytes(self.ChannelMaskEnable, 1)
        if self.ChannelMaskEnable:
            result += to_bytes(self.ChannelPage, 1)
        if self.ChannelMaskEnable:
            result += to_bytes(self.ChannelMaskLength, 1)
        result += to_bytes(self.ChannelMask, self.ChannelMaskLength, reverse=False)
        result += to_bytes(self.XpanIdEnable, 1)
        if self.XpanIdEnable:
            result += to_bytes(self.XpanId, 8)
        result += to_bytes(self.MLPrefixEnable, 1)
        if self.MLPrefixEnable:
            result += self.MLPrefix.pickle()
        result += to_bytes(self.MasterKeyEnable, 1)
        if self.MasterKeyEnable:
            result += to_bytes(self.MasterKey, 16)
        result += to_bytes(self.NwkNameEnable, 1)
        if self.NwkNameEnable:
            result += to_bytes(self.NwkNameSize, 1)
        result += to_bytes(self.NwkName, self.NwkNameSize)
        result += to_bytes(self.PanIdEnable, 1)
        if self.PanIdEnable:
            result += to_bytes(self.PanId, 2)
        result += to_bytes(self.PSKcEnable, 1)
        if self.PSKcEnable:
            result += to_bytes(self.PskcSize, 1)
        result += to_bytes(self.PSKc, self.PskcSize)
        result += to_bytes(self.PolicyEnable, 1)
        if self.PolicyEnable:
            result += to_bytes(self.RotationInterval, 2)
        if self.PolicyEnable:
            result += to_bytes(self.Policy, 1)
        result += to_bytes(self.ActiveTimestampEnable, 1)
        if self.ActiveTimestampEnable:
            result += to_bytes(self.ActiveSeconds, 6)
        if self.ActiveTimestampEnable:
            result += to_bytes(self.ActiveTicks, 2)
        result += to_bytes(self.PendingTimestampEnable, 1)
        if self.PendingTimestampEnable:
            result += to_bytes(self.PendingSeconds, 6)
        if self.PendingTimestampEnable:
            result += to_bytes(self.PendingTicks, 2)
        result += to_bytes(self.DelayTimerEnable, 1)
        if self.DelayTimerEnable:
            result += to_bytes(self.Timeout, 4)
        result += to_bytes(self.FutureTlvEnable, 1)
        if self.FutureTlvEnable:
            result += to_bytes(self.FutureTlvSize, 1)
            result += to_bytes(self.FutureTlvValue, self.FutureTlvSize)
        return result


class MESHCOP_MgmtPendingSetRequest(object):

    class MLPrefix(object):

        def __init__(self, PrefixData=bytearray(16), PrefixLength=bytearray(1)):
            self.PrefixData = PrefixData
            self.PrefixLength = PrefixLength

        def pickle(self):
            result = bytearray()
            result += to_bytes(self.PrefixData, 16)
            result += to_bytes(self.PrefixLength, 1)
            return result

    def __init__(self, InstanceId=bytearray(1), IPAddress=bytearray(16), SessionIdEnable=bytearray(1), SessionId=[], BorderRouterLocatorEnable=bytearray(1), BorderRouterLocator=[], NewSesswionIdEnable=bytearray(1), NewSesswionId=[], SteeringDataEnable=bytearray(1), SteeringDataSize=[], SteeringData=[], ChannelEnable=bytearray(1), Channel=[], ChannelMaskEnable=bytearray(1), ChannelPage=[], ChannelMaskLength=[], ChannelMask=[], XpanIdEnable=bytearray(1), XpanId=[], MLPrefixEnable=bytearray(1), MLPrefix=MLPrefix(), MasterKeyEnable=bytearray(1), MasterKey=[], NwkNameEnable=bytearray(1), NwkNameSize=[], NwkName=[], PanIdEnable=bytearray(1), PanId=[], PSKcEnable=bytearray(1), PskcSize=[], PSKc=[], PolicyEnable=bytearray(1), RotationInterval=[], Policy=[], ActiveTimestampEnable=bytearray(1), ActiveSeconds=[], ActiveTicks=[], PendingTimestampEnable=bytearray(1), PendingSeconds=[], PendingTicks=[], DelayTimerEnable=bytearray(1), Timeout=[], FutureTlvEnable=bytearray(1), FutureTlvSize=bytearray(1), FutureTlvValue=[]):
        '''
        @param InstanceId: Thread instance Id
        @param IPAddress: IPAddress of the destination
        @param SessionIdEnable: SessionIdEnable
        @param SessionId: SessionId
        @param BorderRouterLocatorEnable: BorderRouterLocatorEnable
        @param BorderRouterLocator: Border Router Locator
        @param NewSesswionIdEnable: NewSesswionIdEnable
        @param NewSesswionId: Send new session ID
        @param SteeringDataEnable: SteeringDataEnable
        @param SteeringDataSize: The size of the Steering Data
        @param SteeringData: SteeringData
        @param ChannelEnable: ChannelEnable
        @param Channel: Channel
        @param ChannelMaskEnable: ChannelMaskEnable
        @param ChannelPage: ChannelPage
        @param ChannelMaskLength: ChannelMaskLength
        @param ChannelMask: ChannelMask
        @param XpanIdEnable: XpanIdEnable
        @param XpanId: XpanId
        @param MLPrefixEnable: MLPrefixEnable
        @param MLPrefix: ML Prefix
        @param MasterKeyEnable: MasterKeyEnable
        @param MasterKey: MasterKey
        @param NwkNameEnable: NwkNameEnable
        @param NwkNameSize: Network Name Size
        @param NwkName: Network Name
        @param PanIdEnable: PanIdEnable
        @param PanId: PanId
        @param PSKcEnable: PSKcEnable
        @param PskcSize: PskcSize
        @param PSKc: PSKc
        @param PolicyEnable: PolicyEnable
        @param RotationInterval: RotationInterval
        @param Policy: Policy
        @param ActiveTimestampEnable: Will be applied after Delay Timer expires
        @param ActiveSeconds: Active timestamp seconds
        @param ActiveTicks: Active timestamp ticks
        @param PendingTimestampEnable: Used to compare multiple Pending Set requests
        @param PendingSeconds: Pending timestamp Seconds
        @param PendingTicks: Pending timestamp ticks
        @param DelayTimerEnable: DelayTimerEnable
        @param Timeout: Timeout(ms)
        @param FutureTlvEnable: Future Tlv Enable - for certification purposes
        @param FutureTlvSize: Future Tlv Size - for certification purposes
        @param FutureTlvValue: Future TLV Structure - for certification purposes
        '''
        self.InstanceId = InstanceId
        self.IPAddress = IPAddress
        self.SessionIdEnable = SessionIdEnable
        self.SessionId = SessionId
        self.BorderRouterLocatorEnable = BorderRouterLocatorEnable
        self.BorderRouterLocator = BorderRouterLocator
        self.NewSesswionIdEnable = NewSesswionIdEnable
        self.NewSesswionId = NewSesswionId
        self.SteeringDataEnable = SteeringDataEnable
        self.SteeringDataSize = SteeringDataSize
        self.SteeringData = SteeringData
        self.ChannelEnable = ChannelEnable
        self.Channel = Channel
        self.ChannelMaskEnable = ChannelMaskEnable
        self.ChannelPage = ChannelPage
        self.ChannelMaskLength = ChannelMaskLength
        self.ChannelMask = ChannelMask
        self.XpanIdEnable = XpanIdEnable
        self.XpanId = XpanId
        self.MLPrefixEnable = MLPrefixEnable
        self.MLPrefix = MLPrefix
        self.MasterKeyEnable = MasterKeyEnable
        self.MasterKey = MasterKey
        self.NwkNameEnable = NwkNameEnable
        self.NwkNameSize = NwkNameSize
        self.NwkName = NwkName
        self.PanIdEnable = PanIdEnable
        self.PanId = PanId
        self.PSKcEnable = PSKcEnable
        self.PskcSize = PskcSize
        self.PSKc = PSKc
        self.PolicyEnable = PolicyEnable
        self.RotationInterval = RotationInterval
        self.Policy = Policy
        self.ActiveTimestampEnable = ActiveTimestampEnable
        self.ActiveSeconds = ActiveSeconds
        self.ActiveTicks = ActiveTicks
        self.PendingTimestampEnable = PendingTimestampEnable
        self.PendingSeconds = PendingSeconds
        self.PendingTicks = PendingTicks
        self.DelayTimerEnable = DelayTimerEnable
        self.Timeout = Timeout
        self.FutureTlvEnable = FutureTlvEnable
        self.FutureTlvSize = FutureTlvSize
        self.FutureTlvValue = FutureTlvValue

    def pickle(self):
        result = bytearray()
        result += to_bytes(self.InstanceId, 1)
        result += to_bytes(self.IPAddress, 16)
        result += to_bytes(self.SessionIdEnable, 1)
        if self.SessionIdEnable:
            result += to_bytes(self.SessionId, 2)
        result += to_bytes(self.BorderRouterLocatorEnable, 1)
        if self.BorderRouterLocatorEnable:
            result += to_bytes(self.BorderRouterLocator, 2)
        result += to_bytes(self.NewSesswionIdEnable, 1)
        if self.NewSesswionIdEnable:
            result += to_bytes(self.NewSesswionId, 2)
        result += to_bytes(self.SteeringDataEnable, 1)
        if self.SteeringDataEnable:
            result += to_bytes(self.SteeringDataSize, 1)
        result += to_bytes(self.SteeringData, self.SteeringDataSize)
        result += to_bytes(self.ChannelEnable, 1)
        if self.ChannelEnable:
            result += to_bytes(self.Channel, 1)
        result += to_bytes(self.ChannelMaskEnable, 1)
        if self.ChannelMaskEnable:
            result += to_bytes(self.ChannelPage, 1)
        if self.ChannelMaskEnable:
            result += to_bytes(self.ChannelMaskLength, 1)
        result += to_bytes(self.ChannelMask, self.ChannelMaskLength, reverse=False)
        result += to_bytes(self.XpanIdEnable, 1)
        if self.XpanIdEnable:
            result += to_bytes(self.XpanId, 8)
        result += to_bytes(self.MLPrefixEnable, 1)
        if self.MLPrefixEnable:
            result += self.MLPrefix.pickle()
        result += to_bytes(self.MasterKeyEnable, 1)
        if self.MasterKeyEnable:
            result += to_bytes(self.MasterKey, 16)
        result += to_bytes(self.NwkNameEnable, 1)
        if self.NwkNameEnable:
            result += to_bytes(self.NwkNameSize, 1)
        result += to_bytes(self.NwkName, self.NwkNameSize)
        result += to_bytes(self.PanIdEnable, 1)
        if self.PanIdEnable:
            result += to_bytes(self.PanId, 2)
        result += to_bytes(self.PSKcEnable, 1)
        if self.PSKcEnable:
            result += to_bytes(self.PskcSize, 1)
        result += to_bytes(self.PSKc, self.PskcSize)
        result += to_bytes(self.PolicyEnable, 1)
        if self.PolicyEnable:
            result += to_bytes(self.RotationInterval, 2)
        if self.PolicyEnable:
            result += to_bytes(self.Policy, 1)
        result += to_bytes(self.ActiveTimestampEnable, 1)
        if self.ActiveTimestampEnable:
            result += to_bytes(self.ActiveSeconds, 6)
        if self.ActiveTimestampEnable:
            result += to_bytes(self.ActiveTicks, 2)
        result += to_bytes(self.PendingTimestampEnable, 1)
        if self.PendingTimestampEnable:
            result += to_bytes(self.PendingSeconds, 6)
        if self.PendingTimestampEnable:
            result += to_bytes(self.PendingTicks, 2)
        result += to_bytes(self.DelayTimerEnable, 1)
        if self.DelayTimerEnable:
            result += to_bytes(self.Timeout, 4)
        result += to_bytes(self.FutureTlvEnable, 1)
        if self.FutureTlvEnable:
            result += to_bytes(self.FutureTlvSize, 1)
            result += to_bytes(self.FutureTlvValue, self.FutureTlvSize)
        return result


class MESHCOP_MgmtSendPanIdQueryRequest(object):

    def __init__(self, ThrInstanceID=bytearray(1), ScanChannelMask=bytearray(4), PanId=bytearray(2), IPAddress=bytearray(16)):
        '''
        @param ThrInstanceID: Thread instance ID
        @param ScanChannelMask: Channel mask
        @param PanId: PanId
        @param IPAddress: The IPv6 address of the device added to network
        '''
        self.ThrInstanceID = ThrInstanceID
        self.ScanChannelMask = ScanChannelMask
        self.PanId = PanId
        self.IPAddress = IPAddress


class MESHCOP_MgmtSendEdScanRequest(object):

    def __init__(self, ThrInstanceID=bytearray(1), ScanChannelMask=bytearray(4), Count=bytearray(1), Period=bytearray(2), ScanDuration=bytearray(2), IPAddress=bytearray(16)):
        '''
        @param ThrInstanceID: Thread instance ID
        @param ScanChannelMask: Channel mask
        @param Count: The number of IEEE 802.15.4 ED Scans per channel
        @param Period: The period between successive IEEE 802.15.4 ED Scans
        @param ScanDuration: The IEEE 802.15.4 ScanDuration to use when performing an IEEE 802.15.4 ED Scan
        @param IPAddress: The IPv6 address of the device added to network
        '''
        self.ThrInstanceID = ThrInstanceID
        self.ScanChannelMask = ScanChannelMask
        self.Count = Count
        self.Period = Period
        self.ScanDuration = ScanDuration
        self.IPAddress = IPAddress


class MESHCOP_MgmtSendAnnounceBeginRequest(object):

    def __init__(self, ThrInstanceID=bytearray(1), CommissionerSessionID=bytearray(2), ChannelMask=bytearray(4), Count=bytearray(1), Period=bytearray(2), IPAddress=bytearray(16)):
        '''
        @param ThrInstanceID: Thread instance ID
        @param CommissionerSessionID: Commissioner Session ID. Set to 0 to use the current value on the device.
        @param ChannelMask: Channel mask
        @param Count: The number of MGMT_ANNOUNCE.ntf that the destination device should send.
        @param Period: The period between successive MGMT_ANNOUNCE.ntf frames
        @param IPAddress: The IPv6 address of the destination. Unicast or multicast.
        '''
        self.ThrInstanceID = ThrInstanceID
        self.CommissionerSessionID = CommissionerSessionID
        self.ChannelMask = ChannelMask
        self.Count = Count
        self.Period = Period
        self.IPAddress = IPAddress


class NWKU_IfconfigBindRequest(object):

    def __init__(self, IpAddress=bytearray(16), InterfaceId=bytearray(1)):
        '''
        @param IpAddress: Ip Address
        @param InterfaceId: Interface Id
        '''
        self.IpAddress = IpAddress
        self.InterfaceId = InterfaceId


class NWKU_IfconfigAllRequest(object):

    pass


class NWKU_PingRequest(object):

    def __init__(self, DestinationIpAddress=bytearray(16), SourceIpAddress=bytearray(16), Payloadlength=bytearray(2), Timeout=bytearray(2), Secured=bytearray(1)):
        '''
        @param DestinationIpAddress: Destination Ip Address
        @param SourceIpAddress: Source Ip Address
        @param Payloadlength: Payload Length
        @param Timeout: Timeout
        @param Secured: Set security for ping packets
        '''
        self.DestinationIpAddress = DestinationIpAddress
        self.SourceIpAddress = SourceIpAddress
        self.Payloadlength = Payloadlength
        self.Timeout = Timeout
        self.Secured = Secured


class NWKU_EchoUDPRequest(object):

    def __init__(self, DestinationIpAddress=bytearray(16), SourceIpAddress=bytearray(16), Payloadlength=bytearray(2), Timeout=bytearray(2), Iterations=bytearray(1)):
        '''
        @param DestinationIpAddress: Destination Ip Address
        @param SourceIpAddress: Source Ip Address
        @param Payloadlength: Payload Length
        @param Timeout: Timeout
        @param Iterations: Number of iterations
        '''
        self.DestinationIpAddress = DestinationIpAddress
        self.SourceIpAddress = SourceIpAddress
        self.Payloadlength = Payloadlength
        self.Timeout = Timeout
        self.Iterations = Iterations


class NWKU_CPUResetRequest(object):

    pass


class NWKU_CoapCreateInstanceRequest(object):

    def __init__(self, UDPPort=bytearray(2), SocketDomain=NWKU_CoapCreateInstanceRequestSocketDomain()):
        '''
        @param UDPPort: UDP Port
        @param SocketDomain: Socket Domain
        '''
        self.UDPPort = UDPPort
        self.SocketDomain = SocketDomain


class NWKU_CoapSendRequest(object):

    def __init__(self, InstanceID=bytearray(1), DestinationIpAddress=bytearray(16), UDPPort=bytearray(2), RequestType=NWKU_CoapSendRequestRequestType.CON, MessageType=NWKU_CoapSendRequestMessageType.GET, URIpath=bytearray(30), PayloadLength=bytearray(1), Payload=[]):
        '''
        @param InstanceID: InstanceID
        @param DestinationIpAddress: Destination Ip Address
        @param UDPPort: UDPPort
        @param RequestType: CoAP request type
        @param MessageType: CoAP message type
        @param URIpath: URI-path options separated by /
        @param PayloadLength: CoAP message payload length
        @param Payload: CoAP message payload
        '''
        self.InstanceID = InstanceID
        self.DestinationIpAddress = DestinationIpAddress
        self.UDPPort = UDPPort
        self.RequestType = RequestType
        self.MessageType = MessageType
        self.URIpath = URIpath
        self.PayloadLength = PayloadLength
        self.Payload = Payload


class NWKU_CoapRegisterRequest(object):

    def __init__(self, InstanceID=bytearray(1), SocketDomain=NWKU_CoapRegisterRequestSocketDomain(), URIpath=bytearray(30), Port=bytearray(2)):
        '''
        @param InstanceID: InstanceID
        @param SocketDomain: Socket Domain
        @param URIpath: URI-path options separated by /
        @param Port: UDP Port
        '''
        self.InstanceID = InstanceID
        self.SocketDomain = SocketDomain
        self.URIpath = URIpath
        self.Port = Port


class NWKU_DnsSendRequestRequest(object):

    def __init__(self, DomainName=bytearray(30)):
        '''
        @param DomainName: Domain Name
        '''
        self.DomainName = DomainName


class NWKU_McastGroupShowRequest(object):

    def __init__(self, InterfaceId=bytearray(1)):
        '''
        @param InterfaceId: Interface Id
        '''
        self.InterfaceId = InterfaceId


class NWKU_McastGroupManageRequest(object):

    def __init__(self, InterfaceId=bytearray(1), Action=NWKU_McastGroupManageRequestAction.JoinGroup, McastAddress=bytearray(16)):
        '''
        @param InterfaceId: Interface Id
        @param Action: Join or leave a multicast group
        @param McastAddress: Multicast group address
        '''
        self.InterfaceId = InterfaceId
        self.Action = Action
        self.McastAddress = McastAddress


class NWKU_RoutesShowRequest(object):

    pass


class NWKU_RoutesManageRequest(object):

    class Add(object):

        def __init__(self, PrefixLength=bytearray(1), RouteMetric=bytearray(1), InstanceId=bytearray(1)):
            self.PrefixLength = PrefixLength
            self.RouteMetric = RouteMetric
            self.InstanceId = InstanceId

    def __init__(self, Action=NWKU_RoutesManageRequestAction.Add, Destination=bytearray(16), NextHop=bytearray(16), RouteInfo=[]):
        '''
        @param Action: Adds or Removes an IPv6 route.
        @param Destination: Destination IPv6 address/prefix
        @param NextHop: Next hop IPv6 address
        @param RouteInfo: Route Info
        '''
        self.Action = Action
        self.Destination = Destination
        self.NextHop = NextHop
        self.RouteInfo = RouteInfo


class DTLSOpenRequest(object):

    def __init__(self, Maximumretransmissionscount=bytearray(1), Timeout=bytearray(2), LocalPort=bytearray(2)):
        '''
        @param Maximumretransmissionscount: Maximum Retransmissions Count
        @param Timeout: Timeout
        @param LocalPort: The local port.
        '''
        self.Maximumretransmissionscount = Maximumretransmissionscount
        self.Timeout = Timeout
        self.LocalPort = LocalPort


class DTLSCloseRequest(object):

    def __init__(self, ContextNumber=bytearray(1)):
        '''
        @param ContextNumber: Context Number
        '''
        self.ContextNumber = ContextNumber


class DTLSClosePeerRequest(object):

    def __init__(self, PeerNumber=bytearray(1)):
        '''
        @param PeerNumber: Peer Number
        '''
        self.PeerNumber = PeerNumber


class DTLSConnectRequest(object):

    def __init__(self, ContextNumber=bytearray(1), ServerIPAddress=bytearray(16), ServerPort=bytearray(2)):
        '''
        @param ContextNumber: Context Number
        @param ServerIPAddress: Server IP Address
        @param ServerPort: Server Port
        '''
        self.ContextNumber = ContextNumber
        self.ServerIPAddress = ServerIPAddress
        self.ServerPort = ServerPort


class DTLSSendRequest(object):

    def __init__(self, PeerNumber=bytearray(1), Size=bytearray(2), Data=[]):
        '''
        @param PeerNumber: Peer Number
        @param Size: The number of payload bytes
        @param Data: Data
        '''
        self.PeerNumber = PeerNumber
        self.Size = Size
        self.Data = Data


class FSCIGetUniqueIdRequest(object):

    pass


class FSCIGetMcuIdRequest(object):

    pass


class FSCIGetSwVersionsRequest(object):

    pass


class SerialTun_LinkIndicationRequest(object):

    def __init__(self, State=SerialTun_LinkIndicationRequestState.Down):
        '''
        @param State: State of the TUN/TAP link
        '''
        self.State = State


class SerialTun_IPPacketSendRequest(object):

    def __init__(self, Size=bytearray(2), Data=[]):
        '''
        @param Size: The number of payload bytes
        @param Data: Data
        '''
        self.Size = Size
        self.Data = Data


class Sniffer_MacSetPIBAttributeRequest(object):

    def __init__(self, PIBAttribute=Sniffer_MacSetPIBAttributeRequestPIBAttribute(), Value=bytearray(7)):
        '''
        @param PIBAttribute: The MAC PIB attribute identifier
        @param Value: The value to set the attribute to
        '''
        self.PIBAttribute = PIBAttribute
        self.Value = Value


class FSCIACK(object):

    def __init__(self, Status=FSCIACKStatus.OK):
        '''
        @param Status: Status
        '''
        self.Status = Status


class Sniffer_DetectRequest(object):

    pass


class FSCIEnterBootloaderRequest(object):

    pass


class AspSetPowerLevelRequest(object):

    def __init__(self, powerLevel=bytearray(1)):
        '''
        @param powerLevel: Power Level
        '''
        self.powerLevel = powerLevel


class AspGetPowerLevelRequest(object):

    pass


class SocketCreateConfirm(object):

    def __init__(self, SocketIndex=bytearray(1)):
        '''
        @param SocketIndex: The socket index.
        '''
        self.SocketIndex = SocketIndex


class SocketShutdownConfirm(object):

    def __init__(self, Status=SocketShutdownConfirmStatus.OK):
        '''
        @param Status: Status
        '''
        self.Status = Status


class SocketBindConfirm(object):

    def __init__(self, Status=SocketBindConfirmStatus.OK):
        '''
        @param Status: Status
        '''
        self.Status = Status


class SocketSendConfirm(object):

    def __init__(self, Status=SocketSendConfirmStatus.OK):
        '''
        @param Status: Status
        '''
        self.Status = Status


class SocketSendToConfirm(object):

    def __init__(self, Status=SocketSendToConfirmStatus.OK):
        '''
        @param Status: Status
        '''
        self.Status = Status


class SocketReceiveConfirm(object):

    def __init__(self, Size=bytearray(2), Data=[]):
        '''
        @param Size: The number of payload bytes
        @param Data: Data
        '''
        self.Size = Size
        self.Data = Data


class SocketReceiveFromConfirm(object):

    def __init__(self, Status=SocketReceiveFromConfirmStatus.OK, RemoteIpAddress=bytearray(16), RemotePort=bytearray(2), Size=bytearray(2), Data=[]):
        '''
        @param Status: Status
        @param RemoteIpAddress: The remote IP address.
        @param RemotePort: The remote port.
        @param Size: The number of payload bytes
        @param Data: Data
        '''
        self.Status = Status
        self.RemoteIpAddress = RemoteIpAddress
        self.RemotePort = RemotePort
        self.Size = Size
        self.Data = Data


class SocketConnectConfirm(object):

    def __init__(self, Status=SocketConnectConfirmStatus.OK):
        '''
        @param Status: Status
        '''
        self.Status = Status


class SocketListenConfirm(object):

    def __init__(self, Status=SocketListenConfirmStatus.OK):
        '''
        @param Status: Status
        '''
        self.Status = Status


class SocketAcceptConfirm(object):

    def __init__(self, Status=SocketAcceptConfirmStatus.OK, ConnectedSocketIndex=bytearray(1)):
        '''
        @param Status: Status
        @param ConnectedSocketIndex: The connected socket index.
        '''
        self.Status = Status
        self.ConnectedSocketIndex = ConnectedSocketIndex


class SocketSetOptionConfirm(object):

    def __init__(self, Status=SocketSetOptionConfirmStatus.OK):
        '''
        @param Status: Status
        '''
        self.Status = Status


class SocketGetOptionConfirm(object):

    def __init__(self, Status=SocketGetOptionConfirmStatus.OK, OptionValue=bytearray(4)):
        '''
        @param Status: Status
        @param OptionValue: Option Value
        '''
        self.Status = Status
        self.OptionValue = OptionValue


class MAC_MacFilteringAddEntryConfirm(object):

    def __init__(self, Status=MAC_MacFilteringAddEntryConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class MAC_MacFilteringRemoveEntryConfirm(object):

    def __init__(self, Status=MAC_MacFilteringRemoveEntryConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class MAC_MacFilteringEnableConfirm(object):

    def __init__(self, Status=MAC_MacFilteringEnableConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class MAC_MacFilteringGetTableConfirm(object):

    class MacFilteringEntry(object):

        def __init__(self, ExtendedAddress=bytearray(8), ShortAddress=bytearray(2), LinkIndicator=bytearray(1), BlockedNeighbor=bytearray(1)):
            self.ExtendedAddress = ExtendedAddress
            self.ShortAddress = ShortAddress
            self.LinkIndicator = LinkIndicator
            self.BlockedNeighbor = BlockedNeighbor

    def __init__(self, InstanceId=bytearray(1), NoOfElements=bytearray(1), MacFilteringEntries=[]):
        '''
        @param InstanceId: Instance Id
        @param NoOfElements: No Of Elements
        @param MacFilteringEntries: Mac Filtering Entries
        '''
        self.InstanceId = InstanceId
        self.NoOfElements = NoOfElements
        self.MacFilteringEntries = MacFilteringEntries


class THR_SetDeviceConfigConfirm(object):

    def __init__(self, Status=THR_SetDeviceConfigConfirmStatus.Success):
        '''
        @param Status: Attribute Status
        '''
        self.Status = Status


class THR_NwkScanConfirm(object):

    def __init__(self, Status=THR_NwkScanConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class THR_CreateNwkConfirm(object):

    def __init__(self, Status=THR_CreateNwkConfirmStatus.OK):
        '''
        @param Status: Status
        '''
        self.Status = Status


class THR_JoinConfirm(object):

    def __init__(self, Status=THR_JoinConfirmStatus.OK):
        '''
        @param Status: Status
        '''
        self.Status = Status


class THR_FactoryResetConfirm(object):

    def __init__(self, Status=THR_FactoryResetConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class THR_CpuResetConfirm(object):

    def __init__(self, Status=THR_CpuResetConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class THR_CpuResetIndication(object):

    class ResetCpuSuccess(object):

        def __init__(self, BoardNameLen=bytearray(1), BoardName=[], UniqueMcuId=bytearray(16), StackVersion=bytearray(6), SwVersionLen=bytearray(1), SwVersion=[]):
            self.BoardNameLen = BoardNameLen
            # Array length depends on BoardNameLen.
            self.BoardName = BoardName
            # Unit length: 16 bytes
            self.UniqueMcuId = UniqueMcuId
            # Unit length: 6 bytes
            self.StackVersion = StackVersion
            self.SwVersionLen = SwVersionLen
            # Array length depends on SwVersionLen.
            self.SwVersion = SwVersion

    class ResetCpuPending(object):

        def __init__(self, TimeoutMs=bytearray(4)):
            self.TimeoutMs = TimeoutMs

    def __init__(self, Status=THR_CpuResetIndicationStatus.ResetCpuSuccess, ResetCpuPayload=[]):
        '''
        @param Status: Status
        @param ResetCpuPayload: Reset CPU Payload
        '''
        self.Status = Status
        self.ResetCpuPayload = ResetCpuPayload


class THR_DisconnectConfirm(object):

    def __init__(self, Status=THR_DisconnectConfirmStatus.OK):
        '''
        @param Status: Status
        '''
        self.Status = Status


class THR_AttachConfirm(object):

    def __init__(self, Status=THR_AttachConfirmStatus.OK):
        '''
        @param Status: Status
        '''
        self.Status = Status


class THR_PromoteAsRouterConfirm(object):

    def __init__(self, Status=THR_PromoteAsRouterConfirmStatus.OK):
        '''
        @param Status: Status
        '''
        self.Status = Status


class THR_EventNwkScanConfirm(object):

    class EnergyDetect(object):

        def __init__(self, EnergyDetectEntries=bytearray(1), EnergyDetectList=[]):
            self.EnergyDetectEntries = EnergyDetectEntries
            # Array length depends on EnergyDetectEntries.
            self.EnergyDetectList = EnergyDetectList

    class ActiveScan(object):

        class NwkDiscoveryList(object):

            def __init__(self, NumOfRcvdBeacons=bytearray(2), PanId=bytearray(2), Channel=bytearray(1), Reserved=bytearray(1)):
                self.NumOfRcvdBeacons = NumOfRcvdBeacons
                self.PanId = PanId
                self.Channel = Channel
                self.Reserved = Reserved

        def __init__(self, NwkDiscoveryEntries=bytearray(1), NwkDiscoveryList=[]):
            self.NwkDiscoveryEntries = NwkDiscoveryEntries
            # Array length depends on NwkDiscoveryEntries.
            self.NwkDiscoveryList = NwkDiscoveryList

    class EnergyDetectAndActiveScan(object):

        class NwkDiscoveryList(object):

            def __init__(self, NumOfRcvdBeacons=bytearray(2), PanId=bytearray(2), Channel=bytearray(1), Reserved=bytearray(1)):
                self.NumOfRcvdBeacons = NumOfRcvdBeacons
                self.PanId = PanId
                self.Channel = Channel
                self.Reserved = Reserved

        def __init__(self, EnergyDetectEntries=bytearray(1), EnergyDetectList=[], NwkDiscoveryEntries=bytearray(1), NwkDiscoveryList=[]):
            self.EnergyDetectEntries = EnergyDetectEntries
            # Array length depends on EnergyDetectEntries.
            self.EnergyDetectList = EnergyDetectList
            self.NwkDiscoveryEntries = NwkDiscoveryEntries
            # Array length depends on NwkDiscoveryEntries.
            self.NwkDiscoveryList = NwkDiscoveryList

    def __init__(self, InstanceId=bytearray(1), EventStatus=THR_EventNwkScanConfirmEventStatus.ScanResult, DataSize=bytearray(2), ScanChannelMask=bytearray(4), ScanType=THR_EventNwkScanConfirmScanType.EnergyDetect, ScanDuration=bytearray(1), maxThrNwkToDiscover=bytearray(2), Data=[]):
        '''
        @param InstanceId: Thread Instance Id
        @param EventStatus: Event status
        @param DataSize: The number of payload bytes
        @param ScanChannelMask: ScanChannelMask
        @param ScanType: ScanType
        @param ScanDuration: ScanDuration
        @param maxThrNwkToDiscover: maxThrNwkToDiscover
        @param Data: Data
        '''
        self.InstanceId = InstanceId
        self.EventStatus = EventStatus
        self.DataSize = DataSize
        self.ScanChannelMask = ScanChannelMask
        self.ScanType = ScanType
        self.ScanDuration = ScanDuration
        self.maxThrNwkToDiscover = maxThrNwkToDiscover
        self.Data = Data


class THR_EventNwkCreateConfirm(object):

    def __init__(self, InstanceId=bytearray(1), EventStatus=THR_EventNwkCreateConfirmEventStatus.Success, DataSize=bytearray(2), Data=[]):
        '''
        @param InstanceId: Thread Instance Id
        @param EventStatus: Event status
        @param DataSize: The number of payload bytes
        @param Data: Data
        '''
        self.InstanceId = InstanceId
        self.EventStatus = EventStatus
        self.DataSize = DataSize
        self.Data = Data


class THR_EventNwkJoinConfirm(object):

    def __init__(self, InstanceId=bytearray(1), EventStatus=THR_EventNwkJoinConfirmEventStatus.Attaching, DataSize=bytearray(2), Data=[]):
        '''
        @param InstanceId: Thread Instance Id
        @param EventStatus: Event status
        @param DataSize: The number of payload bytes
        @param Data: Data
        '''
        self.InstanceId = InstanceId
        self.EventStatus = EventStatus
        self.DataSize = DataSize
        self.Data = Data


class THR_EventNwkJoinSelectParentsConfirm(object):

    def __init__(self, InstanceId=bytearray(1), EventStatus=THR_EventNwkJoinSelectParentsConfirmEventStatus.ScanStarted, DataSize=bytearray(2), Data=[]):
        '''
        @param InstanceId: Thread Instance Id
        @param EventStatus: Event status
        @param DataSize: The number of payload bytes
        @param Data: Data
        '''
        self.InstanceId = InstanceId
        self.EventStatus = EventStatus
        self.DataSize = DataSize
        self.Data = Data


class THR_EventGeneralConfirm(object):

    def __init__(self, InstanceId=bytearray(1), EventStatus=THR_EventGeneralConfirmEventStatus.Disconnected, DataSize=bytearray(2), Data=[]):
        '''
        @param InstanceId: Thread Instance Id
        @param EventStatus: Event status
        @param DataSize: The number of payload bytes
        @param Data: Data
        '''
        self.InstanceId = InstanceId
        self.EventStatus = EventStatus
        self.DataSize = DataSize
        self.Data = Data


class THR_EventNwkCommissioningIndication(object):

    def __init__(self, InstanceId=bytearray(1), EventStatus=THR_EventNwkCommissioningIndicationEventStatus.JoinerDiscoveryStarted):
        '''
        @param InstanceId: Thread Instance Id
        @param EventStatus: Event status
        '''
        self.InstanceId = InstanceId
        self.EventStatus = EventStatus


class THR_CommissioningDiagnosticIndication(object):

    def __init__(self, Direction=THR_CommissioningDiagnosticIndicationDirection.OUT, Type=THR_CommissioningDiagnosticIndicationType.JOIN_FIN_REQ, EUI=bytearray(8), TlvsLength=bytearray(1), TlvsBytes=[]):
        '''
        @param Direction: Direction
        @param Type: Type
        @param EUI: EUI
        @param TlvsLength: Total size of the following TLVs
        @param TlvsBytes: The actual TLVs
        '''
        self.Direction = Direction
        self.Type = Type
        self.EUI = EUI
        self.TlvsLength = TlvsLength
        self.TlvsBytes = TlvsBytes


class THR_MgmtDiagnosticGetConfirm(object):

    def __init__(self, Status=THR_MgmtDiagnosticGetConfirmStatus.Success, CoapMsgId=bytearray(2)):
        '''
        @param Status: Status
        @param CoapMsgId: Coap over the air message id -  used for syncronization btw Req-Rsp
        '''
        self.Status = Status
        self.CoapMsgId = CoapMsgId


class THR_DiagTestGetConfirm(object):

    def __init__(self, Status=THR_DiagTestGetConfirmStatus.Success, CoapMsgId=bytearray(2)):
        '''
        @param Status: Status
        @param CoapMsgId: Coap over the air message id -  used for syncronization btw Req-Rsp
        '''
        self.Status = Status
        self.CoapMsgId = CoapMsgId


class THR_MgmtDiagnosticResetConfirm(object):

    def __init__(self, Status=THR_MgmtDiagnosticResetConfirmStatus.Success, CoapMsgId=bytearray(2)):
        '''
        @param Status: Status
        @param CoapMsgId: Coap over the air message id -  used for syncronization btw Req-Rsp
        '''
        self.Status = Status
        self.CoapMsgId = CoapMsgId


class THR_MgmtReadMemoryConfirm(object):

    def __init__(self, Memory=[]):
        '''
        @param Memory: Memory content
        '''
        self.Memory = Memory


class THR_MgmtWriteMemoryConfirm(object):

    def __init__(self, Status=THR_MgmtWriteMemoryConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class THR_SetManualSlaacIIDConfirm(object):

    def __init__(self, Status=THR_SetManualSlaacIIDConfirmStatus.OK):
        '''
        @param Status: Status
        '''
        self.Status = Status


class THR_SendProactiveAddrNotifConfirm(object):

    def __init__(self, Status=THR_SendProactiveAddrNotifConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class THR_NwkDiscoveryConfirm(object):

    def __init__(self, Status=THR_NwkDiscoveryConfirmStatus.OK):
        '''
        @param Status: Status
        '''
        self.Status = Status


class THR_NwkDiscoveryStopConfirm(object):

    def __init__(self, Status=THR_NwkDiscoveryStopConfirmStatus.OK):
        '''
        @param Status: Status
        '''
        self.Status = Status


class THR_SearchNwkWithAnounceConfirm(object):

    def __init__(self, Status=THR_SearchNwkWithAnounceConfirmStatus.OK):
        '''
        @param Status: Status
        '''
        self.Status = Status


class THR_MgmtDiagnosticGetRspIndication(object):

    class TLV(object):

        def __init__(self, type=bytearray(1), length=bytearray(1), value=bytearray()):  # @ReservedAssignment
            self.type = type
            self.length = length
            self.value = value

    def __init__(self, Status=THR_MgmtDiagnosticGetRspIndicationStatus.Success, CoapMsgId=bytearray(2), DataLen=bytearray(2), TLVs=[]):
        '''
        @param Status: Status
        @param CoapMsgId: Coap over the air message id -  used for syncronization btw Req-Rsp
        @param DataLen: Data length
        @param TLVs: List of TLV() objects
        '''
        self.Status = Status
        self.CoapMsgId = CoapMsgId
        self.DataLen = DataLen
        self.TLVs = TLVs


class THR_DiagTestGetRspIndication(object):

    class Data(object):

        def __init__(self, Flags=bytearray(1), RspLatency=bytearray(4), Offset=bytearray(4), SequenceNumber=bytearray(1), PayloadSize=bytearray(1), Payload=[]):
            self.Flags = Flags
            self.RspLatency = RspLatency
            self.Offset = Offset
            self.SequenceNumber = SequenceNumber
            self.PayloadSize = PayloadSize
            # Array length depends on PayloadSize.
            self.Payload = Payload

    class Results(object):

        def __init__(self, ReqLatency=bytearray(4), RspLatency=bytearray(4), Offset=bytearray(4), SequenceNumber=bytearray(1)):
            self.ReqLatency = ReqLatency
            self.RspLatency = RspLatency
            self.Offset = Offset
            self.SequenceNumber = SequenceNumber

    def __init__(self, Status=THR_DiagTestGetRspIndicationStatus.Success, CoapMsgId=bytearray(2), DataLen=bytearray(2), Payload_TlvId=THR_DiagTestGetRspIndicationPayload_TlvId.ColdFactoryReset, Payload_TLVPayload=[]):
        '''
        @param Status: Status
        @param CoapMsgId: Coap over the air message id -  used for syncronization btw Req-Rsp
        @param DataLen: Data length
        @param Payload_TlvId: TLV identifier
        @param Payload_TLVPayload: TLV Payload
        '''
        self.Status = Status
        self.CoapMsgId = CoapMsgId
        self.DataLen = DataLen
        self.Payload_TlvId = Payload_TlvId
        self.Payload_TLVPayload = Payload_TLVPayload


class THR_MgmtDiagnosticDiagTestEventIndication(object):

    def __init__(self, Data_ReqLatency=bytearray(4), Data_SequenceNumber=bytearray(1)):
        '''
        @param Data_ReqLatency: Request Latency
        @param Data_SequenceNumber: Sequence Number
        '''
        self.Data_ReqLatency = Data_ReqLatency
        self.Data_SequenceNumber = Data_SequenceNumber


class THR_MgmtDiagnosticResetRspIndication(object):

    def __init__(self, Status=THR_MgmtDiagnosticResetRspIndicationStatus.Success, CoapMsgId=bytearray(2)):
        '''
        @param Status: Status
        @param CoapMsgId: Coap over the air message id -  used for syncronization btw Req-Rsp
        '''
        self.Status = Status
        self.CoapMsgId = CoapMsgId


class THR_SetNwkIdTimeoutConfirm(object):

    def __init__(self, Status=THR_SetNwkIdTimeoutConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class THR_SetThresholdConfirm(object):

    def __init__(self, Status=THR_SetThresholdConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class THR_GetNeighborInfoConfirm(object):

    class NeighborInfo(object):

        def __init__(self, ExtendedAddress=bytearray(8), ShortAddress=bytearray(2), LastCommTime=bytearray(4), InRSSI=bytearray(1), Timeoutsec=bytearray(4)):
            self.ExtendedAddress = ExtendedAddress
            self.ShortAddress = ShortAddress
            self.LastCommTime = LastCommTime
            self.InRSSI = InRSSI
            self.Timeoutsec = Timeoutsec

    def __init__(self, Status=THR_GetNeighborInfoConfirmStatus.Success, NeighborInfo=[]):
        '''
        @param Status: Status
        @param NeighborInfo: Neighbor Info
        '''
        self.Status = Status
        self.NeighborInfo = NeighborInfo


class THR_GetChildrenTableConfirm(object):

    class NeighborEntry(object):

        def __init__(self, ExtendedAddress=bytearray(8), ShortAddress=bytearray(2), LastCommTime=bytearray(4), LastRSSI=bytearray(1), Timeout=bytearray(4)):
            self.ExtendedAddress = ExtendedAddress
            self.ShortAddress = ShortAddress
            self.LastCommTime = LastCommTime
            self.LastRSSI = LastRSSI
            self.Timeout = Timeout

    def __init__(self, InstanceId=bytearray(1), NoOfElements=bytearray(1), NeighborEntries=[]):
        '''
        @param InstanceId: Instance Id
        @param NoOfElements: No Of Elements
        @param NeighborEntries: Neighbor Table Entries
        '''
        self.InstanceId = InstanceId
        self.NoOfElements = NoOfElements
        self.NeighborEntries = NeighborEntries


class THR_GetNeighborTableConfirm(object):

    class NeighborEntry(object):

        def __init__(self, ExtendedAddress=bytearray(8), ShortAddress=bytearray(2), LastCommTime=bytearray(4), LastRSSI=bytearray(1)):
            self.ExtendedAddress = ExtendedAddress
            self.ShortAddress = ShortAddress
            self.LastCommTime = LastCommTime
            self.LastRSSI = LastRSSI

    def __init__(self, InstanceId=bytearray(1), NoOfElements=bytearray(1), NeighborEntries=[]):
        '''
        @param InstanceId: Instance Id
        @param NoOfElements: No Of Elements
        @param NeighborEntries: Neighbor Table Entries
        '''
        self.InstanceId = InstanceId
        self.NoOfElements = NoOfElements
        self.NeighborEntries = NeighborEntries


class THR_GetRoutingTableConfirm(object):

    class RoutingEntry(object):

        def __init__(self, RouterID=bytearray(1), ShortAddress=bytearray(2), NextHop=bytearray(2), Cost=bytearray(1), nIn=bytearray(1), nOut=bytearray(1)):
            self.RouterID = RouterID
            self.ShortAddress = ShortAddress
            self.NextHop = NextHop
            self.Cost = Cost
            self.nIn = nIn
            self.nOut = nOut

    def __init__(self, NoOfElements=bytearray(1), IdSequenceNb=bytearray(1), RouterIDMask=bytearray(8), RoutingEntries=[]):
        '''
        @param NoOfElements: No Of Elements
        @param IdSequenceNb: Id Sequence Number
        @param RouterIDMask: Router ID Mask
        @param RoutingEntries: Routing Entries
        '''
        self.NoOfElements = NoOfElements
        self.IdSequenceNb = IdSequenceNb
        self.RouterIDMask = RouterIDMask
        self.RoutingEntries = RoutingEntries


class THR_GetAttrConfirm(object):

    class MLPrefix(object):

        def __init__(self, PrefixData=[], PrefixLength=bytearray(1)):
            # Array length depends on PrefixValueLength.
            self.PrefixData = PrefixData
            self.PrefixLength = PrefixLength

    class MacFilteringEntry(object):

        def __init__(self, ExtendedAddress=bytearray(8), ShortAddress=bytearray(2), LinkIndicator=bytearray(1), BlockNeighbor=bytearray(1)):
            self.ExtendedAddress = ExtendedAddress
            self.ShortAddress = ShortAddress
            self.LinkIndicator = LinkIndicator
            self.BlockNeighbor = BlockNeighbor

    class BorderRouter_BrPrefixEntry(object):

        def __init__(self, prefixLength=bytearray(1), PrefixValue=[], PrefixFlagsReserved=bytearray(1), PrefixFlags=bytearray(1), prefixLifetime=bytearray(4), prefixAdvertised=bytearray(1), ExternalRouteFlags=bytearray(1), ExternalRouteLifetime=bytearray(4), ExternalRouteAdvertised=bytearray(1)):
            self.prefixLength = prefixLength
            self.PrefixValue = PrefixValue
            self.PrefixFlagsReserved = PrefixFlagsReserved
            self.PrefixFlags = PrefixFlags
            self.prefixLifetime = prefixLifetime
            self.prefixAdvertised = prefixAdvertised
            self.ExternalRouteFlags = ExternalRouteFlags
            self.ExternalRouteLifetime = ExternalRouteLifetime
            self.ExternalRouteAdvertised = ExternalRouteAdvertised

    class BorderRouter_BrGlobalOnMeshPrefix(object):

        def __init__(self, PrefixData=[], PrefixLength=bytearray(1)):
            self.PrefixData = PrefixData
            self.PrefixLength = PrefixLength

    class BorderRouter_BrExternalIfPrefix(object):

        def __init__(self, PrefixData=[], PrefixLength=bytearray(1)):
            self.PrefixData = PrefixData
            self.PrefixLength = PrefixLength

    def __init__(self, InstanceId=bytearray(1), AttributeId=THR_GetAttrConfirmAttributeId.RandomExtendedAddr, Index=bytearray(1), Status=THR_GetAttrConfirmStatus.Success, AttrSize=bytearray(1), AttributeValue=[]):
        '''
        @param InstanceId: Instance Id
        @param AttributeId: Attribute ID of the requested field
        @param Index: Index in table if required
        @param Status: Attribute Status
        @param AttrSize: Attribute size
        @param AttributeValue: Attribute Value
        '''
        self.InstanceId = InstanceId
        self.AttributeId = AttributeId
        self.Index = Index
        self.Status = Status
        self.AttrSize = AttrSize
        self.AttributeValue = AttributeValue


class THR_SetAttrConfirm(object):

    def __init__(self, Status=THR_SetAttrConfirmStatus.Success):
        '''
        @param Status: Attribute Status
        '''
        self.Status = Status


class THR_GetThreadIpAddrConfirm(object):

    def __init__(self, InstanceId=bytearray(1), Status=THR_GetThreadIpAddrConfirmStatus.Success, AddressType=bytearray(1), NoOfIpAddr=bytearray(1), AddressList=[]):
        '''
        @param InstanceId: Instance Id
        @param Status: Status
        @param AddressType: AddressType
        @param NoOfIpAddr: Number of Ip Addresses
        @param AddressList: AddressList
        '''
        self.InstanceId = InstanceId
        self.Status = Status
        self.AddressType = AddressType
        self.NoOfIpAddr = NoOfIpAddr
        self.AddressList = AddressList


class THR_GetParentConfirm(object):

    def __init__(self, Status=THR_GetParentConfirmStatus.Success, InstanceId=bytearray(1), ShortAddress=bytearray(2), ExtendedAddress=bytearray(8)):
        '''
        @param Status: Status
        @param InstanceId: Instance Id
        @param ShortAddress: Short Address
        @param ExtendedAddress: Extended address
        '''
        self.Status = Status
        self.InstanceId = InstanceId
        self.ShortAddress = ShortAddress
        self.ExtendedAddress = ExtendedAddress


class THR_ChildUpdateToParentConfirm(object):

    def __init__(self, Status=THR_ChildUpdateToParentConfirmStatus.OK):
        '''
        @param Status: Status
        '''
        self.Status = Status


class THR_LeaderRemoveRouterIdConfirm(object):

    def __init__(self, Status=THR_LeaderRemoveRouterIdConfirmStatus.OK):
        '''
        @param Status: Status
        '''
        self.Status = Status


class THR_GenerateAllKeysConfirm(object):

    def __init__(self, Status=THR_GenerateAllKeysConfirmStatus.OK):
        '''
        @param Status: Status
        '''
        self.Status = Status


class THR_SwitchKeyKeyConfirm(object):

    def __init__(self, Status=THR_SwitchKeyKeyConfirmStatus.OK):
        '''
        @param Status: Status
        '''
        self.Status = Status


class THR_BrPrefixAddEntryConfirm(object):

    def __init__(self, Status=THR_BrPrefixAddEntryConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class THR_BrPrefixGetTableConfirm(object):

    class BrPrefixEntry(object):

        def __init__(self, prefixLength=bytearray(1), PrefixValue=[], PrefixFlagsReserved=bytearray(1), PrefixFlags=bytearray(1), prefixLifetime=bytearray(4), prefixAdvertised=bytearray(1), ExternalRouteFlags=bytearray(1), ExternalRouteLifetime=bytearray(4), ExternalRouteAdvertised=bytearray(1)):
            self.prefixLength = prefixLength
            self.PrefixValue = PrefixValue
            self.PrefixFlagsReserved = PrefixFlagsReserved
            self.PrefixFlags = PrefixFlags
            self.prefixLifetime = prefixLifetime
            self.prefixAdvertised = prefixAdvertised
            self.ExternalRouteFlags = ExternalRouteFlags
            self.ExternalRouteLifetime = ExternalRouteLifetime
            self.ExternalRouteAdvertised = ExternalRouteAdvertised

    def __init__(self, InstanceId=bytearray(1), NoOfElements=bytearray(1), BrPrefixEntries=[]):
        '''
        @param InstanceId: Instance Id
        @param NoOfElements: No Of Elements
        @param BrPrefixEntries: Border router prefix entries
        '''
        self.InstanceId = InstanceId
        self.NoOfElements = NoOfElements
        self.BrPrefixEntries = BrPrefixEntries


class THR_BrPrefixRemoveEntryConfirm(object):

    def __init__(self, Status=THR_BrPrefixRemoveEntryConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class THR_BrServiceRemoveEntryConfirm(object):

    def __init__(self, Status=THR_BrServiceRemoveEntryConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class THR_BrServiceAddEntryConfirm(object):

    def __init__(self, Status=THR_BrServiceAddEntryConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class THR_BrPrefixSyncConfirm(object):

    def __init__(self, Status=THR_BrPrefixSyncConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class THR_BrPrefixRemoveAllConfirm(object):

    def __init__(self, Status=THR_BrPrefixRemoveAllConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class NWKU_EidToRlocMapResponse(object):

    class CacheEntry(object):

        def __init__(self, EID=bytearray(16), RLOC=bytearray(2)):
            # Unit length: 16 bytes
            self.EID = EID
            self.RLOC = RLOC

    def __init__(self, NumberOfEntries=bytearray(1), CacheEntry=[]):
        '''
        @param NumberOfEntries: Number of cache entries
        @param CacheEntry: CacheEntry
        '''
        self.NumberOfEntries = NumberOfEntries
        self.CacheEntry = CacheEntry


class THR_IdentifyConfirm(object):

    def __init__(self, Status=THR_IdentifyConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class NWKU_IfconfigBindConfirm(object):

    def __init__(self, Status=NWKU_IfconfigBindConfirmStatus.OK):
        '''
        @param Status: Status
        '''
        self.Status = Status


class NWKU_IfconfigAllResponse(object):

    class InterfaceID(object):

        def __init__(self, InterfaceID=bytearray(1), CountIpAddresses=bytearray(1), Addresses=[]):
            self.InterfaceID = InterfaceID
            self.CountIpAddresses = CountIpAddresses
            # Unit length: 16 bytes
            # Array length depends on CountIpAddresses.
            self.Addresses = Addresses

    def __init__(self, CountInterfaces=bytearray(1), InterfaceList=[]):
        '''
        @param CountInterfaces: Number of interfaces
        @param InterfaceList: List of interface IDs.
        '''
        self.CountInterfaces = CountInterfaces
        self.InterfaceList = InterfaceList


class NWKU_PingConfirm(object):

    def __init__(self, Status=NWKU_PingConfirmStatus.OK, Interval=bytearray(2)):
        '''
        @param Status: Status
        @param Interval: Interval
        '''
        self.Status = Status
        self.Interval = Interval


class NWKU_EchoUDPConfirm(object):

    def __init__(self, Status=NWKU_EchoUDPConfirmStatus.OK):
        '''
        @param Status: Status
        '''
        self.Status = Status


class NWKU_CoapMsgReceivedIndication(object):

    def __init__(self, Status=NWKU_CoapMsgReceivedIndicationStatus.Success, RemoteIpAddress=bytearray(16), UDPPort=bytearray(2), RequestType=NWKU_CoapMsgReceivedIndicationRequestType.CON, MessageType=NWKU_CoapMsgReceivedIndicationMessageType.GET, URIpath=bytearray(30), PayloadLength=bytearray(1), Payload=[]):
        '''
        @param Status: Status
        @param RemoteIpAddress: The source address of the device that sent the message
        @param UDPPort: UDPPort
        @param RequestType: CoAP request type
        @param MessageType: CoAP message type
        @param URIpath: URI-path options separated by /. The maximum length for one URI-path option is 10 characters.
        @param PayloadLength: Data length
        @param Payload: Data
        '''
        self.Status = Status
        self.RemoteIpAddress = RemoteIpAddress
        self.UDPPort = UDPPort
        self.RequestType = RequestType
        self.MessageType = MessageType
        self.URIpath = URIpath
        self.PayloadLength = PayloadLength
        self.Payload = Payload


class NWKU_DnsResponseReceivedIndication(object):

    def __init__(self, IpAddress=bytearray(16), DomainName=bytearray(30)):
        '''
        @param IpAddress: The IPv6 address of the domain name
        @param DomainName: DomainName
        '''
        self.IpAddress = IpAddress
        self.DomainName = DomainName


class NWKU_CoapRegisterConfirm(object):

    def __init__(self, Status=NWKU_CoapRegisterConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class NWKU_CoapCreateInstanceConfirm(object):

    def __init__(self, Status=NWKU_CoapCreateInstanceConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class NWKU_CoapSendConfirm(object):

    def __init__(self, Status=NWKU_CoapSendConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class NWKU_DnsSendRequestConfirm(object):

    def __init__(self, Status=NWKU_DnsSendRequestConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class NWKU_McastGroupManageConfirm(object):

    def __init__(self, Status=NWKU_McastGroupManageConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class NWKU_McastGroupShowResponse(object):

    def __init__(self, CountIpAddresses=bytearray(1), Addresses=[]):
        '''
        @param CountIpAddresses: Number of IP addresses
        @param Addresses: Addresses
        '''
        self.CountIpAddresses = CountIpAddresses
        self.Addresses = Addresses


class NWKU_RoutesManageConfirm(object):

    def __init__(self, Status=NWKU_RoutesManageConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class NWKU_RoutesShowResponse(object):

    class Route(object):

        def __init__(self, Destination=bytearray(16), NextHop=bytearray(16), PrefixLength=bytearray(1), RouteMetric=bytearray(1), InstanceId=bytearray(1)):
            # Unit length: 16 bytes
            self.Destination = Destination
            # Unit length: 16 bytes
            self.NextHop = NextHop
            self.PrefixLength = PrefixLength
            self.RouteMetric = RouteMetric
            self.InstanceId = InstanceId

    def __init__(self, CountRoutes=bytearray(1), Route=[]):
        '''
        @param CountRoutes: Number of IPv6 routes
        @param Route: RoutingTable
        '''
        self.CountRoutes = CountRoutes
        self.Route = Route


class MESHCOP_StartCommissionerConfirm(object):

    def __init__(self, Status=MESHCOP_StartCommissionerConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class MESHCOP_StartNativeCommissionerConfirm(object):

    def __init__(self, Status=MESHCOP_StartNativeCommissionerConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class MESHCOP_StopCommissionerConfirm(object):

    def __init__(self, Status=MESHCOP_StopCommissionerConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class MESHCOP_AddExpectedJoinerConfirm(object):

    def __init__(self, Status=MESHCOP_AddExpectedJoinerConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class MESHCOP_GetExpectedJoinerConfirm(object):

    def __init__(self, Status=MESHCOP_GetExpectedJoinerConfirmStatus.Success, Selected=MESHCOP_GetExpectedJoinerConfirmSelected.FALSE, PSKdSize=bytearray(1), PSKd=[]):
        '''
        @param Status: Status
        @param Selected: Selected
        @param PSKdSize: Length of PSKd
        @param PSKd: PSKd
        '''
        self.Status = Status
        self.Selected = Selected
        self.PSKdSize = PSKdSize
        self.PSKd = PSKd


class MESHCOP_RemoveExpectedJoinerConfirm(object):

    def __init__(self, Status=MESHCOP_RemoveExpectedJoinerConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class MESHCOP_RemoveAllExpectedJoinersConfirm(object):

    def __init__(self, Status=MESHCOP_RemoveAllExpectedJoinersConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class MESHCOP_SyncSteeringDataConfirm(object):

    def __init__(self, Status=MESHCOP_SyncSteeringDataConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class MESHCOP_MgmtSetConfirm(object):

    def __init__(self, Status=MESHCOP_MgmtSetConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class MESHCOP_MgmtGetConfirm(object):

    def __init__(self, Status=MESHCOP_MgmtGetConfirmStatus.Success, Type=MESHCOP_MgmtGetConfirmType.Channel, Length=bytearray(1), Value=[]):
        '''
        @param Status: Status
        @param Type: Type of the requested attribute
        @param Length: Length of the next field
        @param Value: Value
        '''
        self.Status = Status
        self.Type = Type
        self.Length = Length
        self.Value = Value


class MESHCOP_SetCommissionerCredentialConfirm(object):

    def __init__(self, Status=MESHCOP_SetCommissionerCredentialConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class MESHCOP_MgmNwkFormConfirm(object):

    def __init__(self, Status=MESHCOP_MgmNwkFormConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class MESHCOP_MgmtCommissionerGetConfirm(object):

    def __init__(self, Status=MESHCOP_MgmtCommissionerGetConfirmStatus.Success, Length=bytearray(4), TLVs=[]):
        '''
        @param Status: Status
        @param Length: Length of the next field
        @param TLVs: TLVs
        '''
        self.Status = Status
        self.Length = Length
        self.TLVs = TLVs


class MESHCOP_MgmtActiveGetConfirm(object):

    def __init__(self, Status=MESHCOP_MgmtActiveGetConfirmStatus.Success, Length=bytearray(4), TLVs=[]):
        '''
        @param Status: Status
        @param Length: Length of the next field
        @param TLVs: TLVs
        '''
        self.Status = Status
        self.Length = Length
        self.TLVs = TLVs


class MESHCOP_MgmtPendingGetConfirm(object):

    def __init__(self, Status=MESHCOP_MgmtPendingGetConfirmStatus.Success, Length=bytearray(4), TLVs=[]):
        '''
        @param Status: Status
        @param Length: Length of the next field
        @param TLVs: TLVs
        '''
        self.Status = Status
        self.Length = Length
        self.TLVs = TLVs


class MESHCOP_MgmtCommissionerSetConfirm(object):

    def __init__(self, Status=MESHCOP_MgmtCommissionerSetConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class MESHCOP_MgmtActiveSetConfirm(object):

    def __init__(self, Status=MESHCOP_MgmtActiveSetConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class MESHCOP_MgmtPendingSetConfirm(object):

    def __init__(self, Status=MESHCOP_MgmtPendingSetConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class MESHCOP_MgmtSendPanIdQueryConfirm(object):

    def __init__(self, Status=MESHCOP_MgmtSendPanIdQueryConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class MESHCOP_MgmtPanIdConflictConfirm(object):

    def __init__(self, Status=MESHCOP_MgmtPanIdConflictConfirmStatus.Success, ScanChannelMask=bytearray(4), PanId=bytearray(2)):
        '''
        @param Status: Status
        @param ScanChannelMask: Channel mask
        @param PanId: PanId
        '''
        self.Status = Status
        self.ScanChannelMask = ScanChannelMask
        self.PanId = PanId


class MESHCOP_MgmtSendEdScanConfirm(object):

    def __init__(self, Status=MESHCOP_MgmtSendEdScanConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class MESHCOP_MgmtEdReportConfirm(object):

    def __init__(self, Status=MESHCOP_MgmtEdReportConfirmStatus.Success, ScanChannelMask=bytearray(4), Length=bytearray(1), EnergyList=[]):
        '''
        @param Status: Status
        @param ScanChannelMask: Channel mask
        @param Length: Length
        @param EnergyList: EnergyList
        '''
        self.Status = Status
        self.ScanChannelMask = ScanChannelMask
        self.Length = Length
        self.EnergyList = EnergyList


class MESHCOP_MgmtSendAnnounceBeginConfirm(object):

    def __init__(self, Status=MESHCOP_MgmtSendAnnounceBeginConfirmStatus.Success):
        '''
        @param Status: Status
        '''
        self.Status = Status


class DTLSOpenConfirm(object):

    def __init__(self, ContextIndex=bytearray(1)):
        '''
        @param ContextIndex: Context Index
        '''
        self.ContextIndex = ContextIndex


class DTLSCloseConfirm(object):

    def __init__(self, Status=DTLSCloseConfirmStatus.OK):
        '''
        @param Status: Status
        '''
        self.Status = Status


class DTLSClosePeerConfirm(object):

    def __init__(self, Status=DTLSClosePeerConfirmStatus.OK):
        '''
        @param Status: Status
        '''
        self.Status = Status


class DTLSConnectConfirm(object):

    def __init__(self, Status=DTLSConnectConfirmStatus.OK, PeerIndex=bytearray(1)):
        '''
        @param Status: Status
        @param PeerIndex: Peer Index
        '''
        self.Status = Status
        self.PeerIndex = PeerIndex


class DTLSClientConnectedConfirm(object):

    def __init__(self, Status=DTLSClientConnectedConfirmStatus.OK, PeerIndex=bytearray(1)):
        '''
        @param Status: Status
        @param PeerIndex: Peer Index
        '''
        self.Status = Status
        self.PeerIndex = PeerIndex


class DTLSSendConfirm(object):

    def __init__(self, Status=DTLSSendConfirmStatus.OK):
        '''
        @param Status: Status
        '''
        self.Status = Status


class DTLSReceiveConfirm(object):

    def __init__(self, PeerIndex=bytearray(1), Size=bytearray(2), Data=[]):
        '''
        @param PeerIndex: Peer Index
        @param Size: The number of payload bytes
        @param Data: Data
        '''
        self.PeerIndex = PeerIndex
        self.Size = Size
        self.Data = Data


class FSCIGetUniqueIdConfirm(object):

    def __init__(self, UniqueId=bytearray(16)):
        '''
        @param UniqueId: Unique Id
        '''
        self.UniqueId = UniqueId


class FSCIGetMcuIdConfirm(object):

    def __init__(self, McuId=bytearray(4)):
        '''
        @param McuId: Mcu Id
        '''
        self.McuId = McuId


class FSCIGetSwVersionsConfirm(object):

    class SwVersion(object):

        def __init__(self, ModuleId=bytearray(1), VersionNumber=bytearray(3), BuildNumber=bytearray(2)):
            self.ModuleId = ModuleId
            self.VersionNumber = VersionNumber
            self.BuildNumber = BuildNumber

    def __init__(self, NumberOfEntries=bytearray(1), SwVersions=[]):
        '''
        @param NumberOfEntries: Number of entries (one entry has 6 bytes)
        @param SwVersions: List of SW Module versions
        '''
        self.NumberOfEntries = NumberOfEntries
        self.SwVersions = SwVersions


class Sniffer_MacSetPIBAttributeConfirm(object):

    def __init__(self, Status=Sniffer_MacSetPIBAttributeConfirmStatus.gSuccess_c, PIBAttribute=Sniffer_MacSetPIBAttributeConfirmPIBAttribute.macLogicalChannel, DataLength=bytearray(2), PIBAttributeValue=[]):
        '''
        @param Status: The result of the Sniffer_MacSetPIBAttribute.Request
        @param PIBAttribute: The MAC PIB attribute identifier
        @param DataLength: Length of the attribute data
        @param PIBAttributeValue: The MAC PIB attribute value
        '''
        self.Status = Status
        self.PIBAttribute = PIBAttribute
        self.DataLength = DataLength
        self.PIBAttributeValue = PIBAttributeValue


class Sniffer_DetectConfirm(object):

    def __init__(self, SnifferType=Sniffer_DetectConfirmSnifferType.USBKW24D):
        self.SnifferType = SnifferType


class MAC_PromiscuousRxIndication(object):

    def __init__(self, LinkQuality=bytearray(1), TimeStamp=bytearray(4), msduLength=bytearray(1), msdu=[]):
        '''
        @param LinkQuality: Link Quality
        @param TimeStamp: Time Stamp
        @param msduLength: Length of the frame
        @param msdu: The actual frame received
        '''
        self.LinkQuality = LinkQuality
        self.TimeStamp = TimeStamp
        self.msduLength = msduLength
        self.msdu = msdu


class SerialTun_LinkIndicationConfirm(object):

    def __init__(self, Status=SerialTun_LinkIndicationConfirmStatus.OK):
        '''
        @param Status: Status
        '''
        self.Status = Status


class SerialTun_IPPacketReceivedConfirm(object):

    def __init__(self, IPpayload=bytearray()):
        '''
        @param IPpayload: IP payload
        '''
        self.IPpayload = IPpayload


class AspSetPowerLevelConfirm(object):

    def __init__(self, Status=AspSetPowerLevelConfirmStatus.SUCCESS):
        '''
        @param Status: The status of the Set Power Level request
        '''
        self.Status = Status


class AspGetPowerLevelConfirm(object):

    def __init__(self, Value=bytearray(1)):
        '''
        @param Value: The Value of Power Level
        '''
        self.Value = Value


class DBGConfirm(object):

    def __init__(self, text=bytearray()):
        '''
        @param text: text
        '''
        self.text = text
