/* Source file generated from ThreadIP_1.2.6.xml */
/*
 * The Clear BSD License
 * Copyright 2018 NXP
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
 */
#include "cmd_threadip_1.2.6.h"

#if SOCKET_ENABLE
static const char *OK = "OK";
static const char *ERROR = "ERROR";
#endif  /* SOCKET_ENABLE */

#if MAC_ENABLE
static const char *Success = "Success";
static const char *Notpermitted = "Notpermitted";
static const char *Nomemory = "Nomemory";
#endif  /* MAC_ENABLE */

#if THREADMGMT_ENABLE
static const char *Invalidinstance = "Invalidinstance";
static const char *Invalidparameter = "Invalidparameter";
static const char *UnsupportedAttribute = "UnsupportedAttribute";
static const char *EmptyEntry = "EmptyEntry";
static const char *InvalidValue = "InvalidValue";
static const char *InvalidParam = "InvalidParam";
static const char *Theselectedconfigurationisnotvalid = "Theselectedconfigurationisnotvalid";
static const char *AlreadyConnected = "AlreadyConnected";
static const char *AlreadyCreated = "AlreadyCreated";
static const char *ResetCpuSuccess = "ResetCpuSuccess";
static const char *ResetCpuPending = "ResetCpuPending";
static const char *Operationnotpermiteddevicealreadydisconnected = "Operationnotpermiteddevicealreadydisconnected";
static const char *ScanResult = "ScanResult";
static const char *EnergyDetect = "EnergyDetect";
static const char *ActiveScan = "ActiveScan";
static const char *EnergyDetectAndActiveScan = "EnergyDetectAndActiveScan";
static const char *Failed = "Failed";
static const char *SelectBestChannel = "SelectBestChannel";
static const char *GeneratePSKc = "GeneratePSKc";
static const char *Attaching = "Attaching";
static const char *JoinSuccess = "JoinSuccess";
static const char *JoinFailed = "JoinFailed";
static const char *ScanStarted = "ScanStarted";
static const char *ReceivedBeacon = "ReceivedBeacon";
static const char *ScanEnded = "ScanEnded";
static const char *Disconnected = "Disconnected";
static const char *Connected = "Connected";
static const char *Resettofactorydefault = "Resettofactorydefault";
static const char *Instancerestorestarted = "Instancerestorestarted";
static const char *RouterSynced = "RouterSynced";
static const char *EndDeviceSynced = "EndDeviceSynced";
static const char *Connectingstarted = "Connectingstarted";
static const char *Connectingfailed = "Connectingfailed";
static const char *Connectingdeffered = "Connectingdeffered";
static const char *DeviceisLeader = "DeviceisLeader";
static const char *DeviceisRouter = "DeviceisRouter";
static const char *DeviceisREED = "DeviceisREED";
static const char *DeviceisFullEndDevice = "DeviceisFullEndDevice";
static const char *DeviceisSleepyEndDevice = "DeviceisSleepyEndDevice";
static const char *RequestingGlobalAddress = "RequestingGlobalAddress";
static const char *GlobalAddressassigned = "GlobalAddressassigned";
static const char *RequestingRouterId = "RequestingRouterId";
static const char *RouterIdassigned = "RouterIdassigned";
static const char *RouterIdassignedfailed = "RouterIdassignedfailed";
static const char *Allowdevicetosleep = "Allowdevicetosleep";
static const char *Disallowdevicetosleep = "Disallowdevicetosleep";
static const char *ChildIdassigned = "ChildIdassigned";
static const char *DeviceisMinimalEndDevice = "DeviceisMinimalEndDevice";
static const char *JoinerDiscoveryStarted = "JoinerDiscoveryStarted";
static const char *JoinerDiscoveryFailedNoBeacon = "JoinerDiscoveryFailedNoBeacon";
static const char *JoinerDiscoveryFailedFiltered = "JoinerDiscoveryFailedFiltered";
static const char *JoinerDiscoverySuccess = "JoinerDiscoverySuccess";
static const char *JoinerDtlsSessionStarted = "JoinerDtlsSessionStarted";
static const char *JoinerDtlsError = "JoinerDtlsError";
static const char *JoinerError = "JoinerError";
static const char *JoinerAccepted = "JoinerAccepted";
static const char *CommissionerPetitionStarted = "CommissionerPetitionStarted";
static const char *CommissionerPetitionAccepted = "CommissionerPetitionAccepted";
static const char *CommissionerPetitionRejected = "CommissionerPetitionRejected";
static const char *CommissionerPetitionError = "CommissionerPetitionError";
static const char *CommissionerKeepAliveSent = "CommissionerKeepAliveSent";
static const char *CommissionerError = "CommissionerError";
static const char *CommissionerJoinerDtlsSessionStarted = "CommissionerJoinerDtlsSessionStarted";
static const char *CommissionerJoinerDtlsError = "CommissionerJoinerDtlsError";
static const char *CommissionerJoinerAccepted = "CommissionerJoinerAccepted";
static const char *CommissionerNwkDataSynced = "CommissionerNwkDataSynced";
static const char *CommissionerBrDtlsSessionStarted = "CommissionerBrDtlsSessionStarted";
static const char *CommissionerBrDtlsError = "CommissionerBrDtlsError";
static const char *CommissionerBrError = "CommissionerBrError";
static const char *CommissionerBrAccepted = "CommissionerBrAccepted";
static const char *BrCommissionerDtlsSessionStarted = "BrCommissionerDtlsSessionStarted";
static const char *BrCommissionerDtlsError = "BrCommissionerDtlsError";
static const char *BrCommissionerAccepted = "BrCommissionerAccepted";
static const char *BrCommissionerDataRelayedInbound = "BrCommissionerDataRelayedInbound";
static const char *BrCommissionerDataRelayedOutbound = "BrCommissionerDataRelayedOutbound";
static const char *JoinerrouterJoinerDataRelayedInbound = "JoinerrouterJoinerDataRelayedInbound";
static const char *JoinerrouterJoinerDataRelayedOutbound = "JoinerrouterJoinerDataRelayedOutbound";
static const char *JoinerrouterJoinerAccepted = "JoinerrouterJoinerAccepted";
static const char *StartVendorProvisioning = "StartVendorProvisioning";
static const char *OUT = "OUT";
static const char *IN = "IN";
static const char *JOIN_FIN_REQ = "JOIN_FIN_REQ";
static const char *JOIN_FIN_RSP = "JOIN_FIN_RSP";
static const char *JOIN_ENT_REQ = "JOIN_ENT_REQ";
static const char *JOIN_ENT_RSP = "JOIN_ENT_RSP";
static const char *DTLS_CLOSE_NOTIFY = "DTLS_CLOSE_NOTIFY";
static const char *InvalidParameter = "InvalidParameter";
static const char *NotPermitted = "NotPermitted";
static const char *NoMemory = "NoMemory";
static const char *Error = "Error";
static const char *InvalidInstance = "InvalidInstance";
static const char *NotStarted = "NotStarted";
static const char *FailedNotsupported = "FailedNotsupported";
static const char *FailedNotallowed = "FailedNotallowed";
#endif  /* THREADMGMT_ENABLE */

#if THREADUTILS_ENABLE
static const char *NeighborNotFound = "NeighborNotFound";
static const char *RandomExtendedAddr = "RandomExtendedAddr";
static const char *ShortAddress = "ShortAddress";
static const char *ScanChannelMask = "ScanChannelMask";
static const char *ScanDuration = "ScanDuration";
static const char *Channel = "Channel";
static const char *ShortPanId = "ShortPanId";
static const char *ExtendedPanId = "ExtendedPanId";
static const char *PermitJoin = "PermitJoin";
static const char *RxOnIdle = "RxOnIdle";
static const char *SedPollInterval = "SedPollInterval";
static const char *UniqueExtendedAddress = "UniqueExtendedAddress";
static const char *VendorName = "VendorName";
static const char *ModelName = "ModelName";
static const char *SwVersion = "SwVersion";
static const char *StackVersion = "StackVersion";
static const char *NwkCapabilities = "NwkCapabilities";
static const char *NwkName = "NwkName";
static const char *DeviceType = "DeviceType";
static const char *IsDevConnected = "IsDevConnected";
static const char *IsDevCommissioned = "IsDevCommissioned";
static const char *PartitionId = "PartitionId";
static const char *DeviceRole = "DeviceRole";
static const char *Security_NwkMasterKey = "Security_NwkMasterKey";
static const char *Security_NwkKeySeq = "Security_NwkKeySeq";
static const char *Security_PSKc = "Security_PSKc";
static const char *Security_PSKd = "Security_PSKd";
static const char *VendorData = "VendorData";
static const char *MLPrefix = "MLPrefix";
static const char *MacFilteringEntry = "MacFilteringEntry";
static const char *Security_KeyRotationInterval = "Security_KeyRotationInterval";
static const char *ChildAddrMask = "ChildAddrMask";
static const char *ChildSEDTimeout = "ChildSEDTimeout";
static const char *ChildEDTimeout = "ChildEDTimeout";
static const char *EndDevice_ChildEDReqFullNwkData = "EndDevice_ChildEDReqFullNwkData";
static const char *EndDevice_IsFastPollEnabled = "EndDevice_IsFastPollEnabled";
static const char *SleepyEndDevice_FastPollInterval = "SleepyEndDevice_FastPollInterval";
static const char *JoinLqiThreshold = "JoinLqiThreshold";
static const char *ProvisioningURL = "ProvisioningURL";
static const char *SelectBestChannelEDThreshold = "SelectBestChannelEDThreshold";
static const char *CommissionerMode = "CommissionerMode";
static const char *BorderRouter_BrPrefixEntry = "BorderRouter_BrPrefixEntry";
static const char *SteeringData = "SteeringData";
static const char *Security_KeySwitchGuardTime = "Security_KeySwitchGuardTime";
static const char *ParentHoldTime = "ParentHoldTime";
static const char *Security_Policy = "Security_Policy";
static const char *NVM_RestoreAutoStart = "NVM_RestoreAutoStart";
static const char *NVM_Restore = "NVM_Restore";
static const char *SlaacPolicy = "SlaacPolicy";
static const char *IeeeExtendedAddr = "IeeeExtendedAddr";
static const char *LeaderWeight = "LeaderWeight";
static const char *HashIeeeAddr = "HashIeeeAddr";
static const char *BorderRouter_BrGlobalOnMeshPrefix = "BorderRouter_BrGlobalOnMeshPrefix";
static const char *BorderRouter_BrDefaultRouteOnMeshPrefix = "BorderRouter_BrDefaultRouteOnMeshPrefix";
static const char *BorderRouter_BrExternalIfPrefix = "BorderRouter_BrExternalIfPrefix";
static const char *ActiveTimestamp = "ActiveTimestamp";
static const char *PendingChannel = "PendingChannel";
static const char *PendingChannelMask = "PendingChannelMask";
static const char *PendingXpanId = "PendingXpanId";
static const char *PendingMLprefix = "PendingMLprefix";
static const char *PendingNwkMasterKey = "PendingNwkMasterKey";
static const char *PendingNwkName = "PendingNwkName";
static const char *PendingPanId = "PendingPanId";
static const char *PendingPSK = "PendingPSK";
static const char *PendingSecurityPolicy = "PendingSecurityPolicy";
static const char *PendingNwkKeyRotationInterval = "PendingNwkKeyRotationInterval";
static const char *PendingDelayTimer = "PendingDelayTimer";
static const char *PendingActiveTimestamp = "PendingActiveTimestamp";
static const char *PendingTimestamp = "PendingTimestamp";
static const char *CommissionerId = "CommissionerId";
static const char *JoinerPort = "JoinerPort";
static const char *CommissionerUdpPort = "CommissionerUdpPort";
static const char *DiscoveryReqMacTxOptions = "DiscoveryReqMacTxOptions";
#endif  /* THREADUTILS_ENABLE */

#if IPUTILS_ENABLE
static const char *Addressesmaximumlimitreached = "Addressesmaximumlimitreached";
static const char *RequestTimeout = "RequestTimeout";
static const char *Wrongdestinationaddress = "Wrongdestinationaddress";
static const char *Duplicate = "Duplicate";
static const char *CON = "CON";
static const char *NON = "NON";
static const char *ACK = "ACK";
static const char *GET = "GET";
static const char *POST = "POST";
static const char *PUT = "PUT";
static const char *DELETE = "DELETE";
#endif  /* IPUTILS_ENABLE */

#if MESHCOP_ENABLE
static const char *FALSE_STR = "FALSE";
static const char *TRUE_STR = "TRUE";
static const char *ChannelMask = "ChannelMask";
static const char *PanId = "PanId";
static const char *XpanId = "XpanId";
static const char *NetworkName = "NetworkName";
static const char *PSKc = "PSKc";
static const char *MasterKey = "MasterKey";
static const char *KeySequence = "KeySequence";
static const char *MeshLocalUla = "MeshLocalUla";
static const char *BorderRouterLocator = "BorderRouterLocator";
static const char *CommissionerID = "CommissionerID";
static const char *CommissionerSessionID = "CommissionerSessionID";
static const char *SecurityPolicy = "SecurityPolicy";
static const char *DelayTimer = "DelayTimer";
#endif  /* MESHCOP_ENABLE */

