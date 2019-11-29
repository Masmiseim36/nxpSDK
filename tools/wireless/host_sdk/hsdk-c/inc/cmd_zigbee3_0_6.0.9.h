/* Header file generated from ZigBee3_0_6.0.8.xml */
/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _ZIGBEE_CMD_H
#define _ZIGBEE_CMD_H
/*==================================================================================================
Include Files
==================================================================================================*/
#include "os_abstraction.h"

/*==================================================================================================
Public macros
==================================================================================================*/
#define EXTENDEDUTILITIES_ENABLE TRUE
#define IASCLUSTERCOMMANDS_ENABLE TRUE
#define DIAGNOSTICSCLUSTERCOMMANDS_ENABLE TRUE
#define ENTIREPROFILE_ENABLE TRUE
#define EXPORTINGPERSISTENTDATATOHOST_ENABLE TRUE
#define TOUCHLINKCOMMANDS_ENABLE TRUE
#define IDENTIFYCLUSTERCOMMANDS_ENABLE TRUE
#define ZIGBEESTACKANDNODEMANAGEMENTCOMMANDS_ENABLE TRUE
#define GROUPCLUSTERCOMMANDS_ENABLE TRUE
#define DOORLOCKCLUSTERCOMMANDS_ENABLE TRUE
#define ONOFFCLUSTERCOMMANDS_ENABLE TRUE
#define SCENESCLUSTERCOMMANDS_ENABLE TRUE
#define COLOURCLUSTERCOMMANDS_ENABLE TRUE
#define LEVELCLUSTERCOMMANDS_ENABLE TRUE

/*==================================================================================================
Public type definitions
==================================================================================================*/
#if ZIGBEESTACKANDNODEMANAGEMENTCOMMANDS_ENABLE
typedef PACKED_STRUCT SetExtendedPANID_tag {
	uint64_t ExtendedPANID;  // ExtendedPANID
} SetExtendedPANID_t;

typedef PACKED_STRUCT SetChannelMask_tag {
	uint32_t ChannelMask;  // ChannelMask
} SetChannelMask_t;

typedef PACKED_STRUCT SetSecurityStateKey_tag {
	uint8_t KeyType;  // KeyType
	uint8_t KeyData;  // KeyData
} SetSecurityStateKey_t;

/* DeviceType */
typedef enum SetDeviceType_DeviceType_tag {
	SetDeviceType_DeviceType_Coordinator = 0x00,
	SetDeviceType_DeviceType_Router = 0x01,
	SetDeviceType_DeviceType_EndDevice = 0x02
} SetDeviceType_DeviceType_t;

typedef PACKED_STRUCT SetDeviceType_tag {
	SetDeviceType_DeviceType_t DeviceType;  // DeviceType
} SetDeviceType_t;

typedef PACKED_STRUCT Code_tag {
	uint64_t TargetExtendedAddress;  // TargetExtendedAddress
	uint8_t Code[16];  // Code
} Code_t;

/* DestAddressingMode */
typedef enum Bind_DestAddressingMode_tag {
	Bind_DestAddressingMode_NO_PAN = 0x00,
	Bind_DestAddressingMode_Reserved = 0x01,
	Bind_DestAddressingMode_Address_16b = 0x02,
	Bind_DestAddressingMode_Address_64b = 0x03
} Bind_DestAddressingMode_t;

typedef struct Bind_tag {
	uint64_t TargetExtendedAddress;  // TargetExtendedAddress
	uint8_t TargetEndPoint;  // TargetEndPoint
	uint16_t ClusterID;  // ClusterID
	Bind_DestAddressingMode_t DestAddressingMode;  // DestAddressingMode
	union {
		uint16_t Address_16b;  // Address field contains a 16_bit short address.
		uint64_t Address_64b;  // Address field contains a 64_bit extended address.
	} DestinationAddress;  // MHR, Addressing fields. 0xFFFF Broadcast PAN identifier.
	uint8_t DestinationEndPoint;  // DestinationEndPoint
} Bind_t;

/* DestAddressingMode */
typedef enum Unbind_DestAddressingMode_tag {
	Unbind_DestAddressingMode_NO_PAN = 0x00,
	Unbind_DestAddressingMode_Reserved = 0x01,
	Unbind_DestAddressingMode_Address_16b = 0x02,
	Unbind_DestAddressingMode_Address_64b = 0x03
} Unbind_DestAddressingMode_t;

typedef struct Unbind_tag {
	uint64_t TargetExtendedAddress;  // TargetExtendedAddress
	uint8_t TargetEndPoint;  // TargetEndPoint
	uint16_t ClusterID;  // ClusterID
	Unbind_DestAddressingMode_t DestAddressingMode;  // DestAddressingMode
	union {
		uint16_t Address_16b;  // Address field contains a 16_bit short address.
		uint64_t Address_64b;  // Address field contains a 64_bit extended address.
	} DestinationAddress;  // MHR, Addressing fields. 0xFFFF Broadcast PAN identifier.
	uint8_t DestinationEndPoint;  // DestinationEndPoint
} Unbind_t;

/* RequestType */
typedef enum NetworkAddressRequest_RequestType_tag {
	NetworkAddressRequest_RequestType_SingleRequest = 0x00,
	NetworkAddressRequest_RequestType_ExtendedRequest = 0x01
} NetworkAddressRequest_RequestType_t;

typedef PACKED_STRUCT NetworkAddressRequest_tag {
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint64_t ExtendedAddress;  // ExtendedAddress
	NetworkAddressRequest_RequestType_t RequestType;  // RequestType
	uint8_t StartIndex;  // StartIndex
} NetworkAddressRequest_t;

/* RequestType */
typedef enum IEEEAddressRequest_RequestType_tag {
	IEEEAddressRequest_RequestType_SingleRequest = 0x00,
	IEEEAddressRequest_RequestType_ExtendedRequest = 0x01
} IEEEAddressRequest_RequestType_t;

typedef PACKED_STRUCT IEEEAddressRequest_tag {
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint16_t ShortAddress;  // ShortAddress
	IEEEAddressRequest_RequestType_t RequestType;  // RequestType
	uint8_t StartIndex;  // StartIndex
} IEEEAddressRequest_t;

typedef struct NodeDescriptorRequest_tag {
	uint8_t SequenceNumber;  // SequenceNumber
	uint8_t Status;  // Status
	uint16_t NetworkAddress;  // NetworkAddress
	uint16_t ManufacturerCode;  // ManufacturerCode
	uint16_t MaxRxSize;  // MaxRxSize
	uint16_t MaxTxSize;  // MaxTxSize
	uint8_t ServerMask;  // AllocateAddress|NetworkManager|BackupDiscoveryCache|PrimaryDiscoveryCache|BackupBindingCache|PrimaryBindingCache|BackUpTrustCenter|PrimaryTrustCenter  // ServerMask
	uint8_t DescriptorCapability;  // Reserved|ExtendedSimpleDdescriptorList|ExtendedActiveEndPointList  // DescriptorCapability
	uint8_t MacCapability;  // AllocateAddress|SecurityCapability|Reserved|ReceiverOnWhenIdle|PowerSource|DeviceType|AlternatePANCoordinator  // MacCapability
	uint8_t MaxBufferSize;  // MaxBufferSize
	uint16_t BitFields;  // FrequencyBand|APSFlags|Reserved|UserDescriptorAvailable|ComplexDescriptorAvailable|EndDevice|Router|Coordinator  // BitFields
} NodeDescriptorRequest_t;

typedef PACKED_STRUCT SimpleDescriptorRequest_tag {
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t EndPoint;  // EndPoint
} SimpleDescriptorRequest_t;

typedef PACKED_STRUCT PowerDescriptorRequest_tag {
	uint16_t TargetShortAddress;  // TargetShortAddress
} PowerDescriptorRequest_t;

typedef PACKED_STRUCT ActiveEndpointRequest_tag {
	uint16_t TargetShortAddress;  // TargetShortAddress
} ActiveEndpointRequest_t;

typedef struct MatchDescriptorRequest_tag {
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint16_t ProfileID;  // ProfileID
	uint8_t NumberOfInputClusters;  // NumberOfInputClusters
	uint16_t *InputClusterList;  // InputClusterList
	uint8_t NumberOfOutputClusters;  // NumberOfOutputClusters
	uint16_t *OutputClusterList;  // OutputClusterList
} MatchDescriptorRequest_t;

typedef PACKED_STRUCT RemoveDevice_tag {
	uint64_t ParentExtendedAddress;  // ParentExtendedAddress
	uint64_t ChildExtendedAddress;  // ChildExtendedAddress
} RemoveDevice_t;

typedef struct UserDescriptorSet_tag {
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint16_t AddressOfInterest;  // AddressOfInterest
	uint8_t StringLength;  // StringLength
	uint8_t *Parameters;  // Parameters
} UserDescriptorSet_t;

typedef PACKED_STRUCT UserDescriptorRequest_tag {
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint16_t AddressOfInterest;  // AddressOfInterest
} UserDescriptorRequest_t;

typedef PACKED_STRUCT SetPollIntervalRequest_tag {
	uint32_t Interval;  // Poll Interval in seconds
} SetPollIntervalRequest_t;

typedef struct AddInOutClustersRequest_tag {
	uint8_t EndPoint;  // EndPoint
	uint8_t InClusterCount;  // Number of input cluster ids to be added
	uint8_t OutClusterCount;  // Number of output cluster ids to be added
	uint16_t *InputClustersList;  // List of input cluster ids
	uint16_t *OutputClustersList;  // List of output cluster ids
} AddInOutClustersRequest_t;

typedef PACKED_STRUCT FindAndBindRequest_tag {
	bool_t Initiator;  // Initiator
	uint8_t EndPoint;  // EndPoint
} FindAndBindRequest_t;

typedef PACKED_STRUCT ComplexDescriptorRequest_tag {
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint16_t AddressOfInterest;  // AddressOfInterest
} ComplexDescriptorRequest_t;

/* Rejoin */
typedef enum ManagementLeaveRequest_Rejoin_tag {
	ManagementLeaveRequest_Rejoin_DoNotRejoin = 0x00,
	ManagementLeaveRequest_Rejoin_Rejoin = 0x01
} ManagementLeaveRequest_Rejoin_t;

/* RemoveChildren */
typedef enum ManagementLeaveRequest_RemoveChildren_tag {
	ManagementLeaveRequest_RemoveChildren_LeaveRemovingChildren = 0x00,
	ManagementLeaveRequest_RemoveChildren_LeaveDoNotRemoveChildren = 0x01
} ManagementLeaveRequest_RemoveChildren_t;

typedef PACKED_STRUCT ManagementLeaveRequest_tag {
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint64_t ExtendedAddress;  // ExtendedAddress
	ManagementLeaveRequest_Rejoin_t Rejoin;  // Rejoin
	ManagementLeaveRequest_RemoveChildren_t RemoveChildren;  // RemoveChildren
} ManagementLeaveRequest_t;

/* TCSignificance */
typedef enum PermitJoiningRequest_TCSignificance_tag {
	PermitJoiningRequest_TCSignificance_NoChangeInAuthentication = 0x00,
	PermitJoiningRequest_TCSignificance_AuthenticationPolicyAsSpec = 0x01
} PermitJoiningRequest_TCSignificance_t;

typedef PACKED_STRUCT PermitJoiningRequest_tag {
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t Interval;  // Interval: DisableJoining 0x00, TimeInSecondsToAllowJoins 0x01-0xFE, AllowAllJoins 0xFF
	PermitJoiningRequest_TCSignificance_t TCSignificance;  // TCSignificance
} PermitJoiningRequest_t;

typedef PACKED_STRUCT ManagementNetworkUpdateRequest_tag {
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint32_t ChannelMask;  // ChannelMask
	uint8_t ScanDuration;  // ScanDuration
	uint8_t ScanCount;  // ScanCount
	uint8_t NetworkUpdateID;  // NetworkUpdateID
	uint16_t NetworkManagerShortAddress;  // NetworkManagerShortAddress
} ManagementNetworkUpdateRequest_t;

