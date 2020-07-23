/*! *********************************************************************************
* \defgroup HCI HCI
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2020 NXP
* All rights reserved.
*
* \file
*
* This file is the interface file for the HCI module
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef HCI_INTERFACE_H
#define HCI_INTERFACE_H

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "ble_general.h"
#include "hci_types.h"

/************************************************************************************
*************************************************************************************
* Public constants & macros
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/*******************************************************************************
*   COMMANDS and EVENTS parameters
***************************************************************************** */
/*! HCI Commands and Events for ************************************************
*   OGF : 0x01 : Link Control Commands
***************************************************************************** */

/*! OCF 0x0006 */
/*! HCI_Disconnect */
typedef struct
{
    uint16_t            connectionHandle;
    hciErrorCode_t      reason;
} hciDisconnectCommand_t;
/*! Disconnection Complete Event : 0x05 */
typedef struct
{
    hciErrorCode_t      status;
    uint16_t            connectionHandle;
    hciErrorCode_t      reason;
} hciDisconnectionCompleteEvent_t;

/*! OCF 0x001D */
/*! HCI_Read_Remote_Version_Information */
typedef struct
{
    uint16_t            connectionHandle;
} hciReadRemoteVersionInformationCommand_t;
/*! Read Remote Version Information Complete Event : 0x0C */
typedef struct
{
    hciErrorCode_t      status;
    uint16_t            connectionHandle;
    uint8_t             version;
    uint16_t            manufacturerName;
    uint16_t            subversion;
} hciReadRemoteVersionInformationCompleteEvent_t;


/*! HCI Commands and Events for ************************************************
*   OGF : 0x03 : Controller & Baseband Commands
***************************************************************************** */
/*! OCF 0x0001 */
/*! HCI_Set_Event_Mask - Command Complete Parameters */
typedef struct
{
    hciErrorCode_t      status;
} hciSetEventMaskCommandComplete_t;

/*! Command Status Event : 0x0F */
typedef struct
{
    hciErrorCode_t          status;
    uint8_t                 numHciCommandPackets;
    hciCommandOpCode_t      commandOpCode;
} hciCommandStatusEvent_t;

/*! Hardware Error Event : 0x10 */
typedef struct
{
    bleHardwareErrorCode_t      status;
} hciHardwareErrorEvent_t;

typedef struct
{
    uint8_t                 numberOfHandles;
    uint16_t                connectionHandle[gcGapMaximumActiveConnections_c];
    uint16_t                hcNumCompletedPackets[gcGapMaximumActiveConnections_c];
} hciNumberOfCompletedPacketsEvent_t;

/*! Data Buffer Overflow Event : 0x1A */
typedef struct
{
    hciDataBufferOverflowLinkType_t         linkType;
} hciDataBufferOverflowEvent_t;

/*! Authenticated Payload Timeout Expired Event : 0x57 */
typedef struct
{
    uint16_t                connectionHandle;
} hciAuthenticatedPayloadTimeoutExpiredEvent_t;

/*! OCF 0x0001 */
/*! HCI_Set_Event_Mask */
typedef struct
{
    uint8_t             eventMask[8]; /* hciSetEventMaskEventShift_t */
} hciSetEventMaskCommand_t;

/*! OCF 0x0003 */
/*! HCI_Reset */
/*! No parameters */
/*! HCI_Reset - Command Complete Parameters */
typedef struct
{
    hciErrorCode_t      status;
} hciResetCommandComplete_t;

/*! OCF 0x002D */
/*! HCI_Read_Transmit_Power_Level */
typedef struct
{
    uint16_t                        connectionHandle;
    bleTransmitPowerLevelType_t     type;
} hciReadTransmitPowerLevelCommand_t;

/*! HCI_Read_Transmit_Power_Level - Command Complete Parameters */
typedef struct
{
    hciErrorCode_t                  status;
    uint16_t                        connectionHandle;
    int8_t                          transmitPowerlevel;  /* This is a signed 8 bit integer - Range: -30..20 dBm*/
} hciReadTransmitPowerLevelCommandComplete_t;

/*! OCF 0x0031 */
/*! HCI_Set_Controller_To_Host_Flow_Control */
typedef struct
{
    hciControllerToHostFlowControlEnable_t      eventMask;
} hciSetControllerToHostFlowControlCommand_t;

/*! HCI_Set_Controller_To_Host_Flow_Control - Command Complete Parameters */
typedef struct
{
    hciErrorCode_t                              status;
} hciSetControllerToHostFlowControlCommandComplete_t;

/*! OCF 0x0033 */
/*! HCI_Host_Buffer_Size */
typedef struct
{
    uint16_t            hostAclDataPacketlength;
    uint8_t             hostSynchronousDataPacketLength;
    uint16_t            hostTotalNumAclDataPackets;
    uint16_t            hostTotalNumSynchronousDataPackets;
} hciHostBufferSizeCommand_t;

/*! HCI_Host_Buffer_Size - Command Complete Parameters */
typedef struct
{
    hciErrorCode_t      status;
} hciHostBufferSizeCommandComplete_t;

/*! OCF 0x0035 */
/*! HCI_Host_Number_Of_Completed_Packets */
typedef struct
{
    uint16_t            connectionHandle;
    uint16_t            hostNrOfCompletedPackets;
} bleHostCompletedPacketsParams_t;

typedef struct
{
    uint8_t             numberOfHandles;
    uint16_t            connectionHandle[gcGapMaximumActiveConnections_c];
    uint16_t            hostNrOfCompletedPackets[gcGapMaximumActiveConnections_c];
} hciHostNumberOfCompletedPacketsCommand_t;

/*! HCI_Host_Number_Of_Completed_Packets - Command Complete Parameters */
typedef struct
{
    hciErrorCode_t                      status;
} hciHostNumberOfCompletedPacketsCommandComplete_t;

#if (gBLE41_d == TRUE)
/*! OCF 0x0063 */
/*! HCI_Set_Event_Mask_Page_2 */
typedef struct
{
    uint8_t             eventMaskPage2[8]; /* hciSetEventMaskPage2EventShift_t */
} hciSetEventMaskPage2Command_t;

/*! HCI_Set_Event_Mask_Page_2 - Command Complete Parameters */
typedef struct
{
    hciErrorCode_t      status;
} hciSetEventMaskPage2CommandComplete_t;

/*! OCF 0x007B */
/*! HCI_Read_Authenticated_Payload_Timeout */
typedef struct
{
    uint16_t            connectionHandle;
} hciReadAuthenticatedPayloadTimeoutCommand_t;

/*! HCI_Read_Authenticated_Payload_Timeout - Command Complete Parameters */
typedef struct
{
    hciErrorCode_t      status;
    uint16_t            connectionHandle;
    uint16_t            authenticatedPayloadTimeout;
} hciReadAuthenticatedPayloadTimeoutCommandComplete_t;

/*! OCF 0x007C */
/*! HCI_Write_Authenticated_Payload_Timeout */
typedef struct
{
    uint16_t            connectionHandle;
    uint16_t            authenticatedPayloadTimeout;
} hciWriteAuthenticatedPayloadTimeoutCommand_t;

/*! HCI_Write_Authenticated_Payload_Timeout - Command Complete Parameters */
typedef struct
{
    hciErrorCode_t      status;
    uint16_t            connectionHandle;
} hciWriteAuthenticatedPayloadTimeoutCommandComplete_t;

#endif /* gBLE41_d */

/*! HCI Commands and Events for ************************************************
*   OGF : 0x04 : Informational Parameters
***************************************************************************** */

/*! OCF 0x0001 */
/*! HCI_LE_Read_Local_Version_Information */
/*! No parameters */
/*! HCI_LE_Read_Local_Version_Information - Command Complete Parameters */
typedef struct
{
    hciErrorCode_t      status;
    uint8_t             hciVersion;
    uint16_t            hciRevision;
    uint8_t             lmpPalVersion;
    uint16_t            manufacturerName;
    uint16_t            lmpPalSubversion;
} hciReadLocalVersionInfoCommandComplete_t;

/*! OCF 0x0002 */
/*! HCI_Read_Local_Supported_Commands */
/*! No parameters */
/*! HCI_Read_Local_Supported_Commands - Command Complete Parameters */
typedef struct
{
    hciErrorCode_t      status;
    uint8_t             supportedCommands[64]; /* hciSupportedCommandsShift_t */
} hciReadLocalSupportedCommandsCommandComplete_t;

/*! OCF 0x0003 */
/*! HCI_Read_Local_Supported_Features */
/*! No parameters */
/*! HCI_Read_Local_Supported_Features - Command Complete Parameters */
typedef struct
{
    hciErrorCode_t      status;
    uint8_t             lmpFeatures[8];
} hciReadLocalSupportedFeaturesCommandComplete_t;

/*! OCF 0x0009 */
/*! HCI_Read_BD_ADDR */
/*! No parameters */
/*! HCI_Read_BD_ADDR - Command Complete Parameters */
typedef struct
{
    hciErrorCode_t      status;
    uint8_t             bdAddr[gcBleDeviceAddressSize_c];
} hciReadBdAddrCommandComplete_t;


/*! HCI Commands and Events for ************************************************
*   OGF : 0x05 : Status Parameters
***************************************************************************** */

/*! OCF 0x0005 */
/*! HCI_Read_RSSI */
typedef struct
{
    uint16_t            handle;
} hciReadRssiCommand_t;
/*! HCI_Read_RSSI - Command Complete Parameters */
typedef struct
{
    hciErrorCode_t      status;
    uint16_t            handle;
    int8_t              rssi; /* This is a signed 8 bit integer - Range: -127..20 dBm for a LE Controller*/
} hciReadRssiCommandComplete_t;


/*! HCI Commands and Events for ************************************************
*   OGF : 0x08 : LE Controller Commands
***************************************************************************** */

/*! OCF 0x0001 */
/*! HCI_LE_Set_Event_Mask */
typedef struct
{
    uint8_t             leEventMask[8]; /* hciLeSetEventMaskEventShift_t */
} hciLeSetEventMaskCommand_t;

/*! OCF 0x0002 */
/*! HCI_LE_Read_Buffer_Size */
/*! No parameters */

/*! OCF 0x0003 */
/*! HCI_LE_Read_Local_Supported_Features */
/*! No parameters */

/*! OCF 0x0005 */
/*! HCI_LE_Set_Random_Address */
typedef struct
{
    uint8_t             randomAddress[gcBleDeviceAddressSize_c];
} hciLeSetRandomAddressCommand_t;

