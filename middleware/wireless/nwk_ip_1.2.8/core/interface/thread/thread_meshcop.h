/*! *********************************************************************************
 * \defgroup MCOP  Thread Commissioning Interface
 * @{
 ***********************************************************************************/
/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _THREAD_MESHCOP_H
#define _THREAD_MESHCOP_H

/*!
\file       meshcop.h
\brief      This is a header file for the MESHCOP module.

*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "network_utils.h"
#include "thread_types.h"
#include "stack_config.h"

#ifndef MESHCOP_ENABLED
    #define MESHCOP_ENABLED                      1
#endif

#if MESHCOP_ENABLED
/*==================================================================================================
Public macros
==================================================================================================*/
#define MESHCOP_O_MASK                  (1 << 7)
#define MESHCOP_N_MASK                  (1 << 6)
#define MESHCOP_R_MASK                  (1 << 5)
#define MESHCOP_C_MASK                  (1 << 4)
#define MESHCOP_B_MASK                  (1 << 3)
#define MESHCOP_CCM_MASK                (1 << 2)
#define MESHCOP_AE_MASK                 (1 << 1)
#define MESHCOP_NMP_MASK                (1)

#define MESHCOP_L_MASK                  (1 << 7)
#define MESHCOP_NCR_MASK                (1 << 6)
#define MESHCOP_RSV_BITS_MASK           (7 << 3)
#define MESHCOP_VR_BITS_MASK            (0x07)
#define MESHCOP_VR_BITS_VAL0            (0)
#define MESHCOP_VR_BITS_VAL1            (1)
#define MESHCOP_VR_BITS_VAL2            (2)
#define MESHCOP_VR_BITS_VAL3            (3)

#define MESHCOP_STATE_ACCEPT            (1)
#define MESHCOP_STATE_REJECT            ((uint8_t)(-1))
#define MESHCOP_STATE_PENDING           (0)
#define TLV_TYPE_LEN                    uint8_t type;\
                                        uint8_t len;
#define MESHCOP_MAX_PSK_LEN             (32)
#define TLV_NETWORK_PANID_LEN           (2U)
#define TLV_NETWORK_XPANID_LEN          (8U)
#define TLV_NETWORK_KEY_LEN             (16U)
#define TLV_NETWORK_KEY_SEQ_LEN         (4U)
#define TLV_NETWORK_ML_ULA_LEN          (8U)
#define MESHCOP_MAX_COMM_ID_LEN         (8)
#define MESHCOP_NETWORK_NAME_MAX_LEN    (16)
#define TLV_DOMAIN_PREFIX_LEN           (8)
#define MESHCOP_TLV_HDR_SIZE            (2)

#define MESHCOP_MAX_DATASETS            (3U)
/*==================================================================================================
Public type definitions
==================================================================================================*/

