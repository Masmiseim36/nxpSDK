
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
 * \defgroup hfp_unit_module HFP Unit (Hands Free Profile - Unit)
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  Hands Free Profile Unit module to the Application and other upper
 *  layers of the stack.
 */
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
 *  HFP Unit Notification Callback Event Types
 */
/* Connection Management Events */
#define HFP_UNIT_CONNECT_CNF                    0x01
#define HFP_UNIT_CONNECT_IND                    0x02
#define HFP_UNIT_DISCONNECT_CNF                 0x03
#define HFP_UNIT_DISCONNECT_IND                 0x04
#define HFP_UNIT_STOP_CNF                       0x05

/* Voice Call Events */
#define HFP_UNIT_INCALL_ALERT                   0x11
#define HFP_UNIT_CLI_DIGITS                     0x12
#define HFP_UNIT_CALL_WAITING_IND               0x13
#define HFP_UNIT_CALLSETUP                      0x14
#define HFP_UNIT_NO_CALL                        0x15
#define HFP_UNIT_CALL_ACTIVE                    0x16
#define HFP_UNIT_SEND_DTMF_CNF                  0x17
#define HFP_UNIT_TWC_CALL_CTRL_CNF              0x18
#define HFP_UNIT_CALLHANGUP_CNF                 0x19
#define HFP_UNIT_OUTCALL_CNF                    0x1A
#define HFP_UNIT_INCALL_ACCEPT_CNF              0x1B
#define HFP_UNIT_CHLD_MPRTY_CAP                 0x1C

/* Miscellaneous Indications */
#define HFP_UNIT_AG_ERROR_IND                   0x21
#define HFP_UNIT_BSIR_IND                       0x22
#define HFP_UNIT_VGM_IND                        0x23
#define HFP_UNIT_VGS_IND                        0x24
#define HFP_UNIT_VOICE_RECOG_IND                0x25
#define HFP_UNIT_VOICETAG_PHNUM_IND             0x26
#define HFP_UNIT_AG_SERVICE_IND                 0x27
#define HFP_UNIT_CIEV_SIGNAL_IND                0x28
#define HFP_UNIT_CIEV_ROAM_IND                  0x29
#define HFP_UNIT_CIEV_BATTCHG_IND               0x2A
#define HFP_UNIT_PEER_IND_STATUS_CNF            0x2B
#define HFP_UNIT_PEER_IND_STATUS_IND            0x2C
#define HFP_UNIT_OK_IND                         0x2D

/* Confirmations for API Requests */
#define HFP_UNIT_VREC_ENABLE_CNF                0x31
#define HFP_UNIT_VREC_DISABLE_CNF               0x32
#define HFP_UNIT_ECNR_CNF                       0x33
#define HFP_UNIT_CLIP_CNF                       0x34
#define HFP_UNIT_CCWA_CNF                       0x35
#define HFP_UNIT_VOICETAG_PHNUM_CNF             0x36
#define HFP_UNIT_SET_VGM_CNF                    0x37
#define HFP_UNIT_SET_VGS_CNF                    0x38

/* HFP-1.5 Events */
#define HFP_UNIT_RECVD_BTRH_IND                 0x41
#define HFP_UNIT_REQ_SUB_NUM_IND                0x42
#define HFP_UNIT_COPS_QUERY_RESULT_IND          0x43
#define HFP_UNIT_CURRENT_CALL_LIST_IND          0x44
#define HFP_UNIT_CALL_HELD_IND                  0x45
#define HFP_UNIT_CMEE_IND                       0x46
#define HFP_UNIT_CMEE_CNF                       0x47
#define HFP_UNIT_REQ_SUB_NUM_CNF                0x48
#define HFP_UNIT_SET_NW_NAME_FORMAT_CNF         0x49
#define HFP_UNIT_ADV_CALL_HOLD_CNF              0x4A
#define HFP_UNIT_COPS_QUERY_CNF                 0x4B
#define HFP_UNIT_CURRENT_CALL_LIST_CNF          0x4C
#define HFP_UNIT_SEND_BTRH_CNF                  0x4D

/* HFP-1.6 Events */
#define HFP_UNIT_BIA_CNF                        0x4E
#define HFP_UNIT_BAC_CNF                        0x4F
#define HFP_UNIT_BCC_CNF                        0x50
#define HFP_UNIT_BCS_CNF                        0x51
#define HFP_UNIT_BCS_IND                        0x52

/* HFP-1.7 Events */
#define HFP_UNIT_BIEV_CNF                       0x53
#define HFP_UNIT_BIND_IND                       0x54

