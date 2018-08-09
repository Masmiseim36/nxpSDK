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

import copy
from ctypes import cast, c_uint8, c_void_p, POINTER
import inspect
import os
import re
import sys

from com.nxp.wireless_connectivity.commands.ble.frames import *  # @UnusedWildImport
from com.nxp.wireless_connectivity.commands.ble.spec import Spec as _Spec
from com.nxp.wireless_connectivity.hsdk.CFsciLibrary import FsciFrame
from com.nxp.wireless_connectivity.hsdk.library_loader import LibraryLoader
from com.nxp.wireless_connectivity.hsdk.utils import Observer, overrides, print_event


fsciLibrary = LibraryLoader().CFsciLibrary
fsciLibrary.DestroyFSCIFrame.argtypes = [c_void_p]
Spec = _Spec()

class FSCIErrorIndicationObserver(Observer):

    opGroup = Spec.FSCIErrorIndicationFrame.opGroup
    opCode = Spec.FSCIErrorIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.FSCIErrorIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = FSCIErrorIndication()
        frame.Status = FSCIErrorIndicationStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class FSCIAllowDeviceToSleepConfirmObserver(Observer):

    opGroup = Spec.FSCIAllowDeviceToSleepConfirmFrame.opGroup
    opCode = Spec.FSCIAllowDeviceToSleepConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.FSCIAllowDeviceToSleepConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = FSCIAllowDeviceToSleepConfirm()
        frame.Status = FSCIAllowDeviceToSleepConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class FSCIWakeUpIndicationObserver(Observer):

    opGroup = Spec.FSCIWakeUpIndicationFrame.opGroup
    opCode = Spec.FSCIWakeUpIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.FSCIWakeUpIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = FSCIWakeUpIndication()
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class FSCIGetWakeupReasonResponseObserver(Observer):

    opGroup = Spec.FSCIGetWakeupReasonResponseFrame.opGroup
    opCode = Spec.FSCIGetWakeupReasonResponseFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.FSCIGetWakeupReasonResponseFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = FSCIGetWakeupReasonResponse()
        frame.WakeUpReason.Value0x01 = packet.getParamValueAsNumber("WakeUpReasonValue0x01")
        frame.WakeUpReason.Value0x02 = packet.getParamValueAsNumber("WakeUpReasonValue0x02")
        frame.WakeUpReason.Value0x04 = packet.getParamValueAsNumber("WakeUpReasonValue0x04")
        frame.WakeUpReason.Value0x08 = packet.getParamValueAsNumber("WakeUpReasonValue0x08")
        frame.WakeUpReason.Value0x10 = packet.getParamValueAsNumber("WakeUpReasonValue0x10")
        frame.WakeUpReason.Value0x20 = packet.getParamValueAsNumber("WakeUpReasonValue0x20")
        frame.WakeUpReason.Value0x40 = packet.getParamValueAsNumber("WakeUpReasonValue0x40")
        frame.WakeUpReason.Value0x80 = packet.getParamValueAsNumber("WakeUpReasonValue0x80")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class HCIConfirmObserver(Observer):

    opGroup = Spec.HCIConfirmFrame.opGroup
    opCode = Spec.HCIConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.HCIConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = HCIConfirm()
        frame.Status = HCIConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class HCIEventIndicationObserver(Observer):

    opGroup = Spec.HCIEventIndicationFrame.opGroup
    opCode = Spec.HCIEventIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.HCIEventIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = HCIEventIndication()
        frame.EventLength = packet.getParamValueAsNumber("EventLength")
        frame.Event = packet.getParamValueAsList("Event")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class HCIDataIndicationObserver(Observer):

    opGroup = Spec.HCIDataIndicationFrame.opGroup
    opCode = Spec.HCIDataIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.HCIDataIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = HCIDataIndication()
        frame.DataLength = packet.getParamValueAsNumber("DataLength")
        frame.Data = packet.getParamValueAsList("Data")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class HCISynchronousDataIndicationObserver(Observer):

    opGroup = Spec.HCISynchronousDataIndicationFrame.opGroup
    opCode = Spec.HCISynchronousDataIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.HCISynchronousDataIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = HCISynchronousDataIndication()
        frame.SynchronousDataLength = packet.getParamValueAsNumber("SynchronousDataLength")
        frame.SynchronousData = packet.getParamValueAsList("SynchronousData")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class L2CAPConfirmObserver(Observer):

    opGroup = Spec.L2CAPConfirmFrame.opGroup
    opCode = Spec.L2CAPConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.L2CAPConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = L2CAPConfirm()
        frame.Status = L2CAPConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
            if frame.Status != 'gBleSuccess_c':
                print '\t[warning] L2CAPConfirm w/ Status ' + str(frame.Status)
        fsciLibrary.DestroyFSCIFrame(event)


