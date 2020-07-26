/*
*         Copyright (c), NXP Semiconductors Bangalore / India
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
* particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* Generic Discovery Loop Activities Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  SS: Generated 23. Jan 2015
*
*/

#ifndef PHACDICLOOP_H
#define PHACDICLOOP_H

#include <ph_Status.h>
/* #define __DEBUG */

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifdef NXPBUILD__PHAC_DISCLOOP_SW

/** \defgroup phacDiscLoop_Sw Component : Software
* @{
*/
/**
 * ID for this layer (software implementation of Discover loop component).
 *
 * \ref phacDiscLoop_Sw_DataParams::wId is initialized with this ID in the
 * initialization function \ref phacDiscLoop_Sw_Init.
 * */
#define PHAC_DISCLOOP_SW_ID                                   0x01U

/**
 * Maximum cards supported per technologies in passive poll mode.
 *
 * By default maximum cards supported per technology is set to 1. Users can
 * change this value to support more cards.
 *
 * NOTE: Size of discovery loop data structure \ref phacDiscLoop_Sw_DataParams
 * will increase if this value is increased.
 * */
#define PHAC_DISCLOOP_CFG_MAX_CARDS_SUPPORTED                 0x01U

/**
 * Maximum cards supported per vicinity technologies in passive poll mode.
 *
 * By default maximum cards supported per vicinity technology is set to 1. Users can
 * change this value to support more cards.
 *
 * NOTE: Size of discovery loop data structure \ref phacDiscLoop_Sw_DataParams
 * will increase if this value is increased.
 * */
#define PHAC_DISCLOOP_CFG_MAX_VICINITY_CARDS_SUPPORTED        0x01U

/**
 * Maximum (NFC + proprietary) technologies support in passive poll mode.
 *
 * Supported technologies are:\n
 * - Type A (As per NFC Activity 1.1 and EMVCo Specification 2.3.1a)\n
 * - Type B (As per NFC Activity 1.1 and EMVCo Specification 2.3.1a)\n
 * - Type F @ 212 kbps (As per NFC Activity 1.1)\n
 * - Type F @ 424 kbps (As per NFC Activity 1.1)\n
 * - Type V (ISO 15693) (As per NFC Activity draft specification)\n
 * - ISO 18000P3M3 (Proprietary)
 * */
#define PHAC_DISCLOOP_PASS_POLL_MAX_TECHS_SUPPORTED           0x06U

/**
* \name Supported NFC Activity versions.
* \brief Discovery loop support both NFC Activity version 1.0 and 1.1. By
* default discovery loop is configured for NFC Activity version 1.1. Use
* \ref phacDiscLoop_SetConfig with #PHAC_DISCLOOP_CONFIG_ACTIVITY_VERSION option
* to configure below values.
*/
/*@{*/
#define PHAC_DISCLOOP_NFC_ACTIVITY_VERSION_1_0                0x01U    /**< Supports NFC Activity version 1.0. */
#define PHAC_DISCLOOP_NFC_ACTIVITY_VERSION_1_1                0x02U    /**< Supports NFC Activity version 1.1. This is default. */
/*@}*/

/**
* \name Discovery loop return status codes.
*/
/*@{*/
/**
 * Discovery loop failure.
 *
 * This status can be returned by \ref phacDiscLoop_Run in poll and listen modes.
 * This is returned when discovery loop failed to proceed further due to an error
 * in lower layer. Use \ref phacDiscLoop_GetConfig with #PHAC_DISCLOOP_CONFIG_ADDITIONAL_INFO
 * to get the actual error code from lower layer.
 * */
#define PHAC_DISCLOOP_FAILURE                               0x0080U

/**
 * Indicates that collision is pending.
 *
 * This status is returned by \ref phacDiscLoop_Run in passive poll mode when
 * collision resolution is disabled or when device limit is set to zero for a
 * detected technology.
 * Use \ref phacDiscLoop_GetConfig with #PHAC_DISCLOOP_CONFIG_COLLISION_PENDING
 * option to get collision pending status for all technologies.
 * */
#define PHAC_DISCLOOP_COLLISION_PENDING                     0x0081U

/**
 * External RF field on.
 *
 * This status is returned by \ref phacDiscLoop_Run in passive poll mode when
 * external RF is detected during RF collision avoidance (RFCA) procedure. On
 * getting this error application may restart \ref phacDiscLoop_Run in listen
 * mode if supported.
 * */
#define PHAC_DISCLOOP_EXTERNAL_RFON                         0x0082U

/**
 * External RF field off.
 *
 * This status is returned by \ref phacDiscLoop_Run in listen mode if external
 * RF is switched off during listen. On getting this error application may
 * restart discovery loop in poll mode or wait for a specific time for external
 * RF to turn ON.
 * */
#define PHAC_DISCLOOP_EXTERNAL_RFOFF                        0x0083U

/**
 * No technologies detected.
 *
 * This status is returned by \ref phacDiscLoop_Run in passive poll mode if no
 * technologies are detected during detection stage of passive polling.
 * */
#define PHAC_DISCLOOP_NO_TECH_DETECTED                      0x0084U

/**
 * No card/device resolved.
 *
 * This status is returned by \ref phacDiscLoop_Run in passive poll mode if no
 * card/device is resolved during collision resolution stage but that technology
 * was detected during technology detection stage of passive polling.
 * */
#define PHAC_DISCLOOP_NO_DEVICE_RESOLVED                    0x0085U

/**
 * Low Power Card Detection (LPCD) succeeded but no technologies detected.
 *
 * This status is returned by \ref phacDiscLoop_Run in passive poll mode when
 * LPCD is enabled and LPCD returned success but no technologies were detected
 * during technology detection stage of passive poll mode. This can be due to
 * false triggering of LPCD interrupt or LPCD triggered by a card/device not
 * supported by discovery loop.
 * */
#define PHAC_DISCLOOP_LPCD_NO_TECH_DETECTED                 0x0086U

/**
 * Multiple Technologies detected.
 *
 * This status is returned by \ref phacDiscLoop_Run in passive poll mode when
 * multiple technologies are detected during technology detection stage of
 * passive poll mode.\n
 *
 * To continue with collision resolution stage of one of the detected technology,
 * application can follow below steps:\n
 *
 * - Use \ref phacDiscLoop_GetConfig with #PHAC_DISCLOOP_CONFIG_TECH_DETECTED
 * to get technologies detected.\n
 * - Use \ref phacDiscLoop_SetConfig with #PHAC_DISCLOOP_CONFIG_PAS_POLL_TECH_CFG
 * to configure passive polling for a single technology (one of the detected
 * technologies).\n
 * - Use \ref phacDiscLoop_SetConfig with #PHAC_DISCLOOP_CONFIG_NEXT_POLL_STATE
 * option and #PHAC_DISCLOOP_POLL_STATE_COLLISION_RESOLUTION as value and start
 * \ref phacDiscLoop_Run to start passive poll mode in collision resolution phase.
 * */
#define PHAC_DISCLOOP_MULTI_TECH_DETECTED                   0x0088U

/**
 * Multiple cards/devices resolved.
 *
 * This status is returned by \ref phacDiscLoop_Run in passive poll mode when
 * multiple cards/devices are resolved during collision resolution stage of
 * passive poll mode when CON_DEVICES_LIMIT > 1.\n
 * - Use \ref phacDiscLoop_GetConfig with #PHAC_DISCLOOP_CONFIG_TECH_DETECTED
 * to get the technology detected.\n
 * - Use \ref phacDiscLoop_GetConfig with #PHAC_DISCLOOP_CONFIG_NR_TAGS_FOUND
 * option to get number of devices/tags resolved.\n
 * - To activate a specific device/card from these resolved, application can use
 * \ref phacDiscLoop_ActivateCard.
 * */
#define PHAC_DISCLOOP_MULTI_DEVICES_RESOLVED                0x008AU

/**
 * Single card/device activated.
 *
 * This status is returned by \ref phacDiscLoop_Run in passive poll mode when
 * a single card/device is successfully activated during activation stage.\n
 * Use \ref phacDiscLoop_GetConfig with #PHAC_DISCLOOP_CONFIG_TECH_DETECTED
 * to get the technology detected.
 * */
#define PHAC_DISCLOOP_DEVICE_ACTIVATED                      0x008BU

/**
 * Active target activated.
 *
 * This status is returned by \ref phacDiscLoop_Run in active poll mode when
 * a single active target is successfully activated during active polling.\n
 * Use \ref phacDiscLoop_GetConfig with #PHAC_DISCLOOP_CONFIG_TECH_DETECTED
 * to get the activated technology.
 * */
#define PHAC_DISCLOOP_ACTIVE_TARGET_ACTIVATED               0x008CU

/**
 * Passive target activated.
 *
 * This status is returned by \ref phacDiscLoop_Run in passive poll mode when
 * a single passive target is successfully activated during passive polling.\n
 * Use \ref phacDiscLoop_GetConfig with #PHAC_DISCLOOP_CONFIG_TECH_DETECTED
 * to get the activated technology.
 * */
#define PHAC_DISCLOOP_PASSIVE_TARGET_ACTIVATED              0x008DU

/**
 * Merged SEL_RES (Type A) with NFC-DEP and T4T support found.
 *
 * This status is returned by \ref phacDiscLoop_Run in passive poll mode when
 * device/card is successfully activated till ISO 14443-3 and both T4T and
 * NFC DEP support is detected in SEL_RES/SAK. Then ATR_REQ or RATS is not sent
 * by discovery loop. On getting this status application can decide whether to
 * send ATR_REQ or RATS. This status is applicable only to Type A.
 * */
#define PHAC_DISCLOOP_MERGED_SEL_RES_FOUND                  0x008EU

/**
 * Activated in listen mode.
 *
 * This status is returned by \ref phacDiscLoop_Run in listen mode when it got
 * successfully activated in listen mode by another reader/initiator.\n
 * Application shall check \ref phacDiscLoop_TargetParams to validate the
 * received request (ATR_REQ/RATS), data rate and framing (active/passive) and
 * send the response.
 * */
#define PHAC_DISCLOOP_ACTIVATED_BY_PEER                     0x008FU
/*@}*/

