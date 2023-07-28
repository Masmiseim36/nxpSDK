
/**
 *  \file GA_ccp_se_api.h
 *
 *  \brief This file defines the GA Call Control Profile(CCP)
 *  Server Entity(SE) Interfaces - includes Data Structures and Methods.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GA_CCP_SE_API_
#define _H_GA_CCP_SE_API_


/* --------------------------------------------- Header File Inclusion */
#include "GA_common.h"
#include "GA_bearer_api.h"
#include "GA_property_config.h"

/* --------------------------------------------- Global Definitions */
/**
 * \addtogroup bt_ga_profiles
 * \{
 */
/**
 * \addtogroup bt_ga_content_control Content Control
 * \{
 */

/**
 * \addtogroup bt_ga_ccp Call Control
 * \{
 * \brief This section describes the interfaces & APIs offered by the EtherMind
 * Generic Audio (GA) Profile Call Control module to the Application.
 */

/**
 * \addtogroup ga_ccp_defines Defines
 * \{
 * \brief This section describes the defines for the module.
 */

/**
 * \defgroup ga_ccp_se_module_def CCP SE (Call Control Profile) Server Entity
 * \{
 * \brief This section describes the defines for CCP SE.
 */

/**
 * \defgroup ga_ccp_se_events Events
 * \{
 * \brief This section lists the Synchronous/Asynchronous Events notified to
 * Application by the Module.
 */

/**
 * \name CCP Server Events - Control Point Write
 * \{
 * \brief This Event is notified on receving Control Write request from the peer
 * device. The Application is expected to send the response
 * using \ref GA_ccp_se_send_rsp() for the control point write request
 * and then on processing the control point write request,
 * send call control notification and other notifications(if any) depending on
 * control point Opcode.
 */

/**
 * \brief This event is notified when a write request is received for a char
 * from peer device with the following values as parameters in the
 * \ref CCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_BRR_CHR_CONTEXT
 * \param [in] evt_id  \ref CCP_SE_CP_WT_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Write Request received \n
 * \param [in] evt_data  \ref TBS_EVENT_INFO, Expressed as
 *                       - data - Pointer to \ref UCHAR \n
 *                       - datalen - Varies \n
 *                       .
 * \param [in] evt_datalen  sizeof ( \ref TBS_EVENT_INFO ) \n
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref GA_ccp_se_send_rsp()
 */
#define CCP_SE_CP_WT_IND                                0x23U

/** \} */

/**
 * \name CCP Server Events - Read
 * \{
 * \brief This Event is notified on receiving read request from the peer device.
 * Based on the received service instance and char_uuid, Application
 * is expected to form the response data and send response using
 * \ref GA_ccp_se_send_rsp() API.
 */

/**
 * \brief This event is notified when a read request is received for a char from
 * peer device with the following values as parameters in the
 * \ref CCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_BRR_CHR_CONTEXT \n
 * \param [in] evt_id  \ref CCP_SE_RD_IND \n
 * \param [in] evt_status  \ref GA_SUCCESS : Read Request received \n
 * \param [in] evt_data  \ref TBS_EVENT_INFO, Expressed as
 *                       If \ref GA_SUCCESS
 *                         - data - Valid only if evt_datalen is 2,
 *                                  Pointer to UCHAR that represents the
 *                                  offset for the read blob request.\n
 *                         - datalen - \ref sizeof( \ref UINT16) representing
 *                                      offset\n
 *                       Else
 *                         - data - NULL \n
 *                         - datalen - 0 \n
 * \param [in] evt_datalen  If \ref GA_SUCCESS, sizeof( \ref TBS_EVENT_INFO ).
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref GA_ccp_se_send_rsp()
 */
#define CCP_SE_RD_IND                                   0x24U

/** \} */

/**
 * \name CCP Server Events - Control Point Write
 * \{
 * \brief This Event is notified on receving Write request for Bearer Signal
 * Strength Reporting Interval. Based on the received service instance, the
 * Application is expected to form the response and send response using
 * \ref GA_ccp_se_send_rsp() API.
 */

/**
 * \brief This event is notified when a write request is received for a char
 * from peer device with the following values as parameters in the
 * \ref CCP_SE_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] chr  \ref GA_BRR_CHR_CONTEXT
 * \param [in] evt_id  \ref CCP_SE_SET_BRR_SIG_STRTH_RPT_INTVAL_IND
 * \param [in] evt_status  \ref GA_SUCCESS : Write Request received \n
 * \param [in] evt_data  \ref TBS_EVENT_INFO, Expressed as
 *                       - data - Pointer to \ref UCHAR \n
 *                         The Bearer Signal Strength Reporting Interval
 *                         is 1 octet in seconds.
 *                       - datalen - \ref sizeof( \ref UINT8 ) \n
 *                       .
 * \param[in] evt_datalen  sizeof ( \ref TBS_EVENT_INFO ) \n
 *
 * \return \ref GA_SUCCESS (always)
 *
 * \sa \ref GA_ccp_se_send_rsp()
 */