/*! OCF 0x0006 */
/*! HCI_LE_Set_Advertising_Parameters */
typedef struct
{
    uint16_t                        advertisingIntervalMin;
    uint16_t                        advertisingIntervalMax;
    bleAdvertisingType_t            advertisingType;
    hciBleAddressType_t             ownAddressType;
    hciBleAddressType_t             directAddressType;
    uint8_t                         directAddress[gcBleDeviceAddressSize_c];
    bleAdvertisingChannelMap_t      advertisingChannelMap;
    bleAdvertisingFilterPolicy_t    advertisingFilterPolicy;
} hciLeSetAdvertisingParametersCommand_t;

/*! OCF 0x0007 */
/*! HCI_LE_Read_Advertising_Channel_Tx_Power */
/*! No parameters */

/*! OCF 0x0008 */
/*! HCI_LE_Set_Advertising_Data */
typedef struct
{
    uint8_t             advertisingDataLength;
    const uint8_t      *pAdvertisingData;
} hciLeSetAdvertisingDataCommand_t;

/*! OCF 0x0009 */
/*! HCI_LE_Set_Scan_Response_Data */
typedef struct
{
    uint8_t             scanResponseDataLength;
    const uint8_t      *pScanResponseData;
} hciLeSetScanResponseDataCommand_t;

/*! OCF 0x000B */
/*! HCI_LE_Set_Scan_Parameters */
typedef struct
{
    bleScanType_t                 scanType;
    uint16_t                      scanInterval;
    uint16_t                      scanWindow;
    hciBleAddressType_t           ownAddressType;
    hciBleScanningFilterPolicy_t  scanningFilterPolicy;
} hciLeSetScanParametersCommand_t;

/*! OCF 0x000C */
/*! HCI_LE_Set_Scan_Enable */
typedef struct
{
    hciLeScanEnable_t           scanEnable;
    hciLeFilterDuplicates_t     filterDuplicates;
} hciLeSetScanEnableCommand_t;

/*! LE Advertising Report Event : LE Meta Event : 0x3E - Subevent Code : 0x02 */
typedef struct
{
    bleAdvertisingReportEventType_t         eventType;
    bleAddressType_t                        addressType;
    uint8_t                                 address[gcBleDeviceAddressSize_c];
    uint8_t                                 dataLength;
    uint8_t                                 data[gHciLeAdvertisingDataLength_c];
    uint8_t                                 rssi;
} bleAdvertisingReportEventParams_t;

/*! LE Direct Advertising Report Event : LE Meta Event : 0x3E - Subevent Code : 0x0B */
typedef struct
{
    bleAdvertisingReportEventType_t         eventType;
    bleAddressType_t                        addressType;
    uint8_t                                 address[gcBleDeviceAddressSize_c];
    bleAddressType_t                        directAddressType;
    uint8_t                                 directAddress[gcBleDeviceAddressSize_c];
    uint8_t                                 rssi;
} bleDirectedAdvertisingReportEventParams_t;

typedef struct
{
    uint8_t                                 numReports;
    bleAdvertisingReportEventParams_t       *aAdvReports;
} hciLeAdvertisingReportEvent_t;

/*! OCF 0x000D */
/*! HCI_LE_Create_Connection */
typedef struct
{
    uint16_t                    scanInterval;
    uint16_t                    scanWindow;
    bleInitiatorFilterPolicy_t  initiatorFilterPolicy;
    hciBleAddressType_t         peerAddressType;
    uint8_t                     peerAddress[gcBleDeviceAddressSize_c];
    hciBleAddressType_t         ownAddressType;
    uint16_t                    connIntervalMin;
    uint16_t                    connIntervalMax;
    uint16_t                    connLatency;
    uint16_t                    supervisionTimeout;
    uint16_t                    minimumCELength;
    uint16_t                    maximumCELength;
} hciLeCreateConnectionCommand_t;

/*! LE Connection Complete Event : LE Meta Event : 0x3E : Subevent Code : 0x01 */
typedef struct
{
    hciErrorCode_t              status;
    uint16_t                    connectionHandle;
    bleLlConnectionRole_t       role;
    bleAddressType_t            peerAddressType;
    uint8_t                     peerAddress[gcBleDeviceAddressSize_c];
    uint16_t                    connInterval;
    uint16_t                    connLatency;
    uint16_t                    supervisionTimeout;
    bleMasterClockAccuracy_t    masterClockAccuracy;
} hciLeConnectionCompleteEvent_t;

/*! OCF 0x000E */
/*! HCI_LE_Create_Connection_Cancel */
/*! No parameters */

/*! OCF 0x000F */
/*! HCI_LE_Read_White_List_Size */
/*! No parameters */

/*! OCF 0x0010 */
/*! HCI_LE_Clear_White_List */
/*! No parameters */

/*! OCF 0x0011 */
/*! HCI_LE_Add_Device_To_White_List */
typedef struct
{
    bleAddressType_t    addressType;
    uint8_t             address[gcBleDeviceAddressSize_c];
} hciLeAddDeviceToWhiteListCommand_t;

/*! OCF 0x0012 */
/*! HCI_LE_Remove_Device_From_White_List */
typedef struct
{
    bleAddressType_t    addressType;
    uint8_t             address[gcBleDeviceAddressSize_c];
} hciLeRemoveDeviceFromWhiteListCommand_t;

/*! OCF 0x0013 */
/*! HCI_LE_Connection_Update */
typedef struct
{
    uint16_t            connectionHandle;
    uint16_t            connIntervalMin;
    uint16_t            connIntervalMax;
    uint16_t            connLatency;
    uint16_t            supervisionTimeout;
    uint16_t            minimumCeLength;
    uint16_t            maximumCeLength;
} hciLeConnectionUpdateCommand_t;

/*! LE Connection Update Complete Event : LE Meta Event : 0x3E : Subevent Code : 0x03 */
/* Although not used, tag is needed in order to satisfy MISRA rule 10.3 */
typedef struct hciLeConnectionUpdateCompleteEvent_tag
{
    hciErrorCode_t      status;
    uint16_t            connectionHandle;
    uint16_t            connInterval;
    uint16_t            connLatency;
    uint16_t            supervisionTimeout;
} hciLeConnectionUpdateCompleteEvent_t;

/*! OCF 0x0014 */
/*! HCI_LE_Set_Host_Channel_Classification */
typedef struct
{
    uint8_t             channelMap[gcBleChannelMapSize_c];
} hciLeSetHostChannelClassificationCommand_t;

/*! OCF 0x0015 */
/*! HCI_LE_Read_Channel_Map */
typedef struct
{
    uint16_t            connectionHandle;
} hciLeReadChannelMapCommand_t;

/*! OCF 0x0016 */
/*! HCI_LE_Read_Remote_Features */
typedef struct
{
    uint16_t            connectionHandle;
} hciLeReadRemoteFeaturesCommand_t;

/*! LE Read Remote Features Complete Event : LE Meta Event : 0x3E : Subevent Code : 0x04 */
typedef struct
{
    hciErrorCode_t      status;
    uint16_t            connectionHandle;
    uint8_t             leFeatures[8]; /* hciLeSupportedFeatures_tag */
} hciLeReadRemoteFeaturesCompleteEvent_t;

/*! LE Long Term Key Request Event : LE Meta Event : 0x3E - Subevent Code : 0x05 */
typedef struct
{
    uint16_t                connectionHandle;
    uint8_t                 randomNumber[8];
    uint16_t                encryptionDiversifier;
} hciLeLongTermKeyRequestEvent_t;

/*! LE Long Term Key Request Event : LE Meta Event : 0x3E - Subevent Code : 0x06 */
typedef struct
{
    uint16_t                connectionHandle;
    uint16_t                intervalMin;
    uint16_t                intervalMax;
    uint16_t                latency;
    uint16_t                timeout;
} hciLeRemoteConnectionParameterRequestEvent_t;

#if (gBLE42_d == TRUE)
/*! LE Data Length Change Event : LE Meta Event : 0x3E - Subevent Code : 0x07 */
typedef struct
{
    uint16_t                connectionHandle;
    uint16_t                maxTxOctets;
    uint16_t                maxTxTime;
    uint16_t                maxRxOctets;
    uint16_t                maxRxTime;
}hciLeDataLengthChangeEvent_t;

/*! LE Read Local P-256 Public Key Complete Event : LE Meta Event : 0x3E - Subevent Code : 0x08 */
typedef struct
{
    hciErrorCode_t          status;
    uint8_t                 localP256PublicKey[64];
}hciLeReadLocalP256PublicKeyCompleteEvent_t;

/*! LE Generate DHKey Complete Event : LE Meta Event : 0x3E - Subevent Code : 0x09 */
typedef struct
{
    hciErrorCode_t          status;
    uint8_t                 dhKey[32];
}hciLeGenerateDhKeyCompleteEvent_t;

/*! LE Enhanced Connection Complete Event : LE Meta Event : 0x3E - Subevent Code : 0x0A */
typedef struct
{
    hciErrorCode_t              status;
    uint16_t                    connectionHandle;
    bleLlConnectionRole_t       role;
    bleAddressType_t            peerAddressType;
    uint8_t                     peerAddress[gcBleDeviceAddressSize_c];
    uint8_t                     localResolvablePrivateAddress[gcBleDeviceAddressSize_c];
    uint8_t                     peerResolvablePrivateAddress[gcBleDeviceAddressSize_c];
    uint16_t                    connInterval;
    uint16_t                    connLatency;
    uint16_t                    supervisionTimeout;
    bleMasterClockAccuracy_t    masterClockAccuracy;
}hciLeEnhancedConnectionCompleteEvent_t;

typedef struct
{
    uint8_t                                   numReports;
    bleDirectedAdvertisingReportEventParams_t *aAdvReports;
} hciLeDirectedAdvertisingReportEvent_t;
#endif /* gBLE42_d */

#if (gBLE50_d == TRUE) || (gBLE42_d == TRUE)
/*! LE PHY Update Complete Event : LE Meta Event : 0x3E - Subevent Code : 0x0C */
typedef struct
{
    hciErrorCode_t              status;
    uint16_t                    connectionHandle;
    uint8_t                     txPhy;
    uint8_t                     rxPhy;
} hciLePhyUpdateCompleteEvent_t;
#endif

/*! Vendor Specific Enhanced Notification Event : LE Meta Event : 0x3E - Subevent Code : 0xFF */
typedef struct hciVendorEnhancedNotificationEvent_tag
{
    uint16_t                    eventType;
    uint16_t                    connectionHandle;
    int8_t                      rssi;
    uint8_t                     channel;
    uint16_t                    ev_counter;
    uint16_t                    timestamp;
    uint8_t                     adv_handle;
} hciVendorEnhancedNotificationEvent_t;