#if OTHERUTILS_ENABLE
static const char *gSuccess_c = "gSuccess_c";
static const char *UNSUPORTED_ATTRIBUTE = "UNSUPORTED_ATTRIBUTE";
static const char *gInvalidParameter_c = "gInvalidParameter_c";
static const char *gReadOnly_c = "gReadOnly_c";
static const char *macLogicalChannel = "macLogicalChannel";
static const char *macMAC_PromiscuousRxIndicationMode = "macMAC_PromiscuousRxIndicationMode";
static const char *macRxOnWhenIdle = "macRxOnWhenIdle";
static const char *SUCCESS = "SUCCESS";
static const char *INVALID_PARAMETER = "INVALID_PARAMETER";
#endif  /* OTHERUTILS_ENABLE */


void SHELL_ThrEventNotify(void *param)
{
	thrEvtContainer_t *container = (thrEvtContainer_t *)param;

	switch (container->id)
	{
#if SOCKET_ENABLE
		case SocketCreateConfirm_FSCI_ID:
			shell_write("SocketCreateConfirm");
			break;

		case SocketShutdownConfirm_FSCI_ID:
			shell_write("SocketShutdownConfirm");
			shell_write(" -> ");
			switch (container->Data.SocketShutdownConfirm.Status)
			{
				case SocketShutdownConfirm_Status_OK:
					shell_write(OK);
					break;
				case SocketShutdownConfirm_Status_ERROR:
					shell_write(ERROR);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.SocketShutdownConfirm.Status);
					break;
			}
			break;

		case SocketBindConfirm_FSCI_ID:
			shell_write("SocketBindConfirm");
			shell_write(" -> ");
			switch (container->Data.SocketBindConfirm.Status)
			{
				case SocketBindConfirm_Status_OK:
					shell_write(OK);
					break;
				case SocketBindConfirm_Status_ERROR:
					shell_write(ERROR);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.SocketBindConfirm.Status);
					break;
			}
			break;

		case SocketSendConfirm_FSCI_ID:
			shell_write("SocketSendConfirm");
			shell_write(" -> ");
			switch (container->Data.SocketSendConfirm.Status)
			{
				case SocketSendConfirm_Status_OK:
					shell_write(OK);
					break;
				case SocketSendConfirm_Status_ERROR:
					shell_write(ERROR);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.SocketSendConfirm.Status);
					break;
			}
			break;

		case SocketSendToConfirm_FSCI_ID:
			shell_write("SocketSendToConfirm");
			shell_write(" -> ");
			switch (container->Data.SocketSendToConfirm.Status)
			{
				case SocketSendToConfirm_Status_OK:
					shell_write(OK);
					break;
				case SocketSendToConfirm_Status_ERROR:
					shell_write(ERROR);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.SocketSendToConfirm.Status);
					break;
			}
			break;

		case SocketReceiveConfirm_FSCI_ID:
			shell_write("SocketReceiveConfirm");
			break;

		case SocketReceiveFromConfirm_FSCI_ID:
			shell_write("SocketReceiveFromConfirm");
			shell_write(" -> ");
			switch (container->Data.SocketReceiveFromConfirm.Status)
			{
				case SocketReceiveFromConfirm_Status_OK:
					shell_write(OK);
					break;
				case SocketReceiveFromConfirm_Status_ERROR:
					shell_write(ERROR);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.SocketReceiveFromConfirm.Status);
					break;
			}
			break;

		case SocketConnectConfirm_FSCI_ID:
			shell_write("SocketConnectConfirm");
			shell_write(" -> ");
			switch (container->Data.SocketConnectConfirm.Status)
			{
				case SocketConnectConfirm_Status_OK:
					shell_write(OK);
					break;
				case SocketConnectConfirm_Status_ERROR:
					shell_write(ERROR);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.SocketConnectConfirm.Status);
					break;
			}
			break;

		case SocketListenConfirm_FSCI_ID:
			shell_write("SocketListenConfirm");
			shell_write(" -> ");
			switch (container->Data.SocketListenConfirm.Status)
			{
				case SocketListenConfirm_Status_OK:
					shell_write(OK);
					break;
				case SocketListenConfirm_Status_ERROR:
					shell_write(ERROR);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.SocketListenConfirm.Status);
					break;
			}
			break;

		case SocketAcceptConfirm_FSCI_ID:
			shell_write("SocketAcceptConfirm");
			shell_write(" -> ");
			switch (container->Data.SocketAcceptConfirm.Status)
			{
				case SocketAcceptConfirm_Status_OK:
					shell_write(OK);
					break;
				case SocketAcceptConfirm_Status_ERROR:
					shell_write(ERROR);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.SocketAcceptConfirm.Status);
					break;
			}
			break;

		case SocketSetOptionConfirm_FSCI_ID:
			shell_write("SocketSetOptionConfirm");
			shell_write(" -> ");
			switch (container->Data.SocketSetOptionConfirm.Status)
			{
				case SocketSetOptionConfirm_Status_OK:
					shell_write(OK);
					break;
				case SocketSetOptionConfirm_Status_ERROR:
					shell_write(ERROR);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.SocketSetOptionConfirm.Status);
					break;
			}
			break;

		case SocketGetOptionConfirm_FSCI_ID:
			shell_write("SocketGetOptionConfirm");
			shell_write(" -> ");
			switch (container->Data.SocketGetOptionConfirm.Status)
			{
				case SocketGetOptionConfirm_Status_OK:
					shell_write(OK);
					break;
				case SocketGetOptionConfirm_Status_ERROR:
					shell_write(ERROR);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.SocketGetOptionConfirm.Status);
					break;
			}
			break;

#endif  /* SOCKET_ENABLE */

#if MAC_ENABLE
		case MAC_MacFilteringAddEntryConfirm_FSCI_ID:
			shell_write("MAC_MacFilteringAddEntryConfirm");
			shell_write(" -> ");
			switch (container->Data.MAC_MacFilteringAddEntryConfirm.Status)
			{
				case MAC_MacFilteringAddEntryConfirm_Status_Success:
					shell_write(Success);
					break;
				case MAC_MacFilteringAddEntryConfirm_Status_Notpermitted:
					shell_write(Notpermitted);
					break;
				case MAC_MacFilteringAddEntryConfirm_Status_Nomemory:
					shell_write(Nomemory);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.MAC_MacFilteringAddEntryConfirm.Status);
					break;
			}
			break;

		case MAC_MacFilteringRemoveEntryConfirm_FSCI_ID:
			shell_write("MAC_MacFilteringRemoveEntryConfirm");
			shell_write(" -> ");
			switch (container->Data.MAC_MacFilteringRemoveEntryConfirm.Status)
			{
				case MAC_MacFilteringRemoveEntryConfirm_Status_Success:
					shell_write(Success);
					break;
				case MAC_MacFilteringRemoveEntryConfirm_Status_Notpermitted:
					shell_write(Notpermitted);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.MAC_MacFilteringRemoveEntryConfirm.Status);
					break;
			}
			break;

		case MAC_MacFilteringEnableConfirm_FSCI_ID:
			shell_write("MAC_MacFilteringEnableConfirm");
			shell_write(" -> ");
			switch (container->Data.MAC_MacFilteringEnableConfirm.Status)
			{
				case MAC_MacFilteringEnableConfirm_Status_Success:
					shell_write(Success);
					break;
				case MAC_MacFilteringEnableConfirm_Status_Notpermitted:
					shell_write(Notpermitted);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.MAC_MacFilteringEnableConfirm.Status);
					break;
			}
			break;

		case MAC_MacFilteringGetTableConfirm_FSCI_ID:
			shell_write("MAC_MacFilteringGetTableConfirm");
			break;

#endif  /* MAC_ENABLE */

