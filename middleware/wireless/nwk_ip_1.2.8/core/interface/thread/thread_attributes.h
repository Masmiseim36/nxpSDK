/*! *********************************************************************************
 * \defgroup TATTR  Thread Attributes Interface
 * @{
 ***********************************************************************************/
/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _THREAD_ATTRIBUTES_H
#define _THREAD_ATTRIBUTES_H

/*!
\file       thread_attributes.h
\brief      This is a header file for the Thread Attributes module.
*/

/*==================================================================================================
Include Files
==================================================================================================*/
/* General Includes */
#include "EmbeddedTypes.h"
#include "stack_config.h"

#include "thread_types.h"

/*==================================================================================================
Public macros
==================================================================================================*/
#ifndef THR_CHILD_BITS_SIZE
    #define THR_CHILD_BITS_SIZE                             0x09
#endif

/* Thread border router prefix flags */
#define THR_BR_PREFIX_FLAGS_P_PREFERENCE_MASK           0xC0
#define THR_BR_PREFIX_FLAGS_P_PREFERRED_MASK            0x20
#define THR_BR_PREFIX_FLAGS_P_SLAAC_MASK                0x10
#define THR_BR_PREFIX_FLAGS_P_DHCP_MASK                 0x08
#define THR_BR_PREFIX_FLAGS_P_CONFIGURE_MASK            0x04
#define THR_BR_PREFIX_FLAGS_P_DEFAULT_MASK              0x02
#define THR_BR_PREFIX_FLAGS_P_ON_MESH_MASK              0x01
#define THR_BR_PREFIX_FLAGS_P_ND_DNS_MASK               0x80  // MSB of Reserved
#define THR_BR_PREFIX_FLAGS_P_DP_MASK                   0x40

#define THR_BR_PREFIX_FLAGS_P_PREFERENCE_OFFSET         0x06
#define THR_BR_PREFIX_FLAGS_P_PREFERRED_OFFSET          0x05
#define THR_BR_PREFIX_FLAGS_P_SLAAC_OFFSET              0x04
#define THR_BR_PREFIX_FLAGS_P_DHCP_OFFSET               0x03
#define THR_BR_PREFIX_FLAGS_P_CONFIGURE_OFFSET          0x02
#define THR_BR_PREFIX_FLAGS_P_DEFAULT_OFFSET            0x01
#define THR_BR_PREFIX_FLAGS_P_ON_MESH_OFFSET            0x00
#define THR_BR_PREFIX_FLAGS_P_ND_DNS_OFFSET             0x07  // MSB of Reserved
#define THR_BR_PREFIX_FLAGS_P_DP_OFFSET                 0x06

/* thread border router external route flags */
#define THR_BR_EXT_ROUTE_FLAGS_R_PREF_MASK              0x03
#define THR_BR_EXT_ROUTE_FLAGS_R_PREF_OFFSET            0x00

/* thread border router prefix set/get */
#define THR_BR_FLAGS_SET(flag, value, mask, offset)     ((flag)=(((flag)&(~mask))|((value)<<offset)))
#define THR_BR_FLAGS_GET(value, mask, offset)           (((value)&mask)>>offset)

/* P_Preference/R_Preference values */
#define THR_FLAGS_PREFERENCE_MEDIUM                     0x00
#define THR_FLAGS_PREFERENCE_HIGH                       0x01
#define THR_FLAGS_PREFERENCE_RESERVED                   0x02
#define THR_FLAGS_PREFERENCE_LOW                        0x03

#define gNwkAttrId_Undefined_c                          THR_ALL_FFs8

