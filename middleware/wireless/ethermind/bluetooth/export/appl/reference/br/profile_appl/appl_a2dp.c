
/**
 *  \file appl_a2dp.c
 *
 *  Source file for A2DP Source Command Line Test Application
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_a2dp.h"
#include "a2dp_pl.h"
#include "appl_utils.h"

#ifdef A2DP

#define APPL_A2DP_DEFAULT_CONFIG

/* ----------------------------------------- Exported Global Variables */


/* ----------------------------------------- Static Global Variables */
/* Main A2DP Application Menu */
#ifdef AVDTP_ASSISTIVE_MODE
static const char a2dp_main_menu[] = " \n\
--------------- A2DP Menu ---------------------- \n\n\
  0.  Exit. \n\
  1.  Refresh. \n\
 \n\
  2.  A2DP Initialize. \n\
  3.  A2DP Shutdown. \n\
 \n\
  4.  A2DP Register Codec. \n\
 \n\
  5.  A2DP Connect. \n\
  6.  A2DP Start. \n\
  7.  A2DP Suspend. \n\
  8.  A2DP Reconfigure. \n\
  9.  A2DP Disconnect. \n\
 \n\
  10. AVDTP Connect. \n\
  11. AVDTP Discover. \n\
  12. AVDTP Get Capabilities. \n\
  13. AVDTP Get All Capabilities. \n\
  14. AVDTP Disconnect. \n\
  \n\
  15. A2DP Set Initial Delay Report. \n\
  16. A2DP Send Delay Report. \n\
  \n\
  17. Create ACL connection\n\
  18. Release ACL connection\n\
  19. Register Peer BD Address\n\
 \n\
  20. A2DP Set Media MTU. \n\
  21. A2DP Get Media Channel Params. \n\
 \n\
  30. A2DP Set Content Protection header. \n\
 \n\
  40. A2DP Show Instances. \n\
 \n\
  50. Write Flush Timeout. \n\
 \n\
  70. AVDTP Set Signalling msg reject rsp params. \n\
 \n\
Your Option -> ";
#else /* AVDTP_ASSISTIVE_MODE */
static const char a2dp_main_menu[] = " \n\
--------------- A2DP Menu ---------------------- \n\n\
  0.  Exit. \n\
  1.  Refresh. \n\
 \n\
  2.  A2DP Initialize. \n\
  3.  A2DP Shutdown. \n\
 \n\
  4.  A2DP Register Codec. \n\
 \n\
  5.  A2DP Connect. \n\
  6.  A2DP Start. \n\
  7.  A2DP Suspend. \n\
  8.  A2DP Reconfigure. \n\
  9.  A2DP Disconnect. \n\
 \n\
  10. AVDTP Connect. \n\
  11. AVDTP Discover. \n\
  12. AVDTP Get Capabilities. \n\
  13. AVDTP Get All Capabilities. \n\
  14. AVDTP Disconnect. \n\
  \n\
  15. A2DP Set Initial Delay Report. \n\
  16. A2DP Send Delay Report. \n\
  \n\
  17. Create ACL connection\n\
  18. Release ACL connection\n\
  19. Register Peer BD Address\n\
 \n\
  20. A2DP Set Media MTU. \n\
 \n\
  30. A2DP Set Content Protection header. \n\
 \n\
  40. A2DP Show Instances. \n\
 \n\
  50. Write Flush Timeout. \n\
 \n\
  70. AVDTP Set Signalling msg reject rsp params. \n\
 \n\
Your Option -> ";
#endif

/* HCI ACL Connection List */
static HCI_CONNECTION_LIST appl_a2dp_acl_list [BT_MAX_REMOTE_DEVICES];

/* AVDTP Handle */
static AVDTP_HANDLE appl_a2dp_avdtp_handle[APPL_A2DP_MAX_REMOTE_DEVICES];

/* A2DP Codec Instances */
static A2DP_APPL_INSTANCE appl_a2dp_instance [A2DP_MAX_CODECS];

/* Buffer for AVDTP Discover Response */
static UCHAR appl_a2dp_sep_info_buf [10U];

/* Buffer for AVDTP Get Capability Response */
static UCHAR appl_a2dp_sep_cap_buf [50U];

/*Peer device BD Address*/
static UCHAR a2dp_bd_addr[BT_BD_ADDR_SIZE];

/* Buffer for A2DP Delay Report Data */
#if ((defined A2DP_1_3) && (defined A2DP_SINK))
static INT16 appl_a2dp_delay_report;
#endif /* A2DP_1_3 && A2DP_SINK */

static UCHAR appl_a2dp_peer_dr_cap_flag;

/* A2DP Codec Information Element for SBC */
static UCHAR appl_a2dp_sbc_codec_ie [A2DP_SBC_CODEC_CAP_IE_LEN];

#ifdef A2DP_MPEG_1_2
/* A2DP Codec Information Element for MPEG-1,2 */
static UCHAR appl_a2dp_mpeg12_codec_ie [A2DP_MPEG_1_2_CODEC_CAP_IE_LEN];
#endif /* A2DP_MPEG_1_2 */

#ifdef A2DP_MPEG_2_4
/* A2DP Codec Information Element for MPEG-2,4 */
static UCHAR appl_a2dp_mpeg24_codec_ie [A2DP_MPEG_2_4_CODEC_CAP_IE_LEN];
#endif /* A2DP_MPEG_2_4 */

#ifdef A2DP_VENDOR_CODEC
/* A2DP Codec Information Element for Vendor Specific Codec */
static UCHAR appl_a2dp_vendor_codec_ie [AVDTP_MAX_CODEC_IE_LEN];
#endif /* A2DP_VENDOR_CODEC */

#ifdef AVDTP_ASSISTIVE_MODE
static A2DP_MEDIA_CHANNEL_PARAMS appl_a2dp_media_ch_params;
#endif /* AVDTP_ASSISTIVE_MODE */

#ifdef AVDTP_HAVE_CONTENT_PROTECTION
static UCHAR * cp_ie_buf = NULL;
static UCHAR   cp_ie_buf_len = 0U;
static UCHAR   a2dp_cp_header [1U];
static UINT16  a2dp_cp_header_len;
#endif /* AVDTP_HAVE_CONTENT_PROTECTION */

#ifdef A2DP_SINK

#ifdef INCLUDE_JPL
static JPL_PARAM jpl_param;
static UCHAR * appl_a2dp_jpl_pof;
static UINT16 appl_a2dp_jpl_poflen;
#endif /* INCLUDE_JPL */

/* Current instance being played back */
static UCHAR appl_a2dp_playback_instance;

static void appl_a2dp_jpl_configure(UCHAR * codec_ie);

#ifdef INCLUDE_JPL
static API_RESULT jpl_callback_handle
                  (
                      /* IN */ UCHAR   event_type,
                      /* IN */ UCHAR * event_data,
                      /* IN */ UINT16  event_datalen
                  );
#endif /* INCLUDE_JPL */

static void a2dp_pl_snk_callback
            (
                /* IN */ const UCHAR *  p_data,
                /* IN */ UINT16         p_datalen
            );
#endif /* A2DP_SINK */

#ifdef A2DP_SOURCE
#define APP_A2DP_SRC_WR_TH_INIT                 0x01U
#define APP_A2DP_SRC_WR_TH_STOP                 0x02U
#define APP_A2DP_SRC_WR_TH_PLAYING              0x03U

#define A2DP_SRC_SBC_BLOCK_MAX                  512U

/**
 * A2DP Src PCM Queue Size, should be a power of 2 value.
 * This value shall be at least double of the data length being equeued every time (app_a2dp_src_enqueue).
 */
#define A2DP_SRC_MAX_BUFFER_SIZE                (4096U * 2U)

/* Write thread state */
static UINT8 a2dp_src_wr_th_state;

/* Buffers and Rd/Wr indices */
static INT32 a2dp_src_buffer_rd_ptr;
static INT32 a2dp_src_buffer_wr_ptr;
static INT32 a2dp_src_buffer_size;
static UCHAR a2dp_src_buffer[A2DP_SRC_MAX_BUFFER_SIZE];
static UCHAR pcm_to_send[A2DP_SRC_SBC_BLOCK_MAX];

/* Synchronization for thread */
BT_DEFINE_MUTEX_TYPE (static, a2dp_src_th_mutex)
BT_DEFINE_COND_TYPE (static, a2dp_src_th_cond)

#ifdef INCLUDE_SBC
/* SBC Encoder Input/Output Structure */
static SBC_ENCODER_IO_STRUCT a2dp_sbc_encoder_io;
#endif /* INCLUDE_SBC */

/* Size of PCM Data used for encoding one SBC frame */
static UINT16 appl_a2dp_pcm_datalen;

/* Encoder initialized state */
static UCHAR appl_a2dp_sbc_encoder_state;

/* A2dp state */
static UCHAR appl_a2dp_state = APPL_A2DP_INVALID_STATE;

/* Buffer for converting UCHAR pcm stream to UINT16 pcm samples */
static UINT16 appl_a2dp_pcm_data_le [SBC_BLOCK_MAX >> 1U];

#if (defined A2DP_USE_SBC && defined A2DP_SOURCE)
/* Maximum Allowed Payload Length */
static UINT16 a2dp_max_allowed_payload_len;
#endif /* (defined A2DP_USE_SBC && defined A2DP_SOURCE) */

/* Buffer used to hold encoded SBC data */
#ifndef A2DP_SUPPORT_MULTIPLE_MEDIA_FRAME_WRITE
static UCHAR appl_a2dp_sbc_data[SBC_BLOCK_MAX];
#else
/* Allocating MAX_SBC_BLOCK_COUNT times of SBC_BLOCK_MAX */
static UCHAR appl_a2dp_sbc_data[SBC_BLOCK_MAX * MAX_SBC_BLOCK_COUNT];

/* A2DP Source SBC Media Frames Count */
static UCHAR appl_a2dp_src_sbc_frame_cnt;

/* A2DP Source SBC encoded data len */
static UINT16 appl_a2dp_src_sbc_data_len;

/* A2DP Source Max. SBC payload len */
static UINT16  appl_a2dp_src_max_mpl;
#endif /* A2DP_SUPPORT_MULTIPLE_MEDIA_FRAME_WRITE */

void appl_a2dp_src_prepare(void);

API_RESULT app_a2dp_src_enqueue
           (
               /* IN */  const UCHAR   * data,
               /* IN */  UINT16          datalen
           );

DECL_STATIC BT_THREAD_RETURN_TYPE a2dp_src_write_task(BT_THREAD_ARGS args);

API_RESULT a2dp_encode_n_send
           (
               UINT8    a2dp_instance,
               UCHAR  * pcm_data,
               UINT16   pcm_datalen
           );

static void a2dp_pl_src_callback
            (
                /* IN */ const UCHAR *  r_data,
                /* IN */ UINT16         r_datalen
            );

/*
 * Sample SBC Encoded Frame with SBC encoder configuration parameters as below
 *
 * Sampling Frequency = 44100
 * Channel Mode       = Joint Stereo
 * Block Length       = 16
 * Subbands           = 8
 * Allocation Method  = Loudness
 * Bitpool            = 56
 *
 */
static UCHAR appl_sbc_data [MAX_SBC_FRAME_LEN] =
{
    0x9CU, 0xBDU, 0x38U, 0xA2U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x20U,
    0x00U, 0x00U, 0x00U, 0x6DU, 0xB6U, 0xDBU, 0x7FU, 0xEDU, 0xB6U, 0xDBU,
    0x6DU, 0xB6U, 0xDBU, 0x7FU, 0xEDU, 0xB6U, 0xDBU, 0x6DU, 0xB6U, 0xDBU,
    0x7FU, 0xEDU, 0xB6U, 0xDBU, 0x6DU, 0xB6U, 0xDBU, 0x7FU, 0xEDU, 0xB6U,
    0xDBU, 0x6DU, 0xB6U, 0xDBU, 0x7FU, 0xEDU, 0xB6U, 0xDBU, 0x6DU, 0xB6U,
    0xDBU, 0x7FU, 0xEDU, 0xB6U, 0xDBU, 0x6DU, 0xB6U, 0xDBU, 0x7FU, 0xEDU,
    0xB6U, 0xDBU, 0x6DU, 0xB6U, 0xDBU, 0x7FU, 0xEDU, 0xB6U, 0xDBU, 0x6DU,
    0xB6U, 0xDBU, 0x7FU, 0xEDU, 0xB6U, 0xDBU, 0x6DU, 0xB6U, 0xDBU, 0x7FU,
    0xEDU, 0xB6U, 0xDBU, 0x6DU, 0xB6U, 0xDBU, 0x7FU, 0xEDU, 0xB6U, 0xDBU,
    0x6DU, 0xB6U, 0xDBU, 0x7FU, 0x8DU, 0xB6U, 0xDBU, 0x6DU, 0xB6U, 0xDBU,
    0x7EU, 0x0DU, 0xB6U, 0xDBU, 0x6DU, 0xB6U, 0xDBU, 0x76U, 0x0DU, 0xB6U,
    0xDBU, 0x6DU, 0xB6U, 0xDBU, 0x6BU, 0xCDU, 0xB6U, 0xDBU, 0x4DU, 0xB6U,
    0xDBU, 0x22U, 0xACU, 0x95U, 0x24U,
    /* MISRA C-2012 Rule 9.3 */
    0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
    0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
    0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
    0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
    0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U
};
#endif /* A2DP_SOURCE */


/* ----------------------------------------- Functions */

void main_a2dp_operations (void)
{
    int choice, menu_choice;
    API_RESULT retval;
    UCHAR indx;
    UINT16 hci_handle;

#ifdef A2DP_SINK
#ifdef JPL_USE_APP_MEMORY
    JPL_BUFFERS buffer;
#endif /* JPL_USE_APP_MEMORY */
#endif /* A2DP_SINK */

    static UCHAR first_time = 0x0U;

    if (0x0U == first_time)
    {
#ifdef A2DP_SOURCE
        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
        (void) BT_thread_mutex_init(&a2dp_src_th_mutex, NULL);
        (BT_IGNORE_RETURN_VALUE) BT_thread_cond_init(&a2dp_src_th_cond, NULL);

        /* Initialize the task variables and create the task */
        a2dp_src_wr_th_state = APP_A2DP_SRC_WR_TH_INIT;

        a2dp_src_buffer_size = A2DP_SRC_MAX_BUFFER_SIZE;
        a2dp_src_buffer_rd_ptr = 0U;
        a2dp_src_buffer_wr_ptr = 0U;

        /* Create the A2DP task */
        (BT_IGNORE_RETURN_VALUE) a2dp_create_task_pl(a2dp_src_write_task);
#endif /* A2DP_SOURCE */

        first_time = 0x1U;
    }

    BT_LOOP_FOREVER()
    {
        printf("%s", a2dp_main_menu); fflush(stdin);
        scanf("%d", &choice);

        menu_choice = choice;

        switch (choice)
        {
        case 0:
            break; /* return; */

        case 1:
            break;

        case 2:
        	LOG_DEBUG("Initializing A2DP ... "); fflush (stdout);
            LOG_DEBUG("appl_debug_enabled is %d\n", appl_debug_enabled);
            retval = BT_a2dp_init();
            if (API_SUCCESS != retval)
            {
            	LOG_DEBUG("FAILED. Reason = 0x%04X\n", retval);
            }
            else
            {
            	LOG_DEBUG("OK\n");
#if (defined A2DP_USE_SBC && defined A2DP_SOURCE)
                a2dp_max_allowed_payload_len = 0U;
#endif /* (defined A2DP_USE_SBC && defined A2DP_SOURCE) */
                for (choice = 0U; choice < A2DP_MAX_CODECS; choice ++)
                {
                    appl_a2dp_free_instance ((UCHAR)choice);
                }

#ifdef A2DP_SUPPORT_MULTIPLE_MEDIA_FRAME_WRITE
                /* Init */
                appl_a2dp_src_max_mpl = 0xFFFFU;
                appl_a2dp_src_sbc_frame_cnt = 0x00U;
                appl_a2dp_src_sbc_data_len  = 0x00U;
#endif /* A2DP_SUPPORT_MULTIPLE_MEDIA_FRAME_WRITE */

                /* Initialize the playback instance */
                appl_a2dp_playback_instance = A2DP_MAX_CODECS;
            }

            /* Register AVDTO Async Event Indication Callback */
            (BT_IGNORE_RETURN_VALUE) BT_avdtp_callback_register (appl_a2dp_avdtp_notify_cb);

#ifdef A2DP_SINK
#ifdef INCLUDE_JPL
            /* Initialize JPL */
            (BT_IGNORE_RETURN_VALUE) BT_jpl_init(jpl_callback_handle);
#endif /* INCLUDE_JPL */

#ifdef JPL_USE_APP_MEMORY
            buffer.sbc = a2dp_alloc_buffer_pl(A2DP_SBC_BUFFER);
            buffer.pcm = a2dp_alloc_buffer_pl(A2DP_PCM_BUFFER);
            buffer.silence = a2dp_alloc_buffer_pl(A2DP_SILENCE_BUFFER);

            /* Register memory to JPL */
            (BT_IGNORE_RETURN_VALUE) BT_jpl_register_buffers(&buffer);
#endif /* JPL_USE_APP_MEMORY */
#endif /* A2DP_SINK */
            appl_a2dp_state = APPL_A2DP_INITIALSED;
            break;

        case 3:
        	LOG_DEBUG("Shutdown A2DP ... "); fflush (stdout);
            if((appl_a2dp_state != APPL_A2DP_INVALID_STATE) ||
               (appl_a2dp_state == APPL_A2DP_INITIALSED))
            {
                retval = BT_a2dp_shutdown();
                if (API_SUCCESS != retval)
                {
                	LOG_DEBUG("FAILED. Reason = 0x%04X\n", retval);
                }
                else
                {
                	LOG_DEBUG("OK\n");

                    for (choice = 0U; choice < A2DP_MAX_CODECS; choice ++)
                    {
                        appl_a2dp_free_instance ((UCHAR)choice);
                    }

                    appl_a2dp_state = APPL_A2DP_INVALID_STATE;
                }
            }
            else
            {
            	LOG_DEBUG("Invalid A2dp state 0x%02X\n",appl_a2dp_state);
            }

            break;

        case 4:
            appl_a2dp_register_codec();
            break;

        case 5:
            appl_a2dp_connect();
            break;

        case 6:
            appl_a2dp_start();
            break;

        case 7:
            appl_a2dp_suspend();
            break;

        case 8:
            /* Choose an Instance */
            appl_a2dp_get_instance (&indx);
            if (indx < A2DP_MAX_CODECS)
            {
                appl_a2dp_reconfigure(indx);
            }
            break;

        case 9:
            appl_a2dp_disconnect();
            break;

        case 10:
            appl_a2dp_avdtp_connect();
            break;

        case 11:
            appl_a2dp_avdtp_discover();
            break;

        case 12:
            appl_a2dp_avdtp_get_capabilities();
            break;

        case 13:
#ifdef AVDTP_HAVE_GET_ALL_CAPABILITIES
            appl_a2dp_avdtp_get_all_capabilities();
#else
            LOG_DEBUG ("Please Enable AVDTP_HAVE_GET_ALL_CAPABILITIES Flag\n");
#endif  /* AVDTP_HAVE_GET_ALL_CAPABILITIES */
            break;

        case 14:
            appl_a2dp_avdtp_disconnect();
            break;

#ifdef A2DP_1_3
        case 15:
#ifdef A2DP_SINK
            /* Choose Instance */
            appl_a2dp_get_instance (&indx);
            if (indx < A2DP_MAX_CODECS)
            {
                appl_a2dp_set_initial_delay_report(indx);
            }
#else
            LOG_DEBUG ("Please Enable A2DP_SINK Flag\n");
#endif /* A2DP_SINK */
            break;

        case 16:
#ifdef A2DP_SINK
            /* Choose Instance */
            appl_a2dp_get_instance (&indx);
            if (indx < A2DP_MAX_CODECS)
            {
                appl_a2dp_send_delay_report(indx);
            }


#else
            LOG_DEBUG ("Please Enable A2DP_SINK Flag\n");
#endif /* A2DP_SINK */
            break;
#else
            LOG_DEBUG ("Please Enable A2DP_1_3 Flag \n");
#endif  /* A2DP_1_3 */
        case 17:
        	LOG_DEBUG("\n");
        	LOG_DEBUG("Starting ACL Connection ... "); fflush(stdout);

            retval = BT_hci_create_connection
                     (
                         a2dp_bd_addr,
                         LMP_ACL_DH5,
                         0x0U,
                         0x0U,
                         0U,
                         1U
                     );

            if (API_SUCCESS != retval)
            {
            	LOG_DEBUG("FAILED !! Error Code = 0x%04x\n", retval);
            }
            else
            {
            	LOG_DEBUG("Successfully started.\n");
            	LOG_DEBUG("Please wait for Connect Complete.\n");
            }
            break;

        case 18:
            retval = BT_hci_get_acl_connection_handle
                     (
                         a2dp_bd_addr,
                         &hci_handle
                     );

            if (API_SUCCESS == retval)
            {
                LOG_DEBUG("Disconnecting ACL Connection 0x%04X\n",
                hci_handle);
            }
            else
            {
                LOG_DEBUG ("ACL Connection for A2DP not found\n");
                LOG_DEBUG("Enter ACL Handle\n");
                scanf("%x",&choice);
                hci_handle = (UINT16)choice;
            }

            retval = BT_hci_disconnect
                         (
                             hci_handle,
                             0x13U
                         );

            LOG_DEBUG("> API RETVAL ACL disconnect : 0x%04X\n", retval);
            break;

        case 19:
            LOG_DEBUG("Enter Remote BD_ADDR = "); fflush(stdout);
            appl_get_bd_addr(a2dp_bd_addr);
            break;

        case 20:
            appl_a2dp_set_media_mtu();
            break;

        case 21:
#ifdef AVDTP_ASSISTIVE_MODE
            /* Call this after a2dp connection */
            appl_a2dp_get_media_channel_params();
#else /* AVDTP_ASSISTIVE_MODE */
            LOG_DEBUG("Assistive A2DP feature is not supported for the platform.\n");
#endif /* AVDTP_ASSISTIVE_MODE */
            break;

        case 30:
#ifdef AVDTP_HAVE_CONTENT_PROTECTION
            appl_a2dp_set_cp_header();
#else
            LOG_DEBUG ("Please Enable AVDTP_HAVE_CONTENT_PROTECTION \
                     Flag to use this option \n");
#endif /* AVDTP_HAVE_CONTENT_PROTECTION */
            break;

        case 40:
            appl_a2dp_show_instance();
            break;

        case 50:
            retval = appl_a2dp_get_acl_connection (&indx);
            if (API_SUCCESS != retval)
            {
                break;
            }

            (BT_IGNORE_RETURN_VALUE) BT_hci_write_automatic_flush_timeout
            (
                appl_a2dp_acl_list[indx].acl_handle,
                0x00A0U
            );

            break;

         case 70:
#ifdef HAVE_AVDTP_TEST_MODE
             appl_avdtp_set_sig_msg_reject_rsp_params();
#else
             LOG_DEBUG("HAVE_AVDTP_TEST_MODE feature flag not defined\n");
#endif /* HAVE_AVDTP_TEST_MODE */
             break;

        default:
            LOG_DEBUG("Invalid Choice. Try Again\n");
            break;
        }

        if (0 == menu_choice)
        {
            /* return */
            break;
        }

    } /* BT_LOOP_FOREVER() */

    return;
}