/*! TLV types */
typedef enum meshCopTlv_tag
{
    /* 0 -> 63 bit mask */
    /* Network Management TLVs */
    gMeshCopTlvChannel_c                = 0,
    gMeshCopTlvPanID_c                  = 1,
    gMeshCopTlvXpanID_c                 = 2,
    gMeshCopTlvNwkName_c                = 3,
    gMeshCopTlvPskc_c                   = 4,
    gMeshCopTlvNwkMasterKey_c           = 5,
    gMeshCopTlvNwkKeySeq_c              = 6,
    gMeshCopTlvNwkMlUla_c               = 7,
    gMeshCopTlvSteeringData_c           = 8,
    gMeshCopTlvBorderRouterLoc_c        = 9,
    gMeshCopTlvCommID_c                 = 10,
    gMeshCopTlvCommSessId_c             = 11,
    gMeshCopTlvSecPolicy_c              = 12,

    /* MeshCoP Protocol Command TLVs */
    gMeshCopTlvGet_c                    = 13,
    gMeshCopTlvActiveTimestamp_c        = 14,
    gMeshCopTlvCommissionerUdpPort_c    = 15,
    gMeshCopTlvState_c                  = 16,
    gMeshCopTlvJoinerDtlsEnc_c          = 17,
    gMeshCopTlvJoinerUdpPort_c          = 18,
    gMeshCopTlvJoinerAddr_c             = 19,
    gMeshCopTlvJoinerRouterLoc_c        = 20,
    gMeshCopTlvJoinerRouterKEK_c        = 21,
    gMeshCopTlvDomainPrefix_c           = 60,

    /* TMF Provisioning and Discovery TLVs */
    gMeshCopTlvProvisioningUrl_c        = 32,
    gMeshCopTlvVendorName_c             = 33,
    gMeshCopTlvVendorModel_c            = 34,
    gMeshCopTlvVendorSwVer_c            = 35,
    gMeshCopTlvVendorData_c             = 36,
    gMeshCopTlvVendorStackVer_c         = 37,
    gMeshCopTlvUdpEncapsulation_c       = 48,
    gMeshCopTlvIpv6Address_c            = 49,

    /* New Features */
    gMeshCopTlvPendingTimestamp_c       = 51,
    gMeshCopTlvDelayTimer_c             = 52,
    gMeshCopTlvChannelMask_c            = 53,
    gMeshCopTlvCount_c                  = 54,
    gMeshCopTlvPeriod_c                 = 55,
    gMeshCopTlvScanDuration_c           = 56,
    gMeshCopTlvEnergyList_c             = 57,
    gMeshCopTlvChannelPages_c           = 58,
    gMeshCopTlvDomainName_c             = 59,
    gMeshCopTlvAeSteeringData_c         = 61,
    gMeshCopTlvNmkpSteeringData_c       = 62,
    gMeshCopTlvCommTokenTLV_c           = 63,

    /* 64 -> 127 bit mask */
    gMeshCopTlvCommSigTLV_c             = 64,
    gMeshCopTlvAeUdpPort_c              = 65,
    gMeshCopTlvNmkpUdpPort_c            = 66,

    gMeshCopTlvTriHostname_c            = 67,
    gMeshCopTlvRegistrarAddr_c          = 68,
    gMeshCopTlvRegistrarHostname_c      = 69,

    gMeshCopTlvDiscoveryReq_c           = 128,
    gMeshCopTlvDiscoveryResp_c          = 129,


    gMeshCopTlvFuture_c                 = 240,
} meshCopTlv_t;

/*! Bloom filter mode */
typedef enum meshcopEuiMask_tag
{
    gMeshcopEuiMaskAllZeroes_c          = 0,    /*!< Don't allow any device */
    gMeshcopEuiMaskAllFFs_c             = 1,    /*!< Allow all devices */
    gMeshcopEuiMaskExpectedJoinerList_c = 2     /*!< Allow only expected joiners (see expected joiners list) */
} meshcopEuiMask_t;

/*! This entry defines a Joiner */
typedef struct expectedJoinerEntry_tag
{
    uint8_t aHashEui[8];                        /*!< Extended address of the Joiner(hash) */
    uint8_t aPsk[32];                           /*!< Password of the Joiner */
    uint8_t pskLen;                             /*!< Length in byte of the password */
    bool_t selected;                            /*!< This Joiner is used in computing steering or not */
    bool_t ffEntry;                             /*!< This entry represents all addresses */
} expectedJoinerEntry_t;

