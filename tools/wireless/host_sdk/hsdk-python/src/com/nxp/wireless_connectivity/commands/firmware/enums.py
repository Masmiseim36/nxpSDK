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

class GenericEnum(object):

    @classmethod
    def getEnumString(cls, value):
        for k, v in cls.__dict__.items():
            if v == value:
                return k
        return value


class FSCIFirmware_SetOperationModeRequestOperationMode(GenericEnum):

    Reserved = 0x00
    Standalone = 0x01
    Dongle = 0x02


class FSCIFirmware_SetOperationModeRequestTransferType(GenericEnum):
    Unicast = 0x00
    Multicast = 0x01


class FSCIFirmware_QueryImageRspRequestOtaImageStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01
    Abort = 0x95
    NotAuthorized = 0x7E
    InvalidImage = 0x96
    WaitForData = 0x97
    NoImageAvailable = 0x98


class FSCIFirmware_StartImageConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01
    InvalidParameter = 0x05
    InvalidOperation = 0x06


class FSCIFirmware_StartImageConfirmProtocolVersion(GenericEnum):

    # ZigbeeCodebase_3.10-3.11_Version
    ZigbeeCodebase_310311_Version = 0x00
    NewVersion = 0x01


class FSCIFirmware_SetOperationModeConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01
    # Invalid Parameter
    InvalidParameter = 0x05
    NoMemory = 0x06


class FSCIFirmware_QueryImageRspConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01
    # Invalid Parameter
    InvalidParameter = 0x05


class FSCIFirmware_PushImageChunkConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01
    UnexpectedSeqNo = 0x03
    InvalidParameter = 0x05
    InvalidOperation = 0x06
    FlashError = 0x07
    MCUFlashError = 0x08


class FSCIFirmware_CancelProcessConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01


class FSCIFirmware_CommitImageConfirmStatus(GenericEnum):

    Success = 0x00
    InvalidParam = 0x05
    InvalidOperation = 0x06
    ExternalFlashError = 0x07
    MCUFlashError = 0x08
    CRCCheckError = 0x04


class FSCIFirmware_ImageNotifyConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01


class FSCIFirmware_SetFileVersionPolicyConfirmStatus(GenericEnum):

    Success = 0x00
    Failed = 0x01
