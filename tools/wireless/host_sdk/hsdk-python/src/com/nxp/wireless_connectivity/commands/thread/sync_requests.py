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

from com.nxp.wireless_connectivity.commands.fsci_frame_description import Protocol
from com.nxp.wireless_connectivity.commands.thread.enums import *  # @UnusedWildImport
from com.nxp.wireless_connectivity.commands.thread.operations import *  # @UnusedWildImport
from com.nxp.wireless_connectivity.hsdk.config import THREAD_HARNESS
from time import sleep

import com.nxp.wireless_connectivity.commands.thread.frames as Frames


def MAC_MacFilteringAddEntry(
    device,
    InstanceId=bytearray(1),
    ExtendedAddress=bytearray(8),
    ShortAddress=bytearray(2),
    LinkIndicator=bytearray(1),
    BlockNeighbor=False,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.MAC_MacFilteringAddEntryRequest(
        InstanceId, ExtendedAddress, ShortAddress, LinkIndicator, BlockNeighbor)
    return MAC_MacFilteringAddEntryOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def MAC_MacFilteringRemoveEntry(
    device,
    InstanceId=bytearray(1),
    ExtendedAddress=bytearray(8),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.MAC_MacFilteringRemoveEntryRequest(InstanceId, ExtendedAddress)
    return MAC_MacFilteringRemoveEntryOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def MAC_MacFilteringEnable(
    device,
    InstanceId=bytearray(1),
    MacFiltering=MAC_MacFilteringEnableRequestMacFiltering.Disable,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.MAC_MacFilteringEnableRequest(InstanceId, MacFiltering)
    return MAC_MacFilteringEnableOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def MAC_MacFilteringGetTable(
    device,
    InstanceId=bytearray(1),
    StartIndex=bytearray(1),
    NoOfElements=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.MAC_MacFilteringGetTableRequest(InstanceId, StartIndex, NoOfElements)
    return MAC_MacFilteringGetTableOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_SetDeviceConfig(
    device,
    ThrInstanceID=bytearray(1),
    OutOfBandPreconfigured=False,
    outOfBandChannel=bytearray(1),
    PanId=bytearray(2),
    ScanChannels=bytearray(4),
    ExtendedPanId=bytearray(8),
    NwkNameSize=bytearray(1),
    # Array length depends on NwkNameSize. Mask: 0xFF. Shift: 0x00.
    NwkName=[],
    # Unit length: 16 bytes
    MLPrefix=bytearray(16),
    MLprefixSizeInBits=bytearray(1),
    # Unit length: 16 bytes
    MasterKey=bytearray(16),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.THR_SetDeviceConfigRequest(ThrInstanceID, OutOfBandPreconfigured, outOfBandChannel,
                                                PanId, ScanChannels, ExtendedPanId, NwkNameSize, NwkName, MLPrefix, MLprefixSizeInBits, MasterKey)
    return THR_SetDeviceConfigOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_NwkScan(
    device,
    InstanceID=bytearray(1),
    ScanChannelMask=bytearray(4),
    ScanType=THR_NwkScanRequestScanType.EnergyDetect,
    ScanDuration=bytearray(1),
    maxThrNwkToDiscover=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.THR_NwkScanRequest(InstanceID, ScanChannelMask, ScanType, ScanDuration, maxThrNwkToDiscover)
    return THR_NwkScanOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_CreateNwk(
    device,
    InstanceID=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=6
):
    request = Frames.THR_CreateNwkRequest(InstanceID)
    return THR_CreateNwkOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_Join(
    device,
    InstanceID=bytearray(1),
    discoveryMethod=THR_JoinRequestdiscoveryMethod.gUseThreadDiscovery_c,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.THR_JoinRequest(InstanceID, discoveryMethod)
    return THR_JoinOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_FactoryReset(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=0.3,
    retries=1
):
    request = Frames.THR_FactoryResetRequest()
    op = THR_FactoryResetOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True)

    # clear event queues
    with op.comm.fsciFramer.event_queue.mutex:
        op.comm.fsciFramer.event_queue.queue.clear()

    with op.comm.fsciFramer.async_queue.mutex:
        op.comm.fsciFramer.async_queue.queue.clear()

    confirm = op.begin(timeout)

    if THREAD_HARNESS:
        sleep(1)
    else:
        close_open_port(op, device, ack_policy)

    if confirm is None and retries != 0:
        print 'Retrying factory reset...'
        return THR_FactoryReset(device, retries=retries - 1)

    return confirm


def THR_CpuReset(
    device,
    TimeoutMs=5000,  # will reset after 5000 ms by default
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=0.2,
    retries=1
):
    request = Frames.THR_CpuResetRequest(TimeoutMs)
    op = THR_CpuResetOperation(
        device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True)

    confirm = op.begin(timeout)

    if THREAD_HARNESS:
        sleep(float(TimeoutMs) / 1000 + 1)
    else:
        if TimeoutMs > 1000:
            sleep(float(TimeoutMs) / 1000 - 1)
        close_open_port(op, device, ack_policy)

    if confirm is None and retries != 0:
        print 'Retrying CPU reset...'
        return THR_CpuReset(device, TimeoutMs, retries=retries - 1)

    return confirm


def THR_Disconnect(
    device,
    InstanceID=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.THR_DisconnectRequest(InstanceID)
    return THR_DisconnectOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_Attach(
    device,
    InstanceID=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.THR_AttachRequest(InstanceID)
    return THR_AttachOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_PromoteAsRouter(
    device,
    InstanceID=bytearray(1),
    Reason=THR_PromoteAsRouterRequestReason.TooFewRouters,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.THR_PromoteAsRouterRequest(InstanceID, Reason)
    return THR_PromoteAsRouterOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_MgmtDiagnosticGet(
    device,
    InstanceId=bytearray(1),
    # Unit length: 16 bytes
    DestIpAddr=bytearray(16),
    NumberOfTlvIds=bytearray(1),
    # Array length depends on NumberOfTlvIds. Mask: 0xFF. Shift: 0x00.
    TlvIds=[THR_MgmtDiagnosticGetRequestTlvId.SourceAddr],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.THR_MgmtDiagnosticGetRequest(InstanceId, DestIpAddr, NumberOfTlvIds, TlvIds)
    return THR_MgmtDiagnosticGetOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_DiagTestGet(
    device,
    InstanceId=bytearray(1),
    # Unit length: 16 bytes
    DestIpAddr=bytearray(16),
    PayloadSize=bytearray(1),
    TlvId=THR_DiagTestGetRequestTlvId.ColdFactoryReset,
    # Array length depends on TlvId. Mask: 0xFF. Shift: 0x00.
    AttributeValue=bytearray(),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.THR_DiagTestGetRequest(InstanceId, DestIpAddr, PayloadSize, TlvId, AttributeValue)
    return THR_DiagTestGetOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_MgmtDiagnosticReset(
    device,
    InstanceId=bytearray(1),
    # Unit length: 16 bytes
    DestIpAddr=bytearray(16),
    NumberOfTlvIds=bytearray(1),
    # Array length depends on NumberOfTlvIds. Mask: 0xFF. Shift: 0x00.
    TlvIds=[THR_MgmtDiagnosticResetRequestTlvId.SourceAddr],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.THR_MgmtDiagnosticResetRequest(InstanceId, DestIpAddr, NumberOfTlvIds, TlvIds)
    return THR_MgmtDiagnosticResetOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_MgmtReadMemory(
    device,
    Address=bytearray(4),
    Length=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.THR_MgmtReadMemoryRequest(Address, Length)
    return THR_MgmtReadMemoryOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_MgmtWriteMemory(
    device,
    Address=bytearray(4),
    Length=bytearray(1),
    # Array length depends on Length. Mask: 0xFF. Shift: 0x00.
    Value=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.THR_MgmtWriteMemoryRequest(Address, Length, Value)
    return THR_MgmtWriteMemoryOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_NwkDiscovery(
    device,
    InstanceID=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.THR_NwkDiscoveryRequest(InstanceID)
    return THR_NwkDiscoveryOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_NwkDiscoveryStop(
    device,
    InstanceID=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.THR_NwkDiscoveryStopRequest(InstanceID)
    return THR_NwkDiscoveryStopOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_SearchNwkWithAnounce(
    device,
    InstanceID=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.THR_SearchNwkWithAnounceRequest(InstanceID)
    return THR_SearchNwkWithAnounceOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_ChildUpdateToParent(
    device,
    InstanceID=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.THR_ChildUpdateToParentRequest(InstanceID)
    return THR_ChildUpdateToParentOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_SetManualSlaacIID(
    device,
    IID=bytearray(8),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.THR_SetManualSlaacIIDRequest(IID)
    return THR_SetManualSlaacIIDOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_SendProactiveAddrNotif(
    device,
    InstanceId=bytearray(1),
    # Unit length: 16 bytes
    DestinationIPAddress=bytearray(16),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.THR_SendProactiveAddrNotifRequest(InstanceId, DestinationIPAddress)
    return THR_SendProactiveAddrNotifOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_SetThreshold(
    device,
    InstanceId=bytearray(1),
    ThresholdType=THR_SetThresholdRequestThresholdType.RouterUpgradeThreshold,
    Value=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.THR_SetThresholdRequest(InstanceId, ThresholdType, Value)
    return THR_SetThresholdOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_SetNwkIdTimeout(
    device,
    TimeoutInSeconds=bytearray(4),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.THR_SetNwkIdTimeoutRequest(TimeoutInSeconds)
    return THR_SetNwkIdTimeoutOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_GetRoutingTable(
    device,
    InstanceId=bytearray(1),
    StartIndex=bytearray(1),
    NoOfElements=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.THR_GetRoutingTableRequest(InstanceId, StartIndex, NoOfElements)
    return THR_GetRoutingTableOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_GetNeighborTable(
    device,
    InstanceId=bytearray(1),
    StartIndex=bytearray(1),
    NoOfElements=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.THR_GetNeighborTableRequest(InstanceId, StartIndex, NoOfElements)
    return THR_GetNeighborTableOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_GetNeighborInfo(
    device,
    InstanceId=bytearray(1),
    ShortAddress=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.THR_GetNeighborInfoRequest(InstanceId, ShortAddress)
    return THR_GetNeighborInfoOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_GetChildrenTable(
    device,
    InstanceId=bytearray(1),
    StartIndex=bytearray(1),
    NoOfElements=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.THR_GetChildrenTableRequest(InstanceId, StartIndex, NoOfElements)
    return THR_GetChildrenTableOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_GetAttr(
    device,
    InstanceId=bytearray(1),
    AttributeId=THR_GetAttrRequestAttributeId.RandomExtendedAddr,
    Index=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3,
    retries=1
):
    request = Frames.THR_GetAttrRequest(InstanceId, AttributeId, Index)
    op = THR_GetAttrOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True)
    confirm = op.begin(timeout)

    if confirm is None and retries != 0:
        print 'Retrying THR_GetAttr...'
        return THR_GetAttr(device, InstanceId, AttributeId, Index, retries=retries - 1)

    if THREAD_HARNESS:
        if confirm is None and AttributeId == THR_GetAttrRequestAttributeId.SwVersion:
            op.comm.fsciFramer.device.close()

    return confirm


def THR_SetAttr(
    device,
    InstanceId=bytearray(1),
    AttributeId=THR_SetAttrRequestAttributeId.RandomExtendedAddr,
    Index=bytearray(1),
    AttrSize=bytearray(1),
    AttributeValue=bytearray(),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=5,
    retries=1
):
    request = Frames.THR_SetAttrRequest(InstanceId, AttributeId, Index, AttrSize, AttributeValue)
    confirm = THR_SetAttrOperation(
        device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)
    if confirm is None and retries != 0:
        print 'Retrying THR_SetAttr...'
        return THR_SetAttr(device, InstanceId, AttributeId, Index, AttrSize, AttributeValue, retries=retries - 1)
    return confirm


def THR_LeaderRemoveRouterId(
    device,
    InstanceId=bytearray(1),
    RouterShortAddr=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.THR_LeaderRemoveRouterIdRequest(InstanceId, RouterShortAddr)
    return THR_LeaderRemoveRouterIdOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_GenerateAllKeys(
    device,
    InstanceId=bytearray(1),
    KeySequenceCounter=bytearray(4),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.THR_GenerateAllKeysRequest(InstanceId, KeySequenceCounter)
    return THR_GenerateAllKeysOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_SwitchKey(
    device,
    InstanceId=bytearray(1),
    KeySequenceCounter=bytearray(4),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.THR_SwitchKeyRequest(InstanceId, KeySequenceCounter)
    return THR_SwitchKeyOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_BrPrefixAddEntry(
    device,
    InstanceId=bytearray(1),
    prefixLength=bytearray(1),
    # Unit length: 16 bytes
    PrefixValue=bytearray(16),
    PrefixFlagsReserved=bytearray(1),
    PrefixFlags=bytearray(1),
    prefixLifetime=bytearray(4),
    prefixAdvertised=False,
    ExternalRouteFlags=bytearray(1),
    ExternalRouteLifetime=bytearray(4),
    ExternalRouteAdvertised=False,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.THR_BrPrefixAddEntryRequest(InstanceId, prefixLength, PrefixValue, PrefixFlagsReserved, PrefixFlags,
                                                 prefixLifetime, prefixAdvertised, ExternalRouteFlags, ExternalRouteLifetime, ExternalRouteAdvertised)
    return THR_BrPrefixAddEntryOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_BrPrefixGetTable(
    device,
    InstanceId=bytearray(1),
    StartIndex=bytearray(1),
    NoOfElements=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.THR_BrPrefixGetTableRequest(InstanceId, StartIndex, NoOfElements)
    return THR_BrPrefixGetTableOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_BrPrefixRemoveEntry(
    device,
    InstanceId=bytearray(1),
    prefixLength=bytearray(1),
    PrefixValue=bytearray(16),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.THR_BrPrefixRemoveEntryRequest(InstanceId, prefixLength, PrefixValue)
    return THR_BrPrefixRemoveEntryOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_BrServiceRemoveEntry(
    device,
    InstanceId=bytearray(1),
    # Unit length: 9 bytes
    ServiceName='dnsserver',
    # Unit length: 16 bytes
    ServerAddress=bytearray(16),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.THR_BrServiceRemoveEntryRequest(InstanceId, ServiceName, ServerAddress)
    return THR_BrServiceRemoveEntryOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_BrServiceAddEntry(
    device,
    InstanceId=bytearray(1),
    # Unit length: 9 bytes
    ServiceName='dnsserver',
    # Unit length: 16 bytes
    ServerAddress=bytearray(16),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.THR_BrServiceAddEntryRequest(InstanceId, ServiceName, ServerAddress)
    return THR_BrServiceAddEntryOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_BrPrefixSync(
    device,
    InstanceId=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.THR_BrPrefixSyncRequest(InstanceId)
    return THR_BrPrefixSyncOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_BrPrefixRemoveAll(
    device,
    InstanceId=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.THR_BrPrefixRemoveAllRequest(InstanceId)
    return THR_BrPrefixRemoveAllOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_GetThreadIpAddr(
    device,
    InstanceId=bytearray(1),
    AddressType=THR_GetThreadIpAddrRequestAddressType.Link_Local_64,
    StartIndex=None,  # Leave None except for Global.
    NoOfElements=None,  # Leave None except for Global.
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    if AddressType in [THR_GetThreadIpAddrRequestAddressType.Global, THR_GetThreadIpAddrRequestAddressType.Anycast]:
        try:
            Data = bytearray([StartIndex, NoOfElements])
        except TypeError:
            Data = bytearray([0, 1])
    else:
        Data = None
    request = Frames.THR_GetThreadIpAddrRequest(InstanceId, AddressType, Data)
    return THR_GetThreadIpAddrOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_GetParent(
    device,
    InstanceId=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.THR_GetParentRequest(InstanceId)
    return THR_GetParentOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def NWKU_EidToRlocMap(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.NWKU_EidToRlocMapRequest()
    return NWKU_EidToRlocMapOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def THR_Identify(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.THR_IdentifyRequest()
    return THR_IdentifyOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def SocketCreate(
    device,
    SocketDomain=SocketCreateRequestSocketDomain(),
    SocketType=SocketCreateRequestSocketType.Datagram,
    SocketProtocol=SocketCreateRequestSocketProtocol(),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.SocketCreateRequest(SocketDomain, SocketType, SocketProtocol)
    return SocketCreateOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def SocketShutdown(
    device,
    SocketIndex=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.SocketShutdownRequest(SocketIndex)
    return SocketShutdownOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def SocketBind(
    device,
    SocketIndex=bytearray(1),
    # Unit length: 16 bytes
    LocalIpAddress=bytearray(16),
    LocalPort=bytearray(2),
    SocketDomain=SocketBindRequestSocketDomain(),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.SocketBindRequest(SocketIndex, LocalIpAddress, LocalPort, SocketDomain)
    return SocketBindOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def SocketSend(
    device,
    SocketIndex=bytearray(1),
    Flags=SocketSendRequestFlags(),
    Size=bytearray(2),
    # Array length depends on Size. Mask: 0xFFFF. Shift: 0x0000.
    Data=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.SocketSendRequest(SocketIndex, Flags, Size, Data)
    return SocketSendOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def SocketSendTo(
    device,
    SocketIndex=bytearray(1),
    Flags=SocketSendToRequestFlags(),
    SecuredMACData=False,
    Size=bytearray(2),
    RemotePort=bytearray(2),
    # Unit length: 16 bytes
    RemoteIpAddress=bytearray(16),
    # Array length depends on Size. Mask: 0xFFFF. Shift: 0x0000.
    Data=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.SocketSendToRequest(SocketIndex, Flags, SecuredMACData, Size, RemotePort, RemoteIpAddress, Data)
    return SocketSendToOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def SocketReceive(
    device,
    SocketIndex=bytearray(1),
    DataSize=bytearray(2),
    Flags=SocketReceiveRequestFlags(),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.SocketReceiveRequest(SocketIndex, DataSize, Flags)
    return SocketReceiveOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def SocketReceiveFrom(
    device,
    SocketIndex=bytearray(1),
    DataSize=bytearray(2),
    Flags=SocketReceiveFromRequestFlags(),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.SocketReceiveFromRequest(SocketIndex, DataSize, Flags)
    return SocketReceiveFromOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def SocketConnect(
    device,
    SocketIndex=bytearray(1),
    # Unit length: 16 bytes
    RemoteIpAddress=bytearray(16),
    RemotePort=bytearray(2),
    SocketDomain=SocketConnectRequestSocketDomain(),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.SocketConnectRequest(SocketIndex, RemoteIpAddress, RemotePort, SocketDomain)
    return SocketConnectOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def SocketListen(
    device,
    SocketIndex=bytearray(1),
    Backlog=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.SocketListenRequest(SocketIndex, Backlog)
    return SocketListenOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def SocketAccept(
    device,
    SocketIndex=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.SocketAcceptRequest(SocketIndex)
    return SocketAcceptOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def SocketSetOption(
    device,
    SocketIndex=bytearray(1),
    SocketLevel=SocketSetOptionRequestSocketLevel.SOL_SOCKET,
    SocketOption=SocketSetOptionRequestSocketOption.SO_TYPE,
    SocketOptionValue=bytearray(4),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.SocketSetOptionRequest(SocketIndex, SocketLevel, SocketOption, SocketOptionValue)
    return SocketSetOptionOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def SocketGetOption(
    device,
    SocketIndex=bytearray(1),
    SocketLevel=SocketGetOptionRequestSocketLevel.SOL_SOCKET,
    SocketOption=SocketGetOptionRequestSocketOption.SO_TYPE,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.SocketGetOptionRequest(SocketIndex, SocketLevel, SocketOption)
    return SocketGetOptionOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def MESHCOP_StartCommissioner(
    device,
    InstanceId=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.MESHCOP_StartCommissionerRequest(InstanceId)
    return MESHCOP_StartCommissionerOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def MESHCOP_StartNativeCommissioner(
    device,
    InstanceId=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.MESHCOP_StartNativeCommissionerRequest(InstanceId)
    return MESHCOP_StartNativeCommissionerOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def MESHCOP_StopCommissioner(
    device,
    InstanceId=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.MESHCOP_StopCommissionerRequest(InstanceId)
    return MESHCOP_StopCommissionerOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def MESHCOP_AddExpectedJoiner(
    device,
    InstanceId=bytearray(1),
    Selected=False,
    EuiType=MESHCOP_AddExpectedJoinerRequestEuiType(),
    EUI=[],
    PSKdSize=bytearray(1),
    # Array length depends on PSKdSize. Mask: 0xFF. Shift: 0x00.
    PSKd=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.MESHCOP_AddExpectedJoinerRequest(InstanceId, Selected, EuiType, EUI, PSKdSize, PSKd)
    return MESHCOP_AddExpectedJoinerOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def MESHCOP_GetExpectedJoiner(
    device,
    InstanceId=bytearray(1),
    EuiType=MESHCOP_GetExpectedJoinerRequestEuiType(),
    EUI=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.MESHCOP_GetExpectedJoinerRequest(InstanceId, EuiType, EUI)
    return MESHCOP_GetExpectedJoinerOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def MESHCOP_RemoveExpectedJoiner(
    device,
    InstanceId=bytearray(1),
    EuiType=MESHCOP_RemoveExpectedJoinerRequestEuiType(),
    EUI=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.MESHCOP_RemoveExpectedJoinerRequest(InstanceId, EuiType, EUI)
    return MESHCOP_RemoveExpectedJoinerOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def MESHCOP_RemoveAllExpectedJoiners(
    device,
    InstanceId=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.MESHCOP_RemoveAllExpectedJoinersRequest(InstanceId)
    return MESHCOP_RemoveAllExpectedJoinersOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def MESHCOP_SyncSteeringData(
    device,
    InstanceId=bytearray(1),
    EuiMask=MESHCOP_SyncSteeringDataRequestEuiMask.AllZeroes,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.MESHCOP_SyncSteeringDataRequest(InstanceId, EuiMask)
    return MESHCOP_SyncSteeringDataOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def MESHCOP_MgmtSet(
    device,
    InstanceId=bytearray(1),
    PSKcEnable=False,
    PSKcSize=bytearray(1),
    # Array length depends on PSKcSize.
    PSKc=[],
    NetworkNameEnable=False,
    NetworkNameSize=bytearray(1),
    # Array length depends on NetworkNameSize.
    NetworkName=[],
    PolicyEnable=False,
    Policy=bytearray(1),
    KeyRotationInterval=bytearray(2),
    TimestampEnable=False,
    # Unit length: 6 bytes. Included if TimestampEnable.
    Seconds=bytearray(6),
    # Unit length: 2 bytes. Included if TimestampEnable.
    Ticks=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=5  # takes more time to confirm
):
    request = Frames.MESHCOP_MgmtSetRequest(
        InstanceId,
        PSKcEnable, PSKcSize, PSKc,
        NetworkNameEnable, NetworkNameSize, NetworkName,
        PolicyEnable, Policy, KeyRotationInterval=to_bytes(KeyRotationInterval, 2, reverse=True),
        TimestampEnable=TimestampEnable, Seconds=to_bytes(Seconds, 6, reverse=True), Ticks=to_bytes(Ticks, 2, reverse=True)
    )
    return MESHCOP_MgmtSetOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def MESHCOP_MgmtGet(
    device,
    InstanceId=bytearray(1),
    NumberOfTlvIds=bytearray(1),
    # Array length depends on NumberOfTlvIds. Mask: 0xFF. Shift: 0x00.
    TlvIds=[MESHCOP_MgmtGetRequestTlvId.Channel],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.MESHCOP_MgmtGetRequest(InstanceId, NumberOfTlvIds, TlvIds)
    return MESHCOP_MgmtGetOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def MESHCOP_SetCommissionerCredential(
    device,
    InstanceId=bytearray(1),
    PSKcSize=bytearray(1),
    # Array length depends on PSKcSize. Mask: 0xFF. Shift: 0x00.
    PSKc=[],
    XpanId=bytearray(8),
    NetworkNameSize=bytearray(1),
    # Array length depends on NetworkNameSize. Mask: 0xFF. Shift: 0x00.
    NetworkName=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.MESHCOP_SetCommissionerCredentialRequest(
        InstanceId, PSKcSize, PSKc, XpanId, NetworkNameSize, NetworkName)
    return MESHCOP_SetCommissionerCredentialOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def MESHCOP_MgmNwkForm(
    device,
    # Unit length: 16 bytes
    IPAddress=bytearray(16),
    NwkNameSize=bytearray(1),
    # Array length depends on NwkNameSize. Mask: 0xFF. Shift: 0x00.
    NwkName=[],
    # Unit length: 16 bytes
    NetworkMasterKey=bytearray(16),
    PSKcSize=bytearray(1),
    # Array length depends on PSKcSize. Mask: 0xFF. Shift: 0x00.
    PSKc=[],
    Channel=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.MESHCOP_MgmNwkFormRequest(
        IPAddress, NwkNameSize, NwkName, NetworkMasterKey, PSKcSize, PSKc, Channel)
    return MESHCOP_MgmNwkFormOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def MESHCOP_MgmtCommissionerGet(
    device,
    InstanceId=bytearray(1),
    # Unit length: 16 bytes
    IPAddress=bytearray(16),
    NumberOfTlvIds=bytearray(1),
    # Array length depends on NumberOfTlvIds. Mask: 0xFF. Shift: 0x00.
    TlvIds=[MESHCOP_MgmtCommissionerGetRequestTlvId.Channel],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.MESHCOP_MgmtCommissionerGetRequest(InstanceId, IPAddress, NumberOfTlvIds, TlvIds)
    return MESHCOP_MgmtCommissionerGetOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def MESHCOP_MgmtActiveGet(
    device,
    InstanceId=bytearray(1),
    # Unit length: 16 bytes
    IPAddress=bytearray(16),
    NumberOfTlvIds=bytearray(1),
    # Array length depends on NumberOfTlvIds. Mask: 0xFF. Shift: 0x00.
    TlvIds=[MESHCOP_MgmtActiveGetRequestTlvId.Channel],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.MESHCOP_MgmtActiveGetRequest(InstanceId, IPAddress, NumberOfTlvIds, TlvIds)
    return MESHCOP_MgmtActiveGetOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def MESHCOP_MgmtPendingGet(
    device,
    InstanceId=bytearray(1),
    # Unit length: 16 bytes
    IPAddress=bytearray(16),
    NumberOfTlvIds=bytearray(1),
    # Array length depends on NumberOfTlvIds. Mask: 0xFF. Shift: 0x00.
    TlvIds=[MESHCOP_MgmtPendingGetRequestTlvId.Channel],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.MESHCOP_MgmtPendingGetRequest(InstanceId, IPAddress, NumberOfTlvIds, TlvIds)
    return MESHCOP_MgmtPendingGetOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def MESHCOP_MgmtCommissionerSet(
    device,
    InstanceId=bytearray(1),
    IPAddress=bytearray(16),
    SessionIdEnable=False,
    # Array length depends on SessionIdEnable. Mask: 0xFF. Shift: 0x00.
    SessionId=[],
    BorderRouterLocatorEnable=False,
    # Array length depends on BorderRouterLocatorEnable. Mask: 0xFF. Shift: 0x00.
    BorderRouterLocator=[],
    NewSessionIdEnable=False,
    # Array length depends on NewSessionIdEnable. Mask: 0xFF. Shift: 0x00.
    NewSessionId=[],
    SteeringDataEnable=False,
    # Array length depends on SteeringDataEnable. Mask: 0xFF. Shift: 0x00.
    SteeringDataSize=[],
    # Array length depends on SteeringDataSize. Mask: 0xFF. Shift: 0x00.
    SteeringData=[],
    ChannelEnable=False,
    # Array length depends on ChannelEnable. Mask: 0xFF. Shift: 0x00.
    Channel=[],
    ChannelMaskEnable=False,
    # Array length depends on ChannelMaskEnable. Mask: 0xFF. Shift: 0x00.
    ChannelPage=[],
    # Array length depends on ChannelMaskEnable. Mask: 0xFF. Shift: 0x00.
    ChannelMaskLength=[],
    # Array length depends on ChannelMaskLength. Mask: 0xFF. Shift: 0x00.
    ChannelMask=[],
    XpanIdEnable=False,
    # Array length depends on XpanIdEnable. Mask: 0xFF. Shift: 0x00.
    XpanId=[],
    MLPrefixEnable=False,
    # Array length depends on MLPrefixEnable. Mask: 0xFF. Shift: 0x00.
    MLPrefix=MESHCOP_MgmtCommissionerSetRequest.MLPrefix(),
    MasterKeyEnable=False,
    # Unit length: 16 bytes
        # Array length depends on MasterKeyEnable. Mask: 0xFF. Shift: 0x00.
    MasterKey=[],
    NwkNameEnable=False,
    # Array length depends on NwkNameEnable. Mask: 0xFF. Shift: 0x00.
    NwkNameSize=[],
    # Array length depends on NwkNameSize. Mask: 0xFF. Shift: 0x00.
    NwkName=[],
    PanIdEnable=False,
    # Array length depends on PanIdEnable. Mask: 0xFF. Shift: 0x00.
    PanId=[],
    PSKcEnable=False,
    # Array length depends on PSKcEnable. Mask: 0xFF. Shift: 0x00.
    PskcSize=[],
    # Array length depends on PskcSize. Mask: 0xFF. Shift: 0x00.
    PSKc=[],
    PolicyEnable=False,
    # Array length depends on PolicyEnable. Mask: 0xFF. Shift: 0x00.
    RotationInterval=[],
    # Array length depends on PolicyEnable. Mask: 0xFF. Shift: 0x00.
    Policy=[],
    ActiveTimestampEnable=False,
    # Unit length: 6 bytes
    # Array length depends on ActiveTimestampEnable. Mask: 0xFF. Shift: 0x00.
    ActiveSeconds=[],
    # Array length depends on ActiveTimestampEnable. Mask: 0xFF. Shift: 0x00.
    ActiveTicks=[],
    PendingTimestampEnable=False,
    # Unit length: 6 bytes
    # Array length depends on PendingTimestampEnable. Mask: 0xFF. Shift: 0x00.
    PendingSeconds=[],
    # Array length depends on PendingTimestampEnable. Mask: 0xFF. Shift: 0x00.
    PendingTicks=[],
    DelayTimerEnable=False,
    # Array length depends on DelayTimerEnable. Mask: 0xFF. Shift: 0x00.
    Timeout=[],
    FutureTlvEnable=False,
    FutureTlvSize=bytearray(1),
    FutureTlvValue=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=5
):
    request = Frames.MESHCOP_MgmtCommissionerSetRequest(InstanceId, IPAddress, SessionIdEnable, SessionId, BorderRouterLocatorEnable, BorderRouterLocator, NewSessionIdEnable, NewSessionId, SteeringDataEnable, SteeringDataSize, SteeringData, ChannelEnable, Channel, ChannelMaskEnable, ChannelPage, ChannelMaskLength,
                                                        ChannelMask, XpanIdEnable, XpanId, MLPrefixEnable, MLPrefix, MasterKeyEnable, MasterKey, NwkNameEnable, NwkNameSize, NwkName, PanIdEnable, PanId, PSKcEnable, PskcSize, PSKc, PolicyEnable, RotationInterval, Policy, ActiveTimestampEnable, ActiveSeconds, ActiveTicks, PendingTimestampEnable, PendingSeconds, PendingTicks, DelayTimerEnable, Timeout, FutureTlvEnable, FutureTlvSize, FutureTlvValue)
    return MESHCOP_MgmtCommissionerSetOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def MESHCOP_MgmtActiveSet(
    device,
    InstanceId=bytearray(1),
    IPAddress=bytearray(16),
    SessionIdEnable=False,
    # Array length depends on SessionIdEnable. Mask: 0xFF. Shift: 0x00.
    SessionId=[],
    BorderRouterLocatorEnable=False,
    # Array length depends on BorderRouterLocatorEnable. Mask: 0xFF. Shift: 0x00.
    BorderRouterLocator=[],
    NewSesswionIdEnable=False,
    # Array length depends on NewSesswionIdEnable. Mask: 0xFF. Shift: 0x00.
    NewSesswionId=[],
    SteeringDataEnable=False,
    # Array length depends on SteeringDataEnable. Mask: 0xFF. Shift: 0x00.
    SteeringDataSize=[],
    # Array length depends on SteeringDataSize. Mask: 0xFF. Shift: 0x00.
    SteeringData=[],
    ChannelEnable=False,
    # Array length depends on ChannelEnable. Mask: 0xFF. Shift: 0x00.
    Channel=[],
    ChannelMaskEnable=False,
    # Array length depends on ChannelMaskEnable. Mask: 0xFF. Shift: 0x00.
    ChannelPage=[],
    # Array length depends on ChannelMaskEnable. Mask: 0xFF. Shift: 0x00.
    ChannelMaskLength=[],
    # Array length depends on ChannelMaskLength. Mask: 0xFF. Shift: 0x00.
    ChannelMask=[],
    XpanIdEnable=False,
    # Array length depends on XpanIdEnable. Mask: 0xFF. Shift: 0x00.
    XpanId=[],
    MLPrefixEnable=False,
    # Array length depends on MLPrefixEnable. Mask: 0xFF. Shift: 0x00.
    MLPrefix=MESHCOP_MgmtActiveSetRequest.MLPrefix(),
    MasterKeyEnable=False,
    # Unit length: 16 bytes
        # Array length depends on MasterKeyEnable. Mask: 0xFF. Shift: 0x00.
    MasterKey=[],
    NwkNameEnable=False,
    # Array length depends on NwkNameEnable. Mask: 0xFF. Shift: 0x00.
    NwkNameSize=[],
    # Array length depends on NwkNameSize. Mask: 0xFF. Shift: 0x00.
    NwkName=[],
    PanIdEnable=False,
    # Array length depends on PanIdEnable. Mask: 0xFF. Shift: 0x00.
    PanId=[],
    PSKcEnable=False,
    # Array length depends on PSKcEnable. Mask: 0xFF. Shift: 0x00.
    PskcSize=[],
    # Array length depends on PskcSize. Mask: 0xFF. Shift: 0x00.
    PSKc=[],
    PolicyEnable=False,
    # Array length depends on PolicyEnable. Mask: 0xFF. Shift: 0x00.
    RotationInterval=[],
    # Array length depends on PolicyEnable. Mask: 0xFF. Shift: 0x00.
    Policy=[],
    ActiveTimestampEnable=False,
    # Unit length: 6 bytes
    # Array length depends on ActiveTimestampEnable. Mask: 0xFF. Shift: 0x00.
    ActiveSeconds=[],
    # Array length depends on ActiveTimestampEnable. Mask: 0xFF. Shift: 0x00.
    ActiveTicks=[],
    PendingTimestampEnable=False,
    # Unit length: 6 bytes
    # Array length depends on PendingTimestampEnable. Mask: 0xFF. Shift: 0x00.
    PendingSeconds=[],
    # Array length depends on PendingTimestampEnable. Mask: 0xFF. Shift: 0x00.
    PendingTicks=[],
    DelayTimerEnable=False,
    # Array length depends on DelayTimerEnable. Mask: 0xFF. Shift: 0x00.
    Timeout=[],
    FutureTlvEnable=False,
    FutureTlvSize=bytearray(1),
    FutureTlvValue=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=5
):
    request = Frames.MESHCOP_MgmtActiveSetRequest(InstanceId, IPAddress, SessionIdEnable, SessionId, BorderRouterLocatorEnable, BorderRouterLocator, NewSesswionIdEnable, NewSesswionId, SteeringDataEnable, SteeringDataSize, SteeringData, ChannelEnable, Channel, ChannelMaskEnable, ChannelPage, ChannelMaskLength,
                                                  ChannelMask, XpanIdEnable, XpanId, MLPrefixEnable, MLPrefix, MasterKeyEnable, MasterKey, NwkNameEnable, NwkNameSize, NwkName, PanIdEnable, PanId, PSKcEnable, PskcSize, PSKc, PolicyEnable, RotationInterval, Policy, ActiveTimestampEnable, ActiveSeconds, ActiveTicks, PendingTimestampEnable, PendingSeconds, PendingTicks, DelayTimerEnable, Timeout, FutureTlvEnable, FutureTlvSize, FutureTlvValue)
    return MESHCOP_MgmtActiveSetOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def MESHCOP_MgmtPendingSet(
    device,
    InstanceId=bytearray(1),
    IPAddress=bytearray(16),
    SessionIdEnable=False,
    # Array length depends on SessionIdEnable. Mask: 0xFF. Shift: 0x00.
    SessionId=[],
    BorderRouterLocatorEnable=False,
    # Array length depends on BorderRouterLocatorEnable. Mask: 0xFF. Shift: 0x00.
    BorderRouterLocator=[],
    NewSesswionIdEnable=False,
    # Array length depends on NewSesswionIdEnable. Mask: 0xFF. Shift: 0x00.
    NewSesswionId=[],
    SteeringDataEnable=False,
    # Array length depends on SteeringDataEnable. Mask: 0xFF. Shift: 0x00.
    SteeringDataSize=[],
    # Array length depends on SteeringDataSize. Mask: 0xFF. Shift: 0x00.
    SteeringData=[],
    ChannelEnable=False,
    # Array length depends on ChannelEnable. Mask: 0xFF. Shift: 0x00.
    Channel=[],
    ChannelMaskEnable=False,
    # Array length depends on ChannelMaskEnable. Mask: 0xFF. Shift: 0x00.
    ChannelPage=[],
    # Array length depends on ChannelMaskEnable. Mask: 0xFF. Shift: 0x00.
    ChannelMaskLength=[],
    # Array length depends on ChannelMaskLength. Mask: 0xFF. Shift: 0x00.
    ChannelMask=[],
    XpanIdEnable=False,
    # Array length depends on XpanIdEnable. Mask: 0xFF. Shift: 0x00.
    XpanId=[],
    MLPrefixEnable=False,
    # Array length depends on MLPrefixEnable. Mask: 0xFF. Shift: 0x00.
    MLPrefix=MESHCOP_MgmtPendingSetRequest.MLPrefix(),
    MasterKeyEnable=False,
    # Unit length: 16 bytes
        # Array length depends on MasterKeyEnable. Mask: 0xFF. Shift: 0x00.
    MasterKey=[],
    NwkNameEnable=False,
    # Array length depends on NwkNameEnable. Mask: 0xFF. Shift: 0x00.
    NwkNameSize=[],
    # Array length depends on NwkNameSize. Mask: 0xFF. Shift: 0x00.
    NwkName=[],
    PanIdEnable=False,
    # Array length depends on PanIdEnable. Mask: 0xFF. Shift: 0x00.
    PanId=[],
    PSKcEnable=False,
    # Array length depends on PSKcEnable. Mask: 0xFF. Shift: 0x00.
    PskcSize=[],
    # Array length depends on PskcSize. Mask: 0xFF. Shift: 0x00.
    PSKc=[],
    PolicyEnable=False,
    # Array length depends on PolicyEnable. Mask: 0xFF. Shift: 0x00.
    RotationInterval=[],
    # Array length depends on PolicyEnable. Mask: 0xFF. Shift: 0x00.
    Policy=[],
    ActiveTimestampEnable=False,
    # Unit length: 6 bytes
    # Array length depends on ActiveTimestampEnable. Mask: 0xFF. Shift: 0x00.
    ActiveSeconds=[],
    # Array length depends on ActiveTimestampEnable. Mask: 0xFF. Shift: 0x00.
    ActiveTicks=[],
    PendingTimestampEnable=False,
    # Unit length: 6 bytes
    # Array length depends on PendingTimestampEnable. Mask: 0xFF. Shift: 0x00.
    PendingSeconds=[],
    # Array length depends on PendingTimestampEnable. Mask: 0xFF. Shift: 0x00.
    PendingTicks=[],
    DelayTimerEnable=False,
    # Array length depends on DelayTimerEnable. Mask: 0xFF. Shift: 0x00.
    Timeout=[],
    FutureTlvEnable=False,
    FutureTlvSize=bytearray(1),
    FutureTlvValue=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=5
):
    request = Frames.MESHCOP_MgmtPendingSetRequest(InstanceId, IPAddress, SessionIdEnable, SessionId, BorderRouterLocatorEnable, BorderRouterLocator, NewSesswionIdEnable, NewSesswionId, SteeringDataEnable, SteeringDataSize, SteeringData, ChannelEnable, Channel, ChannelMaskEnable, ChannelPage, ChannelMaskLength,
                                                   ChannelMask, XpanIdEnable, XpanId, MLPrefixEnable, MLPrefix, MasterKeyEnable, MasterKey, NwkNameEnable, NwkNameSize, NwkName, PanIdEnable, PanId, PSKcEnable, PskcSize, PSKc, PolicyEnable, RotationInterval, Policy, ActiveTimestampEnable, ActiveSeconds, ActiveTicks, PendingTimestampEnable, PendingSeconds, PendingTicks, DelayTimerEnable, Timeout, FutureTlvEnable, FutureTlvSize, FutureTlvValue)
    return MESHCOP_MgmtPendingSetOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def MESHCOP_MgmtSendPanIdQuery(
    device,
    ThrInstanceID=bytearray(1),
    ScanChannelMask=bytearray(4),
    PanId=bytearray(2),
    # Unit length: 16 bytes
    IPAddress=bytearray(16),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.MESHCOP_MgmtSendPanIdQueryRequest(ThrInstanceID, ScanChannelMask, PanId, IPAddress)
    return MESHCOP_MgmtSendPanIdQueryOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def MESHCOP_MgmtSendEdScan(
    device,
    ThrInstanceID=bytearray(1),
    ScanChannelMask=bytearray(4),
    Count=bytearray(1),
    Period=bytearray(2),
    ScanDuration=bytearray(2),
    # Unit length: 16 bytes
    IPAddress=bytearray(16),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.MESHCOP_MgmtSendEdScanRequest(
        ThrInstanceID, ScanChannelMask, Count, Period, ScanDuration, IPAddress)
    return MESHCOP_MgmtSendEdScanOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def MESHCOP_MgmtSendAnnounceBegin(
    device,
    ThrInstanceID=bytearray(1),
    CommissionerSessionID=bytearray(2),
    ChannelMask=bytearray(4),
    Count=bytearray(1),
    Period=bytearray(2),
    # Unit length: 16 bytes
    IPAddress=bytearray(16),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.MESHCOP_MgmtSendAnnounceBeginRequest(
        ThrInstanceID, CommissionerSessionID, ChannelMask, Count, Period, IPAddress)
    return MESHCOP_MgmtSendAnnounceBeginOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def NWKU_IfconfigBind(
    device,
    # Unit length: 16 bytes
    IpAddress=bytearray(16),
    InterfaceId=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.NWKU_IfconfigBindRequest(IpAddress, InterfaceId)
    return NWKU_IfconfigBindOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def NWKU_IfconfigAll(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.NWKU_IfconfigAllRequest()
    return NWKU_IfconfigAllOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def NWKU_Ping(
    device,
    # Unit length: 16 bytes
    DestinationIpAddress=bytearray(16),
    # Unit length: 16 bytes
    SourceIpAddress=bytearray(16),
    Payloadlength=bytearray(2),
    Timeout=bytearray(2),
    Secured=True,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    if Timeout != 0 and Timeout != bytearray(2):
        timeout = Timeout / 1000.

    if type(DestinationIpAddress) is list:
        DestinationIpAddress = DestinationIpAddress[::-1]
    if type(SourceIpAddress) is list:
        SourceIpAddress = SourceIpAddress[::-1]

    request = Frames.NWKU_PingRequest(
        DestinationIpAddress, SourceIpAddress, Payloadlength, Timeout, Secured)
    return NWKU_PingOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def NWKU_EchoUDP(
    device,
    # Unit length: 16 bytes
    DestinationIpAddress=bytearray(16),
    # Unit length: 16 bytes
    SourceIpAddress=bytearray(16),
    Payloadlength=bytearray(2),
    Timeout=bytearray(2),
    Iterations=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.NWKU_EchoUDPRequest(DestinationIpAddress, SourceIpAddress, Payloadlength, Timeout, Iterations)
    return NWKU_EchoUDPOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def NWKU_CPUReset(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=0
):
    request = Frames.NWKU_CPUResetRequest()
    op = NWKU_CPUResetOperation(
        device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True)

    op.begin(timeout)
    close_open_port(op, device, ack_policy)


def NWKU_CoapCreateInstance(
    device,
    UDPPort=bytearray(2),
    SocketDomain=NWKU_CoapCreateInstanceRequestSocketDomain(),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.NWKU_CoapCreateInstanceRequest(UDPPort, SocketDomain)
    return NWKU_CoapCreateInstanceOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def NWKU_CoapSend(
    device,
    InstanceID=bytearray(1),
    # Unit length: 16 bytes
    DestinationIpAddress=bytearray(16),
    UDPPort=bytearray(2),
    RequestType=NWKU_CoapSendRequestRequestType.CON,
    MessageType=NWKU_CoapSendRequestMessageType.GET,
    # Unit length: 30 bytes
    URIpath=bytearray(30),
    PayloadLength=bytearray(1),
    # Array length depends on PayloadLength. Mask: 0xFF. Shift: 0x00.
    Payload=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.NWKU_CoapSendRequest(
        InstanceID, DestinationIpAddress[::-1], UDPPort, RequestType, MessageType, URIpath, PayloadLength, Payload)
    return NWKU_CoapSendOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def NWKU_CoapRegister(
    device,
    InstanceID=bytearray(1),
    SocketDomain=NWKU_CoapRegisterRequestSocketDomain(),
    # Unit length: 30 bytes
    URIpath=bytearray(30),
    Port=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.NWKU_CoapRegisterRequest(InstanceID, SocketDomain, URIpath, Port)
    return NWKU_CoapRegisterOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def NWKU_DnsSend(
    device,
    # Unit length: 30 bytes
    DomainName=bytearray(30),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.NWKU_DnsSendRequestRequest(DomainName)
    return NWKU_DnsSendOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def NWKU_McastGroupShow(
    device,
    InterfaceId=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.NWKU_McastGroupShowRequest(InterfaceId)
    return NWKU_McastGroupShowOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def NWKU_McastGroupManage(
    device,
    InterfaceId=bytearray(1),
    Action=NWKU_McastGroupManageRequestAction.JoinGroup,
    # Unit length: 16 bytes
    McastAddress=bytearray(16),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.NWKU_McastGroupManageRequest(InterfaceId, Action, McastAddress)
    return NWKU_McastGroupManageOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def NWKU_RoutesShow(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.NWKU_RoutesShowRequest()
    return NWKU_RoutesShowOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def NWKU_RoutesManage(
    device,
    Action=NWKU_RoutesManageRequestAction.Add,
    # Unit length: 16 bytes
    Destination=bytearray(16),
    # Unit length: 16 bytes
    NextHop=bytearray(16),
    # Array length depends on Action. Mask: 0xFF. Shift: 0x00.
    RouteInfo=NWKU_RoutesManageRequest.Add(),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.NWKU_RoutesManageRequest(Action, Destination, NextHop, RouteInfo)
    return NWKU_RoutesManageOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def DTLSOpen(
    device,
    Maximumretransmissionscount=bytearray(1),
    Timeout=bytearray(2),
    LocalPort=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.DTLSOpenRequest(Maximumretransmissionscount, Timeout, LocalPort)
    return DTLSOpenOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def DTLSClose(
    device,
    ContextNumber=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.DTLSCloseRequest(ContextNumber)
    return DTLSCloseOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def DTLSClosePeer(
    device,
    PeerNumber=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.DTLSClosePeerRequest(PeerNumber)
    return DTLSClosePeerOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def DTLSConnect(
    device,
    ContextNumber=bytearray(1),
    # Unit length: 16 bytes
    ServerIPAddress=bytearray(16),
    ServerPort=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.DTLSConnectRequest(ContextNumber, ServerIPAddress, ServerPort)
    return DTLSConnectOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def DTLSSend(
    device,
    PeerNumber=bytearray(1),
    Size=bytearray(2),
    # Array length depends on Size. Mask: 0xFFFF. Shift: 0x0000.
    Data=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.DTLSSendRequest(PeerNumber, Size, Data)
    return DTLSSendOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def FSCIGetUniqueId(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.FSCIGetUniqueIdRequest()
    return FSCIGetUniqueIdOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def FSCIGetMcuId(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.FSCIGetMcuIdRequest()
    return FSCIGetMcuIdOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def FSCIGetSwVersions(
    device,
    ack_policy=FsciAckPolicy.NONE,
    protocol=Protocol.Sniffer,
    timeout=3
):
    request = Frames.FSCIGetSwVersionsRequest()
    return FSCIGetSwVersionsOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def SerialTun_LinkIndication(
    device,
    State=SerialTun_LinkIndicationRequestState.Down,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.SerialTun_LinkIndicationRequest(State)
    return SerialTun_LinkIndicationOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def SerialTun_IPPacketSend(
    device,
    Size=bytearray(2),
    # Array length depends on Size. Mask: 0xFFFF. Shift: 0x0000.
    Data=[],
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.SerialTun_IPPacketSendRequest(Size, Data)
    return SerialTun_IPPacketSendOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def Sniffer_MacSetPIBAttribute(
    device,
    PIBAttribute=Sniffer_MacSetPIBAttributeRequestPIBAttribute(),
    # Unit length: 7 bytes
    Value=bytearray(7),
    ack_policy=FsciAckPolicy.NONE,
    protocol=Protocol.Sniffer,
    timeout=3
):
    request = Frames.Sniffer_MacSetPIBAttributeRequest(PIBAttribute, Value)
    return Sniffer_MacSetPIBAttributeOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def Sniffer_Detect(
    device,
    ack_policy=FsciAckPolicy.NONE,
    protocol=Protocol.Sniffer,
    timeout=3
):
    request = Frames.Sniffer_DetectRequest()
    return Sniffer_DetectOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def FSCIEnterBootloader(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.FSCIEnterBootloaderRequest()
    return FSCIEnterBootloaderOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def AspSetPowerLevel(
    device,
    powerLevel=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.AspSetPowerLevelRequest(powerLevel)
    return AspSetPowerLevelOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def AspGetPowerLevel(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Thread,
    timeout=3
):
    request = Frames.AspGetPowerLevelRequest()
    return AspGetPowerLevelOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def check_for_usb(op, device):
    if (sys.platform.startswith('linux') and not device.startswith(('eth', '/dev/ttymxc', '/dev/spidev'))) or \
            sys.platform.startswith('win'):

        vid = op.comm.fsciFramer.dm.getDevice(device).deviceState.vid
        pid = op.comm.fsciFramer.dm.getDevice(device).deviceState.pid

        # Reopen port if Virtual Com Port
        if sys.platform.startswith('linux'):
            if (vid.lower() == '15a2' or vid.lower() == '1fc9') and pid.lower() == '0300':
                return True

        if sys.platform.startswith('win'):
            if (vid.lower() == 'vid_15a2' or vid.lower() == 'vid_1fc9') and pid.lower() == 'pid_0300':
                return True

    return False


def close_open_port(op, device, ack_policy):
    if(check_for_usb(op, device)):
        op.comm.fsciFramer.device.close()
        sleep(2)
        op.comm.fsciFramer.device.open(ack_policy)

    sleep(3)
