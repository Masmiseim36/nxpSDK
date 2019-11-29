/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
#ifndef _MAC_ABS_TYPES_H
#define _MAC_ABS_TYPES_H
/*!=================================================================================================
\file       mac_abs_types.h
\brief      This contains the generic types for the IEEE 802.15.4 MAC Abstraction module(s).
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/

#include "EmbeddedTypes.h"

/*==================================================================================================
Public macros
==================================================================================================*/
#define MACABS_PANID_NULL                         0xFFFFU

/*==================================================================================================
Public type definitions
==================================================================================================*/

/* Types of MAC confirms and indications */
typedef enum
{
    macAbsMsgMcpsDataCnf_c,
    macAbsMsgMcpsDataInd_c,
    macAbsMsgMlmeBeaconNotifyInd_c,
    macAbsMsgMlmeCommStatusInd_c,
    macAbsMsgMlmeScanCnf_c,
    macAbsMsgMlmeSyncLossInd
} macAbsMsgType_t;

typedef enum
{
    gMacAbsSuccess_c                = 0x00,        /* The requested operation was completed successfully. For a transmission request, this value indicates a successful transmission.*/
    gMacAbsPanAtCapacity_c          = 0x01,        /* Association Status - PAN at capacity.*/
    gMacAbsPanAccessDenied_c        = 0x02,        /* Association Status - PAN access denied.*/
    /* DPS related */
    gMacAbsDpsNotSupported_d                  = 0xd1,

    /* Sounding related */
    gMacAbsSoundingNotSupported_d             = 0xd2,

    /* Calibration related */
    gMacAbsComputationNeeded_d                = 0xd3,

    /* Ranging related */
    gMacAbsRangingNotSupported_d              = 0xd4,
    gMacAbsRangingActive_d                    = 0xd5,
    gMacAbsRangingRequestedButNotsupported_d  = 0xd6,
    gMacAbsNoRangingRequested_d               = 0xd7,

    /* Security related */
    gMacAbsUnavailableDevice_c                      = 0xd8,  /* The DeviceDescriptor lookup lrocedure has failed for the received frame. No device was found which matches the input addressing mode, address and PAN Id */
    gMacAbsUnavailableSecurityLevel_c               = 0xd9,  /* The SecurityLevelDescriptor lookup procedure has failed for the received frame. No SecurityLevelDescriptor was found which matches the input frmae type and command frame identifier. */
    gMacAbsMacPollReqDecryptionError_c              = 0xda,  /* NOT in Spec! */
    gMacAbsCounterError_c           = 0xdb,        /* The frame counter purportedly applied by the originator of the received frame is invalid.*/
    gMacAbsImproperKeyType_c        = 0xdc,        /* The key purportedly applied by the originator of the received frame is not allowed to be used with that frame type according to the key usage policy of the recipient.*/
    gMacAbsImproperSecurityLevel_c  = 0xdd,        /* The security level purportedly applied by the originator of the received frame does not meet the minimum security level required/expected by the recipient for that frame type.*/
    gMacAbsUnsupportedLegacy_c      = 0xde,        /* The received frame was purportedly secured using security based on IEEE Std 802.15.4-2003, and such security is not supported by this standard.*/
    gMacAbsUnsupportedSecurity_c    = 0xdf,        /* The security purportedly applied by the originator of the received frame is not supported.*/
    gMacAbsBeaconLoss_c             = 0xe0,        /* The beacon was lost following a synchronization request.*/
    gMacAbsChannelAccessFailure_c   = 0xe1,        /* A transmission could not take place due to activity on the channel, i.e., the CSMA-CA mechanism has failed.*/
    gMacAbsDenied_c                 = 0xe2,        /* The GTS request has been denied by the PAN coordinator.*/
    gMacAbsDisableTrxFailure_c      = 0xe3,        /* The attempt to disable the transceiver has failed.*/
    gMacAbsSecurityError_c          = 0xe4,        /* Cryptographic processing of the received secured frame failed.*/
    gMacAbsFrameTooLong_c           = 0xe5,        /* Either a frame resulting from processing has a length that is greater than aMaxPHYPacketSize or a requested transaction is too large to fit in the CAP or GTS.*/
    gMacAbsInvalidGts_c             = 0xe6,        /* The requested GTS transmission failed because the specified GTS either did not have a transmit GTS direction or was not defined.*/
    gMacAbsInvalidHandle_c          = 0xe7,        /* A request to purge an MSDU from the transaction queue was made using an MSDU handle that was not found in the transaction table.*/
    gMacAbsInvalidParameter_c       = 0xe8,        /* A parameter in the primitive is either not supported or is out of the valid range.*/
    gMacAbsNoAck_c                  = 0xe9,        /* No acknowledgment was received after macMaxFrameRetries.*/
    gMacAbsNoBeacon_c               = 0xea,        /* A scan operation failed to find any network beacons.*/
    gMacAbsNoData_c                 = 0xeb,        /* No response data were available following a request.*/
    gMacAbsNoShortAddress_c         = 0xec,        /* The operation failed because a 16-bit short address was not allocated.*/
    gMacAbsOutOfCap_c               = 0xed,        /* A receiver enable request was unsuccessful because it could not be completed within the CAP. It is used only to meet the backwards compatibility requirements for IEEE Std 802.15.4-2003.*/
    gMacAbsPanIdConflict_c          = 0xee,        /* A PAN identifier conflict has been detected and communicated to the PAN coordinator.*/
    gMacAbsRealignment_c            = 0xef,        /* A coordinator realignment command has been received.*/
    gMacAbsTransactionExpired_c     = 0xf0,        /* The transaction has expired and its information was discarded.*/
    gMacAbsTransactionOverflow_c    = 0xf1,        /* There is no capacity to store the transaction.*/
    gMacAbsTxActive_c               = 0xf2,        /* The transceiver was in the transmitter enabled state when the receiver was requested to be enabled.It is used only to meet the backwards compatibility requirements for IEEE Std 802.15.4-2003.*/
    gMacAbsUnavailableKey_c         = 0xf3,        /* The key purportedly used by the originator of the received frame is not available or, if available, the originating device is not known or is blacklisted with that particular key.*/
    gMacAbsUnsupportedAttribute_c   = 0xf4,        /* A SET/GET request was issued with the identifier of a PIB attribute that is not supported.*/
    gMacAbsInvalidAddress_c         = 0xf5,        /* A request to send data was unsuccessful because neither the source address parameters nor the destination address parameters were present.*/
    gMacAbsOnTimeTooLong_c          = 0xf6,        /* A receiver enable request was unsuccessful because it specified a number of symbols that was longer than the beacon interval.*/
    gMacAbsPastTime_c               = 0xf7,        /* A receiver enable request was unsuccessful because it could not be completed within the current superframe and was not permitted to be deferred until the next superframe.*/
    gMacAbsTrackingOff_c            = 0xf8,        /* The device was instructed to start sending beacons based on the timing of the beacon transmissions of its coordinator, but the device is not currently tracking the beacon of its coordinator.*/
    gMacAbsInvalidIndex_c           = 0xf9,        /* An attempt to write to a MAC PIB attribute that is in a table failed because the specified table index was out of range.*/
    gMacAbsLimitReached_c           = 0xfa,        /* A scan operation terminated prematurely because the number of PAN descriptors stored reached an implementation-specified maximum.*/
    gMacAbsReadOnly_c               = 0xfb,        /* A SET/GET request was issued with the identifier of an attribute that is read only.*/
    gMacAbsScanInProgress_c         = 0xfc,        /* A request to perform a scan operation failed because the MLME was in the process of performing a previously initiated scan operation.*/
    gMacAbsSuperframeOverlap_c      = 0xfd,        /* The device was instructed to start sending beacons based on the timing of the beacon transmissions of its coordinator, but the instructed start time overlapped the transmission time of the beacon of its coordinator.*/
} macAbsResultType_t;