typedef PACKED_STRUCT SystemServerDiscoveryRequest_tag {
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint16_t ServerMask;  // ServerMask
} SystemServerDiscoveryRequest_t;

typedef PACKED_STRUCT ManagementLQIRequest_tag {
	uint16_t TargetAddress;  // TargetAddress
	uint8_t StartIndex;  // StartIndex
} ManagementLQIRequest_t;

#endif  /* ZIGBEESTACKANDNODEMANAGEMENTCOMMANDS_ENABLE */

#if ENTIREPROFILE_ENABLE
/* Direction */
typedef enum ReadAttributeRequest_Direction_tag {
	ReadAttributeRequest_Direction_FromServerToClient = 0x00,
	ReadAttributeRequest_Direction_FromClientToServer = 0x01
} ReadAttributeRequest_Direction_t;

/* ManufacturerSpecific */
typedef enum ReadAttributeRequest_ManufacturerSpecific_tag {
	ReadAttributeRequest_ManufacturerSpecific_No = 0x00,
	ReadAttributeRequest_ManufacturerSpecific_Yes = 0x01
} ReadAttributeRequest_ManufacturerSpecific_t;

typedef struct ReadAttributeRequest_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint16_t ClusterID;  // ClusterID
	ReadAttributeRequest_Direction_t Direction;  // Direction
	ReadAttributeRequest_ManufacturerSpecific_t ManufacturerSpecific;  // ManufacturerSpecific
	uint16_t ManufacturerID;  // ManufacturerID
	uint8_t NumberOfAttributes;  // NumberOfAttributes
	uint16_t *AttributesList;  // AttributesList
} ReadAttributeRequest_t;

/* Direction */
typedef enum WriteAttributeRequest_Direction_tag {
	WriteAttributeRequest_Direction_FromServerToClient = 0x00,
	WriteAttributeRequest_Direction_FromClientToServer = 0x01
} WriteAttributeRequest_Direction_t;

/* ManufacturerSpecific */
typedef enum WriteAttributeRequest_ManufacturerSpecific_tag {
	WriteAttributeRequest_ManufacturerSpecific_No = 0x00,
	WriteAttributeRequest_ManufacturerSpecific_Yes = 0x01
} WriteAttributeRequest_ManufacturerSpecific_t;

typedef struct WriteAttributeRequest_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint16_t ClusterID;  // ClusterID
	WriteAttributeRequest_Direction_t Direction;  // Direction
	WriteAttributeRequest_ManufacturerSpecific_t ManufacturerSpecific;  // ManufacturerSpecific
	uint16_t ManufacturerID;  // ManufacturerID
	uint8_t NumberOfAttributes;  // NumberOfAttributes
	uint16_t *AttributesList;  // AttributesList
} WriteAttributeRequest_t;

/* Direction */
typedef enum AttributeDiscoveryRequest_Direction_tag {
	AttributeDiscoveryRequest_Direction_FromServerToClient = 0x01,
	AttributeDiscoveryRequest_Direction_FromClientToServer = 0x00
} AttributeDiscoveryRequest_Direction_t;

/* ManufacturerSpecific */
typedef enum AttributeDiscoveryRequest_ManufacturerSpecific_tag {
	AttributeDiscoveryRequest_ManufacturerSpecific_No = 0x00,
	AttributeDiscoveryRequest_ManufacturerSpecific_Yes = 0x01
} AttributeDiscoveryRequest_ManufacturerSpecific_t;

typedef struct AttributeDiscoveryRequest_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint16_t ClusterID;  // ClusterID
	uint16_t AttributeID;  // AttributeID
	AttributeDiscoveryRequest_Direction_t Direction;  // Direction
	AttributeDiscoveryRequest_ManufacturerSpecific_t ManufacturerSpecific;  // ManufacturerSpecific
	uint16_t ManufacturerID;  // ManufacturerID
	uint8_t NumberOfAttributes;  // NumberOfAttributes
	uint8_t *MaxNumberOfIdentifiers;  // MaxNumberOfIdentifiers
} AttributeDiscoveryRequest_t;

/* EnableDisable */
typedef enum EnablePermissionsControlledJoins_EnableDisable_tag {
	EnablePermissionsControlledJoins_EnableDisable_Disable = 0x00,
	EnablePermissionsControlledJoins_EnableDisable_Enable = 0x01
} EnablePermissionsControlledJoins_EnableDisable_t;

typedef PACKED_STRUCT EnablePermissionsControlledJoins_tag {
	EnablePermissionsControlledJoins_EnableDisable_t EnableDisable;  // EnableDisable
} EnablePermissionsControlledJoins_t;

typedef PACKED_STRUCT AuthenticateDevice_tag {
	uint64_t IEEEAddress;  // IEEEAddress
	uint8_t Key[16];  // Key
} AuthenticateDevice_t;

typedef struct ConfigureReportingRequest_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndpoint;  // DestinationEndpoint
	uint16_t ClusterID;  // ClusterID
	uint8_t Direction;  // Direction
	uint8_t ManufacturerSpecific;  // ManufacturerSpecific
	uint16_t ManufacturerID;  // ManufacturerID
	uint8_t NumberOfAttributes;  // NumberOfAttributes
	uint8_t *AttributesList;  // AttributesList
} ConfigureReportingRequest_t;

#endif  /* ENTIREPROFILE_ENABLE */

#if GROUPCLUSTERCOMMANDS_ENABLE
typedef PACKED_STRUCT AddGroup_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint16_t GroupAddress;  // GroupAddress
} AddGroup_t;

typedef PACKED_STRUCT ViewGroup_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint16_t GroupAddress;  // GroupAddress
} ViewGroup_t;

typedef struct GetGroupMembership_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint8_t GroupCount;  // GroupCount
	uint16_t *GroupList;  // GroupList
} GetGroupMembership_t;

typedef PACKED_STRUCT RemoveGroup_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint16_t GroupAddress;  // GroupAddress
} RemoveGroup_t;

typedef PACKED_STRUCT RemoveAllGroups_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
} RemoveAllGroups_t;

typedef PACKED_STRUCT AddGroupIfIdentify_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint16_t GroupAddress;  // GroupAddress
} AddGroupIfIdentify_t;

#endif  /* GROUPCLUSTERCOMMANDS_ENABLE */

#if IDENTIFYCLUSTERCOMMANDS_ENABLE
typedef PACKED_STRUCT IdentifySend_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint16_t TimeInSecconds;  // TimeInSecconds
} IdentifySend_t;

typedef PACKED_STRUCT IdentifyQuery_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
} IdentifyQuery_t;

typedef PACKED_STRUCT IdentifyTriggerEffects_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint8_t EffectID;  // EffectID
	uint8_t EffectGradient;  // EffectGradient
} IdentifyTriggerEffects_t;

#endif  /* IDENTIFYCLUSTERCOMMANDS_ENABLE */

#if LEVELCLUSTERCOMMANDS_ENABLE
typedef PACKED_STRUCT MoveToLevel_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint8_t OnOff;  // OnOff
	uint8_t Mode;  // Mode
	uint8_t Rate;  // Rate
} MoveToLevel_t;

typedef PACKED_STRUCT MoveToLevelWithWithoutOnOff_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint8_t OnOff;  // OnOff
	uint8_t Level;  // Level
	uint16_t TransitionTime;  // TransitionTime
} MoveToLevelWithWithoutOnOff_t;

typedef PACKED_STRUCT MoveStep_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint8_t OnOff;  // OnOff
	uint8_t StepMode;  // StepMode
	uint8_t StepSize;  // StepSize
	uint16_t TransitionTime;  // TransitionTime
} MoveStep_t;

typedef PACKED_STRUCT MoveStopWithOnOff_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
} MoveStopWithOnOff_t;

typedef PACKED_STRUCT MoveToClosestFrequency_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint16_t Frequency;  // Frequency
} MoveToClosestFrequency_t;

#endif  /* LEVELCLUSTERCOMMANDS_ENABLE */

#if ONOFFCLUSTERCOMMANDS_ENABLE
typedef PACKED_STRUCT OnOffWithEffectsSend_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint8_t EffectID;  // EffectID
	uint8_t EffectGradient;  // EffectGradient
} OnOffWithEffectsSend_t;

/* CommandID */
typedef enum OnOffWithNoEffects_CommandID_tag {
	OnOffWithNoEffects_CommandID_Off = 0x00,
	OnOffWithNoEffects_CommandID_On = 0x01,
	OnOffWithNoEffects_CommandID_Toggle = 0x02
} OnOffWithNoEffects_CommandID_t;

typedef PACKED_STRUCT OnOffWithNoEffects_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	OnOffWithNoEffects_CommandID_t CommandID;  // CommandID
} OnOffWithNoEffects_t;

/* OnOff */
typedef enum OnOffTimedSend_OnOff_tag {
	OnOffTimedSend_OnOff_Off = 0x00,
	OnOffTimedSend_OnOff_On = 0x01
} OnOffTimedSend_OnOff_t;

typedef PACKED_STRUCT OnOffTimedSend_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	OnOffTimedSend_OnOff_t OnOff;  // OnOff
	uint16_t OnTimeInSecconds;  // OnTimeInSecconds
	uint16_t OffTimeInSecconds;  // OffTimeInSecconds
} OnOffTimedSend_t;

#endif  /* ONOFFCLUSTERCOMMANDS_ENABLE */

#if DIAGNOSTICSCLUSTERCOMMANDS_ENABLE
/* Address Mode */
typedef enum DiagnosticRequest_AddressMode_tag {
	DiagnosticRequest_AddressMode_TargetShort = 0x02,
	DiagnosticRequest_AddressMode_TargetIEEE = 0x03,
	DiagnosticRequest_AddressMode_TargetBroadcast = 0x04
} DiagnosticRequest_AddressMode_t;

/* CommandID */
typedef enum DiagnosticRequest_CommandID_tag {
	DiagnosticRequest_CommandID_DelayRequest = 0x00,
	DiagnosticRequest_CommandID_DelayResults = 0x01
} DiagnosticRequest_CommandID_t;

typedef struct DiagnosticRequest_tag {
	DiagnosticRequest_AddressMode_t AddressMode;  // Address Mode
	union {
		uint16_t TargetShort_TargetShortAddress;

		uint64_t TargetIEEE_TargetIEEEAddress;

		uint16_t TargetBroadcast_TargetBroadcastAddress;

	} TargetAddressValue;  // Target Address Value
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	DiagnosticRequest_CommandID_t CommandID;  // CommandID
	union {
		struct {
			uint8_t SequenceNumber;  // Sequence Number
			uint8_t PayloadSize;  // Payload Size
			uint8_t *Payload;  // OTA transmited payload
		} DelayRequest;  // Delay Request
	} AttributeValue;  // Attribute Value
} DiagnosticRequest_t;

#endif  /* DIAGNOSTICSCLUSTERCOMMANDS_ENABLE */

#if SCENESCLUSTERCOMMANDS_ENABLE
typedef PACKED_STRUCT ViewScene_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint16_t GroupID;  // GroupID
	uint8_t SceneID;  // SceneID
} ViewScene_t;

typedef struct AddScene_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint16_t GroupID;  // GroupID
	uint8_t SceneID;  // SceneID
	uint16_t TransitionTime;  // TransitionTime
	uint8_t SceneNameLength;  // SceneNameLength
	uint8_t SceneNameMaxLength;  // SceneNameMaxLength
	char *SceneNameData;  // SceneNameData
} AddScene_t;

typedef PACKED_STRUCT RemoveScene_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint16_t GroupID;  // GroupID
	uint8_t SceneID;  // SceneID
} RemoveScene_t;