#if (gBLE50_d == TRUE)
/*! Extended/Periodic Advertising data block */
typedef struct hciLeExtAdvReportEventData_tag
{
    struct hciLeExtAdvReportEventData_tag *pNext;
    uint8_t                               dataLength;
    uint8_t                               aData[1];
}hciLeExtAdvReportEventData_t;

/*! Extended Advertising Report parameters */
typedef struct hciLeExtAdvReportEventParams_tag
{
    hciErrorCode_t                          status; /* Host internal use only. */
    hciBleExtAdvReportEventType_t           eventType;
    hciBleAddressType_t                     addrType;
    uint8_t                                 addr[gcBleDeviceAddressSize_c];
    uint8_t                                 primaryPHY;
    uint8_t                                 secondaryPHY;
    uint8_t                                 SID;
    uint8_t                                 txPower;
    uint8_t                                 rssi;
    uint16_t                                advInterval;
    hciBleAddressType_t                     directAddrType;
    uint8_t                                 directAddr[gcBleDeviceAddressSize_c];
    hciLeExtAdvReportEventData_t            *pData;
} hciLeExtAdvReportEventParams_t;

/*! LE Extended Advertising Report Event : LE Meta Event : 0x3E - Subevent Code : 0x0D */
typedef struct hciLeExtAdvReportEvent_tag
{
    uint8_t                                 numReports;
    hciLeExtAdvReportEventParams_t          *aAdvReports;
} hciLeExtAdvReportEvent_t;

/*! LE Periodic Advertising Sync Established Event : LE Meta Event : 0x3E - Subevent Code : 0x0E */
typedef struct hciLePeriodicAdvSyncEstbEvent_tag
{
    hciErrorCode_t                          status;
    uint16_t                                syncHandle;
    uint8_t                                 advertisingSID;
    bleAddressType_t                        advertiserAddrType;
    uint8_t                                 advertiserAddr[gcBleDeviceAddressSize_c];
    uint8_t                                 advertiserPHY;
    uint16_t                                periodicAdvertisingInterval;
    uint8_t                                 advertiserClockAccuracy;
} hciLePeriodicAdvSyncEstbEvent_t;

/*! LE Periodic Advertising Report Event : LE Meta Event : 0x3E - Subevent Code : 0x0F */
typedef struct hciLePeriodicAdvReportEvent_tag
{
    hciErrorCode_t                          status; /* Host internal use only. */
    uint16_t                                syncHandle;
    uint8_t                                 txPower;
    uint8_t                                 rssi;
    hciBleExtAdvReportStatus_t              dataStatus;
    hciLeExtAdvReportEventData_t            *pData;
} hciLePeriodicAdvReportEvent_t;

/*! LE Periodic Advertising Sync Lost Event : LE Meta Event : 0x3E - Subevent Code : 0x10 */
typedef struct hciLePeriodicAdvSyncLostEvent_tag
{
    uint16_t                                syncHandle;
} hciLePeriodicAdvSyncLostEvent_t;

/*! LE Scan Timeout Event : LE Meta Event : 0x3E - Subevent Code : 0x11 */
/* Event has no parameters. */

/*! LE Advertising Set Terminated Event : LE Meta Event : 0x3E - Subevent Code : 0x12 */
typedef struct hciLeAdvSetTerminatedEvent_tag
{
    hciErrorCode_t                          status;
    uint8_t                                 advHandle;
    uint16_t                                connHandle;
    uint8_t                                 numCompletedExtAdvEvents;
} hciLeAdvSetTerminatedEvent_t;

/*! LE Scan Request Received Event : LE Meta Event : 0x3E - Subevent Code : 0x13 */
typedef struct hciLeScanReqReceivedEvent_tag
{
    uint8_t                                 advHandle;
    bleAddressType_t                        scannerAddrType;
    uint8_t                                 scannerAddr[gcBleDeviceAddressSize_c];
} hciLeScanReqReceivedEvent_t;

/*! LE Channel Selection Algorithm Event : LE Meta Event : 0x3E - Subevent Code : 0x14 */
typedef struct hciLeChannelSelAlgorithmEvent_tag
{
    uint16_t                                connHandle;
    uint8_t                                 channelSelectionAlgorithm;
} hciLeChannelSelAlgorithmEvent_t;
#endif

#if defined(gHciLeEncryptSupport_d) && (gHciLeEncryptSupport_d == TRUE)
/*! OCF 0x0017 */
/*! HCI_LE_Encrypt */
typedef struct
{
    uint8_t             key[16];
    uint8_t             plaintextData[16];
} hciLeEncryptCommand_t;
#endif

/*! OCF 0x0018 */
/*! HCI_LE_Rand */
/*! No parameters */

/*! OCF 0x0019 */
/*! HCI_LE_Start_Encryption */
typedef struct
{
    uint16_t            connectionHandle;
    uint8_t             randomNumber[8];
    uint16_t            encryptedDiversifier;
    uint8_t             longTermKey[16];
} hciLeStartEncryptionCommand_t;

/*! Encryption Change Event : 0x08 */
typedef struct
{
    hciErrorCode_t              status;
    uint16_t                    connectionHandle;
    hciLlEncryptionEnabled_t    encryptionEnabled;
} hciEncryptionChangeEvent_t;

/*! Encryption Key refresh Complete Event : 0x30 */
typedef struct
{
    hciErrorCode_t      status;
    uint16_t            connectionHandle;
} hciEncryptionKeyRefreshCompleteEvent_t;

/*! OCF 0x001A */
/*! HCI_LE_Long_Term_Key_Request_Reply */
typedef struct
{
    uint16_t            connectionHandle;
    uint8_t             longTermKey[16];
} hciLeLongTermKeyRequestReplyCommand_t;

/*! OCF 0x001B */
/*! HCI_LE_Long_Term_Key_Request_Negative_Reply */
typedef struct
{
    uint16_t            connectionHandle;
} hciLeLongTermKeyRequestNegativeReplyCommand_t;

/*! OCF 0x001D */
/*! HCI_LE_Receiver_Test */
typedef struct
{
    bleChannelFrequency_t   rxChannel;
} hciLeReceiverTestCommand_t;

/*! OCF 0x001E */
/*! HCI_LE_Transmitter_Test */
typedef struct
{
    bleChannelFrequency_t       txChannel;
    uint8_t                     lengthOfTestData;
    uint8_t                     packetPayload;
} hciLeTransmitterTestCommand_t;

#if (gBLE41_d == TRUE)
/*! OCF 0x0020 */
/*! LE_Remote_Connection_Parameter_Request_Reply */
typedef struct
{
    uint16_t            connectionHandle;
    uint16_t            intervalMin;
    uint16_t            intervalMax;
    uint16_t            latency;
    uint16_t            timeout;
    uint16_t            minimumCeLength;
    uint16_t            maximumCeLength;
} hciLeRemoteConnectionParameterRequestReplyCommand_t;

/*! OCF 0x0021 */
/*! LE_Remote_Connection_Parameter_Request_Negative_Reply */
typedef struct
{
    uint16_t            connectionHandle;
    hciErrorCode_t      reason;
} hciLeRemoteConnectionParameterRequestNegativeReplyCommand_t;
#endif /* gBLE41_d */

#if (gBLE42_d == TRUE)
/*! HCI_LE_Set_Data_Length */
typedef struct
{
    uint16_t            connectionHandle;
    uint16_t            txOctets;
    uint16_t            txTime;
} hciLeSetDataLengthCommand_t;

/*! HCI_LE_Write_Suggested_Default_Data_Length */
typedef struct
{
    uint16_t            suggestedMaxTxOctets;
    uint16_t            suggestedMaxTxTime;
} hciLeWriteSuggestedDefaultDataLengthCommand_t;

/*! HCI_LE_Generate_DHKey */
typedef struct
{
    uint8_t             remoteP256PublicKey[64];
} hciLeGenerateDhKeyCommand_t;

/*! HCI_LE_Add_Device_To_Resolving_List */
typedef struct
{
    bleAddressType_t    peerIdentityAddressType;
    uint8_t             peerIdentityAddress[gcBleDeviceAddressSize_c];
    uint8_t             peerIRK[gcSmpIrkSize_c];
    uint8_t             localIRK[gcSmpIrkSize_c];
} hciLeAddDeviceToResolvingListCommand_t;

/*! HCI_LE_Remove_Device_From_Resolving_List */
typedef struct
{
    bleAddressType_t    peerIdentityAddressType;
    uint8_t             peerIdentityAddress[gcBleDeviceAddressSize_c];
} hciLeRemoveDeviceFromResolvingListCommand_t;

/*! HCI_LE_Read_Peer_Resolvable_Address */
typedef struct
{
    bleAddressType_t    peerIdentityAddressType;
    uint8_t             peerIdentityAddress[gcBleDeviceAddressSize_c];
} hciLeReadPeerResolvableAddressCommand_t;

/*! HCI_LE_Read_Local_Resolvable_Address */
typedef struct
{
    bleAddressType_t    peerIdentityAddressType;
    uint8_t             peerIdentityAddress[gcBleDeviceAddressSize_c];
} hciLeReadLocalResolvableAddressCommand_t;

/*! HCI_LE_Set_Address_Resolution_Enable */
typedef struct
{
    bool_t              addressResolutionEnable;
} hciLeSetAddressResolutionEnableCommand_t;

/*! HCI_LE_Set_Resolvable_Private_Address_Timeout */
typedef struct
{
    uint16_t            rpaTimeout;
} hciLeSetResolvablePrivateAddressTimeoutCommand_t;

/*! OCF 0x004E */
/*! HCI_LE_Set_Privacy_Mode */
typedef struct
{
    bleAddressType_t    peerIdentityAddressType;
    uint8_t             peerIdentityAddress[gcBleDeviceAddressSize_c];
    blePrivacyMode_t    privacyMode;
} hciLeSetPrivacyModeCommand_t;

#endif /* (gBLE42_d == TRUE) */

/*! OCF 0x0030 */
/*! HCI_LE_Read_Phy */
typedef struct
{
    uint16_t            connectionHandle;
} hciLeReadPhyCommand_t;

/*! OCF 0x0031 */
/*! HCI_LE_Set_Default_Phy */
typedef struct
{
    uint8_t             allPhys;
    uint8_t             txPhys;
    uint8_t             rxPhys;
} hciLeSetDefaultPhyCommand_t;

/*! OCF 0x0032 */
/*! HCI_LE_Set_Phy */
typedef struct
{
    uint16_t            connectionHandle;
    uint8_t             allPhys;
    uint8_t             txPhys;
    uint8_t             rxPhys;
    uint16_t            phyOptions;
} hciLeSetPhyCommand_t;