/* easy access to the thread attributes */
#define THR_GetAttr_Channel(thrInstId)                  gpaThrActiveAttr[thrInstId]->channel
#define THR_GetAttr_PanId(thrInstId)                    gpaThrActiveAttr[thrInstId]->panId
#define THR_GetAttr_XPanId(thrInstId, pXpan)            (void)THR_GetAttr(thrInstId, gNwkAttrId_ExtendedPanId_c, 0, NULL, pXpan)
#define THR_GetAttr_PendingActiveTimestamp(thrInstId, pTimestamp) (void)THR_GetAttr(thrInstId, gNwkAttrId_MeshCop_PendingActiveTimestamp_c, 0, NULL, pTimestamp);
#define THR_GetAttr_ShortAddr(thrInstId)                gpaThrAttr[thrInstId]->shortAddr
#define THR_GetAttr_IsDevConnected(thrInstId)           gpaThrAttr[thrInstId]->devIsConnected
#define THR_GetAttr_PartionId(thrInstId)                gpaThrAttr[thrInstId]->partitionId
#define THR_GetAttr_NwkCapabilities(thrInstId)          gpaThrAttr[thrInstId]->thrNwkCapabilitiesBitMap
#define THR_GetAttr_AttachMode(thrInstId)               gpaThrAttr[thrInstId]->attachMode
#define THR_GetAttr_IsDevCommissioned(thrInstId)        gpaThrAttr[thrInstId]->devIsCommissioned
#define THR_GetAttr_DeviceRole(thrInstId)               gpaThrAttr[thrInstId]->devRole
#define THR_GetAttr_DeviceType(thrInstId)               gpaThrAttr[thrInstId]->deviceType
#define THR_GetAttr_CommissionerMode(thrInstId)         gpaThrAttr[thrInstId]->commissionerMode
#define THR_GetAttr_SelBestChEDThreshold(thrInstId)     gpaThrAttr[thrInstId]->selBestChannelEDThreshold
#define THR_GetAttr_JoinLqiThreshold(thrInstId)         gpaThrAttr[thrInstId]->joinLqiThreshold
#define THR_GetAttr_PermitJoin(thrInstId)               gpaThrAttr[thrInstId]->permitJoin
#define THR_GetAttr_BrDefaultRoute(thrInstId)           gpaThrAttr[thrInstId]->brDefaultRoute
#define THR_GetAttr_BrExternalIfPrefix(thrInstId, ptr)    *ptr = gpaThrAttr[thrInstId]->brExternalIfPrefix
#define THR_GetAttr_BrGlobalOnMeshPrefix(thrInstId, ptr)  *ptr = gpaThrAttr[thrInstId]->brGlobalOnMeshPrefix
#define THR_GetAttr_ScanChannelMask(thrInstId)          gpaThrActiveAttr[thrInstId]->scanChannelMask
#define THR_GetAttr_SedFastPollInterval(thrInstId)      gpaThrAttr[thrInstId]->sedFastPollInterval
#define THR_GetAttr_SedPollInterval(thrInstId)          gpaThrAttr[thrInstId]->sedPollInterval
#define THR_GetAttr_SedFastPoll(thrInstId)              gpaThrAttr[thrInstId]->isFastPollEnabled
#define THR_GetAttr_DoNotGeneratePartId(thrInstId)      gpaThrAttr[thrInstId]->doNotGeneratePartitionId
#define THR_GetAttr_NwkKeySeq(thrInstId)                gpaThrAttr[thrInstId]->nwkKeySeq
#define THR_GetAttr_NvmRestore(thrInstId)               gpaThrAttr[thrInstId]->nvmRestore
#define THR_GetAttr_NvmRestoreData(thrInstId)           gpaThrAttr[thrInstId]->nvmRestoreData
#define THR_GetAttr_NvmAutostart(thrInstId)             gpaThrAttr[thrInstId]->nvmRestoreAutoStart
#define THR_GetAttr_ChildAddrMask(thrInstId)            gpaThrAttr[thrInstId]->childAddrMask
#define THR_GetAttr_ScanDuration(thrInstId)             gpaThrAttr[thrInstId]->scanDuration
#define THR_GetAttr_DiscReqMacTxOptions(thrInstId)      gpaThrAttr[thrInstId]->discoveryReqMacTxOptions
#define THR_GetAttr_RandExtAddr(thrInstId)              gpaThrAttr[thrInstId]->randExtAddr
#define THR_GetAttr_ShaHashAddr(thrInstId)              gpaThrAttr[thrInstId]->shaHashAddr
#define THR_GetAttr_RxOnWhenIdle(thrInstId)             gpaThrAttr[thrInstId]->rxOnWhenIdle
#define THR_GetAttr_Weighting(thrInstId)                gpaThrAttr[thrInstId]->weighting
#define THR_GetAttr_ChildReqFullNwkData(thrInstId)      gpaThrAttr[thrInstId]->childEDReqFullNwkData
#define THR_GetAttr_ParentHoldTime(thrInstId)           gpaThrAttr[thrInstId]->parentHoldTime
#define THR_GetAttr_KeySwitchGuardTime(thrInstId)       gpaThrAttr[thrInstId]->keySwitchGuardTime
#define THR_GetAttr_JoinerUdpPort(thrInstId)            gpaThrAttr[thrInstId]->joinerUdpPort
#define THR_GetAttr_MinDelayTimer(thrInstId)            gpaThrAttr[thrInstId]->minDelayTimer
#define THR_GetAttr_CommissionerUpdPort(thrInstId)      gpaThrAttr[thrInstId]->commissionerUdpPort
#define THR_GetAttr_SedTimeoutPeriod(thrInstId)         gpaThrAttr[thrInstId]->sedTimeoutPeriod
#define THR_GetAttr_EdTimeoutPeriod(thrInstId)          gpaThrAttr[thrInstId]->edTimeoutPeriod
#define THR_GetAttr_SlaacPolicy(thrInstId)              gpaThrAttr[thrInstId]->slaacPolicy
/*==================================================================================================
Public type definitions
==================================================================================================*/

