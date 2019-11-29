/* Source file generated from ZigBee3_0_6.0.8.xml */
/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "cmd_zigbee3_0_6.0.9.h"

#if ZIGBEESTACKANDNODEMANAGEMENTCOMMANDS_ENABLE
static const char *Success = "Success";
static const char *IncorrectParameters = "IncorrectParameters";
static const char *UnhandledCommand = "UnhandledCommand";
static const char *CommandFailed = "CommandFailed";
static const char *Busy = "Busy";
static const char *StackAlreadyStarted = "StackAlreadyStarted";
static const char *Failed = "Failed";
static const char *Reserved = "Reserved";
static const char *Emergency = "Emergency";
static const char *Alert = "Alert";
static const char *Critical = "Critical";
static const char *Error = "Error";
static const char *Warning = "Warning";
static const char *Notice = "Notice";
static const char *Information = "Information";
static const char *Debug = "Debug";
static const char *NO_PAN = "NO_PAN";
static const char *Address_16b = "Address_16b";
static const char *Address_64b = "Address_64b";
static const char *JoinedExistingNetwork = "JoinedExistingNetwork";
static const char *FormedNewNetwork = "FormedNewNetwork";
static const char *Off = "Off";
static const char *On = "On";
static const char *STARTUP = "STARTUP";
static const char *NFN_START = "NFN_START";
static const char *RUNNING = "RUNNING";
#endif  /* ZIGBEESTACKANDNODEMANAGEMENTCOMMANDS_ENABLE */

#if ENTIREPROFILE_ENABLE
static const char *MoreAttributesToFollow = "MoreAttributesToFollow";
static const char *ThisWasTheLastAttribute = "ThisWasTheLastAttribute";
#endif  /* ENTIREPROFILE_ENABLE */

#if DIAGNOSTICSCLUSTERCOMMANDS_ENABLE
static const char *DelayResponse = "DelayResponse";
#endif  /* DIAGNOSTICSCLUSTERCOMMANDS_ENABLE */

#if TOUCHLINKCOMMANDS_ENABLE
static const char *Failure = "Failure";
#endif  /* TOUCHLINKCOMMANDS_ENABLE */

#if EXPORTINGPERSISTENTDATATOHOST_ENABLE
static const char *NoRecordFound = "NoRecordFound";
static const char *RecordRecovered = "RecordRecovered";
#endif  /* EXPORTINGPERSISTENTDATATOHOST_ENABLE */

#if EXTENDEDUTILITIES_ENABLE
static const char *SUCCESS = "SUCCESS";
static const char *COUNTER_ERROR = "COUNTER_ERROR";
static const char *IMPROPER_KEY_TYPE = "IMPROPER_KEY_TYPE";
static const char *IMPROPER_SECURITY_LEVEL = "IMPROPER_SECURITY_LEVEL";
static const char *UNSUPPORTED_LEGACY = "UNSUPPORTED_LEGACY";
static const char *BEACON_LOSS = "BEACON_LOSS";
static const char *CHANNEL_ACCESS_FAILURE = "CHANNEL_ACCESS_FAILURE";
static const char *DENIED = "DENIED";
static const char *DISABLE_TRX_FAILURE = "DISABLE_TRX_FAILURE";
static const char *FAILED_SECURITY_CHECK = "FAILED_SECURITY_CHECK";
static const char *FRAME_TOO_LONG = "FRAME_TOO_LONG";
static const char *INVALID_GTS = "INVALID_GTS";
static const char *INVALID_HANDLE = "INVALID_HANDLE";
static const char *INVALID_PARAMETER = "INVALID_PARAMETER";
static const char *NO_ACK = "NO_ACK";
static const char *NO_BEACON = "NO_BEACON";
static const char *NO_DATA = "NO_DATA";
static const char *NO_SHORT_ADDRESS = "NO_SHORT_ADDRESS";
static const char *OUT_OF_CAP = "OUT_OF_CAP";
static const char *PAN_ID_CONFLICT = "PAN_ID_CONFLICT";
static const char *REALIGNMENT = "REALIGNMENT";
static const char *TRANSACTION_EXPIRED = "TRANSACTION_EXPIRED";
static const char *TRANSACTION_OVERFLOW = "TRANSACTION_OVERFLOW";
static const char *TX_ACTIVE = "TX_ACTIVE";
static const char *UNAVAILABLE_KEY = "UNAVAILABLE_KEY";
static const char *UNSUPPORTED_ATTRIBUTE = "UNSUPPORTED_ATTRIBUTE";
static const char *SCAN_IN_PROGRESS = "SCAN_IN_PROGRESS";
#endif  /* EXTENDEDUTILITIES_ENABLE */


