
/**
 *  \file BT_a2dp_api.h
 *
 *  \brief EtherMind A2DP API Header File.
 *
 *  This Header File contains declaration of A2DP APIs, Macros and
 *  Constant Definitions for use in applications.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_A2DP_API_
#define _H_BT_A2DP_API_

/* ----------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "BT_avdtp_api.h"

/* ----------------------------------------- Global Definitions */
/**
 * \addtogroup bt_profiles Profiles
 * \{
 */
/**
 * \defgroup  a2dp_module  A2DP (Advanced Audio Distribution Profile)
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  Advanced Audio Distribution Profile (A2DP) module to the Application and
 *  other upper layers of the stack.
 */
/**
 * \defgroup  a2dp_defines Defines
 * \{
 * Describes defines for A2DP module.
 */
/**
 * \defgroup a2dp_constants Constants
 * \{
 * Describes Constants defined by A2DP module.
 */
/**
 *  \addtogroup a2dp_constants
 *
 *  \name A2DP Event Notification Callback Event Types
 *  Constant Definitions for A2DP Notification Callback Event Types.
 */

/*@{*/

#define A2DP_CONNECT_CNF                             0x01U
#define A2DP_DISCONNECT_CNF                          0x02U
#define A2DP_START_CNF                               0x03U
#define A2DP_SUSPEND_CNF                             0x04U
#define A2DP_RECONFIGURE_CNF                         0x05U
#define A2DP_CONNECT_IND                             0x06U
#define A2DP_DISCONNECT_IND                          0x07U
#define A2DP_START_IND                               0x08U
#define A2DP_CONFIGURE_IND                           0x09U
#define A2DP_SUSPEND_IND                             0x0AU
#define A2DP_MEDIA_FRAME_IND                         0x0BU
#define A2DP_DELAY_REPORT_IND                        0x0CU
#define A2DP_DELAY_REPORT_CNF                        0x0DU

/*@}*/


/**
 *  \addtogroup a2dp_constants
 *
 *  @name A2DP SBC Codec Sampling Frequency
 *  Constant Definitions for A2DP SBC Codec Sampling Frequency values.
 */
/*@{*/

#define A2DP_SBC_SAMPLING_FREQUENCY_16000            0x80U
#define A2DP_SBC_SAMPLING_FREQUENCY_32000            0x40U
#define A2DP_SBC_SAMPLING_FREQUENCY_44100            0x20U
#define A2DP_SBC_SAMPLING_FREQUENCY_48000            0x10U

/*@}*/


/**
 *  \addtogroup a2dp_constants
 *
 *  @name A2DP SBC Codec Channel Mode
 *  Constant Definitions for types of A2DP SBC Codec Channel Mode.
 */
/*@{*/

#define A2DP_SBC_CHANNEL_MONO                        0x08U
#define A2DP_SBC_CHANNEL_DUAL                        0x04U
#define A2DP_SBC_CHANNEL_STEREO                      0x02U
#define A2DP_SBC_CHANNEL_JOINT_STEREO                0x01U

/*@}*/


/**
 *  \addtogroup a2dp_constants
 *
 *  @name A2DP SBC Codec Block Length
 *  Constant Definitions for length of A2DP SBC Block.
 */
/*@{*/

#define A2DP_SBC_BLOCK_LENGTH_4                      0x80U
#define A2DP_SBC_BLOCK_LENGTH_8                      0x40U
#define A2DP_SBC_BLOCK_LENGTH_12                     0x20U
#define A2DP_SBC_BLOCK_LENGTH_16                     0x10U

/*@}*/


/**
 *  \addtogroup a2dp_constants
 *
 *  @name A2DP SBC Codec Subbands
 *  Constant Definitions for number of A2DP SBC Codec Subbands.
 */
/*@{*/

#define A2DP_SBC_SUBBANDS_4                          0x08U
#define A2DP_SBC_SUBBANDS_8                          0x04U

/*@}*/

/**
 *  \addtogroup a2dp_constants
 *
 *  @name A2DP SBC Allocation Method
 *  Constant Definitions for type of A2DP SBC Bit Allocation Method.
 */
/*@{*/

#define A2DP_SBC_ALLOCATION_METHOD_SNR               0x02U
#define A2DP_SBC_ALLOCATION_METHOD_LOUDNESS          0x01U

/*@}*/


/**
 *  \addtogroup a2dp_constants
 *
 *  @name A2DP Maximum and Minimum Bitpool
 *  Constant Definitions for A2DP SBC Minimum and Maximum Bitpool values.
 */
/*@{*/

#define A2DP_SBC_BITPOOL_MIN                         2U
#define A2DP_SBC_BITPOOL_MAX                         250U

/*@}*/

/**
 *  \addtogroup a2dp_constants
 *
 *  @name A2DP SBC CODEC Capability Information Element (IE) Length
 *  Constant Definitions for A2DP SBC CODEC Capability IE Length.
 */
/*@{*/

#define A2DP_SBC_CODEC_CAP_IE_LEN                    4U

/*@}*/

/**
 *  \addtogroup a2dp_constants
 *
 *  @name A2DP MPEG-1,2 CODEC Capability Information Element (IE) Length
 *  Constant Definitions for A2DP MPEG-1, 2 CODEC Capability IE Length.
 */
/*@{*/

#define A2DP_MPEG_1_2_CODEC_CAP_IE_LEN               4U

/*@}*/

/**
 *  \addtogroup a2dp_constants
 *
 *  @name A2DP MPEG-2,4 CODEC Capability Information Element (IE) Length
 *  Constant Definitions for A2DP MPEG-2, 4 CODEC Capability IE Length.
 */
/*@{*/

#define A2DP_MPEG_2_4_CODEC_CAP_IE_LEN               6U

/*@}*/

/**
 *  \addtogroup a2dp_constants
 *
 *  @name A2DP MPEG-1,2 Codec Sampling Frequency
 *  Constant Definitions for A2DP MPEG-1,2 Sampling Frequency values.
 */
/*@{*/

#define A2DP_MPEG_1_2_SAMPLING_FREQUENCY_16000       0x20U
#define A2DP_MPEG_1_2_SAMPLING_FREQUENCY_22050       0x10U
#define A2DP_MPEG_1_2_SAMPLING_FREQUENCY_24000       0x08U
#define A2DP_MPEG_1_2_SAMPLING_FREQUENCY_32000       0x04U
#define A2DP_MPEG_1_2_SAMPLING_FREQUENCY_44100       0x02U
#define A2DP_MPEG_1_2_SAMPLING_FREQUENCY_48000       0x01U


/*@}*/

/**
 *  \addtogroup a2dp_constants
 *
 *  @name A2DP MPEG-1,2 Codec Channel Mode
 *  Constant Definitions for types of A2DP MPEG-1,2 Codec Channel Mode.
 */
/*@{*/

#define A2DP_MPEG_1_2_CHANNEL_MONO                   0x08U
#define A2DP_MPEG_1_2_CHANNEL_DUAL                   0x04U
#define A2DP_MPEG_1_2_CHANNEL_STEREO                 0x02U
#define A2DP_MPEG_1_2_CHANNEL_JOINT_STEREO           0x01U

/*@}*/

/**
 *  \addtogroup a2dp_constants
 *
 *  @name A2DP MPEG-1,2 Codec Layers
 *  Constant Definitions for types of A2DP MPEG-1,2 Codec Layers.
 */
/*@{*/

#define A2DP_MPEG_1_2_LAYER_I                        0x80U
#define A2DP_MPEG_1_2_LAYER_II                       0x40U
#define A2DP_MPEG_1_2_LAYER_III                      0x20U

/*@}*/


/**
 *  \addtogroup a2dp_constants
 *
 *  @name A2DP MPEG-1,2 Codec CRC Protection
 *  Constant Definitions for CRC Protection of A2DP MPEG-1,2 Codec.
 */
/*@{*/

#define A2DP_MPEG_1_2_CRC_OFF                         0x00U
#define A2DP_MPEG_1_2_CRC_ON                          0x10U

/*@}*/


/**
 *  \addtogroup a2dp_constants
 *
 *  @name A2DP MPEG-1,2 Codec Media Payload Format
 *  Constant Definitions for MRF of A2DP MPEG-1,2 Codec.
 */
/*@{*/

#define A2DP_MPEG_1_2_MPF_1                          0x00U
#define A2DP_MPEG_1_2_MPF_2                          0x40U

/*@}*/


/**
 *  \addtogroup a2dp_constants
 *
 *  @name A2DP MPEG-1,2 Codec Variable Bit Rate
 *  Constant Definitions for VBR of A2DP MPEG-1,2 Codec.
 */
/*@{*/

#define A2DP_MPEG_1_2_VBR_ON                         0x80U
#define A2DP_MPEG_1_2_VBR_OFF                        0x00U

/*@}*/