/* HFP-1.8 Events */
#define HFP_UNIT_ENH_VREC_ENABLE_CNF            0x55

/* Data Mode Events */
#define HFP_UNIT_SEND_DATA_CNF                  0xE0
#define HFP_UNIT_RECVD_DATA_IND                 0xE1


/**
 *  List of HFP Commands
 */
/* Service Level Connection Commands */
#define HFP_UNIT_AT_BRSF                        0x00
#define HFP_UNIT_AT_CIND_TEST                   0x01
#define HFP_UNIT_AT_CIND_READ                   0x02
#define HFP_UNIT_AT_CMER_ENABLE                 0x03
#define HFP_UNIT_AT_CHLDQ                       0x04

/* Commands without User Data (except SLC Commands) */
#define HFP_UNIT_AT_CHUP                        0x05  /* Hang up */
#define HFP_UNIT_AT_ATA                         0x06  /* Accept */

/* Multiparty */
#define HFP_UNIT_AT_CHLDV0                      0x07
#define HFP_UNIT_AT_CHLDV1                      0x08
#define HFP_UNIT_AT_CHLDV2                      0x09
#define HFP_UNIT_AT_CHLDV3                      0x0A
#define HFP_UNIT_AT_CHLDV4                      0x0B

/* CLIP */
#define HFP_UNIT_AT_CLIP_ENABLE                 0x0C
#define HFP_UNIT_AT_CLIP_DISABLE                0x0D

/* Call Waiting */
#define HFP_UNIT_AT_CCWAL_ENABLE                0x0E
#define HFP_UNIT_AT_CCWAL_DISABLE               0x0F

/* Voice Recognition */
#define HFP_UNIT_AT_BVRA_ENABLE                 0x10
#define HFP_UNIT_AT_BVRA_DISABLE                0x11

#define HFP_UNIT_AT_NREC_DISABLE                0x12 /* Disable ECNR */
#define HFP_UNIT_AT_BINPV1                      0x13 /* Voice Tag */
#define HFP_UNIT_AT_BLDN                        0x14 /* Redial */

/* Commands with User Data (except SLC Commands) */
#define HFP_UNIT_AT_DD                          0x15 /* Dial */
#define HFP_UNIT_AT_DM                          0x16 /* Memdial */
#define HFP_UNIT_AT_VGS                         0x17 /* Speaker Gain */
#define HFP_UNIT_AT_VGM                         0x18 /* Microphone gain */
#define HFP_UNIT_AT_VTS                         0x19 /* DTMF */

/* HFP-1.5 Commands */
#define HFP_UNIT_AT_CNUM                        0x1A /* Subscriber Number */
#define HFP_UNIT_AT_QRAH                        0x1B /* Query Resp & Hold */
#define HFP_UNIT_AT_S_NW_NAME_FORMAT            0x1C /* Set NW Name Format */
#define HFP_UNIT_AT_COPS_Q                      0x1D /* Operator Selection */
#define HFP_UNIT_AT_CLCC                        0x1E /* List Current Calls */

/* Send Response & Hold */
#define HFP_UNIT_AT_BTRH_HOLD                   0x1F
#define HFP_UNIT_AT_BTRH_ACCEPT                 0x20
#define HFP_UNIT_AT_BTRH_REJECT                 0x21

/* Enhanced Error Reporting */
#define HFP_UNIT_AT_CMEE_ENABLE                 0x22
#define HFP_UNIT_AT_CMEE_DISABLE                0x23

/* Advanced Call Hold */
#define HFP_UNIT_AT_ADV_CHLDV                   0x24

/* HFP-1.6 Commands */
/* Individual Indicator Activation */
#define HFP_UNIT_AT_BIA                         0x25
/* Available Codecs */
#define HFP_UNIT_AT_BAC                         0x26
/* Trigger Codec Connection */
#define HFP_UNIT_AT_BCC                         0x27
/* Codec Confirmation */
#define HFP_UNIT_AT_BCS                         0x28

/* HFP-1.7 Commands */
/* List HF Indicators to AG */
#define HFP_UNIT_AT_BIND_LIST                   0x29
/* Get AG Supported HF Indicators */
#define HFP_UNIT_AT_BIND_TEST                   0x2A
/* Read Status of HF Indicators in AG */
#define HFP_UNIT_AT_BIND_READ                   0x2B
/* Update HF Indicator Value */
#define HFP_UNIT_AT_BIEV                        0x2C

