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

from com.nxp.wireless_connectivity.commands.fsci_frame_description import FsciAckPolicy,\
    Protocol
from com.nxp.wireless_connectivity.commands.fsci_operation import FsciOperation
from com.nxp.wireless_connectivity.hsdk.CUartLibrary import Baudrate
from com.nxp.wireless_connectivity.hsdk.framing.fsci_framer import FsciFramer

from com.nxp.wireless_connectivity.commands.ble.events import *  # @UnusedWildImport


class FSCICPUResetOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.FSCICPUResetRequestFrame
        self.observers = []
        super(FSCICPUResetOperation, self).subscribeToEvents()


class HCIModeSelectOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.HCIModeSelectRequestFrame
        self.observers = [HCIConfirmObserver('HCIConfirm'), ]
        super(HCIModeSelectOperation, self).subscribeToEvents()

class HCICommandOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.HCICommandRequestFrame
        self.observers = [HCIConfirmObserver('HCIConfirm'), ]
        super(HCICommandOperation, self).subscribeToEvents()

class HCIDataOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.HCIDataRequestFrame
        self.observers = [HCIConfirmObserver('HCIConfirm'), HCIDataIndicationObserver('HCIDataIndication'), ]
        super(HCIDataOperation, self).subscribeToEvents()

class HCISynchronousDataOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.HCISynchronousDataRequestFrame
        self.observers = [HCIConfirmObserver('HCIConfirm'), HCISynchronousDataIndicationObserver('HCISynchronousDataIndication'), ]
        super(HCISynchronousDataOperation, self).subscribeToEvents()

class L2CAPModeSelectOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.L2CAPModeSelectRequestFrame
        self.observers = []
        super(L2CAPModeSelectOperation, self).subscribeToEvents()

class L2CAPInitOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.L2CAPInitRequestFrame
        self.observers = []
        super(L2CAPInitOperation, self).subscribeToEvents()

class L2CAPConfigOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.L2CAPConfigRequestFrame
        self.observers = []
        super(L2CAPConfigOperation, self).subscribeToEvents()

class L2CAPSendAttDataOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.L2CAPSendAttDataRequestFrame
        self.observers = []
        super(L2CAPSendAttDataOperation, self).subscribeToEvents()

class L2CAPSendSmpDataOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.L2CAPSendSmpDataRequestFrame
        self.observers = []
        super(L2CAPSendSmpDataOperation, self).subscribeToEvents()

class L2CAPRegisterAttCallbackOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.L2CAPRegisterAttCallbackRequestFrame
        self.observers = []
        super(L2CAPRegisterAttCallbackOperation, self).subscribeToEvents()

class L2CAPRegisterSmpCallbackOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.L2CAPRegisterSmpCallbackRequestFrame
        self.observers = []
        super(L2CAPRegisterSmpCallbackOperation, self).subscribeToEvents()

class L2CAPRegisterLePsmOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.L2CAPRegisterLePsmRequestFrame
        self.observers = []
        super(L2CAPRegisterLePsmOperation, self).subscribeToEvents()

class L2CAPDeregisterLePsmOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.L2CAPDeregisterLePsmRequestFrame
        self.observers = []
        super(L2CAPDeregisterLePsmOperation, self).subscribeToEvents()

class L2CAPConnectLePsmOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.L2CAPConnectLePsmRequestFrame
        self.observers = []
        super(L2CAPConnectLePsmOperation, self).subscribeToEvents()

class L2CAPDisconnectLeCbChannelOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.L2CAPDisconnectLeCbChannelRequestFrame
        self.observers = []
        super(L2CAPDisconnectLeCbChannelOperation, self).subscribeToEvents()

class L2CAPCancelConnectionOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.L2CAPCancelConnectionRequestFrame
        self.observers = []
        super(L2CAPCancelConnectionOperation, self).subscribeToEvents()

class L2CAPSendLeCbDataOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.L2CAPSendLeCbDataRequestFrame
        self.observers = []
        super(L2CAPSendLeCbDataOperation, self).subscribeToEvents()

class L2CAPSendLeCreditOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.L2CAPSendLeCreditRequestFrame
        self.observers = []
        super(L2CAPSendLeCreditOperation, self).subscribeToEvents()

class SMModeSelectOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SMModeSelectRequestFrame
        self.observers = [SMConfirmObserver('SMConfirm'), ]
        super(SMModeSelectOperation, self).subscribeToEvents()

class SMInitOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SMInitRequestFrame
        self.observers = [SMConfirmObserver('SMConfirm'), ]
        super(SMInitOperation, self).subscribeToEvents()

class SMRegisterUserCallbackOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SMRegisterUserCallbackRequestFrame
        self.observers = [SMConfirmObserver('SMConfirm'), ]
        super(SMRegisterUserCallbackOperation, self).subscribeToEvents()

class SMInitiatorStartPairingOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SMInitiatorStartPairingRequestFrame
        self.observers = [SMConfirmObserver('SMConfirm'), ]
        super(SMInitiatorStartPairingOperation, self).subscribeToEvents()

class SMSendSecurityRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SMSendSecurityRequestRequestFrame
        self.observers = [SMConfirmObserver('SMConfirm'), ]
        super(SMSendSecurityRequestOperation, self).subscribeToEvents()

class SMTerminatePairingOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SMTerminatePairingRequestFrame
        self.observers = [SMConfirmObserver('SMConfirm'), ]
        super(SMTerminatePairingOperation, self).subscribeToEvents()

class SMPairingRequestReplyOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SMPairingRequestReplyRequestFrame
        self.observers = [SMConfirmObserver('SMConfirm'), ]
        super(SMPairingRequestReplyOperation, self).subscribeToEvents()

class SMPasskeyRequestReplyOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SMPasskeyRequestReplyRequestFrame
        self.observers = [SMConfirmObserver('SMConfirm'), ]
        super(SMPasskeyRequestReplyOperation, self).subscribeToEvents()

class SMPairingKeysetRequestReplyOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SMPairingKeysetRequestReplyRequestFrame
        self.observers = [SMConfirmObserver('SMConfirm'), ]
        super(SMPairingKeysetRequestReplyOperation, self).subscribeToEvents()

class SMLlStartEncryptionOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SMLlStartEncryptionRequestFrame
        self.observers = [SMConfirmObserver('SMConfirm'), ]
        super(SMLlStartEncryptionOperation, self).subscribeToEvents()

class SMLlLtkRequestNegativeReplyOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SMLlLtkRequestNegativeReplyRequestFrame
        self.observers = [SMConfirmObserver('SMConfirm'), ]
        super(SMLlLtkRequestNegativeReplyOperation, self).subscribeToEvents()

class SMLlEncryptRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SMLlEncryptRequestRequestFrame
        self.observers = [SMConfirmObserver('SMConfirm'), ]
        super(SMLlEncryptRequestOperation, self).subscribeToEvents()

class SMLlRandRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SMLlRandRequestRequestFrame
        self.observers = [SMConfirmObserver('SMConfirm'), ]
        super(SMLlRandRequestOperation, self).subscribeToEvents()

class SMTbCreateRandomDeviceAddressRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SMTbCreateRandomDeviceAddressRequestRequestFrame
        self.observers = [SMConfirmObserver('SMConfirm'), ]
        super(SMTbCreateRandomDeviceAddressRequestOperation, self).subscribeToEvents()

class SMTbCheckResolvablePrivateAddressRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SMTbCheckResolvablePrivateAddressRequestRequestFrame
        self.observers = [SMConfirmObserver('SMConfirm'), ]
        super(SMTbCheckResolvablePrivateAddressRequestOperation, self).subscribeToEvents()

class SMTbSignDataRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SMTbSignDataRequestRequestFrame
        self.observers = [SMConfirmObserver('SMConfirm'), ]
        super(SMTbSignDataRequestOperation, self).subscribeToEvents()

class SMTbVerifyDataSignatureRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SMTbVerifyDataSignatureRequestRequestFrame
        self.observers = [SMConfirmObserver('SMConfirm'), ]
        super(SMTbVerifyDataSignatureRequestOperation, self).subscribeToEvents()

class SMDeviceConnectNotificationOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SMDeviceConnectNotificationRequestFrame
        self.observers = [SMConfirmObserver('SMConfirm'), ]
        super(SMDeviceConnectNotificationOperation, self).subscribeToEvents()

class SMDeviceDisconnectNotificationOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SMDeviceDisconnectNotificationRequestFrame
        self.observers = [SMConfirmObserver('SMConfirm'), ]
        super(SMDeviceDisconnectNotificationOperation, self).subscribeToEvents()

class SMSetDefaultPasskeyOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SMSetDefaultPasskeyRequestFrame
        self.observers = [SMConfirmObserver('SMConfirm'), ]
        super(SMSetDefaultPasskeyOperation, self).subscribeToEvents()

class SMSetOobMitmProtectionOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SMSetOobMitmProtectionRequestFrame
        self.observers = [SMConfirmObserver('SMConfirm'), ]
        super(SMSetOobMitmProtectionOperation, self).subscribeToEvents()

class SMSendKeypressNotificationOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SMSendKeypressNotificationRequestFrame
        self.observers = [SMConfirmObserver('SMConfirm'), ]
        super(SMSendKeypressNotificationOperation, self).subscribeToEvents()

class SMNcDisplayConfirmOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SMNcDisplayConfirmRequestFrame
        self.observers = [SMConfirmObserver('SMConfirm'), ]
        super(SMNcDisplayConfirmOperation, self).subscribeToEvents()

class SMLeScOobDataRequestReplyOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SMLeScOobDataRequestReplyRequestFrame
        self.observers = [SMConfirmObserver('SMConfirm'), ]
        super(SMLeScOobDataRequestReplyOperation, self).subscribeToEvents()

class SMLocalLeScOobDataOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SMLocalLeScOobDataRequestReqRequestFrame
        self.observers = [SMConfirmObserver('SMConfirm'), SMLocalLeScOobDataIndicationObserver('SMLocalLeScOobDataIndication'), ]
        super(SMLocalLeScOobDataOperation, self).subscribeToEvents()

class SMGenerateNewEcdhPkSkPairOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.SMGenerateNewEcdhPkSkPairRequestFrame
        self.observers = [SMConfirmObserver('SMConfirm'), ]
        super(SMGenerateNewEcdhPkSkPairOperation, self).subscribeToEvents()

class ATTModeSelectOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTModeSelectRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTModeSelectOperation, self).subscribeToEvents()

class ATTInitOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTInitRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTInitOperation, self).subscribeToEvents()

class ATTNotifyConnectionOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTNotifyConnectionRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTNotifyConnectionOperation, self).subscribeToEvents()

class ATTNotifyDisconnectionOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTNotifyDisconnectionRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTNotifyDisconnectionOperation, self).subscribeToEvents()

class ATTSetMtuOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTSetMtuRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTSetMtuOperation, self).subscribeToEvents()

class ATTGetMtuOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTGetMtuRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ATTGetMtuIndicationObserver('ATTGetMtuIndication'), ]
        super(ATTGetMtuOperation, self).subscribeToEvents()

class ATTRegisterOpcodeCallbackOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTRegisterOpcodeCallbackRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTRegisterOpcodeCallbackOperation, self).subscribeToEvents()

class ATTRegisterUnsupportedOpcodeCallbackOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTRegisterUnsupportedOpcodeCallbackRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTRegisterUnsupportedOpcodeCallbackOperation, self).subscribeToEvents()

class ATTRegisterTimeoutCallbackOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTRegisterTimeoutCallbackRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTRegisterTimeoutCallbackOperation, self).subscribeToEvents()

class ATTServerSendErrorResponseOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTServerSendErrorResponseRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTServerSendErrorResponseOperation, self).subscribeToEvents()

class ATTClientSendExchangeMtuRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTClientSendExchangeMtuRequestRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTClientSendExchangeMtuRequestOperation, self).subscribeToEvents()

class ATTServerSendExchangeMtuResponseOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTServerSendExchangeMtuResponseRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTServerSendExchangeMtuResponseOperation, self).subscribeToEvents()

class ATTClientSendFindInformationRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTClientSendFindInformationRequestRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTClientSendFindInformationRequestOperation, self).subscribeToEvents()

class ATTServerSendFindInformationResponseOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTServerSendFindInformationResponseRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTServerSendFindInformationResponseOperation, self).subscribeToEvents()

class ATTClientSendFindByTypeValueRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTClientSendFindByTypeValueRequestRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTClientSendFindByTypeValueRequestOperation, self).subscribeToEvents()

class ATTServerSendFindByTypeValueResponseOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTServerSendFindByTypeValueResponseRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTServerSendFindByTypeValueResponseOperation, self).subscribeToEvents()

class ATTClientSendReadByTypeRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTClientSendReadByTypeRequestRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTClientSendReadByTypeRequestOperation, self).subscribeToEvents()

class ATTServerSendReadByTypeResponseOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTServerSendReadByTypeResponseRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTServerSendReadByTypeResponseOperation, self).subscribeToEvents()

class ATTClientSendReadRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTClientSendReadRequestRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTClientSendReadRequestOperation, self).subscribeToEvents()

class ATTServerSendReadResponseOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTServerSendReadResponseRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTServerSendReadResponseOperation, self).subscribeToEvents()

class ATTClientSendReadBlobRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTClientSendReadBlobRequestRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTClientSendReadBlobRequestOperation, self).subscribeToEvents()

class ATTServerSendReadBlobResponseOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTServerSendReadBlobResponseRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTServerSendReadBlobResponseOperation, self).subscribeToEvents()

class ATTClientSendReadMultipleRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTClientSendReadMultipleRequestRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTClientSendReadMultipleRequestOperation, self).subscribeToEvents()

class ATTServerSendReadMultipleResponseOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTServerSendReadMultipleResponseRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTServerSendReadMultipleResponseOperation, self).subscribeToEvents()

class ATTClientSendReadByGroupTypeRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTClientSendReadByGroupTypeRequestRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTClientSendReadByGroupTypeRequestOperation, self).subscribeToEvents()

class ATTServerSendReadByGroupTypeResponseOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTServerSendReadByGroupTypeResponseRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTServerSendReadByGroupTypeResponseOperation, self).subscribeToEvents()

class ATTClientSendWriteRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTClientSendWriteRequestRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTClientSendWriteRequestOperation, self).subscribeToEvents()

class ATTServerSendWriteResponseOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTServerSendWriteResponseRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTServerSendWriteResponseOperation, self).subscribeToEvents()

class ATTClientSendWriteCommandOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTClientSendWriteCommandRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTClientSendWriteCommandOperation, self).subscribeToEvents()

class ATTClientSendSignedWriteCommandOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTClientSendSignedWriteCommandRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTClientSendSignedWriteCommandOperation, self).subscribeToEvents()

class ATTClientSendPrepareWriteRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTClientSendPrepareWriteRequestRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTClientSendPrepareWriteRequestOperation, self).subscribeToEvents()

class ATTServerSendPrepareWriteResponseOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTServerSendPrepareWriteResponseRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTServerSendPrepareWriteResponseOperation, self).subscribeToEvents()

class ATTClientSendExecuteWriteRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTClientSendExecuteWriteRequestRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTClientSendExecuteWriteRequestOperation, self).subscribeToEvents()

class ATTServerSendExecuteWriteResponseOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTServerSendExecuteWriteResponseRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTServerSendExecuteWriteResponseOperation, self).subscribeToEvents()

class ATTServerSendHandleValueNotificationOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTServerSendHandleValueNotificationRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTServerSendHandleValueNotificationOperation, self).subscribeToEvents()

class ATTServerSendHandleValueIndicationOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTServerSendHandleValueIndicationRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTServerSendHandleValueIndicationOperation, self).subscribeToEvents()

class ATTClientSendHandleValueConfirmationOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.ATTClientSendHandleValueConfirmationRequestFrame
        self.observers = [ATTConfirmObserver('ATTConfirm'), ]
        super(ATTClientSendHandleValueConfirmationOperation, self).subscribeToEvents()

class GATTModeSelectOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTModeSelectRequestFrame
        self.observers = []
        super(GATTModeSelectOperation, self).subscribeToEvents()

class GATTInitOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTInitRequestFrame
        self.observers = []
        super(GATTInitOperation, self).subscribeToEvents()

class GATTGetMtuOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTGetMtuRequestFrame
        self.observers = [GATTGetMtuIndicationObserver('GATTGetMtuIndication'), ]
        super(GATTGetMtuOperation, self).subscribeToEvents()

class GATTClientInitOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTClientInitRequestFrame
        self.observers = []
        super(GATTClientInitOperation, self).subscribeToEvents()

class GATTClientResetProcedureOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTClientResetProcedureRequestFrame
        self.observers = []
        super(GATTClientResetProcedureOperation, self).subscribeToEvents()

class GATTClientRegisterProcedureCallbackOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTClientRegisterProcedureCallbackRequestFrame
        self.observers = []
        super(GATTClientRegisterProcedureCallbackOperation, self).subscribeToEvents()

class GATTClientRegisterNotificationCallbackOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTClientRegisterNotificationCallbackRequestFrame
        self.observers = []
        super(GATTClientRegisterNotificationCallbackOperation, self).subscribeToEvents()

class GATTClientRegisterIndicationCallbackOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTClientRegisterIndicationCallbackRequestFrame
        self.observers = []
        super(GATTClientRegisterIndicationCallbackOperation, self).subscribeToEvents()

class GATTClientExchangeMtuOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTClientExchangeMtuRequestFrame
        self.observers = []
        super(GATTClientExchangeMtuOperation, self).subscribeToEvents()

class GATTClientDiscoverAllPrimaryServicesOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTClientDiscoverAllPrimaryServicesRequestFrame
        self.observers = []
        super(GATTClientDiscoverAllPrimaryServicesOperation, self).subscribeToEvents()

class GATTClientDiscoverPrimaryServicesByUuidOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTClientDiscoverPrimaryServicesByUuidRequestFrame
        self.observers = []
        super(GATTClientDiscoverPrimaryServicesByUuidOperation, self).subscribeToEvents()

class GATTClientFindIncludedServicesOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTClientFindIncludedServicesRequestFrame
        self.observers = []
        super(GATTClientFindIncludedServicesOperation, self).subscribeToEvents()

class GATTClientDiscoverAllCharacteristicsOfServiceOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTClientDiscoverAllCharacteristicsOfServiceRequestFrame
        self.observers = []
        super(GATTClientDiscoverAllCharacteristicsOfServiceOperation, self).subscribeToEvents()

class GATTClientDiscoverCharacteristicOfServiceByUuidOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTClientDiscoverCharacteristicOfServiceByUuidRequestFrame
        self.observers = []
        super(GATTClientDiscoverCharacteristicOfServiceByUuidOperation, self).subscribeToEvents()

class GATTClientDiscoverAllCharacteristicDescriptorsOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTClientDiscoverAllCharacteristicDescriptorsRequestFrame
        self.observers = []
        super(GATTClientDiscoverAllCharacteristicDescriptorsOperation, self).subscribeToEvents()

class GATTClientReadCharacteristicValueOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTClientReadCharacteristicValueRequestFrame
        self.observers = []
        super(GATTClientReadCharacteristicValueOperation, self).subscribeToEvents()

class GATTClientReadUsingCharacteristicUuidOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTClientReadUsingCharacteristicUuidRequestFrame
        self.observers = []
        super(GATTClientReadUsingCharacteristicUuidOperation, self).subscribeToEvents()

class GATTClientReadMultipleCharacteristicValuesOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTClientReadMultipleCharacteristicValuesRequestFrame
        self.observers = []
        super(GATTClientReadMultipleCharacteristicValuesOperation, self).subscribeToEvents()

class GATTClientWriteCharacteristicValueOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTClientWriteCharacteristicValueRequestFrame
        self.observers = []
        super(GATTClientWriteCharacteristicValueOperation, self).subscribeToEvents()

class GATTClientReadCharacteristicDescriptorOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTClientReadCharacteristicDescriptorRequestFrame
        self.observers = []
        super(GATTClientReadCharacteristicDescriptorOperation, self).subscribeToEvents()

class GATTClientWriteCharacteristicDescriptorOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTClientWriteCharacteristicDescriptorRequestFrame
        self.observers = []
        super(GATTClientWriteCharacteristicDescriptorOperation, self).subscribeToEvents()

class GATTServerInitOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTServerInitRequestFrame
        self.observers = []
        super(GATTServerInitOperation, self).subscribeToEvents()

class GATTServerRegisterCallbackOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTServerRegisterCallbackRequestFrame
        self.observers = []
        super(GATTServerRegisterCallbackOperation, self).subscribeToEvents()

class GATTServerRegisterHandlesForWriteNotificationsOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTServerRegisterHandlesForWriteNotificationsRequestFrame
        self.observers = []
        super(GATTServerRegisterHandlesForWriteNotificationsOperation, self).subscribeToEvents()

class GATTServerSendAttributeWrittenStatusOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTServerSendAttributeWrittenStatusRequestFrame
        self.observers = []
        super(GATTServerSendAttributeWrittenStatusOperation, self).subscribeToEvents()

class GATTServerSendNotificationOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTServerSendNotificationRequestFrame
        self.observers = []
        super(GATTServerSendNotificationOperation, self).subscribeToEvents()

class GATTServerSendIndicationOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTServerSendIndicationRequestFrame
        self.observers = []
        super(GATTServerSendIndicationOperation, self).subscribeToEvents()

class GATTServerSendInstantValueNotificationOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTServerSendInstantValueNotificationRequestFrame
        self.observers = []
        super(GATTServerSendInstantValueNotificationOperation, self).subscribeToEvents()

class GATTServerSendInstantValueIndicationOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTServerSendInstantValueIndicationRequestFrame
        self.observers = []
        super(GATTServerSendInstantValueIndicationOperation, self).subscribeToEvents()

class GATTServerRegisterHandlesForReadNotificationsOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTServerRegisterHandlesForReadNotificationsRequestFrame
        self.observers = []
        super(GATTServerRegisterHandlesForReadNotificationsOperation, self).subscribeToEvents()

class GATTServerSendAttributeReadStatusOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTServerSendAttributeReadStatusRequestFrame
        self.observers = []
        super(GATTServerSendAttributeReadStatusOperation, self).subscribeToEvents()

class GATTDBModeSelectOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTDBModeSelectRequestFrame
        self.observers = []
        super(GATTDBModeSelectOperation, self).subscribeToEvents()

class GATTDBInitOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTDBInitRequestFrame
        self.observers = []
        super(GATTDBInitOperation, self).subscribeToEvents()

class GATTDBWriteAttributeOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTDBWriteAttributeRequestFrame
        self.observers = []
        super(GATTDBWriteAttributeOperation, self).subscribeToEvents()

class GATTDBReadAttributeOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTDBReadAttributeRequestFrame
        self.observers = [GATTDBReadAttributeIndicationObserver('GATTDBReadAttributeIndication'), ]
        super(GATTDBReadAttributeOperation, self).subscribeToEvents()

class GATTDBFindServiceHandleOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTDBFindServiceHandleRequestFrame
        self.observers = [GATTDBFindServiceHandleIndicationObserver('GATTDBFindServiceHandleIndication'), ]
        super(GATTDBFindServiceHandleOperation, self).subscribeToEvents()

class GATTDBFindCharValueHandleInServiceOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTDBFindCharValueHandleInServiceRequestFrame
        self.observers = [GATTDBFindCharValueHandleInServiceIndicationObserver('GATTDBFindCharValueHandleInServiceIndication'), ]
        super(GATTDBFindCharValueHandleInServiceOperation, self).subscribeToEvents()

class GATTDBFindCccdHandleForCharValueHandleOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTDBFindCccdHandleForCharValueHandleRequestFrame
        self.observers = [GATTDBFindCccdHandleForCharValueHandleIndicationObserver('GATTDBFindCccdHandleForCharValueHandleIndication'), ]
        super(GATTDBFindCccdHandleForCharValueHandleOperation, self).subscribeToEvents()

class GATTDBFindDescriptorHandleForCharValueHandleOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTDBFindDescriptorHandleForCharValueHandleRequestFrame
        self.observers = [GATTDBFindDescriptorHandleForCharValueHandleIndicationObserver('GATTDBFindDescriptorHandleForCharValueHandleIndication'), ]
        super(GATTDBFindDescriptorHandleForCharValueHandleOperation, self).subscribeToEvents()

class GATTDBDynamicInitOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTDBDynamicInitRequestFrame
        self.observers = []
        super(GATTDBDynamicInitOperation, self).subscribeToEvents()

class GATTDBDynamicReleaseDatabaseOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTDBDynamicReleaseDatabaseRequestFrame
        self.observers = []
        super(GATTDBDynamicReleaseDatabaseOperation, self).subscribeToEvents()

class GATTDBDynamicAddPrimaryServiceDeclarationOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTDBDynamicAddPrimaryServiceDeclarationRequestFrame
        self.observers = [GATTDBDynamicAddPrimaryServiceDeclarationIndicationObserver('GATTDBDynamicAddPrimaryServiceDeclarationIndication'), ]
        super(GATTDBDynamicAddPrimaryServiceDeclarationOperation, self).subscribeToEvents()

class GATTDBDynamicAddSecondaryServiceDeclarationOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTDBDynamicAddSecondaryServiceDeclarationRequestFrame
        self.observers = [GATTDBDynamicAddSecondaryServiceDeclarationIndicationObserver('GATTDBDynamicAddSecondaryServiceDeclarationIndication'), ]
        super(GATTDBDynamicAddSecondaryServiceDeclarationOperation, self).subscribeToEvents()

class GATTDBDynamicAddIncludeDeclarationOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTDBDynamicAddIncludeDeclarationRequestFrame
        self.observers = [GATTDBDynamicAddIncludeDeclarationIndicationObserver('GATTDBDynamicAddIncludeDeclarationIndication'), ]
        super(GATTDBDynamicAddIncludeDeclarationOperation, self).subscribeToEvents()

class GATTDBDynamicAddCharacteristicDeclarationAndValueOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTDBDynamicAddCharacteristicDeclarationAndValueRequestFrame
        self.observers = [GATTDBDynamicAddCharacteristicDeclarationAndValueIndicationObserver('GATTDBDynamicAddCharacteristicDeclarationAndValueIndication'), ]
        super(GATTDBDynamicAddCharacteristicDeclarationAndValueOperation, self).subscribeToEvents()

class GATTDBDynamicAddCharacteristicDescriptorOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTDBDynamicAddCharacteristicDescriptorRequestFrame
        self.observers = [GATTDBDynamicAddCharacteristicDescriptorIndicationObserver('GATTDBDynamicAddCharacteristicDescriptorIndication'), ]
        super(GATTDBDynamicAddCharacteristicDescriptorOperation, self).subscribeToEvents()

class GATTDBDynamicAddCccdOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTDBDynamicAddCccdRequestFrame
        self.observers = [GATTDBDynamicAddCccdIndicationObserver('GATTDBDynamicAddCccdIndication'), ]
        super(GATTDBDynamicAddCccdOperation, self).subscribeToEvents()

class GATTDBDynamicAddCharacteristicDeclarationWithUniqueValueOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTDBDynamicAddCharacteristicDeclarationWithUniqueValueRequestFrame
        self.observers = [GATTDBDynamicAddCharacteristicDeclarationWithUniqueValueIndicationObserver('GATTDBDynamicAddCharacteristicDeclarationWithUniqueValueIndication'), ]
        super(GATTDBDynamicAddCharacteristicDeclarationWithUniqueValueOperation, self).subscribeToEvents()

class GATTDBDynamicRemoveServiceOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTDBDynamicRemoveServiceRequestFrame
        self.observers = []
        super(GATTDBDynamicRemoveServiceOperation, self).subscribeToEvents()

class GATTDBDynamicRemoveCharacteristicOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTDBDynamicRemoveCharacteristicRequestFrame
        self.observers = []
        super(GATTDBDynamicRemoveCharacteristicOperation, self).subscribeToEvents()

class GATTDBAttModeSelectOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTDBAttModeSelectRequestFrame
        self.observers = []
        super(GATTDBAttModeSelectOperation, self).subscribeToEvents()

class GATTDBAttFindInformationOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTDBAttFindInformationRequestFrame
        self.observers = [GATTDBAttFindInformationIndicationObserver('GATTDBAttFindInformationIndication'), ]
        super(GATTDBAttFindInformationOperation, self).subscribeToEvents()

class GATTDBAttFindByTypeValueOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTDBAttFindByTypeValueRequestFrame
        self.observers = [GATTDBAttFindByTypeValueIndicationObserver('GATTDBAttFindByTypeValueIndication'), ]
        super(GATTDBAttFindByTypeValueOperation, self).subscribeToEvents()

class GATTDBAttReadByTypeOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTDBAttReadByTypeRequestFrame
        self.observers = [GATTDBAttReadByTypeIndicationObserver('GATTDBAttReadByTypeIndication'), ]
        super(GATTDBAttReadByTypeOperation, self).subscribeToEvents()

class GATTDBAttReadOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTDBAttReadRequestFrame
        self.observers = [GATTDBAttReadIndicationObserver('GATTDBAttReadIndication'), ]
        super(GATTDBAttReadOperation, self).subscribeToEvents()

class GATTDBAttReadBlobOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTDBAttReadBlobRequestFrame
        self.observers = [GATTDBAttReadBlobIndicationObserver('GATTDBAttReadBlobIndication'), ]
        super(GATTDBAttReadBlobOperation, self).subscribeToEvents()

class GATTDBAttReadMultipleOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTDBAttReadMultipleRequestFrame
        self.observers = [GATTDBAttReadMultipleIndicationObserver('GATTDBAttReadMultipleIndication'), ]
        super(GATTDBAttReadMultipleOperation, self).subscribeToEvents()

class GATTDBAttReadByGroupTypeOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTDBAttReadByGroupTypeRequestFrame
        self.observers = [GATTDBAttReadByGroupTypeIndicationObserver('GATTDBAttReadByGroupTypeIndication'), ]
        super(GATTDBAttReadByGroupTypeOperation, self).subscribeToEvents()

class GATTDBAttWriteOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTDBAttWriteRequestFrame
        self.observers = [GATTDBAttWriteIndicationObserver('GATTDBAttWriteIndication'), ]
        super(GATTDBAttWriteOperation, self).subscribeToEvents()

class GATTDBAttWriteCommandOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTDBAttWriteCommandRequestFrame
        self.observers = []
        super(GATTDBAttWriteCommandOperation, self).subscribeToEvents()

class GATTDBAttSignedWriteCommandOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTDBAttSignedWriteCommandRequestFrame
        self.observers = []
        super(GATTDBAttSignedWriteCommandOperation, self).subscribeToEvents()

class GATTDBAttPrepareWriteOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTDBAttPrepareWriteRequestFrame
        self.observers = [GATTDBAttPrepareWriteIndicationObserver('GATTDBAttPrepareWriteIndication'), ]
        super(GATTDBAttPrepareWriteOperation, self).subscribeToEvents()

class GATTDBAttExecuteWriteOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTDBAttExecuteWriteRequestFrame
        self.observers = [GATTDBAttExecuteWriteIndicationObserver('GATTDBAttExecuteWriteIndication'), ]
        super(GATTDBAttExecuteWriteOperation, self).subscribeToEvents()

class GATTDBAttExecuteWriteFromQueueOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTDBAttExecuteWriteFromQueueRequestFrame
        self.observers = [GATTDBAttExecuteWriteFromQueueIndicationObserver('GATTDBAttExecuteWriteFromQueueIndication'), ]
        super(GATTDBAttExecuteWriteFromQueueOperation, self).subscribeToEvents()

class GATTDBAttPrepareNotificationIndicationOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GATTDBAttPrepareNotificationIndicationRequestFrame
        self.observers = []
        super(GATTDBAttPrepareNotificationIndicationOperation, self).subscribeToEvents()

class GAPModeSelectOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPModeSelectRequestFrame
        self.observers = []
        super(GAPModeSelectOperation, self).subscribeToEvents()

class BLEHostInitializeOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.BLEHostInitializeRequestFrame
        self.observers = []
        super(BLEHostInitializeOperation, self).subscribeToEvents()

class GAPRegisterDeviceSecurityRequirementsOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPRegisterDeviceSecurityRequirementsRequestFrame
        self.observers = []
        super(GAPRegisterDeviceSecurityRequirementsOperation, self).subscribeToEvents()

class GAPSetAdvertisingParametersOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPSetAdvertisingParametersRequestFrame
        self.observers = [GAPGenericEventAdvertisingParametersSetupCompleteIndicationObserver('GAPGenericEventAdvertisingParametersSetupCompleteIndication'), ]
        super(GAPSetAdvertisingParametersOperation, self).subscribeToEvents()

class GAPSetAdvertisingDataOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPSetAdvertisingDataRequestFrame
        self.observers = [GAPGenericEventAdvertisingDataSetupCompleteIndicationObserver('GAPGenericEventAdvertisingDataSetupCompleteIndication') ]
        super(GAPSetAdvertisingDataOperation, self).subscribeToEvents()

class GAPStartAdvertisingOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPStartAdvertisingRequestFrame
        self.observers = [GAPAdvertisingEventStateChangedIndicationObserver('GAPAdvertisingEventStateChangedIndication'), ]
        super(GAPStartAdvertisingOperation, self).subscribeToEvents()

class GAPStopAdvertisingOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPStopAdvertisingRequestFrame
        self.observers = []
        super(GAPStopAdvertisingOperation, self).subscribeToEvents()

class GAPAuthorizeOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPAuthorizeRequestFrame
        self.observers = []
        super(GAPAuthorizeOperation, self).subscribeToEvents()

class GAPSaveCccdOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPSaveCccdRequestFrame
        self.observers = []
        super(GAPSaveCccdOperation, self).subscribeToEvents()

class GAPCheckNotificationStatusOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPCheckNotificationStatusRequestFrame
        self.observers = [GAPCheckNotificationStatusIndicationObserver('GAPCheckNotificationStatusIndication'), ]
        super(GAPCheckNotificationStatusOperation, self).subscribeToEvents()

class GAPCheckIndicationStatusOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPCheckIndicationStatusRequestFrame
        self.observers = []
        super(GAPCheckIndicationStatusOperation, self).subscribeToEvents()

class GAPGetBondedStaticAddressesOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPGetBondedStaticAddressesRequestFrame
        self.observers = [GAPGetBondedStaticAddressesIndicationObserver('GAPGetBondedStaticAddressesIndication'), ]
        super(GAPGetBondedStaticAddressesOperation, self).subscribeToEvents()

class GAPPairOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPPairRequestFrame
        self.observers = []
        super(GAPPairOperation, self).subscribeToEvents()

class GAPSendSlaveSecurityRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPSendSlaveSecurityRequestRequestFrame
        self.observers = []
        super(GAPSendSlaveSecurityRequestOperation, self).subscribeToEvents()

class GAPEncryptLinkOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPEncryptLinkRequestFrame
        self.observers = []
        super(GAPEncryptLinkOperation, self).subscribeToEvents()

class GAPAcceptPairingRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPAcceptPairingRequestRequestFrame
        self.observers = []
        super(GAPAcceptPairingRequestOperation, self).subscribeToEvents()

class GAPRejectPairingOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPRejectPairingRequestFrame
        self.observers = []
        super(GAPRejectPairingOperation, self).subscribeToEvents()

class GAPEnterPasskeyOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPEnterPasskeyRequestFrame
        self.observers = []
        super(GAPEnterPasskeyOperation, self).subscribeToEvents()

class GAPProvideOobOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPProvideOobRequestFrame
        self.observers = []
        super(GAPProvideOobOperation, self).subscribeToEvents()

class GAPRejectPasskeyRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPRejectPasskeyRequestRequestFrame
        self.observers = []
        super(GAPRejectPasskeyRequestOperation, self).subscribeToEvents()

class GAPSendSmpKeysOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPSendSmpKeysRequestFrame
        self.observers = []
        super(GAPSendSmpKeysOperation, self).subscribeToEvents()

class GAPRejectKeyExchangeRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPRejectKeyExchangeRequestRequestFrame
        self.observers = []
        super(GAPRejectKeyExchangeRequestOperation, self).subscribeToEvents()

class GAPProvideLongTermKeyOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPProvideLongTermKeyRequestFrame
        self.observers = []
        super(GAPProvideLongTermKeyOperation, self).subscribeToEvents()

class GAPDenyLongTermKeyOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPDenyLongTermKeyRequestFrame
        self.observers = []
        super(GAPDenyLongTermKeyOperation, self).subscribeToEvents()

class GAPLoadEncryptionInformationOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPLoadEncryptionInformationRequestFrame
        self.observers = [GAPLoadEncryptionInformationIndicationObserver('GAPLoadEncryptionInformationIndication'), ]
        super(GAPLoadEncryptionInformationOperation, self).subscribeToEvents()

class GAPSetLocalPasskeyOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPSetLocalPasskeyRequestFrame
        self.observers = []
        super(GAPSetLocalPasskeyOperation, self).subscribeToEvents()

class GAPStartScanningOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPStartScanningRequestFrame
        self.observers = []
        super(GAPStartScanningOperation, self).subscribeToEvents()

class GAPStopScanningOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPStopScanningRequestFrame
        self.observers = []
        super(GAPStopScanningOperation, self).subscribeToEvents()

class GAPConnectOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPConnectRequestFrame
        self.observers = []
        super(GAPConnectOperation, self).subscribeToEvents()

class GAPDisconnectOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPDisconnectRequestFrame
        self.observers = []
        super(GAPDisconnectOperation, self).subscribeToEvents()

class GAPSaveCustomPeerInformationOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPSaveCustomPeerInformationRequestFrame
        self.observers = []
        super(GAPSaveCustomPeerInformationOperation, self).subscribeToEvents()

class GAPLoadCustomPeerInformationOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPLoadCustomPeerInformationRequestFrame
        self.observers = [GAPLoadCustomPeerInformationIndicationObserver('GAPLoadCustomPeerInformationIndication'), ]
        super(GAPLoadCustomPeerInformationOperation, self).subscribeToEvents()

class GAPCheckIfBondedOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPCheckIfBondedRequestFrame
        self.observers = [GAPCheckIfBondedIndicationObserver('GAPCheckIfBondedIndication'), ]
        super(GAPCheckIfBondedOperation, self).subscribeToEvents()

class GAPReadWhiteListSizeOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPReadWhiteListSizeRequestFrame
        self.observers = []
        super(GAPReadWhiteListSizeOperation, self).subscribeToEvents()

class GAPClearWhiteListOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPClearWhiteListRequestFrame
        self.observers = []
        super(GAPClearWhiteListOperation, self).subscribeToEvents()

class GAPAddDeviceToWhiteListOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPAddDeviceToWhiteListRequestFrame
        self.observers = []
        super(GAPAddDeviceToWhiteListOperation, self).subscribeToEvents()

class GAPRemoveDeviceFromWhiteListOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPRemoveDeviceFromWhiteListRequestFrame
        self.observers = []
        super(GAPRemoveDeviceFromWhiteListOperation, self).subscribeToEvents()

class GAPReadPublicDeviceAddressOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPReadPublicDeviceAddressRequestFrame
        self.observers = [GAPGenericEventPublicAddressReadIndicationObserver('GAPGenericEventPublicAddressReadIndication'), ]
        super(GAPReadPublicDeviceAddressOperation, self).subscribeToEvents()

class GAPCreateRandomDeviceAddressOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPCreateRandomDeviceAddressRequestFrame
        self.observers = []
        super(GAPCreateRandomDeviceAddressOperation, self).subscribeToEvents()

class GAPSaveDeviceNameOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPSaveDeviceNameRequestFrame
        self.observers = []
        super(GAPSaveDeviceNameOperation, self).subscribeToEvents()

class GAPGetBondedDevicesCountOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPGetBondedDevicesCountRequestFrame
        self.observers = [GAPGetBondedDevicesCountIndicationObserver('GAPGetBondedDevicesCountIndication'), ]
        super(GAPGetBondedDevicesCountOperation, self).subscribeToEvents()

class GAPGetBondedDeviceNameOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPGetBondedDeviceNameRequestFrame
        self.observers = [GAPGetBondedDeviceNameIndicationObserver('GAPGetBondedDeviceNameIndication'), ]
        super(GAPGetBondedDeviceNameOperation, self).subscribeToEvents()

class GAPRemoveBondOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPRemoveBondRequestFrame
        self.observers = []
        super(GAPRemoveBondOperation, self).subscribeToEvents()

class GAPRemoveAllBondsOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPRemoveAllBondsRequestFrame
        self.observers = []
        super(GAPRemoveAllBondsOperation, self).subscribeToEvents()

class GAPReadRadioPowerLevelOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPReadRadioPowerLevelRequestFrame
        self.observers = []
        super(GAPReadRadioPowerLevelOperation, self).subscribeToEvents()

class GAPVerifyPrivateResolvableAddressOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPVerifyPrivateResolvableAddressRequestFrame
        self.observers = []
        super(GAPVerifyPrivateResolvableAddressOperation, self).subscribeToEvents()

class GAPSetRandomAddressOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPSetRandomAddressRequestFrame
        self.observers = []
        super(GAPSetRandomAddressOperation, self).subscribeToEvents()

class GAPSetScanModeOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPSetScanModeRequestFrame
        self.observers = []
        super(GAPSetScanModeOperation, self).subscribeToEvents()

class GAPSetDefaultPairingParametersOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPSetDefaultPairingParametersRequestFrame
        self.observers = []
        super(GAPSetDefaultPairingParametersOperation, self).subscribeToEvents()

class GAPUpdateConnectionParametersOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPUpdateConnectionParametersRequestFrame
        self.observers = []
        super(GAPUpdateConnectionParametersOperation, self).subscribeToEvents()

class GAPEnableUpdateConnectionParametersOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPEnableUpdateConnectionParametersRequestFrame
        self.observers = []
        super(GAPEnableUpdateConnectionParametersOperation, self).subscribeToEvents()

class GapUpdateLeDataLengthOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GapUpdateLeDataLengthRequestFrame
        self.observers = []
        super(GapUpdateLeDataLengthOperation, self).subscribeToEvents()

class GAPControllerResetOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPControllerResetRequestFrame
        self.observers = []
        super(GAPControllerResetOperation, self).subscribeToEvents()

class GAPEnableHostPrivacyOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPEnableHostPrivacyRequestFrame
        self.observers = []
        super(GAPEnableHostPrivacyOperation, self).subscribeToEvents()

class GAPEnableControllerPrivacyOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPEnableControllerPrivacyRequestFrame
        self.observers = []
        super(GAPEnableControllerPrivacyOperation, self).subscribeToEvents()

class GAPLeScRegeneratePublicKeyOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPLeScRegeneratePublicKeyRequestFrame
        self.observers = []
        super(GAPLeScRegeneratePublicKeyOperation, self).subscribeToEvents()

class GAPLeScValidateNumericValueOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPLeScValidateNumericValueRequestFrame
        self.observers = []
        super(GAPLeScValidateNumericValueOperation, self).subscribeToEvents()

class GAPLeScGetLocalOobDataOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPLeScGetLocalOobDataRequestFrame
        self.observers = []
        super(GAPLeScGetLocalOobDataOperation, self).subscribeToEvents()

class GAPLeScSetPeerOobDataOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPLeScSetPeerOobDataRequestFrame
        self.observers = []
        super(GAPLeScSetPeerOobDataOperation, self).subscribeToEvents()

class GAPLeScSendKeypressNotificationPrivacyOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPLeScSendKeypressNotificationPrivacyRequestFrame
        self.observers = []
        super(GAPLeScSendKeypressNotificationPrivacyOperation, self).subscribeToEvents()

class GAPGetBondedDevicesIdentityInformationOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.GAPGetBondedDevicesIdentityInformationRequestFrame
        self.observers = [GAPGetBondedDevicesIdentityInformationIndicationObserver('GAPGetBondedDevicesIdentityInformationIndication'), ]
        super(GAPGetBondedDevicesIdentityInformationOperation, self).subscribeToEvents()

class FSCIAllowDeviceToSleepOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.FSCIAllowDeviceToSleepRequestFrame
        self.observers = [FSCIAllowDeviceToSleepConfirmObserver('FSCIAllowDeviceToSleepConfirm'), ]
        super(FSCIAllowDeviceToSleepOperation, self).subscribeToEvents()

class FSCIGetWakeupReasonOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = Spec.FSCIGetWakeupReasonRequestFrame
        self.observers = [FSCIGetWakeupReasonResponseObserver('FSCIGetWakeupReasonResponse'), ]
        super(FSCIGetWakeupReasonOperation, self).subscribeToEvents()

class FSCIErrorOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [FSCIErrorIndicationObserver('FSCIErrorIndication'), ]
        super(FSCIErrorOperation, self).subscribeToEvents()

class FSCIWakeUpOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [FSCIWakeUpIndicationObserver('FSCIWakeUpIndication'), ]
        super(FSCIWakeUpOperation, self).subscribeToEvents()

class HCIEventOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [HCIEventIndicationObserver('HCIEventIndication'), ]
        super(HCIEventOperation, self).subscribeToEvents()

class L2CAPAttDataOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [L2CAPAttDataIndicationObserver('L2CAPAttDataIndication'), ]
        super(L2CAPAttDataOperation, self).subscribeToEvents()

class L2CAPSmpDataOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [L2CAPSmpDataIndicationObserver('L2CAPSmpDataIndication'), ]
        super(L2CAPSmpDataOperation, self).subscribeToEvents()

class L2CAPLePsmConnectionRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [L2CAPLePsmConnectionRequestIndicationObserver('L2CAPLePsmConnectionRequestIndication'), ]
        super(L2CAPLePsmConnectionRequestOperation, self).subscribeToEvents()

class L2CAPLePsmConnectionCompleteOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [L2CAPLePsmConnectionCompleteIndicationObserver('L2CAPLePsmConnectionCompleteIndication'), ]
        super(L2CAPLePsmConnectionCompleteOperation, self).subscribeToEvents()

class L2CAPLePsmDisconnectNotificationOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [L2CAPLePsmDisconnectNotificationIndicationObserver('L2CAPLePsmDisconnectNotificationIndication'), ]
        super(L2CAPLePsmDisconnectNotificationOperation, self).subscribeToEvents()

class L2CAPNoPeerCreditsOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [L2CAPNoPeerCreditsIndicationObserver('L2CAPNoPeerCreditsIndication'), ]
        super(L2CAPNoPeerCreditsOperation, self).subscribeToEvents()

class L2CAPLocalCreditsNotificationOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [L2CAPLocalCreditsNotificationIndicationObserver('L2CAPLocalCreditsNotificationIndication'), ]
        super(L2CAPLocalCreditsNotificationOperation, self).subscribeToEvents()

class L2CAPLeCbDataOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [L2CAPLeCbDataIndicationObserver('L2CAPLeCbDataIndication'), ]
        super(L2CAPLeCbDataOperation, self).subscribeToEvents()

class SMStatusOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [SMStatusIndicationObserver('SMStatusIndication'), ]
        super(SMStatusOperation, self).subscribeToEvents()

class SMRemoteSecurityRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [SMRemoteSecurityRequestIndicationObserver('SMRemoteSecurityRequestIndication'), ]
        super(SMRemoteSecurityRequestOperation, self).subscribeToEvents()

class SMRemotePairingRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [SMRemotePairingRequestIndicationObserver('SMRemotePairingRequestIndication'), ]
        super(SMRemotePairingRequestOperation, self).subscribeToEvents()

class SMRemotePairingResponseOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [SMRemotePairingResponseIndicationObserver('SMRemotePairingResponseIndication'), ]
        super(SMRemotePairingResponseOperation, self).subscribeToEvents()

class SMPasskeyDisplayRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [SMPasskeyDisplayRequestIndicationObserver('SMPasskeyDisplayRequestIndication'), ]
        super(SMPasskeyDisplayRequestOperation, self).subscribeToEvents()

class SMPasskeyRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [SMPasskeyRequestIndicationObserver('SMPasskeyRequestIndication'), ]
        super(SMPasskeyRequestOperation, self).subscribeToEvents()

class SMPairingKeysetRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [SMPairingKeysetRequestIndicationObserver('SMPairingKeysetRequestIndication'), ]
        super(SMPairingKeysetRequestOperation, self).subscribeToEvents()

class SMPairingKeysetReceivedOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [SMPairingKeysetReceivedIndicationObserver('SMPairingKeysetReceivedIndication'), ]
        super(SMPairingKeysetReceivedOperation, self).subscribeToEvents()

class SMPairingCompleteOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [SMPairingCompleteIndicationObserver('SMPairingCompleteIndication'), ]
        super(SMPairingCompleteOperation, self).subscribeToEvents()

class SMPairingFailedOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [SMPairingFailedIndicationObserver('SMPairingFailedIndication'), ]
        super(SMPairingFailedOperation, self).subscribeToEvents()

class SMReceivedPairingFailedOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [SMReceivedPairingFailedIndicationObserver('SMReceivedPairingFailedIndication'), ]
        super(SMReceivedPairingFailedOperation, self).subscribeToEvents()

class SMLlLtkRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [SMLlLtkRequestIndicationObserver('SMLlLtkRequestIndication'), ]
        super(SMLlLtkRequestOperation, self).subscribeToEvents()

class SMLlEncryptionStatusOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [SMLlEncryptionStatusIndicationObserver('SMLlEncryptionStatusIndication'), ]
        super(SMLlEncryptionStatusOperation, self).subscribeToEvents()

class SMLlEncryptResOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [SMLlEncryptResIndicationObserver('SMLlEncryptResIndication'), ]
        super(SMLlEncryptResOperation, self).subscribeToEvents()

class SMLlRandResOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [SMLlRandResIndicationObserver('SMLlRandResIndication'), ]
        super(SMLlRandResOperation, self).subscribeToEvents()

class SMTbCreateRandomDeviceAddrResOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [SMTbCreateRandomDeviceAddrResIndicationObserver('SMTbCreateRandomDeviceAddrResIndication'), ]
        super(SMTbCreateRandomDeviceAddrResOperation, self).subscribeToEvents()

class SMTbCheckResolvalePrivateAddrResOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [SMTbCheckResolvalePrivateAddrResIndicationObserver('SMTbCheckResolvalePrivateAddrResIndication'), ]
        super(SMTbCheckResolvalePrivateAddrResOperation, self).subscribeToEvents()

class SMTbSignDataResOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [SMTbSignDataResIndicationObserver('SMTbSignDataResIndication'), ]
        super(SMTbSignDataResOperation, self).subscribeToEvents()

class SMTbVerifyDataSignatureResOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [SMTbVerifyDataSignatureResIndicationObserver('SMTbVerifyDataSignatureResIndication'), ]
        super(SMTbVerifyDataSignatureResOperation, self).subscribeToEvents()

class SMRemoteKeypressNotificationOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [SMRemoteKeypressNotificationIndicationObserver('SMRemoteKeypressNotificationIndication'), ]
        super(SMRemoteKeypressNotificationOperation, self).subscribeToEvents()

class SMNcDisplayRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [SMNcDisplayRequestIndicationObserver('SMNcDisplayRequestIndication'), ]
        super(SMNcDisplayRequestOperation, self).subscribeToEvents()

class SMLeScOobDataRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [SMLeScOobDataRequestIndicationObserver('SMLeScOobDataRequestIndication'), ]
        super(SMLeScOobDataRequestOperation, self).subscribeToEvents()

class SMGenerateNewEcdhPkSkPairResOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [SMGenerateNewEcdhPkSkPairResIndicationObserver('SMGenerateNewEcdhPkSkPairResIndication'), ]
        super(SMGenerateNewEcdhPkSkPairResOperation, self).subscribeToEvents()

class ATTClientIncomingServerErrorResponseOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [ATTClientIncomingServerErrorResponseIndicationObserver('ATTClientIncomingServerErrorResponseIndication'), ]
        super(ATTClientIncomingServerErrorResponseOperation, self).subscribeToEvents()

class ATTServerIncomingClientExchangeMtuRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [ATTServerIncomingClientExchangeMtuRequestIndicationObserver('ATTServerIncomingClientExchangeMtuRequestIndication'), ]
        super(ATTServerIncomingClientExchangeMtuRequestOperation, self).subscribeToEvents()

class ATTClientIncomingServerExchangeMtuResponseOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [ATTClientIncomingServerExchangeMtuResponseIndicationObserver('ATTClientIncomingServerExchangeMtuResponseIndication'), ]
        super(ATTClientIncomingServerExchangeMtuResponseOperation, self).subscribeToEvents()

class ATTServerIncomingClientFindInformationRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [ATTServerIncomingClientFindInformationRequestIndicationObserver('ATTServerIncomingClientFindInformationRequestIndication'), ]
        super(ATTServerIncomingClientFindInformationRequestOperation, self).subscribeToEvents()

class ATTClientIncomingServerFindInformationResponseOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [ATTClientIncomingServerFindInformationResponseIndicationObserver('ATTClientIncomingServerFindInformationResponseIndication'), ]
        super(ATTClientIncomingServerFindInformationResponseOperation, self).subscribeToEvents()

class ATTServerIncomingClientFindByTypeValueRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [ATTServerIncomingClientFindByTypeValueRequestIndicationObserver('ATTServerIncomingClientFindByTypeValueRequestIndication'), ]
        super(ATTServerIncomingClientFindByTypeValueRequestOperation, self).subscribeToEvents()

class ATTClientIncomingServerFindByTypeValueResponseOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [ATTClientIncomingServerFindByTypeValueResponseIndicationObserver('ATTClientIncomingServerFindByTypeValueResponseIndication'), ]
        super(ATTClientIncomingServerFindByTypeValueResponseOperation, self).subscribeToEvents()

class ATTServerIncomingClientReadByTypeRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [ATTServerIncomingClientReadByTypeRequestIndicationObserver('ATTServerIncomingClientReadByTypeRequestIndication'), ]
        super(ATTServerIncomingClientReadByTypeRequestOperation, self).subscribeToEvents()

class ATTClientIncomingServerReadByTypeResponseOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [ATTClientIncomingServerReadByTypeResponseIndicationObserver('ATTClientIncomingServerReadByTypeResponseIndication'), ]
        super(ATTClientIncomingServerReadByTypeResponseOperation, self).subscribeToEvents()

class ATTServerIncomingClientReadRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [ATTServerIncomingClientReadRequestIndicationObserver('ATTServerIncomingClientReadRequestIndication'), ]
        super(ATTServerIncomingClientReadRequestOperation, self).subscribeToEvents()

class ATTClientIncomingServerReadResponseOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [ATTClientIncomingServerReadResponseIndicationObserver('ATTClientIncomingServerReadResponseIndication'), ]
        super(ATTClientIncomingServerReadResponseOperation, self).subscribeToEvents()

class ATTServerIncomingClientReadBlobRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [ATTServerIncomingClientReadBlobRequestIndicationObserver('ATTServerIncomingClientReadBlobRequestIndication'), ]
        super(ATTServerIncomingClientReadBlobRequestOperation, self).subscribeToEvents()

class ATTClientIncomingServerReadBlobResponseOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [ATTClientIncomingServerReadBlobResponseIndicationObserver('ATTClientIncomingServerReadBlobResponseIndication'), ]
        super(ATTClientIncomingServerReadBlobResponseOperation, self).subscribeToEvents()

class ATTServerIncomingClientReadMultipleRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [ATTServerIncomingClientReadMultipleRequestIndicationObserver('ATTServerIncomingClientReadMultipleRequestIndication'), ]
        super(ATTServerIncomingClientReadMultipleRequestOperation, self).subscribeToEvents()

class ATTClientIncomingServerReadMultipleResponseOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [ATTClientIncomingServerReadMultipleResponseIndicationObserver('ATTClientIncomingServerReadMultipleResponseIndication'), ]
        super(ATTClientIncomingServerReadMultipleResponseOperation, self).subscribeToEvents()

class ATTServerIncomingClientReadByGroupTypeRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [ATTServerIncomingClientReadByGroupTypeRequestIndicationObserver('ATTServerIncomingClientReadByGroupTypeRequestIndication'), ]
        super(ATTServerIncomingClientReadByGroupTypeRequestOperation, self).subscribeToEvents()

class ATTClientIncomingServerReadByGroupTypeResponseOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [ATTClientIncomingServerReadByGroupTypeResponseIndicationObserver('ATTClientIncomingServerReadByGroupTypeResponseIndication'), ]
        super(ATTClientIncomingServerReadByGroupTypeResponseOperation, self).subscribeToEvents()

class ATTServerIncomingClientWriteRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [ATTServerIncomingClientWriteRequestIndicationObserver('ATTServerIncomingClientWriteRequestIndication'), ]
        super(ATTServerIncomingClientWriteRequestOperation, self).subscribeToEvents()

class ATTClientIncomingServerWriteResponseOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [ATTClientIncomingServerWriteResponseIndicationObserver('ATTClientIncomingServerWriteResponseIndication'), ]
        super(ATTClientIncomingServerWriteResponseOperation, self).subscribeToEvents()

class ATTServerIncomingClientWriteCommandOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [ATTServerIncomingClientWriteCommandIndicationObserver('ATTServerIncomingClientWriteCommandIndication'), ]
        super(ATTServerIncomingClientWriteCommandOperation, self).subscribeToEvents()

class ATTServerIncomingClientSignedWriteCommandOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [ATTServerIncomingClientSignedWriteCommandIndicationObserver('ATTServerIncomingClientSignedWriteCommandIndication'), ]
        super(ATTServerIncomingClientSignedWriteCommandOperation, self).subscribeToEvents()

class ATTServerIncomingClientPrepareWriteRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [ATTServerIncomingClientPrepareWriteRequestIndicationObserver('ATTServerIncomingClientPrepareWriteRequestIndication'), ]
        super(ATTServerIncomingClientPrepareWriteRequestOperation, self).subscribeToEvents()

class ATTClientIncomingServerPrepareWriteResponseOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [ATTClientIncomingServerPrepareWriteResponseIndicationObserver('ATTClientIncomingServerPrepareWriteResponseIndication'), ]
        super(ATTClientIncomingServerPrepareWriteResponseOperation, self).subscribeToEvents()

class ATTServerIncomingClientExecuteWriteRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [ATTServerIncomingClientExecuteWriteRequestIndicationObserver('ATTServerIncomingClientExecuteWriteRequestIndication'), ]
        super(ATTServerIncomingClientExecuteWriteRequestOperation, self).subscribeToEvents()

class ATTClientIncomingServerExecuteWriteResponseOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [ATTClientIncomingServerExecuteWriteResponseIndicationObserver('ATTClientIncomingServerExecuteWriteResponseIndication'), ]
        super(ATTClientIncomingServerExecuteWriteResponseOperation, self).subscribeToEvents()

class ATTClientIncomingServerHandleValueNotificationOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [ATTClientIncomingServerHandleValueNotificationIndicationObserver('ATTClientIncomingServerHandleValueNotificationIndication'), ]
        super(ATTClientIncomingServerHandleValueNotificationOperation, self).subscribeToEvents()

class ATTClientIncomingServerHandleValueIndicationOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [ATTClientIncomingServerHandleValueIndicationIndicationObserver('ATTClientIncomingServerHandleValueIndicationIndication'), ]
        super(ATTClientIncomingServerHandleValueIndicationOperation, self).subscribeToEvents()

class ATTServerIncomingClientHandleValueConfirmationOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [ATTServerIncomingClientHandleValueConfirmationIndicationObserver('ATTServerIncomingClientHandleValueConfirmationIndication'), ]
        super(ATTServerIncomingClientHandleValueConfirmationOperation, self).subscribeToEvents()

class ATTUnsupportedOpcodeOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [ATTUnsupportedOpcodeIndicationObserver('ATTUnsupportedOpcodeIndication'), ]
        super(ATTUnsupportedOpcodeOperation, self).subscribeToEvents()

class ATTTimeoutOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [ATTTimeoutIndicationObserver('ATTTimeoutIndication'), ]
        super(ATTTimeoutOperation, self).subscribeToEvents()

class GATTClientProcedureExchangeMtuOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GATTClientProcedureExchangeMtuIndicationObserver('GATTClientProcedureExchangeMtuIndication'), ]
        super(GATTClientProcedureExchangeMtuOperation, self).subscribeToEvents()

class GATTClientProcedureDiscoverAllPrimaryServicesOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GATTClientProcedureDiscoverAllPrimaryServicesIndicationObserver('GATTClientProcedureDiscoverAllPrimaryServicesIndication'), ]
        super(GATTClientProcedureDiscoverAllPrimaryServicesOperation, self).subscribeToEvents()

class GATTClientProcedureDiscoverPrimaryServicesByUuidOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GATTClientProcedureDiscoverPrimaryServicesByUuidIndicationObserver('GATTClientProcedureDiscoverPrimaryServicesByUuidIndication'), ]
        super(GATTClientProcedureDiscoverPrimaryServicesByUuidOperation, self).subscribeToEvents()

class GATTClientProcedureFindIncludedServicesOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GATTClientProcedureFindIncludedServicesIndicationObserver('GATTClientProcedureFindIncludedServicesIndication'), ]
        super(GATTClientProcedureFindIncludedServicesOperation, self).subscribeToEvents()

class GATTClientProcedureDiscoverAllCharacteristicsOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GATTClientProcedureDiscoverAllCharacteristicsIndicationObserver('GATTClientProcedureDiscoverAllCharacteristicsIndication'), ]
        super(GATTClientProcedureDiscoverAllCharacteristicsOperation, self).subscribeToEvents()

class GATTClientProcedureDiscoverCharacteristicByUuidOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GATTClientProcedureDiscoverCharacteristicByUuidIndicationObserver('GATTClientProcedureDiscoverCharacteristicByUuidIndication'), ]
        super(GATTClientProcedureDiscoverCharacteristicByUuidOperation, self).subscribeToEvents()

class GATTClientProcedureDiscoverAllCharacteristicDescriptorsOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GATTClientProcedureDiscoverAllCharacteristicDescriptorsIndicationObserver('GATTClientProcedureDiscoverAllCharacteristicDescriptorsIndication'), ]
        super(GATTClientProcedureDiscoverAllCharacteristicDescriptorsOperation, self).subscribeToEvents()

class GATTClientProcedureReadCharacteristicValueOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GATTClientProcedureReadCharacteristicValueIndicationObserver('GATTClientProcedureReadCharacteristicValueIndication'), ]
        super(GATTClientProcedureReadCharacteristicValueOperation, self).subscribeToEvents()

class GATTClientProcedureReadUsingCharacteristicUuidOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GATTClientProcedureReadUsingCharacteristicUuidIndicationObserver('GATTClientProcedureReadUsingCharacteristicUuidIndication'), ]
        super(GATTClientProcedureReadUsingCharacteristicUuidOperation, self).subscribeToEvents()

class GATTClientProcedureReadMultipleCharacteristicValuesOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GATTClientProcedureReadMultipleCharacteristicValuesIndicationObserver('GATTClientProcedureReadMultipleCharacteristicValuesIndication'), ]
        super(GATTClientProcedureReadMultipleCharacteristicValuesOperation, self).subscribeToEvents()

class GATTClientProcedureWriteCharacteristicValueOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GATTClientProcedureWriteCharacteristicValueIndicationObserver('GATTClientProcedureWriteCharacteristicValueIndication'), ]
        super(GATTClientProcedureWriteCharacteristicValueOperation, self).subscribeToEvents()

class GATTClientProcedureReadCharacteristicDescriptorOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GATTClientProcedureReadCharacteristicDescriptorIndicationObserver('GATTClientProcedureReadCharacteristicDescriptorIndication'), ]
        super(GATTClientProcedureReadCharacteristicDescriptorOperation, self).subscribeToEvents()

class GATTClientProcedureWriteCharacteristicDescriptorOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GATTClientProcedureWriteCharacteristicDescriptorIndicationObserver('GATTClientProcedureWriteCharacteristicDescriptorIndication'), ]
        super(GATTClientProcedureWriteCharacteristicDescriptorOperation, self).subscribeToEvents()

class GATTClientNotificationOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GATTClientNotificationIndicationObserver('GATTClientNotificationIndication'), ]
        super(GATTClientNotificationOperation, self).subscribeToEvents()

class GATTClientIndicationOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GATTClientIndicationIndicationObserver('GATTClientIndicationIndication'), ]
        super(GATTClientIndicationOperation, self).subscribeToEvents()

class GATTServerMtuChangedOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GATTServerMtuChangedIndicationObserver('GATTServerMtuChangedIndication'), ]
        super(GATTServerMtuChangedOperation, self).subscribeToEvents()

class GATTServerHandleValueConfirmationOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GATTServerHandleValueConfirmationIndicationObserver('GATTServerHandleValueConfirmationIndication'), ]
        super(GATTServerHandleValueConfirmationOperation, self).subscribeToEvents()

class GATTServerAttributeWrittenOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GATTServerAttributeWrittenIndicationObserver('GATTServerAttributeWrittenIndication'), ]
        super(GATTServerAttributeWrittenOperation, self).subscribeToEvents()

class GATTServerCharacteristicCccdWrittenOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GATTServerCharacteristicCccdWrittenIndicationObserver('GATTServerCharacteristicCccdWrittenIndication'), ]
        super(GATTServerCharacteristicCccdWrittenOperation, self).subscribeToEvents()

class GATTServerAttributeWrittenWithoutResponseOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GATTServerAttributeWrittenWithoutResponseIndicationObserver('GATTServerAttributeWrittenWithoutResponseIndication'), ]
        super(GATTServerAttributeWrittenWithoutResponseOperation, self).subscribeToEvents()

class GATTServerErrorOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GATTServerErrorIndicationObserver('GATTServerErrorIndication'), ]
        super(GATTServerErrorOperation, self).subscribeToEvents()

class GATTServerLongCharacteristicWrittenOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GATTServerLongCharacteristicWrittenIndicationObserver('GATTServerLongCharacteristicWrittenIndication'), ]
        super(GATTServerLongCharacteristicWrittenOperation, self).subscribeToEvents()

class GATTServerAttributeReadOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GATTServerAttributeReadIndicationObserver('GATTServerAttributeReadIndication'), ]
        super(GATTServerAttributeReadOperation, self).subscribeToEvents()

class GATTDBAttOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GATTDBAttConfirmObserver('GATTDBAttConfirm'), ]
        super(GATTDBAttOperation, self).subscribeToEvents()

class GAPGenericEventInitializationCompleteOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPGenericEventInitializationCompleteIndicationObserver('GAPGenericEventInitializationCompleteIndication'), ]
        super(GAPGenericEventInitializationCompleteOperation, self).subscribeToEvents()

class GAPGenericEventInternalErrorOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPGenericEventInternalErrorIndicationObserver('GAPGenericEventInternalErrorIndication'), ]
        super(GAPGenericEventInternalErrorOperation, self).subscribeToEvents()

class GAPGenericEventAdvertisingSetupFailedOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPGenericEventAdvertisingSetupFailedIndicationObserver('GAPGenericEventAdvertisingSetupFailedIndication'), ]
        super(GAPGenericEventAdvertisingSetupFailedOperation, self).subscribeToEvents()

class GAPGenericEventAdvertisingParametersSetupCompleteOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPGenericEventAdvertisingParametersSetupCompleteIndicationObserver('GAPGenericEventAdvertisingParametersSetupCompleteIndication'), ]
        super(GAPGenericEventAdvertisingParametersSetupCompleteOperation, self).subscribeToEvents()

class GAPGenericEventAdvertisingDataSetupCompleteOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPGenericEventAdvertisingDataSetupCompleteIndicationObserver('GAPGenericEventAdvertisingDataSetupCompleteIndication'), ]
        super(GAPGenericEventAdvertisingDataSetupCompleteOperation, self).subscribeToEvents()

class GAPGenericEventWhiteListSizeReadOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPGenericEventWhiteListSizeReadIndicationObserver('GAPGenericEventWhiteListSizeReadIndication'), ]
        super(GAPGenericEventWhiteListSizeReadOperation, self).subscribeToEvents()

class GAPGenericEventDeviceAddedToWhiteListOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPGenericEventDeviceAddedToWhiteListIndicationObserver('GAPGenericEventDeviceAddedToWhiteListIndication'), ]
        super(GAPGenericEventDeviceAddedToWhiteListOperation, self).subscribeToEvents()

class GAPGenericEventDeviceRemovedFromWhiteListOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPGenericEventDeviceRemovedFromWhiteListIndicationObserver('GAPGenericEventDeviceRemovedFromWhiteListIndication'), ]
        super(GAPGenericEventDeviceRemovedFromWhiteListOperation, self).subscribeToEvents()

class GAPGenericEventWhiteListClearedOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPGenericEventWhiteListClearedIndicationObserver('GAPGenericEventWhiteListClearedIndication'), ]
        super(GAPGenericEventWhiteListClearedOperation, self).subscribeToEvents()

class GAPGenericEventRandomAddressReadyOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPGenericEventRandomAddressReadyIndicationObserver('GAPGenericEventRandomAddressReadyIndication'), ]
        super(GAPGenericEventRandomAddressReadyOperation, self).subscribeToEvents()

class GAPGenericEventCreateConnectionCanceledOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPGenericEventCreateConnectionCanceledIndicationObserver('GAPGenericEventCreateConnectionCanceledIndication'), ]
        super(GAPGenericEventCreateConnectionCanceledOperation, self).subscribeToEvents()

class GAPGenericEventPublicAddressReadOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPGenericEventPublicAddressReadIndicationObserver('GAPGenericEventPublicAddressReadIndication'), ]
        super(GAPGenericEventPublicAddressReadOperation, self).subscribeToEvents()