typedef PACKED_STRUCT RemoveAllScenes_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint16_t GroupID;  // GroupID
} RemoveAllScenes_t;

typedef PACKED_STRUCT StoreScene_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint16_t GroupID;  // GroupID
	uint8_t SceneID;  // SceneID
} StoreScene_t;

typedef PACKED_STRUCT RecallScene_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint16_t GroupID;  // GroupID
	uint8_t SceneID;  // SceneID
	uint16_t TransitionTime;  // TransitionTime
} RecallScene_t;

typedef PACKED_STRUCT SceneMembershipRequest_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint16_t GroupID;  // GroupID
} SceneMembershipRequest_t;

typedef struct AddEnhancedScene_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint16_t GroupID;  // GroupID
	uint8_t SceneID;  // SceneID
	uint16_t TransitionTime;  // TransitionTime
	uint8_t SceneNameLength;  // SceneNameLength
	char *SceneNameData;  // SceneNameData
	uint8_t Length;  // Length
	uint8_t MaxLength;  // MaxLength
	uint8_t *ExtensionFieldSets;  // ExtensionFieldSets
} AddEnhancedScene_t;

typedef PACKED_STRUCT ViewEnhancedScene_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint16_t GroupID;  // GroupID
	uint8_t SceneID;  // SceneID
} ViewEnhancedScene_t;

typedef PACKED_STRUCT CopyScene_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint8_t Mode;  // Mode
	uint16_t FromGroupID;  // FromGroupID
	uint8_t FromSceneID;  // FromSceneID
	uint16_t ToGroupID;  // ToGroupID
	uint8_t ToSceneID;  // ToSceneID
} CopyScene_t;

#endif  /* SCENESCLUSTERCOMMANDS_ENABLE */

#if COLOURCLUSTERCOMMANDS_ENABLE
typedef PACKED_STRUCT MoveToHue_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint8_t Hue;  // Hue
	uint8_t Direction;  // Direction
	uint16_t TransitionTime;  // TransitionTime
} MoveToHue_t;

typedef PACKED_STRUCT MoveHue_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint8_t Mode;  // Mode
	uint8_t Rate;  // Rate
} MoveHue_t;

typedef PACKED_STRUCT StepHue_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint8_t Mode;  // Mode
	uint8_t StepSize;  // StepSize
	uint8_t TransitionTime;  // TransitionTime
} StepHue_t;

typedef PACKED_STRUCT MoveToSaturation_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint8_t Saturation;  // Saturation
	uint16_t TransitionTime;  // TransitionTime
} MoveToSaturation_t;

typedef PACKED_STRUCT MoveSaturation_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint8_t Mode;  // Mode
	uint16_t Rate;  // Rate
} MoveSaturation_t;

typedef PACKED_STRUCT StepSaturation_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint8_t Mode;  // Mode
	uint8_t StepSize;  // StepSize
	uint8_t TransitionTime;  // TransitionTime
} StepSaturation_t;

typedef PACKED_STRUCT MoveToHueAndSaturation_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint8_t Hue;  // Hue
	uint8_t Saturation;  // Saturation
	uint8_t TransitionTime;  // TransitionTime
} MoveToHueAndSaturation_t;

typedef PACKED_STRUCT MoveToColour_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint16_t ColorX;  // ColorX
	uint16_t ColorY;  // ColorY
	uint16_t TransitionTime;  // TransitionTime
} MoveToColour_t;

typedef PACKED_STRUCT MoveColour_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint16_t ColorX;  // ColorX
	uint16_t ColorY;  // ColorY
} MoveColour_t;

typedef PACKED_STRUCT EnhancedMoveToHue_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint8_t Direction;  // Direction
	uint16_t EnhancedHue;  // EnhancedHue
	uint16_t TransitionTime;  // TransitionTime
} EnhancedMoveToHue_t;

typedef PACKED_STRUCT EnhancedMoveHue_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint8_t Mode;  // Mode
	uint16_t Rate;  // Rate
} EnhancedMoveHue_t;

typedef PACKED_STRUCT EnhancedStepHue_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint8_t Mode;  // Mode
	uint16_t StepSize;  // StepSize
	uint16_t TransitionTime;  // TransitionTime
} EnhancedStepHue_t;

typedef PACKED_STRUCT EnhancedMoveToHueAndSaturation_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint8_t Saturation;  // Saturation
	uint16_t EnhancedHue;  // EnhancedHue
	uint16_t TransitionTime;  // TransitionTime
} EnhancedMoveToHueAndSaturation_t;

typedef PACKED_STRUCT ColourLoopSet_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint8_t UpdateFlags;  // UpdateFlags
	uint8_t Action;  // Action
	uint8_t Direction;  // Direction
	uint8_t Time;  // Time
	uint32_t StartHue;  // StartHue
} ColourLoopSet_t;

typedef PACKED_STRUCT StopMoveStep_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint8_t OptionsMask;  // OptionsMask
	uint8_t OptionsOverride;  // OptionsOverride
} StopMoveStep_t;

typedef PACKED_STRUCT MoveToColourTemperature_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint16_t ColourTemperature;  // ColourTemperature
	uint16_t TransitionTime;  // TransitionTime
} MoveToColourTemperature_t;

typedef PACKED_STRUCT MoveColourTemperature_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint8_t Mode;  // Mode
	uint16_t Rate;  // Rate
	uint16_t MinimumTemperature;  // MinimumTemperature
	uint16_t MaximumTemperature;  // MaximumTemperature
	uint8_t OptionsMask;  // OptionsMask
	uint8_t OptionsOverride;  // OptionsOverride
} MoveColourTemperature_t;

typedef PACKED_STRUCT StepColourTemperature_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint8_t Mode;  // Mode
	uint16_t StepSize;  // StepSize
	uint16_t TransitionTime;  // TransitionTime
	uint16_t MinimumTemperature;  // MinimumTemperature
	uint16_t MaximumTemperature;  // MaximumTemperature
	uint8_t OptionsMask;  // OptionsMask
	uint8_t OptionsOverride;  // OptionsOverride
} StepColourTemperature_t;

#endif  /* COLOURCLUSTERCOMMANDS_ENABLE */

#if TOUCHLINKCOMMANDS_ENABLE
#endif  /* TOUCHLINKCOMMANDS_ENABLE */

#if DOORLOCKCLUSTERCOMMANDS_ENABLE
/* LockUnlock */
typedef enum LockUnlockDoor_LockUnlock_tag {
	LockUnlockDoor_LockUnlock_Lock = 0x00,
	LockUnlockDoor_LockUnlock_Unlock = 0x01
} LockUnlockDoor_LockUnlock_t;

typedef PACKED_STRUCT LockUnlockDoor_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	LockUnlockDoor_LockUnlock_t LockUnlock;  // LockUnlock
} LockUnlockDoor_t;

#endif  /* DOORLOCKCLUSTERCOMMANDS_ENABLE */

#if IASCLUSTERCOMMANDS_ENABLE
typedef PACKED_STRUCT IASZoneEnrollResponse_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint8_t EnrollResponseCode;  // EnrollResponseCode
	uint8_t ZoneID;  // ZoneID
} IASZoneEnrollResponse_t;

#endif  /* IASCLUSTERCOMMANDS_ENABLE */

#if EXTENDEDUTILITIES_ENABLE
typedef struct RawAPSDataRequest_tag {
	uint8_t AddressMode;  // AddressMode
	uint16_t TargetShortAddress;  // TargetShortAddress
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	uint16_t ClusterID;  // ClusterID
	uint16_t ProfileID;  // ProfileID
	uint8_t SecurityMode;  // SecurityMode
	uint8_t Radius;  // Radius
	uint8_t DataLength;  // DataLength
	uint8_t *Data;  // Data
} RawAPSDataRequest_t;

#endif  /* EXTENDEDUTILITIES_ENABLE */

#if ZIGBEESTACKANDNODEMANAGEMENTCOMMANDS_ENABLE
/* Status */
typedef enum Status_Status_tag {
	Status_Status_Success = 0x00,
	Status_Status_IncorrectParameters = 0x01,
	Status_Status_UnhandledCommand = 0x02,
	Status_Status_CommandFailed = 0x03,
	Status_Status_Busy = 0x04,
	Status_Status_StackAlreadyStarted = 0x05,
	Status_Status_Failed = 0x80,
	Status_Status_Reserved = 0xF5
} Status_Status_t;

typedef PACKED_STRUCT Status_tag {
	Status_Status_t Status;  // Status
	uint8_t SeqNum;  // SeqNum
	uint16_t PacketType;  // PacketType
} Status_t;

/* Use the Linux / Unix log levels */
typedef enum LogMessage_LogLevel_tag {
	LogMessage_LogLevel_Emergency = 0x00,
	LogMessage_LogLevel_Alert = 0x01,
	LogMessage_LogLevel_Critical = 0x02,
	LogMessage_LogLevel_Error = 0x03,
	LogMessage_LogLevel_Warning = 0x04,
	LogMessage_LogLevel_Notice = 0x05,
	LogMessage_LogLevel_Information = 0x06,
	LogMessage_LogLevel_Debug = 0x07
} LogMessage_LogLevel_t;

typedef PACKED_STRUCT LogMessage_tag {
	LogMessage_LogLevel_t LogLevel;  // Use the Linux / Unix log levels
	char LogMessage[1];  // LogMessage
} LogMessage_t;

/* SourceAddressMode */
typedef enum DataIndication_SourceAddressMode_tag {
	DataIndication_SourceAddressMode_NO_PAN = 0x00,
	DataIndication_SourceAddressMode_Reserved = 0x01,
	DataIndication_SourceAddressMode_Address_16b = 0x02,
	DataIndication_SourceAddressMode_Address_64b = 0x03
} DataIndication_SourceAddressMode_t;

/* DestAddressingMode */
typedef enum DataIndication_DestAddressingMode_tag {
	DataIndication_DestAddressingMode_NO_PAN = 0x00,
	DataIndication_DestAddressingMode_Reserved = 0x01,
	DataIndication_DestAddressingMode_Address_16b = 0x02,
	DataIndication_DestAddressingMode_Address_64b = 0x03
} DataIndication_DestAddressingMode_t;

typedef struct DataIndication_tag {
	uint8_t Status;  // Status
	uint16_t ProfileID;  // ProfileID
	uint16_t ClusterID;  // ClusterID
	uint8_t SourceEndpoint;  // SourceEndpoint
	uint8_t DestinationEndpoint;  // DestinationEndpoint
	DataIndication_SourceAddressMode_t SourceAddressMode;  // SourceAddressMode
	union {
		uint16_t Address_16b;  // Address field contains a 16_bit short address.
		uint64_t Address_64b;  // Address field contains a 64_bit extended address.
	} SourceAddress;  // MHR, Addressing fields. 0xFFFF Broadcast PAN identifier.
	DataIndication_DestAddressingMode_t DestAddressingMode;  // DestAddressingMode
	union {
		uint16_t Address_16b;  // Address field contains a 16_bit short address.
		uint64_t Address_64b;  // Address field contains a 64_bit extended address.
	} DestinationAddress;  // MHR, Addressing fields. 0xFFFF Broadcast PAN identifier.
	uint8_t PayloadSize;  // PayloadSize
	uint8_t *Payload;  // Payload
} DataIndication_t;

typedef PACKED_STRUCT NodeClusterList_tag {
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint16_t ProfileID;  // ProfileID
	uint16_t ClusterList;  // ClusterList
} NodeClusterList_t;

typedef PACKED_STRUCT NodeClusterAttributeList_tag {
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint16_t ProfileID;  // ProfileID
	uint16_t ClusterID;  // ClusterID
	uint16_t AttributeList;  // AttributeList
} NodeClusterAttributeList_t;

typedef PACKED_STRUCT NodeCommandIDList_tag {
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint16_t ProfileID;  // ProfileID
	uint16_t ClusterID;  // ClusterID
	uint8_t CommandIDList;  // CommandIDList
} NodeCommandIDList_t;