#if THREADMGMT_ENABLE
		case THR_SetDeviceConfigConfirm_FSCI_ID:
			shell_write("THR_SetDeviceConfigConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_SetDeviceConfigConfirm.Status)
			{
				case THR_SetDeviceConfigConfirm_Status_Success:
					shell_write(Success);
					break;
				case THR_SetDeviceConfigConfirm_Status_Invalidinstance:
					shell_write(Invalidinstance);
					break;
				case THR_SetDeviceConfigConfirm_Status_Invalidparameter:
					shell_write(Invalidparameter);
					break;
				case THR_SetDeviceConfigConfirm_Status_Notpermitted:
					shell_write(Notpermitted);
					break;
				case THR_SetDeviceConfigConfirm_Status_UnsupportedAttribute:
					shell_write(UnsupportedAttribute);
					break;
				case THR_SetDeviceConfigConfirm_Status_EmptyEntry:
					shell_write(EmptyEntry);
					break;
				case THR_SetDeviceConfigConfirm_Status_InvalidValue:
					shell_write(InvalidValue);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_SetDeviceConfigConfirm.Status);
					break;
			}
			break;

		case THR_NwkScanConfirm_FSCI_ID:
			shell_write("THR_NwkScanConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_NwkScanConfirm.Status)
			{
				case THR_NwkScanConfirm_Status_Success:
					shell_write(Success);
					break;
				case THR_NwkScanConfirm_Status_Invalidinstance:
					shell_write(Invalidinstance);
					break;
				case THR_NwkScanConfirm_Status_InvalidParam:
					shell_write(InvalidParam);
					break;
				case THR_NwkScanConfirm_Status_Nomemory:
					shell_write(Nomemory);
					break;
				case THR_NwkScanConfirm_Status_Theselectedconfigurationisnotvalid:
					shell_write(Theselectedconfigurationisnotvalid);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_NwkScanConfirm.Status);
					break;
			}
			break;

		case THR_CreateNwkConfirm_FSCI_ID:
			shell_write("THR_CreateNwkConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_CreateNwkConfirm.Status)
			{
				case THR_CreateNwkConfirm_Status_OK:
					shell_write(OK);
					break;
				case THR_CreateNwkConfirm_Status_AlreadyConnected:
					shell_write(AlreadyConnected);
					break;
				case THR_CreateNwkConfirm_Status_AlreadyCreated:
					shell_write(AlreadyCreated);
					break;
				case THR_CreateNwkConfirm_Status_Invalidinstance:
					shell_write(Invalidinstance);
					break;
				case THR_CreateNwkConfirm_Status_Theselectedconfigurationisnotvalid:
					shell_write(Theselectedconfigurationisnotvalid);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_CreateNwkConfirm.Status);
					break;
			}
			break;

		case THR_JoinConfirm_FSCI_ID:
			shell_write("THR_JoinConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_JoinConfirm.Status)
			{
				case THR_JoinConfirm_Status_OK:
					shell_write(OK);
					break;
				case THR_JoinConfirm_Status_Invalidinstance:
					shell_write(Invalidinstance);
					break;
				case THR_JoinConfirm_Status_Theselectedconfigurationisnotvalid:
					shell_write(Theselectedconfigurationisnotvalid);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_JoinConfirm.Status);
					break;
			}
			break;

		case THR_FactoryResetConfirm_FSCI_ID:
			shell_write("THR_FactoryResetConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_FactoryResetConfirm.Status)
			{
				case THR_FactoryResetConfirm_Status_Success:
					shell_write(Success);
					break;
				case THR_FactoryResetConfirm_Status_Theselectedconfigurationisnotvalid:
					shell_write(Theselectedconfigurationisnotvalid);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_FactoryResetConfirm.Status);
					break;
			}
			break;

		case THR_CpuResetConfirm_FSCI_ID:
			shell_write("THR_CpuResetConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_CpuResetConfirm.Status)
			{
				case THR_CpuResetConfirm_Status_Success:
					shell_write(Success);
					break;
				case THR_CpuResetConfirm_Status_Theselectedconfigurationisnotvalid:
					shell_write(Theselectedconfigurationisnotvalid);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_CpuResetConfirm.Status);
					break;
			}
			break;

		case THR_CpuResetIndication_FSCI_ID:
			shell_write("THR_CpuResetIndication");
			shell_write(" -> ");
			switch (container->Data.THR_CpuResetIndication.Status)
			{
				case THR_CpuResetIndication_Status_ResetCpuSuccess:
					shell_write(ResetCpuSuccess);
					break;
				case THR_CpuResetIndication_Status_ResetCpuPending:
					shell_write(ResetCpuPending);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_CpuResetIndication.Status);
					break;
			}
			break;

		case THR_DisconnectConfirm_FSCI_ID:
			shell_write("THR_DisconnectConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_DisconnectConfirm.Status)
			{
				case THR_DisconnectConfirm_Status_OK:
					shell_write(OK);
					break;
				case THR_DisconnectConfirm_Status_Invalidinstance:
					shell_write(Invalidinstance);
					break;
				case THR_DisconnectConfirm_Status_Operationnotpermiteddevicealreadydisconnected:
					shell_write(Operationnotpermiteddevicealreadydisconnected);
					break;
				case THR_DisconnectConfirm_Status_Theselectedconfigurationisnotvalid:
					shell_write(Theselectedconfigurationisnotvalid);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_DisconnectConfirm.Status);
					break;
			}
			break;

		case THR_AttachConfirm_FSCI_ID:
			shell_write("THR_AttachConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_AttachConfirm.Status)
			{
				case THR_AttachConfirm_Status_OK:
					shell_write(OK);
					break;
				case THR_AttachConfirm_Status_Invalidinstance:
					shell_write(Invalidinstance);
					break;
				case THR_AttachConfirm_Status_Theselectedconfigurationisnotvalid:
					shell_write(Theselectedconfigurationisnotvalid);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_AttachConfirm.Status);
					break;
			}
			break;

		case THR_PromoteAsRouterConfirm_FSCI_ID:
			shell_write("THR_PromoteAsRouterConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_PromoteAsRouterConfirm.Status)
			{
				case THR_PromoteAsRouterConfirm_Status_OK:
					shell_write(OK);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_PromoteAsRouterConfirm.Status);
					break;
			}
			break;

		case THR_EventNwkScanConfirm_FSCI_ID:
			shell_write("THR_EventNwkScanConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_EventNwkScanConfirm.EventStatus)
			{
				case THR_EventNwkScanConfirm_EventStatus_ScanResult:
					shell_write(ScanResult);
					break;
				default:
					shell_printf("Unrecognized status 0x%04X", container->Data.THR_EventNwkScanConfirm.EventStatus);
					break;
			}
			shell_write(" -> ");
			switch (container->Data.THR_EventNwkScanConfirm.ScanType)
			{
				case THR_EventNwkScanConfirm_ScanType_EnergyDetect:
					shell_write(EnergyDetect);
					break;
				case THR_EventNwkScanConfirm_ScanType_ActiveScan:
					shell_write(ActiveScan);
					break;
				case THR_EventNwkScanConfirm_ScanType_EnergyDetectAndActiveScan:
					shell_write(EnergyDetectAndActiveScan);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_EventNwkScanConfirm.ScanType);
					break;
			}
			break;

		case THR_EventNwkCreateConfirm_FSCI_ID:
			shell_write("THR_EventNwkCreateConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_EventNwkCreateConfirm.EventStatus)
			{
				case THR_EventNwkCreateConfirm_EventStatus_Success:
					shell_write(Success);
					break;
				case THR_EventNwkCreateConfirm_EventStatus_Failed:
					shell_write(Failed);
					break;
				case THR_EventNwkCreateConfirm_EventStatus_SelectBestChannel:
					shell_write(SelectBestChannel);
					break;
				case THR_EventNwkCreateConfirm_EventStatus_GeneratePSKc:
					shell_write(GeneratePSKc);
					break;
				default:
					shell_printf("Unrecognized status 0x%04X", container->Data.THR_EventNwkCreateConfirm.EventStatus);
					break;
			}
			break;

		case THR_EventNwkJoinConfirm_FSCI_ID:
			shell_write("THR_EventNwkJoinConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_EventNwkJoinConfirm.EventStatus)
			{
				case THR_EventNwkJoinConfirm_EventStatus_Attaching:
					shell_write(Attaching);
					break;
				case THR_EventNwkJoinConfirm_EventStatus_JoinSuccess:
					shell_write(JoinSuccess);
					break;
				case THR_EventNwkJoinConfirm_EventStatus_JoinFailed:
					shell_write(JoinFailed);
					break;
				default:
					shell_printf("Unrecognized status 0x%04X", container->Data.THR_EventNwkJoinConfirm.EventStatus);
					break;
			}
			break;

		case THR_EventNwkJoinSelectParentsConfirm_FSCI_ID:
			shell_write("THR_EventNwkJoinSelectParentsConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_EventNwkJoinSelectParentsConfirm.EventStatus)
			{
				case THR_EventNwkJoinSelectParentsConfirm_EventStatus_ScanStarted:
					shell_write(ScanStarted);
					break;
				case THR_EventNwkJoinSelectParentsConfirm_EventStatus_ReceivedBeacon:
					shell_write(ReceivedBeacon);
					break;
				case THR_EventNwkJoinSelectParentsConfirm_EventStatus_ScanEnded:
					shell_write(ScanEnded);
					break;
				default:
					shell_printf("Unrecognized status 0x%04X", container->Data.THR_EventNwkJoinSelectParentsConfirm.EventStatus);
					break;
			}
			break;

		case THR_EventGeneralConfirm_FSCI_ID:
			shell_write("THR_EventGeneralConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_EventGeneralConfirm.EventStatus)
			{
				case THR_EventGeneralConfirm_EventStatus_Disconnected:
					shell_write(Disconnected);
					break;
				case THR_EventGeneralConfirm_EventStatus_Connected:
					shell_write(Connected);
					break;
				case THR_EventGeneralConfirm_EventStatus_Resettofactorydefault:
					shell_write(Resettofactorydefault);
					break;
				case THR_EventGeneralConfirm_EventStatus_Instancerestorestarted:
					shell_write(Instancerestorestarted);
					break;
				case THR_EventGeneralConfirm_EventStatus_RouterSynced:
					shell_write(RouterSynced);
					break;
				case THR_EventGeneralConfirm_EventStatus_EndDeviceSynced:
					shell_write(EndDeviceSynced);
					break;
				case THR_EventGeneralConfirm_EventStatus_Connectingstarted:
					shell_write(Connectingstarted);
					break;
				case THR_EventGeneralConfirm_EventStatus_Connectingfailed:
					shell_write(Connectingfailed);
					break;
				case THR_EventGeneralConfirm_EventStatus_Connectingdeffered:
					shell_write(Connectingdeffered);
					break;
				case THR_EventGeneralConfirm_EventStatus_DeviceisLeader:
					shell_write(DeviceisLeader);
					break;
				case THR_EventGeneralConfirm_EventStatus_DeviceisRouter:
					shell_write(DeviceisRouter);
					break;
				case THR_EventGeneralConfirm_EventStatus_DeviceisREED:
					shell_write(DeviceisREED);
					break;
				case THR_EventGeneralConfirm_EventStatus_DeviceisFullEndDevice:
					shell_write(DeviceisFullEndDevice);
					break;
				case THR_EventGeneralConfirm_EventStatus_DeviceisSleepyEndDevice:
					shell_write(DeviceisSleepyEndDevice);
					break;
				case THR_EventGeneralConfirm_EventStatus_RequestingGlobalAddress:
					shell_write(RequestingGlobalAddress);
					break;
				case THR_EventGeneralConfirm_EventStatus_GlobalAddressassigned:
					shell_write(GlobalAddressassigned);
					break;
				case THR_EventGeneralConfirm_EventStatus_RequestingRouterId:
					shell_write(RequestingRouterId);
					break;
				case THR_EventGeneralConfirm_EventStatus_RouterIdassigned:
					shell_write(RouterIdassigned);
					break;
				case THR_EventGeneralConfirm_EventStatus_RouterIdassignedfailed:
					shell_write(RouterIdassignedfailed);
					break;
				case THR_EventGeneralConfirm_EventStatus_Allowdevicetosleep:
					shell_write(Allowdevicetosleep);
					break;
				case THR_EventGeneralConfirm_EventStatus_Disallowdevicetosleep:
					shell_write(Disallowdevicetosleep);
					break;
				case THR_EventGeneralConfirm_EventStatus_ChildIdassigned:
					shell_write(ChildIdassigned);
					break;
				case THR_EventGeneralConfirm_EventStatus_DeviceisMinimalEndDevice:
					shell_write(DeviceisMinimalEndDevice);
					break;
				default:
					shell_printf("Unrecognized status 0x%04X", container->Data.THR_EventGeneralConfirm.EventStatus);
					break;
			}
			break;

		case THR_EventNwkCommissioningIndication_FSCI_ID:
			shell_write("THR_EventNwkCommissioningIndication");
			shell_write(" -> ");
			switch (container->Data.THR_EventNwkCommissioningIndication.EventStatus)
			{
				case THR_EventNwkCommissioningIndication_EventStatus_JoinerDiscoveryStarted:
					shell_write(JoinerDiscoveryStarted);
					break;
				case THR_EventNwkCommissioningIndication_EventStatus_JoinerDiscoveryFailedNoBeacon:
					shell_write(JoinerDiscoveryFailedNoBeacon);
					break;
				case THR_EventNwkCommissioningIndication_EventStatus_JoinerDiscoveryFailedFiltered:
					shell_write(JoinerDiscoveryFailedFiltered);
					break;
				case THR_EventNwkCommissioningIndication_EventStatus_JoinerDiscoverySuccess:
					shell_write(JoinerDiscoverySuccess);
					break;
				case THR_EventNwkCommissioningIndication_EventStatus_JoinerDtlsSessionStarted:
					shell_write(JoinerDtlsSessionStarted);
					break;
				case THR_EventNwkCommissioningIndication_EventStatus_JoinerDtlsError:
					shell_write(JoinerDtlsError);
					break;
				case THR_EventNwkCommissioningIndication_EventStatus_JoinerError:
					shell_write(JoinerError);
					break;
				case THR_EventNwkCommissioningIndication_EventStatus_JoinerAccepted:
					shell_write(JoinerAccepted);
					break;
				case THR_EventNwkCommissioningIndication_EventStatus_CommissionerPetitionStarted:
					shell_write(CommissionerPetitionStarted);
					break;
				case THR_EventNwkCommissioningIndication_EventStatus_CommissionerPetitionAccepted:
					shell_write(CommissionerPetitionAccepted);
					break;
				case THR_EventNwkCommissioningIndication_EventStatus_CommissionerPetitionRejected:
					shell_write(CommissionerPetitionRejected);
					break;
				case THR_EventNwkCommissioningIndication_EventStatus_CommissionerPetitionError:
					shell_write(CommissionerPetitionError);
					break;
				case THR_EventNwkCommissioningIndication_EventStatus_CommissionerKeepAliveSent:
					shell_write(CommissionerKeepAliveSent);
					break;
				case THR_EventNwkCommissioningIndication_EventStatus_CommissionerError:
					shell_write(CommissionerError);
					break;
				case THR_EventNwkCommissioningIndication_EventStatus_CommissionerJoinerDtlsSessionStarted:
					shell_write(CommissionerJoinerDtlsSessionStarted);
					break;
				case THR_EventNwkCommissioningIndication_EventStatus_CommissionerJoinerDtlsError:
					shell_write(CommissionerJoinerDtlsError);
					break;
				case THR_EventNwkCommissioningIndication_EventStatus_CommissionerJoinerAccepted:
					shell_write(CommissionerJoinerAccepted);
					break;
				case THR_EventNwkCommissioningIndication_EventStatus_CommissionerNwkDataSynced:
					shell_write(CommissionerNwkDataSynced);
					break;
				case THR_EventNwkCommissioningIndication_EventStatus_CommissionerBrDtlsSessionStarted:
					shell_write(CommissionerBrDtlsSessionStarted);
					break;
				case THR_EventNwkCommissioningIndication_EventStatus_CommissionerBrDtlsError:
					shell_write(CommissionerBrDtlsError);
					break;
				case THR_EventNwkCommissioningIndication_EventStatus_CommissionerBrError:
					shell_write(CommissionerBrError);
					break;
				case THR_EventNwkCommissioningIndication_EventStatus_CommissionerBrAccepted:
					shell_write(CommissionerBrAccepted);
					break;
				case THR_EventNwkCommissioningIndication_EventStatus_BrCommissionerDtlsSessionStarted:
					shell_write(BrCommissionerDtlsSessionStarted);
					break;
				case THR_EventNwkCommissioningIndication_EventStatus_BrCommissionerDtlsError:
					shell_write(BrCommissionerDtlsError);
					break;
				case THR_EventNwkCommissioningIndication_EventStatus_BrCommissionerAccepted:
					shell_write(BrCommissionerAccepted);
					break;
				case THR_EventNwkCommissioningIndication_EventStatus_BrCommissionerDataRelayedInbound:
					shell_write(BrCommissionerDataRelayedInbound);
					break;
				case THR_EventNwkCommissioningIndication_EventStatus_BrCommissionerDataRelayedOutbound:
					shell_write(BrCommissionerDataRelayedOutbound);
					break;
				case THR_EventNwkCommissioningIndication_EventStatus_JoinerrouterJoinerDataRelayedInbound:
					shell_write(JoinerrouterJoinerDataRelayedInbound);
					break;
				case THR_EventNwkCommissioningIndication_EventStatus_JoinerrouterJoinerDataRelayedOutbound:
					shell_write(JoinerrouterJoinerDataRelayedOutbound);
					break;
				case THR_EventNwkCommissioningIndication_EventStatus_JoinerrouterJoinerAccepted:
					shell_write(JoinerrouterJoinerAccepted);
					break;
				case THR_EventNwkCommissioningIndication_EventStatus_StartVendorProvisioning:
					shell_write(StartVendorProvisioning);
					break;
				default:
					shell_printf("Unrecognized status 0x%04X", container->Data.THR_EventNwkCommissioningIndication.EventStatus);
					break;
			}
			break;

		case THR_CommissioningDiagnosticIndication_FSCI_ID:
			shell_write("THR_CommissioningDiagnosticIndication");
			shell_write(" -> ");
			switch (container->Data.THR_CommissioningDiagnosticIndication.Direction)
			{
				case THR_CommissioningDiagnosticIndication_Direction_OUT:
					shell_write(OUT);
					break;
				case THR_CommissioningDiagnosticIndication_Direction_IN:
					shell_write(IN);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_CommissioningDiagnosticIndication.Direction);
					break;
			}
			shell_write(" -> ");
			switch (container->Data.THR_CommissioningDiagnosticIndication.Type)
			{
				case THR_CommissioningDiagnosticIndication_Type_JOIN_FIN_REQ:
					shell_write(JOIN_FIN_REQ);
					break;
				case THR_CommissioningDiagnosticIndication_Type_JOIN_FIN_RSP:
					shell_write(JOIN_FIN_RSP);
					break;
				case THR_CommissioningDiagnosticIndication_Type_JOIN_ENT_REQ:
					shell_write(JOIN_ENT_REQ);
					break;
				case THR_CommissioningDiagnosticIndication_Type_JOIN_ENT_RSP:
					shell_write(JOIN_ENT_RSP);
					break;
				case THR_CommissioningDiagnosticIndication_Type_DTLS_CLOSE_NOTIFY:
					shell_write(DTLS_CLOSE_NOTIFY);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_CommissioningDiagnosticIndication.Type);
					break;
			}
			break;

		case THR_MgmtDiagnosticGetConfirm_FSCI_ID:
			shell_write("THR_MgmtDiagnosticGetConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_MgmtDiagnosticGetConfirm.Status)
			{
				case THR_MgmtDiagnosticGetConfirm_Status_Success:
					shell_write(Success);
					break;
				case THR_MgmtDiagnosticGetConfirm_Status_InvalidParameter:
					shell_write(InvalidParameter);
					break;
				case THR_MgmtDiagnosticGetConfirm_Status_NotPermitted:
					shell_write(NotPermitted);
					break;
				case THR_MgmtDiagnosticGetConfirm_Status_NoMemory:
					shell_write(NoMemory);
					break;
				case THR_MgmtDiagnosticGetConfirm_Status_Error:
					shell_write(Error);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_MgmtDiagnosticGetConfirm.Status);
					break;
			}
			break;

		case THR_DiagTestGetConfirm_FSCI_ID:
			shell_write("THR_DiagTestGetConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_DiagTestGetConfirm.Status)
			{
				case THR_DiagTestGetConfirm_Status_Success:
					shell_write(Success);
					break;
				case THR_DiagTestGetConfirm_Status_InvalidParameter:
					shell_write(InvalidParameter);
					break;
				case THR_DiagTestGetConfirm_Status_NotPermitted:
					shell_write(NotPermitted);
					break;
				case THR_DiagTestGetConfirm_Status_Error:
					shell_write(Error);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_DiagTestGetConfirm.Status);
					break;
			}
			break;

		case THR_MgmtDiagnosticResetConfirm_FSCI_ID:
			shell_write("THR_MgmtDiagnosticResetConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_MgmtDiagnosticResetConfirm.Status)
			{
				case THR_MgmtDiagnosticResetConfirm_Status_Success:
					shell_write(Success);
					break;
				case THR_MgmtDiagnosticResetConfirm_Status_InvalidParameter:
					shell_write(InvalidParameter);
					break;
				case THR_MgmtDiagnosticResetConfirm_Status_NotPermitted:
					shell_write(NotPermitted);
					break;
				case THR_MgmtDiagnosticResetConfirm_Status_Error:
					shell_write(Error);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_MgmtDiagnosticResetConfirm.Status);
					break;
			}
			break;

		case THR_MgmtReadMemoryConfirm_FSCI_ID:
			shell_write("THR_MgmtReadMemoryConfirm");
			break;

		case THR_MgmtWriteMemoryConfirm_FSCI_ID:
			shell_write("THR_MgmtWriteMemoryConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_MgmtWriteMemoryConfirm.Status)
			{
				case THR_MgmtWriteMemoryConfirm_Status_Success:
					shell_write(Success);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_MgmtWriteMemoryConfirm.Status);
					break;
			}
			break;

		case THR_SetManualSlaacIIDConfirm_FSCI_ID:
			shell_write("THR_SetManualSlaacIIDConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_SetManualSlaacIIDConfirm.Status)
			{
				case THR_SetManualSlaacIIDConfirm_Status_OK:
					shell_write(OK);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_SetManualSlaacIIDConfirm.Status);
					break;
			}
			break;

		case THR_SendProactiveAddrNotifConfirm_FSCI_ID:
			shell_write("THR_SendProactiveAddrNotifConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_SendProactiveAddrNotifConfirm.Status)
			{
				case THR_SendProactiveAddrNotifConfirm_Status_Success:
					shell_write(Success);
					break;
				case THR_SendProactiveAddrNotifConfirm_Status_Failed:
					shell_write(Failed);
					break;
				case THR_SendProactiveAddrNotifConfirm_Status_InvalidInstance:
					shell_write(InvalidInstance);
					break;
				case THR_SendProactiveAddrNotifConfirm_Status_NotStarted:
					shell_write(NotStarted);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_SendProactiveAddrNotifConfirm.Status);
					break;
			}
			break;

		case THR_NwkDiscoveryConfirm_FSCI_ID:
			shell_write("THR_NwkDiscoveryConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_NwkDiscoveryConfirm.Status)
			{
				case THR_NwkDiscoveryConfirm_Status_OK:
					shell_write(OK);
					break;
				case THR_NwkDiscoveryConfirm_Status_Invalidinstance:
					shell_write(Invalidinstance);
					break;
				case THR_NwkDiscoveryConfirm_Status_Theselectedconfigurationisnotvalid:
					shell_write(Theselectedconfigurationisnotvalid);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_NwkDiscoveryConfirm.Status);
					break;
			}
			break;

		case THR_NwkDiscoveryStopConfirm_FSCI_ID:
			shell_write("THR_NwkDiscoveryStopConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_NwkDiscoveryStopConfirm.Status)
			{
				case THR_NwkDiscoveryStopConfirm_Status_OK:
					shell_write(OK);
					break;
				case THR_NwkDiscoveryStopConfirm_Status_Invalidinstance:
					shell_write(Invalidinstance);
					break;
				case THR_NwkDiscoveryStopConfirm_Status_Theselectedconfigurationisnotvalid:
					shell_write(Theselectedconfigurationisnotvalid);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_NwkDiscoveryStopConfirm.Status);
					break;
			}
			break;

		case THR_SearchNwkWithAnounceConfirm_FSCI_ID:
			shell_write("THR_SearchNwkWithAnounceConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_SearchNwkWithAnounceConfirm.Status)
			{
				case THR_SearchNwkWithAnounceConfirm_Status_OK:
					shell_write(OK);
					break;
				case THR_SearchNwkWithAnounceConfirm_Status_Invalidinstance:
					shell_write(Invalidinstance);
					break;
				case THR_SearchNwkWithAnounceConfirm_Status_Theselectedconfigurationisnotvalid:
					shell_write(Theselectedconfigurationisnotvalid);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_SearchNwkWithAnounceConfirm.Status);
					break;
			}
			break;

		case THR_MgmtDiagnosticGetRspIndication_FSCI_ID:
			shell_write("THR_MgmtDiagnosticGetRspIndication");
			shell_write(" -> ");
			switch (container->Data.THR_MgmtDiagnosticGetRspIndication.Status)
			{
				case THR_MgmtDiagnosticGetRspIndication_Status_Success:
					shell_write(Success);
					break;
				case THR_MgmtDiagnosticGetRspIndication_Status_FailedNotsupported:
					shell_write(FailedNotsupported);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_MgmtDiagnosticGetRspIndication.Status);
					break;
			}
			break;

		case THR_DiagTestGetRspIndication_FSCI_ID:
			shell_write("THR_DiagTestGetRspIndication");
			shell_write(" -> ");
			switch (container->Data.THR_DiagTestGetRspIndication.Status)
			{
				case THR_DiagTestGetRspIndication_Status_Success:
					shell_write(Success);
					break;
				case THR_DiagTestGetRspIndication_Status_FailedNotsupported:
					shell_write(FailedNotsupported);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_DiagTestGetRspIndication.Status);
					break;
			}
			break;

		case THR_MgmtDiagnosticDiagTestEventIndication_FSCI_ID:
			shell_write("THR_MgmtDiagnosticDiagTestEventIndication");
			break;

		case THR_MgmtDiagnosticResetRspIndication_FSCI_ID:
			shell_write("THR_MgmtDiagnosticResetRspIndication");
			shell_write(" -> ");
			switch (container->Data.THR_MgmtDiagnosticResetRspIndication.Status)
			{
				case THR_MgmtDiagnosticResetRspIndication_Status_Success:
					shell_write(Success);
					break;
				case THR_MgmtDiagnosticResetRspIndication_Status_FailedNotallowed:
					shell_write(FailedNotallowed);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_MgmtDiagnosticResetRspIndication.Status);
					break;
			}
			break;

