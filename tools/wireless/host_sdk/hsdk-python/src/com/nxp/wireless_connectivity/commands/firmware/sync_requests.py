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

from com.nxp.wireless_connectivity.commands.fsci_frame_description import Protocol, FsciAckPolicy
from com.nxp.wireless_connectivity.commands.firmware.enums import *  # @UnusedWildImport
import com.nxp.wireless_connectivity.commands.firmware.frames as Frames
from com.nxp.wireless_connectivity.commands.firmware.operations import *  # @UnusedWildImport


def FSCIEnterBootloader(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Firmware,
    timeout=1
):
    request = Frames.FSCIEnterBootloaderRequest()
    return FSCIEnterBootloaderOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def FSCICPUReset(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Firmware,
    timeout=1
):
    request = Frames.FSCICPUResetRequest()
    return FSCICPUResetOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def FSCIFirmware_StartImage(
    device,
    ImageSize=bytearray(4),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Firmware,
    timeout=3
):
    request = Frames.FSCIFirmware_StartImageRequest(ImageSize)
    return FSCIFirmware_StartImageOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def FSCIFirmware_SetOperationMode(
    device,
    OperationMode=FSCIFirmware_SetOperationModeRequestOperationMode.Reserved,
    TransferType=FSCIFirmware_SetOperationModeRequestTransferType.Unicast,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Firmware,
    timeout=3
):
    request = Frames.FSCIFirmware_SetOperationModeRequest(OperationMode)
    return FSCIFirmware_SetOperationModeOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def FSCIFirmware_QueryImageRsp(
    device,
    OtaImageStatus=FSCIFirmware_QueryImageRspRequestOtaImageStatus.Success,
    DeviceId=bytearray(2),
    ManufacturerCode=bytearray(2),
    ImageType=bytearray(2),
    FileVersion=bytearray(4),
    ImageSize=bytearray(4),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Firmware,
    timeout=3
):
    request = Frames.FSCIFirmware_QueryImageRspRequest(OtaImageStatus, DeviceId, ManufacturerCode, ImageType, FileVersion, ImageSize)
    return FSCIFirmware_QueryImageRspOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def FSCIFirmware_PushImageChunk(
    device,
    DataImageBlock=bytearray(),
    SeqNo=bytearray(1),
    FSCIBootloaderMode=False,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Firmware,
    timeout=3
):
    request = Frames.FSCIFirmware_PushImageChunkRequest(DataImageBlock, SeqNo, FSCIBootloaderMode)
    return FSCIFirmware_PushImageChunkOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def FSCIFirmware_CancelProcess(
    device,
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Firmware,
    timeout=3
):
    request = Frames.FSCIFirmware_CancelProcessRequest()
    return FSCIFirmware_CancelProcessOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def FSCIFirmware_CommitImage(
    device,
    BitMask=bytearray(32),
    CRCEnabled=True,
    CRCval=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Firmware,
    timeout=3
):
    request = Frames.FSCIFirmware_CommitImageRequest(BitMask, CRCEnabled, CRCval)
    return FSCIFirmware_CommitImageOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def FSCIFirmware_ImageNotify(
    device,
    DeviceId=bytearray(2),
    ManufacturerCode=bytearray(2),
    ImageType=bytearray(2),
    ImageSize=bytearray(4),
    FileSize=bytearray(4),
    FileVersion=bytearray(4),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Firmware,
    timeout=3
):
    request = Frames.FSCIFirmware_ImageNotifyRequest(DeviceId, ManufacturerCode, ImageType, ImageSize, FileSize, FileVersion)
    return FSCIFirmware_ImageNotifyOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def FSCIFirmware_SetFileVersionPolicy(
    device,
    FileVersionPolicy=bytearray(1),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Firmware,
    timeout=3
):
    request = Frames.FSCIFirmware_SetFileVersionPolicyRequest(FileVersionPolicy)
    return FSCIFirmware_SetFileVersionPolicyOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)


def FSCIFirmware_Abort(
    device,
    DeviceId=bytearray(2),
    ack_policy=FsciAckPolicy.GLOBAL,
    protocol=Protocol.Firmware,
    timeout=3
):
    request = Frames.FSCIFirmware_AbortRequest(DeviceId)
    return FSCIFirmware_AbortOperation(device, request, ack_policy=ack_policy, protocol=protocol, sync_request=True).begin(timeout)
