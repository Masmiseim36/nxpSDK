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
/*==================================================================================================
Include Files
==================================================================================================*/
#include "cmd_zigbee3_0_6.0.7.h"

/*==================================================================================================
Public Functions
==================================================================================================*/
#if ZIGBEESTACKANDNODEMANAGEMENTCOMMANDS_ENABLE
/*!*************************************************************************************************
\fn		memStatus_t GetVersion(void *arg, uint8_t fsciInterface)
\brief	Node Command ID List - sent by Gateway node after reset.

\return	memStatus_t			MEM_SUCCESS_c
***************************************************************************************************/
memStatus_t GetVersion(void *arg, uint8_t fsciInterface)
{
	FSCI_transmitPayload(arg, 0x37, 0x10, NULL, 0, fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t SetExtendedPANID(SetExtendedPANID_t *req, void *arg, uint8_t fsciInterface)
\brief	Node Command ID List - sent by Gateway node after reset.

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t SetExtendedPANID(SetExtendedPANID_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0x20, req, sizeof(SetExtendedPANID_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t SetChannelMask(SetChannelMask_t *req, void *arg, uint8_t fsciInterface)
\brief	Node Command ID List - sent by Gateway node after reset.

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t SetChannelMask(SetChannelMask_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0x21, req, sizeof(SetChannelMask_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t SetSecurityStateKey(SetSecurityStateKey_t *req, void *arg, uint8_t fsciInterface)
\brief	Node Command ID List - sent by Gateway node after reset.

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t SetSecurityStateKey(SetSecurityStateKey_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0x22, req, sizeof(SetSecurityStateKey_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t SetDeviceType(SetDeviceType_t *req, void *arg, uint8_t fsciInterface)
\brief	Node Command ID List - sent by Gateway node after reset.

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t SetDeviceType(SetDeviceType_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0x23, req, sizeof(SetDeviceType_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t StartNetworkMessage(void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c
***************************************************************************************************/
memStatus_t StartNetworkMessage(void *arg, uint8_t fsciInterface)
{
	FSCI_transmitPayload(arg, 0x37, 0x24, NULL, 0, fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t StartNetworkScan(void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c
***************************************************************************************************/
memStatus_t StartNetworkScan(void *arg, uint8_t fsciInterface)
{
	FSCI_transmitPayload(arg, 0x37, 0x25, NULL, 0, fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t Code(Code_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t Code(Code_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0x29, req, sizeof(Code_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t ZLO_ZLLFactoryNewReset(void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c
***************************************************************************************************/
memStatus_t ZLO_ZLLFactoryNewReset(void *arg, uint8_t fsciInterface)
{
	FSCI_transmitPayload(arg, 0x37, 0x13, NULL, 0, fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t PermitJoinStatusOnTheTarget(void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c
***************************************************************************************************/
memStatus_t PermitJoinStatusOnTheTarget(void *arg, uint8_t fsciInterface)
{
	FSCI_transmitPayload(arg, 0x37, 0x14, NULL, 0, fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t Reset(void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c
***************************************************************************************************/
memStatus_t Reset(void *arg, uint8_t fsciInterface)
{
	FSCI_transmitPayload(arg, 0x37, 0x11, NULL, 0, fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t ErasePersistentData(void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c
***************************************************************************************************/
memStatus_t ErasePersistentData(void *arg, uint8_t fsciInterface)
{
	FSCI_transmitPayload(arg, 0x37, 0x12, NULL, 0, fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t Bind(Bind_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t Bind(Bind_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint64_t);  // TargetExtendedAddress
	msgLen += sizeof(uint8_t);  // TargetEndPoint
	msgLen += sizeof(uint16_t);  // ClusterID
	msgLen += sizeof(uint8_t);  // DestAddressingMode

	switch (req->DestAddressingMode)
	{
		case Bind_DestAddressingMode_NO_PAN:
			// NO_PAN
			break;

		case Bind_DestAddressingMode_Reserved:
			// Reserved
			break;

		case Bind_DestAddressingMode_Address_16b:
			msgLen += sizeof(uint16_t);  // Address_16b
			break;

		case Bind_DestAddressingMode_Address_64b:
			msgLen += sizeof(uint64_t);  // Address_64b
			break;
	}	// DestinationAddress
	msgLen += sizeof(uint8_t);  // DestinationEndPoint

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	FLib_MemCpy(pMsg + idx, &(req->TargetExtendedAddress), sizeof(req->TargetExtendedAddress)); idx += sizeof(req->TargetExtendedAddress);
	pMsg[idx] = req->TargetEndPoint; idx++;
	FLib_MemCpy(pMsg + idx, &(req->ClusterID), sizeof(req->ClusterID)); idx += sizeof(req->ClusterID);
	pMsg[idx] = req->DestAddressingMode; idx++;

	switch (req->DestAddressingMode)
	{
		case Bind_DestAddressingMode_NO_PAN:
			break;

		case Bind_DestAddressingMode_Reserved:
			break;

		case Bind_DestAddressingMode_Address_16b:
			FLib_MemCpy(pMsg + idx, &(req->DestinationAddress.Address_16b), sizeof(req->DestinationAddress.Address_16b)); idx += sizeof(req->DestinationAddress.Address_16b);
			break;

		case Bind_DestAddressingMode_Address_64b:
			FLib_MemCpy(pMsg + idx, &(req->DestinationAddress.Address_64b), sizeof(req->DestinationAddress.Address_64b)); idx += sizeof(req->DestinationAddress.Address_64b);
			break;
	}
	pMsg[idx] = req->DestinationEndPoint; idx++;

	/* Send the request */
	FSCI_transmitPayload(arg, 0x37, 0x30, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t Unbind(Unbind_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t Unbind(Unbind_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint64_t);  // TargetExtendedAddress
	msgLen += sizeof(uint8_t);  // TargetEndPoint
	msgLen += sizeof(uint16_t);  // ClusterID
	msgLen += sizeof(uint8_t);  // DestAddressingMode

	switch (req->DestAddressingMode)
	{
		case Unbind_DestAddressingMode_NO_PAN:
			// NO_PAN
			break;

		case Unbind_DestAddressingMode_Reserved:
			// Reserved
			break;

		case Unbind_DestAddressingMode_Address_16b:
			msgLen += sizeof(uint16_t);  // Address_16b
			break;

		case Unbind_DestAddressingMode_Address_64b:
			msgLen += sizeof(uint64_t);  // Address_64b
			break;
	}	// DestinationAddress
	msgLen += sizeof(uint8_t);  // DestinationEndPoint

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	FLib_MemCpy(pMsg + idx, &(req->TargetExtendedAddress), sizeof(req->TargetExtendedAddress)); idx += sizeof(req->TargetExtendedAddress);
	pMsg[idx] = req->TargetEndPoint; idx++;
	FLib_MemCpy(pMsg + idx, &(req->ClusterID), sizeof(req->ClusterID)); idx += sizeof(req->ClusterID);
	pMsg[idx] = req->DestAddressingMode; idx++;

	switch (req->DestAddressingMode)
	{
		case Unbind_DestAddressingMode_NO_PAN:
			break;

		case Unbind_DestAddressingMode_Reserved:
			break;

		case Unbind_DestAddressingMode_Address_16b:
			FLib_MemCpy(pMsg + idx, &(req->DestinationAddress.Address_16b), sizeof(req->DestinationAddress.Address_16b)); idx += sizeof(req->DestinationAddress.Address_16b);
			break;

		case Unbind_DestAddressingMode_Address_64b:
			FLib_MemCpy(pMsg + idx, &(req->DestinationAddress.Address_64b), sizeof(req->DestinationAddress.Address_64b)); idx += sizeof(req->DestinationAddress.Address_64b);
			break;
	}
	pMsg[idx] = req->DestinationEndPoint; idx++;

	/* Send the request */
	FSCI_transmitPayload(arg, 0x37, 0x31, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t NetworkAddressRequest(NetworkAddressRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t NetworkAddressRequest(NetworkAddressRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0x40, req, sizeof(NetworkAddressRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t IEEEAddressRequest(IEEEAddressRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t IEEEAddressRequest(IEEEAddressRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0x41, req, sizeof(IEEEAddressRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t NodeDescriptorRequest(NodeDescriptorRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t NodeDescriptorRequest(NodeDescriptorRequest_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint8_t);  // SequenceNumber
	msgLen += sizeof(uint8_t);  // Status
	msgLen += sizeof(uint16_t);  // NetworkAddress
	msgLen += sizeof(uint16_t);  // ManufacturerCode
	msgLen += sizeof(uint16_t);  // MaxRxSize
	msgLen += sizeof(uint16_t);  // MaxTxSize
	msgLen += 1;  // AllocateAddress|NetworkManager|BackupDiscoveryCache|PrimaryDiscoveryCache|BackupBindingCache|PrimaryBindingCache|BackUpTrustCenter|PrimaryTrustCenter  // ServerMask
	msgLen += 1;  // Reserved|ExtendedSimpleDdescriptorList|ExtendedActiveEndPointList  // DescriptorCapability
	msgLen += 1;  // AllocateAddress|SecurityCapability|Reserved|ReceiverOnWhenIdle|PowerSource|DeviceType|AlternatePANCoordinator  // MacCapability
	msgLen += sizeof(uint8_t);  // MaxBufferSize
	msgLen += 2;  // FrequencyBand|APSFlags|Reserved|UserDescriptorAvailable|ComplexDescriptorAvailable|EndDevice|Router|Coordinator  // BitFields

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	pMsg[idx] = req->SequenceNumber; idx++;
	pMsg[idx] = req->Status; idx++;
	FLib_MemCpy(pMsg + idx, &(req->NetworkAddress), sizeof(req->NetworkAddress)); idx += sizeof(req->NetworkAddress);
	FLib_MemCpy(pMsg + idx, &(req->ManufacturerCode), sizeof(req->ManufacturerCode)); idx += sizeof(req->ManufacturerCode);
	FLib_MemCpy(pMsg + idx, &(req->MaxRxSize), sizeof(req->MaxRxSize)); idx += sizeof(req->MaxRxSize);
	FLib_MemCpy(pMsg + idx, &(req->MaxTxSize), sizeof(req->MaxTxSize)); idx += sizeof(req->MaxTxSize);
	pMsg[idx] = req->ServerMask; idx++;
	pMsg[idx] = req->DescriptorCapability; idx++;
	pMsg[idx] = req->MacCapability; idx++;
	pMsg[idx] = req->MaxBufferSize; idx++;
	FLib_MemCpy(pMsg + idx, &(req->BitFields), 2); idx += 2;

	/* Send the request */
	FSCI_transmitPayload(arg, 0x37, 0x42, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t SimpleDescriptorRequest(SimpleDescriptorRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t SimpleDescriptorRequest(SimpleDescriptorRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0x43, req, sizeof(SimpleDescriptorRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t PowerDescriptorRequest(PowerDescriptorRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t PowerDescriptorRequest(PowerDescriptorRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0x44, req, sizeof(PowerDescriptorRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t ActiveEndpointRequest(ActiveEndpointRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t ActiveEndpointRequest(ActiveEndpointRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0x45, req, sizeof(ActiveEndpointRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MatchDescriptorRequest(MatchDescriptorRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MatchDescriptorRequest(MatchDescriptorRequest_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint16_t);  // TargetShortAddress
	msgLen += sizeof(uint16_t);  // ProfileID
	msgLen += sizeof(uint8_t);  // NumberOfInputClusters
	msgLen += req->NumberOfInputClusters * sizeof(uint16_t);  // InputClusterList
	msgLen += sizeof(uint8_t);  // NumberOfOutputClusters
	msgLen += req->NumberOfOutputClusters * sizeof(uint16_t);  // OutputClusterList

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	FLib_MemCpy(pMsg + idx, &(req->TargetShortAddress), sizeof(req->TargetShortAddress)); idx += sizeof(req->TargetShortAddress);
	FLib_MemCpy(pMsg + idx, &(req->ProfileID), sizeof(req->ProfileID)); idx += sizeof(req->ProfileID);
	pMsg[idx] = req->NumberOfInputClusters; idx++;
	FLib_MemCpy(pMsg + idx, req->InputClusterList, req->NumberOfInputClusters * sizeof(uint16_t)); idx += req->NumberOfInputClusters * sizeof(uint16_t);
	pMsg[idx] = req->NumberOfOutputClusters; idx++;
	FLib_MemCpy(pMsg + idx, req->OutputClusterList, req->NumberOfOutputClusters * sizeof(uint16_t)); idx += req->NumberOfOutputClusters * sizeof(uint16_t);

	/* Send the request */
	FSCI_transmitPayload(arg, 0x37, 0x46, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t RemoveDevice(RemoveDevice_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t RemoveDevice(RemoveDevice_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0x26, req, sizeof(RemoveDevice_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t UserDescriptorSet(UserDescriptorSet_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t UserDescriptorSet(UserDescriptorSet_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint16_t);  // TargetShortAddress
	msgLen += sizeof(uint16_t);  // AddressOfInterest
	msgLen += sizeof(uint8_t);  // StringLength
	msgLen += req->StringLength;  // Parameters

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	FLib_MemCpy(pMsg + idx, &(req->TargetShortAddress), sizeof(req->TargetShortAddress)); idx += sizeof(req->TargetShortAddress);
	FLib_MemCpy(pMsg + idx, &(req->AddressOfInterest), sizeof(req->AddressOfInterest)); idx += sizeof(req->AddressOfInterest);
	pMsg[idx] = req->StringLength; idx++;
	FLib_MemCpy(pMsg + idx, req->Parameters, req->StringLength); idx += req->StringLength;

	/* Send the request */
	FSCI_transmitPayload(arg, 0x37, 0x2B, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t UserDescriptorRequest(UserDescriptorRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t UserDescriptorRequest(UserDescriptorRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0x2C, req, sizeof(UserDescriptorRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t SetPollIntervalRequest(SetPollIntervalRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t SetPollIntervalRequest(SetPollIntervalRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0x2D, req, sizeof(SetPollIntervalRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t ComplexDescriptorRequest(ComplexDescriptorRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t ComplexDescriptorRequest(ComplexDescriptorRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0x34, req, sizeof(ComplexDescriptorRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t ManagementLeaveRequest(ManagementLeaveRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t ManagementLeaveRequest(ManagementLeaveRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0x47, req, sizeof(ManagementLeaveRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t PermitJoiningRequest(PermitJoiningRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t PermitJoiningRequest(PermitJoiningRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0x49, req, sizeof(PermitJoiningRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t ManagementNetworkUpdateRequest(ManagementNetworkUpdateRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t ManagementNetworkUpdateRequest(ManagementNetworkUpdateRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0x4A, req, sizeof(ManagementNetworkUpdateRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t SystemServerDiscoveryRequest(SystemServerDiscoveryRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t SystemServerDiscoveryRequest(SystemServerDiscoveryRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0x4B, req, sizeof(SystemServerDiscoveryRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t ManagementLQIRequest(ManagementLQIRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t ManagementLQIRequest(ManagementLQIRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0x4E, req, sizeof(ManagementLQIRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

#endif  /* ZIGBEESTACKANDNODEMANAGEMENTCOMMANDS_ENABLE */

#if ENTIREPROFILE_ENABLE
/*!*************************************************************************************************
\fn		memStatus_t ReadAttributeRequest(ReadAttributeRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t ReadAttributeRequest(ReadAttributeRequest_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint8_t);  // AddressMode
	msgLen += sizeof(uint16_t);  // TargetShortAddress
	msgLen += sizeof(uint8_t);  // SourceEndPoint
	msgLen += sizeof(uint8_t);  // DestinationEndPoint
	msgLen += sizeof(uint16_t);  // ClusterID
	msgLen += sizeof(uint8_t);  // Direction
	msgLen += sizeof(uint8_t);  // ManufacturerSpecific
	msgLen += sizeof(uint16_t);  // ManufacturerID
	msgLen += sizeof(uint8_t);  // NumberOfAttributes
	msgLen += req->NumberOfAttributes * sizeof(uint16_t);  // AttributesList

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	pMsg[idx] = req->AddressMode; idx++;
	FLib_MemCpy(pMsg + idx, &(req->TargetShortAddress), sizeof(req->TargetShortAddress)); idx += sizeof(req->TargetShortAddress);
	pMsg[idx] = req->SourceEndPoint; idx++;
	pMsg[idx] = req->DestinationEndPoint; idx++;
	FLib_MemCpy(pMsg + idx, &(req->ClusterID), sizeof(req->ClusterID)); idx += sizeof(req->ClusterID);
	pMsg[idx] = req->Direction; idx++;
	pMsg[idx] = req->ManufacturerSpecific; idx++;
	FLib_MemCpy(pMsg + idx, &(req->ManufacturerID), sizeof(req->ManufacturerID)); idx += sizeof(req->ManufacturerID);
	pMsg[idx] = req->NumberOfAttributes; idx++;
	FLib_MemCpy(pMsg + idx, req->AttributesList, req->NumberOfAttributes * sizeof(uint16_t)); idx += req->NumberOfAttributes * sizeof(uint16_t);

	/* Send the request */
	FSCI_transmitPayload(arg, 0x38, 0x00, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t WriteAttributeRequest(WriteAttributeRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t WriteAttributeRequest(WriteAttributeRequest_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint8_t);  // AddressMode
	msgLen += sizeof(uint16_t);  // TargetShortAddress
	msgLen += sizeof(uint8_t);  // SourceEndPoint
	msgLen += sizeof(uint8_t);  // DestinationEndPoint
	msgLen += sizeof(uint16_t);  // ClusterID
	msgLen += sizeof(uint8_t);  // Direction
	msgLen += sizeof(uint8_t);  // ManufacturerSpecific
	msgLen += sizeof(uint16_t);  // ManufacturerID
	msgLen += sizeof(uint8_t);  // NumberOfAttributes
	msgLen += req->NumberOfAttributes * sizeof(uint16_t);  // AttributesList

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	pMsg[idx] = req->AddressMode; idx++;
	FLib_MemCpy(pMsg + idx, &(req->TargetShortAddress), sizeof(req->TargetShortAddress)); idx += sizeof(req->TargetShortAddress);
	pMsg[idx] = req->SourceEndPoint; idx++;
	pMsg[idx] = req->DestinationEndPoint; idx++;
	FLib_MemCpy(pMsg + idx, &(req->ClusterID), sizeof(req->ClusterID)); idx += sizeof(req->ClusterID);
	pMsg[idx] = req->Direction; idx++;
	pMsg[idx] = req->ManufacturerSpecific; idx++;
	FLib_MemCpy(pMsg + idx, &(req->ManufacturerID), sizeof(req->ManufacturerID)); idx += sizeof(req->ManufacturerID);
	pMsg[idx] = req->NumberOfAttributes; idx++;
	FLib_MemCpy(pMsg + idx, req->AttributesList, req->NumberOfAttributes * sizeof(uint16_t)); idx += req->NumberOfAttributes * sizeof(uint16_t);

	/* Send the request */
	FSCI_transmitPayload(arg, 0x38, 0x10, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t AttributeDiscoveryRequest(AttributeDiscoveryRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t AttributeDiscoveryRequest(AttributeDiscoveryRequest_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint8_t);  // AddressMode
	msgLen += sizeof(uint16_t);  // TargetShortAddress
	msgLen += sizeof(uint8_t);  // SourceEndPoint
	msgLen += sizeof(uint8_t);  // DestinationEndPoint
	msgLen += sizeof(uint16_t);  // ClusterID
	msgLen += sizeof(uint16_t);  // AttributeID
	msgLen += sizeof(uint8_t);  // Direction
	msgLen += sizeof(uint8_t);  // ManufacturerSpecific
	msgLen += sizeof(uint16_t);  // ManufacturerID
	msgLen += sizeof(uint8_t);  // NumberOfAttributes
	msgLen += req->NumberOfAttributes;  // MaxNumberOfIdentifiers

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	pMsg[idx] = req->AddressMode; idx++;
	FLib_MemCpy(pMsg + idx, &(req->TargetShortAddress), sizeof(req->TargetShortAddress)); idx += sizeof(req->TargetShortAddress);
	pMsg[idx] = req->SourceEndPoint; idx++;
	pMsg[idx] = req->DestinationEndPoint; idx++;
	FLib_MemCpy(pMsg + idx, &(req->ClusterID), sizeof(req->ClusterID)); idx += sizeof(req->ClusterID);
	FLib_MemCpy(pMsg + idx, &(req->AttributeID), sizeof(req->AttributeID)); idx += sizeof(req->AttributeID);
	pMsg[idx] = req->Direction; idx++;
	pMsg[idx] = req->ManufacturerSpecific; idx++;
	FLib_MemCpy(pMsg + idx, &(req->ManufacturerID), sizeof(req->ManufacturerID)); idx += sizeof(req->ManufacturerID);
	pMsg[idx] = req->NumberOfAttributes; idx++;
	FLib_MemCpy(pMsg + idx, req->MaxNumberOfIdentifiers, req->NumberOfAttributes); idx += req->NumberOfAttributes;

	/* Send the request */
	FSCI_transmitPayload(arg, 0x38, 0x40, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t EnablePermissionsControlledJoins(EnablePermissionsControlledJoins_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t EnablePermissionsControlledJoins(EnablePermissionsControlledJoins_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0x27, req, sizeof(EnablePermissionsControlledJoins_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t AuthenticateDevice(AuthenticateDevice_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t AuthenticateDevice(AuthenticateDevice_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0x28, req, sizeof(AuthenticateDevice_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t ConfigureReportingRequest(ConfigureReportingRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t ConfigureReportingRequest(ConfigureReportingRequest_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint8_t);  // AddressMode
	msgLen += sizeof(uint16_t);  // TargetShortAddress
	msgLen += sizeof(uint8_t);  // SourceEndPoint
	msgLen += sizeof(uint8_t);  // DestinationEndpoint
	msgLen += sizeof(uint16_t);  // ClusterID
	msgLen += sizeof(uint8_t);  // Direction
	msgLen += sizeof(uint8_t);  // ManufacturerSpecific
	msgLen += sizeof(uint16_t);  // ManufacturerID
	msgLen += sizeof(uint8_t);  // NumberOfAttributes
	msgLen += req->NumberOfAttributes;  // AttributesList

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	pMsg[idx] = req->AddressMode; idx++;
	FLib_MemCpy(pMsg + idx, &(req->TargetShortAddress), sizeof(req->TargetShortAddress)); idx += sizeof(req->TargetShortAddress);
	pMsg[idx] = req->SourceEndPoint; idx++;
	pMsg[idx] = req->DestinationEndpoint; idx++;
	FLib_MemCpy(pMsg + idx, &(req->ClusterID), sizeof(req->ClusterID)); idx += sizeof(req->ClusterID);
	pMsg[idx] = req->Direction; idx++;
	pMsg[idx] = req->ManufacturerSpecific; idx++;
	FLib_MemCpy(pMsg + idx, &(req->ManufacturerID), sizeof(req->ManufacturerID)); idx += sizeof(req->ManufacturerID);
	pMsg[idx] = req->NumberOfAttributes; idx++;
	FLib_MemCpy(pMsg + idx, req->AttributesList, req->NumberOfAttributes); idx += req->NumberOfAttributes;

	/* Send the request */
	FSCI_transmitPayload(arg, 0x38, 0x20, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

#endif  /* ENTIREPROFILE_ENABLE */

#if GROUPCLUSTERCOMMANDS_ENABLE
/*!*************************************************************************************************
\fn		memStatus_t AddGroup(AddGroup_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t AddGroup(AddGroup_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0x60, req, sizeof(AddGroup_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t ViewGroup(ViewGroup_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t ViewGroup(ViewGroup_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0x61, req, sizeof(ViewGroup_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t GetGroupMembership(GetGroupMembership_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t GetGroupMembership(GetGroupMembership_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint8_t);  // AddressMode
	msgLen += sizeof(uint16_t);  // TargetShortAddress
	msgLen += sizeof(uint8_t);  // SourceEndPoint
	msgLen += sizeof(uint8_t);  // DestinationEndPoint
	msgLen += sizeof(uint8_t);  // GroupCount
	msgLen += req->GroupCount * sizeof(uint16_t);  // GroupList

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	pMsg[idx] = req->AddressMode; idx++;
	FLib_MemCpy(pMsg + idx, &(req->TargetShortAddress), sizeof(req->TargetShortAddress)); idx += sizeof(req->TargetShortAddress);
	pMsg[idx] = req->SourceEndPoint; idx++;
	pMsg[idx] = req->DestinationEndPoint; idx++;
	pMsg[idx] = req->GroupCount; idx++;
	FLib_MemCpy(pMsg + idx, req->GroupList, req->GroupCount * sizeof(uint16_t)); idx += req->GroupCount * sizeof(uint16_t);

	/* Send the request */
	FSCI_transmitPayload(arg, 0x37, 0x62, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t RemoveGroup(RemoveGroup_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t RemoveGroup(RemoveGroup_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0x63, req, sizeof(RemoveGroup_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t RemoveAllGroups(RemoveAllGroups_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t RemoveAllGroups(RemoveAllGroups_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0x64, req, sizeof(RemoveAllGroups_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t AddGroupIfIdentify(AddGroupIfIdentify_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t AddGroupIfIdentify(AddGroupIfIdentify_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0x65, req, sizeof(AddGroupIfIdentify_t), fsciInterface);
	return MEM_SUCCESS_c;
}

#endif  /* GROUPCLUSTERCOMMANDS_ENABLE */

#if IDENTIFYCLUSTERCOMMANDS_ENABLE
/*!*************************************************************************************************
\fn		memStatus_t IdentifySend(IdentifySend_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t IdentifySend(IdentifySend_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0x70, req, sizeof(IdentifySend_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t IdentifyQuery(IdentifyQuery_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t IdentifyQuery(IdentifyQuery_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0x71, req, sizeof(IdentifyQuery_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t IdentifyTriggerEffects(IdentifyTriggerEffects_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t IdentifyTriggerEffects(IdentifyTriggerEffects_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0xE0, req, sizeof(IdentifyTriggerEffects_t), fsciInterface);
	return MEM_SUCCESS_c;
}

#endif  /* IDENTIFYCLUSTERCOMMANDS_ENABLE */

#if LEVELCLUSTERCOMMANDS_ENABLE
/*!*************************************************************************************************
\fn		memStatus_t MoveToLevel(MoveToLevel_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MoveToLevel(MoveToLevel_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0x80, req, sizeof(MoveToLevel_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MoveToLevelWithWithoutOnOff(MoveToLevelWithWithoutOnOff_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MoveToLevelWithWithoutOnOff(MoveToLevelWithWithoutOnOff_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0x81, req, sizeof(MoveToLevelWithWithoutOnOff_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MoveStep(MoveStep_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MoveStep(MoveStep_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0x82, req, sizeof(MoveStep_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MoveStopWithOnOff(MoveStopWithOnOff_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MoveStopWithOnOff(MoveStopWithOnOff_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0x84, req, sizeof(MoveStopWithOnOff_t), fsciInterface);
	return MEM_SUCCESS_c;
}

#endif  /* LEVELCLUSTERCOMMANDS_ENABLE */

#if ONOFFCLUSTERCOMMANDS_ENABLE
/*!*************************************************************************************************
\fn		memStatus_t OnOffWithEffectsSend(OnOffWithEffectsSend_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t OnOffWithEffectsSend(OnOffWithEffectsSend_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0x94, req, sizeof(OnOffWithEffectsSend_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t OnOffWithNoEffects(OnOffWithNoEffects_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t OnOffWithNoEffects(OnOffWithNoEffects_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0x92, req, sizeof(OnOffWithNoEffects_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t OnOffTimedSend(OnOffTimedSend_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t OnOffTimedSend(OnOffTimedSend_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0x93, req, sizeof(OnOffTimedSend_t), fsciInterface);
	return MEM_SUCCESS_c;
}

#endif  /* ONOFFCLUSTERCOMMANDS_ENABLE */

#if SCENESCLUSTERCOMMANDS_ENABLE
/*!*************************************************************************************************
\fn		memStatus_t ViewScene(ViewScene_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t ViewScene(ViewScene_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0xA0, req, sizeof(ViewScene_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t AddScene(AddScene_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t AddScene(AddScene_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint8_t);  // AddressMode
	msgLen += sizeof(uint16_t);  // TargetShortAddress
	msgLen += sizeof(uint8_t);  // SourceEndPoint
	msgLen += sizeof(uint8_t);  // DestinationEndPoint
	msgLen += sizeof(uint16_t);  // GroupID
	msgLen += sizeof(uint8_t);  // SceneID
	msgLen += sizeof(uint16_t);  // TransitionTime
	msgLen += sizeof(uint8_t);  // SceneNameLength
	msgLen += sizeof(uint8_t);  // SceneNameMaxLength
	msgLen += req->SceneNameLength;  // SceneNameData

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	pMsg[idx] = req->AddressMode; idx++;
	FLib_MemCpy(pMsg + idx, &(req->TargetShortAddress), sizeof(req->TargetShortAddress)); idx += sizeof(req->TargetShortAddress);
	pMsg[idx] = req->SourceEndPoint; idx++;
	pMsg[idx] = req->DestinationEndPoint; idx++;
	FLib_MemCpy(pMsg + idx, &(req->GroupID), sizeof(req->GroupID)); idx += sizeof(req->GroupID);
	pMsg[idx] = req->SceneID; idx++;
	FLib_MemCpy(pMsg + idx, &(req->TransitionTime), sizeof(req->TransitionTime)); idx += sizeof(req->TransitionTime);
	pMsg[idx] = req->SceneNameLength; idx++;
	pMsg[idx] = req->SceneNameMaxLength; idx++;
	FLib_MemCpy(pMsg + idx, req->SceneNameData, req->SceneNameLength); idx += req->SceneNameLength;

	/* Send the request */
	FSCI_transmitPayload(arg, 0x37, 0xA1, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t RemoveScene(RemoveScene_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t RemoveScene(RemoveScene_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0xA2, req, sizeof(RemoveScene_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t RemoveAllScenes(RemoveAllScenes_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t RemoveAllScenes(RemoveAllScenes_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0xA3, req, sizeof(RemoveAllScenes_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t StoreScene(StoreScene_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t StoreScene(StoreScene_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0xA4, req, sizeof(StoreScene_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t RecallScene(RecallScene_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t RecallScene(RecallScene_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0xA5, req, sizeof(RecallScene_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t SceneMembershipRequest(SceneMembershipRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t SceneMembershipRequest(SceneMembershipRequest_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0xA6, req, sizeof(SceneMembershipRequest_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t AddEnhancedScene(AddEnhancedScene_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t AddEnhancedScene(AddEnhancedScene_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint8_t);  // AddressMode
	msgLen += sizeof(uint16_t);  // TargetShortAddress
	msgLen += sizeof(uint8_t);  // SourceEndPoint
	msgLen += sizeof(uint8_t);  // DestinationEndPoint
	msgLen += sizeof(uint16_t);  // GroupID
	msgLen += sizeof(uint8_t);  // SceneID
	msgLen += sizeof(uint16_t);  // TransitionTime
	msgLen += sizeof(uint8_t);  // SceneNameLength
	msgLen += req->SceneNameLength;  // SceneNameData
	msgLen += sizeof(uint8_t);  // Length
	msgLen += sizeof(uint8_t);  // MaxLength
	msgLen += req->Length;  // ExtensionFieldSets

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	pMsg[idx] = req->AddressMode; idx++;
	FLib_MemCpy(pMsg + idx, &(req->TargetShortAddress), sizeof(req->TargetShortAddress)); idx += sizeof(req->TargetShortAddress);
	pMsg[idx] = req->SourceEndPoint; idx++;
	pMsg[idx] = req->DestinationEndPoint; idx++;
	FLib_MemCpy(pMsg + idx, &(req->GroupID), sizeof(req->GroupID)); idx += sizeof(req->GroupID);
	pMsg[idx] = req->SceneID; idx++;
	FLib_MemCpy(pMsg + idx, &(req->TransitionTime), sizeof(req->TransitionTime)); idx += sizeof(req->TransitionTime);
	pMsg[idx] = req->SceneNameLength; idx++;
	FLib_MemCpy(pMsg + idx, req->SceneNameData, req->SceneNameLength); idx += req->SceneNameLength;
	pMsg[idx] = req->Length; idx++;
	pMsg[idx] = req->MaxLength; idx++;
	FLib_MemCpy(pMsg + idx, req->ExtensionFieldSets, req->Length); idx += req->Length;

	/* Send the request */
	FSCI_transmitPayload(arg, 0x37, 0xA7, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t ViewEnhancedScene(ViewEnhancedScene_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t ViewEnhancedScene(ViewEnhancedScene_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0xA8, req, sizeof(ViewEnhancedScene_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t CopyScene(CopyScene_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t CopyScene(CopyScene_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0xA9, req, sizeof(CopyScene_t), fsciInterface);
	return MEM_SUCCESS_c;
}

#endif  /* SCENESCLUSTERCOMMANDS_ENABLE */

#if COLOURCLUSTERCOMMANDS_ENABLE
/*!*************************************************************************************************
\fn		memStatus_t MoveToHue(MoveToHue_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MoveToHue(MoveToHue_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0xB0, req, sizeof(MoveToHue_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MoveHue(MoveHue_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MoveHue(MoveHue_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0xB1, req, sizeof(MoveHue_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t StepHue(StepHue_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t StepHue(StepHue_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0xB2, req, sizeof(StepHue_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MoveToSaturation(MoveToSaturation_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MoveToSaturation(MoveToSaturation_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0xB3, req, sizeof(MoveToSaturation_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MoveSaturation(MoveSaturation_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MoveSaturation(MoveSaturation_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0xB4, req, sizeof(MoveSaturation_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t StepSaturation(StepSaturation_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t StepSaturation(StepSaturation_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0xB5, req, sizeof(StepSaturation_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MoveToHueAndSaturation(MoveToHueAndSaturation_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MoveToHueAndSaturation(MoveToHueAndSaturation_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0xB6, req, sizeof(MoveToHueAndSaturation_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MoveToColour(MoveToColour_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MoveToColour(MoveToColour_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0xB7, req, sizeof(MoveToColour_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MoveColour(MoveColour_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MoveColour(MoveColour_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0xB8, req, sizeof(MoveColour_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t EnhancedMoveToHue(EnhancedMoveToHue_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t EnhancedMoveToHue(EnhancedMoveToHue_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0xBA, req, sizeof(EnhancedMoveToHue_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t EnhancedMoveHue(EnhancedMoveHue_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t EnhancedMoveHue(EnhancedMoveHue_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0xBB, req, sizeof(EnhancedMoveHue_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t EnhancedStepHue(EnhancedStepHue_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t EnhancedStepHue(EnhancedStepHue_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0xBC, req, sizeof(EnhancedStepHue_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t EnhancedMoveToHueAndSaturation(EnhancedMoveToHueAndSaturation_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t EnhancedMoveToHueAndSaturation(EnhancedMoveToHueAndSaturation_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0xBD, req, sizeof(EnhancedMoveToHueAndSaturation_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t ColourLoopSet(ColourLoopSet_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t ColourLoopSet(ColourLoopSet_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0xBE, req, sizeof(ColourLoopSet_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t StopMoveStep(StopMoveStep_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t StopMoveStep(StopMoveStep_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0xBF, req, sizeof(StopMoveStep_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MoveToColourTemperature(MoveToColourTemperature_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MoveToColourTemperature(MoveToColourTemperature_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0xC0, req, sizeof(MoveToColourTemperature_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t MoveColourTemperature(MoveColourTemperature_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t MoveColourTemperature(MoveColourTemperature_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0xC1, req, sizeof(MoveColourTemperature_t), fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t StepColourTemperature(StepColourTemperature_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t StepColourTemperature(StepColourTemperature_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0xC2, req, sizeof(StepColourTemperature_t), fsciInterface);
	return MEM_SUCCESS_c;
}

#endif  /* COLOURCLUSTERCOMMANDS_ENABLE */

#if TOUCHLINKCOMMANDS_ENABLE
/*!*************************************************************************************************
\fn		memStatus_t InitiateTouchlink(void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c
***************************************************************************************************/
memStatus_t InitiateTouchlink(void *arg, uint8_t fsciInterface)
{
	FSCI_transmitPayload(arg, 0x37, 0xD0, NULL, 0, fsciInterface);
	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		memStatus_t TouchLinkFactoryResetTarget(void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c
***************************************************************************************************/
memStatus_t TouchLinkFactoryResetTarget(void *arg, uint8_t fsciInterface)
{
	FSCI_transmitPayload(arg, 0x37, 0xD2, NULL, 0, fsciInterface);
	return MEM_SUCCESS_c;
}

#endif  /* TOUCHLINKCOMMANDS_ENABLE */

#if DOORLOCKCLUSTERCOMMANDS_ENABLE
/*!*************************************************************************************************
\fn		memStatus_t LockUnlockDoor(LockUnlockDoor_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t LockUnlockDoor(LockUnlockDoor_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x37, 0xF0, req, sizeof(LockUnlockDoor_t), fsciInterface);
	return MEM_SUCCESS_c;
}

#endif  /* DOORLOCKCLUSTERCOMMANDS_ENABLE */

#if IASCLUSTERCOMMANDS_ENABLE
/*!*************************************************************************************************
\fn		memStatus_t IASZoneEnrollResponse(IASZoneEnrollResponse_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t IASZoneEnrollResponse(IASZoneEnrollResponse_t *req, void *arg, uint8_t fsciInterface)
{
	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	FSCI_transmitPayload(arg, 0x3B, 0x00, req, sizeof(IASZoneEnrollResponse_t), fsciInterface);
	return MEM_SUCCESS_c;
}

#endif  /* IASCLUSTERCOMMANDS_ENABLE */

#if EXTENDEDUTILITIES_ENABLE
/*!*************************************************************************************************
\fn		memStatus_t RawAPSDataRequest(RawAPSDataRequest_t *req, void *arg, uint8_t fsciInterface)
\brief	None

\return	memStatus_t			MEM_SUCCESS_c, MEM_ALLOC_ERROR_c, MEM_FREE_ERROR_c
							MEM_UNKNOWN_ERROR_c if req is NULL
***************************************************************************************************/
memStatus_t RawAPSDataRequest(RawAPSDataRequest_t *req, void *arg, uint8_t fsciInterface)
{
	uint8_t *pMsg = NULL;
	uint16_t msgLen = 0, idx = 0;

	/* Sanity check */
	if (!req)
	{
		return MEM_UNKNOWN_ERROR_c;
	}

	/* Compute the size of the request */
	msgLen += sizeof(uint8_t);  // AddressMode
	msgLen += sizeof(uint16_t);  // TargetShortAddress
	msgLen += sizeof(uint8_t);  // SourceEndPoint
	msgLen += sizeof(uint8_t);  // DestinationEndPoint
	msgLen += sizeof(uint16_t);  // ClusterID
	msgLen += sizeof(uint16_t);  // ProfileID
	msgLen += sizeof(uint8_t);  // SecurityMode
	msgLen += sizeof(uint8_t);  // Radius
	msgLen += sizeof(uint8_t);  // DataLength
	msgLen += req->DataLength;  // Data

	/* Allocate memory for the marshalled payload */
	pMsg = MEM_BufferAlloc(msgLen);
	if (!pMsg)
	{
		return MEM_ALLOC_ERROR_c;
	}

	/* Serialize */
	pMsg[idx] = req->AddressMode; idx++;
	FLib_MemCpy(pMsg + idx, &(req->TargetShortAddress), sizeof(req->TargetShortAddress)); idx += sizeof(req->TargetShortAddress);
	pMsg[idx] = req->SourceEndPoint; idx++;
	pMsg[idx] = req->DestinationEndPoint; idx++;
	FLib_MemCpy(pMsg + idx, &(req->ClusterID), sizeof(req->ClusterID)); idx += sizeof(req->ClusterID);
	FLib_MemCpy(pMsg + idx, &(req->ProfileID), sizeof(req->ProfileID)); idx += sizeof(req->ProfileID);
	pMsg[idx] = req->SecurityMode; idx++;
	pMsg[idx] = req->Radius; idx++;
	pMsg[idx] = req->DataLength; idx++;
	FLib_MemCpy(pMsg + idx, req->Data, req->DataLength); idx += req->DataLength;

	/* Send the request */
	FSCI_transmitPayload(arg, 0x3C, 0x30, pMsg, msgLen, fsciInterface);
	MEM_BufferFree(pMsg);
	return MEM_SUCCESS_c;
}

#endif  /* EXTENDEDUTILITIES_ENABLE */