#endif  /* THREADMGMT_ENABLE */

#if THREADUTILS_ENABLE
		case THR_SetNwkIdTimeoutConfirm_FSCI_ID:
			shell_write("THR_SetNwkIdTimeoutConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_SetNwkIdTimeoutConfirm.Status)
			{
				case THR_SetNwkIdTimeoutConfirm_Status_Success:
					shell_write(Success);
					break;
				case THR_SetNwkIdTimeoutConfirm_Status_Notpermitted:
					shell_write(Notpermitted);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_SetNwkIdTimeoutConfirm.Status);
					break;
			}
			break;

		case THR_SetThresholdConfirm_FSCI_ID:
			shell_write("THR_SetThresholdConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_SetThresholdConfirm.Status)
			{
				case THR_SetThresholdConfirm_Status_Success:
					shell_write(Success);
					break;
				case THR_SetThresholdConfirm_Status_InvalidParameter:
					shell_write(InvalidParameter);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_SetThresholdConfirm.Status);
					break;
			}
			break;

		case THR_GetNeighborInfoConfirm_FSCI_ID:
			shell_write("THR_GetNeighborInfoConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_GetNeighborInfoConfirm.Status)
			{
				case THR_GetNeighborInfoConfirm_Status_Success:
					shell_write(Success);
					break;
				case THR_GetNeighborInfoConfirm_Status_NeighborNotFound:
					shell_write(NeighborNotFound);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_GetNeighborInfoConfirm.Status);
					break;
			}
			break;

		case THR_GetChildrenTableConfirm_FSCI_ID:
			shell_write("THR_GetChildrenTableConfirm");
			break;

		case THR_GetNeighborTableConfirm_FSCI_ID:
			shell_write("THR_GetNeighborTableConfirm");
			break;

		case THR_GetRoutingTableConfirm_FSCI_ID:
			shell_write("THR_GetRoutingTableConfirm");
			break;

		case THR_GetAttrConfirm_FSCI_ID:
			shell_write("THR_GetAttrConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_GetAttrConfirm.AttributeId)
			{
				case THR_GetAttrConfirm_AttributeId_RandomExtendedAddr:
					shell_write(RandomExtendedAddr);
					break;
				case THR_GetAttrConfirm_AttributeId_ShortAddress:
					shell_write(ShortAddress);
					break;
				case THR_GetAttrConfirm_AttributeId_ScanChannelMask:
					shell_write(ScanChannelMask);
					break;
				case THR_GetAttrConfirm_AttributeId_ScanDuration:
					shell_write(ScanDuration);
					break;
				case THR_GetAttrConfirm_AttributeId_Channel:
					shell_write(Channel);
					break;
				case THR_GetAttrConfirm_AttributeId_ShortPanId:
					shell_write(ShortPanId);
					break;
				case THR_GetAttrConfirm_AttributeId_ExtendedPanId:
					shell_write(ExtendedPanId);
					break;
				case THR_GetAttrConfirm_AttributeId_PermitJoin:
					shell_write(PermitJoin);
					break;
				case THR_GetAttrConfirm_AttributeId_RxOnIdle:
					shell_write(RxOnIdle);
					break;
				case THR_GetAttrConfirm_AttributeId_SedPollInterval:
					shell_write(SedPollInterval);
					break;
				case THR_GetAttrConfirm_AttributeId_UniqueExtendedAddress:
					shell_write(UniqueExtendedAddress);
					break;
				case THR_GetAttrConfirm_AttributeId_VendorName:
					shell_write(VendorName);
					break;
				case THR_GetAttrConfirm_AttributeId_ModelName:
					shell_write(ModelName);
					break;
				case THR_GetAttrConfirm_AttributeId_SwVersion:
					shell_write(SwVersion);
					break;
				case THR_GetAttrConfirm_AttributeId_StackVersion:
					shell_write(StackVersion);
					break;
				case THR_GetAttrConfirm_AttributeId_NwkCapabilities:
					shell_write(NwkCapabilities);
					break;
				case THR_GetAttrConfirm_AttributeId_NwkName:
					shell_write(NwkName);
					break;
				case THR_GetAttrConfirm_AttributeId_DeviceType:
					shell_write(DeviceType);
					break;
				case THR_GetAttrConfirm_AttributeId_IsDevConnected:
					shell_write(IsDevConnected);
					break;
				case THR_GetAttrConfirm_AttributeId_IsDevCommissioned:
					shell_write(IsDevCommissioned);
					break;
				case THR_GetAttrConfirm_AttributeId_PartitionId:
					shell_write(PartitionId);
					break;
				case THR_GetAttrConfirm_AttributeId_DeviceRole:
					shell_write(DeviceRole);
					break;
				case THR_GetAttrConfirm_AttributeId_Security_NwkMasterKey:
					shell_write(Security_NwkMasterKey);
					break;
				case THR_GetAttrConfirm_AttributeId_Security_NwkKeySeq:
					shell_write(Security_NwkKeySeq);
					break;
				case THR_GetAttrConfirm_AttributeId_Security_PSKc:
					shell_write(Security_PSKc);
					break;
				case THR_GetAttrConfirm_AttributeId_Security_PSKd:
					shell_write(Security_PSKd);
					break;
				case THR_GetAttrConfirm_AttributeId_VendorData:
					shell_write(VendorData);
					break;
				case THR_GetAttrConfirm_AttributeId_MLPrefix:
					shell_write(MLPrefix);
					break;
				case THR_GetAttrConfirm_AttributeId_MacFilteringEntry:
					shell_write(MacFilteringEntry);
					break;
				case THR_GetAttrConfirm_AttributeId_Security_KeyRotationInterval:
					shell_write(Security_KeyRotationInterval);
					break;
				case THR_GetAttrConfirm_AttributeId_ChildAddrMask:
					shell_write(ChildAddrMask);
					break;
				case THR_GetAttrConfirm_AttributeId_ChildSEDTimeout:
					shell_write(ChildSEDTimeout);
					break;
				case THR_GetAttrConfirm_AttributeId_ChildEDTimeout:
					shell_write(ChildEDTimeout);
					break;
				case THR_GetAttrConfirm_AttributeId_EndDevice_ChildEDReqFullNwkData:
					shell_write(EndDevice_ChildEDReqFullNwkData);
					break;
				case THR_GetAttrConfirm_AttributeId_EndDevice_IsFastPollEnabled:
					shell_write(EndDevice_IsFastPollEnabled);
					break;
				case THR_GetAttrConfirm_AttributeId_SleepyEndDevice_FastPollInterval:
					shell_write(SleepyEndDevice_FastPollInterval);
					break;
				case THR_GetAttrConfirm_AttributeId_JoinLqiThreshold:
					shell_write(JoinLqiThreshold);
					break;
				case THR_GetAttrConfirm_AttributeId_ProvisioningURL:
					shell_write(ProvisioningURL);
					break;
				case THR_GetAttrConfirm_AttributeId_SelectBestChannelEDThreshold:
					shell_write(SelectBestChannelEDThreshold);
					break;
				case THR_GetAttrConfirm_AttributeId_CommissionerMode:
					shell_write(CommissionerMode);
					break;
				case THR_GetAttrConfirm_AttributeId_BorderRouter_BrPrefixEntry:
					shell_write(BorderRouter_BrPrefixEntry);
					break;
				case THR_GetAttrConfirm_AttributeId_SteeringData:
					shell_write(SteeringData);
					break;
				case THR_GetAttrConfirm_AttributeId_Security_KeySwitchGuardTime:
					shell_write(Security_KeySwitchGuardTime);
					break;
				case THR_GetAttrConfirm_AttributeId_ParentHoldTime:
					shell_write(ParentHoldTime);
					break;
				case THR_GetAttrConfirm_AttributeId_Security_Policy:
					shell_write(Security_Policy);
					break;
				case THR_GetAttrConfirm_AttributeId_NVM_RestoreAutoStart:
					shell_write(NVM_RestoreAutoStart);
					break;
				case THR_GetAttrConfirm_AttributeId_NVM_Restore:
					shell_write(NVM_Restore);
					break;
				case THR_GetAttrConfirm_AttributeId_SlaacPolicy:
					shell_write(SlaacPolicy);
					break;
				case THR_GetAttrConfirm_AttributeId_IeeeExtendedAddr:
					shell_write(IeeeExtendedAddr);
					break;
				case THR_GetAttrConfirm_AttributeId_LeaderWeight:
					shell_write(LeaderWeight);
					break;
				case THR_GetAttrConfirm_AttributeId_HashIeeeAddr:
					shell_write(HashIeeeAddr);
					break;
				case THR_GetAttrConfirm_AttributeId_BorderRouter_BrGlobalOnMeshPrefix:
					shell_write(BorderRouter_BrGlobalOnMeshPrefix);
					break;
				case THR_GetAttrConfirm_AttributeId_BorderRouter_BrDefaultRouteOnMeshPrefix:
					shell_write(BorderRouter_BrDefaultRouteOnMeshPrefix);
					break;
				case THR_GetAttrConfirm_AttributeId_BorderRouter_BrExternalIfPrefix:
					shell_write(BorderRouter_BrExternalIfPrefix);
					break;
				case THR_GetAttrConfirm_AttributeId_ActiveTimestamp:
					shell_write(ActiveTimestamp);
					break;
				case THR_GetAttrConfirm_AttributeId_PendingChannel:
					shell_write(PendingChannel);
					break;
				case THR_GetAttrConfirm_AttributeId_PendingChannelMask:
					shell_write(PendingChannelMask);
					break;
				case THR_GetAttrConfirm_AttributeId_PendingXpanId:
					shell_write(PendingXpanId);
					break;
				case THR_GetAttrConfirm_AttributeId_PendingMLprefix:
					shell_write(PendingMLprefix);
					break;
				case THR_GetAttrConfirm_AttributeId_PendingNwkMasterKey:
					shell_write(PendingNwkMasterKey);
					break;
				case THR_GetAttrConfirm_AttributeId_PendingNwkName:
					shell_write(PendingNwkName);
					break;
				case THR_GetAttrConfirm_AttributeId_PendingPanId:
					shell_write(PendingPanId);
					break;
				case THR_GetAttrConfirm_AttributeId_PendingPSK:
					shell_write(PendingPSK);
					break;
				case THR_GetAttrConfirm_AttributeId_PendingSecurityPolicy:
					shell_write(PendingSecurityPolicy);
					break;
				case THR_GetAttrConfirm_AttributeId_PendingNwkKeyRotationInterval:
					shell_write(PendingNwkKeyRotationInterval);
					break;
				case THR_GetAttrConfirm_AttributeId_PendingDelayTimer:
					shell_write(PendingDelayTimer);
					break;
				case THR_GetAttrConfirm_AttributeId_PendingActiveTimestamp:
					shell_write(PendingActiveTimestamp);
					break;
				case THR_GetAttrConfirm_AttributeId_PendingTimestamp:
					shell_write(PendingTimestamp);
					break;
				case THR_GetAttrConfirm_AttributeId_CommissionerId:
					shell_write(CommissionerId);
					break;
				case THR_GetAttrConfirm_AttributeId_JoinerPort:
					shell_write(JoinerPort);
					break;
				case THR_GetAttrConfirm_AttributeId_CommissionerUdpPort:
					shell_write(CommissionerUdpPort);
					break;
				case THR_GetAttrConfirm_AttributeId_DiscoveryReqMacTxOptions:
					shell_write(DiscoveryReqMacTxOptions);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_GetAttrConfirm.AttributeId);
					break;
			}
			shell_write(" -> ");
			switch (container->Data.THR_GetAttrConfirm.Status)
			{
				case THR_GetAttrConfirm_Status_Success:
					shell_write(Success);
					break;
				case THR_GetAttrConfirm_Status_Invalidinstance:
					shell_write(Invalidinstance);
					break;
				case THR_GetAttrConfirm_Status_Invalidparameter:
					shell_write(Invalidparameter);
					break;
				case THR_GetAttrConfirm_Status_Notpermitted:
					shell_write(Notpermitted);
					break;
				case THR_GetAttrConfirm_Status_UnsupportedAttribute:
					shell_write(UnsupportedAttribute);
					break;
				case THR_GetAttrConfirm_Status_EmptyEntry:
					shell_write(EmptyEntry);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_GetAttrConfirm.Status);
					break;
			}
			break;

		case THR_SetAttrConfirm_FSCI_ID:
			shell_write("THR_SetAttrConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_SetAttrConfirm.Status)
			{
				case THR_SetAttrConfirm_Status_Success:
					shell_write(Success);
					break;
				case THR_SetAttrConfirm_Status_Invalidinstance:
					shell_write(Invalidinstance);
					break;
				case THR_SetAttrConfirm_Status_Invalidparameter:
					shell_write(Invalidparameter);
					break;
				case THR_SetAttrConfirm_Status_Notpermitted:
					shell_write(Notpermitted);
					break;
				case THR_SetAttrConfirm_Status_UnsupportedAttribute:
					shell_write(UnsupportedAttribute);
					break;
				case THR_SetAttrConfirm_Status_EmptyEntry:
					shell_write(EmptyEntry);
					break;
				case THR_SetAttrConfirm_Status_InvalidValue:
					shell_write(InvalidValue);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_SetAttrConfirm.Status);
					break;
			}
			break;

		case THR_GetThreadIpAddrConfirm_FSCI_ID:
			shell_write("THR_GetThreadIpAddrConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_GetThreadIpAddrConfirm.Status)
			{
				case THR_GetThreadIpAddrConfirm_Status_Success:
					shell_write(Success);
					break;
				case THR_GetThreadIpAddrConfirm_Status_Failed:
					shell_write(Failed);
					break;
				case THR_GetThreadIpAddrConfirm_Status_InvalidInstance:
					shell_write(InvalidInstance);
					break;
				case THR_GetThreadIpAddrConfirm_Status_Error:
					shell_write(Error);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_GetThreadIpAddrConfirm.Status);
					break;
			}
			break;

		case THR_GetParentConfirm_FSCI_ID:
			shell_write("THR_GetParentConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_GetParentConfirm.Status)
			{
				case THR_GetParentConfirm_Status_Success:
					shell_write(Success);
					break;
				case THR_GetParentConfirm_Status_Failed:
					shell_write(Failed);
					break;
				case THR_GetParentConfirm_Status_InvalidInstance:
					shell_write(InvalidInstance);
					break;
				case THR_GetParentConfirm_Status_Error:
					shell_write(Error);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_GetParentConfirm.Status);
					break;
			}
			break;

		case THR_ChildUpdateToParentConfirm_FSCI_ID:
			shell_write("THR_ChildUpdateToParentConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_ChildUpdateToParentConfirm.Status)
			{
				case THR_ChildUpdateToParentConfirm_Status_OK:
					shell_write(OK);
					break;
				case THR_ChildUpdateToParentConfirm_Status_Invalidinstance:
					shell_write(Invalidinstance);
					break;
				case THR_ChildUpdateToParentConfirm_Status_NotPermitted:
					shell_write(NotPermitted);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_ChildUpdateToParentConfirm.Status);
					break;
			}
			break;

		case THR_LeaderRemoveRouterIdConfirm_FSCI_ID:
			shell_write("THR_LeaderRemoveRouterIdConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_LeaderRemoveRouterIdConfirm.Status)
			{
				case THR_LeaderRemoveRouterIdConfirm_Status_OK:
					shell_write(OK);
					break;
				case THR_LeaderRemoveRouterIdConfirm_Status_Invalidinstance:
					shell_write(Invalidinstance);
					break;
				case THR_LeaderRemoveRouterIdConfirm_Status_Theselectedconfigurationisnotvalid:
					shell_write(Theselectedconfigurationisnotvalid);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_LeaderRemoveRouterIdConfirm.Status);
					break;
			}
			break;

		case THR_GenerateAllKeysConfirm_FSCI_ID:
			shell_write("THR_GenerateAllKeysConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_GenerateAllKeysConfirm.Status)
			{
				case THR_GenerateAllKeysConfirm_Status_OK:
					shell_write(OK);
					break;
				case THR_GenerateAllKeysConfirm_Status_Invalidinstance:
					shell_write(Invalidinstance);
					break;
				case THR_GenerateAllKeysConfirm_Status_Theselectedconfigurationisnotvalid:
					shell_write(Theselectedconfigurationisnotvalid);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_GenerateAllKeysConfirm.Status);
					break;
			}
			break;

		case THR_SwitchKeyKeyConfirm_FSCI_ID:
			shell_write("THR_SwitchKeyKeyConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_SwitchKeyKeyConfirm.Status)
			{
				case THR_SwitchKeyKeyConfirm_Status_OK:
					shell_write(OK);
					break;
				case THR_SwitchKeyKeyConfirm_Status_Invalidinstance:
					shell_write(Invalidinstance);
					break;
				case THR_SwitchKeyKeyConfirm_Status_Theselectedconfigurationisnotvalid:
					shell_write(Theselectedconfigurationisnotvalid);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_SwitchKeyKeyConfirm.Status);
					break;
			}
			break;

		case THR_BrPrefixAddEntryConfirm_FSCI_ID:
			shell_write("THR_BrPrefixAddEntryConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_BrPrefixAddEntryConfirm.Status)
			{
				case THR_BrPrefixAddEntryConfirm_Status_Success:
					shell_write(Success);
					break;
				case THR_BrPrefixAddEntryConfirm_Status_Invalidinstance:
					shell_write(Invalidinstance);
					break;
				case THR_BrPrefixAddEntryConfirm_Status_Notpermitted:
					shell_write(Notpermitted);
					break;
				case THR_BrPrefixAddEntryConfirm_Status_Nomemory:
					shell_write(Nomemory);
					break;
				case THR_BrPrefixAddEntryConfirm_Status_Theselectedconfigurationisnotvalid:
					shell_write(Theselectedconfigurationisnotvalid);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_BrPrefixAddEntryConfirm.Status);
					break;
			}
			break;

		case THR_BrPrefixGetTableConfirm_FSCI_ID:
			shell_write("THR_BrPrefixGetTableConfirm");
			break;

		case THR_BrPrefixRemoveEntryConfirm_FSCI_ID:
			shell_write("THR_BrPrefixRemoveEntryConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_BrPrefixRemoveEntryConfirm.Status)
			{
				case THR_BrPrefixRemoveEntryConfirm_Status_Success:
					shell_write(Success);
					break;
				case THR_BrPrefixRemoveEntryConfirm_Status_Invalidinstance:
					shell_write(Invalidinstance);
					break;
				case THR_BrPrefixRemoveEntryConfirm_Status_InvalidParameter:
					shell_write(InvalidParameter);
					break;
				case THR_BrPrefixRemoveEntryConfirm_Status_Nomemory:
					shell_write(Nomemory);
					break;
				case THR_BrPrefixRemoveEntryConfirm_Status_Theselectedconfigurationisnotvalid:
					shell_write(Theselectedconfigurationisnotvalid);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_BrPrefixRemoveEntryConfirm.Status);
					break;
			}
			break;

		case THR_BrServiceRemoveEntryConfirm_FSCI_ID:
			shell_write("THR_BrServiceRemoveEntryConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_BrServiceRemoveEntryConfirm.Status)
			{
				case THR_BrServiceRemoveEntryConfirm_Status_Success:
					shell_write(Success);
					break;
				case THR_BrServiceRemoveEntryConfirm_Status_Invalidinstance:
					shell_write(Invalidinstance);
					break;
				case THR_BrServiceRemoveEntryConfirm_Status_InvalidParameter:
					shell_write(InvalidParameter);
					break;
				case THR_BrServiceRemoveEntryConfirm_Status_Nomemory:
					shell_write(Nomemory);
					break;
				case THR_BrServiceRemoveEntryConfirm_Status_Theselectedconfigurationisnotvalid:
					shell_write(Theselectedconfigurationisnotvalid);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_BrServiceRemoveEntryConfirm.Status);
					break;
			}
			break;

		case THR_BrServiceAddEntryConfirm_FSCI_ID:
			shell_write("THR_BrServiceAddEntryConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_BrServiceAddEntryConfirm.Status)
			{
				case THR_BrServiceAddEntryConfirm_Status_Success:
					shell_write(Success);
					break;
				case THR_BrServiceAddEntryConfirm_Status_Invalidinstance:
					shell_write(Invalidinstance);
					break;
				case THR_BrServiceAddEntryConfirm_Status_InvalidParameter:
					shell_write(InvalidParameter);
					break;
				case THR_BrServiceAddEntryConfirm_Status_NotPermitted:
					shell_write(NotPermitted);
					break;
				case THR_BrServiceAddEntryConfirm_Status_Nomemory:
					shell_write(Nomemory);
					break;
				case THR_BrServiceAddEntryConfirm_Status_Theselectedconfigurationisnotvalid:
					shell_write(Theselectedconfigurationisnotvalid);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_BrServiceAddEntryConfirm.Status);
					break;
			}
			break;

		case THR_BrPrefixSyncConfirm_FSCI_ID:
			shell_write("THR_BrPrefixSyncConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_BrPrefixSyncConfirm.Status)
			{
				case THR_BrPrefixSyncConfirm_Status_Success:
					shell_write(Success);
					break;
				case THR_BrPrefixSyncConfirm_Status_Invalidinstance:
					shell_write(Invalidinstance);
					break;
				case THR_BrPrefixSyncConfirm_Status_InvalidParameter:
					shell_write(InvalidParameter);
					break;
				case THR_BrPrefixSyncConfirm_Status_Notpermitted:
					shell_write(Notpermitted);
					break;
				case THR_BrPrefixSyncConfirm_Status_Nomemory:
					shell_write(Nomemory);
					break;
				case THR_BrPrefixSyncConfirm_Status_Theselectedconfigurationisnotvalid:
					shell_write(Theselectedconfigurationisnotvalid);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_BrPrefixSyncConfirm.Status);
					break;
			}
			break;

		case THR_BrPrefixRemoveAllConfirm_FSCI_ID:
			shell_write("THR_BrPrefixRemoveAllConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_BrPrefixRemoveAllConfirm.Status)
			{
				case THR_BrPrefixRemoveAllConfirm_Status_Success:
					shell_write(Success);
					break;
				case THR_BrPrefixRemoveAllConfirm_Status_Invalidinstance:
					shell_write(Invalidinstance);
					break;
				case THR_BrPrefixRemoveAllConfirm_Status_InvalidParameter:
					shell_write(InvalidParameter);
					break;
				case THR_BrPrefixRemoveAllConfirm_Status_Notpermitted:
					shell_write(Notpermitted);
					break;
				case THR_BrPrefixRemoveAllConfirm_Status_Nomemory:
					shell_write(Nomemory);
					break;
				case THR_BrPrefixRemoveAllConfirm_Status_Theselectedconfigurationisnotvalid:
					shell_write(Theselectedconfigurationisnotvalid);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_BrPrefixRemoveAllConfirm.Status);
					break;
			}
			break;

		case NWKU_EidToRlocMapResponse_FSCI_ID:
			shell_write("NWKU_EidToRlocMapResponse");
			break;

		case THR_IdentifyConfirm_FSCI_ID:
			shell_write("THR_IdentifyConfirm");
			shell_write(" -> ");
			switch (container->Data.THR_IdentifyConfirm.Status)
			{
				case THR_IdentifyConfirm_Status_Success:
					shell_write(Success);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.THR_IdentifyConfirm.Status);
					break;
			}
			break;