/**
* \name Bit positions for different technologies.
*
* \brief Application can use these mask values to find the detected technology,
* collision pending status, bail out configuration etc.\n
*
* Example (Check for Type A detection in passive poll mode):\n
*  - \ref phacDiscLoop_GetConfig (pDataParams, #PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTagsDetected)\n
*  - #PHAC_DISCLOOP_CHECK_ANDMASK (wTagsDetected, #PHAC_DISCLOOP_POS_BIT_MASK_A)\n
*/
/*@{*/
#define PHAC_DISCLOOP_POS_BIT_MASK_A                          0x01U   /**< Position bit mask for Type A. */
#define PHAC_DISCLOOP_POS_BIT_MASK_B                          0x02U   /**< Position bit mask for Type B. */
#define PHAC_DISCLOOP_POS_BIT_MASK_F212                       0x04U   /**< Position bit mask for Type F212. */
#define PHAC_DISCLOOP_POS_BIT_MASK_F424                       0x08U   /**< Position bit mask for Type F424. */
#define PHAC_DISCLOOP_POS_BIT_MASK_V                          0x10U   /**< Position bit mask for Type V. */
#define PHAC_DISCLOOP_POS_BIT_MASK_18000P3M3                  0x20U   /**< Position bit mask for ISO 18000P3M3. */
/*@}*/

/**
* \name Bit positions for active poll mode technologies.
*
* \brief Application can use these mask values to find the detected technology
* in active poll mode and to configure technologies to be detected in active
* poll mode.\n
*
* Example (Check for Active @ 106 kbps detection):\n
*  - \ref phacDiscLoop_GetConfig (pDataParams, #PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTagsDetected)\n
*  - #PHAC_DISCLOOP_CHECK_ANDMASK (wTagsDetected, #PHAC_DISCLOOP_ACT_POS_BIT_MASK_106)\n
*/
/*@{*/
#define PHAC_DISCLOOP_ACT_POS_BIT_MASK_106                    0x01U   /**< 106 Kbps Bit rate for Active Communication. */
#define PHAC_DISCLOOP_ACT_POS_BIT_MASK_212                    0x02U   /**< 212 Kbps Bit rate for Active Communication. */
#define PHAC_DISCLOOP_ACT_POS_BIT_MASK_424                    0x04U   /**< 424 Kbps Bit rate for Active Communication. */
/*@}*/

/**
* \name FeliCa data rates.
*/
/*@{*/
#define PHAC_DISCLOOP_CON_BITR_212  PHAC_DISCLOOP_POS_BIT_MASK_F212   /**< Configures 212 Kbps bit rate for Type F tag. */
#define PHAC_DISCLOOP_CON_BITR_424  PHAC_DISCLOOP_POS_BIT_MASK_F424   /**< Configures 424 Kbps bit rate for Type F tag. */
/*@}*/

/**
* \name Type A default parameters.
*/
/*@{*/
#define PHAC_DISCLOOP_I3P3A_MAX_ATQA_LENGTH                   0x02U   /**< Answer to RequestA (ATQA) length for ISO 14443-3A card type. */
#define PHAC_DISCLOOP_I3P3A_MAX_UID_LENGTH                    0x0AU   /**< Maximum UID length for 14443 Type A compliant tags. */
#define PHAC_DISCLOOP_I3P3A_SINGLE_SIZE_UID_LENGTH            0x04U   /**< UID length for single size NFCID1 Type A compliant tags. */
#define PHAC_DISCLOOP_I3P3A_DOUBLE_SIZE_UID_LENGTH            0x07U   /**< UID length for double size NFCID1 Type A compliant tags. */
#define PHAC_DISCLOOP_I3P3A_TRIPLE_SIZE_UID_LENGTH            0x0AU   /**< UID length for triple size NFCID1 Type A compliant tags. */
#define PHAC_DISCLOOP_JEWEL_HR_LENGTH                         0x02U   /**< Type 1 Tag Header ROM byte length. */
/*@}*/

/**
* \name Type B default parameters.
*/
/*@{*/
#define PHAC_DISCLOOP_I3P3B_PUPI_LENGTH                       0x04U   /**< Device Identifier length for Type B tag. */
#define PHAC_DISCLOOP_I3P3B_MAX_ATQB_LENGTH                     13U   /**< Maximum Answer to RequestB (ATQB) Response length. */
/*@}*/

/**
* \name Type F default parameters.
*/
/*@{*/
#define PHAC_DISCLOOP_FELICA_IDM_LENGTH                       0x08U   /**< Manufacture ID (IDM) Length for FeliCa. */
#define PHAC_DISCLOOP_FELICA_PMM_LENGTH                       0x08U   /**< Manufacturer Parameter (PMm) length for FeliCa. */
#define PHAC_DISCLOOP_FELICA_RD_LENGTH                        0x02U   /**< Request Data (RD) length for FeliCa SENSF_RES. */
/*@}*/


/**
* \name Select Acknowledge (SAK) mask values for different Type A tags.
* \brief Application can use these mask values to detect the Type of tag
* from the SAK value.
*
* Usage:
\verbatim
  bTagType = (phacDiscLoop_Sw_DataParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak & 0x60U);
  bTagType = bTagType >> 5U;

  switch(bTagType)
  {
  case PHAC_DISCLOOP_TYPEA_TYPE2_TAG_CONFIG_MASK:
      printf("\n\t\tType: Type 2 Tag\n");
      break;
  case PHAC_DISCLOOP_TYPEA_TYPE4A_TAG_CONFIG_MASK:
      printf ("\n\t\tType: Type 4A Tag\n");
      break;
  case PHAC_DISCLOOP_TYPEA_TYPE_NFC_DEP_TAG_CONFIG_MASK:
      printf ("\n\t\tType: P2P\n");
      break;
  case PHAC_DISCLOOP_TYPEA_TYPE_NFC_DEP_TYPE4A_TAG_CONFIG_MASK:
      printf ("\n\t\tType: Type NFC_DEP and  4A Tag\n");
      break;
  default:
      break;
  }
\endverbatim
*/
/*@{*/
#define PHAC_DISCLOOP_TYPEA_T1T_ATQA1_MASK                      0x1FU /**< Mask for Type 1 tag ATQA Byte 1. */
#define PHAC_DISCLOOP_TYPEA_T1T_ATQA2_MASK                      0x0FU /**< Mask for determining whether a Type A tag is Type 1. Used along with byte 2 of ATQA. */
#define PHAC_DISCLOOP_TYPEA_TYPE2_TAG_CONFIG_MASK               0x00U /**< Mask for determining for Type 2 tag. */
#define PHAC_DISCLOOP_TYPEA_TYPE4A_TAG_CONFIG_MASK              0x01U /**< Mask for determining for Type 4A tag. */
#define PHAC_DISCLOOP_TYPEA_TYPE_NFC_DEP_TAG_CONFIG_MASK        0x02U /**< Mask for determining for NFC DEP Protocol. */
#define PHAC_DISCLOOP_TYPEA_TYPE_NFC_DEP_TYPE4A_TAG_CONFIG_MASK 0x03U /**< Mask for determining support for Type 4A and NFC DEP Protocol. */
/*@}*/

/**
 * \name Macros for checking set bit and set bit position.
 */
/*@{*/
/**
 * Logically AND value and mask, if the corresponding bit is set, then non
 * zero value is returned else zero is returned.
 *
 * Application can use this to check the detection technologies.
 *
 * Example (Check for Type B):\n
 *  - \ref phacDiscLoop_GetConfig (pDataParams, #PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTagsDetected)\n
 *  - PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, #PHAC_DISCLOOP_POS_BIT_MASK_B)\n
 * */
#define PHAC_DISCLOOP_CHECK_ANDMASK(value, mask)          \
    (((value) & (mask)) != 0U)

/**
 * Get the first set bit position starting from least significant bit.
 *
 * Application can use this to check the bit position of detected technology.
 *
 * Example (Activation of device/tag at index 0, when \ref phacDiscLoop_Run
 * returns #PHAC_DISCLOOP_MULTI_DEVICES_RESOLVED):\n
 *  - \ref phacDiscLoop_GetConfig (pDataParams, #PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTagsDetected);\n
 *  - PHAC_DISCLOOP_GET_BIT_POS(wTagsDetected, bTechType);\n
 *  - \ref phacDiscLoop_ActivateCard (pDataParams, (bTechType - 1), 0);
 * */
#define PHAC_DISCLOOP_GET_BIT_POS(x , pos)                 \
    {                                                      \
        (pos) = 0;                                         \
        while(0u != (x)){ (x) >>= 1U; ++(pos); }           \
        if ((pos) == 0U) {return (PH_ERR_INTERNAL_ERROR);} \
    }
/*@}*/

/**
* \brief Reader Library Profiles.
*
* \anchor phacDiscLoop_Profile
*
* This will be set and handled by the application.
*/
typedef enum {
    PHAC_DISCLOOP_PROFILE_UNKNOWN = 0,                                /**< Unknown. */
    PHAC_DISCLOOP_PROFILE_NFC,                                        /**< NFC Forum Profile. */
    PHAC_DISCLOOP_PROFILE_EMVCO,                                      /**< EMVCo Profile. */
    PHAC_DISCLOOP_PROFILE_ISO,                                        /**< ISO Profile. */
    PHAC_DISCLOOP_PROFILE_NFC_AND_ACTIVE                              /**< NFC Forum and Active Profile. */
 }phacDiscLoop_Profile_t;

/**
* \brief Discovery loop poll states to start with while in passive poll mode.\n
*
* \anchor phacDiscLoop_Sw_PollState
*
* Use \ref phacDiscLoop_SetConfig with #PHAC_DISCLOOP_CONFIG_NEXT_POLL_STATE
* option to configure these states. Default discovery loop starts with
* #PHAC_DISCLOOP_POLL_STATE_DETECTION in passive poll mode.
*/
/*@{*/
#define PHAC_DISCLOOP_POLL_STATE_UNKNOWN              ((uint16_t)0x0000U)     /**< Unknown. */
#define PHAC_DISCLOOP_POLL_STATE_DETECTION            ((uint16_t)0x0001U)     /**< Starts passive poll mode in Technology Detection stage. */
#define PHAC_DISCLOOP_POLL_STATE_COLLISION_RESOLUTION ((uint16_t)0x0002U)     /**< Starts passive poll mode in Collision Resolution stage. */
#define PHAC_DISCLOOP_POLL_STATE_REMOVAL              ((uint16_t)0x0003U)     /**< Starts passive poll mode in Card Removal stage (Only for EMVCo mode). */
/*@}*/