/**
 *  \addtogroup a2dp_constants
 *
 *  @name A2DP MPEG-1,2 Codec Samples Per Frame Size
 *  Constant Definitions for Samples Per Frame of A2DP MPEG-1,2 Codec.
 */
/*@{*/

#define A2DP_MPEG_1_2_SAMPLES_PER_FRAME_LSF          576U
#define A2DP_MPEG_1_2_SAMPLES_PER_FRAME              1152U

/*@}*/


/**
 *  \addtogroup a2dp_constants
 *
 *  @name A2DP MPEG-1,2 Bit Rate
 *  Constant Definitions for Bit Rate Values A2DP MPEG-1,2 Codec Layer III.
 */
/*@{*/

#define A2DP_MPEG_1_2_LAYER_III_BITRATE_32           0x0002U
#define A2DP_MPEG_1_2_LAYER_III_BITRATE_40           0x0004U
#define A2DP_MPEG_1_2_LAYER_III_BITRATE_48           0x0008U
#define A2DP_MPEG_1_2_LAYER_III_BITRATE_56           0x0010U
#define A2DP_MPEG_1_2_LAYER_III_BITRATE_64           0x0020U
#define A2DP_MPEG_1_2_LAYER_III_BITRATE_80           0x0040U
#define A2DP_MPEG_1_2_LAYER_III_BITRATE_96           0x0080U
#define A2DP_MPEG_1_2_LAYER_III_BITRATE_112          0x0100U
#define A2DP_MPEG_1_2_LAYER_III_BITRATE_128          0x0200U
#define A2DP_MPEG_1_2_LAYER_III_BITRATE_160          0x0400U
#define A2DP_MPEG_1_2_LAYER_III_BITRATE_192          0x0800U
#define A2DP_MPEG_1_2_LAYER_III_BITRATE_224          0x1000U
#define A2DP_MPEG_1_2_LAYER_III_BITRATE_256          0x2000U
#define A2DP_MPEG_1_2_LAYER_III_BITRATE_320          0x4000U

#define A2DP_MPEG_1_2_BITRATE_INDEX_0                0x0001U
#define A2DP_MPEG_1_2_BITRATE_INDEX_1                0x0002U
#define A2DP_MPEG_1_2_BITRATE_INDEX_2                0x0004U
#define A2DP_MPEG_1_2_BITRATE_INDEX_3                0x0008U
#define A2DP_MPEG_1_2_BITRATE_INDEX_4                0x0010U
#define A2DP_MPEG_1_2_BITRATE_INDEX_5                0x0020U
#define A2DP_MPEG_1_2_BITRATE_INDEX_6                0x0040U
#define A2DP_MPEG_1_2_BITRATE_INDEX_7                0x0080U
#define A2DP_MPEG_1_2_BITRATE_INDEX_8                0x0100U
#define A2DP_MPEG_1_2_BITRATE_INDEX_9                0x0200U
#define A2DP_MPEG_1_2_BITRATE_INDEX_10               0x0400U
#define A2DP_MPEG_1_2_BITRATE_INDEX_11               0x0800U
#define A2DP_MPEG_1_2_BITRATE_INDEX_12               0x1000U
#define A2DP_MPEG_1_2_BITRATE_INDEX_13               0x2000U
#define A2DP_MPEG_1_2_BITRATE_INDEX_14               0x4000U

/*@}*/

/**
 *  \addtogroup a2dp_constants
 *
 *  @name A2DP MPEG-2,4 Codec Object Type
 *  Constant Definitions for Object Type of A2DP MPEG-2,4 Codec.
 */
/*@{*/

#define A2DP_MPEG_2_4_OBJ_TYPE_MPEG2_AAC_LC          0x80U
#define A2DP_MPEG_2_4_OBJ_TYPE_MPEG4_AAC_LC          0x40U
#define A2DP_MPEG_2_4_OBJ_TYPE_MPEG4_AAC_LTP         0x20U
#define A2DP_MPEG_2_4_OBJ_TYPE_MPEG4_AAC_SCALABLE    0x10U
/*@}*/

/**
 *  \addtogroup a2dp_constants
 *
 *  @name A2DP MPEG-2,4 Codec Sampling Frequency
 *  Constant Definitions for A2DP MPEG-2,4 Sampling Frequency values.
 */
/*@{*/
#define A2DP_MPEG_2_4_SAMPLING_FREQUENCY_8000        0x8000U
#define A2DP_MPEG_2_4_SAMPLING_FREQUENCY_11025       0x4000U
#define A2DP_MPEG_2_4_SAMPLING_FREQUENCY_12000       0x2000U
#define A2DP_MPEG_2_4_SAMPLING_FREQUENCY_16000       0x1000U
#define A2DP_MPEG_2_4_SAMPLING_FREQUENCY_22050       0x0800U
#define A2DP_MPEG_2_4_SAMPLING_FREQUENCY_24000       0x0400U
#define A2DP_MPEG_2_4_SAMPLING_FREQUENCY_32000       0x0200U
#define A2DP_MPEG_2_4_SAMPLING_FREQUENCY_44100       0x0100U
#define A2DP_MPEG_2_4_SAMPLING_FREQUENCY_48000       0x0080U
#define A2DP_MPEG_2_4_SAMPLING_FREQUENCY_64000       0x0040U
#define A2DP_MPEG_2_4_SAMPLING_FREQUENCY_88200       0x0020U
#define A2DP_MPEG_2_4_SAMPLING_FREQUENCY_96000       0x0010U

/*@}*/


/**
 *  \addtogroup a2dp_constants
 *
 *  @name A2DP MPEG-2,4 Codec Channels
 *  Constant Definitions for types of A2DP MPEG-2,4 Codec Channels.
 */
/*@{*/

#define A2DP_MPEG_2_4_CHANNELS_1                     0x08U
#define A2DP_MPEG_2_4_CHANNELS_2                     0x04U

/*@}*/

/**
 *  \addtogroup a2dp_constants
 *
 *  @name A2DP MPEG-1,2 Codec Variable Bit Rate
 *  Constant Definitions for VBR of A2DP MPEG-1,2 Codec.
 */
/*@{*/

#define A2DP_MPEG_2_4_VBR_ON                         0x80U
#define A2DP_MPEG_2_4_VBR_OFF                        0x00U

/*@}*/


/**
 *  \addtogroup a2dp_constants
 *
 *  @name A2DP MPEG-2,4 Bit Rate
 *  Constant Definitions for Bit Rate Values A2DP MPEG-2,4 Codec.
 *  It is 23-bit value.
 */
/*@{*/
#define A2DP_MPEG_2_4_BITRATE_UNKNOWN               0x0000U

/*@}*/
/** \} */

/* ----------------------------------------- Structures/Data Types */
/**
 * \defgroup a2dp_structures Structures
 * \{
 * Describes Structures defined by A2DP module.
 */
/**
 *
 *  \addtogroup a2dp_structures
 *  \typedef A2DP_DEVICE_INFO
 *  \brief   A2DP Peer device information structure
 *
 *  This data type is used to hold the A2DP device information consisting of the
 *  A2DP device address, the Codec Information Elements and Delay Reporting status.
 */
typedef struct _A2DP_DEVICE_INFO
{
    /* Bluetooth Device Address */
    UCHAR * bd_addr;

    /* Codec Configuration Element */
    UCHAR * codec_ie_conf;

    /* Codec Configuration Element length */
    UCHAR  codec_ie_len;

#ifdef AVDTP_HAVE_CONTENT_PROTECTION
    AVDTP_CP_CAP cp_conf;
	UINT16  cp_type;
#endif /* AVDTP_HAVE_CONTENT_PROTECTION */
#ifdef AVDTP_HAVE_RECOVERY_SERVICE
    AVDTP_RECOVERY_CAP recovery_conf;
#endif /* AVDTP_HAVE_RECOVERY_SERVICE */
#ifdef A2DP_1_3
    /* Delay report config flag */
    UCHAR   dr_config_flag;
#endif /* A2DP_1_3 */
#ifdef AVDTP_HAVE_REPORTING_SERVICE
    UCHAR reporting_conf;
#endif /* AVDTP_HAVE_REPORTING_SERVICE */
#ifdef AVDTP_HAVE_ROHC_SERVICE
    UCHAR rohc_conf;
#endif /* AVDTP_HAVE_ROHC_SERVICE */
#ifdef AVDTP_HAVE_MULTIPLEXING
    UCHAR multiplexing_conf;
#endif /* AVDTP_HAVE_MULTIPLEXING */

}A2DP_DEVICE_INFO;

/** \} */
/** \} */

/**
 *  \defgroup a2dp_cb Application Callback
 *  \{
 *  This Section Describes the module Notification Callback interface offered
 *  to the application
 */

