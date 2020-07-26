/*
*                     Copyright 2016-2019, NXP
*
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
*particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* NFC Library Top Level API of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*/

#ifndef PH_NFCLIB_H
#define PH_NFCLIB_H

#include <ph_Status.h>
#include <phNxpNfcRdLib_Config.h>
#include <phhalHw.h>

/* PAL Headers */

#ifdef NXPBUILD__PHPAL_I14443P3A_SW
#include <phpalI14443p3a.h>
#endif /* NXPBUILD__PHPAL_I14443P3A_SW */

#ifdef NXPBUILD__PHPAL_I14443P3B_SW
#include <phpalI14443p3b.h>
#endif /* NXPBUILD__PHPAL_I14443P3B_SW */

#ifdef NXPBUILD__PHPAL_I14443P4A_SW
#include <phpalI14443p4a.h>
#endif /* NXPBUILD__PHPAL_I14443P4A_SW */

#ifdef NXPBUILD__PHPAL_I14443P4_SW
#include <phpalI14443p4.h>
#endif /* NXPBUILD__PHPAL_I14443P4_SW */

#ifdef NXPBUILD__PHPAL_FELICA_SW
#include <phpalFelica.h>
#endif /* NXPBUILD__PHPAL_FELICA_SW */

#ifdef NXPBUILD__PHPAL_I18092MPI_SW
#include <phpalI18092mPI.h>
#endif /* NXPBUILD__PHPAL_I18092MPI_SW */

#ifdef NXPBUILD__PHPAL_I18092MT_SW
#include <phpalI18092mT.h>
#endif /* NXPBUILD__PHPAL_I18092MT_SW */

#ifdef NXPBUILD__PHPAL_I14443P4MC_SW
#include <phpalI14443p4mC.h>
#endif /* NXPBUILD__PHPAL_I14443P4MC_SW */

#ifdef NXPBUILD__PHPAL_SLI15693_SW
#include <phpalSli15693.h>
#endif /* NXPBUILD__PHPAL_SLI15693_SW */

#ifdef NXPBUILD__PHPAL_I18000P3M3_SW
#include <phpalI18000p3m3.h>
#endif /* NXPBUILD__PHPAL_I18000P3M3_SW */

/* Generic ICode EPC/UID Component Header */
#ifdef NXPBUILD__PHPAL_EPCUID_SW
#include "phpalEpcUid.h"
#endif /* NXPBUILD__PHPAL_EPCUID_SW */

/* AL Headers */

#ifdef NXPBUILD__PHAL_I18000P3M3_SW
#include <phalI18000p3m3.h>
#endif /* NXPBUILD__PHAL_I18000P3M3_SW */

#ifdef NXPBUILD__PHAL_FELICA_SW
#include <phalFelica.h>
#endif /* NXPBUILD__PHAL_FELICA_SW */

#ifdef NXPBUILD__PHAL_ICODE_SW
#include <phalICode.h>
#endif /* NXPBUILD__PHAL_ICODE_SW */

#ifdef NXPBUILD__PHAL_MFC_SW
#include <phalMfc.h>
#endif /* NXPBUILD__PHAL_MFC_SW */

#ifdef NXPBUILD__PHAL_MFDF_SW
#include <phalMfdf.h>
#endif /* NXPBUILD__PHAL_MFDF_SW */


#ifdef NXPBUILD__PHAL_MFDFLIGHT_SW
#include <phalMfdfLight.h>
#endif /* NXPBUILD__PHAL_MFDFLIGHT_SW */

#ifdef NXPBUILD__PHAL_MFNTAG42XDNA_SW
#include <phalMfNtag42XDna.h>
#endif /* NXPBUILD__PHAL_MFNTAG42XDNA_SW */

#ifdef NXPBUILD__PHAL_MFUL_SW
#include <phalMful.h>
#endif /* NXPBUILD__PHAL_MFUL_SW */

#ifdef NXPBUILD__PHAL_T1T_SW
#include <phalT1T.h>
#endif /* NXPBUILD__PHAL_T1T_SW */

#ifdef NXPBUILD__PHAL_TOP_SW
#include <phalTop.h>
#endif /* NXPBUILD__PHAL_TOP_SW */

/* Discovery Loop Header */

#ifdef NXPBUILD__PHAC_DISCLOOP_SW
#include <phacDiscLoop.h>
#endif /* NXPBUILD__PHAC_DISCLOOP_SW */

/* HCE Header */
#ifdef NXPBUILD__PHCE_T4T_SW
#include <phceT4T.h>
#endif /* NXPBUILD__PHCE_T4T_SW */

/* Crypto Headers */

#ifdef NXPBUILD__PH_CRYPTOSYM_SW
#include "phCryptoSym.h"
#endif /* NXPBUILD__PH_CRYPTOSYM_SW */

#ifdef NXPBUILD__PH_CRYPTORNG_SW
#include "phCryptoRng.h"
#endif /* NXPBUILD__PH_CRYPTORNG_SW */

/* Keystore Header */
#if defined (NXPBUILD__PH_KEYSTORE_SW) || defined (NXPBUILD__PH_KEYSTORE_RC663)
#include <phKeyStore.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif    /* __cplusplus */

#ifdef NXPBUILD__PHNFCLIB
/**
 * \defgroup nfc_lib NXP NFC Library Top Level API
 * \brief NXP NFC Library Top Level API
 *
 * \par General
 * This module provides an high level abstraction of the NXP NFC Reader Library stack, focusing on simplicity while featuring the
 * mostly used functionality. It aims to write NXP NFC applications requiring as less code as possible. Still there is the
 * possibility to use the legacy NXP NFC Reader Library stack API by calling #phNfcLib_GetDataParams in order to retrieve the
 * dataparams structure required when calling NXP NFC Reader Library stack functions.
 *
 * \par Activation and Transmission Channel
 * One of the main concepts of the NXP NFC Library module are the transmission channels. A transmission channel is a logical
 * connection between the device running the library and a peer. Once a peer could be activated this logical channel gets
 * established and subsequent calls to phNfcLib_Transmit and phNfcLib_Receive are performing transmissions over this channel.
 * In fact this channel defines how data sent over the channel gets encapsulated. There are three different channels available:
 * \li NFC Forum Channel: This channel is used to transfer NDEF messages
 * \li EMVCo Channel: This channel is used to transfer ISO 7816 APDUs over the ISO14443-4 protocol, according to EMVCo L1 specification.
 * \li ISO Channel: General channel which performs raw data transfer on block/frame level as defined in the corresponding protocols.
 * \sa For more information refer to phNfcLib_Transmit, phNfcLib_Receive and phNfcLib_Activate.
 *
 * \par NXP Nfc Library EMVCo State Machine
 * The NXP NFC Library module implements a state machine ensuring the API functions exposed are executed in the correct order and state.
 * \dot
  digraph host {
      reset [label="RESET"];
      init [label="INITIALIZED"];
      activated [label="ACTIVATED"];
      deactivation_ongoing [label="Deactivation_Ongoing"];
      reset -> init [label="phNfcLib_Init" URL="\ref phNfcLib_Init" fontcolor="blue"];
      init -> reset [label="phNfcLib_DeInit" URL="\ref phNfcLib_DeInit" fontcolor="blue"];
      init -> activated [label="PH_NFCLIB_STATUS_PEER_ACTIVATION_DONE" URL="\ref phNfcLib_Activate" fontcolor="blue"];
      activated -> init [label="phNfcLib_Deactivate\n(PH_NFCLIB_DEACTIVATION_MODE_RF_OFF)" URL="\ref phNfcLib_Deactivate" fontcolor="blue"];
      activated -> init [label="phNfcLib_Deactivate\n(PH_NFCLIB_DEACTIVATION_MODE_RELEASE) success" URL="\ref phNfcLib_Deactivate" fontcolor="blue"];
      activated -> deactivation_ongoing [label="phNfcLib_Deactivate\n(PH_NFCLIB_DEACTIVATION_MODE_RELEASE) failed" URL="\ref phNfcLib_Deactivate" fontcolor="blue"];
      deactivation_ongoing -> deactivation_ongoing [label="phNfcLib_Deactivate\n(PH_NFCLIB_DEACTIVATION_MODE_RELEASE) failed" URL="\ref phNfcLib_Deactivate" fontcolor="blue"];
      deactivation_ongoing -> init [label="phNfcLib_Deactivate\n(PH_NFCLIB_DEACTIVATION_MODE_RELEASE) success" URL="\ref phNfcLib_Deactivate" fontcolor="blue"];
      deactivation_ongoing -> init [label="phNfcLib_Deactivate\n(PH_NFCLIB_DEACTIVATION_MODE_RF_OFF)" URL="\ref phNfcLib_Deactivate" fontcolor="blue"];
      init -> init [label="phNfcLib_Deactivate\n(PH_NFCLIB_DEACTIVATION_MODE_RF_OFF)" URL="\ref phNfcLib_Deactivate" fontcolor="blue"];
  }
  \enddot
 * \par NXP Nfc Library ISO State Machine
 * The NXP NFC Library module implements a state machine ensuring the API functions exposed are executed in the correct order and state.
 * \dot
  digraph host {
      reset [label="RESET"];
      init [label="INITIALIZED"];
      activated [label="ACTIVATED"];
      reset -> init [label="phNfcLib_Init" URL="\ref phNfcLib_Init" fontcolor="blue"];
      init -> reset [label="phNfcLib_DeInit" URL="\ref phNfcLib_DeInit" fontcolor="blue"];
      init -> activated [label="PHNFCLIB_STATUS_PEER_ACTIVATION_DONE" URL="\ref phNfcLib_Activate" fontcolor="blue"];
      activated -> init [label="phNfcLib_Deactivate\n(PH_NFCLIB_DEACTIVATION_MODE_RF_OFF)" URL="\ref phNfcLib_Deactivate" fontcolor="blue"];
      activated -> init [label="phNfcLib_Deactivate\n(PH_NFCLIB_DEACTIVATION_MODE_RELEASE)" URL="\ref phNfcLib_Deactivate" fontcolor="blue"];
      init -> init [label="phNfcLib_Deactivate\n(PH_NFCLIB_DEACTIVATION_MODE_RF_OFF)" URL="\ref phNfcLib_Deactivate" fontcolor="blue"];
  }
  \enddot
 * Functions in this module are only allowed to be called in specific states, otherwise PH_NFCLIB_STATUS_INVALID_STATE an
 * error is returned
 * The \link nfc_lib_api API function description section\endlink provides information which state is required for each function to be called.
 *
 * \par Example Code for EMVCo polling application. (Activation and one APDU data exchange)
 * \code
 * #include <ph_Status.h>
 * #include <phNfcLib.h>
 *
 * phNfcLib_Status_t EMVCoPollingExample(uint8_t* pCApdu, uint16_t wCApduLength, uint8_t* pRApdu, uint16_t * pRApduLength)
 * {
 *    uint16_t wRfTechnos = PH_NFCLIB_TECHNOLOGY_INITIATOR_ISO_14443_A | PH_NFCLIB_TECHNOLOGY_INITIATOR_ISO_14443_B;
 *    phNfcLib_Status_t status;
 *
 *    phNfcLib_Init();
 *    /-/ set EMVCo Profile
 *    phNfcLib_SetConfig_Value(PH_NFCLIB_CONFIG_ACTIVATION_PROFILE, PH_NFCLIB_ACTIVATION_PROFILE_EMVCO);
 *
 *    /-/ start activation. peerInfo set to NULL, we don't need detailed information
 *    status = phNfcLib_Activate(wRfTechnos, NULL, NULL);
 *
 *    if (status == PH_NFCLIB_STATUS_PEER_ACTIVATION_DONE)
 *    {
 *       status = phNfcLib_Transmit(pCApdu, wCApduLength);
 *       status |= phNfcLib_Receive(pRApdu, pRApduLength, NULL);
 *       phNfcLib_Deactivate(PH_NFCLIB_DEACTIVATION_MODE_RELEASE, NULL);
 *    }
 *    else
 *    {
 *       /-/ could either be PH_NFCLIB_STATUS_MULTIPLE_PEERS_DETECTED or PH_NFCLIB_STATUS_PEER_ACTIVATION_FAILED.
 *       /-/ In any case, inform user about it by escalating the error to the caller
 *    }
 *    return status;
 * }
 * \endcode
 *
 * \par Example Code for Reading an NDEF record.
 * \code
 * #include <ph_Status.h>
 * #include <phNfcLib.h>
 *
 * phNfcLib_Status_t NdefActivateAndRead(uint8_t * aNdefMessage, uint16_t *pNdefMessageSize)
 * {
 *    uint16_t wRfTechnos = (PH_NFCLIB_TECHNOLOGY_INITIATOR_ISO_14443_A | PH_NFCLIB_TECHNOLOGY_INITIATOR_ISO_14443_B |
 *                          PH_NFCLIB_TECHNOLOGY_INITIATOR_ISO_15693);
 *    phNfcLib_Status_t status;
 *
 *    phNfcLib_Init();
 *
 *    /-/ set NXP NFC Profile
 *    phNfcLib_SetConfig_Value(PH_NFCLIB_CONFIG_ACTIVATION_PROFILE, PH_NFCLIB_ACTIVATION_PROFILE_NFC);
 *
 *    /-/ start activation. peerInfo set to NULL, we don't need detailed information
 *    status = phNfcLib_Activate(wRfTechnos, NULL, NULL);
 *    if (status == PH_NFCLIB_STATUS_PEER_ACTIVATION_DONE)
 *    {
 *       /-/ activation done, call phNfcLib_Receive to read the NDEF
 *       status = phNfcLib_Receive(aNdefMessage, pNdefMessageSize, NULL);
 *       phNfcLib_Deactivate(PH_NFCLIB_DEACTIVATION_MODE_RELEASE, NULL);
 *    }
 *    else
 *    {
 *       /-/ could be PH_NFCLIB_STATUS_PEER_ACTIVATION_FAILED.
 *       /-/ In any case, inform user about it by escalating the error to the caller
 *    }
 *    return status;
 * }
 * \endcode
 */