/**
* \brief Discovery entry points.\n
*
* \anchor phacDiscLoop_Sw_EntryPoints
* This is an input parameter to \ref phacDiscLoop_Run.
*/
typedef enum {
    PHAC_DISCLOOP_ENTRY_POINT_POLL = 0,                               /**< Starts discovery loop in poll mode. */
    PHAC_DISCLOOP_ENTRY_POINT_LISTEN                                  /**< Starts discovery loop in listen mode. */
}phacDiscLoop_Sw_EntryPoints_t;

/**
* \brief Technology bit position / sequence.\n
*
* \anchor phacDiscLoop_Sw_NfcTech
*/
typedef enum {
    PHAC_DISCLOOP_TECH_TYPE_A = 0,                                    /**< Type A bit position. */
    PHAC_DISCLOOP_TECH_TYPE_B,                                        /**< Type B bit position. */
    PHAC_DISCLOOP_TECH_TYPE_F212,                                     /**< Type F @ 212 kbps bit position. */
    PHAC_DISCLOOP_TECH_TYPE_F424,                                     /**< Type F @ 424 kbps bit position. */
    PHAC_DISCLOOP_TECH_TYPE_V,                                        /**< Type V bit position. */
    PHAC_DISCLOOP_TECH_TYPE_18000P3M3,                                /**< 18000P3M3 bit position. */
    PHAC_DISCLOOP_TECH_TYPE_UNKNOWN                                   /**< Unknown. */
}phacDiscLoop_Sw_NfcTech_t;

/**
* \brief Type A parameter structure
*
* \anchor phacDiscLoop_Sw_TypeA_Info
*/
typedef struct
{
    uint8_t bTotalTagsFound;                                          /**< Number of type A tags found. */
    /** Flag to indicate detected tag is T1T Tag in NFC profile, could be any jewel tag in other profile */
    uint8_t bT1TFlag;


    /** Structure info required for ISO/IEC 14443 communication */
    #ifdef  NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS
        struct /*phacDiscLoop_TypeA_I3P3*/
        {
            uint8_t aAtqa[PHAC_DISCLOOP_I3P3A_MAX_ATQA_LENGTH];       /**< ATQA received from ISO/IEC 14443A compliant device. */
            uint8_t aUid[PHAC_DISCLOOP_I3P3A_MAX_UID_LENGTH];         /**< Contains known UID bytes for the given cascade level. */
            uint8_t bUidSize;                                         /**< Size of the UID that was received from the PICC. */
            uint8_t aSak;                                             /**< Select acknowledge from the ISO/IEC 14443A type A card. */
            uint8_t bSleep;                                           /**< 1 if device is in SLEEP mode else 0. */
        }aTypeA_I3P3[PHAC_DISCLOOP_CFG_MAX_CARDS_SUPPORTED];
    #endif /* NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS */

    #if defined(NXPBUILD__PHAC_DISCLOOP_TYPEA_P2P_TAGS) || defined(NXPBUILD__PHAC_DISCLOOP_TYPEA_P2P_ACTIVE)
        /** Structure holding information on Type A P2P activation/communication */
        struct /*phacDiscLoop_Type_P2P_TAGS*/
        {
            uint8_t bDid;                                             /**< Device ID (DID) for ISO/IEC 18092 Initiator ATR command. */
            uint8_t bLri;                                             /**< Length reduction for ISO/IEC 18092 initiator. */
            uint8_t bNadEnable;                                       /**< Indicates whether Node Address (NAD) is enabled or not. */
            uint8_t bNad;                                             /**< Node address. Ignored if \a bNadEnabled is zero. */
            uint8_t *pGi;                                             /**< General bytes information. \a User should populate with appropriate address if \a bGiLength in non zero. */
            uint8_t bGiLength;                                        /**< Length of the general bytes. Should be zero if general bytes are not used. */
            uint8_t *pAtrRes;                                         /**< Attribute response; uint8_t[64]. */
            uint8_t bAtrResLength;                                    /**< Length of the ATR response. */
        }sTypeA_P2P;
    #endif

    #ifdef  NXPBUILD__PHAC_DISCLOOP_TYPEA_I3P4_TAGS
        /** Structure info required for ISO/IEC 14443-4 communication */

        struct /*phascDiscLoop_TypeA_I3P4*/
        {
            uint8_t bFsdi;                                            /**< Frame size integer. */
            uint8_t bCid;                                             /**< Card identifier. */
            uint8_t bNad;                                             /**< Node ADdress. */
            uint8_t bDri;                                             /**< Data Rate received by Initiator. */
            uint8_t bDsi;                                             /**< Data Rate send by Initiator. */
            uint8_t * pAts;                                           /**< Holds the answer to select response. */
        }sTypeA_I3P4;
    #endif /* NXPBUILD__PHAC_DISCLOOP_TYPEA_I3P4_TAGS */
} phacDiscLoop_Sw_TypeA_Info_t;

/**
* \brief Type B parameter structure
*
* \anchor phacDiscLoop_Sw_TypeB_Info
*/
typedef struct
{
    uint8_t bTotalTagsFound;                                          /**< Number of type B tags found. */
    uint8_t bAfiReq;                                                  /**< Application file Identifier. */
    uint8_t bExtendedAtqBbit;                                         /**< For coding b5 of PARAM of REQB:  b5=1 extended ATQB supported. */
    uint8_t bAdvFeaturesBit;                                          /**< For coding b6 of PARAM of REQB:  b6=1 advanced features supported (NFC Forum). */
    uint8_t bFsdi;                                                    /**< Frame size integer. Valid values: 0-12. */
    uint8_t bCid;                                                     /**< Card identifier. Valid values 0-14. */
    uint8_t bNad;                                                     /**< Node ADdress. */
    uint8_t bDri;                                                     /**< Data Rate receive (PICC --> PCD) integer. Valid values: 0-3. */
    uint8_t bDsi;                                                     /**< Data Rate send (PCD --> PICC) integer. Valid values: 0-3. */

    /** Structure for Type B (ISO/IEC 14443-B) tags */
    struct /*phacDiscLoop_TypeB_I3P3B*/
    {
        uint8_t aPupi[PHAC_DISCLOOP_I3P3B_PUPI_LENGTH];               /**< ISO/IEC 14443-3B Pseudo Unique PICC identifier. */
        uint8_t aAtqB[PHAC_DISCLOOP_I3P3B_MAX_ATQB_LENGTH];           /**< ATQB received from ISO/IEC 14443-3B compliant device. */
        uint8_t bAtqBLength;                                          /**< Length of the ATQB response. */
        uint8_t bSupportType4B;                                       /**< Indicates activated TypeB tag supports Type 4B Tag Platform or not. */
    } aTypeB_I3P3[PHAC_DISCLOOP_CFG_MAX_CARDS_SUPPORTED];

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEB_I3P4B_TAGS
    /** Type B ISO/IEC 14443-4B tag structure. */
    struct /*phacDiscLoop_TypeB_I3P4B*/
    {
        uint8_t bMbli;                                                /**< Maximum Buffer Length Index (MBLI) is available on bits [3:0]. */
        uint8_t bParam1;                                              /**< Param1 for type B Attrib: \n
                                                                              Bit 2 ... SOF \n
                                                                              Bit 3 ... EOF \n
                                                                              Bit 4-5 ... Minimum TR1 \n
                                                                              Bit 6-7 ... Minimum TR0. \n
                                                                      */
    } sTypeB_I3P4;
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEB_I3P4B_TAGS */
} phacDiscLoop_Sw_TypeB_Info_t;

/**
* \brief Type F parameter structure
*
* \anchor phacDiscLoop_Sw_TypeF_Info
*/
typedef struct
{

    uint8_t   bTotalTagsFound;                                        /**< Number of type F tags found. */
    uint8_t   aSystemCode[2];                                         /**< System code. */
    uint8_t   bTimeSlot;                                              /**< Maximum number of time slots. */

    struct /*phacDiscLoop_TypeF_Tag_Info*/
    {
        uint8_t aIDmPMm[PHAC_DISCLOOP_FELICA_IDM_LENGTH + PHAC_DISCLOOP_FELICA_PMM_LENGTH]; /**< Manufacture ID + PMm. */
        uint8_t aRD[2];                                                                     /**< Request Data received as response to FeliCa polling command. */
        uint8_t bBaud;                                                                      /**< Baud rate which card support. */
    }aTypeFTag[PHAC_DISCLOOP_CFG_MAX_CARDS_SUPPORTED];

#if defined(NXPBUILD__PHAC_DISCLOOP_TYPEF_P2P_TAGS) ||  defined(NXPBUILD__PHAC_DISCLOOP_TYPEF212_P2P_ACTIVE) || defined(NXPBUILD__PHAC_DISCLOOP_TYPEF424_P2P_ACTIVE)
       /** Structure holding information for Type F P2P communication */
       struct /*phacDiscLoop_TypeF_P2P_Info*/
       {
           uint8_t bDid;                                              /**< Device identifier. Disabled if set to 0 else should have a value between 1-14. */
           uint8_t bLri;                                              /**< Length reduction of Initiator. Should be a value in range 0-3. */
           uint8_t bNadEnable;                                        /**< Enable usage of node address. If set to zero NAD is disabled. */
           uint8_t bNad;                                              /**< Node address. */
           uint8_t *pGi;                                              /**< Optional general bytes. */
           uint8_t bGiLength;                                         /**< Length of the optional bytes. */
           uint8_t *pAtrRes;                                          /**< Pointer to the Attribute response. */
           uint8_t bAtrResLength;                                     /**< Attribute response length. */
       }sTypeF_P2P;
   #endif
} phacDiscLoop_Sw_TypeF_Info_t;

/**
* \brief Type V parameter structure
*
* \anchor phacDiscLoop_Sw_TypeV_Info
*/
typedef struct
{
    uint8_t bTotalTagsFound;                                          /**< Number of type V (ISO 15693) tags found. */
    uint8_t bFlag;                                                    /**< Request flags byte. */

    struct  /*phacDiscLoop_TypeV*/
    {
        uint8_t bDsfid;                                               /**< [Out] Data Storage Format Identifier. */
        uint8_t aUid[8];                                              /**< [Out] UID Received from card */
    }aTypeV[PHAC_DISCLOOP_CFG_MAX_VICINITY_CARDS_SUPPORTED];
}phacDiscLoop_Sw_TypeV_Info_t;

