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

from com.nxp.wireless_connectivity.commands.fsci_frame_description import FsciFrameDescription
from com.nxp.wireless_connectivity.commands.fsci_parameter import FsciParameter
from com.nxp.wireless_connectivity.hsdk.singleton import singleton


@singleton
class Spec(object):

    def __init__(self):
        self.HCIModeSelectRequestFrame = self.InitHCIModeSelectRequest()
        self.HCICommandRequestFrame = self.InitHCICommandRequest()
        self.HCIDataRequestFrame = self.InitHCIDataRequest()
        self.HCISynchronousDataRequestFrame = self.InitHCISynchronousDataRequest()
        self.L2CAPModeSelectRequestFrame = self.InitL2CAPModeSelectRequest()
        self.L2CAPInitRequestFrame = self.InitL2CAPInitRequest()
        self.L2CAPConfigRequestFrame = self.InitL2CAPConfigRequest()
        self.L2CAPSendAttDataRequestFrame = self.InitL2CAPSendAttDataRequest()
        self.L2CAPSendSmpDataRequestFrame = self.InitL2CAPSendSmpDataRequest()
        self.L2CAPSendSignalingDataRequestFrame = self.InitL2CAPSendSignalingDataRequest()
        self.L2CAPRegisterAttCallbackRequestFrame = self.InitL2CAPRegisterAttCallbackRequest()
        self.L2CAPRegisterSmpCallbackRequestFrame = self.InitL2CAPRegisterSmpCallbackRequest()
        self.L2CAPRegisterLePsmRequestFrame = self.InitL2CAPRegisterLePsmRequest()
        self.L2CAPDeregisterLePsmRequestFrame = self.InitL2CAPDeregisterLePsmRequest()
        self.L2CAPConnectLePsmRequestFrame = self.InitL2CAPConnectLePsmRequest()
        self.L2CAPDisconnectLeCbChannelRequestFrame = self.InitL2CAPDisconnectLeCbChannelRequest()
        self.L2CAPCancelConnectionRequestFrame = self.InitL2CAPCancelConnectionRequest()
        self.L2CAPSendLeCbDataRequestFrame = self.InitL2CAPSendLeCbDataRequest()
        self.L2CAPSendLeCreditRequestFrame = self.InitL2CAPSendLeCreditRequest()
        self.SMModeSelectRequestFrame = self.InitSMModeSelectRequest()
        self.SMInitRequestFrame = self.InitSMInitRequest()
        self.SMRegisterUserCallbackRequestFrame = self.InitSMRegisterUserCallbackRequest()
        self.SMInitiatorStartPairingRequestFrame = self.InitSMInitiatorStartPairingRequest()
        self.SMSendSecurityRequestRequestFrame = self.InitSMSendSecurityRequestRequest()
        self.SMTerminatePairingRequestFrame = self.InitSMTerminatePairingRequest()
        self.SMPairingRequestReplyRequestFrame = self.InitSMPairingRequestReplyRequest()
        self.SMPasskeyRequestReplyRequestFrame = self.InitSMPasskeyRequestReplyRequest()
        self.SMPairingKeysetRequestReplyRequestFrame = self.InitSMPairingKeysetRequestReplyRequest()
        self.SMLlStartEncryptionRequestFrame = self.InitSMLlStartEncryptionRequest()
        self.SMLlLtkRequestReplyRequestFrame = self.InitSMLlLtkRequestReplyRequest()
        self.SMLlLtkRequestNegativeReplyRequestFrame = self.InitSMLlLtkRequestNegativeReplyRequest()
        self.SMLlEncryptRequestRequestFrame = self.InitSMLlEncryptRequestRequest()
        self.SMLlRandRequestRequestFrame = self.InitSMLlRandRequestRequest()
        self.SMTbCreateRandomDeviceAddressRequestRequestFrame = self.InitSMTbCreateRandomDeviceAddressRequestRequest()
        self.SMTbCheckResolvablePrivateAddressRequestRequestFrame = self.InitSMTbCheckResolvablePrivateAddressRequestRequest()
        self.SMTbSignDataRequestRequestFrame = self.InitSMTbSignDataRequestRequest()
        self.SMTbVerifyDataSignatureRequestRequestFrame = self.InitSMTbVerifyDataSignatureRequestRequest()
        self.SMDeviceConnectNotificationRequestFrame = self.InitSMDeviceConnectNotificationRequest()
        self.SMDeviceDisconnectNotificationRequestFrame = self.InitSMDeviceDisconnectNotificationRequest()
        self.SMSetDefaultPasskeyRequestFrame = self.InitSMSetDefaultPasskeyRequest()
        self.SMSetOobMitmProtectionRequestFrame = self.InitSMSetOobMitmProtectionRequest()
        self.SMSendKeypressNotificationRequestFrame = self.InitSMSendKeypressNotificationRequest()
        self.SMNcDisplayConfirmRequestFrame = self.InitSMNcDisplayConfirmRequest()
        self.SMLeScOobDataRequestReplyRequestFrame = self.InitSMLeScOobDataRequestReplyRequest()
        self.SMLocalLeScOobDataRequestReqRequestFrame = self.InitSMLocalLeScOobDataRequestReqRequest()
        self.SMGenerateNewEcdhPkSkPairRequestFrame = self.InitSMGenerateNewEcdhPkSkPairRequest()
        self.SMSetMinPairingSecurityPropertiesRequestFrame = self.InitSMSetMinPairingSecurityPropertiesRequest()
        self.ATTModeSelectRequestFrame = self.InitATTModeSelectRequest()
        self.ATTInitRequestFrame = self.InitATTInitRequest()
        self.ATTNotifyConnectionRequestFrame = self.InitATTNotifyConnectionRequest()
        self.ATTNotifyDisconnectionRequestFrame = self.InitATTNotifyDisconnectionRequest()
        self.ATTSetMtuRequestFrame = self.InitATTSetMtuRequest()
        self.ATTGetMtuRequestFrame = self.InitATTGetMtuRequest()
        self.ATTRegisterOpcodeCallbackRequestFrame = self.InitATTRegisterOpcodeCallbackRequest()
        self.ATTRegisterUnsupportedOpcodeCallbackRequestFrame = self.InitATTRegisterUnsupportedOpcodeCallbackRequest()
        self.ATTRegisterTimeoutCallbackRequestFrame = self.InitATTRegisterTimeoutCallbackRequest()
        self.ATTServerSendErrorResponseRequestFrame = self.InitATTServerSendErrorResponseRequest()
        self.ATTClientSendExchangeMtuRequestRequestFrame = self.InitATTClientSendExchangeMtuRequestRequest()
        self.ATTServerSendExchangeMtuResponseRequestFrame = self.InitATTServerSendExchangeMtuResponseRequest()
        self.ATTClientSendFindInformationRequestRequestFrame = self.InitATTClientSendFindInformationRequestRequest()
        self.ATTServerSendFindInformationResponseRequestFrame = self.InitATTServerSendFindInformationResponseRequest()
        self.ATTClientSendFindByTypeValueRequestRequestFrame = self.InitATTClientSendFindByTypeValueRequestRequest()
        self.ATTServerSendFindByTypeValueResponseRequestFrame = self.InitATTServerSendFindByTypeValueResponseRequest()
        self.ATTClientSendReadByTypeRequestRequestFrame = self.InitATTClientSendReadByTypeRequestRequest()
        self.ATTServerSendReadByTypeResponseRequestFrame = self.InitATTServerSendReadByTypeResponseRequest()
        self.ATTClientSendReadRequestRequestFrame = self.InitATTClientSendReadRequestRequest()
        self.ATTServerSendReadResponseRequestFrame = self.InitATTServerSendReadResponseRequest()
        self.ATTClientSendReadBlobRequestRequestFrame = self.InitATTClientSendReadBlobRequestRequest()
        self.ATTServerSendReadBlobResponseRequestFrame = self.InitATTServerSendReadBlobResponseRequest()
        self.ATTClientSendReadMultipleRequestRequestFrame = self.InitATTClientSendReadMultipleRequestRequest()
        self.ATTServerSendReadMultipleResponseRequestFrame = self.InitATTServerSendReadMultipleResponseRequest()
        self.ATTClientSendReadByGroupTypeRequestRequestFrame = self.InitATTClientSendReadByGroupTypeRequestRequest()
        self.ATTServerSendReadByGroupTypeResponseRequestFrame = self.InitATTServerSendReadByGroupTypeResponseRequest()
        self.ATTClientSendWriteRequestRequestFrame = self.InitATTClientSendWriteRequestRequest()
        self.ATTServerSendWriteResponseRequestFrame = self.InitATTServerSendWriteResponseRequest()
        self.ATTClientSendWriteCommandRequestFrame = self.InitATTClientSendWriteCommandRequest()
        self.ATTClientSendSignedWriteCommandRequestFrame = self.InitATTClientSendSignedWriteCommandRequest()
        self.ATTClientSendPrepareWriteRequestRequestFrame = self.InitATTClientSendPrepareWriteRequestRequest()
        self.ATTServerSendPrepareWriteResponseRequestFrame = self.InitATTServerSendPrepareWriteResponseRequest()
        self.ATTClientSendExecuteWriteRequestRequestFrame = self.InitATTClientSendExecuteWriteRequestRequest()
        self.ATTServerSendExecuteWriteResponseRequestFrame = self.InitATTServerSendExecuteWriteResponseRequest()
        self.ATTServerSendHandleValueNotificationRequestFrame = self.InitATTServerSendHandleValueNotificationRequest()
        self.ATTServerSendHandleValueIndicationRequestFrame = self.InitATTServerSendHandleValueIndicationRequest()
        self.ATTClientSendHandleValueConfirmationRequestFrame = self.InitATTClientSendHandleValueConfirmationRequest()
        self.GATTModeSelectRequestFrame = self.InitGATTModeSelectRequest()
        self.GATTInitRequestFrame = self.InitGATTInitRequest()
        self.GATTGetMtuRequestFrame = self.InitGATTGetMtuRequest()
        self.GATTClientInitRequestFrame = self.InitGATTClientInitRequest()
        self.GATTClientResetProcedureRequestFrame = self.InitGATTClientResetProcedureRequest()
        self.GATTClientRegisterProcedureCallbackRequestFrame = self.InitGATTClientRegisterProcedureCallbackRequest()
        self.GATTClientRegisterNotificationCallbackRequestFrame = self.InitGATTClientRegisterNotificationCallbackRequest()
        self.GATTClientRegisterIndicationCallbackRequestFrame = self.InitGATTClientRegisterIndicationCallbackRequest()
        self.GATTClientExchangeMtuRequestFrame = self.InitGATTClientExchangeMtuRequest()
        self.GATTClientDiscoverAllPrimaryServicesRequestFrame = self.InitGATTClientDiscoverAllPrimaryServicesRequest()
        self.GATTClientDiscoverPrimaryServicesByUuidRequestFrame = self.InitGATTClientDiscoverPrimaryServicesByUuidRequest()
        self.GATTClientFindIncludedServicesRequestFrame = self.InitGATTClientFindIncludedServicesRequest()
        self.GATTClientDiscoverAllCharacteristicsOfServiceRequestFrame = self.InitGATTClientDiscoverAllCharacteristicsOfServiceRequest()
        self.GATTClientDiscoverCharacteristicOfServiceByUuidRequestFrame = self.InitGATTClientDiscoverCharacteristicOfServiceByUuidRequest()
        self.GATTClientDiscoverAllCharacteristicDescriptorsRequestFrame = self.InitGATTClientDiscoverAllCharacteristicDescriptorsRequest()
        self.GATTClientReadCharacteristicValueRequestFrame = self.InitGATTClientReadCharacteristicValueRequest()
        self.GATTClientReadUsingCharacteristicUuidRequestFrame = self.InitGATTClientReadUsingCharacteristicUuidRequest()
        self.GATTClientReadMultipleCharacteristicValuesRequestFrame = self.InitGATTClientReadMultipleCharacteristicValuesRequest()
        self.GATTClientWriteCharacteristicValueRequestFrame = self.InitGATTClientWriteCharacteristicValueRequest()
        self.GATTClientReadCharacteristicDescriptorRequestFrame = self.InitGATTClientReadCharacteristicDescriptorRequest()
        self.GATTClientWriteCharacteristicDescriptorRequestFrame = self.InitGATTClientWriteCharacteristicDescriptorRequest()
        self.GATTServerInitRequestFrame = self.InitGATTServerInitRequest()
        self.GATTServerRegisterCallbackRequestFrame = self.InitGATTServerRegisterCallbackRequest()
        self.GATTServerRegisterHandlesForWriteNotificationsRequestFrame = self.InitGATTServerRegisterHandlesForWriteNotificationsRequest()
        self.GATTServerSendAttributeWrittenStatusRequestFrame = self.InitGATTServerSendAttributeWrittenStatusRequest()
        self.GATTServerSendNotificationRequestFrame = self.InitGATTServerSendNotificationRequest()
        self.GATTServerSendIndicationRequestFrame = self.InitGATTServerSendIndicationRequest()
        self.GATTServerSendInstantValueNotificationRequestFrame = self.InitGATTServerSendInstantValueNotificationRequest()
        self.GATTServerSendInstantValueIndicationRequestFrame = self.InitGATTServerSendInstantValueIndicationRequest()
        self.GATTServerRegisterHandlesForReadNotificationsRequestFrame = self.InitGATTServerRegisterHandlesForReadNotificationsRequest()
        self.GATTServerSendAttributeReadStatusRequestFrame = self.InitGATTServerSendAttributeReadStatusRequest()
        self.GATTDBModeSelectRequestFrame = self.InitGATTDBModeSelectRequest()
        self.GATTDBInitRequestFrame = self.InitGATTDBInitRequest()
        self.GATTDBWriteAttributeRequestFrame = self.InitGATTDBWriteAttributeRequest()
        self.GATTDBReadAttributeRequestFrame = self.InitGATTDBReadAttributeRequest()
        self.GATTDBFindServiceHandleRequestFrame = self.InitGATTDBFindServiceHandleRequest()
        self.GATTDBFindCharValueHandleInServiceRequestFrame = self.InitGATTDBFindCharValueHandleInServiceRequest()
        self.GATTDBFindCccdHandleForCharValueHandleRequestFrame = self.InitGATTDBFindCccdHandleForCharValueHandleRequest()
        self.GATTDBFindDescriptorHandleForCharValueHandleRequestFrame = self.InitGATTDBFindDescriptorHandleForCharValueHandleRequest()
        self.GATTDBDynamicInitRequestFrame = self.InitGATTDBDynamicInitRequest()
        self.GATTDBDynamicReleaseDatabaseRequestFrame = self.InitGATTDBDynamicReleaseDatabaseRequest()
        self.GATTDBDynamicAddPrimaryServiceDeclarationRequestFrame = self.InitGATTDBDynamicAddPrimaryServiceDeclarationRequest()
        self.GATTDBDynamicAddSecondaryServiceDeclarationRequestFrame = self.InitGATTDBDynamicAddSecondaryServiceDeclarationRequest()
        self.GATTDBDynamicAddIncludeDeclarationRequestFrame = self.InitGATTDBDynamicAddIncludeDeclarationRequest()
        self.GATTDBDynamicAddCharacteristicDeclarationAndValueRequestFrame = self.InitGATTDBDynamicAddCharacteristicDeclarationAndValueRequest()
        self.GATTDBDynamicAddCharacteristicDescriptorRequestFrame = self.InitGATTDBDynamicAddCharacteristicDescriptorRequest()
        self.GATTDBDynamicAddCccdRequestFrame = self.InitGATTDBDynamicAddCccdRequest()
        self.GATTDBDynamicAddCharacteristicDeclarationWithUniqueValueRequestFrame = self.InitGATTDBDynamicAddCharacteristicDeclarationWithUniqueValueRequest()
        self.GATTDBDynamicRemoveServiceRequestFrame = self.InitGATTDBDynamicRemoveServiceRequest()
        self.GATTDBDynamicRemoveCharacteristicRequestFrame = self.InitGATTDBDynamicRemoveCharacteristicRequest()
        self.GATTDBAttModeSelectRequestFrame = self.InitGATTDBAttModeSelectRequest()
        self.GATTDBAttFindInformationRequestFrame = self.InitGATTDBAttFindInformationRequest()
        self.GATTDBAttFindByTypeValueRequestFrame = self.InitGATTDBAttFindByTypeValueRequest()
        self.GATTDBAttReadByTypeRequestFrame = self.InitGATTDBAttReadByTypeRequest()
        self.GATTDBAttReadRequestFrame = self.InitGATTDBAttReadRequest()
        self.GATTDBAttReadBlobRequestFrame = self.InitGATTDBAttReadBlobRequest()
        self.GATTDBAttReadMultipleRequestFrame = self.InitGATTDBAttReadMultipleRequest()
        self.GATTDBAttReadByGroupTypeRequestFrame = self.InitGATTDBAttReadByGroupTypeRequest()
        self.GATTDBAttWriteRequestFrame = self.InitGATTDBAttWriteRequest()
        self.GATTDBAttWriteCommandRequestFrame = self.InitGATTDBAttWriteCommandRequest()
        self.GATTDBAttSignedWriteCommandRequestFrame = self.InitGATTDBAttSignedWriteCommandRequest()
        self.GATTDBAttPrepareWriteRequestFrame = self.InitGATTDBAttPrepareWriteRequest()
        self.GATTDBAttExecuteWriteRequestFrame = self.InitGATTDBAttExecuteWriteRequest()
        self.GATTDBAttExecuteWriteFromQueueRequestFrame = self.InitGATTDBAttExecuteWriteFromQueueRequest()
        self.GATTDBAttPrepareNotificationIndicationRequestFrame = self.InitGATTDBAttPrepareNotificationIndicationRequest()
        self.GAPModeSelectRequestFrame = self.InitGAPModeSelectRequest()
        self.BLEHostInitializeRequestFrame = self.InitBLEHostInitializeRequest()
        self.GAPRegisterDeviceSecurityRequirementsRequestFrame = self.InitGAPRegisterDeviceSecurityRequirementsRequest()
        self.GAPSetAdvertisingParametersRequestFrame = self.InitGAPSetAdvertisingParametersRequest()
        self.GAPSetAdvertisingDataRequestFrame = self.InitGAPSetAdvertisingDataRequest()
        self.GAPStartAdvertisingRequestFrame = self.InitGAPStartAdvertisingRequest()
        self.GAPStopAdvertisingRequestFrame = self.InitGAPStopAdvertisingRequest()
        self.GAPAuthorizeRequestFrame = self.InitGAPAuthorizeRequest()
        self.GAPSaveCccdRequestFrame = self.InitGAPSaveCccdRequest()
        self.GAPCheckNotificationStatusRequestFrame = self.InitGAPCheckNotificationStatusRequest()
        self.GAPCheckIndicationStatusRequestFrame = self.InitGAPCheckIndicationStatusRequest()
        self.GAPGetBondedStaticAddressesRequestFrame = self.InitGAPGetBondedStaticAddressesRequest()
        self.GAPPairRequestFrame = self.InitGAPPairRequest()
        self.GAPSendSlaveSecurityRequestRequestFrame = self.InitGAPSendSlaveSecurityRequestRequest()
        self.GAPEncryptLinkRequestFrame = self.InitGAPEncryptLinkRequest()
        self.GAPAcceptPairingRequestRequestFrame = self.InitGAPAcceptPairingRequestRequest()
        self.GAPRejectPairingRequestFrame = self.InitGAPRejectPairingRequest()
        self.GAPEnterPasskeyRequestFrame = self.InitGAPEnterPasskeyRequest()
        self.GAPProvideOobRequestFrame = self.InitGAPProvideOobRequest()
        self.GAPRejectPasskeyRequestRequestFrame = self.InitGAPRejectPasskeyRequestRequest()
        self.GAPSendSmpKeysRequestFrame = self.InitGAPSendSmpKeysRequest()
        self.GAPRejectKeyExchangeRequestRequestFrame = self.InitGAPRejectKeyExchangeRequestRequest()
        self.GAPProvideLongTermKeyRequestFrame = self.InitGAPProvideLongTermKeyRequest()
        self.GAPDenyLongTermKeyRequestFrame = self.InitGAPDenyLongTermKeyRequest()
        self.GAPLoadEncryptionInformationRequestFrame = self.InitGAPLoadEncryptionInformationRequest()
        self.GAPSetLocalPasskeyRequestFrame = self.InitGAPSetLocalPasskeyRequest()
        self.GAPStartScanningRequestFrame = self.InitGAPStartScanningRequest()
        self.GAPStopScanningRequestFrame = self.InitGAPStopScanningRequest()
        self.GAPConnectRequestFrame = self.InitGAPConnectRequest()
        self.GAPDisconnectRequestFrame = self.InitGAPDisconnectRequest()
        self.GAPSaveCustomPeerInformationRequestFrame = self.InitGAPSaveCustomPeerInformationRequest()
        self.GAPLoadCustomPeerInformationRequestFrame = self.InitGAPLoadCustomPeerInformationRequest()
        self.GAPCheckIfBondedRequestFrame = self.InitGAPCheckIfBondedRequest()
        self.GAPReadWhiteListSizeRequestFrame = self.InitGAPReadWhiteListSizeRequest()
        self.GAPClearWhiteListRequestFrame = self.InitGAPClearWhiteListRequest()
        self.GAPAddDeviceToWhiteListRequestFrame = self.InitGAPAddDeviceToWhiteListRequest()
        self.GAPRemoveDeviceFromWhiteListRequestFrame = self.InitGAPRemoveDeviceFromWhiteListRequest()
        self.GAPReadPublicDeviceAddressRequestFrame = self.InitGAPReadPublicDeviceAddressRequest()
        self.GAPCreateRandomDeviceAddressRequestFrame = self.InitGAPCreateRandomDeviceAddressRequest()
        self.GAPSaveDeviceNameRequestFrame = self.InitGAPSaveDeviceNameRequest()
        self.GAPGetBondedDevicesCountRequestFrame = self.InitGAPGetBondedDevicesCountRequest()
        self.GAPGetBondedDeviceNameRequestFrame = self.InitGAPGetBondedDeviceNameRequest()
        self.GAPRemoveBondRequestFrame = self.InitGAPRemoveBondRequest()
        self.GAPRemoveAllBondsRequestFrame = self.InitGAPRemoveAllBondsRequest()
        self.GAPReadRadioPowerLevelRequestFrame = self.InitGAPReadRadioPowerLevelRequest()
        self.GAPVerifyPrivateResolvableAddressRequestFrame = self.InitGAPVerifyPrivateResolvableAddressRequest()
        self.GAPSetRandomAddressRequestFrame = self.InitGAPSetRandomAddressRequest()
        self.GAPSetScanModeRequestFrame = self.InitGAPSetScanModeRequest()
        self.GAPSetDefaultPairingParametersRequestFrame = self.InitGAPSetDefaultPairingParametersRequest()
        self.GAPUpdateConnectionParametersRequestFrame = self.InitGAPUpdateConnectionParametersRequest()
        self.GAPEnableUpdateConnectionParametersRequestFrame = self.InitGAPEnableUpdateConnectionParametersRequest()
        self.GapUpdateLeDataLengthRequestFrame = self.InitGapUpdateLeDataLengthRequest()
        self.GAPControllerResetRequestFrame = self.InitGAPControllerResetRequest()
        self.GAPEnableHostPrivacyRequestFrame = self.InitGAPEnableHostPrivacyRequest()
        self.GAPEnableControllerPrivacyRequestFrame = self.InitGAPEnableControllerPrivacyRequest()
        self.GAPLeScRegeneratePublicKeyRequestFrame = self.InitGAPLeScRegeneratePublicKeyRequest()
        self.GAPLeScValidateNumericValueRequestFrame = self.InitGAPLeScValidateNumericValueRequest()
        self.GAPLeScGetLocalOobDataRequestFrame = self.InitGAPLeScGetLocalOobDataRequest()
        self.GAPLeScSetPeerOobDataRequestFrame = self.InitGAPLeScSetPeerOobDataRequest()
        self.GAPLeScSendKeypressNotificationPrivacyRequestFrame = self.InitGAPLeScSendKeypressNotificationPrivacyRequest()
        self.GAPGetBondedDevicesIdentityInformationRequestFrame = self.InitGAPGetBondedDevicesIdentityInformationRequest()
        self.GAPSetTxPowerLevelRequestFrame = self.InitGAPSetTxPowerLevelRequest()
        self.FSCICPUResetRequestFrame = self.InitFSCICPUResetRequest()
        self.FSCIAllowDeviceToSleepRequestFrame = self.InitFSCIAllowDeviceToSleepRequest()
        self.FSCIGetWakeupReasonRequestFrame = self.InitFSCIGetWakeupReasonRequest()
        self.FSCIAckIndicationFrame = self.InitFSCIAckIndication()
        self.FSCIErrorIndicationFrame = self.InitFSCIErrorIndication()
        self.FSCIAllowDeviceToSleepConfirmFrame = self.InitFSCIAllowDeviceToSleepConfirm()
        self.FSCIWakeUpIndicationFrame = self.InitFSCIWakeUpIndication()
        self.FSCIGetWakeupReasonResponseFrame = self.InitFSCIGetWakeupReasonResponse()
        self.NVMSaveConfirmFrame = self.InitNVMSaveConfirm()
        self.NVMGetDataSetDescConfirmFrame = self.InitNVMGetDataSetDescConfirm()
        self.NVMGetCountersConfirmFrame = self.InitNVMGetCountersConfirm()
        self.NVMSetMonitoringConfirmFrame = self.InitNVMSetMonitoringConfirm()
        self.NVMWriteMonitoringIndicationFrame = self.InitNVMWriteMonitoringIndication()
        self.NVMPageEraseMonitoringIndicationFrame = self.InitNVMPageEraseMonitoringIndication()
        self.NVMFormatReqConfirmFrame = self.InitNVMFormatReqConfirm()
        self.NVMRestoreReqConfirmFrame = self.InitNVMRestoreReqConfirm()
        self.NVMRestoreMonitoringIndicationFrame = self.InitNVMRestoreMonitoringIndication()
        self.NVMVirtualPageMonitoringIndicationFrame = self.InitNVMVirtualPageMonitoringIndication()
        self.HCIConfirmFrame = self.InitHCIConfirm()
        self.HCIEventIndicationFrame = self.InitHCIEventIndication()
        self.HCIDataIndicationFrame = self.InitHCIDataIndication()
        self.HCISynchronousDataIndicationFrame = self.InitHCISynchronousDataIndication()
        self.L2CAPConfirmFrame = self.InitL2CAPConfirm()
        self.L2CAPAttDataIndicationFrame = self.InitL2CAPAttDataIndication()
        self.L2CAPSmpDataIndicationFrame = self.InitL2CAPSmpDataIndication()
        self.L2CAPSignalingDataIndicationFrame = self.InitL2CAPSignalingDataIndication()
        self.L2CAPLePsmConnectionRequestIndicationFrame = self.InitL2CAPLePsmConnectionRequestIndication()
        self.L2CAPLePsmConnectionCompleteIndicationFrame = self.InitL2CAPLePsmConnectionCompleteIndication()
        self.L2CAPLePsmDisconnectNotificationIndicationFrame = self.InitL2CAPLePsmDisconnectNotificationIndication()
        self.L2CAPNoPeerCreditsIndicationFrame = self.InitL2CAPNoPeerCreditsIndication()
        self.L2CAPLocalCreditsNotificationIndicationFrame = self.InitL2CAPLocalCreditsNotificationIndication()
        self.L2CAPLeCbDataIndicationFrame = self.InitL2CAPLeCbDataIndication()
        self.SMConfirmFrame = self.InitSMConfirm()
        self.SMStatusIndicationFrame = self.InitSMStatusIndication()
        self.SMRemoteSecurityRequestIndicationFrame = self.InitSMRemoteSecurityRequestIndication()
        self.SMRemotePairingRequestIndicationFrame = self.InitSMRemotePairingRequestIndication()
        self.SMRemotePairingResponseIndicationFrame = self.InitSMRemotePairingResponseIndication()
        self.SMPasskeyDisplayRequestIndicationFrame = self.InitSMPasskeyDisplayRequestIndication()
        self.SMPasskeyRequestIndicationFrame = self.InitSMPasskeyRequestIndication()
        self.SMPairingKeysetRequestIndicationFrame = self.InitSMPairingKeysetRequestIndication()
        self.SMPairingKeysetReceivedIndicationFrame = self.InitSMPairingKeysetReceivedIndication()
        self.SMPairingCompleteIndicationFrame = self.InitSMPairingCompleteIndication()
        self.SMPairingFailedIndicationFrame = self.InitSMPairingFailedIndication()
        self.SMReceivedPairingFailedIndicationFrame = self.InitSMReceivedPairingFailedIndication()
        self.SMLlLtkRequestIndicationFrame = self.InitSMLlLtkRequestIndication()
        self.SMLlEncryptionStatusIndicationFrame = self.InitSMLlEncryptionStatusIndication()
        self.SMLlEncryptResIndicationFrame = self.InitSMLlEncryptResIndication()
        self.SMLlRandResIndicationFrame = self.InitSMLlRandResIndication()
        self.SMTbCreateRandomDeviceAddrResIndicationFrame = self.InitSMTbCreateRandomDeviceAddrResIndication()
        self.SMTbCheckResolvalePrivateAddrResIndicationFrame = self.InitSMTbCheckResolvalePrivateAddrResIndication()
        self.SMTbSignDataResIndicationFrame = self.InitSMTbSignDataResIndication()
        self.SMTbVerifyDataSignatureResIndicationFrame = self.InitSMTbVerifyDataSignatureResIndication()
        self.SMRemoteKeypressNotificationIndicationFrame = self.InitSMRemoteKeypressNotificationIndication()
        self.SMNcDisplayRequestIndicationFrame = self.InitSMNcDisplayRequestIndication()
        self.SMLeScOobDataRequestIndicationFrame = self.InitSMLeScOobDataRequestIndication()
        self.SMLocalLeScOobDataIndicationFrame = self.InitSMLocalLeScOobDataIndication()
        self.SMGenerateNewEcdhPkSkPairResIndicationFrame = self.InitSMGenerateNewEcdhPkSkPairResIndication()
        self.ATTConfirmFrame = self.InitATTConfirm()
        self.ATTGetMtuIndicationFrame = self.InitATTGetMtuIndication()
        self.ATTClientIncomingServerErrorResponseIndicationFrame = self.InitATTClientIncomingServerErrorResponseIndication()
        self.ATTServerIncomingClientExchangeMtuRequestIndicationFrame = self.InitATTServerIncomingClientExchangeMtuRequestIndication()
        self.ATTClientIncomingServerExchangeMtuResponseIndicationFrame = self.InitATTClientIncomingServerExchangeMtuResponseIndication()
        self.ATTServerIncomingClientFindInformationRequestIndicationFrame = self.InitATTServerIncomingClientFindInformationRequestIndication()
        self.ATTClientIncomingServerFindInformationResponseIndicationFrame = self.InitATTClientIncomingServerFindInformationResponseIndication()
        self.ATTServerIncomingClientFindByTypeValueRequestIndicationFrame = self.InitATTServerIncomingClientFindByTypeValueRequestIndication()
        self.ATTClientIncomingServerFindByTypeValueResponseIndicationFrame = self.InitATTClientIncomingServerFindByTypeValueResponseIndication()
        self.ATTServerIncomingClientReadByTypeRequestIndicationFrame = self.InitATTServerIncomingClientReadByTypeRequestIndication()
        self.ATTClientIncomingServerReadByTypeResponseIndicationFrame = self.InitATTClientIncomingServerReadByTypeResponseIndication()
        self.ATTServerIncomingClientReadRequestIndicationFrame = self.InitATTServerIncomingClientReadRequestIndication()
        self.ATTClientIncomingServerReadResponseIndicationFrame = self.InitATTClientIncomingServerReadResponseIndication()
        self.ATTServerIncomingClientReadBlobRequestIndicationFrame = self.InitATTServerIncomingClientReadBlobRequestIndication()
        self.ATTClientIncomingServerReadBlobResponseIndicationFrame = self.InitATTClientIncomingServerReadBlobResponseIndication()
        self.ATTServerIncomingClientReadMultipleRequestIndicationFrame = self.InitATTServerIncomingClientReadMultipleRequestIndication()
        self.ATTClientIncomingServerReadMultipleResponseIndicationFrame = self.InitATTClientIncomingServerReadMultipleResponseIndication()
        self.ATTServerIncomingClientReadByGroupTypeRequestIndicationFrame = self.InitATTServerIncomingClientReadByGroupTypeRequestIndication()
        self.ATTClientIncomingServerReadByGroupTypeResponseIndicationFrame = self.InitATTClientIncomingServerReadByGroupTypeResponseIndication()
        self.ATTServerIncomingClientWriteRequestIndicationFrame = self.InitATTServerIncomingClientWriteRequestIndication()
        self.ATTClientIncomingServerWriteResponseIndicationFrame = self.InitATTClientIncomingServerWriteResponseIndication()
        self.ATTServerIncomingClientWriteCommandIndicationFrame = self.InitATTServerIncomingClientWriteCommandIndication()
        self.ATTServerIncomingClientSignedWriteCommandIndicationFrame = self.InitATTServerIncomingClientSignedWriteCommandIndication()
        self.ATTServerIncomingClientPrepareWriteRequestIndicationFrame = self.InitATTServerIncomingClientPrepareWriteRequestIndication()
        self.ATTClientIncomingServerPrepareWriteResponseIndicationFrame = self.InitATTClientIncomingServerPrepareWriteResponseIndication()
        self.ATTServerIncomingClientExecuteWriteRequestIndicationFrame = self.InitATTServerIncomingClientExecuteWriteRequestIndication()
        self.ATTClientIncomingServerExecuteWriteResponseIndicationFrame = self.InitATTClientIncomingServerExecuteWriteResponseIndication()
        self.ATTClientIncomingServerHandleValueNotificationIndicationFrame = self.InitATTClientIncomingServerHandleValueNotificationIndication()
        self.ATTClientIncomingServerHandleValueIndicationIndicationFrame = self.InitATTClientIncomingServerHandleValueIndicationIndication()
        self.ATTServerIncomingClientHandleValueConfirmationIndicationFrame = self.InitATTServerIncomingClientHandleValueConfirmationIndication()
        self.ATTUnsupportedOpcodeIndicationFrame = self.InitATTUnsupportedOpcodeIndication()
        self.ATTTimeoutIndicationFrame = self.InitATTTimeoutIndication()
        self.GATTConfirmFrame = self.InitGATTConfirm()
        self.GATTGetMtuIndicationFrame = self.InitGATTGetMtuIndication()
        self.GATTClientProcedureExchangeMtuIndicationFrame = self.InitGATTClientProcedureExchangeMtuIndication()
        self.GATTClientProcedureDiscoverAllPrimaryServicesIndicationFrame = self.InitGATTClientProcedureDiscoverAllPrimaryServicesIndication()
        self.GATTClientProcedureDiscoverPrimaryServicesByUuidIndicationFrame = self.InitGATTClientProcedureDiscoverPrimaryServicesByUuidIndication()
        self.GATTClientProcedureFindIncludedServicesIndicationFrame = self.InitGATTClientProcedureFindIncludedServicesIndication()
        self.GATTClientProcedureDiscoverAllCharacteristicsIndicationFrame = self.InitGATTClientProcedureDiscoverAllCharacteristicsIndication()
        self.GATTClientProcedureDiscoverCharacteristicByUuidIndicationFrame = self.InitGATTClientProcedureDiscoverCharacteristicByUuidIndication()
        self.GATTClientProcedureDiscoverAllCharacteristicDescriptorsIndicationFrame = self.InitGATTClientProcedureDiscoverAllCharacteristicDescriptorsIndication()
        self.GATTClientProcedureReadCharacteristicValueIndicationFrame = self.InitGATTClientProcedureReadCharacteristicValueIndication()
        self.GATTClientProcedureReadUsingCharacteristicUuidIndicationFrame = self.InitGATTClientProcedureReadUsingCharacteristicUuidIndication()
        self.GATTClientProcedureReadMultipleCharacteristicValuesIndicationFrame = self.InitGATTClientProcedureReadMultipleCharacteristicValuesIndication()
        self.GATTClientProcedureWriteCharacteristicValueIndicationFrame = self.InitGATTClientProcedureWriteCharacteristicValueIndication()
        self.GATTClientProcedureReadCharacteristicDescriptorIndicationFrame = self.InitGATTClientProcedureReadCharacteristicDescriptorIndication()
        self.GATTClientProcedureWriteCharacteristicDescriptorIndicationFrame = self.InitGATTClientProcedureWriteCharacteristicDescriptorIndication()
        self.GATTClientNotificationIndicationFrame = self.InitGATTClientNotificationIndication()
        self.GATTClientIndicationIndicationFrame = self.InitGATTClientIndicationIndication()
        self.GATTServerMtuChangedIndicationFrame = self.InitGATTServerMtuChangedIndication()
        self.GATTServerHandleValueConfirmationIndicationFrame = self.InitGATTServerHandleValueConfirmationIndication()
        self.GATTServerAttributeWrittenIndicationFrame = self.InitGATTServerAttributeWrittenIndication()
        self.GATTServerCharacteristicCccdWrittenIndicationFrame = self.InitGATTServerCharacteristicCccdWrittenIndication()
        self.GATTServerAttributeWrittenWithoutResponseIndicationFrame = self.InitGATTServerAttributeWrittenWithoutResponseIndication()
        self.GATTServerErrorIndicationFrame = self.InitGATTServerErrorIndication()
        self.GATTServerLongCharacteristicWrittenIndicationFrame = self.InitGATTServerLongCharacteristicWrittenIndication()
        self.GATTServerAttributeReadIndicationFrame = self.InitGATTServerAttributeReadIndication()
        self.GATTDBConfirmFrame = self.InitGATTDBConfirm()
        self.GATTDBReadAttributeIndicationFrame = self.InitGATTDBReadAttributeIndication()
        self.GATTDBFindServiceHandleIndicationFrame = self.InitGATTDBFindServiceHandleIndication()
        self.GATTDBFindCharValueHandleInServiceIndicationFrame = self.InitGATTDBFindCharValueHandleInServiceIndication()
        self.GATTDBFindCccdHandleForCharValueHandleIndicationFrame = self.InitGATTDBFindCccdHandleForCharValueHandleIndication()
        self.GATTDBFindDescriptorHandleForCharValueHandleIndicationFrame = self.InitGATTDBFindDescriptorHandleForCharValueHandleIndication()
        self.GATTDBDynamicAddPrimaryServiceDeclarationIndicationFrame = self.InitGATTDBDynamicAddPrimaryServiceDeclarationIndication()
        self.GATTDBDynamicAddSecondaryServiceDeclarationIndicationFrame = self.InitGATTDBDynamicAddSecondaryServiceDeclarationIndication()
        self.GATTDBDynamicAddIncludeDeclarationIndicationFrame = self.InitGATTDBDynamicAddIncludeDeclarationIndication()
        self.GATTDBDynamicAddCharacteristicDeclarationAndValueIndicationFrame = self.InitGATTDBDynamicAddCharacteristicDeclarationAndValueIndication()
        self.GATTDBDynamicAddCharacteristicDescriptorIndicationFrame = self.InitGATTDBDynamicAddCharacteristicDescriptorIndication()
        self.GATTDBDynamicAddCccdIndicationFrame = self.InitGATTDBDynamicAddCccdIndication()
        self.GATTDBDynamicAddCharacteristicDeclarationWithUniqueValueIndicationFrame = self.InitGATTDBDynamicAddCharacteristicDeclarationWithUniqueValueIndication()
        self.GATTDBAttConfirmFrame = self.InitGATTDBAttConfirm()
        self.GATTDBAttFindInformationIndicationFrame = self.InitGATTDBAttFindInformationIndication()
        self.GATTDBAttFindByTypeValueIndicationFrame = self.InitGATTDBAttFindByTypeValueIndication()
        self.GATTDBAttReadByTypeIndicationFrame = self.InitGATTDBAttReadByTypeIndication()
        self.GATTDBAttReadIndicationFrame = self.InitGATTDBAttReadIndication()
        self.GATTDBAttReadBlobIndicationFrame = self.InitGATTDBAttReadBlobIndication()
        self.GATTDBAttReadMultipleIndicationFrame = self.InitGATTDBAttReadMultipleIndication()
        self.GATTDBAttReadByGroupTypeIndicationFrame = self.InitGATTDBAttReadByGroupTypeIndication()
        self.GATTDBAttWriteIndicationFrame = self.InitGATTDBAttWriteIndication()
        self.GATTDBAttPrepareWriteIndicationFrame = self.InitGATTDBAttPrepareWriteIndication()
        self.GATTDBAttExecuteWriteIndicationFrame = self.InitGATTDBAttExecuteWriteIndication()
        self.GATTDBAttExecuteWriteFromQueueIndicationFrame = self.InitGATTDBAttExecuteWriteFromQueueIndication()
        self.GATTDBAttPrepareNotificationIndicationIndicationFrame = self.InitGATTDBAttPrepareNotificationIndicationIndication()
        self.GAPConfirmFrame = self.InitGAPConfirm()
        self.GAPCheckNotificationStatusIndicationFrame = self.InitGAPCheckNotificationStatusIndication()
        self.GAPCheckIndicationStatusIndicationFrame = self.InitGAPCheckIndicationStatusIndication()
        self.GAPGetBondedStaticAddressesIndicationFrame = self.InitGAPGetBondedStaticAddressesIndication()
        self.GAPLoadEncryptionInformationIndicationFrame = self.InitGAPLoadEncryptionInformationIndication()
        self.GAPLoadCustomPeerInformationIndicationFrame = self.InitGAPLoadCustomPeerInformationIndication()
        self.GAPCheckIfBondedIndicationFrame = self.InitGAPCheckIfBondedIndication()
        self.GAPGetBondedDevicesCountIndicationFrame = self.InitGAPGetBondedDevicesCountIndication()
        self.GAPGetBondedDeviceNameIndicationFrame = self.InitGAPGetBondedDeviceNameIndication()
        self.GAPGenericEventInitializationCompleteIndicationFrame = self.InitGAPGenericEventInitializationCompleteIndication()
        self.GAPGenericEventInternalErrorIndicationFrame = self.InitGAPGenericEventInternalErrorIndication()
        self.GAPGenericEventAdvertisingSetupFailedIndicationFrame = self.InitGAPGenericEventAdvertisingSetupFailedIndication()
        self.GAPGenericEventAdvertisingParametersSetupCompleteIndicationFrame = self.InitGAPGenericEventAdvertisingParametersSetupCompleteIndication()
        self.GAPGenericEventAdvertisingDataSetupCompleteIndicationFrame = self.InitGAPGenericEventAdvertisingDataSetupCompleteIndication()
        self.GAPGenericEventWhiteListSizeReadIndicationFrame = self.InitGAPGenericEventWhiteListSizeReadIndication()
        self.GAPGenericEventDeviceAddedToWhiteListIndicationFrame = self.InitGAPGenericEventDeviceAddedToWhiteListIndication()
        self.GAPGenericEventDeviceRemovedFromWhiteListIndicationFrame = self.InitGAPGenericEventDeviceRemovedFromWhiteListIndication()
        self.GAPGenericEventWhiteListClearedIndicationFrame = self.InitGAPGenericEventWhiteListClearedIndication()
        self.GAPGenericEventRandomAddressReadyIndicationFrame = self.InitGAPGenericEventRandomAddressReadyIndication()
        self.GAPGenericEventCreateConnectionCanceledIndicationFrame = self.InitGAPGenericEventCreateConnectionCanceledIndication()
        self.GAPGenericEventPublicAddressReadIndicationFrame = self.InitGAPGenericEventPublicAddressReadIndication()
        self.GAPGenericEventAdvTxPowerLevelReadIndicationFrame = self.InitGAPGenericEventAdvTxPowerLevelReadIndication()
        self.GAPGenericEventPrivateResolvableAddressVerifiedIndicationFrame = self.InitGAPGenericEventPrivateResolvableAddressVerifiedIndication()
        self.GAPGenericEventRandomAddressSetIndicationFrame = self.InitGAPGenericEventRandomAddressSetIndication()
        self.GAPAdvertisingEventStateChangedIndicationFrame = self.InitGAPAdvertisingEventStateChangedIndication()
        self.GAPAdvertisingEventCommandFailedIndicationFrame = self.InitGAPAdvertisingEventCommandFailedIndication()
        self.GAPScanningEventStateChangedIndicationFrame = self.InitGAPScanningEventStateChangedIndication()
        self.GAPScanningEventCommandFailedIndicationFrame = self.InitGAPScanningEventCommandFailedIndication()
        self.GAPScanningEventDeviceScannedIndicationFrame = self.InitGAPScanningEventDeviceScannedIndication()
        self.GAPConnectionEventConnectedIndicationFrame = self.InitGAPConnectionEventConnectedIndication()
        self.GAPConnectionEventPairingRequestIndicationFrame = self.InitGAPConnectionEventPairingRequestIndication()
        self.GAPConnectionEventSlaveSecurityRequestIndicationFrame = self.InitGAPConnectionEventSlaveSecurityRequestIndication()
        self.GAPConnectionEventPairingResponseIndicationFrame = self.InitGAPConnectionEventPairingResponseIndication()
        self.GAPConnectionEventAuthenticationRejectedIndicationFrame = self.InitGAPConnectionEventAuthenticationRejectedIndication()
        self.GAPConnectionEventPasskeyRequestIndicationFrame = self.InitGAPConnectionEventPasskeyRequestIndication()
        self.GAPConnectionEventOobRequestIndicationFrame = self.InitGAPConnectionEventOobRequestIndication()
        self.GAPConnectionEventPasskeyDisplayIndicationFrame = self.InitGAPConnectionEventPasskeyDisplayIndication()
        self.GAPConnectionEventKeyExchangeRequestIndicationFrame = self.InitGAPConnectionEventKeyExchangeRequestIndication()
        self.GAPConnectionEventKeysReceivedIndicationFrame = self.InitGAPConnectionEventKeysReceivedIndication()
        self.GAPConnectionEventLongTermKeyRequestIndicationFrame = self.InitGAPConnectionEventLongTermKeyRequestIndication()
        self.GAPConnectionEventEncryptionChangedIndicationFrame = self.InitGAPConnectionEventEncryptionChangedIndication()
        self.GAPConnectionEventPairingCompleteIndicationFrame = self.InitGAPConnectionEventPairingCompleteIndication()
        self.GAPConnectionEventDisconnectedIndicationFrame = self.InitGAPConnectionEventDisconnectedIndication()
        self.GAPConnectionEventRssiReadIndicationFrame = self.InitGAPConnectionEventRssiReadIndication()
        self.GAPConnectionEventTxPowerLevelReadIndicationFrame = self.InitGAPConnectionEventTxPowerLevelReadIndication()
        self.GAPConnectionEventPowerReadFailureIndicationFrame = self.InitGAPConnectionEventPowerReadFailureIndication()
        self.GAPConnectionEventParameterUpdateRequestIndicationFrame = self.InitGAPConnectionEventParameterUpdateRequestIndication()
        self.GAPConnectionEventParameterUpdateCompleteIndicationFrame = self.InitGAPConnectionEventParameterUpdateCompleteIndication()
        self.GAPConnectionEventLeDataLengthChangedIndicationFrame = self.InitGAPConnectionEventLeDataLengthChangedIndication()
        self.GAPConnectionEventLeScOobDataRequestIndicationFrame = self.InitGAPConnectionEventLeScOobDataRequestIndication()
        self.GAPConnectionEventLeScDisplayNumericValueIndicationFrame = self.InitGAPConnectionEventLeScDisplayNumericValueIndication()
        self.GAPConnectionEventLeScKeypressNotificationIndicationFrame = self.InitGAPConnectionEventLeScKeypressNotificationIndication()
        self.GAPGenericEventControllerResetCompleteIndicationFrame = self.InitGAPGenericEventControllerResetCompleteIndication()
        self.GAPLeScPublicKeyRegeneratedIndicationFrame = self.InitGAPLeScPublicKeyRegeneratedIndication()
        self.GAPGenericEventLeScLocalOobDataIndicationFrame = self.InitGAPGenericEventLeScLocalOobDataIndication()
        self.GAPGenericEventControllerPrivacyStateChangedIndicationFrame = self.InitGAPGenericEventControllerPrivacyStateChangedIndication()
        self.GAPGenericEventTxPowerLevelSetCompleteIndicationFrame = self.InitGAPGenericEventTxPowerLevelSetCompleteIndication()
        self.GAPGetBondedDevicesIdentityInformationIndicationFrame = self.InitGAPGetBondedDevicesIdentityInformationIndication()

    def InitHCIModeSelectRequest(self):
        cmdParams = []
        Enable = FsciParameter("Enable", 1)
        cmdParams.append(Enable)
        return FsciFrameDescription(0x40, 0x00, cmdParams)

    def InitHCICommandRequest(self):
        cmdParams = []
        CommandLength = FsciParameter("CommandLength", 2)
        cmdParams.append(CommandLength)
        Command = FsciParameter("Command", 1, CommandLength)
        cmdParams.append(Command)
        return FsciFrameDescription(0x40, 0x01, cmdParams)

    def InitHCIDataRequest(self):
        cmdParams = []
        DataLength = FsciParameter("DataLength", 2)
        cmdParams.append(DataLength)
        Data = FsciParameter("Data", 1, DataLength)
        cmdParams.append(Data)
        return FsciFrameDescription(0x40, 0x02, cmdParams)

    def InitHCISynchronousDataRequest(self):
        cmdParams = []
        SynchronousDataLength = FsciParameter("SynchronousDataLength", 2)
        cmdParams.append(SynchronousDataLength)
        SynchronousData = FsciParameter("SynchronousData", 1, SynchronousDataLength)
        cmdParams.append(SynchronousData)
        return FsciFrameDescription(0x40, 0x03, cmdParams)

    def InitL2CAPModeSelectRequest(self):
        cmdParams = []
        Enable = FsciParameter("Enable", 1)
        cmdParams.append(Enable)
        return FsciFrameDescription(0x41, 0x00, cmdParams)

    def InitL2CAPInitRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x41, 0x01, cmdParams)

    def InitL2CAPConfigRequest(self):
        cmdParams = []
        ConfigStruct_LeFeatures = FsciParameter("ConfigStruct_LeFeatures", 4)
        cmdParams.append(ConfigStruct_LeFeatures)
        ConfigStruct_HCILeBufferSize = FsciParameter("ConfigStruct_HCILeBufferSize", 4)
        cmdParams.append(ConfigStruct_HCILeBufferSize)
        ConfigStruct_MaxPduPayloadSize = FsciParameter("ConfigStruct_MaxPduPayloadSize", 2)
        cmdParams.append(ConfigStruct_MaxPduPayloadSize)
        return FsciFrameDescription(0x41, 0x02, cmdParams)

    def InitL2CAPSendAttDataRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        PacketLength = FsciParameter("PacketLength", 2)
        cmdParams.append(PacketLength)
        Packet = FsciParameter("Packet", 1, PacketLength)
        cmdParams.append(Packet)
        return FsciFrameDescription(0x41, 0x03, cmdParams)

    def InitL2CAPSendSmpDataRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        PacketLength = FsciParameter("PacketLength", 2)
        cmdParams.append(PacketLength)
        Packet = FsciParameter("Packet", 1, PacketLength)
        cmdParams.append(Packet)
        return FsciFrameDescription(0x41, 0x04, cmdParams)

    def InitL2CAPSendSignalingDataRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        PacketLength = FsciParameter("PacketLength", 2)
        cmdParams.append(PacketLength)
        Packet = FsciParameter("Packet", 1, PacketLength)
        cmdParams.append(Packet)
        return FsciFrameDescription(0x41, 0x05, cmdParams)

    def InitL2CAPRegisterAttCallbackRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x41, 0x06, cmdParams)

    def InitL2CAPRegisterSmpCallbackRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x41, 0x07, cmdParams)

    def InitL2CAPRegisterLePsmRequest(self):
        cmdParams = []
        LePsm = FsciParameter("LePsm", 2)
        cmdParams.append(LePsm)
        LePsmMtu = FsciParameter("LePsmMtu", 2)
        cmdParams.append(LePsmMtu)
        return FsciFrameDescription(0x41, 0x08, cmdParams)

    def InitL2CAPDeregisterLePsmRequest(self):
        cmdParams = []
        LePsm = FsciParameter("LePsm", 2)
        cmdParams.append(LePsm)
        return FsciFrameDescription(0x41, 0x09, cmdParams)

    def InitL2CAPConnectLePsmRequest(self):
        cmdParams = []
        LePsm = FsciParameter("LePsm", 2)
        cmdParams.append(LePsm)
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        InitialCredits = FsciParameter("InitialCredits", 2)
        cmdParams.append(InitialCredits)
        return FsciFrameDescription(0x41, 0x0A, cmdParams)

    def InitL2CAPDisconnectLeCbChannelRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        ChannelId = FsciParameter("ChannelId", 2)
        cmdParams.append(ChannelId)
        return FsciFrameDescription(0x41, 0x0B, cmdParams)

    def InitL2CAPCancelConnectionRequest(self):
        cmdParams = []
        LePsm = FsciParameter("LePsm", 2)
        cmdParams.append(LePsm)
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        RefuseReason = FsciParameter("RefuseReason", 2)
        cmdParams.append(RefuseReason)
        return FsciFrameDescription(0x41, 0x0C, cmdParams)

    def InitL2CAPSendLeCbDataRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        ChannelId = FsciParameter("ChannelId", 2)
        cmdParams.append(ChannelId)
        PacketLength = FsciParameter("PacketLength", 2)
        cmdParams.append(PacketLength)
        Packet = FsciParameter("Packet", 1, PacketLength)
        cmdParams.append(Packet)
        return FsciFrameDescription(0x41, 0x0D, cmdParams)

    def InitL2CAPSendLeCreditRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        ChannelId = FsciParameter("ChannelId", 2)
        cmdParams.append(ChannelId)
        Credits = FsciParameter("Credits", 2)
        cmdParams.append(Credits)
        return FsciFrameDescription(0x41, 0x0E, cmdParams)

    def InitSMModeSelectRequest(self):
        cmdParams = []
        Enable = FsciParameter("Enable", 1)
        cmdParams.append(Enable)
        return FsciFrameDescription(0x42, 0x00, cmdParams)

    def InitSMInitRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x42, 0x01, cmdParams)

    def InitSMRegisterUserCallbackRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x42, 0x02, cmdParams)

    def InitSMInitiatorStartPairingRequest(self):
        cmdParams = []
        DestinationDeviceId = FsciParameter("DestinationDeviceId", 1)
        cmdParams.append(DestinationDeviceId)
        SmPairingParams_IoCapabilities = FsciParameter("SmPairingParams_IoCapabilities", 1)
        cmdParams.append(SmPairingParams_IoCapabilities)
        SmPairingParams_OobDataFlag = FsciParameter("SmPairingParams_OobDataFlag", 1)
        cmdParams.append(SmPairingParams_OobDataFlag)
        SmPairingParams_AuthRequestBondingFlags = FsciParameter("SmPairingParams_AuthRequestBondingFlags", 1)
        cmdParams.append(SmPairingParams_AuthRequestBondingFlags)
        SmPairingParams_AuthRequestMitm = FsciParameter("SmPairingParams_AuthRequestMitm", 1)
        cmdParams.append(SmPairingParams_AuthRequestMitm)
        SmPairingParams_AuthRequestsc = FsciParameter("SmPairingParams_AuthRequestsc", 1)
        cmdParams.append(SmPairingParams_AuthRequestsc)
        SmPairingParams_AuthRequestkeypress = FsciParameter("SmPairingParams_AuthRequestkeypress", 1)
        cmdParams.append(SmPairingParams_AuthRequestkeypress)
        SmPairingParams_MaxEncKeySize = FsciParameter("SmPairingParams_MaxEncKeySize", 1)
        cmdParams.append(SmPairingParams_MaxEncKeySize)
        SmPairingParams_InitatorKeyDistributionEncKey = FsciParameter("SmPairingParams_InitatorKeyDistributionEncKey", 1)
        cmdParams.append(SmPairingParams_InitatorKeyDistributionEncKey)
        SmPairingParams_InitatorKeyDistributionIdKey = FsciParameter("SmPairingParams_InitatorKeyDistributionIdKey", 1)
        cmdParams.append(SmPairingParams_InitatorKeyDistributionIdKey)
        SmPairingParams_InitatorKeyDistributionSign = FsciParameter("SmPairingParams_InitatorKeyDistributionSign", 1)
        cmdParams.append(SmPairingParams_InitatorKeyDistributionSign)
        SmPairingParams_InitatorKeyDistributionLinkKey = FsciParameter("SmPairingParams_InitatorKeyDistributionLinkKey", 1)
        cmdParams.append(SmPairingParams_InitatorKeyDistributionLinkKey)
        SmPairingParams_InitatorKeyDistributionReserved = FsciParameter("SmPairingParams_InitatorKeyDistributionReserved", 1)
        cmdParams.append(SmPairingParams_InitatorKeyDistributionReserved)
        SmPairingParams_ResponderKeyDistributionEncKey = FsciParameter("SmPairingParams_ResponderKeyDistributionEncKey", 1)
        cmdParams.append(SmPairingParams_ResponderKeyDistributionEncKey)
        SmPairingParams_ResponderKeyDistributionIdKey = FsciParameter("SmPairingParams_ResponderKeyDistributionIdKey", 1)
        cmdParams.append(SmPairingParams_ResponderKeyDistributionIdKey)
        SmPairingParams_ResponderKeyDistributionSign = FsciParameter("SmPairingParams_ResponderKeyDistributionSign", 1)
        cmdParams.append(SmPairingParams_ResponderKeyDistributionSign)
        SmPairingParams_ResponderKeyDistributionLinkKey = FsciParameter("SmPairingParams_ResponderKeyDistributionLinkKey", 1)
        cmdParams.append(SmPairingParams_ResponderKeyDistributionLinkKey)
        SmPairingParams_ResponderKeyDistributionReserved = FsciParameter("SmPairingParams_ResponderKeyDistributionReserved", 1)
        cmdParams.append(SmPairingParams_ResponderKeyDistributionReserved)
        return FsciFrameDescription(0x42, 0x03, cmdParams)

    def InitSMSendSecurityRequestRequest(self):
        cmdParams = []
        DestinationDeviceId = FsciParameter("DestinationDeviceId", 1)
        cmdParams.append(DestinationDeviceId)
        SmSecurityReqParams_AuthRequestBondingFlags = FsciParameter("SmSecurityReqParams_AuthRequestBondingFlags", 1)
        cmdParams.append(SmSecurityReqParams_AuthRequestBondingFlags)
        SmSecurityReqParams_AuthRequestMitm = FsciParameter("SmSecurityReqParams_AuthRequestMitm", 1)
        cmdParams.append(SmSecurityReqParams_AuthRequestMitm)
        SmSecurityReqParams_AuthRequestsc = FsciParameter("SmSecurityReqParams_AuthRequestsc", 1)
        cmdParams.append(SmSecurityReqParams_AuthRequestsc)
        SmSecurityReqParams_AuthRequestkeypress = FsciParameter("SmSecurityReqParams_AuthRequestkeypress", 1)
        cmdParams.append(SmSecurityReqParams_AuthRequestkeypress)
        return FsciFrameDescription(0x42, 0x04, cmdParams)

    def InitSMTerminatePairingRequest(self):
        cmdParams = []
        DestinationDeviceId = FsciParameter("DestinationDeviceId", 1)
        cmdParams.append(DestinationDeviceId)
        SmTerminatePairingParams_Reason = FsciParameter("SmTerminatePairingParams_Reason", 1)
        cmdParams.append(SmTerminatePairingParams_Reason)
        return FsciFrameDescription(0x42, 0x05, cmdParams)

    def InitSMPairingRequestReplyRequest(self):
        cmdParams = []
        DestinationDeviceId = FsciParameter("DestinationDeviceId", 1)
        cmdParams.append(DestinationDeviceId)
        SmPairingParams_IoCapabilities = FsciParameter("SmPairingParams_IoCapabilities", 1)
        cmdParams.append(SmPairingParams_IoCapabilities)
        SmPairingParams_OobDataFlag = FsciParameter("SmPairingParams_OobDataFlag", 1)
        cmdParams.append(SmPairingParams_OobDataFlag)
        SmPairingParams_AuthRequestBondingFlags = FsciParameter("SmPairingParams_AuthRequestBondingFlags", 1)
        cmdParams.append(SmPairingParams_AuthRequestBondingFlags)
        SmPairingParams_AuthRequestMitm = FsciParameter("SmPairingParams_AuthRequestMitm", 1)
        cmdParams.append(SmPairingParams_AuthRequestMitm)
        SmPairingParams_AuthRequestsc = FsciParameter("SmPairingParams_AuthRequestsc", 1)
        cmdParams.append(SmPairingParams_AuthRequestsc)
        SmPairingParams_AuthRequestkeypress = FsciParameter("SmPairingParams_AuthRequestkeypress", 1)
        cmdParams.append(SmPairingParams_AuthRequestkeypress)
        SmPairingParams_MaxEncKeySize = FsciParameter("SmPairingParams_MaxEncKeySize", 1)
        cmdParams.append(SmPairingParams_MaxEncKeySize)
        SmPairingParams_InitatorKeyDistributionEncKey = FsciParameter("SmPairingParams_InitatorKeyDistributionEncKey", 1)
        cmdParams.append(SmPairingParams_InitatorKeyDistributionEncKey)
        SmPairingParams_InitatorKeyDistributionIdKey = FsciParameter("SmPairingParams_InitatorKeyDistributionIdKey", 1)
        cmdParams.append(SmPairingParams_InitatorKeyDistributionIdKey)
        SmPairingParams_InitatorKeyDistributionSign = FsciParameter("SmPairingParams_InitatorKeyDistributionSign", 1)
        cmdParams.append(SmPairingParams_InitatorKeyDistributionSign)
        SmPairingParams_InitatorKeyDistributionLinkKey = FsciParameter("SmPairingParams_InitatorKeyDistributionLinkKey", 1)
        cmdParams.append(SmPairingParams_InitatorKeyDistributionLinkKey)
        SmPairingParams_InitatorKeyDistributionReserved = FsciParameter("SmPairingParams_InitatorKeyDistributionReserved", 1)
        cmdParams.append(SmPairingParams_InitatorKeyDistributionReserved)
        SmPairingParams_ResponderKeyDistributionEncKey = FsciParameter("SmPairingParams_ResponderKeyDistributionEncKey", 1)
        cmdParams.append(SmPairingParams_ResponderKeyDistributionEncKey)
        SmPairingParams_ResponderKeyDistributionIdKey = FsciParameter("SmPairingParams_ResponderKeyDistributionIdKey", 1)
        cmdParams.append(SmPairingParams_ResponderKeyDistributionIdKey)
        SmPairingParams_ResponderKeyDistributionSign = FsciParameter("SmPairingParams_ResponderKeyDistributionSign", 1)
        cmdParams.append(SmPairingParams_ResponderKeyDistributionSign)
        SmPairingParams_ResponderKeyDistributionLinkKey = FsciParameter("SmPairingParams_ResponderKeyDistributionLinkKey", 1)
        cmdParams.append(SmPairingParams_ResponderKeyDistributionLinkKey)
        SmPairingParams_ResponderKeyDistributionReserved = FsciParameter("SmPairingParams_ResponderKeyDistributionReserved", 1)
        cmdParams.append(SmPairingParams_ResponderKeyDistributionReserved)
        return FsciFrameDescription(0x42, 0x06, cmdParams)

    def InitSMPasskeyRequestReplyRequest(self):
        cmdParams = []
        DestinationDeviceId = FsciParameter("DestinationDeviceId", 1)
        cmdParams.append(DestinationDeviceId)
        SmPasskeyReqReplyParams_KeyType = FsciParameter("SmPasskeyReqReplyParams_KeyType", 1)
        cmdParams.append(SmPasskeyReqReplyParams_KeyType)
        SmPasskeyReqReplyParams_Keydict = {}
        currentList = []
        SmPasskeyReqReplyParams_KeyPasskey = FsciParameter("SmPasskeyReqReplyParams_KeyPasskey", 4)
        currentList.append(SmPasskeyReqReplyParams_KeyPasskey)
        SmPasskeyReqReplyParams_Keydict[0x00] = currentList
        currentList = []
        SmPasskeyReqReplyParams_KeyOob = FsciParameter("SmPasskeyReqReplyParams_KeyOob", 16)
        currentList.append(SmPasskeyReqReplyParams_KeyOob)
        SmPasskeyReqReplyParams_Keydict[0x01] = currentList
        SmPasskeyReqReplyParams_Key = FsciParameter("SmPasskeyReqReplyParams_Key", 1, SmPasskeyReqReplyParams_KeyType, SmPasskeyReqReplyParams_Keydict)
        cmdParams.append(SmPasskeyReqReplyParams_Key)
        return FsciFrameDescription(0x42, 0x07, cmdParams)

    def InitSMPairingKeysetRequestReplyRequest(self):
        cmdParams = []
        DestinationDeviceId = FsciParameter("DestinationDeviceId", 1)
        cmdParams.append(DestinationDeviceId)
        PairingKeysetRequestReplyParams_KeyDistPayloadLtk = FsciParameter("PairingKeysetRequestReplyParams_KeyDistPayloadLtk", 16)
        cmdParams.append(PairingKeysetRequestReplyParams_KeyDistPayloadLtk)
        PairingKeysetRequestReplyParams_KeyDistPayloadIrk = FsciParameter("PairingKeysetRequestReplyParams_KeyDistPayloadIrk", 16)
        cmdParams.append(PairingKeysetRequestReplyParams_KeyDistPayloadIrk)
        PairingKeysetRequestReplyParams_KeyDistPayloadCsrk = FsciParameter("PairingKeysetRequestReplyParams_KeyDistPayloadCsrk", 16)
        cmdParams.append(PairingKeysetRequestReplyParams_KeyDistPayloadCsrk)
        PairingKeysetRequestReplyParams_KeyDistPayloadEdiv = FsciParameter("PairingKeysetRequestReplyParams_KeyDistPayloadEdiv", 2)
        cmdParams.append(PairingKeysetRequestReplyParams_KeyDistPayloadEdiv)
        PairingKeysetRequestReplyParams_KeyDistPayloadRand = FsciParameter("PairingKeysetRequestReplyParams_KeyDistPayloadRand", 8)
        cmdParams.append(PairingKeysetRequestReplyParams_KeyDistPayloadRand)
        PairingKeysetRequestReplyParams_KeyDistPayloadBDAddr = FsciParameter("PairingKeysetRequestReplyParams_KeyDistPayloadBDAddr", 6)
        cmdParams.append(PairingKeysetRequestReplyParams_KeyDistPayloadBDAddr)
        PairingKeysetRequestReplyParams_KeyDistPayloadBDAddrType = FsciParameter("PairingKeysetRequestReplyParams_KeyDistPayloadBDAddrType", 1)
        cmdParams.append(PairingKeysetRequestReplyParams_KeyDistPayloadBDAddrType)
        PairingKeysetRequestReplyParams_SentKeysEncKey = FsciParameter("PairingKeysetRequestReplyParams_SentKeysEncKey", 1)
        cmdParams.append(PairingKeysetRequestReplyParams_SentKeysEncKey)
        PairingKeysetRequestReplyParams_SentKeysIdKey = FsciParameter("PairingKeysetRequestReplyParams_SentKeysIdKey", 1)
        cmdParams.append(PairingKeysetRequestReplyParams_SentKeysIdKey)
        PairingKeysetRequestReplyParams_SentKeysSign = FsciParameter("PairingKeysetRequestReplyParams_SentKeysSign", 1)
        cmdParams.append(PairingKeysetRequestReplyParams_SentKeysSign)
        PairingKeysetRequestReplyParams_SentKeysLinkKey = FsciParameter("PairingKeysetRequestReplyParams_SentKeysLinkKey", 1)
        cmdParams.append(PairingKeysetRequestReplyParams_SentKeysLinkKey)
        PairingKeysetRequestReplyParams_SentKeysReserved = FsciParameter("PairingKeysetRequestReplyParams_SentKeysReserved", 1)
        cmdParams.append(PairingKeysetRequestReplyParams_SentKeysReserved)
        return FsciFrameDescription(0x42, 0x08, cmdParams)

    def InitSMLlStartEncryptionRequest(self):
        cmdParams = []
        DestinationDeviceId = FsciParameter("DestinationDeviceId", 1)
        cmdParams.append(DestinationDeviceId)
        LlStartEncryptionParams_RandomNumber = FsciParameter("LlStartEncryptionParams_RandomNumber", 8)
        cmdParams.append(LlStartEncryptionParams_RandomNumber)
        LlStartEncryptionParams_EncryptedDiversifier = FsciParameter("LlStartEncryptionParams_EncryptedDiversifier", 2)
        cmdParams.append(LlStartEncryptionParams_EncryptedDiversifier)
        LlStartEncryptionParams_LongTermKey = FsciParameter("LlStartEncryptionParams_LongTermKey", 16)
        cmdParams.append(LlStartEncryptionParams_LongTermKey)
        return FsciFrameDescription(0x42, 0x09, cmdParams)

    def InitSMLlLtkRequestReplyRequest(self):
        cmdParams = []
        DestinationDeviceId = FsciParameter("DestinationDeviceId", 1)
        cmdParams.append(DestinationDeviceId)
        LlLtkRequestReplyParams_LongTermKey = FsciParameter("LlLtkRequestReplyParams_LongTermKey", 16)
        cmdParams.append(LlLtkRequestReplyParams_LongTermKey)
        return FsciFrameDescription(0x42, 0x0A, cmdParams)

    def InitSMLlLtkRequestNegativeReplyRequest(self):
        cmdParams = []
        DestinationDeviceId = FsciParameter("DestinationDeviceId", 1)
        cmdParams.append(DestinationDeviceId)
        return FsciFrameDescription(0x42, 0x0B, cmdParams)

    def InitSMLlEncryptRequestRequest(self):
        cmdParams = []
        LlEncryptReqParams_Key = FsciParameter("LlEncryptReqParams_Key", 16)
        cmdParams.append(LlEncryptReqParams_Key)
        LlEncryptReqParams_PlainTextData = FsciParameter("LlEncryptReqParams_PlainTextData", 16)
        cmdParams.append(LlEncryptReqParams_PlainTextData)
        return FsciFrameDescription(0x42, 0x0C, cmdParams)

    def InitSMLlRandRequestRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x42, 0x0D, cmdParams)

    def InitSMTbCreateRandomDeviceAddressRequestRequest(self):
        cmdParams = []
        TbCreateRandomDeviceAddrReqParams_AddrType = FsciParameter("TbCreateRandomDeviceAddrReqParams_AddrType", 1)
        cmdParams.append(TbCreateRandomDeviceAddrReqParams_AddrType)
        TbCreateRandomDeviceAddrReqParams_Datadict = {}
        currentList = []
        TbCreateRandomDeviceAddrReqParams_DataRandAddrTypeNone = FsciParameter("TbCreateRandomDeviceAddrReqParams_DataRandAddrTypeNone", 1)
        currentList.append(TbCreateRandomDeviceAddrReqParams_DataRandAddrTypeNone)
        TbCreateRandomDeviceAddrReqParams_Datadict[0x00] = currentList
        currentList = []
        TbCreateRandomDeviceAddrReqParams_DataStaticAddr = FsciParameter("TbCreateRandomDeviceAddrReqParams_DataStaticAddr", 1)
        currentList.append(TbCreateRandomDeviceAddrReqParams_DataStaticAddr)
        TbCreateRandomDeviceAddrReqParams_Datadict[0x01] = currentList
        currentList = []
        TbCreateRandomDeviceAddrReqParams_DataNonResolvablePrivateAddr = FsciParameter("TbCreateRandomDeviceAddrReqParams_DataNonResolvablePrivateAddr", 1)
        currentList.append(TbCreateRandomDeviceAddrReqParams_DataNonResolvablePrivateAddr)
        TbCreateRandomDeviceAddrReqParams_Datadict[0x02] = currentList
        currentList = []
        ResolvablePrivateAddrIRK = FsciParameter("ResolvablePrivateAddrIRK", 16)
        currentList.append(ResolvablePrivateAddrIRK)
        ResolvablePrivateAddrRandIncluded = FsciParameter("ResolvablePrivateAddrRandIncluded", 1)
        currentList.append(ResolvablePrivateAddrRandIncluded)
        ResolvablePrivateAddrRand = FsciParameter("ResolvablePrivateAddrRand", 3, ResolvablePrivateAddrRandIncluded)
        currentList.append(ResolvablePrivateAddrRand)
        TbCreateRandomDeviceAddrReqParams_Datadict[0x03] = currentList
        TbCreateRandomDeviceAddrReqParams_Data = FsciParameter("TbCreateRandomDeviceAddrReqParams_Data", 1, TbCreateRandomDeviceAddrReqParams_AddrType, TbCreateRandomDeviceAddrReqParams_Datadict)
        cmdParams.append(TbCreateRandomDeviceAddrReqParams_Data)
        return FsciFrameDescription(0x42, 0x0E, cmdParams)

    def InitSMTbCheckResolvablePrivateAddressRequestRequest(self):
        cmdParams = []
        TbCheckResolvablePrivateAddrReqParams_IRK = FsciParameter("TbCheckResolvablePrivateAddrReqParams_IRK", 16)
        cmdParams.append(TbCheckResolvablePrivateAddrReqParams_IRK)
        TbCheckResolvablePrivateAddrReqParams_Address = FsciParameter("TbCheckResolvablePrivateAddrReqParams_Address", 6)
        cmdParams.append(TbCheckResolvablePrivateAddrReqParams_Address)
        return FsciFrameDescription(0x42, 0x0F, cmdParams)

    def InitSMTbSignDataRequestRequest(self):
        cmdParams = []
        TbSignDataReqParams_CSRK = FsciParameter("TbSignDataReqParams_CSRK", 16)
        cmdParams.append(TbSignDataReqParams_CSRK)
        TbSignDataReqParams_DataLength = FsciParameter("TbSignDataReqParams_DataLength", 2)
        cmdParams.append(TbSignDataReqParams_DataLength)
        TbSignDataReqParams_Data = FsciParameter("TbSignDataReqParams_Data", 1, TbSignDataReqParams_DataLength)
        cmdParams.append(TbSignDataReqParams_Data)
        return FsciFrameDescription(0x42, 0x10, cmdParams)

    def InitSMTbVerifyDataSignatureRequestRequest(self):
        cmdParams = []
        TbVerifyDataSignatureReqParams_CSRK = FsciParameter("TbVerifyDataSignatureReqParams_CSRK", 16)
        cmdParams.append(TbVerifyDataSignatureReqParams_CSRK)
        TbVerifyDataSignatureReqParams_DataLength = FsciParameter("TbVerifyDataSignatureReqParams_DataLength", 2)
        cmdParams.append(TbVerifyDataSignatureReqParams_DataLength)
        TbVerifyDataSignatureReqParams_Data = FsciParameter("TbVerifyDataSignatureReqParams_Data", 1, TbVerifyDataSignatureReqParams_DataLength)
        cmdParams.append(TbVerifyDataSignatureReqParams_Data)
        TbVerifyDataSignatureReqParams_Signature = FsciParameter("TbVerifyDataSignatureReqParams_Signature", 8)
        cmdParams.append(TbVerifyDataSignatureReqParams_Signature)
        return FsciFrameDescription(0x42, 0x11, cmdParams)

    def InitSMDeviceConnectNotificationRequest(self):
        cmdParams = []
        DestinationDeviceId = FsciParameter("DestinationDeviceId", 1)
        cmdParams.append(DestinationDeviceId)
        return FsciFrameDescription(0x42, 0x12, cmdParams)

    def InitSMDeviceDisconnectNotificationRequest(self):
        cmdParams = []
        DestinationDeviceId = FsciParameter("DestinationDeviceId", 1)
        cmdParams.append(DestinationDeviceId)
        return FsciFrameDescription(0x42, 0x13, cmdParams)

    def InitSMSetDefaultPasskeyRequest(self):
        cmdParams = []
        SetDefaultPasskeyParams_DefaultPasskey = FsciParameter("SetDefaultPasskeyParams_DefaultPasskey", 4)
        cmdParams.append(SetDefaultPasskeyParams_DefaultPasskey)
        return FsciFrameDescription(0x42, 0x14, cmdParams)

    def InitSMSetOobMitmProtectionRequest(self):
        cmdParams = []
        oobMitmProt = FsciParameter("oobMitmProt", 1)
        cmdParams.append(oobMitmProt)
        return FsciFrameDescription(0x42, 0x15, cmdParams)

    def InitSMSendKeypressNotificationRequest(self):
        cmdParams = []
        DestinationDeviceId = FsciParameter("DestinationDeviceId", 1)
        cmdParams.append(DestinationDeviceId)
        SmKeypressNotificationParams_keypressNotifType = FsciParameter("SmKeypressNotificationParams_keypressNotifType", 1)
        cmdParams.append(SmKeypressNotificationParams_keypressNotifType)
        return FsciFrameDescription(0x42, 0x16, cmdParams)

    def InitSMNcDisplayConfirmRequest(self):
        cmdParams = []
        DestinationDeviceId = FsciParameter("DestinationDeviceId", 1)
        cmdParams.append(DestinationDeviceId)
        ncValueConfirm = FsciParameter("ncValueConfirm", 1)
        cmdParams.append(ncValueConfirm)
        return FsciFrameDescription(0x42, 0x17, cmdParams)

    def InitSMLeScOobDataRequestReplyRequest(self):
        cmdParams = []
        DestinationDeviceId = FsciParameter("DestinationDeviceId", 1)
        cmdParams.append(DestinationDeviceId)
        LeScOobDataParams_ = FsciParameter("LeScOobDataParams_", 1)
        cmdParams.append(LeScOobDataParams_)
        return FsciFrameDescription(0x42, 0x18, cmdParams)

    def InitSMLocalLeScOobDataRequestReqRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x42, 0x19, cmdParams)

    def InitSMGenerateNewEcdhPkSkPairRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x42, 0x1A, cmdParams)

    def InitSMSetMinPairingSecurityPropertiesRequest(self):
        cmdParams = []
        mitmProtection = FsciParameter("mitmProtection", 1)
        cmdParams.append(mitmProtection)
        leSc = FsciParameter("leSc", 1)
        cmdParams.append(leSc)
        minEncKeySize = FsciParameter("minEncKeySize", 1)
        cmdParams.append(minEncKeySize)
        return FsciFrameDescription(0x42, 0x1B, cmdParams)

    def InitATTModeSelectRequest(self):
        cmdParams = []
        Enable = FsciParameter("Enable", 1)
        cmdParams.append(Enable)
        return FsciFrameDescription(0x43, 0x00, cmdParams)

    def InitATTInitRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x43, 0x01, cmdParams)

    def InitATTNotifyConnectionRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        return FsciFrameDescription(0x43, 0x02, cmdParams)

    def InitATTNotifyDisconnectionRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        return FsciFrameDescription(0x43, 0x03, cmdParams)

    def InitATTSetMtuRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Mtu = FsciParameter("Mtu", 2)
        cmdParams.append(Mtu)
        return FsciFrameDescription(0x43, 0x04, cmdParams)

    def InitATTGetMtuRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        return FsciFrameDescription(0x43, 0x05, cmdParams)

    def InitATTRegisterOpcodeCallbackRequest(self):
        cmdParams = []
        Opcode = FsciParameter("Opcode", 1)
        cmdParams.append(Opcode)
        return FsciFrameDescription(0x43, 0x06, cmdParams)

    def InitATTRegisterUnsupportedOpcodeCallbackRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x43, 0x07, cmdParams)

    def InitATTRegisterTimeoutCallbackRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x43, 0x08, cmdParams)

    def InitATTServerSendErrorResponseRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_RequestOpcodeInError = FsciParameter("Params_RequestOpcodeInError", 1)
        cmdParams.append(Params_RequestOpcodeInError)
        Params_AttributeHandleInError = FsciParameter("Params_AttributeHandleInError", 2)
        cmdParams.append(Params_AttributeHandleInError)
        Params_ErrorCode = FsciParameter("Params_ErrorCode", 1)
        cmdParams.append(Params_ErrorCode)
        return FsciFrameDescription(0x43, 0x09, cmdParams)

    def InitATTClientSendExchangeMtuRequestRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_ClientRxMtu = FsciParameter("Params_ClientRxMtu", 2)
        cmdParams.append(Params_ClientRxMtu)
        return FsciFrameDescription(0x43, 0x0A, cmdParams)

    def InitATTServerSendExchangeMtuResponseRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_ServerRxMtu = FsciParameter("Params_ServerRxMtu", 2)
        cmdParams.append(Params_ServerRxMtu)
        return FsciFrameDescription(0x43, 0x0B, cmdParams)

    def InitATTClientSendFindInformationRequestRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_StartingHandle = FsciParameter("Params_StartingHandle", 2)
        cmdParams.append(Params_StartingHandle)
        Params_EndingHandle = FsciParameter("Params_EndingHandle", 2)
        cmdParams.append(Params_EndingHandle)
        return FsciFrameDescription(0x43, 0x0C, cmdParams)

    def InitATTServerSendFindInformationResponseRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_Format = FsciParameter("Params_Format", 1)
        cmdParams.append(Params_Format)
        Params_InformationDatadict = {}
        currentList = []
        Uuid16BitFormatHandleUuid16PairCount = FsciParameter("Uuid16BitFormatHandleUuid16PairCount", 1)
        currentList.append(Uuid16BitFormatHandleUuid16PairCount)
        HandleUuid16Handle = FsciParameter("HandleUuid16Handle", 2)
        currentList.append(HandleUuid16Handle)
        HandleUuid16Uuid16 = FsciParameter("HandleUuid16Uuid16", 2)
        currentList.append(HandleUuid16Uuid16)
        Params_InformationDatadict[0x01] = currentList
        currentList = []
        Uuid128BitFormatHandleUuid128PairCount = FsciParameter("Uuid128BitFormatHandleUuid128PairCount", 1)
        currentList.append(Uuid128BitFormatHandleUuid128PairCount)
        HandleUuid128Handle = FsciParameter("HandleUuid128Handle", 2)
        currentList.append(HandleUuid128Handle)
        HandleUuid128Uuid128 = FsciParameter("HandleUuid128Uuid128", 16)
        currentList.append(HandleUuid128Uuid128)
        Params_InformationDatadict[0x02] = currentList
        Params_InformationData = FsciParameter("Params_InformationData", 1, Params_Format, Params_InformationDatadict)
        cmdParams.append(Params_InformationData)
        return FsciFrameDescription(0x43, 0x0D, cmdParams)

    def InitATTClientSendFindByTypeValueRequestRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_StartingHandle = FsciParameter("Params_StartingHandle", 2)
        cmdParams.append(Params_StartingHandle)
        Params_EndingHandle = FsciParameter("Params_EndingHandle", 2)
        cmdParams.append(Params_EndingHandle)
        Params_AttributeType = FsciParameter("Params_AttributeType", 2)
        cmdParams.append(Params_AttributeType)
        Params_AttributeLength = FsciParameter("Params_AttributeLength", 2)
        cmdParams.append(Params_AttributeLength)
        Params_AttributeValue = FsciParameter("Params_AttributeValue", 1, Params_AttributeLength)
        cmdParams.append(Params_AttributeValue)
        return FsciFrameDescription(0x43, 0x0E, cmdParams)

    def InitATTServerSendFindByTypeValueResponseRequest(self):
        cmdParams = []
        # not generated, pickle() is used instead; see frames.py
        return FsciFrameDescription(0x43, 0x0F, cmdParams)

    def InitATTClientSendReadByTypeRequestRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_StartingHandle = FsciParameter("Params_StartingHandle", 2)
        cmdParams.append(Params_StartingHandle)
        Params_EndingHandle = FsciParameter("Params_EndingHandle", 2)
        cmdParams.append(Params_EndingHandle)
        Params_Format = FsciParameter("Params_Format", 1)
        cmdParams.append(Params_Format)
        Params_AttributeTypedict = {}
        currentList = []
        Params_AttributeTypeUuid16BitFormat = FsciParameter("Params_AttributeTypeUuid16BitFormat", 2)
        currentList.append(Params_AttributeTypeUuid16BitFormat)
        Params_AttributeTypedict[0x01] = currentList
        currentList = []
        Params_AttributeTypeUuid128BitFormat = FsciParameter("Params_AttributeTypeUuid128BitFormat", 16)
        currentList.append(Params_AttributeTypeUuid128BitFormat)
        Params_AttributeTypedict[0x02] = currentList
        Params_AttributeType = FsciParameter("Params_AttributeType", 1, Params_Format, Params_AttributeTypedict)
        cmdParams.append(Params_AttributeType)
        return FsciFrameDescription(0x43, 0x10, cmdParams)

    def InitATTServerSendReadByTypeResponseRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_Length = FsciParameter("Params_Length", 1)
        cmdParams.append(Params_Length)
        Params_AttributeDataListLength = FsciParameter("Params_AttributeDataListLength", 2)
        cmdParams.append(Params_AttributeDataListLength)
        Params_AttributeDataList = FsciParameter("Params_AttributeDataList", 1, Params_AttributeDataListLength)
        cmdParams.append(Params_AttributeDataList)
        return FsciFrameDescription(0x43, 0x11, cmdParams)

    def InitATTClientSendReadRequestRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_AttributeHandle = FsciParameter("Params_AttributeHandle", 2)
        cmdParams.append(Params_AttributeHandle)
        return FsciFrameDescription(0x43, 0x12, cmdParams)

    def InitATTServerSendReadResponseRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_AttributeLength = FsciParameter("Params_AttributeLength", 2)
        cmdParams.append(Params_AttributeLength)
        Params_AttributeValue = FsciParameter("Params_AttributeValue", 1, Params_AttributeLength)
        cmdParams.append(Params_AttributeValue)
        return FsciFrameDescription(0x43, 0x13, cmdParams)

    def InitATTClientSendReadBlobRequestRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_AttributeHandle = FsciParameter("Params_AttributeHandle", 2)
        cmdParams.append(Params_AttributeHandle)
        Params_ValueOffset = FsciParameter("Params_ValueOffset", 2)
        cmdParams.append(Params_ValueOffset)
        return FsciFrameDescription(0x43, 0x14, cmdParams)

    def InitATTServerSendReadBlobResponseRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_AttributeLength = FsciParameter("Params_AttributeLength", 2)
        cmdParams.append(Params_AttributeLength)
        Params_AttributeValue = FsciParameter("Params_AttributeValue", 1, Params_AttributeLength)
        cmdParams.append(Params_AttributeValue)
        return FsciFrameDescription(0x43, 0x15, cmdParams)

    def InitATTClientSendReadMultipleRequestRequest(self):
        cmdParams = []
        # not generated, pickle() is used instead; see frames.py
        return FsciFrameDescription(0x43, 0x16, cmdParams)

    def InitATTServerSendReadMultipleResponseRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_ListLength = FsciParameter("Params_ListLength", 2)
        cmdParams.append(Params_ListLength)
        Params_ListOfValues = FsciParameter("Params_ListOfValues", 1, Params_ListLength)
        cmdParams.append(Params_ListOfValues)
        return FsciFrameDescription(0x43, 0x17, cmdParams)

    def InitATTClientSendReadByGroupTypeRequestRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_StartingHandle = FsciParameter("Params_StartingHandle", 2)
        cmdParams.append(Params_StartingHandle)
        Params_EndingHandle = FsciParameter("Params_EndingHandle", 2)
        cmdParams.append(Params_EndingHandle)
        Params_Format = FsciParameter("Params_Format", 1)
        cmdParams.append(Params_Format)
        Params_AttributeTypedict = {}
        currentList = []
        Params_AttributeTypeUuid16BitFormat = FsciParameter("Params_AttributeTypeUuid16BitFormat", 2)
        currentList.append(Params_AttributeTypeUuid16BitFormat)
        Params_AttributeTypedict[0x01] = currentList
        currentList = []
        Params_AttributeTypeUuid128BitFormat = FsciParameter("Params_AttributeTypeUuid128BitFormat", 16)
        currentList.append(Params_AttributeTypeUuid128BitFormat)
        Params_AttributeTypedict[0x02] = currentList
        Params_AttributeType = FsciParameter("Params_AttributeType", 1, Params_Format, Params_AttributeTypedict)
        cmdParams.append(Params_AttributeType)
        return FsciFrameDescription(0x43, 0x18, cmdParams)

    def InitATTServerSendReadByGroupTypeResponseRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_Length = FsciParameter("Params_Length", 1)
        cmdParams.append(Params_Length)
        Params_AttributeDataListLength = FsciParameter("Params_AttributeDataListLength", 2)
        cmdParams.append(Params_AttributeDataListLength)
        Params_AttributeDataList = FsciParameter("Params_AttributeDataList", 1, Params_AttributeDataListLength)
        cmdParams.append(Params_AttributeDataList)
        return FsciFrameDescription(0x43, 0x19, cmdParams)

    def InitATTClientSendWriteRequestRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_AttributeHandle = FsciParameter("Params_AttributeHandle", 2)
        cmdParams.append(Params_AttributeHandle)
        Params_AttributeLength = FsciParameter("Params_AttributeLength", 2)
        cmdParams.append(Params_AttributeLength)
        Params_AttributeValue = FsciParameter("Params_AttributeValue", 1, Params_AttributeLength)
        cmdParams.append(Params_AttributeValue)
        return FsciFrameDescription(0x43, 0x1A, cmdParams)

    def InitATTServerSendWriteResponseRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        return FsciFrameDescription(0x43, 0x1B, cmdParams)

    def InitATTClientSendWriteCommandRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_AttributeHandle = FsciParameter("Params_AttributeHandle", 2)
        cmdParams.append(Params_AttributeHandle)
        Params_AttributeLength = FsciParameter("Params_AttributeLength", 2)
        cmdParams.append(Params_AttributeLength)
        Params_AttributeValue = FsciParameter("Params_AttributeValue", 1, Params_AttributeLength)
        cmdParams.append(Params_AttributeValue)
        return FsciFrameDescription(0x43, 0x1C, cmdParams)

    def InitATTClientSendSignedWriteCommandRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_AttributeHandle = FsciParameter("Params_AttributeHandle", 2)
        cmdParams.append(Params_AttributeHandle)
        Params_AttributeLength = FsciParameter("Params_AttributeLength", 2)
        cmdParams.append(Params_AttributeLength)
        Params_AttributeValue = FsciParameter("Params_AttributeValue", 1, Params_AttributeLength)
        cmdParams.append(Params_AttributeValue)
        Params_AuthenticationSignature = FsciParameter("Params_AuthenticationSignature", 12)
        cmdParams.append(Params_AuthenticationSignature)
        return FsciFrameDescription(0x43, 0x1D, cmdParams)

    def InitATTClientSendPrepareWriteRequestRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_AttributeHandle = FsciParameter("Params_AttributeHandle", 2)
        cmdParams.append(Params_AttributeHandle)
        Params_ValueOffset = FsciParameter("Params_ValueOffset", 2)
        cmdParams.append(Params_ValueOffset)
        Params_AttributeLength = FsciParameter("Params_AttributeLength", 2)
        cmdParams.append(Params_AttributeLength)
        Params_AttributeValue = FsciParameter("Params_AttributeValue", 1, Params_AttributeLength)
        cmdParams.append(Params_AttributeValue)
        return FsciFrameDescription(0x43, 0x1E, cmdParams)

    def InitATTServerSendPrepareWriteResponseRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_AttributeHandle = FsciParameter("Params_AttributeHandle", 2)
        cmdParams.append(Params_AttributeHandle)
        Params_ValueOffset = FsciParameter("Params_ValueOffset", 2)
        cmdParams.append(Params_ValueOffset)
        Params_AttributeLength = FsciParameter("Params_AttributeLength", 2)
        cmdParams.append(Params_AttributeLength)
        Params_AttributeValue = FsciParameter("Params_AttributeValue", 1, Params_AttributeLength)
        cmdParams.append(Params_AttributeValue)
        return FsciFrameDescription(0x43, 0x1F, cmdParams)

    def InitATTClientSendExecuteWriteRequestRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_Flags = FsciParameter("Params_Flags", 1)
        cmdParams.append(Params_Flags)
        return FsciFrameDescription(0x43, 0x20, cmdParams)

    def InitATTServerSendExecuteWriteResponseRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        return FsciFrameDescription(0x43, 0x21, cmdParams)

    def InitATTServerSendHandleValueNotificationRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_AttributeHandle = FsciParameter("Params_AttributeHandle", 2)
        cmdParams.append(Params_AttributeHandle)
        Params_AttributeLength = FsciParameter("Params_AttributeLength", 2)
        cmdParams.append(Params_AttributeLength)
        Params_AttributeValue = FsciParameter("Params_AttributeValue", 1, Params_AttributeLength)
        cmdParams.append(Params_AttributeValue)
        return FsciFrameDescription(0x43, 0x22, cmdParams)

    def InitATTServerSendHandleValueIndicationRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_AttributeHandle = FsciParameter("Params_AttributeHandle", 2)
        cmdParams.append(Params_AttributeHandle)
        Params_AttributeLength = FsciParameter("Params_AttributeLength", 2)
        cmdParams.append(Params_AttributeLength)
        Params_AttributeValue = FsciParameter("Params_AttributeValue", 1, Params_AttributeLength)
        cmdParams.append(Params_AttributeValue)
        return FsciFrameDescription(0x43, 0x23, cmdParams)

    def InitATTClientSendHandleValueConfirmationRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        return FsciFrameDescription(0x43, 0x24, cmdParams)

    def InitGATTModeSelectRequest(self):
        cmdParams = []
        Enable = FsciParameter("Enable", 1)
        cmdParams.append(Enable)
        return FsciFrameDescription(0x44, 0x00, cmdParams)

    def InitGATTInitRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x44, 0x01, cmdParams)

    def InitGATTGetMtuRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        return FsciFrameDescription(0x44, 0x02, cmdParams)

    def InitGATTClientInitRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x44, 0x03, cmdParams)

    def InitGATTClientResetProcedureRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x44, 0x04, cmdParams)

    def InitGATTClientRegisterProcedureCallbackRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x44, 0x05, cmdParams)

    def InitGATTClientRegisterNotificationCallbackRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x44, 0x06, cmdParams)

    def InitGATTClientRegisterIndicationCallbackRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x44, 0x07, cmdParams)

    def InitGATTClientExchangeMtuRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        return FsciFrameDescription(0x44, 0x08, cmdParams)

    def InitGATTClientDiscoverAllPrimaryServicesRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        MaxNbOfServices = FsciParameter("MaxNbOfServices", 1)
        cmdParams.append(MaxNbOfServices)
        return FsciFrameDescription(0x44, 0x09, cmdParams)

    def InitGATTClientDiscoverPrimaryServicesByUuidRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        UuidType = FsciParameter("UuidType", 1)
        cmdParams.append(UuidType)
        Uuiddict = {}
        currentList = []
        UuidUuid16Bits = FsciParameter("UuidUuid16Bits", 2)
        currentList.append(UuidUuid16Bits)
        Uuiddict[0x01] = currentList
        currentList = []
        UuidUuid128Bits = FsciParameter("UuidUuid128Bits", 16)
        currentList.append(UuidUuid128Bits)
        Uuiddict[0x02] = currentList
        currentList = []
        UuidUuid32Bits = FsciParameter("UuidUuid32Bits", 4)
        currentList.append(UuidUuid32Bits)
        Uuiddict[0x03] = currentList
        Uuid = FsciParameter("Uuid", 1, UuidType, Uuiddict)
        cmdParams.append(Uuid)
        MaxNbOfServices = FsciParameter("MaxNbOfServices", 1)
        cmdParams.append(MaxNbOfServices)
        return FsciFrameDescription(0x44, 0x0A, cmdParams)

    def InitGATTClientFindIncludedServicesRequest(self):
        cmdParams = []
        # not generated, pickle() is used instead; see frames.py
        return FsciFrameDescription(0x44, 0x0B, cmdParams)

    def InitGATTClientDiscoverAllCharacteristicsOfServiceRequest(self):
        cmdParams = []
        # not generated, pickle() is used instead; see frames.py
        return FsciFrameDescription(0x44, 0x0C, cmdParams)

    def InitGATTClientDiscoverCharacteristicOfServiceByUuidRequest(self):
        cmdParams = []
        # not generated, pickle() is used instead; see frames.py
        return FsciFrameDescription(0x44, 0x0D, cmdParams)

    def InitGATTClientDiscoverAllCharacteristicDescriptorsRequest(self):
        cmdParams = []
        # not generated, pickle() is used instead; see frames.py
        return FsciFrameDescription(0x44, 0x0E, cmdParams)

    def InitGATTClientReadCharacteristicValueRequest(self):
        cmdParams = []
        # not generated, pickle() is used instead; see frames.py
        return FsciFrameDescription(0x44, 0x0F, cmdParams)

    def InitGATTClientReadUsingCharacteristicUuidRequest(self):
        cmdParams = []
        # not generated, pickle() is used instead; see frames.py
        return FsciFrameDescription(0x44, 0x10, cmdParams)

    def InitGATTClientReadMultipleCharacteristicValuesRequest(self):
        cmdParams = []
        # not generated, pickle() is used instead; see frames.py
        return FsciFrameDescription(0x44, 0x11, cmdParams)

    def InitGATTClientWriteCharacteristicValueRequest(self):
        cmdParams = []
        # not generated, pickle() is used instead; see frames.py
        return FsciFrameDescription(0x44, 0x12, cmdParams)

    def InitGATTClientReadCharacteristicDescriptorRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Descriptor_Handle = FsciParameter("Descriptor_Handle", 2)
        cmdParams.append(Descriptor_Handle)
        Descriptor_UuidType = FsciParameter("Descriptor_UuidType", 1)
        cmdParams.append(Descriptor_UuidType)
        Descriptor_Uuiddict = {}
        currentList = []
        Descriptor_UuidUuid16Bits = FsciParameter("Descriptor_UuidUuid16Bits", 2)
        currentList.append(Descriptor_UuidUuid16Bits)
        Descriptor_Uuiddict[0x01] = currentList
        currentList = []
        Descriptor_UuidUuid128Bits = FsciParameter("Descriptor_UuidUuid128Bits", 16)
        currentList.append(Descriptor_UuidUuid128Bits)
        Descriptor_Uuiddict[0x02] = currentList
        currentList = []
        Descriptor_UuidUuid32Bits = FsciParameter("Descriptor_UuidUuid32Bits", 4)
        currentList.append(Descriptor_UuidUuid32Bits)
        Descriptor_Uuiddict[0x03] = currentList
        Descriptor_Uuid = FsciParameter("Descriptor_Uuid", 1, Descriptor_UuidType, Descriptor_Uuiddict)
        cmdParams.append(Descriptor_Uuid)
        Descriptor_ValueLength = FsciParameter("Descriptor_ValueLength", 2)
        cmdParams.append(Descriptor_ValueLength)
        Descriptor_MaxValueLength = FsciParameter("Descriptor_MaxValueLength", 2)
        cmdParams.append(Descriptor_MaxValueLength)
        Descriptor_Value = FsciParameter("Descriptor_Value", 1, Descriptor_ValueLength)
        cmdParams.append(Descriptor_Value)
        MaxReadBytes = FsciParameter("MaxReadBytes", 2)
        cmdParams.append(MaxReadBytes)
        return FsciFrameDescription(0x44, 0x13, cmdParams)

    def InitGATTClientWriteCharacteristicDescriptorRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Descriptor_Handle = FsciParameter("Descriptor_Handle", 2)
        cmdParams.append(Descriptor_Handle)
        Descriptor_UuidType = FsciParameter("Descriptor_UuidType", 1)
        cmdParams.append(Descriptor_UuidType)
        Descriptor_Uuiddict = {}
        currentList = []
        Descriptor_UuidUuid16Bits = FsciParameter("Descriptor_UuidUuid16Bits", 2)
        currentList.append(Descriptor_UuidUuid16Bits)
        Descriptor_Uuiddict[0x01] = currentList
        currentList = []
        Descriptor_UuidUuid128Bits = FsciParameter("Descriptor_UuidUuid128Bits", 16)
        currentList.append(Descriptor_UuidUuid128Bits)
        Descriptor_Uuiddict[0x02] = currentList
        currentList = []
        Descriptor_UuidUuid32Bits = FsciParameter("Descriptor_UuidUuid32Bits", 4)
        currentList.append(Descriptor_UuidUuid32Bits)
        Descriptor_Uuiddict[0x03] = currentList
        Descriptor_Uuid = FsciParameter("Descriptor_Uuid", 1, Descriptor_UuidType, Descriptor_Uuiddict)
        cmdParams.append(Descriptor_Uuid)
        Descriptor_ValueLength = FsciParameter("Descriptor_ValueLength", 2)
        cmdParams.append(Descriptor_ValueLength)
        Descriptor_MaxValueLength = FsciParameter("Descriptor_MaxValueLength", 2)
        cmdParams.append(Descriptor_MaxValueLength)
        Descriptor_Value = FsciParameter("Descriptor_Value", 1, Descriptor_ValueLength)
        cmdParams.append(Descriptor_Value)
        ValueLength = FsciParameter("ValueLength", 2)
        cmdParams.append(ValueLength)
        Value = FsciParameter("Value", 1, ValueLength)
        cmdParams.append(Value)
        return FsciFrameDescription(0x44, 0x14, cmdParams)

    def InitGATTServerInitRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x44, 0x15, cmdParams)

    def InitGATTServerRegisterCallbackRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x44, 0x16, cmdParams)

    def InitGATTServerRegisterHandlesForWriteNotificationsRequest(self):
        cmdParams = []
        HandleCount = FsciParameter("HandleCount", 1)
        cmdParams.append(HandleCount)
        AttributeHandlesHandle = FsciParameter("AttributeHandles", 2, HandleCount)
        cmdParams.append(AttributeHandlesHandle)
        return FsciFrameDescription(0x44, 0x17, cmdParams)

    def InitGATTServerSendAttributeWrittenStatusRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        AttributeHandle = FsciParameter("AttributeHandle", 2)
        cmdParams.append(AttributeHandle)
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0x44, 0x18, cmdParams)

    def InitGATTServerSendNotificationRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Handle = FsciParameter("Handle", 2)
        cmdParams.append(Handle)
        return FsciFrameDescription(0x44, 0x19, cmdParams)

    def InitGATTServerSendIndicationRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Handle = FsciParameter("Handle", 2)
        cmdParams.append(Handle)
        return FsciFrameDescription(0x44, 0x1A, cmdParams)

    def InitGATTServerSendInstantValueNotificationRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Handle = FsciParameter("Handle", 2)
        cmdParams.append(Handle)
        ValueLength = FsciParameter("ValueLength", 2)
        cmdParams.append(ValueLength)
        Value = FsciParameter("Value", 1, ValueLength)
        cmdParams.append(Value)
        return FsciFrameDescription(0x44, 0x1B, cmdParams)

    def InitGATTServerSendInstantValueIndicationRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Handle = FsciParameter("Handle", 2)
        cmdParams.append(Handle)
        ValueLength = FsciParameter("ValueLength", 2)
        cmdParams.append(ValueLength)
        Value = FsciParameter("Value", 1, ValueLength)
        cmdParams.append(Value)
        return FsciFrameDescription(0x44, 0x1C, cmdParams)

    def InitGATTServerRegisterHandlesForReadNotificationsRequest(self):
        cmdParams = []
        HandleCount = FsciParameter("HandleCount", 1)
        cmdParams.append(HandleCount)
        AttributeHandlesHandle = FsciParameter("AttributeHandlesHandle", 2, HandleCount)
        cmdParams.append(AttributeHandlesHandle)
        return FsciFrameDescription(0x44, 0x1D, cmdParams)

    def InitGATTServerSendAttributeReadStatusRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        AttributeHandle = FsciParameter("AttributeHandle", 2)
        cmdParams.append(AttributeHandle)
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0x44, 0x1E, cmdParams)

    def InitGATTDBModeSelectRequest(self):
        cmdParams = []
        Enable = FsciParameter("Enable", 1)
        cmdParams.append(Enable)
        return FsciFrameDescription(0x45, 0x00, cmdParams)

    def InitGATTDBInitRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x45, 0x01, cmdParams)

    def InitGATTDBWriteAttributeRequest(self):
        cmdParams = []
        Handle = FsciParameter("Handle", 2)
        cmdParams.append(Handle)
        ValueLength = FsciParameter("ValueLength", 2)
        cmdParams.append(ValueLength)
        Value = FsciParameter("Value", 1, ValueLength)
        cmdParams.append(Value)
        return FsciFrameDescription(0x45, 0x02, cmdParams)

    def InitGATTDBReadAttributeRequest(self):
        cmdParams = []
        Handle = FsciParameter("Handle", 2)
        cmdParams.append(Handle)
        MaxBytes = FsciParameter("MaxBytes", 2)
        cmdParams.append(MaxBytes)
        return FsciFrameDescription(0x45, 0x03, cmdParams)

    def InitGATTDBFindServiceHandleRequest(self):
        cmdParams = []
        StartHandle = FsciParameter("StartHandle", 2)
        cmdParams.append(StartHandle)
        UuidType = FsciParameter("UuidType", 1)
        cmdParams.append(UuidType)
        Uuiddict = {}
        currentList = []
        UuidUuid16Bits = FsciParameter("UuidUuid16Bits", 2)
        currentList.append(UuidUuid16Bits)
        Uuiddict[0x01] = currentList
        currentList = []
        UuidUuid128Bits = FsciParameter("UuidUuid128Bits", 16)
        currentList.append(UuidUuid128Bits)
        Uuiddict[0x02] = currentList
        currentList = []
        UuidUuid32Bits = FsciParameter("UuidUuid32Bits", 4)
        currentList.append(UuidUuid32Bits)
        Uuiddict[0x03] = currentList
        Uuid = FsciParameter("Uuid", 1, UuidType, Uuiddict)
        cmdParams.append(Uuid)
        return FsciFrameDescription(0x45, 0x04, cmdParams)

    def InitGATTDBFindCharValueHandleInServiceRequest(self):
        cmdParams = []
        ServiceHandle = FsciParameter("ServiceHandle", 2)
        cmdParams.append(ServiceHandle)
        UuidType = FsciParameter("UuidType", 1)
        cmdParams.append(UuidType)
        Uuiddict = {}
        currentList = []
        UuidUuid16Bits = FsciParameter("UuidUuid16Bits", 2)
        currentList.append(UuidUuid16Bits)
        Uuiddict[0x01] = currentList
        currentList = []
        UuidUuid128Bits = FsciParameter("UuidUuid128Bits", 16)
        currentList.append(UuidUuid128Bits)
        Uuiddict[0x02] = currentList
        currentList = []
        UuidUuid32Bits = FsciParameter("UuidUuid32Bits", 4)
        currentList.append(UuidUuid32Bits)
        Uuiddict[0x03] = currentList
        Uuid = FsciParameter("Uuid", 1, UuidType, Uuiddict)
        cmdParams.append(Uuid)
        return FsciFrameDescription(0x45, 0x05, cmdParams)

    def InitGATTDBFindCccdHandleForCharValueHandleRequest(self):
        cmdParams = []
        CharValueHandle = FsciParameter("CharValueHandle", 2)
        cmdParams.append(CharValueHandle)
        return FsciFrameDescription(0x45, 0x06, cmdParams)

    def InitGATTDBFindDescriptorHandleForCharValueHandleRequest(self):
        cmdParams = []
        CharValueHandle = FsciParameter("CharValueHandle", 2)
        cmdParams.append(CharValueHandle)
        UuidType = FsciParameter("UuidType", 1)
        cmdParams.append(UuidType)
        Uuiddict = {}
        currentList = []
        UuidUuid16Bits = FsciParameter("UuidUuid16Bits", 2)
        currentList.append(UuidUuid16Bits)
        Uuiddict[0x01] = currentList
        currentList = []
        UuidUuid128Bits = FsciParameter("UuidUuid128Bits", 16)
        currentList.append(UuidUuid128Bits)
        Uuiddict[0x02] = currentList
        currentList = []
        UuidUuid32Bits = FsciParameter("UuidUuid32Bits", 4)
        currentList.append(UuidUuid32Bits)
        Uuiddict[0x03] = currentList
        Uuid = FsciParameter("Uuid", 1, UuidType, Uuiddict)
        cmdParams.append(Uuid)
        return FsciFrameDescription(0x45, 0x07, cmdParams)

    def InitGATTDBDynamicInitRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x45, 0x08, cmdParams)

    def InitGATTDBDynamicReleaseDatabaseRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x45, 0x09, cmdParams)

    def InitGATTDBDynamicAddPrimaryServiceDeclarationRequest(self):
        cmdParams = []
        UuidType = FsciParameter("UuidType", 1)
        cmdParams.append(UuidType)
        Uuiddict = {}
        currentList = []
        UuidUuid16Bits = FsciParameter("UuidUuid16Bits", 2)
        currentList.append(UuidUuid16Bits)
        Uuiddict[0x01] = currentList
        currentList = []
        UuidUuid128Bits = FsciParameter("UuidUuid128Bits", 16)
        currentList.append(UuidUuid128Bits)
        Uuiddict[0x02] = currentList
        currentList = []
        UuidUuid32Bits = FsciParameter("UuidUuid32Bits", 4)
        currentList.append(UuidUuid32Bits)
        Uuiddict[0x03] = currentList
        Uuid = FsciParameter("Uuid", 1, UuidType, Uuiddict)
        cmdParams.append(Uuid)
        return FsciFrameDescription(0x45, 0x0A, cmdParams)

    def InitGATTDBDynamicAddSecondaryServiceDeclarationRequest(self):
        cmdParams = []
        UuidType = FsciParameter("UuidType", 1)
        cmdParams.append(UuidType)
        Uuiddict = {}
        currentList = []
        UuidUuid16Bits = FsciParameter("UuidUuid16Bits", 2)
        currentList.append(UuidUuid16Bits)
        Uuiddict[0x01] = currentList
        currentList = []
        UuidUuid128Bits = FsciParameter("UuidUuid128Bits", 16)
        currentList.append(UuidUuid128Bits)
        Uuiddict[0x02] = currentList
        currentList = []
        UuidUuid32Bits = FsciParameter("UuidUuid32Bits", 4)
        currentList.append(UuidUuid32Bits)
        Uuiddict[0x03] = currentList
        Uuid = FsciParameter("Uuid", 1, UuidType, Uuiddict)
        cmdParams.append(Uuid)
        return FsciFrameDescription(0x45, 0x0B, cmdParams)

    def InitGATTDBDynamicAddIncludeDeclarationRequest(self):
        cmdParams = []
        IncludedServiceHandle = FsciParameter("IncludedServiceHandle", 2)
        cmdParams.append(IncludedServiceHandle)
        EndGroupHandle = FsciParameter("EndGroupHandle", 2)
        cmdParams.append(EndGroupHandle)
        UuidType = FsciParameter("UuidType", 1)
        cmdParams.append(UuidType)
        Uuiddict = {}
        currentList = []
        UuidUuid16Bits = FsciParameter("UuidUuid16Bits", 2)
        currentList.append(UuidUuid16Bits)
        Uuiddict[0x01] = currentList
        currentList = []
        UuidUuid128Bits = FsciParameter("UuidUuid128Bits", 16)
        currentList.append(UuidUuid128Bits)
        Uuiddict[0x02] = currentList
        currentList = []
        UuidUuid32Bits = FsciParameter("UuidUuid32Bits", 4)
        currentList.append(UuidUuid32Bits)
        Uuiddict[0x03] = currentList
        Uuid = FsciParameter("Uuid", 1, UuidType, Uuiddict)
        cmdParams.append(Uuid)
        return FsciFrameDescription(0x45, 0x0C, cmdParams)

    def InitGATTDBDynamicAddCharacteristicDeclarationAndValueRequest(self):
        cmdParams = []
        UuidType = FsciParameter("UuidType", 1)
        cmdParams.append(UuidType)
        Uuiddict = {}
        currentList = []
        UuidUuid16Bits = FsciParameter("UuidUuid16Bits", 2)
        currentList.append(UuidUuid16Bits)
        Uuiddict[0x01] = currentList
        currentList = []
        UuidUuid128Bits = FsciParameter("UuidUuid128Bits", 16)
        currentList.append(UuidUuid128Bits)
        Uuiddict[0x02] = currentList
        currentList = []
        UuidUuid32Bits = FsciParameter("UuidUuid32Bits", 4)
        currentList.append(UuidUuid32Bits)
        Uuiddict[0x03] = currentList
        Uuid = FsciParameter("Uuid", 1, UuidType, Uuiddict)
        cmdParams.append(Uuid)
        CharacteristicProperties = FsciParameter("CharacteristicProperties", 1)
        cmdParams.append(CharacteristicProperties)
        MaxValueLength = FsciParameter("MaxValueLength", 2)
        cmdParams.append(MaxValueLength)
        InitialValueLength = FsciParameter("InitialValueLength", 2)
        cmdParams.append(InitialValueLength)
        InitialValue = FsciParameter("InitialValue", 1, InitialValueLength)
        cmdParams.append(InitialValue)
        ValueAccessPermissions = FsciParameter("ValueAccessPermissions", 1)
        cmdParams.append(ValueAccessPermissions)
        return FsciFrameDescription(0x45, 0x0D, cmdParams)

    def InitGATTDBDynamicAddCharacteristicDescriptorRequest(self):
        cmdParams = []
        UuidType = FsciParameter("UuidType", 1)
        cmdParams.append(UuidType)
        Uuiddict = {}
        currentList = []
        UuidUuid16Bits = FsciParameter("UuidUuid16Bits", 2)
        currentList.append(UuidUuid16Bits)
        Uuiddict[0x01] = currentList
        currentList = []
        UuidUuid128Bits = FsciParameter("UuidUuid128Bits", 16)
        currentList.append(UuidUuid128Bits)
        Uuiddict[0x02] = currentList
        currentList = []
        UuidUuid32Bits = FsciParameter("UuidUuid32Bits", 4)
        currentList.append(UuidUuid32Bits)
        Uuiddict[0x03] = currentList
        Uuid = FsciParameter("Uuid", 1, UuidType, Uuiddict)
        cmdParams.append(Uuid)
        DescriptorValueLength = FsciParameter("DescriptorValueLength", 2)
        cmdParams.append(DescriptorValueLength)
        Value = FsciParameter("Value", 1, DescriptorValueLength)
        cmdParams.append(Value)
        DescriptorAccessPermissions = FsciParameter("DescriptorAccessPermissions", 1)
        cmdParams.append(DescriptorAccessPermissions)
        return FsciFrameDescription(0x45, 0x0E, cmdParams)

    def InitGATTDBDynamicAddCccdRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x45, 0x0F, cmdParams)

    def InitGATTDBDynamicAddCharacteristicDeclarationWithUniqueValueRequest(self):
        cmdParams = []
        UuidType = FsciParameter("UuidType", 1)
        cmdParams.append(UuidType)
        Uuiddict = {}
        currentList = []
        UuidUuid16Bits = FsciParameter("UuidUuid16Bits", 2)
        currentList.append(UuidUuid16Bits)
        Uuiddict[0x01] = currentList
        currentList = []
        UuidUuid128Bits = FsciParameter("UuidUuid128Bits", 16)
        currentList.append(UuidUuid128Bits)
        Uuiddict[0x02] = currentList
        currentList = []
        UuidUuid32Bits = FsciParameter("UuidUuid32Bits", 4)
        currentList.append(UuidUuid32Bits)
        Uuiddict[0x03] = currentList
        Uuid = FsciParameter("Uuid", 1, UuidType, Uuiddict)
        cmdParams.append(Uuid)
        CharacteristicProperties = FsciParameter("CharacteristicProperties", 1)
        cmdParams.append(CharacteristicProperties)
        ValueAccessPermissions = FsciParameter("ValueAccessPermissions", 1)
        cmdParams.append(ValueAccessPermissions)
        return FsciFrameDescription(0x45, 0x10, cmdParams)

    def InitGATTDBDynamicRemoveServiceRequest(self):
        cmdParams = []
        ServiceHandle = FsciParameter("ServiceHandle", 2)
        cmdParams.append(ServiceHandle)
        return FsciFrameDescription(0x45, 0x11, cmdParams)

    def InitGATTDBDynamicRemoveCharacteristicRequest(self):
        cmdParams = []
        CharacteristicHandle = FsciParameter("CharacteristicHandle", 2)
        cmdParams.append(CharacteristicHandle)
        return FsciFrameDescription(0x45, 0x12, cmdParams)

    def InitGATTDBAttModeSelectRequest(self):
        cmdParams = []
        Enable = FsciParameter("Enable", 1)
        cmdParams.append(Enable)
        return FsciFrameDescription(0x46, 0x00, cmdParams)

    def InitGATTDBAttFindInformationRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_StartingHandle = FsciParameter("Params_StartingHandle", 2)
        cmdParams.append(Params_StartingHandle)
        Params_EndingHandle = FsciParameter("Params_EndingHandle", 2)
        cmdParams.append(Params_EndingHandle)
        return FsciFrameDescription(0x46, 0x01, cmdParams)

    def InitGATTDBAttFindByTypeValueRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_StartingHandle = FsciParameter("Params_StartingHandle", 2)
        cmdParams.append(Params_StartingHandle)
        Params_EndingHandle = FsciParameter("Params_EndingHandle", 2)
        cmdParams.append(Params_EndingHandle)
        Params_AttributeType = FsciParameter("Params_AttributeType", 2)
        cmdParams.append(Params_AttributeType)
        Params_AttributeLength = FsciParameter("Params_AttributeLength", 2)
        cmdParams.append(Params_AttributeLength)
        Params_AttributeValue = FsciParameter("Params_AttributeValue", 1, Params_AttributeLength)
        cmdParams.append(Params_AttributeValue)
        return FsciFrameDescription(0x46, 0x02, cmdParams)

    def InitGATTDBAttReadByTypeRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_StartingHandle = FsciParameter("Params_StartingHandle", 2)
        cmdParams.append(Params_StartingHandle)
        Params_EndingHandle = FsciParameter("Params_EndingHandle", 2)
        cmdParams.append(Params_EndingHandle)
        Params_Format = FsciParameter("Params_Format", 1)
        cmdParams.append(Params_Format)
        Params_AttributeTypedict = {}
        currentList = []
        Params_AttributeTypeUuid16BitFormat = FsciParameter("Params_AttributeTypeUuid16BitFormat", 2)
        currentList.append(Params_AttributeTypeUuid16BitFormat)
        Params_AttributeTypedict[0x01] = currentList
        currentList = []
        Params_AttributeTypeUuid128BitFormat = FsciParameter("Params_AttributeTypeUuid128BitFormat", 16)
        currentList.append(Params_AttributeTypeUuid128BitFormat)
        Params_AttributeTypedict[0x02] = currentList
        Params_AttributeType = FsciParameter("Params_AttributeType", 1, Params_Format, Params_AttributeTypedict)
        cmdParams.append(Params_AttributeType)
        return FsciFrameDescription(0x46, 0x03, cmdParams)

    def InitGATTDBAttReadRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_AttributeHandle = FsciParameter("Params_AttributeHandle", 2)
        cmdParams.append(Params_AttributeHandle)
        return FsciFrameDescription(0x46, 0x04, cmdParams)

    def InitGATTDBAttReadBlobRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_AttributeHandle = FsciParameter("Params_AttributeHandle", 2)
        cmdParams.append(Params_AttributeHandle)
        Params_ValueOffset = FsciParameter("Params_ValueOffset", 2)
        cmdParams.append(Params_ValueOffset)
        return FsciFrameDescription(0x46, 0x05, cmdParams)

    def InitGATTDBAttReadMultipleRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_HandleCount = FsciParameter("Params_HandleCount", 2)
        cmdParams.append(Params_HandleCount)
        Params_ListOfHandlesHandle = FsciParameter("Params_ListOfHandlesHandle", 2, Params_HandleCount)
        cmdParams.append(Params_ListOfHandlesHandle)
        return FsciFrameDescription(0x46, 0x06, cmdParams)

    def InitGATTDBAttReadByGroupTypeRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_StartingHandle = FsciParameter("Params_StartingHandle", 2)
        cmdParams.append(Params_StartingHandle)
        Params_EndingHandle = FsciParameter("Params_EndingHandle", 2)
        cmdParams.append(Params_EndingHandle)
        Params_Format = FsciParameter("Params_Format", 1)
        cmdParams.append(Params_Format)
        Params_AttributeTypedict = {}
        currentList = []
        Params_AttributeTypeUuid16BitFormat = FsciParameter("Params_AttributeTypeUuid16BitFormat", 2)
        currentList.append(Params_AttributeTypeUuid16BitFormat)
        Params_AttributeTypedict[0x01] = currentList
        currentList = []
        Params_AttributeTypeUuid128BitFormat = FsciParameter("Params_AttributeTypeUuid128BitFormat", 16)
        currentList.append(Params_AttributeTypeUuid128BitFormat)
        Params_AttributeTypedict[0x02] = currentList
        Params_AttributeType = FsciParameter("Params_AttributeType", 1, Params_Format, Params_AttributeTypedict)
        cmdParams.append(Params_AttributeType)
        return FsciFrameDescription(0x46, 0x07, cmdParams)

    def InitGATTDBAttWriteRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_AttributeHandle = FsciParameter("Params_AttributeHandle", 2)
        cmdParams.append(Params_AttributeHandle)
        Params_AttributeLength = FsciParameter("Params_AttributeLength", 2)
        cmdParams.append(Params_AttributeLength)
        Params_AttributeValue = FsciParameter("Params_AttributeValue", 1, Params_AttributeLength)
        cmdParams.append(Params_AttributeValue)
        return FsciFrameDescription(0x46, 0x08, cmdParams)

    def InitGATTDBAttWriteCommandRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_AttributeHandle = FsciParameter("Params_AttributeHandle", 2)
        cmdParams.append(Params_AttributeHandle)
        Params_AttributeLength = FsciParameter("Params_AttributeLength", 2)
        cmdParams.append(Params_AttributeLength)
        Params_AttributeValue = FsciParameter("Params_AttributeValue", 1, Params_AttributeLength)
        cmdParams.append(Params_AttributeValue)
        return FsciFrameDescription(0x46, 0x09, cmdParams)

    def InitGATTDBAttSignedWriteCommandRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_AttributeHandle = FsciParameter("Params_AttributeHandle", 2)
        cmdParams.append(Params_AttributeHandle)
        Params_AttributeLength = FsciParameter("Params_AttributeLength", 2)
        cmdParams.append(Params_AttributeLength)
        Params_AttributeValue = FsciParameter("Params_AttributeValue", 1, Params_AttributeLength)
        cmdParams.append(Params_AttributeValue)
        Params_AuthenticationSignature = FsciParameter("Params_AuthenticationSignature", 12)
        cmdParams.append(Params_AuthenticationSignature)
        return FsciFrameDescription(0x46, 0x0A, cmdParams)

    def InitGATTDBAttPrepareWriteRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_AttributeHandle = FsciParameter("Params_AttributeHandle", 2)
        cmdParams.append(Params_AttributeHandle)
        Params_ValueOffset = FsciParameter("Params_ValueOffset", 2)
        cmdParams.append(Params_ValueOffset)
        Params_AttributeLength = FsciParameter("Params_AttributeLength", 2)
        cmdParams.append(Params_AttributeLength)
        Params_AttributeValue = FsciParameter("Params_AttributeValue", 1, Params_AttributeLength)
        cmdParams.append(Params_AttributeValue)
        return FsciFrameDescription(0x46, 0x0B, cmdParams)

    def InitGATTDBAttExecuteWriteRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_Flags = FsciParameter("Params_Flags", 1)
        cmdParams.append(Params_Flags)
        return FsciFrameDescription(0x46, 0x0C, cmdParams)

    def InitGATTDBAttExecuteWriteFromQueueRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_AttributeHandle = FsciParameter("Params_AttributeHandle", 2)
        cmdParams.append(Params_AttributeHandle)
        Params_ValueOffset = FsciParameter("Params_ValueOffset", 2)
        cmdParams.append(Params_ValueOffset)
        Params_AttributeLength = FsciParameter("Params_AttributeLength", 2)
        cmdParams.append(Params_AttributeLength)
        Params_AttributeValue = FsciParameter("Params_AttributeValue", 1, Params_AttributeLength)
        cmdParams.append(Params_AttributeValue)
        return FsciFrameDescription(0x46, 0x0D, cmdParams)

    def InitGATTDBAttPrepareNotificationIndicationRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_AttributeHandle = FsciParameter("Params_AttributeHandle", 2)
        cmdParams.append(Params_AttributeHandle)
        Params_AttributeLength = FsciParameter("Params_AttributeLength", 2)
        cmdParams.append(Params_AttributeLength)
        Params_AttributeValue = FsciParameter("Params_AttributeValue", 1, Params_AttributeLength)
        cmdParams.append(Params_AttributeValue)
        return FsciFrameDescription(0x46, 0x0E, cmdParams)

    def InitGAPModeSelectRequest(self):
        cmdParams = []
        Enable = FsciParameter("Enable", 1)
        cmdParams.append(Enable)
        return FsciFrameDescription(0x47, 0x00, cmdParams)

    def InitBLEHostInitializeRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x47, 0x01, cmdParams)

    def InitGAPRegisterDeviceSecurityRequirementsRequest(self):
        cmdParams = []
        # not generated, pickle() is used instead; see frames.py
        return FsciFrameDescription(0x47, 0x02, cmdParams)

    def InitGAPSetAdvertisingParametersRequest(self):
        cmdParams = []
        AdvertisingParameters_MinInterval = FsciParameter("AdvertisingParameters_MinInterval", 2)
        cmdParams.append(AdvertisingParameters_MinInterval)
        AdvertisingParameters_MaxInterval = FsciParameter("AdvertisingParameters_MaxInterval", 2)
        cmdParams.append(AdvertisingParameters_MaxInterval)
        AdvertisingParameters_AdvertisingType = FsciParameter("AdvertisingParameters_AdvertisingType", 1)
        cmdParams.append(AdvertisingParameters_AdvertisingType)
        AdvertisingParameters_OwnAddressType = FsciParameter("AdvertisingParameters_OwnAddressType", 1)
        cmdParams.append(AdvertisingParameters_OwnAddressType)
        AdvertisingParameters_PeerAddressType = FsciParameter("AdvertisingParameters_PeerAddressType", 1)
        cmdParams.append(AdvertisingParameters_PeerAddressType)
        AdvertisingParameters_PeerAddress = FsciParameter("AdvertisingParameters_PeerAddress", 6)
        cmdParams.append(AdvertisingParameters_PeerAddress)
        AdvertisingParameters_ChannelMap = FsciParameter("AdvertisingParameters_ChannelMap", 1)
        cmdParams.append(AdvertisingParameters_ChannelMap)
        AdvertisingParameters_FilterPolicy = FsciParameter("AdvertisingParameters_FilterPolicy", 1)
        cmdParams.append(AdvertisingParameters_FilterPolicy)
        return FsciFrameDescription(0x47, 0x03, cmdParams)

    def InitGAPSetAdvertisingDataRequest(self):
        cmdParams = []
        # not generated, pickle() is used instead; see frames.py
        return FsciFrameDescription(0x47, 0x04, cmdParams)

    def InitGAPStartAdvertisingRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x47, 0x05, cmdParams)

    def InitGAPStopAdvertisingRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x47, 0x06, cmdParams)

    def InitGAPAuthorizeRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Handle = FsciParameter("Handle", 2)
        cmdParams.append(Handle)
        Access = FsciParameter("Access", 1)
        cmdParams.append(Access)
        return FsciFrameDescription(0x47, 0x07, cmdParams)

    def InitGAPSaveCccdRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Handle = FsciParameter("Handle", 2)
        cmdParams.append(Handle)
        Cccd = FsciParameter("Cccd", 1)
        cmdParams.append(Cccd)
        return FsciFrameDescription(0x47, 0x08, cmdParams)

    def InitGAPCheckNotificationStatusRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Handle = FsciParameter("Handle", 2)
        cmdParams.append(Handle)
        return FsciFrameDescription(0x47, 0x09, cmdParams)

    def InitGAPCheckIndicationStatusRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Handle = FsciParameter("Handle", 2)
        cmdParams.append(Handle)
        return FsciFrameDescription(0x47, 0x0A, cmdParams)

    def InitGAPGetBondedStaticAddressesRequest(self):
        cmdParams = []
        MaxNbOfDevices = FsciParameter("MaxNbOfDevices", 1)
        cmdParams.append(MaxNbOfDevices)
        return FsciFrameDescription(0x47, 0x0B, cmdParams)

    def InitGAPPairRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        PairingParameters_WithBonding = FsciParameter("PairingParameters_WithBonding", 1)
        cmdParams.append(PairingParameters_WithBonding)
        PairingParameters_SecurityModeAndLevel = FsciParameter("PairingParameters_SecurityModeAndLevel", 1)
        cmdParams.append(PairingParameters_SecurityModeAndLevel)
        PairingParameters_MaxEncryptionKeySize = FsciParameter("PairingParameters_MaxEncryptionKeySize", 1)
        cmdParams.append(PairingParameters_MaxEncryptionKeySize)
        PairingParameters_LocalIoCapabilities = FsciParameter("PairingParameters_LocalIoCapabilities", 1)
        cmdParams.append(PairingParameters_LocalIoCapabilities)
        PairingParameters_OobAvailable = FsciParameter("PairingParameters_OobAvailable", 1)
        cmdParams.append(PairingParameters_OobAvailable)
        PairingParameters_CentralKeys = FsciParameter("PairingParameters_CentralKeys", 1)
        cmdParams.append(PairingParameters_CentralKeys)
        PairingParameters_PeripheralKeys = FsciParameter("PairingParameters_PeripheralKeys", 1)
        cmdParams.append(PairingParameters_PeripheralKeys)
        PairingParameters_LeSecureConnectionSupported = FsciParameter("PairingParameters_LeSecureConnectionSupported", 1)
        cmdParams.append(PairingParameters_LeSecureConnectionSupported)
        PairingParameters_UseKeypressNotifications = FsciParameter("PairingParameters_UseKeypressNotifications", 1)
        cmdParams.append(PairingParameters_UseKeypressNotifications)
        return FsciFrameDescription(0x47, 0x0C, cmdParams)

    def InitGAPSendSlaveSecurityRequestRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        BondAfterPairing = FsciParameter("BondAfterPairing", 1)
        cmdParams.append(BondAfterPairing)
        SecurityModeLevel = FsciParameter("SecurityModeLevel", 1)
        cmdParams.append(SecurityModeLevel)
        return FsciFrameDescription(0x47, 0x0D, cmdParams)

    def InitGAPEncryptLinkRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        return FsciFrameDescription(0x47, 0x0E, cmdParams)

    def InitGAPAcceptPairingRequestRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        PairingParameters_WithBonding = FsciParameter("PairingParameters_WithBonding", 1)
        cmdParams.append(PairingParameters_WithBonding)
        PairingParameters_SecurityModeAndLevel = FsciParameter("PairingParameters_SecurityModeAndLevel", 1)
        cmdParams.append(PairingParameters_SecurityModeAndLevel)
        PairingParameters_MaxEncryptionKeySize = FsciParameter("PairingParameters_MaxEncryptionKeySize", 1)
        cmdParams.append(PairingParameters_MaxEncryptionKeySize)
        PairingParameters_LocalIoCapabilities = FsciParameter("PairingParameters_LocalIoCapabilities", 1)
        cmdParams.append(PairingParameters_LocalIoCapabilities)
        PairingParameters_OobAvailable = FsciParameter("PairingParameters_OobAvailable", 1)
        cmdParams.append(PairingParameters_OobAvailable)
        PairingParameters_CentralKeys = FsciParameter("PairingParameters_CentralKeys", 1)
        cmdParams.append(PairingParameters_CentralKeys)
        PairingParameters_PeripheralKeys = FsciParameter("PairingParameters_PeripheralKeys", 1)
        cmdParams.append(PairingParameters_PeripheralKeys)
        PairingParameters_LeSecureConnectionSupported = FsciParameter("PairingParameters_LeSecureConnectionSupported", 1)
        cmdParams.append(PairingParameters_LeSecureConnectionSupported)
        PairingParameters_UseKeypressNotifications = FsciParameter("PairingParameters_UseKeypressNotifications", 1)
        cmdParams.append(PairingParameters_UseKeypressNotifications)
        return FsciFrameDescription(0x47, 0x0F, cmdParams)

    def InitGAPRejectPairingRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Reason = FsciParameter("Reason", 1)
        cmdParams.append(Reason)
        return FsciFrameDescription(0x47, 0x10, cmdParams)

    def InitGAPEnterPasskeyRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Passkey = FsciParameter("Passkey", 4)
        cmdParams.append(Passkey)
        return FsciFrameDescription(0x47, 0x11, cmdParams)

    def InitGAPProvideOobRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Oob = FsciParameter("Oob", 16)
        cmdParams.append(Oob)
        return FsciFrameDescription(0x47, 0x12, cmdParams)

    def InitGAPRejectPasskeyRequestRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        return FsciFrameDescription(0x47, 0x13, cmdParams)

    def InitGAPSendSmpKeysRequest(self):
        cmdParams = []
        # not generated, pickle() is used instead; see frames.py
        return FsciFrameDescription(0x47, 0x14, cmdParams)

    def InitGAPRejectKeyExchangeRequestRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        return FsciFrameDescription(0x47, 0x15, cmdParams)

    def InitGAPProvideLongTermKeyRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        LtkSize = FsciParameter("LtkSize", 1)
        cmdParams.append(LtkSize)
        Ltk = FsciParameter("Ltk", 1, LtkSize)
        cmdParams.append(Ltk)
        return FsciFrameDescription(0x47, 0x16, cmdParams)

    def InitGAPDenyLongTermKeyRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        return FsciFrameDescription(0x47, 0x17, cmdParams)

    def InitGAPLoadEncryptionInformationRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        return FsciFrameDescription(0x47, 0x18, cmdParams)

    def InitGAPSetLocalPasskeyRequest(self):
        cmdParams = []
        Passkey = FsciParameter("Passkey", 4)
        cmdParams.append(Passkey)
        return FsciFrameDescription(0x47, 0x19, cmdParams)

    def InitGAPStartScanningRequest(self):
        cmdParams = []
        # not generated, pickle() is used instead; see frames.py
        return FsciFrameDescription(0x47, 0x1A, cmdParams)

    def InitGAPStopScanningRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x47, 0x1B, cmdParams)

    def InitGAPConnectRequest(self):
        cmdParams = []
        CreateConnectionParameters_ScanInterval = FsciParameter("CreateConnectionParameters_ScanInterval", 2)
        cmdParams.append(CreateConnectionParameters_ScanInterval)
        CreateConnectionParameters_ScanWindow = FsciParameter("CreateConnectionParameters_ScanWindow", 2)
        cmdParams.append(CreateConnectionParameters_ScanWindow)
        CreateConnectionParameters_FilterPolicy = FsciParameter("CreateConnectionParameters_FilterPolicy", 1)
        cmdParams.append(CreateConnectionParameters_FilterPolicy)
        CreateConnectionParameters_OwnAddressType = FsciParameter("CreateConnectionParameters_OwnAddressType", 1)
        cmdParams.append(CreateConnectionParameters_OwnAddressType)
        CreateConnectionParameters_PeerAddressType = FsciParameter("CreateConnectionParameters_PeerAddressType", 1)
        cmdParams.append(CreateConnectionParameters_PeerAddressType)
        CreateConnectionParameters_PeerAddress = FsciParameter("CreateConnectionParameters_PeerAddress", 6)
        cmdParams.append(CreateConnectionParameters_PeerAddress)
        CreateConnectionParameters_ConnIntervalMin = FsciParameter("CreateConnectionParameters_ConnIntervalMin", 2)
        cmdParams.append(CreateConnectionParameters_ConnIntervalMin)
        CreateConnectionParameters_ConnIntervalMax = FsciParameter("CreateConnectionParameters_ConnIntervalMax", 2)
        cmdParams.append(CreateConnectionParameters_ConnIntervalMax)
        CreateConnectionParameters_ConnLatency = FsciParameter("CreateConnectionParameters_ConnLatency", 2)
        cmdParams.append(CreateConnectionParameters_ConnLatency)
        CreateConnectionParameters_SupervisionTimeout = FsciParameter("CreateConnectionParameters_SupervisionTimeout", 2)
        cmdParams.append(CreateConnectionParameters_SupervisionTimeout)
        CreateConnectionParameters_ConnEventLengthMin = FsciParameter("CreateConnectionParameters_ConnEventLengthMin", 2)
        cmdParams.append(CreateConnectionParameters_ConnEventLengthMin)
        CreateConnectionParameters_ConnEventLengthMax = FsciParameter("CreateConnectionParameters_ConnEventLengthMax", 2)
        cmdParams.append(CreateConnectionParameters_ConnEventLengthMax)
        CreateConnectionParameters_usePeerIdentityAddress = FsciParameter("CreateConnectionParameters_usePeerIdentityAddress", 1)
        cmdParams.append(CreateConnectionParameters_usePeerIdentityAddress)
        return FsciFrameDescription(0x47, 0x1C, cmdParams)

    def InitGAPDisconnectRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        return FsciFrameDescription(0x47, 0x1D, cmdParams)

    def InitGAPSaveCustomPeerInformationRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Offset = FsciParameter("Offset", 2)
        cmdParams.append(Offset)
        InfoSize = FsciParameter("InfoSize", 2)
        cmdParams.append(InfoSize)
        Info = FsciParameter("Info", 1, InfoSize)
        cmdParams.append(Info)
        return FsciFrameDescription(0x47, 0x1E, cmdParams)

    def InitGAPLoadCustomPeerInformationRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Offset = FsciParameter("Offset", 2)
        cmdParams.append(Offset)
        InfoSize = FsciParameter("InfoSize", 2)
        cmdParams.append(InfoSize)
        return FsciFrameDescription(0x47, 0x1F, cmdParams)

    def InitGAPCheckIfBondedRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        return FsciFrameDescription(0x47, 0x20, cmdParams)

    def InitGAPReadWhiteListSizeRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x47, 0x21, cmdParams)

    def InitGAPClearWhiteListRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x47, 0x22, cmdParams)

    def InitGAPAddDeviceToWhiteListRequest(self):
        cmdParams = []
        AddressType = FsciParameter("AddressType", 1)
        cmdParams.append(AddressType)
        Address = FsciParameter("Address", 6)
        cmdParams.append(Address)
        return FsciFrameDescription(0x47, 0x23, cmdParams)

    def InitGAPRemoveDeviceFromWhiteListRequest(self):
        cmdParams = []
        AddressType = FsciParameter("AddressType", 1)
        cmdParams.append(AddressType)
        Address = FsciParameter("Address", 6)
        cmdParams.append(Address)
        return FsciFrameDescription(0x47, 0x24, cmdParams)

    def InitGAPReadPublicDeviceAddressRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x47, 0x25, cmdParams)

    def InitGAPCreateRandomDeviceAddressRequest(self):
        cmdParams = []
        IrkIncluded = FsciParameter("IrkIncluded", 1)
        cmdParams.append(IrkIncluded)
        Irk = FsciParameter("Irk", 16, IrkIncluded)
        cmdParams.append(Irk)
        RandomPartIncluded = FsciParameter("RandomPartIncluded", 1, IrkIncluded)
        cmdParams.append(RandomPartIncluded)
        RandomPart = FsciParameter("RandomPart", 3, RandomPartIncluded)
        cmdParams.append(RandomPart)
        return FsciFrameDescription(0x47, 0x26, cmdParams)

    def InitGAPSaveDeviceNameRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        NameSize = FsciParameter("NameSize", 1)
        cmdParams.append(NameSize)
        Name = FsciParameter("Name", 1, NameSize)
        cmdParams.append(Name)
        return FsciFrameDescription(0x47, 0x27, cmdParams)

    def InitGAPGetBondedDevicesCountRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x47, 0x28, cmdParams)

    def InitGAPGetBondedDeviceNameRequest(self):
        cmdParams = []
        NvmIndex = FsciParameter("NvmIndex", 1)
        cmdParams.append(NvmIndex)
        MaxNameSize = FsciParameter("MaxNameSize", 1)
        cmdParams.append(MaxNameSize)
        return FsciFrameDescription(0x47, 0x29, cmdParams)

    def InitGAPRemoveBondRequest(self):
        cmdParams = []
        NvmIndex = FsciParameter("NvmIndex", 1)
        cmdParams.append(NvmIndex)
        return FsciFrameDescription(0x47, 0x2A, cmdParams)

    def InitGAPRemoveAllBondsRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x47, 0x2B, cmdParams)

    def InitGAPReadRadioPowerLevelRequest(self):
        cmdParams = []
        TxReadType = FsciParameter("TxReadType", 1)
        cmdParams.append(TxReadType)
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        return FsciFrameDescription(0x47, 0x2C, cmdParams)

    def InitGAPVerifyPrivateResolvableAddressRequest(self):
        cmdParams = []
        NvmIndex = FsciParameter("NvmIndex", 1)
        cmdParams.append(NvmIndex)
        Address = FsciParameter("Address", 6)
        cmdParams.append(Address)
        return FsciFrameDescription(0x47, 0x2D, cmdParams)

    def InitGAPSetRandomAddressRequest(self):
        cmdParams = []
        Address = FsciParameter("Address", 6)
        cmdParams.append(Address)
        return FsciFrameDescription(0x47, 0x2E, cmdParams)

    def InitGAPSetScanModeRequest(self):
        cmdParams = []
        # not generated, pickle() is used instead; see frames.py
        return FsciFrameDescription(0x47, 0x2F, cmdParams)

    def InitGAPSetDefaultPairingParametersRequest(self):
        cmdParams = []
        # not generated, pickle() is used instead; see frames.py
        return FsciFrameDescription(0x47, 0x30, cmdParams)

    def InitGAPUpdateConnectionParametersRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        IntervalMin = FsciParameter("IntervalMin", 2)
        cmdParams.append(IntervalMin)
        IntervalMax = FsciParameter("IntervalMax", 2)
        cmdParams.append(IntervalMax)
        SlaveLatency = FsciParameter("SlaveLatency", 2)
        cmdParams.append(SlaveLatency)
        TimeoutMultiplier = FsciParameter("TimeoutMultiplier", 2)
        cmdParams.append(TimeoutMultiplier)
        MinCeLength = FsciParameter("MinCeLength", 2)
        cmdParams.append(MinCeLength)
        MaxCeLength = FsciParameter("MaxCeLength", 2)
        cmdParams.append(MaxCeLength)
        return FsciFrameDescription(0x47, 0x31, cmdParams)

    def InitGAPEnableUpdateConnectionParametersRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Enable = FsciParameter("Enable", 1)
        cmdParams.append(Enable)
        return FsciFrameDescription(0x47, 0x32, cmdParams)

    def InitGapUpdateLeDataLengthRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        TxOctets = FsciParameter("TxOctets", 2)
        cmdParams.append(TxOctets)
        TxTime = FsciParameter("TxTime", 2)
        cmdParams.append(TxTime)
        return FsciFrameDescription(0x47, 0x33, cmdParams)

    def InitGAPControllerResetRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x47, 0x34, cmdParams)

    def InitGAPEnableHostPrivacyRequest(self):
        cmdParams = []
        Enable = FsciParameter("Enable", 1)
        cmdParams.append(Enable)
        Irk = FsciParameter("Irk", 16, Enable)
        cmdParams.append(Irk)
        return FsciFrameDescription(0x47, 0x35, cmdParams)

    def InitGAPEnableControllerPrivacyRequest(self):
        cmdParams = []
        # not generated, pickle() is used instead; see frames.py
        return FsciFrameDescription(0x47, 0x36, cmdParams)

    def InitGAPLeScRegeneratePublicKeyRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x47, 0x37, cmdParams)

    def InitGAPLeScValidateNumericValueRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Valid = FsciParameter("Valid", 1)
        cmdParams.append(Valid)
        return FsciFrameDescription(0x47, 0x38, cmdParams)

    def InitGAPLeScGetLocalOobDataRequest(self):
        cmdParams = []
        return FsciFrameDescription(0x47, 0x39, cmdParams)

    def InitGAPLeScSetPeerOobDataRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        RandomValue = FsciParameter("RandomValue", 16)
        cmdParams.append(RandomValue)
        ConfirmValue = FsciParameter("ConfirmValue", 16)
        cmdParams.append(ConfirmValue)
        return FsciFrameDescription(0x47, 0x3A, cmdParams)

    def InitGAPLeScSendKeypressNotificationPrivacyRequest(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        GapLeScKeypressNotificationParams_keypressNotifType = FsciParameter("GapLeScKeypressNotificationParams_keypressNotifType", 1)
        cmdParams.append(GapLeScKeypressNotificationParams_keypressNotifType)
        return FsciFrameDescription(0x47, 0x3B, cmdParams)

    def InitGAPGetBondedDevicesIdentityInformationRequest(self):
        cmdParams = []
        maxDevices = FsciParameter("maxDevices", 1)
        cmdParams.append(maxDevices)
        return FsciFrameDescription(0x47, 0x3C, cmdParams)

    def InitGAPSetTxPowerLevelRequest(self):
        cmdParams = []
        powerLevel = FsciParameter("powerLevel", 1)
        cmdParams.append(powerLevel)
        channelType = FsciParameter("channelType", 1)
        cmdParams.append(channelType)
        return FsciFrameDescription(0x47, 0x3D, cmdParams)

    def InitFSCICPUResetRequest(self):
        cmdParams = []
        return FsciFrameDescription(0xA3, 0x08, cmdParams)

    def InitFSCIAllowDeviceToSleepRequest(self):
        cmdParams = []
        SignalHostWhenWakeUp = FsciParameter("SignalHostWhenWakeUp", 1)
        cmdParams.append(SignalHostWhenWakeUp)
        DeepSleepDuration = FsciParameter("DeepSleepDuration", 4)
        cmdParams.append(DeepSleepDuration)
        DeepSleepMode = FsciParameter("DeepSleepMode", 1)
        cmdParams.append(DeepSleepMode)
        return FsciFrameDescription(0xA3, 0x70, cmdParams)

    def InitFSCIGetWakeupReasonRequest(self):
        cmdParams = []
        return FsciFrameDescription(0xA3, 0x72, cmdParams)

    def InitFSCIAckIndication(self):
        cmdParams = []
        Checksum = FsciParameter("Checksum", 1)
        cmdParams.append(Checksum)
        return FsciFrameDescription(0xA4, 0xFD, cmdParams)

    def InitFSCIErrorIndication(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xA4, 0xFE, cmdParams)

    def InitFSCIAllowDeviceToSleepConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xA4, 0x70, cmdParams)

    def InitFSCIWakeUpIndication(self):
        cmdParams = []
        return FsciFrameDescription(0xA4, 0x71, cmdParams)

    def InitFSCIGetWakeupReasonResponse(self):
        cmdParams = []
        WakeUpReason = FsciParameter("WakeUpReason", 1)
        cmdParams.append(WakeUpReason)
        return FsciFrameDescription(0xA4, 0x72, cmdParams)

    def InitNVMSaveConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xA8, 0xE4, cmdParams)

    def InitNVMGetDataSetDescConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        Count = FsciParameter("Count", 1)
        cmdParams.append(Count)
        CountSizeandID = FsciParameter("CountSizeandID", 7, Count)
        cmdParams.append(CountSizeandID)
        return FsciFrameDescription(0xA8, 0xE5, cmdParams)

    def InitNVMGetCountersConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        nvmFirstVirtualPageEraseCounter = FsciParameter("nvmFirstVirtualPageEraseCounter", 4)
        cmdParams.append(nvmFirstVirtualPageEraseCounter)
        nvmSecondVirtualPageEraseCounter = FsciParameter("nvmSecondVirtualPageEraseCounter", 4)
        cmdParams.append(nvmSecondVirtualPageEraseCounter)
        return FsciFrameDescription(0xA8, 0xE6, cmdParams)

    def InitNVMSetMonitoringConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xA8, 0xE9, cmdParams)

    def InitNVMWriteMonitoringIndication(self):
        cmdParams = []
        nvmDatasetId = FsciParameter("nvmDatasetId", 2)
        cmdParams.append(nvmDatasetId)
        elementId = FsciParameter("elementId", 2)
        cmdParams.append(elementId)
        saveAll = FsciParameter("saveAll", 1)
        cmdParams.append(saveAll)
        return FsciFrameDescription(0xA8, 0xEA, cmdParams)

    def InitNVMPageEraseMonitoringIndication(self):
        cmdParams = []
        nvmFlashErasedPageAddressParmName = FsciParameter("nvmFlashErasedPageAddressParmName", 4)
        cmdParams.append(nvmFlashErasedPageAddressParmName)
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xA8, 0xEB, cmdParams)

    def InitNVMFormatReqConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xA8, 0xEC, cmdParams)

    def InitNVMRestoreReqConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xA8, 0xED, cmdParams)

    def InitNVMRestoreMonitoringIndication(self):
        cmdParams = []
        nvmDatasetId = FsciParameter("nvmDatasetId", 2)
        cmdParams.append(nvmDatasetId)
        Start = FsciParameter("Start", 1)
        cmdParams.append(Start)
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xA8, 0xEE, cmdParams)

    def InitNVMVirtualPageMonitoringIndication(self):
        cmdParams = []
        Start = FsciParameter("Start", 1)
        cmdParams.append(Start)
        Status = FsciParameter("Status", 1)
        cmdParams.append(Status)
        return FsciFrameDescription(0xA8, 0xEF, cmdParams)

    def InitHCIConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 2)
        cmdParams.append(Status)
        return FsciFrameDescription(0x40, 0x80, cmdParams)

    def InitHCIEventIndication(self):
        cmdParams = []
        EventLength = FsciParameter("EventLength", 2)
        cmdParams.append(EventLength)
        Event = FsciParameter("Event", 1, EventLength)
        cmdParams.append(Event)
        return FsciFrameDescription(0x40, 0x81, cmdParams)

    def InitHCIDataIndication(self):
        cmdParams = []
        DataLength = FsciParameter("DataLength", 2)
        cmdParams.append(DataLength)
        Data = FsciParameter("Data", 1, DataLength)
        cmdParams.append(Data)
        return FsciFrameDescription(0x40, 0x82, cmdParams)

    def InitHCISynchronousDataIndication(self):
        cmdParams = []
        SynchronousDataLength = FsciParameter("SynchronousDataLength", 2)
        cmdParams.append(SynchronousDataLength)
        SynchronousData = FsciParameter("SynchronousData", 1, SynchronousDataLength)
        cmdParams.append(SynchronousData)
        return FsciFrameDescription(0x40, 0x83, cmdParams)

    def InitL2CAPConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 2)
        cmdParams.append(Status)
        return FsciFrameDescription(0x41, 0x80, cmdParams)

    def InitL2CAPAttDataIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        PacketLength = FsciParameter("PacketLength", 2)
        cmdParams.append(PacketLength)
        Packet = FsciParameter("Packet", 1, PacketLength)
        cmdParams.append(Packet)
        return FsciFrameDescription(0x41, 0x81, cmdParams)

    def InitL2CAPSmpDataIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        PacketLength = FsciParameter("PacketLength", 2)
        cmdParams.append(PacketLength)
        Packet = FsciParameter("Packet", 1, PacketLength)
        cmdParams.append(Packet)
        return FsciFrameDescription(0x41, 0x82, cmdParams)

    def InitL2CAPSignalingDataIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        PacketLength = FsciParameter("PacketLength", 2)
        cmdParams.append(PacketLength)
        Packet = FsciParameter("Packet", 1, PacketLength)
        cmdParams.append(Packet)
        return FsciFrameDescription(0x41, 0x83, cmdParams)

    def InitL2CAPLePsmConnectionRequestIndication(self):
        cmdParams = []
        # not generated, cursor based approach in observer; see events.py
        return FsciFrameDescription(0x41, 0x84, cmdParams)

    def InitL2CAPLePsmConnectionCompleteIndication(self):
        cmdParams = []
        # not generated, cursor based approach in observer; see events.py
        return FsciFrameDescription(0x41, 0x85, cmdParams)

    def InitL2CAPLePsmDisconnectNotificationIndication(self):
        cmdParams = []
        # not generated, cursor based approach in observer; see events.py
        return FsciFrameDescription(0x41, 0x86, cmdParams)

    def InitL2CAPNoPeerCreditsIndication(self):
        cmdParams = []
        # not generated, cursor based approach in observer; see events.py
        return FsciFrameDescription(0x41, 0x87, cmdParams)

    def InitL2CAPLocalCreditsNotificationIndication(self):
        cmdParams = []
        # not generated, cursor based approach in observer; see events.py
        return FsciFrameDescription(0x41, 0x88, cmdParams)

    def InitL2CAPLeCbDataIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        SrcCid = FsciParameter("SrcCid", 2)
        cmdParams.append(SrcCid)
        PacketLength = FsciParameter("PacketLength", 2)
        cmdParams.append(PacketLength)
        Packet = FsciParameter("Packet", 1, PacketLength)
        cmdParams.append(Packet)
        return FsciFrameDescription(0x41, 0x89, cmdParams)

    def InitSMConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 2)
        cmdParams.append(Status)
        return FsciFrameDescription(0x42, 0x80, cmdParams)

    def InitSMStatusIndication(self):
        cmdParams = []
        SourceDeviceId = FsciParameter("SourceDeviceId", 1)
        cmdParams.append(SourceDeviceId)
        SmStatusParams_Status = FsciParameter("SmStatusParams_Status", 2)
        cmdParams.append(SmStatusParams_Status)
        return FsciFrameDescription(0x42, 0x81, cmdParams)

    def InitSMRemoteSecurityRequestIndication(self):
        cmdParams = []
        SourceDeviceId = FsciParameter("SourceDeviceId", 1)
        cmdParams.append(SourceDeviceId)
        SmSecurityReqParams_AuthRequestBondingFlags = FsciParameter("SmSecurityReqParams_AuthRequestBondingFlags", 1)
        cmdParams.append(SmSecurityReqParams_AuthRequestBondingFlags)
        SmSecurityReqParams_AuthRequestMitm = FsciParameter("SmSecurityReqParams_AuthRequestMitm", 1)
        cmdParams.append(SmSecurityReqParams_AuthRequestMitm)
        SmSecurityReqParams_AuthRequestsc = FsciParameter("SmSecurityReqParams_AuthRequestsc", 1)
        cmdParams.append(SmSecurityReqParams_AuthRequestsc)
        SmSecurityReqParams_AuthRequestkeypress = FsciParameter("SmSecurityReqParams_AuthRequestkeypress", 1)
        cmdParams.append(SmSecurityReqParams_AuthRequestkeypress)
        return FsciFrameDescription(0x42, 0x82, cmdParams)

    def InitSMRemotePairingRequestIndication(self):
        cmdParams = []
        SourceDeviceId = FsciParameter("SourceDeviceId", 1)
        cmdParams.append(SourceDeviceId)
        SmPairingParams_IoCapabilities = FsciParameter("SmPairingParams_IoCapabilities", 1)
        cmdParams.append(SmPairingParams_IoCapabilities)
        SmPairingParams_OobDataFlag = FsciParameter("SmPairingParams_OobDataFlag", 1)
        cmdParams.append(SmPairingParams_OobDataFlag)
        SmPairingParams_AuthRequestBondingFlags = FsciParameter("SmPairingParams_AuthRequestBondingFlags", 1)
        cmdParams.append(SmPairingParams_AuthRequestBondingFlags)
        SmPairingParams_AuthRequestMitm = FsciParameter("SmPairingParams_AuthRequestMitm", 1)
        cmdParams.append(SmPairingParams_AuthRequestMitm)
        SmPairingParams_AuthRequestsc = FsciParameter("SmPairingParams_AuthRequestsc", 1)
        cmdParams.append(SmPairingParams_AuthRequestsc)
        SmPairingParams_AuthRequestkeypress = FsciParameter("SmPairingParams_AuthRequestkeypress", 1)
        cmdParams.append(SmPairingParams_AuthRequestkeypress)
        SmPairingParams_MaxEncKeySize = FsciParameter("SmPairingParams_MaxEncKeySize", 1)
        cmdParams.append(SmPairingParams_MaxEncKeySize)
        SmPairingParams_InitatorKeyDistributionEncKey = FsciParameter("SmPairingParams_InitatorKeyDistributionEncKey", 1)
        cmdParams.append(SmPairingParams_InitatorKeyDistributionEncKey)
        SmPairingParams_InitatorKeyDistributionIdKey = FsciParameter("SmPairingParams_InitatorKeyDistributionIdKey", 1)
        cmdParams.append(SmPairingParams_InitatorKeyDistributionIdKey)
        SmPairingParams_InitatorKeyDistributionSign = FsciParameter("SmPairingParams_InitatorKeyDistributionSign", 1)
        cmdParams.append(SmPairingParams_InitatorKeyDistributionSign)
        SmPairingParams_InitatorKeyDistributionLinkKey = FsciParameter("SmPairingParams_InitatorKeyDistributionLinkKey", 1)
        cmdParams.append(SmPairingParams_InitatorKeyDistributionLinkKey)
        SmPairingParams_InitatorKeyDistributionReserved = FsciParameter("SmPairingParams_InitatorKeyDistributionReserved", 1)
        cmdParams.append(SmPairingParams_InitatorKeyDistributionReserved)
        SmPairingParams_ResponderKeyDistributionEncKey = FsciParameter("SmPairingParams_ResponderKeyDistributionEncKey", 1)
        cmdParams.append(SmPairingParams_ResponderKeyDistributionEncKey)
        SmPairingParams_ResponderKeyDistributionIdKey = FsciParameter("SmPairingParams_ResponderKeyDistributionIdKey", 1)
        cmdParams.append(SmPairingParams_ResponderKeyDistributionIdKey)
        SmPairingParams_ResponderKeyDistributionSign = FsciParameter("SmPairingParams_ResponderKeyDistributionSign", 1)
        cmdParams.append(SmPairingParams_ResponderKeyDistributionSign)
        SmPairingParams_ResponderKeyDistributionLinkKey = FsciParameter("SmPairingParams_ResponderKeyDistributionLinkKey", 1)
        cmdParams.append(SmPairingParams_ResponderKeyDistributionLinkKey)
        SmPairingParams_ResponderKeyDistributionReserved = FsciParameter("SmPairingParams_ResponderKeyDistributionReserved", 1)
        cmdParams.append(SmPairingParams_ResponderKeyDistributionReserved)
        return FsciFrameDescription(0x42, 0x83, cmdParams)

    def InitSMRemotePairingResponseIndication(self):
        cmdParams = []
        SourceDeviceId = FsciParameter("SourceDeviceId", 1)
        cmdParams.append(SourceDeviceId)
        SmPairingParams_IoCapabilities = FsciParameter("SmPairingParams_IoCapabilities", 1)
        cmdParams.append(SmPairingParams_IoCapabilities)
        SmPairingParams_OobDataFlag = FsciParameter("SmPairingParams_OobDataFlag", 1)
        cmdParams.append(SmPairingParams_OobDataFlag)
        SmPairingParams_AuthRequestBondingFlags = FsciParameter("SmPairingParams_AuthRequestBondingFlags", 1)
        cmdParams.append(SmPairingParams_AuthRequestBondingFlags)
        SmPairingParams_AuthRequestMitm = FsciParameter("SmPairingParams_AuthRequestMitm", 1)
        cmdParams.append(SmPairingParams_AuthRequestMitm)
        SmPairingParams_AuthRequestsc = FsciParameter("SmPairingParams_AuthRequestsc", 1)
        cmdParams.append(SmPairingParams_AuthRequestsc)
        SmPairingParams_AuthRequestkeypress = FsciParameter("SmPairingParams_AuthRequestkeypress", 1)
        cmdParams.append(SmPairingParams_AuthRequestkeypress)
        SmPairingParams_MaxEncKeySize = FsciParameter("SmPairingParams_MaxEncKeySize", 1)
        cmdParams.append(SmPairingParams_MaxEncKeySize)
        SmPairingParams_InitatorKeyDistributionEncKey = FsciParameter("SmPairingParams_InitatorKeyDistributionEncKey", 1)
        cmdParams.append(SmPairingParams_InitatorKeyDistributionEncKey)
        SmPairingParams_InitatorKeyDistributionIdKey = FsciParameter("SmPairingParams_InitatorKeyDistributionIdKey", 1)
        cmdParams.append(SmPairingParams_InitatorKeyDistributionIdKey)
        SmPairingParams_InitatorKeyDistributionSign = FsciParameter("SmPairingParams_InitatorKeyDistributionSign", 1)
        cmdParams.append(SmPairingParams_InitatorKeyDistributionSign)
        SmPairingParams_InitatorKeyDistributionLinkKey = FsciParameter("SmPairingParams_InitatorKeyDistributionLinkKey", 1)
        cmdParams.append(SmPairingParams_InitatorKeyDistributionLinkKey)
        SmPairingParams_InitatorKeyDistributionReserved = FsciParameter("SmPairingParams_InitatorKeyDistributionReserved", 1)
        cmdParams.append(SmPairingParams_InitatorKeyDistributionReserved)
        SmPairingParams_ResponderKeyDistributionEncKey = FsciParameter("SmPairingParams_ResponderKeyDistributionEncKey", 1)
        cmdParams.append(SmPairingParams_ResponderKeyDistributionEncKey)
        SmPairingParams_ResponderKeyDistributionIdKey = FsciParameter("SmPairingParams_ResponderKeyDistributionIdKey", 1)
        cmdParams.append(SmPairingParams_ResponderKeyDistributionIdKey)
        SmPairingParams_ResponderKeyDistributionSign = FsciParameter("SmPairingParams_ResponderKeyDistributionSign", 1)
        cmdParams.append(SmPairingParams_ResponderKeyDistributionSign)
        SmPairingParams_ResponderKeyDistributionLinkKey = FsciParameter("SmPairingParams_ResponderKeyDistributionLinkKey", 1)
        cmdParams.append(SmPairingParams_ResponderKeyDistributionLinkKey)
        SmPairingParams_ResponderKeyDistributionReserved = FsciParameter("SmPairingParams_ResponderKeyDistributionReserved", 1)
        cmdParams.append(SmPairingParams_ResponderKeyDistributionReserved)
        return FsciFrameDescription(0x42, 0x84, cmdParams)

    def InitSMPasskeyDisplayRequestIndication(self):
        cmdParams = []
        SourceDeviceId = FsciParameter("SourceDeviceId", 1)
        cmdParams.append(SourceDeviceId)
        PasskeyDisplayReqParams_Passkey = FsciParameter("PasskeyDisplayReqParams_Passkey", 4)
        cmdParams.append(PasskeyDisplayReqParams_Passkey)
        return FsciFrameDescription(0x42, 0x85, cmdParams)

    def InitSMPasskeyRequestIndication(self):
        cmdParams = []
        SourceDeviceId = FsciParameter("SourceDeviceId", 1)
        cmdParams.append(SourceDeviceId)
        PasskeyReqParams_PasskeyType = FsciParameter("PasskeyReqParams_PasskeyType", 1)
        cmdParams.append(PasskeyReqParams_PasskeyType)
        return FsciFrameDescription(0x42, 0x86, cmdParams)

    def InitSMPairingKeysetRequestIndication(self):
        cmdParams = []
        SourceDeviceId = FsciParameter("SourceDeviceId", 1)
        cmdParams.append(SourceDeviceId)
        PairingKeysetReqParams_RequestedKeysEncKey = FsciParameter("PairingKeysetReqParams_RequestedKeysEncKey", 1)
        cmdParams.append(PairingKeysetReqParams_RequestedKeysEncKey)
        PairingKeysetReqParams_RequestedKeysIdKey = FsciParameter("PairingKeysetReqParams_RequestedKeysIdKey", 1)
        cmdParams.append(PairingKeysetReqParams_RequestedKeysIdKey)
        PairingKeysetReqParams_RequestedKeysSign = FsciParameter("PairingKeysetReqParams_RequestedKeysSign", 1)
        cmdParams.append(PairingKeysetReqParams_RequestedKeysSign)
        PairingKeysetReqParams_RequestedKeysLinkKey = FsciParameter("PairingKeysetReqParams_RequestedKeysLinkKey", 1)
        cmdParams.append(PairingKeysetReqParams_RequestedKeysLinkKey)
        PairingKeysetReqParams_RequestedKeysReserved = FsciParameter("PairingKeysetReqParams_RequestedKeysReserved", 1)
        cmdParams.append(PairingKeysetReqParams_RequestedKeysReserved)
        PairingKeysetReqParams_EncKeySize = FsciParameter("PairingKeysetReqParams_EncKeySize", 1)
        cmdParams.append(PairingKeysetReqParams_EncKeySize)
        return FsciFrameDescription(0x42, 0x87, cmdParams)

    def InitSMPairingKeysetReceivedIndication(self):
        cmdParams = []
        SourceDeviceId = FsciParameter("SourceDeviceId", 1)
        cmdParams.append(SourceDeviceId)
        PairingKeysetReceivedParams_KeyDistPayloadLtk = FsciParameter("PairingKeysetReceivedParams_KeyDistPayloadLtk", 16)
        cmdParams.append(PairingKeysetReceivedParams_KeyDistPayloadLtk)
        PairingKeysetReceivedParams_KeyDistPayloadIrk = FsciParameter("PairingKeysetReceivedParams_KeyDistPayloadIrk", 16)
        cmdParams.append(PairingKeysetReceivedParams_KeyDistPayloadIrk)
        PairingKeysetReceivedParams_KeyDistPayloadCsrk = FsciParameter("PairingKeysetReceivedParams_KeyDistPayloadCsrk", 16)
        cmdParams.append(PairingKeysetReceivedParams_KeyDistPayloadCsrk)
        PairingKeysetReceivedParams_KeyDistPayloadEdiv = FsciParameter("PairingKeysetReceivedParams_KeyDistPayloadEdiv", 2)
        cmdParams.append(PairingKeysetReceivedParams_KeyDistPayloadEdiv)
        PairingKeysetReceivedParams_KeyDistPayloadRand = FsciParameter("PairingKeysetReceivedParams_KeyDistPayloadRand", 8)
        cmdParams.append(PairingKeysetReceivedParams_KeyDistPayloadRand)
        PairingKeysetReceivedParams_KeyDistPayloadBDAddr = FsciParameter("PairingKeysetReceivedParams_KeyDistPayloadBDAddr", 6)
        cmdParams.append(PairingKeysetReceivedParams_KeyDistPayloadBDAddr)
        PairingKeysetReceivedParams_KeyDistPayloadBDAddrType = FsciParameter("PairingKeysetReceivedParams_KeyDistPayloadBDAddrType", 1)
        cmdParams.append(PairingKeysetReceivedParams_KeyDistPayloadBDAddrType)
        PairingKeysetReceivedParams_ReceivedKeysEncKey = FsciParameter("PairingKeysetReceivedParams_ReceivedKeysEncKey", 1)
        cmdParams.append(PairingKeysetReceivedParams_ReceivedKeysEncKey)
        PairingKeysetReceivedParams_ReceivedKeysIdKey = FsciParameter("PairingKeysetReceivedParams_ReceivedKeysIdKey", 1)
        cmdParams.append(PairingKeysetReceivedParams_ReceivedKeysIdKey)
        PairingKeysetReceivedParams_ReceivedKeysSign = FsciParameter("PairingKeysetReceivedParams_ReceivedKeysSign", 1)
        cmdParams.append(PairingKeysetReceivedParams_ReceivedKeysSign)
        PairingKeysetReceivedParams_ReceivedKeysLinkKey = FsciParameter("PairingKeysetReceivedParams_ReceivedKeysLinkKey", 1)
        cmdParams.append(PairingKeysetReceivedParams_ReceivedKeysLinkKey)
        PairingKeysetReceivedParams_ReceivedKeysReserved = FsciParameter("PairingKeysetReceivedParams_ReceivedKeysReserved", 1)
        cmdParams.append(PairingKeysetReceivedParams_ReceivedKeysReserved)
        PairingKeysetReceivedParams_EncKeySize = FsciParameter("PairingKeysetReceivedParams_EncKeySize", 1)
        cmdParams.append(PairingKeysetReceivedParams_EncKeySize)
        return FsciFrameDescription(0x42, 0x88, cmdParams)

    def InitSMPairingCompleteIndication(self):
        cmdParams = []
        SourceDeviceId = FsciParameter("SourceDeviceId", 1)
        cmdParams.append(SourceDeviceId)
        PairingCompleteParams_EncKeySize = FsciParameter("PairingCompleteParams_EncKeySize", 1)
        cmdParams.append(PairingCompleteParams_EncKeySize)
        PairingCompleteParams_bMitmProtection = FsciParameter("PairingCompleteParams_bMitmProtection", 1)
        cmdParams.append(PairingCompleteParams_bMitmProtection)
        PairingCompleteParams_bBonding = FsciParameter("PairingCompleteParams_bBonding", 1)
        cmdParams.append(PairingCompleteParams_bBonding)
        PairingCompleteParams_LeSc = FsciParameter("PairingCompleteParams_LeSc", 1)
        cmdParams.append(PairingCompleteParams_LeSc)
        PairingCompleteParams_LeScLtk = FsciParameter("PairingCompleteParams_LeScLtk", 16)
        cmdParams.append(PairingCompleteParams_LeScLtk)
        return FsciFrameDescription(0x42, 0x89, cmdParams)

    def InitSMPairingFailedIndication(self):
        cmdParams = []
        SourceDeviceId = FsciParameter("SourceDeviceId", 1)
        cmdParams.append(SourceDeviceId)
        SmPairingFailedParams_Reason = FsciParameter("SmPairingFailedParams_Reason", 1)
        cmdParams.append(SmPairingFailedParams_Reason)
        return FsciFrameDescription(0x42, 0x8A, cmdParams)

    def InitSMReceivedPairingFailedIndication(self):
        cmdParams = []
        SourceDeviceId = FsciParameter("SourceDeviceId", 1)
        cmdParams.append(SourceDeviceId)
        SmPairingFailedParams_Reason = FsciParameter("SmPairingFailedParams_Reason", 1)
        cmdParams.append(SmPairingFailedParams_Reason)
        return FsciFrameDescription(0x42, 0x8B, cmdParams)

    def InitSMLlLtkRequestIndication(self):
        cmdParams = []
        SourceDeviceId = FsciParameter("SourceDeviceId", 1)
        cmdParams.append(SourceDeviceId)
        LlLtkRequestParams_RandomNumber = FsciParameter("LlLtkRequestParams_RandomNumber", 8)
        cmdParams.append(LlLtkRequestParams_RandomNumber)
        LlLtkRequestParams_EncryptedDiversifier = FsciParameter("LlLtkRequestParams_EncryptedDiversifier", 2)
        cmdParams.append(LlLtkRequestParams_EncryptedDiversifier)
        return FsciFrameDescription(0x42, 0x8C, cmdParams)

    def InitSMLlEncryptionStatusIndication(self):
        cmdParams = []
        SourceDeviceId = FsciParameter("SourceDeviceId", 1)
        cmdParams.append(SourceDeviceId)
        LlEncryptionStatusParams_LlStatus = FsciParameter("LlEncryptionStatusParams_LlStatus", 2)
        cmdParams.append(LlEncryptionStatusParams_LlStatus)
        LlEncryptionStatusParams_EncryptionState = FsciParameter("LlEncryptionStatusParams_EncryptionState", 1)
        cmdParams.append(LlEncryptionStatusParams_EncryptionState)
        LlEncryptionStatusParams_EncKeyType = FsciParameter("LlEncryptionStatusParams_EncKeyType", 1)
        cmdParams.append(LlEncryptionStatusParams_EncKeyType)
        return FsciFrameDescription(0x42, 0x8D, cmdParams)

    def InitSMLlEncryptResIndication(self):
        cmdParams = []
        SourceDeviceId = FsciParameter("SourceDeviceId", 1)
        cmdParams.append(SourceDeviceId)
        LlEncryptResParams_Status = FsciParameter("LlEncryptResParams_Status", 2)
        cmdParams.append(LlEncryptResParams_Status)
        LlEncryptResParams_EncryptedData = FsciParameter("LlEncryptResParams_EncryptedData", 16)
        cmdParams.append(LlEncryptResParams_EncryptedData)
        return FsciFrameDescription(0x42, 0x8E, cmdParams)

    def InitSMLlRandResIndication(self):
        cmdParams = []
        SourceDeviceId = FsciParameter("SourceDeviceId", 1)
        cmdParams.append(SourceDeviceId)
        LlRandResParams_Status = FsciParameter("LlRandResParams_Status", 2)
        cmdParams.append(LlRandResParams_Status)
        LlRandResParams_RandomNumber = FsciParameter("LlRandResParams_RandomNumber", 8)
        cmdParams.append(LlRandResParams_RandomNumber)
        return FsciFrameDescription(0x42, 0x8F, cmdParams)

    def InitSMTbCreateRandomDeviceAddrResIndication(self):
        cmdParams = []
        SourceDeviceId = FsciParameter("SourceDeviceId", 1)
        cmdParams.append(SourceDeviceId)
        TbCreateRandomDeviceAddrResParams_Status = FsciParameter("TbCreateRandomDeviceAddrResParams_Status", 2)
        cmdParams.append(TbCreateRandomDeviceAddrResParams_Status)
        TbCreateRandomDeviceAddrResParams_AddrType = FsciParameter("TbCreateRandomDeviceAddrResParams_AddrType", 1)
        cmdParams.append(TbCreateRandomDeviceAddrResParams_AddrType)
        TbCreateRandomDeviceAddrResParams_Addr = FsciParameter("TbCreateRandomDeviceAddrResParams_Addr", 6)
        cmdParams.append(TbCreateRandomDeviceAddrResParams_Addr)
        return FsciFrameDescription(0x42, 0x90, cmdParams)

    def InitSMTbCheckResolvalePrivateAddrResIndication(self):
        cmdParams = []
        SourceDeviceId = FsciParameter("SourceDeviceId", 1)
        cmdParams.append(SourceDeviceId)
        TbCheckResolvablePrivateAddrResParams_Status = FsciParameter("TbCheckResolvablePrivateAddrResParams_Status", 2)
        cmdParams.append(TbCheckResolvablePrivateAddrResParams_Status)
        return FsciFrameDescription(0x42, 0x91, cmdParams)

    def InitSMTbSignDataResIndication(self):
        cmdParams = []
        SourceDeviceId = FsciParameter("SourceDeviceId", 1)
        cmdParams.append(SourceDeviceId)
        TbSignDataResParams_Status = FsciParameter("TbSignDataResParams_Status", 2)
        cmdParams.append(TbSignDataResParams_Status)
        TbSignDataResParams_Signature = FsciParameter("TbSignDataResParams_Signature", 8)
        cmdParams.append(TbSignDataResParams_Signature)
        return FsciFrameDescription(0x42, 0x92, cmdParams)

    def InitSMTbVerifyDataSignatureResIndication(self):
        cmdParams = []
        SourceDeviceId = FsciParameter("SourceDeviceId", 1)
        cmdParams.append(SourceDeviceId)
        TbVerifyDataSignatureResParams_Status = FsciParameter("TbVerifyDataSignatureResParams_Status", 2)
        cmdParams.append(TbVerifyDataSignatureResParams_Status)
        return FsciFrameDescription(0x42, 0x93, cmdParams)

    def InitSMRemoteKeypressNotificationIndication(self):
        cmdParams = []
        SourceDeviceId = FsciParameter("SourceDeviceId", 1)
        cmdParams.append(SourceDeviceId)
        SmKeypressNotificationParams_keypressNotifType = FsciParameter("SmKeypressNotificationParams_keypressNotifType", 1)
        cmdParams.append(SmKeypressNotificationParams_keypressNotifType)
        return FsciFrameDescription(0x42, 0x94, cmdParams)

    def InitSMNcDisplayRequestIndication(self):
        cmdParams = []
        SourceDeviceId = FsciParameter("SourceDeviceId", 1)
        cmdParams.append(SourceDeviceId)
        ncValue = FsciParameter("ncValue", 4)
        cmdParams.append(ncValue)
        return FsciFrameDescription(0x42, 0x95, cmdParams)

    def InitSMLeScOobDataRequestIndication(self):
        cmdParams = []
        SourceDeviceId = FsciParameter("SourceDeviceId", 1)
        cmdParams.append(SourceDeviceId)
        return FsciFrameDescription(0x42, 0x96, cmdParams)

    def InitSMLocalLeScOobDataIndication(self):
        cmdParams = []
        SourceDeviceId = FsciParameter("SourceDeviceId", 1)
        cmdParams.append(SourceDeviceId)
        LeScOobDataParams_ = FsciParameter("LeScOobDataParams_", 1)
        cmdParams.append(LeScOobDataParams_)
        return FsciFrameDescription(0x42, 0x97, cmdParams)

    def InitSMGenerateNewEcdhPkSkPairResIndication(self):
        cmdParams = []
        SourceDeviceId = FsciParameter("SourceDeviceId", 1)
        cmdParams.append(SourceDeviceId)
        Status = FsciParameter("Status", 2)
        cmdParams.append(Status)
        return FsciFrameDescription(0x42, 0x98, cmdParams)

    def InitATTConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 2)
        cmdParams.append(Status)
        return FsciFrameDescription(0x43, 0x80, cmdParams)

    def InitATTGetMtuIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Mtu = FsciParameter("Mtu", 2)
        cmdParams.append(Mtu)
        return FsciFrameDescription(0x43, 0x81, cmdParams)

    def InitATTClientIncomingServerErrorResponseIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_RequestOpcodeInError = FsciParameter("Params_RequestOpcodeInError", 1)
        cmdParams.append(Params_RequestOpcodeInError)
        Params_AttributeHandleInError = FsciParameter("Params_AttributeHandleInError", 2)
        cmdParams.append(Params_AttributeHandleInError)
        Params_ErrorCode = FsciParameter("Params_ErrorCode", 1)
        cmdParams.append(Params_ErrorCode)
        return FsciFrameDescription(0x43, 0x82, cmdParams)

    def InitATTServerIncomingClientExchangeMtuRequestIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_ClientRxMtu = FsciParameter("Params_ClientRxMtu", 2)
        cmdParams.append(Params_ClientRxMtu)
        return FsciFrameDescription(0x43, 0x83, cmdParams)

    def InitATTClientIncomingServerExchangeMtuResponseIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_ServerRxMtu = FsciParameter("Params_ServerRxMtu", 2)
        cmdParams.append(Params_ServerRxMtu)
        return FsciFrameDescription(0x43, 0x84, cmdParams)

    def InitATTServerIncomingClientFindInformationRequestIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_StartingHandle = FsciParameter("Params_StartingHandle", 2)
        cmdParams.append(Params_StartingHandle)
        Params_EndingHandle = FsciParameter("Params_EndingHandle", 2)
        cmdParams.append(Params_EndingHandle)
        return FsciFrameDescription(0x43, 0x85, cmdParams)

    def InitATTClientIncomingServerFindInformationResponseIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_Format = FsciParameter("Params_Format", 1)
        cmdParams.append(Params_Format)
        Params_InformationDatadict = {}
        currentList = []
        Uuid16BitFormatHandleUuid16PairCount = FsciParameter("Uuid16BitFormatHandleUuid16PairCount", 1)
        currentList.append(Uuid16BitFormatHandleUuid16PairCount)
        HandleUuid16Handle = FsciParameter("HandleUuid16Handle", 2)
        currentList.append(HandleUuid16Handle)
        HandleUuid16Uuid16 = FsciParameter("HandleUuid16Uuid16", 2)
        currentList.append(HandleUuid16Uuid16)
        Params_InformationDatadict[0x01] = currentList
        currentList = []
        Uuid128BitFormatHandleUuid128PairCount = FsciParameter("Uuid128BitFormatHandleUuid128PairCount", 1)
        currentList.append(Uuid128BitFormatHandleUuid128PairCount)
        HandleUuid128Handle = FsciParameter("HandleUuid128Handle", 2)
        currentList.append(HandleUuid128Handle)
        HandleUuid128Uuid128 = FsciParameter("HandleUuid128Uuid128", 16)
        currentList.append(HandleUuid128Uuid128)
        Params_InformationDatadict[0x02] = currentList
        Params_InformationData = FsciParameter("Params_InformationData", 1, Params_Format, Params_InformationDatadict)
        cmdParams.append(Params_InformationData)
        return FsciFrameDescription(0x43, 0x86, cmdParams)

    def InitATTServerIncomingClientFindByTypeValueRequestIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_StartingHandle = FsciParameter("Params_StartingHandle", 2)
        cmdParams.append(Params_StartingHandle)
        Params_EndingHandle = FsciParameter("Params_EndingHandle", 2)
        cmdParams.append(Params_EndingHandle)
        Params_AttributeType = FsciParameter("Params_AttributeType", 2)
        cmdParams.append(Params_AttributeType)
        Params_AttributeLength = FsciParameter("Params_AttributeLength", 2)
        cmdParams.append(Params_AttributeLength)
        Params_AttributeValue = FsciParameter("Params_AttributeValue", 1, Params_AttributeLength)
        cmdParams.append(Params_AttributeValue)
        return FsciFrameDescription(0x43, 0x87, cmdParams)

    def InitATTClientIncomingServerFindByTypeValueResponseIndication(self):
        cmdParams = []
        # not generated, cursor based approach in observer; see events.py
        return FsciFrameDescription(0x43, 0x88, cmdParams)

    def InitATTServerIncomingClientReadByTypeRequestIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_StartingHandle = FsciParameter("Params_StartingHandle", 2)
        cmdParams.append(Params_StartingHandle)
        Params_EndingHandle = FsciParameter("Params_EndingHandle", 2)
        cmdParams.append(Params_EndingHandle)
        Params_Format = FsciParameter("Params_Format", 1)
        cmdParams.append(Params_Format)
        Params_AttributeTypedict = {}
        currentList = []
        Params_AttributeTypeUuid16BitFormat = FsciParameter("Params_AttributeTypeUuid16BitFormat", 2)
        currentList.append(Params_AttributeTypeUuid16BitFormat)
        Params_AttributeTypedict[0x01] = currentList
        currentList = []
        Params_AttributeTypeUuid128BitFormat = FsciParameter("Params_AttributeTypeUuid128BitFormat", 16)
        currentList.append(Params_AttributeTypeUuid128BitFormat)
        Params_AttributeTypedict[0x02] = currentList
        Params_AttributeType = FsciParameter("Params_AttributeType", 1, Params_Format, Params_AttributeTypedict)
        cmdParams.append(Params_AttributeType)
        return FsciFrameDescription(0x43, 0x89, cmdParams)

    def InitATTClientIncomingServerReadByTypeResponseIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_Length = FsciParameter("Params_Length", 1)
        cmdParams.append(Params_Length)
        Params_AttributeDataListLength = FsciParameter("Params_AttributeDataListLength", 2)
        cmdParams.append(Params_AttributeDataListLength)
        Params_AttributeDataList = FsciParameter("Params_AttributeDataList", 1, Params_AttributeDataListLength)
        cmdParams.append(Params_AttributeDataList)
        return FsciFrameDescription(0x43, 0x8A, cmdParams)

    def InitATTServerIncomingClientReadRequestIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_AttributeHandle = FsciParameter("Params_AttributeHandle", 2)
        cmdParams.append(Params_AttributeHandle)
        return FsciFrameDescription(0x43, 0x8B, cmdParams)

    def InitATTClientIncomingServerReadResponseIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_AttributeLength = FsciParameter("Params_AttributeLength", 2)
        cmdParams.append(Params_AttributeLength)
        Params_AttributeValue = FsciParameter("Params_AttributeValue", 1, Params_AttributeLength)
        cmdParams.append(Params_AttributeValue)
        return FsciFrameDescription(0x43, 0x8C, cmdParams)

    def InitATTServerIncomingClientReadBlobRequestIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_AttributeHandle = FsciParameter("Params_AttributeHandle", 2)
        cmdParams.append(Params_AttributeHandle)
        Params_ValueOffset = FsciParameter("Params_ValueOffset", 2)
        cmdParams.append(Params_ValueOffset)
        return FsciFrameDescription(0x43, 0x8D, cmdParams)

    def InitATTClientIncomingServerReadBlobResponseIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_AttributeLength = FsciParameter("Params_AttributeLength", 2)
        cmdParams.append(Params_AttributeLength)
        Params_AttributeValue = FsciParameter("Params_AttributeValue", 1, Params_AttributeLength)
        cmdParams.append(Params_AttributeValue)
        return FsciFrameDescription(0x43, 0x8E, cmdParams)

    def InitATTServerIncomingClientReadMultipleRequestIndication(self):
        cmdParams = []
        # not generated, cursor based approach in observer; see events.py
        return FsciFrameDescription(0x43, 0x8F, cmdParams)

    def InitATTClientIncomingServerReadMultipleResponseIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_ListLength = FsciParameter("Params_ListLength", 2)
        cmdParams.append(Params_ListLength)
        Params_ListOfValues = FsciParameter("Params_ListOfValues", 1, Params_ListLength)
        cmdParams.append(Params_ListOfValues)
        return FsciFrameDescription(0x43, 0x90, cmdParams)

    def InitATTServerIncomingClientReadByGroupTypeRequestIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_StartingHandle = FsciParameter("Params_StartingHandle", 2)
        cmdParams.append(Params_StartingHandle)
        Params_EndingHandle = FsciParameter("Params_EndingHandle", 2)
        cmdParams.append(Params_EndingHandle)
        Params_Format = FsciParameter("Params_Format", 1)
        cmdParams.append(Params_Format)
        Params_AttributeTypedict = {}
        currentList = []
        Params_AttributeTypeUuid16BitFormat = FsciParameter("Params_AttributeTypeUuid16BitFormat", 2)
        currentList.append(Params_AttributeTypeUuid16BitFormat)
        Params_AttributeTypedict[0x01] = currentList
        currentList = []
        Params_AttributeTypeUuid128BitFormat = FsciParameter("Params_AttributeTypeUuid128BitFormat", 16)
        currentList.append(Params_AttributeTypeUuid128BitFormat)
        Params_AttributeTypedict[0x02] = currentList
        Params_AttributeType = FsciParameter("Params_AttributeType", 1, Params_Format, Params_AttributeTypedict)
        cmdParams.append(Params_AttributeType)
        return FsciFrameDescription(0x43, 0x91, cmdParams)

    def InitATTClientIncomingServerReadByGroupTypeResponseIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_Length = FsciParameter("Params_Length", 1)
        cmdParams.append(Params_Length)
        Params_AttributeDataListLength = FsciParameter("Params_AttributeDataListLength", 2)
        cmdParams.append(Params_AttributeDataListLength)
        Params_AttributeDataList = FsciParameter("Params_AttributeDataList", 1, Params_AttributeDataListLength)
        cmdParams.append(Params_AttributeDataList)
        return FsciFrameDescription(0x43, 0x92, cmdParams)

    def InitATTServerIncomingClientWriteRequestIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_AttributeHandle = FsciParameter("Params_AttributeHandle", 2)
        cmdParams.append(Params_AttributeHandle)
        Params_AttributeLength = FsciParameter("Params_AttributeLength", 2)
        cmdParams.append(Params_AttributeLength)
        Params_AttributeValue = FsciParameter("Params_AttributeValue", 1, Params_AttributeLength)
        cmdParams.append(Params_AttributeValue)
        return FsciFrameDescription(0x43, 0x93, cmdParams)

    def InitATTClientIncomingServerWriteResponseIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        return FsciFrameDescription(0x43, 0x94, cmdParams)

    def InitATTServerIncomingClientWriteCommandIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_AttributeHandle = FsciParameter("Params_AttributeHandle", 2)
        cmdParams.append(Params_AttributeHandle)
        Params_AttributeLength = FsciParameter("Params_AttributeLength", 2)
        cmdParams.append(Params_AttributeLength)
        Params_AttributeValue = FsciParameter("Params_AttributeValue", 1, Params_AttributeLength)
        cmdParams.append(Params_AttributeValue)
        return FsciFrameDescription(0x43, 0x95, cmdParams)

    def InitATTServerIncomingClientSignedWriteCommandIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_AttributeHandle = FsciParameter("Params_AttributeHandle", 2)
        cmdParams.append(Params_AttributeHandle)
        Params_AttributeLength = FsciParameter("Params_AttributeLength", 2)
        cmdParams.append(Params_AttributeLength)
        Params_AttributeValue = FsciParameter("Params_AttributeValue", 1, Params_AttributeLength)
        cmdParams.append(Params_AttributeValue)
        Params_AuthenticationSignature = FsciParameter("Params_AuthenticationSignature", 12)
        cmdParams.append(Params_AuthenticationSignature)
        return FsciFrameDescription(0x43, 0x96, cmdParams)

    def InitATTServerIncomingClientPrepareWriteRequestIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_AttributeHandle = FsciParameter("Params_AttributeHandle", 2)
        cmdParams.append(Params_AttributeHandle)
        Params_ValueOffset = FsciParameter("Params_ValueOffset", 2)
        cmdParams.append(Params_ValueOffset)
        Params_AttributeLength = FsciParameter("Params_AttributeLength", 2)
        cmdParams.append(Params_AttributeLength)
        Params_AttributeValue = FsciParameter("Params_AttributeValue", 1, Params_AttributeLength)
        cmdParams.append(Params_AttributeValue)
        return FsciFrameDescription(0x43, 0x97, cmdParams)

    def InitATTClientIncomingServerPrepareWriteResponseIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_AttributeHandle = FsciParameter("Params_AttributeHandle", 2)
        cmdParams.append(Params_AttributeHandle)
        Params_ValueOffset = FsciParameter("Params_ValueOffset", 2)
        cmdParams.append(Params_ValueOffset)
        Params_AttributeLength = FsciParameter("Params_AttributeLength", 2)
        cmdParams.append(Params_AttributeLength)
        Params_AttributeValue = FsciParameter("Params_AttributeValue", 1, Params_AttributeLength)
        cmdParams.append(Params_AttributeValue)
        return FsciFrameDescription(0x43, 0x98, cmdParams)

    def InitATTServerIncomingClientExecuteWriteRequestIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_Flags = FsciParameter("Params_Flags", 1)
        cmdParams.append(Params_Flags)
        return FsciFrameDescription(0x43, 0x99, cmdParams)

    def InitATTClientIncomingServerExecuteWriteResponseIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        return FsciFrameDescription(0x43, 0x9A, cmdParams)

    def InitATTClientIncomingServerHandleValueNotificationIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_AttributeHandle = FsciParameter("Params_AttributeHandle", 2)
        cmdParams.append(Params_AttributeHandle)
        Params_AttributeLength = FsciParameter("Params_AttributeLength", 2)
        cmdParams.append(Params_AttributeLength)
        Params_AttributeValue = FsciParameter("Params_AttributeValue", 1, Params_AttributeLength)
        cmdParams.append(Params_AttributeValue)
        return FsciFrameDescription(0x43, 0x9B, cmdParams)

    def InitATTClientIncomingServerHandleValueIndicationIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Params_AttributeHandle = FsciParameter("Params_AttributeHandle", 2)
        cmdParams.append(Params_AttributeHandle)
        Params_AttributeLength = FsciParameter("Params_AttributeLength", 2)
        cmdParams.append(Params_AttributeLength)
        Params_AttributeValue = FsciParameter("Params_AttributeValue", 1, Params_AttributeLength)
        cmdParams.append(Params_AttributeValue)
        return FsciFrameDescription(0x43, 0x9C, cmdParams)

    def InitATTServerIncomingClientHandleValueConfirmationIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        return FsciFrameDescription(0x43, 0x9D, cmdParams)

    def InitATTUnsupportedOpcodeIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Opcode = FsciParameter("Opcode", 1)
        cmdParams.append(Opcode)
        return FsciFrameDescription(0x43, 0x9E, cmdParams)

    def InitATTTimeoutIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        return FsciFrameDescription(0x43, 0x9F, cmdParams)

    def InitGATTConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 2)
        cmdParams.append(Status)
        return FsciFrameDescription(0x44, 0x80, cmdParams)

    def InitGATTGetMtuIndication(self):
        cmdParams = []
        Mtu = FsciParameter("Mtu", 2)
        cmdParams.append(Mtu)
        return FsciFrameDescription(0x44, 0x81, cmdParams)

    def InitGATTClientProcedureExchangeMtuIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        ProcedureResult = FsciParameter("ProcedureResult", 1)
        cmdParams.append(ProcedureResult)
        Error = FsciParameter("Error", 2)
        cmdParams.append(Error)
        return FsciFrameDescription(0x44, 0x82, cmdParams)

    def InitGATTClientProcedureDiscoverAllPrimaryServicesIndication(self):
        cmdParams = []
        # not generated, cursor based approach in observer; see events.py
        return FsciFrameDescription(0x44, 0x83, cmdParams)

    def InitGATTClientProcedureDiscoverPrimaryServicesByUuidIndication(self):
        cmdParams = []
        # not generated, cursor based approach in observer; see events.py
        return FsciFrameDescription(0x44, 0x84, cmdParams)

    def InitGATTClientProcedureFindIncludedServicesIndication(self):
        cmdParams = []
        # not generated, cursor based approach in observer; see events.py
        return FsciFrameDescription(0x44, 0x85, cmdParams)

    def InitGATTClientProcedureDiscoverAllCharacteristicsIndication(self):
        cmdParams = []
        # not generated, cursor based approach in observer; see events.py
        return FsciFrameDescription(0x44, 0x86, cmdParams)

    def InitGATTClientProcedureDiscoverCharacteristicByUuidIndication(self):
        cmdParams = []
        # not generated, cursor based approach in observer; see events.py
        return FsciFrameDescription(0x44, 0x87, cmdParams)

    def InitGATTClientProcedureDiscoverAllCharacteristicDescriptorsIndication(self):
        cmdParams = []
        # not generated, cursor based approach in observer; see events.py
        return FsciFrameDescription(0x44, 0x88, cmdParams)

    def InitGATTClientProcedureReadCharacteristicValueIndication(self):
        cmdParams = []
        # not generated, cursor based approach in observer; see events.py
        return FsciFrameDescription(0x44, 0x89, cmdParams)

    def InitGATTClientProcedureReadUsingCharacteristicUuidIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        ProcedureResult = FsciParameter("ProcedureResult", 1)
        cmdParams.append(ProcedureResult)
        Error = FsciParameter("Error", 2)
        cmdParams.append(Error)
        NbOfReadBytes = FsciParameter("NbOfReadBytes", 2)
        cmdParams.append(NbOfReadBytes)
        ReadBytes = FsciParameter("ReadBytes", 1, NbOfReadBytes)
        cmdParams.append(ReadBytes)
        return FsciFrameDescription(0x44, 0x8A, cmdParams)

    def InitGATTClientProcedureReadMultipleCharacteristicValuesIndication(self):
        cmdParams = []
        # not generated, cursor based approach in observer; see events.py
        return FsciFrameDescription(0x44, 0x8B, cmdParams)

    def InitGATTClientProcedureWriteCharacteristicValueIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        ProcedureResult = FsciParameter("ProcedureResult", 1)
        cmdParams.append(ProcedureResult)
        Error = FsciParameter("Error", 2)
        cmdParams.append(Error)
        return FsciFrameDescription(0x44, 0x8C, cmdParams)

    def InitGATTClientProcedureReadCharacteristicDescriptorIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        ProcedureResult = FsciParameter("ProcedureResult", 1)
        cmdParams.append(ProcedureResult)
        Error = FsciParameter("Error", 2)
        cmdParams.append(Error)
        Descriptor_Handle = FsciParameter("Descriptor_Handle", 2)
        cmdParams.append(Descriptor_Handle)
        Descriptor_UuidType = FsciParameter("Descriptor_UuidType", 1)
        cmdParams.append(Descriptor_UuidType)
        Descriptor_Uuiddict = {}
        currentList = []
        Descriptor_UuidUuid16Bits = FsciParameter("Descriptor_UuidUuid16Bits", 2)
        currentList.append(Descriptor_UuidUuid16Bits)
        Descriptor_Uuiddict[0x01] = currentList
        currentList = []
        Descriptor_UuidUuid128Bits = FsciParameter("Descriptor_UuidUuid128Bits", 16)
        currentList.append(Descriptor_UuidUuid128Bits)
        Descriptor_Uuiddict[0x02] = currentList
        currentList = []
        Descriptor_UuidUuid32Bits = FsciParameter("Descriptor_UuidUuid32Bits", 4)
        currentList.append(Descriptor_UuidUuid32Bits)
        Descriptor_Uuiddict[0x03] = currentList
        Descriptor_Uuid = FsciParameter("Descriptor_Uuid", 1, Descriptor_UuidType, Descriptor_Uuiddict)
        cmdParams.append(Descriptor_Uuid)
        Descriptor_ValueLength = FsciParameter("Descriptor_ValueLength", 2)
        cmdParams.append(Descriptor_ValueLength)
        Descriptor_MaxValueLength = FsciParameter("Descriptor_MaxValueLength", 2)
        cmdParams.append(Descriptor_MaxValueLength)
        Descriptor_Value = FsciParameter("Descriptor_Value", 1, Descriptor_ValueLength)
        cmdParams.append(Descriptor_Value)
        return FsciFrameDescription(0x44, 0x8D, cmdParams)

    def InitGATTClientProcedureWriteCharacteristicDescriptorIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        ProcedureResult = FsciParameter("ProcedureResult", 1)
        cmdParams.append(ProcedureResult)
        Error = FsciParameter("Error", 2)
        cmdParams.append(Error)
        return FsciFrameDescription(0x44, 0x8E, cmdParams)

    def InitGATTClientNotificationIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        CharacteristicValueHandle = FsciParameter("CharacteristicValueHandle", 2)
        cmdParams.append(CharacteristicValueHandle)
        ValueLength = FsciParameter("ValueLength", 2)
        cmdParams.append(ValueLength)
        Value = FsciParameter("Value", 1, ValueLength)
        cmdParams.append(Value)
        return FsciFrameDescription(0x44, 0x8F, cmdParams)

    def InitGATTClientIndicationIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        CharacteristicValueHandle = FsciParameter("CharacteristicValueHandle", 2)
        cmdParams.append(CharacteristicValueHandle)
        ValueLength = FsciParameter("ValueLength", 2)
        cmdParams.append(ValueLength)
        Value = FsciParameter("Value", 1, ValueLength)
        cmdParams.append(Value)
        return FsciFrameDescription(0x44, 0x90, cmdParams)

    def InitGATTServerMtuChangedIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        MtuChangedEvent_NewMtu = FsciParameter("MtuChangedEvent_NewMtu", 2)
        cmdParams.append(MtuChangedEvent_NewMtu)
        return FsciFrameDescription(0x44, 0x91, cmdParams)

    def InitGATTServerHandleValueConfirmationIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        return FsciFrameDescription(0x44, 0x92, cmdParams)

    def InitGATTServerAttributeWrittenIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        AttributeWrittenEvent_Handle = FsciParameter("AttributeWrittenEvent_Handle", 2)
        cmdParams.append(AttributeWrittenEvent_Handle)
        AttributeWrittenEvent_ValueLength = FsciParameter("AttributeWrittenEvent_ValueLength", 2)
        cmdParams.append(AttributeWrittenEvent_ValueLength)
        AttributeWrittenEvent_Value = FsciParameter("AttributeWrittenEvent_Value", 1, AttributeWrittenEvent_ValueLength)
        cmdParams.append(AttributeWrittenEvent_Value)
        return FsciFrameDescription(0x44, 0x93, cmdParams)

    def InitGATTServerCharacteristicCccdWrittenIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        CharCccdWrittenEvent_Handle = FsciParameter("CharCccdWrittenEvent_Handle", 2)
        cmdParams.append(CharCccdWrittenEvent_Handle)
        CharCccdWrittenEvent_NewCccd = FsciParameter("CharCccdWrittenEvent_NewCccd", 1)
        cmdParams.append(CharCccdWrittenEvent_NewCccd)
        return FsciFrameDescription(0x44, 0x94, cmdParams)

    def InitGATTServerAttributeWrittenWithoutResponseIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        AttributeWrittenEvent_Handle = FsciParameter("AttributeWrittenEvent_Handle", 2)
        cmdParams.append(AttributeWrittenEvent_Handle)
        AttributeWrittenEvent_ValueLength = FsciParameter("AttributeWrittenEvent_ValueLength", 2)
        cmdParams.append(AttributeWrittenEvent_ValueLength)
        AttributeWrittenEvent_Value = FsciParameter("AttributeWrittenEvent_Value", 1, AttributeWrittenEvent_ValueLength)
        cmdParams.append(AttributeWrittenEvent_Value)
        return FsciFrameDescription(0x44, 0x95, cmdParams)

    def InitGATTServerErrorIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        ProcedureError_ProcedureType = FsciParameter("ProcedureError_ProcedureType", 1)
        cmdParams.append(ProcedureError_ProcedureType)
        ProcedureError_Error = FsciParameter("ProcedureError_Error", 2)
        cmdParams.append(ProcedureError_Error)
        return FsciFrameDescription(0x44, 0x96, cmdParams)

    def InitGATTServerLongCharacteristicWrittenIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        LongCharacteristicWrittenEvent_Handle = FsciParameter("LongCharacteristicWrittenEvent_Handle", 2)
        cmdParams.append(LongCharacteristicWrittenEvent_Handle)
        LongCharacteristicWrittenEvent_ValueLength = FsciParameter("LongCharacteristicWrittenEvent_ValueLength", 2)
        cmdParams.append(LongCharacteristicWrittenEvent_ValueLength)
        LongCharacteristicWrittenEvent_Value = FsciParameter("LongCharacteristicWrittenEvent_Value", 1, LongCharacteristicWrittenEvent_ValueLength)
        cmdParams.append(LongCharacteristicWrittenEvent_Value)
        return FsciFrameDescription(0x44, 0x97, cmdParams)

    def InitGATTServerAttributeReadIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        AttributeReadEvent_Handle = FsciParameter("AttributeReadEvent_Handle", 2)
        cmdParams.append(AttributeReadEvent_Handle)
        return FsciFrameDescription(0x44, 0x98, cmdParams)

    def InitGATTDBConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 2)
        cmdParams.append(Status)
        return FsciFrameDescription(0x45, 0x80, cmdParams)

    def InitGATTDBReadAttributeIndication(self):
        cmdParams = []
        ValueLength = FsciParameter("ValueLength", 2)
        cmdParams.append(ValueLength)
        Value = FsciParameter("Value", 1, ValueLength)
        cmdParams.append(Value)
        return FsciFrameDescription(0x45, 0x81, cmdParams)

    def InitGATTDBFindServiceHandleIndication(self):
        cmdParams = []
        ServiceHandle = FsciParameter("ServiceHandle", 2)
        cmdParams.append(ServiceHandle)
        return FsciFrameDescription(0x45, 0x82, cmdParams)

    def InitGATTDBFindCharValueHandleInServiceIndication(self):
        cmdParams = []
        CharValueHandle = FsciParameter("CharValueHandle", 2)
        cmdParams.append(CharValueHandle)
        return FsciFrameDescription(0x45, 0x83, cmdParams)

    def InitGATTDBFindCccdHandleForCharValueHandleIndication(self):
        cmdParams = []
        CccdHandle = FsciParameter("CccdHandle", 2)
        cmdParams.append(CccdHandle)
        return FsciFrameDescription(0x45, 0x84, cmdParams)

    def InitGATTDBFindDescriptorHandleForCharValueHandleIndication(self):
        cmdParams = []
        DescriptorHandle = FsciParameter("DescriptorHandle", 2)
        cmdParams.append(DescriptorHandle)
        return FsciFrameDescription(0x45, 0x85, cmdParams)

    def InitGATTDBDynamicAddPrimaryServiceDeclarationIndication(self):
        cmdParams = []
        ServiceHandle = FsciParameter("ServiceHandle", 2)
        cmdParams.append(ServiceHandle)
        return FsciFrameDescription(0x45, 0x86, cmdParams)

    def InitGATTDBDynamicAddSecondaryServiceDeclarationIndication(self):
        cmdParams = []
        ServiceHandle = FsciParameter("ServiceHandle", 2)
        cmdParams.append(ServiceHandle)
        return FsciFrameDescription(0x45, 0x87, cmdParams)

    def InitGATTDBDynamicAddIncludeDeclarationIndication(self):
        cmdParams = []
        IncludeHandle = FsciParameter("IncludeHandle", 2)
        cmdParams.append(IncludeHandle)
        return FsciFrameDescription(0x45, 0x88, cmdParams)

    def InitGATTDBDynamicAddCharacteristicDeclarationAndValueIndication(self):
        cmdParams = []
        CharacteristicHandle = FsciParameter("CharacteristicHandle", 2)
        cmdParams.append(CharacteristicHandle)
        return FsciFrameDescription(0x45, 0x89, cmdParams)

    def InitGATTDBDynamicAddCharacteristicDescriptorIndication(self):
        cmdParams = []
        DescriptorHandle = FsciParameter("DescriptorHandle", 2)
        cmdParams.append(DescriptorHandle)
        return FsciFrameDescription(0x45, 0x8A, cmdParams)

    def InitGATTDBDynamicAddCccdIndication(self):
        cmdParams = []
        CCCDHandle = FsciParameter("CCCDHandle", 2)
        cmdParams.append(CCCDHandle)
        return FsciFrameDescription(0x45, 0x8B, cmdParams)

    def InitGATTDBDynamicAddCharacteristicDeclarationWithUniqueValueIndication(self):
        cmdParams = []
        CharacteristicHandle = FsciParameter("CharacteristicHandle", 2)
        cmdParams.append(CharacteristicHandle)
        return FsciFrameDescription(0x45, 0x8C, cmdParams)

    def InitGATTDBAttConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 2)
        cmdParams.append(Status)
        return FsciFrameDescription(0x46, 0x80, cmdParams)

    def InitGATTDBAttFindInformationIndication(self):
        cmdParams = []
        Params_Format = FsciParameter("Params_Format", 1)
        cmdParams.append(Params_Format)
        Params_InformationDatadict = {}
        currentList = []
        Uuid16BitFormatHandleUuid16PairCount = FsciParameter("Uuid16BitFormatHandleUuid16PairCount", 1)
        currentList.append(Uuid16BitFormatHandleUuid16PairCount)
        HandleUuid16Handle = FsciParameter("HandleUuid16Handle", 2)
        currentList.append(HandleUuid16Handle)
        HandleUuid16Uuid16 = FsciParameter("HandleUuid16Uuid16", 2)
        currentList.append(HandleUuid16Uuid16)
        Params_InformationDatadict[0x01] = currentList
        currentList = []
        Uuid128BitFormatHandleUuid128PairCount = FsciParameter("Uuid128BitFormatHandleUuid128PairCount", 1)
        currentList.append(Uuid128BitFormatHandleUuid128PairCount)
        HandleUuid128Handle = FsciParameter("HandleUuid128Handle", 2)
        currentList.append(HandleUuid128Handle)
        HandleUuid128Uuid128 = FsciParameter("HandleUuid128Uuid128", 16)
        currentList.append(HandleUuid128Uuid128)
        Params_InformationDatadict[0x02] = currentList
        Params_InformationData = FsciParameter("Params_InformationData", 1, Params_Format, Params_InformationDatadict)
        cmdParams.append(Params_InformationData)
        ErrorAttributeHandle = FsciParameter("ErrorAttributeHandle", 2)
        cmdParams.append(ErrorAttributeHandle)
        return FsciFrameDescription(0x46, 0x81, cmdParams)

    def InitGATTDBAttFindByTypeValueIndication(self):
        cmdParams = []
        # not generated, cursor based approach in observer; see events.py
        return FsciFrameDescription(0x46, 0x82, cmdParams)

    def InitGATTDBAttReadByTypeIndication(self):
        cmdParams = []
        Params_Length = FsciParameter("Params_Length", 1)
        cmdParams.append(Params_Length)
        Params_AttributeDataListLength = FsciParameter("Params_AttributeDataListLength", 2)
        cmdParams.append(Params_AttributeDataListLength)
        Params_AttributeDataList = FsciParameter("Params_AttributeDataList", 1, Params_AttributeDataListLength)
        cmdParams.append(Params_AttributeDataList)
        ErrorAttributeHandle = FsciParameter("ErrorAttributeHandle", 2)
        cmdParams.append(ErrorAttributeHandle)
        return FsciFrameDescription(0x46, 0x83, cmdParams)

    def InitGATTDBAttReadIndication(self):
        cmdParams = []
        Params_AttributeLength = FsciParameter("Params_AttributeLength", 2)
        cmdParams.append(Params_AttributeLength)
        Params_AttributeValue = FsciParameter("Params_AttributeValue", 1, Params_AttributeLength)
        cmdParams.append(Params_AttributeValue)
        ErrorAttributeHandle = FsciParameter("ErrorAttributeHandle", 2)
        cmdParams.append(ErrorAttributeHandle)
        return FsciFrameDescription(0x46, 0x84, cmdParams)

    def InitGATTDBAttReadBlobIndication(self):
        cmdParams = []
        ErrorAttributeHandle = FsciParameter("ErrorAttributeHandle", 2)
        cmdParams.append(ErrorAttributeHandle)
        return FsciFrameDescription(0x46, 0x85, cmdParams)

    def InitGATTDBAttReadMultipleIndication(self):
        cmdParams = []
        Params_ListLength = FsciParameter("Params_ListLength", 2)
        cmdParams.append(Params_ListLength)
        Params_ListOfValues = FsciParameter("Params_ListOfValues", 1, Params_ListLength)
        cmdParams.append(Params_ListOfValues)
        ErrorAttributeHandle = FsciParameter("ErrorAttributeHandle", 2)
        cmdParams.append(ErrorAttributeHandle)
        return FsciFrameDescription(0x46, 0x86, cmdParams)

    def InitGATTDBAttReadByGroupTypeIndication(self):
        cmdParams = []
        Params_Length = FsciParameter("Params_Length", 1)
        cmdParams.append(Params_Length)
        Params_AttributeDataListLength = FsciParameter("Params_AttributeDataListLength", 2)
        cmdParams.append(Params_AttributeDataListLength)
        Params_AttributeDataList = FsciParameter("Params_AttributeDataList", 1, Params_AttributeDataListLength)
        cmdParams.append(Params_AttributeDataList)
        ErrorAttributeHandle = FsciParameter("ErrorAttributeHandle", 2)
        cmdParams.append(ErrorAttributeHandle)
        return FsciFrameDescription(0x46, 0x87, cmdParams)

    def InitGATTDBAttWriteIndication(self):
        cmdParams = []
        ErrorAttributeHandle = FsciParameter("ErrorAttributeHandle", 2)
        cmdParams.append(ErrorAttributeHandle)
        return FsciFrameDescription(0x46, 0x88, cmdParams)

    def InitGATTDBAttPrepareWriteIndication(self):
        cmdParams = []
        Params_AttributeHandle = FsciParameter("Params_AttributeHandle", 2)
        cmdParams.append(Params_AttributeHandle)
        Params_ValueOffset = FsciParameter("Params_ValueOffset", 2)
        cmdParams.append(Params_ValueOffset)
        Params_AttributeLength = FsciParameter("Params_AttributeLength", 2)
        cmdParams.append(Params_AttributeLength)
        Params_AttributeValue = FsciParameter("Params_AttributeValue", 1, Params_AttributeLength)
        cmdParams.append(Params_AttributeValue)
        ErrorAttributeHandle = FsciParameter("ErrorAttributeHandle", 2)
        cmdParams.append(ErrorAttributeHandle)
        return FsciFrameDescription(0x46, 0x89, cmdParams)

    def InitGATTDBAttExecuteWriteIndication(self):
        cmdParams = []
        ErrorAttributeHandle = FsciParameter("ErrorAttributeHandle", 2)
        cmdParams.append(ErrorAttributeHandle)
        return FsciFrameDescription(0x46, 0x8A, cmdParams)

    def InitGATTDBAttExecuteWriteFromQueueIndication(self):
        cmdParams = []
        ErrorAttributeHandle = FsciParameter("ErrorAttributeHandle", 2)
        cmdParams.append(ErrorAttributeHandle)
        return FsciFrameDescription(0x46, 0x8B, cmdParams)

    def InitGATTDBAttPrepareNotificationIndicationIndication(self):
        cmdParams = []
        ErrorAttributeHandle = FsciParameter("ErrorAttributeHandle", 2)
        cmdParams.append(ErrorAttributeHandle)
        return FsciFrameDescription(0x46, 0x8C, cmdParams)

    def InitGAPConfirm(self):
        cmdParams = []
        Status = FsciParameter("Status", 2)
        cmdParams.append(Status)
        return FsciFrameDescription(0x47, 0x80, cmdParams)

    def InitGAPCheckNotificationStatusIndication(self):
        cmdParams = []
        IsActive = FsciParameter("IsActive", 1)
        cmdParams.append(IsActive)
        return FsciFrameDescription(0x47, 0x81, cmdParams)

    def InitGAPCheckIndicationStatusIndication(self):
        cmdParams = []
        IsActive = FsciParameter("IsActive", 1)
        cmdParams.append(IsActive)
        return FsciFrameDescription(0x47, 0x82, cmdParams)

    def InitGAPGetBondedStaticAddressesIndication(self):
        cmdParams = []
        NbOfDeviceAddresses = FsciParameter("NbOfDeviceAddresses", 1)
        cmdParams.append(NbOfDeviceAddresses)
        DeviceAddresses = FsciParameter("DeviceAddresses", 6, NbOfDeviceAddresses)
        cmdParams.append(DeviceAddresses)
        return FsciFrameDescription(0x47, 0x83, cmdParams)

    def InitGAPLoadEncryptionInformationIndication(self):
        cmdParams = []
        LtkSize = FsciParameter("LtkSize", 1)
        cmdParams.append(LtkSize)
        Ltk = FsciParameter("Ltk", 1, LtkSize)
        cmdParams.append(Ltk)
        return FsciFrameDescription(0x47, 0x84, cmdParams)

    def InitGAPLoadCustomPeerInformationIndication(self):
        cmdParams = []
        InfoSize = FsciParameter("InfoSize", 2)
        cmdParams.append(InfoSize)
        Info = FsciParameter("Info", 1, InfoSize)
        cmdParams.append(Info)
        return FsciFrameDescription(0x47, 0x85, cmdParams)

    def InitGAPCheckIfBondedIndication(self):
        cmdParams = []
        IsBonded = FsciParameter("IsBonded", 1)
        cmdParams.append(IsBonded)
        return FsciFrameDescription(0x47, 0x86, cmdParams)

    def InitGAPGetBondedDevicesCountIndication(self):
        cmdParams = []
        NbOfBondedDevices = FsciParameter("NbOfBondedDevices", 1)
        cmdParams.append(NbOfBondedDevices)
        return FsciFrameDescription(0x47, 0x87, cmdParams)

    def InitGAPGetBondedDeviceNameIndication(self):
        cmdParams = []
        NameSize = FsciParameter("NameSize", 1)
        cmdParams.append(NameSize)
        Name = FsciParameter("Name", 1, NameSize)
        cmdParams.append(Name)
        return FsciFrameDescription(0x47, 0x88, cmdParams)

    def InitGAPGenericEventInitializationCompleteIndication(self):
        cmdParams = []
        return FsciFrameDescription(0x47, 0x89, cmdParams)

    def InitGAPGenericEventInternalErrorIndication(self):
        cmdParams = []
        InternalError_ErrorCode = FsciParameter("InternalError_ErrorCode", 2)
        cmdParams.append(InternalError_ErrorCode)
        InternalError_ErrorSource = FsciParameter("InternalError_ErrorSource", 1)
        cmdParams.append(InternalError_ErrorSource)
        InternalError_HciCommandOpcode = FsciParameter("InternalError_HciCommandOpcode", 2)
        cmdParams.append(InternalError_HciCommandOpcode)
        return FsciFrameDescription(0x47, 0x8A, cmdParams)

    def InitGAPGenericEventAdvertisingSetupFailedIndication(self):
        cmdParams = []
        SetupFailReason = FsciParameter("SetupFailReason", 2)
        cmdParams.append(SetupFailReason)
        return FsciFrameDescription(0x47, 0x8B, cmdParams)

    def InitGAPGenericEventAdvertisingParametersSetupCompleteIndication(self):
        cmdParams = []
        return FsciFrameDescription(0x47, 0x8C, cmdParams)

    def InitGAPGenericEventAdvertisingDataSetupCompleteIndication(self):
        cmdParams = []
        return FsciFrameDescription(0x47, 0x8D, cmdParams)

    def InitGAPGenericEventWhiteListSizeReadIndication(self):
        cmdParams = []
        WhiteListSize = FsciParameter("WhiteListSize", 1)
        cmdParams.append(WhiteListSize)
        return FsciFrameDescription(0x47, 0x8E, cmdParams)

    def InitGAPGenericEventDeviceAddedToWhiteListIndication(self):
        cmdParams = []
        return FsciFrameDescription(0x47, 0x8F, cmdParams)

    def InitGAPGenericEventDeviceRemovedFromWhiteListIndication(self):
        cmdParams = []
        return FsciFrameDescription(0x47, 0x90, cmdParams)

    def InitGAPGenericEventWhiteListClearedIndication(self):
        cmdParams = []
        return FsciFrameDescription(0x47, 0x91, cmdParams)

    def InitGAPGenericEventRandomAddressReadyIndication(self):
        cmdParams = []
        Address = FsciParameter("Address", 6)
        cmdParams.append(Address)
        return FsciFrameDescription(0x47, 0x92, cmdParams)

    def InitGAPGenericEventCreateConnectionCanceledIndication(self):
        cmdParams = []
        return FsciFrameDescription(0x47, 0x93, cmdParams)

    def InitGAPGenericEventPublicAddressReadIndication(self):
        cmdParams = []
        Address = FsciParameter("Address", 6)
        cmdParams.append(Address)
        return FsciFrameDescription(0x47, 0x94, cmdParams)

    def InitGAPGenericEventAdvTxPowerLevelReadIndication(self):
        cmdParams = []
        AdvTxPowerLevel = FsciParameter("AdvTxPowerLevel", 1)
        cmdParams.append(AdvTxPowerLevel)
        return FsciFrameDescription(0x47, 0x95, cmdParams)

    def InitGAPGenericEventPrivateResolvableAddressVerifiedIndication(self):
        cmdParams = []
        Verified = FsciParameter("Verified", 1)
        cmdParams.append(Verified)
        return FsciFrameDescription(0x47, 0x96, cmdParams)

    def InitGAPGenericEventRandomAddressSetIndication(self):
        cmdParams = []
        return FsciFrameDescription(0x47, 0x97, cmdParams)

    def InitGAPAdvertisingEventStateChangedIndication(self):
        cmdParams = []
        return FsciFrameDescription(0x47, 0x98, cmdParams)

    def InitGAPAdvertisingEventCommandFailedIndication(self):
        cmdParams = []
        FailReason = FsciParameter("FailReason", 2)
        cmdParams.append(FailReason)
        return FsciFrameDescription(0x47, 0x99, cmdParams)

    def InitGAPScanningEventStateChangedIndication(self):
        cmdParams = []
        return FsciFrameDescription(0x47, 0x9A, cmdParams)

    def InitGAPScanningEventCommandFailedIndication(self):
        cmdParams = []
        FailReason = FsciParameter("FailReason", 2)
        cmdParams.append(FailReason)
        return FsciFrameDescription(0x47, 0x9B, cmdParams)

    def InitGAPScanningEventDeviceScannedIndication(self):
        cmdParams = []
        ScannedDevice_AddressType = FsciParameter("ScannedDevice_AddressType", 1)
        cmdParams.append(ScannedDevice_AddressType)
        ScannedDevice_Address = FsciParameter("ScannedDevice_Address", 6)
        cmdParams.append(ScannedDevice_Address)
        ScannedDevice_Rssi = FsciParameter("ScannedDevice_Rssi", 1)
        cmdParams.append(ScannedDevice_Rssi)
        ScannedDevice_DataLength = FsciParameter("ScannedDevice_DataLength", 1)
        cmdParams.append(ScannedDevice_DataLength)
        ScannedDevice_Data = FsciParameter("ScannedDevice_Data", 1, ScannedDevice_DataLength)
        cmdParams.append(ScannedDevice_Data)
        ScannedDevice_AdvEventType = FsciParameter("ScannedDevice_AdvEventType", 1)
        cmdParams.append(ScannedDevice_AdvEventType)
        ScannedDevice_DirectRpaUsed = FsciParameter("ScannedDevice_DirectRpaUsed", 1)
        cmdParams.append(ScannedDevice_DirectRpaUsed)
        ScannedDevice_DirectRpa = FsciParameter("ScannedDevice_DirectRpa", 6, ScannedDevice_DirectRpaUsed)
        cmdParams.append(ScannedDevice_DirectRpa)
        ScannedDevice_advertisingAddressResolved = FsciParameter("ScannedDevice_advertisingAddressResolved", 1)
        cmdParams.append(ScannedDevice_advertisingAddressResolved)
        return FsciFrameDescription(0x47, 0x9C, cmdParams)

    def InitGAPConnectionEventConnectedIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        ConnectionParameters_ConnInterval = FsciParameter("ConnectionParameters_ConnInterval", 2)
        cmdParams.append(ConnectionParameters_ConnInterval)
        ConnectionParameters_ConnLatency = FsciParameter("ConnectionParameters_ConnLatency", 2)
        cmdParams.append(ConnectionParameters_ConnLatency)
        ConnectionParameters_SupervisionTimeout = FsciParameter("ConnectionParameters_SupervisionTimeout", 2)
        cmdParams.append(ConnectionParameters_SupervisionTimeout)
        ConnectionParameters_MasterClockAccuracy = FsciParameter("ConnectionParameters_MasterClockAccuracy", 1)
        cmdParams.append(ConnectionParameters_MasterClockAccuracy)
        PeerAddressType = FsciParameter("PeerAddressType", 1)
        cmdParams.append(PeerAddressType)
        PeerAddress = FsciParameter("PeerAddress", 6)
        cmdParams.append(PeerAddress)
        peerRpaResolved = FsciParameter("peerRpaResolved", 1)
        cmdParams.append(peerRpaResolved)
        peerRpa = FsciParameter("peerRpa", 6, peerRpaResolved)
        cmdParams.append(peerRpa)
        localRpaUsed = FsciParameter("localRpaUsed", 1)
        cmdParams.append(localRpaUsed)
        localRpa = FsciParameter("localRpa", 6, localRpaUsed)
        cmdParams.append(localRpa)
        return FsciFrameDescription(0x47, 0x9D, cmdParams)

    def InitGAPConnectionEventPairingRequestIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        PairingParameters_WithBonding = FsciParameter("PairingParameters_WithBonding", 1)
        cmdParams.append(PairingParameters_WithBonding)
        PairingParameters_SecurityModeAndLevel = FsciParameter("PairingParameters_SecurityModeAndLevel", 1)
        cmdParams.append(PairingParameters_SecurityModeAndLevel)
        PairingParameters_MaxEncryptionKeySize = FsciParameter("PairingParameters_MaxEncryptionKeySize", 1)
        cmdParams.append(PairingParameters_MaxEncryptionKeySize)
        PairingParameters_LocalIoCapabilities = FsciParameter("PairingParameters_LocalIoCapabilities", 1)
        cmdParams.append(PairingParameters_LocalIoCapabilities)
        PairingParameters_OobAvailable = FsciParameter("PairingParameters_OobAvailable", 1)
        cmdParams.append(PairingParameters_OobAvailable)
        PairingParameters_CentralKeys = FsciParameter("PairingParameters_CentralKeys", 1)
        cmdParams.append(PairingParameters_CentralKeys)
        PairingParameters_PeripheralKeys = FsciParameter("PairingParameters_PeripheralKeys", 1)
        cmdParams.append(PairingParameters_PeripheralKeys)
        PairingParameters_LeSecureConnectionSupported = FsciParameter("PairingParameters_LeSecureConnectionSupported", 1)
        cmdParams.append(PairingParameters_LeSecureConnectionSupported)
        PairingParameters_UseKeypressNotifications = FsciParameter("PairingParameters_UseKeypressNotifications", 1)
        cmdParams.append(PairingParameters_UseKeypressNotifications)
        return FsciFrameDescription(0x47, 0x9E, cmdParams)

    def InitGAPConnectionEventSlaveSecurityRequestIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        SlaveSecurityRequestParameters_BondAfterPairing = FsciParameter("SlaveSecurityRequestParameters_BondAfterPairing", 1)
        cmdParams.append(SlaveSecurityRequestParameters_BondAfterPairing)
        SlaveSecurityRequestParameters_bAuthenticationRequired = FsciParameter("SlaveSecurityRequestParameters_bAuthenticationRequired", 1)
        cmdParams.append(SlaveSecurityRequestParameters_bAuthenticationRequired)
        return FsciFrameDescription(0x47, 0x9F, cmdParams)

    def InitGAPConnectionEventPairingResponseIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        PairingParameters_WithBonding = FsciParameter("PairingParameters_WithBonding", 1)
        cmdParams.append(PairingParameters_WithBonding)
        PairingParameters_SecurityModeAndLevel = FsciParameter("PairingParameters_SecurityModeAndLevel", 1)
        cmdParams.append(PairingParameters_SecurityModeAndLevel)
        PairingParameters_MaxEncryptionKeySize = FsciParameter("PairingParameters_MaxEncryptionKeySize", 1)
        cmdParams.append(PairingParameters_MaxEncryptionKeySize)
        PairingParameters_LocalIoCapabilities = FsciParameter("PairingParameters_LocalIoCapabilities", 1)
        cmdParams.append(PairingParameters_LocalIoCapabilities)
        PairingParameters_OobAvailable = FsciParameter("PairingParameters_OobAvailable", 1)
        cmdParams.append(PairingParameters_OobAvailable)
        PairingParameters_CentralKeys = FsciParameter("PairingParameters_CentralKeys", 1)
        cmdParams.append(PairingParameters_CentralKeys)
        PairingParameters_PeripheralKeys = FsciParameter("PairingParameters_PeripheralKeys", 1)
        cmdParams.append(PairingParameters_PeripheralKeys)
        PairingParameters_LeSecureConnectionSupported = FsciParameter("PairingParameters_LeSecureConnectionSupported", 1)
        cmdParams.append(PairingParameters_LeSecureConnectionSupported)
        PairingParameters_UseKeypressNotifications = FsciParameter("PairingParameters_UseKeypressNotifications", 1)
        cmdParams.append(PairingParameters_UseKeypressNotifications)
        return FsciFrameDescription(0x47, 0xA0, cmdParams)

    def InitGAPConnectionEventAuthenticationRejectedIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        RejectReason = FsciParameter("RejectReason", 1)
        cmdParams.append(RejectReason)
        return FsciFrameDescription(0x47, 0xA1, cmdParams)

    def InitGAPConnectionEventPasskeyRequestIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        return FsciFrameDescription(0x47, 0xA2, cmdParams)

    def InitGAPConnectionEventOobRequestIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        return FsciFrameDescription(0x47, 0xA3, cmdParams)

    def InitGAPConnectionEventPasskeyDisplayIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        PasskeyForDisplay = FsciParameter("PasskeyForDisplay", 4)
        cmdParams.append(PasskeyForDisplay)
        return FsciFrameDescription(0x47, 0xA4, cmdParams)

    def InitGAPConnectionEventKeyExchangeRequestIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        RequestedKeys = FsciParameter("RequestedKeys", 1)
        cmdParams.append(RequestedKeys)
        RequestedLtkSize = FsciParameter("RequestedLtkSize", 1)
        cmdParams.append(RequestedLtkSize)
        return FsciFrameDescription(0x47, 0xA5, cmdParams)

    def InitGAPConnectionEventKeysReceivedIndication(self):
        cmdParams = []
        # not generated, cursor based approach in observer; see events.py
        return FsciFrameDescription(0x47, 0xA6, cmdParams)

    def InitGAPConnectionEventLongTermKeyRequestIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Ediv = FsciParameter("Ediv", 2)
        cmdParams.append(Ediv)
        RandSize = FsciParameter("RandSize", 1)
        cmdParams.append(RandSize)
        Rand = FsciParameter("Rand", 1, RandSize)
        cmdParams.append(Rand)
        return FsciFrameDescription(0x47, 0xA7, cmdParams)

    def InitGAPConnectionEventEncryptionChangedIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        NewEncryptionState = FsciParameter("NewEncryptionState", 1)
        cmdParams.append(NewEncryptionState)
        return FsciFrameDescription(0x47, 0xA8, cmdParams)

    def InitGAPConnectionEventPairingCompleteIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        PairingStatus = FsciParameter("PairingStatus", 1)
        cmdParams.append(PairingStatus)
        PairingDatadict = {}
        currentList = []
        PairingSuccessfulWithBonding = FsciParameter("PairingSuccessfulWithBonding", 1)
        currentList.append(PairingSuccessfulWithBonding)
        PairingDatadict[0x01] = currentList
        currentList = []
        PairingFailedFailReason = FsciParameter("PairingFailedFailReason", 2)
        currentList.append(PairingFailedFailReason)
        PairingDatadict[0x00] = currentList
        PairingData = FsciParameter("PairingData", 1, PairingStatus, PairingDatadict)
        cmdParams.append(PairingData)
        return FsciFrameDescription(0x47, 0xA9, cmdParams)

    def InitGAPConnectionEventDisconnectedIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Reason = FsciParameter("Reason", 2)
        cmdParams.append(Reason)
        return FsciFrameDescription(0x47, 0xAA, cmdParams)

    def InitGAPConnectionEventRssiReadIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        Rssi = FsciParameter("Rssi", 1)
        cmdParams.append(Rssi)
        return FsciFrameDescription(0x47, 0xAB, cmdParams)

    def InitGAPConnectionEventTxPowerLevelReadIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        TxPowerLevel = FsciParameter("TxPowerLevel", 1)
        cmdParams.append(TxPowerLevel)
        return FsciFrameDescription(0x47, 0xAC, cmdParams)

    def InitGAPConnectionEventPowerReadFailureIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        FailReason = FsciParameter("FailReason", 2)
        cmdParams.append(FailReason)
        return FsciFrameDescription(0x47, 0xAD, cmdParams)

    def InitGAPConnectionEventParameterUpdateRequestIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        ConnectionParameterUpdateRequest_IntervalMin = FsciParameter("ConnectionParameterUpdateRequest_IntervalMin", 2)
        cmdParams.append(ConnectionParameterUpdateRequest_IntervalMin)
        ConnectionParameterUpdateRequest_IntervalMax = FsciParameter("ConnectionParameterUpdateRequest_IntervalMax", 2)
        cmdParams.append(ConnectionParameterUpdateRequest_IntervalMax)
        ConnectionParameterUpdateRequest_SlaveLatency = FsciParameter("ConnectionParameterUpdateRequest_SlaveLatency", 2)
        cmdParams.append(ConnectionParameterUpdateRequest_SlaveLatency)
        ConnectionParameterUpdateRequest_TimeoutMultiplier = FsciParameter("ConnectionParameterUpdateRequest_TimeoutMultiplier", 2)
        cmdParams.append(ConnectionParameterUpdateRequest_TimeoutMultiplier)
        return FsciFrameDescription(0x47, 0xAE, cmdParams)

    def InitGAPConnectionEventParameterUpdateCompleteIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        ConnectionParameterUpdateComplete_Status = FsciParameter("ConnectionParameterUpdateComplete_Status", 2)
        cmdParams.append(ConnectionParameterUpdateComplete_Status)
        ConnectionParameterUpdateComplete_ConnInterval = FsciParameter("ConnectionParameterUpdateComplete_ConnInterval", 2)
        cmdParams.append(ConnectionParameterUpdateComplete_ConnInterval)
        ConnectionParameterUpdateComplete_ConnLatency = FsciParameter("ConnectionParameterUpdateComplete_ConnLatency", 2)
        cmdParams.append(ConnectionParameterUpdateComplete_ConnLatency)
        ConnectionParameterUpdateComplete_SupervisionTimeout = FsciParameter("ConnectionParameterUpdateComplete_SupervisionTimeout", 2)
        cmdParams.append(ConnectionParameterUpdateComplete_SupervisionTimeout)
        return FsciFrameDescription(0x47, 0xAF, cmdParams)

    def InitGAPConnectionEventLeDataLengthChangedIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        LeDataLengthChanged_MaxTxOctets = FsciParameter("LeDataLengthChanged_MaxTxOctets", 2)
        cmdParams.append(LeDataLengthChanged_MaxTxOctets)
        LeDataLengthChanged_MaxTxTime = FsciParameter("LeDataLengthChanged_MaxTxTime", 2)
        cmdParams.append(LeDataLengthChanged_MaxTxTime)
        LeDataLengthChanged_MaxRxOctets = FsciParameter("LeDataLengthChanged_MaxRxOctets", 2)
        cmdParams.append(LeDataLengthChanged_MaxRxOctets)
        LeDataLengthChanged_MaxRxTime = FsciParameter("LeDataLengthChanged_MaxRxTime", 2)
        cmdParams.append(LeDataLengthChanged_MaxRxTime)
        return FsciFrameDescription(0x47, 0xB0, cmdParams)

    def InitGAPConnectionEventLeScOobDataRequestIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        return FsciFrameDescription(0x47, 0xB1, cmdParams)

    def InitGAPConnectionEventLeScDisplayNumericValueIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        ncValue = FsciParameter("ncValue", 4)
        cmdParams.append(ncValue)
        return FsciFrameDescription(0x47, 0xB2, cmdParams)

    def InitGAPConnectionEventLeScKeypressNotificationIndication(self):
        cmdParams = []
        DeviceId = FsciParameter("DeviceId", 1)
        cmdParams.append(DeviceId)
        GapLeScKeypressNotificationParams_keypressNotifType = FsciParameter("GapLeScKeypressNotificationParams_keypressNotifType", 1)
        cmdParams.append(GapLeScKeypressNotificationParams_keypressNotifType)
        return FsciFrameDescription(0x47, 0xB3, cmdParams)

    def InitGAPGenericEventControllerResetCompleteIndication(self):
        cmdParams = []
        return FsciFrameDescription(0x47, 0xB4, cmdParams)

    def InitGAPLeScPublicKeyRegeneratedIndication(self):
        cmdParams = []
        return FsciFrameDescription(0x47, 0xB5, cmdParams)

    def InitGAPGenericEventLeScLocalOobDataIndication(self):
        cmdParams = []
        RandomValue = FsciParameter("RandomValue", 16)
        cmdParams.append(RandomValue)
        ConfirmValue = FsciParameter("ConfirmValue", 16)
        cmdParams.append(ConfirmValue)
        return FsciFrameDescription(0x47, 0xB6, cmdParams)

    def InitGAPGenericEventControllerPrivacyStateChangedIndication(self):
        cmdParams = []
        NewControllerPrivacyState = FsciParameter("NewControllerPrivacyState", 1)
        cmdParams.append(NewControllerPrivacyState)
        return FsciFrameDescription(0x47, 0xB7, cmdParams)

    def InitGAPGenericEventTxPowerLevelSetCompleteIndication(self):
        cmdParams = []
        status = FsciParameter("status", 1)
        cmdParams.append(status)
        return FsciFrameDescription(0x47, 0xB8, cmdParams)

    def InitGAPGetBondedDevicesIdentityInformationIndication(self):
        cmdParams = []
        # not generated, cursor based approach in observer; see events.py
        return FsciFrameDescription(0x47, 0x83, cmdParams)
