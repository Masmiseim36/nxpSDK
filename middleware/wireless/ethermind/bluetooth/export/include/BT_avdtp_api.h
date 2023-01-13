
/**
 *  \file BT_avdtp_api.h
 *
 *  \brief EtherMind AVDTP API Header File.
 *
 *  This Header File contains declaration of AVDTP APIs, various Macros and
 *  Constant Definitions for use in upper layer profiles/applications.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_AVDTP_API_
#define _H_BT_AVDTP_API_

/* ----------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "BT_bottom_half.h"

/* ----------------------------------------- Global Definitions */
/**
 * \addtogroup bt_protocol Protocols
 * \{
 */
/**
 * \defgroup  avdtp_module  AVDTP (Audio/Video Distribution Transport Protocol)
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  Advanced Video Distribution Transport Profile (AVDTP) module to the
 *  Application and other upper layers of the stack.
 */
/**
 * \defgroup  avdtp_defines Defines
 * \{
 * Describes defines for AVDTP module.
 */
/**
 * \defgroup avdtp_constants Constants
 * \{
 * Describes Constants defined by AVDTP module.
 */
/**
 *  @name AVDTP Event Notification Callback Event Types
 *  Constant Definitioins for AVDTP Event Notification Callback Event Types.
 */

/*@{*/

#define AVDTP_CONNECT_IND                               0x01U
#define AVDTP_CONNECT_CNF                               0x02U
#define AVDTP_DISCONNECT_IND                            0x03U
#define AVDTP_DISCONNECT_CNF                            0x04U
#define AVDTP_DISCOVER_IND                              0x05U
#define AVDTP_DISCOVER_CNF                              0x06U
#define AVDTP_GET_CAPABILITIES_IND                      0x07U
#define AVDTP_GET_CAPABILITIES_CNF                      0x08U
#define AVDTP_SET_CONFIGURATION_IND                     0x09U
#define AVDTP_SET_CONFIGURATION_CNF                     0x0AU
#define AVDTP_GET_CONFIGURATION_IND                     0x0BU
#define AVDTP_GET_CONFIGURATION_CNF                     0x0CU
#define AVDTP_OPEN_IND                                  0x0DU
#define AVDTP_OPEN_CNF                                  0x0EU
#define AVDTP_CLOSE_IND                                 0x0FU
#define AVDTP_CLOSE_CNF                                 0x10U
#define AVDTP_START_IND                                 0x11U
#define AVDTP_START_CNF                                 0x12U
#define AVDTP_SUSPEND_IND                               0x13U
#define AVDTP_SUSPEND_CNF                               0x14U
#define AVDTP_RECONFIGURE_IND                           0x15U
#define AVDTP_RECONFIGURE_CNF                           0x16U
#define AVDTP_SECURITY_CONTROL_IND                      0x17U
#define AVDTP_SECURITY_CONTROL_CNF                      0x18U
#define AVDTP_ABORT_IND                                 0x19U
#define AVDTP_ABORT_CNF                                 0x1AU

/* AVDTP 1.3 Feature */
#define AVDTP_GET_ALL_CAPABILITIES_IND                  0x1BU
#define AVDTP_GET_ALL_CAPABILITIES_CNF                  0x1CU
#define AVDTP_DELAY_REPORT_IND                          0x1DU
#define AVDTP_DELAY_REPORT_CNF                          0x1EU

#define AVDTP_MEDIA_DATA_IND                            0xF1U
#define AVDTP_RESET_IND                                 0xF2U

/*@}*/


/**
 *  @name AVDTP Media Types
 *  Constant Definitions for AVDTP Media Types - Audio, Video or Multimedia.
 */
/*@{*/

#define AVDTP_MEDIA_AUDIO                               0x00U
#define AVDTP_MEDIA_VIDEO                               0x01U
#define AVDTP_MEDIA_MULTIMEDIA                          0x02U

/*@}*/


/**
 *  @name AVDTP Stream End Point Types
 *  Constant Definitions for AVDTP Stream End Point Types - Source/Sink.
 */
/*@{*/

#define AVDTP_SEP_SOURCE                                0x00U
#define AVDTP_SEP_SINK                                  0x01U

/*@}*/


/**
 *  @name AVDTP Media Codec Types
 *  Constant Definitions for AVDTP Media Codec Types - SBC, MPEG, ATRAC etc.
 */
/*@{*/

#define AVDTP_CODEC_AUDIO_SBC                           0x00U
#define AVDTP_CODEC_AUDIO_MPEG_1_2                      0x01U
#define AVDTP_CODEC_AUDIO_MPEG_2_4                      0x02U
#define AVDTP_CODEC_AUDIO_ATRAC                         0x04U
#define AVDTP_CODEC_AUDIO_VENDOR_SPECIFIC               0xFFU

/*@}*/


/**
 *  @name AVDTP Content Protection Types
 *  Constant Definitions for AVDTP Content Protection Types - DTCP, SCMS-T etc.
 */
/*@{*/

#define AVDTP_INVALID_CP_TYPE                           0x0000U
#define AVDTP_CONTENT_PROTECTION_TYPE_DTCP              0x0001U
#define AVDTP_CONTENT_PROTECTION_TYPE_SCMS_T            0x0002U

/*@}*/

/**
 *  @name AVDTP Content Protection SCMS Bit Definitions
 *  Constant Definitions for SCMS-T Bit Definitions
 */
/*@{*/

#define AVDTP_SCMS_T_COPY_UNLIMITED                     0x00U
#define AVDTP_SCMS_T_COPY_NOT_USED                      0x01U
#define AVDTP_SCMS_T_COPY_ONCE_ONLY                     0x02U
#define AVDTP_SCMS_T_COPY_NOT_ALLOWED                   0x03U

/*@}*/

/**
 *  @name AVDTP Recovery Types
 *  Constant Definitions for AVDTP Recovery Types - RFC2733 (only valid value)
 */
/*@{*/

#define AVDTP_INVALID_RECOVERY_TYPE                     0x00U
#define AVDTP_RECOVERY_TYPE_RFC2733                     0x01U

#define AVDTP_MIN_MRWS                                  0x01U
#define AVDTP_MAX_MRWS                                  0x18U

#define AVDTP_MIN_MNMP                                  0x01U
#define AVDTP_MAX_MNMP                                  0x18U

/*@}*/

/* ----------------------------------------- Internal Macros */

/*@{*/
/**
 * \cond ignore_this
 * \{
 */
/* Bitmap masks for Service Catagories */
#define AVDTP_MASK_SERVICE_MEDIA                        0x01U
#define AVDTP_MASK_SERVICE_REPORTING                    0x02U
#define AVDTP_MASK_SERVICE_RECOVERY                     0x04U
#define AVDTP_MASK_SERVICE_MULTIPLEXING                 0x08U
#define AVDTP_MASK_SERVICE_FRAGMENTATION                0x10U
#define AVDTP_MASK_SERVICE_ROHC                         0x20U
#define AVDTP_MASK_SERVICE_DELAY_REPORTING              0x40U

/** \endcond */
/*@}*/
/** \} */
/* ----------------------------------------- Structures/Data Types */
/**
 * \defgroup avdtp_structures Structures
 * \{
 * Describes Structures defined by the module.
 */
/**
 *  \struct AVDTP_HANDLE
 *  \brief  AVDTP Handle.
 *
 *  This data type represents the abstract handle to refer while
 *  initiating a procedure with AVDTP. Except utility APIs, all AVDTP
 *  APIs require an AVDTP Handle to be provided.
 *
 *  \note
 *  \li    Not all elements of AVDTP handle is required to be populated
 *         correctly everytime an AVDTP API (expecting an AVDTP Handle)
 *         is called. Requirement of AVDTP Handle for each API is described
 *         in the respective API documentation.
 *  \li    The elements of AVDTP Hanle should not be referenced directly,
 *         instead appropriate macros should be used.
 *
 *  \sa
 *  AVDTP_EVENT_NTF_CB
 */
typedef struct _AVDTP_HANDLE
{
    /**
     *  The Event Notification Callback which the AVDTP should
     *  invoke to signal completion of the procedure.
     *
     *  \note
     *  Applications are required to pass a AVDTP_EVENT_NTF_CB varible
     *  typecasted to "void *".
     */
     API_RESULT (* ntf_cb)
                (
                    UCHAR                    event_type,    /* Event Type */
                    UINT16                   event_result,  /* Result/Response */
                    struct _AVDTP_HANDLE *   avdtp_handle,  /* AVDTP Handle */
                    void  *                  event_data,    /* Returned Data */
                    UINT16                   event_datalen  /* Returned Data Length */
                );

    /** The Local Stream End Point Identifier */
    UCHAR local_seid;

    /** The Remote Stream End Point Identifier */
    UCHAR remote_seid;

    /** The Remote Bluetooth Device Address */
    UCHAR bd_addr [BT_BD_ADDR_SIZE];

} AVDTP_HANDLE;


#ifdef AVDTP_ASSISTIVE_MODE
typedef struct avdtp_media_channel_params
{
    /** Local L2CAP CID (LCID) for the streaming channel */
    UINT16 lcid;

    /** Remote L2CAP CID (RCID) for the streaming channel */
    UINT16 rcid;

    /** L2CAP MTU configured */
    UINT16 mtu;

} AVDTP_MEDIA_CHANNEL_PARAMS;
#endif /* AVDTP_ASSISTIVE_MODE */

#ifdef HAVE_AVDTP_TEST_MODE
typedef struct avdtp_sig_msg_reject_rsp_params
{
    /** Flag to enable reject response for Singalling message */
    UCHAR   flag;

    /** AVDTP signalling ID to reject */
    UCHAR   sig_id;

    /** Error code for the AVDTP SIG msg */
    UCHAR   error_code;

} AVDTP_SIG_MSG_REJECT_RSP_PARAMS;
#endif /* HAVE_AVDTP_TEST_MODE */
/** \} */
/** \} */
/**
 * \defgroup avdtp_appl_callbacks Application Callbacks
 * \{
 */
/**
 *  \typedef AVDTP_EVENT_NTF_CB
 *  \brief   AVDTP Event Notification Callback
 *
 *  This data type represents the AVDTP Event Notification Callback, which
 *  AVDTP uses for the following purposes:
 *  \li    To confirm result of an operation initiated by the upper
 *         layer application/profile.
 *  \li    To inform upper layer application/profile of the asynchronous
 *         events - for example, disconnection of the streaming connection,
 *         or, reception of media data.
 *
 *  \param [in] event_type
 *         This parameter defines the AVDTP event being reported. The valid
 *         values for this parameter are defined in "AVDTP Event Notification
 *         Callback Event Types" section.
 *  \param [in] event_result
 *         If the event being reported is a result of an earlier trigger (an
 *         AVDTP API) by the application, this parameter holds the result of
 *         the procedure. On successful completion of the procedure this
 *         parameter will have a value of 0x0000 (API_SUCCESS), else it will
 *         contain an Error Code describing the reason for failure.
 *         For
 *         asynchronous event indications, this parameter holds the possible
 *         reason due to which the event is being reported.
 *  \param [in] avdtp_handle
 *         This parameter is a pointer to AVDTP_HANDLE describing the Stream
 *         Context or SEP responsible for this event.
 *  \param [in] event_data
 *         Depending upon the event_type, this parameters holds any data
 *         being given to the AVDTP application. Description of any such
 *         data will be provided whenever appropriate in the description
 *         of AVDTP APIs that follows.
 *  \param [in] event_datalen
 *         This parameter specifies the size of event_data above.
 *
 *  \return API_RESULT: API_SUCCESS or an AVDTP/A2DP Error Code describing
 *                      the cause of failure.
 *
 *  \par Asynchronous Event Notification
 *
 *  \sa
 *  AVDTP_HANDLE, AVDTP Event Notification Callback Event Types
 */
typedef API_RESULT (* AVDTP_EVENT_NTF_CB)
                   (
                       UCHAR           event_type,
                       UINT16          event_result,
                       AVDTP_HANDLE  * avdtp_handle,
                       void          * event_data,
                       UINT16          event_datalen
                   );
/** \} */
/**
 * \cond ignore_this
 * \{
 */