class L2CAPAttDataIndicationObserver(Observer):

    opGroup = Spec.L2CAPAttDataIndicationFrame.opGroup
    opCode = Spec.L2CAPAttDataIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.L2CAPAttDataIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = L2CAPAttDataIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.PacketLength = packet.getParamValueAsNumber("PacketLength")
        frame.Packet = packet.getParamValueAsList("Packet")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class L2CAPSmpDataIndicationObserver(Observer):

    opGroup = Spec.L2CAPSmpDataIndicationFrame.opGroup
    opCode = Spec.L2CAPSmpDataIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.L2CAPSmpDataIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = L2CAPSmpDataIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.PacketLength = packet.getParamValueAsNumber("PacketLength")
        frame.Packet = packet.getParamValueAsList("Packet")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class L2CAPLePsmConnectionRequestIndicationObserver(Observer):

    opGroup = Spec.L2CAPLePsmConnectionRequestIndicationFrame.opGroup
    opCode = Spec.L2CAPLePsmConnectionRequestIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.L2CAPLePsmConnectionRequestIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = L2CAPLePsmConnectionRequestIndication()
        curr = 0
        frame.InformationIncluded = data.contents[curr]
        curr += 1
        frame.LeCbConnectionRequest = []
        for _ in range(frame.InformationIncluded):
            # Create sub-object container
            LeCbConnectionRequest = L2CAPLePsmConnectionRequestIndication.LeCbConnectionRequest()
            LeCbConnectionRequest.DeviceId = data.contents[curr]
            curr += 1
            LeCbConnectionRequest.LePsm = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            LeCbConnectionRequest.PeerMtu = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            LeCbConnectionRequest.PeerMps = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            LeCbConnectionRequest.InitialCredits = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            # Add sub-object to list container.
            frame.LeCbConnectionRequest.append(copy.deepcopy(LeCbConnectionRequest))

        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class L2CAPLePsmConnectionCompleteIndicationObserver(Observer):

    opGroup = Spec.L2CAPLePsmConnectionCompleteIndicationFrame.opGroup
    opCode = Spec.L2CAPLePsmConnectionCompleteIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.L2CAPLePsmConnectionCompleteIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = L2CAPLePsmConnectionCompleteIndication()
        curr = 0
        frame.InformationIncluded = data.contents[curr]
        curr += 1
        frame.LeCbConnectionComplete = []
        for _ in range(frame.InformationIncluded):
            # Create sub-object container
            LeCbConnectionComplete = L2CAPLePsmConnectionCompleteIndication.LeCbConnectionComplete()
            LeCbConnectionComplete.DeviceId = data.contents[curr]
            curr += 1
            LeCbConnectionComplete.ChannelId = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            LeCbConnectionComplete.PeerMtu = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            LeCbConnectionComplete.PeerMps = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            LeCbConnectionComplete.InitialCredits = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            LeCbConnectionComplete.Result = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            # Add sub-object to list container.
            frame.LeCbConnectionComplete.append(copy.deepcopy(LeCbConnectionComplete))

        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class L2CAPLePsmDisconnectNotificationIndicationObserver(Observer):

    opGroup = Spec.L2CAPLePsmDisconnectNotificationIndicationFrame.opGroup
    opCode = Spec.L2CAPLePsmDisconnectNotificationIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.L2CAPLePsmDisconnectNotificationIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = L2CAPLePsmDisconnectNotificationIndication()
        curr = 0
        frame.InformationIncluded = data.contents[curr]
        curr += 1
        frame.LeCbDisconnection = []
        for _ in range(frame.InformationIncluded):
            # Create sub-object container
            LeCbDisconnection = L2CAPLePsmDisconnectNotificationIndication.LeCbDisconnection()
            LeCbDisconnection.DeviceId = data.contents[curr]
            curr += 1
            LeCbDisconnection.ChannelId = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            # Add sub-object to list container.
            frame.LeCbDisconnection.append(copy.deepcopy(LeCbDisconnection))

        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class L2CAPNoPeerCreditsIndicationObserver(Observer):

    opGroup = Spec.L2CAPNoPeerCreditsIndicationFrame.opGroup
    opCode = Spec.L2CAPNoPeerCreditsIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.L2CAPNoPeerCreditsIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = L2CAPNoPeerCreditsIndication()
        curr = 0
        frame.InformationIncluded = data.contents[curr]
        curr += 1
        frame.LeCbNoPeerCredits = []
        for _ in range(frame.InformationIncluded):
            # Create sub-object container
            LeCbNoPeerCredits = L2CAPNoPeerCreditsIndication.LeCbNoPeerCredits()
            LeCbNoPeerCredits.DeviceId = data.contents[curr]
            curr += 1
            LeCbNoPeerCredits.ChannelId = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            # Add sub-object to list container.
            frame.LeCbNoPeerCredits.append(copy.deepcopy(LeCbNoPeerCredits))

        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class L2CAPLocalCreditsNotificationIndicationObserver(Observer):

    opGroup = Spec.L2CAPLocalCreditsNotificationIndicationFrame.opGroup
    opCode = Spec.L2CAPLocalCreditsNotificationIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.L2CAPLocalCreditsNotificationIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = L2CAPLocalCreditsNotificationIndication()
        curr = 0
        frame.InformationIncluded = data.contents[curr]
        curr += 1
        frame.LeCbLocalCreditsNotification = []
        for _ in range(frame.InformationIncluded):
            # Create sub-object container
            LeCbLocalCreditsNotification = L2CAPLocalCreditsNotificationIndication.LeCbLocalCreditsNotification()
            LeCbLocalCreditsNotification.DeviceId = data.contents[curr]
            curr += 1
            LeCbLocalCreditsNotification.ChannelId = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            LeCbLocalCreditsNotification.LocalCredits = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            # Add sub-object to list container.
            frame.LeCbLocalCreditsNotification.append(copy.deepcopy(LeCbLocalCreditsNotification))

        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class L2CAPLeCbDataIndicationObserver(Observer):

    opGroup = Spec.L2CAPLeCbDataIndicationFrame.opGroup
    opCode = Spec.L2CAPLeCbDataIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.L2CAPLeCbDataIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = L2CAPLeCbDataIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.SrcCid = packet.getParamValueAsNumber("SrcCid")
        frame.PacketLength = packet.getParamValueAsNumber("PacketLength")
        frame.Packet = packet.getParamValueAsList("Packet")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SMConfirmObserver(Observer):

    opGroup = Spec.SMConfirmFrame.opGroup
    opCode = Spec.SMConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SMConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SMConfirm()
        frame.Status = SMConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SMStatusIndicationObserver(Observer):

    opGroup = Spec.SMStatusIndicationFrame.opGroup
    opCode = Spec.SMStatusIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SMStatusIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SMStatusIndication()
        frame.SourceDeviceId = packet.getParamValueAsNumber("SourceDeviceId")
        frame.SmStatusParams_Status = SMStatusIndicationSmStatusParams_Status.getEnumString(packet.getParamValueAsNumber("SmStatusParams_Status"))
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SMRemoteSecurityRequestIndicationObserver(Observer):

    opGroup = Spec.SMRemoteSecurityRequestIndicationFrame.opGroup
    opCode = Spec.SMRemoteSecurityRequestIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SMRemoteSecurityRequestIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SMRemoteSecurityRequestIndication()
        frame.SourceDeviceId = packet.getParamValueAsNumber("SourceDeviceId")
        frame.SmSecurityReqParams_AuthRequest.BondingFlags = packet.getParamValueAsNumber("SmSecurityReqParams_AuthRequestBondingFlags")
        frame.SmSecurityReqParams_AuthRequest.Mitm = packet.getParamValueAsNumber("SmSecurityReqParams_AuthRequestMitm")
        frame.SmSecurityReqParams_AuthRequest.sc = packet.getParamValueAsNumber("SmSecurityReqParams_AuthRequestsc")
        frame.SmSecurityReqParams_AuthRequest.keypress = packet.getParamValueAsNumber("SmSecurityReqParams_AuthRequestkeypress")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SMRemotePairingRequestIndicationObserver(Observer):

    opGroup = Spec.SMRemotePairingRequestIndicationFrame.opGroup
    opCode = Spec.SMRemotePairingRequestIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SMRemotePairingRequestIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SMRemotePairingRequestIndication()
        frame.SourceDeviceId = packet.getParamValueAsNumber("SourceDeviceId")
        frame.SmPairingParams_IoCapabilities = packet.getParamValueAsNumber("SmPairingParams_IoCapabilities")
        frame.SmPairingParams_OobDataFlag = packet.getParamValueAsNumber("SmPairingParams_OobDataFlag")
        frame.SmPairingParams_AuthRequest.BondingFlags = packet.getParamValueAsNumber("SmPairingParams_AuthRequestBondingFlags")
        frame.SmPairingParams_AuthRequest.Mitm = packet.getParamValueAsNumber("SmPairingParams_AuthRequestMitm")
        frame.SmPairingParams_AuthRequest.sc = packet.getParamValueAsNumber("SmPairingParams_AuthRequestsc")
        frame.SmPairingParams_AuthRequest.keypress = packet.getParamValueAsNumber("SmPairingParams_AuthRequestkeypress")
        frame.SmPairingParams_MaxEncKeySize = packet.getParamValueAsNumber("SmPairingParams_MaxEncKeySize")
        frame.SmPairingParams_InitatorKeyDistribution.EncKey = packet.getParamValueAsNumber("SmPairingParams_InitatorKeyDistributionEncKey")
        frame.SmPairingParams_InitatorKeyDistribution.IdKey = packet.getParamValueAsNumber("SmPairingParams_InitatorKeyDistributionIdKey")
        frame.SmPairingParams_InitatorKeyDistribution.Sign = packet.getParamValueAsNumber("SmPairingParams_InitatorKeyDistributionSign")
        frame.SmPairingParams_InitatorKeyDistribution.LinkKey = packet.getParamValueAsNumber("SmPairingParams_InitatorKeyDistributionLinkKey")
        frame.SmPairingParams_InitatorKeyDistribution.Reserved = packet.getParamValueAsNumber("SmPairingParams_InitatorKeyDistributionReserved")
        frame.SmPairingParams_ResponderKeyDistribution.EncKey = packet.getParamValueAsNumber("SmPairingParams_ResponderKeyDistributionEncKey")
        frame.SmPairingParams_ResponderKeyDistribution.IdKey = packet.getParamValueAsNumber("SmPairingParams_ResponderKeyDistributionIdKey")
        frame.SmPairingParams_ResponderKeyDistribution.Sign = packet.getParamValueAsNumber("SmPairingParams_ResponderKeyDistributionSign")
        frame.SmPairingParams_ResponderKeyDistribution.LinkKey = packet.getParamValueAsNumber("SmPairingParams_ResponderKeyDistributionLinkKey")
        frame.SmPairingParams_ResponderKeyDistribution.Reserved = packet.getParamValueAsNumber("SmPairingParams_ResponderKeyDistributionReserved")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SMRemotePairingResponseIndicationObserver(Observer):

    opGroup = Spec.SMRemotePairingResponseIndicationFrame.opGroup
    opCode = Spec.SMRemotePairingResponseIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SMRemotePairingResponseIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SMRemotePairingResponseIndication()
        frame.SourceDeviceId = packet.getParamValueAsNumber("SourceDeviceId")
        frame.SmPairingParams_IoCapabilities = packet.getParamValueAsNumber("SmPairingParams_IoCapabilities")
        frame.SmPairingParams_OobDataFlag = packet.getParamValueAsNumber("SmPairingParams_OobDataFlag")
        frame.SmPairingParams_AuthRequest.BondingFlags = packet.getParamValueAsNumber("SmPairingParams_AuthRequestBondingFlags")
        frame.SmPairingParams_AuthRequest.Mitm = packet.getParamValueAsNumber("SmPairingParams_AuthRequestMitm")
        frame.SmPairingParams_AuthRequest.sc = packet.getParamValueAsNumber("SmPairingParams_AuthRequestsc")
        frame.SmPairingParams_AuthRequest.keypress = packet.getParamValueAsNumber("SmPairingParams_AuthRequestkeypress")
        frame.SmPairingParams_MaxEncKeySize = packet.getParamValueAsNumber("SmPairingParams_MaxEncKeySize")
        frame.SmPairingParams_InitatorKeyDistribution.EncKey = packet.getParamValueAsNumber("SmPairingParams_InitatorKeyDistributionEncKey")
        frame.SmPairingParams_InitatorKeyDistribution.IdKey = packet.getParamValueAsNumber("SmPairingParams_InitatorKeyDistributionIdKey")
        frame.SmPairingParams_InitatorKeyDistribution.Sign = packet.getParamValueAsNumber("SmPairingParams_InitatorKeyDistributionSign")
        frame.SmPairingParams_InitatorKeyDistribution.LinkKey = packet.getParamValueAsNumber("SmPairingParams_InitatorKeyDistributionLinkKey")
        frame.SmPairingParams_InitatorKeyDistribution.Reserved = packet.getParamValueAsNumber("SmPairingParams_InitatorKeyDistributionReserved")
        frame.SmPairingParams_ResponderKeyDistribution.EncKey = packet.getParamValueAsNumber("SmPairingParams_ResponderKeyDistributionEncKey")
        frame.SmPairingParams_ResponderKeyDistribution.IdKey = packet.getParamValueAsNumber("SmPairingParams_ResponderKeyDistributionIdKey")
        frame.SmPairingParams_ResponderKeyDistribution.Sign = packet.getParamValueAsNumber("SmPairingParams_ResponderKeyDistributionSign")
        frame.SmPairingParams_ResponderKeyDistribution.LinkKey = packet.getParamValueAsNumber("SmPairingParams_ResponderKeyDistributionLinkKey")
        frame.SmPairingParams_ResponderKeyDistribution.Reserved = packet.getParamValueAsNumber("SmPairingParams_ResponderKeyDistributionReserved")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SMPasskeyDisplayRequestIndicationObserver(Observer):

    opGroup = Spec.SMPasskeyDisplayRequestIndicationFrame.opGroup
    opCode = Spec.SMPasskeyDisplayRequestIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SMPasskeyDisplayRequestIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SMPasskeyDisplayRequestIndication()
        frame.SourceDeviceId = packet.getParamValueAsNumber("SourceDeviceId")
        frame.PasskeyDisplayReqParams_Passkey = packet.getParamValueAsNumber("PasskeyDisplayReqParams_Passkey")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SMPasskeyRequestIndicationObserver(Observer):

    opGroup = Spec.SMPasskeyRequestIndicationFrame.opGroup
    opCode = Spec.SMPasskeyRequestIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SMPasskeyRequestIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SMPasskeyRequestIndication()
        frame.SourceDeviceId = packet.getParamValueAsNumber("SourceDeviceId")
        frame.PasskeyReqParams_PasskeyType = packet.getParamValueAsNumber("PasskeyReqParams_PasskeyType")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SMPairingKeysetRequestIndicationObserver(Observer):

    opGroup = Spec.SMPairingKeysetRequestIndicationFrame.opGroup
    opCode = Spec.SMPairingKeysetRequestIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SMPairingKeysetRequestIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SMPairingKeysetRequestIndication()
        frame.SourceDeviceId = packet.getParamValueAsNumber("SourceDeviceId")
        frame.PairingKeysetReqParams_RequestedKeys.EncKey = packet.getParamValueAsNumber("PairingKeysetReqParams_RequestedKeysEncKey")
        frame.PairingKeysetReqParams_RequestedKeys.IdKey = packet.getParamValueAsNumber("PairingKeysetReqParams_RequestedKeysIdKey")
        frame.PairingKeysetReqParams_RequestedKeys.Sign = packet.getParamValueAsNumber("PairingKeysetReqParams_RequestedKeysSign")
        frame.PairingKeysetReqParams_RequestedKeys.LinkKey = packet.getParamValueAsNumber("PairingKeysetReqParams_RequestedKeysLinkKey")
        frame.PairingKeysetReqParams_RequestedKeys.Reserved = packet.getParamValueAsNumber("PairingKeysetReqParams_RequestedKeysReserved")
        frame.PairingKeysetReqParams_EncKeySize = packet.getParamValueAsNumber("PairingKeysetReqParams_EncKeySize")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SMPairingKeysetReceivedIndicationObserver(Observer):

    opGroup = Spec.SMPairingKeysetReceivedIndicationFrame.opGroup
    opCode = Spec.SMPairingKeysetReceivedIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SMPairingKeysetReceivedIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SMPairingKeysetReceivedIndication()
        frame.SourceDeviceId = packet.getParamValueAsNumber("SourceDeviceId")
        frame.PairingKeysetReceivedParams_KeyDistPayload.Ltk = packet.getParamValueAsList("PairingKeysetReceivedParams_KeyDistPayloadLtk")
        frame.PairingKeysetReceivedParams_KeyDistPayload.Irk = packet.getParamValueAsList("PairingKeysetReceivedParams_KeyDistPayloadIrk")
        frame.PairingKeysetReceivedParams_KeyDistPayload.Csrk = packet.getParamValueAsList("PairingKeysetReceivedParams_KeyDistPayloadCsrk")
        frame.PairingKeysetReceivedParams_KeyDistPayload.Ediv = packet.getParamValueAsNumber("PairingKeysetReceivedParams_KeyDistPayloadEdiv")
        frame.PairingKeysetReceivedParams_KeyDistPayload.Rand = packet.getParamValueAsList("PairingKeysetReceivedParams_KeyDistPayloadRand")
        frame.PairingKeysetReceivedParams_KeyDistPayload.BDAddr = packet.getParamValueAsList("PairingKeysetReceivedParams_KeyDistPayloadBDAddr")
        frame.PairingKeysetReceivedParams_KeyDistPayload.BDAddrType = packet.getParamValueAsNumber("PairingKeysetReceivedParams_KeyDistPayloadBDAddrType")
        frame.PairingKeysetReceivedParams_ReceivedKeys.EncKey = packet.getParamValueAsNumber("PairingKeysetReceivedParams_ReceivedKeysEncKey")
        frame.PairingKeysetReceivedParams_ReceivedKeys.IdKey = packet.getParamValueAsNumber("PairingKeysetReceivedParams_ReceivedKeysIdKey")
        frame.PairingKeysetReceivedParams_ReceivedKeys.Sign = packet.getParamValueAsNumber("PairingKeysetReceivedParams_ReceivedKeysSign")
        frame.PairingKeysetReceivedParams_ReceivedKeys.LinkKey = packet.getParamValueAsNumber("PairingKeysetReceivedParams_ReceivedKeysLinkKey")
        frame.PairingKeysetReceivedParams_ReceivedKeys.Reserved = packet.getParamValueAsNumber("PairingKeysetReceivedParams_ReceivedKeysReserved")
        frame.PairingKeysetReceivedParams_EncKeySize = packet.getParamValueAsNumber("PairingKeysetReceivedParams_EncKeySize")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SMPairingCompleteIndicationObserver(Observer):

    opGroup = Spec.SMPairingCompleteIndicationFrame.opGroup
    opCode = Spec.SMPairingCompleteIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SMPairingCompleteIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SMPairingCompleteIndication()
        frame.SourceDeviceId = packet.getParamValueAsNumber("SourceDeviceId")
        frame.PairingCompleteParams_EncKeySize = packet.getParamValueAsNumber("PairingCompleteParams_EncKeySize")
        frame.PairingCompleteParams_bMitmProtection = packet.getParamValueAsNumber("PairingCompleteParams_bMitmProtection")
        frame.PairingCompleteParams_bBonding = packet.getParamValueAsNumber("PairingCompleteParams_bBonding")
        frame.PairingCompleteParams_LeSc = packet.getParamValueAsNumber("PairingCompleteParams_LeSc")
        frame.PairingCompleteParams_LeScLtk = packet.getParamValueAsList("PairingCompleteParams_LeScLtk")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SMPairingFailedIndicationObserver(Observer):

    opGroup = Spec.SMPairingFailedIndicationFrame.opGroup
    opCode = Spec.SMPairingFailedIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SMPairingFailedIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SMPairingFailedIndication()
        frame.SourceDeviceId = packet.getParamValueAsNumber("SourceDeviceId")
        frame.SmPairingFailedParams_Reason = SMPairingFailedIndicationSmPairingFailedParams_Reason.getEnumString(packet.getParamValueAsNumber("SmPairingFailedParams_Reason"))
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SMReceivedPairingFailedIndicationObserver(Observer):

    opGroup = Spec.SMReceivedPairingFailedIndicationFrame.opGroup
    opCode = Spec.SMReceivedPairingFailedIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SMReceivedPairingFailedIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SMReceivedPairingFailedIndication()
        frame.SourceDeviceId = packet.getParamValueAsNumber("SourceDeviceId")
        frame.SmPairingFailedParams_Reason = SMReceivedPairingFailedIndicationSmPairingFailedParams_Reason.getEnumString(packet.getParamValueAsNumber("SmPairingFailedParams_Reason"))
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SMLlLtkRequestIndicationObserver(Observer):

    opGroup = Spec.SMLlLtkRequestIndicationFrame.opGroup
    opCode = Spec.SMLlLtkRequestIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SMLlLtkRequestIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SMLlLtkRequestIndication()
        frame.SourceDeviceId = packet.getParamValueAsNumber("SourceDeviceId")
        frame.LlLtkRequestParams_RandomNumber = packet.getParamValueAsList("LlLtkRequestParams_RandomNumber")
        frame.LlLtkRequestParams_EncryptedDiversifier = packet.getParamValueAsNumber("LlLtkRequestParams_EncryptedDiversifier")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SMLlEncryptionStatusIndicationObserver(Observer):

    opGroup = Spec.SMLlEncryptionStatusIndicationFrame.opGroup
    opCode = Spec.SMLlEncryptionStatusIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SMLlEncryptionStatusIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SMLlEncryptionStatusIndication()
        frame.SourceDeviceId = packet.getParamValueAsNumber("SourceDeviceId")
        frame.LlEncryptionStatusParams_LlStatus = SMLlEncryptionStatusIndicationLlEncryptionStatusParams_LlStatus.getEnumString(packet.getParamValueAsNumber("LlEncryptionStatusParams_LlStatus"))
        frame.LlEncryptionStatusParams_EncryptionState = SMLlEncryptionStatusIndicationLlEncryptionStatusParams_EncryptionState.getEnumString(packet.getParamValueAsNumber("LlEncryptionStatusParams_EncryptionState"))
        frame.LlEncryptionStatusParams_EncKeyType = SMLlEncryptionStatusIndicationLlEncryptionStatusParams_EncKeyType.getEnumString(packet.getParamValueAsNumber("LlEncryptionStatusParams_EncKeyType"))
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SMLlEncryptResIndicationObserver(Observer):

    opGroup = Spec.SMLlEncryptResIndicationFrame.opGroup
    opCode = Spec.SMLlEncryptResIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SMLlEncryptResIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SMLlEncryptResIndication()
        frame.SourceDeviceId = packet.getParamValueAsNumber("SourceDeviceId")
        frame.LlEncryptResParams_Status = SMLlEncryptResIndicationLlEncryptResParams_Status.getEnumString(packet.getParamValueAsNumber("LlEncryptResParams_Status"))
        frame.LlEncryptResParams_EncryptedData = packet.getParamValueAsList("LlEncryptResParams_EncryptedData")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SMLlRandResIndicationObserver(Observer):

    opGroup = Spec.SMLlRandResIndicationFrame.opGroup
    opCode = Spec.SMLlRandResIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SMLlRandResIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SMLlRandResIndication()
        frame.SourceDeviceId = packet.getParamValueAsNumber("SourceDeviceId")
        frame.LlRandResParams_Status = SMLlRandResIndicationLlRandResParams_Status.getEnumString(packet.getParamValueAsNumber("LlRandResParams_Status"))
        frame.LlRandResParams_RandomNumber = packet.getParamValueAsList("LlRandResParams_RandomNumber")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SMTbCreateRandomDeviceAddrResIndicationObserver(Observer):

    opGroup = Spec.SMTbCreateRandomDeviceAddrResIndicationFrame.opGroup
    opCode = Spec.SMTbCreateRandomDeviceAddrResIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SMTbCreateRandomDeviceAddrResIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SMTbCreateRandomDeviceAddrResIndication()
        frame.SourceDeviceId = packet.getParamValueAsNumber("SourceDeviceId")
        frame.TbCreateRandomDeviceAddrResParams_Status = SMTbCreateRandomDeviceAddrResIndicationTbCreateRandomDeviceAddrResParams_Status.getEnumString(packet.getParamValueAsNumber("TbCreateRandomDeviceAddrResParams_Status"))
        frame.TbCreateRandomDeviceAddrResParams_AddrType = packet.getParamValueAsNumber("TbCreateRandomDeviceAddrResParams_AddrType")
        frame.TbCreateRandomDeviceAddrResParams_Addr = packet.getParamValueAsList("TbCreateRandomDeviceAddrResParams_Addr")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SMTbCheckResolvalePrivateAddrResIndicationObserver(Observer):

    opGroup = Spec.SMTbCheckResolvalePrivateAddrResIndicationFrame.opGroup
    opCode = Spec.SMTbCheckResolvalePrivateAddrResIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SMTbCheckResolvalePrivateAddrResIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SMTbCheckResolvalePrivateAddrResIndication()
        frame.SourceDeviceId = packet.getParamValueAsNumber("SourceDeviceId")
        frame.TbCheckResolvablePrivateAddrResParams_Status = SMTbCheckResolvalePrivateAddrResIndicationTbCheckResolvablePrivateAddrResParams_Status.getEnumString(packet.getParamValueAsNumber("TbCheckResolvablePrivateAddrResParams_Status"))
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SMTbSignDataResIndicationObserver(Observer):

    opGroup = Spec.SMTbSignDataResIndicationFrame.opGroup
    opCode = Spec.SMTbSignDataResIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SMTbSignDataResIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SMTbSignDataResIndication()
        frame.SourceDeviceId = packet.getParamValueAsNumber("SourceDeviceId")
        frame.TbSignDataResParams_Status = SMTbSignDataResIndicationTbSignDataResParams_Status.getEnumString(packet.getParamValueAsNumber("TbSignDataResParams_Status"))
        frame.TbSignDataResParams_Signature = packet.getParamValueAsList("TbSignDataResParams_Signature")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SMTbVerifyDataSignatureResIndicationObserver(Observer):

    opGroup = Spec.SMTbVerifyDataSignatureResIndicationFrame.opGroup
    opCode = Spec.SMTbVerifyDataSignatureResIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SMTbVerifyDataSignatureResIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SMTbVerifyDataSignatureResIndication()
        frame.SourceDeviceId = packet.getParamValueAsNumber("SourceDeviceId")
        frame.TbVerifyDataSignatureResParams_Status = SMTbVerifyDataSignatureResIndicationTbVerifyDataSignatureResParams_Status.getEnumString(packet.getParamValueAsNumber("TbVerifyDataSignatureResParams_Status"))
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SMRemoteKeypressNotificationIndicationObserver(Observer):

    opGroup = Spec.SMRemoteKeypressNotificationIndicationFrame.opGroup
    opCode = Spec.SMRemoteKeypressNotificationIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SMRemoteKeypressNotificationIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SMRemoteKeypressNotificationIndication()
        frame.SourceDeviceId = packet.getParamValueAsNumber("SourceDeviceId")
        frame.SmKeypressNotificationParams_keypressNotifType = packet.getParamValueAsNumber("SmKeypressNotificationParams_keypressNotifType")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SMNcDisplayRequestIndicationObserver(Observer):

    opGroup = Spec.SMNcDisplayRequestIndicationFrame.opGroup
    opCode = Spec.SMNcDisplayRequestIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SMNcDisplayRequestIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SMNcDisplayRequestIndication()
        frame.SourceDeviceId = packet.getParamValueAsNumber("SourceDeviceId")
        frame.ncValue = packet.getParamValueAsNumber("ncValue")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SMLeScOobDataRequestIndicationObserver(Observer):

    opGroup = Spec.SMLeScOobDataRequestIndicationFrame.opGroup
    opCode = Spec.SMLeScOobDataRequestIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SMLeScOobDataRequestIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SMLeScOobDataRequestIndication()
        frame.SourceDeviceId = packet.getParamValueAsNumber("SourceDeviceId")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SMLocalLeScOobDataIndicationObserver(Observer):

    opGroup = Spec.SMLocalLeScOobDataIndicationFrame.opGroup
    opCode = Spec.SMLocalLeScOobDataIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SMLocalLeScOobDataIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SMLocalLeScOobDataIndication()
        frame.SourceDeviceId = packet.getParamValueAsNumber("SourceDeviceId")
        frame.LeScOobDataParams_ = packet.getParamValueAsNumber("LeScOobDataParams_")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class SMGenerateNewEcdhPkSkPairResIndicationObserver(Observer):

    opGroup = Spec.SMGenerateNewEcdhPkSkPairResIndicationFrame.opGroup
    opCode = Spec.SMGenerateNewEcdhPkSkPairResIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.SMGenerateNewEcdhPkSkPairResIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = SMGenerateNewEcdhPkSkPairResIndication()
        frame.SourceDeviceId = packet.getParamValueAsNumber("SourceDeviceId")
        frame.Status = SMGenerateNewEcdhPkSkPairResIndicationStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ATTConfirmObserver(Observer):

    opGroup = Spec.ATTConfirmFrame.opGroup
    opCode = Spec.ATTConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ATTConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ATTConfirm()
        frame.Status = ATTConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ATTGetMtuIndicationObserver(Observer):

    opGroup = Spec.ATTGetMtuIndicationFrame.opGroup
    opCode = Spec.ATTGetMtuIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ATTGetMtuIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ATTGetMtuIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.Mtu = packet.getParamValueAsNumber("Mtu")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ATTClientIncomingServerErrorResponseIndicationObserver(Observer):

    opGroup = Spec.ATTClientIncomingServerErrorResponseIndicationFrame.opGroup
    opCode = Spec.ATTClientIncomingServerErrorResponseIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ATTClientIncomingServerErrorResponseIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ATTClientIncomingServerErrorResponseIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.Params_RequestOpcodeInError = packet.getParamValueAsNumber("Params_RequestOpcodeInError")
        frame.Params_AttributeHandleInError = packet.getParamValueAsNumber("Params_AttributeHandleInError")
        frame.Params_ErrorCode = packet.getParamValueAsNumber("Params_ErrorCode")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ATTServerIncomingClientExchangeMtuRequestIndicationObserver(Observer):

    opGroup = Spec.ATTServerIncomingClientExchangeMtuRequestIndicationFrame.opGroup
    opCode = Spec.ATTServerIncomingClientExchangeMtuRequestIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ATTServerIncomingClientExchangeMtuRequestIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ATTServerIncomingClientExchangeMtuRequestIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.Params_ClientRxMtu = packet.getParamValueAsNumber("Params_ClientRxMtu")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ATTClientIncomingServerExchangeMtuResponseIndicationObserver(Observer):

    opGroup = Spec.ATTClientIncomingServerExchangeMtuResponseIndicationFrame.opGroup
    opCode = Spec.ATTClientIncomingServerExchangeMtuResponseIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ATTClientIncomingServerExchangeMtuResponseIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ATTClientIncomingServerExchangeMtuResponseIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.Params_ServerRxMtu = packet.getParamValueAsNumber("Params_ServerRxMtu")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ATTServerIncomingClientFindInformationRequestIndicationObserver(Observer):

    opGroup = Spec.ATTServerIncomingClientFindInformationRequestIndicationFrame.opGroup
    opCode = Spec.ATTServerIncomingClientFindInformationRequestIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ATTServerIncomingClientFindInformationRequestIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ATTServerIncomingClientFindInformationRequestIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.Params_StartingHandle = packet.getParamValueAsNumber("Params_StartingHandle")
        frame.Params_EndingHandle = packet.getParamValueAsNumber("Params_EndingHandle")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ATTClientIncomingServerFindInformationResponseIndicationObserver(Observer):

    opGroup = Spec.ATTClientIncomingServerFindInformationResponseIndicationFrame.opGroup
    opCode = Spec.ATTClientIncomingServerFindInformationResponseIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ATTClientIncomingServerFindInformationResponseIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ATTClientIncomingServerFindInformationResponseIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.Params_Format = packet.getParamValueAsNumber("Params_Format")
        frame.Params_InformationData = packet.getParamValueAsNumber("Params_InformationData")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ATTServerIncomingClientFindByTypeValueRequestIndicationObserver(Observer):

    opGroup = Spec.ATTServerIncomingClientFindByTypeValueRequestIndicationFrame.opGroup
    opCode = Spec.ATTServerIncomingClientFindByTypeValueRequestIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ATTServerIncomingClientFindByTypeValueRequestIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ATTServerIncomingClientFindByTypeValueRequestIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.Params_StartingHandle = packet.getParamValueAsNumber("Params_StartingHandle")
        frame.Params_EndingHandle = packet.getParamValueAsNumber("Params_EndingHandle")
        frame.Params_AttributeType = packet.getParamValueAsNumber("Params_AttributeType")
        frame.Params_AttributeLength = packet.getParamValueAsNumber("Params_AttributeLength")
        frame.Params_AttributeValue = packet.getParamValueAsList("Params_AttributeValue")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ATTClientIncomingServerFindByTypeValueResponseIndicationObserver(Observer):

    opGroup = Spec.ATTClientIncomingServerFindByTypeValueResponseIndicationFrame.opGroup
    opCode = Spec.ATTClientIncomingServerFindByTypeValueResponseIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ATTClientIncomingServerFindByTypeValueResponseIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ATTClientIncomingServerFindByTypeValueResponseIndication()
        curr = 0
        frame.DeviceId = data.contents[curr]
        curr += 1
        frame.Params_GroupCount = list_to_int(data.contents[curr:curr + 2])
        curr += 2
        frame.Params_HandleGroup = []
        for _ in range(frame.Params_GroupCount):
            # Create sub-object container
            Params_HandleGroup = ATTClientIncomingServerFindByTypeValueResponseIndication.Params_HandleGroup()
            Params_HandleGroup.StartingHandle = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            Params_HandleGroup.EndingHandle = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            # Add sub-object to list container.
            frame.Params_HandleGroup.append(copy.deepcopy(Params_HandleGroup))

        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ATTServerIncomingClientReadByTypeRequestIndicationObserver(Observer):

    opGroup = Spec.ATTServerIncomingClientReadByTypeRequestIndicationFrame.opGroup
    opCode = Spec.ATTServerIncomingClientReadByTypeRequestIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ATTServerIncomingClientReadByTypeRequestIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ATTServerIncomingClientReadByTypeRequestIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.Params_StartingHandle = packet.getParamValueAsNumber("Params_StartingHandle")
        frame.Params_EndingHandle = packet.getParamValueAsNumber("Params_EndingHandle")
        frame.Params_Format = packet.getParamValueAsNumber("Params_Format")
        frame.Params_AttributeType = packet.getParamValueAsNumber("Params_AttributeType")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ATTClientIncomingServerReadByTypeResponseIndicationObserver(Observer):

    opGroup = Spec.ATTClientIncomingServerReadByTypeResponseIndicationFrame.opGroup
    opCode = Spec.ATTClientIncomingServerReadByTypeResponseIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ATTClientIncomingServerReadByTypeResponseIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ATTClientIncomingServerReadByTypeResponseIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.Params_Length = packet.getParamValueAsNumber("Params_Length")
        frame.Params_AttributeDataListLength = packet.getParamValueAsNumber("Params_AttributeDataListLength")
        frame.Params_AttributeDataList = packet.getParamValueAsList("Params_AttributeDataList")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ATTServerIncomingClientReadRequestIndicationObserver(Observer):

    opGroup = Spec.ATTServerIncomingClientReadRequestIndicationFrame.opGroup
    opCode = Spec.ATTServerIncomingClientReadRequestIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ATTServerIncomingClientReadRequestIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ATTServerIncomingClientReadRequestIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.Params_AttributeHandle = packet.getParamValueAsNumber("Params_AttributeHandle")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ATTClientIncomingServerReadResponseIndicationObserver(Observer):

    opGroup = Spec.ATTClientIncomingServerReadResponseIndicationFrame.opGroup
    opCode = Spec.ATTClientIncomingServerReadResponseIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ATTClientIncomingServerReadResponseIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ATTClientIncomingServerReadResponseIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.Params_AttributeLength = packet.getParamValueAsNumber("Params_AttributeLength")
        frame.Params_AttributeValue = packet.getParamValueAsList("Params_AttributeValue")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ATTServerIncomingClientReadBlobRequestIndicationObserver(Observer):

    opGroup = Spec.ATTServerIncomingClientReadBlobRequestIndicationFrame.opGroup
    opCode = Spec.ATTServerIncomingClientReadBlobRequestIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ATTServerIncomingClientReadBlobRequestIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ATTServerIncomingClientReadBlobRequestIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.Params_AttributeHandle = packet.getParamValueAsNumber("Params_AttributeHandle")
        frame.Params_ValueOffset = packet.getParamValueAsNumber("Params_ValueOffset")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ATTClientIncomingServerReadBlobResponseIndicationObserver(Observer):

    opGroup = Spec.ATTClientIncomingServerReadBlobResponseIndicationFrame.opGroup
    opCode = Spec.ATTClientIncomingServerReadBlobResponseIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ATTClientIncomingServerReadBlobResponseIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ATTClientIncomingServerReadBlobResponseIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.Params_AttributeLength = packet.getParamValueAsNumber("Params_AttributeLength")
        frame.Params_AttributeValue = packet.getParamValueAsList("Params_AttributeValue")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ATTServerIncomingClientReadMultipleRequestIndicationObserver(Observer):

    opGroup = Spec.ATTServerIncomingClientReadMultipleRequestIndicationFrame.opGroup
    opCode = Spec.ATTServerIncomingClientReadMultipleRequestIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ATTServerIncomingClientReadMultipleRequestIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ATTServerIncomingClientReadMultipleRequestIndication()
        curr = 0
        frame.DeviceId = data.contents[curr]
        curr += 1
        frame.Params_HandleCount = list_to_int(data.contents[curr:curr + 2])
        curr += 2
        frame.Params_ListOfHandles = []
        for _ in range(frame.Params_HandleCount):
            # Create sub-object container
            Params_ListOfHandles = ATTServerIncomingClientReadMultipleRequestIndication.Params_ListOfHandles()
            Params_ListOfHandles.Handle = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            # Add sub-object to list container.
            frame.Params_ListOfHandles.append(copy.deepcopy(Params_ListOfHandles))

        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ATTClientIncomingServerReadMultipleResponseIndicationObserver(Observer):

    opGroup = Spec.ATTClientIncomingServerReadMultipleResponseIndicationFrame.opGroup
    opCode = Spec.ATTClientIncomingServerReadMultipleResponseIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ATTClientIncomingServerReadMultipleResponseIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ATTClientIncomingServerReadMultipleResponseIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.Params_ListLength = packet.getParamValueAsNumber("Params_ListLength")
        frame.Params_ListOfValues = packet.getParamValueAsList("Params_ListOfValues")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ATTServerIncomingClientReadByGroupTypeRequestIndicationObserver(Observer):

    opGroup = Spec.ATTServerIncomingClientReadByGroupTypeRequestIndicationFrame.opGroup
    opCode = Spec.ATTServerIncomingClientReadByGroupTypeRequestIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ATTServerIncomingClientReadByGroupTypeRequestIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ATTServerIncomingClientReadByGroupTypeRequestIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.Params_StartingHandle = packet.getParamValueAsNumber("Params_StartingHandle")
        frame.Params_EndingHandle = packet.getParamValueAsNumber("Params_EndingHandle")
        frame.Params_Format = packet.getParamValueAsNumber("Params_Format")
        frame.Params_AttributeType = packet.getParamValueAsNumber("Params_AttributeType")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ATTClientIncomingServerReadByGroupTypeResponseIndicationObserver(Observer):

    opGroup = Spec.ATTClientIncomingServerReadByGroupTypeResponseIndicationFrame.opGroup
    opCode = Spec.ATTClientIncomingServerReadByGroupTypeResponseIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ATTClientIncomingServerReadByGroupTypeResponseIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ATTClientIncomingServerReadByGroupTypeResponseIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.Params_Length = packet.getParamValueAsNumber("Params_Length")
        frame.Params_AttributeDataListLength = packet.getParamValueAsNumber("Params_AttributeDataListLength")
        frame.Params_AttributeDataList = packet.getParamValueAsList("Params_AttributeDataList")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ATTServerIncomingClientWriteRequestIndicationObserver(Observer):

    opGroup = Spec.ATTServerIncomingClientWriteRequestIndicationFrame.opGroup
    opCode = Spec.ATTServerIncomingClientWriteRequestIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ATTServerIncomingClientWriteRequestIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ATTServerIncomingClientWriteRequestIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.Params_AttributeHandle = packet.getParamValueAsNumber("Params_AttributeHandle")
        frame.Params_AttributeLength = packet.getParamValueAsNumber("Params_AttributeLength")
        frame.Params_AttributeValue = packet.getParamValueAsList("Params_AttributeValue")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ATTClientIncomingServerWriteResponseIndicationObserver(Observer):

    opGroup = Spec.ATTClientIncomingServerWriteResponseIndicationFrame.opGroup
    opCode = Spec.ATTClientIncomingServerWriteResponseIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ATTClientIncomingServerWriteResponseIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ATTClientIncomingServerWriteResponseIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ATTServerIncomingClientWriteCommandIndicationObserver(Observer):

    opGroup = Spec.ATTServerIncomingClientWriteCommandIndicationFrame.opGroup
    opCode = Spec.ATTServerIncomingClientWriteCommandIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ATTServerIncomingClientWriteCommandIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ATTServerIncomingClientWriteCommandIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.Params_AttributeHandle = packet.getParamValueAsNumber("Params_AttributeHandle")
        frame.Params_AttributeLength = packet.getParamValueAsNumber("Params_AttributeLength")
        frame.Params_AttributeValue = packet.getParamValueAsList("Params_AttributeValue")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ATTServerIncomingClientSignedWriteCommandIndicationObserver(Observer):

    opGroup = Spec.ATTServerIncomingClientSignedWriteCommandIndicationFrame.opGroup
    opCode = Spec.ATTServerIncomingClientSignedWriteCommandIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ATTServerIncomingClientSignedWriteCommandIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ATTServerIncomingClientSignedWriteCommandIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.Params_AttributeHandle = packet.getParamValueAsNumber("Params_AttributeHandle")
        frame.Params_AttributeLength = packet.getParamValueAsNumber("Params_AttributeLength")
        frame.Params_AttributeValue = packet.getParamValueAsList("Params_AttributeValue")
        frame.Params_AuthenticationSignature = packet.getParamValueAsList("Params_AuthenticationSignature")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ATTServerIncomingClientPrepareWriteRequestIndicationObserver(Observer):

    opGroup = Spec.ATTServerIncomingClientPrepareWriteRequestIndicationFrame.opGroup
    opCode = Spec.ATTServerIncomingClientPrepareWriteRequestIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ATTServerIncomingClientPrepareWriteRequestIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ATTServerIncomingClientPrepareWriteRequestIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.Params_AttributeHandle = packet.getParamValueAsNumber("Params_AttributeHandle")
        frame.Params_ValueOffset = packet.getParamValueAsNumber("Params_ValueOffset")
        frame.Params_AttributeLength = packet.getParamValueAsNumber("Params_AttributeLength")
        frame.Params_AttributeValue = packet.getParamValueAsList("Params_AttributeValue")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ATTClientIncomingServerPrepareWriteResponseIndicationObserver(Observer):

    opGroup = Spec.ATTClientIncomingServerPrepareWriteResponseIndicationFrame.opGroup
    opCode = Spec.ATTClientIncomingServerPrepareWriteResponseIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ATTClientIncomingServerPrepareWriteResponseIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ATTClientIncomingServerPrepareWriteResponseIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.Params_AttributeHandle = packet.getParamValueAsNumber("Params_AttributeHandle")
        frame.Params_ValueOffset = packet.getParamValueAsNumber("Params_ValueOffset")
        frame.Params_AttributeLength = packet.getParamValueAsNumber("Params_AttributeLength")
        frame.Params_AttributeValue = packet.getParamValueAsList("Params_AttributeValue")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ATTServerIncomingClientExecuteWriteRequestIndicationObserver(Observer):

    opGroup = Spec.ATTServerIncomingClientExecuteWriteRequestIndicationFrame.opGroup
    opCode = Spec.ATTServerIncomingClientExecuteWriteRequestIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ATTServerIncomingClientExecuteWriteRequestIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ATTServerIncomingClientExecuteWriteRequestIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.Params_Flags = packet.getParamValueAsNumber("Params_Flags")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ATTClientIncomingServerExecuteWriteResponseIndicationObserver(Observer):

    opGroup = Spec.ATTClientIncomingServerExecuteWriteResponseIndicationFrame.opGroup
    opCode = Spec.ATTClientIncomingServerExecuteWriteResponseIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ATTClientIncomingServerExecuteWriteResponseIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ATTClientIncomingServerExecuteWriteResponseIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ATTClientIncomingServerHandleValueNotificationIndicationObserver(Observer):

    opGroup = Spec.ATTClientIncomingServerHandleValueNotificationIndicationFrame.opGroup
    opCode = Spec.ATTClientIncomingServerHandleValueNotificationIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ATTClientIncomingServerHandleValueNotificationIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ATTClientIncomingServerHandleValueNotificationIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.Params_AttributeHandle = packet.getParamValueAsNumber("Params_AttributeHandle")
        frame.Params_AttributeLength = packet.getParamValueAsNumber("Params_AttributeLength")
        frame.Params_AttributeValue = packet.getParamValueAsList("Params_AttributeValue")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ATTClientIncomingServerHandleValueIndicationIndicationObserver(Observer):

    opGroup = Spec.ATTClientIncomingServerHandleValueIndicationIndicationFrame.opGroup
    opCode = Spec.ATTClientIncomingServerHandleValueIndicationIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ATTClientIncomingServerHandleValueIndicationIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ATTClientIncomingServerHandleValueIndicationIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.Params_AttributeHandle = packet.getParamValueAsNumber("Params_AttributeHandle")
        frame.Params_AttributeLength = packet.getParamValueAsNumber("Params_AttributeLength")
        frame.Params_AttributeValue = packet.getParamValueAsList("Params_AttributeValue")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ATTServerIncomingClientHandleValueConfirmationIndicationObserver(Observer):

    opGroup = Spec.ATTServerIncomingClientHandleValueConfirmationIndicationFrame.opGroup
    opCode = Spec.ATTServerIncomingClientHandleValueConfirmationIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ATTServerIncomingClientHandleValueConfirmationIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ATTServerIncomingClientHandleValueConfirmationIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ATTUnsupportedOpcodeIndicationObserver(Observer):

    opGroup = Spec.ATTUnsupportedOpcodeIndicationFrame.opGroup
    opCode = Spec.ATTUnsupportedOpcodeIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ATTUnsupportedOpcodeIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ATTUnsupportedOpcodeIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.Opcode = packet.getParamValueAsNumber("Opcode")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class ATTTimeoutIndicationObserver(Observer):

    opGroup = Spec.ATTTimeoutIndicationFrame.opGroup
    opCode = Spec.ATTTimeoutIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.ATTTimeoutIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = ATTTimeoutIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTConfirmObserver(Observer):

    opGroup = Spec.GATTConfirmFrame.opGroup
    opCode = Spec.GATTConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTConfirm()
        frame.Status = GATTConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
            if frame.Status != 'gBleSuccess_c':
                print '\t[warning] GATTConfirm w/ Status ' + str(frame.Status)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTGetMtuIndicationObserver(Observer):

    opGroup = Spec.GATTGetMtuIndicationFrame.opGroup
    opCode = Spec.GATTGetMtuIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTGetMtuIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTGetMtuIndication()
        frame.Mtu = packet.getParamValueAsNumber("Mtu")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTClientProcedureExchangeMtuIndicationObserver(Observer):

    opGroup = Spec.GATTClientProcedureExchangeMtuIndicationFrame.opGroup
    opCode = Spec.GATTClientProcedureExchangeMtuIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTClientProcedureExchangeMtuIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTClientProcedureExchangeMtuIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.ProcedureResult = packet.getParamValueAsNumber("ProcedureResult")
        frame.Error = packet.getParamValueAsNumber("Error")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTClientProcedureDiscoverAllPrimaryServicesIndicationObserver(Observer):

    opGroup = Spec.GATTClientProcedureDiscoverAllPrimaryServicesIndicationFrame.opGroup
    opCode = Spec.GATTClientProcedureDiscoverAllPrimaryServicesIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTClientProcedureDiscoverAllPrimaryServicesIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTClientProcedureDiscoverAllPrimaryServicesIndication()
        curr = 0
        frame.DeviceId = data.contents[curr]
        curr += 1
        frame.ProcedureResult = data.contents[curr]
        curr += 1
        frame.Error = list_to_int(data.contents[curr:curr + 2])
        curr += 2
        if frame.ProcedureResult == 0x01:
            callback(self.deviceName, frame) if callback else print_event(self.deviceName, frame)
            fsciLibrary.DestroyFSCIFrame(event)
            return
        frame.NbOfDiscoveredServices = data.contents[curr]
        curr += 1
        frame.DiscoveredServices = []
        for _ in range(frame.NbOfDiscoveredServices):
            # Create sub-object container
            DiscoveredServices = GATTClientProcedureDiscoverAllPrimaryServicesIndication.DiscoveredServices()
            DiscoveredServices.StartHandle = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            DiscoveredServices.EndHandle = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            DiscoveredServices.UuidType = data.contents[curr]
            curr += 1
            if DiscoveredServices.UuidType == UuidType.Uuid16Bits:
                selected_len = 2
            elif DiscoveredServices.UuidType == UuidType.Uuid128Bits:
                selected_len = 16
            elif DiscoveredServices.UuidType == UuidType.Uuid32Bits:
                selected_len = 4
            else:
                print "Wrong UuidType value!"
            DiscoveredServices.Uuid = list_to_int(data.contents[curr:curr + selected_len])
            curr += selected_len
            DiscoveredServices.NbOfCharacteristics = data.contents[curr]
            curr += 1
            DiscoveredServices.Characteristics = []
            for _ in range(DiscoveredServices.NbOfCharacteristics):
                # Create sub-object container
                Characteristics = GATTClientProcedureDiscoverAllPrimaryServicesIndication.DiscoveredServices.Characteristics()
                Characteristics.Properties = data.contents[curr]
                curr += 1
                # Create sub-object container
                Characteristics.Value = GATTClientProcedureDiscoverAllPrimaryServicesIndication.DiscoveredServices.Characteristics.Value()
                Characteristics.Value.Handle = list_to_int(data.contents[curr:curr + 2])
                curr += 2
                Characteristics.Value.UuidType = data.contents[curr]
                curr += 1
                if Characteristics.Value.UuidType == UuidType.Uuid16Bits:
                    selected_len = 2
                elif Characteristics.Value.UuidType == UuidType.Uuid128Bits:
                    selected_len = 16
                elif Characteristics.Value.UuidType == UuidType.Uuid32Bits:
                    selected_len = 4
                else:
                    print "Wrong UuidType value!"
                Characteristics.Value.Uuid = list_to_int(data.contents[curr:curr + selected_len])
                curr += selected_len
                Characteristics.Value.ValueLength = list_to_int(data.contents[curr:curr + 2])
                curr += 2
                Characteristics.Value.MaxValueLength = list_to_int(data.contents[curr:curr + 2])
                curr += 2
                Characteristics.Value.Value = list_to_int(data.contents[curr:curr + Characteristics.Value.ValueLength])
                curr += Characteristics.Value.ValueLength
                Characteristics.NbOfDescriptors = data.contents[curr]
                curr += 1
                Characteristics.Descriptors = []
                for _ in range(Characteristics.NbOfDescriptors):
                    # Create sub-object container
                    Descriptors = GATTClientProcedureDiscoverAllPrimaryServicesIndication.DiscoveredServices.Characteristics.Descriptors()
                    Descriptors.Handle = list_to_int(data.contents[curr:curr + 2])
                    curr += 2
                    Descriptors.UuidType = data.contents[curr]
                    curr += 1
                    if Descriptors.UuidType == UuidType.Uuid16Bits:
                        selected_len = 2
                    elif Descriptors.UuidType == UuidType.Uuid128Bits:
                        selected_len = 16
                    elif Descriptors.UuidType == UuidType.Uuid32Bits:
                        selected_len = 4
                    else:
                        print "Wrong UuidType value!"
                    Descriptors.Uuid = list_to_int(data.contents[curr:curr + selected_len])
                    curr += selected_len
                    Descriptors.ValueLength = list_to_int(data.contents[curr:curr + 2])
                    curr += 2
                    Descriptors.MaxValueLength = list_to_int(data.contents[curr:curr + 2])
                    curr += 2
                    Descriptors.Value = list_to_int(data.contents[curr:curr + Descriptors.ValueLength])
                    curr += Descriptors.ValueLength
                    # Add sub-object to list container.
                    Characteristics.Descriptors.append(copy.deepcopy(Descriptors))

                # Add sub-object to list container.
                DiscoveredServices.Characteristics.append(copy.deepcopy(Characteristics))

            DiscoveredServices.NbOfIncludedServices = data.contents[curr]
            curr += 1
            DiscoveredServices.IncludedServices = []
            for _ in range(DiscoveredServices.NbOfIncludedServices):
                # Create sub-object container
                IncludedServices = GATTClientProcedureDiscoverAllPrimaryServicesIndication.DiscoveredServices.IncludedServices()
                IncludedServices.StartHandle = list_to_int(data.contents[curr:curr + 2])
                curr += 2
                IncludedServices.EndHandle = list_to_int(data.contents[curr:curr + 2])
                curr += 2
                IncludedServices.UuidType = data.contents[curr]
                curr += 1
                if IncludedServices.UuidType == UuidType.Uuid16Bits:
                    selected_len = 2
                elif IncludedServices.UuidType == UuidType.Uuid128Bits:
                    selected_len = 16
                elif IncludedServices.UuidType == UuidType.Uuid32Bits:
                    selected_len = 4
                else:
                    print "Wrong UuidType value!"
                IncludedServices.Uuid = list_to_int(data.contents[curr:curr + selected_len])
                curr += selected_len
                IncludedServices.NbOfCharacteristics = data.contents[curr]
                curr += 1
                IncludedServices.NbOfIncludedServices = data.contents[curr]
                curr += 1
                # Add sub-object to list container.
                DiscoveredServices.IncludedServices.append(copy.deepcopy(IncludedServices))

            # Add sub-object to list container.
            frame.DiscoveredServices.append(copy.deepcopy(DiscoveredServices))

        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTClientProcedureDiscoverPrimaryServicesByUuidIndicationObserver(Observer):

    opGroup = Spec.GATTClientProcedureDiscoverPrimaryServicesByUuidIndicationFrame.opGroup
    opCode = Spec.GATTClientProcedureDiscoverPrimaryServicesByUuidIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTClientProcedureDiscoverPrimaryServicesByUuidIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTClientProcedureDiscoverPrimaryServicesByUuidIndication()
        curr = 0
        frame.DeviceId = data.contents[curr]
        curr += 1
        frame.ProcedureResult = data.contents[curr]
        curr += 1
        frame.Error = list_to_int(data.contents[curr:curr + 2])
        curr += 2
        if frame.ProcedureResult == 0x01:
            callback(self.deviceName, frame) if callback else print_event(self.deviceName, frame)
            fsciLibrary.DestroyFSCIFrame(event)
            return
        frame.NbOfDiscoveredServices = data.contents[curr]
        curr += 1
        frame.DiscoveredServices = []
        for _ in range(frame.NbOfDiscoveredServices):
            # Create sub-object container
            DiscoveredServices = GATTClientProcedureDiscoverPrimaryServicesByUuidIndication.DiscoveredServices()
            DiscoveredServices.StartHandle = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            DiscoveredServices.EndHandle = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            DiscoveredServices.UuidType = data.contents[curr]
            curr += 1
            if DiscoveredServices.UuidType == UuidType.Uuid16Bits:
                selected_len = 2
            elif DiscoveredServices.UuidType == UuidType.Uuid128Bits:
                selected_len = 16
            elif DiscoveredServices.UuidType == UuidType.Uuid32Bits:
                selected_len = 4
            else:
                print "Wrong UuidType value!"
            DiscoveredServices.Uuid = list_to_int(data.contents[curr:curr + selected_len])
            curr += selected_len
            DiscoveredServices.NbOfCharacteristics = data.contents[curr]
            curr += 1
            DiscoveredServices.Characteristics = []
            for _ in range(DiscoveredServices.NbOfCharacteristics):
                # Create sub-object container
                Characteristics = GATTClientProcedureDiscoverPrimaryServicesByUuidIndication.DiscoveredServices.Characteristics()
                Characteristics.Properties = data.contents[curr]
                curr += 1
                # Create sub-object container
                Characteristics.Value = GATTClientProcedureDiscoverPrimaryServicesByUuidIndication.DiscoveredServices.Characteristics.Value()
                Characteristics.Value.Handle = list_to_int(data.contents[curr:curr + 2])
                curr += 2
                Characteristics.Value.UuidType = data.contents[curr]
                curr += 1
                if Characteristics.Value.UuidType == UuidType.Uuid16Bits:
                    selected_len = 2
                elif Characteristics.Value.UuidType == UuidType.Uuid128Bits:
                    selected_len = 16
                elif Characteristics.Value.UuidType == UuidType.Uuid32Bits:
                    selected_len = 4
                else:
                    print "Wrong UuidType value!"
                Characteristics.Value.Uuid = list_to_int(data.contents[curr:curr + selected_len])
                curr += selected_len
                Characteristics.Value.ValueLength = list_to_int(data.contents[curr:curr + 2])
                curr += 2
                Characteristics.Value.MaxValueLength = list_to_int(data.contents[curr:curr + 2])
                curr += 2
                Characteristics.Value.Value = list_to_int(data.contents[curr:curr + Characteristics.Value.ValueLength])
                curr += Characteristics.Value.ValueLength
                Characteristics.NbOfDescriptors = data.contents[curr]
                curr += 1
                Characteristics.Descriptors = []
                for _ in range(Characteristics.NbOfDescriptors):
                    # Create sub-object container
                    Descriptors = GATTClientProcedureDiscoverPrimaryServicesByUuidIndication.DiscoveredServices.Characteristics.Descriptors()
                    Descriptors.Handle = list_to_int(data.contents[curr:curr + 2])
                    curr += 2
                    Descriptors.UuidType = data.contents[curr]
                    curr += 1
                    if Descriptors.UuidType == UuidType.Uuid16Bits:
                        selected_len = 2
                    elif Descriptors.UuidType == UuidType.Uuid128Bits:
                        selected_len = 16
                    elif Descriptors.UuidType == UuidType.Uuid32Bits:
                        selected_len = 4
                    else:
                        print "Wrong UuidType value!"
                    Descriptors.Uuid = list_to_int(data.contents[curr:curr + selected_len])
                    curr += selected_len
                    Descriptors.ValueLength = list_to_int(data.contents[curr:curr + 2])
                    curr += 2
                    Descriptors.MaxValueLength = list_to_int(data.contents[curr:curr + 2])
                    curr += 2
                    Descriptors.Value = list_to_int(data.contents[curr:curr + Descriptors.ValueLength])
                    curr += Descriptors.ValueLength
                    # Add sub-object to list container.
                    Characteristics.Descriptors.append(copy.deepcopy(Descriptors))

                # Add sub-object to list container.
                DiscoveredServices.Characteristics.append(copy.deepcopy(Characteristics))

            DiscoveredServices.NbOfIncludedServices = data.contents[curr]
            curr += 1
            DiscoveredServices.IncludedServices = []
            for _ in range(DiscoveredServices.NbOfIncludedServices):
                # Create sub-object container
                IncludedServices = GATTClientProcedureDiscoverPrimaryServicesByUuidIndication.DiscoveredServices.IncludedServices()
                IncludedServices.StartHandle = list_to_int(data.contents[curr:curr + 2])
                curr += 2
                IncludedServices.EndHandle = list_to_int(data.contents[curr:curr + 2])
                curr += 2
                IncludedServices.UuidType = data.contents[curr]
                curr += 1
                if IncludedServices.UuidType == UuidType.Uuid16Bits:
                    selected_len = 2
                elif IncludedServices.UuidType == UuidType.Uuid128Bits:
                    selected_len = 16
                elif IncludedServices.UuidType == UuidType.Uuid32Bits:
                    selected_len = 4
                else:
                    print "Wrong UuidType value!"
                IncludedServices.Uuid = list_to_int(data.contents[curr:curr + selected_len])
                curr += selected_len
                IncludedServices.NbOfCharacteristics = data.contents[curr]
                curr += 1
                IncludedServices.NbOfIncludedServices = data.contents[curr]
                curr += 1
                # Add sub-object to list container.
                DiscoveredServices.IncludedServices.append(copy.deepcopy(IncludedServices))

            # Add sub-object to list container.
            frame.DiscoveredServices.append(copy.deepcopy(DiscoveredServices))

        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTClientProcedureFindIncludedServicesIndicationObserver(Observer):

    opGroup = Spec.GATTClientProcedureFindIncludedServicesIndicationFrame.opGroup
    opCode = Spec.GATTClientProcedureFindIncludedServicesIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTClientProcedureFindIncludedServicesIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTClientProcedureFindIncludedServicesIndication()
        curr = 0
        frame.DeviceId = data.contents[curr]
        curr += 1
        frame.ProcedureResult = data.contents[curr]
        curr += 1
        frame.Error = list_to_int(data.contents[curr:curr + 2])
        curr += 2
        if frame.ProcedureResult == 0x01:
            callback(self.deviceName, frame) if callback else print_event(self.deviceName, frame)
            fsciLibrary.DestroyFSCIFrame(event)
            return
        frame.Service_StartHandle = list_to_int(data.contents[curr:curr + 2])
        curr += 2
        frame.Service_EndHandle = list_to_int(data.contents[curr:curr + 2])
        curr += 2
        frame.Service_UuidType = data.contents[curr]
        curr += 1
        frame.Service_Uuid = list_to_int(data.contents[curr:curr + 0])
        if frame.Service_UuidType == UuidType.Uuid16Bits:
            selected_len = 2
        elif frame.Service_UuidType == UuidType.Uuid128Bits:
            selected_len = 16
        elif frame.Service_UuidType == UuidType.Uuid32Bits:
            selected_len = 4
        else:
            print "Wrong UuidType value!"
        curr += selected_len
        frame.Service_NbOfCharacteristics = data.contents[curr]
        curr += 1
        frame.Service_Characteristics = []
        for _ in range(frame.Service_NbOfCharacteristics):
            # Create sub-object container
            Service_Characteristics = GATTClientProcedureFindIncludedServicesIndication.Service_Characteristics()
            Service_Characteristics.Properties = data.contents[curr]
            curr += 1
            # Create sub-object container
            Service_Characteristics.Value = GATTClientProcedureFindIncludedServicesIndication.Service_Characteristics.Value()
            Service_Characteristics.Value.Handle = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            Service_Characteristics.Value.UuidType = data.contents[curr]
            curr += 1
            if Service_Characteristics.Value.UuidType == UuidType.Uuid16Bits:
                selected_len = 2
            elif Service_Characteristics.Value.UuidType == UuidType.Uuid128Bits:
                selected_len = 16
            elif Service_Characteristics.Value.UuidType == UuidType.Uuid32Bits:
                selected_len = 4
            else:
                print "Wrong UuidType value!"
            Service_Characteristics.Value.Uuid = list_to_int(data.contents[curr:curr + selected_len])
            curr += selected_len
            Service_Characteristics.Value.ValueLength = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            Service_Characteristics.Value.MaxValueLength = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            Service_Characteristics.Value.Value = list_to_int(data.contents[curr:curr + Service_Characteristics.Value.ValueLength])
            curr += Service_Characteristics.Value.ValueLength
            Service_Characteristics.NbOfDescriptors = data.contents[curr]
            curr += 1
            Service_Characteristics.Descriptors = []
            for _ in range(Service_Characteristics.NbOfDescriptors):
                # Create sub-object container
                Descriptors = GATTClientProcedureFindIncludedServicesIndication.Service_Characteristics.Descriptors()
                Descriptors.Handle = list_to_int(data.contents[curr:curr + 2])
                curr += 2
                Descriptors.UuidType = data.contents[curr]
                curr += 1
                if Descriptors.UuidType == UuidType.Uuid16Bits:
                    selected_len = 2
                elif Descriptors.UuidType == UuidType.Uuid128Bits:
                    selected_len = 16
                elif Descriptors.UuidType == UuidType.Uuid32Bits:
                    selected_len = 4
                else:
                    print "Wrong UuidType value!"
                Descriptors.Uuid = list_to_int(data.contents[curr:curr + selected_len])
                curr += selected_len
                Descriptors.ValueLength = list_to_int(data.contents[curr:curr + 2])
                curr += 2
                Descriptors.MaxValueLength = list_to_int(data.contents[curr:curr + 2])
                curr += 2
                Descriptors.Value = list_to_int(data.contents[curr:curr + Descriptors.ValueLength])
                curr += Descriptors.ValueLength
                # Add sub-object to list container.
                Service_Characteristics.Descriptors.append(copy.deepcopy(Descriptors))

            # Add sub-object to list container.
            frame.Service_Characteristics.append(copy.deepcopy(Service_Characteristics))

        frame.Service_NbOfIncludedServices = data.contents[curr]
        curr += 1
        frame.Service_IncludedServices = []
        for _ in range(frame.Service_NbOfIncludedServices):
            # Create sub-object container
            Service_IncludedServices = GATTClientProcedureFindIncludedServicesIndication.Service_IncludedServices()
            Service_IncludedServices.StartHandle = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            Service_IncludedServices.EndHandle = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            Service_IncludedServices.UuidType = data.contents[curr]
            curr += 1
            if Service_IncludedServices.UuidType == UuidType.Uuid16Bits:
                selected_len = 2
            elif Service_IncludedServices.UuidType == UuidType.Uuid128Bits:
                selected_len = 16
            elif Service_IncludedServices.UuidType == UuidType.Uuid32Bits:
                selected_len = 4
            else:
                print "Wrong UuidType value!"
            Service_IncludedServices.Uuid = list_to_int(data.contents[curr:curr + selected_len])
            curr += selected_len
            Service_IncludedServices.NbOfCharacteristics = data.contents[curr]
            curr += 1
            Service_IncludedServices.NbOfIncludedServices = data.contents[curr]
            curr += 1
            # Add sub-object to list container.
            frame.Service_IncludedServices.append(copy.deepcopy(Service_IncludedServices))

        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTClientProcedureDiscoverAllCharacteristicsIndicationObserver(Observer):

    opGroup = Spec.GATTClientProcedureDiscoverAllCharacteristicsIndicationFrame.opGroup
    opCode = Spec.GATTClientProcedureDiscoverAllCharacteristicsIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTClientProcedureDiscoverAllCharacteristicsIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTClientProcedureDiscoverAllCharacteristicsIndication()
        curr = 0
        frame.DeviceId = data.contents[curr]
        curr += 1
        frame.ProcedureResult = data.contents[curr]
        curr += 1
        frame.Error = list_to_int(data.contents[curr:curr + 2])
        curr += 2
        if frame.ProcedureResult == 0x01:
            callback(self.deviceName, frame) if callback else print_event(self.deviceName, frame)
            fsciLibrary.DestroyFSCIFrame(event)
            return
        frame.Service_StartHandle = list_to_int(data.contents[curr:curr + 2])
        curr += 2
        frame.Service_EndHandle = list_to_int(data.contents[curr:curr + 2])
        curr += 2
        frame.Service_UuidType = data.contents[curr]
        curr += 1
        frame.Service_Uuid = list_to_int(data.contents[curr:curr + 0])
        if frame.Service_UuidType == UuidType.Uuid16Bits:
            selected_len = 2
        elif frame.Service_UuidType == UuidType.Uuid128Bits:
            selected_len = 16
        elif frame.Service_UuidType == UuidType.Uuid32Bits:
            selected_len = 4
        else:
            print "Wrong UuidType value!"
        curr += selected_len
        frame.Service_NbOfCharacteristics = data.contents[curr]
        curr += 1
        frame.Service_Characteristics = []
        for _ in range(frame.Service_NbOfCharacteristics):
            # Create sub-object container
            Service_Characteristics = GATTClientProcedureDiscoverAllCharacteristicsIndication.Service_Characteristics()
            Service_Characteristics.Properties = data.contents[curr]
            curr += 1
            # Create sub-object container
            Service_Characteristics.Value = GATTClientProcedureDiscoverAllCharacteristicsIndication.Service_Characteristics.Value()
            Service_Characteristics.Value.Handle = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            Service_Characteristics.Value.UuidType = data.contents[curr]
            curr += 1
            if Service_Characteristics.Value.UuidType == UuidType.Uuid16Bits:
                selected_len = 2
            elif Service_Characteristics.Value.UuidType == UuidType.Uuid128Bits:
                selected_len = 16
            elif Service_Characteristics.Value.UuidType == UuidType.Uuid32Bits:
                selected_len = 4
            else:
                print "Wrong UuidType value!"
            Service_Characteristics.Value.Uuid = list_to_int(data.contents[curr:curr + selected_len])
            curr += selected_len
            Service_Characteristics.Value.ValueLength = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            Service_Characteristics.Value.MaxValueLength = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            Service_Characteristics.Value.Value = list_to_int(data.contents[curr:curr + Service_Characteristics.Value.ValueLength])
            curr += Service_Characteristics.Value.ValueLength
            Service_Characteristics.NbOfDescriptors = data.contents[curr]
            curr += 1
            Service_Characteristics.Descriptors = []
            for _ in range(Service_Characteristics.NbOfDescriptors):
                # Create sub-object container
                Descriptors = GATTClientProcedureDiscoverAllCharacteristicsIndication.Service_Characteristics.Descriptors()
                Descriptors.Handle = list_to_int(data.contents[curr:curr + 2])
                curr += 2
                Descriptors.UuidType = data.contents[curr]
                curr += 1
                if Descriptors.UuidType == UuidType.Uuid16Bits:
                    selected_len = 2
                elif Descriptors.UuidType == UuidType.Uuid128Bits:
                    selected_len = 16
                elif Descriptors.UuidType == UuidType.Uuid32Bits:
                    selected_len = 4
                else:
                    print "Wrong UuidType value!"
                Descriptors.Uuid = list_to_int(data.contents[curr:curr + selected_len])
                curr += selected_len
                Descriptors.ValueLength = list_to_int(data.contents[curr:curr + 2])
                curr += 2
                Descriptors.MaxValueLength = list_to_int(data.contents[curr:curr + 2])
                curr += 2
                Descriptors.Value = list_to_int(data.contents[curr:curr + Descriptors.ValueLength])
                curr += Descriptors.ValueLength
                # Add sub-object to list container.
                Service_Characteristics.Descriptors.append(copy.deepcopy(Descriptors))

            # Add sub-object to list container.
            frame.Service_Characteristics.append(copy.deepcopy(Service_Characteristics))

        frame.Service_NbOfIncludedServices = data.contents[curr]
        curr += 1
        frame.Service_IncludedServices = []
        for _ in range(frame.Service_NbOfIncludedServices):
            # Create sub-object container
            Service_IncludedServices = GATTClientProcedureDiscoverAllCharacteristicsIndication.Service_IncludedServices()
            Service_IncludedServices.StartHandle = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            Service_IncludedServices.EndHandle = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            Service_IncludedServices.UuidType = data.contents[curr]
            curr += 1
            if Service_IncludedServices.UuidType == UuidType.Uuid16Bits:
                selected_len = 2
            elif Service_IncludedServices.UuidType == UuidType.Uuid128Bits:
                selected_len = 16
            elif Service_IncludedServices.UuidType == UuidType.Uuid32Bits:
                selected_len = 4
            else:
                print "Wrong UuidType value!"
            Service_IncludedServices.Uuid = list_to_int(data.contents[curr:curr + selected_len])
            curr += selected_len
            Service_IncludedServices.NbOfCharacteristics = data.contents[curr]
            curr += 1
            Service_IncludedServices.NbOfIncludedServices = data.contents[curr]
            curr += 1
            # Add sub-object to list container.
            frame.Service_IncludedServices.append(copy.deepcopy(Service_IncludedServices))

        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTClientProcedureDiscoverCharacteristicByUuidIndicationObserver(Observer):

    opGroup = Spec.GATTClientProcedureDiscoverCharacteristicByUuidIndicationFrame.opGroup
    opCode = Spec.GATTClientProcedureDiscoverCharacteristicByUuidIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTClientProcedureDiscoverCharacteristicByUuidIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTClientProcedureDiscoverCharacteristicByUuidIndication()
        curr = 0
        frame.DeviceId = data.contents[curr]
        curr += 1
        frame.ProcedureResult = data.contents[curr]
        curr += 1
        frame.Error = list_to_int(data.contents[curr:curr + 2])
        curr += 2
        if frame.ProcedureResult == 0x01:
            callback(self.deviceName, frame) if callback else print_event(self.deviceName, frame)
            fsciLibrary.DestroyFSCIFrame(event)
            return
        frame.NbOfCharacteristics = data.contents[curr]
        curr += 1
        frame.Characteristics = []
        for _ in range(frame.NbOfCharacteristics):
            # Create sub-object container
            Characteristics = GATTClientProcedureDiscoverCharacteristicByUuidIndication.Characteristics()
            Characteristics.Properties = data.contents[curr]
            curr += 1
            # Create sub-object container
            Characteristics.Value = GATTClientProcedureDiscoverCharacteristicByUuidIndication.Characteristics.Value()
            Characteristics.Value.Handle = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            Characteristics.Value.UuidType = data.contents[curr]
            curr += 1
            if Characteristics.Value.UuidType == UuidType.Uuid16Bits:
                selected_len = 2
            elif Characteristics.Value.UuidType == UuidType.Uuid128Bits:
                selected_len = 16
            elif Characteristics.Value.UuidType == UuidType.Uuid32Bits:
                selected_len = 4
            else:
                print "Wrong UuidType value!"
            Characteristics.Value.Uuid = list_to_int(data.contents[curr:curr + selected_len])
            curr += selected_len
            Characteristics.Value.ValueLength = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            Characteristics.Value.MaxValueLength = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            Characteristics.Value.Value = list_to_int(data.contents[curr:curr + Characteristics.Value.ValueLength])
            curr += Characteristics.Value.ValueLength
            Characteristics.NbOfDescriptors = data.contents[curr]
            curr += 1
            Characteristics.Descriptors = []
            for _ in range(Characteristics.NbOfDescriptors):
                # Create sub-object container
                Descriptors = GATTClientProcedureDiscoverCharacteristicByUuidIndication.Characteristics.Descriptors()
                Descriptors.Handle = list_to_int(data.contents[curr:curr + 2])
                curr += 2
                Descriptors.UuidType = data.contents[curr]
                curr += 1
                if Descriptors.UuidType == UuidType.Uuid16Bits:
                    selected_len = 2
                elif Descriptors.UuidType == UuidType.Uuid128Bits:
                    selected_len = 16
                elif Descriptors.UuidType == UuidType.Uuid32Bits:
                    selected_len = 4
                else:
                    print "Wrong UuidType value!"
                Descriptors.Uuid = list_to_int(data.contents[curr:curr + selected_len])
                curr += selected_len
                Descriptors.ValueLength = list_to_int(data.contents[curr:curr + 2])
                curr += 2
                Descriptors.MaxValueLength = list_to_int(data.contents[curr:curr + 2])
                curr += 2
                Descriptors.Value = list_to_int(data.contents[curr:curr + Descriptors.ValueLength])
                curr += Descriptors.ValueLength
                # Add sub-object to list container.
                Characteristics.Descriptors.append(copy.deepcopy(Descriptors))

            # Add sub-object to list container.
            frame.Characteristics.append(copy.deepcopy(Characteristics))

        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTClientProcedureDiscoverAllCharacteristicDescriptorsIndicationObserver(Observer):

    opGroup = Spec.GATTClientProcedureDiscoverAllCharacteristicDescriptorsIndicationFrame.opGroup
    opCode = Spec.GATTClientProcedureDiscoverAllCharacteristicDescriptorsIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTClientProcedureDiscoverAllCharacteristicDescriptorsIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTClientProcedureDiscoverAllCharacteristicDescriptorsIndication()
        curr = 0
        frame.DeviceId = data.contents[curr]
        curr += 1
        frame.ProcedureResult = data.contents[curr]
        curr += 1
        frame.Error = list_to_int(data.contents[curr:curr + 2])
        curr += 2
        if frame.ProcedureResult == 0x01:
            callback(self.deviceName, frame) if callback else print_event(self.deviceName, frame)
            fsciLibrary.DestroyFSCIFrame(event)
            return
        frame.Characteristic_Properties = data.contents[curr]
        curr += 1
        frame.Characteristic_Value.Handle = list_to_int(data.contents[curr:curr + 2])
        curr += 2
        frame.Characteristic_Value.UuidType = data.contents[curr]
        curr += 1
        frame.Characteristic_Value.Uuid = list_to_int(data.contents[curr:curr + 0])
        if frame.Characteristic_Value.UuidType == UuidType.Uuid16Bits:
            selected_len = 2
        elif frame.Characteristic_Value.UuidType == UuidType.Uuid128Bits:
            selected_len = 16
        elif frame.Characteristic_Value.UuidType == UuidType.Uuid32Bits:
            selected_len = 4
        else:
            print "Wrong UuidType value!"
        curr += selected_len
        frame.Characteristic_Value.ValueLength = list_to_int(data.contents[curr:curr + 2])
        curr += 2
        frame.Characteristic_Value.MaxValueLength = list_to_int(data.contents[curr:curr + 2])
        curr += 2
        frame.Characteristic_Value.Value = data.contents[curr]
        curr += 1
        frame.Characteristic_NbOfDescriptors = data.contents[curr]
        curr += 1
        frame.Characteristic_Descriptors = []
        for _ in range(frame.Characteristic_NbOfDescriptors):
            # Create sub-object container
            Characteristic_Descriptors = GATTClientProcedureDiscoverAllCharacteristicDescriptorsIndication.Characteristic_Descriptors()
            Characteristic_Descriptors.Handle = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            Characteristic_Descriptors.UuidType = data.contents[curr]
            curr += 1
            if Characteristic_Descriptors.UuidType == UuidType.Uuid16Bits:
                selected_len = 2
            elif Characteristic_Descriptors.UuidType == UuidType.Uuid128Bits:
                selected_len = 16
            elif Characteristic_Descriptors.UuidType == UuidType.Uuid32Bits:
                selected_len = 4
            else:
                print "Wrong UuidType value!"
            Characteristic_Descriptors.Uuid = list_to_int(data.contents[curr:curr + selected_len])
            curr += selected_len
            Characteristic_Descriptors.ValueLength = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            Characteristic_Descriptors.MaxValueLength = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            Characteristic_Descriptors.Value = list_to_int(data.contents[curr:curr + Characteristic_Descriptors.ValueLength])
            curr += Characteristic_Descriptors.ValueLength
            # Add sub-object to list container.
            frame.Characteristic_Descriptors.append(copy.deepcopy(Characteristic_Descriptors))

        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTClientProcedureReadCharacteristicValueIndicationObserver(Observer):

    opGroup = Spec.GATTClientProcedureReadCharacteristicValueIndicationFrame.opGroup
    opCode = Spec.GATTClientProcedureReadCharacteristicValueIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTClientProcedureReadCharacteristicValueIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTClientProcedureReadCharacteristicValueIndication()
        curr = 0
        frame.DeviceId = data.contents[curr]
        curr += 1
        frame.ProcedureResult = data.contents[curr]
        curr += 1
        frame.Error = list_to_int(data.contents[curr:curr + 2])
        curr += 2
        if frame.ProcedureResult == 0x01:
            callback(self.deviceName, frame) if callback else print_event(self.deviceName, frame)
            fsciLibrary.DestroyFSCIFrame(event)
            return
        frame.Characteristic_Properties = data.contents[curr]
        curr += 1
        frame.Characteristic_Value.Handle = list_to_int(data.contents[curr:curr + 2])
        curr += 2
        frame.Characteristic_Value.UuidType = data.contents[curr]
        curr += 1
        frame.Characteristic_Value.Uuid = list_to_int(data.contents[curr:curr + 0])
        if frame.Characteristic_Value.UuidType == UuidType.Uuid16Bits:
            selected_len = 2
        elif frame.Characteristic_Value.UuidType == UuidType.Uuid128Bits:
            selected_len = 16
        elif frame.Characteristic_Value.UuidType == UuidType.Uuid32Bits:
            selected_len = 4
        else:
            print "Wrong UuidType value!"
        curr += selected_len
        frame.Characteristic_Value.ValueLength = list_to_int(data.contents[curr:curr + 2])
        curr += 2
        frame.Characteristic_Value.MaxValueLength = list_to_int(data.contents[curr:curr + 2])
        curr += 2
        frame.Characteristic_Value.Value = data.contents[curr]
        curr += 1
        frame.Characteristic_NbOfDescriptors = data.contents[curr]
        curr += 1
        frame.Characteristic_Descriptors = []
        for _ in range(frame.Characteristic_NbOfDescriptors):
            # Create sub-object container
            Characteristic_Descriptors = GATTClientProcedureReadCharacteristicValueIndication.Characteristic_Descriptors()
            Characteristic_Descriptors.Handle = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            Characteristic_Descriptors.UuidType = data.contents[curr]
            curr += 1
            if Characteristic_Descriptors.UuidType == UuidType.Uuid16Bits:
                selected_len = 2
            elif Characteristic_Descriptors.UuidType == UuidType.Uuid128Bits:
                selected_len = 16
            elif Characteristic_Descriptors.UuidType == UuidType.Uuid32Bits:
                selected_len = 4
            else:
                print "Wrong UuidType value!"
            Characteristic_Descriptors.Uuid = list_to_int(data.contents[curr:curr + selected_len])
            curr += selected_len
            Characteristic_Descriptors.ValueLength = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            Characteristic_Descriptors.MaxValueLength = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            Characteristic_Descriptors.Value = list_to_int(data.contents[curr:curr + Characteristic_Descriptors.ValueLength])
            curr += Characteristic_Descriptors.ValueLength
            # Add sub-object to list container.
            frame.Characteristic_Descriptors.append(copy.deepcopy(Characteristic_Descriptors))

        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTClientProcedureReadUsingCharacteristicUuidIndicationObserver(Observer):

    opGroup = Spec.GATTClientProcedureReadUsingCharacteristicUuidIndicationFrame.opGroup
    opCode = Spec.GATTClientProcedureReadUsingCharacteristicUuidIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTClientProcedureReadUsingCharacteristicUuidIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTClientProcedureReadUsingCharacteristicUuidIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.ProcedureResult = packet.getParamValueAsNumber("ProcedureResult")
        frame.Error = packet.getParamValueAsNumber("Error")
        if frame.ProcedureResult == 0x01:
            callback(self.deviceName, frame) if callback else print_event(self.deviceName, frame)
            fsciLibrary.DestroyFSCIFrame(event)
            return
        frame.NbOfReadBytes = packet.getParamValueAsNumber("NbOfReadBytes")
        frame.ReadBytes = packet.getParamValueAsList("ReadBytes")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTClientProcedureReadMultipleCharacteristicValuesIndicationObserver(Observer):

    opGroup = Spec.GATTClientProcedureReadMultipleCharacteristicValuesIndicationFrame.opGroup
    opCode = Spec.GATTClientProcedureReadMultipleCharacteristicValuesIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTClientProcedureReadMultipleCharacteristicValuesIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTClientProcedureReadMultipleCharacteristicValuesIndication()
        curr = 0
        frame.DeviceId = data.contents[curr]
        curr += 1
        frame.ProcedureResult = data.contents[curr]
        curr += 1
        frame.Error = list_to_int(data.contents[curr:curr + 2])
        curr += 2
        if frame.ProcedureResult == 0x01:
            callback(self.deviceName, frame) if callback else print_event(self.deviceName, frame)
            fsciLibrary.DestroyFSCIFrame(event)
            return
        frame.NbOfCharacteristics = data.contents[curr]
        curr += 1
        frame.Characteristics = []
        for _ in range(frame.NbOfCharacteristics):
            # Create sub-object container
            Characteristics = GATTClientProcedureReadMultipleCharacteristicValuesIndication.Characteristics()
            Characteristics.Properties = data.contents[curr]
            curr += 1
            # Create sub-object container
            Characteristics.Value = GATTClientProcedureReadMultipleCharacteristicValuesIndication.Characteristics.Value()
            Characteristics.Value.Handle = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            Characteristics.Value.UuidType = data.contents[curr]
            curr += 1
            if Characteristics.Value.UuidType == UuidType.Uuid16Bits:
                selected_len = 2
            elif Characteristics.Value.UuidType == UuidType.Uuid128Bits:
                selected_len = 16
            elif Characteristics.Value.UuidType == UuidType.Uuid32Bits:
                selected_len = 4
            else:
                print "Wrong UuidType value!"
            Characteristics.Value.Uuid = list_to_int(data.contents[curr:curr + selected_len])
            curr += selected_len
            Characteristics.Value.ValueLength = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            Characteristics.Value.MaxValueLength = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            Characteristics.Value.Value = list_to_int(data.contents[curr:curr + Characteristics.Value.ValueLength])
            curr += Characteristics.Value.ValueLength
            Characteristics.NbOfDescriptors = data.contents[curr]
            curr += 1
            Characteristics.Descriptors = []
            for _ in range(Characteristics.NbOfDescriptors):
                # Create sub-object container
                Descriptors = GATTClientProcedureReadMultipleCharacteristicValuesIndication.Characteristics.Descriptors()
                Descriptors.Handle = list_to_int(data.contents[curr:curr + 2])
                curr += 2
                Descriptors.UuidType = data.contents[curr]
                curr += 1
                if Descriptors.UuidType == UuidType.Uuid16Bits:
                    selected_len = 2
                elif Descriptors.UuidType == UuidType.Uuid128Bits:
                    selected_len = 16
                elif Descriptors.UuidType == UuidType.Uuid32Bits:
                    selected_len = 4
                else:
                    print "Wrong UuidType value!"
                Descriptors.Uuid = list_to_int(data.contents[curr:curr + selected_len])
                curr += selected_len
                Descriptors.ValueLength = list_to_int(data.contents[curr:curr + 2])
                curr += 2
                Descriptors.MaxValueLength = list_to_int(data.contents[curr:curr + 2])
                curr += 2
                Descriptors.Value = list_to_int(data.contents[curr:curr + Descriptors.ValueLength])
                curr += Descriptors.ValueLength
                # Add sub-object to list container.
                Characteristics.Descriptors.append(copy.deepcopy(Descriptors))

            # Add sub-object to list container.
            frame.Characteristics.append(copy.deepcopy(Characteristics))

        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTClientProcedureWriteCharacteristicValueIndicationObserver(Observer):

    opGroup = Spec.GATTClientProcedureWriteCharacteristicValueIndicationFrame.opGroup
    opCode = Spec.GATTClientProcedureWriteCharacteristicValueIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTClientProcedureWriteCharacteristicValueIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTClientProcedureWriteCharacteristicValueIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.ProcedureResult = packet.getParamValueAsNumber("ProcedureResult")
        frame.Error = packet.getParamValueAsNumber("Error")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTClientProcedureReadCharacteristicDescriptorIndicationObserver(Observer):

    opGroup = Spec.GATTClientProcedureReadCharacteristicDescriptorIndicationFrame.opGroup
    opCode = Spec.GATTClientProcedureReadCharacteristicDescriptorIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTClientProcedureReadCharacteristicDescriptorIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTClientProcedureReadCharacteristicDescriptorIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.ProcedureResult = packet.getParamValueAsNumber("ProcedureResult")
        frame.Error = packet.getParamValueAsNumber("Error")
        if frame.ProcedureResult == 0x01:
            callback(self.deviceName, frame) if callback else print_event(self.deviceName, frame)
            fsciLibrary.DestroyFSCIFrame(event)
            return
        frame.Descriptor_Handle = packet.getParamValueAsNumber("Descriptor_Handle")
        frame.Descriptor_UuidType = packet.getParamValueAsNumber("Descriptor_UuidType")
        frame.Descriptor_Uuid = packet.getParamValueAsNumber("Descriptor_Uuid")
        frame.Descriptor_ValueLength = packet.getParamValueAsNumber("Descriptor_ValueLength")
        frame.Descriptor_MaxValueLength = packet.getParamValueAsNumber("Descriptor_MaxValueLength")
        frame.Descriptor_Value = packet.getParamValueAsList("Descriptor_Value")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTClientProcedureWriteCharacteristicDescriptorIndicationObserver(Observer):

    opGroup = Spec.GATTClientProcedureWriteCharacteristicDescriptorIndicationFrame.opGroup
    opCode = Spec.GATTClientProcedureWriteCharacteristicDescriptorIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTClientProcedureWriteCharacteristicDescriptorIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTClientProcedureWriteCharacteristicDescriptorIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.ProcedureResult = packet.getParamValueAsNumber("ProcedureResult")
        frame.Error = packet.getParamValueAsNumber("Error")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTClientNotificationIndicationObserver(Observer):

    opGroup = Spec.GATTClientNotificationIndicationFrame.opGroup
    opCode = Spec.GATTClientNotificationIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTClientNotificationIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTClientNotificationIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.CharacteristicValueHandle = packet.getParamValueAsNumber("CharacteristicValueHandle")
        frame.ValueLength = packet.getParamValueAsNumber("ValueLength")
        frame.Value = packet.getParamValueAsList("Value")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTClientIndicationIndicationObserver(Observer):

    opGroup = Spec.GATTClientIndicationIndicationFrame.opGroup
    opCode = Spec.GATTClientIndicationIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTClientIndicationIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTClientIndicationIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.CharacteristicValueHandle = packet.getParamValueAsNumber("CharacteristicValueHandle")
        frame.ValueLength = packet.getParamValueAsNumber("ValueLength")
        frame.Value = packet.getParamValueAsList("Value")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTServerMtuChangedIndicationObserver(Observer):

    opGroup = Spec.GATTServerMtuChangedIndicationFrame.opGroup
    opCode = Spec.GATTServerMtuChangedIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTServerMtuChangedIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTServerMtuChangedIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.MtuChangedEvent_NewMtu = packet.getParamValueAsNumber("MtuChangedEvent_NewMtu")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTServerHandleValueConfirmationIndicationObserver(Observer):

    opGroup = Spec.GATTServerHandleValueConfirmationIndicationFrame.opGroup
    opCode = Spec.GATTServerHandleValueConfirmationIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTServerHandleValueConfirmationIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTServerHandleValueConfirmationIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTServerAttributeWrittenIndicationObserver(Observer):

    opGroup = Spec.GATTServerAttributeWrittenIndicationFrame.opGroup
    opCode = Spec.GATTServerAttributeWrittenIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTServerAttributeWrittenIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTServerAttributeWrittenIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.AttributeWrittenEvent_Handle = packet.getParamValueAsNumber("AttributeWrittenEvent_Handle")
        frame.AttributeWrittenEvent_ValueLength = packet.getParamValueAsNumber("AttributeWrittenEvent_ValueLength")
        frame.AttributeWrittenEvent_Value = packet.getParamValueAsList("AttributeWrittenEvent_Value")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTServerCharacteristicCccdWrittenIndicationObserver(Observer):

    opGroup = Spec.GATTServerCharacteristicCccdWrittenIndicationFrame.opGroup
    opCode = Spec.GATTServerCharacteristicCccdWrittenIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTServerCharacteristicCccdWrittenIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTServerCharacteristicCccdWrittenIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.CharCccdWrittenEvent_Handle = packet.getParamValueAsNumber("CharCccdWrittenEvent_Handle")
        frame.CharCccdWrittenEvent_NewCccd = packet.getParamValueAsNumber("CharCccdWrittenEvent_NewCccd")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTServerAttributeWrittenWithoutResponseIndicationObserver(Observer):

    opGroup = Spec.GATTServerAttributeWrittenWithoutResponseIndicationFrame.opGroup
    opCode = Spec.GATTServerAttributeWrittenWithoutResponseIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTServerAttributeWrittenWithoutResponseIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTServerAttributeWrittenWithoutResponseIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.AttributeWrittenEvent_Handle = packet.getParamValueAsNumber("AttributeWrittenEvent_Handle")
        frame.AttributeWrittenEvent_ValueLength = packet.getParamValueAsNumber("AttributeWrittenEvent_ValueLength")
        frame.AttributeWrittenEvent_Value = packet.getParamValueAsList("AttributeWrittenEvent_Value")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTServerErrorIndicationObserver(Observer):

    opGroup = Spec.GATTServerErrorIndicationFrame.opGroup
    opCode = Spec.GATTServerErrorIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTServerErrorIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTServerErrorIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.ProcedureError_ProcedureType = GATTServerErrorIndicationProcedureError_ProcedureType.getEnumString(packet.getParamValueAsNumber("ProcedureError_ProcedureType"))
        frame.ProcedureError_Error = GATTServerErrorIndicationProcedureError_Error.getEnumString(packet.getParamValueAsNumber("ProcedureError_Error"))
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTServerLongCharacteristicWrittenIndicationObserver(Observer):

    opGroup = Spec.GATTServerLongCharacteristicWrittenIndicationFrame.opGroup
    opCode = Spec.GATTServerLongCharacteristicWrittenIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTServerLongCharacteristicWrittenIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTServerLongCharacteristicWrittenIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.LongCharacteristicWrittenEvent_Handle = packet.getParamValueAsNumber("LongCharacteristicWrittenEvent_Handle")
        frame.LongCharacteristicWrittenEvent_ValueLength = packet.getParamValueAsNumber("LongCharacteristicWrittenEvent_ValueLength")
        frame.LongCharacteristicWrittenEvent_Value = packet.getParamValueAsList("LongCharacteristicWrittenEvent_Value")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTServerAttributeReadIndicationObserver(Observer):

    opGroup = Spec.GATTServerAttributeReadIndicationFrame.opGroup
    opCode = Spec.GATTServerAttributeReadIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTServerAttributeReadIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTServerAttributeReadIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.AttributeReadEvent_Handle = packet.getParamValueAsNumber("AttributeReadEvent_Handle")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTDBConfirmObserver(Observer):

    opGroup = Spec.GATTDBConfirmFrame.opGroup
    opCode = Spec.GATTDBConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTDBConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTDBConfirm()
        frame.Status = GATTDBConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        if re.match(r'gGattDb.*NotFound_c', frame.Status):
            print '\t[error] GATTDBConfirm w/ Status ' + frame.Status
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
            if frame.Status != 'gBleSuccess_c':
                print '\t[warning] GATTDBConfirm w/ Status ' + str(frame.Status)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTDBReadAttributeIndicationObserver(Observer):

    opGroup = Spec.GATTDBReadAttributeIndicationFrame.opGroup
    opCode = Spec.GATTDBReadAttributeIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTDBReadAttributeIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTDBReadAttributeIndication()
        frame.ValueLength = packet.getParamValueAsNumber("ValueLength")
        frame.Value = packet.getParamValueAsList("Value")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTDBFindServiceHandleIndicationObserver(Observer):

    opGroup = Spec.GATTDBFindServiceHandleIndicationFrame.opGroup
    opCode = Spec.GATTDBFindServiceHandleIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTDBFindServiceHandleIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTDBFindServiceHandleIndication()
        frame.ServiceHandle = packet.getParamValueAsNumber("ServiceHandle")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTDBFindCharValueHandleInServiceIndicationObserver(Observer):

    opGroup = Spec.GATTDBFindCharValueHandleInServiceIndicationFrame.opGroup
    opCode = Spec.GATTDBFindCharValueHandleInServiceIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTDBFindCharValueHandleInServiceIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTDBFindCharValueHandleInServiceIndication()
        frame.CharValueHandle = packet.getParamValueAsNumber("CharValueHandle")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTDBFindCccdHandleForCharValueHandleIndicationObserver(Observer):

    opGroup = Spec.GATTDBFindCccdHandleForCharValueHandleIndicationFrame.opGroup
    opCode = Spec.GATTDBFindCccdHandleForCharValueHandleIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTDBFindCccdHandleForCharValueHandleIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTDBFindCccdHandleForCharValueHandleIndication()
        frame.CccdHandle = packet.getParamValueAsNumber("CccdHandle")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTDBFindDescriptorHandleForCharValueHandleIndicationObserver(Observer):

    opGroup = Spec.GATTDBFindDescriptorHandleForCharValueHandleIndicationFrame.opGroup
    opCode = Spec.GATTDBFindDescriptorHandleForCharValueHandleIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTDBFindDescriptorHandleForCharValueHandleIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTDBFindDescriptorHandleForCharValueHandleIndication()
        frame.DescriptorHandle = packet.getParamValueAsNumber("DescriptorHandle")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTDBDynamicAddPrimaryServiceDeclarationIndicationObserver(Observer):

    opGroup = Spec.GATTDBDynamicAddPrimaryServiceDeclarationIndicationFrame.opGroup
    opCode = Spec.GATTDBDynamicAddPrimaryServiceDeclarationIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTDBDynamicAddPrimaryServiceDeclarationIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTDBDynamicAddPrimaryServiceDeclarationIndication()
        frame.ServiceHandle = packet.getParamValueAsNumber("ServiceHandle")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTDBDynamicAddSecondaryServiceDeclarationIndicationObserver(Observer):

    opGroup = Spec.GATTDBDynamicAddSecondaryServiceDeclarationIndicationFrame.opGroup
    opCode = Spec.GATTDBDynamicAddSecondaryServiceDeclarationIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTDBDynamicAddSecondaryServiceDeclarationIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTDBDynamicAddSecondaryServiceDeclarationIndication()
        frame.ServiceHandle = packet.getParamValueAsNumber("ServiceHandle")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTDBDynamicAddIncludeDeclarationIndicationObserver(Observer):

    opGroup = Spec.GATTDBDynamicAddIncludeDeclarationIndicationFrame.opGroup
    opCode = Spec.GATTDBDynamicAddIncludeDeclarationIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTDBDynamicAddIncludeDeclarationIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTDBDynamicAddIncludeDeclarationIndication()
        frame.IncludeHandle = packet.getParamValueAsNumber("IncludeHandle")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTDBDynamicAddCharacteristicDeclarationAndValueIndicationObserver(Observer):

    opGroup = Spec.GATTDBDynamicAddCharacteristicDeclarationAndValueIndicationFrame.opGroup
    opCode = Spec.GATTDBDynamicAddCharacteristicDeclarationAndValueIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTDBDynamicAddCharacteristicDeclarationAndValueIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTDBDynamicAddCharacteristicDeclarationAndValueIndication()
        frame.CharacteristicHandle = packet.getParamValueAsNumber("CharacteristicHandle")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTDBDynamicAddCharacteristicDescriptorIndicationObserver(Observer):

    opGroup = Spec.GATTDBDynamicAddCharacteristicDescriptorIndicationFrame.opGroup
    opCode = Spec.GATTDBDynamicAddCharacteristicDescriptorIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTDBDynamicAddCharacteristicDescriptorIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTDBDynamicAddCharacteristicDescriptorIndication()
        frame.DescriptorHandle = packet.getParamValueAsNumber("DescriptorHandle")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTDBDynamicAddCccdIndicationObserver(Observer):

    opGroup = Spec.GATTDBDynamicAddCccdIndicationFrame.opGroup
    opCode = Spec.GATTDBDynamicAddCccdIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTDBDynamicAddCccdIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTDBDynamicAddCccdIndication()
        frame.CCCDHandle = packet.getParamValueAsNumber("CCCDHandle")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTDBDynamicAddCharacteristicDeclarationWithUniqueValueIndicationObserver(Observer):

    opGroup = Spec.GATTDBDynamicAddCharacteristicDeclarationWithUniqueValueIndicationFrame.opGroup
    opCode = Spec.GATTDBDynamicAddCharacteristicDeclarationWithUniqueValueIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTDBDynamicAddCharacteristicDeclarationWithUniqueValueIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTDBDynamicAddCharacteristicDeclarationWithUniqueValueIndication()
        frame.CharacteristicHandle = packet.getParamValueAsNumber("CharacteristicHandle")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTDBAttConfirmObserver(Observer):

    opGroup = Spec.GATTDBAttConfirmFrame.opGroup
    opCode = Spec.GATTDBAttConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTDBAttConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTDBAttConfirm()
        frame.Status = GATTDBAttConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTDBAttFindInformationIndicationObserver(Observer):

    opGroup = Spec.GATTDBAttFindInformationIndicationFrame.opGroup
    opCode = Spec.GATTDBAttFindInformationIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTDBAttFindInformationIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTDBAttFindInformationIndication()
        frame.Params_Format = packet.getParamValueAsNumber("Params_Format")
        frame.Params_InformationData = packet.getParamValueAsNumber("Params_InformationData")
        frame.ErrorAttributeHandle = packet.getParamValueAsNumber("ErrorAttributeHandle")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTDBAttFindByTypeValueIndicationObserver(Observer):

    opGroup = Spec.GATTDBAttFindByTypeValueIndicationFrame.opGroup
    opCode = Spec.GATTDBAttFindByTypeValueIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTDBAttFindByTypeValueIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTDBAttFindByTypeValueIndication()
        curr = 0
        frame.Params_GroupCount = list_to_int(data.contents[curr:curr + 2])
        curr += 2
        frame.Params_HandleGroup = []
        for _ in range(frame.Params_GroupCount):
            # Create sub-object container
            Params_HandleGroup = GATTDBAttFindByTypeValueIndication.Params_HandleGroup()
            Params_HandleGroup.StartingHandle = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            Params_HandleGroup.EndingHandle = list_to_int(data.contents[curr:curr + 2])
            curr += 2
            # Add sub-object to list container.
            frame.Params_HandleGroup.append(copy.deepcopy(Params_HandleGroup))

        frame.ErrorAttributeHandle = list_to_int(data.contents[curr:curr + 2])
        curr += 2
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTDBAttReadByTypeIndicationObserver(Observer):

    opGroup = Spec.GATTDBAttReadByTypeIndicationFrame.opGroup
    opCode = Spec.GATTDBAttReadByTypeIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTDBAttReadByTypeIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTDBAttReadByTypeIndication()
        frame.Params_Length = packet.getParamValueAsNumber("Params_Length")
        frame.Params_AttributeDataListLength = packet.getParamValueAsNumber("Params_AttributeDataListLength")
        frame.Params_AttributeDataList = packet.getParamValueAsList("Params_AttributeDataList")
        frame.ErrorAttributeHandle = packet.getParamValueAsNumber("ErrorAttributeHandle")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTDBAttReadIndicationObserver(Observer):

    opGroup = Spec.GATTDBAttReadIndicationFrame.opGroup
    opCode = Spec.GATTDBAttReadIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTDBAttReadIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTDBAttReadIndication()
        frame.Params_AttributeLength = packet.getParamValueAsNumber("Params_AttributeLength")
        frame.Params_AttributeValue = packet.getParamValueAsList("Params_AttributeValue")
        frame.ErrorAttributeHandle = packet.getParamValueAsNumber("ErrorAttributeHandle")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTDBAttReadBlobIndicationObserver(Observer):

    opGroup = Spec.GATTDBAttReadBlobIndicationFrame.opGroup
    opCode = Spec.GATTDBAttReadBlobIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTDBAttReadBlobIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTDBAttReadBlobIndication()
        frame.ErrorAttributeHandle = packet.getParamValueAsNumber("ErrorAttributeHandle")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTDBAttReadMultipleIndicationObserver(Observer):

    opGroup = Spec.GATTDBAttReadMultipleIndicationFrame.opGroup
    opCode = Spec.GATTDBAttReadMultipleIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTDBAttReadMultipleIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTDBAttReadMultipleIndication()
        frame.Params_ListLength = packet.getParamValueAsNumber("Params_ListLength")
        frame.Params_ListOfValues = packet.getParamValueAsList("Params_ListOfValues")
        frame.ErrorAttributeHandle = packet.getParamValueAsNumber("ErrorAttributeHandle")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTDBAttReadByGroupTypeIndicationObserver(Observer):

    opGroup = Spec.GATTDBAttReadByGroupTypeIndicationFrame.opGroup
    opCode = Spec.GATTDBAttReadByGroupTypeIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTDBAttReadByGroupTypeIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTDBAttReadByGroupTypeIndication()
        frame.Params_Length = packet.getParamValueAsNumber("Params_Length")
        frame.Params_AttributeDataListLength = packet.getParamValueAsNumber("Params_AttributeDataListLength")
        frame.Params_AttributeDataList = packet.getParamValueAsList("Params_AttributeDataList")
        frame.ErrorAttributeHandle = packet.getParamValueAsNumber("ErrorAttributeHandle")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTDBAttWriteIndicationObserver(Observer):

    opGroup = Spec.GATTDBAttWriteIndicationFrame.opGroup
    opCode = Spec.GATTDBAttWriteIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTDBAttWriteIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTDBAttWriteIndication()
        frame.ErrorAttributeHandle = packet.getParamValueAsNumber("ErrorAttributeHandle")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTDBAttPrepareWriteIndicationObserver(Observer):

    opGroup = Spec.GATTDBAttPrepareWriteIndicationFrame.opGroup
    opCode = Spec.GATTDBAttPrepareWriteIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTDBAttPrepareWriteIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTDBAttPrepareWriteIndication()
        frame.Params_AttributeHandle = packet.getParamValueAsNumber("Params_AttributeHandle")
        frame.Params_ValueOffset = packet.getParamValueAsNumber("Params_ValueOffset")
        frame.Params_AttributeLength = packet.getParamValueAsNumber("Params_AttributeLength")
        frame.Params_AttributeValue = packet.getParamValueAsList("Params_AttributeValue")
        frame.ErrorAttributeHandle = packet.getParamValueAsNumber("ErrorAttributeHandle")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTDBAttExecuteWriteIndicationObserver(Observer):

    opGroup = Spec.GATTDBAttExecuteWriteIndicationFrame.opGroup
    opCode = Spec.GATTDBAttExecuteWriteIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTDBAttExecuteWriteIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTDBAttExecuteWriteIndication()
        frame.ErrorAttributeHandle = packet.getParamValueAsNumber("ErrorAttributeHandle")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTDBAttExecuteWriteFromQueueIndicationObserver(Observer):

    opGroup = Spec.GATTDBAttExecuteWriteFromQueueIndicationFrame.opGroup
    opCode = Spec.GATTDBAttExecuteWriteFromQueueIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTDBAttExecuteWriteFromQueueIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTDBAttExecuteWriteFromQueueIndication()
        frame.ErrorAttributeHandle = packet.getParamValueAsNumber("ErrorAttributeHandle")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GATTDBAttPrepareNotificationIndicationIndicationObserver(Observer):

    opGroup = Spec.GATTDBAttPrepareNotificationIndicationIndicationFrame.opGroup
    opCode = Spec.GATTDBAttPrepareNotificationIndicationIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GATTDBAttPrepareNotificationIndicationIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GATTDBAttPrepareNotificationIndicationIndication()
        frame.ErrorAttributeHandle = packet.getParamValueAsNumber("ErrorAttributeHandle")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPConfirmObserver(Observer):

    opGroup = Spec.GAPConfirmFrame.opGroup
    opCode = Spec.GAPConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPConfirm()
        frame.Status = GAPConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
            if frame.Status != 'gBleSuccess_c':
                print '\t[warning] GAPConfirm w/ Status ' + str(frame.Status)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPCheckNotificationStatusIndicationObserver(Observer):

    opGroup = Spec.GAPCheckNotificationStatusIndicationFrame.opGroup
    opCode = Spec.GAPCheckNotificationStatusIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPCheckNotificationStatusIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPCheckNotificationStatusIndication()
        frame.IsActive = packet.getParamValueAsNumber("IsActive")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPCheckIndicationStatusIndicationObserver(Observer):

    opGroup = Spec.GAPCheckIndicationStatusIndicationFrame.opGroup
    opCode = Spec.GAPCheckIndicationStatusIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPCheckIndicationStatusIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPCheckIndicationStatusIndication()
        frame.IsActive = packet.getParamValueAsNumber("IsActive")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPGetBondedStaticAddressesIndicationObserver(Observer):

    opGroup = Spec.GAPGetBondedStaticAddressesIndicationFrame.opGroup
    opCode = Spec.GAPGetBondedStaticAddressesIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPGetBondedStaticAddressesIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPGetBondedStaticAddressesIndication()
        frame.NbOfDeviceAddresses = packet.getParamValueAsNumber("NbOfDeviceAddresses")
        frame.DeviceAddresses = packet.getParamValueAsList("DeviceAddresses")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPLoadEncryptionInformationIndicationObserver(Observer):

    opGroup = Spec.GAPLoadEncryptionInformationIndicationFrame.opGroup
    opCode = Spec.GAPLoadEncryptionInformationIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPLoadEncryptionInformationIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPLoadEncryptionInformationIndication()
        frame.LtkSize = packet.getParamValueAsNumber("LtkSize")
        frame.Ltk = packet.getParamValueAsList("Ltk")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPLoadCustomPeerInformationIndicationObserver(Observer):

    opGroup = Spec.GAPLoadCustomPeerInformationIndicationFrame.opGroup
    opCode = Spec.GAPLoadCustomPeerInformationIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPLoadCustomPeerInformationIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPLoadCustomPeerInformationIndication()
        frame.InfoSize = packet.getParamValueAsNumber("InfoSize")
        frame.Info = packet.getParamValueAsList("Info")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPCheckIfBondedIndicationObserver(Observer):

    opGroup = Spec.GAPCheckIfBondedIndicationFrame.opGroup
    opCode = Spec.GAPCheckIfBondedIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPCheckIfBondedIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPCheckIfBondedIndication()
        frame.IsBonded = packet.getParamValueAsNumber("IsBonded")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPGetBondedDevicesCountIndicationObserver(Observer):

    opGroup = Spec.GAPGetBondedDevicesCountIndicationFrame.opGroup
    opCode = Spec.GAPGetBondedDevicesCountIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPGetBondedDevicesCountIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPGetBondedDevicesCountIndication()
        frame.NbOfBondedDevices = packet.getParamValueAsNumber("NbOfBondedDevices")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPGetBondedDeviceNameIndicationObserver(Observer):

    opGroup = Spec.GAPGetBondedDeviceNameIndicationFrame.opGroup
    opCode = Spec.GAPGetBondedDeviceNameIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPGetBondedDeviceNameIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPGetBondedDeviceNameIndication()
        frame.NameSize = packet.getParamValueAsNumber("NameSize")
        frame.Name = packet.getParamValueAsList("Name")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPGenericEventInitializationCompleteIndicationObserver(Observer):

    opGroup = Spec.GAPGenericEventInitializationCompleteIndicationFrame.opGroup
    opCode = Spec.GAPGenericEventInitializationCompleteIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPGenericEventInitializationCompleteIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPGenericEventInitializationCompleteIndication()
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPGenericEventInternalErrorIndicationObserver(Observer):

    opGroup = Spec.GAPGenericEventInternalErrorIndicationFrame.opGroup
    opCode = Spec.GAPGenericEventInternalErrorIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPGenericEventInternalErrorIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPGenericEventInternalErrorIndication()
        frame.InternalError_ErrorCode = packet.getParamValueAsNumber("InternalError_ErrorCode")
        frame.InternalError_ErrorSource = packet.getParamValueAsNumber("InternalError_ErrorSource")
        frame.InternalError_HciCommandOpcode = packet.getParamValueAsNumber("InternalError_HciCommandOpcode")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPGenericEventAdvertisingSetupFailedIndicationObserver(Observer):

    opGroup = Spec.GAPGenericEventAdvertisingSetupFailedIndicationFrame.opGroup
    opCode = Spec.GAPGenericEventAdvertisingSetupFailedIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPGenericEventAdvertisingSetupFailedIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPGenericEventAdvertisingSetupFailedIndication()
        frame.SetupFailReason = GAPGenericEventAdvertisingSetupFailedIndicationSetupFailReason.getEnumString(packet.getParamValueAsNumber("SetupFailReason"))
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPGenericEventAdvertisingParametersSetupCompleteIndicationObserver(Observer):

    opGroup = Spec.GAPGenericEventAdvertisingParametersSetupCompleteIndicationFrame.opGroup
    opCode = Spec.GAPGenericEventAdvertisingParametersSetupCompleteIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPGenericEventAdvertisingParametersSetupCompleteIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPGenericEventAdvertisingParametersSetupCompleteIndication()
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPGenericEventAdvertisingDataSetupCompleteIndicationObserver(Observer):

    opGroup = Spec.GAPGenericEventAdvertisingDataSetupCompleteIndicationFrame.opGroup
    opCode = Spec.GAPGenericEventAdvertisingDataSetupCompleteIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPGenericEventAdvertisingDataSetupCompleteIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPGenericEventAdvertisingDataSetupCompleteIndication()
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPGenericEventWhiteListSizeReadIndicationObserver(Observer):

    opGroup = Spec.GAPGenericEventWhiteListSizeReadIndicationFrame.opGroup
    opCode = Spec.GAPGenericEventWhiteListSizeReadIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPGenericEventWhiteListSizeReadIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPGenericEventWhiteListSizeReadIndication()
        frame.WhiteListSize = packet.getParamValueAsNumber("WhiteListSize")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPGenericEventDeviceAddedToWhiteListIndicationObserver(Observer):

    opGroup = Spec.GAPGenericEventDeviceAddedToWhiteListIndicationFrame.opGroup
    opCode = Spec.GAPGenericEventDeviceAddedToWhiteListIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPGenericEventDeviceAddedToWhiteListIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPGenericEventDeviceAddedToWhiteListIndication()
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPGenericEventDeviceRemovedFromWhiteListIndicationObserver(Observer):

    opGroup = Spec.GAPGenericEventDeviceRemovedFromWhiteListIndicationFrame.opGroup
    opCode = Spec.GAPGenericEventDeviceRemovedFromWhiteListIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPGenericEventDeviceRemovedFromWhiteListIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPGenericEventDeviceRemovedFromWhiteListIndication()
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPGenericEventWhiteListClearedIndicationObserver(Observer):

    opGroup = Spec.GAPGenericEventWhiteListClearedIndicationFrame.opGroup
    opCode = Spec.GAPGenericEventWhiteListClearedIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPGenericEventWhiteListClearedIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPGenericEventWhiteListClearedIndication()
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPGenericEventRandomAddressReadyIndicationObserver(Observer):

    opGroup = Spec.GAPGenericEventRandomAddressReadyIndicationFrame.opGroup
    opCode = Spec.GAPGenericEventRandomAddressReadyIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPGenericEventRandomAddressReadyIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPGenericEventRandomAddressReadyIndication()
        frame.Address = packet.getParamValueAsList("Address")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPGenericEventCreateConnectionCanceledIndicationObserver(Observer):

    opGroup = Spec.GAPGenericEventCreateConnectionCanceledIndicationFrame.opGroup
    opCode = Spec.GAPGenericEventCreateConnectionCanceledIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPGenericEventCreateConnectionCanceledIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPGenericEventCreateConnectionCanceledIndication()
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPGenericEventPublicAddressReadIndicationObserver(Observer):

    opGroup = Spec.GAPGenericEventPublicAddressReadIndicationFrame.opGroup
    opCode = Spec.GAPGenericEventPublicAddressReadIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPGenericEventPublicAddressReadIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPGenericEventPublicAddressReadIndication()
        frame.Address = packet.getParamValueAsList("Address")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPGenericEventAdvTxPowerLevelReadIndicationObserver(Observer):

    opGroup = Spec.GAPGenericEventAdvTxPowerLevelReadIndicationFrame.opGroup
    opCode = Spec.GAPGenericEventAdvTxPowerLevelReadIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPGenericEventAdvTxPowerLevelReadIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPGenericEventAdvTxPowerLevelReadIndication()
        frame.AdvTxPowerLevel = packet.getParamValueAsNumber("AdvTxPowerLevel")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPGenericEventPrivateResolvableAddressVerifiedIndicationObserver(Observer):

    opGroup = Spec.GAPGenericEventPrivateResolvableAddressVerifiedIndicationFrame.opGroup
    opCode = Spec.GAPGenericEventPrivateResolvableAddressVerifiedIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPGenericEventPrivateResolvableAddressVerifiedIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPGenericEventPrivateResolvableAddressVerifiedIndication()
        frame.Verified = packet.getParamValueAsNumber("Verified")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPGenericEventRandomAddressSetIndicationObserver(Observer):

    opGroup = Spec.GAPGenericEventRandomAddressSetIndicationFrame.opGroup
    opCode = Spec.GAPGenericEventRandomAddressSetIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPGenericEventRandomAddressSetIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPGenericEventRandomAddressSetIndication()
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPAdvertisingEventStateChangedIndicationObserver(Observer):

    opGroup = Spec.GAPAdvertisingEventStateChangedIndicationFrame.opGroup
    opCode = Spec.GAPAdvertisingEventStateChangedIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPAdvertisingEventStateChangedIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPAdvertisingEventStateChangedIndication()
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPAdvertisingEventCommandFailedIndicationObserver(Observer):

    opGroup = Spec.GAPAdvertisingEventCommandFailedIndicationFrame.opGroup
    opCode = Spec.GAPAdvertisingEventCommandFailedIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPAdvertisingEventCommandFailedIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPAdvertisingEventCommandFailedIndication()
        frame.FailReason = GAPAdvertisingEventCommandFailedIndicationFailReason.getEnumString(packet.getParamValueAsNumber("FailReason"))
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPScanningEventStateChangedIndicationObserver(Observer):

    opGroup = Spec.GAPScanningEventStateChangedIndicationFrame.opGroup
    opCode = Spec.GAPScanningEventStateChangedIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPScanningEventStateChangedIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPScanningEventStateChangedIndication()
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPScanningEventCommandFailedIndicationObserver(Observer):

    opGroup = Spec.GAPScanningEventCommandFailedIndicationFrame.opGroup
    opCode = Spec.GAPScanningEventCommandFailedIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPScanningEventCommandFailedIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPScanningEventCommandFailedIndication()
        frame.FailReason = GAPScanningEventCommandFailedIndicationFailReason.getEnumString(packet.getParamValueAsNumber("FailReason"))
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPScanningEventDeviceScannedIndicationObserver(Observer):

    opGroup = Spec.GAPScanningEventDeviceScannedIndicationFrame.opGroup
    opCode = Spec.GAPScanningEventDeviceScannedIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPScanningEventDeviceScannedIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPScanningEventDeviceScannedIndication()
        frame.ScannedDevice_AddressType = packet.getParamValueAsNumber("ScannedDevice_AddressType")
        frame.ScannedDevice_Address = packet.getParamValueAsList("ScannedDevice_Address")
        frame.ScannedDevice_Rssi = packet.getParamValueAsNumber("ScannedDevice_Rssi")
        frame.ScannedDevice_DataLength = packet.getParamValueAsNumber("ScannedDevice_DataLength")
        frame.ScannedDevice_Data = packet.getParamValueAsList("ScannedDevice_Data")
        frame.ScannedDevice_AdvEventType = packet.getParamValueAsNumber("ScannedDevice_AdvEventType")
        frame.ScannedDevice_DirectRpaUsed = packet.getParamValueAsNumber("ScannedDevice_DirectRpaUsed")
        frame.ScannedDevice_DirectRpa = packet.getParamValueAsList("ScannedDevice_DirectRpa")
        frame.ScannedDevice_advertisingAddressResolved = packet.getParamValueAsNumber("ScannedDevice_advertisingAddressResolved")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPConnectionEventConnectedIndicationObserver(Observer):

    opGroup = Spec.GAPConnectionEventConnectedIndicationFrame.opGroup
    opCode = Spec.GAPConnectionEventConnectedIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPConnectionEventConnectedIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPConnectionEventConnectedIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.ConnectionParameters_ConnInterval = packet.getParamValueAsNumber("ConnectionParameters_ConnInterval")
        frame.ConnectionParameters_ConnLatency = packet.getParamValueAsNumber("ConnectionParameters_ConnLatency")
        frame.ConnectionParameters_SupervisionTimeout = packet.getParamValueAsNumber("ConnectionParameters_SupervisionTimeout")
        frame.ConnectionParameters_MasterClockAccuracy = packet.getParamValueAsNumber("ConnectionParameters_MasterClockAccuracy")
        frame.PeerAddressType = packet.getParamValueAsNumber("PeerAddressType")
        frame.PeerAddress = packet.getParamValueAsList("PeerAddress")
        frame.peerRpaResolved = packet.getParamValueAsNumber("peerRpaResolved")
        frame.peerRpa = packet.getParamValueAsList("peerRpa")
        frame.localRpaUsed = packet.getParamValueAsNumber("localRpaUsed")
        frame.localRpa = packet.getParamValueAsList("localRpa")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPConnectionEventPairingRequestIndicationObserver(Observer):

    opGroup = Spec.GAPConnectionEventPairingRequestIndicationFrame.opGroup
    opCode = Spec.GAPConnectionEventPairingRequestIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPConnectionEventPairingRequestIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPConnectionEventPairingRequestIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.PairingParameters_WithBonding = packet.getParamValueAsNumber("PairingParameters_WithBonding")
        frame.PairingParameters_SecurityModeAndLevel = packet.getParamValueAsNumber("PairingParameters_SecurityModeAndLevel")
        frame.PairingParameters_MaxEncryptionKeySize = packet.getParamValueAsNumber("PairingParameters_MaxEncryptionKeySize")
        frame.PairingParameters_LocalIoCapabilities = packet.getParamValueAsNumber("PairingParameters_LocalIoCapabilities")
        frame.PairingParameters_OobAvailable = packet.getParamValueAsNumber("PairingParameters_OobAvailable")
        frame.PairingParameters_CentralKeys = packet.getParamValueAsNumber("PairingParameters_CentralKeys")
        frame.PairingParameters_PeripheralKeys = packet.getParamValueAsNumber("PairingParameters_PeripheralKeys")
        frame.PairingParameters_LeSecureConnectionSupported = packet.getParamValueAsNumber("PairingParameters_LeSecureConnectionSupported")
        frame.PairingParameters_UseKeypressNotifications = packet.getParamValueAsNumber("PairingParameters_UseKeypressNotifications")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPConnectionEventSlaveSecurityRequestIndicationObserver(Observer):

    opGroup = Spec.GAPConnectionEventSlaveSecurityRequestIndicationFrame.opGroup
    opCode = Spec.GAPConnectionEventSlaveSecurityRequestIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPConnectionEventSlaveSecurityRequestIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPConnectionEventSlaveSecurityRequestIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.SlaveSecurityRequestParameters_BondAfterPairing = packet.getParamValueAsNumber("SlaveSecurityRequestParameters_BondAfterPairing")
        frame.SlaveSecurityRequestParameters_bAuthenticationRequired = packet.getParamValueAsNumber("SlaveSecurityRequestParameters_bAuthenticationRequired")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPConnectionEventPairingResponseIndicationObserver(Observer):

    opGroup = Spec.GAPConnectionEventPairingResponseIndicationFrame.opGroup
    opCode = Spec.GAPConnectionEventPairingResponseIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPConnectionEventPairingResponseIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPConnectionEventPairingResponseIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.PairingParameters_WithBonding = packet.getParamValueAsNumber("PairingParameters_WithBonding")
        frame.PairingParameters_SecurityModeAndLevel = packet.getParamValueAsNumber("PairingParameters_SecurityModeAndLevel")
        frame.PairingParameters_MaxEncryptionKeySize = packet.getParamValueAsNumber("PairingParameters_MaxEncryptionKeySize")
        frame.PairingParameters_LocalIoCapabilities = packet.getParamValueAsNumber("PairingParameters_LocalIoCapabilities")
        frame.PairingParameters_OobAvailable = packet.getParamValueAsNumber("PairingParameters_OobAvailable")
        frame.PairingParameters_CentralKeys = packet.getParamValueAsNumber("PairingParameters_CentralKeys")
        frame.PairingParameters_PeripheralKeys = packet.getParamValueAsNumber("PairingParameters_PeripheralKeys")
        frame.PairingParameters_LeSecureConnectionSupported = packet.getParamValueAsNumber("PairingParameters_LeSecureConnectionSupported")
        frame.PairingParameters_UseKeypressNotifications = packet.getParamValueAsNumber("PairingParameters_UseKeypressNotifications")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPConnectionEventAuthenticationRejectedIndicationObserver(Observer):

    opGroup = Spec.GAPConnectionEventAuthenticationRejectedIndicationFrame.opGroup
    opCode = Spec.GAPConnectionEventAuthenticationRejectedIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPConnectionEventAuthenticationRejectedIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPConnectionEventAuthenticationRejectedIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.RejectReason = GAPConnectionEventAuthenticationRejectedIndicationRejectReason.getEnumString(packet.getParamValueAsNumber("RejectReason"))
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPConnectionEventPasskeyRequestIndicationObserver(Observer):

    opGroup = Spec.GAPConnectionEventPasskeyRequestIndicationFrame.opGroup
    opCode = Spec.GAPConnectionEventPasskeyRequestIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPConnectionEventPasskeyRequestIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPConnectionEventPasskeyRequestIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPConnectionEventOobRequestIndicationObserver(Observer):

    opGroup = Spec.GAPConnectionEventOobRequestIndicationFrame.opGroup
    opCode = Spec.GAPConnectionEventOobRequestIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPConnectionEventOobRequestIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPConnectionEventOobRequestIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPConnectionEventPasskeyDisplayIndicationObserver(Observer):

    opGroup = Spec.GAPConnectionEventPasskeyDisplayIndicationFrame.opGroup
    opCode = Spec.GAPConnectionEventPasskeyDisplayIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPConnectionEventPasskeyDisplayIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPConnectionEventPasskeyDisplayIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.PasskeyForDisplay = packet.getParamValueAsNumber("PasskeyForDisplay")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPConnectionEventKeyExchangeRequestIndicationObserver(Observer):

    opGroup = Spec.GAPConnectionEventKeyExchangeRequestIndicationFrame.opGroup
    opCode = Spec.GAPConnectionEventKeyExchangeRequestIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPConnectionEventKeyExchangeRequestIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPConnectionEventKeyExchangeRequestIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.RequestedKeys = packet.getParamValueAsNumber("RequestedKeys")
        frame.RequestedLtkSize = packet.getParamValueAsNumber("RequestedLtkSize")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPConnectionEventKeysReceivedIndicationObserver(Observer):

    opGroup = Spec.GAPConnectionEventKeysReceivedIndicationFrame.opGroup
    opCode = Spec.GAPConnectionEventKeysReceivedIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPConnectionEventKeysReceivedIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPConnectionEventKeysReceivedIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPConnectionEventLongTermKeyRequestIndicationObserver(Observer):

    opGroup = Spec.GAPConnectionEventLongTermKeyRequestIndicationFrame.opGroup
    opCode = Spec.GAPConnectionEventLongTermKeyRequestIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPConnectionEventLongTermKeyRequestIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPConnectionEventLongTermKeyRequestIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.Ediv = packet.getParamValueAsNumber("Ediv")
        frame.RandSize = packet.getParamValueAsNumber("RandSize")
        frame.Rand = packet.getParamValueAsList("Rand")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPConnectionEventEncryptionChangedIndicationObserver(Observer):

    opGroup = Spec.GAPConnectionEventEncryptionChangedIndicationFrame.opGroup
    opCode = Spec.GAPConnectionEventEncryptionChangedIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPConnectionEventEncryptionChangedIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPConnectionEventEncryptionChangedIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.NewEncryptionState = packet.getParamValueAsNumber("NewEncryptionState")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPConnectionEventPairingCompleteIndicationObserver(Observer):

    opGroup = Spec.GAPConnectionEventPairingCompleteIndicationFrame.opGroup
    opCode = Spec.GAPConnectionEventPairingCompleteIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPConnectionEventPairingCompleteIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPConnectionEventPairingCompleteIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.PairingStatus = GAPConnectionEventPairingCompleteIndicationPairingStatus.getEnumString(packet.getParamValueAsNumber("PairingStatus"))
        frame.PairingData = packet.getParamValueAsNumber("PairingData")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPConnectionEventDisconnectedIndicationObserver(Observer):

    opGroup = Spec.GAPConnectionEventDisconnectedIndicationFrame.opGroup
    opCode = Spec.GAPConnectionEventDisconnectedIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPConnectionEventDisconnectedIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPConnectionEventDisconnectedIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.Reason = GAPConnectionEventDisconnectedIndicationReason.getEnumString(packet.getParamValueAsNumber("Reason"))
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPConnectionEventRssiReadIndicationObserver(Observer):

    opGroup = Spec.GAPConnectionEventRssiReadIndicationFrame.opGroup
    opCode = Spec.GAPConnectionEventRssiReadIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPConnectionEventRssiReadIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPConnectionEventRssiReadIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.Rssi = packet.getParamValueAsNumber("Rssi")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPConnectionEventTxPowerLevelReadIndicationObserver(Observer):

    opGroup = Spec.GAPConnectionEventTxPowerLevelReadIndicationFrame.opGroup
    opCode = Spec.GAPConnectionEventTxPowerLevelReadIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPConnectionEventTxPowerLevelReadIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPConnectionEventTxPowerLevelReadIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.TxPowerLevel = packet.getParamValueAsNumber("TxPowerLevel")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPConnectionEventPowerReadFailureIndicationObserver(Observer):

    opGroup = Spec.GAPConnectionEventPowerReadFailureIndicationFrame.opGroup
    opCode = Spec.GAPConnectionEventPowerReadFailureIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPConnectionEventPowerReadFailureIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPConnectionEventPowerReadFailureIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.FailReason = GAPConnectionEventPowerReadFailureIndicationFailReason.getEnumString(packet.getParamValueAsNumber("FailReason"))
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPConnectionEventParameterUpdateRequestIndicationObserver(Observer):

    opGroup = Spec.GAPConnectionEventParameterUpdateRequestIndicationFrame.opGroup
    opCode = Spec.GAPConnectionEventParameterUpdateRequestIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPConnectionEventParameterUpdateRequestIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPConnectionEventParameterUpdateRequestIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.ConnectionParameterUpdateRequest_IntervalMin = packet.getParamValueAsNumber("ConnectionParameterUpdateRequest_IntervalMin")
        frame.ConnectionParameterUpdateRequest_IntervalMax = packet.getParamValueAsNumber("ConnectionParameterUpdateRequest_IntervalMax")
        frame.ConnectionParameterUpdateRequest_SlaveLatency = packet.getParamValueAsNumber("ConnectionParameterUpdateRequest_SlaveLatency")
        frame.ConnectionParameterUpdateRequest_TimeoutMultiplier = packet.getParamValueAsNumber("ConnectionParameterUpdateRequest_TimeoutMultiplier")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPConnectionEventParameterUpdateCompleteIndicationObserver(Observer):

    opGroup = Spec.GAPConnectionEventParameterUpdateCompleteIndicationFrame.opGroup
    opCode = Spec.GAPConnectionEventParameterUpdateCompleteIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPConnectionEventParameterUpdateCompleteIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPConnectionEventParameterUpdateCompleteIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.ConnectionParameterUpdateComplete_Status = GAPConnectionEventParameterUpdateCompleteIndicationConnectionParameterUpdateComplete_Status.getEnumString(packet.getParamValueAsNumber("ConnectionParameterUpdateComplete_Status"))
        frame.ConnectionParameterUpdateComplete_ConnInterval = packet.getParamValueAsNumber("ConnectionParameterUpdateComplete_ConnInterval")
        frame.ConnectionParameterUpdateComplete_ConnLatency = packet.getParamValueAsNumber("ConnectionParameterUpdateComplete_ConnLatency")
        frame.ConnectionParameterUpdateComplete_SupervisionTimeout = packet.getParamValueAsNumber("ConnectionParameterUpdateComplete_SupervisionTimeout")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPConnectionEventLeDataLengthChangedIndicationObserver(Observer):

    opGroup = Spec.GAPConnectionEventLeDataLengthChangedIndicationFrame.opGroup
    opCode = Spec.GAPConnectionEventLeDataLengthChangedIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPConnectionEventLeDataLengthChangedIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPConnectionEventLeDataLengthChangedIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.LeDataLengthChanged_MaxTxOctets = packet.getParamValueAsNumber("LeDataLengthChanged_MaxTxOctets")
        frame.LeDataLengthChanged_MaxTxTime = packet.getParamValueAsNumber("LeDataLengthChanged_MaxTxTime")
        frame.LeDataLengthChanged_MaxRxOctets = packet.getParamValueAsNumber("LeDataLengthChanged_MaxRxOctets")
        frame.LeDataLengthChanged_MaxRxTime = packet.getParamValueAsNumber("LeDataLengthChanged_MaxRxTime")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPConnectionEventLeScOobDataRequestIndicationObserver(Observer):

    opGroup = Spec.GAPConnectionEventLeScOobDataRequestIndicationFrame.opGroup
    opCode = Spec.GAPConnectionEventLeScOobDataRequestIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPConnectionEventLeScOobDataRequestIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPConnectionEventLeScOobDataRequestIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPConnectionEventLeScDisplayNumericValueIndicationObserver(Observer):

    opGroup = Spec.GAPConnectionEventLeScDisplayNumericValueIndicationFrame.opGroup
    opCode = Spec.GAPConnectionEventLeScDisplayNumericValueIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPConnectionEventLeScDisplayNumericValueIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPConnectionEventLeScDisplayNumericValueIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.ncValue = packet.getParamValueAsNumber("ncValue")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPConnectionEventLeScKeypressNotificationIndicationObserver(Observer):

    opGroup = Spec.GAPConnectionEventLeScKeypressNotificationIndicationFrame.opGroup
    opCode = Spec.GAPConnectionEventLeScKeypressNotificationIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPConnectionEventLeScKeypressNotificationIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPConnectionEventLeScKeypressNotificationIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.GapLeScKeypressNotificationParams_keypressNotifType = packet.getParamValueAsNumber("GapLeScKeypressNotificationParams_keypressNotifType")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPGenericEventControllerResetCompleteIndicationObserver(Observer):

    opGroup = Spec.GAPGenericEventControllerResetCompleteIndicationFrame.opGroup
    opCode = Spec.GAPGenericEventControllerResetCompleteIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPGenericEventControllerResetCompleteIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPGenericEventControllerResetCompleteIndication()
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPLeScPublicKeyRegeneratedIndicationObserver(Observer):

    opGroup = Spec.GAPLeScPublicKeyRegeneratedIndicationFrame.opGroup
    opCode = Spec.GAPLeScPublicKeyRegeneratedIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPLeScPublicKeyRegeneratedIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPLeScPublicKeyRegeneratedIndication()
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPGenericEventLeScLocalOobDataIndicationObserver(Observer):

    opGroup = Spec.GAPGenericEventLeScLocalOobDataIndicationFrame.opGroup
    opCode = Spec.GAPGenericEventLeScLocalOobDataIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPGenericEventLeScLocalOobDataIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPGenericEventLeScLocalOobDataIndication()
        frame.RandomValue = packet.getParamValueAsList("RandomValue")
        frame.ConfirmValue = packet.getParamValueAsList("ConfirmValue")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPGenericEventControllerPrivacyStateChangedIndicationObserver(Observer):

    opGroup = Spec.GAPGenericEventControllerPrivacyStateChangedIndicationFrame.opGroup
    opCode = Spec.GAPGenericEventControllerPrivacyStateChangedIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPGenericEventControllerPrivacyStateChangedIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPGenericEventControllerPrivacyStateChangedIndication()
        frame.NewControllerPrivacyState = packet.getParamValueAsNumber("NewControllerPrivacyState")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class GAPGetBondedDevicesIdentityInformationIndicationObserver(Observer):

    opGroup = Spec.GAPGetBondedDevicesIdentityInformationIndicationFrame.opGroup
    opCode = Spec.GAPGetBondedDevicesIdentityInformationIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.GAPGetBondedDevicesIdentityInformationIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = GAPGetBondedDevicesIdentityInformationIndication()
        curr = 0
        frame.NbOfDeviceIdentityAddresses = data.contents[curr]
        curr += 1
        frame.IdentityAddresses = []
        for _ in range(frame.NbOfDeviceIdentityAddresses):
            # Create sub-object container
            IdentityAddresses = GAPGetBondedDevicesIdentityInformationIndication.IdentityAddresses()
            IdentityAddresses.IdentityAddressType = data.contents[curr]
            curr += 1
            IdentityAddresses.IdentityAddress = list_to_int(data.contents[curr:curr + 6])
            curr += 6
            IdentityAddresses.Irk = list_to_int(data.contents[curr:curr + 16])
            curr += 16
            # Add sub-object to list container.
            frame.IdentityAddresses.append(copy.deepcopy(IdentityAddresses))

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