#define CCP_SE_SET_BRR_SIG_STRTH_RPT_INTVAL_IND         0x25U

/** \} */

/** \} */

/**
 * \defgroup ga_ccp_se_constants Constants
 * \{
 * \brief This section lists Constants,
 * As defined in TBS Specification.
 */

/**
 * \name CCP Server Constants - TBS Call States
 * \{
 * \brief This section lists Call State values,
 * As defined in TBS Specification.
 */

/** TBS Call State - Incoming */
#define TBS_CALL_STATE_INCOMING                         0x00U
/** TBS Call State - Dialing */
#define TBS_CALL_STATE_DIALING                          0x01U
/** TBS Call State - Alerting */
#define TBS_CALL_STATE_ALERTING                         0x02U
/** TBS Call State - Active */
#define TBS_CALL_STATE_ACTIVE                           0x03U
/** TBS Call State - Locally Held */
#define TBS_CALL_STATE_L_HELD                           0x04U
/** TBS Call State - Remotely Held */
#define TBS_CALL_STATE_R_HELD                           0x05U
/** TBS Call State - Locally and Remotely Held */
#define TBS_CALL_STATE_LR_HELD                          0x06U
/** TBS Call State - Invalid */
#define TBS_CALL_STATE_INVALID                          0xFFU

/** \} */

/**
 * \name CCP Server Constants - TBS Call Terminate Reason
 * \{
 * \brief This section lists Call Termination Reason values,
 * As defined in TBS Specification.
 */

/** TBS Call Terminate Reason - URI Wrong */
#define TBS_CALL_TER_RSN_URI_WRONG                      0x00U
/** TBS Call Terminate Reason - Call failed */
#define TBS_CALL_TER_RSN_CALL_FAILED                    0x01U
/** TBS Call Terminate Reason - The remote party ended the call */
#define TBS_CALL_TER_RSN_R_ENDED_CALL                   0x02U
/** TBS Call Terminate Reason - Call ended from the server */
#define TBS_CALL_TER_RSN_SRV_ENDED_CALL                 0x03U
/** TBS Call Terminate Reason - Line was busy */
#define TBS_CALL_TER_RSN_LINE_BUSY                      0x04U
/** TBS Call Terminate Reason - Network congestion */
#define TBS_CALL_TER_RSN_NWT_CNGSTN                     0x05U
/** TBS Call Terminate Reason - Client terminated the call */
#define TBS_CALL_TER_RSN_CLI_TER_CALL                   0x06U
/** TBS Call Terminate Reason - No service */
#define TBS_CALL_TER_RSN_CLI_NO_SERVICE                 0x07U
/** TBS Call Terminate Reason - No answer */
#define TBS_CALL_TER_RSN_CLI_NO_ANSWER                  0x08U
/** TBS Call Terminate Reason - Unspecified */
#define TBS_CALL_TER_RSN_UNSPECIFIED                    0x09U

/** \} */

/**
 * \name CCP Server Constants - TBS Control Point Opcodes
 * \{
 * \brief This section lists Control Point Opcodes Values,
 * As defined in TBS Specification.
 */

/** TBS Control Point Opcodes - Accept */
#define TBS_CP_OP_ACCEPT                                0x00U
/** TBS Control Point Opcodes - Terminate */
#define TBS_CP_OP_TERMINATE                             0x01U
/** TBS Control Point Opcodes - Local Hold */
#define TBS_CP_OP_L_HLD                                 0x02U
/** TBS Control Point Opcodes - Local Retrieve */
#define TBS_CP_OP_L_RTRV                                0x03U
/** TBS Control Point Opcodes - Originate */
#define TBS_CP_OP_ORIGINATE                             0x04U
/** TBS Control Point Opcodes - Join */
#define TBS_CP_OP_JOIN                                  0x05U

/** \} */

/**
 * \name CCP Server Constants - TBS Call Flags
 * \{
 * \brief This section lists TBS Call Flags,
 * As defined in TBS Specification.
 * NOTE: These are Bitmask.
 */

/** TBS Call Flags - Outgoing */
#define TBS_CALL_FLAGS_OUTGOING                         0x01U
/** TBS Call Flags - Withheld */
#define TBS_CALL_FLAGS_WITHHELD                         0x02U
/** TBS Call Flags - Withheld by Network */
#define TBS_CALL_FLAGS_WITHHELD_BT_NTW                  0x04U

/** \} */

/**
 * \name CCP Server Constants - TBS Uniform Call Identifier(UCI) Values
 * \{
 * \brief This section lists TBS Uniform Call Identifier(UCI) ID values
 * NOTE: Taken from Assigned numbers.
 */