/**
* \brief 18000p3m3 parameter structure
*
* \anchor phacDiscLoop_I18000p3m3_Info
*/
typedef struct
{
    uint8_t bTotalTagsFound;                                          /**< Holds the number of tags detected */
    uint8_t bM;                                                       /**< Modulation; default #PHPAL_I18000P3M3_M_MANCHESTER_4 */
    uint8_t bDr;                                                      /**< Link frequency; default #PHPAL_I18000P3M3_LF_847KHZ */

    struct  /*phacDiscLoop_I18000p3m3*/
    {
        uint8_t aUii[58];                                             /**< ACK response from each tag */
        uint16_t wUiiLength;                                          /**< UII length in bits */
    }aI18000p3m3[PHAC_DISCLOOP_CFG_MAX_VICINITY_CARDS_SUPPORTED];
}phacDiscLoop_I18000p3m3_Info_t;

/**
* \brief Target parameter structure
*
* \anchor phacDiscLoop_TargetParams
*/
typedef struct
{
    uint8_t * pRxBuffer;                                              /**< Pointer to the Receive Buffer. */
    uint16_t wRxBufferLen;                                            /**< Length of Data Present in Receive Buffer. */
    uint16_t wProtParams;                                             /**< Frame mode and RX speed information. */
    uint8_t bRetryCount;                                              /**< Retry count for AUTOCOLL in case of failure. Default is 0. */
}phacDiscLoop_Sw_TargetParams_t;