/**
 * \defgroup nfc_lib_definitions NFC NXP NFC Library Definitions
 * \brief Common defines used within the API of NFC library
 * \ingroup nfc_lib
 * \{
 *
 */


#define PH_NFCLIB_TECHNOLOGY_ISO_14443_A             0x01U
#define PH_NFCLIB_TECHNOLOGY_ISO_14443_B             0x02U
#define PH_NFCLIB_TECHNOLOGY_FELICA                  0x04U
#define PH_NFCLIB_TECHNOLOGY_ISO_15693               0x08U
#define PH_NFCLIB_TECHNOLOGY_ISO_18000_3_3           0x10U
#define PH_NFCLIB_TECHNOLOGY_ISO_18092               0x20U
#define PH_NFCLIB_TECHNOLOGY_ROLE_INITIATOR_SHIFT    0x00U
#define PH_NFCLIB_TECHNOLOGY_ROLE_TARGET_SHIFT       0x08U

/**
 * @name RF Technologies
 *
 * \anchor nfc_lib_definitions_rf_techno
 * \brief Definitions used within phNfcLib_Activate and struct #phNfcLib_PeerInfo_t
 * \{
 */
#define PH_NFCLIB_TECHNOLOGY_DEFAULT                     0x0U
#define PH_NFCLIB_TECHNOLOGY_INITIATOR_ISO_14443_A       ((uint32_t)PH_NFCLIB_TECHNOLOGY_ISO_14443_A    << PH_NFCLIB_TECHNOLOGY_ROLE_INITIATOR_SHIFT) /**< ISO 14443-3A, Initiator mode RF technology. */
#define PH_NFCLIB_TECHNOLOGY_INITIATOR_ISO_14443_B       ((uint32_t)PH_NFCLIB_TECHNOLOGY_ISO_14443_B    << PH_NFCLIB_TECHNOLOGY_ROLE_INITIATOR_SHIFT) /**< ISO 14443-3B, Initiator mode RF technology. */
#define PH_NFCLIB_TECHNOLOGY_INITIATOR_FELICA            ((uint32_t)PH_NFCLIB_TECHNOLOGY_FELICA         << PH_NFCLIB_TECHNOLOGY_ROLE_INITIATOR_SHIFT) /**< FeliCa Initiator mode RF technology. */
#define PH_NFCLIB_TECHNOLOGY_INITIATOR_ISO_15693         ((uint32_t)PH_NFCLIB_TECHNOLOGY_ISO_15693      << PH_NFCLIB_TECHNOLOGY_ROLE_INITIATOR_SHIFT) /**< ISO 15693, Initiator mode RF technology. */
#define PH_NFCLIB_TECHNOLOGY_INITIATOR_ISO_18000_3_3     ((uint32_t)PH_NFCLIB_TECHNOLOGY_ISO_18000_3_3  << PH_NFCLIB_TECHNOLOGY_ROLE_INITIATOR_SHIFT) /**< ISO 18000-3m3, Initiator mode RF technology. */
#define PH_NFCLIB_TECHNOLOGY_INITIATOR_ISO_18092         ((uint32_t)PH_NFCLIB_TECHNOLOGY_ISO_18092      << PH_NFCLIB_TECHNOLOGY_ROLE_INITIATOR_SHIFT) /**< ISO 18092 active mode, Initiator mode RF technology. */

#define PH_NFCLIB_TECHNOLOGY_TARGET_ISO_14443_A          ((uint32_t)PH_NFCLIB_TECHNOLOGY_ISO_14443_A    << PH_NFCLIB_TECHNOLOGY_ROLE_TARGET_SHIFT)    /**< ISO 14443-3A, Target mode RF technology. */
#define PH_NFCLIB_TECHNOLOGY_TARGET_ISO_14443_B          ((uint32_t)PH_NFCLIB_TECHNOLOGY_ISO_14443_B    << PH_NFCLIB_TECHNOLOGY_ROLE_TARGET_SHIFT)    /**< ISO 14443-3B, Target mode RF technology. */
#define PH_NFCLIB_TECHNOLOGY_TARGET_FELICA               ((uint32_t)PH_NFCLIB_TECHNOLOGY_FELICA         << PH_NFCLIB_TECHNOLOGY_ROLE_TARGET_SHIFT)    /**< FeliCa Target mode RF technology. */
#define PH_NFCLIB_TECHNOLOGY_TARGET_ISO_18092            ((uint32_t)PH_NFCLIB_TECHNOLOGY_ISO_18092      << PH_NFCLIB_TECHNOLOGY_ROLE_TARGET_SHIFT)    /**< ISO 18092 active mode, Target mode RF technology. */
/**
 * \}
 */

/**
 * @name Configuration Defaults
 *
 * \anchor nfc_lib_definitions_configuration_default
 * \brief Definition used for default configuration mode
 * \{
 */
#define PH_NFCLIB_ACTIVATION_BLOCKINGMODE_DEFAULT        PH_ON
#define PH_NFCLIB_DEACTIVATION_BLOCKINGMODE_DEFAULT      PH_ON
/**
 * \}
 */

#define PH_NFCLIB_BIT_RATE_106                       0x0U
#define PH_NFCLIB_BIT_RATE_212                       0x1U
#define PH_NFCLIB_BIT_RATE_424                       0x2U
#define PH_NFCLIB_BIT_RATE_848                       0x3U
#define PH_NFCLIB_BIT_RATE_26                        0x10U
#define PH_NFCLIB_BIT_RATE_53                        0x11U