/* HFP-1.8 Commands */
/**
 * This command is used:
 * 1. To inicate the HF Unit is ready to accept audio when the
 *    Audio Connection is First established.
 * 2. This command can also be used to terminate the ongoing VR session
 *    and prepare for a new session.
 */
#define HFP_UNIT_AT_BVRA_ACCEPT_AUDIO           0x2D

/* Data Mode (Send String) Commands */
#define HFP_UNIT_AT_SEND_STRING                 0xE0

/**
 *  Type of HFP Unit Device (HFP_UNIT_DEVICE_TYPE)
 */
#define HFP_UNIT_DEVICE_SPEAKER                 0x00
#define HFP_UNIT_DEVICE_MICROPHONE              0x01

/**
 *  Three Way Call (TWC) Options (HFP_UNIT_TWC_CONTROL_TYPE)
 */
#define HFP_UNIT_TWC_REJECT_NEW_WAITING_CALL    0x00
#define HFP_UNIT_TWC_ACCEPT_NEW_WAITING_CALL    0x01
#define HFP_UNIT_TWC_CALL_HOLD                  0x02
#define HFP_UNIT_TWC_CALL_CONFERENCE            0x03
#define HFP_UNIT_TWC_TERMINATE_SELF_FROM_TWC    0x04

/**
 *  Supported Features (HFP_UNIT_FEATURE)
 */
#define HFP_UNIT_FEATURE_CLIP                   0x00
#define HFP_UNIT_FEATURE_CCWA                   0x01
#define HFP_UNIT_FEATURE_ECNR                   0x02
#define HFP_UNIT_FEATURE_BVRA                   0x03
#define HFP_UNIT_FEATURE_BINP                   0x04

/**
 *  To Enable/Disable AG to send HFP Indications (HFP_UNIT_ACTION_TYPE)
 *  For example, CLIP.
 */
#define HFP_UNIT_ACTION_DISABLE                 0x00
#define HFP_UNIT_ACTION_ENABLE                  0x01

/* HFP 1.8 related */
#define HFP_UNIT_ACTION_ENH_VREC_ENABLE         0x02

/**
 *  Types for Response & Hold Commands (HFP_UNIT_HELD_INCALL_OPTIONS)
 */
#define HFP_UNIT_RANDH_HOLD                     0x00
#define HFP_UNIT_RANDH_ACCEPT                   0x01
#define HFP_UNIT_RANDH_REJECT                   0x02

/**
 * HF Unit BRSF Command Supported Feature Bit-Masks
 */
#define HFP_UNIT_BRSF_CMD_FEATURE_BIT_ECNR             0
#define HFP_UNIT_BRSF_CMD_FEATURE_BIT_TWC              1
#define HFP_UNIT_BRSF_CMD_FEATURE_BIT_CLIP             2
#define HFP_UNIT_BRSF_CMD_FEATURE_BIT_VRA              3
#define HFP_UNIT_BRSF_CMD_FEATURE_BIT_REM_VOL_CTRL     4
#define HFP_UNIT_BRSF_CMD_FEATURE_BIT_ENH_CALL_STAT    5
#define HFP_UNIT_BRSF_CMD_FEATURE_BIT_ENH_CALL_CTRL    6
#define HFP_UNIT_BRSF_CMD_FEATURE_BIT_CODEC_NEGO       7
#define HFP_UNIT_BRSF_CMD_FEATURE_BIT_HF_INDICATORS    8
#define HFP_UNIT_BRSF_CMD_FEATURE_BIT_ESCO_S4_SETTING  9

/* HFP 1.8 related */
#define HFP_UNIT_BRSF_CMD_FEATURE_BIT_ENH_VR_STATUS   10
#define HFP_UNIT_BRSF_CMD_FEATURE_BIT_VR_TEXT         11

/**
 * HF Unit BRSF Response Supported Feature Bit-Masks
 */
#define HFP_UNIT_BRSF_RSP_FEATURE_BIT_TWC              0
#define HFP_UNIT_BRSF_RSP_FEATURE_BIT_ECNR             1
#define HFP_UNIT_BRSF_RSP_FEATURE_BIT_VRA              2
#define HFP_UNIT_BRSF_RSP_FEATURE_BIT_INBAND_RING      3
#define HFP_UNIT_BRSF_RSP_FEATURE_BIT_VOICE_TAG        4
#define HFP_UNIT_BRSF_RSP_FEATURE_BIT_CALL_REJECT      5
#define HFP_UNIT_BRSF_RSP_FEATURE_BIT_ENH_CALL_STAT    6
#define HFP_UNIT_BRSF_RSP_FEATURE_BIT_ENH_CALL_CTRL    7
#define HFP_UNIT_BRSF_RSP_FEATURE_BIT_EXT_ERROR_CODES  8
#define HFP_UNIT_BRSF_RSP_FEATURE_BIT_CODEC_NEGO       9
#define HFP_UNIT_BRSF_RSP_FEATURE_BIT_HF_INDICATORS   10
#define HFP_UNIT_BRSF_RSP_FEATURE_BIT_ESCO_S4_SETTING 11

