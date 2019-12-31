/*! *********************************************************************************
* \addtogroup HCI
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
* All rights reserved.
*
* \file
*
* This file is the interface file for the HCI module
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef HCI_TYPES_H
#define HCI_TYPES_H

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "ble_general.h"
#include "vendor_debug_commands.h"

/************************************************************************************
*************************************************************************************
* Public constants & macros
*************************************************************************************
************************************************************************************/
#if defined(__CC_ARM)
  #pragma anon_unions
#endif

/* Generic constants */
#define gHciLeAdvertisingDataLength_c       (31U)
#define gHciLeScanResponseDataLength_c      (31U)

#if defined(gBLE41_d) && (gBLE41_d == TRUE)
#define gHciNumLeMetaSubEvents_c            (6U)
#elif defined(gBLE42_d) && (gBLE42_d == TRUE)
#define gHciNumLeMetaSubEvents_c            (11U)
#endif
#define gHciNumEvents_c                     (9U)

#if defined(gBLE50_d) && (gBLE50_d == TRUE)
#define gHciLeExtAdvertisingDataLength_c    (251U)
#define gHciLeExtScanRespDataLength_c       (251U)
#define gHciMaxPHYs_c                       (3U)     /* 1M + 2M + Coded */
#define gHciMaxScanningPHYs_c               (2U)     /* 1M + Coded */
#define gHciMaxAdvertisingSets_c            (63U)
#define gHciMaxPeriodicAdvDataLength_c      (252U)
#define gHciMaxAdvReportDataLen_c           (229U)
#define gHciMaxPeriodicAdvReportDataLen_c   (247U) /* ESR11: E8909 - Wrong Maximum */
#endif

/* Hci packets header lengths */
#define gHciCommandPacketHeaderLength_c     (3U)
#define gHciAclDataPacketHeaderLength_c     (4U)
#define gHciEventPacketHeaderLength_c       (2U)

/* Both the Host and the Controller shall support command and event packets, where
the data portion (excluding header) contained in the packets is 500 - header size octets in size. */
#define gHcLeAclDataPacketLengthDefault_c   (500U - gHciAclDataPacketHeaderLength_c)
#define gHcEventPacketLengthDefault_c       (500U - gHciEventPacketHeaderLength_c)

    /*! HCI OGF : 0x01 : Link Control Commands */
#define gHciLinkControlCommands_c                       0x01U
        /*! HCI Commands OCF for OGF : 0x01 : Link Control Commands */
#define gHciDisconnect_c                          0x0006U
#define gHciReadRemoteVersionInformation_c        0x001DU


    /*! HCI OGF : 0x03 : Controller & Baseband Commands */
#define gHciControllerBasebandCommands_c                0x03U
        /*! HCI Commands OCF for OGF : 0x03 : Controller & Baseband Commands */
#define gHciSetEventMask_c                        0x0001U
#define gHciReset_c                               0x0003U
#define gHciReadTransmitPowerLevel_c              0x002DU
#define gHciSetControllerToHostFlowControl_c      0x0031U
#define gHciHostBufferSize_c                      0x0033U
#define gHciHostNumberOfCompletedPackets_c        0x0035U
#if defined(gLePingIsSupported_d) && (gLePingIsSupported_d == TRUE)
#define gHciSetEventMaskPage2_c                   0x0063U
#define gHciReadAuthenticatedPayloadTimeout_c     0x007BU
#define gHciWriteAuthenticatedPayloadTimeout_c    0x007CU
#endif


    /*! HCI OGF : 0x04 : Informational Parameters */
#define gHciInformationalParameters_c                   0x04U
        /*! HCI Commands OCF for OGF : 0x04 : Informational Parameters */
#define gHciReadLocalVersionInformation_c         0x0001U
#define gHciReadLocalSupportedCommands_c          0x0002U
#define gHciReadLocalSupportedFeatures_c          0x0003U
#define gHciReadBufferSize_c                      0x0005U
#define gHciReadBdAddr_c                          0x0009U


    /*! HCI OGF : 0x05 : Status Parameters */