/**
 *  \page page1 AVDTP Asynchronous Event Notification
 *  This section describes the use of AVDTP_EVENT_NTF_CB for asynchronous
 *  event notifications, that is, when called with the following event
 *  types:
 *      \li \ref Section_1 (AVDTP_SET_CONFIGURATION_IND)
 *      \li \ref Section_2 (AVDTP_OPEN_IND)
 *      \li \ref Section_3 (AVDTP_CLOSE_IND)
 *      \li \ref Section_4 (AVDTP_START_IND)
 *      \li \ref Section_5 (AVDTP_SUSPEND_IND)
 *      \li \ref Section_6 (AVDTP_RECONFIGURE_IND)
 *      \li \ref Section_7 (AVDTP_ABORT_IND)
 *      \li \ref Section_8 (AVDTP_MEDIA_DATA_IND)
 *
 *  The sections below describe how AVDTP Event Notfication is called for
 *  asynchronous events and also gives a guidelines on how they should be
 *  handled.
 *
 *
 *
 *  \section Section_1 AVDTP Set Configuration Indication
 *  The AVDTP will call the Event Notification Callback for this event
 *  whenever it receives the AVDTP Set Configuration command from its peer
 *  for the local SEP in IDLE state.
 *
 *  \param [in] event_type
 *         This parameter will be set to AVDTP_SET_CONFIGURATION_IND.
 *  \param [in] event_result
 *         This parameter has no meaning for this event, and will be set to
 *         0x0000.
 *  \param [in] avdtp_handle
 *         This parameter is a pointer to AVDTP_HANDLE describing the Stream
 *         Context or SEP responsible for this event.
 *  \param [in] event_data
 *         This parameter will be a pointer to a UCHAR buffer containing
 *         the AVDTP packet segment which contains the transport and
 *         application service capabilities information received with the
 *         AVDTP Set configuration Command.
 *  \param [in] event_datalen
 *         This parameter specifies the size of event_data above.
 *
 *  \remark
 *  The upper layer application handling this callback can call
 *  BT_avdtp_decode_sep_configuration API to decode the AVDTP provided
 *  buffer.
 *
 *  \par
 *  The upper layer application is expected to verify the configuration
 *  setting offered by the peer for Application Service Capabilities (the
 *  Codec and Content Protection Capabilities).
 *
 *  \return API_RESULT: API_SUCCESS, or an Error Code describing the reason
 *                      for failure.
 *
 *  \sa
 *  BT_avdtp_decode_sep_configuration
 *
 *
 *
 *  \section Section_2 AVDTP Open Indication
 *  The AVDTP will call the Event Notification Callback for this event
 *  whenever it receives the AVDTP Open command from its peer
 *  for the local SEP in CONFIGURED state.
 *
 *  \param [in] event_type
 *         This parameter will be set to AVDTP_OPEN_IND.
 *  \param [in] event_result
 *         This parameter has no meaning for this event, and will be set to
 *         0x0000.
 *  \param [in] avdtp_handle
 *         This parameter is a pointer to AVDTP_HANDLE describing the Stream
 *         Context or SEP responsible for this event.
 *  \param [in] event_data
 *         This parameter is not used for this event, and will be set to NULL.
 *  \param [in] event_datalen
 *         This parameter is not used for this event, and will be set to Zero.
 *
 *  \return API_RESULT: API_SUCCESS, or an Error Code describing the reason
 *                      for failure.
 *
 *
 *
 *  \section Section_3 AVDTP Close Indication
 *  The AVDTP will call the Event Notification Callback for this event
 *  whenever it receives the AVDTP Close command from its peer
 *  for the local SEP in OPEN/STREAMING state.
 *
 *  \param [in] event_type
 *         This parameter will be set to AVDTP_CLOSE_IND.
 *  \param [in] event_result
 *         This parameter has no meaning for this event, and will be set to
 *         0x0000.
 *  \param [in] avdtp_handle
 *         This parameter is a pointer to AVDTP_HANDLE describing the Stream
 *         Context or SEP responsible for this event.
 *  \param [in] event_data
 *         This parameter is not used for this event, and will be set to NULL.
 *  \param [in] event_datalen
 *         This parameter is not used for this event, and will be set to Zero.
 *
 *  \return API_RESULT: API_SUCCESS, or an Error Code describing the reason
 *                      for failure.
 *
 *
 *
 *  \section Section_4 AVDTP Start Indication
 *  The AVDTP will call the Event Notification Callback for this event
 *  whenever it receives the AVDTP Start command from its peer
 *  for the local SEP in OPEN state.
 *
 *  \param [in] event_type
 *         This parameter will be set to AVDTP_START_IND.
 *  \param [in] event_result
 *         This parameter has no meaning for this event, and will be set to
 *         0x0000.
 *  \param [in] avdtp_handle
 *         This parameter is a pointer to AVDTP_HANDLE describing the Stream
 *         Context or SEP responsible for this event.
 *  \param [in] event_data
 *         This parameter will be a pointer to a UCHAR buffer containing
 *         the value of OutMTU of Media Transport channel.
 *  \param [in] event_datalen
 *         This parameter specifies the size of event_data above.
 *
 *  \return API_RESULT: API_SUCCESS, or an Error Code describing the reason
 *                      for failure.
 *
 *
 *
 *  \section Section_5 AVDTP Suspend Indication
 *  The AVDTP will call the Event Notification Callback for this event
 *  whenever it receives the AVDTP Suspend command from its peer
 *  for the local SEP in STREAMING state.
 *
 *  \param [in] event_type
 *         This parameter will be set to AVDTP_SUSPEND_IND.
 *  \param [in] event_result
 *         This parameter has no meaning for this event, and will be set to
 *         0x0000.
 *  \param [in] avdtp_handle
 *         This parameter is a pointer to AVDTP_HANDLE describing the Stream
 *         Context or SEP responsible for this event.
 *  \param [in] event_data
 *         This parameter is not used for this event, and will be set to NULL.
 *  \param [in] event_datalen
 *         This parameter is not used for this event, and will be set to Zero.
 *
 *  \return API_RESULT: API_SUCCESS, or an Error Code describing the reason
 *                      for failure.
 *
 *
 *
 *  \section Section_6 AVDTP Reconfiguration Indication
 *  The AVDTP will call the Event Notification Callback for this event
 *  whenever it receives the AVDTP Reconfigure command from its peer
 *  for the local SEP in OPEN state.
 *
 *  \param [in] event_type
 *         This parameter will be set to AVDTP_RECONFIGURE_IND.
 *  \param [in] event_result
 *         This parameter has no meaning for this event, and will be set to
 *         0x0000.
 *  \param [in] avdtp_handle
 *         This parameter is a pointer to AVDTP_HANDLE describing the Stream
 *         Context or SEP responsible for this event.
 *  \param [in] event_data
 *         This parameter will be a pointer to a UCHAR buffer containing
 *         the AVDTP packet segment which contains the Application service
 *         capabilities information received with the AVDTP Set configuration
 *         Command.
 *  \param [in] event_datalen
 *         This parameter specifies the size of event_data above.
 *
 *  \remark
 *  The upper layer application handling this callback can call
 *  BT_avdtp_decode_sep_configuration API to decode the AVDTP provided
 *  buffer.
 *
 *  \par
 *  The upper layer application is expected to verify the configuration
 *  setting offered by the peer for Application Service Capabilities (the
 *  Codec and Content Protection Capabilities).
 *
 *  \return API_RESULT: API_SUCCESS, or an Error Code describing the reason
 *                      for failure.
 *
 *  \sa
 *  BT_avdtp_decode_sep_configuration
 *
 *
 *
 *  \section Section_7 AVDTP Abort Indication
 *  The AVDTP will call the Event Notification Callback for this event
 *  whenever it receives the AVDTP Abort command from its peer
 *  for the local SEP in any state, except IDLE.
 *
 *  \param [in] event_type
 *         This parameter will be set to AVDTP_ABORT_IND.
 *  \param [in] event_result
 *         This parameter has no meaning for this event, and will be set to
 *         0x0000.
 *  \param [in] avdtp_handle
 *         This parameter is a pointer to AVDTP_HANDLE describing the Stream
 *         Context or SEP responsible for this event.
 *  \param [in] event_data
 *         This parameter is not used for this event, and will be set to NULL.
 *  \param [in] event_datalen
 *         This parameter is not used for this event, and will be set to Zero.
 *
 *  \return API_RESULT: API_SUCCESS, or an Error Code describing the reason
 *                      for failure.
 *
 *
 *
 *  \section Section_8 AVDTP Media Data Indication
 *  The AVDTP will call the Event Notification Callback for this event
 *  whenever it receives media data for a SEP in STREAMING state.
 *
 *  \param [in] event_type
 *         This parameter will be set to AVDTP_MEDIA_DATA_IND.
 *  \param [in] event_result
 *         This parameter has no meaning for this event, and will be set to
 *         0x0000.
 *  \param [in] avdtp_handle
 *         This parameter is a pointer to AVDTP_HANDLE describing the Stream
 *         Context or SEP responsible for this event.
 *  \param [in] event_data
 *         This parameter will be a pointer to a UCHAR buffer containing the
 *         media packet.
 *  \param [in] event_datalen
 *         This parameter contains the size of event_data above.
 *
 *  \return API_RESULT: API_SUCCESS, or an Error Code describing the reason
 *                      for failure.
 */

/** \endcond */
/**
 * \addtogroup avdtp_defines Defines
 * \{
 */
/**
 * \addtogroup avdtp_structures Structures
 * \{
 */
/**
 *  \struct AVDTP_CODEC_CAP BT_avdtp_api.h
 *  \brief  AVDTP Codec Capabilities.
 *
 *  This data type represents the Codec specific capability information,
 *  which includes the type/kind of Codec and an information element which
 *  describes its supported configurations.
 */
typedef struct
{
    /** Buffer containing Codec Capabilities Informtaion Element */
    UCHAR * codec_ie;

    /** Length of the Codec Capabilities information Element */
    UCHAR codec_ie_len;

    /**  Media Type - Audio/Video/Multimedia */
    UCHAR media_type;

    /** Codec Type - SBC, MPEG, ATRAC etc. */
    UCHAR codec_type;

} AVDTP_CODEC_CAP;

/**
 *  \struct AVDTP_CP_CAP BT_avdtp_api.h
 *  \brief  AVDTP Content Protection Capabilities.
 *
 *  This data type represents the Content Protection specific Capability
 *  information, which includes the type/kind of content protection and
 *  information elements which describe supported configurations.
 */
typedef struct
{
    /** Content Protection Informtaion Element */
    UCHAR * cp_ie;

    /** Length of the Content Protection Informtaion Element */
    UINT16 cp_ie_len;

    /** Content Protection Type */
    UINT16 cp_type;

} AVDTP_CP_CAP;


/**
 *  \struct AVDTP_SEP_INFO BT_avdtp_api.h
 *  \brief  AVDTP Stream End Point Discovery information
 *
 *  This data type represents the information obtained for a remote
 *  AVDTP Stream End Point after a AVDTP Discovery Procedure.
 */
typedef struct
{
    /** Stream End Point Identifier (SEID) of the remote SEP */
    UCHAR acp_seid;

    /** Boolean Flag to indicate whether the remote SEP is in use or not */
    UCHAR in_use;

    /** Type of the remote SEP - Source/Sink */
    UCHAR sep_type;

    /** Media Type supported by the remote SEP - Audio/Video/Multimedia */
    UCHAR media_type;

} AVDTP_SEP_INFO;

/**
 *  \struct AVDTP_RECOVERY_CAP BT_avdtp_api.h
 *  \brief  AVDTP Recovery Capabilities.
 *
 *  This data type represents the Recovery Service specific Transport Capability
 *  information, which includes the type of Recovery Service and
 *  information elements which describe supported configurations.
 */
typedef struct
{
    /**
     * This information element indicates the recovery type used.
     *
     * Values: 0x00 -> Forbidden, 0x01 -> RFC2733, Others -> RFD
     */
    UCHAR  recovery_type;

    /**
     *  Maximum Recovery Window Size for a Transport Packet.
     *  Valid Range: 0x01 to 0x18
     */
    UCHAR mrws;

    /**
     *  The maximum number of media packets a specific parity code covers.
     *  Valid Range: 0x01 to 0x18
     */
    UCHAR mnmp;

} AVDTP_RECOVERY_CAP;

/**
 *  \struct AVDTP_SEP_CAP BT_avdtp_api.h
 *  \brief  AVDTP Stream End Point Capability information
 *
 *  This data type represents the Application Service Capabilities of an
 *  AVDTP Stream End Point (SEP). The Application Service Capabilities
 *  include Codec Capabilities and Content Protection Capabilities.
 *
 *  \note
 *  Upper layer applications/profiles should use this data type to inform
 *  AVDTP of their capabilities during SEP registration.
 *
 *  \sa
 *  BT_avdtp_sep_register
 */
typedef struct
{
    /** Codec Capability information */
    AVDTP_CODEC_CAP codec_cap;

#ifdef AVDTP_HAVE_CONTENT_PROTECTION
    /** Content Protection Capability information */
    AVDTP_CP_CAP cp_cap [AVDTP_MAX_CP_IE];
#endif /* AVDTP_HAVE_CONTENT_PROTECTION */

#ifdef AVDTP_HAVE_RECOVERY_SERVICE
    /** Recovery Capability information */
    AVDTP_RECOVERY_CAP recovery_cap;
#endif /* AVDTP_HAVE_RECOVERY_SERVICE */

#ifdef AVDTP_HAVE_DELAY_REPORTING
    /** Flag for delay report support */
    UCHAR dr_cap_flag;
#endif /* AVDTP_HAVE_DELAY_REPORTING */
#ifdef AVDTP_HAVE_REPORTING_SERVICE
    UCHAR reporting_cap;
#endif
#ifdef AVDTP_HAVE_ROHC_SERVICE
    /** header compression cap,
    *  0x00 means the endpoint doesn't have this service.
    */
    UCHAR rohc_cap;
#endif
#ifdef AVDTP_HAVE_MULTIPLEXING
    UCHAR multiplexing_cap;
#endif

    /** Type of the SEP - Source/Sink */
    UCHAR sep_type;

} AVDTP_SEP_CAP;

/**
 *  \struct AVDTP_SEP_CONF BT_avdtp_api.h
 *  \brief  AVDTP Stream End Point Configuration information
 *
 *  This data type represents the Configuration of a Stream End Point.
 *
 *  \note
 *  Upper layer applications/profiles should use this data type while
 *  requesting configuration of a local SEP with respect to a remote one,
 *  or, when decoding AVDTP provided buffer with Set Configuration
 *  Indication.
 *
 *  \sa
 *  BT_avdtp_set_configuration_req, AVDTP_SET_CONFIGURATION_IND
 */
typedef struct
{
    /** Codec Capability information */
    AVDTP_CODEC_CAP codec_cap;

#ifdef AVDTP_HAVE_CONTENT_PROTECTION
    /** Content Protection Capability information */
    AVDTP_CP_CAP cp_cap [AVDTP_MAX_CP_IE];
#endif /* AVDTP_HAVE_CONTENT_PROTECTION */

    /**
     *  Bitmap containing service requested/configured.
     *
     *  Each bit signifies a particular service, as described below:
     *  \li   Bit 0: Media Transport Service
     *  \li   Bit 1: Reporting Service
     *  \li   Bit 2: Recovery Service
     *  \li   Bit 3: Multiplexing Service
     *  \li   Bit 4: Fragmentation Service
     *  \li   Bit 5: Robust Header Compression Service
     *  \li   Bit 6: Delay Reporting Service
     */
    UCHAR service_flag;

    /**
     *  Recovery Capability information.
     *
     *  \note
     *  This element is valid only if Recovery Service is configured.
     */
    AVDTP_RECOVERY_CAP recovery_cap;

    /**
     *  Robust Header Compression Settings (BackCh, Media, Recovery).
     *
     *  \note
     *  This element is valid only if ROHC is configured.
     */
    UCHAR rohc_conf;

    /**
     *  Pointer to the Multiplexing Service Capability information element
     */
    UCHAR mux_offset;

    /**
     *  First Service Category to fail in case of
     *  Set Configuration/Reconfigure Command. This will be included in
     *  Reject Response.
     */
    UCHAR error_info;

} AVDTP_SEP_CONF;
/** \} */
/** \} */

/* ----------------------------------------- Macros */
/**
 * \defgroup avdtp_utility_macros Utility Macros
 * \{
 * Describes Utility Macros defined by AVDTP module.
 */
/**
 *  @name AVDTP Utility Macros
 *  Defining AVDTP Utility Macros.
 */

/*@{*/

/**
 *  This utility macro initializes an AVDTP Handle. Applications may use
 *  this macro prior to set important elements of an AVDTP Handle.
 *
 *  \param [in] handle
 *         The AVDTP Handle to be initialized.
 *
 *  \hideinitializer
 */
#define AVDTP_INIT_HANDLE(handle)      \
        (handle).local_seid   = 0x00U; \
        (handle).remote_seid  = 0x00U; \
        (handle).bd_addr[0U]  = 0x00U; \
        (handle).bd_addr[1U]  = 0x00U; \
        (handle).bd_addr[2U]  = 0x00U; \
        (handle).bd_addr[3U]  = 0x00U; \
        (handle).bd_addr[4U]  = 0x00U; \
        (handle).bd_addr[5U]  = 0x00U; \
        (handle).ntf_cb       = NULL;

/**
 *  This utility macro initializes the Event Notification Callback in an
 *  AVDTP Handle. Applications must use this macro to set the notification
 *  callback that AVDTP is required to invoke to confirm an application
 *  initiated AVDTP procedure.
 *
 *  \param [in] handle
 *         The AVDTP Handle.
 *  \param [in] callback
 *         The AVDTP Event Notification Callback to be set.
 *
 *  \hideinitializer
 */