void SHELL_ZigEventNotify(void *param)
{
	zigEvtContainer_t *container = (zigEvtContainer_t *)param;

	switch (container->id)
	{
#if ZIGBEESTACKANDNODEMANAGEMENTCOMMANDS_ENABLE
		case Status_FSCI_ID:
			shell_write("Status");
			shell_write(" -> ");
			switch (container->Data.Status.Status)
			{
				case Status_Status_Success:
					shell_write(Success);
					break;
				case Status_Status_IncorrectParameters:
					shell_write(IncorrectParameters);
					break;
				case Status_Status_UnhandledCommand:
					shell_write(UnhandledCommand);
					break;
				case Status_Status_CommandFailed:
					shell_write(CommandFailed);
					break;
				case Status_Status_Busy:
					shell_write(Busy);
					break;
				case Status_Status_StackAlreadyStarted:
					shell_write(StackAlreadyStarted);
					break;
				case Status_Status_Failed:
					shell_write(Failed);
					break;
				case Status_Status_Reserved:
					shell_write(Reserved);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.Status.Status);
					break;
			}
			break;

		case LogMessage_FSCI_ID:
			shell_write("LogMessage");
			shell_write(" -> ");
			switch (container->Data.LogMessage.LogLevel)
			{
				case LogMessage_LogLevel_Emergency:
					shell_write(Emergency);
					break;
				case LogMessage_LogLevel_Alert:
					shell_write(Alert);
					break;
				case LogMessage_LogLevel_Critical:
					shell_write(Critical);
					break;
				case LogMessage_LogLevel_Error:
					shell_write(Error);
					break;
				case LogMessage_LogLevel_Warning:
					shell_write(Warning);
					break;
				case LogMessage_LogLevel_Notice:
					shell_write(Notice);
					break;
				case LogMessage_LogLevel_Information:
					shell_write(Information);
					break;
				case LogMessage_LogLevel_Debug:
					shell_write(Debug);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.LogMessage.LogLevel);
					break;
			}
			break;

		case DataIndication_FSCI_ID:
			shell_write("DataIndication");
			shell_write(" -> ");
			switch (container->Data.DataIndication.SourceAddressMode)
			{
				case DataIndication_SourceAddressMode_NO_PAN:
					shell_write(NO_PAN);
					break;
				case DataIndication_SourceAddressMode_Reserved:
					shell_write(Reserved);
					break;
				case DataIndication_SourceAddressMode_Address_16b:
					shell_write(Address_16b);
					break;
				case DataIndication_SourceAddressMode_Address_64b:
					shell_write(Address_64b);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.DataIndication.SourceAddressMode);
					break;
			}
			shell_write(" -> ");
			switch (container->Data.DataIndication.DestAddressingMode)
			{
				case DataIndication_DestAddressingMode_NO_PAN:
					shell_write(NO_PAN);
					break;
				case DataIndication_DestAddressingMode_Reserved:
					shell_write(Reserved);
					break;
				case DataIndication_DestAddressingMode_Address_16b:
					shell_write(Address_16b);
					break;
				case DataIndication_DestAddressingMode_Address_64b:
					shell_write(Address_64b);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.DataIndication.DestAddressingMode);
					break;
			}
			break;

		case NodeClusterList_FSCI_ID:
			shell_write("NodeClusterList");
			break;

		case NodeClusterAttributeList_FSCI_ID:
			shell_write("NodeClusterAttributeList");
			break;

		case NodeCommandIDList_FSCI_ID:
			shell_write("NodeCommandIDList");
			break;

		case VersionList_FSCI_ID:
			shell_write("VersionList");
			break;

		case NetworkJoinedFormed_FSCI_ID:
			shell_write("NetworkJoinedFormed");
			shell_write(" -> ");
			switch (container->Data.NetworkJoinedFormed.Status)
			{
				case NetworkJoinedFormed_Status_JoinedExistingNetwork:
					shell_write(JoinedExistingNetwork);
					break;
				case NetworkJoinedFormed_Status_FormedNewNetwork:
					shell_write(FormedNewNetwork);
					break;
				case NetworkJoinedFormed_Status_Failed:
					shell_write(Failed);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.NetworkJoinedFormed.Status);
					break;
			}
			break;

		case PermitJoinStatusResponse_FSCI_ID:
			shell_write("PermitJoinStatusResponse");
			shell_write(" -> ");
			switch (container->Data.PermitJoinStatusResponse.Status)
			{
				case PermitJoinStatusResponse_Status_Off:
					shell_write(Off);
					break;
				case PermitJoinStatusResponse_Status_On:
					shell_write(On);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.PermitJoinStatusResponse.Status);
					break;
			}
			break;

		case NonFactoryNewRestart_FSCI_ID:
			shell_write("NonFactoryNewRestart");
			shell_write(" -> ");
			switch (container->Data.NonFactoryNewRestart.Status)
			{
				case NonFactoryNewRestart_Status_STARTUP:
					shell_write(STARTUP);
					break;
				case NonFactoryNewRestart_Status_NFN_START:
					shell_write(NFN_START);
					break;
				case NonFactoryNewRestart_Status_RUNNING:
					shell_write(RUNNING);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.NonFactoryNewRestart.Status);
					break;
			}
			break;

		case FactoryNewRestart_FSCI_ID:
			shell_write("FactoryNewRestart");
			shell_write(" -> ");
			switch (container->Data.FactoryNewRestart.Status)
			{
				case FactoryNewRestart_Status_STARTUP:
					shell_write(STARTUP);
					break;
				case FactoryNewRestart_Status_NFN_START:
					shell_write(NFN_START);
					break;
				case FactoryNewRestart_Status_RUNNING:
					shell_write(RUNNING);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.FactoryNewRestart.Status);
					break;
			}
			break;

		case BindResponse_FSCI_ID:
			shell_write("BindResponse");
			break;

		case UnbindResponse_FSCI_ID:
			shell_write("UnbindResponse");
			break;

		case NetworkAddressResponse_FSCI_ID:
			shell_write("NetworkAddressResponse");
			break;

		case IEEEAddressResponse_FSCI_ID:
			shell_write("IEEEAddressResponse");
			break;

		case NodeDescriptorResponse_FSCI_ID:
			shell_write("NodeDescriptorResponse");
			break;

		case SimpleDescriptorResponse_FSCI_ID:
			shell_write("SimpleDescriptorResponse");
			break;

		case PowerDescriptorResponse_FSCI_ID:
			shell_write("PowerDescriptorResponse");
			break;

		case ActiveEndpointResponse_FSCI_ID:
			shell_write("ActiveEndpointResponse");
			break;

		case MatchDescriptorResponse_FSCI_ID:
			shell_write("MatchDescriptorResponse");
			break;

		case DeviceAnnounce_FSCI_ID:
			shell_write("DeviceAnnounce");
			break;

		case UserDescriptorResponse_FSCI_ID:
			shell_write("UserDescriptorResponse");
			break;

		case UserDescriptorNotify_FSCI_ID:
			shell_write("UserDescriptorNotify");
			break;

		case ComplexDescriptorResponse_FSCI_ID:
			shell_write("ComplexDescriptorResponse");
			break;

		case ManagementLeaveResponse_FSCI_ID:
			shell_write("ManagementLeaveResponse");
			break;

		case LeaveIndication_FSCI_ID:
			shell_write("LeaveIndication");
			break;

		case ManagementNetworkUpdateResponse_FSCI_ID:
			shell_write("ManagementNetworkUpdateResponse");
			break;

		case SystemServerDiscoveryResponse_FSCI_ID:
			shell_write("SystemServerDiscoveryResponse");
			break;