/** TBS UCI ID - Skype */
#define TBS_UCI_ID_SKYPE                                "skype"
/** TBS UCI ID - Lync */
#define TBS_UCI_ID_LYNC                                 "lync"
/** TBS UCI ID - Jabberr */
#define TBS_UCI_ID_JABR                                 "jabr"
/** TBS UCI ID - Blackberry Voice */
#define TBS_UCI_ID_BBV                                  "bbv"
/** TBS UCI ID - Eyebeam */
#define TBS_UCI_ID_EYEBM                                "eyebm"
/** TBS UCI ID - Any WebRTC enabled browser */
#define TBS_UCI_ID_WBRTC                                "wbrtc"
/** TBS UCI ID - QQ */
#define TBS_UCI_ID_QQ                                   "qq"
/** TBS UCI ID - Spika */
#define TBS_UCI_ID_SPIKA                                "spika"
/** TBS UCI ID - Google Talk */
#define TBS_UCI_ID_GTALK                                "gtalk"
/** TBS UCI ID - Facebook Chat */
#define TBS_UCI_ID_FBCH                                 "fbch"
/** TBS UCI ID - Telegram */
#define TBS_UCI_ID_TGRM                                 "tgrm"
/** TBS UCI ID - Line */
#define TBS_UCI_ID_LNE                                  "lne"
/** TBS UCI ID - WhatsApp */
#define TBS_UCI_ID_WTSAP                                "wtsap"
/** TBS UCI ID - Viber */
#define TBS_UCI_ID_VBR                                  "vbr"
/** TBS UCI ID - MessageMe */
#define TBS_UCI_ID_MME                                  "mme"
/** TBS UCI ID - KakaoTalk */
#define TBS_UCI_ID_KKT                                  "kkt"
/** TBS UCI ID - WeChat */
#define TBS_UCI_ID_WCHT                                 "wcht"
/** TBS UCI ID - Tango */
#define TBS_UCI_ID_TGO                                  "tgo"
/** TBS UCI ID - KIK */
#define TBS_UCI_ID_KIK                                  "kik"
/** TBS UCI ID - Nimbuzz */
#define TBS_UCI_ID_NBUZ                                 "nbuz"
/** TBS UCI ID - Hangouts */
#define TBS_UCI_ID_HGUS                                 "hgus"
/** TBS UCI ID - ChatOn */
#define TBS_UCI_ID_CON                                  "con"
/** TBS UCI ID - Messanger */
#define TBS_UCI_ID_MNGR                                 "mngr"
/** TBS UCI ID - ChatSecure */
#define TBS_UCI_ID_CHSEC                                "chsec"
/** TBS UCI ID - iChat */
#define TBS_UCI_ID_ICHT                                 "icht"
/** TBS UCI ID - Rounds */
#define TBS_UCI_ID_RNDS                                 "rnds"
/** TBS UCI ID - imo.im */
#define TBS_UCI_ID_IMO                                  "imo"
/** TBS UCI ID - Skye Qik */
#define TBS_UCI_ID_QIK                                  "qik"
/** TBS UCI ID - Libon */
#define TBS_UCI_ID_LBN                                  "lbn"
/** TBS UCI ID - Vonage Mobile */
#define TBS_UCI_ID_VONM                                 "vonm"
/** TBS UCI ID - ooVo */
#define TBS_UCI_ID_OV                                   "ov"
/** TBS UCI ID - Unknown */
#define TBS_UCI_ID_UN000                                "un000"

/** \} */

/**
 * \name CCP Server Constants - TBS Uniform Resource Identifier(URI) Values
 * \{
 * \brief This section lists the TBS Uniform Resource Identifier(URI) Values.
 * NOTE: Took from Assigned numbers. Took only the selected URI, as there
 * are too many Id's.
 */

/** TBS URI ID - mailto */
#define TBS_URI_ID_MAILTO                               "mailto"
/** TBS URI ID - mailto */
#define TBS_URI_ID_TEL                                  "tel"
/** TBS URI ID - tel,mailto */
#define TBS_URI_ID_TEL_MAILTO                           "tel,mailto"

/** \} */

/**
 * \name CCP Server Constants - TBS Bearer Technology IDs
 * \{
 * \brief This section lists the TBS Bearer Technology IDs.
 */