/*! OCF 0x0033 */
/*! HCI_LE_Enhanced_Receiver_Test */
typedef struct
{
    uint8_t             rxChannel;
    uint8_t             phy;
    uint8_t             modulationIndex;
} hciLeEnhancedReceiverTestCommand_t;

/*! OCF 0x0034 */
/*! HCI_LE_Enhanced_Transmitter_Test */
typedef struct
{
    uint8_t             txChannel;
    uint8_t             dataLen;
    uint8_t             payloadType;
    uint8_t             phy;
} hciLeEnhancedTransmitterTestCommand_t;

typedef struct
{
    uint16_t            cmdFlag;
    uint8_t*            pParams;
    uint8_t             paramsLength;
}hciVendorSpecificDebugCommand_t;

typedef struct
{
    uint8_t                       powerLevel;
    bleTransmitPowerChannelType_t channelType;
} hciSetTxPowerLevelCommand_t;

typedef struct
{
    hciErrorCode_t                status;
} hciSetTxPowerCommandComplete_t;

typedef struct
{
    uint16_t    connectionHandle;
    uint16_t    eventType;      /*! bleNotificationEventType_t */
} hciVendorEnhancedNotificationCommand_t;

typedef struct
{
    hciErrorCode_t                status;
} hciVendorEnhancedNotificationCommandComplete_t;

#if (gBLE50_d == TRUE)
/*! OCF 0x0035 */
/*! HCI_LE_Set_Advertising_Set_Random_Address */
typedef struct hciLeSetAdvSetRandomAddrCommand_tag
{
    uint8_t                      advertisingHandle;
    uint8_t                      randomAddress[gcBleDeviceAddressSize_c];
}hciLeSetAdvSetRandomAddrCommand_t;

/*! OCF 0x0036 */
/*! HCI_LE_Set_Ext_Advertising_Parameters */
typedef struct hciLeSetExtAdvParamsCommand_tag
{
    uint8_t                      advertisingHandle;
    uint16_t                     advertisingEventProperties;
    uint32_t                     primaryAdvertisingIntervalMin;
    uint32_t                     primaryAdvertisingIntervalMax;
    bleAdvertisingChannelMap_t   primaryAdvertisingChannelMap;
    hciBleAddressType_t          ownAddressType;
    hciBleAddressType_t          peerAddressType;
    uint8_t                      peerAddress[gcBleDeviceAddressSize_c];
    bleAdvertisingFilterPolicy_t advertisingFilterPolicy;
    int8_t                       advertisingTxPower;
    uint8_t                      primaryAdvertisingPHY;
    uint8_t                      secondaryAdvertisingMaxSkip;
    uint8_t                      secondaryAdvertisingPHY;
    uint8_t                      advertisingSID;
    uint8_t                      scanRequestNotificationEnable;
} hciLeSetExtAdvParamsCommand_t;

/*! OCF 0x0037 */
/*! HCI_LE_Set_Ext_Advertising_Data */
typedef struct hciLeSetExtAdvertisingDataCommand_tag
{
    uint8_t                      advertisingHandle;
    hciBleExtDataOperation_t     operation;
    hciBleFragmentPreference_t   fragmentPreference;
    uint8_t                      advertisingDataLength;
    const uint8_t               *pAdvertisingData;
} hciLeSetExtAdvertisingDataCommand_t;

/*! OCF 0x0038 */
/*! HCI_LE_Set_Ext_Scan_Response Data */
typedef struct hciLeSetExtScanRespDataCommand_tag
{
    uint8_t                      advertisingHandle;
    hciBleExtDataOperation_t     operation;
    hciBleFragmentPreference_t   fragmentPreference;
    uint8_t                      scanResponseDataLength;
    const uint8_t               *pScanResponseData;
} hciLeSetExtScanRespDataCommand_t;

/*! OCF 0x0039 */
/*! HCI_LE_Set_Extended_Advertising_Enable */
typedef struct hciLeSetExtAdvEnableCommand_tag
{
    uint8_t                      enable;
    uint8_t                      numberOfSets;
    uint8_t                      advertisingHandle[gHciMaxAdvertisingSets_c];
    uint16_t                     duration[gHciMaxAdvertisingSets_c];
    uint8_t                      maxExtAdvertisingEvents[gHciMaxAdvertisingSets_c];
} hciLeSetExtAdvEnableCommand_t;

/*! OCF 0x003A */
/*! HCI_LE_Read_Maximum_Advertising_Data_Length - No parameters */

/*! OCF 0x003B */
/*! HCI_LE_Read_Number_Of_Supported_Advertising_Sets - No parameters */

/*! OCF 0x003C */
/*! HCI_LE_Remove_Advertising_Set */
typedef struct hciLeRemoveAdvSetCommand_tag
{
    uint8_t                      advertisingHandle;
} hciLeRemoveAdvSetCommand_t;

/*! OCF 0x003D */
/*! HCI_LE_Clear_Advertising_Sets - No parameters */

/*! OCF 0x003E */
/*! HCI_LE_Set_Periodic_Advertising_Parameters */
typedef struct hciLeSetPeriodicAdvParamsCommand_tag
{
    uint8_t                      advertisingHandle;
    uint16_t                     periodicAdvertisingIntervalMin;
    uint16_t                     periodicAdvertisingIntervalMax;
    uint16_t                     periodicAdvertisingProperties;
} hciLeSetPeriodicAdvParamsCommand_t;

/*! OCF 0x003F */
/*! HCI_LE_Set_Periodic_Advertising_Data */
typedef struct hciLeSetPeriodicAdvDataCommand_tag
{
    uint8_t                     advertisingHandle;
    uint8_t                     operation;
    uint8_t                     advertisingDataLength;
    uint8_t                     *pAdvertisingData;
} hciLeSetPeriodicAdvDataCommand_t;

/*! OCF 0x0040 */
/*! HCI_LE_Set_Periodic_Advertising_Enable */
typedef struct hciLeSetPeriodicAdvEnableCommand_tag
{
    uint8_t enable;
    uint8_t advertisingHandle;
} hciLeSetPeriodicAdvEnableCommand_t;

/*! OCF 0x0041 */
/*! HCI_LE_Set_Ext_Scan_Parameters */
typedef struct hciLeSetExtScanParamsCommand_tag
{
    hciBleAddressType_t          ownAddressType;
    bleScanningFilterPolicy_t    scanningFilterPolicy;
    uint8_t                      scanningPHYs;
    bleScanType_t                scanType;
    uint16_t                     scanInterval;
    uint16_t                     scanWindow;
} hciLeSetExtScanParamsCommand_t;

/*! OCF 0x0042 */
/*! HCI_LE_Ext_Scan_Enable */
typedef struct hciLeExtScanEnableCommand_tag
{
    hciLeScanEnable_t            enable;
    hciLeFilterDuplicates_t      filterDuplicates;
    uint16_t                     duration;
    uint16_t                     period;
} hciLeExtScanEnableCommand_t;

/*! OCF 0x0043 */
/*! HCI_LE_Ext_Create_Connection */
typedef struct hciLeExtCreateConnectionCommand_tag
{
    bleInitiatorFilterPolicy_t   initiatorFilterPolicy;
    hciBleAddressType_t          ownAddressType;
    hciBleAddressType_t          peerAddressType;
    uint8_t                      peerAddress[gcBleDeviceAddressSize_c];
    uint8_t                      initiatingPHYs;
    uint16_t                     scanInterval[gHciMaxPHYs_c];
    uint16_t                     scanWindow[gHciMaxPHYs_c];
    uint16_t                     connIntervalMin[gHciMaxPHYs_c];
    uint16_t                     connIntervalMax[gHciMaxPHYs_c];
    uint16_t                     connLatency[gHciMaxPHYs_c];
    uint16_t                     supervisionTimeout[gHciMaxPHYs_c];
    uint16_t                     minimumCELength[gHciMaxPHYs_c];
    uint16_t                     maximumCELength[gHciMaxPHYs_c];
} hciLeExtCreateConnectionCommand_t;

/*! OCF 0x0044 */
/*! HCI_LE_Periodic_Advertising_Create_Sync */
typedef struct hciLePeriodicAdvCreateSyncCommand_tag
{
    bleInitiatorFilterPolicy_t   filterPolicy;
    uint8_t                      advertisingSID;
    hciBleAddressType_t          advertiserAddressType;
    uint8_t                      advertiserAddress[gcBleDeviceAddressSize_c];
    uint16_t                     skip;
    uint16_t                     syncTimeout;
    uint8_t                      unused;
} hciLePeriodicAdvCreateSyncCommand_t;

/*! OCF 0x0045 */
/*! HCI_LE_Periodic_Advertising_Create_Sync_Cancel - No parameters */

/*! OCF 0x0046 */
/*! HCI_LE_Periodic_Advertising_Terminate_Sync */
typedef struct hciLePeriodicAdvTerminateSyncCommand_tag
{
    uint16_t                     syncHandle;
} hciLePeriodicAdvTerminateSyncCommand_t;

/*! OCF 0x0047 */
/*! HCI_LE_Add_Device_To_Periodic Advertiser_List */
typedef struct hciLeAddDeviceToPeriodicAdvListCommand_tag
{
    hciBleAddressType_t          advertiserAddressType;
    uint8_t                      advertiserAddress[gcBleDeviceAddressSize_c];
    uint8_t                      advertisingSID;
} hciLeAddDeviceToPeriodicAdvListCommand_t;

/*! OCF 0x0048 */
/*! HCI_LE_Remove_Device_From_Periodic_Advertiser_List */
typedef struct hciLeRemoveDeviceFromPeriodicAdvListCommand_tag
{
    hciBleAddressType_t          advertiserAddressType;
    uint8_t                      advertiserAddress[gcBleDeviceAddressSize_c];
    uint8_t                      advertisingSID;
} hciLeRemoveDeviceFromPeriodicAdvListCommand_t;

/*! OCF 0x0049 */
/*! HCI_LE_Clear_Periodic_Advertiser_List - No parameters */

/*! OCF 0x004A */
/*! HCI_LE_Read_Periodic_Advertiser_List_Size - No parameters */

#endif /* (gBLE50_d == TRUE) */

/*
* Command Complete Parameters
*/
/*! HCI Command Complete Events ************************************************
*   OGF : 0x08 : LE Controller Commands
***************************************************************************** */
/*! HCI_LE_Set_Event_Mask - Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
} hciLeSetEventMaskCommandComplete_t;

/*! HCI_LE_Read_Buffer_Size - Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
    uint16_t            hcLeDataPacketLength;
    uint8_t             hcTotalNumLeDataPackets;
} hciLeReadBufferSizeCommandComplete_t;

/*! HCI_LE_Read_Local_Supported_Features - Return Parameters */
/* Only LMP Feature Page 0 is supported and required by BLE */
typedef struct
{
    hciErrorCode_t      status;
    uint8_t             leFeatures[8]; /* hciLeSupportedFeatures_tag */
} hciLeReadLocalSupportedFeaturesCommandComplete_t;