/*! Network Commissioner Events */
typedef enum
{
    /* Joiner Events */
    gThrEv_MeshCop_JoinerDiscoveryStarted_c =               /*!< Joiner has started discovery */
        gThrEvSet_NwkCommissioning_c + 1,
    gThrEv_MeshCop_JoinerDiscoveryFailed_c,                 /*!< No Thread networks/routers found */
    gThrEv_MeshCop_JoinerDiscoveryFailedFiltered_c,         /*!< Joiner Routers found, but device is filtered */
    gThrEv_MeshCop_JoinerDiscoverySuccess_c,                /*!< Network selected */
    gThrEv_MeshCop_JoinerDtlsSessionStarted_c,              /*!< Started DTLS session to commissioner (sent Hello) */
    gThrEv_MeshCop_JoinerDtlsError_c,                       /*!< DTLS session error – all DTLS errors, e.g.: incorrect PSKd */
    gThrEv_MeshCop_JoinerError_c,                           /*!< All other non-DTLS errors (e.g.: Joiner Router failed to send credentials) */
    gThrEv_MeshCop_JoinerAccepted_c,                        /*!< Joiner has received credentials */

    /* Commissioner Events(event set applies for all Commissioners: on-mesh, external, native) */
    gThrEv_MeshCop_CommissionerPetitionStarted_c,           /*!< Petitioning has started */
    gThrEv_MeshCop_CommissionerPetitionAccepted_c,          /*!< Petition success */
    gThrEv_MeshCop_CommissionerPetitionRejected_c,          /*!< Petition rejected */
    gThrEv_MeshCop_CommissionerPetitionError_c,             /*!< Other errors in petitioning (did not get PET response) */
    gThrEv_MeshCop_CommissionerKeepAliveSent_c,             /*!< Generated after each KA */
    gThrEv_MeshCop_CommissionerError_c,                     /*!< Errors during generating KA or other errors on the commissioner session */
    gThrEv_MeshCop_CommissionerJoinerDtlsSessionStarted_c,  /*!< A Joiner sent Hello */
    gThrEv_MeshCop_CommissionerJoinerDtlsError_c,           /*!< DTLS session error – all DTLS errors, e.g.: incorrect PSKd */
    gThrEv_MeshCop_CommissionerJoinerAccepted_c,            /*!< Joiner accepted */
    gThrEv_MeshCop_CommissionerNwkDataSynced_c,             /*!< generated after the commissioner changes the Nwk data */

    /* Commissioner Border Router Events(event set applies for: native, external commissioners) */
    gThrEv_MeshCop_CommissionerBrDtlsSessionStarted_c,      /*!< started DTLS session to BR (sent Hello) */
    gThrEv_MeshCop_CommissionerBrDtlsError_c,               /*!< DTLS session error – all DTLS errors, e.g.: incorrect PSKc */
    gThrEv_MeshCop_CommissionerBrError_c,                   /*!< All Other errors non-DTLS errors when communicating with the BR */
    gThrEv_MeshCop_CommissionerBrAccepted_c,                /*!< BR session established */

    /* Border Router Events */
    gThrEv_MeshCop_BrCommissionerDtlsSessionStarted_c,      /*!< Commissioner sent Hello */
    gThrEv_MeshCop_BrCommissionerDtlsError_c,               /*!< DTLS session error – all DTLS errors, e.g.: incorrect PSKc */
    gThrEv_MeshCop_BrCommissionerAccepted_c,                /*!< BR session established */
    gThrEv_MeshCop_BrCommissionerDataRelayedInbound_c,      /*!< After each relay from BR to Thread */
    gThrEv_MeshCop_BrCommissionerDataRelayedOutbound_c,     /*!< After each relay to BR from Thread */

    /* Joiner Router Events */
    gThrEv_MeshCop_JoinerrouterJoinerDataRelayedInbound_c,  /*!< After each relay from Joiner to Commissioner */
    gThrEv_MeshCop_JoinerrouterJoinerDataRelayedOutbound_c, /*!< After each relay to Joiner from Commissioner */
    gThrEv_MeshCop_JoinerrouterJoinerAccepted_c,            /*!< Before providing the security material to the Joiner */

    /* Provisioning */
    gThrEv_MeshCop_StartVendorProvisioning_c,               /*!< Device entered Joiner Provisioning mode */
} thrEvCodesComm_t;

/*! Structure used to specify input parameters for PSKc generation on commissioner */
typedef struct meshcopCredentialInput_tag
{
    uint8_t *pPskcStr; /*!< Pointer to the human readable password */
    uint32_t pskcStrLen; /*!< Size of the human readable password */
    uint8_t *pXpanId; /*!< Pointer to the extended pan id */
    uint8_t *pNwkName; /*!< Pointer to the network name */
    uint32_t nwkNameLen; /*!< Size of the network name buffer */
} meshcopCredentialInput_t;

/*! State TLV */
typedef struct meshCopStateTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t state; /*!< State value */
} meshCopStateTlv_t;

/*! Vendor name  TLV */
typedef struct meshCopVendorNameTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t vendorName[]; /*!< Vendor name */
} meshCopVendorNameTlv_t;

/*! Vendor model TLV */
typedef struct meshCopVendorModelTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t vendorModel[]; /*!< Vendor model */
} meshCopVendorModelTlv_t;

/*! Vendor software version TLV */
typedef struct meshCopVendorSwVerTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t vendorSwVer[]; /*!< Vendor software version */
} meshCopVendorSwVerTlv_t;

/*! Vendor data TLV */
typedef struct meshCopVendorDataTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t vendorData[]; /*!< Vendor dame */
} meshCopVendorDataTlv_t;

/*! Vendor stack TLV */
typedef struct meshCopStackVersionTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t vendorOui[3]; /*!< Organization unique identifier */
    uint8_t majMin; /*!< Major and minor version numbers of the Thread Stack */
    uint8_t revBuild[2];  /*!< Revision and build numbers of the Thread Stack */
} meshCopStackVersionTlv_t;