/** TBS Bearer Technology ID - 3G */
#define TBS_BRR_TECH_3G                                 0x01U
/** TBS Bearer Technology ID - 4G */
#define TBS_BRR_TECH_4G                                 0x02U
/** TBS Bearer Technology ID - LTE */
#define TBS_BRR_TECH_LTE                                0x03U
/** TBS Bearer Technology ID - WIFI */
#define TBS_BRR_TECH_WIFI                               0x04U
/** TBS Bearer Technology ID - 5G */
#define TBS_BRR_TECH_5G                                 0x05U
/** TBS Bearer Technology ID - GSM */
#define TBS_BRR_TECH_GSM                                0x06U
/** TBS Bearer Technology ID - CDMA */
#define TBS_BRR_TECH_CDMA                               0x07U
/** TBS Bearer Technology ID - 2G */
#define TBS_BRR_TECH_2G                                 0x08U
/** TBS Bearer Technology ID - WCDMA */
#define TBS_BRR_TECH_WCDMA                              0x09U
/** TBS Bearer Technology ID - IP */
#define TBS_BRR_TECH_IP                                 0x0AU

/** \} */

/**
 * \name CCP Server Constants - TBS Signal Strength
 * \{
 * \brief This section lists the TBS Signal Strength,
 * As defined in TBS Specification.
 */

/** TBS Signal Strength - No Service */
#define TBS_BRR_SIG_NO_SERVICE                          0x00U
/** TBS Signal Strength - Max Strength */
#define TBS_BRR_SIG_MAX_STRENGTH                        0x64U
/** TBS Signal Strength - Unavailable */
#define TBS_BRR_SIG_UNAVAILABLE                         0xFFU

/** \} */

/**
 * \name CCP Server Constants - TBS Service Type
 * \{
 * \brief This section lists the TBS Service Type Values.
 */

/** TBS Service Type - GTBS */
#define TBS_SVS_TYPE_GTBS                               0x00U
/** TBS Service Type - TBS */
#define TBS_SVS_TYPE_TBS                                0x01U

/** \} */

/**
 * \name CCP Server Constants - Call Control Point Notification Result Codes
 * \{
 * \brief This section describes the TBS Call Control Point Notification
 * Result Codes,
 * As defined in TBS Specification.
 */

/** TBS Call Control Point Notification Result Code - Success */
#define TBS_CP_NTF_RESULT_SUCCESS                       0x00U
/** TBS Call Control Point Notification Result Code - Opcode Not Supported */
#define TBS_CP_NTF_RESULT_OPCODE_NOT_SUPPORTED          0x01U
/** TBS Call Control Point Notification Result Code - Operation Not Possible */
#define TBS_CP_NTF_RESULT_OP_NOT_POSSIBLE               0x02U
/** TBS Call Control Point Notification Result Code - Invalid Call Index */
#define TBS_CP_NTF_RESULT_INVALID_CALL_INDEX            0x03U
/** TBS Call Control Point Notification Result Code - State Mismatch */
#define TBS_CP_NTF_RESULT_STATE_MISMATCH                0x04U
/** TBS Call Control Point Notification Result Code - Lack Of Resources */
#define TBS_CP_NTF_RESULT_LACK_OF_RESOURCES             0x05U
/** TBS Call Control Point Notification Result Code - Invalid Outgoing URI */
#define TBS_CP_NTF_RESULT_INVALID_OUTGOING_URI          0x06U

/** \} */

/** \} */

/**
 * \defgroup ga_ccp_se_structures Structures
 * \{
 * \brief This section lists the various data structures and typedefs for use
 * by the module.
 */

/** TBS Handle */
typedef UCHAR   TBS_HANDLE;

/** TBS Event Info */
typedef struct _TBS_EVENT_INFO_
{
    /** Service Type */
    UCHAR srvc_type;

    /** Service Handle */
    UCHAR srvc_handle;

    /** Char Info */
    GA_BRR_CHR_CONTEXT *chr;

    /** Event Data */
    void  *data;

    /** Event Data Length */
    UINT16  datalen;
}TBS_EVENT_INFO;

/** TBS Response Info. */
typedef struct _TBS_RSP_INFO
{
    /** Char Info. */
    GA_BRR_CHR_CONTEXT * chr;

    /** Data */
    UCHAR * data;

    /** Data Length */
    UCHAR datalen;
}TBS_RSP_INF;

/** \} */

/** \} */

/** \} */

/**
 * \addtogroup ga_ccp_defines
 * \{
 * \brief This section describes the defines for the module.
 */

/**
 * \addtogroup ga_ccp_common CCP Common
 * \{
 * \brief Describes common macros for the module.
 */

/**
 * \addtogroup ga_ccp_error_code Error Codes
 * \{
 * \brief This section lists the Error Codes that are provided as part of
 * return of functionalities of CCP \ref ga_ccp_ce_module_api,
 * \ref ga_ccp_se_module_api.
 */

/**
 * \name Application Error Codes
 * \{
 * \brief This section lists Application error codes,
 * As defined in TBS Specification.
 */

/**
* A characteristic value has changed while a Read Long Value
* Characteristic sub-procedure is in progress
*/
#define TBS_VAL_CHNGD_DURING_RD_LONG 0x80

/** \} */

/** \} */

/** \} */

/** \} */

