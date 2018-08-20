/* Source file generated from ZigBee3_0_6.0.7.xml */
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
#include "cmd_zigbee3_0_6.0.7.h"

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

#if TOUCHLINKCOMMANDS_ENABLE
static const char *Failure = "Failure";
#endif  /* TOUCHLINKCOMMANDS_ENABLE */

#if EXPORTINGPERSISTENTDATATOHOST_ENABLE
static const char *NoRecordFound = "NoRecordFound";
static const char *RecordRecovered = "RecordRecovered";
#endif  /* EXPORTINGPERSISTENTDATATOHOST_ENABLE */


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