/*!
    Thread network information base (Thread Nibs) Ids:
    - 802.15.4 attributes
    - thread specific attributes
*/
typedef enum
{
    gNwkAttrId_RandExtAddr_c = 0x00,                    /*!< Random extended address */
    gNwkAttrId_ShortAddr_c = 0x01,                      /*!< Short address */
    gNwkAttrId_ScanChannelMask_c = 0x02,                /*!< Scan channel mask */
    gNwkAttrId_ScanDuration_c = 0x03,                   /*!< Scan duration */
    gNwkAttrId_Channel_c = 0x04,                        /*!< Channel */
    gNwkAttrId_PanId_c = 0x05,                          /*!< Pan id */
    gNwkAttrId_ExtendedPanId_c = 0x06,                  /*!< Extended pan id */
    gNwkAttrId_PermitJoin_c = 0x07,                     /*!< Permit join */
    gNwkAttrId_RxOnIdle_c = 0x08,                       /*!< Rx on when idle */
    gNwkAttrId_SEDPollInterval_c = 0x09,                /*!< Poll interval */
    gNwkAttrId_UniqueExtAddr_c = 0x0A,                  /*!< Unique extended address (test only) */

    gNwkAttrId_VendorName_c = 0x0B,                     /*!< Vendor name */
    gNwkAttrId_ModelName_c = 0x0C,                      /*!< Model name */
    gNwkAttrId_SWVersion_c = 0x0D,                      /*!< Software version */
    gNwkAttrId_StackVersion_c = 0x0E,                   /*!< Stack version */

    gNwkAttrId_ThreadNwkCapabilites_c = 0x0F,           /*!< Nwk capabilities */
    gNwkAttrId_NwkName_c = 0x10,                        /*!< Nwk name */
    gNwkAttrId_DeviceType_c = 0x11,                     /*!< Device type */
    gNwkAttrId_IsDevConnected_c = 0x12,                 /*!< Is device connected */
    gNwkAttrId_IsDevCommissioned_c = 0x13,              /*!< Is device out-of band preconfigured */
    gNwkAttrId_PartitionId_c = 0x14,                    /*!< Partition id of the network */
    gNwkAttrId_DeviceRole_c = 0x15,                     /*!< Device role */
    gNwkAttrId_NwkMasterKey_c = 0x16,                   /*!< Nwk master key */
    gNwkAttrId_NwkKeySeq_c = 0x17,                      /*!< Nwk key sequence */
    gNwkAttrId_PSKc_c = 0x18,                           /*!< Network credential*/
    gNwkAttrId_PSKd_c = 0x19,                           /*!< Device password */

    gNwkAttrId_VendorData_c = 0x1A,                     /*!< Vendor data */
    gNwkAttrId_EDTimeoutPeriod_c = 0x1B,                /*!< The timeout period included in the Child ID Request sent
                                                            to the parent. */
    gNwkAttrId_MLPrefix_c = 0x1C,                       /*!< Mesh local prefix*/
    gNwkAttrId_WhiteListEntry_c = 0x1D,                 /*!< White list entry */

    gNwkAttrId_ThreadAttachMode_c = 0x1E,               /*!< Initial attach mode of a Thread child */
    //0x1F,
    gNwkAttrId_KeyRotationInterval_c = 0x20,            /*!< Key rotation interval */
    gNwkAttrId_ChildAddrMask_c = 0x21,                  /*!< Child address mask */
    gNwkAttrId_SEDTimeoutPeriod_c = 0x22,               /*!< The timeout period included in the Child ID Request sent
                                                            to the parent. */
    gNwkAttrId_ChildEDReqFullNwkData_c = 0x23,          /*!< If it is set TRUE The child End device should request the Full network data */
    gNwkAttrId_IsFastPollEnabled_c = 0x24,              /*!< Is fast poll enabled */
    gNwkAttrId_SEDFastPollInterval_c = 0x25,            /*!< Fast poll interval */
    gNwkAttrId_JoinLqiThreshold_c = 0x26,               /*!< Join lqi threshold */
    gNwkAttrId_ProvisioningURL_c = 0x27,                /*!< A URL for the Joiner to communicate to the user which Commissioning application is best to use
                                                            to properly provision it to the appropriate service*/

    gNwkAttrId_SelectBestChEDThreshold_c = 0x28,        /*!< The energy channel threshold to select the best channel when
                                                            more channels are scan to form the network  */
    //0x29
    //0x30
    gNwkAttrId_SteeringData_c = 0x31,                   /*!< Steering data  */
    gNwkAttrId_NvmRestoreData_c = 0x32,                 /*!< Device has data that can be restored from NVM */
    gNwkAttrId_KeySwitchGuardTime_c = 0x33,             /*!< The thread Key switch guard time to prevent inadvertent key switching */
    gNwkAttrId_ParentHoldTime_c = 0x34,                 /*!< The hold time period in seconds used by the parent to
                                                            hold the packets for SED devices */
    gNwkAttrId_SecurityPolicy_c = 0x35,                 /*!< O and N bits without the rotation time */
    gNwkAttrId_NvmRestoreAutoStart_c = 0x36,            /*!< Stack starts automatically with NVM restore after reset*/
    gNwkAttrId_NvmRestore_c = 0x37,                     /*!< Restore from NVM */
    gNwkAttrId_SlaacPolicy_c = 0x38,                    /*!< Specifies the policy for generating the IID
                                                            of an address configured using SLAAC */

    gNwkAttrId_IeeeAddr_c = 0x39,                       /*!< MAC IEE Extended address used for SHA256
                                                            address generation */

    gNwkAttrId_LeaderWeight_c = 0x3A,                   /*!< Leader Weight, an 8-bit unsigned integer */

    gNwkAttrId_HashIeeeAddr_c = 0x40,                   /*!< SHA256 generated MAC address used during
                                                            commissioning phase */
    gNwkAttrId_DoNotGeneratePartitionId_c = 0x41,       /*!< Avoid random generation of partition ID */
    gNwkAttrId_BrGlobalOnMeshPrefix_c = 0x50,           /*!< Global /64 on-Mesh Prefix on Border Router */
    gNwkAttrId_BrDefaultRouteOnMeshPrefix_c = 0x51,     /*!< Default Route of the /64 on-mesh prefix */
    gNwkAttrId_BrExternalIfPrefix_c = 0x52,             /*!< Global /64 external interface prefix  */
    gNwkAttrId_MeshCop_ActiveTimestamp_c = 0x60,                /*!< Active timestamp */
    gNwkAttrId_MeshCop_PendingChannel_c = 0x61,                 /*!< Pending channel */
    gNwkAttrId_MeshCop_PendingChannelMask_c = 0x62,             /*!< Pending Channel Mask */
    gNwkAttrId_MeshCop_PendingXpanId_c = 0x63,                  /*!< Pending Extended PanId */
    gNwkAttrId_MeshCop_PendingMLprefix_c = 0x64,                /*!< Pending MeshLocal prefix */
    gNwkAttrId_MeshCop_PendingNwkMasterKey_c = 0x65,            /*!< Pending Master Key */
    gNwkAttrId_MeshCop_PendingNwkName_c = 0x66,                 /*!< Pending Network Name */
    gNwkAttrId_MeshCop_PendingPanId_c = 0x67,                   /*!< Pending Pan ID */
    gNwkAttrId_MeshCop_PendingPSK_c = 0x68,                     /*!< Pending PSKc */
    gNwkAttrId_MeshCop_PendingSecurityPolicy = 0x69,            /*!< Pending Security Policy bits */
    gNwkAttrId_MeshCop_PendingNwkKeyRotationInterval_c = 0x6A,  /*!< Pending Key Rotation Interval[sec] */
    gNwkAttrId_MeshCop_PendingDelayTimer_c = 0x6B,              /*!< Pending Delay Timer[msec] */
    gNwkAttrId_MeshCop_PendingActiveTimestamp_c = 0x6C,         /*!< Pending Active Timestamp */
    gNwkAttrId_MeshCop_PendingTimestamp_c = 0x6D,               /*!< Pending Timestamp */
    gNwkAttrId_MeshCop_CommissionerId_c = 0x6E,                 /*!< Commissioner string */

    gNwkAttr_JoinerUdpPort_c = 0x6F,                            /*!< Joiner UDP port */
    gNwkAttr_CommissionerUdpPort_c = 0x70,                      /*!< Commissioner UDP port */
    gNwkAttr_DiscoveryReqMacTxOptions_c = 0x71,                 /*!< The default discovery request Mac Tx options (see thrDiscReqTxOptions_t) */
    gNwkAttr_MinDelayTime = 0x72,                               /*!< The minimum accepted time before a Pending Dataset can be installed[s] */

} thrAttrId_t;