#define AVDTP_SET_HANDLE_CALLBACK(handle, callback)    \
        (handle).ntf_cb = (callback);

/**
 *  This utility macro sets remote Bluetooth Device Address in an
 *  AVDTP Handle.
 *
 *  \param [in] handle
 *         The AVDTP Handle.
 *  \param [in] bd
 *         The remote Bluetooth Device Address to be set.
 *
 *  \hideinitializer
 */
#define AVDTP_SET_HANDLE_BD_ADDR(handle, bd)      \
        (handle).bd_addr[0U] = (bd)[0U];          \
        (handle).bd_addr[1U] = (bd)[1U];          \
        (handle).bd_addr[2U] = (bd)[2U];          \
        (handle).bd_addr[3U] = (bd)[3U];          \
        (handle).bd_addr[4U] = (bd)[4U];          \
        (handle).bd_addr[5U] = (bd)[5U];

/**
 *  This utility macro sets the local Stream End Point Identifier (SEID)
 *  in an AVDTP Handle.
 *
 *  \param [in] handle
 *         The AVDTP Handle.
 *  \param [in] seid
 *         The local SEID to be set.
 *
 *  \hideinitializer
 */
#define AVDTP_SET_HANDLE_LOCAL_SEID(handle, seid)    \
        (handle).local_seid = (seid);

/**
 *  This utility macro sets the remote Stream End Point Identifier (SEID)
 *  in an AVDTP Handle.
 *
 *  \param [in] handle
 *         The AVDTP Handle.
 *  \param [in] seid
 *         The local SEID to be set.
 *
 *  \hideinitializer
 */
#define AVDTP_SET_HANDLE_REMOTE_SEID(handle, seid)    \
        (handle).remote_seid = (seid);

/**
 *  This utility macro retrieves remote Bluetooth Device Address
 *  from an AVDTP Handle.
 *
 *  \param [in] handle
 *         The AVDTP Handle.
 *  \param [out] bd_addr
 *         The remote Bluetooth Device Address to be populated.
 *
 *  \hideinitializer
 */
#define AVDTP_GET_HANDLE_BD_ADDR(handle, bd)    \
        (bd)[0U] = (handle).bd_addr[0U]; \
        (bd)[1U] = (handle).bd_addr[1U]; \
        (bd)[2U] = (handle).bd_addr[2U]; \
        (bd)[3U] = (handle).bd_addr[3U]; \
        (bd)[4U] = (handle).bd_addr[4U]; \
        (bd)[5U] = (handle).bd_addr[5U];

/**
 *  This utility macro retrieves the local Stream End Point Identifier
 *  (SEID) from an AVDTP Handle.
 *
 *  \param [in] handle
 *         The AVDTP Handle.
 *  \param [out] local_seid
 *         The local SEID to be populated.
 *
 *  \hideinitializer
 */
#define AVDTP_GET_HANDLE_LOCAL_SEID(handle, seid)    \
        (seid) = (handle).local_seid;

/**
 *  This utility macro retrieves the remote Stream End Point Identifier
 *  (SEID) from an AVDTP Handle.
 *
 *  \param [in] handle
 *         The AVDTP Handle.
 *  \param [out] remote_seid
 *         The remote SEID to be populated.
 *
 *  \hideinitializer
 */
#define AVDTP_GET_HANDLE_REMOTE_SEID(handle, seid)  \
        (seid) = (handle).remote_seid;

/**
 *  This utility macro is used to compare two AVDTP handles
 *  based on Local SEID.
 *
 *  \param [in] h1
 *         The AVDTP Handle 1.
 *  \param [in] h1
 *         The AVDTP Handle 2.
 *
 *  \hideinitializer
 */
#define AVDTP_COMPARE_HANDLE(h1, h2) \
        ((h1).local_seid == (h2).local_seid)

/**
 *  This utility macro is used to retrieve SEP Capability
 *  parameters from AVDTP SEP Capability Structure (AVDTP_SEP_CAP).
 *
 *  \param [in] sep_cap
 *         AVDTP SEP Capability Structure.
 *  \param [out] s_t
 *         AVDTP Stream End Point Type.
 *  \param [out] m_t
 *         AVDTP Media Type.
 *  \param [out] c_t
 *         AVDTP Media Codec Type - SBC/MPEG/ATRAC etc.
 *  \param [out] ie_l
 *         Length of the Codec Capabilities information Element.
 *  \param [out] ie
 *         Buffer containing Codec Capabilities Informataion Element.
 *
 *  \hideinitializer
 */
#define AVDTP_GET_SEP_CAPABILITY(sep_cap, s_t, m_t, c_t, ie_l, ie) \
        (s_t)  = (sep_cap).sep_type; \
        (m_t)  = (sep_cap).codec_cap.media_type; \
        (c_t)  = (sep_cap).codec_cap.codec_type; \
        (ie_l) = (sep_cap).codec_cap.codec_ie_len; \
        (ie)   = (sep_cap).codec_cap.codec_ie;

/**
 *  This utility macro is used to retrieve SEP Recovery capability
 *  parameters from AVDTP SEP Capability Structure (AVDTP_SEP_CAP).
 *
 *  \param [in] sep_cap
 *         AVDTP SEP Capability Structure.
 *
 *  \param [out] recovery_t
 *         AVDTP Recovery Type.
 *
 *  \param [out] mrws_val
 *         Maximum Recovery Window Size for a Transport Packet.
 *
 *  \param [out] mnmp_val
 *         The maximum number of media packets a specific parity code covers.
 *
 *  \hideinitializer
 */
#define AVDTP_GET_SEP_RECOVERY_CAPABILITY(sep_cap, recovery_t, mrws_val, mnmp_val) \
        (recovery_t) = (sep_cap).recovery_cap.recovery_type;                       \
        (mrws_val)   = (sep_cap).recovery_cap.mrws;                                \
        (mnmp_val)   = (sep_cap).recovery_cap.mnmp;

/**
 *  This utility macro is used to set Codec Capability information element
 *  in AVDTP_SEP_CAP.
 *
 *  \param [in] cap
 *         AVDTP SEP Capability Structure.
 *  \param [in] ie
 *         Codec Capability Information elements
 *
 *  \hideinitializer
 */
#define AVDTP_SET_SEP_CAPABILITY_CODEC_IE(cap, ie) \
        (cap).codec_cap.codec_ie = (ie);

/**
 *  This utility macro is used to get Codec Configuration Information element
 *  from AVDTP_SEP_CONF.
 *
 *  \param [in] conf
 *         AVDTP SEP Configuration Structure.
 *  \param [in] ie
 *         Codec Configuration Information elements
 *
 *  \hideinitializer
 */
#define AVDTP_GET_SEP_CONFIGURATION_CODEC_IE(conf, ie) \
        (ie) = (conf).codec_cap.codec_ie;

/**
 *  This utility macro is used to retrieve SEP Configuration
 *  parameters from AVDTP_SEP_CONF Structure.
 *
 *  \param [in] conf
 *         AVDTP SEP Configuration Structure.
 *  \param [out] m_t
 *         AVDTP Media Type.
 *  \param [out] c_t
 *         AVDTP Media Codec Type - SBC/MPEG/ATRAC etc.
 *  \param [out] ie_l
 *         Length of the Codec Configuration Information Element.
 *  \param [out] ie
 *         Buffer containing Codec Configuration Information Element.
 *
 *  \hideinitializer
 */
#define AVDTP_GET_SEP_CONFIGURATION_CODEC_CAP(conf, m_t, c_t, ie_l, ie) \
        (m_t)  = (conf).codec_cap.media_type;     \
        (c_t)  = (conf).codec_cap.codec_type;     \
        (ie_l) = (conf).codec_cap.codec_ie_len;   \
        (ie)   = (conf).codec_cap.codec_ie;

/**
 *  This utility macro is used to retrieve Content Protection Type
 *  parameters from AVDTP_SEP_CONF Structure.
 *
 *  \param [in] conf
 *         AVDTP SEP Configuration Structure.
 *  \param [out] cp_t
 *         AVDTP Content Protection Type.
 *
 *  \hideinitializer
 */
#define AVDTP_GET_SEP_CONFIGURATION_CP_TYPE(conf, cp_t) \
        (cp_t)  = (conf).cp_cap[0U].cp_type;

/**
 *  This utility macro is used to retrieve AVDTP Delay Reporting
 *  Configuration Status.
 *
 *  \param [in] conf
 *         AVDTP SEP Configuration Structure.
 *  \param [out] delay_r
 *         AVDTP Delay Reporting Status.
 *
 *  \hideinitializer
 */
#define AVDTP_GET_SEP_CONFIGURATION_DELAY_REPORTING(conf, delay_r) \
        (delay_r)  = (conf).service_flag & AVDTP_MASK_SERVICE_DELAY_REPORTING;

/**
 *  This utility macro is used to initialize Codec Parameters of the SEP
 *  Capability Structure (AVDTP_SEP_CAP).
 *
 *  \param [in] sep_cap
 *         AVDTP SEP Capability Structure.
 *  \param [in] s_t
 *         AVDTP Stream End Point Type.
 *  \param [in] m_t
 *         AVDTP Media Type.
 *  \param [in] c_t
 *         AVDTP Media Codec Type - SBC/MPEG/ATRAC etc.
 *  \param [in] ie_l
 *         Length of the Codec Capabilities information Element.
 *  \param [in] ie
 *         Buffer containing Codec Capabilities Information Element.
 *
 *  \par Stream End Point Type
 *  Stream End Point Type should be selected from one of constants given
 *  below:
 *  - AVDTP_SEP_SOURCE
 *  - AVDTP_SEP_SINK
 *
 *  \par Media Type
 *  Media Type should be selected from one of constants give below:
 *  - AVDTP_MEDIA_AUDIO
 *  - AVDTP_MEDIA_VIDEO
 *  - AVDTP_MEDIA_MULTIMEDIA
 *
 *  \par Media Codec Type
 *  Media Codec Type should be selected from one of constants give below:
 *  - AVDTP_CODEC_AUDIO_SBC
 *  - AVDTP_CODEC_AUDIO_MPEG_1_2
 *  - AVDTP_CODEC_AUDIO_MPEG_2_4
 *  - AVDTP_CODEC_AUDIO_ATRAC
 *
 *  \sa
 *  AVDTP Media Types, AVDTP Stream End Point Types, AVDTP Media Codec Types
 *
 *  \hideinitializer
 */
#define AVDTP_SET_SEP_CAPABILITY(sep_cap, s_t, m_t, c_t, ie_l, ie) \
        (sep_cap).sep_type               = (s_t);                  \
        (sep_cap).codec_cap.media_type   = (m_t);                  \
        (sep_cap).codec_cap.codec_type   = (c_t);                  \
        (sep_cap).codec_cap.codec_ie_len = (ie_l);                 \
        (sep_cap).codec_cap.codec_ie     = (ie);

/**
 *  This utility macro is used to set SEP Content Protection capabilities.
 *
 *  \param [in] sep_cp
 *         AVDTP_CP_CAP structure.
 *
 *  \param [in] cp_t
 *         AVDTP Content Protection Type.
 *
 *  \param [in] cp_ie_buf
 *         Content protection ie buffer.
 *
 *  \param cp_ie_buf_len
 *         Content protection ie buffer length.
 *
 *  \hideinitializer
 */
#define AVDTP_SET_SEP_CP_CAPABILITY(sep_cp, cp_t, cp_ie_buf, cp_ie_buf_len) \
        (sep_cp).cp_type                 = (cp_t);                          \
        (sep_cp).cp_ie                   = (cp_ie_buf);                     \
        (sep_cp).cp_ie_len               = (cp_ie_buf_len);

/**
 *  This utility macro is used to set SEP Recovery capabilities.
 *
 *  \param [in] sep_cap
 *         AVDTP SEP Capability Structure.
 *
 *  \param [in] recovery_t
 *         AVDTP Recovery Type.
 *
 *  \param [in] mrws_val
 *         Maximum Recovery Window Size for a Transport Packet.
 *
 *  \param [in] mnmp_val
 *         The maximum number of media packets a specific parity code covers.
 *
 *  \hideinitializer
 */
#define AVDTP_SET_SEP_RECOVERY_CAPABILITY(sep_cap, recovery_t, mrws_val, mnmp_val) \
        (sep_cap).recovery_cap.recovery_type = (recovery_t);                       \
        (sep_cap).recovery_cap.mrws = (mrws_val);                                  \
        (sep_cap).recovery_cap.mnmp = (mnmp_val);

/**
 *  This utility macro is used to initialize the SEP Configuration
 *  Structure (AVDTP_SEP_CONF). Applications must use this macro prior
 *  to set important elements of AVDTP_SEP_CONF.
 *
 *  \param [in] sep_conf
 *         AVDTP SEP Configuration Structure.
 *
 *  \hideinitializer
 */
#define AVDTP_INIT_SEP_CONFIGURATION(sep_conf) \
        (sep_conf).service_flag           = 0x00U;

/**
 *  This utility macro is used to set Media Transport Service during
 *  Stream Configuration. Applications must use this macro if Media
 *  Transport Service is required for a stream.
 *
 *  \param [in] sep_conf
 *         AVDTP SEP Configuration Structure.
 *
 *  \hideinitializer
 */
#define AVDTP_SET_SEP_CONFIGURATION_MEDIA(sep_conf) \
        (sep_conf).service_flag |= AVDTP_MASK_SERVICE_MEDIA;

/**
 *  This utility macro is used to set Reporting Service during
 *  Stream Configuration. Applications must use this macro if
 *  Reporting Service is required for a stream.
 *
 *  \param [in] sep_conf
 *         AVDTP SEP Configuration Structure.
 *
 *  \hideinitializer
 */
#define AVDTP_SET_SEP_CONFIGURATION_REPORTING(sep_conf) \
        (sep_conf).service_flag |= AVDTP_MASK_SERVICE_REPORTING;

/**
 *  This utility macro is used to set Multiplexing Service during
 *  Stream Configuration. Applications must use this macro if
 *  Multiplexing Service is required for a stream.
 *
 *  \param [in] sep_conf
 *         AVDTP SEP Configuration Structure.
 *
 *  \hideinitializer
 */
#define AVDTP_SET_SEP_CONFIGURATION_MULTIPLEXING(sep_conf) \
        (sep_conf).service_flag |= AVDTP_MASK_SERVICE_MULTIPLEXING;

/**
 *  This utility macro is used to set Multiplexing Fragmentation
 *  feature during Stream Configuration. Applications must use this
 *  macro only if Multiplexing Service is also set for this stream.
 *
 *  \param [in] sep_conf
 *         AVDTP SEP Configuration Structure.
 *
 *  \hideinitializer
 */
#define AVDTP_SET_SEP_CONFIGURATION_FRAGMENTATION(sep_conf) \
        (sep_conf).service_flag |= AVDTP_MASK_SERVICE_FRAGMENTATION;

/**
 *  This utility macro is used to set Robust Header Compression
 *  Service during Stream Configuration. Applications must use
 *  this macro if Robust Header Compression Service is required
 *  for a stream.
 *
 *  \param [in] sep_conf
 *         AVDTP SEP Configuration Structure.
 *
 *  \hideinitializer
 */