void appl_a2dp_register_codec (void)
{
    UCHAR * codec_ie_cap;
    API_RESULT retval;
    UCHAR indx, a2dp_instance;
    UCHAR sep_type, codec_type, codec_ie_len;
    UINT32 read_val;

    /* Init */
    retval = API_SUCCESS;
    codec_ie_len = 0;

    /* Allocate an Instance */
    appl_a2dp_alloc_instance (&indx);
    if (indx >= A2DP_MAX_CODECS)
    {
        LOG_DEBUG("*** No Free Codec Instance Available\n");
        /* return; */
    }
    else
    {
        /* Set Stream End Point Type - AVDTP Source or Sink */
        LOG_DEBUG("\n");
        LOG_DEBUG("Select Streaming End Point Type.  Source(0)/Sink(1)... "); fflush(stdout);

        scanf ("%d", &read_val);

        if (AVDTP_SEP_SOURCE == read_val)
        {
            sep_type = AVDTP_SEP_SOURCE;

            /* Activate A2DP Source SDP Record */
            (BT_IGNORE_RETURN_VALUE) BT_a2dp_activate_sdp_record_source();
        }
        else
        {
            sep_type = AVDTP_SEP_SINK;

            /* Activate A2DP Sink SDP Record */
            (BT_IGNORE_RETURN_VALUE) BT_a2dp_activate_sdp_record_sink();
        }

        /* Set Codec Type. SBC/MP3/AAC */
        LOG_DEBUG("\n");
        LOG_DEBUG ("Select Codec Type(in Hex):\n");
        LOG_DEBUG ("    0->SBC\n");
#ifdef A2DP_MPEG_1_2
        LOG_DEBUG ("    1->MPEG-1,2(MP3)\n");
#endif /* A2DP_MPEG_1_2 */
#ifdef A2DP_MPEG_2_4
        LOG_DEBUG ("    2->MPEG-2,4(AAC)\n");
#endif /* A2DP_MPEG_2_4 */
#ifdef A2DP_VENDOR_CODEC
        LOG_DEBUG ("   FF->Vendor Specific\n");
#endif /* A2DP_VENDOR_CODEC */
        LOG_DEBUG ("\n>-*** NOTE: It is MANDATORY to have atleast ONE instance of "
                "SBC codec registered as per the A2DP specification ***-<\n");

        scanf ("%x", &read_val);

        /* Set Codec Type */
        if ((read_val > AVDTP_CODEC_AUDIO_MPEG_2_4) &&
            (AVDTP_CODEC_AUDIO_VENDOR_SPECIFIC != read_val))
        {
            /* Set default codec as SBC */
            codec_type = AVDTP_CODEC_AUDIO_SBC;
        }
        else
        {
            codec_type = (UCHAR)read_val;
        }

        /* Get Codec IE */
        codec_ie_cap = appl_a2dp_instance[indx].codec_ie_cap;

        if (AVDTP_CODEC_AUDIO_SBC == codec_type)
        {
            /* Initialize Codec IE */
            /* Set Support for SBC Sampling Freqn & Channel Modes */
            codec_ie_cap[0U] = A2DP_SBC_SAMPLING_FREQUENCY_16000 |
                              A2DP_SBC_SAMPLING_FREQUENCY_32000 |
                              A2DP_SBC_SAMPLING_FREQUENCY_44100 |
                              A2DP_SBC_SAMPLING_FREQUENCY_48000 |
                              A2DP_SBC_CHANNEL_MONO |
                              A2DP_SBC_CHANNEL_DUAL |
                              A2DP_SBC_CHANNEL_STEREO |
                              A2DP_SBC_CHANNEL_JOINT_STEREO;

            /* Set Support for SBC Block Len, Subbands & Allocation Method */
            codec_ie_cap[1U] = A2DP_SBC_BLOCK_LENGTH_4 |
                              A2DP_SBC_BLOCK_LENGTH_8 |
                              A2DP_SBC_BLOCK_LENGTH_12 |
                              A2DP_SBC_BLOCK_LENGTH_16 |
                              A2DP_SBC_SUBBANDS_4 |
                              A2DP_SBC_SUBBANDS_8 |
                              A2DP_SBC_ALLOCATION_METHOD_SNR |
                              A2DP_SBC_ALLOCATION_METHOD_LOUDNESS;

            /* Set Support for Minimum Bitpool */
            codec_ie_cap[2U] = A2DP_SBC_BITPOOL_MIN;

            /* Set Support for Maximum Bitpool */
            codec_ie_cap[3U] = A2DP_SBC_BITPOOL_MAX;

            /* Set Codec IE Length */
            codec_ie_len = A2DP_SBC_CODEC_CAP_IE_LEN;
        }
#ifdef A2DP_MPEG_1_2
        else if (AVDTP_CODEC_AUDIO_MPEG_1_2 == codec_type)
        {
            UINT16 bit_rate_index;

            /* Initialize Codec IE */
            /* Update MPEG_1_2 Layer, CRC, Channel Mode */
            codec_ie_cap[0U] = A2DP_MPEG_1_2_LAYER_I             |
                              A2DP_MPEG_1_2_LAYER_II            |
                              A2DP_MPEG_1_2_LAYER_III           |
                              A2DP_MPEG_1_2_CRC_ON              |
                              A2DP_MPEG_1_2_CHANNEL_MONO        |
                              A2DP_MPEG_1_2_CHANNEL_DUAL        |
                              A2DP_MPEG_1_2_CHANNEL_STEREO      |
                              A2DP_MPEG_1_2_CHANNEL_JOINT_STEREO;

            /* Update Media Payload Format(MPF), Sampling Frequency */
            codec_ie_cap[1U] = A2DP_MPEG_1_2_MPF_1                    |
                              A2DP_MPEG_1_2_SAMPLING_FREQUENCY_16000 |
                              A2DP_MPEG_1_2_SAMPLING_FREQUENCY_22050 |
                              A2DP_MPEG_1_2_SAMPLING_FREQUENCY_24000 |
                              A2DP_MPEG_1_2_SAMPLING_FREQUENCY_32000 |
                              A2DP_MPEG_1_2_SAMPLING_FREQUENCY_44100 |
                              A2DP_MPEG_1_2_SAMPLING_FREQUENCY_48000;

            /* Form Supported Bit Rate indexes */
            bit_rate_index = A2DP_MPEG_1_2_BITRATE_INDEX_0  |
                             A2DP_MPEG_1_2_BITRATE_INDEX_1  |
                             A2DP_MPEG_1_2_BITRATE_INDEX_2  |
                             A2DP_MPEG_1_2_BITRATE_INDEX_3  |
                             A2DP_MPEG_1_2_BITRATE_INDEX_4  |
                             A2DP_MPEG_1_2_BITRATE_INDEX_5  |
                             A2DP_MPEG_1_2_BITRATE_INDEX_6  |
                             A2DP_MPEG_1_2_BITRATE_INDEX_7  |
                             A2DP_MPEG_1_2_BITRATE_INDEX_8  |
                             A2DP_MPEG_1_2_BITRATE_INDEX_9  |
                             A2DP_MPEG_1_2_BITRATE_INDEX_10 |
                             A2DP_MPEG_1_2_BITRATE_INDEX_11 |
                             A2DP_MPEG_1_2_BITRATE_INDEX_12 |
                             A2DP_MPEG_1_2_BITRATE_INDEX_13 |
                             A2DP_MPEG_1_2_BITRATE_INDEX_14;

            /* Update Bit Rate Index  */
            codec_ie_cap[3U] = (UCHAR )bit_rate_index;

            /* Update Variable Bit Rate(VBR) and Bit Rate Index */
            codec_ie_cap[2U] = (A2DP_MPEG_1_2_VBR_ON | (bit_rate_index >> 8U));

            /* Set Codec IE Length */
            codec_ie_len = A2DP_MPEG_1_2_CODEC_CAP_IE_LEN;
        }
#endif /* A2DP_MPEG_1_2 */
#ifdef A2DP_MPEG_2_4
        else if (AVDTP_CODEC_AUDIO_MPEG_2_4 == codec_type)
        {
            UINT16 sampling_frequency;
            UINT32 bit_rate;

            /* Initialize Codec IE */
            /* Update MPEG_2_4 Object Type */
            codec_ie_cap[0U] = A2DP_MPEG_2_4_OBJ_TYPE_MPEG2_AAC_LC;

            /* Update sampling frequency, channels */
            sampling_frequency = A2DP_MPEG_2_4_SAMPLING_FREQUENCY_44100 |
                                 A2DP_MPEG_2_4_SAMPLING_FREQUENCY_48000 |
                                 A2DP_MPEG_2_4_CHANNELS_1  |
                                 A2DP_MPEG_2_4_CHANNELS_2;

            codec_ie_cap[1U] = (UCHAR )(sampling_frequency >> 8U);

            codec_ie_cap[2U] = (UCHAR )sampling_frequency;

            /* Update the VBR, Bit rate */
            /*
             * Note: If VBR is set, then bit rate correspond to peak bit rate
             *       else it correspond constant bit rate.
             */
            bit_rate = A2DP_MPEG_2_4_BITRATE_UNKNOWN;

            codec_ie_cap[3U] = (((UCHAR )(bit_rate >> 16U)) | A2DP_MPEG_2_4_VBR_ON);

            codec_ie_cap[4U] = (UCHAR )(bit_rate >> 8U);

            codec_ie_cap[5U] = (UCHAR )(bit_rate);

            /* Set Codec IE Length */
            codec_ie_len = A2DP_MPEG_2_4_CODEC_CAP_IE_LEN;
        }
#endif /* A2DP_MPEG_2_4 */
#ifdef A2DP_VENDOR_CODEC
        else if (AVDTP_CODEC_AUDIO_VENDOR_SPECIFIC == codec_type)
        {
            int       val;
            UINT32    vendor_id;
            UINT16    vendor_codec_id, i;

            LOG_DEBUG ("\n=== Enter Vendor Specific Codec Info. ===\n\n");

            LOG_DEBUG ("Enter Vendor ID(in Hex): ");
            scanf ("%x", &val);

            vendor_id = (UINT32)val;

            /* pack vendor id */
            BT_PACK_LE_4_BYTE_VAL (&codec_ie_cap[0U], vendor_id);

            LOG_DEBUG ("Enter Vendor Codec ID(in Hex): ");
            scanf ("%x", &val);

            vendor_codec_id = (UINT16)val;

            /* pack vendor codec id */
            BT_PACK_LE_2_BYTE_VAL (&codec_ie_cap[4U], vendor_codec_id);

            LOG_DEBUG ("Enter Vendor Specific Codec Value Length: ");
            scanf ("%d", &val);

            if ((6U + val) > AVDTP_MAX_CODEC_IE_LEN)
            {
                LOG_DEBUG ("\n== AVDTP_MAX_CODEC_IE_LEN is NOT sufficient for the Codec Info.== \n");
                LOG_DEBUG ("== Update AVDTP_MAX_CODEC_IE_LEN definition as required ==\n");

                retval = API_FAILURE; /* return; */
            }
            else
            {
                codec_ie_len = (UCHAR )(6U + val);

                LOG_DEBUG ("Enter Vendor Specific Codec Value(in Hex): ");
                for (i = 6U; i < codec_ie_len; i++)
                {
                    scanf ("%x", &val);
                    codec_ie_cap[i] = (UCHAR )val;
                }
                LOG_DEBUG ("\n");
            }
        }
#endif /* A2DP_VENDOR_CODEC */
        else
        {
            LOG_DEBUG ("Unsupported Codec Type 0x%02X. Registration Failed\n",
            codec_type);

            appl_a2dp_free_instance(indx);

            retval = API_FAILURE; /* return; */
        }

        if (API_SUCCESS == retval)
        {
            /* Update the registered Codec Type */
            appl_a2dp_instance[indx].codec_type = codec_type;

            /* Update A2DP role */
            appl_a2dp_instance[indx].role = sep_type;

            /* Initialize Stream End Point Capability */
            AVDTP_SET_SEP_CAPABILITY
            (
                appl_a2dp_instance[indx].sep_cap,
                sep_type,
                AVDTP_MEDIA_AUDIO,
                codec_type,
                codec_ie_len,
                codec_ie_cap
            );

#ifdef AVDTP_HAVE_CONTENT_PROTECTION
            /* Initialize the Streaming End Point CP Capability */
            AVDTP_SET_SEP_CP_CAPABILITY
            (
                (appl_a2dp_instance[indx].sep_cap.cp_cap[0U]),
                AVDTP_CONTENT_PROTECTION_TYPE_SCMS_T,
                cp_ie_buf,
                cp_ie_buf_len
            );
#endif /* AVDTP_HAVE_CONTENT_PROTECTION */

#ifdef AVDTP_HAVE_RECOVERY_SERVICE
            /* Initialize the Streaming End Point Recovery Capability */
            AVDTP_SET_SEP_RECOVERY_CAPABILITY
            (
                appl_a2dp_instance[indx].sep_cap,
                AVDTP_RECOVERY_TYPE_RFC2733,
                AVDTP_MAX_MRWS,
                AVDTP_MAX_MNMP
            );
#endif /* AVDTP_HAVE_RECOVERY_SERVICE */

#ifdef AVDTP_HAVE_DELAY_REPORTING
            /* Enable delay reporting service */
            appl_a2dp_instance[indx].sep_cap.dr_cap_flag = BT_TRUE;

            /* Init peer delay reporting support to false */
            appl_a2dp_peer_dr_cap_flag = BT_FALSE;
#endif /* AVDTP_HAVE_DELAY_REPORTING */

            /* Register to A2DP */
            LOG_DEBUG("A2DP Register Codec ... "); fflush (stdout);
            retval = BT_a2dp_register_codec
                     (
                         &a2dp_instance,
                         &appl_a2dp_instance[indx].sep_cap,
                         appl_a2dp_notify_cb
                     );

            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("*** FAILED. Reason = 0x%04X\n", retval);

                /* Free Instance */
                appl_a2dp_free_instance (indx);
            }
            else
            {
                LOG_DEBUG("OK\n");

                /* Save A2DP Codec Instance */
                appl_a2dp_instance[indx].a2dp_instance = a2dp_instance;

                /*
                 * Update the Media MTU for A2DP sink.
                 *
                 * NOTE:
                 * For A2DP Sources that support AAC codec,
                 * the MTU requirement may be large. For example, in
                 * case of iDevices, the developer guide states the
                 * MTU requirement of Sink devices to be minimum 885 bytes.
                 */
                if (AVDTP_SEP_SINK == sep_type)
                {
                    LOG_DEBUG ("Set A2DP Sink Media MTU ...\n");
                    retval = BT_a2dp_set_media_mtu (a2dp_instance, APPL_A2DP_SNK_MTU);
                    LOG_DEBUG ("Retval - 0x%04X\n", retval);
                }

                /* Show A2DP Instances */
                LOG_DEBUG("\n");
                appl_a2dp_show_instance();
            }
        }
    }

    return;
}


void appl_a2dp_connect (void)
{
    int choice, ret;
    UCHAR * codec_ie_conf;
    API_RESULT retval;
    UCHAR indx, acl_indx, remote_seid/*, i */;
    AVDTP_SEP_CONF sep_conf;

    /* Init */
    retval = API_SUCCESS;
    acl_indx = 0;

    /* Choose an Instance */
    appl_a2dp_get_instance (&indx);
    if (indx >= A2DP_MAX_CODECS)
    {
        LOG_DEBUG("Instance is out of Range\n");
        retval = API_FAILURE; /* return; */
    }
    else
    {
        if (0x00U == appl_a2dp_instance[indx].allocated)
        {
            LOG_DEBUG("Instance is not registered\n");
            retval = API_FAILURE; /* return; */
        }

        if (API_SUCCESS == retval)
        {
            /* Get Remote BD_ADDR */
            retval = appl_a2dp_get_acl_connection (&acl_indx);
            if (API_SUCCESS != retval)
            {
                retval = API_FAILURE; /* return; */
            }
        }
    }

    if (API_SUCCESS == retval)
    {
        if (BT_FALSE == BT_BD_ADDR_IS_NON_ZERO(appl_a2dp_instance[indx].bd_addr))
        {
            /* Save BD_ADDR */
            BT_COPY_BD_ADDR
            (
                appl_a2dp_instance[indx].bd_addr,
                appl_a2dp_acl_list[acl_indx].bd_addr
            );
        }

        /* Initialize SEP Configuration Parameters */
        AVDTP_INIT_SEP_CONFIGURATION (sep_conf);

        /* Enable Media Transport Service in SEP Configuration */
        AVDTP_SET_SEP_CONFIGURATION_MEDIA (sep_conf);

        /* Get SEP Configuration Codec IE */
        codec_ie_conf = appl_a2dp_instance[indx].codec_ie_conf;

        /* Set SEP Configuration Codec IE */
        ret = appl_a2dp_read_codec_ie_configuration
              (
                  APPL_A2DP_CONFIG_FILE,
                  appl_a2dp_instance[indx].sep_cap.codec_cap.codec_type,
                  codec_ie_conf
              );

        if (ret < 0)
        {
            LOG_DEBUG ("Failed to read Codec IE Configuration\n");
        }
        else
        {
#ifdef A2DP_VENDOR_CODEC
            if (AVDTP_CODEC_AUDIO_VENDOR_SPECIFIC == appl_a2dp_instance[indx].sep_cap.codec_cap.codec_type)
            {
                int       val;
                UINT32    vendor_id;
                UINT16    vendor_codec_id, i;

                LOG_DEBUG ("\n=== Enter Vendor Specific Codec Info. ===\n\n");

                LOG_DEBUG ("Enter Vendor ID(in Hex): ");
                scanf ("%x", &val);

                vendor_id = (UINT32)val;

                /* pack vendor id */
                BT_PACK_LE_4_BYTE_VAL (&codec_ie_conf[0], vendor_id);

                LOG_DEBUG ("Enter Vendor Codec ID(in Hex): ");
                scanf ("%x", &val);

                vendor_codec_id = (UINT16)val;

                /* pack vendor codec id */
                BT_PACK_LE_2_BYTE_VAL (&codec_ie_conf[4], vendor_codec_id);

                LOG_DEBUG ("Enter Vendor Specific Codec Value Length: ");
                scanf ("%d", &val);

                if ((6 + val) > AVDTP_MAX_CODEC_IE_LEN)
                {
                    LOG_DEBUG ("\n== AVDTP_MAX_CODEC_IE_LEN is NOT sufficient for the Codec Info.== \n");
                    LOG_DEBUG ("== Update AVDTP_MAX_CODEC_IE_LEN definition as required ==\n");

                    retval = API_FAILURE; /* return; */
                }

                if (API_SUCCESS == retval)
                {
                    if ((6 + val) != appl_a2dp_instance[indx].sep_cap.codec_cap.codec_ie_len)
                    {
                        LOG_DEBUG ("Codec IE Config. length and Codec IE Cap. length does NOT match\n");
                        LOG_DEBUG ("Returing....\n");
                        retval = API_FAILURE; /* return; */
                    }
                }

                if (API_SUCCESS == retval)
                {
                    LOG_DEBUG ("Enter Vendor Specific Codec Value(in Hex): ");
                    for
                    (
                        i = 6;
                        i < appl_a2dp_instance[indx].sep_cap.codec_cap.codec_ie_len;
                        i++
                    )
                    {
                        scanf ("%x", &val);
                        codec_ie_conf[i] = (UCHAR )val;
                    }
                    LOG_DEBUG ("\n");
                }
            }
        #endif /* A2DP_VENDOR_CODEC */

            if (API_SUCCESS == retval)
            {
                /* Enable Codec Capabilities in SEP Configuration */
                AVDTP_SET_SEP_CONFIGURATION_CODEC_CAP
                (
                    sep_conf,
                    appl_a2dp_instance[indx].sep_cap.codec_cap.media_type,
                    appl_a2dp_instance[indx].sep_cap.codec_cap.codec_type,
                    appl_a2dp_instance[indx].sep_cap.codec_cap.codec_ie_len,
                    codec_ie_conf
                )

#ifdef AVDTP_HAVE_CONTENT_PROTECTION
                
                /* Set Content Protection in the Configuration */
                printf("Want to Enable CONTENT_PROTECTION: SCMS_T(0/1)\n"); fflush(stdout);
                printf("\n>-*** NOTE: Check if the peer support for Content Protection in its Capability Response ***-<\n");
                scanf("%x", &choice);
                if (0 != choice)
                {
                    /* Enable CP capabilities in SEP Configuration */
                    AVDTP_SET_SEP_CP_CAPABILITY
                    (
                        (sep_conf.cp_cap[0U]),
                        AVDTP_CONTENT_PROTECTION_TYPE_SCMS_T,
                        cp_ie_buf,
                        cp_ie_buf_len
                    );

                    appl_a2dp_instance[indx].cp_type = AVDTP_CONTENT_PROTECTION_TYPE_SCMS_T;
                    appl_a2dp_instance[indx].cp_hdr_len = 0x01U;
                }
                else
                {
                    appl_a2dp_instance[indx].cp_type = AVDTP_INVALID_CP_TYPE;
                    appl_a2dp_instance[indx].cp_hdr_len = 0x00U;

                    /* Enable CP capabilities in SEP Configuration */
                    AVDTP_SET_SEP_CP_CAPABILITY
                    (
                        (sep_conf.cp_cap[0U]),
                        AVDTP_INVALID_CP_TYPE,
                        cp_ie_buf,
                        cp_ie_buf_len
                    );
                }
#endif /* AVDTP_HAVE_CONTENT_PROTECTION */

                /**
                 * Enable Delay Reporting Service in SEP Configuration,
                 * if the peer notifies through the GET_ALL_CAPABILITIES command rsp and locally
                 * delay report support enabled.
                 */
#ifdef A2DP_1_3
                if ((BT_TRUE == appl_a2dp_peer_dr_cap_flag) &&
                    (BT_TRUE == appl_a2dp_instance[indx].sep_cap.dr_cap_flag))
                {
                    AVDTP_SET_SEP_CONFIGURATION_DELAY_REPORTING (sep_conf);
                }
#endif /* A2DP_1_3 */

                /* Get Remote SEID */
                LOG_DEBUG("Enter Remote SEID (0x) = "); fflush (stdout);
                scanf("%x", &choice);
                remote_seid = (UCHAR) choice;
                if (remote_seid == 0)
                {
                    LOG_DEBUG("Remote SEID is 0x00\n");
                    LOG_DEBUG("=> A2DP will perform AVDTP Discover & Get Capabilities\n");
                }

                /* A2DP Connect */
                LOG_DEBUG("A2DP Connect ... "); fflush (stdout);
                retval = BT_a2dp_connect
                         (
                             appl_a2dp_instance[indx].a2dp_instance,
                             appl_a2dp_instance[indx].bd_addr,
                             remote_seid,
                             &sep_conf
                         );

                if (API_SUCCESS != retval)
                {
                    LOG_DEBUG("FAILED. Reason = 0x%04X\n", retval);
                }
                else
                {
                    LOG_DEBUG("OK. Wait for A2DP_CONNECT_CNF\n");

                    appl_a2dp_save_codec_ie_configuration
                    (
                        appl_a2dp_instance[indx].role,
                        appl_a2dp_instance[indx].sep_cap.codec_cap.codec_type,
                        codec_ie_conf,
                        appl_a2dp_instance[indx].sep_cap.codec_cap.codec_ie_len
                    );
                }
            } /* if (API_SUCCESS == retval) */
        }
    }

    return;
}


void appl_a2dp_start (void)
{
    API_RESULT retval;
    UINT16 acl_handle;
    UCHAR indx;

    /* Choose an Instance */
    appl_a2dp_get_instance (&indx);
    if (indx >= A2DP_MAX_CODECS)
    {
        LOG_DEBUG("Instance is out of Range\n");
        /* return; */
    }
    else
    {
        if (0x00U == appl_a2dp_instance[indx].allocated)
        {
            LOG_DEBUG("Instance is not registered\n");
            /* return; */
        }
        else
        {
            /* A2DP Start */
            LOG_DEBUG("A2DP Start ... "); fflush (stdout);
            retval = BT_a2dp_start
                     (
                         appl_a2dp_instance[indx].a2dp_instance
                     );

            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("FAILED. Reason = 0x%04X\n", retval);
            }
            else
            {
                LOG_DEBUG("OK. Wait for A2DP_START_CNF\n");

                /* Unsniff the link if required */
                (BT_IGNORE_RETURN_VALUE) BT_hci_get_acl_connection_handle(appl_a2dp_instance[indx].bd_addr, &acl_handle);
                (BT_IGNORE_RETURN_VALUE) BT_hci_exit_sniff_mode(acl_handle);
            }
        }
    }

    return;
}


void appl_a2dp_suspend (void)
{
    API_RESULT retval;
    UCHAR indx;

    /* Choose an Instance */
    appl_a2dp_get_instance (&indx);
    if (indx >= A2DP_MAX_CODECS)
    {
        LOG_DEBUG("Instance is out of Range\n");
        /* return; */
    }
    else
    {
        if (0x00U == appl_a2dp_instance[indx].allocated)
        {
            LOG_DEBUG("Instance is not registered\n");
            /* return; */
        }
        else
        {
            /* A2DP Suspend */
            LOG_DEBUG("A2DP Suspend ... "); fflush (stdout);
            retval = BT_a2dp_suspend
                     (
                         appl_a2dp_instance[indx].a2dp_instance
                     );

            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("FAILED. Reason = 0x%04X\n", retval);
            }
            else
            {
                LOG_DEBUG("OK. Wait for A2DP_SUSPEND_CNF\n");
            }
        }
    }

    return;
}


void appl_a2dp_reconfigure (UCHAR indx)
{
    int ret;
    UCHAR * codec_ie_conf;
    API_RESULT retval;
    AVDTP_SEP_CONF sep_conf;

    /* Check Instance */
    if (indx >= A2DP_MAX_CODECS)
    {
        LOG_DEBUG("Instance is outof Range\n");
        /* return; */
    }
    else
    {
        if (0x00U == appl_a2dp_instance[indx].allocated)
        {
            LOG_DEBUG("Instance is not registered\n");
            /* return; */
        }
        else
        {
            /* Initialize SEP Configuration Parameters */
            AVDTP_INIT_SEP_CONFIGURATION (sep_conf);

            /* Get SEP Configuration Codec IE */
            codec_ie_conf = appl_a2dp_instance[indx].codec_ie_conf;

            ret = appl_a2dp_read_codec_ie_configuration
                  (
                      APPL_A2DP_CONFIG_FILE,
                      appl_a2dp_instance[indx].sep_cap.codec_cap.codec_type,
                      codec_ie_conf
                  );

            if (ret >= 0)
            {
                /* return; */

                /* Enable Codec Capabilities in SEP Configuration */
                AVDTP_SET_SEP_CONFIGURATION_CODEC_CAP
                (
                    sep_conf,
                    appl_a2dp_instance[indx].sep_cap.codec_cap.media_type,
                    appl_a2dp_instance[indx].sep_cap.codec_cap.codec_type,
                    appl_a2dp_instance[indx].sep_cap.codec_cap.codec_ie_len,
                    codec_ie_conf
                )
            #ifdef AVDTP_HAVE_CONTENT_PROTECTION
                /* Enable CP capabilities in SEP Configuration */
                AVDTP_SET_SEP_CP_CAPABILITY
                (
                    (sep_conf.cp_cap[0]),
                    AVDTP_CONTENT_PROTECTION_TYPE_SCMS_T,
                    cp_ie_buf,
                    cp_ie_buf_len
                );
            #endif /* AVDTP_HAVE_CONTENT_PROTECTION */

                /**
                 * Enable Delay Reporting Service in SEP Configuration,
                 * if the peer notifies through the GET_ALL_CAPABILITIES command rsp and locally
                 * delay report support enabled.
                 */
            #ifdef A2DP_1_3
            #if 0
                if ((BT_TRUE == appl_a2dp_peer_dr_cap_flag) &&
                    (BT_TRUE == appl_a2dp_instance[indx].sep_cap.dr_cap_flag))
                {
                    AVDTP_SET_SEP_CONFIGURATION_DELAY_REPORTING (sep_conf);
                }
            #endif /* 0 */
            #endif /* A2DP_1_3 */

                /* A2DP Reconfigure */
                LOG_DEBUG("A2DP Reconfigure ... "); fflush (stdout);
                retval = BT_a2dp_reconfigure
                         (
                             appl_a2dp_instance[indx].a2dp_instance,
                             &sep_conf
                         );

                if (API_SUCCESS != retval)
                {
                    LOG_DEBUG("FAILED. Reason = 0x%04X\n", retval);
                }
                else
                {
                    LOG_DEBUG("OK. Wait for A2DP_RECONFIGURE_CNF\n");

                    appl_a2dp_save_codec_ie_configuration
                    (
                        appl_a2dp_instance[indx].role,
                        appl_a2dp_instance[indx].sep_cap.codec_cap.codec_type,
                        codec_ie_conf,
                        appl_a2dp_instance[indx].sep_cap.codec_cap.codec_ie_len
                    );
                }
            }/* if (ret >= 0) */
        } /* else of (0x00U == appl_a2dp_instance[indx].allocated) */
    } /* else of (indx >= A2DP_MAX_CODECS) */

    return;
}


void appl_a2dp_disconnect (void)
{
    API_RESULT retval;
    UCHAR indx;

    /* Choose an Instance */
    appl_a2dp_get_instance (&indx);
    if (indx >= A2DP_MAX_CODECS)
    {
        LOG_DEBUG("Instance is out of Range\n");
        /* return; */
    }
    else
    {
        if (0x00U == appl_a2dp_instance[indx].allocated)
        {
            LOG_DEBUG("Instance is not registered\n");
            /* return; */
        }
        else
        {
            /* A2DP Disconnect */
            LOG_DEBUG("A2DP Disconnect ... "); fflush (stdout);
            retval = BT_a2dp_disconnect
                     (
                         appl_a2dp_instance[indx].a2dp_instance
                     );

            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("FAILED. Reason = 0x%04X\n", retval);
            }
            else
            {
                LOG_DEBUG("OK. Wait for A2DP_DISCONNECT_CNF\n");
            }
        }
    }

    return;
}


void appl_a2dp_set_media_mtu (void)
{
    int choice;
    API_RESULT retval;
    UINT16 mtu;
    UCHAR indx;

    /* Choose an Instance */
    appl_a2dp_get_instance (&indx);
    if (indx >= A2DP_MAX_CODECS)
    {
        LOG_DEBUG("Instance is out of Range\n");
        /* return;  */
    }
    else
    {
        if (0x00U == appl_a2dp_instance[indx].allocated)
        {
            LOG_DEBUG("Instance is not registered\n");
            /* return; */
        }
        else
        {
            LOG_DEBUG("Enter Media MTU = "); fflush (stdout);
            scanf("%d", &choice);
            if ((choice < 48U) || (choice > L2CAP_MAX_MTU))
            {
                LOG_DEBUG("*** Invalid Media MTU - %d\n", choice);
                /* return; */
            }
            else
            {
                mtu = (UINT16) choice;

                /* A2DP Set Media MTU */
                LOG_DEBUG("A2DP Set Media MTU ... "); fflush (stdout);
                retval = BT_a2dp_set_media_mtu
                         (
                             appl_a2dp_instance[indx].a2dp_instance,
                             mtu
                         );

                if (API_SUCCESS != retval)
                {
                    LOG_DEBUG("FAILED. Reason = 0x%04X\n", retval);
                }
                else
                {
                    LOG_DEBUG("OK\n");
                }
            }
        }
    }

    return;
}