class GAPGenericEventAdvTxPowerLevelReadOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPGenericEventAdvTxPowerLevelReadIndicationObserver('GAPGenericEventAdvTxPowerLevelReadIndication'), ]
        super(GAPGenericEventAdvTxPowerLevelReadOperation, self).subscribeToEvents()

class GAPGenericEventPrivateResolvableAddressVerifiedOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPGenericEventPrivateResolvableAddressVerifiedIndicationObserver('GAPGenericEventPrivateResolvableAddressVerifiedIndication'), ]
        super(GAPGenericEventPrivateResolvableAddressVerifiedOperation, self).subscribeToEvents()

class GAPGenericEventRandomAddressSetOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPGenericEventRandomAddressSetIndicationObserver('GAPGenericEventRandomAddressSetIndication'), ]
        super(GAPGenericEventRandomAddressSetOperation, self).subscribeToEvents()

class GAPAdvertisingEventStateChangedOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPAdvertisingEventStateChangedIndicationObserver('GAPAdvertisingEventStateChangedIndication'), ]
        super(GAPAdvertisingEventStateChangedOperation, self).subscribeToEvents()

class GAPAdvertisingEventCommandFailedOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPAdvertisingEventCommandFailedIndicationObserver('GAPAdvertisingEventCommandFailedIndication'), ]
        super(GAPAdvertisingEventCommandFailedOperation, self).subscribeToEvents()

class GAPScanningEventStateChangedOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPScanningEventStateChangedIndicationObserver('GAPScanningEventStateChangedIndication'), ]
        super(GAPScanningEventStateChangedOperation, self).subscribeToEvents()

class GAPScanningEventCommandFailedOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPScanningEventCommandFailedIndicationObserver('GAPScanningEventCommandFailedIndication'), ]
        super(GAPScanningEventCommandFailedOperation, self).subscribeToEvents()

class GAPScanningEventDeviceScannedOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPScanningEventDeviceScannedIndicationObserver('GAPScanningEventDeviceScannedIndication'), ]
        super(GAPScanningEventDeviceScannedOperation, self).subscribeToEvents()

class GAPConnectionEventConnectedOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPConnectionEventConnectedIndicationObserver('GAPConnectionEventConnectedIndication'), ]
        super(GAPConnectionEventConnectedOperation, self).subscribeToEvents()

class GAPConnectionEventPairingRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPConnectionEventPairingRequestIndicationObserver('GAPConnectionEventPairingRequestIndication'), ]
        super(GAPConnectionEventPairingRequestOperation, self).subscribeToEvents()

class GAPConnectionEventSlaveSecurityRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPConnectionEventSlaveSecurityRequestIndicationObserver('GAPConnectionEventSlaveSecurityRequestIndication'), ]
        super(GAPConnectionEventSlaveSecurityRequestOperation, self).subscribeToEvents()

class GAPConnectionEventPairingResponseOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPConnectionEventPairingResponseIndicationObserver('GAPConnectionEventPairingResponseIndication'), ]
        super(GAPConnectionEventPairingResponseOperation, self).subscribeToEvents()

class GAPConnectionEventAuthenticationRejectedOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPConnectionEventAuthenticationRejectedIndicationObserver('GAPConnectionEventAuthenticationRejectedIndication'), ]
        super(GAPConnectionEventAuthenticationRejectedOperation, self).subscribeToEvents()

class GAPConnectionEventPasskeyRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPConnectionEventPasskeyRequestIndicationObserver('GAPConnectionEventPasskeyRequestIndication'), ]
        super(GAPConnectionEventPasskeyRequestOperation, self).subscribeToEvents()

class GAPConnectionEventOobRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPConnectionEventOobRequestIndicationObserver('GAPConnectionEventOobRequestIndication'), ]
        super(GAPConnectionEventOobRequestOperation, self).subscribeToEvents()

class GAPConnectionEventPasskeyDisplayOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPConnectionEventPasskeyDisplayIndicationObserver('GAPConnectionEventPasskeyDisplayIndication'), ]
        super(GAPConnectionEventPasskeyDisplayOperation, self).subscribeToEvents()

class GAPConnectionEventKeyExchangeRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPConnectionEventKeyExchangeRequestIndicationObserver('GAPConnectionEventKeyExchangeRequestIndication'), ]
        super(GAPConnectionEventKeyExchangeRequestOperation, self).subscribeToEvents()

class GAPConnectionEventKeysReceivedOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPConnectionEventKeysReceivedIndicationObserver('GAPConnectionEventKeysReceivedIndication'), ]
        super(GAPConnectionEventKeysReceivedOperation, self).subscribeToEvents()

class GAPConnectionEventLongTermKeyRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPConnectionEventLongTermKeyRequestIndicationObserver('GAPConnectionEventLongTermKeyRequestIndication'), ]
        super(GAPConnectionEventLongTermKeyRequestOperation, self).subscribeToEvents()

class GAPConnectionEventEncryptionChangedOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPConnectionEventEncryptionChangedIndicationObserver('GAPConnectionEventEncryptionChangedIndication'), ]
        super(GAPConnectionEventEncryptionChangedOperation, self).subscribeToEvents()

class GAPConnectionEventPairingCompleteOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPConnectionEventPairingCompleteIndicationObserver('GAPConnectionEventPairingCompleteIndication'), ]
        super(GAPConnectionEventPairingCompleteOperation, self).subscribeToEvents()

class GAPConnectionEventDisconnectedOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPConnectionEventDisconnectedIndicationObserver('GAPConnectionEventDisconnectedIndication'), ]
        super(GAPConnectionEventDisconnectedOperation, self).subscribeToEvents()

class GAPConnectionEventRssiReadOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPConnectionEventRssiReadIndicationObserver('GAPConnectionEventRssiReadIndication'), ]
        super(GAPConnectionEventRssiReadOperation, self).subscribeToEvents()

class GAPConnectionEventTxPowerLevelReadOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPConnectionEventTxPowerLevelReadIndicationObserver('GAPConnectionEventTxPowerLevelReadIndication'), ]
        super(GAPConnectionEventTxPowerLevelReadOperation, self).subscribeToEvents()

class GAPConnectionEventPowerReadFailureOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPConnectionEventPowerReadFailureIndicationObserver('GAPConnectionEventPowerReadFailureIndication'), ]
        super(GAPConnectionEventPowerReadFailureOperation, self).subscribeToEvents()

class GAPConnectionEventParameterUpdateRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPConnectionEventParameterUpdateRequestIndicationObserver('GAPConnectionEventParameterUpdateRequestIndication'), ]
        super(GAPConnectionEventParameterUpdateRequestOperation, self).subscribeToEvents()

class GAPConnectionEventParameterUpdateCompleteOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPConnectionEventParameterUpdateCompleteIndicationObserver('GAPConnectionEventParameterUpdateCompleteIndication'), ]
        super(GAPConnectionEventParameterUpdateCompleteOperation, self).subscribeToEvents()

class GAPConnectionEventLeDataLengthChangedOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPConnectionEventLeDataLengthChangedIndicationObserver('GAPConnectionEventLeDataLengthChangedIndication'), ]
        super(GAPConnectionEventLeDataLengthChangedOperation, self).subscribeToEvents()

class GAPConnectionEventLeScOobDataRequestOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPConnectionEventLeScOobDataRequestIndicationObserver('GAPConnectionEventLeScOobDataRequestIndication'), ]
        super(GAPConnectionEventLeScOobDataRequestOperation, self).subscribeToEvents()

class GAPConnectionEventLeScDisplayNumericValueOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPConnectionEventLeScDisplayNumericValueIndicationObserver('GAPConnectionEventLeScDisplayNumericValueIndication'), ]
        super(GAPConnectionEventLeScDisplayNumericValueOperation, self).subscribeToEvents()

class GAPConnectionEventLeScKeypressNotificationOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPConnectionEventLeScKeypressNotificationIndicationObserver('GAPConnectionEventLeScKeypressNotificationIndication'), ]
        super(GAPConnectionEventLeScKeypressNotificationOperation, self).subscribeToEvents()

class GAPGenericEventControllerResetCompleteOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPGenericEventControllerResetCompleteIndicationObserver('GAPGenericEventControllerResetCompleteIndication'), ]
        super(GAPGenericEventControllerResetCompleteOperation, self).subscribeToEvents()

class GAPLeScPublicKeyRegeneratedOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPLeScPublicKeyRegeneratedIndicationObserver('GAPLeScPublicKeyRegeneratedIndication'), ]
        super(GAPLeScPublicKeyRegeneratedOperation, self).subscribeToEvents()

class GAPGenericEventLeScLocalOobDataOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPGenericEventLeScLocalOobDataIndicationObserver('GAPGenericEventLeScLocalOobDataIndication'), ]
        super(GAPGenericEventLeScLocalOobDataOperation, self).subscribeToEvents()

class GAPGenericEventControllerPrivacyStateChangedOperation(FsciOperation):

    def subscribeToEvents(self):
        self.spec = None
        self.observers = [GAPGenericEventControllerPrivacyStateChangedIndicationObserver('GAPGenericEventControllerPrivacyStateChangedIndication'), ]
        super(GAPGenericEventControllerPrivacyStateChangedOperation, self).subscribeToEvents()

def subscribe_to_async_ble_events_from(device, ack_policy=FsciAckPolicy.GLOBAL):
    ble_events = [
        L2CAPConfirmObserver('L2CAPConfirm'),
        GAPConfirmObserver('GAPConfirm'),
        GATTConfirmObserver('GATTConfirm'),
        GATTDBConfirmObserver('GATTDBConfirm'),
        GAPGenericEventInitializationCompleteIndicationObserver('GAPGenericEventInitializationCompleteIndication'),
        GAPAdvertisingEventCommandFailedIndicationObserver('GAPAdvertisingEventCommandFailedIndication'),
        GATTServerErrorIndicationObserver('GATTServerErrorIndication'),
        GATTServerCharacteristicCccdWrittenIndicationObserver('GATTServerCharacteristicCccdWrittenIndication')
    ]

    for ble_event in ble_events:
        FsciFramer(device, ack_policy=FsciAckPolicy.GLOBAL, protocol=Protocol.BLE, baudrate=Baudrate.BR115200).addObserver(ble_event)