#endif  /* THREADUTILS_ENABLE */

#if IPUTILS_ENABLE
		case NWKU_IfconfigBindConfirm_FSCI_ID:
			shell_write("NWKU_IfconfigBindConfirm");
			shell_write(" -> ");
			switch (container->Data.NWKU_IfconfigBindConfirm.Status)
			{
				case NWKU_IfconfigBindConfirm_Status_OK:
					shell_write(OK);
					break;
				case NWKU_IfconfigBindConfirm_Status_Addressesmaximumlimitreached:
					shell_write(Addressesmaximumlimitreached);
					break;
				case NWKU_IfconfigBindConfirm_Status_ERROR:
					shell_write(ERROR);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.NWKU_IfconfigBindConfirm.Status);
					break;
			}
			break;

		case NWKU_IfconfigAllResponse_FSCI_ID:
			shell_write("NWKU_IfconfigAllResponse");
			break;

		case NWKU_PingConfirm_FSCI_ID:
			shell_write("NWKU_PingConfirm");
			shell_write(" -> ");
			switch (container->Data.NWKU_PingConfirm.Status)
			{
				case NWKU_PingConfirm_Status_OK:
					shell_write(OK);
					break;
				case NWKU_PingConfirm_Status_RequestTimeout:
					shell_write(RequestTimeout);
					break;
				case NWKU_PingConfirm_Status_Wrongdestinationaddress:
					shell_write(Wrongdestinationaddress);
					break;
				case NWKU_PingConfirm_Status_ERROR:
					shell_write(ERROR);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.NWKU_PingConfirm.Status);
					break;
			}
			break;

		case NWKU_EchoUDPConfirm_FSCI_ID:
			shell_write("NWKU_EchoUDPConfirm");
			shell_write(" -> ");
			switch (container->Data.NWKU_EchoUDPConfirm.Status)
			{
				case NWKU_EchoUDPConfirm_Status_OK:
					shell_write(OK);
					break;
				case NWKU_EchoUDPConfirm_Status_RequestTimeout:
					shell_write(RequestTimeout);
					break;
				case NWKU_EchoUDPConfirm_Status_Wrongdestinationaddress:
					shell_write(Wrongdestinationaddress);
					break;
				case NWKU_EchoUDPConfirm_Status_ERROR:
					shell_write(ERROR);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.NWKU_EchoUDPConfirm.Status);
					break;
			}
			break;

		case NWKU_CoapMsgReceivedIndication_FSCI_ID:
			shell_write("NWKU_CoapMsgReceivedIndication");
			shell_write(" -> ");
			switch (container->Data.NWKU_CoapMsgReceivedIndication.Status)
			{
				case NWKU_CoapMsgReceivedIndication_Status_Success:
					shell_write(Success);
					break;
				case NWKU_CoapMsgReceivedIndication_Status_Failed:
					shell_write(Failed);
					break;
				case NWKU_CoapMsgReceivedIndication_Status_Duplicate:
					shell_write(Duplicate);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.NWKU_CoapMsgReceivedIndication.Status);
					break;
			}
			shell_write(" -> ");
			switch (container->Data.NWKU_CoapMsgReceivedIndication.RequestType)
			{
				case NWKU_CoapMsgReceivedIndication_RequestType_CON:
					shell_write(CON);
					break;
				case NWKU_CoapMsgReceivedIndication_RequestType_NON:
					shell_write(NON);
					break;
				case NWKU_CoapMsgReceivedIndication_RequestType_ACK:
					shell_write(ACK);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.NWKU_CoapMsgReceivedIndication.RequestType);
					break;
			}
			shell_write(" -> ");
			switch (container->Data.NWKU_CoapMsgReceivedIndication.MessageType)
			{
				case NWKU_CoapMsgReceivedIndication_MessageType_GET:
					shell_write(GET);
					break;
				case NWKU_CoapMsgReceivedIndication_MessageType_POST:
					shell_write(POST);
					break;
				case NWKU_CoapMsgReceivedIndication_MessageType_PUT:
					shell_write(PUT);
					break;
				case NWKU_CoapMsgReceivedIndication_MessageType_DELETE:
					shell_write(DELETE);
					break;
				case NWKU_CoapMsgReceivedIndication_MessageType_Success:
					shell_write(Success);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.NWKU_CoapMsgReceivedIndication.MessageType);
					break;
			}
			break;

		case NWKU_DnsResponseReceivedIndication_FSCI_ID:
			shell_write("NWKU_DnsResponseReceivedIndication");
			break;

		case NWKU_CoapRegisterConfirm_FSCI_ID:
			shell_write("NWKU_CoapRegisterConfirm");
			shell_write(" -> ");
			switch (container->Data.NWKU_CoapRegisterConfirm.Status)
			{
				case NWKU_CoapRegisterConfirm_Status_Success:
					shell_write(Success);
					break;
				case NWKU_CoapRegisterConfirm_Status_Failed:
					shell_write(Failed);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.NWKU_CoapRegisterConfirm.Status);
					break;
			}
			break;

		case NWKU_CoapCreateInstanceConfirm_FSCI_ID:
			shell_write("NWKU_CoapCreateInstanceConfirm");
			shell_write(" -> ");
			switch (container->Data.NWKU_CoapCreateInstanceConfirm.Status)
			{
				case NWKU_CoapCreateInstanceConfirm_Status_Success:
					shell_write(Success);
					break;
				case NWKU_CoapCreateInstanceConfirm_Status_Failed:
					shell_write(Failed);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.NWKU_CoapCreateInstanceConfirm.Status);
					break;
			}
			break;

		case NWKU_CoapSendConfirm_FSCI_ID:
			shell_write("NWKU_CoapSendConfirm");
			shell_write(" -> ");
			switch (container->Data.NWKU_CoapSendConfirm.Status)
			{
				case NWKU_CoapSendConfirm_Status_Success:
					shell_write(Success);
					break;
				case NWKU_CoapSendConfirm_Status_Failed:
					shell_write(Failed);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.NWKU_CoapSendConfirm.Status);
					break;
			}
			break;

		case NWKU_DnsSendRequestConfirm_FSCI_ID:
			shell_write("NWKU_DnsSendRequestConfirm");
			shell_write(" -> ");
			switch (container->Data.NWKU_DnsSendRequestConfirm.Status)
			{
				case NWKU_DnsSendRequestConfirm_Status_Success:
					shell_write(Success);
					break;
				case NWKU_DnsSendRequestConfirm_Status_Failed:
					shell_write(Failed);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.NWKU_DnsSendRequestConfirm.Status);
					break;
			}
			break;

		case NWKU_McastGroupManageConfirm_FSCI_ID:
			shell_write("NWKU_McastGroupManageConfirm");
			shell_write(" -> ");
			switch (container->Data.NWKU_McastGroupManageConfirm.Status)
			{
				case NWKU_McastGroupManageConfirm_Status_Success:
					shell_write(Success);
					break;
				case NWKU_McastGroupManageConfirm_Status_Failed:
					shell_write(Failed);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.NWKU_McastGroupManageConfirm.Status);
					break;
			}
			break;

		case NWKU_McastGroupShowResponse_FSCI_ID:
			shell_write("NWKU_McastGroupShowResponse");
			break;

		case NWKU_RoutesManageConfirm_FSCI_ID:
			shell_write("NWKU_RoutesManageConfirm");
			shell_write(" -> ");
			switch (container->Data.NWKU_RoutesManageConfirm.Status)
			{
				case NWKU_RoutesManageConfirm_Status_Success:
					shell_write(Success);
					break;
				case NWKU_RoutesManageConfirm_Status_Failed:
					shell_write(Failed);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.NWKU_RoutesManageConfirm.Status);
					break;
			}
			break;

		case NWKU_RoutesShowResponse_FSCI_ID:
			shell_write("NWKU_RoutesShowResponse");
			break;