#ifdef AVDTP_HAVE_CONTENT_PROTECTION
void appl_a2dp_set_cp_header (void)
{
    int choice;
    API_RESULT retval;
    UCHAR indx;

    /* Choose an Instance */
    appl_a2dp_get_instance (&indx);
    if (indx >= A2DP_MAX_CODECS)
    {
        LOG_DEBUG("Instance is out of Range\n");
        /* return; */
    }
    else
    {
        if (0x00U == appl_a2dp_instance[indx].allocated)
        {
            LOG_DEBUG("Instance is not registered\n");
            /* return; */
        }
        else
        {
            LOG_DEBUG ("1. COPY RESTRICTED\n2. COPY ONCE ONLY\n3. COPY UNLIMITED\n");
            LOG_DEBUG("Enter SCMS-T CP Type: "); fflush (stdout);
            scanf("%d", &choice);
            if ((choice < 1U) || (choice > 3U))
            {
                LOG_DEBUG("*** Invalid Choice - %d\n", choice);
                /* return; */
            }
            else
            {
                switch (choice)
                {
                    case 2U:
                        a2dp_cp_header [0U] = AVDTP_SCMS_T_COPY_ONCE_ONLY;
                    break;
                    case 3U:
                        a2dp_cp_header [0U] = AVDTP_SCMS_T_COPY_UNLIMITED;
                    break;
                    default: /* 1U */
                        a2dp_cp_header[0U] = AVDTP_SCMS_T_COPY_NOT_ALLOWED;
                    break;
                }

                a2dp_cp_header_len = 0x01U;

                /* A2DP Set the Content Protection Header*/
                LOG_DEBUG("A2DP Set CP Header ... "); fflush (stdout);
                retval = BT_a2dp_set_cp_header
                         (
                             appl_a2dp_instance[indx].a2dp_instance,
                             a2dp_cp_header,
                             a2dp_cp_header_len
                         );

                if (API_SUCCESS != retval)
                {
                    LOG_DEBUG("FAILED. Reason = 0x%04X\n", retval);
                }
                else
                {
                    LOG_DEBUG("OK\n");
                }
            }
        }
    }

    return;
}
#endif /* AVDTP_HAVE_CONTENT_PROTECTION */

API_RESULT appl_a2dp_notify_cb
           (
               UCHAR    codec_instance,
               UCHAR    event_type,
               UINT16   event_result,
               void  *  event_data,
               UINT16   event_datalen
           )
{
    A2DP_DEVICE_INFO * a2dp_dev_info;
    UCHAR * codec_ie;
    UCHAR codec_type;
    UCHAR role;
    UCHAR i;
    API_RESULT retval;

#ifdef A2DP_1_3
    /**
     * Delay Report is a Singed Value.
     * Using a Unsigned storage type, for the bit wise shift operation.
     * While using the reported delay, it will be treated as a singed value.
     */
    UINT16 delay_report_data;
#endif /* A2DP_1_3 */

    /* Get the role */
    role          = appl_a2dp_instance[codec_instance].role;
    codec_ie      = NULL;
    retval = API_SUCCESS;

#ifdef A2DP_1_3
    delay_report_data = 0x00U;
#endif /* A2DP_1_3 */

    switch (event_type)
    {
    case A2DP_CONNECT_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received A2DP_CONNECT_CNF\n");
        LOG_DEBUG("\tCodec Instance = %d\n", codec_instance);
        LOG_DEBUG("\tEvent Result   = 0x%04X\n", event_result);

        if (API_SUCCESS == event_result)
        {
            /* Set Connection Info */
            a2dp_dev_info = (A2DP_DEVICE_INFO *)event_data;

            /* BD Address */
            BT_mem_copy
            (
                appl_a2dp_instance[codec_instance].bd_addr,
                a2dp_dev_info->bd_addr,
                BT_BD_ADDR_SIZE
            );

            /* Codec Configuration */
            BT_mem_copy
            (
                appl_a2dp_instance[codec_instance].codec_ie_conf,
                a2dp_dev_info->codec_ie_conf,
                a2dp_dev_info->codec_ie_len
            );

            /* Set Application Stream State to FALSE */
            appl_a2dp_instance[codec_instance].stream_state = APPL_A2DP_STREAM_FALSE;
#ifdef APPL_A2DP_SET_AUTO_FLUSH_TIMEOUT
            if (AVDTP_CODEC_AUDIO_SBC == appl_a2dp_instance[codec_instance].codec_type)
            {
                /*
                 * Set flush timeout for the source role - The timeout is for 48KHz sampling rate audio.
                 * The application can make this setting dynamic if the source has a dynamic
                 * sampling rate configuration for audio.
                 */
                if (AVDTP_SEP_SOURCE == appl_a2dp_instance[codec_instance].role)
                {
                    UINT16 acl_handle = 0;
                    LOG_DEBUG("Updating flush timeout...\n");
                    (BT_IGNORE_RETURN_VALUE) BT_hci_get_acl_connection_handle(appl_a2dp_instance[codec_instance].bd_addr, &acl_handle);
                    (BT_IGNORE_RETURN_VALUE) BT_hci_write_automatic_flush_timeout(acl_handle, 0x40U);
                }
            }
#endif /* APPL_A2DP_SET_AUTO_FLUSH_TIMEOUT */
        }
#ifdef AVDTP_HAVE_CONTENT_PROTECTION
        else
        {
            appl_a2dp_instance[codec_instance].cp_type = AVDTP_INVALID_CP_TYPE;
        }
#endif /* AVDTP_HAVE_CONTENT_PROTECTION */
        /* show instances */
        appl_a2dp_show_instance();
        break;

    case A2DP_CONNECT_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received A2DP_CONNECT_IND\n");
        LOG_DEBUG("\tCodec Instance = %d\n", codec_instance);
        LOG_DEBUG("\tEvent Result   = 0x%04X\n", event_result);

        /* Set Connection Info */
        if (API_SUCCESS == event_result)
        {
            a2dp_dev_info = (A2DP_DEVICE_INFO *)event_data;

            /* BD Address */
            BT_mem_copy
            (
                appl_a2dp_instance[codec_instance].bd_addr,
                a2dp_dev_info->bd_addr,
                BT_BD_ADDR_SIZE
            );

            /* Codec Configuration */
            BT_mem_copy
            (
                appl_a2dp_instance[codec_instance].codec_ie_conf,
                a2dp_dev_info->codec_ie_conf,
                a2dp_dev_info->codec_ie_len
            );

            /* Set Application Stream State to FALSE */
            appl_a2dp_instance[codec_instance].stream_state = APPL_A2DP_STREAM_FALSE;
        }
#ifdef AVDTP_HAVE_CONTENT_PROTECTION
        else
        {
            appl_a2dp_instance[codec_instance].cp_type = AVDTP_INVALID_CP_TYPE;
        }
#endif /* AVDTP_HAVE_CONTENT_PROTECTION */

        if (AVDTP_CODEC_AUDIO_SBC == appl_a2dp_instance[codec_instance].codec_type)
        {
#if 0
            /* Enable below code segment, if want to reconfigure for 44100 sampling rate - as default */
            if ((A2DP_SBC_SAMPLING_FREQUENCY_44100 != (a2dp_dev_info->codec_ie_conf[0] & 0xF0)) &&
                (AVDTP_SEP_SOURCE == role))
            {
                appl_a2dp_reconfigure (codec_instance);
            }
#endif /* 0 */
#ifdef APPL_A2DP_SET_AUTO_FLUSH_TIMEOUT
            /*
             * Set flush timeout for the source role - The timeout is for 48KHz sampling rate audio.
             * The application can make this setting dynamic if the source has a dynamic
             * sampling rate configuration for audio.
             */
            if (AVDTP_SEP_SOURCE == appl_a2dp_instance[codec_instance].role)
            {
                UINT16 acl_handle = 0;
                LOG_DEBUG("Updating flush timeout...\n");
                (BT_IGNORE_RETURN_VALUE) BT_hci_get_acl_connection_handle(appl_a2dp_instance[codec_instance].bd_addr, &acl_handle);
                (BT_IGNORE_RETURN_VALUE) BT_hci_write_automatic_flush_timeout(acl_handle, 0x40U);
            }
#endif //APPL_A2DP_SET_AUTO_FLUSH_TIMEOUT
        }
        /* show instances */
        appl_a2dp_show_instance();
        break;

    case A2DP_START_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received A2DP_START_CNF\n");
        LOG_DEBUG("\tCodec Instance = %d\n", codec_instance);
        LOG_DEBUG("\tEvent Result   = 0x%04X\n", event_result);

        /* Start Audio Device for Recording */
        if (API_SUCCESS == event_result)
        {
            for (i = 0U; i < A2DP_MAX_CODECS; i++)
            {
                if (APPL_A2DP_STREAM_TRUE == appl_a2dp_instance[i].stream_state)
                {
                    /**
                     * Audio play or record might already started.
                     * Skip A2DP pl start.
                     */
                    break;
                }
            }

            if (A2DP_MAX_CODECS == i)
            {
                /**
                 * All A2DP instances are NOT in stream state.
                 * Now start A2DP pl.
                 */

                if (AVDTP_CODEC_AUDIO_SBC == appl_a2dp_instance[codec_instance].codec_type)
                {
                    if ((BT_FALSE == appl_a2dp_sbc_encoder_state) &&
                        (AVDTP_SEP_SOURCE == role))
                    {
                        LOG_DEBUG("SBC Encoder is not Initialized Succesfully\n");
                        break;
                    }

#ifdef AVDTP_HAVE_CONTENT_PROTECTION
                    if ((AVDTP_SEP_SOURCE == role) &&
                        (AVDTP_CONTENT_PROTECTION_TYPE_SCMS_T == appl_a2dp_instance[codec_instance].cp_type))
                    {
                        printf("CP Header Value: 0x%02x\n", a2dp_cp_header[0U]);
                        a2dp_cp_header_len = 0x01U;

                        /* A2DP Set the Content Protection Header*/
                        printf("A2DP Set CP Header ... ");
                        retval = BT_a2dp_set_cp_header
                                 (
                                     appl_a2dp_instance[codec_instance].a2dp_instance,
                                     a2dp_cp_header,
                                     a2dp_cp_header_len
                                 );

                        if (API_SUCCESS != retval)
                        {
                            printf("FAILED. Reason = 0x%04X\n", retval);
                        }
                        else
                        {
                            printf("OK\n");
                        }
                    }
#endif /* AVDTP_HAVE_CONTENT_PROTECTION */

                    /* Start the Pl */
                    a2dp_start_pl(role);
                }

#ifdef A2DP_SINK
                if ((AVDTP_SEP_SINK == role) &&
                    (AVDTP_CODEC_AUDIO_SBC == appl_a2dp_instance[codec_instance].codec_type))
                {
#ifdef INCLUDE_JPL
                    /* Start JPL */
                    (BT_IGNORE_RETURN_VALUE) BT_jpl_start(&jpl_param);
#endif /* INCLUDE_JPL */
                }
#endif /* A2DP_SINK */
            }

#if (defined A2DP_USE_SBC && defined A2DP_SOURCE)
            a2dp_max_allowed_payload_len = *(UINT16 *)event_data;

            /*
             * If a content protection scheme (eg. SCMT-T or DTCP etc.) is
             * configured for the media transport, subtract the correspoinding
             * 'Content protection header length' from
             * the 'Maximum Allowed Payload Length' to arrive at the exact
             * 'Maximum Payload Length' application can send in a single
             * media packet.
             */
        #ifdef AVDTP_HAVE_CONTENT_PROTECTION
            a2dp_max_allowed_payload_len -= a2dp_cp_header_len;
        #endif /* AVDTP_HAVE_CONTENT_PROTECTION */

            LOG_DEBUG("\tMaximum Media Payload Allowed = %d\n",
                                        a2dp_max_allowed_payload_len);

            /*
             * Assuming 'A2DP_SUPPORT_MULTIPLE_MEDIA_FRAME_WRITE' is defined,
             * if A2DP source application streams encoded SBC frames with
             * each media frame of same size (say 'N') and
             * the calculated 'Maximum Payload Length' (say 'M'), then
             * the application can send 'M'/'N' media frames at maximum using
             * BT_a2dp_media_write() API.
             */
#endif /* (defined A2DP_USE_SBC && defined A2DP_SOURCE) */

            /* Maximum Media payload length */
            appl_a2dp_instance[codec_instance].max_mpl =  *(UINT16 *)event_data;

            /* print */
            LOG_DEBUG ("Max. Media Payload Length Allowed: %d\n", appl_a2dp_instance[codec_instance].max_mpl);

#ifdef A2DP_SUPPORT_MULTIPLE_MEDIA_FRAME_WRITE
            /**
             * Setting the Max. media payload length as minimum of all instances
             */
            if (appl_a2dp_instance[codec_instance].max_mpl < appl_a2dp_src_max_mpl)
            {
                appl_a2dp_src_max_mpl = appl_a2dp_instance[codec_instance].max_mpl;
            }
#endif /* A2DP_SUPPORT_MULTIPLE_MEDIA_FRAME_WRITE */

            if ((AVDTP_CODEC_AUDIO_SBC == appl_a2dp_instance[codec_instance].codec_type) &&
                (APPL_A2DP_STREAM_FALSE == appl_a2dp_instance[codec_instance].stream_state))
            {
                /* Set Application Stream State to TRUE */
                appl_a2dp_instance[codec_instance].stream_state = APPL_A2DP_STREAM_TRUE;
            }

            /* Update the playback instance if not already used up */
            if (A2DP_MAX_CODECS == appl_a2dp_playback_instance)
            {
                LOG_DEBUG ("Update playback instance - %d\n", codec_instance);
                appl_a2dp_playback_instance = codec_instance;
            }
        }

        break;

    case A2DP_START_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received A2DP_START_IND\n");
        LOG_DEBUG("\tCodec Instance = %d\n", codec_instance);
        LOG_DEBUG("\tEvent Result   = 0x%04X\n", event_result);

        if (API_SUCCESS == event_result)
        {
            /* Maximum Media payload length */
            appl_a2dp_instance[codec_instance].max_mpl =  *(UINT16 *)event_data;

            /* print */
            LOG_DEBUG ("Max. Media Payload Length Allowed: %d\n", appl_a2dp_instance[codec_instance].max_mpl);

#ifdef A2DP_SUPPORT_MULTIPLE_MEDIA_FRAME_WRITE
            /**
             * Setting the Max. media payload length as minimum of all instances
             */
            if (appl_a2dp_instance[codec_instance].max_mpl < appl_a2dp_src_max_mpl)
            {
                appl_a2dp_src_max_mpl = appl_a2dp_instance[codec_instance].max_mpl;
            }
#endif /* A2DP_SUPPORT_MULTIPLE_MEDIA_FRAME_WRITE */

            for (i = 0U; i < A2DP_MAX_CODECS; i++)
            {
                if (APPL_A2DP_STREAM_TRUE == appl_a2dp_instance[i].stream_state)
                {
                    /**
                     * Audio play or record might already started.
                     * Skip A2DP pl start.
                     */
                    break;
                }
            }

            if (A2DP_MAX_CODECS == i)
            {
                /**
                 * All A2DP instances are NOT in stream state.
                 * Now start A2DP pl.
                 */

                if (AVDTP_CODEC_AUDIO_SBC == appl_a2dp_instance[codec_instance].codec_type)
                {
                    if ((BT_FALSE == appl_a2dp_sbc_encoder_state) &&
                        (AVDTP_SEP_SOURCE == role))
                    {
                        LOG_DEBUG("SBC Encoder is not Initialized Succesfully\n");
                        break;
                    }

#ifdef AVDTP_HAVE_CONTENT_PROTECTION
                    if ((AVDTP_SEP_SOURCE == role) &&
                        (AVDTP_CONTENT_PROTECTION_TYPE_SCMS_T == appl_a2dp_instance[codec_instance].cp_type))
                    {
                        /* SCMS_T CP Type */
                        printf("CP Header Value: 0x%02x\n", a2dp_cp_header[0U]);
                        a2dp_cp_header_len = 0x01U;

                        /* A2DP Set the Content Protection Header*/
                        printf("A2DP Set CP Header ... ");
                        retval = BT_a2dp_set_cp_header
                                 (
                                     appl_a2dp_instance[codec_instance].a2dp_instance,
                                     a2dp_cp_header,
                                     a2dp_cp_header_len
                                 );

                        if (API_SUCCESS != retval)
                        {
                            printf("FAILED. Reason = 0x%04X\n", retval);
                        }
                        else
                        {
                            printf("OK\n");
                        }
                    }
#endif /* AVDTP_HAVE_CONTENT_PROTECTION */

                    /* Start the Pl */
                    a2dp_start_pl(role);
                }

#ifdef A2DP_SINK
                if ((AVDTP_SEP_SINK == role) &&
                    (AVDTP_CODEC_AUDIO_SBC == appl_a2dp_instance[codec_instance].codec_type))
                {
#ifdef INCLUDE_JPL

                    /* Start JPL */
                    (BT_IGNORE_RETURN_VALUE) BT_jpl_start(&jpl_param);
#endif /* INCLUDE_JPL */
                }
#endif /* A2DP_SINK */
            }

            if ((AVDTP_CODEC_AUDIO_SBC == appl_a2dp_instance[codec_instance].codec_type) &&
                (APPL_A2DP_STREAM_FALSE == appl_a2dp_instance[codec_instance].stream_state))
            {
                /* Set Application Stream State to TRUE */
                appl_a2dp_instance[codec_instance].stream_state = APPL_A2DP_STREAM_TRUE;
            }

            /* Update the playback instance if not already used up */
            if (A2DP_MAX_CODECS == appl_a2dp_playback_instance)
            {
                LOG_DEBUG ("Update playback instance - %d\n", codec_instance);
                appl_a2dp_playback_instance = codec_instance;
            }
        }

#if (defined A2DP_USE_SBC && defined A2DP_SOURCE)
        a2dp_max_allowed_payload_len = *(UINT16 *)event_data;

        /*
         * If a content protection scheme (eg. SCMT-T or DTCP etc.) is
         * configured for the media transport, subtract the correspoinding
         * 'Content protection header length' from
         * the 'Maximum Allowed Payload Length' to arrive at the exact
         * 'Maximum Payload Length' application can send in a single
         * media packet.
         */
    #ifdef AVDTP_HAVE_CONTENT_PROTECTION
        a2dp_max_allowed_payload_len -= a2dp_cp_header_len;
    #endif /* AVDTP_HAVE_CONTENT_PROTECTION */

        LOG_DEBUG("\tMaximum Media Payload Allowed = %d\n",
                                        a2dp_max_allowed_payload_len);

        /*
         * Assuming 'A2DP_SUPPORT_MULTIPLE_MEDIA_FRAME_WRITE' is defined,
         * if A2DP source application streams encoded SBC frames with
         * each media frame of same size (say 'N') and
         * the calculated 'Maximum Payload Length' (say 'M'), then
         * the application can send 'M'/'N' media frames at maximum using
         * BT_a2dp_media_write() API.
         */
#endif /* (defined A2DP_USE_SBC && defined A2DP_SOURCE) */

        break;

    case A2DP_SUSPEND_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received A2DP_SUSPEND_CNF\n");
        LOG_DEBUG("\tCodec Instance = %d\n", codec_instance);
        LOG_DEBUG("\tEvent Result   = 0x%04X\n", event_result);

        /* Stop Audio Device for Recording */
        if (API_SUCCESS == event_result)
        {
            if ((AVDTP_CODEC_AUDIO_SBC == appl_a2dp_instance[codec_instance].codec_type) &&
                (APPL_A2DP_STREAM_TRUE == appl_a2dp_instance[codec_instance].stream_state))
            {
                /* Set Application Stream State to FALSE */
                appl_a2dp_instance[codec_instance].stream_state = APPL_A2DP_STREAM_FALSE;
            }

            for (i = 0U; i < A2DP_MAX_CODECS; i++)
            {
                if (APPL_A2DP_STREAM_TRUE == appl_a2dp_instance[i].stream_state)
                {
                    LOG_DEBUG ("Update playback instance - %d\n", i);

                    /* Update playback instance */
                    appl_a2dp_playback_instance = i;

                    /**
                     * Skip A2DP pl stop
                     */
                    break;
                }
            }

            if (A2DP_MAX_CODECS == i)
            {
                /**
                 * All the instances are NOT in stream state.
                 * Now Stop the pl.
                 */

                if (AVDTP_CODEC_AUDIO_SBC == appl_a2dp_instance[codec_instance].codec_type)
                {
                    /* Stop the Pl */
                    a2dp_stop_pl(role);
                }

#ifdef A2DP_SINK
                if ((AVDTP_SEP_SINK == role) &&
                    (AVDTP_CODEC_AUDIO_SBC == appl_a2dp_instance[codec_instance].codec_type))
                {
#ifdef INCLUDE_JPL
                    /* Stop JPL */
                    (BT_IGNORE_RETURN_VALUE) BT_jpl_stop();
#endif /* INCLUDE_JPL */
                }
#endif /* A2DP_SINK */
            }

            /* Free the playback instance */
            if (codec_instance == appl_a2dp_playback_instance)
            {
                LOG_DEBUG ("Reset playback instance\n");
                appl_a2dp_playback_instance = A2DP_MAX_CODECS;
            }
        }

        break;

    case A2DP_SUSPEND_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received A2DP_SUSPEND_IND\n");
        LOG_DEBUG("\tCodec Instance = %d\n", codec_instance);
        LOG_DEBUG("\tEvent Result   = 0x%04X\n", event_result);

        if (API_SUCCESS == event_result)
        {
            if ((AVDTP_CODEC_AUDIO_SBC == appl_a2dp_instance[codec_instance].codec_type) &&
                (APPL_A2DP_STREAM_TRUE == appl_a2dp_instance[codec_instance].stream_state))
            {
                /* Set Application Stream State to FALSE */
                appl_a2dp_instance[codec_instance].stream_state = APPL_A2DP_STREAM_FALSE;
            }

            for (i = 0U; i < A2DP_MAX_CODECS; i++)
            {
                if (APPL_A2DP_STREAM_TRUE == appl_a2dp_instance[i].stream_state)
                {
                    LOG_DEBUG ("Update playback instance - %d\n", i);

                    /* Update playback instance */
                    appl_a2dp_playback_instance = i;

                    /**
                     * Skip A2DP pl stop
                     */
                    break;
                }
            }

            if (A2DP_MAX_CODECS == i)
            {
                /**
                 * All the instances are NOT in stream state.
                 * Now Stop the pl.
                 */

                if (AVDTP_CODEC_AUDIO_SBC == appl_a2dp_instance[codec_instance].codec_type)
                {
                    /* Stop the Pl */
                    a2dp_stop_pl(role);
                }

#ifdef A2DP_SINK
                if ((AVDTP_SEP_SINK == role) &&
                    (AVDTP_CODEC_AUDIO_SBC == appl_a2dp_instance[codec_instance].codec_type))
                {
#ifdef INCLUDE_JPL
                    /* Stop JPL */
                    (BT_IGNORE_RETURN_VALUE) BT_jpl_stop();
#endif /* INCLUDE_JPL */
                }
#endif /* A2DP_SINK */
            }

            /* Free the playback instance */
            if (codec_instance == appl_a2dp_playback_instance)
            {
                LOG_DEBUG ("Reset playback instance\n");
                appl_a2dp_playback_instance = A2DP_MAX_CODECS;
            }
        }

        break;

    case A2DP_RECONFIGURE_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received A2DP_RECONFIGURE_CNF\n");
        LOG_DEBUG("\tCodec Instance = %d\n", codec_instance);
        LOG_DEBUG("\tEvent Result   = 0x%04X\n", event_result);
        break;

    case A2DP_CONFIGURE_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received A2DP_CONFIGURE_IND\n");
        LOG_DEBUG("\tCodec Instance = %d\n", codec_instance);
        LOG_DEBUG("\tEvent Result   = 0x%04X\n", event_result);
        LOG_DEBUG("\tEvent Data = %p, Data Length = %d\n",
        event_data, event_datalen);

        /* Get Codec Type */
        codec_type = appl_a2dp_instance[codec_instance].codec_type;

        /* Init */
        a2dp_dev_info = NULL;

#ifdef  A2DP_1_3
        if (NULL != event_data)
        {
            a2dp_dev_info = (A2DP_DEVICE_INFO *)event_data;
            codec_ie = a2dp_dev_info->codec_ie_conf;
            event_datalen = a2dp_dev_info->codec_ie_len;
        }
#else
        codec_ie = (UCHAR *) event_data;
#endif /* A2DP_1_3 */

        /* Save Codec Configuration */
        appl_a2dp_save_codec_ie_configuration
        (
            role,
            codec_type,
            codec_ie,
            event_datalen
        );

        /* Print Codec Configuration */
        appl_a2dp_print_codec_ie_configuration
        (
            codec_type,
            codec_ie,
            event_datalen
        );

#ifdef AVDTP_HAVE_CONTENT_PROTECTION
        if ((NULL != a2dp_dev_info) &&
            (AVDTP_CONTENT_PROTECTION_TYPE_SCMS_T == a2dp_dev_info->cp_type))
        {
            printf("Content Protection Type:SCMS_T\n");
            appl_a2dp_instance[codec_instance].cp_type = AVDTP_CONTENT_PROTECTION_TYPE_SCMS_T;
            appl_a2dp_instance[codec_instance].cp_hdr_len = 0x01U;
        }
        else
        {
            appl_a2dp_instance[codec_instance].cp_type = AVDTP_INVALID_CP_TYPE;
            appl_a2dp_instance[codec_instance].cp_hdr_len = 0x00U;
            printf("Content Protection is NOT configured\n");
        }
#endif /* AVDTP_HAVE_CONTENT_PROTECTION */

#ifdef A2DP_1_3
        /* Delay report suport */
        printf ("Delay Report Support: ");
        if ((NULL != a2dp_dev_info) &&
            (BT_TRUE == a2dp_dev_info->dr_config_flag))
        {
             LOG_DEBUG ("YES\n");
        }
        else
        {
            LOG_DEBUG ("NO\n");
        }
#endif /* A2DP_1_3 */

        break;

    case A2DP_DISCONNECT_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received A2DP_DISCONNECT_CNF\n");
        LOG_DEBUG("\tCodec Instance = %d\n", codec_instance);
        LOG_DEBUG("\tEvent Result   = 0x%04X\n", event_result);

        /* Reset Connection Info */
        if (API_SUCCESS == event_result)
        {
            if ((AVDTP_CODEC_AUDIO_SBC == appl_a2dp_instance[codec_instance].codec_type) &&
                (APPL_A2DP_STREAM_TRUE == appl_a2dp_instance[codec_instance].stream_state))
            {
                /* Stop the Pl */
                a2dp_stop_pl(role);

#ifdef A2DP_SINK
                if (AVDTP_SEP_SINK == role)
                {
#ifdef INCLUDE_JPL
                    /* Stop JPL */
                    (BT_IGNORE_RETURN_VALUE) BT_jpl_stop();
#endif /* INCLUDE_JPL */
                }
#endif /* A2DP_SINK */
            }

            /* Set Application Stream State to FALSE */
            appl_a2dp_instance[codec_instance].stream_state = APPL_A2DP_STREAM_FALSE;

            appl_a2dp_connect_info_reset(codec_instance);

            /* Shutdown the Pl */
            a2dp_shutdown_pl();
        }

        break;

    case A2DP_DISCONNECT_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received A2DP_DISCONNECT_IND\n");
        LOG_DEBUG("\tCodec Instance = %d\n", codec_instance);
        LOG_DEBUG("\tEvent Result   = 0x%04X\n", event_result);

        /* Reset Connection Info */
        if (API_SUCCESS == event_result)
        {
            if ((AVDTP_CODEC_AUDIO_SBC == appl_a2dp_instance[codec_instance].codec_type) &&
                (APPL_A2DP_STREAM_TRUE == appl_a2dp_instance[codec_instance].stream_state))
            {
                /* Stop the Pl */
                a2dp_stop_pl(role);

#ifdef A2DP_SINK
                if (AVDTP_SEP_SINK == role)
                {
#ifdef INCLUDE_JPL
                    /* Stop JPL */
                    (BT_IGNORE_RETURN_VALUE) BT_jpl_stop();
#endif /* INCLUDE_JPL */
                }
#endif /* A2DP_SINK */
            }

            /* Set Application Stream State to FALSE */
            appl_a2dp_instance[codec_instance].stream_state = APPL_A2DP_STREAM_FALSE;

            appl_a2dp_connect_info_reset(codec_instance);

            /* Shutdown the Pl */
            a2dp_shutdown_pl();
        }

        break;

#ifdef A2DP_SINK
    case A2DP_MEDIA_FRAME_IND:
#if 0
        LOG_DEBUG("\n");
        LOG_DEBUG("Received A2DP_MEDIA_FRAME_IND\n");
        LOG_DEBUG("\tCodec Instance = %d\n", codec_instance);
        LOG_DEBUG("\tEvent Result   = 0x%04X\n", event_result);
        LOG_DEBUG ("Media Packet Length : %d\n", event_datalen);

        /* Print Sequence Number and Time Stamp */
        {
            UINT16 seq_number;
            UINT32 timestamp;

            BT_UNPACK_BE_2_BYTE (&seq_number, ((UCHAR *)(event_data) + 2));
            BT_UNPACK_BE_4_BYTE (&timestamp, ((UCHAR *)(event_data) + 4));

            LOG_DEBUG("\tSequence Number = %d\n", seq_number);
            LOG_DEBUG("\tTimestamp       = %d\n", timestamp);
        }
#endif /* 0 */

        /* Drop media if not on current playback instance */
        if (codec_instance != appl_a2dp_playback_instance)
        {
            break;
        }

        if (AVDTP_CODEC_AUDIO_SBC == appl_a2dp_instance[codec_instance].codec_type)
        {
            UINT32 timestamp;
            UCHAR * data;
            UINT16 seq_number;

            /* LOG_DEBUG(". "); */

            /**
             *   Media Packet: 12 Octets of RTP header, followed by Media Payload
             *   For SBC, the Media Payload will have one byte of header containing
             *   number of frames, followed by SBC Frames.
             */
            data = (UCHAR *)event_data;


            /* Extract Sequence Number */
            seq_number = data[2U];
            seq_number <<= 8U;
            seq_number |= data[3U];

            /* Extract Time Stamp */
            timestamp = data[4U];
            timestamp <<= 8U;
            timestamp |= data[5U];
            timestamp <<= 8U;
            timestamp |= data[6U];
            timestamp <<= 8U;
            timestamp |= data[7U];

#ifdef AVDTP_HAVE_CONTENT_PROTECTION
            /* Check the content Protection is enabled */
            if (AVDTP_CONTENT_PROTECTION_TYPE_SCMS_T == appl_a2dp_instance[codec_instance].cp_type)
            {
                /* Skip the content proection header */
                data += 1U;
            }
#endif /* AVDTP_HAVE_CONTENT_PROTECTION */

#ifdef INCLUDE_JPL
            /**
             * Add SBC media frames to JPL.
             *
             * JPL to be provided with the one byte of SBC header,
             * along with the SBC frames.
             */
            retval = BT_jpl_add_frames
                     (
                         seq_number,
                         timestamp,
                         (&data[12U]),
                         (event_datalen - 12U)
                     );
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("JPL Add Frames Failed - 0x%04X\n", retval);
            }
#endif /* INCLUDE_JPL */
        }

        break;
#endif /* A2DP_SINK */

#ifdef A2DP_1_3
    case A2DP_DELAY_REPORT_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received A2DP_DELAY_REPORT_IND\n");
        LOG_DEBUG("\tCodec Instance = %d\n", codec_instance);
        LOG_DEBUG("\tEvent Result   = 0x%04X\n", event_result);

        /* Delay Report is in 2 octet (Big Endian format) */
        delay_report_data = ((UCHAR *)event_data)[0U];
        delay_report_data = (delay_report_data << 8U) | ((UCHAR *)event_data)[1U];

        LOG_DEBUG ("\tA2DP Delay Report Value: %d\n", ((INT16)delay_report_data));
        break;

  case A2DP_DELAY_REPORT_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received A2DP_DELAY_REPORT_CNF\n");
        LOG_DEBUG("\tCodec Instance = %d\n", codec_instance);
        LOG_DEBUG("\tEvent Result   = 0x%04X\n", event_result);
        break;
#endif /* A2DP_1_3 */

    default:
        retval = API_FAILURE;
        break;
    }

    return retval;
}