/* HFP 1.8 related */
#define HFP_UNIT_BRSF_RSP_FEATURE_BIT_ENH_VR_STATUS   12
#define HFP_UNIT_BRSF_RSP_FEATURE_BIT_VR_TEXT         13

/* HFP Codec IDs  */
#define HFP_UNIT_CODEC_ID_CVSD                         0x01
#define HFP_UNIT_CODEC_ID_MSBC                         0x02

/* HF Indicator IDs */
#define HFP_UNIT_HF_IND_ID_ENHANCED_SAFETY             0x0001
#define HFP_UNIT_HF_IND_ID_BATTERY_LEVEL               0x0002

/* ======= HFP 1.8 related defines ======= */
/**
 * Bitmask to reflect the current state of Voice recognition engine in AG.
 */
#define HFP_ENH_VR_STATE_AG_READY_2_ACCEPT_AUDIO_INPUT      0x01
#define HFP_ENH_VR_STATE_AG_SENDING_AUDIO_TO_HF             0x02
#define HFP_ENH_VR_STATE_AG_PROCESSING_AUDIO_INPUT          0x04

/**
 * Voice Recognition Text Type defines
 */
#define HFP_ENH_VR_TEXT_TYPE_TEXT_FROM_HF_AUDIO             0x00
#define HFP_ENH_VR_TEXT_TYPE_TEXT_FROM_AG_AUDIO             0x01
#define HFP_ENH_VR_TEXT_TYPE_TEXT_FROM_AG_AUDIO_WITH_Q      0x02
#define HFP_ENH_VR_TEXT_TYPE_TEXT_FROM_AG_AUDIO_WITH_ERR    0x03

/**
 * Voice Recognition Text Operation defines
 */
#define HFP_ENH_VR_TEXT_OP_NEW_TEXT                         0x01
#define HFP_ENH_VR_TEXT_OP_REPLACE                          0x02
#define HFP_ENH_VR_TEXT_OP_APPEND                           0x03

/* ------------------------------------------- Macros */
#define HFP_UNIT_BRSF_IS_FEATURE_SET(value, feature) \
        ((value) & (1 << (feature)))? (BT_TRUE): (BT_FALSE)
/** \} */
/* ------------------------------------------- Structures/Data Types */
/**
 *  HFP Unit Callback Events
 */
typedef UCHAR HFP_UNIT_EVENTS;

/**
 *  HFP Unit Handle
 */
typedef UINT32 HFP_UNIT_HANDLE;

/**
 *  HFP Unit Device Type
 */
typedef UCHAR HFP_UNIT_DEVICE_TYPE;

/**
 *  HFP Unit Feature Type (CLIP, CWA, VRA etc.)
 */
typedef UCHAR HFP_UNIT_FEATURE;

/**
 *  HFP Unit Action Type
 *  (To Enable/Disable AG to send HFP Indications)
 */
typedef UCHAR HFP_UNIT_ACTION_TYPE;

/**
 *  HFP Unit Command Value to be sent
 */
typedef UCHAR HFP_UNIT_COMMAND;

/**
 *  HFP Unit Two Way Call (TWC) Control Type
 */
typedef UCHAR HFP_UNIT_TWC_CONTROL_TYPE;

/**
 *  HFP Unit Response & Hold Command Options
 */
typedef UCHAR HFP_UNIT_HELD_INCALL_OPTIONS;

/**
 *  HFP Unit Codec ID
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
 *  by the HFP-1.5 specification. More Indicators may be addded in fiture,
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
 *  HFP Unit Event Notification Callback Type
 */
typedef API_RESULT (* HFP_UNIT_EVENT_NOTIFICATION_CB)
                   (
                       /* IN */  HFP_UNIT_HANDLE,    /* Handle */
                       /* IN */  HFP_UNIT_EVENTS,    /* Event Type */
                       /* IN */  API_RESULT,         /* Event Result */
                       /* IN */  void *,             /* Event Data */
                       /* IN */  UINT16              /* Event Data Size */
                   );
/**
 * \defgroup hfp_unit_structures Structures
 * \{
 * Describes Structures defined by the module.
 */
