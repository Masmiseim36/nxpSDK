
/**
 *  \file BT_hfp_unit_api.h
 *
 *  This file contains the data structures, the type definitions
 *  and API functions that HFP Unit exports to the application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_HFP_UNIT_API_
#define _H_BT_HFP_UNIT_API_

/* ------------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "BT_at_parser_api.h"
#include "at_parser_hsp_hfp.h"


/* ------------------------------------------- Global Definitions */
/**
 * \addtogroup bt_profiles Profiles
 * \{
 */
/**
 * \defgroup hfp_unit_module HFP Unit (Hands Free Profile - Unit)
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  Hands Free Profile Unit module to the Application and other upper
 *  layers of the stack.
 */
/**
 * \defgroup hfp_unit_events Events
 * \{
 * Describes Events for Hands Free Profile - Unit module
 */
/**
 *  HFP Unit Notification Callback Event Types
 */
/** Connection Management Events */
#define HFP_UNIT_CONNECT_CNF                    0x01U
#define HFP_UNIT_CONNECT_IND                    0x02U
#define HFP_UNIT_DISCONNECT_CNF                 0x03U
#define HFP_UNIT_DISCONNECT_IND                 0x04U
#define HFP_UNIT_STOP_CNF                       0x05U

/** Voice Call Events */
#define HFP_UNIT_INCALL_ALERT                   0x11U
#define HFP_UNIT_CLI_DIGITS                     0x12U
#define HFP_UNIT_CALL_WAITING_IND               0x13U
#define HFP_UNIT_CALLSETUP                      0x14U
#define HFP_UNIT_NO_CALL                        0x15U
#define HFP_UNIT_CALL_ACTIVE                    0x16U
#define HFP_UNIT_SEND_DTMF_CNF                  0x17U
#define HFP_UNIT_TWC_CALL_CTRL_CNF              0x18U
#define HFP_UNIT_CALLHANGUP_CNF                 0x19U
#define HFP_UNIT_OUTCALL_CNF                    0x1AU
#define HFP_UNIT_INCALL_ACCEPT_CNF              0x1BU
#define HFP_UNIT_CHLD_MPRTY_CAP                 0x1CU

/** Miscellaneous Indications */
#define HFP_UNIT_AG_ERROR_IND                   0x21U
#define HFP_UNIT_BSIR_IND                       0x22U
#define HFP_UNIT_VGM_IND                        0x23U
#define HFP_UNIT_VGS_IND                        0x24U
#define HFP_UNIT_VOICE_RECOG_IND                0x25U
#define HFP_UNIT_VOICETAG_PHNUM_IND             0x26U
#define HFP_UNIT_AG_SERVICE_IND                 0x27U
#define HFP_UNIT_CIEV_SIGNAL_IND                0x28U
#define HFP_UNIT_CIEV_ROAM_IND                  0x29U
#define HFP_UNIT_CIEV_BATTCHG_IND               0x2AU
#define HFP_UNIT_PEER_IND_STATUS_CNF            0x2BU
#define HFP_UNIT_PEER_IND_STATUS_IND            0x2CU
#define HFP_UNIT_OK_IND                         0x2DU

/** Confirmations for API Requests */
#define HFP_UNIT_VREC_ENABLE_CNF                0x31U
#define HFP_UNIT_VREC_DISABLE_CNF               0x32U
#define HFP_UNIT_ECNR_CNF                       0x33U
#define HFP_UNIT_CLIP_CNF                       0x34U
#define HFP_UNIT_CCWA_CNF                       0x35U
#define HFP_UNIT_VOICETAG_PHNUM_CNF             0x36U
#define HFP_UNIT_SET_VGM_CNF                    0x37U
#define HFP_UNIT_SET_VGS_CNF                    0x38U

/** HFP-1.5 Events */
#define HFP_UNIT_RECVD_BTRH_IND                 0x41U
#define HFP_UNIT_REQ_SUB_NUM_IND                0x42U
#define HFP_UNIT_COPS_QUERY_RESULT_IND          0x43U
#define HFP_UNIT_CURRENT_CALL_LIST_IND          0x44U
#define HFP_UNIT_CALL_HELD_IND                  0x45U
#define HFP_UNIT_CMEE_IND                       0x46U
#define HFP_UNIT_CMEE_CNF                       0x47U
#define HFP_UNIT_REQ_SUB_NUM_CNF                0x48U
#define HFP_UNIT_SET_NW_NAME_FORMAT_CNF         0x49U
#define HFP_UNIT_ADV_CALL_HOLD_CNF              0x4AU
#define HFP_UNIT_COPS_QUERY_CNF                 0x4BU
#define HFP_UNIT_CURRENT_CALL_LIST_CNF          0x4CU
#define HFP_UNIT_SEND_BTRH_CNF                  0x4DU

/** HFP-1.6 Events */
#define HFP_UNIT_BIA_CNF                        0x4EU
#define HFP_UNIT_BAC_CNF                        0x4FU
#define HFP_UNIT_BCC_CNF                        0x50U
#define HFP_UNIT_BCS_CNF                        0x51U
#define HFP_UNIT_BCS_IND                        0x52U

/** HFP-1.7 Events */
#define HFP_UNIT_BIEV_CNF                       0x53U
#define HFP_UNIT_BIND_IND                       0x54U

/** HFP-1.8 Events */
#define HFP_UNIT_ENH_VREC_ENABLE_CNF            0x55U

/** Data Mode Events */
#define HFP_UNIT_SEND_DATA_CNF                  0xE0U
#define HFP_UNIT_RECVD_DATA_IND                 0xE1U
/** \} */
/**
 * \defgroup hfp_unit_defines Defines
 * \{
 * Describes defines for Hands Free Profile - Unit module
 */
/**
 * \defgroup hfp_unit_constants Constants
 * \{
 * Describes Constants defined by the module.
 */
/**
 *  List of HFP Commands
 */
/** Service Level Connection Commands */
#define HFP_UNIT_AT_BRSF                        0x00U
#define HFP_UNIT_AT_CIND_TEST                   0x01U
#define HFP_UNIT_AT_CIND_READ                   0x02U
#define HFP_UNIT_AT_CMER_ENABLE                 0x03U
#define HFP_UNIT_AT_CHLDQ                       0x04U

/** Commands without User Data (except SLC Commands) */
#define HFP_UNIT_AT_CHUP                        0x05U  /**< Hang up */
#define HFP_UNIT_AT_ATA                         0x06U  /**< Accept */

/** Multiparty */
#define HFP_UNIT_AT_CHLDV0                      0x07U
#define HFP_UNIT_AT_CHLDV1                      0x08U
#define HFP_UNIT_AT_CHLDV2                      0x09U
#define HFP_UNIT_AT_CHLDV3                      0x0AU
#define HFP_UNIT_AT_CHLDV4                      0x0BU

/** CLIP */
#define HFP_UNIT_AT_CLIP_ENABLE                 0x0CU
#define HFP_UNIT_AT_CLIP_DISABLE                0x0DU

/** Call Waiting */
#define HFP_UNIT_AT_CCWAL_ENABLE                0x0EU
#define HFP_UNIT_AT_CCWAL_DISABLE               0x0FU

/** Voice Recognition */
#define HFP_UNIT_AT_BVRA_ENABLE                 0x10U
#define HFP_UNIT_AT_BVRA_DISABLE                0x11U

#define HFP_UNIT_AT_NREC_DISABLE                0x12U /**< Disable ECNR */
#define HFP_UNIT_AT_BINPV1                      0x13U /**< Voice Tag */
#define HFP_UNIT_AT_BLDN                        0x14U /**< Redial */