#endif  /* ZIGBEESTACKANDNODEMANAGEMENTCOMMANDS_ENABLE */

#if ENTIREPROFILE_ENABLE
		case ManagementLQIResponse_FSCI_ID:
			shell_write("ManagementLQIResponse");
			break;

		case AttributeDiscoveryResponse_FSCI_ID:
			shell_write("AttributeDiscoveryResponse");
			shell_write(" -> ");
			switch (container->Data.AttributeDiscoveryResponse.Complete)
			{
				case AttributeDiscoveryResponse_Complete_MoreAttributesToFollow:
					shell_write(MoreAttributesToFollow);
					break;
				case AttributeDiscoveryResponse_Complete_ThisWasTheLastAttribute:
					shell_write(ThisWasTheLastAttribute);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.AttributeDiscoveryResponse.Complete);
					break;
			}
			break;

		case AuthenticateResponse_FSCI_ID:
			shell_write("AuthenticateResponse");
			break;

		case CodeResponse_FSCI_ID:
			shell_write("CodeResponse");
			break;

		case ConfigureReportingResponse_FSCI_ID:
			shell_write("ConfigureReportingResponse");
			break;

		case ReadIndividualAttributeResponse_FSCI_ID:
			shell_write("ReadIndividualAttributeResponse");
			break;

		case WriteAttributeResponse_FSCI_ID:
			shell_write("WriteAttributeResponse");
			break;

		case ReportIndividualAttributeResponse_FSCI_ID:
			shell_write("ReportIndividualAttributeResponse");
			break;

		case DefaultResponse_FSCI_ID:
			shell_write("DefaultResponse");
			break;