/**
 *  \typedef A2DP_EVENT_NTF_CB
 *  \brief   A2DP Event Notification Callback
 *
 *  This data type represents the A2DP Event Notification Callback, which
 *  A2DP uses for the following purposes:
 *  \li    To confirm result of an operation initiated by the application.
 *  \li    To inform the application of asynchronous events.
 *
 *  \param [in] codec_instance
 *         Instance of the A2DP Codec Entity responsible for this event.
 *  \param [in] event_type
 *         This parameter defines the A2DP event being reported. The valid
 *         values for this parameter are defined in "A2DP Event Notification
 *         Callback Event Types" section.
 *  \param [in] event_result
 *         If the event being reported is a result of an earlier trigger (an
 *         A2DP API) by the application, this parameter holds the result of
 *         the procedure. On successful completion of the procedure this
 *         parameter will have a value of 0x0000 (API_SUCCESS), else it will
 *         contain an Error Code describing the reason for failure.
 *         For asynchronous event indications, this parameter holds the
 *         possible reason due to which the event is being reported.
 *  \param [in] event_data
 *         Depending upon the event_type, this parameters holds any data
 *         being given to the A2DP application. Description of any such
 *         data will be provided whenever appropriate in the description
 *         of A2DP APIs that follows.
 *  \param [in] event_datalen
 *         This parameter specifies the size of event_data above.
 *
 *  \return API_RESULT: API_SUCCESS or an A2DP Error Code describing
 *                      the cause of failure.
 *
 *  \par Asynchronous Event Notification
 */
typedef API_RESULT (* A2DP_EVENT_NTF_CB)
                   (
                       UCHAR    codec_instance,
                       UCHAR    event_type,
                       UINT16   event_result,
                       void  *  event_data,
                       UINT16   event_datalen
                   );
/** \} */

/**
 * \cond ignore_this
 * \{
 */

/**
 *  \page page1 A2DP Asynchronous Event Notification
 *  This section describes the use of A2DP_EVENT_NTF_CB for asynchronous
 *  event notifications, that is, when called with the following event
 *  types:
 *      \li \ref Section_1 (A2DP_CONNECT_IND)
 *      \li \ref Section_2 (A2DP_DISCONNECT_IND)
 *      \li \ref Section_3 (A2DP_START_IND)
 *      \li \ref Section_4 (A2DP_CONFIGURE_IND)
 *      \li \ref Section_5 (A2DP_SUSPEND_IND)
 *      \li \ref Section_6 (A2DP_MEDIA_FRAME_IND)
 *
 *  The sections below describe how A2DP Event Notfication is called for
 *  asynchronous events and also gives a guidelines on how they should be
 *  handled.
 *
 *
 *  \section Section_1 A2DP Connection Indication
 *  The A2DP will call the Event Notification Callback for this event
 *  whenever a remote SEP is able to establish a connection with the SEP
 *  exposed by this A2DP Codec Entity.
 *
 *  \param [in] codec_instance
 *         Instance of the A2DP Codec Entity responsible for this event.
 *  \param [in] event_type
 *         This parameter will be set to A2DP_CONNECT_IND.
 *  \param [in] event_result
 *         This parameter has no meaning for this event, and will be set to
 *         0x0000.
 *  \param [in] event_data
 *         This parameter will have reference to A2DP_DEVICE_INFO structure
 *         that will contain peer device address and current codec
 *         configuration information.
 *  \param [in] event_datalen
 *         This parameter will be the sizeof(A2DP_DEVICE_INFO).
 *
 *  \return API_RESULT: API_SUCCESS, or an Error Code describing the reason
 *                      for failure.
 *
 *
 *  \section Section_2 A2DP Disconnection Indication
 *  The A2DP will call the Event Notification Callback for this event
 *  whenever the connection of the SEP represented by this A2DP Codec Entity
 *  with the remote SEP terminated without the instance of application.
 *
 *  \param [in] codec_instance
 *         Instance of the A2DP Codec Entity responsible for this event.
 *  \param [in] event_type
 *         This parameter will be set to A2DP_DISCONNECT_IND.
 *  \param [in] event_result
 *         This parameter describes the reason for disconnection.
 *  \param [in] event_data
 *         This parameter is not used for this event, and will be set to NULL.
 *  \param [in] event_datalen
 *         This parameter is not used for this event, and will be set to Zero.
 *
 *  \return API_RESULT: API_SUCCESS, or an Error Code describing the reason
 *                      for failure.
 *
 *
 *  \section Section_3 A2DP Start Indication
 *  The A2DP will call the Event Notification Callback for this event
 *  whenever it receives the notification to enable Streaming Data
 *  over this connection.
 *
 *  \param [in] codec_instance
 *         Instance of the A2DP Codec Entity responsible for this event.
 *  \param [in] event_type
 *         This parameter will be set to A2DP_START_IND.
 *  \param [in] event_result
 *         This parameter has no meaning for this event, and will be set to
 *         0x0000.
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
 *  \section Section_4 A2DP Configure Indication
 *  The A2DP will call the Event Notification Callback for this event
 *  whenever it receives notification for service capability configuration
 *  for connection with a remote SEP entity.
 *
 *  \param [in] codec_instance
 *         Instance of the A2DP Codec Entity responsible for this event.
 *  \param [in] event_type
 *         This parameter will be set to A2DP_CONFIGURE_IND.
 *  \param [in] event_result
 *         This parameter has no meaning for this event, and will be set to
 *         0x0000.
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
 *  \section Section_5 A2DP Suspend Indication
 *  The A2DP will call the Event Notification Callback for this event
 *  whenever it receives the notification to disable Streaming Data
 *  over this connection.
 *
 *  \param [in] codec_instance
 *         Instance of the A2DP Codec Entity responsible for this event.
 *  \param [in] event_type
 *         This parameter will be set to A2DP_SUSPEND_IND.
 *  \param [in] event_result
 *         This parameter has no meaning for this event, and will be set to
 *         0x0000.
 *  \param [in] event_data
 *         This parameter is not used for this event, and will be set to NULL.
 *  \param [in] event_datalen
 *         This parameter is not used for this event, and will be set to Zero.
 *
 *  \return API_RESULT: API_SUCCESS, or an Error Code describing the reason
 *                      for failure.
 *
 *
 *  \section Section_6 A2DP Media Frame Indication
 *  The A2DP will call the Event Notification Callback for this event
 *  whenever it receives Media Frames over the streaming connection with a
 *  remote SEP entity.
 *
 *  \param [in] codec_instance
 *         Instance of the A2DP Codec Entity responsible for this event.
 *  \param [in] event_type
 *         This parameter will be set to A2DP_MEDIA_FRAME_IND.
 *  \param [in] event_result
 *         This parameter has no meaning for this event, and will be set to
 *         0x0000.
 *  \param [in] event_data
 *         This parameter will be a pointer to a UCHAR buffer contining the
 *         media packet.
 *  \param [in] event_datalen
 *         This parameter contains the size of event_data above.
 *
 *
 *  \return API_RESULT: API_SUCCESS, or an Error Code describing the reason
 *                      for failure.
 *
 *
 *  \page page2 A2DP Synchronous Event Notification
 *  This section describes the use of A2DP_EVENT_NTF_CB for synchronous
 *  event notifications, that is, when called with the following event
 *  types:
 *      \li \ref Section_1 (A2DP_CONNECT_CNF)
 *      \li \ref Section_2 (A2DP_DISCONNECT_CNF)
 *      \li \ref Section_3 (A2DP_START_CNF)
 *      \li \ref Section_4 (A2DP_RECONFIGURE_CNF)
 *
 *  The sections below describe how A2DP Event Notfication is called for
 *  synchronous events and also gives a guidelines on how they should be
 *  handled.
 *
 *
 *  \section Section_1 A2DP Connection Confirmation
 *  The A2DP will call the Event Notification Callback for this event
 *  for confirmation of the result of the application initiated connect
 *  request to a remote SEP entity.
 *
 *  \param [in] codec_instance
 *         Instance of the A2DP Codec Entity responsible for this event.
 *  \param [in] event_type
 *         This parameter will be set to A2DP_CONNECT_CNF.
 *  \param [in] event_result
 *         0x0000, if successful. Otherwise, it is an error code
 *         describing the reason for failure.
 *  \param [in] event_data
 *         This parameter will have reference to A2DP_DEVICE_INFO structure
 *         that will contain peer device address and current codec
 *         configuration information.
 *  \param [in] event_datalen
 *         This parameter will be the sizeof(A2DP_DEVICE_INFO).
 *
 *  \return API_RESULT: API_SUCCESS, or an Error Code describing the reason
 *                      for failure.
 *
 *
 *  \section Section_1 A2DP Disconnection Confirmation
 *  The A2DP will call the Event Notification Callback for this event
 *  for confirmation of the result of the application initiated disconnect
 *  request from a remote SEP entity.
 *
 *  \param [in] codec_instance
 *         Instance of the A2DP Codec Entity responsible for this event.
 *  \param [in] event_type
 *         This parameter will be set to A2DP_DISCONNECT_CNF.
 *  \param [in] event_result
 *         0x0000, if successful. Otherwise, it is an error code
 *         describing the reason for failure.
 *  \param [in] event_data
 *         This parameter is not used for this event, and will be set to NULL.
 *  \param [in] event_datalen
 *         This parameter is not used for this event, and will be set to Zero.
 *
 *  \return API_RESULT: API_SUCCESS, or an Error Code describing the reason
 *                      for failure.
 *
 *
 *  \section Section_3 A2DP Start Confirmation
 *  The A2DP will call the Event Notification Callback for this event
 *  for confirmation of the result of the application initiated request
 *  for streaming over connection with a remote SEP entity.
 *
 *  \param [in] codec_instance
 *         Instance of the A2DP Codec Entity responsible for this event.
 *  \param [in] event_type
 *         This parameter will be set to A2DP_START_CNF.
 *  \param [in] event_result
 *         0x0000, if successful. Otherwise, it is an error code
 *         describing the reason for failure.
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
 *  \section Section_4 A2DP Reconfigure Confirmation
 *  The A2DP will call the Event Notification Callback for this event
 *  for confirmation of the result of the application initiated request
 *  for reconfiguration of the service configurations for the connection
 *  with a remote SEP entity.
 *
 *  \param [in] codec_instance
 *         Instance of the A2DP Codec Entity responsible for this event.
 *  \param [in] event_type
 *         This parameter will be set to A2DP_RECONFIGURE_CNF.
 *  \param [in] event_result
 *         This parameter has no meaning for this event, and will be set to
 *         0x0000.
 *  \param [in] event_data
 *         This parameter is not used for this event, and will be set to NULL.
 *  \param [in] event_datalen
 *         This parameter is not used for this event, and will be set to Zero.
 *
 *  \return API_RESULT: API_SUCCESS, or an Error Code describing the reason
 *                      for failure.
 */