typedef uint32_t macAbsPibId_t;
typedef enum
{
    // Freescale specific MAC PIBs
    gMacAbsMPibRole_c                                 = 0x20,    /*!< The MAC role: Device/Coordinator */
    gMacAbsMPibLogicalChannel_c                       = 0x21,    /*!< Logical channel number */
    gMacAbsMPibTreemodeStartTime_c                    = 0x22,    /*!< Unused */
    gMacAbsMPibPanIdConflictDetection_c               = 0x23,    /*!< Unused */
    gMacAbsMPibBeaconResponseDenied_c                 = 0x24,    /*!< Ignore MAC Beacon Request commands */
    gMacAbsMPibNBSuperFrameInterval_c                 = 0x25,    /*!< Unused */
    gMacAbsMPibBeaconPayloadLengthVendor              = 0x26,    /*!< Unused */
    gMacAbsMPibBeaconResponseLQIThreshold_c           = 0x27,    /*!< Unused */
    gMacAbsMPibUseExtendedAddressForBeacon_c          = 0x28,    /*!< Force the use of the extended address for Beacons */
    gMacAbsMPibCsmaDisableMask_c                      = 0x29,
    gMacAbsMPibAlternateExtendedAddress_c             = 0x2A,    /*!< Alternate Extended Address used for nonce creation (MAC Security) */
    gMacAbsMPibMac2003Compatibility_c                 = 0x2F,    /*!< Maintain OTA compatibillity with 802.15.4/2003 */

    // Standard MAC PIBs
    gMacAbsMPibAckWaitDuration_c                      = 0x40,    /*!< Number of symbols to wait for an ACK (ReadOnly) */
    gMacAbsMPibAssociationPermit_c                    = 0x41,    /*!< Allow devices o associate (Coordinator only) */
    gMacAbsMPibAutoRequest_c                          = 0x42,    /*!< Send automatic MAC Data Requests, and notifications for every Beacon received */
    gMacAbsMPibBattLifeExt_c                          = 0x43,    /*!< Unused */
    gMacAbsMPibBattLifeExtPeriods_c                   = 0x44,    /*!< Unused */
    gMacAbsMPibBeaconPayload_c                        = 0x45,    /*!< Array, containind the Beacon payload */
    gMacAbsMPibBeaconPayloadLength_c                  = 0x46,    /*!< Number of bytes used for Beacon payload */
    gMacAbsMPibBeaconOrder_c                          = 0x47,    /*!< How often the beacon is to be transmitted. A value of 15 indicated a non-Beaconned PAN */
    gMacAbsMPibBeaconTxTime_c                         = 0x48,    /*!< The time of transmission of the most recent Beacon*/
    gMacAbsMPibBSN_c                                  = 0x49,    /*!< Beacon Sequence Number */
    gMacAbsMPibCoordExtendedAddress_c                 = 0x4a,    /*!< The extended address of the associated Coordinator */
    gMacAbsMPibCoordShortAddress_c                    = 0x4b,    /*!< The short address of the associated Coordinator */
    gMacAbsMPibDSN_c                                  = 0x4c,    /*!< Data Sequence Number */
    gMacAbsMPibGtsPermit_c                            = 0x4d,    /*!< Coordinator accepts GTS or not */
    gMacAbsMPibMaxCSMABackoffs_c                      = 0x4e,    /*!< The maximum number of backoffs the CSMA-CA algorithm will attempt before declaring a channel access failure */
    gMacAbsMPibMinBE_c                                = 0x4f,    /*!< The minimum value of the backoff exponent (BE) in the CSMA-CA algorithm */
    gMacAbsMPibPanId_c                                = 0x50,    /*!< The 16-bit identifier of the PAN on which the device is operating. If this value is 0xffff, the device is not associated. */
    gMacAbsMPibPromiscuousMode_c                      = 0x51,    /*!< Remove all MAC filtering. All packets are received (including ACKs) */
    gMacAbsMPibRxOnWhenIdle_c                         = 0x52,    /*!< Keep the RX on, when MAC is idle */
    gMacAbsMPibShortAddress_c                         = 0x53,    /*!< The 16-bit address that the device uses to communicate in the PAN. */
    gMacAbsMPibSuperframeOrder_c                      = 0x54,    /*!< The length of the active portion of the superframe, including the beacon frame */
    gMacAbsMPibTransactionPersistenceTime_c           = 0x55,    /*!< The storage time of a transaction (in units: aBaseSuperframeDuration for Non-Beaconned) */
    gMacAbsMPibAssociatedPANCoord_c                   = 0x56,    /*!< Indication of whether the device is associated to the PAN through the PAN coordinator. */
    gMacAbsMPibMaxBE_c                                = 0x57,    /*!< The maximum value of the backoff exponent, BE, in the CSMA-CA algorithm */
    gMacAbsMPibMaxFrameTotalWaitTime_c                = 0x58,    /*!< Number of symbols to wait for a frame intended as a response (see 802.15.4 spec for details) */
    gMacAbsMPibMaxFrameRetries_c                      = 0x59,    /*!< The maximum number of retries allowed after a transmission failure  */
    gMacAbsMPibResponseWaitTime_c                     = 0x5a,    /*!< The maximum time, in multiples of aBaseSuperframeDuration, a device shall wait for a response command frame to be available following a request command frame */
    gMacAbsMPibSyncSymbolOffset_c                     = 0x5b,    /*!< Offset between the frame timestamp and the first symbol after SFD */
    gMacAbsMPibTimestampSupported_c                   = 0x5c,    /*!< Indicates if frame timestamp is supported (ReadOnly) */
    gMacAbsMPibSecurityEnabled_c                      = 0x5d,    /*!< Indication of whether the MAC sublayer has security enabled */
    gMacAbsMPibMinLIFSPeriod_c                        = 0x5e,    /*!< Value not in standard - taken from bare metal MAC */
    gMacAbsMPibMinSIFSPeriod_c                        = 0x5f,    /*!< Value not in standard - taken from bare metal MAC */

    // MAC2006 Security Related PIB Attributes
    gMacAbsMPibKeyTable_c                             = 0x71,
    gMacAbsMPibKeyTableEntries_c                      = 0x72,    /*!< Freescale specific MAC 2011 */
    gMacAbsMPibDeviceTable_c                          = 0x73,
    gMacAbsMPibDeviceTableEntries_c                   = 0x74,    /*!< Freescale specific MAC 2011 */
    gMacAbsMPibSecurityLevelTable_c                    = 0x75,
    gMacAbsMPibSecurityLevelTableEntries_c            = 0x76,    /*!< Freescale specific MAC 2011 */
    gMacAbsMPibFrameCounter_c                         = 0x77,
    gMacAbsMPibAutoRequestSecurityLevel_c             = 0x78,    /*!< The security level used for automatic MAC requests (ex: Data Request) */
    gMacAbsMPibAutoRequestKeyIdMode_c                 = 0x79,    /*!< The key Id mode used for automatic MAC requests*/
    gMacAbsMPibAutoRequestKeySource_c                 = 0x7a,    /*!< The key source used for automatic MAC requests*/
    gMacAbsMPibAutoRequestKeyIndex_c                  = 0x7b,    /*!< The key index used for automatic MAC requests*/
    gMacAbsMPibDefaultKeySource_c                     = 0x7c,
    gMacAbsMPibPANCoordExtendedAddress_c              = 0x7d,
    gMacAbsMPibPANCoordShortAddress_c                 = 0x7e,
    gMacAbsMPibKeyIdLookupList_c                      = 0x7F,
    gMacAbsMPibKeyIdLookupListEntries_c               = 0x80,    /*!< Freescale specific MAC 2011 */
    gMacAbsMPibKeyDeviceList_c                        = 0x81,
    gMacAbsMPibKeyDeviceListEntries_c                 = 0x82,
    gMacAbsMPibKeyUsageList_c                         = 0x83,
    gMacAbsMPibKeyUsageListEntries_c                  = 0x84,    /*!< Freescale specific MAC 2011 */
    gMacAbsMPibKey_c                                  = 0x85,
    // KeyUsageDescriptor
    gMacAbsMPibKeyUsageFrameType_c                    = 0x86,
    gMacAbsMPibKeyUsageCommandFrameIdentifier_c       = 0x87,
    // KeyDeviceDescriptor
    gMacAbsMPibKeyDeviceDescriptorHandle_c            = 0x88,
    gMacAbsMPibUniqueDevice_c                         = 0x89,
    gMacAbsMPibBlackListed_c                          = 0x8A,
    // SecurityLevelDescriptor
    gMacAbsMPibSecLevFrameType_c                      = 0x8B,
    gMacAbsMPibSecLevCommnadFrameIdentifier_c         = 0x8C,
    gMacAbsMPibSecLevSecurityMinimum_c                = 0x8D,
    gMacAbsMPibSecLevDeviceOverrideSecurityMinimum_c  = 0x8E,

    gMacAbsMPibDeviceDescriptorPanId_c                = 0x8F,
    gMacAbsMPibDeviceDescriptorShortAddress_c         = 0x90,
    gMacAbsMPibDeviceDescriptorExtAddress_c           = 0x91,
    gMacAbsMPibDeviceDescriptorFrameCounter_c         = 0x92,
    gMacAbsMPibDeviceDescriptorExempt                 = 0x93,

    gMacAbsMPibKeyIdLookupData_c                      = 0x94,
    gMacAbsMPibKeyIdLookupDataSize_c                  = 0x95,
    gMacAbsMPibiKeyTableCrtEntry_c                    = 0x96,
    gMacAbsMPibiDeviceTableCrtEntry_c                 = 0x97,
    gMacAbsMPibiSecurityLevelTableCrtEntry_c          = 0x98,
    gMacAbsMPibiKeyIdLookuplistCrtEntry_c             = 0x99,
    gMacAbsMPibiKeyUsageListCrtEntry_c                = 0x9A,
    gMacAbsMPibiKeyDeviceListCrtEntry_c               = 0x9B,

    /* Specific for PLC-G3 */
    gMacAbsMPibHighPriorityWindowSize_c               = 0x01000113,
    gMacAbsMPibTxDataPacketCount_c                    = 0x02000101,
    gMacAbsMPibRxDataPacketCount_c                    = 0x02000102,
    gMacAbsMPibTxCmdPacketCount_c                     = 0x02000201,
    gMacAbsMPibRxCmdPacketCount_c                     = 0x02000202,
    gMacAbsMPibCSMAFailCount_c                        = 0x02000103,
    gMacAbsMPibCSMACollisionCount_c                   = 0x02000104,
    gMacAbsMPibBroadcastCount_c                       = 0x02000106,
    gMacAbsMPibMulticastCount_c                       = 0x02000107,
    gMacAbsMPibBadCRCCount_c                          = 0x02000108,
    gMacAbsMPibMaxOrphanTimer_c                       = 0x02000109,
    gMacAbsMPibNeighbourTable_c                       = 0x0000006B,
    gMacAbsMPibNumberOfHops_c                         = 0x0000006C,
    gMacAbsMPibFreqNotching_c                         = 0x0000006D,
    gMacAbsMPibCSMAFairnessLimit_c                    = 0x02000112,
    gMacAbsMPibMaxAgeTime_c                           = 0x02000113,
    gMacAbsMPibMaxNeighborValidTime_c                 = 0x02000114,
    gMacAbsMPibRCCoord_c                              = 0x02000115,

    /* Custom attributes */
    gMacAbsCustomMPibAGCEnable                        = 0x1C00001C,
    gMacAbsCustomMPibTxGain                           = 0x1C00001D,
    gMacAbsCustomMPibExtendedAddress                  = 0x1C00001E,

    /* Timing attributes */
    gMacAbsCustomMPibFixedBO                          = 0x1C000020,
    gMacAbsCustomMPibSymbolTime                       = 0x1C000021,
    gMacAbsCustomMPibSlotTime                         = 0x1C000022,
    gMacAbsCustomMPibRIFSTime                         = 0x1C000023,
    gMacAbsCustomMPibAckTime                          = 0x1C000024,
    gMacAbsCustomMPibCIFSTime                         = 0x1C000025,
    gMacAbsCustomMPibEIFSTime                         = 0x1C000026,
    gMacAbsCustomMPibFCHProcTime                      = 0x1C000027,
} macAbsPibId_e;

