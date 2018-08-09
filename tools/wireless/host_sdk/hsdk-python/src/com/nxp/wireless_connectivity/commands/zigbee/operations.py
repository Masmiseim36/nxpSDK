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

from com.nxp.wireless_connectivity.commands.zigbee.events import *  # @UnusedWildImport
from com.nxp.wireless_connectivity.commands.fsci_operation import FsciOperation


class NetworkAddressOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.NetworkAddressRequestFrame
        self.observers = [NetworkAddressResponseObserver('NetworkAddressResponse'), ]
        super(NetworkAddressOperation, self).subscribeToEvents()


class IEEEAddressOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.IEEEAddressRequestFrame
        self.observers = [IEEEAddressResponseObserver('IEEEAddressResponse'), ]
        super(IEEEAddressOperation, self).subscribeToEvents()


class NodeDescriptorOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.NodeDescriptorRequestFrame
        self.observers = []
        super(NodeDescriptorOperation, self).subscribeToEvents()


class SimpleDescriptorOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SimpleDescriptorRequestFrame
        self.observers = [SimpleDescriptorResponseObserver('SimpleDescriptorResponse'), ]
        super(SimpleDescriptorOperation, self).subscribeToEvents()


class PowerDescriptorOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.PowerDescriptorRequestFrame
        self.observers = [PowerDescriptorResponseObserver('PowerDescriptorResponse'), ]
        super(PowerDescriptorOperation, self).subscribeToEvents()


class ActiveEndpointOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ActiveEndpointRequestFrame
        self.observers = [ActiveEndpointResponseObserver('ActiveEndpointResponse'), ]
        super(ActiveEndpointOperation, self).subscribeToEvents()


class MatchDescriptorOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.MatchDescriptorRequestFrame
        self.observers = [MatchDescriptorResponseObserver('MatchDescriptorResponse'), ]
        super(MatchDescriptorOperation, self).subscribeToEvents()


class UserDescriptorOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.UserDescriptorRequestFrame
        self.observers = [UserDescriptorResponseObserver('UserDescriptorResponse'), ]
        super(UserDescriptorOperation, self).subscribeToEvents()


class SetPollIntervalOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SetPollIntervalRequestFrame
        self.observers = []
        super(SetPollIntervalOperation, self).subscribeToEvents()


class ComplexDescriptorOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ComplexDescriptorRequestFrame
        self.observers = [ComplexDescriptorResponseObserver('ComplexDescriptorResponse'), ]
        super(ComplexDescriptorOperation, self).subscribeToEvents()


class ManagementLeaveOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ManagementLeaveRequestFrame
        self.observers = [ManagementLeaveResponseObserver('ManagementLeaveResponse'), ]
        super(ManagementLeaveOperation, self).subscribeToEvents()


class PermitJoiningOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.PermitJoiningRequestFrame
        self.observers = []
        super(PermitJoiningOperation, self).subscribeToEvents()


class ManagementNetworkUpdateOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ManagementNetworkUpdateRequestFrame
        self.observers = [ManagementNetworkUpdateResponseObserver('ManagementNetworkUpdateResponse'), ]
        super(ManagementNetworkUpdateOperation, self).subscribeToEvents()


class SystemServerDiscoveryOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SystemServerDiscoveryRequestFrame
        self.observers = [SystemServerDiscoveryResponseObserver('SystemServerDiscoveryResponse'), ]
        super(SystemServerDiscoveryOperation, self).subscribeToEvents()


class ManagementLQIOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ManagementLQIRequestFrame
        self.observers = []
        super(ManagementLQIOperation, self).subscribeToEvents()


class ReadAttributeOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ReadAttributeRequestFrame
        self.observers = []
        super(ReadAttributeOperation, self).subscribeToEvents()


class WriteAttributeOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.WriteAttributeRequestFrame
        self.observers = [WriteAttributeResponseObserver('WriteAttributeResponse'), ]
        super(WriteAttributeOperation, self).subscribeToEvents()


class AttributeDiscoveryOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.AttributeDiscoveryRequestFrame
        self.observers = []
        super(AttributeDiscoveryOperation, self).subscribeToEvents()


class ConfigureReportingOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ConfigureReportingRequestFrame
        self.observers = [ConfigureReportingResponseObserver('ConfigureReportingResponse'), ]
        super(ConfigureReportingOperation, self).subscribeToEvents()


class SceneMembershipOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SceneMembershipRequestFrame
        self.observers = [SceneMembershipResponseObserver('SceneMembershipResponse'), ]
        super(SceneMembershipOperation, self).subscribeToEvents()


class IASZoneEnrollOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [IASZoneEnrollResponseObserver('IASZoneEnrollResponse'), ]
        super(IASZoneEnrollOperation, self).subscribeToEvents()


class RawAPSDataOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.RawAPSDataRequestFrame
        self.observers = []
        super(RawAPSDataOperation, self).subscribeToEvents()


class DataOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [DataIndicationObserver('DataIndication'), ]
        super(DataOperation, self).subscribeToEvents()


class PermitJoinStatusOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [PermitJoinStatusResponseObserver('PermitJoinStatusResponse'), ]
        super(PermitJoinStatusOperation, self).subscribeToEvents()


class BindOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [BindResponseObserver('BindResponse'), ]
        super(BindOperation, self).subscribeToEvents()


class UnbindOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [UnbindResponseObserver('UnbindResponse'), ]
        super(UnbindOperation, self).subscribeToEvents()


class LeaveOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [LeaveIndicationObserver('LeaveIndication'), ]
        super(LeaveOperation, self).subscribeToEvents()


class AuthenticateOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [AuthenticateResponseObserver('AuthenticateResponse'), ]
        super(AuthenticateOperation, self).subscribeToEvents()


class ReadIndividualAttributeOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [ReadIndividualAttributeResponseObserver('ReadIndividualAttributeResponse'), ]
        super(ReadIndividualAttributeOperation, self).subscribeToEvents()


class ReportIndividualAttributeOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [ReportIndividualAttributeResponseObserver('ReportIndividualAttributeResponse'), ]
        super(ReportIndividualAttributeOperation, self).subscribeToEvents()


class DefaultOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [DefaultResponseObserver('DefaultResponse'), ]
        super(DefaultOperation, self).subscribeToEvents()


class AddGroupOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [AddGroupResponseObserver('AddGroupResponse'), ]
        super(AddGroupOperation, self).subscribeToEvents()


class ViewGroupOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [ViewGroupResponseObserver('ViewGroupResponse'), ]
        super(ViewGroupOperation, self).subscribeToEvents()


class GetGroupMembershipOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GetGroupMembershipResponseObserver('GetGroupMembershipResponse'), ]
        super(GetGroupMembershipOperation, self).subscribeToEvents()


class RemoveGroupOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [RemoveGroupResponseObserver('RemoveGroupResponse'), ]
        super(RemoveGroupOperation, self).subscribeToEvents()


class ViewSceneOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [ViewSceneResponseObserver('ViewSceneResponse'), ]
        super(ViewSceneOperation, self).subscribeToEvents()


class AddSceneOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [AddSceneResponseObserver('AddSceneResponse'), ]
        super(AddSceneOperation, self).subscribeToEvents()


class RemoveSceneOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [RemoveSceneResponseObserver('RemoveSceneResponse'), ]
        super(RemoveSceneOperation, self).subscribeToEvents()


class RemoveAllSceneOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [RemoveAllSceneResponseObserver('RemoveAllSceneResponse'), ]
        super(RemoveAllSceneOperation, self).subscribeToEvents()


class AddEnhancedSceneOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [AddEnhancedSceneResponseObserver('AddEnhancedSceneResponse'), ]
        super(AddEnhancedSceneOperation, self).subscribeToEvents()


class StoreSceneOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [StoreSceneResponseObserver('StoreSceneResponse'), ]
        super(StoreSceneOperation, self).subscribeToEvents()


class HostPersistentDataManagerAvailableOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [HostPersistentDataManagerAvailableResponseObserver(
            'HostPersistentDataManagerAvailableResponse'), ]
        super(HostPersistentDataManagerAvailableOperation, self).subscribeToEvents()


class LoadRecordOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [LoadRecordResponseObserver('LoadRecordResponse'), ]
        super(LoadRecordOperation, self).subscribeToEvents()


class SaveRecordOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [SaveRecordResponseObserver('SaveRecordResponse'), ]
        super(SaveRecordOperation, self).subscribeToEvents()


class RouterDiscoveryOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [RouterDiscoveryConfirmObserver('RouterDiscoveryConfirm'), ]
        super(RouterDiscoveryOperation, self).subscribeToEvents()