/** \endcond */

#ifdef AVDTP_ASSISTIVE_MODE
typedef AVDTP_MEDIA_CHANNEL_PARAMS A2DP_MEDIA_CHANNEL_PARAMS;
#endif /* AVDTP_ASSISTIVE_MODE */

/* ----------------------------------------- Macros */
/**
 *  \defgroup A2DP_Utility_Macros Utility Macros
 *
 *  This section describes the EtherMind Utility Macros of A2DP module.
 */
/** \{ */
/**
 *  \addtogroup A2DP_Utility_Macros
 *
 *  @name A2DP Utility Macros
 *  Defining A2DP Utility Macros.
 */

/*@{*/

/**
 *  This utility macro initializes the SBC Codec specific Information
 *  Elements which are stored in AVDTP_SEP_CONF structure.
 *
 *  \param [out] conf
 *         Placeholder for SEP Configuration Information.
 *
 *  \hideinitializer
 */
#define A2DP_SBC_INIT_CODEC_CAP( conf )         \
        (conf).codec_cap.codec_ie[0U] = 0x00U;  \
        (conf).codec_cap.codec_ie[1U] = 0x00U;  \
        (conf).codec_cap.codec_ie[2U] = 0x00U;  \
        (conf).codec_cap.codec_ie[3U] = 0x00U;

/**
 *  This utility macro initializes Sampling Frequency field in
 *  the SBC Codec specific Information Elements which are stored in
 *  AVDTP_SEP_CONF structure.
 *
 *  \param [in,out] conf
 *         Placeholder for SEP Configuration Information.
 *  \param [in] freqn
 *         A2DP SBC Codec Sampling Frequency Value.
 *
 *  \hideinitializer
 */
#define A2DP_SBC_SET_SAMPLING_FREQUENCY( conf, freqn ) \
        (conf).codec_cap.codec_ie[0U] |= (freqn);

/**
 *  This utility macro initializes Channel Mode field in
 *  the SBC Codec specific Information Elements which are stored in
 *  AVDTP_SEP_CONF structure.
 *
 *  \param [in,out] conf
 *         Placeholder for SEP Configuration Information.
 *  \param [in] cm
 *         A2DP SBC Codec Channel Mode Value.
 *
 *  \hideinitializer
 */
#define A2DP_SBC_SET_CHANNEL_MODE( conf, cm )   \
        (conf).codec_cap.codec_ie[0U] |= (cm);

/**
 *  This utility macro initializes Block Length field in
 *  the SBC Codec specific Information Elements which are stored in
 *  AVDTP_SEP_CONF structure.
 *
 *  \param [in,out] conf
 *         Placeholder for SEP Configuration Information.
 *  \param [in] bl
 *         A2DP SBC Block Length.
 *
 *  \hideinitializer
 */
#define A2DP_SBC_SET_BLOCK_LENGTH( conf, bl )   \
        (conf).codec_cap.codec_ie[1U] |= (bl);

/**
 *  This utility macro initializes Number of A2DP SBC Sub Bands field
 *  in the SBC Codec specific Information Elements which are stored in
 *  AVDTP_SEP_CONF structure.
 *
 *  \param [in,out] conf
 *         Placeholder for SEP Configuration Information.
 *  \param [in] sb
 *         Number of A2DP SBC Sub Bands.
 *
 *  \hideinitializer
 */
#define A2DP_SBC_SET_SUBBANDS( conf, sb )       \
        (conf).codec_cap.codec_ie[1U] |= (sb);

/**
 *  This utility macro initializes A2DP SBC Allocation Method field
 *  in the SBC Codec specific Information Elements which are stored in
 *  AVDTP_SEP_CONF structure.
 *
 *  \param [in,out] conf
 *         Placeholder for SEP Configuration Information.
 *  \param [in] am
 *         A2DP SBC Allocation Method Bands.
 *
 *  \hideinitializer
 */
#define A2DP_SBC_SET_ALLOCATION_METHOD( conf, am ) \
        (conf).codec_cap.codec_ie[1U] |= (am);

/**
 *  This utility macro initializes Minimum and Maximum Bitpool Value fields
 *  in the SBC Codec specific Information Elements which are stored in
 *  AVDTP_SEP_CONF structure.
 *
 *  \param [out] conf
 *         Placeholder for SEP Configuration Information.
 *  \param [in] min_bp
 *         Minimum Bitpool Value.
 *  \param [in] max_bp
 *         Maximum Bitpool Value.
 *
 *  \hideinitializer
 */
#define A2DP_SBC_SET_BITPOOL( conf,  min_bp,   max_bp ) \
        (conf).codec_cap.codec_ie[2U] = (min_bp); \
        (conf).codec_cap.codec_ie[3U] = (max_bp);

/**
 *  This utility macro is used to obtain Sampling Frequency value stored in
 *  the SBC Codec specific Information Elements which are stored in
 *  AVDTP_SEP_CONF structure.
 *
 *  \param [in] conf
 *         Placeholder for SEP Configuration Information.
 *  \param [out] freqn
 *         A2DP SBC Codec Sampling Frequency Value.
 *
 *  \hideinitializer
 */
#define A2DP_SBC_GET_SAMPLING_FREQUENCY( conf, freqn ) \
        (freqn) = (conf).codec_cap.codec_ie[0U] & 0xF0U;

/**
 *  This utility macro is used to obtain Channel Mode value stored in
 *  the SBC Codec specific Information Elements which are stored in
 *  AVDTP_SEP_CONF structure.
 *
 *  \param [in] conf
 *         Placeholder for SEP Configuration Information.
 *  \param [out] cm
 *         A2DP SBC Codec Channel Mode Value.
 *
 *  \hideinitializer
 */
#define A2DP_SBC_GET_CHANNEL_MODE( conf, cm )   \
         (cm) = (conf).codec_cap.codec_ie[0U] & 0x0FU;

/**
 *  This utility macro is used to obtain Block Length value stored in
 *  the SBC Codec specific Information Elements which are stored in
 *  AVDTP_SEP_CONF structure.
 *
 *  \param [in] conf
 *         Placeholder for SEP Configuration Information.
 *  \param [out] bl
 *         A2DP SBC Block Length.
 *
 *  \hideinitializer
 */
#define A2DP_SBC_GET_BLOCK_LENGTH( conf, bl )   \
        (bl) = (conf).codec_cap.codec_ie[1U] & 0xF0U;

/**
 *  This utility macro is used to obtain Number of A2DP SBC Sub Bands value
 *  stored in the SBC Codec specific Information Elements which are stored
 *  in AVDTP_SEP_CONF structure.
 *
 *  \param [in] conf
 *         Placeholder for SEP Configuration Information.
 *  \param [out] sb
 *         Number of A2DP SBC Sub Bands.
 *
 *  \hideinitializer
 */
#define A2DP_SBC_GET_SUBBANDS( conf, sb ) \
        (sb) = (conf).codec_cap.codec_ie[1U] & 0x0CU;

/**
 *  This utility macro obtains value of A2DP SBC Allocation Method
 *  in the SBC Codec specific Information Elements which are stored in
 *  AVDTP_SEP_CONF structure.
 *
 *  \param [in] conf
 *         Placeholder for SEP Configuration Information.
 *  \param [out] am
 *         A2DP SBC Allocation Method Bands.
 *
 *  \hideinitializer
 */