/**
 * @name Activation Priority
 *
 * \anchor nfc_lib_definitions_activation_priority
 * \brief Definitions used as values for #phNfcLib_SetConfig_Value and #phNfcLib_GetConfig when using
 * configuration tag #PH_NFCLIB_CONFIG_ACTIVATION_MERGED_SAK_PRIO
 *
 * \{
 */
#define PH_NFCLIB_ACTIVATION_MERGED_SAK_PRIO_14443   0x0U /**< Priority given to ISO 14443 */
#define PH_NFCLIB_ACTIVATION_MERGED_SAK_PRIO_18092   0x1U /**< Priority given to ISO 18092 */

/**
 *\}
 */

/**
 * @name Deactivation Mode
 *
 * \anchor nfc_lib_definitions_deactivation_mode
 * \brief Definitions used within phNfcLib_DeActivate for \a bDeactivateMode parameter.
 * \{
 */
#define PH_NFCLIB_DEACTIVATION_MODE_RF_OFF           0x0U /**< No target deactivation procedure is performed,
                                                          RF field is just switched off and wait for 5.1ms. */
#define PH_NFCLIB_DEACTIVATION_MODE_RELEASE          0x1U /**< A target deactivation procedure is performed
                                                          if applicable. (Example, in case of activation
                                                          profile #PH_NFCLIB_ACTIVATION_PROFILE_EMVCO this means the
                                                          removal procedure is performed. In other profiles this
                                                          could imply execution of deactivation procedures like DSL_REQ.)
                                                          RF field is not switched off. phNfcLib_DeActivate should be called
                                                          with #PH_NFCLIB_DEACTIVATION_MODE_RF_OFF to switch off RF field. */
/**
 *\}
 */


/**
 * \name Activation Profiles
 *
 * \{
 *
 * \anchor nfc_lib_definitions_activation_profile
 *
 * \brief Definitions used as values for #phNfcLib_SetConfig_Value and #phNfcLib_GetConfig when using
 * configuration tag \ref PH_NFCLIB_CONFIG_ACTIVATION_PROFILE
 *
 */

/** NFC Forum Activation Loop Profile and NDEF communication channel
 *
 *
 *  \warning NFC Profile is not implementend/available in this release.
 */
#define PH_NFCLIB_ACTIVATION_PROFILE_NFC             0x0U

/** EMVCo Activation Loop Profile and ISO14443-4 communication channel */
#define PH_NFCLIB_ACTIVATION_PROFILE_EMVCO           0x1U

/** Generic ISO Activation Loop Profile and block/frame oriented communication
 * channel (depending on underlying protocol)  */
#define PH_NFCLIB_ACTIVATION_PROFILE_ISO             0x2U
/**
 * \}
 */
#define PH_NFCLIB_CHECK(status)         {if ((status) != PH_NFCLIB_STATUS_SUCCESS) {return (status);}}
#define PH_NFCLIB_CHECK_FCT(status,fct) {(status) = (fct); PH_NFCLIB_CHECK(status);}

#define PH_NFCLIB_STATUS_GROUP_GENERAL                0U
#define PH_NFCLIB_STATUS_GROUP_ACTIVATION             8U
#define PH_NFCLIB_STATUS_GROUP_RF_COMMUNICATION      16U

/** \name Simplified API Errors
*/
/** \{ */
#define PH_NFCLIB_STATUS_SUCCESS                     ((phNfcLib_Status_t)0x00U)
#define PH_NFCLIB_STATUS_INVALID_STATE               ((phNfcLib_Status_t)0x01U << PH_NFCLIB_STATUS_GROUP_GENERAL)
#define PH_NFCLIB_STATUS_INVALID_PARAMETER           ((phNfcLib_Status_t)0x02U << PH_NFCLIB_STATUS_GROUP_GENERAL)
#define PH_NFCLIB_STATUS_BUFFER_OVERFLOW             ((phNfcLib_Status_t)0x04U << PH_NFCLIB_STATUS_GROUP_GENERAL)
#define PH_NFCLIB_STATUS_ABORTED                     ((phNfcLib_Status_t)0x08U << PH_NFCLIB_STATUS_GROUP_GENERAL)
#define PH_NFCLIB_STATUS_INTERNAL_ERROR              ((phNfcLib_Status_t)0x10U << PH_NFCLIB_STATUS_GROUP_GENERAL)

#define PH_NFCLIB_STATUS_PEER_ACTIVATION_DONE        ((phNfcLib_Status_t)0x01U << PH_NFCLIB_STATUS_GROUP_ACTIVATION)
#define PH_NFCLIB_STATUS_MULTIPLE_PEERS_DETECTED     ((phNfcLib_Status_t)0x02U << PH_NFCLIB_STATUS_GROUP_ACTIVATION)
#define PH_NFCLIB_STATUS_PEER_ACTIVATION_FAILED      ((phNfcLib_Status_t)0x04U << PH_NFCLIB_STATUS_GROUP_ACTIVATION)
#define PH_NFCLIB_STATUS_NO_PEER_DISCOVERED          ((phNfcLib_Status_t)0x08U << PH_NFCLIB_STATUS_GROUP_ACTIVATION)
#define PH_NFCLIB_STATUS_PEER_DEACTIVATION_FAILED    ((phNfcLib_Status_t)0x10U << PH_NFCLIB_STATUS_GROUP_ACTIVATION)

#define PH_NFCLIB_STATUS_RF_TRANSMISSION_ERROR       ((phNfcLib_Status_t)0x01U << PH_NFCLIB_STATUS_GROUP_RF_COMMUNICATION)
#define PH_NFCLIB_STATUS_RF_TIMEOUT_ERROR            ((phNfcLib_Status_t)0x02U << PH_NFCLIB_STATUS_GROUP_RF_COMMUNICATION)
#define PH_NFCLIB_STATUS_RF_PROTOCOL_ERROR           ((phNfcLib_Status_t)0x04U << PH_NFCLIB_STATUS_GROUP_RF_COMMUNICATION)
#define PH_NFCLIB_STATUS_RF_PEER_LOST                ((phNfcLib_Status_t)0x08U << PH_NFCLIB_STATUS_GROUP_RF_COMMUNICATION)
/** \} */

/**
 *\}
 */

/**
 * \defgroup nfc_lib_config_tags NFC Library Configuration Tags
 * \ingroup nfc_lib
 * \brief These tags are used as \p bConfigTag parameter for #phNfcLib_SetConfig_Value, #phNfcLib_SetConfig and #phNfcLib_GetConfig
 * \{
 */
/**
 * @name Tags Activation
 *
 * \anchor nfc_lib_config_tags_activation
 * \brief  Activation Influencing Configuration Tags
 * \{
 */
/**
 * \anchor activation_blocking
 * This configuration parameter indicates if the activation procedure (in fact #phNfcLib_Activate) should return only on successful
 * activation or if it should return after every activation loop iteration.
 * \par Parameter Size
 * 1 Byte (\p uint8_t)
 * \par Allowed Values
 * \li \p #PH_OFF Activation procedure is not blocking, i.e. #phNfcLib_Activate returns after every iteration of its internal discovery loop.
 * \li \p #PH_ON Activation procedure is blocking, i.e. #phNfcLib_Activate only returns upon successful activation or in case of an
 *     activation related error (e.g. collision can't be resolved).
 * \par Default Value
 * \p #PH_ON
 */
#define PH_NFCLIB_CONFIG_ACTIVATION_BLOCKING             0x0U

/**
 * This configuration parameter specifies the time in milliseconds the activation procedure, or in fact its internal discovery loop,
 * should wait in target/listen phase or in pause phase (in case no target technology is specified.) These two phases are following
 * the initiator/poll phase, i.e. the total duration of one discovery loop iteration is this target/pause time + time required for
 * executing the polling phase.
 * \par Parameter Size
 * 2 Byte (\p uint16_t)
 * \par Allowed Value Range
 * \p 0 - \p 65535
 * \par Default Value
 * \p 1000
 * \par Example
 * \code
 * phNfcLib_SetConfig_Value(PH_NFCLIB_CONFIG_ACTIVATION_TARGET_PAUSE_TIME, 1000); /-/ 1sec
 * \endcode
 */
#define PH_NFCLIB_CONFIG_ACTIVATION_TARGET_PAUSE_TIME    0x1U

/**
 * \anchor activation_profile
 * This configuration parameter is used to select the profile to be used when running the activation.
 * Depending on the profile the behavior of activation procedure (guard times, discovery loop execution order) is adapted.
 * \par Transmission Channel Mapping
 * This setting also defines how phNfcLib_Transmit and phNfcLib_Receive are mapped on the transmission channel:
 * \li #PH_NFCLIB_ACTIVATION_PROFILE_ISO establishes a raw communication channel, i.e. data is transmitted in blocks/frames as defined
 * by the corresponding standard. For ISO14443 this refers to Layer 4 block oriented half duplex protocol, for ISO18092 it refers to
 * the half duplex data exchange protocol, for all other protocols (FeliCa, ISO15693, ISO18000p3m3, MIFARE Ultralight contactless IC etc.) the data is
 * encapsulated in frames of the respective standard.
 * \li #PH_NFCLIB_ACTIVATION_PROFILE_EMVCO Here only ISO14443 layer 4 block oriented half duplex protocol is supported.
 * \li #PH_NFCLIB_ACTIVATION_PROFILE_NFC Here the data is sent is actually a well formatted NDEF record. The NFC library in this case performs
 * the NDEF detection procedure. If the tag is not formatted yet, it will be automatically formatted. Existing data will be replaced.
 *
 * \par Parameter Size
 * 1 Byte (uint8_t)
 * \par Allowed Values
 * Refer to \ref nfc_lib_definitions_activation_profile
 * \par Default Value
 * #PH_NFCLIB_ACTIVATION_PROFILE_NFC
 * \par Example
 * \code
 * phNfcLib_SetConfig_Value(PH_NFCLIB_CONFIG_ACTIVATION_PROFILE, PH_NFCLIB_ACTIVATION_PROFILE_EMVCO); /-/ set EMVCo Profile
 * \endcode
 */
