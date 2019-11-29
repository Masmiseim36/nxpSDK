/* Source file generated from ThreadIP_1.2.8.xml */
/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*==================================================================================================
Include Files
==================================================================================================*/
#include "cmd_threadip_1.2.8.h"

/*==================================================================================================
Public Functions
==================================================================================================*/
#if MAC_ENABLE
/*!*************************************************************************************************
\fn		memStatus_t MAC_MacFilteringAddEntryRequest(MAC_MacFilteringAddEntryRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Add in Mac Filtering Table.

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MAC_MacFilteringAddEntryRequest(MAC_MacFilteringAddEntryRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x12, req, sizeof(MAC_MacFilteringAddEntryRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MAC_MacFilteringRemoveEntryRequest(MAC_MacFilteringRemoveEntryRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Remove entry from mac filtering table.

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MAC_MacFilteringRemoveEntryRequest(MAC_MacFilteringRemoveEntryRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x13, req, sizeof(MAC_MacFilteringRemoveEntryRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MAC_MacFilteringEnableRequest(MAC_MacFilteringEnableRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Enable or disable mac filtering

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MAC_MacFilteringEnableRequest(MAC_MacFilteringEnableRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x14, req, sizeof(MAC_MacFilteringEnableRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MAC_MacFilteringGetTableRequest(MAC_MacFilteringGetTableRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Get the current mac filtering table

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MAC_MacFilteringGetTableRequest(MAC_MacFilteringGetTableRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x15, req, sizeof(MAC_MacFilteringGetTableRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

#endif  /* MAC_ENABLE */

