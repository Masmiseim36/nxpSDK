'''
* The Clear BSD License
* Copyright 2017-2018 NXP
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

from com.nxp.wireless_connectivity.commands.zigbee.enums import *  # @UnusedWildImport


class GetVersion(object):

    pass


class SetExtendedPANID(object):

    def __init__(self, ExtendedPANID=bytearray(8)):
        '''
        @param ExtendedPANID: ExtendedPANID
        '''
        self.ExtendedPANID = ExtendedPANID


class SetChannelMask(object):

    def __init__(self, ChannelMask=bytearray(4)):
        '''
        @param ChannelMask: ChannelMask
        '''
        self.ChannelMask = ChannelMask


class SetSecurityStateKey(object):

    def __init__(self, KeyType=bytearray(1), KeyData=bytearray(1)):
        '''
        @param KeyType: KeyType
        @param KeyData: KeyData
        '''
        self.KeyType = KeyType
        self.KeyData = KeyData


class SetDeviceType(object):

    def __init__(self, DeviceType=SetDeviceTypeDeviceType.Coordinator):
        '''
        @param DeviceType: DeviceType
        '''
        self.DeviceType = DeviceType


class StartNetworkMessage(object):

    pass


class StartNetworkScan(object):

    pass


class Code(object):

    def __init__(self, TargetExtendedAddress=bytearray(8), Code=bytearray(16)):
        '''
        @param TargetExtendedAddress: TargetExtendedAddress
        @param Code: Code
        '''
        self.TargetExtendedAddress = TargetExtendedAddress
        self.Code = Code


class ZLO_ZLLFactoryNewReset(object):

    pass


class PermitJoinStatusOnTheTarget(object):

    pass


class Reset(object):

    pass


class ErasePersistentData(object):

    pass


class Bind(object):

    def __init__(self, TargetExtendedAddress=bytearray(8), TargetEndPoint=bytearray(1), ClusterID=bytearray(2), DestAddressingMode=BindDestAddressingMode.NO_PAN, DestinationAddress=[], DestinationEndPoint=bytearray(1)):
        '''
        @param TargetExtendedAddress: TargetExtendedAddress
        @param TargetEndPoint: TargetEndPoint
        @param ClusterID: ClusterID
        @param DestAddressingMode: DestAddressingMode
        @param DestinationAddress: MHR, Addressing fields. 0xFFFF Broadcast PAN identifier.
        @param DestinationEndPoint: DestinationEndPoint
        '''
        self.TargetExtendedAddress = TargetExtendedAddress
        self.TargetEndPoint = TargetEndPoint
        self.ClusterID = ClusterID
        self.DestAddressingMode = DestAddressingMode
        self.DestinationAddress = DestinationAddress
        self.DestinationEndPoint = DestinationEndPoint


class Unbind(object):

    def __init__(self, TargetExtendedAddress=bytearray(8), TargetEndPoint=bytearray(1), ClusterID=bytearray(2), DestAddressingMode=UnbindDestAddressingMode.NO_PAN, DestinationAddress=[], DestinationEndPoint=bytearray(1)):
        '''
        @param TargetExtendedAddress: TargetExtendedAddress
        @param TargetEndPoint: TargetEndPoint
        @param ClusterID: ClusterID
        @param DestAddressingMode: DestAddressingMode
        @param DestinationAddress: MHR, Addressing fields. 0xFFFF Broadcast PAN identifier.
        @param DestinationEndPoint: DestinationEndPoint
        '''
        self.TargetExtendedAddress = TargetExtendedAddress
        self.TargetEndPoint = TargetEndPoint
        self.ClusterID = ClusterID
        self.DestAddressingMode = DestAddressingMode
        self.DestinationAddress = DestinationAddress
        self.DestinationEndPoint = DestinationEndPoint


class NetworkAddressRequest(object):

    def __init__(self, TargetShortAddress=bytearray(2), ExtendedAddress=bytearray(8), RequestType=NetworkAddressRequestRequestType.SingleRequest, StartIndex=bytearray(1)):
        '''
        @param TargetShortAddress: TargetShortAddress
        @param ExtendedAddress: ExtendedAddress
        @param RequestType: RequestType
        @param StartIndex: StartIndex
        '''
        self.TargetShortAddress = TargetShortAddress
        self.ExtendedAddress = ExtendedAddress
        self.RequestType = RequestType
        self.StartIndex = StartIndex


class IEEEAddressRequest(object):

    def __init__(self, TargetShortAddress=bytearray(2), ShortAddress=bytearray(2), RequestType=IEEEAddressRequestRequestType.SingleRequest, StartIndex=bytearray(1)):
        '''
        @param TargetShortAddress: TargetShortAddress
        @param ShortAddress: ShortAddress
        @param RequestType: RequestType
        @param StartIndex: StartIndex
        '''
        self.TargetShortAddress = TargetShortAddress
        self.ShortAddress = ShortAddress
        self.RequestType = RequestType
        self.StartIndex = StartIndex


class NodeDescriptorRequest(object):

    class ServerMask(object):

        def __init__(self, PrimaryTrustCenter=bytearray(1), BackUpTrustCenter=bytearray(1), PrimaryBindingCache=bytearray(1), BackupBindingCache=bytearray(1), PrimaryDiscoveryCache=bytearray(1), BackupDiscoveryCache=bytearray(1), NetworkManager=bytearray(1), AllocateAddress=bytearray(1)):
            self.PrimaryTrustCenter = PrimaryTrustCenter
            self.BackUpTrustCenter = BackUpTrustCenter
            self.PrimaryBindingCache = PrimaryBindingCache
            self.BackupBindingCache = BackupBindingCache
            self.PrimaryDiscoveryCache = PrimaryDiscoveryCache
            self.BackupDiscoveryCache = BackupDiscoveryCache
            self.NetworkManager = NetworkManager
            self.AllocateAddress = AllocateAddress

    class DescriptorCapability(object):

        def __init__(self, ExtendedActiveEndPointList=bytearray(1), ExtendedSimpleDdescriptorList=bytearray(1), Reserved=bytearray(6)):
            self.ExtendedActiveEndPointList = ExtendedActiveEndPointList
            self.ExtendedSimpleDdescriptorList = ExtendedSimpleDdescriptorList
            # Unit length: 6 bytes
            self.Reserved = Reserved

    class MacCapability(object):

        def __init__(self, AlternatePANCoordinator=bytearray(1), DeviceType=bytearray(1), PowerSource=bytearray(1), ReceiverOnWhenIdle=bytearray(1), Reserved=bytearray(2), SecurityCapability=bytearray(1), AllocateAddress=bytearray(1)):
            self.AlternatePANCoordinator = AlternatePANCoordinator
            self.DeviceType = DeviceType
            self.PowerSource = PowerSource
            self.ReceiverOnWhenIdle = ReceiverOnWhenIdle
            self.Reserved = Reserved
            self.SecurityCapability = SecurityCapability
            self.AllocateAddress = AllocateAddress

    class BitFields(object):

        def __init__(self, Coordinator=bytearray(1), Router=bytearray(1), EndDevice=bytearray(1), ComplexDescriptorAvailable=bytearray(1), UserDescriptorAvailable=bytearray(1), Reserved=bytearray(3), APSFlags=bytearray(3), FrequencyBand=bytearray(5)):
            self.Coordinator = Coordinator
            self.Router = Router
            self.EndDevice = EndDevice
            self.ComplexDescriptorAvailable = ComplexDescriptorAvailable
            self.UserDescriptorAvailable = UserDescriptorAvailable
            # Unit length: 3 bytes
            self.Reserved = Reserved
            # Unit length: 3 bytes
            self.APSFlags = APSFlags
            # Unit length: 5 bytes
            self.FrequencyBand = FrequencyBand

    def __init__(self, SequenceNumber=bytearray(1), Status=bytearray(1), NetworkAddress=bytearray(2), ManufacturerCode=bytearray(2), MaxRxSize=bytearray(2), MaxTxSize=bytearray(2), ServerMask=[], DescriptorCapability=DescriptorCapability(1), MacCapability=MacCapability(1), MaxBufferSize=bytearray(1), BitFields=BitFields(2)):
        '''
        @param SequenceNumber: SequenceNumber
        @param Status: Status
        @param NetworkAddress: NetworkAddress
        @param ManufacturerCode: ManufacturerCode
        @param MaxRxSize: MaxRxSize
        @param MaxTxSize: MaxTxSize
        @param ServerMask: ServerMask
        @param DescriptorCapability: DescriptorCapability
        @param MacCapability: MacCapability
        @param MaxBufferSize: MaxBufferSize
        @param BitFields: BitFields
        '''
        self.SequenceNumber = SequenceNumber
        self.Status = Status
        self.NetworkAddress = NetworkAddress
        self.ManufacturerCode = ManufacturerCode
        self.MaxRxSize = MaxRxSize
        self.MaxTxSize = MaxTxSize
        self.ServerMask = ServerMask
        self.DescriptorCapability = DescriptorCapability
        self.MacCapability = MacCapability
        self.MaxBufferSize = MaxBufferSize
        self.BitFields = BitFields


class SimpleDescriptorRequest(object):

    def __init__(self, TargetShortAddress=bytearray(2), EndPoint=bytearray(1)):
        '''
        @param TargetShortAddress: TargetShortAddress
        @param EndPoint: EndPoint
        '''
        self.TargetShortAddress = TargetShortAddress
        self.EndPoint = EndPoint


class PowerDescriptorRequest(object):

    def __init__(self, TargetShortAddress=bytearray(2)):
        '''
        @param TargetShortAddress: TargetShortAddress
        '''
        self.TargetShortAddress = TargetShortAddress


class ActiveEndpointRequest(object):

    def __init__(self, TargetShortAddress=bytearray(2)):
        '''
        @param TargetShortAddress: TargetShortAddress
        '''
        self.TargetShortAddress = TargetShortAddress


class MatchDescriptorRequest(object):

    def __init__(self, TargetShortAddress=bytearray(2), ProfileID=bytearray(2), NumberOfInputClusters=bytearray(1), InputClusterList=[], NumberOfOutputClusters=bytearray(1), OutputClusterList=[]):
        '''
        @param TargetShortAddress: TargetShortAddress
        @param ProfileID: ProfileID
        @param NumberOfInputClusters: NumberOfInputClusters
        @param InputClusterList: InputClusterList
        @param NumberOfOutputClusters: NumberOfOutputClusters
        @param OutputClusterList: OutputClusterList
        '''
        self.TargetShortAddress = TargetShortAddress
        self.ProfileID = ProfileID
        self.NumberOfInputClusters = NumberOfInputClusters
        self.InputClusterList = InputClusterList
        self.NumberOfOutputClusters = NumberOfOutputClusters
        self.OutputClusterList = OutputClusterList


class RemoveDevice(object):

    def __init__(self, ParentExtendedAddress=bytearray(8), ChildExtendedAddress=bytearray(8)):
        '''
        @param ParentExtendedAddress: ParentExtendedAddress
        @param ChildExtendedAddress: ChildExtendedAddress
        '''
        self.ParentExtendedAddress = ParentExtendedAddress
        self.ChildExtendedAddress = ChildExtendedAddress


class UserDescriptorSet(object):

    def __init__(self, TargetShortAddress=bytearray(2), AddressOfInterest=bytearray(2), StringLength=bytearray(1), Parameters=[]):
        '''
        @param TargetShortAddress: TargetShortAddress
        @param AddressOfInterest: AddressOfInterest
        @param StringLength: StringLength
        @param Parameters: Parameters
        '''
        self.TargetShortAddress = TargetShortAddress
        self.AddressOfInterest = AddressOfInterest
        self.StringLength = StringLength
        self.Parameters = Parameters


class UserDescriptorRequest(object):

    def __init__(self, TargetShortAddress=bytearray(2), AddressOfInterest=bytearray(2)):
        '''
        @param TargetShortAddress: TargetShortAddress
        @param AddressOfInterest: AddressOfInterest
        '''
        self.TargetShortAddress = TargetShortAddress
        self.AddressOfInterest = AddressOfInterest


class SetPollIntervalRequest(object):

    def __init__(self, Interval=bytearray(4)):
        '''
        @param Interval: Poll Interval in seconds
        '''
        self.Interval = Interval


class ComplexDescriptorRequest(object):

    def __init__(self, TargetShortAddress=bytearray(2), AddressOfInterest=bytearray(2)):
        '''
        @param TargetShortAddress: TargetShortAddress
        @param AddressOfInterest: AddressOfInterest
        '''
        self.TargetShortAddress = TargetShortAddress
        self.AddressOfInterest = AddressOfInterest


class ManagementLeaveRequest(object):

    def __init__(self, TargetShortAddress=bytearray(2), ExtendedAddress=bytearray(8), Rejoin=ManagementLeaveRequestRejoin.DoNotRejoin, RemoveChildren=ManagementLeaveRequestRemoveChildren.LeaveRemovingChildren):
        '''
        @param TargetShortAddress: TargetShortAddress
        @param ExtendedAddress: ExtendedAddress
        @param Rejoin: Rejoin
        @param RemoveChildren: RemoveChildren
        '''
        self.TargetShortAddress = TargetShortAddress
        self.ExtendedAddress = ExtendedAddress
        self.Rejoin = Rejoin
        self.RemoveChildren = RemoveChildren


class PermitJoiningRequest(object):

    def __init__(self, TargetShortAddress=bytearray(2), Interval=bytearray(1), TCSignificance=PermitJoiningRequestTCSignificance.NoChangeInAuthentication):
        '''
        @param TargetShortAddress: TargetShortAddress
        @param Interval: Interval: DisableJoining 0x00, TimeInSecondsToAllowJoins 0x01-0xFE, AllowAllJoins 0xFF
        @param TCSignificance: TCSignificance
        '''
        self.TargetShortAddress = TargetShortAddress
        self.Interval = Interval
        self.TCSignificance = TCSignificance


class ManagementNetworkUpdateRequest(object):

    def __init__(self, TargetShortAddress=bytearray(2), ChannelMask=bytearray(4), ScanDuration=bytearray(1), ScanCount=bytearray(1), NetworkUpdateID=bytearray(1), NetworkManagerShortAddress=bytearray(2)):
        '''
        @param TargetShortAddress: TargetShortAddress
        @param ChannelMask: ChannelMask
        @param ScanDuration: ScanDuration
        @param ScanCount: ScanCount
        @param NetworkUpdateID: NetworkUpdateID
        @param NetworkManagerShortAddress: NetworkManagerShortAddress
        '''
        self.TargetShortAddress = TargetShortAddress
        self.ChannelMask = ChannelMask
        self.ScanDuration = ScanDuration
        self.ScanCount = ScanCount
        self.NetworkUpdateID = NetworkUpdateID
        self.NetworkManagerShortAddress = NetworkManagerShortAddress


class SystemServerDiscoveryRequest(object):

    def __init__(self, TargetShortAddress=bytearray(2), ServerMask=bytearray(2)):
        '''
        @param TargetShortAddress: TargetShortAddress
        @param ServerMask: ServerMask
        '''
        self.TargetShortAddress = TargetShortAddress
        self.ServerMask = ServerMask


class ManagementLQIRequest(object):

    def __init__(self, TargetAddress=bytearray(2), StartIndex=bytearray(1)):
        '''
        @param TargetAddress: TargetAddress
        @param StartIndex: StartIndex
        '''
        self.TargetAddress = TargetAddress
        self.StartIndex = StartIndex


class ReadAttributeRequest(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), ClusterID=bytearray(2), Direction=ReadAttributeRequestDirection.FromServerToClient, ManufacturerSpecific=ReadAttributeRequestManufacturerSpecific.No, ManufacturerID=bytearray(2), NumberOfAttributes=bytearray(1), AttributesList=[]):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param ClusterID: ClusterID
        @param Direction: Direction
        @param ManufacturerSpecific: ManufacturerSpecific
        @param ManufacturerID: ManufacturerID
        @param NumberOfAttributes: NumberOfAttributes
        @param AttributesList: AttributesList
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.ClusterID = ClusterID
        self.Direction = Direction
        self.ManufacturerSpecific = ManufacturerSpecific
        self.ManufacturerID = ManufacturerID
        self.NumberOfAttributes = NumberOfAttributes
        self.AttributesList = AttributesList


class WriteAttributeRequest(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), ClusterID=bytearray(2), Direction=WriteAttributeRequestDirection.FromServerToClient, ManufacturerSpecific=WriteAttributeRequestManufacturerSpecific.No, ManufacturerID=bytearray(2), NumberOfAttributes=bytearray(1), AttributesList=[]):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param ClusterID: ClusterID
        @param Direction: Direction
        @param ManufacturerSpecific: ManufacturerSpecific
        @param ManufacturerID: ManufacturerID
        @param NumberOfAttributes: NumberOfAttributes
        @param AttributesList: AttributesList
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.ClusterID = ClusterID
        self.Direction = Direction
        self.ManufacturerSpecific = ManufacturerSpecific
        self.ManufacturerID = ManufacturerID
        self.NumberOfAttributes = NumberOfAttributes
        self.AttributesList = AttributesList


class AttributeDiscoveryRequest(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), ClusterID=bytearray(2), AttributeID=bytearray(2), Direction=AttributeDiscoveryRequestDirection.FromServerToClient, ManufacturerSpecific=AttributeDiscoveryRequestManufacturerSpecific.No, ManufacturerID=bytearray(2), NumberOfAttributes=bytearray(1), MaxNumberOfIdentifiers=[]):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param ClusterID: ClusterID
        @param AttributeID: AttributeID
        @param Direction: Direction
        @param ManufacturerSpecific: ManufacturerSpecific
        @param ManufacturerID: ManufacturerID
        @param NumberOfAttributes: NumberOfAttributes
        @param MaxNumberOfIdentifiers: MaxNumberOfIdentifiers
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.ClusterID = ClusterID
        self.AttributeID = AttributeID
        self.Direction = Direction
        self.ManufacturerSpecific = ManufacturerSpecific
        self.ManufacturerID = ManufacturerID
        self.NumberOfAttributes = NumberOfAttributes
        self.MaxNumberOfIdentifiers = MaxNumberOfIdentifiers


class EnablePermissionsControlledJoins(object):

    def __init__(self, EnableDisable=EnablePermissionsControlledJoinsEnableDisable.Disable):
        '''
        @param EnableDisable: EnableDisable
        '''
        self.EnableDisable = EnableDisable


class AuthenticateDevice(object):

    def __init__(self, IEEEAddress=bytearray(8), Key=bytearray(16)):
        '''
        @param IEEEAddress: IEEEAddress
        @param Key: Key
        '''
        self.IEEEAddress = IEEEAddress
        self.Key = Key


class ConfigureReportingRequest(object):

    class AttributesList(object):

        def __init__(self, AttributeDirection=bytearray(1), AttributeType=bytearray(1), AttributeID=bytearray(2), MinInterval=bytearray(2), MaxInterval=bytearray(2), Timeout=bytearray(2), Change=bytearray(1)):
            self.AttributeDirection = AttributeDirection
            self.AttributeType = AttributeType
            self.AttributeID = AttributeID
            self.MinInterval = MinInterval
            self.MaxInterval = MaxInterval
            self.Timeout = Timeout
            self.Change = Change

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndpoint=bytearray(1), ClusterID=bytearray(2), Direction=bytearray(1), ManufacturerSpecific=bytearray(1), ManufacturerID=bytearray(2), NumberOfAttributes=bytearray(1), AttributesList=[]):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndpoint: DestinationEndpoint
        @param ClusterID: ClusterID
        @param Direction: Direction
        @param ManufacturerSpecific: ManufacturerSpecific
        @param ManufacturerID: ManufacturerID
        @param NumberOfAttributes: NumberOfAttributes
        @param AttributesList: AttributesList
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndpoint = DestinationEndpoint
        self.ClusterID = ClusterID
        self.Direction = Direction
        self.ManufacturerSpecific = ManufacturerSpecific
        self.ManufacturerID = ManufacturerID
        self.NumberOfAttributes = NumberOfAttributes
        self.AttributesList = AttributesList


class AddGroup(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), GroupAddress=bytearray(2)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param GroupAddress: GroupAddress
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.GroupAddress = GroupAddress


class ViewGroup(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), GroupAddress=bytearray(2)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param GroupAddress: GroupAddress
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.GroupAddress = GroupAddress


class GetGroupMembership(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), GroupCount=bytearray(1), GroupList=[]):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param GroupCount: GroupCount
        @param GroupList: GroupList
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.GroupCount = GroupCount
        self.GroupList = GroupList


class RemoveGroup(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), GroupAddress=bytearray(2)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param GroupAddress: GroupAddress
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.GroupAddress = GroupAddress


class RemoveAllGroups(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint


class AddGroupIfIdentify(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), GroupAddress=bytearray(2)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param GroupAddress: GroupAddress
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.GroupAddress = GroupAddress


class IdentifySend(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), TimeInSecconds=bytearray(2)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param TimeInSecconds: TimeInSecconds
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.TimeInSecconds = TimeInSecconds


class IdentifyQuery(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint


class IdentifyTriggerEffects(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), EffectID=bytearray(1), EffectGradient=bytearray(1)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param EffectID: EffectID
        @param EffectGradient: EffectGradient
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.EffectID = EffectID
        self.EffectGradient = EffectGradient


class MoveToLevel(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), OnOff=bytearray(1), Mode=bytearray(1), Rate=bytearray(1)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param OnOff: OnOff
        @param Mode: Mode
        @param Rate: Rate
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.OnOff = OnOff
        self.Mode = Mode
        self.Rate = Rate


class MoveToLevelWithWithoutOnOff(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), OnOff=bytearray(1), Level=bytearray(1), TransitionTime=bytearray(2)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param OnOff: OnOff
        @param Level: Level
        @param TransitionTime: TransitionTime
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.OnOff = OnOff
        self.Level = Level
        self.TransitionTime = TransitionTime


class MoveStep(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), OnOff=bytearray(1), StepMode=bytearray(1), StepSize=bytearray(1), TransitionTime=bytearray(2)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param OnOff: OnOff
        @param StepMode: StepMode
        @param StepSize: StepSize
        @param TransitionTime: TransitionTime
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.OnOff = OnOff
        self.StepMode = StepMode
        self.StepSize = StepSize
        self.TransitionTime = TransitionTime


class MoveStopWithOnOff(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint


class OnOffWithEffectsSend(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), EffectID=bytearray(1), EffectGradient=bytearray(1)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param EffectID: EffectID
        @param EffectGradient: EffectGradient
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.EffectID = EffectID
        self.EffectGradient = EffectGradient


class OnOffWithNoEffects(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), CommandID=OnOffWithNoEffectsCommandID.Off):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param CommandID: CommandID
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.CommandID = CommandID


class OnOffTimedSend(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), OnOff=OnOffTimedSendOnOff.Off, OnTimeInSecconds=bytearray(2), OffTimeInSecconds=bytearray(2)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param OnOff: OnOff
        @param OnTimeInSecconds: OnTimeInSecconds
        @param OffTimeInSecconds: OffTimeInSecconds
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.OnOff = OnOff
        self.OnTimeInSecconds = OnTimeInSecconds
        self.OffTimeInSecconds = OffTimeInSecconds


class ViewScene(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), GroupID=bytearray(2), SceneID=bytearray(1)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param GroupID: GroupID
        @param SceneID: SceneID
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.GroupID = GroupID
        self.SceneID = SceneID


class AddScene(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), GroupID=bytearray(2), SceneID=bytearray(1), TransitionTime=bytearray(2), SceneNameLength=bytearray(1), SceneNameMaxLength=bytearray(1), SceneNameData=[]):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param GroupID: GroupID
        @param SceneID: SceneID
        @param TransitionTime: TransitionTime
        @param SceneNameLength: SceneNameLength
        @param SceneNameMaxLength: SceneNameMaxLength
        @param SceneNameData: SceneNameData
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.GroupID = GroupID
        self.SceneID = SceneID
        self.TransitionTime = TransitionTime
        self.SceneNameLength = SceneNameLength
        self.SceneNameMaxLength = SceneNameMaxLength
        self.SceneNameData = SceneNameData


class RemoveScene(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), GroupID=bytearray(2), SceneID=bytearray(1)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param GroupID: GroupID
        @param SceneID: SceneID
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.GroupID = GroupID
        self.SceneID = SceneID


class RemoveAllScenes(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), GroupID=bytearray(2)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param GroupID: GroupID
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.GroupID = GroupID


class StoreScene(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), GroupID=bytearray(2), SceneID=bytearray(1)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param GroupID: GroupID
        @param SceneID: SceneID
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.GroupID = GroupID
        self.SceneID = SceneID


class RecallScene(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), GroupID=bytearray(2), SceneID=bytearray(1)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param GroupID: GroupID
        @param SceneID: SceneID
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.GroupID = GroupID
        self.SceneID = SceneID


class SceneMembershipRequest(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), GroupID=bytearray(2)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param GroupID: GroupID
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.GroupID = GroupID


class AddEnhancedScene(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), GroupID=bytearray(2), SceneID=bytearray(1), TransitionTime=bytearray(2), SceneNameLength=bytearray(1), SceneNameData=[], Length=bytearray(1), MaxLength=bytearray(1), ExtensionFieldSets=[]):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param GroupID: GroupID
        @param SceneID: SceneID
        @param TransitionTime: TransitionTime
        @param SceneNameLength: SceneNameLength
        @param SceneNameData: SceneNameData
        @param Length: Length
        @param MaxLength: MaxLength
        @param ExtensionFieldSets: ExtensionFieldSets
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.GroupID = GroupID
        self.SceneID = SceneID
        self.TransitionTime = TransitionTime
        self.SceneNameLength = SceneNameLength
        self.SceneNameData = SceneNameData
        self.Length = Length
        self.MaxLength = MaxLength
        self.ExtensionFieldSets = ExtensionFieldSets


class ViewEnhancedScene(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), GroupID=bytearray(2), SceneID=bytearray(1)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param GroupID: GroupID
        @param SceneID: SceneID
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.GroupID = GroupID
        self.SceneID = SceneID


class CopyScene(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), Mode=bytearray(1), FromGroupID=bytearray(2), FromSceneID=bytearray(1), ToGroupID=bytearray(2), ToSceneID=bytearray(1)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param Mode: Mode
        @param FromGroupID: FromGroupID
        @param FromSceneID: FromSceneID
        @param ToGroupID: ToGroupID
        @param ToSceneID: ToSceneID
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.Mode = Mode
        self.FromGroupID = FromGroupID
        self.FromSceneID = FromSceneID
        self.ToGroupID = ToGroupID
        self.ToSceneID = ToSceneID


class MoveToHue(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), Hue=bytearray(1), Direction=bytearray(1), TransitionTime=bytearray(2)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param Hue: Hue
        @param Direction: Direction
        @param TransitionTime: TransitionTime
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.Hue = Hue
        self.Direction = Direction
        self.TransitionTime = TransitionTime


class MoveHue(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), Mode=bytearray(1), Rate=bytearray(1)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param Mode: Mode
        @param Rate: Rate
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.Mode = Mode
        self.Rate = Rate


class StepHue(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), Mode=bytearray(1), StepSize=bytearray(1), TransitionTime=bytearray(1)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param Mode: Mode
        @param StepSize: StepSize
        @param TransitionTime: TransitionTime
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.Mode = Mode
        self.StepSize = StepSize
        self.TransitionTime = TransitionTime


class MoveToSaturation(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), Saturation=bytearray(1), TransitionTime=bytearray(2)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param Saturation: Saturation
        @param TransitionTime: TransitionTime
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.Saturation = Saturation
        self.TransitionTime = TransitionTime


class MoveSaturation(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), Mode=bytearray(1), Rate=bytearray(2)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param Mode: Mode
        @param Rate: Rate
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.Mode = Mode
        self.Rate = Rate


class StepSaturation(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), Mode=bytearray(1), StepSize=bytearray(1), TransitionTime=bytearray(1)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param Mode: Mode
        @param StepSize: StepSize
        @param TransitionTime: TransitionTime
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.Mode = Mode
        self.StepSize = StepSize
        self.TransitionTime = TransitionTime


class MoveToHueAndSaturation(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), Hue=bytearray(1), Saturation=bytearray(1), TransitionTime=bytearray(1)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param Hue: Hue
        @param Saturation: Saturation
        @param TransitionTime: TransitionTime
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.Hue = Hue
        self.Saturation = Saturation
        self.TransitionTime = TransitionTime


class MoveToColour(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), ColorX=bytearray(2), ColorY=bytearray(2), TransitionTime=bytearray(2)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param ColorX: ColorX
        @param ColorY: ColorY
        @param TransitionTime: TransitionTime
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.ColorX = ColorX
        self.ColorY = ColorY
        self.TransitionTime = TransitionTime


class MoveColour(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), ColorX=bytearray(2), ColorY=bytearray(2)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param ColorX: ColorX
        @param ColorY: ColorY
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.ColorX = ColorX
        self.ColorY = ColorY


class EnhancedMoveToHue(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), Direction=bytearray(1), EnhancedHue=bytearray(2), TransitionTime=bytearray(2)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param Direction: Direction
        @param EnhancedHue: EnhancedHue
        @param TransitionTime: TransitionTime
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.Direction = Direction
        self.EnhancedHue = EnhancedHue
        self.TransitionTime = TransitionTime


class EnhancedMoveHue(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), Mode=bytearray(1), Rate=bytearray(2)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param Mode: Mode
        @param Rate: Rate
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.Mode = Mode
        self.Rate = Rate


class EnhancedStepHue(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), Mode=bytearray(1), StepSize=bytearray(2), TransitionTime=bytearray(2)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param Mode: Mode
        @param StepSize: StepSize
        @param TransitionTime: TransitionTime
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.Mode = Mode
        self.StepSize = StepSize
        self.TransitionTime = TransitionTime


class EnhancedMoveToHueAndSaturation(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), Saturation=bytearray(1), EnhancedHue=bytearray(2), TransitionTime=bytearray(2)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param Saturation: Saturation
        @param EnhancedHue: EnhancedHue
        @param TransitionTime: TransitionTime
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.Saturation = Saturation
        self.EnhancedHue = EnhancedHue
        self.TransitionTime = TransitionTime


class ColourLoopSet(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), UpdateFlags=bytearray(1), Action=bytearray(1), Direction=bytearray(1), Time=bytearray(1), StartHue=bytearray(4)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param UpdateFlags: UpdateFlags
        @param Action: Action
        @param Direction: Direction
        @param Time: Time
        @param StartHue: StartHue
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.UpdateFlags = UpdateFlags
        self.Action = Action
        self.Direction = Direction
        self.Time = Time
        self.StartHue = StartHue


class StopMoveStep(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), OptionsMask=bytearray(1), OptionsOverride=bytearray(1)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param OptionsMask: OptionsMask
        @param OptionsOverride: OptionsOverride
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.OptionsMask = OptionsMask
        self.OptionsOverride = OptionsOverride


class MoveToColourTemperature(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), ColourTemperature=bytearray(2), TransitionTime=bytearray(2)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param ColourTemperature: ColourTemperature
        @param TransitionTime: TransitionTime
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.ColourTemperature = ColourTemperature
        self.TransitionTime = TransitionTime


class MoveColourTemperature(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), Mode=bytearray(1), Rate=bytearray(2), MinimumTemperature=bytearray(2), MaximumTemperature=bytearray(2), OptionsMask=bytearray(1), OptionsOverride=bytearray(1)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param Mode: Mode
        @param Rate: Rate
        @param MinimumTemperature: MinimumTemperature
        @param MaximumTemperature: MaximumTemperature
        @param OptionsMask: OptionsMask
        @param OptionsOverride: OptionsOverride
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.Mode = Mode
        self.Rate = Rate
        self.MinimumTemperature = MinimumTemperature
        self.MaximumTemperature = MaximumTemperature
        self.OptionsMask = OptionsMask
        self.OptionsOverride = OptionsOverride


class StepColourTemperature(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), Mode=bytearray(1), StepSize=bytearray(2), TransitionTime=bytearray(2), MinimumTemperature=bytearray(2), MaximumTemperature=bytearray(2), OptionsMask=bytearray(1), OptionsOverride=bytearray(1)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param Mode: Mode
        @param StepSize: StepSize
        @param TransitionTime: TransitionTime
        @param MinimumTemperature: MinimumTemperature
        @param MaximumTemperature: MaximumTemperature
        @param OptionsMask: OptionsMask
        @param OptionsOverride: OptionsOverride
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.Mode = Mode
        self.StepSize = StepSize
        self.TransitionTime = TransitionTime
        self.MinimumTemperature = MinimumTemperature
        self.MaximumTemperature = MaximumTemperature
        self.OptionsMask = OptionsMask
        self.OptionsOverride = OptionsOverride


class InitiateTouchlink(object):

    pass


class TouchLinkFactoryResetTarget(object):

    pass


class LockUnlockDoor(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), LockUnlock=LockUnlockDoorLockUnlock.Lock):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param LockUnlock: LockUnlock
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.LockUnlock = LockUnlock


class IASZoneEnrollResponse(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), EnrollResponseCode=bytearray(1), ZoneID=bytearray(1)):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param EnrollResponseCode: EnrollResponseCode
        @param ZoneID: ZoneID
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.EnrollResponseCode = EnrollResponseCode
        self.ZoneID = ZoneID


class RawAPSDataRequest(object):

    def __init__(self, AddressMode=bytearray(1), TargetShortAddress=bytearray(2), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), ClusterID=bytearray(2), ProfileID=bytearray(2), SecurityMode=bytearray(1), Radius=bytearray(1), DataLength=bytearray(1), Data=[]):
        '''
        @param AddressMode: AddressMode
        @param TargetShortAddress: TargetShortAddress
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param ClusterID: ClusterID
        @param ProfileID: ProfileID
        @param SecurityMode: SecurityMode
        @param Radius: Radius
        @param DataLength: DataLength
        @param Data: Data
        '''
        self.AddressMode = AddressMode
        self.TargetShortAddress = TargetShortAddress
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.ClusterID = ClusterID
        self.ProfileID = ProfileID
        self.SecurityMode = SecurityMode
        self.Radius = Radius
        self.DataLength = DataLength
        self.Data = Data


class Status(object):

    def __init__(self, Status=StatusStatus.Success, SeqNum=bytearray(1), PacketType=bytearray(2)):
        '''
        @param Status: Status
        @param SeqNum: SeqNum
        @param PacketType: PacketType
        '''
        self.Status = Status
        self.SeqNum = SeqNum
        self.PacketType = PacketType


class LogMessage(object):

    def __init__(self, LogLevel=LogMessageLogLevel.Emergency, LogMessage=[]):
        '''
        @param LogLevel: Use the Linux / Unix log levels
        @param LogMessage: LogMessage
        '''
        self.LogLevel = LogLevel
        self.LogMessage = LogMessage


class DataIndication(object):

    def __init__(self, Status=bytearray(1), ProfileID=bytearray(2), ClusterID=bytearray(2), SourceEndpoint=bytearray(1), DestinationEndpoint=bytearray(1), SourceAddressMode=DataIndicationSourceAddressMode.NO_PAN, SourceAddress=[], DestAddressingMode=DataIndicationDestAddressingMode.NO_PAN, DestinationAddress=[], PayloadSize=bytearray(1), Payload=[]):
        '''
        @param Status: Status
        @param ProfileID: ProfileID
        @param ClusterID: ClusterID
        @param SourceEndpoint: SourceEndpoint
        @param DestinationEndpoint: DestinationEndpoint
        @param SourceAddressMode: SourceAddressMode
        @param SourceAddress: MHR, Addressing fields. 0xFFFF Broadcast PAN identifier.
        @param DestAddressingMode: DestAddressingMode
        @param DestinationAddress: MHR, Addressing fields. 0xFFFF Broadcast PAN identifier.
        @param PayloadSize: PayloadSize
        @param Payload: Payload
        '''
        self.Status = Status
        self.ProfileID = ProfileID
        self.ClusterID = ClusterID
        self.SourceEndpoint = SourceEndpoint
        self.DestinationEndpoint = DestinationEndpoint
        self.SourceAddressMode = SourceAddressMode
        self.SourceAddress = SourceAddress
        self.DestAddressingMode = DestAddressingMode
        self.DestinationAddress = DestinationAddress
        self.PayloadSize = PayloadSize
        self.Payload = Payload


class NodeClusterList(object):

    def __init__(self, SourceEndPoint=bytearray(1), ProfileID=bytearray(2), ClusterList=bytearray(2)):
        '''
        @param SourceEndPoint: SourceEndPoint
        @param ProfileID: ProfileID
        @param ClusterList: ClusterList
        '''
        self.SourceEndPoint = SourceEndPoint
        self.ProfileID = ProfileID
        self.ClusterList = ClusterList


class NodeClusterAttributeList(object):

    def __init__(self, SourceEndPoint=bytearray(1), ProfileID=bytearray(2), ClusterID=bytearray(2), AttributeList=bytearray(2)):
        '''
        @param SourceEndPoint: SourceEndPoint
        @param ProfileID: ProfileID
        @param ClusterID: ClusterID
        @param AttributeList: AttributeList
        '''
        self.SourceEndPoint = SourceEndPoint
        self.ProfileID = ProfileID
        self.ClusterID = ClusterID
        self.AttributeList = AttributeList


class NodeCommandIDList(object):

    def __init__(self, SourceEndPoint=bytearray(1), ProfileID=bytearray(2), ClusterID=bytearray(2), CommandIDList=bytearray(1)):
        '''
        @param SourceEndPoint: SourceEndPoint
        @param ProfileID: ProfileID
        @param ClusterID: ClusterID
        @param CommandIDList: CommandIDList
        '''
        self.SourceEndPoint = SourceEndPoint
        self.ProfileID = ProfileID
        self.ClusterID = ClusterID
        self.CommandIDList = CommandIDList


class VersionList(object):

    def __init__(self, MajorVersionNumber=bytearray(2), InstallerVersionNumber=bytearray(2)):
        '''
        @param MajorVersionNumber: MajorVersionNumber
        @param InstallerVersionNumber: InstallerVersionNumber
        '''
        self.MajorVersionNumber = MajorVersionNumber
        self.InstallerVersionNumber = InstallerVersionNumber


class NetworkJoinedFormed(object):

    def __init__(self, Status=NetworkJoinedFormedStatus.JoinedExistingNetwork, ShortAddr=bytearray(2), ExtAddr=bytearray(8), Channel=bytearray(1)):
        '''
        @param Status: Status
        @param ShortAddr: Short Address
        @param ExtAddr: Extended Address
        @param Channel: Channel
        '''
        self.Status = Status
        self.ShortAddr = ShortAddr
        self.ExtAddr = ExtAddr
        self.Channel = Channel


class PermitJoinStatusResponse(object):

    def __init__(self, Status=PermitJoinStatusResponseStatus.Off):
        '''
        @param Status: Status
        '''
        self.Status = Status


class NonFactoryNewRestart(object):

    def __init__(self, Status=NonFactoryNewRestartStatus.STARTUP):
        '''
        @param Status: Status
        '''
        self.Status = Status


class FactoryNewRestart(object):

    def __init__(self, Status=FactoryNewRestartStatus.STARTUP):
        '''
        @param Status: Status
        '''
        self.Status = Status


class BindResponse(object):

    def __init__(self, SequenceNumber=bytearray(1), Status=bytearray(1)):
        '''
        @param SequenceNumber: SequenceNumber
        @param Status: Status
        '''
        self.SequenceNumber = SequenceNumber
        self.Status = Status


class UnbindResponse(object):

    def __init__(self, SequenceNumber=bytearray(1), Status=bytearray(1)):
        '''
        @param SequenceNumber: SequenceNumber
        @param Status: Status
        '''
        self.SequenceNumber = SequenceNumber
        self.Status = Status


class NetworkAddressResponse(object):

    def __init__(self, SequenceNumber=bytearray(1), Status=bytearray(1), IEEEAddress=bytearray(8), ShortAddress=bytearray(2), NumberOfAssociatedDevices=bytearray(1), StartIndex=bytearray(1), DeviceList=bytearray(2)):
        '''
        @param SequenceNumber: SequenceNumber
        @param Status: Status
        @param IEEEAddress: IEEEAddress
        @param ShortAddress: ShortAddress
        @param NumberOfAssociatedDevices: NumberOfAssociatedDevices
        @param StartIndex: StartIndex
        @param DeviceList: DeviceList
        '''
        self.SequenceNumber = SequenceNumber
        self.Status = Status
        self.IEEEAddress = IEEEAddress
        self.ShortAddress = ShortAddress
        self.NumberOfAssociatedDevices = NumberOfAssociatedDevices
        self.StartIndex = StartIndex
        self.DeviceList = DeviceList


class IEEEAddressResponse(object):

    def __init__(self, SequenceNumber=bytearray(1), Status=bytearray(1), IEEEAddress=bytearray(8), shortAddress=bytearray(2), NumberOfAssociatedDevices=bytearray(1), StartIndex=bytearray(1), DeviceList=[]):
        '''
        @param SequenceNumber: SequenceNumber
        @param Status: Status
        @param IEEEAddress: IEEEAddress
        @param shortAddress: shortAddress
        @param NumberOfAssociatedDevices: NumberOfAssociatedDevices
        @param StartIndex: StartIndex
        @param DeviceList: DeviceList
        '''
        self.SequenceNumber = SequenceNumber
        self.Status = Status
        self.IEEEAddress = IEEEAddress
        self.shortAddress = shortAddress
        self.NumberOfAssociatedDevices = NumberOfAssociatedDevices
        self.StartIndex = StartIndex
        self.DeviceList = DeviceList


class NodeDescriptorResponse(object):

    def __init__(self, SequenceNumber=bytearray(1), Status=bytearray(1), NetworkAddress=bytearray(2), ManufacturerCode=bytearray(2), MaxRxSize=bytearray(2), MaxTxSize=bytearray(2), ServerMask=bytearray(2), DescriptorCapability=bytearray(1), MacCapability=bytearray(1), MaxBufferSize=bytearray(1), BitFields=bytearray(2)):
        '''
        @param SequenceNumber: SequenceNumber
        @param Status: Status
        @param NetworkAddress: NetworkAddress
        @param ManufacturerCode: ManufacturerCode
        @param MaxRxSize: MaxRxSize
        @param MaxTxSize: MaxTxSize
        @param ServerMask: ServerMask
        @param DescriptorCapability: DescriptorCapability
        @param MacCapability: MacCapability
        @param MaxBufferSize: MaxBufferSize
        @param BitFields: BitFields
        '''
        self.SequenceNumber = SequenceNumber
        self.Status = Status
        self.NetworkAddress = NetworkAddress
        self.ManufacturerCode = ManufacturerCode
        self.MaxRxSize = MaxRxSize
        self.MaxTxSize = MaxTxSize
        self.ServerMask = ServerMask
        self.DescriptorCapability = DescriptorCapability
        self.MacCapability = MacCapability
        self.MaxBufferSize = MaxBufferSize
        self.BitFields = BitFields


class SimpleDescriptorResponse(object):

    class BitFields(object):

        def __init__(self, DeviceVersion=bytearray(4), Reserved=bytearray(4)):
            self.DeviceVersion = DeviceVersion
            self.Reserved = Reserved

    def __init__(self, SequenceNumber=bytearray(1), Status=bytearray(1), NwkAddress=bytearray(2), Length=bytearray(1), EndPoint=bytearray(1), Profile=bytearray(2), DeviceID=bytearray(2), BitFields=BitFields(1), InClusterCount=bytearray(1), InClusterList=[], OutClusterCount=bytearray(1), OutClusterList=[]):
        '''
        @param SequenceNumber: SequenceNumber
        @param Status: Status
        @param NwkAddress: NwkAddress
        @param Length: Length
        @param EndPoint: EndPoint
        @param Profile: Profile
        @param DeviceID: DeviceID
        @param BitFields: BitFields
        @param InClusterCount: InClusterCount
        @param InClusterList: InClusterList
        @param OutClusterCount: OutClusterCount
        @param OutClusterList: OutClusterList
        '''
        self.SequenceNumber = SequenceNumber
        self.Status = Status
        self.NwkAddress = NwkAddress
        self.Length = Length
        self.EndPoint = EndPoint
        self.Profile = Profile
        self.DeviceID = DeviceID
        self.BitFields = BitFields
        self.InClusterCount = InClusterCount
        self.InClusterList = InClusterList
        self.OutClusterCount = OutClusterCount
        self.OutClusterList = OutClusterList


class PowerDescriptorResponse(object):

    class BitFields(object):

        def __init__(self, CurrentPowerMode=bytearray(4), AvailablePowerSource=bytearray(4), CurrentPowerSource=bytearray(4), CurrentPowerSourceLevel=bytearray(4)):
            self.CurrentPowerMode = CurrentPowerMode
            self.AvailablePowerSource = AvailablePowerSource
            self.CurrentPowerSource = CurrentPowerSource
            self.CurrentPowerSourceLevel = CurrentPowerSourceLevel

    def __init__(self, SequenceNumber=bytearray(1), Status=bytearray(1), BitFields=BitFields(2)):
        '''
        @param SequenceNumber: SequenceNumber
        @param Status: Status
        @param BitFields: BitFields
        '''
        self.SequenceNumber = SequenceNumber
        self.Status = Status
        self.BitFields = BitFields


class ActiveEndpointResponse(object):

    def __init__(self, SequenceNumber=bytearray(1), Status=bytearray(1), Address=bytearray(2), EndpointCount=bytearray(1), ActiveEndPointList=[]):
        '''
        @param SequenceNumber: SequenceNumber
        @param Status: Status
        @param Address: Address
        @param EndpointCount: EndpointCount
        @param ActiveEndPointList: ActiveEndPointList
        '''
        self.SequenceNumber = SequenceNumber
        self.Status = Status
        self.Address = Address
        self.EndpointCount = EndpointCount
        self.ActiveEndPointList = ActiveEndPointList


class MatchDescriptorResponse(object):

    def __init__(self, SequenceNumber=bytearray(1), Status=bytearray(1), NetworkAddress=bytearray(2), LengthOfList=bytearray(1), MatchList=[]):
        '''
        @param SequenceNumber: SequenceNumber
        @param Status: Status
        @param NetworkAddress: NetworkAddress
        @param LengthOfList: LengthOfList
        @param MatchList: MatchList
        '''
        self.SequenceNumber = SequenceNumber
        self.Status = Status
        self.NetworkAddress = NetworkAddress
        self.LengthOfList = LengthOfList
        self.MatchList = MatchList


class DeviceAnnounce(object):

    class MACCapability(object):

        def __init__(self, AlternatePANCoordinator=bytearray(1), DeviceType=bytearray(1), PowerSource=bytearray(1), ReceiverOnWhenIdle=bytearray(1), Reserved=bytearray(2), SecurityCapability=bytearray(1), AllocateAddress=bytearray(1)):
            self.AlternatePANCoordinator = AlternatePANCoordinator
            self.DeviceType = DeviceType
            self.PowerSource = PowerSource
            self.ReceiverOnWhenIdle = ReceiverOnWhenIdle
            self.Reserved = Reserved
            self.SecurityCapability = SecurityCapability
            self.AllocateAddress = AllocateAddress

    def __init__(self, ShortAddress=bytearray(2), IEEEAddress=bytearray(8), MACCapability=MACCapability(1)):
        '''
        @param ShortAddress: ShortAddress
        @param IEEEAddress: IEEEAddress
        @param MACCapability: MACCapability
        '''
        self.ShortAddress = ShortAddress
        self.IEEEAddress = IEEEAddress
        self.MACCapability = MACCapability


class UserDescriptorResponse(object):

    def __init__(self, SequenceNumber=bytearray(1), Status=bytearray(1), NetworkAddressOfOnterest=bytearray(2), LengthOfData=bytearray(1), DataList=[]):
        '''
        @param SequenceNumber: SequenceNumber
        @param Status: Status
        @param NetworkAddressOfOnterest: NetworkAddressOfOnterest
        @param LengthOfData: LengthOfData
        @param DataList: DataList
        '''
        self.SequenceNumber = SequenceNumber
        self.Status = Status
        self.NetworkAddressOfOnterest = NetworkAddressOfOnterest
        self.LengthOfData = LengthOfData
        self.DataList = DataList


class UserDescriptorNotify(object):

    def __init__(self, SequenceNumber=bytearray(1), Status=bytearray(1), NetworkAddressOfOnterest=bytearray(2)):
        '''
        @param SequenceNumber: SequenceNumber
        @param Status: Status
        @param NetworkAddressOfOnterest: NetworkAddressOfOnterest
        '''
        self.SequenceNumber = SequenceNumber
        self.Status = Status
        self.NetworkAddressOfOnterest = NetworkAddressOfOnterest


class ComplexDescriptorResponse(object):

    def __init__(self, SequenceNumber=bytearray(1), Status=bytearray(1), NetworkAddressOfOnterest=bytearray(2), Length=bytearray(1), XMLTag=bytearray(1), FieldCount=bytearray(1), FieldValues=[]):
        '''
        @param SequenceNumber: SequenceNumber
        @param Status: Status
        @param NetworkAddressOfOnterest: NetworkAddressOfOnterest
        @param Length: Length
        @param XMLTag: XMLTag
        @param FieldCount: FieldCount
        @param FieldValues: FieldValues
        '''
        self.SequenceNumber = SequenceNumber
        self.Status = Status
        self.NetworkAddressOfOnterest = NetworkAddressOfOnterest
        self.Length = Length
        self.XMLTag = XMLTag
        self.FieldCount = FieldCount
        self.FieldValues = FieldValues


class ManagementLeaveResponse(object):

    def __init__(self, SequenceNumber=bytearray(1), Status=bytearray(1)):
        '''
        @param SequenceNumber: SequenceNumber
        @param Status: Status
        '''
        self.SequenceNumber = SequenceNumber
        self.Status = Status


class LeaveIndication(object):

    def __init__(self, ExtendedAddress=bytearray(8), RejoinStatus=bytearray(1)):
        '''
        @param ExtendedAddress: ExtendedAddress
        @param RejoinStatus: RejoinStatus
        '''
        self.ExtendedAddress = ExtendedAddress
        self.RejoinStatus = RejoinStatus


class ManagementNetworkUpdateResponse(object):

    def __init__(self, SequenceNumber=bytearray(1), Status=bytearray(1), TotalTransmission=bytearray(2), TransmissionFailures=bytearray(2), ScannedChannels=bytearray(4), ScannedChannelListCount=bytearray(1), ChannelList=bytearray(1)):
        '''
        @param SequenceNumber: SequenceNumber
        @param Status: Status
        @param TotalTransmission: TotalTransmission
        @param TransmissionFailures: TransmissionFailures
        @param ScannedChannels: ScannedChannels
        @param ScannedChannelListCount: ScannedChannelListCount
        @param ChannelList: ChannelList
        '''
        self.SequenceNumber = SequenceNumber
        self.Status = Status
        self.TotalTransmission = TotalTransmission
        self.TransmissionFailures = TransmissionFailures
        self.ScannedChannels = ScannedChannels
        self.ScannedChannelListCount = ScannedChannelListCount
        self.ChannelList = ChannelList


class SystemServerDiscoveryResponse(object):

    def __init__(self, SequenceNumber=bytearray(1), Status=bytearray(1), ServerMask=bytearray(2)):
        '''
        @param SequenceNumber: SequenceNumber
        @param Status: Status
        @param ServerMask: ServerMask
        '''
        self.SequenceNumber = SequenceNumber
        self.Status = Status
        self.ServerMask = ServerMask


class ManagementLQIResponse(object):

    class ListOfEntries_BitMapOfAttributes(object):

        def __init__(self, DeviceType=ListOfEntries_BitMapOfAttributesDeviceType.Coordinator, PermitJoinStatus=ListOfEntries_BitMapOfAttributesPermitJoinStatus.Off, Relationship=ListOfEntries_BitMapOfAttributesRelationship.Parent, RxOnWhenIdleStatus=ListOfEntries_BitMapOfAttributesRxOnWhenIdleStatus.Off):
            self.DeviceType = DeviceType
            self.PermitJoinStatus = PermitJoinStatus
            self.Relationship = Relationship
            self.RxOnWhenIdleStatus = RxOnWhenIdleStatus

    def __init__(self, SequenceNumber=bytearray(1), Status=bytearray(1), NeighbourTableEntries=bytearray(1), NeighbourTableListCount=bytearray(1), StartIndex=bytearray(1), ListOfEntries_NWKAddress=bytearray(2), ListOfEntries_ExtendedPANID=bytearray(8), ListOfEntries_IEEEAddress=bytearray(8), ListOfEntries_Depth=bytearray(1), ListOfEntries_LinkQuality=bytearray(1), ListOfEntries_BitMapOfAttributes=ListOfEntries_BitMapOfAttributes(1)):
        '''
        @param SequenceNumber: SequenceNumber
        @param Status: Status
        @param NeighbourTableEntries: NeighbourTableEntries
        @param NeighbourTableListCount: NeighbourTableListCount
        @param StartIndex: StartIndex
        @param ListOfEntries_NWKAddress: NWKAddress
        @param ListOfEntries_ExtendedPANID: ExtendedPANID
        @param ListOfEntries_IEEEAddress: IEEEAddress
        @param ListOfEntries_Depth: Depth
        @param ListOfEntries_LinkQuality: LinkQuality
        @param ListOfEntries_BitMapOfAttributes: BitMapOfAttributes
        '''
        self.SequenceNumber = SequenceNumber
        self.Status = Status
        self.NeighbourTableEntries = NeighbourTableEntries
        self.NeighbourTableListCount = NeighbourTableListCount
        self.StartIndex = StartIndex
        self.ListOfEntries_NWKAddress = ListOfEntries_NWKAddress
        self.ListOfEntries_ExtendedPANID = ListOfEntries_ExtendedPANID
        self.ListOfEntries_IEEEAddress = ListOfEntries_IEEEAddress
        self.ListOfEntries_Depth = ListOfEntries_Depth
        self.ListOfEntries_LinkQuality = ListOfEntries_LinkQuality
        self.ListOfEntries_BitMapOfAttributes = ListOfEntries_BitMapOfAttributes


class AttributeDiscoveryResponse(object):

    def __init__(self, Complete=AttributeDiscoveryResponseComplete.MoreAttributesToFollow, AttributeType=bytearray(1), AttributeID=bytearray(2)):
        '''
        @param Complete: Complete
        @param AttributeType: AttributeType
        @param AttributeID: AttributeID
        '''
        self.Complete = Complete
        self.AttributeType = AttributeType
        self.AttributeID = AttributeID


class AuthenticateResponse(object):

    def __init__(self, IEEEAddressOfTtheGateway=bytearray(8), EncryptedKey=bytearray(2), MIC=bytearray(1), IEEEAddressOfTheInitiatingNode=bytearray(8), ActiveKeySequenceNumber=bytearray(1), Channel=bytearray(1), ShortPANID=bytearray(2), ExtendedPANID=bytearray(8)):
        '''
        @param IEEEAddressOfTtheGateway: IEEEAddressOfTtheGateway
        @param EncryptedKey: EncryptedKey
        @param MIC: MIC
        @param IEEEAddressOfTheInitiatingNode: IEEEAddressOfTheInitiatingNode
        @param ActiveKeySequenceNumber: ActiveKeySequenceNumber
        @param Channel: Channel
        @param ShortPANID: ShortPANID
        @param ExtendedPANID: ExtendedPANID
        '''
        self.IEEEAddressOfTtheGateway = IEEEAddressOfTtheGateway
        self.EncryptedKey = EncryptedKey
        self.MIC = MIC
        self.IEEEAddressOfTheInitiatingNode = IEEEAddressOfTheInitiatingNode
        self.ActiveKeySequenceNumber = ActiveKeySequenceNumber
        self.Channel = Channel
        self.ShortPANID = ShortPANID
        self.ExtendedPANID = ExtendedPANID


class CodeResponse(object):

    def __init__(self, Status=bytearray(1)):
        '''
        @param Status: Status
        '''
        self.Status = Status


class ConfigureReportingResponse(object):

    def __init__(self, SequenceNumber=bytearray(1), SrcAddress=bytearray(2), Endpoint=bytearray(1), ClusterID=bytearray(2), Status=bytearray(1)):
        '''
        @param SequenceNumber: SequenceNumber
        @param SrcAddress: SrcAddress
        @param Endpoint: Endpoint
        @param ClusterID: ClusterID
        @param Status: Status
        '''
        self.SequenceNumber = SequenceNumber
        self.SrcAddress = SrcAddress
        self.Endpoint = Endpoint
        self.ClusterID = ClusterID
        self.Status = Status


class ReadIndividualAttributeResponse(object):

    def __init__(self, SequenceNumber=bytearray(1), SrcAddress=bytearray(2), Endpoint=bytearray(1), ClusterID=bytearray(2), AttributeEnum=bytearray(2), AttributeStatus=bytearray(1), AttributeDataType=bytearray(1), SizeOfTheAttributes=bytearray(2), DataByteList=[]):
        '''
        @param SequenceNumber: SequenceNumber
        @param SrcAddress: SrcAddress
        @param Endpoint: Endpoint
        @param ClusterID: ClusterID
        @param AttributeEnum: AttributeEnum
        @param AttributeStatus: AttributeStatus
        @param AttributeDataType: AttributeDataType
        @param SizeOfTheAttributes: SizeOfTheAttributes
        @param DataByteList: DataByteList
        '''
        self.SequenceNumber = SequenceNumber
        self.SrcAddress = SrcAddress
        self.Endpoint = Endpoint
        self.ClusterID = ClusterID
        self.AttributeEnum = AttributeEnum
        self.AttributeStatus = AttributeStatus
        self.AttributeDataType = AttributeDataType
        self.SizeOfTheAttributes = SizeOfTheAttributes
        self.DataByteList = DataByteList


class WriteAttributeResponse(object):

    def __init__(self, SequenceNumber=bytearray(1), SrcAddress=bytearray(2), Endpoint=bytearray(1), ClusterID=bytearray(2), AttributeEnum=bytearray(2), AttributeStatus=bytearray(1), AttributeDataType=bytearray(1), SizeOfTheAttributes=bytearray(2), DataByteList=[]):
        '''
        @param SequenceNumber: SequenceNumber
        @param SrcAddress: SrcAddress
        @param Endpoint: Endpoint
        @param ClusterID: ClusterID
        @param AttributeEnum: AttributeEnum
        @param AttributeStatus: AttributeStatus
        @param AttributeDataType: AttributeDataType
        @param SizeOfTheAttributes: SizeOfTheAttributes
        @param DataByteList: DataByteList
        '''
        self.SequenceNumber = SequenceNumber
        self.SrcAddress = SrcAddress
        self.Endpoint = Endpoint
        self.ClusterID = ClusterID
        self.AttributeEnum = AttributeEnum
        self.AttributeStatus = AttributeStatus
        self.AttributeDataType = AttributeDataType
        self.SizeOfTheAttributes = SizeOfTheAttributes
        self.DataByteList = DataByteList


class ReportIndividualAttributeResponse(object):

    def __init__(self, SequenceNumber=bytearray(1), SrcAddress=bytearray(2), Endpoint=bytearray(1), ClusterID=bytearray(2), AttributeEnum=bytearray(2), AttributeStatus=bytearray(1), AttributeDataType=bytearray(1), SizeOfTheAttributes=bytearray(2), DataByteList=[]):
        '''
        @param SequenceNumber: SequenceNumber
        @param SrcAddress: SrcAddress
        @param Endpoint: Endpoint
        @param ClusterID: ClusterID
        @param AttributeEnum: AttributeEnum
        @param AttributeStatus: AttributeStatus
        @param AttributeDataType: AttributeDataType
        @param SizeOfTheAttributes: SizeOfTheAttributes
        @param DataByteList: DataByteList
        '''
        self.SequenceNumber = SequenceNumber
        self.SrcAddress = SrcAddress
        self.Endpoint = Endpoint
        self.ClusterID = ClusterID
        self.AttributeEnum = AttributeEnum
        self.AttributeStatus = AttributeStatus
        self.AttributeDataType = AttributeDataType
        self.SizeOfTheAttributes = SizeOfTheAttributes
        self.DataByteList = DataByteList


class DefaultResponse(object):

    def __init__(self, SequenceNumber=bytearray(1), Endpoint=bytearray(1), ClusterID=bytearray(2), CommandID=bytearray(1), StatusCode=bytearray(1)):
        '''
        @param SequenceNumber: SequenceNumber
        @param Endpoint: Endpoint
        @param ClusterID: ClusterID
        @param CommandID: CommandID
        @param StatusCode: StatusCode
        '''
        self.SequenceNumber = SequenceNumber
        self.Endpoint = Endpoint
        self.ClusterID = ClusterID
        self.CommandID = CommandID
        self.StatusCode = StatusCode


class AddGroupResponse(object):

    def __init__(self, SequenceNumber=bytearray(1), Endpoint=bytearray(1), ClusterID=bytearray(2), Status=bytearray(1), GroupID=bytearray(2)):
        '''
        @param SequenceNumber: SequenceNumber
        @param Endpoint: Endpoint
        @param ClusterID: ClusterID
        @param Status: Status
        @param GroupID: GroupID
        '''
        self.SequenceNumber = SequenceNumber
        self.Endpoint = Endpoint
        self.ClusterID = ClusterID
        self.Status = Status
        self.GroupID = GroupID


class ViewGroupResponse(object):

    def __init__(self, SequenceNumber=bytearray(1), Endpoint=bytearray(1), ClusterID=bytearray(2), Status=bytearray(1), GroupID=bytearray(2)):
        '''
        @param SequenceNumber: SequenceNumber
        @param Endpoint: Endpoint
        @param ClusterID: ClusterID
        @param Status: Status
        @param GroupID: GroupID
        '''
        self.SequenceNumber = SequenceNumber
        self.Endpoint = Endpoint
        self.ClusterID = ClusterID
        self.Status = Status
        self.GroupID = GroupID


class GetGroupMembershipResponse(object):

    def __init__(self, SequenceNumber=bytearray(1), Endpoint=bytearray(1), ClusterID=bytearray(2), Capacity=bytearray(1), GroupCount=bytearray(1), GroupList=[]):
        '''
        @param SequenceNumber: SequenceNumber
        @param Endpoint: Endpoint
        @param ClusterID: ClusterID
        @param Capacity: Capacity
        @param GroupCount: GroupCount
        @param GroupList: GroupList
        '''
        self.SequenceNumber = SequenceNumber
        self.Endpoint = Endpoint
        self.ClusterID = ClusterID
        self.Capacity = Capacity
        self.GroupCount = GroupCount
        self.GroupList = GroupList


class RemoveGroupResponse(object):

    def __init__(self, SequenceNumber=bytearray(1), Endpoint=bytearray(1), ClusterID=bytearray(2), Status=bytearray(1), GroupID=bytearray(2)):
        '''
        @param SequenceNumber: SequenceNumber
        @param Endpoint: Endpoint
        @param ClusterID: ClusterID
        @param Status: Status
        @param GroupID: GroupID
        '''
        self.SequenceNumber = SequenceNumber
        self.Endpoint = Endpoint
        self.ClusterID = ClusterID
        self.Status = Status
        self.GroupID = GroupID


class ViewSceneResponse(object):

    def __init__(self, SequenceNumber=bytearray(1), Endpoint=bytearray(1), ClusterID=bytearray(2), Status=bytearray(1), GroupID=bytearray(2), SceneID=bytearray(1), TransitionTime=bytearray(2), SceneNameLength=bytearray(1), SceneNameMaxLength=bytearray(1), SceneNameData=[], ExtensionsLength=bytearray(2), ExtensionsMaxLength=bytearray(2), ExtensionsData=[]):
        '''
        @param SequenceNumber: SequenceNumber
        @param Endpoint: Endpoint
        @param ClusterID: ClusterID
        @param Status: Status
        @param GroupID: GroupID
        @param SceneID: SceneID
        @param TransitionTime: TransitionTime
        @param SceneNameLength: SceneNameLength
        @param SceneNameMaxLength: SceneNameMaxLength
        @param SceneNameData: SceneNameData
        @param ExtensionsLength: ExtensionsLength
        @param ExtensionsMaxLength: ExtensionsMaxLength
        @param ExtensionsData: ExtensionsData
        '''
        self.SequenceNumber = SequenceNumber
        self.Endpoint = Endpoint
        self.ClusterID = ClusterID
        self.Status = Status
        self.GroupID = GroupID
        self.SceneID = SceneID
        self.TransitionTime = TransitionTime
        self.SceneNameLength = SceneNameLength
        self.SceneNameMaxLength = SceneNameMaxLength
        self.SceneNameData = SceneNameData
        self.ExtensionsLength = ExtensionsLength
        self.ExtensionsMaxLength = ExtensionsMaxLength
        self.ExtensionsData = ExtensionsData


class ViewEnhancedSceneResponse(object):

    def __init__(self, SequenceNumber=bytearray(1), Endpoint=bytearray(1), ClusterID=bytearray(2), Status=bytearray(1), GroupID=bytearray(2), SceneID=bytearray(1), TransitionTime=bytearray(2), SceneNameLength=bytearray(1), SceneNameMaxLength=bytearray(1), SceneNameData=[], ExtensionsLength=bytearray(2), ExtensionsMaxLength=bytearray(2), ExtensionsData=[]):
        '''
        @param SequenceNumber: SequenceNumber
        @param Endpoint: Endpoint
        @param ClusterID: ClusterID
        @param Status: Status
        @param GroupID: GroupID
        @param SceneID: SceneID
        @param TransitionTime: TransitionTime
        @param SceneNameLength: SceneNameLength
        @param SceneNameMaxLength: SceneNameMaxLength
        @param SceneNameData: SceneNameData
        @param ExtensionsLength: ExtensionsLength
        @param ExtensionsMaxLength: ExtensionsMaxLength
        @param ExtensionsData: ExtensionsData
        '''
        self.SequenceNumber = SequenceNumber
        self.Endpoint = Endpoint
        self.ClusterID = ClusterID
        self.Status = Status
        self.GroupID = GroupID
        self.SceneID = SceneID
        self.TransitionTime = TransitionTime
        self.SceneNameLength = SceneNameLength
        self.SceneNameMaxLength = SceneNameMaxLength
        self.SceneNameData = SceneNameData
        self.ExtensionsLength = ExtensionsLength
        self.ExtensionsMaxLength = ExtensionsMaxLength
        self.ExtensionsData = ExtensionsData


class AddSceneResponse(object):

    def __init__(self, SequenceNumber=bytearray(1), Endpoint=bytearray(1), ClusterID=bytearray(2), Status=bytearray(1), GroupID=bytearray(2), SceneID=bytearray(1)):
        '''
        @param SequenceNumber: SequenceNumber
        @param Endpoint: Endpoint
        @param ClusterID: ClusterID
        @param Status: Status
        @param GroupID: GroupID
        @param SceneID: SceneID
        '''
        self.SequenceNumber = SequenceNumber
        self.Endpoint = Endpoint
        self.ClusterID = ClusterID
        self.Status = Status
        self.GroupID = GroupID
        self.SceneID = SceneID


class CopySceneResponse(object):

    def __init__(self, SequenceNumber=bytearray(1), Endpoint=bytearray(1), ClusterID=bytearray(2), Status=bytearray(1), FromGroupID=bytearray(2), FromSceneID=bytearray(1)):
        '''
        @param SequenceNumber: SequenceNumber
        @param Endpoint: Endpoint
        @param ClusterID: ClusterID
        @param Status: Status
        @param FromGroupID: FromGroupID
        @param FromSceneID: FromSceneID
        '''
        self.SequenceNumber = SequenceNumber
        self.Endpoint = Endpoint
        self.ClusterID = ClusterID
        self.Status = Status
        self.FromGroupID = FromGroupID
        self.FromSceneID = FromSceneID


class RemoveSceneResponse(object):

    def __init__(self, SequenceNumber=bytearray(1), Endpoint=bytearray(1), ClusterID=bytearray(2), Status=bytearray(1), GroupID=bytearray(2), SceneID=bytearray(1)):
        '''
        @param SequenceNumber: SequenceNumber
        @param Endpoint: Endpoint
        @param ClusterID: ClusterID
        @param Status: Status
        @param GroupID: GroupID
        @param SceneID: SceneID
        '''
        self.SequenceNumber = SequenceNumber
        self.Endpoint = Endpoint
        self.ClusterID = ClusterID
        self.Status = Status
        self.GroupID = GroupID
        self.SceneID = SceneID


class RemoveAllSceneResponse(object):

    def __init__(self, SequenceNumber=bytearray(1), Endpoint=bytearray(1), ClusterID=bytearray(2), Status=bytearray(1), GroupID=bytearray(2)):
        '''
        @param SequenceNumber: SequenceNumber
        @param Endpoint: Endpoint
        @param ClusterID: ClusterID
        @param Status: Status
        @param GroupID: GroupID
        '''
        self.SequenceNumber = SequenceNumber
        self.Endpoint = Endpoint
        self.ClusterID = ClusterID
        self.Status = Status
        self.GroupID = GroupID


class AddEnhancedSceneResponse(object):

    def __init__(self, SeqNum=bytearray(1), srcEndpoint=bytearray(1), ClusterId=bytearray(2), Status=bytearray(1), GroupID=bytearray(2), SceneID=bytearray(1)):
        '''
        @param SeqNum: SeqNum
        @param srcEndpoint: srcEndpoint
        @param ClusterId: ClusterId
        @param Status: Status
        @param GroupID: GroupID
        @param SceneID: SceneID
        '''
        self.SeqNum = SeqNum
        self.srcEndpoint = srcEndpoint
        self.ClusterId = ClusterId
        self.Status = Status
        self.GroupID = GroupID
        self.SceneID = SceneID


class StoreSceneResponse(object):

    def __init__(self, SequenceNumber=bytearray(1), Endpoint=bytearray(1), ClusterID=bytearray(2), Status=bytearray(1), GroupID=bytearray(2), SceneID=bytearray(1)):
        '''
        @param SequenceNumber: SequenceNumber
        @param Endpoint: Endpoint
        @param ClusterID: ClusterID
        @param Status: Status
        @param GroupID: GroupID
        @param SceneID: SceneID
        '''
        self.SequenceNumber = SequenceNumber
        self.Endpoint = Endpoint
        self.ClusterID = ClusterID
        self.Status = Status
        self.GroupID = GroupID
        self.SceneID = SceneID


class SceneMembershipResponse(object):

    def __init__(self, SequenceNumber=bytearray(1), Endpoint=bytearray(1), ClusterID=bytearray(2), Status=bytearray(1), Capacity=bytearray(1), GroupID=bytearray(2), SceneCount=bytearray(1), SceneList=[]):
        '''
        @param SequenceNumber: SequenceNumber
        @param Endpoint: Endpoint
        @param ClusterID: ClusterID
        @param Status: Status
        @param Capacity: Capacity
        @param GroupID: GroupID
        @param SceneCount: SceneCount
        @param SceneList: SceneList
        '''
        self.SequenceNumber = SequenceNumber
        self.Endpoint = Endpoint
        self.ClusterID = ClusterID
        self.Status = Status
        self.Capacity = Capacity
        self.GroupID = GroupID
        self.SceneCount = SceneCount
        self.SceneList = SceneList


class TouchlinkStatus(object):

    def __init__(self, Status=TouchlinkStatusStatus.Success, JoinedNodeShortAddress=bytearray(2)):
        '''
        @param Status: Status
        @param JoinedNodeShortAddress: JoinedNodeShortAddress
        '''
        self.Status = Status
        self.JoinedNodeShortAddress = JoinedNodeShortAddress


class ZoneStatusChangeNotification(object):

    def __init__(self, SequenceNumber=bytearray(1), EndPoint=bytearray(1), ClusteID=bytearray(2), SourceAddressMode=ZoneStatusChangeNotificationSourceAddressMode.NO_PAN, SourceAddress=[], ZoneStatus=bytearray(2), ExtendedStatus=bytearray(1), ZoneID=bytearray(1), Delay=bytearray(2)):
        '''
        @param SequenceNumber: SequenceNumber
        @param EndPoint: EndPoint
        @param ClusteID: ClusteID
        @param SourceAddressMode: SourceAddressMode
        @param SourceAddress: MHR, Addressing fields. 0xFFFF Broadcast PAN identifier.
        @param ZoneStatus: ZoneStatus
        @param ExtendedStatus: ExtendedStatus
        @param ZoneID: ZoneID
        @param Delay: Delay
        '''
        self.SequenceNumber = SequenceNumber
        self.EndPoint = EndPoint
        self.ClusteID = ClusteID
        self.SourceAddressMode = SourceAddressMode
        self.SourceAddress = SourceAddress
        self.ZoneStatus = ZoneStatus
        self.ExtendedStatus = ExtendedStatus
        self.ZoneID = ZoneID
        self.Delay = Delay


class HostPersistentDataManagerAvailableResponse(object):

    pass


class LoadRecordResponse(object):

    def __init__(self, Status=LoadRecordResponseStatus.NoRecordFound, Record=bytearray(2), TotalSize=bytearray(4), TotalNumberOfBlocks=bytearray(4), CurrentBlock=bytearray(4), BlockSize=bytearray(4), Parameters=[]):
        '''
        @param Status: Status
        @param Record: Record
        @param TotalSize: TotalSize
        @param TotalNumberOfBlocks: TotalNumberOfBlocks
        @param CurrentBlock: CurrentBlock
        @param BlockSize: BlockSize
        @param Parameters: Parameters
        '''
        self.Status = Status
        self.Record = Record
        self.TotalSize = TotalSize
        self.TotalNumberOfBlocks = TotalNumberOfBlocks
        self.CurrentBlock = CurrentBlock
        self.BlockSize = BlockSize
        self.Parameters = Parameters


class SaveRecordResponse(object):

    def __init__(self, Record=bytearray(2), TotalSize=bytearray(4), TotalNumberOfBlocks=bytearray(4), CurrentBlock=bytearray(4), BlockSize=bytearray(4)):
        '''
        @param Record: Record
        @param TotalSize: TotalSize
        @param TotalNumberOfBlocks: TotalNumberOfBlocks
        @param CurrentBlock: CurrentBlock
        @param BlockSize: BlockSize
        '''
        self.Record = Record
        self.TotalSize = TotalSize
        self.TotalNumberOfBlocks = TotalNumberOfBlocks
        self.CurrentBlock = CurrentBlock
        self.BlockSize = BlockSize


class RouterDiscoveryConfirm(object):

    def __init__(self, Status=bytearray(1), NwkStatus=bytearray(1)):
        '''
        @param Status: Status
        @param NwkStatus: NwkStatus
        '''
        self.Status = Status
        self.NwkStatus = NwkStatus


class APSDataConfirmFail(object):

    def __init__(self, Status=bytearray(1), SourceEndPoint=bytearray(1), DestinationEndPoint=bytearray(1), DestAddressingMode=APSDataConfirmFailDestAddressingMode.NO_PAN, DestinationAddress=[], SequenceNumber=bytearray(1)):
        '''
        @param Status: Status
        @param SourceEndPoint: SourceEndPoint
        @param DestinationEndPoint: DestinationEndPoint
        @param DestAddressingMode: DestAddressingMode
        @param DestinationAddress: MHR, Addressing fields. 0xFFFF Broadcast PAN identifier.
        @param SequenceNumber: SequenceNumber
        '''
        self.Status = Status
        self.SourceEndPoint = SourceEndPoint
        self.DestinationEndPoint = DestinationEndPoint
        self.DestAddressingMode = DestAddressingMode
        self.DestinationAddress = DestinationAddress
        self.SequenceNumber = SequenceNumber