typedef enum
{
    gMacAbsAddrModeNoAddress_c            = 0x00,        /* No address (addressing fields omitted)*/
    gMacAbsAddrModeReserved_c             = 0x01,        /* Reserved value*/
    gMacAbsAddrModeShortAddress_c         = 0x02,        /* 16-bit short address*/
    gMacAbsAddrModeExtendedAddress_c      = 0x03         /* 64-bit extended address*/
} macAbsAddrModeType_t;

typedef enum
{                                                  /* Security attributes    Data confidentiality    Data authenticity*/
    gMacAbsMacSecurityDisabled_c          = 0x00,
    gMacAbsMacSecurityEncMic32_c          = 0x05        /*     ENC-MIC-32                 ON                YES (M = 4)*/
} macAbsSecurityLevel_t;

typedef enum
{
    gMacAbsKeyIdMode1_c                   = 0x01,        /* Key is determined from the 1-octet Key Index subfield
                                                         of the Key Identifier field of the auxiliary security header
                                                         in conjunction with macDefaultKeySource. */
    gMacAbsKeyIdMode2_c                   = 0x02        /*!< Key is determined explicitly from the 4-octet Key Source
                                                            subfield and the 1-octet Key Index subfield of the Key Identifier field of
                                                         the auxiliary security header.\n
                                                         KeyId length (bytes): 5 */
} macAbsKeyIdModeType_t;