#if THREADMGMT_ENABLE
/*!*************************************************************************************************
\fn		memStatus_t THR_SetDeviceConfigRequest(THR_SetDeviceConfigRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Overwrites the default settings with the minimum data needed to start a node.

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_SetDeviceConfigRequest(THR_SetDeviceConfigRequest_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint8_t);  // ThrInstanceID
	msgLen += sizeof(bool_t);  // OutOfBandPreconfigured
	msgLen += sizeof(uint8_t);  // outOfBandChannel
	msgLen += sizeof(uint16_t);  // PanId
	msgLen += sizeof(uint32_t);  // ScanChannels
	msgLen += 8;  // ExtendedPanId
	msgLen += sizeof(uint8_t);  // NwkNameSize
	msgLen += req->NwkNameSize;  // NwkName
	msgLen += 16;  // MLPrefix
	msgLen += sizeof(uint8_t);  // MLprefixSizeInBits
	msgLen += sizeof(req->MasterKey);  // MasterKey

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	pMsg[idx] = req->ThrInstanceID; idx++;
	pMsg[idx] = req->OutOfBandPreconfigured; idx++;
	pMsg[idx] = req->outOfBandChannel; idx++;
	FLib_MemCpy(pMsg + idx, &(req->PanId), sizeof(req->PanId)); idx += sizeof(req->PanId);
	FLib_MemCpy(pMsg + idx, &(req->ScanChannels), sizeof(req->ScanChannels)); idx += sizeof(req->ScanChannels);
	FLib_MemCpy(pMsg + idx, req->ExtendedPanId, 8); idx += 8;
	pMsg[idx] = req->NwkNameSize; idx++;
	FLib_MemCpy(pMsg + idx, req->NwkName, req->NwkNameSize); idx += req->NwkNameSize;
	FLib_MemCpy(pMsg + idx, req->MLPrefix, 16); idx += 16;
	pMsg[idx] = req->MLprefixSizeInBits; idx++;
	FLib_MemCpy(pMsg + idx, req->MasterKey, sizeof(req->MasterKey)); idx += sizeof(req->MasterKey);

	/* Send the request */
	FSCI_transmitPayload(arg, 0xCE, 0x16, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_NwkScanRequest(THR_NwkScanRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Start a network scan request

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_NwkScanRequest(THR_NwkScanRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x1A, req, sizeof(THR_NwkScanRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_CreateNwkRequest(THR_CreateNwkRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Create thread network. Should be run after setting the configuration parameters.

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_CreateNwkRequest(THR_CreateNwkRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x1B, req, sizeof(THR_CreateNwkRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_JoinRequest(THR_JoinRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Join the network. Depending on the attributes configuration it can perform the following actions:
                - join as an end node using commissioning (devIsCommissioned attribute is FALSE).
                - direct attach to a thread network (devIsCommissioned attribute is TRUE).

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_JoinRequest(THR_JoinRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x1C, req, sizeof(THR_JoinRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_FactoryResetRequest(void *arg, uint8_t fsciInterface)
\brief	Reset the device to factory default

\return	memStatus_t			MEM_SUCCESS_c
***************************************************************************************************/
memStatus_t THR_FactoryResetRequest(void *arg, uint8_t fsciInterface)
{
	FSCI_transmitPayload(arg, 0xCE, 0x1F, NULL, 0, fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_CpuResetRequest(THR_CpuResetRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Request to reset the device.

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_CpuResetRequest(THR_CpuResetRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x21, req, sizeof(THR_CpuResetRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_DisconnectRequest(THR_DisconnectRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Network disconnect

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_DisconnectRequest(THR_DisconnectRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x1D, req, sizeof(THR_DisconnectRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_AttachRequest(THR_AttachRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Attaches to Thread network using the configured attributes: channel, panId, network key

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_AttachRequest(THR_AttachRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x80, req, sizeof(THR_AttachRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_PromoteAsRouterRequest(THR_PromoteAsRouterRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Promotes device as router for the given reason

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_PromoteAsRouterRequest(THR_PromoteAsRouterRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x94, req, sizeof(THR_PromoteAsRouterRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_MgmtDiagnosticGetRequest(THR_MgmtDiagnosticGetRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Management Network Diagnostic Get request

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_MgmtDiagnosticGetRequest(THR_MgmtDiagnosticGetRequest_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint8_t);  // InstanceId
	msgLen += 16;  // DestIpAddr
	msgLen += sizeof(uint8_t);  // NumberOfTlvIds
	msgLen += req->NumberOfTlvIds * sizeof(uint8_t);  // TlvIds

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	pMsg[idx] = req->InstanceId; idx++;
	FLib_MemCpy(pMsg + idx, req->DestIpAddr, 16); idx += 16;
	pMsg[idx] = req->NumberOfTlvIds; idx++;
	FLib_MemCpy(pMsg + idx, req->TlvIds, req->NumberOfTlvIds * sizeof(uint8_t)); idx += req->NumberOfTlvIds * sizeof(uint8_t);

	/* Send the request */
	FSCI_transmitPayload(arg, 0xCE, 0x61, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_DiagTestGetRequest(THR_DiagTestGetRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Large Network Diagnostic Get request

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_DiagTestGetRequest(THR_DiagTestGetRequest_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint8_t);  // InstanceId
	msgLen += 16;  // DestIpAddr
	msgLen += sizeof(uint8_t);  // PayloadSize
	msgLen += sizeof(uint8_t);  // TlvId

	switch (req->TlvId)
	{
		case THR_DiagTestGetRequest_TlvId_ColdFactoryReset:
			break;

		case THR_DiagTestGetRequest_TlvId_WarmCPUReset:
			break;

		case THR_DiagTestGetRequest_TlvId_Data:
			msgLen += sizeof(uint8_t);  // SequenceNumber
			msgLen += req->PayloadSize;  // Payload
			break;

		case THR_DiagTestGetRequest_TlvId_Results:
			break;
	}	// AttributeValue

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	pMsg[idx] = req->InstanceId; idx++;
	FLib_MemCpy(pMsg + idx, req->DestIpAddr, 16); idx += 16;
	pMsg[idx] = req->PayloadSize; idx++;
	pMsg[idx] = req->TlvId; idx++;

	switch (req->TlvId)
	{
		case THR_DiagTestGetRequest_TlvId_ColdFactoryReset:
			break;

		case THR_DiagTestGetRequest_TlvId_WarmCPUReset:
			break;

		case THR_DiagTestGetRequest_TlvId_Data:
			pMsg[idx] = req->AttributeValue.Data.SequenceNumber; idx++;
			FLib_MemCpy(pMsg + idx, req->AttributeValue.Data.Payload, req->PayloadSize); idx += req->PayloadSize;
			break;

		case THR_DiagTestGetRequest_TlvId_Results:
			break;
	}

	/* Send the request */
	FSCI_transmitPayload(arg, 0xCE, 0x67, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_MgmtDiagnosticResetRequest(THR_MgmtDiagnosticResetRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Management Network Diagnostic Reset request

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_MgmtDiagnosticResetRequest(THR_MgmtDiagnosticResetRequest_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint8_t);  // InstanceId
	msgLen += 16;  // DestIpAddr
	msgLen += sizeof(uint8_t);  // NumberOfTlvIds
	msgLen += req->NumberOfTlvIds * sizeof(uint8_t);  // TlvIds

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	pMsg[idx] = req->InstanceId; idx++;
	FLib_MemCpy(pMsg + idx, req->DestIpAddr, 16); idx += 16;
	pMsg[idx] = req->NumberOfTlvIds; idx++;
	FLib_MemCpy(pMsg + idx, req->TlvIds, req->NumberOfTlvIds * sizeof(uint8_t)); idx += req->NumberOfTlvIds * sizeof(uint8_t);

	/* Send the request */
	FSCI_transmitPayload(arg, 0xCE, 0x62, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_MgmtReadMemoryRequest(THR_MgmtReadMemoryRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Reads 'Length' octets from the given memory address.

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_MgmtReadMemoryRequest(THR_MgmtReadMemoryRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x65, req, sizeof(THR_MgmtReadMemoryRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_MgmtWriteMemoryRequest(THR_MgmtWriteMemoryRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Writes 'Length' octets to the given memory address.

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_MgmtWriteMemoryRequest(THR_MgmtWriteMemoryRequest_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint32_t);  // Address
	msgLen += sizeof(uint8_t);  // Length
	msgLen += req->Length;  // Value

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	FLib_MemCpy(pMsg + idx, &(req->Address), sizeof(req->Address)); idx += sizeof(req->Address);
	pMsg[idx] = req->Length; idx++;
	FLib_MemCpy(pMsg + idx, req->Value, req->Length); idx += req->Length;

	/* Send the request */
	FSCI_transmitPayload(arg, 0xCE, 0x66, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_NwkDiscoveryRequest(THR_NwkDiscoveryRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	This function starts the Thread Discovery Procedure.

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_NwkDiscoveryRequest(THR_NwkDiscoveryRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0xC0, req, sizeof(THR_NwkDiscoveryRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_NwkDiscoveryStopRequest(THR_NwkDiscoveryStopRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	This function stops the Thread Discovery Procedure.

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_NwkDiscoveryStopRequest(THR_NwkDiscoveryStopRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0xC1, req, sizeof(THR_NwkDiscoveryStopRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_SearchNwkWithAnounceRequest(THR_SearchNwkWithAnounceRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	This function starts the Thread Discovery Procedure.

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_SearchNwkWithAnounceRequest(THR_SearchNwkWithAnounceRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0xC2, req, sizeof(THR_SearchNwkWithAnounceRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_GetDeviceInfoRequest(THR_GetDeviceInfoRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	This function returns extended device info.

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_GetDeviceInfoRequest(THR_GetDeviceInfoRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0xC3, req, sizeof(THR_GetDeviceInfoRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

#endif  /* THREADMGMT_ENABLE */

#if THREADUTILS_ENABLE
/*!*************************************************************************************************
\fn		memStatus_t THR_ChildUpdateToParentRequest(THR_ChildUpdateToParentRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Notifies parent of updated child parameters (sends Child Update request to the parent node).

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_ChildUpdateToParentRequest(THR_ChildUpdateToParentRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x2F, req, sizeof(THR_ChildUpdateToParentRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_SetParentPriorityRequest(THR_SetParentPriorityRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Set parent priority

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_SetParentPriorityRequest(THR_SetParentPriorityRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0xC4, req, sizeof(THR_SetParentPriorityRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_SetManualSlaacIIDRequest(THR_SetManualSlaacIIDRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Sets the IID 

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_SetManualSlaacIIDRequest(THR_SetManualSlaacIIDRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x75, req, sizeof(THR_SetManualSlaacIIDRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_SendProactiveAddrNotifRequest(THR_SendProactiveAddrNotifRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Send ADDR_NTF.ntf - Proactive Address Notification

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_SendProactiveAddrNotifRequest(THR_SendProactiveAddrNotifRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x76, req, sizeof(THR_SendProactiveAddrNotifRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_SetThresholdRequest(THR_SetThresholdRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Set thread threshold (1 byte)

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_SetThresholdRequest(THR_SetThresholdRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x20, req, sizeof(THR_SetThresholdRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_SetThreshold32Request(THR_SetThreshold32Request_t *req, void *arg, uint8_t fsciInterface)
\brief	Set thread threshold (4 bytes)

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_SetThreshold32Request(THR_SetThreshold32Request_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0xE0, req, sizeof(THR_SetThreshold32Request_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_SetNwkIdTimeoutRequest(THR_SetNwkIdTimeoutRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Set network ID timeout in seconds

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_SetNwkIdTimeoutRequest(THR_SetNwkIdTimeoutRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x2E, req, sizeof(THR_SetNwkIdTimeoutRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_SetRouterPromoteMaxJitterRequest(THR_SetRouterPromoteMaxJitterRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Set router promotion max jitter in seconds

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_SetRouterPromoteMaxJitterRequest(THR_SetRouterPromoteMaxJitterRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x81, req, sizeof(THR_SetRouterPromoteMaxJitterRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_GetRoutingTableRequest(THR_GetRoutingTableRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Get the routing table

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_GetRoutingTableRequest(THR_GetRoutingTableRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x25, req, sizeof(THR_GetRoutingTableRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_GetNeighborTableRequest(THR_GetNeighborTableRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Get the neighbor table

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_GetNeighborTableRequest(THR_GetNeighborTableRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x24, req, sizeof(THR_GetNeighborTableRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_GetNeighborInfoRequest(THR_GetNeighborInfoRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Get detailed info about a neighbor

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_GetNeighborInfoRequest(THR_GetNeighborInfoRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x10, req, sizeof(THR_GetNeighborInfoRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_GetChildrenTableRequest(THR_GetChildrenTableRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Get the children table

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_GetChildrenTableRequest(THR_GetChildrenTableRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x23, req, sizeof(THR_GetChildrenTableRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_GetAttrRequest(THR_GetAttrRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Get Attribute

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_GetAttrRequest(THR_GetAttrRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x17, req, sizeof(THR_GetAttrRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_SetAttrRequest(THR_SetAttrRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Set attribute request 

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_SetAttrRequest(THR_SetAttrRequest_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint8_t);  // InstanceId
	msgLen += sizeof(uint8_t);  // AttributeId
	msgLen += sizeof(uint8_t);  // Index

	switch (req->AttributeId)
	{
		case THR_SetAttrRequest_AttributeId_RandomExtendedAddr:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += sizeof(uint64_t);  // Value
			break;

		case THR_SetAttrRequest_AttributeId_ScanChannelMask:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += sizeof(uint32_t);  // Value
			break;

		case THR_SetAttrRequest_AttributeId_ScanDuration:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += sizeof(uint8_t);  // Value
			break;

		case THR_SetAttrRequest_AttributeId_Channel:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += sizeof(uint8_t);  // Value
			break;

		case THR_SetAttrRequest_AttributeId_ShortPanId:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += sizeof(uint16_t);  // Value
			break;

		case THR_SetAttrRequest_AttributeId_ExtendedPanId:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += req->AttributeValue.ExtendedPanId.AttrSize;  // Value
			break;

		case THR_SetAttrRequest_AttributeId_RxOnIdle:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += sizeof(bool_t);  // Value
			break;

		case THR_SetAttrRequest_AttributeId_SedPollInterval:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += sizeof(uint32_t);  // Value
			break;

		case THR_SetAttrRequest_AttributeId_UniqueExtendedAddress:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += sizeof(bool_t);  // Value
			break;

		case THR_SetAttrRequest_AttributeId_VendorName:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += req->AttributeValue.VendorName.AttrSize;  // Value
			break;

		case THR_SetAttrRequest_AttributeId_ModelName:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += req->AttributeValue.ModelName.AttrSize;  // Value
			break;

		case THR_SetAttrRequest_AttributeId_SwVersion:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += req->AttributeValue.SwVersion.AttrSize;  // Value
			break;

		case THR_SetAttrRequest_AttributeId_NwkCapabilities:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += 1;  // IsFullThreadDevice|IsPollingEndDevice|CanBecomeActiveRouter|CanCreateNewNetwork  // Value
			break;

		case THR_SetAttrRequest_AttributeId_NwkName:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += req->AttributeValue.NwkName.AttrSize;  // Value
			break;

		case THR_SetAttrRequest_AttributeId_IsDevConnected:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += sizeof(bool_t);  // Value
			break;

		case THR_SetAttrRequest_AttributeId_IsDevCommissioned:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += sizeof(bool_t);  // Value
			break;

		case THR_SetAttrRequest_AttributeId_PartitionId:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += sizeof(uint32_t);  // Value
			break;

		case THR_SetAttrRequest_AttributeId_DeviceRole:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += sizeof(uint8_t);  // Value
			break;

		case THR_SetAttrRequest_AttributeId_Security_NwkMasterKey:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += req->AttributeValue.Security_NwkMasterKey.AttrSize;  // Value
			break;

		case THR_SetAttrRequest_AttributeId_Security_NwkKeySeq:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += sizeof(uint32_t);  // Value
			break;

		case THR_SetAttrRequest_AttributeId_Security_PSKc:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += req->AttributeValue.Security_PSKc.AttrSize;  // Value
			break;

		case THR_SetAttrRequest_AttributeId_Security_PSKd:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += req->AttributeValue.Security_PSKd.AttrSize;  // Value
			break;

		case THR_SetAttrRequest_AttributeId_VendorData:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += req->AttributeValue.VendorData.AttrSize;  // Value
			break;

		case THR_SetAttrRequest_AttributeId_MLPrefix:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += 16;  // PrefixData
			msgLen += sizeof(uint8_t);  // PrefixLength
			break;

		case THR_SetAttrRequest_AttributeId_Security_KeyRotationInterval:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += sizeof(uint32_t);  // Value
			break;

		case THR_SetAttrRequest_AttributeId_ChildSEDTimeout:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += sizeof(uint32_t);  // Value
			break;

		case THR_SetAttrRequest_AttributeId_EndDevice_ChildEDReqFullNwkData:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += sizeof(bool_t);  // Value
			break;

		case THR_SetAttrRequest_AttributeId_EndDevice_IsFastPollEnabled:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += sizeof(bool_t);  // Value
			break;

		case THR_SetAttrRequest_AttributeId_SleepyEndDevice_FastPollInterval:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += sizeof(uint32_t);  // Value
			break;

		case THR_SetAttrRequest_AttributeId_JoinLqiThreshold:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += sizeof(uint8_t);  // Value
			break;

		case THR_SetAttrRequest_AttributeId_ProvisioningURL:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += req->AttributeValue.ProvisioningURL.AttrSize;  // Value
			break;

		case THR_SetAttrRequest_AttributeId_SelectBestChannelEDThreshold:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += sizeof(uint8_t);  // Value
			break;

		case THR_SetAttrRequest_AttributeId_BorderRouter_BrPrefixEntry:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += sizeof(uint8_t);  // prefixLength
			msgLen += 16;  // PrefixValue
			msgLen += sizeof(uint8_t);  // PrefixFlagsReserved
			msgLen += 1;  // P_preference|P_Slaac_preferred|P_Slaac_valid|P_Dhcp|P_Configure|P_Default|Reserved_Bit0  // PrefixFlags
			msgLen += sizeof(uint32_t);  // prefixLifetime
			msgLen += sizeof(bool_t);  // prefixAdvertised
			msgLen += 1;  // R_preference  // ExternalRouteFlags
			msgLen += sizeof(uint32_t);  // ExternalRouteLifetime
			msgLen += sizeof(bool_t);  // ExternalRouteAdvertised
			break;

		case THR_SetAttrRequest_AttributeId_Security_KeySwitchGuardTime:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += sizeof(uint32_t);  // Value
			break;

		case THR_SetAttrRequest_AttributeId_ParentHoldTime:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += sizeof(uint16_t);  // Value
			break;

		case THR_SetAttrRequest_AttributeId_Security_Policy:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += sizeof(uint8_t);  // Value
			break;

		case THR_SetAttrRequest_AttributeId_NVM_RestoreAutoStart:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += sizeof(bool_t);  // Value
			break;

		case THR_SetAttrRequest_AttributeId_NVM_Restore:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += sizeof(bool_t);  // Value
			break;

		case THR_SetAttrRequest_AttributeId_SlaacPolicy:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += sizeof(uint8_t);  // Value
			break;

		case THR_SetAttrRequest_AttributeId_IeeeExtendedAddr:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += sizeof(uint64_t);  // Value
			break;

		case THR_SetAttrRequest_AttributeId_LeaderWeight:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += sizeof(uint8_t);  // Value
			break;

		case THR_SetAttrRequest_AttributeId_DoNotGeneratePartitionId:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += sizeof(uint32_t);  // Value
			break;

		case THR_SetAttrRequest_AttributeId_BorderRouter_BrGlobalOnMeshPrefix:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += 16;  // PrefixData
			msgLen += sizeof(uint8_t);  // PrefixLength
			break;

		case THR_SetAttrRequest_AttributeId_BorderRouter_BrDefaultRouteOnMeshPrefix:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += sizeof(bool_t);  // Value
			break;

		case THR_SetAttrRequest_AttributeId_BorderRouter_BrExternalIfPrefix:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += 16;  // PrefixData
			msgLen += sizeof(uint8_t);  // PrefixLength
			break;

		case THR_SetAttrRequest_AttributeId_ActiveTimestamp:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += sizeof(req->AttributeValue.ActiveTimestamp.ActiveSeconds);  // ActiveSeconds
			msgLen += sizeof(uint16_t);  // ActiveTicks
			break;

		case THR_SetAttrRequest_AttributeId_JoinerPort:
			msgLen += sizeof(uint8_t);  // AttrSize
			msgLen += sizeof(uint16_t);  // Value
			break;
	}	// AttributeValue

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	pMsg[idx] = req->InstanceId; idx++;
	pMsg[idx] = req->AttributeId; idx++;
	pMsg[idx] = req->Index; idx++;

	switch (req->AttributeId)
	{
		case THR_SetAttrRequest_AttributeId_RandomExtendedAddr:
			pMsg[idx] = req->AttributeValue.RandomExtendedAddr.AttrSize; idx++;
			FLib_MemCpy(pMsg + idx, &(req->AttributeValue.RandomExtendedAddr.Value), sizeof(req->AttributeValue.RandomExtendedAddr.Value)); idx += sizeof(req->AttributeValue.RandomExtendedAddr.Value);
			break;

		case THR_SetAttrRequest_AttributeId_ScanChannelMask:
			pMsg[idx] = req->AttributeValue.ScanChannelMask.AttrSize; idx++;
			FLib_MemCpy(pMsg + idx, &(req->AttributeValue.ScanChannelMask.Value), sizeof(req->AttributeValue.ScanChannelMask.Value)); idx += sizeof(req->AttributeValue.ScanChannelMask.Value);
			break;

		case THR_SetAttrRequest_AttributeId_ScanDuration:
			pMsg[idx] = req->AttributeValue.ScanDuration.AttrSize; idx++;
			pMsg[idx] = req->AttributeValue.ScanDuration.Value; idx++;
			break;

		case THR_SetAttrRequest_AttributeId_Channel:
			pMsg[idx] = req->AttributeValue.Channel.AttrSize; idx++;
			pMsg[idx] = req->AttributeValue.Channel.Value; idx++;
			break;

		case THR_SetAttrRequest_AttributeId_ShortPanId:
			pMsg[idx] = req->AttributeValue.ShortPanId.AttrSize; idx++;
			FLib_MemCpy(pMsg + idx, &(req->AttributeValue.ShortPanId.Value), sizeof(req->AttributeValue.ShortPanId.Value)); idx += sizeof(req->AttributeValue.ShortPanId.Value);
			break;

		case THR_SetAttrRequest_AttributeId_ExtendedPanId:
			pMsg[idx] = req->AttributeValue.ExtendedPanId.AttrSize; idx++;
			FLib_MemCpy(pMsg + idx, req->AttributeValue.ExtendedPanId.Value, req->AttributeValue.ExtendedPanId.AttrSize); idx += req->AttributeValue.ExtendedPanId.AttrSize;
			break;

		case THR_SetAttrRequest_AttributeId_RxOnIdle:
			pMsg[idx] = req->AttributeValue.RxOnIdle.AttrSize; idx++;
			pMsg[idx] = req->AttributeValue.RxOnIdle.Value; idx++;
			break;

		case THR_SetAttrRequest_AttributeId_SedPollInterval:
			pMsg[idx] = req->AttributeValue.SedPollInterval.AttrSize; idx++;
			FLib_MemCpy(pMsg + idx, &(req->AttributeValue.SedPollInterval.Value), sizeof(req->AttributeValue.SedPollInterval.Value)); idx += sizeof(req->AttributeValue.SedPollInterval.Value);
			break;

		case THR_SetAttrRequest_AttributeId_UniqueExtendedAddress:
			pMsg[idx] = req->AttributeValue.UniqueExtendedAddress.AttrSize; idx++;
			pMsg[idx] = req->AttributeValue.UniqueExtendedAddress.Value; idx++;
			break;

		case THR_SetAttrRequest_AttributeId_VendorName:
			pMsg[idx] = req->AttributeValue.VendorName.AttrSize; idx++;
			FLib_MemCpy(pMsg + idx, req->AttributeValue.VendorName.Value, req->AttributeValue.VendorName.AttrSize); idx += req->AttributeValue.VendorName.AttrSize;
			break;

		case THR_SetAttrRequest_AttributeId_ModelName:
			pMsg[idx] = req->AttributeValue.ModelName.AttrSize; idx++;
			FLib_MemCpy(pMsg + idx, req->AttributeValue.ModelName.Value, req->AttributeValue.ModelName.AttrSize); idx += req->AttributeValue.ModelName.AttrSize;
			break;

		case THR_SetAttrRequest_AttributeId_SwVersion:
			pMsg[idx] = req->AttributeValue.SwVersion.AttrSize; idx++;
			FLib_MemCpy(pMsg + idx, req->AttributeValue.SwVersion.Value, req->AttributeValue.SwVersion.AttrSize); idx += req->AttributeValue.SwVersion.AttrSize;
			break;

		case THR_SetAttrRequest_AttributeId_NwkCapabilities:
			pMsg[idx] = req->AttributeValue.NwkCapabilities.AttrSize; idx++;
			pMsg[idx] = req->AttributeValue.NwkCapabilities.Value; idx++;
			break;

		case THR_SetAttrRequest_AttributeId_NwkName:
			pMsg[idx] = req->AttributeValue.NwkName.AttrSize; idx++;
			FLib_MemCpy(pMsg + idx, req->AttributeValue.NwkName.Value, req->AttributeValue.NwkName.AttrSize); idx += req->AttributeValue.NwkName.AttrSize;
			break;

		case THR_SetAttrRequest_AttributeId_IsDevConnected:
			pMsg[idx] = req->AttributeValue.IsDevConnected.AttrSize; idx++;
			pMsg[idx] = req->AttributeValue.IsDevConnected.Value; idx++;
			break;

		case THR_SetAttrRequest_AttributeId_IsDevCommissioned:
			pMsg[idx] = req->AttributeValue.IsDevCommissioned.AttrSize; idx++;
			pMsg[idx] = req->AttributeValue.IsDevCommissioned.Value; idx++;
			break;

		case THR_SetAttrRequest_AttributeId_PartitionId:
			pMsg[idx] = req->AttributeValue.PartitionId.AttrSize; idx++;
			FLib_MemCpy(pMsg + idx, &(req->AttributeValue.PartitionId.Value), sizeof(req->AttributeValue.PartitionId.Value)); idx += sizeof(req->AttributeValue.PartitionId.Value);
			break;

		case THR_SetAttrRequest_AttributeId_DeviceRole:
			pMsg[idx] = req->AttributeValue.DeviceRole.AttrSize; idx++;
			pMsg[idx] = req->AttributeValue.DeviceRole.Value; idx++;
			break;

		case THR_SetAttrRequest_AttributeId_Security_NwkMasterKey:
			pMsg[idx] = req->AttributeValue.Security_NwkMasterKey.AttrSize; idx++;
			FLib_MemCpy(pMsg + idx, req->AttributeValue.Security_NwkMasterKey.Value, req->AttributeValue.Security_NwkMasterKey.AttrSize); idx += req->AttributeValue.Security_NwkMasterKey.AttrSize;
			break;

		case THR_SetAttrRequest_AttributeId_Security_NwkKeySeq:
			pMsg[idx] = req->AttributeValue.Security_NwkKeySeq.AttrSize; idx++;
			FLib_MemCpy(pMsg + idx, &(req->AttributeValue.Security_NwkKeySeq.Value), sizeof(req->AttributeValue.Security_NwkKeySeq.Value)); idx += sizeof(req->AttributeValue.Security_NwkKeySeq.Value);
			break;

		case THR_SetAttrRequest_AttributeId_Security_PSKc:
			pMsg[idx] = req->AttributeValue.Security_PSKc.AttrSize; idx++;
			FLib_MemCpy(pMsg + idx, req->AttributeValue.Security_PSKc.Value, req->AttributeValue.Security_PSKc.AttrSize); idx += req->AttributeValue.Security_PSKc.AttrSize;
			break;

		case THR_SetAttrRequest_AttributeId_Security_PSKd:
			pMsg[idx] = req->AttributeValue.Security_PSKd.AttrSize; idx++;
			FLib_MemCpy(pMsg + idx, req->AttributeValue.Security_PSKd.Value, req->AttributeValue.Security_PSKd.AttrSize); idx += req->AttributeValue.Security_PSKd.AttrSize;
			break;

		case THR_SetAttrRequest_AttributeId_VendorData:
			pMsg[idx] = req->AttributeValue.VendorData.AttrSize; idx++;
			FLib_MemCpy(pMsg + idx, req->AttributeValue.VendorData.Value, req->AttributeValue.VendorData.AttrSize); idx += req->AttributeValue.VendorData.AttrSize;
			break;

		case THR_SetAttrRequest_AttributeId_MLPrefix:
			pMsg[idx] = req->AttributeValue.MLPrefix.AttrSize; idx++;
			FLib_MemCpy(pMsg + idx, req->AttributeValue.MLPrefix.Value.PrefixData, 16); idx += 16;
			pMsg[idx] = req->AttributeValue.MLPrefix.Value.PrefixLength; idx++;
			break;

		case THR_SetAttrRequest_AttributeId_Security_KeyRotationInterval:
			pMsg[idx] = req->AttributeValue.Security_KeyRotationInterval.AttrSize; idx++;
			FLib_MemCpy(pMsg + idx, &(req->AttributeValue.Security_KeyRotationInterval.Value), sizeof(req->AttributeValue.Security_KeyRotationInterval.Value)); idx += sizeof(req->AttributeValue.Security_KeyRotationInterval.Value);
			break;

		case THR_SetAttrRequest_AttributeId_ChildSEDTimeout:
			pMsg[idx] = req->AttributeValue.ChildSEDTimeout.AttrSize; idx++;
			FLib_MemCpy(pMsg + idx, &(req->AttributeValue.ChildSEDTimeout.Value), sizeof(req->AttributeValue.ChildSEDTimeout.Value)); idx += sizeof(req->AttributeValue.ChildSEDTimeout.Value);
			break;

		case THR_SetAttrRequest_AttributeId_EndDevice_ChildEDReqFullNwkData:
			pMsg[idx] = req->AttributeValue.EndDevice_ChildEDReqFullNwkData.AttrSize; idx++;
			pMsg[idx] = req->AttributeValue.EndDevice_ChildEDReqFullNwkData.Value; idx++;
			break;

		case THR_SetAttrRequest_AttributeId_EndDevice_IsFastPollEnabled:
			pMsg[idx] = req->AttributeValue.EndDevice_IsFastPollEnabled.AttrSize; idx++;
			pMsg[idx] = req->AttributeValue.EndDevice_IsFastPollEnabled.Value; idx++;
			break;

		case THR_SetAttrRequest_AttributeId_SleepyEndDevice_FastPollInterval:
			pMsg[idx] = req->AttributeValue.SleepyEndDevice_FastPollInterval.AttrSize; idx++;
			FLib_MemCpy(pMsg + idx, &(req->AttributeValue.SleepyEndDevice_FastPollInterval.Value), sizeof(req->AttributeValue.SleepyEndDevice_FastPollInterval.Value)); idx += sizeof(req->AttributeValue.SleepyEndDevice_FastPollInterval.Value);
			break;

		case THR_SetAttrRequest_AttributeId_JoinLqiThreshold:
			pMsg[idx] = req->AttributeValue.JoinLqiThreshold.AttrSize; idx++;
			pMsg[idx] = req->AttributeValue.JoinLqiThreshold.Value; idx++;
			break;

		case THR_SetAttrRequest_AttributeId_ProvisioningURL:
			pMsg[idx] = req->AttributeValue.ProvisioningURL.AttrSize; idx++;
			FLib_MemCpy(pMsg + idx, req->AttributeValue.ProvisioningURL.Value, req->AttributeValue.ProvisioningURL.AttrSize); idx += req->AttributeValue.ProvisioningURL.AttrSize;
			break;

		case THR_SetAttrRequest_AttributeId_SelectBestChannelEDThreshold:
			pMsg[idx] = req->AttributeValue.Channel.AttrSize; idx++;
			pMsg[idx] = req->AttributeValue.Channel.Value; idx++;
			break;

		case THR_SetAttrRequest_AttributeId_BorderRouter_BrPrefixEntry:
			pMsg[idx] = req->AttributeValue.BorderRouter_BrPrefixEntry.AttrSize; idx++;
			pMsg[idx] = req->AttributeValue.BorderRouter_BrPrefixEntry.Value.prefixLength; idx++;
			FLib_MemCpy(pMsg + idx, req->AttributeValue.BorderRouter_BrPrefixEntry.Value.PrefixValue, 16); idx += 16;
			pMsg[idx] = req->AttributeValue.BorderRouter_BrPrefixEntry.Value.PrefixFlagsReserved; idx++;
			pMsg[idx] = req->AttributeValue.BorderRouter_BrPrefixEntry.Value.PrefixFlags; idx++;
			FLib_MemCpy(pMsg + idx, &(req->AttributeValue.BorderRouter_BrPrefixEntry.Value.prefixLifetime), sizeof(req->AttributeValue.BorderRouter_BrPrefixEntry.Value.prefixLifetime)); idx += sizeof(req->AttributeValue.BorderRouter_BrPrefixEntry.Value.prefixLifetime);
			pMsg[idx] = req->AttributeValue.BorderRouter_BrPrefixEntry.Value.prefixAdvertised; idx++;
			pMsg[idx] = req->AttributeValue.BorderRouter_BrPrefixEntry.Value.ExternalRouteFlags; idx++;
			FLib_MemCpy(pMsg + idx, &(req->AttributeValue.BorderRouter_BrPrefixEntry.Value.ExternalRouteLifetime), sizeof(req->AttributeValue.BorderRouter_BrPrefixEntry.Value.ExternalRouteLifetime)); idx += sizeof(req->AttributeValue.BorderRouter_BrPrefixEntry.Value.ExternalRouteLifetime);
			pMsg[idx] = req->AttributeValue.BorderRouter_BrPrefixEntry.Value.ExternalRouteAdvertised; idx++;
			break;

		case THR_SetAttrRequest_AttributeId_Security_KeySwitchGuardTime:
			pMsg[idx] = req->AttributeValue.Security_KeySwitchGuardTime.AttrSize; idx++;
			FLib_MemCpy(pMsg + idx, &(req->AttributeValue.Security_KeySwitchGuardTime.Value), sizeof(req->AttributeValue.Security_KeySwitchGuardTime.Value)); idx += sizeof(req->AttributeValue.Security_KeySwitchGuardTime.Value);
			break;

		case THR_SetAttrRequest_AttributeId_ParentHoldTime:
			pMsg[idx] = req->AttributeValue.ParentHoldTime.AttrSize; idx++;
			FLib_MemCpy(pMsg + idx, &(req->AttributeValue.ParentHoldTime.Value), sizeof(req->AttributeValue.ParentHoldTime.Value)); idx += sizeof(req->AttributeValue.ParentHoldTime.Value);
			break;

		case THR_SetAttrRequest_AttributeId_Security_Policy:
			pMsg[idx] = req->AttributeValue.Security_Policy.AttrSize; idx++;
			pMsg[idx] = req->AttributeValue.Security_Policy.Value; idx++;
			break;

		case THR_SetAttrRequest_AttributeId_NVM_RestoreAutoStart:
			pMsg[idx] = req->AttributeValue.NVM_RestoreAutoStart.AttrSize; idx++;
			pMsg[idx] = req->AttributeValue.NVM_RestoreAutoStart.Value; idx++;
			break;

		case THR_SetAttrRequest_AttributeId_NVM_Restore:
			pMsg[idx] = req->AttributeValue.NVM_Restore.AttrSize; idx++;
			pMsg[idx] = req->AttributeValue.NVM_Restore.Value; idx++;
			break;

		case THR_SetAttrRequest_AttributeId_SlaacPolicy:
			pMsg[idx] = req->AttributeValue.SlaacPolicy.AttrSize; idx++;
			pMsg[idx] = req->AttributeValue.SlaacPolicy.Value; idx++;
			break;

		case THR_SetAttrRequest_AttributeId_IeeeExtendedAddr:
			pMsg[idx] = req->AttributeValue.IeeeExtendedAddr.AttrSize; idx++;
			FLib_MemCpy(pMsg + idx, &(req->AttributeValue.IeeeExtendedAddr.Value), sizeof(req->AttributeValue.IeeeExtendedAddr.Value)); idx += sizeof(req->AttributeValue.IeeeExtendedAddr.Value);
			break;

		case THR_SetAttrRequest_AttributeId_LeaderWeight:
			pMsg[idx] = req->AttributeValue.LeaderWeight.AttrSize; idx++;
			pMsg[idx] = req->AttributeValue.LeaderWeight.Value; idx++;
			break;

		case THR_SetAttrRequest_AttributeId_DoNotGeneratePartitionId:
			pMsg[idx] = req->AttributeValue.PartitionId.AttrSize; idx++;
			FLib_MemCpy(pMsg + idx, &(req->AttributeValue.PartitionId.Value), sizeof(req->AttributeValue.PartitionId.Value)); idx += sizeof(req->AttributeValue.PartitionId.Value);
			break;

		case THR_SetAttrRequest_AttributeId_BorderRouter_BrGlobalOnMeshPrefix:
			pMsg[idx] = req->AttributeValue.BorderRouter_BrGlobalOnMeshPrefix.AttrSize; idx++;
			FLib_MemCpy(pMsg + idx, req->AttributeValue.BorderRouter_BrGlobalOnMeshPrefix.Value.PrefixData, 16); idx += 16;
			pMsg[idx] = req->AttributeValue.BorderRouter_BrGlobalOnMeshPrefix.Value.PrefixLength; idx++;
			break;

		case THR_SetAttrRequest_AttributeId_BorderRouter_BrDefaultRouteOnMeshPrefix:
			pMsg[idx] = req->AttributeValue.BorderRouter_BrDefaultRouteOnMeshPrefix.AttrSize; idx++;
			pMsg[idx] = req->AttributeValue.BorderRouter_BrDefaultRouteOnMeshPrefix.Value; idx++;
			break;

		case THR_SetAttrRequest_AttributeId_BorderRouter_BrExternalIfPrefix:
			pMsg[idx] = req->AttributeValue.BorderRouter_BrExternalIfPrefix.AttrSize; idx++;
			FLib_MemCpy(pMsg + idx, req->AttributeValue.BorderRouter_BrExternalIfPrefix.Value.PrefixData, 16); idx += 16;
			pMsg[idx] = req->AttributeValue.BorderRouter_BrExternalIfPrefix.Value.PrefixLength; idx++;
			break;

		case THR_SetAttrRequest_AttributeId_ActiveTimestamp:
			pMsg[idx] = req->AttributeValue.ActiveTimestamp.AttrSize; idx++;
			FLib_MemCpy(pMsg + idx, req->AttributeValue.ActiveTimestamp.ActiveSeconds, sizeof(req->AttributeValue.ActiveTimestamp.ActiveSeconds)); idx += sizeof(req->AttributeValue.ActiveTimestamp.ActiveSeconds);
			FLib_MemCpy(pMsg + idx, &(req->AttributeValue.ActiveTimestamp.ActiveTicks), sizeof(req->AttributeValue.ActiveTimestamp.ActiveTicks)); idx += sizeof(req->AttributeValue.ActiveTimestamp.ActiveTicks);
			break;

		case THR_SetAttrRequest_AttributeId_JoinerPort:
			pMsg[idx] = req->AttributeValue.JoinerPort.AttrSize; idx++;
			FLib_MemCpy(pMsg + idx, &(req->AttributeValue.JoinerPort.Value), sizeof(req->AttributeValue.JoinerPort.Value)); idx += sizeof(req->AttributeValue.JoinerPort.Value);
			break;
	}

	/* Send the request */
	FSCI_transmitPayload(arg, 0xCE, 0x18, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_LeaderRemoveRouterIdRequest(THR_LeaderRemoveRouterIdRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Remove router id. Available only for Leader

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_LeaderRemoveRouterIdRequest(THR_LeaderRemoveRouterIdRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x26, req, sizeof(THR_LeaderRemoveRouterIdRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_GenerateAllKeysRequest(THR_GenerateAllKeysRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	This will generate 3 keys: previous key associated with thrKeySeqNum-1,
                the current key associated with thrKeySeqNum and the next key associated with
                thrKeySeqNum+1.

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_GenerateAllKeysRequest(THR_GenerateAllKeysRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x28, req, sizeof(THR_GenerateAllKeysRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_SwitchKeyRequest(THR_SwitchKeyRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Switch the active key sequence (which is specified by nwkKeySeq attribute)
                with the KeySequenceCounter. Note that the key for KeySequenceCounter should be already generated using THR_GenerateAllKeys.Request. 

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_SwitchKeyRequest(THR_SwitchKeyRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x27, req, sizeof(THR_SwitchKeyRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_BrPrefixAddEntryRequest(THR_BrPrefixAddEntryRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Add prefix entry to border router attribute table

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_BrPrefixAddEntryRequest(THR_BrPrefixAddEntryRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x29, req, sizeof(THR_BrPrefixAddEntryRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_BrPrefixGetTableRequest(THR_BrPrefixGetTableRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Get border router prefix table

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_BrPrefixGetTableRequest(THR_BrPrefixGetTableRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x2A, req, sizeof(THR_BrPrefixGetTableRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_BrPrefixRemoveEntryRequest(THR_BrPrefixRemoveEntryRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Remove a BrPrefix entry

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_BrPrefixRemoveEntryRequest(THR_BrPrefixRemoveEntryRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x2B, req, sizeof(THR_BrPrefixRemoveEntryRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_BrServiceRemoveEntryRequest(THR_BrServiceRemoveEntryRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Remove a Service entry

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_BrServiceRemoveEntryRequest(THR_BrServiceRemoveEntryRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x86, req, sizeof(THR_BrServiceRemoveEntryRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_BrServiceAddEntryRequest(THR_BrServiceAddEntryRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Add a Service entry

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_BrServiceAddEntryRequest(THR_BrServiceAddEntryRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x85, req, sizeof(THR_BrServiceAddEntryRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_BrPrefixSyncRequest(THR_BrPrefixSyncRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Used to syncronize Border Router prefix attributes with the global network data information. This is also propagating the network data over the air.

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_BrPrefixSyncRequest(THR_BrPrefixSyncRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x2C, req, sizeof(THR_BrPrefixSyncRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_BrPrefixRemoveAllRequest(THR_BrPrefixRemoveAllRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Used to remove all Border Router prefix attributes.

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_BrPrefixRemoveAllRequest(THR_BrPrefixRemoveAllRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x2D, req, sizeof(THR_BrPrefixRemoveAllRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_GetThreadIpAddrRequest(THR_GetThreadIpAddrRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Get thread Ip address

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_GetThreadIpAddrRequest(THR_GetThreadIpAddrRequest_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint8_t);  // InstanceId
	msgLen += sizeof(uint8_t);  // AddressType

	switch (req->AddressType)
	{
		case THR_GetThreadIpAddrRequest_AddressType_Link_Local_64:
			break;

		case THR_GetThreadIpAddrRequest_AddressType_MLEID:
			break;

		case THR_GetThreadIpAddrRequest_AddressType_RLOC:
			break;

		case THR_GetThreadIpAddrRequest_AddressType_Global:
			msgLen += sizeof(uint8_t);  // StartIndex
			msgLen += sizeof(uint8_t);  // NoOfElements
			break;

		case THR_GetThreadIpAddrRequest_AddressType_Anycast:
			msgLen += sizeof(uint8_t);  // StartIndex
			msgLen += sizeof(uint8_t);  // NoOfElements
			break;
	}	// Data

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	pMsg[idx] = req->InstanceId; idx++;
	pMsg[idx] = req->AddressType; idx++;

	switch (req->AddressType)
	{
		case THR_GetThreadIpAddrRequest_AddressType_Link_Local_64:
			break;

		case THR_GetThreadIpAddrRequest_AddressType_MLEID:
			break;

		case THR_GetThreadIpAddrRequest_AddressType_RLOC:
			break;

		case THR_GetThreadIpAddrRequest_AddressType_Global:
			pMsg[idx] = req->Data.Global.StartIndex; idx++;
			pMsg[idx] = req->Data.Global.NoOfElements; idx++;
			break;

		case THR_GetThreadIpAddrRequest_AddressType_Anycast:
			pMsg[idx] = req->Data.Anycast.StartIndex; idx++;
			pMsg[idx] = req->Data.Anycast.NoOfElements; idx++;
			break;
	}

	/* Send the request */
	FSCI_transmitPayload(arg, 0xCE, 0x19, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_GetParentRequest(THR_GetParentRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Get parent request

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t THR_GetParentRequest(THR_GetParentRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x1E, req, sizeof(THR_GetParentRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t NWKU_EidToRlocMapRequest(void *arg, uint8_t fsciInterface)
\brief	Displays the cache table that contains EID to RLOC mapping

\return	memStatus_t			MEM_SUCCESS_c
***************************************************************************************************/
memStatus_t NWKU_EidToRlocMapRequest(void *arg, uint8_t fsciInterface)
{
	FSCI_transmitPayload(arg, 0xCE, 0x6A, NULL, 0, fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t THR_IdentifyRequest(void *arg, uint8_t fsciInterface)
\brief	Send command to identify board by its LEDs.

\return	memStatus_t			MEM_SUCCESS_c
***************************************************************************************************/
memStatus_t THR_IdentifyRequest(void *arg, uint8_t fsciInterface)
{
	FSCI_transmitPayload(arg, 0xCE, 0x69, NULL, 0, fsciInterface);
	return MEM_SUCCESS_c;
}

#endif  /* THREADUTILS_ENABLE */

#if SOCKET_ENABLE
/*!*************************************************************************************************
\fn		memStatus_t SocketCreateRequest(SocketCreateRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Creates a new socket in the IP stack.

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t SocketCreateRequest(SocketCreateRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x00, req, sizeof(SocketCreateRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t SocketShutdownRequest(SocketShutdownRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Closes a connection for a socket and clears any data that it uses.

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t SocketShutdownRequest(SocketShutdownRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x01, req, sizeof(SocketShutdownRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t SocketBindRequest(SocketBindRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Sets the local information for a socket.

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t SocketBindRequest(SocketBindRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x02, req, sizeof(SocketBindRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t SocketSendRequest(SocketSendRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Sends data using a connected socket (usually used for sockets over TCP).

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t SocketSendRequest(SocketSendRequest_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint8_t);  // SocketIndex
	msgLen += sizeof(uint8_t);  // Flags
	msgLen += sizeof(uint16_t);  // Size
	msgLen += req->Size;  // Data

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	pMsg[idx] = req->SocketIndex; idx++;
	pMsg[idx] = req->Flags; idx++;
	FLib_MemCpy(pMsg + idx, &(req->Size), sizeof(req->Size)); idx += sizeof(req->Size);
	FLib_MemCpy(pMsg + idx, req->Data, req->Size); idx += req->Size;

	/* Send the request */
	FSCI_transmitPayload(arg, 0xCE, 0x03, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t SocketSendToRequest(SocketSendToRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Sends data using a connected socket (usually used for sockets over UDP).

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t SocketSendToRequest(SocketSendToRequest_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint8_t);  // SocketIndex
	msgLen += sizeof(uint8_t);  // Flags
	msgLen += sizeof(bool_t);  // SecuredMACData
	msgLen += sizeof(uint16_t);  // Size
	msgLen += sizeof(uint16_t);  // RemotePort
	msgLen += sizeof(req->RemoteIpAddress);  // RemoteIpAddress
	msgLen += req->Size;  // Data

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	pMsg[idx] = req->SocketIndex; idx++;
	pMsg[idx] = req->Flags; idx++;
	pMsg[idx] = req->SecuredMACData; idx++;
	FLib_MemCpy(pMsg + idx, &(req->Size), sizeof(req->Size)); idx += sizeof(req->Size);
	FLib_MemCpy(pMsg + idx, &(req->RemotePort), sizeof(req->RemotePort)); idx += sizeof(req->RemotePort);
	FLib_MemCpy(pMsg + idx, req->RemoteIpAddress, sizeof(req->RemoteIpAddress)); idx += sizeof(req->RemoteIpAddress);
	FLib_MemCpy(pMsg + idx, req->Data, req->Size); idx += req->Size;

	/* Send the request */
	FSCI_transmitPayload(arg, 0xCE, 0x04, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t SocketReceiveRequest(SocketReceiveRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Get data received on a connected socket (usually used for sockets over TCP)

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t SocketReceiveRequest(SocketReceiveRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x05, req, sizeof(SocketReceiveRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t SocketReceiveFromRequest(SocketReceiveFromRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Get data received on a connected socket (usually used for sockets over UDP)

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t SocketReceiveFromRequest(SocketReceiveFromRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x06, req, sizeof(SocketReceiveFromRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t SocketConnectRequest(SocketConnectRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Connect the socket to a remote socket (in case of UDP, just set the remote information).

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t SocketConnectRequest(SocketConnectRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x07, req, sizeof(SocketConnectRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t SocketListenRequest(SocketListenRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Prepare a socket for accepting a connection.

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t SocketListenRequest(SocketListenRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x08, req, sizeof(SocketListenRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t SocketAcceptRequest(SocketAcceptRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Accept a connection from a client.

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t SocketAcceptRequest(SocketAcceptRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x09, req, sizeof(SocketAcceptRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t SocketSetOptionRequest(SocketSetOptionRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Set socket options

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t SocketSetOptionRequest(SocketSetOptionRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x0A, req, sizeof(SocketSetOptionRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t SocketGetOptionRequest(SocketGetOptionRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Get socket option

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t SocketGetOptionRequest(SocketGetOptionRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x0B, req, sizeof(SocketGetOptionRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t SocketCloseRequest(SocketCloseRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Closes a socket file descriptor and frees all socket allocated resources.

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t SocketCloseRequest(SocketCloseRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x0F, req, sizeof(SocketCloseRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

#endif  /* SOCKET_ENABLE */

#if MESHCOP_ENABLE
/*!*************************************************************************************************
\fn		memStatus_t MESHCOP_StartCommissionerRequest(MESHCOP_StartCommissionerRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Start Commissioner request

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MESHCOP_StartCommissionerRequest(MESHCOP_StartCommissionerRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x40, req, sizeof(MESHCOP_StartCommissionerRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MESHCOP_StartNativeCommissionerRequest(MESHCOP_StartNativeCommissionerRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Start Native Commissioner request

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MESHCOP_StartNativeCommissionerRequest(MESHCOP_StartNativeCommissionerRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x4F, req, sizeof(MESHCOP_StartNativeCommissionerRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MESHCOP_StopCommissionerRequest(MESHCOP_StopCommissionerRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Stop Commissioner request

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MESHCOP_StopCommissionerRequest(MESHCOP_StopCommissionerRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x41, req, sizeof(MESHCOP_StopCommissionerRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MESHCOP_AddExpectedJoinerRequest(MESHCOP_AddExpectedJoinerRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Add an expected Joiner in the Commissioner's list

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MESHCOP_AddExpectedJoinerRequest(MESHCOP_AddExpectedJoinerRequest_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint8_t);  // InstanceId
	msgLen += sizeof(bool_t);  // Selected
	msgLen += sizeof(uint8_t);  // EuiType

	switch (req->EuiType)
	{
		case MESHCOP_AddExpectedJoinerRequest_EuiType_ShortEUI:
			msgLen += sizeof(uint32_t);  // ShortEUI
			break;

		case MESHCOP_AddExpectedJoinerRequest_EuiType_LongEUI:
			msgLen += sizeof(uint64_t);  // LongEUI
			break;
	}	// EUI
	msgLen += sizeof(uint8_t);  // PSKdSize
	msgLen += req->PSKdSize;  // PSKd

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	pMsg[idx] = req->InstanceId; idx++;
	pMsg[idx] = req->Selected; idx++;
	pMsg[idx] = req->EuiType; idx++;

	switch (req->EuiType)
	{
		case MESHCOP_AddExpectedJoinerRequest_EuiType_ShortEUI:
			FLib_MemCpy(pMsg + idx, &(req->EUI.ShortEUI), sizeof(req->EUI.ShortEUI)); idx += sizeof(req->EUI.ShortEUI);
			break;

		case MESHCOP_AddExpectedJoinerRequest_EuiType_LongEUI:
			FLib_MemCpy(pMsg + idx, &(req->EUI.LongEUI), sizeof(req->EUI.LongEUI)); idx += sizeof(req->EUI.LongEUI);
			break;
	}
	pMsg[idx] = req->PSKdSize; idx++;
	FLib_MemCpy(pMsg + idx, req->PSKd, req->PSKdSize); idx += req->PSKdSize;

	/* Send the request */
	FSCI_transmitPayload(arg, 0xCE, 0x42, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MESHCOP_GetExpectedJoinerRequest(MESHCOP_GetExpectedJoinerRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Get an expected Joiner from the Commissioner's list

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MESHCOP_GetExpectedJoinerRequest(MESHCOP_GetExpectedJoinerRequest_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint8_t);  // InstanceId
	msgLen += sizeof(uint8_t);  // EuiType

	switch (req->EuiType)
	{
		case MESHCOP_GetExpectedJoinerRequest_EuiType_ShortEUI:
			msgLen += sizeof(uint32_t);  // ShortEUI
			break;

		case MESHCOP_GetExpectedJoinerRequest_EuiType_LongEUI:
			msgLen += sizeof(uint64_t);  // LongEUI
			break;
	}	// EUI

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	pMsg[idx] = req->InstanceId; idx++;
	pMsg[idx] = req->EuiType; idx++;

	switch (req->EuiType)
	{
		case MESHCOP_GetExpectedJoinerRequest_EuiType_ShortEUI:
			FLib_MemCpy(pMsg + idx, &(req->EUI.ShortEUI), sizeof(req->EUI.ShortEUI)); idx += sizeof(req->EUI.ShortEUI);
			break;

		case MESHCOP_GetExpectedJoinerRequest_EuiType_LongEUI:
			FLib_MemCpy(pMsg + idx, &(req->EUI.LongEUI), sizeof(req->EUI.LongEUI)); idx += sizeof(req->EUI.LongEUI);
			break;
	}

	/* Send the request */
	FSCI_transmitPayload(arg, 0xCE, 0x43, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MESHCOP_RemoveExpectedJoinerRequest(MESHCOP_RemoveExpectedJoinerRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Remove an expected joiner from the Commissioner's list

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MESHCOP_RemoveExpectedJoinerRequest(MESHCOP_RemoveExpectedJoinerRequest_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint8_t);  // InstanceId
	msgLen += sizeof(uint8_t);  // EuiType

	switch (req->EuiType)
	{
		case MESHCOP_RemoveExpectedJoinerRequest_EuiType_ShortEUI:
			msgLen += sizeof(uint32_t);  // ShortEUI
			break;

		case MESHCOP_RemoveExpectedJoinerRequest_EuiType_LongEUI:
			msgLen += sizeof(uint64_t);  // LongEUI
			break;
	}	// EUI

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	pMsg[idx] = req->InstanceId; idx++;
	pMsg[idx] = req->EuiType; idx++;

	switch (req->EuiType)
	{
		case MESHCOP_RemoveExpectedJoinerRequest_EuiType_ShortEUI:
			FLib_MemCpy(pMsg + idx, &(req->EUI.ShortEUI), sizeof(req->EUI.ShortEUI)); idx += sizeof(req->EUI.ShortEUI);
			break;

		case MESHCOP_RemoveExpectedJoinerRequest_EuiType_LongEUI:
			FLib_MemCpy(pMsg + idx, &(req->EUI.LongEUI), sizeof(req->EUI.LongEUI)); idx += sizeof(req->EUI.LongEUI);
			break;
	}

	/* Send the request */
	FSCI_transmitPayload(arg, 0xCE, 0x44, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MESHCOP_RemoveAllExpectedJoinersRequest(MESHCOP_RemoveAllExpectedJoinersRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Remove all expected joiners from the Commissioner's list

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MESHCOP_RemoveAllExpectedJoinersRequest(MESHCOP_RemoveAllExpectedJoinersRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x45, req, sizeof(MESHCOP_RemoveAllExpectedJoinersRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MESHCOP_SyncSteeringDataRequest(MESHCOP_SyncSteeringDataRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Sync steering data request on the network with the expected joiners.

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MESHCOP_SyncSteeringDataRequest(MESHCOP_SyncSteeringDataRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x46, req, sizeof(MESHCOP_SyncSteeringDataRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MESHCOP_MgmtSetRequest(MESHCOP_MgmtSetRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Management Set request

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MESHCOP_MgmtSetRequest(MESHCOP_MgmtSetRequest_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint8_t);  // InstanceId
	msgLen += sizeof(bool_t);  // PSKcEnable

	if (req->PSKcEnable)
	{
		msgLen += 1;
	}  // PSKcSize

	msgLen += req->PSKcSize;  // PSKc
	msgLen += sizeof(bool_t);  // NetworkNameEnable

	if (req->NetworkNameEnable)
	{
		msgLen += 1;
	}  // NetworkNameSize

	msgLen += req->NetworkNameSize;  // NetworkName
	msgLen += sizeof(bool_t);  // PolicyEnable
	msgLen += 1;  // OutOfBand|Native  // Policy


	if (req->PolicyEnable)
	{
		msgLen += 2;
	}  // KeyRotationInterval

	msgLen += sizeof(bool_t);  // TimestampEnable

	if (req->TimestampEnable)
	{
		msgLen += 6;
	}  // Seconds


	if (req->TimestampEnable)
	{
		msgLen += 2;
	}  // Ticks


	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	pMsg[idx] = req->InstanceId; idx++;
	pMsg[idx] = req->PSKcEnable; idx++;

	if (req->PSKcEnable)
	{
		pMsg[idx] = req->PSKcSize; idx++;
	}

	FLib_MemCpy(pMsg + idx, req->PSKc, req->PSKcSize); idx += req->PSKcSize;
	pMsg[idx] = req->NetworkNameEnable; idx++;

	if (req->NetworkNameEnable)
	{
		pMsg[idx] = req->NetworkNameSize; idx++;
	}

	FLib_MemCpy(pMsg + idx, req->NetworkName, req->NetworkNameSize); idx += req->NetworkNameSize;
	pMsg[idx] = req->PolicyEnable; idx++;
	pMsg[idx] = req->Policy; idx++;


	if (req->PolicyEnable)
	{
		FLib_MemCpy(pMsg + idx, &(req->KeyRotationInterval), sizeof(req->KeyRotationInterval)); idx += sizeof(req->KeyRotationInterval);
	}

	pMsg[idx] = req->TimestampEnable; idx++;

	if (req->TimestampEnable)
	{
		FLib_MemCpy(pMsg + idx, req->Seconds, sizeof(req->Seconds)); idx += sizeof(req->Seconds);
	}


	if (req->TimestampEnable)
	{
		FLib_MemCpy(pMsg + idx, &(req->Ticks), sizeof(req->Ticks)); idx += sizeof(req->Ticks);
	}


	/* Send the request */
	FSCI_transmitPayload(arg, 0xCE, 0x4B, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MESHCOP_MgmtGetRequest(MESHCOP_MgmtGetRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Management Get request

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MESHCOP_MgmtGetRequest(MESHCOP_MgmtGetRequest_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint8_t);  // InstanceId
	msgLen += sizeof(uint8_t);  // NumberOfTlvIds
	msgLen += req->NumberOfTlvIds * sizeof(uint8_t);  // TlvIds

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	pMsg[idx] = req->InstanceId; idx++;
	pMsg[idx] = req->NumberOfTlvIds; idx++;
	FLib_MemCpy(pMsg + idx, req->TlvIds, req->NumberOfTlvIds * sizeof(uint8_t)); idx += req->NumberOfTlvIds * sizeof(uint8_t);

	/* Send the request */
	FSCI_transmitPayload(arg, 0xCE, 0x4C, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MESHCOP_SetCommissionerCredentialRequest(MESHCOP_SetCommissionerCredentialRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Setup the commissioner credential to connect to a network

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MESHCOP_SetCommissionerCredentialRequest(MESHCOP_SetCommissionerCredentialRequest_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint8_t);  // InstanceId
	msgLen += sizeof(uint8_t);  // PSKcSize
	msgLen += req->PSKcSize;  // PSKc
	msgLen += 8;  // XpanId
	msgLen += sizeof(uint8_t);  // NetworkNameSize
	msgLen += req->NetworkNameSize;  // NetworkName

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	pMsg[idx] = req->InstanceId; idx++;
	pMsg[idx] = req->PSKcSize; idx++;
	FLib_MemCpy(pMsg + idx, req->PSKc, req->PSKcSize); idx += req->PSKcSize;
	FLib_MemCpy(pMsg + idx, req->XpanId, 8); idx += 8;
	pMsg[idx] = req->NetworkNameSize; idx++;
	FLib_MemCpy(pMsg + idx, req->NetworkName, req->NetworkNameSize); idx += req->NetworkNameSize;

	/* Send the request */
	FSCI_transmitPayload(arg, 0xCE, 0x4D, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MESHCOP_MgmNwkFormRequest(MESHCOP_MgmNwkFormRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Sets the network start parameters

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MESHCOP_MgmNwkFormRequest(MESHCOP_MgmNwkFormRequest_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(req->IPAddress);  // IPAddress
	msgLen += sizeof(uint8_t);  // NwkNameSize
	msgLen += req->NwkNameSize;  // NwkName
	msgLen += sizeof(req->NetworkMasterKey);  // NetworkMasterKey
	msgLen += sizeof(uint8_t);  // PSKcSize
	msgLen += req->PSKcSize;  // PSKc
	msgLen += sizeof(uint8_t);  // Channel

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	FLib_MemCpy(pMsg + idx, req->IPAddress, sizeof(req->IPAddress)); idx += sizeof(req->IPAddress);
	pMsg[idx] = req->NwkNameSize; idx++;
	FLib_MemCpy(pMsg + idx, req->NwkName, req->NwkNameSize); idx += req->NwkNameSize;
	FLib_MemCpy(pMsg + idx, req->NetworkMasterKey, sizeof(req->NetworkMasterKey)); idx += sizeof(req->NetworkMasterKey);
	pMsg[idx] = req->PSKcSize; idx++;
	FLib_MemCpy(pMsg + idx, req->PSKc, req->PSKcSize); idx += req->PSKcSize;
	pMsg[idx] = req->Channel; idx++;

	/* Send the request */
	FSCI_transmitPayload(arg, 0xCE, 0x47, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MESHCOP_MgmNwkLeaveRequest(MESHCOP_MgmNwkLeaveRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Send a Network Leave command

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MESHCOP_MgmNwkLeaveRequest(MESHCOP_MgmNwkLeaveRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x4A, req, sizeof(MESHCOP_MgmNwkLeaveRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MESHCOP_MgmtCommissionerGetRequest(MESHCOP_MgmtCommissionerGetRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Management Commissioner Get Request

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MESHCOP_MgmtCommissionerGetRequest(MESHCOP_MgmtCommissionerGetRequest_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint8_t);  // InstanceId
	msgLen += sizeof(req->IPAddress);  // IPAddress
	msgLen += sizeof(uint8_t);  // NumberOfTlvIds
	msgLen += req->NumberOfTlvIds * sizeof(uint8_t);  // TlvIds

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	pMsg[idx] = req->InstanceId; idx++;
	FLib_MemCpy(pMsg + idx, req->IPAddress, sizeof(req->IPAddress)); idx += sizeof(req->IPAddress);
	pMsg[idx] = req->NumberOfTlvIds; idx++;
	FLib_MemCpy(pMsg + idx, req->TlvIds, req->NumberOfTlvIds * sizeof(uint8_t)); idx += req->NumberOfTlvIds * sizeof(uint8_t);

	/* Send the request */
	FSCI_transmitPayload(arg, 0xCE, 0xA0, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MESHCOP_MgmtActiveGetRequest(MESHCOP_MgmtActiveGetRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Management Active Get Request

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MESHCOP_MgmtActiveGetRequest(MESHCOP_MgmtActiveGetRequest_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint8_t);  // InstanceId
	msgLen += sizeof(req->IPAddress);  // IPAddress
	msgLen += sizeof(uint8_t);  // NumberOfTlvIds
	msgLen += req->NumberOfTlvIds * sizeof(uint8_t);  // TlvIds

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	pMsg[idx] = req->InstanceId; idx++;
	FLib_MemCpy(pMsg + idx, req->IPAddress, sizeof(req->IPAddress)); idx += sizeof(req->IPAddress);
	pMsg[idx] = req->NumberOfTlvIds; idx++;
	FLib_MemCpy(pMsg + idx, req->TlvIds, req->NumberOfTlvIds * sizeof(uint8_t)); idx += req->NumberOfTlvIds * sizeof(uint8_t);

	/* Send the request */
	FSCI_transmitPayload(arg, 0xCE, 0xA2, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MESHCOP_MgmtPendingGetRequest(MESHCOP_MgmtPendingGetRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Management Pending Get Request

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MESHCOP_MgmtPendingGetRequest(MESHCOP_MgmtPendingGetRequest_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint8_t);  // InstanceId
	msgLen += sizeof(req->IPAddress);  // IPAddress
	msgLen += sizeof(uint8_t);  // NumberOfTlvIds
	msgLen += req->NumberOfTlvIds * sizeof(uint8_t);  // TlvIds

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	pMsg[idx] = req->InstanceId; idx++;
	FLib_MemCpy(pMsg + idx, req->IPAddress, sizeof(req->IPAddress)); idx += sizeof(req->IPAddress);
	pMsg[idx] = req->NumberOfTlvIds; idx++;
	FLib_MemCpy(pMsg + idx, req->TlvIds, req->NumberOfTlvIds * sizeof(uint8_t)); idx += req->NumberOfTlvIds * sizeof(uint8_t);

	/* Send the request */
	FSCI_transmitPayload(arg, 0xCE, 0xA4, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MESHCOP_MgmtCommissionerSetRequest(MESHCOP_MgmtCommissionerSetRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Management Commissioner Set Request

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MESHCOP_MgmtCommissionerSetRequest(MESHCOP_MgmtCommissionerSetRequest_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint8_t);  // InstanceId
	msgLen += sizeof(req->IPAddress);  // IPAddress
	msgLen += sizeof(bool_t);  // SessionIdEnable

	if (req->SessionIdEnable)
	{
		msgLen += 2;
	}  // SessionId

	msgLen += sizeof(bool_t);  // BorderRouterLocatorEnable

	if (req->BorderRouterLocatorEnable)
	{
		msgLen += 2;
	}  // BorderRouterLocator

	msgLen += sizeof(bool_t);  // NewSessionIdEnable

	if (req->NewSessionIdEnable)
	{
		msgLen += 2;
	}  // NewSessionId

	msgLen += sizeof(bool_t);  // SteeringDataEnable

	if (req->SteeringDataEnable)
	{
		msgLen += 1;
	}  // SteeringDataSize

	msgLen += req->SteeringDataSize;  // SteeringData
	msgLen += sizeof(bool_t);  // ChannelEnable

	if (req->ChannelEnable)
	{
		msgLen += 1;
	}  // Channel

	msgLen += sizeof(bool_t);  // ChannelMaskEnable

	if (req->ChannelMaskEnable)
	{
		msgLen += 1;
	}  // ChannelPage


	if (req->ChannelMaskEnable)
	{
		msgLen += 1;
	}  // ChannelMaskLength

	msgLen += req->ChannelMaskLength;  // ChannelMask
	msgLen += sizeof(bool_t);  // XpanIdEnable

	if (req->XpanIdEnable)
	{
		msgLen += 8;
	}  // XpanId

	msgLen += sizeof(bool_t);  // MLPrefixEnable

	if (req->MLPrefixEnable)
	{
		msgLen += 16;  // PrefixData
		msgLen += sizeof(uint8_t);  // PrefixLength
	}  // MLPrefix

	msgLen += sizeof(bool_t);  // MasterKeyEnable

	if (req->MasterKeyEnable)
	{
		msgLen += 16;
	}  // MasterKey

	msgLen += sizeof(bool_t);  // NwkNameEnable

	if (req->NwkNameEnable)
	{
		msgLen += 1;
	}  // NwkNameSize

	msgLen += req->NwkNameSize;  // NwkName
	msgLen += sizeof(bool_t);  // PanIdEnable

	if (req->PanIdEnable)
	{
		msgLen += 2;
	}  // PanId

	msgLen += sizeof(bool_t);  // PSKcEnable

	if (req->PSKcEnable)
	{
		msgLen += 1;
	}  // PskcSize

	msgLen += req->PskcSize;  // PSKc
	msgLen += sizeof(bool_t);  // PolicyEnable

	if (req->PolicyEnable)
	{
		msgLen += 2;
	}  // RotationInterval

	msgLen += 1;  // OutOfBand|Native  // Policy

	msgLen += sizeof(bool_t);  // ActiveTimestampEnable

	if (req->ActiveTimestampEnable)
	{
		msgLen += 6;
	}  // ActiveSeconds


	if (req->ActiveTimestampEnable)
	{
		msgLen += 2;
	}  // ActiveTicks

	msgLen += sizeof(bool_t);  // PendingTimestampEnable

	if (req->PendingTimestampEnable)
	{
		msgLen += 6;
	}  // PendingSeconds


	if (req->PendingTimestampEnable)
	{
		msgLen += 2;
	}  // PendingTicks

	msgLen += sizeof(bool_t);  // DelayTimerEnable

	if (req->DelayTimerEnable)
	{
		msgLen += 4;
	}  // Timeout

	msgLen += sizeof(bool_t);  // FutureTlvEnable

	if (req->FutureTlvEnable)
	{
		msgLen += sizeof(uint8_t);  // FutureTlvSize
		msgLen += req->FutureTlv.FutureTlvSize;  // FutureTlvValue
	}	// FutureTlv


	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	pMsg[idx] = req->InstanceId; idx++;
	FLib_MemCpy(pMsg + idx, req->IPAddress, sizeof(req->IPAddress)); idx += sizeof(req->IPAddress);
	pMsg[idx] = req->SessionIdEnable; idx++;

	if (req->SessionIdEnable)
	{
		FLib_MemCpy(pMsg + idx, &(req->SessionId), sizeof(req->SessionId)); idx += sizeof(req->SessionId);
	}

	pMsg[idx] = req->BorderRouterLocatorEnable; idx++;

	if (req->BorderRouterLocatorEnable)
	{
		FLib_MemCpy(pMsg + idx, &(req->BorderRouterLocator), sizeof(req->BorderRouterLocator)); idx += sizeof(req->BorderRouterLocator);
	}

	pMsg[idx] = req->NewSessionIdEnable; idx++;

	if (req->NewSessionIdEnable)
	{
		FLib_MemCpy(pMsg + idx, &(req->NewSessionId), sizeof(req->NewSessionId)); idx += sizeof(req->NewSessionId);
	}

	pMsg[idx] = req->SteeringDataEnable; idx++;

	if (req->SteeringDataEnable)
	{
		pMsg[idx] = req->SteeringDataSize; idx++;
	}

	FLib_MemCpy(pMsg + idx, req->SteeringData, req->SteeringDataSize); idx += req->SteeringDataSize;
	pMsg[idx] = req->ChannelEnable; idx++;

	if (req->ChannelEnable)
	{
		pMsg[idx] = req->Channel; idx++;
	}

	pMsg[idx] = req->ChannelMaskEnable; idx++;

	if (req->ChannelMaskEnable)
	{
		pMsg[idx] = req->ChannelPage; idx++;
	}


	if (req->ChannelMaskEnable)
	{
		pMsg[idx] = req->ChannelMaskLength; idx++;
	}

	FLib_MemCpy(pMsg + idx, req->ChannelMask, req->ChannelMaskLength); idx += req->ChannelMaskLength;
	pMsg[idx] = req->XpanIdEnable; idx++;

	if (req->XpanIdEnable)
	{
		FLib_MemCpy(pMsg + idx, req->XpanId, 8); idx += 8;
	}

	pMsg[idx] = req->MLPrefixEnable; idx++;

	if (req->MLPrefixEnable)
	{
		FLib_MemCpy(pMsg + idx, req->MLPrefix.PrefixData, 16); idx += 16;
		pMsg[idx] = req->MLPrefix.PrefixLength; idx++;
	}
	pMsg[idx] = req->MasterKeyEnable; idx++;

	if (req->MasterKeyEnable)
	{
		FLib_MemCpy(pMsg + idx, req->MasterKey, sizeof(req->MasterKey)); idx += sizeof(req->MasterKey);
	}

	pMsg[idx] = req->NwkNameEnable; idx++;

	if (req->NwkNameEnable)
	{
		pMsg[idx] = req->NwkNameSize; idx++;
	}

	FLib_MemCpy(pMsg + idx, req->NwkName, req->NwkNameSize); idx += req->NwkNameSize;
	pMsg[idx] = req->PanIdEnable; idx++;

	if (req->PanIdEnable)
	{
		FLib_MemCpy(pMsg + idx, &(req->PanId), sizeof(req->PanId)); idx += sizeof(req->PanId);
	}

	pMsg[idx] = req->PSKcEnable; idx++;

	if (req->PSKcEnable)
	{
		pMsg[idx] = req->PskcSize; idx++;
	}

	FLib_MemCpy(pMsg + idx, req->PSKc, req->PskcSize); idx += req->PskcSize;
	pMsg[idx] = req->PolicyEnable; idx++;

	if (req->PolicyEnable)
	{
		FLib_MemCpy(pMsg + idx, &(req->RotationInterval), sizeof(req->RotationInterval)); idx += sizeof(req->RotationInterval);
	}

	pMsg[idx] = req->Policy; idx++;

	pMsg[idx] = req->ActiveTimestampEnable; idx++;

	if (req->ActiveTimestampEnable)
	{
		FLib_MemCpy(pMsg + idx, req->ActiveSeconds, sizeof(req->ActiveSeconds)); idx += sizeof(req->ActiveSeconds);
	}


	if (req->ActiveTimestampEnable)
	{
		FLib_MemCpy(pMsg + idx, &(req->ActiveTicks), sizeof(req->ActiveTicks)); idx += sizeof(req->ActiveTicks);
	}

	pMsg[idx] = req->PendingTimestampEnable; idx++;

	if (req->PendingTimestampEnable)
	{
		FLib_MemCpy(pMsg + idx, req->PendingSeconds, sizeof(req->PendingSeconds)); idx += sizeof(req->PendingSeconds);
	}


	if (req->PendingTimestampEnable)
	{
		FLib_MemCpy(pMsg + idx, &(req->PendingTicks), sizeof(req->PendingTicks)); idx += sizeof(req->PendingTicks);
	}

	pMsg[idx] = req->DelayTimerEnable; idx++;

	if (req->DelayTimerEnable)
	{
		FLib_MemCpy(pMsg + idx, &(req->Timeout), sizeof(req->Timeout)); idx += sizeof(req->Timeout);
	}

	pMsg[idx] = req->FutureTlvEnable; idx++;

	if (req->FutureTlvEnable)
	{
		pMsg[idx] = req->FutureTlv.FutureTlvSize; idx++;
		FLib_MemCpy(pMsg + idx, req->FutureTlv.FutureTlvValue, req->FutureTlv.FutureTlvSize); idx += req->FutureTlv.FutureTlvSize;
	}

	/* Send the request */
	FSCI_transmitPayload(arg, 0xCE, 0xA1, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MESHCOP_MgmtActiveSetRequest(MESHCOP_MgmtActiveSetRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Management Active Set Request

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MESHCOP_MgmtActiveSetRequest(MESHCOP_MgmtActiveSetRequest_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint8_t);  // InstanceId
	msgLen += sizeof(req->IPAddress);  // IPAddress
	msgLen += sizeof(bool_t);  // SessionIdEnable

	if (req->SessionIdEnable)
	{
		msgLen += 2;
	}  // SessionId

	msgLen += sizeof(bool_t);  // BorderRouterLocatorEnable

	if (req->BorderRouterLocatorEnable)
	{
		msgLen += 2;
	}  // BorderRouterLocator

	msgLen += sizeof(bool_t);  // NewSesswionIdEnable

	if (req->NewSesswionIdEnable)
	{
		msgLen += 2;
	}  // NewSesswionId

	msgLen += sizeof(bool_t);  // SteeringDataEnable

	if (req->SteeringDataEnable)
	{
		msgLen += 1;
	}  // SteeringDataSize

	msgLen += req->SteeringDataSize;  // SteeringData
	msgLen += sizeof(bool_t);  // ChannelEnable

	if (req->ChannelEnable)
	{
		msgLen += 1;
	}  // Channel

	msgLen += sizeof(bool_t);  // ChannelMaskEnable

	if (req->ChannelMaskEnable)
	{
		msgLen += 1;
	}  // ChannelPage


	if (req->ChannelMaskEnable)
	{
		msgLen += 1;
	}  // ChannelMaskLength

	msgLen += req->ChannelMaskLength;  // ChannelMask
	msgLen += sizeof(bool_t);  // XpanIdEnable

	if (req->XpanIdEnable)
	{
		msgLen += 8;
	}  // XpanId

	msgLen += sizeof(bool_t);  // MLPrefixEnable

	if (req->MLPrefixEnable)
	{
		msgLen += 16;  // PrefixData
		msgLen += sizeof(uint8_t);  // PrefixLength
	}  // MLPrefix

	msgLen += sizeof(bool_t);  // MasterKeyEnable

	if (req->MasterKeyEnable)
	{
		msgLen += 16;
	}  // MasterKey

	msgLen += sizeof(bool_t);  // NwkNameEnable

	if (req->NwkNameEnable)
	{
		msgLen += 1;
	}  // NwkNameSize

	msgLen += req->NwkNameSize;  // NwkName
	msgLen += sizeof(bool_t);  // PanIdEnable

	if (req->PanIdEnable)
	{
		msgLen += 2;
	}  // PanId

	msgLen += sizeof(bool_t);  // PSKcEnable

	if (req->PSKcEnable)
	{
		msgLen += 1;
	}  // PskcSize

	msgLen += req->PskcSize;  // PSKc
	msgLen += sizeof(bool_t);  // PolicyEnable

	if (req->PolicyEnable)
	{
		msgLen += 2;
	}  // RotationInterval

	msgLen += 1;  // O|N|R|C|B|Reserved  // Policy

	msgLen += sizeof(bool_t);  // ActiveTimestampEnable

	if (req->ActiveTimestampEnable)
	{
		msgLen += 6;
	}  // ActiveSeconds


	if (req->ActiveTimestampEnable)
	{
		msgLen += 2;
	}  // ActiveTicks

	msgLen += sizeof(bool_t);  // PendingTimestampEnable

	if (req->PendingTimestampEnable)
	{
		msgLen += 6;
	}  // PendingSeconds


	if (req->PendingTimestampEnable)
	{
		msgLen += 2;
	}  // PendingTicks

	msgLen += sizeof(bool_t);  // DelayTimerEnable

	if (req->DelayTimerEnable)
	{
		msgLen += 4;
	}  // Timeout

	msgLen += sizeof(bool_t);  // FutureTlvEnable

	if (req->FutureTlvEnable)
	{
		msgLen += sizeof(uint8_t);  // FutureTlvSize
		msgLen += req->FutureTlv.FutureTlvSize;  // FutureTlvValue
	}	// FutureTlv


	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	pMsg[idx] = req->InstanceId; idx++;
	FLib_MemCpy(pMsg + idx, req->IPAddress, sizeof(req->IPAddress)); idx += sizeof(req->IPAddress);
	pMsg[idx] = req->SessionIdEnable; idx++;

	if (req->SessionIdEnable)
	{
		FLib_MemCpy(pMsg + idx, &(req->SessionId), sizeof(req->SessionId)); idx += sizeof(req->SessionId);
	}

	pMsg[idx] = req->BorderRouterLocatorEnable; idx++;

	if (req->BorderRouterLocatorEnable)
	{
		FLib_MemCpy(pMsg + idx, &(req->BorderRouterLocator), sizeof(req->BorderRouterLocator)); idx += sizeof(req->BorderRouterLocator);
	}

	pMsg[idx] = req->NewSesswionIdEnable; idx++;

	if (req->NewSesswionIdEnable)
	{
		FLib_MemCpy(pMsg + idx, &(req->NewSesswionId), sizeof(req->NewSesswionId)); idx += sizeof(req->NewSesswionId);
	}

	pMsg[idx] = req->SteeringDataEnable; idx++;

	if (req->SteeringDataEnable)
	{
		pMsg[idx] = req->SteeringDataSize; idx++;
	}

	FLib_MemCpy(pMsg + idx, req->SteeringData, req->SteeringDataSize); idx += req->SteeringDataSize;
	pMsg[idx] = req->ChannelEnable; idx++;

	if (req->ChannelEnable)
	{
		pMsg[idx] = req->Channel; idx++;
	}

	pMsg[idx] = req->ChannelMaskEnable; idx++;

	if (req->ChannelMaskEnable)
	{
		pMsg[idx] = req->ChannelPage; idx++;
	}


	if (req->ChannelMaskEnable)
	{
		pMsg[idx] = req->ChannelMaskLength; idx++;
	}

	FLib_MemCpy(pMsg + idx, req->ChannelMask, req->ChannelMaskLength); idx += req->ChannelMaskLength;
	pMsg[idx] = req->XpanIdEnable; idx++;

	if (req->XpanIdEnable)
	{
		FLib_MemCpy(pMsg + idx, req->XpanId, 8); idx += 8;
	}

	pMsg[idx] = req->MLPrefixEnable; idx++;

	if (req->MLPrefixEnable)
	{
		FLib_MemCpy(pMsg + idx, req->MLPrefix.PrefixData, 16); idx += 16;
		pMsg[idx] = req->MLPrefix.PrefixLength; idx++;
	}
	pMsg[idx] = req->MasterKeyEnable; idx++;

	if (req->MasterKeyEnable)
	{
		FLib_MemCpy(pMsg + idx, req->MasterKey, sizeof(req->MasterKey)); idx += sizeof(req->MasterKey);
	}

	pMsg[idx] = req->NwkNameEnable; idx++;

	if (req->NwkNameEnable)
	{
		pMsg[idx] = req->NwkNameSize; idx++;
	}

	FLib_MemCpy(pMsg + idx, req->NwkName, req->NwkNameSize); idx += req->NwkNameSize;
	pMsg[idx] = req->PanIdEnable; idx++;

	if (req->PanIdEnable)
	{
		FLib_MemCpy(pMsg + idx, &(req->PanId), sizeof(req->PanId)); idx += sizeof(req->PanId);
	}

	pMsg[idx] = req->PSKcEnable; idx++;

	if (req->PSKcEnable)
	{
		pMsg[idx] = req->PskcSize; idx++;
	}

	FLib_MemCpy(pMsg + idx, req->PSKc, req->PskcSize); idx += req->PskcSize;
	pMsg[idx] = req->PolicyEnable; idx++;

	if (req->PolicyEnable)
	{
		FLib_MemCpy(pMsg + idx, &(req->RotationInterval), sizeof(req->RotationInterval)); idx += sizeof(req->RotationInterval);
	}

	pMsg[idx] = req->Policy; idx++;

	pMsg[idx] = req->ActiveTimestampEnable; idx++;

	if (req->ActiveTimestampEnable)
	{
		FLib_MemCpy(pMsg + idx, req->ActiveSeconds, sizeof(req->ActiveSeconds)); idx += sizeof(req->ActiveSeconds);
	}


	if (req->ActiveTimestampEnable)
	{
		FLib_MemCpy(pMsg + idx, &(req->ActiveTicks), sizeof(req->ActiveTicks)); idx += sizeof(req->ActiveTicks);
	}

	pMsg[idx] = req->PendingTimestampEnable; idx++;

	if (req->PendingTimestampEnable)
	{
		FLib_MemCpy(pMsg + idx, req->PendingSeconds, sizeof(req->PendingSeconds)); idx += sizeof(req->PendingSeconds);
	}


	if (req->PendingTimestampEnable)
	{
		FLib_MemCpy(pMsg + idx, &(req->PendingTicks), sizeof(req->PendingTicks)); idx += sizeof(req->PendingTicks);
	}

	pMsg[idx] = req->DelayTimerEnable; idx++;

	if (req->DelayTimerEnable)
	{
		FLib_MemCpy(pMsg + idx, &(req->Timeout), sizeof(req->Timeout)); idx += sizeof(req->Timeout);
	}

	pMsg[idx] = req->FutureTlvEnable; idx++;

	if (req->FutureTlvEnable)
	{
		pMsg[idx] = req->FutureTlv.FutureTlvSize; idx++;
		FLib_MemCpy(pMsg + idx, req->FutureTlv.FutureTlvValue, req->FutureTlv.FutureTlvSize); idx += req->FutureTlv.FutureTlvSize;
	}

	/* Send the request */
	FSCI_transmitPayload(arg, 0xCE, 0xA3, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MESHCOP_MgmtPendingSetRequest(MESHCOP_MgmtPendingSetRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Management Pending Set Request

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MESHCOP_MgmtPendingSetRequest(MESHCOP_MgmtPendingSetRequest_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint8_t);  // InstanceId
	msgLen += sizeof(req->IPAddress);  // IPAddress
	msgLen += sizeof(bool_t);  // SessionIdEnable

	if (req->SessionIdEnable)
	{
		msgLen += 2;
	}  // SessionId

	msgLen += sizeof(bool_t);  // BorderRouterLocatorEnable

	if (req->BorderRouterLocatorEnable)
	{
		msgLen += 2;
	}  // BorderRouterLocator

	msgLen += sizeof(bool_t);  // NewSesswionIdEnable

	if (req->NewSesswionIdEnable)
	{
		msgLen += 2;
	}  // NewSesswionId

	msgLen += sizeof(bool_t);  // SteeringDataEnable

	if (req->SteeringDataEnable)
	{
		msgLen += 1;
	}  // SteeringDataSize

	msgLen += req->SteeringDataSize;  // SteeringData
	msgLen += sizeof(bool_t);  // ChannelEnable

	if (req->ChannelEnable)
	{
		msgLen += 1;
	}  // Channel

	msgLen += sizeof(bool_t);  // ChannelMaskEnable

	if (req->ChannelMaskEnable)
	{
		msgLen += 1;
	}  // ChannelPage


	if (req->ChannelMaskEnable)
	{
		msgLen += 1;
	}  // ChannelMaskLength

	msgLen += req->ChannelMaskLength;  // ChannelMask
	msgLen += sizeof(bool_t);  // XpanIdEnable

	if (req->XpanIdEnable)
	{
		msgLen += 8;
	}  // XpanId

	msgLen += sizeof(bool_t);  // MLPrefixEnable

	if (req->MLPrefixEnable)
	{
		msgLen += 16;  // PrefixData
		msgLen += sizeof(uint8_t);  // PrefixLength
	}  // MLPrefix

	msgLen += sizeof(bool_t);  // MasterKeyEnable

	if (req->MasterKeyEnable)
	{
		msgLen += 16;
	}  // MasterKey

	msgLen += sizeof(bool_t);  // NwkNameEnable

	if (req->NwkNameEnable)
	{
		msgLen += 1;
	}  // NwkNameSize

	msgLen += req->NwkNameSize;  // NwkName
	msgLen += sizeof(bool_t);  // PanIdEnable

	if (req->PanIdEnable)
	{
		msgLen += 2;
	}  // PanId

	msgLen += sizeof(bool_t);  // PSKcEnable

	if (req->PSKcEnable)
	{
		msgLen += 1;
	}  // PskcSize

	msgLen += req->PskcSize;  // PSKc
	msgLen += sizeof(bool_t);  // PolicyEnable

	if (req->PolicyEnable)
	{
		msgLen += 2;
	}  // RotationInterval

	msgLen += 1;  // OutOfBand|Native  // Policy

	msgLen += sizeof(bool_t);  // ActiveTimestampEnable

	if (req->ActiveTimestampEnable)
	{
		msgLen += 6;
	}  // ActiveSeconds


	if (req->ActiveTimestampEnable)
	{
		msgLen += 2;
	}  // ActiveTicks

	msgLen += sizeof(bool_t);  // PendingTimestampEnable

	if (req->PendingTimestampEnable)
	{
		msgLen += 6;
	}  // PendingSeconds


	if (req->PendingTimestampEnable)
	{
		msgLen += 2;
	}  // PendingTicks

	msgLen += sizeof(bool_t);  // DelayTimerEnable

	if (req->DelayTimerEnable)
	{
		msgLen += 4;
	}  // Timeout

	msgLen += sizeof(bool_t);  // FutureTlvEnable

	if (req->FutureTlvEnable)
	{
		msgLen += sizeof(uint8_t);  // FutureTlvSize
		msgLen += req->FutureTlv.FutureTlvSize;  // FutureTlvValue
	}	// FutureTlv


	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	pMsg[idx] = req->InstanceId; idx++;
	FLib_MemCpy(pMsg + idx, req->IPAddress, sizeof(req->IPAddress)); idx += sizeof(req->IPAddress);
	pMsg[idx] = req->SessionIdEnable; idx++;

	if (req->SessionIdEnable)
	{
		FLib_MemCpy(pMsg + idx, &(req->SessionId), sizeof(req->SessionId)); idx += sizeof(req->SessionId);
	}

	pMsg[idx] = req->BorderRouterLocatorEnable; idx++;

	if (req->BorderRouterLocatorEnable)
	{
		FLib_MemCpy(pMsg + idx, &(req->BorderRouterLocator), sizeof(req->BorderRouterLocator)); idx += sizeof(req->BorderRouterLocator);
	}

	pMsg[idx] = req->NewSesswionIdEnable; idx++;

	if (req->NewSesswionIdEnable)
	{
		FLib_MemCpy(pMsg + idx, &(req->NewSesswionId), sizeof(req->NewSesswionId)); idx += sizeof(req->NewSesswionId);
	}

	pMsg[idx] = req->SteeringDataEnable; idx++;

	if (req->SteeringDataEnable)
	{
		pMsg[idx] = req->SteeringDataSize; idx++;
	}

	FLib_MemCpy(pMsg + idx, req->SteeringData, req->SteeringDataSize); idx += req->SteeringDataSize;
	pMsg[idx] = req->ChannelEnable; idx++;

	if (req->ChannelEnable)
	{
		pMsg[idx] = req->Channel; idx++;
	}

	pMsg[idx] = req->ChannelMaskEnable; idx++;

	if (req->ChannelMaskEnable)
	{
		pMsg[idx] = req->ChannelPage; idx++;
	}


	if (req->ChannelMaskEnable)
	{
		pMsg[idx] = req->ChannelMaskLength; idx++;
	}

	FLib_MemCpy(pMsg + idx, req->ChannelMask, req->ChannelMaskLength); idx += req->ChannelMaskLength;
	pMsg[idx] = req->XpanIdEnable; idx++;

	if (req->XpanIdEnable)
	{
		FLib_MemCpy(pMsg + idx, req->XpanId, 8); idx += 8;
	}

	pMsg[idx] = req->MLPrefixEnable; idx++;

	if (req->MLPrefixEnable)
	{
		FLib_MemCpy(pMsg + idx, req->MLPrefix.PrefixData, 16); idx += 16;
		pMsg[idx] = req->MLPrefix.PrefixLength; idx++;
	}
	pMsg[idx] = req->MasterKeyEnable; idx++;

	if (req->MasterKeyEnable)
	{
		FLib_MemCpy(pMsg + idx, req->MasterKey, sizeof(req->MasterKey)); idx += sizeof(req->MasterKey);
	}

	pMsg[idx] = req->NwkNameEnable; idx++;

	if (req->NwkNameEnable)
	{
		pMsg[idx] = req->NwkNameSize; idx++;
	}

	FLib_MemCpy(pMsg + idx, req->NwkName, req->NwkNameSize); idx += req->NwkNameSize;
	pMsg[idx] = req->PanIdEnable; idx++;

	if (req->PanIdEnable)
	{
		FLib_MemCpy(pMsg + idx, &(req->PanId), sizeof(req->PanId)); idx += sizeof(req->PanId);
	}

	pMsg[idx] = req->PSKcEnable; idx++;

	if (req->PSKcEnable)
	{
		pMsg[idx] = req->PskcSize; idx++;
	}

	FLib_MemCpy(pMsg + idx, req->PSKc, req->PskcSize); idx += req->PskcSize;
	pMsg[idx] = req->PolicyEnable; idx++;

	if (req->PolicyEnable)
	{
		FLib_MemCpy(pMsg + idx, &(req->RotationInterval), sizeof(req->RotationInterval)); idx += sizeof(req->RotationInterval);
	}

	pMsg[idx] = req->Policy; idx++;

	pMsg[idx] = req->ActiveTimestampEnable; idx++;

	if (req->ActiveTimestampEnable)
	{
		FLib_MemCpy(pMsg + idx, req->ActiveSeconds, sizeof(req->ActiveSeconds)); idx += sizeof(req->ActiveSeconds);
	}


	if (req->ActiveTimestampEnable)
	{
		FLib_MemCpy(pMsg + idx, &(req->ActiveTicks), sizeof(req->ActiveTicks)); idx += sizeof(req->ActiveTicks);
	}

	pMsg[idx] = req->PendingTimestampEnable; idx++;

	if (req->PendingTimestampEnable)
	{
		FLib_MemCpy(pMsg + idx, req->PendingSeconds, sizeof(req->PendingSeconds)); idx += sizeof(req->PendingSeconds);
	}


	if (req->PendingTimestampEnable)
	{
		FLib_MemCpy(pMsg + idx, &(req->PendingTicks), sizeof(req->PendingTicks)); idx += sizeof(req->PendingTicks);
	}

	pMsg[idx] = req->DelayTimerEnable; idx++;

	if (req->DelayTimerEnable)
	{
		FLib_MemCpy(pMsg + idx, &(req->Timeout), sizeof(req->Timeout)); idx += sizeof(req->Timeout);
	}

	pMsg[idx] = req->FutureTlvEnable; idx++;

	if (req->FutureTlvEnable)
	{
		pMsg[idx] = req->FutureTlv.FutureTlvSize; idx++;
		FLib_MemCpy(pMsg + idx, req->FutureTlv.FutureTlvValue, req->FutureTlv.FutureTlvSize); idx += req->FutureTlv.FutureTlvSize;
	}

	/* Send the request */
	FSCI_transmitPayload(arg, 0xCE, 0xA5, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MESHCOP_MgmtSendPanIdQueryRequest(MESHCOP_MgmtSendPanIdQueryRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Search for Pan ID conflict

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MESHCOP_MgmtSendPanIdQueryRequest(MESHCOP_MgmtSendPanIdQueryRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0xA8, req, sizeof(MESHCOP_MgmtSendPanIdQueryRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MESHCOP_MgmtSendEdScanRequest(MESHCOP_MgmtSendEdScanRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Search for Pan ID conflict

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MESHCOP_MgmtSendEdScanRequest(MESHCOP_MgmtSendEdScanRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0xAA, req, sizeof(MESHCOP_MgmtSendEdScanRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MESHCOP_MgmtSendAnnounceBeginRequest(MESHCOP_MgmtSendAnnounceBeginRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Request to send a MGMT_ANNOUNCE_BEGIN.ntf

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MESHCOP_MgmtSendAnnounceBeginRequest(MESHCOP_MgmtSendAnnounceBeginRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0xA7, req, sizeof(MESHCOP_MgmtSendAnnounceBeginRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

#endif  /* MESHCOP_ENABLE */

#if IPUTILS_ENABLE
/*!*************************************************************************************************
\fn		memStatus_t NWKU_IfconfigBindRequest(NWKU_IfconfigBindRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Bind an IP address to an interface.

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t NWKU_IfconfigBindRequest(NWKU_IfconfigBindRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x0C, req, sizeof(NWKU_IfconfigBindRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t NWKU_IfconfigAllRequest(void *arg, uint8_t fsciInterface)
\brief	Shows all the interfaces with their addresses.

\return	memStatus_t			MEM_SUCCESS_c
***************************************************************************************************/
memStatus_t NWKU_IfconfigAllRequest(void *arg, uint8_t fsciInterface)
{
	FSCI_transmitPayload(arg, 0xCE, 0x0D, NULL, 0, fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t NWKU_PingRequest(NWKU_PingRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Initiates a single ping request.

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t NWKU_PingRequest(NWKU_PingRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x0E, req, sizeof(NWKU_PingRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t NWKU_EchoUDPRequest(NWKU_EchoUDPRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Initiates a single UDP echo request.

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t NWKU_EchoUDPRequest(NWKU_EchoUDPRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x70, req, sizeof(NWKU_EchoUDPRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t NWKU_CPUResetRequest(void *arg, uint8_t fsciInterface)
\brief	Reset CPU

\return	memStatus_t			MEM_SUCCESS_c
***************************************************************************************************/
memStatus_t NWKU_CPUResetRequest(void *arg, uint8_t fsciInterface)
{
	FSCI_transmitPayload(arg, 0xCE, 0x60, NULL, 0, fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t NWKU_CoapCreateInstanceRequest(NWKU_CoapCreateInstanceRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Creates a CoAP instance on a specific UDP port

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t NWKU_CoapCreateInstanceRequest(NWKU_CoapCreateInstanceRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x93, req, sizeof(NWKU_CoapCreateInstanceRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t NWKU_CoapSendRequest(NWKU_CoapSendRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Sends a COAP message

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t NWKU_CoapSendRequest(NWKU_CoapSendRequest_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint8_t);  // InstanceID
	msgLen += sizeof(req->DestinationIpAddress);  // DestinationIpAddress
	msgLen += sizeof(uint16_t);  // UDPPort
	msgLen += sizeof(uint8_t);  // RequestType
	msgLen += sizeof(uint8_t);  // MessageType
	msgLen += 30;  // URIpath
	msgLen += sizeof(uint8_t);  // PayloadLength
	msgLen += req->PayloadLength;  // Payload

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	pMsg[idx] = req->InstanceID; idx++;
	FLib_MemCpy(pMsg + idx, req->DestinationIpAddress, sizeof(req->DestinationIpAddress)); idx += sizeof(req->DestinationIpAddress);
	FLib_MemCpy(pMsg + idx, &(req->UDPPort), sizeof(req->UDPPort)); idx += sizeof(req->UDPPort);
	pMsg[idx] = req->RequestType; idx++;
	pMsg[idx] = req->MessageType; idx++;
	FLib_MemCpy(pMsg + idx, req->URIpath, 30); idx += 30;
	pMsg[idx] = req->PayloadLength; idx++;
	FLib_MemCpy(pMsg + idx, req->Payload, req->PayloadLength); idx += req->PayloadLength;

	/* Send the request */
	FSCI_transmitPayload(arg, 0xCE, 0x90, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t NWKU_CoapRegisterRequest(NWKU_CoapRegisterRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Registers a CoAP URI-path

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t NWKU_CoapRegisterRequest(NWKU_CoapRegisterRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x91, req, sizeof(NWKU_CoapRegisterRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t NWKU_DnsSendRequestRequest(NWKU_DnsSendRequestRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Initiates a DNS request message

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t NWKU_DnsSendRequestRequest(NWKU_DnsSendRequestRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x95, req, sizeof(NWKU_DnsSendRequestRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t NWKU_McastGroupShowRequest(NWKU_McastGroupShowRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Displays the list of joined multicast groups.

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t NWKU_McastGroupShowRequest(NWKU_McastGroupShowRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x72, req, sizeof(NWKU_McastGroupShowRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t NWKU_McastGroupManageRequest(NWKU_McastGroupManageRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Joins or leaves a multicast group.

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t NWKU_McastGroupManageRequest(NWKU_McastGroupManageRequest_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint8_t);  // InterfaceId
	msgLen += sizeof(uint8_t);  // Action
	msgLen += sizeof(bool_t);  // UseTimeout
	msgLen += sizeof(uint8_t);  // McastAddrCount
	msgLen += sizeof(uint32_t);  // Timeout
	msgLen += sizeof(req->DestIpAddress);  // DestIpAddress
	msgLen += req->McastAddrCount;  // McastAddress

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	pMsg[idx] = req->InterfaceId; idx++;
	pMsg[idx] = req->Action; idx++;
	pMsg[idx] = req->UseTimeout; idx++;
	pMsg[idx] = req->McastAddrCount; idx++;
	FLib_MemCpy(pMsg + idx, &(req->Timeout), sizeof(req->Timeout)); idx += sizeof(req->Timeout);
	FLib_MemCpy(pMsg + idx, req->DestIpAddress, sizeof(req->DestIpAddress)); idx += sizeof(req->DestIpAddress);
	FLib_MemCpy(pMsg + idx, req->McastAddress, req->McastAddrCount); idx += req->McastAddrCount;

	/* Send the request */
	FSCI_transmitPayload(arg, 0xCE, 0x71, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t NWKU_FirewallEnableRequest(NWKU_FirewallEnableRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Enable/Disable IPv6 Firewall

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t NWKU_FirewallEnableRequest(NWKU_FirewallEnableRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x77, req, sizeof(NWKU_FirewallEnableRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t NWKU_FirewallTableShowRequest(void *arg, uint8_t fsciInterface)
\brief	Displays the list of IPv6 Firewall rules.

\return	memStatus_t			MEM_SUCCESS_c
***************************************************************************************************/
memStatus_t NWKU_FirewallTableShowRequest(void *arg, uint8_t fsciInterface)
{
	FSCI_transmitPayload(arg, 0xCE, 0x7A, NULL, 0, fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t NWKU_FirewallRuleRemoveRequest(NWKU_FirewallRuleRemoveRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Remove an existing IPv6 Firewall rule

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t NWKU_FirewallRuleRemoveRequest(NWKU_FirewallRuleRemoveRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x79, req, sizeof(NWKU_FirewallRuleRemoveRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t NWKU_FirewallRuleAddRequest(NWKU_FirewallRuleAddRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Adds an IPv6 Firewall rule

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t NWKU_FirewallRuleAddRequest(NWKU_FirewallRuleAddRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x78, req, sizeof(NWKU_FirewallRuleAddRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t NWKU_RoutesShowRequest(void *arg, uint8_t fsciInterface)
\brief	Displays the list of IPv6 routes.

\return	memStatus_t			MEM_SUCCESS_c
***************************************************************************************************/
memStatus_t NWKU_RoutesShowRequest(void *arg, uint8_t fsciInterface)
{
	FSCI_transmitPayload(arg, 0xCE, 0x74, NULL, 0, fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t NWKU_RoutesManageRequest(NWKU_RoutesManageRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Adds or Removes an IPv6 route.

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t NWKU_RoutesManageRequest(NWKU_RoutesManageRequest_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint8_t);  // Action
	msgLen += sizeof(req->Destination);  // Destination
	msgLen += sizeof(req->NextHop);  // NextHop

	switch (req->Action)
	{
		case NWKU_RoutesManageRequest_Action_Add:
			msgLen += sizeof(uint8_t);  // PrefixLength
			msgLen += sizeof(uint8_t);  // RouteMetric
			msgLen += sizeof(uint8_t);  // InstanceId
			break;

		case NWKU_RoutesManageRequest_Action_Remove:
			break;
	}	// RouteInfo

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	pMsg[idx] = req->Action; idx++;
	FLib_MemCpy(pMsg + idx, req->Destination, sizeof(req->Destination)); idx += sizeof(req->Destination);
	FLib_MemCpy(pMsg + idx, req->NextHop, sizeof(req->NextHop)); idx += sizeof(req->NextHop);

	switch (req->Action)
	{
		case NWKU_RoutesManageRequest_Action_Add:
			pMsg[idx] = req->RouteInfo.Add.PrefixLength; idx++;
			pMsg[idx] = req->RouteInfo.Add.RouteMetric; idx++;
			pMsg[idx] = req->RouteInfo.Add.InstanceId; idx++;
			break;

		case NWKU_RoutesManageRequest_Action_Remove:
			break;
	}

	/* Send the request */
	FSCI_transmitPayload(arg, 0xCE, 0x73, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

#endif  /* IPUTILS_ENABLE */

#if DTLS_ENABLE
/*!*************************************************************************************************
\fn		memStatus_t DTLSOpenRequest(DTLSOpenRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	DTLS Open

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t DTLSOpenRequest(DTLSOpenRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x30, req, sizeof(DTLSOpenRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t DTLSCloseRequest(DTLSCloseRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	DTLS Close

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t DTLSCloseRequest(DTLSCloseRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x31, req, sizeof(DTLSCloseRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t DTLSClosePeerRequest(DTLSClosePeerRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	DTLS Close Peer

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t DTLSClosePeerRequest(DTLSClosePeerRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x32, req, sizeof(DTLSClosePeerRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t DTLSConnectRequest(DTLSConnectRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	DTLS Connect

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t DTLSConnectRequest(DTLSConnectRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0x33, req, sizeof(DTLSConnectRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t DTLSSendRequest(DTLSSendRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	DTLS Send

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t DTLSSendRequest(DTLSSendRequest_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint8_t);  // PeerNumber
	msgLen += sizeof(uint16_t);  // Size
	msgLen += req->Size;  // Data

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	pMsg[idx] = req->PeerNumber; idx++;
	FLib_MemCpy(pMsg + idx, &(req->Size), sizeof(req->Size)); idx += sizeof(req->Size);
	FLib_MemCpy(pMsg + idx, req->Data, req->Size); idx += req->Size;

	/* Send the request */
	FSCI_transmitPayload(arg, 0xCE, 0x35, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

#endif  /* DTLS_ENABLE */

#if OTHERUTILS_ENABLE
/*!*************************************************************************************************
\fn		memStatus_t FSCICPUResetRequest(void *arg, uint8_t fsciInterface)
\brief	Reset CPU

\return	memStatus_t			MEM_SUCCESS_c
***************************************************************************************************/
memStatus_t FSCICPUResetRequest(void *arg, uint8_t fsciInterface)
{
	FSCI_transmitPayload(arg, 0xA3, 0x08, NULL, 0, fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t FSCIGetUniqueIdRequest(void *arg, uint8_t fsciInterface)
\brief	Read the Unique ID for every module present

\return	memStatus_t			MEM_SUCCESS_c
***************************************************************************************************/
memStatus_t FSCIGetUniqueIdRequest(void *arg, uint8_t fsciInterface)
{
	FSCI_transmitPayload(arg, 0xA3, 0xB0, NULL, 0, fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t FSCIGetMcuIdRequest(void *arg, uint8_t fsciInterface)
\brief	Read the MCU Id for every module present

\return	memStatus_t			MEM_SUCCESS_c
***************************************************************************************************/
memStatus_t FSCIGetMcuIdRequest(void *arg, uint8_t fsciInterface)
{
	FSCI_transmitPayload(arg, 0xA3, 0xB1, NULL, 0, fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t FSCIGetSwVersionsRequest(void *arg, uint8_t fsciInterface)
\brief	Read the SW version for every module present

\return	memStatus_t			MEM_SUCCESS_c
***************************************************************************************************/
memStatus_t FSCIGetSwVersionsRequest(void *arg, uint8_t fsciInterface)
{
	FSCI_transmitPayload(arg, 0xA3, 0xB2, NULL, 0, fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t SerialTun_LinkIndicationRequest(SerialTun_LinkIndicationRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Inform the black-box of TUN/TAP link changes

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t SerialTun_LinkIndicationRequest(SerialTun_LinkIndicationRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xCE, 0xF1, req, sizeof(SerialTun_LinkIndicationRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t SerialTun_IPPacketSendRequest(SerialTun_IPPacketSendRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Send an IP packet over serial TUN interface

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t SerialTun_IPPacketSendRequest(SerialTun_IPPacketSendRequest_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint16_t);  // Size
	msgLen += req->Size;  // Data

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	FLib_MemCpy(pMsg + idx, &(req->Size), sizeof(req->Size)); idx += sizeof(req->Size);
	FLib_MemCpy(pMsg + idx, req->Data, req->Size); idx += req->Size;

	/* Send the request */
	FSCI_transmitPayload(arg, 0xCE, 0xF2, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t Sniffer_MacSetPIBAttributeRequest(Sniffer_MacSetPIBAttributeRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	Sniffer_MacSetPIBAttribute.Request description

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t Sniffer_MacSetPIBAttributeRequest(Sniffer_MacSetPIBAttributeRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x85, 0x09, req, sizeof(Sniffer_MacSetPIBAttributeRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t FSCIACK(FSCIACK_t *req, void *arg, uint8_t fsciInterface)
\brief	FSCI ACK Packet

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t FSCIACK(FSCIACK_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0xA4, 0xFD, req, sizeof(FSCIACK_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t FSCIEnableMSDBootloaderRequest(void *arg, uint8_t fsciInterface)
\brief	Enable MSD Bootloader

\return	memStatus_t			MEM_SUCCESS_c
***************************************************************************************************/
memStatus_t FSCIEnableMSDBootloaderRequest(void *arg, uint8_t fsciInterface)
{
	FSCI_transmitPayload(arg, 0xA3, 0xCF, NULL, 0, fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t AspSetPowerLevelRequest(AspSetPowerLevelRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	AspSetPowerLevel.Request description

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t AspSetPowerLevelRequest(AspSetPowerLevelRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x95, 0x0F, req, sizeof(AspSetPowerLevelRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t AspGetPowerLevelRequest(void *arg, uint8_t fsciInterface)
\brief	AspGetPowerLevel.Request description

\return	memStatus_t			MEM_SUCCESS_c
***************************************************************************************************/
memStatus_t AspGetPowerLevelRequest(void *arg, uint8_t fsciInterface)
{
	FSCI_transmitPayload(arg, 0x95, 0x1F, NULL, 0, fsciInterface);
	return MEM_SUCCESS_c;
}

#endif  /* OTHERUTILS_ENABLE */