#define AVDTP_SET_SEP_CONFIGURATION_ROHC(sep_conf) \
        (sep_conf).service_flag |= AVDTP_MASK_SERVICE_ROHC;

/**
 *  This utility macro is used to set Recovery Parameters
 *  during Stream Configuration. Applications must use this macro
 *  if Recovery Service is required for a stream.
 *
 *  \param [in] sep_conf
 *         AVDTP SEP Configuration Structure.
 *  \param [in] type
 *         Type of Recovery.
 *  \param [in] mrws
 *         Maximum Recovery Window Size.
 *  \param [in] mnmp
 *         Maximum number of Media packets a specific parity code covers.
 *
 *  \hideinitializer
 */
#define AVDTP_SET_SEP_CONFIGURATION_RECOVERY(sep_conf, type, mrws_val, mnmp_val) \
        (sep_conf).recovery_cap.recovery_type  = (type);                         \
        (sep_conf).recovery_cap.mrws  = (mrws_val);                              \
        (sep_conf).recovery_cap.mnmp  = (mnmp_val);                              \
        (sep_conf).service_flag  |= AVDTP_MASK_SERVICE_RECOVERY;

/**
 *  This utility macro is used to set Delay Reporting Service during
 *  Stream Configuration. Applications must use this macro if Delay
 *  Reporting Service is required for a stream.
 *
 *  \param [in] sep_conf
 *         AVDTP SEP Configuration Structure.
 *
 *  \hideinitializer
 */
#define AVDTP_SET_SEP_CONFIGURATION_DELAY_REPORTING(sep_conf) \
        (sep_conf).service_flag |= AVDTP_MASK_SERVICE_DELAY_REPORTING;

/**
 *  This utility macro is used to initialize important elements of the SEP
 *  Configuration Structure (AVDTP_SEP_CONF).
 *
 *  \param [in] sep_conf
 *         AVDTP SEP Configuration Structure.
 *  \param [in] m_t
 *         AVDTP Media Type - Audio/Video/Multimedia.
 *  \param [in] c_t
 *         AVDTP Media Codec Type - SBC/MPEG/ATRAC etc.
 *  \param [in] ie_l
 *         Length of the Codec Capabilities information Element.
 *  \param [in] ie
 *         Buffer containing Codec Capabilities Informtaion Element.
 *
 *  \par Media Type
 *  Media Type should be selected from one of constants give below:
 *  - AVDTP_MEDIA_AUDIO
 *  - AVDTP_MEDIA_VIDEO
 *  - AVDTP_MEDIA_MULTIMEDIA
 *
 *  \par Media Codec Type
 *  Media Codec Type should be selected from one of constants give below:
 *  - AVDTP_CODEC_AUDIO_SBC
 *  - AVDTP_CODEC_AUDIO_MPEG_1_2
 *  - AVDTP_CODEC_AUDIO_MPEG_2_4
 *  - AVDTP_CODEC_AUDIO_ATRAC
 *
 *  \sa
 *  AVDTP Media Types, AVDTP Media Codec Types
 *
 *  \hideinitializer
 */
#define AVDTP_SET_SEP_CONFIGURATION_CODEC_CAP(sep_conf, m_t, c_t, ie_l, ie) \
        (sep_conf).codec_cap.media_type   = (m_t);    \
        (sep_conf).codec_cap.codec_type   = (c_t);    \
        (sep_conf).codec_cap.codec_ie_len = (ie_l);   \
        (sep_conf).codec_cap.codec_ie     = (ie);

/*
 *  Construct Media Packet Header
 *
 *              7   6   5   4   3   2   1   0
 *  Octet  0:   ------CC-----   X   P   --V--
 *  Octet  1:   ------------PT-----------   M
 *  Octet  2:   -----------Seq No------------
 *  Octet  3:   -----------Seq No------------
 *  Octet  4:   -------------TS--------------
 *  Octet  5:   -------------TS--------------
 *  Octet  6:   -------------TS--------------
 *  Octet  7:   -------------TS--------------
 *  Octet  8:   ------------SSRC-------------
 *  Octet  9:   ------------SSRC-------------
 *  Octet 10:   ------------SSRC-------------
 *  Octet 11:   ------------SSRC-------------
 *
 *  V   : Version
 *  P   : Padding
 *  X   : Extension Bit
 *  CC  : CSRC Count (Not Supported)
 *  M   : Marker Bit
 *  PT  : Payload Type
 *  Seq : Sequence Number (Filled by AVDTP)
 *  TS  : Timestamp
 *  SSRC: Synchronization Source
 */

/**
 *  This utility macro is used to initialize a Media Packet Header with
 *  Version, Padding, Extension, Marker and Payload Type.
 *
 *  \param [in,out] ptr
 *         Pointer to the buffer location for the Media Packet Header
 *  \param [in] v
 *         Version of the RTP implementation
 *  \param [in] p
 *         Padding Bit (see AVDTP Specification v1.0, section 7.2.1)
 *  \param [in] x
 *         Extension Bit (see AVDTP Specification v1.0, section 7.2.1)
 *  \param [in] m
 *         Marker Bit (see AVDTP Specification v1.0, section 7.2.1)
 *  \param [in] pt
 *         Payload Type (see AVDTP Specification v1.0, section 7.2.1)
 *
 *  \hideinitializer
 */
#define AVDTP_INIT_MEDIA_PACKET_HEADER(ptr, v, p, x, m, pt) \
        (ptr)[0U]  = (((v) & 0x03U) | (((p) & 0x01U) << 2U) | (((x) & 0x01U) << 3U)); \
        (ptr)[1U]  = (((m) & 0x01U) | (((pt) & 0x7FU) << 1U));

/**
 *  This utility macro is used to initialize a Media Packet Header with
 *  Sequence Number information.
 *
 *  \param [in,out] ptr
 *         Pointer to the buffer location for the Media Packet Header
 *  \param [in] seq
 *         Sequence Number (see AVDTP Specification v1.0, section 7.2.1)
 *
 *  \hideinitializer
 */
#define AVDTP_SET_MEDIA_PACKET_HEADER_SEQ(ptr, seq) \
        (ptr)[2U]  = (UCHAR) ((seq) >> 8U); \
        (ptr)[3U]  = (UCHAR) (seq);

/**
 *  This utility macro is used to initialize a Media Packet Header with
 *  Time Stamp information.
 *
 *  \param [in,out] ptr
 *         Pointer to the buffer location for the Media Packet Header
 *  \param [in] ts
 *         Time Stamp (see AVDTP Specification v1.0, section 7.2.1)
 *
 *  \hideinitializer
 */
#define AVDTP_SET_MEDIA_PACKET_HEADER_TS(ptr, ts) \
        (ptr)[4U]  = (UCHAR) ((ts) >> 24U); \
        (ptr)[5U]  = (UCHAR) ((ts) >> 16U); \
        (ptr)[6U]  = (UCHAR) ((ts) >> 8U); \
        (ptr)[7U]  = (UCHAR) (ts);

/**
 *  This utility macro is used to initialize a Media Packet Header with
 *  Synchronization Source (SSRC) information.
 *
 *  \param [in,out] ptr
 *         Pointer to the buffer location for the Media Packet Header
 *  \param [in] ssrc
 *         The SSRC (see AVDTP Specification v1.0, section 7.2.1)
 *
 *  \hideinitializer
 */
#define AVDTP_SET_MEDIA_PACKET_HEADER_SSRC(ptr, ssrc) \
        (ptr)[8U]  = (UCHAR) ((ssrc) >> 24U); \
        (ptr)[9U]  = (UCHAR) ((ssrc) >> 16U); \
        (ptr)[10U] = (UCHAR) ((ssrc) >> 8U); \
        (ptr)[11U] = (UCHAR) (ssrc);

/**
 *  This utility macro is used to initialize a Media Packet Header with
 *  all required information - Version, Padding, Extension, Marker and
 *  Payload Type, Time Stamp and Synchronization Source (SSRC) information.
 *  The application does not need to provide the Sequence Number, as the
 *  AVDTP fills up the Sequence Number field internally. Also, this
 *  version of AVDTP implementation does not support transmission of
 *  Contributing Source (CSRC) information. Hence, the "CSRC Count (CC)"
 *  field is set to zero.
 *
 *  \param [in,out] ptr
 *         Pointer to the buffer location for the Media Packet Header
 *  \param [in] v
 *         Version of the RTP implementation
 *  \param [in] p
 *         Padding Bit (see AVDTP Specification v1.0, section 7.2.1)
 *  \param [in] x
 *         Extension Bit (see AVDTP Specification v1.0, section 7.2.1)
 *  \param [in] m
 *         Marker Bit (see AVDTP Specification v1.0, section 7.2.1)
 *  \param [in] pt
 *         Payload Type (see AVDTP Specification v1.0, section 7.2.1)
 *  \param [in] ts
 *         Time Stamp (see AVDTP Specification v1.0, section 7.2.1)
 *  \param [in] ssrc
 *         The SSRC (see AVDTP Specification v1.0, section 7.2.1)
 *
 *  \hideinitializer
 */
#define AVDTP_SET_MEDIA_PACKET_HEADER(ptr, v, p, x, m, pt, ts, ssrc) \
        AVDTP_INIT_MEDIA_PACKET_HEADER((ptr), (v), (p), (x), (m), (pt)); \
        AVDTP_SET_MEDIA_PACKET_HEADER_TS((ptr), (ts)); \
        AVDTP_SET_MEDIA_PACKET_HEADER_SSRC((ptr), (ssrc));

/*@}*/

/** \} */
/* ----------------------------------------- Internal APIs */

/**
 * \defgroup avdtp_api API Definitions
 * \{
 * Describes APIs defined by the module.
 */