typedef enum
{
    gMacAbsMacQoSNormalPrio_c             = 0x00,        /* Normal priority */
    gMacAbsMacQoSHighPrio_c               = 0x01,        /* High priority */
    gMacAbsMacQoSContentionFree_c         = 0x02         /* Contention free (optional) */
} macAbsQoS_t;

typedef enum
{
    gMacAbsMacTxOptionsNoAck_c            = 0x00,        /* No Ack is required.*/
    gMacAbsMacTxOptionsAck_c              = 0x01,        /* Ack is required.*/
    gMacAbsMacTxOptionIndirect_c          = 0x04,        /* Indirect transmission is required.*/
    gMacAbsMacTxOptionCsl_c               = 0x08,        /* CSL transmission is required.*/
    gMacAbsTxOptionAltExtAddr_c           = 0x80,        /*!< Use alternate Extended Address for nonce creation (MAC Security).*/
} macAbsTxOptions_t;

typedef enum
{
    gMacAbsRoleDevice_c   = 0,
    gMacAbsRoleCoord_c    = 1,
    gMacAbsRolePanCoord_c = 2
} macAbsDeviceRole_t;

typedef enum
{
    gMacAbsScanModeED_c                   = 0x00,        /* Energy Detection scan.*/
    gMacAbsScanModeActive_c               = 0x01,        /* Active scan.*/
} macAbsScanType_t;