/**
 *  Data Type to represent the parameters received from the Application layer,
 *  required to start the HFU.
 */
typedef struct
{
    /* HFP Unit supported features in decimal format */
    UCHAR *   supp_features;

    /* HFP Unit available codecs in decimal format */
    UCHAR *   available_codecs;

    /* No. of octets used to represent supported features */
    UINT16    supp_features_len;

    /* No. of octets used to represent supported features */
    UINT16    available_codecs_len;

#ifdef HFP_UNIT_1_7
    /* HFP Unit Supported HF Indicator List */
    UCHAR *    hf_indicators;

    /* No. of octets used to represent the HF indicators List */
    UINT16     hf_indicators_len;

#endif /* HFP_UNIT_1_7 */

    /* Local server channel for accepting incoming HFP Unit connection */
    UCHAR     server_channel;

} HFP_UNIT_APPL_CONFIG_PARAMS;

/**
 *  Data Type to represent the parameters of CHLD, received from peer
 *  (HFP Audio Gateway)
 */
typedef struct
{
    /* Number of CHLD Parameters */
    UCHAR hold_count;

    /* Value of CHLD Parameters */
    UCHAR peer_hold_value [HFP_UNIT_MAX_HOLD_TYPES];

} HFP_UNIT_CHLD_RESP_RESULT;

/**
 *  Data Type to represent the parameters of +BIND, received from peer
 *  (HFP Audio Gateway)
 */
#define  HFP_UNIT_BIND_TEST_RESULT       AT_PARSER_BIND_TEST_RESULT
#define  HFP_UNIT_BIND_READ_RESULT       AT_PARSER_BIND_READ_RESULT


/**
 *  Data Type to hold values for Call Control options (0-4) and Call ID
 */
typedef struct
{
    /* Call Control Option */
    HFP_UNIT_TWC_CONTROL_TYPE    option;

    /* Call ID */
    UCHAR                        call_id;

} HFP_UNIT_ADV_CALL_HOLD;


/**
 *  Data Type which is provided to Application, over HFP-Unit, with HFP Unit
 *  Event Notification Callback, for certain Events.
 *
 *  Refer to the HFP Unit API Document to understand what values are
 *  populated for the data type elements, for every event.
 */
typedef struct
{
    /* AT Parser Response */
    AT_PARSER_RESPONSE *    parser_resp;

    /* Buffer containing the AT Response String */
    UCHAR *                 buffer;

    /* Size of buffer containing the AT Response String */
    UINT16                  buffer_len;

} HFP_UNIT_APP_DATA;
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
/*
 *  This are internal functions required to implement functions & macros
 *  exported for application's use. These functions should not be called
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
/** To intialize the HFP Unit */
API_RESULT BT_hfp_unit_init
           (
               /* IN */  HFP_UNIT_EVENT_NOTIFICATION_CB    notify_cb
           );

#ifndef HFP_UNIT_1_6
/** To start HFP Unit, and start services */
API_RESULT BT_hfp_unit_start
           (
               /* IN */  UCHAR      server_channel,
               /* IN */  UCHAR *    supp_features,
               /* IN */  UINT16     supp_features_len
           );
#else /* HFP_UNIT_1_6 */
/** To start HFP Unit, and start services */
API_RESULT BT_hfp_unit_start
           (
               /* IN */  HFP_UNIT_APPL_CONFIG_PARAMS *p_hfp_unit_appl_conf_params
           );
#endif /* HFP_UNIT_1_6 */

/** To stop HFP Unit, and its services */
API_RESULT BT_hfp_unit_stop
           (
               void
           );

/** To establish HFP Unit connection with remote HFP Audio Gateway */
API_RESULT BT_hfp_unit_connect
           (
               /* IN */  UCHAR *    bd_addr,
               /* IN */  UCHAR      server_channel
           );

/** To disconnect HFP Unit connection with a remote HFP Audio Gateway */
API_RESULT BT_hfp_unit_disconnect
           (
               /* IN */  HFP_UNIT_HANDLE    handle
           );

/**
 *  To abort current ongoing service request (except service level
 *  connection), and reset HFP Unit state
 */
API_RESULT BT_hfp_unit_abort
           (
               /* IN */  HFP_UNIT_HANDLE    handle
           );

/** To get peer HFP Audio Gateway's Indicator List */
API_RESULT BT_hfp_unit_get_peer_indicator_list
           (
               /* IN */  HFP_UNIT_HANDLE                handle,
               /* OUT */ HFP_UNIT_CIND_TEST_RESULT *    cind_test_result
           );