#ifdef __cplusplus
extern "C" {
#endif

/*@{*/
/**
 * \cond ignore_this
 * \{
 */
/*
 *  Common API Handler for the following AVDTP APIs:
 *  - BT_avdtp_connect_req
 *  - BT_avdtp_disconnect_req
 *  - BT_avdtp_open_req
 *  - BT_avdtp_close_req
 *  - BT_avdtp_start_req
 *  - BT_avdtp_suspend_req
 *  - BT_avdtp_abort_req
 */
API_RESULT avdtp_common_api_handler_set_1
           (
               /* IN */ AVDTP_HANDLE *  handle,
               /* IN */ UCHAR           req_type,
               /* IN */ void            (* bh_func)(void *args, UINT16 args_length)
           );

/*
 *  Common API Handler for the following AVDTP APIs:
 *  - BT_avdtp_discover_req
 *  - BT_avdtp_get_capabilities_req
 *  - BT_avdtp_get_configuration_req
 */
API_RESULT avdtp_common_api_handler_set_2
           (
               /* IN */ AVDTP_HANDLE *  handle,
               /* IN */ UCHAR *         data,
               /* IN */ UINT16          datalen,
               /* IN */ UCHAR           req_type,
               /* IN */ void            (* bh_func)(void *args, UINT16 args_length)
           );

/*
 *  Common API Handler for the following AVDTP APIs:
 *  - BT_avdtp_set_configuration_req
 *  - BT_avdtp_reconfigure_req
 */
API_RESULT avdtp_common_api_handler_set_3
           (
               /* IN */ AVDTP_HANDLE *      handle,
               /* IN */ AVDTP_SEP_CONF *    sep_conf,
               /* IN */ UCHAR               req_type,
               /* IN */ void                (* bh_func)(void *args, UINT16 args_length)
           );

/*
 *  Decoding function for the following AVDTP API.
 *  - BT_avdtp_decode_sep_configuration
 */
API_RESULT avdtp_decode_sep_configuration
           (
               /* IN */  UCHAR           sig_id,
               /* IN */  UCHAR           *data,
               /* IN */  UINT16          datalen,
               /* OUT */ AVDTP_SEP_CONF  *sep_conf
           );

/** \endcond */
/*@}*/


/* ----------------------------------------- API Declarations */

/**
 *  @name AVDTP APIs - Callback Registration
 */

/*@{*/

/**
 *  \brief To register the application's callback
 *
 *  \par Description:
 *  This API enables applications to register the callback for receiving
 *  asynchornous indication for the following AVDTP Events:
 *  - AVDTP_CONNECT_IND (establishment of AVDTP Control channel)
 *  - AVDTP_DISCONNECT_IND (disconnection of AVDTP Control channel)
 *  - AVDTP_DISCOVER_IND (when Peer sends an AVDTP Discover message)
 *
 *  \param [in] ind_cb
 *         The AVDTP Event Notification Callback which AVDTP should use to
 *         report the asynchronous AVDTP_CONNECT_IND & AVDTP_DISCONNECT_IND
 *
 *  \return API_RESULT: API_SUCCESS, or an Error Code describing the reason
 *                      for failure.
 */

API_RESULT BT_avdtp_callback_register
           (
               /* IN */  AVDTP_EVENT_NTF_CB    ind_cb
           );

/*@}*/

/**
 *  @name AVDTP APIs - Stream Registration
 */

/*@{*/

/**
 *  \brief To register a Stream End Point with AVDTP.
 *
 *  \par Description:
 *  This API enables applications/profiles to register a Stream End Point,
 *  along with its Application Service Capabilities, to AVDTP. On successful
 *  registration, AVDTP allocates a Stream End Point Identifier and sets it
 *  in the AVDTP Handle passed by the application. This newly allocated
 *  SEID should be used hereafter by the application to refer to the local
 *  Stream End Point.
 *
 *  \param [in] sep_cap
 *         The Application Service Capabilities of the Stream End Point
 *         to be registered to AVDTP.
 *  \param [in] ind_cb
 *         The AVDTP Event Notification Callback which AVDTP should use to
 *         report any asynchronous event for this Stream End Point.
 *  \param [in,out] handle
 *         The AVDTP Handle in which local SEID will be appropriately set,
 *         on successful registration.
 *
 *  \return API_RESULT: API_SUCCESS, or an Error Code describing the reason
 *                      for failure.
 *
 *  \sa
 *  BT_avdtp_sep_deregister
 */
API_RESULT BT_avdtp_sep_register
           (
               /* IN */    AVDTP_SEP_CAP       *sep_cap,
               /* IN */    AVDTP_EVENT_NTF_CB  ind_cb,
               /* INOUT */ AVDTP_HANDLE        *handle
           );

/**
 *  \brief To deregister a Stream End Point to AVDTP.
 *
 *  \par Description:
 *  This API enables applications/profiles to deregister a Stream End Point,
 *  along with its Application Service Capabilities, from AVDTP.
 *
 *  \param [in] handle
 *         The AVDTP Handle, from which local SEID will be used to identify
 *         the registered Stream End Point to deregister.
 *
 *  \return API_RESULT: API_SUCCESS, or an Error Code describing the reason
 *                      for failure.
 *
 *  \note
 *  Applications/Profiles should call this API only when it is known that
 *  the Stream End Point is idle and not involved in any streaming
 *  connection.
 *
 *  \sa
 *  BT_avdtp_sep_register
 */
API_RESULT BT_avdtp_sep_deregister
           (
               /* IN */ AVDTP_HANDLE        *handle
           );

/*@}*/


/**
 *  @name AVDTP APIs - Signalling Connection
 */

/*@{*/

/**
 *  \brief To establish a Signalling Connection with a remote AVDTP entity.
 *
 *  \par Synopsis:
 *  API_RESULT BT_avdtp_connect_req
 *             (
 *                 IN AVDTP_HANDLE     *handle
 *             );
 *
 *  \par Description:
 *  This API enables applications/profiles to establish a Signalling
 *  Connection with a remote AVDTP entity.
 *
 *  \param [in] handle
 *         The AVDTP Handle, from which BD_ADDR will be used to identify
 *         the remote Bluetooth device.
 *
 *  \return API_RESULT
 *  - API_SUCCESS, on successful initiation of the procedure.
 *  - Error Code, describing the reason for failure.
 *
 *  \remark
 *
 *  \par
 *  Following parameters of the AVDTP Handle must be set appropriately,
 *  before invoking this API:
 *  - Bluetooth Device Address of the remote AVDTP entity
 *  - AVDTP_EVENT_NTF_CB, which AVDTP needs to invoke to inform
 *    result of the procedure.
 *
 *  \par
 *  AVDTP will confirm the result of the procedure by invoking the
 *  AVDTP_EVENT_NTF_CB, with following parameters:
 *  - event_type:    AVDTP_CONNECT_CNF
 *  - event_result:  0x0000, if successful. Otherwise, it is an error code
 *                   describing the reason for failure.
 *  - avdtp_handle:  Set to AVDTP Handle provided by application during the
 *                   API call.
 *  - event_data:    NULL
 *  - event_datalen: 0 (Zero)
 *
 *  \note
 *  Even though this API is implemented as a macro, it should be treated
 *  as a function while invoking. An example is shown below:
 *
 *  \par
 *  \code
 *
 *      AVDTP_SET_HANDLE_BD_ADDR(handle, bd_addr);
 *      AVDTP_SET_HANDLE_CALLBACK(handle, ntf_callback);
 *
 *      retval = BT_avdtp_connect_req(&handle);
 *      if (retval != API_SUCCESS)
 *      {
 *          Take recovery action ...
 *      }
 *      else
 *      {
 *          Signalling Connection initiated ...
 *          Wait for Notification Callback with AVDTP_CONNECT_CNF
 *      }
 *
 *  \endcode
 *
 *  \sa
 *  BT_avdtp_disconnect_req, AVDTP_HANDLE, AVDTP_EVENT_NTF_CB
 *
 *  \hideinitializer
 */
#ifdef BT_DRIVER_MODE

    #define BT_avdtp_connect_req(handle)    \
            avdtp_common_api_handler_set_1 ((handle), AVDTP_CONNECT_CNF, NULL)

#else  /* BT_DRIVER_MODE */

    #define BT_avdtp_connect_req(handle)    \
            avdtp_common_api_handler_set_1  \
            ((handle), AVDTP_CONNECT_CNF, avdtp_connect_req_bh)

#endif /* BT_DRIVER_MODE */


/**
 *  \brief To disconnect a Signalling Connection with a remote AVDTP entity.
 *
 *  \par Synopsis:
 *  API_RESULT BT_avdtp_disconnect_req
 *             (
 *                 IN AVDTP_HANDLE     *handle
 *             );
 *
 *  \par Description:
 *  This API enables applications/profiles to disconnect a Signalling
 *  Connection with a remote AVDTP entity.
 *
 *  \param [in] handle
 *         The AVDTP Handle, from which BD_ADDR will be used to identify
 *         the remote Bluetooth device.
 *
 *  \return API_RESULT
 *  - API_SUCCESS, on successful initiation of the procedure.
 *  - Error Code, describing the reason for failure.
 *
 *  \remark
 *  There should be no Streaming Connection to the specified remote
 *  AVDTP entity at the time of calling this API.
 *
 *  \par
 *  Following parameters of the AVDTP Handle must be set appropriately,
 *  before invoking this API:
 *  - Bluetooth Device Address of the remote AVDTP entity
 *  - Local SEID, Stream End Point Identifier of the local AVDTP entity.
 *    This must have been obtained during a prior registration of a SEP.
 *  - AVDTP_EVENT_NTF_CB, which AVDTP needs to invoke to inform
 *    result of the procedure.
 *
 *  \par
 *  AVDTP will confirm the result of the procedure by invoking the
 *  AVDTP_EVENT_NTF_CB, with following parameters:
 *  - event_type:    AVDTP_DISCONNECT_CNF
 *  - event_result:  0x0000, if successful. Otherwise, it is an error code
 *                   describing the reason for failure.
 *  - avdtp_handle:  Set to AVDTP Handle provided by application during the
 *                   API call.
 *  - event_data:    NULL
 *  - event_datalen: 0 (Zero)
 *
 *  \note
 *  Even though this API is implemented as a macro, it should be treated
 *  as a function while invoking. An example is shown below:
 *
 *  \par
 *  \code
 *
 *      AVDTP_SET_HANDLE_BD_ADDR(handle, bd_addr);
 *      AVDTP_SET_HANDLE_CALLBACK(handle, ntf_callback);
 *
 *      retval = BT_avdtp_disconnect_req(&handle);
 *      if (retval != API_SUCCESS)
 *      {
 *          Take recovery action ...
 *      }
 *      else
 *      {
 *          Signalling Disconnection initiated ...
 *          Wait for Notification Callback with AVDTP_DISCONNECT_CNF
 *      }
 *
 *  \endcode
 *
 *  \sa
 *  BT_avdtp_connect_req, AVDTP_HANDLE, AVDTP_EVENT_NTF_CB
 *
 *  \hideinitializer
 */
#ifdef BT_DRIVER_MODE

    #define BT_avdtp_disconnect_req(handle)    \
            avdtp_common_api_handler_set_1     \
            ( (handle), AVDTP_DISCONNECT_CNF, NULL )

#else  /* BT_DRIVER_MODE */

    #define BT_avdtp_disconnect_req(handle)    \
            avdtp_common_api_handler_set_1     \
            ( (handle), AVDTP_DISCONNECT_CNF, avdtp_disconnect_req_bh )

#endif/* BT_DRIVER_MODE */

/*@}*/


/**
 *  @name AVDTP APIs - Stream Discovery
 */

/*@{*/

/**
 *  \brief To discover Stream End Points on a remote AVDTP entity.
 *
 *  \par Synopsis:
 *  API_RESULT BT_avdtp_discover_req
 *             (
 *                 IN AVDTP_HANDLE *    handle,
 *                 IN UCHAR *           data,
 *                 IN UINT16            datalen
 *             );
 *
 *  \par Description:
 *  This API enables applications/profiles to discover Stream End Points
 *  on a remote AVDTP entities. Stream End Point Discovery is not a
 *  mandatory procedure and can be skipped prior to Stream Configuration
 *  and Establishment procedures, if the application has prior knowledge
 *  about the remote SEPs.
 *
 *  \param [in] handle
 *         The AVDTP Handle, from which BD_ADDR will be used to identify
 *         the remote Bluetooth device.
 *  \param [in] data
 *         An application allocated buffer, which AVDTP will use to store
 *         the AVDTP packet segment containing the information about
 *         remote SEPs. The buffer pointer must be resident/global.
 *  \param [in] datalen
 *         Size of the application allocated buffer.
 *
 *  \return API_RESULT
 *  - API_SUCCESS, on successful initiation of the procedure.
 *  - Error Code, describing the reason for failure.
 *
 *  \remark
 *  Signalling Channel must be available for the specified remote AVDTP
 *  entity, before this API could be invoked.
 *
 *  \par
 *  Following parameters of the AVDTP Handle must be set appropriately,
 *  before invoking this API:
 *  - Bluetooth Device Address of the remote AVDTP entity
 *  - AVDTP_EVENT_NTF_CB, which AVDTP needs to invoke to inform
 *    result of the procedure.
 *
 *  \par
 *  AVDTP will confirm the result of the procedure by invoking the
 *  AVDTP_EVENT_NTF_CB, with following parameters:
 *  - event_type:    AVDTP_DISCOVER_CNF
 *  - event_result:  0x0000, if successful. Otherwise, it is an error code
 *                   describing the reason for failure.
 *  - avdtp_handle:  Set to AVDTP Handle provided by application during the
 *                   API call.
 *  - event_data:    Buffer containing discover response
 *  - event_datalen: Discover response length
 *
 *  \par
 *  If the size of Discover response is more than the size of buffer
 *  passed with the request, Notification Callback will be called
 *  with the error code AVDTP_INSUFFICIENT_USER_AREA. The buffer will be
 *  filled with Discover response of size datalen passed with the request.
 *  Also the size of buffer to receive complete Discover response is passed
 *  as event_datalen.
 *
 *  \note
 *  This API is implemented as a macro, but it should be treated as a
 *  function while invoking. An example is shown below:
 *
 *  \par
 *  \code
 *
 *      AVDTP_SET_HANDLE_BD_ADDR(handle, bd_addr);
 *      AVDTP_SET_HANDLE_CALLBACK(handle, ntf_callback);
 *
 *      retval = BT_avdtp_discover_req(&handle, data, datalen);
 *      if (retval != API_SUCCESS)
 *      {
 *          Take recovery action ...
 *      }
 *      else
 *      {
 *          Discover Procedure initiated ...
 *          Wait for Notification Callback with AVDTP_DISCOVER_CNF
 *      }
 *
 *  \endcode
 *
 *  \sa
 *  BT_avdtp_connect_req, AVDTP_HANDLE, AVDTP_EVENT_NTF_CB
 *
 *  \hideinitializer
 */
#ifdef BT_DRIVER_MODE

#define BT_avdtp_discover_req(handle, data, datalen)  \
        avdtp_common_api_handler_set_2                \
        ((handle), (data), (datalen), AVDTP_DISCOVER_CNF, NULL)

#else  /* BT_DRIVER_MODE */

#define BT_avdtp_discover_req(handle, data, datalen)  \
        avdtp_common_api_handler_set_2                \
        ((handle), (data), (datalen),                 \
         AVDTP_DISCOVER_CNF, avdtp_common_req_bh_set_1)

#endif /* BT_DRIVER_MODE */


/**
 *  \brief To get capabilities of a remote Stream End Point.
 *
 *  \par Synopsis:
 *  API_RESULT BT_avdtp_get_capabilities_req
 *             (
 *                 IN AVDTP_HANDLE *    handle,
 *                 IN UCHAR *           data,
 *                 IN UINT16            datalen
 *             );
 *
 *  \par Description:
 *  This API enables applications/profiles to get application and
 *  transport service capabilities of a remote Stream End Point.
 *  This procedure usually follows AVDTP Discover procedure.
 *
 *  \param [in] handle
 *         The AVDTP Handle, from which BD_ADDR, and the remote SEID,
 *         will be used to identify the remote Stream End Point.
 *  \param [in] data
 *         An application allocated buffer, which AVDTP will use to store
 *         the AVDTP packet segment containing the Capability information
 *         from the remote SEP. The buffer pointer must be resident/global.
 *  \param [in] datalen
 *         Size of the application allocated buffer.
 *
 *  \return API_RESULT
 *  - API_SUCCESS, on successful initiation of the procedure.
 *  - Error Code, describing the reason for failure.
 *
 *  \remark
 *  Signalling Channel must be available for the specified remote AVDTP
 *  entity, before this API could be invoked.
 *
 *  \par
 *  Following parameters of the AVDTP Handle must be set appropriately,
 *  before invoking this API:
 *  - Bluetooth Device Address of the remote AVDTP entity.
 *  - Remote SEID, Stream End Point Identifier on the remote AVDTP Entity.
 *  - AVDTP_EVENT_NTF_CB, which AVDTP needs to invoke to inform
 *    result of the procedure.
 *
 *  \par
 *  AVDTP will confirm the result of the procedure by invoking the
 *  AVDTP_EVENT_NTF_CB, with following parameters:
 *  - event_type:    AVDTP_GET_CAPABILITIES_CNF
 *  - event_result:  0x0000, if successful. Otherwise, it is an error code
 *                   describing the reason for failure.
 *  - avdtp_handle:  Set to AVDTP Handle provided by application during the
 *                   API call.
 *  - event_data:    The response from the peer containing SEP Capabilities
 *  - event_datalen: Size of the response from the peer
 *
 *  \par
 *  If the size of Get Capabilities response is more than the size of buffer
 *  passed with the request, Notification Callback will be called
 *  with the error code AVDTP_INSUFFICIENT_USER_AREA. The information in
 *  the buffer should be ignored. The size of buffer to receive complete
 *  Get Capabilities response is passed as event_datalen.
 *
 *  \note
 *  This API is implemented as a macro, but it should be treated as a
 *  function while invoking. An example is shown below:
 *
 *  \par
 *  \code
 *
 *      AVDTP_SET_HANDLE_BD_ADDR(handle, bd_addr);
 *      AVDTP_SET_HANDLE_REMOTE_SEID(handle, remote_seid);
 *      AVDTP_SET_HANDLE_CALLBACK(handle, ntf_callback);
 *
 *      retval = BT_avdtp_get_capabilities_req(&handle, data, datalen);
 *      if (retval != API_SUCCESS)
 *      {
 *          Take recovery action ...
 *      }
 *      else
 *      {
 *          Get Capabilities Procedure initiated ...
 *          Wait for Notification Callback with AVDTP_GET_CAPABILITIES_CNF
 *      }
 *
 *  \endcode
 *
 *  \sa
 *  BT_avdtp_discover_req, AVDTP_HANDLE, AVDTP_EVENT_NTF_CB
 *
 *  \hideinitializer
 */
#ifdef BT_DRIVER_MODE

#define BT_avdtp_get_capabilities_req(handle, data, datalen)  \
        avdtp_common_api_handler_set_2                        \
        ((handle), (data), (datalen), AVDTP_GET_CAPABILITIES_CNF, NULL)

#else  /* BT_DRIVER_MODE */

#define BT_avdtp_get_capabilities_req(handle, data, datalen)  \
        avdtp_common_api_handler_set_2                        \
        ((handle), (data), (datalen),                         \
         AVDTP_GET_CAPABILITIES_CNF, avdtp_common_req_bh_set_1)

#endif /* BT_DRIVER_MODE */

/**
 *  \brief To get all capabilities of a remote Stream End Point.
 *
 *  \par Synopsis:
 *  API_RESULT BT_avdtp_get_all_capabilities_req
 *             (
 *                 IN AVDTP_HANDLE  * handle,
 *                 IN UCHAR         * data,
 *                 IN UINT16            datalen
 *             );
 *
 *  \par Description:
 *  This API enables applications/profiles to get all application and
 *  transport service capabilities of a remote Stream End Point.
 *  This procedure usually follows AVDTP Discover procedure.
 *
 *  \param [in] handle
 *         The AVDTP Handle, from which BD_ADDR, and the remote SEID,
 *         will be used to identify the remote Stream End Point.
 *  \param [in] data
 *         An application allocated buffer, which AVDTP will use to store
 *         the AVDTP packet segment containing the Capability information
 *         from the remote SEP. The buffer pointer must be resident/global.
 *  \param [in] datalen
 *         Size of the application allocated buffer.
 *
 *  \return API_RESULT
 *  - API_SUCCESS, on successful initiation of the procedure.
 *  - Error Code, describing the reason for failure.
 *
 *  \remark
 *  Signalling Channel must be available for the specified remote AVDTP
 *  entity, before this API could be invoked.
 */
#ifdef AVDTP_HAVE_GET_ALL_CAPABILITIES
#define BT_avdtp_get_all_capabilities_req(handle, data, datalen)  \
        avdtp_common_api_handler_set_2                            \
        ((handle), (data), (datalen),                             \
         AVDTP_GET_ALL_CAPABILITIES_CNF, avdtp_common_req_bh_set_1)
#endif /* AVDTP_HAVE_GET_ALL_CAPABILITIES */

/**
 *  \brief To get send delay report value.
 *
 *  \par Synopsis:
 *  API_RESULT BT_avdtp_send_delay_report
 *             (
 *                 IN AVDTP_HANDLE   * handle,
 *                 IN UCHAR          * data,
 *                 IN UINT16           datalen
 *             );
 *
 *  \par Description:
 *  This API enables applications/profiles to send delay report value from
 *  AVDTP Sink Stream End Point to remote AVDTP Source Stream End Point.
 *  This procedure usually follows after AVDTP configure and AVDTP start procedures.
 *
 *  \param [in] handle
 *         The AVDTP Handle, from which BD_ADDR, and the remote SEID,
 *         will be used to identify the remote Stream End Point.
 *  \param [in] data
 *         An application allocated buffer, which AVDTP will use to store
 *         the AVDTP packet segment containing the Delay Report. The buffer
 *         pointer must be resident/global.
 *  \param [in] datalen
 *         Size of the application allocated buffer(2 bytes).
 *
 *  \return API_RESULT
 *  - API_SUCCESS, on successful initiation of the procedure.
 *  - Error Code, describing the reason for failure.
 *
 *  \remark
 *  Signalling Channel must be available for the specified remote AVDTP
 *  entity, before this API could be invoked.
 */
#ifdef AVDTP_HAVE_DELAY_REPORTING
#define BT_avdtp_send_delay_report(handle, data, datalen)         \
        avdtp_common_api_handler_set_2                            \
        ((handle), (data), (datalen),                             \
         AVDTP_DELAY_REPORT_CNF, avdtp_common_req_bh_set_2)
#endif /* AVDTP_HAVE_DELAY_REPORTING */


/*@}*/



/**
 *  @name AVDTP APIs - Streaming Connection
 */

/*@{*/

/**
 *  \brief To configure local (& remote) Stream End Point(s).
 *
 *  \par Synopsis:
 *  API_RESULT BT_avdtp_set_configuration_req
 *             (
 *                 IN AVDTP_HANDLE *    handle,
 *                 IN AVDTP_SEP_CONF *  sep_conf
 *             );
 *
 *  \par Description:
 *  This API enables applications/profiles to configure a stream.
 *  On successful completion, AVDTP allocates resources for the
 *  new Stream Context, which is now identified by the local &
 *  remote SEP (addressed by local & remote * SEID).
 *
 *  \par
 *  Apart from specifying the Application Service Capabilities, the
 *  application invoking this API, should also specify the desired
 *  Transport Service Capabilities. AVDTP will return an Error Code,
 *  if the requested Transport Service Capability is not supported, or,
 *  cannot be accomodated for any reason.
 *
 *  \param [in] handle
 *         The AVDTP Handle, from which BD_ADDR, and the remote SEID,
 *         will be used to identify the remote Stream End Point.
 *  \param [in] sep_conf
 *         This parameter must be a pointer to a caller allocated
 *         AVDTP_SEP_CONF data type, containing the specification of
 *         Application and Transport Service Capabilities.
 *
 *  \return API_RESULT
 *  - API_SUCCESS, on successful initiation of the procedure.
 *  - Error Code, describing the reason for failure.
 *
 *  \remark
 *  Signalling Channel must be available for the specified remote AVDTP
 *  entity, before this API could be invoked.
 *
 *  \par
 *  Media Transport Service must always be requested in the 'service_flag'
 *  of AVDTP_SEP_CONF.
 *
 *  \par
 *  The local Stream End Point must be free to be configured for the new
 *  Stream Context with the specified remote SEP.
 *
 *  \par
 *  Following parameters of the AVDTP Handle must be set appropriately,
 *  before invoking this API:
 *  - Bluetooth Device Address of the remote AVDTP entity
 *  - Local SEID, Stream End Point Identifier of the local AVDTP entity.
 *    This must have been obtained during a prior registration of a SEP.
 *  - Remote SEID, Stream End Point Identifier on the remote AVDTP entity
 *  - AVDTP_EVENT_NTF_CB, which AVDTP needs to invoke to inform
 *    result of the procedure.
 *
 *  \par
 *  AVDTP will confirm the result of the procedure by invoking the
 *  AVDTP_EVENT_NTF_CB, with following parameters:
 *  - event_type:    AVDTP_SET_CONFIGURATION_CNF
 *  - event_result:  0x0000, if successful. Otherwise, it is an error code
 *                   describing the reason for failure.
 *  - avdtp_handle:  Set to AVDTP Handle provided by application during the
 *                   API call.
 *  - event_data:    NULL
 *  - event_datalen: 0 (Zero)
 *
 *  \note
 *  This API is implemented as a macro, but it should be treated as a
 *  function while invoking. An example is shown below:
 *
 *  \par
 *  \code
 *
 *      AVDTP_SET_HANDLE_BD_ADDR(handle, bd_addr);
 *      AVDTP_SET_HANDLE_LOCAL_SEID(handle, local_seid);
 *      AVDTP_SET_HANDLE_REMOTE_SEID(handle, remote_seid);
 *      AVDTP_SET_HANDLE_CALLBACK(handle, ntf_callback);
 *
 *      retval = BT_avdtp_set_configuration_req(&handle, &sep_conf);
 *      if (retval != API_SUCCESS)
 *      {
 *          Take recovery action ...
 *      }
 *      else
 *      {
 *          Set Configuration Procedure initiated ...
 *          Wait for Notification Callback with AVDTP_SET_CONFIGURATION_CNF
 *      }
 *
 *  \endcode
 *
 *  \sa
 *  AVDTP_SEP_CONF, AVDTP_HANDLE, AVDTP_EVENT_NTF_CB
 *
 *  \hideinitializer
 */
#ifdef BT_DRIVER_MODE

#define BT_avdtp_set_configuration_req(handle, sep_conf)    \
        avdtp_common_api_handler_set_3                      \
        ((handle), (sep_conf), AVDTP_SET_CONFIGURATION_CNF, NULL)

#else  /* BT_DRIVER_MODE */

#define BT_avdtp_set_configuration_req(handle, sep_conf)    \
        avdtp_common_api_handler_set_3                      \
        ((handle), (sep_conf),                              \
         AVDTP_SET_CONFIGURATION_CNF, avdtp_common_req_bh_set_3)

#endif /* BT_DRIVER_MODE */


/**
 *  \brief To get configuration of remote Stream End Point.
 *
 *  \par Synopsis:
 *  API_RESULT BT_avdtp_get_configuration_req
 *             (
 *                 IN AVDTP_HANDLE *    handle,
 *                 IN UCHAR *           data,
 *                 IN UINT16            datalen
 *             );
 *
 *  \par Description:
 *  This API enables applications/profiles to retrieve various Service
 *  and Application specific configuration of a stream context.
 *
 *  \param [in] handle
 *         The AVDTP Handle, from which BD_ADDR, and the remote SEID,
 *         will be used to identify the remote Stream End Point.
 *  \param [in] data
 *         An application allocated buffer, which AVDTP will use to store
 *         the AVDTP packet segment containing the configuration information
 *         from the remote SEP. The buffer pointer must be resident/global.
 *  \param [in] datalen
 *         Size of the application allocated buffer.
 *
 *  \return API_RESULT
 *  - API_SUCCESS, on successful initiation of the procedure.
 *  - Error Code, describing the reason for failure.
 *
 *  \remark
 *  Signalling Channel must be available for the specified remote AVDTP
 *  entity, and Stream must be configured before this API could be invoked.
 *
 *  \par
 *  Following parameters of the AVDTP Handle must be set appropriately,
 *  before invoking this API:
 *  - Bluetooth Device Address of the remote AVDTP entity
 *  - Local SEID, Stream End Point Identifier of the local AVDTP entity.
 *    This must have been obtained during a prior registration of a SEP.
 *  - Remote SEID, Stream End Point Identifier on the remote AVDTP entity
 *  - AVDTP_EVENT_NTF_CB, which AVDTP needs to invoke to inform
 *    result of the procedure.
 *
 *  \par
 *  AVDTP will confirm the result of the procedure by invoking the
 *  AVDTP_EVENT_NTF_CB, with following parameters:
 *  - event_type:    AVDTP_GET_CONFIGURATION_CNF
 *  - event_result:  0x0000, if successful. Otherwise, it is an error code
 *                   describing the reason for failure.
 *  - avdtp_handle:  Set to AVDTP Handle provided by application during the
 *                   API call.
 *  - event_data:    The response from the peer containing SEP Configuration
 *  - event_datalen: Size of the response from the peer
 *
 *  \par
 *  If the size of Get Configuration response is more than the size of buffer
 *  passed with the request, Notification Callback will be called
 *  with the error code AVDTP_INSUFFICIENT_USER_AREA. The information in
 *  the buffer should be ignored. The size of buffer to receive complete
 *  Get Configuration response is passed as event_datalen.
 *
 *  \note
 *  This API is implemented as a macro, but it should be treated as a
 *  function while invoking. An example is shown below:
 *
 *  \par
 *  \code
 *
 *      AVDTP_SET_HANDLE_BD_ADDR(handle, bd_addr);
 *      AVDTP_SET_HANDLE_LOCAL_SEID(handle, local_seid);
 *      AVDTP_SET_HANDLE_REMOTE_SEID(handle, remote_seid);
 *      AVDTP_SET_HANDLE_CALLBACK(handle, ntf_callback);
 *
 *      retval = BT_avdtp_get_configuration_req(&handle, data, datalen);
 *      if (retval != API_SUCCESS)
 *      {
 *          Take recovery action ...
 *      }
 *      else
 *      {
 *          Get configuration Procedure initiated ...
 *          Wait for Notification Callback with AVDTP_GET_CONFIGURATION_CNF
 *      }
 *
 *  \endcode
 *
 *  \sa
 *  AVDTP_SEP_CONF, AVDTP_EVENT_NTF_CB, BT_avdtp_set_configuration_req
 *
 *  \hideinitializer
 */
#ifdef BT_DRIVER_MODE

#define BT_avdtp_get_configuration_req(handle, data, datalen)    \
        avdtp_common_api_handler_set_2                           \
        ( (handle), (data), (datalen),                           \
          AVDTP_GET_CONFIGURATION_CNF, NULL )

#else  /* BT_DRIVER_MODE */

#define BT_avdtp_get_configuration_req(handle, data, datalen)    \
        avdtp_common_api_handler_set_2                           \
        ( (handle), (data), (datalen),                           \
          AVDTP_GET_CONFIGURATION_CNF, avdtp_common_req_bh_set_2 )

#endif /* BT_DRIVER_MODE */


/**
 *  \brief To reconfigure a Stream Context.
 *
 *  \par Synopsis:
 *  API_RESULT BT_avdtp_reconfigure_req
 *             (
 *                 IN AVDTP_HANDLE *    handle,
 *                 IN AVDTP_SEP_CONF *  sep_conf
 *             );
 *
 *  \par Description:
 *  This API enables applications/profiles to reconfigure a Stream Context.
 *  Only Application Service Capabilities can be reconfigured, and other
 *  parameters will be ignored by AVDTP if present.
 *
 *  \param [in] handle
 *         The AVDTP Handle, from which BD_ADDR, and the local SEID
 *         will be used to identify the Stream Context.
 *  \param [in] sep_conf
 *         This parameter must be a pointer to a caller allocated
 *         AVDTP_SEP_CONF data type, containing the specification of
 *         Application Service Capabilities to be reconfigured.
 *
 *  \return API_RESULT
 *  - API_SUCCESS, on successful initiation of the procedure.
 *  - Error Code, describing the reason for failure.
 *
 *  \remark
 *  Signalling Channel must be available for the specified remote AVDTP
 *  entity, before this API could be invoked.
 *
 *  \par
 *  The Stream Context must have been suspended using BT_avdtp_suspend_req,
 *  for this API to succeed.
 *
 *  \par
 *  Following parameters of the AVDTP Handle must be set appropriately,
 *  before invoking this API:
 *  - Bluetooth Device Address of the remote AVDTP entity
 *  - Local Stream End Point Identifier (SEID) of the Stream Context.
 *    This must have been obtained during a prior registration of a SEP.
 *  - Remote Stream End Point Identifier (SEID) of the Stream Context.
 *  - AVDTP_EVENT_NTF_CB, which AVDTP needs to invoke to inform
 *    result of the procedure.
 *
 *  \par
 *  AVDTP will confirm the result of the procedure by invoking the
 *  AVDTP_EVENT_NTF_CB, with following parameters:
 *  - event_type:    AVDTP_RECONFIGURE_CNF
 *  - event_result:  0x0000, if successful. Otherwise, it is an error code
 *                   describing the reason for failure.
 *  - avdtp_handle:  Set to AVDTP Handle provided by application during the
 *                   API call.
 *  - event_data:    NULL
 *  - event_datalen: 0 (Zero)
 *
 *  \note
 *  This API is implemented as a macro, but it should be treated as a
 *  function while invoking. An example is shown below:
 *
 *  \par
 *  \code
 *
 *      AVDTP_SET_HANDLE_BD_ADDR(handle, bd_addr);
 *      AVDTP_SET_HANDLE_LOCAL_SEID(handle, local_seid);
 *      AVDTP_SET_HANDLE_REMOTE_SEID(handle, remote_seid);
 *      AVDTP_SET_HANDLE_CALLBACK(handle, ntf_callback);
 *
 *      retval = BT_avdtp_reconfigure_req(&handle, &sep_conf);
 *      if (retval != API_SUCCESS)
 *      {
 *          Take recovery action ...
 *      }
 *      else
 *      {
 *          Reconfigure Procedure initiated ...
 *          Wait for Notification Callback with AVDTP_RECONFIGURE_CNF
 *      }
 *
 *  \endcode
 *
 *  \sa
 *  AVDTP_SEP_CONF, AVDTP_HANDLE, AVDTP_EVENT_NTF_CB, BT_avdtp_suspend_req
 *
 *  \hideinitializer
 */
#ifdef BT_DRIVER_MODE

#define BT_avdtp_reconfigure_req(handle, sep_conf)    \
        avdtp_common_api_handler_set_3                \
        ((handle), (sep_conf), AVDTP_RECONFIGURE_CNF, NULL)

#else  /* BT_DRIVER_MODE */

#define BT_avdtp_reconfigure_req(handle, sep_conf)    \
        avdtp_common_api_handler_set_3                \
        ((handle), (sep_conf),                        \
         AVDTP_RECONFIGURE_CNF, avdtp_common_req_bh_set_3)

#endif /* BT_DRIVER_MODE */


/**
 *  \brief To establish a Streaming connection.
 *
 *  \par Synopsis:
 *  API_RESULT BT_avdtp_open_req
 *             (
 *                 IN AVDTP_HANDLE *    handle
 *             );
 *
 *  \par Description:
 *  This API enables applications/profiles to establish a Stream, which
 *  has already been configured using the BT_avdtp_set_configuration_req API.
 *
 *  \param [in] handle
 *         The AVDTP Handle, from which BD_ADDR, and the local SEID
 *         will be used to identify the Stream Context.
 *
 *  \return API_RESULT
 *  - API_SUCCESS, on successful initiation of the procedure.
 *  - Error Code, describing the reason for failure.
 *
 *  \remark
 *  Signalling Channel must be available for the specified remote AVDTP
 *  entity, before this API could be invoked.
 *
 *  \par
 *  The Stream Context must have been configured using
 *  BT_avdtp_set_configuration_req for this API to succeed.
 *
 *  \par
 *  Following parameters of the AVDTP Handle must be set appropriately,
 *  before invoking this API:
 *  - Bluetooth Device Address of the remote AVDTP entity
 *  - Local Stream End Point Identifier (SEID) of the Stream Context.
 *    This must have been obtained during a prior registration of a SEP.
 *  - Remote Stream End Point Identifier (SEID) of the Stream Context.
 *  - AVDTP_EVENT_NTF_CB, which AVDTP needs to invoke to inform
 *    result of the procedure.
 *
 *  \par
 *  AVDTP will confirm the result of the procedure by invoking the
 *  AVDTP_EVENT_NTF_CB, with following parameters:
 *  - event_type:    AVDTP_OPEN_CNF
 *  - event_result:  0x0000, if successful. Otherwise, it is an error code
 *                   describing the reason for failure.
 *  - avdtp_handle:  Set to AVDTP Handle provided by application during the
 *                   API call.
 *  - event_data:    NULL
 *  - event_datalen: 0 (Zero)
 *
 *  \note
 *  This API is implemented as a macro, but it should be treated as a
 *  function while invoking. An example is shown below:
 *
 *  \par
 *  \code
 *
 *      AVDTP_SET_HANDLE_BD_ADDR(handle, bd_addr);
 *      AVDTP_SET_HANDLE_LOCAL_SEID(handle, local_seid);
 *      AVDTP_SET_HANDLE_REMOTE_SEID(handle, remote_seid);
 *      AVDTP_SET_HANDLE_CALLBACK(handle, ntf_callback);
 *
 *      retval = BT_avdtp_open_req(&handle);
 *      if (retval != API_SUCCESS)
 *      {
 *          Take recovery action ...
 *      }
 *      else
 *      {
 *          Open Procedure initiated ...
 *          Wait for Notification Callback with AVDTP_OPEN_CNF
 *      }
 *
 *  \endcode
 *
 *  \sa
 *  AVDTP_HANDLE, AVDTP_EVENT_NTF_CB, BT_avdtp_set_configuration_req
 *
 *  \hideinitializer
 */
#ifdef BT_DRIVER_MODE

#define BT_avdtp_open_req(handle)       \
        avdtp_common_api_handler_set_1 ((handle), AVDTP_OPEN_CNF, NULL)

#else  /* BT_DRIVER_MODE */

#define BT_avdtp_open_req(handle)       \
        avdtp_common_api_handler_set_1  \
        ((handle), AVDTP_OPEN_CNF, avdtp_common_req_bh_set_2)

#endif /* BT_DRIVER_MODE */


/**
 *  \brief To release a Streaming connection.
 *
 *  \par Synopsis:
 *  API_RESULT BT_avdtp_close_req
 *             (
 *                 IN AVDTP_HANDLE *    handle
 *             );
 *
 *  \par Description:
 *  This API enables applications/profiles to release a Stream,
 *  which has been established using the BT_avdtp_open_req API.
 *
 *  \param [in] handle
 *         The AVDTP Handle, from which BD_ADDR, and the local SEID
 *         will be used to identify the Stream Context.
 *
 *  \return API_RESULT
 *  - API_SUCCESS, on successful initiation of the procedure.
 *  - Error Code, describing the reason for failure.
 *
 *  \remark
 *  Signalling Channel must be available for the specified remote AVDTP
 *  entity, before this API could be invoked.
 *
 *  \par
 *  Following parameters of the AVDTP Handle must be set appropriately,
 *  before invoking this API:
 *  - Bluetooth Device Address of the remote AVDTP entity
 *  - Local Stream End Point Identifier (SEID) of the Stream Context.
 *    This must have been obtained during a prior registration of a SEP.
 *  - Remote Stream End Point Identifier (SEID) of the Stream Context.
 *  - AVDTP_EVENT_NTF_CB, which AVDTP needs to invoke to inform
 *    result of the procedure.
 *
 *  \par
 *  AVDTP will confirm the result of the procedure by invoking the
 *  AVDTP_EVENT_NTF_CB, with following parameters:
 *  - event_type:    AVDTP_CLOSE_CNF
 *  - event_result:  0x0000, if successful. Otherwise, it is an error code
 *                   describing the reason for failure.
 *  - avdtp_handle:  Set to AVDTP Handle provided by application during the
 *                   API call.
 *  - event_data:    NULL
 *  - event_datalen: 0 (Zero)
 *
 *  \note
 *  This API is implemented as a macro, but it should be treated as a
 *  function while invoking. An example is shown below:
 *
 *  \par
 *  \code
 *
 *      AVDTP_SET_HANDLE_BD_ADDR(handle, bd_addr);
 *      AVDTP_SET_HANDLE_LOCAL_SEID(handle, local_seid);
 *      AVDTP_SET_HANDLE_REMOTE_SEID(handle, remote_seid);
 *      AVDTP_SET_HANDLE_CALLBACK(handle, ntf_callback);
 *
 *      retval = BT_avdtp_close_req(&handle);
 *      if (retval != API_SUCCESS)
 *      {
 *          Take recovery action ...
 *      }
 *      else
 *      {
 *          Close Procedure initiated ...
 *          Wait for Notification Callback with AVDTP_CLOSE_CNF
 *      }
 *
 *  \endcode
 *
 *  \sa
 *  AVDTP_HANDLE, AVDTP_EVENT_NTF_CB, BT_avdtp_open_req
 *
 *  \hideinitializer
 */
#ifdef BT_DRIVER_MODE

#define BT_avdtp_close_req(handle)      \
        avdtp_common_api_handler_set_1 ((handle), AVDTP_CLOSE_CNF, NULL)

#else  /* BT_DRIVER_MODE */

#define BT_avdtp_close_req(handle)      \
        avdtp_common_api_handler_set_1  \
        ((handle), AVDTP_CLOSE_CNF, avdtp_common_req_bh_set_2)

#endif /* BT_DRIVER_MODE */


/**
 *  \brief To start streaming for a Stream Context.
 *
 *  \par Synopsis:
 *  API_RESULT BT_avdtp_start_req
 *             (
 *                 IN AVDTP_HANDLE *    handle
 *             );
 *
 *  \par Description:
 *  This API enables applications/profiles to start streaming on a
 *  Stream Context, which has been established by AVDTP Stream Establishment
 *  procedure (eg, BT_avdtp_open_req).
 *
 *  \param [in] handle
 *         The AVDTP Handle, from which BD_ADDR, and the local SEID
 *         will be used to identify the Stream Context.
 *
 *  \return API_RESULT
 *  - API_SUCCESS, on successful initiation of the procedure.
 *  - Error Code, describing the reason for failure.
 *
 *  \remark
 *  Following parameters of the AVDTP Handle must be set appropriately,
 *  before invoking this API:
 *  - Bluetooth Device Address of the remote AVDTP entity
 *  - Local Stream End Point Identifier (SEID) of the Stream Context.
 *    This must have been obtained during a prior registration of a SEP.
 *  - Remote Stream End Point Identifier (SEID) of the Stream Context.
 *  - AVDTP_EVENT_NTF_CB, which AVDTP needs to invoke to inform
 *    result of the procedure.
 *
 *  \par
 *  AVDTP will confirm the result of the procedure by invoking the
 *  AVDTP_EVENT_NTF_CB. OutMTU of the Media Transport channel is
 *  passed as event_data. The callback is called with following parameters:
 *  - event_type:    AVDTP_START_CNF
 *  - event_result:  0x0000, if successful. Otherwise, it is an error code
 *                   describing the reason for failure.
 *  - avdtp_handle:  Set to AVDTP Handle provided by application during the
 *                   API call.
 *  - event_data:    This parameter will be a pointer to a UCHAR buffer
 *                   containing OutMTU value of Media Transport channel.
 *  - event_datalen: This parameter specifies the size of event_data above.
 *
 *  \note
 *  This API is implemented as a macro, but it should be treated as a
 *  function while invoking. An example is shown below:
 *
 *  \par
 *  \code
 *
 *      AVDTP_SET_HANDLE_BD_ADDR(handle, bd_addr);
 *      AVDTP_SET_HANDLE_LOCAL_SEID(handle, local_seid);
 *      AVDTP_SET_HANDLE_REMOTE_SEID(handle, remote_seid);
 *      AVDTP_SET_HANDLE_CALLBACK(handle, ntf_callback);
 *
 *      retval = BT_avdtp_start_req(&handle);
 *      if (retval != API_SUCCESS)
 *      {
 *          Take recovery action ...
 *      }
 *      else
 *      {
 *          Start Procedure initiated ...
 *          Wait for Notification Callback with AVDTP_START_CNF
 *      }
 *
 *  \endcode
 *
 *  \sa
 *  AVDTP_HANDLE, AVDTP_EVENT_NTF_CB, BT_avdtp_open_req
 *
 *  \hideinitializer
 */
#ifdef BT_DRIVER_MODE

#define BT_avdtp_start_req(handle)      \
        avdtp_common_api_handler_set_1  ((handle), AVDTP_START_CNF, NULL)

#else  /* BT_DRIVER_MODE */

#define BT_avdtp_start_req(handle)      \
        avdtp_common_api_handler_set_1  \
        ((handle), AVDTP_START_CNF, avdtp_common_req_bh_set_2)

#endif /* BT_DRIVER_MODE */


/**
 *  \brief To suspend streaming for a Stream Context.
 *
 *  \par Synopsis:
 *  API_RESULT BT_avdtp_suspend_req
 *             (
 *                 IN AVDTP_HANDLE *    handle
 *             );
 *
 *  \par Description:
 *  This API enables applications/profiles to suspend streaming on a
 *  Stream Context, for which streaming should have been started using
 *  the AVDTP Start Stream procedure (eg, BT_avdtp_start_req).
 *
 *  \param [in] handle
 *         The AVDTP Handle, from which BD_ADDR, and the local SEID
 *         will be used to identify the Stream Context.
 *
 *  \return API_RESULT
 *  - API_SUCCESS, on successful initiation of the procedure.
 *  - Error Code, describing the reason for failure.
 *
 *  \remark
 *  Following parameters of the AVDTP Handle must be set appropriately,
 *  before invoking this API:
 *  - Bluetooth Device Address of the remote AVDTP entity
 *  - Local Stream End Point Identifier (SEID) of the Stream Context.
 *    This must have been obtained during a prior registration of a SEP.
 *  - Remote Stream End Point Identifier (SEID) of the Stream Context.
 *  - AVDTP_EVENT_NTF_CB, which AVDTP needs to invoke to inform
 *    result of the procedure.
 *
 *  \par
 *  AVDTP will confirm the result of the procedure by invoking the
 *  AVDTP_EVENT_NTF_CB, with following parameters:
 *  - event_type:    AVDTP_SUSPEND_CNF
 *  - event_result:  0x0000, if successful. Otherwise, it is an error code
 *                   describing the reason for failure.
 *  - avdtp_handle:  Set to AVDTP Handle provided by application during the
 *                   API call.
 *  - event_data:    NULL
 *  - event_datalen: 0 (Zero)
 *
 *  \note
 *  This API is implemented as a macro, but it should be treated as a
 *  function while invoking. An example is shown below:
 *
 *  \par
 *  \code
 *
 *      AVDTP_SET_HANDLE_BD_ADDR(handle, bd_addr);
 *      AVDTP_SET_HANDLE_LOCAL_SEID(handle, local_seid);
 *      AVDTP_SET_HANDLE_REMOTE_SEID(handle, remote_seid);
 *      AVDTP_SET_HANDLE_CALLBACK(handle, ntf_callback);
 *
 *      retval = BT_avdtp_suspend_req(&handle);
 *      if (retval != API_SUCCESS)
 *      {
 *          Take recovery action ...
 *      }
 *      else
 *      {
 *          Suspend Procedure initiated ...
 *          Wait for Notification Callback with AVDTP_SUSPEND_CNF
 *      }
 *
 *  \endcode
 *
 *  \sa
 *  AVDTP_HANDLE, AVDTP_EVENT_NTF_CB, BT_avdtp_start_req
 *
 *  \hideinitializer
 */
#ifdef BT_DRIVER_MODE

#define BT_avdtp_suspend_req(handle)    \
        avdtp_common_api_handler_set_1  ((handle), AVDTP_SUSPEND_CNF, NULL)

#else  /* BT_DRIVER_MODE */

#define BT_avdtp_suspend_req(handle)    \
        avdtp_common_api_handler_set_1  \
        ((handle), AVDTP_SUSPEND_CNF, avdtp_common_req_bh_set_2)

#endif /* BT_DRIVER_MODE */


/**
 *  \brief To abort & disconnect a Stream Context.
 *
 *  \par Synopsis:
 *  API_RESULT BT_avdtp_abort_req
 *             (
 *                 IN AVDTP_HANDLE *    handle
 *             );
 *
 *  \par Description:
 *  This API enables applications/profiles to abort a Stream Context at
 *  any point of time. This should be used as the last resort to sync up
 *  AVDTP Stream Context in case of protocol misbehavior.
 *
 *  \param [in] handle
 *         The AVDTP Handle, from which BD_ADDR, and the local SEID
 *         will be used to identify the Stream Context.
 *
 *  \return API_RESULT
 *  - API_SUCCESS, on successful initiation of the procedure.
 *  - Error Code, describing the reason for failure.
 *
 *  \remark
 *  Following parameters of the AVDTP Handle must be set appropriately,
 *  before invoking this API:
 *  - Bluetooth Device Address of the remote AVDTP entity
 *  - Local Stream End Point Identifier (SEID) of the Stream Context.
 *    This must have been obtained during a prior registration of a SEP.
 *  - Remote Stream End Point Identifier (SEID) of the Stream Context.
 *  - AVDTP_EVENT_NTF_CB, which AVDTP needs to invoke to inform
 *    result of the procedure.
 *
 *  \par
 *  AVDTP will confirm the result of the procedure by invoking the
 *  AVDTP_EVENT_NTF_CB, with following parameters:
 *  - event_type:    AVDTP_ABORT_CNF
 *  - event_result:  0x0000, if successful. Otherwise, it is an error code
 *                   describing the reason for failure.
 *  - avdtp_handle:  Set to AVDTP Handle provided by application during the
 *                   API call.
 *  - event_data:    NULL
 *  - event_datalen: 0 (Zero)
 *
 *  \note
 *  This API is implemented as a macro, but it should be treated as a
 *  function while invoking. An example is shown below:
 *
 *  \par
 *  \code
 *
 *      AVDTP_SET_HANDLE_BD_ADDR(handle, bd_addr);
 *      AVDTP_SET_HANDLE_LOCAL_SEID(handle, local_seid);
 *      AVDTP_SET_HANDLE_REMOTE_SEID(handle, remote_seid);
 *      AVDTP_SET_HANDLE_CALLBACK(handle, ntf_callback);
 *
 *      retval = BT_avdtp_abort_req(&handle);
 *      if (retval != API_SUCCESS)
 *      {
 *          Take recovery action ...
 *      }
 *      else
 *      {
 *          Abort Procedure initiated ...
 *          Wait for Notification Callback with AVDTP_ABORT_CNF
 *      }
 *
 *  \endcode
 *
 *  \sa
 *  AVDTP_HANDLE, AVDTP_EVENT_NTF_CB
 *
 *  \hideinitializer
 */
#ifdef BT_DRIVER_MODE

#define BT_avdtp_abort_req(handle)      \
        avdtp_common_api_handler_set_1 ((handle), AVDTP_ABORT_CNF, NULL)

#else  /* BT_DRIVER_MODE */

#define BT_avdtp_abort_req(handle)      \
        avdtp_common_api_handler_set_1  \
        ((handle), AVDTP_ABORT_CNF, avdtp_common_req_bh_set_2)

#endif /* BT_DRIVER_MODE */


/**
 *  \brief To request transfer of media data.
 *
 *  \par Description:
 *  This API enables applications/profiles to write media data over a
 *  stream.
 *
 *  \param [in] handle
 *         The AVDTP Handle, from which BD_ADDR, and the local SEID
 *         will be used to identify the Stream Context.
 *  \param [in] packet
 *         The Media Packet to transfer over the Streaming Connection.
 *  \param [in] packet_len
 *         Size of the Media Packet.
 *
 *  \return API_RESULT
 *  - API_SUCCESS, on successful initiation of the procedure.
 *  - Error Code, describing the reason for failure.
 *
 *  \remark
 *  The application should provide a complete Media Packet for transmission,
 *  which includes the following:
 *  - Space for L2CAP Header - 4 Bytes. This will be filled by L2CAP module.
 *  - Space for Adaptation Layer Header - 1 Byte. This should be accounted
 *    only if Multiplexing of various transport sessions are configured for
 *    the Stream Context. This will be filled by AVDTP module
 *  - Media Packet Header - 12 Bytes. Except Sequence number, all other fields
 *    should be filled by the module calling this API.
 *  - Media Packet Payload.
 *
 *  \par
 *  It should also be noted that the size of the entire media packet, as
 *  described above, minus L2CAP's Header of 4 bytes, should never be more
 *  than the OutMTU configured for the Media Transport Channel in AVDTP.
 *
 *  \par
 *  The memory for Media Packet should always be dynamically allocated,
 *  and it will be freed by the EtherMind Stack, upon successful transmission.
 *
 *  \par
 *  Following parameters of the AVDTP Handle must be set appropriately,
 *  before invoking this API:
 *  - Bluetooth Device Address of the remote AVDTP entity
 *  - Local Stream End Point Identifier (SEID) of the Stream Context.
 *    This must have been obtained during a prior registration of a SEP.
 *  - Remote Stream End Point Identifier (SEID) of the Stream Context.
 *
 *  \sa
 *  AVDTP_HANDLE
 */

API_RESULT BT_avdtp_media_write
           (
               /* IN */ AVDTP_HANDLE *    handle,
               /* IN */ UCHAR *           packet,
               /* IN */ UINT16            packet_len
           );

#ifdef AVDTP_ASSISTIVE_MODE
/**
 *  \brief Get the Media Channel Parameters for the given AVDTP handle
 *
 *  \Description
 *         This API is used by the application to fetch the underlying media
 *         transport channel parameters. This is required by the application
 *         to make use of Assistive A2DP or similar feature, to configure
 *         the Bluetooth controller with the L2CAP CID and MTU values of
 *         the media channel, to enable decoding and rendering the media
 *         by the Bluetooth controller.
 *
 *  \param [in] handle
 *         AVDTP Handle pointer
 *
 *  \param [out] params
 *         Pointer to the AVDTP Media Channel Parameter structure
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_avdtp_get_media_channel_params
           (
               /* IN */  AVDTP_HANDLE               * handle,
               /* OUT */ AVDTP_MEDIA_CHANNEL_PARAMS * params
           );