#define gHciStatusParameters_c                          0x05U
        /*! HCI Commands OCF for OGF : 0x05 : Status Parameters */
#define gHciReadRssi_c                           0x0005U

    /*! HCI OGF : 0x08 : LE Controller Commands */
#define gHciLeControllerCommands_c                      0x08U
    /*! HCI Commands OCF for OGF : 0x08 : LE Controller Commands */
#define gHciLeSetEventMask_c                    0x0001U
#define gHciLeReadBufferSize_c                  0x0002U
#define gHciLeReadLocalSupportedFeatures_c      0x0003U
#define gHciLeSetRandomAddress_c                0x0005U
#define gHciLeSetAdvertisingParameters_c        0x0006U
#define gHciLeReadAdvertisingChannelTxPower_c   0x0007U
#define gHciLeSetAdvertisingData_c              0x0008U
#define gHciLeSetScanResponseData_c             0x0009U
#define gHciLeSetAdvertisingEnable_c            0x000AU
#define gHciLeSetScanParameters_c               0x000BU
#define gHciLeSetScanEnable_c                   0x000CU
#define gHciLeCreateConnection_c                0x000DU
#define gHciLeCreateConnectionCancel_c          0x000EU
#define gHciLeReadWhiteListSize_c               0x000FU
#define gHciLeClearWhiteList_c                  0x0010U
#define gHciLeAddDeviceToWhiteList_c            0x0011U
#define gHciLeRemoveDeviceFromWhiteList_c       0x0012U
#define gHciLeConnectionUpdate_c                0x0013U
#define gHciLeSetHostChannelClassification_c    0x0014U
#define gHciLeReadChannelMap_c                  0x0015U
#define gHciLeReadRemoteFeatures_c              0x0016U
#define gHciLeEncrypt_c                         0x0017U
#define gHciLeRand_c                            0x0018U
#define gHciLeStartEncryption_c                 0x0019U
#define gHciLeLongTermKeyRequestReply_c         0x001AU
#define gHciLeLongTermKeyRequestNegativeReply_c 0x001BU
#define gHciLeReadSupportedStates_c             0x001CU
#define gHciLeReceiverTest_c                    0x001DU
#define gHciLeTransmitterTest_c                 0x001EU
#define gHciLeTestEnd_c                         0x001FU

#define gHciLeRemoteConnectionParameterRequestReply_c             0x0020U
#define gHciLeRemoteConnectionParameterRequestNegativeReply_c     0x0021U

#if defined(gBLE42_d) && (gBLE42_d == TRUE)
/* v4.2 specific interface */
#define gHciLeSetDataLength_c                       0x0022U
#define gHciLeReadSuggestedDefaultDataLength_c      0x0023U
#define gHciLeWriteSuggestedDefaultDataLength_c     0x0024U
#define gHciLeReadLocalP256PublicKey_c              0x0025U
#define gHciLeGenerateDhKey_c                       0x0026U
#define gHciLeAddDeviceToResolvingList_c            0x0027U
#define gHciLeRemoveDeviceFromResolvingList_c       0x0028U
#define gHciLeClearResolvingList_c                  0x0029U
#define gHciLeReadResolvingListSize_c               0x002AU
#define gHciLeReadPeerResolvableAddress_c           0x002BU
#define gHciLeReadLocalResolvableAddress_c          0x002CU
#define gHciLeSetAddressResolutionEnable_c          0x002DU
#define gHciLeSetResolvablePrivateAddressTimeout_c  0x002EU
#define gHciLeReadMaximumDataLength_c               0x002FU
#define gHciLeSetPrivacyMode_c                      0x004EU
#endif /* gBLE42_d */

#define gHciLeEnhancedReceiverTest_c                0x0033U
#define gHciLeEnhancedTransmitterTest_c             0x0034U