/*!
    Thread network information base (Thread Nibs) structure:
    - 802.15.4 attributes
    - thread specific attributes
*/
typedef struct thrAttr_tag
{
    /* 802.15.4 attributes */
    /* ------------------- */
    uint64_t ieeeAddr;                          /*!< The MAC extended address, also called IEEE address,
                                                   long address or 64-bit MAC address */
    uint64_t shaHashAddr;                       /*!< MAC address used for commissioning generated
                                                   from SHA256 hash on the ieeeAddr */

    uint64_t  randExtAddr;                      /*!< The random MAC extended address, used for
                                                   communication inside the network after commissioning */
    /* ------------------- */
    uint8_t   scanDuration;                     /*!< The scan duration time. This is an exponential scale, as seen in the 802.15.4 specification.
                                                   Range: 0 - 14. Values greater than 14 will be set to 14, as described in Thread Specification chapter 8.10.2.10.
                                                   The user can take into account that the total scanning time also depends on the number of channels scanned.
                                                   Ex.: for a scanmask of 16 channels, maximum value of scanDuration is 8, meaning 3.75 seconds/channel. */
    /* ------------------- */
    uint16_t  shortAddr;                        /*!< The short address.*/
    bool_t    permitJoin;                       /*!< Permit Join(Router devices only).
                                                   True = Device is allowing the child to join the network,
                                                   False = Device is not allowing any child to join the network */
    bool_t    rxOnWhenIdle;                     /*!< The receiver is ON when the device is in idle state.
                                                   Set RxOnWhenIdle TRUE for mains-powered (non-sleeping) end-devices. Set this
                                                   FALSE for sleeping end-devices. When FALSE, end-devices will poll their
                                                   parent for messages. See sedPollInterval for the polling timeout.*/
    uint32_t  sedPollInterval;                  /*!< The poll interval in milliseconds. This attribute is used only for sleepy end devices */
    /* ------------------- */
    /* Thread specific attributes: */
    /* ------------------- */
    uint32_t  sedFastPollInterval;              /*!< The fast poll interval in milliseconds. This attribute is used only for sleepy end devices
                                                   during the joining procedure*/
    bool_t    isFastPollEnabled;                /*!< Specify if the fast polling is enabled */
    bool_t    uniqueExtAddr;                    /*!< If is set to TRUE, the device is automatically generated a random extended address */
    bool_t    devIsConnected;                   /*!< Specifies if the device is connected or not */
    bool_t    devIsCommissioned;                /*!< If TRUE than the device is commissioned.
                                                   - On joining calling THR_NwkJoin() with devIsCommissioned == TRUE, the device will perform the attaching
                                                   procedure using the commissioning settings. Note that a network active scan is performed before attaching.
                                                   - On joining calling THR_NwkJoin() with devIsCommissioned == FALSE, the device will perform the
                                                   joining with Commissioner procedure (mesh-cop joining).*/
    /* ------------------- */
    uint32_t  sedTimeoutPeriod;                 /*!< The Timeout period used by the parent to consider a sleepy end device (SED) disconnected. */
    uint32_t  edTimeoutPeriod;                  /*!< The Timeout period used by the parent to consider an end device (ED) disconnected. */
    bool_t    childEDReqFullNwkData;            /*!< If it is set TRUE, the end device is requesting the full network data(stable and temporary nwk data).
                                                   If it is set FALSE, the end device is requesting only the unstable (temporary) network data. */
    thrDeviceType_t   deviceType;               /*!< The device type: 0x00 -EndNode, 0x01 - Combo Node  */
    thrInternalDeviceRole_t   devRole;          /*!< The device role: 0x01 - SED, 0x02 - MED, 0x03 - FED,0x04 - REED, 0x05 - Router, 0x06 - Leader */
    uint8_t   thrNwkCapabilitiesBitMap;         /*!< A bitmap that specify network capabilities of this device */
    uint8_t   attachMode;                       /*!< A bitmap that keeps the initial attach mode of a Thread child */
    /* ------------------- */
    uint32_t  nwkKeySeq;                        /*!< The current network key sequence number */
    /* ------------------- */
#if !THREAD_ED_CONFIG
    uint32_t  childAddrMask[THR_MAX_CHILD_IDS]; /*!< The child address mask */
#endif
    /* ------------------- */
    uint32_t  partitionId;                      /*!< The current partition identifier */
    uint8_t   weighting;                        /*!< Leader weight */
    bool_t    doNotGeneratePartitionId;         /*!< Avoid random generation of partition ID */
    uint8_t   joinLqiThreshold;                 /*!< The joining LQI threshold used to select a potential parent. */
    uint8_t   selBestChannelEDThreshold;        /*!< The energy channel threshold to select the best channel when
                                                     more channels are scan (energy detect scan) to form the network.
                                                     Note that this is used only if the scanChannelMask attribute includes more than one channel.   */
    uint16_t   joinerUdpPort;                   /*!< Joiner UDP port */
    uint16_t   commissionerUdpPort;             /*!< Commissioner UDP port */
    uint32_t   keySwitchGuardTime;              /*!< The thread Key switch guard time to prevent inadvertent key switching(Hours) */
    uint16_t   parentHoldTime;                  /*!< Thread hold time in seconds used by the parent to
                                                      hold the packets for SED devices */
    uint8_t    slaacPolicy;                     /*!< Used SLAAC policy (see thrSlaacPolicy_t)*/

    /* ------------------- */
    bool_t     nvmRestoreAutoStart;             /*!< Stack starts automatically with NVM restore after reset*/
    bool_t     nvmRestore;                      /*!< Restore from NVM */
    bool_t     nvmRestoreData;                  /*!< Device has data that can be restored from NVM or FALSE if device is blank */
    thrPrefixAttr_t brGlobalOnMeshPrefix;       /*!< Global /64 on-Mesh Prefix on Border Router */
    bool_t     brDefaultRoute;                  /*!< Default Route of the /64 on-mesh prefix */
    thrPrefixAttr_t brExternalIfPrefix;         /*!< Global /64 external interface prefix  */
    uint8_t    discoveryReqMacTxOptions;        /*!< The default discovery request Mac Tx options (see thrDiscReqTxOptions_t) */
    /* ------------------- */

    uint32_t   minDelayTimer;                    /*!< The minimum accepted time before a Pending Dataset can be installed[s] */
} thrAttr_t;