/*! Provisioning URL TLV */
typedef struct meshCopProvUrlTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t provUrl[]; /*!< Provisioning URL */
} meshCopProvUrlTlv_t;

/*!  Joiner Finalization TLVs*/
typedef struct meshCopJoinFinTlvs_tag
{
    meshCopStateTlv_t *pState; /*!< Pointer to the state tlv */
    meshCopVendorNameTlv_t *pVendorName; /*!< Pointer to the vendor name */
    meshCopVendorModelTlv_t *pVendorModel; /*!< Pointer to the vendor model */
    meshCopVendorSwVerTlv_t *pVendorSwVer; /*!< Pointer to the vendor software version */
    meshCopVendorDataTlv_t *pVendorData; /*!< Pointer to the vendor data */
    meshCopStackVersionTlv_t *pVendorStackVer; /*!< Pointer to the vendor stack version */
    meshCopProvUrlTlv_t *pProvUrl; /*!< Pointer to the provisioning url */
} meshCopJoinFinTlvs_t;

/*! Channel TLV */
typedef struct meshCopChannelTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t channelPage;  /*!< Channel page */
    uint8_t channel[2];  /*!< Channel */
} meshCopChannelTlv_t;

typedef struct meshCopChannelMaskTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t channelPage;  /*!< Channel page */
    uint8_t maskLength;  /*!< Channel mask length */
    uint8_t channelMask[4];  /*!< Channel mask */
} meshCopChannelMaskTlv_t;

typedef struct meshCopCountTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t count;  /*!< Count */
} meshCopCountTlv_t;

typedef struct meshCopPeriodTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t period[2];   /*!< Period between successive scans */
} meshCopPeriodTlv_t;

typedef struct meshCopEnergyListTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t aList[]; /*!< Energy list start */
} meshCopEnergyListTlv_t;

typedef struct meshCopScanDurationTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t scanDuration[2];   /*!< The scan duration in MAC units */
} meshCopScanDurationTlv_t;

typedef struct meshCopDiscoveryReqTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t verFlags;   /*!< Version flags */
    uint8_t reserved;  /*!< Reserved */
} meshCopDiscoveryReqTlv_t;

typedef struct meshCopDiscoveryRespTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t verFlags; /*!< Version flags */
    uint8_t reserved; /*!< Reserved */
} meshCopDiscoveryRespTlv_t;

typedef struct meshCopDiscoveryTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t value[];   /*!< Start of the discovery tlvs */
} meshCopDiscoveryTlv_t;

typedef struct meshCopNwkChannelTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t channel;  /*!< Channel */
} meshCopNwkChannelTlv_t; /* read-only */

typedef struct meshCopNwkPanIdTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t panId[TLV_NETWORK_PANID_LEN];  /*!< Pan id */
} meshCopNwkPanIdTlv_t; /* read-only */

typedef struct meshCopNwkXPanIdTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t xPanId[TLV_NETWORK_XPANID_LEN];  /*!< Extended pan id */
} meshCopNwkXPanIdTlv_t; /* read-only */

typedef struct meshCopNwkNameTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t nwkName[];  /*!< Network name */
} meshCopNwkNameTlv_t; /* read/write */

typedef struct meshCopPskcTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t pskc[];  /*!< Commissioner credential */
} meshCopPskcTlv_t; /* write-only */

typedef struct meshCopNwkMasterKeyTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t masterKey[TLV_NETWORK_KEY_LEN];  /*!< Master key */
} meshCopNwkMasterKeyTlv_t; /* read-only */

typedef struct meshCopNwkKeySeqTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t keySeq[TLV_NETWORK_KEY_SEQ_LEN];  /*!< Key sequence */
} meshCopNwkKeySeqTlv_t; /* read-only */

typedef struct meshCopNwkMlUlaTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t mlUla[TLV_NETWORK_ML_ULA_LEN];  /*!< Mesh local prefix */
} meshCopNwkMlUlaTlv_t; /* read-only */

typedef struct meshCopSteeringTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t filter[]; /*!< Filter bytes */
} meshCopSteeringTlv_t; /* read/write */

typedef struct meshCopBrLocTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t addr[2]; /*!< Short address in network order */
} meshCopBrLocTlv_t; /* read-only */

typedef struct meshCopCommIdTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t commId[]; /*!< Commissioner id */
} meshcopCommIdTlv_t; /* read-only */

