'''
* Copyright 2017-2018 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
'''

from com.nxp.wireless_connectivity.commands.fsci_frame_description import FsciAckPolicy, Protocol
from com.nxp.wireless_connectivity.commands.zigbee.enums import *  # @UnusedWildImport
import com.nxp.wireless_connectivity.commands.zigbee.frames as Frames
from com.nxp.wireless_connectivity.commands.zigbee.operations import *  # @UnusedWildImport


def NetworkAddress(
    device,
    TargetShortAddress=bytearray(2),
    ExtendedAddress=bytearray(8),
    RequestType=NetworkAddressRequestRequestType.SingleRequest,
    StartIndex=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.ZigBee,
    timeout=3
):
    request = Frames.NetworkAddressRequest(TargetShortAddress, ExtendedAddress, RequestType, StartIndex)
    return NetworkAddressOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def IEEEAddress(
    device,
    TargetShortAddress=bytearray(2),
    ShortAddress=bytearray(2),
    RequestType=IEEEAddressRequestRequestType.SingleRequest,
    StartIndex=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.ZigBee,
    timeout=3
):
    request = Frames.IEEEAddressRequest(TargetShortAddress, ShortAddress, RequestType, StartIndex)
    return IEEEAddressOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def NodeDescriptor(
    device,
    SequenceNumber=bytearray(1),
    Status=bytearray(1),
    NetworkAddress=bytearray(2),
    ManufacturerCode=bytearray(2),
    MaxRxSize=bytearray(2),
    MaxTxSize=bytearray(2),
    # Array length depends on MaxRxSize. Mask: 0xFF. Shift: 0x00.
    ServerMask=bytearray(1),
    DescriptorCapability=bytearray(1),
    MacCapability=bytearray(1),
    MaxBufferSize=bytearray(1),
    BitFields=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.ZigBee,
    timeout=3
):
    request = Frames.NodeDescriptorRequest(SequenceNumber, Status, NetworkAddress, ManufacturerCode,
                                           MaxRxSize, MaxTxSize, ServerMask, DescriptorCapability, MacCapability, MaxBufferSize, BitFields)
    return NodeDescriptorOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def SimpleDescriptor(
    device,
    TargetShortAddress=bytearray(2),
    EndPoint=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.ZigBee,
    timeout=3
):
    request = Frames.SimpleDescriptorRequest(TargetShortAddress, EndPoint)
    return SimpleDescriptorOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def PowerDescriptor(
    device,
    TargetShortAddress=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.ZigBee,
    timeout=3
):
    request = Frames.PowerDescriptorRequest(TargetShortAddress)
    return PowerDescriptorOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def ActiveEndpoint(
    device,
    TargetShortAddress=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.ZigBee,
    timeout=3
):
    request = Frames.ActiveEndpointRequest(TargetShortAddress)
    return ActiveEndpointOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def MatchDescriptor(
    device,
    TargetShortAddress=bytearray(2),
    ProfileID=bytearray(2),
    NumberOfInputClusters=bytearray(1),
    # Array length depends on NumberOfInputClusters. Mask: 0xFF. Shift: 0x00.
    InputClusterList=[],
    NumberOfOutputClusters=bytearray(1),
    # Array length depends on NumberOfOutputClusters. Mask: 0xFF. Shift: 0x00.
    OutputClusterList=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.ZigBee,
    timeout=3
):
    request = Frames.MatchDescriptorRequest(
        TargetShortAddress, ProfileID, NumberOfInputClusters, InputClusterList, NumberOfOutputClusters, OutputClusterList)
    return MatchDescriptorOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def UserDescriptor(
    device,
    TargetShortAddress=bytearray(2),
    AddressOfInterest=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.ZigBee,
    timeout=3
):
    request = Frames.UserDescriptorRequest(TargetShortAddress, AddressOfInterest)
    return UserDescriptorOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def SetPollInterval(
    device,
    Interval=bytearray(4),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.ZigBee,
    timeout=3
):
    request = Frames.SetPollIntervalRequest(Interval)
    return SetPollIntervalOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def AddInOutClusters(
    device,
    EndPoint=bytearray(1),
    InClusterCount=bytearray(1),
    OutClusterCount=bytearray(1),
    # Array length depends on InClusterCount. Mask: 0xFF. Shift: 0x00.
    InputClustersList=[],
    # Array length depends on OutClusterCount. Mask: 0xFF. Shift: 0x00.
    OutputClustersList=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.ZigBee,
    timeout=3
):
    request = Frames.AddInOutClustersRequest(
        EndPoint, InClusterCount, OutClusterCount, InputClustersList, OutputClustersList)
    return AddInOutClustersOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def FindAndBind(
    device,
    Initiator=False,
    EndPoint=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.ZigBee,
    timeout=3
):
    request = Frames.FindAndBindRequest(Initiator, EndPoint)
    return FindAndBindOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)