typedef enum
{
    gMacAbsPollingED_c                    = 0x00,       /* Polling End Device */
    gMacAbsWakeOnRadioED_c                = 0x01,       /* Wake-on-radio End Device*/
    gMacAbsUnsyncedWakeOnRadioED_c        = 0x02,       /* Wake-on-radio ED that lost sync and 
                                                            needs indirect and CSL Tx */
    gMacAbsInvalidType_c                  = 0xFF,       
}macAbsRfdType_t;

typedef uint32_t absInstanceId_t;

typedef struct macAbsPanDescriptor_tag
{
    uint64_t              address;          /* The source address of the responding device */
    uint16_t              coordPANId;       /* The 16-bit PAN identifier. It must be logically ANDed with 0xFCFF. */
    macAbsAddrModeType_t  addrMode;         /* The source address mode of the responding device */
    uint8_t               channel;          /* Channel */
    uint8_t               linkQuality;      /* The 8-bit Link Quality */
} macAbsPanDescriptor_t;

typedef struct
{
    uint16_t            index;
    uint16_t            address;
    uint16_t            age;
    uint16_t            is_neighbor;
    uint16_t            tx_res;
    uint16_t            tx_gain;
    uint8_t             modulation;
    uint8_t             tone_map_8;
    uint8_t             tone_map_0_7;
    uint8_t             lqi;
    uint8_t             tx_coef_3_0;
    uint8_t             tx_coef_7_4;
    uint8_t             tx_coef_11_8;
    uint8_t             tx_coef_15_12;
    uint8_t             tx_coef_19_16;
    uint8_t             tx_coef_23_20;
    uint16_t            reserved;
} macNeighborTableEntry_T;