/**
* \brief Discovery loop parameter structure
*
* \anchor phacDiscLoop_Sw_DataParams
*/
typedef struct
{
    /**
     * ID for this component, not to be modified.
     *
     * This is initialized with #PHAC_DISCLOOP_SW_ID in the initialization
     * function \ref phacDiscLoop_Sw_Init. This is used for validating the data
     * parameter structure and to make sure that discovery loop APIs are
     * called after initialization.
     * */
    uint16_t  wId;

    /**
     * Technologies to poll for in passive mode.
     *
     * #PHAC_DISCLOOP_POS_BIT_MASK_A         -> Detect Type A.\n
     * #PHAC_DISCLOOP_POS_BIT_MASK_B         -> Detect Type B.\n
     * #PHAC_DISCLOOP_POS_BIT_MASK_F212      -> Detect Type FeliCa @ 212 kbps.\n
     * #PHAC_DISCLOOP_POS_BIT_MASK_F424      -> Detect Type FeliCa @ 424 kbps.\n
     * #PHAC_DISCLOOP_POS_BIT_MASK_V         -> Detect Type V / ISO 15693.\n
     * #PHAC_DISCLOOP_POS_BIT_MASK_18000P3M3 -> Detect ISO 18000p3m3/ EPC Gen2.\n
     *
     * Use \ref phacDiscLoop_SetConfig with #PHAC_DISCLOOP_CONFIG_PAS_POLL_TECH_CFG
     * option to configure above values.
     * */
    uint8_t bPasPollTechCfg;

    /**
     * Technologies to poll for in active mode.
     *
     * #PHAC_DISCLOOP_ACT_POS_BIT_MASK_106 -> Detect active target @ 106 kbps\n
     * #PHAC_DISCLOOP_ACT_POS_BIT_MASK_212 -> Detect active target @ 212 kbps\n
     * #PHAC_DISCLOOP_ACT_POS_BIT_MASK_424 -> Detect active target @ 424 kbps\n
     *
     * Use \ref phacDiscLoop_SetConfig with #PHAC_DISCLOOP_CONFIG_ACT_POLL_TECH_CFG
     * option to configure above values.
     * */
    uint8_t bActPollTechCfg;

    /**
     * Bail out for various technologies in Passive mode.
     *
     * #PHAC_DISCLOOP_POS_BIT_MASK_A         -> Bail out for Type A.\n
     * #PHAC_DISCLOOP_POS_BIT_MASK_B         -> Bail out for Type B.\n
     * #PHAC_DISCLOOP_POS_BIT_MASK_F212      -> Bail out for Type FeliCa @ 212 kbps.\n
     * #PHAC_DISCLOOP_POS_BIT_MASK_F424      -> Bail out for Type FeliCa @ 424 kbps.\n
     * #PHAC_DISCLOOP_POS_BIT_MASK_V         -> Bail out for Type V / ISO 15693.\n
     * #PHAC_DISCLOOP_POS_BIT_MASK_18000P3M3 -> Bail out for ISO 18000p3m3/ EPC Gen2.\n
     *
     * Use \ref phacDiscLoop_SetConfig with #PHAC_DISCLOOP_CONFIG_BAIL_OUT option
     * to configure above values.
     * */
    uint8_t bPasPollBailOut;

    /**
     * Technologies to listen for in passive listen mode.
     *
     * #PHAC_DISCLOOP_POS_BIT_MASK_A         -> Listen for Type A.\n
     * #PHAC_DISCLOOP_POS_BIT_MASK_B         -> Listen for Type B. [Reserved. Not supported currently] \n
     * #PHAC_DISCLOOP_POS_BIT_MASK_F212      -> Listen for Type FeliCa @ 212 kbps.\n
     * #PHAC_DISCLOOP_POS_BIT_MASK_F424      -> Listen for Type FeliCa @ 424 kbps.\n
     * #PHAC_DISCLOOP_POS_BIT_MASK_V         -> Listen for Type V / ISO 15693.[Reserved. Not supported currently]\n
     * #PHAC_DISCLOOP_POS_BIT_MASK_18000P3M3 -> Listen for ISO 18000p3m3/ EPC Gen2.[Reserved. Not supported currently]\n
     *
     * Use \ref phacDiscLoop_SetConfig with #PHAC_DISCLOOP_CONFIG_PAS_LIS_TECH_CFG
     * option to configure above values.
     * */
    uint8_t bPasLisTechCfg;

    /**
     * Technologies to listen for in active listen mode.
     *
     * #PHAC_DISCLOOP_POS_BIT_MASK_A         -> Listen for Type A.\n
     * #PHAC_DISCLOOP_POS_BIT_MASK_B         -> Listen for Type B.[Reserved. Not supported currently] \n
     * #PHAC_DISCLOOP_POS_BIT_MASK_F212      -> Listen for Type FeliCa @ 212 kbps.\n
     * #PHAC_DISCLOOP_POS_BIT_MASK_F424      -> Listen for Type FeliCa @ 424 kbps.\n
     * #PHAC_DISCLOOP_POS_BIT_MASK_V         -> Listen for Type V / ISO 15693.[Reserved. Not supported currently]\n
     * #PHAC_DISCLOOP_POS_BIT_MASK_18000P3M3 -> Listen for ISO 18000p3m3/ EPC Gen2.[Reserved. Not supported currently]\n
     *
     * Use \ref phacDiscLoop_SetConfig with #PHAC_DISCLOOP_CONFIG_ACT_LIS_TECH_CFG
     * option to configure above values.
     * */
    uint8_t bActLisTechCfg;

    /**
     * Discovery loop stage to start with while in poll mode.
     *
     * #PHAC_DISCLOOP_POLL_STATE_DETECTION            -> Start with detection phase as per NFC Forum Activity Specification.\n
     * #PHAC_DISCLOOP_POLL_STATE_COLLISION_RESOLUTION -> Start with collision resolution as per NFC Forum Activity Specification.\n
     * #PHAC_DISCLOOP_POLL_STATE_REMOVAL              -> Start with card removal phase (in EMVCo mode).\n
     *
     * When \ref phacDiscLoop_Run gives #PHAC_DISCLOOP_MULTI_TECH_DETECTED in poll mode, application
     * shall select one technology and start poll mode again in collision resolution state.
     *
     * Use \ref phacDiscLoop_SetConfig with #PHAC_DISCLOOP_CONFIG_NEXT_POLL_STATE
     * option to configure above states.
     * */
    uint16_t  bPollState;

    /**
     * Contains device limits for different technologies in passive poll mode.
     *
     * #PHAC_DISCLOOP_CONFIG_TYPEA_DEVICE_LIMIT     -> Device limit for Type A.\n
     * #PHAC_DISCLOOP_CONFIG_TYPEB_DEVICE_LIMIT     -> Device limit for Type B.\n
     * #PHAC_DISCLOOP_CONFIG_TYPEF_DEVICE_LIMIT     -> Device limit for Type F.\n
     * #PHAC_DISCLOOP_CONFIG_TYPEV_DEVICE_LIMIT     -> Device limit for Type V.\n
     * #PHAC_DISCLOOP_CONFIG_18000P3M3_DEVICE_LIMIT -> Device limit for ISO 18000p3m3/ EPC Gen2.\n
     *
     * Use \ref phacDiscLoop_SetConfig with above options to configure device limit.
     * Maximum device limit is #PHAC_DISCLOOP_CFG_MAX_CARDS_SUPPORTED.
     * */
    uint8_t baPasConDevLim[PHAC_DISCLOOP_PASS_POLL_MAX_TECHS_SUPPORTED];

    /**
     * Contains guard times for different technologies in passive poll mode.
     *
     * #PHAC_DISCLOOP_CONFIG_GTA_VALUE_US         -> Guard time for Type A.\n
     * #PHAC_DISCLOOP_CONFIG_GTB_VALUE_US         -> Guard time for Type B.\n
     * #PHAC_DISCLOOP_CONFIG_GTFB_VALUE_US        -> Guard time for Type F. Guard time is applied when polling for Type B is preceded by polling for Type F.\n
     * #PHAC_DISCLOOP_CONFIG_GTBF_VALUE_US        -> Guard time for Type F. Guard time is applied when polling for Type F is preceded by polling for Type B.\n
     * #PHAC_DISCLOOP_CONFIG_GTV_VALUE_US         -> Guard time for Type V.\n
     * #PHAC_DISCLOOP_CONFIG_GT18000P3M3_VALUE_US -> Guard time for ISO 18000p3m3/ EPC Gen2.\n
     *
     * Use \ref phacDiscLoop_SetConfig with above options to configure guard time.
     * */
    uint16_t waPasPollGTimeUs[PHAC_DISCLOOP_PASS_POLL_MAX_TECHS_SUPPORTED];

    /**
     * Contains guard times for different technologies in Active poll mode.
     *
     * #PHAC_DISCLOOP_CONFIG_GT_ACTIVE_US            -> Guard time for Active mode polling
     * Default value is 5100 micro seconds.
     *
     * Use \ref phacDiscLoop_SetConfig with above options to configure guard time.
     * */
    uint16_t wActPollGTimeUs;

    /**
     * Flag to indicate whether collision is pending or not.
     *
     * #PHAC_DISCLOOP_POS_BIT_MASK_A         -> Type A.\n
     * #PHAC_DISCLOOP_POS_BIT_MASK_B         -> Type B.\n
     * #PHAC_DISCLOOP_POS_BIT_MASK_F212      -> FeliCa @ 212 kbps.\n
     * #PHAC_DISCLOOP_POS_BIT_MASK_F424      -> FeliCa @ 424 kbps.\n
     * #PHAC_DISCLOOP_POS_BIT_MASK_V         -> Type V / ISO 15693.\n
     * #PHAC_DISCLOOP_POS_BIT_MASK_18000P3M3 -> ISO 18000p3m3/ EPC Gen2.\n
     *
     * Use \ref phacDiscLoop_GetConfig with #PHAC_DISCLOOP_CONFIG_COLLISION_PENDING
     * option to get collision pending status.
     * */
    uint8_t bCollPend;

    /**
     * Enable/Disable Low Power Card Detection.
     *
     * Use \ref phacDiscLoop_SetConfig with #PHAC_DISCLOOP_CONFIG_ENABLE_LPCD option
     * to #PH_ON / #PH_OFF LPCD.
     * */
    uint8_t bLpcdEnabled;

    /**
     * Enable/Disable anti-collision for Type A for passive poll mode.
     *
     * Use \ref phacDiscLoop_SetConfig with #PHAC_DISCLOOP_CONFIG_ANTI_COLL option
     * to #PH_ON / #PH_OFF anti-collision.
     * */
    uint8_t bUseAntiColl;

    /**
     * Operation mode of discovery loop.
     *
     * #RD_LIB_MODE_EMVCO -> Run in EMVCO mode as per the EMVCo 2.5 Digital specification.\n
     * #RD_LIB_MODE_NFC   -> Run in NFC mode as per NFC Forum Activity Specification 1.1. This is the default mode.\n
     *
     * Use \ref phacDiscLoop_SetConfig with #PHAC_DISCLOOP_CONFIG_OPE_MODE option
     * to configure above modes.
     * */
    uint8_t bOpeMode;

    /**
     * Number of cards detected in passive poll mode.
     *
     * Use \ref phacDiscLoop_GetConfig with #PHAC_DISCLOOP_CONFIG_NR_TAGS_FOUND
     * option to get number of tags.
     * */
    uint8_t bNumOfCards;

    /**
     * Current FeliCa baud rate.
     * */
    uint8_t bFelicaBaud;

    /**
     * Supported NFC Activity version.
     *
     * #PHAC_DISCLOOP_NFC_ACTIVITY_VERSION_1_0 -> Run in NFC Forum Activity 1.0 mode.\n
     * #PHAC_DISCLOOP_NFC_ACTIVITY_VERSION_1_1 -> Run in NFC Forum Activity 1.1 mode. This is the default.\n
     *
     * Use \ref phacDiscLoop_SetConfig with #PHAC_DISCLOOP_CONFIG_ACTIVITY_VERSION
     * option to configure above modes.
     * */
    uint8_t bNfcActivityVersion;

    /**
     * Detected technologies during poll mode.
     *
     * Passive technologies:\n
     * #PHAC_DISCLOOP_POS_BIT_MASK_A         -> Detected Type A.\n
     * #PHAC_DISCLOOP_POS_BIT_MASK_B         -> Detected Type B.\n
     * #PHAC_DISCLOOP_POS_BIT_MASK_F212      -> Detected FeliCa @ 212 kbps.\n
     * #PHAC_DISCLOOP_POS_BIT_MASK_F424      -> Detected FeliCa @ 424 kbps.\n
     * #PHAC_DISCLOOP_POS_BIT_MASK_V         -> Detected Type V / ISO 15693.\n
     * #PHAC_DISCLOOP_POS_BIT_MASK_18000P3M3 -> Detected ISO 18000p3m3/ EPC Gen2.\n
     *
     * Active technologies:\n
     * #PHAC_DISCLOOP_ACT_POS_BIT_MASK_106   -> Detected active target @ 106 kbps.\n
     * #PHAC_DISCLOOP_ACT_POS_BIT_MASK_212   -> Detected active target @ 212 kbps.\n
     * #PHAC_DISCLOOP_ACT_POS_BIT_MASK_424   -> Detected active target @ 424 kbps.\n
     *
     * Use \ref phacDiscLoop_GetConfig with #PHAC_DISCLOOP_CONFIG_TECH_DETECTED
     * option to get detected technologies.
     * */
    uint8_t bDetectedTechs;

    /**
     * Hold error received from lower layer when \ref phacDiscLoop_Run returns #PHAC_DISCLOOP_FAILURE.
     *
     * Use \ref phacDiscLoop_GetConfig with #PHAC_DISCLOOP_CONFIG_ADDITIONAL_INFO
     * option to get error from lower layer.
     * */
    phStatus_t wErrorCode;

    /**
     * Pointer to the HAL data parameter.
     *
     * Application should initialize this pointer with address of HAL data
     * parameter structure.
     * */
    void * pHalDataParams;

    /**
     * Pointer to the 14443P3A layer PAL data parameter.
     *
     * Application should initialize this pointer with address of 14443P3A PAL
     * data parameter structure.
     * */
    void * pPal1443p3aDataParams;

    /**
     * Pointer to the FeliCa PAL data parameter.
     *
     * Application should initialize this pointer with address of FeliCa PAL
     * data parameter structure.
     * */
    void * pPalFelicaDataParams;

    /**
     * Pointer to the 14443P3B PAL data parameter.
     *
     * Application should initialize this pointer with address of 14443P3B PAL
     * data parameter structure.
     * */
    void * pPal1443p3bDataParams;

    /**
     * Pointer to the 14443P4 PAL data parameter.
     *
     * Application should initialize this pointer with address of 14443P4 PAL
     * data parameter structure.
     * */
    void * pPal14443p4DataParams;

    /**
     * Pointer to the 14443P4A PAL data parameter.
     *
     * Application should initialize this pointer with address of 14443P4A PAL
     * data parameter structure.
     * */
    void * pPal1443p4aDataParams;

    /**
     * Pointer to the 15693 PAL data parameter.
     *
     * Application should initialize this pointer with address of 15693 PAL
     * data parameter structure.
     * */
    void * pPalSli15693DataParams;

    /**
     * Pointer to the 18000P3M3 PAL data parameter.
     *
     * Application should initialize this pointer with address of 18000P3M3 PAL
     * data parameter structure.
     * */
    void * pPal18000p3m3DataParams;

    /**
     * Pointer to the 18000P3M3 AL data parameter.
     *
     * Application should initialize this pointer with address of 18000P3M3 AL
     * data parameter structure.
     * */
    void * pAl18000p3m3DataParams;

    /**
     * Pointer to 18092MPI PAL data parameter.
     *
     * Application should initialize this pointer with address of 18092MPI PAL
     * data parameter structure.
     * */
    void * pPal18092mPIDataParams;
    /**
     * Pointer to T1T AL data parameter.
     *
     * Application should initialize this pointer with address of T1T AL data
     * parameter structure.
     * */
    void * pAlT1TDataParams;

#if defined(NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS) || defined(NXPBUILD__PHAC_DISCLOOP_TYPEA_P2P_ACTIVE)
    /**
     * Information gathered from TYPE A devices/tags in poll mode.
     *
     * Application can access this directly to get information like UID, UID size,
     * SAK etc. of all Type A devices/tags detected in poll mode.
     * */
    phacDiscLoop_Sw_TypeA_Info_t sTypeATargetInfo;
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS */

#if defined(NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS) || defined(NXPBUILD__PHAC_DISCLOOP_TYPEF212_P2P_ACTIVE) || defined(NXPBUILD__PHAC_DISCLOOP_TYPEF424_P2P_ACTIVE)
    /**
     * Information gathered from TYPE F devices/tags in poll mode.
     *
     * Application can access this directly to get information like ID, Baud rate,
     * etc. of all Type F devices/tags detected in poll mode.
     * */
    phacDiscLoop_Sw_TypeF_Info_t sTypeFTargetInfo;
#endif  /* NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS */

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEB_TAGS
    /**
     * Information gathered from TYPE B devices/tags in poll mode.
     *
     * Application can access this directly to get information like PUPI, ATQB,
     * ATQB length etc. of all Type B devices/tags detected in poll mode.
     * */
    phacDiscLoop_Sw_TypeB_Info_t sTypeBTargetInfo;
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEB_TAGS */

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEV_TAGS
    /**
     * Information gathered from TYPE V (15693) devices/tags in poll mode.
     *
     * Application can access this directly to get information like UID, DSFID,
     * etc. of all Type V devices/tags detected in poll mode.
     * */
    phacDiscLoop_Sw_TypeV_Info_t sTypeVTargetInfo;
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEV_TAGS */

#ifdef NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS
    /**
     * Information gathered from 18000P3M3 devices/tags in poll mode.
     *
     * Application can access this directly to get information like StoredCRC,
     * etc. of all 18000P3M3 devices/tags detected in poll mode.
     * */
    phacDiscLoop_I18000p3m3_Info_t sI18000p3m3TargetInfo;
#endif /* NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS */

#ifdef NXPBUILD__PHAC_DISCLOOP_TARGET
    /**
     * Information gathered from listen mode.
     *
     * Application can access this directly to get information like Request
     * received, Request length etc. from AUTOCOLL in listen mode.
     * */
    phacDiscLoop_Sw_TargetParams_t sTargetParams;
#endif  /* NXPBUILD__PHAC_DISCLOOP_TARGET  */
}phacDiscLoop_Sw_DataParams_t;

/**
 * \brief Initialize discovery  loop.
 * \anchor initialization
 * This should be called before using any of the discovery loop functions to do
 * the initialization of discovery loop component. This configures discovery loop
 * with default configurations. Refer \ref configurations "configuration options"
 * for supported configurations and their default values.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval #PH_ERR_INVALID_PARAMETER Invalid option/response received.
 * \retval other Other Depending on implementation and underlying component.
 */