typedef PACKED_STRUCT VersionList_tag {
	uint16_t MajorVersionNumber;  // MajorVersionNumber
	uint16_t InstallerVersionNumber;  // InstallerVersionNumber
} VersionList_t;

/* Status */
typedef enum NetworkJoinedFormed_Status_tag {
	NetworkJoinedFormed_Status_JoinedExistingNetwork = 0x00,
	NetworkJoinedFormed_Status_FormedNewNetwork = 0x01,
	NetworkJoinedFormed_Status_Failed = 0x80
} NetworkJoinedFormed_Status_t;

typedef PACKED_STRUCT NetworkJoinedFormed_tag {
	NetworkJoinedFormed_Status_t Status;  // Status
	uint16_t ShortAddr;  // Short Address
	uint64_t ExtAddr;  // Extended Address
	uint8_t Channel;  // Channel
} NetworkJoinedFormed_t;

/* Status */
typedef enum PermitJoinStatusResponse_Status_tag {
	PermitJoinStatusResponse_Status_Off = 0x00,
	PermitJoinStatusResponse_Status_On = 0x01
} PermitJoinStatusResponse_Status_t;

typedef PACKED_STRUCT PermitJoinStatusResponse_tag {
	PermitJoinStatusResponse_Status_t Status;  // Status
} PermitJoinStatusResponse_t;

/* Status */
typedef enum NonFactoryNewRestart_Status_tag {
	NonFactoryNewRestart_Status_STARTUP = 0x00,
	NonFactoryNewRestart_Status_NFN_START = 0x02,
	NonFactoryNewRestart_Status_RUNNING = 0x06
} NonFactoryNewRestart_Status_t;

typedef PACKED_STRUCT NonFactoryNewRestart_tag {
	NonFactoryNewRestart_Status_t Status;  // Status
} NonFactoryNewRestart_t;

/* Status */
typedef enum FactoryNewRestart_Status_tag {
	FactoryNewRestart_Status_STARTUP = 0x00,
	FactoryNewRestart_Status_NFN_START = 0x02,
	FactoryNewRestart_Status_RUNNING = 0x06
} FactoryNewRestart_Status_t;

typedef PACKED_STRUCT FactoryNewRestart_tag {
	NonFactoryNewRestart_Status_t Status;  // Status
} FactoryNewRestart_t;

typedef PACKED_STRUCT BindResponse_tag {
	uint8_t SequenceNumber;  // SequenceNumber
	uint8_t Status;  // Status
} BindResponse_t;

typedef PACKED_STRUCT UnbindResponse_tag {
	uint8_t SequenceNumber;  // SequenceNumber
	uint8_t Status;  // Status
} UnbindResponse_t;

typedef PACKED_STRUCT NetworkAddressResponse_tag {
	uint8_t SequenceNumber;  // SequenceNumber
	uint8_t Status;  // Status
	uint64_t IEEEAddress;  // IEEEAddress
	uint16_t ShortAddress;  // ShortAddress
	uint8_t NumberOfAssociatedDevices;  // NumberOfAssociatedDevices
	uint8_t StartIndex;  // StartIndex
	uint16_t DeviceList;  // DeviceList
} NetworkAddressResponse_t;

typedef struct IEEEAddressResponse_tag {
	uint8_t SequenceNumber;  // SequenceNumber
	uint8_t Status;  // Status
	uint64_t IEEEAddress;  // IEEEAddress
	uint16_t shortAddress;  // shortAddress
	uint8_t NumberOfAssociatedDevices;  // NumberOfAssociatedDevices
	uint8_t StartIndex;  // StartIndex
	uint16_t *DeviceList;  // DeviceList
} IEEEAddressResponse_t;

typedef PACKED_STRUCT NodeDescriptorResponse_tag {
	uint8_t SequenceNumber;  // SequenceNumber
	uint8_t Status;  // Status
	uint16_t NetworkAddress;  // NetworkAddress
	uint16_t ManufacturerCode;  // ManufacturerCode
	uint16_t MaxRxSize;  // MaxRxSize
	uint16_t MaxTxSize;  // MaxTxSize
	uint16_t ServerMask;  // ServerMask
	uint8_t DescriptorCapability;  // DescriptorCapability
	uint8_t MacCapability;  // MacCapability
	uint8_t MaxBufferSize;  // MaxBufferSize
	uint16_t BitFields;  // BitFields
} NodeDescriptorResponse_t;

typedef struct SimpleDescriptorResponse_tag {
	uint8_t SequenceNumber;  // SequenceNumber
	uint8_t Status;  // Status
	uint16_t NwkAddress;  // NwkAddress
	uint8_t Length;  // Length
	uint8_t EndPoint;  // EndPoint
	uint16_t Profile;  // Profile
	uint16_t DeviceID;  // DeviceID
	uint8_t BitFields;  // Reserved|DeviceVersion  // BitFields
	uint8_t InClusterCount;  // InClusterCount
	uint16_t *InClusterList;  // InClusterList
	uint8_t OutClusterCount;  // OutClusterCount
	uint16_t *OutClusterList;  // OutClusterList
} SimpleDescriptorResponse_t;

typedef PACKED_STRUCT PowerDescriptorResponse_tag {
	uint8_t SequenceNumber;  // SequenceNumber
	uint8_t Status;  // Status
	uint16_t BitFields;  // CurrentPowerSourceLevel|CurrentPowerSource|AvailablePowerSource|CurrentPowerMode  // BitFields
} PowerDescriptorResponse_t;

typedef struct ActiveEndpointResponse_tag {
	uint8_t SequenceNumber;  // SequenceNumber
	uint8_t Status;  // Status
	uint16_t Address;  // Address
	uint8_t EndpointCount;  // EndpointCount
	uint8_t *ActiveEndPointList;  // ActiveEndPointList
} ActiveEndpointResponse_t;

typedef struct MatchDescriptorResponse_tag {
	uint8_t SequenceNumber;  // SequenceNumber
	uint8_t Status;  // Status
	uint16_t NetworkAddress;  // NetworkAddress
	uint8_t LengthOfList;  // LengthOfList
	uint8_t *MatchList;  // MatchList
} MatchDescriptorResponse_t;

typedef PACKED_STRUCT DeviceAnnounce_tag {
	uint16_t ShortAddress;  // ShortAddress
	uint64_t IEEEAddress;  // IEEEAddress
	uint8_t MACCapability;  // AllocateAddress|SecurityCapability|Reserved|ReceiverOnWhenIdle|PowerSource|DeviceType|AlternatePANCoordinator  // MACCapability
} DeviceAnnounce_t;

typedef struct UserDescriptorResponse_tag {
	uint8_t SequenceNumber;  // SequenceNumber
	uint8_t Status;  // Status
	uint16_t NetworkAddressOfOnterest;  // NetworkAddressOfOnterest
	uint8_t LengthOfData;  // LengthOfData
	uint8_t *DataList;  // DataList
} UserDescriptorResponse_t;

typedef PACKED_STRUCT UserDescriptorNotify_tag {
	uint8_t SequenceNumber;  // SequenceNumber
	uint8_t Status;  // Status
	uint16_t NetworkAddressOfOnterest;  // NetworkAddressOfOnterest
} UserDescriptorNotify_t;

typedef struct ComplexDescriptorResponse_tag {
	uint8_t SequenceNumber;  // SequenceNumber
	uint8_t Status;  // Status
	uint16_t NetworkAddressOfOnterest;  // NetworkAddressOfOnterest
	uint8_t Length;  // Length
	uint8_t XMLTag;  // XMLTag
	uint8_t FieldCount;  // FieldCount
	uint8_t *FieldValues;  // FieldValues
} ComplexDescriptorResponse_t;

typedef PACKED_STRUCT ManagementLeaveResponse_tag {
	uint8_t SequenceNumber;  // SequenceNumber
	uint8_t Status;  // Status
} ManagementLeaveResponse_t;

typedef PACKED_STRUCT LeaveIndication_tag {
	uint64_t ExtendedAddress;  // ExtendedAddress
	uint8_t RejoinStatus;  // RejoinStatus
} LeaveIndication_t;

typedef PACKED_STRUCT ManagementNetworkUpdateResponse_tag {
	uint8_t SequenceNumber;  // SequenceNumber
	uint8_t Status;  // Status
	uint16_t TotalTransmission;  // TotalTransmission
	uint16_t TransmissionFailures;  // TransmissionFailures
	uint32_t ScannedChannels;  // ScannedChannels
	uint8_t ScannedChannelListCount;  // ScannedChannelListCount
	uint8_t ChannelList;  // ChannelList
} ManagementNetworkUpdateResponse_t;

typedef PACKED_STRUCT SystemServerDiscoveryResponse_tag {
	uint8_t SequenceNumber;  // SequenceNumber
	uint8_t Status;  // Status
	uint16_t ServerMask;  // ServerMask
} SystemServerDiscoveryResponse_t;

#endif  /* ZIGBEESTACKANDNODEMANAGEMENTCOMMANDS_ENABLE */

#if ENTIREPROFILE_ENABLE
/* DeviceType */
typedef enum ManagementLQIResponse_ListOfEntries_BitMapOfAttributes_DeviceType_tag {
	ManagementLQIResponse_ListOfEntries_BitMapOfAttributes_DeviceType_Coordinator = 0x00,
	ManagementLQIResponse_ListOfEntries_BitMapOfAttributes_DeviceType_Router = 0x01,
	ManagementLQIResponse_ListOfEntries_BitMapOfAttributes_DeviceType_EndDevice = 0x02
} ManagementLQIResponse_ListOfEntries_BitMapOfAttributes_DeviceType_t;

/* PermitJoinStatus */
typedef enum ManagementLQIResponse_ListOfEntries_BitMapOfAttributes_PermitJoinStatus_tag {
	ManagementLQIResponse_ListOfEntries_BitMapOfAttributes_PermitJoinStatus_Off = 0x00,
	ManagementLQIResponse_ListOfEntries_BitMapOfAttributes_PermitJoinStatus_On = 0x01
} ManagementLQIResponse_ListOfEntries_BitMapOfAttributes_PermitJoinStatus_t;

/* Relationship */
typedef enum ManagementLQIResponse_ListOfEntries_BitMapOfAttributes_Relationship_tag {
	ManagementLQIResponse_ListOfEntries_BitMapOfAttributes_Relationship_Parent = 0x00,
	ManagementLQIResponse_ListOfEntries_BitMapOfAttributes_Relationship_Child = 0x01,
	ManagementLQIResponse_ListOfEntries_BitMapOfAttributes_Relationship_Sibling = 0x02
} ManagementLQIResponse_ListOfEntries_BitMapOfAttributes_Relationship_t;

/* RxOnWhenIdleStatus */
typedef enum ManagementLQIResponse_ListOfEntries_BitMapOfAttributes_RxOnWhenIdleStatus_tag {
	ManagementLQIResponse_ListOfEntries_BitMapOfAttributes_RxOnWhenIdleStatus_Off = 0x00,
	ManagementLQIResponse_ListOfEntries_BitMapOfAttributes_RxOnWhenIdleStatus_On = 0x01
} ManagementLQIResponse_ListOfEntries_BitMapOfAttributes_RxOnWhenIdleStatus_t;

typedef PACKED_STRUCT ManagementLQIResponse_tag {
	uint8_t SequenceNumber;  // SequenceNumber
	uint8_t Status;  // Status
	uint8_t NeighbourTableEntries;  // NeighbourTableEntries
	uint8_t NeighbourTableListCount;  // NeighbourTableListCount
	uint8_t StartIndex;  // StartIndex
	struct {
		uint16_t NWKAddress;  // NWKAddress
		uint64_t ExtendedPANID;  // ExtendedPANID
		uint64_t IEEEAddress;  // IEEEAddress
		uint8_t Depth;  // Depth
		uint8_t LinkQuality;  // LinkQuality
		uint8_t BitMapOfAttributes;  // RxOnWhenIdleStatus|Relationship|PermitJoinStatus|DeviceType  // BitMapOfAttributes
	} ListOfEntries;  // ListOfEntries
} ManagementLQIResponse_t;