typedef struct meshCopCommSessIdTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t id[2]; /*!< Commissioner session id in network order */
} meshCopCommSessIdTlv_t; /* read-only */

typedef struct meshCopGetTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t aIds[]; /*!< List of tlv ids */
} meshCopGetTlv_t;

typedef struct meshCopActiveTimestampTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t seconds[6]; /*!< Seconds part */
    uint8_t ticks[2]; /*!< Ticks part */
} meshCopActiveTimestampTlv_t; /* read/write */

typedef struct meshCopCommissionerUdpPortTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t aPort[2]; /*!< Port number in network order */
} meshCopCommissionerUdpPortTlv_t;

typedef struct meshCopJoinerUdpPortTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t aPort[2]; /*!< Port number in network order */
} meshCopJoinerUdpPortTlv_t;

typedef struct meshCopPendingTimestampTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t seconds[6]; /*!< Seconds part */
    uint8_t ticks[2]; /*!< Ticks part */
} meshCopPendingTimestampTlv_t; /* read/write */

typedef struct meshCopSecurityPolicyTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t rotationInterval[2]; /*!< Key rotation interval in network order */
    uint8_t policy; /*!< Policy bits */
} meshCopSecurityPolicyTlv_t; /* read/write */

typedef struct meshCopMacExtendedAddressTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t aExtendedAddress[8]; /*!< Extended address */
} meshCopMacExtendedAddressTlv_t;

typedef struct meshCopDelayTimerTlv_tag
{
    uint8_t type; /*!< Tlv type */
    uint8_t len; /*!< Tlv length */
    uint8_t timeRemaining[4]; /*!< Timer value in netowrk byte order[ms] */
} meshCopDelayTimerTlv_t;

typedef struct meshCopStoreTlv_tag
{
    uint8_t len;
    uint8_t value[256];
}meshCopStoreTlv_t;


/* Diagnostics types */
typedef enum meshcopDiagnosticDir_tag
{
    gMeshcopDiagnosticOut_c = 0, /*!< The packet was sent */
    gMeshcopDiagnosticIn_c = 1, /*!< The packet was received */
} meshcopDiagnosticDir_t;

typedef enum meshcopDiagnosticType_tag
{
    gMeshcopDiagnosticJoinFinReq_c, /*!< JOIN_FIN.req packet */
    gMeshcopDiagnosticJoinFinRsp_c, /*!< JOIN_FIN.rsp packet */
    gMeshcopDiagnosticJoinEntReq_c, /*!< JOIN_ENT.req packet */
    gMeshcopDiagnosticJoinEntRsp_c, /*!< JOIN_ENT.rsp packet */
    gMeshcopDiagnosticCloseNotify_c, /*!< DTLS close notify packet */
    gMeshcopDiagnosticLog_c, /*!< Logging */
} meshcopDiagnosticType_t;

/*!*************************************************************************************************
\brief  Callback used to send meshcop diagnostics

\param  [in]    meshcopDiagType         Diagnostics type
\param  [in]    dir                     Direction of packet
\param  [in]    pEui                    Pointer to eui address
\param  [in]    pTlvs                   Pointer to tlvs
\param  [in]    tlvsLen                 Tlvs length

\return         NONE
***************************************************************************************************/
typedef void (*meshcopDiagnosticHandlerCb_t)(meshcopDiagnosticType_t meshcopDiagType,
    meshcopDiagnosticDir_t dir, uint8_t *pEui, uint8_t *pTlvs, uint32_t tlvsLen);

/*! Discovery TLVs */
typedef struct meshcopDiscoveryRespTlvs_tag
{
    meshCopDiscoveryRespTlv_t *pDiscRespTlv; /*!< Pointer to discovery response tlv */
    meshCopNwkXPanIdTlv_t *pXpanIdTlv; /*!< Pointer to extended pan id tlv */
    meshCopNwkNameTlv_t *pNwkNameTlv; /*!< Pointer to network name tlv */
    meshCopSteeringTlv_t *pSteeringDataTlv; /*!< Pointer to steering data tlv */
    meshCopJoinerUdpPortTlv_t *pJoinerUdpPortTlv; /*!< Pointer to joiner udp port tlv */
    meshCopCommissionerUdpPortTlv_t *pCommissionerUdpPortTlv; /*!< Pointer to Commissioner udp port tlv */
} meshcopDiscoveryRespTlvs_t;