#define PH_NFCLIB_CONFIG_ACTIVATION_PROFILE              0x2U
/**
 * \}
 */

/**
 * \anchor activation_prio
 * This configuration parameter is used set the activation priority in case a peer indicate a 'merged SAK',
 * i.e. peer supports both, ISO18092 and ISO14443 Layer 4. Depending on this parameter, activation procedure then
 * activates the protocol indicated by its value.
 * \par Parameter Size
 * 1 Byte (\p uint8_t)
 * \par Allowed Values
 * Refer to \ref nfc_lib_definitions_activation_priority
 * \par Default Value
 * #PH_NFCLIB_ACTIVATION_MERGED_SAK_PRIO_14443
 */
#define PH_NFCLIB_CONFIG_ACTIVATION_MERGED_SAK_PRIO      0x3U
/**
 * \}
 */

/**
 * \anchor De-activation_blocking
 * This configuration parameter indicates if the deactivation procedure (in fact #phNfcLib_Deactivate) should return only on successful
 * deactivation or if it should return after every deactivation loop iteration. This configuration parameter is applicable only in case
 * of activation profile is #PH_NFCLIB_ACTIVATION_PROFILE_EMVCO.
 * This configuration is not applicable and doesn't impact behavior of #phNfcLib_Deactivate API in ISO and NFC mode.
 * \par Parameter Size
 * 1 Byte (\p uint8_t)
 * \par Allowed Values
 * \li \p #PH_OFF De-activation procedure is not blocking, i.e. #phNfcLib_Deactivate returns after one cycle of deactivation procedure.
 *     One cycle includes sending several WUPA/B commands according to EMVCo Contactless specification, Book D. If a response is received
 *     for any one WUPA/B the function returns immediately with #PH_NFCLIB_STATUS_PEER_DEACTIVATION_FAILED and user needs to call
 *     #phNfcLib_Deactivate once again to perform deactivation procedure.
 * \li \p #PH_ON De-activation procedure is blocking, i.e. #phNfcLib_Deactivate only returns upon successful deactivation from discovery loop
 *     or in case of an abort.
 * \par Default Value
 * \p #PH_ON
 *
 * \pre NFC Library should not be in state RESET
 */
#define PH_NFCLIB_CONFIG_DEACTIVATION_BLOCKING           0x4U

/**
 * \anchor Enable_LPCD
 * This configuration parameter indicates if LPCD (Low Power Card Detection) is ON during Activation procedure (in fact #phNfcLib_Activate)
 * should perform poll only on change in RF field (except for activation profile #PH_NFCLIB_ACTIVATION_PROFILE_EMVCO).
 * \par Parameter Size
 * 1 Byte (\p uint8_t)
 * \par Allowed Values
 * \li \p #PH_OFF LPCD procedure is OFF.
 * \li \p #PH_ON  LPCD procedure if ON.
 * \par Default Value
 * \p #PH_OFF
 */
#define PH_NFCLIB_CONFIG_INITIATOR_ENABLE_LPCD           0x10U

#define PH_NFCLIB_CONFIG_TARGET_TYPEA_ATQA               0x20U
#define PH_NFCLIB_CONFIG_TARGET_TYPEA_UID                0x21U
#define PH_NFCLIB_CONFIG_TARGET_TYPEA_SAK                0x22U
#define PH_NFCLIB_CONFIG_TARGET_TYPEF_UID                0x30U
#define PH_NFCLIB_CONFIG_TARGET_P2P_GI                   0x40U
/**
 * \}
 */

typedef uint32_t phNfcLib_Status_t;

 /**
  * \anchor nfc_lib_mfc_uid_type
  * \name UID Type definitions for Personalize UID Command
  */
 /** \{ */
 #define PH_NFCLIB_MFC_UID_TYPE_UIDF0      0x00U     /**< MIFARE(R) Classic contactless IC UID type UIDF0. */
 #define PH_NFCLIB_MFC_UID_TYPE_UIDF1      0x40U     /**< MIFARE(R) Classic contactless IC UID type UIDF1. */
 #define PH_NFCLIB_MFC_UID_TYPE_UIDF2      0x20U     /**< MIFARE(R) Classic contactless IC UID type UIDF2. */
 #define PH_NFCLIB_MFC_UID_TYPE_UIDF3      0x60U     /**< MIFARE(R) Classic contactless IC UID type UIDF3. */
 /** \} */

 /**
  * \anchor nfc_lib_18000p3m3_options
  * \name ISO/IEC 18000p3m3 options that can be provided to Write, Kill, and Access commands.
  */
 /** \{ */
 #define PH_NFCLIB_18000P3M3_AC_NO_COVER_CODING  0x00U   /**< Use cover coding to diversify passwords. */
 #define PH_NFCLIB_18000P3M3_AC_USE_COVER_CODING 0x01U   /**< Do not use cover coding, send plain passwords. */
 /** \} */

#ifdef NXPBUILD__PHAC_DISCLOOP_SW
 /**
  * \anchor nfc_lib_max_vicinity_tag
  * \name Max ISO/IEC 15693 and ISO/IEC 18000p3m3 tags that can be detected by Simplified API.
  */
 /** \{ */
#define PH_NFCLIB_MAX_ISO15693_TAGS_SUPPORTED    PHAC_DISCLOOP_CFG_MAX_VICINITY_CARDS_SUPPORTED
#define PH_NFCLIB_MAX_ISO18000_TAGS_SUPPORTED    PHAC_DISCLOOP_CFG_MAX_VICINITY_CARDS_SUPPORTED
 /** \} */
#endif /* NXPBUILD__PHAC_DISCLOOP_SW */

/**
 * \brief NFC Lib error call-back function type.
 *
 * This callback will be called by /ref phNfcLib_Activate API in case of collision/transmission/protocol errors
 * during acivation procedure in EMVCo mode only.
 * \note
 * Error that are reported are #PH_NFCLIB_STATUS_MULTIPLE_PEERS_DETECTED, #PH_NFCLIB_STATUS_RF_TRANSMISSION_ERROR,
 * and #PH_NFCLIB_STATUS_RF_PROTOCOL_ERROR.
 */
typedef void(*pphNfcLib_ErrCallbck)(phNfcLib_Status_t dwStatus);

/**
 * \brief NFC LIB StateMachine
 * \anchor NfcLib_StateMachine
 * Refer state machine for each mode defined above.
 */
typedef struct
{
    void * pBalDataparams;                   /* Should point to the BAL Dataparams initialized by the application. */
    void * pWtxCallback;                     /* Pointer to the WTX Callback implemented by the application. */
    void * pRtoxCallback;                    /* Pointer to the RTOX Callback implemented by the application. */
} phNfcLib_AppContext_t;

/**
 * \brief NFC LIB ACTIVATED TYPE
 * \anchor phNfcLib_ActivatedType_t
 */
typedef enum /*phNfcLib_ActivatedType*/
{
    E_PH_NFCLIB_UNKNOWN            = 0,
    E_PH_NFCLIB_MIFARE_ULTRALIGHT,                          /**< Card Type can be any one of MIFARE Ultralight, MIFARE Ultralight C, MIFARE Ultralight EV1. */
    E_PH_NFCLIB_MIFARE_CLASSIC_1K,
    E_PH_NFCLIB_MIFARE_CLASSIC_4K,
    E_PH_NFCLIB_MIFARE_DESFIRE,
    E_PH_NFCLIB_TYPEA_LAYER3,
    E_PH_NFCLIB_TYPEA_LAYER4,
    E_PH_NFCLIB_TYPEB_LAYER3,
    E_PH_NFCLIB_TYPEB_LAYER4,
    E_PH_NFCLIB_ISO15693,
    E_PH_NFCLIB_ISO18000p3m3,
    E_PH_NFCLIB_LAST = 0x7FFFFFFF
} phNfcLib_ActivatedType_t;

#ifdef NXPBUILD__PHNFCLIB_PROFILES
/**
 * \brief Peer Information
 * \anchor peer_info
 */
typedef struct
{
    uint8_t  bId;                                /* Layer ID : Not used */
    phNfcLib_ActivatedType_t dwActivatedType;                    /**< Activated type may be ORed value of \ref phNfcLib_ActivatedType_t. */
#if 0
    uint32_t dwSelectedType;                     /**< Not used in this Beta release. User has to choose from 'dwActivatedType'
                                                      with any one card type \ref phNfcLib_ActivatedType_t he is interested. */
#endif
    union {
        union {
            struct
            {
                uint16_t wAtqa;                  /**< To be done. */
                uint8_t bUidLength;              /**< Activated Type A tag UID length is provided. */
                uint8_t aUid[10];                /**< Provides UID of activated Type A card. */
                uint8_t bSak;                    /**< To be done. */
            } tIso14443_3a;

            struct
            {
                uint8_t bAtqbLength;             /**< To be done. */
                uint8_t aPupi[4];                /**< To be done. */
                uint8_t aApplicationData[4];     /**< To be done. */
                uint8_t bIsExtendedAtqb;         /**< To be done. */
                uint8_t aProtocolInfo[4];        /**< To be done. */
            } tIso14443_3b;

            struct
            {
                struct
                {
                    uint8_t* pUid;               /* Provides pointer to array of 8bytes UID of ISO 15693 tags detected. */
                }TagIndex[PH_NFCLIB_MAX_ISO15693_TAGS_SUPPORTED];

                uint8_t bTotalTagsFound;         /* Number of ISO 15693 Tags detected. */
            } tIso15693;

            struct
            {
                struct
                {
                    uint8_t* pUii;               /**< Provides pointer to UII of ISO 18000p3m3 tags detected. */
                    uint16_t wUiiLength;         /**< UII length in bits pointed by \a pUii. */
                }TagIndex[PH_NFCLIB_MAX_ISO18000_TAGS_SUPPORTED];
                uint8_t bTotalTagsFound;         /* Number of ISO 18000p3m3 Tags detected. */
            } tIso18000_3_3;
        } uInitiator;
    } uTi;
    uint8_t  bRfProtocolInformationLength;
    union {
        union {
            struct
            {
                uint8_t bDid;
                uint8_t aAts[PH_NXPNFCRDLIB_CONFIG_ATS_BUFF_LENGTH];
            } tIso14443_4;                       /**< To be done. */

            struct
            {
                uint8_t aNfcId3[10];
                uint8_t bDid;
                uint8_t bGeneralBytesLength;
                uint8_t aGeneralBytes[48];
            } tIso18092;                         /**< To be done. */
        } uInitiator;

    } uRi;

} phNfcLib_PeerInfo_t;
#endif /* NXPBUILD__PHNFCLIB_PROFILES */