/*!
    Thread network information base (Thread Nibs) structure:
    - thread specific string attributes
*/
typedef struct thrStringAttr_tag
{
    thrOctet32_t vendorName;                    /*!< Vendor name */
    thrOctet16_t modelName;                     /*!< Model Name */
    thrOctet16_t swVersion;                     /*!< Software version */
    thrOctet64_t provisioningURL;               /*!< Provisioning URL*/
    thrOctet16_t steeringData;                  /*!< Steering data */
    thrOctet32_t pskD;                          /*!< The passphrase used in authentication procedure - a new Joiner device is the correct one */
    uint8_t      stackVersion[6];               /*!< Stack version - ReadOnly */
    thrOctet64_t vendorData;                    /*!< Vendor data */
    thrOctet64_t commissionerId;                /*!< Commissioner ID */
} thrStringAttr_t;

typedef struct thrActiveDataSetAttr_tag
{
    uint8_t         channel;                    /*!< The current channel. */
    uint32_t        scanChannelMask;            /*!< The channels mask used when a network scanning is performed (energy scan, active scan or both);
                                                   0x07FFF800 means all 16 channels are used (from 11 to 26). */
    uint16_t        panId;                      /*!< The PAN identifier (ID).
                                                   - On network creation (calling THR_NwkCreate()), if it is set 0xffff then the device will generate a random pan ID. */
    uint8_t         xPanId[8];                  /*!< The extended PAN ID.
                                                   - On network creation (calling THR_NwkCreate()), if all bytes are 0xff's then the device will generate a random extended pan ID.
                                                   - On joining using out-of-band configuration (calling THR_NwkJoin() when devIsCommissioned = TRUE), if xPanId != all ff's
                                                   the device is using this extended pan id to find the pan ID and channel; otherwise no filter is apply*/
    thrPrefixAttr_t MLprefix;
    uint8_t         nwkMasterKey[16];           /*!< The network master key */
    thrOctet16_t    nwkName;                    /*!< Network Name.
                                                 -  On joining calling THR_NwkJoin() with devIsCommissioned == TRUE, if
                                                    nwkName.length != 0, the device will filter after network name to find the network parameters. */
    uint8_t         pskC[16];                   /*!< The Pre-Shared Key (PSKc) derived from  Commissioning Credential (network password)*/
    uint8_t         timestamp[8];               /*!< First 6 bytes: seconds, last 2 bytes: ticks */
    uint32_t        nwkKeyRotationInterval;     /*!< The current key rotation interval in hours */
    uint8_t         securityPolicy;             /*!< O and N bits without rotation time */
} thrActiveAttr_t;

