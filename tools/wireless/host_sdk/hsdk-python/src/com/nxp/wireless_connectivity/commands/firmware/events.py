'''
* The Clear BSD License
* Copyright 2016-2017 NXP
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

from ctypes import cast, c_uint8, c_void_p, POINTER
import inspect
import sys

from com.nxp.wireless_connectivity.commands.firmware.frames import *  # @UnusedWildImport
from com.nxp.wireless_connectivity.commands.firmware.spec import Spec as _Spec
from com.nxp.wireless_connectivity.hsdk.CFsciLibrary import FsciFrame
from com.nxp.wireless_connectivity.hsdk.library_loader import LibraryLoader
from com.nxp.wireless_connectivity.hsdk.utils import Observer, overrides, print_event


fsciLibrary = LibraryLoader().CFsciLibrary
fsciLibrary.DestroyFSCIFrame.argtypes = [c_void_p]
Spec = _Spec()


class FSCIFirmware_QueryImageIndicationObserver(Observer):

    opGroup = Spec.FSCIFirmware_QueryImageIndicationFrame.opGroup
    opCode = Spec.FSCIFirmware_QueryImageIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.FSCIFirmware_QueryImageIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = FSCIFirmware_QueryImageIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.ManufacturerCode = packet.getParamValueAsNumber("ManufacturerCode")
        frame.ImageType = packet.getParamValueAsNumber("ImageType")
        frame.FileVersion = packet.getParamValueAsNumber("FileVersion")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class FSCIFirmware_ChunkIndicationObserver(Observer):

    opGroup = Spec.FSCIFirmware_ChunkIndicationFrame.opGroup
    opCode = Spec.FSCIFirmware_ChunkIndicationFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.FSCIFirmware_ChunkIndicationFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = FSCIFirmware_ChunkIndication()
        frame.DeviceId = packet.getParamValueAsNumber("DeviceId")
        frame.ImageOffset = packet.getParamValueAsNumber("ImageOffset")
        frame.BlockLength = packet.getParamValueAsNumber("BlockLength")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class FSCIFirmware_StartImageConfirmObserver(Observer):

    opGroup = Spec.FSCIFirmware_StartImageConfirmFrame.opGroup
    opCode = Spec.FSCIFirmware_StartImageConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.FSCIFirmware_StartImageConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = FSCIFirmware_StartImageConfirm()
        frame.Status = FSCIFirmware_StartImageConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        frame.ProtocolVersion = packet.getParamValueAsNumber("ProtocolVersion")
        frame.ExternalMemorySupported = packet.getParamValueAsNumber("ExternalMemorySupported")
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class FSCIFirmware_SetOperationModeConfirmObserver(Observer):

    opGroup = Spec.FSCIFirmware_SetOperationModeConfirmFrame.opGroup
    opCode = Spec.FSCIFirmware_SetOperationModeConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.FSCIFirmware_SetOperationModeConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = FSCIFirmware_SetOperationModeConfirm()
        frame.Status = FSCIFirmware_SetOperationModeConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class FSCIFirmware_QueryImageRspConfirmObserver(Observer):

    opGroup = Spec.FSCIFirmware_QueryImageRspConfirmFrame.opGroup
    opCode = Spec.FSCIFirmware_QueryImageRspConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.FSCIFirmware_QueryImageRspConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = FSCIFirmware_QueryImageRspConfirm()
        frame.Status = FSCIFirmware_QueryImageRspConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class FSCIFirmware_PushImageChunkConfirmObserver(Observer):

    opGroup = Spec.FSCIFirmware_PushImageChunkConfirmFrame.opGroup
    opCode = Spec.FSCIFirmware_PushImageChunkConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.FSCIFirmware_PushImageChunkConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = FSCIFirmware_PushImageChunkConfirm()
        frame.Status = FSCIFirmware_PushImageChunkConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class FSCIFirmware_CancelProcessConfirmObserver(Observer):

    opGroup = Spec.FSCIFirmware_CancelProcessConfirmFrame.opGroup
    opCode = Spec.FSCIFirmware_CancelProcessConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.FSCIFirmware_CancelProcessConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = FSCIFirmware_CancelProcessConfirm()
        frame.Status = FSCIFirmware_CancelProcessConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class FSCIFirmware_CommitImageConfirmObserver(Observer):

    opGroup = Spec.FSCIFirmware_CommitImageConfirmFrame.opGroup
    opCode = Spec.FSCIFirmware_CommitImageConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.FSCIFirmware_CommitImageConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = FSCIFirmware_CommitImageConfirm()
        frame.Status = FSCIFirmware_CommitImageConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class FSCIFirmware_ImageNotifyConfirmObserver(Observer):

    opGroup = Spec.FSCIFirmware_ImageNotifyConfirmFrame.opGroup
    opCode = Spec.FSCIFirmware_ImageNotifyConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.FSCIFirmware_ImageNotifyConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = FSCIFirmware_ImageNotifyConfirm()
        frame.Status = FSCIFirmware_ImageNotifyConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
        framer.event_queue.put(frame) if sync_request else None

        if callback is not None:
            callback(self.deviceName, frame)
        else:
            print_event(self.deviceName, frame)
        fsciLibrary.DestroyFSCIFrame(event)


class FSCIFirmware_SetFileVersionPolicyConfirmObserver(Observer):

    opGroup = Spec.FSCIFirmware_SetFileVersionPolicyConfirmFrame.opGroup
    opCode = Spec.FSCIFirmware_SetFileVersionPolicyConfirmFrame.opCode

    @overrides(Observer)
    def observeEvent(self, framer, event, callback, sync_request):
        # Call super, print common information
        Observer.observeEvent(self, framer, event, callback, sync_request)
        # Get payload
        fsciFrame = cast(event, POINTER(FsciFrame))
        data = cast(fsciFrame.contents.data, POINTER(fsciFrame.contents.length * c_uint8))
        packet = Spec.FSCIFirmware_SetFileVersionPolicyConfirmFrame.getFsciPacketFromByteArray(data.contents, fsciFrame.contents.length)
        # Create frame object
        frame = FSCIFirmware_SetFileVersionPolicyConfirm()
        frame.Status = FSCIFirmware_SetFileVersionPolicyConfirmStatus.getEnumString(packet.getParamValueAsNumber("Status"))
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