/*! \struct macAbsMcpsDataReq_tag
\brief
Structure defining the MCPS-DATA.request primitive
*/
typedef struct macAbsMcpsDataReq_tag
{
    macAbsAddrModeType_t     srcAddrMode;
    macAbsAddrModeType_t     dstAddrMode;
    uint16_t                 dstPANId;
    uint64_t                 dstAddr;
    uint8_t                  msduHandle;
    macAbsSecurityLevel_t    securityLevel;
    macAbsKeyIdModeType_t    keyIdMode;
    uint8_t                  keyIndex;
    uint64_t                 keySource;
    uint8_t *                pMsdu;
    uint16_t                 msduLength;
    macAbsTxOptions_t        txOptions;
    macAbsQoS_t              qualityOfService;
} macAbsMcpsDataReq_t;

/*! \struct macAbsMcpsDataCnf_tag
\brief
Structure defining the MCPS-DATA.confirm primitive
*/
typedef struct macAbsMcpsDataCnf_tag
{
    instanceId_t            instanceId;
    uint32_t                timestamp;
    uint8_t                 msduHandle;
    macAbsResultType_t      status;
    bool_t                  ehFramePending;
} macAbsMcpsDataCnf_t;

/*! Structure which defines the MCPS Data Indication parameters */
typedef struct macAbsMcpsDataInd_tag
{
    instanceId_t            instanceId;
    macAbsAddrModeType_t    srcAddrMode;
    macAbsAddrModeType_t    dstAddrMode;
    uint16_t                dstPANId;
    uint64_t                dstAddr;
    uint64_t                srcAddr;
    uint16_t                srcPANId;
    uint16_t                msduLength;
    uint32_t                timestamp;
    uint8_t                 mpduLinkQuality;
    uint8_t                 dsn;
    macAbsSecurityLevel_t   securityLevel;
    macAbsKeyIdModeType_t   keyIdMode;
    uint64_t                keySource;
    uint8_t                 keyIndex;
    macAbsQoS_t             qualityOfService;
    uint8_t *               pMsdu;
    bool_t                  ehFramePending;
} macAbsMcpsDataInd_t;

typedef struct macRcvdDiffKeyIndexInd_tag
{
    instanceId_t            macInstId;
    uint8_t                 keyIdMode;
    uint8_t                 keyIndex;
}macRcvdDiffKeyIndexInd_t;

/*! \struct macAbsMlmeGetReq_tag
\brief
Structure defining the MLME-GET.request primitive
*/
typedef struct macAbsMlmeGetReq_tag
{
    macAbsPibId_t            pibAttribute;
    uint32_t                 pibAttributeIndex;
} macAbsMlmeGetReq_t;

/*! \struct macAbsMlmeGetCnf_tag
\brief
Structure defining the MLME-GET.confirm primitive
*/
typedef struct macAbsMlmeGetCnf_tag
{
    macAbsPibId_t            pibAttribute;
    uint32_t                 pibAttributeIndex;
    void *                   pPibAttributeValue;
    macAbsResultType_t       status;
} macAbsMlmeGetCnf_t;

/*! \struct macAbsMlmeResetReq_tag
\brief
Structure defining the MLME-RESET.request primitive
*/
typedef struct macAbsMlmeResetReq_tag
{
    bool_t                   setDefaultPIB;
} macAbsMlmeResetReq_t;

/*! \struct macAbsMlmeResetCnf_t
\brief
Structure defining the MLME-RESET.confirm primitive
*/
typedef macAbsResultType_t   macAbsMlmeResetCnf_t;

/*! \struct macAbsMlmeScanReq_tag
\brief
Structure defining the MLME-SCAN.request primitive
*/
typedef struct macAbsMlmeScanReq_tag
{
    macAbsScanType_t scanType;
    uint32_t scanChannels;
    uint8_t  scanDuration;
} macAbsMlmeScanReq_t;

/*! \struct macAbsMlmeScanCnf_tag
\brief
Structure defining the MLME-SCAN.confirm primitive
*/
typedef struct macAbsMlmeScanCnf_tag
{
    instanceId_t       instanceId;
    macAbsResultType_t status;
    macAbsScanType_t   scanType;
    uint8_t            resultListSize;
    uint8_t*           pEnergyDetectList;
} macAbsMlmeScanCnf_t;