/*! HCI_LE_Set_Random_Address - Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
} hciLeSetRandomAddressCommandComplete_t;

/*! HCI_LE_Set_Advertising_Parameters - Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
} hciLeSetAdvertingParametersCommandComplete_t;

/*! HCI_LE_Read_Advertising_Channel_Tx_Power - Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
    int8_t              transmitPowerlevel; /* This is a signed 8 bit integer - Range: -20..10 dBm */
} hciLeReadAdvertisingChannelTxPowerCommandComplete_t;

/*! HCI_LE_Set_Advertising_Data - Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
} hciLeSetAdvertisingDataCommandComplete_t;

/*! HCI_LE_Set_Scan_Response_Data - Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
} hciLeSetScanResponseDataCommandComplete_t;

/*! HCI_LE_Set_Advertising_Enable - Return Parameters */
typedef struct
{
    hciErrorCode_t              status;
} hciLeSetAdvertisingEnableCommandComplete_t;

/*! HCI_LE_Set_Scan_Parameters - Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
} hciLeSetScanParametersCommandComplete_t;

/*! HCI_LE_Set_Scan_Enable - Return Parameters */
typedef struct
{
    hciErrorCode_t              status;
} hciLeSetScanEnableCommandComplete_t;

/*! HCI_LE_Create_Connection_Cancel - Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
} hciLeCreateConnectionCancelCommandComplete_t;

/*! HCI_LE_Read_White_List_Size - Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
    uint8_t             whiteListSize;
} hciLeReadWhiteListSizeCommandComplete_t;

/*! HCI_LE_Clear_White_List - Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
} hciLeClearWhiteListCommandComplete_t;

/*! HCI_LE_Add_Device_To_White_List - Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
} hciLeAddDeviceToWhiteListCommandComplete_t;

/*! HCI_LE_Remove_Device_From_White_List - Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
} hciLeRemoveDeviceFromWhiteListCommandComplete_t;

/*! HCI_LE_Set_Host_Channel_Classification - Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
} hciLeSetHostChannelClassificationCommandComplete_t;

/*! HCI_LE_Read_Channel_Map - Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
    uint16_t            connectionHandle;
    uint8_t             channelMap[gcBleChannelMapSize_c];
} hciLeReadChannelMapCommandComplete_t;

#if defined(gHciLeEncryptSupport_d) && (gHciLeEncryptSupport_d == TRUE)
/*! HCI_LE_Encrypt - Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
    uint8_t             encryptedData[16];
} hciLeEncryptCommandComplete_t;
#endif

#if defined(gHciLeRandomSupport_d) && (gHciLeRandomSupport_d == TRUE)
/*! HCI_LE_Rand - Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
    uint8_t             randomNumber[8];
} hciLeRandCommandComplete_t;
#endif

/*! HCI_LE_Long_Term_Key_Request_Reply - Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
    uint16_t            connectionHandle;
} hciLeLongTermKeyRequestReplyCommandComplete_t;

/*! HCI_LE_Long_Term_Key_Request_Negative_Reply - Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
    uint16_t            connectionHandle;
} hciLeLongTermKeyRequestNegativeReplyCommandComplete_t;

/*! HCI_LE_Read_Supported_States - Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
    uint8_t             leStates[8]; /* hciLeLLSupportedStatesShift_t */
} hciLeReadSupportedStatesCommandComplete_t;

/*! HCI_LE_Receiver_Test - Return Parameters */
typedef struct
{
    hciErrorCode_t          status;
} hciLeReceiverTestCommandComplete_t;

/*! HCI_LE_Transmitter_Test - Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
} hciLeTransmitterTestCommandComplete_t;

/*! HCI_LE_Test_End - Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
    uint16_t            numberOfPackets;
} hciLeTestEndCommandComplete_t;

#if (gBLE41_d == TRUE)
/*! LE_Remote_Connection_Parameter_Request_Reply - Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
    uint16_t            connectionHandle;
} hciLeRemoteConnectionParameterRequestReplyCommandComplete_t;

/*! LE_Remote_Connection_Parameter_Request_Negative_Reply - Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
    uint16_t            connectionHandle;
} hciLeRemoteConnectionParameterRequestNegativeReplyCommandComplete_t;
#endif /* gBLE41_d */

#if (gBLE42_d == TRUE)
/*! HCI_LE_Set_Data_Length Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
    uint16_t            connectionHandle;
} hciLeSetDataLengthCommandComplete_t;

/*! HCI_LE_Read_Suggested_Default_Data_Length Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
    uint16_t            suggestedMaxTxOctets;
    uint16_t            suggestedMaxTxTime;
} hciLeReadSuggestedDefaultDataLengthCommandComplete_t;

/*! HCI_LE_Write_Suggested_Default_Data_Length Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
} hciLeWriteSuggestedDefaultDataLengthCommandComplete_t;

/*! HCI_LE_Add_Device_To_Resolving_List Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
} hciLeAddDeviceToResolvingListCommandComplete_t;

/*! HCI_LE_Clear_Resolving_List Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
} hciLeClearResolvingListCommandComplete_t;

/*! HCI_LE_Read_Peer_Resolvable_Address Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
    uint8_t             peerResolvableAddress[gcBleDeviceAddressSize_c];
} hciLeReadPeerResolvableAddressCommandComplete_t;

/*! HCI_LE_Read_Local_Resolvable_Address Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
    uint8_t             localResolvableAddress[gcBleDeviceAddressSize_c];
} hciLeReadLocalResolvableAddressCommandComplete_t;

/*! HCI_LE_Set_Address_Resolution_Enable Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
} hciLeSetAddressResolutionEnableCommandComplete_t;

/*! HCI_LE_Set_Resolvable_Private_Address_Timeout Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
} hciLeSetResolvablePrivateAddressTimeoutCommandComplete_t;

/*! HCI_LE_Set_Privacy_Mode */
typedef struct hciLeSetPrivacyModeCommandComplete_tag
{
    hciErrorCode_t      status;
} hciLeSetPrivacyModeCommandComplete_t;

#endif /* (gBLE42_d == TRUE) */

/*! HCI_LE_Read_Phy - Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
    uint16_t            connectionHandle;
    uint8_t             txPhy;
    uint8_t             rxPhy;
} hciLeReadPhyCommandComplete_t;

/*! HCI_LE_Set_Default_Phy - Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
} hciLeSetDefaultPhyCommandComplete_t;

/*! HCI_LE_Enhanced_Receiver_Test - Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
} hciLeEnhancedReceieverTestCommandComplete_t;

/*! HCI_LE_Enhanced_Transmitter_Test - Return Parameters */
typedef struct
{
    hciErrorCode_t      status;
} hciLeEnhancedTransmitterTestCommandComplete_t;

#if (gBLE50_d == TRUE)
/*! HCI_LE_Set_Advertising_Set_Random_Address - Return Parameters */
typedef struct hciLeSetAdvSetRandomAddrCommandComplete_tag
{
    hciErrorCode_t      status;
} hciLeSetAdvSetRandomAddrCommandComplete_t;

/*! HCI_LE_Set_Ext_Advertising_Parameters - Return Parameters */
typedef struct hciLeSetExtAdvParamsCommandComplete_tag
{
    hciErrorCode_t      status;
    int8_t              transmitPowerlevel; /* This is a signed 8 bit integer - Range: -20..10 dBm */
} hciLeSetExtAdvertingParamsCommandComplete_t;

/*! HCI_LE_Set_Ext_Advertising_Data - Return Parameters */
typedef struct hciLeSetExtAdvertisingDataCommandComplete_tag
{
    hciErrorCode_t      status;
} hciLeSetExtAdvertingDataCommandComplete_t;

/*! HCI_LE_Set_Ext_Scan_Response_Data - Return Parameters */
typedef struct hciLeSetExtScanRespDataCommandComplete_tag
{
    hciErrorCode_t      status;
} hciLeSetExtScanRespDataCommandComplete_t;

/*! HCI_LE_Set_Extended_Advertising_Enable - Return Parameters */
typedef struct hciLeSetExtAdvEnableCommandComplete_tag
{
    hciErrorCode_t      status;
} hciLeSetExtAdvEnableCommandComplete_t;

/*! HCI_LE_Read_Maximum_Advertising_Data_Length - Return Parameters */
typedef struct hciLeReadMaxAdvDataLenCommandComplete_tag
{
    hciErrorCode_t      status;
    uint16_t            maxAdvDataLen;
} hciLeReadMaxAdvDataLenCommandComplete_t;

/*! HCI_LE_Read_Number_Of_Supported_Advertising_Sets - Return Parameters */
typedef struct hciLeReadNumOfSupportedAdvSetsCommandComplete_tag
{
    hciErrorCode_t      status;
    uint8_t             numOfAdvSets;
} hciLeReadNumOfSupportedAdvSetsCommandComplete_t;

/*! HCI_LE_Remove_Advertising_Set */
typedef struct hciLeRemoveAdvSetCommandComplete_tag
{
    hciErrorCode_t      status;
} hciLeRemoveAdvSetCommandComplete_t;

/*! HCI_LE_Clear_Advertising_Sets - Return Parameters */
typedef struct hciLeClearAdvSetsCommandComplete_tag
{
    hciErrorCode_t      status;
} hciLeClearAdvSetsCommandComplete_t;

/*! HCI_LE_Set_Periodic_Advertising_Parameters */
typedef struct hciLeSetPeriodicAdvParamsCommandComplete_tag
{
    hciErrorCode_t      status;
} hciLeSetPeriodicAdvParamsCommandComplete_t;

/*! HCI_LE_Set_Periodic_Advertising_Data - Return Parameters */
typedef struct hciLeSetPeriodicAdvDataCommandComplete_tag
{
    hciErrorCode_t      status;
} hciLeSetPeriodicAdvDataCommandComplete_t;

/*! HCI_LE_Set_Periodic_Advertising_Enable - Return Parameters */
typedef struct hciLeSetPeriodicAdvEnableCommandComplete_tag
{
    hciErrorCode_t      status;
} hciLeSetPeriodicAdvEnableCommandComplete_t;

/*! HCI_LE_Set_Ext_Scan_Parameters - Return Parameters */
typedef struct hciLeSetExtScanParamsCommandComplete_tag
{
    hciErrorCode_t      status;
} hciLeSetExtScanParamsCommandComplete_t;

/*! HCI_LE_Set_Ext_Scan_Enable - Return Parameters */
typedef struct hciLeSetExtScanEnableCommandComplete_tag
{
    hciErrorCode_t      status;
} hciLeSetExtScanEnableCommandComplete_t;