/* Complete */
typedef enum AttributeDiscoveryResponse_Complete_tag {
	AttributeDiscoveryResponse_Complete_MoreAttributesToFollow = 0x00,
	AttributeDiscoveryResponse_Complete_ThisWasTheLastAttribute = 0x01
} AttributeDiscoveryResponse_Complete_t;

typedef PACKED_STRUCT AttributeDiscoveryResponse_tag {
	AttributeDiscoveryResponse_Complete_t Complete;  // Complete
	uint8_t AttributeType;  // AttributeType
	uint16_t AttributeID;  // AttributeID
} AttributeDiscoveryResponse_t;

typedef PACKED_STRUCT AuthenticateResponse_tag {
	uint64_t IEEEAddressOfTtheGateway;  // IEEEAddressOfTtheGateway
	uint16_t EncryptedKey;  // EncryptedKey
	uint8_t MIC;  // MIC
	uint64_t IEEEAddressOfTheInitiatingNode;  // IEEEAddressOfTheInitiatingNode
	uint8_t ActiveKeySequenceNumber;  // ActiveKeySequenceNumber
	uint8_t Channel;  // Channel
	uint16_t ShortPANID;  // ShortPANID
	uint64_t ExtendedPANID;  // ExtendedPANID
} AuthenticateResponse_t;

typedef PACKED_STRUCT CodeResponse_tag {
	uint8_t Status;  // Status
} CodeResponse_t;

typedef PACKED_STRUCT ConfigureReportingResponse_tag {
	uint8_t SequenceNumber;  // SequenceNumber
	uint16_t SrcAddress;  // SrcAddress
	uint8_t Endpoint;  // Endpoint
	uint16_t ClusterID;  // ClusterID
	uint8_t Status;  // Status
} ConfigureReportingResponse_t;

typedef struct ReadIndividualAttributeResponse_tag {
	uint8_t SequenceNumber;  // SequenceNumber
	uint16_t SrcAddress;  // SrcAddress
	uint8_t Endpoint;  // Endpoint
	uint16_t ClusterID;  // ClusterID
	uint16_t AttributeEnum;  // AttributeEnum
	uint8_t AttributeStatus;  // AttributeStatus
	uint8_t AttributeDataType;  // AttributeDataType
	uint16_t SizeOfTheAttributes;  // SizeOfTheAttributes
	uint8_t *DataByteList;  // DataByteList
} ReadIndividualAttributeResponse_t;

typedef struct WriteAttributeResponse_tag {
	uint8_t SequenceNumber;  // SequenceNumber
	uint16_t SrcAddress;  // SrcAddress
	uint8_t Endpoint;  // Endpoint
	uint16_t ClusterID;  // ClusterID
	uint16_t AttributeEnum;  // AttributeEnum
	uint8_t AttributeStatus;  // AttributeStatus
	uint8_t AttributeDataType;  // AttributeDataType
	uint16_t SizeOfTheAttributes;  // SizeOfTheAttributes
	uint8_t *DataByteList;  // DataByteList
} WriteAttributeResponse_t;

typedef struct ReportIndividualAttributeResponse_tag {
	uint8_t SequenceNumber;  // SequenceNumber
	uint16_t SrcAddress;  // SrcAddress
	uint8_t Endpoint;  // Endpoint
	uint16_t ClusterID;  // ClusterID
	uint16_t AttributeEnum;  // AttributeEnum
	uint8_t AttributeStatus;  // AttributeStatus
	uint8_t AttributeDataType;  // AttributeDataType
	uint16_t SizeOfTheAttributes;  // SizeOfTheAttributes
	uint8_t *DataByteList;  // DataByteList
} ReportIndividualAttributeResponse_t;

typedef PACKED_STRUCT DefaultResponse_tag {
	uint8_t SequenceNumber;  // SequenceNumber
	uint8_t Endpoint;  // Endpoint
	uint16_t ClusterID;  // ClusterID
	uint8_t CommandID;  // CommandID
	uint8_t StatusCode;  // StatusCode
} DefaultResponse_t;

#endif  /* ENTIREPROFILE_ENABLE */

#if GROUPCLUSTERCOMMANDS_ENABLE
typedef PACKED_STRUCT AddGroupResponse_tag {
	uint8_t SequenceNumber;  // SequenceNumber
	uint8_t Endpoint;  // Endpoint
	uint16_t ClusterID;  // ClusterID
	uint8_t Status;  // Status
	uint16_t GroupID;  // GroupID
} AddGroupResponse_t;

typedef PACKED_STRUCT ViewGroupResponse_tag {
	uint8_t SequenceNumber;  // SequenceNumber
	uint8_t Endpoint;  // Endpoint
	uint16_t ClusterID;  // ClusterID
	uint8_t Status;  // Status
	uint16_t GroupID;  // GroupID
} ViewGroupResponse_t;

typedef struct GetGroupMembershipResponse_tag {
	uint8_t SequenceNumber;  // SequenceNumber
	uint8_t Endpoint;  // Endpoint
	uint16_t ClusterID;  // ClusterID
	uint8_t Capacity;  // Capacity
	uint8_t GroupCount;  // GroupCount
	uint16_t *GroupList;  // GroupList
} GetGroupMembershipResponse_t;

typedef PACKED_STRUCT RemoveGroupResponse_tag {
	uint8_t SequenceNumber;  // SequenceNumber
	uint8_t Endpoint;  // Endpoint
	uint16_t ClusterID;  // ClusterID
	uint8_t Status;  // Status
	uint16_t GroupID;  // GroupID
} RemoveGroupResponse_t;

#endif  /* GROUPCLUSTERCOMMANDS_ENABLE */

#if DIAGNOSTICSCLUSTERCOMMANDS_ENABLE
/* Status */
typedef enum DiagnosticResponse_Status_tag {
	DiagnosticResponse_Status_DelayResponse = 0x01
} DiagnosticResponse_Status_t;

typedef struct DiagnosticResponse_tag {
	uint8_t SeqNum;  // SeqNum
	uint8_t srcEndpoint;  // srcEndpoint
	uint16_t ClusterId;  // ClusterId
	DiagnosticResponse_Status_t Status;  // Status
	uint32_t RequestLatency;  // Request Latency
	uint32_t ResponseLatency;  // Response Latency
	uint32_t Offset;  // Offset
	uint8_t SequenceNumber;  // Sequence Number
	uint8_t PayloadSize;  // Payload Size
	uint8_t *Payload;  // OTA received payload
} DiagnosticResponse_t;

#endif  /* DIAGNOSTICSCLUSTERCOMMANDS_ENABLE */

#if SCENESCLUSTERCOMMANDS_ENABLE
typedef struct ViewSceneResponse_tag {
	uint8_t SequenceNumber;  // SequenceNumber
	uint8_t Endpoint;  // Endpoint
	uint16_t ClusterID;  // ClusterID
	uint8_t Status;  // Status
	uint16_t GroupID;  // GroupID
	uint8_t SceneID;  // SceneID
	uint16_t TransitionTime;  // TransitionTime
	uint8_t SceneNameLength;  // SceneNameLength
	uint8_t SceneNameMaxLength;  // SceneNameMaxLength
	uint8_t *SceneNameData;  // SceneNameData
	uint16_t ExtensionsLength;  // ExtensionsLength
	uint16_t ExtensionsMaxLength;  // ExtensionsMaxLength
	uint8_t *ExtensionsData;  // ExtensionsData
} ViewSceneResponse_t;

typedef struct ViewEnhancedSceneResponse_tag {
	uint8_t SequenceNumber;  // SequenceNumber
	uint8_t Endpoint;  // Endpoint
	uint16_t ClusterID;  // ClusterID
	uint8_t Status;  // Status
	uint16_t GroupID;  // GroupID
	uint8_t SceneID;  // SceneID
	uint16_t TransitionTime;  // TransitionTime
	uint8_t SceneNameLength;  // SceneNameLength
	uint8_t SceneNameMaxLength;  // SceneNameMaxLength
	uint8_t *SceneNameData;  // SceneNameData
	uint16_t ExtensionsLength;  // ExtensionsLength
	uint16_t ExtensionsMaxLength;  // ExtensionsMaxLength
	uint8_t *ExtensionsData;  // ExtensionsData
} ViewEnhancedSceneResponse_t;

typedef PACKED_STRUCT AddSceneResponse_tag {
	uint8_t SequenceNumber;  // SequenceNumber
	uint8_t Endpoint;  // Endpoint
	uint16_t ClusterID;  // ClusterID
	uint8_t Status;  // Status
	uint16_t GroupID;  // GroupID
	uint8_t SceneID;  // SceneID
} AddSceneResponse_t;

typedef PACKED_STRUCT CopySceneResponse_tag {
	uint8_t SequenceNumber;  // SequenceNumber
	uint8_t Endpoint;  // Endpoint
	uint16_t ClusterID;  // ClusterID
	uint8_t Status;  // Status
	uint16_t FromGroupID;  // FromGroupID
	uint8_t FromSceneID;  // FromSceneID
} CopySceneResponse_t;

typedef PACKED_STRUCT RemoveSceneResponse_tag {
	uint8_t SequenceNumber;  // SequenceNumber
	uint8_t Endpoint;  // Endpoint
	uint16_t ClusterID;  // ClusterID
	uint8_t Status;  // Status
	uint16_t GroupID;  // GroupID
	uint8_t SceneID;  // SceneID
} RemoveSceneResponse_t;

typedef PACKED_STRUCT RemoveAllSceneResponse_tag {
	uint8_t SequenceNumber;  // SequenceNumber
	uint8_t Endpoint;  // Endpoint
	uint16_t ClusterID;  // ClusterID
	uint8_t Status;  // Status
	uint16_t GroupID;  // GroupID
} RemoveAllSceneResponse_t;

typedef PACKED_STRUCT AddEnhancedSceneResponse_tag {
	uint8_t SeqNum;  // SeqNum
	uint8_t srcEndpoint;  // srcEndpoint
	uint16_t ClusterId;  // ClusterId
	uint8_t Status;  // Status
	uint16_t GroupID;  // GroupID
	uint8_t SceneID;  // SceneID
} AddEnhancedSceneResponse_t;

typedef PACKED_STRUCT StoreSceneResponse_tag {
	uint8_t SequenceNumber;  // SequenceNumber
	uint8_t Endpoint;  // Endpoint
	uint16_t ClusterID;  // ClusterID
	uint8_t Status;  // Status
	uint16_t GroupID;  // GroupID
	uint8_t SceneID;  // SceneID
} StoreSceneResponse_t;

typedef struct SceneMembershipResponse_tag {
	uint8_t SequenceNumber;  // SequenceNumber
	uint8_t Endpoint;  // Endpoint
	uint16_t ClusterID;  // ClusterID
	uint8_t Status;  // Status
	uint8_t Capacity;  // Capacity
	uint16_t GroupID;  // GroupID
	uint8_t SceneCount;  // SceneCount
	uint8_t *SceneList;  // SceneList
} SceneMembershipResponse_t;

#endif  /* SCENESCLUSTERCOMMANDS_ENABLE */

#if TOUCHLINKCOMMANDS_ENABLE
/* Status */
typedef enum TouchlinkStatus_Status_tag {
	TouchlinkStatus_Status_Success = 0x00,
	TouchlinkStatus_Status_Failure = 0x01
} TouchlinkStatus_Status_t;