/*! \struct macAbsMlmeCommStatusInd_tag
\brief
Structure defining the MLME-COMM-STATUS.indication primitive
*/
typedef struct macAbsMlmeCommStatusInd_tag
{
    uint64_t                 srcAddr;
    uint64_t                 dstAddr;
    uint64_t                 keySource;
    void *                   ifHandle;
    uint16_t                 panId;
    macAbsAddrModeType_t     srcAddrMode;
    macAbsAddrModeType_t     dstAddrMode;
    macAbsResultType_t       status;
    macAbsSecurityLevel_t    securityLevel;
    macAbsKeyIdModeType_t    keyIdMode;
    uint8_t                  keyIndex;
    instanceId_t             macInstanceId;
} macAbsMlmeCommStatusInd_t;

/*! \struct macAbsMlmeSetReq_tag
\brief
Structure defining the MLME-SET.request primitive
*/
typedef struct macAbsMlmeSetReq_tag
{
    macAbsPibId_t            pibAttribute;
    uint32_t                 pibAttributeIndex;
    void*                    pPibAttributeValue;
} macAbsMlmeSetReq_t;

/*! \struct macAbsMlmeSetCnf_t
\brief
Structure defining the MLME-SET.confirm primitive
*/
typedef macAbsResultType_t   macAbsMlmeSetCnf_t;

/*! \struct macAbsMlmeStartReq_tag
\brief
Structure defining the MLME-START.request primitive
*/
typedef struct macAbsMlmeStartReq_tag
{
    uint16_t                 panId;
    uint8_t                  channel;
} macAbsMlmeStartReq_t;

/*! \struct macAbsMlmeStartCnf_t
\brief
Structure defining the MLME-START.confirm primitive
*/
typedef macAbsResultType_t   macAbsMlmeStartCnf_t;

/*! MAC Poll Request structure */
typedef struct macAbsMlmePollReq_tag
{
    macAbsAddrModeType_t    coordAddrMode;
    uint16_t                coordPanId;
    uint64_t                coordAddress;
    macAbsSecurityLevel_t   securityLevel;
    macAbsKeyIdModeType_t   keyIdMode;
    uint64_t                keySource;
    uint8_t                 keyIndex;
} macAbsMlmePollReq_t;

/*! MAC Poll Confirm structure */
typedef struct macAbsMlmePollCnf_tag
{
    instanceId_t            instanceId;
    macAbsResultType_t      status;
} macAbsMlmePollCnf_t;

/*! MAC Poll Notify Indication structure */
typedef struct macAbsMlmePollNotifyInd_tag
{
    uint64_t                srcAddr;
    instanceId_t            instanceId;
    uint16_t                srcPanId;
    macAbsAddrModeType_t    srcAddrMode;
} macAbsMlmePollNotifyInd_t;

/*! \struct macAbsMlmeBeaconNotifyInd_tag
\brief
Structure defining the MLME-BEACON-NOTIFY.indication primitive
*/

typedef struct macAbsMlmeBeaconNotifyInd_tag
{
    macAbsPanDescriptor_t   panDescriptor;
    instanceId_t            slwpInstId;
    uint32_t                beaconPloadSize;
    uint8_t                 aBeaconPload[];
} macAbsMlmeBeaconNotifyInd_t;

/*! \struct macAbsMlmeSyncLossInd_tag
\brief
Structure defining the MLME-SYNC-LOSS.indication primitive
*/
typedef struct macAbsMlmeSyncLossInd_tag
{
    uint16_t                panId;
    macAbsResultType_t      lossReason;
} macAbsMlmeSyncLossInd_t;

typedef struct macAbsCallbacks_tag
{
    /* MLME Callback functions */
    void (* mlmeBeaconNotifyInd)   (macAbsMlmeBeaconNotifyInd_t * pParam);
    void (* mlmeScanCnf)           (macAbsMlmeScanCnf_t * pParam);
    void (* mlmeCommStatusInd)     (macAbsMlmeCommStatusInd_t * pParam);
    void (* mlmeSyncLossInd)       (macAbsMlmeSyncLossInd_t * pParam);
    void (* mlmePollCnf)           (macAbsMlmePollCnf_t * pParam);
    void (* mcpsDataCnf)           (macAbsMcpsDataCnf_t* pParam);
    void (* mcpsKeyIdMode2DataInd) (macAbsMcpsDataInd_t* pParam);
} macAbsCallbacks_t;