/*! HCI_LE_Periodic_Advertising_Create_Sync_Cancel - Return Parameters */
typedef struct hciLePeriodicAdvCreateSyncCancelCommandComplete_tag
{
    hciErrorCode_t      status;
} hciLePeriodicAdvCreateSyncCancelCommandComplete_t;

/*! HCI_LE_Periodic_Advertising_Terminate_Sync */
typedef struct hciLePeriodicAdvTerminateSyncCommandComplete_tag
{
    hciErrorCode_t      status;
} hciLePeriodicAdvTerminateSyncCommandComplete_t;

/*! HCI_LE_Clear_Periodic_Advertiser_List - Return Parameters */
typedef struct hciLeClearPeriodicAdvListCommandComplete_tag
{
    hciErrorCode_t      status;
} hciLeClearPeriodicAdvListCommandComplete_t;

typedef struct hciLeAddDeviceToPeriodicAdvListCommandComplete_tag
{
    hciErrorCode_t      status;
} hciLeAddDeviceToPeriodicAdvListCommandComplete_t;

/*! HCI_LE_Remove_Device_From_Periodic_Advertiser_List - Return Parameters */
typedef struct hciLeRemoveDeviceFromPeriodicAdvListCommandComplete_tag
{
    hciErrorCode_t      status;
} hciLeRemoveDeviceFromPeriodicAdvListCommandComplete_t;

/*! HCI_LE_Read_Periodic_Advertiser_List_Size */
typedef struct hciLeReadPeriodicAdvListSizeCommandComplete_tag
{
    hciErrorCode_t      status;
    uint8_t             periodicAdvListSize;
} hciLeReadPeriodicAdvListSizeCommandComplete_t;

#endif /* (gBLE50_d == TRUE) */

/*! LE Meta Event : 0x3E */
typedef struct
{
    hciLeMetaSubEventCode_t        subEventCode;
    PACKED_UNION
    {
        hciLeConnectionCompleteEvent_t                  hciLeConnCompleteEvent;
        hciLeAdvertisingReportEvent_t                   hciLeAdvReportEvent;
        hciLeConnectionUpdateCompleteEvent_t            hciLeConnUpdateCompleteEvent;
        hciLeReadRemoteFeaturesCompleteEvent_t          hciLeReadRemoteFeaturesCompleteEvent;
        hciLeLongTermKeyRequestEvent_t                  hciLeLongTermKeyReqEvent;
#if (gBLE41_d == TRUE)
        hciLeRemoteConnectionParameterRequestEvent_t    hciLeRemoteConnectionParameterRequestEvent;
#endif /* gBLE41_d */
#if (gBLE42_d == TRUE)
        hciLeDataLengthChangeEvent_t                    hciLeDataLengthChangeEvent;
        hciLeReadLocalP256PublicKeyCompleteEvent_t      hciLeReadLocalP256PublicKeyCompleteEvent;
        hciLeGenerateDhKeyCompleteEvent_t               hciLeGenerateDhKeyCompleteEvent;
        hciLeEnhancedConnectionCompleteEvent_t          hciLeEnhancedConnCompleteEvent;
        hciLeDirectedAdvertisingReportEvent_t           hciLeDirectedAdvReportEvent;
#endif /* gBLE42_d */
#if (gBLE50_d == TRUE) || (gBLE42_d == TRUE)
        hciLePhyUpdateCompleteEvent_t                   hciLePhyUpdateCompleteEvent;
#endif
#if (gBLE50_d == TRUE)
        hciLeExtAdvReportEvent_t                        hciLeExtAdvReportEvent;
        hciLePeriodicAdvReportEvent_t                   hciLePeriodicAdvReportEvent;
        hciLePeriodicAdvSyncEstbEvent_t                 hciLePeriodicAdvSyncEstbEvent;
        hciLePeriodicAdvSyncLostEvent_t                 hciLePeriodicAdvSyncLostEvent;
        hciLeAdvSetTerminatedEvent_t                    hciLeAdvSetTerminatedEvent;
        hciLeScanReqReceivedEvent_t                     hciLeScanReqReceivedEvent;
        hciLeChannelSelAlgorithmEvent_t                 hciLeChannelSelAlgorithm;
#endif
        hciVendorEnhancedNotificationEvent_t            hciEnhancedNotificationEvent;
    }eventData;
} hciLeMetaEvent_t;

typedef struct
{
    uint8_t                 numHciCommandPackets;
    hciCommandOpCode_t      commandOpCode;
    union
    {
        hciErrorCode_t                                          status;
        /*! HCI OGF : 0x03 : Controller & Baseband Commands */
        hciSetEventMaskCommandComplete_t                        hciSetEventMaskCommComplete;
        hciResetCommandComplete_t                               hciResetCommComplete;
        hciReadTransmitPowerLevelCommandComplete_t              hciReadTransmitPowerLevelCommComplete;
        hciSetControllerToHostFlowControlCommandComplete_t      hciSetControllerToHostFlowControlCommComplete;
        hciHostBufferSizeCommandComplete_t                      hciHostBufferSizeCommComplete;
        hciHostNumberOfCompletedPacketsCommandComplete_t        hciHostNumberOfCompletedPacketsCommComplete;
#if (gBLE41_d == TRUE)
        hciSetEventMaskPage2CommandComplete_t                   hciSetEventMaskPage2CommComplete;
        hciReadAuthenticatedPayloadTimeoutCommandComplete_t     hciReadAuthenticatedPayloadTimeoutCommComplete;
        hciWriteAuthenticatedPayloadTimeoutCommandComplete_t    hciWriteAuthenticatedPayloadTimeoutCommComplete;
#endif /* gBLE41_d */
        /*! HCI OGF : 0x04 : Informational Parameters */
        hciReadLocalVersionInfoCommandComplete_t                hciReadLocalVersionInfoCommComplete;
        hciReadLocalSupportedCommandsCommandComplete_t          hciReadLocalSupportedCommandsCommComplete;
        hciReadLocalSupportedFeaturesCommandComplete_t          hciReadLocalSupportedFeaturesCommComplete;
        hciReadBdAddrCommandComplete_t                          hciReadBdAddrCommComplete;
        /*! HCI OGF : 0x05 : Status Parameters */
        hciReadRssiCommandComplete_t                            hciReadRssiCommComplete;
        /*! HCI OGF : 0x08 : LE Controller Commands */
        hciLeSetEventMaskCommandComplete_t                      hciLeSetEventMaskCommComplete;
        hciLeReadBufferSizeCommandComplete_t                    hciLeReadBufferSizeCommComplete;
        hciLeReadLocalSupportedFeaturesCommandComplete_t        hciLeReadLocalSupportedFeaturesCommComplete;
        hciLeSetRandomAddressCommandComplete_t                  hciLeSetRandomAddressCommComplete;
        hciLeSetAdvertingParametersCommandComplete_t            hciLeSetAdvertingParametersCommComplete;
        hciLeReadAdvertisingChannelTxPowerCommandComplete_t     hciLeReadAdvertisingChannelTxPowerCommComplete;
        hciLeSetAdvertisingDataCommandComplete_t                hciLeSetAdvertiseDataCommComplete;
        hciLeSetScanResponseDataCommandComplete_t               hciLeSetScanResponseDataCommComplete;
        hciLeSetAdvertisingEnableCommandComplete_t              hciLeSetAdvertisingEnableCommComplete;
        hciLeSetScanParametersCommandComplete_t                 hciLeSetScanParametersCommComplete;
        hciLeSetScanEnableCommandComplete_t                     hciLeSetScanEnableCommComplete;
        hciLeCreateConnectionCancelCommandComplete_t            hciLeCreateConnectionCancelCommComplete;
        hciLeReadWhiteListSizeCommandComplete_t                 hciLeReadWhiteListSizeCommComplete;
        hciLeClearWhiteListCommandComplete_t                    hciLeClearWhiteListCommComplete;
        hciLeAddDeviceToWhiteListCommandComplete_t              hciLeAddDeviceToWhiteListCommComplete;
        hciLeRemoveDeviceFromWhiteListCommandComplete_t         hciLeRemoveDeviceFromWhiteListCommComplete;
        hciLeSetHostChannelClassificationCommandComplete_t      hciLeSetHostChannelClassificationCommComplete;
        hciLeReadChannelMapCommandComplete_t                    hciLeReadChannelMapCommComplete;
#if defined(gHciLeEncryptSupport_d) && (gHciLeEncryptSupport_d == TRUE)
        hciLeEncryptCommandComplete_t                           hciLeEncryptCommComplete;
#endif
#if defined(gHciLeRandomSupport_d) && (gHciLeRandomSupport_d == TRUE)
        hciLeRandCommandComplete_t                              hciLeRandCommComplete;
#endif
        hciLeLongTermKeyRequestReplyCommandComplete_t           hciLeLongTermKeyRequestReplyCommComplete;
        hciLeLongTermKeyRequestNegativeReplyCommandComplete_t   hciLeLongTermKeyRequestNegativeReplyCommComplete;
        hciLeReadSupportedStatesCommandComplete_t               hciLeReadSupportedStatesCommComplete;
        hciLeReceiverTestCommandComplete_t                      hciLeReceiverTestCommComplete;
        hciLeTransmitterTestCommandComplete_t                   hciLeTransmitterTestCommComplete;
        hciLeTestEndCommandComplete_t                           hciLeTestEndCommComplete;
#if (gBLE41_d == TRUE)
        hciLeRemoteConnectionParameterRequestReplyCommandComplete_t             hciLeRemoteConnectionParameterRequestReplyCommComplete;
        hciLeRemoteConnectionParameterRequestNegativeReplyCommandComplete_t     hciLeRemoteConnectionParameterRequestNegativeReplyCommComplete;
#endif /* gBLE41_d */
#if (gBLE42_d == TRUE)
        hciLeWriteSuggestedDefaultDataLengthCommandComplete_t    hciLeWriteSuggestedDefaultDataLengthCommComplete;
        hciLeReadSuggestedDefaultDataLengthCommandComplete_t     hciLeReadSuggestedDefaultDataLengthCommComplete;
        hciLeSetDataLengthCommandComplete_t                      hciLeSetDataLengthCommComplete;
        hciLeSetResolvablePrivateAddressTimeoutCommandComplete_t hciLeSetResolvablePrivateAddressTimeoutCommComplete;
        hciLeSetAddressResolutionEnableCommandComplete_t         hciLeSetAddressResolutionEnableCommComplete;
        hciLeClearResolvingListCommandComplete_t                 hciLeClearResolvingListCommComplete;
        hciLeAddDeviceToResolvingListCommandComplete_t           hciLeAddDeviceToResolvingListCommComplete;
        hciLeReadPeerResolvableAddressCommandComplete_t          hciLeReadPeerResolvableAddressCommComplete;
        hciLeReadLocalResolvableAddressCommandComplete_t         hciLeReadLocalResolvableAddressCommComplete;
        hciLeSetPrivacyModeCommandComplete_t                     hciLeSetPrivacyModeCommComplete;
#endif /* gBLE42_d */
        /* v5.0 */
        hciLeReadPhyCommandComplete_t                            hciLeReadPhyCommandComplete;
        hciLeSetDefaultPhyCommandComplete_t                      hciLeSetDefaultPhyCommandComplete;
        hciLeEnhancedReceieverTestCommandComplete_t              hciLeEnhancedReceieverTestCommandComplete;
        hciLeEnhancedTransmitterTestCommandComplete_t            hciLeEnhancedTransmitterTestCommandComplete;
        hciSetTxPowerCommandComplete_t                           hciSetTxPowerCommComplete;
        hciVendorEnhancedNotificationCommandComplete_t           hciVendorEnhNotifCommComplete;
#if (gBLE50_d == TRUE)
        hciLeSetAdvSetRandomAddrCommandComplete_t                hciLeSetAdvSetRandomAddrCommComplete;
        hciLeSetExtAdvertingParamsCommandComplete_t              hciLeSetExtAdvertingParamsCommComplete;
        hciLeSetExtAdvertingDataCommandComplete_t                hciLeSetExtAdvertingDataCommComplete;
        hciLeSetExtScanRespDataCommandComplete_t                 hciLeSetExtScanRespDataCommComplete;
        hciLeSetExtAdvEnableCommandComplete_t                    hciLeSetExtAdvEnableCommComplete;
        hciLeReadMaxAdvDataLenCommandComplete_t                  hciLeReadMaxAdvDataLenCommComplete;
        hciLeReadNumOfSupportedAdvSetsCommandComplete_t          hciLeReadNumOfSupportedAdvSetsCommComplete;
        hciLeRemoveAdvSetCommandComplete_t                       hciLeRemoveAdvSetCommComplete;
        hciLeClearAdvSetsCommandComplete_t                       hciLeClearAdvSetsCommComplete;
        hciLeSetPeriodicAdvParamsCommandComplete_t               hciLeSetPeriodicAdvParamsCommComplete;
        hciLeSetPeriodicAdvDataCommandComplete_t                 hciLeSetPeriodicAdvDataCommComplete;
        hciLeSetPeriodicAdvEnableCommandComplete_t               hciLeSetPeriodicAdvEnableCommComplete;
        hciLeSetExtScanParamsCommandComplete_t                   hciLeSetExtScanParamsCommComplete;
        hciLeSetExtScanEnableCommandComplete_t                   hciLeSetExtScanEnableCommComplete;
        hciLePeriodicAdvCreateSyncCancelCommandComplete_t        hciLePeriodicAdvCreateSyncCancelCommComplete;
        hciLePeriodicAdvTerminateSyncCommandComplete_t           hciLePeriodicAdvTerminateSyncCommComplete;
        hciLeAddDeviceToPeriodicAdvListCommandComplete_t         hciLeAddDeviceToPeriodicAdvListCommComplete;
        hciLeRemoveDeviceFromPeriodicAdvListCommandComplete_t    hciLeRemoveDeviceFromPeriodicAdvListCommComplete;
        hciLeClearPeriodicAdvListCommandComplete_t               hciLeClearPeriodicAdvListCommComplete;
        hciLeReadPeriodicAdvListSizeCommandComplete_t            hciLeReadPeriodicAdvListSizeCommComplete;
#endif
    }commCompleteReturnParams;
} hciCommandCompleteEvent_t;