/**
 * \defgroup ga_ccp_cb Application Callback
 * \{
 * \brief This section describes the application callback for CCP.
 */

/**
 * \defgroup ga_ccp_se_cb CCP SE (Call Control Profile) Server Entity
 * \{
 * \brief This section describes the application callback for CCP SE.
 */

/**
 * \brief CCP Notification Callback Type
 * \param [in] device      Peer Device Info
 * \param [in] ccp_event   Event type
 * \param [in] ccp_status  Event Status
 * \param [in] ccp_data    Event Data
 * \param [in] ccp_datalen Event Data Length
 */
typedef GA_RESULT (* CCP_SE_NTF_CB)
                  (
                      /* IN */ GA_ENDPOINT * device,
                      /* IN */ UINT8         ccp_event,
                      /* IN */ UINT16        ccp_status,
                      /* IN */ void        * ccp_data,
                      /* IN */ UINT16        ccp_datalen
                  );

/** \} */
/** \} */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CCP_SERVER

/**
 * \defgroup ga_ccp_api_defs API Definitions
 * \{
 * \brief This section describes the EtherMind Call Control Profile APIs.
 */

/**
 * \defgroup ga_ccp_se_module_api CCP SE (Call Control Profile) Server Entity
 * \{
 * \brief This section describes the defines for CCP SE.
 */

/**
 * \name CCP Server APIs - Init
 * \{
 * \brief This section describes the Call Control Profile Init APIs
 * for Server.
 */

/**
 *  \brief To Initialize Call Control Server Module.
 *
 *  \par Description:
 *       This function enables to initiaze the Call Control Server.
 *       This function must be called (only once) before any other CCP SE
 *       APIs are used.
 *
 *  \param [in] cb
 *         Callback to be registered with CCP Server.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *  \ref GA_error.h.
 *
 *  \sa ga_ccp_error_code
 */
GA_RESULT GA_ccp_se_init(/* IN */ CCP_SE_NTF_CB cb);

/**
 *  \brief To register TBS Service with CCP Server.
 *
 *  \par Description:
 *       This function enables to register TBS with CCP server module.
 *
 *  \param [out] handle
 *         Allocated Service Handle.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *  \ref GA_error.h.
 *
 *  \sa ga_ccp_error_code
 */
GA_RESULT GA_ccp_se_register_tbs
          (
              /* OUT */ TBS_HANDLE  *handle
          );

/** \} */

/**
 * \name CCP Server APIs - De-Init
 * \{
 * \brief This section describes the Call Control Profile De-Init APIs
 * for Server.
 */

/**
 *  \brief To shutdown Call Control Server Module.
 *
 *  \par Description:
 *       This function enables to shutdown the Call Control Server.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *  \ref GA_error.h.
 *
 *  \sa ga_ccp_error_code
 */
GA_RESULT GA_ccp_se_shutdown(void);

/**
 *  \brief To unregister TBS Service with CCP Server.
 *
 *  \par Description:
 *       This function enables to unregister TBS with the CCP Server Module.
 *
 *  \param [in] handle
 *         Handle associated with TBS.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *  \ref GA_error.h.
 *
 *  \sa ga_ccp_error_code
 */
GA_RESULT GA_ccp_se_unregister_tbs
          (
              /* IN */ TBS_HANDLE    handle
          );

/** \} */

/**
 * \name CCP Server APIs - Notify
 * \{
 * \brief This section describes the Call Control Profile Notify APIs
 * for Server.
 */

/**
 *  \brief To Notify Bearer Provider Name.
 *
 *  \par Description:
 *       This function enables to Notify the Bearer Provider name with the
 *       peer(s).
 *
  *  \param [in] ga_dev
 *         Peer Device Address Information.
 *
 *  \param [in] svs_inst
 *         Associated GTBS/TBS instance handle.
 *
 *  \param [in] data
 *         Pointer to the buffer containing the Bearer Provider Name.
 *
 *  \param [in] datalen
 *         Bearer Provider Name length.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *  \ref GA_error.h.
 *
 *  \sa ga_ccp_error_code
 */
#define GA_ccp_se_ntf_brr_provider_name(ga_dev, svs_inst, data, datalen)        \
        ccp_tbs_se_char_notify                                                  \
        (                                                                       \
            (ga_dev),                                                           \
            (svs_inst),                                                         \
            (GA_CHAR_TBS_BRR_PROVIDER_NAME),                                       \
            (data),                                                             \
            (datalen)                                                           \
        )

/**
 *  \brief To Notify Bearer Technology.
 *
 *  \par Description:
 *       This function enables to Notify the Bearer Technology with the
 *       peer(s).
 *
  *  \param [in] ga_dev
 *         Peer Device Address Information.
 *
 *  \param [in] svs_inst
 *         Associated GTBS/TBS instance handle.
 *
 *  \param [in] data
 *         Pointer to the buffer containing the Bearer Technology.
 *
 *  \param [in] datalen
 *         Bearer Technology length.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *  \ref GA_error.h.
 *
 *  \ref ga_ccp_se_constants
 *  \sa ga_ccp_error_code
 */