typedef PACKED_STRUCT TouchlinkStatus_tag {
	TouchlinkStatus_Status_t Status;  // Status
	uint16_t JoinedNodeShortAddress;  // JoinedNodeShortAddress
} TouchlinkStatus_t;

#endif  /* TOUCHLINKCOMMANDS_ENABLE */

#if IASCLUSTERCOMMANDS_ENABLE
/* SourceAddressMode */
typedef enum ZoneStatusChangeNotification_SourceAddressMode_tag {
	ZoneStatusChangeNotification_SourceAddressMode_NO_PAN = 0x00,
	ZoneStatusChangeNotification_SourceAddressMode_Reserved = 0x01,
	ZoneStatusChangeNotification_SourceAddressMode_Address_16b = 0x02,
	ZoneStatusChangeNotification_SourceAddressMode_Address_64b = 0x03
} ZoneStatusChangeNotification_SourceAddressMode_t;

typedef struct ZoneStatusChangeNotification_tag {
	uint8_t SequenceNumber;  // SequenceNumber
	uint8_t EndPoint;  // EndPoint
	uint16_t ClusteID;  // ClusteID
	ZoneStatusChangeNotification_SourceAddressMode_t SourceAddressMode;  // SourceAddressMode
	union {
		uint16_t Address_16b;  // Address field contains a 16_bit short address.
		uint64_t Address_64b;  // Address field contains a 64_bit extended address.
	} SourceAddress;  // MHR, Addressing fields. 0xFFFF Broadcast PAN identifier.
	uint16_t ZoneStatus;  // ZoneStatus
	uint8_t ExtendedStatus;  // ExtendedStatus
	uint8_t ZoneID;  // ZoneID
	uint16_t Delay;  // Delay
} ZoneStatusChangeNotification_t;

#endif  /* IASCLUSTERCOMMANDS_ENABLE */

#if EXPORTINGPERSISTENTDATATOHOST_ENABLE
typedef PACKED_STRUCT HostPersistentDataManagerAvailableResponse_tag {
	uint16_t PayloadLength;  // The FSCI length field
} HostPersistentDataManagerAvailableResponse_t;

/* Status */
typedef enum LoadRecordResponse_Status_tag {
	LoadRecordResponse_Status_NoRecordFound = 0x00,
	LoadRecordResponse_Status_RecordRecovered = 0x01
} LoadRecordResponse_Status_t;

typedef struct LoadRecordResponse_tag {
	LoadRecordResponse_Status_t Status;  // Status
	uint16_t Record;  // Record
	uint32_t TotalSize;  // TotalSize
	uint32_t TotalNumberOfBlocks;  // TotalNumberOfBlocks
	uint32_t CurrentBlock;  // CurrentBlock
	uint32_t BlockSize;  // BlockSize
	uint8_t *Parameters;  // Parameters
} LoadRecordResponse_t;

typedef PACKED_STRUCT SaveRecordResponse_tag {
	uint16_t Record;  // Record
	uint32_t TotalSize;  // TotalSize
	uint32_t TotalNumberOfBlocks;  // TotalNumberOfBlocks
	uint32_t CurrentBlock;  // CurrentBlock
	uint32_t BlockSize;  // BlockSize
} SaveRecordResponse_t;

#endif  /* EXPORTINGPERSISTENTDATATOHOST_ENABLE */

#if EXTENDEDUTILITIES_ENABLE
/* Status */
typedef enum RouterDiscoveryConfirm_Status_tag {
	RouterDiscoveryConfirm_Status_Success = 0x00,
	RouterDiscoveryConfirm_Status_Failed = 0xD0
} RouterDiscoveryConfirm_Status_t;

/* NwkStatus */
typedef enum RouterDiscoveryConfirm_NwkStatus_tag {
	RouterDiscoveryConfirm_NwkStatus_SUCCESS = 0x00,
	RouterDiscoveryConfirm_NwkStatus_COUNTER_ERROR = 0xDB,
	RouterDiscoveryConfirm_NwkStatus_IMPROPER_KEY_TYPE = 0xDC,
	RouterDiscoveryConfirm_NwkStatus_IMPROPER_SECURITY_LEVEL = 0xDD,
	RouterDiscoveryConfirm_NwkStatus_UNSUPPORTED_LEGACY = 0xDE,
	RouterDiscoveryConfirm_NwkStatus_BEACON_LOSS = 0xE0,
	RouterDiscoveryConfirm_NwkStatus_CHANNEL_ACCESS_FAILURE = 0xE1,
	RouterDiscoveryConfirm_NwkStatus_DENIED = 0xE2,
	RouterDiscoveryConfirm_NwkStatus_DISABLE_TRX_FAILURE = 0xE3,
	RouterDiscoveryConfirm_NwkStatus_FAILED_SECURITY_CHECK = 0xE4,
	RouterDiscoveryConfirm_NwkStatus_FRAME_TOO_LONG = 0xE5,
	RouterDiscoveryConfirm_NwkStatus_INVALID_GTS = 0xE6,
	RouterDiscoveryConfirm_NwkStatus_INVALID_HANDLE = 0xE7,
	RouterDiscoveryConfirm_NwkStatus_INVALID_PARAMETER = 0xE8,
	RouterDiscoveryConfirm_NwkStatus_NO_ACK = 0xE9,
	RouterDiscoveryConfirm_NwkStatus_NO_BEACON = 0xEA,
	RouterDiscoveryConfirm_NwkStatus_NO_DATA = 0xEB,
	RouterDiscoveryConfirm_NwkStatus_NO_SHORT_ADDRESS = 0xEC,
	RouterDiscoveryConfirm_NwkStatus_OUT_OF_CAP = 0xED,
	RouterDiscoveryConfirm_NwkStatus_PAN_ID_CONFLICT = 0xEE,
	RouterDiscoveryConfirm_NwkStatus_REALIGNMENT = 0xEF,
	RouterDiscoveryConfirm_NwkStatus_TRANSACTION_EXPIRED = 0xF0,
	RouterDiscoveryConfirm_NwkStatus_TRANSACTION_OVERFLOW = 0xF1,
	RouterDiscoveryConfirm_NwkStatus_TX_ACTIVE = 0xF2,
	RouterDiscoveryConfirm_NwkStatus_UNAVAILABLE_KEY = 0xF3,
	RouterDiscoveryConfirm_NwkStatus_UNSUPPORTED_ATTRIBUTE = 0xF4,
	RouterDiscoveryConfirm_NwkStatus_SCAN_IN_PROGRESS = 0xF5
} RouterDiscoveryConfirm_NwkStatus_t;

typedef PACKED_STRUCT RouterDiscoveryConfirm_tag {
	RouterDiscoveryConfirm_Status_t Status;  // Status
	RouterDiscoveryConfirm_NwkStatus_t NwkStatus;  // NwkStatus
} RouterDiscoveryConfirm_t;

/* DestAddressingMode */
typedef enum APSDataConfirmFail_DestAddressingMode_tag {
	APSDataConfirmFail_DestAddressingMode_NO_PAN = 0x00,
	APSDataConfirmFail_DestAddressingMode_Reserved = 0x01,
	APSDataConfirmFail_DestAddressingMode_Address_16b = 0x02,
	APSDataConfirmFail_DestAddressingMode_Address_64b = 0x03
} APSDataConfirmFail_DestAddressingMode_t;

typedef struct APSDataConfirmFail_tag {
	uint8_t Status;  // Status
	uint8_t SourceEndPoint;  // SourceEndPoint
	uint8_t DestinationEndPoint;  // DestinationEndPoint
	APSDataConfirmFail_DestAddressingMode_t DestAddressingMode;  // DestAddressingMode
	union {
		uint16_t Address_16b;  // Address field contains a 16_bit short address.
		uint64_t Address_64b;  // Address field contains a 64_bit extended address.
	} DestinationAddress;  // MHR, Addressing fields. 0xFFFF Broadcast PAN identifier.
	uint8_t SequenceNumber;  // SequenceNumber
} APSDataConfirmFail_t;

#endif  /* EXTENDEDUTILITIES_ENABLE */