typedef struct
{
    hciEventCode_t      eventCode;
    uint8_t             parLength;
    union
    {
        hciDisconnectionCompleteEvent_t                 hciDisconnectionCompleteEvent;
        hciEncryptionChangeEvent_t                      hciEncryptionChangeEvent;
        hciReadRemoteVersionInformationCompleteEvent_t  hciReadRemoteVersionInformationCompleteEvent;
        hciCommandCompleteEvent_t                       hciCommandCompleteEvent;
        hciCommandStatusEvent_t                         hciCommandStatusEvent;
        hciHardwareErrorEvent_t                         hciHardwareErrorEvent;
        hciNumberOfCompletedPacketsEvent_t              hciNumberOfCompletedPacketsEvent;
        hciDataBufferOverflowEvent_t                    hciDataBufferOverflowEvent;
        hciEncryptionKeyRefreshCompleteEvent_t          hciEncryptionKeyRefreshCompleteEvent;
        hciLeMetaEvent_t                                hciLeMetaEvent;
#if (gBLE41_d == TRUE)
        hciAuthenticatedPayloadTimeoutExpiredEvent_t    hciAuthenticatedPayloadTimeoutExpiredEvent;
#endif /* gBLE41_d */
    }eventData;
} hciEvent_t;

/*! ****************************************************************************
*   COMMANDS
***************************************************************************** */

/*! ****************************************************************************
*   DATA PACKETS
***************************************************************************** */

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

extern const uint8_t gMaxAdvReportQueueSize;

/************************************************************************************
*************************************************************************************
* Interface callback type definitions
*************************************************************************************
************************************************************************************/
typedef bleResult_t (*hciLeEventCallback_t) (hciLeMetaEvent_t* pEvent);

typedef bleResult_t (*hciCommandCompleteCallback_t) (hciCommandCompleteEvent_t* pEvent);

typedef bleResult_t (*hciCommandStatusCallback_t) (hciCommandStatusEvent_t* pEvent);

typedef bleResult_t (*hciEventCallback_t) (hciEvent_t* pEvent);

typedef bleResult_t (*hciAclDataCallback_t) (uint16_t connectionHandle, void* pPacket);

