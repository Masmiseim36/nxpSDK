/* Source file generated from ZigBee3_0_6.0.8.xml */
/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*==================================================================================================
Include Files
==================================================================================================*/
#include "cmd_zigbee3_0_6.0.9.h"

/*==================================================================================================
Private Prototypes
==================================================================================================*/
#if ZIGBEESTACKANDNODEMANAGEMENTCOMMANDS_ENABLE
static memStatus_t Load_Status(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_LogMessage(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_DataIndication(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_NodeClusterList(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_NodeClusterAttributeList(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_NodeCommandIDList(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_VersionList(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_NetworkJoinedFormed(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_PermitJoinStatusResponse(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_NonFactoryNewRestart(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_FactoryNewRestart(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_BindResponse(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_UnbindResponse(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_NetworkAddressResponse(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_IEEEAddressResponse(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_NodeDescriptorResponse(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_SimpleDescriptorResponse(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_PowerDescriptorResponse(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_ActiveEndpointResponse(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_MatchDescriptorResponse(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_DeviceAnnounce(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_UserDescriptorResponse(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_UserDescriptorNotify(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_ComplexDescriptorResponse(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_ManagementLeaveResponse(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_LeaveIndication(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_ManagementNetworkUpdateResponse(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_SystemServerDiscoveryResponse(zigEvtContainer_t *container, uint8_t *pPayload);
#endif  /* ZIGBEESTACKANDNODEMANAGEMENTCOMMANDS_ENABLE */

#if ENTIREPROFILE_ENABLE
static memStatus_t Load_ManagementLQIResponse(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_AttributeDiscoveryResponse(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_AuthenticateResponse(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_CodeResponse(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_ConfigureReportingResponse(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_ReadIndividualAttributeResponse(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_WriteAttributeResponse(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_ReportIndividualAttributeResponse(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_DefaultResponse(zigEvtContainer_t *container, uint8_t *pPayload);
#endif  /* ENTIREPROFILE_ENABLE */

#if GROUPCLUSTERCOMMANDS_ENABLE
static memStatus_t Load_AddGroupResponse(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_ViewGroupResponse(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_GetGroupMembershipResponse(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_RemoveGroupResponse(zigEvtContainer_t *container, uint8_t *pPayload);
#endif  /* GROUPCLUSTERCOMMANDS_ENABLE */

#if DIAGNOSTICSCLUSTERCOMMANDS_ENABLE
static memStatus_t Load_DiagnosticResponse(zigEvtContainer_t *container, uint8_t *pPayload);
#endif  /* DIAGNOSTICSCLUSTERCOMMANDS_ENABLE */

#if SCENESCLUSTERCOMMANDS_ENABLE
static memStatus_t Load_ViewSceneResponse(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_ViewEnhancedSceneResponse(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_AddSceneResponse(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_CopySceneResponse(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_RemoveSceneResponse(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_RemoveAllSceneResponse(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_AddEnhancedSceneResponse(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_StoreSceneResponse(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_SceneMembershipResponse(zigEvtContainer_t *container, uint8_t *pPayload);
#endif  /* SCENESCLUSTERCOMMANDS_ENABLE */

#if TOUCHLINKCOMMANDS_ENABLE
static memStatus_t Load_TouchlinkStatus(zigEvtContainer_t *container, uint8_t *pPayload);
#endif  /* TOUCHLINKCOMMANDS_ENABLE */

#if IASCLUSTERCOMMANDS_ENABLE
static memStatus_t Load_ZoneStatusChangeNotification(zigEvtContainer_t *container, uint8_t *pPayload);
#endif  /* IASCLUSTERCOMMANDS_ENABLE */

#if EXPORTINGPERSISTENTDATATOHOST_ENABLE
static memStatus_t Load_HostPersistentDataManagerAvailableResponse(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_LoadRecordResponse(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_SaveRecordResponse(zigEvtContainer_t *container, uint8_t *pPayload);
#endif  /* EXPORTINGPERSISTENTDATATOHOST_ENABLE */

#if EXTENDEDUTILITIES_ENABLE
static memStatus_t Load_RouterDiscoveryConfirm(zigEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_APSDataConfirmFail(zigEvtContainer_t *container, uint8_t *pPayload);
#endif  /* EXTENDEDUTILITIES_ENABLE */

/*==================================================================================================
Private global variables declarations
==================================================================================================*/
static const zigEvtHandler_t evtHandlerTbl[] =
{
#if ZIGBEESTACKANDNODEMANAGEMENTCOMMANDS_ENABLE
	{Status_FSCI_ID, Load_Status},
	{LogMessage_FSCI_ID, Load_LogMessage},
	{DataIndication_FSCI_ID, Load_DataIndication},
	{NodeClusterList_FSCI_ID, Load_NodeClusterList},
	{NodeClusterAttributeList_FSCI_ID, Load_NodeClusterAttributeList},
	{NodeCommandIDList_FSCI_ID, Load_NodeCommandIDList},
	{VersionList_FSCI_ID, Load_VersionList},
	{NetworkJoinedFormed_FSCI_ID, Load_NetworkJoinedFormed},
	{PermitJoinStatusResponse_FSCI_ID, Load_PermitJoinStatusResponse},
	{NonFactoryNewRestart_FSCI_ID, Load_NonFactoryNewRestart},
	{FactoryNewRestart_FSCI_ID, Load_FactoryNewRestart},
	{BindResponse_FSCI_ID, Load_BindResponse},
	{UnbindResponse_FSCI_ID, Load_UnbindResponse},
	{NetworkAddressResponse_FSCI_ID, Load_NetworkAddressResponse},
	{IEEEAddressResponse_FSCI_ID, Load_IEEEAddressResponse},
	{NodeDescriptorResponse_FSCI_ID, Load_NodeDescriptorResponse},
	{SimpleDescriptorResponse_FSCI_ID, Load_SimpleDescriptorResponse},
	{PowerDescriptorResponse_FSCI_ID, Load_PowerDescriptorResponse},
	{ActiveEndpointResponse_FSCI_ID, Load_ActiveEndpointResponse},
	{MatchDescriptorResponse_FSCI_ID, Load_MatchDescriptorResponse},
	{DeviceAnnounce_FSCI_ID, Load_DeviceAnnounce},
	{UserDescriptorResponse_FSCI_ID, Load_UserDescriptorResponse},
	{UserDescriptorNotify_FSCI_ID, Load_UserDescriptorNotify},
	{ComplexDescriptorResponse_FSCI_ID, Load_ComplexDescriptorResponse},
	{ManagementLeaveResponse_FSCI_ID, Load_ManagementLeaveResponse},
	{LeaveIndication_FSCI_ID, Load_LeaveIndication},
	{ManagementNetworkUpdateResponse_FSCI_ID, Load_ManagementNetworkUpdateResponse},
	{SystemServerDiscoveryResponse_FSCI_ID, Load_SystemServerDiscoveryResponse},
#endif  /* ZIGBEESTACKANDNODEMANAGEMENTCOMMANDS_ENABLE */

#if ENTIREPROFILE_ENABLE
	{ManagementLQIResponse_FSCI_ID, Load_ManagementLQIResponse},
	{AttributeDiscoveryResponse_FSCI_ID, Load_AttributeDiscoveryResponse},
	{AuthenticateResponse_FSCI_ID, Load_AuthenticateResponse},
	{CodeResponse_FSCI_ID, Load_CodeResponse},
	{ConfigureReportingResponse_FSCI_ID, Load_ConfigureReportingResponse},
	{ReadIndividualAttributeResponse_FSCI_ID, Load_ReadIndividualAttributeResponse},
	{WriteAttributeResponse_FSCI_ID, Load_WriteAttributeResponse},
	{ReportIndividualAttributeResponse_FSCI_ID, Load_ReportIndividualAttributeResponse},
	{DefaultResponse_FSCI_ID, Load_DefaultResponse},
#endif  /* ENTIREPROFILE_ENABLE */

#if GROUPCLUSTERCOMMANDS_ENABLE
	{AddGroupResponse_FSCI_ID, Load_AddGroupResponse},
	{ViewGroupResponse_FSCI_ID, Load_ViewGroupResponse},
	{GetGroupMembershipResponse_FSCI_ID, Load_GetGroupMembershipResponse},
	{RemoveGroupResponse_FSCI_ID, Load_RemoveGroupResponse},
#endif  /* GROUPCLUSTERCOMMANDS_ENABLE */

#if DIAGNOSTICSCLUSTERCOMMANDS_ENABLE
	{DiagnosticResponse_FSCI_ID, Load_DiagnosticResponse},
#endif  /* DIAGNOSTICSCLUSTERCOMMANDS_ENABLE */

#if SCENESCLUSTERCOMMANDS_ENABLE
	{ViewSceneResponse_FSCI_ID, Load_ViewSceneResponse},
	{ViewEnhancedSceneResponse_FSCI_ID, Load_ViewEnhancedSceneResponse},
	{AddSceneResponse_FSCI_ID, Load_AddSceneResponse},
	{CopySceneResponse_FSCI_ID, Load_CopySceneResponse},
	{RemoveSceneResponse_FSCI_ID, Load_RemoveSceneResponse},
	{RemoveAllSceneResponse_FSCI_ID, Load_RemoveAllSceneResponse},
	{AddEnhancedSceneResponse_FSCI_ID, Load_AddEnhancedSceneResponse},
	{StoreSceneResponse_FSCI_ID, Load_StoreSceneResponse},
	{SceneMembershipResponse_FSCI_ID, Load_SceneMembershipResponse},
#endif  /* SCENESCLUSTERCOMMANDS_ENABLE */

#if TOUCHLINKCOMMANDS_ENABLE
	{TouchlinkStatus_FSCI_ID, Load_TouchlinkStatus},
#endif  /* TOUCHLINKCOMMANDS_ENABLE */

#if IASCLUSTERCOMMANDS_ENABLE
	{ZoneStatusChangeNotification_FSCI_ID, Load_ZoneStatusChangeNotification},
#endif  /* IASCLUSTERCOMMANDS_ENABLE */

#if EXPORTINGPERSISTENTDATATOHOST_ENABLE
	{HostPersistentDataManagerAvailableResponse_FSCI_ID, Load_HostPersistentDataManagerAvailableResponse},
	{LoadRecordResponse_FSCI_ID, Load_LoadRecordResponse},
	{SaveRecordResponse_FSCI_ID, Load_SaveRecordResponse},
#endif  /* EXPORTINGPERSISTENTDATATOHOST_ENABLE */

#if EXTENDEDUTILITIES_ENABLE
	{RouterDiscoveryConfirm_FSCI_ID, Load_RouterDiscoveryConfirm},
	{APSDataConfirmFail_FSCI_ID, Load_APSDataConfirmFail},
#endif  /* EXTENDEDUTILITIES_ENABLE */
};

/*==================================================================================================
Public Functions
==================================================================================================*/
#if ZIGBEESTACKANDNODEMANAGEMENTCOMMANDS_ENABLE
/*!*************************************************************************************************
\fn		static memStatus_t Load_Status(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_Status(zigEvtContainer_t *container, uint8_t *pPayload)
{
	Status_t *evt = &(container->Data.Status);

	/* Store (OG, OC) in ID */
	container->id = Status_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(Status_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_LogMessage(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_LogMessage(zigEvtContainer_t *container, uint8_t *pPayload)
{
	LogMessage_t *evt = &(container->Data.LogMessage);

	/* Store (OG, OC) in ID */
	container->id = LogMessage_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(LogMessage_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_DataIndication(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_DataIndication(zigEvtContainer_t *container, uint8_t *pPayload)
{
	DataIndication_t *evt = &(container->Data.DataIndication);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = DataIndication_FSCI_ID;

	evt->Status = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->ProfileID), pPayload + idx, sizeof(evt->ProfileID)); idx += sizeof(evt->ProfileID);
	FLib_MemCpy(&(evt->ClusterID), pPayload + idx, sizeof(evt->ClusterID)); idx += sizeof(evt->ClusterID);
	evt->SourceEndpoint = pPayload[idx]; idx++;
	evt->DestinationEndpoint = pPayload[idx]; idx++;
	evt->SourceAddressMode = (DataIndication_SourceAddressMode_t)pPayload[idx]; idx++;

	switch (evt->SourceAddressMode)
	{
		case DataIndication_SourceAddressMode_NO_PAN:
			break;

		case DataIndication_SourceAddressMode_Reserved:
			break;

		case DataIndication_SourceAddressMode_Address_16b:
			FLib_MemCpy(&(evt->SourceAddress.Address_16b), pPayload + idx, sizeof(evt->SourceAddress.Address_16b)); idx += sizeof(evt->SourceAddress.Address_16b);
			break;

		case DataIndication_SourceAddressMode_Address_64b:
			FLib_MemCpy(&(evt->SourceAddress.Address_64b), pPayload + idx, sizeof(evt->SourceAddress.Address_64b)); idx += sizeof(evt->SourceAddress.Address_64b);
			break;
	}
	evt->DestAddressingMode = (DataIndication_DestAddressingMode_t)pPayload[idx]; idx++;

	switch (evt->DestAddressingMode)
	{
		case DataIndication_DestAddressingMode_NO_PAN:
			break;

		case DataIndication_DestAddressingMode_Reserved:
			break;

		case DataIndication_DestAddressingMode_Address_16b:
			FLib_MemCpy(&(evt->DestinationAddress.Address_16b), pPayload + idx, sizeof(evt->DestinationAddress.Address_16b)); idx += sizeof(evt->DestinationAddress.Address_16b);
			break;

		case DataIndication_DestAddressingMode_Address_64b:
			FLib_MemCpy(&(evt->DestinationAddress.Address_64b), pPayload + idx, sizeof(evt->DestinationAddress.Address_64b)); idx += sizeof(evt->DestinationAddress.Address_64b);
			break;
	}
	evt->PayloadSize = pPayload[idx]; idx++;

	if (evt->PayloadSize > 0)
	{
		evt->Payload = MEM_BufferAlloc(evt->PayloadSize);

		if (!evt->Payload)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->Payload = NULL;
	}

	FLib_MemCpy(evt->Payload, pPayload + idx, evt->PayloadSize); idx += evt->PayloadSize;

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_NodeClusterList(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	Node Cluster List - Sent by gateway node after reset.
***************************************************************************************************/
static memStatus_t Load_NodeClusterList(zigEvtContainer_t *container, uint8_t *pPayload)
{
	NodeClusterList_t *evt = &(container->Data.NodeClusterList);

	/* Store (OG, OC) in ID */
	container->id = NodeClusterList_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(NodeClusterList_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_NodeClusterAttributeList(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	Node Cluster Attribute List - Sent by Gateway node after reset.
***************************************************************************************************/
static memStatus_t Load_NodeClusterAttributeList(zigEvtContainer_t *container, uint8_t *pPayload)
{
	NodeClusterAttributeList_t *evt = &(container->Data.NodeClusterAttributeList);

	/* Store (OG, OC) in ID */
	container->id = NodeClusterAttributeList_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(NodeClusterAttributeList_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_NodeCommandIDList(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	Node Command ID List - sent by Gateway node after reset.
***************************************************************************************************/
static memStatus_t Load_NodeCommandIDList(zigEvtContainer_t *container, uint8_t *pPayload)
{
	NodeCommandIDList_t *evt = &(container->Data.NodeCommandIDList);

	/* Store (OG, OC) in ID */
	container->id = NodeCommandIDList_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(NodeCommandIDList_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_VersionList(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_VersionList(zigEvtContainer_t *container, uint8_t *pPayload)
{
	VersionList_t *evt = &(container->Data.VersionList);

	/* Store (OG, OC) in ID */
	container->id = VersionList_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(VersionList_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_NetworkJoinedFormed(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_NetworkJoinedFormed(zigEvtContainer_t *container, uint8_t *pPayload)
{
	NetworkJoinedFormed_t *evt = &(container->Data.NetworkJoinedFormed);

	/* Store (OG, OC) in ID */
	container->id = NetworkJoinedFormed_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(NetworkJoinedFormed_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_PermitJoinStatusResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_PermitJoinStatusResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	PermitJoinStatusResponse_t *evt = &(container->Data.PermitJoinStatusResponse);

	/* Store (OG, OC) in ID */
	container->id = PermitJoinStatusResponse_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(PermitJoinStatusResponse_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_NonFactoryNewRestart(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_NonFactoryNewRestart(zigEvtContainer_t *container, uint8_t *pPayload)
{
	NonFactoryNewRestart_t *evt = &(container->Data.NonFactoryNewRestart);

	/* Store (OG, OC) in ID */
	container->id = NonFactoryNewRestart_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(NonFactoryNewRestart_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_FactoryNewRestart(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_FactoryNewRestart(zigEvtContainer_t *container, uint8_t *pPayload)
{
	FactoryNewRestart_t *evt = &(container->Data.FactoryNewRestart);

	/* Store (OG, OC) in ID */
	container->id = FactoryNewRestart_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(FactoryNewRestart_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_BindResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_BindResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	BindResponse_t *evt = &(container->Data.BindResponse);

	/* Store (OG, OC) in ID */
	container->id = BindResponse_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(BindResponse_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_UnbindResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_UnbindResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	UnbindResponse_t *evt = &(container->Data.UnbindResponse);

	/* Store (OG, OC) in ID */
	container->id = UnbindResponse_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(UnbindResponse_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_NetworkAddressResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_NetworkAddressResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	NetworkAddressResponse_t *evt = &(container->Data.NetworkAddressResponse);

	/* Store (OG, OC) in ID */
	container->id = NetworkAddressResponse_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(NetworkAddressResponse_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_IEEEAddressResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_IEEEAddressResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	IEEEAddressResponse_t *evt = &(container->Data.IEEEAddressResponse);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = IEEEAddressResponse_FSCI_ID;

	evt->SequenceNumber = pPayload[idx]; idx++;
	evt->Status = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->IEEEAddress), pPayload + idx, sizeof(evt->IEEEAddress)); idx += sizeof(evt->IEEEAddress);
	FLib_MemCpy(&(evt->shortAddress), pPayload + idx, sizeof(evt->shortAddress)); idx += sizeof(evt->shortAddress);
	evt->NumberOfAssociatedDevices = pPayload[idx]; idx++;
	evt->StartIndex = pPayload[idx]; idx++;

	if (evt->NumberOfAssociatedDevices > 0)
	{
		evt->DeviceList = MEM_BufferAlloc(evt->NumberOfAssociatedDevices * 2);

		if (!evt->DeviceList)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->DeviceList = NULL;
	}

	FLib_MemCpy(evt->DeviceList, pPayload + idx, evt->NumberOfAssociatedDevices * 2); idx += evt->NumberOfAssociatedDevices * 2;

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_NodeDescriptorResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_NodeDescriptorResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	NodeDescriptorResponse_t *evt = &(container->Data.NodeDescriptorResponse);

	/* Store (OG, OC) in ID */
	container->id = NodeDescriptorResponse_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(NodeDescriptorResponse_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_SimpleDescriptorResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_SimpleDescriptorResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	SimpleDescriptorResponse_t *evt = &(container->Data.SimpleDescriptorResponse);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = SimpleDescriptorResponse_FSCI_ID;

	evt->SequenceNumber = pPayload[idx]; idx++;
	evt->Status = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->NwkAddress), pPayload + idx, sizeof(evt->NwkAddress)); idx += sizeof(evt->NwkAddress);
	evt->Length = pPayload[idx]; idx++;
	evt->EndPoint = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->Profile), pPayload + idx, sizeof(evt->Profile)); idx += sizeof(evt->Profile);
	FLib_MemCpy(&(evt->DeviceID), pPayload + idx, sizeof(evt->DeviceID)); idx += sizeof(evt->DeviceID);
	evt->BitFields = pPayload[idx]; idx++;
	evt->InClusterCount = pPayload[idx]; idx++;

	if (evt->InClusterCount > 0)
	{
		evt->InClusterList = MEM_BufferAlloc(evt->InClusterCount * 2);

		if (!evt->InClusterList)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->InClusterList = NULL;
	}

	FLib_MemCpy(evt->InClusterList, pPayload + idx, evt->InClusterCount * 2); idx += evt->InClusterCount * 2;
	evt->OutClusterCount = pPayload[idx]; idx++;

	if (evt->OutClusterCount > 0)
	{
		evt->OutClusterList = MEM_BufferAlloc(evt->OutClusterCount * 2);

		if (!evt->OutClusterList)
		{
			MEM_BufferFree(evt->InClusterList);
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->OutClusterList = NULL;
	}

	FLib_MemCpy(evt->OutClusterList, pPayload + idx, evt->OutClusterCount * 2); idx += evt->OutClusterCount * 2;

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_PowerDescriptorResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_PowerDescriptorResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	PowerDescriptorResponse_t *evt = &(container->Data.PowerDescriptorResponse);

	/* Store (OG, OC) in ID */
	container->id = PowerDescriptorResponse_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(PowerDescriptorResponse_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_ActiveEndpointResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_ActiveEndpointResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	ActiveEndpointResponse_t *evt = &(container->Data.ActiveEndpointResponse);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = ActiveEndpointResponse_FSCI_ID;

	evt->SequenceNumber = pPayload[idx]; idx++;
	evt->Status = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->Address), pPayload + idx, sizeof(evt->Address)); idx += sizeof(evt->Address);
	evt->EndpointCount = pPayload[idx]; idx++;

	if (evt->EndpointCount > 0)
	{
		evt->ActiveEndPointList = MEM_BufferAlloc(evt->EndpointCount);

		if (!evt->ActiveEndPointList)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->ActiveEndPointList = NULL;
	}

	FLib_MemCpy(evt->ActiveEndPointList, pPayload + idx, evt->EndpointCount); idx += evt->EndpointCount;

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_MatchDescriptorResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_MatchDescriptorResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	MatchDescriptorResponse_t *evt = &(container->Data.MatchDescriptorResponse);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = MatchDescriptorResponse_FSCI_ID;

	evt->SequenceNumber = pPayload[idx]; idx++;
	evt->Status = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->NetworkAddress), pPayload + idx, sizeof(evt->NetworkAddress)); idx += sizeof(evt->NetworkAddress);
	evt->LengthOfList = pPayload[idx]; idx++;

	if (evt->LengthOfList > 0)
	{
		evt->MatchList = MEM_BufferAlloc(evt->LengthOfList);

		if (!evt->MatchList)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->MatchList = NULL;
	}

	FLib_MemCpy(evt->MatchList, pPayload + idx, evt->LengthOfList); idx += evt->LengthOfList;

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_DeviceAnnounce(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	DeviceAnnounce
***************************************************************************************************/
static memStatus_t Load_DeviceAnnounce(zigEvtContainer_t *container, uint8_t *pPayload)
{
	DeviceAnnounce_t *evt = &(container->Data.DeviceAnnounce);

	/* Store (OG, OC) in ID */
	container->id = DeviceAnnounce_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(DeviceAnnounce_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_UserDescriptorResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_UserDescriptorResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	UserDescriptorResponse_t *evt = &(container->Data.UserDescriptorResponse);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = UserDescriptorResponse_FSCI_ID;

	evt->SequenceNumber = pPayload[idx]; idx++;
	evt->Status = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->NetworkAddressOfOnterest), pPayload + idx, sizeof(evt->NetworkAddressOfOnterest)); idx += sizeof(evt->NetworkAddressOfOnterest);
	evt->LengthOfData = pPayload[idx]; idx++;

	if (evt->LengthOfData > 0)
	{
		evt->DataList = MEM_BufferAlloc(evt->LengthOfData);

		if (!evt->DataList)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->DataList = NULL;
	}

	FLib_MemCpy(evt->DataList, pPayload + idx, evt->LengthOfData); idx += evt->LengthOfData;

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_UserDescriptorNotify(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_UserDescriptorNotify(zigEvtContainer_t *container, uint8_t *pPayload)
{
	UserDescriptorNotify_t *evt = &(container->Data.UserDescriptorNotify);

	/* Store (OG, OC) in ID */
	container->id = UserDescriptorNotify_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(UserDescriptorNotify_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_ComplexDescriptorResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_ComplexDescriptorResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	ComplexDescriptorResponse_t *evt = &(container->Data.ComplexDescriptorResponse);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = ComplexDescriptorResponse_FSCI_ID;

	evt->SequenceNumber = pPayload[idx]; idx++;
	evt->Status = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->NetworkAddressOfOnterest), pPayload + idx, sizeof(evt->NetworkAddressOfOnterest)); idx += sizeof(evt->NetworkAddressOfOnterest);
	evt->Length = pPayload[idx]; idx++;
	evt->XMLTag = pPayload[idx]; idx++;
	evt->FieldCount = pPayload[idx]; idx++;

	if (evt->FieldCount > 0)
	{
		evt->FieldValues = MEM_BufferAlloc(evt->FieldCount);

		if (!evt->FieldValues)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->FieldValues = NULL;
	}

	FLib_MemCpy(evt->FieldValues, pPayload + idx, evt->FieldCount); idx += evt->FieldCount;

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_ManagementLeaveResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_ManagementLeaveResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	ManagementLeaveResponse_t *evt = &(container->Data.ManagementLeaveResponse);

	/* Store (OG, OC) in ID */
	container->id = ManagementLeaveResponse_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(ManagementLeaveResponse_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_LeaveIndication(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_LeaveIndication(zigEvtContainer_t *container, uint8_t *pPayload)
{
	LeaveIndication_t *evt = &(container->Data.LeaveIndication);

	/* Store (OG, OC) in ID */
	container->id = LeaveIndication_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(LeaveIndication_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_ManagementNetworkUpdateResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_ManagementNetworkUpdateResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	ManagementNetworkUpdateResponse_t *evt = &(container->Data.ManagementNetworkUpdateResponse);

	/* Store (OG, OC) in ID */
	container->id = ManagementNetworkUpdateResponse_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(ManagementNetworkUpdateResponse_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_SystemServerDiscoveryResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_SystemServerDiscoveryResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	SystemServerDiscoveryResponse_t *evt = &(container->Data.SystemServerDiscoveryResponse);

	/* Store (OG, OC) in ID */
	container->id = SystemServerDiscoveryResponse_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(SystemServerDiscoveryResponse_t));

	return MEM_SUCCESS_c;
}

#endif  /* ZIGBEESTACKANDNODEMANAGEMENTCOMMANDS_ENABLE */

#if ENTIREPROFILE_ENABLE
/*!*************************************************************************************************
\fn		static memStatus_t Load_ManagementLQIResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_ManagementLQIResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	ManagementLQIResponse_t *evt = &(container->Data.ManagementLQIResponse);

	/* Store (OG, OC) in ID */
	container->id = ManagementLQIResponse_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(ManagementLQIResponse_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_AttributeDiscoveryResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_AttributeDiscoveryResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	AttributeDiscoveryResponse_t *evt = &(container->Data.AttributeDiscoveryResponse);

	/* Store (OG, OC) in ID */
	container->id = AttributeDiscoveryResponse_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(AttributeDiscoveryResponse_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_AuthenticateResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_AuthenticateResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	AuthenticateResponse_t *evt = &(container->Data.AuthenticateResponse);

	/* Store (OG, OC) in ID */
	container->id = AuthenticateResponse_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(AuthenticateResponse_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_CodeResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_CodeResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	CodeResponse_t *evt = &(container->Data.CodeResponse);

	/* Store (OG, OC) in ID */
	container->id = CodeResponse_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(CodeResponse_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_ConfigureReportingResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_ConfigureReportingResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	ConfigureReportingResponse_t *evt = &(container->Data.ConfigureReportingResponse);

	/* Store (OG, OC) in ID */
	container->id = ConfigureReportingResponse_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(ConfigureReportingResponse_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_ReadIndividualAttributeResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_ReadIndividualAttributeResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	ReadIndividualAttributeResponse_t *evt = &(container->Data.ReadIndividualAttributeResponse);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = ReadIndividualAttributeResponse_FSCI_ID;

	evt->SequenceNumber = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->SrcAddress), pPayload + idx, sizeof(evt->SrcAddress)); idx += sizeof(evt->SrcAddress);
	evt->Endpoint = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->ClusterID), pPayload + idx, sizeof(evt->ClusterID)); idx += sizeof(evt->ClusterID);
	FLib_MemCpy(&(evt->AttributeEnum), pPayload + idx, sizeof(evt->AttributeEnum)); idx += sizeof(evt->AttributeEnum);
	evt->AttributeStatus = pPayload[idx]; idx++;
	evt->AttributeDataType = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->SizeOfTheAttributes), pPayload + idx, sizeof(evt->SizeOfTheAttributes)); idx += sizeof(evt->SizeOfTheAttributes);

	if (evt->SizeOfTheAttributes > 0)
	{
		evt->DataByteList = MEM_BufferAlloc(evt->SizeOfTheAttributes);

		if (!evt->DataByteList)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->DataByteList = NULL;
	}

	FLib_MemCpy(evt->DataByteList, pPayload + idx, evt->SizeOfTheAttributes); idx += evt->SizeOfTheAttributes;

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_WriteAttributeResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_WriteAttributeResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	WriteAttributeResponse_t *evt = &(container->Data.WriteAttributeResponse);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = WriteAttributeResponse_FSCI_ID;

	evt->SequenceNumber = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->SrcAddress), pPayload + idx, sizeof(evt->SrcAddress)); idx += sizeof(evt->SrcAddress);
	evt->Endpoint = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->ClusterID), pPayload + idx, sizeof(evt->ClusterID)); idx += sizeof(evt->ClusterID);
	FLib_MemCpy(&(evt->AttributeEnum), pPayload + idx, sizeof(evt->AttributeEnum)); idx += sizeof(evt->AttributeEnum);
	evt->AttributeStatus = pPayload[idx]; idx++;
	evt->AttributeDataType = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->SizeOfTheAttributes), pPayload + idx, sizeof(evt->SizeOfTheAttributes)); idx += sizeof(evt->SizeOfTheAttributes);

	if (evt->SizeOfTheAttributes > 0)
	{
		evt->DataByteList = MEM_BufferAlloc(evt->SizeOfTheAttributes);

		if (!evt->DataByteList)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->DataByteList = NULL;
	}

	FLib_MemCpy(evt->DataByteList, pPayload + idx, evt->SizeOfTheAttributes); idx += evt->SizeOfTheAttributes;

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_ReportIndividualAttributeResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_ReportIndividualAttributeResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	ReportIndividualAttributeResponse_t *evt = &(container->Data.ReportIndividualAttributeResponse);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = ReportIndividualAttributeResponse_FSCI_ID;

	evt->SequenceNumber = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->SrcAddress), pPayload + idx, sizeof(evt->SrcAddress)); idx += sizeof(evt->SrcAddress);
	evt->Endpoint = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->ClusterID), pPayload + idx, sizeof(evt->ClusterID)); idx += sizeof(evt->ClusterID);
	FLib_MemCpy(&(evt->AttributeEnum), pPayload + idx, sizeof(evt->AttributeEnum)); idx += sizeof(evt->AttributeEnum);
	evt->AttributeStatus = pPayload[idx]; idx++;
	evt->AttributeDataType = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->SizeOfTheAttributes), pPayload + idx, sizeof(evt->SizeOfTheAttributes)); idx += sizeof(evt->SizeOfTheAttributes);

	if (evt->SizeOfTheAttributes > 0)
	{
		evt->DataByteList = MEM_BufferAlloc(evt->SizeOfTheAttributes);

		if (!evt->DataByteList)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->DataByteList = NULL;
	}

	FLib_MemCpy(evt->DataByteList, pPayload + idx, evt->SizeOfTheAttributes); idx += evt->SizeOfTheAttributes;

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_DefaultResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_DefaultResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	DefaultResponse_t *evt = &(container->Data.DefaultResponse);

	/* Store (OG, OC) in ID */
	container->id = DefaultResponse_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(DefaultResponse_t));

	return MEM_SUCCESS_c;
}

#endif  /* ENTIREPROFILE_ENABLE */

#if GROUPCLUSTERCOMMANDS_ENABLE
/*!*************************************************************************************************
\fn		static memStatus_t Load_AddGroupResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_AddGroupResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	AddGroupResponse_t *evt = &(container->Data.AddGroupResponse);

	/* Store (OG, OC) in ID */
	container->id = AddGroupResponse_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(AddGroupResponse_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_ViewGroupResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_ViewGroupResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	ViewGroupResponse_t *evt = &(container->Data.ViewGroupResponse);

	/* Store (OG, OC) in ID */
	container->id = ViewGroupResponse_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(ViewGroupResponse_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_GetGroupMembershipResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_GetGroupMembershipResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	GetGroupMembershipResponse_t *evt = &(container->Data.GetGroupMembershipResponse);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = GetGroupMembershipResponse_FSCI_ID;

	evt->SequenceNumber = pPayload[idx]; idx++;
	evt->Endpoint = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->ClusterID), pPayload + idx, sizeof(evt->ClusterID)); idx += sizeof(evt->ClusterID);
	evt->Capacity = pPayload[idx]; idx++;
	evt->GroupCount = pPayload[idx]; idx++;

	if (evt->GroupCount > 0)
	{
		evt->GroupList = MEM_BufferAlloc(evt->GroupCount * 2);

		if (!evt->GroupList)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->GroupList = NULL;
	}

	FLib_MemCpy(evt->GroupList, pPayload + idx, evt->GroupCount * 2); idx += evt->GroupCount * 2;

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_RemoveGroupResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_RemoveGroupResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	RemoveGroupResponse_t *evt = &(container->Data.RemoveGroupResponse);

	/* Store (OG, OC) in ID */
	container->id = RemoveGroupResponse_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(RemoveGroupResponse_t));

	return MEM_SUCCESS_c;
}

#endif  /* GROUPCLUSTERCOMMANDS_ENABLE */

#if DIAGNOSTICSCLUSTERCOMMANDS_ENABLE
/*!*************************************************************************************************
\fn		static memStatus_t Load_DiagnosticResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_DiagnosticResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	DiagnosticResponse_t *evt = &(container->Data.DiagnosticResponse);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = DiagnosticResponse_FSCI_ID;

	evt->SeqNum = pPayload[idx]; idx++;
	evt->srcEndpoint = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->ClusterId), pPayload + idx, sizeof(evt->ClusterId)); idx += sizeof(evt->ClusterId);
	evt->Status = (DiagnosticResponse_Status_t)pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->RequestLatency), pPayload + idx, sizeof(evt->RequestLatency)); idx += sizeof(evt->RequestLatency);
	FLib_MemCpy(&(evt->ResponseLatency), pPayload + idx, sizeof(evt->ResponseLatency)); idx += sizeof(evt->ResponseLatency);
	FLib_MemCpy(&(evt->Offset), pPayload + idx, sizeof(evt->Offset)); idx += sizeof(evt->Offset);
	evt->SequenceNumber = pPayload[idx]; idx++;
	evt->PayloadSize = pPayload[idx]; idx++;

	if (evt->PayloadSize > 0)
	{
		evt->Payload = MEM_BufferAlloc(evt->PayloadSize);

		if (!evt->Payload)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->Payload = NULL;
	}

	FLib_MemCpy(evt->Payload, pPayload + idx, evt->PayloadSize); idx += evt->PayloadSize;

	return MEM_SUCCESS_c;
}

#endif  /* DIAGNOSTICSCLUSTERCOMMANDS_ENABLE */

#if SCENESCLUSTERCOMMANDS_ENABLE
/*!*************************************************************************************************
\fn		static memStatus_t Load_ViewSceneResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_ViewSceneResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	ViewSceneResponse_t *evt = &(container->Data.ViewSceneResponse);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = ViewSceneResponse_FSCI_ID;

	evt->SequenceNumber = pPayload[idx]; idx++;
	evt->Endpoint = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->ClusterID), pPayload + idx, sizeof(evt->ClusterID)); idx += sizeof(evt->ClusterID);
	evt->Status = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->GroupID), pPayload + idx, sizeof(evt->GroupID)); idx += sizeof(evt->GroupID);
	evt->SceneID = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->TransitionTime), pPayload + idx, sizeof(evt->TransitionTime)); idx += sizeof(evt->TransitionTime);
	evt->SceneNameLength = pPayload[idx]; idx++;
	evt->SceneNameMaxLength = pPayload[idx]; idx++;

	if (evt->SceneNameLength > 0)
	{
		evt->SceneNameData = MEM_BufferAlloc(evt->SceneNameLength);

		if (!evt->SceneNameData)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->SceneNameData = NULL;
	}

	FLib_MemCpy(evt->SceneNameData, pPayload + idx, evt->SceneNameLength); idx += evt->SceneNameLength;
	FLib_MemCpy(&(evt->ExtensionsLength), pPayload + idx, sizeof(evt->ExtensionsLength)); idx += sizeof(evt->ExtensionsLength);
	FLib_MemCpy(&(evt->ExtensionsMaxLength), pPayload + idx, sizeof(evt->ExtensionsMaxLength)); idx += sizeof(evt->ExtensionsMaxLength);

	if (evt->ExtensionsMaxLength > 0)
	{
		evt->ExtensionsData = MEM_BufferAlloc(evt->ExtensionsMaxLength);

		if (!evt->ExtensionsData)
		{
			MEM_BufferFree(evt->SceneNameData);
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->ExtensionsData = NULL;
	}

	FLib_MemCpy(evt->ExtensionsData, pPayload + idx, evt->ExtensionsMaxLength); idx += evt->ExtensionsMaxLength;

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_ViewEnhancedSceneResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_ViewEnhancedSceneResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	ViewEnhancedSceneResponse_t *evt = &(container->Data.ViewEnhancedSceneResponse);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = ViewEnhancedSceneResponse_FSCI_ID;

	evt->SequenceNumber = pPayload[idx]; idx++;
	evt->Endpoint = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->ClusterID), pPayload + idx, sizeof(evt->ClusterID)); idx += sizeof(evt->ClusterID);
	evt->Status = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->GroupID), pPayload + idx, sizeof(evt->GroupID)); idx += sizeof(evt->GroupID);
	evt->SceneID = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->TransitionTime), pPayload + idx, sizeof(evt->TransitionTime)); idx += sizeof(evt->TransitionTime);
	evt->SceneNameLength = pPayload[idx]; idx++;
	evt->SceneNameMaxLength = pPayload[idx]; idx++;

	if (evt->SceneNameLength > 0)
	{
		evt->SceneNameData = MEM_BufferAlloc(evt->SceneNameLength);

		if (!evt->SceneNameData)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->SceneNameData = NULL;
	}

	FLib_MemCpy(evt->SceneNameData, pPayload + idx, evt->SceneNameLength); idx += evt->SceneNameLength;
	FLib_MemCpy(&(evt->ExtensionsLength), pPayload + idx, sizeof(evt->ExtensionsLength)); idx += sizeof(evt->ExtensionsLength);
	FLib_MemCpy(&(evt->ExtensionsMaxLength), pPayload + idx, sizeof(evt->ExtensionsMaxLength)); idx += sizeof(evt->ExtensionsMaxLength);

	if (evt->ExtensionsLength > 0)
	{
		evt->ExtensionsData = MEM_BufferAlloc(evt->ExtensionsLength);

		if (!evt->ExtensionsData)
		{
			MEM_BufferFree(evt->SceneNameData);
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->ExtensionsData = NULL;
	}

	FLib_MemCpy(evt->ExtensionsData, pPayload + idx, evt->ExtensionsLength); idx += evt->ExtensionsLength;

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_AddSceneResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_AddSceneResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	AddSceneResponse_t *evt = &(container->Data.AddSceneResponse);

	/* Store (OG, OC) in ID */
	container->id = AddSceneResponse_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(AddSceneResponse_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_CopySceneResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_CopySceneResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	CopySceneResponse_t *evt = &(container->Data.CopySceneResponse);

	/* Store (OG, OC) in ID */
	container->id = CopySceneResponse_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(CopySceneResponse_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_RemoveSceneResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_RemoveSceneResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	RemoveSceneResponse_t *evt = &(container->Data.RemoveSceneResponse);

	/* Store (OG, OC) in ID */
	container->id = RemoveSceneResponse_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(RemoveSceneResponse_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_RemoveAllSceneResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_RemoveAllSceneResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	RemoveAllSceneResponse_t *evt = &(container->Data.RemoveAllSceneResponse);

	/* Store (OG, OC) in ID */
	container->id = RemoveAllSceneResponse_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(RemoveAllSceneResponse_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_AddEnhancedSceneResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_AddEnhancedSceneResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	AddEnhancedSceneResponse_t *evt = &(container->Data.AddEnhancedSceneResponse);

	/* Store (OG, OC) in ID */
	container->id = AddEnhancedSceneResponse_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(AddEnhancedSceneResponse_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_StoreSceneResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_StoreSceneResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	StoreSceneResponse_t *evt = &(container->Data.StoreSceneResponse);

	/* Store (OG, OC) in ID */
	container->id = StoreSceneResponse_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(StoreSceneResponse_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_SceneMembershipResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_SceneMembershipResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	SceneMembershipResponse_t *evt = &(container->Data.SceneMembershipResponse);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = SceneMembershipResponse_FSCI_ID;

	evt->SequenceNumber = pPayload[idx]; idx++;
	evt->Endpoint = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->ClusterID), pPayload + idx, sizeof(evt->ClusterID)); idx += sizeof(evt->ClusterID);
	evt->Status = pPayload[idx]; idx++;
	evt->Capacity = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->GroupID), pPayload + idx, sizeof(evt->GroupID)); idx += sizeof(evt->GroupID);
	evt->SceneCount = pPayload[idx]; idx++;

	if (evt->SceneCount > 0)
	{
		evt->SceneList = MEM_BufferAlloc(evt->SceneCount);

		if (!evt->SceneList)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->SceneList = NULL;
	}

	FLib_MemCpy(evt->SceneList, pPayload + idx, evt->SceneCount); idx += evt->SceneCount;

	return MEM_SUCCESS_c;
}

#endif  /* SCENESCLUSTERCOMMANDS_ENABLE */

#if TOUCHLINKCOMMANDS_ENABLE
/*!*************************************************************************************************
\fn		static memStatus_t Load_TouchlinkStatus(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_TouchlinkStatus(zigEvtContainer_t *container, uint8_t *pPayload)
{
	TouchlinkStatus_t *evt = &(container->Data.TouchlinkStatus);

	/* Store (OG, OC) in ID */
	container->id = TouchlinkStatus_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(TouchlinkStatus_t));

	return MEM_SUCCESS_c;
}

#endif  /* TOUCHLINKCOMMANDS_ENABLE */

#if IASCLUSTERCOMMANDS_ENABLE
/*!*************************************************************************************************
\fn		static memStatus_t Load_ZoneStatusChangeNotification(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_ZoneStatusChangeNotification(zigEvtContainer_t *container, uint8_t *pPayload)
{
	ZoneStatusChangeNotification_t *evt = &(container->Data.ZoneStatusChangeNotification);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = ZoneStatusChangeNotification_FSCI_ID;

	evt->SequenceNumber = pPayload[idx]; idx++;
	evt->EndPoint = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->ClusteID), pPayload + idx, sizeof(evt->ClusteID)); idx += sizeof(evt->ClusteID);
	evt->SourceAddressMode = (ZoneStatusChangeNotification_SourceAddressMode_t)pPayload[idx]; idx++;

	switch (evt->SourceAddressMode)
	{
		case ZoneStatusChangeNotification_SourceAddressMode_NO_PAN:
			break;

		case ZoneStatusChangeNotification_SourceAddressMode_Reserved:
			break;

		case ZoneStatusChangeNotification_SourceAddressMode_Address_16b:
			FLib_MemCpy(&(evt->SourceAddress.Address_16b), pPayload + idx, sizeof(evt->SourceAddress.Address_16b)); idx += sizeof(evt->SourceAddress.Address_16b);
			break;

		case ZoneStatusChangeNotification_SourceAddressMode_Address_64b:
			FLib_MemCpy(&(evt->SourceAddress.Address_64b), pPayload + idx, sizeof(evt->SourceAddress.Address_64b)); idx += sizeof(evt->SourceAddress.Address_64b);
			break;
	}
	FLib_MemCpy(&(evt->ZoneStatus), pPayload + idx, sizeof(evt->ZoneStatus)); idx += sizeof(evt->ZoneStatus);
	evt->ExtendedStatus = pPayload[idx]; idx++;
	evt->ZoneID = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->Delay), pPayload + idx, sizeof(evt->Delay)); idx += sizeof(evt->Delay);

	return MEM_SUCCESS_c;
}

#endif  /* IASCLUSTERCOMMANDS_ENABLE */

#if EXPORTINGPERSISTENTDATATOHOST_ENABLE
/*!*************************************************************************************************
\fn		static memStatus_t Load_HostPersistentDataManagerAvailableResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_HostPersistentDataManagerAvailableResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	HostPersistentDataManagerAvailableResponse_t *evt = &(container->Data.HostPersistentDataManagerAvailableResponse);

	/* Store (OG, OC) in ID */
	container->id = HostPersistentDataManagerAvailableResponse_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(HostPersistentDataManagerAvailableResponse_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_LoadRecordResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_LoadRecordResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	LoadRecordResponse_t *evt = &(container->Data.LoadRecordResponse);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = LoadRecordResponse_FSCI_ID;

	evt->Status = (LoadRecordResponse_Status_t)pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->Record), pPayload + idx, sizeof(evt->Record)); idx += sizeof(evt->Record);
	FLib_MemCpy(&(evt->TotalSize), pPayload + idx, sizeof(evt->TotalSize)); idx += sizeof(evt->TotalSize);
	FLib_MemCpy(&(evt->TotalNumberOfBlocks), pPayload + idx, sizeof(evt->TotalNumberOfBlocks)); idx += sizeof(evt->TotalNumberOfBlocks);
	FLib_MemCpy(&(evt->CurrentBlock), pPayload + idx, sizeof(evt->CurrentBlock)); idx += sizeof(evt->CurrentBlock);
	FLib_MemCpy(&(evt->BlockSize), pPayload + idx, sizeof(evt->BlockSize)); idx += sizeof(evt->BlockSize);

	if (evt->BlockSize > 0)
	{
		evt->Parameters = MEM_BufferAlloc(evt->BlockSize);

		if (!evt->Parameters)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->Parameters = NULL;
	}

	FLib_MemCpy(evt->Parameters, pPayload + idx, evt->BlockSize); idx += evt->BlockSize;

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_SaveRecordResponse(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_SaveRecordResponse(zigEvtContainer_t *container, uint8_t *pPayload)
{
	SaveRecordResponse_t *evt = &(container->Data.SaveRecordResponse);

	/* Store (OG, OC) in ID */
	container->id = SaveRecordResponse_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(SaveRecordResponse_t));

	return MEM_SUCCESS_c;
}

#endif  /* EXPORTINGPERSISTENTDATATOHOST_ENABLE */

#if EXTENDEDUTILITIES_ENABLE
/*!*************************************************************************************************
\fn		static memStatus_t Load_RouterDiscoveryConfirm(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_RouterDiscoveryConfirm(zigEvtContainer_t *container, uint8_t *pPayload)
{
	RouterDiscoveryConfirm_t *evt = &(container->Data.RouterDiscoveryConfirm);

	/* Store (OG, OC) in ID */
	container->id = RouterDiscoveryConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(RouterDiscoveryConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_APSDataConfirmFail(zigEvtContainer_t *container, uint8_t *pPayload)
\brief	None
***************************************************************************************************/
static memStatus_t Load_APSDataConfirmFail(zigEvtContainer_t *container, uint8_t *pPayload)
{
	APSDataConfirmFail_t *evt = &(container->Data.APSDataConfirmFail);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = APSDataConfirmFail_FSCI_ID;

	evt->Status = pPayload[idx]; idx++;
	evt->SourceEndPoint = pPayload[idx]; idx++;
	evt->DestinationEndPoint = pPayload[idx]; idx++;
	evt->DestAddressingMode = (APSDataConfirmFail_DestAddressingMode_t)pPayload[idx]; idx++;

	switch (evt->DestAddressingMode)
	{
		case APSDataConfirmFail_DestAddressingMode_NO_PAN:
			break;

		case APSDataConfirmFail_DestAddressingMode_Reserved:
			break;

		case APSDataConfirmFail_DestAddressingMode_Address_16b:
			FLib_MemCpy(&(evt->DestinationAddress.Address_16b), pPayload + idx, sizeof(evt->DestinationAddress.Address_16b)); idx += sizeof(evt->DestinationAddress.Address_16b);
			break;

		case APSDataConfirmFail_DestAddressingMode_Address_64b:
			FLib_MemCpy(&(evt->DestinationAddress.Address_64b), pPayload + idx, sizeof(evt->DestinationAddress.Address_64b)); idx += sizeof(evt->DestinationAddress.Address_64b);
			break;
	}
	evt->SequenceNumber = pPayload[idx]; idx++;

	return MEM_SUCCESS_c;
}

#endif  /* EXTENDEDUTILITIES_ENABLE */


void KHC_ZIGBEE_RX_MsgHandler(void *pData, void *param, uint8_t fsciInterface)
{
	if (!pData || !param)
	{
		return;
	}

	fsciPacket_t *frame = (fsciPacket_t *)pData;
	zigEvtContainer_t *container = (zigEvtContainer_t *)param;
	uint8_t og = frame->opGroup;
	uint8_t oc = frame->opCode;
	uint8_t *pPayload = frame->data;
	uint16_t id = (og << 8) + oc, i;

	for (i = 0; i < sizeof(evtHandlerTbl) / sizeof(evtHandlerTbl[0]); i++)
	{
		if (evtHandlerTbl[i].id == id)
		{
			evtHandlerTbl[i].handlerFunc(container, pPayload);
			break;
		}
	}
}