phStatus_t phacDiscLoop_Sw_Init(
    phacDiscLoop_Sw_DataParams_t * pDataParams,    /**< [In] Pointer to discovery loop parameter structure. */
    uint16_t wSizeOfDataParams,                    /**< [In] Specifies the size of the discovery data parameter structure. */
    void * pHalDataParams                          /**< [In] Pointer to HAL parameter structure. */
    );
/** @} */
#endif /* NXPBUILD__PHAC_DISCLOOP_SW */

#ifdef NXPBUILD__PHAC_DISCLOOP
/** \defgroup phacDiscLoop Discovery Loop
*  \brief Discovery loop component as per NFC Forum Activity Specification 1.1 and EMVCo Digital 2.6 specification.
*
*  Discovery loop implements poll mode based on NFC Activity Specification 1.1
*  (backward compatibility with version 1.0) and EMVCo Specification 2.6. Listen
*  mode is implemented based on NFC Activity Specification 1.1. Support for Type V
*  (ISO 15693) polling is included based on NFC Forum draft specification. ISO 18000-3
*  Mode 3 (EPC Gen2) is supported as a proprietary technology in poll mode.
*
*<b>Poll_Mode</b>
*<em>This scenario depicts sequence for Poll_Mode.</em>
*\image html Poll_Mode.png
*
*<b>Listen_Mode</b>
*<em>This scenario depicts sequence for Listen_Mode.</em>
*\image html Listen_Mode.png
* @{
*/

/**
* \name Configuration Types
* \anchor configurations
* \brief These options shall be used along with \ref phacDiscLoop_SetConfig and
* \ref phacDiscLoop_GetConfig to set/get discovery loop configurations.
*/
/*@{*/
/**
 * Set/Get the guard time for Type A technology in microseconds for passive poll
 * mode.
 *
 * Default guard time is set to 5200 microseconds for Type A.
 * */
#define PHAC_DISCLOOP_CONFIG_GTA_VALUE_US                     0x00U

/**
 * Set/Get the guard time for Type B technology in microseconds for passive poll
 * mode.
 *
 * Default guard time is set to 5200 microseconds for Type B.
 * */
#define PHAC_DISCLOOP_CONFIG_GTB_VALUE_US                     0x01U

/**
 * Set/Get the guard time for Type F technology in microseconds for passive poll mode
 * when polling for Type B is preceded by polling for Type F.
 *
 * Default guard time is set to 20400 microseconds for Type F.
 * */
#define PHAC_DISCLOOP_CONFIG_GTFB_VALUE_US                     0x02U

/**
 * Set/Get the guard time for Type F technology in microseconds for passive poll mode
 * when polling for Type F is preceded by polling for Type B.
 *
 * Default guard time is set to 15300 microseconds for Type F.
 * */
#define PHAC_DISCLOOP_CONFIG_GTBF_VALUE_US                     0x03U

/**
 * Set/Get the guard time for Type V technology in microseconds for passive poll
 * mode.
 *
 * Default guard time is set to 5200 microseconds for Type V.
 * */
#define PHAC_DISCLOOP_CONFIG_GTV_VALUE_US                     0x04U

/**
 * Set/Get the guard time for 18000P3M3 technology in microseconds for passive
 * poll mode.
 *
 * Default guard time is set to 10000 microseconds for 18000P3M3.
 * */
#define PHAC_DISCLOOP_CONFIG_GT18000P3M3_VALUE_US             0x05U

/**
 * Set/Get the guard time for Active poll mode in microseconds
 *
 * Default guard time is set to 5100 microseconds.
 * */
#define PHAC_DISCLOOP_CONFIG_GT_ACTIVE_US                     0x06U

/**
 * Set/Get the number of Type A devices/tags that can be detected in passive
 * poll mode.
 *
 * Default device limit is 1 for Type A. Maximum supported device limit is
 * #PHAC_DISCLOOP_CFG_MAX_CARDS_SUPPORTED.
 * */
#define PHAC_DISCLOOP_CONFIG_TYPEA_DEVICE_LIMIT               0x10U

/**
 * Set/Get the number of Type B devices/tags that can be detected in passive
 * poll mode.
 *
 * Default device limit is 1 for Type B. Maximum supported device limit is
 * #PHAC_DISCLOOP_CFG_MAX_CARDS_SUPPORTED.
 * */
#define PHAC_DISCLOOP_CONFIG_TYPEB_DEVICE_LIMIT               0x11U

/**
 * Set/Get the number of Type F devices/tags that can be detected in passive
 * poll mode.
 *
 * Default device limit is 1 for Type F. Maximum supported device limit is
 * #PHAC_DISCLOOP_CFG_MAX_CARDS_SUPPORTED.
 * */
#define PHAC_DISCLOOP_CONFIG_TYPEF_DEVICE_LIMIT               0x12U

/**
 * Set/Get the number of Type V tags that can be detected in passive
 * poll mode.
 *
 * Default device limit is 1 for Type V. Maximum supported device limit is
 * #PHAC_DISCLOOP_CFG_MAX_CARDS_SUPPORTED.
 * */
#define PHAC_DISCLOOP_CONFIG_TYPEV_DEVICE_LIMIT               0x13U

/**
 * Set/Get the number of 18000P3M3 tags that can be detected in passive
 * poll mode.
 *
 * Default device limit is 1 for 18000P3M3. Maximum supported device limit is
 * #PHAC_DISCLOOP_CFG_MAX_CARDS_SUPPORTED.
 * */
#define PHAC_DISCLOOP_CONFIG_18000P3M3_DEVICE_LIMIT           0x14U

/**
 * Returns the number of Type A devices/tags found in passive poll mode.
 *
 * This gives the number of Type A devices/tags found after \ref phacDiscLoop_Run
 * in poll mode (#PHAC_DISCLOOP_ENTRY_POINT_POLL).
 * */
#define PHAC_DISCLOOP_CONFIG_TYPEA_TAGS_FOUND                 0x20U

/**
 * Returns the number of Type B devices/tags found in passive poll mode.
 *
 * This gives the number of Type B devices/tags found after \ref phacDiscLoop_Run
 * in poll mode (#PHAC_DISCLOOP_ENTRY_POINT_POLL).
 * */
#define PHAC_DISCLOOP_CONFIG_TYPEB_TAGS_FOUND                 0x21U

/**
 * Returns the number of Type F devices/tags found in passive poll mode.
 *
 * This gives the number of Type F devices/tags found after \ref phacDiscLoop_Run
 * in poll mode (#PHAC_DISCLOOP_ENTRY_POINT_POLL).
 * */
#define PHAC_DISCLOOP_CONFIG_TYPEF_TAGS_FOUND                 0x22U

/**
 * Returns the number of Type V tags found in passive poll mode.
 *
 * This gives the number of Type V tags found after \ref phacDiscLoop_Run
 * in poll mode (#PHAC_DISCLOOP_ENTRY_POINT_POLL).
 * */
#define PHAC_DISCLOOP_CONFIG_TYPEV_TAGS_FOUND                 0x23U

/**
 * Returns the number of 18000P3M3 tags found in passive poll mode.
 *
 * This gives the number of 18000P3M3 tags found after \ref phacDiscLoop_Run
 * in poll mode (#PHAC_DISCLOOP_ENTRY_POINT_POLL).
 * */
#define PHAC_DISCLOOP_CONFIG_18000P3M3_TAGS_FOUND             0x24U

/**
 * Returns if the activated Type B device/tag is compliant with ISO/IEC 14443-4 or not.
 *
 * In ISO mode this is used to take decision if tag is compliant with ISO/IEC 14443-4.
 * */
#define PHAC_DISCLOOP_CONFIG_TYPEB_SUPPORT_TYPE4B             0x25U

/**
 * Set/Get Application Family Identifier (AFI) to be used for Type B in
 * passive poll mode.
 *
 * Default is 0.
 * */
#define PHAC_DISCLOOP_CONFIG_TYPEB_AFI_REQ                    0x30U

/**
 * Enable or disable (#PH_ON or #PH_OFF) extended ATQB (Answer to Request B)
 * option for Type B during passive poll mode.
 *
 * Default is #PH_OFF.
 * */
#define PHAC_DISCLOOP_CONFIG_TYPEB_EXTATQB                    0x31U

/**
 * Sets the Frame Size Device Integer (FSDI) value for the Type B tags in
 * passive poll mode.
 *
 * Default is 0x08.
 * */
#define PHAC_DISCLOOP_CONFIG_TYPEB_FSDI                       0x32U

/**
 * Sets the Card Identifier (CID) value for the Type B tags in passive poll mode.
 *
 * Default is 0.
 * */
#define PHAC_DISCLOOP_CONFIG_TYPEB_CID                        0x33U

/**
 * Sets the Node ADdress (NAD) value for the Type B tags in passive poll mode.
 *
 * Default is 0.
 * */
#define PHAC_DISCLOOP_CONFIG_TYPEB_NAD                        0x34U

/**
 * Sets the Divisor Receive Integer (DRI) for Type B tags in passive poll mode.
 *
 * Default is 0.
 * */
#define PHAC_DISCLOOP_CONFIG_TYPEB_DRI                        0x35U

/**
 * Sets the Divisor Send Integer (DSI) for Type B tags in passive poll mode.
 *
 * Default is 0.
 * */
#define PHAC_DISCLOOP_CONFIG_TYPEB_DSI                        0x36U

/**
 * Sets the Frame Size Device Integer (FSDI) for the 14443P4A tags in passive
 * poll mode.
 *
 * Default is 0x08.
 * */
#define PHAC_DISCLOOP_CONFIG_TYPEA_I3P4_FSDI                  0x40U

/**
 * Sets the Card Identifier (CID) for the 14443P4A tags in passive poll mode.
 *
 * Default is 0.
 * */
#define PHAC_DISCLOOP_CONFIG_TYPEA_I3P4_CID                   0x41U

/**
 * Sets the Node ADdress (NAD) for the 14443P4A tags in passive poll mode.
 *
 * Default is 0.
 * */
#define PHAC_DISCLOOP_CONFIG_TYPEA_I3P4_NAD                   0x42U