/*! Discovery event received by the Discovery Response Callback */
typedef enum thrDiscoveryEvent_tag
{
    gThrDiscoveryStarted_c, /*!< The discovery mechanism has been started */
    gThrDiscoveryRespRcv_c, /*!< Discovery response packet has been received */
    gThrDiscoveryStopped_c /*!< Discovery mechanism has been completed */
} thrDiscoveryEvent_t;

/*! Discovery Response message */
typedef struct thrDiscoveryRespInfo_tag
{
    uint32_t LQI; /*!< LQI of the packet */
    uint16_t panId; /*!< Pan id from where the discovery response packet came */
    uint8_t aEui[8]; /*!< Extended address of the sender of the discovery response packet */
    uint8_t channel; /*!< Channel number from where the discovery packet came */
} thrDiscoveryRespInfo_t;

/*!*************************************************************************************************
\brief  The Discovery Response Callback used by the application to receive the Discovery Responses
 received during the Discovery process

\param  [in]    thrInstId               Thread instance ID
\param  [in]    event                   Discovery event type
\param  [in]    lqi                     LQI of the Discovery packet received
\param  [in]    pDiscoveryRespInfo      Pointer to a structure containing information about the
                                        received Discovery response packet
\param  [in]    pDiscoveryRespTlvs      Pointer to the Discovery response tlvs

\return         NONE
***************************************************************************************************/
typedef void (*thrDiscoveryRespCb_t)(instanceId_t thrInstId, thrDiscoveryEvent_t event,
                                        uint8_t lqi, thrDiscoveryRespInfo_t* pDiscoveryRespInfo,
                                        meshcopDiscoveryRespTlvs_t *pDiscoveryRespTlvs);

/*==================================================================================================
Public global variables declarations
==================================================================================================*/
/*!  List of expected joiners (of type expectedJoinerEntry_t) */
extern list_t gThrExpectedJoinerList;

/*!  The current commissioner mode */
extern thrCommissionerMode_t gMeshcopCommissionerMode;

/*==================================================================================================
Public function prototypes
==================================================================================================*/
/*!*************************************************************************************************
\fn     bool_t MESHCOP_WeAreCommissioner
\brief  This function is used to determine if current device is a commissioner

\param  [in]    thrInstId               Thread instance ID

\return         bool_t                  TRUE if device is commissioner, FALSE otherwise
***************************************************************************************************/
bool_t MESHCOP_WeAreCommissioner(instanceId_t thrInstId);

/*!*************************************************************************************************
\fn     thrStatus_t MESHCOP_StartCommissioner
\brief  This function is used to start the Commissioner on the local device.

\param  [in]    thrInstId               Thread instance ID

\return         thrStatus_t
***************************************************************************************************/
thrStatus_t MESHCOP_StartCommissioner(instanceId_t thrInstId);

/*!*************************************************************************************************
\fn     thrStatus_t MESHCOP_StartNativeCommissionerScan(instanceId_t thrInstId)
\brief  This function is used to start the scan process on behalf of the Native Commissioner.

\param  [in]    thrInstId               Thread instance ID

\return         thrStatus_t             Status
***************************************************************************************************/
thrStatus_t MESHCOP_StartNativeCommissionerScan(instanceId_t thrInstId);

/*!*************************************************************************************************
\fn     bool_t MESHCOP_StopCommissioner(instanceId_t thrInstId, bool_t updateNwk)
\brief  This function is used to stop the Commissioner on this device.

\param  [in]    thrInstId               Thread instance ID
\param  [in]    updateNwk               Send information into the network

\return         bool_t                  TRUE - if the stop operation succeeded
                                        FALSE - otherwise
***************************************************************************************************/
bool_t MESHCOP_StopCommissioner(instanceId_t thrInstId, bool_t updateNwk);

/*!*************************************************************************************************
\fn     bool_t MESHCOP_AddExpectedJoiner(instanceId_t thrInstId, uint8_t *pEui, uint8_t *pPsk,
                                         uint32_t pskLen, bool_t selected)
\brief  Add a Joiner to the expected joiners list.

\param  [in]    thrInstId               Thread instance ID
\param  [in]    pEui                    Pointer to the extended address of the Joiner
\param  [in]    pPsk                    Pointer to given pskc
\param  [in]    pskLen                  Length of given pskc
\param  [in]    selected                Use this entry or not

\return         TRUE                    The Joiner was scanned successfully
\return         FALSE                   The Joiner was not scanned successfully
***************************************************************************************************/
bool_t MESHCOP_AddExpectedJoiner(instanceId_t thrInstId, uint8_t *pEui, uint8_t *pPsk,
    uint32_t pskLen, bool_t selected);