typedef enum zigFsciIds_tag
{
	GetVersion_FSCI_ID = 0x3710,
	SetExtendedPANID_FSCI_ID = 0x3720,
	SetChannelMask_FSCI_ID = 0x3721,
	SetSecurityStateKey_FSCI_ID = 0x3722,
	SetDeviceType_FSCI_ID = 0x3723,
	StartNetworkMessage_FSCI_ID = 0x3724,
	StartNetworkScan_FSCI_ID = 0x3725,
	Code_FSCI_ID = 0x3729,
	ZLO_ZLLFactoryNewReset_FSCI_ID = 0x3713,
	PermitJoinStatusOnTheTarget_FSCI_ID = 0x3714,
	Reset_FSCI_ID = 0x3711,
	ErasePersistentData_FSCI_ID = 0x3712,
	Bind_FSCI_ID = 0x3730,
	Unbind_FSCI_ID = 0x3731,
	NetworkAddressRequest_FSCI_ID = 0x3740,
	IEEEAddressRequest_FSCI_ID = 0x3741,
	NodeDescriptorRequest_FSCI_ID = 0x3742,
	SimpleDescriptorRequest_FSCI_ID = 0x3743,
	PowerDescriptorRequest_FSCI_ID = 0x3744,
	ActiveEndpointRequest_FSCI_ID = 0x3745,
	MatchDescriptorRequest_FSCI_ID = 0x3746,
	RemoveDevice_FSCI_ID = 0x3726,
	UserDescriptorSet_FSCI_ID = 0x372B,
	UserDescriptorRequest_FSCI_ID = 0x372C,
	SetPollIntervalRequest_FSCI_ID = 0x372D,
	AddInOutClustersRequest_FSCI_ID = 0x372E,
	FindAndBindRequest_FSCI_ID = 0x372F,
	ComplexDescriptorRequest_FSCI_ID = 0x3734,
	ManagementLeaveRequest_FSCI_ID = 0x3747,
	PermitJoiningRequest_FSCI_ID = 0x3749,
	ManagementNetworkUpdateRequest_FSCI_ID = 0x374A,
	SystemServerDiscoveryRequest_FSCI_ID = 0x374B,
	ManagementLQIRequest_FSCI_ID = 0x374E,
	ReadAttributeRequest_FSCI_ID = 0x3800,
	WriteAttributeRequest_FSCI_ID = 0x3810,
	AttributeDiscoveryRequest_FSCI_ID = 0x3840,
	EnablePermissionsControlledJoins_FSCI_ID = 0x3727,
	AuthenticateDevice_FSCI_ID = 0x3728,
	ConfigureReportingRequest_FSCI_ID = 0x3820,
	AddGroup_FSCI_ID = 0x3760,
	ViewGroup_FSCI_ID = 0x3761,
	GetGroupMembership_FSCI_ID = 0x3762,
	RemoveGroup_FSCI_ID = 0x3763,
	RemoveAllGroups_FSCI_ID = 0x3764,
	AddGroupIfIdentify_FSCI_ID = 0x3765,
	IdentifySend_FSCI_ID = 0x3770,
	IdentifyQuery_FSCI_ID = 0x3771,
	IdentifyTriggerEffects_FSCI_ID = 0x37E0,
	MoveToLevel_FSCI_ID = 0x3780,
	MoveToLevelWithWithoutOnOff_FSCI_ID = 0x3781,
	MoveStep_FSCI_ID = 0x3782,
	MoveStopWithOnOff_FSCI_ID = 0x3784,
	MoveToClosestFrequency_FSCI_ID = 0x3785,
	OnOffWithEffectsSend_FSCI_ID = 0x3794,
	OnOffWithNoEffects_FSCI_ID = 0x3792,
	OnOffTimedSend_FSCI_ID = 0x3793,
	DiagnosticRequest_FSCI_ID = 0x3799,
	ViewScene_FSCI_ID = 0x37A0,
	AddScene_FSCI_ID = 0x37A1,
	RemoveScene_FSCI_ID = 0x37A2,
	RemoveAllScenes_FSCI_ID = 0x37A3,
	StoreScene_FSCI_ID = 0x37A4,
	RecallScene_FSCI_ID = 0x37A5,
	SceneMembershipRequest_FSCI_ID = 0x37A6,
	AddEnhancedScene_FSCI_ID = 0x37A7,
	ViewEnhancedScene_FSCI_ID = 0x37A8,
	CopyScene_FSCI_ID = 0x37A9,
	MoveToHue_FSCI_ID = 0x37B0,
	MoveHue_FSCI_ID = 0x37B1,
	StepHue_FSCI_ID = 0x37B2,
	MoveToSaturation_FSCI_ID = 0x37B3,
	MoveSaturation_FSCI_ID = 0x37B4,
	StepSaturation_FSCI_ID = 0x37B5,
	MoveToHueAndSaturation_FSCI_ID = 0x37B6,
	MoveToColour_FSCI_ID = 0x37B7,
	MoveColour_FSCI_ID = 0x37B8,
	EnhancedMoveToHue_FSCI_ID = 0x37BA,
	EnhancedMoveHue_FSCI_ID = 0x37BB,
	EnhancedStepHue_FSCI_ID = 0x37BC,
	EnhancedMoveToHueAndSaturation_FSCI_ID = 0x37BD,
	ColourLoopSet_FSCI_ID = 0x37BE,
	StopMoveStep_FSCI_ID = 0x37BF,
	MoveToColourTemperature_FSCI_ID = 0x37C0,
	MoveColourTemperature_FSCI_ID = 0x37C1,
	StepColourTemperature_FSCI_ID = 0x37C2,
	InitiateTouchlink_FSCI_ID = 0x37D0,
	TouchLinkFactoryResetTarget_FSCI_ID = 0x37D2,
	LockUnlockDoor_FSCI_ID = 0x37F0,
	IASZoneEnrollResponse_FSCI_ID = 0x3B00,
	RawAPSDataRequest_FSCI_ID = 0x3C30,
	Status_FSCI_ID = 0xB700,
	LogMessage_FSCI_ID = 0xB701,
	DataIndication_FSCI_ID = 0xB702,
	NodeClusterList_FSCI_ID = 0xB703,
	NodeClusterAttributeList_FSCI_ID = 0xB704,
	NodeCommandIDList_FSCI_ID = 0xB705,
	VersionList_FSCI_ID = 0xB710,
	NetworkJoinedFormed_FSCI_ID = 0xB724,
	PermitJoinStatusResponse_FSCI_ID = 0xB714,
	NonFactoryNewRestart_FSCI_ID = 0xB706,
	FactoryNewRestart_FSCI_ID = 0xB707,
	BindResponse_FSCI_ID = 0xB730,
	UnbindResponse_FSCI_ID = 0xB731,
	NetworkAddressResponse_FSCI_ID = 0xB740,
	IEEEAddressResponse_FSCI_ID = 0xB741,
	NodeDescriptorResponse_FSCI_ID = 0xB742,
	SimpleDescriptorResponse_FSCI_ID = 0xB743,
	PowerDescriptorResponse_FSCI_ID = 0xB744,
	ActiveEndpointResponse_FSCI_ID = 0xB745,
	MatchDescriptorResponse_FSCI_ID = 0xB746,
	DeviceAnnounce_FSCI_ID = 0x374D,
	UserDescriptorResponse_FSCI_ID = 0xB72C,
	UserDescriptorNotify_FSCI_ID = 0xB72B,
	ComplexDescriptorResponse_FSCI_ID = 0xB734,
	ManagementLeaveResponse_FSCI_ID = 0xB747,
	LeaveIndication_FSCI_ID = 0xB748,
	ManagementNetworkUpdateResponse_FSCI_ID = 0xB74A,
	SystemServerDiscoveryResponse_FSCI_ID = 0xB74B,
	ManagementLQIResponse_FSCI_ID = 0xB74E,
	AttributeDiscoveryResponse_FSCI_ID = 0xB840,
	AuthenticateResponse_FSCI_ID = 0xB728,
	CodeResponse_FSCI_ID = 0xB729,
	ConfigureReportingResponse_FSCI_ID = 0xB820,
	ReadIndividualAttributeResponse_FSCI_ID = 0xB800,
	WriteAttributeResponse_FSCI_ID = 0xB810,
	ReportIndividualAttributeResponse_FSCI_ID = 0xB802,
	DefaultResponse_FSCI_ID = 0xB801,
	AddGroupResponse_FSCI_ID = 0xB760,
	ViewGroupResponse_FSCI_ID = 0xB761,
	GetGroupMembershipResponse_FSCI_ID = 0xB762,
	RemoveGroupResponse_FSCI_ID = 0xB763,
	DiagnosticResponse_FSCI_ID = 0xB799,
	ViewSceneResponse_FSCI_ID = 0xB7A0,
	ViewEnhancedSceneResponse_FSCI_ID = 0xB7A8,
	AddSceneResponse_FSCI_ID = 0xB7A1,
	CopySceneResponse_FSCI_ID = 0xB7A9,
	RemoveSceneResponse_FSCI_ID = 0xB7A2,
	RemoveAllSceneResponse_FSCI_ID = 0xB7A3,
	AddEnhancedSceneResponse_FSCI_ID = 0xB7A7,
	StoreSceneResponse_FSCI_ID = 0xB7A4,
	SceneMembershipResponse_FSCI_ID = 0xB7A6,
	TouchlinkStatus_FSCI_ID = 0xB7D1,
	ZoneStatusChangeNotification_FSCI_ID = 0xBB01,
	HostPersistentDataManagerAvailableResponse_FSCI_ID = 0xBA00,
	LoadRecordResponse_FSCI_ID = 0xB901,
	SaveRecordResponse_FSCI_ID = 0xB900,
	RouterDiscoveryConfirm_FSCI_ID = 0xBE01,
	APSDataConfirmFail_FSCI_ID = 0xBE02,
} zigFsciIds_t;

typedef struct zigEvtContainer_tag
{
	uint16_t id;
	union {
#if ZIGBEESTACKANDNODEMANAGEMENTCOMMANDS_ENABLE
		Status_t Status;
		LogMessage_t LogMessage;
		DataIndication_t DataIndication;
		NodeClusterList_t NodeClusterList;
		NodeClusterAttributeList_t NodeClusterAttributeList;
		NodeCommandIDList_t NodeCommandIDList;
		VersionList_t VersionList;
		NetworkJoinedFormed_t NetworkJoinedFormed;
		PermitJoinStatusResponse_t PermitJoinStatusResponse;
		NonFactoryNewRestart_t NonFactoryNewRestart;
		FactoryNewRestart_t FactoryNewRestart;
		BindResponse_t BindResponse;
		UnbindResponse_t UnbindResponse;
		NetworkAddressResponse_t NetworkAddressResponse;
		IEEEAddressResponse_t IEEEAddressResponse;
		NodeDescriptorResponse_t NodeDescriptorResponse;
		SimpleDescriptorResponse_t SimpleDescriptorResponse;
		PowerDescriptorResponse_t PowerDescriptorResponse;
		ActiveEndpointResponse_t ActiveEndpointResponse;
		MatchDescriptorResponse_t MatchDescriptorResponse;
		DeviceAnnounce_t DeviceAnnounce;
		UserDescriptorResponse_t UserDescriptorResponse;
		UserDescriptorNotify_t UserDescriptorNotify;
		ComplexDescriptorResponse_t ComplexDescriptorResponse;
		ManagementLeaveResponse_t ManagementLeaveResponse;
		LeaveIndication_t LeaveIndication;
		ManagementNetworkUpdateResponse_t ManagementNetworkUpdateResponse;
		SystemServerDiscoveryResponse_t SystemServerDiscoveryResponse;
#endif  /* ZIGBEESTACKANDNODEMANAGEMENTCOMMANDS_ENABLE */

#if ENTIREPROFILE_ENABLE
		ManagementLQIResponse_t ManagementLQIResponse;
		AttributeDiscoveryResponse_t AttributeDiscoveryResponse;
		AuthenticateResponse_t AuthenticateResponse;
		CodeResponse_t CodeResponse;
		ConfigureReportingResponse_t ConfigureReportingResponse;
		ReadIndividualAttributeResponse_t ReadIndividualAttributeResponse;
		WriteAttributeResponse_t WriteAttributeResponse;
		ReportIndividualAttributeResponse_t ReportIndividualAttributeResponse;
		DefaultResponse_t DefaultResponse;
#endif  /* ENTIREPROFILE_ENABLE */

#if GROUPCLUSTERCOMMANDS_ENABLE
		AddGroupResponse_t AddGroupResponse;
		ViewGroupResponse_t ViewGroupResponse;
		GetGroupMembershipResponse_t GetGroupMembershipResponse;
		RemoveGroupResponse_t RemoveGroupResponse;
#endif  /* GROUPCLUSTERCOMMANDS_ENABLE */

#if DIAGNOSTICSCLUSTERCOMMANDS_ENABLE
		DiagnosticResponse_t DiagnosticResponse;
#endif  /* DIAGNOSTICSCLUSTERCOMMANDS_ENABLE */

#if SCENESCLUSTERCOMMANDS_ENABLE
		ViewSceneResponse_t ViewSceneResponse;
		ViewEnhancedSceneResponse_t ViewEnhancedSceneResponse;
		AddSceneResponse_t AddSceneResponse;
		CopySceneResponse_t CopySceneResponse;
		RemoveSceneResponse_t RemoveSceneResponse;
		RemoveAllSceneResponse_t RemoveAllSceneResponse;
		AddEnhancedSceneResponse_t AddEnhancedSceneResponse;
		StoreSceneResponse_t StoreSceneResponse;
		SceneMembershipResponse_t SceneMembershipResponse;
#endif  /* SCENESCLUSTERCOMMANDS_ENABLE */

#if TOUCHLINKCOMMANDS_ENABLE
		TouchlinkStatus_t TouchlinkStatus;
#endif  /* TOUCHLINKCOMMANDS_ENABLE */

#if IASCLUSTERCOMMANDS_ENABLE
		ZoneStatusChangeNotification_t ZoneStatusChangeNotification;
#endif  /* IASCLUSTERCOMMANDS_ENABLE */

#if EXPORTINGPERSISTENTDATATOHOST_ENABLE
		HostPersistentDataManagerAvailableResponse_t HostPersistentDataManagerAvailableResponse;
		LoadRecordResponse_t LoadRecordResponse;
		SaveRecordResponse_t SaveRecordResponse;
#endif  /* EXPORTINGPERSISTENTDATATOHOST_ENABLE */

#if EXTENDEDUTILITIES_ENABLE
		RouterDiscoveryConfirm_t RouterDiscoveryConfirm;
		APSDataConfirmFail_t APSDataConfirmFail;
#endif  /* EXTENDEDUTILITIES_ENABLE */
	} Data;
} zigEvtContainer_t;

typedef memStatus_t (*pfnZigEvtHandler)(zigEvtContainer_t *container, uint8_t *pPayload);

typedef struct zigEvtHandler_tag
{
	uint16_t id;
	pfnZigEvtHandler handlerFunc;
} zigEvtHandler_t;