/**
 * \anchor nfc_lib_mfc_commands
 * \brief Defines all the commands that can be performed on MIFARE Classic contactless IC card
 */
typedef enum /*phNfcLib_MFC_Commands*/
{
    NoCommand = 0,
    Read,
    Write,
    Increment,
    Decrement,
    Transfer,
    Restore,
    Authenticate,                                   /**< Provides Authentication using only software keystore. */
    PersonalizeUID
} phNfcLib_MFC_Commands_t;

/**
 * \anchor nfc_lib_mful_commands
 * \brief Provides commands that can be performed on MIFARE Ultralight contactless IC
 */
typedef enum /*phNfcLib_MFUL_Commands*/
{
    MFUL_NoCommand = 0,
    MFUL_Read,
    MFUL_Write,
    MFUL_Compatibility_Write
} phNfcLib_MFUL_Commands_t;

/**
 * \anchor nfc_lib_mfdf_commands
 * \brief Provides commands that can be performed on MIFARE DESFire contactless IC
 */
typedef enum /* phNfcLib_MFDF_Commands */
{
    MFDF_NoCommand = 0,
    MFDF_Create_Application,
    MFDF_Select_Application,
    MFDF_Get_Version,
    MFDF_Create_StdDataFile,
    MFDF_Write_Data,
    MFDF_Iso_SelectFile,
    MFDF_Iso_ReadBinary,
    MFDF_Iso_UpdateBinary
} phNfcLib_MFDF_Commands_t;

/**
 * \anchor nfc_lib_iso15693_commands
 * \brief Provides commands that can be performed on ISO 15693 compliant card
 * Simplified API always communicates in Address Mode with ISO 15693 compliant card.
 */
typedef enum /*phNfcLib_I15693_Commands*/
{
    ISO15693_NoCommand = 0,
    ISO15693_ReadSingleBlock,
    ISO15693_Extended_ReadSingleBlock,
    ISO15693_ReadMultipleBlocks,
    ISO15693_Extended_ReadMultipleBlocks,
    ISO15693_GetSystemInformation,
    ISO15693_ExtendedGetSystemInformation,
    ISO15693_GetMultipleBlockSecurityStatus,
    ISO15693_Extended_GetMultipleBlockSecurityStatus,
    ISO15693_WriteSingleBlock,
    ISO15693_Extended_WriteSingleBlock,
    ISO15693_WriteMultipleBlocks,
    ISO15693_Extended_WriteMultipleBlocks,
    ISO15693_LockBlock,
    ISO15693_Extended_LockBlock,
    ISO15693_WriteAFI,
    ISO15693_LockAFI,
    ISO15693_WriteDSFID,
    ISO15693_LockDSFID
}phNfcLib_I15693_Commands_t;

/**
 * \anchor nfc_lib_iso18000p3m3_commands
 * \brief Provides commands that can be performed on ISO 18000p3m3 compliant card
 */
typedef enum /*phNfcLib_I18000_Commands*/
{
    ISO18000_NoCommand = 0,
    ISO18000_Ack,
    ISO18000_ReqRn,                                /**< ReqRN with Handle and can be executed on Tags which are in open or secured
                                                        states. No option to execute on Tags which are in acknowledged state. */
    ISO18000_Read,
    ISO18000_Write,
    ISO18000_Kill,
    ISO18000_Lock,
    ISO18000_Access,
    ISO18000_BlockWrite,
    ISO18000_BlockErase,
    ISO18000_BlockPermaLock,
    ISO18000_SetHandle
} phNfcLib_I18000_Commands_t;

/**
 * \anchor nfc_lib_mfc_parameters
 * \brief Defines parameters that needs be provided by User to communicate with MIFARE Classic contactless IC
 */
typedef struct /*phNfcLib_MFC*/{
    phNfcLib_MFC_Commands_t bCommand;               /**< [In] MIFARE Classic command needs to be provided. */
    uint8_t bBlockNumber;                           /**< [In] Block number on which MIFARE Classic command to be executed. */
    uint8_t bKeyType;                               /**< [In] Key Type used to Authenticate MIFARE Classic. Takes either #PHHAL_HW_MFC_KEYA or #PHHAL_HW_MFC_KEYB. */
    uint8_t bKeyNumber;                             /**< [In] Key Number from Sw KeyStore to be fetched to Authenticate MIFARE Classic. */
    uint8_t bKeyVersion;                            /**< [In] Key version from Sw KeyStore to be retrieved to Authenticate MIFARE Classic. */
    uint8_t bUidType;                               /**< [In] UID Type used to Personalize the MFC card. Refer \ref nfc_lib_mfc_uid_type */
    uint8_t* pBuffer;
}phNfcLib_MFC_t;

/**
 * \anchor nfc_lib_mful_parameters
 * \brief Defines parameters that needs be provided by User to communicate with MIFARE Ultralight contactless IC
 */
typedef struct /*phNfcLib_MFUL*/{
    phNfcLib_MFUL_Commands_t bCommand;              /**< [In] MIFARE Ultralight command needs to be provided. */
    uint8_t bPageNumber;                            /**< [In] Page number on which MIFARE Ultralight command to be executed. */
    uint8_t* pBuffer;
}phNfcLib_MFUL_t;

/**
 * \anchor nfc_lib_mfdf_parameters
 * \brief Defines parameters that needs be provided by User to communicate with MIFARE DESFire IC-based contactless card
 */
typedef struct /*phNfcLib_MFdf*/{
    phNfcLib_MFDF_Commands_t bCommand;              /**< [In] MIFARE DESFire command needs to be provided. */
    union{

       struct {
    uint8_t bOptions;                               /**< [In] Option parameter. */
    uint8_t* pApplicationIdentifier;                /**< [In] MIFARE DESFire AID (Application identifier), 0x000000 is reserved. */
    uint8_t bKeySettings1;                          /**< [In] MIFARE DESFire KeySetting 1 currently can be only set to 0x0F, as
                                                              authentication support is not yet provided by Simplified API. */
    uint8_t bKeySettings2;                          /**< [In] MIFARE DESFire KeySetting 2, bits 6,7 defines the key that will be used,
                                                              bit 5 indicates if application supports ISO File ID for selection,
                                                              for whole application and lower nibble defines no of Keys that can be used. */
    uint8_t* pISOFileId;                            /**< [In] Optional ISO File ID that can be used to Select application using 7816-4 command,
                                                              Valid only if bit 5 of KeySetting 2 is set to '1'. */
    uint8_t * pISODFName;                           /**< [In] Optional ISO DF name that can be used to Select application using 7816-4 command,
                                                              Length of this parameter can be from 1 to 16bytes. */
    uint8_t bISODFNameLen;                          /**< [In] ISO DF name length provided by User, can take value from 1 to 16bytes. */
        }Create_Application;

       struct{
    uint8_t* pApplicationIdentifier;                /**< [In] MIFARE DESFire AID (Application identifier) to select file. */
        }Select_Application;

        struct{
    uint8_t bOption;                                /**< [In] Option parameter. */
    uint8_t bFileNo;                                /**< [In] File number in the range of 0x00 to 0x1F. */
    uint8_t* pISOFileId;                            /**< [In] Optional ISO File ID that can be used to select file using 7816-4 command. */
    uint8_t bCommSett;                              /**< [In] Communication settings, currently only plain comm is supported with bit 0 set to 0. */
    uint8_t* pAccessRights;                         /**< [In] Read/Write access rights, currently only free access is supported with all nibbles set to 0xE. */
    uint8_t* pFileSize;                             /**< [In] File size. */
        }Create_StdDataFile;

        struct{
    uint8_t bCommOption;                            /**< [In] Communication settings, currently only plain comm is supported (value 0x00). */
    uint8_t bFileNo;                                /**< [In] File number in the range of 0x00 to 0x1F. */
    uint8_t* pOffset;                               /**< [In] Offset from which write operation to be done. */
    uint8_t * pTxData;                              /**< [In] Pointer to array of data to be written into File. */
    uint8_t* pTxDataLen;                            /**< [In] Length of data. */
        }Write_Data;

        struct{
    uint8_t bSelector;                              /**< [In] P1 field in ISO 7816-4, can take either 0x00 or 0x02 or 0x04. */
    uint8_t bOption;                                /**< [In] P2 field in ISO 7816-4, when 0x00 FCI stored in file will be returned.
                                                              when 0x0C FCI shall not be returned. Any other value is invalid. */
    uint8_t* pFid;                                  /**< [In] Elementary File Identifier used to select application when P1 is
                                                              either 0x00 or 0x02. */
    uint8_t* pDFname;                               /**< [In] DF name that can be used to Select ISO file. */
    uint8_t bDFnameLen;                             /**< [In] Length of DF name provided by User, can take value from 1 to 16bytes. */
        }Select_File;


        struct{
    uint16_t wOption;                               /**< [In] Option parameter indicating #PH_EXCHANGE_DEFAULT or #PH_EXCHANGE_RXCHAINING. */
    uint8_t bSfid;                                  /**< [In] P1 field in ISO 7816-4, if bit 8 is set to zero then p1 indicates MSB of offset else
                                                              bit5 to bit1 (5bits) indicates short ISO FID. */
    uint8_t bOffset;                                /**< [In] P2 field in ISO 7816-4, codes offset. */
    uint8_t bBytesToRead;                           /**< [In] Le field in ISO 7816-4, number of bytes to read from file. */
        }Read_Binary;

        struct{
    uint8_t bSfid;                                  /**< [In] P1 field in ISO 7816-4, if bit 8 is set to zero then p1 indicates MSB of offset else
                                                              bit5 to bit1 (5bits) indicates short ISO FID. */
    uint8_t bOffset;                                /**< [In] P2 field in ISO 7816-4, codes offset. */
    uint8_t * pData;                                /**< [In] Pointer to array data to be written. */
    uint8_t bDataLen;                               /**< [In] Length of data provided by application. */
        }Update_Binary;

    }Operation;
    uint8_t* pBuffer;
}phNfcLib_MFDF_t;