#endif  /* IPUTILS_ENABLE */

#if MESHCOP_ENABLE
		case MESHCOP_StartCommissionerConfirm_FSCI_ID:
			shell_write("MESHCOP_StartCommissionerConfirm");
			shell_write(" -> ");
			switch (container->Data.MESHCOP_StartCommissionerConfirm.Status)
			{
				case MESHCOP_StartCommissionerConfirm_Status_Success:
					shell_write(Success);
					break;
				case MESHCOP_StartCommissionerConfirm_Status_Failed:
					shell_write(Failed);
					break;
				case MESHCOP_StartCommissionerConfirm_Status_Error:
					shell_write(Error);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.MESHCOP_StartCommissionerConfirm.Status);
					break;
			}
			break;

		case MESHCOP_StartNativeCommissionerConfirm_FSCI_ID:
			shell_write("MESHCOP_StartNativeCommissionerConfirm");
			shell_write(" -> ");
			switch (container->Data.MESHCOP_StartNativeCommissionerConfirm.Status)
			{
				case MESHCOP_StartNativeCommissionerConfirm_Status_Success:
					shell_write(Success);
					break;
				case MESHCOP_StartNativeCommissionerConfirm_Status_Failed:
					shell_write(Failed);
					break;
				case MESHCOP_StartNativeCommissionerConfirm_Status_Error:
					shell_write(Error);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.MESHCOP_StartNativeCommissionerConfirm.Status);
					break;
			}
			break;

		case MESHCOP_StopCommissionerConfirm_FSCI_ID:
			shell_write("MESHCOP_StopCommissionerConfirm");
			shell_write(" -> ");
			switch (container->Data.MESHCOP_StopCommissionerConfirm.Status)
			{
				case MESHCOP_StopCommissionerConfirm_Status_Success:
					shell_write(Success);
					break;
				case MESHCOP_StopCommissionerConfirm_Status_Failed:
					shell_write(Failed);
					break;
				case MESHCOP_StopCommissionerConfirm_Status_Error:
					shell_write(Error);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.MESHCOP_StopCommissionerConfirm.Status);
					break;
			}
			break;

		case MESHCOP_AddExpectedJoinerConfirm_FSCI_ID:
			shell_write("MESHCOP_AddExpectedJoinerConfirm");
			shell_write(" -> ");
			switch (container->Data.MESHCOP_AddExpectedJoinerConfirm.Status)
			{
				case MESHCOP_AddExpectedJoinerConfirm_Status_Success:
					shell_write(Success);
					break;
				case MESHCOP_AddExpectedJoinerConfirm_Status_Failed:
					shell_write(Failed);
					break;
				case MESHCOP_AddExpectedJoinerConfirm_Status_Error:
					shell_write(Error);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.MESHCOP_AddExpectedJoinerConfirm.Status);
					break;
			}
			break;

		case MESHCOP_GetExpectedJoinerConfirm_FSCI_ID:
			shell_write("MESHCOP_GetExpectedJoinerConfirm");
			shell_write(" -> ");
			switch (container->Data.MESHCOP_GetExpectedJoinerConfirm.Status)
			{
				case MESHCOP_GetExpectedJoinerConfirm_Status_Success:
					shell_write(Success);
					break;
				case MESHCOP_GetExpectedJoinerConfirm_Status_Failed:
					shell_write(Failed);
					break;
				case MESHCOP_GetExpectedJoinerConfirm_Status_Error:
					shell_write(Error);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.MESHCOP_GetExpectedJoinerConfirm.Status);
					break;
			}
			shell_write(" -> ");
			switch (container->Data.MESHCOP_GetExpectedJoinerConfirm.Selected)
			{
				case MESHCOP_GetExpectedJoinerConfirm_Selected_FALSE:
					shell_write(FALSE_STR);
					break;
				case MESHCOP_GetExpectedJoinerConfirm_Selected_TRUE:
					shell_write(TRUE_STR);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.MESHCOP_GetExpectedJoinerConfirm.Selected);
					break;
			}
			break;

		case MESHCOP_RemoveExpectedJoinerConfirm_FSCI_ID:
			shell_write("MESHCOP_RemoveExpectedJoinerConfirm");
			shell_write(" -> ");
			switch (container->Data.MESHCOP_RemoveExpectedJoinerConfirm.Status)
			{
				case MESHCOP_RemoveExpectedJoinerConfirm_Status_Success:
					shell_write(Success);
					break;
				case MESHCOP_RemoveExpectedJoinerConfirm_Status_Failed:
					shell_write(Failed);
					break;
				case MESHCOP_RemoveExpectedJoinerConfirm_Status_Error:
					shell_write(Error);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.MESHCOP_RemoveExpectedJoinerConfirm.Status);
					break;
			}
			break;

		case MESHCOP_RemoveAllExpectedJoinersConfirm_FSCI_ID:
			shell_write("MESHCOP_RemoveAllExpectedJoinersConfirm");
			shell_write(" -> ");
			switch (container->Data.MESHCOP_RemoveAllExpectedJoinersConfirm.Status)
			{
				case MESHCOP_RemoveAllExpectedJoinersConfirm_Status_Success:
					shell_write(Success);
					break;
				case MESHCOP_RemoveAllExpectedJoinersConfirm_Status_Failed:
					shell_write(Failed);
					break;
				case MESHCOP_RemoveAllExpectedJoinersConfirm_Status_Error:
					shell_write(Error);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.MESHCOP_RemoveAllExpectedJoinersConfirm.Status);
					break;
			}
			break;

		case MESHCOP_SyncSteeringDataConfirm_FSCI_ID:
			shell_write("MESHCOP_SyncSteeringDataConfirm");
			shell_write(" -> ");
			switch (container->Data.MESHCOP_SyncSteeringDataConfirm.Status)
			{
				case MESHCOP_SyncSteeringDataConfirm_Status_Success:
					shell_write(Success);
					break;
				case MESHCOP_SyncSteeringDataConfirm_Status_Failed:
					shell_write(Failed);
					break;
				case MESHCOP_SyncSteeringDataConfirm_Status_Error:
					shell_write(Error);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.MESHCOP_SyncSteeringDataConfirm.Status);
					break;
			}
			break;

		case MESHCOP_MgmtSetConfirm_FSCI_ID:
			shell_write("MESHCOP_MgmtSetConfirm");
			shell_write(" -> ");
			switch (container->Data.MESHCOP_MgmtSetConfirm.Status)
			{
				case MESHCOP_MgmtSetConfirm_Status_Success:
					shell_write(Success);
					break;
				case MESHCOP_MgmtSetConfirm_Status_Failed:
					shell_write(Failed);
					break;
				case MESHCOP_MgmtSetConfirm_Status_Error:
					shell_write(Error);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.MESHCOP_MgmtSetConfirm.Status);
					break;
			}
			break;

		case MESHCOP_MgmtGetConfirm_FSCI_ID:
			shell_write("MESHCOP_MgmtGetConfirm");
			shell_write(" -> ");
			switch (container->Data.MESHCOP_MgmtGetConfirm.Status)
			{
				case MESHCOP_MgmtGetConfirm_Status_Success:
					shell_write(Success);
					break;
				case MESHCOP_MgmtGetConfirm_Status_Failed:
					shell_write(Failed);
					break;
				case MESHCOP_MgmtGetConfirm_Status_Error:
					shell_write(Error);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.MESHCOP_MgmtGetConfirm.Status);
					break;
			}
			shell_write(" -> ");
			switch (container->Data.MESHCOP_MgmtGetConfirm.Type)
			{
				case MESHCOP_MgmtGetConfirm_Type_Channel:
					shell_write(Channel);
					break;
				case MESHCOP_MgmtGetConfirm_Type_ChannelMask:
					shell_write(ChannelMask);
					break;
				case MESHCOP_MgmtGetConfirm_Type_PanId:
					shell_write(PanId);
					break;
				case MESHCOP_MgmtGetConfirm_Type_XpanId:
					shell_write(XpanId);
					break;
				case MESHCOP_MgmtGetConfirm_Type_NetworkName:
					shell_write(NetworkName);
					break;
				case MESHCOP_MgmtGetConfirm_Type_PSKc:
					shell_write(PSKc);
					break;
				case MESHCOP_MgmtGetConfirm_Type_MasterKey:
					shell_write(MasterKey);
					break;
				case MESHCOP_MgmtGetConfirm_Type_KeySequence:
					shell_write(KeySequence);
					break;
				case MESHCOP_MgmtGetConfirm_Type_MeshLocalUla:
					shell_write(MeshLocalUla);
					break;
				case MESHCOP_MgmtGetConfirm_Type_SteeringData:
					shell_write(SteeringData);
					break;
				case MESHCOP_MgmtGetConfirm_Type_BorderRouterLocator:
					shell_write(BorderRouterLocator);
					break;
				case MESHCOP_MgmtGetConfirm_Type_CommissionerID:
					shell_write(CommissionerID);
					break;
				case MESHCOP_MgmtGetConfirm_Type_CommissionerSessionID:
					shell_write(CommissionerSessionID);
					break;
				case MESHCOP_MgmtGetConfirm_Type_SecurityPolicy:
					shell_write(SecurityPolicy);
					break;
				case MESHCOP_MgmtGetConfirm_Type_DelayTimer:
					shell_write(DelayTimer);
					break;
				case MESHCOP_MgmtGetConfirm_Type_ActiveTimestamp:
					shell_write(ActiveTimestamp);
					break;
				case MESHCOP_MgmtGetConfirm_Type_PendingTimestamp:
					shell_write(PendingTimestamp);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.MESHCOP_MgmtGetConfirm.Type);
					break;
			}
			break;

		case MESHCOP_SetCommissionerCredentialConfirm_FSCI_ID:
			shell_write("MESHCOP_SetCommissionerCredentialConfirm");
			shell_write(" -> ");
			switch (container->Data.MESHCOP_SetCommissionerCredentialConfirm.Status)
			{
				case MESHCOP_SetCommissionerCredentialConfirm_Status_Success:
					shell_write(Success);
					break;
				case MESHCOP_SetCommissionerCredentialConfirm_Status_Failed:
					shell_write(Failed);
					break;
				case MESHCOP_SetCommissionerCredentialConfirm_Status_Error:
					shell_write(Error);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.MESHCOP_SetCommissionerCredentialConfirm.Status);
					break;
			}
			break;

		case MESHCOP_MgmNwkFormConfirm_FSCI_ID:
			shell_write("MESHCOP_MgmNwkFormConfirm");
			shell_write(" -> ");
			switch (container->Data.MESHCOP_MgmNwkFormConfirm.Status)
			{
				case MESHCOP_MgmNwkFormConfirm_Status_Success:
					shell_write(Success);
					break;
				case MESHCOP_MgmNwkFormConfirm_Status_Notpermitted:
					shell_write(Notpermitted);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.MESHCOP_MgmNwkFormConfirm.Status);
					break;
			}
			break;

		case MESHCOP_MgmtCommissionerGetConfirm_FSCI_ID:
			shell_write("MESHCOP_MgmtCommissionerGetConfirm");
			shell_write(" -> ");
			switch (container->Data.MESHCOP_MgmtCommissionerGetConfirm.Status)
			{
				case MESHCOP_MgmtCommissionerGetConfirm_Status_Success:
					shell_write(Success);
					break;
				case MESHCOP_MgmtCommissionerGetConfirm_Status_Failed:
					shell_write(Failed);
					break;
				case MESHCOP_MgmtCommissionerGetConfirm_Status_Error:
					shell_write(Error);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.MESHCOP_MgmtCommissionerGetConfirm.Status);
					break;
			}
			break;

		case MESHCOP_MgmtActiveGetConfirm_FSCI_ID:
			shell_write("MESHCOP_MgmtActiveGetConfirm");
			shell_write(" -> ");
			switch (container->Data.MESHCOP_MgmtActiveGetConfirm.Status)
			{
				case MESHCOP_MgmtActiveGetConfirm_Status_Success:
					shell_write(Success);
					break;
				case MESHCOP_MgmtActiveGetConfirm_Status_Failed:
					shell_write(Failed);
					break;
				case MESHCOP_MgmtActiveGetConfirm_Status_Error:
					shell_write(Error);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.MESHCOP_MgmtActiveGetConfirm.Status);
					break;
			}
			break;

		case MESHCOP_MgmtPendingGetConfirm_FSCI_ID:
			shell_write("MESHCOP_MgmtPendingGetConfirm");
			shell_write(" -> ");
			switch (container->Data.MESHCOP_MgmtPendingGetConfirm.Status)
			{
				case MESHCOP_MgmtPendingGetConfirm_Status_Success:
					shell_write(Success);
					break;
				case MESHCOP_MgmtPendingGetConfirm_Status_Failed:
					shell_write(Failed);
					break;
				case MESHCOP_MgmtPendingGetConfirm_Status_Error:
					shell_write(Error);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.MESHCOP_MgmtPendingGetConfirm.Status);
					break;
			}
			break;

		case MESHCOP_MgmtCommissionerSetConfirm_FSCI_ID:
			shell_write("MESHCOP_MgmtCommissionerSetConfirm");
			shell_write(" -> ");
			switch (container->Data.MESHCOP_MgmtCommissionerSetConfirm.Status)
			{
				case MESHCOP_MgmtCommissionerSetConfirm_Status_Success:
					shell_write(Success);
					break;
				case MESHCOP_MgmtCommissionerSetConfirm_Status_Failed:
					shell_write(Failed);
					break;
				case MESHCOP_MgmtCommissionerSetConfirm_Status_Error:
					shell_write(Error);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.MESHCOP_MgmtCommissionerSetConfirm.Status);
					break;
			}
			break;

		case MESHCOP_MgmtActiveSetConfirm_FSCI_ID:
			shell_write("MESHCOP_MgmtActiveSetConfirm");
			shell_write(" -> ");
			switch (container->Data.MESHCOP_MgmtActiveSetConfirm.Status)
			{
				case MESHCOP_MgmtActiveSetConfirm_Status_Success:
					shell_write(Success);
					break;
				case MESHCOP_MgmtActiveSetConfirm_Status_Failed:
					shell_write(Failed);
					break;
				case MESHCOP_MgmtActiveSetConfirm_Status_Error:
					shell_write(Error);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.MESHCOP_MgmtActiveSetConfirm.Status);
					break;
			}
			break;

		case MESHCOP_MgmtPendingSetConfirm_FSCI_ID:
			shell_write("MESHCOP_MgmtPendingSetConfirm");
			shell_write(" -> ");
			switch (container->Data.MESHCOP_MgmtPendingSetConfirm.Status)
			{
				case MESHCOP_MgmtPendingSetConfirm_Status_Success:
					shell_write(Success);
					break;
				case MESHCOP_MgmtPendingSetConfirm_Status_Failed:
					shell_write(Failed);
					break;
				case MESHCOP_MgmtPendingSetConfirm_Status_Error:
					shell_write(Error);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.MESHCOP_MgmtPendingSetConfirm.Status);
					break;
			}
			break;

		case MESHCOP_MgmtSendPanIdQueryConfirm_FSCI_ID:
			shell_write("MESHCOP_MgmtSendPanIdQueryConfirm");
			shell_write(" -> ");
			switch (container->Data.MESHCOP_MgmtSendPanIdQueryConfirm.Status)
			{
				case MESHCOP_MgmtSendPanIdQueryConfirm_Status_Success:
					shell_write(Success);
					break;
				case MESHCOP_MgmtSendPanIdQueryConfirm_Status_Failed:
					shell_write(Failed);
					break;
				case MESHCOP_MgmtSendPanIdQueryConfirm_Status_Error:
					shell_write(Error);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.MESHCOP_MgmtSendPanIdQueryConfirm.Status);
					break;
			}
			break;

		case MESHCOP_MgmtPanIdConflictConfirm_FSCI_ID:
			shell_write("MESHCOP_MgmtPanIdConflictConfirm");
			shell_write(" -> ");
			switch (container->Data.MESHCOP_MgmtPanIdConflictConfirm.Status)
			{
				case MESHCOP_MgmtPanIdConflictConfirm_Status_Success:
					shell_write(Success);
					break;
				case MESHCOP_MgmtPanIdConflictConfirm_Status_Failed:
					shell_write(Failed);
					break;
				case MESHCOP_MgmtPanIdConflictConfirm_Status_Error:
					shell_write(Error);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.MESHCOP_MgmtPanIdConflictConfirm.Status);
					break;
			}
			break;

		case MESHCOP_MgmtSendEdScanConfirm_FSCI_ID:
			shell_write("MESHCOP_MgmtSendEdScanConfirm");
			shell_write(" -> ");
			switch (container->Data.MESHCOP_MgmtSendEdScanConfirm.Status)
			{
				case MESHCOP_MgmtSendEdScanConfirm_Status_Success:
					shell_write(Success);
					break;
				case MESHCOP_MgmtSendEdScanConfirm_Status_Failed:
					shell_write(Failed);
					break;
				case MESHCOP_MgmtSendEdScanConfirm_Status_Error:
					shell_write(Error);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.MESHCOP_MgmtSendEdScanConfirm.Status);
					break;
			}
			break;

		case MESHCOP_MgmtEdReportConfirm_FSCI_ID:
			shell_write("MESHCOP_MgmtEdReportConfirm");
			shell_write(" -> ");
			switch (container->Data.MESHCOP_MgmtEdReportConfirm.Status)
			{
				case MESHCOP_MgmtEdReportConfirm_Status_Success:
					shell_write(Success);
					break;
				case MESHCOP_MgmtEdReportConfirm_Status_Failed:
					shell_write(Failed);
					break;
				case MESHCOP_MgmtEdReportConfirm_Status_Error:
					shell_write(Error);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.MESHCOP_MgmtEdReportConfirm.Status);
					break;
			}
			break;

		case MESHCOP_MgmtSendAnnounceBeginConfirm_FSCI_ID:
			shell_write("MESHCOP_MgmtSendAnnounceBeginConfirm");
			shell_write(" -> ");
			switch (container->Data.MESHCOP_MgmtSendAnnounceBeginConfirm.Status)
			{
				case MESHCOP_MgmtSendAnnounceBeginConfirm_Status_Success:
					shell_write(Success);
					break;
				case MESHCOP_MgmtSendAnnounceBeginConfirm_Status_Failed:
					shell_write(Failed);
					break;
				case MESHCOP_MgmtSendAnnounceBeginConfirm_Status_Error:
					shell_write(Error);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.MESHCOP_MgmtSendAnnounceBeginConfirm.Status);
					break;
			}
			break;