/*!*************************************************************************************************
\fn     expectedJoinerEntry_t *MESHCOP_GetExpectedJoinerList(instanceId_t thrInstId)
\brief  Get the expected joiner list gThrExpectedJoinerList.

\param  [in]    thrInstId               Thread instance ID

\return         expectedJoinerEntry_t*  Pointer to first entry in the gThrExpectedJoinerList
\return         NULL                    In case the entry was not found
***************************************************************************************************/
expectedJoinerEntry_t *MESHCOP_GetExpectedJoinerList(instanceId_t thrInstId);

/*!*************************************************************************************************
\fn     expectedJoinerEntry_t *MESHCOP_GetExpectedJoiner(instanceId_t thrInstId, uint8_t *pHashEui)
\brief  Get an expected joiner from gThrExpectedJoinerList.

\param  [in]    thrInstId               Thread instance ID
\param  [in]    pHashEui                Pointer to the hash extended address(optional)
\param  [in]    pEui                    Pointer to the extended address(optional)

\return         expectedJoinerEntry_t*  Pointer to the Joiner entry
\return         NULL                    In case the entry was not found
***************************************************************************************************/
expectedJoinerEntry_t *MESHCOP_GetExpectedJoiner(instanceId_t thrInstId, uint8_t *pHashEui,
    uint8_t *pEui);

/*!*************************************************************************************************
\fn     bool_t MESHCOP_RemoveExpectedJoiner(instanceId_t thrInstId, uint8_t *pHashEui, uint8_t *pEui)
\brief  Remove an expected joiner from the gThrExpectedJoinerList list.

\param  [in]    thrInstId               Thread instance ID
\param  [in]    pHashEui                Pointer to the hash extended address(optional)
\param  [in]    pEui                    Pointer to the extended address(optional)

\return         TRUE                    Item was found
\return         FALSE                   Item was not found
***************************************************************************************************/
bool_t MESHCOP_RemoveExpectedJoiner(instanceId_t thrInstId, uint8_t *pHashEui, uint8_t *pEui);

/*!*************************************************************************************************
\fn     void MESHCOP_RemoveAllExpectedJoiners(instanceId_t thrInstId)
\brief  Remove all expected joiners from the gThrExpectedJoinerList list.

\param  [in]    thrInstId               Thread instance ID
***************************************************************************************************/
void MESHCOP_RemoveAllExpectedJoiners(instanceId_t thrInstId);

/*!*************************************************************************************************
\fn     void MESHCOP_SyncSteeringData(instanceId_t thrInstId, meshcopEuiMask_t euiMask)
\brief  Sync the steering data on the network with our device.

\param  [in]    thrInstId               Thread instance ID
\param  [in]    euiMask                 Specify which devices will be steered

\return         none
***************************************************************************************************/
void MESHCOP_SyncSteeringData(instanceId_t thrInstId, meshcopEuiMask_t euiMask);

/*!*************************************************************************************************
\fn     meshcopSteeringMatch_t MESHCOP_CheckSteeringData(meshCopSteeringTlv_t *pSteeringDataTlv)
\brief  Check if this device is in the received steering data.

\param  [in]    pSteeringDataTlv        Pointer to the Steering Data TLV

\return         meshcopSteeringMatch_t  Matching type
***************************************************************************************************/
meshcopSteeringMatch_t MESHCOP_CheckSteeringData(const meshCopSteeringTlv_t *pSteeringDataTlv);

/*!*************************************************************************************************
\fn     void MESHCOP_SetCommissionerCredential(instanceId_t thrInstId, meshcopCredentialInput_t *pParams)
\brief  Function used to compute and set the PSKc, network name, extended pan Id attributes
        on the commissioner.

\param  [in]    thrInstId               Thread instance id
\param  [in]    pParams                 Pointer to the input parameters
***************************************************************************************************/
void MESHCOP_SetCommissionerCredential(instanceId_t thrInstId, const meshcopCredentialInput_t *pParams);

/*!*************************************************************************************************
\fn     void MESHCOP_SetDiagHandler(instanceId_t thrInstId, meshcopDiagnosticHandlerCb_t pfTlvsHandler)
\brief  Function used to set the function which will handle tlvs received during the commissioning
        process.

\param  [in]    thrInstId               Thread instance id
\param  [in]    pfTlvsHandler           Pointer to the tlvs function handler
***************************************************************************************************/
void MESHCOP_SetDiagHandler(instanceId_t thrInstId, meshcopDiagnosticHandlerCb_t pfTlvsHandler);