#define A2DP_SBC_GET_ALLOCATION_METHOD( conf, am ) \
        (am) = (conf).codec_cap.codec_ie[1U] & 0x03U;

/**
 *  This utility macro is used to obtain value of Minimum and Maximum Bitpool
 *  in the SBC Codec specific Information Elements which are stored in
 *  AVDTP_SEP_CONF structure.
 *
 *  \param [in] conf
 *         Placeholder for SEP Configuration Information.
 *  \param [out] min_bp
 *         Minimum Bitpool Value.
 *  \param [out] max_bp
 *         Maximum Bitpool Value.
 *
 *  \hideinitializer
 */
#define A2DP_SBC_GET_BITPOOL( conf, min_bp, max_bp ) \
        (min_bp) = (conf).codec_cap.codec_ie[2U]; \
        (max_bp) = (conf).codec_cap.codec_ie[3U];

/**
 *  This utility macro is used to to set role of Codec as Player in
 *  SDP database service record.
 *
 *  \param [in,out] rec
 *         Placeholder for Codec Specific SDP data.
 *
 *  \hideinitializer
 */
#define A2DP_SET_SERVICE_RECORD_PLAYER( rec ) \
        (rec) |= 0x01U;

/**
 *  This utility macro is used to set role of Codec as Microphone in
 *  SDP database service record.
 *
 *  \param [in,out] rec
 *         Placeholder for Codec Specific SDP data.
 *
 *  \hideinitializer
 */
#define A2DP_SET_SERVICE_RECORD_MICROPHONE( rec ) \
        (rec) |= 0x02U;

/**
 *  This utility macro is used to set role of Codec as Tuner in
 *  SDP database service record.
 *
 *  \param [in,out] rec
 *         Placeholder for Codec Specific SDP data.
 *
 *  \hideinitializer
 */
#define A2DP_SET_SERVICE_RECORD_TUNER( rec ) \
        (rec) |= 0x04U;

/**
 *  This utility macro is used to set role of Codec as Mixer in
 *  SDP database service record.
 *
 *  \param [in,out] rec
 *         Placeholder for Codec Specific SDP data.
 *
 *  \hideinitializer
 */
#define A2DP_SET_SERVICE_RECORD_MIXER( rec ) \
        (rec) |= 0x08U;

/**
 *  This utility macro is used to set role of Codec as Headphone in
 *  SDP database service record.
 *
 *  \param [in,out] rec
 *         Placeholder for Codec Specific SDP data.
 *
 *  \hideinitializer
 */
#define A2DP_SET_SERVICE_RECORD_HEADPHONE( rec ) \
        (rec) |= 0x01U;

/**
 *  This utility macro is used to set role of Codec as Speaker in
 *  SDP database service record.
 *
 *  \param [in,out] rec
 *         Placeholder for Codec Specific SDP data.
 *
 *  \hideinitializer
 */
#define A2DP_SET_SERVICE_RECORD_SPEAKER( rec ) \
        (rec) |= 0x02U;

/**
 *  This utility macro is used to set role of Codec as Recorder in
 *  SDP database service record.
 *
 *  \param [in,out] rec
 *         Placeholder for Codec Specific SDP data.
 *
 *  \hideinitializer
 */
#define A2DP_SET_SERVICE_RECORD_RECORDER( rec ) \
        (rec) |= 0x04U;

/**
 *  This utility macro is used to set role of Codec as Amplifier in
 *  SDP database service record.
 *
 *  \param [in,out] rec
 *         Placeholder for Codec Specific SDP data.
 *
 *  \hideinitializer
 */
#define A2DP_SET_SERVICE_RECORD_AMPLIFIER( rec ) \
        (rec) |= 0x08U;

/**
 *  This utility macro initializes the MPEG-1,2 Codec specific Information
 *  Elements which are stored in AVDTP_SEP_CONF structure.
 *
 *  \param [out] conf
 *         Placeholder for SEP Configuration Information.
 *
 *  \hideinitializer
 */
#define A2DP_MPEG_1_2_INIT_CODEC_CAP( conf )    \
        (conf).codec_cap.codec_ie[0U] = 0x00U;  \
        (conf).codec_cap.codec_ie[1U] = 0x00U;  \
        (conf).codec_cap.codec_ie[2U] = 0x00U;  \
        (conf).codec_cap.codec_ie[3U] = 0x00U;

/**
 *  This utility macro initializes layer field in the MPEG-1,2 Codec
 *  specific Information Elements which are stored in AVDTP_SEP_CONF
 *  structure.
 *
 *  \param [in,out] conf
 *         Placeholder for SEP Configuration Information.
 *  \param [in] layer
 *         A2DP MPEG-1,2 Codec Layer Value.
 *
 *  \hideinitializer
 */
#define A2DP_MPEG_1_2_SET_LAYER( conf, layer ) \
        (conf).codec_cap.codec_ie[0U] |= (layer);

/**
 *  This utility macro initializes CRC field in the MPEG-1,2 Codec
 *  specific Information Elements which are stored in AVDTP_SEP_CONF
 *  structure.
 *
 *  \param [in,out] conf
 *         Placeholder for SEP Configuration Information.
 *  \param [in] crc
 *         A2DP MPEG-1,2 Codec CRC Value.
 *
 *  \hideinitializer
 */
#define A2DP_MPEG_1_2_SET_CRC( conf, crc ) \
        (conf).codec_cap.codec_ie[0U] |= (crc);

/**
 *  This utility macro initializes Channel Mode field in the MPEG-1,2 Codec
 *  specific Information Elements which are stored in AVDTP_SEP_CONF
 *  structure.
 *
 *  \param [in,out] conf
 *         Placeholder for SEP Configuration Information.
 *  \param [in] cm
 *         A2DP MPEG-1,2 Codec Channel Mode Value.
 *
 *  \hideinitializer
 */
#define A2DP_MPEG_1_2_SET_CHANNEL_MODE( conf, cm )   \
        (conf).codec_cap.codec_ie[0U] |= (cm);

/**
 *  This utility macro initializes Media Payload Format (MPF) field in the
 *  MPEG-1,2 Codec specific Information Elements which are stored in
 *  AVDTP_SEP_CONF structure.
 *
 *  \param [in,out] conf
 *         Placeholder for SEP Configuration Information.
 *  \param [in] mpf
 *         A2DP MPEG-1,2 Codec Media Payload Format Type.
 *
 *  \hideinitializer
 */
#define A2DP_MPEG_1_2_SET_MPF( conf, mpf )  \
        (conf).codec_cap.codec_ie[1U] |= (mpf);

/**
 *  This utility macro initializes Sampling Frequency field in
 *  the MPEG-1_2 Codec specific Information Elements which are stored in
 *  AVDTP_SEP_CONF structure.
 *
 *  \param [in,out] conf
 *         Placeholder for SEP Configuration Information.
 *  \param [in] freqn
 *         A2DP MPEG_1_2 Codec Sampling Frequency Value.
 *
 *  \hideinitializer
 */
#define A2DP_MPEG_1_2_SET_SAMPLING_FREQUENCY( conf, freqn ) \
        (conf).codec_cap.codec_ie[1U] |= (freqn);

/**
 *  This utility macro initializes Variable Bit Rate field in
 *  the MPEG-1,2 Codec specific Information Elements which are stored in
 *  AVDTP_SEP_CONF structure.
 *
 *  \param [in,out] conf
 *         Placeholder for SEP Configuration Information.
 *  \param [in] vbr
 *         A2DP MPEG_1_2 Codec VBR support value.
 *
 *  \hideinitializer
 */
#define A2DP_MPEG_1_2_SET_VBR( conf, vbr ) \
        (conf).codec_cap.codec_ie[2U] |= (vbr);

/**
 *  This utility macro obtains value of Layer in the MPEG_1_2
 *  Codec specific Information Elements which are stored in AVDTP_SEP_CONF
 *  structure.
 *
 *  \param [in] conf
 *         Placeholder for SEP Configuration Information.
 *  \param [out] layer
 *         A2DP MPEG-1,2 Codec Layer Value.
 *
 *  \hideinitializer
 */
#define A2DP_MPEG_1_2_GET_LAYER( conf, layer ) \
        (layer) = ((conf).codec_cap.codec_ie[0U] & 0xE0U);

/**
 *  This utility macro obtains value of CRC Protection bit in
 *  the MPEG_1_2 Codec specific Information Elements which are stored in
 *  AVDTP_SEP_CONF structure.
 *
 *  \param [in] conf
 *         Placeholder for SEP Configuration Information.
 *  \param [out] crc
 *         A2DP MPEG-1,2 Codec CRC Value.
 *
 *  \hideinitializer
 */
#define A2DP_MPEG_1_2_GET_CRC( conf, crc ) \
        (crc) = ((conf).codec_cap.codec_ie[0U] & 0x10U);