#endif /* AVDTP_ASSISTIVE_MODE */

/*@}*/


/**
 *  @name AVDTP APIs - Utility Functions
 */

/*@{*/

/**
 *  \brief To decode Stream End Point information.
 *
 *  \par Description:
 *  This API enables applications/profiles to decode Stream End Point
 *  information as contained in the AVDTP packet segment, received as a
 *  result of BT_avdtp_discover_req API.
 *
 *  \param [in] buffer
 *         The buffer which contains the AVDTP packet segment containing
 *         Stream End Point information.
 *  \param [in] buffer_len
 *         Size of the buffer.
 *  \param [out] sep_info
 *         This must be a pointer to a caller allocated AVDTP_SEP_INFO
 *         data type, onto which decoded information will be stored.
 *
 *  \return API_RESULT
 *  - API_SUCCESS, on successful initiation of the procedure.
 *  - Error Code, describing the reason for failure.
 *
 *  \sa
 *  AVDTP_SEP_INFO, BT_avdtp_discover_req
 */

API_RESULT BT_avdtp_decode_sep_information
           (
               /* IN */  UCHAR *             buffer,
               /* IN */  UINT16              buffer_len,
               /* OUT */ AVDTP_SEP_INFO *    sep_info
           );


/**
 *  \brief To decode Stream End Point Capabilities.
 *
 *  \par Description:
 *  This API enables applications/profiles to decode Stream End Point
 *  Application Service Capabilities (Codec & Content Protection Capabilities)
 *  information as contained in the AVDTP packet segment, received as a
 *  result of BT_avdtp_get_capabilities API.
 *
 *  \param [in] buffer
 *         The buffer which contains the AVDTP packet segment containing
 *         Stream End Point Capabilities information.
 *  \param [in] buffer_len
 *         Size of the buffer.
 *  \param [out] sep_cap
 *         This must be a pointer to a caller allocated AVDTP_SEP_CAP
 *         data type, onto which decoded information will be stored.
 *
 *  \return API_RESULT: API_SUCCESS, or an Error Code describing the reason
 *                      for failure.
 *
 *  \sa
 *  AVDTP_SEP_CAP, BT_avdtp_get_capabilities_req
 *
 *  \remarks
 *  Application shall allocate space for the sep_cap structure
 *  before calling this API.
 *  The API returns the information for the first instance of
 *  Media Codec Capability Information found in the buffer.
 *  If no Media Codec Capabality Information is found, the Codec
 *  IE Buffer is set to NULL.
 */