/*!*************************************************************************************************
\fn     uint8_t *MESHCOP_AddTlvs(instanceId_t thrInstanceID, uint8_t *pStart, uint64_t *pMask,
        uint8_t datasetType, bool_t noSecPolicy, void *pExtraParams)
\brief  Function used add TLV information into buffer.

\param  [in]    thrInstId               Thread instance id
\param  [in]    pStart                  Pointer to the start of the buffer
\param  [in]    pMask                   Pointer to the mask array
\param  [in]    datasetType             Request data from active, pending or CIM provisioning set
\param  [in]    noSecPolicy             Internal use: add any TLV w/o taking care of security policy
\param  [in]    pExtraParams            Extra parameters passed to the function

\return         uint8_t*                Pointer to the buffer after addition
***************************************************************************************************/
uint8_t *MESHCOP_AddTlvs(instanceId_t thrInstanceID, uint8_t *pStart, uint64_t *pMask,
    uint8_t datasetType, bool_t noSecPolicy, void *pExtraParams);

/*!*************************************************************************************************
\fn     uint32_t MESHCOP_GetTlvsLen(instanceId_t thrInstanceID, uint64_t *pMask, uint8_t datasetType,
        bool_t noSecPolicy)
\brief  Function used to get the length of the TLVs requested in mask.

\param  [in]    thrInstId               Thread instance id
\param  [in]    pStart                  Pointer to the start of the buffer
\param  [in]    pMask                   Pointer to the mask array
\param  [in]    datasetType             Compute length from active, pending or CIM provisioning set
\param  [in]    noSecPolicy             Do not take care of security policy

\return         uint32_t                Length of the TLVs requested in the mask
***************************************************************************************************/
uint32_t MESHCOP_GetTlvsLen(instanceId_t thrInstanceID, uint64_t *pMask, uint8_t datasetType, bool_t
    noSecPolicy);

/*!*************************************************************************************************
\fn     uint8_t MESHCOP_RegisterBrServerAddr6(instanceId_t thrInstId, ipIfUniqueId_t ifId,
        ipAddr_t *pAddr)
\brief  Function used to register border router server address.

\param  [in]    thrInstId               Thread instance ID
\param  [in]    ifId                    IP Interface identifier
\param  [in]    pAddr                   Pointer to the IP address

\return         uint8_t
***************************************************************************************************/
uint8_t MESHCOP_RegisterBrServerAddr6(instanceId_t thrInstId, ipIfUniqueId_t ifId, const ipAddr_t *pAddr);

/*!*************************************************************************************************
\fn     void MESHCOP_NwkJoin(instanceId_t thrInstId, thrNwkJoiningEntry_t *pNwkJoiningList,
                             uint32_t nbOfNwkJoiningEntries)
\brief  Run through the pNwkJoiningList list and join using the commissioning procedure.
        If Thread BH is enabled then pAeNwkJoiningList is filled with candidate parents for
        Autonomous Enrollment. The Joiner will first try to do AE. If not successful, it will
        continue with 1.1 Commissioning.
        NOTE:
        - More callback functions must be registered (using EVM_RegisterStatic() function)
          with the gThrEvSet_NwkJoin_c and gThrEvSet_NwkCommissioning_c event
          set to receive the network join events.
        - pNwkJoiningList is a allocated buffer and it will be free by the function

\param [in]  thrInstID                  Thread instance Id
\param [in]  pNwkJoiningList            Pointer to network joining list
\param [in]  nbOfNwkJoiningEntries      Size of network joining list
\param [in]  pAeNwkJoiningList          Pointer to the AE network joining list (only for THREAD BH)
\param [in]  nbAeOfNwkJoiningEntries    Size of AE network joining list (only for THREAD_BH)

\return      thrStatus_t            Status
***************************************************************************************************/
void MESHCOP_NwkJoin(instanceId_t thrInstId, thrNwkJoiningEntry_t *pNwkJoiningList,
                     uint32_t nbOfNwkJoiningEntries, thrNwkJoiningEntry_t *pAeNwkJoiningList,
                     uint32_t nbAeOfNwkJoiningEntries);

#endif /* _THREAD_MESHCOP_H*/
/*================================================================================================*/
#endif

/*!
** @}
*/