/**
 *  To get peer HFP Audio Gateway's Call Hold values of supported
 *  Call Hold options, for a multi-party call
 */
API_RESULT BT_hfp_unit_get_peer_chld_values
           (
               /* IN */  HFP_UNIT_HANDLE                handle,
               /* OUT */ HFP_UNIT_CHLD_RESP_RESULT *    peer_chld_values
           );

/**
 * To get the peer supported features received
 * during the SLC procedure.
 */
API_RESULT BT_hfp_unit_get_peer_supported_features
           (
               /* IN */  HFP_UNIT_HANDLE    handle,
               /* OUT */ UINT32           * peer_supp_features
           );

/** To get peer HFP Audio Gateway's Indicator Status */
#define BT_hfp_unit_get_peer_indicator_status(hdl) \
        hfp_unit_service_request                   \
        (                                          \
            (hdl),                                 \
            HFP_UNIT_AT_CIND_READ,                 \
            NULL,                                  \
            0                                      \
        )

/** HFP Unit Call Management: Dial */
#define BT_hfp_unit_dial(hdl, dg, len) \
        hfp_unit_service_request       \
        (                              \
            (hdl),                     \
            HFP_UNIT_AT_DD,            \
            (dg),                      \
            (len)                      \
        )

/** HFP Unit Call Management: Redial */
#define BT_hfp_unit_redial(hdl)  \
        hfp_unit_service_request \
        (                        \
            (hdl),               \
            HFP_UNIT_AT_BLDN,    \
            NULL,                \
            0                    \
        )

/** HFP Unit Call Management: Memdial */
#define BT_hfp_unit_memdial(hdl, dg, len) \
        hfp_unit_service_request          \
        (                                 \
            (hdl),                        \
            HFP_UNIT_AT_DM,               \
            (dg),                         \
            (len)                         \
        )

/** HFP Unit Call Management: Call Accept */
#define BT_hfp_unit_callaccept(hdl) \
        hfp_unit_service_request    \
        (                           \
            (hdl),                  \
            HFP_UNIT_AT_ATA,        \
            NULL,                   \
            0                       \
        )

/** HFP Unit Call Management: Call Hangup */
#define BT_hfp_unit_callhangup(hdl) \
        hfp_unit_service_request    \
        (                           \
            (hdl),                  \
            HFP_UNIT_AT_CHUP,       \
            NULL,                   \
            0                       \
        )

/** HFP Unit Call Management: Send DTMF */
#define BT_hfp_unit_send_dtmf(hdl, dg, len) \
        hfp_unit_service_request            \
        (                                   \
            (hdl),                          \
            HFP_UNIT_AT_VTS,                \
            (dg),                           \
            (len)                           \
        )

/**
 *  HFP Unit Call Management: Control Multi-party Call
 *  The Call Type (ct) must be 'HFP_UNIT_TWC_CONTROL_TYPE'.
 */
#define BT_hfp_unit_control_multiparty_call(hdl, ct) \
        (((ct) > 4)                                  \
         ? HFP_UNIT_ERR_INVALID_PARAMETERS           \
         : hfp_unit_service_request                  \
           (                                         \
               (hdl),                                \
               (UCHAR) (HFP_UNIT_AT_CHLDV0 + (ct)),  \
               NULL,                                 \
               0                                     \
           ))

/** To set/change Mic/Speaker Gain */
API_RESULT BT_hfp_unit_set_gain
           (
               /* IN */  HFP_UNIT_HANDLE         handle,
               /* IN */  HFP_UNIT_DEVICE_TYPE    type,
               /* IN */  UCHAR *                 gain,
               /* IN */  UCHAR                   gain_length
           );

/**
 *  To enable/disable the following features:
 *  - Caller ID Indication
 *  - ECNR
 *  - Call Waiting
 *  - Voice Tag Phone Number
 *  - Voice Recognition
 */
API_RESULT BT_hfp_unit_feature_control
           (
               /* IN */  HFP_UNIT_HANDLE         handle,
               /* IN */  HFP_UNIT_FEATURE        feature,
               /* IN */  HFP_UNIT_ACTION_TYPE    action
           );

/** HFP Unit: Enable Voice Recognition */
#define BT_hfp_unit_enable_voice_recognition(hdl)  \
        BT_hfp_unit_feature_control                \
        (                                          \
            (hdl),                                 \
            HFP_UNIT_FEATURE_BVRA,                 \
            HFP_UNIT_ACTION_ENABLE                 \
        )

