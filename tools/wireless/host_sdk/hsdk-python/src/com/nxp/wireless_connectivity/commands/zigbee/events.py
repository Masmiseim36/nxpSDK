'''
* Copyright 2017-2018 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
'''

from ctypes import cast, c_uint8, c_void_p, POINTER
import inspect
import sys

from com.nxp.wireless_connectivity.commands.zigbee.frames import *  # @UnusedWildImport
from com.nxp.wireless_connectivity.commands.zigbee.spec import Spec as _Spec
from com.nxp.wireless_connectivity.hsdk.CFsciLibrary import FsciFrame
from com.nxp.wireless_connectivity.hsdk.library_loader import LibraryLoader
from com.nxp.wireless_connectivity.hsdk.utils import Observer, overrides, print_event


fsciLibrary = LibraryLoader().CFsciLibrary
fsciLibrary.DestroyFSCIFrame.argtypes = [c_void_p]
Spec = _Spec()


class GetVersionObserver(Observer):

    opGroup = Spec.GetVersionFrame.opGroup
    opCode = Spec.GetVersionFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Create frame object
        frame = GetVersion()
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SetExtendedPANIDObserver(Observer):

    opGroup = Spec.SetExtendedPANIDFrame.opGroup
    opCode = Spec.SetExtendedPANIDFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SetExtendedPANIDFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SetExtendedPANID()
        frame.ExtendedPANID = packet.getParamValueAsNumber("ExtendedPANID")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SetChannelMaskObserver(Observer):

    opGroup = Spec.SetChannelMaskFrame.opGroup
    opCode = Spec.SetChannelMaskFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SetChannelMaskFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SetChannelMask()
        frame.ChannelMask = packet.getParamValueAsNumber("ChannelMask")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SetSecurityStateKeyObserver(Observer):

    opGroup = Spec.SetSecurityStateKeyFrame.opGroup
    opCode = Spec.SetSecurityStateKeyFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SetSecurityStateKeyFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SetSecurityStateKey()
        frame.KeyType = packet.getParamValueAsNumber("KeyType")
        frame.KeyData = packet.getParamValueAsNumber("KeyData")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SetDeviceTypeObserver(Observer):

    opGroup = Spec.SetDeviceTypeFrame.opGroup
    opCode = Spec.SetDeviceTypeFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SetDeviceTypeFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SetDeviceType()
        frame.DeviceType = packet.getParamValueAsNumber("DeviceType")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class StartNetworkMessageObserver(Observer):

    opGroup = Spec.StartNetworkMessageFrame.opGroup
    opCode = Spec.StartNetworkMessageFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Create frame object
        frame = StartNetworkMessage()
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class StartNetworkScanObserver(Observer):

    opGroup = Spec.StartNetworkScanFrame.opGroup
    opCode = Spec.StartNetworkScanFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Create frame object
        frame = StartNetworkScan()
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)

class CodeObserver(Observer):

    opGroup = Spec.CodeFrame.opGroup
    opCode = Spec.CodeFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.CodeFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = Code()
        frame.TargetExtendedAddress = packet.getParamValueAsNumber("TargetExtendedAddress")
        frame.Code = packet.getParamValueAsNumber("Code")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)