void appl_a2dp_avdtp_connect (void)
{
    API_RESULT retval;
    UCHAR acl_indx;

    /* Get Remote BD_ADDR */
    retval = appl_a2dp_get_acl_connection (&acl_indx);
    if (API_SUCCESS != retval)
    {
        LOG_DEBUG("Invalid ACL Handle\n");
        /* return; */
    }
    else
    {
        /* Initialize AVDTP Handle */
        AVDTP_INIT_HANDLE (appl_a2dp_avdtp_handle[acl_indx]);

        /* Set AVDTP Handle Callback */
        AVDTP_SET_HANDLE_CALLBACK
        (
            appl_a2dp_avdtp_handle[acl_indx],
            appl_a2dp_avdtp_notify_cb
        );

        /* Set AVDTP Remote BD_ADDR */
        AVDTP_SET_HANDLE_BD_ADDR
        (
            appl_a2dp_avdtp_handle[acl_indx],
            appl_a2dp_acl_list[acl_indx].bd_addr
        );

        /* AVDTP Connect */
        LOG_DEBUG("AVDTP Connect ... "); fflush (stdout);
        retval = BT_avdtp_connect_req
                 (
                     &appl_a2dp_avdtp_handle[acl_indx]
                 );

        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("FAILED. Reason = 0x%04X\n", retval);
        }
        else
        {
            LOG_DEBUG("OK. Wait for AVDTP_CONNECT_CNF\n");
        }
    }

    return;
}

#ifdef AVDTP_ASSISTIVE_MODE

void appl_a2dp_get_media_channel_params (void)
{
    API_RESULT retval;
    UCHAR indx;

    /* Choose an Instance */
    appl_a2dp_get_instance (&indx);
    if (indx >= A2DP_MAX_CODECS)
    {
        LOG_DEBUG("Instance is out of Range\n");
        /* return; */
    }
    else
    {
        if (0x00U == appl_a2dp_instance[indx].allocated)
        {
            LOG_DEBUG("Instance is not registered\n");
            /* return; */
        }
        else
        {
            LOG_DEBUG("A2DP Media Channel Params ... "); fflush (stdout);
            retval = BT_a2dp_get_media_channel_params
                     (
                         appl_a2dp_instance[indx].a2dp_instance,
                         &appl_a2dp_media_ch_params
                     );

            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("FAILED. Reason = 0x%04X\n", retval);
            }
            else
            {
                LOG_DEBUG("Media Channel params:\n");
                LOG_DEBUG("Media Channel ID: 0x%04x\n", appl_a2dp_media_ch_params.lcid);
                LOG_DEBUG("Configured MTU: 0x%04x \n",  appl_a2dp_media_ch_params.mtu);
            }
        }
    }

    return;
}

#endif /* AVDTP_ASSISTIVE_MODE */


void appl_a2dp_avdtp_discover (void)
{
    API_RESULT retval;
    UCHAR acl_indx;

    /* Get Remote BD_ADDR */
    retval = appl_a2dp_get_acl_connection (&acl_indx);
    if (API_SUCCESS != retval)
    {
        LOG_DEBUG("Invalid ACL Handle\n");
        /* return; */
    }
    else
    {
        /* Initialize AVDTP Handle */
        AVDTP_INIT_HANDLE (appl_a2dp_avdtp_handle[acl_indx]);

        /* Set AVDTP Handle Callback */
        AVDTP_SET_HANDLE_CALLBACK
        (
            appl_a2dp_avdtp_handle[acl_indx],
            appl_a2dp_avdtp_notify_cb
        );

        /* Set AVDTP Remote BD_ADDR */
        AVDTP_SET_HANDLE_BD_ADDR
        (
            appl_a2dp_avdtp_handle[acl_indx],
            appl_a2dp_acl_list[acl_indx].bd_addr
        );

        /* AVDTP Discover */
        LOG_DEBUG("AVDTP Discover ... "); fflush (stdout);
        retval = BT_avdtp_discover_req
                 (
                     &appl_a2dp_avdtp_handle[acl_indx],
                     appl_a2dp_sep_info_buf,
                     32U
                 );

        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("FAILED. Reason = 0x%04X\n", retval);
        }
        else
        {
            LOG_DEBUG("OK. Wait for AVDTP_DISCOVER_CNF\n");
        }
    }

    return;
}


void appl_a2dp_avdtp_get_capabilities (void)
{
    int choice;
    API_RESULT retval;
    UCHAR remote_seid;
    UCHAR acl_indx;

    /* Get Remote BD_ADDR */
    retval = appl_a2dp_get_acl_connection (&acl_indx);
    if (API_SUCCESS != retval)
    {
        LOG_DEBUG("Invalid ACL Handle\n");
        /* return; */
    }
    else
    {
        /* Initialize AVDTP Handle */
        AVDTP_INIT_HANDLE (appl_a2dp_avdtp_handle[acl_indx]);

        /* Set AVDTP Handle Callback */
        AVDTP_SET_HANDLE_CALLBACK
        (
            appl_a2dp_avdtp_handle[acl_indx],
            appl_a2dp_avdtp_notify_cb
        );

        /* Set AVDTP Remote BD_ADDR */
        AVDTP_SET_HANDLE_BD_ADDR
        (
            appl_a2dp_avdtp_handle[acl_indx],
            appl_a2dp_acl_list[acl_indx].bd_addr
        );

        LOG_DEBUG("Enter Remote SEID (0x) = "); fflush (stdout);
        scanf("%x", &choice);
        remote_seid = (UCHAR) choice;

        /* Set AVDTP Handle Remote SEID */
        AVDTP_SET_HANDLE_REMOTE_SEID (appl_a2dp_avdtp_handle[acl_indx], remote_seid);

        /* AVDTP Get Capabilities */
        LOG_DEBUG("AVDTP Get Capabilities ... "); fflush (stdout);
        retval = BT_avdtp_get_capabilities_req
                 (
                     &appl_a2dp_avdtp_handle[acl_indx],
                     appl_a2dp_sep_cap_buf,
                     50U
                 );

        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("FAILED. Reason = 0x%04X\n", retval);
        }
        else
        {
            LOG_DEBUG("OK. Wait for AVDTP_GET_CAPABILITIES_CNF\n");
        }
    }

    return;
}


void appl_a2dp_avdtp_disconnect (void)
{
    API_RESULT retval;
    UCHAR acl_indx;

    /* Get Remote BD_ADDR */
    retval = appl_a2dp_get_acl_connection (&acl_indx);
    if (API_SUCCESS != retval)
    {
        LOG_DEBUG("Invalid ACL Handle\n");
        /* return; */
    }
    else
    {
        /* Initialize AVDTP Handle */
        AVDTP_INIT_HANDLE (appl_a2dp_avdtp_handle[acl_indx]);

        /* Set AVDTP Handle Callback */
        AVDTP_SET_HANDLE_CALLBACK
        (
            appl_a2dp_avdtp_handle[acl_indx],
            appl_a2dp_avdtp_notify_cb
        );

        /* Set AVDTP Remote BD_ADDR */
        AVDTP_SET_HANDLE_BD_ADDR
        (
            appl_a2dp_avdtp_handle[acl_indx],
            appl_a2dp_acl_list[acl_indx].bd_addr
        );

        /* AVDTP Disconnect */
        LOG_DEBUG("AVDTP Disconnect ... "); fflush (stdout);
        retval = BT_avdtp_disconnect_req
                 (
                    &appl_a2dp_avdtp_handle[acl_indx]
                 );

        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("FAILED. Reason = 0x%04X\n", retval);
        }
        else
        {
            LOG_DEBUG("OK. Wait for AVDTP_DISCONNECT_CNF\n");
        }
    }

    return;
}

#ifdef AVDTP_HAVE_GET_ALL_CAPABILITIES
void appl_a2dp_avdtp_get_all_capabilities (void)
{
    int choice;
    API_RESULT retval;
    UCHAR remote_seid;
    UCHAR acl_indx;

    /* Get Remote BD_ADDR */
    retval = appl_a2dp_get_acl_connection (&acl_indx);
    if (API_SUCCESS != retval)
    {
        LOG_DEBUG("Invalid ACL Handle\n");
        /* return; */
    }
    else
    {
        /* Initialize AVDTP Handle */
        AVDTP_INIT_HANDLE (appl_a2dp_avdtp_handle[acl_indx]);

        /* Set AVDTP Handle Callback */
        AVDTP_SET_HANDLE_CALLBACK
        (
            appl_a2dp_avdtp_handle[acl_indx],
            appl_a2dp_avdtp_notify_cb
        );

        /* Set AVDTP Remote BD_ADDR */
        AVDTP_SET_HANDLE_BD_ADDR
        (
            appl_a2dp_avdtp_handle[acl_indx],
            appl_a2dp_acl_list[acl_indx].bd_addr
        );

        LOG_DEBUG("Enter Remote SEID (0x) = "); fflush (stdout);
        scanf("%x", &choice);
        remote_seid = (UCHAR) choice;

        /* Set AVDTP Handle Remote SEID */
        AVDTP_SET_HANDLE_REMOTE_SEID (appl_a2dp_avdtp_handle[acl_indx], remote_seid);

        /* AVDTP Get All Capabilities */
        LOG_DEBUG("AVDTP Get All Capabilities ... "); fflush (stdout);
        retval = BT_avdtp_get_all_capabilities_req
                 (
                     &appl_a2dp_avdtp_handle[acl_indx],
                     appl_a2dp_sep_cap_buf,
                     50U
                 );

        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("FAILED. Reason = 0x%04X\n", retval);
        }
        else
        {
            LOG_DEBUG("OK. Wait for AVDTP_GET_ALL_CAPABILITIES_CNF\n");
        }
    }

    return;
}
#endif /* AVDTP_HAVE_GET_ALL_CAPABILITIES */

#if ((defined A2DP_1_3) && (defined A2DP_SINK))
void appl_a2dp_set_initial_delay_report (UCHAR inst)
{
    API_RESULT retval;
    int        delay_val;

    retval = API_SUCCESS;

    /* Set delay report only if SEP is AVDTP SINK  */
    if (AVDTP_SEP_SINK == appl_a2dp_instance[inst].sep_cap.sep_type)
    {
        LOG_DEBUG ("Enter Delay Report Value(in decimal frmt[multiple of 0.1 milliseconds])\n");
        scanf ("%d", &delay_val);

        retval = BT_a2dp_set_initial_delay_report
                 (
                     inst,
                     (INT16)delay_val
                 );
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("FAILED. Reason = 0x%04X\n", retval);
        }
        else
        {
            LOG_DEBUG ("Updated Initial Delay Report\n");
        }
    }
    else
    {
        LOG_DEBUG ("\nInvalid SEP_TYPE to Set Delay Report\n");
    }

    return;
}

void appl_a2dp_send_delay_report (UCHAR inst)
{
    API_RESULT retval;
    int        delay_val;

    retval = API_SUCCESS;

    /* Send delay report only if SEP is AVDTP SINK  */
    if (AVDTP_SEP_SINK == appl_a2dp_instance[inst].sep_cap.sep_type)
    {
        LOG_DEBUG ("Enter Delay Report Value(in decimal frmt[multiple of 0.1 milliseconds])\n");
        scanf ("%d", &delay_val);

        /* Sample Delay Report Data */
        appl_a2dp_delay_report = (INT16) delay_val;

        /* AVDTP Send Delay Report */
        LOG_DEBUG("AVDTP Send Delay Report... "); fflush (stdout);
        retval = BT_a2dp_send_delay_report
                 (
                     appl_a2dp_instance[inst].a2dp_instance,
                     &appl_a2dp_delay_report
                 );
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("FAILED. Reason = 0x%04X\n", retval);
        }
        else
        {
            LOG_DEBUG("OK. Wait for AVDTP_DELAY_REPORT_CNF\n");
        }
    }
    else
    {
        LOG_DEBUG ("\nInvalid SEP_TYPE to Send Delay Report\n");
    }

    return;
}
#endif /* A2DP_1_3 && A2DP_SINK */

API_RESULT appl_a2dp_avdtp_notify_cb
           (
               UCHAR             event_type,
               UINT16            event_result,
               AVDTP_HANDLE *    avdtp_handle,
               void *            event_data,
               UINT16            event_datalen
           )
{
    UCHAR * rsp_buf;
    UCHAR * codec_ie;
    API_RESULT retval;
    UINT16 count, codec_ie_len, i;
    AVDTP_SEP_CAP sep_cap;
    AVDTP_SEP_INFO sep_info;

    switch (event_type)
    {
    case AVDTP_CONNECT_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received AVDTP_CONNECT_IND\n");
        LOG_DEBUG("\tAVDTP Handle:\n");
        LOG_DEBUG("\t    " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (avdtp_handle->bd_addr));
        LOG_DEBUG("\tResult = 0x%04X\n", event_result);

        break;

    case AVDTP_CONNECT_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received AVDTP_CONNECT_CNF\n");
        LOG_DEBUG("\tAVDTP Handle:\n");
        LOG_DEBUG("\t    " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (avdtp_handle->bd_addr));
        LOG_DEBUG("\tResult = 0x%04X\n", event_result);

        break;

    case AVDTP_DISCONNECT_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received AVDTP_DISCONNECT_IND\n");
        LOG_DEBUG("\tAVDTP Handle:\n");
        LOG_DEBUG("\t    " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (avdtp_handle->bd_addr));
        LOG_DEBUG("\tResult = 0x%04X\n", event_result);

        if (API_SUCCESS == event_result)
        {
            for (count = 0U; count < A2DP_MAX_CODECS; count ++)
            {
                if (BT_TRUE == BT_COMPARE_ADDR(appl_a2dp_instance[count].bd_addr, avdtp_handle->bd_addr))
                {
                    /* Reset BD Address */
                    BT_mem_set
                    (
                        appl_a2dp_instance[count].bd_addr,
                        0x00,
                        BT_BD_ADDR_SIZE
                    );

                    break;
                }
            }
        }

        break;

    case AVDTP_DISCONNECT_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received AVDTP_DISCONNECT_CNF\n");
        LOG_DEBUG("\tAVDTP Handle:\n");
        LOG_DEBUG("\t    " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (avdtp_handle->bd_addr));
        LOG_DEBUG("\tResult = 0x%04X\n", event_result);

        if (API_SUCCESS == event_result)
        {
            for (count = 0U; count < A2DP_MAX_CODECS; count ++)
            {
                if (BT_TRUE == BT_COMPARE_ADDR(appl_a2dp_instance[count].bd_addr, avdtp_handle->bd_addr))
                {
                    /* Reset BD Address */
                    BT_mem_set
                    (
                        appl_a2dp_instance[count].bd_addr,
                        0x00,
                        BT_BD_ADDR_SIZE
                    );

                    break;
                }
            }
        }

        break;

    case AVDTP_DISCOVER_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received AVDTP_DISCOVER_CNF\n");
        LOG_DEBUG("\tAVDTP Handle:\n");
        LOG_DEBUG("\t    " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (avdtp_handle->bd_addr));
        LOG_DEBUG("\t    Remote SEID = 0x%02X (%d)\n",
        avdtp_handle->remote_seid, avdtp_handle->remote_seid);
        LOG_DEBUG("\tResult = 0x%04X\n", event_result);

        if (API_SUCCESS != event_result)
        {
            LOG_DEBUG("*** Discover RSP Error Code = 0x%04X\n",
            event_result);
            break;
        }

        LOG_DEBUG("\tEvent Data = %p, Data Length = %d\n",
        event_data, event_datalen);

        rsp_buf = (UCHAR *) event_data;
        for (count = 0U; count < event_datalen; count += 2U)
        {
            LOG_DEBUG("Response Buf [0] = 0x%02X, [1] = 0x%02X\n",
            rsp_buf[count + 0U], rsp_buf[count + 1U]);

            retval = BT_avdtp_decode_sep_information
                     (
                         &rsp_buf[count],
                         (UINT16) (event_datalen - count),
                         &sep_info
                     );

            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("*** FAILED to Decode SEP Information (0x%04X)\n",
                retval);
            }
            else
            {
                LOG_DEBUG("\tRemote SEP Information [%d]:\n", count);

                /* SEP In Use */
                LOG_DEBUG("\t    In Use     = %s\n",
                ((0x1U == sep_info.in_use) ? "Yes" : "No"));

                /* SEP Type */
                LOG_DEBUG("\t    SEP Type   = 0x%02X -> ", sep_info.sep_type);
                switch (sep_info.sep_type)
                {
                case AVDTP_SEP_SOURCE:
                    LOG_DEBUG("Source\n");
                    break;

                case AVDTP_SEP_SINK:
                    LOG_DEBUG("Sink\n");
                    break;

                default:
                    LOG_DEBUG("????\n");
                    break;
                }

                /* ACP SEID */
                LOG_DEBUG("\t    ACP SEID   = 0x%02X\n", sep_info.acp_seid);

                /* Media Type */
                LOG_DEBUG("\t    Media Type = 0x%02X -> ", sep_info.media_type);
                switch (sep_info.media_type)
                {
                case AVDTP_MEDIA_AUDIO:
                    LOG_DEBUG("Audio\n");
                    break;

                case AVDTP_MEDIA_VIDEO:
                    LOG_DEBUG("Video\n");
                    break;

                case AVDTP_MEDIA_MULTIMEDIA:
                    LOG_DEBUG("Multi\n");
                    break;

                default:
                    LOG_DEBUG("????\n");
                    break;
                }
            }
        }

        break;

    case AVDTP_GET_CAPABILITIES_CNF:
#ifdef AVDTP_HAVE_GET_ALL_CAPABILITIES
    case AVDTP_GET_ALL_CAPABILITIES_CNF:
#endif /* AVDTP_HAVE_GET_ALL_CAPABILITIES */

        LOG_DEBUG("Received %s\n",(AVDTP_GET_CAPABILITIES_CNF == event_type)?
            "AVDTP_GET_CAPABILITIES_CNF":"AVDTP_GET_ALL_CAPABILITIES_CNF");


        LOG_DEBUG("\tAVDTP Handle:\n");
        LOG_DEBUG("\t    " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (avdtp_handle->bd_addr));
        LOG_DEBUG("\t    Remote SEID = 0x%02X (%d)\n",
        avdtp_handle->remote_seid, avdtp_handle->remote_seid);
        LOG_DEBUG("\tResult = 0x%04X\n", event_result);

        if (API_SUCCESS != event_result)
        {
            LOG_DEBUG("*** Get Capabilities RSP Error Code = 0x%04X\n",
            event_result);
            break;
        }

        LOG_DEBUG("\tEvent Data = %p, Data Length = %d\n",
        event_data, event_datalen);

        /* Decode SEP Capabilities - Codec Capabilities */
        rsp_buf = (UCHAR *) event_data;
        retval = BT_avdtp_decode_sep_capabilities
                 (
                     rsp_buf,
                     event_datalen,
                     &sep_cap
                 );

        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("*** FAILED to Decode SEP Capabilities = 0x%04X\n", retval);
            break;
        }

        LOG_DEBUG("\tRemote SEP Capabilities:\n");

        /* Media Type */
        LOG_DEBUG("\t    Media Type = 0x%02X -> ", sep_cap.codec_cap.media_type);
        switch (sep_cap.codec_cap.media_type)
        {
        case AVDTP_MEDIA_AUDIO:
            LOG_DEBUG("Audio\n");
            break;

        case AVDTP_MEDIA_VIDEO:
            LOG_DEBUG("Video\n");
            break;

        case AVDTP_MEDIA_MULTIMEDIA:
            LOG_DEBUG("Multi\n");
            break;

        default:
            LOG_DEBUG("????\n");
            break;
        }

        /* Codec Type */
        LOG_DEBUG("\t    Codec Type = 0x%02X -> ", sep_cap.codec_cap.codec_type);
        switch (sep_cap.codec_cap.codec_type)
        {
        case AVDTP_CODEC_AUDIO_SBC:
            LOG_DEBUG("SBC\n");
            break;

        case AVDTP_CODEC_AUDIO_MPEG_1_2:
            LOG_DEBUG("MPG12\n");
            break;

        case AVDTP_CODEC_AUDIO_MPEG_2_4:
            LOG_DEBUG("MPG24\n");
            break;

        case AVDTP_CODEC_AUDIO_ATRAC:
            LOG_DEBUG("ATRAC\n");
            break;

        case AVDTP_CODEC_AUDIO_VENDOR_SPECIFIC:
            LOG_DEBUG("VENDOR_SPECIFIC\n");
            break;

        default:
            LOG_DEBUG("????\n");
            break;
        }

        /* Codec IE */
        codec_ie = sep_cap.codec_cap.codec_ie;
        codec_ie_len = sep_cap.codec_cap.codec_ie_len;

        LOG_DEBUG("\t    Codec IE   = "); fflush (stdout);
        for (count = 0U; count < codec_ie_len; count ++)
        {
            LOG_DEBUG("0x%02X ", codec_ie[count]);
        }
        LOG_DEBUG("\n");

        /* Print Codec Capabilities */
        appl_a2dp_print_codec_ie_capabilities
        (
            sep_cap.codec_cap.codec_type,
            codec_ie,
            codec_ie_len
        );

#ifdef AVDTP_HAVE_CONTENT_PROTECTION
        if (AVDTP_INVALID_CP_TYPE != sep_cap.cp_cap[0U].cp_type)
        {
            printf("\tContent Protection Type:0x%04x\n", sep_cap.cp_cap[0U].cp_type);
            printf("\tCP_IE Length:0x%02x\n", sep_cap.cp_cap[0U].cp_ie_len);
            if (0U != sep_cap.cp_cap[0U].cp_ie_len)
            {
                printf("\tCP_IE:");
                for (i = 0U; i < sep_cap.cp_cap[0U].cp_ie_len; i++)
                {
                    printf("0x%02x ", sep_cap.cp_cap[0U].cp_ie[i]);
                }
                printf("\n");
            }
        }
#endif /* AVDTP_HAVE_CONTENT_PROTECTION */