/** Commands with User Data (except SLC Commands) */
#define HFP_UNIT_AT_DD                          0x15U /**< Dial */
#define HFP_UNIT_AT_DM                          0x16U /**< Memdial */
#define HFP_UNIT_AT_VGS                         0x17U /**< Speaker Gain */
#define HFP_UNIT_AT_VGM                         0x18U /**< Microphone gain */
#define HFP_UNIT_AT_VTS                         0x19U /**< DTMF */

/** HFP-1.5 Commands */
#define HFP_UNIT_AT_CNUM                        0x1AU /**< Subscriber Number */
#define HFP_UNIT_AT_QRAH                        0x1BU /**< Query Resp & Hold */
#define HFP_UNIT_AT_S_NW_NAME_FORMAT            0x1CU /**< Set NW Name Format */
#define HFP_UNIT_AT_COPS_Q                      0x1DU /**< Operator Selection */
#define HFP_UNIT_AT_CLCC                        0x1EU /**< List Current Calls */

/** Send Response & Hold */
#define HFP_UNIT_AT_BTRH_HOLD                   0x1FU
#define HFP_UNIT_AT_BTRH_ACCEPT                 0x20U
#define HFP_UNIT_AT_BTRH_REJECT                 0x21U

/** Enhanced Error Reporting */
#define HFP_UNIT_AT_CMEE_ENABLE                 0x22U
#define HFP_UNIT_AT_CMEE_DISABLE                0x23U

/** Advanced Call Hold */
#define HFP_UNIT_AT_ADV_CHLDV                   0x24U

/** HFP-1.6 Commands */
/** Individual Indicator Activation */
#define HFP_UNIT_AT_BIA                         0x25U
/** Available Codecs */
#define HFP_UNIT_AT_BAC                         0x26U
/** Trigger Codec Connection */
#define HFP_UNIT_AT_BCC                         0x27U
/** Codec Confirmation */
#define HFP_UNIT_AT_BCS                         0x28U

/** HFP-1.7 Commands */
/** List HF Indicators to AG */
#define HFP_UNIT_AT_BIND_LIST                   0x29U
/** Get AG Supported HF Indicators */
#define HFP_UNIT_AT_BIND_TEST                   0x2AU
/** Read Status of HF Indicators in AG */
#define HFP_UNIT_AT_BIND_READ                   0x2BU
/** Update HF Indicator Value */
#define HFP_UNIT_AT_BIEV                        0x2CU

/** HFP-1.8 Commands */
/**
 * This command is used:
 * 1. To indicate the HF Unit is ready to accept audio when the
 *    Audio Connection is First established.
 * 2. This command can also be used to terminate the ongoing VR session
 *    and prepare for a new session.
 */
#define HFP_UNIT_AT_BVRA_ACCEPT_AUDIO           0x2DU

/** Data Mode (Send String) Commands */
#define HFP_UNIT_AT_SEND_STRING                 0xE0U

/**
 *  Type of HFP Unit Device (HFP_UNIT_DEVICE_TYPE)
 */
#define HFP_UNIT_DEVICE_SPEAKER                 0x00U
#define HFP_UNIT_DEVICE_MICROPHONE              0x01U

/**
 *  Three Way Call (TWC) Options (HFP_UNIT_TWC_CONTROL_TYPE)
 */
#define HFP_UNIT_TWC_REJECT_NEW_WAITING_CALL    0x00U
#define HFP_UNIT_TWC_ACCEPT_NEW_WAITING_CALL    0x01U
#define HFP_UNIT_TWC_CALL_HOLD                  0x02U
#define HFP_UNIT_TWC_CALL_CONFERENCE            0x03U
#define HFP_UNIT_TWC_TERMINATE_SELF_FROM_TWC    0x04U

/**
 *  Supported Features (HFP_UNIT_FEATURE)
 */
#define HFP_UNIT_FEATURE_CLIP                   0x00U
#define HFP_UNIT_FEATURE_CCWA                   0x01U
#define HFP_UNIT_FEATURE_ECNR                   0x02U
#define HFP_UNIT_FEATURE_BVRA                   0x03U
#define HFP_UNIT_FEATURE_BINP                   0x04U

/**
 *  To Enable/Disable AG to send HFP Indications (HFP_UNIT_ACTION_TYPE)
 *  For example, CLIP.
 */
#define HFP_UNIT_ACTION_DISABLE                 0x00U
#define HFP_UNIT_ACTION_ENABLE                  0x01U

/** HFP 1.8 related */
#define HFP_UNIT_ACTION_ENH_VREC_ENABLE         0x02U

/**
 *  Types for Response & Hold Commands (HFP_UNIT_HELD_INCALL_OPTIONS)
 */
#define HFP_UNIT_RANDH_HOLD                     0x00U
#define HFP_UNIT_RANDH_ACCEPT                   0x01U
#define HFP_UNIT_RANDH_REJECT                   0x02U

/**
 * HF Unit BRSF Command Supported Feature Bit-Masks
 */
#define HFP_UNIT_BRSF_CMD_FEATURE_BIT_ECNR             0U
#define HFP_UNIT_BRSF_CMD_FEATURE_BIT_TWC              1U
#define HFP_UNIT_BRSF_CMD_FEATURE_BIT_CLIP             2U
#define HFP_UNIT_BRSF_CMD_FEATURE_BIT_VRA              3U
#define HFP_UNIT_BRSF_CMD_FEATURE_BIT_REM_VOL_CTRL     4U
#define HFP_UNIT_BRSF_CMD_FEATURE_BIT_ENH_CALL_STAT    5U
#define HFP_UNIT_BRSF_CMD_FEATURE_BIT_ENH_CALL_CTRL    6U
#define HFP_UNIT_BRSF_CMD_FEATURE_BIT_CODEC_NEGO       7U
#define HFP_UNIT_BRSF_CMD_FEATURE_BIT_HF_INDICATORS    8U
#define HFP_UNIT_BRSF_CMD_FEATURE_BIT_ESCO_S4_SETTING  9U

/** HFP 1.8 related */
#define HFP_UNIT_BRSF_CMD_FEATURE_BIT_ENH_VR_STATUS   10U
#define HFP_UNIT_BRSF_CMD_FEATURE_BIT_VR_TEXT         11U

/**
 * HF Unit BRSF Response Supported Feature Bit-Masks
 */
#define HFP_UNIT_BRSF_RSP_FEATURE_BIT_TWC              0U
#define HFP_UNIT_BRSF_RSP_FEATURE_BIT_ECNR             1U
#define HFP_UNIT_BRSF_RSP_FEATURE_BIT_VRA              2U
#define HFP_UNIT_BRSF_RSP_FEATURE_BIT_INBAND_RING      3U
#define HFP_UNIT_BRSF_RSP_FEATURE_BIT_VOICE_TAG        4U
#define HFP_UNIT_BRSF_RSP_FEATURE_BIT_CALL_REJECT      5U
#define HFP_UNIT_BRSF_RSP_FEATURE_BIT_ENH_CALL_STAT    6U
#define HFP_UNIT_BRSF_RSP_FEATURE_BIT_ENH_CALL_CTRL    7U
#define HFP_UNIT_BRSF_RSP_FEATURE_BIT_EXT_ERROR_CODES  8U
#define HFP_UNIT_BRSF_RSP_FEATURE_BIT_CODEC_NEGO       9U
#define HFP_UNIT_BRSF_RSP_FEATURE_BIT_HF_INDICATORS   10U
#define HFP_UNIT_BRSF_RSP_FEATURE_BIT_ESCO_S4_SETTING 11U