/* v5.0 specific interface */
#if (defined(gBLE50_d) && (gBLE50_d == TRUE)) || (defined(gBLE42_d) && (gBLE42_d == TRUE))
#define gHciLeReadPhy_c                             0x0030U
#define gHciLeSetDefaultPhy_c                       0x0031U
#define gHciLeSetPhy_c                              0x0032U
#endif
/* Advertising extensions */
#if defined(gBLE50_d) && (gBLE50_d == TRUE)
#define gHciLeSetAdvSetRandomAddr_c                 0x0035U
#define gHciLeSetExtAdvParams_c                     0x0036U
#define gHciLeSetExtAdvertisingData_c               0x0037U
#define gHciLeSetExtScanRespData_c                  0x0038U
#define gHciLeSetExtAdvEnable_c                     0x0039U
#define gHciLeReadMaxAdvDataLen_c                   0x003AU
#define gHciLeReadNumOfSupportedAdvSets_c           0x003BU
#define gHciLeRemoveAdvSet_c                        0x003CU
#define gHciLeClearAdvSets_c                        0x003DU
#define gHciLeSetPeriodicAdvParams_c                0x003EU
#define gHciLeSetPeriodicAdvData_c                  0x003FU
#define gHciLeSetPeriodicAdvEnable_c                0x0040U
#define gHciLeSetExtScanParams_c                    0x0041U
#define gHciLeSetExtScanEnable_c                    0x0042U
#define gHciLeExtCreateConnection_c                 0x0043U
#define gHciLePeriodicAdvCreateSync_c               0x0044U
#define gHciLePeriodicAdvCreateSyncCancel_c         0x0045U
#define gHciLePeriodicAdvTerminateSync_c            0x0046U
#define gHciLeAddDeviceToPeriodicAdvList_c          0x0047U
#define gHciLeRemoveDeviceFromPeriodicAdvList_c     0x0048U
#define gHciLeClearPeriodicAdvList_c                0x0049U
#define gHciLeReadPeriodicAdvListSize_c             0x004AU
#define gHciLeReadTransmitPower_c                   0x004BU
#define gHciLeReadRfPathCompensation_c              0x004CU
#define gHciLeWriteRfPathCompensation_c             0x004DU
#endif /* gBLE50_d */

     /*! HCI OGF : 0x3F : Vendor Specific Debug Commands */
#define gHciVendorSpecificDebugCommands_c           0x03FU
#define gHciSetTxPowerCommand_c                     0x012DU
#define gHciVendorEnhancedNotificationCommand_c     0x01F0U
        /*! HCI Commands OCF for OGF : 0x3F : Vendor Specific Debug Commands */
        /* Will be inherited from "vendor_debug_commands.h" */


    /* Macros used to build full HCI OpCodes */
#define HciCmdOpcode(cmdOgf, cmdOcf)        (((uint16_t)(cmdOgf) << SHIFT10) | (cmdOcf))
#define HciLeCmdOpcode(leCmdOcf)            (HciCmdOpcode(gHciLeControllerCommands_c, leCmdOcf))
#define HciInfoCmdOpcode(infoCmdOcf)        (HciCmdOpcode(gHciInformationalParameters_c, infoCmdOcf))
#define HciControllerCmdOpcode(ctrlCmdOcf)  (HciCmdOpcode(gHciControllerBasebandCommands_c, ctrlCmdOcf))
#define HciStatusCmdOpcode(statusCmdOcf)    (HciCmdOpcode(gHciStatusParameters_c, statusCmdOcf))

#define BIT(x) (1 << (x))

#define gHciAddrTypePrivacyOffset_c      (0x02U)
#define gHciFilterPolicyPrivacyOffset_c  (0x02U)

#if defined(gBLE50_d) && (gBLE50_d == TRUE)
#define gHciAdvReportTypeMask_c     (0x1FU)
#define gHciAdvReportTypeShift_c    (0x00U)
#define gHciAdvReportStatusMask_c   (0x60U)
#define gHciAdvReportStatusShift_c  (0x05U)
#endif