#ifdef AVDTP_HAVE_DELAY_REPORTING
        /* Check for delay report support in peer device */
        if ((AVDTP_GET_ALL_CAPABILITIES_CNF == event_type) &&
            (BT_TRUE == sep_cap.dr_cap_flag))
        {
            LOG_DEBUG ("\tDelay Reporting Service: YES \n");

            /*
             * This flag is used to enable delay report configuration if
             * connection is initiated from local size
             */
            appl_a2dp_peer_dr_cap_flag = BT_TRUE;
        }
        else
        {
            /*
             * This flag is used to enable delay report configuration if
             * connection is initiated from local size
             */
            appl_a2dp_peer_dr_cap_flag = BT_FALSE;
        }
#endif /* AVDTP_HAVE_DELAY_REPORTING */

        /* Show Instances */
        appl_a2dp_show_instance();

        break;

    default:
        LOG_DEBUG("\n");
        LOG_DEBUG("*** Received Unknown Event 0x%02X. Result = 0x%04X\n",
        event_type, event_result);
        break;
    }

    return API_SUCCESS;
}


void appl_a2dp_show_instance (void)
{
    UINT32 indx;

    LOG_DEBUG("\n");
    LOG_DEBUG("       SEP     Media  Codec    Remote\n");
    LOG_DEBUG("Index  Type    Type   Type     BD_ADDR                   State\n");
    LOG_DEBUG("-----  ------  -----  -----    -------------------       -----\n");

    for (indx = 0U; indx < A2DP_MAX_CODECS; indx ++)
    {
        if (0x0U == appl_a2dp_instance[indx].allocated)
        {
            LOG_DEBUG("%3d    ", indx);
            LOG_DEBUG("--      ");
            LOG_DEBUG("--     ");
            LOG_DEBUG("--       ");
            LOG_DEBUG("--:--:--:--:--:--  ");
            LOG_DEBUG("      NOT ALLOCATED\n");
            continue;
        }

        /* Index */
        LOG_DEBUG("%3d    ", indx);

        /* SEP Type */
        switch (appl_a2dp_instance[indx].sep_cap.sep_type)
        {
        case AVDTP_SEP_SOURCE:
            LOG_DEBUG("Source  ");
            break;

        case AVDTP_SEP_SINK:
            LOG_DEBUG("Sink    ");
            break;

        default:
            LOG_DEBUG("????    ");
            break;
        }

        /* Media Type */
        switch (appl_a2dp_instance[indx].sep_cap.codec_cap.media_type)
        {
        case AVDTP_MEDIA_AUDIO:
            LOG_DEBUG("Audio  ");
            break;

        case AVDTP_MEDIA_VIDEO:
            LOG_DEBUG("Video  ");
            break;

        case AVDTP_MEDIA_MULTIMEDIA:
            LOG_DEBUG("Multi  ");
            break;

        default:
            LOG_DEBUG("????    ");
            break;
        }

        /* Codec Type */
        switch (appl_a2dp_instance[indx].sep_cap.codec_cap.codec_type)
        {
        case AVDTP_CODEC_AUDIO_SBC:
            LOG_DEBUG("SBC      ");
            break;

        case AVDTP_CODEC_AUDIO_MPEG_1_2:
            LOG_DEBUG("MPG12    ");
            break;

        case AVDTP_CODEC_AUDIO_MPEG_2_4:
            LOG_DEBUG("MPG24    ");
            break;

        case AVDTP_CODEC_AUDIO_ATRAC:
            LOG_DEBUG("ATRAC    ");
            break;

        case AVDTP_CODEC_AUDIO_VENDOR_SPECIFIC:
            LOG_DEBUG("VENDOR   ");
            break;

        default:
            LOG_DEBUG("????    ");
            break;
        }


        /* Remote BD_ADDR */
        LOG_DEBUG(BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "  ",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (appl_a2dp_instance[indx].bd_addr));

        /* State */
        LOG_DEBUG("\n");
    }

    LOG_DEBUG("\n");
}

void appl_a2dp_get_instance (UCHAR * indx)
{
    int choice;

    /* Show Instances */
    appl_a2dp_show_instance();
    BT_LOOP_FOREVER()
    {
        LOG_DEBUG("Choose A2DP Instance = "); fflush(stdout);
        scanf("%d", &choice);
        if ((choice >= 0) || (choice < A2DP_MAX_CODECS))
        {
            break;
        }
    }

    *indx = (UCHAR)choice;
    return;
}

void appl_a2dp_alloc_instance (UCHAR * indx)
{
    UINT32 count;

    *indx = A2DP_MAX_CODECS;
    for (count = 0U; count < A2DP_MAX_CODECS; count++)
    {
        if (0x0U == appl_a2dp_instance[count].allocated)
        {
            appl_a2dp_instance[count].allocated = 0x1U;
            *indx = (UCHAR )count;
            break; /* return; */
        }
    }

    return;
}

void appl_a2dp_free_instance (UCHAR indx)
{
    appl_a2dp_instance[indx].allocated = 0x0U;
    BT_mem_set(appl_a2dp_instance[indx].bd_addr, 0x00, BT_BD_ADDR_SIZE);
}


API_RESULT appl_a2dp_get_acl_connection (UCHAR * indx)
{
    int choice;
    API_RESULT retval;
    UCHAR i, num_list;

    /* Init */
    retval = API_SUCCESS;

    retval = BT_hci_get_connection_details
             (appl_a2dp_acl_list, BT_MAX_REMOTE_DEVICES, &num_list);

    if (API_SUCCESS != retval)
    {
         LOG_DEBUG("*** HCI Get Connection Details FAILED !! "\
         "Error Code = 0x%04X\n", retval);
         retval = API_FAILURE; /*  return API_FAILURE; */
    }
    else
    {
        if (0U == num_list)
        {
            LOG_DEBUG("*** ACL Connection to a Remote Device NOT Available\n");
            retval = API_FAILURE; /* return API_FAILURE; */
        }
        else
        {
            LOG_DEBUG("\n");
            LOG_DEBUG("==========================================================\n");
            LOG_DEBUG("Active ACL Connections\n");
            LOG_DEBUG("----------------------------------------------------------\n");
            for (i = 0U; i < num_list; i ++)
            {
                LOG_DEBUG("  [%2d]  ", i);
                LOG_DEBUG(BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "   ",
                BT_DEVICE_ADDR_ONLY_PRINT_STR (appl_a2dp_acl_list[i].bd_addr));

                LOG_DEBUG("ACL Handle = 0x%04X\n",
                appl_a2dp_acl_list[i].acl_handle);
            }
            LOG_DEBUG("==========================================================\n");
            LOG_DEBUG("\n");
        }
    }

    if (API_SUCCESS == retval)
    {
        LOG_DEBUG("Choose ACL Connection Index = "); fflush (stdout);
        scanf("%d", &choice);
        if ((UINT16) choice >= BT_MAX_REMOTE_DEVICES)
        {
            retval = API_FAILURE; /* return API_FAILURE; */
        }
        else
        {
            *indx = (UCHAR) choice;
        }
    }

    return retval;
}


INT32 appl_a2dp_read_sbc_configuration
      (
          CHAR *     filename,
          UCHAR *    codec_ie
      )
{
    int option;
    int minbitpool = 0, maxbitpool = 0;
    INT32 retval;
    CHAR config_buf [BT_CONFIG_MAX_CHARS_PER_LINE];

    /* Initialize Codec IE */
    codec_ie[0U] = 0x0U;
    codec_ie[1U] = 0x0U;
    codec_ie[2U] = 0x0U;
    codec_ie[3U] = 0x0U;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    BT_mem_set(config_buf, 0, sizeof(CHAR)*BT_CONFIG_MAX_CHARS_PER_LINE);

    /* Init */
    retval = 0;

    /* -------------------------- SBC Sampling Frequency */
    retval = BT_config_read (filename, "SBC_SAMPLING_FREQUENCY", config_buf);
    if (retval < 0)
    {
#if 0
        LOG_DEBUG("Read Configuration: Sampling Frequency NOT found\n");
        return retval;
#else /* 0 */
        retval = 0;

#ifdef APPL_A2DP_DEFAULT_CONFIG
#ifndef A2DP_PL_SOURCE_16KHz_SAMPLE
        LOG_DEBUG("\n*** The sample audio built along is at 48KHz and Stereo ***");
        option = 48000;
#else /* A2DP_PL_SOURCE_16KHz_SAMPLE */
        LOG_DEBUG("\n*** The sample audio built along is at 16KHz and Stereo ***");
        option = 16000;
#endif /* A2DP_PL_SOURCE_16KHz_SAMPLE */
#else /* APPL_A2DP_DEFAULT_CONFIG */
#ifndef A2DP_PL_SOURCE_16KHz_SAMPLE
        LOG_DEBUG("\n*** If local role is Source, set SF to 48000 as the sample audio built along is at 48KHz and Stereo ***");
#else /* A2DP_PL_SOURCE_16KHz_SAMPLE */
        LOG_DEBUG("\n*** If local role is Source, set SF to 16000 as the sample audio built along is at 16KHz and Stereo ***");
#endif /* A2DP_PL_SOURCE_16KHz_SAMPLE */
        LOG_DEBUG("\nEnter the Sampling Frequency (16000, 32000, 44100, 48000): ");
        scanf("%d", &option);
#endif /* APPL_A2DP_DEFAULT_CONFIG */
        LOG_DEBUG("\nEnter Sampling Frequency: %d", option);
#endif /* 0 */
    }
    else
    {
        (BT_IGNORE_RETURN_VALUE) sscanf(config_buf, "%d", &option);
        LOG_DEBUG("\nRead Configuration: Sampling Frequency -> %d\n", option);
    }

    switch (option)
    {
    case 16000:
        codec_ie[0U] |= A2DP_SBC_SAMPLING_FREQUENCY_16000;
        break;

    case 32000:
        codec_ie[0U] |= A2DP_SBC_SAMPLING_FREQUENCY_32000;
        break;

    case 44100:
        codec_ie[0U] |= A2DP_SBC_SAMPLING_FREQUENCY_44100;
        break;

    case 48000:
        codec_ie[0U] |= A2DP_SBC_SAMPLING_FREQUENCY_48000;
        break;

    default:
        LOG_DEBUG("Read Configuration: Invalid Sampling Frequency\n");
        retval = -1; /* return -1; */
    }

    if (0 == retval)
    {
        /* -------------------------- SBC Encoder Channel Mode Value */
        retval = BT_config_read (filename, "SBC_CHANNEL_MODE", config_buf);
        if (retval < 0)
        {
    #if 0
            LOG_DEBUG("Read Configuration: SBC Channel Mode NOT found\n");
            return retval;
    #else /* 0 */
            retval = 0;

    #ifdef APPL_A2DP_DEFAULT_CONFIG
            BT_str_print(config_buf, "%s", "JOINT_STEREO");
            LOG_DEBUG("\nEnter Channel Mode: %s", config_buf);
    #else /* APPL_A2DP_DEFAULT_CONFIG */
            LOG_DEBUG("\nEnter the Channel Mode (MONO, DUAL, STEREO, JOINT_STEREO): ");
            scanf("%s", config_buf);
    #endif /* APPL_A2DP_DEFAULT_CONFIG */
    #endif /* 0 */
        }
        else
        {
            LOG_DEBUG("Read Configuration: Channel Mode -> %s\n", config_buf);
        }

        if (0 == BT_str_cmp(config_buf, "MONO"))
        {
            codec_ie[0U] |= A2DP_SBC_CHANNEL_MONO;
        }
        else if (0 == BT_str_cmp(config_buf, "DUAL"))
        {
            codec_ie[0U] |= A2DP_SBC_CHANNEL_DUAL;
        }
        else if (0 == BT_str_cmp(config_buf, "JOINT_STEREO"))
        {
            codec_ie[0U] |= A2DP_SBC_CHANNEL_JOINT_STEREO;
        }
        else if (0 == BT_str_cmp(config_buf, "STEREO"))
        {
            codec_ie[0U] |= A2DP_SBC_CHANNEL_STEREO;
        }
        else
        {
            LOG_DEBUG("Read Configuration: Invalid Channel Mode\n");
            retval = -1; /* return -1; */
        }
    }

    if (0 == retval)
    {
        /* -------------------------- SBC Encoder Block Length Value */
        retval = BT_config_read (filename, "SBC_BLOCK_LEN", config_buf);
        if (retval < 0)
        {
    #if 0
            LOG_DEBUG("Read Configuration: SBC Block Length NOT found\n");
            return retval;
    #else /* 0 */
            retval = 0;
    #ifdef APPL_A2DP_DEFAULT_CONFIG
            option = 16;
            LOG_DEBUG("\nEnter Block Length: %d", option);
    #else /* APPL_A2DP_DEFAULT_CONFIG */
            LOG_DEBUG("\nEnter the Block Length (4, 8, 12, 16): ");
            scanf("%d", &option);
    #endif /* APPL_A2DP_DEFAULT_CONFIG */
    #endif /* 0 */
        }
        else
        {
            (BT_IGNORE_RETURN_VALUE) sscanf(config_buf, "%d", &option);
            LOG_DEBUG("Read Configuration: SBC Block Length -> %d\n", option);
        }

        switch (option)
        {
        case 4:
            codec_ie[1U] |= A2DP_SBC_BLOCK_LENGTH_4;
            break;

        case 8:
            codec_ie[1U] |= A2DP_SBC_BLOCK_LENGTH_8;
            break;

        case 12:
            codec_ie[1U] |= A2DP_SBC_BLOCK_LENGTH_12;
            break;

        case 16:
            codec_ie[1U] |= A2DP_SBC_BLOCK_LENGTH_16;
            break;

        default:
            LOG_DEBUG("Read Configuration: Invalid SBC Block Length\n");
            retval = -1; /* return -1; */
        }
    }

    if (0 == retval)
    {
        /* -------------------------- SBC Encoder Subband Value */
        retval = BT_config_read (filename, "SBC_SUBBAND", config_buf);
        if (retval < 0)
        {
    #if 0
            LOG_DEBUG("Read Configuration: SBC Subband NOT found\n");
            return retval;
    #else /* 0 */
            retval = 0;
    #ifdef APPL_A2DP_DEFAULT_CONFIG
            option = 8;
            LOG_DEBUG("\nEnter Num Subbands: %d", option);
    #else /* APPL_A2DP_DEFAULT_CONFIG */
            LOG_DEBUG("\nEnter the Num SubBands (4, 8): ");
            scanf("%d", &option);
    #endif /* APPL_A2DP_DEFAULT_CONFIG */
    #endif /* 0 */

        }
        else
        {
            (BT_IGNORE_RETURN_VALUE) sscanf(config_buf, "%d", &option);
            LOG_DEBUG("Read Configuration: SBC Subband -> %d\n", option);
        }

        switch (option)
        {
        case 4:
            codec_ie[1U] |= A2DP_SBC_SUBBANDS_4;
            break;

        case 8:
            codec_ie[1U] |= A2DP_SBC_SUBBANDS_8;
            break;

        default:
            LOG_DEBUG("Read Configuration: Invalid SBC Subband\n");
            retval = -1; /* return -1; */
        }
    }

    if (0 == retval)
    {
        /* -------------------------- SBC Encoder Allocation Method Value */
        retval = BT_config_read (filename, "SBC_ALLOCATION_METHOD", config_buf);
        if (retval < 0)
        {
    #if 0
            LOG_DEBUG("Read Configuration: SBC Allocation Method NOT found\n");
            return retval;
    #else /* 0 */
            retval = 0;
    #ifdef APPL_A2DP_DEFAULT_CONFIG
            BT_str_print(config_buf, "%s", "LOUDNESS");
            LOG_DEBUG("\nEnter Allocation Method: %s", config_buf);
    #else /* APPL_A2DP_DEFAULT_CONFIG */
            LOG_DEBUG("\nEnter the Allocation Method (LOUDNESS, SNR): ");
            scanf("%s", config_buf);
    #endif /* APPL_A2DP_DEFAULT_CONFIG */
    #endif /* 0 */
        }
        else
        {
            LOG_DEBUG("Read Configuration: Allocation Method -> %s\n", config_buf);
        }

        if (0 == BT_str_cmp(config_buf, "LOUDNESS"))
        {
            codec_ie[1U] |= A2DP_SBC_ALLOCATION_METHOD_LOUDNESS;
        }
        else if (0 == BT_str_cmp(config_buf, "SNR"))
        {
            codec_ie[1U] |= A2DP_SBC_ALLOCATION_METHOD_SNR;
        }
        else
        {
            LOG_DEBUG("Read Configuration: Invalid Allocation Method\n");
            retval = -1; /* return -1; */
        }
    }

    if (0 == retval)
    {
        /* -------------------------- SBC Encoder Bitpool Value */
        retval = BT_config_read (filename, "SBC_BITPOOL", config_buf);
        if (retval < 0)
        {
    #if 0
            LOG_DEBUG("Read Configuration: SBC Bitpool NOT found\n");
            return retval;
    #else /* 0 */
            /* MISRA C - 2012 Rule 2.2 */
            /* retval = 0; */
            retval = 0;
    #ifdef APPL_A2DP_DEFAULT_CONFIG
            minbitpool= 18;
            printf("\nMin Bitpool: %d", minbitpool);
            maxbitpool= 35;
            printf("\nMax Bitpool: %d", maxbitpool);
    #else /* APPL_A2DP_DEFAULT_CONFIG */
            printf("\nEnter the Minimum Bitpool (2-250): ");
            scanf("%d", &minbitpool);
            printf("\nEnter the Maximum Bitpool (2-250): ");
            scanf("%d", &maxbitpool);
    #endif /* APPL_A2DP_DEFAULT_CONFIG */
    #endif /* 0 */
        }
        else
        {
            (BT_IGNORE_RETURN_VALUE) sscanf(config_buf, "%d", &maxbitpool);
            printf("Read Configuration: SBC Bitpool -> %d\n", maxbitpool);
        }

        if ((option >= A2DP_SBC_BITPOOL_MIN) &&
            (option <= A2DP_SBC_BITPOOL_MAX))
        {
            codec_ie[2U] = (UCHAR)minbitpool;
            codec_ie[3U] = (UCHAR)maxbitpool;
        }
        else
        {
            LOG_DEBUG("Read Configuration: Invalid SBC Bitpool\n");
            retval = -1; /* return -1; */
        }
    }

    LOG_DEBUG("\nRead Configuration: Codec IE = 0x%02X 0x%02X 0x%02X 0x%02X\n",
    codec_ie[0U], codec_ie[1U], codec_ie[2U], codec_ie[3U]);

    return retval;
}

#ifdef  A2DP_MPEG_1_2

INT32 appl_a2dp_read_mpeg12_configuration
      (
          CHAR *     filename,
          UCHAR *    codec_ie
      )
{
    int option;
    INT32 retval;
    CHAR config_buf [BT_CONFIG_MAX_CHARS_PER_LINE];


    /* Initialize Codec IE */
    codec_ie[0U] = 0x0U;
    codec_ie[1U] = 0x0U;
    codec_ie[2U] = 0x0U;
    codec_ie[3U] = 0x0U;

    retval = 0;

    LOG_DEBUG ("===== Read Configuration of MPEG-1,2 Codec =====\n");

    /* -------------------------- MPEG12 Layer Value */
    retval = BT_config_read (filename, "MPEG12_LAYER", config_buf);
    if (retval < 0)
    {
        LOG_DEBUG("\t    Layer NOT found\n");
        /* return retval; */
    }
    else
    {
        LOG_DEBUG("\t    Layer -> %s\n", config_buf);

        if (0 == BT_str_cmp(config_buf, "I"))
        {
            codec_ie[0U] |= A2DP_MPEG_1_2_LAYER_I;
        }
        else if (0 == BT_str_cmp(config_buf, "II"))
        {
            codec_ie[0U] |= A2DP_MPEG_1_2_LAYER_II;

        }
        else if (0 == BT_str_cmp(config_buf, "III"))
        {
            codec_ie[0U] |= A2DP_MPEG_1_2_LAYER_III;
        }
    }

    if (retval >= 0)
    {
        /* -------------------------- MPEG12 CRC Value */
        retval = BT_config_read (filename, "MPEG12_CRC", config_buf);
        if (retval < 0)
        {
            LOG_DEBUG("\t    CRC NOT found\n");
            /* return retval; */
        }
        else
        {
            LOG_DEBUG("\t    CRC -> %s\n", config_buf);

            if (0 == BT_str_cmp(config_buf, "ON"))
            {
                codec_ie[0U] |= A2DP_MPEG_1_2_CRC_ON;
            }
            else if (0 == BT_str_cmp(config_buf, "OFF"))
            {
                codec_ie[0U] |= A2DP_MPEG_1_2_CRC_OFF;
            }
        }
    }

    if (retval >= 0)
    {
        /* -------------------------- MPEG12 Channel Mode */
        retval = BT_config_read (filename, "MPEG12_CHANNEL_MODE", config_buf);
        if (retval < 0)
        {
            LOG_DEBUG("\t    Channel Mode NOT found\n");
            /* return retval; */
        }
        else
        {
            LOG_DEBUG("\t    Channel Mode -> %s\n", config_buf);

            if (0 == BT_str_cmp(config_buf, "MONO"))
            {
                codec_ie[0U] |= A2DP_MPEG_1_2_CHANNEL_MONO;
            }
            else if (0 == BT_str_cmp(config_buf, "DUAL"))
            {
                codec_ie[0U] |= A2DP_MPEG_1_2_CHANNEL_DUAL;
            }
            else if (0 == BT_str_cmp(config_buf, "STEREO"))
            {
                codec_ie[0U] |= A2DP_MPEG_1_2_CHANNEL_STEREO;
            }
            else if (0 == BT_str_cmp(config_buf, "JOINT_STEREO"))
            {
                codec_ie[0U] |= A2DP_MPEG_1_2_CHANNEL_JOINT_STEREO;
            }
        }
    }

    if (retval >= 0)
    {
        /* -------------------------- MPEG12 Media Payload Format */
        retval = BT_config_read (filename, "MPEG12_MPF", config_buf);
        if (retval < 0)
        {
            LOG_DEBUG("\t    Media Payload Format NOT found\n");
            /* return retval; */
        }
        else
        {
            LOG_DEBUG("\t    MPF -> %s\n", config_buf);

            if (0 == BT_str_cmp(config_buf, "MPF-1"))
            {
                codec_ie[1U] |= A2DP_MPEG_1_2_MPF_1;
            }
            else if (0 == BT_str_cmp(config_buf, "MPF-2"))
            {
                codec_ie[1U] |= A2DP_MPEG_1_2_MPF_2;
            }
        }
    }

    if (retval >= 0)
    {
        /* -------------------------- MPEG12 Sampling Frequeny */
        retval = BT_config_read (filename, "MPEG12_SAMPLING_FREQUENCY", config_buf);
        if (retval < 0)
        {
            LOG_DEBUG("\t    Sampling Frequeny NOT found\n");
            /* return retval; */
        }
        else
        {
            sscanf(config_buf, "%d", &option);
            LOG_DEBUG("\t    Sampling Frequeny -> %d\n", option);

            switch(option)
            {
            case 16000:
                codec_ie[1U] |= A2DP_MPEG_1_2_SAMPLING_FREQUENCY_16000;
                break;

            case 22050:
                codec_ie[1U] |= A2DP_MPEG_1_2_SAMPLING_FREQUENCY_22050;
                break;

            case 24000:
                codec_ie[1U] |= A2DP_MPEG_1_2_SAMPLING_FREQUENCY_24000;
                break;

            case 32000:
                codec_ie[1U] |= A2DP_MPEG_1_2_SAMPLING_FREQUENCY_32000;
                break;

            case 44100:
                codec_ie[1U] |= A2DP_MPEG_1_2_SAMPLING_FREQUENCY_44100;
                break;

            case 48000:
                codec_ie[1U] |= A2DP_MPEG_1_2_SAMPLING_FREQUENCY_48000;
                break;

            default:
                LOG_DEBUG("Invalid Option: %d\n", option);
                break;
            }
        }
    }

    if (retval >= 0)
    {
        /* -------------------------- MPEG12 Variable Bit Rate */
        retval = BT_config_read (filename, "MPEG12_VBR", config_buf);
        if (retval < 0)
        {
            LOG_DEBUG("\t    Variable Bit Rate NOT found\n");
            /* return retval; */
        }
        else
        {
            LOG_DEBUG("\t    Variable Bit Rate -> %s\n", config_buf);

            if (0 == BT_str_cmp(config_buf, "ON"))
            {
                codec_ie[2U] |= A2DP_MPEG_1_2_VBR_ON;
            }
            else if (0 == BT_str_cmp(config_buf, "OFF"))
            {
                codec_ie[2U] |= A2DP_MPEG_1_2_VBR_OFF;
            }
        }
    }

    if (retval >= 0)
    {
        /* -------------------------- MPEG12 Bit Rate */
        retval = BT_config_read (filename, "MPEG12_BIT_RATE", config_buf);
        if (retval < 0)
        {
            LOG_DEBUG("\t    Bit Rate index NOT found\n");
            /* return retval; */
        }
        else
        {
            sscanf(config_buf, "%d", &option);
            LOG_DEBUG("\t    Bit Rate index -> %d\n", option);

            switch(option)
            {
            case 0:
                codec_ie[3U] |= (UCHAR )A2DP_MPEG_1_2_BITRATE_INDEX_0;
                codec_ie[2U] |= (UCHAR )(A2DP_MPEG_1_2_BITRATE_INDEX_0 >> 8U);
                break;
            case 1:
                codec_ie[3U] |= (UCHAR )A2DP_MPEG_1_2_BITRATE_INDEX_1;
                codec_ie[2U] |= (UCHAR )(A2DP_MPEG_1_2_BITRATE_INDEX_1 >> 8U);
                break;
            case 2:
                codec_ie[3U] |= (UCHAR )A2DP_MPEG_1_2_BITRATE_INDEX_2;
                codec_ie[2U] |= (UCHAR )(A2DP_MPEG_1_2_BITRATE_INDEX_2 >> 8U);
                break;
            case 3:
                codec_ie[3U] |= (UCHAR )A2DP_MPEG_1_2_BITRATE_INDEX_3;
                codec_ie[2U] |= (UCHAR )(A2DP_MPEG_1_2_BITRATE_INDEX_3 >> 8U);
                break;
            case 4:
                codec_ie[3U] |= (UCHAR )A2DP_MPEG_1_2_BITRATE_INDEX_4;
                codec_ie[2U] |= (UCHAR )(A2DP_MPEG_1_2_BITRATE_INDEX_4 >> 8U);
                break;
            case 5:
                codec_ie[3U] |= (UCHAR )A2DP_MPEG_1_2_BITRATE_INDEX_5;
                codec_ie[2U] |= (UCHAR )(A2DP_MPEG_1_2_BITRATE_INDEX_5 >> 8U);
                break;
            case 6:
                codec_ie[3U] |= (UCHAR )A2DP_MPEG_1_2_BITRATE_INDEX_6;
                codec_ie[2U] |= (UCHAR )(A2DP_MPEG_1_2_BITRATE_INDEX_6 >> 8U);
                break;
            case 7:
                codec_ie[3U] |= (UCHAR )A2DP_MPEG_1_2_BITRATE_INDEX_7;
                codec_ie[2U] |= (UCHAR )(A2DP_MPEG_1_2_BITRATE_INDEX_7 >> 8U);
                break;
            case 8:
                codec_ie[3U] |= (UCHAR )A2DP_MPEG_1_2_BITRATE_INDEX_8;
                codec_ie[2U] |= (UCHAR )(A2DP_MPEG_1_2_BITRATE_INDEX_8 >> 8U);
                break;
            case 9:
                codec_ie[3U] |= (UCHAR )A2DP_MPEG_1_2_BITRATE_INDEX_9;
                codec_ie[2U] |= (UCHAR )(A2DP_MPEG_1_2_BITRATE_INDEX_9 >> 8U);
                break;
            case 10:
                codec_ie[3U] |= (UCHAR )A2DP_MPEG_1_2_BITRATE_INDEX_10;
                codec_ie[2U] |= (UCHAR )(A2DP_MPEG_1_2_BITRATE_INDEX_10 >> 8U);
                break;
            case 11:
                codec_ie[3U] |= (UCHAR )A2DP_MPEG_1_2_BITRATE_INDEX_11;
                codec_ie[2U] |= (UCHAR )(A2DP_MPEG_1_2_BITRATE_INDEX_11 >> 8U);
                break;
            case 12:
                codec_ie[3U] |= (UCHAR )A2DP_MPEG_1_2_BITRATE_INDEX_12;
                codec_ie[2U] |= (UCHAR )(A2DP_MPEG_1_2_BITRATE_INDEX_12 >> 8U);
                break;
            case 13:
                codec_ie[3U] |= (UCHAR )A2DP_MPEG_1_2_BITRATE_INDEX_13;
                codec_ie[2U] |= (UCHAR )(A2DP_MPEG_1_2_BITRATE_INDEX_13 >> 8U);
                break;
            case 14:
                codec_ie[3U] |= (UCHAR )A2DP_MPEG_1_2_BITRATE_INDEX_13;
                codec_ie[2U] |= (UCHAR )(A2DP_MPEG_1_2_BITRATE_INDEX_13 >> 8U);
                break;

            default:
                LOG_DEBUG("Invalid Option: %d\n", option);
                break;
            }
        }
    }

    LOG_DEBUG("\n\t    Codec IE = 0x%02X 0x%02X 0x%02X 0x%02X\n",
    codec_ie[0U], codec_ie[1U], codec_ie[2U], codec_ie[3U]);

    return retval;
}
#endif /* A2DP_MPEG_1_2 */