/** HFP 1.8 related */
#define HFP_UNIT_BRSF_RSP_FEATURE_BIT_ENH_VR_STATUS   12U
#define HFP_UNIT_BRSF_RSP_FEATURE_BIT_VR_TEXT         13U

/** HFP Codec IDs  */
#define HFP_UNIT_CODEC_ID_CVSD                         0x01U
#define HFP_UNIT_CODEC_ID_MSBC                         0x02U

/** HF Indicator IDs */
#define HFP_UNIT_HF_IND_ID_ENHANCED_SAFETY             0x0001U
#define HFP_UNIT_HF_IND_ID_BATTERY_LEVEL               0x0002U

/* ======= HFP 1.8 related defines ======= */
/**
 * Bitmask to reflect the current state of Voice recognition engine in AG.
 */
#define HFP_ENH_VR_STATE_AG_READY_2_ACCEPT_AUDIO_INPUT      0x01U
#define HFP_ENH_VR_STATE_AG_SENDING_AUDIO_TO_HF             0x02U
#define HFP_ENH_VR_STATE_AG_PROCESSING_AUDIO_INPUT          0x04U

/**
 * Voice Recognition Text Type defines
 */
#define HFP_ENH_VR_TEXT_TYPE_TEXT_FROM_HF_AUDIO             0x00U
#define HFP_ENH_VR_TEXT_TYPE_TEXT_FROM_AG_AUDIO             0x01U
#define HFP_ENH_VR_TEXT_TYPE_TEXT_FROM_AG_AUDIO_WITH_Q      0x02U
#define HFP_ENH_VR_TEXT_TYPE_TEXT_FROM_AG_AUDIO_WITH_ERR    0x03U

/**
 * Voice Recognition Text Operation defines
 */
#define HFP_ENH_VR_TEXT_OP_NEW_TEXT                         0x01U
#define HFP_ENH_VR_TEXT_OP_REPLACE                          0x02U
#define HFP_ENH_VR_TEXT_OP_APPEND                           0x03U
/** \} */
/** \} */
/* ------------------------------------------- Macros */
/**
 * \defgroup hfp_unit_utility_macros Utility Macros
 * \{
 * Describes Utility Macros defined by the module.
 */
#define HFP_UNIT_BRSF_IS_FEATURE_SET(value, feature) \
        ((value) & (1U << (feature)))? (BT_TRUE): (BT_FALSE)
/** \} */
/* ------------------------------------------- Structures/Data Types */
/**
 * \addtogroup hfp_unit_defines Defines
 * \{
 */
/**
 * \defgroup hfp_unit_structures Structures
 * \{
 * Describes Constants defined by the module.
 */
/**
 * \typedef HFP_UNIT_EVENTS
 *
 * \par Description:
 *      HFP Unit Callback Events
 */
typedef UCHAR HFP_UNIT_EVENTS;

/**
 * \typedef HFP_UNIT_HANDLE
 *
 * \par Description:
 *      HFP Unit Handle
 */
typedef UINT32 HFP_UNIT_HANDLE;

/**
 * \typedef HFP_UNIT_DEVICE_TYPE
 *
 * \par Description:
 *      HFP Unit Device Type
 */
typedef UCHAR HFP_UNIT_DEVICE_TYPE;

/**
 * \typedef HFP_UNIT_FEATURE
 *
 * \par Description:
 *      HFP Unit Feature Type (CLIP, CWA, VRA etc.)
 */
typedef UCHAR HFP_UNIT_FEATURE;

/**
 * \typedef HFP_UNIT_FEATURE
 *
 * \par Description:
 *      HFP Unit Action Type
 *     (To Enable/Disable AG to send HFP Indications)
 */
typedef UCHAR HFP_UNIT_ACTION_TYPE;

/**
 * \typedef HFP_UNIT_COMMAND
 *
 * \par Description:
 *      HFP Unit Command Value to be sent
 */
typedef UCHAR HFP_UNIT_COMMAND;

/**
 * \typedef HFP_UNIT_TWC_CONTROL_TYPE
 *
 * \par Description:
 *      HFP Unit Two Way Call (TWC) Control Type
 */
typedef UCHAR HFP_UNIT_TWC_CONTROL_TYPE;

/**
 * \typedef HFP_UNIT_HELD_INCALL_OPTIONS
 *
 * \par Description:
 *      HFP Unit Response & Hold Command Options
 */
typedef UCHAR HFP_UNIT_HELD_INCALL_OPTIONS;

/**
 * \typedef HFP_UNIT_HELD_INCALL_OPTIONS
 *
 * \par Description:
 *      HFP Unit Codec ID
 */
typedef UINT8 HFP_UNIT_CODEC_ID;

/**
 *  The CIND Indicator Names are sent by the peer (HFP Audio Gateway)
 */
#define HFP_UNIT_IND_SERVICE            AT_PARSER_IND_SERVICE
#define HFP_UNIT_IND_CALL               AT_PARSER_IND_CALL
#define HFP_UNIT_IND_CALLSETUP          AT_PARSER_IND_CALLSETUP
#define HFP_UNIT_IND_CALL_SETUP         AT_PARSER_IND_CALL_SETUP
#define HFP_UNIT_IND_SIGNAL             AT_PARSER_IND_SIGNAL
#define HFP_UNIT_IND_ROAM               AT_PARSER_IND_ROAM
#define HFP_UNIT_IND_BATTCHG            AT_PARSER_IND_BATTCHG
#define HFP_UNIT_IND_CALL_HELD          AT_PARSER_IND_CALL_HELD
#define HFP_UNIT_IND_UNKNOWN            AT_PARSER_IND_UNKNOWN

#define HFP_UNIT_CIND_INDICATOR_NAME    AT_PARSER_CIND_INDICATOR_NAME

/**
 *  Data Type to represent maximum & minimum values for Indicators mandated
 *  by the HFP-1.5 specification. More Indicators may be addded in future,
 *  as and when required.
 */
#define HFP_UNIT_CIND_INDICATOR_VALUE   AT_PARSER_CIND_INDICATOR_VALUE

/**
 *  Data Type to represent the results obtained from peer for the
 *  CIND Test Command (AT+CIND=?)
 */
#define HFP_UNIT_CIND_TEST_RESULT       AT_PARSER_CIND_TEST_RESULT

/**
 *  Data Type to represent the value of Indicators obtained from peer as
 *  response to CIND Read Command sent by Unit(AT+CIND?).
 *  More Indicators may be addded in future, as and when required.
 *
 *  Each element holds the corresponding value received in CIND Read response.
 *  For Example: If 'service' indicator is supported and its value is
 *  1 in CIND Read Response, then 1 will be stored in member 'service' of
 *  this structure.
 */
#define HFP_UNIT_CIND_READ_RESULT       AT_PARSER_CIND_READ_RESULT

/**
 *  \typedef HFP_UNIT_EVENT_NOTIFICATION_CB
 *
 *  \par Description:
 *       HFP Unit Event Notification Callback Type.
 *       Application should register a callback through the API BT_hfp_unit_init().
 *       HFP Unit will call the registered application callback after
 *       - Processing the APIs called by application.
 *       - Receiving connect and disconnect events initiated from the HF AG.
 *       - Receiving events for HF scenarios from the HF AG.
 *       - Stopping profile if it was connected when the stop API call was made.
 *
 *       In the callback, events of type \ref HFP_UNIT_EVENTS will be indicated along with the parameters.
 *
 *  \param [in] handle
 *         Identifies the Connection Instance.
 *
 *  \param [in] event_type
 *         Event indicated from profile to Application.
 *
 *  \param [in] event_result
 *         Status of event received.
 *
 *  \param [in] event_data
 *         Pointer to the array containing the parameters associated with the event.
 *
 *  \param [in] event_datalen
 *         Length of the parameters associated with the event.
 */