class ZLO_ZLLFactoryNewResetObserver(Observer):

    opGroup = Spec.ZLO_ZLLFactoryNewResetFrame.opGroup
    opCode = Spec.ZLO_ZLLFactoryNewResetFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Create frame object
        frame = ZLO_ZLLFactoryNewReset()
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class PermitJoinStatusOnTheTargetObserver(Observer):

    opGroup = Spec.PermitJoinStatusOnTheTargetFrame.opGroup
    opCode = Spec.PermitJoinStatusOnTheTargetFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        frame = PermitJoinStatusOnTheTarget()
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ResetObserver(Observer):

    opGroup = Spec.ResetFrame.opGroup
    opCode = Spec.ResetFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        frame = Reset()
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ErasePersistentDataObserver(Observer):

    opGroup = Spec.ErasePersistentDataFrame.opGroup
    opCode = Spec.ErasePersistentDataFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        frame = ErasePersistentData()
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class BindObserver(Observer):

    opGroup = Spec.BindFrame.opGroup
    opCode = Spec.BindFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.BindFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = Bind()
        frame.TargetExtendedAddress = packet.getParamValueAsNumber("TargetExtendedAddress")
        frame.TargetEndPoint = packet.getParamValueAsNumber("TargetEndPoint")
        frame.ClusterID = packet.getParamValueAsNumber("ClusterID")
        frame.DestAddressingMode = packet.getParamValueAsNumber("DestAddressingMode")
        frame.DestinationAddress = packet.getParamValueAsNumber("DestinationAddress")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class UnbindObserver(Observer):

    opGroup = Spec.UnbindFrame.opGroup
    opCode = Spec.UnbindFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.UnbindFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = Unbind()
        frame.TargetExtendedAddress = packet.getParamValueAsNumber("TargetExtendedAddress")
        frame.TargetEndPoint = packet.getParamValueAsNumber("TargetEndPoint")
        frame.ClusterID = packet.getParamValueAsNumber("ClusterID")
        frame.DestAddressingMode = packet.getParamValueAsNumber("DestAddressingMode")
        frame.DestinationAddress = packet.getParamValueAsNumber("DestinationAddress")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class RemoveDeviceObserver(Observer):

    opGroup = Spec.RemoveDeviceFrame.opGroup
    opCode = Spec.RemoveDeviceFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.RemoveDeviceFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = RemoveDevice()
        frame.ParentExtendedAddress = packet.getParamValueAsNumber("ParentExtendedAddress")
        frame.ChildExtendedAddress = packet.getParamValueAsNumber("ChildExtendedAddress")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class UserDescriptorSetObserver(Observer):

    opGroup = Spec.UserDescriptorSetFrame.opGroup
    opCode = Spec.UserDescriptorSetFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.UserDescriptorSetFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = UserDescriptorSet()
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.AddressOfInterest = packet.getParamValueAsNumber("AddressOfInterest")
        frame.StringLength = packet.getParamValueAsNumber("StringLength")
        frame.Parameters = packet.getParamValueAsNumber("Parameters")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class EnablePermissionsControlledJoinsObserver(Observer):

    opGroup = Spec.EnablePermissionsControlledJoinsFrame.opGroup
    opCode = Spec.EnablePermissionsControlledJoinsFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.EnablePermissionsControlledJoinsFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = EnablePermissionsControlledJoins()
        frame.EnableDisable = packet.getParamValueAsNumber("EnableDisable")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class AuthenticateDeviceObserver(Observer):

    opGroup = Spec.AuthenticateDeviceFrame.opGroup
    opCode = Spec.AuthenticateDeviceFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.AuthenticateDeviceFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = AuthenticateDevice()
        frame.IEEEAddress = packet.getParamValueAsNumber("IEEEAddress")
        frame.Key = packet.getParamValueAsNumber("Key")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class AddGroupObserver(Observer):

    opGroup = Spec.AddGroupFrame.opGroup
    opCode = Spec.AddGroupFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.AddGroupFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = AddGroup()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.GroupAddress = packet.getParamValueAsNumber("GroupAddress")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ViewGroupObserver(Observer):

    opGroup = Spec.ViewGroupFrame.opGroup
    opCode = Spec.ViewGroupFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ViewGroupFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ViewGroup()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.GroupAddress = packet.getParamValueAsNumber("GroupAddress")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GetGroupMembershipObserver(Observer):

    opGroup = Spec.GetGroupMembershipFrame.opGroup
    opCode = Spec.GetGroupMembershipFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GetGroupMembershipFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GetGroupMembership()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.GroupCount = packet.getParamValueAsNumber("GroupCount")
        frame.GroupList = packet.getParamValueAsNumber("GroupList")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class RemoveGroupObserver(Observer):

    opGroup = Spec.RemoveGroupFrame.opGroup
    opCode = Spec.RemoveGroupFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.RemoveGroupFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = RemoveGroup()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.GroupAddress = packet.getParamValueAsNumber("GroupAddress")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class RemoveAllGroupsObserver(Observer):

    opGroup = Spec.RemoveAllGroupsFrame.opGroup
    opCode = Spec.RemoveAllGroupsFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.RemoveAllGroupsFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = RemoveAllGroups()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class AddGroupIfIdentifyObserver(Observer):

    opGroup = Spec.AddGroupIfIdentifyFrame.opGroup
    opCode = Spec.AddGroupIfIdentifyFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.AddGroupIfIdentifyFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = AddGroupIfIdentify()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.GroupAddress = packet.getParamValueAsNumber("GroupAddress")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class IdentifySendObserver(Observer):

    opGroup = Spec.IdentifySendFrame.opGroup
    opCode = Spec.IdentifySendFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.IdentifySendFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = IdentifySend()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.TimeInSecconds = packet.getParamValueAsNumber("TimeInSecconds")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class IdentifyQueryObserver(Observer):

    opGroup = Spec.IdentifyQueryFrame.opGroup
    opCode = Spec.IdentifyQueryFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.IdentifyQueryFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = IdentifyQuery()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class IdentifyTriggerEffectsObserver(Observer):

    opGroup = Spec.IdentifyTriggerEffectsFrame.opGroup
    opCode = Spec.IdentifyTriggerEffectsFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.IdentifyTriggerEffectsFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = IdentifyTriggerEffects()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.EffectID = packet.getParamValueAsNumber("EffectID")
        frame.EffectGradient = packet.getParamValueAsNumber("EffectGradient")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MoveToLevelObserver(Observer):

    opGroup = Spec.MoveToLevelFrame.opGroup
    opCode = Spec.MoveToLevelFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MoveToLevelFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MoveToLevel()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.OnOff = packet.getParamValueAsNumber("OnOff")
        frame.Mode = packet.getParamValueAsNumber("Mode")
        frame.Rate = packet.getParamValueAsNumber("Rate")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MoveToLevelWithWithoutOnOffObserver(Observer):

    opGroup = Spec.MoveToLevelWithWithoutOnOffFrame.opGroup
    opCode = Spec.MoveToLevelWithWithoutOnOffFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MoveToLevelWithWithoutOnOffFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MoveToLevelWithWithoutOnOff()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.OnOff = packet.getParamValueAsNumber("OnOff")
        frame.Level = packet.getParamValueAsNumber("Level")
        frame.TransitionTime = packet.getParamValueAsNumber("TransitionTime")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MoveStepObserver(Observer):

    opGroup = Spec.MoveStepFrame.opGroup
    opCode = Spec.MoveStepFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MoveStepFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MoveStep()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.OnOff = packet.getParamValueAsNumber("OnOff")
        frame.StepMode = packet.getParamValueAsNumber("StepMode")
        frame.StepSize = packet.getParamValueAsNumber("StepSize")
        frame.TransitionTime = packet.getParamValueAsNumber("TransitionTime")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MoveStopWithOnOffObserver(Observer):

    opGroup = Spec.MoveStopWithOnOffFrame.opGroup
    opCode = Spec.MoveStopWithOnOffFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MoveStopWithOnOffFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MoveStopWithOnOff()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MoveToClosestFrequencyObserver(Observer):

    opGroup = Spec.MoveToClosestFrequencyFrame.opGroup
    opCode = Spec.MoveToClosestFrequencyFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MoveToClosestFrequencyFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MoveToClosestFrequency()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.Frequency = packet.getParamValueAsNumber("Frequency")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class OnOffWithEffectsSendObserver(Observer):

    opGroup = Spec.OnOffWithEffectsSendFrame.opGroup
    opCode = Spec.OnOffWithEffectsSendFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.OnOffWithEffectsSendFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = OnOffWithEffectsSend()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.EffectID = packet.getParamValueAsNumber("EffectID")
        frame.EffectGradient = packet.getParamValueAsNumber("EffectGradient")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class OnOffWithNoEffectsObserver(Observer):

    opGroup = Spec.OnOffWithNoEffectsFrame.opGroup
    opCode = Spec.OnOffWithNoEffectsFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.OnOffWithNoEffectsFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = OnOffWithNoEffects()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.CommandID = packet.getParamValueAsNumber("CommandID")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class OnOffTimedSendObserver(Observer):

    opGroup = Spec.OnOffTimedSendFrame.opGroup
    opCode = Spec.OnOffTimedSendFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.OnOffTimedSendFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = OnOffTimedSend()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.OnOff = packet.getParamValueAsNumber("OnOff")
        frame.OnTimeInSecconds = packet.getParamValueAsNumber("OnTimeInSecconds")
        frame.OffTimeInSecconds = packet.getParamValueAsNumber("OffTimeInSecconds")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class DiagnosticResponseObserver(Observer):

    opGroup = Spec.DiagnosticResponseFrame.opGroup
    opCode = Spec.DiagnosticResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.DiagnosticResponseFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = DiagnosticResponse()
        frame.SeqNum = packet.getParamValueAsNumber("SeqNum")
        frame.srcEndpoint = packet.getParamValueAsNumber("srcEndpoint")
        frame.ClusterId = packet.getParamValueAsNumber("ClusterId")
        frame.Status = DiagnosticResponseStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame.RequestLatency = packet.getParamValueAsNumber("RequestLatency")
        frame.ResponseLatency = packet.getParamValueAsNumber("ResponseLatency")
        frame.Offset = packet.getParamValueAsNumber("Offset")
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame.PayloadSize = packet.getParamValueAsNumber("PayloadSize")
        frame.Payload = packet.getParamValueAsList("Payload")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ViewSceneObserver(Observer):

    opGroup = Spec.ViewSceneFrame.opGroup
    opCode = Spec.ViewSceneFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ViewSceneFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ViewScene()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.GroupID = packet.getParamValueAsNumber("GroupID")
        frame.SceneID = packet.getParamValueAsNumber("SceneID")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class AddSceneObserver(Observer):

    opGroup = Spec.AddSceneFrame.opGroup
    opCode = Spec.AddSceneFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.AddSceneFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = AddScene()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.GroupID = packet.getParamValueAsNumber("GroupID")
        frame.SceneID = packet.getParamValueAsNumber("SceneID")
        frame.TransitionTime = packet.getParamValueAsNumber("TransitionTime")
        frame.SceneNameLength = packet.getParamValueAsNumber("SceneNameLength")
        frame.SceneNameMaxLength = packet.getParamValueAsNumber("SceneNameMaxLength")
        frame.SceneNameData = packet.getParamValueAsNumber("SceneNameData")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class RemoveSceneObserver(Observer):

    opGroup = Spec.RemoveSceneFrame.opGroup
    opCode = Spec.RemoveSceneFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.RemoveSceneFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = RemoveScene()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.GroupID = packet.getParamValueAsNumber("GroupID")
        frame.SceneID = packet.getParamValueAsNumber("SceneID")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class RemoveAllScenesObserver(Observer):

    opGroup = Spec.RemoveAllScenesFrame.opGroup
    opCode = Spec.RemoveAllScenesFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.RemoveAllScenesFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = RemoveAllScenes()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.GroupID = packet.getParamValueAsNumber("GroupID")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class StoreSceneObserver(Observer):

    opGroup = Spec.StoreSceneFrame.opGroup
    opCode = Spec.StoreSceneFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.StoreSceneFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = StoreScene()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.GroupID = packet.getParamValueAsNumber("GroupID")
        frame.SceneID = packet.getParamValueAsNumber("SceneID")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class RecallSceneObserver(Observer):

    opGroup = Spec.RecallSceneFrame.opGroup
    opCode = Spec.RecallSceneFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.RecallSceneFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = RecallScene()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.GroupID = packet.getParamValueAsNumber("GroupID")
        frame.SceneID = packet.getParamValueAsNumber("SceneID")
        frame.TransitionTime = packet.getParamValueAsNumber("TransitionTime")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class AddEnhancedSceneObserver(Observer):

    opGroup = Spec.AddEnhancedSceneFrame.opGroup
    opCode = Spec.AddEnhancedSceneFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.AddEnhancedSceneFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = AddEnhancedScene()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.GroupID = packet.getParamValueAsNumber("GroupID")
        frame.SceneID = packet.getParamValueAsNumber("SceneID")
        frame.TransitionTime = packet.getParamValueAsNumber("TransitionTime")
        frame.SceneNameLength = packet.getParamValueAsNumber("SceneNameLength")
        frame.SceneNameData = packet.getParamValueAsNumber("SceneNameData")
        frame.Length = packet.getParamValueAsNumber("Length")
        frame.MaxLength = packet.getParamValueAsNumber("MaxLength")
        frame.ExtensionFieldSets = packet.getParamValueAsNumber("ExtensionFieldSets")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ViewEnhancedSceneObserver(Observer):

    opGroup = Spec.ViewEnhancedSceneFrame.opGroup
    opCode = Spec.ViewEnhancedSceneFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ViewEnhancedSceneFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ViewEnhancedScene()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.GroupID = packet.getParamValueAsNumber("GroupID")
        frame.SceneID = packet.getParamValueAsNumber("SceneID")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class CopySceneObserver(Observer):

    opGroup = Spec.CopySceneFrame.opGroup
    opCode = Spec.CopySceneFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.CopySceneFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = CopyScene()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.Mode = packet.getParamValueAsNumber("Mode")
        frame.FromGroupID = packet.getParamValueAsNumber("FromGroupID")
        frame.FromSceneID = packet.getParamValueAsNumber("FromSceneID")
        frame.ToGroupID = packet.getParamValueAsNumber("ToGroupID")
        frame.ToSceneID = packet.getParamValueAsNumber("ToSceneID")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)