#endif  /* MESHCOP_ENABLE */

#if DTLS_ENABLE
		case DTLSOpenConfirm_FSCI_ID:
			shell_write("DTLSOpenConfirm");
			break;

		case DTLSCloseConfirm_FSCI_ID:
			shell_write("DTLSCloseConfirm");
			shell_write(" -> ");
			switch (container->Data.DTLSCloseConfirm.Status)
			{
				case DTLSCloseConfirm_Status_OK:
					shell_write(OK);
					break;
				case DTLSCloseConfirm_Status_ERROR:
					shell_write(ERROR);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.DTLSCloseConfirm.Status);
					break;
			}
			break;

		case DTLSClosePeerConfirm_FSCI_ID:
			shell_write("DTLSClosePeerConfirm");
			shell_write(" -> ");
			switch (container->Data.DTLSClosePeerConfirm.Status)
			{
				case DTLSClosePeerConfirm_Status_OK:
					shell_write(OK);
					break;
				case DTLSClosePeerConfirm_Status_ERROR:
					shell_write(ERROR);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.DTLSClosePeerConfirm.Status);
					break;
			}
			break;

		case DTLSConnectConfirm_FSCI_ID:
			shell_write("DTLSConnectConfirm");
			shell_write(" -> ");
			switch (container->Data.DTLSConnectConfirm.Status)
			{
				case DTLSConnectConfirm_Status_OK:
					shell_write(OK);
					break;
				case DTLSConnectConfirm_Status_ERROR:
					shell_write(ERROR);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.DTLSConnectConfirm.Status);
					break;
			}
			break;

		case DTLSClientConnectedConfirm_FSCI_ID:
			shell_write("DTLSClientConnectedConfirm");
			shell_write(" -> ");
			switch (container->Data.DTLSClientConnectedConfirm.Status)
			{
				case DTLSClientConnectedConfirm_Status_OK:
					shell_write(OK);
					break;
				case DTLSClientConnectedConfirm_Status_ERROR:
					shell_write(ERROR);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.DTLSClientConnectedConfirm.Status);
					break;
			}
			break;

		case DTLSSendConfirm_FSCI_ID:
			shell_write("DTLSSendConfirm");
			shell_write(" -> ");
			switch (container->Data.DTLSSendConfirm.Status)
			{
				case DTLSSendConfirm_Status_OK:
					shell_write(OK);
					break;
				case DTLSSendConfirm_Status_ERROR:
					shell_write(ERROR);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.DTLSSendConfirm.Status);
					break;
			}
			break;

		case DTLSReceiveConfirm_FSCI_ID:
			shell_write("DTLSReceiveConfirm");
			break;