typedef API_RESULT (* HFP_UNIT_EVENT_NOTIFICATION_CB)
                   (
                       /* IN */  HFP_UNIT_HANDLE   handle,
                       /* IN */  HFP_UNIT_EVENTS   event_type,
                       /* IN */  API_RESULT        event_result,
                       /* IN */  void *            event_data,
                       /* IN */  UINT16            event_datalen
                   );
/**
 * \defgroup hfp_unit_structures Structures
 * \{
 * Describes Structures defined by the module.
 */

/**
 *  \typedef HFP_UNIT_APPL_CONFIG_PARAMS
 *
 *  \par Description:
 *       Data Type to represent the parameters received from the Application layer,
 *       required to start the HFU.
 */
typedef struct
{
    /** HFP Unit supported features in decimal format */
    UCHAR *   supp_features;

    /** HFP Unit available codecs in decimal format */
    UCHAR *   available_codecs;

    /** No. of octets used to represent supported features */
    UINT16    supp_features_len;

    /** No. of octets used to represent supported features */
    UINT16    available_codecs_len;

#ifdef HFP_UNIT_1_7
    /** HFP Unit Supported HF Indicator List */
    UCHAR *    hf_indicators;

    /** No. of octets used to represent the HF indicators List */
    UINT16     hf_indicators_len;

#endif /* HFP_UNIT_1_7 */

    /** Local server channel for accepting incoming HFP Unit connection */
    UCHAR     server_channel;

} HFP_UNIT_APPL_CONFIG_PARAMS;

/**
 *  \typedef HFP_UNIT_CHLD_RESP_RESULT
 *
 *  \par Description:
 *       Data Type to represent the parameters of CHLD, received from peer
 *       (HFP Audio Gateway)
 */
typedef struct
{
    /** Number of CHLD Parameters */
    UCHAR hold_count;

    /** Value of CHLD Parameters */
    UCHAR peer_hold_value [HFP_UNIT_MAX_HOLD_TYPES];

} HFP_UNIT_CHLD_RESP_RESULT;

/**
 *  Data Type to represent the parameters of +BIND, received from peer
 *  (HFP Audio Gateway)
 */
#define  HFP_UNIT_BIND_TEST_RESULT       AT_PARSER_BIND_TEST_RESULT
#define  HFP_UNIT_BIND_READ_RESULT       AT_PARSER_BIND_READ_RESULT

/**
 *  \typedef HFP_UNIT_ADV_CALL_HOLD
 *
 *  \par Description:
 *       Data Type to hold values for Call Control options (0-4) and Call ID
 */
typedef struct
{
    /** Call Control Option */
    HFP_UNIT_TWC_CONTROL_TYPE    option;

    /** Call ID */
    UCHAR                        call_id;

} HFP_UNIT_ADV_CALL_HOLD;


/**
 *  \typedef HFP_UNIT_ADV_CALL_HOLD
 *
 *  \par Description:
 *       Data Type which is provided to Application, over HFP-Unit, with HFP Unit
 *       Event Notification Callback, for certain Events.
 *
 *       Refer to the HFP Unit API Document to understand what values are
 *       populated for the data type elements, for every event.
 */
typedef struct
{
    /** AT Parser Response */
    AT_PARSER_RESPONSE *    parser_resp;

    /** Buffer containing the AT Response String */
    UCHAR *                 buffer;

    /** Size of buffer containing the AT Response String */
    UINT16                  buffer_len;

} HFP_UNIT_APP_DATA;
/** \} */
/** \} */
/** \} */
/* ------------------------------------------- Internal Functions */
#ifdef __cplusplus
extern "C" {
#endif
/**
 * \defgroup hfp_unit_api_definitions API Definitions
 * \{
 * Describes API definitions of this module.
 */

/**
 *  These are internal functions required to implement functions & macros
 *  exported for application use. These functions should not be called
 *  directly by application.
 */

/** To request a HFP Unit service over an established connection */
API_RESULT hfp_unit_service_request
           (
               /* IN */  HFP_UNIT_HANDLE     handle,
               /* IN */  HFP_UNIT_COMMAND    command,
               /* IN */  void *              data,
               /* IN */  UCHAR               data_len
           );

#ifdef HFP_UNIT_1_5
API_RESULT hfp_unit_service_request_1_5
           (
               /* IN */  HFP_UNIT_HANDLE     handle,
               /* IN */  HFP_UNIT_COMMAND    command,
               /* IN */  void *              data,
               /* IN */  UCHAR               data_len
           );
#endif /* HFP_UNIT_1_5 */

#ifdef HFP_UNIT_1_6
API_RESULT hfp_unit_service_request_1_6
           (
               /* IN */  HFP_UNIT_HANDLE     handle,
               /* IN */  HFP_UNIT_COMMAND    command,
               /* IN */  void *              data,
               /* IN */  UCHAR               data_len
           );
#endif /* HFP_UNIT_1_6 */

#ifdef HFP_UNIT_1_7
API_RESULT hfp_unit_service_request_1_7
           (
               /* IN */  HFP_UNIT_HANDLE     handle,
               /* IN */  HFP_UNIT_COMMAND    command,
               /* IN */  void *              data,
               /* IN */  UCHAR               data_len
           );
#endif /* HFP_UNIT_1_7 */

/* ------------------------------------------- Function Declarations */
/**
 *  \brief To intialize the HFP Unit
 *
 *  \par Description:
 *       This API registers application callback function with the HFP Unit
 *       and initializes the HF profile.
 *
 *  \param [in] event_notification_cb
 *         Application callback function pointer.
 *
 *  \return
 *      API_SUCCESS: Indicates initialization done successfully
 *      Relevant HF Unit error code describing the reason for failure.
 */
API_RESULT BT_hfp_unit_init
           (
               /* IN */  HFP_UNIT_EVENT_NOTIFICATION_CB    event_notification_cb
           );

#ifndef HFP_UNIT_1_6
/**
 *  \brief To start HFP Unit and start its services
 *
 *  \par Description:
 *       This API starts the HFP profile, i.e. enables the application to use the services
 *       of HFP Unit and starts waiting for incoming connections on the RFCOMM Channel specified.
 *
 *  \param [in] server_channel
 *          Local server channel for accepting incoming HFP Unit connection
 *
 *  \param [in] supp_features
 *         HFP Unit supported features in decimal format
 *
 *  \param [in] supp_features_len
 *         No. of octets used to represent supported features
 *
 *  \return
 *      API_SUCCESS: Indicates HF service started successfully
 *      Relevant HF Unit error code describing the reason for failure.
 */
API_RESULT BT_hfp_unit_start
           (
               /* IN */  UCHAR      server_channel,
               /* IN */  UCHAR *    supp_features,
               /* IN */  UINT16     supp_features_len
           );
#else /* HFP_UNIT_1_6 */
/**
 *  \brief To start HFP Unit and start services
 *
 *  \par Description:
 *       This API starts the HFP profile. i.e., enables the application to use the services
 *       of HFP Unit and starts waiting for incoming connections on the RFCOMM Channel specified.
 *
 *  \param [in] p_hfp_unit_appl_conf_params
 *         Pointer to the APPL CONFIG params (application configuration parameters) structure
 *
 *  \return
 *      API_SUCCESS: Indicates HF service started successfully
 *      Relevant HF Unit error code describing the reason for failure.
 */
API_RESULT BT_hfp_unit_start
           (
               /* IN */  HFP_UNIT_APPL_CONFIG_PARAMS *p_hfp_unit_appl_conf_params
           );
#endif /* HFP_UNIT_1_6 */

/**
 *  \brief To stop HFP Unit and its services
 *
 *  \par Description:
 *       This API allows the application to stop using the HFP Unit service.
 *       Once stopped, HFP Unit APIs are disabled and incoming connections will not be accepted by HFP Unit.
 *
 *  \return
 *      API_SUCCESS: Successful return from API
 *      Relevant HF Unit error code describing the reason for failure.
 *
 *  \note Event is indicated through the application callback on completion of this procedure.
 *        HFP_UNIT_ERR_STOP_PENDING indicates that there is active Hands-Free connection that needs to be released.
 *        Application should wait until HFP_UNIT_STOP_CNF event is indicated in the callback.
 *        A return API_SUCCESS indicates that HFP service is stopped
 */
API_RESULT BT_hfp_unit_stop
           (
               void
           );

/**
 *  \brief To establish HFP Unit connection with remote HFP Audio Gateway
 *
 *  \par Description:
 *       This API allows the application to establish HFP Unit connection with remote HFP Audio Gateway.
 *
 *  \param [in] bd_addr
 *         BD_ADDR of the remote AG.
 *
 *  \param [in] server_channel
 *         The RFCOMM server channel of AG.
 *
 *  \return
 *      API_SUCCESS: Indicates successfully started connection procedure.
 *      Relevant HF Unit error code describing the reason for failure.
 *
 *  \note
 *      Event is indicated through the application callback on completion of this procedure.
 *      In the callback, application receives connection handle and BD_ADDR for the established service connection.
 *      Application has to use this connection handle to request any API service from HFP Unit.
 *      Server channel of AG is obtained through service discovery procedures performed by Application.
 *      Refer for service discovery APIs.
 */
API_RESULT BT_hfp_unit_connect
           (
               /* IN */  UCHAR *    bd_addr,
               /* IN */  UCHAR      server_channel
           );

/**
 *  \brief To disconnect HFP Unit connection with a remote HFP Audio Gateway
 *
 *  \par Description:
 *       This API releases existing service level connection between HFP Unit and
 *       remote HFP AG described by connection handle.
 *
 *  \param [in] handle
 *         Connection Handle to identify the connection to remote AG.
 *
 *  \return
 *      API_SUCCESS: Indicates successfully started connection procedure.
 *      Relevant HF Unit error code describing the reason for failure.
 *
 *  \note
 *      Event is indicated through the application callback on completion of this procedure.
 */
API_RESULT BT_hfp_unit_disconnect
           (
               /* IN */  HFP_UNIT_HANDLE    handle
           );

/**
 *  \brief To abort current ongoing service request (except service level
 *         connection), and reset HFP Unit state
 *
 *  \par Description:
 *       This API allows user to abort current ongoing procedure and reset HFU profile state.
 *
 *  \param [in] handle
 *         Connection Handle to identify the connection to remote AG.
 *
 *  \return
 *      API_SUCCESS: Indicates successfully started connection procedure.
 *      Relevant HF Unit error code describing the reason for failure.
 */
API_RESULT BT_hfp_unit_abort
           (
               /* IN */  HFP_UNIT_HANDLE    handle
           );

/**
 *  \brief To get peer HFP Audio Gateway's Indicator List.
 *
 *  \par Description:
 *       This API allows user to get the list of indicators supported by AG.
 *
 *  \param [in] handle
 *         Connection Handle to identify the connection to remote AG.
 *
 *  \param [in] indicators
 *         Pointer to the CIND TEST result structure.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
API_RESULT BT_hfp_unit_get_peer_indicator_list
           (
               /* IN */  HFP_UNIT_HANDLE                handle,
               /* OUT */ HFP_UNIT_CIND_TEST_RESULT *    indicators
           );

