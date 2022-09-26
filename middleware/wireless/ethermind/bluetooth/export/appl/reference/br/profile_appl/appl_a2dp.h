
/**
 *  \file appl_a2dp.h
 *
 *  Header file for A2DP Command Line Test Application
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_A2DP_
#define _H_APPL_A2DP_

/* ----------------------------------------- Header File Inclusion */
#include "BT_hci_api.h"
#include "BT_sdp_api.h"
#include "BT_avdtp_api.h"
#include "BT_a2dp_api.h"
#include "BT_config.h"
#include "BT_jpl_api.h"
#include "BT_fops.h"

#ifndef BT_DRIVER_MODE

/* #define A2DP_USE_SBC */

#ifdef A2DP_USE_SBC
#include "sbc_api.h"
#ifdef USE_WAVE_AUDIO
#include "wa_recorder.h"
#endif /* USE_WAVE_AUDIO */
#endif /* A2DP_USE_SBC */
#endif  /* !BT_DRIVER_MODE */


/* ----------------------------------------- Global Definitions */

/* A2DP connection configuration files */
#define APPL_A2DP_CONFIG_BASE_FOLDER     BT_FOPS_PATH_JOIN("data","a2dp")
#define APPL_A2DP_CONFIG_FILE  \
        BT_FOPS_PATH_JOIN(APPL_A2DP_CONFIG_BASE_FOLDER,"a2dp_source.conf")

#define APPL_A2DP_SINK_PCM_FILE         "appl_a2dp_sink_sbc_to_raw.pcm"

/**
 * Application specific MTU size for A2DP sink
 *
 * NOTE:
 * For A2DP Sources that support AAC codec,
 * the MTU requirement may be large. For example, in
 * case of iDevices, the developer guide states the
 * MTU requirement of Sink devices to be minimum 885 bytes.
 */
#define APPL_A2DP_SNK_MTU               895U

/** Maximum Block Size of SBC Frame */
#define SBC_BLOCK_MAX                   512U

/**
 * For 512kb/s, Maximum SBC frame length  = 512/3
 *                                        = 170.6666
 *                                       ~= 172
 */
#define MAX_SBC_FRAME_LEN                172U

/**
 * Maximum No. of SBC Blocks that can be allocated by
 * the application. Set this parameter as per application
 * requirement.
 */
#define MAX_SBC_BLOCK_COUNT             8U

/*
 * Maximum Number of Remote A2DP Devices.
 *
 *  Minimum Value: 1
 *  Maximum Value: AVDTP_MAX_REMOTE_DEVICES
 */

#define APPL_A2DP_MAX_REMOTE_DEVICES     2U

/* A2DP Application Streaming Status Flags */
#define APPL_A2DP_STREAM_FALSE           0x00U
#define APPL_A2DP_STREAM_TRUE            0x01U

/* ----------------------------------------- Macros */

/* Application states */
#define APPL_A2DP_INITIALSED    0x01U
#define APPL_A2DP_INVALID_STATE 0xFFU

/* ----------------------------------------- Structures/ Data Types */
/* A2DP Codec Instances */
typedef struct
{
    /* SEP Capability */
    AVDTP_SEP_CAP sep_cap;

    /* A2DP Codec Instance */
    UCHAR a2dp_instance;

    /* A2DP Codec Type */
    UCHAR codec_type;

    /* A2DP Sink or Source */
    UCHAR role;

    /* Flag - Whether allocated or free */
    UCHAR allocated;

    /* A2DP Appl. stream state - TRUE or FALSE */
    UCHAR stream_state;

    /* Remote BD_ADDR */
    UCHAR bd_addr [BT_BD_ADDR_SIZE];

    /* Static/Resident Codec Capability IE Buffer */
    UCHAR codec_ie_cap [AVDTP_MAX_CODEC_IE_LEN];

    /* Static/Resident Codec Configuration IE Buffer */
    UCHAR codec_ie_conf [AVDTP_MAX_CODEC_IE_LEN];

    /* Maximum Media Payload Length (Rx./Tx.)*/
    UINT16  max_mpl;

#ifdef AVDTP_HAVE_CONTENT_PROTECTION
    /* Content Protection Type */
    UINT16 cp_type;

    /* Content Protection Header */
    UCHAR  *cp_hdr;

    /* Content Protection Header Length */
    UCHAR cp_hdr_len;
#endif /* AVDTP_HAVE_CONTENT_PROTECTION */
} A2DP_APPL_INSTANCE;

/* ----------------------------------------- Function Declarations */
/* A2DP Application Menu */
void main_a2dp_operations (void);