typedef struct
{
    /* LE Controller Uplink Interface function pointer */
    hciHostToControllerInterface_t   pHostToController;
}hciInterfaceStruct_t;

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/
#ifdef __cplusplus
    extern "C" {
#endif

/* Generic Interface */
bleResult_t Hci_Init(
                const hciInterfaceStruct_t* pConfigStruct);

bleResult_t Hci_RegisterLeEventCallback(
                hciLeEventCallback_t pLeCallback);

bleResult_t Hci_RegisterCommandCompleteEventCallback(
                hciCommandCompleteCallback_t pCcCallback);

bleResult_t Hci_RegisterCommandStatusEventCallback(
                hciCommandStatusCallback_t pCsCallback);

bleResult_t Hci_RegisterGenericEventCallback(
                hciEventCallback_t pCallback);

bleResult_t Hci_RegisterL2caCallback(
                hciEventCallback_t      pCallback,
                hciAclDataCallback_t    pAclCallback);

bleResult_t Hci_RegisterSmCallback(
                hciEventCallback_t pSmCallback);


/* Device Setup Group */
bleResult_t Hci_Reset(void);                                                                    /* 3.2 */

/* Controller Flow Control Group */
#if (gHciBrEdrCommandSupport_d == TRUE)
bleResult_t Hci_ReadBufferSize(void);                                                           /* 4.5 */
#endif

bleResult_t Hci_LeReadBufferSize(void);                                                         /* 8.2 */

/* Controller Information Group */
#if defined(gHciCompleteHciCmdSupport_d) && (gHciCompleteHciCmdSupport_d == TRUE)
bleResult_t Hci_ReadLocalVersionInformation(void);                                              /* 4.1 */
bleResult_t Hci_ReadLocalSupportedFeatures(void);                                               /* 4.3 */
bleResult_t Hci_LeReadSupportedStates(void);                                                    /* 8.27 */

#endif
bleResult_t Hci_ReadLocalSupportedCommands(void);                                      /* 4.2 */

bleResult_t Hci_ReadBdAddr(void);                                                              /* 4.6 */
bleResult_t Hci_LeReadLocalSupportedFeatures(void);                                   /* 8.3 */
bleResult_t Hci_LeSetRandomAddress(
                const hciLeSetRandomAddressCommand_t *pParam);                                        /* 8.4 */

#if gLeBroadcasterSupported_d
/* Controller Configuration Group */
bleResult_t Hci_LeSetAdvertisingEnable(hciLeAdvertisingEnable_t advertisingEnable);      /* 8.9 */
bleResult_t Hci_LeSetAdvertisingData(
                const hciLeSetAdvertisingDataCommand_t* pParam);                                      /* 8.7 */
bleResult_t Hci_LeSetAdvertisingParameters(
                const hciLeSetAdvertisingParametersCommand_t *pParam);                                /* 8.5 */
bleResult_t Hci_LeSetScanResponseData(
                const hciLeSetScanResponseDataCommand_t *pParam);                                     /* 8.8 */
#endif /* gLeBroadcasterSupported_d */

#if gLeObserverSupported_d
/* Device Discovery Group */
bleResult_t Hci_LeSetScanEnable(const hciLeSetScanEnableCommand_t *pParam);                 /* 8.11 */
bleResult_t Hci_LeSetScanParameters(const hciLeSetScanParametersCommand_t* pParam);       /* 8.10 */
#endif /* gLeObserverSupported_d */

/* Connection Setup Group */
bleResult_t Hci_Disconnect(const hciDisconnectCommand_t *pParam);                              /* 1.6 */

#if gConnMasterSupported_d
bleResult_t Hci_LeCreateConnection(const hciLeCreateConnectionCommand_t *pParam);          /* 8.12 */
bleResult_t Hci_LeCreateConnectionCancel(void);                                         /* 8.13 */
/* Physical Links Group */
bleResult_t Hci_LeSetHostChannelClassification(
                const hciLeSetHostChannelClassificationCommand_t *pParam);                            /* 8.19 */
#endif /* gConnMasterSupported_d */

#if defined(gHciCompleteHciCmdSupport_d) && (gHciCompleteHciCmdSupport_d == TRUE)
/* Remote Information Group */
bleResult_t Hci_ReadRemoteVersionInformation(
                const hciReadRemoteVersionInformationCommand_t *pParam);                              /* 1.23 */
#endif
bleResult_t Hci_LeReadRemoteFeatures(
                const hciLeReadRemoteFeaturesCommand_t *pParam);                                      /* 8.21 */

/* Connection State Group */
bleResult_t Hci_LeConnectionUpdate(const hciLeConnectionUpdateCommand_t* pParam);                     /* 8.18 */
bleResult_t Hci_LeRemoteConnParamReqReply(
                const hciLeRemoteConnectionParameterRequestReplyCommand_t *pParam);                   /* 8.31 */
bleResult_t Hci_LeRemoteConnectionParameterRequestNegativeReply(
                const hciLeRemoteConnectionParameterRequestNegativeReplyCommand_t *pParam);           /* 8.32 */

#if defined(gHciCompleteHciCmdSupport_d) && (gHciCompleteHciCmdSupport_d == TRUE)
/* Host Flow Control Group */
bleResult_t Hci_HostBufferSize(
                const hciHostBufferSizeCommand_t *pParam);                                            /* 3.39 */
bleResult_t Hci_SetControllerToHostFlowControl(
                const hciSetControllerToHostFlowControlCommand_t *pParam);                            /* 3.38 */
bleResult_t Hci_HostNumberOfCompletedPackets(
                const hciHostNumberOfCompletedPacketsCommand_t *pParam);                              /* 3.40 */
#endif

#if (gLePingIsSupported_d == TRUE)
bleResult_t Hci_SetEventMaskPage2(
                const hciSetEventMaskPage2Command_t *pParam);                                         /* 3.69 */
#endif

bleResult_t Hci_SetEventMask(
                const hciSetEventMaskCommand_t *pParam);                                              /* 3.1 */

bleResult_t Hci_LeAddDeviceToWhiteList(
                const hciLeAddDeviceToWhiteListCommand_t *pParam);                                    /* 8.16 */
bleResult_t Hci_LeClearWhiteList(void);                                                         /* 8.15 */
bleResult_t Hci_LeReadWhiteListSize(void);                                                      /* 8.14 */
bleResult_t Hci_LeRemoveDeviceFromWhiteList(
                const hciLeRemoveDeviceFromWhiteListCommand_t *pParam);                               /* 8.17 */
bleResult_t Hci_LeSetEventMask(
                const hciLeSetEventMaskCommand_t *pParam);                                            /* 8.1 */

/* Link Information Group */
bleResult_t Hci_ReadTransmitPowerLevel(const hciReadTransmitPowerLevelCommand_t *pParam);             /* 3.35 */
bleResult_t Hci_ReadRssi(const hciReadRssiCommand_t *pParam);                                         /* 5.4 */
#if gLeBroadcasterSupported_d
bleResult_t Hci_LeReadAdvertisingChannelTxPower(void);                                          /* 8.6 */
#endif
bleResult_t Hci_LeReadChannelMap(const hciLeReadChannelMapCommand_t *pParam);                         /* 8.20 */

/* Data Transfer Group */
bleResult_t Hci_SendAclPacket ( uint8_t *pHciPacket );

/* Authentication and Encryption Group */
#if defined(gHciLeEncryptSupport_d) && (gHciLeEncryptSupport_d == TRUE)
bleResult_t Hci_LeEncrypt(const hciLeEncryptCommand_t *pParam);                                       /* 8.22 */
#endif
#if defined(gHciLeRandomSupport_d) && (gHciLeRandomSupport_d == TRUE)
bleResult_t Hci_LeRand(void);                                                                   /* 8.23 */
#endif
#if (gSmpInitiatorSupported_d==1)
bleResult_t Hci_LeStartEncryption(const hciLeStartEncryptionCommand_t *pParam);                       /* 8.24 */
#endif

#if (gSmpResponderSupported_d == 1)
bleResult_t Hci_LeLongTermKeyRequestReply(
                const hciLeLongTermKeyRequestReplyCommand_t   *pParam);                               /* 8.25 */
bleResult_t Hci_LeLongTermKeyRequestNegativeReply(
                const hciLeLongTermKeyRequestNegativeReplyCommand_t *pParam);                         /* 8.26 */
#endif

#if (gLePingIsSupported_d == TRUE)
bleResult_t Hci_ReadAuthenticatedPayloadTimeout(
                const hciReadAuthenticatedPayloadTimeoutCommand_t *pParam);                           /* 3.93 */
bleResult_t Hci_WriteAuthenticatedPayloadTimeout(
                const hciWriteAuthenticatedPayloadTimeoutCommand_t *pParam);                          /* 3.94 */
#endif

/* Testing Group */
bleResult_t Hci_LeReceiverTest(const hciLeReceiverTestCommand_t *pParam);                             /* 8.28 */
bleResult_t Hci_LeTransmitterTest(const hciLeTransmitterTestCommand_t *pParam);                       /* 8.29 */
bleResult_t Hci_LeTestEnd(void);                                                                /* 8.30 */

/* HCI Vendor Specific Commands */
#if defined(gHciCompleteHciCmdSupport_d) && (gHciCompleteHciCmdSupport_d == TRUE)
bleResult_t Hci_VendorSpecificDebug(const hciVendorSpecificDebugCommand_t *pParam);
#endif
bleResult_t Hci_VendorSpecificSetTxPowerLevel(const hciSetTxPowerLevelCommand_t *pParam);
bleResult_t Hci_VendorSpecificEnhancedNotification(const hciVendorEnhancedNotificationCommand_t *pParam);

#if (gBLE42_d == TRUE)
/* v4.2 specific interface */
bleResult_t Hci_LeSetDataLength(
                const hciLeSetDataLengthCommand_t *pParam);                                           /* 8.33 */
bleResult_t Hci_LeReadSuggestedDefaultDataLength(void);                                         /* 8.34 */
bleResult_t Hci_LeWriteSuggestedDefaultDataLength(
                const hciLeWriteSuggestedDefaultDataLengthCommand_t *pParam);                         /* 8.35 */
#if defined(gHciCompleteHciCmdSupport_d) && (gHciCompleteHciCmdSupport_d == TRUE)
bleResult_t Hci_LeReadLocalP256PublicKey(void);                                                 /* 8.36 */
bleResult_t Hci_LeGenerateDhKey(
                const hciLeGenerateDhKeyCommand_t *pParam);                                           /* 8.37 */
bleResult_t Hci_LeRemoveDeviceFromResolvingList(
                const hciLeRemoveDeviceFromResolvingListCommand_t *pParam);                           /* 8.39 */
bleResult_t Hci_LeReadResolvingListSize(void);                                                  /* 8.41 */
bleResult_t Hci_LeReadLocalResolvableAddress(
                const hciLeReadLocalResolvableAddressCommand_t *pParam);                              /* 8.43 */
bleResult_t Hci_LeReadMaximumDataLength(void);                                                        /* 8.46 */
#endif

bleResult_t Hci_LeAddDeviceToResolvingList(
                const hciLeAddDeviceToResolvingListCommand_t *pParam);                                /* 8.38 */

bleResult_t Hci_LeClearResolvingList(void);                                                     /* 8.40 */
bleResult_t Hci_LeReadPeerResolvableAddress(
                const hciLeReadPeerResolvableAddressCommand_t *pParam);                               /* 8.42 */

bleResult_t Hci_LeSetAddressResolutionEnable(
                const hciLeSetAddressResolutionEnableCommand_t *pParam);                              /* 8.44 */
bleResult_t Hci_LeSetResolvablePrivateAddressTimeout(
                const hciLeSetResolvablePrivateAddressTimeoutCommand_t *pParam);                      /* 8.45 */

bleResult_t Hci_LeSetPrivacyMode(const hciLeSetPrivacyModeCommand_t *pParam);                         /* 8.77 */
#endif /* gBLE42_d */

/* v5.0 specific interface */
bleResult_t Hci_LeReadPhy(const hciLeReadPhyCommand_t *pParam);                                       /* 8.47 */
bleResult_t Hci_LeSetDefaultPhy(const hciLeSetDefaultPhyCommand_t *pParam);                           /* 8.48 */
bleResult_t Hci_LeSetPhy(const hciLeSetPhyCommand_t *pParam);                                         /* 8.49 */

#if defined(gHciCompleteHciCmdSupport_d) && (gHciCompleteHciCmdSupport_d == TRUE)
bleResult_t Hci_LeEnhancedReceiverTest(const hciLeEnhancedReceiverTestCommand_t *pParam);             /* 8.50 */
bleResult_t Hci_LeEnhancedTransmitterTest(const hciLeEnhancedTransmitterTestCommand_t *pParam);       /* 8.51 */
#endif

#if (gBLE50_d == TRUE)
bleResult_t Hci_LeSetAdvSetRandomAddr(hciLeSetAdvSetRandomAddrCommand_t* pParam);
bleResult_t Hci_LeSetExtAdvParams(hciLeSetExtAdvParamsCommand_t* pParam);
bleResult_t Hci_LeSetExtAdvertisingData(hciLeSetExtAdvertisingDataCommand_t* pParam);
bleResult_t Hci_LeSetExtScanRespData(hciLeSetExtScanRespDataCommand_t* pParam);
bleResult_t Hci_LeSetExtAdvEnable(hciLeSetExtAdvEnableCommand_t* pParam);
bleResult_t Hci_LeReadMaxAdvDataLen(void);
bleResult_t Hci_LeReadNumOfSupportedAdvSets(void);
bleResult_t Hci_LeRemoveAdvSet(hciLeRemoveAdvSetCommand_t* pParam);
bleResult_t Hci_LeSetPeriodicAdvParams(hciLeSetPeriodicAdvParamsCommand_t* pParam);
bleResult_t Hci_LeClearAdvSets(void);
bleResult_t Hci_LeSetPeriodicAdvData(hciLeSetPeriodicAdvDataCommand_t* pParam);
bleResult_t Hci_LeSetPeriodicAdvEnable(hciLeSetPeriodicAdvEnableCommand_t* pParam);
bleResult_t Hci_LeSetExtScanParams(hciLeSetExtScanParamsCommand_t* pParam);
bleResult_t Hci_LeSetExtScanEnable(hciLeExtScanEnableCommand_t* pParam);
bleResult_t Hci_LeExtCreateConnection(hciLeExtCreateConnectionCommand_t* pParam);
bleResult_t Hci_LePeriodicAdvCreateSync(hciLePeriodicAdvCreateSyncCommand_t* pParam);
bleResult_t Hci_LePeriodicAdvCreateSyncCancel(void);
bleResult_t Hci_LePeriodicAdvTerminateSync(hciLePeriodicAdvTerminateSyncCommand_t* pParam);
bleResult_t Hci_LeAddDeviceToPeriodicAdvList(hciLeAddDeviceToPeriodicAdvListCommand_t *pParam);
bleResult_t Hci_LeRemoveDeviceFromPeriodicAdvList(hciLeRemoveDeviceFromPeriodicAdvListCommand_t *pParam);
bleResult_t Hci_LeClearPeriodicAdvList(void);
bleResult_t Hci_LeReadPeriodicAdvListSize(void);
#endif

#ifdef __cplusplus
    }
#endif

#endif /* HCI_INTERFACE_H */

/*! *********************************************************************************
* @}
********************************************************************************** */