#define GA_ccp_se_ntf_brr_technology(ga_dev, svs_inst, data, datalen)           \
        ccp_tbs_se_char_notify                                                  \
        (                                                                       \
            (ga_dev),                                                           \
            (svs_inst),                                                         \
            (GA_CHAR_TBS_BRR_TECHNOLOGY),                                          \
            (data),                                                             \
            (datalen)                                                           \
        )

/**
 *  \brief To Notify Bearer Supported URI schemes.
 *
 *  \par Description:
 *       This function enables to Notify the Bearer supported URI schemes with the
 *       peer(s).
 *
  *  \param [in] ga_dev
 *         Peer Device Address Information.
 *
 *  \param [in] svs_inst
 *         Associated GTBS/TBS instance handle.
 *
 *  \param [in] data
 *         Pointer to the buffer containing the list of Bearer URI Schemes.
 *
 *  \param [in] datalen
 *         Bearer URI Schemes List length.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *  \ref GA_error.h.
 *
 *  \ref ga_ccp_se_constants
 *  \sa ga_ccp_error_code
 */
#define GA_ccp_se_ntf_brr_uri_schms_supp_list(ga_dev, svs_inst, data, datalen)  \
        ccp_tbs_se_char_notify                                                  \
        (                                                                       \
            (ga_dev),                                                           \
            (svs_inst),                                                         \
            (GA_CHAR_TBS_BRR_URI_SCHMS_SUPPORT_LIST),                              \
            (data),                                                             \
            (datalen)                                                           \
        )

/**
 *  \brief To Notify Bearer Signal Strength.
 *
 *  \par Description:
 *       This function enables to Notify  the Bearer Signal Strength with the
 *       peer(s).
 *
  *  \param [in] ga_dev
 *         Peer Device Address Information.
 *
 *  \param [in] svs_inst
 *         Associated GTBS/TBS instance handle.
 *
 *  \param [in] data
 *         Pointer to the buffer bearer signal strength.
 *
 *  \param [in] datalen
 *         Bearer Signal Strength Length.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *  \ref GA_error.h.
 *
 *  \sa ga_ccp_error_code
 */
#define GA_ccp_se_ntf_brr_sig_strength(ga_dev, svs_inst, data, datalen)         \
        ccp_tbs_se_char_notify                                                  \
        (                                                                       \
            (ga_dev),                                                           \
            (svs_inst),                                                         \
            (GA_CHAR_TBS_BRR_SIGSTRENGTH),                                     \
            (data),                                                             \
            (datalen)                                                           \
        )

/**
 *  \brief To Notify Bearer Current Calls List.
 *
 *  \par Description:
 *       This function enables to Notify the Bearer Current Calls list with the
 *       peer(s).
 *
 *  \param [in] ga_dev
 *         Peer Device Address Information.
 *
 *  \param [in] svs_inst
 *         Associated GTBS/TBS instance handle.
 *
 *  \param [in] data
 *         Pointer to buffer containing the Bearer Current Calls.
 *
 *  \param [in] datalen
 *         Bearer Current Calls Length.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *  \ref GA_error.h.
 *
 *  \sa ga_ccp_error_code
 */
#define GA_ccp_se_ntf_brr_cur_call_list(ga_dev, svs_inst, data, datalen)        \
        ccp_tbs_se_char_notify                                                  \
        (                                                                       \
            (ga_dev),                                                           \
            (svs_inst),                                                         \
            (GA_CHAR_TBS_BRR_LIST_CUR_CALLS),                                      \
            (data),                                                             \
            (datalen)                                                           \
        )

/**
 *  \brief To Notify Bearer Status Flags.
 *
 *  \par Description:
 *       This function enables to Notify the Bearer Status Flags with the
 *       peer(s).
 *       This flag indicate if inband ringtone or Silent mode is
 *       enabled/disabled.
 *
 *  \param [in] ga_dev
 *         Peer Device Address Information.
 *
 *  \param [in] svs_inst
 *         Associated GTBS/TBS instance handle.
 *
 *  \param [in] data
 *         Pointer to buffer containing the Bearer Status Flags.
 *
 *  \param [in] datalen
 *         Bearer Status Flags Length.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *  \ref GA_error.h.
 *
 *  \sa ga_ccp_error_code
 */
#define GA_ccp_se_ntf_brr_status_flags(ga_dev, svs_inst, data, datalen)         \
        ccp_tbs_se_char_notify                                                  \
        (                                                                       \
            (ga_dev),                                                           \
            (svs_inst),                                                         \
            (GA_CHAR_TBS_STATUS_FLAGS),                                            \
            (data),                                                             \
            (datalen)                                                           \
        )