/**
 *  \brief To get peer HFP Audio Gateway's Call Hold values of supported
 *         Call Hold options, for a multi-party call.
 *
 *  \par Description:
 *       This API allows user to get the call hold values of call hold options
 *       supported by AG for a multi party call
 *
 *  \param [in] handle
 *         Connection Handle to identify the connection to remote AG.
 *
 *  \param [in] peer_chld_values
 *         Pointer to CHLD RESP result structure.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
API_RESULT BT_hfp_unit_get_peer_chld_values
           (
               /* IN */  HFP_UNIT_HANDLE                handle,
               /* OUT */ HFP_UNIT_CHLD_RESP_RESULT *    peer_chld_values
           );

/**
 *  \brief To get the peer supported features received during the SLC procedure.
 *
 *  \par Description:
 *       This API allows user to read the peer supported features that get exchanged
 *       during SLC procedure.
 *
 *  \param [in] handle
 *         Connection Handle to identify the connection to remote AG.
 *
 *  \param [in] peer_supp_features
 *         Pointer to a UINT32 variable to get the peer supported features.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
API_RESULT BT_hfp_unit_get_peer_supported_features
           (
               /* IN */  HFP_UNIT_HANDLE    handle,
               /* OUT */ UINT32           * peer_supp_features
           );