#ifdef  A2DP_MPEG_2_4

INT32 appl_a2dp_read_mpeg24_configuration
      (
          CHAR *     filename,
          UCHAR *    codec_ie
      )
{
    int option;
    INT32 retval;
    CHAR config_buf [BT_CONFIG_MAX_CHARS_PER_LINE];


    /* Initialize Codec IE */
    codec_ie[0U] = codec_ie[1U] = codec_ie[2U] = codec_ie[3U] = 0x0U;
    codec_ie[1U] = 0x0U;
    codec_ie[2U] = 0x0U;
    codec_ie[3U] = 0x0U;
    codec_ie[4U] = 0x0U;
    codec_ie[5U] = 0x0U;

    retval = 0;

    LOG_DEBUG ("===== Read Configuration of MPEG-2,4 Codec =====\n");

    /* -------------------------- MPEG-2,4 Object Type */
    retval = BT_config_read (filename, "MPEG24_OBEJECT_TYPE", config_buf);
    if (retval < 0)
    {
        LOG_DEBUG("\t    Object Type NOT found\n");
        /* return retval; */
    }
    else
    {
        LOG_DEBUG("\t    Object Type -> %s\n", config_buf);

        if (0 == BT_str_cmp(config_buf, "MPEG_2_AAC_LC"))
        {
            codec_ie[0U] |= A2DP_MPEG_2_4_OBJ_TYPE_MPEG2_AAC_LC;
        }
        else if (0 == BT_str_cmp(config_buf, "MPEG_4_AAC_LC"))
        {
            codec_ie[0U] |= A2DP_MPEG_2_4_OBJ_TYPE_MPEG4_AAC_LC;

        }
        else if (0 == BT_str_cmp(config_buf, "MPEG_4_AAC_LTP"))
        {
            codec_ie[0U] |= A2DP_MPEG_2_4_OBJ_TYPE_MPEG4_AAC_LTP;
        }
        else if (0 == BT_str_cmp(config_buf, "MPEG_4_AAC_SCALABLE"))
        {
            codec_ie[0U] |= A2DP_MPEG_2_4_OBJ_TYPE_MPEG4_AAC_SCALABLE;
        }
    }

    if (retval >= 0)
    {
        /* -------------------------- MPEG24 Sampling Frequeny */
        retval = BT_config_read (filename, "MPEG24_SAMPLING_FREQUENCY", config_buf);
        if (retval < 0)
        {
            LOG_DEBUG("\t    Sampling Frequeny NOT found\n");
            /* return retval; */
        }
        else
        {
            sscanf(config_buf, "%d", &option);
            LOG_DEBUG("\t    Sampling Frequeny -> %d\n", option);

            switch(option)
            {
            case 8000:
                codec_ie[2U] |= (UCHAR )A2DP_MPEG_2_4_SAMPLING_FREQUENCY_8000;
                codec_ie[1U] |= (UCHAR )(A2DP_MPEG_2_4_SAMPLING_FREQUENCY_8000 >> 8U);
                break;

            case 11025:
                codec_ie[2U] |= (UCHAR )A2DP_MPEG_2_4_SAMPLING_FREQUENCY_11025;
                codec_ie[1U] |= (UCHAR )(A2DP_MPEG_2_4_SAMPLING_FREQUENCY_11025 >> 8U);
                break;

            case 12000:
                codec_ie[2U] |= (UCHAR )A2DP_MPEG_2_4_SAMPLING_FREQUENCY_12000;
                codec_ie[1U] |= (UCHAR )(A2DP_MPEG_2_4_SAMPLING_FREQUENCY_12000 >> 8U);
                break;

            case 16000:
                codec_ie[2U] |= (UCHAR )A2DP_MPEG_2_4_SAMPLING_FREQUENCY_16000;
                codec_ie[1U] |= (UCHAR )(A2DP_MPEG_2_4_SAMPLING_FREQUENCY_16000 >> 8U);
                break;

            case 22050:
                codec_ie[2U] |= (UCHAR )A2DP_MPEG_2_4_SAMPLING_FREQUENCY_22050;
                codec_ie[1U] |= (UCHAR )(A2DP_MPEG_2_4_SAMPLING_FREQUENCY_22050 >> 8U);
                break;

            case 24000:
                codec_ie[2U] |= (UCHAR )A2DP_MPEG_2_4_SAMPLING_FREQUENCY_24000;
                codec_ie[1U] |= (UCHAR )(A2DP_MPEG_2_4_SAMPLING_FREQUENCY_24000 >> 8U);
                break;

            case 32000:
                codec_ie[2U] |= (UCHAR )A2DP_MPEG_2_4_SAMPLING_FREQUENCY_32000;
                codec_ie[1U] |= (UCHAR )(A2DP_MPEG_2_4_SAMPLING_FREQUENCY_32000 >> 8U);
                break;

            case 44100:
                codec_ie[2U] |= (UCHAR )A2DP_MPEG_2_4_SAMPLING_FREQUENCY_44100;
                codec_ie[1U] |= (UCHAR )(A2DP_MPEG_2_4_SAMPLING_FREQUENCY_44100 >> 8U);
                break;

            case 48000:
                codec_ie[2U] |= (UCHAR )A2DP_MPEG_2_4_SAMPLING_FREQUENCY_48000;
                codec_ie[1U] |= (UCHAR )(A2DP_MPEG_2_4_SAMPLING_FREQUENCY_48000 >> 8U);
                break;

            case 64000:
                codec_ie[2U] |= (UCHAR )A2DP_MPEG_2_4_SAMPLING_FREQUENCY_64000;
                codec_ie[1U] |= (UCHAR )(A2DP_MPEG_2_4_SAMPLING_FREQUENCY_64000 >> 8U);
                break;

            case 88200:
                codec_ie[2U] |= (UCHAR )A2DP_MPEG_2_4_SAMPLING_FREQUENCY_88200;
                codec_ie[1U] |= (UCHAR )(A2DP_MPEG_2_4_SAMPLING_FREQUENCY_88200 >> 8U);
                break;

            case 96000:
                codec_ie[2U] |= (UCHAR )A2DP_MPEG_2_4_SAMPLING_FREQUENCY_96000;
                codec_ie[1U] |= (UCHAR )(A2DP_MPEG_2_4_SAMPLING_FREQUENCY_96000 >> 8U);
                break;

            default:
                LOG_DEBUG("Invalid Option: %d\n", option);
                break;
            }
        }
    }

    if (retval >= 0)
    {
        /* -------------------------- MPEG24 Channels */
        retval = BT_config_read (filename, "MPEG24_CHANNELS", config_buf);
        if (retval < 0)
        {
            LOG_DEBUG("\t    Channels NOT found\n");
            /* return retval; */
        }
        else
        {
            sscanf(config_buf, "%d", &option);
            LOG_DEBUG("\t    Channels -> %d\n", option);

            if (1 == option)
            {
                codec_ie[2U] |= A2DP_MPEG_2_4_CHANNELS_1;
            }
            else if (2 == option)
            {
                codec_ie[2U] |= A2DP_MPEG_2_4_CHANNELS_2;
            }
        }
    }

    if (retval >= 0)
    {
        /* -------------------------- MPEG24 Variable Bit Rate */
        retval = BT_config_read (filename, "MPEG24_VBR", config_buf);
        if (retval < 0)
        {
            LOG_DEBUG("\t    Variable Bit Rate NOT found\n");
            /* return retval; */
        }
        else
        {
            LOG_DEBUG("\t    Variable Bit Rate -> %s\n", config_buf);

            if (0 == BT_str_cmp(config_buf, "ON"))
            {
                codec_ie[3U] |= A2DP_MPEG_2_4_VBR_ON;
            }
            else if (0 == BT_str_cmp(config_buf, "OFF"))
            {
                codec_ie[3U] |= A2DP_MPEG_2_4_VBR_OFF;
            }
        }
    }

    if (retval >= 0)
    {
        /* -------------------------- MPEG24 Bit Rate */
        retval = BT_config_read (filename, "MPEG24_BIT_RATE", config_buf);
        if (retval < 0)
        {
            LOG_DEBUG("\t    Bit Rate NOT found\n");
            /* return retval; */
        }
        else
        {
            sscanf(config_buf, "%d", &option);
            LOG_DEBUG("\t    Bit Rate -> %d\n", option);

            if (0 != option)
            {
                codec_ie[5U]  = (UCHAR )option;
                codec_ie[4U]  = (UCHAR )(option >> 8U);
                codec_ie[3U] |= (UCHAR )(option >> 16U);
            }
        }
    }

    LOG_DEBUG("\n\t    Codec IE = 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n",
    codec_ie[0U], codec_ie[1U], codec_ie[2U], codec_ie[3U], codec_ie[4U], codec_ie[5U]);

    return retval;
}
#endif /* A2DP_MPEG_2_4 */



#ifdef A2DP_SOURCE
void appl_a2dp_src_prepare (void)
{
#ifdef INCLUDE_SBC
    API_RESULT retval;
    UINT16 samples_per_sec;
    UCHAR number_of_channels;
    UCHAR * codec_ie;
    SBC_CODEC_PARAM * codec_param;

    /* Init */
    retval = API_SUCCESS;
    samples_per_sec = 0U;
    number_of_channels = 0U;

    /* Get SEP Configuration Codec IE */
    codec_ie = appl_a2dp_sbc_codec_ie;

    /* Get SBC Encoder Codec Parameters */
    codec_param = &a2dp_sbc_encoder_io.encoder_params;

    /* Decode Sampling Frequency */
    switch (codec_ie[0U] & 0xF0U)
    {
    case A2DP_SBC_SAMPLING_FREQUENCY_16000:
        samples_per_sec = 16000U;
        codec_param->sampling_frequency = 0U;

        break;

    case A2DP_SBC_SAMPLING_FREQUENCY_32000:
        samples_per_sec = 32000U;
        codec_param->sampling_frequency = 1U;

        break;

    case A2DP_SBC_SAMPLING_FREQUENCY_44100:
        samples_per_sec = 44100U;
        codec_param->sampling_frequency = 2U;

        break;

    case A2DP_SBC_SAMPLING_FREQUENCY_48000:
        samples_per_sec = 48000U;
        codec_param->sampling_frequency = 3U;

        break;

    default:
        LOG_DEBUG("*** SBC Config: Sampling Frequency = Unknown (0x%02X)!!\n",
        (codec_ie[0U] & 0xF0U));

        retval = API_FAILURE; /* return; */
    }

    if (API_SUCCESS == retval)
    {
        /* Decode Channel Mode */
        switch (codec_ie[0U] & 0x0FU)
        {
        case A2DP_SBC_CHANNEL_MONO:
            codec_param->channel_mode = SBC_CHANNEL_MONO;
            codec_param->nrof_channels = 1U;

            break;

        case A2DP_SBC_CHANNEL_DUAL:
            codec_param->channel_mode = SBC_CHANNEL_DUAL;
            codec_param->nrof_channels = 2U;

            break;

        case A2DP_SBC_CHANNEL_STEREO:
            codec_param->channel_mode = SBC_CHANNEL_STEREO;
            codec_param->nrof_channels = 2U;

            break;

        case A2DP_SBC_CHANNEL_JOINT_STEREO:
            codec_param->channel_mode = SBC_CHANNEL_JOINT_STEREO;
            codec_param->nrof_channels = 2U;

            break;

        default:
            LOG_DEBUG("*** SBC Config: Channel Mode = Unknown (0x%02X)!!\n",
            (codec_ie[0U] & 0x0FU));

            retval = API_FAILURE; /* return; */
        }
    }

    if (API_SUCCESS == retval)
    {
        /* Set Number of Channels */
        number_of_channels = codec_param->nrof_channels;

        /* Decode Block Length */
        switch (codec_ie[1U] & 0xF0U)
        {
        case A2DP_SBC_BLOCK_LENGTH_4:
            codec_param->nrof_blocks = SBC_BLOCK_SIZE_4;

            break;

        case A2DP_SBC_BLOCK_LENGTH_8:
            codec_param->nrof_blocks = SBC_BLOCK_SIZE_8;

            break;

        case A2DP_SBC_BLOCK_LENGTH_12:
            codec_param->nrof_blocks = SBC_BLOCK_SIZE_12;

            break;

        case A2DP_SBC_BLOCK_LENGTH_16:
            codec_param->nrof_blocks = SBC_BLOCK_SIZE_16;

            break;

        default:
            LOG_DEBUG("*** SBC Config: Block Length = Unknown (0x%02X)!!\n",
            (codec_ie[1U] & 0xF0U));

            retval = API_FAILURE; /* return; */
        }
    }

    if (API_SUCCESS == retval)
    {
        /* Decode Subbands */
        switch (codec_ie[1U] & 0x0CU)
        {
        case A2DP_SBC_SUBBANDS_4:
            codec_param->nrof_subbands = SBC_SUBBAND_4;

            break;

        case A2DP_SBC_SUBBANDS_8:
            codec_param->nrof_subbands = SBC_SUBBAND_8;

            break;

        default:
            LOG_DEBUG("*** SBC Config: Subbands = Unknown (0x%02X)!!\n",
            (codec_ie[1U] & 0x0CU));

            retval = API_FAILURE; /* return; */
        }
    }

    if (API_SUCCESS == retval)
    {
        /* Decode Allocation Method */
        switch (codec_ie[1U] & 0x03U)
        {
        case A2DP_SBC_ALLOCATION_METHOD_SNR:
            codec_param->allocation_method = SBC_SNR;

            break;

        case A2DP_SBC_ALLOCATION_METHOD_LOUDNESS:
            codec_param->allocation_method = SBC_LOUDNESS;

            break;

        default:
            LOG_DEBUG("*** SBC Config: Allocation Method = Unknown (0x%02X)!!\n",
            (codec_ie[1U] & 0x03U));

            retval = API_FAILURE; /* return; */
        }
    }

    if (API_SUCCESS == retval)
    {
        /* Set Bitpool */
        codec_param->bitpool = codec_ie[3U];

        /* Initialize SBC Decoder */
        retval = sbc_encoder_init (&a2dp_sbc_encoder_io);
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("*** Failed to Initialize SBC Encoder: 0x%04X\n",
            retval);

            /* Set Encoder not initialized */
            appl_a2dp_sbc_encoder_state = BT_FALSE;

            retval = API_FAILURE; /* return; */
        }
    }

    if (API_SUCCESS == retval)
    {
        /* Set Encoder initialized */
        appl_a2dp_sbc_encoder_state = BT_TRUE;

        /* Calculate Number of Bytes to be encoded per SBC frame */
        appl_a2dp_pcm_datalen = 2U *
                               (UINT16)codec_param->nrof_subbands *
                               codec_param->nrof_blocks *
                               codec_param->nrof_channels;

        /* Init the Pl */
        a2dp_init_pl(A2DP_EP_SOURCE);

        /* Setup the Pl */
        a2dp_setup_pl
        (
            A2DP_EP_SOURCE,
            a2dp_pl_src_callback,
            samples_per_sec,
            16U,
            number_of_channels,
            appl_a2dp_pcm_datalen
        );
    }

#endif /* INCLUDE_SBC */

    return;
}

static void a2dp_pl_src_callback
       (
           const UCHAR *    r_data,
           UINT16           r_datalen
       )
{
    /* If encoder not initialized, drop the data */
    if (BT_TRUE != appl_a2dp_sbc_encoder_state)
    {
        LOG_DEBUG("* ");
        /* return; */
    }
    else
    {
        /* LOG_DEBUG("<%d> ", r_datalen); */
        (BT_IGNORE_RETURN_VALUE)app_a2dp_src_enqueue(r_data, r_datalen);
    }

    return;
}