/*! Bluetooth HCI Device Address Types */
#define gHciBleAddrTypePublic_c         (0x00U) /*!< Public Device Address - fixed into the Controller by the manufacturer. */
#define gHciBleAddrTypeRandom_c         (0x01U) /*!< Random Device Address - set by the Host into the Controller for privacy reasons. */
#if defined(gBLE42_d) && (gBLE42_d == TRUE)
#define gHciBleAddrTypePublicIdentity_c (0x02U) /*!< Public Identity Address - corresponds to Resolvable Private Address. */
#define gHciBleAddrTypeRandomIdentity_c (0x03U) /*!< Random (static) Identity Address - corresponds to Resolvable Private Address. */
#endif
#if defined(gBLE50_d) && (gBLE50_d == TRUE)
#define gHciBleAddrTypeNone_c           (0xFFU) /*!< No address provided (anonymous advertisement) */
#endif

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/
typedef enum
{
    gHciLeScanDisable_c   = 0x00,
    gHciLeScanEnable_c    = 0x01,
} hciLeScanEnable_t;

typedef enum
{
    gHciLeDuplicateFilteringDisable_c    = 0x00,
    gHciLeDuplicateFilteringEnable_c     = 0x01,
#if defined(gBLE50_d) && (gBLE50_d == TRUE)
    gHciLeDuplicateFilteringPeriodicEnable_c = 0x02,
#endif
} hciLeFilterDuplicates_t;

typedef enum
{
    gLinkLayerEncryptionOff_c    = 0x00,
    gLinkLayerEncryptionOn_c     = 0x01,
} hciLlEncryptionEnabled_t;

typedef union
{
    struct
    {
        uint16_t    ocf:        10;
        uint16_t    ogf:        6;
    };
    uint16_t    opCode;
} hciCommandOpCode_t;

typedef uint8_t hciEventCode_t;
typedef enum
{
    gHciDisconnectionCompleteEvent_c                    = 0x05,
    gHciEncryptionChangeEvent_c                         = 0x08,
    gHciReadRemoteVersionInformationCompleteEvent_c     = 0x0C,
    gHciCommandCompleteEvent_c                          = 0x0E,
    gHciCommandStatusEvent_c                            = 0x0F,
    gHciHardwareErrorEvent_c                            = 0x10,
    gHciNumberOfCompletedPacketsEvent_c                 = 0x13,
    gHciDataBufferOverflowEvent_c                       = 0x1A,
    gHciEncryptionKeyRefreshCompleteEvent_c             = 0x30,
    gHciLeMetaEvent_c                                   = 0x3E,
#if defined(gBLE41_d) && (gBLE41_d == TRUE)
    gHciAuthenticatedPayloadTimeoutExpiredEvent_c       = 0x57,
#endif
} hciEventCode_tag;

typedef uint8_t hciLeMetaSubEventCode_t;
typedef enum
{
    gHciLeConnectionCompleteEvent_c                 = 0x01,
    gHciLeAdvertisingReportEvent_c                  = 0x02,
    gHciLeConnectionUpdateCompleteEvent_c           = 0x03,
    gHciLeReadRemoteFeaturesCompleteEvent_c         = 0x04,
    gHciLeLongTermKeyRequestEvent_c                 = 0x05,
    /* BLE v4.1 */
    gHciLeRemoteConnectionParameterRequestEvent_c   = 0x06,
    /* BLE v4.2 */
    gHciLeDataLengthChangeEvent_c                   = 0x07,
    gHciLeReadLocalP256PublicKeyCompleteEvent_c     = 0x08,
    gHciLeGenerateDhKeyCompleteEvent_c              = 0x09,
    gHciLeEnhancedConnectionCompleteEvent_c         = 0x0A,
    gHciLeDirectedAdvertisingReportEvent_c          = 0x0B,
    /* BLE v5.0 */
    gHciLePhyUpdateCompleteEvent_c                  = 0x0C,
    gHciLeExtAdvReportEvent_c                       = 0x0D,
    gHciLePeriodicAdvSyncEstbEvent_c                = 0x0E,
    gHciLePeriodicAdvReportEvent_c                  = 0x0F,
    gHciLePeriodicAdvSyncLostEvent_c                = 0x10,
    gHciLeScanTimeoutEvent_c                        = 0x11,
    gHciLeAdvSetTerminatedEvent_c                   = 0x12,
    gHciLeScanReqReceivedEvent_c                    = 0x13,
    gHciLeChannelSelAlgorithmEvent_c                = 0x14,
    gHciLeVendorEnhancedNotificationEvent_c         = 0xFF,
} hciLeMetaSubEventCode_tag;