typedef struct thrPendingDataSetAttr_tag
{
    uint16_t        channel;                    /*!< Pending channel */
    uint32_t        scanChannelMask;            /*!< Pending Channel Mask */
    uint16_t        panId;                      /*!< Pending Pan ID */
    uint8_t         xPanId[8];                  /*!< Pending Extended PanId */
    thrPrefixAttr_t mlPrefix;                   /*!< Pending MeshLocal prefix */
    uint8_t         nwkMasterKey[16];           /*!< Pending Master Key */
    thrOctet16_t    nwkName;                    /*!< Pending Network Name */
    uint8_t         pskC[16];                   /*!< Pending PSKc */
    uint8_t         securityPolicy;             /*!< Pending Security Policy bits */
    uint32_t        nwkKeyRotationInterval;     /*!< Pending Key Rotation Interval[sec] */
    uint8_t         activeTimestamp[8];         /*!< Pending Active Timestamp */
    uint8_t         timestamp[8];               /*!< Pending Timestamp */
    uint32_t        delayTimer;                 /*!< Pending Delay Timer[msec] */
} thrPendingAttr_t;


/*! border router network data attributes - ota format */
typedef struct thrOtaBrPrefixSet_tag
{
    uint8_t thrBrPrefixLength;                  /*!< Prefix length in bits */
    uint8_t thrBrPrefixValue[16];               /*!< Prefix value */

    uint8_t thrBrPrefixFlags[2];                /*!< Flags data - Border Router TLV of the Thread Network Data:
                                                   byte0 - reserved, byte1 - border router flags (see above BR prefix flags)*/
    uint8_t thrBrPrefixLifetime[4];             /*!< Prefix Data Lifetime (seconds) */
    bool_t thrBrPrefixAdvertised;               /*!< Flag that indicates whether a Border Router TLV SHALL
                                                   be advertised for prefix in the Network Data */

    uint8_t thrBrExtRouteFlags;                 /*!< Flags data - Has Route TLV of the Thread Network Data (see above BR external route flags)*/
    uint8_t thrBrExtRouteLifetime[4];           /*!< External Route Data Lifetime (seconds) */
    bool_t thrBrExtRouteAdvertised;             /*!< Flag that indicates whether a Has Route TLV SHALL
                                                   be advertised for prefix in the Network Data */
} thrOtaBrPrefixSet_t;

