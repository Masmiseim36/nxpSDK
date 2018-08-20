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

from com.nxp.wireless_connectivity.commands.fsci_frame_description import FsciFrameDescription
from com.nxp.wireless_connectivity.commands.fsci_parameter import FsciParameter
from com.nxp.wireless_connectivity.hsdk.singleton import singleton


@singleton
class Spec(object):

    def __init__(self):
        self.MAC_MacFilteringAddEntryRequestFrame = self.InitMAC_MacFilteringAddEntryRequest()
        self.MAC_MacFilteringRemoveEntryRequestFrame = self.InitMAC_MacFilteringRemoveEntryRequest()
        self.MAC_MacFilteringEnableRequestFrame = self.InitMAC_MacFilteringEnableRequest()
        self.MAC_MacFilteringGetTableRequestFrame = self.InitMAC_MacFilteringGetTableRequest()
        self.THR_SetDeviceConfigRequestFrame = self.InitTHR_SetDeviceConfigRequest()
        self.THR_NwkScanRequestFrame = self.InitTHR_NwkScanRequest()
        self.THR_CreateNwkRequestFrame = self.InitTHR_CreateNwkRequest()
        self.THR_JoinRequestFrame = self.InitTHR_JoinRequest()
        self.THR_FactoryResetRequestFrame = self.InitTHR_FactoryResetRequest()
        self.THR_CpuResetRequestFrame = self.InitTHR_CpuResetRequest()
        self.THR_DisconnectRequestFrame = self.InitTHR_DisconnectRequest()
        self.THR_AttachRequestFrame = self.InitTHR_AttachRequest()
        self.THR_PromoteAsRouterRequestFrame = self.InitTHR_PromoteAsRouterRequest()
        self.THR_MgmtDiagnosticGetRequestFrame = self.InitTHR_MgmtDiagnosticGetRequest()
        self.THR_DiagTestGetRequestFrame = self.InitTHR_DiagTestGetRequest()
        self.THR_MgmtDiagnosticResetRequestFrame = self.InitTHR_MgmtDiagnosticResetRequest()
        self.THR_MgmtReadMemoryRequestFrame = self.InitTHR_MgmtReadMemoryRequest()
        self.THR_MgmtWriteMemoryRequestFrame = self.InitTHR_MgmtWriteMemoryRequest()
        self.THR_NwkDiscoveryRequestFrame = self.InitTHR_NwkDiscoveryRequest()
        self.THR_NwkDiscoveryStopRequestFrame = self.InitTHR_NwkDiscoveryStopRequest()
        self.THR_SearchNwkWithAnounceRequestFrame = self.InitTHR_SearchNwkWithAnounceRequest()
        self.THR_ChildUpdateToParentRequestFrame = self.InitTHR_ChildUpdateToParentRequest()
        self.THR_SetManualSlaacIIDRequestFrame = self.InitTHR_SetManualSlaacIIDRequest()
        self.THR_SendProactiveAddrNotifRequestFrame = self.InitTHR_SendProactiveAddrNotifRequest()
        self.THR_SetThresholdRequestFrame = self.InitTHR_SetThresholdRequest()
        self.THR_SetNwkIdTimeoutRequestFrame = self.InitTHR_SetNwkIdTimeoutRequest()
        self.THR_GetRoutingTableRequestFrame = self.InitTHR_GetRoutingTableRequest()
        self.THR_GetNeighborTableRequestFrame = self.InitTHR_GetNeighborTableRequest()
        self.THR_GetNeighborInfoRequestFrame = self.InitTHR_GetNeighborInfoRequest()
        self.THR_GetChildrenTableRequestFrame = self.InitTHR_GetChildrenTableRequest()
        self.THR_GetAttrRequestFrame = self.InitTHR_GetAttrRequest()
        self.THR_SetAttrRequestFrame = self.InitTHR_SetAttrRequest()
        self.THR_LeaderRemoveRouterIdRequestFrame = self.InitTHR_LeaderRemoveRouterIdRequest()
        self.THR_GenerateAllKeysRequestFrame = self.InitTHR_GenerateAllKeysRequest()
        self.THR_SwitchKeyRequestFrame = self.InitTHR_SwitchKeyRequest()
        self.THR_BrPrefixAddEntryRequestFrame = self.InitTHR_BrPrefixAddEntryRequest()
        self.THR_BrPrefixGetTableRequestFrame = self.InitTHR_BrPrefixGetTableRequest()
        self.THR_BrPrefixRemoveEntryRequestFrame = self.InitTHR_BrPrefixRemoveEntryRequest()
        self.THR_BrServiceRemoveEntryRequestFrame = self.InitTHR_BrServiceRemoveEntryRequest()
        self.THR_BrServiceAddEntryRequestFrame = self.InitTHR_BrServiceAddEntryRequest()
        self.THR_BrPrefixSyncRequestFrame = self.InitTHR_BrPrefixSyncRequest()
        self.THR_BrPrefixRemoveAllRequestFrame = self.InitTHR_BrPrefixRemoveAllRequest()
        self.THR_GetThreadIpAddrRequestFrame = self.InitTHR_GetThreadIpAddrRequest()
        self.THR_GetParentRequestFrame = self.InitTHR_GetParentRequest()
        self.NWKU_EidToRlocMapRequestFrame = self.InitNWKU_EidToRlocMapRequest()
        self.THR_IdentifyRequestFrame = self.InitTHR_IdentifyRequest()
        self.SocketCreateRequestFrame = self.InitSocketCreateRequest()
        self.SocketShutdownRequestFrame = self.InitSocketShutdownRequest()
        self.SocketBindRequestFrame = self.InitSocketBindRequest()
        self.SocketSendRequestFrame = self.InitSocketSendRequest()
        self.SocketSendToRequestFrame = self.InitSocketSendToRequest()
        self.SocketReceiveRequestFrame = self.InitSocketReceiveRequest()
        self.SocketReceiveFromRequestFrame = self.InitSocketReceiveFromRequest()
        self.SocketConnectRequestFrame = self.InitSocketConnectRequest()
        self.SocketListenRequestFrame = self.InitSocketListenRequest()
        self.SocketAcceptRequestFrame = self.InitSocketAcceptRequest()
        self.SocketSetOptionRequestFrame = self.InitSocketSetOptionRequest()
        self.SocketGetOptionRequestFrame = self.InitSocketGetOptionRequest()
        self.MESHCOP_StartCommissionerRequestFrame = self.InitMESHCOP_StartCommissionerRequest()
        self.MESHCOP_StartNativeCommissionerRequestFrame = self.InitMESHCOP_StartNativeCommissionerRequest()
        self.MESHCOP_StopCommissionerRequestFrame = self.InitMESHCOP_StopCommissionerRequest()
        self.MESHCOP_AddExpectedJoinerRequestFrame = self.InitMESHCOP_AddExpectedJoinerRequest()
        self.MESHCOP_GetExpectedJoinerRequestFrame = self.InitMESHCOP_GetExpectedJoinerRequest()
        self.MESHCOP_RemoveExpectedJoinerRequestFrame = self.InitMESHCOP_RemoveExpectedJoinerRequest()
        self.MESHCOP_RemoveAllExpectedJoinersRequestFrame = self.InitMESHCOP_RemoveAllExpectedJoinersRequest()
        self.MESHCOP_SyncSteeringDataRequestFrame = self.InitMESHCOP_SyncSteeringDataRequest()
        self.MESHCOP_MgmtSetRequestFrame = self.InitMESHCOP_MgmtSetRequest()
        self.MESHCOP_MgmtGetRequestFrame = self.InitMESHCOP_MgmtGetRequest()
        self.MESHCOP_SetCommissionerCredentialRequestFrame = self.InitMESHCOP_SetCommissionerCredentialRequest()
        self.MESHCOP_MgmNwkFormRequestFrame = self.InitMESHCOP_MgmNwkFormRequest()
        self.MESHCOP_MgmtCommissionerGetRequestFrame = self.InitMESHCOP_MgmtCommissionerGetRequest()
        self.MESHCOP_MgmtActiveGetRequestFrame = self.InitMESHCOP_MgmtActiveGetRequest()
        self.MESHCOP_MgmtPendingGetRequestFrame = self.InitMESHCOP_MgmtPendingGetRequest()
        self.MESHCOP_MgmtCommissionerSetRequestFrame = self.InitMESHCOP_MgmtCommissionerSetRequest()
        self.MESHCOP_MgmtActiveSetRequestFrame = self.InitMESHCOP_MgmtActiveSetRequest()
        self.MESHCOP_MgmtPendingSetRequestFrame = self.InitMESHCOP_MgmtPendingSetRequest()
        self.MESHCOP_MgmtSendPanIdQueryRequestFrame = self.InitMESHCOP_MgmtSendPanIdQueryRequest()
        self.MESHCOP_MgmtSendEdScanRequestFrame = self.InitMESHCOP_MgmtSendEdScanRequest()
        self.MESHCOP_MgmtSendAnnounceBeginRequestFrame = self.InitMESHCOP_MgmtSendAnnounceBeginRequest()
        self.NWKU_IfconfigBindRequestFrame = self.InitNWKU_IfconfigBindRequest()
        self.NWKU_IfconfigAllRequestFrame = self.InitNWKU_IfconfigAllRequest()
        self.NWKU_PingRequestFrame = self.InitNWKU_PingRequest()
        self.NWKU_EchoUDPRequestFrame = self.InitNWKU_EchoUDPRequest()
        self.NWKU_CPUResetRequestFrame = self.InitNWKU_CPUResetRequest()
        self.NWKU_CoapCreateInstanceRequestFrame = self.InitNWKU_CoapCreateInstanceRequest()
        self.NWKU_CoapSendRequestFrame = self.InitNWKU_CoapSendRequest()
        self.NWKU_CoapRegisterRequestFrame = self.InitNWKU_CoapRegisterRequest()
        self.NWKU_DnsSendRequestRequestFrame = self.InitNWKU_DnsSendRequestRequest()
        self.NWKU_McastGroupShowRequestFrame = self.InitNWKU_McastGroupShowRequest()
        self.NWKU_McastGroupManageRequestFrame = self.InitNWKU_McastGroupManageRequest()
        self.NWKU_RoutesShowRequestFrame = self.InitNWKU_RoutesShowRequest()
        self.NWKU_RoutesManageRequestFrame = self.InitNWKU_RoutesManageRequest()
        self.DTLSOpenRequestFrame = self.InitDTLSOpenRequest()
        self.DTLSCloseRequestFrame = self.InitDTLSCloseRequest()
        self.DTLSClosePeerRequestFrame = self.InitDTLSClosePeerRequest()
        self.DTLSConnectRequestFrame = self.InitDTLSConnectRequest()
        self.DTLSSendRequestFrame = self.InitDTLSSendRequest()
        self.FSCIGetUniqueIdRequestFrame = self.InitFSCIGetUniqueIdRequest()
        self.FSCIGetMcuIdRequestFrame = self.InitFSCIGetMcuIdRequest()
        self.FSCIGetSwVersionsRequestFrame = self.InitFSCIGetSwVersionsRequest()
        self.SerialTun_LinkIndicationRequestFrame = self.InitSerialTun_LinkIndicationRequest()
        self.SerialTun_IPPacketSendRequestFrame = self.InitSerialTun_IPPacketSendRequest()
        self.Sniffer_MacSetPIBAttributeRequestFrame = self.InitSniffer_MacSetPIBAttributeRequest()
        self.Sniffer_DetectRequestFrame = self.InitSniffer_DetectRequest()
        self.FSCIACKFrame = self.InitFSCIACK()
        self.FSCIEnterBootloaderRequestFrame = self.InitFSCIEnterBootloaderRequest()
        self.AspSetPowerLevelRequestFrame = self.InitAspSetPowerLevelRequest()
        self.AspGetPowerLevelRequestFrame = self.InitAspGetPowerLevelRequest()
        self.SocketCreateConfirmFrame = self.InitSocketCreateConfirm()
        self.SocketShutdownConfirmFrame = self.InitSocketShutdownConfirm()
        self.SocketBindConfirmFrame = self.InitSocketBindConfirm()
        self.SocketSendConfirmFrame = self.InitSocketSendConfirm()
        self.SocketSendToConfirmFrame = self.InitSocketSendToConfirm()
        self.SocketReceiveConfirmFrame = self.InitSocketReceiveConfirm()
        self.SocketReceiveFromConfirmFrame = self.InitSocketReceiveFromConfirm()
        self.SocketConnectConfirmFrame = self.InitSocketConnectConfirm()
        self.SocketListenConfirmFrame = self.InitSocketListenConfirm()
        self.SocketAcceptConfirmFrame = self.InitSocketAcceptConfirm()
        self.SocketSetOptionConfirmFrame = self.InitSocketSetOptionConfirm()
        self.SocketGetOptionConfirmFrame = self.InitSocketGetOptionConfirm()
        self.MAC_MacFilteringAddEntryConfirmFrame = self.InitMAC_MacFilteringAddEntryConfirm()
        self.MAC_MacFilteringRemoveEntryConfirmFrame = self.InitMAC_MacFilteringRemoveEntryConfirm()
        self.MAC_MacFilteringEnableConfirmFrame = self.InitMAC_MacFilteringEnableConfirm()
        self.MAC_MacFilteringGetTableConfirmFrame = self.InitMAC_MacFilteringGetTableConfirm()
        self.THR_SetDeviceConfigConfirmFrame = self.InitTHR_SetDeviceConfigConfirm()
        self.THR_NwkScanConfirmFrame = self.InitTHR_NwkScanConfirm()
        self.THR_CreateNwkConfirmFrame = self.InitTHR_CreateNwkConfirm()
        self.THR_JoinConfirmFrame = self.InitTHR_JoinConfirm()
        self.THR_FactoryResetConfirmFrame = self.InitTHR_FactoryResetConfirm()
        self.THR_CpuResetConfirmFrame = self.InitTHR_CpuResetConfirm()
        self.THR_CpuResetIndicationFrame = self.InitTHR_CpuResetIndication()
        self.THR_DisconnectConfirmFrame = self.InitTHR_DisconnectConfirm()
        self.THR_AttachConfirmFrame = self.InitTHR_AttachConfirm()
        self.THR_PromoteAsRouterConfirmFrame = self.InitTHR_PromoteAsRouterConfirm()
        self.THR_EventNwkScanConfirmFrame = self.InitTHR_EventNwkScanConfirm()
        self.THR_EventNwkCreateConfirmFrame = self.InitTHR_EventNwkCreateConfirm()
        self.THR_EventNwkJoinConfirmFrame = self.InitTHR_EventNwkJoinConfirm()
        self.THR_EventNwkJoinSelectParentsConfirmFrame = self.InitTHR_EventNwkJoinSelectParentsConfirm()
        self.THR_EventGeneralConfirmFrame = self.InitTHR_EventGeneralConfirm()
        self.THR_EventNwkCommissioningIndicationFrame = self.InitTHR_EventNwkCommissioningIndication()
        self.THR_CommissioningDiagnosticIndicationFrame = self.InitTHR_CommissioningDiagnosticIndication()
        self.THR_MgmtDiagnosticGetConfirmFrame = self.InitTHR_MgmtDiagnosticGetConfirm()
        self.THR_DiagTestGetConfirmFrame = self.InitTHR_DiagTestGetConfirm()
        self.THR_MgmtDiagnosticResetConfirmFrame = self.InitTHR_MgmtDiagnosticResetConfirm()
        self.THR_MgmtReadMemoryConfirmFrame = self.InitTHR_MgmtReadMemoryConfirm()
        self.THR_MgmtWriteMemoryConfirmFrame = self.InitTHR_MgmtWriteMemoryConfirm()
        self.THR_SetManualSlaacIIDConfirmFrame = self.InitTHR_SetManualSlaacIIDConfirm()
        self.THR_SendProactiveAddrNotifConfirmFrame = self.InitTHR_SendProactiveAddrNotifConfirm()
        self.THR_NwkDiscoveryConfirmFrame = self.InitTHR_NwkDiscoveryConfirm()
        self.THR_NwkDiscoveryStopConfirmFrame = self.InitTHR_NwkDiscoveryStopConfirm()
        self.THR_SearchNwkWithAnounceConfirmFrame = self.InitTHR_SearchNwkWithAnounceConfirm()
        self.THR_MgmtDiagnosticGetRspIndicationFrame = self.InitTHR_MgmtDiagnosticGetRspIndication()
        self.THR_DiagTestGetRspIndicationFrame = self.InitTHR_DiagTestGetRspIndication()
        self.THR_MgmtDiagnosticDiagTestEventIndicationFrame = self.InitTHR_MgmtDiagnosticDiagTestEventIndication()
        self.THR_MgmtDiagnosticResetRspIndicationFrame = self.InitTHR_MgmtDiagnosticResetRspIndication()
        self.THR_SetNwkIdTimeoutConfirmFrame = self.InitTHR_SetNwkIdTimeoutConfirm()
        self.THR_SetThresholdConfirmFrame = self.InitTHR_SetThresholdConfirm()
        self.THR_GetNeighborInfoConfirmFrame = self.InitTHR_GetNeighborInfoConfirm()
        self.THR_GetChildrenTableConfirmFrame = self.InitTHR_GetChildrenTableConfirm()
        self.THR_GetNeighborTableConfirmFrame = self.InitTHR_GetNeighborTableConfirm()
        self.THR_GetRoutingTableConfirmFrame = self.InitTHR_GetRoutingTableConfirm()
        self.THR_GetAttrConfirmFrame = self.InitTHR_GetAttrConfirm()
        self.THR_SetAttrConfirmFrame = self.InitTHR_SetAttrConfirm()
        self.THR_GetThreadIpAddrConfirmFrame = self.InitTHR_GetThreadIpAddrConfirm()
        self.THR_GetParentConfirmFrame = self.InitTHR_GetParentConfirm()
        self.THR_ChildUpdateToParentConfirmFrame = self.InitTHR_ChildUpdateToParentConfirm()
        self.THR_LeaderRemoveRouterIdConfirmFrame = self.InitTHR_LeaderRemoveRouterIdConfirm()
        self.THR_GenerateAllKeysConfirmFrame = self.InitTHR_GenerateAllKeysConfirm()
        self.THR_SwitchKeyKeyConfirmFrame = self.InitTHR_SwitchKeyKeyConfirm()
        self.THR_BrPrefixAddEntryConfirmFrame = self.InitTHR_BrPrefixAddEntryConfirm()
        self.THR_BrPrefixGetTableConfirmFrame = self.InitTHR_BrPrefixGetTableConfirm()
        self.THR_BrPrefixRemoveEntryConfirmFrame = self.InitTHR_BrPrefixRemoveEntryConfirm()
        self.THR_BrServiceRemoveEntryConfirmFrame = self.InitTHR_BrServiceRemoveEntryConfirm()
        self.THR_BrServiceAddEntryConfirmFrame = self.InitTHR_BrServiceAddEntryConfirm()
        self.THR_BrPrefixSyncConfirmFrame = self.InitTHR_BrPrefixSyncConfirm()
        self.THR_BrPrefixRemoveAllConfirmFrame = self.InitTHR_BrPrefixRemoveAllConfirm()
        self.NWKU_EidToRlocMapResponseFrame = self.InitNWKU_EidToRlocMapResponse()
        self.THR_IdentifyConfirmFrame = self.InitTHR_IdentifyConfirm()
        self.NWKU_IfconfigBindConfirmFrame = self.InitNWKU_IfconfigBindConfirm()
        self.NWKU_IfconfigAllResponseFrame = self.InitNWKU_IfconfigAllResponse()
        self.NWKU_PingConfirmFrame = self.InitNWKU_PingConfirm()
        self.NWKU_EchoUDPConfirmFrame = self.InitNWKU_EchoUDPConfirm()
        self.NWKU_CoapMsgReceivedIndicationFrame = self.InitNWKU_CoapMsgReceivedIndication()
        self.NWKU_DnsResponseReceivedIndicationFrame = self.InitNWKU_DnsResponseReceivedIndication()
        self.NWKU_CoapRegisterConfirmFrame = self.InitNWKU_CoapRegisterConfirm()
        self.NWKU_CoapCreateInstanceConfirmFrame = self.InitNWKU_CoapCreateInstanceConfirm()
        self.NWKU_CoapSendConfirmFrame = self.InitNWKU_CoapSendConfirm()
        self.NWKU_DnsSendRequestConfirmFrame = self.InitNWKU_DnsSendRequestConfirm()
        self.NWKU_McastGroupManageConfirmFrame = self.InitNWKU_McastGroupManageConfirm()
        self.NWKU_McastGroupShowResponseFrame = self.InitNWKU_McastGroupShowResponse()
        self.NWKU_RoutesManageConfirmFrame = self.InitNWKU_RoutesManageConfirm()
        self.NWKU_RoutesShowResponseFrame = self.InitNWKU_RoutesShowResponse()
        self.MESHCOP_StartCommissionerConfirmFrame = self.InitMESHCOP_StartCommissionerConfirm()
        self.MESHCOP_StartNativeCommissionerConfirmFrame = self.InitMESHCOP_StartNativeCommissionerConfirm()
        self.MESHCOP_StopCommissionerConfirmFrame = self.InitMESHCOP_StopCommissionerConfirm()
        self.MESHCOP_AddExpectedJoinerConfirmFrame = self.InitMESHCOP_AddExpectedJoinerConfirm()
        self.MESHCOP_GetExpectedJoinerConfirmFrame = self.InitMESHCOP_GetExpectedJoinerConfirm()
        self.MESHCOP_RemoveExpectedJoinerConfirmFrame = self.InitMESHCOP_RemoveExpectedJoinerConfirm()
        self.MESHCOP_RemoveAllExpectedJoinersConfirmFrame = self.InitMESHCOP_RemoveAllExpectedJoinersConfirm()
        self.MESHCOP_SyncSteeringDataConfirmFrame = self.InitMESHCOP_SyncSteeringDataConfirm()
        self.MESHCOP_MgmtSetConfirmFrame = self.InitMESHCOP_MgmtSetConfirm()
        self.MESHCOP_MgmtGetConfirmFrame = self.InitMESHCOP_MgmtGetConfirm()
        self.MESHCOP_SetCommissionerCredentialConfirmFrame = self.InitMESHCOP_SetCommissionerCredentialConfirm()
        self.MESHCOP_MgmNwkFormConfirmFrame = self.InitMESHCOP_MgmNwkFormConfirm()
        self.MESHCOP_MgmtCommissionerGetConfirmFrame = self.InitMESHCOP_MgmtCommissionerGetConfirm()
        self.MESHCOP_MgmtActiveGetConfirmFrame = self.InitMESHCOP_MgmtActiveGetConfirm()
        self.MESHCOP_MgmtPendingGetConfirmFrame = self.InitMESHCOP_MgmtPendingGetConfirm()
        self.MESHCOP_MgmtCommissionerSetConfirmFrame = self.InitMESHCOP_MgmtCommissionerSetConfirm()
        self.MESHCOP_MgmtActiveSetConfirmFrame = self.InitMESHCOP_MgmtActiveSetConfirm()
        self.MESHCOP_MgmtPendingSetConfirmFrame = self.InitMESHCOP_MgmtPendingSetConfirm()
        self.MESHCOP_MgmtSendPanIdQueryConfirmFrame = self.InitMESHCOP_MgmtSendPanIdQueryConfirm()
        self.MESHCOP_MgmtPanIdConflictConfirmFrame = self.InitMESHCOP_MgmtPanIdConflictConfirm()
        self.MESHCOP_MgmtSendEdScanConfirmFrame = self.InitMESHCOP_MgmtSendEdScanConfirm()
        self.MESHCOP_MgmtEdReportConfirmFrame = self.InitMESHCOP_MgmtEdReportConfirm()
        self.MESHCOP_MgmtSendAnnounceBeginConfirmFrame = self.InitMESHCOP_MgmtSendAnnounceBeginConfirm()
        self.DTLSOpenConfirmFrame = self.InitDTLSOpenConfirm()
        self.DTLSCloseConfirmFrame = self.InitDTLSCloseConfirm()
        self.DTLSClosePeerConfirmFrame = self.InitDTLSClosePeerConfirm()
        self.DTLSConnectConfirmFrame = self.InitDTLSConnectConfirm()
        self.DTLSClientConnectedConfirmFrame = self.InitDTLSClientConnectedConfirm()
        self.DTLSSendConfirmFrame = self.InitDTLSSendConfirm()
        self.DTLSReceiveConfirmFrame = self.InitDTLSReceiveConfirm()
        self.FSCIGetUniqueIdConfirmFrame = self.InitFSCIGetUniqueIdConfirm()
        self.FSCIGetMcuIdConfirmFrame = self.InitFSCIGetMcuIdConfirm()
        self.FSCIGetSwVersionsConfirmFrame = self.InitFSCIGetSwVersionsConfirm()
        self.Sniffer_MacSetPIBAttributeConfirmFrame = self.InitSniffer_MacSetPIBAttributeConfirm()
        self.Sniffer_DetectConfirmFrame = self.InitSniffer_DetectConfirm()
        self.MAC_PromiscuousRxIndicationFrame = self.InitMAC_PromiscuousRxIndication()
        self.SerialTun_LinkIndicationConfirmFrame = self.InitSerialTun_LinkIndicationConfirm()
        self.SerialTun_IPPacketReceivedConfirmFrame = self.InitSerialTun_IPPacketReceivedConfirm()
        self.AspSetPowerLevelConfirmFrame = self.InitAspSetPowerLevelConfirm()
        self.AspGetPowerLevelConfirmFrame = self.InitAspGetPowerLevelConfirm()
        self.DBGConfirmFrame = self.InitDBGConfirm()

    def InitMAC_MacFilteringAddEntryRequest(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        ExtendedAddress = FsciParameter("ExtendedAddress", 8)
        cmdParams.append(ExtendedAddress)
        ShortAddress = FsciParameter("ShortAddress", 2)
        cmdParams.append(ShortAddress)
        LinkIndicator = FsciParameter("LinkIndicator", 1)
        cmdParams.append(LinkIndicator)
        BlockNeighbor = FsciParameter("BlockNeighbor", 1)
        cmdParams.append(BlockNeighbor)
        return FsciFrameDescription(0xCE, 0x12, cmdParams)

    def InitMAC_MacFilteringRemoveEntryRequest(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        ExtendedAddress = FsciParameter("ExtendedAddress", 8)
        cmdParams.append(ExtendedAddress)
        return FsciFrameDescription(0xCE, 0x13, cmdParams)

    def InitMAC_MacFilteringEnableRequest(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        MacFiltering = FsciParameter("MacFiltering", 1)
        cmdParams.append(MacFiltering)
        return FsciFrameDescription(0xCE, 0x14, cmdParams)

    def InitMAC_MacFilteringGetTableRequest(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        StartIndex = FsciParameter("StartIndex", 1)
        cmdParams.append(StartIndex)
        NoOfElements = FsciParameter("NoOfElements", 1)
        cmdParams.append(NoOfElements)
        return FsciFrameDescription(0xCE, 0x15, cmdParams)

    def InitTHR_SetDeviceConfigRequest(self):
        cmdParams = []
        ThrInstanceID = FsciParameter("ThrInstanceID", 1)
        cmdParams.append(ThrInstanceID)
        OutOfBandPreconfigured = FsciParameter("OutOfBandPreconfigured", 1)
        cmdParams.append(OutOfBandPreconfigured)
        outOfBandChannel = FsciParameter("outOfBandChannel", 1)
        cmdParams.append(outOfBandChannel)
        PanId = FsciParameter("PanId", 2)
        cmdParams.append(PanId)
        ScanChannels = FsciParameter("ScanChannels", 4)
        cmdParams.append(ScanChannels)
        ExtendedPanId = FsciParameter("ExtendedPanId", 8)
        cmdParams.append(ExtendedPanId)
        NwkNameSize = FsciParameter("NwkNameSize", 1)
        cmdParams.append(NwkNameSize)
        NwkName = FsciParameter("NwkName", 1, NwkNameSize)
        cmdParams.append(NwkName)
        MLPrefix = FsciParameter("MLPrefix", 16)
        cmdParams.append(MLPrefix)
        MLprefixSizeInBits = FsciParameter("MLprefixSizeInBits", 1)
        cmdParams.append(MLprefixSizeInBits)
        MasterKey = FsciParameter("MasterKey", 16)
        cmdParams.append(MasterKey)
        return FsciFrameDescription(0xCE, 0x16, cmdParams)

    def InitTHR_NwkScanRequest(self):
        cmdParams = []
        InstanceID = FsciParameter("InstanceID", 1)
        cmdParams.append(InstanceID)
        ScanChannelMask = FsciParameter("ScanChannelMask", 4)
        cmdParams.append(ScanChannelMask)
        ScanType = FsciParameter("ScanType", 1)
        cmdParams.append(ScanType)
        ScanDuration = FsciParameter("ScanDuration", 1)
        cmdParams.append(ScanDuration)
        maxThrNwkToDiscover = FsciParameter("maxThrNwkToDiscover", 2)
        cmdParams.append(maxThrNwkToDiscover)
        return FsciFrameDescription(0xCE, 0x1A, cmdParams)

    def InitTHR_CreateNwkRequest(self):
        cmdParams = []
        InstanceID = FsciParameter("InstanceID", 1)
        cmdParams.append(InstanceID)
        return FsciFrameDescription(0xCE, 0x1B, cmdParams)

    def InitTHR_JoinRequest(self):
        cmdParams = []
        InstanceID = FsciParameter("InstanceID", 1)
        cmdParams.append(InstanceID)
        discoveryMethod = FsciParameter("discoveryMethod", 1)
        cmdParams.append(discoveryMethod)
        return FsciFrameDescription(0xCE, 0x1C, cmdParams)

    def InitTHR_FactoryResetRequest(self):
        cmdParams = []
        return FsciFrameDescription(0xCE, 0x1F, cmdParams)

    def InitTHR_CpuResetRequest(self):
        cmdParams = []
        TimeoutMs = FsciParameter("TimeoutMs", 4)
        cmdParams.append(TimeoutMs)
        return FsciFrameDescription(0xCE, 0x21, cmdParams)

    def InitTHR_DisconnectRequest(self):
        cmdParams = []
        InstanceID = FsciParameter("InstanceID", 1)
        cmdParams.append(InstanceID)
        return FsciFrameDescription(0xCE, 0x1D, cmdParams)

    def InitTHR_AttachRequest(self):
        cmdParams = []
        InstanceID = FsciParameter("InstanceID", 1)
        cmdParams.append(InstanceID)
        return FsciFrameDescription(0xCE, 0x80, cmdParams)

    def InitTHR_PromoteAsRouterRequest(self):
        cmdParams = []
        InstanceID = FsciParameter("InstanceID", 1)
        cmdParams.append(InstanceID)
        Reason = FsciParameter("Reason", 1)
        cmdParams.append(Reason)
        return FsciFrameDescription(0xCE, 0x94, cmdParams)

    def InitTHR_MgmtDiagnosticGetRequest(self):
        cmdParams = []
        # not generated, pickle() is used instead; see frames.py
        return FsciFrameDescription(0xCE, 0x61, cmdParams)

    def InitTHR_DiagTestGetRequest(self):
        cmdParams = []
        # not generated, pickle() is used instead; see frames.py
        return FsciFrameDescription(0xCE, 0x67, cmdParams)

    def InitTHR_MgmtDiagnosticResetRequest(self):
        cmdParams = []
        # not generated, pickle() is used instead; see frames.py
        return FsciFrameDescription(0xCE, 0x62, cmdParams)

    def InitTHR_MgmtReadMemoryRequest(self):
        cmdParams = []
        Address = FsciParameter("Address", 4)
        cmdParams.append(Address)
        Length = FsciParameter("Length", 1)
        cmdParams.append(Length)
        return FsciFrameDescription(0xCE, 0x65, cmdParams)

    def InitTHR_MgmtWriteMemoryRequest(self):
        cmdParams = []
        Address = FsciParameter("Address", 4)
        cmdParams.append(Address)
        Length = FsciParameter("Length", 1)
        cmdParams.append(Length)
        Value = FsciParameter("Value", 1, Length)
        cmdParams.append(Value)
        return FsciFrameDescription(0xCE, 0x66, cmdParams)

    def InitTHR_NwkDiscoveryRequest(self):
        cmdParams = []
        InstanceID = FsciParameter("InstanceID", 1)
        cmdParams.append(InstanceID)
        return FsciFrameDescription(0xCE, 0xC0, cmdParams)

    def InitTHR_NwkDiscoveryStopRequest(self):
        cmdParams = []
        InstanceID = FsciParameter("InstanceID", 1)
        cmdParams.append(InstanceID)
        return FsciFrameDescription(0xCE, 0xC1, cmdParams)

    def InitTHR_SearchNwkWithAnounceRequest(self):
        cmdParams = []
        InstanceID = FsciParameter("InstanceID", 1)
        cmdParams.append(InstanceID)
        return FsciFrameDescription(0xCE, 0xC2, cmdParams)

    def InitTHR_ChildUpdateToParentRequest(self):
        cmdParams = []
        InstanceID = FsciParameter("InstanceID", 1)
        cmdParams.append(InstanceID)
        return FsciFrameDescription(0xCE, 0x2F, cmdParams)

    def InitTHR_SetManualSlaacIIDRequest(self):
        cmdParams = []
        IID = FsciParameter("IID", 8)
        cmdParams.append(IID)
        return FsciFrameDescription(0xCE, 0x75, cmdParams)

    def InitTHR_SendProactiveAddrNotifRequest(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        DestinationIPAddress = FsciParameter("DestinationIPAddress", 16)
        cmdParams.append(DestinationIPAddress)
        return FsciFrameDescription(0xCE, 0x76, cmdParams)

    def InitTHR_SetThresholdRequest(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        ThresholdType = FsciParameter("ThresholdType", 1)
        cmdParams.append(ThresholdType)
        Value = FsciParameter("Value", 1)
        cmdParams.append(Value)
        return FsciFrameDescription(0xCE, 0x20, cmdParams)

    def InitTHR_SetNwkIdTimeoutRequest(self):
        cmdParams = []
        TimeoutInSeconds = FsciParameter("TimeoutInSeconds", 4)
        cmdParams.append(TimeoutInSeconds)
        return FsciFrameDescription(0xCE, 0x2E, cmdParams)

    def InitTHR_GetRoutingTableRequest(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        StartIndex = FsciParameter("StartIndex", 1)
        cmdParams.append(StartIndex)
        NoOfElements = FsciParameter("NoOfElements", 1)
        cmdParams.append(NoOfElements)
        return FsciFrameDescription(0xCE, 0x25, cmdParams)

    def InitTHR_GetNeighborTableRequest(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        StartIndex = FsciParameter("StartIndex", 1)
        cmdParams.append(StartIndex)
        NoOfElements = FsciParameter("NoOfElements", 1)
        cmdParams.append(NoOfElements)
        return FsciFrameDescription(0xCE, 0x24, cmdParams)

    def InitTHR_GetNeighborInfoRequest(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        ShortAddress = FsciParameter("ShortAddress", 2)
        cmdParams.append(ShortAddress)
        return FsciFrameDescription(0xCE, 0x10, cmdParams)

    def InitTHR_GetChildrenTableRequest(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        StartIndex = FsciParameter("StartIndex", 1)
        cmdParams.append(StartIndex)
        NoOfElements = FsciParameter("NoOfElements", 1)
        cmdParams.append(NoOfElements)
        return FsciFrameDescription(0xCE, 0x23, cmdParams)

    def InitTHR_GetAttrRequest(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        AttributeId = FsciParameter("AttributeId", 1)
        cmdParams.append(AttributeId)
        Index = FsciParameter("Index", 1)
        cmdParams.append(Index)
        return FsciFrameDescription(0xCE, 0x17, cmdParams)

    def InitTHR_SetAttrRequest(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        AttributeId = FsciParameter("AttributeId", 1)
        cmdParams.append(AttributeId)
        Index = FsciParameter("Index", 1)
        cmdParams.append(Index)
        AttrSize = FsciParameter("AttrSize", 1)
        cmdParams.append(AttrSize)
        AttributeValue = FsciParameter("AttributeValue", 1, AttrSize)
        cmdParams.append(AttributeValue)
        return FsciFrameDescription(0xCE, 0x18, cmdParams)

    def InitTHR_LeaderRemoveRouterIdRequest(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        RouterShortAddr = FsciParameter("RouterShortAddr", 2)
        cmdParams.append(RouterShortAddr)
        return FsciFrameDescription(0xCE, 0x26, cmdParams)

    def InitTHR_GenerateAllKeysRequest(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        KeySequenceCounter = FsciParameter("KeySequenceCounter", 4)
        cmdParams.append(KeySequenceCounter)
        return FsciFrameDescription(0xCE, 0x28, cmdParams)

    def InitTHR_SwitchKeyRequest(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        KeySequenceCounter = FsciParameter("KeySequenceCounter", 4)
        cmdParams.append(KeySequenceCounter)
        return FsciFrameDescription(0xCE, 0x27, cmdParams)

    def InitTHR_BrPrefixAddEntryRequest(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        prefixLength = FsciParameter("prefixLength", 1)
        cmdParams.append(prefixLength)
        PrefixValue = FsciParameter("PrefixValue", 16)
        cmdParams.append(PrefixValue)
        PrefixFlagsReserved = FsciParameter("PrefixFlagsReserved", 1)
        cmdParams.append(PrefixFlagsReserved)
        PrefixFlags = FsciParameter("PrefixFlags", 1)
        cmdParams.append(PrefixFlags)
        prefixLifetime = FsciParameter("prefixLifetime", 4)
        cmdParams.append(prefixLifetime)
        prefixAdvertised = FsciParameter("prefixAdvertised", 1)
        cmdParams.append(prefixAdvertised)
        ExternalRouteFlags = FsciParameter("ExternalRouteFlags", 1)
        cmdParams.append(ExternalRouteFlags)
        ExternalRouteLifetime = FsciParameter("ExternalRouteLifetime", 4)
        cmdParams.append(ExternalRouteLifetime)
        ExternalRouteAdvertised = FsciParameter("ExternalRouteAdvertised", 1)
        cmdParams.append(ExternalRouteAdvertised)
        return FsciFrameDescription(0xCE, 0x29, cmdParams)

    def InitTHR_BrPrefixGetTableRequest(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        StartIndex = FsciParameter("StartIndex", 1)
        cmdParams.append(StartIndex)
        NoOfElements = FsciParameter("NoOfElements", 1)
        cmdParams.append(NoOfElements)
        return FsciFrameDescription(0xCE, 0x2A, cmdParams)

    def InitTHR_BrPrefixRemoveEntryRequest(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        prefixLength = FsciParameter("prefixLength", 1)
        cmdParams.append(prefixLength)
        PrefixValue = FsciParameter("PrefixValue", 16)
        cmdParams.append(PrefixValue)
        return FsciFrameDescription(0xCE, 0x2B, cmdParams)

    def InitTHR_BrServiceRemoveEntryRequest(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        ServiceName = FsciParameter("ServiceName", 9)
        cmdParams.append(ServiceName)
        ServerAddress = FsciParameter("ServerAddress", 16)
        cmdParams.append(ServerAddress)
        return FsciFrameDescription(0xCE, 0x86, cmdParams)

    def InitTHR_BrServiceAddEntryRequest(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        ServiceName = FsciParameter("ServiceName", 9)
        cmdParams.append(ServiceName)
        ServerAddress = FsciParameter("ServerAddress", 16)
        cmdParams.append(ServerAddress)
        return FsciFrameDescription(0xCE, 0x85, cmdParams)

    def InitTHR_BrPrefixSyncRequest(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        return FsciFrameDescription(0xCE, 0x2C, cmdParams)

    def InitTHR_BrPrefixRemoveAllRequest(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        return FsciFrameDescription(0xCE, 0x2D, cmdParams)

    def InitTHR_GetThreadIpAddrRequest(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        AddressType = FsciParameter("AddressType", 1)
        cmdParams.append(AddressType)
        Data = FsciParameter("Data", 2)
        cmdParams.append(Data)
        return FsciFrameDescription(0xCE, 0x19, cmdParams)

    def InitTHR_GetParentRequest(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        return FsciFrameDescription(0xCE, 0x1E, cmdParams)

    def InitNWKU_EidToRlocMapRequest(self):
        cmdParams = []
        return FsciFrameDescription(0xCE, 0x6A, cmdParams)

    def InitTHR_IdentifyRequest(self):
        cmdParams = []
        return FsciFrameDescription(0xCE, 0x69, cmdParams)

    def InitSocketCreateRequest(self):
        cmdParams = []
        SocketDomain = FsciParameter("SocketDomain", 1)
        cmdParams.append(SocketDomain)
        SocketType = FsciParameter("SocketType", 1)
        cmdParams.append(SocketType)
        SocketProtocol = FsciParameter("SocketProtocol", 1)
        cmdParams.append(SocketProtocol)
        return FsciFrameDescription(0xCE, 0x00, cmdParams)

    def InitSocketShutdownRequest(self):
        cmdParams = []
        SocketIndex = FsciParameter("SocketIndex", 1)
        cmdParams.append(SocketIndex)
        return FsciFrameDescription(0xCE, 0x01, cmdParams)

    def InitSocketBindRequest(self):
        cmdParams = []
        SocketIndex = FsciParameter("SocketIndex", 1)
        cmdParams.append(SocketIndex)
        LocalIpAddress = FsciParameter("LocalIpAddress", 16)
        cmdParams.append(LocalIpAddress)
        LocalPort = FsciParameter("LocalPort", 2)
        cmdParams.append(LocalPort)
        SocketDomain = FsciParameter("SocketDomain", 1)
        cmdParams.append(SocketDomain)
        return FsciFrameDescription(0xCE, 0x02, cmdParams)

    def InitSocketSendRequest(self):
        cmdParams = []
        SocketIndex = FsciParameter("SocketIndex", 1)
        cmdParams.append(SocketIndex)
        Flags = FsciParameter("Flags", 1)
        cmdParams.append(Flags)
        Size = FsciParameter("Size", 2)
        cmdParams.append(Size)
        Data = FsciParameter("Data", 1, Size)
        cmdParams.append(Data)
        return FsciFrameDescription(0xCE, 0x03, cmdParams)

    def InitSocketSendToRequest(self):
        cmdParams = []
        SocketIndex = FsciParameter("SocketIndex", 1)
        cmdParams.append(SocketIndex)
        Flags = FsciParameter("Flags", 1)
        cmdParams.append(Flags)
        SecuredMACData = FsciParameter("SecuredMACData", 1)
        cmdParams.append(SecuredMACData)
        Size = FsciParameter("Size", 2)
        cmdParams.append(Size)
        RemotePort = FsciParameter("RemotePort", 2)
        cmdParams.append(RemotePort)
        RemoteIpAddress = FsciParameter("RemoteIpAddress", 16)
        cmdParams.append(RemoteIpAddress)
        Data = FsciParameter("Data", 1, Size)
        cmdParams.append(Data)
        return FsciFrameDescription(0xCE, 0x04, cmdParams)

    def InitSocketReceiveRequest(self):
        cmdParams = []
        SocketIndex = FsciParameter("SocketIndex", 1)
        cmdParams.append(SocketIndex)
        DataSize = FsciParameter("DataSize", 2)
        cmdParams.append(DataSize)
        Flags = FsciParameter("Flags", 1)
        cmdParams.append(Flags)
        return FsciFrameDescription(0xCE, 0x05, cmdParams)

    def InitSocketReceiveFromRequest(self):
        cmdParams = []
        SocketIndex = FsciParameter("SocketIndex", 1)
        cmdParams.append(SocketIndex)
        DataSize = FsciParameter("DataSize", 2)
        cmdParams.append(DataSize)
        Flags = FsciParameter("Flags", 1)
        cmdParams.append(Flags)
        return FsciFrameDescription(0xCE, 0x06, cmdParams)

    def InitSocketConnectRequest(self):
        cmdParams = []
        SocketIndex = FsciParameter("SocketIndex", 1)
        cmdParams.append(SocketIndex)
        RemoteIpAddress = FsciParameter("RemoteIpAddress", 16)
        cmdParams.append(RemoteIpAddress)
        RemotePort = FsciParameter("RemotePort", 2)
        cmdParams.append(RemotePort)
        SocketDomain = FsciParameter("SocketDomain", 1)
        cmdParams.append(SocketDomain)
        return FsciFrameDescription(0xCE, 0x07, cmdParams)

    def InitSocketListenRequest(self):
        cmdParams = []
        SocketIndex = FsciParameter("SocketIndex", 1)
        cmdParams.append(SocketIndex)
        Backlog = FsciParameter("Backlog", 1)
        cmdParams.append(Backlog)
        return FsciFrameDescription(0xCE, 0x08, cmdParams)

    def InitSocketAcceptRequest(self):
        cmdParams = []
        SocketIndex = FsciParameter("SocketIndex", 1)
        cmdParams.append(SocketIndex)
        return FsciFrameDescription(0xCE, 0x09, cmdParams)

    def InitSocketSetOptionRequest(self):
        cmdParams = []
        SocketIndex = FsciParameter("SocketIndex", 1)
        cmdParams.append(SocketIndex)
        SocketLevel = FsciParameter("SocketLevel", 1)
        cmdParams.append(SocketLevel)
        SocketOption = FsciParameter("SocketOption", 2)
        cmdParams.append(SocketOption)
        SocketOptionValue = FsciParameter("SocketOptionValue", 4)
        cmdParams.append(SocketOptionValue)
        return FsciFrameDescription(0xCE, 0x0A, cmdParams)

    def InitSocketGetOptionRequest(self):
        cmdParams = []
        SocketIndex = FsciParameter("SocketIndex", 1)
        cmdParams.append(SocketIndex)
        SocketLevel = FsciParameter("SocketLevel", 1)
        cmdParams.append(SocketLevel)
        SocketOption = FsciParameter("SocketOption", 2)
        cmdParams.append(SocketOption)
        return FsciFrameDescription(0xCE, 0x0B, cmdParams)

    def InitMESHCOP_StartCommissionerRequest(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        return FsciFrameDescription(0xCE, 0x40, cmdParams)

    def InitMESHCOP_StartNativeCommissionerRequest(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        return FsciFrameDescription(0xCE, 0x4F, cmdParams)

    def InitMESHCOP_StopCommissionerRequest(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        return FsciFrameDescription(0xCE, 0x41, cmdParams)

    def InitMESHCOP_AddExpectedJoinerRequest(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        Selected = FsciParameter("Selected", 1)
        cmdParams.append(Selected)
        EuiType = FsciParameter("EuiType", 1)
        cmdParams.append(EuiType)
        EUIdict = {}
        currentList = []
        EUIShortEUI = FsciParameter("EUIShortEUI", 4)
        currentList.append(EUIShortEUI)
        EUIdict[0x00] = currentList
        currentList = []
        EUILongEUI = FsciParameter("EUILongEUI", 8)
        currentList.append(EUILongEUI)
        EUIdict[0x01] = currentList
        EUI = FsciParameter("EUI", 1, EuiType, EUIdict)
        cmdParams.append(EUI)
        PSKdSize = FsciParameter("PSKdSize", 1)
        cmdParams.append(PSKdSize)
        PSKd = FsciParameter("PSKd", 1, PSKdSize)
        cmdParams.append(PSKd)
        return FsciFrameDescription(0xCE, 0x42, cmdParams)

    def InitMESHCOP_GetExpectedJoinerRequest(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        EuiType = FsciParameter("EuiType", 1)
        cmdParams.append(EuiType)
        EUIdict = {}
        currentList = []
        EUIShortEUI = FsciParameter("EUIShortEUI", 4)
        currentList.append(EUIShortEUI)
        EUIdict[0x00] = currentList
        currentList = []
        EUILongEUI = FsciParameter("EUILongEUI", 8)
        currentList.append(EUILongEUI)
        EUIdict[0x01] = currentList
        EUI = FsciParameter("EUI", 1, EuiType, EUIdict)
        cmdParams.append(EUI)
        return FsciFrameDescription(0xCE, 0x43, cmdParams)

    def InitMESHCOP_RemoveExpectedJoinerRequest(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        EuiType = FsciParameter("EuiType", 1)
        cmdParams.append(EuiType)
        EUIdict = {}
        currentList = []
        EUIShortEUI = FsciParameter("EUIShortEUI", 4)
        currentList.append(EUIShortEUI)
        EUIdict[0x00] = currentList
        currentList = []
        EUILongEUI = FsciParameter("EUILongEUI", 8)
        currentList.append(EUILongEUI)
        EUIdict[0x01] = currentList
        EUI = FsciParameter("EUI", 1, EuiType, EUIdict)
        cmdParams.append(EUI)
        return FsciFrameDescription(0xCE, 0x44, cmdParams)

    def InitMESHCOP_RemoveAllExpectedJoinersRequest(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        return FsciFrameDescription(0xCE, 0x45, cmdParams)

    def InitMESHCOP_SyncSteeringDataRequest(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        EuiMask = FsciParameter("EuiMask", 1)
        cmdParams.append(EuiMask)
        return FsciFrameDescription(0xCE, 0x46, cmdParams)

    def InitMESHCOP_MgmtSetRequest(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        PSKcEnable = FsciParameter("PSKcEnable", 1)
        cmdParams.append(PSKcEnable)
        PSKcSize = FsciParameter("PSKcSize", 1, PSKcEnable)
        cmdParams.append(PSKcSize)
        PSKc = FsciParameter("PSKc", 1, PSKcSize)
        cmdParams.append(PSKc)
        NetworkNameEnable = FsciParameter("NetworkNameEnable", 1)
        cmdParams.append(NetworkNameEnable)
        NetworkNameSize = FsciParameter("NetworkNameSize", 1, NetworkNameEnable)
        cmdParams.append(NetworkNameSize)
        NetworkName = FsciParameter("NetworkName", 1, NetworkNameSize)
        cmdParams.append(NetworkName)
        PolicyEnable = FsciParameter("PolicyEnable", 1)
        cmdParams.append(PolicyEnable)
        Policy = FsciParameter("Policy", 1, PolicyEnable)
        cmdParams.append(Policy)
        KeyRotationInterval = FsciParameter("KeyRotationInterval", 2, PolicyEnable)
        cmdParams.append(KeyRotationInterval)
        TimestampEnable = FsciParameter("TimestampEnable", 1)
        cmdParams.append(TimestampEnable)
        Seconds = FsciParameter("Seconds", 6, TimestampEnable)
        cmdParams.append(Seconds)
        Ticks = FsciParameter("Ticks", 2, TimestampEnable)
        cmdParams.append(Ticks)
        return FsciFrameDescription(0xCE, 0x4B, cmdParams)

    def InitMESHCOP_MgmtGetRequest(self):
        cmdParams = []
        # not generated, pickle() is used instead; see frames.py
        return FsciFrameDescription(0xCE, 0x4C, cmdParams)

    def InitMESHCOP_SetCommissionerCredentialRequest(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        PSKcSize = FsciParameter("PSKcSize", 1)
        cmdParams.append(PSKcSize)
        PSKc = FsciParameter("PSKc", 1, PSKcSize)
        cmdParams.append(PSKc)
        XpanId = FsciParameter("XpanId", 8)
        cmdParams.append(XpanId)
        NetworkNameSize = FsciParameter("NetworkNameSize", 1)
        cmdParams.append(NetworkNameSize)
        NetworkName = FsciParameter("NetworkName", 1, NetworkNameSize)
        cmdParams.append(NetworkName)
        return FsciFrameDescription(0xCE, 0x4D, cmdParams)

    def InitMESHCOP_MgmNwkFormRequest(self):
        cmdParams = []
        IPAddress = FsciParameter("IPAddress", 16)
        cmdParams.append(IPAddress)
        NwkNameSize = FsciParameter("NwkNameSize", 1)
        cmdParams.append(NwkNameSize)
        NwkName = FsciParameter("NwkName", 1, NwkNameSize)
        cmdParams.append(NwkName)
        NetworkMasterKey = FsciParameter("NetworkMasterKey", 16)
        cmdParams.append(NetworkMasterKey)
        PSKcSize = FsciParameter("PSKcSize", 1)
        cmdParams.append(PSKcSize)
        PSKc = FsciParameter("PSKc", 1, PSKcSize)
        cmdParams.append(PSKc)
        Channel = FsciParameter("Channel", 1)
        cmdParams.append(Channel)
        return FsciFrameDescription(0xCE, 0x47, cmdParams)

    def InitMESHCOP_MgmtCommissionerGetRequest(self):
        cmdParams = []
        # not generated, pickle() is used instead; see frames.py
        return FsciFrameDescription(0xCE, 0xA0, cmdParams)

    def InitMESHCOP_MgmtActiveGetRequest(self):
        cmdParams = []
        # not generated, pickle() is used instead; see frames.py
        return FsciFrameDescription(0xCE, 0xA2, cmdParams)

    def InitMESHCOP_MgmtPendingGetRequest(self):
        cmdParams = []
        # not generated, pickle() is used instead; see frames.py
        return FsciFrameDescription(0xCE, 0xA4, cmdParams)

    def InitMESHCOP_MgmtCommissionerSetRequest(self):
        cmdParams = []
        # not generated, pickle() is used instead; see frames.py
        return FsciFrameDescription(0xCE, 0xA1, cmdParams)

    def InitMESHCOP_MgmtActiveSetRequest(self):
        cmdParams = []
        # not generated, pickle() is used instead; see frames.py
        return FsciFrameDescription(0xCE, 0xA3, cmdParams)

    def InitMESHCOP_MgmtPendingSetRequest(self):
        cmdParams = []
        # not generated, pickle() is used instead; see frames.py
        return FsciFrameDescription(0xCE, 0xA5, cmdParams)

    def InitMESHCOP_MgmtSendPanIdQueryRequest(self):
        cmdParams = []
        ThrInstanceID = FsciParameter("ThrInstanceID", 1)
        cmdParams.append(ThrInstanceID)
        ScanChannelMask = FsciParameter("ScanChannelMask", 4)
        cmdParams.append(ScanChannelMask)
        PanId = FsciParameter("PanId", 2)
        cmdParams.append(PanId)
        IPAddress = FsciParameter("IPAddress", 16)
        cmdParams.append(IPAddress)
        return FsciFrameDescription(0xCE, 0xA8, cmdParams)

    def InitMESHCOP_MgmtSendEdScanRequest(self):
        cmdParams = []
        ThrInstanceID = FsciParameter("ThrInstanceID", 1)
        cmdParams.append(ThrInstanceID)
        ScanChannelMask = FsciParameter("ScanChannelMask", 4)
        cmdParams.append(ScanChannelMask)
        Count = FsciParameter("Count", 1)
        cmdParams.append(Count)
        Period = FsciParameter("Period", 2)
        cmdParams.append(Period)
        ScanDuration = FsciParameter("ScanDuration", 2)
        cmdParams.append(ScanDuration)
        IPAddress = FsciParameter("IPAddress", 16)
        cmdParams.append(IPAddress)
        return FsciFrameDescription(0xCE, 0xAA, cmdParams)

    def InitMESHCOP_MgmtSendAnnounceBeginRequest(self):
        cmdParams = []
        ThrInstanceID = FsciParameter("ThrInstanceID", 1)
        cmdParams.append(ThrInstanceID)
        CommissionerSessionID = FsciParameter("CommissionerSessionID", 2)
        cmdParams.append(CommissionerSessionID)
        ChannelMask = FsciParameter("ChannelMask", 4)
        cmdParams.append(ChannelMask)
        Count = FsciParameter("Count", 1)
        cmdParams.append(Count)
        Period = FsciParameter("Period", 2)
        cmdParams.append(Period)
        IPAddress = FsciParameter("IPAddress", 16)
        cmdParams.append(IPAddress)
        return FsciFrameDescription(0xCE, 0xA7, cmdParams)

    def InitNWKU_IfconfigBindRequest(self):
        cmdParams = []
        IpAddress = FsciParameter("IpAddress", 16)
        cmdParams.append(IpAddress)
        InterfaceId = FsciParameter("InterfaceId", 1)
        cmdParams.append(InterfaceId)
        return FsciFrameDescription(0xCE, 0x0C, cmdParams)

    def InitNWKU_IfconfigAllRequest(self):
        cmdParams = []
        return FsciFrameDescription(0xCE, 0x0D, cmdParams)

    def InitNWKU_PingRequest(self):
        cmdParams = []
        DestinationIpAddress = FsciParameter("DestinationIpAddress", 16)
        cmdParams.append(DestinationIpAddress)
        SourceIpAddress = FsciParameter("SourceIpAddress", 16)
        cmdParams.append(SourceIpAddress)
        Payloadlength = FsciParameter("Payloadlength", 2)
        cmdParams.append(Payloadlength)
        Timeout = FsciParameter("Timeout", 2)
        cmdParams.append(Timeout)
        Secured = FsciParameter("Secured", 1)
        cmdParams.append(Secured)
        return FsciFrameDescription(0xCE, 0x0E, cmdParams)

    def InitNWKU_EchoUDPRequest(self):
        cmdParams = []
        DestinationIpAddress = FsciParameter("DestinationIpAddress", 16)
        cmdParams.append(DestinationIpAddress)
        SourceIpAddress = FsciParameter("SourceIpAddress", 16)
        cmdParams.append(SourceIpAddress)
        Payloadlength = FsciParameter("Payloadlength", 2)
        cmdParams.append(Payloadlength)
        Timeout = FsciParameter("Timeout", 2)
        cmdParams.append(Timeout)
        Iterations = FsciParameter("Iterations", 1)
        cmdParams.append(Iterations)
        return FsciFrameDescription(0xCE, 0x70, cmdParams)

    def InitNWKU_CPUResetRequest(self):
        cmdParams = []
        return FsciFrameDescription(0xCE, 0x60, cmdParams)

    def InitNWKU_CoapCreateInstanceRequest(self):
        cmdParams = []
        UDPPort = FsciParameter("UDPPort", 2)
        cmdParams.append(UDPPort)
        SocketDomain = FsciParameter("SocketDomain", 1)
        cmdParams.append(SocketDomain)
        return FsciFrameDescription(0xCE, 0x93, cmdParams)

    def InitNWKU_CoapSendRequest(self):
        cmdParams = []
        InstanceID = FsciParameter("InstanceID", 1)
        cmdParams.append(InstanceID)
        DestinationIpAddress = FsciParameter("DestinationIpAddress", 16)
        cmdParams.append(DestinationIpAddress)
        UDPPort = FsciParameter("UDPPort", 2)
        cmdParams.append(UDPPort)
        RequestType = FsciParameter("RequestType", 1)
        cmdParams.append(RequestType)
        MessageType = FsciParameter("MessageType", 1)
        cmdParams.append(MessageType)
        URIpath = FsciParameter("URIpath", 30)
        cmdParams.append(URIpath)
        PayloadLength = FsciParameter("PayloadLength", 1)
        cmdParams.append(PayloadLength)
        Payload = FsciParameter("Payload", 1, PayloadLength)
        cmdParams.append(Payload)
        return FsciFrameDescription(0xCE, 0x90, cmdParams)

    def InitNWKU_CoapRegisterRequest(self):
        cmdParams = []
        InstanceID = FsciParameter("InstanceID", 1)
        cmdParams.append(InstanceID)
        SocketDomain = FsciParameter("SocketDomain", 1)
        cmdParams.append(SocketDomain)
        URIpath = FsciParameter("URIpath", 30)
        cmdParams.append(URIpath)
        Port = FsciParameter("Port", 2)
        cmdParams.append(Port)
        return FsciFrameDescription(0xCE, 0x91, cmdParams)

    def InitNWKU_DnsSendRequestRequest(self):
        cmdParams = []
        DomainName = FsciParameter("DomainName", 30)
        cmdParams.append(DomainName)
        return FsciFrameDescription(0xCE, 0x95, cmdParams)

    def InitNWKU_McastGroupShowRequest(self):
        cmdParams = []
        InterfaceId = FsciParameter("InterfaceId", 1)
        cmdParams.append(InterfaceId)
        return FsciFrameDescription(0xCE, 0x72, cmdParams)

    def InitNWKU_McastGroupManageRequest(self):
        cmdParams = []
        InterfaceId = FsciParameter("InterfaceId", 1)
        cmdParams.append(InterfaceId)
        Action = FsciParameter("Action", 1)
        cmdParams.append(Action)
        McastAddress = FsciParameter("McastAddress", 16)
        cmdParams.append(McastAddress)
        return FsciFrameDescription(0xCE, 0x71, cmdParams)

    def InitNWKU_RoutesShowRequest(self):
        cmdParams = []
        return FsciFrameDescription(0xCE, 0x74, cmdParams)

    def InitNWKU_RoutesManageRequest(self):
        cmdParams = []
        Action = FsciParameter("Action", 1)
        cmdParams.append(Action)
        Destination = FsciParameter("Destination", 16)
        cmdParams.append(Destination)
        NextHop = FsciParameter("NextHop", 16)
        cmdParams.append(NextHop)
        RouteInfodict = {}
        currentList = []
        AddPrefixLength = FsciParameter("AddPrefixLength", 1)
        currentList.append(AddPrefixLength)
        AddRouteMetric = FsciParameter("AddRouteMetric", 1)
        currentList.append(AddRouteMetric)
        AddInstanceId = FsciParameter("AddInstanceId", 1)
        currentList.append(AddInstanceId)
        RouteInfodict[0x01] = currentList
        RouteInfo = FsciParameter("RouteInfo", 1, Action, RouteInfodict)
        cmdParams.append(RouteInfo)
        return FsciFrameDescription(0xCE, 0x73, cmdParams)

    def InitDTLSOpenRequest(self):
        cmdParams = []
        Maximumretransmissionscount = FsciParameter("Maximumretransmissionscount", 1)
        cmdParams.append(Maximumretransmissionscount)
        Timeout = FsciParameter("Timeout", 2)
        cmdParams.append(Timeout)
        LocalPort = FsciParameter("LocalPort", 2)
        cmdParams.append(LocalPort)
        return FsciFrameDescription(0xCE, 0x30, cmdParams)

    def InitDTLSCloseRequest(self):
        cmdParams = []
        ContextNumber = FsciParameter("ContextNumber", 1)
        cmdParams.append(ContextNumber)
        return FsciFrameDescription(0xCE, 0x31, cmdParams)

    def InitDTLSClosePeerRequest(self):
        cmdParams = []
        PeerNumber = FsciParameter("PeerNumber", 1)
        cmdParams.append(PeerNumber)
        return FsciFrameDescription(0xCE, 0x32, cmdParams)

    def InitDTLSConnectRequest(self):
        cmdParams = []
        ContextNumber = FsciParameter("ContextNumber", 1)
        cmdParams.append(ContextNumber)
        ServerIPAddress = FsciParameter("ServerIPAddress", 16)
        cmdParams.append(ServerIPAddress)
        ServerPort = FsciParameter("ServerPort", 2)
        cmdParams.append(ServerPort)
        return FsciFrameDescription(0xCE, 0x33, cmdParams)

    def InitDTLSSendRequest(self):
        cmdParams = []
        PeerNumber = FsciParameter("PeerNumber", 1)
        cmdParams.append(PeerNumber)
        Size = FsciParameter("Size", 2)
        cmdParams.append(Size)
        Data = FsciParameter("Data", 1, Size)
        cmdParams.append(Data)
        return FsciFrameDescription(0xCE, 0x35, cmdParams)

    def InitFSCIGetUniqueIdRequest(self):
        cmdParams = []
        return FsciFrameDescription(0xA3, 0xB0, cmdParams)

    def InitFSCIGetMcuIdRequest(self):
        cmdParams = []
        return FsciFrameDescription(0xA3, 0xB1, cmdParams)

    def InitFSCIGetSwVersionsRequest(self):
        cmdParams = []
        return FsciFrameDescription(0xA3, 0xB2, cmdParams)

    def InitSerialTun_LinkIndicationRequest(self):
        cmdParams = []
        State = FsciParameter("State", 1)
        cmdParams.append(State)
        return FsciFrameDescription(0xCE, 0xF1, cmdParams)

    def InitSerialTun_IPPacketSendRequest(self):
        cmdParams = []
        Size = FsciParameter("Size", 2)
        cmdParams.append(Size)
        Data = FsciParameter("Data", 1, Size)
        cmdParams.append(Data)
        return FsciFrameDescription(0xCE, 0xF2, cmdParams)

    def InitSniffer_MacSetPIBAttributeRequest(self):
        cmdParams = []
        PIBAttribute = FsciParameter("PIBAttribute", 1)
        cmdParams.append(PIBAttribute)
        Value = FsciParameter("Value", 7)
        cmdParams.append(Value)
        return FsciFrameDescription(0x85, 0x09, cmdParams)

    def InitSniffer_DetectRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x95, 0x20, cmdParams)

    def InitFSCIACK(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xA4, 0xFD, cmdParams)

    def InitFSCIEnterBootloaderRequest(self):
        cmdParams = []
        return FsciFrameDescription(0xA3, 0xCF, cmdParams)

    def InitAspSetPowerLevelRequest(self):
        cmdParams = []
        powerLevel = FsciParameter("powerLevel", 1)
        cmdParams.append(powerLevel)
        return FsciFrameDescription(0x95, 0x0F, cmdParams)

    def InitAspGetPowerLevelRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x95, 0x1F, cmdParams)

    def InitSocketCreateConfirm(self):
        cmdParams = []
        SocketIndex = FsciParameter("SocketIndex", 1)
        cmdParams.append(SocketIndex)
        return FsciFrameDescription(0xCF, 0x00, cmdParams)

    def InitSocketShutdownConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x01, cmdParams)

    def InitSocketBindConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x02, cmdParams)

    def InitSocketSendConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x03, cmdParams)

    def InitSocketSendToConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x04, cmdParams)

    def InitSocketReceiveConfirm(self):
        cmdParams = []
        Size = FsciParameter("Size", 2)
        cmdParams.append(Size)
        Data = FsciParameter("Data", 1, Size)
        cmdParams.append(Data)
        return FsciFrameDescription(0xCF, 0x05, cmdParams)

    def InitSocketReceiveFromConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        RemoteIpAddress = FsciParameter("RemoteIpAddress", 16)
        cmdParams.append(RemoteIpAddress)
        RemotePort = FsciParameter("RemotePort", 2)
        cmdParams.append(RemotePort)
        Size = FsciParameter("Size", 2)
        cmdParams.append(Size)
        Data = FsciParameter("Data", 1, Size)
        cmdParams.append(Data)
        return FsciFrameDescription(0xCF, 0x06, cmdParams)

    def InitSocketConnectConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x07, cmdParams)

    def InitSocketListenConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x08, cmdParams)

    def InitSocketAcceptConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        ConnectedSocketIndex = FsciParameter("ConnectedSocketIndex", 1)
        cmdParams.append(ConnectedSocketIndex)
        return FsciFrameDescription(0xCF, 0x09, cmdParams)

    def InitSocketSetOptionConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x0A, cmdParams)

    def InitSocketGetOptionConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        OptionValue = FsciParameter("OptionValue", 4)
        cmdParams.append(OptionValue)
        return FsciFrameDescription(0xCF, 0x0B, cmdParams)

    def InitMAC_MacFilteringAddEntryConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x12, cmdParams)

    def InitMAC_MacFilteringRemoveEntryConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x13, cmdParams)

    def InitMAC_MacFilteringEnableConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x14, cmdParams)

    def InitMAC_MacFilteringGetTableConfirm(self):
        cmdParams = []
        # not generated, cursor based approach in observer; see events.py
        return FsciFrameDescription(0xCF, 0x15, cmdParams)

    def InitTHR_SetDeviceConfigConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x16, cmdParams)

    def InitTHR_NwkScanConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x1A, cmdParams)

    def InitTHR_CreateNwkConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x1B, cmdParams)

    def InitTHR_JoinConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x1C, cmdParams)

    def InitTHR_FactoryResetConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x1F, cmdParams)

    def InitTHR_CpuResetConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x21, cmdParams)

    def InitTHR_CpuResetIndication(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        ResetCpuPayloaddict = {}
        currentList = []
        ResetCpuSuccessBoardNameLen = FsciParameter("ResetCpuSuccessBoardNameLen", 1)
        currentList.append(ResetCpuSuccessBoardNameLen)
        ResetCpuSuccessBoardName = FsciParameter(
            "ResetCpuSuccessBoardName", 1, ResetCpuSuccessBoardNameLen)
        currentList.append(ResetCpuSuccessBoardName)
        ResetCpuSuccessUniqueMcuId = FsciParameter("ResetCpuSuccessUniqueMcuId", 16)
        currentList.append(ResetCpuSuccessUniqueMcuId)
        ResetCpuSuccessStackVersion = FsciParameter("ResetCpuSuccessStackVersion", 6)
        currentList.append(ResetCpuSuccessStackVersion)
        ResetCpuSuccessSwVersionLen = FsciParameter("ResetCpuSuccessSwVersionLen", 1)
        currentList.append(ResetCpuSuccessSwVersionLen)
        ResetCpuSuccessSwVersion = FsciParameter(
            "ResetCpuSuccessSwVersion", 1, ResetCpuSuccessSwVersionLen)
        currentList.append(ResetCpuSuccessSwVersion)
        ResetCpuPayloaddict[0x00] = currentList
        currentList = []
        ResetCpuPendingTimeoutMs = FsciParameter("ResetCpuPendingTimeoutMs", 4)
        currentList.append(ResetCpuPendingTimeoutMs)
        ResetCpuPayloaddict[0x01] = currentList
        ResetCpuPayload = FsciParameter("ResetCpuPayload", 1, Status, ResetCpuPayloaddict)
        cmdParams.append(ResetCpuPayload)
        return FsciFrameDescription(0xCF, 0x22, cmdParams)

    def InitTHR_DisconnectConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x1D, cmdParams)

    def InitTHR_AttachConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x80, cmdParams)

    def InitTHR_PromoteAsRouterConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x94, cmdParams)

    def InitTHR_EventNwkScanConfirm(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        EventStatus = FsciParameter("EventStatus", 2)
        cmdParams.append(EventStatus)
        DataSize = FsciParameter("DataSize", 2)
        cmdParams.append(DataSize)
        ScanChannelMask = FsciParameter("ScanChannelMask", 4)
        cmdParams.append(ScanChannelMask)
        ScanType = FsciParameter("ScanType", 1)
        cmdParams.append(ScanType)
        ScanDuration = FsciParameter("ScanDuration", 1)
        cmdParams.append(ScanDuration)
        maxThrNwkToDiscover = FsciParameter("maxThrNwkToDiscover", 2)
        cmdParams.append(maxThrNwkToDiscover)
        Datadict = {}
        currentList = []
        EnergyDetectEnergyDetectEntries = FsciParameter("EnergyDetectEnergyDetectEntries", 1)
        currentList.append(EnergyDetectEnergyDetectEntries)
        EnergyDetectEnergyDetectList = FsciParameter(
            "EnergyDetectEnergyDetectList", 1, EnergyDetectEnergyDetectEntries)
        currentList.append(EnergyDetectEnergyDetectList)
        Datadict[0x01] = currentList
        currentList = []
        ActiveScanNwkDiscoveryEntries = FsciParameter("ActiveScanNwkDiscoveryEntries", 1)
        currentList.append(ActiveScanNwkDiscoveryEntries)
        NwkDiscoveryListNumOfRcvdBeacons = FsciParameter("NwkDiscoveryListNumOfRcvdBeacons", 2)
        currentList.append(NwkDiscoveryListNumOfRcvdBeacons)
        NwkDiscoveryListPanId = FsciParameter("NwkDiscoveryListPanId", 2)
        currentList.append(NwkDiscoveryListPanId)
        NwkDiscoveryListChannel = FsciParameter("NwkDiscoveryListChannel", 1)
        currentList.append(NwkDiscoveryListChannel)
        NwkDiscoveryListReserved = FsciParameter("NwkDiscoveryListReserved", 1)
        currentList.append(NwkDiscoveryListReserved)
        Datadict[0x02] = currentList
        currentList = []
        EnergyDetectAndActiveScanEnergyDetectEntries = FsciParameter(
            "EnergyDetectAndActiveScanEnergyDetectEntries", 1)
        currentList.append(EnergyDetectAndActiveScanEnergyDetectEntries)
        EnergyDetectAndActiveScanEnergyDetectList = FsciParameter(
            "EnergyDetectAndActiveScanEnergyDetectList", 1, EnergyDetectEnergyDetectEntries)
        currentList.append(EnergyDetectAndActiveScanEnergyDetectList)
        EnergyDetectAndActiveScanNwkDiscoveryEntries = FsciParameter(
            "EnergyDetectAndActiveScanNwkDiscoveryEntries", 1)
        currentList.append(EnergyDetectAndActiveScanNwkDiscoveryEntries)
        NwkDiscoveryListNumOfRcvdBeacons = FsciParameter("NwkDiscoveryListNumOfRcvdBeacons", 2)
        currentList.append(NwkDiscoveryListNumOfRcvdBeacons)
        NwkDiscoveryListPanId = FsciParameter("NwkDiscoveryListPanId", 2)
        currentList.append(NwkDiscoveryListPanId)
        NwkDiscoveryListChannel = FsciParameter("NwkDiscoveryListChannel", 1)
        currentList.append(NwkDiscoveryListChannel)
        NwkDiscoveryListReserved = FsciParameter("NwkDiscoveryListReserved", 1)
        currentList.append(NwkDiscoveryListReserved)
        Datadict[0x03] = currentList
        Data = FsciParameter("Data", 1, ScanType, Datadict)
        cmdParams.append(Data)
        return FsciFrameDescription(0xCF, 0x50, cmdParams)

    def InitTHR_EventNwkCreateConfirm(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        EventStatus = FsciParameter("EventStatus", 2)
        cmdParams.append(EventStatus)
        DataSize = FsciParameter("DataSize", 2)
        cmdParams.append(DataSize)
        Data = FsciParameter("Data", 1, DataSize)
        cmdParams.append(Data)
        return FsciFrameDescription(0xCF, 0x51, cmdParams)

    def InitTHR_EventNwkJoinConfirm(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        EventStatus = FsciParameter("EventStatus", 2)
        cmdParams.append(EventStatus)
        DataSize = FsciParameter("DataSize", 2)
        cmdParams.append(DataSize)
        Data = FsciParameter("Data", 1, DataSize)
        cmdParams.append(Data)
        return FsciFrameDescription(0xCF, 0x52, cmdParams)

    def InitTHR_EventNwkJoinSelectParentsConfirm(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        EventStatus = FsciParameter("EventStatus", 2)
        cmdParams.append(EventStatus)
        DataSize = FsciParameter("DataSize", 2)
        cmdParams.append(DataSize)
        Data = FsciParameter("Data", 1, DataSize)
        cmdParams.append(Data)
        return FsciFrameDescription(0xCF, 0x53, cmdParams)

    def InitTHR_EventGeneralConfirm(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        EventStatus = FsciParameter("EventStatus", 2)
        cmdParams.append(EventStatus)
        DataSize = FsciParameter("DataSize", 2)
        cmdParams.append(DataSize)
        Data = FsciParameter("Data", 1, DataSize)
        cmdParams.append(Data)
        return FsciFrameDescription(0xCF, 0x54, cmdParams)

    def InitTHR_EventNwkCommissioningIndication(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        EventStatus = FsciParameter("EventStatus", 2)
        cmdParams.append(EventStatus)
        return FsciFrameDescription(0xCF, 0x55, cmdParams)

    def InitTHR_CommissioningDiagnosticIndication(self):
        cmdParams = []
        Direction = FsciParameter("Direction", 1)
        cmdParams.append(Direction)
        Type = FsciParameter("Type", 1)
        cmdParams.append(Type)
        EUI = FsciParameter("EUI", 8)
        cmdParams.append(EUI)
        TlvsLength = FsciParameter("TlvsLength", 1)
        cmdParams.append(TlvsLength)
        TlvsBytes = FsciParameter("TlvsBytes", 1, TlvsLength)
        cmdParams.append(TlvsBytes)
        return FsciFrameDescription(0xCF, 0x4E, cmdParams)

    def InitTHR_MgmtDiagnosticGetConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        CoapMsgId = FsciParameter("CoapMsgId", 2)
        cmdParams.append(CoapMsgId)
        return FsciFrameDescription(0xCF, 0x61, cmdParams)

    def InitTHR_DiagTestGetConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        CoapMsgId = FsciParameter("CoapMsgId", 2)
        cmdParams.append(CoapMsgId)
        return FsciFrameDescription(0xCF, 0x67, cmdParams)

    def InitTHR_MgmtDiagnosticResetConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        CoapMsgId = FsciParameter("CoapMsgId", 2)
        cmdParams.append(CoapMsgId)
        return FsciFrameDescription(0xCF, 0x62, cmdParams)

    def InitTHR_MgmtReadMemoryConfirm(self):
        cmdParams = []
        Memory = FsciParameter("Memory", 1)
        cmdParams.append(Memory)
        return FsciFrameDescription(0xCF, 0x65, cmdParams)

    def InitTHR_MgmtWriteMemoryConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x66, cmdParams)

    def InitTHR_SetManualSlaacIIDConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x75, cmdParams)

    def InitTHR_SendProactiveAddrNotifConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x76, cmdParams)

    def InitTHR_NwkDiscoveryConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0xC0, cmdParams)

    def InitTHR_NwkDiscoveryStopConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0xC1, cmdParams)

    def InitTHR_SearchNwkWithAnounceConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0xC2, cmdParams)

    def InitTHR_MgmtDiagnosticGetRspIndication(self):
        cmdParams = []
        # not generated, cursor based approach in observer; see events.py
        return FsciFrameDescription(0xCF, 0x63, cmdParams)

    def InitTHR_DiagTestGetRspIndication(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        CoapMsgId = FsciParameter("CoapMsgId", 2)
        cmdParams.append(CoapMsgId)
        DataLen = FsciParameter("DataLen", 2)
        cmdParams.append(DataLen)
        Payload_TlvId = FsciParameter("Payload_TlvId", 1)
        cmdParams.append(Payload_TlvId)
        Payload_TLVPayloaddict = {}
        currentList = []
        Payload_TLVPayloadColdFactoryReset = FsciParameter("Payload_TLVPayloadColdFactoryReset", 2)
        currentList.append(Payload_TLVPayloadColdFactoryReset)
        Payload_TLVPayloaddict[0xB0] = currentList
        currentList = []
        Payload_TLVPayloadWarmCPUReset = FsciParameter("Payload_TLVPayloadWarmCPUReset", 2)
        currentList.append(Payload_TLVPayloadWarmCPUReset)
        Payload_TLVPayloaddict[0xB1] = currentList
        currentList = []
        DataReqLatency = FsciParameter("DataReqLatency", 4)
        currentList.append(DataReqLatency)
        DataRspLatency = FsciParameter("DataRspLatency", 4)
        currentList.append(DataRspLatency)
        DataOffset = FsciParameter("DataOffset", 4)
        currentList.append(DataOffset)
        DataSequenceNumber = FsciParameter("DataSequenceNumber", 1)
        currentList.append(DataSequenceNumber)
        DataPayloadSize = FsciParameter("DataPayloadSize", 1)
        currentList.append(DataPayloadSize)
        DataPayload = FsciParameter("DataPayload", 1, DataPayloadSize)
        currentList.append(DataPayload)
        Payload_TLVPayloaddict[0xB2] = currentList
        currentList = []
        ResultsReqLatency = FsciParameter("ResultsReqLatency", 4)
        currentList.append(ResultsReqLatency)
        ResultsRspLatency = FsciParameter("ResultsRspLatency", 4)
        currentList.append(ResultsRspLatency)
        ResultsOffset = FsciParameter("ResultsOffset", 4)
        currentList.append(ResultsOffset)
        ResultsSequenceNumber = FsciParameter("ResultsSequenceNumber", 1)
        currentList.append(ResultsSequenceNumber)
        Payload_TLVPayloaddict[0xB3] = currentList
        Payload_TLVPayload = FsciParameter(
            "Payload_TLVPayload", 1, Payload_TlvId, Payload_TLVPayloaddict)
        cmdParams.append(Payload_TLVPayload)
        return FsciFrameDescription(0xCF, 0x68, cmdParams)

    def InitTHR_MgmtDiagnosticDiagTestEventIndication(self):
        cmdParams = []
        Data_ReqLatency = FsciParameter("Data_ReqLatency", 4)
        cmdParams.append(Data_ReqLatency)
        Data_SequenceNumber = FsciParameter("Data_SequenceNumber", 1)
        cmdParams.append(Data_SequenceNumber)
        return FsciFrameDescription(0xCF, 0x6B, cmdParams)

    def InitTHR_MgmtDiagnosticResetRspIndication(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        CoapMsgId = FsciParameter("CoapMsgId", 2)
        cmdParams.append(CoapMsgId)
        return FsciFrameDescription(0xCF, 0x64, cmdParams)

    def InitTHR_SetNwkIdTimeoutConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x2E, cmdParams)

    def InitTHR_SetThresholdConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x20, cmdParams)

    def InitTHR_GetNeighborInfoConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        NeighborInfoExtendedAddress = FsciParameter("NeighborInfoExtendedAddress", 8, Status)
        cmdParams.append(NeighborInfoExtendedAddress)
        NeighborInfoShortAddress = FsciParameter("NeighborInfoShortAddress", 2, Status)
        cmdParams.append(NeighborInfoShortAddress)
        NeighborInfoLastCommTime = FsciParameter("NeighborInfoLastCommTime", 4, Status)
        cmdParams.append(NeighborInfoLastCommTime)
        NeighborInfoInRSSI = FsciParameter("NeighborInfoInRSSI", 1, Status)
        cmdParams.append(NeighborInfoInRSSI)
        NeighborInfoTimeoutsec = FsciParameter("NeighborInfoTimeoutsec", 4, Status)
        cmdParams.append(NeighborInfoTimeoutsec)
        return FsciFrameDescription(0xCF, 0x10, cmdParams)

    def InitTHR_GetChildrenTableConfirm(self):
        cmdParams = []
        # not generated, cursor based approach in observer; see events.py
        return FsciFrameDescription(0xCF, 0x23, cmdParams)

    def InitTHR_GetNeighborTableConfirm(self):
        cmdParams = []
        # not generated, cursor based approach in observer; see events.py
        return FsciFrameDescription(0xCF, 0x24, cmdParams)

    def InitTHR_GetRoutingTableConfirm(self):
        cmdParams = []
        # not generated, cursor based approach in observer; see events.py
        return FsciFrameDescription(0xCF, 0x25, cmdParams)

    def InitTHR_GetAttrConfirm(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        AttributeId = FsciParameter("AttributeId", 1)
        cmdParams.append(AttributeId)
        Index = FsciParameter("Index", 1)
        cmdParams.append(Index)
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        AttrSize = FsciParameter("AttrSize", 1)
        cmdParams.append(AttrSize)
        AttributeValue = FsciParameter("AttributeValue", 1, AttrSize)
        cmdParams.append(AttributeValue)
        return FsciFrameDescription(0xCF, 0x17, cmdParams)

    def InitTHR_SetAttrConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x18, cmdParams)

    def InitTHR_GetThreadIpAddrConfirm(self):
        cmdParams = []
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        AddressType = FsciParameter("AddressType", 1)
        cmdParams.append(AddressType)
        NoOfIpAddr = FsciParameter("NoOfIpAddr", 1)
        cmdParams.append(NoOfIpAddr)
        AddressList = FsciParameter("AddressList", 16, NoOfIpAddr)
        cmdParams.append(AddressList)
        return FsciFrameDescription(0xCF, 0x19, cmdParams)

    def InitTHR_GetParentConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        InstanceId = FsciParameter("InstanceId", 1)
        cmdParams.append(InstanceId)
        ShortAddress = FsciParameter("ShortAddress", 2)
        cmdParams.append(ShortAddress)
        ExtendedAddress = FsciParameter("ExtendedAddress", 8)
        cmdParams.append(ExtendedAddress)
        return FsciFrameDescription(0xCF, 0x1E, cmdParams)

    def InitTHR_ChildUpdateToParentConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x2F, cmdParams)

    def InitTHR_LeaderRemoveRouterIdConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x26, cmdParams)

    def InitTHR_GenerateAllKeysConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x28, cmdParams)

    def InitTHR_SwitchKeyKeyConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x27, cmdParams)

    def InitTHR_BrPrefixAddEntryConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x29, cmdParams)

    def InitTHR_BrPrefixGetTableConfirm(self):
        cmdParams = []
        # not generated, cursor based approach in observer; see events.py
        return FsciFrameDescription(0xCF, 0x2A, cmdParams)

    def InitTHR_BrPrefixRemoveEntryConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x2B, cmdParams)

    def InitTHR_BrServiceRemoveEntryConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x86, cmdParams)

    def InitTHR_BrServiceAddEntryConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x85, cmdParams)

    def InitTHR_BrPrefixSyncConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x2C, cmdParams)

    def InitTHR_BrPrefixRemoveAllConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x2D, cmdParams)

    def InitNWKU_EidToRlocMapResponse(self):
        cmdParams = []
        # not generated, cursor based approach in observer; see events.py
        return FsciFrameDescription(0xCF, 0x6A, cmdParams)

    def InitTHR_IdentifyConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x69, cmdParams)

    def InitNWKU_IfconfigBindConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x0C, cmdParams)

    def InitNWKU_IfconfigAllResponse(self):
        cmdParams = []
        # not generated, cursor based approach in observer; see events.py
        return FsciFrameDescription(0xCF, 0x0D, cmdParams)

    def InitNWKU_PingConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        Interval = FsciParameter("Interval", 2)
        cmdParams.append(Interval)
        return FsciFrameDescription(0xCF, 0x0E, cmdParams)

    def InitNWKU_EchoUDPConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x70, cmdParams)

    def InitNWKU_CoapMsgReceivedIndication(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        RemoteIpAddress = FsciParameter("RemoteIpAddress", 16)
        cmdParams.append(RemoteIpAddress)
        UDPPort = FsciParameter("UDPPort", 2)
        cmdParams.append(UDPPort)
        RequestType = FsciParameter("RequestType", 1)
        cmdParams.append(RequestType)
        MessageType = FsciParameter("MessageType", 1)
        cmdParams.append(MessageType)
        URIpath = FsciParameter("URIpath", 30)
        cmdParams.append(URIpath)
        PayloadLength = FsciParameter("PayloadLength", 1)
        cmdParams.append(PayloadLength)
        Payload = FsciParameter("Payload", 1, PayloadLength)
        cmdParams.append(Payload)
        return FsciFrameDescription(0xCF, 0x92, cmdParams)

    def InitNWKU_DnsResponseReceivedIndication(self):
        cmdParams = []
        IpAddress = FsciParameter("IpAddress", 16)
        cmdParams.append(IpAddress)
        DomainName = FsciParameter("DomainName", 30)
        cmdParams.append(DomainName)
        return FsciFrameDescription(0xCF, 0x96, cmdParams)

    def InitNWKU_CoapRegisterConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x91, cmdParams)

    def InitNWKU_CoapCreateInstanceConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x93, cmdParams)

    def InitNWKU_CoapSendConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x90, cmdParams)

    def InitNWKU_DnsSendRequestConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x95, cmdParams)

    def InitNWKU_McastGroupManageConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x71, cmdParams)

    def InitNWKU_McastGroupShowResponse(self):
        cmdParams = []
        CountIpAddresses = FsciParameter("CountIpAddresses", 1)
        cmdParams.append(CountIpAddresses)
        Addresses = FsciParameter("Addresses", 16, CountIpAddresses)
        cmdParams.append(Addresses)
        return FsciFrameDescription(0xCF, 0x72, cmdParams)

    def InitNWKU_RoutesManageConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x73, cmdParams)

    def InitNWKU_RoutesShowResponse(self):
        cmdParams = []
        # not generated, cursor based approach in observer; see events.py
        return FsciFrameDescription(0xCF, 0x74, cmdParams)

    def InitMESHCOP_StartCommissionerConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x40, cmdParams)

    def InitMESHCOP_StartNativeCommissionerConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x4F, cmdParams)

    def InitMESHCOP_StopCommissionerConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x41, cmdParams)

    def InitMESHCOP_AddExpectedJoinerConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x42, cmdParams)

    def InitMESHCOP_GetExpectedJoinerConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        Selected = FsciParameter("Selected", 1)
        cmdParams.append(Selected)
        PSKdSize = FsciParameter("PSKdSize", 1)
        cmdParams.append(PSKdSize)
        PSKd = FsciParameter("PSKd", 1, PSKdSize)
        cmdParams.append(PSKd)
        return FsciFrameDescription(0xCF, 0x43, cmdParams)

    def InitMESHCOP_RemoveExpectedJoinerConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x44, cmdParams)

    def InitMESHCOP_RemoveAllExpectedJoinersConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x45, cmdParams)

    def InitMESHCOP_SyncSteeringDataConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x46, cmdParams)

    def InitMESHCOP_MgmtSetConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x4B, cmdParams)

    def InitMESHCOP_MgmtGetConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        Type = FsciParameter("Type", 1)
        cmdParams.append(Type)
        Length = FsciParameter("Length", 1)
        cmdParams.append(Length)
        Valuedict = {}
        currentList = []
        ValueChannel = FsciParameter("ValueChannel", 1)
        currentList.append(ValueChannel)
        Valuedict[0x00] = currentList
        currentList = []
        ValueChannelMask = FsciParameter("ValueChannelMask", 1, Length)
        currentList.append(ValueChannelMask)
        Valuedict[0x35] = currentList
        currentList = []
        ValuePanId = FsciParameter("ValuePanId", 1, Length)
        currentList.append(ValuePanId)
        Valuedict[0x01] = currentList
        currentList = []
        ValueXpanId = FsciParameter("ValueXpanId", 1, Length)
        currentList.append(ValueXpanId)
        Valuedict[0x02] = currentList
        currentList = []
        ValueNetworkName = FsciParameter("ValueNetworkName", 1, Length)
        currentList.append(ValueNetworkName)
        Valuedict[0x03] = currentList
        currentList = []
        ValuePSKc = FsciParameter("ValuePSKc", 1, Length)
        currentList.append(ValuePSKc)
        Valuedict[0x04] = currentList
        currentList = []
        ValueMasterKey = FsciParameter("ValueMasterKey", 1, Length)
        currentList.append(ValueMasterKey)
        Valuedict[0x05] = currentList
        currentList = []
        ValueKeySequence = FsciParameter("ValueKeySequence", 1, Length)
        currentList.append(ValueKeySequence)
        Valuedict[0x06] = currentList
        currentList = []
        ValueMeshLocalUla = FsciParameter("ValueMeshLocalUla", 1, Length)
        currentList.append(ValueMeshLocalUla)
        Valuedict[0x07] = currentList
        currentList = []
        ValueSteeringData = FsciParameter("ValueSteeringData", 1, Length)
        currentList.append(ValueSteeringData)
        Valuedict[0x08] = currentList
        currentList = []
        ValueBorderRouterLocator = FsciParameter("ValueBorderRouterLocator", 1, Length)
        currentList.append(ValueBorderRouterLocator)
        Valuedict[0x09] = currentList
        currentList = []
        ValueCommissionerID = FsciParameter("ValueCommissionerID", 1, Length)
        currentList.append(ValueCommissionerID)
        Valuedict[0x0A] = currentList
        currentList = []
        ValueCommissionerSessionID = FsciParameter("ValueCommissionerSessionID", 1, Length)
        currentList.append(ValueCommissionerSessionID)
        Valuedict[0x0B] = currentList
        currentList = []
        ValueSecurityPolicy = FsciParameter("ValueSecurityPolicy", 1, Length)
        currentList.append(ValueSecurityPolicy)
        Valuedict[0x0C] = currentList
        currentList = []
        ValueDatasetTimestamp = FsciParameter("ValueDatasetTimestamp", 1, Length)
        currentList.append(ValueDatasetTimestamp)
        Valuedict[0x34] = currentList
        Value = FsciParameter("Value", 1, Type, Valuedict)
        cmdParams.append(Value)
        return FsciFrameDescription(0xCF, 0x4C, cmdParams)

    def InitMESHCOP_SetCommissionerCredentialConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x4D, cmdParams)

    def InitMESHCOP_MgmNwkFormConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x47, cmdParams)

    def InitMESHCOP_MgmtCommissionerGetConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        Length = FsciParameter("Length", 4)
        cmdParams.append(Length)
        TLVs = FsciParameter("TLVs", 1, Length)
        cmdParams.append(TLVs)
        return FsciFrameDescription(0xCF, 0xA0, cmdParams)

    def InitMESHCOP_MgmtActiveGetConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        Length = FsciParameter("Length", 4)
        cmdParams.append(Length)
        TLVs = FsciParameter("TLVs", 1, Length)
        cmdParams.append(TLVs)
        return FsciFrameDescription(0xCF, 0xA2, cmdParams)

    def InitMESHCOP_MgmtPendingGetConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        Length = FsciParameter("Length", 4)
        cmdParams.append(Length)
        TLVs = FsciParameter("TLVs", 1, Length)
        cmdParams.append(TLVs)
        return FsciFrameDescription(0xCF, 0xA4, cmdParams)

    def InitMESHCOP_MgmtCommissionerSetConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0xA1, cmdParams)

    def InitMESHCOP_MgmtActiveSetConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0xA3, cmdParams)

    def InitMESHCOP_MgmtPendingSetConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0xA5, cmdParams)

    def InitMESHCOP_MgmtSendPanIdQueryConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0xA8, cmdParams)

    def InitMESHCOP_MgmtPanIdConflictConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        ScanChannelMask = FsciParameter("ScanChannelMask", 4)
        cmdParams.append(ScanChannelMask)
        PanId = FsciParameter("PanId", 2)
        cmdParams.append(PanId)
        return FsciFrameDescription(0xCF, 0xA9, cmdParams)

    def InitMESHCOP_MgmtSendEdScanConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0xAA, cmdParams)

    def InitMESHCOP_MgmtEdReportConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        ScanChannelMask = FsciParameter("ScanChannelMask", 4)
        cmdParams.append(ScanChannelMask)
        Length = FsciParameter("Length", 1)
        cmdParams.append(Length)
        EnergyList = FsciParameter("EnergyList", 1, Length)
        cmdParams.append(EnergyList)
        return FsciFrameDescription(0xCF, 0xAB, cmdParams)

    def InitMESHCOP_MgmtSendAnnounceBeginConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0xA7, cmdParams)

    def InitDTLSOpenConfirm(self):
        cmdParams = []
        ContextIndex = FsciParameter("ContextIndex", 1)
        cmdParams.append(ContextIndex)
        return FsciFrameDescription(0xCF, 0x30, cmdParams)

    def InitDTLSCloseConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x31, cmdParams)

    def InitDTLSClosePeerConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x32, cmdParams)

    def InitDTLSConnectConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        PeerIndex = FsciParameter("PeerIndex", 1)
        cmdParams.append(PeerIndex)
        return FsciFrameDescription(0xCF, 0x33, cmdParams)

    def InitDTLSClientConnectedConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        PeerIndex = FsciParameter("PeerIndex", 1)
        cmdParams.append(PeerIndex)
        return FsciFrameDescription(0xCF, 0x34, cmdParams)

    def InitDTLSSendConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0x35, cmdParams)

    def InitDTLSReceiveConfirm(self):
        cmdParams = []
        PeerIndex = FsciParameter("PeerIndex", 1)
        cmdParams.append(PeerIndex)
        Size = FsciParameter("Size", 2)
        cmdParams.append(Size)
        Data = FsciParameter("Data", 1, Size)
        cmdParams.append(Data)
        return FsciFrameDescription(0xCF, 0x36, cmdParams)

    def InitFSCIGetUniqueIdConfirm(self):
        cmdParams = []
        UniqueId = FsciParameter("UniqueId", 16)
        cmdParams.append(UniqueId)
        return FsciFrameDescription(0xA4, 0xB0, cmdParams)

    def InitFSCIGetMcuIdConfirm(self):
        cmdParams = []
        McuId = FsciParameter("McuId", 4)
        cmdParams.append(McuId)
        return FsciFrameDescription(0xA4, 0xB1, cmdParams)

    def InitFSCIGetSwVersionsConfirm(self):
        cmdParams = []
        listSize = FsciParameter("NumberOfEntries", 1)
        cmdParams.append(listSize)
        SwVersions = FsciParameter("SwVersions", 6, listSize)
        cmdParams.append(SwVersions)
        return FsciFrameDescription(0xA4, 0xB2, cmdParams)

    def InitSniffer_MacSetPIBAttributeConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        PIBAttribute = FsciParameter("PIBAttribute", 1)
        cmdParams.append(PIBAttribute)
        DataLength = FsciParameter("DataLength", 2)
        cmdParams.append(DataLength)
        PIBAttributeValue = FsciParameter("PIBAttributeValue", 1, DataLength)
        cmdParams.append(PIBAttributeValue)
        return FsciFrameDescription(0x84, 0x0D, cmdParams)

    def InitSniffer_DetectConfirm(self):
        cmdParams = []
        SnifferType = FsciParameter("SnifferType", 1)
        cmdParams.append(SnifferType)
        return FsciFrameDescription(0x94, 0x20, cmdParams)

    def InitMAC_PromiscuousRxIndication(self):
        cmdParams = []
        LinkQuality = FsciParameter("LinkQuality", 1)
        cmdParams.append(LinkQuality)
        TimeStamp = FsciParameter("TimeStamp", 4)
        cmdParams.append(TimeStamp)
        msduLength = FsciParameter("msduLength", 1)
        cmdParams.append(msduLength)
        msdu = FsciParameter("msdu", 1, msduLength)
        cmdParams.append(msdu)
        return FsciFrameDescription(0x86, 0x03, cmdParams)

    def InitSerialTun_LinkIndicationConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xCF, 0xF1, cmdParams)

    def InitSerialTun_IPPacketReceivedConfirm(self):
        cmdParams = []
        IPpayload = FsciParameter("IPpayload", 1)
        cmdParams.append(IPpayload)
        return FsciFrameDescription(0xCF, 0xF3, cmdParams)

    def InitAspSetPowerLevelConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0x94, 0x0F, cmdParams)

    def InitAspGetPowerLevelConfirm(self):
        cmdParams = []
        Value = FsciParameter("Value", 1)
        cmdParams.append(Value)
        return FsciFrameDescription(0x94, 0x1F, cmdParams)

    def InitDBGConfirm(self):
        cmdParams = []
        text = FsciParameter("text", 1)
        cmdParams.append(text)
        return FsciFrameDescription(0xCF, 0x89, cmdParams)