/**
 *  This utility macro obtains value of Channel Mode in
 *  the MPEG_1_2 Codec specific Information Elements which are stored in
 *  AVDTP_SEP_CONF structure.
 *
 *  \param [in] conf
 *         Placeholder for SEP Configuration Information.
 *  \param [out] cm
 *         A2DP MPEG-1,2 Codec Channel Mode Value.
 *
 *  \hideinitializer
 */
#define A2DP_MPEG_1_2_GET_CHANNEL_MODE( conf, cm )   \
        (cm) = ((conf).codec_cap.codec_ie[0U] & 0x0FU);

/**
 *  This utility macro obtains value of Media Payload
 *  Foramt in the MPEG_1_2 Codec specific Information Elements which
 *  are stored in AVDTP_SEP_CONF structure.
 *
 *  \param [in] conf
 *         Placeholder for SEP Configuration Information.
 *  \param [out] mpf
 *         A2DP MPEG-1,2 Codec Media Payload Format Type.
 *
 *  \hideinitializer
 */
#define A2DP_MPEG_1_2_GET_MPF( conf, mpf )  \
        (mpf) = ((conf).codec_cap.codec_ie[1U] & 0x40U);

/**
 *  This utility macro obtains value of Sampling Frequency
 *  the MPEG_1_2 Codec specific Information Elements which are stored in
 *  AVDTP_SEP_CONF structure.
 *
 *  \param [in] conf
 *         Placeholder for SEP Configuration Information.
 *  \param [out] freqn
 *         A2DP MPEG_1_2 Codec Sampling Frequency Value.
 *
 *  \hideinitializer
 */
#define A2DP_MPEG_1_2_GET_SAMPLING_FREQUENCY( conf, freqn ) \
        (freq) = ((conf).codec_cap.codec_ie[1U] & 0x3FU);

/**
 *  This utility macro obtains value of Variable Bit Rate
 *  bit in the MPEG_1_2 Codec specific Information Elements which are
 *  stored in AVDTP_SEP_CONF structure.
 *
 *  \param [in] conf
 *         Placeholder for SEP Configuration Information.
 *  \param [out] vbr
 *         A2DP MPEG_1_2 Codec VBR support value.
 *
 *  \hideinitializer
 */
#define A2DP_MPEG_1_2_GET_VBR( conf, vbr ) \
        (vbr) = ((conf).codec_cap.codec_ie[2U] & 0x80U);

/**
 *  This utility macro initializes the MPEG-2,4 Codec specific Information
 *  Elements which are stored in AVDTP_SEP_CONF structure.
 *
 *  \param [out] conf
 *         Placeholder for SEP Configuration Information.
 *
 *  \hideinitializer
 */
#define A2DP_MPEG_2_4_INIT_CODEC_CAP( conf )    \
        (conf).codec_cap.codec_ie[0U] = 0x00U;  \
        (conf).codec_cap.codec_ie[1U] = 0x00U;  \
        (conf).codec_cap.codec_ie[2U] = 0x00U;  \
        (conf).codec_cap.codec_ie[3U] = 0x00U;  \
        (conf).codec_cap.codec_ie[4U] = 0x00U;  \
        (conf).codec_cap.codec_ie[5U] = 0x00U;

/**
 *  This utility macro initializes object type field in the MPEG-2,4 Codec
 *  specific Information Elements which are stored in AVDTP_SEP_CONF
 *  structure.
 *
 *  \param [out] conf
 *         Placeholder for SEP Configuration Information.
 *  \param [in] obj_type
 *         A2DP MPEG-2,4 object type.
 *
 *  \hideinitializer
 */
#define A2DP_MPEG_2_4_SET_OBJECT_TYPE( conf, obj_type ) \
        (conf).codec_cap.codec_ie[0U] = (obj_type);

/**
 *  This utility macro initializes channels field in the MPEG-2,4 Codec
 *  specific Information Elements which are stored in AVDTP_SEP_CONF
 *  structure.
 *
 *  \param [in,out] conf
 *         Placeholder for SEP Configuration Information.
 *  \param [in] channels
 *         A2DP MPEG-2,4 Channels.
 *
 *  \hideinitializer
 */
#define A2DP_MPEG_2_4_SET_CHANNELS( conf, channels ) \
        (conf).codec_cap.codec_ie[2U] |= (channels);

/**
 *  This utility macro initializes variable bit rate(VBR) bit in the MPEG-2,4 Codec
 *  specific Information Elements which are stored in AVDTP_SEP_CONF structure.
 *
 *  \param [in,out] conf
 *         Placeholder for SEP Configuration Information.
 *  \param [in] vbr
 *         A2DP MPEG-2,4 Variable Bit Rate(VBR).
 *
 *  \hideinitializer
 */
#define A2DP_MPEG_2_4_SET_VBR(conf, vbr) \
        (conf).codec_cap.codec_ie[3U] |= (vbr);

/**
 *  This utility macro obtains value of object type field in the MPEG_2_4 Codec
 *  specific Information Elements which are stored in AVDTP_SEP_CONF structure.
 *
 *  \param [in] conf
 *         Placeholder for SEP Configuration Information.
 *  \param [out] obj_type
 *         A2DP MPEG_2_4 Obect Type value.
 *
 *  \hideinitializer
 */
#define A2DP_MPEG_2_4_GET_OBJECT_TYPE( conf, obj_type ) \
        (obj_type) = (conf).codec_cap.codec_ie[0U];

/**
 *  This utility macro obtains value of channels field in the MPEG_2_4 Codec
 *  specific Information Elements which are stored in AVDTP_SEP_CONF structure.
 *
 *  \param [in] conf
 *         Placeholder for SEP Configuration Information.
 *  \param [out] channels
 *         A2DP MPEG_2_4 Channels.
 *
 *  \hideinitializer
 */
#define A2DP_MPEG_2_4_GET_CHANNELS( conf, channels ) \
        (channels) = ((conf).codec_cap.codec_ie[2U] & 0x0CU);

/**
 *  This utility macro obtains value of Variable Bit Rate bit in the MPEG_2_4
 *  Codec specific Information Elements which are stored in AVDTP_SEP_CONF structure.
 *
 *  \param [in] conf
 *         Placeholder for SEP Configuration Information.
 *  \param [out] vbr
 *         A2DP MPEG_2_4 Codec VBR support value.
 *
 *  \hideinitializer
 */
#define A2DP_MPEG_2_4_GET_VBR( conf, vbr ) \
        (vbr) = ((conf).codec_cap.codec_ie[3U] & 0x80U);

/**
 *  This utility macro activates SDP Database Service Record for
 *  A2DP 'Source' Service
 *
 *  \param None
 *
 *  \hideinitializer
 */
#define BT_a2dp_activate_sdp_record_source() \
        a2dp_update_sdp_record(DB_RECORD_A2DP_SOURCE, 0x1U);

/**
 *  This utility macro inactivates SDP Database Service Record for
 *  A2DP 'Source' Service
 *
 *  \param None
 *
 *  \hideinitializer
 */
#define BT_a2dp_inactivate_sdp_record_source() \
        a2dp_update_sdp_record(DB_RECORD_A2DP_SOURCE, 0x0U);

/**
 *  This utility macro activates SDP Database Service Record for
 *  A2DP 'Sink' Service
 *
 *  \param None
 *
 *  \hideinitializer
 */
#define BT_a2dp_activate_sdp_record_sink() \
        a2dp_update_sdp_record(DB_RECORD_A2DP_SINK, 0x1U);

/**
 *  This utility macro inactivates SDP Database Service Record for
 *  A2DP 'Sink' Service
 *
 *  \param None
 *
 *  \hideinitializer
 */
#define BT_a2dp_inactivate_sdp_record_sink() \
        a2dp_update_sdp_record(DB_RECORD_A2DP_SINK, 0x0U);

/*@}*/
/** \} */

/* ----------------------------------------- APIs */
#ifdef __cplusplus
extern "C"{
#endif
/**
 * \defgroup A2DP_API API Definitions
 * \{
 * This section describes the EtherMind A2DP APIs.
 */
/**
 *  \brief To initialize A2DP module.
 *
 *  \par Description:
 *  This API is used to initialize the A2DP module. It initializes the
 *  A2DP Codec Entity table and the control mutex required for the
 *  A2DP module.
 *
 *  \param None
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \note
 *  This function must be called only once as a part of initialization
 *  routine of the complete system.
 *
 *  \sa
 *  BT_a2dp_shutdown
 */
API_RESULT BT_a2dp_init ( void );

/**
 *  \brief To shutdown A2DP module.
 *
 *  \par Description:
 *  This API is used to terminate the A2DP module. It disconnects all
 *  streaming connections if present, resets all A2DP Codec Entities,
 *  dereigsters A2DP module from SDP and AVDTP layers.
 *
 *  \param None
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \note
 *
 *  \sa
 *  BT_a2dp_init
 */
API_RESULT BT_a2dp_shutdown ( void );

/**
 *  \brief To register an Audio Codec with A2DP module.
 *
 *  \par Description:
 *  This API registers an Audio Codec along with its capabilities.
 *  On the successful registration, this API returns a Codec Instance
 *  which is an index to the A2DP Codec.
 *
 *  \param [out] inst
 *         Code Instance representing this Codec.
 *  \param [in] sep_cap
 *         Capabilities of the Codec.
 *  \param [in] ntf_cb
 *         Callback function which will be called for all
 *         Event Notifications.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_a2dp_register_codec
           (
               /* OUT */  UCHAR  *           inst,
               /* IN */   AVDTP_SEP_CAP  *   sep_cap,
               /* IN */   A2DP_EVENT_NTF_CB  ntf_cb
           );

