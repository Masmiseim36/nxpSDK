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

from com.nxp.wireless_connectivity.commands.fsci_frame_description import FsciFrameDescription
from com.nxp.wireless_connectivity.commands.fsci_parameter import FsciParameter
from com.nxp.wireless_connectivity.hsdk.singleton import singleton


@singleton
class Spec(object):

    def __init__(self):
        self.GetVersionFrame = self.InitGetVersion()
        self.SetExtendedPANIDFrame = self.InitSetExtendedPANID()
        self.SetChannelMaskFrame = self.InitSetChannelMask()
        self.SetSecurityStateKeyFrame = self.InitSetSecurityStateKey()
        self.SetDeviceTypeFrame = self.InitSetDeviceType()
        self.StartNetworkMessageFrame = self.InitStartNetworkMessage()
        self.StartNetworkScanFrame = self.InitStartNetworkScan()
        self.ZLO_ZLLFactoryNewResetFrame = self.InitZLO_ZLLFactoryNewReset()
        self.PermitJoinStatusOnTheTargetFrame = self.InitPermitJoinStatusOnTheTarget()
        self.ResetFrame = self.InitReset()
        self.ErasePersistentDataFrame = self.InitErasePersistentData()
        self.BindFrame = self.InitBind()
        self.UnbindFrame = self.InitUnbind()
        self.NetworkAddressRequestFrame = self.InitNetworkAddressRequest()
        self.IEEEAddressRequestFrame = self.InitIEEEAddressRequest()
        self.NodeDescriptorRequestFrame = self.InitNodeDescriptorRequest()
        self.SimpleDescriptorRequestFrame = self.InitSimpleDescriptorRequest()
        self.PowerDescriptorRequestFrame = self.InitPowerDescriptorRequest()
        self.ActiveEndpointRequestFrame = self.InitActiveEndpointRequest()
        self.MatchDescriptorRequestFrame = self.InitMatchDescriptorRequest()
        self.RemoveDeviceFrame = self.InitRemoveDevice()
        self.UserDescriptorSetFrame = self.InitUserDescriptorSet()
        self.UserDescriptorRequestFrame = self.InitUserDescriptorRequest()
        self.SetPollIntervalRequestFrame = self.InitSetPollIntervalRequest()
        self.ComplexDescriptorRequestFrame = self.InitComplexDescriptorRequest()
        self.ManagementLeaveRequestFrame = self.InitManagementLeaveRequest()
        self.PermitJoiningRequestFrame = self.InitPermitJoiningRequest()
        self.ManagementNetworkUpdateRequestFrame = self.InitManagementNetworkUpdateRequest()
        self.SystemServerDiscoveryRequestFrame = self.InitSystemServerDiscoveryRequest()
        self.ManagementLQIRequestFrame = self.InitManagementLQIRequest()
        self.ReadAttributeRequestFrame = self.InitReadAttributeRequest()
        self.WriteAttributeRequestFrame = self.InitWriteAttributeRequest()
        self.AttributeDiscoveryRequestFrame = self.InitAttributeDiscoveryRequest()
        self.EnablePermissionsControlledJoinsFrame = self.InitEnablePermissionsControlledJoins()
        self.AuthenticateDeviceFrame = self.InitAuthenticateDevice()
        self.ConfigureReportingRequestFrame = self.InitConfigureReportingRequest()
        self.AddGroupFrame = self.InitAddGroup()
        self.ViewGroupFrame = self.InitViewGroup()
        self.GetGroupMembershipFrame = self.InitGetGroupMembership()
        self.RemoveGroupFrame = self.InitRemoveGroup()
        self.RemoveAllGroupsFrame = self.InitRemoveAllGroups()
        self.AddGroupIfIdentifyFrame = self.InitAddGroupIfIdentify()
        self.IdentifySendFrame = self.InitIdentifySend()
        self.IdentifyQueryFrame = self.InitIdentifyQuery()
        self.IdentifyTriggerEffectsFrame = self.InitIdentifyTriggerEffects()
        self.MoveToLevelFrame = self.InitMoveToLevel()
        self.MoveToLevelWithWithoutOnOffFrame = self.InitMoveToLevelWithWithoutOnOff()
        self.MoveStepFrame = self.InitMoveStep()
        self.MoveStopWithOnOffFrame = self.InitMoveStopWithOnOff()
        self.OnOffWithEffectsSendFrame = self.InitOnOffWithEffectsSend()
        self.OnOffWithNoEffectsFrame = self.InitOnOffWithNoEffects()
        self.OnOffTimedSendFrame = self.InitOnOffTimedSend()
        self.ViewSceneFrame = self.InitViewScene()
        self.AddSceneFrame = self.InitAddScene()
        self.RemoveSceneFrame = self.InitRemoveScene()
        self.RemoveAllScenesFrame = self.InitRemoveAllScenes()
        self.StoreSceneFrame = self.InitStoreScene()
        self.RecallSceneFrame = self.InitRecallScene()
        self.SceneMembershipRequestFrame = self.InitSceneMembershipRequest()
        self.MoveToHueFrame = self.InitMoveToHue()
        self.MoveHueFrame = self.InitMoveHue()
        self.StepHueFrame = self.InitStepHue()
        self.MoveToSaturationFrame = self.InitMoveToSaturation()
        self.MoveSaturationFrame = self.InitMoveSaturation()
        self.StepsaturationFrame = self.InitStepsaturation()
        self.MoveToHueAndSaturationFrame = self.InitMoveToHueAndSaturation()
        self.MoveToColourFrame = self.InitMoveToColour()
        self.MoveColourFrame = self.InitMoveColour()
        self.StepColourFrame = self.InitStepColour()
        self.InitiateTouchlinkFrame = self.InitInitiateTouchlink()
        self.TouchLinkFactoryResetTargetFrame = self.InitTouchLinkFactoryResetTarget()
        self.OnOffNoEffectsFrame = self.InitOnOffNoEffects()
        self.AddEnhancedSceneFrame = self.InitAddEnhancedScene()
        self.ViewEnhancedSceneFrame = self.InitViewEnhancedScene()
        self.CopySceneFrame = self.InitCopyScene()
        self.EnhancedMoveToHueFrame = self.InitEnhancedMoveToHue()
        self.EnhancedMoveHueFrame = self.InitEnhancedMoveHue()
        self.EnhancedStepHueFrame = self.InitEnhancedStepHue()
        self.EnhancedMoveToHueAndSaturationFrame = self.InitEnhancedMoveToHueAndSaturation()
        self.ColourLoopSetFrame = self.InitColourLoopSet()
        self.StopMoveStepFrame = self.InitStopMoveStep()
        self.MoveToColourTemperatureFrame = self.InitMoveToColourTemperature()
        self.MoveColourTemperatureFrame = self.InitMoveColourTemperature()
        self.StepColourTemperatureFrame = self.InitStepColourTemperature()
        self.LockUnlockDoorFrame = self.InitLockUnlockDoor()
        self.IASZoneEnrollResponseFrame = self.InitIASZoneEnrollResponse()
        self.RawAPSDataRequestFrame = self.InitRawAPSDataRequest()
        self.StatusFrame = self.InitStatus()
        self.LogMessageFrame = self.InitLogMessage()
        self.DataIndicationFrame = self.InitDataIndication()
        self.NodeClusterListFrame = self.InitNodeClusterList()
        self.NodeClusterAttributeListFrame = self.InitNodeClusterAttributeList()
        self.NodeCommandIDListFrame = self.InitNodeCommandIDList()
        self.VersionListFrame = self.InitVersionList()
        self.NetworkJoinedFormedFrame = self.InitNetworkJoinedFormed()
        self.PermitJoinStatusResponseFrame = self.InitPermitJoinStatusResponse()
        self.NonFactoryNewRestartFrame = self.InitNonFactoryNewRestart()
        self.FactoryNewRestartFrame = self.InitFactoryNewRestart()
        self.BindResponseFrame = self.InitBindResponse()
        self.UnbindResponseFrame = self.InitUnbindResponse()
        self.NetworkAddressResponseFrame = self.InitNetworkAddressResponse()
        self.IEEEAddressResponseFrame = self.InitIEEEAddressResponse()
        self.SimpleDescriptorResponseFrame = self.InitSimpleDescriptorResponse()
        self.PowerDescriptorResponseFrame = self.InitPowerDescriptorResponse()
        self.ActiveEndpointResponseFrame = self.InitActiveEndpointResponse()
        self.MatchDescriptorResponseFrame = self.InitMatchDescriptorResponse()
        self.DeviceAnnounceFrame = self.InitDeviceAnnounce()
        self.UserDescriptorResponseFrame = self.InitUserDescriptorResponse()
        self.UserDescriptorNotifyFrame = self.InitUserDescriptorNotify()
        self.ComplexDescriptorResponseFrame = self.InitComplexDescriptorResponse()
        self.ManagementLeaveResponseFrame = self.InitManagementLeaveResponse()
        self.LeaveIndicationFrame = self.InitLeaveIndication()
        self.ManagementNetworkUpdateResponseFrame = self.InitManagementNetworkUpdateResponse()
        self.SystemServerDiscoveryResponseFrame = self.InitSystemServerDiscoveryResponse()
        self.AuthenticateResponseFrame = self.InitAuthenticateResponse()
        self.ConfigureReportingResponseFrame = self.InitConfigureReportingResponse()
        self.ReadIndividualAttributeResponseFrame = self.InitReadIndividualAttributeResponse()
        self.WriteAttributeResponseFrame = self.InitWriteAttributeResponse()
        self.ReportIndividualAttributeResponseFrame = self.InitReportIndividualAttributeResponse()
        self.DefaultResponseFrame = self.InitDefaultResponse()
        self.AddGroupResponseFrame = self.InitAddGroupResponse()
        self.ViewGroupResponseFrame = self.InitViewGroupResponse()
        self.GetGroupMembershipResponseFrame = self.InitGetGroupMembershipResponse()
        self.RemoveGroupResponseFrame = self.InitRemoveGroupResponse()
        self.ViewSceneResponseFrame = self.InitViewSceneResponse()
        self.AddSceneResponseFrame = self.InitAddSceneResponse()
        self.RemoveSceneResponseFrame = self.InitRemoveSceneResponse()
        self.RemoveAllSceneResponseFrame = self.InitRemoveAllSceneResponse()
        self.AddEnhancedSceneResponseFrame = self.InitAddEnhancedSceneResponse()
        self.StoreSceneResponseFrame = self.InitStoreSceneResponse()
        self.SceneMembershipResponseFrame = self.InitSceneMembershipResponse()
        self.TouchlinkStatusFrame = self.InitTouchlinkStatus()
        self.ZoneStatusChangeNotificationFrame = self.InitZoneStatusChangeNotification()
        self.HostPersistentDataManagerAvailableResponseFrame = self.InitHostPersistentDataManagerAvailableResponse()
        self.LoadRecordResponseFrame = self.InitLoadRecordResponse()
        self.SaveRecordResponseFrame = self.InitSaveRecordResponse()
        self.RouterDiscoveryConfirmFrame = self.InitRouterDiscoveryConfirm()
        self.APSDataConfirmFailFrame = self.InitAPSDataConfirmFail()

    def InitGetVersion(self):
        cmdParams = []
        return FsciFrameDescription(0x37, 0x10, cmdParams)

    def InitSetExtendedPANID(self):
        cmdParams = []
        ExtendedPANID = FsciParameter("ExtendedPANID", 8)
        cmdParams.append(ExtendedPANID)
        return FsciFrameDescription(0x37, 0x20, cmdParams)

    def InitSetChannelMask(self):
        cmdParams = []
        ChannelMask = FsciParameter("ChannelMask", 4)
        cmdParams.append(ChannelMask)
        return FsciFrameDescription(0x37, 0x21, cmdParams)

    def InitSetSecurityStateKey(self):
        cmdParams = []
        KeyType = FsciParameter("KeyType", 1)
        cmdParams.append(KeyType)
        KeyData = FsciParameter("KeyData", 1)
        cmdParams.append(KeyData)
        return FsciFrameDescription(0x37, 0x22, cmdParams)

    def InitSetDeviceType(self):
        cmdParams = []
        DeviceType = FsciParameter("DeviceType", 1)
        cmdParams.append(DeviceType)
        return FsciFrameDescription(0x37, 0x23, cmdParams)

    def InitStartNetworkMessage(self):
        cmdParams = []
        return FsciFrameDescription(0x37, 0x24, cmdParams)

    def InitStartNetworkScan(self):
        cmdParams = []
        return FsciFrameDescription(0x37, 0x25, cmdParams)

    def InitZLO_ZLLFactoryNewReset(self):
        cmdParams = []
        return FsciFrameDescription(0x37, 0x13, cmdParams)

    def InitPermitJoinStatusOnTheTarget(self):
        cmdParams = []
        return FsciFrameDescription(0x37, 0x14, cmdParams)

    def InitReset(self):
        cmdParams = []
        return FsciFrameDescription(0x37, 0x11, cmdParams)

    def InitErasePersistentData(self):
        cmdParams = []
        return FsciFrameDescription(0x37, 0x12, cmdParams)

    def InitBind(self):
        cmdParams = []
        TargetExtendedAddress = FsciParameter("TargetExtendedAddress", 8)
        cmdParams.append(TargetExtendedAddress)
        TargetEndPoint = FsciParameter("TargetEndPoint", 1)
        cmdParams.append(TargetEndPoint)
        ClusterID = FsciParameter("ClusterID", 2)
        cmdParams.append(ClusterID)
        DestAddressingMode = FsciParameter("DestAddressingMode", 1)
        cmdParams.append(DestAddressingMode)
        DestinationAddressdict = {}
        currentList = []
        DestinationAddressNO_PAN = FsciParameter("DestinationAddressNO_PAN", 1)
        currentList.append(DestinationAddressNO_PAN)
        DestinationAddressdict[0x00] = currentList
        currentList = []
        DestinationAddressReserved = FsciParameter("DestinationAddressReserved", 1)
        currentList.append(DestinationAddressReserved)
        DestinationAddressdict[0x01] = currentList
        currentList = []
        DestinationAddressAddress_16b = FsciParameter("DestinationAddressAddress_16b", 2)
        currentList.append(DestinationAddressAddress_16b)
        DestinationAddressdict[0x02] = currentList
        currentList = []
        DestinationAddressAddress_64b = FsciParameter("DestinationAddressAddress_64b", 8)
        currentList.append(DestinationAddressAddress_64b)
        DestinationAddressdict[0x03] = currentList
        DestinationAddress = FsciParameter("DestinationAddress", 1, DestAddressingMode, DestinationAddressdict)
        cmdParams.append(DestinationAddress)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        return FsciFrameDescription(0x37, 0x30, cmdParams)

    def InitUnbind(self):
        cmdParams = []
        TargetExtendedAddress = FsciParameter("TargetExtendedAddress", 8)
        cmdParams.append(TargetExtendedAddress)
        TargetEndPoint = FsciParameter("TargetEndPoint", 1)
        cmdParams.append(TargetEndPoint)
        ClusterID = FsciParameter("ClusterID", 2)
        cmdParams.append(ClusterID)
        DestAddressingMode = FsciParameter("DestAddressingMode", 1)
        cmdParams.append(DestAddressingMode)
        DestinationAddressdict = {}
        currentList = []
        DestinationAddressNO_PAN = FsciParameter("DestinationAddressNO_PAN", 1)
        currentList.append(DestinationAddressNO_PAN)
        DestinationAddressdict[0x00] = currentList
        currentList = []
        DestinationAddressReserved = FsciParameter("DestinationAddressReserved", 1)
        currentList.append(DestinationAddressReserved)
        DestinationAddressdict[0x01] = currentList
        currentList = []
        DestinationAddressAddress_16b = FsciParameter("DestinationAddressAddress_16b", 2)
        currentList.append(DestinationAddressAddress_16b)
        DestinationAddressdict[0x02] = currentList
        currentList = []
        DestinationAddressAddress_64b = FsciParameter("DestinationAddressAddress_64b", 8)
        currentList.append(DestinationAddressAddress_64b)
        DestinationAddressdict[0x03] = currentList
        DestinationAddress = FsciParameter("DestinationAddress", 1, DestAddressingMode, DestinationAddressdict)
        cmdParams.append(DestinationAddress)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        return FsciFrameDescription(0x37, 0x31, cmdParams)

    def InitNetworkAddressRequest(self):
        cmdParams = []
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        ExtendedAddress = FsciParameter("ExtendedAddress", 8)
        cmdParams.append(ExtendedAddress)
        RequestType = FsciParameter("RequestType", 1)
        cmdParams.append(RequestType)
        StartIndex = FsciParameter("StartIndex", 1)
        cmdParams.append(StartIndex)
        return FsciFrameDescription(0x37, 0x40, cmdParams)

    def InitIEEEAddressRequest(self):
        cmdParams = []
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        ShortAddress = FsciParameter("ShortAddress", 2)
        cmdParams.append(ShortAddress)
        RequestType = FsciParameter("RequestType", 1)
        cmdParams.append(RequestType)
        StartIndex = FsciParameter("StartIndex", 1)
        cmdParams.append(StartIndex)
        return FsciFrameDescription(0x37, 0x41, cmdParams)

    def InitNodeDescriptorRequest(self):
        cmdParams = []
        SequenceNumber = FsciParameter("SequenceNumber", 1)
        cmdParams.append(SequenceNumber)
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        NetworkAddress = FsciParameter("NetworkAddress", 2)
        cmdParams.append(NetworkAddress)
        ManufacturerCode = FsciParameter("ManufacturerCode", 2)
        cmdParams.append(ManufacturerCode)
        MaxRxSize = FsciParameter("MaxRxSize", 2)
        cmdParams.append(MaxRxSize)
        MaxTxSize = FsciParameter("MaxTxSize", 2)
        cmdParams.append(MaxTxSize)
        ServerMask = FsciParameter("ServerMask", 1, MaxRxSize)
        cmdParams.append(ServerMask)
        DescriptorCapability = FsciParameter("DescriptorCapability", 1)
        cmdParams.append(DescriptorCapability)
        MacCapability = FsciParameter("MacCapability", 1)
        cmdParams.append(MacCapability)
        MaxBufferSize = FsciParameter("MaxBufferSize", 1)
        cmdParams.append(MaxBufferSize)
        BitFields = FsciParameter("BitFields", 2)
        cmdParams.append(BitFields)
        return FsciFrameDescription(0x37, 0x42, cmdParams)

    def InitSimpleDescriptorRequest(self):
        cmdParams = []
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        EndPoint = FsciParameter("EndPoint", 1)
        cmdParams.append(EndPoint)
        return FsciFrameDescription(0x37, 0x43, cmdParams)

    def InitPowerDescriptorRequest(self):
        cmdParams = []
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        return FsciFrameDescription(0x37, 0x44, cmdParams)

    def InitActiveEndpointRequest(self):
        cmdParams = []
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        return FsciFrameDescription(0x37, 0x45, cmdParams)

    def InitMatchDescriptorRequest(self):
        cmdParams = []
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        ProfileID = FsciParameter("ProfileID", 2)
        cmdParams.append(ProfileID)
        NumberOfInputClusters = FsciParameter("NumberOfInputClusters", 1)
        cmdParams.append(NumberOfInputClusters)
        InputClusterList = FsciParameter("InputClusterList", 2, NumberOfInputClusters)
        cmdParams.append(InputClusterList)
        NumberOfOutputClusters = FsciParameter("NumberOfOutputClusters", 1)
        cmdParams.append(NumberOfOutputClusters)
        OutputClusterList = FsciParameter("OutputClusterList", 2, NumberOfOutputClusters)
        cmdParams.append(OutputClusterList)
        return FsciFrameDescription(0x37, 0x46, cmdParams)

    def InitRemoveDevice(self):
        cmdParams = []
        ParentExtendedAddress = FsciParameter("ParentExtendedAddress", 8)
        cmdParams.append(ParentExtendedAddress)
        ChildExtendedAddress = FsciParameter("ChildExtendedAddress", 8)
        cmdParams.append(ChildExtendedAddress)
        return FsciFrameDescription(0x37, 0x26, cmdParams)

    def InitUserDescriptorSet(self):
        cmdParams = []
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        AddressOfInterest = FsciParameter("AddressOfInterest", 2)
        cmdParams.append(AddressOfInterest)
        StringLength = FsciParameter("StringLength", 1)
        cmdParams.append(StringLength)
        Parameters = FsciParameter("Parameters", 1, StringLength)
        cmdParams.append(Parameters)
        return FsciFrameDescription(0x37, 0x2B, cmdParams)

    def InitUserDescriptorRequest(self):
        cmdParams = []
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        AddressOfInterest = FsciParameter("AddressOfInterest", 2)
        cmdParams.append(AddressOfInterest)
        return FsciFrameDescription(0x37, 0x2C, cmdParams)

    def InitSetPollIntervalRequest(self):
        cmdParams = []
        Interval = FsciParameter("Interval", 4)
        cmdParams.append(Interval)
        return FsciFrameDescription(0x37, 0x2D, cmdParams)

    def InitComplexDescriptorRequest(self):
        cmdParams = []
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        AddressOfInterest = FsciParameter("AddressOfInterest", 2)
        cmdParams.append(AddressOfInterest)
        return FsciFrameDescription(0x37, 0x34, cmdParams)

    def InitManagementLeaveRequest(self):
        cmdParams = []
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        ExtendedAddress = FsciParameter("ExtendedAddress", 8)
        cmdParams.append(ExtendedAddress)
        Rejoin = FsciParameter("Rejoin", 1)
        cmdParams.append(Rejoin)
        RemoveChildren = FsciParameter("RemoveChildren", 1)
        cmdParams.append(RemoveChildren)
        return FsciFrameDescription(0x37, 0x47, cmdParams)

    def InitPermitJoiningRequest(self):
        cmdParams = []
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        Interval = FsciParameter("Interval", 1)
        cmdParams.append(Interval)
        TCSignificance = FsciParameter("TCSignificance", 1)
        cmdParams.append(TCSignificance)
        return FsciFrameDescription(0x37, 0x49, cmdParams)

    def InitManagementNetworkUpdateRequest(self):
        cmdParams = []
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        ChannelMask = FsciParameter("ChannelMask", 4)
        cmdParams.append(ChannelMask)
        ScanDuration = FsciParameter("ScanDuration", 1)
        cmdParams.append(ScanDuration)
        ScanCount = FsciParameter("ScanCount", 1)
        cmdParams.append(ScanCount)
        NetworkUpdateID = FsciParameter("NetworkUpdateID", 1)
        cmdParams.append(NetworkUpdateID)
        NetworkManagerShortAddress = FsciParameter("NetworkManagerShortAddress", 2)
        cmdParams.append(NetworkManagerShortAddress)
        return FsciFrameDescription(0x37, 0x4A, cmdParams)

    def InitSystemServerDiscoveryRequest(self):
        cmdParams = []
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        ServerMask = FsciParameter("ServerMask", 2)
        cmdParams.append(ServerMask)
        return FsciFrameDescription(0x37, 0x4B, cmdParams)

    def InitManagementLQIRequest(self):
        cmdParams = []
        TargetAddress = FsciParameter("TargetAddress", 2)
        cmdParams.append(TargetAddress)
        StartIndex = FsciParameter("StartIndex", 1)
        cmdParams.append(StartIndex)
        return FsciFrameDescription(0x37, 0x4E, cmdParams)

    def InitReadAttributeRequest(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        ClusterID = FsciParameter("ClusterID", 2)
        cmdParams.append(ClusterID)
        Direction = FsciParameter("Direction", 1)
        cmdParams.append(Direction)
        ManufacturerSpecific = FsciParameter("ManufacturerSpecific", 1)
        cmdParams.append(ManufacturerSpecific)
        ManufacturerID = FsciParameter("ManufacturerID", 2)
        cmdParams.append(ManufacturerID)
        NumberOfAttributes = FsciParameter("NumberOfAttributes", 1)
        cmdParams.append(NumberOfAttributes)
        AttributesList = FsciParameter("AttributesList", 2, NumberOfAttributes)
        cmdParams.append(AttributesList)
        return FsciFrameDescription(0x38, 0x00, cmdParams)

    def InitWriteAttributeRequest(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        ClusterID = FsciParameter("ClusterID", 2)
        cmdParams.append(ClusterID)
        Direction = FsciParameter("Direction", 1)
        cmdParams.append(Direction)
        ManufacturerSpecific = FsciParameter("ManufacturerSpecific", 1)
        cmdParams.append(ManufacturerSpecific)
        ManufacturerID = FsciParameter("ManufacturerID", 2)
        cmdParams.append(ManufacturerID)
        NumberOfAttributes = FsciParameter("NumberOfAttributes", 1)
        cmdParams.append(NumberOfAttributes)
        AttributesList = FsciParameter("AttributesList", 2, NumberOfAttributes)
        cmdParams.append(AttributesList)
        return FsciFrameDescription(0x38, 0x10, cmdParams)

    def InitAttributeDiscoveryRequest(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        ClusterID = FsciParameter("ClusterID", 2)
        cmdParams.append(ClusterID)
        AttributeID = FsciParameter("AttributeID", 2)
        cmdParams.append(AttributeID)
        Direction = FsciParameter("Direction", 1)
        cmdParams.append(Direction)
        ManufacturerSpecific = FsciParameter("ManufacturerSpecific", 1)
        cmdParams.append(ManufacturerSpecific)
        ManufacturerID = FsciParameter("ManufacturerID", 2)
        cmdParams.append(ManufacturerID)
        NumberOfAttributes = FsciParameter("NumberOfAttributes", 1)
        cmdParams.append(NumberOfAttributes)
        MaxNumberOfIdentifiers = FsciParameter("MaxNumberOfIdentifiers", 1, NumberOfAttributes)
        cmdParams.append(MaxNumberOfIdentifiers)
        return FsciFrameDescription(0x38, 0x40, cmdParams)

    def InitEnablePermissionsControlledJoins(self):
        cmdParams = []
        EnableDisable = FsciParameter("EnableDisable", 1)
        cmdParams.append(EnableDisable)
        return FsciFrameDescription(0x37, 0x27, cmdParams)

    def InitAuthenticateDevice(self):
        cmdParams = []
        IEEEAddress = FsciParameter("IEEEAddress", 8)
        cmdParams.append(IEEEAddress)
        Key = FsciParameter("Key", 16)
        cmdParams.append(Key)
        return FsciFrameDescription(0x37, 0x28, cmdParams)

    def InitConfigureReportingRequest(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndpoint = FsciParameter("DestinationEndpoint", 1)
        cmdParams.append(DestinationEndpoint)
        ClusterID = FsciParameter("ClusterID", 2)
        cmdParams.append(ClusterID)
        Direction = FsciParameter("Direction", 1)
        cmdParams.append(Direction)
        ManufacturerSpecific = FsciParameter("ManufacturerSpecific", 1)
        cmdParams.append(ManufacturerSpecific)
        ManufacturerID = FsciParameter("ManufacturerID", 2)
        cmdParams.append(ManufacturerID)
        NumberOfAttributes = FsciParameter("NumberOfAttributes", 1)
        cmdParams.append(NumberOfAttributes)
        AttributesListAttributeDirection = FsciParameter("AttributesListAttributeDirection", 1, NumberOfAttributes)
        cmdParams.append(AttributesListAttributeDirection)
        AttributesListAttributeType = FsciParameter("AttributesListAttributeType", 1, NumberOfAttributes)
        cmdParams.append(AttributesListAttributeType)
        AttributesListAttributeID = FsciParameter("AttributesListAttributeID", 2, NumberOfAttributes)
        cmdParams.append(AttributesListAttributeID)
        AttributesListMinInterval = FsciParameter("AttributesListMinInterval", 2, NumberOfAttributes)
        cmdParams.append(AttributesListMinInterval)
        AttributesListMaxInterval = FsciParameter("AttributesListMaxInterval", 2, NumberOfAttributes)
        cmdParams.append(AttributesListMaxInterval)
        AttributesListTimeout = FsciParameter("AttributesListTimeout", 2, NumberOfAttributes)
        cmdParams.append(AttributesListTimeout)
        AttributesListChange = FsciParameter("AttributesListChange", 1, NumberOfAttributes)
        cmdParams.append(AttributesListChange)
        return FsciFrameDescription(0x38, 0x20, cmdParams)

    def InitAddGroup(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        GroupAddress = FsciParameter("GroupAddress", 2)
        cmdParams.append(GroupAddress)
        return FsciFrameDescription(0x37, 0x60, cmdParams)

    def InitViewGroup(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        GroupAddress = FsciParameter("GroupAddress", 2)
        cmdParams.append(GroupAddress)
        return FsciFrameDescription(0x37, 0x61, cmdParams)

    def InitGetGroupMembership(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        GroupCount = FsciParameter("GroupCount", 1)
        cmdParams.append(GroupCount)
        GroupList = FsciParameter("GroupList", 2, GroupCount)
        cmdParams.append(GroupList)
        return FsciFrameDescription(0x37, 0x62, cmdParams)

    def InitRemoveGroup(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        GroupAddress = FsciParameter("GroupAddress", 2)
        cmdParams.append(GroupAddress)
        return FsciFrameDescription(0x37, 0x63, cmdParams)

    def InitRemoveAllGroups(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        return FsciFrameDescription(0x37, 0x64, cmdParams)

    def InitAddGroupIfIdentify(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        GroupAddress = FsciParameter("GroupAddress", 2)
        cmdParams.append(GroupAddress)
        return FsciFrameDescription(0x37, 0x65, cmdParams)

    def InitIdentifySend(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        TimeInSecconds = FsciParameter("TimeInSecconds", 2)
        cmdParams.append(TimeInSecconds)
        return FsciFrameDescription(0x37, 0x70, cmdParams)

    def InitIdentifyQuery(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        return FsciFrameDescription(0x37, 0x71, cmdParams)

    def InitIdentifyTriggerEffects(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        EffectID = FsciParameter("EffectID", 1)
        cmdParams.append(EffectID)
        EffectGradient = FsciParameter("EffectGradient", 1)
        cmdParams.append(EffectGradient)
        return FsciFrameDescription(0x37, 0xE0, cmdParams)

    def InitMoveToLevel(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        OnOff = FsciParameter("OnOff", 1)
        cmdParams.append(OnOff)
        Mode = FsciParameter("Mode", 1)
        cmdParams.append(Mode)
        Rate = FsciParameter("Rate", 1)
        cmdParams.append(Rate)
        return FsciFrameDescription(0x37, 0x80, cmdParams)

    def InitMoveToLevelWithWithoutOnOff(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        OnOff = FsciParameter("OnOff", 1)
        cmdParams.append(OnOff)
        Level = FsciParameter("Level", 1)
        cmdParams.append(Level)
        TransitionTime = FsciParameter("TransitionTime", 2)
        cmdParams.append(TransitionTime)
        return FsciFrameDescription(0x37, 0x81, cmdParams)

    def InitMoveStep(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        OnOff = FsciParameter("OnOff", 1)
        cmdParams.append(OnOff)
        StepMode = FsciParameter("StepMode", 1)
        cmdParams.append(StepMode)
        StepSize = FsciParameter("StepSize", 1)
        cmdParams.append(StepSize)
        TransitionTime = FsciParameter("TransitionTime", 2)
        cmdParams.append(TransitionTime)
        return FsciFrameDescription(0x37, 0x82, cmdParams)

    def InitMoveStopWithOnOff(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        return FsciFrameDescription(0x37, 0x84, cmdParams)

    def InitOnOffWithEffectsSend(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        EffectID = FsciParameter("EffectID", 1)
        cmdParams.append(EffectID)
        EffectGradient = FsciParameter("EffectGradient", 1)
        cmdParams.append(EffectGradient)
        return FsciFrameDescription(0x37, 0x94, cmdParams)

    def InitOnOffWithNoEffects(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        CommandID = FsciParameter("CommandID", 1)
        cmdParams.append(CommandID)
        return FsciFrameDescription(0x37, 0x92, cmdParams)

    def InitOnOffTimedSend(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        OnOff = FsciParameter("OnOff", 1)
        cmdParams.append(OnOff)
        OnTimeInSecconds = FsciParameter("OnTimeInSecconds", 2)
        cmdParams.append(OnTimeInSecconds)
        OffTimeInSecconds = FsciParameter("OffTimeInSecconds", 2)
        cmdParams.append(OffTimeInSecconds)
        return FsciFrameDescription(0x37, 0x93, cmdParams)

    def InitViewScene(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        GroupID = FsciParameter("GroupID", 2)
        cmdParams.append(GroupID)
        SceneID = FsciParameter("SceneID", 1)
        cmdParams.append(SceneID)
        return FsciFrameDescription(0x37, 0xA0, cmdParams)

    def InitAddScene(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        GroupID = FsciParameter("GroupID", 2)
        cmdParams.append(GroupID)
        SceneID = FsciParameter("SceneID", 1)
        cmdParams.append(SceneID)
        TransitionTime = FsciParameter("TransitionTime", 2)
        cmdParams.append(TransitionTime)
        SceneNameLength = FsciParameter("SceneNameLength", 1)
        cmdParams.append(SceneNameLength)
        SceneNameMaxLength = FsciParameter("SceneNameMaxLength", 1)
        cmdParams.append(SceneNameMaxLength)
        SceneNameData = FsciParameter("SceneNameData", 1, SceneNameLength)
        cmdParams.append(SceneNameData)
        return FsciFrameDescription(0x37, 0xA1, cmdParams)

    def InitRemoveScene(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        GroupID = FsciParameter("GroupID", 2)
        cmdParams.append(GroupID)
        SceneID = FsciParameter("SceneID", 1)
        cmdParams.append(SceneID)
        return FsciFrameDescription(0x37, 0xA2, cmdParams)

    def InitRemoveAllScenes(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        GroupID = FsciParameter("GroupID", 2)
        cmdParams.append(GroupID)
        return FsciFrameDescription(0x37, 0xA3, cmdParams)

    def InitStoreScene(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        GroupID = FsciParameter("GroupID", 2)
        cmdParams.append(GroupID)
        SceneID = FsciParameter("SceneID", 1)
        cmdParams.append(SceneID)
        return FsciFrameDescription(0x37, 0xA4, cmdParams)

    def InitRecallScene(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        GroupID = FsciParameter("GroupID", 2)
        cmdParams.append(GroupID)
        SceneID = FsciParameter("SceneID", 1)
        cmdParams.append(SceneID)
        return FsciFrameDescription(0x37, 0xA5, cmdParams)

    def InitSceneMembershipRequest(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        GroupID = FsciParameter("GroupID", 2)
        cmdParams.append(GroupID)
        return FsciFrameDescription(0x37, 0xA6, cmdParams)

    def InitMoveToHue(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        Hue = FsciParameter("Hue", 1)
        cmdParams.append(Hue)
        Direction = FsciParameter("Direction", 1)
        cmdParams.append(Direction)
        TransitionTime = FsciParameter("TransitionTime", 2)
        cmdParams.append(TransitionTime)
        return FsciFrameDescription(0x37, 0xB0, cmdParams)

    def InitMoveHue(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        Mode = FsciParameter("Mode", 1)
        cmdParams.append(Mode)
        Rate = FsciParameter("Rate", 1)
        cmdParams.append(Rate)
        return FsciFrameDescription(0x37, 0xB1, cmdParams)

    def InitStepHue(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        Mode = FsciParameter("Mode", 1)
        cmdParams.append(Mode)
        StepSize = FsciParameter("StepSize", 1)
        cmdParams.append(StepSize)
        TransitionTime = FsciParameter("TransitionTime", 1)
        cmdParams.append(TransitionTime)
        return FsciFrameDescription(0x37, 0xB2, cmdParams)

    def InitMoveToSaturation(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        Saturation = FsciParameter("Saturation", 1)
        cmdParams.append(Saturation)
        TransitionTime = FsciParameter("TransitionTime", 2)
        cmdParams.append(TransitionTime)
        return FsciFrameDescription(0x37, 0xB3, cmdParams)

    def InitMoveSaturation(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        Mode = FsciParameter("Mode", 1)
        cmdParams.append(Mode)
        Rate = FsciParameter("Rate", 2)
        cmdParams.append(Rate)
        return FsciFrameDescription(0x37, 0xB4, cmdParams)

    def InitStepsaturation(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        Mode = FsciParameter("Mode", 1)
        cmdParams.append(Mode)
        StepSize = FsciParameter("StepSize", 1)
        cmdParams.append(StepSize)
        TransitionTime = FsciParameter("TransitionTime", 1)
        cmdParams.append(TransitionTime)
        return FsciFrameDescription(0x37, 0xB5, cmdParams)

    def InitMoveToHueAndSaturation(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        Hue = FsciParameter("Hue", 1)
        cmdParams.append(Hue)
        Saturation = FsciParameter("Saturation", 1)
        cmdParams.append(Saturation)
        TransitionTime = FsciParameter("TransitionTime", 1)
        cmdParams.append(TransitionTime)
        return FsciFrameDescription(0x37, 0xB6, cmdParams)

    def InitMoveToColour(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        ColorX = FsciParameter("ColorX", 2)
        cmdParams.append(ColorX)
        ColorY = FsciParameter("ColorY", 2)
        cmdParams.append(ColorY)
        TransitionTime = FsciParameter("TransitionTime", 2)
        cmdParams.append(TransitionTime)
        return FsciFrameDescription(0x37, 0xB7, cmdParams)

    def InitMoveColour(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        ColorX = FsciParameter("ColorX", 2)
        cmdParams.append(ColorX)
        ColorY = FsciParameter("ColorY", 2)
        cmdParams.append(ColorY)
        return FsciFrameDescription(0x37, 0xB8, cmdParams)

    def InitStepColour(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        ColorX = FsciParameter("ColorX", 2)
        cmdParams.append(ColorX)
        ColorY = FsciParameter("ColorY", 2)
        cmdParams.append(ColorY)
        TransitionTime = FsciParameter("TransitionTime", 2)
        cmdParams.append(TransitionTime)
        return FsciFrameDescription(0x37, 0xB7, cmdParams)

    def InitInitiateTouchlink(self):
        cmdParams = []
        return FsciFrameDescription(0x37, 0xD0, cmdParams)

    def InitTouchLinkFactoryResetTarget(self):
        cmdParams = []
        return FsciFrameDescription(0x37, 0xD2, cmdParams)

    def InitOnOffNoEffects(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        CommandID = FsciParameter("CommandID", 1)
        cmdParams.append(CommandID)
        return FsciFrameDescription(0x37, 0x92, cmdParams)

    def InitAddEnhancedScene(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        GroupID = FsciParameter("GroupID", 2)
        cmdParams.append(GroupID)
        SceneID = FsciParameter("SceneID", 1)
        cmdParams.append(SceneID)
        TransitionTime = FsciParameter("TransitionTime", 2)
        cmdParams.append(TransitionTime)
        SceneNameLength = FsciParameter("SceneNameLength", 1)
        cmdParams.append(SceneNameLength)
        SceneNameData = FsciParameter("SceneNameData", 1, SceneNameLength)
        cmdParams.append(SceneNameData)
        Length = FsciParameter("Length", 1)
        cmdParams.append(Length)
        MaxLength = FsciParameter("MaxLength", 1)
        cmdParams.append(MaxLength)
        ExtensionFieldSets = FsciParameter("ExtensionFieldSets", 1, Length)
        cmdParams.append(ExtensionFieldSets)
        return FsciFrameDescription(0x37, 0xA7, cmdParams)

    def InitViewEnhancedScene(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        GroupID = FsciParameter("GroupID", 2)
        cmdParams.append(GroupID)
        SceneID = FsciParameter("SceneID", 1)
        cmdParams.append(SceneID)
        return FsciFrameDescription(0x37, 0xA8, cmdParams)

    def InitCopyScene(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        Mode = FsciParameter("Mode", 1)
        cmdParams.append(Mode)
        FromGroupID = FsciParameter("FromGroupID", 2)
        cmdParams.append(FromGroupID)
        FromSceneID = FsciParameter("FromSceneID", 1)
        cmdParams.append(FromSceneID)
        ToGroupID = FsciParameter("ToGroupID", 2)
        cmdParams.append(ToGroupID)
        ToSceneID = FsciParameter("ToSceneID", 1)
        cmdParams.append(ToSceneID)
        return FsciFrameDescription(0x37, 0xA9, cmdParams)

    def InitEnhancedMoveToHue(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        Direction = FsciParameter("Direction", 1)
        cmdParams.append(Direction)
        EnhancedHue = FsciParameter("EnhancedHue", 2)
        cmdParams.append(EnhancedHue)
        TransitionTime = FsciParameter("TransitionTime", 2)
        cmdParams.append(TransitionTime)
        return FsciFrameDescription(0x37, 0xBA, cmdParams)

    def InitEnhancedMoveHue(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        Mode = FsciParameter("Mode", 1)
        cmdParams.append(Mode)
        Rate = FsciParameter("Rate", 2)
        cmdParams.append(Rate)
        return FsciFrameDescription(0x37, 0xBB, cmdParams)

    def InitEnhancedStepHue(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        Mode = FsciParameter("Mode", 1)
        cmdParams.append(Mode)
        StepSize = FsciParameter("StepSize", 2)
        cmdParams.append(StepSize)
        TransitionTime = FsciParameter("TransitionTime", 2)
        cmdParams.append(TransitionTime)
        return FsciFrameDescription(0x37, 0xBC, cmdParams)

    def InitEnhancedMoveToHueAndSaturation(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        Saturation = FsciParameter("Saturation", 1)
        cmdParams.append(Saturation)
        EnhancedHue = FsciParameter("EnhancedHue", 2)
        cmdParams.append(EnhancedHue)
        TransitionTime = FsciParameter("TransitionTime", 2)
        cmdParams.append(TransitionTime)
        return FsciFrameDescription(0x37, 0xBD, cmdParams)

    def InitColourLoopSet(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        UpdateFlags = FsciParameter("UpdateFlags", 1)
        cmdParams.append(UpdateFlags)
        Action = FsciParameter("Action", 1)
        cmdParams.append(Action)
        Direction = FsciParameter("Direction", 1)
        cmdParams.append(Direction)
        Time = FsciParameter("Time", 1)
        cmdParams.append(Time)
        StartHue = FsciParameter("StartHue", 4)
        cmdParams.append(StartHue)
        return FsciFrameDescription(0x37, 0xBE, cmdParams)

    def InitStopMoveStep(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        UpdateFlags = FsciParameter("UpdateFlags", 1)
        cmdParams.append(UpdateFlags)
        Action = FsciParameter("Action", 1)
        cmdParams.append(Action)
        Direction = FsciParameter("Direction", 1)
        cmdParams.append(Direction)
        Time = FsciParameter("Time", 1)
        cmdParams.append(Time)
        StartHue = FsciParameter("StartHue", 4)
        cmdParams.append(StartHue)
        return FsciFrameDescription(0x37, 0xBF, cmdParams)

    def InitMoveToColourTemperature(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        ColourTemperature = FsciParameter("ColourTemperature", 2)
        cmdParams.append(ColourTemperature)
        TransitionTime = FsciParameter("TransitionTime", 2)
        cmdParams.append(TransitionTime)
        return FsciFrameDescription(0x37, 0xC0, cmdParams)

    def InitMoveColourTemperature(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        Mode = FsciParameter("Mode", 1)
        cmdParams.append(Mode)
        Rate = FsciParameter("Rate", 2)
        cmdParams.append(Rate)
        MinimumTemperature = FsciParameter("MinimumTemperature", 2)
        cmdParams.append(MinimumTemperature)
        MaximumTemperature = FsciParameter("MaximumTemperature", 2)
        cmdParams.append(MaximumTemperature)
        OptionsMask = FsciParameter("OptionsMask", 1)
        cmdParams.append(OptionsMask)
        OptionsOverride = FsciParameter("OptionsOverride", 1)
        cmdParams.append(OptionsOverride)
        return FsciFrameDescription(0x37, 0xC1, cmdParams)

    def InitStepColourTemperature(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        Mode = FsciParameter("Mode", 1)
        cmdParams.append(Mode)
        StepSize = FsciParameter("StepSize", 2)
        cmdParams.append(StepSize)
        TransitionTime = FsciParameter("TransitionTime", 2)
        cmdParams.append(TransitionTime)
        MinimumTemperature = FsciParameter("MinimumTemperature", 2)
        cmdParams.append(MinimumTemperature)
        MaximumTemperature = FsciParameter("MaximumTemperature", 2)
        cmdParams.append(MaximumTemperature)
        return FsciFrameDescription(0x37, 0xC2, cmdParams)

    def InitLockUnlockDoor(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        LockUnlock = FsciParameter("LockUnlock", 1)
        cmdParams.append(LockUnlock)
        return FsciFrameDescription(0x37, 0xF0, cmdParams)

    def InitIASZoneEnrollResponse(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        EnrollResponseCode = FsciParameter("EnrollResponseCode", 1)
        cmdParams.append(EnrollResponseCode)
        ZoneID = FsciParameter("ZoneID", 1)
        cmdParams.append(ZoneID)
        return FsciFrameDescription(0x3B, 0x00, cmdParams)

    def InitRawAPSDataRequest(self):
        cmdParams = []
        AddressMode = FsciParameter("AddressMode", 1)
        cmdParams.append(AddressMode)
        TargetShortAddress = FsciParameter("TargetShortAddress", 2)
        cmdParams.append(TargetShortAddress)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        ClusterID = FsciParameter("ClusterID", 2)
        cmdParams.append(ClusterID)
        ProfileID = FsciParameter("ProfileID", 2)
        cmdParams.append(ProfileID)
        SecurityMode = FsciParameter("SecurityMode", 1)
        cmdParams.append(SecurityMode)
        Radius = FsciParameter("Radius", 1)
        cmdParams.append(Radius)
        DataLength = FsciParameter("DataLength", 1)
        cmdParams.append(DataLength)
        Data = FsciParameter("Data", 1, DataLength)
        cmdParams.append(Data)
        return FsciFrameDescription(0x3C, 0x30, cmdParams)

    def InitStatus(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        SeqNum = FsciParameter("SeqNum", 1)
        cmdParams.append(SeqNum)
        PacketType = FsciParameter("PacketType", 2)
        cmdParams.append(PacketType)
        return FsciFrameDescription(0xB7, 0x00, cmdParams)

    def InitLogMessage(self):
        cmdParams = []
        LogLevel = FsciParameter("LogLevel", 1)
        cmdParams.append(LogLevel)
        LogMessage = FsciParameter("LogMessage", 1)
        cmdParams.append(LogMessage)
        return FsciFrameDescription(0xB7, 0x01, cmdParams)

    def InitDataIndication(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        ProfileID = FsciParameter("ProfileID", 2)
        cmdParams.append(ProfileID)
        ClusterID = FsciParameter("ClusterID", 2)
        cmdParams.append(ClusterID)
        SourceEndpoint = FsciParameter("SourceEndpoint", 1)
        cmdParams.append(SourceEndpoint)
        DestinationEndpoint = FsciParameter("DestinationEndpoint", 1)
        cmdParams.append(DestinationEndpoint)
        SourceAddressMode = FsciParameter("SourceAddressMode", 1)
        cmdParams.append(SourceAddressMode)
        SourceAddressdict = {}
        currentList = []
        SourceAddressNO_PAN = FsciParameter("SourceAddressNO_PAN", 1)
        currentList.append(SourceAddressNO_PAN)
        SourceAddressdict[0x00] = currentList
        currentList = []
        SourceAddressReserved = FsciParameter("SourceAddressReserved", 1)
        currentList.append(SourceAddressReserved)
        SourceAddressdict[0x01] = currentList
        currentList = []
        SourceAddressAddress_16b = FsciParameter("SourceAddressAddress_16b", 2)
        currentList.append(SourceAddressAddress_16b)
        SourceAddressdict[0x02] = currentList
        currentList = []
        SourceAddressAddress_64b = FsciParameter("SourceAddressAddress_64b", 8)
        currentList.append(SourceAddressAddress_64b)
        SourceAddressdict[0x03] = currentList
        SourceAddress = FsciParameter("SourceAddress", 1, SourceAddressMode, SourceAddressdict)
        cmdParams.append(SourceAddress)
        DestAddressingMode = FsciParameter("DestAddressingMode", 1)
        cmdParams.append(DestAddressingMode)
        DestinationAddressdict = {}
        currentList = []
        DestinationAddressNO_PAN = FsciParameter("DestinationAddressNO_PAN", 1)
        currentList.append(DestinationAddressNO_PAN)
        DestinationAddressdict[0x00] = currentList
        currentList = []
        DestinationAddressReserved = FsciParameter("DestinationAddressReserved", 1)
        currentList.append(DestinationAddressReserved)
        DestinationAddressdict[0x01] = currentList
        currentList = []
        DestinationAddressAddress_16b = FsciParameter("DestinationAddressAddress_16b", 2)
        currentList.append(DestinationAddressAddress_16b)
        DestinationAddressdict[0x02] = currentList
        currentList = []
        DestinationAddressAddress_64b = FsciParameter("DestinationAddressAddress_64b", 8)
        currentList.append(DestinationAddressAddress_64b)
        DestinationAddressdict[0x03] = currentList
        DestinationAddress = FsciParameter("DestinationAddress", 1, DestAddressingMode, DestinationAddressdict)
        cmdParams.append(DestinationAddress)
        PayloadSize = FsciParameter("PayloadSize", 1)
        cmdParams.append(PayloadSize)
        Payload = FsciParameter("Payload", 1, PayloadSize)
        cmdParams.append(Payload)
        return FsciFrameDescription(0xB7, 0x02, cmdParams)

    def InitNodeClusterList(self):
        cmdParams = []
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        ProfileID = FsciParameter("ProfileID", 2)
        cmdParams.append(ProfileID)
        ClusterList = FsciParameter("ClusterList", 2)
        cmdParams.append(ClusterList)
        return FsciFrameDescription(0xB7, 0x03, cmdParams)

    def InitNodeClusterAttributeList(self):
        cmdParams = []
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        ProfileID = FsciParameter("ProfileID", 2)
        cmdParams.append(ProfileID)
        ClusterID = FsciParameter("ClusterID", 2)
        cmdParams.append(ClusterID)
        AttributeList = FsciParameter("AttributeList", 2)
        cmdParams.append(AttributeList)
        return FsciFrameDescription(0xB7, 0x04, cmdParams)

    def InitNodeCommandIDList(self):
        cmdParams = []
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        ProfileID = FsciParameter("ProfileID", 2)
        cmdParams.append(ProfileID)
        ClusterID = FsciParameter("ClusterID", 2)
        cmdParams.append(ClusterID)
        CommandIDList = FsciParameter("CommandIDList", 1)
        cmdParams.append(CommandIDList)
        return FsciFrameDescription(0xB7, 0x05, cmdParams)

    def InitVersionList(self):
        cmdParams = []
        MajorVersionNumber = FsciParameter("MajorVersionNumber", 2)
        cmdParams.append(MajorVersionNumber)
        InstallerVersionNumber = FsciParameter("InstallerVersionNumber", 2)
        cmdParams.append(InstallerVersionNumber)
        return FsciFrameDescription(0xB7, 0x10, cmdParams)

    def InitNetworkJoinedFormed(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        ShortAddr = FsciParameter("ShortAddr", 2)
        cmdParams.append(ShortAddr)
        ExtAddr = FsciParameter("ExtAddr", 8)
        cmdParams.append(ExtAddr)
        Channel = FsciParameter("Channel", 1)
        cmdParams.append(Channel)
        return FsciFrameDescription(0xB7, 0x24, cmdParams)

    def InitPermitJoinStatusResponse(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xB7, 0x14, cmdParams)

    def InitNonFactoryNewRestart(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xB7, 0x06, cmdParams)

    def InitFactoryNewRestart(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xB7, 0x07, cmdParams)

    def InitBindResponse(self):
        cmdParams = []
        SequenceNumber = FsciParameter("SequenceNumber", 1)
        cmdParams.append(SequenceNumber)
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xB7, 0x30, cmdParams)

    def InitUnbindResponse(self):
        cmdParams = []
        SequenceNumber = FsciParameter("SequenceNumber", 1)
        cmdParams.append(SequenceNumber)
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xB7, 0x31, cmdParams)

    def InitNetworkAddressResponse(self):
        cmdParams = []
        SequenceNumber = FsciParameter("SequenceNumber", 1)
        cmdParams.append(SequenceNumber)
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        IEEEAddress = FsciParameter("IEEEAddress", 8)
        cmdParams.append(IEEEAddress)
        ShortAddress = FsciParameter("ShortAddress", 1)
        cmdParams.append(ShortAddress)
        NumberOfAssociatedDevices = FsciParameter("NumberOfAssociatedDevices", 1)
        cmdParams.append(NumberOfAssociatedDevices)
        StartIndex = FsciParameter("StartIndex", 1)
        cmdParams.append(StartIndex)
        DeviceList = FsciParameter("DeviceList", 2)
        cmdParams.append(DeviceList)
        return FsciFrameDescription(0xB7, 0x40, cmdParams)

    def InitIEEEAddressResponse(self):
        cmdParams = []
        SequenceNumber = FsciParameter("SequenceNumber", 1)
        cmdParams.append(SequenceNumber)
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        IEEEAddress = FsciParameter("IEEEAddress", 8)
        cmdParams.append(IEEEAddress)
        shortAddress = FsciParameter("shortAddress", 2)
        cmdParams.append(shortAddress)
        NumberOfAssociatedDevices = FsciParameter("NumberOfAssociatedDevices", 1)
        cmdParams.append(NumberOfAssociatedDevices)
        StartIndex = FsciParameter("StartIndex", 1)
        cmdParams.append(StartIndex)
        DeviceList = FsciParameter("DeviceList", 2)
        cmdParams.append(DeviceList)
        return FsciFrameDescription(0xB7, 0x41, cmdParams)

    def InitSimpleDescriptorResponse(self):
        cmdParams = []
        SequenceNumber = FsciParameter("SequenceNumber", 1)
        cmdParams.append(SequenceNumber)
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        NwkAddress = FsciParameter("NwkAddress", 2)
        cmdParams.append(NwkAddress)
        Length = FsciParameter("Length", 1)
        cmdParams.append(Length)
        EndPoint = FsciParameter("EndPoint", 1)
        cmdParams.append(EndPoint)
        Profile = FsciParameter("Profile", 2)
        cmdParams.append(Profile)
        DeviceID = FsciParameter("DeviceID", 2)
        cmdParams.append(DeviceID)
        BitFields = FsciParameter("BitFields", 1)
        cmdParams.append(BitFields)
        InClusterCount = FsciParameter("InClusterCount", 1)
        cmdParams.append(InClusterCount)
        InClusterList = FsciParameter("InClusterList", 2, InClusterCount)
        cmdParams.append(InClusterList)
        OutClusterCount = FsciParameter("OutClusterCount", 1)
        cmdParams.append(OutClusterCount)
        OutClusterList = FsciParameter("OutClusterList", 2, OutClusterCount)
        cmdParams.append(OutClusterList)
        return FsciFrameDescription(0xB7, 0x43, cmdParams)

    def InitPowerDescriptorResponse(self):
        cmdParams = []
        SequenceNumber = FsciParameter("SequenceNumber", 1)
        cmdParams.append(SequenceNumber)
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        BitFields = FsciParameter("BitFields", 2)
        cmdParams.append(BitFields)
        return FsciFrameDescription(0xB7, 0x44, cmdParams)

    def InitActiveEndpointResponse(self):
        cmdParams = []
        SequenceNumber = FsciParameter("SequenceNumber", 1)
        cmdParams.append(SequenceNumber)
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        Address = FsciParameter("Address", 2)
        cmdParams.append(Address)
        EndpointCount = FsciParameter("EndpointCount", 1)
        cmdParams.append(EndpointCount)
        ActiveEndPointList = FsciParameter("ActiveEndPointList", 1, EndpointCount)
        cmdParams.append(ActiveEndPointList)
        return FsciFrameDescription(0xB7, 0x45, cmdParams)

    def InitMatchDescriptorResponse(self):
        cmdParams = []
        SequenceNumber = FsciParameter("SequenceNumber", 1)
        cmdParams.append(SequenceNumber)
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        NetworkAddress = FsciParameter("NetworkAddress", 2)
        cmdParams.append(NetworkAddress)
        LengthOfList = FsciParameter("LengthOfList", 1)
        cmdParams.append(LengthOfList)
        MatchList = FsciParameter("MatchList", 1, LengthOfList)
        cmdParams.append(MatchList)
        return FsciFrameDescription(0xB7, 0x46, cmdParams)

    def InitDeviceAnnounce(self):
        cmdParams = []
        ShortAddress = FsciParameter("ShortAddress", 2)
        cmdParams.append(ShortAddress)
        IEEEAddress = FsciParameter("IEEEAddress", 8)
        cmdParams.append(IEEEAddress)
        MACCapability = FsciParameter("MACCapability", 1)
        cmdParams.append(MACCapability)
        return FsciFrameDescription(0x37, 0x4D, cmdParams)

    def InitUserDescriptorResponse(self):
        cmdParams = []
        SequenceNumber = FsciParameter("SequenceNumber", 1)
        cmdParams.append(SequenceNumber)
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        NetworkAddressOfOnterest = FsciParameter("NetworkAddressOfOnterest", 2)
        cmdParams.append(NetworkAddressOfOnterest)
        LengthOfData = FsciParameter("LengthOfData", 1)
        cmdParams.append(LengthOfData)
        DataList = FsciParameter("DataList", 1, LengthOfData)
        cmdParams.append(DataList)
        return FsciFrameDescription(0xB7, 0x2C, cmdParams)

    def InitUserDescriptorNotify(self):
        cmdParams = []
        SequenceNumber = FsciParameter("SequenceNumber", 1)
        cmdParams.append(SequenceNumber)
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        NetworkAddressOfOnterest = FsciParameter("NetworkAddressOfOnterest", 2)
        cmdParams.append(NetworkAddressOfOnterest)
        return FsciFrameDescription(0xB7, 0x2B, cmdParams)

    def InitComplexDescriptorResponse(self):
        cmdParams = []
        SequenceNumber = FsciParameter("SequenceNumber", 1)
        cmdParams.append(SequenceNumber)
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        NetworkAddressOfOnterest = FsciParameter("NetworkAddressOfOnterest", 2)
        cmdParams.append(NetworkAddressOfOnterest)
        Length = FsciParameter("Length", 1)
        cmdParams.append(Length)
        XMLTag = FsciParameter("XMLTag", 1)
        cmdParams.append(XMLTag)
        FieldCount = FsciParameter("FieldCount", 1)
        cmdParams.append(FieldCount)
        FieldValues = FsciParameter("FieldValues", 1, FieldCount)
        cmdParams.append(FieldValues)
        return FsciFrameDescription(0xB7, 0x34, cmdParams)

    def InitManagementLeaveResponse(self):
        cmdParams = []
        SequenceNumber = FsciParameter("SequenceNumber", 1)
        cmdParams.append(SequenceNumber)
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xB7, 0x47, cmdParams)

    def InitLeaveIndication(self):
        cmdParams = []
        ExtendedAddress = FsciParameter("ExtendedAddress", 8)
        cmdParams.append(ExtendedAddress)
        RejoinStatus = FsciParameter("RejoinStatus", 1)
        cmdParams.append(RejoinStatus)
        return FsciFrameDescription(0xB7, 0x48, cmdParams)

    def InitManagementNetworkUpdateResponse(self):
        cmdParams = []
        SequenceNumber = FsciParameter("SequenceNumber", 1)
        cmdParams.append(SequenceNumber)
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        TotalTransmission = FsciParameter("TotalTransmission", 2)
        cmdParams.append(TotalTransmission)
        TransmissionFailures = FsciParameter("TransmissionFailures", 2)
        cmdParams.append(TransmissionFailures)
        ScannedChannels = FsciParameter("ScannedChannels", 4)
        cmdParams.append(ScannedChannels)
        ScannedChannelListCount = FsciParameter("ScannedChannelListCount", 1)
        cmdParams.append(ScannedChannelListCount)
        ChannelList = FsciParameter("ChannelList", 1)
        cmdParams.append(ChannelList)
        return FsciFrameDescription(0xB7, 0x4A, cmdParams)

    def InitSystemServerDiscoveryResponse(self):
        cmdParams = []
        SequenceNumber = FsciParameter("SequenceNumber", 1)
        cmdParams.append(SequenceNumber)
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        ServerMask = FsciParameter("ServerMask", 2)
        cmdParams.append(ServerMask)
        return FsciFrameDescription(0xB7, 0x4B, cmdParams)

    def InitAuthenticateResponse(self):
        cmdParams = []
        IEEEAddressOfTtheGateway = FsciParameter("IEEEAddressOfTtheGateway", 8)
        cmdParams.append(IEEEAddressOfTtheGateway)
        EncryptedKey = FsciParameter("EncryptedKey", 2)
        cmdParams.append(EncryptedKey)
        MIC = FsciParameter("MIC", 1)
        cmdParams.append(MIC)
        IEEEAddressOfTheInitiatingNode = FsciParameter("IEEEAddressOfTheInitiatingNode", 8)
        cmdParams.append(IEEEAddressOfTheInitiatingNode)
        ActiveKeySequenceNumber = FsciParameter("ActiveKeySequenceNumber", 1)
        cmdParams.append(ActiveKeySequenceNumber)
        Channel = FsciParameter("Channel", 1)
        cmdParams.append(Channel)
        ShortPANID = FsciParameter("ShortPANID", 2)
        cmdParams.append(ShortPANID)
        ExtendedPANID = FsciParameter("ExtendedPANID", 8)
        cmdParams.append(ExtendedPANID)
        return FsciFrameDescription(0xB7, 0x28, cmdParams)

    def InitConfigureReportingResponse(self):
        cmdParams = []
        SequenceNumber = FsciParameter("SequenceNumber", 1)
        cmdParams.append(SequenceNumber)
        SrcAddress = FsciParameter("SrcAddress", 2)
        cmdParams.append(SrcAddress)
        Endpoint = FsciParameter("Endpoint", 1)
        cmdParams.append(Endpoint)
        ClusterID = FsciParameter("ClusterID", 2)
        cmdParams.append(ClusterID)
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xB7, 0x00, cmdParams)

    def InitReadIndividualAttributeResponse(self):
        cmdParams = []
        SequenceNumber = FsciParameter("SequenceNumber", 1)
        cmdParams.append(SequenceNumber)
        SrcAddress = FsciParameter("SrcAddress", 2)
        cmdParams.append(SrcAddress)
        Endpoint = FsciParameter("Endpoint", 1)
        cmdParams.append(Endpoint)
        ClusterID = FsciParameter("ClusterID", 2)
        cmdParams.append(ClusterID)
        AttributeEnum = FsciParameter("AttributeEnum", 2)
        cmdParams.append(AttributeEnum)
        AttributeStatus = FsciParameter("AttributeStatus", 1)
        cmdParams.append(AttributeStatus)
        AttributeDataType = FsciParameter("AttributeDataType", 1)
        cmdParams.append(AttributeDataType)
        SizeOfTheAttributes = FsciParameter("SizeOfTheAttributes", 2)
        cmdParams.append(SizeOfTheAttributes)
        DataByteList = FsciParameter("DataByteList", 1, SizeOfTheAttributes)
        cmdParams.append(DataByteList)
        return FsciFrameDescription(0xB8, 0x00, cmdParams)

    def InitWriteAttributeResponse(self):
        cmdParams = []
        SequenceNumber = FsciParameter("SequenceNumber", 1)
        cmdParams.append(SequenceNumber)
        SrcAddress = FsciParameter("SrcAddress", 2)
        cmdParams.append(SrcAddress)
        Endpoint = FsciParameter("Endpoint", 1)
        cmdParams.append(Endpoint)
        ClusterID = FsciParameter("ClusterID", 2)
        cmdParams.append(ClusterID)
        AttributeEnum = FsciParameter("AttributeEnum", 2)
        cmdParams.append(AttributeEnum)
        AttributeStatus = FsciParameter("AttributeStatus", 1)
        cmdParams.append(AttributeStatus)
        AttributeDataType = FsciParameter("AttributeDataType", 1)
        cmdParams.append(AttributeDataType)
        SizeOfTheAttributes = FsciParameter("SizeOfTheAttributes", 2)
        cmdParams.append(SizeOfTheAttributes)
        DataByteList = FsciParameter("DataByteList", 1, SizeOfTheAttributes)
        cmdParams.append(DataByteList)
        return FsciFrameDescription(0xB8, 0x10, cmdParams)

    def InitReportIndividualAttributeResponse(self):
        cmdParams = []
        SequenceNumber = FsciParameter("SequenceNumber", 1)
        cmdParams.append(SequenceNumber)
        SrcAddress = FsciParameter("SrcAddress", 2)
        cmdParams.append(SrcAddress)
        Endpoint = FsciParameter("Endpoint", 1)
        cmdParams.append(Endpoint)
        ClusterID = FsciParameter("ClusterID", 2)
        cmdParams.append(ClusterID)
        AttributeEnum = FsciParameter("AttributeEnum", 2)
        cmdParams.append(AttributeEnum)
        AttributeStatus = FsciParameter("AttributeStatus", 1)
        cmdParams.append(AttributeStatus)
        AttributeDataType = FsciParameter("AttributeDataType", 1)
        cmdParams.append(AttributeDataType)
        SizeOfTheAttributes = FsciParameter("SizeOfTheAttributes", 2)
        cmdParams.append(SizeOfTheAttributes)
        DataByteList = FsciParameter("DataByteList", 1, SizeOfTheAttributes)
        cmdParams.append(DataByteList)
        return FsciFrameDescription(0xB8, 0x02, cmdParams)

    def InitDefaultResponse(self):
        cmdParams = []
        SequenceNumber = FsciParameter("SequenceNumber", 1)
        cmdParams.append(SequenceNumber)
        Endpoint = FsciParameter("Endpoint", 1)
        cmdParams.append(Endpoint)
        ClusterID = FsciParameter("ClusterID", 2)
        cmdParams.append(ClusterID)
        CommandID = FsciParameter("CommandID", 1)
        cmdParams.append(CommandID)
        StatusCode = FsciParameter("StatusCode", 1)
        cmdParams.append(StatusCode)
        return FsciFrameDescription(0xB8, 0x01, cmdParams)

    def InitAddGroupResponse(self):
        cmdParams = []
        SequenceNumber = FsciParameter("SequenceNumber", 1)
        cmdParams.append(SequenceNumber)
        Endpoint = FsciParameter("Endpoint", 1)
        cmdParams.append(Endpoint)
        ClusterID = FsciParameter("ClusterID", 2)
        cmdParams.append(ClusterID)
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        GroupID = FsciParameter("GroupID", 2)
        cmdParams.append(GroupID)
        return FsciFrameDescription(0xB7, 0x60, cmdParams)

    def InitViewGroupResponse(self):
        cmdParams = []
        SequenceNumber = FsciParameter("SequenceNumber", 1)
        cmdParams.append(SequenceNumber)
        Endpoint = FsciParameter("Endpoint", 1)
        cmdParams.append(Endpoint)
        ClusterID = FsciParameter("ClusterID", 2)
        cmdParams.append(ClusterID)
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        GroupID = FsciParameter("GroupID", 2)
        cmdParams.append(GroupID)
        return FsciFrameDescription(0xB7, 0x61, cmdParams)

    def InitGetGroupMembershipResponse(self):
        cmdParams = []
        SequenceNumber = FsciParameter("SequenceNumber", 1)
        cmdParams.append(SequenceNumber)
        Endpoint = FsciParameter("Endpoint", 1)
        cmdParams.append(Endpoint)
        ClusterID = FsciParameter("ClusterID", 2)
        cmdParams.append(ClusterID)
        Capacity = FsciParameter("Capacity", 1)
        cmdParams.append(Capacity)
        GroupCount = FsciParameter("GroupCount", 1)
        cmdParams.append(GroupCount)
        GroupList = FsciParameter("GroupList", 2, GroupCount)
        cmdParams.append(GroupList)
        return FsciFrameDescription(0xB7, 0x62, cmdParams)

    def InitRemoveGroupResponse(self):
        cmdParams = []
        SequenceNumber = FsciParameter("SequenceNumber", 1)
        cmdParams.append(SequenceNumber)
        Endpoint = FsciParameter("Endpoint", 1)
        cmdParams.append(Endpoint)
        ClusterID = FsciParameter("ClusterID", 2)
        cmdParams.append(ClusterID)
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        GroupID = FsciParameter("GroupID", 2)
        cmdParams.append(GroupID)
        return FsciFrameDescription(0xB7, 0x63, cmdParams)

    def InitViewSceneResponse(self):
        cmdParams = []
        SequenceNumber = FsciParameter("SequenceNumber", 1)
        cmdParams.append(SequenceNumber)
        Endpoint = FsciParameter("Endpoint", 1)
        cmdParams.append(Endpoint)
        ClusterID = FsciParameter("ClusterID", 2)
        cmdParams.append(ClusterID)
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        GroupID = FsciParameter("GroupID", 2)
        cmdParams.append(GroupID)
        SceneID = FsciParameter("SceneID", 1)
        cmdParams.append(SceneID)
        TransitionTime = FsciParameter("TransitionTime", 2)
        cmdParams.append(TransitionTime)
        SceneNameLength = FsciParameter("SceneNameLength", 1)
        cmdParams.append(SceneNameLength)
        SceneNameMaxLength = FsciParameter("SceneNameMaxLength", 1)
        cmdParams.append(SceneNameMaxLength)
        SceneNameData = FsciParameter("SceneNameData", 1)
        cmdParams.append(SceneNameData)
        ExtensionsLength = FsciParameter("ExtensionsLength", 2)
        cmdParams.append(ExtensionsLength)
        ExtensionsMaxLength = FsciParameter("ExtensionsMaxLength", 2)
        cmdParams.append(ExtensionsMaxLength)
        ExtensionsData = FsciParameter("ExtensionsData", 1, ExtensionsMaxLength)
        cmdParams.append(ExtensionsData)
        return FsciFrameDescription(0xB7, 0xA0, cmdParams)

    def InitAddSceneResponse(self):
        cmdParams = []
        SequenceNumber = FsciParameter("SequenceNumber", 1)
        cmdParams.append(SequenceNumber)
        Endpoint = FsciParameter("Endpoint", 1)
        cmdParams.append(Endpoint)
        ClusterID = FsciParameter("ClusterID", 2)
        cmdParams.append(ClusterID)
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        GroupID = FsciParameter("GroupID", 2)
        cmdParams.append(GroupID)
        SceneID = FsciParameter("SceneID", 1)
        cmdParams.append(SceneID)
        return FsciFrameDescription(0xB7, 0xA1, cmdParams)

    def InitRemoveSceneResponse(self):
        cmdParams = []
        SequenceNumber = FsciParameter("SequenceNumber", 1)
        cmdParams.append(SequenceNumber)
        Endpoint = FsciParameter("Endpoint", 1)
        cmdParams.append(Endpoint)
        ClusterID = FsciParameter("ClusterID", 2)
        cmdParams.append(ClusterID)
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        GroupID = FsciParameter("GroupID", 2)
        cmdParams.append(GroupID)
        SceneID = FsciParameter("SceneID", 1)
        cmdParams.append(SceneID)
        return FsciFrameDescription(0xB7, 0xA2, cmdParams)

    def InitRemoveAllSceneResponse(self):
        cmdParams = []
        SequenceNumber = FsciParameter("SequenceNumber", 1)
        cmdParams.append(SequenceNumber)
        Endpoint = FsciParameter("Endpoint", 1)
        cmdParams.append(Endpoint)
        ClusterID = FsciParameter("ClusterID", 2)
        cmdParams.append(ClusterID)
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        GroupID = FsciParameter("GroupID", 2)
        cmdParams.append(GroupID)
        return FsciFrameDescription(0xB7, 0xA3, cmdParams)

    def InitAddEnhancedSceneResponse(self):
        cmdParams = []
        SeqNum = FsciParameter("SeqNum", 1)
        cmdParams.append(SeqNum)
        srcEndpoint = FsciParameter("srcEndpoint", 1)
        cmdParams.append(srcEndpoint)
        ClusterId = FsciParameter("ClusterId", 2)
        cmdParams.append(ClusterId)
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        GroupID = FsciParameter("GroupID", 2)
        cmdParams.append(GroupID)
        SceneID = FsciParameter("SceneID", 1)
        cmdParams.append(SceneID)
        return FsciFrameDescription(0xB7, 0xA7, cmdParams)

    def InitStoreSceneResponse(self):
        cmdParams = []
        SequenceNumber = FsciParameter("SequenceNumber", 1)
        cmdParams.append(SequenceNumber)
        Endpoint = FsciParameter("Endpoint", 1)
        cmdParams.append(Endpoint)
        ClusterID = FsciParameter("ClusterID", 2)
        cmdParams.append(ClusterID)
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        GroupID = FsciParameter("GroupID", 2)
        cmdParams.append(GroupID)
        SceneID = FsciParameter("SceneID", 1)
        cmdParams.append(SceneID)
        return FsciFrameDescription(0xB7, 0xA4, cmdParams)

    def InitSceneMembershipResponse(self):
        cmdParams = []
        SequenceNumber = FsciParameter("SequenceNumber", 1)
        cmdParams.append(SequenceNumber)
        Endpoint = FsciParameter("Endpoint", 1)
        cmdParams.append(Endpoint)
        ClusterID = FsciParameter("ClusterID", 2)
        cmdParams.append(ClusterID)
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        Capacity = FsciParameter("Capacity", 1)
        cmdParams.append(Capacity)
        GroupID = FsciParameter("GroupID", 2)
        cmdParams.append(GroupID)
        SceneCount = FsciParameter("SceneCount", 1)
        cmdParams.append(SceneCount)
        SceneList = FsciParameter("SceneList", 1, SceneCount)
        cmdParams.append(SceneList)
        return FsciFrameDescription(0xB7, 0xA6, cmdParams)

    def InitTouchlinkStatus(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        JoinedNodeShortAddress = FsciParameter("JoinedNodeShortAddress", 2)
        cmdParams.append(JoinedNodeShortAddress)
        return FsciFrameDescription(0xB7, 0xA6, cmdParams)

    def InitZoneStatusChangeNotification(self):
        cmdParams = []
        SequenceNumber = FsciParameter("SequenceNumber", 1)
        cmdParams.append(SequenceNumber)
        EndPoint = FsciParameter("EndPoint", 1)
        cmdParams.append(EndPoint)
        ClusteID = FsciParameter("ClusteID", 2)
        cmdParams.append(ClusteID)
        SourceAddressMode = FsciParameter("SourceAddressMode", 1)
        cmdParams.append(SourceAddressMode)
        SourceAddressdict = {}
        currentList = []
        SourceAddressNO_PAN = FsciParameter("SourceAddressNO_PAN", 1)
        currentList.append(SourceAddressNO_PAN)
        SourceAddressdict[0x00] = currentList
        currentList = []
        SourceAddressReserved = FsciParameter("SourceAddressReserved", 1)
        currentList.append(SourceAddressReserved)
        SourceAddressdict[0x01] = currentList
        currentList = []
        SourceAddressAddress_16b = FsciParameter("SourceAddressAddress_16b", 2)
        currentList.append(SourceAddressAddress_16b)
        SourceAddressdict[0x02] = currentList
        currentList = []
        SourceAddressAddress_64b = FsciParameter("SourceAddressAddress_64b", 8)
        currentList.append(SourceAddressAddress_64b)
        SourceAddressdict[0x03] = currentList
        SourceAddress = FsciParameter("SourceAddress", 1, SourceAddressMode, SourceAddressdict)
        cmdParams.append(SourceAddress)
        ZoneStatus = FsciParameter("ZoneStatus", 2)
        cmdParams.append(ZoneStatus)
        ExtendedStatus = FsciParameter("ExtendedStatus", 1)
        cmdParams.append(ExtendedStatus)
        ZoneID = FsciParameter("ZoneID", 1)
        cmdParams.append(ZoneID)
        Delay = FsciParameter("Delay", 2)
        cmdParams.append(Delay)
        return FsciFrameDescription(0xBB, 0x01, cmdParams)

    def InitHostPersistentDataManagerAvailableResponse(self):
        cmdParams = []
        return FsciFrameDescription(0xBA, 0x00, cmdParams)

    def InitLoadRecordResponse(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        Record = FsciParameter("Record", 2)
        cmdParams.append(Record)
        TotalSize = FsciParameter("TotalSize", 4)
        cmdParams.append(TotalSize)
        TotalNumberOfBlocks = FsciParameter("TotalNumberOfBlocks", 4)
        cmdParams.append(TotalNumberOfBlocks)
        CurrentBlock = FsciParameter("CurrentBlock", 4)
        cmdParams.append(CurrentBlock)
        BlockSize = FsciParameter("BlockSize", 4)
        cmdParams.append(BlockSize)
        Parameters = FsciParameter("Parameters", 1, BlockSize)
        cmdParams.append(Parameters)
        return FsciFrameDescription(0xB9, 0x01, cmdParams)

    def InitSaveRecordResponse(self):
        cmdParams = []
        Record = FsciParameter("Record", 2)
        cmdParams.append(Record)
        TotalSize = FsciParameter("TotalSize", 4)
        cmdParams.append(TotalSize)
        TotalNumberOfBlocks = FsciParameter("TotalNumberOfBlocks", 4)
        cmdParams.append(TotalNumberOfBlocks)
        CurrentBlock = FsciParameter("CurrentBlock", 4)
        cmdParams.append(CurrentBlock)
        BlockSize = FsciParameter("BlockSize", 4)
        cmdParams.append(BlockSize)
        return FsciFrameDescription(0xB9, 0x00, cmdParams)

    def InitRouterDiscoveryConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        NwkStatus = FsciParameter("NwkStatus", 1)
        cmdParams.append(NwkStatus)
        return FsciFrameDescription(0xBE, 0x01, cmdParams)

    def InitAPSDataConfirmFail(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        SourceEndPoint = FsciParameter("SourceEndPoint", 1)
        cmdParams.append(SourceEndPoint)
        DestinationEndPoint = FsciParameter("DestinationEndPoint", 1)
        cmdParams.append(DestinationEndPoint)
        DestAddressingMode = FsciParameter("DestAddressingMode", 1)
        cmdParams.append(DestAddressingMode)
        DestinationAddressdict = {}
        currentList = []
        DestinationAddressNO_PAN = FsciParameter("DestinationAddressNO_PAN", 1)
        currentList.append(DestinationAddressNO_PAN)
        DestinationAddressdict[0x00] = currentList
        currentList = []
        DestinationAddressReserved = FsciParameter("DestinationAddressReserved", 1)
        currentList.append(DestinationAddressReserved)
        DestinationAddressdict[0x01] = currentList
        currentList = []
        DestinationAddressAddress_16b = FsciParameter("DestinationAddressAddress_16b", 2)
        currentList.append(DestinationAddressAddress_16b)
        DestinationAddressdict[0x02] = currentList
        currentList = []
        DestinationAddressAddress_64b = FsciParameter("DestinationAddressAddress_64b", 8)
        currentList.append(DestinationAddressAddress_64b)
        DestinationAddressdict[0x03] = currentList
        DestinationAddress = FsciParameter("DestinationAddress", 1, DestAddressingMode, DestinationAddressdict)
        cmdParams.append(DestinationAddress)
        SequenceNumber = FsciParameter("SequenceNumber", 1)
        cmdParams.append(SequenceNumber)
        return FsciFrameDescription(0xBE, 0x02, cmdParams)
