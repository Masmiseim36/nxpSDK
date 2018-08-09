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

from com.nxp.wireless_connectivity.commands.fsci_frame_description import FsciAckPolicy
from com.nxp.wireless_connectivity.commands.fsci_operation import FsciOperation
from com.nxp.wireless_connectivity.commands.thread.events import *  # @UnusedWildImport


class MAC_MacFilteringAddEntryOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.MAC_MacFilteringAddEntryRequestFrame
        self.observers = [MAC_MacFilteringAddEntryConfirmObserver(
            'MAC_MacFilteringAddEntryConfirm'), ]
        super(MAC_MacFilteringAddEntryOperation, self).subscribeToEvents()


class MAC_MacFilteringRemoveEntryOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.MAC_MacFilteringRemoveEntryRequestFrame
        self.observers = [MAC_MacFilteringRemoveEntryConfirmObserver(
            'MAC_MacFilteringRemoveEntryConfirm'), ]
        super(MAC_MacFilteringRemoveEntryOperation, self).subscribeToEvents()


class MAC_MacFilteringEnableOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.MAC_MacFilteringEnableRequestFrame
        self.observers = [MAC_MacFilteringEnableConfirmObserver('MAC_MacFilteringEnableConfirm'), ]
        super(MAC_MacFilteringEnableOperation, self).subscribeToEvents()


class MAC_MacFilteringGetTableOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.MAC_MacFilteringGetTableRequestFrame
        self.observers = [MAC_MacFilteringGetTableConfirmObserver(
            'MAC_MacFilteringGetTableConfirm'), ]
        super(MAC_MacFilteringGetTableOperation, self).subscribeToEvents()


class THR_SetDeviceConfigOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_SetDeviceConfigRequestFrame
        self.observers = [THR_SetDeviceConfigConfirmObserver('THR_SetDeviceConfigConfirm'), ]
        super(THR_SetDeviceConfigOperation, self).subscribeToEvents()


class THR_NwkScanOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_NwkScanRequestFrame
        self.observers = [THR_NwkScanConfirmObserver('THR_NwkScanConfirm'), ]
        super(THR_NwkScanOperation, self).subscribeToEvents()


class THR_CreateNwkOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_CreateNwkRequestFrame
        self.observers = [THR_CreateNwkConfirmObserver('THR_CreateNwkConfirm'), ]
        super(THR_CreateNwkOperation, self).subscribeToEvents()


class THR_JoinOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_JoinRequestFrame
        self.observers = [THR_JoinConfirmObserver('THR_JoinConfirm'), ]
        super(THR_JoinOperation, self).subscribeToEvents()


class THR_FactoryResetOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_FactoryResetRequestFrame
        self.observers = [THR_FactoryResetConfirmObserver('THR_FactoryResetConfirm'), ]
        super(THR_FactoryResetOperation, self).subscribeToEvents()


class THR_CpuResetOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_CpuResetRequestFrame
        self.observers = [THR_CpuResetConfirmObserver('THR_CpuResetConfirm'), ]
        super(THR_CpuResetOperation, self).subscribeToEvents()


class THR_CpuResetIndicationOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [THR_CpuResetIndicationObserver('THR_CpuResetIndication'), ]
        super(THR_CpuResetIndicationOperation, self).subscribeToEvents()


class THR_DisconnectOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_DisconnectRequestFrame
        self.observers = [THR_DisconnectConfirmObserver('THR_DisconnectConfirm'), ]
        super(THR_DisconnectOperation, self).subscribeToEvents()


class THR_AttachOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_AttachRequestFrame
        self.observers = [THR_AttachConfirmObserver('THR_AttachConfirm'), ]
        super(THR_AttachOperation, self).subscribeToEvents()


class THR_PromoteAsRouterOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_PromoteAsRouterRequestFrame
        self.observers = [THR_PromoteAsRouterConfirmObserver('THR_PromoteAsRouterConfirm'), ]
        super(THR_PromoteAsRouterOperation, self).subscribeToEvents()


class THR_MgmtDiagnosticGetOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_MgmtDiagnosticGetRequestFrame
        self.observers = [THR_MgmtDiagnosticGetConfirmObserver('THR_MgmtDiagnosticGetConfirm'), ]
        super(THR_MgmtDiagnosticGetOperation, self).subscribeToEvents()


class THR_DiagTestGetOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_DiagTestGetRequestFrame
        self.observers = [THR_DiagTestGetRspIndicationObserver('THR_DiagTestGetRspIndication'), ]
        super(THR_DiagTestGetOperation, self).subscribeToEvents()


class THR_MgmtDiagnosticResetOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_MgmtDiagnosticResetRequestFrame
        self.observers = [THR_MgmtDiagnosticResetConfirmObserver(
            'THR_MgmtDiagnosticResetConfirm'), ]
        super(THR_MgmtDiagnosticResetOperation, self).subscribeToEvents()


class THR_MgmtReadMemoryOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_MgmtReadMemoryRequestFrame
        self.observers = [THR_MgmtReadMemoryConfirmObserver('THR_MgmtReadMemoryConfirm'), ]
        super(THR_MgmtReadMemoryOperation, self).subscribeToEvents()


class THR_MgmtWriteMemoryOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_MgmtWriteMemoryRequestFrame
        self.observers = [THR_MgmtWriteMemoryConfirmObserver('THR_MgmtWriteMemoryConfirm'), ]
        super(THR_MgmtWriteMemoryOperation, self).subscribeToEvents()


class THR_NwkDiscoveryOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_NwkDiscoveryRequestFrame
        self.observers = [THR_NwkDiscoveryConfirmObserver('THR_NwkDiscoveryConfirm'), ]
        super(THR_NwkDiscoveryOperation, self).subscribeToEvents()


class THR_NwkDiscoveryStopOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_NwkDiscoveryStopRequestFrame
        self.observers = [THR_NwkDiscoveryStopConfirmObserver('THR_NwkDiscoveryStopConfirm'), ]
        super(THR_NwkDiscoveryStopOperation, self).subscribeToEvents()


class THR_SearchNwkWithAnounceOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_SearchNwkWithAnounceRequestFrame
        self.observers = [THR_SearchNwkWithAnounceConfirmObserver(
            'THR_SearchNwkWithAnounceConfirm'), ]
        super(THR_SearchNwkWithAnounceOperation, self).subscribeToEvents()


class THR_ChildUpdateToParentOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_ChildUpdateToParentRequestFrame
        self.observers = [THR_ChildUpdateToParentConfirmObserver(
            'THR_ChildUpdateToParentConfirm'), ]
        super(THR_ChildUpdateToParentOperation, self).subscribeToEvents()


class THR_SetManualSlaacIIDOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_SetManualSlaacIIDRequestFrame
        self.observers = [THR_SetManualSlaacIIDConfirmObserver('THR_SetManualSlaacIIDConfirm'), ]
        super(THR_SetManualSlaacIIDOperation, self).subscribeToEvents()


class THR_SendProactiveAddrNotifOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_SendProactiveAddrNotifRequestFrame
        self.observers = [THR_SendProactiveAddrNotifConfirmObserver(
            'THR_SendProactiveAddrNotifConfirm'), ]
        super(THR_SendProactiveAddrNotifOperation, self).subscribeToEvents()


class THR_SetThresholdOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_SetThresholdRequestFrame
        self.observers = [THR_SetThresholdConfirmObserver('THR_SetThresholdConfirm'), ]
        super(THR_SetThresholdOperation, self).subscribeToEvents()


class THR_SetNwkIdTimeoutOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_SetNwkIdTimeoutRequestFrame
        self.observers = [THR_SetNwkIdTimeoutConfirmObserver('THR_SetNwkIdTimeoutConfirm'), ]
        super(THR_SetNwkIdTimeoutOperation, self).subscribeToEvents()


class THR_GetRoutingTableOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_GetRoutingTableRequestFrame
        self.observers = [THR_GetRoutingTableConfirmObserver('THR_GetRoutingTableConfirm'), ]
        super(THR_GetRoutingTableOperation, self).subscribeToEvents()


class THR_GetNeighborTableOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_GetNeighborTableRequestFrame
        self.observers = [THR_GetNeighborTableConfirmObserver('THR_GetNeighborTableConfirm'), ]
        super(THR_GetNeighborTableOperation, self).subscribeToEvents()


class THR_GetNeighborInfoOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_GetNeighborInfoRequestFrame
        self.observers = [THR_GetNeighborInfoConfirmObserver('THR_GetNeighborInfoConfirm'), ]
        super(THR_GetNeighborInfoOperation, self).subscribeToEvents()


class THR_GetChildrenTableOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_GetChildrenTableRequestFrame
        self.observers = [THR_GetChildrenTableConfirmObserver('THR_GetChildrenTableConfirm'), ]
        super(THR_GetChildrenTableOperation, self).subscribeToEvents()


class THR_GetAttrOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_GetAttrRequestFrame
        self.observers = [THR_GetAttrConfirmObserver('THR_GetAttrConfirm'), ]
        super(THR_GetAttrOperation, self).subscribeToEvents()