typedef uint32_t hciLeMetaSubEventMask_t;
typedef enum
{
    gHciLeConnectionCompleteEventMask_c                 = BIT(0 * 8 + 0),
    gHciLeAdvertisingReportEventMask_c                  = BIT(0 * 8 + 1),
    gHciLeConnectionUpdateCompleteEventMask_c           = BIT(0 * 8 + 2),
    gHciLeReadRemoteFeaturesCompleteEventMask_c         = BIT(0 * 8 + 3),
    gHciLeLongTermKeyRequestEventMask_c                 = BIT(0 * 8 + 4),
    /* BLE v4.1 */
    gHciLeRemoteConnectionParameterRequestEventMask_c   = BIT(0 * 8 + 5),
    /* BLE v4.2 */
    gHciLeDataLengthChangeEventMask_c                   = BIT(0 * 8 + 6),
    gHciLeReadLocalP256PublicKeyCompleteEventMask_c     = BIT(0 * 8 + 7),
    gHciLeGenerateDhKeyCompleteEventMask_c              = BIT(1 * 8 + 0),
    gHciLeEnhancedConnectionCompleteEventMask_c         = BIT(1 * 8 + 1),
    gHciLeDirectedAdvertisingReportEventMask_c          = BIT(1 * 8 + 2),
    /* BLE v5.0 */
    gHciLePhyUpdateCompleteEventMask_c                  = BIT(1 * 8 + 3),
    gHciLeExtAdvReportEventMask_c                       = BIT(1 * 8 + 4),
    gHciLePeriodicAdvSyncEstbEventMask_c                = BIT(1 * 8 + 5),
    gHciLePeriodicAdvReportEventMask_c                  = BIT(1 * 8 + 6),
    gHciLePeriodicAdvSyncLostEventMask_c                = BIT(1 * 8 + 7),
    gHciLeScanTimeoutEventMask_c                        = BIT(2 * 8 + 0),
    gHciLeAdvSetTerminatedEventMask_c                   = BIT(2 * 8 + 1),
    gHciLeScanReqReceivedEventMask_c                    = BIT(2 * 8 + 2),
    gHciLeChannelSelAlgorithmEventMask_c                = BIT(2 * 8 + 3),
} hciLeMetaSubEventMask_tag;

typedef uint32_t hciLeSupportedFeatures_t;
typedef enum
{
    gHciLeEncryption_c                            = BIT((0 * 8) + 0), /* 0 */
    gHciConnectionParametersRequestProcedure_c    = BIT((0 * 8) + 1), /* 1 */
    gHciExtendedRejectIndication_c                = BIT((0 * 8) + 2), /* 2 */
    gHciSlaveInitiatedFeaturesExchange_c          = BIT((0 * 8) + 3), /* 3 */
    gHciLePing_c                                  = BIT((0 * 8) + 4), /* 4 */
#if defined(gBLE42_d) && (gBLE42_d == TRUE)
    gHciLeDataPacketLengthExtension_c             = BIT((0 * 8) + 5), /* 5 */
    gHciLlPrivacy_c                               = BIT((0 * 8) + 6), /* 6 */
    gHciExtendedScannerFilterPolicies_c           = BIT((0 * 8) + 7), /* 7 */
#endif
} hciLeSupportedFeatures_tag;

typedef uint8_t hciLeAdvertisingEnable_t;
typedef enum
{
    gHciLeAdvertisingDisable_c   = 0x00,
    gHciLeAdvertisingEnable_c    = 0x01,
} hciLeAdvertisingEnable_tag;

