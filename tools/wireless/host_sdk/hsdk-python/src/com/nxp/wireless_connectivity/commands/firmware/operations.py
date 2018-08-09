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

from com.nxp.wireless_connectivity.commands.firmware.events import *  # @UnusedWildImport
from com.nxp.wireless_connectivity.commands.fsci_operation import FsciOperation


class FSCIEnterBootloaderOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.FSCIEnterBootloaderRequestFrame
        self.observers = []
        super(FSCIEnterBootloaderOperation, self).subscribeToEvents()


class FSCICPUResetOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.FSCICPUResetRequestFrame
        self.observers = []
        super(FSCICPUResetOperation, self).subscribeToEvents()


class FSCIFirmware_StartImageOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.FSCIFirmware_StartImageRequestFrame
        self.observers = [FSCIFirmware_StartImageConfirmObserver('FSCIFirmware_StartImageConfirm'), ]
        super(FSCIFirmware_StartImageOperation, self).subscribeToEvents()


class FSCIFirmware_SetOperationModeOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.FSCIFirmware_SetOperationModeRequestFrame
        self.observers = [FSCIFirmware_SetOperationModeConfirmObserver('FSCIFirmware_SetOperationModeConfirm'), ]
        super(FSCIFirmware_SetOperationModeOperation, self).subscribeToEvents()


class FSCIFirmware_QueryImageRspOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.FSCIFirmware_QueryImageRspRequestFrame
        self.observers = [FSCIFirmware_QueryImageRspConfirmObserver('FSCIFirmware_QueryImageRspConfirm'), ]
        super(FSCIFirmware_QueryImageRspOperation, self).subscribeToEvents()


class FSCIFirmware_PushImageChunkOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.FSCIFirmware_PushImageChunkRequestFrame
        self.observers = [FSCIFirmware_PushImageChunkConfirmObserver('FSCIFirmware_PushImageChunkConfirm'), ]
        super(FSCIFirmware_PushImageChunkOperation, self).subscribeToEvents()


class FSCIFirmware_CancelProcessOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.FSCIFirmware_CancelProcessRequestFrame
        self.observers = [FSCIFirmware_CancelProcessConfirmObserver('FSCIFirmware_CancelProcessConfirm'), ]
        super(FSCIFirmware_CancelProcessOperation, self).subscribeToEvents()


class FSCIFirmware_CommitImageOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.FSCIFirmware_CommitImageRequestFrame
        self.observers = [FSCIFirmware_CommitImageConfirmObserver('FSCIFirmware_CommitImageConfirm'), ]
        super(FSCIFirmware_CommitImageOperation, self).subscribeToEvents()


class FSCIFirmware_ImageNotifyOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.FSCIFirmware_ImageNotifyRequestFrame
        self.observers = [FSCIFirmware_ImageNotifyConfirmObserver('FSCIFirmware_ImageNotifyConfirm'), ]
        super(FSCIFirmware_ImageNotifyOperation, self).subscribeToEvents()


class FSCIFirmware_SetFileVersionPolicyOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.FSCIFirmware_SetFileVersionPolicyRequestFrame
        self.observers = [FSCIFirmware_SetFileVersionPolicyConfirmObserver('FSCIFirmware_SetFileVersionPolicyConfirm'), ]
        super(FSCIFirmware_SetFileVersionPolicyOperation, self).subscribeToEvents()


class FSCIFirmware_AbortOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.FSCIFirmware_AbortRequestFrame
        self.observers = []
        super(FSCIFirmware_AbortOperation, self).subscribeToEvents()


class FSCIFirmware_QueryImageOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [FSCIFirmware_QueryImageIndicationObserver('FSCIFirmware_QueryImageIndication'), ]
        super(FSCIFirmware_QueryImageOperation, self).subscribeToEvents()


class FSCIFirmware_ChunkOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [FSCIFirmware_ChunkIndicationObserver('FSCIFirmware_ChunkIndication'), ]
        super(FSCIFirmware_ChunkOperation, self).subscribeToEvents()