/**
 *  \brief To get peer HFP Audio Gateway's Indicator Status.
 *
 *  \par Description:
 *       This API allows user to get the current status of AG's indicators.
 *
 *  \param [in] hdl
 *         Connection Handle to identify the connection to remote AG.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
#define BT_hfp_unit_get_peer_indicator_status(hdl) \
        hfp_unit_service_request                   \
        (                                          \
            (hdl),                                 \
            HFP_UNIT_AT_CIND_READ,                 \
            NULL,                                  \
            0U                                     \
        )

/**
 *  \brief HFP Unit Call Management: Dial.
 *
 *  \par Description:
 *       This API allows user to dial a number.
 *
 *  \param [in] hdl
 *         Connection Handle to identify the connection to remote AG.
 *
 *  \param [in] dg
 *         Dialed digits in decimal format.
 *
 *  \param [in] len
 *         Number of octets used to represent the dialed digits in
 *         decimal format.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
#define BT_hfp_unit_dial(hdl, dg, len) \
        hfp_unit_service_request       \
        (                              \
            (hdl),                     \
            HFP_UNIT_AT_DD,            \
            (dg),                      \
            (len)                      \
        )

/**
 *  \brief HFP Unit Call Management: Redial.
 *
 *  \par Description:
 *       This API allows user to redial the last number dialed.
 *
 *  \param [in] hdl
 *         Connection Handle to identify the connection to remote AG.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
#define BT_hfp_unit_redial(hdl)  \
        hfp_unit_service_request \
        (                        \
            (hdl),               \
            HFP_UNIT_AT_BLDN,    \
            NULL,                \
            0U                   \
        )

/**
 *  \brief HFP Unit Call Management: Memdial.
 *
 *  \par Description:
 *       This API allows user to make a call using memory dial facility of AG.
 *
 *  \param [in] hdl
 *         Connection Handle to identify the connection to remote AG.
 *
 *  \param [in] dg
 *         Memory ID in decimal format.
 *
 *  \param [in] len
 *         Number of octets used to represent the memory ID in
 *         decimal format.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
#define BT_hfp_unit_memdial(hdl, dg, len) \
        hfp_unit_service_request          \
        (                                 \
            (hdl),                        \
            HFP_UNIT_AT_DM,               \
            (dg),                         \
            (len)                         \
        )

/**
 *  \brief HFP Unit Call Management: Call Accept.
 *
 *  \par Description:
 *       This API allows the application to accept an incoming call.
 *
 *  \param [in] hdl
 *         Connection Handle to identify the connection to remote AG.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
#define BT_hfp_unit_callaccept(hdl) \
        hfp_unit_service_request    \
        (                           \
            (hdl),                  \
            HFP_UNIT_AT_ATA,        \
            NULL,                   \
            0U                      \
        )

/**
 *  \brief HFP Unit Call Management: Call Hangup.
 *
 *  \par Description:
 *       This API allows the application to hang up the ongoing call.
 *
 *  \param [in] hdl
 *         Connection Handle to identify the connection to remote AG.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
#define BT_hfp_unit_callhangup(hdl) \
        hfp_unit_service_request    \
        (                           \
            (hdl),                  \
            HFP_UNIT_AT_CHUP,       \
            NULL,                   \
            0U                      \
        )

/**
 *  \brief HFP Unit Call Management: Send DTMF.
 *
 *  \par Description:
 *       This API allows user to send DTMF code to AG.
 *
 *  \param [in] hdl
 *         Connection Handle to identify the connection to remote AG.
 *
 *  \param [in] dg
 *         DTMF code in decimal format.
 *
 *  \param [in] len
 *         Number of octets used to represent the DTMF code in decimal format.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
#define BT_hfp_unit_send_dtmf(hdl, dg, len) \
        hfp_unit_service_request            \
        (                                   \
            (hdl),                          \
            HFP_UNIT_AT_VTS,                \
            (dg),                           \
            (len)                           \
        )

/**
 *  \brief HFP Unit Call Management: Control Multi-party Call.
 *
 *  \par Description:
 *       This API allows user to
 *       - Reject new waiting call
 *       - Accept new waiting call and release all held calls
 *       - Hold waiting call
 *       - Conference all calls
 *       - Release self from multi party call.
 *
 *  \param [in] hdl
 *         Connection Handle to identify the connection to remote AG.
 *
 *  \param [in] ct
 *         Type of operation for TWC, i.e. 'HFP_UNIT_TWC_CONTROL_TYPE'.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
#define BT_hfp_unit_control_multiparty_call(hdl, ct) \
        (((ct) > 4)                                  \
         ? HFP_UNIT_ERR_INVALID_PARAMETERS           \
         : hfp_unit_service_request                  \
           (                                         \
               (hdl),                                \
               (UCHAR) (HFP_UNIT_AT_CHLDV0 + (ct)),  \
               NULL,                                 \
               0U                                    \
           ))

/**
 *  \brief To set/change Mic/Speaker Gain.
 *
 *  \par Description:
 *       This API allows user to change the microphone/speaker gain settings
 *       and synchronizes the same as that of the AG
 *
 *  \param [in] handle
 *         Connection Handle to identify the connection to remote AG.
 *
 *  \param [in] type
 *         Component type, i.e. microphone/speaker.
 *
 *  \param [in] gain
 *         Value of Gain or Volume level (Allowed range [0-15])
 *
 *  \param [in] gain_length
 *         Number of octets used to represent Gain or Volume level.
 *         (Allowed value is 1 or 2 ).
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
API_RESULT BT_hfp_unit_set_gain
           (
               /* IN */  HFP_UNIT_HANDLE         handle,
               /* IN */  HFP_UNIT_DEVICE_TYPE    type,
               /* IN */  UCHAR *                 gain,
               /* IN */  UCHAR                   gain_length
           );

/**
 *  \brief To enable/disable HFP Unit features.
 *
 *  \par Description:
 *       This API allows user to enable/disable the following features,
 *       - Caller ID Indication
 *       - Call Waiting
 *       - Voice recognition
 *       - ECNR
 *       - Voice tag phone number
 *
 *  \param [in] handle
 *         Connection Handle to identify the connection to remote AG.
 *
 *  \param [in] feature
 *         Type of feature.
 *
 *  \param [in] action
 *         Enable or disable feature.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
API_RESULT BT_hfp_unit_feature_control
           (
               /* IN */  HFP_UNIT_HANDLE         handle,
               /* IN */  HFP_UNIT_FEATURE        feature,
               /* IN */  HFP_UNIT_ACTION_TYPE    action
           );

/**
 *  \brief To Enable Voice Recognition.
 *
 *  \par Description:
 *       This API allows user to enable the Voice recognition feature.
 *
 *  \param [in] hdl
 *              Connection Handle to identify the connection to remote AG.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
#define BT_hfp_unit_enable_voice_recognition(hdl)  \
        BT_hfp_unit_feature_control                \
        (                                          \
            (hdl),                                 \
            HFP_UNIT_FEATURE_BVRA,                 \
            HFP_UNIT_ACTION_ENABLE                 \
        )

/**
 *  \brief To Disable Voice Recognition
 *
 *  \par Description:
 *       This API allows user to disable the Voice recognition feature.
 *
 *  \param [in] hdl
 *              Connection Handle to identify the connection to remote AG.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
#define BT_hfp_unit_disable_voice_recognition(hdl)  \
        BT_hfp_unit_feature_control                 \
        (                                           \
            (hdl),                                  \
            HFP_UNIT_FEATURE_BVRA,                  \
            HFP_UNIT_ACTION_DISABLE                 \
        )

#ifdef HFP_UNIT_1_8
/**
 *  \brief To Enable Enhanced Voice Recongnition.
 *
 *  \par Description:
 *       This API allows user to indicate the HF Unit is ready to accept audio
 *       when the Audio Connection is first established. It can also be used to
 *       terminate the ongoing VR session and prepare for a new session.
 *
 *       This command is used:
 *       1. To inicate the HF Unit is ready to accept audio when the
 *          Audio Connection is First established.
 *       2. This command can also be used to terminate the ongoing VR session
 *          and prepare for a new session.
 *
 *  \param [in] hdl
 *              Connection Handle to identify the connection to remote AG.
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
#define BT_hfp_unit_enable_enh_voice_recognition(hdl)  \
        BT_hfp_unit_feature_control                    \
        (                                              \
            (hdl),                                     \
            HFP_UNIT_FEATURE_BVRA,                     \
            HFP_UNIT_ACTION_ENH_VREC_ENABLE            \
        )
#endif /* HFP_UNIT_1_8 */

/**
 *  \brief HFP Unit Data Mode: To Send String
 *
 *  \par Description:
 *       This API allows the application to send any proprietary or
 *       standard AT-command or messages to AG.
 *
 *  \param [in] hdl
 *              Connection Handle to identify the connection to remote AG.
 *
 *  \param [in] str
 *              Message or AT-command buffer to be sent to AG.
 *
 *  \param [in] str_len
 *              Length of the string.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
#define BT_hfp_unit_send_string(hdl, str, str_len) \
        hfp_unit_service_request                   \
        (                                          \
            (hdl),                                 \
            HFP_UNIT_AT_SEND_STRING,               \
            (str),                                 \
            (str_len)                              \
        )

#ifdef HFP_UNIT_HAVE_RFCOMM_CB_SUPPORT
/**
 *  \brief To register HFP rfcomm callback function.
 *
 *  \par Description:
 *       This API is used to register HFP rfcomm callback function.
 *
 *  \param [in] event_notification_cb
 *              Event notification callback function pointer.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
API_RESULT BT_hfp_unit_register_rfcomm_cb
           (
               /* IN */  HFP_UNIT_EVENT_NOTIFICATION_CB    event_notification_cb
           );
#endif /* HFP_UNIT_HAVE_RFCOMM_CB_SUPPORT */

#ifdef HFP_UNIT_1_5
/** Support for HFP v1.5 Specification */