/**
 *  \brief To Notify Incoming Call Target Bearer URI.
 *
 *  \par Description:
 *       This function enables to Notify the Incoming Call Target Bearer URI
 *       with the peer(s).
 *       It contains Call Index followed by the URI of the target of an
 *       incoming call.
 *
  *  \param [in] ga_dev
 *         Peer Device Address Information.
 *
 *  \param [in] svs_inst
 *         Associated GTBS/TBS instance handle.
 *
 *  \param [in] data
 *         Pointer to buffer containing the Incoming Call Target Bearer URI.
 *
 *  \param [in] datalen
 *         Incoming Call Target Bearer URI length.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *  \ref GA_error.h.
 *
 *  \sa ga_ccp_error_code
 */
#define GA_ccp_se_ntf_incoming_call_trgt_brr_uri(ga_dev, svs_inst, data, datalen)   \
        ccp_tbs_se_char_notify                                                      \
        (                                                                           \
            (ga_dev),                                                               \
            (svs_inst),                                                             \
            (GA_CHAR_TBS_INCOMING_CALL_TARGET_BRR_URI),                                  \
            (data),                                                                 \
            (datalen)                                                               \
        )

/**
 *  \brief To Notify Call State Information.
 *
 *  \par Description:
 *       This function enables to Notify Call State with the peer(s).
 *       It contains array of members each 3 octetes.
 *       Each member contains
 *         - Call Index: Index to identify a call.
 *         - Call State:This indicate the current state of the call.
 *         - Call Flags: This indicate if call is incomming/outgoing,
 *                       withheld by the server or network/
 *
  *  \param [in] ga_dev
 *         Peer Device Address Information.
 *
 *  \param [in] svs_inst
 *         Associated GTBS/TBS instance handle.
 *
 *  \param [in] data
 *         Pointer to buffer containing the Call State Information.
 *
 *  \param [in] datalen
 *         Call State Information Length.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *  \ref GA_error.h.
 *
 *  \ref ga_ccp_se_constants
 *  \sa ga_ccp_error_code
 */
#define GA_ccp_se_ntf_call_state(ga_dev, svs_inst, data, datalen)               \
        ccp_tbs_se_char_notify                                                  \
        (                                                                       \
            (ga_dev),                                                           \
            (svs_inst),                                                         \
            (GA_CHAR_TBS_CALL_STATE),                                              \
            (data),                                                             \
            (datalen)                                                           \
        )

/**
 *  \brief To Notify Call Control Point.
 *
 *  \par Description:
 *       This function enables to Notify Call Control Point.
 *       The control point notification is sent from the server to the client
 *       after each control point opcode write.
 *       Format:
 *       - Requested opcode(1 octet): Client Request Opcode.
 *       - Call Index (1 octet): Call Index associate with the Opcode
 *       - Result Code(1 octet): Result of opcode procedure.
 *
 *  \param [in] ga_dev
 *         Peer Device Address Information.
 *
 *  \param [in] svs_inst
 *         Associated GTBS/TBS instance handle.
 *
 *  \param [in] data
 *         Pointer to buffer containing Call Control Point Notification.
 *
 *  \param [in] datalen
 *         Call Control Point Notification length.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *  \ref GA_error.h.
 *
 *  \ref ga_ccp_se_constants
 *  \sa ga_ccp_error_code
 */
#define GA_ccp_se_ntf_call_cp(ga_dev, svs_inst, data, datalen)                  \
        ccp_tbs_se_char_notify                                                  \
        (                                                                       \
            (ga_dev),                                                           \
            (svs_inst),                                                         \
            (GA_CHAR_TBS_CALL_CP),                                                 \
            (data),                                                             \
            (datalen)                                                           \
        )

/**
 *  \brief To Notify Call Termination Reason.
 *
 *  \par Description:
 *       This function enables to Notify reason for the Termination of a call
 *       with the peer(s).
 *       Format:
 *       - Call Index(1 octet): Index of the call to be notified.
 *       - Reason Code(1 octet): Reason for the Termination.
 *
  *  \param [in] ga_dev
 *         Peer Device Address Information.
 *
 *  \param [in] svs_inst
 *         Associated GTBS/TBS instance handle.
 *
 *  \param [in] data
 *         Pointer to buffer containing the Call Termination reason
 *         information.
 *
 *  \param [in] datalen
 *         Call Termination reason information length.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *  \ref GA_error.h.
 *
 *  \ref ga_ccp_se_constants
 *  \sa ga_ccp_error_code
 */
#define GA_ccp_se_ntf_terminate_reason(ga_dev, svs_inst, data, datalen)         \
        ccp_tbs_se_char_notify                                                  \
        (                                                                       \
            (ga_dev),                                                           \
            (svs_inst),                                                         \
            (GA_CHAR_TBS_TERMINATION_REASON),                                      \
            (data),                                                             \
            (datalen)                                                           \
        )