/**
 * \anchor nfc_lib_iso15693_parameters
 * \brief Defines parameters that needs be provided by User to communicate with ISO 15693 card
 */
typedef struct /*phNfcLib_I15693*/
{
    phNfcLib_I15693_Commands_t bCommand;            /**< [In] ISO-IEC 15693-3 command needs to be provided. */
    uint8_t bOption;                                /**< [In] Option flag shall be set in Request flags. Meaning is defined by
                                                              each command description. For more info refer ISO-IEC 15693-3 Spec. */
    uint8_t bAfi;                                   /**< [In] Application family identifier. */
    uint8_t bDsfid;                                 /**< [In] Data storage format identifier. */
    uint16_t wBlockNumber;                          /**< [In] Block number on which ISO 15693-3 command to be executed.
                                                              Optional commands block number is single byte. Only in case of Extended
                                                              commands block number will be 2 bytes. */
    uint16_t wNumBlocks;                            /**< [In] Number of blocks on which command like ReadMultiple/WriteMultiple blocks
                                                              command to be executed. */
    uint8_t bUid[8];
    uint8_t* pBuffer;
}phNfcLib_I15693_t;

/**
 * \anchor nfc_lib_iso18000_parameters
 * \brief Defines parameters that needs be provided by User to communicate with ISO 18000p3m3 card
 */
typedef struct /*phNfcLib_I18000p3m3*/
{
    phNfcLib_I18000_Commands_t bCommand;            /**< [In] ISO-IEC 18000-p3m3 command needs to be provided. */
    uint8_t   bOption;                              /**< [In] Options that can provided for Write, Kill and Access. Refer \ref nfc_lib_18000p3m3_options Options */
    uint8_t   bMemBank;                             /**< [In] Memory bank being addressed. Valid memory bank are 0, 1, 2 and 3. */
    uint8_t   bRFU;
    uint8_t   bReadLock;                            /**< [In] Indicates if BlockPermaLock should read (bit 0) or lock (bit 1) the block. */
    uint8_t*  pWordPtr;                             /**< [In] Pointer to starting address for Read. Shall be of 8, 16, 24, 32 bit length. */
    uint8_t   bWordPtrLength;                       /**< [In] Should indicate length of pWordPtr provided by User. Valid values are 0, 1, 2 and 3. */
    uint8_t   bWordCount;                           /**< [In] Number of 16-bit words to be Read or BlockWrite. */
    uint8_t*  pPassword;                            /**< [In] Pointer to 4 bytes of password used by Kill and Access command to put the Tag into
                                                              killed or secured state respectively. */
    uint8_t   bRecom;                               /**< [In] Recommission Option - 3 bit significant, Option 0x04 is mandatory implementation rest is optional . */
    uint8_t*  pMask;                                /**< [In] Mask specifies which memory blocks a tag PermaLocks. */
    uint8_t*  pAction;                              /**< [In] Action specifies teh action to be perfomed for Lock on the given Mask. */
    uint8_t*  pBlockPtr;                            /**< [In] Pointer to starting address for BlockPermaLock. Shall be of 8, 16, 24, 32 bit length. */
    uint8_t   bBlockPtrLength;                      /**< [In] Should indicate length of pBlockPtr provided by User. Valid values are 0, 1, 2 and 3. */
    uint8_t   bBlockRange;                          /**< [In] Specifies range of Mask, starting at BlockPtr and ending (16 BlockRange) 1 blocks later
                                                              to starting address for BlockPermaLock. If 0x00 then tag ignore the BlockPermalock command. */
    uint8_t*  pHandle;                             /**< [In] Pointer to the 2 byte handle of the tag. */
    uint8_t*  pUii;                                /**< [In] Pointer to the UII Mem bank of the tag, could be at max 58 bytes data . */
    uint8_t   wUiiMaskLength;                      /**< [In] Length of the UII Membank data received . */
    uint8_t*  pBuffer;                              /**< [In] Pointer to 16bit or multiple of 16bit data that needs to be written on Tag. */
}phNfcLib_I18000p3m3_t;

/**
 * \anchor nfc_lib_raw_parameters
 * \brief Defines parameters that needs be provided by User to communicate with raw ISO 14443-3 or ISO 14443-4 peer.
 */
typedef struct /*phNfcLib_RawTransmit*/
{
    uint8_t* pBuffer;
}phNfcLib_RawTransmit_t;

/**
 * \anchor nfc_lib_transmit_metadata
 * \brief Defines parameters that needs be provided by User to use \ref phNfcLib_Transmit API in ISO mode.
 */
typedef union /*phNfcLib_Transmit*/{
    phNfcLib_MFC_t         phNfcLib_MifareClassic;
    phNfcLib_MFUL_t        phNfcLib_MifareUltraLight;
    phNfcLib_MFDF_t        phNfcLib_MifareDesfire;
    phNfcLib_I15693_t      phNfcLib_ISO15693;
    phNfcLib_I18000p3m3_t  phNfcLib_ISO18000;
    phNfcLib_RawTransmit_t phNfcLib_RawTransmit;
} phNfcLib_Transmit_t;


/**
 * \defgroup nfc_lib_api NFC Library Functions
 * \ingroup nfc_lib
 * \brief All Public Functions of the NFC Library
 * \{
 */

/**
 *  \brief Used to set the application context to NFC Library before Initialization.
 *
 *  This function should be used before calling \ref phNfcLib_Init API.
 *  1. Used to provide the context of BAL Dataparams for HAL initializations.
 *  2. Used to provide the WTX Callback for PAL 14443-4mC initialization.
 *  3. Used to provide the RTOX Callback for PAL 18092mT initialization.
 *
 */
phNfcLib_Status_t phNfcLib_SetContext(
                                      phNfcLib_AppContext_t * pAppContext     /**< [In] Pointers to phNfcLib_AppContext_t structure which provides the context to NFCLIB by Application. */
                                      );

/**
 *  \brief Initialize the NFC Library.
 *
 *  Calling this function initializes the NFC library including all its
 *  relevant protocol and hardware abstraction layers. It also calls the
 *  initialization functions for the underlying platform. After calling this
 *  function all other functions of the phNfcLib module may be used.
 *
 * \pre NFC library must be in state RESET
 * \post All APIs are allowed to be called. NFC Library is in state INITIALIZED
 * \retval PH_NFCLIB_STATUS_INVALID_STATE
 * NFC Library had been in a wrong state when calling this function.
 */
phNfcLib_Status_t phNfcLib_Init(void);


/**
 * \brief Deinitialize the NFC Library
 *
 * Calling this function resets all internal structures to its default values, releases all allocated resources and
 * calls DeInit of all underlying layers.
 *
 * \pre NFC Library must be in state INITIALIZED
 * \post NFC Library is in state RESET. Only #phNfcLib_Init is allowed to be called.
 * \retval PH_NFCLIB_STATUS_INVALID_STATE
 * NFC Library had been in a wrong state when calling this function.
 */
phNfcLib_Status_t phNfcLib_DeInit(void);