class MoveToHueObserver(Observer):

    opGroup = Spec.MoveToHueFrame.opGroup
    opCode = Spec.MoveToHueFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MoveToHueFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MoveToHue()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.Hue = packet.getParamValueAsNumber("Hue")
        frame.Direction = packet.getParamValueAsNumber("Direction")
        frame.TransitionTime = packet.getParamValueAsNumber("TransitionTime")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MoveHueObserver(Observer):

    opGroup = Spec.MoveHueFrame.opGroup
    opCode = Spec.MoveHueFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MoveHueFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MoveHue()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.Mode = packet.getParamValueAsNumber("Mode")
        frame.Rate = packet.getParamValueAsNumber("Rate")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class StepHueObserver(Observer):

    opGroup = Spec.StepHueFrame.opGroup
    opCode = Spec.StepHueFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.StepHueFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = StepHue()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.Mode = packet.getParamValueAsNumber("Mode")
        frame.StepSize = packet.getParamValueAsNumber("StepSize")
        frame.TransitionTime = packet.getParamValueAsNumber("TransitionTime")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MoveToSaturationObserver(Observer):

    opGroup = Spec.MoveToSaturationFrame.opGroup
    opCode = Spec.MoveToSaturationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MoveToSaturationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MoveToSaturation()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.Saturation = packet.getParamValueAsNumber("Saturation")
        frame.TransitionTime = packet.getParamValueAsNumber("TransitionTime")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MoveSaturationObserver(Observer):

    opGroup = Spec.MoveSaturationFrame.opGroup
    opCode = Spec.MoveSaturationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MoveSaturationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MoveSaturation()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.Mode = packet.getParamValueAsNumber("Mode")
        frame.Rate = packet.getParamValueAsNumber("Rate")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class StepSaturationObserver(Observer):

    opGroup = Spec.StepSaturationFrame.opGroup
    opCode = Spec.StepSaturationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.StepSaturationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = StepSaturation()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.Mode = packet.getParamValueAsNumber("Mode")
        frame.StepSize = packet.getParamValueAsNumber("StepSize")
        frame.TransitionTime = packet.getParamValueAsNumber("TransitionTime")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MoveToHueAndSaturationObserver(Observer):

    opGroup = Spec.MoveToHueAndSaturationFrame.opGroup
    opCode = Spec.MoveToHueAndSaturationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MoveToHueAndSaturationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MoveToHueAndSaturation()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.Hue = packet.getParamValueAsNumber("Hue")
        frame.Saturation = packet.getParamValueAsNumber("Saturation")
        frame.TransitionTime = packet.getParamValueAsNumber("TransitionTime")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MoveToColourObserver(Observer):

    opGroup = Spec.MoveToColourFrame.opGroup
    opCode = Spec.MoveToColourFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MoveToColourFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MoveToColour()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.ColorX = packet.getParamValueAsNumber("ColorX")
        frame.ColorY = packet.getParamValueAsNumber("ColorY")
        frame.TransitionTime = packet.getParamValueAsNumber("TransitionTime")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MoveColourObserver(Observer):

    opGroup = Spec.MoveColourFrame.opGroup
    opCode = Spec.MoveColourFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MoveColourFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MoveColour()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.ColorX = packet.getParamValueAsNumber("ColorX")
        frame.ColorY = packet.getParamValueAsNumber("ColorY")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)







class EnhancedMoveToHueObserver(Observer):

    opGroup = Spec.EnhancedMoveToHueFrame.opGroup
    opCode = Spec.EnhancedMoveToHueFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.EnhancedMoveToHueFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = EnhancedMoveToHue()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.Direction = packet.getParamValueAsNumber("Direction")
        frame.EnhancedHue = packet.getParamValueAsNumber("EnhancedHue")
        frame.TransitionTime = packet.getParamValueAsNumber("TransitionTime")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class EnhancedMoveHueObserver(Observer):

    opGroup = Spec.EnhancedMoveHueFrame.opGroup
    opCode = Spec.EnhancedMoveHueFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.EnhancedMoveHueFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = EnhancedMoveHue()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.Mode = packet.getParamValueAsNumber("Mode")
        frame.Rate = packet.getParamValueAsNumber("Rate")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class EnhancedStepHueObserver(Observer):

    opGroup = Spec.EnhancedStepHueFrame.opGroup
    opCode = Spec.EnhancedStepHueFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.EnhancedStepHueFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = EnhancedStepHue()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.Mode = packet.getParamValueAsNumber("Mode")
        frame.StepSize = packet.getParamValueAsNumber("StepSize")
        frame.TransitionTime = packet.getParamValueAsNumber("TransitionTime")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class EnhancedMoveToHueAndSaturationObserver(Observer):

    opGroup = Spec.EnhancedMoveToHueAndSaturationFrame.opGroup
    opCode = Spec.EnhancedMoveToHueAndSaturationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.EnhancedMoveToHueAndSaturationFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = EnhancedMoveToHueAndSaturation()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.Saturation = packet.getParamValueAsNumber("Saturation")
        frame.EnhancedHue = packet.getParamValueAsNumber("EnhancedHue")
        frame.TransitionTime = packet.getParamValueAsNumber("TransitionTime")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ColourLoopSetObserver(Observer):

    opGroup = Spec.ColourLoopSetFrame.opGroup
    opCode = Spec.ColourLoopSetFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ColourLoopSetFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ColourLoopSet()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.UpdateFlags = packet.getParamValueAsNumber("UpdateFlags")
        frame.Action = packet.getParamValueAsNumber("Action")
        frame.Direction = packet.getParamValueAsNumber("Direction")
        frame.Time = packet.getParamValueAsNumber("Time")
        frame.StartHue = packet.getParamValueAsNumber("StartHue")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class StopMoveStepObserver(Observer):

    opGroup = Spec.StopMoveStepFrame.opGroup
    opCode = Spec.StopMoveStepFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.StopMoveStepFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = StopMoveStep()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.OptionsMask = packet.getParamValueAsNumber("OptionsMask")
        frame.OptionsOverride = packet.getParamValueAsNumber("OptionsOverride")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MoveToColourTemperatureObserver(Observer):

    opGroup = Spec.MoveToColourTemperatureFrame.opGroup
    opCode = Spec.MoveToColourTemperatureFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MoveToColourTemperatureFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MoveToColourTemperature()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.ColourTemperature = packet.getParamValueAsNumber("ColourTemperature")
        frame.TransitionTime = packet.getParamValueAsNumber("TransitionTime")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MoveColourTemperatureObserver(Observer):

    opGroup = Spec.MoveColourTemperatureFrame.opGroup
    opCode = Spec.MoveColourTemperatureFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MoveColourTemperatureFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MoveColourTemperature()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.Mode = packet.getParamValueAsNumber("Mode")
        frame.Rate = packet.getParamValueAsNumber("Rate")
        frame.MinimumTemperature = packet.getParamValueAsNumber("MinimumTemperature")
        frame.MaximumTemperature = packet.getParamValueAsNumber("MaximumTemperature")
        frame.OptionsMask = packet.getParamValueAsNumber("OptionsMask")
        frame.OptionsOverride = packet.getParamValueAsNumber("OptionsOverride")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class StepColourTemperatureObserver(Observer):

    opGroup = Spec.StepColourTemperatureFrame.opGroup
    opCode = Spec.StepColourTemperatureFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.StepColourTemperatureFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = StepColourTemperature()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.Mode = packet.getParamValueAsNumber("Mode")
        frame.StepSize = packet.getParamValueAsNumber("StepSize")
        frame.TransitionTime = packet.getParamValueAsNumber("TransitionTime")
        frame.MinimumTemperature = packet.getParamValueAsNumber("MinimumTemperature")
        frame.MaximumTemperature = packet.getParamValueAsNumber("MaximumTemperature")
        frame.OptionsMask = packet.getParamValueAsNumber("OptionsMask")
        frame.OptionsOverride = packet.getParamValueAsNumber("OptionsOverride")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class InitiateTouchlinkObserver(Observer):

    opGroup = Spec.InitiateTouchlinkFrame.opGroup
    opCode = Spec.InitiateTouchlinkFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Create frame object
        frame = InitiateTouchlink()
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class TouchLinkFactoryResetTargetObserver(Observer):

    opGroup = Spec.TouchLinkFactoryResetTargetFrame.opGroup
    opCode = Spec.TouchLinkFactoryResetTargetFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Create frame object
        frame = TouchLinkFactoryResetTarget()
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)