class THR_SetAttrOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_SetAttrRequestFrame
        self.observers = [THR_SetAttrConfirmObserver('THR_SetAttrConfirm'), ]
        super(THR_SetAttrOperation, self).subscribeToEvents()


class THR_LeaderRemoveRouterIdOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_LeaderRemoveRouterIdRequestFrame
        self.observers = [THR_LeaderRemoveRouterIdConfirmObserver(
            'THR_LeaderRemoveRouterIdConfirm'), ]
        super(THR_LeaderRemoveRouterIdOperation, self).subscribeToEvents()


class THR_GenerateAllKeysOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_GenerateAllKeysRequestFrame
        self.observers = [THR_GenerateAllKeysConfirmObserver('THR_GenerateAllKeysConfirm'), ]
        super(THR_GenerateAllKeysOperation, self).subscribeToEvents()


class THR_SwitchKeyOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_SwitchKeyRequestFrame
        self.observers = []
        super(THR_SwitchKeyOperation, self).subscribeToEvents()


class THR_BrPrefixAddEntryOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_BrPrefixAddEntryRequestFrame
        self.observers = [THR_BrPrefixAddEntryConfirmObserver('THR_BrPrefixAddEntryConfirm'), ]
        super(THR_BrPrefixAddEntryOperation, self).subscribeToEvents()


class THR_BrPrefixGetTableOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_BrPrefixGetTableRequestFrame
        self.observers = [THR_BrPrefixGetTableConfirmObserver('THR_BrPrefixGetTableConfirm'), ]
        super(THR_BrPrefixGetTableOperation, self).subscribeToEvents()


class THR_BrPrefixRemoveEntryOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_BrPrefixRemoveEntryRequestFrame
        self.observers = [THR_BrPrefixRemoveEntryConfirmObserver(
            'THR_BrPrefixRemoveEntryConfirm'), ]
        super(THR_BrPrefixRemoveEntryOperation, self).subscribeToEvents()


class THR_BrServiceRemoveEntryOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_BrServiceRemoveEntryRequestFrame
        self.observers = [THR_BrServiceRemoveEntryConfirmObserver(
            'THR_BrServiceRemoveEntryConfirm'), ]
        super(THR_BrServiceRemoveEntryOperation, self).subscribeToEvents()


class THR_BrServiceAddEntryOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_BrServiceAddEntryRequestFrame
        self.observers = [THR_BrServiceAddEntryConfirmObserver('THR_BrServiceAddEntryConfirm'), ]
        super(THR_BrServiceAddEntryOperation, self).subscribeToEvents()


class THR_BrPrefixSyncOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_BrPrefixSyncRequestFrame
        self.observers = [THR_BrPrefixSyncConfirmObserver('THR_BrPrefixSyncConfirm'), ]
        super(THR_BrPrefixSyncOperation, self).subscribeToEvents()


class THR_BrPrefixRemoveAllOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_BrPrefixRemoveAllRequestFrame
        self.observers = [THR_BrPrefixRemoveAllConfirmObserver('THR_BrPrefixRemoveAllConfirm'), ]
        super(THR_BrPrefixRemoveAllOperation, self).subscribeToEvents()


class THR_GetThreadIpAddrOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_GetThreadIpAddrRequestFrame
        self.observers = [THR_GetThreadIpAddrConfirmObserver('THR_GetThreadIpAddrConfirm'), ]
        super(THR_GetThreadIpAddrOperation, self).subscribeToEvents()


class THR_GetParentOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_GetParentRequestFrame
        self.observers = [THR_GetParentConfirmObserver('THR_GetParentConfirm'), ]
        super(THR_GetParentOperation, self).subscribeToEvents()


class NWKU_EidToRlocMapOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.NWKU_EidToRlocMapRequestFrame
        self.observers = [NWKU_EidToRlocMapResponseObserver('NWKU_EidToRlocMapResponse'), ]
        super(NWKU_EidToRlocMapOperation, self).subscribeToEvents()


class THR_IdentifyOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.THR_IdentifyRequestFrame
        self.observers = [THR_IdentifyConfirmObserver('THR_IdentifyConfirm'), ]
        super(THR_IdentifyOperation, self).subscribeToEvents()


class SocketCreateOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SocketCreateRequestFrame
        self.observers = [SocketCreateConfirmObserver('SocketCreateConfirm'), ]
        super(SocketCreateOperation, self).subscribeToEvents()


class SocketShutdownOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SocketShutdownRequestFrame
        self.observers = [SocketShutdownConfirmObserver('SocketShutdownConfirm'), ]
        super(SocketShutdownOperation, self).subscribeToEvents()


class SocketBindOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SocketBindRequestFrame
        self.observers = [SocketBindConfirmObserver('SocketBindConfirm'), ]
        super(SocketBindOperation, self).subscribeToEvents()


class SocketSendOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SocketSendRequestFrame
        self.observers = [SocketSendConfirmObserver('SocketSendConfirm'), ]
        super(SocketSendOperation, self).subscribeToEvents()


class SocketSendToOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SocketSendToRequestFrame
        self.observers = [SocketSendToConfirmObserver('SocketSendToConfirm'), ]
        super(SocketSendToOperation, self).subscribeToEvents()


class SocketReceiveOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SocketReceiveRequestFrame
        self.observers = [SocketReceiveConfirmObserver('SocketReceiveConfirm'), ]
        super(SocketReceiveOperation, self).subscribeToEvents()


class SocketReceiveFromOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SocketReceiveFromRequestFrame
        self.observers = [SocketReceiveFromConfirmObserver('SocketReceiveFromConfirm'), ]
        super(SocketReceiveFromOperation, self).subscribeToEvents()


class SocketConnectOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SocketConnectRequestFrame
        self.observers = [SocketConnectConfirmObserver('SocketConnectConfirm'), ]
        super(SocketConnectOperation, self).subscribeToEvents()


class SocketListenOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SocketListenRequestFrame
        self.observers = [SocketListenConfirmObserver('SocketListenConfirm'), ]
        super(SocketListenOperation, self).subscribeToEvents()


class SocketAcceptOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SocketAcceptRequestFrame
        self.observers = [SocketAcceptConfirmObserver('SocketAcceptConfirm'), ]
        super(SocketAcceptOperation, self).subscribeToEvents()


class SocketSetOptionOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SocketSetOptionRequestFrame
        self.observers = [SocketSetOptionConfirmObserver('SocketSetOptionConfirm'), ]
        super(SocketSetOptionOperation, self).subscribeToEvents()


class SocketGetOptionOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SocketGetOptionRequestFrame
        self.observers = [SocketGetOptionConfirmObserver('SocketGetOptionConfirm'), ]
        super(SocketGetOptionOperation, self).subscribeToEvents()


class MESHCOP_StartCommissionerOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.MESHCOP_StartCommissionerRequestFrame
        self.observers = [MESHCOP_StartCommissionerConfirmObserver(
            'MESHCOP_StartCommissionerConfirm'), ]
        super(MESHCOP_StartCommissionerOperation, self).subscribeToEvents()


class MESHCOP_StartNativeCommissionerOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.MESHCOP_StartNativeCommissionerRequestFrame
        self.observers = [MESHCOP_StartNativeCommissionerConfirmObserver(
            'MESHCOP_StartNativeCommissionerConfirm'), ]
        super(MESHCOP_StartNativeCommissionerOperation, self).subscribeToEvents()


class MESHCOP_StopCommissionerOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.MESHCOP_StopCommissionerRequestFrame
        self.observers = [MESHCOP_StopCommissionerConfirmObserver(
            'MESHCOP_StopCommissionerConfirm'), ]
        super(MESHCOP_StopCommissionerOperation, self).subscribeToEvents()


class MESHCOP_AddExpectedJoinerOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.MESHCOP_AddExpectedJoinerRequestFrame
        self.observers = [MESHCOP_AddExpectedJoinerConfirmObserver(
            'MESHCOP_AddExpectedJoinerConfirm'), ]
        super(MESHCOP_AddExpectedJoinerOperation, self).subscribeToEvents()


class MESHCOP_GetExpectedJoinerOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.MESHCOP_GetExpectedJoinerRequestFrame
        self.observers = [MESHCOP_GetExpectedJoinerConfirmObserver(
            'MESHCOP_GetExpectedJoinerConfirm'), ]
        super(MESHCOP_GetExpectedJoinerOperation, self).subscribeToEvents()


class MESHCOP_RemoveExpectedJoinerOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.MESHCOP_RemoveExpectedJoinerRequestFrame
        self.observers = [MESHCOP_RemoveExpectedJoinerConfirmObserver(
            'MESHCOP_RemoveExpectedJoinerConfirm'), ]
        super(MESHCOP_RemoveExpectedJoinerOperation, self).subscribeToEvents()


class MESHCOP_RemoveAllExpectedJoinersOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.MESHCOP_RemoveAllExpectedJoinersRequestFrame
        self.observers = [MESHCOP_RemoveAllExpectedJoinersConfirmObserver(
            'MESHCOP_RemoveAllExpectedJoinersConfirm'), ]
        super(MESHCOP_RemoveAllExpectedJoinersOperation, self).subscribeToEvents()