#ifdef NXPBUILD__PHNFCLIB_PROFILES
/**
 * \brief Run the Activation Procedure.
 *
 * This command starts the activation procedure which in fact implements a discovery loop. This loop performs technology detection
 * (for given technologies using bit mask \p wTechnologyMask) and, if a technology has been detected, the activation with the
 * associated peer is performed.
 *
 * The activation procedure can be configured by calling #phNfcLib_SetConfig_Value or #phNfcLib_SetConfig and appropriate configuration
 * tags (see \ref nfc_lib_config_tags_activation), for instance #PH_NFCLIB_CONFIG_ACTIVATION_BLOCKING.
 * These configuration parameters influence the behavior of the activation procedure. The activation procedure also establishes a logical
 * channel which depends on the configured \ref activation_profile. This channel defines how data is being
 * encapsulated when calling phNfcLib_Transmit or phNfcLib_Receive, respectively.
 *
 * Once a peer has been activated and a channel could be established \p peerInfo is set with \ref peer_info
 * about the peer and the established link. #phNfcLib_Transmit and #phNfcLib_Receive may then be used in order to perform data exchange
 * over the established link. In order to deactivate an activated peer/connection #phNfcLib_Deactivate has to be called.
 *
 * \note
 * \li The activation procedure will always try to resolve collisions (except for activation profile #PH_NFCLIB_ACTIVATION_PROFILE_EMVCO).
 * So even if there a multiple peers in proximity (except in case of ISO156936 and ISO18000p3m3), the activation procedure will activate
 * only one peer.
 * The priority (i.e. the order in which technology detection is performed) depends on the configured \ref activation_profile
 * and additionally on the configured \ref activation_prio
 * \li When polling for multiple technologies, activation procedure always uses bail out after first technology is detected and doesn't
 * poll further for other technologies.
 * \li When multiple tags/peers are detected, activation procedure uses different mechanism for each technology to resolve and activate
 * tag/peer.
 *    Type A : Collision resolution of Type A always resolves the tag with highest UID value,
 *    Type B : The first tag to reply to the slot marker command which can be any random tag will be activated
 *    ISO 15693 : In Type V, when single tag is placed in vicinity, tag will be moved to selected state using select command
 *             (TBD : If detected tag doesn't support Select command) and in case of multiple tags in vicinity, based on
 *             discovery loop #PHAC_DISCLOOP_CFG_MAX_VICINITY_CARDS_SUPPORTED macro value collision will be resolved and
 *             #PHAC_DISCLOOP_CFG_MAX_VICINITY_CARDS_SUPPORTED number of tag UID's in proximity are returned to User.
 *    ISO 18000p3m3 : When single tag is placed in vicinity, tag will be moved to Open state using Req_Rn command and handle
 *             is available which is used to communicate with tag. In case of multiple tags in vicinity, based on
 *             discovery loop #PHAC_DISCLOOP_CFG_MAX_VICINITY_CARDS_SUPPORTED macro value collision will be resolved and
 *             #PHAC_DISCLOOP_CFG_MAX_VICINITY_CARDS_SUPPORTED number of tag UII's in proximity are returned to User.
 * \li In case #PH_NFCLIB_ACTIVATION_PROFILE_NFC is configured as \ref activation_profile and underlying
 * protocol is ISO18092 this procedure also tries to perform LLCP activation in order to be able to transmit NDEF messages over SNEP
 * when using phNfcLib_Transmit and phNfcLib_Receive.
 *
 * \pre NFC Library must be in state INITIALIZED or ACTIVATED
 * \post After successful activation (PH_NFCLIB_STATUS_PEER_ACTIVATION_DONE) NFC library is in state ACTIVATED.
 *
 * \retval PH_NFCLIB_STATUS_PEER_ACTIVATION_DONE
 * Activation procedure could activate a peer and establish a channel.
 * \p peerInfo holds detailed information about the peer and the associated connection.
 * \retval PH_NFCLIB_STATUS_MULTIPLE_PEERS_DETECTED
 * Activation procedure detected multiple peers in RF field based on the discovery loop macro #PHAC_DISCLOOP_CFG_MAX_VICINITY_CARDS_SUPPORTED
 * for Type V and 18000p3m3 and these tag identifiers are returned to user as collision need not be resolved to communicate in case of
 * \ref activation_profile is ISO.
 * Detected a collision and was not able to resolve it in case of \ref activation_profile is EMVCo.
 * \retval PH_NFCLIB_STATUS_PEER_ACTIVATION_FAILED
 * Activation procedure detected a peer/technology but was not able to properly activate it. Example: #PH_NFCLIB_ACTIVATION_PROFILE_NFC
 * is the configure profile but peer does not support LLCP.
 * \retval PH_NFCLIB_STATUS_NO_PEER_DISCOVERED
 * In case activation procedure is configured for \ref activation_blocking non-blocking execution and no peer has been detected
 * within one iteration, activation returns with this status code.
 * \retval PH_NFCLIB_STATUS_INVALID_STATE
 * NFC Library had been in a wrong state when calling this function.
 */
phNfcLib_Status_t phNfcLib_Activate(
                 uint16_t wTechnologyMask,        /**< [In] A bit mask indicating all \ref nfc_lib_definitions_rf_techno
                                                            activation procedure shall handle during its discovery loop execution.
                                                             \bug What if a technology is not supported in a profile? */
                 phNfcLib_PeerInfo_t * peerInfo , /**< [Out] Pointer to a phNfcLib_PeerInfo_t structure, to be allocated by the caller.
                                                             On successful activation of a peer, activation procedure fills the structure
                                                             with detailed information. */
                 pphNfcLib_ErrCallbck  ErrCallbck /**< [In] Error callback specific to #PH_NFCLIB_ACTIVATION_PROFILE_EMVCO to intimate
                                                            about errors during polling for EMVCo. */
                 );

/**
 * \brief Send Data to an Activated Peer.
 *
 * Once a peer has been activated this function can be used to send data to the peer. The channel over which the data is sent to
 * the peer is defined by the configured \ref activation_profile .
 *
 * \note
 * \li Depending on the \ref activation_profile this function may block until a response has actually
 * been sent back by the peer. For instance #PH_NFCLIB_ACTIVATION_PROFILE_EMVCO requires acknowledgment from the peer for a sent frame,
 * so phNfcLib_Transmit will only return once this acknowledgment has been received.
 *
 * \par Behavior in NFC PROFILE
 *
 *  Behavior in case of \ref activation_profile is #PH_NFCLIB_ACTIVATION_PROFILE_NFC
 * \li In case underlying protocol is ISO18092 this function acts as a SNEP client,  i.e. connecting to a server in order to push the NDEF record.
 * \li In case underlying protocol is ISO14443-4 host card emulation, this function is used to read the record of the NDEF emulation.
 *
 * \par  Behavior in ISO PROFILE
 *
 * Behavior in case of \ref activation_profile is #PH_NFCLIB_ACTIVATION_PROFILE_ISO
 * \li In case detected card is MIFARE Ultralight, MIFARE Classic or MIFARE DESFire, input to phNfcLib_Transmit will be pointer of type
 * \ref nfc_lib_transmit_metadata. User needs to provide correct information in metadata to communicate with tags.
 *
 * \pre NFC Library must be in state ACTIVATED
 *
 * \retval PH_NFCLIB_STATUS_SUCCESS
 * \p wTxBufferLength bytes given in \p pTxBuffer have been successfully sent to the peer.
 * \retval PH_NFCLIB_STATUS_BUFFER_OVERFLOW
 * Received bytes could not be stored in NFCLIB buffer. User needs to call \ref phNfcLib_Receive API to copy data from NFCLIB
 * buffer to application and then call phNfcLib_Transmit API with \p pTxBuffer as NULL to receive next set of data.
 * \retval PH_NFCLIB_STATUS_RF_TIMEOUT_ERROR
 * Sent data not acknowledged in time.
 * \retval PH_NFCLIB_STATUS_RF_PEER_LOST
 * Peer lost during communication.
 * \retval PH_NFCLIB_STATUS_RF_PROTOCOL_ERROR
 * General RF protocol violation.
 * \retval PH_NFCLIB_STATUS_INVALID_STATE
 * NFC Library had been in a wrong state when calling this function.
 */
phNfcLib_Status_t phNfcLib_Transmit(
                 void * const pTxBuffer,     /**< [In] Byte array containing the data to be sent or \link phNfcLib_Transmit Metadata \endlink in ISO mode. */
                 uint16_t wTxBufferLength    /**< [In] Number of bytes to be sent, in fact size of byte array or Zero in case Metadata is sent. */
                 );

/**
 * \brief Receive Data from an Activated Peer.
 *
 * This function can be called in order to receive data from an activated peer, in fact after #phNfcLib_Activate returned with status
 * PH_NFCLIB_STATUS_PEER_ACTIVATION_DONE. Like #phNfcLib_Transmit, data received using this function may get decapsulated from underlying
 * protocol headers, depending on \ref activation_profile configured. See #phNfcLib_Transmit for more details.
 *
 * \note
 * \li Caller is responsible for implementing correct sequence of #phNfcLib_Transmit and #phNfcLib_Receive. So depending
 * on the underlying protocol the two APIs have the be called alternately, starting with either transmit or receive, depending on the
 * underlying technology (initiator vs. target).
 * \li Similar to phNfcLib_Transmit, in case PH_NFCLIB_ACTIVATION_PROFILE_NFC is the active \ref activation_profile and
 * underlying protocol is ISO18092 calling
 *
 * \par Behavior in case of NFC PROFILE
 *
 * Behavior in case of \ref activation_profile is #PH_NFCLIB_ACTIVATION_PROFILE_NFC
 * \li In case underlying protocol is ISO18092 this API waits for receiving an NDEF message via SNEP (i.e. phNfcLib_Activate starts a SNEP
 * server and data received via that server socket can be read by this function).
 * \li In case underlying protocol is ISO14443-4 host card emulation, this function is used to write the record of the NDEF emulation.
 * \pre NFC Library must be in state ACTIVATED
 *
 * \sa phNfcLib_Transmit
 * \retval PH_NFCLIB_STATUS_SUCCESS
 * \p pNumberOfBytes bytes read from peer and stored in \p pRxBuffer.
 * \retval PH_NFCLIB_STATUS_RF_TIMEOUT_ERROR
 * Timeout waiting for data from peer.
 * \retval PH_NFCLIB_STATUS_RF_PEER_LOST
 * Peer lost during communication (for ISO18092 target and or active communication).
 * \retval PH_NFCLIB_STATUS_RF_PROTOCOL_ERROR
 * General RF protocol violation.
 * \retval PH_NFCLIB_STATUS_RF_TRANSMISSION_ERROR
 * Transmission error detected, e.g. RF integrity error.
 * \retval PH_NFCLIB_STATUS_INVALID_STATE
 * NFC Library had been in a wrong state when calling this function.
 * \retval PH_NFCLIB_STATUS_BUFFER_OVERFLOW
 * pMoreDataAvailable parameter is set to NULL and pRxBuffer is too small to receive all data.
 */