class LockUnlockDoorObserver(Observer):

    opGroup = Spec.LockUnlockDoorFrame.opGroup
    opCode = Spec.LockUnlockDoorFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.LockUnlockDoorFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = LockUnlockDoor()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.LockUnlock = packet.getParamValueAsNumber("LockUnlock")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class IASZoneEnrollResponseObserver(Observer):

    opGroup = Spec.IASZoneEnrollResponseFrame.opGroup
    opCode = Spec.IASZoneEnrollResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.IASZoneEnrollResponseFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = IASZoneEnrollResponse()
        frame.AddressMode = packet.getParamValueAsNumber("AddressMode")
        frame.TargetShortAddress = packet.getParamValueAsNumber("TargetShortAddress")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.EnrollResponseCode = packet.getParamValueAsNumber("EnrollResponseCode")
        frame.ZoneID = packet.getParamValueAsNumber("ZoneID")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class StatusObserver(Observer):

    opGroup = Spec.StatusFrame.opGroup
    opCode = Spec.StatusFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.StatusFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = Status()
        frame.Status = StatusStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame.SeqNum = packet.getParamValueAsNumber("SeqNum")
        frame.PacketType = packet.getParamValueAsNumber("PacketType")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class LogMessageObserver(Observer):

    opGroup = Spec.LogMessageFrame.opGroup
    opCode = Spec.LogMessageFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.LogMessageFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = LogMessage()
        frame.LogLevel = packet.getParamValueAsNumber("LogLevel")
        frame.LogMessage = packet.getParamValueAsNumber("LogMessage")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class DataIndicationObserver(Observer):

    opGroup = Spec.DataIndicationFrame.opGroup
    opCode = Spec.DataIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.DataIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = DataIndication()
        frame.Status = packet.getParamValueAsNumber("Status")
        frame.ProfileID = packet.getParamValueAsNumber("ProfileID")
        frame.ClusterID = packet.getParamValueAsNumber("ClusterID")
        frame.SourceEndpoint = packet.getParamValueAsNumber("SourceEndpoint")
        frame.DestinationEndpoint = packet.getParamValueAsNumber("DestinationEndpoint")
        frame.SourceAddressMode = packet.getParamValueAsNumber("SourceAddressMode")
        frame.SourceAddress = packet.getParamValueAsNumber("SourceAddress")
        frame.DestAddressingMode = packet.getParamValueAsNumber("DestAddressingMode")
        frame.DestinationAddress = packet.getParamValueAsNumber("DestinationAddress")
        frame.PayloadSize = packet.getParamValueAsNumber("PayloadSize")
        frame.Payload = packet.getParamValueAsNumber("Payload")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class NodeClusterListObserver(Observer):

    opGroup = Spec.NodeClusterListFrame.opGroup
    opCode = Spec.NodeClusterListFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.NodeClusterListFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = NodeClusterList()
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.ProfileID = packet.getParamValueAsNumber("ProfileID")
        frame.ClusterList = packet.getParamValueAsNumber("ClusterList")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class NodeClusterAttributeListObserver(Observer):

    opGroup = Spec.NodeClusterAttributeListFrame.opGroup
    opCode = Spec.NodeClusterAttributeListFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.NodeClusterAttributeListFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = NodeClusterAttributeList()
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.ProfileID = packet.getParamValueAsNumber("ProfileID")
        frame.ClusterID = packet.getParamValueAsNumber("ClusterID")
        frame.AttributeList = packet.getParamValueAsNumber("AttributeList")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class NodeCommandIDListObserver(Observer):

    opGroup = Spec.NodeCommandIDListFrame.opGroup
    opCode = Spec.NodeCommandIDListFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.NodeCommandIDListFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = NodeCommandIDList()
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.ProfileID = packet.getParamValueAsNumber("ProfileID")
        frame.ClusterID = packet.getParamValueAsNumber("ClusterID")
        frame.CommandIDList = packet.getParamValueAsNumber("CommandIDList")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class VersionListObserver(Observer):

    opGroup = Spec.VersionListFrame.opGroup
    opCode = Spec.VersionListFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.VersionListFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = VersionList()
        frame.MajorVersionNumber = packet.getParamValueAsNumber("MajorVersionNumber")
        frame.InstallerVersionNumber = packet.getParamValueAsNumber("InstallerVersionNumber")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class NetworkJoinedFormedObserver(Observer):

    opGroup = Spec.NetworkJoinedFormedFrame.opGroup
    opCode = Spec.NetworkJoinedFormedFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.NetworkJoinedFormedFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = NetworkJoinedFormed()
        frame.Status = NetworkJoinedFormedStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame.ShortAddr = packet.getParamValueAsNumber("ShortAddr")
        frame.ExtAddr = packet.getParamValueAsNumber("ExtAddr")
        frame.Channel = packet.getParamValueAsNumber("Channel")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class PermitJoinStatusResponseObserver(Observer):

    opGroup = Spec.PermitJoinStatusResponseFrame.opGroup
    opCode = Spec.PermitJoinStatusResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.PermitJoinStatusResponseFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = PermitJoinStatusResponse()
        frame.Status = PermitJoinStatusResponseStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class NonFactoryNewRestartObserver(Observer):

    opGroup = Spec.NonFactoryNewRestartFrame.opGroup
    opCode = Spec.NonFactoryNewRestartFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.NonFactoryNewRestartFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = NonFactoryNewRestart()
        frame.Status = NonFactoryNewRestartStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class FactoryNewRestartObserver(Observer):

    opGroup = Spec.FactoryNewRestartFrame.opGroup
    opCode = Spec.FactoryNewRestartFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.FactoryNewRestartFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = FactoryNewRestart()
        frame.Status = FactoryNewRestartStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class BindResponseObserver(Observer):

    opGroup = Spec.BindResponseFrame.opGroup
    opCode = Spec.BindResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.BindResponseFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = BindResponse()
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame.Status = packet.getParamValueAsNumber("Status")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class UnbindResponseObserver(Observer):

    opGroup = Spec.UnbindResponseFrame.opGroup
    opCode = Spec.UnbindResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.UnbindResponseFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = UnbindResponse()
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame.Status = packet.getParamValueAsNumber("Status")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class NetworkAddressResponseObserver(Observer):

    opGroup = Spec.NetworkAddressResponseFrame.opGroup
    opCode = Spec.NetworkAddressResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.NetworkAddressResponseFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = NetworkAddressResponse()
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame.Status = packet.getParamValueAsNumber("Status")
        frame.IEEEAddress = packet.getParamValueAsNumber("IEEEAddress")
        frame.ShortAddress = packet.getParamValueAsNumber("ShortAddress")
        frame.NumberOfAssociatedDevices = packet.getParamValueAsNumber("NumberOfAssociatedDevices")
        frame.StartIndex = packet.getParamValueAsNumber("StartIndex")
        frame.DeviceList = packet.getParamValueAsNumber("DeviceList")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class IEEEAddressResponseObserver(Observer):

    opGroup = Spec.IEEEAddressResponseFrame.opGroup
    opCode = Spec.IEEEAddressResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.IEEEAddressResponseFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = IEEEAddressResponse()
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame.Status = packet.getParamValueAsNumber("Status")
        frame.IEEEAddress = packet.getParamValueAsNumber("IEEEAddress")
        frame.shortAddress = packet.getParamValueAsNumber("shortAddress")
        frame.NumberOfAssociatedDevices = packet.getParamValueAsNumber("NumberOfAssociatedDevices")
        frame.StartIndex = packet.getParamValueAsNumber("StartIndex")
        frame.DeviceList = packet.getParamValueAsNumber("DeviceList")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class NodeDescriptorResponseObserver(Observer):

    opGroup = Spec.NodeDescriptorResponseFrame.opGroup
    opCode = Spec.NodeDescriptorResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.NodeDescriptorResponseFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = NodeDescriptorResponse()
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame.Status = packet.getParamValueAsNumber("Status")
        frame.NetworkAddress = packet.getParamValueAsNumber("NetworkAddress")
        frame.ManufacturerCode = packet.getParamValueAsNumber("ManufacturerCode")
        frame.MaxRxSize = packet.getParamValueAsNumber("MaxRxSize")
        frame.MaxTxSize = packet.getParamValueAsNumber("MaxTxSize")
        frame.ServerMask = packet.getParamValueAsNumber("ServerMask")
        frame.DescriptorCapability = packet.getParamValueAsNumber("DescriptorCapability")
        frame.MacCapability = packet.getParamValueAsNumber("MacCapability")
        frame.MaxBufferSize = packet.getParamValueAsNumber("MaxBufferSize")
        frame.BitFields = packet.getParamValueAsNumber("BitFields")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SimpleDescriptorResponseObserver(Observer):

    opGroup = Spec.SimpleDescriptorResponseFrame.opGroup
    opCode = Spec.SimpleDescriptorResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SimpleDescriptorResponseFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SimpleDescriptorResponse()
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame.Status = packet.getParamValueAsNumber("Status")
        frame.NwkAddress = packet.getParamValueAsNumber("NwkAddress")
        frame.Length = packet.getParamValueAsNumber("Length")
        frame.EndPoint = packet.getParamValueAsNumber("EndPoint")
        frame.Profile = packet.getParamValueAsNumber("Profile")
        frame.DeviceID = packet.getParamValueAsNumber("DeviceID")
        frame.BitFields.DeviceVersion = packet.getParamValueAsNumber("BitFieldsDeviceVersion")
        frame.BitFields.Reserved = packet.getParamValueAsNumber("BitFieldsReserved")
        frame.InClusterCount = packet.getParamValueAsNumber("InClusterCount")
        frame.InClusterList = packet.getParamValueAsNumber("InClusterList")
        frame.OutClusterCount = packet.getParamValueAsNumber("OutClusterCount")
        frame.OutClusterList = packet.getParamValueAsNumber("OutClusterList")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class PowerDescriptorResponseObserver(Observer):

    opGroup = Spec.PowerDescriptorResponseFrame.opGroup
    opCode = Spec.PowerDescriptorResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.PowerDescriptorResponseFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = PowerDescriptorResponse()
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame.Status = packet.getParamValueAsNumber("Status")
        frame.BitFields.CurrentPowerMode = packet.getParamValueAsNumber("BitFieldsCurrentPowerMode")
        frame.BitFields.AvailablePowerSource = packet.getParamValueAsNumber("BitFieldsAvailablePowerSource")
        frame.BitFields.CurrentPowerSource = packet.getParamValueAsNumber("BitFieldsCurrentPowerSource")
        frame.BitFields.CurrentPowerSourceLevel = packet.getParamValueAsNumber("BitFieldsCurrentPowerSourceLevel")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ActiveEndpointResponseObserver(Observer):

    opGroup = Spec.ActiveEndpointResponseFrame.opGroup
    opCode = Spec.ActiveEndpointResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ActiveEndpointResponseFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ActiveEndpointResponse()
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame.Status = packet.getParamValueAsNumber("Status")
        frame.Address = packet.getParamValueAsNumber("Address")
        frame.EndpointCount = packet.getParamValueAsNumber("EndpointCount")
        frame.ActiveEndPointList = packet.getParamValueAsNumber("ActiveEndPointList")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MatchDescriptorResponseObserver(Observer):

    opGroup = Spec.MatchDescriptorResponseFrame.opGroup
    opCode = Spec.MatchDescriptorResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MatchDescriptorResponseFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MatchDescriptorResponse()
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame.Status = packet.getParamValueAsNumber("Status")
        frame.NetworkAddress = packet.getParamValueAsNumber("NetworkAddress")
        frame.LengthOfList = packet.getParamValueAsNumber("LengthOfList")
        frame.MatchList = packet.getParamValueAsNumber("MatchList")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class DeviceAnnounceObserver(Observer):

    opGroup = Spec.DeviceAnnounceFrame.opGroup
    opCode = Spec.DeviceAnnounceFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.DeviceAnnounceFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = DeviceAnnounce()
        frame.ShortAddress = packet.getParamValueAsNumber("ShortAddress")
        frame.IEEEAddress = packet.getParamValueAsNumber("IEEEAddress")
        frame.MACCapability.AlternatePANCoordinator = packet.getParamValueAsNumber(
            "MACCapabilityAlternatePANCoordinator")
        frame.MACCapability.DeviceType = packet.getParamValueAsNumber("MACCapabilityDeviceType")
        frame.MACCapability.PowerSource = packet.getParamValueAsNumber("MACCapabilityPowerSource")
        frame.MACCapability.ReceiverOnWhenIdle = packet.getParamValueAsNumber("MACCapabilityReceiverOnWhenIdle")
        frame.MACCapability.Reserved = packet.getParamValueAsNumber("MACCapabilityReserved")
        frame.MACCapability.SecurityCapability = packet.getParamValueAsNumber("MACCapabilitySecurityCapability")
        frame.MACCapability.AllocateAddress = packet.getParamValueAsNumber("MACCapabilityAllocateAddress")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class UserDescriptorResponseObserver(Observer):

    opGroup = Spec.UserDescriptorResponseFrame.opGroup
    opCode = Spec.UserDescriptorResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.UserDescriptorResponseFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = UserDescriptorResponse()
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame.Status = packet.getParamValueAsNumber("Status")
        frame.NetworkAddressOfOnterest = packet.getParamValueAsNumber("NetworkAddressOfOnterest")
        frame.LengthOfData = packet.getParamValueAsNumber("LengthOfData")
        frame.DataList = packet.getParamValueAsNumber("DataList")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class UserDescriptorNotifyObserver(Observer):

    opGroup = Spec.UserDescriptorNotifyFrame.opGroup
    opCode = Spec.UserDescriptorNotifyFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.UserDescriptorNotifyFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = UserDescriptorNotify()
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame.Status = packet.getParamValueAsNumber("Status")
        frame.NetworkAddressOfOnterest = packet.getParamValueAsNumber("NetworkAddressOfOnterest")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ComplexDescriptorResponseObserver(Observer):

    opGroup = Spec.ComplexDescriptorResponseFrame.opGroup
    opCode = Spec.ComplexDescriptorResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ComplexDescriptorResponseFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ComplexDescriptorResponse()
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame.Status = packet.getParamValueAsNumber("Status")
        frame.NetworkAddressOfOnterest = packet.getParamValueAsNumber("NetworkAddressOfOnterest")
        frame.Length = packet.getParamValueAsNumber("Length")
        frame.XMLTag = packet.getParamValueAsNumber("XMLTag")
        frame.FieldCount = packet.getParamValueAsNumber("FieldCount")
        frame.FieldValues = packet.getParamValueAsNumber("FieldValues")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ManagementLeaveResponseObserver(Observer):

    opGroup = Spec.ManagementLeaveResponseFrame.opGroup
    opCode = Spec.ManagementLeaveResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ManagementLeaveResponseFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ManagementLeaveResponse()
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame.Status = packet.getParamValueAsNumber("Status")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class LeaveIndicationObserver(Observer):

    opGroup = Spec.LeaveIndicationFrame.opGroup
    opCode = Spec.LeaveIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.LeaveIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = LeaveIndication()
        frame.ExtendedAddress = packet.getParamValueAsNumber("ExtendedAddress")
        frame.RejoinStatus = packet.getParamValueAsNumber("RejoinStatus")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ManagementNetworkUpdateResponseObserver(Observer):

    opGroup = Spec.ManagementNetworkUpdateResponseFrame.opGroup
    opCode = Spec.ManagementNetworkUpdateResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ManagementNetworkUpdateResponseFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ManagementNetworkUpdateResponse()
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame.Status = packet.getParamValueAsNumber("Status")
        frame.TotalTransmission = packet.getParamValueAsNumber("TotalTransmission")
        frame.TransmissionFailures = packet.getParamValueAsNumber("TransmissionFailures")
        frame.ScannedChannels = packet.getParamValueAsNumber("ScannedChannels")
        frame.ScannedChannelListCount = packet.getParamValueAsNumber("ScannedChannelListCount")
        frame.ChannelList = packet.getParamValueAsNumber("ChannelList")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SystemServerDiscoveryResponseObserver(Observer):

    opGroup = Spec.SystemServerDiscoveryResponseFrame.opGroup
    opCode = Spec.SystemServerDiscoveryResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SystemServerDiscoveryResponseFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SystemServerDiscoveryResponse()
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame.Status = packet.getParamValueAsNumber("Status")
        frame.ServerMask = packet.getParamValueAsNumber("ServerMask")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ManagementLQIResponseObserver(Observer):

    opGroup = Spec.ManagementLQIResponseFrame.opGroup
    opCode = Spec.ManagementLQIResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ManagementLQIResponseFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ManagementLQIResponse()
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame.Status = packet.getParamValueAsNumber("Status")
        frame.NeighbourTableEntries = packet.getParamValueAsNumber("NeighbourTableEntries")
        frame.NeighbourTableListCount = packet.getParamValueAsNumber("NeighbourTableListCount")
        frame.StartIndex = packet.getParamValueAsNumber("StartIndex")
        frame.ListOfEntries_NWKAddress = packet.getParamValueAsNumber("ListOfEntries_NWKAddress")
        frame.ListOfEntries_ExtendedPANID = packet.getParamValueAsNumber("ListOfEntries_ExtendedPANID")
        frame.ListOfEntries_IEEEAddress = packet.getParamValueAsNumber("ListOfEntries_IEEEAddress")
        frame.ListOfEntries_Depth = packet.getParamValueAsNumber("ListOfEntries_Depth")
        frame.ListOfEntries_LinkQuality = packet.getParamValueAsNumber("ListOfEntries_LinkQuality")
        frame.ListOfEntries_BitMapOfAttributes.DeviceType = packet.getParamValueAsNumber(
            "ListOfEntries_BitMapOfAttributesDeviceType")
        frame.ListOfEntries_BitMapOfAttributes.PermitJoinStatus = ManagementLQIResponse.ListOfEntries_BitMapOfAttributesPermitJoinStatus.getEnumString(
            packet.getParamValueAsNumber("ListOfEntries_BitMapOfAttributesPermitJoinStatus"))
        frame.ListOfEntries_BitMapOfAttributes.Relationship = packet.getParamValueAsNumber(
            "ListOfEntries_BitMapOfAttributesRelationship")
        frame.ListOfEntries_BitMapOfAttributes.RxOnWhenIdleStatus = ManagementLQIResponse.ListOfEntries_BitMapOfAttributesRxOnWhenIdleStatus.getEnumString(
            packet.getParamValueAsNumber("ListOfEntries_BitMapOfAttributesRxOnWhenIdleStatus"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class AttributeDiscoveryResponseObserver(Observer):

    opGroup = Spec.AttributeDiscoveryResponseFrame.opGroup
    opCode = Spec.AttributeDiscoveryResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.AttributeDiscoveryResponseFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = AttributeDiscoveryResponse()
        frame.Complete = packet.getParamValueAsNumber("Complete")
        frame.AttributeType = packet.getParamValueAsNumber("AttributeType")
        frame.AttributeID = packet.getParamValueAsNumber("AttributeID")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class AuthenticateResponseObserver(Observer):

    opGroup = Spec.AuthenticateResponseFrame.opGroup
    opCode = Spec.AuthenticateResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.AuthenticateResponseFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = AuthenticateResponse()
        frame.IEEEAddressOfTtheGateway = packet.getParamValueAsNumber("IEEEAddressOfTtheGateway")
        frame.EncryptedKey = packet.getParamValueAsNumber("EncryptedKey")
        frame.MIC = packet.getParamValueAsNumber("MIC")
        frame.IEEEAddressOfTheInitiatingNode = packet.getParamValueAsNumber("IEEEAddressOfTheInitiatingNode")
        frame.ActiveKeySequenceNumber = packet.getParamValueAsNumber("ActiveKeySequenceNumber")
        frame.Channel = packet.getParamValueAsNumber("Channel")
        frame.ShortPANID = packet.getParamValueAsNumber("ShortPANID")
        frame.ExtendedPANID = packet.getParamValueAsNumber("ExtendedPANID")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class CodeResponseObserver(Observer):

    opGroup = Spec.CodeResponseFrame.opGroup
    opCode = Spec.CodeResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.CodeResponseFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = CodeResponse()
        frame.Status = packet.getParamValueAsNumber("Status")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ConfigureReportingResponseObserver(Observer):

    opGroup = Spec.ConfigureReportingResponseFrame.opGroup
    opCode = Spec.ConfigureReportingResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ConfigureReportingResponseFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ConfigureReportingResponse()
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame.SrcAddress = packet.getParamValueAsNumber("SrcAddress")
        frame.Endpoint = packet.getParamValueAsNumber("Endpoint")
        frame.ClusterID = packet.getParamValueAsNumber("ClusterID")
        frame.Status = packet.getParamValueAsNumber("Status")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ReadIndividualAttributeResponseObserver(Observer):

    opGroup = Spec.ReadIndividualAttributeResponseFrame.opGroup
    opCode = Spec.ReadIndividualAttributeResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ReadIndividualAttributeResponseFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ReadIndividualAttributeResponse()
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame.SrcAddress = packet.getParamValueAsNumber("SrcAddress")
        frame.Endpoint = packet.getParamValueAsNumber("Endpoint")
        frame.ClusterID = packet.getParamValueAsNumber("ClusterID")
        frame.AttributeEnum = packet.getParamValueAsNumber("AttributeEnum")
        frame.AttributeStatus = packet.getParamValueAsNumber("AttributeStatus")
        frame.AttributeDataType = packet.getParamValueAsNumber("AttributeDataType")
        frame.SizeOfTheAttributes = packet.getParamValueAsNumber("SizeOfTheAttributes")
        frame.DataByteList = packet.getParamValueAsNumber("DataByteList")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class WriteAttributeResponseObserver(Observer):

    opGroup = Spec.WriteAttributeResponseFrame.opGroup
    opCode = Spec.WriteAttributeResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.WriteAttributeResponseFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = WriteAttributeResponse()
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame.SrcAddress = packet.getParamValueAsNumber("SrcAddress")
        frame.Endpoint = packet.getParamValueAsNumber("Endpoint")
        frame.ClusterID = packet.getParamValueAsNumber("ClusterID")
        frame.AttributeEnum = packet.getParamValueAsNumber("AttributeEnum")
        frame.AttributeStatus = packet.getParamValueAsNumber("AttributeStatus")
        frame.AttributeDataType = packet.getParamValueAsNumber("AttributeDataType")
        frame.SizeOfTheAttributes = packet.getParamValueAsNumber("SizeOfTheAttributes")
        frame.DataByteList = packet.getParamValueAsNumber("DataByteList")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ReportIndividualAttributeResponseObserver(Observer):

    opGroup = Spec.ReportIndividualAttributeResponseFrame.opGroup
    opCode = Spec.ReportIndividualAttributeResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ReportIndividualAttributeResponseFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ReportIndividualAttributeResponse()
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame.SrcAddress = packet.getParamValueAsNumber("SrcAddress")
        frame.Endpoint = packet.getParamValueAsNumber("Endpoint")
        frame.ClusterID = packet.getParamValueAsNumber("ClusterID")
        frame.AttributeEnum = packet.getParamValueAsNumber("AttributeEnum")
        frame.AttributeStatus = packet.getParamValueAsNumber("AttributeStatus")
        frame.AttributeDataType = packet.getParamValueAsNumber("AttributeDataType")
        frame.SizeOfTheAttributes = packet.getParamValueAsNumber("SizeOfTheAttributes")
        frame.DataByteList = packet.getParamValueAsNumber("DataByteList")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class DefaultResponseObserver(Observer):

    opGroup = Spec.DefaultResponseFrame.opGroup
    opCode = Spec.DefaultResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.DefaultResponseFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = DefaultResponse()
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame.Endpoint = packet.getParamValueAsNumber("Endpoint")
        frame.ClusterID = packet.getParamValueAsNumber("ClusterID")
        frame.CommandID = packet.getParamValueAsNumber("CommandID")
        frame.StatusCode = packet.getParamValueAsNumber("StatusCode")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class AddGroupResponseObserver(Observer):

    opGroup = Spec.AddGroupResponseFrame.opGroup
    opCode = Spec.AddGroupResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.AddGroupResponseFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = AddGroupResponse()
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame.Endpoint = packet.getParamValueAsNumber("Endpoint")
        frame.ClusterID = packet.getParamValueAsNumber("ClusterID")
        frame.Status = packet.getParamValueAsNumber("Status")
        frame.GroupID = packet.getParamValueAsNumber("GroupID")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ViewGroupResponseObserver(Observer):

    opGroup = Spec.ViewGroupResponseFrame.opGroup
    opCode = Spec.ViewGroupResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ViewGroupResponseFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ViewGroupResponse()
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame.Endpoint = packet.getParamValueAsNumber("Endpoint")
        frame.ClusterID = packet.getParamValueAsNumber("ClusterID")
        frame.Status = packet.getParamValueAsNumber("Status")
        frame.GroupID = packet.getParamValueAsNumber("GroupID")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GetGroupMembershipResponseObserver(Observer):

    opGroup = Spec.GetGroupMembershipResponseFrame.opGroup
    opCode = Spec.GetGroupMembershipResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GetGroupMembershipResponseFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GetGroupMembershipResponse()
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame.Endpoint = packet.getParamValueAsNumber("Endpoint")
        frame.ClusterID = packet.getParamValueAsNumber("ClusterID")
        frame.Capacity = packet.getParamValueAsNumber("Capacity")
        frame.GroupCount = packet.getParamValueAsNumber("GroupCount")
        frame.GroupList = packet.getParamValueAsNumber("GroupList")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class RemoveGroupResponseObserver(Observer):

    opGroup = Spec.RemoveGroupResponseFrame.opGroup
    opCode = Spec.RemoveGroupResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.RemoveGroupResponseFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = RemoveGroupResponse()
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame.Endpoint = packet.getParamValueAsNumber("Endpoint")
        frame.ClusterID = packet.getParamValueAsNumber("ClusterID")
        frame.Status = packet.getParamValueAsNumber("Status")
        frame.GroupID = packet.getParamValueAsNumber("GroupID")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ViewSceneResponseObserver(Observer):

    opGroup = Spec.ViewSceneResponseFrame.opGroup
    opCode = Spec.ViewSceneResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ViewSceneResponseFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ViewSceneResponse()
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame.Endpoint = packet.getParamValueAsNumber("Endpoint")
        frame.ClusterID = packet.getParamValueAsNumber("ClusterID")
        frame.Status = packet.getParamValueAsNumber("Status")
        frame.GroupID = packet.getParamValueAsNumber("GroupID")
        frame.SceneID = packet.getParamValueAsNumber("SceneID")
        frame.TransitionTime = packet.getParamValueAsNumber("TransitionTime")
        frame.SceneNameLength = packet.getParamValueAsNumber("SceneNameLength")
        frame.SceneNameMaxLength = packet.getParamValueAsNumber("SceneNameMaxLength")
        frame.SceneNameData = packet.getParamValueAsNumber("SceneNameData")
        frame.ExtensionsLength = packet.getParamValueAsNumber("ExtensionsLength")
        frame.ExtensionsMaxLength = packet.getParamValueAsNumber("ExtensionsMaxLength")
        frame.ExtensionsData = packet.getParamValueAsNumber("ExtensionsData")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ViewEnhancedSceneResponseObserver(Observer):

    opGroup = Spec.ViewEnhancedSceneResponseFrame.opGroup
    opCode = Spec.ViewEnhancedSceneResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ViewEnhancedSceneResponseFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ViewEnhancedSceneResponse()
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame.Endpoint = packet.getParamValueAsNumber("Endpoint")
        frame.ClusterID = packet.getParamValueAsNumber("ClusterID")
        frame.Status = packet.getParamValueAsNumber("Status")
        frame.GroupID = packet.getParamValueAsNumber("GroupID")
        frame.SceneID = packet.getParamValueAsNumber("SceneID")
        frame.TransitionTime = packet.getParamValueAsNumber("TransitionTime")
        frame.SceneNameLength = packet.getParamValueAsNumber("SceneNameLength")
        frame.SceneNameMaxLength = packet.getParamValueAsNumber("SceneNameMaxLength")
        frame.SceneNameData = packet.getParamValueAsNumber("SceneNameData")
        frame.ExtensionsLength = packet.getParamValueAsNumber("ExtensionsLength")
        frame.ExtensionsMaxLength = packet.getParamValueAsNumber("ExtensionsMaxLength")
        frame.ExtensionsData = packet.getParamValueAsNumber("ExtensionsData")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class AddSceneResponseObserver(Observer):

    opGroup = Spec.AddSceneResponseFrame.opGroup
    opCode = Spec.AddSceneResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.AddSceneResponseFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = AddSceneResponse()
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame.Endpoint = packet.getParamValueAsNumber("Endpoint")
        frame.ClusterID = packet.getParamValueAsNumber("ClusterID")
        frame.Status = packet.getParamValueAsNumber("Status")
        frame.GroupID = packet.getParamValueAsNumber("GroupID")
        frame.SceneID = packet.getParamValueAsNumber("SceneID")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class CopySceneResponseObserver(Observer):

    opGroup = Spec.CopySceneResponseFrame.opGroup
    opCode = Spec.CopySceneResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.CopySceneResponseFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = CopySceneResponse()
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame.Endpoint = packet.getParamValueAsNumber("Endpoint")
        frame.ClusterID = packet.getParamValueAsNumber("ClusterID")
        frame.Status = packet.getParamValueAsNumber("Status")
        frame.FromGroupID = packet.getParamValueAsNumber("FromGroupID")
        frame.FromSceneID = packet.getParamValueAsNumber("FromSceneID")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class RemoveSceneResponseObserver(Observer):

    opGroup = Spec.RemoveSceneResponseFrame.opGroup
    opCode = Spec.RemoveSceneResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.RemoveSceneResponseFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = RemoveSceneResponse()
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame.Endpoint = packet.getParamValueAsNumber("Endpoint")
        frame.ClusterID = packet.getParamValueAsNumber("ClusterID")
        frame.Status = packet.getParamValueAsNumber("Status")
        frame.GroupID = packet.getParamValueAsNumber("GroupID")
        frame.SceneID = packet.getParamValueAsNumber("SceneID")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class RemoveAllSceneResponseObserver(Observer):

    opGroup = Spec.RemoveAllSceneResponseFrame.opGroup
    opCode = Spec.RemoveAllSceneResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.RemoveAllSceneResponseFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = RemoveAllSceneResponse()
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame.Endpoint = packet.getParamValueAsNumber("Endpoint")
        frame.ClusterID = packet.getParamValueAsNumber("ClusterID")
        frame.Status = packet.getParamValueAsNumber("Status")
        frame.GroupID = packet.getParamValueAsNumber("GroupID")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class AddEnhancedSceneResponseObserver(Observer):

    opGroup = Spec.AddEnhancedSceneResponseFrame.opGroup
    opCode = Spec.AddEnhancedSceneResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.AddEnhancedSceneResponseFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = AddEnhancedSceneResponse()
        frame.SeqNum = packet.getParamValueAsNumber("SeqNum")
        frame.srcEndpoint = packet.getParamValueAsNumber("srcEndpoint")
        frame.ClusterId = packet.getParamValueAsNumber("ClusterId")
        frame.Status = packet.getParamValueAsNumber("Status")
        frame.GroupID = packet.getParamValueAsNumber("GroupID")
        frame.SceneID = packet.getParamValueAsNumber("SceneID")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class StoreSceneResponseObserver(Observer):

    opGroup = Spec.StoreSceneResponseFrame.opGroup
    opCode = Spec.StoreSceneResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.StoreSceneResponseFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = StoreSceneResponse()
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame.Endpoint = packet.getParamValueAsNumber("Endpoint")
        frame.ClusterID = packet.getParamValueAsNumber("ClusterID")
        frame.Status = packet.getParamValueAsNumber("Status")
        frame.GroupID = packet.getParamValueAsNumber("GroupID")
        frame.SceneID = packet.getParamValueAsNumber("SceneID")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SceneMembershipResponseObserver(Observer):

    opGroup = Spec.SceneMembershipResponseFrame.opGroup
    opCode = Spec.SceneMembershipResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SceneMembershipResponseFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SceneMembershipResponse()
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame.Endpoint = packet.getParamValueAsNumber("Endpoint")
        frame.ClusterID = packet.getParamValueAsNumber("ClusterID")
        frame.Status = packet.getParamValueAsNumber("Status")
        frame.Capacity = packet.getParamValueAsNumber("Capacity")
        frame.GroupID = packet.getParamValueAsNumber("GroupID")
        frame.SceneCount = packet.getParamValueAsNumber("SceneCount")
        frame.SceneList = packet.getParamValueAsNumber("SceneList")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class TouchlinkStatusObserver(Observer):

    opGroup = Spec.TouchlinkStatusFrame.opGroup
    opCode = Spec.TouchlinkStatusFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.TouchlinkStatusFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = TouchlinkStatus()
        frame.Status = TouchlinkStatusStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame.JoinedNodeShortAddress = packet.getParamValueAsNumber("JoinedNodeShortAddress")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ZoneStatusChangeNotificationObserver(Observer):

    opGroup = Spec.ZoneStatusChangeNotificationFrame.opGroup
    opCode = Spec.ZoneStatusChangeNotificationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ZoneStatusChangeNotificationFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ZoneStatusChangeNotification()
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame.EndPoint = packet.getParamValueAsNumber("EndPoint")
        frame.ClusteID = packet.getParamValueAsNumber("ClusteID")
        frame.SourceAddressMode = packet.getParamValueAsNumber("SourceAddressMode")
        frame.SourceAddress = packet.getParamValueAsNumber("SourceAddress")
        frame.ZoneStatus = packet.getParamValueAsNumber("ZoneStatus")
        frame.ExtendedStatus = packet.getParamValueAsNumber("ExtendedStatus")
        frame.ZoneID = packet.getParamValueAsNumber("ZoneID")
        frame.Delay = packet.getParamValueAsNumber("Delay")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class HostPersistentDataManagerAvailableResponseObserver(Observer):

    opGroup = Spec.HostPersistentDataManagerAvailableResponseFrame.opGroup
    opCode = Spec.HostPersistentDataManagerAvailableResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Create frame object
        frame = HostPersistentDataManagerAvailableResponse()
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class LoadRecordResponseObserver(Observer):

    opGroup = Spec.LoadRecordResponseFrame.opGroup
    opCode = Spec.LoadRecordResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.LoadRecordResponseFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = LoadRecordResponse()
        frame.Status = LoadRecordResponseStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame.Record = packet.getParamValueAsNumber("Record")
        frame.TotalSize = packet.getParamValueAsNumber("TotalSize")
        frame.TotalNumberOfBlocks = packet.getParamValueAsNumber("TotalNumberOfBlocks")
        frame.CurrentBlock = packet.getParamValueAsNumber("CurrentBlock")
        frame.BlockSize = packet.getParamValueAsNumber("BlockSize")
        frame.Parameters = packet.getParamValueAsNumber("Parameters")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SaveRecordResponseObserver(Observer):

    opGroup = Spec.SaveRecordResponseFrame.opGroup
    opCode = Spec.SaveRecordResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SaveRecordResponseFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SaveRecordResponse()
        frame.Record = packet.getParamValueAsNumber("Record")
        frame.TotalSize = packet.getParamValueAsNumber("TotalSize")
        frame.TotalNumberOfBlocks = packet.getParamValueAsNumber("TotalNumberOfBlocks")
        frame.CurrentBlock = packet.getParamValueAsNumber("CurrentBlock")
        frame.BlockSize = packet.getParamValueAsNumber("BlockSize")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class RouterDiscoveryConfirmObserver(Observer):

    opGroup = Spec.RouterDiscoveryConfirmFrame.opGroup
    opCode = Spec.RouterDiscoveryConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.RouterDiscoveryConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = RouterDiscoveryConfirm()
        frame.Status = packet.getParamValueAsNumber("Status")
        frame.NwkStatus = packet.getParamValueAsNumber("NwkStatus")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class APSDataConfirmFailObserver(Observer):

    opGroup = Spec.APSDataConfirmFailFrame.opGroup
    opCode = Spec.APSDataConfirmFailFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.APSDataConfirmFailFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = APSDataConfirmFail()
        frame.Status = packet.getParamValueAsNumber("Status")
        frame.SourceEndPoint = packet.getParamValueAsNumber("SourceEndPoint")
        frame.DestinationEndPoint = packet.getParamValueAsNumber("DestinationEndPoint")
        frame.DestAddressingMode = packet.getParamValueAsNumber("DestAddressingMode")
        frame.DestinationAddress = packet.getParamValueAsNumber("DestinationAddress")
        frame.SequenceNumber = packet.getParamValueAsNumber("SequenceNumber")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


allObservers = {}
observersList = [cls(clsName[:-len('Observer')]) for (clsName, cls) in inspect.getmembers(
    sys.modules[__name__], inspect.isclass) if clsName.endswith('Observer') and clsName != 'Observer']
for observer in observersList:
    allObservers[(observer.opGroup, observer.opCode)] = observer