class MESHCOP_SyncSteeringDataOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.MESHCOP_SyncSteeringDataRequestFrame
        self.observers = [MESHCOP_SyncSteeringDataConfirmObserver(
            'MESHCOP_SyncSteeringDataConfirm'), ]
        super(MESHCOP_SyncSteeringDataOperation, self).subscribeToEvents()


class MESHCOP_MgmtSetOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.MESHCOP_MgmtSetRequestFrame
        self.observers = [MESHCOP_MgmtSetConfirmObserver('MESHCOP_MgmtSetConfirm'), ]
        super(MESHCOP_MgmtSetOperation, self).subscribeToEvents()


class MESHCOP_MgmtGetOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.MESHCOP_MgmtGetRequestFrame
        self.observers = [MESHCOP_MgmtGetConfirmObserver('MESHCOP_MgmtGetConfirm'), ]
        super(MESHCOP_MgmtGetOperation, self).subscribeToEvents()


class MESHCOP_SetCommissionerCredentialOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.MESHCOP_SetCommissionerCredentialRequestFrame
        self.observers = [MESHCOP_SetCommissionerCredentialConfirmObserver(
            'MESHCOP_SetCommissionerCredentialConfirm'), ]
        super(MESHCOP_SetCommissionerCredentialOperation, self).subscribeToEvents()


class MESHCOP_MgmNwkFormOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.MESHCOP_MgmNwkFormRequestFrame
        self.observers = [MESHCOP_MgmNwkFormConfirmObserver('MESHCOP_MgmNwkFormConfirm'), ]
        super(MESHCOP_MgmNwkFormOperation, self).subscribeToEvents()


class MESHCOP_MgmtCommissionerGetOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.MESHCOP_MgmtCommissionerGetRequestFrame
        self.observers = [MESHCOP_MgmtCommissionerGetConfirmObserver(
            'MESHCOP_MgmtCommissionerGetConfirm'), ]
        super(MESHCOP_MgmtCommissionerGetOperation, self).subscribeToEvents()


class MESHCOP_MgmtActiveGetOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.MESHCOP_MgmtActiveGetRequestFrame
        self.observers = [MESHCOP_MgmtActiveGetConfirmObserver('MESHCOP_MgmtActiveGetConfirm'), ]
        super(MESHCOP_MgmtActiveGetOperation, self).subscribeToEvents()


class MESHCOP_MgmtPendingGetOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.MESHCOP_MgmtPendingGetRequestFrame
        self.observers = [MESHCOP_MgmtPendingGetConfirmObserver('MESHCOP_MgmtPendingGetConfirm'), ]
        super(MESHCOP_MgmtPendingGetOperation, self).subscribeToEvents()


class MESHCOP_MgmtCommissionerSetOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.MESHCOP_MgmtCommissionerSetRequestFrame
        self.observers = [MESHCOP_MgmtCommissionerSetConfirmObserver(
            'MESHCOP_MgmtCommissionerSetConfirm'), ]
        super(MESHCOP_MgmtCommissionerSetOperation, self).subscribeToEvents()


class MESHCOP_MgmtActiveSetOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.MESHCOP_MgmtActiveSetRequestFrame
        self.observers = [MESHCOP_MgmtActiveSetConfirmObserver('MESHCOP_MgmtActiveSetConfirm'), ]
        super(MESHCOP_MgmtActiveSetOperation, self).subscribeToEvents()


class MESHCOP_MgmtPendingSetOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.MESHCOP_MgmtPendingSetRequestFrame
        self.observers = [MESHCOP_MgmtPendingSetConfirmObserver('MESHCOP_MgmtPendingSetConfirm'), ]
        super(MESHCOP_MgmtPendingSetOperation, self).subscribeToEvents()


class MESHCOP_MgmtSendPanIdQueryOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.MESHCOP_MgmtSendPanIdQueryRequestFrame
        self.observers = [MESHCOP_MgmtSendPanIdQueryConfirmObserver(
            'MESHCOP_MgmtSendPanIdQueryConfirm'), ]
        super(MESHCOP_MgmtSendPanIdQueryOperation, self).subscribeToEvents()


class MESHCOP_MgmtSendEdScanOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.MESHCOP_MgmtSendEdScanRequestFrame
        self.observers = [MESHCOP_MgmtSendEdScanConfirmObserver('MESHCOP_MgmtSendEdScanConfirm'), ]
        super(MESHCOP_MgmtSendEdScanOperation, self).subscribeToEvents()


class MESHCOP_MgmtSendAnnounceBeginOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.MESHCOP_MgmtSendAnnounceBeginRequestFrame
        self.observers = [MESHCOP_MgmtSendAnnounceBeginConfirmObserver(
            'MESHCOP_MgmtSendAnnounceBeginConfirm'), ]
        super(MESHCOP_MgmtSendAnnounceBeginOperation, self).subscribeToEvents()


class NWKU_IfconfigBindOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.NWKU_IfconfigBindRequestFrame
        self.observers = [NWKU_IfconfigBindConfirmObserver('NWKU_IfconfigBindConfirm'), ]
        super(NWKU_IfconfigBindOperation, self).subscribeToEvents()


class NWKU_IfconfigAllOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.NWKU_IfconfigAllRequestFrame
        self.observers = [NWKU_IfconfigAllResponseObserver('NWKU_IfconfigAllResponse'), ]
        super(NWKU_IfconfigAllOperation, self).subscribeToEvents()


class NWKU_PingOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.NWKU_PingRequestFrame
        self.observers = [NWKU_PingConfirmObserver('NWKU_PingConfirm'), ]
        super(NWKU_PingOperation, self).subscribeToEvents()


class NWKU_EchoUDPOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.NWKU_EchoUDPRequestFrame
        self.observers = [NWKU_EchoUDPConfirmObserver('NWKU_EchoUDPConfirm'), ]
        super(NWKU_EchoUDPOperation, self).subscribeToEvents()


class NWKU_CPUResetOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.NWKU_CPUResetRequestFrame
        self.observers = []
        super(NWKU_CPUResetOperation, self).subscribeToEvents()


class NWKU_CoapCreateInstanceOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.NWKU_CoapCreateInstanceRequestFrame
        self.observers = [NWKU_CoapCreateInstanceConfirmObserver(
            'NWKU_CoapCreateInstanceConfirm'), ]
        super(NWKU_CoapCreateInstanceOperation, self).subscribeToEvents()


class NWKU_CoapSendOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.NWKU_CoapSendRequestFrame
        self.observers = [NWKU_CoapSendConfirmObserver('NWKU_CoapSendConfirm'), ]
        super(NWKU_CoapSendOperation, self).subscribeToEvents()


class NWKU_CoapRegisterOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.NWKU_CoapRegisterRequestFrame
        self.observers = [NWKU_CoapRegisterConfirmObserver('NWKU_CoapRegisterConfirm'), ]
        super(NWKU_CoapRegisterOperation, self).subscribeToEvents()


class NWKU_DnsSendOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.NWKU_DnsSendRequestRequestFrame
        self.observers = [NWKU_DnsSendRequestConfirmObserver('NWKU_DnsSendRequestConfirm'), ]
        super(NWKU_DnsSendOperation, self).subscribeToEvents()


class NWKU_McastGroupShowOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.NWKU_McastGroupShowRequestFrame
        self.observers = [NWKU_McastGroupShowResponseObserver('NWKU_McastGroupShowResponse'), ]
        super(NWKU_McastGroupShowOperation, self).subscribeToEvents()


class NWKU_McastGroupManageOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.NWKU_McastGroupManageRequestFrame
        self.observers = [NWKU_McastGroupManageConfirmObserver('NWKU_McastGroupManageConfirm'), ]
        super(NWKU_McastGroupManageOperation, self).subscribeToEvents()


class NWKU_RoutesShowOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.NWKU_RoutesShowRequestFrame
        self.observers = [NWKU_RoutesShowResponseObserver('NWKU_RoutesShowResponse'), ]
        super(NWKU_RoutesShowOperation, self).subscribeToEvents()


class NWKU_RoutesManageOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.NWKU_RoutesManageRequestFrame
        self.observers = [NWKU_RoutesManageConfirmObserver('NWKU_RoutesManageConfirm'), ]
        super(NWKU_RoutesManageOperation, self).subscribeToEvents()


class DTLSOpenOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.DTLSOpenRequestFrame
        self.observers = [DTLSOpenConfirmObserver('DTLSOpenConfirm'), ]
        super(DTLSOpenOperation, self).subscribeToEvents()


class DTLSCloseOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.DTLSCloseRequestFrame
        self.observers = [DTLSCloseConfirmObserver('DTLSCloseConfirm'), ]
        super(DTLSCloseOperation, self).subscribeToEvents()


class DTLSClosePeerOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.DTLSClosePeerRequestFrame
        self.observers = [DTLSClosePeerConfirmObserver('DTLSClosePeerConfirm'), ]
        super(DTLSClosePeerOperation, self).subscribeToEvents()


class DTLSConnectOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.DTLSConnectRequestFrame
        self.observers = [DTLSConnectConfirmObserver('DTLSConnectConfirm'), ]
        super(DTLSConnectOperation, self).subscribeToEvents()


class DTLSSendOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.DTLSSendRequestFrame
        self.observers = [DTLSSendConfirmObserver('DTLSSendConfirm'), ]
        super(DTLSSendOperation, self).subscribeToEvents()


class FSCIGetUniqueIdOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.FSCIGetUniqueIdRequestFrame
        self.observers = [FSCIGetUniqueIdConfirmObserver('FSCIGetUniqueIdConfirm'), ]
        super(FSCIGetUniqueIdOperation, self).subscribeToEvents()


class FSCIGetMcuIdOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.FSCIGetMcuIdRequestFrame
        self.observers = [FSCIGetMcuIdConfirmObserver('FSCIGetMcuIdConfirm'), ]
        super(FSCIGetMcuIdOperation, self).subscribeToEvents()


class FSCIGetSwVersionsOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.FSCIGetSwVersionsRequestFrame
        self.observers = [FSCIGetSwVersionsConfirmObserver('FSCIGetSwVersionsConfirm'), ]
        super(FSCIGetSwVersionsOperation, self).subscribeToEvents()


class SerialTun_LinkIndicationOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SerialTun_LinkIndicationRequestFrame
        self.observers = [SerialTun_LinkIndicationConfirmObserver(
            'SerialTun_LinkIndicationConfirm'), ]
        super(SerialTun_LinkIndicationOperation, self).subscribeToEvents()


class SerialTun_IPPacketSendOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SerialTun_IPPacketSendRequestFrame
        self.observers = []
        super(SerialTun_IPPacketSendOperation, self).subscribeToEvents()


class Sniffer_MacSetPIBAttributeOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.Sniffer_MacSetPIBAttributeRequestFrame
        self.observers = [Sniffer_MacSetPIBAttributeConfirmObserver(
            'Sniffer_MacSetPIBAttributeConfirm'), ]
        super(Sniffer_MacSetPIBAttributeOperation, self).subscribeToEvents()


class Sniffer_DetectOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.Sniffer_DetectRequestFrame
        self.observers = [Sniffer_DetectConfirmObserver(
            'Sniffer_DetectConfirm'), ]
        super(Sniffer_DetectOperation, self).subscribeToEvents()


class FSCIEnterBootloaderOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.FSCIEnterBootloaderRequestFrame
        self.observers = []
        super(FSCIEnterBootloaderOperation, self).subscribeToEvents()


class AspSetPowerLevelOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.AspSetPowerLevelRequestFrame
        self.observers = [AspSetPowerLevelConfirmObserver('AspSetPowerLevelConfirm'), ]
        super(AspSetPowerLevelOperation, self).subscribeToEvents()


class AspGetPowerLevelOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.AspGetPowerLevelRequestFrame
        self.observers = [AspGetPowerLevelConfirmObserver('AspGetPowerLevelConfirm'), ]
        super(AspGetPowerLevelOperation, self).subscribeToEvents()


class THR_EventNwkScanOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [THR_EventNwkScanConfirmObserver('THR_EventNwkScanConfirm'), ]
        super(THR_EventNwkScanOperation, self).subscribeToEvents()


class THR_EventNwkCreateOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [THR_EventNwkCreateConfirmObserver('THR_EventNwkCreateConfirm'), ]
        super(THR_EventNwkCreateOperation, self).subscribeToEvents()


class THR_EventNwkJoinOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [THR_EventNwkJoinConfirmObserver('THR_EventNwkJoinConfirm'), ]
        super(THR_EventNwkJoinOperation, self).subscribeToEvents()


class THR_EventNwkJoinSelectParentsOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [THR_EventNwkJoinSelectParentsConfirmObserver(
            'THR_EventNwkJoinSelectParentsConfirm'), ]
        super(THR_EventNwkJoinSelectParentsOperation, self).subscribeToEvents()


class THR_EventGeneralOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [THR_EventGeneralConfirmObserver('THR_EventGeneralConfirm'), ]
        super(THR_EventGeneralOperation, self).subscribeToEvents()


class THR_EventNwkCommissioningOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [THR_EventNwkCommissioningIndicationObserver(
            'THR_EventNwkCommissioningIndication'), ]
        super(THR_EventNwkCommissioningOperation, self).subscribeToEvents()


class THR_CommissioningDiagnosticOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [THR_CommissioningDiagnosticIndicationObserver(
            'THR_CommissioningDiagnosticIndication'), ]
        super(THR_CommissioningDiagnosticOperation, self).subscribeToEvents()


class THR_MgmtDiagnosticGetRspOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [THR_MgmtDiagnosticGetRspIndicationObserver(
            'THR_MgmtDiagnosticGetRspIndication'), ]
        super(THR_MgmtDiagnosticGetRspOperation, self).subscribeToEvents()


class THR_DiagTestGetConfirmOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [THR_DiagTestGetConfirmObserver('THR_DiagTestGetConfirm'), ]
        super(THR_DiagTestGetConfirmOperation, self).subscribeToEvents()


class THR_MgmtDiagnosticDiagTestEventOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [THR_MgmtDiagnosticDiagTestEventIndicationObserver(
            'THR_MgmtDiagnosticDiagTestEventIndication'), ]
        super(THR_MgmtDiagnosticDiagTestEventOperation, self).subscribeToEvents()


class THR_MgmtDiagnosticResetRspOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [THR_MgmtDiagnosticResetRspIndicationObserver(
            'THR_MgmtDiagnosticResetRspIndication'), ]
        super(THR_MgmtDiagnosticResetRspOperation, self).subscribeToEvents()


class THR_SwitchKeyKeyOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [THR_SwitchKeyKeyConfirmObserver('THR_SwitchKeyKeyConfirm'), ]
        super(THR_SwitchKeyKeyOperation, self).subscribeToEvents()


class NWKU_CoapMsgReceivedOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [NWKU_CoapMsgReceivedIndicationObserver(
            'NWKU_CoapMsgReceivedIndication'), ]
        super(NWKU_CoapMsgReceivedOperation, self).subscribeToEvents()


class NWKU_DnsResponseReceivedOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [NWKU_DnsResponseReceivedIndicationObserver(
            'NWKU_DnsResponseReceivedIndication'), ]
        super(NWKU_DnsResponseReceivedOperation, self).subscribeToEvents()


class MESHCOP_MgmtPanIdConflictOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [MESHCOP_MgmtPanIdConflictConfirmObserver(
            'MESHCOP_MgmtPanIdConflictConfirm'), ]
        super(MESHCOP_MgmtPanIdConflictOperation, self).subscribeToEvents()


class MESHCOP_MgmtEdReportOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [MESHCOP_MgmtEdReportConfirmObserver('MESHCOP_MgmtEdReportConfirm'), ]
        super(MESHCOP_MgmtEdReportOperation, self).subscribeToEvents()


class DTLSClientConnectedOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [DTLSClientConnectedConfirmObserver('DTLSClientConnectedConfirm'), ]
        super(DTLSClientConnectedOperation, self).subscribeToEvents()


class DTLSReceiveOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [DTLSReceiveConfirmObserver('DTLSReceiveConfirm'), ]
        super(DTLSReceiveOperation, self).subscribeToEvents()


class MAC_PromiscuousRxOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [MAC_PromiscuousRxIndicationObserver('MAC_PromiscuousRxIndication'), ]
        super(MAC_PromiscuousRxOperation, self).subscribeToEvents()


class SerialTun_IPPacketReceivedOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [SerialTun_IPPacketReceivedConfirmObserver(
            'SerialTun_IPPacketReceivedConfirm'), ]
        super(SerialTun_IPPacketReceivedOperation, self).subscribeToEvents()


class DBGOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [DBGConfirmObserver('DBGConfirm'), ]
        super(DBGOperation, self).subscribeToEvents()


class FSCI_AckOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [FSCIACKObserver('FSCI_Ack'), ]
        super(FSCI_AckOperation, self).subscribeToEvents()


def subscribe_to_async_thread_events_from(device, ack_policy=FsciAckPolicy.GLOBAL):
    thread_events = [
        THR_EventNwkScanOperation(device, ack_policy=ack_policy),
        THR_EventNwkCreateOperation(device, ack_policy=ack_policy),
        THR_EventNwkJoinOperation(device, ack_policy=ack_policy),
        THR_EventNwkJoinSelectParentsOperation(device, ack_policy=ack_policy),
        THR_EventGeneralOperation(device, ack_policy=ack_policy),
        THR_EventNwkCommissioningOperation(device, ack_policy=ack_policy),
        THR_CommissioningDiagnosticOperation(device, ack_policy=ack_policy),
        MESHCOP_MgmtPanIdConflictOperation(device, ack_policy=ack_policy),
        MESHCOP_MgmtEdReportOperation(device, ack_policy=ack_policy),
        THR_CpuResetIndicationOperation(device, ack_policy=ack_policy),
        THR_DiagTestGetConfirmOperation(device, ack_policy=ack_policy)
    ]

    for thread_event in thread_events:
        thread_event.subscribeToEvents()