/**
 *  \brief To Notify incoming call.
 *
 *  \par Description:
 *       This function enables to Notify the incoming call name with the
 *       peer(s).
 *       Format:
 *       - Call Index(1 octet): Index of the associated incoming call.
 *       - URI(Variable): URI of the incoming call.
 *
 *  \param [in] ga_dev
 *         Peer Device Address Information.
 *
 *  \param [in] svs_inst
 *         Associated GTBS/TBS instance handle.
 *
 *  \param [in] data
 *         Pointer to buffer containing incoming call information.
 *
 *  \param [in] datalen
 *         Incoming Call information length.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *  \ref GA_error.h.
 *
 *  \sa ga_ccp_error_code
 */
#define GA_ccp_se_ntf_incoming_call(ga_dev, svs_inst, data, datalen)            \
        ccp_tbs_se_char_notify                                                  \
        (                                                                       \
            (ga_dev),                                                           \
            (svs_inst),                                                         \
            (GA_CHAR_TBS_INCOMING_CALL),                                           \
            (data),                                                             \
            (datalen)                                                           \
        )

/**
 *  \brief To Notify Call Friendly Name.
 *
 *  \par Description:
 *       This function enables to notify call index and friendly name
 *       (determined by the server) of the incoming or outgoing call with the
 *       peer(s).
 *       Format:
 *       - Call_Index(1 octet):Index associated with the call.
 *       - Friendly_Name(Variable):Caller Friendly name.
 *
 *  \param [in] ga_dev
 *         Peer Device Address Information.
 *
 *  \param [in] svs_inst
 *         Associated GTBS/TBS instance handle.
 *
 *  \param [in] data
 *         Pointer to buffer containing the caller friendly name information.
 *
 *  \param [in] datalen
 *         Caller Friendly Name information length.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *  \ref GA_error.h.
 *
 *  \sa ga_ccp_error_code
 */
#define GA_ccp_se_ntf_call_friendly_name(ga_dev, svs_inst, data, datalen)       \
        ccp_tbs_se_char_notify                                                  \
        (                                                                       \
            (ga_dev),                                                           \
            (svs_inst),                                                         \
            (GA_CHAR_TBS_CALL_FRIENDLY_NAME),                                        \
            (data),                                                             \
            (datalen)                                                           \
        )

/**
 *  \brief Notify Charateristic Values to connected devices.
 *
 *  \par Description
 *          This routine enables to notifies updated char value to specified
 *          remote device.
 *
 *  \param [in] device
 *          Endpoint Device Address to be notified.
 *
 *  \param [in] srvs_inst
 *              GTBS/TBS instance.
 *
 *  \param [in] char_uuid
 *              characteristic to be notified.
 *
 *  \param [in] char_info
 *              characteristic info.
 *
 *  \param [in] char_info_len
 *              characteristic info.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *  \ref GA_error.h.
 *
 *  \sa ga_ccp_error_code
 */
GA_RESULT ccp_tbs_se_char_notify
          (
              /* IN */ GA_ENDPOINT * device,
              /* IN */ UINT8         srvs_inst,
              /* IN */ UINT16        char_uuid,
              /* IN */ void         *char_info,
              /* IN */ UINT16        char_info_len
          );

/** \} */

/**
 * \name CCP Server APIs - Read/Write
 * \{
 * \brief This section describes the Call Control Profile Read/Write APIs
 * for Server.
 */

/**
 *  \brief To send response to incoming request.
 *
 *  \par Description:
 *       This function enables to send response to incoming request from the
 *       peer.
 *
 *  \param [in] device
 *         Peer Device Address Information.
 *
 *  \param [in] event_type
 *         Event type indicating response for read or write.
 *           - \ref GA_RSP_TYPE_RD
 *           - \ref GA_RSP_TYPE_WT
 *
 *  \param [in] event_status
 *         Response status for the read/write operation.
 *
 *  \param [in] rsp_info
 *         Response information for read/write operatios \ref TBS_RSP_INF.
 *
 *  \param [in] rsp_info_len
 *         Response information size.
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in
 *  \ref GA_error.h.
 *
 *  \sa ga_ccp_error_code
 */
GA_RESULT GA_ccp_se_send_rsp
          (
              /* IN */ GA_ENDPOINT * device,
              /* IN */ UCHAR         event_type,
              /* IN */ UINT16        event_status,
              /* IN */ void         *rsp_info,
              /* IN */ UCHAR         rsp_info_len
          );

/** \} */

/** \} */

/** \} */

#endif /* CCP_SERVER */

#ifdef __cplusplus
};
#endif

/** \} */
/** \} */
/** \} */

#endif /* _H_GA_CCP_SE_API_ */