#endif  /* DTLS_ENABLE */

#if OTHERUTILS_ENABLE
		case FSCIGetUniqueIdConfirm_FSCI_ID:
			shell_write("FSCIGetUniqueIdConfirm");
			break;

		case FSCIGetMcuIdConfirm_FSCI_ID:
			shell_write("FSCIGetMcuIdConfirm");
			break;

		case FSCIGetSwVersionsConfirm_FSCI_ID:
			shell_write("FSCIGetSwVersionsConfirm");
			break;

		case Sniffer_MacSetPIBAttributeConfirm_FSCI_ID:
			shell_write("Sniffer_MacSetPIBAttributeConfirm");
			shell_write(" -> ");
			switch (container->Data.Sniffer_MacSetPIBAttributeConfirm.Status)
			{
				case Sniffer_MacSetPIBAttributeConfirm_Status_gSuccess_c:
					shell_write(gSuccess_c);
					break;
				case Sniffer_MacSetPIBAttributeConfirm_Status_UNSUPORTED_ATTRIBUTE:
					shell_write(UNSUPORTED_ATTRIBUTE);
					break;
				case Sniffer_MacSetPIBAttributeConfirm_Status_gInvalidParameter_c:
					shell_write(gInvalidParameter_c);
					break;
				case Sniffer_MacSetPIBAttributeConfirm_Status_gReadOnly_c:
					shell_write(gReadOnly_c);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.Sniffer_MacSetPIBAttributeConfirm.Status);
					break;
			}
			shell_write(" -> ");
			switch (container->Data.Sniffer_MacSetPIBAttributeConfirm.PIBAttribute)
			{
				case Sniffer_MacSetPIBAttributeConfirm_PIBAttribute_macLogicalChannel:
					shell_write(macLogicalChannel);
					break;
				case Sniffer_MacSetPIBAttributeConfirm_PIBAttribute_macMAC_PromiscuousRxIndicationMode:
					shell_write(macMAC_PromiscuousRxIndicationMode);
					break;
				case Sniffer_MacSetPIBAttributeConfirm_PIBAttribute_macRxOnWhenIdle:
					shell_write(macRxOnWhenIdle);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.Sniffer_MacSetPIBAttributeConfirm.PIBAttribute);
					break;
			}
			break;

		case MAC_PromiscuousRxIndication_FSCI_ID:
			shell_write("MAC_PromiscuousRxIndication");
			break;

		case SerialTun_LinkIndicationConfirm_FSCI_ID:
			shell_write("SerialTun_LinkIndicationConfirm");
			shell_write(" -> ");
			switch (container->Data.SerialTun_LinkIndicationConfirm.Status)
			{
				case SerialTun_LinkIndicationConfirm_Status_OK:
					shell_write(OK);
					break;
				case SerialTun_LinkIndicationConfirm_Status_ERROR:
					shell_write(ERROR);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.SerialTun_LinkIndicationConfirm.Status);
					break;
			}
			break;

		case SerialTun_IPPacketReceivedConfirm_FSCI_ID:
			shell_write("SerialTun_IPPacketReceivedConfirm");
			break;

		case AspSetPowerLevelConfirm_FSCI_ID:
			shell_write("AspSetPowerLevelConfirm");
			shell_write(" -> ");
			switch (container->Data.AspSetPowerLevelConfirm.Status)
			{
				case AspSetPowerLevelConfirm_Status_SUCCESS:
					shell_write(SUCCESS);
					break;
				case AspSetPowerLevelConfirm_Status_INVALID_PARAMETER:
					shell_write(INVALID_PARAMETER);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.AspSetPowerLevelConfirm.Status);
					break;
			}
			break;

		case AspGetPowerLevelConfirm_FSCI_ID:
			shell_write("AspGetPowerLevelConfirm");
			break;

		case DBGConfirm_FSCI_ID:
			shell_write("DBGConfirm");
			break;

#endif  /* OTHERUTILS_ENABLE */

	}

	shell_refresh();
}