/**
 *  \brief To Query Subscriber Number.
 *
 *  \par Description:
 *       This API allows user to query AG’s subscriber number.
 *
 *  \param [in] hdl
 *              Connection Handle to identify the connection to remote AG.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
#define BT_hfp_unit_request_subscriber_num(hdl) \
        hfp_unit_service_request_1_5            \
        (                                       \
            (hdl),                              \
            HFP_UNIT_AT_CNUM,                   \
            NULL,                               \
            0U                                  \
        )

/**
 *  \brief To Query Response & Hold Status
 *
 *  \par Description:
 *       This API allows user to query the current status of any incoming call
 *       'Response and Hold' Status
 *
 *  \param [in] hdl
 *              Connection Handle to identify the connection to remote AG.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
#define BT_hfp_unit_query_resp_hold_status(hdl) \
        hfp_unit_service_request_1_5            \
        (                                       \
            (hdl),                              \
            HFP_UNIT_AT_QRAH,                   \
            NULL,                               \
            0U                                  \
        )

/**
 *  \brief HFP Unit v1.5: To Set Network Name Format
 *
 *  \par Description:
 *       This API allows user to set the name format of the network name.
 *
 *  \param [in] hdl
 *              Connection Handle to identify the connection to remote AG.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
#define BT_hfp_unit_set_network_name_format(hdl) \
        hfp_unit_service_request_1_5             \
        (                                        \
            (hdl),                               \
            HFP_UNIT_AT_S_NW_NAME_FORMAT,        \
            NULL,                                \
            0U                                   \
        )

/**
 *  \brief HFP Unit v1.5: To Query Operator Selection
 *
 *  \par Description:
 *       This API allows user to query the network operator’s name.
 *
 *  \param [in] hdl
 *              Connection Handle to identify the connection to remote AG.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
#define BT_hfp_unit_query_operator_selection(hdl) \
        hfp_unit_service_request_1_5              \
        (                                         \
            (hdl),                                \
            HFP_UNIT_AT_COPS_Q,                   \
            NULL,                                 \
            0U                                    \
        )

/**
 *  \brief HFP Unit v1.5: To Query List of Current Calls
 *
 *  \par Description:
 *       This API allows user to query the list of current calls
 *       and their status on AG.
 *
 *  \param [in] hdl
 *              Connection Handle to identify the connection to remote AG.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
#define BT_hfp_unit_query_list_current_calls(hdl) \
        hfp_unit_service_request_1_5              \
        (                                         \
            (hdl),                                \
            HFP_UNIT_AT_CLCC,                     \
            NULL,                                 \
            0U                                    \
        )

/**
 *  \brief HFP Unit v1.5: Send Response & Hold Values
 *
 *  \par Description:
 *       This API allows user to
 *       - Hold an incoming call
 *       - Reject previously held incoming call
 *       - Accept previously held incoming call
 *
 *  \param [in] hdl
 *              Connection Handle to identify the connection to remote AG.
 *
 *  \param [in] ct
 *              Incoming call hold type like Accept, Reject or Hold.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
#define BT_hfp_unit_send_resp_hold_values(hdl, ct)     \
        (((ct) > 2)                                    \
         ? HFP_UNIT_ERR_INVALID_PARAMETERS             \
         : hfp_unit_service_request_1_5                \
           (                                           \
               (hdl),                                  \
               (UCHAR) (HFP_UNIT_AT_BTRH_HOLD + (ct)), \
               NULL,                                   \
               0U                                      \
           ))

/**
 *  \brief HFP Unit v1.5: Send Advanced Call Hold Values
 *
 *  \par Description:
 *       This API allows user to issue call hold options with call ID during multi party call.
 *
 *  \param [in] hdl
 *              Connection Handle to identify the connection to remote AG.
 *
 *  \param [in] val
 *              Type of call hold with call ID.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
#define BT_hfp_unit_send_adv_call_hold(hdl, val) \
        hfp_unit_service_request_1_5             \
        (                                        \
            (hdl),                               \
            HFP_UNIT_AT_ADV_CHLDV,               \
            (val),                              \
            sizeof (HFP_UNIT_ADV_CALL_HOLD)      \
        )

/**
 *  \brief HFP Unit v1.5: Enable CMEE Reporting
 *
 *  \par Description:
 *       This API allows user to enable CMEE Error reporting on AG.
 *
 *  \param [in] hdl
 *              Connection Handle to identify the connection to remote AG.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
#define BT_hfp_unit_enable_cmee_reporting(hdl) \
        hfp_unit_service_request_1_5           \
        (                                      \
            (hdl),                             \
            HFP_UNIT_AT_CMEE_ENABLE,           \
            NULL,                              \
            0U                                 \
        )

/**
 *  \brief HFP Unit v1.5: Disable CMEE Reporting
 *
 *  \par Description:
 *       This API allows user to disable CMEE Error reporting on AG.
 *
 *  \param [in] hdl
 *              Connection Handle to identify the connection to remote AG.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
#define BT_hfp_unit_disable_cmee_reporting(hdl) \
        hfp_unit_service_request_1_5            \
        (                                       \
            (hdl),                              \
            HFP_UNIT_AT_CMEE_DISABLE,           \
            NULL,                               \
            0U                                  \
        )
#endif /* HFP_UNIT_1_5 */

#ifdef HFP_UNIT_1_6

/** Support for HFP v1.6 Specification */

/**
 *  \brief HFP Unit v1.6: Send Individual Indicator Activation (AT+BIA)
 *
 *  \par Description:
 *       This API allows user to activate/deactivate indicators supported by the AG
 *       \ref BT_hfp_unit_get_peer_indicator_list().
 *
 *  \param [in] hdl
 *              Connection Handle to identify the connection to remote AG.
 *
 *  \param [in] iia_list
 *              String indicating activation/deactivation status of indicators.
 *
 *  \param [in] iia_list_len
 *              Number of octets used to represent the IIA list.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
#define BT_hfp_unit_send_iia(hdl, iia_list, iia_list_len) \
        hfp_unit_service_request_1_6              \
        (                                         \
            (hdl),                                \
            HFP_UNIT_AT_BIA,                      \
            (iia_list),                           \
            (iia_list_len)                        \
        )

/**
 *  \brief HFP Unit v1.6: Send Individual Indicator Activation (AT+BIA) - indicator
 *         activation bit mask provided.
 *
 *  \par Description:
 *       This API allows user to activate/deactivate indicators supported by the AG
 *       \ref BT_hfp_unit_get_peer_indicator_list().
 *       The user is expected to send the bitmask showing the status that the indicators are to be set to.
 *
 *  \param [in] handle
 *              Connection Handle to identify the connection to remote AG.
 *
 *  \param [in] iia_bit_mask
 *              Bit mask indicating activation/deactivation status of indicators.
 *
 *  \param [in] iia_bit_mask_len
 *              Number of bits in 'iia_bit_mask'.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
API_RESULT BT_hfp_unit_iia_simple_bitmask
           (
               /* IN */  HFP_UNIT_HANDLE         handle,
               /* IN */  UINT8 *                 iia_bit_mask,
               /* IN */  UINT8                   iia_bit_mask_len
           );

/**
 *  \brief HFP Unit v1.6: Send Individual Indicator Activation (AT+BIA) - new indicator
 *         activation bit mask and current indicator activation bit mask provided.
 *
 *  \par Description:
 *       This API allows user to activate/deactivate indicators supported by the AG
 *       \ref BT_hfp_unit_get_peer_indicator_list().
 *       The user is expected to send the new bitmask  showing the status that the indicators
 *       are to be set to along with the bitmask showing the current status of the indicators.
 *
 *  \param [in] handle
 *              Connection Handle to identify the connection to remote AG.
 *
 *  \param [in] new_iia_bit_mask
 *              Bit mask indicating activation/deactivation status the indicators should be set to.
 *
 *  \param [in] curr_iia_bit_mask
 *              Bit mask indicating the current activation/deactivation status of the indicators.
 *
 *  \param [in] iia_bit_mask_len
 *              Number of bits in 'new_iia_bit_mask'.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
API_RESULT BT_hfp_unit_iia_new_and_curr_bitmask
           (
               /* IN */  HFP_UNIT_HANDLE         handle,
               /* IN */  UINT8 *                 new_iia_bit_mask,
               /* IN */  UINT8 *                 curr_iia_bit_mask,
               /* IN */  UINT8                   iia_bit_mask_len
           );