#endif  /* ENTIREPROFILE_ENABLE */

#if GROUPCLUSTERCOMMANDS_ENABLE
		case AddGroupResponse_FSCI_ID:
			shell_write("AddGroupResponse");
			break;

		case ViewGroupResponse_FSCI_ID:
			shell_write("ViewGroupResponse");
			break;

		case GetGroupMembershipResponse_FSCI_ID:
			shell_write("GetGroupMembershipResponse");
			break;

		case RemoveGroupResponse_FSCI_ID:
			shell_write("RemoveGroupResponse");
			break;

#endif  /* GROUPCLUSTERCOMMANDS_ENABLE */

#if DIAGNOSTICSCLUSTERCOMMANDS_ENABLE
		case DiagnosticResponse_FSCI_ID:
			shell_write("DiagnosticResponse");
			shell_write(" -> ");
			switch (container->Data.DiagnosticResponse.Status)
			{
				case DiagnosticResponse_Status_DelayResponse:
					shell_write(DelayResponse);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.DiagnosticResponse.Status);
					break;
			}
			break;

#endif  /* DIAGNOSTICSCLUSTERCOMMANDS_ENABLE */

#if SCENESCLUSTERCOMMANDS_ENABLE
		case ViewSceneResponse_FSCI_ID:
			shell_write("ViewSceneResponse");
			break;

		case ViewEnhancedSceneResponse_FSCI_ID:
			shell_write("ViewEnhancedSceneResponse");
			break;

		case AddSceneResponse_FSCI_ID:
			shell_write("AddSceneResponse");
			break;

		case CopySceneResponse_FSCI_ID:
			shell_write("CopySceneResponse");
			break;

		case RemoveSceneResponse_FSCI_ID:
			shell_write("RemoveSceneResponse");
			break;

		case RemoveAllSceneResponse_FSCI_ID:
			shell_write("RemoveAllSceneResponse");
			break;

		case AddEnhancedSceneResponse_FSCI_ID:
			shell_write("AddEnhancedSceneResponse");
			break;

		case StoreSceneResponse_FSCI_ID:
			shell_write("StoreSceneResponse");
			break;

		case SceneMembershipResponse_FSCI_ID:
			shell_write("SceneMembershipResponse");
			break;

#endif  /* SCENESCLUSTERCOMMANDS_ENABLE */