API_RESULT BT_avdtp_decode_sep_capabilities
           (
               /* IN */  UCHAR *             buffer,
               /* IN */  UINT16              buffer_len,
               /* OUT */ AVDTP_SEP_CAP *     sep_cap
           );


/**
 *  \brief To decode Stream End Point Configuration.
 *
 *  \par Synopsis:
 *  API_RESULT BT_avdtp_decode_sep_configuration
 *             (
 *                 IN  UCHAR *             buffer,
 *                 IN  UINT16              buffer_len,
 *                 OUT AVDTP_SEP_CONF *    sep_conf
 *             );
 *
 *  \par Description:
 *  This API enables applications/profiles to decode Stream End Point
 *  Configuration information as contained in the AVDTP packet segment,
 *  received as a result of BT_avdtp_get_configuration_req API, or, with
 *  AVDTP_EVENT_NTF_CB for AVDTP_SET_CONFIGURATION_IND.
 *
 *  \param [in] buffer
 *         The buffer which contains the AVDTP packet segment containing
 *         Stream End Point Configuration information.
 *  \param [in] buffer_len
 *         Size of the buffer.
 *  \param [out] sep_conf
 *         This must be a pointer to a caller allocated AVDTP_SEP_CONF
 *         data type, onto which decoded information will be stored.
 *
 *  \return API_RESULT
 *  - API_SUCCESS, on successful initiation of the procedure.
 *  - Error Code, describing the reason for failure.
 *
 *  \sa
 *  AVDTP_SEP_CONF, BT_avdtp_get_configuration_req
 *
 *  \hideinitializer
 */

