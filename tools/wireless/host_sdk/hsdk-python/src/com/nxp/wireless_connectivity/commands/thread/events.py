'''
* Copyright 2014-2015 Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
'''

import copy
from ctypes import cast, c_uint8, c_void_p, POINTER
from datetime import datetime
import inspect
import sys

from com.nxp.wireless_connectivity.commands.thread.frames import *  # @UnusedWildImport
from com.nxp.wireless_connectivity.commands.thread.spec import Spec as _Spec
from com.nxp.wireless_connectivity.hsdk.CFsciLibrary import FsciFrame
from com.nxp.wireless_connectivity.hsdk.library_loader import LibraryLoader
from com.nxp.wireless_connectivity.hsdk.utils import Observer, overrides, print_event


fsciLibrary = LibraryLoader().CFsciLibrary
fsciLibrary.DestroyFSCIFrame.argtypes = [c_void_p]
Spec = _Spec()


class FSCIACKObserver(Observer):

    opGroup = Spec.FSCIACKFrame.opGroup
    opCode = Spec.FSCIACKFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.FSCIACKFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = FSCIACK()
        frame.Status = FSCIACKStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SocketCreateConfirmObserver(Observer):

    opGroup = Spec.SocketCreateConfirmFrame.opGroup
    opCode = Spec.SocketCreateConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SocketCreateConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SocketCreateConfirm()
        frame.SocketIndex = packet.getParamValueAsNumber("SocketIndex")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SocketShutdownConfirmObserver(Observer):

    opGroup = Spec.SocketShutdownConfirmFrame.opGroup
    opCode = Spec.SocketShutdownConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SocketShutdownConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SocketShutdownConfirm()
        frame.Status = SocketShutdownConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SocketBindConfirmObserver(Observer):

    opGroup = Spec.SocketBindConfirmFrame.opGroup
    opCode = Spec.SocketBindConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SocketBindConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SocketBindConfirm()
        frame.Status = SocketBindConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SocketSendConfirmObserver(Observer):

    opGroup = Spec.SocketSendConfirmFrame.opGroup
    opCode = Spec.SocketSendConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SocketSendConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SocketSendConfirm()
        frame.Status = SocketSendConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SocketSendToConfirmObserver(Observer):

    opGroup = Spec.SocketSendToConfirmFrame.opGroup
    opCode = Spec.SocketSendToConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SocketSendToConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SocketSendToConfirm()
        frame.Status = SocketSendToConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SocketReceiveConfirmObserver(Observer):

    opGroup = Spec.SocketReceiveConfirmFrame.opGroup
    opCode = Spec.SocketReceiveConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SocketReceiveConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SocketReceiveConfirm()
        frame.Size = packet.getParamValueAsNumber("Size")
        frame.Data = packet.getParamValueAsList("Data")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SocketReceiveFromConfirmObserver(Observer):

    opGroup = Spec.SocketReceiveFromConfirmFrame.opGroup
    opCode = Spec.SocketReceiveFromConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SocketReceiveFromConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SocketReceiveFromConfirm()
        frame.Status = SocketReceiveFromConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame.RemoteIpAddress = packet.getParamValueAsNumber("RemoteIpAddress")
        frame.RemotePort = packet.getParamValueAsNumber("RemotePort")
        frame.Size = packet.getParamValueAsNumber("Size")
        frame.Data = packet.getParamValueAsList("Data")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SocketConnectConfirmObserver(Observer):

    opGroup = Spec.SocketConnectConfirmFrame.opGroup
    opCode = Spec.SocketConnectConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SocketConnectConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SocketConnectConfirm()
        frame.Status = SocketConnectConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SocketListenConfirmObserver(Observer):

    opGroup = Spec.SocketListenConfirmFrame.opGroup
    opCode = Spec.SocketListenConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SocketListenConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SocketListenConfirm()
        frame.Status = SocketListenConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SocketAcceptConfirmObserver(Observer):

    opGroup = Spec.SocketAcceptConfirmFrame.opGroup
    opCode = Spec.SocketAcceptConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SocketAcceptConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SocketAcceptConfirm()
        frame.Status = SocketAcceptConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame.ConnectedSocketIndex = packet.getParamValueAsNumber("ConnectedSocketIndex")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SocketSetOptionConfirmObserver(Observer):

    opGroup = Spec.SocketSetOptionConfirmFrame.opGroup
    opCode = Spec.SocketSetOptionConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SocketSetOptionConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SocketSetOptionConfirm()
        frame.Status = SocketSetOptionConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SocketGetOptionConfirmObserver(Observer):

    opGroup = Spec.SocketGetOptionConfirmFrame.opGroup
    opCode = Spec.SocketGetOptionConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SocketGetOptionConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SocketGetOptionConfirm()
        frame.Status = SocketGetOptionConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame.OptionValue = packet.getParamValueAsNumber("OptionValue")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SocketCloseConfirmObserver(Observer):

    opGroup = Spec.SocketCloseConfirmFrame.opGroup
    opCode = Spec.SocketCloseConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SocketCloseConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SocketCloseConfirm()
        frame.Status = SocketCloseConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MAC_MacFilteringAddEntryConfirmObserver(Observer):

    opGroup = Spec.MAC_MacFilteringAddEntryConfirmFrame.opGroup
    opCode = Spec.MAC_MacFilteringAddEntryConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MAC_MacFilteringAddEntryConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MAC_MacFilteringAddEntryConfirm()
        frame.Status = MAC_MacFilteringAddEntryConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MAC_MacFilteringRemoveEntryConfirmObserver(Observer):

    opGroup = Spec.MAC_MacFilteringRemoveEntryConfirmFrame.opGroup
    opCode = Spec.MAC_MacFilteringRemoveEntryConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MAC_MacFilteringRemoveEntryConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MAC_MacFilteringRemoveEntryConfirm()
        frame.Status = MAC_MacFilteringRemoveEntryConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MAC_MacFilteringEnableConfirmObserver(Observer):

    opGroup = Spec.MAC_MacFilteringEnableConfirmFrame.opGroup
    opCode = Spec.MAC_MacFilteringEnableConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MAC_MacFilteringEnableConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MAC_MacFilteringEnableConfirm()
        frame.Status = MAC_MacFilteringEnableConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MAC_MacFilteringGetTableConfirmObserver(Observer):

    opGroup = Spec.MAC_MacFilteringGetTableConfirmFrame.opGroup
    opCode = Spec.MAC_MacFilteringGetTableConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        # Create frame object
        frame = MAC_MacFilteringGetTableConfirm()
        curr = 0
        frame.InstanceId = data.contents[curr]
        curr += 1
        frame.NoOfElements = data.contents[curr]
        curr += 1
        frame.MacFilteringEntries = []
        for _ in range(frame.NoOfElements):
            # Create sub-object container
            MacFilteringEntry = MAC_MacFilteringGetTableConfirm.MacFilteringEntry()
            MacFilteringEntry.ExtendedAddress = list_to_int(data.contents[curr:curr + 8])
            curr += 8
            MacFilteringEntry.ShortAddress = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            MacFilteringEntry.LinkIndicator = data.contents[curr]
            curr += 1
            MacFilteringEntry.BlockedNeighbor = data.contents[curr]
            curr += 1
            # Add sub-object to list container.
            frame.MacFilteringEntries.append(copy.deepcopy(MacFilteringEntry))

        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
            del frame.MacFilteringEntries[:]
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_SetDeviceConfigConfirmObserver(Observer):

    opGroup = Spec.THR_SetDeviceConfigConfirmFrame.opGroup
    opCode = Spec.THR_SetDeviceConfigConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_SetDeviceConfigConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_SetDeviceConfigConfirm()
        frame.Status = THR_SetDeviceConfigConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_NwkScanConfirmObserver(Observer):

    opGroup = Spec.THR_NwkScanConfirmFrame.opGroup
    opCode = Spec.THR_NwkScanConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_NwkScanConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_NwkScanConfirm()
        frame.Status = THR_NwkScanConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_CreateNwkConfirmObserver(Observer):

    opGroup = Spec.THR_CreateNwkConfirmFrame.opGroup
    opCode = Spec.THR_CreateNwkConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_CreateNwkConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_CreateNwkConfirm()
        frame.Status = THR_CreateNwkConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_JoinConfirmObserver(Observer):

    opGroup = Spec.THR_JoinConfirmFrame.opGroup
    opCode = Spec.THR_JoinConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_JoinConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_JoinConfirm()
        frame.Status = THR_JoinConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_FactoryResetConfirmObserver(Observer):

    opGroup = Spec.THR_FactoryResetConfirmFrame.opGroup
    opCode = Spec.THR_FactoryResetConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_FactoryResetConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_FactoryResetConfirm()
        frame.Status = THR_FactoryResetConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_CpuResetConfirmObserver(Observer):

    opGroup = Spec.THR_CpuResetConfirmFrame.opGroup
    opCode = Spec.THR_CpuResetConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_CpuResetConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_CpuResetConfirm()
        frame.Status = THR_CpuResetConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_CpuResetIndicationObserver(Observer):

    opGroup = Spec.THR_CpuResetIndicationFrame.opGroup
    opCode = Spec.THR_CpuResetIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_CpuResetIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_CpuResetIndication()
        frame.Status = THR_CpuResetIndicationStatus.getEnumString(packet.getParamValueAsNumber("Status"))

        curr = 1

        if frame.Status == 'ResetCpuPending':
            frame.ResetCpuPayload = packet.getParamValueAsNumber("ResetCpuPayload")

        elif frame.Status == 'ResetCpuSuccess':
            frame.ResetCpuPayload = THR_CpuResetIndication.ResetCpuSuccess()
            frame.ResetCpuPayload.BoardNameLen = data.contents[curr]
            curr += 1
            frame.ResetCpuPayload.BoardName = ''.join(
                chr(x) for x in data.contents[curr:curr + frame.ResetCpuPayload.BoardNameLen])
            curr += frame.ResetCpuPayload.BoardNameLen
            frame.ResetCpuPayload.UniqueMcuId = list_to_int(data.contents[curr:curr + 16])
            curr += 16
            frame.ResetCpuPayload.StackVersion = list_to_int(data.contents[curr:curr + 6])
            curr += 6
            frame.ResetCpuPayload.SwVersionLen = data.contents[curr]
            curr += 1
            frame.ResetCpuPayload.SwVersion = ''.join(
                chr(x) for x in data.contents[curr:curr + frame.ResetCpuPayload.SwVersionLen])

        frame._DevicePort = self.deviceName
        framer.async_queue.put(frame)

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
            if frame.Status == 'ResetCpuSuccess':
                print_event(self.deviceName, frame.ResetCpuPayload)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_DisconnectConfirmObserver(Observer):

    opGroup = Spec.THR_DisconnectConfirmFrame.opGroup
    opCode = Spec.THR_DisconnectConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_DisconnectConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_DisconnectConfirm()
        frame.Status = THR_DisconnectConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_AttachConfirmObserver(Observer):

    opGroup = Spec.THR_AttachConfirmFrame.opGroup
    opCode = Spec.THR_AttachConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_AttachConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_AttachConfirm()
        frame.Status = THR_AttachConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_PromoteAsRouterConfirmObserver(Observer):

    opGroup = Spec.THR_PromoteAsRouterConfirmFrame.opGroup
    opCode = Spec.THR_PromoteAsRouterConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_PromoteAsRouterConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_PromoteAsRouterConfirm()
        frame.Status = THR_PromoteAsRouterConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_EventNwkScanConfirmObserver(Observer):

    opGroup = Spec.THR_EventNwkScanConfirmFrame.opGroup
    opCode = Spec.THR_EventNwkScanConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_EventNwkScanConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_EventNwkScanConfirm()
        frame.InstanceId = packet.getParamValueAsNumber("InstanceId")
        frame.EventStatus = THR_EventNwkScanConfirmEventStatus.getEnumString(
            packet.getParamValueAsNumber("EventStatus"))
        frame.DataSize = packet.getParamValueAsNumber("DataSize")
        frame.ScanChannelMask = packet.getParamValueAsNumber("ScanChannelMask")
        frame.ScanType = packet.getParamValueAsNumber("ScanType")
        frame.ScanDuration = packet.getParamValueAsNumber("ScanDuration")
        frame.maxThrNwkToDiscover = packet.getParamValueAsNumber("maxThrNwkToDiscover")
        frame.Data = packet.getParamValueAsList("Data")
        frame._DevicePort = self.deviceName
        framer.async_queue.put(frame)

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_EventNwkCreateConfirmObserver(Observer):

    opGroup = Spec.THR_EventNwkCreateConfirmFrame.opGroup
    opCode = Spec.THR_EventNwkCreateConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_EventNwkCreateConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_EventNwkCreateConfirm()
        frame.InstanceId = packet.getParamValueAsNumber("InstanceId")
        frame.EventStatus = THR_EventNwkCreateConfirmEventStatus.getEnumString(
            packet.getParamValueAsNumber("EventStatus"))
        frame.DataSize = packet.getParamValueAsNumber("DataSize")
        frame.Data = packet.getParamValueAsList("Data")
        frame._DevicePort = self.deviceName
        framer.async_queue.put(frame)

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_EventNwkJoinConfirmObserver(Observer):

    opGroup = Spec.THR_EventNwkJoinConfirmFrame.opGroup
    opCode = Spec.THR_EventNwkJoinConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_EventNwkJoinConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_EventNwkJoinConfirm()
        frame.InstanceId = packet.getParamValueAsNumber("InstanceId")
        frame.EventStatus = THR_EventNwkJoinConfirmEventStatus.getEnumString(
            packet.getParamValueAsNumber("EventStatus"))
        frame.DataSize = packet.getParamValueAsNumber("DataSize")
        frame.Data = packet.getParamValueAsList("Data")
        frame._DevicePort = self.deviceName
        framer.async_queue.put(frame)

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_EventNwkJoinSelectParentsConfirmObserver(Observer):

    opGroup = Spec.THR_EventNwkJoinSelectParentsConfirmFrame.opGroup
    opCode = Spec.THR_EventNwkJoinSelectParentsConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_EventNwkJoinSelectParentsConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_EventNwkJoinSelectParentsConfirm()
        frame.InstanceId = packet.getParamValueAsNumber("InstanceId")
        frame.EventStatus = THR_EventNwkJoinSelectParentsConfirmEventStatus.getEnumString(
            packet.getParamValueAsNumber("EventStatus"))
        frame.DataSize = packet.getParamValueAsNumber("DataSize")
        frame.Data = packet.getParamValueAsList("Data")
        frame._DevicePort = self.deviceName
        framer.async_queue.put(frame)

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_EventGeneralConfirmObserver(Observer):

    opGroup = Spec.THR_EventGeneralConfirmFrame.opGroup
    opCode = Spec.THR_EventGeneralConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_EventGeneralConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_EventGeneralConfirm()
        frame.InstanceId = packet.getParamValueAsNumber("InstanceId")
        frame.EventStatus = THR_EventGeneralConfirmEventStatus.getEnumString(
            packet.getParamValueAsNumber("EventStatus"))
        frame.DataSize = packet.getParamValueAsNumber("DataSize")
        frame.Data = packet.getParamValueAsList("Data")
        frame._DevicePort = self.deviceName
        framer.async_queue.put(frame)

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_EventNwkCommissioningIndicationObserver(Observer):

    opGroup = Spec.THR_EventNwkCommissioningIndicationFrame.opGroup
    opCode = Spec.THR_EventNwkCommissioningIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_EventNwkCommissioningIndicationFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_EventNwkCommissioningIndication()
        frame.InstanceId = packet.getParamValueAsNumber("InstanceId")
        frame.EventStatus = THR_EventNwkCommissioningIndicationEventStatus.getEnumString(
            packet.getParamValueAsNumber("EventStatus"))
        frame._DevicePort = self.deviceName
        framer.async_queue.put(frame)

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_CommissioningDiagnosticIndicationObserver(Observer):

    opGroup = Spec.THR_CommissioningDiagnosticIndicationFrame.opGroup
    opCode = Spec.THR_CommissioningDiagnosticIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_CommissioningDiagnosticIndicationFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_CommissioningDiagnosticIndication()
        frame.Direction = packet.getParamValueAsNumber("Direction")
        frame.Type = packet.getParamValueAsNumber("Type")
        frame.EUI = list_to_int(packet.getParamValueAsList("EUI"))
        frame.TlvsLength = packet.getParamValueAsNumber("TlvsLength")
        frame.TlvsBytes = packet.getParamValueAsList("TlvsBytes")[::-1]
        frame._DevicePort = self.deviceName
        framer.async_queue.put(frame)

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_MgmtDiagnosticGetConfirmObserver(Observer):

    opGroup = Spec.THR_MgmtDiagnosticGetConfirmFrame.opGroup
    opCode = Spec.THR_MgmtDiagnosticGetConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_MgmtDiagnosticGetConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_MgmtDiagnosticGetConfirm()
        frame.Status = THR_MgmtDiagnosticGetConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame.CoapMsgId = packet.getParamValueAsNumber("CoapMsgId")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_DiagTestGetConfirmObserver(Observer):

    opGroup = Spec.THR_DiagTestGetConfirmFrame.opGroup
    opCode = Spec.THR_DiagTestGetConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_DiagTestGetConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_DiagTestGetConfirm()
        frame.Status = THR_DiagTestGetConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame.CoapMsgId = packet.getParamValueAsNumber("CoapMsgId")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_MgmtDiagnosticResetConfirmObserver(Observer):

    opGroup = Spec.THR_MgmtDiagnosticResetConfirmFrame.opGroup
    opCode = Spec.THR_MgmtDiagnosticResetConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_MgmtDiagnosticResetConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_MgmtDiagnosticResetConfirm()
        frame.Status = THR_MgmtDiagnosticResetConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame.CoapMsgId = packet.getParamValueAsNumber("CoapMsgId")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_MgmtReadMemoryConfirmObserver(Observer):

    opGroup = Spec.THR_MgmtReadMemoryConfirmFrame.opGroup
    opCode = Spec.THR_MgmtReadMemoryConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_MgmtReadMemoryConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_MgmtReadMemoryConfirm()
        frame.Memory = packet.getParamValueAsList("Memory")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_MgmtWriteMemoryConfirmObserver(Observer):

    opGroup = Spec.THR_MgmtWriteMemoryConfirmFrame.opGroup
    opCode = Spec.THR_MgmtWriteMemoryConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_MgmtWriteMemoryConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_MgmtWriteMemoryConfirm()
        frame.Status = THR_MgmtWriteMemoryConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_SetManualSlaacIIDConfirmObserver(Observer):

    opGroup = Spec.THR_SetManualSlaacIIDConfirmFrame.opGroup
    opCode = Spec.THR_SetManualSlaacIIDConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_SetManualSlaacIIDConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_SetManualSlaacIIDConfirm()
        frame.Status = THR_SetManualSlaacIIDConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_SetParentPriorityConfirmObserver(Observer):

    opGroup = Spec.THR_SetParentPriorityConfirmFrame.opGroup
    opCode = Spec.THR_SetParentPriorityConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_SetParentPriorityConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_SetParentPriorityConfirm()
        frame.Status = THR_SetParentPriorityConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_SendProactiveAddrNotifConfirmObserver(Observer):

    opGroup = Spec.THR_SendProactiveAddrNotifConfirmFrame.opGroup
    opCode = Spec.THR_SendProactiveAddrNotifConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_SendProactiveAddrNotifConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_SendProactiveAddrNotifConfirm()
        frame.Status = THR_SendProactiveAddrNotifConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_NwkDiscoveryConfirmObserver(Observer):

    opGroup = Spec.THR_NwkDiscoveryConfirmFrame.opGroup
    opCode = Spec.THR_NwkDiscoveryConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_NwkDiscoveryConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_NwkDiscoveryConfirm()
        frame.Status = THR_NwkDiscoveryConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_NwkDiscoveryStopConfirmObserver(Observer):

    opGroup = Spec.THR_NwkDiscoveryStopConfirmFrame.opGroup
    opCode = Spec.THR_NwkDiscoveryStopConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_NwkDiscoveryStopConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_NwkDiscoveryStopConfirm()
        frame.Status = THR_NwkDiscoveryStopConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_SearchNwkWithAnounceConfirmObserver(Observer):

    opGroup = Spec.THR_SearchNwkWithAnounceConfirmFrame.opGroup
    opCode = Spec.THR_SearchNwkWithAnounceConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_SearchNwkWithAnounceConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_SearchNwkWithAnounceConfirm()
        frame.Status = THR_SearchNwkWithAnounceConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_GetDeviceInfoConfirmObserver(Observer):

    opGroup = Spec.THR_GetDeviceInfoConfirmFrame.opGroup
    opCode = Spec.THR_GetDeviceInfoConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_GetDeviceInfoConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_GetDeviceInfoConfirm()
        frame.InstanceId = packet.getParamValueAsNumber("InstanceId")
        frame.NwkCapabilities.CanCreateNewNetwork = packet.getParamValueAsNumber("NwkCapabilitiesCanCreateNewNetwork")
        frame.NwkCapabilities.CanBecomeActiveRouter = packet.getParamValueAsNumber(
            "NwkCapabilitiesCanBecomeActiveRouter")
        frame.NwkCapabilities.IsPollingEndDevice = packet.getParamValueAsNumber("NwkCapabilitiesIsPollingEndDevice")
        frame.NwkCapabilities.IsFullThreadDevice = packet.getParamValueAsNumber("NwkCapabilitiesIsFullThreadDevice")
        frame.ExtendedNwkCapabilities.IsBrDev = packet.getParamValueAsNumber("ExtendedNwkCapabilitiesIsBrDev")
        frame.ExtendedNwkCapabilities.IsNdRouterBr = packet.getParamValueAsNumber("ExtendedNwkCapabilitiesIsNdRouterBr")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_MgmtDiagnosticGetRspIndicationObserver(Observer):

    opGroup = Spec.THR_MgmtDiagnosticGetRspIndicationFrame.opGroup
    opCode = Spec.THR_MgmtDiagnosticGetRspIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        # Create frame object
        frame = THR_MgmtDiagnosticGetRspIndication()
        curr = 0
        frame.Status = THR_MgmtDiagnosticGetRspIndicationStatus.getEnumString(data.contents[curr])
        curr += 1
        frame.CoapMsgId = list_to_int(data.contents[curr: curr + 2])
        curr += 2
        frame.DataLen = list_to_int(data.contents[curr: curr + 2])
        curr += 2
        frame.NoOfTLVs = list_to_int(data.contents[curr: curr + 1])
        curr += 1
        frame.TLVs = []

        while curr < frame.DataLen + 6:
            TLV = THR_MgmtDiagnosticGetRspIndication.TLV()
            TLV.type = THR_MgmtDiagnosticGetRspIndicationTlvId.getEnumString(data.contents[curr])
            curr += 1
            TLV.length = data.contents[curr]
            curr += 1
            if TLV.length != 0:
                TLV.value = hex(list_to_int(data.contents[curr:curr + TLV.length]))
                TLV.raw_value = data.contents[curr:curr + TLV.length]
                TLV.strvalue = ''.join(chr(i) for i in data.contents[curr:curr + TLV.length])
            else:
                TLV.value, TLV.raw_value, TLV.strvalue = None, [], ""
            curr += TLV.length
            frame.TLVs.append(copy.deepcopy(TLV))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_DiagTestGetRspIndicationObserver(Observer):

    opGroup = Spec.THR_DiagTestGetRspIndicationFrame.opGroup
    opCode = Spec.THR_DiagTestGetRspIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_DiagTestGetRspIndicationFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_DiagTestGetRspIndication()
        frame.Status = THR_DiagTestGetRspIndicationStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame.CoapMsgId = packet.getParamValueAsNumber("CoapMsgId")
        frame.DataLen = packet.getParamValueAsNumber("DataLen")
        frame.NoOfTLVs = packet.getParamValueAsNumber("NoOfTLVs")
        frame.Payload_TlvId = packet.getParamValueAsNumber("Payload_TlvId")

        curr = 7
        if frame.Payload_TlvId == THR_DiagTestGetRspIndicationPayload_TlvId.Data:
            frame.Payload_TLVPayload = THR_DiagTestGetRspIndication.Data()

        if frame.Payload_TlvId == THR_DiagTestGetRspIndicationPayload_TlvId.Results:
            frame.Payload_TLVPayload = THR_DiagTestGetRspIndication.Results()

        frame.Payload_TLVPayload.ReqLatency = list_to_int(data.contents[curr: curr + 4])
        curr += 4
        frame.Payload_TLVPayload.RspLatency = list_to_int(data.contents[curr: curr + 4])
        curr += 4
        frame.Payload_TLVPayload.Offset = list_to_int(data.contents[curr: curr + 4])
        curr += 4
        frame.Payload_TLVPayload.SequenceNumber = list_to_int(data.contents[curr: curr + 1])
        curr += 1

        if frame.Payload_TlvId == THR_DiagTestGetRspIndicationPayload_TlvId.Data:
            frame.Payload_TLVPayload.PayloadSize = list_to_int(data.contents[curr: curr + 1])
            curr += 1
            frame.Payload_TLVPayload.Payload = data.contents[
                curr:curr + frame.Payload_TLVPayload.PayloadSize]
            curr += frame.Payload_TLVPayload.PayloadSize
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
            if frame.Payload_TlvId == THR_DiagTestGetRspIndicationPayload_TlvId.Data or frame.Payload_TlvId == THR_DiagTestGetRspIndicationPayload_TlvId.Results:
                print_event(self.deviceName, frame.Payload_TLVPayload)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_MgmtDiagnosticDiagTestEventIndicationObserver(Observer):

    opGroup = Spec.THR_MgmtDiagnosticDiagTestEventIndicationFrame.opGroup
    opCode = Spec.THR_MgmtDiagnosticDiagTestEventIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_MgmtDiagnosticDiagTestEventIndicationFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_MgmtDiagnosticDiagTestEventIndication()
        frame.Data_ReqLatency = packet.getParamValueAsNumber("Data_ReqLatency")
        frame.Data_SequenceNumber = packet.getParamValueAsNumber("Data_SequenceNumber")
        frame._DevicePort = self.deviceName
        framer.async_queue.put(frame)

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_MgmtDiagnosticResetRspIndicationObserver(Observer):

    opGroup = Spec.THR_MgmtDiagnosticResetRspIndicationFrame.opGroup
    opCode = Spec.THR_MgmtDiagnosticResetRspIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_MgmtDiagnosticResetRspIndicationFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_MgmtDiagnosticResetRspIndication()
        frame.Status = THR_MgmtDiagnosticResetRspIndicationStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame.CoapMsgId = packet.getParamValueAsNumber("CoapMsgId")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_SetNwkIdTimeoutConfirmObserver(Observer):

    opGroup = Spec.THR_SetNwkIdTimeoutConfirmFrame.opGroup
    opCode = Spec.THR_SetNwkIdTimeoutConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_SetNwkIdTimeoutConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_SetNwkIdTimeoutConfirm()
        frame.Status = THR_SetNwkIdTimeoutConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_SetRouterPromoteMaxJitterConfirmObserver(Observer):

    opGroup = Spec.THR_SetRouterPromoteMaxJitterConfirmFrame.opGroup
    opCode = Spec.THR_SetRouterPromoteMaxJitterConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_SetRouterPromoteMaxJitterConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_SetRouterPromoteMaxJitterConfirm()
        frame.Status = THR_SetRouterPromoteMaxJitterConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_SetThresholdConfirmObserver(Observer):

    opGroup = Spec.THR_SetThresholdConfirmFrame.opGroup
    opCode = Spec.THR_SetThresholdConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_SetThresholdConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_SetThresholdConfirm()
        frame.Status = THR_SetThresholdConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_SetThreshold32ConfirmObserver(Observer):

    opGroup = Spec.THR_SetThreshold32ConfirmFrame.opGroup
    opCode = Spec.THR_SetThreshold32ConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_SetThreshold32ConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_SetThreshold32Confirm()
        frame.Status = THR_SetThreshold32ConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_GetNeighborInfoConfirmObserver(Observer):

    opGroup = Spec.THR_GetNeighborInfoConfirmFrame.opGroup
    opCode = Spec.THR_GetNeighborInfoConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_GetNeighborInfoConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_GetNeighborInfoConfirm()
        frame.Status = packet.getParamValueAsNumber("Status")

        if frame.Status == THR_GetNeighborInfoConfirmStatus.Success:
            # Create sub-object container
            frame.NeighborInfo = THR_GetNeighborInfoConfirm.NeighborInfo()
            frame.NeighborInfo.ExtendedAddress = packet.getParamValueAsList(
                "NeighborInfoExtendedAddress")
            frame.NeighborInfo.ShortAddress = packet.getParamValueAsNumber(
                "NeighborInfoShortAddress")
            frame.NeighborInfo.LastCommTime = packet.getParamValueAsNumber(
                "NeighborInfoLastCommTime")
            frame.NeighborInfo.InRSSI = packet.getParamValueAsNumber("NeighborInfoInRSSI")
            frame.NeighborInfo.Timeoutsec = packet.getParamValueAsNumber("NeighborInfoTimeoutsec")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_GetChildrenTableConfirmObserver(Observer):

    opGroup = Spec.THR_GetChildrenTableConfirmFrame.opGroup
    opCode = Spec.THR_GetChildrenTableConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        # Create frame object
        frame = THR_GetChildrenTableConfirm()
        curr = 0
        frame.InstanceId = data.contents[curr]
        curr += 1
        frame.NoOfElements = data.contents[curr]
        curr += 1
        frame.NeighborEntries = []
        for _ in range(frame.NoOfElements):
            # Create sub-object container
            NeighborEntry = THR_GetChildrenTableConfirm.NeighborEntry()
            NeighborEntry.ExtendedAddress = list_to_int(data.contents[curr:curr + 8])
            curr += 8
            NeighborEntry.ShortAddress = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            NeighborEntry.LastCommTime = list_to_int(data.contents[curr:curr + 4])
            curr += 4
            NeighborEntry.LastRSSI = data.contents[curr]
            curr += 1
            NeighborEntry.Timeout = list_to_int(data.contents[curr:curr + 4])
            curr += 4
            # Add sub-object to list container.
            frame.NeighborEntries.append(copy.deepcopy(NeighborEntry))

        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
            del frame.NeighborEntries[:]
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_GetNeighborTableConfirmObserver(Observer):

    opGroup = Spec.THR_GetNeighborTableConfirmFrame.opGroup
    opCode = Spec.THR_GetNeighborTableConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        # Create frame object
        frame = THR_GetNeighborTableConfirm()
        curr = 0
        frame.InstanceId = data.contents[curr]
        curr += 1
        frame.NoOfElements = data.contents[curr]
        curr += 1
        frame.NeighborEntries = []
        for _ in range(frame.NoOfElements):
            # Create sub-object container
            NeighborEntry = THR_GetNeighborTableConfirm.NeighborEntry()
            NeighborEntry.ExtendedAddress = list_to_int(data.contents[curr:curr + 8])
            curr += 8
            NeighborEntry.ShortAddress = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            NeighborEntry.LastCommTime = list_to_int(data.contents[curr:curr + 4])
            curr += 4
            NeighborEntry.LastRSSI = data.contents[curr]
            curr += 1
            # Add sub-object to list container.
            frame.NeighborEntries.append(copy.deepcopy(NeighborEntry))

        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
            del frame.NeighborEntries[:]
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_GetRoutingTableConfirmObserver(Observer):

    opGroup = Spec.THR_GetRoutingTableConfirmFrame.opGroup
    opCode = Spec.THR_GetRoutingTableConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        # Create frame object
        frame = THR_GetRoutingTableConfirm()
        curr = 0
        frame.NoOfElements = data.contents[curr]
        curr += 1
        frame.IdSequenceNb = data.contents[curr]
        curr += 1
        frame.RouterIDMask = list_to_int(data.contents[curr:curr + 8])
        curr += 8
        frame.RoutingEntries = []
        for _ in range(frame.NoOfElements):
            # Create sub-object container
            RoutingEntry = THR_GetRoutingTableConfirm.RoutingEntry()
            RoutingEntry.RouterID = data.contents[curr]
            curr += 1
            RoutingEntry.ShortAddress = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            RoutingEntry.NextHop = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            RoutingEntry.Cost = data.contents[curr]
            curr += 1
            RoutingEntry.nIn = data.contents[curr]
            curr += 1
            RoutingEntry.nOut = data.contents[curr]
            curr += 1
            # Add sub-object to list container.
            frame.RoutingEntries.append(copy.deepcopy(RoutingEntry))

        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
            del frame.RoutingEntries[:]
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_GetAttrConfirmObserver(Observer):

    opGroup = Spec.THR_GetAttrConfirmFrame.opGroup
    opCode = Spec.THR_GetAttrConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_GetAttrConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_GetAttrConfirm()
        frame.InstanceId = packet.getParamValueAsNumber("InstanceId")
        frame.AttributeId = packet.getParamValueAsNumber("AttributeId")
        frame.Index = packet.getParamValueAsNumber("Index")
        frame.Status = THR_GetAttrConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame.AttrSize = packet.getParamValueAsNumber("AttrSize")

        if frame.AttrSize == 1:
            frame.AttributeValue = packet.getParamValueAsNumber("AttributeValue")
        else:
            frame.AttributeValue = packet.getParamValueAsList("AttributeValue")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_SetAttrConfirmObserver(Observer):

    opGroup = Spec.THR_SetAttrConfirmFrame.opGroup
    opCode = Spec.THR_SetAttrConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_SetAttrConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_SetAttrConfirm()
        frame.Status = THR_SetAttrConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_GetThreadIpAddrConfirmObserver(Observer):

    opGroup = Spec.THR_GetThreadIpAddrConfirmFrame.opGroup
    opCode = Spec.THR_GetThreadIpAddrConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_GetThreadIpAddrConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_GetThreadIpAddrConfirm()
        frame.InstanceId = packet.getParamValueAsNumber("InstanceId")
        frame.Status = THR_GetThreadIpAddrConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame.AddressType = packet.getParamValueAsNumber("AddressType")
        frame.NoOfIpAddr = packet.getParamValueAsNumber("NoOfIpAddr")
        frame.AddressList = packet.getParamValueAsList("AddressList")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_GetParentConfirmObserver(Observer):

    opGroup = Spec.THR_GetParentConfirmFrame.opGroup
    opCode = Spec.THR_GetParentConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_GetParentConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_GetParentConfirm()
        frame.Status = THR_GetParentConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame.InstanceId = packet.getParamValueAsNumber("InstanceId")
        frame.ShortAddress = packet.getParamValueAsNumber("ShortAddress")
        frame.ExtendedAddress = packet.getParamValueAsNumber("ExtendedAddress")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_ChildUpdateToParentConfirmObserver(Observer):

    opGroup = Spec.THR_ChildUpdateToParentConfirmFrame.opGroup
    opCode = Spec.THR_ChildUpdateToParentConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_ChildUpdateToParentConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_ChildUpdateToParentConfirm()
        frame.Status = THR_ChildUpdateToParentConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_LeaderRemoveRouterIdConfirmObserver(Observer):

    opGroup = Spec.THR_LeaderRemoveRouterIdConfirmFrame.opGroup
    opCode = Spec.THR_LeaderRemoveRouterIdConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_LeaderRemoveRouterIdConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_LeaderRemoveRouterIdConfirm()
        frame.Status = THR_LeaderRemoveRouterIdConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_GenerateAllKeysConfirmObserver(Observer):

    opGroup = Spec.THR_GenerateAllKeysConfirmFrame.opGroup
    opCode = Spec.THR_GenerateAllKeysConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_GenerateAllKeysConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_GenerateAllKeysConfirm()
        frame.Status = THR_GenerateAllKeysConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_SwitchKeyKeyConfirmObserver(Observer):

    opGroup = Spec.THR_SwitchKeyKeyConfirmFrame.opGroup
    opCode = Spec.THR_SwitchKeyKeyConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_SwitchKeyKeyConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_SwitchKeyKeyConfirm()
        frame.Status = THR_SwitchKeyKeyConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_BrPrefixAddEntryConfirmObserver(Observer):

    opGroup = Spec.THR_BrPrefixAddEntryConfirmFrame.opGroup
    opCode = Spec.THR_BrPrefixAddEntryConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_BrPrefixAddEntryConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_BrPrefixAddEntryConfirm()
        frame.Status = THR_BrPrefixAddEntryConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_BrPrefixGetTableConfirmObserver(Observer):

    opGroup = Spec.THR_BrPrefixGetTableConfirmFrame.opGroup
    opCode = Spec.THR_BrPrefixGetTableConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        # Create frame object
        frame = THR_BrPrefixGetTableConfirm()
        curr = 0
        frame.InstanceId = data.contents[curr]
        curr += 1
        frame.NoOfElements = data.contents[curr]
        curr += 1
        frame.BrPrefixEntries = []
        for _ in range(frame.NoOfElements):
            # Create sub-object container
            BrPrefixEntry = THR_BrPrefixGetTableConfirm.BrPrefixEntry()
            BrPrefixEntry.prefixLength = data.contents[curr]
            curr += 1
            BrPrefixEntry.PrefixValue = data.contents[curr:curr + 16]
            curr += 16
            BrPrefixEntry.PrefixFlagsReserved = data.contents[curr]
            curr += 1
            BrPrefixEntry.PrefixFlags = data.contents[curr]
            curr += 1
            BrPrefixEntry.prefixLifetime = list_to_int(data.contents[curr:curr + 4])
            curr += 4
            BrPrefixEntry.prefixAdvertised = data.contents[curr]
            curr += 1
            BrPrefixEntry.ExternalRouteFlags = data.contents[curr]
            curr += 1
            BrPrefixEntry.ExternalRouteLifetime = list_to_int(data.contents[curr:curr + 4])
            curr += 4
            BrPrefixEntry.ExternalRouteAdvertised = data.contents[curr]
            curr += 1
            # Add sub-object to list container.
            frame.BrPrefixEntries.append(copy.deepcopy(BrPrefixEntry))

        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
            del frame.BrPrefixEntries[:]
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_BrPrefixRemoveEntryConfirmObserver(Observer):

    opGroup = Spec.THR_BrPrefixRemoveEntryConfirmFrame.opGroup
    opCode = Spec.THR_BrPrefixRemoveEntryConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_BrPrefixRemoveEntryConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_BrPrefixRemoveEntryConfirm()
        frame.Status = THR_BrPrefixRemoveEntryConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_BrServiceRemoveEntryConfirmObserver(Observer):

    opGroup = Spec.THR_BrServiceRemoveEntryConfirmFrame.opGroup
    opCode = Spec.THR_BrServiceRemoveEntryConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_BrServiceRemoveEntryConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_BrServiceRemoveEntryConfirm()
        frame.Status = THR_BrServiceRemoveEntryConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_BrServiceAddEntryConfirmObserver(Observer):

    opGroup = Spec.THR_BrServiceAddEntryConfirmFrame.opGroup
    opCode = Spec.THR_BrServiceAddEntryConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_BrServiceAddEntryConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_BrServiceAddEntryConfirm()
        frame.Status = THR_BrServiceAddEntryConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_BrPrefixSyncConfirmObserver(Observer):

    opGroup = Spec.THR_BrPrefixSyncConfirmFrame.opGroup
    opCode = Spec.THR_BrPrefixSyncConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_BrPrefixSyncConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_BrPrefixSyncConfirm()
        frame.Status = THR_BrPrefixSyncConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_BrPrefixRemoveAllConfirmObserver(Observer):

    opGroup = Spec.THR_BrPrefixRemoveAllConfirmFrame.opGroup
    opCode = Spec.THR_BrPrefixRemoveAllConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_BrPrefixRemoveAllConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_BrPrefixRemoveAllConfirm()
        frame.Status = THR_BrPrefixRemoveAllConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class NWKU_EidToRlocMapResponseObserver(Observer):

    opGroup = Spec.NWKU_EidToRlocMapResponseFrame.opGroup
    opCode = Spec.NWKU_EidToRlocMapResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        # Create frame object
        frame = NWKU_EidToRlocMapResponse()
        curr = 0
        frame.NumberOfEntries = data.contents[curr]
        curr += 1
        frame.CacheEntries = []
        for _ in range(frame.NumberOfEntries):
            # Create sub-object container
            CacheEntry = NWKU_EidToRlocMapResponse.CacheEntry()
            CacheEntry.EID = list_to_int(data.contents[curr:curr + 16])
            curr += 16
            CacheEntry.RLOC = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            # Add sub-object to list container.
            frame.CacheEntries.append(copy.deepcopy(CacheEntry))

        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class THR_IdentifyConfirmObserver(Observer):

    opGroup = Spec.THR_IdentifyConfirmFrame.opGroup
    opCode = Spec.THR_IdentifyConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.THR_IdentifyConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = THR_IdentifyConfirm()
        frame.Status = THR_IdentifyConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class NWKU_IfconfigBindConfirmObserver(Observer):

    opGroup = Spec.NWKU_IfconfigBindConfirmFrame.opGroup
    opCode = Spec.NWKU_IfconfigBindConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.NWKU_IfconfigBindConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = NWKU_IfconfigBindConfirm()
        frame.Status = NWKU_IfconfigBindConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class NWKU_IfconfigAllResponseObserver(Observer):

    opGroup = Spec.NWKU_IfconfigAllResponseFrame.opGroup
    opCode = Spec.NWKU_IfconfigAllResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        # Create frame object
        frame = NWKU_IfconfigAllResponse()
        curr = 0
        frame.CountInterfaces = data.contents[curr]
        curr += 1
        frame.InterfaceList = []
        for _ in range(frame.CountInterfaces):
            # Create sub-object container
            interface = NWKU_IfconfigAllResponse.InterfaceID()
            interface.InterfaceID = data.contents[curr]
            curr += 1
            interface.CountIpAddresses = data.contents[curr]
            curr += 1
            for _ in range(interface.CountIpAddresses):
                address_bytes = data.contents[curr:curr + 16]
                curr += 16
                address_hex = ['%02x' % byte for byte in address_bytes[::-1]]
                address = ":".join(address_hex[i] + address_hex[i + 1] for i in range(0, 16, 2))
                interface.Addresses.append(address)
            # Add sub-object to list container.
            frame.InterfaceList.append(copy.deepcopy(interface))
            del interface.Addresses[:]
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
            del frame.InterfaceList[:]
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class NWKU_PingConfirmObserver(Observer):

    opGroup = Spec.NWKU_PingConfirmFrame.opGroup
    opCode = Spec.NWKU_PingConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.NWKU_PingConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = NWKU_PingConfirm()
        frame.Status = NWKU_PingConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame.Interval = packet.getParamValueAsNumber("Interval")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class NWKU_EchoUDPConfirmObserver(Observer):

    opGroup = Spec.NWKU_EchoUDPConfirmFrame.opGroup
    opCode = Spec.NWKU_EchoUDPConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.NWKU_EchoUDPConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = NWKU_EchoUDPConfirm()
        frame.Status = NWKU_EchoUDPConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class NWKU_CoapMsgReceivedIndicationObserver(Observer):

    opGroup = Spec.NWKU_CoapMsgReceivedIndicationFrame.opGroup
    opCode = Spec.NWKU_CoapMsgReceivedIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.NWKU_CoapMsgReceivedIndicationFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = NWKU_CoapMsgReceivedIndication()
        frame.Status = NWKU_CoapMsgReceivedIndicationStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame.RemoteIpAddress = packet.getParamValueAsList("RemoteIpAddress")
        frame.UDPPort = packet.getParamValueAsNumber("UDPPort")
        frame.RequestType = packet.getParamValueAsNumber("RequestType")
        frame.MessageType = packet.getParamValueAsNumber("MessageType")
        packet.getParamValueAsNumber("URIpath")
        frame.PayloadLength = packet.getParamValueAsNumber("PayloadLength")
        frame.Payload = ''.join([chr(x) for x in packet.getParamValueAsList("Payload")][::-1])
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class NWKU_DnsResponseReceivedIndicationObserver(Observer):

    opGroup = Spec.NWKU_DnsResponseReceivedIndicationFrame.opGroup
    opCode = Spec.NWKU_DnsResponseReceivedIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.NWKU_DnsResponseReceivedIndicationFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = NWKU_DnsResponseReceivedIndication()
        frame.IpAddress = packet.getParamValueAsNumber("IpAddress")
        frame.DomainName = packet.getParamValueAsNumber("DomainName")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class NWKU_CoapRegisterConfirmObserver(Observer):

    opGroup = Spec.NWKU_CoapRegisterConfirmFrame.opGroup
    opCode = Spec.NWKU_CoapRegisterConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.NWKU_CoapRegisterConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = NWKU_CoapRegisterConfirm()
        frame.Status = NWKU_CoapRegisterConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class NWKU_CoapCreateInstanceConfirmObserver(Observer):

    opGroup = Spec.NWKU_CoapCreateInstanceConfirmFrame.opGroup
    opCode = Spec.NWKU_CoapCreateInstanceConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.NWKU_CoapCreateInstanceConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = NWKU_CoapCreateInstanceConfirm()
        frame.Status = NWKU_CoapCreateInstanceConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class NWKU_CoapSendConfirmObserver(Observer):

    opGroup = Spec.NWKU_CoapSendConfirmFrame.opGroup
    opCode = Spec.NWKU_CoapSendConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.NWKU_CoapSendConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = NWKU_CoapSendConfirm()
        frame.Status = NWKU_CoapSendConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class NWKU_DnsSendRequestConfirmObserver(Observer):

    opGroup = Spec.NWKU_DnsSendRequestConfirmFrame.opGroup
    opCode = Spec.NWKU_DnsSendRequestConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.NWKU_DnsSendRequestConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = NWKU_DnsSendRequestConfirm()
        frame.Status = NWKU_DnsSendRequestConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class NWKU_McastGroupManageConfirmObserver(Observer):

    opGroup = Spec.NWKU_McastGroupManageConfirmFrame.opGroup
    opCode = Spec.NWKU_McastGroupManageConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.NWKU_McastGroupManageConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = NWKU_McastGroupManageConfirm()
        frame.Status = NWKU_McastGroupManageConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class NWKU_McastGroupShowResponseObserver(Observer):

    opGroup = Spec.NWKU_McastGroupShowResponseFrame.opGroup
    opCode = Spec.NWKU_McastGroupShowResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.NWKU_McastGroupShowResponseFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = NWKU_McastGroupShowResponse()
        frame.CountIpAddresses = packet.getParamValueAsNumber("CountIpAddresses")
        frame.Addresses = packet.getParamValueAsList("Addresses")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class NWKU_FirewallEnableConfirmObserver(Observer):

    opGroup = Spec.NWKU_FirewallEnableConfirmFrame.opGroup
    opCode = Spec.NWKU_FirewallEnableConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.NWKU_FirewallEnableConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = NWKU_FirewallEnableConfirm()
        frame.Status = NWKU_FirewallEnableConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class NWKU_FirewallRuleAddConfirmObserver(Observer):

    opGroup = Spec.NWKU_FirewallRuleAddConfirmFrame.opGroup
    opCode = Spec.NWKU_FirewallRuleAddConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.NWKU_FirewallRuleAddConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = NWKU_FirewallRuleAddConfirm()
        frame.Status = NWKU_FirewallRuleAddConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class NWKU_FirewallRuleRemoveConfirmObserver(Observer):

    opGroup = Spec.NWKU_FirewallRuleRemoveConfirmFrame.opGroup
    opCode = Spec.NWKU_FirewallRuleRemoveConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.NWKU_FirewallRuleRemoveConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = NWKU_FirewallRuleRemoveConfirm()
        frame.Status = NWKU_FirewallRuleRemoveConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class NWKU_FirewallTableShowResponseObserver(Observer):

    opGroup = Spec.NWKU_FirewallTableShowResponseFrame.opGroup
    opCode = Spec.NWKU_FirewallTableShowResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        # Create frame object
        frame = NWKU_FirewallTableShowResponse()
        curr = 0
        frame.CountEntries = data.contents[curr]
        curr += 1
        for _ in range(frame.CountEntries):
            # Create sub-object container
            Rule = NWKU_FirewallTableShowResponse.Rule()
            Rule.RuleEntryNumber = data.contents[curr]
            curr += 1
            Rule.SourcePrefix = list_to_int(data.contents[curr:curr + 16])
            curr += 16
            Rule.DestinationPrefix = list_to_int(data.contents[curr:curr + 16])
            curr += 16
            Rule.SourcePort = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            Rule.DestinationPort = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            Rule.SourcePrefixLength = data.contents[curr]
            curr += 1
            Rule.DestinationPrefixLength = data.contents[curr]
            curr += 1
            Rule.SecurityLevel = data.contents[curr]
            curr += 1
            Rule.InterfaceID = data.contents[curr]
            curr += 1
            Rule.Rule = data.contents[curr]
            curr += 1
            # Add sub-object to list container.
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class NWKU_RoutesManageConfirmObserver(Observer):

    opGroup = Spec.NWKU_RoutesManageConfirmFrame.opGroup
    opCode = Spec.NWKU_RoutesManageConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.NWKU_RoutesManageConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = NWKU_RoutesManageConfirm()
        frame.Status = NWKU_RoutesManageConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class NWKU_RoutesShowResponseObserver(Observer):

    opGroup = Spec.NWKU_RoutesShowResponseFrame.opGroup
    opCode = Spec.NWKU_RoutesShowResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        # Create frame object
        frame = NWKU_RoutesShowResponse()
        curr = 0
        frame.CountRoutes = data.contents[curr]
        curr += 1
        for _ in range(frame.CountRoutes):
            # Create sub-object container
            Route = NWKU_RoutesShowResponse.Route()
            Route.Destination = list_to_int(data.contents[curr:curr + 16])
            curr += 16
            Route.NextHop = list_to_int(data.contents[curr:curr + 16])
            curr += 16
            Route.PrefixLength = data.contents[curr]
            curr += 1
            Route.RouteMetric = data.contents[curr]
            curr += 1
            Route.InstanceId = data.contents[curr]
            curr += 1
            # Add sub-object to list container.
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MESHCOP_StartCommissionerConfirmObserver(Observer):

    opGroup = Spec.MESHCOP_StartCommissionerConfirmFrame.opGroup
    opCode = Spec.MESHCOP_StartCommissionerConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MESHCOP_StartCommissionerConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MESHCOP_StartCommissionerConfirm()
        frame.Status = MESHCOP_StartCommissionerConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MESHCOP_StartNativeCommissionerConfirmObserver(Observer):

    opGroup = Spec.MESHCOP_StartNativeCommissionerConfirmFrame.opGroup
    opCode = Spec.MESHCOP_StartNativeCommissionerConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MESHCOP_StartNativeCommissionerConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MESHCOP_StartNativeCommissionerConfirm()
        frame.Status = MESHCOP_StartNativeCommissionerConfirmStatus.getEnumString(
            packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MESHCOP_StopCommissionerConfirmObserver(Observer):

    opGroup = Spec.MESHCOP_StopCommissionerConfirmFrame.opGroup
    opCode = Spec.MESHCOP_StopCommissionerConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MESHCOP_StopCommissionerConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MESHCOP_StopCommissionerConfirm()
        frame.Status = MESHCOP_StopCommissionerConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MESHCOP_AddExpectedJoinerConfirmObserver(Observer):

    opGroup = Spec.MESHCOP_AddExpectedJoinerConfirmFrame.opGroup
    opCode = Spec.MESHCOP_AddExpectedJoinerConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MESHCOP_AddExpectedJoinerConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MESHCOP_AddExpectedJoinerConfirm()
        frame.Status = MESHCOP_AddExpectedJoinerConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MESHCOP_GetExpectedJoinerConfirmObserver(Observer):

    opGroup = Spec.MESHCOP_GetExpectedJoinerConfirmFrame.opGroup
    opCode = Spec.MESHCOP_GetExpectedJoinerConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MESHCOP_GetExpectedJoinerConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MESHCOP_GetExpectedJoinerConfirm()
        frame.Status = MESHCOP_GetExpectedJoinerConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame.Selected = packet.getParamValueAsNumber("Selected")
        frame.PSKdSize = packet.getParamValueAsNumber("PSKdSize")
        frame.PSKd = packet.getParamValueAsNumber("PSKd")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MESHCOP_RemoveExpectedJoinerConfirmObserver(Observer):

    opGroup = Spec.MESHCOP_RemoveExpectedJoinerConfirmFrame.opGroup
    opCode = Spec.MESHCOP_RemoveExpectedJoinerConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MESHCOP_RemoveExpectedJoinerConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MESHCOP_RemoveExpectedJoinerConfirm()
        frame.Status = MESHCOP_RemoveExpectedJoinerConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MESHCOP_RemoveAllExpectedJoinersConfirmObserver(Observer):

    opGroup = Spec.MESHCOP_RemoveAllExpectedJoinersConfirmFrame.opGroup
    opCode = Spec.MESHCOP_RemoveAllExpectedJoinersConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MESHCOP_RemoveAllExpectedJoinersConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MESHCOP_RemoveAllExpectedJoinersConfirm()
        frame.Status = MESHCOP_RemoveAllExpectedJoinersConfirmStatus.getEnumString(
            packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MESHCOP_SyncSteeringDataConfirmObserver(Observer):

    opGroup = Spec.MESHCOP_SyncSteeringDataConfirmFrame.opGroup
    opCode = Spec.MESHCOP_SyncSteeringDataConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MESHCOP_SyncSteeringDataConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MESHCOP_SyncSteeringDataConfirm()
        frame.Status = MESHCOP_SyncSteeringDataConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MESHCOP_MgmtSetConfirmObserver(Observer):

    opGroup = Spec.MESHCOP_MgmtSetConfirmFrame.opGroup
    opCode = Spec.MESHCOP_MgmtSetConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MESHCOP_MgmtSetConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MESHCOP_MgmtSetConfirm()
        frame.Status = MESHCOP_MgmtSetConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MESHCOP_MgmtGetConfirmObserver(Observer):

    opGroup = Spec.MESHCOP_MgmtGetConfirmFrame.opGroup
    opCode = Spec.MESHCOP_MgmtGetConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MESHCOP_MgmtGetConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MESHCOP_MgmtGetConfirm()
        frame.Status = MESHCOP_MgmtGetConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame.Type = packet.getParamValueAsNumber("Type")
        frame.Length = packet.getParamValueAsNumber("Length")
        frame.Value = packet.getParamValueAsNumber("Value")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MESHCOP_SetCommissionerCredentialConfirmObserver(Observer):

    opGroup = Spec.MESHCOP_SetCommissionerCredentialConfirmFrame.opGroup
    opCode = Spec.MESHCOP_SetCommissionerCredentialConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MESHCOP_SetCommissionerCredentialConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MESHCOP_SetCommissionerCredentialConfirm()
        frame.Status = MESHCOP_SetCommissionerCredentialConfirmStatus.getEnumString(
            packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MESHCOP_MgmNwkFormConfirmObserver(Observer):

    opGroup = Spec.MESHCOP_MgmNwkFormConfirmFrame.opGroup
    opCode = Spec.MESHCOP_MgmNwkFormConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MESHCOP_MgmNwkFormConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MESHCOP_MgmNwkFormConfirm()
        frame.Status = MESHCOP_MgmNwkFormConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MESHCOP_MgmtCommissionerGetConfirmObserver(Observer):

    opGroup = Spec.MESHCOP_MgmtCommissionerGetConfirmFrame.opGroup
    opCode = Spec.MESHCOP_MgmtCommissionerGetConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MESHCOP_MgmtCommissionerGetConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MESHCOP_MgmtCommissionerGetConfirm()
        frame.Status = MESHCOP_MgmtCommissionerGetConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame.Length = packet.getParamValueAsNumber("Length")
        frame.TLVs = packet.getParamValueAsList("TLVs")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MESHCOP_MgmtActiveGetConfirmObserver(Observer):

    opGroup = Spec.MESHCOP_MgmtActiveGetConfirmFrame.opGroup
    opCode = Spec.MESHCOP_MgmtActiveGetConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MESHCOP_MgmtActiveGetConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MESHCOP_MgmtActiveGetConfirm()
        frame.Status = MESHCOP_MgmtActiveGetConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame.Length = packet.getParamValueAsNumber("Length")
        frame.TLVs = packet.getParamValueAsList("TLVs")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MESHCOP_MgmtPendingGetConfirmObserver(Observer):

    opGroup = Spec.MESHCOP_MgmtPendingGetConfirmFrame.opGroup
    opCode = Spec.MESHCOP_MgmtPendingGetConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MESHCOP_MgmtPendingGetConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MESHCOP_MgmtPendingGetConfirm()
        frame.Status = MESHCOP_MgmtPendingGetConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame.Length = packet.getParamValueAsNumber("Length")
        frame.TLVs = packet.getParamValueAsList("TLVs")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MESHCOP_MgmtCommissionerSetConfirmObserver(Observer):

    opGroup = Spec.MESHCOP_MgmtCommissionerSetConfirmFrame.opGroup
    opCode = Spec.MESHCOP_MgmtCommissionerSetConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MESHCOP_MgmtCommissionerSetConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MESHCOP_MgmtCommissionerSetConfirm()
        frame.Status = MESHCOP_MgmtCommissionerSetConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MESHCOP_MgmtActiveSetConfirmObserver(Observer):

    opGroup = Spec.MESHCOP_MgmtActiveSetConfirmFrame.opGroup
    opCode = Spec.MESHCOP_MgmtActiveSetConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MESHCOP_MgmtActiveSetConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MESHCOP_MgmtActiveSetConfirm()
        frame.Status = MESHCOP_MgmtActiveSetConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MESHCOP_MgmtPendingSetConfirmObserver(Observer):

    opGroup = Spec.MESHCOP_MgmtPendingSetConfirmFrame.opGroup
    opCode = Spec.MESHCOP_MgmtPendingSetConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MESHCOP_MgmtPendingSetConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MESHCOP_MgmtPendingSetConfirm()
        frame.Status = MESHCOP_MgmtPendingSetConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MESHCOP_MgmtSendPanIdQueryConfirmObserver(Observer):

    opGroup = Spec.MESHCOP_MgmtSendPanIdQueryConfirmFrame.opGroup
    opCode = Spec.MESHCOP_MgmtSendPanIdQueryConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MESHCOP_MgmtSendPanIdQueryConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MESHCOP_MgmtSendPanIdQueryConfirm()
        frame.Status = MESHCOP_MgmtSendPanIdQueryConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MESHCOP_MgmtPanIdConflictConfirmObserver(Observer):

    opGroup = Spec.MESHCOP_MgmtPanIdConflictConfirmFrame.opGroup
    opCode = Spec.MESHCOP_MgmtPanIdConflictConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MESHCOP_MgmtPanIdConflictConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MESHCOP_MgmtPanIdConflictConfirm()
        frame.Status = MESHCOP_MgmtPanIdConflictConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame.ScanChannelMask = packet.getParamValueAsNumber("ScanChannelMask")
        frame.PanId = packet.getParamValueAsNumber("PanId")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MESHCOP_MgmtSendEdScanConfirmObserver(Observer):

    opGroup = Spec.MESHCOP_MgmtSendEdScanConfirmFrame.opGroup
    opCode = Spec.MESHCOP_MgmtSendEdScanConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MESHCOP_MgmtSendEdScanConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MESHCOP_MgmtSendEdScanConfirm()
        frame.Status = MESHCOP_MgmtSendEdScanConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MESHCOP_MgmtEdReportConfirmObserver(Observer):

    opGroup = Spec.MESHCOP_MgmtEdReportConfirmFrame.opGroup
    opCode = Spec.MESHCOP_MgmtEdReportConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MESHCOP_MgmtEdReportConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MESHCOP_MgmtEdReportConfirm()
        frame.Status = MESHCOP_MgmtEdReportConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame.ScanChannelMask = packet.getParamValueAsNumber("ScanChannelMask")
        frame.Length = packet.getParamValueAsNumber("Length")
        frame.EnergyList = packet.getParamValueAsList("EnergyList")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MESHCOP_MgmtSendAnnounceBeginConfirmObserver(Observer):

    opGroup = Spec.MESHCOP_MgmtSendAnnounceBeginConfirmFrame.opGroup
    opCode = Spec.MESHCOP_MgmtSendAnnounceBeginConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MESHCOP_MgmtSendAnnounceBeginConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MESHCOP_MgmtSendAnnounceBeginConfirm()
        frame.Status = MESHCOP_MgmtSendAnnounceBeginConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class DTLSOpenConfirmObserver(Observer):

    opGroup = Spec.DTLSOpenConfirmFrame.opGroup
    opCode = Spec.DTLSOpenConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.DTLSOpenConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = DTLSOpenConfirm()
        frame.ContextIndex = packet.getParamValueAsNumber("ContextIndex")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class DTLSCloseConfirmObserver(Observer):

    opGroup = Spec.DTLSCloseConfirmFrame.opGroup
    opCode = Spec.DTLSCloseConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.DTLSCloseConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = DTLSCloseConfirm()
        frame.Status = DTLSCloseConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class DTLSClosePeerConfirmObserver(Observer):

    opGroup = Spec.DTLSClosePeerConfirmFrame.opGroup
    opCode = Spec.DTLSClosePeerConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.DTLSClosePeerConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = DTLSClosePeerConfirm()
        frame.Status = DTLSClosePeerConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class DTLSConnectConfirmObserver(Observer):

    opGroup = Spec.DTLSConnectConfirmFrame.opGroup
    opCode = Spec.DTLSConnectConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.DTLSConnectConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = DTLSConnectConfirm()
        frame.Status = DTLSConnectConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame.PeerIndex = packet.getParamValueAsNumber("PeerIndex")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class DTLSClientConnectedConfirmObserver(Observer):

    opGroup = Spec.DTLSClientConnectedConfirmFrame.opGroup
    opCode = Spec.DTLSClientConnectedConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.DTLSClientConnectedConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = DTLSClientConnectedConfirm()
        frame.Status = DTLSClientConnectedConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame.PeerIndex = packet.getParamValueAsNumber("PeerIndex")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class DTLSSendConfirmObserver(Observer):

    opGroup = Spec.DTLSSendConfirmFrame.opGroup
    opCode = Spec.DTLSSendConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.DTLSSendConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = DTLSSendConfirm()
        frame.Status = DTLSSendConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class DTLSReceiveConfirmObserver(Observer):

    opGroup = Spec.DTLSReceiveConfirmFrame.opGroup
    opCode = Spec.DTLSReceiveConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.DTLSReceiveConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = DTLSReceiveConfirm()
        frame.PeerIndex = packet.getParamValueAsNumber("PeerIndex")
        frame.Size = packet.getParamValueAsNumber("Size")
        frame.Data = packet.getParamValueAsList("Data")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class FSCIGetUniqueIdConfirmObserver(Observer):

    opGroup = Spec.FSCIGetUniqueIdConfirmFrame.opGroup
    opCode = Spec.FSCIGetUniqueIdConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.FSCIGetUniqueIdConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = FSCIGetUniqueIdConfirm()
        frame.UniqueId = packet.getParamValueAsNumber("UniqueId")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class FSCIGetMcuIdConfirmObserver(Observer):

    opGroup = Spec.FSCIGetMcuIdConfirmFrame.opGroup
    opCode = Spec.FSCIGetMcuIdConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.FSCIGetMcuIdConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = FSCIGetMcuIdConfirm()
        frame.McuId = packet.getParamValueAsNumber("McuId")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class FSCIGetSwVersionsConfirmObserver(Observer):

    opGroup = Spec.FSCIGetSwVersionsConfirmFrame.opGroup
    opCode = Spec.FSCIGetSwVersionsConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.FSCIGetSwVersionsConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = FSCIGetSwVersionsConfirm()
        frame.listSize = packet.getParamValueAsNumber("listSize")
        # support for USBKW41Z will be added later, for now return version 1.0.0
        frame.SwVersions = packet.getParamValueAsList("SwVersions")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class Sniffer_MacSetPIBAttributeConfirmObserver(Observer):

    opGroup = Spec.Sniffer_MacSetPIBAttributeConfirmFrame.opGroup
    opCode = Spec.Sniffer_MacSetPIBAttributeConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.Sniffer_MacSetPIBAttributeConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = Sniffer_MacSetPIBAttributeConfirm()
        frame.Status = Sniffer_MacSetPIBAttributeConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame.PIBAttribute = packet.getParamValueAsNumber("PIBAttribute")
        frame.DataLength = packet.getParamValueAsNumber("DataLength")
        frame.PIBAttributeValue = packet.getParamValueAsNumber("PIBAttributeValue")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class Sniffer_DetectConfirmObserver(Observer):

    opGroup = Spec.Sniffer_DetectConfirmFrame.opGroup
    opCode = Spec.Sniffer_DetectConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.Sniffer_DetectConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = Sniffer_DetectConfirm()
        frame.SnifferType = Sniffer_DetectConfirmSnifferType.getEnumString(
            packet.getParamValueAsNumber("SnifferType"))
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class MAC_PromiscuousRxIndicationObserver(Observer):

    opGroup = Spec.MAC_PromiscuousRxIndicationFrame.opGroup
    opCode = Spec.MAC_PromiscuousRxIndicationFrame.opCode
    SNIFFER_QUEUE_ENABLED = False

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, _):
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.MAC_PromiscuousRxIndicationFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = MAC_PromiscuousRxIndication()
        frame.LinkQuality = packet.getParamValueAsNumber("LinkQuality")
        frame.TimeStamp = packet.getParamValueAsNumber("TimeStamp")
        frame.msduLength = packet.getParamValueAsNumber("msduLength")
        frame.msdu = data.contents[6:]
        frame._DevicePort = self.deviceName
        timestamp = datetime.now().strftime("%H:%M:%S.%f")

        if MAC_PromiscuousRxIndicationObserver.SNIFFER_QUEUE_ENABLED:
            framer.sniffer_queue.put((timestamp, frame.msdu))
        else:
            callback(frame.msdu, timestamp)

        fsciLibrary.DestroyFSCIFrame(event)


class SerialTun_LinkIndicationConfirmObserver(Observer):

    opGroup = Spec.SerialTun_LinkIndicationConfirmFrame.opGroup
    opCode = Spec.SerialTun_LinkIndicationConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SerialTun_LinkIndicationConfirmFrame.getFsciPacketFromByteArray(
            data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SerialTun_LinkIndicationConfirm()
        frame.Status = SerialTun_LinkIndicationConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SerialTun_IPPacketReceivedConfirmObserver(Observer):

    opGroup = Spec.SerialTun_IPPacketReceivedConfirmFrame.opGroup
    opCode = Spec.SerialTun_IPPacketReceivedConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        # Create frame object
        frame = SerialTun_IPPacketReceivedConfirm()
        frame.IPpayload = data.contents[:]
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class AspSetPowerLevelConfirmObserver(Observer):

    opGroup = Spec.AspSetPowerLevelConfirmFrame.opGroup
    opCode = Spec.AspSetPowerLevelConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.AspSetPowerLevelConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = AspSetPowerLevelConfirm()
        frame.Status = AspSetPowerLevelConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class AspGetPowerLevelConfirmObserver(Observer):

    opGroup = Spec.AspGetPowerLevelConfirmFrame.opGroup
    opCode = Spec.AspGetPowerLevelConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.AspGetPowerLevelConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = AspGetPowerLevelConfirm()
        frame.Value = packet.getParamValueAsNumber("Value")
        frame._DevicePort = self.deviceName
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class DBGConfirmObserver(Observer):

    opGroup = Spec.DBGConfirmFrame.opGroup
    opCode = Spec.DBGConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.DBGConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = DBGConfirm()
        frame.Text = packet.getParamValueAsNumber("Text")
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