#if TOUCHLINKCOMMANDS_ENABLE
		case TouchlinkStatus_FSCI_ID:
			shell_write("TouchlinkStatus");
			shell_write(" -> ");
			switch (container->Data.TouchlinkStatus.Status)
			{
				case TouchlinkStatus_Status_Success:
					shell_write(Success);
					break;
				case TouchlinkStatus_Status_Failure:
					shell_write(Failure);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.TouchlinkStatus.Status);
					break;
			}
			break;

#endif  /* TOUCHLINKCOMMANDS_ENABLE */

#if IASCLUSTERCOMMANDS_ENABLE
		case ZoneStatusChangeNotification_FSCI_ID:
			shell_write("ZoneStatusChangeNotification");
			shell_write(" -> ");
			switch (container->Data.ZoneStatusChangeNotification.SourceAddressMode)
			{
				case ZoneStatusChangeNotification_SourceAddressMode_NO_PAN:
					shell_write(NO_PAN);
					break;
				case ZoneStatusChangeNotification_SourceAddressMode_Reserved:
					shell_write(Reserved);
					break;
				case ZoneStatusChangeNotification_SourceAddressMode_Address_16b:
					shell_write(Address_16b);
					break;
				case ZoneStatusChangeNotification_SourceAddressMode_Address_64b:
					shell_write(Address_64b);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.ZoneStatusChangeNotification.SourceAddressMode);
					break;
			}
			break;

#endif  /* IASCLUSTERCOMMANDS_ENABLE */

#if EXPORTINGPERSISTENTDATATOHOST_ENABLE
		case HostPersistentDataManagerAvailableResponse_FSCI_ID:
			shell_write("HostPersistentDataManagerAvailableResponse");
			break;

		case LoadRecordResponse_FSCI_ID:
			shell_write("LoadRecordResponse");
			shell_write(" -> ");
			switch (container->Data.LoadRecordResponse.Status)
			{
				case LoadRecordResponse_Status_NoRecordFound:
					shell_write(NoRecordFound);
					break;
				case LoadRecordResponse_Status_RecordRecovered:
					shell_write(RecordRecovered);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.LoadRecordResponse.Status);
					break;
			}
			break;

		case SaveRecordResponse_FSCI_ID:
			shell_write("SaveRecordResponse");
			break;

#endif  /* EXPORTINGPERSISTENTDATATOHOST_ENABLE */