/** HFP Unit: Disable Voice Recognition */
#define BT_hfp_unit_disable_voice_recognition(hdl)  \
        BT_hfp_unit_feature_control                 \
        (                                           \
            (hdl),                                  \
            HFP_UNIT_FEATURE_BVRA,                  \
            HFP_UNIT_ACTION_DISABLE                 \
        )

#ifdef HFP_UNIT_1_8
/***
 * HFP Unit: Enable Enhanced Voice Recongnition.
 *
 * This command is used:
 * 1. To inicate the HF Unit is ready to accept audio when the
 *    Audio Connection is First established.
 * 2. This command can also be used to terminate the ongoing VR session
 *    and prepare for a new session.
 */
#define BT_hfp_unit_enable_enh_voice_recognition(hdl)  \
        BT_hfp_unit_feature_control                    \
        (                                              \
            (hdl),                                     \
            HFP_UNIT_FEATURE_BVRA,                     \
            HFP_UNIT_ACTION_ENH_VREC_ENABLE            \
        )
#endif /* HFP_UNIT_1_8 */

/** HFP Unit Data Mode: Send String */
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
 * To register HFP rfcomm callback function
 */
API_RESULT BT_hfp_unit_register_rfcomm_cb
           (
               /* IN */  HFP_UNIT_EVENT_NOTIFICATION_CB    event_notification_cb
           );
#endif /* HFP_UNIT_HAVE_RFCOMM_CB_SUPPORT */

#ifdef HFP_UNIT_1_5
/* Support for HFP v1.5 Specification */

/* HFP Unit Query Subscriber Number */
#define BT_hfp_unit_request_subscriber_num(hdl) \
        hfp_unit_service_request_1_5            \
        (                                       \
            (hdl),                              \
            HFP_UNIT_AT_CNUM,                   \
            NULL,                               \
            0                                   \
        )

/* HFP Unit v1.5: Query Response & Hold Status */
#define BT_hfp_unit_query_resp_hold_status(hdl) \
        hfp_unit_service_request_1_5            \
        (                                       \
            (hdl),                              \
            HFP_UNIT_AT_QRAH,                   \
            NULL,                               \
            0                                   \
        )

/* HFP Unit v1.5: Set Network Name Format */
#define BT_hfp_unit_set_network_name_format(hdl) \
        hfp_unit_service_request_1_5             \
        (                                        \
            (hdl),                               \
            HFP_UNIT_AT_S_NW_NAME_FORMAT,        \
            NULL,                                \
            0                                    \
        )

/* HFP Unit v1.5: Query Operator Selection */
#define BT_hfp_unit_query_operator_selection(hdl) \
        hfp_unit_service_request_1_5              \
        (                                         \
            (hdl),                                \
            HFP_UNIT_AT_COPS_Q,                   \
            NULL,                                 \
            0                                     \
        )

/* HFP Unit v1.5: Query List of Current Calls */
#define BT_hfp_unit_query_list_current_calls(hdl) \
        hfp_unit_service_request_1_5              \
        (                                         \
            (hdl),                                \
            HFP_UNIT_AT_CLCC,                     \
            NULL,                                 \
            0                                     \
        )

/* HFP Unit v1.5: Send Response & Hold Values */
#define BT_hfp_unit_send_resp_hold_values(hdl, ct)     \
        (((ct) > 2)                                    \
         ? HFP_UNIT_ERR_INVALID_PARAMETERS             \
         : hfp_unit_service_request_1_5                \
           (                                           \
               (hdl),                                  \
               (UCHAR) (HFP_UNIT_AT_BTRH_HOLD + (ct)), \
               NULL,                                   \
               0                                       \
           ))

/* HFP Unit v1.5: Send Advanced Call Hold Values */
#define BT_hfp_unit_send_adv_call_hold(hdl, val) \
        hfp_unit_service_request_1_5             \
        (                                        \
            (hdl),                               \
            HFP_UNIT_AT_ADV_CHLDV,               \
            (val),                              \
            sizeof (HFP_UNIT_ADV_CALL_HOLD)      \
        )

/* HFP Unit v1.5: Enable CMEE Reporting */
#define BT_hfp_unit_enable_cmee_reporting(hdl) \
        hfp_unit_service_request_1_5           \
        (                                      \
            (hdl),                             \
            HFP_UNIT_AT_CMEE_ENABLE,           \
            NULL,                              \
            0                                  \
        )

/* HFP Unit v1.5: Disable CMEE Reporting */
#define BT_hfp_unit_disable_cmee_reporting(hdl) \
        hfp_unit_service_request_1_5            \
        (                                       \
            (hdl),                              \
            HFP_UNIT_AT_CMEE_DISABLE,           \
            NULL,                               \
            0                                   \
        )