/**
 * Sets the Divisor Receive Integer (DRI) for the 14443P4A tags in passive
 * poll mode.
 *
 * Default is 0.
 * */
#define PHAC_DISCLOOP_CONFIG_TYPEA_I3P4_DRI                   0x43U

/**
 * Sets the Divisor Send Integer (DSI) for the 14443P4A tags in passive
 * poll mode.
 *
 * Default is 0.
 * */
#define PHAC_DISCLOOP_CONFIG_TYPEA_I3P4_DSI                   0x44U

/**
 * Sets the Device Identifier (DID) for Type A P2P devices in passive
 * poll mode.
 *
 * Default is 0.
 * */
#define PHAC_DISCLOOP_CONFIG_TYPEA_P2P_DID                    0x45U

/**
 * Sets the Initiator Length Reduction field (LRI) for Type A P2P devices in
 * passive poll mode.
 *
 * Default is 0.
 * */
#define PHAC_DISCLOOP_CONFIG_TYPEA_P2P_LRI                    0x46U

/**
 * Enable/Disable Node Addressing (NAD) (#PH_ON or #PH_OFF) for Type A P2P
 * devices in passive poll mode.
 *
 * Default is #PH_OFF.
 * */
#define PHAC_DISCLOOP_CONFIG_TYPEA_P2P_NAD_ENABLE             0x47U

/**
 * Sets the Node Address (NAD) for Type A P2P devices in passive poll mode.
 *
 * Default is 0.
 * */
#define PHAC_DISCLOOP_CONFIG_TYPEA_P2P_NAD                    0x48U

/**
 * Sets the length of the general bytes for Type A P2P devices in passive poll
 * mode.
 *
 * Default is 0.
 * */
#define PHAC_DISCLOOP_CONFIG_TYPEA_P2P_GI_LEN                 0x49U

/**
 * Get ATR response length from Type A P2P devices in passive poll mode.
 * */
#define PHAC_DISCLOOP_CONFIG_TYPEA_P2P_ATR_RES_LEN            0x50U

/**
 * Sets the Device Identifier (DID) for Type F P2P devices in passive
 * poll mode.
 *
 * Default is 0.
 * */
#define PHAC_DISCLOOP_CONFIG_TYPEF_P2P_DID                    0x51U

/**
 * Sets the Initiator Length Reduction field (LRI) for Type F P2P devices in
 * passive poll mode.
 *
 * Default is 0.
 * */
#define PHAC_DISCLOOP_CONFIG_TYPEF_P2P_LRI                    0x52U

/**
 * Enable/Disable Node Addressing (NAD) (#PH_ON or #PH_OFF) for Type F P2P
 * devices in passive poll mode.
 *
 * Default is #PH_OFF.
 * */
#define PHAC_DISCLOOP_CONFIG_TYPEF_P2P_NAD_ENABLE             0x53U

/**
 * Sets the Node Address (NAD) for Type F P2P devices in passive poll mode.
 *
 * Default is 0.
 * */
#define PHAC_DISCLOOP_CONFIG_TYPEF_P2P_NAD                    0x54U

/**
 * Sets the length of the general bytes for Type F P2P devices in passive poll
 * mode.
 *
 * Default is 0.
 * */
#define PHAC_DISCLOOP_CONFIG_TYPEF_P2P_GI_LEN                 0x55U

/**
 * Get ATR response length from Type A P2P devices in passive poll mode.
 * */
#define PHAC_DISCLOOP_CONFIG_TYPEF_P2P_ATR_RES_LEN            0x56U
/**
 * Set/Get the baud rate of FeliCa in passive poll mode.
 *
 * #PHAC_DISCLOOP_CON_BITR_212 - FeliCa @ 212 kbps.\n
 * #PHAC_DISCLOOP_CON_BITR_424 - FeliCa @ 424 kbps.\n
 * */
#define PHAC_DISCLOOP_CONFIG_TYPEF_CUR_BAUD                   0x57U

/**
 * Configure ISO 15693 for high/low RX data rate
 *
 * #PHHAL_HW_RF_RX_DATARATE_HIGH\n
 * #PHHAL_HW_RF_RX_DATARATE_LOW\n
 *
 * Default is #PHHAL_HW_RF_RX_DATARATE_HIGH.
 * */
#define PHAC_DISCLOOP_CONFIG_TYPEV_RX_DATA_RATE               0x60U

/**
 * Configure ISO 18000p3m3 modulation/sub-carrier.
 *
 * #PHPAL_I18000P3M3_M_MANCHESTER_2 - Use two sub-carrier.\n
 * #PHPAL_I18000P3M3_M_MANCHESTER_4 - Use four sub-carrier.\n
 *
 * Default is #PHPAL_I18000P3M3_M_MANCHESTER_4.
 * */
#define PHAC_DISCLOOP_CONFIG_I18000P3M3_MODULATION            0x70U

/**
 * Configure ISO 18000p3m3 link frequency.
 *
 * #PHPAL_I18000P3M3_LF_423KHZ - 423kHz (fc/32).\n
 * #PHPAL_I18000P3M3_LF_847KHZ - 847kHz (fc/16).\n
 *
 * Default is #PHPAL_I18000P3M3_LF_847KHZ.
 * */
#define PHAC_DISCLOOP_CONFIG_I18000P3M3_FREQUENCY             0x71U

/**
 * Enable/Disable Low Power Card Detection (LPCD) in passive poll mode (#PH_ON
 * or #PH_OFF).
 *
 * Default is #PH_OFF.
 * */
#define PHAC_DISCLOOP_CONFIG_ENABLE_LPCD                      0x80U

/**
 * Returns the number of devices/tags found in passive poll mode.
 *
 * This gives the number of devices/tags found after \ref phacDiscLoop_Run
 * in poll mode (#PHAC_DISCLOOP_ENTRY_POINT_POLL).
 * */
#define PHAC_DISCLOOP_CONFIG_NR_TAGS_FOUND                    0x81U

/**
 * Enable/Disable anti-collision support for Type A technology (#PH_ON or #PH_OFF).
 *
 * Default is #PH_ON.
 * */
#define PHAC_DISCLOOP_CONFIG_ANTI_COLL                        0x88U

/**
 * Get the actual error code from lower layer when discovery loop returns
 * #PHAC_DISCLOOP_FAILURE.
 * */
#define PHAC_DISCLOOP_CONFIG_ADDITIONAL_INFO                  0x85U

/**
 * Configure operation mode of discovery loop.
 *
 * #RD_LIB_MODE_EMVCO -> Run in EMVCO mode.\n
 * #RD_LIB_MODE_NFC -> Run in NFC mode. This is the default mode.\n
 *
 * Default is #RD_LIB_MODE_NFC.
 *
 * Note: Only EMVCo, NFC and ISO modes configuration is allowed from Discovery loop set configuration.
 * */
#define PHAC_DISCLOOP_CONFIG_OPE_MODE                         0x82U

/**
 * Get collision pending status of polled technologies.
 *
 * #PHAC_DISCLOOP_POS_BIT_MASK_A         -> Collision pending for Type A.\n
 * #PHAC_DISCLOOP_POS_BIT_MASK_B         -> Collision pending for Type B.\n
 * #PHAC_DISCLOOP_POS_BIT_MASK_F212      -> Collision pending for FeliCa @ 212 kbps.\n
 * #PHAC_DISCLOOP_POS_BIT_MASK_F424      -> Collision pending for FeliCa @ 424 kbps.\n
 * #PHAC_DISCLOOP_POS_BIT_MASK_V         -> Collision pending for Type V / ISO 15693.\n
 * #PHAC_DISCLOOP_POS_BIT_MASK_18000P3M3 -> Collision pending for ISO 18000p3m3/ EPC Gen2.\n
 * */
#define PHAC_DISCLOOP_CONFIG_COLLISION_PENDING                0x83U

/**
 * Set the discovery loop state to start with while in poll mode.
 *
 * Refer \ref phacDiscLoop_Sw_PollState for all supported states.\n
 *
 * #PHAC_DISCLOOP_POLL_STATE_DETECTION            -> Start with detection phase.\n
 * #PHAC_DISCLOOP_POLL_STATE_COLLISION_RESOLUTION -> Start with collision resolution.\n
 * #PHAC_DISCLOOP_POLL_STATE_REMOVAL              -> Start with card removal phase (shall be used only in EMVCo mode).\n
 *
 * Default is #PHAC_DISCLOOP_POLL_STATE_DETECTION. When \ref phacDiscLoop_Run gives
 * #PHAC_DISCLOOP_MULTI_TECH_DETECTED in poll mode, application shall select one technology
 * and start poll mode again in #PHAC_DISCLOOP_POLL_STATE_COLLISION_RESOLUTION state.
 * */
#define PHAC_DISCLOOP_CONFIG_NEXT_POLL_STATE                  0x84U

/**
 * Set/Get the bail out option for various technologies in passive poll mode.
 *
 * Default bail out is not set for any technology.\n
 * #PHAC_DISCLOOP_POS_BIT_MASK_A         -> Bail out for Type A.\n
 * #PHAC_DISCLOOP_POS_BIT_MASK_B         -> Bail out for Type B.\n
 * #PHAC_DISCLOOP_POS_BIT_MASK_F212      -> Bail out for Type FeliCa @ 212 kbps.\n
 * #PHAC_DISCLOOP_POS_BIT_MASK_F424      -> Bail out for Type FeliCa @ 424 kbps.\n
 * #PHAC_DISCLOOP_POS_BIT_MASK_V         -> Bail out for Type V / ISO 15693.\n
 * #PHAC_DISCLOOP_POS_BIT_MASK_18000P3M3 -> Bail out for ISO 18000p3m3/ EPC Gen2.\n
 * */
#define PHAC_DISCLOOP_CONFIG_BAIL_OUT                         0x86U