typedef struct thrBrServiceSet_tag
{
    uint8_t thrSenterpriseNumber[4];                    /*!< Senterprise number */
    uint8_t thrSserviceDataLen;                         /*!< Size of the Sservice data */
    uint8_t thrSserviceData[THR_SERVICE_DATA_MAX_LEN];  /*!< Sservice data eg "dnsserver" */
    uint8_t thrSserver16Addr[2];                        /*!< Sserver address */
    uint8_t thrSserverDataLen;                          /*!< Size of sserver data (16) */
    uint8_t thrSserverData[THR_SERVER_DATA_MAX_LEN];    /*!< Sserver data */
    uint8_t thrSserviceId;                              /*!< Sservice id */
    bool_t  thrSstable;                                 /*!< Sstable mode */
    //bool_t  thrSserviceAdvertised;
} thrLocalServiceSet_t;

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/*! Thread attributes:
    - saved using NVNG */
extern thrAttr_t *gpaThrAttr[];
/*! Thread string attributes:
    - saved using NVNG */
extern thrStringAttr_t *gpaThrStringAttr[];
/*! Thread active data set attributes:
    - saved using NVNG */
extern thrActiveAttr_t *gpaThrActiveAttr[];
/*! Thread pending data set attributes:
    - saved using NVNG */