/**
 *  \brief To deregister an Audio Codec with A2DP module.
 *
 *  \par Description:
 *  This API deregisters an Audio Codec from AVDTP which as already
 *  registered.
 *
 *  \param [in] inst
 *         Code Instance representing this Codec.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 */
API_RESULT BT_a2dp_deregister_codec
           (
               /* IN */ UCHAR inst
           );

/**
 *  \brief To Configure and Establish Stream.
 *
 *  \par Description:
 *  This API is used to establish a configured stream with a remote
 *  SEP Entity. The stream configuration parameters and the remote
 *  SEID with which the connection is to be established are provided by
 *  the application. The API can be used to perform the AVDTP
 *  Discover and Get Capability procedures by setting the remote SEID
 *  to Zero.
 *
 *  \param [in] inst
 *         Code Instance representing this Codec.
 *  \param [in] bd_addr
 *         Bluetooth Device Address of the remote device.
 *  \param [in] remote_seid
 *         SEID of the remote SEP entity.
 *  \param [in] sep_conf
 *         Service configuration parameters for the streaming
 *         connection.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \note
 *  Before calling this API the application needs to ensure that
 *  an AVDTP signalling channel with the device having bluetooth device
 *  address of bd_addr (which is passed an input parameter) is established.
 *
 *  The API does AVDTP Discovery, Get Capability, Set Configuration and
 *  Open Procedures when called with remote SEID set to 0.
 *  When the remote SEID is specified the API performs AVDTP Set Configuration
 *  and Open Procedures only.
 *
 *  \sa
 *  BT_a2dp_start, BT_a2dp_disconnect
 */
API_RESULT BT_a2dp_connect
           (
               /* IN */  UCHAR              inst,
               /* IN */  UCHAR  *           bd_addr,
               /* IN */  UCHAR              remote_seid,
               /* IN */  AVDTP_SEP_CONF  *  sep_conf
           );

/**
 *  \brief To Disconnect an Established Stream.
 *
 *  \par Description:
 *  This API is used to disconnect a stream connection established with
 *  a remote SEP Entity.
 *
 *  \param [in] inst
 *         Code Instance representing this Codec.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \remarks
 *
 *  \sa
 *  BT_a2dp_connect, BT_a2dp_start
 */
API_RESULT BT_a2dp_disconnect
           (
               /* IN */  UCHAR  inst
           );

/**
 *  \brief To Start Media Streaming.
 *
 *  \par Description:
 *  This API is used to enable Media Streaming for a connection
 *  with a remote SEP entity.
 *
 *  \param [in] inst
 *         Code Instance representing this Codec.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \remarks
 *  This API is called to enable streaming over a pre-configured channel.
 *  The channel is configured and ready for streaming when the A2DP module
 *  notifies the application by AVDTP_CONNECT_CNF event.
 *
 *  \sa
 *  BT_a2dp_connect, BT_a2dp_disconnect
 */
API_RESULT BT_a2dp_start
           (
               /* IN */  UCHAR  inst
           );


/**
 *  \brief To Reconfigure Stream.
 *
 *  \par Description:
 *  This API is used to reconfigure the Code Capabilities for the
 *  streaming connection with a remote SEP entity.
 *
 *  \param [in] inst
 *         Code Instance representing this Codec.
 *  \param [in] sep_conf
 *         Service configuration parameters for the
 *         re-configured streaming connection.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \remarks
 *  This API is used to reconfigure the service capabilities defined
 *  for the channel. It should be called only when the channel is in
 *  open state. If the channel is in the streaming state, the application
 *  needs to initiate streaming suspend procedure using BT_a2dp_suspend
 *  API. To enable the streaming over the reconfigured channels application
 *  needs to initiate start streaming procedure using BT_a2dp_start API.
 *  The streaming state is set only when the A2DP notifies application with
 *  A2DP_START_CNF or A2DP_START_IND events.
 *
 *  \sa
 *  BT_a2dp_connect, BT_a2dp_suspend, BT_a2dp_start, BT_a2dp_disconnect
 */
API_RESULT BT_a2dp_reconfigure
           (
              /* IN */  UCHAR              inst,
              /* IN */  AVDTP_SEP_CONF  *  sep_conf
           );


/**
 *  \brief To Suspend Streaming.
 *
 *  \par Description:
 *  This API is used to suspend the streaming with a remote SEP entity.
 *
 *  \param [in] inst
 *         Code Instance representing this Codec.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \remarks
 *
 *  \sa
 *  BT_a2dp_connect, BT_a2dp_start, BT_a2dp_reconfigure, BT_a2dp_disconnect
 */
API_RESULT BT_a2dp_suspend
           (
              /* IN */  UCHAR inst
           );

/**
 *  \brief To send Media Frames.
 *
 *  \par Description:
 *  This API is used to send Media Frames over a streaming connection
 *  with a remote SEP entity. Frames larger than the MTU size defined
 *  for the channel are fragmented to fit the MTU requirement by this
 *  API.
 *
 *  \param [in] inst
 *         Code Instance representing this Codec.
 *  \param [in] data
 *         Buffer containing Media Frames.
 *  \param [in] datalen
 *         Size of the buffer containing Media Frames.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \remarks
 *  This API should be called to send Media frames only after the
 *  A2DP module has given the indication for steaming state.
 *  The streaming state is set only when the A2DP notifies application
 *  with A2DP_START_CNF or A2DP_START_IND events.
 */
#ifndef A2DP_SUPPORT_MULTIPLE_MEDIA_FRAME_WRITE
API_RESULT BT_a2dp_media_write
           (
               /* IN */  UCHAR     inst,
               /* IN */  UCHAR  *  data,
               /* IN */  UINT16    datalen
           );
#else
API_RESULT BT_a2dp_media_write
           (
               /* IN */  UCHAR     codec_instance,
               /* IN */  UCHAR     frame_count,
               /* IN */  UCHAR  *  data,
               /* IN */  UINT16    data_length
           );
#endif /* A2DP_SUPPORT_MULTIPLE_MEDIA_FRAME_WRITE */

/**
 *  \brief To flush Media Frames send previously.
 *
 *  \par Description:
 *  This API is used to flush Media Frames out of A2DP which were send
 *  earlier.
 *
 *  \param [in] inst
 *         Codec Instance representing this Codec.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \remarks
 *  Application should issue the call to flush Media Frames out of A2DP
 *  to ensure that no Media Frames are left buffered within A2DP. Such
 *  situation may arise typically while end of a Media File is reached.
 *  Buffer optimization in A2DP resort to storing of Media Frames to
 *  capitalize available bandwidth.
 *
 *  Application should request for flush when it receives an indication
 *  of end of a Media File.
 *
 *  \sa
 *  BT_a2dp_media_write
 */
API_RESULT BT_a2dp_media_flush
           (
               /* IN */  UCHAR  inst
           );

/**
 *  \brief To calculate SBC frame size
 *
 *  \par Description:
 *  This function calculates the SBC frame size.
 *
 *  \param [in] channel_mode
 *         Mode of channel used to encode Audio Data.
 *  \param [in] nrof_subbands
 *         Number of subbands used to encode Audio Data.
 *  \param [in] nrof_blocks
 *         Number of blocks used to encode the Audio Data.
 *  \param [in] bitpool
 *         Bitpool value used to encode the Audio Data.
 *
 *  \return UINT16
 *          Frame size of encoded SBC frame.
 *
 *  \remarks
 *  Application needs to ensure that all parameters used for
 *  frame size calculation are correct. No error returns are
 *  defined for the request. For incorrect input parameters,
 *  the returned value is not defined.
 */
UINT16 BT_a2dp_get_sbc_frame_size
       (
           /* IN */  UCHAR  channel_mode,
           /* IN */  UCHAR  nrof_subbands,
           /* IN */  UCHAR  nrof_blocks,
           /* IN */  UCHAR  bitpool
       );