API_RESULT app_a2dp_src_enqueue
           (
               /* IN */  const UCHAR   * data,
               /* IN */  UINT16          datalen
           )
{
    INT32  n_free;
    UINT32 count;
    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;

    /*
     *  Get amount of free buffer space.
     */
    if (a2dp_src_buffer_wr_ptr >= a2dp_src_buffer_rd_ptr)
    {
        /*
         *    ----------DDDDDDDDDDDDDDD--------------X
         *    ^         ^              ^             ^
         *    |         |              |             |
         *    0       rd_ptr         wr_ptr         max
         *
         *  Free Space => '-----------'
         */
        n_free = (a2dp_src_buffer_size - a2dp_src_buffer_wr_ptr)
                + a2dp_src_buffer_rd_ptr;
    }
    else
    {
        /*
         *    DDDDDDDDDD---------------DDDDDDDDDDDDDDX
         *    ^         ^              ^             ^
         *    |         |              |             |
         *    0       wr_ptr         rd_ptr         max
         *
         *  Free Space => '-----------'
         */
        n_free = a2dp_src_buffer_rd_ptr - a2dp_src_buffer_wr_ptr;
    }

    /*
     *  Do we have enough space to accomodate new data ?
     *  Buffer Empty Condition: when rd_ptr == wr_ptr
     *  Buffer Full  Condition: when diff(rd_ptr, wr_ptr) == 1
     */
    if ((n_free < 1U) || (n_free < datalen))
    {
        /* Overflow ! */
        /* APPL_ERR ("+ %d %d + ", n_free, datalen); */
        retval = API_FAILURE; /* return API_FAILURE; */
    }

    if (API_SUCCESS == retval)
    {
        /* Store new data into Buffer */
        for (count = 0U; count < datalen; count++)
        {
            a2dp_src_buffer[a2dp_src_buffer_wr_ptr] = data[count];
            a2dp_src_buffer_wr_ptr++;

            if(a2dp_src_buffer_wr_ptr == a2dp_src_buffer_size)
            {
                a2dp_src_buffer_wr_ptr = 0;
            }
        }

        if (APP_A2DP_SRC_WR_TH_INIT == a2dp_src_wr_th_state)
        {
            /* Signal the waiting thread */
            (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_lock (&a2dp_src_th_mutex);
            a2dp_src_wr_th_state = APP_A2DP_SRC_WR_TH_PLAYING;
            (BT_IGNORE_RETURN_VALUE) BT_thread_cond_signal (&a2dp_src_th_cond);
            (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_unlock (&a2dp_src_th_mutex);
        }
    }

    return retval;
}

DECL_STATIC BT_THREAD_RETURN_TYPE a2dp_src_write_task (BT_THREAD_ARGS args)
{
    INT32  rd_ptr, index, remaining;
    UINT16 bytes_to_send, buf_index, encode_len;

    BT_IGNORE_UNUSED_PARAM(args);

    BT_LOOP_FOREVER()
    {
        if (APP_A2DP_SRC_WR_TH_INIT == a2dp_src_wr_th_state)
        {
            /* Wait for data in buffer */
            (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_lock (&a2dp_src_th_mutex);
            (BT_IGNORE_RETURN_VALUE) BT_thread_cond_wait (&a2dp_src_th_cond, &a2dp_src_th_mutex);
            (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_unlock (&a2dp_src_th_mutex);
        }
        else if (APP_A2DP_SRC_WR_TH_STOP == a2dp_src_wr_th_state)
        {
            /* Exit from task */
            break; /* return BT_THREAD_RETURN_VAL_SUCCESS; */
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if (a2dp_src_buffer_wr_ptr >= a2dp_src_buffer_rd_ptr)
        {
            /*
             *    ----------DDDDDDDDDDDDDDD--------------X
             *    ^         ^              ^             ^
             *    |         |              |             |
             *    0       rd_ptr         wr_ptr         max
             *
             *  Free Space => '-----------'
             */
            remaining = a2dp_src_buffer_wr_ptr - a2dp_src_buffer_rd_ptr;
        }
        else
        {
            /*
             *    DDDDDDDDDD---------------DDDDDDDDDDDDDDX
             *    ^         ^              ^             ^
             *    |         |              |             |
             *    0       wr_ptr         rd_ptr         max
             *
             *  Free Space => '-----------'
             */
            remaining = a2dp_src_buffer_size -
                    (a2dp_src_buffer_rd_ptr - a2dp_src_buffer_wr_ptr);
        }

        /* Do we really have anything to read ? */
        if (remaining < appl_a2dp_pcm_datalen)
        {
            /* Wait for data in buffer */
            (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_lock (&a2dp_src_th_mutex);
            a2dp_src_wr_th_state = APP_A2DP_SRC_WR_TH_INIT;
            (BT_IGNORE_RETURN_VALUE) BT_thread_cond_wait (&a2dp_src_th_cond, &a2dp_src_th_mutex);
            (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_unlock (&a2dp_src_th_mutex);

            /* APPL_INF (bt_debug_fd, "remaining < 1\n"); */
            continue;
        }

        rd_ptr = a2dp_src_buffer_rd_ptr;

        bytes_to_send = (remaining > appl_a2dp_pcm_datalen) ?
            appl_a2dp_pcm_datalen : (UINT16)remaining;

        for (index = 0; index < bytes_to_send; index++)
        {
            pcm_to_send[index] = a2dp_src_buffer[rd_ptr];

            rd_ptr = rd_ptr + 1;
            if (rd_ptr == a2dp_src_buffer_size)
            {
                rd_ptr = 0;
            }
        }

        /* Update the read pointer */
        a2dp_src_buffer_rd_ptr = rd_ptr;

        buf_index = 0U;

        while (buf_index < bytes_to_send)
        {
            if ((bytes_to_send - buf_index) <= appl_a2dp_pcm_datalen)
            {
                encode_len = bytes_to_send - buf_index;
            }
            else
            {
                encode_len = appl_a2dp_pcm_datalen;
            }

            (BT_IGNORE_RETURN_VALUE) a2dp_encode_n_send
            (
                appl_a2dp_instance[0U].a2dp_instance,
                &pcm_to_send[buf_index],
                encode_len
            );

            buf_index += encode_len;
        }
    }

    return BT_THREAD_RETURN_VAL_SUCCESS;
}

API_RESULT a2dp_encode_n_send
           (
               UINT8    a2dp_instance,
               UCHAR  * pcm_data,
               UINT16   pcm_datalen
           )
{
    API_RESULT        retval;

#ifdef INCLUDE_SBC
    UINT16            count;
    UINT16            sbc_datalen;
    UCHAR             i;

    BT_IGNORE_UNUSED_PARAM(a2dp_instance);

    /* Init */
    retval = API_SUCCESS;

    /* Convert UCHAR LE16 PCM Data to UINT16 LE16 */
    for (count = 0U; count < (pcm_datalen >> 1U); count ++)
    {
        appl_a2dp_pcm_data_le[count] =
        (UINT16) ((pcm_data[2U * count + 1U] << 8U) | (pcm_data[2U * count]));
    }

    /* Initialize amount of buffer passed for SBC stream */
    sbc_datalen = appl_a2dp_pcm_datalen;

    /* Prepare for SBC Encoder */
    a2dp_sbc_encoder_io.encoder_input_buflen = count;
    a2dp_sbc_encoder_io.encoder_input_buffer = appl_a2dp_pcm_data_le;

    a2dp_sbc_encoder_io.encoder_output_buffer = appl_a2dp_sbc_data;

#ifdef A2DP_SUPPORT_MULTIPLE_MEDIA_FRAME_WRITE
    a2dp_sbc_encoder_io.encoder_output_buffer += appl_a2dp_src_sbc_data_len;
#endif /* A2DP_SUPPORT_MULTIPLE_MEDIA_FRAME_WRITE */

    a2dp_sbc_encoder_io.encoder_output_buflen = sbc_datalen;

    /* Do SBC Encoding */
    retval = sbc_encoder
             (
                 &a2dp_sbc_encoder_io
             );

    if (API_SUCCESS != retval)
    {
        LOG_DEBUG ("SBC Encoder failed - 0x%04X\n", retval);
        /* return retval; */
    }
    else
    {
        /* Update SBC Frame Size */
        sbc_datalen = a2dp_sbc_encoder_io.encoder_output_buflen;

        /* LOG_DEBUG ("~ "); */

#ifndef A2DP_SUPPORT_MULTIPLE_MEDIA_FRAME_WRITE
        for (i = 0U; i < A2DP_MAX_CODECS; i++)
        {
            if (APPL_A2DP_STREAM_TRUE == appl_a2dp_instance[i].stream_state)
            {
                /* Write to A2DP */
                retval = BT_a2dp_media_write
                         (
                             appl_a2dp_instance[i].a2dp_instance,
                             appl_a2dp_sbc_data,
                             sbc_datalen
                         );
                if (API_SUCCESS != retval)
                {
                    LOG_DEBUG ("A2DP media write failed - 0x%04X\n", retval);
                }
            }
        }
#else
        /* Update sbc encoded data len & frame count */
        appl_a2dp_src_sbc_data_len += sbc_datalen;
        appl_a2dp_src_sbc_frame_cnt += 1U;

        if ((appl_a2dp_src_max_mpl - appl_a2dp_src_sbc_data_len) >= sbc_datalen)
        {
            /**
             * Encode one more frame.
             */
            retval = API_SUCCESS; /* return API_SUCCESS; */
        }
        else
        {
            for (i = 0U; i < A2DP_MAX_CODECS; i++)
            {
                if (APPL_A2DP_STREAM_TRUE == appl_a2dp_instance[i].stream_state)
                {
                    /* Write to A2DP */
                    retval = BT_a2dp_media_write
                             (
                                 appl_a2dp_instance[i].a2dp_instance,
                                 appl_a2dp_src_sbc_frame_cnt,
                                 appl_a2dp_sbc_data,
                                 appl_a2dp_src_sbc_data_len
                             );
                    if (API_SUCCESS != retval)
                    {
                        LOG_DEBUG ("A2DP media write failed - 0x%04X\n", retval);
                    }
                }
            }

            /* Reset */
            appl_a2dp_src_sbc_frame_cnt = 0x00U;
            appl_a2dp_src_sbc_data_len  = 0x00U;
        }
#endif /* A2DP_SUPPORT_MULTIPLE_MEDIA_FRAME_WRITE */
    }
#else /* INCLUDE_SBC */
    /* Init */
    retval = API_SUCCESS;

    BT_IGNORE_UNUSED_PARAM(a2dp_instance);
    BT_IGNORE_UNUSED_PARAM(pcm_data);
    BT_IGNORE_UNUSED_PARAM(pcm_datalen);
#endif /* INCLUDE_SBC */
    return retval;
}

API_RESULT appl_a2dp_media_write(void)
{
    API_RESULT    retval;
    UCHAR i, j;
    UCHAR sbc_frame_hdr[3U];
    UINT16 sbc_frame_len;

    retval = API_SUCCESS;
    sbc_frame_len = 0x00U;
    sbc_frame_hdr[0U] = 0x00U;
    sbc_frame_hdr[1U] = 0x00U;
    sbc_frame_hdr[2U] = 0x00U;


    /**
     * If A2DP source is streaming same encoded audio to multiple A2DP sink
     * devices, then make sure that the codec configuration is same for all
     * the connected devices.
     */
    for (i = 0U; i < A2DP_MAX_CODECS; i++)
    {
        /* Send A2DP data only if stream state is set to TRUE */
        if (APPL_A2DP_STREAM_TRUE == appl_a2dp_instance[i].stream_state)
        {
            /*
             * Get SBC frame hdr. First 3 bytes of SBC frame will have required
             * configuration parameters
             */
            retval = appl_a2dp_get_sbc_frame_details
                     (
                         &appl_a2dp_instance[i].codec_ie_conf[0U],
                         &sbc_frame_hdr[0U],
                         &sbc_frame_len
                     );

            appl_sbc_data[0U] = sbc_frame_hdr[0U];
            appl_sbc_data[1U] = sbc_frame_hdr[1U];
            appl_sbc_data[2U] = sbc_frame_hdr[2U];


            LOG_DEBUG ("\n==========\n");
            LOG_DEBUG ("0x%02X, 0x%02X, 0x%02X", appl_sbc_data[0U], appl_sbc_data[1U], appl_sbc_data[2U]);
            LOG_DEBUG ("\n===========\n");

            for (j = 0U; j < 10U; j++)
            {

#ifndef A2DP_SUPPORT_MULTIPLE_MEDIA_FRAME_WRITE
                /* Write to A2DP */
                retval = BT_a2dp_media_write
                         (
                            appl_a2dp_instance[i].a2dp_instance,
                            appl_sbc_data,
                            sbc_frame_len
                         );
#else
                /* Write to A2DP */
                retval = BT_a2dp_media_write
                         (
                            appl_a2dp_instance[i].a2dp_instance,
                            0x01U,
                            appl_sbc_data,
                            sbc_frame_len
                         );
#endif /* A2DP_SUPPORT_MULTIPLE_MEDIA_FRAME_WRITE */

                if (API_SUCCESS != retval)
                {
                    LOG_DEBUG ("A2DP media write failed - 0x%04X\n", retval);
                }
            }
        }
    }

    return retval;
}
#endif /* A2DP_SOURCE */

INT32 appl_a2dp_read_codec_ie_configuration
      (
          /* IN */  CHAR  * filename,
          /* IN */  UCHAR   codec_type,
          /* OUT */ UCHAR * codec_ie
      )
{
    INT32 ret;

    ret = -1;

    if ((NULL == filename) || (NULL == codec_ie))
    {
        LOG_DEBUG ("NULL parameter in Read Codec Configuration\n");
        /* return ret; */
    }
    else
    {
        if (AVDTP_CODEC_AUDIO_SBC == codec_type)
        {
            /* Set SEP Configuration Codec IE for SBC */
            ret = appl_a2dp_read_sbc_configuration
                  (
                      filename,
                      codec_ie
                  );
        }
#ifdef A2DP_MPEG_1_2
        else if (AVDTP_CODEC_AUDIO_MPEG_1_2 == codec_type)
        {
            /* Set SEP Configuration Codec IE for MPEG-1,2 */
            ret = appl_a2dp_read_mpeg12_configuration
                  (
                      filename,
                      codec_ie
                  );
        }
#endif /* A2DP_MPEG_1_2 */
#ifdef A2DP_MPEG_2_4
        else if (AVDTP_CODEC_AUDIO_MPEG_2_4 == codec_type)
        {
               /* Set SEP Configuration Codec IE for MPEG-2,4 */
            ret = appl_a2dp_read_mpeg24_configuration
                  (
                      filename,
                      codec_ie
                  );
        }
#endif /* A2DP_MPEG_2_4 */
#ifdef A2DP_VENDOR_CODEC
        else if (AVDTP_CODEC_AUDIO_VENDOR_SPECIFIC == codec_type)
        {
             /* Just return Success */
            ret = 0; /*  return 0U; */
        }
#endif /* A2DP_VENDOR_CODEC */
    }

    return ret;
}

void appl_a2dp_save_codec_ie_configuration
      (
          /* IN */  UCHAR   role,
          /* IN */  UCHAR   codec_type,
          /* IN */  UCHAR * codec_ie,
          /* IN */  UINT16   codec_ie_len
      )
{
    UCHAR * codec_ie_storage;
    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;
    codec_ie_storage = NULL;

    if ((NULL == codec_ie) ||
        (0x00U == codec_ie_len))
    {
        LOG_DEBUG ("Invalid parameters to save Codec IE Info.\n");
        retval = API_FAILURE;  /* return; */
    }

    if(API_SUCCESS == retval)
    {
        if (AVDTP_CODEC_AUDIO_SBC == codec_type)
        {
            /* Save SBC Codec IE */
            codec_ie_storage = appl_a2dp_sbc_codec_ie;
        }
#ifdef A2DP_MPEG_1_2
        else if (AVDTP_CODEC_AUDIO_MPEG_1_2 == codec_type)
        {
            /* Save MPEG-1,2 Codec IE */
            codec_ie_storage = appl_a2dp_mpeg12_codec_ie;
        }
#endif /* A2DP_MPEG_1_2 */
#ifdef A2DP_MPEG_2_4
        else if (AVDTP_CODEC_AUDIO_MPEG_2_4 == codec_type)
        {
            /* Save MPEG-2,4 Codec IE */
            codec_ie_storage = appl_a2dp_mpeg24_codec_ie;
        }
#endif /* A2DP_MPEG_2_4 */
#ifdef A2DP_VENDOR_CODEC
        else if (AVDTP_CODEC_AUDIO_VENDOR_SPECIFIC == codec_type)
        {
            /* Save Vendor Specific Codec IE */
            codec_ie_storage = appl_a2dp_vendor_codec_ie;
        }
#endif /* A2DP_VENDOR_CODEC */
        else
        {
            /* No matching codec found */
            retval = API_FAILURE;  /* return; */
        }
    }

    if (API_SUCCESS == retval)
    {
        /* Save Codec IE */
        BT_mem_copy
        (
            codec_ie_storage,
            codec_ie,
            codec_ie_len
        );

        /* Check if codec is SBC */
        if (AVDTP_CODEC_AUDIO_SBC == codec_type)
        {
            if (AVDTP_SEP_SINK == role)
            {
#ifdef A2DP_SINK
                appl_a2dp_jpl_configure(codec_ie);
#else
                LOG_DEBUG ("A2DP_SINK Flag is not defined\n");
#endif /* A2DP_SINK */
            }
#ifdef A2DP_SOURCE
            else
            {
                appl_a2dp_src_prepare();
            }
#endif /* A2DP_SOURCE */
        }
    }

    return;
}

void appl_a2dp_print_codec_ie_configuration
      (
          /* IN */  UCHAR   codec_type,
          /* IN */  UCHAR * codec_ie,
          /* IN */  UINT16  codec_ie_len
      )
{
    if (NULL == codec_ie)
    {
        LOG_DEBUG ("NULL Codec IE\n");

        /* return; */
    }
    else
    {
        if (AVDTP_CODEC_AUDIO_SBC == codec_type)
        {
            LOG_DEBUG("Code Type: SBC\n");

            if (A2DP_SBC_CODEC_CAP_IE_LEN == codec_ie_len)
            {
                LOG_DEBUG("\tSBC Codec IE = 0x%02X 0x%02X 0x%02X 0x%02X\n",
                codec_ie[0U], codec_ie[1U], codec_ie[2U], codec_ie[3U]);

                /* Decode Sampling Frequency */
                switch (codec_ie[0U] & 0xF0U)
                {
                case A2DP_SBC_SAMPLING_FREQUENCY_16000:
                    LOG_DEBUG("\t-> Sampling Frequency = 16000\n");
                    break;

                case A2DP_SBC_SAMPLING_FREQUENCY_32000:
                    LOG_DEBUG("\t-> Sampling Frequency = 32000\n");
                    break;

                case A2DP_SBC_SAMPLING_FREQUENCY_44100:
                    LOG_DEBUG("\t-> Sampling Frequency = 44100\n");
                    break;

                case A2DP_SBC_SAMPLING_FREQUENCY_48000:
                    LOG_DEBUG("\t-> Sampling Frequency = 48000\n");
                    break;

                default:
                    LOG_DEBUG("\t-> Sampling Frequency = ?????\n");
                    break;
                }

                /* Decode Channel Mode */
                switch (codec_ie[0U] & 0x0FU)
                {
                case A2DP_SBC_CHANNEL_MONO:
                    LOG_DEBUG("\t-> Channel Mode = Mono (1)\n");
                    break;

                case A2DP_SBC_CHANNEL_DUAL:
                    LOG_DEBUG("\t-> Channel Mode = Dual (2)\n");
                    break;

                case A2DP_SBC_CHANNEL_STEREO:
                    LOG_DEBUG("\t-> Channel Mode = Stereo (2)\n");
                    break;

                case A2DP_SBC_CHANNEL_JOINT_STEREO:
                    LOG_DEBUG("\t-> Channel Mode = Joint Stereo (2)\n");
                    break;

                default:
                    LOG_DEBUG("\t-> Channel Mode = ?????\n");
                    break;
                }

                /* Decode Block Length */
                switch (codec_ie[1U] & 0xF0U)
                {
                case A2DP_SBC_BLOCK_LENGTH_4:
                    LOG_DEBUG("\t-> Block Length = 4\n");
                    break;

                case A2DP_SBC_BLOCK_LENGTH_8:
                    LOG_DEBUG("\t-> Block Length = 8\n");
                    break;

                case A2DP_SBC_BLOCK_LENGTH_12:
                    LOG_DEBUG("\t-> Block Length = 12\n");
                    break;

                case A2DP_SBC_BLOCK_LENGTH_16:
                    LOG_DEBUG("\t-> Block Length = 16\n");
                    break;

                default:
                    LOG_DEBUG("\t-> Block Length = ?????\n");
                    break;
                }

                /* Decode Subbands */
                switch (codec_ie[1U] & 0x0CU)
                {
                case A2DP_SBC_SUBBANDS_4:
                    LOG_DEBUG("\t-> Subbands = 4\n");
                    break;

                case A2DP_SBC_SUBBANDS_8:
                    LOG_DEBUG("\t-> Subbands = 8\n");
                    break;

                default:
                    LOG_DEBUG("\t-> Subbands = ?????\n");
                    break;
                }

                /* Decode Allocation Method */
                switch (codec_ie[1U] & 0x03U)
                {
                case A2DP_SBC_ALLOCATION_METHOD_SNR:
                    LOG_DEBUG("\t-> Allocation Method = SNR\n");
                    break;

                case A2DP_SBC_ALLOCATION_METHOD_LOUDNESS:
                    LOG_DEBUG("\t-> Allocation Method = Loudness\n");
                    break;

                default:
                    LOG_DEBUG("\t-> Allocation Method = ?????\n");
                    break;
                }

                LOG_DEBUG("\t-> Bitpool = %d, %d\n", codec_ie[2U], codec_ie[3U]);
            }
            else
            {
                LOG_DEBUG("*** A2DP_CONFIGURE_IND: No Event Data/Data Length\n");
            }
        }
#ifdef A2DP_MPEG_1_2
        else if (AVDTP_CODEC_AUDIO_MPEG_1_2 == codec_type)
        {
            UINT16 bit_rate_index;

            LOG_DEBUG("Code Type: MPEG-1,2\n");

            if (A2DP_MPEG_1_2_CODEC_CAP_IE_LEN == codec_ie_len)
            {
                LOG_DEBUG("\tMPEG-1,2 Codec IE = 0x%02X 0x%02X 0x%02X 0x%02X\n",
                codec_ie[0U], codec_ie[1U], codec_ie[2U], codec_ie[3U]);

                /* Decode MPEG Layer */
                LOG_DEBUG("\t    -> Layer = ");
                switch(codec_ie[0U]& 0xE0U)
                {
                case A2DP_MPEG_1_2_LAYER_I:
                    LOG_DEBUG("I");
                    break;

                case A2DP_MPEG_1_2_LAYER_II:
                    LOG_DEBUG("II");
                    break;

                case A2DP_MPEG_1_2_LAYER_III:
                    LOG_DEBUG("III");
                    break;

                default:
                    LOG_DEBUG("?????");
                    break;
                }
                LOG_DEBUG("\n");

                /* Decode CRC */
                LOG_DEBUG("\t    -> CRC = ");
                if (codec_ie[0U]& 0x10U)
                {
                    LOG_DEBUG("ON");
                }
                else
                {
                    LOG_DEBUG("OFF");
                }
                LOG_DEBUG("\n");

                /* Decode Channel Mode */
                LOG_DEBUG("\t    -> Channel Mode = ");
                switch(codec_ie[0U]& 0x0FU)
                {
                case A2DP_MPEG_1_2_CHANNEL_MONO:
                    LOG_DEBUG("MONO");
                    break;
                case A2DP_MPEG_1_2_CHANNEL_STEREO:
                    LOG_DEBUG("STEREO");
                    break;
                case A2DP_MPEG_1_2_CHANNEL_DUAL:
                    LOG_DEBUG("DUAL");
                    break;
                case A2DP_MPEG_1_2_CHANNEL_JOINT_STEREO:
                    LOG_DEBUG("JOINT_STEREO");
                    break;
                default:
                    LOG_DEBUG("?????");
                    break;
                }
                LOG_DEBUG("\n");

                /* Decode Media Payload Format */
                LOG_DEBUG("\t    -> MPF = ");
                if(codec_ie[1U]& 0x40U)
                {
                    LOG_DEBUG("MPF-2");
                }
                else
                {
                    LOG_DEBUG("MPF-1");
                }
                LOG_DEBUG("\n");

                /* Decode Sampling Frequency */
                LOG_DEBUG("\t    -> Sampling Frequency = ");
                switch(codec_ie[1U]& 0x3FU)
                {
                case A2DP_MPEG_1_2_SAMPLING_FREQUENCY_16000:
                    LOG_DEBUG ("16000");
                    break;

                case A2DP_MPEG_1_2_SAMPLING_FREQUENCY_22050:
                    LOG_DEBUG ("22050");
                    break;

                case A2DP_MPEG_1_2_SAMPLING_FREQUENCY_24000:
                    LOG_DEBUG ("24000");
                    break;

                case A2DP_MPEG_1_2_SAMPLING_FREQUENCY_32000:
                    LOG_DEBUG ("32000");
                    break;

                case A2DP_MPEG_1_2_SAMPLING_FREQUENCY_44100:
                    LOG_DEBUG ("44100");
                    break;

                case A2DP_MPEG_1_2_SAMPLING_FREQUENCY_48000:
                    LOG_DEBUG ("48000");
                    break;

                default:
                    LOG_DEBUG("?????");
                    break;
                }
                LOG_DEBUG("\n");

                /* Decode Variable Bit Rate */
                LOG_DEBUG("\t    -> VBR = ");
                if (codec_ie[2U]& 0x80U)
                {
                    LOG_DEBUG("ON");
                }
                else
                {
                    LOG_DEBUG("OFF");
                }
                LOG_DEBUG("\n");

                /* Decode Bit Rate Index */
                bit_rate_index = codec_ie[2U] & 0x7FU;
                bit_rate_index = (bit_rate_index << 8U) | codec_ie[3U];

                LOG_DEBUG("\t    -> Bit Rate Index = ");
                switch(bit_rate_index)
                {
                case A2DP_MPEG_1_2_BITRATE_INDEX_0:
                    LOG_DEBUG("0");
                    break;
                case A2DP_MPEG_1_2_BITRATE_INDEX_1:
                    LOG_DEBUG("1");
                    break;
                case A2DP_MPEG_1_2_BITRATE_INDEX_2:
                    LOG_DEBUG("2");
                    break;
                case A2DP_MPEG_1_2_BITRATE_INDEX_3:
                    LOG_DEBUG("3");
                    break;
                case A2DP_MPEG_1_2_BITRATE_INDEX_4:
                    LOG_DEBUG("4");
                    break;
                case A2DP_MPEG_1_2_BITRATE_INDEX_5:
                    LOG_DEBUG("5");
                    break;
                case A2DP_MPEG_1_2_BITRATE_INDEX_6:
                    LOG_DEBUG("6");
                    break;
                case A2DP_MPEG_1_2_BITRATE_INDEX_7:
                    LOG_DEBUG("7");
                    break;
                case A2DP_MPEG_1_2_BITRATE_INDEX_8:
                    LOG_DEBUG("8");
                    break;
                case A2DP_MPEG_1_2_BITRATE_INDEX_9:
                    LOG_DEBUG("9");
                    break;
                case A2DP_MPEG_1_2_BITRATE_INDEX_10:
                    LOG_DEBUG("10");
                    break;
                case A2DP_MPEG_1_2_BITRATE_INDEX_11:
                    LOG_DEBUG("11");
                    break;
                case A2DP_MPEG_1_2_BITRATE_INDEX_12:
                    LOG_DEBUG("12");
                    break;
                case A2DP_MPEG_1_2_BITRATE_INDEX_13:
                    LOG_DEBUG("13");
                    break;
                case A2DP_MPEG_1_2_BITRATE_INDEX_14:
                    LOG_DEBUG("14");
                    break;
                default:
                    LOG_DEBUG("?????");
                    break;
                }
                LOG_DEBUG("\n");
            }
            else
            {
                LOG_DEBUG("*** A2DP_CONFIGURE_IND: No Event Data/Data Length\n");
            }
        }
#endif /* A2DP_MPEG_1_2 */

#ifdef A2DP_MPEG_2_4
        else if (AVDTP_CODEC_AUDIO_MPEG_2_4 == codec_type)
        {
            UINT16 sampling_rate;
            UINT32 bit_rate;

            LOG_DEBUG("Code Type: MPEG-2,4\n");

            if (A2DP_MPEG_2_4_CODEC_CAP_IE_LEN == codec_ie_len)
            {
                LOG_DEBUG("\tMPEG-2,4 Codec IE = 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n",
                codec_ie[0U], codec_ie[1U], codec_ie[2U], codec_ie[3U],
                codec_ie[4U], codec_ie[5U]);

                /* Decode MPEG Object Type */
                LOG_DEBUG("\t    -> Object Type = ");
                switch(codec_ie[0U]& 0xF0U)
                {
                case A2DP_MPEG_2_4_OBJ_TYPE_MPEG2_AAC_LC:
                    LOG_DEBUG("MPEG-2 AAC LC");
                    break;

                case A2DP_MPEG_2_4_OBJ_TYPE_MPEG4_AAC_LC:
                    LOG_DEBUG("MPEG-4 AAC LC");
                    break;

                case A2DP_MPEG_2_4_OBJ_TYPE_MPEG4_AAC_LTP:
                    LOG_DEBUG("MPEG-4 AAC LTP");
                    break;

                case A2DP_MPEG_2_4_OBJ_TYPE_MPEG4_AAC_SCALABLE:
                    LOG_DEBUG("MPEG-4 AAC SCALABLE");
                    break;

                default:
                    LOG_DEBUG("?????");
                    break;
                }
                LOG_DEBUG("\n");

                sampling_rate = (UINT16 )codec_ie[1U];
                sampling_rate = ((sampling_rate << 8U) | (codec_ie[2U] & 0xF0U));

                /* Decode Sampling Frequency */
                LOG_DEBUG("\t    -> Sampling Frequency = ");
                switch(sampling_rate)
                {
                case A2DP_MPEG_2_4_SAMPLING_FREQUENCY_8000:
                    LOG_DEBUG ("8000");
                    break;
                case A2DP_MPEG_2_4_SAMPLING_FREQUENCY_11025:
                    LOG_DEBUG ("11025");
                    break;

                case A2DP_MPEG_2_4_SAMPLING_FREQUENCY_12000:
                    LOG_DEBUG ("12000");
                    break;

                case A2DP_MPEG_2_4_SAMPLING_FREQUENCY_16000:
                    LOG_DEBUG ("16000");
                    break;

                case A2DP_MPEG_2_4_SAMPLING_FREQUENCY_22050:
                    LOG_DEBUG ("22050");
                    break;

                case A2DP_MPEG_2_4_SAMPLING_FREQUENCY_24000:
                    LOG_DEBUG ("24000");
                    break;

                case A2DP_MPEG_2_4_SAMPLING_FREQUENCY_32000:
                    LOG_DEBUG ("32000");
                    break;

                case A2DP_MPEG_2_4_SAMPLING_FREQUENCY_44100:
                    LOG_DEBUG ("44100");
                    break;

                case A2DP_MPEG_2_4_SAMPLING_FREQUENCY_48000:
                    LOG_DEBUG ("48000");
                    break;

                case A2DP_MPEG_2_4_SAMPLING_FREQUENCY_64000:
                    LOG_DEBUG ("64000");
                    break;

                case A2DP_MPEG_2_4_SAMPLING_FREQUENCY_88200:
                    LOG_DEBUG ("88200");
                    break;

                case A2DP_MPEG_2_4_SAMPLING_FREQUENCY_96000:
                    LOG_DEBUG ("96000");
                    break;

                default:
                    LOG_DEBUG("?????");
                    break;
                }

                LOG_DEBUG("\n");

                /* Decode Channels */
                LOG_DEBUG("\t    -> Channels = ");
                switch(codec_ie[2U]& 0x0CU)
                {
                case A2DP_MPEG_2_4_CHANNELS_1:
                    LOG_DEBUG("1");
                    break;
                case A2DP_MPEG_2_4_CHANNELS_2:
                    LOG_DEBUG("2");
                    break;
                default:
                    LOG_DEBUG("?????");
                    break;
                }
                LOG_DEBUG("\n");

                /* Decode Variable Bit Rate */
                LOG_DEBUG("\t    -> VBR = ");
                if (codec_ie[3U]& 0x80U)
                {
                    LOG_DEBUG("ON");
                }
                else
                {
                    LOG_DEBUG("OFF");
                }

                LOG_DEBUG("\n");

                /* Decode Bit Rate */
                bit_rate = 0U;
                bit_rate = codec_ie[3U] & 0x7FU;
                bit_rate = (bit_rate << 8U) | codec_ie[4U];
                bit_rate = (bit_rate << 8U) | codec_ie[5U];

                LOG_DEBUG("\t    -> Bit Rate = ");
                if (bit_rate)
                {
                    LOG_DEBUG ("%d", bit_rate);
                }
                else
                {
                    LOG_DEBUG("?????");
                }
                LOG_DEBUG("\n");
            }
            else
            {
                LOG_DEBUG("*** A2DP_CONFIGURE_IND: No Event Data/Data Length\n");
            }
        }
#endif /* A2DP_MPEG_2_4 */
#ifdef A2DP_VENDOR_CODEC
        else if (AVDTP_CODEC_AUDIO_VENDOR_SPECIFIC == codec_type)
        {
            UINT32 vendor_id;
            UINT16 vendor_codec_id, rem_ie_len;
            UCHAR  i;

            LOG_DEBUG("Code Type: Vendor Specific\n");

            /* Length should be greater than 6 */
            if (codec_ie_len > 6U)
            {
                BT_UNPACK_LE_4_BYTE(&vendor_id, &codec_ie[0U]);
                LOG_DEBUG ("\t    -> Vendor ID: 0x%08X\n", vendor_id);

                BT_UNPACK_LE_2_BYTE(&vendor_codec_id, &codec_ie[4U]);
                LOG_DEBUG ("\t    -> Vendor Codec ID: 0x%04X\n", vendor_codec_id);

                rem_ie_len = (codec_ie_len - 6U);

                LOG_DEBUG ("\t    -> Vendor Codec Value(in Hex): ");
                if (0x00U != rem_ie_len)
                {
                    for (i = 0U; i < rem_ie_len; i++)
                    {
                        LOG_DEBUG ("0x%02X\n", codec_ie[6U+i]);
                    }
                    LOG_DEBUG ("\n");
                }
                else
                {
                    LOG_DEBUG ("???\n");
                }

                /**
                 * Since Vendor Specific Codec is NOT part of A2DP specification,
                 * application has to take core Codec Config parameter validation.
                 * If it doesn't match as per requirement, application has to take
                 * appropriate action, like initiate disconnection etc.
                 */
                LOG_DEBUG ("Note:Application has " \
                 "to take care of the validation of Codec config parameters\n\n");
            }
            else
            {
                LOG_DEBUG("*** A2DP_CONFIGURE_IND: No Event Data/Data Length\n");
            }
        }
#endif /* A2DP_VENDOR_CODEC */
    }

    return;
}

void appl_a2dp_print_codec_ie_capabilities
      (
          /* IN */  UCHAR   codec_type,
          /* IN */  UCHAR * codec_ie,
          /* IN */  UINT16  codec_ie_len
      )
{
    if (NULL == codec_ie)
    {
        LOG_DEBUG ("NULL Codec IE\n");

        /* return; */
    }
    else
    {
        if (AVDTP_CODEC_AUDIO_SBC == codec_type)
        {
            LOG_DEBUG("\tCode Type: SBC\n");

            if (A2DP_SBC_CODEC_CAP_IE_LEN == codec_ie_len)
            {
                LOG_DEBUG("\tSBC Codec Capability IE = 0x%02X 0x%02X 0x%02X 0x%02X\n",
                codec_ie[0U], codec_ie[1U], codec_ie[2U], codec_ie[3U]);

                /* Decode Support for Sampling Frequency */
                LOG_DEBUG("\t    -> Sampling Frequency = ");
                if (0U != (codec_ie[0U] & A2DP_SBC_SAMPLING_FREQUENCY_16000))
                {
                    LOG_DEBUG("16000 ");
                }
                if (0U != (codec_ie[0U] & A2DP_SBC_SAMPLING_FREQUENCY_32000))
                {
                    LOG_DEBUG("32000 ");
                }
                if (0U != (codec_ie[0U] & A2DP_SBC_SAMPLING_FREQUENCY_44100))
                {
                    LOG_DEBUG("44100 ");
                }
                if (0U != (codec_ie[0U] & A2DP_SBC_SAMPLING_FREQUENCY_48000))
                {
                    LOG_DEBUG("48000");
                }
                LOG_DEBUG("\n");

                /* Decode Support for Channel Mode */
                LOG_DEBUG("\t    -> Channel Modes = ");
                if (0U != (codec_ie[0U] & A2DP_SBC_CHANNEL_MONO))
                {
                    LOG_DEBUG("Mono ");
                }
                if (0U != (codec_ie[0U] & A2DP_SBC_CHANNEL_DUAL))
                {
                    LOG_DEBUG("Dual ");
                }
                if (0U != (codec_ie[0U] & A2DP_SBC_CHANNEL_STEREO))
                {
                    LOG_DEBUG("Stereo ");
                }
                if (0U != (codec_ie[0U] & A2DP_SBC_CHANNEL_JOINT_STEREO))
                {
                    LOG_DEBUG("Joint-Stereo");
                }
                LOG_DEBUG("\n");

                 /* Decode Support for Block Length */
                LOG_DEBUG("\t    -> Block Length = ");
                if (0U != (codec_ie[1U] & A2DP_SBC_BLOCK_LENGTH_4))
                {
                    LOG_DEBUG("4 ");
                }
                if (0U != (codec_ie[1U] & A2DP_SBC_BLOCK_LENGTH_8))
                {
                    LOG_DEBUG("8 ");
                }
                if (0U != (codec_ie[1U] & A2DP_SBC_BLOCK_LENGTH_12))
                {
                    LOG_DEBUG("12 ");
                }
                if (0U != (codec_ie[1U] & A2DP_SBC_BLOCK_LENGTH_16))
                {
                    LOG_DEBUG("16");
                }
                LOG_DEBUG("\n");

                /* Decode Support for Subbands */
                LOG_DEBUG("\t    -> Subbands = ");
                if (0U != (codec_ie[1U] & A2DP_SBC_SUBBANDS_4))
                {
                    LOG_DEBUG("4 ");
                }
                if (0U != (codec_ie[1U] & A2DP_SBC_SUBBANDS_8))
                {
                    LOG_DEBUG("8");
                }
                LOG_DEBUG("\n");

                /* Decode Support for Allocation Method */
                LOG_DEBUG("\t    -> Allocation Method = ");
                if (0U != (codec_ie[1U] & A2DP_SBC_ALLOCATION_METHOD_SNR))
                {
                    LOG_DEBUG("SNR ");
                }
                if (0U != (codec_ie[1U] & A2DP_SBC_ALLOCATION_METHOD_LOUDNESS))
                {
                    LOG_DEBUG("Loudness");
                }
                LOG_DEBUG("\n");

                /* Decode Support for Min/Max Bitpools */
                LOG_DEBUG("\t    -> Bitpool Range = %d - %d\n",
                codec_ie[2U], codec_ie[3U]);
            }
            else
            {
                LOG_DEBUG("*** A2DP_CAPABILITY_CNF: No Event Data/Data Length\n");
            }
        }
#ifdef A2DP_MPEG_1_2
        else if (AVDTP_CODEC_AUDIO_MPEG_1_2 == codec_type)
        {
            UINT16 bit_rate_index;

            LOG_DEBUG("\tCode Type: MPEG-1,2\n");

            if (A2DP_MPEG_1_2_CODEC_CAP_IE_LEN == codec_ie_len)
            {
                LOG_DEBUG("\tMPEG-1,2 Codec Capability IE = 0x%02X 0x%02X 0x%02X 0x%02X\n",
                codec_ie[0U], codec_ie[1U], codec_ie[2U], codec_ie[3U]);

                /* Decode MPEG Layer */
                LOG_DEBUG("\t    -> Layer = ");
                if (0U != (codec_ie[0U]& A2DP_MPEG_1_2_LAYER_I))
                {
                    LOG_DEBUG("I ");
                }
                if (0U != (codec_ie[0U]& A2DP_MPEG_1_2_LAYER_II))
                {
                    LOG_DEBUG("II ");
                }
                if (0U != (codec_ie[0U]& A2DP_MPEG_1_2_LAYER_III))
                {
                    LOG_DEBUG("III ");
                }
                LOG_DEBUG("\n");

                /* Decode CRC */
                LOG_DEBUG("\t    -> CRC = ");
                if (0U != (codec_ie[0U]& 0x10U))
                {
                    LOG_DEBUG("ON");
                }
                else
                {
                    LOG_DEBUG("OFF");
                }
                LOG_DEBUG("\n");

                /* Decode Channel Mode */
                LOG_DEBUG("\t    -> Channel Mode = ");
                if (0U != (codec_ie[0U] & A2DP_MPEG_1_2_CHANNEL_MONO))
                {
                    LOG_DEBUG("Mono ");
                }
                if (0U != (codec_ie[0U] & A2DP_MPEG_1_2_CHANNEL_STEREO))
                {
                    LOG_DEBUG("Strereo ");
                }
                if (0U != (codec_ie[0U] & A2DP_MPEG_1_2_CHANNEL_DUAL))
                {
                    LOG_DEBUG("Dual ");
                }
                if (0U != (codec_ie[0U] & A2DP_MPEG_1_2_CHANNEL_JOINT_STEREO))
                {
                    LOG_DEBUG("Joint Stereo ");
                }
                LOG_DEBUG ("\n");

                /* Decode Media Payload Format */
                LOG_DEBUG("\t    -> MPF = ");
                if(0U != (codec_ie[1U]& 0x40U))
                {
                    LOG_DEBUG("MPF-2");
                }
                else
                {
                    LOG_DEBUG("MPF-1");
                }
                LOG_DEBUG("\n");

                /* Decode Sampling Frequency */
                LOG_DEBUG("\t    -> Sampling Frequency = ");
                if (0U != (codec_ie[1U] & A2DP_MPEG_1_2_SAMPLING_FREQUENCY_16000))
                {
                    LOG_DEBUG ("16000 ");
                }
                if (0U != (codec_ie[1U] & A2DP_MPEG_1_2_SAMPLING_FREQUENCY_22050))
                {
                    LOG_DEBUG ("22050 ");
                }
                if (0U != (codec_ie[1U] & A2DP_MPEG_1_2_SAMPLING_FREQUENCY_24000))
                {
                    LOG_DEBUG ("24000 ");
                }
                if (0U != (codec_ie[1U] & A2DP_MPEG_1_2_SAMPLING_FREQUENCY_32000))
                {
                    LOG_DEBUG ("32000 ");
                }
                if (0U != (codec_ie[1U] & A2DP_MPEG_1_2_SAMPLING_FREQUENCY_44100))
                {
                    LOG_DEBUG ("44100 ");
                }
                if (0U != (codec_ie[1U] & A2DP_MPEG_1_2_SAMPLING_FREQUENCY_48000))
                {
                    LOG_DEBUG ("48000");
                }
                LOG_DEBUG ("\n");

                /* Decode Variable Bit Rate */
                LOG_DEBUG("\t    -> VBR = ");
                if (0U != (codec_ie[2U]& 0x80U))
                {
                    LOG_DEBUG("ON");
                }
                else
                {
                    LOG_DEBUG("OFF");
                }
                LOG_DEBUG("\n");

                /* Decode Bit Rate Index */
                bit_rate_index = codec_ie[2U] & 0x7FU;
                bit_rate_index = (bit_rate_index << 8U) | codec_ie[3U];

                LOG_DEBUG("\t    -> Bit Rate Index = 0x%04x\n", bit_rate_index);
            }
            else
            {
                LOG_DEBUG("*** A2DP_CAPABILITY_CNF: No Event Data/Data Length\n");
            }
        }
#endif /* A2DP_MPEG_1_2 */

#ifdef A2DP_MPEG_2_4
        else if (AVDTP_CODEC_AUDIO_MPEG_2_4 == codec_type)
        {
            UINT16 sampling_rate;
            UINT32 bit_rate;

            LOG_DEBUG("\tCode Type: MPEG-2,4\n");

            if (A2DP_MPEG_2_4_CODEC_CAP_IE_LEN == codec_ie_len)
            {
                LOG_DEBUG("\tMPEG-2,4 Codec IE = 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n",
                codec_ie[0U], codec_ie[1U], codec_ie[2U], codec_ie[3U],
                codec_ie[4U], codec_ie[5U]);

                /* Decode MPEG Object Type */
                LOG_DEBUG("\t    -> Object Type = ");
                if(0U != (codec_ie[0U] & A2DP_MPEG_2_4_OBJ_TYPE_MPEG2_AAC_LC))
                {
                    LOG_DEBUG("MPEG-2 AAC LC ");
                }
                if(0U != (codec_ie[0U] & A2DP_MPEG_2_4_OBJ_TYPE_MPEG4_AAC_LC))
                {
                    LOG_DEBUG("MPEG-4 AAC LC ");
                }
                if(0U != (codec_ie[0U] & A2DP_MPEG_2_4_OBJ_TYPE_MPEG4_AAC_LTP))
                {
                    LOG_DEBUG("MPEG-4 AAC LTP ");
                }
                if(0U != (codec_ie[0U] & A2DP_MPEG_2_4_OBJ_TYPE_MPEG4_AAC_SCALABLE))
                {
                    LOG_DEBUG("MPEG-4 AAC SCALABLE");
                }
                LOG_DEBUG("\n");

                sampling_rate = (UINT16 )codec_ie[1U];
                sampling_rate = ((sampling_rate << 8U) | (codec_ie[2U] & 0xF0U));

                /* Decode Sampling Frequency */
                LOG_DEBUG("\t    -> Sampling Frequency = ");
                if (0U != (sampling_rate & A2DP_MPEG_2_4_SAMPLING_FREQUENCY_8000))
                {
                    LOG_DEBUG ("8000 ");
                }
                if (0U != (sampling_rate & A2DP_MPEG_2_4_SAMPLING_FREQUENCY_11025))
                {
                    LOG_DEBUG ("11025 ");
                }
                if (0U != (sampling_rate & A2DP_MPEG_2_4_SAMPLING_FREQUENCY_12000))
                {
                    LOG_DEBUG ("12000 ");
                }
                if (0U != (sampling_rate & A2DP_MPEG_2_4_SAMPLING_FREQUENCY_16000))
                {
                    LOG_DEBUG ("16000 ");
                }
                if (0U != (sampling_rate & A2DP_MPEG_2_4_SAMPLING_FREQUENCY_22050))
                {
                    LOG_DEBUG ("22050 ");
                }
                if (0U != (sampling_rate & A2DP_MPEG_2_4_SAMPLING_FREQUENCY_24000))
                {
                    LOG_DEBUG ("24000 ");
                }
                if (0U != (sampling_rate & A2DP_MPEG_2_4_SAMPLING_FREQUENCY_32000))
                {
                    LOG_DEBUG ("32000 ");
                }
                if (0U != (sampling_rate & A2DP_MPEG_2_4_SAMPLING_FREQUENCY_44100))
                {
                    LOG_DEBUG ("44100 ");
                }
                if (0U != (sampling_rate & A2DP_MPEG_2_4_SAMPLING_FREQUENCY_48000))
                {
                    LOG_DEBUG ("48000 ");
                }
                if (0U != (sampling_rate & A2DP_MPEG_2_4_SAMPLING_FREQUENCY_64000))
                {
                    LOG_DEBUG ("64000 ");
                }
                if (0U != (sampling_rate & A2DP_MPEG_2_4_SAMPLING_FREQUENCY_88200))
                {
                    LOG_DEBUG ("88200 ");
                }
                if (0U != (sampling_rate & A2DP_MPEG_2_4_SAMPLING_FREQUENCY_96000))
                {
                    LOG_DEBUG ("96000");
                }
                LOG_DEBUG("\n");

                /* Decode Channels */
                LOG_DEBUG("\t    -> Channels = ");
                if (0U != (codec_ie[2U] & A2DP_MPEG_2_4_CHANNELS_1))
                {
                    LOG_DEBUG ("1 ");
                }
                if (0U != (codec_ie[2U] & A2DP_MPEG_2_4_CHANNELS_2))
                {
                    LOG_DEBUG ("2");
                }
                LOG_DEBUG ("\n");

                /* Decode Variable Bit Rate */
                LOG_DEBUG("\t    -> VBR = ");
                if (0U != (codec_ie[3U]& 0x80U))
                {
                    LOG_DEBUG("ON");
                }
                else
                {
                    LOG_DEBUG("OFF");
                }

                LOG_DEBUG("\n");

                /* Decode Bit Rate */
                bit_rate = 0U;
                bit_rate = codec_ie[3U] & 0x7FU;
                bit_rate = (bit_rate << 8U) | codec_ie[4U];
                bit_rate = (bit_rate << 8U) | codec_ie[5U];

                LOG_DEBUG("\t    -> Bit Rate = ");
                if (bit_rate)
                {
                    LOG_DEBUG ("%d", bit_rate);
                }
                else
                {
                    LOG_DEBUG("Unknown");
                }
                LOG_DEBUG("\n");
            }
            else
            {
                LOG_DEBUG("*** A2DP_CAPABILITY_CNF: No Event Data/Data Length\n");
            }
        }
#endif /* A2DP_MPEG_2_4 */
    }

    return;
}

/*
 * This function is to get SBC frame hdr details and sbc frame length from
 * A2DP SBC codec configuration parameters.
 */
API_RESULT appl_a2dp_get_sbc_frame_details
            (
                UCHAR *sbc_config_info,
                UCHAR *sbc_frame_hdr,
                UINT16 *sbc_frame_len
            )
{
    API_RESULT retval;
    UCHAR sampling_freq;
    UCHAR ch_mode, num_ch;
    UCHAR block_len;
    UCHAR num_sub_bands;
    UCHAR alloc_method;

    UINT16 frame_len;
    UINT32 factor_1, factor_2;

    /* Init */
    retval = API_SUCCESS;
    sampling_freq = 0U;
    ch_mode = 0U;
    num_ch = 0U;
    block_len = 0U;
    num_sub_bands = 0U;
    alloc_method = 0U;
    frame_len   = 0U;

    /* Validate input params */
    if ((NULL == sbc_config_info) ||
        (NULL == sbc_frame_hdr) ||
        (NULL == sbc_frame_len))
    {
        retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
        /* Get the SBC config parameters to update the SBC frame header */

        /* Frequency */
        if (0U != (sbc_config_info[0U] & A2DP_SBC_SAMPLING_FREQUENCY_16000))
        {
            sampling_freq = 0U;
        }
        else if (0U != (sbc_config_info[0U] & A2DP_SBC_SAMPLING_FREQUENCY_32000))
        {
            sampling_freq = 1U;
        }
        else if (0U != (sbc_config_info[0U] & A2DP_SBC_SAMPLING_FREQUENCY_44100))
        {
            sampling_freq = 2U;
        }
        else
        {
            sampling_freq = 3U;
        }

        /* Channel Mode */
        if (0U != (sbc_config_info[0U] & A2DP_SBC_CHANNEL_MONO))
        {
            ch_mode = 0U;
            num_ch = 1U;
        }
        else if (0U != (sbc_config_info[0U] & A2DP_SBC_CHANNEL_DUAL))
        {
            ch_mode = 1U;
            num_ch = 2U;
        }
        else if (0U != (sbc_config_info[0U] & A2DP_SBC_CHANNEL_STEREO))
        {
            ch_mode = 2U;
            num_ch = 2U;
        }
        else
        {
            ch_mode = 3U;
            num_ch = 2U;
        }

        /* Block length */
        if (0U != (sbc_config_info[1U] & A2DP_SBC_BLOCK_LENGTH_4))
        {
            block_len = 4U;
        }
        else if (0U != (sbc_config_info[1U] & A2DP_SBC_BLOCK_LENGTH_8))
        {
            block_len = 8U;
        }
        else if (0U != (sbc_config_info[1U] & A2DP_SBC_BLOCK_LENGTH_12))
        {
            block_len = 12U;
        }
        else
        {
            block_len = 16U;
        }

        /* Number of subbands */
        if (0U != (sbc_config_info[1U] & A2DP_SBC_SUBBANDS_4))
        {
            num_sub_bands = 4U;
        }
        else
        {
            num_sub_bands = 8U;
        }

        /* Allocation Method */
        if (0U != (sbc_config_info[1U] & A2DP_SBC_ALLOCATION_METHOD_SNR))
        {
            alloc_method = 0x01U;
        }

        /* Update SBC frame header with config parameters */
        sbc_frame_hdr[0U] = 0x9CU;

        /* MISRA C - 2012 Rule 2.2 */
        /* sbc_frame_hdr[1U] = 0x00U; */

        sbc_frame_hdr[1U] = ((sampling_freq << 6U) |
                            ((block_len - 4U) << 2U) |
                            (ch_mode << 2U) |
                            (alloc_method << 1U) |
                            (num_sub_bands >> 3U));

        sbc_frame_hdr[2U] = sbc_config_info[3U];

        /* Calculate SBC encoded frame length based on the config parameters */
        factor_1 = 4U + (4U * (UINT32)num_sub_bands * num_ch)/8U;
        factor_2 = (UINT32)block_len * sbc_config_info[3U];

            /* Mono & Dual */
        if ((0U == ch_mode) ||
            (1U == ch_mode))
        {
            frame_len = (UINT16)((factor_2 * num_ch) >> 3U);

            if(0U != ((factor_2 * num_ch) & 0x0007U))
            {
                frame_len++;
            }
        }

        /* Stereo */
        if (2U == ch_mode)
        {
            frame_len = (UINT16)(factor_2 >> 3U);

            if(0U != (factor_2 & 0x0007U))
            {
                frame_len++;
            }
        }

        /* Joint Stereo */
        if (3U == ch_mode)
        {
            frame_len = (UINT16)((num_sub_bands + factor_2) >> 3U);

            if(0U != ((num_sub_bands + factor_2) & 0x0007U))
            {
                frame_len++;
            }
        }

        /* Finally, add the Factor_1 */
        frame_len += (UINT16)factor_1;

        *sbc_frame_len = frame_len;
    }

    return retval;
}


void appl_a2dp_connect_info_reset (UCHAR inst)
{
    /* Check for valid inst */
    if (inst >= A2DP_MAX_CODECS)
    {
        LOG_DEBUG("Invalid A2DP instance\n");
        /* return; */
    }
    else
    {
        /* Reset BD Address */
        BT_mem_set
        (
            appl_a2dp_instance[inst].bd_addr,
            0x00,
            BT_BD_ADDR_SIZE
        );

        /* Reset Codec Configuration */
        BT_mem_set
        (
            appl_a2dp_instance[inst].codec_ie_conf,
            0x00,
            sizeof (appl_a2dp_instance[inst].codec_ie_conf)
        );

        appl_a2dp_instance[inst].stream_state = APPL_A2DP_STREAM_FALSE;
    }

    return;
}

#ifdef HAVE_AVDTP_TEST_MODE
void appl_avdtp_set_sig_msg_reject_rsp_params(void)
{
    int         read_val;
    UCHAR       sig_id;
    UCHAR       error_code;
    API_RESULT  retval;

    retval = API_SUCCESS;

    LOG_DEBUG ("Enter AVDTP Signalling ID(in Hex): ");
    scanf ("%x", &read_val);
    sig_id = (UCHAR)read_val;

    LOG_DEBUG ("Enter Error Code(in Hex): ");
    scanf ("%x", &read_val);
    error_code = (UCHAR)read_val;

    retval = BT_avdtp_set_sig_msg_reject_rsp_params
             (
                 sig_id,
                 error_code,
                 0x01U
             );
    if(API_SUCCESS != retval)
    {
        LOG_DEBUG ("BT_avdtp_set_sig_msg_reject_rsp_params Failed:(0x%04x)\n", retval);
    }

    return;
}
#endif /* HAVE_AVDTP_TEST_MODE */

#ifdef A2DP_SINK
static void appl_a2dp_jpl_configure(UCHAR* codec_ie)
{
    API_RESULT retval;
#ifdef INCLUDE_SBC
    SBC_CODEC_PARAM * codec_param;
    UINT16 wav_sf;

    /* Init */
    retval = API_SUCCESS;
    codec_param = &jpl_param.sbc_param;
    wav_sf = 0U;

    /* Get the sampling Frequency */
    switch (codec_ie[0U] & 0xF0U)
    {
    case A2DP_SBC_SAMPLING_FREQUENCY_16000:
        codec_param->sampling_frequency = 0U;
        wav_sf = 16000U;
        break;

    case A2DP_SBC_SAMPLING_FREQUENCY_32000:
        codec_param->sampling_frequency = 1U;
        wav_sf = 32000U;
        break;

    case A2DP_SBC_SAMPLING_FREQUENCY_44100:
        codec_param->sampling_frequency = 2U;
        wav_sf = 44100U;
        break;

    case A2DP_SBC_SAMPLING_FREQUENCY_48000:
        codec_param->sampling_frequency = 3U;
        wav_sf = 48000U;
        break;

    default:
        /* This should not be reached */
        LOG_DEBUG("[APPL] Invalid sampling frequency\r\n");
        retval = API_FAILURE; /* return; */
    }

    if (API_SUCCESS == retval)
    {
        /* Get the Channel Mode */
        switch (codec_ie[0U] & 0x0FU)
        {
        case A2DP_SBC_CHANNEL_MONO:
            codec_param->channel_mode = SBC_CHANNEL_MONO;
            codec_param->nrof_channels = 1U;
            break;

        case A2DP_SBC_CHANNEL_DUAL:
            codec_param->channel_mode = SBC_CHANNEL_DUAL;
            codec_param->nrof_channels = 2U;
            break;

        case A2DP_SBC_CHANNEL_STEREO:
            codec_param->channel_mode = SBC_CHANNEL_STEREO;
            codec_param->nrof_channels = 2U;
            break;

        case A2DP_SBC_CHANNEL_JOINT_STEREO:
            codec_param->channel_mode = SBC_CHANNEL_JOINT_STEREO;
            codec_param->nrof_channels = 2U;
            break;

        default:
            /* This should not be reached */
            LOG_DEBUG("[APPL] Invalid Channel mode\r\n");
            retval = API_FAILURE; /* return; */
        }
    }

    if (API_SUCCESS == retval)
    {
        /* Get the Block Length */
        switch (codec_ie[1U] & 0xF0U)
        {
        case A2DP_SBC_BLOCK_LENGTH_4:
            codec_param->nrof_blocks = SBC_BLOCK_SIZE_4;
            break;

        case A2DP_SBC_BLOCK_LENGTH_8:
            codec_param->nrof_blocks = SBC_BLOCK_SIZE_8;
            break;

        case A2DP_SBC_BLOCK_LENGTH_12:
            codec_param->nrof_blocks = SBC_BLOCK_SIZE_12;
            break;

        case A2DP_SBC_BLOCK_LENGTH_16:
            codec_param->nrof_blocks = SBC_BLOCK_SIZE_16;
            break;

        default:
            /* This should not be reached */
            LOG_DEBUG("[APPL] Invalid Block Length\r\n");
            retval = API_FAILURE; /* return; */
        }
    }

    if (API_SUCCESS == retval)
    {
        /* Get the Subbands */
        switch (codec_ie[1U] & 0x0CU)
        {
        case A2DP_SBC_SUBBANDS_4:
            codec_param->nrof_subbands = SBC_SUBBAND_4;
            break;

        case A2DP_SBC_SUBBANDS_8:
            codec_param->nrof_subbands = SBC_SUBBAND_8;
            break;

        default:
            /* This should not be reached */
            LOG_DEBUG("[APPL] Invalid Subband\r\n");
            retval = API_FAILURE; /* return; */
        }
    }

    if (API_SUCCESS == retval)
    {
        /* Get the Allocation Method */
        switch (codec_ie[1U] & 0x03U)
        {
        case A2DP_SBC_ALLOCATION_METHOD_SNR:
            codec_param->allocation_method = SBC_SNR;
            break;

        case A2DP_SBC_ALLOCATION_METHOD_LOUDNESS:
            codec_param->allocation_method = SBC_LOUDNESS;
            break;

        default:
            /* This should not be reached */
            LOG_DEBUG("[APPL] Invalid Allocation method\r\n");
            retval = API_FAILURE; /* return; */
        }
    }

    if (API_SUCCESS == retval)
    {
        /* Set Bitpool */
        codec_param->bitpool = codec_ie[3U];

        jpl_param.pcm_num_frames =
            ((3U * (SBC_MAX_BLOCK_SIZE * SBC_MAX_SUBBAND)) /
            (codec_param->nrof_blocks * codec_param->nrof_subbands));

        jpl_param.pod_frames =
            ((18U * (SBC_MAX_BLOCK_SIZE * SBC_MAX_SUBBAND)) /
            (codec_param->nrof_blocks * codec_param->nrof_subbands));

        LOG_DEBUG("[APPL] A2DP Sink Configured\r\n");

        /* Init the Pl */
        a2dp_init_pl(A2DP_EP_SINK);

        /* Setup the pl */
        if (0x01U == codec_param->nrof_channels)
        {
            /* Mono Channel*/
            a2dp_setup_pl
            (
                A2DP_EP_SINK,
                a2dp_pl_snk_callback,
                wav_sf,
                16U,
                codec_param->nrof_channels,
                768U
            );
        }
        else
        {
            /* Stereo Channel */
            a2dp_setup_pl
            (
                A2DP_EP_SINK,
                a2dp_pl_snk_callback,
                wav_sf,
                16U,
                codec_param->nrof_channels,
                1536U
            );
        }
    }
#else /* INCLUDE_SBC */
    BT_IGNORE_UNUSED_PARAM(codec_ie);

    /* Init */
    retval = API_SUCCESS;

#endif /* INCLUDE_SBC */
    return;
}

#ifdef INCLUDE_JPL
static API_RESULT jpl_callback_handle
       (
           /* IN */ UCHAR   event_type,
           /* IN */ UCHAR * event_data,
           /* IN */ UINT16  event_datalen
       )
{
    BT_IGNORE_UNUSED_PARAM(event_type);

    if ((JPL_UNDERFLOW_IND == event_type) ||
        (JPL_SILENCE_DATA_IND == event_type))
    {
        LOG_DEBUG("U ");
    }
#if 0
    else
    {
        LOG_DEBUG("~ ");
    }
#endif /* 0 */

    appl_a2dp_jpl_pof = event_data;
    appl_a2dp_jpl_poflen = event_datalen;

    a2dp_write_pl(A2DP_EP_SINK, event_data, event_datalen);

    return API_SUCCESS;
}
#endif /* INCLUDE_JPL */

static void a2dp_pl_snk_callback
       (
           /* IN */ const UCHAR *  p_data,
           /* IN */ UINT16         p_datalen
       )
{
    API_RESULT retval;

    BT_IGNORE_UNUSED_PARAM(p_data);
    BT_IGNORE_UNUSED_PARAM(p_datalen);

    /* LOG_DEBUG("* "); */

    /* do */
    {
#ifdef INCLUDE_SBC
        retval = BT_jpl_remove_frames(appl_a2dp_jpl_pof, appl_a2dp_jpl_poflen);
#endif /* INCLUDE_SBC */
    }/* while (JPL_ERR_MORE_PCM_FRAMES_AVAILABLE == retval); */

    BT_IGNORE_UNUSED_PARAM(retval);
}
#endif /* A2DP_SINK */
#endif /* A2DP */