/**
 * Returns the technologies that were detected during poll mode.
 *
 * Passive technologies:\n
 * #PHAC_DISCLOOP_POS_BIT_MASK_A         -> Detected Type A.\n
 * #PHAC_DISCLOOP_POS_BIT_MASK_B         -> Detected Type B.\n
 * #PHAC_DISCLOOP_POS_BIT_MASK_F212      -> Detected FeliCa @ 212 kbps.\n
 * #PHAC_DISCLOOP_POS_BIT_MASK_F424      -> Detected FeliCa @ 424 kbps.\n
 * #PHAC_DISCLOOP_POS_BIT_MASK_V         -> Detected Type V / ISO 15693.\n
 * #PHAC_DISCLOOP_POS_BIT_MASK_18000P3M3 -> Detected ISO 18000p3m3/ EPC Gen2.\n
 *
 * Active technologies:\n
 * #PHAC_DISCLOOP_ACT_POS_BIT_MASK_106 -> Detected active target @ 106 kbps.\n
 * #PHAC_DISCLOOP_ACT_POS_BIT_MASK_212 -> Detected active target @ 212 kbps.\n
 * #PHAC_DISCLOOP_ACT_POS_BIT_MASK_424 -> Detected active target @ 424 kbps.\n
 * */
#define PHAC_DISCLOOP_CONFIG_TECH_DETECTED                    0x87U

/**
 * Configure technologies to listen for in active listen mode.
 *
 * #PHAC_DISCLOOP_POS_BIT_MASK_A         -> Listen for Type A.\n
 * #PHAC_DISCLOOP_POS_BIT_MASK_B         -> Listen for Type B.[Reserved. Not supported currently]\n
 * #PHAC_DISCLOOP_POS_BIT_MASK_F212      -> Listen for Type FeliCa @ 212 kbps.\n
 * #PHAC_DISCLOOP_POS_BIT_MASK_F424      -> Listen for Type FeliCa @ 424 kbps.\n
 * #PHAC_DISCLOOP_POS_BIT_MASK_V         -> Listen for Type V / ISO 15693.[Reserved. Not supported currently]\n
 * #PHAC_DISCLOOP_POS_BIT_MASK_18000P3M3 -> Listen for ISO 18000p3m3/ EPC Gen2.[Reserved. Not supported currently]\n
 * */
#define PHAC_DISCLOOP_CONFIG_ACT_LIS_TECH_CFG                 0x89U

/**
 * Configure technologies to be detected in passive poll mode.
 *
 * #PHAC_DISCLOOP_ACT_POS_BIT_MASK_106 -> Detect active target @ 106 kbps\n
 * #PHAC_DISCLOOP_ACT_POS_BIT_MASK_212 -> Detect active target @ 212 kbps\n
 * #PHAC_DISCLOOP_ACT_POS_BIT_MASK_424 -> Detect active target @ 424 kbps\n
 * */
#define PHAC_DISCLOOP_CONFIG_ACT_POLL_TECH_CFG                0x8AU

/**
 * Configure technologies to listen for in passive listen mode.
 *
 * #PHAC_DISCLOOP_POS_BIT_MASK_A         -> Listen for Type A.\n
 * #PHAC_DISCLOOP_POS_BIT_MASK_B         -> Listen for Type B.[Reserved. Not supported currently]\n
 * #PHAC_DISCLOOP_POS_BIT_MASK_F212      -> Listen for Type FeliCa @ 212 kbps.\n
 * #PHAC_DISCLOOP_POS_BIT_MASK_F424      -> Listen for Type FeliCa @ 424 kbps.\n
 * #PHAC_DISCLOOP_POS_BIT_MASK_V         -> Listen for Type V / ISO 15693.[Reserved. Not supported currently]\n
 * #PHAC_DISCLOOP_POS_BIT_MASK_18000P3M3 -> Listen for ISO 18000p3m3/ EPC Gen2.[Reserved. Not supported currently]\n
 * */
#define PHAC_DISCLOOP_CONFIG_PAS_LIS_TECH_CFG                 0x8BU

/**
 * Configure technologies to be detected in passive poll mode.
 *
 * #PHAC_DISCLOOP_POS_BIT_MASK_A         -> Detect Type A.\n
 * #PHAC_DISCLOOP_POS_BIT_MASK_B         -> Detect Type B.\n
 * #PHAC_DISCLOOP_POS_BIT_MASK_F212      -> Detect Type FeliCa @ 212 kbps.\n
 * #PHAC_DISCLOOP_POS_BIT_MASK_F424      -> Detect Type FeliCa @ 424 kbps.\n
 * #PHAC_DISCLOOP_POS_BIT_MASK_V         -> Detect Type V / ISO 15693.\n
 * #PHAC_DISCLOOP_POS_BIT_MASK_18000P3M3 -> Detect ISO 18000p3m3/ EPC Gen2.\n
 * */
#define PHAC_DISCLOOP_CONFIG_PAS_POLL_TECH_CFG                0x8CU

/**
 * Configure NFC activity version.
 *
 * #PHAC_DISCLOOP_NFC_ACTIVITY_VERSION_1_0 -> Run in Activity 1.0 mode.\n
 * #PHAC_DISCLOOP_NFC_ACTIVITY_VERSION_1_1 -> Run in Activity 1.1 mode. This is the default.\n
 * */
#define PHAC_DISCLOOP_CONFIG_ACTIVITY_VERSION                 0x8DU

/**
 * Configure AUTOCOLL retry count in listen mode.
 *
 * How many times to retry AUTOCOLL command if it fails. Default is 0. Supported
 * range is from 0 to 255.
 * */
#define PHAC_DISCLOOP_CONFIG_TARGET_RETRY_COUNT               0x90U
/*@}*/

#ifdef NXPRDLIB_REM_GEN_INTFS

#include "../comps/phacDiscLoop/src/Sw/phacDiscLoop_Sw.h"

#define phacDiscLoop_Run(pDataParams, bEntryPoint) \
        phacDiscLoop_Sw_Run((phacDiscLoop_Sw_DataParams_t *)pDataParams, bEntryPoint)

#define phacDiscLoop_GetConfig(pDataParams, wConfig, pValue) \
        phacDiscLoop_Sw_GetConfig((phacDiscLoop_Sw_DataParams_t *)pDataParams, wConfig, pValue)

#define phacDiscLoop_SetConfig(pDataParams, wConfig, wValue) \
        phacDiscLoop_Sw_SetConfig((phacDiscLoop_Sw_DataParams_t *)pDataParams, wConfig, wValue)

#define phacDiscLoop_ActivateCard(pDataParams, bTagType, bTagIndex) \
        phacDiscLoop_Sw_ActivateCard((phacDiscLoop_Sw_DataParams_t *)pDataParams, bTagType, bTagIndex)

#else

/**
 * \brief Starts the discovery loop in poll mode or listen mode.
 *
 * Use \ref phacDiscLoop_SetConfig with \ref configurations "configuration options"
 * to modify poll mode and listen mode settings of discovery loop.
 *
 * \return Status code
 * \retval #PHAC_DISCLOOP_FAILURE Discovery loop failure due to error from lower layer.
 * \retval #PHAC_DISCLOOP_COLLISION_PENDING Indicates that collision is pending.
 * \retval #PHAC_DISCLOOP_EXTERNAL_RFON External RF field on.
 * \retval #PHAC_DISCLOOP_EXTERNAL_RFOFF External RF field off.
 * \retval #PHAC_DISCLOOP_NO_TECH_DETECTED No technologies detected.
 * \retval #PHAC_DISCLOOP_NO_DEVICE_RESOLVED No card/device resolved.
 * \retval #PHAC_DISCLOOP_LPCD_NO_TECH_DETECTED LPCD succeeded but no technologies detected..
 * \retval #PHAC_DISCLOOP_MULTI_TECH_DETECTED Multiple Technologies detected.
 * \retval #PHAC_DISCLOOP_MULTI_DEVICES_RESOLVED Multiple cards/devices resolved.
 * \retval #PHAC_DISCLOOP_DEVICE_ACTIVATED Single card/device activated.
 * \retval #PHAC_DISCLOOP_ACTIVE_TARGET_ACTIVATED Active target activated..
 * \retval #PHAC_DISCLOOP_PASSIVE_TARGET_ACTIVATED Passive target activated.
 * \retval #PHAC_DISCLOOP_MERGED_SEL_RES_FOUND Merged SAK with NFC-DEP and T4T support found.
 * \retval #PHAC_DISCLOOP_ACTIVATED_BY_PEER Activated in listen mode.
 */
phStatus_t phacDiscLoop_Run(
    void * pDataParams,                /**< [In] Pointer to discovery loop parameter structure. */
    uint8_t bEntryPoint                /**< [In] Entry point to start discovery loop. Refer \ref phacDiscLoop_Sw_EntryPoints for supported list of entry points. */
    );

/**
 * \brief Set discovery loop \ref configurations "configuration" value.
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval #PH_ERR_INVALID_PARAMETER Invalid option/response received.
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phacDiscLoop_SetConfig(
    void * pDataParams,                /**< [In] Pointer to discovery loop parameter structure. */
    uint16_t wConfig,                  /**< [In] Configuration identifier. */
    uint16_t wValue                    /**< [In] Configuration value. */
    );

/**
 * \brief Get discovery loop \ref configurations "configuration" value.
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful
 * \retval #PH_ERR_INVALID_PARAMETER Invalid option/response received
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phacDiscLoop_GetConfig(
    void * pDataParams,                /**< [In] Pointer to discovery loop parameter structure. */
    uint16_t wConfig,                  /**< [In] Configuration identifier. */
    uint16_t *pValue                   /**< [Out] Configuration value. */
    );

/**
 * \brief Activates the given tag type with given index.
 * \return Status code
 * \retval #PHAC_DISCLOOP_DEVICE_ACTIVATED Single card/device activated.
 * \retval #PHAC_DISCLOOP_PASSIVE_TARGET_ACTIVATED Passive target activated.
 * \retval #PHAC_DISCLOOP_MERGED_SEL_RES_FOUND Merged SAK with NFC-DEP and T4T support found.
 * \retval #PH_ERR_INVALID_PARAMETER If invalid index or card type was given
 * \retval Other Depending on implementation and underlying component.
 */
phStatus_t phacDiscLoop_ActivateCard(
    void * pDataParams,                /**< [In] Pointer to discovery loop parameter structure. */
    uint8_t bTagType,                  /**< [In] Tag type \ref phacDiscLoop_Sw_NfcTech. */
    uint8_t bTagIndex                  /**< [In] The tag index to be activated. */
    );
/** @} */
#endif /* NXPBUILD__PHAC_DISCLOOP */

#endif /* NXPRDLIB_REM_GEN_INTFS */

#ifdef __cplusplus
} /* Extern C */
#endif

#endif