/* AVDTP Notification Callback Handler */
API_RESULT appl_a2dp_avdtp_notify_cb
           (
               UCHAR             event_type,
               UINT16            event_result,
               AVDTP_HANDLE *    avdtp_handle,
               void *            event_data,
               UINT16            event_datalen
           );

/* A2DP Notification Callback Handler */
API_RESULT appl_a2dp_notify_cb
           (
               UCHAR    codec_instance,
               UCHAR    event_type,
               UINT16   event_result,
               void  *  event_data,
               UINT16   event_datalen
           );

void appl_a2dp_register_codec (void);
void appl_a2dp_connect (void);
void appl_a2dp_start (void);
void appl_a2dp_suspend (void);
void appl_a2dp_reconfigure (UCHAR indx);
void appl_a2dp_disconnect (void);

void appl_a2dp_avdtp_connect (void);
void appl_a2dp_avdtp_discover (void);
void appl_a2dp_avdtp_get_capabilities (void);
void appl_a2dp_avdtp_disconnect (void);

#ifdef AVDTP_ASSISTIVE_MODE
void appl_a2dp_get_media_channel_params (void);
#endif /* AVDTP_ASSISTIVE_MODE */

void appl_a2dp_set_media_mtu (void);

void appl_a2dp_get_instance (UCHAR * indx);
void appl_a2dp_free_instance (UCHAR indx);
void appl_a2dp_alloc_instance (UCHAR * indx);
void appl_a2dp_show_instance (void);

API_RESULT appl_a2dp_get_acl_connection (UCHAR * indx);

INT32 appl_a2dp_read_sbc_configuration
      (
          CHAR *     filename,
          UCHAR *    codec_ie_conf
      );

#ifdef A2DP_MPEG_1_2

INT32 appl_a2dp_read_mpeg12_configuration
      (
          CHAR *     filename,
          UCHAR *    codec_ie
      );

#endif  /* A2DP_MPEG_1_2 */

#ifdef A2DP_MPEG_2_4

INT32 appl_a2dp_read_mpeg24_configuration
      (
          CHAR *     filename,
          UCHAR *    codec_ie
      );

#endif  /* A2DP_MPEG_2_4 */

#ifdef AVDTP_HAVE_CONTENT_PROTECTION
void appl_a2dp_set_cp_header(void);
#endif /* AVDTP_HAVE_CONTENT_PROTECTION */

void appl_a2dp_connect_info_reset(/* IN */ UCHAR inst);

API_RESULT appl_a2dp_media_write(void);

API_RESULT appl_a2dp_get_sbc_frame_details
            (
                UCHAR *sbc_config_info,
                UCHAR *sbc_frame_hdr,
                UINT16 *sbc_frame_len
            );

INT32 appl_a2dp_read_codec_ie_configuration
      (
          /* IN */  CHAR  * filename,
          /* IN */  UCHAR   codec_type,
          /* OUT */ UCHAR * codec_ie
      );

#if 0
INT32 appl_a2dp_read_codec_configuration
      (
          /* IN */  CHAR  * filename,
          /* IN */  UCHAR   codec_type,
          /* OUT */ UCHAR * codec_ie
      );
#endif /* 0 */

void appl_a2dp_save_codec_ie_configuration
      (
          /* IN */  UCHAR   role,
          /* IN */  UCHAR   codec_type,
          /* IN */  UCHAR * codec_ie,
          /* IN */  UINT16  codec_ie_len
      );

void appl_a2dp_print_codec_ie_configuration
      (
          /* IN */  UCHAR   codec_type,
          /* IN */  UCHAR * codec_ie,
          /* IN */  UINT16  codec_ie_len
      );

void appl_a2dp_print_codec_ie_capabilities
     (
         /* IN */  UCHAR   codec_type,
         /* IN */  UCHAR * codec_ie,
         /* IN */  UINT16  codec_ie_len
     );

#ifdef AVDTP_HAVE_GET_ALL_CAPABILITIES
void appl_a2dp_avdtp_get_all_capabilities(void);
#endif /* AVDTP_HAVE_GET_ALL_CAPABILITIES */

#ifdef A2DP_1_3
void appl_a2dp_set_initial_delay_report(UCHAR inst);
void appl_a2dp_send_delay_report(UCHAR inst);
#endif /* A2DP_1_3 */

#ifdef HAVE_AVDTP_TEST_MODE
void appl_avdtp_set_sig_msg_reject_rsp_params(void);
#endif /* HAVE_AVDTP_TEST_MODE */

#endif /* _H_APPL_A2DP_ */