typedef uint8_t hciControllerToHostFlowControlEnable_t;
typedef enum
{
    gHciFlowControlOffAclPacketsOffHciSyncPackets_c  = 0x00,
    gHciFlowControlOnAclPacketsOffHciSyncPackets_c   = 0x01,
    gHciFlowControlOffAclPacketsOnHciSyncPackets_c   = 0x02,
    gHciFlowControlOnAclPacketsOnHciSyncPackets_c    = 0x03,
} hciControllerToHostFlowControlEnable_tag;

typedef uint8_t hciDataBufferOverflowLinkType_t;
typedef enum
{
    gHciDataBufferOverflowSynchronous_c  = 0x00,
    gHciDataBufferOverflowACL_c          = 0x01,
} hciDataBufferOverflowLinkType_tag;

/*! Bluetooth HCI Device Address Type - Size: 1 Octet
    Range: [gHciBleAddrTypePublic_c:gHciBleAddrTypeRandomIdentity_c, gHciBleAddrTypeNone_c] */
typedef uint8_t hciBleAddressType_t;

typedef enum {
    gHciScanAll_c              =  0x00,    /*!< Scans all advertising packets. */
    gHciScanWithWhiteList_c    =  0x01,    /*!< Scans advertising packets using the White List. */
#if defined(gBLE42_d) && (gBLE42_d == TRUE)
    gHciScanAllRpa_c           =  0x02,
    gHciScanWithWhiteListRpa_c =  0x03
#endif
} hciBleScanningFilterPolicy_t;

#if defined(gBLE50_d) && (gBLE50_d == TRUE)
typedef uint16_t hciBleExtAdvReportEventType_t;
typedef enum
{
    gHciAdvReportConnectableAdv_c = 0x01,
    gHciAdvReportScannableAdv_c   = 0x02,
    gHciAdvReportDirectedAdv_c    = 0x04,
    gHciAdvReportScanResp_c       = 0x08,
    gHciAdvReportLegacyAdv_c      = 0x10,
} hciBleExtAdvReportEventType_tag;

typedef uint8_t hciBleExtAdvReportStatus_t;
typedef enum
{
    gHciAdvReportComplete_c   = 0x00,  /*!< Report complete */
    gHciAdvReportMoreToCome_c = 0x01,  /*!< Report incomplete, more data to come */
    gHciAdvReportTruncated_c  = 0x02,  /*!< Report incomplete, data truncated, no more to come */
} hciBleExtAdvReportStatus_tag;

typedef uint8_t hciBleExtDataOperation_t;
typedef enum
{
    gHciExtDataIntermediateFrag_c = 0x00, /*!< Intermediate fragment of extended advertising or scan response data. */
    gHciExtDataFirstFrag_c        = 0x01, /*!< First fragment of extended advertising or scan response data. */
    gHciExtDataLastFrag_c         = 0x02, /*!< Last fragment of extended advertising or scan response data. */
    gHciExtDataComplete_c         = 0x03  /*!< Complete extended advertising or scan response data. No fragmeentation. */
} hciBleExtDataOperation_tag;

typedef uint8_t hciBleFragmentPreference_t;
typedef enum
{
    gHciUseFragmentation_c = 0x00,
    gHciNoFragmentation_c  = 0x01
} hciBleFragmentPreference_tag;
#endif

typedef PACKED_STRUCT
{
    uint16_t    opCode;
    uint8_t     parameterTotalLength;
    uint8_t     aData[1];
}hciCommandPacket_t;

typedef PACKED_STRUCT
{
    uint16_t    handle      :12;
    uint16_t    pbFlag      :2;
    uint16_t    bcFlag      :2;
    uint16_t    dataTotalLength;
    uint8_t     aData[1];
}hciAclDataPacket_t;

typedef PACKED_STRUCT
{
    hciEventCode_t  eventCode;
    uint8_t     dataTotalLength;
    uint8_t     aData[0];
}hciEventPacket_t;

typedef bleResult_t hciErrorCode_t;

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

#endif /* HCI_TYPES_H */

/*! *********************************************************************************
* @}
********************************************************************************** */