extern thrPendingAttr_t *gpaThrPendingAttr[];
/*! Border router network data attributes
    - saved using NVNG */

/* Server Data tables */
extern ipAddr_t gaServerDataPrefixTbl[];
extern uint8_t gaServerDataPrefixLenTbl[];
extern borderRouterSet_t gaThrServerDataBrSetTbl[];
extern externalRouteSet_t gaServerDataExtRouteTbl[];
extern thrLocalServiceSet_t *gpaLocalServiceSetTbl[];

/*! Size of the border router service set table */
extern const uint8_t gLocalServiceSetTblSize;

/*==================================================================================================
Public function prototypes
==================================================================================================*/
/*!*************************************************************************************************
\fn    thrStatus_t THR_InitAttr(instanceId_t thrInstId, stackConfig_t *pStackCfg)
\brief  Initialize the attributes.

\param  [in]  thrInstID                  Thread instance Id
\param  [in]  pStackCfg                  Pointer to the stack config containing attribute data sets

\return       thrStatus_t                Thread status - Succes/Fail
 ***************************************************************************************************/
thrStatus_t THR_InitAttr(instanceId_t thrInstId, stackConfig_t *pStackCfg);

/*!*************************************************************************************************
\fn     thrStatus_t THR_GetAttr(instanceId_t thrInstID, thrAttrId_t attrID, uint32_t index,
                                uint32_t* pSize, uint8_t* pAttrValue);
\brief  Get thread attribute value.

\param  [in]   thrInstID     Thread instance Id
\param  [in]   attrID        Attribute Id
\param  [in]   index         Index (use zero if it is a scalar attribute)
\param  [out]  pSize         Attribute size
\param  [out]  pAttrValue    Pointer to Attribute Value

\return       thrStatus_t  Thread status - Succes/Fail
 ***************************************************************************************************/
thrStatus_t THR_GetAttr(instanceId_t thrInstID, thrAttrId_t attrID, uint32_t index,
                        uint32_t *pSize, uint8_t *pAttrValue);

/*!*************************************************************************************************
\fn     thrStatus_t THR_SetAttr(instanceId_t thrInstID, thrAttrId_t attrID, uint32_t index,
                               uint32_t size , uint8_t* pAttrValue)
\brief  Set thread attribute value.

\param  [in]  thrInstID    Thread instance Id
\param  [in]  attrID       Attribute Id
\param  [in]  index        Index
\param  [in]  size         Attribute size
\param  [in]  pAttrValue   Pointer to Attribute Value

\return       thrStatus_t  Thread status - Succes/Fail
 ***************************************************************************************************/
thrStatus_t THR_SetAttr(instanceId_t thrInstID, thrAttrId_t attrID, uint32_t index,
                        uint32_t size , uint8_t *pAttrValue);

/*================================================================================================*/
#endif  /* _THREAD_ATTRIBUTES_H */
/*!
** @}
*/