#if EXTENDEDUTILITIES_ENABLE
		case RouterDiscoveryConfirm_FSCI_ID:
			shell_write("RouterDiscoveryConfirm");
			shell_write(" -> ");
			switch (container->Data.RouterDiscoveryConfirm.Status)
			{
				case RouterDiscoveryConfirm_Status_Success:
					shell_write(Success);
					break;
				case RouterDiscoveryConfirm_Status_Failed:
					shell_write(Failed);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.RouterDiscoveryConfirm.Status);
					break;
			}
			shell_write(" -> ");
			switch (container->Data.RouterDiscoveryConfirm.NwkStatus)
			{
				case RouterDiscoveryConfirm_NwkStatus_SUCCESS:
					shell_write(SUCCESS);
					break;
				case RouterDiscoveryConfirm_NwkStatus_COUNTER_ERROR:
					shell_write(COUNTER_ERROR);
					break;
				case RouterDiscoveryConfirm_NwkStatus_IMPROPER_KEY_TYPE:
					shell_write(IMPROPER_KEY_TYPE);
					break;
				case RouterDiscoveryConfirm_NwkStatus_IMPROPER_SECURITY_LEVEL:
					shell_write(IMPROPER_SECURITY_LEVEL);
					break;
				case RouterDiscoveryConfirm_NwkStatus_UNSUPPORTED_LEGACY:
					shell_write(UNSUPPORTED_LEGACY);
					break;
				case RouterDiscoveryConfirm_NwkStatus_BEACON_LOSS:
					shell_write(BEACON_LOSS);
					break;
				case RouterDiscoveryConfirm_NwkStatus_CHANNEL_ACCESS_FAILURE:
					shell_write(CHANNEL_ACCESS_FAILURE);
					break;
				case RouterDiscoveryConfirm_NwkStatus_DENIED:
					shell_write(DENIED);
					break;
				case RouterDiscoveryConfirm_NwkStatus_DISABLE_TRX_FAILURE:
					shell_write(DISABLE_TRX_FAILURE);
					break;
				case RouterDiscoveryConfirm_NwkStatus_FAILED_SECURITY_CHECK:
					shell_write(FAILED_SECURITY_CHECK);
					break;
				case RouterDiscoveryConfirm_NwkStatus_FRAME_TOO_LONG:
					shell_write(FRAME_TOO_LONG);
					break;
				case RouterDiscoveryConfirm_NwkStatus_INVALID_GTS:
					shell_write(INVALID_GTS);
					break;
				case RouterDiscoveryConfirm_NwkStatus_INVALID_HANDLE:
					shell_write(INVALID_HANDLE);
					break;
				case RouterDiscoveryConfirm_NwkStatus_INVALID_PARAMETER:
					shell_write(INVALID_PARAMETER);
					break;
				case RouterDiscoveryConfirm_NwkStatus_NO_ACK:
					shell_write(NO_ACK);
					break;
				case RouterDiscoveryConfirm_NwkStatus_NO_BEACON:
					shell_write(NO_BEACON);
					break;
				case RouterDiscoveryConfirm_NwkStatus_NO_DATA:
					shell_write(NO_DATA);
					break;
				case RouterDiscoveryConfirm_NwkStatus_NO_SHORT_ADDRESS:
					shell_write(NO_SHORT_ADDRESS);
					break;
				case RouterDiscoveryConfirm_NwkStatus_OUT_OF_CAP:
					shell_write(OUT_OF_CAP);
					break;
				case RouterDiscoveryConfirm_NwkStatus_PAN_ID_CONFLICT:
					shell_write(PAN_ID_CONFLICT);
					break;
				case RouterDiscoveryConfirm_NwkStatus_REALIGNMENT:
					shell_write(REALIGNMENT);
					break;
				case RouterDiscoveryConfirm_NwkStatus_TRANSACTION_EXPIRED:
					shell_write(TRANSACTION_EXPIRED);
					break;
				case RouterDiscoveryConfirm_NwkStatus_TRANSACTION_OVERFLOW:
					shell_write(TRANSACTION_OVERFLOW);
					break;
				case RouterDiscoveryConfirm_NwkStatus_TX_ACTIVE:
					shell_write(TX_ACTIVE);
					break;
				case RouterDiscoveryConfirm_NwkStatus_UNAVAILABLE_KEY:
					shell_write(UNAVAILABLE_KEY);
					break;
				case RouterDiscoveryConfirm_NwkStatus_UNSUPPORTED_ATTRIBUTE:
					shell_write(UNSUPPORTED_ATTRIBUTE);
					break;
				case RouterDiscoveryConfirm_NwkStatus_SCAN_IN_PROGRESS:
					shell_write(SCAN_IN_PROGRESS);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.RouterDiscoveryConfirm.NwkStatus);
					break;
			}
			break;

		case APSDataConfirmFail_FSCI_ID:
			shell_write("APSDataConfirmFail");
			shell_write(" -> ");
			switch (container->Data.APSDataConfirmFail.DestAddressingMode)
			{
				case APSDataConfirmFail_DestAddressingMode_NO_PAN:
					shell_write(NO_PAN);
					break;
				case APSDataConfirmFail_DestAddressingMode_Reserved:
					shell_write(Reserved);
					break;
				case APSDataConfirmFail_DestAddressingMode_Address_16b:
					shell_write(Address_16b);
					break;
				case APSDataConfirmFail_DestAddressingMode_Address_64b:
					shell_write(Address_64b);
					break;
				default:
					shell_printf("Unrecognized status 0x%02X", container->Data.APSDataConfirmFail.DestAddressingMode);
					break;
			}
			break;

#endif  /* EXTENDEDUTILITIES_ENABLE */

	}

	shell_refresh();
}