phNfcLib_Status_t phNfcLib_Receive(
                 uint8_t * const pRxBuffer,  /**< [Out] Allocated byte array to which the received data has to be written. */
                 uint16_t *pNumberOfBytes,   /**< [In,Out] Maximum receive size, in fact size of \p pRxBuffer. On function return
                                                           it contains the actual number of bytes received, but in case when receiving
                                                           from 18000p3m3 tag actual number of bits are returned.*/
                 uint8_t *pMoreDataAvailable /**< [Out] Set to 1, if not all data could be stored and phNfcLib_Receive needs to be
                                                        called again to receive the remaining data.
                                                        If all data could be received, this parameter is set to 0. If this parameter
                                                        is set to NULL and pRxBuffer is too small to receive all data,
                                                        PH_NFCLIB_STATUS_BUFFER_OVERFLOW is returned. */
                 );


/**
 * \brief Set Configuration Parameter
 *
 * This function is used to configure the phNfcLib module. It takes the parameter indicated by \p bConfigTag which
 * should be one of the \link nfc_lib_config_tags available configuration tags\endlink and sets it to the value given by \p pConfigParam.
 *
 * \note
 * \li For pure integer parameter the helper function #phNfcLib_SetConfig_Value can be used instead.
 * \li phNfcLib_Init resets all configuration parameters to its default value.
 *
 * \pre NFC Library need to be in state INITIALIZED, i.e. after calling phNfcLib_Init, phNfcLib_Deactivate or as long
 * as no activation happened.
 *
 * \retval PH_NFCLIB_STATUS_SUCCESS
 * Parameter Value successfully set.
 * \retval PH_NFCLIB_STATUS_INVALID_PARAMETER
 * Unknown configuration tag or wrong length given (\p wConfigParamLength)
 * \retval PH_NFCLIB_STATUS_INVALID_STATE
 * NFC Library had been in a wrong state when calling this function.
 */
phNfcLib_Status_t phNfcLib_SetConfig(
                 uint8_t bConfigTag,             /**< [In] \link nfc_lib_config_tags A configuration tag\endlink indicating the parameter to set. */
                 const uint8_t* pConfigParam,    /**< [In] An array holding the value to be set for the given parameter. */
                 uint16_t wConfigParamLength     /**< [In] Length of the parameter value, in fact length of \p pConfigParam */
                 );

/**
 * \brief Set Configuration Parameter using a Value
 *
 * Like #phNfcLib_SetConfig this function is used to configure the phNfcLib module.
 * All \link nfc_lib_config_tags configuration tags\endlink which take an unsigned integer as value can be set
 * using this function. In fact it's a convenient short cut for the actual #phNfcLib_SetConfig
 * function, which does not require passing in a pointer in case only an integer value needs to be set.
 *
 * \note
 * \li phNfcLib_Init resets all configuration parameters to its default value.
 *
 * \pre NFC Library need to be in state INITIALIZED, i.e. after calling phNfcLib_Init, phNfcLib_Deactivate or as long
 * as no activation happened for Activation configurations. In case of De-activation configuration NFC Library should
 * not be in state RESET state.
 *
 * \sa phNfcLib_SetConfig
 *
 * \retval PH_NFCLIB_STATUS_SUCCESS
 * Parameter Value successfully set.
 * \retval PH_NFCLIB_STATUS_INVALID_PARAMETER
 * Unknown configuration tag.
 * \retval PH_NFCLIB_STATUS_INVALID_STATE
 * NFC Library had been in a wrong state when calling this function.
 */
phNfcLib_Status_t phNfcLib_SetConfig_Value(
                 uint8_t bConfigTag,     /**< [In] \link nfc_lib_config_tags A configuration tag\endlink indicating the parameter to set. */
                 uint32_t dwValue        /**< [In] The value which has to be set for parameter indicated by \p bConfigTag */
                 );

/**
 * \brief Retrieve a configuration parameter value
 *
 * This is the counterpart to #phNfcLib_SetConfig, i.e. it can be used to read out the value for a given parameter.
 *
 * \sa phNfcLib_SetConfig
 *
 * \pre NFC Library need to be in state INITIALIZED or ACTIVATED
 * as no activation happened.
 *
 * \retval PH_NFCLIB_STATUS_SUCCESS
 * Parameter Value successfully read out.
 * \retval PH_NFCLIB_STATUS_INVALID_PARAMETER
 * Unknown configuration tag or provided buffer too small.
 * \retval PH_NFCLIB_STATUS_INVALID_STATE
 * NFC Library had been in a wrong state when calling this function.
 * \bug should we return a pointer instead, so calling application has no need to allocate memory? (with the risk the pointer might get invalid)
 */
phNfcLib_Status_t phNfcLib_GetConfig(
                 uint8_t bConfigTag,             /**< [In] \link nfc_lib_config_tags A configuration tag\endlink indicating the parameter whose value needs to be read out. */
                 uint8_t* pConfigParam,          /**< [Out] Allocated byte array to which the read out value will be written to. */
                 uint16_t * pConfigParamLength   /**< [In,Out] Size of \p pConfigParam. On function return, it holds the actual number of bytes read. */
                 );

/**
 * \brief Deactivate an Activated Peer.
 *
 * The deactivation procedure can be configured by calling #phNfcLib_SetConfig_Value with configuration tag \ref De-activation_blocking.
 * This configuration parameter influence the behavior of the deactivation procedure only in EMVCo profile.
 *
 * After a peer has been activated using #phNfcLib_Activate and the caller wants to release it again (e.g. after communication
 * sequences using #phNfcLib_Transmit and #phNfcLib_Receive have been finished) this API has to be called.
 * Depending on \a bDeactivateMode parameter the peer gets released using defined release procedure according to the
 * \ref activation_profile. This for instance could indicate if the removal procedure should
 * be performed for #PH_NFCLIB_ACTIVATION_PROFILE_EMVCO. In ISO profile, deactivation procedures like HLTA, HLTB, DSL_REQ etc is performed and
 * allows starting a new \link phNfcLib_Activate activation procedure\endlink.
 * To releases all RF related resources this function should be called with \a bDeactivateMode parameter as #PH_NFCLIB_DEACTIVATION_MODE_RF_OFF.
 * If Deactivation with \a bDeactivateMode parameter as #PH_NFCLIB_DEACTIVATION_MODE_RELEASE fails due to card being removed out of proximity
 * or due to some RF irregularity the user may call Deactivation with \a bDeactivateMode parameter as #PH_NFCLIB_DEACTIVATION_MODE_RELEASE again
 * or Deactivation with \a bDeactivateMode parameter as #PH_NFCLIB_DEACTIVATION_MODE_RF_OFF according to the scenario requirements.
 *
 * \pre NFC Library must be in state ACTIVATED
 * \post NFC Library is in state INITIALIZED
 * \retval PH_NFCLIB_STATUS_SUCCESS
 * Peer successfully deactivated and released.
 * \retval PH_NFCLIB_STATUS_INVALID_STATE
 * NFC Library had been in a wrong state when calling this function.
 * \retval PH_NFCLIB_STATUS_PEER_DEACTIVATION_FAILED
 * Peer deactivation is not successful (this error code is returned only when config \ref De-activation_blocking is set to PH_OFF).
 * Internal state move to Deactivation Ongoing State. #phNfcLib_Deactivate has to be called in loop to check if peer deactivation is successful.
 */
phNfcLib_Status_t phNfcLib_Deactivate(
                 uint8_t bDeactivateMode,         /**< [In] Deactivation mode/behavior.
                                                    Refer to \ref nfc_lib_definitions_deactivation_mode
                                                    for more information. */
                 phNfcLib_PeerInfo_t * peerInfo   /**< [In] Pointer to a phNfcLib_PeerInfo_t structure updated by /ref phNfcLib_Activate API. */
                 );

/**
 * \brief Used to asynchronously stop HAL waiting for RF event and exit from HAL.
 *
* Note: This function will post an 'abort' event which is used by the HAL to move into 'Idle' state and exit from polling for RF Event.
* The functions that were waiting for a HAL event will return #PH_ERR_ABORTED error code once this function is executed.
*
* This function shall be called in a task other than the task where reader library is running.
* This API can be called at any time after the phNfcLib_Init(); and any of the NFCLIB operations like transmit and receive are in progress.
*
* \retval PH_NFCLIB_STATUS_SUCCESS
* Operation successful.
*/
phNfcLib_Status_t phNfcLib_AsyncAbort(void);

#endif /* NXPBUILD__PHNFCLIB_PROFILES */

/**
 * \brief Retrieve the pointer to the dataparams structure of an internal NFC Reader Library component.
 *
 * The NfcLib encapsulates all relevant NFC Reader Library components. In case a caller needs to directly access one of the components
 * a pointer to the dataparams is required. This function returns the pointer to the dataparams structure of the component
 * indicated with \p wComponent.
 * \par Example
 * \code
 * phNfcLib_Init();
 * /-/ manually switch on the RF field
 * phhalHw_FieldOn(phNfcLib_GetDataParams(PH_COMP_HAL));
 * \endcode
 *
 * \return Pointer to the component's dataparam structure. NULL in case phNfcLib is not managing the component indicated by \p wComponent
 * or NFC library is in state RESET.
 */
void* phNfcLib_GetDataParams(
                 uint16_t wComponent     /**< [In] The component ID for which the data params pointer should be returned. For possible value refer to ph_Status.h, PH_COMP_* */
                 );

/**
 * \}
 */

#endif /* NXPBUILD__PHNFCLIB */

#ifdef __cplusplus
} /* Extern C */
#endif

#endif  /* PH_NFCLIB_H */