/*==================================================================================================
Public function prototypes
==================================================================================================*/
#if ZIGBEESTACKANDNODEMANAGEMENTCOMMANDS_ENABLE
memStatus_t GetVersion(void *arg, uint8_t fsciInterface);
memStatus_t SetExtendedPANID(SetExtendedPANID_t *req, void *arg, uint8_t fsciInterface);
memStatus_t SetChannelMask(SetChannelMask_t *req, void *arg, uint8_t fsciInterface);
memStatus_t SetSecurityStateKey(SetSecurityStateKey_t *req, void *arg, uint8_t fsciInterface);
memStatus_t SetDeviceType(SetDeviceType_t *req, void *arg, uint8_t fsciInterface);
memStatus_t StartNetworkMessage(void *arg, uint8_t fsciInterface);
memStatus_t StartNetworkScan(void *arg, uint8_t fsciInterface);
memStatus_t Code(Code_t *req, void *arg, uint8_t fsciInterface);
memStatus_t ZLO_ZLLFactoryNewReset(void *arg, uint8_t fsciInterface);
memStatus_t PermitJoinStatusOnTheTarget(void *arg, uint8_t fsciInterface);
memStatus_t Reset(void *arg, uint8_t fsciInterface);
memStatus_t ErasePersistentData(void *arg, uint8_t fsciInterface);
memStatus_t Bind(Bind_t *req, void *arg, uint8_t fsciInterface);
memStatus_t Unbind(Unbind_t *req, void *arg, uint8_t fsciInterface);
memStatus_t NetworkAddressRequest(NetworkAddressRequest_t *req, void *arg, uint8_t fsciInterface);
memStatus_t IEEEAddressRequest(IEEEAddressRequest_t *req, void *arg, uint8_t fsciInterface);
memStatus_t NodeDescriptorRequest(NodeDescriptorRequest_t *req, void *arg, uint8_t fsciInterface);
memStatus_t SimpleDescriptorRequest(SimpleDescriptorRequest_t *req, void *arg, uint8_t fsciInterface);
memStatus_t PowerDescriptorRequest(PowerDescriptorRequest_t *req, void *arg, uint8_t fsciInterface);
memStatus_t ActiveEndpointRequest(ActiveEndpointRequest_t *req, void *arg, uint8_t fsciInterface);
memStatus_t MatchDescriptorRequest(MatchDescriptorRequest_t *req, void *arg, uint8_t fsciInterface);
memStatus_t RemoveDevice(RemoveDevice_t *req, void *arg, uint8_t fsciInterface);
memStatus_t UserDescriptorSet(UserDescriptorSet_t *req, void *arg, uint8_t fsciInterface);
memStatus_t UserDescriptorRequest(UserDescriptorRequest_t *req, void *arg, uint8_t fsciInterface);
memStatus_t SetPollIntervalRequest(SetPollIntervalRequest_t *req, void *arg, uint8_t fsciInterface);
memStatus_t AddInOutClustersRequest(AddInOutClustersRequest_t *req, void *arg, uint8_t fsciInterface);
memStatus_t FindAndBindRequest(FindAndBindRequest_t *req, void *arg, uint8_t fsciInterface);
memStatus_t ComplexDescriptorRequest(ComplexDescriptorRequest_t *req, void *arg, uint8_t fsciInterface);
memStatus_t ManagementLeaveRequest(ManagementLeaveRequest_t *req, void *arg, uint8_t fsciInterface);
memStatus_t PermitJoiningRequest(PermitJoiningRequest_t *req, void *arg, uint8_t fsciInterface);
memStatus_t ManagementNetworkUpdateRequest(ManagementNetworkUpdateRequest_t *req, void *arg, uint8_t fsciInterface);
memStatus_t SystemServerDiscoveryRequest(SystemServerDiscoveryRequest_t *req, void *arg, uint8_t fsciInterface);
memStatus_t ManagementLQIRequest(ManagementLQIRequest_t *req, void *arg, uint8_t fsciInterface);
#endif  /* ZIGBEESTACKANDNODEMANAGEMENTCOMMANDS_ENABLE */

#if ENTIREPROFILE_ENABLE
memStatus_t ReadAttributeRequest(ReadAttributeRequest_t *req, void *arg, uint8_t fsciInterface);
memStatus_t WriteAttributeRequest(WriteAttributeRequest_t *req, void *arg, uint8_t fsciInterface);
memStatus_t AttributeDiscoveryRequest(AttributeDiscoveryRequest_t *req, void *arg, uint8_t fsciInterface);
memStatus_t EnablePermissionsControlledJoins(EnablePermissionsControlledJoins_t *req, void *arg, uint8_t fsciInterface);
memStatus_t AuthenticateDevice(AuthenticateDevice_t *req, void *arg, uint8_t fsciInterface);
memStatus_t ConfigureReportingRequest(ConfigureReportingRequest_t *req, void *arg, uint8_t fsciInterface);
#endif  /* ENTIREPROFILE_ENABLE */

#if GROUPCLUSTERCOMMANDS_ENABLE
memStatus_t AddGroup(AddGroup_t *req, void *arg, uint8_t fsciInterface);
memStatus_t ViewGroup(ViewGroup_t *req, void *arg, uint8_t fsciInterface);
memStatus_t GetGroupMembership(GetGroupMembership_t *req, void *arg, uint8_t fsciInterface);
memStatus_t RemoveGroup(RemoveGroup_t *req, void *arg, uint8_t fsciInterface);
memStatus_t RemoveAllGroups(RemoveAllGroups_t *req, void *arg, uint8_t fsciInterface);
memStatus_t AddGroupIfIdentify(AddGroupIfIdentify_t *req, void *arg, uint8_t fsciInterface);
#endif  /* GROUPCLUSTERCOMMANDS_ENABLE */

#if IDENTIFYCLUSTERCOMMANDS_ENABLE
memStatus_t IdentifySend(IdentifySend_t *req, void *arg, uint8_t fsciInterface);
memStatus_t IdentifyQuery(IdentifyQuery_t *req, void *arg, uint8_t fsciInterface);
memStatus_t IdentifyTriggerEffects(IdentifyTriggerEffects_t *req, void *arg, uint8_t fsciInterface);
#endif  /* IDENTIFYCLUSTERCOMMANDS_ENABLE */

#if LEVELCLUSTERCOMMANDS_ENABLE
memStatus_t MoveToLevel(MoveToLevel_t *req, void *arg, uint8_t fsciInterface);
memStatus_t MoveToLevelWithWithoutOnOff(MoveToLevelWithWithoutOnOff_t *req, void *arg, uint8_t fsciInterface);
memStatus_t MoveStep(MoveStep_t *req, void *arg, uint8_t fsciInterface);
memStatus_t MoveStopWithOnOff(MoveStopWithOnOff_t *req, void *arg, uint8_t fsciInterface);
memStatus_t MoveToClosestFrequency(MoveToClosestFrequency_t *req, void *arg, uint8_t fsciInterface);
#endif  /* LEVELCLUSTERCOMMANDS_ENABLE */

#if ONOFFCLUSTERCOMMANDS_ENABLE
memStatus_t OnOffWithEffectsSend(OnOffWithEffectsSend_t *req, void *arg, uint8_t fsciInterface);
memStatus_t OnOffWithNoEffects(OnOffWithNoEffects_t *req, void *arg, uint8_t fsciInterface);
memStatus_t OnOffTimedSend(OnOffTimedSend_t *req, void *arg, uint8_t fsciInterface);
#endif  /* ONOFFCLUSTERCOMMANDS_ENABLE */

#if DIAGNOSTICSCLUSTERCOMMANDS_ENABLE
memStatus_t DiagnosticRequest(DiagnosticRequest_t *req, void *arg, uint8_t fsciInterface);
#endif  /* DIAGNOSTICSCLUSTERCOMMANDS_ENABLE */

#if SCENESCLUSTERCOMMANDS_ENABLE
memStatus_t ViewScene(ViewScene_t *req, void *arg, uint8_t fsciInterface);
memStatus_t AddScene(AddScene_t *req, void *arg, uint8_t fsciInterface);
memStatus_t RemoveScene(RemoveScene_t *req, void *arg, uint8_t fsciInterface);
memStatus_t RemoveAllScenes(RemoveAllScenes_t *req, void *arg, uint8_t fsciInterface);
memStatus_t StoreScene(StoreScene_t *req, void *arg, uint8_t fsciInterface);
memStatus_t RecallScene(RecallScene_t *req, void *arg, uint8_t fsciInterface);
memStatus_t SceneMembershipRequest(SceneMembershipRequest_t *req, void *arg, uint8_t fsciInterface);
memStatus_t AddEnhancedScene(AddEnhancedScene_t *req, void *arg, uint8_t fsciInterface);
memStatus_t ViewEnhancedScene(ViewEnhancedScene_t *req, void *arg, uint8_t fsciInterface);
memStatus_t CopyScene(CopyScene_t *req, void *arg, uint8_t fsciInterface);
#endif  /* SCENESCLUSTERCOMMANDS_ENABLE */

#if COLOURCLUSTERCOMMANDS_ENABLE
memStatus_t MoveToHue(MoveToHue_t *req, void *arg, uint8_t fsciInterface);
memStatus_t MoveHue(MoveHue_t *req, void *arg, uint8_t fsciInterface);
memStatus_t StepHue(StepHue_t *req, void *arg, uint8_t fsciInterface);
memStatus_t MoveToSaturation(MoveToSaturation_t *req, void *arg, uint8_t fsciInterface);
memStatus_t MoveSaturation(MoveSaturation_t *req, void *arg, uint8_t fsciInterface);
memStatus_t StepSaturation(StepSaturation_t *req, void *arg, uint8_t fsciInterface);
memStatus_t MoveToHueAndSaturation(MoveToHueAndSaturation_t *req, void *arg, uint8_t fsciInterface);
memStatus_t MoveToColour(MoveToColour_t *req, void *arg, uint8_t fsciInterface);
memStatus_t MoveColour(MoveColour_t *req, void *arg, uint8_t fsciInterface);
memStatus_t EnhancedMoveToHue(EnhancedMoveToHue_t *req, void *arg, uint8_t fsciInterface);
memStatus_t EnhancedMoveHue(EnhancedMoveHue_t *req, void *arg, uint8_t fsciInterface);
memStatus_t EnhancedStepHue(EnhancedStepHue_t *req, void *arg, uint8_t fsciInterface);
memStatus_t EnhancedMoveToHueAndSaturation(EnhancedMoveToHueAndSaturation_t *req, void *arg, uint8_t fsciInterface);
memStatus_t ColourLoopSet(ColourLoopSet_t *req, void *arg, uint8_t fsciInterface);
memStatus_t StopMoveStep(StopMoveStep_t *req, void *arg, uint8_t fsciInterface);
memStatus_t MoveToColourTemperature(MoveToColourTemperature_t *req, void *arg, uint8_t fsciInterface);
memStatus_t MoveColourTemperature(MoveColourTemperature_t *req, void *arg, uint8_t fsciInterface);
memStatus_t StepColourTemperature(StepColourTemperature_t *req, void *arg, uint8_t fsciInterface);
#endif  /* COLOURCLUSTERCOMMANDS_ENABLE */

#if TOUCHLINKCOMMANDS_ENABLE
memStatus_t InitiateTouchlink(void *arg, uint8_t fsciInterface);
memStatus_t TouchLinkFactoryResetTarget(void *arg, uint8_t fsciInterface);
#endif  /* TOUCHLINKCOMMANDS_ENABLE */

#if DOORLOCKCLUSTERCOMMANDS_ENABLE
memStatus_t LockUnlockDoor(LockUnlockDoor_t *req, void *arg, uint8_t fsciInterface);
#endif  /* DOORLOCKCLUSTERCOMMANDS_ENABLE */

#if IASCLUSTERCOMMANDS_ENABLE
memStatus_t IASZoneEnrollResponse(IASZoneEnrollResponse_t *req, void *arg, uint8_t fsciInterface);
#endif  /* IASCLUSTERCOMMANDS_ENABLE */

#if EXTENDEDUTILITIES_ENABLE
memStatus_t RawAPSDataRequest(RawAPSDataRequest_t *req, void *arg, uint8_t fsciInterface);
#endif  /* EXTENDEDUTILITIES_ENABLE */

void KHC_ZIGBEE_RX_MsgHandler(void *pData, void *param, uint8_t fsciInterface);

#endif  /* _ZIGBEE_CMD_H */