/**
 *  \brief HFP Unit v1.6: Send Individual Indicator Activation (AT+BIA) - indicator
 *         activation bit mask and required indicators bit mask provided.
 *
 *  \par Description:
 *       This API allows user to activate/deactivate indicators supported by the AG
 *       \ref BT_hfp_unit_get_peer_indicator_list()).
 *       The user is expected to send the bitmask showing the status that the indicators
 *       are to be set to along with the bitmask showing the indicators whose status is to be sent.
 *
 *  \param [in] handle
 *              Connection Handle to identify the connection to remote AG.
 *
 *  \param [in] iia_bit_mask
 *              Bit mask indicating activation/deactivation status of indicators.
 *
 *  \param [in] req_ind_bit_mask
 *              Bit mask showing the indicators whose activation/deactivation status is to be sent.
 *
 *  \param [in] iia_bit_mask_len
 *              Number of bits in 'iia_bit_mask'.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
API_RESULT BT_hfp_unit_iia_actvn_and_req_ind_bitmask
           (
               /* IN */  HFP_UNIT_HANDLE         handle,
               /* IN */  UINT8 *                 iia_bit_mask,
               /* IN */  UINT8 *                 req_ind_bit_mask,
               /* IN */  UINT8                   iia_bit_mask_len
           );

/**
 *  \brief HFP Unit v1.6: Send Available Codecs List (AT+BAC).
 *
 *  \par Description:
 *       This API allows user to inform the AG about what codecs the HF unit supports.
 *
 *  \param [in] hdl
 *              Connection Handle to identify the connection to remote AG.
 *
 *  \param [in] avl_cdc_list
 *              String containing list of available codecs in decimal format.
 *
 *  \param [in] avl_cdc_len
 *              Length of ‘avl_cdc_list’ as number of octets.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
#define BT_hfp_unit_send_avl_cdc_list(hdl, avl_cdc_list, avl_cdc_len) \
        hfp_unit_service_request_1_6              \
        (                                         \
            (hdl),                                \
            HFP_UNIT_AT_BAC,                      \
            (avl_cdc_list),                       \
            (avl_cdc_len)                         \
        )

/**
 *  \brief HFP Unit v1.6: To Send Available Codecs List (AT+BAC) - Numeric
 *
 *  \par Description:
 *       This API allows user to inform the AG about what codecs the HF unit supports.
 *
 *  \param [in] handle
 *              Connection Handle to identify the connection to remote AG.
 *
 *  \param [in] avl_cdc_list
 *              Numeric array of available codecs.
 *
 *  \param [in] avl_cdc_len
 *              Number of codec ids in ‘avl_cdc_list’.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
API_RESULT BT_hfp_unit_avl_cdc_list_num
           (
               /* IN */  HFP_UNIT_HANDLE         handle,
               /* IN */  HFP_UNIT_CODEC_ID *     avl_cdc_list,
               /* IN */  UINT8                   avl_cdc_len
           );

/**
 *  \brief HFP Unit v1.6: Trigger Codec Connection (AT+BCC)
 *
 *  \par Description:
 *       This API allows user to request the AG to start the codec connection procedure.
 *
 *  \param [in] hdl
 *              Connection Handle to identify the connection to remote AG.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
#define BT_hfp_unit_trigger_codec_connection(hdl) \
        hfp_unit_service_request_1_6              \
        (                                         \
            (hdl),                                \
            HFP_UNIT_AT_BCC,                      \
            NULL,                                 \
            0U                                    \
        )

/**
 *  \brief HFP Unit v1.6: Codec Confirmation (AT+BCS)
 *
 *  \par Description:
 *       This API allows user to request the AG to start the codec connection procedure.
 *
 *  \param [in] hdl
 *              Connection Handle to identify the connection to remote AG.
 *
 *  \param [in] codec_id
 *              Codec Id in decimal format.
 *
 *  \param [in] codec_id_len
 *              Number of octets used to represent the codec id in decimal format.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
#define BT_hfp_unit_send_codec_confirmation(hdl, codec_id, codec_id_len) \
        hfp_unit_service_request_1_6              \
        (                                         \
            (hdl),                                \
            HFP_UNIT_AT_BCS,                      \
            (codec_id),                           \
            (codec_id_len)                        \
        )

/**
 *  \brief HFP Unit v1.6: Codec Confirmation (AT+BCS) - Numeric
 *
 *  \par Description:
 *       This API allows user to request the AG to start the codec connection procedure.
 *
 *  \param [in] handle
 *              Connection Handle to identify the connection to remote AG.
 *
 *  \param [in] codec_id
 *              Codec Id in decimal format.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
API_RESULT BT_hfp_unit_codec_confirmation_num
           (
               /* IN */  HFP_UNIT_HANDLE         handle,
               /* IN */  HFP_UNIT_CODEC_ID       codec_id
           );
#endif /* HFP_UNIT_1_6 */

#ifdef HFP_UNIT_1_7
/**
 *  \brief To get remote AG supported HF indicators. The indicators are
 *         received during SLC procedure.
 *
 *  \par Description:
 *       This API allows user to get the HF indicators information supported by remote AG,
 *       these are received during the SLC procedure.
 *       Usually this API should be called soon after the SLC procedure
 *
 *  \param [in] handle
 *              Connection Handle to identify the connection to remote AG.
 *
 *  \param [in] peer_supp_hf_ind_list
 *              List of HF indicators supported by peer.
 *
 *  \param [in] peer_supp_hf_ind_count
 *              Number of HF indicators supported by peer.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
API_RESULT BT_hfp_unit_get_peer_hf_indicators
           (
               /* IN */  HFP_UNIT_HANDLE                 handle,
               /* OUT */ HFP_UNIT_BIND_READ_RESULT     * peer_supp_hf_ind_list,
               /* OUT */ UCHAR                         * peer_supp_hf_ind_count
           );

/**
 *  \brief To update the value of Indicator.
 *
 *  \par Description:
 *       This API allows user to notify AG about change in the HF indicator value.
 *       "AT+BIEV=ind_id, ind_val" (Example: AT+BIEV=1,1)
 *
 *  \param [in] hdl
 *              Connection Handle to identify the connection to remote AG.
 *
 *  \param [in] hf_ind_value
 *              HF indicator id and its value in string format.
 *
 *  \param [in] hf_ind_value_len
 *              Length of HF indicator id & its value.
 *
 *  \return
 *      API_SUCCESS: Successful return from API.
 *      Relevant HF Unit error code describing the reason for failure.
 */
#define BT_hfp_unit_update_hf_ind_value(hdl, hf_ind_value, hf_ind_value_len) \
        hfp_unit_service_request_1_7              \
        (                                         \
            (hdl),                                \
            HFP_UNIT_AT_BIEV,                     \
            (hf_ind_value),                       \
            (hf_ind_value_len)                    \
        )

#endif /* HFP_UNIT_1_7 */

#ifdef __cplusplus
};
#endif
/** \} */
/** \} */
/** \} */
#endif /* _H_BT_HFP_UNIT_API_ */