/*! Structure which holds the MAC requests function pointers */
typedef struct macAbsRequests_tag
{
    /* MCPS Request Functions */
    macAbsResultType_t (* MCPS_DataReq)(macAbsMcpsDataReq_t * pParam, instanceId_t instanceId, uint32_t upperLayerInstanceId);
    macAbsResultType_t (* MCPS_PurgeReq)(uint8_t msduHandle, instanceId_t instanceId);

    /* MLME Request Functions */
    macAbsMlmeSetCnf_t      (* MLME_SetReq)     (macAbsMlmeSetReq_t * pParam, instanceId_t instanceId);
    macAbsMlmeGetCnf_t *    (* MLME_GetReq)     (macAbsMlmeGetReq_t * pParam, instanceId_t instanceId);
    macAbsMlmeResetCnf_t    (* MLME_ResetReq)   (macAbsMlmeResetReq_t * pParam, instanceId_t instanceId);
    macAbsMlmeStartCnf_t    (* MLME_StartReq)   (macAbsMlmeStartReq_t * pParam, instanceId_t instanceId);
    void                    (* MLME_PollReq)    (macAbsMlmePollReq_t * pParam, instanceId_t instanceId);
    macAbsMlmeScanCnf_t     (* MLME_ScanReq)    (macAbsMlmeScanReq_t * pParam, instanceId_t instanceId);

    /* Implementation Specific Functions */
    uint32_t    (* GetMaxMsduSize)  (macAbsMcpsDataReq_t *pParam, instanceId_t instanceId);
    bool_t      (* SetPANId)        (uint16_t panId, instanceId_t instanceId);
    uint16_t    (* GetPANId)        (instanceId_t instanceId);
    bool_t      (* SetShortAddress) (uint16_t shortAddress, instanceId_t instanceId);
    uint16_t    (* GetShortAddress) (instanceId_t instanceId);
    bool_t      (* SetExtendedAddress)(uint64_t extendedAddress, instanceId_t instanceId);
    uint64_t    (* GetExtendedAddress)(instanceId_t instanceId);
    bool_t      (* SetKey)          (uint8_t idxInKeyTable, uint8_t* key, uint8_t keyIndex, instanceId_t instanceId);
    bool_t      (* GetKey)          (uint8_t idxInKeyTable, uint8_t* key, instanceId_t instanceId);
    bool_t      (* SetChannel)      (uint8_t channel, instanceId_t instanceId);
    uint8_t     (* GetChannel)      (instanceId_t instanceId);
    bool_t      (* SetRxOnWhenIdle) (bool_t bRxOnWhenIdle, instanceId_t instanceId);
    bool_t      (* GetRxOnWhenIdle) (instanceId_t instanceId);
    void        (* SetPanSecurity)  (uint8_t maxNbOfKeys, uint8_t maxNbOfNeighbors, uint8_t index, instanceId_t instanceId, bool_t autoReqSecLevel);
    void        (* SetNeighborSecurity)(uint8_t neighborIdx, uint8_t maxNeighbors, uint8_t macKeyIdx, uint8_t maxMacKeys,
                                        uint64_t extAddr, uint16_t shortAddr, uint16_t panId, uint32_t frameCounter, instanceId_t instanceId);
    void        (* SetMlmeCallbacks)(macAbsCallbacks_t* mlmeCallbacks, instanceId_t instanceId);
    bool_t      (* SetFrameCounter) (uint32_t channel, instanceId_t instanceId);
    uint32_t    (* GetFrameCounter)(instanceId_t instanceId);
    bool_t      (* GetNeighborFrameCounter)(uint32_t *pFrameCtr, uint8_t neighborIdx, uint8_t maxNeighbors,
                                            uint8_t macKeyIdx, uint8_t maxMacKeys, instanceId_t instanceId);
    bool_t      (* SetICanHearYouTblEntry) (uint8_t* pSrcAddr, uint8_t addrMode);
    bool_t      (* SetBeaconSourceExtendedAddress) (bool_t enable, instanceId_t instanceId);
    bool_t      (* PhyAddToNeighborTable)(uint8_t *pAddr, uint8_t addrMode, uint16_t PanId);
    bool_t      (* PhyRemoveFromNeighborTable)(uint8_t *pAddr, uint8_t addrMode, uint16_t PanId);
    int8_t      (* PhyConvertLQIToRSSI)(uint8_t LQI);
    void        (* AllowBroadcast)   (instanceId_t instanceId, bool_t allowBroadcast);
} macAbsRequests_t;

typedef struct macAbsStats_tag
{
    uint32_t mNbDataReq;
    uint32_t mNbDataReqSuccess;
    uint32_t mNbDataReqFail;
    uint32_t mNbDataCnfSuccess;
    uint32_t mNbDataCnfFail;
} macAbsStats_t;

typedef struct code_time_tag
{
    uint64_t dataIndTime;
    uint64_t dataReqTime;
    uint64_t deltaTime;
} code_time_t;

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/*==================================================================================================
Public function prototypes
==================================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /* _MAC_ABS_TYPES_H */