def ComplexDescriptor(
    device,
    TargetShortAddress=bytearray(2),
    AddressOfInterest=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.ZigBee,
    timeout=3
):
    request = Frames.ComplexDescriptorRequest(TargetShortAddress, AddressOfInterest)
    return ComplexDescriptorOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def ManagementLeave(
    device,
    TargetShortAddress=bytearray(2),
    ExtendedAddress=bytearray(8),
    Rejoin=ManagementLeaveRequestRejoin.DoNotRejoin,
    RemoveChildren=ManagementLeaveRequestRemoveChildren.LeaveRemovingChildren,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.ZigBee,
    timeout=3
):
    request = Frames.ManagementLeaveRequest(TargetShortAddress, ExtendedAddress, Rejoin, RemoveChildren)
    return ManagementLeaveOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def PermitJoining(
    device,
    TargetShortAddress=bytearray(2),
    Interval=bytearray(1),
    TCSignificance=PermitJoiningRequestTCSignificance.NoChangeInAuthentication,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.ZigBee,
    timeout=3
):
    request = Frames.PermitJoiningRequest(TargetShortAddress, Interval, TCSignificance)
    return PermitJoiningOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def ManagementNetworkUpdate(
    device,
    TargetShortAddress=bytearray(2),
    ChannelMask=bytearray(4),
    ScanDuration=bytearray(1),
    ScanCount=bytearray(1),
    NetworkUpdateID=bytearray(1),
    NetworkManagerShortAddress=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.ZigBee,
    timeout=3
):
    request = Frames.ManagementNetworkUpdateRequest(
        TargetShortAddress, ChannelMask, ScanDuration, ScanCount, NetworkUpdateID, NetworkManagerShortAddress)
    return ManagementNetworkUpdateOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def SystemServerDiscovery(
    device,
    TargetShortAddress=bytearray(2),
    ServerMask=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.ZigBee,
    timeout=3
):
    request = Frames.SystemServerDiscoveryRequest(TargetShortAddress, ServerMask)
    return SystemServerDiscoveryOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def ManagementLQI(
    device,
    TargetAddress=bytearray(2),
    StartIndex=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.ZigBee,
    timeout=3
):
    request = Frames.ManagementLQIRequest(TargetAddress, StartIndex)
    return ManagementLQIOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def ReadAttribute(
    device,
    AddressMode=bytearray(1),
    TargetShortAddress=bytearray(2),
    SourceEndPoint=bytearray(1),
    DestinationEndPoint=bytearray(1),
    ClusterID=bytearray(2),
    Direction=ReadAttributeRequestDirection.FromServerToClient,
    ManufacturerSpecific=ReadAttributeRequestManufacturerSpecific.No,
    ManufacturerID=bytearray(2),
    NumberOfAttributes=bytearray(1),
    # Array length depends on NumberOfAttributes. Mask: 0xFF. Shift: 0x00.
    AttributesList=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.ZigBee,
    timeout=3
):
    request = Frames.ReadAttributeRequest(AddressMode, TargetShortAddress, SourceEndPoint, DestinationEndPoint,
                                          ClusterID, Direction, ManufacturerSpecific, ManufacturerID, NumberOfAttributes, AttributesList)
    return ReadAttributeOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def WriteAttribute(
    device,
    AddressMode=bytearray(1),
    TargetShortAddress=bytearray(2),
    SourceEndPoint=bytearray(1),
    DestinationEndPoint=bytearray(1),
    ClusterID=bytearray(2),
    Direction=WriteAttributeRequestDirection.FromServerToClient,
    ManufacturerSpecific=WriteAttributeRequestManufacturerSpecific.No,
    ManufacturerID=bytearray(2),
    NumberOfAttributes=bytearray(1),
    # Array length depends on NumberOfAttributes. Mask: 0xFF. Shift: 0x00.
    AttributesList=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.ZigBee,
    timeout=3
):
    request = Frames.WriteAttributeRequest(AddressMode, TargetShortAddress, SourceEndPoint, DestinationEndPoint,
                                           ClusterID, Direction, ManufacturerSpecific, ManufacturerID, NumberOfAttributes, AttributesList)
    return WriteAttributeOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def AttributeDiscovery(
    device,
    AddressMode=bytearray(1),
    TargetShortAddress=bytearray(2),
    SourceEndPoint=bytearray(1),
    DestinationEndPoint=bytearray(1),
    ClusterID=bytearray(2),
    AttributeID=bytearray(2),
    Direction=AttributeDiscoveryRequestDirection.FromServerToClient,
    ManufacturerSpecific=AttributeDiscoveryRequestManufacturerSpecific.No,
    ManufacturerID=bytearray(2),
    NumberOfAttributes=bytearray(1),
    # Array length depends on NumberOfAttributes. Mask: 0xFF. Shift: 0x00.
    MaxNumberOfIdentifiers=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.ZigBee,
    timeout=3
):
    request = Frames.AttributeDiscoveryRequest(AddressMode, TargetShortAddress, SourceEndPoint, DestinationEndPoint,
                                               ClusterID, AttributeID, Direction, ManufacturerSpecific, ManufacturerID, NumberOfAttributes, MaxNumberOfIdentifiers)
    return AttributeDiscoveryOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def ConfigureReporting(
    device,
    AddressMode=bytearray(1),
    TargetShortAddress=bytearray(2),
    SourceEndPoint=bytearray(1),
    DestinationEndpoint=bytearray(1),
    ClusterID=bytearray(2),
    Direction=bytearray(1),
    ManufacturerSpecific=bytearray(1),
    ManufacturerID=bytearray(2),
    NumberOfAttributes=bytearray(1),
    # Array length depends on NumberOfAttributes. Mask: 0xFF. Shift: 0x00.
    AttributesList=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.ZigBee,
    timeout=3
):
    request = Frames.ConfigureReportingRequest(AddressMode, TargetShortAddress, SourceEndPoint, DestinationEndpoint,
                                               ClusterID, Direction, ManufacturerSpecific, ManufacturerID, NumberOfAttributes, AttributesList)
    return ConfigureReportingOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def Diagnostic(
    device,
    AddressMode=DiagnosticRequestAddressMode.TargetShort,
    # Array length depends on AddressMode. Mask: 0xFF. Shift: 0x00.
    TargetAddressValue=[],
    SourceEndPoint=bytearray(1),
    DestinationEndPoint=bytearray(1),
    CommandID=DiagnosticRequestCommandID.DelayRequest,
    # Array length depends on CommandID. Mask: 0xFF. Shift: 0x00.
    AttributeValue=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.DiagnosticRequest(AddressMode, TargetAddressValue, SourceEndPoint,
                                       DestinationEndPoint, CommandID, AttributeValue)
    return DiagnosticOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def SceneMembership(
    device,
    AddressMode=bytearray(1),
    TargetShortAddress=bytearray(2),
    SourceEndPoint=bytearray(1),
    DestinationEndPoint=bytearray(1),
    GroupID=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.ZigBee,
    timeout=3
):
    request = Frames.SceneMembershipRequest(AddressMode, TargetShortAddress,
                                            SourceEndPoint, DestinationEndPoint, GroupID)
    return SceneMembershipOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def RawAPSData(
    device,
    AddressMode=bytearray(1),
    TargetShortAddress=bytearray(2),
    SourceEndPoint=bytearray(1),
    DestinationEndPoint=bytearray(1),
    ClusterID=bytearray(2),
    ProfileID=bytearray(2),
    SecurityMode=bytearray(1),
    Radius=bytearray(1),
    DataLength=bytearray(1),
    # Array length depends on DataLength. Mask: 0xFF. Shift: 0x00.
    Data=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.ZigBee,
    timeout=3
):
    request = Frames.RawAPSDataRequest(AddressMode, TargetShortAddress, SourceEndPoint,
                                       DestinationEndPoint, ClusterID, ProfileID, SecurityMode, Radius, DataLength, Data)
    return RawAPSDataOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)