#endif /* HFP_UNIT_1_5 */

#ifdef HFP_UNIT_1_6
/* Support for HFP v1.6 Specification */

/* HFP Unit v1.6: Send Individual Indicator Activation (AT+BIA) */
#define BT_hfp_unit_send_iia(hdl, iia_list, iia_list_len) \
        hfp_unit_service_request_1_6              \
        (                                         \
            (hdl),                                \
            HFP_UNIT_AT_BIA,                      \
            (iia_list),                           \
            (iia_list_len)                        \
        )

/* HFP Unit v1.6: Send Individual Indicator Activation (AT+BIA) - indicator
 * activation bit mask provided */
API_RESULT BT_hfp_unit_iia_simple_bitmask
           (
               /* IN */  HFP_UNIT_HANDLE         handle,
               /* IN */  UINT8 *                 iia_bit_mask,
               /* IN */  UINT8                   iia_bit_mask_len
           );

/* HFP Unit v1.6: Send Individual Indicator Activation (AT+BIA) - new indicator
 * activation bit mask and current indicator activation bit mask provided */
API_RESULT BT_hfp_unit_iia_new_and_curr_bitmask
           (
               /* IN */  HFP_UNIT_HANDLE         handle,
               /* IN */  UINT8 *                 new_iia_bit_mask,
               /* IN */  UINT8 *                 curr_iia_bit_mask,
               /* IN */  UINT8                   iia_bit_mask_len
           );

/* HFP Unit v1.6: Send Individual Indicator Activation (AT+BIA) - indicator
 * activation bit mask and required indicators bit mask provided */
API_RESULT BT_hfp_unit_iia_actvn_and_req_ind_bitmask
           (
               /* IN */  HFP_UNIT_HANDLE         handle,
               /* IN */  UINT8 *                 iia_bit_mask,
               /* IN */  UINT8 *                 req_ind_bit_mask,
               /* IN */  UINT8                   iia_bit_mask_len
           );

/* HFP Unit v1.6: Send Available Codecs List (AT+BAC) */
#define BT_hfp_unit_send_avl_cdc_list(hdl, avl_cdc_list, avl_cdc_len) \
        hfp_unit_service_request_1_6              \
        (                                         \
            (hdl),                                \
            HFP_UNIT_AT_BAC,                      \
            (avl_cdc_list),                       \
            (avl_cdc_len)                         \
        )

/* HFP Unit v1.6: Send Available Codecs List (AT+BAC) - Numeric */
API_RESULT BT_hfp_unit_avl_cdc_list_num
           (
               /* IN */  HFP_UNIT_HANDLE         handle,
               /* IN */  HFP_UNIT_CODEC_ID *     avl_cdc_list,
               /* IN */  UINT8                   avl_cdc_len
           );

/* HFP Unit v1.6: Trigger Codec Connection (AT+BCC) */
#define BT_hfp_unit_trigger_codec_connection(hdl) \
        hfp_unit_service_request_1_6              \
        (                                         \
            (hdl),                                \
            HFP_UNIT_AT_BCC,                      \
            NULL,                                 \
            0                                     \
        )

/* HFP Unit v1.6: Codec Confirmation (AT+BCS) */
#define BT_hfp_unit_send_codec_confirmation(hdl, codec_id, codec_id_len) \
        hfp_unit_service_request_1_6              \
        (                                         \
            (hdl),                                \
            HFP_UNIT_AT_BCS,                      \
            (codec_id),                           \
            (codec_id_len)                        \
        )

/* HFP Unit v1.6: Codec Confirmation (AT+BCS) - Numeric */
API_RESULT BT_hfp_unit_codec_confirmation_num
           (
               /* IN */  HFP_UNIT_HANDLE         handle,
               /* IN */  HFP_UNIT_CODEC_ID       codec_id
           );
#endif /* HFP_UNIT_1_6 */

#ifdef HFP_UNIT_1_7
/**
 * To get remote AG supported HF indicators. The indicators are
 * received during SLC procedure.
 */
API_RESULT BT_hfp_unit_get_peer_hf_indicators
           (
               /* IN */  HFP_UNIT_HANDLE                 handle,
               /* OUT */ HFP_UNIT_BIND_READ_RESULT     * peer_supp_hf_ind_list,
               /* OUT */ UCHAR                         * peer_supp_hf_ind_list_count
           );

/* To update the value of Indicator:"AT+BIEV=ind_id, ind_val" (ex: AT+BIEV=1,1) */
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
#endif /* _H_BT_HFP_UNIT_API_ */