#ifdef A2DP_MPEG
/**
 *  \brief To calculate MPEG-1,2 frame size
 *
 *  \par Description:
 *  This function calculates the MPEG-1,2 frame size.
 *
 *  \param [in] sampling_frequency
 *         Sampling Frequency with which the audio data are sampled.
 *  \param [in] bit_rate
 *         Bit Rate of the MPEG-1,2 encoded stream.
 *
 *  \return UINT16
 *          Frame size of encoded MPEG-1,2 frame.
 *
 *  \remarks
 *  Application needs to ensure that all parameters used for
 *  frame size calculation are correct. No error returns are
 *  defined for the request. For incorrect input parameters,
 *  the returned value is not defined.
 */
UINT16 BT_a2dp_get_mpeg_1_2_frame_size
       (
           /* IN */  UINT16  sampling_frequency,
           /* IN */  UINT16  bit_rate
       );
#endif /* A2DP_MPEG */

/**
 *  \brief To set Media MTU
 *
 *  \par Description:
 *  This function enables applications to specify the InMTU which will
 *  be used by the underlying AVDTP to configure Media Transport Channel
 *  during Stream establishment.
 *
 *  \param [in] codec_instance
 *         Codec Instance representing this Codec.
 *  \param [in] in_mtu
 *         MTU value to be used for the Media Transport Channel
 *
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \sa
 *  BT_avdtp_set_media_mtu
 */
API_RESULT BT_a2dp_set_media_mtu
           (
               /* IN */  UCHAR   inst,
               /* IN */  UINT16  in_mtu
           );

#ifdef AVDTP_ASSISTIVE_MODE
/**
 *  \brief To Get Media Channel params.
 *
 *  \par Description:
 *         This API is used by the application to fetch the underlying media
 *         transport channel parameters. This is required by the application
 *         to make use of Assistive A2DP or similar feature, to configure
 *         the Bluetooth controller with the L2CAP CID and MTU values of
 *         the media channel, to enable decoding and rendering the media
 *         by the Bluetooth controller
 *
 *  \param [in] inst
 *         Codec Instance representing this Codec.
 *
 *  \param [out] params
 *         Pointer to the Media Channel Parameter structure
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \sa
 *  BT_avdtp_get_media_channel_params
 */
API_RESULT BT_a2dp_get_media_channel_params
           (
               /* IN */  UCHAR   inst,
               /* OUT */ A2DP_MEDIA_CHANNEL_PARAMS * params
           );
#endif /* AVDTP_ASSISTIVE_MODE */

#ifdef AVDTP_HAVE_CONTENT_PROTECTION
/**
 *  \brief To read the Content Protection Type.
 *
 *  \par Description:
 *       This API is used to read configured content protection type
 *       with a remote SEP entity.
 *
 *  \param [in] inst
 *         Codec Instance representing this Codec.
 *
 *  \param [out] cp_type
 *         Configured Content Protection Type.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \remarks
 */
API_RESULT BT_a2dp_get_configured_cp_type
           (
               /* IN */  UCHAR    inst,
               /* OUT */ UINT16 * cp_type
           );

/**
 *  \brief To set the content protection header
 *
 *  \par Description :
 *       This function is used to set the content protection header that will be
 *       used in the media frames.
 *
 *  \param [in] inst
 *         Codec Instance representing this Codec.
 *  \param [in] header
 *         The CP header to be set.
 *  \param [in] header_len
 *         Length of the CP header to be set
 *
 *  \return  API_RESULT: API_SUCCESS on success otherwise an error code
 *                       describing the cause of failure.
 */
API_RESULT BT_a2dp_set_cp_header
           (
               /* IN */  UCHAR     inst,
               /* IN */  UCHAR   * header,
               /* IN */  UINT16    header_len
           );

/**
 *
 *  \brief To Get the CP Type is set during A2DP connection.
 *
 *  \par Description:
 *       This function is used to get the Content Protection Type set durring
 *       the A2DP connection.
 *
 *  \param [in] inst
 *         A2DP instance
 *
 *  \param [out] cp_type
 *         Content Protection Type.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_a2dp_get_cp_type
           (
               /* IN */   UCHAR     inst,
               /* OUT */  UINT16  * cp_type
           );
#endif /* AVDTP_HAVE_CONTENT_PROTECTION */

/* Internal Function fo Activate/Inactivate SDP Record for A2DP Source/Sink */
/**
 *  \brief To update SDP Service Record for A2DP
 *
 *  \par Description:
 *       This function activates or inactivates SDP Record for A2DP Source/Sink
 *
 *  \param [in] service_type
 *         A2DP Service Type - Source or Sink
 *
 *  \param [in] flag
 *         0x01 - Activate SDP Record for Service Type specified
 *         0x00 - Inactivate SDP Record for Service Type specified
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT a2dp_update_sdp_record
           (
               /* IN */  UCHAR    service_type,
               /* IN */  UCHAR    flag
           );

#ifdef A2DP_1_3

/**
 *  \brief To set initial delay report.
 *
 *  \par Description:
 *  This function sets the initial delay report that will be sent from
 *  A2DP sink device during A2DP connection.
 *
 *  \param [in] inst
 *         Codec Instance representing this Codec.
 *
 *  \param [in] delay_report
 *         Initial Delay Report value to be sent during A2DP connection from
 *         A2DP Sink Device.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_a2dp_set_initial_delay_report
           (
               /* IN */ UCHAR   inst,
               /* IN */ INT16   delay_report
           );

/**
 *  \brief To send delay report.
 *
 *  \par Description:
 *  This function enables application to send delay report from A2DP sink device
 *  during streaming.
 *
 *  \param [in] inst
 *         Codec Instance representing this Codec.
 *
 *  \param [in] delay_report
 *         Delay Report value to be sent to remote A2DP source device.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \sa
 *  BT_avdtp_send_delay_report
 */
API_RESULT BT_a2dp_send_delay_report
           (
              /* IN */ UCHAR    inst,
              /* IN */ INT16   *delay_report
           );

#endif /* A2DP_1_3  */

#ifdef A2DP_HAVE_ABORT_INTERFACE
/**
 *  \brief To Abort Streaming establishment.
 *
 *  \par Description:
 *  This API is used to abort streaming establishment with a remote SEP entity.
 *
 *  \param [in] inst
 *         Codec Instance representing this Codec.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \remarks
 *   Use this interface only for PTS testing.
 *   In the normal scenarios, this interface is not required to be used.
 */
API_RESULT BT_a2dp_abort
           (
               /* IN */ UCHAR  inst
           );
#endif /* A2DP_HAVE_ABORT_INTERFACE */

#ifdef AVDTP_STREAM_NO_AUTORSP
/**
 *  \brief To respond to Media Streaming Command Start/Suspend.
 *
 *  \par Description:
 *  This API is used to respond to Media Streaming request from a peer SEP entity.
 *
 *  \param [in] inst
 *         Codec Instance representing this Codec.
 *
 *  \param [in] rsp_type
 *         Response type to send.
 *
 *  \param [in] rsp_code
 *         Response code for the operation.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \remarks
 */
API_RESULT BT_a2dp_send_rsp
           (
               /* IN */  UCHAR  inst,
               /* IN */  UCHAR  rsp_type,
               /* IN */  UCHAR  rsp_code
           );

/**
 *  \brief To accept Start Media Streaming.
 *
 *  \par Description:
 *  This API is used to accept Media Streaming Start request from a peer SEP entity.
 *
 *  \param [in] inst
 *         Codec Instance representing this Codec.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \remarks
 */
#define BT_a2dp_start_accept(inst) \
        BT_a2dp_send_rsp((inst), AVDTP_START_CNF, 0x00)

/**
 *  \brief To accept Suspend Media Streaming.
 *
 *  \par Description:
 *  This API is used to accept Media Streaming Suspend request from a peer SEP entity.
 *
 *  \param [in] inst
 *         Codec Instance representing this Codec.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \remarks
 */
#define BT_a2dp_suspend_accept(inst) \
        BT_a2dp_send_rsp((inst), AVDTP_SUSPEND_CNF, 0x00)

/**
 *  \brief To reject Start Media Streaming.
 *
 *  \par Description:
 *  This API is used to reject Media Streaming Start request from a peer SEP entity.
 *
 *  \param [in] inst
 *         Codec Instance representing this Codec.
 *
 *  \param [in] reason
 *         Reject reason code.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \remarks
 */
#define BT_a2dp_start_reject(inst, reason) \
        BT_a2dp_send_rsp((inst), AVDTP_START_CNF, (reason))

/**
 *  \brief To reject Suspend Media Streaming.
 *
 *  \par Description:
 *  This API is used to reject Media Streaming Suspend request from a peer SEP entity.
 *
 *  \param [in] inst
 *         Codec Instance representing this Codec.
 *
 *  \param [in] reason
 *         Reject reason code.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \remarks
 */
#define BT_a2dp_suspend_reject(inst, reason) \
        BT_a2dp_send_rsp((inst), AVDTP_SUSPEND_CNF, (reason))
#endif /* AVDTP_STREAM_NO_AUTORSP */
/** \} */
#ifdef __cplusplus
};
#endif
/** \} */
/** \} */
#endif /* _H_BT_A2DP_API_ */