#define BT_avdtp_decode_sep_configuration(buffer, buffer_len, sep_conf) \
        avdtp_decode_sep_configuration                                  \
        (0x00U, (buffer), (buffer_len), (sep_conf))

/**
 *  \brief To set In MTU for Media transport channel.
 *
 *  \par Description:
 *  This API enables applications/profiles to set the In MTU values for
 *  media transport channel. This value will be used during transport
 *  channel establishment as part of AVDTP Open procedure.
 *
 *  \param [in] handle
 *         The AVDTP handle, from which the 'local_seid' will be used
 *         to identify the local SEP.
 *  \param [in] in_mtu
 *         value of In MTU.
 *
 *  \return API_RESULT
 *  - API_SUCCESS, on successful initiation of the procedure.
 *  - Error Code, describing the reason for failure.
 *
 *  \remarks
 *  The SEPs which are Source in nature do not need to specify a L2CAP
 *  InMTU in excess of 48 octets.
 *
 *  \par
 *  Following parameters of the AVDTP Handle must be set appropriately,
 *  before invoking this API:
 *  - Local Stream End Point Identifier (SEID) of the Stream Context.
 *
 */
API_RESULT BT_avdtp_set_media_mtu
           (
               /* IN */  AVDTP_HANDLE *  handle,
               /* IN */  UINT16          in_mtu
           );

#ifdef HAVE_AVDTP_TEST_MODE
/**
 *  \brief To update reject response parameters for a valid AVDTP SIG ID message.
 *
 *  \par Description:
 *  This function is used by the application to enable reject response and to
 *  set reject response parameters for the AVDTP SIG message. It updates
 *  error code to be sent for the given AVDTP SIG ID.
 *
 *  \param [in] avdtp_sig_id
 *         AVDTP singalling message id
 *  \param [in] avdtp_error_code
 *         Error code for the given signalling id.
 *  \param [in] flag
 *         Flag to enable reject response
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT  BT_avdtp_set_sig_msg_reject_rsp_params
            (
                /* IN */ UCHAR    avdtp_sig_id,
                /* IN */ UCHAR    avdtp_error_code,
                /* IN */ UCHAR    flag
            );

#endif /* HAVE_AVDTP_TEST_MODE */

#ifdef HAVE_AVDTP_MEDIA_PKT_FLUSHABLE
/**
 *  \brief To Set Packets as Flushable for AVDTP Media Transport Connection.
 *
 *  \par Description:
 *  This API enables the application to set all AVDTP Media Transport packets
 *  as flushable L2CAP packets.
 *
 *  \param [in] handle
 *         The AVDTP Handle corresponding to the connection
 *         on which to enable flushable packets.
 *
 *  \return API_RESULT
 *          API_SUCCESS, or, an Error Code from BT_error.h
 *
 *  \note
 */
API_RESULT BT_avdtp_set_flushable
           (
               /* IN */  AVDTP_HANDLE *  handle
           );

/*@}*/

#endif /* HAVE_AVDTP_MEDIA_PKT_FLUSHABLE */

#ifdef HAVE_AVDTP_SET_GET_SIG_CH_OUTMTU
/**
 *  \brief To set/get Out MTU of AVDTP signalling channel.
 *
 *  \par Description:
 *  This API enables the upper layer to get or set the Out MTU size of AVDTP signaling channel.
 *
 *  \param [in] handle
 *         The AVDTP Handle corresponding to the connection.
 *
 *  \param [in] flag
 *         Flag to indicate out mtu to get or set.
 *         0 -> Get out mtu.
 *         1 -> Set out mtu
 *
 *  \param [in,out] out_mtu
 *         Pointer to Out MTU value of the AVDTP signalling channel.
 *
 *  \return API_RESULT
 *          API_SUCCESS, or, an Error Code from BT_error.h
 *
 *  \note
 */
API_RESULT BT_avdtp_access_sig_channel_out_mtu
           (
               /* IN */    AVDTP_HANDLE    * handle,
               /* IN */    UCHAR             flag,
               /* INOUT */ UINT16          * out_mtu
           );

/**
 *  \brief To get Out MTU of AVDTP signalling channel.
 *
 *  \par Description:
 *  This API enables the upper layer to get the Out MTU size of AVDTP signalling channel.
 *
 *  \param [in] handle
 *         The AVDTP Handle corresponding to the connection.
 *
 *  \param [out] mtu
 *         Pointer to Out MTU value of the AVDTP signalling channel.
 *
 *  \return API_RESULT
 *          API_SUCCESS, or, an Error Code from BT_error.h
 *
 *  \note
 */
#define BT_avdtp_get_sig_channel_out_mtu(h, mtu)  \
        BT_avdtp_access_sig_channel_out_mtu((h), 0x00, (mtu))

/**
 *  \brief To set Out MTU of AVDTP signalling channel.
 *
 *  \par Description:
 *  This API enables the upper layer to set the Out MTU size of AVDTP signalling channel.
 *  In normal scenario, upper layer need not to change the out mtu.
 *  Typically this is used to set the outmtu to a smaller value to enable sending
 *  AVDTP signalling messages in fragments, required for some of the qualification
 *  test cases
 *
 *  \param [in] handle
 *         The AVDTP Handle corresponding to the connection.
 *
 *  \param [in] mtu
 *         Pointer to Out MTU vallue of the AVDTP signalling channel.
 *
 *  \return API_RESULT
 *          API_SUCCESS, or, an Error Code from BT_error.h
 *
 *  \note
 *   It is application responsibility to set back to original/initial outmtu value after changing.
 */
#define BT_avdtp_set_sig_channel_out_mtu(h, mtu)  \
        BT_avdtp_access_sig_channel_out_mtu((h), 0x01, (mtu))

#endif /* HAVE_AVDTP_SET_GET_SIG_CH_OUTMTU */
#ifdef __cplusplus
};
#endif
/** \} */
/** \} */
/** \} */
/** \} */
#endif /* _H_BT_AVDTP_API_ */

